# . ../options.sh && make -f xa_dxf_w.mak 

DLLNAM = xa_dxf_w

SRC1 =\
 ../exp/dxf_w.c\
 ../exp/dxf_ut.c

DIRSRC1 = ../exp/:../gr/

include gcad_dll.mak

########################### EOF #######################
