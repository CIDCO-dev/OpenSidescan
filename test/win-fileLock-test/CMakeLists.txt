#[[
Opensidescan cmake config file

[[
written by : Patrick Charron-Morneau
Date : 2 septembre 2021

]]
cmake_minimum_required(VERSION 3.10)

# set the project name
project(wintest)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)




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
                ../../src/thirdParty/opencv/apps/createsamples/utility.cpp;
    )

# headers include directories
set(INCLUDE_DIR ${SS};
                ${3rdParty};
                #src/thridParty/opencv/apps/createsamples;
                #src/thirdParty/QDarkStyleSheet;
    )
INCLUDE_DIRECTORIES(${INCLUDE_DIR})

# executable sources files
set(SOURCE  ${SS}/main.cpp;
            #src/thirdParty/QDarkStyleSheet/qdarkstyle/style.qrc;
            #${SS}/resources.qrc;
			#${SS}/resources/icon.rc;
    )

if(WIN32)
	message("windows cmake")
	include_directories(C:/LIBS/eigen)
	find_package(OpenCV 3.4.7 REQUIRED PATHS C:/LIBS/opencv/build)
	find_package(Qt5 5.12 COMPONENTS Widgets REQUIRED PATHS C:/Qt2/5.12.11/msvc2015_64/lib/cmake/Qt5)
	find_package(Qt5Xml REQUIRED PATHS C:/Qt2/5.12.11/msvc2015_64/lib)
	
	# creating targets
	#add_library(locker-dep )
	add_executable(winlocker winlocker.cpp)
	# linking dependencies
	#target_link_libraries(locker-dep ${OpenCV_LIBS})
	#target_link_libraries(winlocker PUBLIC locker-dep)
	
		# creating targets
	add_library(lockcath-dep ${LIB_FILES} ${THIRDPARTY})
	add_executable(winLockTest winlocker.cpp)
	# linking dependencies
	target_link_libraries(lockcath-dep ${OpenCV_LIBS})
	target_link_libraries(winLockTest PUBLIC lockcath-dep)

	
endif()

