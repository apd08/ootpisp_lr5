@echo off
setlocal

set "CMAKE=C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
set "VCVARS=C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"

if not exist "%CMAKE%" (
    echo CMake from Visual Studio Build Tools was not found.
    echo Install "Desktop development with C++" or add cmake to PATH.
    exit /b 1
)

call "%VCVARS%"
if errorlevel 1 exit /b 1

"%CMAKE%" -S "%~dp0." -B "%~dp0build" -G "NMake Makefiles"
if errorlevel 1 (
    echo.
    echo If build failed due to non-ASCII path, run:
    echo   cmake -S "%~dp0" -B C:\temp\lr3_build -G "NMake Makefiles"
    echo   cmake --build C:\temp\lr3_build
    exit /b 1
)

"%CMAKE%" --build "%~dp0build"
exit /b %errorlevel%
