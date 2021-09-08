
//#include <iostream>

#include "../catch.hpp"

#include "../../src/OpenSidescan/sidescanimager.h"
#include "../../src/OpenSidescan/sidescanimage.h"
#include "../../src/OpenSidescan/georeferencedobject.h"

#include "../../src/OpenSidescan/opencvhelper.h"

#include "../../src/thirdParty/MBES-lib/src/math/Distance.hpp"

TEST_CASE( "Test Georeferencing" ) {

//    std::cout << std::fixed << std::setprecision( 15 );


    const double maxAcceptableDistance = 5.0;

    // The Scotsman wreck is on channel 0: "Port"
    std::string sidescanFileName = "../data/wrecks/scotsman5.xtf";
    int channelIndex = 0;

    // Position of Scotsman, from file "reference points on SSS scotsman5.pptx"
    // A, -68.828392, 48.445606
    double longitudeCarisScotsman = -68.828392;
    double latitudeCarisScotsman = 48.445606;

    Eigen::Vector3d leverArm(0,0,0);

    DatagramParser * parser = nullptr;

    SidescanFile * file = nullptr;

    try {

        SidescanImager imager;

//        std::cout << "\nBefore parsing\n" << std::endl;


        parser = DatagramParserFactory::build(sidescanFileName,imager);
        parser->parse(sidescanFileName);

//        std::cout << "\nBefore imager.generate(sFileName)\n" << std::endl;

        file = imager.generate(sidescanFileName, leverArm);

        delete parser;
        parser = nullptr;

    }
    catch ( std::exception * e )
    {
        if ( parser != nullptr )
            delete parser;

        if ( file != nullptr )
            delete file;

        std::cerr << e->what();

        exit(1);
    }


    SidescanImage * image = file->getImages()[ channelIndex ];


//    std::cout << "\nChannel name: " << image->getChannelName() << "\n" << std::endl;


    std::vector<GeoreferencedObject*> objectsDetected;

    //Detection parameters
    int    fastThresholdValue                   = 358;
    int    fastTypeValue                        = cv::FastFeatureDetector::TYPE_9_16;
    bool   fastNonMaxSuppressionValue           = false;
    int    dbscanEpsilonValue                   = 51;
    int    dbscanMinPointsValue                 = 19;
    int    mserDeltaValue                       = 5;
    int    mserMinimumAreaValue                 = 320;
    int    mserMaximumAreaValue                 = 14400;
    bool   showFeatureMarkersValue              = false;
    bool   mergeOverlappingBoundingBoxesValue   = true;


    OpencvHelper::detectObjects(
                objectsDetected,
                *file,
                *image,

                fastThresholdValue,
                fastTypeValue,
                fastNonMaxSuppressionValue,
                dbscanEpsilonValue,
                dbscanMinPointsValue,
                mserDeltaValue,
                mserMinimumAreaValue,
                mserMaximumAreaValue,
                showFeatureMarkersValue,
                mergeOverlappingBoundingBoxesValue
            );


    REQUIRE( objectsDetected.size() == 1 );

    Position * position = objectsDetected[ 0 ]->getPosition();

//    std::cout << "\nObject name: "
//              << objectsDetected[ 0 ]->getName() << std::endl;

//    std::cout << std::fixed << std::setprecision( 15 )
//              << "\n  longitude: " << position->getLongitude()
//              << "\n  latitude:  " << position->getLatitude()
//              << "\n  width (m): " << objectsDetected[ 0 ]->getWidth()
//              << "\n  height(m): " << objectsDetected[ 0 ]->getHeight()
//              << "\n" << std::endl;


    double distance = Distance::haversine( longitudeCarisScotsman, latitudeCarisScotsman,
                                           position->getLongitude(), position->getLatitude() );

//    std::cout << "\nDistance: " << distance << "\n" << std::endl;


    REQUIRE( distance < maxAcceptableDistance );

    if ( file != nullptr )
        delete file;
}




