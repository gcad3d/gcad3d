# . ../options.sh && make -f tst.mak 

# compile only - view precompiled:
# gcc -E tst_cv1.c > t1 && vi t1

DLLNAM = plugins/tst


# die hier befindlichen  Sourcen - recompile!
SRC1 =\
 ../APP/tst.c\
 ../APP/tst1.c\
 ../APP/tst_exp.c\


# ../APP/tst_mdl__.c              // ../xa/mdl__.c
# ../APP/tst_mdlmp.c              // ../xa/mdlmp.c
#../APP/tst_ut_GR.c\
#../APP/tst_ut_GL.c\
#../APP/tst_rbspl.c\
#../APP/tst_ut_geo.c\



#../APP/tst_ut_DL.c\

# ../APP/tst_edmpt.c == ../APP/xa_edmpt.c
#../APP/tst_ut_objtab.c\
#../ut/ut_prcv_db.c\
#../APP/tst_surfaces.c\
#../APP/tst_new.c\
#../APP/tst_msh2d.c\
#../APP/tst_ut_tin.c\
#../APP/tst_ut_geo.c\
#../APP/tst_mem.c\

#../ut/ut_prcv.c\
#../ut/ut_cntf.c\
#../APP/tst_su1.c\
#../APP/tst_cv1.c\
#../APP/tst_gtk.c\


DIRSRC1 := ../xa/:../ut/:
#DIRSRC1 := ../xa/:../ut/:../gl2ps/

include gcad_dll.mak


########################### EOF #######################
