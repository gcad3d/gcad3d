#!/bin/sh

# create ${doxy_path_out}/index.htm and all files 
# start script in doc/ or src/
# doxy_path_out="<base>/doc/gcad"

# UPDATE files src/gcad_version and src/gcad_date
# ReCreate documentation:   ./gcad_doxygen.sh




# version, date, project-name
version=`cat ../src/gcad_version`
date=`cat ../src/gcad_date`
doxy_prj_nam="gCAD3D $version"
export doxy_prj_nam


# path from doxygen-InputfileDirectory -> sourceBasedirectory
# start create doc here ..
srcBase="../src/APP"

# path from srcBase -> doxygen-InputfileDirectory
doxBase="../../doc/gcad_doxygen"

# path from srcBase -> doxygen-outputDirectory
doxy_path_out="../../doc/gcad"
export doxy_path_out

# path from doxygen-outputDirectory -> srcBase
dox_path_src="../../src"
export dox_path_src

doxy_path_base=`pwd`
export doxy_path_base

# sourcedirectories
srcDirs="\
 ../APP\
 ../xa\
 ../ut\
 ../gr\
 ../ci\
 ../db\
 ../exp\
 ../prc\
 ../gui\
 ../gui_gtk2\
 ../gui_gtk3\
"
export srcDirs


# all dox-files; this makes the main-menu.
# Do not change first line (sourcedirectories)
myFiles="${srcDirs}\
 ${doxBase}/mainpage.dox\
 ${doxBase}/Introduction.dox\
 ${doxBase}/Sourcefiles_gcad.dox\
 ${doxBase}/Sourcefiles_gui.dox\
 ${doxBase}/Download.dox\
 ${doxBase}/TODO.dox\
 ${doxBase}/ruler1.dox\
 ${doxBase}/Plugins.dox\
 ${doxBase}/Userinteractions.dox\
 ${doxBase}/Messages.dox\
 ${doxBase}/Objects-Create.dox\
 ${doxBase}/Objects-Resolve.dox\
 ${doxBase}/ObjectFormatConversions.dox\
 ${doxBase}/Attributes.dox\
 ${doxBase}/ruler2.dox\
 ${doxBase}/Tools-MemoryFunctions.dox\
 ${doxBase}/Tools-TextFunctions.dox\
 ${doxBase}/Tools-GeometricFunctions.dox\
 ${doxBase}/Tools-MathNumericFunctions.dox\
 ${doxBase}/Tools-OperatingSystemFunctions.dox\
 ${doxBase}/ruler3.dox\
 ${doxBase}/Userinteractions.dox\
 ${doxBase}/ProgramFlow.dox\
 ${doxBase}/Binary-Format.dox\
 ${doxBase}/File-Format-native.dox\
 ${doxBase}/File-Format-binary.dox\
 ${doxBase}/Programminginfos.dox\
 ${doxBase}/NamingConventions.dox\
 ${doxBase}/Doxygen_hints.dox\
 ${doxBase}/ruler3.dox\
"
export myFiles


############# work ..
cd $srcBase


# update version & date in mainpage.dox
cat ${doxBase}/mainpage | sed "s/<VERSION>/$version/"  | sed "s/<DATE>/$date/" > ${doxBase}/mainpage.dox
#exit


# update version in Download.dox
cat ${doxBase}/Download | sed "s/<VERSION>/$version/g" > ${doxBase}/Download.dox
#exit


#-------------------------------------------------------
# create Sourcefiles_gcad.dox
# create file ../src/APP/srcFiles (a list of sourcefiles)
make -f gcad3d.mak srclst
./doxy_help1.sh "Sourcefiles_gcad"
mv -f Sourcefiles.dox ${doxBase}/Sourcefiles_gcad.dox
#vi ${doxBase}/Sourcefiles_gcad.dox
#exit


#-------------------------------------------------------
# create Sourcefiles_gui.dox
# create file ../src/APP/srcFiles (a list of sourcefiles)
make -f gcad_gui_lnk.mak srclst
./doxy_help1.sh "Sourcefiles_gui"
mv -f Sourcefiles.dox ${doxBase}/Sourcefiles_gui.dox
#vi ${doxBase}/Sourcefiles_gui.dox
#exit


#-------------------------------------------------------
# clear outDir, but keep .gitignore
rm -rf ${doxy_path_out}/*


# copy image
cp ${doxBase}/*.jpg ${doxy_path_out}/.


# start doxygen
# doxygen-input: $doxy_prj_nam $doxy_path $myFiles and file Doxyfile
doxygen ${doxBase}/Doxyfile


# display new docu
xdg-open ${doxy_path_out}/index.htm

exit
# EOF
