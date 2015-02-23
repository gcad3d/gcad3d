# make -f DemoKondensatorplatte.mak

DLLNAM = plugins/DemoKondensatorplatte

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../xa/DemoKondensatorplatte.c

DIRSRC1 := ../xa/

include gcad_dll.mak

########################### EOF #######################
