#[[
fit-detector unit test cmake config file

written by : Patrick Charron-Morneau
Date : sept

]]
cmake_minimum_required(VERSION 3.10)

# set the project name
project(fit-detector_test)

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
                ${SS}/utilities/opencvhelper.cpp;
                #${SS}/project/project.cpp;
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
    )
INCLUDE_DIRECTORIES(${INCLUDE_DIR})

# executable sources files
set(SOURCE  main.cpp;

    )


if(UNIX)
	message("*nix like cmake")

	# find_package will locate dependencies whether they are installed from source or from a package manager
	find_package (Eigen3 3.3 REQUIRED NO_MODULE)
	find_package(OpenCV 3.4.7 REQUIRED)
	find_package(Qt5 COMPONENTS Widgets Xml REQUIRED)
		
	# creating targets
	add_library(cli_dep ${LIB_FILES} ${THIRDPARTY})
	add_executable(${PROJECT_NAME} ${SOURCE} )

	# linking dependencies
	target_link_libraries(cli_dep Eigen3::Eigen ${OpenCV_LIBS})
	target_link_libraries(${PROJECT_NAME} PUBLIC cli_dep)


endif()
