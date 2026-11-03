# M7: writes Shots\Wide-<build>.html, every WideShots view at 4:3, 16:9 and 21:9 with the retail field of view
# (vert-) and with Hor+. The screenshots are taken without the present gamma (like RenderShots); the page applies
# the Glide present curve of the default Brightness 0.5 (exponent 1/1.25) so they look as they do in the game.
# Run by WideShots.bat, or: powershell -File WideCompare.ps1 -Out <Shots\Wide-build folder> -Build <commit>
param( [Parameter(Mandatory=$true)][string]$Out, [Parameter(Mandatory=$true)][string]$Build )
$ErrorActionPreference = 'Stop'
$sets = @(
  @{ Dir='1440x1080-0'; Label='4:3 (1440x1080)' },
  @{ Dir='1920x1080-0'; Label='16:9 retail FOV' },
  @{ Dir='1920x1080-1'; Label='16:9 Hor+' },
  @{ Dir='2560x1080-0'; Label='21:9 retail FOV' },
  @{ Dir='2560x1080-1'; Label='21:9 Hor+' } )
$folder = Split-Path $Out -Leaf
$names = @( foreach( $s in $sets ) { $d = Join-Path $Out $s.Dir; if( Test-Path $d ) { Get-ChildItem $d -Filter *.bmp | ForEach-Object Name } } ) | Sort-Object -Unique
$rows = foreach( $n in $names ) {
  $cells = foreach( $s in $sets ) {
    if( Test-Path (Join-Path (Join-Path $Out $s.Dir) $n) ) { "<figure><img src=""$folder/$($s.Dir)/$n"" loading=lazy><figcaption>$($s.Label)</figcaption></figure>" }
    else { "<figure><div class=none>missing</div><figcaption>$($s.Label)</figcaption></figure>" }
  }
  "<h2>$([IO.Path]::GetFileNameWithoutExtension($n))</h2><div class=row>$($cells -join '')</div>"
}
$html = @"
<!doctype html><html><head><meta charset=utf-8><title>KHGWin $Build widescreen</title>
<style>body{background:#111;color:#ddd;font:14px sans-serif;margin:16px}h2{font-size:15px;margin:22px 0 6px}
.row{display:flex;flex-wrap:wrap;gap:8px;align-items:flex-end}figure{margin:0}figure img{height:300px;width:auto;display:block;filter:url(#g);cursor:zoom-in}
figure img.big{height:auto;width:96vw}figcaption{font-size:12px;color:#aaa;margin-top:2px}.none{height:300px;width:200px;background:#222;display:flex;align-items:center;justify-content:center}</style></head>
<body><svg width=0 height=0 style=position:absolute><filter id=g color-interpolation-filters=sRGB><feComponentTransfer>
<feFuncR type=gamma exponent=0.8 /><feFuncG type=gamma exponent=0.8 /><feFuncB type=gamma exponent=0.8 /></feComponentTransfer></filter></svg>
<h1>KHGWin $Build at 4:3, 16:9 and 21:9 (click a picture to enlarge)</h1>
$($rows -join "`n")
<script>document.querySelectorAll('figure img').forEach(i=>i.onclick=()=>i.classList.toggle('big'))</script>
</body></html>
"@
$page = Join-Path (Split-Path $Out -Parent) "Wide-$Build.html"
Set-Content -Path $page -Value $html -Encoding UTF8
Write-Host "Widescreen page: $page"
