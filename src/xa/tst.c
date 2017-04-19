// make -f tst.mak
/*
 *
 * Copyright (C) 2015 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
-----------------------------------------------------
TODO:
  ..

-----------------------------------------------------
Modifications:
  ..

-----------------------------------------------------
*/
/*!
\file  ../APP/tst.c
\brief plugin tests
\code
=====================================================
List_functions_start:
..
List_functions_end:
=====================================================

\endcode *//*----------------------------------------

make -f tst.mak
nmake -f tst.nmak


gCad_main
gCad_fini
tst_sel_CB


*/



#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                         // for ...


// #include <errno.h>
// #include <dlfcn.h>           // Unix: dlopen


#ifdef _MSC_VER
// die folgenden 2 Funktionen exportieren (werden vom Main gerufen):
__declspec(dllexport) int gCad_main ();
__declspec(dllexport) int gCad_fini ();
// nachfolgende externals werden aus dem Main-Exe imported:
#define extern __declspec(dllimport)
#endif



#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_geo_const.h"        // UT3D_CCV_NUL
#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_txTab.h"            // TxtTab
#include "../ut/ut_os.h"               // OS_get_bas_dir ..
#include "../ut/ut_obj.h"              // UTO_stru_2_obj
#include "../ut/ut_TX.h"               // TX_Print
#include "../ut/ut_plg.h"              // UT3D_par1_parplg
#include "../ut/func_types.h"               // SYM_..
#include "../ut/ut_ox_base.h"          // OGX_SET_INDEX
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../db/ut_DB.h"               // DB_get_..
#include "../gr/tess_su.h"             // TypTsuSur
#include "../gr/ut_GL.h"               // GL_get_Scale
#include "../gui/gui__.h"              // GUI_
#include "../xa/xa_msg.h"              // MSG_*
#include "../xa/xa_mem.h"              // memspc51, mem_cbuf1


// Externals aus ../ci/NC_Main.c:
extern double    APT_ModSiz;

// Externals aus ../xa/xa.c:
extern char      WC_modnam[128];
extern Plane     WC_sur_act;            // die aktive Plane

// protos:
  int tst_sel_CB (int src, long dl_ind);
  int tst_key_CB (int key);



//=========================================================
  int gCad_main () {
//=========================================================
// init userfunction

  int   i1;


  TX_Print("-->> gCad_main aus tst; 2016-04-18.");
  printf("-->> gCad_main aus tst;\n");

  // attach KeyIn - connect KeyIn -> func dia_KeyIn
  AP_UserKeyIn_get (tst_key_CB);


  //================================================================
  // i1 = tst__ (0);       goto L_end;   // // exit plugin
  // if(i1 == 0) return 0;     // keep plugin alive
  //================================================================
  // TEST EXPORT_DLL'S:
  // tst_print__ ();  // OS_dll_do ("xa_print__", "PRI__"
  // tst_print_pdf ();
  // tst_exp_stp ();
  // tst_exp_dxf ();
  // tst_exp_vrml2 ();
  // tst_exp_stl ();
  // tst_exp_obj ();
  // tst_exp_svg ();
  //================================================================
  // TEST IMPORT_DLL'S:
  // UI_menCB (NULL, "new");
  // tst_imp_gcad ();
  // tst_imp_dxf ();
  tst_imp_igs ();
  // tst_imp_vrml1 (); // test import VRML-1
  // tst_imp_vrml2 (); // test import VRML-2
  // tst_imp_stp (); // tst_imp_exp.c  Test Import/Export-functions
  // tst_imp_stl ();
  // tst_imp_obj ();
  // tst_imp_tess ();
  // LandXml-Import: in core (AP_ImportXML -> lxml_read); 
  //================================================================
  // AP_print__ (); // test feedbackbuffer - out -> <tempDir>/print.tmp

  L_end:
  DL_Redraw ();

  L_fini:
  gCad_fini ();

  return 0;

}

