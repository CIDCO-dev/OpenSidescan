// Temporary: current working directory: in the folder with the sources
// g++ -Wall -I/usr/include/eigen3 -o build/tests main.cpp GeoreferencingTest.cpp ../../src/OpenSidescan/sidescanfile.cpp ../../src/OpenSidescan/sidescanimager.cpp ../../src/OpenSidescan/sidescanimage.cpp ../../src/OpenSidescan/sidescanimage.cpp  ../../src/OpenSidescan/georeferencedobject.cpp ../../src/thirdParty/MBES-lib/src/datagrams/DatagramParser.cpp ../../src/thirdParty/MBES-lib/src/datagrams/DatagramParserFactory.cpp ../../src/thirdParty/MBES-lib/src/datagrams/s7k/S7kParser.cpp ../../src/thirdParty/MBES-lib/src/datagrams/kongsberg/KongsbergParser.cpp ../../src/thirdParty/MBES-lib/src/datagrams/xtf/XtfParser.cpp ../../src/thirdParty/MBES-lib/src/utils/NmeaUtils.cpp ../../src/thirdParty/MBES-lib/src/utils/StringUtils.cpp ../../src/thirdParty/MBES-lib/src/sidescan/SidescanPing.cpp ../../src/thirdParty/opencv/apps/createsamples/utility.cpp `pkg-config --cflags --libs opencv`




#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "SideScanImagerTest.h"
//#include "GeoreferencingTest.hpp"

