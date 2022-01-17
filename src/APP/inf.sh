#! /bin/sh

# find infos in infofiles ../inf/*
# script for cscope
# use with "Find this text string:"

# get DIR_DEV DIR_BIN gcad_dir_bin
. ../options.sh

# set EDITOR = name of script-file with editor-call
#EDITOR="${DIR_DEV}gcad3d/src/vi.sh"
EDITOR=vim
export EDITOR

# get OS (eg "Linux32")
#bitNr=`getconf LONG_BIT`
#OS=Linux${bitNr}
#echo ${OS}

# create list of info-files in file infFiles
ls ../inf/* > infFiles

cscope -i infFiles
# start; -p2 = 2 directory-levels
#cscope -p2 -i srcFiles

exit
#
