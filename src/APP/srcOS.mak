# gCAD3D-sourcefiles

ifeq "$(OS)" "Linux32"
  SRCOS = ../ut/ut_os_aix.c ../ut/ctrl_os_aix.c
# SRCOS = ../ut/ut_os_aix.c ../ut/ctrl_os_f.c
endif

ifeq "$(OS)" "Linux64"
  SRCOS = ../ut/ut_os_aix.c ../ut/ctrl_os_aix.c
# SRCOS = ../ut/ut_os_aix.c ../lic/ut_lic_aix.c
endif

# eof
