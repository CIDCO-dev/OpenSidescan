mkdir test\win-fileLock-test\build\
mkdir build\reports\
cd test/win-fileLock-test/build
cmake .. -G"Visual Studio 15 2017 Win64"
MSBuild.exe wintest.sln
