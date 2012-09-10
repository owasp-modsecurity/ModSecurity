# Microsoft Developer Studio Generated NMAKE File, Based on libhttpd.dsp
!IF "$(CFG)" == ""
CFG=libhttpd - Win32 Release
!MESSAGE No configuration specified. Defaulting to libhttpd - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "libhttpd - Win32 Release" && "$(CFG)" != "libhttpd - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libhttpd.mak" CFG="libhttpd - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libhttpd - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libhttpd - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "libhttpd - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : ".\include\mod_dav.h" "$(OUTDIR)\libhttpd.dll" "$(DS_POSTBUILD_DEP)"

!ELSE 

ALL : "pcre - Win32 Release" "gen_test_char - Win32 Release" "libaprutil - Win32 Release" "libapriconv - Win32 Release" "libapr - Win32 Release" ".\include\mod_dav.h" "$(OUTDIR)\libhttpd.dll" "$(DS_POSTBUILD_DEP)"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"libapr - Win32 ReleaseCLEAN" "libapriconv - Win32 ReleaseCLEAN" "libaprutil - Win32 ReleaseCLEAN" "gen_test_char - Win32 ReleaseCLEAN" "pcre - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ap_regkey.obj"
	-@erase "$(INTDIR)\byterange_filter.obj"
	-@erase "$(INTDIR)\child.obj"
	-@erase "$(INTDIR)\chunk_filter.obj"
	-@erase "$(INTDIR)\config.obj"
	-@erase "$(INTDIR)\connection.obj"
	-@erase "$(INTDIR)\core.obj"
	-@erase "$(INTDIR)\core_filters.obj"
	-@erase "$(INTDIR)\eoc_bucket.obj"
	-@erase "$(INTDIR)\error_bucket.obj"
	-@erase "$(INTDIR)\http_core.obj"
	-@erase "$(INTDIR)\http_etag.obj"
	-@erase "$(INTDIR)\http_filters.obj"
	-@erase "$(INTDIR)\http_protocol.obj"
	-@erase "$(INTDIR)\http_request.obj"
	-@erase "$(INTDIR)\libhttpd.res"
	-@erase "$(INTDIR)\libhttpd_cl.idb"
	-@erase "$(INTDIR)\libhttpd_cl.pdb"
	-@erase "$(INTDIR)\listen.obj"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\mod_so.obj"
	-@erase "$(INTDIR)\mod_win32.obj"
	-@erase "$(INTDIR)\modules.obj"
	-@erase "$(INTDIR)\mpm_common.obj"
	-@erase "$(INTDIR)\mpm_winnt.obj"
	-@erase "$(INTDIR)\nt_eventlog.obj"
	-@erase "$(INTDIR)\protocol.obj"
	-@erase "$(INTDIR)\provider.obj"
	-@erase "$(INTDIR)\request.obj"
	-@erase "$(INTDIR)\scoreboard.obj"
	-@erase "$(INTDIR)\service.obj"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\util_cfgtree.obj"
	-@erase "$(INTDIR)\util_filter.obj"
	-@erase "$(INTDIR)\util_md5.obj"
	-@erase "$(INTDIR)\util_pcre.obj"
	-@erase "$(INTDIR)\util_script.obj"
	-@erase "$(INTDIR)\util_time.obj"
	-@erase "$(INTDIR)\util_win32.obj"
	-@erase "$(INTDIR)\util_xml.obj"
	-@erase "$(INTDIR)\vhost.obj"
	-@erase "$(OUTDIR)\libhttpd.dll"
	-@erase "$(OUTDIR)\libhttpd.exp"
	-@erase "$(OUTDIR)\libhttpd.lib"
	-@erase "$(OUTDIR)\libhttpd.pdb"
	-@erase ".\include\mod_dav.h"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /Zi /O2 /Oy- /I "./include" /I "./srclib/apr/include" /I "./srclib/apr-util/include" /I "./srclib/pcre" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "PCRE_STATIC" /D "AP_DECLARE_EXPORT" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\libhttpd_cl" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\libhttpd.res" /i "./include" /i "./srclib/apr/include" /d "NDEBUG" /d BIN_NAME="libhttpd.dll" /d LONG_NAME="Apache HTTP Server Core" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libhttpd.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib advapi32.lib ws2_32.lib mswsock.lib "Release\buildmark.obj" /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\libhttpd.pdb" /debug /out:"$(OUTDIR)\libhttpd.dll" /implib:"$(OUTDIR)\libhttpd.lib" /base:@"os\win32\BaseAddr.ref",libhttpd.dll /opt:ref 
