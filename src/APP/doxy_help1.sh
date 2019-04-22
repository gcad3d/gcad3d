#!/bin/sh
# gcad_dir_dev and gcad_dir_bin must be set (. ../options.sh)

echo "doxy_help1.sh start"
echo $*


#bitNr=`getconf LONG_BIT`
#OS=Linux${bitNr}
#OUTDIR=../../bin$OS

filNam=${gcad_dir_bin}doxy_help1
echo "help-file=" ${filNam}

# TEST ONLY
#rm -rf ${filNam}

# create executable if it does not exist
if [ ! -f ${filNam} ]; then
echo "build" ${filNam}
cc -DDEB -ggdb -pg doxy_help1.c
mv -f a.out ${filNam}
fi


# create Files.dox
${filNam} $*
#gdb ${filNam}

# eof
