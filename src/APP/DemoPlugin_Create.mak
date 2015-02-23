# make -f DemoPlugin_Create.mak


DLLNAM = plugins/DemoPlugin_Create

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../xa/DemoPlugin_Create.c

DIRSRC1 := ../xa/

include gcad_dll.mak

########################### EOF #######################
