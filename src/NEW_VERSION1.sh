#!/bin/sh

# ./NEW_VERSION1.sh
# start in <baseDir>/gcad3d/src/

srcDir=`pwd`
cd ..
basDir=`pwd`


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
. ../options.sh
./ut_stat.sh
# wait for blank or enter-key
echo "continue with enter-key .."
inp=`sh -c 'read INPLIN ; echo $INPLIN '`


# update logfile
cd ${basDir}/doc
vi gCAD3D_log.txt


# rebuild gCAD3D
echo "build Core; cd ${srcDir}/APP"
cd ${srcDir}/APP
./do c


# test gCAD3D
./do


# create gCAD3D.def for MS-Windows;
cd ${srcDir}/APP
./gendef.sh



echo fertig

# EOF
