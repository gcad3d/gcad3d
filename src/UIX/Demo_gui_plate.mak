# . ./devbase.sh && make -f Demo_gui_plate.mak

DLLNAM = plugins/Demo_gui_plate

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
	../APP/Demo_gui_plate.c

DIRSRC1 := ../APP/

include gcad_dll.mak

########################### EOF #######################
