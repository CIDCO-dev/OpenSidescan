#!/bin/bash

cd test/build
cmake ..;
make
; ./lockTests -r junit -o ../../build/reports/lock-test-report.xml || true