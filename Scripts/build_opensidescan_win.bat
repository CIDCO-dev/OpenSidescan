echo "build opensidescan windows script"
mkdir -p build
cd build
cmake .. -DCMAKE_GENERATOR_PLATFORM=x64
msbuild.exe opensidescan.sln /property:configuration=release

