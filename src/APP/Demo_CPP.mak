# make -f Demo_CPP.mak "OS=Linux32"
# 


DLLNAM = plugins/Demo_CPP

SRCTYP = cpp

SRC1 =\
  ../xa/Demo_CPP.cpp

DIRSRC1 := ../xa/

include gcad_dll.mak

########################### EOF #######################
