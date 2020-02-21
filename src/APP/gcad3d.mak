# compile / link gcad3d
#
# make -f gcad_gui__.mak       build core
# make all                     build all basis-DLL's xa_XX.so
# make allDemos                build all demo-DLL's xa_XX.so
#
# USING SYMBOLS (. ../options.sh):
# gcad_dir_dev gcad_dir_bin
#

#OPSYS = `uname`

EXENAM  = gCAD3D


# get sourcefiles - SRC1 SRC3 SRCA SRCB SRCG
include gcad_src.mak


# get VGUI 
VGUI := $(shell cat ../gcad_gui_version)


# get debug-settings DEB CPDEB LKDEB
include deb.mak


# OpenGL-binding: SRCGLB GLBLIB
#include glb_$(OS).mak
#SRCGLB = ../xa/GL_C-KIT.c



# OpenGL: get OGLLIB
#include ogl.mak
OGLLIB = -lGLU -lGL


# get SRCOS
#include srcOS.mak
SRCOS = ../ut/os_uix.c ../ut/ut_os_aix.c ../ut/ctrl_os_aix.c


# get OBJ* from SRC* and VPATH
include src2obj.mak


ifndef gcad_dir_bin
$(error ERROR - gcad_dir_bin undefined. Start with "./do c" or do ". ../options.sh")
endif


# fmod dlclose
LKDEF = -lm -ldl

CPFLG = $(CPDEB) -D$(VGUI) $(GLBCP) -Wno-implicit
#CPFLG = $(CPDEB) -D$(VGUI) $(GUICP) $(GLBCP)


LKFLG = $(LKDEF) $(LKDEB) -rdynamic

# OS=MSGCC32: gcc on Windows:
# LKFLG = -Wl,--export-all-symbols,--out-implib,gCAD3D.lib
# MK = mingw32-make






#=============================================================
# create core
default: $(OBJ1) $(SRC3) $(OBJG) $(OBJA) $(OBJGLB) $(OBJOS)
	@echo "====================== link ======================"
	@echo "gcad_dir_bin=" $(gcad_dir_bin) 
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
	ctags -f ../tags/src_h.tag $(SRC3)

#	ctags --excmd=number -f ut.tag $(SRCG)
#	etags -f ut.tag $(SRCG) $(SRC3)
#	etags -f src_h.tag $(SRC3)



#=============================================================
# Alle Basis-DLL's xa_XX.so linken
all:
	@echo "link core-dll's .."
# create makeFiles.lst = list of makefiles of "xa_*.mak"
	rm -f makeFiles.lst
	find . -maxdepth 1 -name "xa_*.mak" -exec echo {} >> makeFiles.lst \;
# read list, build ..
	while read line; do \
	echo $$line; \
	make -f $$line; \
	if [ $$? -ne 0 ]; then exit 1; fi \
	done < makeFiles.lst



#=============================================================
# Alle Demo-DLL's xa_XX.so linken
allDemos:
	@echo "link plugins .."
# test if plugins/ exist - else create
	mkdir -p "$(gcad_dir_bin)/plugins"
	mkdir -p "$(gcad_dir_bin)plugins/cut1"
# create makeFiles.lst = list of makefiles
	rm -f makeFiles.lst
	find . -maxdepth 1 -name "Demo*.mak" -exec echo {} >> makeFiles.lst \;
	find . -maxdepth 1 -name "PRC_*.mak" -exec echo {} >> makeFiles.lst \;
# read list, build ..
	while read line; do \
	echo $$line; \
	make -f $$line; \
	if [ $$? -ne 0 ]; then exit 1; fi \
	done < makeFiles.lst
	# link PP's for PRC_cut1
	make -f ../prc/cut1/G-Code.mak
	if [ $$? -ne 0 ]; then exit 1; fi \


##	if [ ! -f "$(gcad_dir_bin)/plugins" ]; then\
## mkdir "$(gcad_dir_bin)/plugins";\
## fi
#	#find . -maxdepth 1 -name "Demo*.mak" -exec make -f {} \;
#	find . -maxdepth 1 -name "PRC_*.mak" -exec make -f {} \;
##	find . -maxdepth 1 -name "APP_*.mak" -exec make -f {} \;
##	find . -maxdepth 1 -name "PRC_*.mak" -exec make -f {} "OS=${OS}" \;
#	# link PP's for PRC_cut1
#	mkdir -p "$(gcad_dir_bin)plugins/cut1"
#	make -f ../prc/cut1/G-Code.mak
##	$(MK) -f 3Dfrom2D.mak "OS=${OS}"



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
# used for cscope, DocuDevel (doxygen)
srclst:
	@echo $(MK)
	@echo $(SRC1) > srcFiles
	@echo $(SRCG) >> srcFiles
	@echo $(SRCB) >> srcFiles
	@echo $(SRCA) >> srcFiles
	@echo $(SRCOS) >> srcFiles
	@echo ../xa/*.h ../db/*h ../ci/*h ../ut/*h ../gr/*h ../exp/*.h >> srcFiles
	@echo ../xa/test*.c ../xa/tst*.c ../inf/*.c >> srcFiles
	@echo ../myAPPS/*.c ../myAPPS/*.h >> srcFiles
	@echo ../APP/Demo*.c >> srcFiles
	@echo ../prc/*.c ../prc/*.h >> srcFiles
	find . -maxdepth 1 -name "xa_*.mak" -exec make -f {} "add_srclst" \;



#=====================================================================
add_srclst:
	@echo $(SRC1) >> srcFiles



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
