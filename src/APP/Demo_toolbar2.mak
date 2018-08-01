# . ../options.sh && make -f Demo_toolbar2.mak


DLLNAM = plugins/Demo_toolbar2

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../APP/Demo_toolbar2.c

DIRSRC1 := ../APP/

include gcad_dll.mak

########################### EOF #######################
