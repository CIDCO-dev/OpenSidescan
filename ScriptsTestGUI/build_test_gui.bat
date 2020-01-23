@echo off

::NOTE - cl.exe must be in Environment PATH (C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.16.27023\bin\Hostx64\x64)

if exist buildTest rmdir /s/q buildTest
mkdir buildTest
cd buildTest

::Locate c++ files for compilation
call "%windows10_x64_BUILD_TOOLS_ROOT%\VC\Auxiliary\Build\vcvarsall.bat" x64


call "%qt_compiler_dir%\bin\qmake.exe" ..\test\testGUI\testGUI.pro -spec win32-msvc "CONFIG+=qtquickcompiler" && "%qt_compiler_dir%\..\..\Tools\QtCreator\bin\jom.exe"  -f Makefile.Release

cd release

mkdir folderRunTest

move testGUI.exe folderRunTest

cd ..\..

