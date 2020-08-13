#!/bin/bash
sed '/<\/testsuites>/q' build/reports/lock-test-report.xml > build/reports/report.xml
