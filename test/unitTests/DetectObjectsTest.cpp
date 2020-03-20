
#include <iostream>


#include "catch.hpp"

#include "../../src/OpenSidescan/sidescanimager.h"
#include "../../src/OpenSidescan/sidescanimage.h"
#include "../../src/OpenSidescan/georeferencedobject.h"

#include "../../src/OpenSidescan/opencvhelper.h"

#include "../../src/thirdParty/MBES-lib/src/math/Distance.hpp"


TEST_CASE( "Test Detect Objects" ) {

    std::cout << std::fixed << std::setprecision( 15 );


    const double maxAcceptableDistance = 5.0;

    // The Scotsman wreck is on channel 0: "Port"
    std::string sidescanFileName = "../data/wrecks/scotsman5.xtf";
    int channelIndex = 0;

    // TODO: Changed to the coordinates at the center of the box for the found object
    // Position of Scotsman, from file "reference points on SSS scotsman5.pptx"
    // X, -68.828327, 48.445632
    double longitudeCarisScotsman = -68.828327;
    double latitudeCarisScotsman = 48.445632;


    DatagramParser * parser = nullptr;

    SidescanFile * file = nullptr;

    try {

        SidescanImager imager;

        std::cout << "\nBefore parsing\n" << std::endl;


        parser = DatagramParserFactory::build(sidescanFileName,imager);
        parser->parse(sidescanFileName);

        std::cout << "\nBefore imager.generate(sFileName)\n" << std::endl;

        file = imager.generate(sidescanFileName);

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


    std::cout << "\nChannel name: " << image->getChannelName() << "\n" << std::endl;


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


    REQUIRE( objectsDetected.size() > 0 );

    double minDistance = 1e40; // TODO: replace with maximum positive value a double can hold
    int indexObjectClosest = 0;

    for (int count = 0; count < objectsDetected.size(); count++ ) {

        Position * position = objectsDetected[ count ]->getPosition();

        std::cout << "\n" << count << ") Object name: "
                  << objectsDetected[ count ]->getName() << std::endl;

        std::cout << std::fixed << std::setprecision( 15 )
                  << "\n  longitude: " << position->getLongitude()
                  << "\n  latitude:  " << position->getLatitude()
                  << "\n  width (m): " << objectsDetected[ count ]->getWidth()
                  << "\n  height(m): " << objectsDetected[ count ]->getHeight()
                  << "\n" << std::endl;


        double distance = Distance::haversine( longitudeCarisScotsman, latitudeCarisScotsman,
                                               position->getLongitude(), position->getLatitude() );

        std::cout << "\nDistance: " << distance << "\n" << std::endl;

        if ( distance < minDistance ) {
            minDistance = distance;
            indexObjectClosest = count;
        }


    }

    std::cout << "\nindexObjectClosest: " << indexObjectClosest << "\n"
              << "minDistance: " << minDistance << "\n" << std::endl;

    REQUIRE( minDistance < maxAcceptableDistance );

    if ( file != nullptr )
        delete file;
}




