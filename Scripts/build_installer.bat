set version=%1

mkdir build
cd build
cmake .. -DPROJECT_VERSION=%1
cpack -G NSIS
