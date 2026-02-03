@echo off
setlocal enabledelayedexpansion

REM ================================
REM  GET VERSION FROM GIT TAG
REM ================================
for /f %%v in ('git describe --tags --abbrev^=0') do set VERSION=%%v

echo Building version %VERSION%

REM Convert x.y.z or x.y.z.w → x,y,z,w
for /f "tokens=1-4 delims=." %%a in ("%VERSION%") do (
    set MAJOR=%%a
    set MINOR=%%b
    set PATCH=%%c
    set BUILD=%%d
)

if "%BUILD%"=="" set BUILD=0
set VERSION_NUMERIC=%MAJOR%,%MINOR%,%PATCH%,%BUILD%

echo Numeric version: %VERSION_NUMERIC%

REM ================================
REM  LOCATE MSBUILD
REM ================================
for /f "tokens=2,*" %%a in ('reg query "HKLM\SOFTWARE\Microsoft\MSBuild\ToolsVersions\4.0" /v MSBuildToolsPath 2^>nul') do (
    set MSBUILDPATH=%%b
)

if not exist "%MSBUILDPATH%msbuild.exe" (
    for /f "usebackq tokens=*" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe"`) do (
        set MSBUILDPATH=%%i
    )
)

if not exist "%MSBUILDPATH%" (
    echo ERROR: MSBuild not found.
    exit /b 1
)

echo Using MSBuild: %MSBUILDPATH%

REM ================================
REM  BUILD
REM ================================
"%MSBUILDPATH%" ColorCop.sln ^
    /t:Rebuild ^
    /p:Configuration=Release ^
    /p:Platform=Win32 ^
    /p:ProductVersion=%VERSION% ^
    /p:FileVersion=%VERSION% ^
    /p:NumericVersion=%VERSION_NUMERIC% ^
    /m

exit /b %errorlevel%