LINK32_OBJS= \
	"$(INTDIR)\byterange_filter.obj" \
	"$(INTDIR)\chunk_filter.obj" \
	"$(INTDIR)\config.obj" \
	"$(INTDIR)\connection.obj" \
	"$(INTDIR)\core.obj" \
	"$(INTDIR)\core_filters.obj" \
	"$(INTDIR)\http_core.obj" \
	"$(INTDIR)\http_etag.obj" \
	"$(INTDIR)\http_filters.obj" \
	"$(INTDIR)\http_protocol.obj" \
	"$(INTDIR)\http_request.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\protocol.obj" \
	"$(INTDIR)\request.obj" \
	"$(INTDIR)\vhost.obj" \
	"$(INTDIR)\mod_so.obj" \
	"$(INTDIR)\mod_win32.obj" \
	"$(INTDIR)\modules.obj" \
	"$(INTDIR)\eoc_bucket.obj" \
	"$(INTDIR)\error_bucket.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\util_cfgtree.obj" \
	"$(INTDIR)\util_filter.obj" \
	"$(INTDIR)\util_md5.obj" \
	"$(INTDIR)\util_pcre.obj" \
	"$(INTDIR)\util_script.obj" \
	"$(INTDIR)\util_time.obj" \
	"$(INTDIR)\util_win32.obj" \
	"$(INTDIR)\util_xml.obj" \
	"$(INTDIR)\ap_regkey.obj" \
	"$(INTDIR)\child.obj" \
	"$(INTDIR)\listen.obj" \
	"$(INTDIR)\mpm_common.obj" \
	"$(INTDIR)\mpm_winnt.obj" \
	"$(INTDIR)\nt_eventlog.obj" \
	"$(INTDIR)\provider.obj" \
	"$(INTDIR)\scoreboard.obj" \
	"$(INTDIR)\service.obj" \
	"$(INTDIR)\libhttpd.res" \
	".\srclib\apr\Release\libapr-1.lib" \
	".\srclib\apr-iconv\Release\libapriconv-1.lib" \
	".\srclib\apr-util\Release\libaprutil-1.lib" \
	".\srclib\pcre\LibR\pcre.lib"

"$(OUTDIR)\libhttpd.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
   cl.exe /nologo /MD /W3 /O2 /Oy- /Zi /I "./include" /I "./srclib/apr/include" /I "./srclib/apr-util/include" /I "./srclib/pcre" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "PCRE_STATIC" /D "AP_DECLARE_EXPORT" /Fd"Release\libhttpd" /FD /c server\buildmark.c /Fo"Release\buildmark.obj"
	 $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

TargetPath=.\Release\libhttpd.dll
SOURCE="$(InputPath)"
PostBuild_Desc=Embed .manifest
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

"$(DS_POSTBUILD_DEP)" : "$(OUTDIR)\libhttpd.dll"
   if exist .\Release\libhttpd.dll.manifest mt.exe -manifest .\Release\libhttpd.dll.manifest -outputresource:.\Release\libhttpd.dll;2
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "libhttpd - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : ".\server\test_char.h" ".\include\mpm_default.h" ".\include\mpm.h" ".\include\mod_so.h" ".\include\mod_proxy.h" ".\include\mod_include.h" ".\include\mod_dav.h" ".\include\mod_core.h" ".\include\mod_cgi.h" "$(OUTDIR)\libhttpd.dll" "$(DS_POSTBUILD_DEP)"

!ELSE 

