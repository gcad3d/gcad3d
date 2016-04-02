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
# copy all files -> instDir (


echo "pack_deb.sh starting .."

# basDir: ~/devel/gcad3d
debDir=`pwd`
echo "debDir = " ${debDir}
cd ..
basDir=`pwd`
echo "basDir = " ${basDir}
# copy all files -> instDir = ~/devel/gcad3d/debian/gCAD3D/debian
instDir=${debDir}/gCAD3D/debian
echo "instDir = " ${instDir}
srcDir=${basDir}/src
echo "srcDir = " ${srcDir}
outDir=${basDir}/packages
echo "outDir = " ${outDir}



# get bitNr BINLOC gcad_dir_bin
. src/options.sh
echo "gcad_dir_bin = " ${gcad_dir_bin}



Version=`cat ${srcDir}/gcad_version`
echo "Version =" ${Version}


if [ "${bitNr}" = "32" ] ; then
platform="i386"
else
platform="amd64"
fi


packNam="gCAD3D-${Version}-bin-${platform}.deb"
echo "start create debian-package" ${packNam}


#-------------------------------------------------------
# create ~/devel/gcad3d/debian/examples.gz
cd ${debDir}
. ./pack_examples.sh
echo "examples.gz created .."


#------------------------------------------------------
# remove install-dir; rm -rf ~/devel/gcad3d/debian/gCAD3D/debian
rm -rf ${instDir}
/bin/mkdir -p ${instDir}


/bin/mkdir -p -m 755 ${instDir}/DEBIAN
/bin/mkdir -p -m 755 ${instDir}/usr/bin
/bin/mkdir -p -m 755 ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/plugins
/bin/mkdir -p -m 755 ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/plugins/cut1
/bin/mkdir -p -m 755 ${instDir}/usr/share/gcad3d/icons
/bin/mkdir -p -m 755 ${instDir}/usr/share/doc/gcad3d/html
/bin/mkdir -p -m 755 ${instDir}/usr/share/doc/gcad3d/msg
/bin/mkdir -p -m 755 ${instDir}/usr/share/menu
/bin/mkdir -p -m 755 ${instDir}/usr/share/pixmaps

#echo "instDir = " ${instDir}
#find ${instDir}


# copy ControlFiles
#../debian/gCAD3D/debian/DEBIAN/control,preinst,postinst,prerm
/bin/cp ${debDir}/_control/p* ${instDir}/DEBIAN/.



if [ "${bitNr}" = "32" ] ; then
cat ${debDir}/_control/control32 | sed "s/<VERSION>/$Version/" > ${instDir}/DEBIAN/control
else
# fix "Version" in ../debian/_control/control
cat ${debDir}/_control/control64 | sed "s/<VERSION>/$Version/" > ${instDir}/DEBIAN/control
fi



# startscript /usr/bin/gcad3d -> ../debian/gCAD3D/debian/usr/bin/gcad3d
/bin/cp ${debDir}/gcad3d ${instDir}/usr/bin/.



# executables  -> ../debian/gCAD3D/debian/usr/lib/gCAD3D/binLinux##
/bin/cp -f ${gcad_dir_bin}/gCAD3D         ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/.
/bin/cp -f ${gcad_dir_bin}/*.so           ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/.
/bin/cp -f ${gcad_dir_bin}/plugins/*.so   ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/plugins/.
/bin/cp -f ${gcad_dir_bin}/plugins/cut1/* ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/plugins/cut1/.

#strip ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/gCAD3D
#strip ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/*.so
#strip ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/plugins/*.so
#strip ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/plugins/cut1/*

chmod 0755 ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/gCAD3D
chmod 0755 ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/*.so
chmod 0755 ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/plugins/*.so
chmod 0755 ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/plugins/cut1/*



# examples.gz (dat,prg,ctlg,..) gCAD3D.desktop -> /usr/share/gcad3d/.
/bin/cp -f ${debDir}/examples.gz ${instDir}/usr/share/gcad3d/.
/bin/cp -f ${debDir}/gCAD3D.desktop ${instDir}/usr/share/gcad3d/.
chmod 0644 ${instDir}/usr/share/gcad3d/examples.gz
chmod 0644 ${instDir}/usr/share/gcad3d/gCAD3D.desktop



#/usr/share/gcad3d/icons/
#../debian/gCAD3D/debian/usr/share/gcad3d/icons
/bin/cp -f ${basDir}/icons/*.png   ${instDir}/usr/share/gcad3d/icons/.
/bin/cp -f ${basDir}/icons/*.xpm   ${instDir}/usr/share/gcad3d/icons/.
/bin/cp -f ${basDir}/icons/*.bmp   ${instDir}/usr/share/gcad3d/icons/.
chmod 0644 ${instDir}/usr/share/gcad3d/icons/*



#/usr/share/doc/gcad3d/
#../debian/gCAD3D/debian/usr/share/doc/gcad3d/changelog.gz,copyright,gCAD3D.ico,gCAD3D.desktop
#cp /usr/share/common-licenses/GPL-3 ${instDir}/usr/share/doc/gcad3d/copyright
/bin/cp ${basDir}/LICENSE           ${instDir}/usr/share/doc/gcad3d/copyright
#cp copyright            ${instDir}/usr/share/doc/gcad3d/.
/bin/cp ${debDir}/changelog            ${instDir}/usr/share/doc/gcad3d/.
gzip -f --best ${instDir}/usr/share/doc/gcad3d/changelog
chmod 0644 ${instDir}/usr/share/doc/gcad3d/copyright
chmod 0644 ${instDir}/usr/share/doc/gcad3d/changelog.gz



#/usr/share/doc/gcad3d/html/
/bin/cp -f ${basDir}/doc/html/*.htm  ${instDir}/usr/share/doc/gcad3d/html/.
/bin/cp -f ${basDir}/doc/html/*.png  ${instDir}/usr/share/doc/gcad3d/html/.
/bin/cp -f ${basDir}/doc/html/*.js   ${instDir}/usr/share/doc/gcad3d/html/.
chmod 0644 ${instDir}/usr/share/doc/gcad3d/html/*



#/usr/share/doc/gcad3d/msg/
/bin/cp -f ${basDir}/doc/msg/*.txt   ${instDir}/usr/share/doc/gcad3d/msg/.
chmod 0644 ${instDir}/usr/share/doc/gcad3d/msg/*



#/usr/share/menu/gcad3d
/bin/cp ${debDir}/_control/menu      ${instDir}/usr/share/menu/gcad3d
chmod 0644 ${instDir}/usr/share/menu/gcad3d


#/usr/share/pixmaps/gCAD3D.xpm
/bin/cp ${basDir}/icons/gCAD3D.xpm   ${instDir}/usr/share/pixmaps/.
chmod 0644 ${instDir}/usr/share/pixmaps/*



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
/usr/bin/strip ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/gCAD3D
/usr/bin/strip ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/*.so
/usr/bin/strip ${instDir}/usr/lib/gCAD3D/binLinux${bitNr}/plugins/*.so


#-----------------------------------------------------
# cd ~/devel/gcad3d/debian/gCAD3D
cd ${instDir}

cd ..
#
rm -f debian.deb
fakeroot dpkg-deb --build debian && lintian debian.deb
mv -f debian.deb ${outDir}/${packNam}

echo "${outDir}/${packNam} created."

rm -rf ${debDir}/examples.gz
rm -rf ${instDir}

exit
# eof
