set version=%1

mkdir build
cd build
cmake .. -G"Visual Studio 15 2017 Win64" -DPROJECT_VERSION=%1
cpack -G NSIS
