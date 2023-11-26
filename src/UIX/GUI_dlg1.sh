#!/bin/bash
# . ./devbase.sh && ./GUI_dlg1.sh

# Testprog for Dialog-program GUI_dlg1
# use with:
# set one of the <par> free;
# ./GUI_dlg1.sh
# TODO: dlgbe with entry and 0 buttons ..
# TODO: dlgbe without entry and 1 button ..



fnID=/tmp/temp_$RANDOM
fnIn=${fnID}_in
fnOut=${fnID}_out

echo "DIR_EXE = ${DIR_EXE}"
echo fnIn = ${fnIn}



#########################################################
# info  - infotext with 1, max 3 lines;
# lineSeparator = "\n";
#########################################################


function do_info () {
echo "function do_info .."

echo info > ${fnIn}
#echo Einzeiliger Text >>  ${fnIn}
echo Line1 abc\\nLine2 xyz >>  ${fnIn}
#echo Line1 abc\\nLine2 xyz\\n - letzte Zeile >>  ${fnIn}

}





#########################################################
# dlgbe = dialog with infotext, 1 - n buttons, optional entry;
#   <infoText>         1. line
#   <buttonText>       1- 9 buttons
#   --ent              indicates following entry-parameters
#   <presetText>
#   <entrysize>        max 256 chars
# Output: first char = index of selected button; first button returns "0";
#   following chars are content of inputfield.
#########################################################

function do_dlgbe () {
echo "function do_dlgbe .."

echo dlgbe > ${fnIn}

## info + 1 button
# infotext 
echo - infotext .. >>  ${fnIn}
# buttons
echo OK >>  ${fnIn}
#echo NO >>  ${fnID}
echo Cancel >>  ${fnIn}
# entry
echo --ent >>  ${fnIn}
echo preset >>  ${fnIn}
echo 200 >>  ${fnIn}

}


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
# list1                 dialog displays list from file; returns selected text;
# <fileNam>             file to display; every line can be selected;
# infoText              windowTitle
# x<nrChr>,<nrLines>    nr of characters per line and nr of lines to view
#
# Output: text of selected line
# Cancels returns empty output.
#########################################################


function do_list1 () {
echo "function do_list1 .."

echo list1 > ${fnIn}
echo ../../test/cfg_Linux/dir.lst >> ${fnIn}
echo - select file - >> ${fnIn}
echo x40,y20 >> ${fnIn}

}


#par="list1 /tmp/temp.lst \" - select file -\" \"x40,y10\" ${fnID}"
#par="list1 /mnt/serv2/devel/gcad3d/test/tmp/MdlLst.txt \"select model\" \"x40,y30\" ${fnID}"
#par="list1 /mnt/serv2/devel/gcad3d/test/tmp/catParts.act \"sel.-\" \"x40,y30,a\" ${fnID}"




#########################################################
#do_info 
#do_dlgbe
do_list1


gdb -q --args ${DIR_EXE}/GUI_dlg1_gtk2 ${fnIn}

echo "---out : --------------------------------"
cat ${fnOut}
echo "-----------------------------------------"


rm -f ${fnIn}
rm -f ${fnOut}

type /tmp/debug.dat

# eof
