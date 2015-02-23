/* make -f tst.mak

Tests der Import/Export-Functions.

  fuer allg. tests use tst.c !

VRML-Export:
  Vrml1-ExportFunctions (TSU_exp__ - TSU_exp_wrlFac) sind im Kernel; daher:
  make && ./go defLoad tst3                    // tst_exp_vrml1 tst_exp_vrml2
Model .05


gCad_main
gCad_fini
gCad_sel_CB

*/



#ifdef _MSC_VER
#include "MS_Def0.h"
#endif


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                         // for ...

#include <ctype.h>                          // f isdigit ..
#include <time.h>                           // CLOCKS_PER_SEC

#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_msh.h"              // Fac3 ..
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_os.h"               // OS_get_bas_dir ..
#include "../ut/ut_obj.h"              // UTO_stru_2_obj
#include "../ut/ut_txfil.h"            // UTF_GetPosLnr
#include "../ut/ut_bspl.h"             // UT3D_par_par1BSp
#include "../ut/byteorder.h"
#include "../ut/ut_TX.h"               // TX_Print

#include "../gr/ut_gr.h"               // GR_gtx_ckBlockWidth
#include "../gr/ut_UI.h"               // UI_Func... SYM_..
#include "../gr/ut_DL.h"               // DL_GetAtt
#include "../gr/ut_GL.h"               // GL_GetCen
#include "../gr/vf.h"                  // GL_vf1_CS

#include "../db/ut_DB.h"               // DB_GetGTxt

#include "../xa/xa.h"                  // aus_SIZ
#include "../xa/xa_mem.h"              // memspc51, mem_cbuf1





//typedef_MemTab(char);






// extern GtkWidget *ckb_Bar1, *ckb_Bar2;
extern char      WC_modnam[128];
extern char      AP_dir_save[128];


// static char  tmpSpc1[1000];
// static char  tmpSpc2[1000];
// static char  myMemspc[50000];


// #define UTX_SKP1bl(tp) {++(*(tp)); while(**(tp) == ' ') ++(*(tp));}


// aus xa.c:
extern AP_STAT   AP_stat;         // sysStat,errStat..
extern int       WC_modnr;        // the Nr of the active submodel; -1 = main.
extern ColRGB     AP_defcol;


// das folgende gibt direkten Zugriff auf die DL:
extern DL_Att     *GR_ObjTab;
extern long       GR_TAB_IND;







