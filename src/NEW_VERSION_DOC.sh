#!/bin/sh

# ./NEW_VERSION_DOC.sh
# start in <baseDir>/gcad3d/src/

srcDir=`pwd`
cd ..
basDir=`pwd`



# rebuild development-documentation:
echo "Rebuild development-documentation; cd ${basDir}/doc"
cd ${basDir}/doc
./gcad_doxygen.sh


echo fertig

# EOF
