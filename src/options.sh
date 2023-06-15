#!/bin/sh
#
# set symbols: gcad_dir_dev gcad_dir_bin hTyp
# Using symbols DIR_DEV and DIR_BIN.
#
# DIR_DEV     home of sourcefiles, scripts, linkjobs, packages
# DIR_BIN     home of objects, executables, libraries
#
# start ${DIR_DEV}gcad3d/src/options.sh
#
#
# must be set before gcad3d.mak - export gcad_dir_dev and gcad_dir_bin or:
# . ../options.sh
#
# EXPORTS:
# gcad_dir_dev:     src/, icons/, doc/, README ..
# gcad_dir_bin:     *.o, *.so, gCAD3D, plugins/*.so
#
#
# see also ../../doc/html/SW_layout_en.htm

# test if active directory is xx/gcad3d/src/APP

echo entering options

## test if active directory is xx/gcad3d/src/APP
#S2=`expr length ${PWD} - 14`
#if [ `echo ${PWD} | cut -c$S2-` != "/gcad3d/src/APP" ] ; then
#  echo "****** ERROR - not in directory .../gcad3d/src/APP *****"
#  exit 1;
#fi



if [ ! -n "$DIR_DEV" ]; then
# not set:
# remove "/gcad3d/src/APP" (14 chars) from pwd
export DIR_DEV=`echo ${PWD} | cut -c1-$S2`
fi

if [ ! -n "$DIR_BIN" ]; then
# not set: 
# remove "/gcad3d/src/APP" (14 chars) from pwd and add "bin/"
export DIR_BIN=`echo ${PWD} | cut -c1-$S2`bin/
fi




# get host-typ; eg "Linux_x86_64"
hTyp=`uname -s`_`uname -m`


# gcad_dir_dev      home of sourcefiles, scripts, linkjobs, packages
export gcad_dir_dev=${DIR_DEV}gcad3d/

# gcad_dir_bin      home of objects, executables, libraries
export gcad_dir_bin=${DIR_BIN}gcad3d/${hTyp}/



mkdir -p ${gcad_dir_bin}
mkdir -p ${gcad_dir_bin}plugins/
mkdir -p ${gcad_dir_bin}plugins/cut1/


echo -exit options:
echo "DIR_DEV=|"${DIR_DEV}"|"
echo "DIR_BIN=|"${DIR_BIN}"|"
echo "gcad_dir_dev=|"${gcad_dir_dev}"|"
echo "gcad_dir_bin=|"${gcad_dir_bin}"|"
echo

# use it with ". ./options.sh"

#
