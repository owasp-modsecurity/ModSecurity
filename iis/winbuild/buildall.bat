set FULLBUILD=1
set DROP=c:\drop\amd64
cmd.exe /c build64.bat
set DROP=c:\drop\x86
cmd.exe /c build32.bat
