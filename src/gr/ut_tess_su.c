//             Tesselate Surfaces                 21014-03-06    RF
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
- make UT3D_pt_par_swp faster;
  -) keep all planes (parU-value == 3D-plane)
  -) keep all contour-points (parV-value == 2D-point)

-----------------------------------------------------
Modifications:
  ..

-----------------------------------------------------
*/
/*!
\file  ../gr/ut_tess_su.c
\brief Tesselate Surfaces  (New version; old version=../gr/tess_su.c)
\code
=====================================================
List_functions_start:

Tess_sur__           tesselate, display sweep-surface
Tess_sSym__          draw symbolic sweep-surf
Tess_sSym_swp__
Tess_sSym_swp_1
Tess_sSym_swp_2
Tess_sur_bnd_swp__   create edges for unlimited sweep-surface
Tess_sur_td_ci_obj   save tesselation-date for sweep with Contour=Circ
Tess_sur_grd__       compute all gridbox-points
Tess_sur_bnd__       create edges for surface

Tess_tr_3D_2D_swp    get 3D-point from 2D-point on sweep-surface
UT3D_pt_par_swp      translate 2D-pt from parU,parV back to 3D.
Tess_ptNr_SURSWP     get nr of points for sweep-surf

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



TESTMODELS:
../gr/ut_tess_su.c



===============================*/






#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>



#include "../ut/ut_geo.h"                 // Point ...
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_itmsh.h"            // MSHIG_EDGLN_.. typedef_MemTab.. Fac3
#include "../ut/ut_ox_base.h"             // OGX_SET_INDEX
#include "../ut/ut_os.h"                  // OS_..
#include "../ut/ut_plg.h"                 // UT3D_par_par1plg
#include "../ut/ut_face.h"                 // MSH_EDGLN_OB
#include "../ut/ut_col.h"                 // COL_INT32
#include "../ut/gr_types.h"                  // UI_Func... SYM_..

#include "../db/ut_DB.h"                  // DB_GetGTxt

#include "../gr/tess_su.h"                // TypTsuSur

#include "../xa/xa_mem.h"                 // memspc51..
#include "../xa/xa.h"                     // AP_STAT
#include "../xa/xa_ga.h"                  // GA_tex_ga2tr


// typedef_MemTab(int);
// typedef_MemTab(char);
// typedef_MemTab(Point);
// typedef_MemTab(IndTab);

typedef_MemTab(SegBnd);
typedef_MemTab(BndSur);


static MemTab(Point)    pas1 = _MEMTAB_NUL;
static MemTab(Point)    pa2D = _MEMTAB_NUL;
static MemTab(char)     pst2D = _MEMTAB_NUL;
static MemTab(int)      ia2D = _MEMTAB_NUL;
static MemTab(IndTab)   ca2D = _MEMTAB_NUL;


// tesselation-data for sweep-surfaces
typedef struct {int typPath, typCov; void *datPath, *datCov;
                Plane pl1, plb; Circ2C ci2c;}       TessDat_SWP;


#define TessDatSiz sizeof(TessDat_SWP)   // 144


// tesselation-data for surfaces
// typ      SWP-Sur: Typ_SURSWP
// tr_*     functions 3D to 2D and retour
// td       individual for surface-type;
//          TessDat_SWP if typ = Typ_SURSWP
typedef struct {int typ, A_id, B_id;
                double rx, ry, dx, dy, tol;
                int (*tr_2D_3D)(); int (*tr_3D_2D)();
                char td[TessDatSiz];}                                TessDat__;



  int Tess_tr_2D_3D_swp (Point *pt2, Point *pt3);
  int Tess_tr_3D_2D_swp (Point *pt3, Point *pt2, TessDat_SWP *swp);



//================================================================
// Extern Vars:

// from ../ci/NC_Main.h
extern int     APT_dispSOL;           // 0=ON=shade; 1=OFF=symbolic
extern long    AP_dli_act;      // index dispList

// from ../gr/tess_su.c:
extern int     TSU_mode;   // 0=normal darstellen; 1=speichern

// from ../gr/ut_GLU.c:
extern Point     *GLT_pta;
// extern long      GLT_pta_SIZ;







