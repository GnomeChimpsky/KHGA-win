@echo off
rem M3 rendering comparison. Takes the same camera views as screenshots in KHGWin (64-bit build named
rem below) and in the retail Khg.exe (Glide), into builds\tools\Shots\<build> and builds\tools\Shots\Retail.
rem Double-click to run, or RenderShots.bat <commit> [maps...]. Don't touch the PC while the retail part runs:
rem it sends F11/F12 key presses to the game. Takes about a minute per map.
rem Copies only the KHGWin files into Installed\System; Khg.ini is not touched (the retail run uses KHGShot.ini).
setlocal
set BUILD=%~1
if "%BUILD%"=="" set BUILD=80d650e
set MAPS=%2 %3 %4 %5 %6 %7 %8 %9
if "%~2"=="" set MAPS=M02 M04A M06B M09A M11 M14 M17 m19 m20b DR05
set TOOLS=%~dp0
set SYS=%~dp0..\..\Installed\System
set SHOTS=%TOOLS%Shots
if not exist "%TOOLS%..\%BUILD%-win64\KHGWin.exe" (
  echo Build folder %BUILD%-win64 not found in builds.
  goto fail
)
taskkill /f /im KHGWin.exe >nul 2>&1
taskkill /f /im Khg.exe >nul 2>&1
xcopy /y /e /q "%TOOLS%..\%BUILD%-win64\*" "%SYS%\" >nul || (echo Could not copy %BUILD%-win64 into Installed\System. & goto fail)
if not exist "%SHOTS%\Views" mkdir "%SHOTS%\Views"
if exist "%SHOTS%\%BUILD%" rmdir /s /q "%SHOTS%\%BUILD%"
mkdir "%SHOTS%\%BUILD%"
echo [1/2] KHGWin screenshots ...
pushd "%SYS%"
for %%M in (%MAPS%) do (
  echo   %%M
  start "" /wait KHGWin.exe -khgshots=..\..\builds\tools\Shots\%BUILD% -khgviews=..\..\builds\tools\Shots\Views -khgmap=%%M.unr LOG=KHGWin.log
  copy /y KHGWin.log "%SHOTS%\%BUILD%\KHGWin-%%M.log" >nul
)
popd
dir /b "%SHOTS%\%BUILD%\*.bmp" >nul 2>&1 || (echo No KHGWin screenshots were written, see Shots\%BUILD%\KHGWin-*.log & goto fail)
echo [2/2] Retail screenshots (hands off the keyboard and mouse) ...
if exist "%SHOTS%\Retail" rmdir /s /q "%SHOTS%\Retail"
powershell -NoProfile -ExecutionPolicy Bypass -File "%TOOLS%RetailShots.ps1" -Sys "%SYS%" -Views "%SHOTS%\Views" -Out "%SHOTS%\Retail" -Maps %MAPS: =,% || goto fail
powershell -NoProfile -ExecutionPolicy Bypass -File "%TOOLS%ShotsCompare.ps1" -Shots "%SHOTS%" -Build %BUILD% || goto fail
start "" "%SHOTS%\Compare-%BUILD%.html"
echo Done. Screenshots are in builds\tools\Shots\%BUILD% and builds\tools\Shots\Retail.
pause
exit /b 0
:fail
echo FAILED.
pause
exit /b 1
