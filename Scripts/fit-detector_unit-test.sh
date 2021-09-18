#!/bin/bash

mkdir -p test/fit-detector_unit-test/build
cd test/fit-detector_unit-test/build
cmake ..
make tests
