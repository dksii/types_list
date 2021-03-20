@echo OFF

:: choice target
echo 1) win32
echo 2) win64
@echo:
choice /n /C 12

:: set build target and directory
if %ERRORLEVEL% equ 1 set build_target=Win32& set build_dir=build_win32
if %ERRORLEVEL% equ 2 set build_target=x64& set build_dir=build_win64

set options=-DBUILD_EXAMPLES=ON -DBUILD_TESTING=ON
set cmd=cmake -S . -B %build_dir% -A %build_target% -G "Visual Studio 16 2019" %options%

:: print command and execute
echo %cmd%
%cmd%

pause