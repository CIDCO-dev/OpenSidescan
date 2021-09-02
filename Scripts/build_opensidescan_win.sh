mkdir -p build && cd build
cmake ..
msbuild.exe opensidescan.sln /property:configuration=release
