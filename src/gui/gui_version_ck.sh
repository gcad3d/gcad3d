#!/bin/bash
# gui_version_ck.sh - test if dev-pack for VGUI is installed
#


# test if OSTYP DIR_BAS DIR_DEV gcad_dir_bin DEB VGUI defined (devbase.sh)
if test .$OSTYP = .; then
  echo "**** ERROR OSTYP is not set"
  exit 1
fi

PKGTYP=`cat ../$OSTYP/gcad_pkgtyp`
VGUI=`cat  ../$OSTYP/gcad_gui_version`

echo "- enter gui_version_ck.sh  $PKGTYP $VGUI"

if [ $VGUI = "gtk2" ] ; then
  if [ $PKGTYP = "DEB" ] ; then
    if [ -z "`dpkg -l | grep libgtk2.0-dev`" ] ; then
       echo "**** gui_version_set.sh ERROR gtk2 not installed"
       exit 1
    fi
    exit 0
  elif [ $PKGTYP = "NSIS" ] ; then
    echo "**** TODO - check MSYS - pkgconfig"
  elif [ $PKGTYP = "RPM" ] ; then
#   if [ -z "`pkg-config --list-all | grep gtk+-2.0`" ] ; then
    if [ -z "`rpm -qa | grep gtk2-dev`" ] ; then
       echo "**** gui_version_set.sh ERROR gtk2 not installed"
       exit 1
    fi
  fi
  exit 0
fi


if [ $VGUI = "gtk3" ] ; then
  if [ $PKGTYP = "DEB" ] ; then
    if [ -z "`dpkg -l | grep libgtk-3-dev`" ] ; then
       echo "**** gui_version_set.sh ERROR gtk3 not installed"
       exit 1
    fi
    exit 0
  else
#   if [ -z "`pkg-config --list-all | grep gtk+-3.0`" ] ; then
    if [ -z "`rpm -qa | grep gtk3-dev`" ] ; then
       echo "**** gui_version_set.sh ERROR gtk3 not installed"
       exit 1
    fi
  fi
  exit 0
fi


echo "******* gui_version_set.sh ERROR unknownGuiSoftware"
exit 2;


# EOF

