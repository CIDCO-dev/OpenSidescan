/* 
 * File:   DetectObjectsTest.hpp
 * Author: glm,jordan,pat
 */

#ifndef DETECTOBJECTSTEST_HPP
#define DETECTOBJECTSTEST_HPP

#include "catch.hpp"
#include <string>
#include <vector>

#include "../src/OpenSidescan/inventoryobject/inventoryobject.h"
#include "../src/OpenSidescan/sidescan/sidescanimager.h"
#include "../src/OpenSidescan/sidescan/sidescanfile.h"
#include "../src/OpenSidescan/detector/roidetector.h"
#include "../src/OpenSidescan/detector/houghdetector.h"
#include "../src/OpenSidescan/detector/yolov5detector.h"
#include "../src/thirdParty/MBES-lib/src/datagrams/DatagramParser.hpp"
#include "../src/thirdParty/MBES-lib/src/datagrams/DatagramParserFactory.hpp"
#include "../src/thirdParty/MBES-lib/src/datagrams/xtf/XtfParser.hpp"
#include "opencv2/opencv.hpp"

TEST_CASE("Test Wreck Detector") {

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
            //plane is on channel 1
            image = file->getImages()[i];
        }
    }

    REQUIRE(image);

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
    std::string sidescanFileName = "test/data/starfish/22_07_2020_C2.xtf";

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

TEST_CASE("INVENTORY OBJECT IS INSIDE") {

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
            //plane is on channel 1
            image = file->getImages()[i];
        }
    }

    REQUIRE(image);
    
    InventoryObject positive_detect(*image,100,100,100,100);
    InventoryObject false_detect(*image,400,400,100,100);
    
    struct region area{0,0,300,300};
    
    bool ans1 = positive_detect.is_inside(area);
    REQUIRE(ans1 == true);
    bool ans2 = false_detect.is_inside(area);
    REQUIRE(ans2 == false);
}

TEST_CASE("Crab trap model test") {

    std::string sidescanFileName = "test/data/ghostfishinggear/crabtrap1.xtf";

    SidescanImager imager;
    DatagramParser * parser = DatagramParserFactory::build(sidescanFileName, imager);
    parser->parse(sidescanFileName);

    Eigen::Vector3d leverArm(0, 0, 0);

    SidescanFile * file = imager.generate(sidescanFileName, leverArm);

    REQUIRE(file);

    SidescanImage * image;

    for (unsigned int i = 0; i < file->getImages().size(); i++) {
        if (file->getImages()[i]->getChannelNumber() == 1) {
            //crabtrap is on channel 1
            image = file->getImages()[i];
        }
    }
	
	std::string modelPath;
	
	const std::filesystem::path modelDir{"models"};
	for (auto const& dir_entry : std::filesystem::directory_iterator{modelDir}) {
        if(dir_entry.path().extension() == ".onnx"){
        	modelPath = dir_entry.path().string();
        }
    }
	
	Yolov5Detector crabtrapDetector(modelPath);
	
    std::vector<InventoryObject*> objectsFound;
    crabtrapDetector.detect(*image, objectsFound);
    REQUIRE(objectsFound.size() >= 1);  


    //clean up pointers
    delete image;
    delete parser;

    for(unsigned int i=0; i<objectsFound.size(); i++) {
        delete objectsFound[i];
    }
}

#endif /* DETECTOBJECTSTEST_HPP */

