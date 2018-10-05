#
# 2018-07-09 initial build. RF.
# Please report problems / corrections to franz.reiter@gcad3d.org
# Software-layout see ../doc/html/SW_layout_en.htm


%define gcad_version %(cat ${gcad_dir_dev}src/gcad_version)
%define hTyp %(echo "`uname -s`_`uname -i`")
%define outDir $RPM_BUILD_ROOT
%define icoDir "${gcad_dir_dev}icons"
%define docDir "${gcad_dir_dev}doc"





Summary: 3D - CADCAM - software
Name: gcad3d
Version: %{gcad_version}
Release: 1
License: GPLv3
Packager: gcad3d.org
Source:  https://github.com/gcad3d/gcad3d
Group: Applications/Graphics
BuildRoot: ../rpm/
#Requires: libGL.so.1 libGLU.so.1
Requires: tar
Requires: zenity




#####################################################################
%description
3D - CADCAM - software with functions for -
      import and export of cad-models and pictures (bitmaps)
      creation of geometrical objects
      NC-working, direct and programcontrolled
      preparation of data (building contours ..)
      analysis of data
      all objects can be connected with interactions
      add-on-programs (plugins)
      remote control


#####################################################################
%install
echo "gcad_dir_dev = " ${gcad_dir_dev}
echo "gcad_dir_bin = " ${gcad_dir_bin}
echo "outDir = " %{outDir}
echo "icoDir = " %{icoDir}
echo "hTyp = " %{hTyp}
echo "gcad_version = " %{gcad_version}


# copy startscript /usr/bin/gcad3d -> /usr/bin/gcad3d
mkdir -p %{outDir}/usr/bin
install ${gcad_dir_dev}src/gcad3d %{outDir}/usr/bin/gcad3d

# copy executable and libs -> /usr/lib64/gcad3d/Linux_x86_64/.
mkdir -p %{outDir}/usr/lib64/gcad3d/%{hTyp}
install -p ${gcad_dir_bin}/gCAD3D %{outDir}/usr/lib64/gcad3d/%{hTyp}/.
install -p ${gcad_dir_bin}/*.so %{outDir}/usr/lib64/gcad3d/%{hTyp}/.

# copy plugins -> /usr/lib64/gcad3d/Linux_x86_64/plugins/.
mkdir -p %{outDir}/usr/lib64/gcad3d/%{hTyp}/plugins
install -p ${gcad_dir_bin}/plugins/*.so %{outDir}/usr/lib64/gcad3d/%{hTyp}/plugins/.

# copy NC-processes -> /usr/lib64/gcad3d/Linux_x86_64/plugins/cut1/.
#mkdir -p %{outDir}/usr/lib64/gcad3d/%{hTyp}/plugins/cut1
#install -p ${gcad_dir_bin}/plugins/cut1/* %{outDir}/usr/lib64/gcad3d/%{hTyp}/plugins/cut1/.

# copy README LICENSE LICENSE_GPLv3.txt gCAD3D_log.txt -> /usr/share/doc/gcad3d
mkdir -p %{outDir}/usr/share/doc/gcad3d
/bin/cp -f ${gcad_dir_dev}README %{outDir}/usr/share/doc/gcad3d/.
/bin/cp -f ${gcad_dir_dev}LICEN* %{outDir}/usr/share/doc/gcad3d/.
/bin/cp -f ${gcad_dir_dev}doc/gCAD3D_log.txt %{outDir}/usr/share/doc/gcad3d/.


# copy png,xpm,bmp  icons -> /usr/share/gcad3d/icons/.
mkdir -p %{outDir}/usr/share/gcad3d/icons
install -p %{icoDir}/*.png %{outDir}/usr/share/gcad3d/icons/.
install -p %{icoDir}/*.xpm %{outDir}/usr/share/gcad3d/icons/.
install -p %{icoDir}/*.bmp %{outDir}/usr/share/gcad3d/icons/.

# copy Docu -> /usr/share/doc/gcad3d/html/
mkdir -p %{outDir}/usr/share/doc/gcad3d/html
install -p %{docDir}/html/*.htm  %{outDir}/usr/share/doc/gcad3d/html/.
install -p %{docDir}/html/*.png  %{outDir}/usr/share/doc/gcad3d/html/.
install -p %{docDir}/html/*.js   %{outDir}/usr/share/doc/gcad3d/html/.

# copy programMessagefiles -> /usr/share/doc/gcad3d/msg/
mkdir -p %{outDir}/usr/share/doc/gcad3d/msg
install -p %{docDir}/msg/*.txt  %{outDir}/usr/share/doc/gcad3d/msg/.

# copy desktop-starter -> /usr/share/applications/gcad3d.desktop
mkdir -p %{outDir}/usr/share/applications
install -p ${gcad_dir_dev}src/gcad3d.desktop  %{outDir}/usr/share/applications/.

# copy pixmap for desktop-starter -> /usr/share/pixmaps/gCAD3D.xpm
mkdir -p %{outDir}/usr/share/pixmaps
install -p %{icoDir}/gCAD3D.xpm %{outDir}/usr/share/pixmaps/gcad3d.xpm

# copy examples.gz (dat,prg,ctlg,..) -> /usr/share/gcad3d/.
/bin/cp -f ${gcad_dir_dev}packages/examples.gz %{outDir}/usr/share/gcad3d/.


#####################################################################
%files
%defattr(-,root,root)
/usr/bin/gcad3d
/usr/lib64/gcad3d/%{hTyp}/*

/usr/share/gcad3d/examples.gz
/usr/share/gcad3d/icons/*
/usr/share/doc/gcad3d/*

/usr/share/applications/gcad3d.desktop
/usr/share/pixmaps/gcad3d.xpm


%clean
rm -rf %{outDir}


# EOF
