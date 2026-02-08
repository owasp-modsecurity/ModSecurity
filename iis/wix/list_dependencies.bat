
@echo off

echo *
echo *
echo * ModSecurityIIS - list dependencies
echo *
echo *
echo * This script is likely to be used as part as a debugging process.
echo *
echo *
echo * The main function of this script is to list all ModSecurityIIS runtime
echo * dependencies, including system dependencies, to check if there is a
echo * missing library or a version mismatch. This can be very useful in case
echo * ModSecurityIIS refuses to register as IIS module or if IIS refuses to
echo * start.
echo *
echo *
echo *

echo.
echo Checking prerequisites...
echo.

REM Check for Visual C++ 2019 Redistributable (x64)
reg query "HKLM\SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x64" /v Installed 2>nul | find "0x1" >nul
if %errorlevel% equ 0 (
    echo [OK] Visual C++ 2019 Redistributable ^(x64^) is installed
) else (
    echo [MISSING] Visual C++ 2019 Redistributable ^(x64^) is NOT installed
    echo          Download from: https://aka.ms/vs/17/release/vc_redist.x64.exe
)

REM Check for Visual C++ 2019 Redistributable (x86)
reg query "HKLM\SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x86" /v Installed 2>nul | find "0x1" >nul
if %errorlevel% equ 0 (
    echo [OK] Visual C++ 2019 Redistributable ^(x86^) is installed
) else (
    echo [MISSING] Visual C++ 2019 Redistributable ^(x86^) is NOT installed
    echo          Download from: https://aka.ms/vs/17/release/vc_redist.x86.exe
)

echo.
pause

:LOOP_FILE
SET /a log_file=%RANDOM%+100000
SET log_file=%TEMP%\ModSecurityIIS-dependencies-%log_file:~-5%.TXT
IF EXIST %log_file% GOTO LOOP_FILE

echo Saving logs at: %log_file%
echo.

REM Log prerequisite checks to file
echo ================================================ >> %log_file%
echo Visual C++ 2019 Redistributable Check >> %log_file%
echo ================================================ >> %log_file%

reg query "HKLM\SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x64" /v Installed 2>nul | find "0x1" >nul
if %errorlevel% equ 0 (
    echo [OK] Visual C++ 2019 Redistributable (x64) is installed >> %log_file%
) else (
    echo [MISSING] Visual C++ 2019 Redistributable (x64) is NOT installed >> %log_file%
    echo          Download from: https://aka.ms/vs/17/release/vc_redist.x64.exe >> %log_file%
)

reg query "HKLM\SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x86" /v Installed 2>nul | find "0x1" >nul
if %errorlevel% equ 0 (
    echo [OK] Visual C++ 2019 Redistributable (x86) is installed >> %log_file%
) else (
    echo [MISSING] Visual C++ 2019 Redistributable (x86) is NOT installed >> %log_file%
    echo          Download from: https://aka.ms/vs/17/release/vc_redist.x86.exe >> %log_file%
)
echo. >> %log_file%

REM Updated paths to include Visual Studio 2019 and 2022
set POSSIBLE_PATHS_X86="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\*\bin\Hostx86\x86\dumpbin.exe" "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Tools\MSVC\*\bin\Hostx86\x86\dumpbin.exe" "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\*\bin\Hostx86\x86\dumpbin.exe" "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\*\bin\Hostx86\x86\dumpbin.exe" "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Tools\MSVC\*\bin\Hostx86\x86\dumpbin.exe" "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Tools\MSVC\*\bin\Hostx86\x86\dumpbin.exe" "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\bin\dumpbin.exe" "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\dumpbin.exe"
set POSSIBLE_PATHS_X64="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\*\bin\Hostx64\x64\dumpbin.exe" "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Tools\MSVC\*\bin\Hostx64\x64\dumpbin.exe" "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\*\bin\Hostx64\x64\dumpbin.exe" "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\*\bin\Hostx64\x64\dumpbin.exe" "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Tools\MSVC\*\bin\Hostx64\x64\dumpbin.exe" "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Tools\MSVC\*\bin\Hostx64\x64\dumpbin.exe" "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\x86_amd64\dumpbin.exe"

