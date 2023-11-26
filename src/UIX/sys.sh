#!/bin/bash
# set OSTYP and HTYP

# get HTYP
s1=`uname -s`_`uname -m`

s2=`echo ${s1} | cut -c1-5`
if test "$s2" = "MINGW"; then
  s1=${s2}_`uname -m`
  export OSTYP=MSYS

elif test "$s2" = "Linux"; then
  s1=${s2}_`uname -m`
  export OSTYP=UIX

else
  echo "****** ERROR - opsys ${s1} not supported. *****"
  exit 1

fi
export HTYP=${s1}

echo HTYP = "${HTYP}"
echo OSTYP = "${OSTYP}"

# eof
