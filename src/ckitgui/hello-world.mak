# Link & run:
#   make run -f hello-world.mak
# Menu on Ubuntu: export UBUNTU_MENUPROXY=


# name of this makefile
PRJNAM = hello-world


SRC1 =\
 ../ckitgui/hello-world.c


#include ../ckitgui/tst__.mak
include ../ckitgui/ckitgui.mak

# eof
