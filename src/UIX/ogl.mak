# ogl.mak
# # set CPOGL LKOGL


ifndef OSTYP
$(error ERROR lib_ogl.mak - OSTYP undefined. Start with ". ./devbase.sh")
endif


# OpenGL: get OGLLIB
#include ogl.mak
ifeq "$(OSTYP)" "MSYS"
CPOGL = -I/mingw64/include/gtkgl-2.0
LKOGL = -L/mingw64/lib -lopengl32 -lglu32 -lgtkgl-2.0.dll

else
CPOGL = -I/usr/include/gtkgl-2.0
LKOGL = -lGLU -lGL -lgtkgl-2.0 

endif


$(info - CPOGL = $(CPOGL))
$(info - LKOGL = $(LKOGL))
$(info - exit ogl.mak)


# eof
