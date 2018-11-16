// ut_rbspl.c       Thomas Backmeister      18.9.2004
/*
 *
 * Copyright (C) 2015 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
 * Copyright (C) 2007 Thomas Backmeister (t.backmeister@gmx.at)
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
UT3D_par_par1_rbsp       get knotValue from parameter 0-1
UT3D_par_rbsp_pt         get knotvalue from point on rational-b-spline curve

intersect RBSP mit allen anderen ....
UTO_stru_int
UTO_stru_inv

project Point --> RBSP
 Thomas denkt nach; ev iterativ ...
    zuerst testen vieviele Minima es gibt; rund um diese Stellen iterieren ..
    Minimum am Polygon testen ...

RBSP Kopieren
UTO_isol__
UTO_copy_obj
..

RBSP transformieren ....
UTRA_app_obj
UTO_obj_apptra
UTO_ox_tra
UTO_obj_tra_m3

-----------------------------------------------------
Modifications:
2010-04-02 new UT3D_pt_projptrbspl RF

-----------------------------------------------------
*/
/*!
\file  ../ut/ut_rbspl.c
\brief rational b-spline curve functions
\code
=====================================================
List_functions_start:

UT3D_pt_evparCrvRBSpl    point <-- rational b-spline at parameter
UT3D_pt_projptrbspl      project point onto rational-b-spline curve
UT3D_vc_evparCrvRBSpl    tangent vector <-- rational b-spline at parameter
UT3D_ptNr_rbsp           estimate nr of polygonPoints for rat-B-spline
UT3D_cv_rbsp             make Polygon from Rat.B-Spline-Kurve + tol
UT3D_bspl_deriv1         1. derivation b-spline curve <-- b-spline curve
UT3D_rbspl_deriv1        1. deriv. rat. b-spline curve <-- rat. b-spline curve
UT3D_rbsp_rbez           rational-Bspline-curve from rational-bezier-curve

UT3D_rbspl_ck_closed     check if B-SplCv is closed
UT3D_par_rbsp_pt         get knotvalue from point on rational-b-spline curve
UT3D_par_par1_rbsp       get knotValue from parameter 0-1
UT3D_par1_par_rbsp       get parameter 0-1 from knotvalue of rational-b-spline
UT3D_obj_cnvt_rbspl      convert Rat.B-Spl --> CI/LN; see UT3D_bsp_degrad
UT3D_ci_rbspl3           create circle from Rat.B-SplineCurve 3 ControlPoints

UT3D_pt_int_ln_rbsp      intersect line X rat-b-spline-curv
UT3D_pt_int_bsp_rbsp     intersect b-spline-curv X rat-b-spline-curv

------------------------ testfunctions:
UT3D_rbspl_tst_1         testprogs
UT3D_rbspl_tst_load      load a teststruct
UT3D_rbspl_tst_cc        display ControlPoints/Curve
UT3D_rbspl_tst_tg        disp points und tangents

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_geo.h"
#include "../ut/ut_ox_base.h"             // OGX_SET_INDEX
#include "../ut/ut_math.h"
#include "../ut/ut_TX.h"
#include "../ut/ut_bspl.h"
#include "../ut/ut_obj.h"                 // UTO_stru_2_obj

#include "../ut/func_types.h"                  // SYM_..

#include "../xa/xa_mem.h"



//===============================================================================
  int UT3D_par_rbsp_pt (double *kv, double *dist, CurvRBSpl *cvi, Point *pt) {
//===============================================================================
// UT3D_par_rbsp_pt         get knotvalue from point on rational-b-spline curve
// see also UT3D_parCv_bsplpt


#define  TABSIZ1 16
#define  TABSIZ2 1000000
  int    irc, nip;
  double va[TABSIZ1];
  char   *memSpc;
  Point  pa[TABSIZ1];
  Memspc wrkSpc;


  UT3D_stru_dump (Typ_PT, pt, "UT3D_par_rbsp_pt:");


  memSpc = MEM_alloc_tmp (TABSIZ2);
  UME_init (&wrkSpc, memSpc, TABSIZ2);
  irc = UT3D_pt_projptrbspl (&nip, pa, va, &wrkSpc, UT_TOL_cv, cvi, pt);

  if(irc > 0) nip = 1; /// ???
  if(irc < 0){TX_Error("UTO_stru_int E004"); return irc;}

  *kv = va[0];

    printf("ex par_rbsp_pt n=%d kv=%f\n",nip,va[0]);


  return 0;

}


//================================================================
  double UT3D_par1_par_rbsp (double *kv, CurvRBSpl* cv1) {
//================================================================
// UT3D_par1_par_rbsp       get parameter 0-1 from knotvalue of rational-b-spline
// see also UT3D_par1_parbsp


  double pv, u1, u2, uTot, uMin, uMax;


  printf("UT3D_par1_par_rbsp %f\n", *kv);

  u1 = cv1->v0;
  u2 = cv1->v1;

  if(cv1->dir) MEM_swap_2db (&u1, &u2);


  uMin = cv1->kvTab[0];
  uMax = cv1->kvTab[cv1->ptNr + cv1->deg];
    printf("    _par1_parbsp u1=%f u2=%f uMin=%f uMax=%f\n",u1,u2,uMin,uMax);

  if(u2 < u1) {
    // umax - u1 + u2 - umin
    uTot = uMax - u1 + u2 - uMin;
    if(*kv > u1) pv = (*kv - u1) / uTot;
    else         pv = (uMax - u1 + *kv - uMin) / uTot;

  } else {
    uTot = u2 - u1;
    pv = (*kv - u1) / uTot;
  }

  if(cv1->dir) pv = 1. - pv;   // removed 2017-02-15

    printf("ex _par1_par_rbsp pv=%f kv=%f uTot=%f\n",pv,*kv,uTot);

  return (pv);

}


//====================================================================
 int UT3D_rbsp_rbez (CurvRBSpl *cvo, CurvRBez *cvi, Memspc *memSeg1) {
//====================================================================
/// \code
/// UT3D_rbsp_rbez         rational-Bspline-curve from rational-bezier-curve
///                        Backmeister, Reiter    2014-09-11
/// controlpoints, weights from cvi; knots in memSeg1.
/// \endcode

  int     deg, i1;

  deg = cvi->ptNr - 1;

  cvo->deg = deg;
  cvo->ptNr = cvi->ptNr;
  cvo->v0 = cvi->va;
  cvo->v1 = cvi->vb;
  cvo->dir = 0;

/*
  i1 = sizeof(Point) * cvi->ptNr;
  cvo->cpTab = UME_reserve (memSeg1, i1);
  i1 = sizeof(double) * cvi->ptNr;
  cvo->wTab = UME_reserve (memSeg1, i1);
  for(i1=0; i1<cvi->ptNr; ++i1) {
    cvo->cpTab[i1] = cvi->cptab[i1];
    cvo->wTab[i1] = cvi->wtab[i1];
  }
*/
  cvo->cpTab = cvi->cptab;
  cvo->wTab = cvi->wtab;


  i1 = sizeof(double) * 2 * (deg + 1);
  cvo->kvTab = UME_reserve (memSeg1, i1);
  if(!cvo->kvTab) {TX_Error("UT3D_rbsp_rbez EOM"); return -1;}

  // knotVec: deg+1 mal va, then deg+1 mal vb
  for(i1=0; i1<deg+1; ++i1) {
    cvo->kvTab[i1] = 0.; // cvi->va;
  }
  for(i1=0; i1<deg+1; ++i1) {
    cvo->kvTab[i1+deg+1] = 1.;  // cvi->vb;
  }

  cvo->v0 = cvi->va;
  cvo->v1 = cvi->vb;

    // UT3D_stru_dump (Typ_CVRBSP, cvo, "ex UT3D_rbsp_rbez");

  return 0;

}


