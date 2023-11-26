# . ./devbase.sh && make -f DemoAssy1.mak

DLLNAM = plugins/DemoAssy1

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
 ../APP/DemoAssy1.c

DIRSRC1 := ../APP/

include gcad_dll.mak

########################### EOF #######################
