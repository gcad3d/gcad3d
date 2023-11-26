# include-File to link gCAD3D-Plugins (DLLs)
#
# USING SYMBOLS:
# gcad_dir_bin SRC1
# 


ifndef OSTYP
$(error ERROR gcad_gui_lib.mak - OSTYP undefined. Start with ". ./devbase.sh")
endif

ifndef gcad_dir_bin
$(error ERROR gcad_gui_lib.mak - gcad_dir_bin undefined.)
endif


ifndef SRC1
$(error ERROR gcad_gui_lib.mak - SRC1 undefined.)
endif

 
# get VGUI 
VGUI := $(shell cat ./gcad_gui_version_$(OSTYP))

## eg Linux_x86_64
#OSTYP := $(shell cat gcad_ostyp)


$(info - entering gcad_dll.mak $(OSTYP) $(VGUI))


# debug-settings DEB CPDEB LKDEB
include deb.mak


# get SRCOS
include srcOS.mak


# get CPGUI LKGUI
include ../gui/gui_$(VGUI).mak


# OpenGL: get CPOGL LKOGL
include ogl.mak



ifeq "$(SRCTYP)" "cpp"
OBJ1 := $(patsubst %.cpp,%.o, $(notdir $(SRC1)))
else
OBJ1 := $(patsubst %.c,%.o, $(notdir $(SRC1)))
endif


# objects-directory AND source-directories
VPATH := "$(gcad_dir_bin)" $(DIRSRC1)

#CC = gcc


# compile and linkOptions; default = 64-bit "Linux_x86_64"
$(info - OSTYP = $(OSTYP))
$(info - HTYP = $(HTYP))
$(info - OBJ1 = $(OBJ1))


ifeq "$(HTYP)" "Linux_x86_64"
FTYPGUI = so
CPFLG = -fPIC $(CPDEB) -D$(VGUI) $(CPGUI) $(CPOGL) -Wno-implicit
CPPFLG = -fPIC $(CPDEB)
LKFLG = $(LKDEF) -shared -export-dynamic
endif

ifeq "$(HTYP)" "Linux_i386"
FTYPGUI = so
CPFLG = -fPIC $(CPDEB) -D$(VGUI) $(CPGUI) $(CPOGL) -Wno-implicit
CPPFLG = -fPIC $(CPDEB)
LKFLG = -shared -export-dynamic
endif

ifeq "$(HTYP)" "Linux_i586"
FTYPGUI = so
CPFLG = -fPIC $(CPDEB) -D$(VGUI) $(CPGUI) $(CPOGL) -Wno-implicit
CPPFLG = -fPIC $(CPDEB)
LKFLG = -shared -export-dynamic
endif

ifeq "$(HTYP)" "Linux_i686"
FTYPGUI = so
CPFLG = -fPIC $(CPDEB) -D$(VGUI) $(CPGUI) $(CPOGL) -Wno-implicit
CPPFLG = -fPIC $(CPDEB)
LKFLG = -shared -export-dynamic
endif

# Raspi:
ifeq "$(HTYP)" "Linux_armv7l"
FTYPGUI = so
CPFLG = -fPIC $(CPDEB) -D$(VGUI) $(CPGUI) $(CPOGL)
CPPFLG = -fPIC $(CPDEB)
LKFLG = $(LKDEF) -shared
endif

# Windows-Msys2/ucrt64
ifeq "$(HTYP)" "MINGW_x86_64"
  # get first 10 chars of DLLNAM
  FTYP = $(shell expr substr $(DLLNAM) 1 10)
  $(info - FTYP = $(FTYP))
  ifeq "$(FTYP)" "gCAD3D_gui"
    # MSYS - lib - GUI
    FTYPGUI = a
  else    
    # MSYS - core-dll's and plugins
    FTYPGUI = dll
  endif
  ifeq "$(FTYPGUI)" "dll"
    # MSYS - dynamic-dll
    CPFLG = $(CPDEB) -D$(VGUI) $(CPGUI) $(CPOGL) -Wno-implicit
    LKFLG = -shared  $(LKOGL) gCAD3D.a             #-Wl,-femulated-tls
  else
    # MSYS - lib - GUI  ".a"
    CPFLG = $(CPDEB) -D$(VGUI) $(CPGUI) $(CPOGL)
    LKFLG =
  endif
endif


ifndef FTYPGUI
$(error ERROR gcad_dll.mak - HTYP $(HTYP) unsupported.")
endif



#=====================================================================
#default:
default: $(OBJ1)
	@echo ====================== Link $(DLLNAM).$(FTYPGUI) ======================
	@echo "OBJ1: $(OBJ1)"
#	@echo "OSTYP: $(OSTYP)"
#	@echo "VGUI: $(VGUI)"
#	@echo "VPATH: $(VPATH)"

# Linux:
	if test "$(FTYPGUI)" = "so"; then\
  cd $(gcad_dir_bin) &&\
  $(CC) -o $(DLLNAM).$(FTYPGUI) $(OBJ1) $(LKFLG);\
  fi

# MSYS-GUI:
	if test "$(FTYPGUI)" = "a"; then\
  cd "$(gcad_dir_bin)" &&\
  ar rcs $(DLLNAM).$(FTYPGUI) $(OBJ1);\
  fi

# MSYS-dyn-dll:
	if test "$(FTYPGUI)" = "dll"; then\
  cd "$(gcad_dir_bin)" &&\
  $(CC) -o $(DLLNAM).$(FTYPGUI) $(OBJ1) $(LKFLG);\
  fi



#	mv -f $(gcad_dir_bin)/$(DLLNAM).so $(DIRBIN)/.
#ifeq "$(DEB)" "2"
#	strip -o $(gcad_dir_bin)/$(DLLNAM).$(FTYPGUI) $(gcad_dir_bin)/$(DLLNAM).$(FTYPGUI)
#endif

#	if test $(gcad_dir_bin) != $(DIRBIN); then mv -f $(gcad_dir_bin)/$(DLLNAM).$(FTYPGUI) $(DIRBIN)/.; fi


#=====================================================================
# link after changing VGUI in ./devbase.mak
gui_link:
	@echo make the link ..
	cd $(gcad_dir_bin) &&\
  ln -fs $(DLLNAM).so gCAD3D_gui.so;\


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
