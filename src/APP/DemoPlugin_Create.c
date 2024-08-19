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
Example create objects.
DemoPlugin_Create.mak ../APP/DemoPlugin_Create.c ../APP/UPDATE.c

See also:
../UIX/Demosimple.mak             // minimum example plugin
../UIX/DemoPlugin_Create.mak      // create objects
../UIX/DemoPlugin_Hili.c          // hilite 
../UIX/DemoPlugin_Resolv.mak
../UIX/DemoPlugin_Mousemove.mak
../UIX/DemoPlugin_Dialog.mak
../UIX/Demo_gui_plate.mak


-----------------------------------------------------
TODO:
  ..

-----------------------------------------------------
Modifications:
2024-08-19  new functions in ../APP/UPDATE.c added.

-----------------------------------------------------
*/
/*!
\file  ../APP/DemoPlugin_Create.c
\brief  demo create objects
\code
=====================================================
List_functions_start:

cre_perm_obj              permanent objects from binary-obj
cre_perm_src              permanent objects from sourceCode
cre_perm_src_surpln       planare surf; trimmed / perforated
cre_perm_src_img_tag      image, tag with text;
cre_tDyn_obj              disp. dynamic obj from binary-obj
cre_tDyn_sym              temporary-dynamic objects (test objects) - symbols
cre_tDyn_txt              temporary-dynamic text;  (test objects)
cre_temp_1                temporary objects

obj_hide__                hide / redisplay obj
obj_del__                 remove obj

cre_sav_obj               store DB-obj from source
SUR_ox_surTP              make ObjGX for trimmed-perforated-surf

List_functions_end:
=====================================================

\endcode *//*----------------------------------------

First time build plugin with:
. ../options.sh && make -f DemoPlugin_Create.mak

To compile/Link/Reload while gCad3D is up and running: start with "./do comp"

- activate plugin with Plugin/Select slect from list;

- restart with keys Ctrl-P


*/


// definition "export"
#include "../xa/export.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // va_list va_arg


// #include <GL/gl.h>                     // GL_TRIANGLE_FAN
#define GL_TRIANGLE_FAN  6             // unsauber ..

#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_cast.h"             // INT__PTR
#include "../ut/ut_ox_base.h"          // OGX_SET_OBJ
#include "../ut/func_types.h"               // UI_Func...
#include "../ut/gr_types.h"               // SYM_* ATT_* LTYP_*
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_itmsh.h"            // Fac3 MshFac

#include "../db/ut_DB.h"               // DB_GetObjGX

#include "../gr/ut_gr.h"               // GR_tDyn_*
#include "../gr/ut_GL.h"               // GL_*

#include "../gui/gui__.h"

#include "../xa/xa_ui.h"                     // UID_..
#include "../xa/xa_mem.h"              // memspc*
#include "../xa/xa_msg.h"              // MSG_*



//----------------------------------------------------------------
// EXPORTS to main-module
export int gCad_main ();
export int gCad_fini ();



//===========================================================================
// EXTERNALS:

// ../gr/ut_DL.c
extern long DL_temp_ind;  // next temp-index to used

// ../ci/NC_Main.c
extern int     APT_dispPT;


//================================================================
// LOCAL-VARS:

static  char   cBuf[512];
char myMemspc[50000];


//================================================================
// PROTOTYPES:

  int SUR_ox_surTP (ObjGX **oxo, int typ, Memspc *mSeg, ...);


//=========================================================
//=========================================================




//=========================================================
  int gCad_fini () {
//=========================================================

  AP_User_reset ();      // close application

  return 0;
}