//================================================================
  int UT3D_pt_int_ln_rbsp (int *nxp, Point *xptab, Memspc *memSeg1,
                           Line *ln1, CurvRBSpl *rbsp1) {
//================================================================
// UT3D_pt_int_ln_rbsp      intersect line X rat-b-spline-curv
// see UT3D_pt_int_bsp_rbsp


  int       i1, i2, irc, xptSiz, ptxNr, siztab2;
  CurvRBez  *bcvtab2;
  Point     xp[UT_BEZDEG_MAX];
  void      *memPos0, *memPos1;
  // Memspc    tmpSpc=UME_NEW, *memSeg1;


  // printf("UT3D_pt_int_bsp_rbsp *nxp=%d\n",*nxp);

  memPos0 = memSeg1->next;
  // get tempspace (gets lost on func-return)
  // UME_alloc_tmp (&tmpSpc, 250000);
  // memSeg1 = &tmpSpc;


  // rat-bezier-curves from rat-b-spline curve
  irc = UT3D_rbez_rbsp (&siztab2, &bcvtab2, memSeg1, rbsp1);
  if(irc < 0) goto Fertig;
    for(i1=0; i1<siztab2; ++i1)
       // UT3D_stru_dump (Typ_CVRBEZ, &bcvtab2[i1], " rbez[%d]",i1);

  // init
  xptSiz  = *nxp;
  *nxp    = 0;

  memPos1 = memSeg1->next;   // merken; ab hier nur tempData

  // intersection of curves
  for (i1=0; i1<siztab2; ++i1) {

    ptxNr = 0;
    irc = UT3D_pt_int_ln_rbez (&ptxNr, xp, UT_BEZDEG_MAX,
                               ln1, &bcvtab2[i1],
                               memSeg1, UT_TOL_cv);
    if(irc < 0) goto Fertig;
      // printf(" ptxNr=%d xptSiz=%d\n",ptxNr,xptSiz);

    // copy fount points out
    if(*nxp + ptxNr >= xptSiz) {
       TX_Error("UT3D_pt_int_bsp_rbsp E002");
       irc = -2;
       goto Fertig;
    }
    for(i2=0; i2<ptxNr; ++i2) {
      xptab[*nxp] = xp[i2];
      ++(*nxp);
    }

    // reset memSeg1 (UT3D_pt_intlnbcv braucht nur temp. Objekte)
    // UME_dump(memSeg1, " space used: ");
    memSeg1->next = memPos1;
  }

  Fertig:
    // reset memSeg1
    memSeg1->next = memPos0;

      // for(i1=0; i1 < *nxp; ++i1)
      // UT3D_stru_dump (Typ_PT, &xptab[i1], " xptab[%d]",i1);

    return irc;


}


//==================================================================
  int UT3D_pt_int_bsp_rbsp (int *nxp, Point *xptab, Memspc *memSeg1,
                            CurvBSpl *bspl1, CurvRBSpl *bspl2) {
//==================================================================
// UT3D_pt_int_bsp_rbsp     intersect b-spline-curv X rat-b-spline-curv
// - change CurvBSpl -> bezier-curve (UT3D_bez_bspl__)
// - change CurvRBSpl -> rational-bezier-curve (UT3D_rbez_rbsp)
// - intersect bezier-curve X rational-bezier-curve (UT3D_pt_intbezrbez)
// 
// IN:
//   int *nxp         ... max Nr of outputPoints
//   CurvBSpl *bspl1  ... b-spline curve 1
//   CurvRBSpl *bspl2 ... rat-b-spline curve 2
//   Memspc *memSeg1  ... space for temp. data; bis zu 100 KB erforderlich !!
// OUT:
//   int *nxp         ... number of intersection points
//   Point *xptab     ... intersection points
// Returncode:
//   0 = OK
//  -1 = out of mem (Memspc too small)
//  -2 = out of mem (xptab too small)


  int       nbcv1, nbcv2, i1, i2, i3, irc, ptxNr, xptSiz, siztab1, siztab2;
  ObjGX     bezTab1, bezTab2;
  CurvBez   *bcvtab1;
  CurvRBez  *bcvtab2;
  Point     xp[UT_BEZDEG_MAX];
  void      *memPos0, *memPos1;


  // printf("UT3D_pt_int_bsp_rbsp *nxp=%d\n",*nxp);

  memPos0 = memSeg1->next;

  // Bezier curves from each b-spline curve
  irc = UT3D_bez_bspl__ (&bezTab1, memSeg1, bspl1);
  if(irc < 0) goto Fertig;
    // UTO_dump__ (&bezTab1, "bezTab < bspl-cv");

  // rat-bezier-curves from rat-b-spline curve
  irc = UT3D_rbez_rbsp (&siztab2, &bcvtab2, memSeg1, bspl2);
  if(irc < 0) goto Fertig;
    for(i1=0; i1<siztab2; ++i1)
       // UT3D_stru_dump (Typ_CVRBEZ, &bcvtab2[i1], " rbez[%d]",i1);

  // init
  xptSiz  = *nxp;
  *nxp    = 0;

  siztab1 = bezTab1.siz;
  bcvtab1 = bezTab1.data;

  memPos1 = memSeg1->next;   // merken; ab hier nur tempData


  // intersection of curves
  for (i1=0; i1<siztab1; ++i1) {

    for (i2=0; i2<siztab2; ++i2) {
      // printf("  _pt_int2bspl %d %d\n",i1,i2);

      ptxNr = 0;
      irc = UT3D_pt_intbezrbez (&ptxNr, xp, UT_BEZDEG_MAX,
                               &bcvtab1[i1], &bcvtab2[i2],
                               memSeg1, UT_TOL_ln);
      if(irc < 0) goto Fertig;
        // printf(" ptxNr=%d xptSiz=%d\n",ptxNr,xptSiz);

      // copy fount points out
      if(*nxp + ptxNr >= xptSiz) {
         TX_Error("UT3D_pt_int_bsp_rbsp E002");
         irc = -2;
         goto Fertig;
      }
      for(i3=0; i3<ptxNr; ++i3) {
        xptab[*nxp] = xp[i3];
        ++(*nxp);
      }

      // reset memSeg1 (UT3D_pt_intlnbcv braucht nur temp. Objekte)
      // UME_dump(memSeg1, " space used: ");
      memSeg1->next = memPos1;
    }
  }

  Fertig:
    // reset memSeg1
    memSeg1->next = memPos0;


      // for(i1=0; i1 < *nxp; ++i1)
      // UT3D_stru_dump (Typ_PT, &xptab[i1], " xptab[%d]",i1);


    return irc;


}


//================================================================
  int UT3D_pt_projptrbspl (int *nxp, Point *ptOut, double *vaOut,
                           Memspc *memSeg1, double tol, 
                           CurvRBSpl *cv, Point *pt) {
//================================================================
/// \code
/// UT3D_pt_projptrbspl   project point onto rational-b-spline curve
/// 
/// UT3D_pt_projptrbspl   Author: Franz Reiter            2.4.2010
/// 
/// 
/// IN:
///   int *nxp         ... size of ptOut and vaOut
///   CurvRBSpl *cv    ... rational-b-spline curve
///   Point *pt        ... point
///   Memspc *memSeg1  ... Memory fuer temp. Objects;
///   double tol           pointtolerance
///
/// OUT:
///   int *nxp         ... number of projection points
///   Point *ptOut     ... array of projection points
///   double *vaOut    ... array of corresponding parameter values of
///                        projection points on b-spline curve;
///                        none if vaOut=NULL
/// Returncode:
///   0 = OK
///  -1 = out of mem (Memspc too small)
///  -2 = out of mem (ptab too small)
///
/// \endcode

// Keine analytische Lösung möglich; nur approximieren.
// see also UT3D_pt_projptbspl

  void     *memPos0, *memPos1;

  int      i1, i2, ii, pnr, paSiz, outSiz;
  double   d1, d2, dd, *vTab;
  Point    pt1, *pTab;


  // printf("UT3D_pt_projptrbspl nxp=%d tol=%f\n",*nxp,tol);
  // UT3D_stru_dump (Typ_PT, pt, "");
  // UT3D_stru_dump (Typ_CVRBSP, cv, "");

  memPos0 = memSeg1->next;     // save memSeg1

  outSiz = *nxp;
  *nxp   = 0;


  // space in memSeg1 / 4   (point(3 doubles) + 1 double(parameter))
  paSiz = UME_ck_free (memSeg1) / (sizeof(Point) + sizeof(double));
  pTab = UME_reserve (memSeg1, paSiz * sizeof(int));
  vTab = UME_get_next (memSeg1);


  // get the polygon. Die richtige Loesung (wenn mehrere Loesungen moeglich)
  // kann man nur auswaehlen, wenn man ein Polygon mit brauchbarer Tolerenz
  // hat. Davon die n-te Loesung auswaehlen; ab dann wieder via 3 Punkte ..
  // Problem: man braucht zu jedem Polygonpunkt den Parameterwert !
  UT3D_cv_rbsp (&pnr, pTab, vTab, cv, paSiz, tol);


  // TEST:
  // for(i1=0; i1<pnr; ++i1) {
  // printf("p[%d]=%f,%f,%f p=%f\n",i1,pTab[i1].x,pTab[i1].y,pTab[i1].z,vTab[i1]);
  // GR_Disp_pt (&pTab[i1], SYM_STAR_S, 2);
  // }



  // find nearest point on the polygon
  d2 = UT_VAL_MAX;
  for(i1=0; i1<pnr; ++i1) {
    d1 = UT3D_lenq_PtPt (&pTab[i1], pt);
    if(d1 < d2) {
      d2 = d1;
      ii = i1;
    }
  }

  // test dst form this pt (ii)
  d1 = UT3D_len_2pt (&pTab[ii], pt);
    // printf(" minPos=%d minDist=%f\n",ii,d1);
  if(d1 < tol) {
    ptOut[*nxp] = pTab[ii];
    if(vaOut) vaOut[*nxp] = vTab[ii];
    ++(*nxp);
    goto L_done;
  }


  // which segment ? before or after point ii ?
  // test which parameter has ii on the segments
  // first segment:
  i1 = ii - 1;
  if(i1 >= 0) UT3D_parpt_3pt (&d1, pt, &pTab[i1], &pTab[ii]);
  else        d1 = -1;
  // second segment:
  i2 = ii + 1;
  if(i2 < pnr) UT3D_parpt_3pt (&d2, pt, &pTab[ii], &pTab[i2]);
  else        d2 = -1;
    // printf(" d1=%f d2=%f\n",d1,d2);

  if((d2 < 0.)||(d2 > 1.)) {
    // use 1. segment (ii-1, ii)
    dd = UTP_px_paramp0p1px (vTab[i1], vTab[ii], d1);
  } else {
    // use 2. segment (ii, ii+1)
    dd = UTP_px_paramp0p1px (vTab[ii], vTab[i2], d2);
  }  
    // printf(" dd=%f\n",dd);


  // create point with parameter dd
  UT3D_pt_evparCrvRBSpl (&pt1, &d1, cv, dd);
  ptOut[*nxp] = pt1;
  if(vaOut) vaOut[*nxp] = dd;
  ++(*nxp);
    // GR_Disp_pt (ptOut, SYM_STAR_S, 1);



  L_done:
  memSeg1->next = memPos0;  // restore memSeg1

    // printf("ex UT3D_pt_projptrbspl nxp=%d %f,%f,%f  d=%f\n",*nxp,
            // ptOut[0].x,ptOut[0].y,ptOut[0].z,vaOut[0]);


  return 0;

}


