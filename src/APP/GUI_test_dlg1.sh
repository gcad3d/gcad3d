#!/bin/bash


par="dlgbe \" model exists; overwrite ? \" NO YES"
#par="dlgbe \"text info\" Cancel NO YES --ent \"entPreset\" 16"







########################################
fnmak=GUI_dlg1.mak
VGUI=`cat ../gcad_gui_version`
cmd=". ../options.sh && make -f ${fnmak} && ${gcad_dir_bin}GUI_dlg1_${VGUI} "

eval "${cmd} ${par}"

cat /tmp/debug.dat
########################################

# eof
