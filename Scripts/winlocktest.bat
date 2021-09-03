mkdir test/win-fileLock-test/build
cd test/win-fileLock-test/build
cmake ..
msbuild.exe test/win-fileLock-test/build/wintest.sln
