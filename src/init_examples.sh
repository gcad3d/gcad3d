#!/bin/sh
#
# unpack examples.gz into test-environment (NOT INTO install-directory)
# use it for testing gcad3d without installation.
# unpack ${gcad_dir_dev}packages/examples.gz into ${gcad_dir_dev}gCAD3D/.
# for ${gcad_dir_dev} use options.sh


fnam=${gcad_dir_dev}packages/examples.gz
if [ ! -f ${fnam} ]; then
  echo "****** ERROR - file $fnam does not exist. *******"
  exit 2
else
  echo "- unpack "${fnam}
fi

mkdir -p ${gcad_dir_dev}
mkdir -p ${gcad_dir_dev}gCAD3D

cd ${gcad_dir_dev}gCAD3D
tar -xzf ${fnam}
if [ $? -ne 0 ]; then
  echo "****** ERROR unpacking package ${fnam} *******"
  exit 3
else
  echo "- example-files unpacked into ${gcad_dir_dev}gCAD3D *******"
fi

 exit 0

# EOF
