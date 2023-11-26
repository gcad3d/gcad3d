# build GUI_exe's (SRC1 SRCOS) +  gtk(VGUI)

$(info - enter build_gui_exe.mak)


OBJ1 = $(patsubst %.c,%.o, $(notdir $(SRC1)))
OBJOS = $(patsubst %.c,%.o, $(notdir $(SRCOS)))


# get GUICP GUILIB
include ../gui/gui_$(VGUI).mak


VPATH = "$(gcad_dir_bin)" ../ut/ ../gui/ ../gui_$(VGUI)
#vpath %.o "$(gcad_dir_bin)"
#vpath %.c ../ut/ ../gui/ ../gui_$(VGUI)
.SUFFIXES: .c .o


# get debug-settings DEB CPDEB LKDEB
include deb.mak

# get CPGUI LKGUI
include ../gui/gui_$(VGUI).mak



CPFLG = $(CPDEB) -D$(VGUI) $(CPGUI) -Wno-implicit
LKFLG = $(LKDEB) $(LKGUI) -lm -ldl



default: $(OBJ1) $(OBJOS)
	@echo "........... build $(EXENAM) ..........."
	@echo "OBJ1 = $(OBJ1)"
	@echo "OBJOS = $(OBJOS)"
	@echo gcad_dir_bin = $(gcad_dir_bin)
	@echo VPATH = $(VPATH)
	cd "$(gcad_dir_bin)" && \
	$(CC) -o $(EXENAM) $(OBJ1) $(OBJOS) $(GUILIB) $(LKFLG)
	@echo
# Test:
# $(gcad_dir_bin)$(EXENAM) /p2/fwork/t1.dat /p2/fwork/devel/gcad3d/gCAD3D/cfg/dir.lst '*' 'Speichern'
	@echo


.c.o:
	@echo "........... compile $< ..........."
	$(CC) -c $(GUICP) $(CPFLG) $<
	mv -f $(@F) "$(gcad_dir_bin)."
	ctags -f ../tags/$(*F).tag -IMemTab --tag-relative=yes $<

# eof
