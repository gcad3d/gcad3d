# gcad_gui_lib.mak
# create the core-gui-lib xa_gui_<VGUI>.so


ifndef OSTYP
$(error ERROR gcad_gui_lib.mak - OSTYP undefined. Start with ". ./devbase.sh")
endif


# get VGUI 
VGUI = $(shell cat ./gcad_gui_version_$(OSTYP))


$(info - entering gcad_gui_lib.mak $(OSTYP) $(VGUI))


# get SRCGUI (filList)
include ../gui/gui_src_$(OSTYP).mak


DLLNAM = gCAD3D_gui_$(VGUI)


SRC1 = $(SRCGUI)


DIRSRC1 = ../gui/:../gui_$(VGUI)/:../ut/


include gcad_dll.mak


########################### EOF #######################
