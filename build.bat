@echo off
cd /d "%~dp0Graphical-Algo-Visulizer-main"
echo Closing any previous visualizer.exe instance...
taskkill /F /IM visualizer.exe >nul 2>&1
echo === Compiling main.c ===
g++ main.c -o visualizer.exe -I. -L. -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32 -fpermissive
if %ERRORLEVEL% EQU 0 (
    echo === Compilation Successful! Launching visualizer.exe ===
    visualizer.exe
) else (
    echo === Compilation Failed! Please check errors above ===
    pause
)
