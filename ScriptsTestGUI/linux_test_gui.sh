#!/bin/bash
mkdir -p test/testGUI/build
cd test/testGUI/build
cmake .. 
make Opensidescan_gui_Tests
cd ../../..
#mkdir -p build/reports
#./test/testGUI/build/Opensidescan_gui_Tests.exe -o build/reports/linux-testGUI.xml -xunitxml
