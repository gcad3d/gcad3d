/*  ../inf/files.c

see ../inf/inf_help1.c

*/ 

void INF_files__ () { }  /*

../xa/xa.c                   main application functions; not depending on GUI.
../xa/xa_msg.c               functions for output of messages in different languages
../xa/xa_obj_txt.c           convert objects --> APT-Text
../xa/xa_mod.c               subModel-managment-functions without GUI
../xa/xa_edi__.c             Neutral EditorFunctions (not Gtk-specific).
../xa/xa_ed.c                Neutral EditorFunctions (not Gtk-specific).
../xa/xa_ed_mem.c            ApplicationSpecific Textfunctions in Memory
../xa/xa_src.c               check, modify, convert source-objects (text)  (SRC_)
../xa/xa_ato.c               functions handling atomicObjects.
../xa/xa_grp.c               group functions (Grp_)
../xa/xa_ga.c                PermanentAttributeList.
../xa/xa_dll.c               DLL-UtilityProgs  OS-indep.
../xa/xa_app.c               applicationInterface for plugins APP_
../xa/xa_appDat.c            application data
../xa/catalog.c              CATALOG-UtilityProgs
../xa/ckint_fac.c            intersect faces,edges UT2D_ckint_
../xa/cvoff2.c               create offset-contour for planar curves     CVOFF2_*
../db/ut_DB.c                database
../db/ut_dbo.c               database-object-functions (typ, DB-index)        DBO_
../ci/NC_Main.c              CommandInterpreter - main
../ci/NC_apt.c               CommandInterpreter - decode-functions
../ut/ut3d_npt.c             polygon (list of points) from curves, surfaces ...
../ut/ut2d_npt.c             polygon (list of points) from curves, surfaces ...
../ut/ut_gtypes.c            get|set|check object typ, index, ID
../ut/ut_ox_base.c           complex-object functions (transform, convert ..)  OGX_
../ut/ut_transform.c         transformations UTRA_
../ut/ut_ui_TX.c             TX_Print .. for messageWindow
../ut/ut_pspl.c              polynom. splines
../ut/ut_bspl.c              B-Spline-Curve-Utilityfunctions
../ut/ut_rbspl.c             rational b-spline curve functions
../ut/ut_bsp_approx.c        Approximation Polygon -> B-Spline
../ut/cvClot.c               clothoid curve functions
../ut/utemp.c                2D-functions for moebius circles
../ut/cirMoeb.c              curves (bsplines, conic sections, etc.)
../ut/ut_col.c               main application functions; not depending on GUI.
../ut/ut_srv.c               Revolved-Surface-Functions
../ut/ut_sru.c               Ruled-Surface-Functions
../ut/ut_dump.c              dump structs, objects
../ut/ut_err.c               crash handler
../ut/ut_surstd.c            Planar & Revolved-Surface-Functions
../ut/ut_sbsp.c              B-Spline-Surface-Functions
../ut/ut_srbsp.c             Rat.B-Spline-Surface-Functions
../ut/ut_objtab.c            functions for list of binary-objects (type,data) with boundingBox
../ut/ut_bez.c               Bezier-Curve - Utilityfunctions
../ut/ut_rbez.c              Rational-Bezier-Curve - Utilityfunctions
../ut/ut_tria.c              triangles (using struct Point - UTRI_)
../ut/ut_quat.c              Quaternion-utilities
../ut/ut_plg.c               PolygonCurve  with lengthTable (struct CurvPoly)
../ut/ut_cvtrm.c             CVTRM = trimmed-curve (CurvCCV) functions
../ut/ut_elli.c              ellipse
../ut/ut_cvApp.c             change polygon -> circles, lines
../ut/ut_sdc.c               SubdivisonCurve-Functions
../ut/tri_orient.c           orient triangles
../ut/ut_project.c           project
../ut/ut_tess.c              tesselated-Surfaces
../ut/ut_hatch.c             hatch 2D
../ut/ut_log.c               write log-messages into file, display file
../ut/ubs.c                  b spline surface functions
../ut/ubscrv.c               b spline curve functions
../ut/ubssur.c               bspline surface functions
../ut/sbsu1.c                evaluate sample-point on curve
../ut/u3d.c                  general 3d-geometric functions
../ut/ut_cvoff.c             offset curve on surface
../ut/ut_msh.c               Mesh-Functions MSH_.. (using struct MemTab)
../ut/ut_face.c              faces (using struct Fac3 - UFA_)
../gr/tess_su.c              Tesselate Surfaces
../gr/ut_tess_su.c           Tesselate Surfaces  (New version; old version=../gr/tess_su.c)
../gr/tess_ut.c              write STL, OBJ, WRl files
../gr/tess_vr_ut.c           general purpose export-import-functions
../gr/ut_GLU.c               aux. functions tesselate using GLU
../gr/glbt.c                 2D - Buttons in OpenGL.
../ut/ut_cntf.c              create-countour-functions CNTF_
../ut/ut_prcv__.c            polygonal_representation_of_curve  (struct CurvPrcv)
../ut/ut_prcv_db.c           PRCV database in memspc
../ut/ut_geo2d.c             2D geometric point vector line circle
../ut/ut_geo3d.c             3D geometric point vector line circle
../ut/ut_iTab.c              list of integers
../ut/ut_dbf.c               save retrieve key-value-Records in file
../ut/ut_obj.c               binary-object-manipulations  (transform, convert ..)  (UTO_)
../ut/ut_txfil.c             textfile in memory
../ut/ut_txt.c               text manipulations
../ut/ut_umb.c               memoryblocks (malloc, free, .. UMB_)
../ut/ut_math.c              math (Cholesky, Fresnel, ..)
../ut/ut_umem.c              variable-length-records in memory: reserve/connect/release .. UME_
../ut/ut_mem.c               Memory manipulations: swap, insert, delete, invert ..
../ut/ut_box.c               2D-bounding-box-, 3D-bounding-box-functions
../ut/ut_txTab.c             list of strings (0-terminated); add, find, ..
../ut/ut_int.c               intersect
../ut/ut_uti.c               auxiliary functions for integers, doubles (UTI_ UTP_ UTN_)
../ut/byteorder.c            read short,long from BigEndian/LittleEndian
../ut/ut_memTab.c            fixed-length-records in memory: add,insert,delete, realloc. .. MemTab_
../ut/ut_memTab1.c           fixed-length-records in memory: add,insert,delete, realloc. .. MemTab_
../ut/ut_os__.c              opsys functions - OS-independant.
../ut/ut_BitTab.c            array of bits
../xa/xa_main.c              main () des Interaktiven gCAD
../xa/xa_ctrl.c              communication-control
../xa/xa_ui.c                Main GUI
../xa/xa_cad_ui.c            GUI CAD toolbar
../xa/xa_cad_inp.c           CAD inputfunctions
../xa/xa_cad_ccv_ui.c        GUI simple CurveEditor
../xa/xa_cad_ed1.c           GUI CAD-GroupEditor
../xa/xa_sele.c              obj. selections
../xa/xa_mod_gui.c           GUI Model-managment-functions
../xa/xa_brw.c               Object-Browser
../xa/xa_edmpt.c             CurveEditor
../xa/xa_gmvo.c              Group-MoVe-Objects
../xa/xa_ted.c               TextureEditor
../xa/xa_meas.c              measure
../xa/xa_mod_att.c           GUI for modify surfStyle, surfTransp, surfCol
../xa/xa_ico.c               aux-functions for icons
../xa/xa_objmenu.c           objspecific mouse-menus
../xa/xa_objvwr.c            ObjectViewer  (function analyze)
../xa/xa_aux.c               import / export aux-funcs
../xa/xa_undo.c              undo-functions
../xa/xa_del.c               delete object
../xa/ui_tex.c               userInterface Textures UI_Tex_
../xa/xa_tex.c               Texture-Utility-Functions Tex_
../xa/xa_ui_gr.c             GUI graphicwindow
../xa/xa_vwpt.c              Viewport-functions
../xa/xa_tra.c               GUI Modify_Modelposition UI_Tra_
../xa/xa_plu.c               applicationInterface for plugins APP_
../xa/xa_prg.c               Programming-functions
../xa/xa_proc.c              processes, plugins, remoteProcesses
../xa/xa_objLst.c            MultiColumnList (Search/Name)
../xa/xa_joint.c             functions for joints
../xa/catalog_gui.c          CATALOG-GUI
../xa/opar.c                 Object-parent-table.
../xa/srcu.c                 Dynamic update numeric strings
../xa/xa_batch.c             dummy-functions for batch-module
../ut/ut_TX.c                TX_Print - functions without GUI
../exp/bmp_ut.c              BMP -Bitmap functions
../exp/csv_ut.c              write, read csv-files (comm-separated-values, "flat-files")
../exp/print_pdf.c           write PDF
../exp/ds3_r.c               Read 3DS-File
../exp/ige_w.c               write iges files
../exp/prt_w.c               write PS PCL5 HPGL file
../exp/lwo_r.c               read LWO file
../exp/lxml.c                read LandXML file
../gr/ut_gr.c                create graphic objects
../gr/ut_GL.c                the OpenGL functions
../gr/ut_DL.c                displayList functions
../gr/ut_gtx.c               graphic text functions
../gr/gru.c                  auxil. graphic-functions not using OpenGL
../gr/ut_dim.c               GraficDimensions-Utilities.
../gr/gl_sSym.c              display surfaces symbolic
../gr/gl_solids.c            OpenGL: create solid surfaces symbolic or shaded
../ut/ut_os_aix.c            opsys functions for unix (AIX, Linux)
../ut/ctrl_os_aix.c          remote control - opsys-specific Unix
../xa/MS_Def0.h              -
../xa/MS_Def1.h              -
../xa/gcad_version.h         -
../xa/tst_new.h              -
../xa/xa.h                   -
../xa/xa_app.h               -
../xa/xa_ato.h               -
../xa/xa_aux.h               -
../xa/xa_brw.h               -
../xa/xa_cad_ed1.h           -
../xa/xa_ed.h                -
../xa/xa_ed_mem.h            -
../xa/xa_edi__.h             -
../xa/xa_ga.h                -
../xa/xa_ico.h               -
../xa/xa_mem.h               -
../xa/xa_msg.h               -
../xa/xa_obj_txt.h           -
../xa/xa_sele.h              -
../xa/xa_tex.h               -
../xa/xa_ui.h                -
../xa/xa_cad_ui.h            -
../xa/xa_ui_gr.h             -
../xa/xa_uid.h               -
../xa/xa_undo.h              -
../db/ut_DB.h                -
../ci/NC_Main.h              -
../ci/NC_apt.h               -
../ut/AP_types.h             -
../ut/byteorder.h            -
../ut/ctrl_os.h              -
../ut/func_types.h           -
../ut/gr_types.h             -
../APP/tst_msh2d.h                -
../ut/u3d.h                  -
../ut/ubs.h                  -
../ut/ubscrv.h               -
../ut/ubssur.h               -
../ut/ut_BitTab.h            -
../ut/ut_TX.h                -
../ut/ut_bspl.h              -
../ut/ut_cast.h              -
../ut/ut_col.h               -
../ut/ut_const.h             -
../ut/ut_cvApp.h             -
../ut/ut_cvoff.h             -
../ut/ut_elli.h              -
../ut/ut_err.h               -
../ut/ut_face.h              -
../ut/ut_geo.h               -
../ut/ut_geo_const.h         -
../ut/ut_gtypes.h            -
../ut/ut_iTab.h              -
../ut/ut_itmsh.h             -
../ut/ut_math.h              -
../ut/ut_mem.h               -
../ut/ut_memTab.h            -
../ut/ut_obj.h               -
../ut/ut_os.h                -
../ut/ut_ox_base.h           -
../ut/ut_plg.h               -
../ut/ut_prcv__.h            -
../ut/ut_rbspl.h             -
../ut/ut_sbsp.h              -
../ut/ut_sur.h               -
../ut/ut_tol_const.h         -
../ut/ut_txTab.h             -
../ut/ut_txfil.h             -
../ut/ut_txt.h               -
../ut/ut_types.h             -
../ut/ut_umb.h               -
../ut/ut_umem.h              includes for Memspc - ../ut/ut_umem.c
../ut/ut_uti.h               -
../ut/utemp.h                -
../gr/tess_su.h              -
../gr/ut_DL.h                -
../gr/ut_GL.h                -
../gr/ut_GL_bitmaps.h        -
../gr/ut_gr.h                -
../gr/vf.h                   -
../exp/csv_ut.h              -
../exp/dxf_r.h               -
../exp/dxf_w.h               -
../exp/ige.h                 -
../exp/prt_w.h               -
../xa/test_3D_to_2D.c        -
../xa/test_CVOFF2.c          -
../xa/test_EDMPT.c           -
../xa/test_GMVO.c            -
../xa/test_MSH2D.c           -
../xa/test_SUR.c             -
../xa/tst.c                  plugin tests
../xa/tst1.c                 plugin tests
../xa/tst_NC_apt.c           -
../xa/tst_cvoff2.c           create offset-contour for planar curves     CVOFF2_*
../xa/tst_exp.c              -
../xa/tst_geo.c              -
../xa/tst_gl1.c              -
../xa/tst_gl2.c              -
../xa/tst_glDrawElements1.c  -
../xa/tst_glt_tess.c         aux. functions tesselate using GLU
../xa/tst_gmvo.c             Group-MoVe-Objects
../xa/tst_mem.c              -
../xa/tst_msh2d.c            functions for 2D-tesselation
../xa/tst_mshig.c            aux. functions init tesselation using GLU
../xa/tst_new.c              -
../xa/tst_surfaces.c         plugin tests
../xa/tst_tess_1.c           -
../xa/tst_ut_DB.c            -
../xa/tst_ut_GL.c            -
../xa/tst_ut_geo2d.c         -
../xa/tst_ut_geo3d.c         -
../xa/tst_ut_int.c           -
../xa/tst_ut_obj.c           -
../xa/tst_ut_objtab.c        functions for list of binary-objects (type,data) with boundingBox
../xa/tst_ut_patch.c         3D geometric point vector line circle
../xa/tst_ut_sur1.c          Surface functions (tesselate, ..)
../xa/tst_ut_surpln.c        Surface planar functions (tesselate, ..)
../xa/tst_ut_tin.c           -
../inf/Attributes.c          -
../inf/ObjectFormatConversions.c -
../inf/Objects-Create.c      -
../inf/Objects-Format.c      -
../inf/Objects-Resolve.c     -
../inf/Userinteractions.c    -
../inf/files.c               -
../inf/inf_core.c            -
../inf/inf_help1.c           -
../inf/infotext.c            Dokumentation with tags
../myAPPS/3Dfrom2D.c         -
../myAPPS/GIS1.c             -
../myAPPS/GIS__.c            Mesh-Functions MSH_.. (using struct MemTab)
../myAPPS/Gis_u1.c           -
../myAPPS/gMsh__.c           -
../myAPPS/gMsh_dum.c         -
../myAPPS/image-3d.c         image-3d
../myAPPS/t1.c               -
../myAPPS/GIS1.h             -
../myAPPS/t1.h               -
../APP/DemoAssy1.c           demo build assembly
../APP/DemoPlugin_Create.c   demo create objects
../APP/DemoPlugin_Dialog.c   example dialog
../APP/DemoPlugin_Hili.c     example hilite objects
../APP/DemoPlugin_Mousemove.c example user-callback from mousemove
../APP/DemoPlugin_Resolv.c   resolve selected objects
../APP/Demo_appDat1.c        ApplicationData-Demo  (minimum)
../APP/Demo_gui1.c           Simple GUI Demo  (minimum)
../APP/Demo_gui_anim1.c      Demoprog Animation
../APP/Demo_gui_plate.c      dialog demo
../APP/Demo_toolbar1.c       Toolbar-Demo  (minimum)
../APP/Demo_toolbar2.c       Toolbar-Demo-2
../APP/Demosimple.c          minimum example
../prc/prc_Demo1__.c         processes main
../prc/prc_cut1__.c          process cut (Numeric Control)
../prc/prc_cut1_hist.c       store active sate (position, toolnr, sourceLineNr ..)
../prc/prc_Demo1__.h         -
../prc/prc_cut1__.h          -
../exp/stp_r.c               read STeP file
../exp/dxf_r.c               read, write DXF-files
../exp/dxf_ut.c              DXF utility funtions
../exp/stl_r.c               read STL file
../exp/stp_w.c               write STeP file
../exp/vr2_r.c               read VRML-2 file
../exp/dxf_w.c               write DXF
../exp/dxf_ut.c              DXF utility funtions
../exp/obj_ut.c              read OBJ file (WaveFront)
../exp/svg_w.c               write SVG file
../exp/wrl_ut.c              read VRML-1 file
../exp/ige_r.c               read IGEs files
../xa/xa_print__.c           write / print PDF EPS SVP
../gl2ps/gl2ps.c             -
*/
