// Example create objects.
/*
 *
 * Copyright (C) 2015 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
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
\file  ../APP/DemoPlugin_Create.c
\brief  demo create objects
\code
=====================================================
List_functions_start:
..
List_functions_end:
=====================================================

\endcode *//*----------------------------------------

First time build plugin with:
. ../options.sh && make -f DemoPlugin_Create.mak

Compile/Link/Reload is done while gCad3D is up and running !!


. ../options.sh && make -f DemoPlugin_Create.mak

*/

#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// #include <GL/gl.h>                     // GL_TRIANGLE_FAN
#define GL_TRIANGLE_FAN  6             // unsauber ..



#ifdef _MSC_VER
// die folgenden 2 Funktionen exportieren (werden vom Main gerufen):
__declspec(dllexport) int gCad_main ();
__declspec(dllexport) int gCad_fini ();
// nachfolgende externals werden aus dem Main-Exe imported:
#define extern __declspec(dllimport)
#endif



#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_ox_base.h"          // OGX_SET_OBJ
#include "../ut/func_types.h"               // UI_Func...
#include "../ut/gr_types.h"               // SYM_* ATT_* LTYP_*
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_itmsh.h"            // Fac3 MshFac

#include "../gr/ut_gr.h"               // GR_tDyn_*

#include "../gui/gui__.h"

#include "../xa/xa_ui.h"                     // UID_..





//===========================================================================
// EXTERNALS:

// ex ../gr/ut_DL.c
extern long DL_temp_ind;  // next temp-index to used


//================================================================
// LOCAL-VARS:

static  char   cBuf[512];
char myMemspc[50000];




//=========================================================
  int gCad_fini () {
//=========================================================

  AP_User_reset ();      // close application

  return 0;
}



//=========================================================
  int gCad_main () {
//=========================================================


  printf("gCad_main aus DemoPlugin_Create\n");


  // Message into mesaagewindow (bottom)
  TX_Print("gCad_main aus DemoPlugin_Create\n");


  // clear model ("File/New")
  UI_men__ ("new");





  //================================================================
  UI_GR_ScalAuto (1);   // scale to Model (temp. and dynamic object cannot scale)


  // permanent objects
  cre_perm_src ();        // permanent objects from sourceCode
  // cre_perm_obj ();        // permanent objects from binary-obj


  // temporary objects; (max 30 aux.objects)
  // cre_temp_1 ();          


  // create dynamic objects (not stored in DB, but visible, can move, zoom ..)
  // - cannot be found by "Scale All"
  // - Rework ("END") will delete these objects
  // cre_tDyn_obj ();        // Temporary-Dynamic binary-obj
  // cre_tDyn_sym ();        // Temporary-Dynamic symbols
  // cre_tDyn_txt ();        // Temporary-Dynamic text
  // cre_tDyn_mdr ();        // Temporary-Dynamic modelRefs;


  //----------------------------------------------------------------
  // surfaces
  cre_tDyn_sur_nifac1 ();    // indexed-faces
  cre_tDyn_sur_nifac2 ();    // double-indexed-faces



/*
  //================================================================
// DO NOT USE; beeing replaced ..
  // cre_6 ();      // ruled surfaces
  // cre_7 ();      // revolved surfaces (perm)
  // cre_8 ();      // tesselated surfaces
  // cre_9 ();      // planare surf; trimmed / perforated
  // cre_10();      // TestDisplayfunktions (_Disp_)
*/

  //================================================================
  // Redraw DispList
  DL_Redraw ();

  // UI_GR_ScalAuto (0);  // rescale all

  gCad_fini ();        // exit & reset

  return 0;

}


//================================================================
  int cre_tDyn_obj () {
//================================================================
// disp. dynamic obj from binary-obj


  int      i1;
  long     l1;
  Point    pt1, ptc;
  Line     ln1;
  Vector   vz;
  Point    pp1[]={60.,30., 0.,   65.,30., 0.,
                  65.,40., 0.,   60.,40., 0.,   60.,30., 0.};
  Point    cvp[]={-200.,100.,0.,
                  -220.,125.,0.,
                  -250.,150.,0.,
                  -220.,175.,0.,
                  -200.,200.,0.};
  Circ     ci1;
  CurvBSpl bsp1;
  Memspc   memSeg1;


  // display point
  // for parameters see GR_set_obj (use tagfiles - vi ctrl-r)
  UT3D_pt_3db (&pt1, 60., 20., 10.);
  GR_tDyn_pt (&pt1, ATT_PT_HILI);


  // disp line
  UT3D_pt_3db (&ln1.p1, 100.,   0.,   0.);
  UT3D_pt_3db (&ln1.p2, 100., 100.,   0.);
  GR_tDyn_ln (&ln1, Typ_Att_dash_long);


  // display curve (polygon)
  GR_tDyn_pcv (pp1, 5, Typ_Att_blue);


  // 3D-Circ
  UT3D_pt_3db (&pt1, 100., 100., 100.);
  UT3D_pt_3db (&ptc, 100., 100., 125.);
  UT3D_vc_3db (&vz, 0., 1., 0.);
  // circ from startPt, center, z-vector, opening angle (pos. = CCW)
  UT3D_ci_ptptvcangr (&ci1, &pt1, &ptc, &vz, UT_RADIANS(90.));
  // curve, dbi=unknown=0
  GR_tDyn_ocv (Typ_CI, &ci1, 0, Typ_Att_blue);



  // get mespc for B-Spline
  UME_init (&memSeg1, myMemspc, sizeof(myMemspc));

  // B-Spline-Curve, Degree 3  from points
  //           (cvBsp, &memSeg1, pTab, ptNr, deg);
  UT3D_cbsp_ptn (&bsp1, &memSeg1, cvp,     5,   3); 
  // disp
  GR_tDyn_ocv (Typ_CVBSP, &bsp1, 0, Typ_Att_blue);


  return 0;

}


