# . ../options.sh && make -f gcad_gui_lnk.mak
# Create GUI-dll for Gtk
# Delete all GUI-dll-objects: make delete -f gui_del.mak
#
# used symbols: gcad_dir_bin VGUI


# get VGUI 
VGUI := $(shell cat ../gcad_gui_version)


# get SRCGUI (filList), GUICP (compile with gui), GUILIB (link with gui)
include ../gui/gui_$(VGUI).mak


DLLNAM = xa_gui_$(VGUI)


SRC1 := $(SRCGUI)


DIRSRC1 = ../gui:../gui_$(VGUI):../ut


include gcad_dll.mak


########################### EOF #######################
