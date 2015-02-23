# make -f DemoPlugin_Mousemove.mak


DLLNAM = plugins/DemoPlugin_Mousemove

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../xa/DemoPlugin_Mousemove.c

DIRSRC1 := ../xa/

include gcad_dll.mak

########################### EOF #######################
