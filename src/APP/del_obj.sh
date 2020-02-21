#!/bin/bash
#
# delete all gcad-objs (*.o)


# get bitNr BINLOC gcad_dir_bin DirBasSer
. ../options.sh


echo "delete all gcad-objs (*.o)"

#if test "$OSTYPE" = "linux-gnu"; then

# del. exe
rm -f ${gcad_dir_bin}/gCAD3D

# del all dll's
rm -f ${gcad_dir_bin}/xa*.so

# del all obj's
rm -f ${gcad_dir_bin}/*.o

# remove GUI_* executables
rm -f ${gcad_dir_bin}/GUI_*

# PRC_cut1.mak
rm -f ../prc/cut1/*.o

#elif test "$OSTYPE" = "msys"; then
#  echo "binMS32 OS = `uname`"
#    rm -f ../binMS32/*.o
#
#fi

# eof
