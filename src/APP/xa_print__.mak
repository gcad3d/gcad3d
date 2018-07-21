# . ../options.sh && make -f xa_print__.mak

DLLNAM = xa_print__

SRC1 =\
 ../xa/xa_print__.c\
 ../gl2ps/gl2ps.c

DIRSRC1 = ../exp/:../gr/:../xa/:../gl2ps/

include gcad_dll.mak

########################### EOF #######################