/*
//================================================================
  int tst_sav_db () {
//================================================================
// speichern Objekt tmpSpc1

  long  ind;

  printf("tst_sav_db \n");

  // get next SUR-ind
  // ind = DB_QueryNxtFree (Typ_CV, 20);
  ind = DB_QueryNxtFree (Typ_SUR, 20);
  printf(" ind=%d\n",ind);

  // save obj in tmpSpc1 als Typ_SUR,ind.
  DB_store_obj (tmpSpc1, &ind);

  DB_dump__ ();

  return 0;
}


//================================================================
  int tst_sav_tx () {
//================================================================
// speichern Objekt tmpSpc1


  printf("tst_sav_tx \n");

  // generiert mehrere Objekte;
  // eine UTO_obj_2_txt die AP_obj_2_txt aufruft. (AP_obj_2_txt nur f. structs)
  // Od AP_obj_2_txt soll oGX resolven; AP_stru_2_txt == bisherige AP_obj_2_txt




  AP_stru_2_txt (NULL, 0, NULL, 0);  // reset Startindizes

  UTF_clear1();     // init AuxBuf
  // create text for obj tmpSpc1 --> mem_cbuf1; add mem_cbuf1 --> AuxBuf
  AP_obj_2_txt (mem_cbuf1, mem_cbuf1_SIZ, tmpSpc1, -1);

  UTF_insert1(-1);  // Add Aux.Buffer -> MainBuffer

  return 0;

}


//================================================================
  int tst_tra () {
//================================================================
// test transform objects (surfs) ..
 
  int     iTyp1, iTyp2, i1, sizSpc1, sizSpc2;
  Vector  *vc0, vc1;
  Point   *pt0, *pt1, *pt2;
  Line    *ln1, *ln2;
  Circ    *ci1;
  Plane   *pl1;
  Mat_4x3 m1, mi1;
  Mat_4x3 mb, mc;
  TraRot  tr1;
  void    *ov1, *ov2, *datSpc1, *datSpc2;
  ObjGX   ox1, ox2, *oc1, *oc2;
  Memspc  memSeg1;


  datSpc1 = tmpSpc1;     sizSpc1 = sizeof(tmpSpc1);
  datSpc2 = tmpSpc2;     sizSpc2 = sizeof(tmpSpc2);
  printf(" siz = %d %d\n",sizSpc1,sizSpc2);


  UME_init (&memSeg1, myMemspc, sizeof(myMemspc));


  // structs aus DB holen
  pl1 = DB_get_PLN (7L);  // DB_StoreRef DB_GetRef
  // pt0 = DB_get_PT (20L);
  // pt1 = DB_get_PT (21L);
  // pt2 = DB_get_PT (23L);
  // vc0 = DB_get_VC (20L);
  // ln1 = DB_get_LN (20L);
  // ln2 = DB_get_LN (21L);
  // ci1 = DB_get_CI (23L);

  // alle folgenden liefern oGX:
  // oc1 = DB_GetCurv (21L); // BSPL
  // oc2 = DB_GetCurv (20L); // CCV
  // oc2 = DB_GetCurv (22L); // Polygon
  // oc2 = DB_GetCurv (23L); // CCV


  // ox1 = DB_GetObjGX (Typ_AC, 23L);
  // ox1 = DB_GetObjGX (Typ_CV, 23L);
  ox1 = DB_GetObjGX (Typ_SUR, 21L);


    UTO_dump_s_ (&ox1, "A##");
    UTO_dump__ (&ox1, "A##");
    // UT3D_stru_dump (Typ_AC, ci1, "C21:\n");

    // UTO_obj_Disp__ (&ox1, &memSeg1, 2, SYM_STAR_S);
    // return 0;

    // UTO_dump__ (oc2, "A20");  // CCV

    // UTO_obj_Disp__ (&ox1, &memSeg1, 2, SYM_STAR_S);
    // UTO_obj_Disp__ (datSpc1, &memSeg1, 2, SYM_STAR_S);
    // UTO_obj_Disp__ (oc2, &memSeg1, 2, SYM_STAR_S);
    // return 0;

  // UT3D_stru_dump (Typ_LN, ln1, "L20:");
  // UT3D_stru_dump (Typ_LN, ln2, "L21:");
  // UT3D_stru_dump(iTyp1, ov1, "obj-CCV:");
  // UT3D_stru_dump(iTyp2, ov2, "obj-CV:");


  // trMat zu R generieren
  UT3D_m3_loadpl (m1, pl1);
  // UT3D_stru_dump (Typ_ObjM4x3, m1, "m1\n");
  // inv. trMat zu R20 generieren
  UT3D_m3_invm3 (mi1, m1);
  // UT3D_stru_dump (Typ_ObjM4x3, mi1, "mi1\n");


  // Translation:
  // UT3D_vc_3db (&vc1, 0., 0., 50.);    // define a transl.vector
  // UTRA_def__ (1, Typ_TraTra, &vc1);  // init translation-function


  // TRANSFORMATION:
  // UTRA_def__ (1, Typ_TraMat, m1);
  UTRA_def__ (1, Typ_TraMat, mi1);


  // tra struct's
  // i1 = sizSpc1;
  // UTRA_app__ (datSpc1, &i1, Typ_PT, pt0, &memSeg1);  // PT23
  // GR_Disp_pt (datSpc1, SYM_STAR_S, 2);
  // UTRA_app__ (datSpc1, &i1, Typ_LN, ln1, &memSeg1);  // Circ
  // GR_Disp_ln (datSpc1, 2);
  // i1 = sizSpc1;
  // UTRA_app__ (datSpc1, &i1, Typ_AC, ci1, &memSeg1);  // Circ
  // GR_Disp_ac (datSpc1, 2);
  // GR_Disp_bsp (((ObjGX*)datSpc1)->data, 2, &memSeg1);
  

  // tra obj's
  i1 = sizSpc1;
  // UTRA_app_obj (datSpc1, &i1, oc1, &memSeg1);  // BSPL OK
  // UTRA_app_obj (datSpc1, &i1, oc2, &memSeg1);  // CCV
  UTRA_app_obj (datSpc1, &i1, &ox1, &memSeg1);  // Sur CCV
  // UTRA_app_obj (datSpc1, &i1, &ox2, &memSeg1);  // Sur 2 Cir

  UTO_dump_s_ (datSpc1, "obj-translated");
  UTO_dump__ (datSpc1, "obj-translated");  // CCV

  UTO_obj_Disp__ (datSpc1, &memSeg1, 2, SYM_STAR_S);

  return 0;


}

*/

//================================================================
  int tst_FileOpen () {
//================================================================

  printf("tst_FileOpen \n");

  // UI_open__ ("sample_1.gcad", "/mnt/serv1/Devel/dev/gCAD3D/dat/");
  UI_open__ ("DIM_TEST.dxf", "/mnt/serv1/Devel/dev/gCAD3D/dxf/");
  // UI_open__ ("sample_activity_modval_1.gcad", "X:\\Devel\\dev\\gCAD3D\\dat\\");

  return 0;

}

 
//================================================================
  int tst_imp_dxf () {
//================================================================
// test import dxf

  int      irc;
  char     cbuf[256];


  printf("tst_imp_dxf \n");

  // Compile, Link.
  strcpy(cbuf, "xa_dxf_r.so");
  irc = DLL_build__ (cbuf);
  printf(" build=%d\n",irc);
  if(irc != 0) return -1;


 
  // UI_open__ ("bsplTest1.dxf", "/mnt/serv1/Devel/dev/gCAD3D/dxf/");
  // UI_open__ ("elli_2000_1.dxf", "/mnt/serv1/Devel/dev/gCAD3D/dxf/");
  // UI_open__ ("t2.dxf", "/mnt/serv1/Devel/dev/gCAD3D/dxf/");// lines,circs
  // UI_open__ ("t3.dxf", "/mnt/serv1/Devel/dev/gCAD3D/dxf/");// points
  // UI_open__ ("t1.dxf", "/mnt/serv1/Devel/dev/gCAD3D/dxf/");// lines,circs
  // UI_open__ ("elli1.dxf", "/mnt/serv1/Devel/dev/gCAD3D/dxf/");//ELLIPSE
  // UI_open__ ("poly1.dxf", "/mnt/serv1/Devel/dev/gCAD3D/dxf/");//POLYLINE
  // UI_open__ ("DIM_TEST.dxf", "/mnt/serv1/Devel/dev/gCAD3D/dxf/");
  UI_open__ ("c109806.dxf", "/mnt/serv1/Devel/dev/gCAD3D/dxf/");
  // UI_open__ ("Spinne.dxf", "/mnt/serv1/Devel/dev/gCAD3D/dxf/");//LWPOLYLINE
  // UI_open__ ("dim4.dxf", "/mnt/serv1/Devel/dev/gCAD3D/dxf/");//LEADER
  // UI_open__ ("t7.dxf", "/mnt/serv1/Devel/dev/gCAD3D/dxf/");//TEXT
  // UI_open__ ("t13.dxf", "/mnt/serv1/Devel/dev/gCAD3D/dxf/");//Radius-DIMEN
  // UI_open__ ("schieberkontur.dxf", "/mnt/serv1/Devel/dev/gCAD3D/dxf/");//INSERT
  // UI_open__ ("fac2.dxf", "/mnt/serv1/Devel/dev/gCAD3D/dxf/");//3DFACE
  // UI_open__ ("lkw3.dxf", "/mnt/serv1/Devel/dev/gCAD3D/dxf/");
  // UI_open__ ("Gelaende1.dxf", "/mnt/serv1/Devel/dev/gCAD3D/dxf/");
  // UI_open__ ("gelaende2.dxf", "/mnt/serv1/Devel/dev/gCAD3D/dxf/");
  // UI_open__ ("OpenGeo2.dxf", "/mnt/serv1/Devel/dev/gCAD3D/dxf/");
  // UI_open__ ("OpenGeo1.dxf", "/mnt/serv1/Devel/dev/gCAD3D/dxf/");
  // UI_open__ ("OpenGeo3.dxf", "/mnt/serv1/Devel/dev/gCAD3D/dxf/");
  // UI_open__ ("t1.dxf", "/mnt/serv1/Devel/dev/gCAD3D/dat/");
  // UI_open__ ("t4.dxf", "/p2/fwork/");

  return 0;

}


