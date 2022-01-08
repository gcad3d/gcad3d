@echo off
:: start with   GUI_file.bat


::nmake -f GUI_file.nmak


START /B /WAIT "" "X:\devel\gcad3d\binMS32\GUI_file_gtk2_MS.exe" open X:\devel\cadfiles\gcad\ "C:\Users\freiter\AppData\Roaming\gCAD3D\cfg_MS\dir.lst" "*" Öffnen


type %TMP%\debug.dat

:: EOF
