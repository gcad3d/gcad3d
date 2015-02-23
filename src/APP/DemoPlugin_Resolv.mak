# make -f DemoPlugin_Resolv.mak


DLLNAM = plugins/DemoPlugin_Resolv

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../xa/DemoPlugin_Resolv.c

DIRSRC1 := ../xa/

include gcad_dll.mak

########################### EOF #######################
