$CMD='"C:\Program Files (x86)\Windows Kits\10\App Certification Kit\signtool" sign /debug /tr http://timestamp.digicert.com /td sha256 /fd sha256 /a build/OpenSidescan-*-win64.exe'

Run("cmd.exe")
Sleep(2000)
Send($CMD)
Send("{Enter}")
Sleep(4000)
WinClose("C:\Windows\SYSTEM32\cmd.exe", "")
