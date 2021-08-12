cmake_minimum_required(VERSION 3.13)

# set the project name
project(Opensidescan VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# QT5
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)



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
if(UNIX)
	message("*nix like cmake")

	# find_package will locate dependencies wheter they are installed from source or from a package manager
	find_package (Eigen3 3.3 REQUIRED)
	find_package(OpenCV 3.4.7 REQUIRED)
	find_package(Qt5 COMPONENTS Widgets REQUIRED)
	find_package(Qt5Xml REQUIRED)


	# linking dependencies
	add_library(dep ${LIB_FILES} ${THIRDPARTY})
	target_link_libraries(dep Qt5::Widgets Qt5::Xml Eigen3::Eigen ${OpenCV_LIBS})

endif()
if(WIN32)
	message("windows cmake")
	include_directories(C:/LIBS/eigen)
	find_package(OpenCV 3.4.7 REQUIRED PATHS C:/LIBS/opencv/build)
	find_package(Qt5 5.12 COMPONENTS Widgets REQUIRED PATHS C:/Qt2/5.12.11/msvc2015_64/lib/cmake/Qt5)
	find_package(Qt5Xml REQUIRED PATHS C:/Qt2/5.12.11/msvc2015_64/lib)
	# linking dependencies
	add_library(dep ${LIB_FILES} ${THIRDPARTY})
	target_link_libraries(dep Qt5::Widgets Qt5::Xml ${OpenCV_LIBS})
	set(CV_DLLS C:/LIBS/opencv/build/x64/vc15/bin/opencv_world347.dll)
	set(QT_DLLS_PATH C:/Qt2/5.12.11/msvc2015_64/bin)
	set(QT_DLLS ${QT_DLLS_PATH}/QT5Widgets.dll
				${QT_DLLS_PATH}/QT5Core.dll
				${QT_DLLS_PATH}/QT5Gui.dll
	)
	install(FILES ${QT_DLLS} ${CV_DLLS} DESTINATION bin)
	install(FILES C:/Qt2/5.12.11/msvc2015_64/plugins/platforms/qwindows.dll DESTINATION bin/platforms)
	set(CPACK_NSIS_PACKAGE_NAME "Opensidescan")
	set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/${SS}/resources/cidco_icon.ico")
	set(CPACK_NSIS_CREATE_ICONS_EXTRA "CreateShortCut '$desktop\\\\Opensidescan.lnk' '$INSTDIR\\\\bin\\\\Opensidescan.exe' ")
	set(CPACK_NSIS_DELETE_ICONS_EXTRA "CreateShortCut '$desktop\\\\Opensidescan.lnk' '$INSTDIR\\\\bin\\\\Opensidescan.exe' ")
	
endif()

add_executable(${PROJECT_NAME} ${SOURCE})
target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenCV_LIBS} dep)

include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "CIDCO")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_VERBATIM_VARIABLES true)
set(CPACK_PACKAGE_VERSION_MAJOR)
set(CPACK_PACKAGE_VERSION_MINOR)
set(CPACK_PACKAGE_VERSION_PATCH)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
include(CPack)