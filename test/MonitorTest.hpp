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
#include <atomic>
#include "../src/OpenSidescan/utilities/FileLockUtils.h"
#include "../src/OpenSidescan/utilities/SideScanFileProcessor.hpp"
#include "../src/OpenSidescan/utilities/DirectoryMonitor.hpp"
#include <string>
#include <iostream>
#include <cstring>

class SideScanFileProcessorForTest : public SideScanFileProcessor {
    //Sub class this to connect with OpenSideScan
public:

    void processFile(SidescanFile * f) {
        std::string filenameTest = "test/data/lockTest/s4.xtf";
        REQUIRE(filenameTest.compare(f->getFilename()) == 0);
        REQUIRE(f->getImages().size() == 2);
        REQUIRE(f->getImages()[1]->getObjects().size() == 1);
        
    }

    void reportProgress(std::string progress) {
    }

};

TEST_CASE("Test monitor") {
	std::cout<<"monitor test 1" << std::endl;

    std::string path = "test/data/lockTest/";

    Eigen::Vector3d leverArm;
    leverArm << 0.0, 0.0, 0.0;

    // setup region of interest detector
    int fastThreshold = 300;
    cv::FastFeatureDetector::DetectorType fastType = cv::FastFeatureDetector::TYPE_9_16;
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

    monitor->monitor(path);

    delete processor;
    delete monitor;
}

#endif /* MONITORTEST_HPP */

