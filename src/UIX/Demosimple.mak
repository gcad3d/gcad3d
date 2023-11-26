# . ./devbase.sh && make -f Demosimple.mak


DLLNAM = plugins/Demosimple

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../APP/Demosimple.c

DIRSRC1 := ../APP/

include gcad_dll.mak

########################### EOF #######################
