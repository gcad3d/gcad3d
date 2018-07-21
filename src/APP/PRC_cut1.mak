# . ../options.sh && make -f PRC_cut1.mak

DLLNAM = plugins/PRC_cut1

SRC1 =\
 ../prc/prc_cut1__.c\
 ../prc/prc_cut1_hist.c

DIRSRC1 := ../prc/

include gcad_dll.mak

########################### EOF #######################