//================================================================
  int Tess_sur__ (ObjGX *oxi, int att, long dbi) {
//================================================================
// tesselate, display sweep-surface.

// pst2D: 
//   bit0 = point is on boundary (val 1)
//   bit1 = 2D-value set (val 2)
//   bit2 = 3D-value set (val 4)

// new version of TSU_DrawSurTC < TSU_DrawSurT_

  int       sTyp, irc, i1, i2, ii, bNr, itbMax, pNr;
  long      ld, dli;
  char      c1, *pc1;
  // double    tol;
  void      *sDat;
  ObjGX     *ox1;
  Point     *pa3, *pa2; 
  IndTab    *itAct;     
  TessDat__ td1;

  GridBox  gbh1;
  IndTab    *itBnd;
  MemTab(IndTab) fmt=_MEMTAB_NUL;
  ColRGB    *col1;

  // int TSU_mode, TSU_sStyl, APT_dispSOL; // GLOBAL !
  int  TSU_sStyl;



  printf("TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT \n");
  printf("Tess_sur__ %ld\n",dbi);
  DEB_dump_obj__ (Typ_ObjGX, oxi, "");
    // DEB_dump_ox_0 (ox1, "A%ld",dbi);


//   TSU_dli = AP_dli_act;

  col1 = COL_INT32(&att);   // col1 = (ColRGB*)&att;

  // TEST ONLY:
  // col1->vsym = 1;  //set symbolic
  // TEST END





  //----------------------------------------------------------------
  sTyp = UTO_objDat_ox (&sDat, &i1, oxi);
  if(sTyp < 1) return -1;
    // DEB_dump_obj__ (sTyp, sDat, "sDat: ");


  if(TSU_mode == 0) {  //0=disp; 1=store.
    td1.tol = UT_DISP_cv * 4;
  } else {
    td1.tol = UT_DISP_cv;
  }

// TODO: can use GLT_pta for points ?
  // check nr of points
  pNr = UT3D_ptNr_obj (sTyp, sDat, td1.tol);
    printf(" sur__pNr=%d\n",pNr);
  pNr += 100;
  pNr *= 3; // for gridpoints; for new tesselated points; ..
    printf(" sur__pNr=%d\n",pNr);

  // // realloc GLT_pta if necessary
  // if(pNr >= GLT_pta_SIZ) {
    // GLT_alloc_pta ((long)pNr);
  // }


  //----------------------------------------------------------------
    // TEST ONLY
    // { ColRGB col2;
      // UTcol__3i (&col2, 100, 100, 5);
      // col2.vsym = 1;      // 0=ON=shade; 1=OFF=symbolic    <<<<<<<<<<<<
      // memcpy(&att, &col2, sizeof(int));
        // printf(" att=%d\n",att);
    // }
  // TSU_mode = 0; APT_dispSOL = ON;


  //----------------------------------------------------------------
  // TEST SYMBOLIC
    // DEB_dump_obj__ (Typ_Color, col1, "sur-col1");
  TSU_sStyl = 0;            // 0=ON=shade; 1=OFF=symbolic
  if(TSU_mode == 0)  {      // 0=draw; 1=store
    if((APT_dispSOL == OFF) || (col1->vsym  == 1)) TSU_sStyl = 1;
  }
    printf(" TSU_mode=%d TSU_sStyl=%d APT_dispSOL=%d\n",
           TSU_mode, TSU_sStyl, APT_dispSOL);



  //----------------------------------------------------------------
  // draw symbolic; done.
  if(TSU_sStyl > 0) {
    GL_att_cv (Typ_Att_dash_long);  // attrib for symbolic
    return Tess_sSym__ (&td1, sTyp, sDat, att, dbi);
  }


  //----------------------------------------------------------------
  // get memspc for pNr points
  // 2D-points - REALLOCATE NOT ALLOWED (OpenGL keeps pointers)
  MemTab_ini__ (&pa2D, sizeof(Point), Typ_PT, pNr);

  // 3D-points - REALLOCATE NOT ALLOWED (OpenGL keeps pointers)
  MemTab_ini__ (&pas1, sizeof(Point), Typ_PT, pNr);

  // point-flags
  MemTab_ini__ (&pst2D, sizeof(char), Typ_Int1, pNr);

  // tesselated-faces
  ii = pNr / 3;
  MemTab_ini__ (&fmt, sizeof(IndTab), Typ_IndTab, ii);

  // point-indextables
  MemTab_ini__ (&ia2D, sizeof(int), Typ_Int4, pNr * 2);

  // contours
  MemTab_ini__ (&ca2D, sizeof(IndTab), Typ_IndTab, 100);



  //----------------------------------------------------------------
  // load 3D-outer- and inner-boundaries
  // add 2D-points > pa2D; indices > imt.
  // load td1 with all necessary data for tesselation.
  // was SUP_load_c
  bNr = Tess_sur_bnd__ (&td1, sTyp, sDat, dbi);
  if(bNr < 0) goto L_exit;
    printf(" pmt-ind=%d\n",MEMTAB_IND (&pa2D));

    // TESTBLOCK
    // // Testdisp. 3D-boundaries
    // GR_tDyn_npt__ (cTab[0].iNr, cTab[0].pa, SYM_STAR_S, 2);
    // for(i1=0; i1<cNr; ++i1)
    // GR_tDyn_txiA (&cTab[i1].pa[i2], i2, 0);
    // return 0;
    // END TESTBLOCK

  //----------------------------------------------------------------
  // get 2D-boundaries from 3D-boundaries
  // make same nr of points fro 2D and 3D
  // get 2D from 3D (loop tru 2D, check for uninitialized)
  // was TSU_tr_cont_

  // Testdisp. 2D-boundaries

  //----------------------------------------------------------------
  // remove 2D-points with deviation < minTol
  // was UT2D_cv3_linear

  // Testdisp. 2D-boundaries after removing points

  // Test 2D-points back > 3D


  //----------------------------------------------------------------
  // fill 2D-gridbox gb; all all points to pa2D
  irc = Tess_sur_grd__ (&gbh1, &td1);
  if(irc < 0) goto L_exit;   //return -1;

  // add statuswords for the gridPoints
  i1 = MEMTAB_IND (&pa2D);
  i2 = MEMTAB_IND (&pst2D);
  ii = i1 - i2;
    // printf(" 2D-ptNr=%d ptStatNr=%d %d\n",i1,i2,ii);
  if(ii > 0) {
    // add point-status
    c1 = 2;
    irc = MemTab_add (&pst2D, &ld, &c1, ii, 4);
    if(irc < 0) return irc;
  }
    printf(" after _grd__ 2D-ptNr=%d ptStatNr=%d\n",
            MEMTAB_IND(&pa2D),MEMTAB_IND(&pst2D));



  //----------------------------------------------------------------
  // tesselate 2D; fill fmt.
  irc = GLU_tess__ (&fmt, &pst2D, &ia2D, &pa2D, &ca2D, &gbh1, NULL);
  // if(irc == 1) goto L_noAmoi;
  if(irc) {
    TX_Print ("***** GLU_tess__ error %d %c%ld",irc,AP_typChar_typ(sTyp),dbi);
      printf(" sTyp=%d\n",sTyp);
    goto L_exit;
  }

    // Testdisp. 2D-faces
    // ii = MEMTAB_IND (&fmt); for(i1=0;i1<ii;++i1)
      // DEB_dump_obj__(Typ_IndTab,MEMTAB__ (&fmt,i1),"fmt[%d]",i1);
    // GR_Disp_iSur (att, &fmt, &ia2D, &pa2D);  return -1;


  //----------------------------------------------------------------
  // get 3D-points from 2D-points

  // make same nr of points for 2D and 3D
  i1 = MEMTAB_IND (&pa2D);
  i2 = MEMTAB_IND (&pas1); // nr of 3D-pts
  ii = i1 - i2;
  if(ii > 0) {
    irc = MemTab_add (&pas1, &ld, NULL, ii, 1);
    if(irc < 0) return irc;
  }
    // printf(" 2D-ptNr=%d 3D-ptNr=%d stat=%d\n",MEMTAB_IND(&pa2D),
           // MEMTAB_IND(&pas1), MEMTAB_IND(&pst2D));



  // get 3D-point from 2D-point
  i1 = MEMTAB_IND (&pa2D);
  pc1 = MEMTAB__ (&pst2D, 0);   // stat
  pa2 = MEMTAB__ (&pa2D, 0);    // 2D-pts
  pa3 = MEMTAB__ (&pas1, 0);    // 3D-pts


  // translate 3D <- 2D
  for(ii=0; ii<i1; ++ii) {
    // pst2D: 1=2D-val; 3=3D-val.
      // printf(" 2D_3D pas1[%d]=%d %d\n",ii,pc1[ii],BIT_GET(pc1[ii],4));
    // skip all unused points
    // test if 3D-coords already exist
    if(!(BIT_GET(pc1[ii],8))) continue;
    // translate 3D <- 2D ( Tess_tr_3D_2D_swp UT3D_pt_par_swp)
    td1.tr_3D_2D (&pa3[ii], &pa2[ii], td1.td);
  }
    // printf(" 2D-points: %d\n",i1);
    // printf(" 3D-points: %d\n",ii);


    // // Testdisp. 3D-faces
    // GR_Disp_iSur (att, &fmt, &ia2D, &pas1);



  //----------------------------------------------------------------
  // display or store
  if(TSU_mode == 0) {
//     dli = DL_StoreObj (Typ_SUR, dbi, att);
    GR_Draw_nipatch (&dli, att, &fmt, &ia2D, &pas1);
//     AP_dli_act = dli;

  } else {
    // iSur -> Sur
    Tess_bMsh_iMsh (&fmt, &ia2D, &pas1);
    // TSU_store (gSur);
  }


  L_exit:

  MemTab_free (&ca2D);
  MemTab_free (&ia2D);
  MemTab_free (&pst2D);
  MemTab_free (&fmt);
  MemTab_free (&pa2D);
  MemTab_free (&pas1);

  return 0;

}


