SET APACHE24_ZIP=httpd-2.4.6-win64.zip
mkdir %DROP%
rem call vcvars64.bat
call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\x86_amd64\vcvarsx86_amd64.bat"

call init.bat
SET CL=/D "WIN64" /D "_WIN64"
call build.bat
