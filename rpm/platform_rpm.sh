#!/bin/sh
#
# set Version platform packNam


# set SW-Version
Version=`cat ${DIR_BAS}src/gcad_version`
echo "Version =" ${Version}


# set ${platform} - x86_64|i386|i586|i686
# see ../debian/platform_deb.sh     only x86_64|i386 - ??
#platform=`uname -s`_`uname -i`
platform=`uname -i`
echo "platform =" ${platform}


# set packNam = filename for the rpm-package
packNam="gcad3d-${Version}-1.${platform}.rpm"
echo "packNam =" ${packNam}

# eof
