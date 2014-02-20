
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
echo * missing library or a version mismatch. This can be very usefull in case
echo * ModSecurityIIS refuses to register as IIS module or if IIS refuses to
echo * start.
echo * 
echo *
echo *

pause

:LOOP_FILE
SET /a log_file=%RANDOM%+100000
SET log_file=%TEMP%\ModSecurityIIS-depedencies-%log_file:~-5%.TXT
IF EXIST %log_file% GOTO LOOP_FILE

echo Saving logs at: %log_file%

set POSSIBLE_PATHS_X86="C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\bin\dumpbin.exe" "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\dumpbin.exe"
set POSSIBLE_PATHS_X64="C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\x86_amd64\dumpbin.exe"

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

if "%DUMPBIN_X86:~1,-1%" == "" (
	echo Dumpbin x86 not found.
	echo Dumpbin x86 not found. >> %log_file%
) else (
	echo Using dumpbin x86: %DUMPBIN_X86%
	echo Using dumpbin x86: %DUMPBIN_X86% >> %log_file% 
	%DUMPBIN_X86% /imports /dependents  %* >> %log_file%
	)
)

if "%DUMPBIN_X64:~1,-1%" == "" (
	echo Dumpbin x64 not found.
	echo Dumpbin x64 not found. >> %log_file% 
) else (
	echo Using dumpbin x64: %DUMPBIN_X64%
	echo Using dumpbin x64: %DUMPBIN_X64% >> %log_file% 
	%DUMPBIN_X64% /imports /dependents  %* >> %log_file%
)

goto exit

:exit
echo Logs were saved at: %log_file%.
echo Trying to open it with explorer...
explorer %log_file%
echo Done.
pause
