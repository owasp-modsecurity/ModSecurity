SET APACHE24_ZIP=httpd-2.4.3-win64.zip
mkdir %DROP%
call vcvars64.bat
call init.bat
SET CL=/D "WIN64" /D "_WIN64"
call build.bat
