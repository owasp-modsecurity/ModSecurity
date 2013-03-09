# Microsoft Developer Studio Generated NMAKE File, Based on pcre.dsp
!IF "$(CFG)" == ""
CFG=pcre - Win32 Debug
!MESSAGE No configuration specified. Defaulting to pcre - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "pcre - Win32 Release" && "$(CFG)" != "pcre - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "pcre.mak" CFG="pcre - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "pcre - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "pcre - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "pcre - Win32 Release"

OUTDIR=.\LibR
INTDIR=.\LibR
# Begin Custom Macros
OutDir=.\LibR
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\pcre.lib"

!ELSE 

ALL : "dftables - Win32 Release" "$(OUTDIR)\pcre.lib"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"dftables - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\get.obj"
	-@erase "$(INTDIR)\maketables.obj"
	-@erase "$(INTDIR)\pcre.obj"
	-@erase "$(INTDIR)\pcre_src.idb"
	-@erase "$(INTDIR)\pcre_src.pdb"
	-@erase "$(INTDIR)\study.obj"
	-@erase "$(OUTDIR)\pcre.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /Zi /O2 /D "_WIN32" /D "NDEBUG" /D "_WINDOWS" /D "PCRE_STATIC" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\pcre_src" /FD /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\pcre.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\pcre.lib" 
LIB32_OBJS= \
	"$(INTDIR)\get.obj" \
	"$(INTDIR)\maketables.obj" \
	"$(INTDIR)\pcre.obj" \
	"$(INTDIR)\study.obj"

"$(OUTDIR)\pcre.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "pcre - Win32 Debug"

OUTDIR=.\LibD
INTDIR=.\LibD
# Begin Custom Macros
OutDir=.\LibD
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : ".\pcre.h" ".\config.h" ".\chartables.c" "$(OUTDIR)\pcre.lib"

!ELSE 

ALL : "dftables - Win32 Debug" ".\pcre.h" ".\config.h" ".\chartables.c" "$(OUTDIR)\pcre.lib"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"dftables - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\get.obj"
	-@erase "$(INTDIR)\maketables.obj"
	-@erase "$(INTDIR)\pcre.obj"
	-@erase "$(INTDIR)\pcre_src.idb"
	-@erase "$(INTDIR)\pcre_src.pdb"
	-@erase "$(INTDIR)\study.obj"
	-@erase "$(OUTDIR)\pcre.lib"
	-@erase ".\chartables.c"
	-@erase ".\config.h"
	-@erase ".\pcre.h"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Zi /Od /D "_WIN32" /D "_DEBUG" /D "_WINDOWS" /D "PCRE_STATIC" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\pcre_src" /FD /EHsc /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\pcre.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\pcre.lib" 
LIB32_OBJS= \
	"$(INTDIR)\get.obj" \
	"$(INTDIR)\maketables.obj" \
	"$(INTDIR)\pcre.obj" \
	"$(INTDIR)\study.obj"

"$(OUTDIR)\pcre.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("pcre.dep")
!INCLUDE "pcre.dep"
!ELSE 
!MESSAGE Warning: cannot find "pcre.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "pcre - Win32 Release" || "$(CFG)" == "pcre - Win32 Debug"
SOURCE=.\dftables.exe

!IF  "$(CFG)" == "pcre - Win32 Release"

InputPath=.\dftables.exe

".\chartables.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile79.bat 
	@echo off 
	.\dftables.exe chartables.c 
<< 
	

!ELSEIF  "$(CFG)" == "pcre - Win32 Debug"

InputPath=.\dftables.exe

".\chartables.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile80.bat 
	@echo off 
	.\dftables.exe chartables.c 
<< 
	

!ENDIF 

SOURCE=.\get.c

"$(INTDIR)\get.obj" : $(SOURCE) "$(INTDIR)" ".\config.h" ".\pcre.h"


SOURCE=.\maketables.c

"$(INTDIR)\maketables.obj" : $(SOURCE) "$(INTDIR)" ".\config.h" ".\pcre.h"


SOURCE=.\pcre.c

"$(INTDIR)\pcre.obj" : $(SOURCE) "$(INTDIR)" ".\chartables.c" ".\config.h" ".\pcre.h"


SOURCE=.\study.c

"$(INTDIR)\study.obj" : $(SOURCE) "$(INTDIR)" ".\config.h" ".\pcre.h"


SOURCE=.\config.hw

!IF  "$(CFG)" == "pcre - Win32 Release"

InputPath=.\config.hw

".\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile81.bat 
	@echo off 
	type .\config.hw > .\config.h
<< 
	

!ELSEIF  "$(CFG)" == "pcre - Win32 Debug"

InputPath=.\config.hw

".\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile82.bat 
	@echo off 
	type .\config.hw > .\config.h
<< 
	

!ENDIF 

SOURCE=.\pcre.hw

!IF  "$(CFG)" == "pcre - Win32 Release"

InputPath=.\pcre.hw

".\pcre.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile83.bat 
	@echo off 
	type .\pcre.hw > .\pcre.h
<< 
	

!ELSEIF  "$(CFG)" == "pcre - Win32 Debug"

InputPath=.\pcre.hw

".\pcre.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile84.bat 
	@echo off 
	type .\pcre.hw > .\pcre.h
<< 
	

!ENDIF 

!IF  "$(CFG)" == "pcre - Win32 Release"

"dftables - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\dftables.mak" CFG="dftables - Win32 Release" 
   cd "."

"dftables - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\dftables.mak" CFG="dftables - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "pcre - Win32 Debug"

"dftables - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\dftables.mak" CFG="dftables - Win32 Debug" 
   cd "."

"dftables - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\dftables.mak" CFG="dftables - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 


!ENDIF 

