# make -f APP_GIS1.mak


DLLNAM = plugins/APP_GIS1

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../GIS1/APP_GIS1.c

DIRSRC1 := ../GIS1/

include gcad_dll.mak

########################### EOF #######################
