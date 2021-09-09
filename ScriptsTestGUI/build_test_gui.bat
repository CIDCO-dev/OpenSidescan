mkdir test\testGUI\build\
mkdir build\reports\
cd test/testGUI/build
cmake .. -DCMAKE_GENERATOR_PLATFORM=x64
MSBuild.exe Opensidescan_gui_Tests.sln /property:configuration=Release
Release\\Opensidescan_gui_Tests.exe  -o ..\\..\\..\\build\\reports\\testGUI_result_XUNIT.xml -xunitxml
