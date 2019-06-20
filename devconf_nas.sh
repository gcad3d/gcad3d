#!/bin/sh
#
# set symbols DIR_DEV and DIR_BIN for development on NAS witout git
# see devconf_git.sh (development with git - local)
#
# DIR_DEV     home of sourcefiles, scripts, linkjobs, packages
# for gcad3d: ${DIR_DEV}gcad3d/....
export DIR_DEV=/mnt/serv2/Devel/
#
# DIR_BIN     home of objects, executables, libraries
export DIR_BIN=${HOME}/devel/bin/
# for gcad3d: ${HOME}/devel/bin/gcad3d/
#
# EOF