//================================================================
  double UT3D_par_par1_rbsp (double pv, CurvRBSpl* cv1) {
//================================================================
/// \code
/// UT3D_par_par1_rbsp       get knotValue from parameter 0-1
/// see also UT3D_parbsp_par1
/// \endcode

  double kv, uTot, uMin, uMax;


  // printf("UT3D_par_par1_rbsp %f\n",pv);

  // uMin = cv1->kvTab[0];
  // uMax = cv1->kvTab[cv1->ptNr + cv1->deg];

  uMin = cv1->v0;
  uMax = cv1->v1;

    // printf("UT3D_par_par1_rbsp pv=%lf uMin=%lf uMax=%lf\n",pv,uMin,uMax);
    // printf("  in v0=%lf v1=%lf\n",cv1->v0,cv1->v1);


  kv = uMin + ((uMax - uMin) * pv);

    // printf("ex UT3D_par_par1_rbsp %lf\n",kv);

  return (kv);

}


//=========================================================
  int UT3D_rbspl_ck_closed (CurvRBSpl *cv1) {
//=========================================================
// UT3D_sbs_ck_closed        check if B-SplSurf is closed/degenerated
// ignore trimmed.
// irc =  1 = cv is NOT closed (normal)
// irc =  0 = cv is closed
// irc = -2 = cv is degenerated (shorter UT_TOL_cv)
// see also UT3D_bsp_ck_closed__

  int     i1, ie, irc;
  double  d1;

  ie = cv1->ptNr - 1;

  // test if first-point == last-point
  if(UT3D_comp2pt(&cv1->cpTab[0], &cv1->cpTab[ie], UT_TOL_cv)) {

    if(ie < 2) return -2;   // 2 points only, degen !

    // find 2 points with a dist > UT_TOL_cv
    for(i1 = 2; i1 <= ie; ++i1) {
      // get max dist between 2 points
      d1 = UT3D_lenB_2pt (&cv1->cpTab[i1 - 1], &cv1->cpTab[i1]);
      if(d1 > UT_TOL_cv) return 0; // closed
    }

    return -1; // degenerated

  }

  return 1; // not closed

}


//=====================================================================
  int UT3D_obj_cnvt_rbspl (ObjGX *opo, CurvRBSpl *spi, Memspc *mpWrk) {
//=====================================================================
// UT3D_obj_cnvt_rbspl          convert Rat.B-Spl --> CI/LN; see UT3D_bsp_degrad

// EV SOLLTE MAN NOCH RBSPL's in BSPL's aendern, wenn alle weigths 1 sind !!!

// RetCod: >0  OK, rbspl converted; RetCod is Typ_LN/Typ_CI/Typ_BSP.
//         -1  No; cannot convert rbspl.

  int      irc, i1, i2, i3;
  double   d1, d2;
  Point    p1, p2, p3;
  Circ     *cp1, c1;
  Line     *lp1;
  CurvBSpl *cv1;


  // printf("UT3D_obj_cnvt_rbspl\n");
  // UT3D_stru_dump (Typ_CVRBSP, spi, "UT3D_obj_cnvt_rbspl in:");

  irc = -1;



  //======= LINE ===================================================
  // testen ob curve eine LN ist;
  // ptNr muss 2 sein  (nun muss degree 1 sein ..)
  if(spi->ptNr > 2) goto L_ck_Circ;


  // add Line to memSpc
  lp1 = (Line*)mpWrk->next;
  irc = UME_add (mpWrk, sizeof(Line));
  if(irc < 0) return -1;

  // mit ps, pe begrenzen
  UT3D_pt_evparCrvRBSpl (&lp1->p1, &d1, spi, spi->v0);
  UT3D_pt_evparCrvRBSpl (&lp1->p2, &d1, spi, spi->v1);

  // *ox1 = UTO_stru_2_obj (Typ_LN, Typ_LN, (void*)ln1);
  opo->typ  = Typ_LN;
  opo->form = Typ_LN;
  opo->siz  = 1;
  opo->data = (void*)lp1;

  irc = Typ_LN;
  goto L_exit;





  //================================================================
  //================================================================
  // testen ob curve ein Circ ist.
  // Degree 2: weights sind cyclic; jeder 2. punkt hat w=1 / liegt am Circ.
  L_ck_Circ:
  if(spi->deg != 2) goto L_Circ_deg3;
   // printf(" ck weigths\n");

  // check if weights are cyclic
  d1 = spi->wTab[0];
  d2 = spi->wTab[1];
  i2 = 0;
  for(i1=2; i1<spi->ptNr; ++i1) {
    if(i2 == 0) {
      if(spi->wTab[i1] != d1) {
        // printf(" err1 w[%d] %f %f\n",i1,spi->wTab[i1],d1);
        goto L_std_circ;
      }
      i2 = 1;
    } else {
      if(spi->wTab[i1] != d2) {
        // printf(" err2 w[%d] %f %f\n",i1,spi->wTab[i1],d1);
        goto L_std_circ;
      }
      i2 = 0;
    }
  }




  //----------------------------------------------------------------
  // check if closed ....
  L_closed:
    // printf("\n\n ck closed\n");
  i1 = UT3D_rbspl_ck_closed(spi);
  if(i1 > 0) goto L_std_circ;

  // curve is closed; get 3 opposite points --> 1,3,5.
  // 9 Points == 4-Eck, 7-points == 3-Eck.

  // normalvec to 3 pt's
  UT3D_vc_perp3pt (&c1.vz, &spi->cpTab[1], &spi->cpTab[3], &spi->cpTab[5]);
  // test for length 0 
  if(UT3D_compvc0 (&c1.vz, UT_TOL_min0) == 1) goto L_ck_bsp;
  UT3D_vc_setLength (&c1.vz, &c1.vz, 1.);


  // Kreismittelpunkt: 4-Eck: mitte von p[1] - p[5]
  if(spi->ptNr == 9) {
    UT3D_pt_mid2pt (&c1.pc, &spi->cpTab[1], &spi->cpTab[5]);
    // make 360-deg-circ from p[0]
    UT3D_ci_cipt360 (&c1, &spi->cpTab[0]);
    goto L_ck_planar;


  } else if(spi->ptNr == 7) {
    // get circ from 3 points (0,2,4)
    UT3D_ci_3pt (&c1, &spi->cpTab[0], &spi->cpTab[2], &spi->cpTab[4]);
    goto L_ck_planar;


  }
  // nicht 7 und nicht 9 Punkte .. ??
  // gesamtlen < UT_DISP_cv; nix tun ..
  // hier zB ein sehr kurzer Arc (3 PT's) der kleiner als tol ist ..
  // TX_Print("UT3D_obj_cnvt_rbspl I2C1");
  // UT3D_stru_dump (Typ_CVRBSP, spi, "UT3D_obj_cnvt_rbspl");
  goto L_std_circ;

  




  //================================================================
  //================================================================
  // testen ob curve ein Circ ist.
  // Degree 3: hat zB 4 Punkte, die mittleren 2 sind nicht am Kreis !
  // weights ca.: 1 0.3 0.3 1
  L_Circ_deg3:
  if(spi->deg != 3) goto L_std_circ;


  // Halbkreis: 4 Punkte im Quadrat.
  // check if its a sqare
  if(spi->ptNr != 4) goto L_std_circ;
    // printf("L_Circ_deg3:\n");
    // UT3D_stru_dump (Typ_CVRBSP, spi, "UT3D_obj_cnvt_rbspl");
    // TX_Print("UT3D_obj_cnvt_rbspl I002 %d",spi->ptNr);    exit(0);


  // weights testen; die mittleren duerfen nicht 1 sein.
  if(UTP_comp2db(spi->wTab[1], 1., UT_TOL_PAR) == 1) goto L_std_circ;
  if(UTP_comp2db(spi->wTab[2], 1., UT_TOL_PAR) == 1) goto L_std_circ;


  // normalvec to 3 pt's
  UT3D_vc_perp3pt (&c1.vz, &spi->cpTab[0], &spi->cpTab[1], &spi->cpTab[3]);
  // test for length 0 
  if(UT3D_compvc0 (&c1.vz, UT_TOL_min0) == 1) goto L_ck_bsp;
  UT3D_vc_setLength (&c1.vz, &c1.vz, 1.);

  // Kreismittelpunkt: 4-Eck: mitte von p[1] - p[5]
  UT3D_pt_mid2pt (&c1.pc, &spi->cpTab[0], &spi->cpTab[3]);

  // UT3D_ci_cipt180
  c1.p1  = spi->cpTab[0];
  c1.p2  = spi->cpTab[3];
  c1.rad = UT3D_len_2pt (&spi->cpTab[0], &spi->cpTab[3]) / 2.;
  c1.ango = RAD_180;





  // check if controlpoints are planar ...
  // testen ob alle Punkte planar
  L_ck_planar:
  irc = UT3D_pta_ck_planar (spi->ptNr, spi->cpTab, &c1.pc, &c1.vz);
  if(irc < 0) goto L_ck_bsp;
  goto L_circ_OK;



  //================================================================
  //================================================================
  // total allgemeinen circ testen;
  L_std_circ:
    // printf(" L_std_circ: \n");
  irc = UT3D_ci_rbspl_ (&c1, spi);
  if(irc < 0) goto L_ck_bsp;



  //================================================================
  //================================================================
  // circ fertig; create object mit circ.
  L_circ_OK:

  // add Circ to memSpc
  cp1 = (Circ*)mpWrk->next;
  irc = UME_add (mpWrk, sizeof(Circ));
  if(irc < 0) return -1;

  // den kreis kopieren
  *cp1 = c1;

  // *ox1 = UTO_stru_2_obj (Typ_LN, Typ_LN, (void*)ln1);
  opo->typ  = Typ_CI;
  opo->form = Typ_CI;
  opo->siz  = 1;
  opo->dir  = 0;
  opo->data = (void*)cp1;

    // GR_Disp_ac (cp1, 9);
    // if(isnan(cp1->rad)) exit(0);


  irc = Typ_CI;
  goto L_exit;




  //================================================================
  // check if it is a Bspline (all weights must be 1.)
  L_ck_bsp:
    
  for(i1=0; i1<spi->ptNr; ++i1)
    if(UTP_comp2db(spi->wTab[i1], 1., UT_TOL_PAR) != 1) goto L_exit;

  // ok, degrade this to B-Spline.
  // printf(" UT3D_obj_cnvt_rbspl EEEEEEEEEEE\n");
  // UT3D_stru_dump (Typ_CVRBSP, spi, "UT3D_obj_cnvt_rbspl");
  // exit(0);


  cv1 = (CurvBSpl*)mpWrk->next;
  irc = UME_add (mpWrk, sizeof(CurvBSpl));
  if(irc < 0) return -1;

  cv1->ptNr = spi->ptNr;
  cv1->deg  = spi->deg;
  cv1->v0   = spi->v0;
  cv1->v1   = spi->v1;
  cv1->kvTab= spi->kvTab;
  cv1->cpTab= spi->cpTab;

  opo->typ  = Typ_CVBSP;
  opo->form = Typ_CVBSP;
  opo->siz  = 1;
  opo->data = (void*)cv1;

  // goto L_obj_OK;

  irc = Typ_CVBSP;







  //================================================================
  L_exit:

  // printf("ex UT3D_obj_cnvt_rbspl %d\n",irc);
  // if(irc > 0) UTO_dump__ (opo, "ex UT3D_obj_cnvt_rbspl");

  return irc;

}


