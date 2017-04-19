#!/bin/sh
# set gcad_dir_dev and gcad_dir_bin 
# must be set before gcad3d.mak - export gcad_dir_dev and gcad_dir_bin or:
# . ../options.sh
#
# gcad_dir_dev: src/, icons/, doc/, README ..
# gcad_dir_bin: *.o, *.so, gCAD3D, plugins/*.so
#
# config 1: development;  src on server, bin local.
#           is active, if DIR_DEV and DIR_BIN are set 
#
# config 2: src local, bin local
#           unset DIR_DEV, DIR_BIN

# see also ../options.mak



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


# gcad_dir_bin = objs,exes,libs -
# gcad_dir_bin on server: 
#export gcad_dir_bin=${gcad_dir_dev}gCAD3D/binLinux${bitNr}

# gcad_dir_bin on SSD:
export gcad_dir_bin=${HOME}/devel/binLinux${bitNr}

# use it with ". ./options.sh"
#
