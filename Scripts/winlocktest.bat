mkdir test\win-fileLock-test\build\
mkdir build\reports\
cd test/win-fileLock-test/build
cmake .. -DCMAKE_GENERATOR_PLATFORM=x64
MSBuild.exe wintest.sln
Debug\\wincatchLockTest.exe -r junit -o build\\reports\\winlock-test-report.xml