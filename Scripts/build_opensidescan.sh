#!/bin/bash

mkdir build/
cd build/
cmake .. -DPROJECT_VERSION
make -j8 Opensidescan
