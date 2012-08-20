Set WshShell = CreateObject("WScript.Shell")

WshShell.Exec("%windir%\system32\inetsrv\appcmd.exe install module /name:ModSecurityIIS /image:%windir%\system32\inetsrv\modsecurityiis.dll")
