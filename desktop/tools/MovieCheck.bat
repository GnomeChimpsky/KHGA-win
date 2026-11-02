@echo off
rem M4 movie check. Plays a set of movies in KHGWin (64-bit build named below), then lets you play the
rem same set in the retail Khg.exe, so the two can be compared by eye and ear.
rem Double-click to run, or MovieCheck.bat <commit>. The sets are the files in tools\Movies.
rem Copies only the KHGWin files into Installed\System; Khg.ini is not touched (retail uses KHGMovie.ini,
rem a copy of Khg.ini where NumPad1-6 play the sets).
setlocal EnableDelayedExpansion
set BUILD=%~1
if "%BUILD%"=="" set BUILD=@BUILD@
set TOOLS=%~dp0
set SYS=%~dp0..\..\Installed\System
if not exist "%TOOLS%..\%BUILD%-win64\KHGWin.exe" (
  echo Build folder %BUILD%-win64 not found in builds.
  goto fail
)
taskkill /f /im KHGWin.exe >nul 2>&1
taskkill /f /im Khg.exe >nul 2>&1
xcopy /y /e /q "%TOOLS%..\%BUILD%-win64\*" "%SYS%\" >nul || (echo Could not copy %BUILD%-win64 into Installed\System. & goto fail)
powershell -NoProfile -ExecutionPolicy Bypass -File "%TOOLS%MovieCheck.ps1" -Sys "%SYS%" -Movies "%TOOLS%Movies" || goto fail

:menu
echo.
echo Movie sets:
for %%F in ("%TOOLS%Movies\*.txt") do echo   %%~nF
echo.
set CHOICE=
set /p CHOICE=Number of the set to play (Enter to quit):
if "%CHOICE%"=="" goto done
set FILE=
for %%F in ("%TOOLS%Movies\%CHOICE%-*.txt") do set FILE=%%~nxF
if "%FILE%"=="" (echo No set %CHOICE%. & goto menu)
pushd "%SYS%"
echo.
echo [KHGWin] %FILE% plays right after start-up. Esc, Space, Enter or a click skips a movie, as in
echo          retail. Quit the game when it is done.
start "" /wait KHGWin.exe -khgexec=..\..\builds\tools\Movies\%FILE% LOG=KHGWin.log
copy /y KHGWin.log "%TOOLS%Movies-KHGWin.log" >nul
echo [Retail] At the main menu press NumPad%CHOICE% (or Tab, then: exec ..\..\builds\tools\Movies\%FILE%).
echo          Quit the game when it is done.
start "" /wait Khg.exe INI=KHGMovie.ini LOG=KHGMovie.log
popd
goto menu

:done
echo The KHGWin log of the last set is in builds\tools\Movies-KHGWin.log.
exit /b 0
:fail
echo FAILED.
pause
exit /b 1
