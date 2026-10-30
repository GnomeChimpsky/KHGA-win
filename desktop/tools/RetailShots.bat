@echo off
rem Retail half of RenderShots.bat only: takes the retail Khg.exe screenshots for the views already in
rem builds\tools\Shots\Views. Double-click, or RetailShots.bat [maps...]. Don't touch the PC while it runs.
rem Output: builds\tools\Shots\Retail, log in builds\tools\Shots\RetailShots.log.
setlocal
set MAPS=%1 %2 %3 %4 %5 %6 %7 %8 %9
if "%~1"=="" set MAPS=M02 M04A M06B M09A M11 M14 M17 m19 m20b DR05
set TOOLS=%~dp0
set SYS=%~dp0..\..\Installed\System
set SHOTS=%TOOLS%Shots
if not exist "%SHOTS%\Views\*.txt" (echo No view files in Shots\Views. Run RenderShots.bat first. & goto fail)
taskkill /f /im KHGWin.exe >nul 2>&1
taskkill /f /im Khg.exe >nul 2>&1
echo Retail screenshots (hands off the keyboard and mouse) ...
if exist "%SHOTS%\Retail" rmdir /s /q "%SHOTS%\Retail"
powershell -NoProfile -ExecutionPolicy Bypass -File "%TOOLS%RetailShots.ps1" -Sys "%SYS%" -Views "%SHOTS%\Views" -Out "%SHOTS%\Retail" -Maps %MAPS: =,% || goto fail
echo Done. Screenshots are in builds\tools\Shots\Retail.
pause
exit /b 0
:fail
echo FAILED. Details are in builds\tools\Shots\RetailShots.log.
pause
exit /b 1
