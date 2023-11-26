# . ./devbase.sh && make -f Demo_appDat1.mak


DLLNAM = plugins/Demo_appDat1

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../APP/Demo_appDat1.c

DIRSRC1 := ../APP/

include gcad_dll.mak

########################### EOF #######################
