# make -f DemoPlugin_Dialog.mak


DLLNAM = plugins/DemoPlugin_Dialog

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../xa/DemoPlugin_Dialog.c

DIRSRC1 := ../xa/

include gcad_dll.mak

########################### EOF #######################
