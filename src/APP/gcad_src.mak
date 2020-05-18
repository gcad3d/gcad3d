# gCAD3D-sourcefiles
# returns SRC1 SRC3 SRCA SRCB SRCG

#ifeq "$(OS)" "Linux32"
#  SRCOS = ../ut/ut_os_aix.c ../lic/ut_lic_aix.c
#endif
#
#ifeq "$(OS)" "Linux64"
#  SRCOS = ../ut/ut_os_aix.c ../lic/ut_lic_aix.c
#endif
#
#ifeq "$(OS)" "MS32"
#  SRCOS = ../ut/ut_os_w32.c ../lic/ut_lic_w32.c
#endif



# sources for the GUI (not used in batch-progs)
SRCG =\
  ../xa/xa_main.c\
  ../xa/xa_ctrl.c\
  ../xa/xa_ui.c\
  ../xa/xa_cad_ui.c\
  ../xa/xa_cad_inp.c\
  ../xa/xa_cad_ccv_ui.c\
  ../xa/xa_cad_ed1.c\
  ../xa/xa_sele.c\
  ../xa/xa_mod_gui.c\
  ../xa/xa_brw.c\
  ../xa/xa_gmvo.c\
  ../xa/xa_ted.c\
  ../xa/xa_meas.c\
  ../xa/xa_mod_att.c\
  ../xa/xa_ico.c\
  ../xa/xa_objmenu.c\
  ../xa/xa_objvwr.c\
  ../xa/xa_aux.c\
  ../xa/xa_undo.c\
  ../xa/xa_del.c\
  ../xa/ui_tex.c\
  ../xa/xa_tex.c\
  ../xa/xa_ui_gr.c\
  ../xa/xa_vwpt.c\
  ../xa/xa_tra.c\
  ../xa/xa_plu.c\
  ../xa/xa_prg.c\
  ../xa/xa_proc.c\
  ../xa/xa_objLst.c\
  ../xa/xa_joint.c\
  ../xa/catalog_gui.c\
  ../xa/opar.c\
  ../xa/srcu.c\
  ../gui/gui_base.c\

# ../xa/xa_edmpt.c\
# ../xa/xa_ui_nc.c\
# ../xa/xa_smd.c

# sources for Batch-progs only
SRCB = \
  ../xa/xa_batch.c\
  ../ut/ut_TX.c


