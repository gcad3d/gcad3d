# make -f DemoGtk1.mak "OS=Linux32"


DLLNAM = plugins/DemoGtk1

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../xa/DemoGtk1.c

DIRSRC1 := ../xa/

include gcad_dll.mak

########################### EOF #######################
