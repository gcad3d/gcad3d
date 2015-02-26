@echo off

REM copy all for ckitgui/gcad3d necessary files

set inDir=X:\Devel\dev\gCAD3D\binMS32
set outDir=X:\Devel\BACKUP\ckitgui\binMS32


xcopy/I/E %inDir%\lib           %outDir%\lib
xcopy/I/E %inDir%\include       %outDir%\include

REM copy/Y %inDir%\lib*.dll       %outDir%\.
REM copy/Y %inDir%\jpeg62.dll     %outDir%\.
REM copy/Y %inDir%\intl.dll       %outDir%\.
REM copy/Y %inDir%\freetype6.dll  %outDir%\.
REM copy/Y %inDir%\zlib1.dll      %outDir%\.
REM copy/Y %inDir%\gzip.exe       %outDir%\.
REM copy/Y %inDir%\pixmap.exe     %outDir%\.
REM copy/Y %inDir%\djpeg.exe      %outDir%\.
REM copy/Y %inDir%\zenity.exe     %outDir%\.
REM copy/Y %inDir%\zenity.ui      %outDir%\.

copy/Y %inDir%\fPrint.exe     %outDir%\.

:Fertig
