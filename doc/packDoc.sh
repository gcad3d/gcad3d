#!/bin/sh

# create gCAD3D-doc-x.xx.zip
# start in ${DIR_DEV}gcad3d


docDir=${DIR_DEV}gcad3d/doc
srcDir=${DIR_DEV}gcad3d/src
instDir=${DIR_DEV}gcad3d/packages


Version=`cat ${srcDir}/gcad_version`
Product="gCAD3D-doc"
packNam="${Product}-${Version}"
fileNam=${DIR_DEV}gcad3d/packages/${packNam}.zip

#-----------------------------------------------------
# remove complete instDir
echo "rm -rf ${instDir}/doc"
rm -rf ${instDir}/doc
echo "rm -rf ${instDir}/devdoc"
rm -rf ${instDir}/devdoc
echo "rm -rf ${instDir}/prg"
rm -rf ${instDir}/prg



#-----------------------------------------------------
# create all dirs
mkdir ${instDir}/doc
mkdir ${instDir}/doc/html
mkdir ${instDir}/doc/msg
mkdir ${instDir}/devdoc
mkdir ${instDir}/prg



#-----------------------------------------------------
# gCAD3D/doc
cp ${docDir}/html/*.htm              ${instDir}/doc/html/.
cp ${docDir}/html/*.png              ${instDir}/doc/html/.
cp ${docDir}/html/*.js               ${instDir}/doc/html/.

cp ${docDir}/msg/*.txt               ${instDir}/doc/msg/.

cp ${docDir}/gcad/*                  ${instDir}/devdoc/.

cp ${basDir}/gCAD3D/prg/DemoRC1.java ${instDir}/prg/.



#-----------------------------------------------------
# make Filellist gCAD3D.lst
cd ${instDir}
find doc -type f > gCAD3D.lst
find devdoc -type f >> gCAD3D.lst
find prg -type f >> gCAD3D.lst


# create Zip-File ${fileNam}
rm -rf ${fileNam}
zip ${fileNam} `cat gCAD3D.lst`


# move gCAD3D-doc-x.xx.zip   ->   packages
echo "****** File ${fileNam} created."


# remove complete instDir
echo "rm -rf ${instDir}/doc"
rm -rf ${instDir}/doc
echo "rm -rf ${instDir}/devdoc"
rm -rf ${instDir}/devdoc
echo "rm -rf ${instDir}/prg"
rm -rf ${instDir}/prg


# eof