//================================================================
  int UT3D_ci_rbspl_ (Circ *cpo, CurvRBSpl *spi) {
//================================================================
// try to degrade RBSP --> Circ;
// Den KreisWinkel feststellen:
// Wenn (da < RAD_60): geschlossen: 3 Punkt (0, 0.33, 0.66) erzeugen
// else:   3 Punkt (0, 0.5, 1) erzeugen.
// Kreis aus den 3 Punkten.
// Kreis mit p1, pEnd begrenzen.
// Nochmal alle 45 Grad ueberpruefen ..
// RetCod: Typ_CI == OK; else -1.

  int    i1, i2;
  double da, di, d1, d2, kTot;
  Point  p1, p2, p3, p4;


  // printf("UT3D_ci_rbspl_ \n");


  // wenn Curve nur 3 Kontrollpunkte hat: UT3D_ci_rbspl3.
  if(spi->ptNr == 3) {
    // curve has only 3 controlpoints;
    return UT3D_ci_rbspl3 (cpo, spi);
  }




  // Den KreisWinkel feststellen:
  //  da=den Oeffnungswinkel des ersten, des mittleren und des letzten
  //  Kontrollpunktes errechnen; (360-da ist der Kreisumfangswinkel).
  i2 = spi->ptNr - 1;         // index last Controlpoint
  i1 = i2 / 2;                // index mittlerer Controlpoint
  da = UT3D_angr_3pt (&spi->cpTab[0], &spi->cpTab[i1], &spi->cpTab[i2]);
    // printf(" da=%f\n",da);

  kTot = (spi->v1 - spi->v0);

  //----------------------------------------------------------------
  // Kreis mit > 300 Grad Umfangswinkel; make Circ aus pt's (0, 0.33, 0.66).
  if(da < RAD_60) goto L_small;
    // printf(" >300  %f %f\n",spi->v0+(kTot*0.33),spi->v0+(kTot*0.66));
  UT3D_pt_evparCrvRBSpl (&p1, &d1, spi, spi->v0);
  UT3D_pt_evparCrvRBSpl (&p2, &d1, spi, spi->v0+(kTot*0.33));
  UT3D_pt_evparCrvRBSpl (&p3, &d1, spi, spi->v0+(kTot*0.66));
  UT3D_ci_3pt (cpo, &p1, &p2, &p3);
  UT3D_pt_evparCrvRBSpl (&p3, &d1, spi, spi->v1);
  goto L_ck1;


  //----------------------------------------------------------------
  // Kreis mit < 300 Grad Umfangswinkel; make Circ aus pt's (0, 0.5, 1).
  L_small:
    // printf(" <300  %f\n",spi->v0+(kTot*0.5));
  UT3D_pt_evparCrvRBSpl (&p1, &d1, spi, spi->v0);
  UT3D_pt_evparCrvRBSpl (&p2, &d1, spi, spi->v0+(kTot*0.5));
  UT3D_pt_evparCrvRBSpl (&p3, &d1, spi, spi->v1);
  UT3D_ci_3pt (cpo, &p1, &p2, &p3);


  L_ck1:
  UT3D_ci_cip3 (cpo, &p1, &p3);  // Circ --> Arc
    // GR_Disp_ac (cpo, 9);


  // Nochmal alle 45 Grad ueberpruefen.
  // der Gesamtumfangswinkel ist 360-da; dieses / Rad_45 ist Anzahl von Tests.
  // kTot / anzahl = inkrement von k.
  d1 = RAD_360 - da;
  i1 = d1 / RAD_45;
  di = kTot / i1;      // increment k
  // printf(" i1=%d di=%f\n",i1,di);

  d1 = spi->v0 + di;

  while (d1 < spi->v1) {
    // printf(" nxt d1=%f %f\n",d1,spi->v1);
    // naechsten Punkt am Kreis testen ..
    UT3D_pt_evparCrvRBSpl (&p1, &d2, spi, d1);
    d2 = UT3D_len_2pt (&p1, &cpo->pc);
    // printf(" r=%f d2=%f\n",cpo->rad,d2);
    if(UTP_comp2db(d2,fabs(cpo->rad),UT_DISP_cv) == 0) return -1;

    d1 += di;
  }

  // printf("ex UT3D_ci_rbspl_ %d\n",Typ_CI);
  return Typ_CI;

}