//=========================================================
  int gCad_main () {
//=========================================================
// gCad_main                  plugin-entry-point

  printf("gCad_main aus DemoPlugin_Create\n");


  // Message into mesaagewindow (bottom)
  TX_Print("gCad_main aus DemoPlugin_Create\n");


  UI_men__ ("clear");   // new model, but do NOT save active model
  // UI_men__ ("Redraw"); // redraw
  // UI_men__ ("new");   // save and clear all objects

  // UI_GR_ScalAuto (1);   // scale to Model (temp. and dynamic object cannot scale)


  // display points also in mode "VWR"
  SRC_add_cmd ("MODE DISP_PT ON");


  //================================================================
  // uncomment only one of following functions ..


  //================================================================
  // permanent objects

  // permanent objects from binary-obj
  // cre_perm_obj ();        


  // permanent objects from sourceCode
  // cre_perm_src ();

  // image, tag with text;
  // cre_perm_src_img_tag ();

  // planar-surface (trimmed-perforated) - via code
  // cre_perm_src_surpln ();

  // B-spline-surf - via direct-code
  // cre_perm_surbsp ();           // via direct-code


  //================================================================
  // temporary objects;
  // - limited nr of objects  (~30)
  // - cannot be found by "Scale All"
  // - Rework ("END") will delete these objects
  // cre_temp_1 ();          


  //================================================================
  // create dynamic objects (not stored in DB, but visible, can move, zoom ..)
  // - any nr of objects
  // - cannot be found by "Scale All"
  // - Rework ("END") will delete these objects

  // Temporary-Dynamic binary-obj
  // cre_tDyn_obj ();

  // Temporary-Dynamic symbols
  // cre_tDyn_sym ();

  // Temporary-Dynamic text
  // cre_tDyn_txt ();

  // Temporary-Dynamic modelRefs;
  // cre_tDyn_mdr ();

  // hide / redisplay obj
  // obj_hide__ ();

  // remove obj
  obj_del__ ();


  //================================================================
// DO NOT USE; beeing replaced ..
  // cre_6 ();      // ruled surfaces
  // cre_7 ();      // revolved surfaces (perm)
  // cre_8 ();      // tesselated surfaces
  // cre_tDyn_sur_nifac1 ();    // indexed-faces
  // cre_tDyn_sur_nifac2 ();    // double-indexed-faces
  // cre_tDyn_surbsp ();        // create, display, dump
  // cre_DB_surbsp ();          // create, store in DB, load from DB, dump


  //================================================================
  UI_GR_ScalAuto (0);  // rescale all

  // // Redraw DispList
  // DL_Redraw ();



  //================================================================
  gCad_fini ();        // exit & reset

  return 0;

}


//================================================================
  int obj_del__ () {
//================================================================
// obj_del__                                remove obj
// - if objID starts with '_' obj is marked for deletion;
// - "_P20=P(10 20 30)"  - this point is not active;
// - to reactivate remove '_' and rework obj
// - see code with "MAN"


  int      irc, typ1, lLen;
  long     dbi1, dli1, lNr;
  char     sOid[80], *p1;
  Point    ptc = {0., 0., 0.,};
  Circ     ci1;



  // get circ
  UT3D_ci_ptvcr (&ci1, &ptc, (Vector*)&UT3D_VECTOR_Z, 15.);

  // save Circ in DB as C<dbi1>
  UTO_sav_ost (&typ1, &dbi1, Typ_CI, Typ_CI, 1, &ci1);
    printf("IB1-typ=%d dbi=%ld\n",typ1,dbi1);


  DL_Redraw ();
  TX_Print("- active - wait 2 seconds ..");
  GUI_update__ (); // else TX_Print-text not visible
  OS_Wait (2000);  // millisecs


  // get obj-ID; eg "C20"
  irc = APED_oid_dbo__ (sOid, typ1, dbi1);
  if(irc < 0) return irc;


  // get sourcelineNr <lNr> of obj <sOid>
  irc = APED_search_defLn (&p1, &lNr, &lLen, sOid, -1L, 0);
  if(irc < 0) return irc;


  // delete sourcelineNr <lNr>
  irc = APED_srcLn_del (p1, lLen);
  if(irc < 0) return irc;
  DL_Redraw ();
  TX_Print("- deleted - see \"MAN\" ..");


  // update editor after modification
  return APED_update (lNr);


  return 0;

}


//================================================================
  int obj_hide__ () {
//================================================================
// obj_hide__                       hide / redisplay obj


  int      irc, typ1;
  long     dbi1, dli1;
  Point    ptc = {0., 0., 0.,};
  Circ     ci1;



  // get circ 
  UT3D_ci_ptvcr (&ci1, &ptc, (Vector*)&UT3D_VECTOR_Z, 15.);

  // save Circ in DB as C<dbi1>
  UTO_sav_ost (&typ1, &dbi1, Typ_CI, Typ_CI, 1, &ci1);
    printf("IB1-typ=%d dbi=%ld\n",typ1,dbi1);

  // get dispListIndex from typ,dbi
  dli1 = DL_dli__dbo (typ1, dbi1, -1L);

  DL_Redraw ();
  TX_Print("- visible - wait 2 seconds ..");
  GUI_update__ (); // else TX_Print-text not visible
  OS_Wait (2000);  // millisecs


  DL_hide__ (dli1, 1);   // hide
  DL_Redraw ();
  TX_Print("- hidden - wait 2 seconds ..");
  GUI_update__ (); // else TX_Print-text not visible
  OS_Wait (2000);  // millisecs


  DL_hide__ (dli1, 0);   // reset
  DL_Redraw ();
  TX_Print("- OK");


  return 0;

}


