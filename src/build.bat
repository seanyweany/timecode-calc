@echo off

if exist ..\tccalc.exe (
    del ..\tccalc.exe
)

gcc main.c -o ..\tccalc.exe
