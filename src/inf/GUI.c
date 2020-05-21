/* ../inf/GUI.c

================================================================== */
void INF_GUI_dlg1(){                   /*! \code

=============================================================================
Exexcutable file for gtk2 and gtk3 for info-box, dialog-box, selection-list.

- Format: exename function parameters
  - exename: /home/fwork/devel/bin/gcad3d/Linux_x86_64/GUI_dlg1_gtk3
  - function: info|list1|dlgbe



------------- info ----------------------------------
// infobox
info "Directory does not exist"


------------- dlgbe ----------------------------------
// dialog with buttons; example displays 3 buttons "Cancel", "NO" and "YES".
// returns nr of selected button; 0=first; cancel window returns NULL.
dlgbe "text" Cancel NO YES --ent "entPreset" 16



------------- list1 ----------------------------------
// select from list;
// 1.parameter: filename of options
// 2.parameter: title
// 3.parameter: options; windowsize in characters/lines, 'a' for complete-line
//              else only first word of line
// returns selected linetext; cancel window returns NULL.

list1 /mnt/serv2/devel/gcad3d/gCAD3D/tmp/MdlLst.txt "select model" "x40,y30,a"

list1 /mnt/serv2/devel/gcad3d/gCAD3D/cfg/dir.lst title "x20,y16"




=============================================================================



================================================================== \endcode */}
// eof