ALL : "pcre - Win32 Debug" "gen_test_char - Win32 Debug" "libaprutil - Win32 Debug" "libapriconv - Win32 Debug" "libapr - Win32 Debug" ".\server\test_char.h" ".\include\mpm_default.h" ".\include\mpm.h" ".\include\mod_so.h" ".\include\mod_proxy.h" ".\include\mod_include.h" ".\include\mod_dav.h" ".\include\mod_core.h" ".\include\mod_cgi.h" "$(OUTDIR)\libhttpd.dll" "$(DS_POSTBUILD_DEP)"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"libapr - Win32 DebugCLEAN" "libapriconv - Win32 DebugCLEAN" "libaprutil - Win32 DebugCLEAN" "gen_test_char - Win32 DebugCLEAN" "pcre - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ap_regkey.obj"
	-@erase "$(INTDIR)\byterange_filter.obj"
	-@erase "$(INTDIR)\child.obj"
	-@erase "$(INTDIR)\chunk_filter.obj"
	-@erase "$(INTDIR)\config.obj"
	-@erase "$(INTDIR)\connection.obj"
	-@erase "$(INTDIR)\core.obj"
	-@erase "$(INTDIR)\core_filters.obj"
	-@erase "$(INTDIR)\eoc_bucket.obj"
	-@erase "$(INTDIR)\error_bucket.obj"
	-@erase "$(INTDIR)\http_core.obj"
	-@erase "$(INTDIR)\http_etag.obj"
	-@erase "$(INTDIR)\http_filters.obj"
	-@erase "$(INTDIR)\http_protocol.obj"
	-@erase "$(INTDIR)\http_request.obj"
	-@erase "$(INTDIR)\libhttpd.res"
	-@erase "$(INTDIR)\libhttpd_cl.idb"
	-@erase "$(INTDIR)\libhttpd_cl.pdb"
	-@erase "$(INTDIR)\listen.obj"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\mod_so.obj"
	-@erase "$(INTDIR)\mod_win32.obj"
	-@erase "$(INTDIR)\modules.obj"
	-@erase "$(INTDIR)\mpm_common.obj"
	-@erase "$(INTDIR)\mpm_winnt.obj"
	-@erase "$(INTDIR)\nt_eventlog.obj"
	-@erase "$(INTDIR)\protocol.obj"
	-@erase "$(INTDIR)\provider.obj"
	-@erase "$(INTDIR)\request.obj"
	-@erase "$(INTDIR)\scoreboard.obj"
	-@erase "$(INTDIR)\service.obj"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\util_cfgtree.obj"
	-@erase "$(INTDIR)\util_filter.obj"
	-@erase "$(INTDIR)\util_md5.obj"
	-@erase "$(INTDIR)\util_pcre.obj"
	-@erase "$(INTDIR)\util_script.obj"
	-@erase "$(INTDIR)\util_time.obj"
	-@erase "$(INTDIR)\util_win32.obj"
	-@erase "$(INTDIR)\util_xml.obj"
	-@erase "$(INTDIR)\vhost.obj"
	-@erase "$(OUTDIR)\libhttpd.dll"
	-@erase "$(OUTDIR)\libhttpd.exp"
	-@erase "$(OUTDIR)\libhttpd.lib"
	-@erase "$(OUTDIR)\libhttpd.pdb"
	-@erase ".\include\mod_cgi.h"
	-@erase ".\include\mod_core.h"
	-@erase ".\include\mod_dav.h"
	-@erase ".\include\mod_include.h"
	-@erase ".\include\mod_proxy.h"
	-@erase ".\include\mod_so.h"
	-@erase ".\include\mpm.h"
	-@erase ".\include\mpm_default.h"
	-@erase ".\server\test_char.h"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Zi /Od /I "./include" /I "./srclib/apr/include" /I "./srclib/apr-util/include" /I "./srclib/pcre" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "PCRE_STATIC" /D "AP_DECLARE_EXPORT" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\libhttpd_cl" /FD /EHsc /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\libhttpd.res" /i "./include" /i "./srclib/apr/include" /d "_DEBUG" /d BIN_NAME="libhttpd.dll" /d LONG_NAME="Apache HTTP Server Core" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libhttpd.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib advapi32.lib ws2_32.lib mswsock.lib "Debug\buildmark.obj" /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\libhttpd.pdb" /debug /out:"$(OUTDIR)\libhttpd.dll" /implib:"$(OUTDIR)\libhttpd.lib" /base:@"os\win32\BaseAddr.ref",libhttpd.dll 
