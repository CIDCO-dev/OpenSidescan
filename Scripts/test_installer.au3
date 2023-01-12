$installerPath="OpenSidescan\build\_CPack_Packages\win64\NSIS\OpenSidescan-VERSION-win64.exe"
$installerPath=StringReplace($CMD, "VERSION", $CmdLine[1])

#RequireAdmin
Run("C:\Users\pat\OpenSidescan\build\_CPack_Packages\win64\NSIS\OpenSidescan-666-win64.exe")

WinWaitActive("OpenSidescan Setup")

send("{ENTER}")
send("{ENTER}")
send("{ENTER}")
send("{ENTER}")
WinWait("OpenSidescan Setup", "Completing OpenSidescan Setup")
send("{ENTER}")

WinWaitActive("New Project")
WinClose("New Project")
