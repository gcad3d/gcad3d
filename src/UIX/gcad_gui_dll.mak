# gcad_gui_lib.mak
# create the core-gui-lib xa_gui_<VGUI>.so


$(info - entering gcad_gui_lnk.mak)

# get VGUI 
VGUI := $(shell cat ./gcad_gui_version_$(OSTYP))


# get SRCGUI (filList)
include ../gui/gui_src.mak


# get CPGUI LIBGUI
include ../gui/gui_$(VGUI).mak


DLLNAM = gCAD3D_gui_$(VGUI)


SRC1 := $(SRCGUI)


DIRSRC1 = ../gui:../gui_$(VGUI):../ut


include gcad_dll.mak


########################### EOF #######################