LINK32_OBJS= \
	"$(INTDIR)\byterange_filter.obj" \
	"$(INTDIR)\chunk_filter.obj" \
	"$(INTDIR)\config.obj" \
	"$(INTDIR)\connection.obj" \
	"$(INTDIR)\core.obj" \
	"$(INTDIR)\core_filters.obj" \
	"$(INTDIR)\http_core.obj" \
	"$(INTDIR)\http_etag.obj" \
	"$(INTDIR)\http_filters.obj" \
	"$(INTDIR)\http_protocol.obj" \
	"$(INTDIR)\http_request.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\protocol.obj" \
	"$(INTDIR)\request.obj" \
	"$(INTDIR)\vhost.obj" \
	"$(INTDIR)\mod_so.obj" \
	"$(INTDIR)\mod_win32.obj" \
	"$(INTDIR)\modules.obj" \
	"$(INTDIR)\eoc_bucket.obj" \
	"$(INTDIR)\error_bucket.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\util_cfgtree.obj" \
	"$(INTDIR)\util_filter.obj" \
	"$(INTDIR)\util_md5.obj" \
	"$(INTDIR)\util_pcre.obj" \
	"$(INTDIR)\util_script.obj" \
	"$(INTDIR)\util_time.obj" \
	"$(INTDIR)\util_win32.obj" \
	"$(INTDIR)\util_xml.obj" \
	"$(INTDIR)\ap_regkey.obj" \
	"$(INTDIR)\child.obj" \
	"$(INTDIR)\listen.obj" \
	"$(INTDIR)\mpm_common.obj" \
	"$(INTDIR)\mpm_winnt.obj" \
	"$(INTDIR)\nt_eventlog.obj" \
	"$(INTDIR)\provider.obj" \
	"$(INTDIR)\scoreboard.obj" \
	"$(INTDIR)\service.obj" \
	"$(INTDIR)\libhttpd.res" \
	".\srclib\apr\Debug\libapr-1.lib" \
	".\srclib\apr-iconv\Debug\libapriconv-1.lib" \
	".\srclib\apr-util\Debug\libaprutil-1.lib" \
	".\srclib\pcre\LibD\pcre.lib"

"$(OUTDIR)\libhttpd.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
   cl.exe /nologo /MDd /W3 /EHsc /Zi /Od /I "./include" /I "./srclib/apr/include" /I "./srclib/apr-util/include" /I "./srclib/pcre" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "PCRE_STATIC" /D "AP_DECLARE_EXPORT" /Fd"Debug\libhttpd" /FD /c server\buildmark.c /Fo"Debug\buildmark.obj"
	 $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

TargetPath=.\Debug\libhttpd.dll
SOURCE="$(InputPath)"
PostBuild_Desc=Embed .manifest
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

"$(DS_POSTBUILD_DEP)" : "$(OUTDIR)\libhttpd.dll"
   if exist .\Debug\libhttpd.dll.manifest mt.exe -manifest .\Debug\libhttpd.dll.manifest -outputresource:.\Debug\libhttpd.dll;2
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("libhttpd.dep")
!INCLUDE "libhttpd.dep"
!ELSE 
!MESSAGE Warning: cannot find "libhttpd.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "libhttpd - Win32 Release" || "$(CFG)" == "libhttpd - Win32 Debug"
SOURCE=.\modules\generators\mod_cgi.h

!IF  "$(CFG)" == "libhttpd - Win32 Release"

InputPath=.\modules\generators\mod_cgi.h

".\include\mod_cgi.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile57.bat 
	@echo off 
	type .\modules\generators\mod_cgi.h > .\include\mod_cgi.h
<< 
	

!ELSEIF  "$(CFG)" == "libhttpd - Win32 Debug"

InputPath=.\modules\generators\mod_cgi.h

".\include\mod_cgi.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile58.bat 
	@echo off 
	type .\modules\generators\mod_cgi.h > .\include\mod_cgi.h
<< 
	

!ENDIF 

SOURCE=.\modules\http\mod_core.h

!IF  "$(CFG)" == "libhttpd - Win32 Release"

InputPath=.\modules\http\mod_core.h

".\include\mod_core.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile59.bat 
	@echo off 
	type .\modules\http\mod_core.h > .\include\mod_core.h
<< 
	

!ELSEIF  "$(CFG)" == "libhttpd - Win32 Debug"

InputPath=.\modules\http\mod_core.h

".\include\mod_core.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile60.bat 
	@echo off 
	type .\modules\http\mod_core.h > .\include\mod_core.h
<< 
	

!ENDIF 

SOURCE=.\modules\dav\main\mod_dav.h

!IF  "$(CFG)" == "libhttpd - Win32 Release"

InputPath=.\modules\dav\main\mod_dav.h

