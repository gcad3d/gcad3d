# . ./devbase.sh && make -f DemoPlugin_KeyIn.mak


DLLNAM = plugins/DemoPlugin_KeyIn

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../APP/DemoPlugin_KeyIn.c

DIRSRC1 := ../APP/

include gcad_dll.mak

########################### EOF #######################