//=========================================================
  int cre_6 () {
//=========================================================
// ruled surfaces ..

  long   id1;
  Point  pt1,  pt2, pa[9];
  Line   ln1, ln2;
  Vector v1;
  Circ   ci1;
  ObjGX  os, oa[2];


  printf("cre_6\n");



  // create Lineobj
  UT3D_pt_3db (&ln1.p1, 300.,  0.,   0.);
  UT3D_pt_3db (&ln1.p2, 350.,  0.,   0.);
  OGX_SET_OBJ (&oa[0], Typ_LN, Typ_LN, 1, &ln1);

  // create pointobj
  UT3D_pt_3db (&pt1, 300.,  0.,   100.);
  OGX_SET_OBJ (&oa[1], Typ_PT, Typ_PT, 1, &pt1);

  // create ruled surf from line + point
  OGX_SET_OBJ (&os, Typ_SURRU, Typ_ObjGX, 2, oa);

  // draw ruled surf
  // TSU_DrawSurT_ (&os, Typ_Att_dash_long, 0L);



  // create ln 1
  UT3D_pt_3db (&ln1.p1, 300.,  100.,   0.);
  UT3D_pt_3db (&ln1.p2, 350.,  100.,   0.);
  OGX_SET_OBJ (&oa[0], Typ_LN, Typ_LN, 1, &ln1);

  // create ln 2
  UT3D_pt_3db (&ln2.p1, 320.,  120.,   100.);
  UT3D_pt_3db (&ln2.p2, 350.,  100.,   100.);
  OGX_SET_OBJ (&oa[1], Typ_LN, Typ_LN, 1, &ln2);

  // create ruled surf from 2 lines
  OGX_SET_OBJ (&os, Typ_SURRU, Typ_ObjGX, 2, oa);
    // DEB_dump_ox_0 (&os, "SURRU ln+ln:");
    // DEB_dump_ox_s_ (&os, "SURRU ln+ln:");

  // display
  // TSU_DrawSurT_ (&os, Typ_Att_dash_long, 0L);




  UT3D_pt_3db (&ln1.p1, 300.,  200.,   0.);
  UT3D_pt_3db (&ln1.p2, 350.,  200.,   0.);
  // ruled obj # 1 = ln
  OGX_SET_OBJ (&oa[0], Typ_LN, Typ_LN, 1, &ln1);


  UT3D_pt_3db (&pt1, 350.,  200.,   100.);
  UT3D_pt_3db (&pt2, 300.,  200.,   100.);
  // Z-vector of active construction-plane
  AP_Get_ConstPl_vz (&v1);

  // Circ from center, startpoint, axis, opening angle
  UT3D_ci_ptptvcangr (&ci1, &pt1, &pt2, &v1, UT_RADIANS(-120.));
    DEB_dump_obj__ (Typ_CI, &ci1, "ci1");
  OGX_SET_OBJ (&oa[1], Typ_CI, Typ_CI, 1, &ci1);

  // ruled obj from line & circ
  OGX_SET_OBJ (&os, Typ_SURRU, Typ_ObjGX, 2, oa);
  // TSU_DrawSurT_ (&os, Typ_Att_dash_long, 0L);





  UT3D_pt_3db (&pt1, 450.,  0.,  75.);
  UT3D_pt_3db (&pt2, 450.,  0.,   0.);
  id1 = 1;
  // cylinder (ID, att, cen1, cen2, rad1, rad2)
  GR_CreCyl (&id1, Typ_Att_dash_long, &pt1, &pt2, 15., 35.);




  id1 = 1;
  UT3D_pt_3db (&pt1, 450.,  0.,   0.);
  // disc (ID, att, cen, vector, inner_rad, outer_rad)
  GR_CreDisk (&id1, Typ_Att_dash_long, &pt1, (Vector*)&UT3D_VECTOR_Z, -35., -50.0);




  // triangle-fan (ID, att, startpt, nr_of_points, points)
  id1 = 1;
  UT3D_pt_3db (&pa[0], 350.,  300.,   0.);
  UT3D_pt_3db (&pa[1], 350.,  350.,   0.);
  UT3D_pt_3db (&pa[2], 350.,  350.,  75.);
  UT3D_pt_3db (&pa[3], 350.,  300.,  75.);
  GR_CreTriaFan (&id1, Typ_Att_dash_long, pa, 3, &pa[1]);



  // triangle-strip (ID, att, nr_of_points_dir1, nr_of_points_dir2, points)
  UT3D_pt_3db (&pa[0], 350.,  400.,   0.);
  UT3D_pt_3db (&pa[1], 350.,  450.,   0.);
  UT3D_pt_3db (&pa[2], 350.,  500.,   0.);

  UT3D_pt_3db (&pa[3], 348.,  400.,  25.);
  UT3D_pt_3db (&pa[4], 346.,  450.,  25.);
  UT3D_pt_3db (&pa[5], 348.,  500.,  25.);

  UT3D_pt_3db (&pa[6], 350.,  400.,  50.);
  UT3D_pt_3db (&pa[7], 350.,  450.,  50.);
  UT3D_pt_3db (&pa[8], 350.,  500.,  50.);

  GR_CreTriaStrip (&id1, Typ_Att_dash_long, 3, 3, pa);


  return 0;

}


