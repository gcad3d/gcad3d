# . ./devbase.sh && make -f xa_print__.mak

DLLNAM = xa_print__

SRC1 =\
 ../xa/xa_print__.c\
 ../ut/ut_gl2ps.c\
 ../gl2ps/gl2ps.c

DIRSRC1 = ../exp/:../ut/:../xa/:../gl2ps/

include gcad_dll.mak

########################### EOF #######################
