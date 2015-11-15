# make -f Demo_toolbar1.mak


DLLNAM = plugins/Demo_toolbar1

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../xa/Demo_toolbar1.c

DIRSRC1 := ../xa/

include gcad_dll.mak

########################### EOF #######################
