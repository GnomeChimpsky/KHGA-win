@echo off
rem M8 timing test: runs the same scripted input (walking, jumping, turning, mouse, every weapon, the map
rem left running) at fixed 60, 144, 240, 500 and 1000 Hz game ticks with one KHGWin build after the other
rem (64-bit, then 32-bit), and writes builds\tools\Timing\Timing-<map>-<arch>.html comparing every value
rem with the 60 Hz run. Red cells are differences beyond the tolerance.
rem Double-click to test the build named below, or run TimingTest.bat <commit> [maps] for another one,
rem e.g. TimingTest.bat 1234567 M02.unr,M11.unr
rem Don't touch the mouse or keyboard while it runs: the test presses keys itself. It takes about
rem an hour per map and build. The ticks aren't paced to real time, so any PC can run the 1000 Hz pass.
rem Copies only the KHGWin files (KHGWin*, OpenAL32.dll, FFmpeg DLLs) into Installed\System; no retail file is touched.
set BUILD=%~1
if "%BUILD%"=="" set BUILD=6fa906a
set MAPS=%~2
if "%MAPS%"=="" set MAPS=M02.unr,M04A.unr,M08.unr,M12.unr
set TOOLS=%~dp0
set SYS=%~dp0..\..\Installed\System
if not exist "%TOOLS%Timing" mkdir "%TOOLS%Timing"
for %%A in (win64 win32) do (
  if exist "%TOOLS%..\%BUILD%-%%A\KHGWin.exe" (
    echo Testing %BUILD%-%%A on %MAPS% ...
    taskkill /f /im KHGWin.exe >nul 2>&1
    xcopy /y /e /q "%TOOLS%..\%BUILD%-%%A\*" "%SYS%\" >nul || echo Could not copy %BUILD%-%%A into Installed\System.
    pushd "%SYS%"
    start "" /wait KHGWin.exe -khgtiming=..\..\builds\tools\Timing -khgmaps=%MAPS% -nolauncher LOG=KHGWin.log
    popd
    copy /y "%SYS%\KHGWin.log" "%TOOLS%Timing\TimingTest-%BUILD%-%%A.log" >nul
  ) else (
    echo Build folder %BUILD%-%%A not found in builds.
  )
)
echo Done. Reports are in builds\tools\Timing. The 32-bit build is now the one in Installed\System.
start "" "%TOOLS%Timing"
pause