//=========================================================
  int cre_7 () {
//=========================================================
// revolved surfaces (perm)

  int    typ1, typ2, dbTyp;
  long   id1, id2, dbInd;
  Point  cvp[]={500.,100.,0.,
                520.,125.,0.,
                550.,150.,0.,
                520.,175.,0.,
                500.,200.,0.};
  Line     ln1, ln2;
  SurRev   srv;
  Memspc   memSeg1;
  CurvBSpl cv1;
  ObjGX    os, ocv;


  printf("cre_7 \n");


  // create Centerline
  UT3D_pt_3db (&ln1.p1, 450.,  100.,   0.);
  UT3D_pt_3db (&ln1.p2, 450.,  150.,   0.);
  // id1 = DB_StoreLine (DB_QueryNxtFree(Typ_LN, 1), &ln1);
  // AP_obj_2_txt_query (&typ1, &id1);           // query index
  UTO_sav_ost (&dbTyp, &id1, Typ_LN, Typ_LN, 1, (void*)&ln1);


  // create contour
  UME_init (&memSeg1, myMemspc, sizeof(myMemspc));  // init memoryseg (tempspc)
  //           (cvBsp, &memSeg1, pTab, ptNr, deg);
  UT3D_cbsp_ptn (&cv1,  &memSeg1, cvp,     5,   3);  // make BSP-curve from pTab
  // OGX_SET_OBJ (&ocv, Typ_CVBSP, Typ_CVBSP, 1, &cv1);
  // id2 = DB_StoreCurv (DB_QueryNxtFree(Typ_CV, 1), &ocv, 0);
  // AP_obj_2_txt_query (&typ2, &id2);           // query index
  UTO_sav_ost (&dbTyp, &id2, Typ_CVBSP, Typ_CVBSP, 1, (void*)&cv1);


  // RevSur
  srv.indCen = id1;              // Ind of Centerline (Typ = Typ_LN)
  srv.typCen = Typ_LN;           // typ of Centerline
  srv.indCov = id2;              // Ind of contourelement
  srv.typCov = Typ_CV;           // typ of contourelement
  srv.ang1   = UT_RADIANS(300.); // starting angle
  srv.ang2   = UT_RADIANS(360.); // ending angle
  srv.v0     = 0.;               // ending par.
  srv.v1     = 1.;               // ending par.


  UTO_sav_ost (&dbTyp, &dbInd, Typ_SURRV, Typ_SURRV, 1, (void*)&srv);
  // OGX_SET_OBJ (&os, Typ_SURRV, Typ_SURRV, 1, &srv);
  // TSU_DrawSurT_ (&os, Typ_Att_dash_long, 0L);
  


  return 0;

}


