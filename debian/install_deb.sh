#!/bin/sh
# install debian-package
# parameter 1 = root for installation-files; none: system-install.
# Using symbols DIR_DEV and DIR_BIN. See ../../doc/html/SW_layout_en.htm


echo "- entering gcad3d-install_deb.sh"


bitNr=`getconf LONG_BIT`
hTyp=`uname -s`_`uname -m`
echo "hTyp = " ${hTyp}   "bitNr = " ${bitNr}



echo "gcad_dir_bas = " ${gcad_dir_bas}
cd ${gcad_dir_bas}


debDir=${gcad_dir_bas}debian
echo "debDir = " ${debDir}


gcad_dir_dev=${gcad_dir_bas}
echo "gcad_dir_dev = " ${gcad_dir_dev}


echo "gcad_dir_bin = " ${gcad_dir_bin}


instDir=$1
echo "install -> \""${instDir}\"






#-----------------------------------------------------
# copy startscript /usr/bin/gcad3d -> /usr/bin/gcad3d
/bin/mkdir -p -m 755 ${instDir}/usr/bin

/bin/cp ${gcad_dir_bas}src/gcad3d ${instDir}/usr/bin/.

chmod 0755 ${instDir}/usr/bin/gcad3d





#-----------------------------------------------------
# copy executable and dynLibs -> /usr/lib/gCAD3D/binLinux32|64
/bin/mkdir -p -m 755 ${instDir}/usr/lib/gcad3d
/bin/mkdir -p -m 755 ${instDir}/usr/lib/gcad3d/${hTyp}
/bin/mkdir -p -m 755 ${instDir}/usr/lib/gcad3d/${hTyp}/plugins
/bin/mkdir -p -m 755 ${instDir}/usr/lib/gcad3d/${hTyp}/plugins/cut1

/bin/cp -f ${gcad_dir_bin}gCAD3D         ${instDir}/usr/lib/gcad3d/${hTyp}/.
/bin/cp -f ${gcad_dir_bin}*.so           ${instDir}/usr/lib/gcad3d/${hTyp}/.
/bin/cp -f ${gcad_dir_bin}GUI_*          ${instDir}/usr/lib/gcad3d/${hTyp}/.
/bin/cp -f ${gcad_dir_bin}gcad3d_gMsh    ${instDir}/usr/lib/gcad3d/${hTyp}/.
/bin/cp -f ${gcad_dir_bin}plugins/*.so   ${instDir}/usr/lib/gcad3d/${hTyp}/plugins/.
/bin/cp -f ${gcad_dir_bin}plugins/cut1/* ${instDir}/usr/lib/gcad3d/${hTyp}/plugins/cut1/.


chmod 0755 ${instDir}/usr/lib/gcad3d/${hTyp}/gCAD3D
chmod 0755 ${instDir}/usr/lib/gcad3d/${hTyp}/*.so
chmod 0755 ${instDir}/usr/lib/gcad3d/${hTyp}/GUI_*
chmod 0755 ${instDir}/usr/lib/gcad3d/${hTyp}/gcad3d_gMsh
chmod 0755 ${instDir}/usr/lib/gcad3d/${hTyp}/plugins/*.so
chmod 0755 ${instDir}/usr/lib/gcad3d/${hTyp}/plugins/cut1/*





#-----------------------------------------------------
# copy examples.gz (dat,prg,ctlg,..) -> /usr/share/gcad3d/.
/bin/mkdir -p -m 755 ${instDir}/usr/share/gcad3d/
/bin/mkdir -p -m 755 ${instDir}/usr/share/gcad3d/icons
/bin/mkdir -p -m 755 ${instDir}/usr/share/gcad3d/doc
/bin/mkdir -p -m 755 ${instDir}/usr/share/gcad3d/doc/html
/bin/mkdir -p -m 755 ${instDir}/usr/share/gcad3d/doc/msg

/bin/cp -f ${gcad_dir_bas}src/gcad3d.desktop   ${instDir}/usr/share/gcad3d/.
/bin/cp -f ${gcad_dir_bas}examples.gz          ${instDir}/usr/share/gcad3d/.
/bin/cp -f ${gcad_dir_bas}icons/*.png          ${instDir}/usr/share/gcad3d/icons/.
/bin/cp -f ${gcad_dir_bas}icons/*.xpm          ${instDir}/usr/share/gcad3d/icons/.
/bin/cp -f ${gcad_dir_bas}icons/*.bmp          ${instDir}/usr/share/gcad3d/icons/.
/bin/cp -f ${gcad_dir_bas}doc/*.txt            ${instDir}/usr/share/gcad3d/doc/.
/bin/cp -f ${gcad_dir_bas}doc/html/*.htm       ${instDir}/usr/share/gcad3d/doc/html/.
/bin/cp -f ${gcad_dir_bas}doc/html/*.png       ${instDir}/usr/share/gcad3d/doc/html/.
/bin/cp -f ${gcad_dir_bas}doc/html/*.js        ${instDir}/usr/share/gcad3d/doc/html/.
/bin/cp -f ${gcad_dir_bas}doc/msg/*.txt        ${instDir}/usr/share/gcad3d/doc/msg/.

chmod 0644 ${instDir}/usr/share/gcad3d/gcad3d.desktop
chmod 0644 ${instDir}/usr/share/gcad3d/examples.gz
chmod 0644 ${instDir}/usr/share/gcad3d/icons/*
chmod 0644 ${instDir}/usr/share/gcad3d/doc/html/*
chmod 0644 ${instDir}/usr/share/gcad3d/doc/msg/*






#-----------------------------------------------------
# copy icon for desktop-starter -> /usr/share/pixmaps/gCAD3D.xpm
/bin/mkdir -p -m 755 ${instDir}/usr/share/pixmaps

/bin/cp ${gcad_dir_bas}/icons/gCAD3D.xpm   ${instDir}/usr/share/pixmaps/gcad3d.xpm

chmod 0644 ${instDir}/usr/share/pixmaps/*






#-----------------------------------------------------
#copy -> /usr/share/doc/gcad3d/
/bin/mkdir -p -m 755 ${instDir}/usr/share/doc/gcad3d

/bin/cp -f ${gcad_dir_bas}/LICENSE          ${instDir}/usr/share/doc/gcad3d/copyright
/bin/cp -f ${debDir}/changelog              ${instDir}/usr/share/doc/gcad3d/.
/bin/cp -f ${gcad_dir_bas}/doc/gCAD3D_log.txt ${instDir}/usr/share/doc/gcad3d/NEWS
/bin/cp -f ${gcad_dir_bas}/README.md        ${instDir}/usr/share/doc/gcad3d/.

gzip -f --best ${instDir}/usr/share/doc/gcad3d/changelog
gzip -f --best ${instDir}/usr/share/doc/gcad3d/NEWS
gzip -f --best ${instDir}/usr/share/doc/gcad3d/README.md


chmod 0644 ${instDir}/usr/share/doc/gcad3d/copyright
chmod 0644 ${instDir}/usr/share/doc/gcad3d/changelog.gz
chmod 0644 ${instDir}/usr/share/doc/gcad3d/NEWS.gz
chmod 0644 ${instDir}/usr/share/doc/gcad3d/README.md.gz






#-----------------------------------------------------
# copy menuFile -> /usr/share/menu/gcad3d
/bin/mkdir -p -m 755 ${instDir}/usr/share/menu

/bin/cp ${debDir}/_control/menu      ${instDir}/usr/share/menu/gcad3d

chmod 0644 ${instDir}/usr/share/menu/gcad3d

echo "- leaving install_deb.sh"

# EOF
