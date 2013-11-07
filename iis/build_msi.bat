
set WIX=C:\Program Files (x86)\WixEdit\wix-3.0.5419.0\
set CURRENT_DIR=%cd%

del installer.wix*

"%WIX%\candle.exe" -ext WixUtilExtension -ext WixUIExtension "%CURRENT_DIR%\installer.wxs" -out "%CURRENT_DIR%\installer.wixobj"

"%WIX%\light.exe" -ext WixUtilExtension -ext WixUIExtension "%CURRENT_DIR%\installer.wixobj" -out "%CURRENT_DIR%\installer.msi"



exit /B 0

:build_failed
@echo Problems during the building phase
@goto failed

:failed
@cd %CURRENT_DIR%
@exit /B 1

