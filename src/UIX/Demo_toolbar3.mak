# . ./devbase.sh && make -f Demo_toolbar3.mak


DLLNAM = plugins/Demo_toolbar3

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../APP/Demo_toolbar3.c

DIRSRC1 := ../APP/

include gcad_dll.mak

########################### EOF #######################
