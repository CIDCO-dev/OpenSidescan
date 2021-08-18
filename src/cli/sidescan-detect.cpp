/*
* Copyright 2021 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

/*
* \author Guillaume Labbe-Morissette
*/

#ifndef MAIN_CPP
#define MAIN_CPP

#include "../thirdParty/MBES-lib/src/datagrams/DatagramParserFactory.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include "../OpenSidescan/sidescan/sidescanimager.h"
#include "../OpenSidescan/utilities/opencvhelper.h"
#include "../OpenSidescan/detector/roidetector.h"
#include "../OpenSidescan/inventoryobject/inventoryobject.h"

/**Writes the usage information about the datagram-list*/
void printUsage(){
	std::cerr << "\n\
	NAME\n\n\
	sidescan-detect - Dumps sidescan data of an image file\n\n\
	SYNOPSIS\n \
	sidescan-detect file fastThreshold dbscanEpsilon dbscanMinPts mserDelta mserMinimumArea mserMaximumArea\n\n\
	DESCRIPTION\n\n \
	Copyright 2021 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), All rights reserved" << std::endl;
	exit(1);
}

/**
* Declares the parser depending on argument received
*
* @param argc number of argument
* @param argv value of the arguments
*/
int main (int argc , char ** argv ){
	DatagramParser *    parser      = NULL;

        #ifdef __GNU__
	setenv("TZ", "UTC", 1);
	#endif
	#ifdef _WIN32
	putenv("TZ");
	#endif

	if(argc != 8){
		printUsage();
	}

	std::string fileName(argv[1]);
        
        
        
    int fastThreshold = std::atoi(argv[2]);
    int dbscanEpsilon = std::atoi(argv[3]);
    int dbscanMinPts  = std::atoi(argv[4]);
    int mserDelta = std::atoi(argv[5]);
    int mserMinimumArea = std::atoi(argv[6]);
    int mserMaximumArea = std::atoi(argv[7]);
        
	try{
        SidescanImager imager;
        DatagramParser * parser = DatagramParserFactory::build(fileName,imager);
        parser->parse(fileName);
        
        Eigen::Vector3d leverArm(0, 0, 0);
        
        SidescanFile * file = imager.generate(fileName,leverArm);
        SidescanImage *image;
        
        for(auto i=file->getImages().begin();i!=file->getImages().end();i++){
            std::vector<InventoryObject*> objects;
            //TODO : selector detector type
            RoiDetector roiDetector(
                                    fastThreshold,
                                    cv::FastFeatureDetector::TYPE_9_16,
                                    false,
                                    dbscanEpsilon,
                                    dbscanMinPts,
                                    mserDelta,
                                    mserMinimumArea,
                                    mserMaximumArea,
                                    true
                                   );
                                   
            roiDetector.detect(**i, objects);
            for(auto j=objects.begin();j!=objects.end();j++){std::cout << (**i).getChannelNumber() << " " << (*j)->getX() << " " << (*j)->getY() 
                                                                << " "  << (*j)->getPixelWidth() << " " << (*j)->getPixelHeight() << std::endl;
                    }
                }

                delete parser;
                parser=NULL;
	}
	catch(std::exception * e){
		std::cerr << "[-] Error while parsing " << fileName << ": " << e->what() << std::endl;
	}


	if(parser) delete parser;
}


#endif
