# returns BITS OS OUTDIR CC MK 

# Version GTK; gtk2 or gtk3
#VGUI = gtk2
VGUI = gtk3



# do not modify ....
BITS := $(shell getconf LONG_BIT)

ifeq "$(BITS)" "32"
OS := Linux32
else
OS := Linux64
endif

OUTDIR := ../../bin$(OS)

CC = gcc
MK = make


# get first char of kernel-release, 2 or 3
#VEROS := $(shell expr substr `uname -r` 1 1)

# eof