//================================================================
  int cre_perm_src_img_tag () {
//================================================================
// cre_perm_src_img_tag             image, tag with text;
// - imag can be applied to 3D-point (billboard - always full size)
// - image can be applied to 3D-surface (texture); not shown here ..
//   - Tex_addBas1 APT_work_ATTS GA_Tex__

  long    dbi;         
  char    s1[512];


  printf("\ncre_perm_src_img_tag \n");

  UTF_clear1(); // init buffer for sourcecodes


  //----------------------------------------------------------------
  // image (billboard)
  // get next free DB-index for note, start always from index 20
  dbi = DB_QueryNxtFree (Typ_Note, 20); printf(" dbi=%ld\n",dbi);

  sprintf (s1,"N%ld=IMG P(20., 0., 0.) \"Data/SteinMarmor1.jpg\"",dbi);
    printf(" s1 |%s|\n",s1);

  // add s1 to buffer
  UTF_add1_line (s1);



  //----------------------------------------------------------------
  // tag; pos 0,0,0; color 5 (see INF_COL_SYMB);
  ++dbi; 
  sprintf (s1,"N%ld=TAG P(0., 0., 0.) 5 \"SteinMarmor1.jpg\"",dbi);
  UTF_add1_line (s1);



  //----------------------------------------------------------------
  UTF_insert1 (-1L);  // add buffer to model
  ED_load__ ();       // update model
  UI_but_END ();      // process additional src


  return 0;

}


