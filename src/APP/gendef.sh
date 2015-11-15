#! /bin/bash

# ./gendef.sh
# create Microsoft .def file from Linux executable.


# get bitNr gcad_dir_bin
source ../options.sh


# bits; 32|64

filNam=${gcad_dir_bin}/gendef
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
${filNam} $*
#gdb ./${filNam}

# eof
