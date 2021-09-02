$CMD='"C:\Program Files (x86)\Windows Kits\10\App Certification Kit\signtool" sign /debug /tr http://timestamp.digicert.com /td sha256 /fd sha256 /a OpenSidescan-*.exe'
$SIGN=StringReplace($CMD, "VERSION", $CmdLine[1])


Run("cmd.exe")
Sleep(2000)
Send($SIGN)
Send("{Enter}")
Sleep(8000)
Send(EnvGet("Sign_Password"),1)
Send("{Enter}")
Sleep(4000)

WinClose("C:\Windows\SYSTEM32\cmd.exe", "")
