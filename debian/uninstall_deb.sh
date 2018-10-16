#!/bin/sh
# 
# unistall from system:
# sudo -E ../uninstall.sh


echo "gcad3d-uninstall.sh"


instDir=$1
echo "uninstall \""${instDir}\"






#-----------------------------------------------------
# startscript
rm -rf ${instDir}/usr/bin/gcad3d


# executable and dynLibs
rm -rf ${instDir}/usr/lib/gCAD3D


# examples.gz (dat,prg,ctlg,..) icons ..
rm -rf ${instDir}/usr/share/gcad3d


# icon for desktop-starter
rm -rf ${instDir}/usr/share/pixmaps/gcad3d.xpm


# /usr/share/doc/gcad3d
rm -rf ${instDir}/usr/share/doc/gcad3d


# menuFile
rm -rf ${instDir}/usr/share/menu/gcad3d




# EOF
