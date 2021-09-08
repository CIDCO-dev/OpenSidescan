mkdir test\testGUI\build\
mkdir build\reports\
cd test/testGUI/build
cmake .. -DCMAKE_GENERATOR_PLATFORM=x64
MSBuild.exe Opensidescan_gui_Tests.sln
Debug\\wincatchLockTest.exe -r junit -o ..\\..\\..\\build\\reports\\testGUI_result.xml
