@echo off
setlocal enabledelayedexpansion

REM ================================
REM  LOCATE MSBUILD.EXE
REM ================================
set MSBUILD_EXE=

for /f "usebackq tokens=*" %%i in (`
    "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" ^
        -latest -requires Microsoft.Component.MSBuild ^
        -find MSBuild\**\Bin\MSBuild.exe
`) do (
    set MSBUILD_EXE=%%i
)

if "%MSBUILD_EXE%"=="" (
    echo ERROR: C++-capable MSBuild.exe not found.
    exit /b 1
)

echo Using MSBuild: %MSBUILD_EXE%

REM ================================
REM  BUILD TESTS
REM ================================
"%MSBUILD_EXE%" ColorCopTests\ColorCopTests.vcxproj ^
    /t:Rebuild ^
    /p:Configuration=Release ^
    /p:Platform=Win32

if errorlevel 1 (
    echo.
    echo ERROR: Test build failed.
    exit /b %errorlevel%
)

REM ================================
REM  LOCATE TEST EXECUTABLE
REM ================================
set TEST_EXE=

for /r %%i in (ColorCopTests.exe) do (
    set TEST_EXE=%%i
)

if "%TEST_EXE%"=="" (
    echo.
    echo ERROR: ColorCopTests.exe not found.
    exit /b 1
)

echo.
echo Running: %TEST_EXE%
echo.

"%TEST_EXE%"

exit /b %errorlevel%
