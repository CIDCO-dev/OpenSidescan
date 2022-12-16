echo "build opensidescan windows script"
mkdir build
cd build
cmake .. -DPROJECT_VERSION=%1
msbuild.exe opensidescan.sln /property:configuration=release