//================================================================
  int tst_imp_igs () {
//================================================================
// test import iges
// use mit ./go comp tst3
 
  int   irc;
  char  cbuf[256];


  printf("tst_imp_igs \n");


  // Compile, Link.
  strcpy(cbuf, "xa_ige_r.so");
  irc = DLL_build__ (cbuf);
  printf(" build=%d\n",irc);
  if(irc != 0) return -1;


  // Run DLL
  strcpy(cbuf,
  "/mnt/serv1/Devel/dev/gCAD3D/igs/blech1.igs");
  // "/mnt/serv1/Devel/dev/gCAD3D/igs/Starcd_1.igs");
  // "/mnt/win_d/dev/gCAD3D/dat/.0.igs");
  // "/mnt/win_d/dev/gCAD3D/igs/schmiernippel_71412.igs");
  // "/mnt/serv1/Devel/dev/gCAD3D/igs/t2.igs");
  // "/mnt/serv1/Devel/dev/gCAD3D/igs/M1.igs");
  // "/mnt/serv1/Devel/dev/gCAD3D/igs/pt+ln+ci2.igs");
  // "/mnt/serv1/Devel/dev/gCAD3D/dat/warner1.igs");


  OS_dll_do ("xa_ige_r", "IGE_r__", cbuf);
  // AP_Import__ ("xa_ige_r", "IGE_r__", cbuf);
  // AP_ExportIges__ (fNam);

  return 0;

}


//================================================================
  int tst_imp_stp () {
//================================================================
// test import step              make -f tst.mak

  int   irc;
  char  cbuf[256];


  printf("tst_imp_stp \n");


  // Compile, Link.
  strcpy(cbuf, "xa_stp_r.so");
  irc = DLL_build__ (cbuf);
  printf(" build=%d\n",irc);
  if(irc != 0) return -1;

  // Run DLL
  strcpy(cbuf,
  "/home/fwork/gCAD3D/dat/unknown.stp");
  // "/mnt/serv1/Devel/dev/gCAD3D/dat/sample_step1.stp");
  // "/mnt/serv1/Devel/dev/gCAD3D/formate/step/Models/hookchain.stp");
  // "/mnt/serv1/Devel/dev/gCAD3D/formate/step/Models/Assy6.stp");
  // "/mnt/serv1/Devel/dev/gCAD3D/formate/step/Models/Coupling_Link.stp");
  // "/mnt/serv1/Devel/dev/gCAD3D/formate/step/Models/14.dat.step");
  // "/mnt/serv1/Devel/dev/gCAD3D/formate/step/ErrorModels/angle1.stp");
  // "/mnt/serv1/Devel/dev/gCAD3D/cadenas/wuerth_010734925_sechskantschraube_din_933_m4x25_10_9_a2c_2_03.stp");

  // printf("vi %s\n",cbuf);
  OS_dll_do ("xa_stp_r", "STP_r__", cbuf);
  // AP_Import__ ("xa_stp_r", "STP_r__", cbuf);
  printf(" nach OS_dll_do\n");
  // AP_ExportIges__ (fNam);

  return 0;

}



//================================================================
  int tst_imp_stl () {
//================================================================
// test import obj
  
  printf("tst_imp_stl \n");
  
  UI_open__ ("head.stl", "/mnt/serv1/Devel/dev/gCAD3D/formate/stl/");
  // UI_open__ ("Fraesteil1.stl", "/mnt/serv1/Devel/dev/gCAD3D/formate/stl/");
  
  return 0;
  
} 
  

//================================================================
  int tst_imp_obj () {
//================================================================
// test import obj

  printf("tst_imp_obj \n");

  UI_open__ ("cub2.obj", "/mnt/serv1/Devel/dev/gCAD3D/formate/obj/");

  return 0;

}


