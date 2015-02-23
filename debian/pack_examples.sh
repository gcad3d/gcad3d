#!/bin/csh
# create package ${basDir}/debian/examples.gz
# start frome <base-directory>/debian
# copy files from ? -> ?
# gcad3d/debian/exa_dir is temporary only for creating examples.gz
#  inputfiles come from gcad3d/gCAD3D

set debDir=`pwd`
cd ../gCAD3D
set tempDir=${debDir}/exa_dir


rm -rf ${tempDir}
mkdir -p  ${tempDir}
cp .gitignore ${tempDir}/.


echo " start mkdir " ${tempDir}
mkdir -p ${tempDir}/cfg
mkdir -p ${tempDir}/tmp
mkdir -p ${tempDir}/dat
mkdir -p ${tempDir}/prg
mkdir -p ${tempDir}/ctlg
mkdir -p ${tempDir}/Schrau
mkdir -p ${tempDir}/Profile
mkdir -p ${tempDir}/symEl1

cp cfg/*                 ${tempDir}/cfg/.
# this files must be created:
rm -f ${tempDir}/cfg/xa.rc
rm -f ${tempDir}/cfg/dir.lst

# copy Startup-Model
#cp dat/sample_activity_modval_1.gcad  ${tempDir}/tmp/Model
#cp dat/Haus1.gcad        ${tempDir}/tmp/Model
#cp dat/sample_PRC_cut1_1.gcad ${tempDir}/tmp/Model
#cp dat/Boot_Dach_V1.gcad ${tempDir}/tmp/Model
cp dat/sample_activity_modval_1.gcad ${tempDir}/tmp/Model

# copy example-models
cp dat/samp*.*           ${tempDir}/dat/.
cp dat/Renault1.gcad     ${tempDir}/dat/.
cp dat/Haus*.*           ${tempDir}/dat/.
cp dat/DachWS4.jpg       ${tempDir}/dat/.
cp dat/SteinMarmor1.jpg  ${tempDir}/dat/.
cp dat/Niet1.gcad        ${tempDir}/dat/.
cp dat/Schrau1.gcad      ${tempDir}/dat/.

cp prg/demo_*.gcap       ${tempDir}/prg/.
cp prg/CirPat.gcap       ${tempDir}/prg/.
cp prg/LinPat.gcap       ${tempDir}/prg/.
cp prg/Haus1.gcap        ${tempDir}/prg/.
cp prg/Stair*.gcap       ${tempDir}/prg/.

cp prg/DemoRC*           ${tempDir}/prg/.

cp ctlg/*                ${tempDir}/ctlg/.
cp Schrau/*              ${tempDir}/Schrau/.
cp Profile/*             ${tempDir}/Profile/.
cp symEl1/*              ${tempDir}/symEl1/.


cd ${tempDir}
tar cvf ${debDir}/examples *
gzip -f --best ${debDir}/examples


# eof
