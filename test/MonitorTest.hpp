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
#include <thread>
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

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif



//need g++ 8 to remove experimental namespace
namespace fs = std::experimental::filesystem;

class SideScanFileProcessor {
    //Sub class this to connect with OpenSideScan
public:

    void processFile(SidescanFile * f) {
        //In OpenSidescan, add file to project
        std::cout << "Processing sidescan file: " << f->getFilename() << std::endl;
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
                        std::cout << "File is already scanned: " << filepath << std::endl;
                    }
                }
            } else {
                std::cout << "File is locked: " << filepath << std::endl;
            }
        }
    }

    SidescanFile * loadAndDetectObjects(std::string & filepath) {
        SidescanImager imager;
        DatagramParser * parser = NULL;
        parser = DatagramParserFactory::build(filepath, imager);
        parser->parse(filepath);

        std::cout << filepath << " parsed" << std::endl;

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

class MonitorThread {
};

TEST_CASE("Test Monitor") {

    std::string path = "../data/lockTest/";
    //std::string path = "../data/wrecks/";


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

    std::cout << "monitor->monitor(path) returned" << std::endl;

    delete processor;
    delete monitor;
}

TEST_CASE("Test Monitor on locked file linux") {


    std::string path = "../data/lockTest/";
    std::string file = "../data/lockTest/s4.xtf";
    
    
    /*Obtain lock on file*/
        int fd = open(file.c_str(), O_RDWR | O_NOATIME);

        if (fd == -1) {
            std::cout << "Couldn't open file for lock" << std::endl;
            REQUIRE(false);
        }

        struct flock flockStructForTest;
        memset(&flockStructForTest, 0, sizeof (flockStructForTest));

        flockStructForTest.l_type = F_WRLCK;
        if (fcntl(fd, F_SETLKW, &flockStructForTest) == -1) // If a lock already set, does not wait
        {
            REQUIRE(false);
        }
        /*Done Obtaining lock*/
        
        
        /*Start monitor*/
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
        /*Done starting monitor*/
        
        

        /*Unlock file*/
        flockStructForTest.l_type = F_UNLCK;
        fcntl(fd, F_SETLKW, &flockStructForTest); // Release the lock
        close(fd);
        /*Done Unlock file*/
        
        
        //clean up
        delete processor;
        delete monitor;
}


#endif /* MONITORTEST_HPP */

