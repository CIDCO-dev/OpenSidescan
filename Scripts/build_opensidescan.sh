#!/bin/bash

mkdir -p build/
cd build/
cmake .. -DPROJECT_VERSION=$1
make -j8 OpenSidescan
