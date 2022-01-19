#!/bin/sh
#
# unpack examples.gz into test-environment (NOT INTO install-directory)
# use it for testing gcad3d without installation.
# unpack ${DIR_DEV}gcad3d/packages/examples.gz into ${DIR_DEV}gcad3d/gCAD3D/.


fnam=${gcad_dir_bas}packages/examples.gz
if [ ! -f ${fnam} ]; then
  echo "****** ERROR - file $fnam does not exist. *******"
  exit 2
fi

mkdir -p ${DIR_DEV}gcad3d
mkdir -p ${DIR_DEV}gcad3d/gCAD3D

cd ${DIR_DEV}gcad3d/gCAD3D
tar -xzf ${fnam}
if [ $? -ne 0 ]; then
  echo "****** ERROR unpacking package ${fnam} *******"
  exit 3
fi

 exit 0

# EOF
