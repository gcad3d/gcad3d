#!/bin/sh

# ./NEW_VERSION2.sh
# start in <baseDir>/gcad3d/

basDir=`pwd`



# create source-package (<basDir>/packages/gCAD3D-#.##-src.zip)
cd ${basDir}/src
#./pack_src.sh



# create docu-package (<basDir>/packages/gCAD3D-doc-#.##.zip)
cd ${basDir}/doc
#./packDoc.sh



# create <basDir>/packages/upd.zip
cd ${basDir}/packages
./packUpd.sh


echo fertig

# EOF
