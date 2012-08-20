# Microsoft Developer Studio Generated NMAKE File, Based on libaprutil.dsp
!IF "$(CFG)" == ""
CFG=libaprutil - Win32 Release
!MESSAGE No configuration specified. Defaulting to libaprutil - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "libaprutil - Win32 Release" && "$(CFG)" != "libaprutil - Win32 Debug" && "$(CFG)" != "libaprutil - x64 Release" && "$(CFG)" != "libaprutil - x64 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libaprutil.mak" CFG="libaprutil - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libaprutil - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libaprutil - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libaprutil - x64 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libaprutil - x64 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "libaprutil - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : ".\include\apu_want.h" "$(OUTDIR)\libaprutil-1.dll" "$(DS_POSTBUILD_DEP)"

!ELSE 

ALL : "xml - Win32 Release" "libapriconv - Win32 Release" "libapr - Win32 Release" ".\include\apu_want.h" "$(OUTDIR)\libaprutil-1.dll" "$(DS_POSTBUILD_DEP)"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"libapr - Win32 ReleaseCLEAN" "libapriconv - Win32 ReleaseCLEAN" "xml - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\apr_base64.obj"
	-@erase "$(INTDIR)\apr_brigade.obj"
	-@erase "$(INTDIR)\apr_buckets.obj"
	-@erase "$(INTDIR)\apr_buckets_alloc.obj"
	-@erase "$(INTDIR)\apr_buckets_eos.obj"
	-@erase "$(INTDIR)\apr_buckets_file.obj"
	-@erase "$(INTDIR)\apr_buckets_flush.obj"
	-@erase "$(INTDIR)\apr_buckets_heap.obj"
	-@erase "$(INTDIR)\apr_buckets_mmap.obj"
	-@erase "$(INTDIR)\apr_buckets_pipe.obj"
	-@erase "$(INTDIR)\apr_buckets_pool.obj"
	-@erase "$(INTDIR)\apr_buckets_refcount.obj"
	-@erase "$(INTDIR)\apr_buckets_simple.obj"
	-@erase "$(INTDIR)\apr_buckets_socket.obj"
	-@erase "$(INTDIR)\apr_crypto.obj"
	-@erase "$(INTDIR)\apr_date.obj"
	-@erase "$(INTDIR)\apr_dbd.obj"
	-@erase "$(INTDIR)\apr_dbm.obj"
	-@erase "$(INTDIR)\apr_dbm_sdbm.obj"
	-@erase "$(INTDIR)\apr_hooks.obj"
	-@erase "$(INTDIR)\apr_ldap_stub.obj"
	-@erase "$(INTDIR)\apr_ldap_url.obj"
	-@erase "$(INTDIR)\apr_md4.obj"
	-@erase "$(INTDIR)\apr_md5.obj"
	-@erase "$(INTDIR)\apr_memcache.obj"
	-@erase "$(INTDIR)\apr_queue.obj"
	-@erase "$(INTDIR)\apr_reslist.obj"
	-@erase "$(INTDIR)\apr_rmm.obj"
	-@erase "$(INTDIR)\apr_sha1.obj"
	-@erase "$(INTDIR)\apr_strmatch.obj"
	-@erase "$(INTDIR)\apr_thread_pool.obj"
	-@erase "$(INTDIR)\apr_uri.obj"
	-@erase "$(INTDIR)\apr_xml.obj"
	-@erase "$(INTDIR)\apu_dso.obj"
	-@erase "$(INTDIR)\apu_version.obj"
	-@erase "$(INTDIR)\getuuid.obj"
	-@erase "$(INTDIR)\libaprutil.res"
	-@erase "$(INTDIR)\libaprutil_src.idb"
	-@erase "$(INTDIR)\libaprutil_src.pdb"
	-@erase "$(INTDIR)\sdbm.obj"
	-@erase "$(INTDIR)\sdbm_hash.obj"
	-@erase "$(INTDIR)\sdbm_lock.obj"
	-@erase "$(INTDIR)\sdbm_pair.obj"
	-@erase "$(INTDIR)\uuid.obj"
	-@erase "$(INTDIR)\xlate.obj"
	-@erase "$(OUTDIR)\libaprutil-1.dll"
	-@erase "$(OUTDIR)\libaprutil-1.exp"
	-@erase "$(OUTDIR)\libaprutil-1.lib"
	-@erase "$(OUTDIR)\libaprutil-1.pdb"
	-@erase ".\include\apu_want.h"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /Zi /O2 /Oy- /I "./include" /I "../apr/include" /I "./include/private" /I "../apr-iconv/include" /I "./dbm/sdbm" /I "./xml/expat/lib" /D "NDEBUG" /D "APU_DECLARE_EXPORT" /D "APU_USE_SDBM" /D "XML_STATIC" /D "WIN32" /D "_WINDOWS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\libaprutil_src" /FD /c 

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
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL" 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\libaprutil.res" /i "./include" /i "../apr/include" /d "NDEBUG" /d "APU_VERSION_ONLY" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libaprutil.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib advapi32.lib ws2_32.lib mswsock.lib ole32.lib /nologo /base:"0x6EE60000" /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\libaprutil-1.pdb" /debug /out:"$(OUTDIR)\libaprutil-1.dll" /implib:"$(OUTDIR)\libaprutil-1.lib"   /opt:ref 
LINK32_OBJS= \
	"$(INTDIR)\apr_brigade.obj" \
	"$(INTDIR)\apr_buckets.obj" \
	"$(INTDIR)\apr_buckets_alloc.obj" \
	"$(INTDIR)\apr_buckets_eos.obj" \
	"$(INTDIR)\apr_buckets_file.obj" \
	"$(INTDIR)\apr_buckets_flush.obj" \
	"$(INTDIR)\apr_buckets_heap.obj" \
	"$(INTDIR)\apr_buckets_mmap.obj" \
	"$(INTDIR)\apr_buckets_pipe.obj" \
	"$(INTDIR)\apr_buckets_pool.obj" \
	"$(INTDIR)\apr_buckets_refcount.obj" \
	"$(INTDIR)\apr_buckets_simple.obj" \
	"$(INTDIR)\apr_buckets_socket.obj" \
	"$(INTDIR)\apr_crypto.obj" \
	"$(INTDIR)\apr_md4.obj" \
	"$(INTDIR)\apr_md5.obj" \
	"$(INTDIR)\apr_sha1.obj" \
	"$(INTDIR)\getuuid.obj" \
	"$(INTDIR)\uuid.obj" \
	"$(INTDIR)\apr_dbd.obj" \
	"$(INTDIR)\apr_dbm.obj" \
	"$(INTDIR)\apr_dbm_sdbm.obj" \
	"$(INTDIR)\apr_base64.obj" \
	"$(INTDIR)\apr_hooks.obj" \
	"$(INTDIR)\apr_ldap_stub.obj" \
	"$(INTDIR)\apr_ldap_url.obj" \
	"$(INTDIR)\apr_memcache.obj" \
	"$(INTDIR)\apr_date.obj" \
	"$(INTDIR)\apu_dso.obj" \
	"$(INTDIR)\apr_queue.obj" \
	"$(INTDIR)\apr_reslist.obj" \
	"$(INTDIR)\apr_rmm.obj" \
	"$(INTDIR)\apr_thread_pool.obj" \
	"$(INTDIR)\apu_version.obj" \
	"$(INTDIR)\sdbm.obj" \
	"$(INTDIR)\sdbm_hash.obj" \
	"$(INTDIR)\sdbm_lock.obj" \
	"$(INTDIR)\sdbm_pair.obj" \
	"$(INTDIR)\apr_strmatch.obj" \
	"$(INTDIR)\apr_uri.obj" \
	"$(INTDIR)\xlate.obj" \
	"$(INTDIR)\apr_xml.obj" \
	"$(INTDIR)\libaprutil.res" \
	"..\apr\Release\libapr-1.lib" \
	"..\apr-iconv\Release\libapriconv-1.lib" \
	".\xml\expat\lib\LibR\xml.lib"

