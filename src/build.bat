@echo off

if exist ..\tccalc.exe (
    del ..\tccalc.exe
)

gcc tccalc.c -o ..\tccalc.exe