//=========================================================
  int cre_8 () {
//=========================================================
// create tesselated surface manually
// Surf = 1-n PlanarPatches
//            Planar patch = 1-n contours (and vector)
//                               Contour = ipoints (closed, tesselated)
// see also BMSH_test_cre1

  long  il;
  ObjGX gCont[10], gPat[2], gSur;

  Point pp1[]={600., 0., 0., 650., 0., 0.,
               650.,20., 0., 600.,20., 0., 600., 0., 0.,};
  Point pp2[]={600.,20., 0., 650.,20., 0.,
               650.,40., 0., 600.,40., 0., 600.,20., 0.,};
  Point pp3[]={600.,40., 0., 650.,40., 0.,
               650.,40.,40., 600.,40.,40., 600.,40., 0.,};


  //----------------------------------------------------------------
  // Create sur
  gSur.typ  = Typ_GL_Sur;
  gSur.form = Typ_ObjGX;
  gSur.siz  = 1;
  gSur.data = gPat;


  // Create 1. Patch
  gPat[0].typ  = Typ_GL_PP;
  gPat[0].form = Typ_ObjGX;
  gPat[0].siz  = 0;
  gPat[0].data = &gCont[0];


  //----------------------------------------------------------------
  // add  vector to 1. patch
  gCont[0].typ  = Typ_VC;
  gCont[0].form = Typ_VC;
  gCont[0].siz  = 1;
  gCont[0].data = (Vector*)&UT3D_VECTOR_Z;

  gPat[0].siz  = 1;


  //----------------------------------------------------------------
  // add 1. contour to 1. patch
  gCont[1].typ  = Typ_PT;
  gCont[1].form = Typ_PT;
  gCont[1].siz  = 5;
  gCont[1].data = pp1;
  gCont[1].aux  = GL_TRIANGLE_FAN;

  gPat[0].siz  = 2;


  //----------------------------------------------------------------
  // add 2. contour to 1. patch
  gCont[2].typ  = Typ_PT;
  gCont[2].form = Typ_PT;
  gCont[2].siz  = 5;
  gCont[2].data = pp2;
  gCont[2].aux  = GL_TRIANGLE_FAN;

  gPat[0].siz  = 3;


  //================================================================
  // add 2. patch to surf
  gPat[1].typ  = Typ_GL_PP;
  gPat[1].form = Typ_ObjGX;
  gPat[1].siz  = 0; // 2
  gPat[1].data = &gCont[3];

  gSur.siz  = 2;


  //----------------------------------------------------------------
  // add vector to 2. patch
  gCont[3].typ  = Typ_VC;
  gCont[3].form = Typ_VC;
  gCont[3].siz  = 1;
  gCont[3].data = (Vector*)&UT3D_VECTOR_IY;

  gPat[1].siz  = 1;


  //----------------------------------------------------------------
  // add 1. contour to 2. patch
  gCont[4].typ  = Typ_PT;
  gCont[4].form = Typ_PT;
  gCont[4].siz  = 5;
  gCont[4].data = pp3;
  gCont[4].aux  = GL_TRIANGLE_FAN;

  gPat[1].siz  = 2;


  //----------------------------------------------------------------
  // display
  il = DL_StoreObj (Typ_SUR, 20L, Typ_Att_dash_long);  // identifier 20
  // DEB_dump_ox_s_ (&gSur, "sur-tess:");
  // DEB_dump_ox_0 (&gSur, "sur-tess:");
  GL_set_sur_tess (&gSur, Typ_Att_dash_long);


  return 0;

}



//=========================================================
  int cre_9 () {
//=========================================================
// planare surf; trimmed / perforated

  Point pp1[]={600.,100., 0., 650.,100., 0.,
               650.,200., 0., 600.,200., 0., 600.,100., 0.};

  Point pp2[]={625.,125., 0., 625.,175., 0.};

  int      irc;
  long     id1, id2, id3, ids;
  Vector   v1;
  Circ     ci1;
  CurvPoly plg1;
  ObjGX    os, oa[4], otmp;
  Memspc   memSeg1;


  printf("cre_9 \n");

  // Z-vector of active construction-plane
  AP_Get_ConstPl_vz (&v1);


  // save Polygon pp1
  UME_init (&memSeg1, myMemspc, sizeof(myMemspc));  // init memoryseg (tempspc)
  UT3D_plg_pta (&plg1, pp1, 5, &memSeg1);           // PolgonCurve from pt-tab
  OGX_SET_OBJ (&otmp, Typ_CVPOL, Typ_CVPOL, 1, &plg1);
    // DEB_dump_obj__(Typ_CVPOL, otmp.data, "plg1:");
  id1 = DB_StoreCurv (DB_QueryNxtFree(Typ_CV, 1), &otmp, 0);  // save in DB



  // save holes
  UT3D_ci_ptvcr (&ci1, &pp2[0], &v1, 20.);
  OGX_SET_OBJ (&otmp, Typ_CI, Typ_CI, 1, &ci1);
  id2 = DB_StoreCirc (DB_QueryNxtFree(Typ_CI, 1), &ci1);

  UT3D_ci_ptvcr (&ci1, &pp2[1], &v1, 20.);
  OGX_SET_OBJ (&otmp, Typ_CI, Typ_CI, 1, &ci1);
  id3 = DB_StoreCirc (DB_QueryNxtFree(Typ_CI, 1), &ci1);


  // 1.obj: define typ of supporting surface (planar)
  OGX_SET_OBJ (&oa[0], Typ_Typ, Typ_Int4, 1, PTR_INT(Typ_SURPLN));

  // 2.obj: boundary; 3,4: holes
  OGX_SET_INDEX (&oa[1], Typ_CV, id1);
  OGX_SET_INDEX (&oa[2], Typ_CI, id2);
  OGX_SET_INDEX (&oa[3], Typ_CI, id3);

  // all -> container
  OGX_SET_OBJ (&os, Typ_SUR, Typ_ObjGX, 4, &oa);
    DEB_dump_ox_0 (&os, "os=");

  // draw
  ids = DB_QueryNxtFree(Typ_SUR, 1);
  irc = DB_StoreSur (&ids, &os);

  // TSU_DrawSurT_ (&os, Typ_Att_dash_long, ids);
  
  return 0;

}


