#!/bin/bash
# use with:      ./GUI_dlg1.sh

#par="info \"Directory does not exist\""


#par="dlgbe \" model exists; overwrite ? \" NO YES"

#par="dlgbe \"text info\" Cancel NO YES --ent \"entPreset\" 16"


par="list1 /mnt/serv2/devel/gcad3d/gCAD3D/tmp/MdlLst.txt \"select model\" \"x40,y30\""
#par="list1 /mnt/serv2/devel/gcad3d/gCAD3D/tmp/catParts.act \"sel.-\" \"x40,y30,a\""


########################################
. ../options.sh
fnmak=GUI_dlg1.mak
VGUI=`cat ../gcad_gui_version`
cmd=". ../options.sh && make -f ${fnmak} && ${gcad_dir_bin}GUI_dlg1_${VGUI} "

eval "${cmd} ${par}"

cat /tmp/debug.dat
########################################

# eof