"$(OUTDIR)\libaprutil-1.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

TargetPath=.\Release\libaprutil-1.dll
SOURCE="$(InputPath)"
PostBuild_Desc=Embed .manifest
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

"$(DS_POSTBUILD_DEP)" : "$(OUTDIR)\libaprutil-1.dll"
   if exist .\Release\libaprutil-1.dll.manifest mt.exe -manifest .\Release\libaprutil-1.dll.manifest -outputresource:.\Release\libaprutil-1.dll;2
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "libaprutil - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : ".\include\private\apu_select_dbm.h" ".\include\private\apu_config.h" ".\include\apu_want.h" ".\include\apu.h" ".\include\apr_ldap.h" "$(OUTDIR)\libaprutil-1.dll" "$(DS_POSTBUILD_DEP)"

!ELSE 

ALL : "xml - Win32 Debug" "libapriconv - Win32 Debug" "libapr - Win32 Debug" ".\include\private\apu_select_dbm.h" ".\include\private\apu_config.h" ".\include\apu_want.h" ".\include\apu.h" ".\include\apr_ldap.h" "$(OUTDIR)\libaprutil-1.dll" "$(DS_POSTBUILD_DEP)"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"libapr - Win32 DebugCLEAN" "libapriconv - Win32 DebugCLEAN" "xml - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\apr_base64.obj"
	-@erase "$(INTDIR)\apr_brigade.obj"
	-@erase "$(INTDIR)\apr_buckets.obj"
	-@erase "$(INTDIR)\apr_buckets_alloc.obj"
	-@erase "$(INTDIR)\apr_buckets_eos.obj"
	-@erase "$(INTDIR)\apr_buckets_file.obj"
	-@erase "$(INTDIR)\apr_buckets_flush.obj"
	-@erase "$(INTDIR)\apr_buckets_heap.obj"
	-@erase "$(INTDIR)\apr_buckets_mmap.obj"
	-@erase "$(INTDIR)\apr_buckets_pipe.obj"
	-@erase "$(INTDIR)\apr_buckets_pool.obj"
	-@erase "$(INTDIR)\apr_buckets_refcount.obj"
	-@erase "$(INTDIR)\apr_buckets_simple.obj"
	-@erase "$(INTDIR)\apr_buckets_socket.obj"
	-@erase "$(INTDIR)\apr_crypto.obj"
	-@erase "$(INTDIR)\apr_date.obj"
	-@erase "$(INTDIR)\apr_dbd.obj"
	-@erase "$(INTDIR)\apr_dbm.obj"
	-@erase "$(INTDIR)\apr_dbm_sdbm.obj"
	-@erase "$(INTDIR)\apr_hooks.obj"
	-@erase "$(INTDIR)\apr_ldap_stub.obj"
	-@erase "$(INTDIR)\apr_ldap_url.obj"
	-@erase "$(INTDIR)\apr_md4.obj"
	-@erase "$(INTDIR)\apr_md5.obj"
	-@erase "$(INTDIR)\apr_memcache.obj"
	-@erase "$(INTDIR)\apr_queue.obj"
	-@erase "$(INTDIR)\apr_reslist.obj"
	-@erase "$(INTDIR)\apr_rmm.obj"
	-@erase "$(INTDIR)\apr_sha1.obj"
	-@erase "$(INTDIR)\apr_strmatch.obj"
	-@erase "$(INTDIR)\apr_thread_pool.obj"
	-@erase "$(INTDIR)\apr_uri.obj"
	-@erase "$(INTDIR)\apr_xml.obj"
	-@erase "$(INTDIR)\apu_dso.obj"
	-@erase "$(INTDIR)\apu_version.obj"
	-@erase "$(INTDIR)\getuuid.obj"
	-@erase "$(INTDIR)\libaprutil.res"
	-@erase "$(INTDIR)\libaprutil_src.idb"
	-@erase "$(INTDIR)\libaprutil_src.pdb"
	-@erase "$(INTDIR)\sdbm.obj"
	-@erase "$(INTDIR)\sdbm_hash.obj"
	-@erase "$(INTDIR)\sdbm_lock.obj"
	-@erase "$(INTDIR)\sdbm_pair.obj"
	-@erase "$(INTDIR)\uuid.obj"
	-@erase "$(INTDIR)\xlate.obj"
	-@erase "$(OUTDIR)\libaprutil-1.dll"
	-@erase "$(OUTDIR)\libaprutil-1.exp"
	-@erase "$(OUTDIR)\libaprutil-1.lib"
	-@erase "$(OUTDIR)\libaprutil-1.pdb"
	-@erase ".\include\apr_ldap.h"
	-@erase ".\include\apu.h"
	-@erase ".\include\apu_want.h"
	-@erase ".\include\private\apu_config.h"
	-@erase ".\include\private\apu_select_dbm.h"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Zi /Od /I "./include" /I "../apr/include" /I "./include/private" /I "../apr-iconv/include" /I "./dbm/sdbm" /I "./xml/expat/lib" /D "_DEBUG" /D "APU_DECLARE_EXPORT" /D "APU_USE_SDBM" /D "XML_STATIC" /D "WIN32" /D "_WINDOWS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\libaprutil_src" /FD /EHsc /c 

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
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL" 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\libaprutil.res" /i "./include" /i "../apr/include" /d "_DEBUG" /d "APU_VERSION_ONLY" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libaprutil.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib advapi32.lib ws2_32.lib mswsock.lib ole32.lib /nologo /base:"0x6EE60000" /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\libaprutil-1.pdb" /debug /out:"$(OUTDIR)\libaprutil-1.dll" /implib:"$(OUTDIR)\libaprutil-1.lib"   
LINK32_OBJS= \
	"$(INTDIR)\apr_brigade.obj" \
	"$(INTDIR)\apr_buckets.obj" \
	"$(INTDIR)\apr_buckets_alloc.obj" \
	"$(INTDIR)\apr_buckets_eos.obj" \
	"$(INTDIR)\apr_buckets_file.obj" \
	"$(INTDIR)\apr_buckets_flush.obj" \
	"$(INTDIR)\apr_buckets_heap.obj" \
	"$(INTDIR)\apr_buckets_mmap.obj" \
	"$(INTDIR)\apr_buckets_pipe.obj" \
	"$(INTDIR)\apr_buckets_pool.obj" \
	"$(INTDIR)\apr_buckets_refcount.obj" \
	"$(INTDIR)\apr_buckets_simple.obj" \
	"$(INTDIR)\apr_buckets_socket.obj" \
	"$(INTDIR)\apr_crypto.obj" \
	"$(INTDIR)\apr_md4.obj" \
	"$(INTDIR)\apr_md5.obj" \
	"$(INTDIR)\apr_sha1.obj" \
	"$(INTDIR)\getuuid.obj" \
	"$(INTDIR)\uuid.obj" \
	"$(INTDIR)\apr_dbd.obj" \
	"$(INTDIR)\apr_dbm.obj" \
	"$(INTDIR)\apr_dbm_sdbm.obj" \
	"$(INTDIR)\apr_base64.obj" \
	"$(INTDIR)\apr_hooks.obj" \
	"$(INTDIR)\apr_ldap_stub.obj" \
	"$(INTDIR)\apr_ldap_url.obj" \
	"$(INTDIR)\apr_memcache.obj" \
	"$(INTDIR)\apr_date.obj" \
	"$(INTDIR)\apu_dso.obj" \
	"$(INTDIR)\apr_queue.obj" \
	"$(INTDIR)\apr_reslist.obj" \
	"$(INTDIR)\apr_rmm.obj" \
	"$(INTDIR)\apr_thread_pool.obj" \
	"$(INTDIR)\apu_version.obj" \
	"$(INTDIR)\sdbm.obj" \
	"$(INTDIR)\sdbm_hash.obj" \
	"$(INTDIR)\sdbm_lock.obj" \
	"$(INTDIR)\sdbm_pair.obj" \
	"$(INTDIR)\apr_strmatch.obj" \
	"$(INTDIR)\apr_uri.obj" \
	"$(INTDIR)\xlate.obj" \
	"$(INTDIR)\apr_xml.obj" \
	"$(INTDIR)\libaprutil.res" \
	"..\apr\Debug\libapr-1.lib" \
	"..\apr-iconv\Debug\libapriconv-1.lib" \
	".\xml\expat\lib\LibD\xml.lib"

