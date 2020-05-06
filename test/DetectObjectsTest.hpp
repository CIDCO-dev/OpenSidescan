/* 
 * File:   DetectObjectsTest.hpp
 * Author: jordan
 */

#ifndef DETECTOBJECTSTEST_HPP
#define DETECTOBJECTSTEST_HPP

#include "catch.hpp"
#include <string>
#include <vector>
#include "../src/OpenSidescan/sidescan/sidescanimager.h"
#include "../src/OpenSidescan/sidescan/sidescanfile.h"
#include "../src/OpenSidescan/detector/roidetector.h"
#include "../src/thirdParty/MBES-lib/src/datagrams/DatagramParser.hpp"
#include "../src/thirdParty/MBES-lib/src/datagrams/DatagramParserFactory.hpp"
#include "../src/thirdParty/MBES-lib/src/datagrams/xtf/XtfParser.hpp"
#include "opencv2/opencv.hpp"

TEST_CASE("Test Detect Objects") {

    std::string sidescanFileName = "test/data/wrecks/plane1.xtf";

    SidescanImager imager;
    DatagramParser * parser = DatagramParserFactory::build(sidescanFileName, imager);
    parser->parse(sidescanFileName);

    Eigen::Vector3d leverArm(0, 0, 0);

    SidescanFile * file = imager.generate(sidescanFileName, leverArm);

    REQUIRE(file);

    SidescanImage * image;

    for (unsigned int i = 0; i < file->getImages().size(); i++) {
        if (file->getImages()[i]->getChannelNumber() == 1) {
            image = file->getImages()[i];
        }
    }

    REQUIRE(image);
    
    Detector * detector = new RoiDetector(
                    300,
                    cv::FastFeatureDetector::TYPE_9_16,
                    false,
                    50,
                    20,
                    6,
                    320,
                    15000,
                    true
                );

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

    RoiDetector roiDetector(
            fastThreshold,
            fastType,
            fastNonMaxSuppression,
            dbscanEpsilon,
            dbscanMinimumPoints,
            mserDelta,
            mserMinimumArea,
            mserMaximumArea,
            mergeOverlappingObjects);
    
    std::vector<InventoryObject*> objectsFound;
    
    roiDetector.detect(*image, objectsFound);
    
    REQUIRE(objectsFound.size() == 1);
}


#endif /* DETECTOBJECTSTEST_HPP */

