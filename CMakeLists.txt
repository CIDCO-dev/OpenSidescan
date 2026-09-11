#[[
Opensidescan cmake config file

written by : Patrick Charron-Morneau
Date : 13 August 2021

]]
cmake_minimum_required(VERSION 3.10)

# set the project name
if(NOT DEFINED PROJECT_VERSION OR PROJECT_VERSION STREQUAL "")
	set(PROJECT_VERSION "0.1.0")
endif()
project(OpenSidescan VERSION ${PROJECT_VERSION})

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# QT5
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# models variable
set(CRABTRAP_MODEL_VERSION crabtrapV1.onnx)
set(CRABTRAP_MODEL_URL http://updates.cidco.ca/Opensidescan-modeles/crabtrapV1.onnx)


if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/models/${CRABTRAP_MODEL_VERSION}")
	make_directory("${CMAKE_CURRENT_SOURCE_DIR}/models/")
	file(DOWNLOAD ${CRABTRAP_MODEL_URL} "${CMAKE_CURRENT_SOURCE_DIR}/models/${CRABTRAP_MODEL_VERSION}" SHOW_PROGRESS)
endif()

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
                ${SS}/detector/yolov5detector.cpp
    )

# thirdParty sources
set(3rdParty src/thirdParty/MBES-lib/src)
set(WMM ${3rdParty}/thirdParty/WorldMagneticModel/WMM2020_Linux/src)
set(THIRDPARTY  ${3rdParty}/datagrams/DatagramParser.cpp;
                ${3rdParty}/datagrams/DatagramParserFactory.cpp;
                ${3rdParty}/datagrams/s7k/S7kParser.cpp;
                ${3rdParty}/datagrams/kongsberg/KongsbergParser.cpp;
                ${3rdParty}/datagrams/xtf/XtfParser.cpp;
                ${3rdParty}/utils/NmeaUtils.cpp;
                ${3rdParty}/utils/StringUtils.cpp;
                ${3rdParty}/sidescan/SidescanPing.cpp;
                ${3rdParty}/hydroblock/Hydroblock20Parser.cpp
                src/thirdParty/opencv/apps/createsamples/utility.cpp;
                ${WMM}/GeomagnetismHeader.h;
                ${WMM}/GeomagnetismLibrary.c;
                ${WMM}/EGM9615.h;
                ${WMM}/wmm_calculations.cpp;)

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
	find_package(OpenCV 4.5 REQUIRED)
	find_package(Qt5 COMPONENTS Widgets Xml Network REQUIRED)

	# creating targets
	add_library(dep ${LIB_FILES} ${THIRDPARTY})
	add_library(cli_dep ${CLI_SOURCE} ${THIRDPARTY})
	add_executable(${PROJECT_NAME} ${SOURCE})
	add_executable(fit-detector src/cli/fit-detector.cpp )
	add_executable(sidescan-detect src/cli/sidescan-detect.cpp)
	add_executable(sidescan-dump src/cli/sidescan-dump.cpp)
	add_executable(sidescan-glcm src/cli/sidescan-glcm.cpp)
	add_executable(haralick-analysis src/cli/haralick-analysis.cpp)
	if(APPLE)
		find_package(OpenMP REQUIRED) # AppleClang OpenMP uses libomp, not gomp
	else()
		target_compile_options(haralick-analysis PUBLIC -fopenmp) # mlpack is compile with openMP and need this compile option
	endif()
	set_target_properties(fit-detector PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
	set_target_properties(sidescan-detect PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
	set_target_properties(sidescan-dump PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
	set_target_properties(sidescan-glcm PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
	set_target_properties(haralick-analysis PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
	# linking dependencies
	target_link_libraries(dep Qt5::Widgets Qt5::Xml Qt5::Network Eigen3::Eigen ${OpenCV_LIBS})
	target_link_libraries(${PROJECT_NAME} PUBLIC dep)
	target_link_libraries(cli_dep Eigen3::Eigen ${OpenCV_LIBS})
	target_link_libraries(fit-detector PUBLIC cli_dep)
	target_link_libraries(sidescan-detect PUBLIC cli_dep)
	target_link_libraries(sidescan-dump PUBLIC cli_dep)
	target_link_libraries(sidescan-glcm PUBLIC cli_dep)
	if(APPLE)
		target_link_libraries(haralick-analysis PUBLIC cli_dep mlpack armadillo OpenMP::OpenMP_CXX)
	else()
		target_link_libraries(haralick-analysis PUBLIC cli_dep mlpack armadillo gomp)
	endif()
	
	install(TARGETS sidescan-dump DESTINATION /usr/local/bin/)
	
endif()
	if(WIN32)
		message("windows cmake")
		set(OPENCV_ROOT "C:/LIBS/opencv/build")
	set(_opencv_runtime "")
	foreach(_rt vc17 vc16 vc15 vc14)
		if(EXISTS "${OPENCV_ROOT}/x64/${_rt}/lib")
			set(_opencv_runtime "${_rt}")
			break()
		endif()
	endforeach()
	if(_opencv_runtime STREQUAL "")
		message(FATAL_ERROR "Unable to find OpenCV libraries under ${OPENCV_ROOT}/x64/(vc17|vc16|vc15|vc14)/lib")
	endif()
		# Older OpenCVConfig.cmake files may not detect newer MSVC toolsets automatically.
		set(OpenCV_RUNTIME "${_opencv_runtime}" CACHE STRING "OpenCV runtime" FORCE)
		include_directories(C:/LIBS/eigen-3.4.0)
		find_package(OpenCV 4.5 REQUIRED PATHS "${OPENCV_ROOT}")
		set(_qt_prefix_hint "")
		foreach(_qt_root_candidate
			"C:/Qt/5.15.2/win64_msvc2019_64"
			"C:/Qt/5.15.2/msvc2019_64"
		)
			if(EXISTS "${_qt_root_candidate}/lib/cmake/Qt5")
				set(_qt_prefix_hint "${_qt_root_candidate}")
				break()
			endif()
		endforeach()
		if(_qt_prefix_hint)
			find_package(Qt5 5.15 COMPONENTS Widgets Network REQUIRED PATHS "${_qt_prefix_hint}")
			find_package(Qt5Xml REQUIRED PATHS "${_qt_prefix_hint}")
		else()
			find_package(Qt5 5.15 COMPONENTS Widgets Network REQUIRED)
			find_package(Qt5Xml REQUIRED)
		endif()
	
	# creating targets
	add_library(dep ${LIB_FILES} ${THIRDPARTY})
	add_executable(${PROJECT_NAME} WIN32 ${SOURCE})
	if(MSVC)
		target_compile_definitions(dep PRIVATE _CRT_SECURE_NO_WARNINGS)
		target_compile_options(dep PRIVATE /wd4996 /wd4244 /wd4267 /wd4200 /wd4305 /wd4101)
	endif()
	# linking dependencies
	target_link_libraries(dep Qt5::Widgets Qt5::Xml Qt5::Network ${OpenCV_LIBS})
	target_link_libraries(${PROJECT_NAME} PUBLIC dep)

	set(_opencv_bin_dir "${OPENCV_ROOT}/x64/${OpenCV_RUNTIME}/bin")
		file(GLOB CV_DLLS "${_opencv_bin_dir}/opencv_world*.dll")
		if(NOT CV_DLLS)
			message(FATAL_ERROR "Unable to find OpenCV runtime DLLs in ${_opencv_bin_dir}")
		endif()
		get_filename_component(_qt_install_prefix "${Qt5_DIR}/../../.." ABSOLUTE)
		set(QT_DLLS_PATH "${_qt_install_prefix}/bin")
		set(_qt_platforms_dir "${_qt_install_prefix}/plugins/platforms")
		function(_append_imported_runtime_dll out_list target_name)
			set(_dll "")
			foreach(_cfg_prop
				IMPORTED_LOCATION_RELEASE
				IMPORTED_LOCATION_RELWITHDEBINFO
				IMPORTED_LOCATION_MINSIZEREL
				IMPORTED_LOCATION_DEBUG
				IMPORTED_LOCATION
			)
				get_target_property(_candidate "${target_name}" ${_cfg_prop})
				if(_candidate AND EXISTS "${_candidate}")
					set(_dll "${_candidate}")
					break()
				endif()
			endforeach()
			if(NOT _dll)
				message(FATAL_ERROR "Unable to resolve runtime DLL for ${target_name}")
			endif()
			set(${out_list} "${${out_list}};${_dll}" PARENT_SCOPE)
		endfunction()
		set(QT_DLLS "")
		_append_imported_runtime_dll(QT_DLLS Qt5::Widgets)
		_append_imported_runtime_dll(QT_DLLS Qt5::Core)
		_append_imported_runtime_dll(QT_DLLS Qt5::Gui)
		_append_imported_runtime_dll(QT_DLLS Qt5::Network)
		_append_imported_runtime_dll(QT_DLLS Qt5::Xml)
		install(FILES ${QT_DLLS} ${CV_DLLS} DESTINATION bin)
		install(FILES "${_qt_platforms_dir}/qwindows.dll" DESTINATION bin/platforms)
		install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/models/${CRABTRAP_MODEL_VERSION}" DESTINATION models)
	install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/thirdParty/MBES-lib/src/thirdParty/WorldMagneticModel/WMM2020_Linux/src/WMM.COF" DESTINATION src/thirdParty/MBES-lib/src/thirdParty/WorldMagneticModel/WMM2020_Linux/src/)
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
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_NAME})
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "CIDCO")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_VERBATIM_VARIABLES true)
#set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
include(CPack)
