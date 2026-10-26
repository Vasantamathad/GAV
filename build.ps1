$ErrorActionPreference = "Stop"

$ProjectDir = Join-Path $PSScriptRoot "Graphical-Algo-Visulizer-main"
if (-not (Test-Path $ProjectDir)) {
    $ProjectDir = $PSScriptRoot
}

Set-Location $ProjectDir
Stop-Process -Name "visualizer" -ErrorAction SilentlyContinue
Write-Host "=== Compiling main.c in $ProjectDir ==="
g++ main.c -o visualizer.exe -I. -L. -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32 -fpermissive

Write-Host "=== SUCCESS! Launching Visualizer ==="
.\visualizer.exe
