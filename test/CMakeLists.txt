cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(tests)

#[[
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
]]

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

add_executable(tests main.cpp ../src/OpenSidescan/utilities/opencvhelper.cpp ../src/OpenSidescan/detector/roidetector.cpp ../src/OpenSidescan/detector/detector.cpp ../src/OpenSidescan/inventoryobject/inventoryobject.cpp ../src/OpenSidescan/sidescan/sidescanimage.cpp ../src/OpenSidescan/sidescan/sidescanfile.cpp ../src/thirdParty/MBES-lib/src/datagrams/DatagramParser.cpp ../src/thirdParty/MBES-lib/src/datagrams/DatagramParserFactory.cpp ../src/thirdParty/MBES-lib/src/datagrams/s7k/S7kParser.cpp ../src/thirdParty/MBES-lib/src/datagrams/kongsberg/KongsbergParser.cpp ../src/thirdParty/MBES-lib/src/datagrams/xtf/XtfParser.cpp ../src/thirdParty/MBES-lib/src/utils/NmeaUtils.cpp ../src/thirdParty/MBES-lib/src/utils/StringUtils.cpp ../src/thirdParty/MBES-lib/src/sidescan/SidescanPing.cpp ../src/thirdParty/MBES-lib/src/getopt.c )

set_target_properties(tests PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries (tests stdc++fs)
target_link_libraries(tests ${OpenCV_LIBS})