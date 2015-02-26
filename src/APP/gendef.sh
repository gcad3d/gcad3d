#! /bin/bash

# ./gendef.sh
# create Microsoft .def file from Linux executable.




# bits; 32|64
bitNr=`getconf LONG_BIT`
OS=Linux${bitNr}
OUTDIR=../../bin$OS

filNam=${OUTDIR}/gendef
echo "help-file=" ${filNam}

# TEST ONLY
rm -rf ${filNam}

# create executable if it does not exist
if [ ! -f ${filNam} ]; then
echo "build" ${filNam}
cc -DDEB -ggdb -pg gendef.c
mv -f a.out ${filNam}
fi


# create Files.dox
./${filNam} $*
#gdb ./${filNam}

# eof
