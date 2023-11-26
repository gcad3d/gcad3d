# . ./devbase.sh && make -f DemoPlugin_Dialog.mak


DLLNAM = plugins/DemoPlugin_Dialog

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../APP/DemoPlugin_Dialog.c

DIRSRC1 := ../APP/

include gcad_dll.mak

########################### EOF #######################
