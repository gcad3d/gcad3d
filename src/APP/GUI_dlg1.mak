# Link & run:
# . ./devbase.sh && make -f GUI_dlg1.mak
# ./GUI_dlg1.sh
# ./do gui
#
# DIR_DEV     home of sourcefiles, scripts, linkjobs, packages
# DIR_EXE     home of objects, executables, libraries
#

$(info -)
$(info - entering GUI_dlg1.mak)


# get VGUI  - eg gtk3
VGUI := $(shell cat ../gcad_gui_version)


SRC1 =\
 ../gui_$(VGUI)/GUI_dlg1.c\
 ../gui_$(VGUI)/GUI_aux1.c\
 ../ut/ut_txTab.c\
 ../ut/ut_umem.c\
 ../ut/ut_umb.c\
 ../ut/deb_prt.c

# get SRCOS
SRCOS = ../ut/os__.c GUI_os.c
#SRCOS = ../ut/os_uix.c


EXENAM  = GUI_dlg1_$(VGUI)


include build_gui_exe.mak

# eof
