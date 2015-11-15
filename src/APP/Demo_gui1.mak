# make -f Demo_gui1.mak


DLLNAM = plugins/Demo_gui1

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../xa/Demo_gui1.c

DIRSRC1 := ../xa/

include gcad_dll.mak

########################### EOF #######################
