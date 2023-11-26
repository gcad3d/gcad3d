# Create the <gui>.dll        make -f xa_gui__.mak
# Create all GUI_*.exe                             GUI_exe:
# Create the Linkfile to the <gui>.so              link:


$(info - entering gcad_gui__.mak)


# get VGUI 
VGUI := $(shell cat ./gcad_gui_version_$(OSTYP))


# get SRCGUI
include ../gui/gui_src_$(OSTYP).mak

# get GUICP GUILIB
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
	@echo "- gcad_gui__.mak link GUI_* executables .."
	rm -f makeFiles.lst
	find . -maxdepth 1 -name "GUI_*.mak" -exec echo {} >> makeFiles.lst \;
# read list, build ..
	while read line; do \
	echo "........... start $$line ........... "; \
	make -f $$line; \
	if [ $$? -ne 0 ]; then exit 1; fi \
	done < makeFiles.lst




# link after changing VGUI in ./devbase.mak
link:
	@echo make the link ..
	cd $(gcad_dir_bin) ;\
 ln -fs xa_gui_gtk2.so xa_gui.so;\


#if test "$(VGUI)" = "gtk2"; then\
#echo "Gtk2";\
#fi;\
#if test "$(VGUI)" = "gtk3"; then\
#echo "Gtk3";\
#ln -fs xa_gui_gtk3.so xa_gui.so;\
#fi


# EOF
