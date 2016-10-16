#!/bin/sh
# create package ${basDir}/debian/examples.gz
# start frome <base-directory>/debian   (<base-dir> /mnt/serv1/Devel/gcad3d/)
# copy files from <base-directory>/gCAD3D/dat/ -> <base-directory>/debian/exa_dir/
# gcad3d/debian/exa_dir is temporary only for creating examples.gz
#  inputfiles come from gcad3d/gCAD3D
# Test with:
# cd /mnt/serv1/Devel/gcad3d/debian/
# . ./pack_examples.sh
# ls -l /mnt/serv1/Devel/gcad3d/debian/examples.gz

echo "pack_examples.sh - copy from "`pwd`"/dat"

dirMdl=/mnt/serv1/Devel/cadfiles/gcad/

debDir=`pwd`
tempDir=${debDir}/exa_dir
cd ..
basDir=`pwd`

cd gCAD3D

rm -rf ${tempDir}
/bin/mkdir -p  ${tempDir}
cp .gitignore ${tempDir}/.


echo " start mkdir " ${tempDir}
/bin/mkdir -p ${tempDir}/cfg
/bin/mkdir -p ${tempDir}/tmp
/bin/mkdir -p ${tempDir}/dat
/bin/mkdir -p ${tempDir}/prg
/bin/mkdir -p ${tempDir}/ctlg
/bin/mkdir -p ${tempDir}/Schrau
/bin/mkdir -p ${tempDir}/Profile
/bin/mkdir -p ${tempDir}/symEl1

echo " - copy from "`pwd`"/dat"
echo " - copy to   "${tempDir}"/dat/"


# copy Startup-Model
#cp dat/sample_activity_modval_1.gcad  ${tempDir}/tmp/Model
#cp dat/Haus1.gcad        ${tempDir}/tmp/Model
#cp dat/sample_PRC_cut1_1.gcad ${tempDir}/tmp/Model
#cp dat/Boot_Dach_V1.gcad ${tempDir}/tmp/Model
/bin/cp ${dirMdl}sample_activity_modval_1.gcad ${tempDir}/tmp/Model

# copy example-models
/bin/cp ${dirMdl}samp*.*           ${tempDir}/dat/.
/bin/cp ${dirMdl}Renault1.gcad     ${tempDir}/dat/.
/bin/cp ${dirMdl}Haus1.gcad        ${tempDir}/dat/.
/bin/cp ${dirMdl}Haus*.jpg         ${tempDir}/dat/.
/bin/cp ${dirMdl}DachWS4.jpg       ${tempDir}/dat/.
/bin/cp ${dirMdl}SteinMarmor1.jpg  ${tempDir}/dat/.
/bin/cp ${dirMdl}Niet1.gcad        ${tempDir}/dat/.
/bin/cp ${dirMdl}Schrau1.gcad      ${tempDir}/dat/.

/bin/cp prg/demo_*.gcap       ${tempDir}/prg/.
/bin/cp prg/CirPat.gcap       ${tempDir}/prg/.
/bin/cp prg/LinPat.gcap       ${tempDir}/prg/.
/bin/cp prg/Haus1.gcap        ${tempDir}/prg/.
/bin/cp prg/Stair*.gcap       ${tempDir}/prg/.
/bin/cp prg/demo_*.gcap       ${tempDir}/prg/.
/bin/cp prg/DemoRC*           ${tempDir}/prg/.

/bin/cp cfg/ltyp.rc           ${tempDir}/cfg/.
/bin/cp cfg/gCAD3D.rc         ${tempDir}/cfg/.
/bin/cp cfg/psv.setup         ${tempDir}/cfg/.
/bin/cp cfg/Default.ctlg      ${tempDir}/cfg/.

/bin/cp ctlg/*                ${tempDir}/ctlg/.
/bin/cp Schrau/*              ${tempDir}/Schrau/.
/bin/cp Profile/*             ${tempDir}/Profile/.
/bin/cp symEl1/*              ${tempDir}/symEl1/.

# this files must be created:
#rm -f ${tempDir}/cfg/xa.rc
#rm -f ${tempDir}/cfg/dir.lst


cd ${tempDir}
tar cvf ${debDir}/examples *
gzip -f --best ${debDir}/examples


# eof