//================================================================
  int tst_imp_tess () {
//================================================================
// test import obj
  
  printf("tst_imp_tess \n");
  
  UI_open__ ("test_tex3.tess", "/mnt/serv1/Devel/dev/gCAD3D/dat/");
  
  return 0;
  
} 


//================================================================
  int tst_imp_vrml2 () {
//================================================================

  int   irc;
  char  cbuf[256];


  printf("tst_imp_vrml2 \n");


  // Compile, Link.
  strcpy(cbuf, "xa_vr2_r.so");
  irc = DLL_build__ (cbuf);
  printf(" build=%d\n",irc);
  if(irc != 0) return -1;


  // get filename from file ../tmp/input.dat
  // sprintf(cbuf, "%stmp/input.dat", OS_get_bas_dir());
  // UTX_fgetLine (cbuf, 256, cbuf, 1);


  strcpy(cbuf,
  // "/mnt/serv1/Devel/dev/gCAD3D/formate/vrml/ModelsV2/t_def2.wrl");
  // "/mnt/serv1/Devel/dev/gCAD3D/formate/vrml/ModelsV2/t1.wrl");
  // "/mnt/serv1/Devel/dev/gCAD3D/formate/vrml/ModelsV2/t_prot6.wrl");
  // "/mnt/serv1/Devel/dev/gCAD3D/formate/vrml/ModelsV2/5.wrl");
  // "/mnt/serv1/Devel/dev/gCAD3D/formate/vrml/ModelsV2/t_def2.wrl");
  // "/mnt/serv1/Devel/dev/gCAD3D/formate/vrml/ModelsV2/scissor.wrl");
  // "/mnt/serv1/Devel/dev/gCAD3D/formate/vrml/ModelsV2/t_tex1.wrl");
  "/mnt/serv1/Devel/dev/gCAD3D/formate/vrml/ModelsV2/Gelaende4.wrl");
  // "/mnt/serv1/Devel/dev/gCAD3D/formate/vrml/ModelsV2/Linie0.wrl");
  // "/mnt/serv1/Devel/dev/gCAD3D/formate/vrml/ModelsV2/venus.wrl");
  // "/mnt/serv1/Devel/dev/gCAD3D/formate/vrml/ModelsV2/nefertiti.wrl");
  // "/mnt/serv1/Devel/dev/gCAD3D/formate/vrml/ModelsV2/groundPlane.wrl");
  // "/mnt/serv1/Devel/dev/gCAD3D/formate/vrml/ModelsV2/t_def1.wrl");
  // "/mnt/serv1/Devel/dev/gCAD3D/formate/vrml/ModelsV2/t_tra1.wrl");
  // "/mnt/serv1/Devel/dev/gCAD3D/formate/vrml/ModelsV2/t_ifs1.wrl");
  // "/mnt/serv1/Devel/dev/gCAD3D/formate/vrml/ModelsV2/t_cyl1.wrl");
  // "/mnt/serv1/Devel/dev/gCAD3D/formate/vrml/ModelsV2/t_con1.wrl");
  // "/mnt/serv1/Devel/dev/gCAD3D/formate/vrml/ModelsV2/t_sph1.wrl");
  // "/mnt/serv1/Devel/dev/gCAD3D/formate/vrml/ModelsV2/t_box1.wrl");
  // "/mnt/serv1/Devel/dev/gCAD3D/formate/vrml/ModelsV2/39.wrl");
  // "/mnt/serv1/Devel/dev/gCAD3D/formate/vrml/ModelsV2/Camera1.wrl");
  // "/mnt/serv1/Devel/dev/gCAD3D/formate/vrml/ModelsV2/mnt.wrl");


  OS_dll_do ("xa_vr2_r", "VR2_r__", cbuf);
  // AP_ExportIges__ (fNam);

  return 0;

}



//================================================================
  int tst_exp_vrml1 () {
//================================================================

  TX_Print("tst_exp_vrml1\n");

  strcpy(WC_modnam, ".0.wrl");

  AP_stat.subtyp = 0;  // VRML1

  UI_save__ (1);

  UTX_ftyp_cut (WC_modnam);   // remove Filetyp !!!

  return 0;

}


//================================================================
  int tst_exp_stl () {
//================================================================

  TX_Print("tst_exp_stl\n");
  strcpy(WC_modnam, "unknown.stl");
  UI_save__ (1);
  UTX_ftyp_cut (WC_modnam);   // remove Filetyp !!!

  return 0;

}


//================================================================
  int tst_exp_obj () {
//================================================================
  
  TX_Print("tst_exp_obj\n");
  strcpy(WC_modnam, "unknown.obj");
  UI_save__ (1);
  UTX_ftyp_cut (WC_modnam);   // remove Filetyp !!!
  
  return 0;
  
} 
  
  
//================================================================
  int tst_exp_dxf () {
//================================================================
  
  TX_Print("tst_exp_dxf\n");
  AP_stat.subtyp = 1;  // 0=R12; 1=2000
  strcpy(WC_modnam, "unknown.dxf");
  UI_save__ (1);
  UTX_ftyp_cut (WC_modnam);   // remove Filetyp !!!
  return 0;

}


//================================================================
  int tst_exp_stp () {
//================================================================
  
  int    irc;
  char   cbuf[256];
  
  
  TX_Print("tst_exp_stp\n");
  
  
  // Compile, Link.
  strcpy(cbuf, "xa_stp_w.so");
  irc = DLL_build__ (cbuf);
  printf(" build=%d\n",irc);
  if(irc != 0) return -1;
  
  
  // export
  strcpy(WC_modnam, "unknown.stp");
  UI_save__ (1);
  UTX_ftyp_cut (WC_modnam);   // remove Filetyp !!!
  
  return 0;

}