//================================================================
  int UT3D_ci_rbspl3 (Circ *cpo, CurvRBSpl *spi) {
//================================================================
// create circle from Rat.B-SplineCurve mit nur 3 ControlPoints.
//
// TODO: wenn p2 nicht mittig: Ellipse erzeugen ?
//
// weight*Radius = Laenge (pm-Center);
//       pm ist der Mittelpunkt der beiden ausseren Punkte p1,p3.
// UT2D_solvtriri_c_abfc
// scalene tri: c from a and x; b=x*c;
//
//          p1
//        / |\              pM = Mitte p1 / p3
//       /  | \             v1 = Vektor p2 - pm
//     /   a|  \c           Cen = Mittelpunkt des gesuchten Kreises
//    /  |  |   \           vZ  = Kreuzvek v1 / vec(p2-p1)
//  p2---|-pM--x-Cen        c = radius des gesuchten Kreises
//   \   |  |               (p2-Cen) = weight[p2] * Radius
//     \    |               a = (pM-p1) = (pM-p3)
//      \   |               x = (pM-Cen) = sqrt(c*c - a*a)
//        \ |
//          p3
//
// Retcod:
//   0    OK
//  -1    cannot create vZ


  double a, c, x;
  Point  pM;
  Vector vmc, vp1p2;

  // printf("UT3D_ci_rbspl3 w=%f\n",spi->cpTab[2]);
  // UT3D_stru_dump (Typ_PT, &spi->cpTab[0], "p0: ");
  // UT3D_stru_dump (Typ_PT, &spi->cpTab[1], "p1: ");
  // UT3D_stru_dump (Typ_PT, &spi->cpTab[2], "p2: ");
    // GR_Disp_pt (&spi->cpTab[0], SYM_STAR_S, 2);
    // GR_Disp_pt (&spi->cpTab[1], SYM_STAR_S, 2);
    // GR_Disp_pt (&spi->cpTab[2], SYM_STAR_S, 2);
  // disp points
  // UT3D_rbspl_tst_cc (spi);


  // test if p2 has same distance from p1 and p3
  if(UT3D_pt_ck_mid_2pt (&spi->cpTab[0],
                         &spi->cpTab[2],
                         &spi->cpTab[1], &UT_TOL_pt) != 1) return -1;


  // create normalvec from p0 p1 p2
  UT3D_vc_perp3pt (&cpo->vz, &spi->cpTab[0], &spi->cpTab[1], &spi->cpTab[2]);
  if(UT3D_compvcNull (&cpo->vz) == 1) return -1;
  UT3D_vc_setLength (&cpo->vz, &cpo->vz, 1.);


  // pM = Mittelpunkt p0-p2
  UT3D_pt_mid2pt (&pM, &spi->cpTab[0], &spi->cpTab[2]);
    // GR_Disp_pt (&pM, SYM_SQU_B, 2);


  // create normalvec from pM to Cen
  UT3D_vc_2pt (&vp1p2, &spi->cpTab[0], &spi->cpTab[2]);
  UT3D_vc_perp2vc (&vmc, &vp1p2, &cpo->vz);


  // a = Laenge p0-p2 / 2  (= Abstand p0 - pM)
  a = UT3D_len_2pt (&spi->cpTab[0], &spi->cpTab[2]) / 2.;
  // printf(" a=%f w=%f\n",a,spi->wTab[1]);


  UT2D_solvtriri_c_abfc (&c, a, spi->wTab[1]);
  // printf(" c=%f\n",c);

  // c = radius
  cpo->rad = c;

  // x = (pM-Cen) = sqrt(c*c - a*a)
  x = sqrt (c*c - a*a);
    // printf(" x=%f\n",x);

  // c = Radius ab pM in Richtung p1-->pM
  // UT3D_pt_traptptlen (&cpo->pc, &pM, &spi->cpTab[1], -x);
  UT3D_pt_traptvclen (&cpo->pc, &pM, &vmc, -x);
    // GR_Disp_pt (&cpo->pc, SYM_SQU_B, 2);


  // create arc from p0-p2
  UT3D_ci_cip3 (cpo, &spi->cpTab[0], &spi->cpTab[2]);

  // UT3D_stru_dump (Typ_CI, cpo, "ex UT3D_ci_rbspl3\n");

  return 0;

}

//================================================================
  int UT3D_ptNr_rbsp (int *iu, CurvRBSpl *bsp, double tol1) {
//================================================================
/// \code
/// UT3D_ptNr_rbsp     estimate nr of polygonPoints for rat-B-spline
/// see also UT3D_ptNr_bsp UT3D_cv_rbsp
/// \endcode

  int    i2, i3, ip;
  double ao, a1, d1, d2;
  Vector vc1, vc2;


  // UT3D_stru_dump (Typ_CVRBSP, bsp, "UT3D_ptNr_rbsp:\n");


  if(bsp->ptNr < 3) {*iu = 2; return 0;}

  // if(bsp->deg == 1) { ??


  //===============================================================
  // Toleranz voff: loop durch Kurve, max. Abweichung pro Laenge suchen

  *iu = 0;
  tol1 /= 8.;  // else too much bufferOverflows .. 2014-06-04

  // degree
  d1 = bsp->deg - 1.;     // printf(" _ptNr_bsp-d1=%f\n",d1);
  tol1 /= d1;


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
    *iu += ip + 2;

      // printf(" %d ao=%f d2=%f ip=%d\n",i3,UT_DEGREES(ao),d2,ip);
  }


  if(*iu < 2) *iu = 2;

  // printf("ex UT3D_ptNr_rbsp %d\n",*iu);


  return 0;

}