/*
//================================================================
  int UT3D_ptNr_rbsp (int *iu, CurvRBSpl *bsp, double tol1) {
//================================================================
/// \code
/// UT3D_ptNr_rbsp     estimate nr of polygonPoints for rat-B-spline
/// see also UT3D_ptNr_bsplTol UT3D_cv_rbsp
/// \endcode

  int    i2, i3, ip;
  double ao, a1, d1, d2;
  Vector vc1, vc2;


  UT3D_stru_dump (Typ_CVRBSP, bsp, "UT3D_ptNr_rbsp:\n");


  if(bsp->ptNr < 3) {*iu = 2; return 0;}

  // if(bsp->deg == 1) { ??


  //===============================================================
  // Toleranz voff: loop durch Kurve, max. Abweichung pro Laenge suchen

  *iu = 0;
  tol1 /= 8.;  // else too much bufferOverflows .. 2014-06-04


  // printf("------------ %d\n",i3);
  // aTot = 0.;
  // dTot = 0.;
  i3 = 2;
  d2 = UT3D_len_2pt (&bsp->cpTab[0], &bsp->cpTab[1]);

  // startvector
  UT3D_vc_2pt (&vc2, &bsp->cpTab[0], &bsp->cpTab[1]);

  for(i3=2; i3<bsp->ptNr; ++i3) {
      // UT3D_stru_dump(Typ_PT, &bsp->cpTab[i3], "P[%d][%d]=",i1,i2);
      // GR_Disp_pt (&bsp->cpTab[i3], SYM_STAR_S, 2);
    d1 = d2;
    vc1 = vc2;
    UT3D_vc_2pt (&vc2, &bsp->cpTab[i3-1], &bsp->cpTab[i3]);

    // opening-angle
    ao = UT3D_angr_2vc__ (&vc1, &vc2);
    ao *= (1. / bsp->wTab[i3]);
    // aTot += ao;

    // dist
    d2 = UT3D_len_2pt (&bsp->cpTab[i3-1], &bsp->cpTab[i3]);

    // get angle for rad = d1 + d2
    a1 = UT2D_angr_ciSec (tol1, d1 + d2);

    ip = ao / a1;
    *iu += ip;

      printf(" %d ao=%f d2=%f ip=%d\n",i3,UT_DEGREES(ao),d2,ip);
  }


  if(*iu < 2) *iu = 2;

  printf("ex UT3D_ptNr_rbsp %d\n",*iu);


  return 0;

}
*/

