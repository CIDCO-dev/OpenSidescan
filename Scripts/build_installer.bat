mkdir build
cd build
cmake .. -DCMAKE_GENERATOR_PLATFORM=x64
cpack -G NSIS
