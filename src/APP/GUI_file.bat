@echo off
:: start with   GUI_file.bat


::nmake -f GUI_file.nmak

set fnID=%TEMP%\temp_%RANDOM%

::START /B /WAIT "" "X:\devel\gcad3d\binMS32\GUI_file_gtk2_MS.exe" open X:\devel\cadfiles\gcad\ "C:\Users\freiter\AppData\Roaming\gCAD3D\cfg_MS\dir.lst" "*" Öffnen > t2

CMD /C ""X:\devel\gcad3d\binMS32\GUI_file_gtk2_MS.exe" open "X:\devel\cadfiles\gcad\\" "C:\Users\freiter\AppData\Roaming\gCAD3D\cfg_MS\dir.lst" "*" "Datei Öffnen" "%fnID%""




::START "" /B /WAIT "X:\devel\gcad3d\binMS32\GUI_file_gtk2_MS.exe" open "C:\Users\freiter\AppData\Roaming\gCAD3D\dat\gMsh\ " "C:\Users\freiter\AppData\Roaming\gCAD3D\cfg_MS\dir.lst" "*"


echo "---out : --------------------------------"
type %fnID%
echo "-----------------------------------------"

del %fnID%

::type %TMP%\debug.dat

:: EOF
