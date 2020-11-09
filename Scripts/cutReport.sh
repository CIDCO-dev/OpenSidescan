#!/bin/bash
sed '/<\/testsuites>/q' build/reports/lock-test-report.xml > build/reports/cut-report.xml
