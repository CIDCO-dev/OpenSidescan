/* 
 * File:   DetectObjectsTest.hpp
 * Author: glm,jordan
 */

#ifndef DETECTOBJECTSTEST_HPP
#define DETECTOBJECTSTEST_HPP

#include "catch.hpp"
#include <string>
#include <vector>
#include "../src/OpenSidescan/sidescan/sidescanimager.h"
#include "../src/OpenSidescan/sidescan/sidescanfile.h"
#include "../src/OpenSidescan/detector/roidetector.h"
#include "../src/OpenSidescan/detector/houghdetector.h"
#include "../src/thirdParty/MBES-lib/src/datagrams/DatagramParser.hpp"
#include "../src/thirdParty/MBES-lib/src/datagrams/DatagramParserFactory.hpp"
#include "../src/thirdParty/MBES-lib/src/datagrams/xtf/XtfParser.hpp"
#include "opencv2/opencv.hpp"

TEST_CASE("Test Wreck Detector") {

    std::string sidescanFileName = "../data/wrecks/plane1.xtf";

    SidescanImager imager;
    DatagramParser * parser = DatagramParserFactory::build(sidescanFileName, imager);
    parser->parse(sidescanFileName);

    Eigen::Vector3d leverArm(0, 0, 0);

    SidescanFile * file = imager.generate(sidescanFileName, leverArm);

    REQUIRE(file);

    SidescanImage * image;

    for (unsigned int i = 0; i < file->getImages().size(); i++) {
        if (file->getImages()[i]->getChannelNumber() == 1) {
            //plane is on channel 1
            image = file->getImages()[i];
        }
    }

    REQUIRE(image);

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

    InventoryObject* airplane = objectsFound[0];

    Position* position = airplane->getPosition();

    //std::cout << "airplane longitude: " << position->getLongitude() << std::endl;
    //std::cout << "airplane latitude: " << position->getLatitude() << std::endl;

    //TODO: test position

    //clean up pointers
    delete image;
    delete parser;

    for(unsigned int i=0; i<objectsFound.size(); i++) {
        delete objectsFound[i];
    }
}

TEST_CASE("Test Hough Detector"){
    //std::string sidescanFileName = "/media/glm/Backup Plus/Archeo_Beauport_AECOM/StarFish/xtf/22_07_2020_C2.xtf";
    std::string sidescanFileName = "../data/starfish/22_07_2020_C2.xtf";

    SidescanImager imager;
    DatagramParser * parser = DatagramParserFactory::build(sidescanFileName, imager);
    parser->parse(sidescanFileName);

    Eigen::Vector3d leverArm(0, 0, 0);

    SidescanFile * file = imager.generate(sidescanFileName, leverArm);

    REQUIRE(file);

    HoughDetector detector;
    //std::vector<InventoryObject*> objectsFound; remove double declaration

    for(auto i = file->getImages().begin();i!=file->getImages().end();i++){
        
	std::vector<InventoryObject*> objectsFound;
        
    	detector.detect(**i,objectsFound);

	//cv::imshow("detect",(*i)->getImage());
	//cv::waitKey();
        
        
        
	if((*i)->getChannelNumber()==0){
                //cv::imwrite("chan0.png", (*i)->getImage());
		REQUIRE(objectsFound.size() > 1); // this finds 4998 objects
                
	}
	else{
                //cv::imwrite("chan1.png", (*i)->getImage());
		//REQUIRE(objectsFound.size() == 0); // this finds 5505 objects
	}
        
        for(InventoryObject* obj : objectsFound) {
            delete obj; // fix memory leak
        }
        objectsFound.clear();

    }

}

#endif /* DETECTOBJECTSTEST_HPP */