//=========================================================
  int cre_10 () {
//=========================================================
// TestDisplayfunktions (_Disp_)


  int    i1;
  Point  pt1;
  Vector v1;
  Point  pp1[]={600.,300., 0., 650.,300., 0.,
                650.,400., 0., 600.,400., 0., 600.,300., 0.};


  // display points with symbols
  GR_tDyn_npt__ (pp1, 5, ATT_PT_GREEN);


  // display numbers at points
  for(i1=0; i1<4; ++i1) GR_tDyn_txiA (&pp1[i1], i1, 0);


  // display curve
  GR_tDyn_pcv (pp1, 5, 2);

  
  // display surf - planar - unperforated
  GR_Disp_spu (5, pp1, Typ_Att_dash_long);


  return 0;

}


//================================================================
  int cre_perm_obj () {
//================================================================

  int    dbTyp, iCv1, iCv2;
  long   dbInd;
  Point  pt1, pt2, ptc, pta[5];
  Line   ln1;
  Circ   ci1;
  AText  txa1;
  GText  txg1;
  char   cbuf[80];


  // create 2 3D-Points
  pt1.x=100.; pt1.y=100.; pt1.z=100.;
  pt2.x=100.; pt2.y=200.; pt2.z=100.;
  UTO_sav_ost (&dbTyp, &dbInd, Typ_PT, Typ_PT, 1, (void*)&pt1);
  UTO_sav_ost (&dbTyp, &dbInd, Typ_PT, Typ_PT, 1, (void*)&pt2);


  // 3D-Line
  ln1.p1 = pt1; ln1.p2 = pt2;
  UTO_sav_ost (&dbTyp, &dbInd, Typ_LN, Typ_LN, 1, (void*)&ln1);


  // 3D-Circ
  ci1.pc.x=100.; ci1.pc.y=100.; ci1.pc.z=100.;
  ci1.p1.x=100.; ci1.p1.y=100.; ci1.p1.z=125.;
  // ci1.p2 = ci1.p1;    // full circ
  ci1.p2.x=125.; ci1.p2.y=100.; ci1.p2.z=100.;
  ci1.vz.dx=0.;  ci1.vz.dy=1.;  ci1.vz.dz=0.;
  ci1.rad = -25.;            // pos = CCW; neg = CW
  UTO_sav_ost (&dbTyp, &dbInd, Typ_CI, Typ_CI, 1, (void*)&ci1);


  // Alfatext (does not scale)
  strcpy(cbuf, "ci1");
  UT3D_pt_3db (&txa1.p1, 200., 100.,  100.);    // textpos
  UT3D_pt_3db (&txa1.p2, 200., 100.,  50.);     // startPt leaderline; optional
  txa1.aTyp = 0;          // 0=Text
  txa1.col  = ATT_COL_BLUE;
  txa1.txt  = cbuf;
  UTO_sav_ost (&dbTyp, &dbInd, Typ_ATXT, Typ_ATXT, 1, (void*)&txa1);


  // Graf.text (scales)
  strcpy(cbuf, "Graf.text");
  UT3D_pt_3db (&txg1.pt, 0., 100., 0.);
  txg1.size = 10.;
  txg1.dir  = 90.;         // direction
  txg1.txt  = cbuf;
  UTO_sav_ost (&dbTyp, &dbInd, Typ_GTXT, Typ_GTXT, 1, (void*)&txg1);


  // 5 points -> pta
  UT3D_pt_3db (&pta[0], 200., 0.,  0.);
  UT3D_pt_3db (&pta[1], 250., 0.,  0.);
  UT3D_pt_3db (&pta[2], 300., 50., 0.);
  UT3D_pt_3db (&pta[3], 350., 50., 0.);
  UT3D_pt_3db (&pta[4], 400., 80., 0.);


  // Polygon from 5 points
  iCv1 = UTO_sav_ost (&dbTyp, &dbInd, Typ_CVPOL, Typ_PT, 5, (void*)pta);



  // B-Spline-Curve, Degree 3  from points
  iCv2 = UTO_sav_ost (&dbTyp, &dbInd, Typ_CVBSP, Typ_PT, 5, (void*)pta);


  // clean undo-tables
  UNDO_clear ();


  return 0;

}


