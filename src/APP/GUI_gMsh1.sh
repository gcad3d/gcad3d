#!/bin/bash
# Testprog for Dialog-program GUI_gMsh1 (link and start)
# use with:
# ./GUI_gMsh1.sh
# ../gui_gtk2/GUI_gMsh1.c
# ../gui_gtk2_MS/GUI_gMsh1.c
# cat  /tmp/debug.dat
#


#########################################################
# "directory-dataFiles" "outfilename"
par="/mnt/serv2/devel/cadfiles/gcad/gMsh/ sample_0.gmsh1" 


########################################
########################################
. ../options.sh
fnmak=GUI_gMsh1.mak

#VGUI=`cat ../gcad_gui_version`
VGUI=gtk2

cmd=". ../options.sh && make -f ${fnmak} && ${gcad_dir_bin}GUI_gMsh1_${VGUI} "

# debug:
#cmd=". ../options.sh && gdb ${gcad_dir_bin}GUI_gMsh1_${VGUI} "
# - run /mnt/serv2/devel/cadfiles/gcad/gMsh/

echo ${cmd}
echo ${par}

eval ${cmd} ${par}

# debug-data
cat /tmp/debug.dat

########################################
# eof
