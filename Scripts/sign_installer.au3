$CMD='"C:\Program Files (x86)\Windows Kits\10\App Certification Kit\signtool" sign /build /tr http://timestamp.digicert.com /td sha256 /fd sha256 /a OpenSidescan-1.0.0.exe'

Run("cmd.exe")
Sleep(2000)
Send($CMD)
Send("{Enter}")
Sleep(4000)
WinClose("C:\Windows\SYSTEM32\cmd.exe", "")
