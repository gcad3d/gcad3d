# Create the gui-lib        make -f xa_gui__.mak
# Link all GUI_*.mak
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


#srclst:
add_srclst:
	@echo "add_srclst:" $(VGUI)
	@echo $(SRCGUI) >> srcFiles
	@echo ../gui_$(VGUI)/GUI_*.c >> srcFiles


# link all GUI_* executables
GUI_exe:
# create makeFiles.lst = list of makefiles of "xa_*.mak"
	@echo "link GUI_* executables .."
	rm -f makeFiles.lst
	find . -maxdepth 1 -name "GUI_*.mak" -exec echo {} >> makeFiles.lst \;
# read list, build ..
	while read line; do \
	echo "........... start $$line ........... "; \
	make -f $$line; \
	if [ $$? -ne 0 ]; then exit 1; fi \
	done < makeFiles.lst




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
