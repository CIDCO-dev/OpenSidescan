#!/bin/bash

mkdir build/
cd build/
cmake .. -DPROJECT_VERSION=$1
make -j8 Opensidescan
