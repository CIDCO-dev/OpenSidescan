#!/bin/bash

mkdir -p test/build
cd test/build
cmake ..;
make
; ./lockTests -r junit -o ../../build/reports/lock-test-report.xml || true