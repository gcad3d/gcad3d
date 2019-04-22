# Link & run:
#   . ../options.sh && make run -f hello-world.mak


# name of this makefile
PRJNAM = hello-world


SRC1 =\
 ../ckitgui/hello-world.c\
 ../ckitgui/tst_gui_gl1.c


include ../ckitgui/ckitgui.mak

# eof
