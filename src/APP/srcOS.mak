# gCAD3D-sourcefiles

ifeq "$(hTyp)" "Linux_x86_32"
  SRCOS = ../ut/os_uix.c ../ut/ut_os_aix.c ../ut/ctrl_os_aix.c
endif

ifeq "$(hTyp)" "Linux_x86_64"
  SRCOS = ../ut/os_uix.c ../ut/ut_os_aix.c ../ut/ctrl_os_aix.c
endif

# eof
