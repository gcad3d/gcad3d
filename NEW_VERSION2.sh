#!/bin/sh

# ./NEW_VERSION2.sh
# start in <baseDir>/gcad3d/

basDir=`pwd`


# create debian-package (binary, <basDir>/packages/gCAD3D-#.##-bin-xxxxx.deb)
cd ${basDir}/debian
./pack_deb.sh


echo fertig

# EOF
