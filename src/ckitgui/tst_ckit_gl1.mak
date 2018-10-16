# Link & run:
#   . ../options.sh && make run -f tst_ckit_gl1.mak
# debug: 
#   . ../options.sh && make deb -f tst_ckit_gl1.mak


# name of this makefile
PRJNAM = tst_ckit_gl1


SRC1 =\
 ../ckitgui/tst_ckit_gl1.c\
 ../ckitgui/tst_gui_gl1.c


include ../ckitgui/ckitgui.mak

# eof
