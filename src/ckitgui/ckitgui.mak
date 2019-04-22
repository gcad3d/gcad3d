# auxiliary for ckitgui - testprogs; eg hello-world

ifndef gcad_dir_bin
$(error ERROR - gcad_dir_bin undefined. Start with "./do c" or do ". ../options.sh")
endif


## get VGUI BITS OS CC MK
#include ../options.mak


# get VGUI 
VGUI := $(shell cat ../gcad_gui_version)


# get SRCGUI (filList), GUICP (compile with gui), GUILIB (link with gui)
include ../gui/gui_$(VGUI).mak

# OpenGL: get OGLLIB
include ../APP/ogl.mak


# add utility-funcs
SRC1 := $(SRC1)\
 ../ut/ut_umem.c\
 ../ut/ut_umb.c\
 ../ckitgui/ckit_ut.c\
 ../ckitgui/ckit_os_ut_x.c\


#../xa/GL_C-KIT.c\



OBJ1 = $(patsubst %.c,%.o, $(notdir $(SRC1)))
OBJGUI = $(patsubst %.c,%.o, $(notdir $(SRCGUI)))

GLBCP = -I/usr/include/gtkglext-1.0 -I/usr/lib/gtkglext-1.0/include
GUICP = -D$(VGUI) $(GLBCP)
CPDEB = -DDEB -ggdb -pg
LKDEB = -ggdb -pg
LKDEF = -lm -ldl -rdynamic


VPATH = $(OUTDIR):../ckitgui/:../ut/:../gui_$(VGUI)/:.

OGLLIB = -lGLU -lGL


default: $(OBJ1)
	@echo "====================== link ======================"
	@echo "OS=" $(OS) 
	@echo "gcad_dir_bin=" $(gcad_dir_bin) 
	cd $(gcad_dir_bin) && \
	$(CC) -o $(PRJNAM) $(OBJ1) $(OGLLIB) $(LKDEB) $(LKDEF) xa_gui.so


run:
	make -f $(PRJNAM).mak
	export LD_LIBRARY_PATH=$(gcad_dir_bin) && $(gcad_dir_bin)/$(PRJNAM)

deb:
	make -f $(PRJNAM).mak
	export LD_LIBRARY_PATH=$(gcad_dir_bin) && gdb $(gcad_dir_bin)/$(PRJNAM) 


.c.o:
	@echo "cc - " $<
	$(CC) -c $(GUICP) $(CPDEB) $<
	mv -f $(@F) $(gcad_dir_bin)/.
	rm -f ../tags/$(*F).tag
	ctags -f ../tags/$(*F).tag -IMemTab $<

# eof
