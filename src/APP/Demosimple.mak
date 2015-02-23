# make -f Demosimple.mak "OS=Linux32"


DLLNAM = plugins/Demosimple

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../xa/Demosimple.c

DIRSRC1 := ../xa/

include gcad_dll.mak

########################### EOF #######################
