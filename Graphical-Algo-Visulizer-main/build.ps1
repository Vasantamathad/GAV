$ErrorActionPreference = "Stop"

Stop-Process -Name "visualizer" -ErrorAction SilentlyContinue
Write-Host "=== Compiling main.c ==="
g++ main.c -o visualizer.exe -I. -L. -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32 -fpermissive

Write-Host "=== SUCCESS! Launching Visualizer ==="
.\visualizer.exe
