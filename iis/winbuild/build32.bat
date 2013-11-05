SET APACHE24_ZIP=httpd-2.4.6-win32.zip
mkdir %DROP%
rem call vcvars32.bat
call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\vcvars32.bat"
call init.bat
call build.bat
