# M3: writes Shots\Compare-<build>.html, a side-by-side page of every KHGWin screenshot next to the retail one.
# Retail screenshots are brightened by Khg.exe's screenshot gamma (1/(1.5*Brightness)); the page undoes that
# with an SVG gamma filter (exponent 0.75 at the default Brightness 0.5), so both sides show the frame as rendered.
# Run by RenderShots.bat / RetailShots.bat, or: powershell -File ShotsCompare.ps1 -Shots <Shots folder> -Build <commit>
param( [Parameter(Mandatory=$true)][string]$Shots, [Parameter(Mandatory=$true)][string]$Build )
$ErrorActionPreference = 'Stop'
$port = Join-Path $Shots $Build
$retail = Join-Path $Shots 'Retail'
if( -not (Test-Path $port) ) { throw "No screenshots in $port" }
$rows = foreach( $f in Get-ChildItem $port -Filter *.bmp | Sort-Object Name ) {
  $r = Join-Path $retail $f.Name
  $right = if( Test-Path $r ) { "<img class=r src=""Retail/$($f.Name)"">" } else { '<div class=none>no retail shot</div>' }
  "<h2>$($f.BaseName)</h2><div class=pair><img src=""$Build/$($f.Name)"">$right</div>"
}
$html = @"
<!doctype html><html><head><meta charset=utf-8><title>KHGWin $Build vs retail</title>
<style>body{background:#111;color:#ddd;font:14px sans-serif;margin:16px}h2{font-size:15px;margin:18px 0 6px}
.pair{display:flex;gap:8px}.pair img,.none{width:50%;height:auto}.r{filter:url(#lin)}.none{display:flex;align-items:center;justify-content:center;background:#222}</style></head>
<body><svg width=0 height=0 style=position:absolute><filter id=lin color-interpolation-filters=sRGB><feComponentTransfer>
<feFuncR type=gamma exponent=0.75 /><feFuncG type=gamma exponent=0.75 /><feFuncB type=gamma exponent=0.75 /></feComponentTransfer></filter></svg>
<h1>KHGWin $Build (left) vs retail Khg.exe (right)</h1>
$($rows -join "`n")
</body></html>
"@
$out = Join-Path $Shots "Compare-$Build.html"
Set-Content -Path $out -Value $html -Encoding UTF8
Write-Host "Side-by-side page: $out"
