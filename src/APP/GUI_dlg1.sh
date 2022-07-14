#!/bin/bash
# Testprog for Dialog-program GUI_dlg1
# use with:
# set one of the <par> free;
# ./GUI_dlg1.sh
# TODO: dlgbe with entry and 0 buttons ..
# TODO: dlgbe without entry and 1 button ..

# ../gui_gtk3/GUI_dlg1.c
#!/bin/bash
# ./GUI_dlg1.sh
# cat  /tmp/debug.dat


fnID=/tmp/temp_$RANDOM



#########################################################
#par="info  \"Directory XX does not exist\" ${fnID}"

# infotext - max 3 lines, separated by "\n"
#par="info  \"Einzeiliger Text\" ${fnID}"
#par="info  \"Line1\nLine2\" ${fnID}"
#par="info  \"Line1\nLine2\n.. letzte Zeile\" ${fnID}"
par="info  \"Line1\nLine2\nLine3 \n- ERR: NOT DISPLAYED; ONLY 3 LINES !\""


#########################################################
# dlgbe = dialog with optional entry and 1 - n buttons
# returns index of selected button; first button returns "0".
# entrysize max 256

# info + 1 button
#par="dlgbe \" - infotext ..\" ok ${fnID}"

# info + 2 buttons
#par="dlgbe \" model exists; overwrite ? \" NO YES ${fnID}"

# info + 3 buttons
#par="dlgbe \"text info\" Cancel NO YES ${fnID}"

# entry only            TODO - NYI
# par="dlgbe \" --- enter name \" --ent \"\" 16 ${fnID}"

# info + entry + 2 buttons + entrySize
#par="dlgbe \"text info\" Cancel NO YES --ent \"\" 16 ${fnID}"

# info + entry + entryPreset + entrySize
#par="dlgbe \"text info\" Cancel NO YES --ent \"entPreset\" 256 ${fnID}"


#########################################################
# list1 = dialog displays list from file; returns selected text; "x<chrNr>,y<lNr>"
# Cancels returns empty output.

#par="list1 /tmp/temp.lst \" - select file -\" \"x40,y10\" ${fnID}"

#par="list1 /mnt/serv2/devel/gcad3d/gCAD3D/tmp/MdlLst.txt \"select model\" \"x40,y30\" ${fnID}"

#par="list1 /mnt/serv2/devel/gcad3d/gCAD3D/tmp/catParts.act \"sel.-\" \"x40,y30,a\" ${fnID}"


#########################################################
. ../options.sh
fnmak=GUI_dlg1.mak
VGUI=`cat ../gcad_gui_version`
cmd1=". ../options.sh && make -f ${fnmak} "
cmd2="${gcad_dir_bin}GUI_dlg1_${VGUI}"


#-----------------------------
# link
echo ${cmd1}
eval ${cmd1}
if [ $? != 0 ]; then
  echo "******  ERROR LINK ${fnmak}"
  exit $?
fi


#-----------------------------
# execute
echo ${cmd2}
echo ${par}
echo "----------------"
eval ${cmd2} ${par}
echo "----------------"


# no output from info
ss1=`echo ${par} | cut -c1-4`

if [ ${ss1} != "info" ]; then
echo "--------------- ${fnID} -------------------------"
cat ${fnID}
echo "-------------------------------------------------"
rm -f ${fnID}
fi

#cat /tmp/debug.dat
########################################


# debug:
# gdb /home/fwork/devel/bin/gcad3d/Linux_x86_64/GUI_dlg1_gtk2
# set args list1 /mnt/serv2/devel/gcad3d/gCAD3D/tmp/temp.lst "select file" "x40,y10"
# r


# eof
