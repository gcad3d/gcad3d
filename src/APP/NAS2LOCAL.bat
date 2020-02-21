@echo on
:: copy <NAS>/devel/gcad3d/ into  %APPDATA%\devel\
:: start in X:\devel\gcad3d\src\APP   - get there with cdgc
:: get into %APPDATA%\devel\gcad3d\src\APP with cdgcl

set NAS=X:\devel\gcad3d
set LOCAL=%APPDATA%\devel\gcad3d

:: xcopy /s /i %NAS% %LOCAL%
robocopy %NAS% %LOCAL% /E /XO /PURGE


REM EOF
