#!/bin/sh
#
# install into system. Must be root (sudo -E)
#
# Using symbols DIR_DEV and DIR_BIN. See ../../doc/html/SW_layout_en.htm



# get Version platform packNam
. ${DIR_DEV}gcad3d/rpm/platform_rpm.sh

echo "# install ${packNam}"

rpm -i ${DIR_DEV}gcad3d/packages/${packNam}
exit $?

# eof
