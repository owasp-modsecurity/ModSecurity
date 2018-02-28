::@if NOT (%ERRORLEVEL%) == (1) Echo "Patch successfull... For more info on patch see: https://vcs.pcre.org/pcre/code/trunk/CMakeLists.txt?r1=1659&r2=1677&view=patch"

cd "%WORK_DIR%"

@if NOT EXIST "%SOURCE_DIR%\%APACHE_BIN%" goto file_not_found_bin

7z.exe x "%SOURCE_DIR%\%PCRE%"
set PCRE_DIR=%PCRE:~0,-4%

move "%PCRE_DIR%" "pcre"

@if "%PCRE_DIR%" == "pcre-8.40" (
	Echo. && Echo "PCRE 8.40 found... patching with patch-pcre-8.40.vbs..." 
	cscript /B /Nologo ../patch-pcre-8.40.vbs
)

cd "pcre"
cat CMakeLists.txt | sed "s/PCRE_STATIC_RUNTIME OFF CACHE BOOL/PCRE_STATIC_RUNTIME/g" > CMakeLists.txt.ops
move CMakeLists.txt CMakeLists.txt.old
move CMakeLists.txt.ops CMakeLists.txt
CMAKE -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_SHARED_LIBS=True
@if NOT (%ERRORLEVEL%) == (0) goto build_failed
NMAKE
@if NOT (%ERRORLEVEL%) == (0) goto build_failed
cd "%WORK%"

copy /y "%WORK_DIR%\pcre\pcre.dll" "%OUTPUT_DIR%"
copy /y "%WORK_DIR%\pcre\pcre.pdb" "%OUTPUT_DIR%"
copy /y "%WORK_DIR%\pcre\pcre.lib" "%OUTPUT_DIR%"
copy /y "%WORK_DIR%\pcre\pcre.h.generic" "%WORK_DIR%\pcre\pcre.h"
echo "a"
@exit /B 0

:file_not_found_bin
@echo File not found: "%SOURCE_DIR%\%PCRE%"
@goto failed

:build_failed
@echo Problems during the building phase
@goto failed

:failed
@exit /B 1
