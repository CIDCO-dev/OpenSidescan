#!/bin/bash

mkdir -p test/testGUI/build
cd test/testGUI/build
cmake ..
make
cd ../../../
mkdir -p build/reports
./test/testGUI/build/Opensidescan_gui_Tests -o build/reports/linux-testGUI.xml -xunitxml
