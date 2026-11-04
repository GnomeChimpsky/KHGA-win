@echo off
rem M6 save compatibility test. Runs everything unattended (about 45 minutes); don't use the PC while
rem it runs, because the retail steps send keys to the game window.
rem   1. Retail Khg.exe: saves once in every single-player map            -> Compat\Retail\Save1xx.usa
rem   2. KHGWin 64-bit and 32-bit: load every retail save, log the pawns  -> SaveCompat-<build>-retail-<arch>.log
rem   3. KHGWin 64-bit and 32-bit: save in every map and load it back     -> Compat\Win64, Compat\Win32
rem   4. KHGWin 32-bit loads the 64-bit saves, 64-bit loads the 32-bit saves
rem   5. Retail Khg.exe loads the KHGWin 64-bit and 32-bit saves, one screenshot each -> Compat\Shots
rem Double-click, or SaveCompat.bat <build>. Run one part with SaveCompat.bat <build> <step number>,
rem or steps 1 and 2 with SaveCompat.bat <build> retail.
rem Test saves stay in builds\tools\Compat; the game's own Save folder and Khg.ini are not touched.
rem Copies only the KHGWin files into Installed\System, like the other test bats.
setlocal
set BUILD=%~1
if "%BUILD%"=="" set BUILD=b03f445
set ONLY=%~2
set TOOLS=%~dp0
set SYS=%~dp0..\..\Installed\System
set COMPAT=%TOOLS%Compat
set REL=..\..\builds\tools\Compat
if not exist "%COMPAT%" mkdir "%COMPAT%"
taskkill /f /im KHGWin.exe >nul 2>&1
taskkill /f /im Khg.exe >nul 2>&1

if "%ONLY%"=="" goto step1
if "%ONLY%"=="1" goto step1
if /i "%ONLY%"=="retail" goto step1
goto after1
:step1
echo Step 1: retail saves (hands off the keyboard and mouse) ...
powershell -NoProfile -ExecutionPolicy Bypass -File "%TOOLS%RetailSaves.ps1" -Sys "%SYS%" -Mode Save -SaveDir %REL%\Retail -Out "%COMPAT%"
:after1

if "%ONLY%"=="" goto step2
if "%ONLY%"=="2" goto step2
if /i "%ONLY%"=="retail" goto step2
goto after2
:step2
echo Step 2: KHGWin loads the retail saves ...
call :run win64 "-khgloadsaves -khgsavedir=%REL%\Retail" retail-win64
call :run win32 "-khgloadsaves -khgsavedir=%REL%\Retail" retail-win32
:after2

if "%ONLY%"=="" goto step3
if "%ONLY%"=="3" goto step3
goto after3
:step3
echo Step 3: KHGWin saves in every map ...
call :run win64 "-khgsavetest -khgsavedir=%REL%\Win64" save-win64
call :run win32 "-khgsavetest -khgsavedir=%REL%\Win32" save-win32
:after3

if "%ONLY%"=="" goto step4
if "%ONLY%"=="4" goto step4
goto after4
:step4
echo Step 4: KHGWin loads the other build's saves ...
call :run win32 "-khgloadtest -khgsavedir=%REL%\Win64" win32-loads-win64
call :run win64 "-khgloadtest -khgsavedir=%REL%\Win32" win64-loads-win32
:after4

if "%ONLY%"=="" goto step5
if "%ONLY%"=="5" goto step5
goto after5
:step5
echo Step 5: retail loads the KHGWin saves (hands off the keyboard and mouse) ...
powershell -NoProfile -ExecutionPolicy Bypass -File "%TOOLS%RetailSaves.ps1" -Sys "%SYS%" -Mode Load -SaveDir %REL%\Win64 -Out "%COMPAT%" -PortLog "%TOOLS%SaveCompat-%BUILD%-save-win64.log"
powershell -NoProfile -ExecutionPolicy Bypass -File "%TOOLS%RetailSaves.ps1" -Sys "%SYS%" -Mode Load -SaveDir %REL%\Win32 -Out "%COMPAT%" -PortLog "%TOOLS%SaveCompat-%BUILD%-save-win32.log"
:after5

echo Done. Logs are builds\tools\SaveCompat-%BUILD%-*.log and builds\tools\Compat\*.log.
pause
exit /b 0

:run
if not exist "%TOOLS%..\%BUILD%-%1\KHGWin.exe" (
  echo Build folder %BUILD%-%1 not found in builds.
  goto :eof
)
echo   %3 ...
taskkill /f /im KHGWin.exe >nul 2>&1
xcopy /y /e /q "%TOOLS%..\%BUILD%-%1\*" "%SYS%\" >nul || echo Could not copy %BUILD%-%1 into Installed\System.
pushd "%SYS%"
start "" /wait KHGWin.exe %~2 LOG=KHGWin.log
popd
copy /y "%SYS%\KHGWin.log" "%TOOLS%SaveCompat-%BUILD%-%3.log" >nul
goto :eof