//================================================================
  int tst__ (int ii) {
//================================================================
// general test ..
// retcod   0 exit and unload
//          1 do not unload
 

  int       irc, i1, i2, iNr, oNr, typ;
  double    d1, d2, d3;
  char      *p1, s1[256], s2[256];
  int       triSiz, triNr, surSiz, surNr;
  int       sTyp[10];
  long      sTab[10], l1, dbi;
  ObjGX     *oTab=NULL, ox1;
  Triangle  *triTab;
  TypTsuSur *surTab;
  Point2    p20={-4.03350,-65.62150}; // 1691
  Point2    p21={-4.03750,-65.63950}; // 2138
  Point2    p22={-2.46750,-65.68750}; // 1694
  Point2    p23={9.55,50.6};
  Point     p30={147.802,  -53.896,    0.000};
  // Point     p32={80., -50., 0.}, p33={50., 50., 0.};
  // Point     p32={50., -50., 0.}, p33={80., 50., 0.};
  // Point     p32={85., -50., 0.}, p33={115., 50., 0.};
  // Point     p32={80., -50., 0.}, p33={80., 50., 0.};      // NORMAL
  // Point     p32={0., 10., 0.}, p33={100., 10., 0.};         // PARALL
  Point     p32={50., 0., 0.}, p33={120., 0., 0.};         // PARALL
  Point     p34, *pa;
  Vector2   v21={10., 0.};
  Vector2   v22={-15., 5.};
  Vector    v30={1., 0., 0.};
  // Vector    v32={1., 1., .0};
  // Vector    v32={0.866,    0.500, 0.};    // 30 deg
  // Vector    v32={0.707,    0.707, 0.};    // 45 deg
  Vector    v31={+0.500,   -0.866, 0.};    // 60 deg
  // Vector    v32={1.0,    1.72, 0.};    // 60 deg
  // Vector    v32={0.,    1., 0.};    // 90 deg
  // Vector    v32={-0.500,    0.866, 0.};    // 120 deg
  // Vector    v32={-0.707,    0.707, 0.};    // 135 deg
  // Vector    v32={-0.866,    0.500, 0.};    // 150 deg
  // Vector    v32={0., -1., 0.};  // 180 deg
  Vector    v32={0., 0., -1.};  // 180 deg
  Vector    vab, vcd;
  //               55.814,   76.822,    0.000,    // p1
  //             -155.238,  -50.440,    0.000,    // p2
  CurvElli  el1={ 200.000,    0.000,    0.000,    // p1
                   55.814,   76.822,    0.000,    // p2
                    0.000,    0.000,    0.000,    // pc
                    0.000,    0.000,    1.000,    // vz
                  200.000,    0.000,    0.000,    // va
                    0.000,   80.000,    0.000,    // vb
                   (short)0, (char)0, (char)0, (char)0 };  // srot,clo,uu1,uu2
  void      *obj;

  // FILE *fp;
  // int state = GL2PS_OVERFLOW, buffsize = 0, format, sort, opts, icol;
  // char ext[32], fNam[256];
  // GLint viewport[4];
  char fNam[256];

  static void  *vp1 = NULL;


  printf("\n\ntst__ 3\n");

  //----------------------------------------------------------------
  // {
    // strcpy(s1, "(3*3*sin(9)) + (4*4))");
    // printf("|%s|\n",UTX_pos_skipBrack1(&s1[1]));
    // return 1;
  // }
  //----------------------------------------------------------------

  {
    // Point    pt1={-98.320, -134.421,    0.000};  // C20
    // Point    pt2={00.00, 100.000,    0.000};     // C21
    Point   pb1, pb2;
    CurvRBSpl *obj;
    dbi = 12L;
#define oaSIZ 10
    ObjDB   parTab[oaSIZ];

    typ = DB_GetObjDat (&obj, &oNr, Typ_CV, dbi);
      printf(" typ=%d\n",typ);
    // UT3D_box_ini0 (&pb1, &pb2);
    // UT3D_box_ln (&pb1, &pb2, (Line*)obj);
    // cv1 = *(CurvPoly*)obj;
    // cv1.v0  = 248.039;
    // cv1.v1  = 24.558;
    // cv1.dir = 1;
    // UPLG_2par_iseg (&d1, &d2, 0, &cv1);
    // UPLG_2par_iseg (&d1, &d2, 1, &cv1);
    // UPLG_2par_iseg (&d1, &d2, 2, &cv1);
    // d1 = 280.;
    // d2 = 360.; 
    // CVTRM__plg_2par (&cv1, &d1, &d2, dbi, (CurvPoly*)obj);
    // i1=0;
    // AP_parent_get (&i1, parTab, oaSIZ, Typ_LN, 24L);
    UT3D_ptNr_rbsp (&i1, obj, UT_DISP_cv);
  }

  //----------------------------------------------------------------
  // {
    // int      ptNr;
    // CurvBSpl cv1;
    // Point    pta[100];
    // typ = DB_GetObjDat (&obj, &oNr, Typ_CV, 20L);
    // cv1 = *(CurvBSpl*)obj;
    // cv1.v0 = 0.984481;
    // cv1.v1 = 1.;
    // UT3D_stru_dump (typ, &cv1, " cv1 ");
    // irc = UT3D_pta_bsp  (&ptNr, pta, &cv1, 100, UT_DISP_cv);
      // printf(" irc=%d ptNr=%d\n",irc,ptNr);
    // UT3D_nstru_dump (Typ_PT, ptNr, pta, "pta");
    // GR_Disp_pt (&pt1, SYM_STAR_S, ATT_COL_RED);

  // }
  //----------------------------------------------------------------
  {
  // CNTF_test__ ();
  // PRCV_test__ ();
  // i1 = UT___ptNr_ell (&el1, UT_DISP_cv);
  // i1 = 10;
  // pa = (Point*) MEM_alloc_tmp (i1 * sizeof(Point));
  // _npt_ell (i1, pa, &el1);
  // UT3D_npt_ell (i1, pa, &el1);
    // GR_Disp_cv (pa, i1, 8);

    // GR_Disp_ln2 (&p32,&p33,5);
    // irc =   _pt_int2pt2pt_lim(&p34, &d1,&d2, &p30,&p31,&p32,&p33,UT_TOL_pt);
    // printf(" irc=%d\n",irc);


    //----------------------------------------------------------------
    // PRCV_test__ ();
    return 1;
  }

  // test message
  // MSG_STD_ERR (func_not_impl, "E001 - %d",4);
  // return 0;

/*
  {
  int    sx, sy, dx, dy, typ, oNr;
  long   dbi, dli;
  double dsx, dsy, dsz;
  Point  p1, p2;

    dbi = 1L;
    dli = 0L;

    // get data for note l1
    typ = DB_GetObjDat (&obj, &oNr, Typ_Note, dbi);
      UT3D_stru_dump (typ, obj, " N1 ");

    // DL_txtgetInfo (&typ, &p1, &sx, &sy, &dx, &dy, dli);
    irc = GR_img_get_dbi (&typ, &p1, &sx, &sy, &dx, &dy, dbi);

      UT3D_stru_dump (Typ_PT, &p1, "p1");
      printf(" sx=%d sy=%d\n",sx,sy);
      GR_Disp_pt (&p1, SYM_STAR_S, ATT_COL_RED);


    // change Textpoint --> Screencoords
    GL_Uk2Sk (&dsx, &dsy, &dsz, p1.x, p1.y, p1.z);
      // printf(" LU(ScrCoords) dsx=%f dsy=%f dsz=%f\n",dsx,dsy,dsz);

    dsx += sx + dx;
    dsy += sy + dy;

    // change RO --> UserCoords
    GL_Sk2Uk (&p2.x, &p2.y, &p2.z,  dsx, dsy, dsz);
      GR_Disp_pt (&p2, SYM_STAR_S, ATT_COL_RED);
      // UT3D_stru_dump(Typ_PT, &p2, " RO=");

    // use RO
    // UT3D_box_extend (pb1, pb2, &p2);   ++pNr;


  } return 0;
*/

  //----------------------------------------------------------------
  // testdll__ (); return 0;
  goto L_test_func;
  goto L_test_ReScale;
  goto L_test_tess1;

{

  Point    pb1, pb2, pOri;
  Memspc   impSpc;


  // strcpy (fNam, "/mnt/serv1/Devel/GITHUB/gcad3d/gCAD3D/tmp/SERV_VR1_nor_frac_graphics_model_9.tess");
  // strcpy (fNam, "/mnt/serv1/Devel/GITHUB/gcad3d/gCAD3D/tmp/Data_warner1.tess");
  strcpy (fNam, "/mnt/serv1/Devel/GITHUB/gcad3d/gCAD3D/tmp/Data_samp_anilin.tess");
  // strcpy (fNam, "/mnt/serv1/Devel/GITHUB/gcad3d/gCAD3D/tmp/Data_Haus1.tess");
  // strcpy (fNam, "/mnt/serv1/Devel/GITHUB/gcad3d/gCAD3D/tmp/Data_unknown.tess");


  // load tess-model from file
  if(OS_checkFilExist (fNam, 1) == 0) return -1;
  TSU_imp_tess (&impSpc, fNam);

  // // find box of tess-model in memory
  // tess_box_get (&pb1, &pb2, &impSpc);

  // // get origin from box of tess-model
  // tess_origin_box (&pOri, &pb1, &pb2);

  // // subract origin of tess-model in memory
  // tess_origin_set__ (&impSpc, &pOri);

  // analyze tess-model
  tess_analyz__ (&impSpc);
  // tess_res_CB__ (&impSpc, DXFW_tess_CB);

  UME_free (&impSpc);

  return 1;

}


/*
  TX_Print ("XXXXXXXXXXXXX q=Quit, l=reLoad; u=unLoad;  tst__ %d\n",ii);
  printf("CXXXXXXXXXXXXXXXXXXXXXXXX  tst__ %d\n",ii);
    printf(" dll1=%p\n",dll1);
    printf(" UT3D_VECTOR_Y = %f %f %f\n",
           UT3D_VECTOR_Y.dx, UT3D_VECTOR_Y.dy, UT3D_VECTOR_Y.dz);


  // if(ii == 1) {
    // UI_PRI__ (FUNC_UNLOAD);
    // // i1 = _OS_dll__ (&dll1,  2, NULL); // unload
    // return 0;
  // }
// 
  // UI_PRI__ (FUNC_EXEC); // 0=start, 2=unload


  return 1;
  // do not yet unload (gui is active)
*/


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// xa_print__.mak ../xa/print__.c
// xa_print_pdf.mak
// ../exp/print_pdf.c  int PRI_PDF__ (char* fnam)
  // i1 = OS_dll_do ("xa_print__", "PRI__", "abc");

  // i1 = OS_ck_SW_is_installed ("ps2pdf");
  // printf(" ps2pdf = %d\n",i1);
  // GUI_printer__ ("C:\Users\freiter\AppData\Roaming\gCAD3D\tmp\print.eps");
  // GUI_printer__ ("/mnt/serv1/Devel/GITHUB/gcad3d/gCAD3D/tmp/print.eps");


/*
//XXXXXX GL2PS XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  // see GL_Feedback
  // pdf svg tex eps
  sprintf(fNam, "%s.eps", "test");
  gl2ps_print3 (fNam, WC_modnam, "gCAD3D", GL_Redraw);
    // GL_Redraw (); // display();   // redraw
*/



//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
/*
  sprintf(s1, "%swin.bmp",OS_get_tmp_dir());
  sprintf(s2, "%swin.jpg",OS_get_tmp_dir());

  // create BMP-file of active OpenGL-window
  bmp_save__ (s1);
  // create JPG-file from BMP-file
  OS_jpg_bmp (s2, s1);
*/


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
/*
  // AP_print__ (); // test feedbackbuffer - out -> <tempDir>/print.tmp

  // AP_print_work3 ();
  // AP_print_psv3 (1, "0,0", "1", "2");
*/

  AP_print_work2 ();
  AP_print_psv2 (1, "0,0", "1", "2");
  // system("v /mnt/serv1/Devel/GITHUB/gcad3d/gCAD3D/tmp/print.eps");
return 0;



//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  // irc = UT3D_pt_int2pt2pt_lim (&p34, NULL, NULL, &p30, &p31, &p32, &p33, 0.05);
  // printf(" irc=%d\n",irc);
  // UT3D_stru_dump (Typ_PT, &p34, "p34");

  // tst_tst_1 (&v31, &v32);

  // UT3D_vc_perp2vc (&v33, &v31, &v32);
  // UT3D_stru_dump (Typ_VC, &v33, "v33");

  // d1 = UT3D_angr_3vcn_CCW (&v33, &v31, &v32);
  // printf(" d1=%f %f\n",d1, UT_DEGREES(d1));

  // TX_Print(" plugin tst start ..");





//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  L_test_func:    printf("L_test_func: \n");
    {
      double d1;
      Point  pt1={-2985.076, 6834.254,    0.0};     // 0.09
      // Point  pt2={-2174.711, 9139.990,    0.0};     // 0.37
      // Point  pt2={3253.244, 9056.007,    0.000};
      // Point  pt2={-4834.796, 2087.975,   0.0};
      // Point  pt2={-4834.796, 2087.975,   0.0};
      // Point  pt2={-4834.796, 2087.975,   0.0};
      Point  pt2={-4834.796, 2087.975,   0.0};
      Circ   *ci1;
      ObjAto ato1;
      char   s1[256];

      // ci1 = DB_get_CI (21L);

      // ATO_getSpc__ (&ato1);
      strcpy(s1, "DISP_PT OFF");
      ATO_wTab_str (&ato1, s1);

    }
    return 0;



//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  L_test_ReScale:    printf("L_test_ReScale: \n");
    {
      Point  pb1, pb2;

      // get box of active model
      UT3D_box_mdl__ (&pb1, &pb2, -1);

      // view box
      DL_ReScale_box (&pb1, &pb2);

    }
    return 0;


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  L_test_tess1:    printf("L_test_tess1: \n");


  sTyp[0] = Typ_SUR;
  // sTyp[0] = Typ_SOL;
  sTab[0] = 20L;

  TSU_tess_sTab (&oTab, sTyp, sTab, 1);

  // init
  ox1.typ = TYP_FuncInit;
  tess_analyz_CB (&ox1);


  // resolv all obj's, call tess_analyz_CB
  tess_res_CB__ (&oTab, tess_analyz_CB);


  // exit
  ox1.typ = TYP_FuncExit;
  tess_analyz_CB (&ox1);


/*
  // space for triangles --> triTab  (12bytes/Tria)
  triTab = (Triangle*)memspc501;
  triSiz = sizeof(memspc501) / sizeof(Triangle);
    printf(" triSiz=%d\n",triSiz);

  surTab = (TypTsuSur*)memspc51;
  surSiz = sizeof(memspc51) / sizeof(TypTsuSur);
    printf(" surSiz=%d\n",surSiz);

  // get triangles from spc1 --> triTab
  // (triangles are pointers into tesselated data !)
  surNr = surSiz;
  triNr = triSiz;
  irc = TSU_tsu2tria__ (surTab, &surNr, surSiz, triTab, &triNr, triSiz, oTab);
    printf(" _tsu2tria__ irc=%d surNr=%d triNr=%d\n",irc,surNr,triNr);

    // dump_Triangle
    // for(ii=0;ii<triNr;++ii) UT3D_stru_dump (Typ_Tria,&triTab[ii],"tria");
*/


  // free tesselated data
  TSU_free ();



  return 0;

}


