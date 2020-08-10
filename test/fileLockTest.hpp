/*
 * Copyright 2020 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
 */

/* 
 * File:   fileLockTest.hpp
 * Author: Jordan McManus
 */

#ifndef FILELOCKTEST_HPP
#define FILELOCKTEST_HPP

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
        std::cerr << "Processing sidescan file: " << f->getFilename() << std::endl;
        // The file is locked and should not be processed.
        REQUIRE(FALSE);
    }

    void reportProgress(std::string progress) {
        std::cerr << progress << std::endl;
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

TEST_CASE("Test File Lock") {

    std::string path = "../data/lockTest/";

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
    
    REQUIRE(TRUE);
}


#endif /* FILELOCKTEST_HPP */