//============================================================================
  int Tess_sSym__ (TessDat__ *td1, int sTyp, void *sDat, int att, long suID) {
//============================================================================
// draw symbolic sweep-surf


  // printf("TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT \n");
  // DEB_dump_obj__ (sTyp, sDat, "Tess_sSym__:");



  switch (sTyp) {

    case Typ_SURSWP:
      // get tess-date
      Tess_sur_bnd_swp__ (td1, sDat, suID, 1);
      return Tess_sSym_swp__ (td1, sDat, att, suID);

    default:
      TX_Error("Tess_sSym__ NYI typ=%d",sTyp);
  }


  return -1;

}


//======================================================================
  int Tess_sSym_swp__ (TessDat__ *td1, void *sDat, int att, long suID) {
//======================================================================
// draw symbolic sweep-surf; spine = b-spline

// see also GR_sSym_spl


  // DEB_dump_obj__ (Typ_SURSWP, sDat, "Tess_sSym_swp__:");
  // printf("  td1.typ=%d\n",td1->typ);


  // path=LN: cyl  Typ_SURCON
  if(td1->typ == Typ_SURSWP) {
    Tess_sSym_swp_1 (td1, sDat, att, suID);

/*
  if(td1->typ == Typ_SURCON) {
    Tess_sSym_swp_con__ ((void*)td1->td, sDat, att, suID);

  // path=CI: torus Typ_SURTOR
  } else if(td1->typ == Typ_SURTOR) {
    Tess_sSym_swp_tor__ ((void*)td1->td, sDat, att, suID);

  // path=bspl: pipe Typ_SURSWP
  } else if(td1->typ == Typ_SURSWP) {
    Tess_sSym_swp_1 ((void*)td1->td, sDat, att, suID);
*/

  } else {
    TX_Error("Tess_sSym_swp__ E001 %d",td1->typ);
    return -1;
  }

  return 0;

}

//======================================================================
  int Tess_sSym_swp_1 (TessDat__ *tdSwp, void *sDat, int att, long suID) {
//======================================================================
// draw symbolic sweep-surf 

// see also GR_sSym_spl

  int         i1, cvMax, cvNr, pNr, ipa, pMax;
  long        dli;
  double      par1, tol;
  Point       pt1, *pTab;
  Vector      vc1;
  Plane       pl0, pl1, pl2;
  ObjGX       oo, *cvTab;
  TraRot      tra1;
  Mat_4x3     m0, im0, m1, im1;
  TessDat_SWP *td1;


  // DEB_dump_obj__ (Typ_SURSWP, sDat, "Tess_sSym_swp_1:");
  // DEB_dump_obj__ (Typ_PLN, &td1->pl1, "  SWP-pl1:");

  td1 = (TessDat_SWP*)tdSwp->td;
  tol = tdSwp->tol;
  // suID = tdSwp->A_id;


  cvTab = (ObjGX*)memspc54;
  cvMax = sizeof(memspc54) / sizeof(ObjGX);

  pTab = (Point*)memspc201;
  pMax = sizeof(memspc201) / sizeof(Point);
    // printf(" cvMax=%d pMax=%d\n",cvMax,pMax);


  //----------------------------------------------------------------
  // get basic-plane pl0
  // copy the basic-plane
  pl0 = td1->pl1;
  // get matrix for basic-plane
  UT3D_m3_loadpl (m0, &pl0);
  // get inverse-matrix for basic-plane
  UT3D_m3_invm3  (im0, m0);


  //----------------------------------------------------------------
  // get contour pTab[pNr] on basic-plane pl0
  // contour: typ=td1->typCov  data=td1->datCov
  // spine:   typ=td1->typPath data=td1->datPath
  // get contour
  pNr = 0;
  i1 = UT3D_npt_obj (&pNr, &pTab[0], pMax, td1->typCov, td1->datCov, 1, tol, 2);
  if(i1 < 0) return -1;
  // transfer points onto basic-plane pl1
  for(i1=0; i1<pNr; ++i1) {
    UT3D_pt_tra_pt_m3 (&pTab[i1], im0, &pTab[i1]);
      // GR_tDyn_obj (Typ_PT, &pTab[i1], ATT_PT_GREEN, 0);
  }

  ipa = pNr;
  cvNr = 0;


  //----------------------------------------------------------------
  // get points pl1 at spine-par=0.0;
  Tess_sSym_swp_2 (&pTab[ipa],
                     pTab, pNr, &pl0, td1->typPath, td1->datPath, 0.0);

  // add as curve
  OGX_SET_OBJ (&cvTab[cvNr], Typ_PT, Typ_PT, pNr, &pTab[ipa]);
  cvNr += 1;
  ipa += pNr;


  //----------------------------------------------------------------
  // get points pl1 at spine-par=0.5;
  // get new refSys on curve ar parameter par; add points to pTab.
  Tess_sSym_swp_2 (&pTab[ipa],
                     pTab, pNr, &pl0, td1->typPath, td1->datPath, 0.5);

  // add as curve
  OGX_SET_OBJ (&cvTab[cvNr], Typ_PT, Typ_PT, pNr, &pTab[ipa]);
  cvNr += 1;
  ipa += pNr;

  //----------------------------------------------------------------
  // get points pl1 at spine-par=1.0;
  // get new refSys on curve ar parameter par; add points to pTab.
  Tess_sSym_swp_2 (&pTab[ipa],
                     pTab, pNr, &pl0, td1->typPath, td1->datPath, 1.0);

  // add as curve
  OGX_SET_OBJ (&cvTab[cvNr], Typ_PT, Typ_PT, pNr, &pTab[ipa]);
  cvNr += 1;
  ipa += pNr;

  //----------------------------------------------------------------
  // get polygon from spine
  // pNr = pMax - ipa; // max nr of points
  // UT3D_npt_obj (&pNr, &pTab[ipa], td1->typPath, td1->datPath, 1, tol);
  pNr = 0; // max nr of points
  UT3D_npt_obj (&pNr, &pTab[ipa], pMax - ipa, td1->typPath, td1->datPath,
                1, tol, 1);

  // add as curve
  OGX_SET_OBJ (&cvTab[cvNr], Typ_PT, Typ_PT, pNr, &pTab[ipa]);
  cvNr += 1;
  ipa += pNr;


  //----------------------------------------------------------------
  // create GL_CV-record
  oo.typ  = Typ_GL_CV;
  oo.form = Typ_ObjGX;
  oo.siz  = cvNr;
  oo.data = cvTab;

  // display
  dli = DL_StoreObj (Typ_SUR, suID, att);
  GL_Draw_obj (&dli, Typ_Att_dash_long, &oo);


  return 0;


  L_NYI:
  TX_Error("Tess_sSym_swp_1 NYI %d %d",td1->typPath,td1->typCov);
  return -1;

}

