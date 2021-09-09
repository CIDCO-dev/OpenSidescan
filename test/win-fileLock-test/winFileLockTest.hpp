/*
 * Copyright 2020 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
 */

/* 
 * File:   winFileLockTest.hpp
 * Author: Jordan McManus
 */

#ifndef WINFILELOCKTEST_HPP
#define WINFILELOCKTEST_HPP

#include "../catch.hpp"
#include <stdio.h>
#include <iostream>
#include <string>

#include <Eigen/Dense>
#include "../../src/OpenSidescan/sidescan/sidescanfile.h"
#include "../../src/OpenSidescan/sidescan/sidescanimager.h"
#include "../../src/OpenSidescan/detector/roidetector.h"
#include "../../src/thirdParty/MBES-lib/src/datagrams/DatagramParserFactory.hpp"
#include "../../src/thirdParty/MBES-lib/src/datagrams/DatagramParser.hpp"
#include "../../src/OpenSidescan/sidescan/sidescanfile.h"
#include "../../src/OpenSidescan/sidescan/sidescanimager.h"
#include "../../src/OpenSidescan/detector/roidetector.h"
#include "../../src/thirdParty/MBES-lib/src/datagrams/DatagramParserFactory.hpp"
#include "../../src/thirdParty/MBES-lib/src/datagrams/DatagramParser.hpp"

#include "../../src/OpenSidescan/utilities/FileLockUtils.h"
#include "../../src/OpenSidescan/utilities/SideScanFileProcessor.hpp"
#include "../../src/OpenSidescan/utilities/DirectoryMonitor.hpp"

class SideScanFileProcessorForTest : public SideScanFileProcessor {
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

TEST_CASE("Test file lock with monitor, windows version") {

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof (si));
    si.cb = sizeof (si);
    ZeroMemory(&pi, sizeof (pi));
	
    if (CreateProcess("Debug\\winlocker.exe",
            " test\\data\\lockTest\\s4.xtf", // Additional application arguments
            NULL,
            NULL,
            FALSE,
            DETACHED_PROCESS,
            NULL,
            NULL,
            &si,
            &pi) == FALSE) {

        std::cout << "Couldn't create process" << std::endl;
        REQUIRE(false);
    }


    Sleep(2000);

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

    SideScanFileProcessorForTest * processor = new SideScanFileProcessorForTest();
    DirectoryMonitor *monitor = new DirectoryMonitor(roiDetector, processor, leverArm);

    std::cout << "Monitoring folder: " << path << std::endl;
    monitor->monitor(path);

    delete processor;
    delete monitor;
    
    // let the locking process finish
    Sleep(15000);

    REQUIRE(true);

}

#endif /* WINFILELOCKTEST_HPP */