//================================================================
  int cre_tDyn_obj () {
//================================================================
// cre_tDyn_obj                    disp. dynamic obj from binary-obj
// - pt ln ci curve


  int      i1;
  long     l1;
  Point    pt1, ptc;
  Line     ln1;
  Vector   vz = {0.,0., 1.};
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



  //----------------------------------------------------------------
  // create 2 points (only for automatic scaling) - temp-objs cannot scale; 
  i1 = cre_sav_obj (Typ_PT, "P(-200., -100., 0.)", 0);
  i1 = cre_sav_obj (Typ_PT, "P(200., 100., 0.)", 0);



  //----------------------------------------------------------------
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

  // display numbers at points
  for(i1=0; i1<4; ++i1) GR_tDyn_txiA (&pp1[i1], i1, 0);

  // display n points with attribute
  GR_tDyn_npt__ (4, pp1, ATT_PT_GREEN);




  // 3D-Circ
  UT3D_pt_3db (&pt1, 100., 100., 100.);
  UT3D_pt_3db (&ptc, 100., 100., 125.);
  UT3D_vc_3db (&vz, 0., 1., 0.);
  // circ from startPt, center, z-vector, opening angle (pos. = CCW)
  UT3D_ci_ptptvcangr (&ci1, &pt1, &ptc, &vz, UT_RADIANS(90.));
  // curve, dbi=unknown=0
  GR_tDyn_ocv (Typ_CI, &ci1, 0, Typ_Att_blue);



  // get memspc for B-Spline
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
  int cre_perm_src_surpln () {
//=========================================================
// cre_perm_src_surpln                       planare surf; trimmed / perforated

  int      irc, typ1, typ2;
  long     dbi1, dbi2;
  Point    pp1[]={600.,100., 0., 650.,100., 0.,
                  650.,200., 0., 600.,200., 0., 600.,100., 0.};
  Point    ptc = {625., 125., 0,};
  Circ     ci1;
  CurvPoly plg1;
  Memspc   memSeg1;


  printf("\ncre_perm_src_surpln \n");


  // save circ (for hole in surf (must be closed contour))
  UT3D_ci_ptvcr (&ci1, &ptc, (Vector*)&UT3D_VECTOR_Z, 15.);

  // save Circ in DB as C<dbi>
  UTO_sav_ost (&typ1, &dbi1, Typ_CI, Typ_CI, 1, &ci1);
    printf("IB1-typ=%d dbi=%ld\n",typ1,dbi1);




  // save closed Polygon pp1 from binary data
  UME_init (&memSeg1, myMemspc, sizeof(myMemspc));  // init memoryseg (tempspc)
  UT3D_plg_pta (&plg1, pp1, 5, &memSeg1);           // PolgonCurve from pt-tab

  // save polygon in DB as S<dbi>
  UTO_sav_ost (&typ2, &dbi2, Typ_CV, Typ_CVPOL, 1, &plg1);
    printf("OB-typ=%d dbi=%ld\n",typ2,dbi2);


  // save trimmed-perforated-surf
  SRC_add_surTP (mem_cbuf1, mem_cbuf1_SIZ, Typ_SURPLN,
                 typ1,  dbi1,                 // outer-boundary
                 typ2,  dbi2,                 // 1. inner-boundary
                -1);                          // end of list


    printf("ex-pcre_perm_src_surpln\n");

  return 0;

}


//================================================================
  int cre_perm_obj () {
//================================================================
// cre_perm_obj                      permanent objects from binary-obj
// - create source from binary-obj; store in srcBuf;
// - can be saved, analyzed, dumped;


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
  UT3D_pt_3db (&pta[2], 300., 150., 0.);
  UT3D_pt_3db (&pta[3], 350., 50., 0.);
  UT3D_pt_3db (&pta[4], 400., 80., 0.);


  // Polygon from 5 points
  iCv1 = UTO_sav_ost (&dbTyp, &dbInd, Typ_CVPOL, Typ_PT, 5, (void*)pta);



  // B-Spline-Curve, Degree 3  from points
  iCv2 = UTO_sav_ost (&dbTyp, &dbInd, Typ_CVBSP, Typ_PT, 5, (void*)pta);


  // clean undo-tables
  // UNDO_clear ();



  return 0;

}


//================================================================
  int cre_temp_1 () {
//================================================================
// cre_temp_1                   Create Temporary-objects:
// - max 30 aux.objects
// - cannot analyze, scale;
// - GR_temp_* for display; clear with GL_temp_del_1 (index);

  int     i1, dbi;
  Point   pt1={ 0., 0., 0.};
  Point   pt2={ 10., 10., 0.};
  Vector  vc1={ 1., 0., 2.};
  Plane   pln1;
  GText   txtg;



  //----------------------------------------------------------------
  // create 2 points (only for automatic scaling) - temp-objs cannot scale; 
  dbi = cre_sav_obj (Typ_PT, "P(-5., -1., 0.)", 0);
  dbi = cre_sav_obj (Typ_PT, "P(10., 10., 0.)", 0);


  //----------------------------------------------------------------
  // 2D-text (bitmap)
  // disp integers at position pt1
  pt1.y += 2;
  DL_temp_ind = 1;
  GR_temp_txiA (&pt1, 123, ATT_COL_RED);    // att: see INF_COL_SYMB
  pt1.x += 2;
  GR_temp_txiA (&pt1, 456, ATT_COL_GREEN);

  // disp 2D-text at position pt1
  pt1.x += 2;
  GR_temp_txtA (&pt1, "abc", ATT_COL_YELLOW);


  // disp scaled rotated text (grafic-text; 3D)
  pt1.x += 2.;
  txtg.pt   = pt1;
  txtg.size = 0.5;
  txtg.dir  = 30.;    // deg
  txtg.txt  = "Test";
  GR_temp_txtG (&txtg, Typ_Att_blue); // att: see INF_ATT_CV


  // points 
  pt1.x = 0;
  pt1.y += 2;
  GR_temp_pt (&pt1, ATT_PT_HILI);
  pt1.x += 2;
  GR_temp_pt (&pt1, ATT_PT_GREEN);
  pt1.x += 2;

  // use a fixed index; first is 0, last is <GL_temp_iLast()>
  //  - else next free index is used;
  DL_temp_ind = 8;     // use a fixed index
  GR_temp_pt (&pt1, ATT_PT_YELLOW);
    printf(" last temp-index is %ld\n",GL_temp_iLast());



  // vector  0=display normalized length; 1=display real length
  pt1.x = 0;
  pt1.y += 2;
  GR_temp_vc (&vc1, &pt1, ATT_COL_BLUE, 0);
  pt1.x += 2;
  GR_temp_vc (&vc1, &pt1, ATT_COL_GREEN, 1);


  // get index of last created temp-obj: delete it;
  pt1.x += 2;
  GR_temp_vc (&vc1, &pt1, ATT_COL_GREEN, 1);
  i1 = GL_temp_iNxt () - 1;   printf(" temp-index=%d\n",i1);
  GL_temp_del_1 (i1);

  // disp at screenCenter
  // GR_temp_vc (&vc1, NULL, ATT_COL_CYAN, 1);


  //================================================================
  // Symbols
  pt1.x = 0;
  pt1.y += 2;
  // plane  1=pln 4=Axis+Chars 5=Plane+Axis+Chars
  UT3D_pl_ptvc (&pln1, &pt1, &vc1);
  GR_temp_pln (&pln1, Typ_Att_Symb, 1);

  pt1.x += 2;
  UT3D_pl_ptvc (&pln1, &pt1, &vc1);
  GR_temp_pln (&pln1, Typ_Att_def, 4);  // 4=Axis+Chars


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
// cre_tDyn_mdr             add temporary modelRefs; internal subModel must exist;
//   (load or create subModel: eg call cre_perm_src)



  int       irc, i1, bmi;
  long      dbi;
  double    scale;
  Point     pt1 = { 20., 20., 0.};
  Plane     pl1;
  ModelRef  mdr;


  printf("\ncre_tDyn_mdr \n");

  //----------------------------------------------------------------
  // must create internal submodel;

  i1 = DB_get_ModBasNr();  // get nr of existing basic-models
    printf(" DYN_MB_IND=%d\n",i1);

  if(i1 < 1) {
    // load catalogPart at pos 10,10,0
    cre_sav_obj (Typ_Model, "\"Schrauben/SKS_6x30.ctlg\" R(PERP P(10 10 0) DZ)", 0);
  }

  i1 = DB_get_ModBasNr();
    printf(" DYN_MB_IND=%d\n",i1);


  //----------------------------------------------------------------
  // get refSys (origin & orientation) for modelRef from origin (pt1) and z-vec;
  UT3D_pl_ptvc (&pl1, &pt1, (Vector*)&UT3D_VECTOR_Z);
    DEB_dump_obj__ (Typ_PLN, &pl1, "  temp_mdr-pl1: ");

  // disp (already existing) internal subModel, defined by its basicModelNr bmi
  // create modelRef
  bmi = 0;      // internal subModel <bmi> must exist !
  scale = 1.;
  // set modelRef from basicModelNr and refSys
  Mod_mdr__bmi_pln (&mdr, bmi, &pl1, scale);
    DEB_dump_obj__ (Typ_Model, &mdr, "  temp_mdb-mdr: ");

  // display temp:
  // DL_temp_ind = 8;   // set fixed GL-index
  // irc = GR_temp_mdr (&mdr, GR_TMP_HILI);
              // modelRef,    att:  GR_TMP_DEF|GR_TMP_HILI|GR_TMP_DIM

  // disp. temp.Dyn:
  irc = GR_tDyn_mdr (&mdr, GR_TMP_HILI);
              // modelRef,    att:  GR_TMP_DEF|GR_TMP_HILI|GR_TMP_DIM



  return irc;

}


//================================================================
  int cre_tDyn_sym () {
//================================================================
// cre_tDyn_sym              temporary-dynamic objects (test objects) - symbols
// - Rework ("END") will delete these objects
// - vectors symbols (star triangle ..)


  static long    startInd=0;

  int     i1;
  double  d1;
  Point   pt1 = { 0., -5., 0.};
  Point2  pt_2;
  Vector  vc1 = { 2., 1., 3.}, vc2;
  Vector2 vc_2;



  printf("cre_tDyn_sym \n");

  //----------------------------------------------------------------
  // create 2 points (only for automatic scaling) - temp-objs cannot scale; 
  i1 = cre_sav_obj (Typ_PT, "P(-5., -15., 0.)", 0);
  i1 = cre_sav_obj (Typ_PT, "P(15., 0., 0.)", 0);



  //----------------------------------------------------------------
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


  // vectors 3D-true-length
  for(i1=0; i1<5; ++i1) {
    pt1.x += 2.;
    GR_tDyn_vc__ (&vc1, &pt1, ATT_COL_BLUE, 1);  // true length
    UT3D_vc_mult_d (&vc2, &UT3D_VECTOR_Y, i1+1); // pt1.x);
    GR_tDyn_vc__ (&vc2, &pt1, ATT_COL_RED, 1);  // true length
  }


  // symbols in 2D-plane; scaled, rotated  
  pt1.x = 0.;
  pt1.y -= 2.;
//   GR_tDyn_symV2 (SYM_ARROW, ATT_COL_BLACK, &pt1, NULL, 12.);
//     pt1.x += 2.;
  GR_tDyn_symV2 (SYM_SQUARE, ATT_COL_RED, &pt1, NULL, 1.);
    pt1.x += 2.;
  GR_tDyn_symV2 (SYM_PLANE, ATT_COL_GREEN, &pt1, NULL, 1.);
    pt1.x += 2.;
  GR_tDyn_symV2 (SYM_AXIS1, ATT_COL_BLUE, &pt1, NULL, 1.);
    pt1.x += 2.;
  GR_tDyn_symV2 (SYM_AXIS, ATT_COL_YELLOW, &pt1, NULL, 1.);
    pt1.x += 2.;
  GR_tDyn_symV2 (SYM_CROSS, ATT_COL_MAGENTA, &pt1, NULL, 1.);
    pt1.x += 2.;
  GR_tDyn_symV2 (SYM_CROSS1, ATT_COL_CYAN, &pt1, NULL, 1.);
    pt1.x += 2.;
  GR_tDyn_symV2 (SYM_TRIANG, ATT_COL_WHITE, &pt1, NULL, 10.);


  // bitmap-symbols
  pt1.x = 0.;
  pt1.y -= 2.;
  GR_tDyn_symB__ (&pt1, SYM_TRI_S, ATT_COL_BLACK);
    pt1.x += 2.;
  GR_tDyn_symB__ (&pt1, SYM_STAR_S, ATT_COL_RED);
    pt1.x += 2.;
  GR_tDyn_symB__ (&pt1, SYM_CIR_S, ATT_COL_GREEN);
    pt1.x += 2.;
  GR_tDyn_symB__ (&pt1, SYM_SQU_S, ATT_COL_BLUE);
    pt1.x += 2.;
  GR_tDyn_symB__ (&pt1, SYM_TRI_B, ATT_COL_YELLOW);
    pt1.x += 2.;
  GR_tDyn_symB__ (&pt1, SYM_SQU_B, ATT_COL_MAGENTA);



  // 2D-vectors, normalized
  pt_2.x = 0.;
  pt_2.y = pt1.y - 2.;
  for(d1=0.; d1<359.; d1 += 30.) {
    UT2D_vc_angr (&vc_2, UT_RADIANS(d1));
      printf(" vc_2 = %f %f len=%f\n",vc_2.dx,vc_2.dy,UT2D_len_vc(&vc_2));
    GR_tDyn_vc2__ (&vc_2, &pt_2, ATT_COL_BLACK, 0);      // ATT_COL_T_GREEN
  }


  // 2D-vectors, true-length, linetype thick
  vc_2.dx = 2;
  vc_2.dy = 0;
  GR_tDyn_vc2__ (&vc_2, &pt_2, ATT_COL_T_GREEN, 1);


  //----------------------------------------------------------------
  // remove all temporary-dynamic objects:
  // if(startInd) {GL_Delete (startInd); startInd=0;}

  return 0;

}


//================================================================
  int cre_tDyn_txt () {
//================================================================
// cre_tDyn_txt                temporary-dynamic text;  (test objects)
// Rework ("END") will delete these objects


  static long    startInd=0;

  int     i1;
  Point   pt1 = { 0., 8., 0.};
  GText   txtg;


  printf("cre_tDyn_txt \n");


  //----------------------------------------------------------------
  // create 2 points (only for automatic scaling) - temp-objs cannot scale; 
  i1 = cre_sav_obj (Typ_PT, "P(-1., 5., 0.)", 0);
  i1 = cre_sav_obj (Typ_PT, "P(6., 10., 0.)", 0);



  //----------------------------------------------------------------
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
// cre_perm_src                   permanent objects from sourceCode
// - clear buffer 
// - create source-text for perm.obj. and add to buffer
// - add buffer to model
// - update model

  long    dbi;                        // DataBase-index
  char    s1[256];
  Point   pt1={ 20., 0., 0.};


  //----------------------------------------------------------------
  // init buffer for sourcecodes
  UTF_clear1();


  //----------------------------------------------------------------
  // get next free DB-index for point, start always from index 20
  dbi = DB_QueryNxtFree (Typ_PT, 20); printf(" dbiPT1=%ld\n",dbi);

  // create source for point
  sprintf (s1,"P%ld=P(20., 0., 10.)",dbi);

  // add source into buffer
  UTF_add1_line (s1);

  // one more point
  ++dbi;
  sprintf (s1,"P%ld=P(20., 0., 20.)",dbi);
  UTF_add1_line (s1);


  //----------------------------------------------------------------
  // add a catalog-part as internal subModel
  // get next free dbi for modelReference
  dbi = DB_QueryNxtFree (Typ_Model, 20);
    printf(" dbiNxtM=%ld\n",dbi);
  // create source
  sprintf (s1,"M%ld=\"Schrauben/SKS_6x30.ctlg\" R(PERP P(10 10 0) DZ)",dbi);
  // add source into buffer
  UTF_add1_line (s1);


  //----------------------------------------------------------------
  // add buffer to model
  UTF_insert1 (-1L);  // add buffer to model
  ED_load__ ();     // update model

  UI_but_END ();    // process additional src


  return 0;

}


//================================================================
  int cre_perm_surbsp () {
//================================================================
//         // via code

  // clear ("new")
  UI_men__ ( "new");

  UTF_clear1();
  UTF_add1_line ("S20=POL P(10.,10.,0.) P(20.,10.,0.) P(30.,10.,0.)");
  UTF_add1_line ("S21=POL P(10.,13.,0.) P(20.,13.,5.) P(30.,13.,0.)");
  UTF_add1_line ("S22=POL P(10.,16.,0.) P(20.,16.,0.) P(30.,16.,0.)");
  UTF_add1_line ("A20=BSP U(S20 S21 S22)");

  // Add Aux.Buffer -> MainBuffer
  UTF_insert1 (-1L);

  // execute 
  UI_but_END ();

  return 0;

}


//================================================================
  int cre_DB_surbsp () {
//================================================================
// cre_DB_surbsp ();          // create, store in DB, load from DB, dump

  int      irc, i1, oTyp, sTyp, oNr;
  long     dbi;
  Memspc   msSu1;
  char     spc1[10000];  // space for surf
  ObjGX    ox1;
  void     *obj, *sur;


  printf("--------------- cre_DB_surbsp\n");

  // get ox1 = b-spline-surface
  UME_init (&msSu1, spc1, sizeof(spc1));
  irc = cre_get_surbsp (&ox1, &msSu1);
  if(irc < 0) {TX_Error("cre_DB_surbsp E1"); return -1;}


  // store surf in DB as A20
  dbi = 20L;
  irc = DB_StoreSur (&dbi, &ox1);


  // load A20 from DB (returns TypObjGX = ObjGX)
  oTyp = UTO__dbo (&obj, &oNr, Typ_SUR, dbi);
  if(oTyp < 0) {TX_Error("cre_DB_surbsp E2"); return -1;}
    DEB_dump_ox_s_ (obj, "cre_DB_surbsp-obj");

  // ox2 = DB_GetObjGX (Typ_SUR, dbi);
  // if(ox2.typ == Typ_Error) {TX_Error("cre_DB_surbsp E2"); return -1;}
    // DEB_dump_ox_s_ (&ox2, "cre_DB_surbsp-ox2");

  // display
  GR_tDyn_osu (obj, 0L, ATT_COL_GREEN);

  // unpack ObjGX
  OGX_GET_OBJ (&sTyp, &sur, &oNr, (ObjGX*)obj);

  // dump
  DEB_dump_obj__ (sTyp, sur, "cre_DB_surbsp-sur");

  return 0;

}

  
//================================================================
  int cre_tDyn_surbsp () {
//================================================================
// cre_tDyn_surbsp            create b-spline-surface, disp dynam., dump

  int      irc, oTyp, oNr;
  Memspc   msSu1;
  char     spc1[10000];  // space for surf
  ObjGX    ox1;
  void     *obj;


  //----------------------------------------------------------------
  // get ox1 = b-spline-surface
  UME_init (&msSu1, spc1, sizeof(spc1));
  irc = cre_get_surbsp (&ox1, &msSu1);
  if(irc < 0) {TX_Error("cre_DB_surbsp E1"); return -1;}

  // display
  GR_tDyn_osu (&ox1, 0L, ATT_COL_GREEN);
    DEB_dump_ox_s_ (&ox1, "cre_tDyn_surbsp-ox1");

  // unpack ObjGX
  OGX_GET_OBJ (&oTyp, &obj, &oNr, &ox1);

  // dump obj
  DEB_dump_obj__ (oTyp, obj, "cre_tDyn_surbsp-su1");

  return 0;

}


//================================================================
  int cre_get_surbsp (ObjGX *ox1, Memspc *msSu1) {
//================================================================
// test_msh3d_getSur                 test 3D-tesselate
// test tesselate simple blspl-surf

  int      irc, i1;
  Point    cv1[] = {{10.,10.,0.},{20.,10.,0.},{30.,10.,0.}};
  Point    cv2[] = {{10.,13.,0.},{20.,13.,5.},{30.,13.,0.}};
  Point    cv3[] = {{10.,16.,0.},{20.,16.,0.},{30.,16.,0.}};
  CurvBSpl cvTab[3];
  SurBSpl  su1;
  Memspc   msCv, msTb1, msTb2;
  char     spc1[10000];  // space for curves
  void     *cv1tab[3];


  //----------------------------------------------------------------
  // create su1 = b-spline-surface

  // get mespc for B-Spline
  UME_init (&msCv, spc1, sizeof(spc1));

  // B-Spline-Curve, Degree 3  from points
  //           (cvBsp, &memSeg1, pTab, ptNr, deg);
  UT3D_cbsp_ptn (&cvTab[0], &msCv, cv1,   3,   2);
  UT3D_cbsp_ptn (&cvTab[1], &msCv, cv2,   3,   2);
  UT3D_cbsp_ptn (&cvTab[2], &msCv, cv3,   3,   2);

    // disp
    // GR_tDyn_ocv (Typ_CVBSP, &cvTab[0], 0, Typ_Att_blue);
    // GR_tDyn_ocv (Typ_CVBSP, &cvTab[1], 0, Typ_Att_blue);
    // GR_tDyn_ocv (Typ_CVBSP, &cvTab[2], 0, Typ_Att_blue);


  // get temp memspc
  UME_init (&msTb1, memspc101, sizeof(memspc101));   // temSpc
  UME_init (&msTb2, memspc102, sizeof(memspc102));   // temSpc

  // get cv1tab = pointers to 3 curves
  for(i1=0; i1<3; ++i1) cv1tab[i1] = &cvTab[i1];

  // get su1 = bslpl-surf from table-of-curves
  irc = UT3D_sbsp_ncv (&su1, msSu1, 3, cv1tab, 2, &msTb1, &msTb2);
  if(irc < 0) {TX_Error("test_msh3d_getSur E1"); return -1;}
    // DEB_dump_obj__ (Typ_SURBSP, &su1, "cre_get_surbsp-su1");

  // pack into complex-obj
  OGX_SET_OBJ (ox1, Typ_SURBSP, Typ_SURBSP, 1, &su1);
    // DEB_dump_ox_s_ (ox1, "cre_get_surbsp-ox1");

  // display
  // GR_tDyn_osu (ox1, 0L, ATT_COL_GREEN);

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
  GR_tDyn_nifac (-1L, &nfa, 0);

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
  GR_tDyn_nifac (-1L, &nifa, 0);

  return 0;

}



//================================================================
  int cre_sav_obj (int typ, char *src, int att) {
//================================================================
// cre_sav_obj                  store DB-obj from source

  long       dbi;
  char       oid[80];


  dbi = DB_QueryNxtFree (typ, 20);

  APED_oid_dbo__ (oid, typ, dbi);
    printf("cre_sav_obj typ=%d dbi=%ld oid |%s|\n",typ,dbi,oid);

  sprintf(mem_cbuf1, "%s=%s", oid, src);
    printf("cre_sav_obj-src |%s|\n",mem_cbuf1);

  ED_srcLn_add (mem_cbuf1, 0);              // 2010-11-29


  return (int)dbi;

}


//======================== EOF ======================
