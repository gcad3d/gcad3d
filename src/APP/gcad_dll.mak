# include-File to link gCAD3D-Plugins (DLLs)
# Alle ausfuehren:
# find . -name "xa_*.mak" -exec make -f {} \;
# Alle Files listen:
# find . -name "xa_*.mak" -exec make srclst -f {} \;


# get OUTDIR CC VGUI
include ../options.mak


# debug-settings
include deb.umak


# OpenGL: get OGLLIB
include ogl.mak


# get SRCOS
include srcOS.mak


ifeq "$(SRCTYP)" "cpp"
OBJ1 := $(patsubst %.cpp,%.o, $(notdir $(SRC1)))
else
OBJ1 := $(patsubst %.c,%.o, $(notdir $(SRC1)))
endif

VPATH := $(OUTDIR):$(DIRSRC1)

CC = gcc


ifeq "$(OS)" "Linux32"
FTYP = so
CPFLG = -fPIC $(CPDEB) -D$(VGUI) $(GUICP) $(GLBCP)
CPPFLG = -fPIC $(CPDEB)
LKFLG = -shared -export-dynamic
endif

ifeq "$(OS)" "Linux64"
FTYP = so
CPFLG = -fPIC $(CPDEB) -D$(VGUI) $(GUICP) $(GLBCP)
CPPFLG = -fPIC $(CPDEB)
LKFLG = $(LKDEF) $(LKDEB) -shared
endif

#ifeq "$(OS)" "MSGCC32"
#FTYP = dll
#OUTDIR=../bin$(OS)
#LKFLG = -shared gCAD3D.lib
#endif



#=====================================================================
#default:
default:	$(OBJ1)
#	@echo "OUTDIR = "$(OUTDIR)
#@echo $(VPATH)
#	@echo $(OBJ1)
	@echo ====================== Link ======================
	cd $(OUTDIR) &&\
	$(CC) -o $(DLLNAM).$(FTYP) $(LKDEB) $(OBJ1)\
  $(GUILIB) $(OGLLIB) $(LKFLG)
#	mv -f $(OUTDIR)/$(DLLNAM).so $(DIRBIN)/.

#ifeq "$(DEB)" "2"
#	strip -o $(OUTDIR)/$(DLLNAM).$(FTYP) $(OUTDIR)/$(DLLNAM).$(FTYP)
#endif

#	if test $(OUTDIR) != $(DIRBIN); then mv -f $(OUTDIR)/$(DLLNAM).$(FTYP) $(DIRBIN)/.; fi



#=====================================================================
# Ausgabe der ObjectFileList (f xa_find ..)
objlst:
	@echo $(OBJ1)



#=====================================================================
# vi holt damit seine sourcefileliste
srclst:
	@echo $(SRC1) ../APP/Demo_gui*.c > srcFiles
#	@echo Demo_gui*.c >> srcFiles



#=====================================================================
# ACHTUNG: case of filename muss genau stimmen, else xx.o Datei nicht gefunden
include compile.mak

# EOF