"$(OUTDIR)\libaprutil-1.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

TargetPath=.\Debug\libaprutil-1.dll
SOURCE="$(InputPath)"
PostBuild_Desc=Embed .manifest
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

"$(DS_POSTBUILD_DEP)" : "$(OUTDIR)\libaprutil-1.dll"
   if exist .\Debug\libaprutil-1.dll.manifest mt.exe -manifest .\Debug\libaprutil-1.dll.manifest -outputresource:.\Debug\libaprutil-1.dll;2
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "libaprutil - x64 Release"

OUTDIR=.\x64\Release
INTDIR=.\x64\Release
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep
# Begin Custom Macros
OutDir=.\x64\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : ".\include\private\apu_select_dbm.h" ".\include\private\apu_config.h" ".\include\apu_want.h" ".\include\apu.h" ".\include\apr_ldap.h" "$(OUTDIR)\libaprutil-1.dll" "$(DS_POSTBUILD_DEP)"

!ELSE 

ALL : "xml - x64 Release" "libapriconv - x64 Release" "libapr - x64 Release" ".\include\private\apu_select_dbm.h" ".\include\private\apu_config.h" ".\include\apu_want.h" ".\include\apu.h" ".\include\apr_ldap.h" "$(OUTDIR)\libaprutil-1.dll" "$(DS_POSTBUILD_DEP)"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"libapr - x64 ReleaseCLEAN" "libapriconv - x64 ReleaseCLEAN" "xml - x64 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\apr_base64.obj"
	-@erase "$(INTDIR)\apr_brigade.obj"
	-@erase "$(INTDIR)\apr_buckets.obj"
	-@erase "$(INTDIR)\apr_buckets_alloc.obj"
	-@erase "$(INTDIR)\apr_buckets_eos.obj"
	-@erase "$(INTDIR)\apr_buckets_file.obj"
	-@erase "$(INTDIR)\apr_buckets_flush.obj"
	-@erase "$(INTDIR)\apr_buckets_heap.obj"
	-@erase "$(INTDIR)\apr_buckets_mmap.obj"
	-@erase "$(INTDIR)\apr_buckets_pipe.obj"
	-@erase "$(INTDIR)\apr_buckets_pool.obj"
	-@erase "$(INTDIR)\apr_buckets_refcount.obj"
	-@erase "$(INTDIR)\apr_buckets_simple.obj"
	-@erase "$(INTDIR)\apr_buckets_socket.obj"
	-@erase "$(INTDIR)\apr_crypto.obj"
	-@erase "$(INTDIR)\apr_date.obj"
	-@erase "$(INTDIR)\apr_dbd.obj"
	-@erase "$(INTDIR)\apr_dbm.obj"
	-@erase "$(INTDIR)\apr_dbm_sdbm.obj"
	-@erase "$(INTDIR)\apr_hooks.obj"
	-@erase "$(INTDIR)\apr_ldap_stub.obj"
	-@erase "$(INTDIR)\apr_ldap_url.obj"
	-@erase "$(INTDIR)\apr_md4.obj"
	-@erase "$(INTDIR)\apr_md5.obj"
	-@erase "$(INTDIR)\apr_memcache.obj"
	-@erase "$(INTDIR)\apr_queue.obj"
	-@erase "$(INTDIR)\apr_reslist.obj"
	-@erase "$(INTDIR)\apr_rmm.obj"
	-@erase "$(INTDIR)\apr_sha1.obj"
	-@erase "$(INTDIR)\apr_strmatch.obj"
	-@erase "$(INTDIR)\apr_thread_pool.obj"
	-@erase "$(INTDIR)\apr_uri.obj"
	-@erase "$(INTDIR)\apr_xml.obj"
	-@erase "$(INTDIR)\apu_dso.obj"
	-@erase "$(INTDIR)\apu_version.obj"
	-@erase "$(INTDIR)\getuuid.obj"
	-@erase "$(INTDIR)\libaprutil.res"
	-@erase "$(INTDIR)\libaprutil_src.idb"
	-@erase "$(INTDIR)\libaprutil_src.pdb"
	-@erase "$(INTDIR)\sdbm.obj"
	-@erase "$(INTDIR)\sdbm_hash.obj"
	-@erase "$(INTDIR)\sdbm_lock.obj"
	-@erase "$(INTDIR)\sdbm_pair.obj"
	-@erase "$(INTDIR)\uuid.obj"
	-@erase "$(INTDIR)\xlate.obj"
	-@erase "$(OUTDIR)\libaprutil-1.dll"
	-@erase "$(OUTDIR)\libaprutil-1.exp"
	-@erase "$(OUTDIR)\libaprutil-1.lib"
	-@erase "$(OUTDIR)\libaprutil-1.pdb"
	-@erase ".\include\apr_ldap.h"
	-@erase ".\include\apu.h"
	-@erase ".\include\apu_want.h"
	-@erase ".\include\private\apu_config.h"
	-@erase ".\include\private\apu_select_dbm.h"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /Zi /O2 /Oy- /I "./include" /I "../apr/include" /I "./include/private" /I "../apr-iconv/include" /I "./dbm/sdbm" /I "./xml/expat/lib" /D "NDEBUG" /D "APU_DECLARE_EXPORT" /D "APU_USE_SDBM" /D "XML_STATIC" /D "WIN32" /D "_WINDOWS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\libaprutil_src" /FD /c 

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
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL" 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\libaprutil.res" /i "./include" /i "../apr/include" /d "NDEBUG" /d "APU_VERSION_ONLY" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libaprutil.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib advapi32.lib ws2_32.lib mswsock.lib ole32.lib /nologo /base:"0x6EE60000" /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\libaprutil-1.pdb" /debug /out:"$(OUTDIR)\libaprutil-1.dll" /implib:"$(OUTDIR)\libaprutil-1.lib" /MACHINE:X64 /opt:ref 
LINK32_OBJS= \
	"$(INTDIR)\apr_brigade.obj" \
	"$(INTDIR)\apr_buckets.obj" \
	"$(INTDIR)\apr_buckets_alloc.obj" \
	"$(INTDIR)\apr_buckets_eos.obj" \
	"$(INTDIR)\apr_buckets_file.obj" \
	"$(INTDIR)\apr_buckets_flush.obj" \
	"$(INTDIR)\apr_buckets_heap.obj" \
	"$(INTDIR)\apr_buckets_mmap.obj" \
	"$(INTDIR)\apr_buckets_pipe.obj" \
	"$(INTDIR)\apr_buckets_pool.obj" \
	"$(INTDIR)\apr_buckets_refcount.obj" \
	"$(INTDIR)\apr_buckets_simple.obj" \
	"$(INTDIR)\apr_buckets_socket.obj" \
	"$(INTDIR)\apr_crypto.obj" \
	"$(INTDIR)\apr_md4.obj" \
	"$(INTDIR)\apr_md5.obj" \
	"$(INTDIR)\apr_sha1.obj" \
	"$(INTDIR)\getuuid.obj" \
	"$(INTDIR)\uuid.obj" \
	"$(INTDIR)\apr_dbd.obj" \
	"$(INTDIR)\apr_dbm.obj" \
	"$(INTDIR)\apr_dbm_sdbm.obj" \
	"$(INTDIR)\apr_base64.obj" \
	"$(INTDIR)\apr_hooks.obj" \
	"$(INTDIR)\apr_ldap_stub.obj" \
	"$(INTDIR)\apr_ldap_url.obj" \
	"$(INTDIR)\apr_memcache.obj" \
	"$(INTDIR)\apr_date.obj" \
	"$(INTDIR)\apu_dso.obj" \
	"$(INTDIR)\apr_queue.obj" \
	"$(INTDIR)\apr_reslist.obj" \
	"$(INTDIR)\apr_rmm.obj" \
	"$(INTDIR)\apr_thread_pool.obj" \
	"$(INTDIR)\apu_version.obj" \
	"$(INTDIR)\sdbm.obj" \
	"$(INTDIR)\sdbm_hash.obj" \
	"$(INTDIR)\sdbm_lock.obj" \
	"$(INTDIR)\sdbm_pair.obj" \
	"$(INTDIR)\apr_strmatch.obj" \
	"$(INTDIR)\apr_uri.obj" \
	"$(INTDIR)\xlate.obj" \
	"$(INTDIR)\apr_xml.obj" \
	"$(INTDIR)\libaprutil.res" \
	"..\apr\x64\Release\libapr-1.lib" \
	"..\apr-iconv\x64\Release\libapriconv-1.lib" \
	".\xml\expat\lib\x64\LibR\xml.lib"

