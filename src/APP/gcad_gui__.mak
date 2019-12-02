# Create the gui-lib        make -f xa_gui__.mak
#
# used symbols: gcad_dir_bin VGUI


# get VGUI 
VGUI := $(shell cat ../gcad_gui_version)


# get SRCGUI GUICP GUILIB
include ../gui/gui_$(VGUI).mak


OBJGUI = $(patsubst %.c,%.o, $(notdir $(SRCGUI)))
VPATH = $(gcad_dir_bin):../gui/:../gui_$(VGUI)/


default:
	@echo "build guilib for $(VGUI) .."
	make -f gcad_gui_lnk.mak


delete:
	@echo "delete guilibs .."
	cd $(gcad_dir_bin) && rm -f $(OBJGUI)


srclst:
	@echo "srclst:" $(VGUI)
	@echo $(SRCGUI) >> srcFiles

add_srclst:
	@echo "add_srclst:"


# link after changing VGUI in ../options.mak
link:
	@echo make the link ..
	cd $(gcad_dir_bin) ;\
 if test "$(VGUI)" = "gtk2"; then\
 echo "Gtk2";\
 ln -fs xa_gui_gtk2.so xa_gui.so;\
 fi;\
 if test "$(VGUI)" = "gtk3"; then\
 echo "Gtk3";\
 ln -fs xa_gui_gtk3.so xa_gui.so;\
 fi


# EOF
