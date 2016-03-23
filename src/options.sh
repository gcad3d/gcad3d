#!/bin/sh
# see also ../options.mak
# set gcad_dir_bin

bitNr=`getconf LONG_BIT`
echo $bitNr"-bit-system"

BINLOC=${HOME}/dev/gCAD3D

gcad_dir_bin=${BINLOC}/binLinux${bitNr}
echo "gcad_dir_bin =" ${gcad_dir_bin}
/bin/mkdir -p ${gcad_dir_bin}/plugins


#DirBasSer=/mnt/serv1/Devel/GITHUB/gcad3d
DirBasSer=${DIR_DEV}GITHUB/gcad3d

DirBinSer=${DirBasSer}/binLinux${bitNr}

# use it with ". ./options.sh"
#
