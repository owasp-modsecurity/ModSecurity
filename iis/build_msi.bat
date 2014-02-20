

set PATH="%PATH%;C:\Program Files (x86)\WiX Toolset v3.8\bin;C:\Program Files (x86)\WiX Toolset v3.7\bin;"
set CURRENT_DIR=%cd%

del installer.wix*

"candle.exe" -ext WixUtilExtension -ext WixUIExtension "%CURRENT_DIR%\installer.wxs" -out "%CURRENT_DIR%\installer.wixobj" -arch x64
@if NOT (%ERRORLEVEL%) == (0) goto build_failed

"light.exe" -ext WixUtilExtension -ext WixUIExtension "%CURRENT_DIR%\installer.wixobj" -out "%CURRENT_DIR%\installer-64.msi" 
@if NOT (%ERRORLEVEL%) == (0) goto build_failed

"candle.exe" -ext WixUtilExtension -ext WixUIExtension "%CURRENT_DIR%\installer.wxs" -out "%CURRENT_DIR%\installer.wixobj" -arch x86
@if NOT (%ERRORLEVEL%) == (0) goto build_failed

"light.exe" -ext WixUtilExtension -ext WixUIExtension "%CURRENT_DIR%\installer.wixobj" -out "%CURRENT_DIR%\installer-32.msi" 
@if NOT (%ERRORLEVEL%) == (0) goto build_failed

exit /B 0

:build_failed
@echo Problems during the building phase
@goto failed

:failed
@cd %CURRENT_DIR%
@exit /B 1

