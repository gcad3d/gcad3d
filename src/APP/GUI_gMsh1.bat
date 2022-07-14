@echo off
:: start with   GUI_gMsh1.bat
:: ..\gui_gtk2_MS.c

nmake -f GUI_gMsh1.nmak

:: START /B /WAIT "" "X:\devel\gcad3d\binMS32\GUI_gMsh1_gtk2_MS.exe" X:\devel\cadfiles\gcad\gMsh\ unknown.gmsh1

START /B /WAIT "" "X:\devel\gcad3d\binMS32\GUI_gMsh1_gtk2_MS.exe" X:\devel\cadfiles\gcad\gMsh\ unknown.gmsh1

::START /B /WAIT "" "X:\devel\gcad3d\binMS32\GUI_gMsh1_gtk2_MS.exe" X:\devel\cadfiles\gcad\gMsh\ unknown.gmsh1

type %TMP%\debug.dat

:: EOF
