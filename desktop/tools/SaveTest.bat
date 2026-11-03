@echo off
rem Save/load round trip for every map, on both builds:
rem   1. 64-bit: play each map 300 ticks, save, load the save back
rem   2. 32-bit: load the saves the 64-bit build wrote
rem   3. 32-bit: play, save, load back
rem   4. 64-bit: load the saves the 32-bit build wrote
rem Test saves go in builds\tools\TestSaves, not in the game's Save folder.
rem Logs: builds\tools\SaveTest-<build>-<step>.log. Copies only KHGWin files into Installed\System.
set BUILD=%~1
if "%BUILD%"=="" set BUILD=4a90344
set TOOLS=%~dp0
set SYS=%~dp0..\..\Installed\System
set SAVES=%~dp0TestSaves
if not exist "%SAVES%" mkdir "%SAVES%"
call :run win64 -khgsavetest 1-win64-save
call :run win32 -khgloadtest 2-win32-loads-win64
call :run win32 -khgsavetest 3-win32-save
call :run win64 -khgloadtest 4-win64-loads-win32
echo Done. Logs are in builds\tools. The 64-bit build is now the one in Installed\System.
pause
goto :eof

:run
if not exist "%TOOLS%..\%BUILD%-%1\KHGWin.exe" (
  echo Build folder %BUILD%-%1 not found in builds.
  goto :eof
)
echo Step %3 ...
taskkill /f /im KHGWin.exe >nul 2>&1
xcopy /y /e /q "%TOOLS%..\%BUILD%-%1\*" "%SYS%\" >nul || echo Could not copy %BUILD%-%1 into Installed\System.
pushd "%SYS%"
start "" /wait KHGWin.exe %2 -khgsavedir=..\..\builds\tools\TestSaves LOG=KHGWin.log
popd
copy /y "%SYS%\KHGWin.log" "%TOOLS%SaveTest-%BUILD%-%3.log" >nul
goto :eof