//================================================================
  int tst_exp_svg () {
//================================================================

  int    irc;
  char   cbuf[256];


  TX_Print("tst_exp_svg\n");


  // Compile, Link.
  strcpy(cbuf, "xa_svg_w.so");
  irc = DLL_build__ (cbuf);
  printf(" build=%d\n",irc);
  if(irc != 0) return -1;


  // export
  strcpy(WC_modnam, "unknown.svg");
  UI_save__ (1);
  UTX_ftyp_cut (WC_modnam);   // remove Filetyp !!!

  return 0;

}

//================================================================
  int tst_exp_vrml2 () {
//================================================================
// test export VRML2

// use mit make -f tst3.mak
// ./go defLoad tst3


  TX_Print("tst_exp_vrml2\n");

  strcpy(WC_modnam, ".0.wrl");

  AP_stat.subtyp = 1;  // VRML2

  UI_save__ (1);

  UTX_ftyp_cut (WC_modnam);   // remove Filetyp !!!

  return 0;

}



//================================================================
  int tst_imp_vrml1 () {
//================================================================
// test import Vrml
// Im Model muss stehen:
// M1="Data/t1.wrl" P(0 0 0)
// M1=MOCK "WRL/x29.wrl" P(0 0 0)
// aktivieren durch RUN ..

  int      irc;
  char     cbuf[256];
  ModelRef *mdr;


  printf("tst_imp_vrml1\n");


  // Compile, Link.
  strcpy(cbuf, "xa_wrl_r.so");
  irc = DLL_build__ (cbuf);
  printf(" build=%d\n",irc);
  if(irc != 0) return -1;


  UI_but_END ();


/*
 das folgende geht nicht korrekt ....
  // reset den Name-Buffer (init)
  DB_StoreModBas (0, NULL);

  // save ext. Model 
  irc = DB_StoreModBas (3, "WRL/Test1.wrl");
  printf(" modnr=%d\n",irc);

  mdr = DB_get_ModBas (irc);
  mdr->po  = UT3D_PT_NUL;
  mdr->vx  = UT3D_VECTOR_NUL;
  mdr->vz  = UT3D_VECTOR_NUL;
  mdr->scl =  1.;

  UT3D_stru_dump (Typ_Model, mdr, "ModRef:");

  GR_DrawModel (1L, Typ_Att_Fac, mdr);

  GR_DrawModel (1L, Typ_Att_Fac, mdr);
*/


  return 0;

}


/*
//================================================================
  int test_stp_w () {
//================================================================

  printf("test_stp_w \n");

  STP_w__ ("t1.stp");

  printf("\n\n");
  system("cat t1.stp");

  return 0;

}

//================================================================
  int test_stp_r () {
//================================================================

  printf("test_stp_r \n");

  STP_r__ ("t1.stp");

  return 0;

}
*/


//================================================================
//================================================================
//================================================================
//================================================================


/*
///================================================================
  int UME_start (Memspc **memSpc, void* objDat, int osiz) {
///================================================================
/// UME_start             init Memspc; return Memspc with TimeStamp.

/// puts memSpc as first Record into objDat; gives back its Adress.

/// Example:
/// char     memspc51[50000];
/// Memspc   *memSeg1;
/// int      *iTab, sizTab;
/// // place memSeg1 in memspc51 and load it.
/// UME_start (&memSeg1, memspc51, sizeof(memspc51));

/// // give space to iTab; do not fix memspc.next
/// UME_connect (iTab, memSeg1);
/// // set max size for iTab
/// sizTab = UME_ck_free (memSeg1) / sizeof(int);

/// // reserve 100 int's in memSeg1; update memspc.next.
/// iTab = UME_reserve (memSeg1, 100 * sizeof(int));
/// // give the remaining space to (char*)cbuf
/// UME_connect (cbuf, memSeg1);
/// // query size of cbuf
/// cbufSiz = UME_ck_free (memSeg1);




  Memspc  *spc;

  // load memSpc as first record in memSpace
  spc = objDat;

  spc->start = (char*)objDat + sizeof(Memspc);;
  spc->next  = spc->start;
  spc->end   = (char*)objDat + osiz;

  OS_TimeStamp (&spc->ts);

  *memSpc = spc;

  return 0;

}
*/

