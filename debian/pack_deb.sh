#!/bin/sh

# create debian-package                         ./pack_deb.sh
# start in <basDir>/gcad3d/debian
#
# remove gcad:       sudo apt-get remove gcad3d
# install gcad-32    sudo dpkg -i /mnt/serv1/gCAD3D-1.80-Linux-x86.deb
# install gcad-64    sudo dpkg -i /mnt/serv1/gCAD3D-1.80-Linux64.deb
# start gcad:        gcad3d
#
# check Installed-Size (_control/control)
# du -c /usr/lib/gCAD3D /usr/share/gcad3d /usr/share/doc/gcad3d
#
#
# Local/HDEV3:
# cd ../../debian
# ./pack_deb.sh
## creates /mnt/serv1/Devel/gcad3d/packages/gCAD3D-#.##-bin-amd64.deb
## instDir =  /mnt/serv1/Devel/gcad3d/debian/gCAD3D/debian
#
# Software-Layout see <basDir>/gcad3d/doc/html/SW_layout_en.htm
#

echo "pack_deb.sh starting .."




basDir=${DIR_DEV}gcad3d
cd ${basDir}

# get gcad_dir_bin
. ${DIR_DEV}gcad3d/src/options.sh
echo "gcad_dir_bin = " ${gcad_dir_bin}

srcDir=${DIR_DEV}gcad3d/src
echo "srcDir = " ${srcDir}

debDir=${DIR_DEV}gcad3d/debian
echo "debDir = " ${debDir}

# copy all files -> instDir = ${DIR_DEV}gcad3d/debian/gCAD3D/debian
instDir=${DIR_DEV}gcad3d/debian/gCAD3D/debian
echo "instDir = " ${instDir}

outDir=${DIR_DEV}gcad3d/packages
echo "outDir = " ${outDir}



Version=`cat ${srcDir}/gcad_version`
echo "Version =" ${Version}


bitNr=`getconf LONG_BIT`
mach=`uname -m`

if [ "${mach}" = "armv7l" ] ; then
# raspi
 platform="armhf"
elif [ "${bitNr}" = "32" ] ; then
 platform="i386"
else
 platform="amd64"
fi


packNam="gCAD3D-${Version}-bin-${platform}.deb"
echo "start create debian-package" ${packNam}


#------------------------------------------------------
# remove install-dir; rm -rf ~/devel/gcad3d/debian/gCAD3D/debian
rm -rf ${instDir}
/bin/mkdir -p ${instDir}

# create install-directory with all subDirectories
/bin/mkdir -p -m 755 ${instDir}/DEBIAN
/bin/mkdir -p -m 755 ${instDir}/usr/bin
/bin/mkdir -p -m 755 ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/plugins
/bin/mkdir -p -m 755 ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/plugins/cut1
/bin/mkdir -p -m 755 ${instDir}/usr/share/gcad3d/icons
/bin/mkdir -p -m 755 ${instDir}/usr/share/doc/gcad3d/html
/bin/mkdir -p -m 755 ${instDir}/usr/share/doc/gcad3d/msg
/bin/mkdir -p -m 755 ${instDir}/usr/share/menu
/bin/mkdir -p -m 755 ${instDir}/usr/share/pixmaps


# copy debian-ControlFiles -> ${instDir}/DEBIAN/.
#   ../debian/gCAD3D/debian/DEBIAN/control,preinst,postinst,prerm
/bin/cp ${debDir}/_control/p* ${instDir}/DEBIAN/.


# ${instDir}/DEBIAN/control - select debian-controlfile for active platform
# and fix "Version"
cat ${debDir}/_control/control_${platform} | sed "s/<VERSION>/$Version/" > ${instDir}/DEBIAN/control


# copy startscript /usr/bin/gcad3d -> /usr/bin/gcad3d
/bin/cp ${debDir}/gcad3d ${instDir}/usr/bin/.


