#!/bin/bash

# create debian-package                         ./pack_deb.sh
# start in <basDir>/gcad3d/src/APP
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
## instDir =  gcad3d/debian/gCAD3D/debian
#
# Software-Layout see <basDir>/gcad3d/doc/html/SW_layout_en.htm
#

echo "- entering pack_deb.sh"


bitNr=`getconf LONG_BIT`
hTyp=`uname -s`_`uname -m`



# get ${Version} - SW-Version
# get ${platform} - amd64|i386|armhf 
# get ${packNam} - filename for the deb-package
. ${gcad_dir_bas}debian/platform_deb.sh


debDir=${gcad_dir_bas}debian
echo "debDir = " ${debDir}


# copy all files -> instDir = ${gcad_dir_bas}debian/gCAD3D/debian
instDir=${gcad_dir_bas}debian/gCAD3D/debian
echo "instDir = " ${instDir}


# copy package -> ${outDir}
outDir=${gcad_dir_bas}packages
echo "outDir = " ${outDir}


echo "start create debian-package" ${packNam}

#exit 1   # TESTEXIT


#-----------------------------------------------------
# remove install-dir; rm -rf ~/devel/gcad3d/debian/gCAD3D/debian
rm -rf ${instDir}


/bin/mkdir -p -m 755 ${instDir}
/bin/mkdir -p -m 755 ${instDir}/DEBIAN


# ${instDir}/DEBIAN/control - select debian-controlfile for active platform and fix "Version"
cat ${debDir}/_control/control_${platform} | sed "s/<VERSION>/$Version/" > ${instDir}/DEBIAN/control

# copy debian-ControlFiles -> ${instDir}/DEBIAN/. (preinst postinst prerm)
/bin/cp -f ${debDir}/_control/p* ${instDir}/DEBIAN/.

chmod 0755 ${instDir}/DEBIAN/p*




#------------------------------------------------------
# copy all application-files
${debDir}/install_deb.sh ${instDir}



#-----------------------------------------------------
# strip exe and so-files
/usr/bin/strip ${instDir}/usr/lib/gcad3d/${hTyp}/gCAD3D
/usr/bin/strip ${instDir}/usr/lib/gcad3d/${hTyp}/*.so
/usr/bin/strip ${instDir}/usr/lib/gcad3d/${hTyp}/GUI_*
/usr/bin/strip ${instDir}/usr/lib/gcad3d/${hTyp}/gcad3d_gMsh
/usr/bin/strip ${instDir}/usr/lib/gcad3d/${hTyp}/plugins/*.so
/usr/bin/strip ${instDir}/usr/lib/gcad3d/${hTyp}/plugins/cut1/*

chmod 0755 ${instDir}/usr
chmod 0755 ${instDir}/usr/lib
chmod 0755 ${instDir}/usr/share
chmod 0755 ${instDir}/usr/share/doc



#-----------------------------------------------------
cd ${instDir}

cd ..
 
rm -f debian.deb



# build
fakeroot dpkg-deb --build debian && lintian debian.deb
mv -f debian.deb ${outDir}/${packNam}

echo "${outDir}/${packNam} created."

rm -rf ${instDir}

exit
# eof
