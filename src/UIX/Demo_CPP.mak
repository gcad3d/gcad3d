# . ./devbase.sh && make -f Demo_CPP.mak
# update 2024-08-20

DLLNAM = plugins/Demo_CPP

SRCTYP = cpp

SRC1 =\
  ../APP/Demo_CPP.cpp

DIRSRC1 := ../APP/

include gcad_dll.mak

########################### EOF #######################