"$(OUTDIR)\libaprutil-1.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

TargetPath=.\x64\Release\libaprutil-1.dll
SOURCE="$(InputPath)"
PostBuild_Desc=Embed .manifest
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

# Begin Custom Macros
OutDir=.\x64\Release
# End Custom Macros

"$(DS_POSTBUILD_DEP)" : "$(OUTDIR)\libaprutil-1.dll"
   if exist .\x64\Release\libaprutil-1.dll.manifest mt.exe -manifest .\x64\Release\libaprutil-1.dll.manifest -outputresource:.\x64\Release\libaprutil-1.dll;2
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "libaprutil - x64 Debug"

OUTDIR=.\x64\Debug
INTDIR=.\x64\Debug
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep
# Begin Custom Macros
OutDir=.\x64\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : ".\include\private\apu_select_dbm.h" ".\include\private\apu_config.h" ".\include\apu_want.h" ".\include\apu.h" ".\include\apr_ldap.h" "$(OUTDIR)\libaprutil-1.dll" "$(DS_POSTBUILD_DEP)"

!ELSE 

ALL : "xml - x64 Debug" "libapriconv - x64 Debug" "libapr - x64 Debug" ".\include\private\apu_select_dbm.h" ".\include\private\apu_config.h" ".\include\apu_want.h" ".\include\apu.h" ".\include\apr_ldap.h" "$(OUTDIR)\libaprutil-1.dll" "$(DS_POSTBUILD_DEP)"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"libapr - x64 DebugCLEAN" "libapriconv - x64 DebugCLEAN" "xml - x64 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\apr_base64.obj"
	-@erase "$(INTDIR)\apr_brigade.obj"
	-@erase "$(INTDIR)\apr_buckets.obj"
	-@erase "$(INTDIR)\apr_buckets_alloc.obj"
	-@erase "$(INTDIR)\apr_buckets_eos.obj"
	-@erase "$(INTDIR)\apr_buckets_file.obj"
	-@erase "$(INTDIR)\apr_buckets_flush.obj"
	-@erase "$(INTDIR)\apr_buckets_heap.obj"
	-@erase "$(INTDIR)\apr_buckets_mmap.obj"
	-@erase "$(INTDIR)\apr_buckets_pipe.obj"
	-@erase "$(INTDIR)\apr_buckets_pool.obj"
	-@erase "$(INTDIR)\apr_buckets_refcount.obj"
	-@erase "$(INTDIR)\apr_buckets_simple.obj"
	-@erase "$(INTDIR)\apr_buckets_socket.obj"
	-@erase "$(INTDIR)\apr_crypto.obj"
	-@erase "$(INTDIR)\apr_date.obj"
	-@erase "$(INTDIR)\apr_dbd.obj"
	-@erase "$(INTDIR)\apr_dbm.obj"
	-@erase "$(INTDIR)\apr_dbm_sdbm.obj"
	-@erase "$(INTDIR)\apr_hooks.obj"
	-@erase "$(INTDIR)\apr_ldap_stub.obj"
	-@erase "$(INTDIR)\apr_ldap_url.obj"
	-@erase "$(INTDIR)\apr_md4.obj"
	-@erase "$(INTDIR)\apr_md5.obj"
	-@erase "$(INTDIR)\apr_memcache.obj"
	-@erase "$(INTDIR)\apr_queue.obj"
	-@erase "$(INTDIR)\apr_reslist.obj"
	-@erase "$(INTDIR)\apr_rmm.obj"
	-@erase "$(INTDIR)\apr_sha1.obj"
	-@erase "$(INTDIR)\apr_strmatch.obj"
	-@erase "$(INTDIR)\apr_thread_pool.obj"
	-@erase "$(INTDIR)\apr_uri.obj"
	-@erase "$(INTDIR)\apr_xml.obj"
	-@erase "$(INTDIR)\apu_dso.obj"
	-@erase "$(INTDIR)\apu_version.obj"
	-@erase "$(INTDIR)\getuuid.obj"
	-@erase "$(INTDIR)\libaprutil.res"
	-@erase "$(INTDIR)\libaprutil_src.idb"
	-@erase "$(INTDIR)\libaprutil_src.pdb"
	-@erase "$(INTDIR)\sdbm.obj"
	-@erase "$(INTDIR)\sdbm_hash.obj"
	-@erase "$(INTDIR)\sdbm_lock.obj"
	-@erase "$(INTDIR)\sdbm_pair.obj"
	-@erase "$(INTDIR)\uuid.obj"
	-@erase "$(INTDIR)\xlate.obj"
	-@erase "$(OUTDIR)\libaprutil-1.dll"
	-@erase "$(OUTDIR)\libaprutil-1.exp"
	-@erase "$(OUTDIR)\libaprutil-1.lib"
	-@erase "$(OUTDIR)\libaprutil-1.pdb"
	-@erase ".\include\apr_ldap.h"
	-@erase ".\include\apu.h"
	-@erase ".\include\apu_want.h"
	-@erase ".\include\private\apu_config.h"
	-@erase ".\include\private\apu_select_dbm.h"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Zi /Od /I "./include" /I "../apr/include" /I "./include/private" /I "../apr-iconv/include" /I "./dbm/sdbm" /I "./xml/expat/lib" /D "_DEBUG" /D "APU_DECLARE_EXPORT" /D "APU_USE_SDBM" /D "XML_STATIC" /D "WIN32" /D "_WINDOWS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\libaprutil_src" /FD /EHsc /c 

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
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL" 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\libaprutil.res" /i "./include" /i "../apr/include" /d "_DEBUG" /d "APU_VERSION_ONLY" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libaprutil.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib advapi32.lib ws2_32.lib mswsock.lib ole32.lib /nologo /base:"0x6EE60000" /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\libaprutil-1.pdb" /debug /out:"$(OUTDIR)\libaprutil-1.dll" /implib:"$(OUTDIR)\libaprutil-1.lib" /MACHINE:X64 
LINK32_OBJS= \
	"$(INTDIR)\apr_brigade.obj" \
	"$(INTDIR)\apr_buckets.obj" \
	"$(INTDIR)\apr_buckets_alloc.obj" \
	"$(INTDIR)\apr_buckets_eos.obj" \
	"$(INTDIR)\apr_buckets_file.obj" \
	"$(INTDIR)\apr_buckets_flush.obj" \
	"$(INTDIR)\apr_buckets_heap.obj" \
	"$(INTDIR)\apr_buckets_mmap.obj" \
	"$(INTDIR)\apr_buckets_pipe.obj" \
	"$(INTDIR)\apr_buckets_pool.obj" \
	"$(INTDIR)\apr_buckets_refcount.obj" \
	"$(INTDIR)\apr_buckets_simple.obj" \
	"$(INTDIR)\apr_buckets_socket.obj" \
	"$(INTDIR)\apr_crypto.obj" \
	"$(INTDIR)\apr_md4.obj" \
	"$(INTDIR)\apr_md5.obj" \
	"$(INTDIR)\apr_sha1.obj" \
	"$(INTDIR)\getuuid.obj" \
	"$(INTDIR)\uuid.obj" \
	"$(INTDIR)\apr_dbd.obj" \
	"$(INTDIR)\apr_dbm.obj" \
	"$(INTDIR)\apr_dbm_sdbm.obj" \
	"$(INTDIR)\apr_base64.obj" \
	"$(INTDIR)\apr_hooks.obj" \
	"$(INTDIR)\apr_ldap_stub.obj" \
	"$(INTDIR)\apr_ldap_url.obj" \
	"$(INTDIR)\apr_memcache.obj" \
	"$(INTDIR)\apr_date.obj" \
	"$(INTDIR)\apu_dso.obj" \
	"$(INTDIR)\apr_queue.obj" \
	"$(INTDIR)\apr_reslist.obj" \
	"$(INTDIR)\apr_rmm.obj" \
	"$(INTDIR)\apr_thread_pool.obj" \
	"$(INTDIR)\apu_version.obj" \
	"$(INTDIR)\sdbm.obj" \
	"$(INTDIR)\sdbm_hash.obj" \
	"$(INTDIR)\sdbm_lock.obj" \
	"$(INTDIR)\sdbm_pair.obj" \
	"$(INTDIR)\apr_strmatch.obj" \
	"$(INTDIR)\apr_uri.obj" \
	"$(INTDIR)\xlate.obj" \
	"$(INTDIR)\apr_xml.obj" \
	"$(INTDIR)\libaprutil.res" \
	"..\apr\x64\Debug\libapr-1.lib" \
	"..\apr-iconv\x64\Debug\libapriconv-1.lib" \
	".\xml\expat\lib\x64\LibD\xml.lib"

