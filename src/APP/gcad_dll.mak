# include-File to link gCAD3D-Plugins (DLLs)
# gcad_dir_bin must be set (. ../options.sh)
#
# used symbols: gcad_dir_bin VGUI

# get VGUI 
VGUI := $(shell cat ../gcad_gui_version)


# debug-settings DEB CPDEB LKDEB
include deb.umak


# OpenGL: get OGLLIB
include ogl.mak


# get SRCOS
include srcOS.mak


ifndef gcad_dir_bin
$(error ERROR - gcad_dir_bin undefined. Start with "./do c" or do ". ../options.sh")
endif


ifeq "$(SRCTYP)" "cpp"
OBJ1 := $(patsubst %.cpp,%.o, $(notdir $(SRC1)))
else
OBJ1 := $(patsubst %.c,%.o, $(notdir $(SRC1)))
endif

VPATH := $(gcad_dir_bin):$(DIRSRC1)

CC = gcc


ifeq "$(hTyp)" "Linux_i386"
FTYP = so
CPFLG = -fPIC $(CPDEB) -D$(VGUI) $(GUICP) $(GLBCP)
CPPFLG = -fPIC $(CPDEB)
LKFLG = -shared -export-dynamic
endif

ifeq "$(hTyp)" "Linux_x86_64"
FTYP = so
CPFLG = -fPIC $(CPDEB) -D$(VGUI) $(GUICP) $(GLBCP)
CPPFLG = -fPIC $(CPDEB)
LKFLG = $(LKDEF) -shared
endif

#ifeq "$(OS)" "MSGCC32"
#FTYP = dll
#gcad_dir_bin=../bin$(OS)
#LKFLG = -shared gCAD3D.lib
#endif



#=====================================================================
#default:
default:	$(OBJ1)
	@echo "gcad_dll build $(DLLNAM) gui = $(VGUI) .."
	@echo $(VPATH)
	@echo $(OBJ1)
	@echo ====================== Link ======================
	cd $(gcad_dir_bin) &&\
	$(CC) -o $(DLLNAM).$(FTYP) $(LKDEB) $(OBJ1)\
  $(GUILIB) $(OGLLIB) $(LKFLG)
#	mv -f $(gcad_dir_bin)/$(DLLNAM).so $(DIRBIN)/.

#ifeq "$(DEB)" "2"
#	strip -o $(gcad_dir_bin)/$(DLLNAM).$(FTYP) $(gcad_dir_bin)/$(DLLNAM).$(FTYP)
#endif

#	if test $(gcad_dir_bin) != $(DIRBIN); then mv -f $(gcad_dir_bin)/$(DLLNAM).$(FTYP) $(DIRBIN)/.; fi



#=====================================================================
# Ausgabe der ObjectFileList (f xa_find ..)
objlst:
	@echo $(OBJ1)


#=====================================================================
add_srclst:
	@echo $(SRC1) >> srcFiles


#=====================================================================
srclst:
	@echo $(SRC1) > srcFiles


#=====================================================================
# ACHTUNG: case of filename muss genau stimmen, else xx.o Datei nicht gefunden
include compile.mak

# EOF
