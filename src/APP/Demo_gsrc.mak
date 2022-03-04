# . ../options.sh && make -f Demo_gsrc.mak


DLLNAM = plugins/Demo_gsrc

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../APP/Demo_gsrc.c

DIRSRC1 := ../xa/:../ut/

include gcad_dll.mak

########################### EOF #######################
