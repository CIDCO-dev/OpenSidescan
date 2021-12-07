mkdir test\testGUI\build\
mkdir build\reports\
cd test/testGUI/build
cmake .. -G"Visual Studio 15 2017 Win64"
MSBuild.exe Opensidescan_gui_Tests.sln /property:configuration=Release

