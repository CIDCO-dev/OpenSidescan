
#include <iostream>

#include <vector>
#include <map>
#include <utility> // std::pair, std::make_pair

#include "catch.hpp"

#include "../../src/OpenSidescan/sidescanimager.h"
#include "../../src/OpenSidescan/sidescanimage.h"
#include "../../src/OpenSidescan/georeferencedobject.h"


#include "../../src/thirdParty/MBES-lib/src/math/Distance.hpp"

typedef std::map< std::string, std::pair< double, double >, std::less< std::string > > MapObjectNameLongLat;


TEST_CASE( "Test Caris vs. OpenSidescan" ) {

    std::cout << std::fixed << std::setprecision( 15 );


    const double maxAcceptableDistance = 5.0;

    // The Scotsman wreck is on channel 0: "Port"
    std::string sFileName = "../data/wrecks/scotsman5.xtf";
    int channelIndex = 0;


    // TODO: Read data position of Caris from csv file

    //    V, -68.828257, 48.445629,
    //    W, -68.828268, 48.445548,
    //    X, -68.828327, 48.445632,
    //    Y, -68.828370, 48.445703,
    //    Z, -68.828345, 48.445666,

    MapObjectNameLongLat carisData;

    carisData.insert( std::make_pair( "V", std::make_pair( -68.828257, 48.445629 ) ) );
    carisData.insert( std::make_pair( "W", std::make_pair( -68.828268, 48.445548 ) ) );
    carisData.insert( std::make_pair( "X", std::make_pair( -68.828327, 48.445632 ) ) );
    carisData.insert( std::make_pair( "Y", std::make_pair( -68.828370, 48.445703 ) ) );
    carisData.insert( std::make_pair( "Z", std::make_pair( -68.828345, 48.445666 ) ) );


    DatagramParser * parser = nullptr;

    SidescanFile * file = nullptr;

    try {

        SidescanImager imager;

        std::cout << "\nBefore parsing\n" << std::endl;


        parser = DatagramParserFactory::build(sFileName,imager);
        parser->parse(sFileName);

        std::cout << "\nBefore imager.generate(sFileName)\n" << std::endl;

        file = imager.generate(sFileName);

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


    // TODO: Read OpenSidescan pixels information from a file

    std::vector< std::string > objectNamesOpenSidescan;

    std::vector< std::vector<int> > pixelInfo;

    //    W
    //    x1: 370
    //    y1: 654
    //    width: 10
    //    height: 7

    //    V
    //    x1: 461
    //    y1: 722
    //    width: 10
    //    height: 8

    //    X
    //    x1: 388
    //    y1: 752
    //    width: 9
    //    height: 12

    //    Z
    //    x1: 399
    //    y1: 805
    //    width: 13
    //    height: 11

    //    Y
    //    x1: 382
    //    y1: 854
    //    width: 12
    //    height: 11

    objectNamesOpenSidescan.push_back( "W" );
    pixelInfo.push_back( std::vector<int>( { 370, 654, 10, 7 } ) ); // x1, y1, width, height

    objectNamesOpenSidescan.push_back( "V" );
    pixelInfo.push_back( std::vector<int>( { 461, 722, 10, 8 } ) ); // x1, y1, width, height

    objectNamesOpenSidescan.push_back( "X" );
    pixelInfo.push_back( std::vector<int>( { 388, 752, 9, 12 } ) ); // x1, y1, width, height

    objectNamesOpenSidescan.push_back( "Z" );
    pixelInfo.push_back( std::vector<int>( { 399, 805, 13, 11 } ) ); // x1, y1, width, height

    objectNamesOpenSidescan.push_back( "Y" );
    pixelInfo.push_back( std::vector<int>( { 382, 854, 12, 11 } ) ); // x1, y1, width, height


    for (int count = 0; count < objectNamesOpenSidescan.size(); count++ )
    {
        GeoreferencedObject object ( *file,
                                      *image,
                                      pixelInfo[ count ][ 0 ], pixelInfo[ count ][ 1 ],
                                      pixelInfo[ count ][ 2 ], pixelInfo[ count ][ 3 ] );

        Position * position = object.getPosition();

        std::cout << "\n" << count << ") Object name: " << objectNamesOpenSidescan[ count ] << std::endl;

        std::cout << std::fixed << std::setprecision( 15 )
                  << "\nOpenSidescan:"
                  << "\n  longitude: " << position->getLongitude()
                  << "\n  latitude:  " << position->getLatitude() << "\n" << std::endl;


        std::string objectNameToFind = objectNamesOpenSidescan[ count ];
        MapObjectNameLongLat::const_iterator mapLocation = carisData.find( objectNameToFind );


        REQUIRE( mapLocation != carisData.end() );

        if ( mapLocation != carisData.end() ) {

//            std::cout << "\n\"" << objectNameToFind << "\" found\n" << mapLocation->second.first << "\n"
//                      << mapLocation->second.second << "\n" << std::endl;

            std::cout << std::fixed << std::setprecision( 15 )
                      << "\nCaris:"
                      << "\n  longitude: " << mapLocation->second.first
                      << "\n  latitude:  " << mapLocation->second.second << "\n" << std::endl;

            double distance = Distance::haversine( mapLocation->second.first, mapLocation->second.second,
                                                   position->getLongitude(), position->getLatitude() );

            std::cout << "\nDistance: " << distance << "\n" << std::endl;

            REQUIRE( distance <= maxAcceptableDistance );
        }
//        else {
//            std::cout << "\n\"" << objectNameToFind << "\" not found\n"<< std::endl;
//        }


    }


    if ( file != nullptr )
        delete file;
}




