mkdir test\win-fileLock-test\build\
mkdir build\reports\
cd test/win-fileLock-test/build
cmake ..
MSBuild.exe wintest.sln
