Set WshShell = CreateObject("WScript.Shell")

WshShell.Exec("%windir%\system32\inetsrv\appcmd.exe uninstall module ModSecurityIIS")