echo "build opensidescan windows script"
mkdir build
cd build
cmake .. -G"Visual Studio 15 2017 Win64" -DPROJECT_VERSION=%1
msbuild.exe opensidescan.sln /property:configuration=release