for %%i in (%POSSIBLE_PATHS_X86%) do (
	echo Checking for dumpbin x86... %%i
	echo Checking for dumpbin x86... %%i >> %log_file% 
	if exist %%i (
		SET DUMPBIN_X86=%%i
		goto found_x86
	)
)
:found_x86

for %%i in (%POSSIBLE_PATHS_X64%) do (
	echo Checking for dumpbin x64... %%i
	echo Checking for dumpbin x64... %%i >> %log_file% 
	if exist %%i (
		SET DUMPBIN_X64=%%i
		goto found_x64
	)
)
:found_x64

echo ================================================ >> %log_file%
echo ModSecurity IIS Dependency Analysis >> %log_file%
echo ================================================ >> %log_file%
echo. >> %log_file%

REM Check ModSecurity DLL locations
echo Checking ModSecurity DLL locations... >> %log_file%
if exist "%SystemRoot%\System32\inetsrv\ModSecurityIIS.dll" (
    echo [FOUND] %SystemRoot%\System32\inetsrv\ModSecurityIIS.dll >> %log_file%
) else (
    echo [MISSING] %SystemRoot%\System32\inetsrv\ModSecurityIIS.dll >> %log_file%
)

if exist "%SystemRoot%\SysWOW64\inetsrv\ModSecurityIIS.dll" (
    echo [FOUND] %SystemRoot%\SysWOW64\inetsrv\ModSecurityIIS.dll >> %log_file%
) else (
    echo [MISSING] %SystemRoot%\SysWOW64\inetsrv\ModSecurityIIS.dll >> %log_file%
)
echo. >> %log_file%

if "%DUMPBIN_X86:~1,-1%" == "" (
	echo Dumpbin x86 not found.
	echo Dumpbin x86 not found. >> %log_file%
	echo NOTE: Install Visual Studio 2019/2022 with C++ tools to use dumpbin. >> %log_file%
) else (
	echo Using dumpbin x86: %DUMPBIN_X86%
	echo Using dumpbin x86: %DUMPBIN_X86% >> %log_file%
	echo. >> %log_file%
	if exist "%SystemRoot%\System32\inetsrv\ModSecurityIIS.dll" (
		echo ================================================ >> %log_file%
		echo 32-bit ModSecurityIIS.dll dependencies: >> %log_file%
		echo ================================================ >> %log_file%
		%DUMPBIN_X86% /imports /dependents "%SystemRoot%\System32\inetsrv\ModSecurityIIS.dll" >> %log_file% 2>&1
	)
	if not "%*" == "" (
		echo. >> %log_file%
		echo ================================================ >> %log_file%
		echo Additional files specified: >> %log_file%
		echo ================================================ >> %log_file%
		%DUMPBIN_X86% /imports /dependents %* >> %log_file% 2>&1
	)
)

echo. >> %log_file%

if "%DUMPBIN_X64:~1,-1%" == "" (
	echo Dumpbin x64 not found.
	echo Dumpbin x64 not found. >> %log_file%
	echo NOTE: Install Visual Studio 2019/2022 with C++ tools to use dumpbin. >> %log_file%
) else (
	echo Using dumpbin x64: %DUMPBIN_X64%
	echo Using dumpbin x64: %DUMPBIN_X64% >> %log_file%
	echo. >> %log_file%
	if exist "%SystemRoot%\SysWOW64\inetsrv\ModSecurityIIS.dll" (
		echo ================================================ >> %log_file%
		echo 64-bit ModSecurityIIS.dll dependencies: >> %log_file%
		echo ================================================ >> %log_file%
		%DUMPBIN_X64% /imports /dependents "%SystemRoot%\SysWOW64\inetsrv\ModSecurityIIS.dll" >> %log_file% 2>&1
	)
	if not "%*" == "" (
		echo. >> %log_file%
		echo ================================================ >> %log_file%
		echo Additional files specified: >> %log_file%
		echo ================================================ >> %log_file%
		%DUMPBIN_X64% /imports /dependents %* >> %log_file% 2>&1
	)
)

goto exit

:exit
echo Logs were saved at: %log_file%.
echo Trying to open it with explorer...
explorer %log_file%
echo Done.
pause
