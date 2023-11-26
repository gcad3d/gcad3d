# . ./devbase.sh && make -f DemoPlugin_Resolv.mak


DLLNAM = plugins/DemoPlugin_Resolv

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../APP/DemoPlugin_Resolv.c

DIRSRC1 := ../APP/

include gcad_dll.mak

########################### EOF #######################
