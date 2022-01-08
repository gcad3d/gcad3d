#!/bin/bash
# Testprog for Dialog-program GUI_dlg1
# use with:
# set one of the <par> free;
# ./GUI_dlg1.sh
# TODO: dlgbe with entry and 0 buttons ..
# TODO: dlgbe without entry and 1 button ..

# ../gui_gtk3/GUI_dlg1.c
# ./GUI_dlg1.sh
# cat  /tmp/debug.dat


#########################################################
#par="info \"Directory does not exist\""
# every string in new line; max 4 lines.

par="info \"ERROR\" \"cannot create directory /xyz/tmp/.\" \" Fix gcad_dir_local in startup-script.\""

#########################################################
# dlgbe = dialog with 1 - n buttons
#par="dlgbe \" model exists; overwrite ? \" NO YES"

#par="dlgbe \" - infotext ..\" ok"                        TODO - NYI
#par="dlgbe \" - infotext ..\" YES no"
#par="dlgbe \"text info\" Cancel NO YES"
# par="dlgbe \" --- enter name \" --ent \"\" 16"                  TODO - NYI
#par="dlgbe \"text info\" Cancel NO YES --ent \"\" 16"
#par="dlgbe \"text info\" Cancel NO YES --ent \"entPreset\" 300"


#########################################################
# list1 = dialog displays list from file; returns selected text
#par="list1 /mnt/serv2/devel/gcad3d/gCAD3D/tmp/MdlLst.txt \"select model\" \"x40,y30\""

#par="list1 /mnt/serv2/devel/gcad3d/gCAD3D/tmp/catParts.act \"sel.-\" \"x40,y30,a\""


########################################
########################################
. ../options.sh
fnmak=GUI_dlg1.mak
VGUI=`cat ../gcad_gui_version`
cmd=". ../options.sh && make -f ${fnmak} && ${gcad_dir_bin}GUI_dlg1_${VGUI} "

echo ${cmd}
echo ${par}

eval ${cmd} ${par}

cat /tmp/debug.dat
########################################

# eof
