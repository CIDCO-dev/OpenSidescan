cmake_minimum_required(VERSION 3.13)

# set the project name
project(opensidescan VERSION 0.01)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_AUTOMOC ON)

# add the executable
add_executable(opensidescan ${CMAKE_CURRENT_SOURCE_DIR}src/OpenSidescan/main.cpp)

# find_package will locate dependencies wheter they are installed from source or from a package manager
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package(OpenCV 3.4.7 REQUIRED)
list(APPEND CMAKE_PREFIX_PATH "/usr/include/x86_64-linux-gnu") # append path list where find_package is looking
find_package(Qt5 COMPONENTS Widgets REQUIRED)
#message("opencv_libs: " ${OpenCV_LIBS})

set(SS src/OpenSidescan)
set(SOURCE_FILES ${SS}/sidescan/sidescanfile.cpp;
    ${SS}/sidescan/sidescanimager.cpp;
    ${SS}/sidescan/sidescanimage.cpp;
    ${SS}/inventoryobject/inventoryobject.cpp;
    ${SS}/detector/roidetector.cpp;
    ${SS}/detector/houghdetector.cpp;
    ${SS}/detector/detector.cpp;
    )

set(3RDParty src/thirdparty)
set(THIRDPARTY ${3rdParty}/MBES-lib/src/datagrams/DatagramParser.cpp;
    ${3rdParty}/MBES-lib/src/datagrams/DatagramParserFactory.cpp;
    ${3rdParty}/MBES-lib/src/datagrams/s7k/S7kParser.cpp;
    ${3rdParty}/MBES-lib/src/datagrams/s7k/S7kParser.cpp;
    ${3rdParty}/MBES-lib/src/datagrams/kongsberg/KongsbergParser.cpp;
    ${3rdParty}/MBES-lib/src/datagrams/xtf/XtfParser.cpp;
    ${3rdParty}/MBES-lib/src/utils/NmeaUtils.cpp;
    ${3rdParty}/MBES-lib/src/utils/StringUtils.cpp;
    ${3rdParty}/MBES-lib/src/sidescan/SidescanPing.cpp;
    ${3rdParty}/opencv/apps/createsamples/utility.cpp;
    )

add_library(libvar STATIC ${SOURCE_FILES}) 
target_include_directories(libvar PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${SS} ${CMAKE_CURRENT_SOURCE_DIR}/${3rdParty}) # include directories where header files are located
target_link_libraries(libvar PUBLIC Eigen3::Eigen)
target_link_libraries(opensidescan PUBLIC libvar Eigen3::Eigen ${OpenCV_LIBS} Qt5::Widgets) # link libraries to executable
#target_link_libraries(libvar Eigen3::Eigen)



