#!/bin/sh

bitNr=`getconf LONG_BIT`
echo $bitNr"-bit-system"
BINLOC=${HOME}/gCAD3D
gcad_dir_bin=${BINLOC}/binLinux${bitNr}
echo "gcad_dir_bin =" ${gcad_dir_bin}

DirBasSer=/mnt/serv1/Devel/GITHUB/gcad3d
DirBinSer=${DirBasSer}/binLinux${bitNr}

# use it with ". ./options.sh"
#
