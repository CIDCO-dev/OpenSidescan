mkdir test\build\
mkdir build\reports\
cd test/build
cmake .. -G"Visual Studio 15 2017 Win64"
MSBuild.exe tests.sln /property:configuration=Release
Move Release\* ../build/