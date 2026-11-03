# M6 save compatibility, retail half. Two modes:
#   -Mode Save : for each map, starts the retail Khg.exe on it, lets it run, then saves with the console
#                command "savegame 9" (retail SAVEGAME takes one digit only), and renames the save to slot
#                100, 101, ... in map order. Saves go to -SaveDir and the slot -> map list to <SaveDir>\slots.txt.
#   -Mode Load : loads KHGWin saves in the retail Khg.exe ("open ?load=<slot>") from -SaveDir, using the
#                map list in -PortLog (a KHGWin -khgsavetest log), and takes a screenshot of each loaded save.
# Console commands go through F11 (= exec Compat\cmd.txt) and F12 (= shot), sent to the game window, so the
# PC must not be used while it runs. Writes only KHGSave.ini, KHGSave.log and (for a moment) Shot*.bmp in
# Installed\System; Khg.ini and the retail Save folder are not touched.
param(
  [string]$Sys,      # Installed\System
  [string]$Mode,     # Save or Load
  [string]$SaveDir,  # save folder, relative to Installed\System (no drive letter: the retail URL parser needs that)
  [string]$Out,      # folder for logs and screenshots
  [string]$PortLog,  # Load: the KHGWin -khgsavetest log that wrote the saves
  [string[]]$Maps    # map names; empty = every single-player map (M*.unr)
)
$ErrorActionPreference = 'Stop'
New-Item -ItemType Directory -Force -Path $Out | Out-Null
$transcript = Join-Path $Out "Retail$Mode.log"
try { Start-Transcript -Path $transcript -Force | Out-Null } catch { }
trap {
  Write-Host "ERROR: $_" -ForegroundColor Red
  Write-Host $_.InvocationInfo.PositionMessage
  try { Stop-Transcript | Out-Null } catch { }
  exit 1
}
$Maps = @($Maps -split '[, ]' | Where-Object { $_ })
Add-Type -AssemblyName System.Windows.Forms
$shell = New-Object -ComObject WScript.Shell
$saveFull = [System.IO.Path]::GetFullPath((Join-Path $Sys $SaveDir))
New-Item -ItemType Directory -Force -Path $saveFull | Out-Null
$cur = Join-Path $Out 'cmd.txt'
$curRel = '..\..\builds\tools\Compat\cmd.txt'   # exec path as Khg.exe sees it from Installed\System

# KHGSave.ini: the retail Khg.ini with the test save folder, 1024x768 fullscreen, F11/F12 bound.
$ini = Get-Content (Join-Path $Sys 'Khg.ini')
$section = ''
$ini = foreach ($line in $ini) {
  if ($line -match '^\[(.+)\]') { $section = $Matches[1] }
  if ($section -eq 'Core.System' -and $line -match '^SavePath=') { "SavePath=$SaveDir"; continue }
  if ($section -eq 'WinDrv.WindowsClient' -and $line -match '^ViewportX=') { 'ViewportX=1024'; continue }
  if ($section -eq 'WinDrv.WindowsClient' -and $line -match '^ViewportY=') { 'ViewportY=768'; continue }
  if ($section -eq 'WinDrv.WindowsClient' -and $line -match '^StartupFullscreen=') { 'StartupFullscreen=True'; continue }
  if ($section -eq 'Engine.Input' -and $line -match '^(F11|F12)=') { continue }
  $line
  if ($line -eq '[Engine.Input]') { "F11=exec $curRel"; 'F12=shot' }
}
$ini | Set-Content (Join-Path $Sys 'KHGSave.ini')
$log = Join-Path $Sys 'KHGSave.log'

