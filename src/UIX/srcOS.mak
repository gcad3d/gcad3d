# gCAD3D-OS-sourcefiles

ifndef OSTYP
$(error ERROR gcad3d.mak - OSTYP undefined. Start with ". ./devbase.sh")
endif


ifeq "$(OSTYP)" "MSYS"
  SRCOS =\
 ../ut/os__.c\
 ../ut/ut_os_ms.c\
 ../ut/os_dll_ms.c\
 ../ut/os_exe_file.c\
 ../ut/ctrl_os_ms.c

else
  SRCOS =\
 ../ut/os__.c\
 ../ut/ut_os_uix.c\
 ../ut/os_dll_uix.c\
 ../ut/os_exe_file.c\
 ../ut/ctrl_os_uix.c
endif

#


$(info - exit srcOS.mak $(OSTYP) $(SRCOS))

# eof
