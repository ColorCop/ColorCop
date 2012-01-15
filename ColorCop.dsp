# Microsoft Developer Studio Project File - Name="ColorCop" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ColorCop - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ColorCop.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ColorCop.mak" CFG="ColorCop - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ColorCop - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ColorCop - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "ColorCop - Win32 Release without ALIGN" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/ColorCop", BAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ColorCop - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W2 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /Fp"Release/ColorCop.pch" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /fo"Release/ColorCop.res" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Release/ColorCop.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 MSVCRT.LIB kernel32.lib user32.lib LIBCTINY.LIB /nologo /subsystem:windows /machine:I386 /out:"Release/ColorCop.exe" /OPT:REF /ALIGN:4096
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ColorCop - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fp"Debug/ColorCop.pch" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /fo"Debug/ColorCop.res" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Debug/ColorCop.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "ColorCop - Win32 Release without ALIGN"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ColorCop___Win32_Release_without_ALIGN"
# PROP BASE Intermediate_Dir "ColorCop___Win32_Release_without_ALIGN"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ColorCop___Win32_Release_without_ALIGN"
# PROP Intermediate_Dir "ColorCop___Win32_Release_without_ALIGN"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W2 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fp"Release/ColorCop.pch" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W2 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fp"Release/ColorCop.pch" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /fo"Release/ColorCop.res" /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /fo"Release/ColorCop.res" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"Release/ColorCop.bsc"
# ADD BSC32 /nologo /o"Release/ColorCop.bsc"
LINK32=link.exe
# ADD BASE LINK32 MSVCRT.LIB kernel32.lib user32.lib LIBCTINY.LIB /nologo /subsystem:windows /machine:I386 /out:"Release/ColorCop.exe" /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 MSVCRT.LIB kernel32.lib user32.lib LIBCTINY.LIB /nologo /subsystem:windows /machine:I386 /out:"Release/ColorCop.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ColorCop - Win32 Release"
# Name "ColorCop - Win32 Debug"
# Name "ColorCop - Win32 Release without ALIGN"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\colorspace.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorCop.cpp

!IF  "$(CFG)" == "ColorCop - Win32 Release"

# ADD CPP /O2

!ELSEIF  "$(CFG)" == "ColorCop - Win32 Debug"

!ELSEIF  "$(CFG)" == "ColorCop - Win32 Release without ALIGN"

# ADD BASE CPP /O2
# ADD CPP /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ColorCop.rc
# End Source File
# Begin Source File

SOURCE=.\ColorCopDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Label.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SystemTray.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AggressiveOptimize.h
# End Source File
# Begin Source File

SOURCE=.\colorspace.h
# End Source File
# Begin Source File

SOURCE=.\ColorCop.h
# End Source File
# Begin Source File

SOURCE=.\ColorCopDlg.h
# End Source File
# Begin Source File

SOURCE=.\Label.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SystemTray.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Res\cross.cur
# End Source File
# Begin Source File

SOURCE=.\Res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\Res\cur00002.cur
# End Source File
# Begin Source File

SOURCE=.\Res\cur00003.cur
# End Source File
# Begin Source File

SOURCE=.\Res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\Res\cursor_m.cur
# End Source File
# Begin Source File

SOURCE=.\Res\eyeyedropper.ico
# End Source File
# Begin Source File

SOURCE=.\Res\hand.cur
# End Source File
# Begin Source File

SOURCE=.\res\ColorCop.rc2
# End Source File
# Begin Source File

SOURCE=.\Res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\Res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\Res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\Res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\Res\idr_main.ico
# End Source File
# Begin Source File

SOURCE=.\Res\MOVE4WAY.CUR
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
