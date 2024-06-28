#!/bin/bash
# set environment for gcad3d-build-scripts
# set HTYP (Linux_x86_64 | MINGW_x86_64); 
# set OSTYP (UIX | MSYS)
# set directories gcad_dir_dev gcad_dir_bin gcad_dir_local
# display VGUI = gtk2        (file ./gcad_gui_version_${OSTYP})
# display DEB  =  ON | OFF   (file ./gcad_deb_${OSTYP})


#================================================================
# get HTYP OSTYP  (UIX/MSYS)
. ../UIX/sys.sh
#if [ $? -ne 0 ]; then exit 1; fi


# get version
export gcad_version=`cat ../gcad_version`
echo "gcad_version = ${gcad_version}"

#================================================================
# set gcad_dir_dev - only used for recompile plugin while main is active ..
export gcad_dir_dev="${PWD}/"

#================================================================
# gcad_dir_bin = home of all executables, shared-overlays, ..

#i1=`expr $ii + 8`
#export gcad_dir_bin=`expr substr ${gcad_dir_dev} 1 $i1`bin_${HTYP}/

if test "${OSTYP}" = "MSYS"; then
  # MSYS - %APPDATA%/test_bin/
  export gcad_dir_bin=$(cygpath -u "${APPDATA}\\test_bin\\")
else
  # Linux - # release-default: gcad3d/test/
  export gcad_dir_bin=${HOME}/test_bin/${gcad_version}/
fi

export LD_LIBRARY_PATH=${gcad_dir_bin}


#================================================================
# gcad_dir_bas = home of directories doc, icons, examples.gz,startup-script
# remove "src/UIX" from end of gcad_dir_dev
ii=`expr length ${gcad_dir_dev}`
i1=`expr $ii - 8`
export gcad_dir_bas=`expr substr ${gcad_dir_dev} 1 $i1`


#================================================================
# gcad_dir_local = home of directories tmp, cfg_Linux, dat, ctlg, prg
# ../../test

if test "${OSTYP}" = "MSYS"; then
  # MSYS - %APPDATA%/test/
  export gcad_dir_local=${APPDATA}\\test_local\\
else
  # Linux - # release-default: gcad3d/test/
  #export gcad_dir_local=`expr substr $gcad_dir_dev 1 $i1`test/gCAD3D/
  export gcad_dir_local=${HOME}/test_local/
fi

#================================================================
echo gcad_dir_dev = "${gcad_dir_dev}"
echo gcad_dir_bin = "${gcad_dir_bin}"
echo gcad_dir_bas = "${gcad_dir_bas}"
echo gcad_dir_local = "${gcad_dir_local}"
echo DEB     = `cat ./gcad_deb_${OSTYP}`
echo VGUI    = `cat ./gcad_gui_version_${OSTYP}`


# create gcad_dir_bin
if ! test -d "${gcad_dir_bin}"; then
  echo " - ${gcad_dir_bin} does NOT exist"
  /usr/bin/mkdir -p "${gcad_dir_bin}"
  /usr/bin/mkdir -p "${gcad_dir_bin}/plugins"
  /usr/bin/mkdir -p "${gcad_dir_bin}/plugins/cut1"
fi


# create gcad_dir_local
if ! test -d "${gcad_dir_local}"; then
  echo " - ${gcad_dir_local} does NOT exist"
  mkdir -p "${gcad_dir_local}"
fi

echo - exit devbase.sh

# EOF
