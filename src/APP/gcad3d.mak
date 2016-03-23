# Link gcad3d-main                    make -f gcad3d.mak
# make all            Alle Basis-DLL's xa_XX.so linken
# make allDemos       Alle Demo-DLL's xa_XX.so linken

#OPSYS = `uname`

EXENAM  = gCAD3D


# get SRC1 SRCA SRCG SRC3 SRCB
include gcad_src.mak


# get gcad_dir_bin CC VGUI
include ../options.mak


# get debug-settings DEB CPDEB LKDEB
include deb.umak


## get SRCGUI (filList), GUICP (compile with gui), GUILIB (link with gui)
#include ../gui/gui_$(VGUI).mak


# OpenGL-binding: SRCGLB GLBLIB
include glb_$(OS).mak


# OpenGL: get OGLLIB
include ogl.mak


# get SRCOS
include srcOS.mak


# get OBJ* from SRC* and VPATH
include src2obj.mak


# fmod dlclose
LKDEF = -lm -ldl

CPFLG = $(CPDEB) -D$(VGUI) $(GLBCP)
#CPFLG = $(CPDEB) -D$(VGUI) $(GUICP) $(GLBCP)


LKFLG = $(LKDEF) $(LKDEB) -rdynamic

# OS=MSGCC32: gcc on Windows:
# LKFLG = -Wl,--export-all-symbols,--out-implib,gCAD3D.lib
# MK = mingw32-make






#=============================================================
# Mainmodule linken und das tagfile fuer alle obj3-sourcen machen
#default: $(OBJ1) $(SRC3) $(OBJG) $(OBJA) $(OBJGUI) $(OBJGLB) $(OBJOS)
default: $(OBJ1) $(SRC3) $(OBJG) $(OBJA) $(OBJGLB) $(OBJOS)
	@echo "====================== link ======================"
#	@echo "OBJ1=" $(OBJ1) "==================="
#	@echo "SRC3=" $(SRC3) "==================="
#	@echo "OBJG=" $(OBJG) "==================="
#	@echo "OBJA=" $(OBJA) "==================="
#	@echo "OBJGLB=" $(OBJGLB) "==================="
#	@echo "OBJOS=" $(OBJOS) "==================="
	cd $(gcad_dir_bin) && \
	$(CC) -o $(EXENAM)\
  $(OBJ1) $(OBJG) $(OBJA) $(OBJGLB) $(OBJOS)\
  $(GLBLIB) $(OGLLIB) $(LKFLG)\
  xa_gui.so

# xa_gui_$(VGUI).so


#ifeq "$(DEB)" "2"
#	echo "STRIP:"
#	strip $(gcad_dir_bin)/$(EXENAM)
#	rm -rf libxa_test.so.1
#endif

#	if test $(gcad_dir_bin) != $(DIRBIN); then mv -f $(gcad_dir_bin)/$(EXENAM) $(DIRBIN)/.; fi
	ctags --excmd=number -f ../tags/src_h.tag $(SRC3)

#	ctags --excmd=number -f ut.tag $(SRCG)
#	etags -f ut.tag $(SRCG) $(SRC3)
#	etags -f src_h.tag $(SRC3)



#=============================================================
# Alle Basis-DLL's xa_XX.so linken
all:
	@echo "link core-dll's, MK=|"$(MK)"|"
	find . -maxdepth 1 -name "xa_*.mak" -exec $(MK) -f {} "OS=${OS}" \;



#=============================================================
# Alle Demo-DLL's xa_XX.so linken
allDemos:
# test if plugins/ exist - else create
	mkdir -p "$(gcad_dir_bin)/plugins"
#	if [ ! -f "$(gcad_dir_bin)/plugins" ]; then\
# mkdir "$(gcad_dir_bin)/plugins";\
# fi
	find . -maxdepth 1 -name "Demo*.mak" -exec $(MK) -f {} "OS=${OS}" \;
	find . -maxdepth 1 -name "APP_*.mak" -exec $(MK) -f {} "OS=${OS}" \;
	find . -maxdepth 1 -name "PRC_*.mak" -exec $(MK) -f {} "OS=${OS}" \;
	# link PP's for PRC_cut1
	make -f ../prc/cut1/G-Code.mak
#	$(MK) -f 3Dfrom2D.mak "OS=${OS}"



#=============================================================
# unused ?
tag:
	for i in $(nam1); do \
		echo etags -f ../xa/$$i.tag $$i.c; \
		etags -f ../tags/$$i.tag $$i.c; \
	done



# die anzahl sourcezeilen ausgeben
lnr:
	wc -l $(SRC1) $(SRCG) $(SRCOS)



# Ausgabe der ObjectFileList (f xa_find ..)
# does not include sourcefilenames of xa_*.mak
objlst:
#	@echo $(OBJ1) $(OBJG)
	@echo "actDIR="`pwd`
	@echo "gcad_dir_bin="$(gcad_dir_bin)
	@echo "VPATH="$(VPATH)
	@echo "OBJ1="$(OBJ1)
	@echo "SRC3="$(SRC3)
	@echo "OBJG="$(OBJG)
	@echo "OBJA="$(OBJA)
#	@echo "OBJGUI="$(OBJGUI)
	@echo "OBJGLB="$(OBJGLB)
	@echo "OBJOS="$(OBJOS)
#	@echo $(OBJ1) $(SRC3) $(OBJG) $(OBJA) $(OBJGUI) $(OBJGLB) $(OBJOS)



#=============================================================
# write list of all sourcefiles into file files; see gcad_src.mak
srclst:
	@echo $(SRC1) > srcFiles
	@echo $(SRCG) >> srcFiles
	@echo $(SRCB) >> srcFiles
	@echo $(SRCA) >> srcFiles
	@echo $(SRCOS) >> srcFiles
	@echo\
 ../xa/*.h ../db/*h ../ci/*h ../ut/*h ../gr/*h\
 ../xa/tst*.c ../exp/*.c ../exp/*.h\
 ../prc/*.c ../prc/*.h\
 ../myAPPS/*.c ../myAPPS/*.h\
 ../APP/Demo*.c >> srcFiles
# ../gtk/*h

# ../exp/dxf_r.c ../exp/ige_r.c ../exp/obj_ut.c ../exp/stl_r.c \
# ../exp/stp_r.c ../exp/svg_w.c ../exp/vr2_r.c ../exp/wrl_ut.c 



#=============================================================
# zum generieren einer SourceListe fuer doxygen.
# make srcDoxy
srcDoxy:
	@echo $(SRC1) $(SRCG)\
 xa_batch.c \
 ../xa/*.h ../exp/*h ../db/*h ../gtk/*h ../ci/*h ../ut/*h ../gr/*h



#=============================================================
# zum generieren einer DocuFileList fuer dsearch & dmodify.
# make doclst
doclst:
	@echo ../dox/ProgramFlow.dox ../dox/gCAD3D_prog_de.dox\
  ../doc/*.htm


#=============================================================
include compile.mak

########################## eof #######################
