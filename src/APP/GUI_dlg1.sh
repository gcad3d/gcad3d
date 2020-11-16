#!/bin/bash
# Testprog for Dialog-program GUI_dlg1
# use with:
# set one of the <par> free;
# ./GUI_dlg1.sh
# TODO: test dlgbe with entry and 0 buttons ..




#########################################################
#par="info \"Directory does not exist\""

#par="info \"cannot create directory /xyz/tmp/. \\0 Fix gcad_dir_local in startup-script.\""

#########################################################
# dlgbe = dialog with 1 - n buttons
#par="dlgbe \" model exists; overwrite ? \" NO YES"

par="dlgbe \"text info\" Cancel NO YES --ent \"\" 16"

#par="dlgbe \"text info\" Cancel NO YES --ent \"entPreset\" 300"


#########################################################
# list1 = dialog displays list from file; returns ?
#par="list1 /mnt/serv2/devel/gcad3d/gCAD3D/tmp/MdlLst.txt \"select model\" \"x40,y30\""

#par="list1 /mnt/serv2/devel/gcad3d/gCAD3D/tmp/catParts.act \"sel.-\" \"x40,y30,a\""


########################################
########################################
. ../options.sh
fnmak=GUI_dlg1.mak
VGUI=`cat ../gcad_gui_version`
cmd=". ../options.sh && make -f ${fnmak} && ${gcad_dir_bin}GUI_dlg1_${VGUI} "

eval ${cmd} ${par}

cat /tmp/debug.dat
########################################

# eof
