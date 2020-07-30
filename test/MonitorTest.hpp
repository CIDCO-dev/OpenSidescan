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
#include <experimental/filesystem>
#include <eigen3/Eigen/src/Core/Matrix.h>
#include <atomic>

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
        std::cout << f->getFilename() << std::endl;
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

    std::thread getThread(std::string path) {
        return std::thread(&DirectoryMonitor::monitor, this, path);
    }

    void monitor(std::string path) {
        std::string ext = ".xtf";

        while (!exterminate) {
            for (const auto & entry : fs::directory_iterator(path)) {
                if (entry.path().extension() == ext) {
                    std::string filepath = entry.path().generic_string();

                    if (exterminate) {
                        return;
                    }

                    processor->reportProgress("Loading and detecting objects in file " + filepath);
                    SidescanFile * file = loadAndDetectObjects(filepath);
                    processor->processFile(file);
                }

            }
            
            sleep(3);
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

    void stop() {
        exterminate = true;
    }

private:

    Detector * detector;
    Eigen::Vector3d leverArm;
    SideScanFileProcessor * processor;
    std::atomic<bool> exterminate{false};

};

TEST_CASE("Test Monitor") {

    std::string path = "../data/wrecks/";


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
    std::thread tw1 = monitor->getThread(path);
    
    sleep(10);
    
    std::cout << "Time is up" << std::endl;
    
    
    monitor->stop();
    tw1.join(); // wait for thread to finish before deleting
    
    delete monitor;
}


#endif /* MONITORTEST_HPP */

