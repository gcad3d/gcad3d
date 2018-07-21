#!/bin/sh

# ./NEW_VERSION3.sh


basDir=${DIR_DEV}gcad3d


# create source-package (<basDir>/packages/gCAD3D-#.##-src.zip)
cd ${basDir}/src
./pack_src.sh



# create doc-package (${DIR_DEV}gcad3d/packages/gCAD3D-doc-#.##.zip)
# (user-doc and dev-doc)
cd ${basDir}/doc
./packDoc.sh



# create <basDir>/packages/upd.zip
cd ${basDir}/packages
./packUpd.sh


echo fertig

# EOF
