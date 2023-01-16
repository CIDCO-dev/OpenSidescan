mkdir test\testGUI\build\
mkdir build\reports\
cd test/testGUI/build
cmake ..
MSBuild.exe Opensidescan_gui_Tests.sln /property:configuration=Release

