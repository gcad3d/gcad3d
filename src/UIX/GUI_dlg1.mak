# Link & run:
# . ./devbase.sh && make -f ../UIX/GUI_dlg1.mak
#
# gcad_dir_bin     home of objects, executables, libraries
#

$(info -)
$(info - entering GUI_dlg1.mak)


# get VGUI  - eg gtk3
VGUI := $(shell cat ./gcad_gui_version_$(OSTYP))
$(info - VGUI = $(VGUI))


SRC1 =\
 ../gui_$(VGUI)/GUI_dlg1.c\
 ../gui_$(VGUI)/GUI_aux1.c\
 ../ut/ut_txTab.c\
 ../ut/ut_umem.c\
 ../ut/ut_umb.c\
 ../ut/deb_prt.c


# get SRCOS
ifeq "${OSTYP}" "UIX"
SRCOS = ../ut/os__.c ../ut/GUI_os_uix.c ../ut/os_exe_file.c
endif

ifeq "${OSTYP}" "MSYS"
SRCOS = ../ut/os__.c ../ut/GUI_os_ms.c ../ut/os_exe_file.c
endif


$(info - SRCOS = $(SRCOS))


EXENAM  = GUI_dlg1_$(VGUI)


include ./build_gui_exe.mak

# eof
