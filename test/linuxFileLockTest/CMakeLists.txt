cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(locktest)

find_package (Eigen3 3.3 REQUIRED NO_MODULE)

find_package(OpenCV 4.5 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_executable(lockTests main.cpp ../../src/OpenSidescan/utilities/opencvhelper.cpp ../../src/OpenSidescan/detector/roidetector.cpp ../../src/OpenSidescan/detector/detector.cpp ../../src/OpenSidescan/inventoryobject/inventoryobject.cpp ../../src/OpenSidescan/sidescan/sidescanimage.cpp ../../src/OpenSidescan/sidescan/sidescanfile.cpp ../../src/thirdParty/MBES-lib/src/datagrams/DatagramParser.cpp ../../src/thirdParty/MBES-lib/src/datagrams/DatagramParserFactory.cpp ../../src/thirdParty/MBES-lib/src/datagrams/s7k/S7kParser.cpp ../../src/thirdParty/MBES-lib/src/datagrams/kongsberg/KongsbergParser.cpp ../../src/thirdParty/MBES-lib/src/datagrams/xtf/XtfParser.cpp ../../src/thirdParty/MBES-lib/src/utils/NmeaUtils.cpp ../../src/thirdParty/MBES-lib/src/utils/StringUtils.cpp ../../src/thirdParty/MBES-lib/src/sidescan/SidescanPing.cpp ../../src/thirdParty/MBES-lib/src/getopt.c ../../src/thirdParty/MBES-lib/src/hydroblock/Hydroblock20Parser.cpp)

set_target_properties(lockTests PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries (lockTests stdc++fs)
target_link_libraries(lockTests ${OpenCV_LIBS})
target_link_libraries (lockTests Eigen3::Eigen)