"$(OUTDIR)\libaprutil-1.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

TargetPath=.\x64\Debug\libaprutil-1.dll
SOURCE="$(InputPath)"
PostBuild_Desc=Embed .manifest
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

# Begin Custom Macros
OutDir=.\x64\Debug
# End Custom Macros

"$(DS_POSTBUILD_DEP)" : "$(OUTDIR)\libaprutil-1.dll"
   if exist .\x64\Debug\libaprutil-1.dll.manifest mt.exe -manifest .\x64\Debug\libaprutil-1.dll.manifest -outputresource:.\x64\Debug\libaprutil-1.dll;2
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("libaprutil.dep")
!INCLUDE "libaprutil.dep"
!ELSE 
!MESSAGE Warning: cannot find "libaprutil.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "libaprutil - Win32 Release" || "$(CFG)" == "libaprutil - Win32 Debug" || "$(CFG)" == "libaprutil - x64 Release" || "$(CFG)" == "libaprutil - x64 Debug"
SOURCE=.\buckets\apr_brigade.c

"$(INTDIR)\apr_brigade.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\buckets\apr_buckets.c

"$(INTDIR)\apr_buckets.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\buckets\apr_buckets_alloc.c

"$(INTDIR)\apr_buckets_alloc.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\buckets\apr_buckets_eos.c

