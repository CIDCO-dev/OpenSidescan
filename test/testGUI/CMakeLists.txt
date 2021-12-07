cmake_minimum_required(VERSION 3.10)

# set the project name
project(Opensidescan_gui_Tests)

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
                ${SS}/ui/detection/monitorwindow.cpp;
                ${SS}/ui/docks/projectwindow/projecttreeitem.cpp;
                ${SS}/ui/docks/projectwindow/projecttreemodel.cpp;
                ${SS}/ui/docks/projectwindow/projectwindow.cpp;
                ${SS}/ui/docks/channelpropertieswindow.cpp;
                ${SS}/ui/docks/filepropertieswindow.cpp;
                ${SS}/ui/docks/inventorywindow.cpp;
                ${SS}/ui/about/aboutdialog.cpp;
                ${SS}/refactorme/progressdialognotclosingrightawayoncancel.cpp;
                ${SS}/ui/detection/detectionwindow.cpp;
                ${SS}/ui/detection/workerdetection.cpp;
                ${SS}/ui/inventory/inventoryobjectmenu.cpp;
                ${SS}/ui/inventory/inventoryobjectwindow.cpp;
                ${SS}/ui/mainimage/imagetab.cpp;
                ${SS}/ui/mainimage/imagetablabel.cpp;
                ${SS}/ui/mainwindow/mainwindow.cpp;
                ${SS}/ui/mainwindow/workerimportsidescanfiles.cpp;
                ${SS}/ui/mainwindow/workeropenproject.cpp;
                ${SS}/ui/training/trainingsampleswindow.cpp;
                ${SS}/ui/training/workertrainingsamples.cpp;
                ${SS}/utilities/opencvhelper.cpp;
                ${SS}/utilities/qthelper.cpp;
                ${SS}/ui/training/parameterscvCreateTrainingSamples.cpp;
                ${SS}/ui/docks/vesselgeometrywindow.cpp;
                ${SS}/ui/mainwindow/monitorthread.cpp;
                ${SS}/ui/about/aboutdialog.ui;
                ${SS}/ui/mainwindow/mainwindow.ui;
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
                ../../src/thridParty/opencv/apps/createsamples;
                ../../src/thirdParty/QDarkStyleSheet;
    )
INCLUDE_DIRECTORIES(${INCLUDE_DIR})

# executable sources files
set(SOURCE  testGUI.cpp;
            ../../src/thirdParty/QDarkStyleSheet/qdarkstyle/style.qrc;
            ${SS}/resources.qrc;
			${SS}/resources/icon.rc;
    )
if(WIN32)
	message("Gui test")
	include_directories(C:/LIBS/eigen)
	find_package(OpenCV 4.5 REQUIRED PATHS C:/LIBS/opencv4.5/opencv/build/x64/vc15/lib)
	find_package(Qt5 5.12 COMPONENTS Widgets Core Xml REQUIRED PATHS C:/Qt2/5.12.11/msvc2015_64/lib/cmake/Qt5)
	find_package(Qt5Test REQUIRED PATHS C:/Qt2/5.12.11/msvc2015_64/lib/cmake/Qt5Test)
	
	# creating targets
	add_library(dep ${LIB_FILES} ${THIRDPARTY})
	add_executable(${PROJECT_NAME} WIN32 ${SOURCE})
	# linking dependencies
	target_link_libraries(dep Qt5::Widgets Qt5::Core Qt5::Test Qt5::Xml ${OpenCV_LIBS})
	target_link_libraries(${PROJECT_NAME} PUBLIC dep)
		
endif()
if (UNIX)
    message("Gui test cmake")

	# find_package will locate dependencies wheter they are installed from source or from a package manager
	find_package (Eigen3 3.3 REQUIRED NO_MODULE)
	find_package(OpenCV 3.4.7 REQUIRED)
	find_package(Qt5 COMPONENTS Widgets Xml Test REQUIRED)
		
	# creating targets
	add_library(dep ${LIB_FILES} ${THIRDPARTY})
	add_executable(${PROJECT_NAME} ${SOURCE})
	
	# linking dependencies
	target_link_libraries(dep Qt5::Widgets Qt5::Xml Qt5::Test Eigen3::Eigen ${OpenCV_LIBS})
	target_link_libraries(${PROJECT_NAME} PUBLIC dep)
endif()
