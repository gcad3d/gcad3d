#
# 2018-07-09 initial build. RF.
# Please report problems / corrections to franz.reiter@gcad3d.org
# Software-layout see ../../doc/html/SW_layout_en.htm
# must have defined DIR_DEV and DIR_BIN

%define hTyp %(echo "`uname -s`_`uname -i`")
%define	gcad_version %(cat ${DIR_BAS}src/gcad_version)
%define gcad_dir_dev "${DIR_BAS}"
# gcad_dir_bin = ?
%define gcad_dir_bin "${DIR_BIN}gcad3d/%{hTyp}/"
%define outDir $RPM_BUILD_ROOT
%define icoDir "%{gcad_dir_dev}icons"
%define docDir "%{gcad_dir_dev}doc"





Summary: 3D - CADCAM - software
Name:		gcad3d
Version: %{gcad_version}
Release: 1
License: GPLv3+
Packager: gcad3d.org
URL:			http://www.gcad3d.org
Source:		https://github.com/gcad3d/gcad3d
Group: Applications/Graphics
BuildRoot: ../rpm/
#Requires: libGL.so.1 libGLU.so.1
BuildRequires:	ctags
#BuildRequires:	pkgconfig(gtk+-3.0)
BuildRequires:	pkgconfig(gtk+-2.0)
BuildRequires:	pkgconfig(gl)
BuildRequires:	pkgconfig(glu)
Requires: tar




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
%prep

#####################################################################
%build

#####################################################################
%install
echo "hTyp = " %{hTyp}
echo "gcad_version = " %{gcad_version}
echo "DIR_DEV = " ${DIR_DEV}
echo "DIR_BIN = " ${DIR_BIN}
echo "gcad_dir_dev = " %{gcad_dir_dev}
echo "gcad_dir_bin = " %{gcad_dir_bin}
echo "outDir = " %{outDir}
echo "icoDir = " %{icoDir}
echo "docDir = " %{docDir}


# copy startscript /usr/bin/gcad3d -> /usr/bin/gcad3d
mkdir -p %{outDir}/usr/bin

install %{gcad_dir_dev}src/gcad3d %{outDir}/usr/bin/gcad3d




# copy executable and libs -> /usr/lib/gcad3d/Linux_x86_64/.
mkdir -p -m 755 %{outDir}/usr/lib/gcad3d/%{hTyp}
mkdir -p -m 755 %{outDir}/usr/lib/gcad3d/%{hTyp}/plugins
mkdir -p -m 755 %{outDir}/usr/lib/gcad3d/%{hTyp}/plugins/cut1

install -m 755 %{gcad_dir_bin}gCAD3D %{outDir}/usr/lib/gcad3d/%{hTyp}/.
install -m 755 %{gcad_dir_bin}GUI_* %{outDir}/usr/lib/gcad3d/%{hTyp}/.
install -m 755 %{gcad_dir_bin}*.so %{outDir}/usr/lib/gcad3d/%{hTyp}/.
install -m 755 %{gcad_dir_bin}gcad3d_gMsh %{outDir}/usr/lib/gcad3d/%{hTyp}/.
install -m 755 %{gcad_dir_bin}plugins/*.so %{outDir}/usr/lib/gcad3d/%{hTyp}/plugins/.
install -m 755 %{gcad_dir_bin}plugins/cut1/* %{outDir}/usr/lib/gcad3d/%{hTyp}/plugins/cut1/.



# copy demo-models, icons, help-files
mkdir -p -m 755 %{outDir}/usr/share/gcad3d
mkdir -p -m 755 %{outDir}/usr/share/gcad3d/icons
mkdir -p -m 755 %{outDir}/usr/share/gcad3d/doc/html
mkdir -p -m 755 %{outDir}/usr/share/gcad3d/doc/msg

install -m 644 %{gcad_dir_dev}packages/examples.gz %{outDir}/usr/share/gcad3d/.
install -m 644 %{icoDir}/*.png        %{outDir}/usr/share/gcad3d/icons/.
install -m 644 %{icoDir}/*.xpm        %{outDir}/usr/share/gcad3d/icons/.
install -m 644 %{icoDir}/*.bmp        %{outDir}/usr/share/gcad3d/icons/.
install -m 644 %{docDir}/*.txt        %{outDir}/usr/share/gcad3d/doc/.
install -m 644 %{docDir}/html/*.htm   %{outDir}/usr/share/gcad3d/doc/html/.
install -m 644 %{docDir}/html/*.png   %{outDir}/usr/share/gcad3d/doc/html/.
install -m 644 %{docDir}/html/*.js    %{outDir}/usr/share/gcad3d/doc/html/.
install -m 644 %{docDir}/msg/*.txt    %{outDir}/usr/share/gcad3d/doc/msg/.



# copy desktop-starter -> /usr/share/applications/gcad3d.desktop
mkdir -p -m 755 %{outDir}/usr/share/applications

install -m 644 %{gcad_dir_dev}src/gcad3d.desktop  %{outDir}/usr/share/applications/.



# copy pixmap for desktop-starter -> /usr/share/pixmaps/gcad3d.xpm
mkdir -p -m 755 %{outDir}/usr/share/pixmaps
install -m 644 %{icoDir}/gCAD3D.xpm %{outDir}/usr/share/pixmaps/gcad3d.xpm




# copy README LICENSE LICENSE_GPLv3.txt gCAD3D_log.txt -> /usr/share/doc/gcad3d/
mkdir -p -m 755 %{outDir}/usr/share/doc/gcad3d

install -m 644 %{gcad_dir_dev}README             %{outDir}/usr/share/doc/gcad3d/.
install -m 644 %{gcad_dir_dev}LICENSE            %{outDir}/usr/share/doc/gcad3d/.
install -m 644 %{gcad_dir_dev}doc/gCAD3D_log.txt %{outDir}/usr/share/doc/gcad3d/NEWS



#####################################################################
%files
%defattr(-,root,root)
/usr/bin/gcad3d
/usr/lib/gcad3d/%{hTyp}/*
/usr/share/gcad3d/*
/usr/share/doc/gcad3d/*
/usr/share/applications/gcad3d.desktop
/usr/share/pixmaps/gcad3d.xpm


%post
# create link for active gui-dll
# back to gtk2 (problems with NV cards)
#libInf=`ldconfig -p | grep "libgtk-3"`
#if [ ! -z "$libInf" ]; then
#  echo "gtk3-libs: $libInf"
#  ln -fs ${gcad_dir_bin}/xa_gui_gtk3.so ${gcad_dir_bin}/xa_gui.so
#fi
libInf=`ldconfig -p | grep "libgtk-x11-2"`
if [ ! -z "$libInf" ]; then
  echo "gtk2-libs: $libInf"
  ln -fs ${gcad_dir_bin}/xa_gui_gtk2.so ${gcad_dir_bin}/xa_gui.so
fi



%clean
rm -rf %{outDir}


%postun
echo "******************************"
echo "*** REMOVE ~/gCAD3D manually !"
echo "******************************"



# EOF
