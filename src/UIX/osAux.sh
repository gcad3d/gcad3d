#!/bin/bash
# copy all libs indo binDir
# ./devbase.sh && ./osAux.sh <filname_binFiles>


if ! test -f "$1"; then
  echo "****** ERROR inputfile $1 does not exist *******"
  exit 1;
fi


#--------------------- MS-Win
if test "${OSTYP}" = "MSYS"; then
  if ! test -f "osAux.exe"; then
    cc  ../APP/osAux.c ../ut/os__.c ../ut/ut_os_ms.c ../ut/ut_TX.c ../ut/msg_simple.c -o osAux.exe
  fi

  if ! test -f "osAux.exe"; then
    echo "****** ERROR copy systemfiles *******"
    exit 1;
  fi

  # copy files
  ./osAux.exe $1


#--------------------- Linux
else
  if ! test -f "osAux"; then
    cc  ../APP/osAux.c ../ut/os__.c ../ut/ut_os_uix.c ../ut/ut_TX.c ../ut/msg_simple.c -o osAux
  fi

  if ! test -f "osAux"; then
    echo "****** ERROR copy systemfiles *******"
    exit 1;
  fi

  # copy files
  ./osAux $1
fi


# eof 