"$(INTDIR)\apr_buckets_eos.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\buckets\apr_buckets_file.c

"$(INTDIR)\apr_buckets_file.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\buckets\apr_buckets_flush.c

"$(INTDIR)\apr_buckets_flush.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\buckets\apr_buckets_heap.c

"$(INTDIR)\apr_buckets_heap.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\buckets\apr_buckets_mmap.c

"$(INTDIR)\apr_buckets_mmap.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\buckets\apr_buckets_pipe.c

"$(INTDIR)\apr_buckets_pipe.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\buckets\apr_buckets_pool.c

"$(INTDIR)\apr_buckets_pool.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\buckets\apr_buckets_refcount.c

"$(INTDIR)\apr_buckets_refcount.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\buckets\apr_buckets_simple.c

"$(INTDIR)\apr_buckets_simple.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\buckets\apr_buckets_socket.c

"$(INTDIR)\apr_buckets_socket.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\crypto\apr_crypto.c

"$(INTDIR)\apr_crypto.obj" : $(SOURCE) "$(INTDIR)" ".\include\private\apu_config.h" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\crypto\apr_md4.c

"$(INTDIR)\apr_md4.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\crypto\apr_md5.c

"$(INTDIR)\apr_md5.obj" : $(SOURCE) "$(INTDIR)" ".\include\private\apu_config.h" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\crypto\apr_sha1.c

"$(INTDIR)\apr_sha1.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\crypto\getuuid.c

"$(INTDIR)\getuuid.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\crypto\uuid.c

"$(INTDIR)\uuid.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\dbd\apr_dbd.c

"$(INTDIR)\apr_dbd.obj" : $(SOURCE) "$(INTDIR)" ".\include\private\apu_config.h" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\dbd\apr_dbd_freetds.c
SOURCE=.\dbd\apr_dbd_mysql.c
SOURCE=.\dbd\apr_dbd_odbc.c
SOURCE=.\dbd\apr_dbd_oracle.c
SOURCE=.\dbd\apr_dbd_pgsql.c
SOURCE=.\dbd\apr_dbd_sqlite2.c
SOURCE=.\dbd\apr_dbd_sqlite3.c
SOURCE=.\dbm\apr_dbm.c

"$(INTDIR)\apr_dbm.obj" : $(SOURCE) "$(INTDIR)" ".\include\private\apu_config.h" ".\include\apu.h" ".\include\private\apu_select_dbm.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\dbm\apr_dbm_berkeleydb.c
SOURCE=.\dbm\apr_dbm_gdbm.c
SOURCE=.\dbm\apr_dbm_sdbm.c

"$(INTDIR)\apr_dbm_sdbm.obj" : $(SOURCE) "$(INTDIR)" ".\include\private\apu_config.h" ".\include\apu.h" ".\include\private\apu_select_dbm.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\encoding\apr_base64.c

"$(INTDIR)\apr_base64.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\hooks\apr_hooks.c

"$(INTDIR)\apr_hooks.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ldap\apr_ldap_init.c
SOURCE=.\ldap\apr_ldap_option.c
SOURCE=.\ldap\apr_ldap_rebind.c
SOURCE=.\ldap\apr_ldap_stub.c

"$(INTDIR)\apr_ldap_stub.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h" ".\include\private\apu_config.h" ".\include\apr_ldap.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ldap\apr_ldap_url.c

"$(INTDIR)\apr_ldap_url.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h" ".\include\apr_ldap.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\memcache\apr_memcache.c

"$(INTDIR)\apr_memcache.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\misc\apr_date.c

"$(INTDIR)\apr_date.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\misc\apr_queue.c

"$(INTDIR)\apr_queue.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\misc\apr_reslist.c

"$(INTDIR)\apr_reslist.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\misc\apr_rmm.c

"$(INTDIR)\apr_rmm.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\misc\apr_thread_pool.c

"$(INTDIR)\apr_thread_pool.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\misc\apu_dso.c

"$(INTDIR)\apu_dso.obj" : $(SOURCE) "$(INTDIR)" ".\include\private\apu_config.h" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\misc\apu_version.c

"$(INTDIR)\apu_version.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\dbm\sdbm\sdbm.c

"$(INTDIR)\sdbm.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\dbm\sdbm\sdbm_hash.c

"$(INTDIR)\sdbm_hash.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\dbm\sdbm\sdbm_lock.c

"$(INTDIR)\sdbm_lock.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\dbm\sdbm\sdbm_pair.c

"$(INTDIR)\sdbm_pair.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\strmatch\apr_strmatch.c

"$(INTDIR)\apr_strmatch.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\uri\apr_uri.c