function Wait-Log([System.Diagnostics.Process]$p, [string]$pattern, [int]$seconds, [switch]$Regex) {
  $deadline = (Get-Date).AddSeconds($seconds)
  while ((Get-Date) -lt $deadline -and -not $p.HasExited) {
    Start-Sleep -Milliseconds 500
    if ((Test-Path $log) -and (Select-String -Path $log -Pattern $pattern -SimpleMatch:(-not $Regex) -Quiet)) { return $true }
  }
  return $false
}
function Send-Console([System.Diagnostics.Process]$p, [string[]]$lines) {
  $lines | Set-Content $cur
  [void]$shell.AppActivate($p.Id); Start-Sleep -Milliseconds 200
  [System.Windows.Forms.SendKeys]::SendWait('{F11}'); Start-Sleep -Milliseconds 900
}
function Stop-Game([System.Diagnostics.Process]$p) {
  if (-not $p.HasExited) {
    Send-Console $p @('exit')
    if (-not $p.WaitForExit(20000)) { Stop-Process -Id $p.Id -Force }
  }
  Start-Sleep -Seconds 2
}
function Start-Game([string]$map) {
  Remove-Item $log -ErrorAction SilentlyContinue
  return Start-Process -FilePath (Join-Path $Sys 'Khg.exe') -WorkingDirectory $Sys -PassThru `
         -ArgumentList "..\Maps\$map.unr INI=KHGSave.ini LOG=KHGSave.log"
}

$ok = 0; $failed = @()
if ($Mode -eq 'Save') {
  if (-not $Maps) {
    $Maps = @(Get-ChildItem (Join-Path $Sys '..\Maps') -Filter 'M*.unr' | Sort-Object Name | ForEach-Object BaseName)
  }
  $slots = @()
  for ($i = 0; $i -lt $Maps.Count; $i++) {
    $map = $Maps[$i]; $slot = 100 + $i
    $file = Join-Path $saveFull "Save$slot.usa"
    $tmp = Join-Path $saveFull 'Save9.usa'
    Remove-Item $file, $tmp -ErrorAction SilentlyContinue
    Write-Host "Retail $map -> slot $slot"
    $p = Start-Game $map
    if (-not (Wait-Log $p "Bringing Level $map.MyLevel up for play" 120)) {
      Write-Host "  level did not come up" -ForegroundColor Red; $failed += $map; Stop-Game $p; continue
    }
    Start-Sleep -Seconds 12          # let the level run (AI, movers) before saving
    Send-Console $p @('savegame 9')
    $deadline = (Get-Date).AddSeconds(30)
    while ((Get-Date) -lt $deadline -and -not (Test-Path $tmp)) { Start-Sleep -Milliseconds 500 }
    Start-Sleep -Seconds 2
    Stop-Game $p
    if (Test-Path $tmp) { Move-Item -Force $tmp $file }
    Copy-Item -Force $log (Join-Path $Out "KHGSave-$map.log") -ErrorAction SilentlyContinue
    if (Test-Path $file) { $ok++; $slots += "$slot $map" ; Write-Host "  saved ($((Get-Item $file).Length) bytes)" }
    else { Write-Host "  no save file" -ForegroundColor Red; $failed += $map }
  }
  $slots | Set-Content (Join-Path $saveFull 'slots.txt')
  Write-Host "Retail saves: $ok written, $($failed.Count) failed"
}
else {
  # Slot -> map from the KHGWin save test log: "[KHG] MapTest <n>/<total>: <map>.unr" is slot 100+n-1.
  $slotMap = [ordered]@{}
  foreach ($m in (Select-String -Path $PortLog -Pattern '\[KHG\] MapTest (\d+)/\d+: (\S+)\.unr')) {
    $slotMap[$m.Matches[0].Groups[2].Value] = 100 + [int]$m.Matches[0].Groups[1].Value - 1
  }
  if (-not $Maps) { $Maps = @($slotMap.Keys | Where-Object { $_ -like 'M*' }) }
  $shots = Join-Path $Out 'Shots'
  New-Item -ItemType Directory -Force -Path $shots | Out-Null
  $tag = Split-Path $saveFull -Leaf
  foreach ($map in $Maps) {
    $slot = $slotMap[$map]
    if (-not $slot -or -not (Test-Path (Join-Path $saveFull "Save$slot.usa"))) { Write-Host "No $tag save for $map"; $failed += "$map (no save)"; continue }
    Write-Host "Retail loads $tag $map (slot $slot)"
    $p = Start-Game $map
    if (-not (Wait-Log $p "Bringing Level $map.MyLevel up for play" 120)) {
      Write-Host "  level did not come up" -ForegroundColor Red; $failed += $map; Stop-Game $p; continue
    }
    Start-Sleep -Seconds 4
    Send-Console $p @("open ?load=$slot")
    # A loaded save doesn't log "Bringing Level"; the player is possessed in the Save<slot> package.
    $up = Wait-Log $p "Possessed PlayerPawn: \S+ Save$slot\." 60 -Regex
    if ($up) { Start-Sleep -Seconds 5 }
    $alive = -not $p.HasExited
    $shot = $false
    if ($up -and $alive) {
      $before = @(Get-ChildItem $Sys -Filter 'Shot*.bmp' | ForEach-Object Name)
      [void]$shell.AppActivate($p.Id); [System.Windows.Forms.SendKeys]::SendWait('{F12}'); Start-Sleep -Milliseconds 900
      $new = Get-ChildItem $Sys -Filter 'Shot*.bmp' | Where-Object { $before -notcontains $_.Name } | Select-Object -First 1
      if ($new) { Move-Item -Force $new.FullName (Join-Path $shots "${tag}_$map.bmp"); $shot = $true }
      Start-Sleep -Seconds 3
      $alive = -not $p.HasExited
    }
    Stop-Game $p
    Copy-Item -Force $log (Join-Path $Out "KHGLoad-$tag-$map.log") -ErrorAction SilentlyContinue
    if ($up -and $alive) { $ok++; Write-Host "  loaded$(if (-not $shot) { ' (no screenshot)' })" }
    else { Write-Host "  FAILED (level up: $up, still running: $alive)" -ForegroundColor Red; $failed += $map }
  }
  Write-Host "Retail loads of $tag saves: $ok loaded, $($failed.Count) failed"
}
Remove-Item $cur -ErrorAction SilentlyContinue
if ($failed) { Write-Host ('FAILED: ' + ($failed -join ', ')) -ForegroundColor Red }
try { Stop-Transcript | Out-Null } catch { }
