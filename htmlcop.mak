# Microsoft Developer Studio Generated NMAKE File, Based on htmlcop.dsp
!IF "$(CFG)" == ""
CFG=htmlcop - Win32 Debug
!MESSAGE No configuration specified. Defaulting to htmlcop - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "htmlcop - Win32 Release" && "$(CFG)" != "htmlcop - Win32 Debug" && "$(CFG)" != "htmlcop - Win32 Release without ALIGN"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "htmlcop.mak" CFG="htmlcop - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "htmlcop - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "htmlcop - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "htmlcop - Win32 Release without ALIGN" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "htmlcop - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\ColorCop.exe" "$(OUTDIR)\ColorCop.bsc"


CLEAN :
	-@erase "$(INTDIR)\ColorCop.pch"
	-@erase "$(INTDIR)\ColorCop.res"
	-@erase "$(INTDIR)\htmlcop.obj"
	-@erase "$(INTDIR)\htmlcop.sbr"
	-@erase "$(INTDIR)\htmlcopDlg.obj"
	-@erase "$(INTDIR)\htmlcopDlg.sbr"
	-@erase "$(INTDIR)\Label.obj"
	-@erase "$(INTDIR)\Label.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\SystemTray.obj"
	-@erase "$(INTDIR)\SystemTray.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WavTipCtrl.obj"
	-@erase "$(INTDIR)\WavTipCtrl.sbr"
	-@erase "$(OUTDIR)\ColorCop.bsc"
	-@erase "$(OUTDIR)\ColorCop.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W2 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ColorCop.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ColorCop.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ColorCop.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\htmlcop.sbr" \
	"$(INTDIR)\htmlcopDlg.sbr" \
	"$(INTDIR)\Label.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\SystemTray.sbr" \
	"$(INTDIR)\WavTipCtrl.sbr"

"$(OUTDIR)\ColorCop.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=MSVCRT.LIB kernel32.lib user32.lib LIBCTINY.LIB /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\ColorCop.pdb" /machine:I386 /out:"$(OUTDIR)\ColorCop.exe" /OPT:REF /ALIGN:4096 
LINK32_OBJS= \
	"$(INTDIR)\htmlcop.obj" \
	"$(INTDIR)\htmlcopDlg.obj" \
	"$(INTDIR)\Label.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SystemTray.obj" \
	"$(INTDIR)\WavTipCtrl.obj" \
	"$(INTDIR)\ColorCop.res"

"$(OUTDIR)\ColorCop.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
   taskkill /F /IM colorcop.exe /T
	 $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"

!ELSEIF  "$(CFG)" == "htmlcop - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\htmlcop.exe"


CLEAN :
	-@erase "$(INTDIR)\ColorCop.pch"
	-@erase "$(INTDIR)\ColorCop.res"
	-@erase "$(INTDIR)\htmlcop.obj"
	-@erase "$(INTDIR)\htmlcopDlg.obj"
	-@erase "$(INTDIR)\Label.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\SystemTray.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WavTipCtrl.obj"
	-@erase "$(OUTDIR)\htmlcop.exe"
	-@erase "$(OUTDIR)\htmlcop.ilk"
	-@erase "$(OUTDIR)\htmlcop.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /GX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fp"$(INTDIR)\ColorCop.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ColorCop.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ColorCop.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\htmlcop.pdb" /debug /machine:I386 /out:"$(OUTDIR)\htmlcop.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\htmlcop.obj" \
	"$(INTDIR)\htmlcopDlg.obj" \
	"$(INTDIR)\Label.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SystemTray.obj" \
	"$(INTDIR)\WavTipCtrl.obj" \
	"$(INTDIR)\ColorCop.res"

"$(OUTDIR)\htmlcop.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "htmlcop - Win32 Release without ALIGN"

OUTDIR=.\htmlcop___Win32_Release_without_ALIGN
INTDIR=.\htmlcop___Win32_Release_without_ALIGN

ALL : ".\Release\ColorCop.exe"


CLEAN :
	-@erase "$(INTDIR)\htmlcop.obj"
	-@erase "$(INTDIR)\htmlcopDlg.obj"
	-@erase "$(INTDIR)\Label.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\SystemTray.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WavTipCtrl.obj"
	-@erase ".\Release\ColorCop.exe"
	-@erase ".\Release\ColorCop.pch"
	-@erase ".\Release\ColorCop.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W2 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fp"Release/ColorCop.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x409 /fo"Release/ColorCop.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"Release/ColorCop.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=MSVCRT.LIB kernel32.lib user32.lib LIBCTINY.LIB /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\ColorCop.pdb" /machine:I386 /out:"Release/ColorCop.exe" /OPT:REF 
