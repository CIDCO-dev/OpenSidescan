$installerPath="C:\Users\pat\jenkins\\workspace\\OpenSidescan-Pat\\build\\OpenSidescan\\build\\_CPack_Packages\\win64\\NSIS\\OpenSidescan-VERSION-win64.exe"
$path=StringReplace($installerPath, "VERSION", $CmdLine[1])

#RequireAdmin
Run($path)

WinWaitActive("OpenSidescan Setup")

send("{ENTER}")
send("{ENTER}")
send("{ENTER}")
send("{ENTER}")
WinWait("OpenSidescan Setup", "Completing OpenSidescan Setup")
send("{ENTER}")

WinWaitActive("New Project")
WinClose("New Project")
