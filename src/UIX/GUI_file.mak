# Link & run:
# . ./devbase.sh && make -f ../UIX/GUI_file.mak
#
# gcad_dir_bin     home of objects, executables, libraries
#

$(info -)
$(info - entering GUI_file.mak)


# get VGUI  - eg gtk2
VGUI := $(shell cat ./gcad_gui_version_$(OSTYP))


SRC1 =\
 ../gui_$(VGUI)/GUI_file.c\
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


EXENAM  = GUI_file_$(VGUI)


include ../UIX/build_gui_exe.mak

# eof