"$(INTDIR)\apr_uri.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\xlate\xlate.c

"$(INTDIR)\xlate.obj" : $(SOURCE) "$(INTDIR)" ".\include\apu.h" ".\include\private\apu_config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\xml\apr_xml.c

"$(INTDIR)\apr_xml.obj" : $(SOURCE) "$(INTDIR)" ".\include\private\apu_config.h" ".\include\apu.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\include\apr_ldap.hw

!IF  "$(CFG)" == "libaprutil - Win32 Release"

InputPath=.\include\apr_ldap.hw

".\include\apr_ldap.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile37.bat 
	@echo off 
	type .\include\apr_ldap.hw > .\include\apr_ldap.h
<< 
	

!ELSEIF  "$(CFG)" == "libaprutil - Win32 Debug"

InputPath=.\include\apr_ldap.hw

".\include\apr_ldap.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile38.bat 
	@echo off 
	type .\include\apr_ldap.hw > .\include\apr_ldap.h
<< 
	

!ELSEIF  "$(CFG)" == "libaprutil - x64 Release"

InputPath=.\include\apr_ldap.hw

".\include\apr_ldap.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile39.bat 
	@echo off 
	type .\include\apr_ldap.hw > .\include\apr_ldap.h
<< 
	

!ELSEIF  "$(CFG)" == "libaprutil - x64 Debug"

InputPath=.\include\apr_ldap.hw

".\include\apr_ldap.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile40.bat 
	@echo off 
	type .\include\apr_ldap.hw > .\include\apr_ldap.h
<< 
	

!ENDIF 

SOURCE=.\include\apu.hw

!IF  "$(CFG)" == "libaprutil - Win32 Release"

InputPath=.\include\apu.hw

".\include\apu.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile41.bat 
	@echo off 
	type .\include\apu.hw > .\include\apu.h
<< 
	

!ELSEIF  "$(CFG)" == "libaprutil - Win32 Debug"

InputPath=.\include\apu.hw

".\include\apu.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile42.bat 
	@echo off 
	type .\include\apu.hw > .\include\apu.h
<< 
	

!ELSEIF  "$(CFG)" == "libaprutil - x64 Release"

InputPath=.\include\apu.hw

".\include\apu.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile43.bat 
	@echo off 
	type .\include\apu.hw > .\include\apu.h
<< 
	

!ELSEIF  "$(CFG)" == "libaprutil - x64 Debug"

InputPath=.\include\apu.hw

".\include\apu.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile44.bat 
	@echo off 
	type .\include\apu.hw > .\include\apu.h
<< 
	

!ENDIF 

SOURCE=.\include\private\apu_config.hw

!IF  "$(CFG)" == "libaprutil - Win32 Release"

InputPath=.\include\private\apu_config.hw

".\include\private\apu_config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile45.bat 
	@echo off 
	type .\include\private\apu_config.hw > .\include\private\apu_config.h
<< 
	

!ELSEIF  "$(CFG)" == "libaprutil - Win32 Debug"

InputPath=.\include\private\apu_config.hw

".\include\private\apu_config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile46.bat 
	@echo off 
	type .\include\private\apu_config.hw > .\include\private\apu_config.h
<< 
	

!ELSEIF  "$(CFG)" == "libaprutil - x64 Release"

InputPath=.\include\private\apu_config.hw

".\include\private\apu_config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile47.bat 
	@echo off 
	type .\include\private\apu_config.hw > .\include\private\apu_config.h
<< 
	

!ELSEIF  "$(CFG)" == "libaprutil - x64 Debug"

InputPath=.\include\private\apu_config.hw

".\include\private\apu_config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile48.bat 
	@echo off 
	type .\include\private\apu_config.hw > .\include\private\apu_config.h
<< 
	

!ENDIF 

SOURCE=.\include\private\apu_select_dbm.hw

!IF  "$(CFG)" == "libaprutil - Win32 Release"

InputPath=.\include\private\apu_select_dbm.hw

".\include\private\apu_select_dbm.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile49.bat 
	@echo off 
	type .\include\private\apu_select_dbm.hw > .\include\private\apu_select_dbm.h
<< 
	

!ELSEIF  "$(CFG)" == "libaprutil - Win32 Debug"

InputPath=.\include\private\apu_select_dbm.hw

".\include\private\apu_select_dbm.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile50.bat 
	@echo off 
	type .\include\private\apu_select_dbm.hw > .\include\private\apu_select_dbm.h
<< 
	

!ELSEIF  "$(CFG)" == "libaprutil - x64 Release"

InputPath=.\include\private\apu_select_dbm.hw

".\include\private\apu_select_dbm.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile51.bat 
	@echo off 
	type .\include\private\apu_select_dbm.hw > .\include\private\apu_select_dbm.h
<< 
	

!ELSEIF  "$(CFG)" == "libaprutil - x64 Debug"

InputPath=.\include\private\apu_select_dbm.hw

".\include\private\apu_select_dbm.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile52.bat 
	@echo off 
	type .\include\private\apu_select_dbm.hw > .\include\private\apu_select_dbm.h
<< 
	

!ENDIF 

SOURCE=.\include\apu_want.hw

!IF  "$(CFG)" == "libaprutil - Win32 Release"

InputPath=.\include\apu_want.hw

".\include\apu_want.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile53.bat 
	@echo off 
	type .\include\apu_want.hw > .\include\apu_want.h
<< 
	

!ELSEIF  "$(CFG)" == "libaprutil - Win32 Debug"

InputPath=.\include\apu_want.hw

".\include\apu_want.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile54.bat 
	@echo off 
	type .\include\apu_want.hw > .\include\apu_want.h
<< 
	

!ELSEIF  "$(CFG)" == "libaprutil - x64 Release"

InputPath=.\include\apu_want.hw

".\include\apu_want.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile55.bat 
	@echo off 
	type .\include\apu_want.hw > .\include\apu_want.h
<< 
	

!ELSEIF  "$(CFG)" == "libaprutil - x64 Debug"

InputPath=.\include\apu_want.hw

".\include\apu_want.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile56.bat 
	@echo off 
	type .\include\apu_want.hw > .\include\apu_want.h
<< 
	

!ENDIF 

!IF  "$(CFG)" == "libaprutil - Win32 Release"

"libapr - Win32 Release" : 
   cd ".\..\apr"
   $(MAKE) /$(MAKEFLAGS) /F ".\libapr.mak" CFG="libapr - Win32 Release" 
   cd "..\apr-util"

