#!/bin/sh
#
# get ${platform} - amd64|i386|armhf
echo "- entering platform_deb.sh"


# get HOSTTYPE eg x86_64
mach=`uname -m`


if [ "${mach}" = "armv7l" ] ; then
# raspi
 platform="armhf"
elif [ "${bitNr}" = "32" ] ; then
 # x86
 platform="i386"
else
 # x86_64
 platform="amd64"
fi

echo "platform =" ${platform}


# get SW-Version
#Version=`cat ${gcad_dir_bas}src/gcad_version`
Version=`cat ${gcad_dir_bas}src/gcad_version | sed "s/_/./g"`
echo "Version =" ${Version}


packNam="gCAD3D-${Version}-bin-${platform}.deb"
echo "packNam =" ${packNam}

echo "- leaving platform_deb.sh"
# eof
