
#include <iostream>


#include "catch.hpp"

#include "../../src/OpenSidescan/sidescanimager.h"
#include "../../src/OpenSidescan/sidescanimage.h"
#include "../../src/OpenSidescan/georeferencedobject.h"


TEST_CASE( "Test Caris vs. OpenSidescan" ) {


    std::cout << "\nBeginning of TEST_CASE\n" << std::endl;


//    int a = 1;

//    REQUIRE( a == 1);

//    REQUIRE( a == 2 );

    // The Scotsman wreck is on channel 0: Port
    std::string sFileName = "../data/wrecks/scotsman5.xtf";
    int channelIndex = 0;



//    W
//    x1: 370
//    y1: 654
//    x2: 380
//    y2: 661
//    width: 10
//    height: 7

//    V
//    x1: 461
//    y1: 722
//    x2: 471
//    y2: 730
//    width: 10
//    height: 8

//    X
//    x1: 388
//    y1: 752
//    x2: 397
//    y2: 764
//    width: 9
//    height: 12

//    Z
//    x1: 399
//    y1: 805
//    x2: 412
//    y2: 816
//    width: 13
//    height: 11

//    Y
//    x1: 382
//    y1: 854
//    x2: 394
//    y2: 865
//    width: 12
//    height: 11




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


    //    W
    int x1 = 370;
    int y1 = 654;
    //    x2: 380
    //    y2: 661
    int width = 10;
    int height = 7;


    GeoreferencedObject objectW ( *file,
                                  *image,
                                  x1, y1,
                                  width, height );

    Position * position = objectW.getPosition();

    std::cout << std::fixed << std::setprecision( 15 )
              << "\nW longitude: " << position->getLongitude()
              << "\nW latitude:  " << position->getLatitude() << "\n" << std::endl;



    if ( file != nullptr )
        delete file;


}