# sourcen; bei denen keine abhaengigkeit erforderlich ist.
# ../gtk/ut_gtk1.c  ../gtk/ut_gtked.c ../gtk/ut_gtktx.c
#
SRC1 =\
  ../xa/xa.c\
  ../xa/xa_msg.c\
  ../xa/xa_obj_txt.c ../xa/xa_mod.c     ../xa/xa_edi__.c   ../xa/xa_ed.c\
  ../xa/xa_ed_mem.c  ../xa/xa_src.c     ../xa/xa_ato.c     ../xa/xa_grp.c\
  ../xa/xa_ga.c      ../xa/xa_dll.c     ../xa/xa_app.c     ../xa/xa_appDat.c\
  ../xa/catalog.c    ../xa/ckint_fac.c  ../xa/cvoff2.c     ../xa/mdlfn.c\
  ../db/ut_DB.c      ../db/ut_dbo.c\
  ../ci/NC_Main.c    ../ci/NC_apt.c     ../ut/ut3d_npt.c   ../ut/ut2d_npt.c\
  ../ut/ut_gtypes.c  ../ut/ut_ox_base.c ../ut/ut_transform.c ../ut/ut_ui_TX.c\
  ../ut/ut_pspl.c    ../ut/ut_bspl.c    ../ut/ut_rbspl.c   ../ut/ut_bsp_approx.c\
  ../ut/cvClot.c     ../ut/utemp.c      ../ut/cirMoeb.c    ../ut/ut_col.c\
  ../ut/ut_srv.c     ../ut/ut_sru.c     ../ut/ut_dump.c    ../ut/ut_err.c\
  ../ut/ut_surstd.c  ../ut/ut_sbsp.c    ../ut/ut_srbsp.c   ../ut/ut_objtab.c\
  ../ut/ut_bez.c     ../ut/ut_rbez.c    ../ut/ut_tria.c    ../ut/ut_quat.c\
  ../ut/ut_plg.c     ../ut/ut_cvtrm.c   ../ut/ut_elli.c    ../ut/ut_cvApp.c\
  ../ut/ut_sdc.c     ../ut/tri_orient.c ../ut/ut_project.c\
  ../ut/ut_tess.c    ../ut/ut_hatch.c   ../ut/ut_log.c\
  ../ut/ubs.c        ../ut/ubscrv.c     ../ut/ubssur.c     ../ut/sbsu1.c\
  ../ut/u3d.c        ../ut/ut_cvoff.c   ../ut/ut_msh.c     ../ut/ut_face.c\
  ../gr/tess_su.c    ../gr/ut_tess_su.c ../gr/tess_ut.c    ../gr/tess_vr_ut.c\
  ../gr/ut_GLU.c     ../gr/glbt.c       ../ut/ut_cntf.c\
  ../ut/ut_prcv__.c  ../ut/ut_prcv_db.c\
  ../ut/ut_geo2d.c   ../ut/ut_geo3d.c   ../ut/ut_iTab.c    ../ut/ut_dbf.c\
  ../ut/ut_obj.c     ../ut/ut_txfil.c   ../ut/ut_txt.c     ../ut/ut_umb.c\
  ../ut/ut_math.c    ../ut/ut_umem.c    ../ut/ut_mem.c     ../ut/ut_box.c\
  ../ut/ut_txTab.c   ../ut/ut_int.c     ../ut/ut_uti.c     ../ut/byteorder.c\
  ../ut/ut_memTab.c  ../ut/ut_memTab1.c ../ut/ut_os__.c    ../ut/ut_BitTab.c\
  ../ut/deb_prt.c

# ../ut/ut_prcv_f.c - replaced by ../ut/ut_prcv_db.c = PRCV in memSpc
# ../xa/tess2dg.c
# ../ut/ut_prcv.c
# ../ut/ut_cntf.c
# ../exp/ut_exp.c\
# ../xa/xa_cont.c     replaced by ../ut/ut_cntf.c
# ../ut/cv_parl_pln.c replaced by ../xa/cvoff2.c

# sources of applications; not used in batch
SRCA =\
  ../exp/bmp_ut.c\
  ../exp/csv_ut.c    ../exp/print_pdf.c\
  ../exp/ds3_r.c     ../exp/ige_w.c     ../exp/prt_w.c\
  ../exp/lwo_r.c\
  ../exp/lxml.c\
  ../gr/ut_gr.c      ../gr/ut_GL.c      ../gr/ut_DL.c      ../gr/ut_gtx.c\
  ../gr/gru.c        ../gr/ut_dim.c\
  ../gr/gr_sSym.c    ../gr/gl_solids.c

# ../wc/wcut.c      ../wc/wc_pock.c    ../wc/ut_nc.c\
# ../ci/NC_drill.c\
# ../xa/nct.c\
# nct.c\

# ../ut/ut_tess.c\
# ../ut/ut_surstd.c\

# Files in plugins:
# ../exp/ige_r.c ../exp/svg_w.c ../exp/vr2_r.c ../exp/wrl_ut.c
# ../exp/dxf_w.c ../exp/dxf_r.c ../exp/dxf_ut.c
# ../exp/stl_r.c ../exp/obj_ut.c ../exp/stp_w.c ../exp/stp_r.c


# wichtige includefiles fuer etags (damit sind Inlinefunctions im tagFile)
SRC3 =\
  ../ut/ut_geo.h     ../ut/ut_txt.h    ../ut/ut_BitTab.h\
  ../ut/ut_types.h   ../ut/gr_types.h  ../ut/AP_types.h\
  ../ut/ut_mem.h     ../ut/ut_umem.h   ../db/ut_DB.h      ../xa/xa_sele.h 
# ../gtk/ut_gtk.h



# alte wcut-sourcen: ../wc_old     etags -f wc_old.tag ../wc_old/*.c
# alte ut-sourcen:   ../ut/ut_string.c


# EOF
