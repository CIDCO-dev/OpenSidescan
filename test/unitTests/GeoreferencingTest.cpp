
#include <iostream>


#include "catch.hpp"

#include "../../src/OpenSidescan/sidescanimager.h"


TEST_CASE( "Test Caris vs. OpenSidescan" ) {


    std::cout << "\nBeginning of TEST_CASE\n" << std::endl;


//    int a = 1;

//    REQUIRE( a == 1);

//    REQUIRE( a == 2 );

    // The Scotsman is on channel 0: Port
    std::string sFileName = "../data/wrecks/scotsman5.xtf";

    DatagramParser * parser = nullptr;

    SidescanFile * file = nullptr;

    try {

        SidescanImager imager;

        parser = DatagramParserFactory::build(sFileName,imager);
        parser->parse(sFileName);

        file = imager.generate(sFileName);

        delete parser;
        parser=NULL;

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












    if ( file != nullptr )
        delete file;


}