".\include\mod_dav.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile61.bat 
	@echo off 
	type .\modules\dav\main\mod_dav.h > .\include\mod_dav.h
<< 
	

!ELSEIF  "$(CFG)" == "libhttpd - Win32 Debug"

InputPath=.\modules\dav\main\mod_dav.h

".\include\mod_dav.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile62.bat 
	@echo off 
	type .\modules\dav\main\mod_dav.h > .\include\mod_dav.h
<< 
	

!ENDIF 

SOURCE=.\modules\filters\mod_include.h

!IF  "$(CFG)" == "libhttpd - Win32 Release"

InputPath=.\modules\filters\mod_include.h

".\include\mod_include.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile63.bat 
	@echo off 
	type .\modules\filters\mod_include.h > .\include\mod_include.h
<< 
	

!ELSEIF  "$(CFG)" == "libhttpd - Win32 Debug"

InputPath=.\modules\filters\mod_include.h

".\include\mod_include.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile64.bat 
	@echo off 
	type .\modules\filters\mod_include.h > .\include\mod_include.h
<< 
	

!ENDIF 

SOURCE=.\modules\proxy\mod_proxy.h

!IF  "$(CFG)" == "libhttpd - Win32 Release"

InputPath=.\modules\proxy\mod_proxy.h

".\include\mod_proxy.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile65.bat 
	@echo off 
	type .\modules\proxy\mod_proxy.h > .\include\mod_proxy.h
<< 
	

!ELSEIF  "$(CFG)" == "libhttpd - Win32 Debug"

InputPath=.\modules\proxy\mod_proxy.h

".\include\mod_proxy.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile66.bat 
	@echo off 
	type .\modules\proxy\mod_proxy.h > .\include\mod_proxy.h
<< 
	

!ENDIF 

SOURCE=.\modules\mappers\mod_so.h

!IF  "$(CFG)" == "libhttpd - Win32 Release"

InputPath=.\modules\mappers\mod_so.h

".\include\mod_so.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile67.bat 
	@echo off 
	type .\modules\mappers\mod_so.h > .\include\mod_so.h
<< 
	

!ELSEIF  "$(CFG)" == "libhttpd - Win32 Debug"

InputPath=.\modules\mappers\mod_so.h

".\include\mod_so.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile68.bat 
	@echo off 
	type .\modules\mappers\mod_so.h > .\include\mod_so.h
<< 
	

!ENDIF 

SOURCE=.\os\win32\os.h

!IF  "$(CFG)" == "libhttpd - Win32 Release"

InputPath=.\os\win32\os.h

".\include\os.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile69.bat 
	@echo off 
	type .\os\win32\os.h > .\include\os.h
<< 
	

!ELSEIF  "$(CFG)" == "libhttpd - Win32 Debug"

InputPath=.\os\win32\os.h

".\include\os.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile70.bat 
	@echo off 
	type .\os\win32\os.h > .\include\os.h
<< 
	

!ENDIF 

SOURCE=.\server\buildmark.c
SOURCE=.\modules\http\byterange_filter.c

"$(INTDIR)\byterange_filter.obj" : $(SOURCE) "$(INTDIR)" ".\modules\http\mod_core.h" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\modules\http\chunk_filter.c

"$(INTDIR)\chunk_filter.obj" : $(SOURCE) "$(INTDIR)" ".\modules\http\mod_core.h" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\server\config.c

"$(INTDIR)\config.obj" : $(SOURCE) "$(INTDIR)" ".\include\mpm.h" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\server\connection.c

"$(INTDIR)\connection.obj" : $(SOURCE) "$(INTDIR)" ".\include\mpm_default.h" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\server\core.c

"$(INTDIR)\core.obj" : $(SOURCE) "$(INTDIR)" ".\include\mpm.h" ".\include\mod_core.h" ".\include\mod_proxy.h" ".\include\mod_so.h" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\server\core_filters.c

"$(INTDIR)\core_filters.obj" : $(SOURCE) "$(INTDIR)" ".\include\mpm.h" ".\include\mod_core.h" ".\include\mod_proxy.h" ".\include\mod_so.h" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\modules\http\http_core.c

"$(INTDIR)\http_core.obj" : $(SOURCE) "$(INTDIR)" ".\modules\http\mod_core.h" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\modules\http\http_etag.c

"$(INTDIR)\http_etag.obj" : $(SOURCE) "$(INTDIR)" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\modules\http\http_filters.c

