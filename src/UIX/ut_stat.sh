#! /bin/bash
#set verbose

# gCAD3D-Statistik; Anzahl Zeilen, Anzahl Funktionen.

. ./devbase.sh

# bits; 32|64
osVer=`getconf LONG_BIT`
#gcad_dir_bin=../../binLinux${osVer}


# Liste aller KernSourcefiles -> srcFiles
make -f gcad3d.mak srclst
mv -f srcFiles srcFilesCad


# Liste aller GUI-Sourcefiles -> srcFiles
make -f gcad_gui_lnk.mak srclst
mv -f srcFiles srcFilesGui


## Files in ../gui_gtk2:
#set gtk2Tab = `ls ../gui_gtk2/gtk_*.c ../gui_gtk2/gtk_*.h`
#echo "=========== guiTab: ====================="
#echo $gtk2Tab

## Files in ../gui_gtk3:
#set gtk3Tab = `ls ../gui_gtk3/gtk_*.c ../gui_gtk3/gtk_*.h`
#echo "=========== guiTab: ====================="
#echo $gtk3Tab

#set dllTab = `find . -maxdepth 1 -name "xa_*.mak" -o -name "PRC_*.mak" -exec make -f {} "OS=Linux32" srclst \;`
#dllTab=`find . -maxdepth 1 -name "xa_*.mak" -exec make -f {} srclst \;`
#echo "=========== dllTab: ====================="
#echo $dllTab

#dll1Tab=`find . -maxdepth 1 -name "PRC_*.mak" -exec make -f {} srclst \;`
#echo "=========== dll1Tab: ====================="
#echo $dll1Tab

#set appTab = `find . -name "APP*.mak" -exec make srclst -f {} \;`


#-----------------------------------------------------
echo "======================================="
wc -l `cat srcFilesCad` `cat srcFilesGui`

echo " = Anzahl Programmzeilen"

#-----------------------------------------------------
# get nr of functions
rm -rf /tmp/t2

echo ${gcad_dir_bin}
./ut1_stat.sh ${gcad_dir_bin}gCAD3D

find ${gcad_dir_bin} -maxdepth 1 -name "xa_*.so" -exec ./ut1_stat.sh {} \;


wc -l /tmp/t2
echo " = Anzahl Funktionen (siehe /tmp/t2)"



# eof
