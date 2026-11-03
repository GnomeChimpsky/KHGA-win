@echo off
rem Opens KHGWin at the shiny-floor spots (m19 first, then m20b when you quit) so the mirror floors can be
rem compared with the retail game. Double-click to run, or SpotCheck.bat <commit> [maps...].
rem Copies only the KHGWin files into Installed\System, like RenderShots.bat. The spots are in tools\Spots.
rem In the retail game the same spot is: Tab, "open m19", then Tab and the two lines from Spots\m19.txt.
setlocal
set BUILD=%~1
if "%BUILD%"=="" set BUILD=4a90344
set MAPS=%2 %3 %4 %5 %6 %7 %8 %9
if "%~2"=="" set MAPS=m19 m20b
set TOOLS=%~dp0
set SYS=%~dp0..\..\Installed\System
if not exist "%TOOLS%..\%BUILD%-win64\KHGWin.exe" (
  echo Build folder %BUILD%-win64 not found in builds.
  goto fail
)
taskkill /f /im KHGWin.exe >nul 2>&1
xcopy /y /e /q "%TOOLS%..\%BUILD%-win64\*" "%SYS%\" >nul || (echo Could not copy %BUILD%-win64 into Installed\System. & goto fail)
pushd "%SYS%"
for %%M in (%MAPS%) do (
  if not exist "%TOOLS%Spots\%%M.txt" (echo No spot file Spots\%%M.txt. & popd & goto fail)
  echo %%M: look along the floor, then quit the game to go on.
  start "" /wait KHGWin.exe %%M.unr -khgexec=..\..\builds\tools\Spots\%%M.txt LOG=KHGWin.log
)
popd
echo Done.
pause
exit /b 0
:fail
echo FAILED.
pause
exit /b 1
