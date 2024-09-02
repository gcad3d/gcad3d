# . ./devbase.sh && make -f DemoPlugin_Selection.mak


DLLNAM = plugins/DemoPlugin_Selection

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../APP/DemoPlugin_Selection.c

DIRSRC1 := ../APP/

include gcad_dll.mak

########################### EOF #######################
