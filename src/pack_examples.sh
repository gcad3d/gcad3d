#!/bin/sh
#
# create package ${DIR_DEV}gcad3d/packages/examples.gz
# examples.gz contains config-files and sample-models.
# Using symbols DIR_DEV and DIR_BIN. See ../../doc/html/SW_layout_en.htm
#
# start ${DIR_DEV}gcad3d/src/pack_examples.sh
#  inputfiles come from ${DIR_DEV}gcad3d/gCAD3D/

echo "============ pack_examples.sh ============"

# test if options.sh exists
if [ ! -f ${DIR_DEV}gcad3d/src/options.sh ] ; then
  echo "****** ERROR - file ../options.sh does not exist. *******"
  exit
fi

# get symbols gcad_dir_dev and gcad_dir_bin
. ${DIR_DEV}gcad3d/src/options.sh


cd ${DIR_DEV}gcad3d/gCAD3D
echo " .. now in `pwd`"

# set tempDir for output
# tempDir is temporary only for creating examples.gz
tempDir=../packages/tmp

# set outDir for outputfile examples.gz
outDir=${gcad_dir_dev}packages


rm -rf ${tempDir}
/bin/mkdir -p  ${tempDir}
#cp .gitignore ${tempDir}/.


# create outDirectory  ${tempDir}
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
#/bin/cp ${dirMdl}sample_activity_modval_1.gcad ${tempDir}/tmp/Model


#-----------------------------------------------------------
# copy example-models  (see pack_models.sh)
/bin/cp dat/*                 ${tempDir}/dat/.

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
tar cvf ${outDir}/examples *
gzip -f --best ${outDir}/examples


echo "---- file ${outDir}/examples.gz crated .."
ls -latr ${outDir}/examples*


# eof