//================================================================
  int cre_temp_1 () {
//================================================================
// Create Temporary-objects:


  Point   pt1={ 0., 0., 0.}, pt2;
  Vector  vc1={ 1., 0., 2.};
  Plane   pln1;
  GText   txtg;

  // 2D-text (bitmap)
  // disp integers at position pt1
  pt1.y += 2;
  DL_temp_ind = 1;
  GR_temp_txiA (&pt1, 123, ATT_COL_RED);
  pt1.x += 2;
  GR_temp_txiA (&pt1, 456, ATT_COL_GREEN);
  pt1.x += 2;

  // disp 2D-text
  GR_temp_txtA (&pt1, "abc", ATT_COL_YELLOW);


  // disp scaled rotated text
  pt1.x += 2.;
  txtg.pt   = pt1;
  txtg.size = 0.5;
  txtg.dir  = 30.;    // deg
  txtg.txt  = "Test";
  GR_temp_txtG (&txtg, ATT_COL_BLUE);


  // points 
  pt1.x = 0;
  pt1.y += 2;
  GR_temp_pt (&pt1, ATT_PT_HILI);
  pt1.x += 2;
  GR_temp_pt (&pt1, ATT_PT_GREEN);
  pt1.x += 2;
  DL_temp_ind = 8;     // use a fixed index
  GR_temp_pt (&pt1, ATT_PT_YELLOW);


  // vector  0=display normalized length; 1=display real length
  pt1.x = 0;
  pt1.y += 2;
  GR_temp_vc (&vc1, &pt1, ATT_COL_BLUE, 0);
  pt1.x += 2;
  GR_temp_vc (&vc1, &pt1, ATT_COL_GREEN, 1);
  // disp at screenCenter
  GR_temp_vc (&vc1, NULL, ATT_COL_CYAN, 1);


  //================================================================
  // Symbols
  pt1.x = 0;
  pt1.y += 2;
  // plane  1=pln 4=Axis+Chars 5=Plane+Axis+Chars
  UT3D_pl_ptvc (&pln1, &pt1, &vc1);
  GR_temp_pln (&pln1, Typ_Att_Symb, 1);
  pt1.x += 2;
  UT3D_pl_ptvc (&pln1, &pt1, &vc1);
  GR_temp_pln (&pln1, Typ_Att_def, 4);

  // composite temporary obj:
  pt1.x += 2.;
  pt2 = pt1;
  pt2.x += 2.;
  DL_temp_init ();           // start temporary-obj
  GL_att_cv (Typ_Att_blue);  // see INF_COL_CV
  GL_set_SymV2 (SYM_ARROH, &pt1, &pt2, 1.);  // head only at pt1 !
  GL_set_ln2_2pt ((Point2*)&pt1, (Point2*)&pt2);
  GL_set_SymV2 (SYM_ARROH, &pt2, &pt1, 1.);  // head only at pt2 !
  GL_list_close ();          // close obj

  pt1.x += 4.;
  DL_temp_init ();           // start temporary-obj
  GL_att_cv (Typ_Att_blue);  // see INF_COL_CV
  // APT_disp_SymV1 (SYM_TRI_S,  1, &pt1, 4.);
  GL_set_symB (SYM_CROSS, &pt1);
  // GL_set_symV_r (SYM_CROSS, &pt1, 0., 2.);
  GL_list_close ();          // close obj


//   // angle
//   pt1.x += 2;
//   GR_temp_ang (12, Typ_Att_hili1, &pt1, &WC_sur_act.vx, &WC_sur_act.vz, RAD_90);




  //================================================================
  // Remove Temporary-Dynamic-objects:


  return 0;

}


//================================================================
  int cre_tDyn_mdr () {      // temporary modelRefs;
//================================================================
// add temporary modelRefs; internal subModel must exist;
//   (load or create subModel: eg call cre_perm_src)



  int       irc, bmi;
  long      dbi;
  double    scale;
  Point     pt1 = { 10., 100., 0.};
  Plane     pl1;
  ModelRef  mdr;


  // get refSys (origin & orientation) for modelRef
  UT3D_pl_ptvc (&pl1, &pt1, (Vector*)&UT3D_VECTOR_Z);
    DEB_dump_obj__ (Typ_PLN, &pl1, "  temp_mdr-pl1: ");


  //----------------------------------------------------------------
  // disp (already existing) internal subModel, defined by its basicModelNr bmi

  // create modelRef
  bmi = 0;      // internal subModel <bmi> must exist !
  scale = 1.;
  // get modelRef from basicModelNr and refSys
  Mod_mdr__bmi_pln (&mdr, bmi, &pl1, scale);
    DEB_dump_obj__ (Typ_Model, &mdr, "  temp_mdb-mdr: ");

  // display temp:
  // DL_temp_ind = 8;   // set fixed GL-index
  // irc = GR_temp_mdr (&mdr, GR_TMP_HILI);
              // modelRef,    att:  GR_TMP_DEF|GR_TMP_HILI|GR_TMP_DIM

  // disp. temp.Dyn:
  irc = GR_tDyn_mdr (&mdr, GR_TMP_DEF);
              // modelRef,    att:  GR_TMP_DEF|GR_TMP_HILI|GR_TMP_DIM


  //----------------------------------------------------------------
  // disp external subModel, defined by its basicModelNr bmi
/*
  // create modelRef
  bmi = 0;      // internal subModel <bmi> must exist !
  scale = 1.;
  // get modelRef from basicModelNr and refSys
  Mod_mdr__bmi_pln (&mdr, bmi, &pl1, scale);
    DEB_dump_obj__ (Typ_Model, &mdr, "  temp_mdb-mdr: ");


  GR_tDyn_mdr_mock (&mdr, GR_TMP_HILI);
*/




  return irc;

}


