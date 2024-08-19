# . ./devbase.sh && make -f DemoPlugin_Create.mak


DLLNAM = plugins/DemoPlugin_Create

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../APP/DemoPlugin_Create.c\
  ../APP/UPDATE.c

DIRSRC1 := ../APP/

include gcad_dll.mak

########################### EOF #######################
