# Microsoft Developer Studio Generated NMAKE File, Based on ColorCop.dsp
!IF "$(CFG)" == ""
CFG=ColorCop - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ColorCop - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ColorCop - Win32 Release" && "$(CFG)" != "ColorCop - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ColorCop.mak" CFG="ColorCop - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ColorCop - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ColorCop - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ColorCop - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\ColorCop.exe" "$(OUTDIR)\ColorCop.bsc"


CLEAN :
	-@erase "$(INTDIR)\ColorCop.obj"
	-@erase "$(INTDIR)\ColorCop.pch"
	-@erase "$(INTDIR)\ColorCop.res"
	-@erase "$(INTDIR)\ColorCop.sbr"
	-@erase "$(INTDIR)\ColorCopDlg.obj"
	-@erase "$(INTDIR)\ColorCopDlg.sbr"
	-@erase "$(INTDIR)\colorspace.obj"
	-@erase "$(INTDIR)\colorspace.sbr"
	-@erase "$(INTDIR)\Label.obj"
	-@erase "$(INTDIR)\Label.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\SystemTray.obj"
	-@erase "$(INTDIR)\SystemTray.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ColorCop.bsc"
	-@erase "$(OUTDIR)\ColorCop.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W2 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ColorCop.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL" 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ColorCop.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ColorCop.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ColorCop.sbr" \
	"$(INTDIR)\ColorCopDlg.sbr" \
	"$(INTDIR)\colorspace.sbr" \
	"$(INTDIR)\Label.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\SystemTray.sbr"

"$(OUTDIR)\ColorCop.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=MSVCRT.LIB kernel32.lib user32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\ColorCop.pdb" /machine:I386 /out:"$(OUTDIR)\ColorCop.exe" /OPT:REF 
LINK32_OBJS= \
	"$(INTDIR)\ColorCop.obj" \
	"$(INTDIR)\ColorCopDlg.obj" \
	"$(INTDIR)\colorspace.obj" \
	"$(INTDIR)\Label.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SystemTray.obj" \
	"$(INTDIR)\ColorCop.res"

"$(OUTDIR)\ColorCop.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ColorCop - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\ColorCop.exe"


CLEAN :
	-@erase "$(INTDIR)\ColorCop.obj"
	-@erase "$(INTDIR)\ColorCop.pch"
	-@erase "$(INTDIR)\ColorCop.res"
	-@erase "$(INTDIR)\ColorCopDlg.obj"
	-@erase "$(INTDIR)\colorspace.obj"
	-@erase "$(INTDIR)\Label.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\SystemTray.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ColorCop.exe"
	-@erase "$(OUTDIR)\ColorCop.ilk"
	-@erase "$(OUTDIR)\ColorCop.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /GX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fp"$(INTDIR)\ColorCop.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL" 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ColorCop.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ColorCop.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\ColorCop.pdb" /debug /machine:I386 /out:"$(OUTDIR)\ColorCop.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\ColorCop.obj" \
	"$(INTDIR)\ColorCopDlg.obj" \
	"$(INTDIR)\colorspace.obj" \
	"$(INTDIR)\Label.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SystemTray.obj" \
	"$(INTDIR)\ColorCop.res"

"$(OUTDIR)\ColorCop.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

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


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ColorCop.dep")
!INCLUDE "ColorCop.dep"
!ELSE 
!MESSAGE Warning: cannot find "ColorCop.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ColorCop - Win32 Release" || "$(CFG)" == "ColorCop - Win32 Debug"
SOURCE=.\ColorCop.cpp

!IF  "$(CFG)" == "ColorCop - Win32 Release"

CPP_SWITCHES=/nologo /MD /W2 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ColorCop.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ColorCop.obj"	"$(INTDIR)\ColorCop.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ColorCop.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ColorCop - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fp"$(INTDIR)\ColorCop.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ColorCop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ColorCop.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\ColorCop.rc

"$(INTDIR)\ColorCop.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\ColorCopDlg.cpp

!IF  "$(CFG)" == "ColorCop - Win32 Release"


"$(INTDIR)\ColorCopDlg.obj"	"$(INTDIR)\ColorCopDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ColorCop.pch"


!ELSEIF  "$(CFG)" == "ColorCop - Win32 Debug"


"$(INTDIR)\ColorCopDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ColorCop.pch"


!ENDIF 

SOURCE=.\colorspace.cpp

!IF  "$(CFG)" == "ColorCop - Win32 Release"


"$(INTDIR)\colorspace.obj"	"$(INTDIR)\colorspace.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ColorCop.pch"


!ELSEIF  "$(CFG)" == "ColorCop - Win32 Debug"


"$(INTDIR)\colorspace.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ColorCop.pch"


!ENDIF 

SOURCE=.\Label.cpp

!IF  "$(CFG)" == "ColorCop - Win32 Release"


"$(INTDIR)\Label.obj"	"$(INTDIR)\Label.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ColorCop.pch"


!ELSEIF  "$(CFG)" == "ColorCop - Win32 Debug"


"$(INTDIR)\Label.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ColorCop.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ColorCop - Win32 Release"

CPP_SWITCHES=/nologo /MD /W2 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ColorCop.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\ColorCop.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ColorCop - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fp"$(INTDIR)\ColorCop.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ColorCop.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\SystemTray.cpp

!IF  "$(CFG)" == "ColorCop - Win32 Release"


"$(INTDIR)\SystemTray.obj"	"$(INTDIR)\SystemTray.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ColorCop.pch"


!ELSEIF  "$(CFG)" == "ColorCop - Win32 Debug"


"$(INTDIR)\SystemTray.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ColorCop.pch"


!ENDIF 


!ENDIF 

