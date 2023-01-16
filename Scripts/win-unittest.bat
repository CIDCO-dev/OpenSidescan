mkdir test\build\
mkdir build\reports\
cd test/build
cmake ..
MSBuild.exe tests.sln /property:configuration=Release
Move Release\* ../build/