# make -f DemoKondensatorplatte.mak

DLLNAM = plugins/Demo_gui_plate

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../xa/Demo_gui_plate.c

DIRSRC1 := ../xa/

include gcad_dll.mak

########################### EOF #######################
