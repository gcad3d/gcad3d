#! /bin/bash

# ./gendef.sh
# create Microsoft gCAD3D.def file from Linux executable.


# get bitNr gcad_dir_bin
source ../options.sh


# bits; 32|64


outFile="gCAD3D.def" 
inFile1=${gcad_dir_bin}/gCAD3D
inFile2=${gcad_dir_bin}/xa_gui.so

echo "outfile =" ${outFile}
echo "infile1 =" ${inFile1}
echo "infile2 =" ${inFile2}


exeNam=${gcad_dir_bin}/gendef
echo "exeNam" ${exeNam}

# TEST ONLY
rm -rf ${filNam}

# create executable if it does not exist
#if [ ! -f ${exeNam} ]; then
echo "build" ${exeNam}
cc -DDEB -ggdb -pg gendef.c
mv -f a.out ${exeNam}
#fi


# create Files.dox
${exeNam} ${outFile} ${inFile1} ${inFile2}
#gdb ./${filNam}

# eof