//================================================================
  int tst31 () {
//================================================================
// use miti
// ./go comp defLoad tst3

  int       irc, i1, i2, iTyp, ptNr, lnNr, pTabSiz, iNr, iTab[100], oSiz;
  long      l1, dli;
  double    d1, d2, dTab[10], *da;
  char      cBuf[80], *tp1, *tp2, oh1[OBJ_SIZ_MAX], oh2[OBJ_SIZ_MAX];
  Point     *pp0, *pp1, *pp2, *pp3, *pp4, *pp5, *pp6;
  Point     *pTab, p1={0,0,0}, p2={1,0,0},pta[]={
           {0,0,0},{10,0,1},{20,0,3},{30,0,0},{40,0,0},{50,0,0}};
    // pa2[]={{0,0,0},{10,0,0},{-2,2,0},{0,-10,0},{0,0,0}};
  Vector    v1={1,0,0}, v2={0,0,1}, v3={-1,-3,0}, v4, v5;
  Vector    *vcp1;
  Point2    p21={0,0}, p22={0.5,5};
  Vector2   v21={10,0};
  Line      *lp1, *lp2, *lp3, ln1, ln2;
  Circ      *ci1, *ci2, *ci3, circ1, circ2, circ3;
  CurvBSpl  *bsp1, *bsp2, cb1, cb2, cb3;
  CurvRBSpl rbspl, *rbsp;
  CurvElli  *ell1;
  CurvClot  *clo1, clot;
  CurvPoly  *plg1;
  Plane     *plp1, *plp2, pln1;

  SurBSpl   *sb;
  SurRBSpl  *srb, srbsp;
  Mesh      msh1;
  void      *vp1;
  ObjGX     *ox1, *ox2, o1, oSur1;
  Memspc    objSeg, tmpSeg;
  Mat_4x3   m1;
  Triangle  tri1, tri2;
  
// Point2    pti={ -7.600000,   12.000000};
// Point2    p20={-10.000000,   12.000000};
// Point2    p21={-10.000000,   8.184550};
// Point2    p22={ 10.000000,   12.000000};


  //----------------------------------------------------------------
  // hier beginnen die Load-fromDB's

  // pp0 = DB_get_PT (20L);
  // pp1 = DB_get_PT (21L);

  // vcp1 = DB_get_VC (20L);

  // lp1 = DB_get_LN (20L);

  // ci1 = DB_get_CI (20L);
    // UT3D_stru_dump (Typ_AC, ci1, "C21:\n");

  // ell1 = DB_get_CV (&i1, 21L); // Typ_CVELL CurvElli
  // clo1 = DB_get_CV (&i1, 20L); // Typ_CVCLOT CurvClot
  // plg1 = DB_get_CV (&i1, 20L); // Typ_CVPOL CurvPoly
  // bsp1 = DB_get_CV (&i1, 21L); // Typ_CVRBSP CurvBSpl
    // UT3D_stru_dump (Typ_CVPOL, plg1, "");

  // plp1 = DB_get_PLN (23L);
    // UT3D_stru_dump (Typ_PLN, plp1, "Plane");

  // ox1 = DB_GetSur (2L, 1);
    // UT3D_stru_dump (Typ_ObjGX, ox1, "");
    // if(ox1->typ == Typ_SURMSH) .. // 160

  // UT3D_stru_dump (Typ_SURRBSP, ox1->data, "");
  // srb = ox1->data;
     // ox1 = DB_GetTra (20L); UTO_dump__ (ox1, "T20");

  // ox1 = DB_GetGTxt (21L);


/*
  // GL_GetEyeLn (&p1, &v1); // Sichtstrahl (Point & Vector) retournieren
  UT3D_pt_3db (&p1, 10.26, 1.8, 0.);
  UT3D_vc_3db (&v1, 0., 0., 1.);

  // use clothoid clo1

  // intersect Clothoid X Line(pt+vc) 
  ptNr = 6;
  _UT3D_pt_intclotptvc (&ptNr, pta, dTab, &p1, &v1, clo1, UT_DISP_cv);
*/



//----------------------------------------------------------------
// ab hier Testfunctions ...
//----------------------------------------------------------------
    // GR_Disp_pt (&pTab[i1], SYM_STAR_S, 2);

  // UME_init (&objSeg, memspc501, sizeof(memspc501));
  // UME_init (&tmpSeg, memspc201, sizeof(memspc201));

  // tst30(); return 0;
  // tst32(); return 0;

//----------------------------------------------------------------
// Test project point > Mesh
  // if(ox1->typ != Typ_SURMSH) {TX_Print("E001"); return -1;}

// {  // see TSU_DrawSurMsh
/*
  int       mNr;  // db-Ind of mesh
  int       pgNr;
  int       pNr, fNr;
  Point     *pTab = NULL;
  Fac3      *fTab = NULL;



  mNr = 2;  // db-Ind of mesh

  // PointGroupNumber = (int)data (Typ_Index)
  pgNr = OGX_GET_INDEX (ox1);

  MSH_pt_prjptmsh_ (&p1, pp0, mNr, pgNr);
    GR_Disp_pt (&p1, SYM_STAR_S, 2);
*/

/*
  // load PointFile  (write: lxml_read) pTab=malloc !
  i1 = MSH_bload_pTab (&pTab, &pNr, WC_modnr, pgNr);
  if(i1 < 0) {TX_Error("TSU_DrawSurMsh E001"); return -1;}
    printf(" pNr=%d\n",pNr);


  // load MeshFile    fTab=malloc !
  i1 = MSH_bload_fTab (&fTab, &fNr, WC_modnr, mNr);
  if(i1 < 0) {TX_Error("TSU_DrawSurMsh E002"); return -1;}
    printf(" fNr=%d\n",fNr);

  // project point > Mesh
  MSH_pt_prjpt (&p1, pp0, fTab, fNr, pTab, pNr);
    GR_Disp_pt (&p1, SYM_STAR_S, 2);

  // project point > Mesh
  MSH_pt_prjpt (&p1, pp1, fTab, fNr, pTab, pNr);
    GR_Disp_pt (&p1, SYM_STAR_S, 2);


  if(pTab) free(pTab);
  if(fTab) free(fTab);
*/
// }

  return 0;

/*
//----------------------------------------------------------------
// Test UPRJ_ project along vektor
  // vcp1 = &UT3D_VECTOR_Y;
  // UPRJ_def__ (Typ_PLN, plp1, NULL);
  UPRJ_def__ (Typ_PLN, plp1, vcp1);

  // UPRJ_app_pt (&p1, pp0);

  oSiz = OBJ_SIZ_MAX;
  // UPRJ_app__ (oh1, &iTyp, &oSiz, 1, pp0, Typ_PT, NULL);
  // UPRJ_app__ (oh1, &iTyp, &oSiz, 1, lp1, Typ_LN, NULL);

  // UPRJ_app__ (oh1, &iTyp, &oSiz, 1, ci1, Typ_AC, NULL);
  // UPRJ_def__ (Typ_PLN, plp2, &UT3D_VECTOR_Z);
  // oSiz = OBJ_SIZ_MAX;
  // UPRJ_app__ (oh2, &iTyp, &oSiz, 1, oh1, Typ_CVELL, NULL);

  // UPRJ_app__ (oh1, &iTyp, &oSiz, 1, plg1, Typ_CVPOL, &objSeg);

  UPRJ_app__ (oh1, &iTyp, &oSiz, 1, bsp1, Typ_CVBSP, &objSeg);

  return 0;
*/

//----------------------------------------------------------------
  // GL_Disp_vSym testen
  // GL_Disp_vSym ();


/*
  // test 'struct/obj mit den limitParameters begrenzen':
  // - Copy struct                                        UTO_copy_stru
  // - struct/obj mit den limitParameters begrenzen       UTO_stru_int (structs)
  // - struct/obj in Polygon umwandeln ...                UT3D_npt_ox (ObjGX)

  // obj op2 mit den limitParameters v0, v1 begrenzen
  v0 = 0.15;
  v1 = 0.75;

  // obj to cut
  oi.typ   = Typ_CVBSP; // Typ_AC; // Typ_LN;
  oi.form  = Typ_Index;
  oi.data  = (void*)(long)20L;


  // 1. cutting obj:
  oc1.typ  = Typ_Par1; // Typ_LN; // Typ_Val;
  oc1.form = Typ_Float8; // Typ_Index;  // Typ_Val;
  oc1.data = &v0; // (void*)(long)21L; // &v0;

  // 2. cutting obj:
  oc2.typ  = Typ_Par1; // Typ_LN; // Typ_Val;
  oc2.form = Typ_Float8; // Typ_Index;  // Typ_Val;
  oc2.data = &v1; // (void*)(long)22L; // &v1;


  // trim
  irc = UTO_obj_trim (&oo, &oi, &oc1, 0, &oc2, 0, &wrkSeg);
  if(irc < 0) return irc;
    // UTO_obj_Disp__ (&oo, &wrkSeg, 9);


  // - struct/obj in Polygon umwandeln ...                UT3D_npt_ox (ObjGX)
  p1Tab = (Point*)memspc55;
  p1Max = sizeof(memspc55) / sizeof(Point);

  ptNr = p1Max;
  i1 = UT3D_npt_ox (&ptNr, p1Tab, &oo, UT_DISP_cv*2.);
  if(i1 != 0) return 0;
    GR_Disp_cv (p1Tab, ptNr, 9);
*/



  // -----------------------------------
/*  test UT3D_pta_rot__
  int  p1Max;
  Point *p1Tab, *p2Tab;

  // prepare space for polygons
  p1Tab = (Point*)memspc55;
  p1Max = sizeof(memspc55) / sizeof(Point);


  // Polygon p1Tab[ptNr] aus Randkurve oxc generieren
  ptNr = p1Max;
  i1 = bspl_pol_bsp (&ptNr, p1Tab, bsp1, ptNr, UT_DISP_cv*2.);
  // i1 = UT3D_npt_ox (&ptNr, p1Tab, &oxc, UT_DISP_cv*2.);
  if(i1 != 0) return 0;
  // GR_Disp_cv (p1Tab, ptNr, 9);

  p2Tab = &p1Tab[ptNr];

  UT3D_pta_rot__ (p2Tab, p1Tab, ptNr, pp1, &UT3D_VECTOR_IZ, RAD_90);
*/

  // UT3D_ci_ptvcpt2angr (&c1,  pp1, pp2,
                       // &UT3D_VECTOR_Z,
                       // -RAD_90, RAD_90);





///////////// ab hier Test fuer UT3D_angr_vc2pt - unfertig ///////////////
  // hidLin ();
  // pp1 = DB_get_PT (1L);
  // pp2 = DB_get_PT (2L);
  // pp3 = DB_get_PT (3L);
  // printf(" %f\n",UT3D_angr_vc2pt(pp1, &v1, pp2, pp3));
  // return 0;

/*
  // die aktuell selektierte ObjId suchen
  irc = DL_find_sel (&i1, &l1);
  if(irc < 0) {TX_Print("no obj selected ..."); return 0;}


  // prepare APTsource: MAN-Mode: copy Edi --> memory.
  AP_SRC_mem_edi ();


  // init obj-list
  Grp_init ();


  // add obj to obj-list
  Grp_add (i1,l1);


  // obj-list Recursiv nach seinen Eltern absuchen und diese zufuegen
  Grp_res ();


  // export (native) alle objects of obj-list --> file
  char cbuf1[256], cbuf2[256];
  
  sprintf(cbuf1, "%s.dat",WC_modnam);
  sprintf(cbuf2, "%sxa%cdir.lst",OS_get_bas_dir(),fnam_del);
  GUI_save__ ("save Group native",      // titletext
            AP_dir_save,                // path
            cbuf2,                      // directoryList
            cbuf1,                      // defaultModelname
            (void*)Grp_exp);


  return 0;

*/






/*
///////////// ab hier Test fuer UT3D_obj_cnvt_rbspl - unfertig ///////////////
  CurvRBSpl *spi;

  rbsp = DB_get_CV (&i1, 4L);
  UT3D_stru_dump (Typ_CVRBSP, rbsp, "");

  spi = rbsp;

  UT3D_rbspl_tst_cc (spi);


  irc = UT3D_obj_cnvt_rbspl (&o1, spi, &tbuf1);
  if(irc >= 0) {
    UT3D_stru_dump (Typ_AC, o1.data, "ex UT3D_ci_rbspl3\n");
    GR_Disp_ac (o1.data, 9);
  }

  return 0;



///////////// ab hier Test fuer UT3D_obj_cnvt_srbsp - unfertig ///////////////
  // Model conrod/S19,20          Rhi_cv1
  // weiter bei // UT3D_ci_3pt (...)
  // get S1
  // rbsp = DB_get_CV (&i1, 19L); // 5-8   conrod:11 12 13 14 15 19 20
  // UT3D_stru_dump (i1, rbsp, "rbsp");
  op1 = DB_GetSur (18L, 1); // conrod A4/5 A7/8 A9/10 A28 A30
  // UT3D_stru_dump (Typ_SURRBSP, op1->data, "");
  srb = op1->data;


  // convert Rat.B-SplSur --> CI/LN
  // dann --> ../ut/ut_rbspl.c
  // see also UT3D_bsp_degrad
  irc = _UT3D_obj_cnvt_srbsp (&o1, srb, &tbuf1);
  if(irc < 0) return 0;

  // convert sru --> text
  // AP_obj_2_txt (NULL, 0, NULL, 0);
  // AP_obj_2_txt (mem_cbuf1, mem_cbuf1_SIZ, &o1, -1);


  return 0;
*/

}


