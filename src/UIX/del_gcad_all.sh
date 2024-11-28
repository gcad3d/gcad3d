#!/bin/bash
#
# delete all gcad-objects, -dlls, -exes;

echo "- entering del_gcad_all.sh - delete all gcad-objs -"
echo "- in ${gcad_dir_bin}"


if test "${gcad_dir_bin}" = ""; then
  echo "**** Directory gcad_dir_bin is not set."
  exit 1
fi

rmCmd="`which rm` -f"

cd "${gcad_dir_bin}"

# del all obj's
echo $rmCmd *.o
$rmCmd *.o

## PRC_cut1.mak
#echo $rmCmd ../prc/cut1/*.o
#$rmCmd ../prc/cut1/*.o

# del. exe
echo $rmCmd gCAD3D*
$rmCmd gCAD3D*

# remove GUI_* executables
echo $rmCmd GUI_*
$rmCmd GUI_*

# remove plugins
echo $rmCmd plugins/*.*
$rmCmd plugins/*.*


# remove PP's
echo $rmCmd plugins/cut1/*.*
$rmCmd plugins/cut1/*


# del all dll's
echo $rmCmd xa_*
$rmCmd xa_*

# eof
