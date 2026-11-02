@echo off
rem Replays builds\tools\Demo\M02.txt (your recording) at normal speed with the 64-bit and then the
rem 32-bit build. Hands off while it runs. Recordings made before build c9eecfa do not store
rem the key bindings they were made with, so they drift after a binding change: record a new one with DemoTest.bat.
rem Logs: builds\tools\Replay-<build>-<arch>.log. Copies only KHGWin files into Installed\System.
set BUILD=%~1
if "%BUILD%"=="" set BUILD=dc433dc
set TOOLS=%~dp0
set SYS=%~dp0..\..\Installed\System
if not exist "%TOOLS%Demo\M02.txt" (
  echo No recording found. Run DemoTest.bat first.
  pause
  goto :eof
)
for %%A in (win64 win32) do call :run %%A
echo Done. Logs are in builds\tools. The 32-bit build is now the one in Installed\System.
pause
goto :eof

:run
if not exist "%TOOLS%..\%BUILD%-%1\KHGWin.exe" (
  echo Build folder %BUILD%-%1 not found in builds.
  goto :eof
)
echo Replaying with %BUILD%-%1 ...
taskkill /f /im KHGWin.exe >nul 2>&1
xcopy /y /e /q "%TOOLS%..\%BUILD%-%1\*" "%SYS%\" >nul || echo Could not copy %BUILD%-%1 into Installed\System.
pushd "%SYS%"
start "" /wait KHGWin.exe M02.unr -khgreplay=..\..\builds\tools\Demo\M02.txt
popd
copy /y "%SYS%\KHGWin.log" "%TOOLS%Replay-%BUILD%-%1.log" >nul
goto :eof