//================================================================
  int tst3_2 (Point *pp1) {
//================================================================

  int       i1, ptNr;
  double    dTab[10];
  Memspc    objSeg, tmpSeg;
  Point     *pTab, p1, pta[10];
  Vector    v1;
  CurvClot  *clo1;


  clo1 = DB_get_CV (&i1, 20L);


  UME_init (&objSeg, memspc501, sizeof(memspc501));
  UME_init (&tmpSeg, memspc201, sizeof(memspc201));
  pTab = (void*)memspc55;
  ptNr = sizeof(memspc55) / sizeof(Point);

  // GL_GetEyeLn (&p1, &v1); // Sichtstrahl (Point & Vector) retournieren
  // UT3D_pt_3db (&p1, 10.26, 1.8, 0.);
  UT3D_vc_3db (&v1, 0., 0., 1.);


  // use clothoid clo1

  // intersect Clothoid X Line(pt+vc)
  ptNr = 1;
  // _UT3D_pt_intclotptvc (&ptNr, pta, dTab, pp1, &v1, clo1, UT_DISP_cv);
  // _UT3D_par_clotpt (dTab, pp1, clo1, UT_DISP_cv);


  // UT3D_par_clotpt (&spt);

  DL_Redraw ();

  return 0;

}


//================================================================
//================================================================
// unfertige Funktionen:
//================================================================
//================================================================


