# . ../options.sh && make -f DemoPlugin_Mousemove.mak


DLLNAM = plugins/DemoPlugin_Mousemove

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../APP/DemoPlugin_Mousemove.c

DIRSRC1 := ../APP/

include gcad_dll.mak

########################### EOF #######################