"libapr - Win32 ReleaseCLEAN" : 
   cd ".\..\apr"
   $(MAKE) /$(MAKEFLAGS) /F ".\libapr.mak" CFG="libapr - Win32 Release" RECURSE=1 CLEAN 
   cd "..\apr-util"

!ELSEIF  "$(CFG)" == "libaprutil - Win32 Debug"

"libapr - Win32 Debug" : 
   cd ".\..\apr"
   $(MAKE) /$(MAKEFLAGS) /F ".\libapr.mak" CFG="libapr - Win32 Debug" 
   cd "..\apr-util"

"libapr - Win32 DebugCLEAN" : 
   cd ".\..\apr"
   $(MAKE) /$(MAKEFLAGS) /F ".\libapr.mak" CFG="libapr - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\apr-util"

!ELSEIF  "$(CFG)" == "libaprutil - x64 Release"

"libapr - x64 Release" : 
   cd ".\..\apr"
   $(MAKE) /$(MAKEFLAGS) /F ".\libapr.mak" CFG="libapr - x64 Release" 
   cd "..\apr-util"

"libapr - x64 ReleaseCLEAN" : 
   cd ".\..\apr"
   $(MAKE) /$(MAKEFLAGS) /F ".\libapr.mak" CFG="libapr - x64 Release" RECURSE=1 CLEAN 
   cd "..\apr-util"

!ELSEIF  "$(CFG)" == "libaprutil - x64 Debug"

"libapr - x64 Debug" : 
   cd ".\..\apr"
   $(MAKE) /$(MAKEFLAGS) /F ".\libapr.mak" CFG="libapr - x64 Debug" 
   cd "..\apr-util"

"libapr - x64 DebugCLEAN" : 
   cd ".\..\apr"
   $(MAKE) /$(MAKEFLAGS) /F ".\libapr.mak" CFG="libapr - x64 Debug" RECURSE=1 CLEAN 
   cd "..\apr-util"

!ENDIF 

!IF  "$(CFG)" == "libaprutil - Win32 Release"

"libapriconv - Win32 Release" : 
   cd ".\..\apr-iconv"
   $(MAKE) /$(MAKEFLAGS) /F ".\libapriconv.mak" CFG="libapriconv - Win32 Release" 
   cd "..\apr-util"

"libapriconv - Win32 ReleaseCLEAN" : 
   cd ".\..\apr-iconv"
   $(MAKE) /$(MAKEFLAGS) /F ".\libapriconv.mak" CFG="libapriconv - Win32 Release" RECURSE=1 CLEAN 
   cd "..\apr-util"

!ELSEIF  "$(CFG)" == "libaprutil - Win32 Debug"

"libapriconv - Win32 Debug" : 
   cd ".\..\apr-iconv"
   $(MAKE) /$(MAKEFLAGS) /F ".\libapriconv.mak" CFG="libapriconv - Win32 Debug" 
   cd "..\apr-util"

"libapriconv - Win32 DebugCLEAN" : 
   cd ".\..\apr-iconv"
   $(MAKE) /$(MAKEFLAGS) /F ".\libapriconv.mak" CFG="libapriconv - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\apr-util"

!ELSEIF  "$(CFG)" == "libaprutil - x64 Release"

"libapriconv - x64 Release" : 
   cd ".\..\apr-iconv"
   $(MAKE) /$(MAKEFLAGS) /F ".\libapriconv.mak" CFG="libapriconv - x64 Release" 
   cd "..\apr-util"

"libapriconv - x64 ReleaseCLEAN" : 
   cd ".\..\apr-iconv"
   $(MAKE) /$(MAKEFLAGS) /F ".\libapriconv.mak" CFG="libapriconv - x64 Release" RECURSE=1 CLEAN 
   cd "..\apr-util"

!ELSEIF  "$(CFG)" == "libaprutil - x64 Debug"

"libapriconv - x64 Debug" : 
   cd ".\..\apr-iconv"
   $(MAKE) /$(MAKEFLAGS) /F ".\libapriconv.mak" CFG="libapriconv - x64 Debug" 
   cd "..\apr-util"

"libapriconv - x64 DebugCLEAN" : 
   cd ".\..\apr-iconv"
   $(MAKE) /$(MAKEFLAGS) /F ".\libapriconv.mak" CFG="libapriconv - x64 Debug" RECURSE=1 CLEAN 
   cd "..\apr-util"

!ENDIF 

!IF  "$(CFG)" == "libaprutil - Win32 Release"

"xml - Win32 Release" : 
   cd ".\xml\expat\lib"
   $(MAKE) /$(MAKEFLAGS) /F ".\xml.mak" CFG="xml - Win32 Release" 
   cd "..\..\.."

"xml - Win32 ReleaseCLEAN" : 
   cd ".\xml\expat\lib"
   $(MAKE) /$(MAKEFLAGS) /F ".\xml.mak" CFG="xml - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\.."

!ELSEIF  "$(CFG)" == "libaprutil - Win32 Debug"

"xml - Win32 Debug" : 
   cd ".\xml\expat\lib"
   $(MAKE) /$(MAKEFLAGS) /F ".\xml.mak" CFG="xml - Win32 Debug" 
   cd "..\..\.."

"xml - Win32 DebugCLEAN" : 
   cd ".\xml\expat\lib"
   $(MAKE) /$(MAKEFLAGS) /F ".\xml.mak" CFG="xml - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\.."

!ELSEIF  "$(CFG)" == "libaprutil - x64 Release"

"xml - x64 Release" : 
   cd ".\xml\expat\lib"
   $(MAKE) /$(MAKEFLAGS) /F ".\xml.mak" CFG="xml - x64 Release" 
   cd "..\..\.."

"xml - x64 ReleaseCLEAN" : 
   cd ".\xml\expat\lib"
   $(MAKE) /$(MAKEFLAGS) /F ".\xml.mak" CFG="xml - x64 Release" RECURSE=1 CLEAN 
   cd "..\..\.."

!ELSEIF  "$(CFG)" == "libaprutil - x64 Debug"

"xml - x64 Debug" : 
   cd ".\xml\expat\lib"
   $(MAKE) /$(MAKEFLAGS) /F ".\xml.mak" CFG="xml - x64 Debug" 
   cd "..\..\.."

"xml - x64 DebugCLEAN" : 
   cd ".\xml\expat\lib"
   $(MAKE) /$(MAKEFLAGS) /F ".\xml.mak" CFG="xml - x64 Debug" RECURSE=1 CLEAN 
   cd "..\..\.."

!ENDIF 

SOURCE=.\libaprutil.rc

"$(INTDIR)\libaprutil.res" : $(SOURCE) "$(INTDIR)" ".\include\apu.h"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

