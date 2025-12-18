@echo off
if exist main.exe (
    del main.exe
    echo main.exe deleted.
) else (
    echo main.exe not found.
)
