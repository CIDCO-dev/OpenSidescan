/*
 * Copyright 2020 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
 */

/* 
 * File:   FileLockUtils.h
 * Author: Christian Bouchard
 */

#ifndef FILELOCKUTILS_H
#define FILELOCKUTILS_H

#if defined(_WIN32)

#include <windows.h>
#include <stdio.h>

#else
#if defined(__linux) || defined(__linux__) || defined(linux)

#include <sys/types.h>
#include <fcntl.h>
#include <sys/file.h>   // For flock structure
struct flock lockStruct;

#endif
#endif

class FileLockUtils {
public:

#if defined(_WIN32)

static bool fileNotLocked(std::string & filename) {
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

static bool fileNotLocked(std::string & filename) {
    // https://linux.die.net/man/2/fcntl

    int fd = open(filename.c_str(), O_RDWR | O_NOATIME);
    

    if (fd == -1) {
        return false;
    }
    
    memset(&lockStruct, 0, sizeof(lockStruct));
    lockStruct.l_type = F_WRLCK;
    

    if (fcntl(fd, F_SETLK, &lockStruct) == -1) // If a lock already set, does not wait
    {
        return false;
    }

    lockStruct.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lockStruct); // Release the lock

    close(fd);

    return true;
}

#else

static bool fileNotLocked(std::string & filename) {
    return true;
}

#endif
#endif

};

#endif /* FILELOCKUTILS_H */

