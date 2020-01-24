@echo off

cd buildTest\Release\folderRunTest

testGUI.exe -o test-report-OpenSidescanXUNIT.xml,xunitxml -o test-report-OpenSidescan.xml,xml -o test-report-OpenSidescanTAP.txt,tap -o test-report-OpenSidescanTXT.txt,txt

cd ..\..\..
