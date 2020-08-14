/*
 * Copyright 2020 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
 */

/* 
 * File:   MonitorTest.hpp
 * Author: Jordan McManus
 */

#ifndef MONITORTEST_HPP
#define MONITORTEST_HPP

#include "catch.hpp"
#include <Eigen/Dense>
#include "../src/OpenSidescan/sidescan/sidescanfile.h"
#include "../src/OpenSidescan/sidescan/sidescanimager.h"
#include "../src/OpenSidescan/detector/roidetector.h"
#include "../src/thirdParty/MBES-lib/src/datagrams/DatagramParserFactory.hpp"
#include "../src/thirdParty/MBES-lib/src/datagrams/DatagramParser.hpp"
#include <string>
#include <map>
#include <vector>
#include <experimental/filesystem>
#include <atomic>
#include "../src/OpenSidescan/utilities/FileLockUtils.h"
#include <string>
#include <iostream>
#include <cstring>

#include <unistd.h>
#include <sys/types.h>

#if defined(_WIN32)

#include <windows.h>
#include <stdio.h>

#else
#if defined(__linux) || defined(__linux__) || defined(linux)


#include <fcntl.h>
#include <sys/file.h>   // For flock structure

#endif
#endif

//need g++ 8 to remove experimental namespace
namespace fs = std::experimental::filesystem;

class SideScanFileProcessor {
    //Sub class this to connect with OpenSideScan
public:

    void processFile(SidescanFile * f) {
        //In OpenSidescan, add file to project
        std::cout << "Processing sidescan file: " << f->getFilename() << std::endl;
        REQUIRE(false);
    }

    void reportProgress(std::string progress) {
        //In OpenSidescan, give feedback to user
        std::cout << progress << std::endl;
    }

};

class DirectoryMonitor {
public:

    DirectoryMonitor(Detector * detector, SideScanFileProcessor * processor, Eigen::Vector3d leverArm) : detector(detector), processor(processor), leverArm(leverArm) {
    };

    void monitor(std::string path) {
        for (const auto & entry : fs::directory_iterator(path)) {
            std::string filepath = entry.path().generic_string();

            if (FileLockUtils::fileNotLocked(filepath)) {
                if (isXtf(entry)) {
                    if (!alreadyScanned(filepath)) {
                        std::string filepath = entry.path().generic_string();

                        processor->reportProgress("Loading and detecting objects in file " + filepath);
                        SidescanFile * file = loadAndDetectObjects(filepath);
                        processor->processFile(file);
                        processor->reportProgress("Objects detected in file " + filepath);

                        scannedFiles.insert(std::pair<std::string, std::string>(filepath, filepath));
                    } else {
                        //File is already scanned, do nothing
                    }
                }
            } else {
                //File is locked, do nothing
                processor->reportProgress("File is locked by another program: " + filepath);
            }
        }
    }

    SidescanFile * loadAndDetectObjects(std::string & filepath) {
        SidescanImager imager;
        DatagramParser * parser = NULL;
        parser = DatagramParserFactory::build(filepath, imager);
        parser->parse(filepath);

        SidescanFile * file = imager.generate(filepath, leverArm);

        for (auto j = file->getImages().begin(); j != file->getImages().end(); j++) {
            detector->detect(**j, (*j)->getObjects());
        }

        return file;
    }

    void setAlreadyScanned(std::vector<std::string> alreadyScanned) {
        for (unsigned int i = 0; i < alreadyScanned.size(); i++) {
            scannedFiles.insert(std::pair<std::string, std::string>(alreadyScanned[i], alreadyScanned[i]));
        }
    }

private:

    Detector * detector;
    Eigen::Vector3d leverArm;
    SideScanFileProcessor * processor;
    std::map<std::string, std::string> scannedFiles;

    bool isXtf(const fs::directory_entry & entry) {
        //eventually can scan other file extensions
        return entry.path().extension() == ".xtf";
    }

    bool alreadyScanned(std::string & filepath) {
        return scannedFiles.count(filepath);
    }

};

TEST_CASE("Test file lock with monitor") {

    if (fork() == 0) {
        //child process
        std::string file = "test/data/lockTest/s4.xtf";
        int fd = open(file.c_str(), O_RDWR | O_NOATIME);

        struct flock flockStructForTest;
        memset(&flockStructForTest, 0, sizeof (flockStructForTest));

        flockStructForTest.l_type = F_WRLCK;
        if (fcntl(fd, F_SETLKW, &flockStructForTest) == -1) // F_SETLKW waits until lock obtained
        {
            //file is already locked
            REQUIRE(false);
        }

        sleep(10);

        flockStructForTest.l_type = F_UNLCK;
        fcntl(fd, F_SETLKW, &flockStructForTest); // Release the lock
        close(fd);

        REQUIRE(true);

    } else {
        //parent process
        //let child aquire lock
        sleep(5);

        std::string path = "test/data/lockTest/";

        Eigen::Vector3d leverArm;
        leverArm << 0.0, 0.0, 0.0;

        // setup region of interest detector
        int fastThreshold = 300;
        int fastType = cv::FastFeatureDetector::TYPE_9_16;
        bool fastNonMaxSuppression = false;
        double dbscanEpsilon = 50;
        int dbscanMinimumPoints = 20;
        int mserDelta = 6;
        int mserMinimumArea = 320;
        int mserMaximumArea = 15000;
        bool mergeOverlappingObjects = true;

        RoiDetector * roiDetector = new RoiDetector(
                fastThreshold,
                fastType,
                fastNonMaxSuppression,
                dbscanEpsilon,
                dbscanMinimumPoints,
                mserDelta,
                mserMinimumArea,
                mserMaximumArea,
                mergeOverlappingObjects);

        SideScanFileProcessor * processor = new SideScanFileProcessor();
        DirectoryMonitor *monitor = new DirectoryMonitor(roiDetector, processor, leverArm);

        monitor->monitor(path);

        delete processor;
        delete monitor;

        REQUIRE(true);
    }
}

#endif /* MONITORTEST_HPP */

