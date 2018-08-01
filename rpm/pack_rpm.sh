#! /bin/bash
#
# start with ${DIR_DEV}gcad3d/rpm/pack_rpm.sh
# Using symbols DIR_DEV and DIR_BIN. See ../../doc/html/SW_layout_en.htm
# 
#
echo "============ pack_rpm.sh ============"

# test if options.sh exists
if [ ! -f ${DIR_DEV}gcad3d/src/options.sh ] ; then
  echo "****** ERROR - file ../options.sh does not exist. *******"
  exit
fi

# get symbols gcad_dir_dev and gcad_dir_bin
. ${DIR_DEV}gcad3d/src/options.sh

cd ${DIR_DEV}gcad3d/rpm

hTyp=${HOSTTYPE}

# create RPMS/gcad3d-#-##-#.<architecture>.rpm
rpmbuild --define "_topdir ${DIR_DEV}gcad3d/rpm" -ba -vv gcad3d.spec
#
# list package
rpm -qipl ${DIR_DEV}gcad3d/rpm/RPMS/${hTyp}/*


# copy package -> packages/.
/bin/mv -f ${DIR_DEV}gcad3d/rpm/RPMS/${hTyp}/* ${DIR_DEV}gcad3d/packages/.


# TODO:
# add into uninstall-process: remove ~/gCAD3D manually
#   (see ../debian/_control/prerm)


# EOF
