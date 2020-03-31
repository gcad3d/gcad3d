@echo off
:: SourceNavigationTool for gcad:
:: use cscript.exe


:: create file %TMP%\srcfiles.txt = list of sourcefiles
call devbase.bat
nmake -f gcad3d.nmak srclst
::nmake -f xa.nmak mgrep "OS=MS32"


::set EDITOR=D:\bin\vi
::set EDITOR=X:\MSwin\bin\vi
set EDITOR=vi.bat


:: Test with PED_init
::cscope -i %TMP%\srcfiles.txt
cscope -i srcfiles

:: EOF
