#!/bin/csh

# create debian-package                         ./pack_deb.sh
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



# debDir: ~/devel/gcad3d/debian
set debDir=`pwd`
# copy all files -> instDir = ~/devel/gcad3d/debian/gCAD3D/debian
set instDir = ${debDir}/gCAD3D/debian
set srcDir=../src

set bits=`getconf LONG_BIT`
set gcad_dir_bin=../binLinux${bits}

set Version=`cat ${srcDir}/gcad_version`

if ("${bits}" == "32") then
set platform = "i386"
else
set platform = "amd64"
endif

set packNam = "gCAD3D-${Version}-bin-${platform}.deb"

echo "start create debian-package" ${packNam}


#-------------------------------------------------------
# create ~/devel/gcad3d/debian/examples.gz
./pack_examples.sh
echo "examples.gz created .."



#-------------------------------------------------------
# remove install-dir; rm -rf ~/devel/gcad3d/debian/gCAD3D/debian
rm -rf ${instDir}
mkdir -p ${instDir}


mkdir -p -m 755 ${instDir}/DEBIAN
mkdir -p -m 755 ${instDir}/usr/bin
mkdir -p -m 755 ${instDir}/usr/lib/gCAD3D/binLinux${bits}/plugins
mkdir -p -m 755 ${instDir}/usr/lib/gCAD3D/binLinux${bits}/plugins/cut1
mkdir -p -m 755 ${instDir}/usr/share/gcad3d/icons
mkdir -p -m 755 ${instDir}/usr/share/doc/gcad3d/html
mkdir -p -m 755 ${instDir}/usr/share/doc/gcad3d/msg
mkdir -p -m 755 ${instDir}/usr/share/menu



# copy ControlFiles
#../debian/gCAD3D/debian/DEBIAN/control,preinst,postinst,prerm
cp _control/p* ${instDir}/DEBIAN/.



if ("${bits}" == "32") then
cat _control/control32 | sed "s/<VERSION>/$Version/" > ${instDir}/DEBIAN/control
else
# fix "Version" in ../debian/_control/control
cat _control/control64 | sed "s/<VERSION>/$Version/" > ${instDir}/DEBIAN/control
endif



# startscript /usr/bin/gcad3d -> ../debian/gCAD3D/debian/usr/bin/gcad3d
cp gcad3d ${instDir}/usr/bin/.



# executables  -> ../debian/gCAD3D/debian/usr/lib/gCAD3D/binLinux##
cp -f ${gcad_dir_bin}/gCAD3D         ${instDir}/usr/lib/gCAD3D/binLinux${bits}/.
cp -f ${gcad_dir_bin}/*.so           ${instDir}/usr/lib/gCAD3D/binLinux${bits}/.
cp -f ${gcad_dir_bin}/plugins/*.so   ${instDir}/usr/lib/gCAD3D/binLinux${bits}/plugins/.
cp -f ${gcad_dir_bin}/plugins/cut1/* ${instDir}/usr/lib/gCAD3D/binLinux${bits}/plugins/cut1/.

strip ${instDir}/usr/lib/gCAD3D/binLinux${bits}/gCAD3D
strip ${instDir}/usr/lib/gCAD3D/binLinux${bits}/*.so
strip ${instDir}/usr/lib/gCAD3D/binLinux${bits}/plugins/*.so
strip ${instDir}/usr/lib/gCAD3D/binLinux${bits}/plugins/cut1/*

chmod 0755 ${instDir}/usr/lib/gCAD3D/binLinux${bits}/gCAD3D
chmod 0755 ${instDir}/usr/lib/gCAD3D/binLinux${bits}/*.so
chmod 0755 ${instDir}/usr/lib/gCAD3D/binLinux${bits}/plugins/*.so
chmod 0755 ${instDir}/usr/lib/gCAD3D/binLinux${bits}/plugins/cut1/*



# examples.gz (dat,prg,ctlg,..) gCAD3D.desktop -> /usr/share/gcad3d/.
cp -f examples.gz ${instDir}/usr/share/gcad3d/.
cp -f gCAD3D.desktop ${instDir}/usr/share/gcad3d/.
chmod 0644 ${instDir}/usr/share/gcad3d/examples.gz
chmod 0644 ${instDir}/usr/share/gcad3d/gCAD3D.desktop



#/usr/share/gcad3d/icons/
#../debian/gCAD3D/debian/usr/share/gcad3d/icons
cp -f ../icons/*.png   ${instDir}/usr/share/gcad3d/icons/.
cp -f ../icons/*.xpm   ${instDir}/usr/share/gcad3d/icons/.
cp -f ../icons/*.bmp   ${instDir}/usr/share/gcad3d/icons/.
chmod 0644 ${instDir}/usr/share/gcad3d/icons/*



#/usr/share/doc/gcad3d/
#../debian/gCAD3D/debian/usr/share/doc/gcad3d/changelog.gz,copyright,gCAD3D.ico,gCAD3D.desktop
#cp /usr/share/common-licenses/GPL-3 ${instDir}/usr/share/doc/gcad3d/copyright
cp ../LICENSE           ${instDir}/usr/share/doc/gcad3d/copyright
#cp copyright            ${instDir}/usr/share/doc/gcad3d/.
cp changelog            ${instDir}/usr/share/doc/gcad3d/.
gzip -f --best ${instDir}/usr/share/doc/gcad3d/changelog
chmod 0644 ${instDir}/usr/share/doc/gcad3d/copyright
chmod 0644 ${instDir}/usr/share/doc/gcad3d/changelog.gz



#/usr/share/doc/gcad3d/html/
cp -f ../doc/html/*.htm  ${instDir}/usr/share/doc/gcad3d/html/.
cp -f ../doc/html/*.png  ${instDir}/usr/share/doc/gcad3d/html/.
cp -f ../doc/html/*.js   ${instDir}/usr/share/doc/gcad3d/html/.
chmod 0644 ${instDir}/usr/share/doc/gcad3d/html/*



#/usr/share/doc/gcad3d/msg/
cp -f ../doc/msg/*.txt   ${instDir}/usr/share/doc/gcad3d/msg/.
chmod 0644 ${instDir}/usr/share/doc/gcad3d/msg/*



#/usr/share/menu/gcad3d
cp _control/menu         ${instDir}/usr/share/menu/gcad3d
chmod 0644 ${instDir}/usr/share/menu/gcad3d



chmod 0755 ${instDir}/usr/lib/gCAD3D/binLinux${bits}
chmod 0755 ${instDir}/usr/lib/gCAD3D
chmod 0755 ${instDir}/usr/lib

chmod 0755 ${instDir}/usr/share/doc/gcad3d/html
chmod 0755 ${instDir}/usr/share/doc/gcad3d/msg
chmod 0755 ${instDir}/usr/share/doc/gcad3d
chmod 0755 ${instDir}/usr/share/doc

chmod 0755 ${instDir}/usr/share/gcad3d
chmod 0755 ${instDir}/usr/share/menu
chmod 0755 ${instDir}/usr/share

chmod 0755 ${instDir}/usr


#-----------------------------------------------------
# cd ~/devel/gcad3d/debian/gCAD3D
cd ${instDir}
cd ..
rm -f debian.deb
fakeroot dpkg-deb --build debian && lintian debian.deb
mv -f debian.deb ../${packNam}

echo "${packNam} created."

exit
# eof
