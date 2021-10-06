#[[
Opensidescan cmake config file

written by : Patrick Charron-Morneau
Date : 13 August 2021

]]
cmake_minimum_required(VERSION 3.10)

# set the project name
project(Opensidescan VERSION ${PROJECT_VERSION})

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# QT5
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)



# opensidescan sources
set(SS src/OpenSidescan)
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
                ${SS}/telemetrymanager/telemetrymanager.cpp;
    )

# thirdParty sources
set(3rdParty src/thirdParty/MBES-lib/src)
set(THIRDPARTY  ${3rdParty}/datagrams/DatagramParser.cpp;
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
set(INCLUDE_DIR ${SS};
                ${3rdParty};
                src/thridParty/opencv/apps/createsamples;
                src/thirdParty/QDarkStyleSheet;
    )
INCLUDE_DIRECTORIES(${INCLUDE_DIR})

# executable sources files
set(SOURCE  ${SS}/main.cpp;
            src/thirdParty/QDarkStyleSheet/qdarkstyle/style.qrc;
            ${SS}/resources.qrc;
			${SS}/resources/icon.rc;
    )

set(CLI_SOURCE 
            ${SS}/sidescan/sidescanfile.cpp;
            ${SS}/sidescan/sidescanimager.cpp;
            ${SS}/sidescan/sidescanimage.cpp;
            ${SS}/inventoryobject/inventoryobject.cpp;
            ${SS}/detector/roidetector.cpp;
            ${SS}/detector/houghdetector.cpp;
            ${SS}/detector/detector.cpp;
            ${SS}/utilities/opencvhelper.cpp;
   )
   
add_compile_definitions(PROJECT_VERSION=${PROJECT_VERSION})

if(UNIX)
	message("*nix like cmake")

	# find_package will locate dependencies whether they are installed from source or from a package manager
	find_package (Eigen3 3.3 REQUIRED NO_MODULE)
	find_package(OpenCV 3.4.7 REQUIRED)
	find_package(Qt5 COMPONENTS Widgets Xml Network REQUIRED)
		
	# creating targets
	add_library(dep ${LIB_FILES} ${THIRDPARTY})
	add_library(cli_dep ${CLI_SOURCE} ${THIRDPARTY})
	add_executable(${PROJECT_NAME} ${SOURCE})
	add_executable(fit-detector src/cli/fit-detector.cpp )
	add_executable(sidescan-detect src/cli/sidescan-detect.cpp)
	add_executable(sidescan-dump src/cli/sidescan-dump.cpp)
	set_target_properties(fit-detector PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
	set_target_properties(sidescan-detect PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
	set_target_properties(sidescan-dump PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
	# linking dependencies
	target_link_libraries(dep Qt5::Widgets Qt5::Xml Qt5::Network Eigen3::Eigen ${OpenCV_LIBS})
	target_link_libraries(${PROJECT_NAME} PUBLIC dep)
	target_link_libraries(cli_dep Eigen3::Eigen ${OpenCV_LIBS})
	target_link_libraries(fit-detector PUBLIC cli_dep)
	target_link_libraries(sidescan-detect PUBLIC cli_dep)
	target_link_libraries(sidescan-dump PUBLIC cli_dep)

endif()
if(WIN32)
	message("windows cmake")
	include_directories(C:/LIBS/eigen)
	find_package(OpenCV 3.4.7 REQUIRED PATHS C:/LIBS/opencv/build)
	find_package(Qt5 5.12 COMPONENTS Widgets Network REQUIRED PATHS C:/Qt2/5.12.11/msvc2015_64/lib/cmake/Qt5)
	find_package(Qt5Xml REQUIRED PATHS C:/Qt2/5.12.11/msvc2015_64/lib)
	
	# creating targets
	add_library(dep ${LIB_FILES} ${THIRDPARTY})
	add_executable(${PROJECT_NAME} WIN32 ${SOURCE})
	# linking dependencies
	target_link_libraries(dep Qt5::Widgets Qt5::Xml Qt5::Network ${OpenCV_LIBS})
	target_link_libraries(${PROJECT_NAME} PUBLIC dep)

	set(CV_DLLS C:/LIBS/opencv/build/x64/vc15/bin/opencv_world347.dll)
	set(QT_DLLS_PATH C:/Qt2/5.12.11/msvc2015_64/bin)
	set(QT_DLLS ${QT_DLLS_PATH}/QT5Widgets.dll
				${QT_DLLS_PATH}/QT5Core.dll
				${QT_DLLS_PATH}/QT5Gui.dll
				${QT_DLLS_PATH}/Qt5Network.dll
	)
	install(FILES ${QT_DLLS} ${CV_DLLS} DESTINATION bin)
	install(FILES C:/Qt2/5.12.11/msvc2015_64/plugins/platforms/qwindows.dll DESTINATION bin/platforms)
	set(CPACK_NSIS_DISPLAY_NAME ${PROJECT_NAME})# add project to program and features in control panel
	set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\${PROJECT_NAME}.exe")
	set(CPACK_NSIS_PACKAGE_NAME ${PROJECT_NAME}) # show project name in installer
	set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/${SS}/resources/splash.ico") # installer icon
	set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/${SS}/resources/splash.ico") # UNinstaller icon
	set(CPACK_NSIS_CREATE_ICONS_EXTRA "CreateShortCut '$desktop\\\\${PROJECT_NAME}.lnk' '$INSTDIR\\\\bin\\\\${PROJECT_NAME}.exe' ") # add desktop shortcut
	set(CPACK_NSIS_DELETE_ICONS_EXTRA "Delete '$desktop\\\\${PROJECT_NAME}.lnk'")# remove desktop shortcut
	LIST(APPEND CPACK_NSIS_CREATE_ICONS_EXTRA "CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${PROJECT_NAME}.lnk' '$INSTDIR\\\\bin\\\\${PROJECT_NAME}.exe' ")# add START MENU shortcut
	LIST(APPEND CPACK_NSIS_DELETE_ICONS_EXTRA "Delete '$SMPROGRAMS\\\\$START_MENU\\\\${PROJECT_NAME}.lnk'")# remove START MENU shortcut
	set(CPACK_PACKAGE_ICON  "${CMAKE_CURRENT_SOURCE_DIR}/${SS}/resources\\\\splash.bmp") #picture during installation
	set(CPACK_NSIS_MUI_FINISHPAGE_RUN ${PROJECT_NAME}.exe)# ask to run program at the end
	
endif()

# Packaging
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "CIDCO")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_VERBATIM_VARIABLES true)
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
include(CPack)
