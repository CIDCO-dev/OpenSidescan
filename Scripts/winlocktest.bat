mkdir -p test/win-fileLock-test/build
cd test/win-fileLock-test/build
cmake ..
msbuild.exe wintest.sln
