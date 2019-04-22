#! /bin/sh
#
# create rpm-package
# start with ${DIR_DEV}gcad3d/rpm/pack_rpm.sh
#
# Using symbols DIR_DEV and DIR_BIN. See ../../doc/html/SW_layout_en.htm
# 
#
echo "============ pack_rpm.sh ============"

if [ ! -n "$DIR_DEV" ]; then
  echo "****** ERROR - DIR_DEV undefined *******"
  exit 1
fi

if [ ! -n "$DIR_BIN" ]; then
  echo "****** ERROR - DIR_BIN undefined *******"
  exit 2
fi



# build; create RPMS/gcad3d-#-##-#.<architecture>.rpm
cd ${DIR_DEV}gcad3d/rpm
rpmbuild --define "_topdir ${DIR_DEV}gcad3d/rpm" -ba -vv gcad3d.spec
if [ $? -ne 0 ]; then
  echo "****** ERROR rpmbuild *******"
  exit 3;
fi


# get Version platform packNam
. ${DIR_DEV}gcad3d/rpm/platform_rpm.sh


# test success
fnam=${DIR_DEV}gcad3d/rpm/RPMS/${HOSTTYPE}/${packNam}
if [ ! -f $fnam ]; then
  echo "****** ERROR - $fnam does not exist. *******"
  exit 4
fi


# list package
rpm -qipl ${fnam}


# copy package -> packages/.
/bin/mv -f ${fnam} ${DIR_DEV}gcad3d/packages/.

# remove unused source-package
rm -f ${DIR_DEV}gcad3d/rpm/SRPMS/*


echo "-----------------------------------------"
#echo "- $fnam created -"
echo "- ${DIR_DEV}gcad3d/packages/${packNam} created -"


exit 0
# EOF