//========================================================================
  int UT3D_cv_rbsp (int *ptNr, Point *pTab, double *dTab,
                    CurvRBSpl *cv1, int ptMax, double tol) {
//========================================================================
// UT3D_cv_rbsp               make Polygon from Rat.B-Spline-Kurve
// tol (typ. 0.003) kleinste Strecke zur Berechnung der Abweichung
// Die Anzahl von Polygonsegment entspricht der Toleranz (NUR OUT !!)

// Output:
//   vTab       curveparameter of corresponding point (if vTab <> NULL)
// RetCode: 0 = OK
//         -1 = pTab zu klein (E001)



  int    i1, ianz, ideg, irc, ptOut, ptStackNr, iout, idir;
  double d1, *vTab, pVal, kvs, kve, u1, u2, us, ue, dist, uStack[20], pw;
  Point  *ptCrv, pt1, pts, pte, ptStack[20];



  // printf("UT3D_cv_rbsp ptNr=%d deg=%d tol=%f\n",cv1->ptNr,cv1->deg,tol);
  // printf("          v0=%f v1=%f ptMax=%d\n",cv1->v0,cv1->v1,ptMax);
  // UT3D_stru_dump (Typ_CVRBSP, cv1, "");


  ptOut = 0;

  ianz  = cv1->ptNr - 1;
  ideg  = cv1->deg;
  ptCrv = cv1->cpTab;
  vTab  = cv1->kvTab;

  kvs   = cv1->v0;
  kve   = cv1->v1;
  if(kvs < kve) idir = 1;
  else          idir = -1;
    // printf("  kvs=%f kve=%f idir=%d\n",kvs,kve,idir);

  u1 = vTab[0];
  u2 = vTab[cv1->ptNr + ideg];
    // printf(" u1=%f u2=%f\n",u1,u2);


  // bspl_pol_bs wird immer von v0 nach v1 abgearbeitet.
  // u1 ist aber immer kleiner als u2.


  // wenn kvs=v0 outside Curve: add Point.
  iout = UTP_db_ck_in2db (kvs, u1, u2);
    // printf(" iout=%d\n",iout);
  if(iout == 0) goto L_1;

  if(iout != 0) {
    if(idir > 0) kvs = u1;
    else         kvs = u2;
  }

  d1 = cv1->v0 - kvs;
  // if(idir < 0) d1 = kvs - cv1->v0;
  // else         d1 = cv1->v0 - kvs;
    // printf(" d1=%f\n",d1);

  // get Startpt CV
  // bspl_eval_expPt (&pTab[ptOut], cv1, kvs, d1);
  // UT3D_pt_evparxCrvRBSpl (&pTab[ptOut], cv1, kvs, d1);
  UT3D_pt_evparCrvRBSpl (&pTab[ptOut], &pw, cv1, kvs);  // 2014-06-21
  if(dTab) dTab[ptOut] = kvs;
  ++ptOut;          // der aktuelle Punkt ist immer pTab[ptOut-1].



  //----------------------------------------------------------------
  L_1:

  // testen ob kve innerhalb Bereich liegt
  iout = UTP_db_ck_in2db (kve, u1, u2);
    // printf(" iout=%d\n",iout);
  if(iout != 0) {
    if(idir > 0) kve = u2;
    else         kve = u1;
  }
    // printf(" startCurve; kvs=%f kve=%f\n",kvs,kve);


  // // Init Datensegmente
  // UME_init (&workSeg, workspc, sizeof(workspc));


  // den ersten Pt sofort raus
  // irc = bspl_eval_Pt (ianz, ideg, ptCrv, vTab, kvs, &pTab[ptOut]);
  irc = UT3D_pt_evparCrvRBSpl (&pTab[ptOut], &d1, cv1, kvs);
    // UT3D_stru_dump (Typ_PT, &pTab[ptOut], " kvs=%f StartPt",kvs);
  if(dTab) dTab[ptOut] = kvs;
  ++ptOut;          // der aktuelle Punkt ist immer pTab[ptOut-1].



  // nur 2 Punkte -
  if(ianz == 1) {
    pTab[ptOut] = ptCrv[1]; // copy Point
    if(dTab) dTab[ptOut] = kve;
    ++ptOut;
    goto L_fertig;
  }



  // den letzen Punkt auf den Stack.
  ptStackNr = 0;
  uStack[ptStackNr] = kve;
  // irc = bspl_eval_Pt (ianz, ideg, ptCrv, vTab, kve, &ptStack[ptStackNr]);
  irc = UT3D_pt_evparCrvRBSpl (&ptStack[ptStackNr], &d1, cv1, kve);
    // UT3D_stru_dump (Typ_PT, &ptStack[ptStackNr], " kve=%f EndPt",kve);
  ++ptStackNr;

  pts = pTab[0];
  us = kvs;



  //-------------------------------------------
  // der Startpunkt ist pts; den naechsten Endpunkt vom ptStack holen;
  // dessen u-Wert ist uStack.

  L_next_inPoint:
  // den naechsten Endpunkt vom Stack -> pte holen
  pte = ptStack[ptStackNr-1];
  ue  = uStack[ptStackNr-1];

  // u-Mittelwert errechnen
  pVal = (us + ue) / 2.0;

  // printf("next pVal=%f us=%f ue=%f ptStackNr=%d\n",pVal,us,ue,ptStackNr);

  // den Punkt dazu holen (Zwischenpunkt)
  // irc = bspl_eval_Pt (ianz, ideg, ptCrv, vTab, pVal, &pt1);
  irc = UT3D_pt_evparCrvRBSpl (&pt1, &d1, cv1, pVal);
  // printf(" irc=%d pt1=%f %f %f\n",irc,pt1.x,pt1.y,pt1.z);
  if(irc < 0) return irc;


  if(ptStackNr == 1) {
    // wenn pts == pte, dann ist es eine closed curve; noch einen Zwischenpunkt!
    if(UT3D_comp2pt(&pts, &pte, UT_TOL_pt) == 1) {
      // es gibt den Sonderfall, dass alle Punkte gleich sind (zB die Spitze
      // eines Dreiecks). Wenn pt1 ebenfalls gleich, dann
      // nur einen Punkt liefern
      // printf(" Anf==End\n");
      if(UT3D_comp2pt(&pts, &pt1, UT_TOL_pt) == 1) {
        goto L_exit;
      }
      dist = UT_VAL_MAX;
      goto L_test;
    }
  }

  // die Abweichung feststellen
  dist = UT3D_nlen_3pt (&pts, &pt1, &pte);
  // if(ptStackNr > 1) {dist = 0.001;} else {dist = 10.0;} // TEST


  // if(dist > tol) {
  // mind. 3 Punkte; sonst geht eine S-Form nicht !!
  L_test:
  if((dist > tol)||((ptOut < 2)&&(ptStackNr<2))) {
    // Abweichung zu gross; Punkt speichern
    // printf(" >>>> save %d\n",ptStackNr);
    ptStack[ptStackNr] = pt1;
    uStack[ptStackNr]  = pVal;
    ++ptStackNr;
    if(ptStackNr >= 20) return -1;
    goto L_next_inPoint;
  }


  // Abweichung tolerierbar. pte raus.
  pTab[ptOut] = pte; // copy Point
  if(dTab) dTab[ptOut] = ue;
  if(ptOut >= ptMax) {
    TX_Error("UT3D_cv_rbsp E001");
    return -1;
  }
  ++ptOut;
  --ptStackNr;
  pts = pte;
  us = ue;
  if(ptStackNr > 0) goto L_next_inPoint;




  //-------------------------------------------
  L_fertig:

  // wenn v1 outside Curve: add Point.
  if(iout == 0) goto L_exit;

  d1 = cv1->v1 - kve;
  // if(idir > 0) d1 = cv1->v1 - kve;
  // else         d1 = kve - cv1->v1;
    // printf(" d1=%f\n",d1);

  // get Endpt CV
  // bspl_eval_expPt (&pTab[ptOut], cv1, kve, d1);
  // UT3D_pt_evparxCrvRBSpl (&pTab[ptOut], cv1, kve, d1);
  UT3D_pt_evparCrvRBSpl (&pTab[ptOut], &pw, cv1, kve);  // 2014-06-21
  if(dTab) dTab[ptOut] = kve;
  ++ptOut;






  //----------------------------------------------------------------
  L_exit:

  *ptNr = ptOut;

  // swap all points ...
  if(cv1->dir) {
    UT3D_cv_inv (*ptNr, pTab);
  }


  //------- Testausg------
  // printf("ex UT3D_cv_rbsp ptNr=%d\n",ptOut);
  // for(i1=0;i1<ptOut;++i1) {
    // UT3D_stru_dump (Typ_PT, &pTab[i1], "P[%d]=",i1);
    // APT_disp_SymB (SYM_TRI_S, 2, &pTab[i1]);
  // }
  //------- Testausg------


  // printf("ex UT3D_cv_rbsp v0=%f v1=%f\n",cv1->v0,cv1->v1);
  return 0;

}


//================================================================
  int UT3D_pt_evparxCrvRBSpl (Point *pto,
                              CurvRBSpl *cv1, double uv, double dist) {
//================================================================
// uv is outside curve;  iout is from func UTP_db_ck_in2db
// extrapolate len=dist
// see bspl_eval_expPt
 

  TX_Error("UT3D_pt_evparxCrvRBSpl E001");
  printf(" uv=%lf\n",uv);


  return 0;

}


//================================================================
  int UT3D_pt_evparCrvRBSpl (Point *pt, double *wt,
                             CurvRBSpl *rbspl, double u) {
//================================================================
/// \code
/// UT3D_pt_evparCrvRBSpl    point <-- rational b-spline at parameter
/// 
/// UT3D_pt_evparCrvRBSpl    Author: Thomas Backmeister       18.9.2004
/// 
/// Evaluate a point on a rational b-spline curve.
/// The underlying algorithm is "rational de Boor".
/// The point is computed for any parameter value within the maximal
/// support of the curve (we do not consider the limit parameters
/// rbspl->v0 and rbspl->v1).
/// The function needs work space of size
/// (curve degree + 1) * (sizeof(Point) + sizeof(double)).
/// 
/// IN:
///   CurvRBSpl *rbspl  ... rational b-spline curve
///   double u          ... parameter value
///   Memspc *workSeg   ... work space (control points and weights)
/// OUT:
///   Point  *pt        ... curve point
///   double *wt        ... weight for *pt
/// Returncodes:
///   0 = OK
///  -1 = out of work space
///  -2 = input error
///
/// \endcode

  int rc, d, n, m, r, s;
  int i1, i2, ih, jh, n1;
  double *wtab, w_old, a, b;
  void *memstart;
  Point *ptab, p1, p2;

// UT3D_pt_evparCrvRBSpl ist nicht rekursiv; daher lokal moeglich !
  char      tmpspc[100000];
  Memspc    tmpSeg, *workSeg;


  // Init Datensegmente
  UME_init (&tmpSeg, tmpspc, sizeof(tmpspc));
  workSeg = &tmpSeg;



  // printf("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\n");
  // printf("UT3D_pt_evparCrvRBSpl %lf\n",u);
  // UT3D_stru_dump (Typ_CVRBSP, rbspl, "");


  // some abbreviations
  d = rbspl->deg;
  n = rbspl->ptNr - 1;
  m = n + d + 1;

  // check parameter
  if (u < rbspl->kvTab[d]) {
    // printf("UT3D_pt_evparCrvRBSpl: parameter below %f\n",u);
    u = rbspl->kvTab[d];
  } 
  if (u > rbspl->kvTab[rbspl->ptNr]) {
    // printf("UT3D_pt_evparCrvRBSpl: parameter above %f\n",u);
    u = rbspl->kvTab[rbspl->ptNr];
  }

  // determine segment of parameter
  r = -1;
  for (s=d; s<m-d; ++s) {
    if (rbspl->kvTab[s] <= u && u <= rbspl->kvTab[s+1]) {
      r = s;
      break;
    }
  }
  if (r == -1) {
    printf("UT3D_pt_evparCrvRBSpl: input error\n");
    return -2;
  }

  // provide work space
  memstart = workSeg->next;
  ptab = workSeg->next;
  rc = UME_add (workSeg, sizeof(Point) * (d+1));
  if (rc < 0) goto L_outOfWorkSpace;
  wtab = workSeg->next;
  rc = UME_add (workSeg, sizeof(double) * (d+1));
  if (rc < 0) goto L_outOfWorkSpace;
 
  // init control points & weights
  ih = r - d;
  for (i1=0; i1<d+1; ++i1) {
		ptab[i1] = rbspl->cpTab[ih+i1];
		wtab[i1] = rbspl->wTab[ih+i1];
	}	

  // compute curve point
  n1 = d;
  for (i1=0; i1<d; ++i1) {
    ih = r + 1;
    jh = ih - n1;
    for (i2=0; i2<n1; ++i2) {
      a = (u - rbspl->kvTab[jh+i2])/(rbspl->kvTab[ih+i2] - rbspl->kvTab[jh+i2]);
      b = 1.0 - a;
			w_old = wtab[i2];
      wtab[i2] = b * wtab[i2] + a * wtab[i2+1];        
      if (w_old == 0.0) p1 = ptab[i2];
			else {
				p1.x = w_old * ptab[i2].x;
				p1.y = w_old * ptab[i2].y;
				p1.z = w_old * ptab[i2].z;
			}	
      if (wtab[i2+1] == 0.0) p2 = ptab[i2+1];
			else {
				p2.x = wtab[i2+1] * ptab[i2+1].x;
				p2.y = wtab[i2+1] * ptab[i2+1].y;
				p2.z = wtab[i2+1] * ptab[i2+1].z;
			}	
      ptab[i2].x = (b * p1.x + a * p2.x);
      ptab[i2].y = (b * p1.y + a * p2.y);
      ptab[i2].z = (b * p1.z + a * p2.z);
			if (wtab[i2] != 0.0) {
				ptab[i2].x /= wtab[i2];			
				ptab[i2].y /= wtab[i2];			
				ptab[i2].z /= wtab[i2];
	    }			
    }
    --n1;
  }
  *pt = ptab[0];
  *wt = wtab[0];

  // if(*wt == 0.) *wt = 1.;       // soll lt Thomas wichtig sein .. ??

  // release work space
  workSeg->next = memstart;

    // UT3D_stru_dump (Typ_PT, pt, "ex UT3D_pt_evparCrvRBSpl:");

  return 0;



  L_outOfWorkSpace:
    TX_Error("UT3D_pt_evparCrvRBSpl: out of work space");
  return -1;
}


