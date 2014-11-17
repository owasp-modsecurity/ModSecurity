::set WORK_DIR=C:\buildbot\win-local_-_Release\build\ModSecurity\iis\dependencies\build_dir
::set SOURCE_DIR=C:\Users\fcosta\Downloads
::set OPENSSL=openssl-1.0.1j.tar.gz
::set OUTPUT_DIR=C:\buildbot\win-local_-_Release\build\ModSecurity\iis\release_files

cd "%WORK_DIR%"
@if NOT EXIST "%SOURCE_DIR%\%OPENSSL%" goto file_not_found_bin

echo "7z..."
@7z.exe x "%SOURCE_DIR%\%OPENSSL%" -so | 7z.exe x -aoa -si -ttar

echo "Timeout..."
timeout 5

echo "OpenSSL..."
set OPENSSL_DIR=%OPENSSL:~0,-7%

echo "Move..."
move "%OPENSSL_DIR%" "openssl"

echo "Cd..."
cd "openssl"


@set ARCH=x86
@call cl 2>&1 | findstr /C:"x64"
@if (%ERRORLEVEL%) == (0) set ARCH=x64

echo "Starting to build" 

if "%ARCH%" == "x64" ( goto first ) else ( goto second )
goto end

:first
  echo "Building x64" 
  perl Configure VC-WIN64A --prefix=%WORK_DIR%/openssl_inst
  @if NOT (%ERRORLEVEL%) == (0) goto build_failed
  call ms\do_win64a
  @if NOT (%ERRORLEVEL%) == (0) goto build_failed
  nmake -f ms\ntdll.mak
  @if NOT (%ERRORLEVEL%) == (0) goto build_failed
  nmake -f ms\ntdll.mak install
  @if NOT (%ERRORLEVEL%) == (0) goto build_failed

  copy /y "%WORK_DIR%\openssl\out32dll\libeay32.dll" "%OUTPUT_DIR%"
  copy /y "%WORK_DIR%\openssl\out32dll\ssleay32.dll" "%OUTPUT_DIR%"
  copy /y "%WORK_DIR%\openssl\out32dll\libeay32.lib" "%OUTPUT_DIR%"
  copy /y "%WORK_DIR%\openssl\out32dll\ssleay32.lib" "%OUTPUT_DIR%"
  goto end

:second
  echo "Building x86"
  perl Configure VC-WIN32 no-asm --prefix=%WORK_DIR%/openssl_inst
  @if NOT (%ERRORLEVEL%) == (0) goto build_failed
  :: call ms\do_nasm
  call ms\do_ms
  @if NOT (%ERRORLEVEL%) == (0) goto build_failed
  nmake -f ms\ntdll.mak
  @if NOT (%ERRORLEVEL%) == (0) goto build_failed
  nmake -f ms\ntdll.mak install
  @if NOT (%ERRORLEVEL%) == (0) goto build_failed

  copy /y "%WORK_DIR%\openssl\out32dll\libeay32.dll" "%OUTPUT_DIR%"
  copy /y "%WORK_DIR%\openssl\out32dll\ssleay32.dll" "%OUTPUT_DIR%"
  copy /y "%WORK_DIR%\openssl\out32dll\libeay32.lib" "%OUTPUT_DIR%"
  copy /y "%WORK_DIR%\openssl\out32dll\ssleay32.lib" "%OUTPUT_DIR%"
  goto end


:end
exit /B 0

:file_not_found_bin
@echo File not found: "%SOURCE_DIR%\%OPENSSL%"
@goto failed

:: :build_failed
:: @echo Problems during the building phase
:: @goto failed

:failed
@exit /B 1