"$(INTDIR)\http_filters.obj" : $(SOURCE) "$(INTDIR)" ".\modules\http\mod_core.h" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\modules\http\http_protocol.c

"$(INTDIR)\http_protocol.obj" : $(SOURCE) "$(INTDIR)" ".\modules\http\mod_core.h" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\modules\http\http_request.c

"$(INTDIR)\http_request.obj" : $(SOURCE) "$(INTDIR)" ".\modules\http\mod_core.h" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\server\log.c

"$(INTDIR)\log.obj" : $(SOURCE) "$(INTDIR)" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\server\protocol.c

"$(INTDIR)\protocol.obj" : $(SOURCE) "$(INTDIR)" ".\include\mod_core.h" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\server\request.c

"$(INTDIR)\request.obj" : $(SOURCE) "$(INTDIR)" ".\include\mod_core.h" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\server\vhost.c

"$(INTDIR)\vhost.obj" : $(SOURCE) "$(INTDIR)" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\modules\mappers\mod_so.c

"$(INTDIR)\mod_so.obj" : $(SOURCE) "$(INTDIR)" ".\modules\mappers\mod_so.h" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\modules\arch\win32\mod_win32.c

"$(INTDIR)\mod_win32.obj" : $(SOURCE) "$(INTDIR)" ".\include\mod_core.h" ".\include\mod_cgi.h" ".\include\os.h" ".\include\mod_include.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\os\win32\modules.c

"$(INTDIR)\modules.obj" : $(SOURCE) "$(INTDIR)" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\server\eoc_bucket.c

"$(INTDIR)\eoc_bucket.obj" : $(SOURCE) "$(INTDIR)" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\server\error_bucket.c

"$(INTDIR)\error_bucket.obj" : $(SOURCE) "$(INTDIR)" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\server\util.c

"$(INTDIR)\util.obj" : $(SOURCE) "$(INTDIR)" ".\server\test_char.h" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\server\util_cfgtree.c

"$(INTDIR)\util_cfgtree.obj" : $(SOURCE) "$(INTDIR)" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\server\util_filter.c

"$(INTDIR)\util_filter.obj" : $(SOURCE) "$(INTDIR)" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\server\util_md5.c

"$(INTDIR)\util_md5.obj" : $(SOURCE) "$(INTDIR)" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\server\util_pcre.c

"$(INTDIR)\util_pcre.obj" : $(SOURCE) "$(INTDIR)" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\server\util_script.c

"$(INTDIR)\util_script.obj" : $(SOURCE) "$(INTDIR)" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\server\util_time.c

"$(INTDIR)\util_time.obj" : $(SOURCE) "$(INTDIR)" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\os\win32\util_win32.c

"$(INTDIR)\util_win32.obj" : $(SOURCE) "$(INTDIR)" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\server\util_xml.c

"$(INTDIR)\util_xml.obj" : $(SOURCE) "$(INTDIR)" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\os\win32\ap_regkey.c

"$(INTDIR)\ap_regkey.obj" : $(SOURCE) "$(INTDIR)" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\server\mpm\winnt\child.c

"$(INTDIR)\child.obj" : $(SOURCE) "$(INTDIR)" ".\server\mpm\winnt\mpm_default.h" ".\include\os.h" ".\include\mpm.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\server\listen.c

"$(INTDIR)\listen.obj" : $(SOURCE) "$(INTDIR)" ".\include\mpm.h" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\server\mpm\winnt\mpm.h

!IF  "$(CFG)" == "libhttpd - Win32 Release"

InputPath=.\server\mpm\winnt\mpm.h

".\include\mpm.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile71.bat 
	@echo off 
	type .\server\mpm\winnt\mpm.h > .\include\mpm.h
<< 
	

!ELSEIF  "$(CFG)" == "libhttpd - Win32 Debug"

InputPath=.\server\mpm\winnt\mpm.h

".\include\mpm.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile72.bat 
	@echo off 
	type .\server\mpm\winnt\mpm.h > .\include\mpm.h
<< 
	

!ENDIF 

SOURCE=.\server\mpm_common.c

"$(INTDIR)\mpm_common.obj" : $(SOURCE) "$(INTDIR)" ".\include\mpm.h" ".\include\mpm_default.h" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\server\mpm\winnt\mpm_default.h

