# make -f DemoPlugin_Hili.mak


DLLNAM = plugins/DemoPlugin_Hili

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../xa/DemoPlugin_Hili.c

DIRSRC1 := ../xa/

include gcad_dll.mak

########################### EOF #######################
