# . ./devbase.sh && make -f Demo_gui1.mak


DLLNAM = plugins/Demo_gui1

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../APP/Demo_gui1.c

DIRSRC1 := ../APP/:../xa/

include gcad_dll.mak

########################### EOF #######################
