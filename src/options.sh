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
# gcad_dir_dev:     src/, icons/, doc/, README ..
# gcad_dir_bin:     *.o, *.so, gCAD3D, plugins/*.so
# hTyp              eg "Linux_x86_64"
#
#
# see also ../../doc/html/SW_layout_en.htm



#bitNr=`getconf LONG_BIT`
#export OS=Linux${bitNr}


if [ ! -n "$DIR_DEV" ]; then
# not set to server: set to local
export DIR_DEV=${HOME}/devel/
fi

if [ ! -n "$DIR_BIN" ]; then
# vi.sh
export DIR_BIN=${HOME}/devel/bin/
fi





export hTyp=`uname -s`_`uname -m`

# gcad_dir_dev      home of sourcefiles, scripts, linkjobs, packages
export gcad_dir_dev=${DIR_DEV}gcad3d/

# gcad_dir_bin      home of objects, executables, libraries
export gcad_dir_bin=${DIR_BIN}gcad3d/${hTyp}/



mkdir -p ${gcad_dir_bin}
mkdir -p ${gcad_dir_bin}plugins/
mkdir -p ${gcad_dir_bin}plugins/cut1/


echo "gcad_dir_dev=|"${gcad_dir_dev}"|"
echo "gcad_dir_bin=|"${gcad_dir_bin}"|"

# use it with ". ./options.sh"
#
