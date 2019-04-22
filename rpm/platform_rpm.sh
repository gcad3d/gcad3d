#!/bin/sh
#
# set Version platform packNam
#
# Using symbols DIR_DEV


# set SW-Version
Version=`cat ${DIR_DEV}gcad3d/src/gcad_version`

# set ${platform} - x86_64|i386|i586|i686
# see ../debian/platform_deb.sh     only x86_64|i386 - ??
#platform=`uname -s`_`uname -i`
platform=`uname -i`

# set packNam = filename for the rpm-package
packNam="gcad3d-${Version}-1.${platform}.rpm"

# eof
