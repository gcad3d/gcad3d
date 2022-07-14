# Link & run:
# . ../options.sh && make -f GUI_gMsh1.mak
# ./do gui
#
# DIR_DEV     home of sourcefiles, scripts, linkjobs, packages
# DIR_BIN     home of objects, executables, libraries
#


# get VGUI  - eg gtk3
#VGUI := $(shell cat ../gcad_gui_version)
VGUI := gtk2


# ../gui_gtk2/GUI_gMsh1.c
SRC1 =\
 ../gui_$(VGUI)/GUI_gMsh1.c\
 ../ut/ut_txTab.c\
 ../ut/ut_umem.c\
 ../ut/ut_umb.c\
 ../ut/deb_prt.c

# get SRCOS
SRCOS = ../ut/os_uix.c


EXENAM  = GUI_gMsh1_$(VGUI)


OBJ1 = $(patsubst %.c,%.o, $(notdir $(SRC1)))
OBJOS = $(patsubst %.c,%.o, $(notdir $(SRCOS)))


# get SRCGUI GUICP GUILIB
include ../gui/gui_$(VGUI).mak

# get debug-settings DEB CPDEB LKDEB
include deb.mak

VPATH = $(OUTDIR):../ut/:../xa/:../gui/:../gui_$(VGUI)/:.

CPFLG = $(CPDEB) -D$(VGUI) -Wno-implicit

# fmod dlclose
LKDEF = -lm -ldl


LKFLG = $(LKDEF) $(LKDEB)



default: $(OBJ1) $(OBJOS)
	@echo "........... build $(EXENAM) ..........."
	cd $(gcad_dir_bin) && \
	$(CC) -o $(EXENAM) $(OBJ1) $(OBJOS) $(GUILIB) $(LKFLG)
	@echo
# Test:
#	$(gcad_dir_bin)$(EXENAM) /p2/fwork/t1.dat /p2/fwork/devel/gcad3d/gCAD3D/cfg/dir.lst '*' 'Speichern'
	@echo


.c.o:
	$(CC) -c $(GUICP) $(CPFLG) $<
	mv -f $(@F) $(gcad_dir_bin).
	ctags -f ../tags/$(*F).tag -IMemTab --tag-relative=yes $<

# eof