!IF  "$(CFG)" == "libhttpd - Win32 Release"

InputPath=.\server\mpm\winnt\mpm_default.h

".\include\mpm_default.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile73.bat 
	@echo off 
	type .\server\mpm\winnt\mpm_default.h > .\include\mpm_default.h
<< 
	

!ELSEIF  "$(CFG)" == "libhttpd - Win32 Debug"

InputPath=.\server\mpm\winnt\mpm_default.h

".\include\mpm_default.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile74.bat 
	@echo off 
	type .\server\mpm\winnt\mpm_default.h > .\include\mpm_default.h
<< 
	

!ENDIF 

SOURCE=.\server\mpm\winnt\mpm_winnt.c

"$(INTDIR)\mpm_winnt.obj" : $(SOURCE) "$(INTDIR)" ".\server\mpm\winnt\mpm_default.h" ".\include\os.h" ".\include\mpm.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\server\mpm\winnt\nt_eventlog.c

"$(INTDIR)\nt_eventlog.obj" : $(SOURCE) "$(INTDIR)" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\server\provider.c

"$(INTDIR)\provider.obj" : $(SOURCE) "$(INTDIR)" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\server\scoreboard.c

"$(INTDIR)\scoreboard.obj" : $(SOURCE) "$(INTDIR)" ".\include\mpm.h" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\server\mpm\winnt\service.c

"$(INTDIR)\service.obj" : $(SOURCE) "$(INTDIR)" ".\include\os.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "libhttpd - Win32 Release"

"libapr - Win32 Release" : 
   cd ".\srclib\apr"
   $(MAKE) /$(MAKEFLAGS) /F ".\libapr.mak" CFG="libapr - Win32 Release" 
   cd "..\.."

"libapr - Win32 ReleaseCLEAN" : 
   cd ".\srclib\apr"
   $(MAKE) /$(MAKEFLAGS) /F ".\libapr.mak" CFG="libapr - Win32 Release" RECURSE=1 CLEAN 
   cd "..\.."

!ELSEIF  "$(CFG)" == "libhttpd - Win32 Debug"

"libapr - Win32 Debug" : 
   cd ".\srclib\apr"
   $(MAKE) /$(MAKEFLAGS) /F ".\libapr.mak" CFG="libapr - Win32 Debug" 
   cd "..\.."

"libapr - Win32 DebugCLEAN" : 
   cd ".\srclib\apr"
   $(MAKE) /$(MAKEFLAGS) /F ".\libapr.mak" CFG="libapr - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\.."

!ENDIF 

!IF  "$(CFG)" == "libhttpd - Win32 Release"

"libapriconv - Win32 Release" : 
   cd ".\srclib\apr-iconv"
   $(MAKE) /$(MAKEFLAGS) /F ".\libapriconv.mak" CFG="libapriconv - Win32 Release" 
   cd "..\.."

"libapriconv - Win32 ReleaseCLEAN" : 
   cd ".\srclib\apr-iconv"
   $(MAKE) /$(MAKEFLAGS) /F ".\libapriconv.mak" CFG="libapriconv - Win32 Release" RECURSE=1 CLEAN 
   cd "..\.."

!ELSEIF  "$(CFG)" == "libhttpd - Win32 Debug"

"libapriconv - Win32 Debug" : 
   cd ".\srclib\apr-iconv"
   $(MAKE) /$(MAKEFLAGS) /F ".\libapriconv.mak" CFG="libapriconv - Win32 Debug" 
   cd "..\.."

"libapriconv - Win32 DebugCLEAN" : 
   cd ".\srclib\apr-iconv"
   $(MAKE) /$(MAKEFLAGS) /F ".\libapriconv.mak" CFG="libapriconv - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\.."

!ENDIF 

!IF  "$(CFG)" == "libhttpd - Win32 Release"

"libaprutil - Win32 Release" : 
   cd ".\srclib\apr-util"
   $(MAKE) /$(MAKEFLAGS) /F ".\libaprutil.mak" CFG="libaprutil - Win32 Release" 
   cd "..\.."

"libaprutil - Win32 ReleaseCLEAN" : 
   cd ".\srclib\apr-util"
   $(MAKE) /$(MAKEFLAGS) /F ".\libaprutil.mak" CFG="libaprutil - Win32 Release" RECURSE=1 CLEAN 
   cd "..\.."

