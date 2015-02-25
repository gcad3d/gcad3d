# Create the gui-lib        make -f xa_gui__.mak


# get OS OUTDIR CC VGUI (Version GTK)
include ../options.mak

# get SRCGUI GUICP GUILIB
include ../gui/gui_$(VGUI).mak
OBJGUI = $(patsubst %.c,%.o, $(notdir $(SRCGUI)))


default:
	@echo build ..
	make -f gcad_gui_lnk.mak


delete:
	@echo delete ..
	cd $(OUTDIR) && rm -f $(OBJGUI)


srclst:
	make -f gcad_gui_lnk.mak srclst


# link after changing VGUI in ../options.mak
link:
	@echo make the link ..
	cd $(OUTDIR) ;\
 if test "$(VGUI)" = "gtk2"; then\
 echo "Gtk2";\
 ln -fs xa_gui_gtk2.so xa_gui.so;\
 fi;\
 if test "$(VGUI)" = "gtk3"; then\
 echo "Gtk3";\
 ln -fs xa_gui_gtk3.so xa_gui.so;\
 fi


# EOF
