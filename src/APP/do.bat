@echo off
:: Ohne @echo in erster zeile wird nix kompiliert !!??
:: make gCAD3D for MS-Win/Visual-C-Compiler.
:: to update def-file execute ./gendef before.

echo "do complete          // do all following"
echo "do delObj            // delete all objects"
echo "do guiinit           // change gui-version"
echo "do gui               // rebuild libgui_gtk2_MS.lib
echo "do c                 // compile & link core (gCAD3D.exe)"
echo "do all               // compile & link corelibs"
echo "do allDemos          // compile & link demos"
echo "do                   // start"
echo "do d                 // start with debugger"



set gcad_dir_bin=..\..\binMS32\
set gcad_dir_local=%APPDATA%\
set gcad_exe=%gcad_dir_bin%gCAD3D.exe

REM debugger for VS2008:
set prgDeb="%ProgramFiles%\Debugging Tools for Windows (x86)\windbg.exe"


REM if exist %SystemRoot%\SysWOW64 goto L_64bit




::==============================================================
if .%1==. goto L_start


if gui==%1 (
  nmake -f gcad_gui__.nmak
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


REM delete all gui-objects and gui-lib
if guiinit==%1 (
  nmake -f gcad3d.nmak %1
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


if d==%1 (
REM start with debugger
  %prgDeb% %gcad_exe%
  goto L_exit
)


REM do complete
if complete==%1 (
  nmake -f gcad3d.nmak delobj
  nmake -f gcad3d.nmak guiinit
  nmake -f ..\gui_gtk2_MS\libgui.nmak
  nmake -f gcad3d.nmak
  nmake -f gcad3d.nmak all
  nmake -f gcad3d.nmak allDemos
  goto L_exit
)   


echo "**** ERROR; function not implemented *****"
goto L_exit


::==============================================================
REM start
:L_start
%gcad_exe%
goto L_exit


::==============================================================
:L_exit

REM EOF