//================================================================
  int Tess_sSym_swp_2 (Point *pao,
                         Point *pai, int pNr,
                         Plane *pl0,
                         int typPath, void *datPath, double par1) {
//================================================================
// get contour pTab[pcNr] to pTab[ipa] on plane pl1
// Input:
//   pl0   base-plane

  int       i1, irc;
  Point     pt1;
  Vector    vc1;
  Plane     pl1;
  Mat_4x3   m1;


  // printf("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS \n");
  // DEB_dump_obj__ (Typ_PLN, pl0, "Tess_sSym_swp_2");
  // GR_tDyn_obj (Typ_PLN, pl0, 8, 4);


  // get point and tangent-vector on curve at parameter par1
  // UTO_pt_par1_obj (&pt1, par1, typPath, datPath);
  irc = UT3D_pt_vc__par_cv (&pt1, &vc1, typPath, datPath, 1, par1);
  if(irc < 0) goto L_NYI;
    // GR_tDyn_obj (Typ_PT, &pt1, ATT_PT_GREEN, 0);
    // GR_tDyn_vc (&vc1, &pt1, 8, 0);


  // get rot-axis and rot-angle (difference new refSys - basic-refSys)
  irc = UT3D_m3_inirot_2vc (m1, &pt1, &pl0->vz, &vc1);
  if(irc) {
    // vecs parallel|antiparallel
      // printf(" _rot_2vc - %d\n",irc);
    pl1 = *pl0;
    if(irc < 0) UT3D_pl_invert (&pl1);
    // set new origin
    UT3D_pl_ptpl (&pl1, &pt1);

  } else {
    // pl1 = transform refSys pl0
    UT3D_pl_tra_m3 (&pl1, pl0, m1);
  }
    // GR_tDyn_obj (Typ_PLN, &pl1, 8, 4);


  // transfer all points onto pl1
  UT3D_m3_loadpl (m1, &pl1);
  for(i1=0; i1<pNr; ++i1) {
    UT3D_pt_tra_pt_m3 (&pao[i1], m1, &pai[i1]);
      // GR_tDyn_obj (Typ_PT, &pao[i1], ATT_PT_YELLOW, 0);
  }


  return 0;


  L_NYI:
  TX_Error("Tess_sSym_swp_2 NYI %d",typPath);
  return -1;

}


//===========================================================================
  int Tess_sur_bnd_swp__ (TessDat__ *td1, SurSwp *swp, long suID, int mode) {
//===========================================================================
// create edges for unlimited sweep-surface. 
// Add 2D-OuterBoundary, CCW to pa2D, ia2d, pst2D, ca2D.
// Input:
//   mode      0 create tesselation-data and 2D-boundary
//             1 create only tesselation-data; do not modify other data
// Output:
//   td1       tesselation-data
//   pa2D      add 2D-points here
//   ia2d      add indices into pa2D here
//   pst2D     add point-status
//   ca2D      add contour as MSH_EDGLN_OB
//   RetCod    bNr = nr of boundaries
//
// see TSU_tr_init_ Tess_bound_SRU__ Tess_bound_SCIR Tess_bound_SURPLN


  int    irc, i1, ip1, iip, iii, typCov, typPath;
  long   ld;
  char   c1;
  void   *datCov, *datPath;
  Conus  con1;
  Torus  tor1;
  Point  pt1;
  // SurSwp *swp;

  Point  pa[4]={{0.0, 0.0, 0.0},{1.0, 0.0, 0.0},{1.0, 1.0, 0.0},{0.0, 1.0, 0.0}};
//Point  pa[4]={{0.0, 0.0, 0.0},{0.0, 1.0, 0.0},{1.0, 1.0, 0.0},{1.0, 0.0, 0.0}};
  IndTab cnt1;



  // printf("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS \n");
  // printf("Tess_sur_bnd_swp__ %ld\n",suID);
  // DEB_dump_ox_0 (ox1, "Tess_sur_bnd_swp A[%ld]",suID);
  // DEB_dump_obj__ (Typ_SURSWP, swp, "Tess_sur_bnd_swp__:");


  //----------------------------------------------------------------
  // analyze ox1; sweep-surf (contour,path) can be CYL|TOR|..

  // get contour (typCov,indCov)
  typCov = DB_GetObjDat (&datCov, &i1, swp->typCov, swp->indCov);
    printf("typCov=%d \n",typCov);
    DEB_dump_obj__ (typCov, datCov, "SWP-Cov=");


  // get path    (typPath,indPath)
  typPath = DB_GetObjDat (&datPath, &i1, swp->typPath, swp->indPath);
    DEB_dump_obj__ (typPath, datPath, "SWP-Path=");


  // save all necessary data for tessel. -> td1
  irc = Tess_sur_td__ (td1, typCov, datCov, typPath, datPath);
  if(irc < 0) return irc;


  //----------------------------------------------------------------
/*
  // test for CYLinder;
  if(typCov == Typ_CI) {

    //----------------------------------------------------------------
    // spine = LINE (cylinder)
    if((typPath == Typ_LN)     ||
       (typPath == Typ_CI)     ||
       (typPath == Typ_CVBSP))    {

      // store surface-date -> td1->td
      irc = Tess_sur_td_ci_obj (td1, datCov, typPath, datPath);
      if(irc < 0) return irc;


    //----------------------------------------------------------------
    // spine not supported
    } else {
      TX_Error("Tess_sur_bnd_swp__ Spine not (yet) supported ..");
      return -1;
    }


  //----------------------------------------------------------------
  // Contour unsupported ..
  } else {
    TX_Error("Tess_sur_bnd_swp__ Contour not (yet) supported ..");
    return -1;
  }
*/

  //----------------------------------------------------------------
  if(mode != 0) return 0;

  // get all points into pa2D ..

  // irc = Tess_add_points (&ip2, &pa2D, &eas1, 0, 4, pa);
  // Test_dump_sur (&eas1, suID);

  iip = MEMTAB_IND (&pa2D);
  ip1 = iip;
  iii = MEMTAB_IND (&ia2D);

  // add 2D-points to pa2D, links to eas1
  pt1.z = 0.;
  for(i1=0; i1<4; ++i1) {
    pt1.x = pa[i1].x * td1->rx;
    pt1.y = pa[i1].y * td1->ry;
    irc = MemTab_sav (&pa2D, &ld, &pt1, 1);
    if(irc < 0) return irc;
  }

  // add links
  for(i1=0; i1<4; ++i1) {
    // add links to imt
    irc = MemTab_add (&ia2D, &ld, &iip, 1, 0);
    if(irc < 0) return irc;
    ++iip;
  }
  // add a closing link to first point
  irc = MemTab_add (&ia2D, &ld, &ip1, 1, 0);
  if(irc < 0) return irc;

    // Test_dump_pts (&pa2D, suID);
    // Test_dump_imt (&ia2D);


  // add point-status to pst2D
  c1 = 2;
  irc = MemTab_add (&pst2D, &ld, &c1, 4, 4);
  if(irc < 0) return irc;


  // add boundary to ca2D
  cnt1.ibeg = iii;    // startindex in ia2D
  cnt1.iNr  = MEMTAB_IND (&ia2D) - iii;   // nr of points
  cnt1.typi = MSH_EDGLN_OB;
  cnt1.aux  = 0; //GL_TRIANGLES; // WHY ?
  irc = MemTab_add (&ca2D, &ld, &cnt1, 1, 0);
  if(irc < 0) return irc;


    // Testdisp contour
    // GR_Disp_ipTab (cnt1.iNr,
                   // MEMTAB__ (&ia2D, cnt1.ibeg),
                   // MEMTAB_DAT(&pa2D),
                   // SYM_STAR_S, 0);
    // printf(" typCov=%d\n",((TessDat_SWP*)td1->td)->typCov);


  return 1;

}


