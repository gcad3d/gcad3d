@ECHO OFF
echo "- start with test_gCAD3D.bat"

set gcad_dir_bin=%ProgramFiles%\gCAD3D\
set gcad_dir_bas=%ProgramFiles%\gCAD3D\
set gcad_dir_local=%APPDATA%\gCAD3D\

if .gdb == .%1 (
  shift
  "C:\Program Files (x86)\Windows Kits\10\Debuggers\x64\windbg.exe" "%gcad_dir_bin%gCAD3D.exe" %*
REM windbg "%gcad_dir_bin%\gCAD3D.exe"
) else (
  "%gcad_dir_bin%gCAD3D.exe" %*
)

:: EOF
