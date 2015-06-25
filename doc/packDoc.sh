#!/bin/sh

# create gCAD3D-doc-x.xx.zip
# start in <basDir>/doc

docDir=`pwd`
cd ..
basDir=`pwd`
srcDir=${basDir}/src

Version=`cat ${srcDir}/gcad_version`
Product="gCAD3D-doc"

packNam="${Product}-${Version}"

instDir="${basDir}/packages"


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


# create Zip-File
zip ${basDir}/packages/${packNam}.zip `cat gCAD3D.lst`


# move gCAD3D-doc-x.xx.zip   ->   packages
echo "****** ${basDir}/packages/${packNam} created."


# remove complete instDir
echo "rm -rf ${instDir}/doc"
rm -rf ${instDir}/doc
echo "rm -rf ${instDir}/devdoc"
rm -rf ${instDir}/devdoc
echo "rm -rf ${instDir}/prg"
rm -rf ${instDir}/prg


# eof