//=====================================================================
  int Tess_sur_td__ (TessDat__ *td1,
                     int typCov, void *datCov,
                     int typPath, void *datPath) {
//=====================================================================
// save tesselation-date for sweep with spine=cvBspl
// 2D-X=path; 2D-Y=contour (circ)


  int          i1;
  double       d1;
  Vector       vc1;
  Circ2C       ci2c;
  TessDat_SWP  *tdSWP;



  printf("Tess_sur_td__ \n");
  DEB_dump_obj__ (typCov, datCov, "  datCov:");
  DEB_dump_obj__ (typPath, datPath, "  datPath:");

    
  td1->typ     = Typ_SURSWP;

  tdSWP = (void*)td1->td; 

  // save (only pointers !) to path and contour.
  tdSWP->typCov  = typCov;
  tdSWP->datCov  = datCov;

  tdSWP->typPath = typPath;
  tdSWP->datPath = datPath;




  //----------------------------------------------------------------
  td1->tr_2D_3D = Tess_tr_2D_3D_swp;  // transl.func 3D -> 2D
  td1->tr_3D_2D = Tess_tr_3D_2D_swp;  // transl.func 2D -> 3D



  //================================================================
  // fix ry, dy from contour
  //----------------------------------------------------------------
  // get circle-baseplane and 2D-circle in centerPosition from ci1
  if(typCov == Typ_CI) {    // torus
    td1->ry = 1.;  // total range contour
    // increment contour = circle
    td1->dy = UT2D_angr_ciSec (td1->tol, fabs(((Circ*)datCov)->rad)) / RAD_360;
    // transfer circ into 2D; get basicPlane -> tdSWP->pl1
    UT2D_cic_ci3 (&tdSWP->ci2c, &tdSWP->plb, datCov);
      // GR_tDyn_obj (Typ_PLN, &tdSWP->pl1, 8, 4);


  //----------------------------------------------------------------
  } else if(typCov == Typ_CVPOL) {
    td1->ry = 1.;  // total range contour
    // increment individually
    td1->dy = -1.;
    // get basic-plane from polygon
    UT3D_pl_obj (&tdSWP->plb, typCov, datCov, 1);
      // DEB_dump_obj__ (Typ_VC, &tdSWP->plb, "  plb");


  //----------------------------------------------------------------
  } else if(typCov == Typ_CVELL) {
    td1->ry = 1.;  // total range contour
    // increment contour = circle
    // get lenght of minor axis
    d1 = UT3D_len_vc (&((CurvElli*)datCov)->vb);
    td1->dy = UT2D_angr_ciSec (td1->tol, d1) / RAD_360;
    // transfer circ into 2D; get basicPlane -> tdSWP->pl1
    UT3D_pl_obj (&tdSWP->plb, typCov, datCov, 1);
      // GR_tDyn_obj (Typ_PLN, &tdSWP->pl1, 8, 4);


  //----------------------------------------------------------------
  // } else if(typCov == Typ_CVBSP) {


  } else {
    TX_Error ("Tess_sur_td__ contour %d not supported",typCov);
    return -1;
  }


  //----------------------------------------------------------------
  // vz of pl1 must go into direction of path; if not: invert !
  // get vc1 = startvector of path
  UT3D_pt_vc__par_cv (NULL, &vc1, typPath, datPath, 1, 0.);
    // DEB_dump_obj__ (Typ_VC, &vc1, "  vc1");
  // vc1=startvec of path; z-vec of plane plb
  i1 = UT3D_sid_2vc (&vc1, &tdSWP->plb.vz);
    // printf(" sid=%d\n",i1);
  if(!i1) {
        TX_Error ("Tess_sur_td__ contour - path perpendicular");
    return -1;
  }
  if(i1 < 0 ) {
    UT3D_pl_invert (&tdSWP->plb);
  }

  tdSWP->pl1 = tdSWP->plb;
    // GR_tDyn_obj (Typ_PLN, &tdSWP->pl1, 9, 4);




  //================================================================
  // fix rx, dx from path
  //----------------------------------------------------------------
  if(typPath == Typ_LN) {  // cylinder
    td1->rx = 1.;
    td1->dx = 1.;


  //----------------------------------------------------------------
  } else if(typPath == Typ_CI) {    // torus
    td1->rx = 1.;

    td1->dx = UT2D_angr_ciSec (td1->tol, fabs(((Circ*)datPath)->rad)) /
                                         fabs(((Circ*)datPath)->ango);


  //----------------------------------------------------------------
  } else if(typPath == Typ_CVBSP) {    // pipe
    td1->rx = 1.;
    //td1->rx = cbsp->v1 - cbsp->v0;

    UT3D_du_bsplTol (&td1->dx, datPath, td1->tol);



  //----------------------------------------------------------------
  } else {
    TX_Error ("Tess_sur_td_ci_obj path %d not supported",typPath);
    return -1;
  }

    // TEST_ONLY:
    // td1->dx = 0.3; // td1->dy = 0.16;

  
  //----------------------------------------------------------------

    // printf("TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT \n");
    printf("ex-Tess_sur_td__\n");
    printf(" rx=%lf ry=%lf\n",td1->rx,td1->ry);
    printf(" dx=%lf dy=%lf\n",td1->dx,td1->dy);


  return 0;

}


