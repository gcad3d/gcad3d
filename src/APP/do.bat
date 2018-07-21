@echo on
:: Ohne @echo in erster zeile wird nix kompiliert !!??
:: make gCAD3D for MS-Win/Visual-C-Compiler.
:: to update def-file execute ./gendef before.
:: VS-2010; "C:\Programme\Microsoft Visual Studio 8\Common7\Tools\vsvars32.bat"

echo "do complete          // do all following"
echo "do delObj            // delete all objects"
echo "do guiinit           // rebuild complete libgui
echo "do gui               // rebuild libgui_gtk2_MS.lib
echo "do c                 // compile & link core (gCAD3D.exe)"
echo "do all               // compile & link corelibs"
echo "do allDemos          // compile & link demos"
echo "do                   // start"
echo "do debug             // start with debugger"
echo "do cfg-ux2ms         // change cfg\xa.rc and cfg\dir.lst



:: set to Server or to Local;
set gcad_dir_bin=..\..\binMS32\
set gcad_dir_dev=..\..\

set gcad_dir_dev=X:\Devel\gcad3d\
set gcad_dir_bin=X:\Devel\gcad3d\binMS32\


:: set to Local;  %APPDATA%..


::set gcad_dir_local=%APPDATA%\



::::::::::::::::::::::::::::::::::::::::::::::::
REM debugger for VS2008:
set prgDeb="%ProgramFiles%\Debugging Tools for Windows (x86)\windbg.exe"

REM if exist %SystemRoot%\SysWOW64 goto L_64bit



::==============================================================
if .%1==. goto L_start


if gui==%1 (
  nmake -f gcad_gui__.nmak
  goto L_exit
)


if guiinit==%1 (
  nmake -f gcad3d.nmak guiinit
  goto L_exit
)


if c==%1 (
  nmake -f gcad3d.nmak
  goto L_exit
)


if delObj==%1 (
REM delete all gui-objects and gui-lib
  nmake -f gcad3d.nmak delObj
  goto L_exit
)


REM do help
if help==%1 (
  goto L_exit
)   


REM do all
if all==%1 (
  nmake -f gcad3d.nmak %1
  goto L_exit
)   


REM do allDemos
if allDemos==%1 (
  nmake -f gcad3d.nmak %1
  goto L_exit
)   


REM do complete
if complete==%1 (
  nmake -f gcad3d.nmak delobj
  nmake -f gcad3d.nmak guiinit
::nmake -f ..\gui_gtk2_MS\libgui.nmak
  nmake -f gcad_gui__.nmak
  nmake -f gcad3d.nmak
  nmake -f gcad3d.nmak all
  nmake -f gcad3d.nmak allDemos
  goto L_exit
)   


REM echo "do cfg-ux2ms         // change cfg\xa.rc and cfg\dir.lst
REM "do cfg-ux2ms         // change cfg\xa.rc and cfg\dir.lst
if cfg-ux2ms==%1 (
  if exist dirMS.lst (
    copy/y X:\Devel\GITHUB\gcad3d\gCAD3D\cfg\dir.lst dirUX.lst
    copy/y X:\Devel\GITHUB\gcad3d\gCAD3D\cfg\xa.rc xaUX.rc
    copy/y dirMS.lst X:\Devel\GITHUB\gcad3d\gCAD3D\cfg\dir.lst
    del dirMS.lst
    copy/y xaMS.rc X:\Devel\GITHUB\gcad3d\gCAD3D\cfg\xa.rc
  ) else (
    echo "**** ERROR"
  )
  goto L_exit
)   


::==============================================================
REM normal start
:L_start

set gcad_dir_local=%gcad_dir_dev%
set gcad_dir_doc=%gcad_dir_dev%doc\
set cfgDir=%gcad_dir_dev%gCAD3D\cfg\

echo "cfgDir=|%cfgDir%|"

REM change configfiles to MS-Windows if last start was with Unix
if exist UX.lock (
  set cfgDir=%gcad_dir_dev%gCAD3D\cfg\
  copy/y %cfgDir%xa.rc %cfgDir%xaUX.rc
  copy/y %cfgDir%dir.lst %cfgDir%dirUX.lst
  copy/y %cfgDir%xaMS.rc %cfgDir%xa.rc
  copy/y %cfgDir%dirMS.lst %cfgDir%dir.lst
  del UX.lock
  echo MS > MS.lock
)

set gcad_exe=%gcad_dir_bin%gCAD3D.exe


if .debug==.%1 (
windbg "%gcad_dir_bin%gCAD3D.exe"
) else (
"%gcad_dir_bin%gCAD3D.exe" %1
)


::==============================================================
:L_exit

REM EOF
