@echo on

rem vcvars32.bat

rmdir /S /Q Release
nmake /NOLOGO /f "ColorCop.mak" CFG="ColorCop - Win32 Release"

rmdir /S /Q Debug
nmake /NOLOGO /f "ColorCop.mak" CFG="ColorCop - Win32 Debug"