//=====================================================================
  int Tess_sur_td_ci_obj (TessDat__ *td1, Circ *ciCov,
                         int typPath, void *datPath) {
//=====================================================================
// save tesselation-date for sweep with spine=cvBspl
// Contour (Cov) must be Circle;
// 2D-X=path; 2D-Y=contour (circ)


  Circ2C       ci2c;
  TessDat_SWP  *tdSWP;



  // DEB_dump_obj__ (Typ_CI, ciCov, "Tess_sur_td_ci_obj ciCov:");
  // DEB_dump_obj__ (typPath, datPath, "  datPath:");


  td1->typ     = Typ_SURSWP;

  tdSWP = (void*)td1->td;

  // save (only pointers !) to path and contour.
  tdSWP->typCov  = Typ_CI;
  tdSWP->datCov  = ciCov;

  tdSWP->typPath = typPath;
  tdSWP->datPath = datPath;


  // get circle-baseplane and 2D-circle in centerPosition from ci1
  UT2D_cic_ci3 (&tdSWP->ci2c, &tdSWP->pl1, ciCov);
    // GR_tDyn_obj (Typ_PLN, &tdSWP->pl1, 8, 4);

  td1->ry = 1.;  // total range contour

  // increment contour = circle
  td1->dy = UT2D_angr_ciSec (td1->tol, fabs(ciCov->rad)) / RAD_360;



  //----------------------------------------------------------------
  td1->tr_2D_3D = Tess_tr_2D_3D_swp;  // transl.func 3D -> 2D
  td1->tr_3D_2D = Tess_tr_3D_2D_swp;  // transl.func 2D -> 3D


  //----------------------------------------------------------------
  if(typPath == Typ_LN) {  // cylinder
    td1->rx = 1.;
    td1->dx = 1.;


  //----------------------------------------------------------------
  } else if(typPath == Typ_CI) {    // torus
    td1->rx = 1.;

    td1->dx = UT2D_angr_ciSec (td1->tol, fabs(((Circ*)datPath)->rad)) / 
                                         fabs(((Circ*)datPath)->ango);


  //----------------------------------------------------------------
  } else if(typPath == Typ_CVBSP) {    // pipe
    td1->rx = 1.;
    //td1->rx = cbsp->v1 - cbsp->v0;

    UT3D_du_bsplTol (&td1->dx, datPath, td1->tol);



  //----------------------------------------------------------------
  } else {
    TX_Error ("Tess_sur_td_ci_obj %d not supported",typPath);
    return -1;
  }

    // TEST_ONLY:
    // td1->dx = 0.3; // td1->dy = 0.16;

    // printf("TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT \n");
    // printf(" rx=%lf ry=%lf\n",td1->rx,td1->ry);
    // printf(" dx=%lf dy=%lf\n",td1->dx,td1->dy);


  return 0;

}


//================================================================
  int Tess_tr_2D_3D_swp (Point *pt2, Point *pt3) {
//================================================================
// get 2D-point from 3D-point on cylinder
// see TSU_tr_2D_3D_con

  DEB_dump_obj__ (Typ_PT, pt3, "Tess_tr_2D_3D_con: ");

  return 0;

}


//==================================================================
  int Tess_tr_3D_2D_swp (Point *pt3, Point *pt2, TessDat_SWP *swp) {
//==================================================================
// get 3D-point from 2D-point on sweep-surface
// see TSU_tr_3D_2D_con

  // DEB_dump_obj__ (Typ_PT, pt2, "Tess_tr_3D_2D_swp: ");
  // DEB_dump_obj__ (Typ_CON, &con->con, " icon.con: ");

  UT3D_pt_par_swp (pt3, pt2->x, pt2->y, swp);

    // DEB_dump_obj__ (Typ_PT, pt3, "  pt3: ");
    // APT_disp_SymB (SYM_TRI_S, 2, pt3);

  return 0;

}


