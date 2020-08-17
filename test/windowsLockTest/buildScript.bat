if exist "test\windowsLockTest\build" rmdir /s/q "test\windowsLockTest\build"
mkdir "test\windowsLockTest\build"
cd "test\windowsLockTest\build"
cmake ..
make


