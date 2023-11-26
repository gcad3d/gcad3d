# Create the <gui>.dll        make -f xa_gui__.mak
# Create all GUI_*.exe                             GUI_exe:
# Create the Linkfile to the <gui>.so              link:


$(info - entering gcad_gui_exe.mak)


ifndef OSTYP
$(error ERROR - OSTYP undefined. Start with ". ./devbase.sh")
endif


# get VGUI 
VGUI := $(shell cat ./gcad_gui_version_$(OSTYP))
$(info - VGUI = $(VGUI))


# get SRCGUI
include ../gui/gui_src_$(OSTYP).mak


OBJGUI = $(patsubst %.c,%.o, $(notdir $(SRCGUI)))
VPATH = $(gcad_dir_bin):../gui/:../gui_$(VGUI)/



# link all GUI_* executables
GUI_exe:
# create makeFiles.lst = list of makefiles of "xa_*.mak"
	@echo "- gcad_gui__.mak link GUI_* executables .."
	rm -f makeFiles.lst
	find . -maxdepth 1 -name "GUI_*.mak" -exec echo {} >> makeFiles.lst \;
# read list, build ..
	while read line; do \
	echo "........... start $$line ........... "; \
	make -f $$line default; \
	if [ $$? -ne 0 ]; then exit 1; fi \
	done < makeFiles.lst


#srclst:
add_srclst:
	@echo "add_srclst:" $(VGUI)
	@echo $(SRCGUI) >> srcFiles
	@echo ../gui_$(VGUI)/GUI_*.c >> srcFiles


$(info - exit gcad_gui__.mak)

# EOF
