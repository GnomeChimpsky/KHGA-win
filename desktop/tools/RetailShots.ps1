# Takes the M3 comparison screenshots in the retail Khg.exe: for each map's view file (written by
# KHGWin.exe -khgshots), starts Khg.exe on that map and, per view, runs the view's console commands
# (F11 = exec Shots\cur.txt) and takes a screenshot (F12 = shot). Keys are sent to the game window, so
# the PC must not be used while it runs.
# Writes only KHGShot.ini, KHGShot.log and (temporarily) Shot*.bmp in Installed\System; Khg.ini is not touched.
param(
  [string]$Sys,    # Installed\System
  [string]$Views,  # folder of <map>.txt view files
  [string]$Out,    # folder for <map>_<view>.bmp
  [string[]]$Maps  # map names; empty = every view file
)
$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.Windows.Forms
$shell = New-Object -ComObject WScript.Shell
New-Item -ItemType Directory -Force -Path $Out | Out-Null
$cur = Join-Path $Views '..\cur.txt'
$curRel = '..\..\builds\tools\Shots\cur.txt'   # exec path as Khg.exe sees it from Installed\System

# KHGShot.ini: the retail Khg.ini at 1024x768 fullscreen, with F11/F12 bound for the script.
$ini = Get-Content (Join-Path $Sys 'Khg.ini')
$section = ''
$ini = foreach ($line in $ini) {
  if ($line -match '^\[(.+)\]') { $section = $Matches[1] }
  if ($section -eq 'WinDrv.WindowsClient' -and $line -match '^ViewportX=') { 'ViewportX=1024'; continue }
  if ($section -eq 'WinDrv.WindowsClient' -and $line -match '^ViewportY=') { 'ViewportY=768'; continue }
  if ($section -eq 'WinDrv.WindowsClient' -and $line -match '^StartupFullscreen=') { 'StartupFullscreen=True'; continue }
  if ($section -eq 'Engine.Input' -and $line -match '^(F11|F12)=') { continue }
  $line
  if ($line -eq '[Engine.Input]') { "F11=exec $curRel"; 'F12=shot' }
}
$ini | Set-Content (Join-Path $Sys 'KHGShot.ini')
$bright = ($ini | Where-Object { $_ -match '^Brightness=' } | Select-Object -First 1) -replace 'Brightness=',''
"Brightness=$bright" | Set-Content (Join-Path $Out 'retail-brightness.txt')

$files = Get-ChildItem $Views -Filter *.txt | Where-Object { -not $Maps -or $Maps -contains $_.BaseName }
if (-not $files) { throw "No view files in $Views" }
$taken = 0; $failed = @()
foreach ($f in $files) {
  $map = $f.BaseName
  # Each "view <name>" line starts a block of console commands.
  $views = [ordered]@{}; $name = $null
  foreach ($line in Get-Content $f.FullName) {
    if ($line -match '^view (\S+)') { $name = $Matches[1]; $views[$name] = @(); continue }
    if ($name -and $line -and -not $line.StartsWith(';')) { $views[$name] += $line }
  }
  Write-Host "Retail $map : $($views.Count) views"
  $log = Join-Path $Sys 'KHGShot.log'
  Remove-Item $log -ErrorAction SilentlyContinue
  $p = Start-Process -FilePath (Join-Path $Sys 'Khg.exe') -WorkingDirectory $Sys -PassThru `
       -ArgumentList "..\Maps\$map.unr INI=KHGShot.ini LOG=KHGShot.log"
  # Wait for the level to come up (the log is written as the game runs), then for Glide to settle.
  $deadline = (Get-Date).AddSeconds(120)
  while ((Get-Date) -lt $deadline -and -not $p.HasExited) {
    Start-Sleep -Milliseconds 500
    if ((Test-Path $log) -and (Select-String -Path $log -Pattern "Bringing Level $map.MyLevel up for play" -SimpleMatch -Quiet)) { break }
  }
  if ($p.HasExited) { $failed += "$map (Khg.exe exited)"; continue }
  Start-Sleep -Seconds 8
  foreach ($v in $views.Keys) {
    $before = @(Get-ChildItem $Sys -Filter 'Shot*.bmp' | ForEach-Object Name)
    $views[$v] | Set-Content $cur
    [void]$shell.AppActivate($p.Id); Start-Sleep -Milliseconds 200
    [System.Windows.Forms.SendKeys]::SendWait('{F11}'); Start-Sleep -Milliseconds 900
    [void]$shell.AppActivate($p.Id)
    [System.Windows.Forms.SendKeys]::SendWait('{F12}'); Start-Sleep -Milliseconds 900
    $new = Get-ChildItem $Sys -Filter 'Shot*.bmp' | Where-Object { $before -notcontains $_.Name } | Select-Object -First 1
    if ($new) { Move-Item -Force $new.FullName (Join-Path $Out "${map}_$v.bmp"); $taken++ }
    else { $failed += "${map}_$v" }
  }
  'exit' | Set-Content $cur
  [void]$shell.AppActivate($p.Id); [System.Windows.Forms.SendKeys]::SendWait('{F11}')
  if (-not $p.WaitForExit(20000)) { Stop-Process -Id $p.Id -Force }
  Start-Sleep -Seconds 2
}
Remove-Item $cur -ErrorAction SilentlyContinue
Write-Host "Retail screenshots: $taken taken, $($failed.Count) failed"
if ($failed) { Write-Host ('FAILED: ' + ($failed -join ', ')) -ForegroundColor Red }
if ($taken -eq 0) { exit 1 }