//=========================================================
  int gCad_fini () {
//=========================================================
// dll being unloaded - reset Input, kill all open windows !!!

  printf("gCad_fini tst \n");

  TX_Print("---<<<<<< exit tst__ <<<<<<<---\n");

  // tst_ogl_reset ();
  // VcSel_exit ();

  AP_User_reset ();

  return 0;

}



//=========================================================
  int tst_sel_CB (int src, long dl_ind) {
//=========================================================
// userselection callback

  int   irc, typ;
  long  dbi;
  // char  so[512];
  char  *so;
  // ObjGX o1;


  printf("tst_sel_CB src=%d ind=%ld\n",src,dl_ind);

  return 0;

}


//=========================================================
  int tst_key_CB (int key) {
//=========================================================
// userKeyin callback


  printf("tst_key_CB %d\n",key);

  switch (key) {

    case GUI_KeyEsc:
    case 'q':
      // DL_hili_off (-1L);         //  -1 = unhilite all
      // DL_Redraw ();               // Redraw DispList
      gCad_fini ();
      // PED_CB1 (NULL, "Exit");
      break;

    case 'l':
      tst__ (0);
      break;

    case 'u':
      tst__ (1);
      break;

    case 't':
      printf(" GUI_Win_exist=%d\n",GUI_Win_exist ("Export/Print"));

      break;

  }

  return 0;

}



//======== EOF =========
