SET APACHE24_ZIP=httpd-2.4.3-win32.zip
mkdir %DROP%
call vsvars32.bat
call init.bat
call build.bat
