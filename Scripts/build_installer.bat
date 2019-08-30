@echo off

set version=%1

mkdir installer\packages\ca.cidco.opensidescan\data
xcopy /h/k/e/y build\release installer\packages\ca.cidco.opensidescan\data

call powershell.exe -Command "& { (Get-Content '%CD%\installer\config\config.xml') -replace '1.0.0', '%version%' | Set-Content '%CD%\installer\config\config2.xml' }"
del /f "%CD%\installer\config\config.xml"
move "%CD%\installer\config\config2.xml" "installer\config\config.xml"

call powershell.exe -Command "& { (Get-Content '%CD%\installer\packages\ca.cidco.opensidescan\meta\package.xml') -replace '1.0.0', '%version%' | Set-Content '%CD%\installer\packages\ca.cidco.opensidescan\meta\package2.xml' }"
del /f "%CD%\installer\packages\ca.cidco.opensidescan\meta\package.xml"
move "%CD%\installer\packages\ca.cidco.opensidescan\meta\package2.xml" "installer\packages\ca.cidco.opensidescan\meta\package.xml"

call powershell.exe -Command "& { (Get-Content '%CD%\installer\packages\ca.cidco.opensidescan\meta\package.xml') -replace '2000-00-00', '%date:~10,4%-%date:~4,2%-%date:~7,2%' | Set-Content '%CD%\installer\packages\ca.cidco.opensidescan\meta\package2.xml' }"
del /f "%CD%\installer\packages\ca.cidco.opensidescan\meta\package.xml"
move "%CD%\installer\packages\ca.cidco.opensidescan\meta\package2.xml" "installer\packages\ca.cidco.opensidescan\meta\package.xml"


binarycreator -c installer\config\config.xml -p installer\packages OpenSidescan_installer_%version%
