#[[
Opensidescan cmake config file

[[
written by : Patrick Charron-Morneau
Date : 2 septembre 2021

]]
cmake_minimum_required(VERSION 3.10)

# set the project name
project(win_unit_test)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# QT5
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# opensidescan sources
set(SS ../../src/OpenSidescan)
set(LIB_FILES   ${SS}/sidescan/sidescanfile.cpp;
                ${SS}/sidescan/sidescanimager.cpp;
                ${SS}/sidescan/sidescanimage.cpp;
                ${SS}/inventoryobject/inventoryobject.cpp;
                ${SS}/detector/roidetector.cpp;
                ${SS}/detector/houghdetector.cpp;
                ${SS}/detector/detector.cpp;
                ${SS}/machinelearning/dbscan.cpp;
                ${SS}/project/project.cpp;
    )

# thirdParty sources
set(3rdParty ../../src/thirdParty/MBES-lib/src)
set(THIRDPARTY  ${3rdParty}/datagrams/DatagramParser.cpp;
                ${3rdParty}/datagrams/DatagramParserFactory.cpp;
                ${3rdParty}/datagrams/s7k/S7kParser.cpp;
                ${3rdParty}/datagrams/kongsberg/KongsbergParser.cpp;
                ${3rdParty}/datagrams/xtf/XtfParser.cpp;
                ${3rdParty}/utils/NmeaUtils.cpp;
                ${3rdParty}/utils/StringUtils.cpp;
                ${3rdParty}/sidescan/SidescanPing.cpp;
    )

# headers include directories
set(INCLUDE_DIR ${SS};
                ${3rdParty};
    )
INCLUDE_DIRECTORIES(${INCLUDE_DIR})


if(WIN32)
	message("windows cmake")
	include_directories(C:/LIBS/eigen)
	find_package(OpenCV 3.4.7 REQUIRED PATHS C:/LIBS/opencv/build)
	find_package(Qt5 5.12 COMPONENTS Widgets Core REQUIRED PATHS C:/Qt2/5.12.11/msvc2015_64/lib/cmake/Qt5)
	find_package(Qt5Xml REQUIRED PATHS C:/Qt2/5.12.11/msvc2015_64/lib)
	
	# creating targets
	add_executable(${PROJECT_NAME} main.cpp )
	add_library(unittest_dep ${LIB_FILES} ${THIRDPARTY})
	# linking librairies
	target_link_libraries(unittest_dep ${OpenCV_LIBS} Qt5::Widgets Qt5::Xml)
	target_link_libraries(${PROJECT_NAME} PUBLIC unittest_dep)
	

	
endif()

