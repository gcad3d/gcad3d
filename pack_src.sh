#!/bin/sh

# create zip-package gCAD3D_<version>-src.zip
# includes Linux and MS-windows version

version=`cat src/gcad_version`

basDir=gcad3d


cd ..

rm -f ${basDir}/gCAD3D_${version}-src.zip

zip ${basDir}/gCAD3D_${version}-src.zip\
 ${basDir}/TODO ${basDir}/README ${basDir}/LIC*\
 -r ${basDir}/doc/gcad_doxygen ${basDir}/src ${basDir}/icons ${basDir}/binMS32\



#-x ${basDir}/doc/gcad/*\



# eof
