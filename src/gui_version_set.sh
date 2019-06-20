#!/bin/sh
#
# test if gtk-version is installed;  set the active gtk-version:
#   write into file ../gcad_gui_version.
#
# Input:  "gtk2 or gtk3"
# Output: "gtk2 or gtk3" in file ../gcad_gui_version
# RetCode: 0=OK; 1=notInstalled; 2=unknownGuiSoftware
#


# get install-type, "rpm" or "deb"
instTyp=`../installtyp.sh`
echo "instTyp = |"${instTyp}"|"


if [ $1 = "gtk2" ] ; then
  if [ ${instTyp} = "deb" ] ; then
    if [ -z "`dpkg -l | grep libgtk2.0-dev`" ] ; then
       echo "**** gui_version_set.sh ERROR gtk2 not installed"
       exit 1
    fi
  else
#   if [ -z "`pkg-config --list-all | grep gtk+-2.0`" ] ; then
    if [ -z "`rpm -qa | grep gtk2-dev`" ] ; then
       echo "**** gui_version_set.sh ERROR gtk2 not installed"
       exit 1
    fi
  fi
  echo "gtk2" > ../gcad_gui_version
  exit 0
fi


if [ $1 = "gtk3" ] ; then
  if [ ${instTyp} = "deb" ] ; then
    if [ -z "`dpkg -l | grep libgtk-3-dev`" ] ; then
       echo "**** gui_version_set.sh ERROR gtk3 not installed"
       exit 1
    fi
  else
#   if [ -z "`pkg-config --list-all | grep gtk+-3.0`" ] ; then
    if [ -z "`rpm -qa | grep gtk3-dev`" ] ; then
       echo "**** gui_version_set.sh ERROR gtk3 not installed"
       exit 1
    fi
  fi
  echo "gtk3" > ../gcad_gui_version
  exit 0
fi


echo "******* gui_version_set.sh ERROR unknownGuiSoftware"
exit 2;


# EOF
