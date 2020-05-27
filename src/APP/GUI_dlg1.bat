@echo off
:: start with   GUI_dlg1.bat


::nmake -f GUI_dlg1.nmak


START /B /WAIT "" "X:\devel\gcad3d\binMS32\GUI_dlg1_gtk2_MS.exe" list1 C:\Users\freiter\AppData\Roaming\gCAD3D\tmp\plugins.lst "select program" "x40,y20"


type %TMP%\debug.dat

:: EOF
