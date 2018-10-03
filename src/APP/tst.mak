# . ../options.sh && make -f tst.mak 

# compile only - view precompiled:
# gcc -E tst_cv1.c > t1 && vi t1

DLLNAM = plugins/tst


# die hier befindlichen  Sourcen - recompile!
SRC1 =\
 ../xa/tst.c\
 ../xa/tst1.c\
#../xa/tst_exp.c\
#../xa/tst_surfaces.c\
#../xa/tst_new.c\
#../xa/tst_msh2d.c\
#../xa/tst_ut_tin.c\
#../xa/tst_ut_GL.c\
#../xa/tst_geo.c\
#../xa/tst_mem.c\

#../ut/ut_prcv.c\
#../ut/ut_cntf.c\
#../xa/tst_su1.c\
#../xa/tst_cv1.c\
#../xa/tst_gtk.c\
#../xa/tst_ogl.c\


DIRSRC1 := ../xa/:../ut/:../gl2ps/

include gcad_dll.mak


########################### EOF #######################
