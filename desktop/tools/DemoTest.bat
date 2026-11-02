@echo off
rem Input record and replay test.
rem   1. Starts the 64-bit KHGWin in M02 and records everything you press. Play for a minute or two
rem      (move, fight, switch weapons), then quit the game normally.
rem   2. Replays your recording with the 64-bit build, then with the 32-bit build.
rem The recording is builds\tools\Demo\M02.txt; logs are builds\tools\DemoTest-<build>-<step>.log.
rem Movies are skipped while recording and replaying. Copies only KHGWin files into Installed\System.
set BUILD=%~1
if "%BUILD%"=="" set BUILD=80d650e
set TOOLS=%~dp0
set SYS=%~dp0..\..\Installed\System
if not exist "%TOOLS%Demo" mkdir "%TOOLS%Demo"
echo Step 1: play M02 for a minute or two, then quit the game.
call :run win64 "-khgrecord=..\..\builds\tools\Demo\M02.txt" 1-win64-record
if not exist "%TOOLS%Demo\M02.txt" (
  echo No recording was made, stopping.
  pause
  goto :eof
)
echo Step 2: replaying with the 64-bit build. Hands off, this runs by itself.
call :run win64 "-khgreplay=..\..\builds\tools\Demo\M02.txt" 2-win64-replay
echo Step 3: replaying with the 32-bit build. Hands off, this runs by itself.
call :run win32 "-khgreplay=..\..\builds\tools\Demo\M02.txt" 3-win32-replay
echo Done. Logs are in builds\tools. The 32-bit build is now the one in Installed\System.
pause
goto :eof

:run
if not exist "%TOOLS%..\%BUILD%-%1\KHGWin.exe" (
  echo Build folder %BUILD%-%1 not found in builds.
  goto :eof
)
taskkill /f /im KHGWin.exe >nul 2>&1
xcopy /y /e /q "%TOOLS%..\%BUILD%-%1\*" "%SYS%\" >nul || echo Could not copy %BUILD%-%1 into Installed\System.
pushd "%SYS%"
start "" /wait KHGWin.exe M02.unr %~2 LOG=KHGWin.log
popd
copy /y "%SYS%\KHGWin.log" "%TOOLS%DemoTest-%BUILD%-%3.log" >nul
goto :eof
