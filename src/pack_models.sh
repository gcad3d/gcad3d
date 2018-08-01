#!/bin/sh
#
# copy demo-models from /mnt/serv1/Devel/cadfiles/gcad/
# >>>>> /mnt/serv1/Devel/gcad3d/gCAD3D/dat/

mdlDir=${DIR_DEV}cadfiles/gcad
outDir=${DIR_DEV}gcad3d/gCAD3D

rm -rf ${outDir}/dat/*


/bin/cp ${mdlDir}/samp*.*           ${outDir}/dat/.
/bin/cp ${mdlDir}/Haus1.gcad        ${outDir}/dat/.
/bin/cp ${mdlDir}/Haus*.jpg         ${outDir}/dat/.
/bin/cp ${mdlDir}/DachWS4.jpg       ${outDir}/dat/.
/bin/cp ${mdlDir}/SteinMarmor1.jpg  ${outDir}/dat/.
/bin/cp ${mdlDir}/Niet1.gcad        ${outDir}/dat/.
/bin/cp ${mdlDir}/Schrau1.gcad      ${outDir}/dat/.

# eof