//================================================================
  int UT3D_pt_par_swp (Point *pt3, double parU, double parV, TessDat_SWP *swp) {
//================================================================
// translate 2D-pt from parU,parV back to 3D.
// parU=2D-X=parameter along path;
// parV=2D-Y= parameter along contour.
// see also UT3D_pt_evparscon UT3D_pt_evparstor

  double  d1;
  Point2  pt21;
  Point   pt1;
  Vector  vc1;
  Circ2C  ci2c;
  Plane   pl1;
  char    s1[80];


  // printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX \n");
  // printf("UT3D_pt_par_swp %lf %lf\n",parU,parV);
  // DEB_dump_obj__ (Typ_PLN, &swp->pl1, "  pl1:");
  // printf(" typPath=%d typCov=%d\n",swp->typPath, swp->typCov);

  // if(parU < 0.3) return 0; // TEST ONLY
  // if(parU > 0.4) return 0; // TEST ONLY


//....................
// UTO_ptvc_obj_par1 (&pt1, &vc1, swp->typPath, swp->datPath, parU);

  // find pt1=position and its vc1=tangent-vector along path from parU
  UT3D_pt_vc__par_cv (&pt1, &vc1, swp->typPath, swp->datPath, 1, parU);
    // GR_tDyn_obj (Typ_PT, &pt1, ATT_PT_GREEN, 0);
    // sprintf(s1,"%lf",parU);GR_tDyn_txtA (&pt1, s1, 0);
    // GR_tDyn_obj (Typ_SymB, &pt1, ATT_COL_RED, SYM_STAR_S);
    // GR_tDyn_vc (&vc1, &pt1, 9, 0);


  // create activePlane 
  // - from last plane, new vz, create vy from old vx an d new vz, create new vx.
  UT3D_pl_pto_vcz_vcx (&pl1, &pt1, &vc1, &swp->pl1.vx);
  swp->pl1 = pl1;
    // GR_tDyn_obj (Typ_PLN, &pl1, 8, 4);
    // DEB_dump_obj__ (Typ_PLN, &swp->pl1, "  pl1:");
    // DEB_dump_obj__ (Typ_PLN, &pl1, "  pl1:");



  //----------------------------------------------------------------
  // get pt21 = position of 2D-contourpoint from its parameter parV

  //----------------------------------------------------------------
  // contour = polygon:
  if(swp->typCov == Typ_CVPOL) {
    // d1 = real-lenght from 0-1-parameter parV
    d1 = UT3D_par_par1plg (parV, swp->datCov);
      // printf(" f1 = %lf\n",d1);
    // get 3D-point from knotVal (real-lenght)
    UT3D_pt_evalplg (&pt1, swp->datCov, d1);
      // GR_tDyn_obj (Typ_PT, &pt1, ATT_PT_GREEN, 0);
      // DEB_dump_obj__ (Typ_PT, &pt1, "  pt1:");
    // get dx,dy on basic-plan swp->plb
    UT3D_vc_2pt (&vc1, &swp->plb.po, &pt1);
    UT3D_2par_vc_vcx_vcy (&pt21.x, &pt21.y, &vc1, &swp->plb.vx, &swp->plb.vy);


  //----------------------------------------------------------------
  // contour = ellipse
  } else if(swp->typCov == Typ_CVELL) {
    UT3D_ptvc_eval_ell_par (&pt1, NULL, swp->datCov, parV);
    // get dx,dy on basic-plan swp->plb
    UT3D_vc_2pt (&vc1, &swp->plb.po, &pt1);
    UT3D_2par_vc_vcx_vcy (&pt21.x, &pt21.y, &vc1, &swp->plb.vx, &swp->plb.vy);


  //----------------------------------------------------------------
  // contour = circle:
  } else if(swp->typCov == Typ_CI) { 
    // get 2D-point pt21 from parV, ango, radius.
    UT2D_pt_cic_par (&pt21, &parV, &swp->ci2c);


  //----------------------------------------------------------------
  } else goto L_NYI;


  // transfer 2D-point into pl1
  UT3D_pt_tra_pt_2vc_2par (pt3, &pl1.po, &pl1.vx, pt21.x, &pl1.vy, pt21.y);



      // printf("ex UT3D_pt_par_swp %lf,%lf %lf\n",pt3->x,pt3->y,pt3->z);
      // GR_tDyn_obj (Typ_PT, pt3, ATT_PT_HILI, 0);





  return 0;


  L_NYI:
  TX_Error("UT3D_pt_par_swp NYI %d %d",swp->typPath,swp->typCov);
  return -1;

}


//================================================================
  int Tess_sur_grd__ (GridBox *gbh1, TessDat__ *td1) {
//================================================================
// compute all gridbox-points; add these points to pa2D
// Input:
//   td1       get rx, ry (total range of 2D-space X and Y)
//             and dx, dy (minimum increment; same unit as total-range)
// Output:
//   pgb       pointer to (static) gridbox or NULL (for planar)
//
// Example rx,dx:  rx=RAD_360; dx=minAng from UT2D_angr_ciSec

// see TSU_tr_init_


  // static GridBox gb;
  static Point    ptll;

  int         iip, pNr, i1, typCov;
  int         ix, iy;
  double      rx, ry, dx, dy, *da;
  void        *datCov;
  Point2      p21, p22;
  Point       *pa;
  TessDat_SWP *tdSwp;


  // printf("GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG \n");
  // printf("Tess_sur_grd__\n");


  //----------------------------------------------------------------

  pNr = MEMTAB_IND (&pa2D);
  pa = MEMTAB_DAT (&pa2D);
     // for(i1=0;i1<pNr;++i1) DEB_dump_obj__(Typ_PT, &pa[i1]," p2[%d]",i1);

  // find p21-p22 = boundingbox of all pa2D-points (boundaries)
  UT2D_rect_pta3 (&p21, &p22, pa, pNr);
    // DEB_dump_obj__ (Typ_PT2, &p21, " p21");
    // DEB_dump_obj__ (Typ_PT2, &p22, " p22");


  rx = td1->rx;
  ry = td1->ry;
  dx = td1->dx;
  dy = td1->dy;
    // printf(" rx=%lf dx=%lf ry=%lf dy=%lf\n",rx,dx,ry,dy);


  // compute ix,iy from rX,dX
  // ix = rX / dX;
  ix = rx / dx;
  iy = ry / dy;
    // printf(" v1 - ix=%d iy=%d\n",ix,iy);
  dx = rx / ix;
  dy = ry / iy;
  ++ix;
  ++iy;

    // printf(" v2 - ix=%d iy=%d\n",ix,iy);



  //----------------------------------------------------------------
  // fill gridbox
  // get lower-leftpoint of boundingbox = startpoint of gridbox;
  ptll = UT3D_pt_pt2 (&p21);
  gbh1->pMin = ptll;

  gbh1->dx   =   dx;
  gbh1->dy   =   dy;
  gbh1->dz   =   0.0;

  gbh1->ix   =  ix;
  gbh1->iy   =  iy;
  gbh1->iz   =  1;


  // get startposition of points to be added by UT3D_grd_pta
  iip = MEMTAB_IND (&pa2D);
    // printf(" iip=%d\n",iip);

  //----------------------------------------------------------------
// ONLY FOR SURSWP:
  tdSwp = ((TessDat_SWP*)td1->td);

  //----------------------------------------------------------------
  // add gridbox for contour=polygon
  typCov = tdSwp->typCov;
    // printf(" typCov=%d\n",typCov);


  //----------------------------------------------------------------
  if(typCov == Typ_CVPOL) {
    datCov = tdSwp->datCov;
    gbh1->iy = ((CurvPoly*)datCov)->ptNr;
    // get normalized values for points
    da = (double*)MEM_alloc_tmp ((int)(sizeof(double) * iy));
    UT3D_plg_npar1_npar (da, datCov);
    // add gridbox-points with y-valTab da
    pNr = UT3D_grd_ptya (&pa2D, gbh1, da);
    if(pNr < 1) return -1;


  //----------------------------------------------------------------
  // add gridbox-points to pa2D for contour=circ
  } else if((typCov == Typ_CI)      ||
            (typCov == Typ_CVELL))      {
    // add points to pa2D
    pNr = UT3D_grd_pta (&pa2D, gbh1);
    if(pNr < 1) return -1;
      // printf(" grd-pNr=%d\n",pNr);


  //----------------------------------------------------------------
  } else {
    TX_Error ("Tess_sur_grd__ E001 %d",typCov);
    return -1;
  }


  //----------------------------------------------------------------
  // save lower left point to points in gb.pMin
  gbh1->pMin = *((Point*)MEMTAB__ (&pa2D, iip));

  return 0;

}


