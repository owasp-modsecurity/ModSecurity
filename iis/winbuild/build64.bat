mkdir %DROP%
call vcvars64.bat
call init.bat
SET CL=/D "WIN64" /D "_WIN64"
call build.bat
