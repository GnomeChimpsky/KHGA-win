@echo off
rem M7 widescreen check. Takes the same camera views as RenderShots (plus the in-game menu) in the 64-bit
rem KHGWin build named below at 4:3, 16:9 and 21:9, retail-style (stretched HUD, vert- view) and widescreen
rem (Hor+ view, unstretched HUD panels, 4:3 menu), into
rem builds\tools\Shots\Wide-<build>, and opens a page that shows them side by side.
rem Double-click to run, or WideShots.bat <commit> [maps...]. KHGWin only (no retail run); about a minute per map.
rem Copies only the KHGWin files into Installed\System. KHGWin.ini is not changed (sizes and modes are per run).
setlocal
set BUILD=%~1
if "%BUILD%"=="" set BUILD=4a90344
set MAPS=%2 %3 %4 %5 %6 %7 %8 %9
if "%~2"=="" set MAPS=M02 M09A M17 m20b
set TOOLS=%~dp0
set SYS=%~dp0..\..\Installed\System
set SHOTS=%TOOLS%Shots
set OUT=%SHOTS%\Wide-%BUILD%
if not exist "%TOOLS%..\%BUILD%-win64\KHGWin.exe" (
  echo Build folder %BUILD%-win64 not found in builds.
  goto fail
)
taskkill /f /im KHGWin.exe >nul 2>&1
xcopy /y /e /q "%TOOLS%..\%BUILD%-win64\*" "%SYS%\" >nul || (echo Could not copy %BUILD%-win64 into Installed\System. & goto fail)
if not exist "%SHOTS%\WideViews" mkdir "%SHOTS%\WideViews"
if exist "%OUT%" rmdir /s /q "%OUT%"
mkdir "%OUT%"
pushd "%SYS%"
rem The views: RenderShots' views of each map (planned now if there are none), then the menu over the last one.
for %%M in (%MAPS%) do (
  if not exist "%SHOTS%\WideViews\%%M.txt" (
    if exist "%SHOTS%\Views\%%M.txt" (
      copy /y "%SHOTS%\Views\%%M.txt" "%SHOTS%\WideViews\%%M.txt" >nul
    ) else (
      echo   planning views for %%M
      start "" /wait KHGWin.exe -khgshots=..\..\builds\tools\Shots\Wide-%BUILD%\plan -khgviews=..\..\builds\tools\Shots\WideViews -khgmap=%%M.unr -khgres=1024x768 LOG=KHGWin.log
    )
    >>"%SHOTS%\WideViews\%%M.txt" echo view menu
    >>"%SHOTS%\WideViews\%%M.txt" echo ShowMenu
  )
)
if exist "%OUT%\plan" rmdir /s /q "%OUT%\plan"
set N=0
for %%R in (1440x1080 1920x1080 2560x1080) do (
  for %%H in (0 1) do (
    if not "%%R%%H"=="1440x10801" (
      echo [%%R, widescreen %%H]
      for %%M in (%MAPS%) do (
        echo   %%M
        start "" /wait KHGWin.exe -khgshots=..\..\builds\tools\Shots\Wide-%BUILD%\%%R-%%H -khgviews=..\..\builds\tools\Shots\WideViews -khgmap=%%M.unr -khgres=%%R -khghorplus=%%H -khgwidehud=%%H LOG=KHGWin.log
        copy /y KHGWin.log "%OUT%\KHGWin-%%R-%%H-%%M.log" >nul
      )
    )
  )
)
popd
dir /b /s "%OUT%\*.bmp" >nul 2>&1 || (echo No screenshots were written, see Shots\Wide-%BUILD%\KHGWin-*.log & goto fail)
powershell -NoProfile -ExecutionPolicy Bypass -File "%TOOLS%WideCompare.ps1" -Out "%OUT%" -Build %BUILD% || goto fail
start "" "%SHOTS%\Wide-%BUILD%.html"
echo Done. Screenshots are in builds\tools\Shots\Wide-%BUILD%.
pause
exit /b 0
:fail
echo FAILED.
pause
exit /b 1