# copy executable and dynLibs -> /usr/lib/gCAD3D/binLinux32|64
/bin/cp -f ${gcad_dir_bin}/gCAD3D         ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/.
/bin/cp -f ${gcad_dir_bin}/*.so           ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/.
/bin/cp -f ${gcad_dir_bin}/plugins/*.so   ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/plugins/.
/bin/cp -f ${gcad_dir_bin}/plugins/cut1/* ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/plugins/cut1/.
chmod 0755 ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/gCAD3D
chmod 0755 ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/*.so
chmod 0755 ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/plugins/*.so
chmod 0755 ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/plugins/cut1/*



# copy examples.gz (dat,prg,ctlg,..) -> /usr/share/gcad3d/.
/bin/cp -f ${gcad_dir_dev}packages/examples.gz ${instDir}/usr/share/gcad3d/.
chmod 0644 ${instDir}/usr/share/gcad3d/examples.gz


# copy desktop-starter -> /usr/share/applications/gcad3d.desktop
/bin/cp -f ${gcad_dir_dev}src/gcad3d.desktop ${instDir}/usr/share/gcad3d/.
chmod 0644 ${instDir}/usr/share/gcad3d/gcad3d.desktop

# copy icon for desktop-starter -> /usr/share/pixmaps/gCAD3D.xpm
/bin/cp ${basDir}/icons/gCAD3D.xpm   ${instDir}/usr/share/pixmaps/gcad3d.xpm
chmod 0644 ${instDir}/usr/share/pixmaps/*



# copy icons -> /usr/share/gcad3d/icons/
#../debian/gCAD3D/debian/usr/share/gcad3d/icons
/bin/cp -f ${basDir}/icons/*.png    ${instDir}/usr/share/gcad3d/icons/.
/bin/cp -f ${basDir}/icons/*.xpm    ${instDir}/usr/share/gcad3d/icons/.
/bin/cp -f ${basDir}/icons/*.bmp    ${instDir}/usr/share/gcad3d/icons/.
chmod 0644 ${instDir}/usr/share/gcad3d/icons/*



#copy -> /usr/share/doc/gcad3d/
#../debian/gCAD3D/debian/usr/share/doc/gcad3d/changelog.gz,copyright,gCAD3D.ico,gcad3d.desktop
#cp /usr/share/common-licenses/GPL-3 ${instDir}/usr/share/doc/gcad3d/copyright
/bin/cp ${basDir}/LICENSE           ${instDir}/usr/share/doc/gcad3d/copyright
/bin/cp ${debDir}/changelog         ${instDir}/usr/share/doc/gcad3d/.
gzip -f --best ${instDir}/usr/share/doc/gcad3d/changelog
chmod 0644 ${instDir}/usr/share/doc/gcad3d/copyright
chmod 0644 ${instDir}/usr/share/doc/gcad3d/changelog.gz



# copy Docu -> /usr/share/doc/gcad3d/html/
/bin/cp -f ${basDir}/doc/html/*.htm  ${instDir}/usr/share/doc/gcad3d/html/.
/bin/cp -f ${basDir}/doc/html/*.png  ${instDir}/usr/share/doc/gcad3d/html/.
/bin/cp -f ${basDir}/doc/html/*.js   ${instDir}/usr/share/doc/gcad3d/html/.
chmod 0644 ${instDir}/usr/share/doc/gcad3d/html/*



# copy programMessagefiles -> /usr/share/doc/gcad3d/msg/
/bin/cp -f ${basDir}/doc/msg/*.txt   ${instDir}/usr/share/doc/gcad3d/msg/.
chmod 0644 ${instDir}/usr/share/doc/gcad3d/msg/*



# copy menuFile -> /usr/share/menu/gcad3d
/bin/cp ${debDir}/_control/menu      ${instDir}/usr/share/menu/gcad3d
chmod 0644 ${instDir}/usr/share/menu/gcad3d




chmod 0755 ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}
chmod 0755 ${instDir}/usr/lib/gCAD3D
chmod 0755 ${instDir}/usr/lib

chmod 0755 ${instDir}/usr/share/doc/gcad3d/html
chmod 0755 ${instDir}/usr/share/doc/gcad3d/msg
chmod 0755 ${instDir}/usr/share/doc/gcad3d
chmod 0755 ${instDir}/usr/share/doc

chmod 0755 ${instDir}/usr/share/gcad3d
chmod 0755 ${instDir}/usr/share/menu
chmod 0755 ${instDir}/usr/share/pixmaps
chmod 0755 ${instDir}/usr/share

chmod 0755 ${instDir}/usr


#-----------------------------------------------------
# strip exe and so-files
#/usr/bin/strip ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/gCAD3D
#/usr/bin/strip ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/*.so
#/usr/bin/strip ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/plugins/*.so
#/usr/bin/strip ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/plugins/cut1/*


#-----------------------------------------------------
# cd ~/devel/gcad3d/debian/gCAD3D
cd ${instDir}

cd ..
#
rm -f debian.deb
# build
fakeroot dpkg-deb --build debian && lintian debian.deb
mv -f debian.deb ${outDir}/${packNam}

echo "${outDir}/${packNam} created."

rm -rf ${instDir}

exit
# eof
