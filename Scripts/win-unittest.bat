mkdir test\build\
mkdir build\reports\
cd test/build
cmake .. -DCMAKE_GENERATOR_PLATFORM=x64
MSBuild.exe tests.sln /property:configuration=Release
Move Release\* ../build/
tests.exe -r junit -o build/reports/win-unittest-result.xml