LINK32_OBJS= \
	"$(INTDIR)\htmlcop.obj" \
	"$(INTDIR)\htmlcopDlg.obj" \
	"$(INTDIR)\Label.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SystemTray.obj" \
	"$(INTDIR)\WavTipCtrl.obj" \
	".\Release\ColorCop.res"

".\Release\ColorCop.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("htmlcop.dep")
!INCLUDE "htmlcop.dep"
!ELSE 
!MESSAGE Warning: cannot find "htmlcop.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "htmlcop - Win32 Release" || "$(CFG)" == "htmlcop - Win32 Debug" || "$(CFG)" == "htmlcop - Win32 Release without ALIGN"
SOURCE=.\htmlcop.cpp

!IF  "$(CFG)" == "htmlcop - Win32 Release"

CPP_SWITCHES=/nologo /MD /W2 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ColorCop.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\htmlcop.obj"	"$(INTDIR)\htmlcop.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ColorCop.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "htmlcop - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fp"$(INTDIR)\ColorCop.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\htmlcop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ColorCop.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "htmlcop - Win32 Release without ALIGN"

CPP_SWITCHES=/nologo /MD /W2 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fp"Release/ColorCop.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\htmlcop.obj" : $(SOURCE) "$(INTDIR)" ".\Release\ColorCop.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\htmlcop.rc

!IF  "$(CFG)" == "htmlcop - Win32 Release"


"$(INTDIR)\ColorCop.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "htmlcop - Win32 Debug"


"$(INTDIR)\ColorCop.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "htmlcop - Win32 Release without ALIGN"


".\Release\ColorCop.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\htmlcopDlg.cpp

!IF  "$(CFG)" == "htmlcop - Win32 Release"


"$(INTDIR)\htmlcopDlg.obj"	"$(INTDIR)\htmlcopDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ColorCop.pch"


!ELSEIF  "$(CFG)" == "htmlcop - Win32 Debug"


"$(INTDIR)\htmlcopDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ColorCop.pch"


!ELSEIF  "$(CFG)" == "htmlcop - Win32 Release without ALIGN"


"$(INTDIR)\htmlcopDlg.obj" : $(SOURCE) "$(INTDIR)" ".\Release\ColorCop.pch"


!ENDIF 

SOURCE=.\Label.cpp

!IF  "$(CFG)" == "htmlcop - Win32 Release"


"$(INTDIR)\Label.obj"	"$(INTDIR)\Label.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ColorCop.pch"


!ELSEIF  "$(CFG)" == "htmlcop - Win32 Debug"


"$(INTDIR)\Label.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ColorCop.pch"


!ELSEIF  "$(CFG)" == "htmlcop - Win32 Release without ALIGN"


"$(INTDIR)\Label.obj" : $(SOURCE) "$(INTDIR)" ".\Release\ColorCop.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "htmlcop - Win32 Release"

CPP_SWITCHES=/nologo /MD /W2 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ColorCop.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\ColorCop.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "htmlcop - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fp"$(INTDIR)\ColorCop.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ColorCop.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "htmlcop - Win32 Release without ALIGN"

CPP_SWITCHES=/nologo /MD /W2 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fp"Release/ColorCop.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	".\Release\ColorCop.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\SystemTray.cpp

!IF  "$(CFG)" == "htmlcop - Win32 Release"


"$(INTDIR)\SystemTray.obj"	"$(INTDIR)\SystemTray.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ColorCop.pch"


!ELSEIF  "$(CFG)" == "htmlcop - Win32 Debug"


"$(INTDIR)\SystemTray.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ColorCop.pch"


!ELSEIF  "$(CFG)" == "htmlcop - Win32 Release without ALIGN"


"$(INTDIR)\SystemTray.obj" : $(SOURCE) "$(INTDIR)" ".\Release\ColorCop.pch"


!ENDIF 

SOURCE=.\WavTipCtrl.cpp

!IF  "$(CFG)" == "htmlcop - Win32 Release"


"$(INTDIR)\WavTipCtrl.obj"	"$(INTDIR)\WavTipCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ColorCop.pch"


!ELSEIF  "$(CFG)" == "htmlcop - Win32 Debug"


"$(INTDIR)\WavTipCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ColorCop.pch"


!ELSEIF  "$(CFG)" == "htmlcop - Win32 Release without ALIGN"


"$(INTDIR)\WavTipCtrl.obj" : $(SOURCE) "$(INTDIR)" ".\Release\ColorCop.pch"


!ENDIF 


!ENDIF 