!ELSEIF  "$(CFG)" == "libhttpd - Win32 Debug"

"libaprutil - Win32 Debug" : 
   cd ".\srclib\apr-util"
   $(MAKE) /$(MAKEFLAGS) /F ".\libaprutil.mak" CFG="libaprutil - Win32 Debug" 
   cd "..\.."

"libaprutil - Win32 DebugCLEAN" : 
   cd ".\srclib\apr-util"
   $(MAKE) /$(MAKEFLAGS) /F ".\libaprutil.mak" CFG="libaprutil - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\.."

!ENDIF 

!IF  "$(CFG)" == "libhttpd - Win32 Release"

"gen_test_char - Win32 Release" : 
   cd ".\server"
   $(MAKE) /$(MAKEFLAGS) /F ".\gen_test_char.mak" CFG="gen_test_char - Win32 Release" 
   cd ".."

"gen_test_char - Win32 ReleaseCLEAN" : 
   cd ".\server"
   $(MAKE) /$(MAKEFLAGS) /F ".\gen_test_char.mak" CFG="gen_test_char - Win32 Release" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "libhttpd - Win32 Debug"

"gen_test_char - Win32 Debug" : 
   cd ".\server"
   $(MAKE) /$(MAKEFLAGS) /F ".\gen_test_char.mak" CFG="gen_test_char - Win32 Debug" 
   cd ".."

"gen_test_char - Win32 DebugCLEAN" : 
   cd ".\server"
   $(MAKE) /$(MAKEFLAGS) /F ".\gen_test_char.mak" CFG="gen_test_char - Win32 Debug" RECURSE=1 CLEAN 
   cd ".."

!ENDIF 

!IF  "$(CFG)" == "libhttpd - Win32 Release"

"pcre - Win32 Release" : 
   cd ".\srclib\pcre"
   $(MAKE) /$(MAKEFLAGS) /F ".\pcre.mak" CFG="pcre - Win32 Release" 
   cd "..\.."

"pcre - Win32 ReleaseCLEAN" : 
   cd ".\srclib\pcre"
   $(MAKE) /$(MAKEFLAGS) /F ".\pcre.mak" CFG="pcre - Win32 Release" RECURSE=1 CLEAN 
   cd "..\.."

!ELSEIF  "$(CFG)" == "libhttpd - Win32 Debug"

"pcre - Win32 Debug" : 
   cd ".\srclib\pcre"
   $(MAKE) /$(MAKEFLAGS) /F ".\pcre.mak" CFG="pcre - Win32 Debug" 
   cd "..\.."

"pcre - Win32 DebugCLEAN" : 
   cd ".\srclib\pcre"
   $(MAKE) /$(MAKEFLAGS) /F ".\pcre.mak" CFG="pcre - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\.."

!ENDIF 

SOURCE=.\server\gen_test_char.exe

!IF  "$(CFG)" == "libhttpd - Win32 Release"

InputPath=.\server\gen_test_char.exe
USERDEP__GEN_T=".\include\os.h"	

".\server\test_char.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__GEN_T)
	<<tempfile75.bat 
	@echo off 
	.\server\gen_test_char.exe >.\server\test_char.h
<< 
	

!ELSEIF  "$(CFG)" == "libhttpd - Win32 Debug"

InputPath=.\server\gen_test_char.exe
USERDEP__GEN_T=".\include\os.h"	

".\server\test_char.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__GEN_T)
	<<tempfile76.bat 
	@echo off 
	.\server\gen_test_char.exe >.\server\test_char.h
<< 
	

!ENDIF 

SOURCE=.\build\win32\httpd.rc

!IF  "$(CFG)" == "libhttpd - Win32 Release"


"$(INTDIR)\libhttpd.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\libhttpd.res" /i "./include" /i "./srclib/apr/include" /i "build\win32" /d "NDEBUG" /d BIN_NAME="libhttpd.dll" /d LONG_NAME="Apache HTTP Server Core" $(SOURCE)


!ELSEIF  "$(CFG)" == "libhttpd - Win32 Debug"


"$(INTDIR)\libhttpd.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\libhttpd.res" /i "./include" /i "./srclib/apr/include" /i "build\win32" /d "_DEBUG" /d BIN_NAME="libhttpd.dll" /d LONG_NAME="Apache HTTP Server Core" $(SOURCE)


!ENDIF 


!ENDIF 

