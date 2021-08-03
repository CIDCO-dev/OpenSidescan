cmake_minimum_required(VERSION 3.13)

# set the project name
project(opensidescan VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# QT5
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# find_package will locate dependencies wheter they are installed from source or from a package manager
find_package (Eigen3 3.3 REQUIRED)
find_package(OpenCV 3.4.7 REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)

# opensidescan sources
set(SS src/OpenSidescan)
set(LIB_FILES ${SS}/sidescan/sidescanfile.cpp;
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
set(3rdParty src/thirdParty/MBES-lib/src)
set(THIRDPARTY ${3rdParty}/datagrams/DatagramParser.cpp;
    ${3rdParty}/datagrams/DatagramParserFactory.cpp;
    ${3rdParty}/datagrams/s7k/S7kParser.cpp;
    ${3rdParty}/datagrams/kongsberg/KongsbergParser.cpp;
    ${3rdParty}/datagrams/xtf/XtfParser.cpp;
    ${3rdParty}/utils/NmeaUtils.cpp;
    ${3rdParty}/utils/StringUtils.cpp;
    ${3rdParty}/sidescan/SidescanPing.cpp;
    src/thirdParty/opencv/apps/createsamples/utility.cpp;
    )

# headers include directories
set(INCLUDE_DIR
    ${SS};
    ${3rdParty};
    src/thridParty/opencv/apps/createsamples;
    src/thirdParty/QDarkStyleSheet;
    )
INCLUDE_DIRECTORIES(${INCLUDE_DIR})

# executable sources files
set(SOURCE ${SS}/main.cpp;
    src/thirdParty/QDarkStyleSheet/qdarkstyle/style.qrc;
    ${SS}/resources.qrc;
    )

# linking dependencies
add_library(dep ${LIB_FILES} ${THIRDPARTY})
target_link_libraries(dep Qt5::Widgets Qt5::Xml Eigen3::Eigen ${OpenCV_LIBS})


add_executable(opensidescan ${SOURCE})
target_link_libraries(opensidescan PUBLIC dep) # link libraries to executable




