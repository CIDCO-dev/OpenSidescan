mkdir -p test/testGUI/build/
mkdir -p build/reports/
cd test/testGUI/build
cmake .. 
make
cd ../../../
./test/testGUI/build/Opensidescan_gui_Tests.exe -o build/reports/linux-testGUI.xml -xunitxml
