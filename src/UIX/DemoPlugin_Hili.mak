# . ./devbase.sh && make -f DemoPlugin_Hili.mak


DLLNAM = plugins/DemoPlugin_Hili

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../APP/DemoPlugin_Hili.c

DIRSRC1 := ../APP/

include gcad_dll.mak

########################### EOF #######################
