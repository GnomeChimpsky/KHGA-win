@echo off
rem Loads every map with one KHGWin build after the other (64-bit, then 32-bit), 300 game ticks each,
rem and saves each log as builds\tools\MapTest-<build>.log.
rem Double-click to test the build named below, or run MapTest.bat <commit> for another one.
rem Copies only the KHGWin files (KHGWin*, OpenAL32.dll, FFmpeg DLLs) into Installed\System; no retail file is touched.
set BUILD=%~1
if "%BUILD%"=="" set BUILD=c9eecfa
set TOOLS=%~dp0
set SYS=%~dp0..\..\Installed\System
for %%A in (win64 win32) do (
  if exist "%TOOLS%..\%BUILD%-%%A\KHGWin.exe" (
    echo Testing %BUILD%-%%A ...
    taskkill /f /im KHGWin.exe >nul 2>&1
    xcopy /y /e /q "%TOOLS%..\%BUILD%-%%A\*" "%SYS%\" >nul || echo Could not copy %BUILD%-%%A into Installed\System.
    echo Running the map test, this takes a few minutes ...
    pushd "%SYS%"
    start "" /wait KHGWin.exe -khgmaptest
    popd
    copy /y "%SYS%\KHGWin.log" "%TOOLS%MapTest-%BUILD%-%%A.log" >nul
  ) else (
    echo Build folder %BUILD%-%%A not found in builds.
  )
)
echo Done. Logs are in builds\tools. The 32-bit build is now the one in Installed\System.
pause
