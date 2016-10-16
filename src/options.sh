#!/bin/sh
# see also ../options.mak
# set gcad_dir_bin

# config 1: development;  src on server, bin local.
#           is active, if DIR_DEV and DIR_BIN are set 
# config 2: src local, bin local
#           unset DIR_DEV, DIR_BIN

bitNr=`getconf LONG_BIT`

export OS=Linux${bitNr}


if [ ! -n "$DIR_DEV" ]; then
# not set to server: set to local
DIR_DEV=${HOME}/devel/
fi

if [ ! -n "$DIR_BIN" ]; then
# vi.sh
DIR_BIN=${HOME}/devel/bin/
fi



gcad_dir_dev=${DIR_DEV}gcad3d/
# sources; local or server


export gcad_dir_bin=${gcad_dir_dev}gCAD3D/binLinux${bitNr}
# objs,exes,libs; local



# use it with ". ./options.sh"
#