/*=======================================================================*/
  int UT3D_bspl_deriv1 (CurvBSpl *bspld, Memspc *memSeg1, CurvBSpl *bspl) {
/*====================
UT3D_bspl_deriv1       1. derivation b-spline curve <-- b-spline curve

UT3D_bspl_deriv1       Author: Thomas Backmeister       26.9.2004

Computation of the 1. derivation of a b-spline curve.
This is again a b-spline curve of degree 1 less than degree of original curve.

IN:
  CurvBSpl *bspl  ... b-spline curve
OUT:
  CurvBSpl *bspld ... 1. derivation b-spline curve
Returncodes:
  0 = OK
 -1 = Memspc too small
 -2 = input error
*/

  int i1, deg, rc;
	double t;

  // printf("UT3D_bspl_deriv1\n");

  // check input
  if (bspl->deg < 1) {
    printf("UT3D_bspl_deriv1: curve degree < 1 -> no derivation!\n");
    return -2;
  }

  // init derivation curve
  bspld->deg = bspl->deg - 1;
  bspld->ptNr = bspl->ptNr - 1;
  bspld->v0 = bspl->v0;	
  bspld->v1 = bspl->v1;	
 
  // space for control points and knotvector
	bspld->cpTab = memSeg1->next;
  rc = UME_add (memSeg1, sizeof(Point) * bspld->ptNr);
  if (rc < 0) return -1;
	bspld->kvTab = memSeg1->next;
  rc = UME_add (memSeg1, sizeof(double) * (bspld->ptNr+bspld->deg+1));
  if (rc < 0) return -1;

  // knotvector of derivation curve
	for (i1=0; i1<bspld->ptNr+bspld->deg+1; ++i1)
		bspld->kvTab[i1] = bspl->kvTab[i1+1];

  // control points of derivation curve
  for (i1=0; i1<bspld->ptNr; ++i1) {
		t = bspl->kvTab[i1+bspl->deg+1] - bspl->kvTab[i1+1];
    bspld->cpTab[i1].x = bspl->deg * (bspl->cpTab[i1+1].x - bspl->cpTab[i1].x) / t;
    bspld->cpTab[i1].y = bspl->deg * (bspl->cpTab[i1+1].y - bspl->cpTab[i1].y) / t;
    bspld->cpTab[i1].z = bspl->deg * (bspl->cpTab[i1+1].z - bspl->cpTab[i1].z) / t;
  }

  return 0;
}


/*=======================================================================*/
  int UT3D_rbspl_deriv1 (CurvBSpl *g, CurvBSpl *fd, CurvBSpl *gd,
									       Memspc *memSeg1, CurvRBSpl *rbspl) {
/*=====================
UT3D_rbspl_deriv1       1. deriv. rat. b-spline curve <-- rat. b-spline curve

UT3D_rbspl_deriv1       Author: Thomas Backmeister       26.9.2004

Computation of the "1. derivation" of a rational b-spline curve, i.e.
the denominator b-spline curve and the 1. derivation nominator and denominator
b-spline curves of the rational b-spline curve.

IN:
  CurvRBSpl *rbspl  ... rational b-spline curve
	Memspc *memSeg1   ... memory space for g, fd, gd (and f, another bspl curve)
OUT:
  CurvBSpl *g       ... denominator b-spline curve
  CurvBSpl *fd      ... 1. derivation nominator b-spline curve
  CurvBSpl *gd      ... 1. derivation denominator b-spline curve
Returncodes:
  0 = OK
 -1 = Memspc too small
 -2 = input error
 -3 = internal error
*/

	int i1, rc;				
	CurvBSpl f;				

  // printf("UT3D_rbspl_deriv1\n");

  // check input
  if (rbspl->deg < 1) {
    printf("UT3D_rbspl_deriv1: curve degree < 1 -> no derivation!\n");
    return -2;
  }

	// -----------------------------
  // g: DENOMINATOR b-spline curve
	// -----------------------------
					
  // init
  g->deg = rbspl->deg;
  g->ptNr = rbspl->ptNr;
  g->v0 = rbspl->v0;	
  g->v1 = rbspl->v1;	
 
  // space for "control points" and knotvector of g
	g->cpTab = memSeg1->next;
  rc = UME_add (memSeg1, sizeof(Point) * g->ptNr);
  if (rc < 0) return -1;
	g->kvTab = memSeg1->next;
  rc = UME_add (memSeg1, sizeof(double) * (g->ptNr+g->deg+1));
  if (rc < 0) return -1;

  // knotvector of g
	for (i1=0; i1<g->ptNr+g->deg+1; ++i1) g->kvTab[i1] = rbspl->kvTab[i1];

  // "control points" of g (1-dimensional)
	for (i1=0; i1<g->ptNr; ++i1) {
		g->cpTab[i1].x = rbspl->wTab[i1];
		g->cpTab[i1].y = 0.0;
		g->cpTab[i1].z = 0.0;
	}	

	// -----------------------------------------------
  // gd: 1. derivation of DENOMINATOR b-spline curve
	// -----------------------------------------------

  rc = UT3D_bspl_deriv1 (gd, memSeg1, g);
  if (rc < 0) return -3;

	// ---------------------------
  // f: NOMINATOR b-spline curve
	// ---------------------------

  // init
  f.deg = rbspl->deg;
  f.ptNr = rbspl->ptNr;
  f.v0 = rbspl->v0;	
  f.v1 = rbspl->v1;	
 
  // space for control points and knotvector of f
	f.cpTab = memSeg1->next;
  rc = UME_add (memSeg1, sizeof(Point) * f.ptNr);
  if (rc < 0) return -1;
	f.kvTab = memSeg1->next;
  rc = UME_add (memSeg1, sizeof(double) * (f.ptNr+f.deg+1));
  if (rc < 0) return -1;

  // knotvector of f
	for (i1=0; i1<f.ptNr+f.deg+1; ++i1) f.kvTab[i1] = rbspl->kvTab[i1];

  // control points of f
	for (i1=0; i1<f.ptNr; ++i1) {
		if (rbspl->wTab[i1] == 0.0) f.cpTab[i1] = rbspl->cpTab[i1];
		else {
		  f.cpTab[i1].x = rbspl->wTab[i1] * rbspl->cpTab[i1].x;
		  f.cpTab[i1].y = rbspl->wTab[i1] * rbspl->cpTab[i1].y;
		  f.cpTab[i1].z = rbspl->wTab[i1] * rbspl->cpTab[i1].z;
		}	
	}	

	// ---------------------------------------------
  // fd: 1. derivation of NOMINATOR b-spline curve
	// ---------------------------------------------

  rc = UT3D_bspl_deriv1 (fd, memSeg1, &f);
  if (rc < 0) return -3;

  return 0;
}


