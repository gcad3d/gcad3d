# create primary executable gCAD3D                       default:
# create all core-libs xa_*.so                           all:
# create all demo-libs plugin/Demo_*.so                  allDemos:
#
# USING SYMBOLS (. ./devbase.sh):


ifndef OSTYP
$(error ERROR gcad3d.mak - OSTYP undefined. Start with ". ./devbase.sh")
endif

ifndef gcad_dir_bin
$(error ERROR gcad3d.mak - gcad_dir_bin undefined. Start with ". ./devbase.sh")
endif


# get VGUI 
VGUI := $(shell cat ./gcad_gui_version_$(OSTYP))


$(info -)
$(info - entering gcad3d.mak $(OSTYP) $(VGUI))

EXENAM  = gCAD3D


# get sourcefiles - SRC1 SRC3 SRCA SRCB SRCG
include gcad_src.mak


# get debug-settings DEB CPDEB LKDEB
include deb.mak


# get CPGUI LKGUI
include ../gui/gui_$(VGUI).mak


# OpenGL: get CPOGL LKOGL
include ogl.mak


# get SRCOS
include srcOS.mak


# only for srclst
#SRC_MS_OS = ../ut/os_w32.c ../ut/ut_os_w32.c ../ut/ctrl_os_w32.c


# get OBJ* from SRC* and VPATH
include src2obj.mak


ifeq "$(OSTYP)" "MSYS"
# Windows-Msys2/ucrt64
CPFLG = $(CPDEB)  $(CPGUI) $(CPOGL) -D$(VGUI) -O0 -Wno-implicit
LKFLG = $(LKDEB) $(LKOGL) $(LKGUI) -Wl,-export-all-symbols -Wl,--out-implib,gCAD3D.a
FTYPGUI = a

else
# Linux
CPFLG = $(CPDEB) $(CPGUI) -D$(VGUI) -O0 -Wno-implicit
LKFLG = $(LKDEB) $(LKGUI) $(LKOGL) -lm -ldl -rdynamic -Wl,--unresolved-symbols=ignore-all
FTYPGUI = so

endif



#=============================================================
# create primary executable gCAD3D
default: $(OBJ1) $(SRC3) $(OBJG) $(OBJA) $(OBJGLB) $(OBJOS)
#default: xa_main.o xa_ui.o
	@echo "====================== link $(EXENAM) ======================"
	@echo "gcad_dir_bin=" $(gcad_dir_bin) 
#	@echo "OBJ1=" $(OBJ1) "==================="
#	@echo "SRC3=" $(SRC3) "==================="
#	@echo "OBJG=" $(OBJG) "==================="
#	@echo "OBJA=" $(OBJA) "==================="
#	@echo "OBJGLB=" $(OBJGLB) "==================="
#	@echo "OBJOS=" $(OBJOS) "==================="
	mkdir -p "$(gcad_dir_bin)"
	cd "$(gcad_dir_bin)" && $(CC)\
 $(OBJG) $(OBJ1) $(OBJA) $(OBJGLB) $(OBJOS)\
 gCAD3D_gui_$(VGUI).$(FTYPGUI)\
 $(LKFLG)\
 -o $(EXENAM)



#=============================================================
# create all core-libs xa_*.so
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
# create all demo-libs plugin/Demo_*.so
allDemos:
	@echo "link plugins .."
# test if plugins/ exist - else create
	mkdir -p "$(gcad_dir_bin)/plugins"
# create makeFiles.lst = list of makefiles
	rm -f makeFiles.lst
	find . -maxdepth 1 -name "Demo*.mak" -exec echo {} >> makeFiles.lst \;
# read list, build ..
	while read line; do \
	echo $$line; \
	make -f $$line; \
	if [ $$? -ne 0 ]; then exit 1; fi \
	done < makeFiles.lst


#=============================================================
# Alle processes xa_XX.so linken
allPrcs:
	@echo "link processes .."
# test if plugins/ exist - else create
	mkdir -p "$(gcad_dir_bin)/plugins"
	mkdir -p "$(gcad_dir_bin)plugins/cut1"
# create makeFiles.lst = list of makefiles
	rm -f makeFiles.lst
	find . -maxdepth 1 -name "PRC_*.mak" -exec echo {} > makeFiles.lst \;
# read list, build ..
	while read line; do \
	echo $$line; \
	make -f $$line; \
	if [ $$? -ne 0 ]; then exit 1; fi \
	done < makeFiles.lst
# link PP's for PRC_cut1
	@echo "====================== link processes .."
	touch ../prc/cut1/G-Code.c
	make -f ../prc/cut1/G-Code.mak
	if [ $$? -ne 0 ]; then exit 1; fi
	@echo "link processes done OK"



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
# Link all extra-plugins
allExtras:
	@echo "link extras .."
	make -f gMsh1.mak
	if [ $$? -ne 0 ]; then exit 1; fi
	@echo "link extras done OK"


#=============================================================
# unused ?
tag:
	for i in $(nam1); do \
		echo etags -f ../xa/$$i.tag $$i.c; \
#		etags -f ../tags/$$i.tag $$i.c; \
		etags -f tags/$$i.tag $$i.c; \
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
	@echo $(SRCG) >  srcFiles
	@echo $(SRC1) >> srcFiles
	@echo $(SRCB) >> srcFiles
	@echo $(SRCA) >> srcFiles
	@echo $(SRCOS) >> srcFiles
	@echo $(SRC_MS_OS) >> srcFiles
	@echo ../xa/*.h ../db/*h ../ci/*h ../ut/*h ../gr/*h ../exp/*.h >> srcFiles
	@echo ../APP/*.c ../APP/*.h >> srcFiles
	@echo ../prc/*.c ../prc/*.h >> srcFiles
	@echo ../prc/cut1/*.c ../prc/cut1/*.h >> srcFiles
	@echo ../inf/*.c >> srcFiles
	find . -maxdepth 1 -name "xa_*.mak" -exec make -f {} "add_srclst" \;
	# testprogs
	@echo ../myAPPS/*.c ../myAPPS/*.h >> srcFiles
	@echo ../msh3d/*.c ../msh3d/*.h >> srcFiles
	@echo ../msh2d/*.c ../msh2d/*.h >> srcFiles
	@echo ../suop/*.c ../suop/*.h >> srcFiles



#=============================================================
# write list of all sourcefiles - without APPS,prc - into file srcFiles;
# used for cscope, DocuDevel (doxygen)
prjlst:
	@echo $(SRCG) >> srcFiles
	@echo $(SRC1) > srcFiles
	@echo $(SRCA) >> srcFiles
	@echo $(SRCOS) >> srcFiles
	@echo ../xa/*.h ../db/*h ../ci/*h ../ut/*h ../gr/*h ../exp/*.h >> srcFiles
	@echo ../inf/*.c >> srcFiles



#=====================================================================
add_srclst:
	@echo $(SRC1) >> srcFiles



##=============================================================
## zum generieren einer SourceListe fuer doxygen.
## make srcDoxy
#srcDoxy:
#	@echo $(SRC1) $(SRCG)\
# xa_batch.c \
# ../xa/*.h ../exp/*h ../db/*h ../gtk/*h ../ci/*h ../ut/*h ../gr/*h
#
#
#
##=============================================================
## zum generieren einer DocuFileList fuer dsearch & dmodify.
## make doclst
#doclst:
#	@echo ../dox/ProgramFlow.dox ../dox/gCAD3D_prog_de.dox\
#  ../doc/*.htm
#
#
#=============================================================
include compile.mak

########################## eof #######################