//================================================================
  int cre_tDyn_sym () {
//================================================================
// temporary-dynamic objects;  (test objects)
// Rework ("END") will delete these objects


  static long    startInd=0;

  int     i1;
  Point   pt1 = { 0., -5., 0.};
  Vector  vc1 = { 2., 1., 3.}, vc2;


  printf("cre_tDyn_sym \n");

  // get DL-startindex (for later delete)
  startInd = GL_Get_DLind ();

  //----------------------------------------------------------------
  // create point-symbols
  for(i1=0; i1<10; ++i1) {
    GR_tDyn_pt (&pt1, ATT_PT_GREEN);
    pt1.x += 2.;
  }


  // create bitmap-symbols
  pt1.x = 0.;
  pt1.y -= 2.;
  for(i1=0; i1<10; ++i1) {
    GR_tDyn_symB__ (&pt1, SYM_STAR_S, ATT_COL_RED);
    pt1.x += 2.;
  }


  // create vector-symbols        see INF_symbols
  pt1.x = 0.;
  pt1.y -= 2.;
  // normalized vector
  GR_tDyn_vc__ (&vc1, &pt1, ATT_COL_BLUE, 0);
  GR_tDyn_vc__ (&UT3D_VECTOR_Y, &pt1, ATT_COL_BLUE, 0);
  // vector true-length
  for(i1=0; i1<5; ++i1) {
    pt1.x += 2.;
    GR_tDyn_vc__ (&vc1, &pt1, ATT_COL_BLUE, 1);
    UT3D_vc_multvc (&vc2, &UT3D_VECTOR_Y, pt1.x);
    GR_tDyn_vc__ (&vc2, &pt1, ATT_COL_RED, 1);
  }


  // vector-symbols in 2D-plane; scaled, rotated  
  pt1.x = 0.;
  pt1.y -= 2.;
  GR_tDyn_symV_r (SYM_ARROW, ATT_COL_BLACK, &pt1, NULL, 10.);
    pt1.x += 2.;
  GR_tDyn_symV_r (SYM_SQUARE, ATT_COL_RED, &pt1, NULL, 1.);
    pt1.x += 2.;
  GR_tDyn_symV_r (SYM_PLANE, ATT_COL_GREEN, &pt1, NULL, 1.);
    pt1.x += 2.;
  GR_tDyn_symV_r (SYM_AXIS1, ATT_COL_BLUE, &pt1, NULL, 1.);
    pt1.x += 2.;
  GR_tDyn_symV_r (SYM_AXIS, ATT_COL_YELLOW, &pt1, NULL, 1.);
    pt1.x += 2.;
  GR_tDyn_symV_r (SYM_CROSS, ATT_COL_MAGENTA, &pt1, NULL, 1.);
    pt1.x += 2.;
  GR_tDyn_symV_r (SYM_CROSS1, ATT_COL_CYAN, &pt1, NULL, 1.);
    pt1.x += 2.;
  GR_tDyn_symV_r (SYM_TRIANG, ATT_COL_WHITE, &pt1, NULL, 10.);
    pt1.x += 2.;
  GR_tDyn_symV_r (SYM_ARRO3H, ATT_COL_HILI, &pt1, &UT3D_VECTOR_Y, 1.);
  GR_tDyn_symV_r (SYM_LENGTH, ATT_COL_DIMMED, &pt1, &UT3D_VECTOR_Y, 2.);

  // vector-symbols oriented - 3D (rotate + tilt)
    pt1.x += 2.;
  GR_tDyn_symV_o (SYM_ARRO3H, ATT_COL_BLUE, &pt1, &vc1, 1.);
  GR_tDyn_symV_o (SYM_LENGTH, ATT_COL_BLUE, &pt1, &vc1, 2.);


  //----------------------------------------------------------------
  // remove all temporary-dynamic objects:
  // if(startInd) {GL_Delete (startInd); startInd=0;}

  return 0;

}


//================================================================
  int cre_tDyn_txt () {
//================================================================
// temporary-dynamic text;  (test objects)
// Rework ("END") will delete these objects


  static long    startInd=0;

  int     i1;
  Point   pt1 = { 0., 8., 0.};
  GText   txtg;


  printf("cre_tDyn_txt \n");

  // get DL-startindex (for later delete)
  startInd = GL_Get_DLind ();


  //----------------------------------------------------------------
  // bitmap text
  // see INF_COL_SYMB
  GR_tDyn_txtA (&pt1, "cre_tDyn_txt", ATT_COL_BLUE);


  // scaled rotated text
  pt1.x += 6.;
  txtg.pt   = pt1;
  txtg.size = 1.;
  txtg.dir  = 30.;    // deg
  txtg.txt  = "abc";
  GR_tDyn_txtG (&txtg, Typ_Att_Symb);


  //----------------------------------------------------------------
  // remove all temporary-dynamic objects:
  // if(startInd) {GL_Delete (startInd); startInd=0;}

  return 0;

}


