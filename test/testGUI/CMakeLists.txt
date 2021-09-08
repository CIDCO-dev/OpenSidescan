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
	find_package(OpenCV 3.4.7 REQUIRED PATHS C:/LIBS/opencv/build)
	find_package(Qt5 5.12 COMPONENTS Widgets Core Xml REQUIRED PATHS C:/Qt2/5.12.11/msvc2015_64/lib/cmake/Qt5)
	find_package(Qt5Test REQUIRED PATHS C:/Qt2/5.12.11/msvc2015_64/lib/cmake/Qt5Test)
	
	# creating targets
	add_library(dep ${LIB_FILES} ${THIRDPARTY})
	add_executable(${PROJECT_NAME} WIN32 ${SOURCE})
	# linking dependencies
	target_link_libraries(dep Qt5::Widgets Qt5::Core Qt5::Test Qt5::Xml ${OpenCV_LIBS})
	target_link_libraries(${PROJECT_NAME} PUBLIC dep)

	set(CV_DLLS C:/LIBS/opencv/build/x64/vc15/bin/opencv_world347.dll)
	set(QT_DLLS_PATH C:/Qt2/5.12.11/msvc2015_64/bin)
	set(QT_DLLS ${QT_DLLS_PATH}/QT5Widgets.dll
				${QT_DLLS_PATH}/QT5Core.dll
				${QT_DLLS_PATH}/QT5Gui.dll
	)
		
endif()


#[[
windows installer pipeline
install dependencies:
suggestion for install dependencies location : C:\LIBS\
download eigen 3.3.X https://eigen.tuxfamily.org/
download the executable file of opencv 3.4.7 https://github.com/opencv/opencv/releases/tag/3.4.7
add to path env : C:\LIBS\opencv\build\x64\vc15\bin   ** restart terminal after adding path ..its not linux
here we can leave default location
download visual studio : during installation select desktop dev c++ kit https://visualstudio.microsoft.com/downloads/
download qtcreator 5.12.X https://www.qt.io/qt-5-12
add to path env : C:\Qt\5.12.11\msvc2015_64\bin   ** restart terminal after adding path ..its not linux



Download Opensidescan
git clone --recursive https://github.com/CIDCO-dev/OpenSidescan



Install ( git bash required + add path env for cmake and git )
1- navigate to Opensidescan directory
2-mkdir build && cd build
3- cmake ..
4- msbuild.exe opensidescan.sln

build installer :
>cmake ..
>msbuild.exe opensidescan.sln /property:configuration=Release
>cpack
]]
