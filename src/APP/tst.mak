# make -f tst.mak 

# compile only - view precompiled:
# gcc -E tst_cv1.c > t1 && vi t1

DLLNAM = plugins/tst


# die hier befindlichen  Sourcen - recompile!
SRC1 =\
 ../xa/tst.c\
 ../xa/tst_exp.c\
#../xa/tst_su1.c\
#../xa/tst_cv1.c\
#../xa/tst_gtk.c\
#../xa/tst_ogl.c\


DIRSRC1 := ../xa/

include gcad_dll.mak


########################### EOF #######################
