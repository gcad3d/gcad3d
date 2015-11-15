#!/bin/sh

# create zip-package gCAD3D_<version>-src.zip
# includes Linux and MS-windows version
# start in <basDir>/src

version=`cat gcad_version`


cd ..
basDir=`pwd`


rm -f gCAD3D-src.zip

zip gCAD3D-src.zip\
 ${basDir}/TODO ${basDir}/README ${basDir}/LIC* ${basDir}/sav_excl.txt -r\
 ${basDir}/src\
 ${basDir}/icons\
 ${basDir}/pack_MS\
 ${basDir}/binMS32\
 ${basDir}/doc\
 -x "${basDir}/doc/gcad/*" "*.tag" "*.obj" "*.exp" "*.pdb" "*.lib" "*/my*"


mv gCAD3D-src.zip ${basDir}/packages/gCAD3D_${version}-src.zip


# eof
