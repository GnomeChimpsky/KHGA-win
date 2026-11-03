@echo off
rem Loads the retail save builds\tools\RetailSave\Save0.usa (a copy of the one Khg.exe made) with the
rem 64-bit and then the 32-bit KHGWin, logs every pawn right after loading and after 300 ticks.
rem Logs: builds\tools\RetailSaveTest-<build>-<arch>.log. The real Installed\Save folder is not used.
set BUILD=%~1
if "%BUILD%"=="" set BUILD=aaabbec
set TOOLS=%~dp0
set SYS=%~dp0..\..\Installed\System
for %%A in (win64 win32) do (
  if exist "%TOOLS%..\%BUILD%-%%A\KHGWin.exe" (
    echo Loading the retail save with %BUILD%-%%A ...
    taskkill /f /im KHGWin.exe >nul 2>&1
    xcopy /y /e /q "%TOOLS%..\%BUILD%-%%A\*" "%SYS%\" >nul || echo Could not copy %BUILD%-%%A into Installed\System.
    pushd "%SYS%"
    start "" /wait KHGWin.exe -khgloadsave=0 -khgsavedir=..\..\builds\tools\RetailSave LOG=KHGWin.log
    popd
    copy /y "%SYS%\KHGWin.log" "%TOOLS%RetailSaveTest-%BUILD%-%%A.log" >nul
  ) else (
    echo Build folder %BUILD%-%%A not found in builds.
  )
)
echo Done. Logs are in builds\tools. The 32-bit build is now the one in Installed\System.
pause
