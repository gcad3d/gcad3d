# . ./devbase.sh && make -f Demo_export.mak


DLLNAM = plugins/Demo_export

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../APP/Demo_export.c

DIRSRC1 := ../APP/

include gcad_dll.mak

########################### EOF #######################
