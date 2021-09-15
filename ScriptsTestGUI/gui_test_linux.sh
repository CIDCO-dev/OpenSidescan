#!/bin/bash

mkdir -p test/testGUI/build
mkdir -p build/reports
cd test/testGUI/build
cmake ..
make


#./test/testGUI/build/Opensidescan_gui_Tests -o build/reports/linux-testGUI.xml -xunitxml
