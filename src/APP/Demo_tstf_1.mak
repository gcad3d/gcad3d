# . ../options.sh && make -f Demo_tstf_1.mak 


DLLNAM = plugins/Demo_tstf_1


# die hier befindlichen  Sourcen - recompile!
SRC1 =\
 ../APP/Demo_tstf_1.c\


DIRSRC1 := ../xa/:../ut/

include gcad_dll.mak


########################### EOF #######################
