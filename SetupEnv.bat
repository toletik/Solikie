@echo off
@echo Setup Perforce : 
@echo ----------------
where p4v.exe >nul 2>nul
if not ERRORLEVEL 0 (
    @echo p4v.exe not found in path.
) else (
    p4 set P4IGNORE=.p4ignore.txt
    @echo p4ignore : OK
)
pause


