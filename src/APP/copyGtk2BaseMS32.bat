@echo off

REM copy all for ckitgui/gcad3d necessary files

set inDir=X:\Devel\dev\gCAD3D\binMS32
set outDir=X:\Devel\BACKUP\gcad3d\binMS32


xcopy/I/E %inDir%\lib           %outDir%\lib
xcopy/I/E %inDir%\include       %outDir%\include

copy/Y %inDir%\lib*.dll       %outDir%\.
copy/Y %inDir%\jpeg62.dll     %outDir%\.
copy/Y %inDir%\intl.dll       %outDir%\.
copy/Y %inDir%\freetype6.dll  %outDir%\.
copy/Y %inDir%\zlib1.dll      %outDir%\.
copy/Y %inDir%\gzip.exe       %outDir%\.
copy/Y %inDir%\pixmap.exe     %outDir%\.
copy/Y %inDir%\djpeg.exe      %outDir%\.
copy/Y %inDir%\zenity.exe     %outDir%\.
copy/Y %inDir%\zenity.ui      %outDir%\.

copy/Y %inDir%\fPrint.exe     %outDir%\.

:Fertig
