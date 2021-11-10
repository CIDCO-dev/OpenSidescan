echo "build opensidescan windows script"
mkdir build
cd build
cmake .. -DCMAKE_GENERATOR_PLATFORM=x64 -DPROJECT_VERSION=%1
msbuild.exe opensidescan.sln /property:configuration=release


