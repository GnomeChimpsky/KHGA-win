@echo off
rem Runs the retail Khg.exe on a fresh copy of default.ini, so its log can be compared with KHGWin.log.
rem Writes only KHGRef.ini and KHGRef.log in Installed\System; Khg.ini is not touched.
cd /d "%~dp0..\..\Installed\System"
copy /y default.ini KHGRef.ini >nul
start "" /wait Khg.exe INI=KHGRef.ini LOG=KHGRef.log
echo Done. Log written to Installed\System\KHGRef.log
pause
