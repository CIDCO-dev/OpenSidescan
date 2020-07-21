#include "monitorthread.h"

#include <iostream>
#include <QStringList>
#include <QString>
#include <QMap>
#include <sys/stat.h>
#include "../../src/thirdParty/MBES-lib/src/datagrams/DatagramParserFactory.hpp"
#include "../../src/thirdParty/MBES-lib/src/datagrams/DatagramParser.hpp"
#include "sidescan/sidescanimager.h"



#if defined(_WIN32)

#include <windows.h>
#include <stdio.h>

#else
#if defined(__linux) || defined(__linux__) || defined(linux)

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>   // For flock structure



struct flock flockStruct;

#endif
#endif

MonitorThread::MonitorThread(QDir directory, Project * project, Detector * detector) : directory(directory), project(project), detector(detector)
{

}

MonitorThread::~MonitorThread(){

}

void MonitorThread::process(){

    //QDir directory("/home/jordan/workspaceOpenSideScan/OpenSidescan/test/data/wrecks");
    QMap<QString, QString> monitoredFiles;
    QMap<QString, QString> scannedFiles;
    QMap<QString, qint64> fileSizes;

    int count=0;

    while(!exterminate){
        QThread::sleep(3);
        QStringList xtfFiles = directory.entryList(QStringList() << "*.xtf", QDir::Files);

        foreach (QString filename, xtfFiles) {
            QString absoPath(directory.absolutePath() + "/" + filename);
            std::string fileNameStr = absoPath.toStdString();

            if(scannedFiles.contains(filename)) {
                //Do nothing, already scanned
                //File could have been removed from project by hydrographer so don't add it again
            } else if(project->containsFile(fileNameStr)) {
                //Already in project, this happens if monitor is stopped and started again
                scannedFiles.insert(filename, filename);
            } else {

                //This assumes that the loging software obtains a lock on the file when it writes
                //If it doesn't, then there will be an error since it will load an unfinished file...
                //Perhaps check to file size to see if it is stable for a few iterations?
                if(fileCanBeChecked(absoPath.toStdString())) {
                    QFile xtfQFile(absoPath);

                    if(xtfQFile.size() > 0) {
                        // add it to local table to remember it is scanned
                        scannedFiles.insert(filename, filename);


                        emit updateProgress(QString("Loading file: ") + filename);
                        SidescanImager imager;
                        DatagramParser * parser = NULL;

                        parser = DatagramParserFactory::build(fileNameStr,imager);
                        parser->parse(fileNameStr);


                        emit updateProgress(QString("Generating image for: ") + filename);
                        SidescanFile * file = imager.generate(fileNameStr, project->getAntenna2TowPointLeverArm());

                        emit updateProgress(QString("Detecting objects in: ") + filename);
                        for(auto j=file->getImages().begin();j != file->getImages().end();j++){
                            detector->detect(**j,(*j)->getObjects());
                        }

                        // Done loading and detecting objects, add it to project
                        emit fileToBeAddedToProjectWindow( file );


                        emit updateProgress(QString("Monitoring directory for new sidescan files..."));
                    } else {
                        //File is empty, nothing has been written into it yet.
                        //wait for next pass
                    }

                } else {
                    //Can't obtain lock, do nothing for now
                    //wait for next pass
                }

            }
        }
    }

}

void MonitorThread::start(){

    //show window to set up detector and folder










    this->moveToThread(&thread);
    //connect(this, SIGNAL (error(QString)), this, SLOT (errorString(QString)));
    connect(&thread, &QThread::started, this, &MonitorThread::process);
    //connect(this, &MonitorThread::finished()), &thread, SLOT (quit()));
    //connect(this, SIGNAL (finished()), this, SLOT (deleteLater()));
    //connect(&thread, SIGNAL (finished()), &thread, SLOT (deleteLater()));
    exterminate = false;
    thread.start();
}

void MonitorThread::stop(){
    exterminate = true;
    thread.quit();
    thread.wait();
}

#if defined(_WIN32)

bool MonitorThread::fileCanBeChecked(std::string filename) {
    // Code modified from
    //https://docs.microsoft.com/en-us/windows/win32/fileio/locking-and-unlocking-byte-ranges-in-files

    // Reference
    //https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea
    //https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-lockfileex
    //https://docs.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-overlapped
    //https://docs.microsoft.com/en-us/windows/win32/debug/system-error-codes


    LPCSTR lpFileName = filename.c_str();

    BOOL fSuccess = FALSE;

    // Open existing file for read
    HANDLE hFile = CreateFileA(lpFileName,
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL, // no security attributes
            OPEN_EXISTING,
            0,
            NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        return false;
    }

    // Set up the Overlapped structure with the file offset
    // required by LockFileEx.
    OVERLAPPED sOverlapped;
    sOverlapped.Offset = 0;
    sOverlapped.OffsetHigh = 0;

    // Try and get a lock on the file.
    // Specify exclusive access, and fail
    // immediately if the lock cannot be obtained.
    fSuccess = LockFileEx(hFile, // exclusive access,
            LOCKFILE_EXCLUSIVE_LOCK |
            LOCKFILE_FAIL_IMMEDIATELY,
            0, // reserved, must be zero
            MAXDWORD, // number of bytes to lock: lock the entire file
            MAXDWORD,
            &sOverlapped); // contains the file offset

    if (!fSuccess) {
        CloseHandle(hFile);

        return false;
    }


    // Unlock the file.
    fSuccess = UnlockFileEx(hFile,
            0, // reserved, must be zero
            MAXDWORD, // num. of bytes to unlock
            MAXDWORD,
            &sOverlapped); // contains the file offset
    if (!fSuccess) {
        CloseHandle(hFile);

        return false;
    }

    // Clean up handles, memory, and the created file.
    fSuccess = CloseHandle(hFile);

    if (!fSuccess) {
        return false;
    }

    return true;
}

#else
#if defined(__linux) || defined(__linux__) || defined(linux)

bool MonitorThread::fileCanBeChecked(std::string filename) {
    // https://linux.die.net/man/2/fcntl

    int fd = open(filename.c_str(), O_RDWR | O_NOATIME);

    if (fd == -1)
        return false;

    flockStruct.l_type = F_WRLCK;

    if (fcntl(fd, F_SETLK, &flockStruct) == -1) // If a lock already set, does not wait
    {
        return false;
    }

    flockStruct.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &flockStruct); // Release the lock

    close(fd);

    return true;
}

#else

bool MonitorThread::fileCanBeChecked(std::string filename) {
    return true;
}


#endif
#endif


