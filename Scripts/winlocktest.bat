mkdir test\win-fileLock-test\build
cd test/win-fileLock-test/build
cmake .. -DCMAKE_GENERATOR_PLATFORM=x64
MSBuild.exe wintest.sln