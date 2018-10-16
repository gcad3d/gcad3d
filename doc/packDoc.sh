#!/bin/sh

# create gCAD3D-doc-x.xx.zip
# start in ${DIR_DEV}gcad3d


docDir=${DIR_DEV}gcad3d/doc
srcDir=${DIR_DEV}gcad3d/src
instDir=${DIR_DEV}gcad3d/packages


Version=`cat ${srcDir}/gcad_version`
Product="gCAD3D-doc"
packNam="${Product}-${Version}"
fileNam=${instDir}/${packNam}.zip


cd ${DIR_DEV}gcad3d/

echo "- create ${fileNam} started .."


#-----------------------------------------------------
tmpDir=${instDir}/tmp
rm -rf ${tmpDir}/*



#-----------------------------------------------------
# create all dirs
mkdir ${tmpDir}/doc
mkdir ${tmpDir}/doc/html
mkdir ${tmpDir}/doc/msg
mkdir ${tmpDir}/devdoc
mkdir ${tmpDir}/prg



#-----------------------------------------------------
# gCAD3D/doc
cp ${docDir}/html/*.htm              ${tmpDir}/doc/html/.
cp ${docDir}/html/*.png              ${tmpDir}/doc/html/.
cp ${docDir}/html/*.js               ${tmpDir}/doc/html/.

cp ${docDir}/msg/*.txt               ${tmpDir}/doc/msg/.

cp ${docDir}/gcad/*                  ${tmpDir}/devdoc/.

cp ${DIR_DEV}/gcad3d/gCAD3D/prg/DemoRC1.java ${tmpDir}/prg/.



#-----------------------------------------------------
# make Filellist gCAD3D.lst
cd ${tmpDir}
find doc -type f > gCAD3D.lst
find devdoc -type f >> gCAD3D.lst
find prg -type f >> gCAD3D.lst


# create Zip-File ${fileNam}
rm -rf ${fileNam}

zip gCAD3D-doc.zip `cat gCAD3D.lst`
if [ $? -ne 0 ]; then
  echo "****** ERROR zip *******"
  exit 1;
fi


mv gCAD3D-doc.zip ${fileNam}


# remove complete tmpDir
rm -rf ${tmpDir}/*


echo "****** File ${fileNam} created."

# eof