//================================================================
  int UT3D_hbp_bppt (double *hbp, int bp, Point *pt) {
//================================================================
// Hoehenwert der Backplane aus Punkt uebernehmen


  if(bp == 0) {        // BCKPLN_YZ
    *hbp = pt->x;

  } else if(bp == 1) { // BCKPLN_XZ
    *hbp = pt->y;

  } else if(bp == 2) { // BCKPLN_XY
    *hbp = pt->z;
  }

  printf("ex UT3D_hbp_bppt %f\n",*hbp);
  return 0;

}


//===========================================================================
  int UT3D_3ipt_ckfar_npt (int *i1, int *i2, int *i3, int pNr, Point *pTab) {
//===========================================================================
// UNFERTIG 
// die 3 am weitest entfernten Punkte suchen ..
// Achtung: Umdrehungssinn kann nun falsch sein !!!!! (Korrektur: UT3D_vcn_ccv)

// see UT3D_pl_pta UT3D_sbs_ck_planar UT3D_vcn_ccv UT3D_pt_ckfar_npt

  printf("UT3D_3ipt_ckfar_npt %d\n",pNr);

  if(pNr < 4) {
    if(pNr == 3) {*i1=0; *i2=1; *i3=2; goto L_exit;}
    TX_Error("UT3D_3ipt_ckfar_npt E001");
    return -1;
  }

  TX_Error("UT3D_3ipt_ckfar_npt E002");
    return -1;


  L_exit:
  printf("ex UT3D_3ipt_ckfar_npt %d %d %d\n",*i1,*i2,*i3);
  return 0;

}


//================================================================
//================================================================
// neue Funktionen:
//================================================================
//================================================================



//================================================================
  int _UT3D_rMin_pta (double *rMin, int pNr, Point *pTab) {
//================================================================
// vom gesamten Polgon den kleinsten Radius errechnen
// Nur Abschaetzung; es wird von jeweils 3 Punkten der Kreisradius errechnet.
// ev in eine neue source ../ut/ut_pta.c ?

  int    i1;
  double d1;

  if(pNr < 3) {TX_Error("UT3D_rMin_pta E001"); return -1;}


  GR_Disp_pTab (pNr, pTab, SYM_STAR_S, 9);


  *rMin = UT_VAL_MAX;
  for(i1=2; i1<pNr; ++i1) {
    // UT3D_stru_dump (Typ_PT, &pTab[i1], "p[%d]",i1);

    // radius zw 3 punkten
    UT3D_rdc_3pt (&d1, &pTab[i1-2], &pTab[i1-1], &pTab[i1]);
    if(d1 < *rMin) *rMin = d1;
    printf(" %d-%d-%d r = %f\n",i1-2,i1-1,i1,d1);

  }


    printf("ex UT3D_rMin_pta %f\n",*rMin);

  return 0;

}

//======== EOF =========
