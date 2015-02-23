@echo off

set actdir=doc

if .%1% == . goto inperr



REM del *.ncb

cd ..


del %actdir%*.zip


echo zip %actdir%%1 %actdir%\*
zip %actdir%%1 %actdir%\* -x *.zip -x *.tag -x *.gz -x *.tar -x *~



echo copy %actdir%%1.zip a:
verify on

REM copy %actdir%%1.zip i:\sichXP\.
REM dir i:\sichXP\%actdir%*.*

copy %actdir%%1.zip a:
dir a:%actdir%*.*

verify off

cd %actdir%

goto Fertig


:inperr
echo dir ..\%actdir%*.zip
dir ..\%actdir%*.zip
echo Erster Inputparameter muss die Backupnummer sein
dir a:%actdir%*.zip
echo ^G

:Fertig
