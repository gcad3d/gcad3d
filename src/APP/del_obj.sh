#!/bin/bash
#
# delete all gcad-objs (*.o)


# get bitNr BINLOC gcad_dir_bin DirBasSer
. ../options.sh


echo "delete all gcad-objs (*.o)"

#if test "$OSTYPE" = "linux-gnu"; then

rm -f ${gcad_dir_bin}/*.o

# PRC_cut1.mak
rm -f ../prc/cut1/*.o

#elif test "$OSTYPE" = "msys"; then
#  echo "binMS32 OS = `uname`"
#    rm -f ../binMS32/*.o
#
#fi

# eof
