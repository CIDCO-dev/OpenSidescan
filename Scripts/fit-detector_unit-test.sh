#!/bin/bash

mkdir -p test/fit-detector_unit-test/build
cd test/fit-detector_unit-test/build
cmake ..
make -j4
#./fit-detector_test -r junit -o ../../../build/reports/fit-detector_test_result.xml
