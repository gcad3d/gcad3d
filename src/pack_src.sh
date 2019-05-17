#!/bin/sh

# create zip-package gCAD3D_<version>-src.zip
# includes Linux and MS-windows version
# start in <basDir>/src

docDir=${DIR_DEV}gcad3d/doc
srcDir=${DIR_DEV}gcad3d/src
instDir=${DIR_DEV}gcad3d/packages


Version=`cat ${srcDir}/gcad_version`
Product="gCAD3D-src"
packNam="${Product}-${Version}"
fileNam=${instDir}/${packNam}.zip


cd ${DIR_DEV}gcad3d/

echo "- create ${fileNam} started .."



#-----------------------------------------------------
#cd ${DIR_DEV}gcad3d
#basDir=`pwd`
#version=`cat src/gcad_version`


rm -f ${fileNam}

zip gCAD3D-src.zip\
 TODO README LIC* sav_excl.txt -r\
 src\
 icons\
 rpm\
 debian\
 pack_MS\
 binMS32\
 doc\
 -x "doc/gcad/*" "*.tag" "*.obj" "*.exp" "*.pdb" "*.lib" "*/my*" "*/t1*"
if [ $? -ne 0 ]; then
  echo "****** ERROR zip *******"
  exit 1;
fi



mv gCAD3D-src.zip ${fileNam}


echo "****** File ${fileNam} created."


# eof
