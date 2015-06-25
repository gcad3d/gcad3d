#!/bin/sh

# ./NEW_VERSION.sh
# start in <baseDir>/gcad3d/src/

srcDir=`pwd`
cd ..
basDir=`pwd`


#if [ -z "abc" ] ; then


# create new version-info
vi ${srcDir}/gcad_version


# create new version-date
vi ${srcDir}/gcad_date


# update History (end of file)
vi ${basDir}/README


gcad_version=`cat ${srcDir}/gcad_version`
gcad_date=`cat ${srcDir}/gcad_date`

echo "#define INIT_TXT \" gCAD3D Version ${gcad_version} / ${gcad_date}\"" > ${srcDir}/xa/gcad_version.h

touch ${srcDir}/xa/xa_main.c ${srcDir}/xa/xa_ui.c




# find nr_of_sourcelines, nr_of_functions
cd ${srcDir}/APP
./ut_stat.sh
# wait for blank or enter-key
echo "continue with enter-key .."
inp=`sh -c 'read INPLIN ; echo $INPLIN '`


# update logfile
cd ${basDir}/doc
vi gcad_log.txt


#fi


# rebuild development-documentation:
cd ${basDir}/doc
./gcad_doxygen.sh


# rebuild gCAD3D
cd ${srcDir}/APP
./do c


# test gCAD3D
./do


# create gCAD3D.def for MS-Windows;
cd ${srcDir}/APP
./gendef.sh



# create debian-package (binary, <basDir>/packages/gCAD3D-#.##-bin-xxxxx.deb)
cd ${basDir}/debian
./pack_deb.sh



# create source-package (<basDir>/packages/gCAD3D-#.##-src.zip)
cd ${basDir}
./pack_src.sh



# create docu-package (<basDir>/packages/gCAD3D-doc-#.##.zip)
cd ${basDir}/doc
./packDoc.sh



# create <basDir>/packages/upd.zip
cd ${basDir}/packages
./packUpd.sh


echo fertig

# EOF