//================================================================
  int cre_perm_src () {      // permanent objects
//================================================================


  long    dbi;                        // DataBase-index
  char    s1[256];
  Point   pt1={ 20., 0., 0.};


  //----------------------------------------------------------------
  // init buffer
  UTF_clear1();


  //----------------------------------------------------------------
  // create source-text for perm.obj. and add to buffer
  // points 
  // add direct:
  // UTF_add1_line("P21=P(20., 0., 10.)");

  // or get next free DB-index for point
  dbi = DB_QueryNxtFree (Typ_PT, 20); printf(" dbiPT1=%ld\n",dbi);
  // create source
  sprintf (s1,"P%ld=P(20., 0., 10.)",dbi);
  // add source into buffer
  UTF_add1_line (s1);


  //----------------------------------------------------------------
  // create perm.obj. from text and add line into buffer
  //   UTF_add1_line("L20=L(P20 P21)");


  //----------------------------------------------------------------
  // add a catalog-part as internal subModel
  // get next free dbi for modelReference
  dbi = DB_QueryNxtFree (Typ_Model, 20);
    printf(" dbiNxtM=%ld\n",dbi);
  // create source
  sprintf (s1,"M%ld=CTLG \"Schrauben/SKS_6x30\" R(PERP P(10 10 0) DZ)",dbi);
  // add source into buffer
  UTF_add1_line (s1);


  //----------------------------------------------------------------
  // add buffer to model
  UTF_insert1 (-1L);  // add buffer to model
  ED_load__ ();     // update model


  //----------------------------------------------------------------
  UI_but_END ();    // process additional src


  return 0;

}


//================================================================
  int cre_tDyn_sur_nifac1 () {
//================================================================
// indexed-faces

  int    gTyp, ptNr, otyp, att;
  Line   ln1, ln2;
  Vector v1;
  Circ   ci1;
  Point  pt1, pt2;
  ObjGX  os, oa[2];
  MshFac nfa = _MSHFAC_NUL;
  ColRGB col1 = _ColRGB_NUL;


  printf("cre_tDyn_sur_nifac1 \n");

  // create ln 1
  UT3D_pt_3db (&ln1.p1, 300.,  100.,   0.);
  UT3D_pt_3db (&ln1.p2, 350.,  100.,   0.);
  OGX_SET_OBJ (&oa[0], Typ_LN, Typ_LN, 1, &ln1);

  // create ln 2
  // UT3D_pt_3db (&ln2.p1, 320.,  120.,   100.);
  // UT3D_pt_3db (&ln2.p2, 350.,  100.,   100.);
  // OGX_SET_OBJ (&oa[1], Typ_LN, Typ_LN, 1, &ln2);

  // or create Circ from center, startpoint, axis, opening angle
  UT3D_pt_3db (&pt1, 320.,  120.,   100.);
  UT3D_pt_3db (&pt2, 350.,  100.,   100.);
  AP_Get_ConstPl_vz (&v1);
  UT3D_ci_ptptvcrd (&ci1, &pt1, &pt2, 25., &v1, 1, 0);
    DEB_dump_obj__ (Typ_CI, &ci1, "ci1");
  OGX_SET_OBJ (&oa[1], Typ_CI, Typ_CI, 1, &ci1);

  // create ruled surf from 2 curves
  OGX_SET_OBJ (&os, Typ_SURRU, Typ_ObjGX, 2, oa);
    DEB_dump_ox_0 (&os, "SURRU ln+ln:");
    DEB_dump_ox_s_ (&os, "SURRU ln+ln:");

  // tesselate sru
  GRTSU_nifac_sru (&nfa, &os);

  // disp with default-color (att=0);
  // GR_tDyn_nfac (&nfa, 0);
  GR_tDyn_nifac (&nfa, 0);

  // or set color yellow (1,1,0)
  // UTcol__3i (&col1, 200,200,0);     // RGB 0-255
  // GR_tDyn_nfac (&nfa, INT32I_COL(&col1));

  UME_free (&nfa.mSpc);

  return 0;

}



//================================================================
  int cre_tDyn_sur_nifac2 () {
//================================================================
// double-indexed-faces

//         4--- 3 ---- 2    points
//           \  |   /  |
//             \| /    |
//              0 ---- 1

  int     irc, fNr, oTyp, iAtt;
  long    dbi;
  Point   pa[] = {{0.,0.,0}, {10.,0.,0.}, {10., 10., 0.},
                  {0.,10.,0}, {-10.,10.,0.}};
  int     ia[] = { 0,1,2,      2,3,0,      3,4,0};      // index into pa
  Fac3    fa[] = {{0,1,2, 0}, {3,4,5, 0}, {6,7,8, 0}};  // index into ia
  Vec3f   va[] = {0.f, 0.f, 1.f};
  MshFac  nifa;


  printf("cre_tDyn_sur_nifac2 \n");

  nifa.oTyp  = Typ_SURPLN;
  nifa.fNr   = 3;
  nifa.pa3   = pa;
  nifa.ipa   = ia;
  nifa.fac   = fa;
  nifa.vc3   = va;

  // disp with default-color (att=0);
  GR_tDyn_nifac (&nifa, 0);

  return 0;

}



//======================== EOF ======================
