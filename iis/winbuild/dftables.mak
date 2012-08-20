# Microsoft Developer Studio Generated NMAKE File, Based on dftables.dsp
!IF "$(CFG)" == ""
CFG=dftables - Win32 Debug
!MESSAGE No configuration specified. Defaulting to dftables - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "dftables - Win32 Release" && "$(CFG)" != "dftables - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "dftables.mak" CFG="dftables - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "dftables - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "dftables - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "dftables - Win32 Release"

OUTDIR=.
INTDIR=.\Release
# Begin Custom Macros
OutDir=.
# End Custom Macros

ALL : "$(OUTDIR)\dftables.exe"


CLEAN :
	-@erase "$(INTDIR)\dftables.idb"
	-@erase "$(INTDIR)\dftables.obj"
	-@erase "$(OUTDIR)\dftables.exe"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /O2 /D "_WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_CRT_SECURE_NO_DEPRECATE" /D "_CRT_NONSTDC_NO_DEPRECATE" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\dftables" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\dftables.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\Release\dftables.pdb" /out:"$(OUTDIR)\dftables.exe" /opt:ref 
LINK32_OBJS= \
	"$(INTDIR)\dftables.obj"

"$(OUTDIR)\dftables.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dftables - Win32 Debug"

OUTDIR=.
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.
# End Custom Macros

ALL : ".\pcre.h" ".\config.h" "$(OUTDIR)\dftables.exe"


CLEAN :
	-@erase "$(INTDIR)\dftables.idb"
	-@erase "$(INTDIR)\dftables.obj"
	-@erase "$(OUTDIR)\Debug\dftables.pdb"
	-@erase "$(OUTDIR)\dftables.exe"
	-@erase ".\config.h"
	-@erase ".\pcre.h"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Zi /Od /D "_WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_CRT_SECURE_NO_DEPRECATE" /D "_CRT_NONSTDC_NO_DEPRECATE" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\dftables" /FD /EHsc /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\dftables.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\Debug\dftables.pdb" /debug /out:"$(OUTDIR)\dftables.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dftables.obj"

"$(OUTDIR)\dftables.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("dftables.dep")
!INCLUDE "dftables.dep"
!ELSE 
!MESSAGE Warning: cannot find "dftables.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "dftables - Win32 Release" || "$(CFG)" == "dftables - Win32 Debug"
SOURCE=.\dftables.c

"$(INTDIR)\dftables.obj" : $(SOURCE) "$(INTDIR)" ".\config.h" ".\pcre.h"


SOURCE=.\config.hw

!IF  "$(CFG)" == "dftables - Win32 Release"

InputPath=.\config.hw

".\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile27.bat 
	@echo off 
	type .\config.hw > .\config.h
<< 
	

!ELSEIF  "$(CFG)" == "dftables - Win32 Debug"

InputPath=.\config.hw

".\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile28.bat 
	@echo off 
	type .\config.hw > .\config.h
<< 
	

!ENDIF 

SOURCE=.\maketables.c
SOURCE=.\pcre.hw

!IF  "$(CFG)" == "dftables - Win32 Release"

InputPath=.\pcre.hw

".\pcre.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile29.bat 
	@echo off 
	type .\pcre.hw > .\pcre.h
<< 
	

!ELSEIF  "$(CFG)" == "dftables - Win32 Debug"

InputPath=.\pcre.hw

".\pcre.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile30.bat 
	@echo off 
	type .\pcre.hw > .\pcre.h
<< 
	

!ENDIF 


!ENDIF 