/*=======================================================================*/
  int UT3D_vc_evparCrvRBSpl (Vector *tg, CurvRBSpl *rbspl, CurvBSpl *g,
								             CurvBSpl *fd, CurvBSpl *gd, double t) {
/*=========================
UT3D_vc_evparCrvRBSpl       tangent vector <-- rational b-spline at parameter

UT3D_vc_evparCrvRBSpl       Author: Thomas Backmeister       26.9.2004

Computation of the tangent vector at the given parameter value of a
rational b-spline curve.
The function needs workspace of size (curve degree * sizeof(Point)).

IN:
  CurvRBSpl *rbspl  ... rational b-spline curve
  CurvBSpl *g       ... denominator of rational b-spline curve
  CurvBSpl *fd      ... 1. derivation nominator of rational b-spline curve
  CurvBSpl *gd      ... 1. derivation denominator of rational b-spline curve
	double t          ... parameter value
OUT:
  Vector *tg        ... tangent vector (not normalized)
Returncodes:
  0 = OK
 -1 = internal error
*/

	int    rc;
  double d1;
	Point  pg, pfd, pgd, pr;


  printf("UT3D_vc_evparCrvRBSpl %f\n",t);


	// evaluate g at t
  rc = UT3D_pt_evparCrvBSpl (&pg, g, t);
  if (rc < 0) return -1;

	// evaluate fd at t
  rc = UT3D_pt_evparCrvBSpl (&pfd, fd, t);
  if (rc < 0) return -1;

	// evaluate gd at t
  rc = UT3D_pt_evparCrvBSpl (&pgd, gd, t);
  if (rc < 0) return -1;

	// evaluate rbspl at t
  rc = UT3D_pt_evparCrvRBSpl (&pr, &d1, rbspl, t);
  if (rc < 0) return -1;

	// tangent vector
	if (pg.x != 0.0) {
    tg->dx = (pfd.x - pgd.x * pr.x) / pg.x;
    tg->dy = (pfd.y - pgd.x * pr.y) / pg.x;
    tg->dz = (pfd.z - pgd.x * pr.z) / pg.x;
	}
  else {
		printf("UT3D_vc_evparCrvRBSpl: no tangent vector at %f", t);
    return -1;
	}	
	
  UT3D_stru_dump (Typ_VC, tg, "ex UT3D_vc_evparCrvRBSpl:");
	return 0;
}


//================================================================
//================================================================
// Liste_TESTFUNKTIONEN:
//================================================================
//================================================================


#ifdef DEB


//=========================================================
  int UT3D_rbspl_tst_load (CurvRBSpl *rbspl) {
//=========================================================
// UT3D_rbspl_tst_load      load a teststruct

  static double    kvtab[10], wtab[7];
  static Point     cptab[7];


  // define a rational b-spline curve (a circle)
  rbspl->ptNr = 7;
  rbspl->deg = 2;
  rbspl->v0 = 0.0;
  rbspl->v1 = 1.0;

  kvtab[0] = 0.0;
  kvtab[1] = 0.0;
  kvtab[2] = 0.0;
  kvtab[3] = 0.25;
  kvtab[4] = 0.5;
  kvtab[5] = 0.5;
  kvtab[6] = 0.75;
  kvtab[7] = 1.0;
  kvtab[8] = 1.0;
  kvtab[9] = 1.0;
  rbspl->kvTab = kvtab;

  cptab[0].x =  1; cptab[0].y =  0; cptab[0].z = 0;
  cptab[1].x =  1; cptab[1].y =  1; cptab[1].z = 0;
  cptab[2].x = -1; cptab[2].y =  1; cptab[2].z = 0;
  cptab[3].x = -1; cptab[3].y =  0; cptab[3].z = 0;
  cptab[4].x = -1; cptab[4].y = -1; cptab[4].z = 0;
  cptab[5].x =  1; cptab[5].y = -1; cptab[5].z = 0;
  cptab[6].x =  1; cptab[6].y =  0; cptab[6].z = 0;
  rbspl->cpTab = cptab;

  wtab[0] = 1.0;
  wtab[1] = 0.5;
  wtab[2] = 0.5;
  wtab[3] = 1.0;
  wtab[4] = 0.5;
  wtab[5] = 0.5;
  wtab[6] = 1.0;
  rbspl->wTab = wtab;

  // print data of rational b-spline curve
  // UT3D_rbspl (&rbspl);
  // UT3D_stru_dump (Typ_CVRBSP, rbspl, "rbspl:");


  return 0;
}



//================================================================
  int UT3D_rbspl_tst_1 () {
//================================================================

  int       ptNr, ptMax;
  CurvRBSpl rbspl;
  ObjGX     ox1;
  Memspc    workSeg;
  Point     *pTab;


  // load a teststruct
  UT3D_rbspl_tst_load (&rbspl);


  // test tangents ..
  // UME_init (&workSeg, memspc101, sizeof(memspc101));
  // UT3D_rbspl_tst_tg (&rbspl, &workSeg);
  // return 0;



  // make a polygon of Rat.B-SplCurve
  // pTab = (Point*) memspc101;
  // ptMax = sizeof(memspc101) / sizeof(Point);
  // UT3D_cv_rbsp (&ptNr, pTab, &rbspl, ptMax, UT_DISP_cv);   // 0.05 tol
  // GR_Disp_cv (pTab, ptNr, 0);
  // return 0;


  //================================================================
  // RBSP abspeichern
  // make R-Bspl-object
  // ox1 = UTO_stru_2_obj (Typ_CVRBSP, Typ_CVRBSP, &rbspl);
  OGX_SET_OBJ (&ox1, Typ_CVRBSP, Typ_CVRBSP, 1, &rbspl);

  // reset Startindizes
  AP_obj_2_txt (NULL, 0L, NULL, 0L);
  // den Buffer zuruecksetzen.
  UTF_clear1 ();
  // R-Bspl-object --> Text
  AP_obj_2_txt (mem_cbuf1, mem_cbuf1_SIZ, &ox1, -1L);
  // UTF_FilBuf1 an die Hauptdatei (UTF_FilBuf0, nur memory!) anfuegen
  UTF_insert1 (-1L);


  //================================================================



  return 0;
}


//================================================================
  int UT3D_rbspl_tst_cv (CurvRBSpl *rbspl) {
//================================================================
// UT3D_rbspl_tst_cv     display Curve

  int   ptNr;
  Point *pTab;


  // disp. Rat.B-Spl as fine polygon ..
  pTab = (Point*)memspc55;
  ptNr = sizeof(memspc55) / sizeof(Point);
  UT3D_cv_rbsp (&ptNr, pTab, NULL, rbspl, ptNr, 0.1);
  GR_Disp_cv (pTab, ptNr, 9);


  return 0;

}


//================================================================
  int UT3D_rbspl_tst_cc (CurvRBSpl *rbspl) {
//================================================================
// UT3D_rbspl_tst_cc     display ControlPoints/Curve

  int   i1;
  Point *pa;

  // disp points
  pa = rbspl->cpTab;
  for(i1=0; i1<rbspl->ptNr; ++i1) GR_Disp_pt (&pa[i1], SYM_STAR_S, 1);


  // disp controlcurve
  GR_Disp_cv (pa, rbspl->ptNr, 9);

  return 0;

}


//================================================================
  int UT3D_rbspl_tst_tg (CurvRBSpl *rbspl, Memspc *workSeg) {
//================================================================
// UT3D_rbspl_tst_tg    Rational b-spline curve segments

// UT3D_rbspl_tst_tg    Author: Thomas Backmeister       25.9.2003

// IN:
//   CurvRBSpl *rbspl  ... rational b-spline curve
//   Memspc *workSeg   ... work space
// OUT:
// Returncodes:
//   0 = OK
//  -1 = internal error


  int rc, p;
  long nbseg;
  double va, vb, t, u, d1;
  Point pt;
	Vector tg;
	Line ln;
	CurvBSpl g, fd, gd;
	void *memstart;

  printf("\nUT3D_rbspl_tst_tg\n");

  nbseg = rbspl->ptNr - rbspl->deg;
  printf("number of segments= %ld\n", nbseg);

	// "1. derivation" of rational b-spline curve
	memstart = workSeg->next;
  rc = UT3D_rbspl_deriv1 (&g, &fd, &gd, workSeg, rbspl);
  if (rc < 0) return -1;

  for (p=0; p<nbseg; ++p) {

    // segment start- and endparameter
    va = rbspl->kvTab[rbspl->deg+p];
    vb = rbspl->kvTab[rbspl->deg+p+1];
    if (rbspl->v0 >= vb) continue;
    if (rbspl->v1 <= va) break;
    if (rbspl->v0 > va) va = rbspl->v0;
    if (rbspl->v1 < vb) vb = rbspl->v1;
		if (va == vb) continue;

    // ---- TEST rational b-spline segment ----
    printf("\nRational B-SPLINE segment %d\n", p);
    for (t=0.0; t<=1.0; t+=0.2) {
      u = (1.0-t) * va + t * vb;

			// curve point
      rc = UT3D_pt_evparCrvRBSpl (&pt, &d1, rbspl, u);
      if (rc < 0) return -1;
      printf("u= %f   pt= %f %f %f\n",u,pt.x,pt.y,pt.z);
      // cre_obj (Typ_PT, Typ_PT, 1, (void*)&pt);
      GR_Disp_pt (&pt, SYM_STAR_S, 1);

			// curve tangent
      rc = UT3D_vc_evparCrvRBSpl (&tg, rbspl, &g, &fd, &gd, u);
      if (rc < 0) return -1;
      printf("u= %f   tg= %f %f %f\n",u,tg.dx,tg.dy,tg.dz);
      // ln.p1 = pt;
		  // ln.p2.x = pt.x + tg.dx;
		  // ln.p2.y = pt.y + tg.dy;
		  // ln.p2.z = pt.z + tg.dz;
			// cre_obj (Typ_LN, Typ_LN, 1, (void*)&ln);
      GR_Disp_vc (&tg, &pt, 9, 0);  // 0=normiert
    }
  }

	// release workspace
	workSeg->next = memstart;

  return 0;
}

#endif

/*=============================== eof ===================================*/
