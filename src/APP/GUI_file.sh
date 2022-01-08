#!/bin/bash
# Testprog for Dialog-program GUI_file
# use with:      ./GUI_file.sh
#
# ../gui_gtk3/GUI_file.c
# ../gui_gtk2/GUI_file.c
# ../gui_gtk2_MS/GUI_file.c
# ./GUI_file.sh
# cat  /tmp/debug.dat
#
#----------------------------------------------------------------------
# Parameters for open-function:  (get filename for later file-read)
#  "open"
#  start-directory for file-selection  -
#  filename of symbolic-directories    -
#  filterText  (eg "*")                -
#  window-title                        -
#
# Output:
#   stdout     full selected filename; empty for Cancel;
#
#----------------------------------------------------------------------
# Parameters for save-function:   (get filename for later file-write)
#  "save"
#  outDir/outfilename
#  filename of symbolic-directories    -
#  filterText  (eg "*")                -
#  window-title                        -
#
# Output:
#   stdout     full selected filename; empty for Cancel;
#
#
#----------------------------------------------------------------------



par="open /mnt/serv2/devel/cadfiles/gcad/ /mnt/serv2/devel/gcad3d/gCAD3D/cfg_Linux/dir.lst '*' 'Datei Öffnen'"


#par="save /mnt/serv2/devel/cadfiles/gcad/unknown.gcad /mnt/serv2/devel/gcad3d/gCAD3D/cfg_Linux/dir.lst '*' 'Speichern'"


echo "par = |"${par}"|"

########################################
. ../options.sh
fnmak=GUI_file.mak
VGUI=`cat ../gcad_gui_version`
cmd=". ../options.sh && make -f ${fnmak} && ${gcad_dir_bin}GUI_file_${VGUI} "

echo "cmd = |"${cmd}"|"

eval "${cmd} ${par}"

cat /tmp/debug.dat
########################################

# eof