//======================================================================
  int Tess_sur_bnd__ (TessDat__ *td1, int sTyp, void *sDat, long suID) {
//======================================================================
// create edges for surface
// set global pa2D, ia2d, pst2D, ca2D
// add all points -> pa2D
// add status for all points -> pst2D
// add links to points -> ia2d
// add contour as MSH_EDGLN_OB -> ca2D
// add all data necessary for 2D -> 3D -> 2D into td1
//   RetCod    bNr = nr of boundaries
// see Tess_sol__

  int    bNr;

  // printf("Tess_sur_bnd__ \n");


  td1->A_id = suID;


  switch (sTyp) {

    case Typ_SURSWP:
      bNr = Tess_sur_bnd_swp__ (td1, sDat, suID, 0);
      break;

    default:
      TX_Error("Tess_sur_bnd__ NYI typ=%d",sTyp);
      return -1;
  }

    // printf("ex Tess_sur_bnd__:\n");
    // printf(" tot.points=%d\n",MEMTAB_IND(&pa2D));
    // printf(" tot.edges=%d\n",MEMTAB_IND(&eas1));
    // Test_dump_sur (&eas1, sba[is].suID);


  return bNr;

}


//================================================================
  int Tess_bMsh_iMsh (MemTab(IndTab) *fmt,
                      MemTab(int)    *imt,
                      MemTab(Point)  *pmt) {
//================================================================
// change iMsh -> bMsh; add bMsh to GLT_sur (TSU_store)
// omit vectors, colors.
// bMsh - bSur - bFac
//   bMsh:    (ObjGX)Typ_GL_Sur with n (ObjGX) bSur's
//     bSur:  (ObjGX)Typ_GL_PP  with n (Typ_PT) points


  int     i1, i2, ii, sNr, pNr, *ia0, *ia1, gTyp;
  ObjGX   bMsh, *bSur, *bFac;
  Point   *p1a, *p2a;
  IndTab  *itAct;

  printf("Tess_bMsh_iMsh \n");


  // get space for bSur's
  sNr = MEMTAB_IND (fmt);   // nr of bSur
  bSur = (void*) MEM_alloc_tmp ((int)(sizeof(ObjGX) * sNr));
  bFac = (void*) MEM_alloc_tmp ((int)(sizeof(ObjGX) * sNr));


  // Create mesh (topmost obj)
  OGX_SET_OBJ (&bMsh, Typ_GL_Sur, Typ_ObjGX, sNr, bSur);

  // loop tru surfs
  p1a = MEMTAB__ (pmt, 0);
  ia0 = MEMTAB__ (imt, 0);
  for(i1=0; i1<sNr; ++i1) {
    itAct = MEMTAB__ (fmt, i1);
      DEB_dump_obj__ (Typ_IndTab, itAct, "it[%d]",i1);
    // get space for points
    pNr = itAct->iNr;
    p2a = (void*) MEM_alloc_tmp ((int)(sizeof(Point) * pNr));
    // copy points -> pa
    ia1 = &ia0[itAct->ibeg];
    for(i2=0; i2<pNr; ++i2) {
      ii = ia1[i2];
      memcpy (&p2a[i2], &p1a[ii], sizeof(Point));
    }
    // create bSur-record
    OGX_SET_OBJ (&bSur[i1], Typ_GL_PP, Typ_ObjGX, 1, &bFac[i1]);
    gTyp = BIT_GET (itAct->aux, 15);   // remove GL_FAC_PLANAR
    OGX_SET_OBJ_aux (&bFac[i1], Typ_PT, Typ_PT, pNr, p2a, gTyp);
  }

    // dump bMsh
    // DEB_dump_ox_s_ (&bMsh, "sur-tess:");
    // DEB_dump_ox_0 (&bMsh, "sur-tess:");


/*
  //----------------------------------------------------------------
  // TEST: display bMsh
  { long    dli;
    dli = DL_StoreObj (Typ_SUR, -1L, Typ_Att_dash_long);
    GL_DrawSur (&dli, Typ_Att_dash_long, &bMsh);
  }

  //----------------------------------------------------------------
  // TEST: add bMsh to GLT_sur
  { ObjGX  *vTab;
    TSU_Init (1, &vTab);  // start 
    TSU_store (&bMsh);    // save bMsh -> GLT_sur
    TSU_Init (0, &vTab);  // reset, get surf's
  }
*/

  TSU_store (&bMsh);    // save bMsh -> GLT_sur


  return 0;

}


//================================================================
  int Tess_ptNr_SURSWP (SurSwp *swp, double tol) {
//================================================================
// get nr of points for sweep-surf
  
  int    pNr, typCov, typPath, iu, iv, rNr;
  void   *datCov, *datPath;


  // DEB_dump_obj__ (Typ_SURSWP, swp, "Tess_ptNr_SURSWP:");
  

  // get contour (typCov,indCov)
  typCov = DB_GetObjDat (&datCov, &rNr, swp->typCov, swp->indCov);
    // DEB_dump_obj__ (typCov, datCov, "SWP-Cov=");

  
  // get path    (typPath,indPath)
  typPath = DB_GetObjDat (&datPath, &rNr, swp->typPath, swp->indPath);
    // DEB_dump_obj__ (typPath, datPath, "SWP-Path=");


  iu = UT3D_ptNr_obj (typPath, datPath, tol);

  iv = UT3D_ptNr_obj (typCov, datCov, tol);

  pNr = iu * iv;

    // printf("ex Tess_ptNr_SURSWP pNr=%d iu=%d iv=%d\n",pNr,iu,iv);


  return pNr;

}


// EOF
