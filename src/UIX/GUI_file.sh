#!/bin/bash
# . ./devbase.sh && ./GUI_file.sh
#
# ../gui_gtk3/GUI_file.c
# ../gui_gtk2/GUI_file.c
# ../gui_gtk2_MS/GUI_file.c
# ./GUI_file.sh
# cat  /tmp/debug.dat
#

. ../UIX/devbase.sh

fnID=/tmp/temp_$RANDOM
fnIn=${fnID}_in
fnOut=${fnID}_out

echo "gcad_dir_bin = ${gcad_dir_bin}"
echo fnIn = ${fnIn}
echo fnOut = ${fnOut}



#############################################################################
# Parameters for open-function:  (get filename for later file-read)
#  1 "open"
#  2 start-directory for file-selection  (must have closing '\'
#  3 filename of symbolic-directories    -
#  4 filterText  (eg "*.dxf" or NONE (MSYS expands "*" only !!)
#  5 window-title                        -
#
# Output:
#   stdout     full selected filename; empty for Cancel;
#----------------------------------------------------------------------

function do_open () {
echo "function do_open .."

echo open > ${fnIn}
echo /mnt/serv2/devel/cadfiles/gcad/ >> ${fnIn}
echo /mnt/serv2/devel/gcad3d/test/cfg_Linux/dir.lst >> ${fnIn}
echo *.dxf >> ${fnIn}
echo Datei OPEN >> ${fnIn}
}





#############################################################################
# Parameters for save-function:   (get filename for later file-write)
#  "save"
#  outDir/outfilename
#  filename of symbolic-directories    -
#  filterText  (eg "*")                -
#  window-title                        -
#
# Output:
#   stdout     full selected filename; empty for Cancel;
#----------------------------------------------------------------------

function do_save () {
echo "function do_save .."

echo save > ${fnIn}
echo /mnt/serv2/devel/cadfiles/gcad/unknown.gcad >> ${fnIn}
echo /mnt/serv2/devel/gcad3d/test/cfg_Linux/dir.lst >> ${fnIn}
echo *.gcad >> ${fnIn}
echo Datei SAVE >> ${fnIn}
}





#############################################################################
#do_open
do_save


gdb -q --args ${gcad_dir_bin}/GUI_file_gtk2 ${fnIn}


echo "--------------- ${fnOut} -------------------------"
cat ${fnOut}
echo "-------------------------------------------------"


rm -f ${fnIn}
rm -f ${fnOut}

#type /tmp/debug.dat

# eof
