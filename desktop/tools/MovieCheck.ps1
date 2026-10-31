# Writes KHGMovie.ini for MovieCheck.bat: the retail Khg.ini with NumPad1-NumPad6 bound to
# "exec" of the movie sets in tools\Movies (1-*.txt ... 6-*.txt). Only KHGMovie.ini is written.
param(
  [string]$Sys,     # Installed\System
  [string]$Movies   # builds\tools\Movies
)
$ErrorActionPreference = 'Stop'
$sets = @{}
foreach ($f in Get-ChildItem $Movies -Filter '*.txt') {
  if ($f.Name -match '^(\d)-') { $sets[[int]$Matches[1]] = $f.Name }
}
$out = foreach ($line in Get-Content (Join-Path $Sys 'Khg.ini')) {
  if ($line -match '^NumPad[1-6]=') { continue }
  $line
  if ($line -eq '[Engine.Input]') {
    foreach ($n in ($sets.Keys | Sort-Object)) { "NumPad$n=exec ..\..\builds\tools\Movies\$($sets[$n])" }
  }
}
$out | Set-Content (Join-Path $Sys 'KHGMovie.ini')
Write-Host "Wrote KHGMovie.ini (NumPad1-$($sets.Count) play the movie sets in retail)."
