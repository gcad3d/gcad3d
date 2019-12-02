# . ../options.sh && make -f xa_dxf_r.mak 

DLLNAM = xa_dxf_r

SRC1 =\
 ../exp/dxf_r.c\
 ../exp/dxf_ut.c

DIRSRC1 = ../exp/

include gcad_dll.mak

########################### EOF #######################
