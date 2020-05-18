// ut_bez.c              Maintainer: Thomas Backmeister       02.6.2003
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
  ..

-----------------------------------------------------
Modifications:
2003-06-30 (bcv->ptNr >= UT_BEZDEG_MAX) -> (bcv->ptNr > UT_BEZDEG_MAX + 1) TB.
2003-06-02 Neu.  TB/RF.

-----------------------------------------------------
*/
/*!
\file  ../ut/ut_bez.c
\brief Bezier-Curve - Utilityfunctions
\code
=====================================================
List_functions_start:

UT3D_pt_projptbez     project point onto Bezier curve
UT3D_pt_int2bcv           intersect 2 Bezier curves
UT3D_pt_intlnbcv          intersect line with Bezier curve
UT3D_pt_intplbez      Intersect Plane / BezierCurve
UT3D_pta_bez          make Polygon from Bezier-Curve

UT3D_bez_bspl__       Bezier curves <-- b-spline curve
UT3D_bez_bsplseg      Bezier curve <-- b-spline curve segment
UT3D_bez_convcps_r1   convert control points by rule 1
UT3D_bez_convcps_r2   convert control points by rule 2
UT3D_bez_subdiv       subdivide Bezier curve  ("zerschneiden")
UT3D_bez_eval_Pt      evaluate a Bezier curve point
UT3D_bez_trabezm3     transformation of a Bezier curve
UT3D_bez_deriv1       1. derivation Bezier curve <-- Bezier curve

UT3D_pol_bez          x,y,z - polynomials <-- Bezier curve
UT2D_bez_pol          2D-Bezier curve <-- polynomial

UT2D_zeros_bez        zeros of a 2D-Bezier curve (Nulldurchgaenge)
UT2D_bez_subdiv       subdivide 2D-Bezier curve

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


Compile:
cc -c ut_bez.c


Man kann rbez und rbspl nicht verlustfrei in bez/bspl umwandeln.

Offline testen:
cc ut_bez.c -DOFFLINE&&a.out
==============================================================
*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




// #include "../ut/ut_umem.h"          // UME_save
#include "../ut/ut_geo.h"
#include "../ut/ut_math.h"          // UTM_db_binquot
#include "../ut/func_types.h"                  // SYM_SQUARE ..







// double UT_TOL_ln     = 0.001; // Toleranz fuer die Behandlung einer Menge
                              // von Punkten als Gerade







/*=======================================================================*/
  int UT3D_pt_int2bcv (int *nxp, Point *xptab, int ptxMax,
                       CurvBez *bcv1, CurvBez *bcv2,
                       Memspc *memSeg1, double tol)        {
/*===================
UT3D_pt_int2bcv      intersect 2 Bezier curves

UT3D_pt_int2bcv      Author: Thomas Backmeister       20.5.2003

Intersection of two Bezier curves.
The function works recursively!

IN:
  int ptxMax    ... max Anzahl von Ausgabepunkten
  CurvBez *bcv1 ... Bezier curve 1
  CurvBez *bcv2 ... Bezier curve 2
  double tol    ... tolerance for considering Bezier curve as line
                    (as smaller tol as straighter the curve)
  Memspc *memSeg1 . fuer temporaere Elemente erforderlich
OUT:
  int *nxp      ... number of intersection points
  Point *xptab  ... intersection points
Returncodes:
  0 = OK
 -1 = out of mem (Memspc too small)
 -2 = out of mem (xptab too small)
*/

  int     rc;
  double  dist;
  Point   pmin1, pmax1, pmin2, pmax2, xp1, xp2;
  Line    l1, l2;
  CurvBez bcv3, bcv4;

  // static int it1=0;   // TEST ONLY



  // printf("============================================== \n");
  // ++it1;
  // printf("UT3D_pt_int2bcv: %d %f %d\n",ptxMax,tol,it1);


  // bounding box of Bezier curve 1
  UT3D_box_pts (&pmin1, &pmax1, bcv1->ptNr, bcv1->cptab);

  // bounding box of Bezier curve 2
  UT3D_box_pts (&pmin2, &pmax2, bcv2->ptNr, bcv2->cptab);


  // bounding boxes do not intersect -> return
  if (pmin2.x > (pmax1.x + UT_TOL_min1) ||
      pmin2.y > (pmax1.y + UT_TOL_min1) ||
      pmin2.z > (pmax1.z + UT_TOL_min1) ||
      pmin1.x > (pmax2.x + UT_TOL_min1) ||
      pmin1.y > (pmax2.y + UT_TOL_min1) ||
      pmin1.z > (pmax2.z + UT_TOL_min1))
    return 0;


  // --- Bezier curve 1 is no line ---
/*
  printf(" bez-test-%d\n",it1);
    if(it1 == 16) {
      DEB_dump_obj__(Typ_PT, &pmin1, " &pmin1=");
      DEB_dump_obj__(Typ_PT, &pmax1, " &pmax1=");
      DEB_dump_obj__(Typ_PT, &pmin2, " &pmin2=");
      DEB_dump_obj__(Typ_PT, &pmax2, " &pmax2=");
      GR_tDyn_box__(&pmin1, &pmax1, 2);
      GR_tDyn_box__(&pmin2, &pmax2, 5);
      GR_tDyn_txiA (&pmin1, it1, 2);
      GR_tDyn_txiA (&pmax1, it1, 2);
      GR_tDyn_txiA (&pmin2, it1, 5);
      GR_tDyn_txiA (&pmax2, it1, 5);
    }
*/



  if (!UT3D_pt_ck_linear (bcv1->ptNr, bcv1->cptab, tol)) {

    // subdivide Bezier curve 1
    rc = UT3D_bez_subdiv (&bcv3, &bcv4, memSeg1, bcv1, 0.5);
    if(rc < 0) return rc;

    // repeat this function with bcv3 and bcv4
    rc = UT3D_pt_int2bcv (nxp, xptab, ptxMax, &bcv3, bcv2, memSeg1, tol);
    if(rc < 0) return rc;

    rc = UT3D_pt_int2bcv (nxp, xptab, ptxMax, &bcv4, bcv2, memSeg1, tol);
    if(rc < 0) return rc;



  // --- Bezier curve 2 is no line ---
  } else if (!UT3D_pt_ck_linear (bcv2->ptNr, bcv2->cptab, tol)) {

    // subdivide Bezier curve 2
    rc = UT3D_bez_subdiv (&bcv3, &bcv4, memSeg1, bcv2, 0.5);
    if(rc < 0) return rc;

    // repeat this function with bcv3 and bcv4
    rc = UT3D_pt_int2bcv (nxp, xptab, ptxMax, bcv1, &bcv3, memSeg1, tol);
    if(rc < 0) return rc;

    rc = UT3D_pt_int2bcv (nxp, xptab, ptxMax, bcv1, &bcv4, memSeg1, tol);
    if(rc < 0) return rc;



  // --- both Bezier curves are (limited) lines ---
  } else {
      // printf(" bez-test-ln x ln\n");

    // lines from Bezier curves
    l1.p1 = bcv1->cptab[0];
    l1.p2 = bcv1->cptab[bcv1->ptNr - 1];
    l2.p1 = bcv2->cptab[0];
    l2.p2 = bcv2->cptab[bcv2->ptNr - 1];

/*
      // TEST-ONLY
      if(it1 == 16) {
      // DEB_dump_obj__(Typ_LN, &l1, " bez-ln1");
      // DEB_dump_obj__(Typ_LN, &l2, " bez-ln2");
      GR_Disp_ln (&l1, 2);
      GR_Disp_ln (&l2, 5);
      }
      // TEST-ONLY
*/


    // l1 ist bereits ein Punkt: liegt er auf l2 ?
    if(UT3D_len_2pt(&l1.p1, &l1.p2) < UT_TOL_min1) {
      xp1 = l1.p1;
      if(!UT3D_pt_ck_inLine (&l2.p1, &l2.p2, &xp1, UT_TOL_min1)) return 0;
      goto L_found;
    }


    // l2 ist bereits ein Punkt: liegt er auf l1 ?
    if(UT3D_len_2pt(&l2.p1, &l2.p2) < UT_TOL_min1) {
      xp1 = l2.p1;
      if(!UT3D_pt_ck_inLine (&l1.p1, &l1.p2, &xp1, UT_TOL_min1)) return 0;
      goto L_found;
    }


    // intersect lines
    rc = UT3D_pt_int2ln (&xp1, &xp2, &dist, &l1, &l2);
    if (rc < 0) return 0;


    // check if X-point is inside both line segments
    //   (UT_TOL_min1 -> PT)  2011-08-22 RF
    if(!UT3D_pt_ck_inLine (&(l1.p1), &(l1.p2), &xp1, UT_TOL_cv)) return 0;
    if(!UT3D_pt_ck_inLine (&(l2.p1), &(l2.p2), &xp1, UT_TOL_cv)) return 0;


    // check array size for X-points
    L_found:
    if (*nxp >= ptxMax) {
      TX_Error("UT3D_pt_int2bcv E002");
      return -2;
    }

    // save intersection point
     // DEB_dump_obj__(Typ_PT, &xp1, "  save xp1[%d]",*nxp);
    xptab[*nxp] = xp1;
    ++(*nxp);
  }

  return 0;
}


/*=======================================================================*/
  int UT3D_pt_intlnbcv (int *nxp, Point *xptab, int ptxMax,
                        Line *ln, CurvBez *bcv, Memspc *memSeg1, double tol) {
/*====================
UT3D_pt_intlnbcv      intersect line with Bezier curve

UT3D_pt_intlnbcv      Author: Thomas Backmeister       26.5.2003

Intersection of line with Bezier curve.
The function works recursively!

IN:
  int ptxMax    ... max Anzahl von Ausgabepunkten
  Line *ln      ... line
  CurvBez *bcv  ... Bezier curve
  double tol    ... tolerance for considering Bezier curve as line
                    (as smaller tol as straighter the curve)
  int *nxp      ... number of intersection points
  Point *xptab  ... intersection points
  Memspc *memSeg1 . fuer temporaere Elemente erforderlich
OUT:
  int *nxp      ... number of intersection points
  Point *xptab  ... intersection points
Returncodes:
  0 = OK
  1 = array for X-points too small
 -1 = out of mem (Memspc too small)
 -2 = out of mem (xptab too small)
*/

  int     rc, i1;
  double  dist, tol_d;
  Point   pmin, pmax, xp1, *pp1, *pp2;
  CurvBez bcv1, bcv2;



  // printf("UT3D_pt_intlnbcv %d\n",*nxp);
// 

  // make bounding box of Bezier curve
  UT3D_box_pts (&pmin, &pmax, bcv->ptNr, bcv->cptab);
    // GR_tDyn_box__(&pmin, &pmax, 9);


  // if line does not intersect bounding box -> return
  // if (!UT3D_box_ck_intLn (ln, &pmin, &pmax, UT_TOL_min1)) return 0;
  if (!UT3D_box_ck_intLn (ln, &pmin, &pmax, UT_TOL_pt)) return 0;


  // --- Bezier curve is no (not yet) line ---
  if (!UT3D_pt_ck_linear (bcv->ptNr, bcv->cptab, tol)) {

    // subdivide Bezier curve
    rc = UT3D_bez_subdiv (&bcv1, &bcv2, memSeg1, bcv, 0.5);
    if(rc < 0) return rc;

    // repeat this function with bcv1 and bcv2 (recursion !)
    if(UT3D_pt_intlnbcv(nxp,xptab,ptxMax,ln,&bcv1,memSeg1,tol) < 0) return rc;
    if(UT3D_pt_intlnbcv(nxp,xptab,ptxMax,ln,&bcv2,memSeg1,tol) < 0) return rc;





  // --- Bezier curve is a (limited) line ---
  } else {
      // printf(" UT3D_pt_intlnbcv ===== test line\n");

    // line from Bezier curve
    pp1 = &bcv->cptab[0];
    pp2 = &bcv->cptab[bcv->ptNr - 1];

    // bereits ein Punkt: intersectPoint found.
    if(UT3D_len_2pt(pp1, pp2) < UT_TOL_pt) {
      xp1 = *pp1;
      goto L_found;
    }


    // intersect limited lines
    rc = UT3D_pt_int2pt2pt_lim (&xp1, NULL, NULL,
                            &ln->p1, &ln->p2, pp1, pp2, tol);
    if(rc < 0) return 0;
      // GR_Disp_pt (&xp1, SYM_STAR_S, ATT_COL_RED);


    // check distance to last found point (schleifende Schnitte entfernen)
    // see also UT3D_pt_int2rbez
    i1 = *nxp - 1;
    // if(rc > 0) {
      // this point is on 2 parallel lines
      if(i1 >= 0) {
        tol_d = tol * tol * 100.;
        // if(rc > 0) tol_d *= 1000.;
        tol_d *= 1000.;
        dist = UT3D_lenq_PtPt (&xp1, &xptab[i1]);
          // printf(" nxp=%d dist=%lf tol_d=%lf\n",*nxp,dist,tol_d);
        // if(sqrt(dist) < UT_DISP_cv * 10.) {
        if(dist < tol_d) {
          UT3D_pt_mid2pt (&xptab[i1], &xptab[i1], &xp1);
            // printf(" ********** remove schleifenden Schnitt ..\n");
          return 0;
        }
      }
    // }


    // save intersection point
      // printf(" xp[%d]=%f,%f,%f\n",*nxp,xp1.x,xp1.y,xp1.z);
    L_found:
    if (*nxp >= ptxMax) { TX_Error("UT3D_pt_intlnbcv E002"); return -2; }
    xptab[*nxp] = xp1;
    ++(*nxp);
  }


    // printf("ex _intlnbcv nxp=%d\n",*nxp);


  return 0;
}


//===================================================================
  int UT3D_pt_intplbez (int *pNr, Point *pa,
                     Plane *pln, CurvBez *bcv, Memspc *memSeg1, double tol) {
//===================================================================
// UT3D_pt_intplbez               Intersect Plane / BezierCurve
// ACHTUNG: REENTRANT!

// Input:
// pNr = max.Nr.inters.points/parameters

// Output:
// pNr = Nr.inters.points/parameters

// see UT3D_pt_intlnbcv          intersect line with Bezier curve


  int     irc, pSiz, p1Nr;
  double  bDiag;
  Point   pmin, pmax;
  Vector  vc1;
  CurvBez bcv1, bcv2;


  // printf("UT3D_pt_intplbez pNr=%d tol=%f\n",*pNr,tol);
  // DEB_dump_obj__ (Typ_PLN,   pln, "");
  // DEB_dump_obj__ (Typ_CVBEZ, bcv, "");



  pSiz  = *pNr;
  *pNr  = 0;


  // make boundingBox of Bezier curve
  UT3D_box_pts (&pmin, &pmax, bcv->ptNr, bcv->cptab);
  // pmax.z = 100.; // NUR TEST
  // GR_tDyn_box__(&pmin, &pmax, SYM_TRI_S, 0); // disp boundingBox


  // if plane does not intersect bounding box -> return
  if(UT3D_box_ck_intpl (pln, &pmin, &pmax, UT_TOL_min1) == 0) return 0;


  // if Curve not yet linear: Curve halbieren; else test
  if(UT3D_pt_ck_linear (bcv->ptNr, bcv->cptab, tol) == 0) {


    // subdivide Bezier curve
    irc = UT3D_bez_subdiv (&bcv1, &bcv2, memSeg1, bcv, 0.5);
    // GR_tDyn_bez (&bcv1, 2, memSeg1);
    // GR_tDyn_bez (&bcv2, 3, memSeg1);
    if(irc < 0) return irc;

    // test both new Curves
    p1Nr = pSiz - *pNr;
    irc = UT3D_pt_intplbez(&p1Nr,&pa[*pNr],pln,&bcv1,memSeg1,tol);
    *pNr += p1Nr;
    if(irc < 0) return irc;

    p1Nr = pSiz - *pNr;
    irc = UT3D_pt_intplbez(&p1Nr,&pa[*pNr],pln,&bcv2,memSeg1,tol);
    *pNr += p1Nr;
    if(irc < 0) return irc;



  // Curve ist nun linear
  } else {

    // GR_tDyn_bez (bcv, 2, memSeg1);
    if(*pNr >= pSiz) {TX_Error("UT3D_pt_intplbez E002"); return -2; }

    // intersect line / plane
    UT3D_vc_2pt (&vc1, &bcv->cptab[0], &bcv->cptab[bcv->ptNr - 1]);
    irc = UT3D_pt_intptvcpl_ (&pa[*pNr], pln, &bcv->cptab[0], &vc1);
    if(irc == 1) ++(*pNr);
    // GR_Disp_pt (&pa[*pNr-1], SYM_TRI_S, 0);
    // DEB_dump_obj__ (Typ_PT, &pa[*pNr-1], " intplbez P[%d]=",*pNr-1);

  }


  return 0;

}


//===================================================================
  int UT3D_pta_bez (int *ptNr, Point *pTab, CurvBez *bez,
                    int ptMax, double tol) {
//===================================================================
// UT3D_pta_bez             make Polygon from Bezier-Curve
// see bspl_pol_bsp

  // irc = UT3D_bez_eval_Pt (&pt1, bez, 0.5);
  // GR_Disp_pt (&pt1, SYM_STAR_S, 0);

  int     ianz, irc, ptOut, ptStackNr;
  double  pVal, kvs, kve, us, ue, dist, uStack[20];
  Point   pt1, pt2, pts, pte, ptStack[20];


  // DEB_dump_obj__ (Typ_CVBEZ, bez, "UT3D_pta_bez ptMax=%d\n",ptMax);


  ianz  = bez->ptNr - 1;
  kvs   = 0.;
  kve   = 1.;

    // printf("  kvs=%f kve=%f ianz=%d\n",kvs,kve,ianz);
    // return 0;


  // den ersten Pt sofort raus
  ptOut = 0;
  irc = UT3D_bez_eval_Pt (&pTab[ptOut], bez, kvs);
    // DEB_dump_obj__ (Typ_PT, &pTab[ptOut], "P[%d]=",ptOut);
  ++ptOut;          // der aktuelle Punkt ist immer pTab[ptOut-1].


  // nur 2 Punkte -
  if(ianz == 1) {
    pTab[ptOut] = bez->cptab[1]; // copy Point
    ++ptOut;
    goto L_fertig;
  }


  // den letzen Punkt auf den Stack.
  ptStackNr = 0;
  uStack[ptStackNr] = kve;
  irc = UT3D_bez_eval_Pt (&ptStack[ptStackNr], bez, kve);
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
  irc = UT3D_bez_eval_Pt (&pt1, bez, pVal);
    // GR_Disp_pt (&pt1, SYM_TRI_S, 0);
    // printf(" irc=%d pt1=%f %f %f\n",irc,pt1.x,pt1.y,pt1.z);
  if(irc < 0) return irc;


  // die Abweichung feststellen
  UT3D_pt_projpt2pt (&pt2, &dist, &pt1, &pts, &pte);
  // printf(" dist=%f\n",dist);
  // if(ptStackNr > 1) {dist = 0.001;} else {dist = 10.0;} // TEST


  // if(dist > tol) {
  // mind. 3 Punkte; sonst geht eine S-Form nicht !!
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
  if(ptOut >= ptMax) {
    TX_Error("UT3D_pta_bez E001");
    return -1;
  }
  ++ptOut;
  --ptStackNr;
  pts = pte;
  us = ue;
  if(ptStackNr > 0) goto L_next_inPoint;


  //-------------------------------------------
  L_fertig:

  *ptNr = ptOut;



  //------- Testausg------
  // GR_tDyn_npt__ (*ptNr, pTab, SYM_STAR_S, 2);
  // printf("ex UT3D_pta_bez ptNr=%d\n",ptOut);
  // for(ianz=0;ianz<ptOut;++ianz)
    // DEB_dump_obj__ (Typ_PT, &pTab[ianz], "P[%d]=",ianz);
  //------- Testausg------



  return 0;

}


/*=======================================================================*/
  int UT3D_bez_bsplseg (CurvBez *bcv,
                        double segs,   double sege, int numkn,
                        double *kvtab, int numcp,   Point *cptab) {
/*====================
UT3D_bez_bsplseg      Bezier curve <-- b-spline curve segment

UT3D_bez_bsplseg      Author: Thomas Backmeister         14.5.2003

Computation of a Bezier curve from a b-spline curve segment.

The function works recursively!
Before calling it, the output variable bcv must be initialized (bcv->deg = 0)!

IN:
  double segs   ... start parameter of b-spline curve segment
  double sege   ... end parameter of b-spline curve segment
  int numkn     ... number of knots for b-spline curve segment
                    (= degree of b-spline curve * 2)
  double *kvtab ... knot vector for b-spline curve segment
  int numcp     ... number of control points
                    (= degree of b-spline curve + 1)
  Point *cptab  ... control points of b-spline curve segment
OUT:
  CurvBez *bcv  ... Bezier curve
Returncodes:
  0 = OK
  1 = spline curve segment with zero length
 -1 = input error
 -2 = too many control points
*/

  int   i1, num1, num2;
  Point ptab1[UT_BEZDEG_MAX+1], ptab2[UT_BEZDEG_MAX+1];



  // printf("UT3D_bez_bsplseg: ptNr=%d numcp=%d\n",bcv->ptNr,numcp);


  // check input
  if (UTP_comp2db (segs, sege, UT_TOL_min1)) {
    // printf("UT3D_bez_bsplseg: spline curve segment with zero length\n");
    return 1;
  }

  if (numcp < 1) {
    printf("UT3D_bez_bsplseg: no control points\n");
    return -1;
  }

  if (numcp > UT_BEZDEG_MAX + 1) {
    printf("UT3D_bez_bsplseg: too many control points\n");
    return -2;
  }


  if (numcp == 1) {
    // save Bezier control point
    // printf(" init bezpt[%d]=%f,%f,%f\n",bcv->ptNr,
            // cptab[0].x,cptab[0].y,cptab[0].z);
    bcv->cptab[bcv->ptNr] = cptab[0];
    ++(bcv->ptNr);
    return 0;
  }

  // copy control points
  for (i1=0; i1<numcp; ++i1) {
    // printf(" copy %d = %f,%f,%f\n",i1,cptab[i1].x,cptab[i1].y,cptab[i1].z);
    ptab1[i1] = cptab[i1];
  }
  num1 = numcp;

  // convert control points by rule 1
  UT3D_bez_convcps_r1 (&num1, ptab1, segs, numkn, kvtab);

  // convert control points by rule 2
  UT3D_bez_convcps_r2 (ptab2, cptab, num1, ptab1, segs, sege,
                       numkn, kvtab);
  num2 = num1;

  // convert control points by rule 1 until 1 point is left
  for (i1=1; i1<numcp-1; ++i1) {
    UT3D_bez_convcps_r1 (&num1, ptab1, segs, numkn, kvtab);
  }

  // save Bezier control point
  // printf(" save bezpt[%d]=%f,%f,%f\n",bcv->ptNr,
           // ptab1[0].x,ptab1[0].y,ptab1[0].z);
  bcv->cptab[bcv->ptNr] = ptab1[0];
  ++(bcv->ptNr);

  // repeat this function
  UT3D_bez_bsplseg (bcv, segs, sege, numkn, kvtab, num2, ptab2);

  // printf("ex UT3D_bez_bsplseg: ptNr=%d numcp=%d\n",bcv->ptNr,numcp);
  return 0;

}


/*=======================================================================*/
  int UT3D_bez_convcps_r1 (int *num, Point *ptab, double segs, int numkn,
                           double *kvtab) {
/*=======================
UT3D_bez_convcps_r1     convert control points by rule 1

UT3D_bez_convcps_r1     Author: Thomas Backmeister       16.5.2003

Conversion of curve control points by rule 1 of some tetrahedral
algorithm (e.g. see H. Prautzsch: Bezier and B-Spline Techniques,
Springer 2002, p.72)

Zero divisions cannot occur, if the function has correct input!
More precise: the differences of knotvalues are > 0, if the underlying
spline curve segment is welldefined.

IN:
  int *num      ... number of control points
  Point *ptab   ... control points
  double segs   ... start parameter of spline curve segment
  int numkn     ... number of knots for spline curve segment
                    (= degree of spline curve * 2)
  double *kvtab ... knot vector for spline curve segment
OUT:
  int *num      ... number of new control points (1 less than before)
  Point *ptab   ... new control points
Returncodes:
  0 = OK
 -1 = input error
*/

  int i1, indi, indj;
  double a, b;

  //printf("UT3D_bez_convcps_r1: convert control points by rule 1\n");

  // indi, indj
  indj = numkn / 2;
  indi = indj + 1 - (*num);
  if (indi < 0 || (indi + (*num) - 2) >= numkn) {
    printf("UT3D_bez_convcps_r1: input error 1\n");
    return -1;
  }
  if (indj < 0 || (indj + (*num) - 2) >= numkn) {
    printf("UT3D_bez_convcps_r1: input error 2\n");
    return -1;
  }

  // new control points
  --(*num);
  for (i1=0; i1<*num; ++i1) {
    a = (segs - kvtab[indi]) / (kvtab[indj] - kvtab[indi]);
    b = 1.0 - a;
    ptab[i1].x = b * ptab[i1].x + a * ptab[i1+1].x;
    ptab[i1].y = b * ptab[i1].y + a * ptab[i1+1].y;
    ptab[i1].z = b * ptab[i1].z + a * ptab[i1+1].z;
    ++indi;
    ++indj;
  }

  return 0;
}


/*=======================================================================*/
  int UT3D_bez_convcps_r2 (Point *ptab, Point *cptab, int num1, Point *ptab1,
                           double segs, double sege, int numkn, double *kvtab) {
/*=======================
UT3D_bez_convcps_r2     convert control points by rule 2

UT3D_bez_convcps_r2     Author: Thomas Backmeister       16.5.2003

Conversion of curve control points by rule 2 of some tetrahedral
algorithm (e.g. see H. Prautzsch: Bezier and B-Spline Techniques,
Springer 2002, p.72)

Zero divisions cannot occur, if the function has correct input!
More precise: the differences of knotvalues are > 0, if the underlying
spline curve segment is welldefined.

IN:
  Point *cptab  ... control points (number = num1 + 1)
  int num1      ... number of control points after conversion by rule 1
  Point *ptab1  ... control points after conversion by rule 1
  double segs   ... start parameter of spline curve segment
  double sege   ... end parameter of spline curve segment
  int numkn     ... number of knots for spline curve segment
                    (= degree of spline curve * 2)
  double *kvtab ... knot vector for spline curve segment
OUT:
  Point *ptab   ... new control points (number = num1)
Returncodes:
  0 = OK
 -1 = input error
*/

  int    i1, indj;
  double a, b;



/*
  // printf("UT3D_bez_convcps_r2: num1=%d s=%f e=%f\n",num1,segs,sege);
  // nur Testaug
  for (i1=0; i1<num1; ++i1) {
    printf(" in  [%d] %f,%f,%f\n",i1,cptab[i1].x,cptab[i1].y,cptab[i1].z);
  }
*/


  // indj
  indj = (numkn / 2);
  if (indj < 0 || (indj + num1 - 1) >= numkn) {
    printf("UT3D_bez_convcps_r2: input error\n");
    return -1;
  }

  // new control points
  for (i1=0; i1<num1; ++i1) {
    a = (sege - segs) / (kvtab[indj] - segs);
    b = 1.0 - a;
    ptab[i1].x = a * cptab[i1+1].x + b * ptab1[i1].x;
    ptab[i1].y = a * cptab[i1+1].y + b * ptab1[i1].y;
    ptab[i1].z = a * cptab[i1+1].z + b * ptab1[i1].z;
    ++indj;
    // printf(" out [%d] %f,%f,%f\n",i1,ptab[i1].x,ptab[i1].y,ptab[i1].z);
  }


  return 0;
}



/*=======================================================================*/
  int UT3D_bez_subdiv (CurvBez *bcv1, CurvBez *bcv2, Memspc *memSeg1,
                       CurvBez *bcv, double t) {
/*===================
UT3D_bez_subdiv     subdivide Bezier curve  ("zerschneiden")

UT3D_bez_subdiv     Author: Thomas Backmeister       20.5.2003

Subdivide a Bezier curve into 2 Bezier curves.
The composition of the two resulting curves is equal to the original curve.
The underlying algorithm is "de Casteljau".

IN:
  CurvBez *bcv  ... Bezier curve
  double t      ... parameter of subdivision point on the curve
                    (from interval (0,1))
OUT:
  CurvBez *bcv1 ... Bezier curve 1
  CurvBez *bcv2 ... Bezier curve 2
Returncodes:
  0 = OK
  1 = Bezier curve degree too large
 -1 = input error
*/

  int    irc, i1, i2, numpt;
  double s;
  Point  ptab[UT_BEZDEG_MAX+1];



  // printf("UT3D_bez_subdiv: t=%f\n",t);



  if (t <= 0.0 || t >= 1.0) {
    printf("UT3D_bez_subdiv: input error\n");
    return -1;
  }
  if (bcv->ptNr >= UT_BEZDEG_MAX + 1) {
    printf("UT3D_bez_subdiv: degree of Bezier curve too large\n");
    return 1;
  }


  // init
  bcv1->ptNr = bcv->ptNr;
  bcv2->ptNr = bcv->ptNr;


  // die Startadressen der Punktetabellen -> memSeg1.
  // bcv1 und bcv2 haben ebenfalls ptNr Punkte !
  bcv1->cptab = memSeg1->next;
  // bcv2->cptab = memSeg1->next + (sizeof(Point) * bcv->ptNr);
  bcv2->cptab = (Point*)((char*)memSeg1->next + (sizeof(Point) * bcv->ptNr));


  // den Speicher reservieren (manuell speichen)
  irc = UME_add (memSeg1, sizeof(Point) * bcv->ptNr * 2);
  if(irc < 0) {
    TX_Error("UT3D_bez_subdiv: out of tempSpace");
    return -1;
  }


  // save 1. / last point
  bcv1->cptab[0] = bcv->cptab[0];
  bcv2->cptab[bcv->ptNr - 1] = bcv->cptab[bcv->ptNr - 1];


  // copy Bezier control points
  for (i1=0; i1<bcv->ptNr; ++i1) {
    ptab[i1] = bcv->cptab[i1];
  }


  // subdivision
  s = 1.0 - t;
  numpt = bcv->ptNr - 1;
  for (i1=0; i1<bcv->ptNr - 1; ++i1) {
    for (i2=0; i2<numpt; ++i2) {
      ptab[i2].x = s * ptab[i2].x + t * ptab[i2+1].x;
      ptab[i2].y = s * ptab[i2].y + t * ptab[i2+1].y;
      ptab[i2].z = s * ptab[i2].z + t * ptab[i2+1].z;
    }

    // die neuen Punkte speichern
    bcv1->cptab[i1+1] = ptab[0];
    bcv2->cptab[bcv->ptNr-i1-2] = ptab[numpt-1];

    --numpt;
  }



/*
  // nur Testausgabe der Punkte
  for(i1=0; i1<bcv->ptNr; ++i1) {
    // drawCircF (&bcv->cptab[i1], 0.1);
    printf(" bcv[%d]=%9.3f,%9.3f\n",i1,
             bcv->cptab[i1].x,bcv->cptab[i1].y,bcv->cptab[i1].z);
  }
  for(i1=0; i1<bcv->ptNr; ++i1) {
    // drawCircF (&bcv1->cptab[i1], 0.1);
    printf(" bcv1[%d]=%9.3f,%9.3f\n",i1,
             bcv1->cptab[i1].x,bcv1->cptab[i1].y,bcv1->cptab[i1].z);
  }
  for(i1=0; i1<bcv->ptNr; ++i1) {
    // drawCircF (&bcv2->cptab[i1], 0.2);
    printf(" bcv2[%d]=%9.3f,%9.3f\n",i1,
             bcv2->cptab[i1].x,bcv2->cptab[i1].y,bcv2->cptab[i1].z);
  }
  // nur Testausgabe der Punkte
*/



  return 0;
}



/*=======================================================================*/
  int UT3D_bez_eval_Pt (Point *pt, CurvBez *bcv, double t) {
/*====================
UT3D_bez_eval_Pt       evaluate a Bezier curve point

UT3D_bez_eval_Pt       Author: Thomas Backmeister       19.5.2003

Evaluate a point on a Bezier curve.
The underlying algorithm is "de Casteljau".

IN:
  CurvBez *bcv  ... Bezier curve
  double t      ... parameter of curve point (from interval [0,1])
OUT:
  Point *pt     ... curve point
Returncodes:
  0 = OK
  1 = Bezier curve degree too large
 -1 = input error
*/

  int    i1, i2, numpt;
  double s;
  Point  ptab[UT_BEZDEG_MAX+1];


  // printf("UT3D_bez_eval_Pt: t=%.12f\n",t);


  // RF: Problem bei 1.00000000001 (aus vorheriger Berechnung)
  if(t < 0.) {
     t = 0.;
  } else if ( t > 1.) {
     t = 1.;
  }


  if (bcv->ptNr >= UT_BEZDEG_MAX + 1) {
    printf("UT3D_bez_eval_Pt: degree of Bezier curve too large\n");
    return 1;
  }


  // copy Bezier control points
  for (i1=0; i1<bcv->ptNr; ++i1) {
    ptab[i1] = bcv->cptab[i1];
    // printf(" p[%d] = %f,%f,%f\n",i1,ptab[i1].x,ptab[i1].y,ptab[i1].z);
  }

  // compute the curve point
  s = 1.0 - t;
  numpt = bcv->ptNr - 1;
  for (i1=0; i1<(bcv->ptNr - 1); ++i1) {
    for (i2=0; i2<numpt; ++i2) {
      ptab[i2].x = s * ptab[i2].x + t * ptab[i2+1].x;
      ptab[i2].y = s * ptab[i2].y + t * ptab[i2+1].y;
      ptab[i2].z = s * ptab[i2].z + t * ptab[i2+1].z;
    }
    --numpt;
  }
  *pt = ptab[0];


  // printf("ex UT3D_bez_eval_Pt t=%f p=%f,%f,%f\n",t,pt->x,pt->y,pt->z);


  return 0;
}



/*=======================================================================*/
  int UT3D_bez_trabezm3 (CurvBez *bcvt, Memspc *memSeg1,
                         CurvBez *bcv, Mat_4x3 mata) {
/*=====================
UT3D_bez_trabezm3     transformation of a Bezier curve

UT3D_bez_trabezm3     Author: Thomas Backmeister       2.6.2003

Computation of the (affine) transformation of a Bezier curve.
This is again a Bezier curve of same degree as the original curve.

IN:
  CurvBez *bcv  ... Bezier curve
  Mat_4x3 mata  ... transformation matrix
OUT:
  CurvBez *bcvt ... transformed Bezier curve
Returncode:
  0 = OK
 -1 = Memspc too small
*/

  int i1, rc;


  // printf("UT3D_bez_trabezm3\n");


  // degree of transformed curve
  bcvt->ptNr = bcv->ptNr;

  // bcvt->va   = bcv->va; // zu RF 2004-10-30
  // bcvt->vb   = bcv->vb;


  // platz fuer bcvt->ptNr Punkte reservieren
  bcvt->cptab = memSeg1->next;
  rc = UME_add (memSeg1, sizeof(Point) * bcv->ptNr);
  if(rc < 0) return -1;


  // control points of transformed curve
  for (i1=0; i1<=bcv->ptNr-1; ++i1) {
    UT3D_pt_tra_pt_m3 (&(bcvt->cptab[i1]), mata, &(bcv->cptab[i1]));
  }


  return 0;
}



/*=======================================================================*/
  int UT3D_bez_deriv1 (CurvBez *bcvd, Memspc *memSeg1, CurvBez *bcv) {
/*===================
UT3D_bez_deriv1       1. derivation Bezier curve <-- Bezier curve

UT3D_bez_deriv1       Author: Thomas Backmeister       2.6.2003

Computation of the 1. derivation of a Bezier curve.
This is again a Bezier curve of degree 1 less than degree of original curve.

IN:
  CurvBez *bcv  ... Bezier curve
OUT:
  CurvBez *bcvd ... 1. derivation Bezier curve
Returncodes:
  0 = OK
 -1 = Memspc too small
 -2 = input error
*/

  int    i1, deg, rc;

  // printf("UT3D_bez_deriv1\n");

  // check input
  if (bcv->ptNr < 2) {
    printf("UT3D_bez_deriv1: curve degree < 1 -> no derivation!\n");
    return -2;
  }

  // degree of 1. derivation curve
  deg = bcv->ptNr-1;
  bcvd->ptNr = deg;


  // platz fuer bcvt->ptNr Punkte reservieren
  bcvd->cptab = memSeg1->next;
  rc = UME_add (memSeg1, sizeof(Point) * deg);
  if(rc < 0) return -1;


  // control points of 1. derivation curve
  for (i1=0; i1<deg; ++i1) {
    bcvd->cptab[i1].x = deg * (bcv->cptab[i1+1].x - bcv->cptab[i1].x);
    bcvd->cptab[i1].y = deg * (bcv->cptab[i1+1].y - bcv->cptab[i1].y);
    bcvd->cptab[i1].z = deg * (bcv->cptab[i1+1].z - bcv->cptab[i1].z);
  }

  return 0;
}



/*=======================================================================*/
  int UT3D_pol_bez (Polynom3 *polbcv, Memspc *memSeg1, CurvBez *bcv) {
/*================
UT3D_pol_bez         x,y,z - polynomials <-- Bezier curve

UT3D_pol_bez         Author: Thomas Backmeister      2.6.2003

Computation of the polynomials for x-, y- and z- coordinate of a
Bezier curve.
The degree of the polynomials is equal degree of Bezier curve.
The parameter range of the polynomials is [0,1].

IN:
  CurvBez *bcv       ... Bezier curve
OUT:
  double polbcv[0][] ... coefficients of x - polynomial
  double polbcv[1][] ... coefficients of y - polynomial
  double polbcv[2][] ... coefficients of z - polynomial
Returncodes:
  0 = OK
 -1 = Memspc too small
 <0 = internal error
*/

  int      rc, i1, j1, deg, lr;
  // double polB[UT_BEZDEG_MAX+1][UT_BEZDEG_MAX+1];
  Polynom_ polB;
  double   polVal;
  void     *memPos1;


  // printf("UT3D_pol_bez deg=%d\n",bcv->ptNr-1);


  polbcv->polNr = bcv->ptNr;

  deg = bcv->ptNr-1;


  // Datenspace fuer Ausgabe reservieren:
  // size einer Polynom / 3
  i1 = sizeof(double) * (deg+1);

  polbcv->pol[0] = memSeg1->next;
  if(UME_add (memSeg1, i1) < 0) return -1;

  polbcv->pol[1] = memSeg1->next;
  if(UME_add (memSeg1, i1) < 0) return -1;

  polbcv->pol[2] = memSeg1->next;
  if(UME_add (memSeg1, i1) < 0) return -1;


  // reicht der restliche Platz im memSeg1 ?
  // brauchen (ptNr * ptNr) doubles
  i1 = bcv->ptNr * bcv->ptNr * sizeof(double);
  if(UME_ck_free (memSeg1) < i1) return -1;


  // Bernstein polynomials of degree bcv->deg
  polB.dim1 = bcv->ptNr;
  polB.dim2 = bcv->ptNr;
  polB.pol  = memSeg1->next;   // ab hier nur tempData (polB).


  rc = UTM_pol_Bernstein (&polB);
  if (rc < 0) return rc;
    // Testausg polB
    // DEB_dump_obj__ (Typ_Polynom_, &polB, "Polynom_ polB =\n");



  // x-, y- and z- polynomial
  lr = bcv->ptNr;               // RecLen
  for (j1=0; j1<=deg; ++j1) {
    polbcv->pol[0][j1] = 0.;
    polbcv->pol[1][j1] = 0.;
    polbcv->pol[2][j1] = 0.;

    for (i1=0; i1<=deg; ++i1) {
      polVal = polB.pol[i1*lr+j1];
      polbcv->pol[0][j1] += bcv->cptab[i1].x * polVal;
      polbcv->pol[1][j1] += bcv->cptab[i1].y * polVal;
      polbcv->pol[2][j1] += bcv->cptab[i1].z * polVal;
    }
  }


  return 0;
}



/*=======================================================================*/
  int UT2D_bez_pol (CurvBez2 *bcv2, Memspc *memSeg1, Polynom1 *pol) {
/*================
UT2D_bez_pol         2D-Bezier curve <-- polynomial

UT2D_bez_pol         Author: Thomas Backmeister      3.6.2003

Representation of a polynomial over [0,1] as 2D-Bezier curve of same degree.

IN:
  int deg         ... degree of polynomial
  double *pol     ... coefficients of polynomial
OUT:
  CurvBez2 *bcv2  ... 2D-Bezier curve
Returncodes:
  0 = OK
 -1 = Memspc too small
 -2 = input error
*/

  int    i1, j1, rc, deg;
  double bq;



  // printf("UT2D_bez_pol deg=%d\n",pol->polNr);

  deg = pol->polNr - 1;


  // check input
  if (deg < 0) {
    printf("UT2D_bez_pol: input error\n");
    return -2;
  }
  if (deg > UT_BEZDEG_MAX) {
    printf("UT2D_bez_pol: degree too large!\n");
    return -2;
  }


  // degree of Bezier curve
  bcv2->ptNr = deg + 1;


  // platz fuer bcvt->ptNr Punkte reservieren
  bcv2->cptab = memSeg1->next;
  rc = UME_add (memSeg1, sizeof(Point2) * bcv2->ptNr);
  if(rc < 0) return -1;


  // control points of Bezier curve
  for (i1=0; i1<=deg; ++i1) {
    bcv2->cptab[i1] = UT2D_PT_NUL;
    for (j1=0; j1<=i1; ++j1) {
      bq = UTM_db_binquot (deg, i1, j1);
      if (j1 == 1) bcv2->cptab[i1].x = bq;
      bcv2->cptab[i1].y += bq * pol->pol[j1];
    }
  }

  return 0;
}



/*=======================================================================*/
  int UT2D_zeros_bez (int *nz, double *ztab, Memspc *memSeg1,
                      CurvBez2 *bcv2, double tol) {
/*==================
UT2D_zeros_bez        zeros of a 2D-Bezier curve

UT2D_zeros_bez        Author: Thomas Backmeister       3.6.2003

Computation of all zeros of a 2D-Bezier curve.
These are the x-coordinates of intersection points of the curve with
the x-axis.
There are at most bcv2->deg such zeros (intersections).

The function works recursively!
Before calling it, set nz = 0!

IN:
  CurvBez2 *bcv2  ... 2D-Bezier curve
  double tol      ... tolerance for considering Bezier curve as line
                      (as smaller tol as straighter the curve)
  int *nz         ... number of zeros
  double *ztab    ... zeros (parameter values in [0,1])
OUT:
  int *nz         ... number of zeros
  double *ztab    ... zeros (parameter values in [0,1])
Returncodes:
  0 = OK
  1 = array for zeros too small
*/

  int      rc, i1, lnex;
  Point2   pmin, pmax, xp;
  Line2    lb, lx;
  CurvBez2 bcv21, bcv22;




  // printf("UT2D_zeros_bez tol=%f nz=%d\n",tol,*nz);


  // bounding rectangle of 2D-Bezier curve
  UT2D_rect_pts (&pmin, &pmax, bcv2->ptNr, bcv2->cptab);
    // printf("bounding %9.3f,%9.3f-%9.3f,%9.3f\n",pmin.x,pmin.y,pmax.x,pmax.y);


  // x-axis does not intersect bounding rectangle -> return
  if (pmax.y < -UT_TOL_min1 || pmin.y > UT_TOL_min1) return 0;


  if (!UT2D_pt_ck_linear (bcv2->ptNr, bcv2->cptab, tol)) {
    // --- Bezier curve is no line ---

    // subdivide Bezier curve
    rc = UT2D_bez_subdiv (&bcv21, &bcv22, memSeg1, bcv2, 0.5);
    if(rc != 0) return 1;

    // repeat this function with bcv21 and bcv22
    rc = UT2D_zeros_bez (nz, ztab, memSeg1, &bcv21, tol);
    if(rc != 0) return 1;
    rc = UT2D_zeros_bez (nz, ztab, memSeg1, &bcv22, tol);
    if(rc != 0) return 1;
  }
  else {
    // --- Bezier curve is a (limited) line ---

    // line from Bezier curve
    lnex = UT2D_ln_pts_dmax (&lb, bcv2->ptNr, bcv2->cptab, UT_TOL_min1);

    if (lnex == 1) {
      // x-axis as line
      lx.p1 = UT2D_PT_NUL;
      lx.p2.x = 1.0;
      lx.p2.y = 0.0;

      // intersect lines
      rc = UT2D_pt_int2ln (&xp, &lb, &lx);
      if (rc <= 0) return 0;
    }
    else {
      // Bezier control points all equal
      xp = bcv2->cptab[0];
    }

    // check array size for zeros
    if (*nz >= UT_BEZDEG_MAX) {
      printf("UT2D_zeros_bez: array for zeros too small\n");
      return 1;
    }

    // save zero (= x-coordinate of intersection point)
    // printf(" ztab[%d] =%f\n",*nz,xp.x);
    ztab[*nz] = xp.x;
    ++(*nz);
  }

  return 0;
}



/*=======================================================================*/
  int UT2D_bez_subdiv (CurvBez2 *bcv1, CurvBez2 *bcv2, Memspc *memSeg1,
                       CurvBez2 *bcv, double t) {
/*===================
UT2D_bez_subdiv     subdivide 2D-Bezier curve

UT2D_bez_subdiv     Author: Thomas Backmeister       3.6.2003

Subdivide a 2D-Bezier curve into 2 2D-Bezier curves.
The composition of the two resulting curves is equal to the original curve.
The underlying algorithm is "de Casteljau".

IN:
  CurvBez2 *bcv  ... 2D-Bezier curve
  double t       ... parameter of subdivision point on the curve
                     (from interval (0,1))
OUT:
  CurvBez2 *bcv1 ... 2D-Bezier curve 1
  CurvBez2 *bcv2 ... 2D-Bezier curve 2
Returncodes:
  0 = OK
  1 = 2D-Bezier curve degree too large
 -1 = input error
*/

  int    i1, i2, numpt, irc;
  double s;
  Point2 ptab[UT_BEZDEG_MAX+1];



  // printf("UT2D_bez_subdiv: ptNr=%d\n",bcv->ptNr);


  if (t <= 0.0 || t >= 1.0) {
    printf("UT2D_bez_subdiv: input error\n");
    return -1;
  }
  if (bcv->ptNr >= UT_BEZDEG_MAX) {
    printf("UT2D_bez_subdiv: degree of 2D-Bezier curve too large\n");
    return 1;
  }


  // init
  bcv1->ptNr = bcv->ptNr;
  bcv2->ptNr = bcv->ptNr;


  // die Startadressen der Punktetabellen -> memSeg1.
  // bcv1 und bcv2 haben ebenfalls ptNr Punkte !
  bcv1->cptab = memSeg1->next;
  //bcv2->cptab = memSeg1->next + (sizeof(Point2) * bcv->ptNr);
  bcv2->cptab = (Point2*)((char*)memSeg1->next + (sizeof(Point2) * bcv->ptNr));


  // den Speicher reservieren (manuell speichen)
  irc = UME_add (memSeg1, sizeof(Point2) * bcv->ptNr * 2);
  if(irc < 0) {
    TX_Error("UT2D_bez_subdiv: out of tempSpace");
    return -1;
  }


  // save 1. / last point
  bcv1->cptab[0] = bcv->cptab[0];
  bcv2->cptab[bcv->ptNr - 1] = bcv->cptab[bcv->ptNr - 1];


  // copy Bezier control points
  for (i1=0; i1<bcv->ptNr; ++i1) {
    ptab[i1] = bcv->cptab[i1];
  }


  // subdivision
  s = 1.0 - t;
  numpt = bcv->ptNr - 1;
  for (i1=0; i1<bcv->ptNr - 1; ++i1) {
    for (i2=0; i2<numpt; ++i2) {
      ptab[i2].x = s * ptab[i2].x + t * ptab[i2+1].x;
      ptab[i2].y = s * ptab[i2].y + t * ptab[i2+1].y;
    }
    bcv1->cptab[i1+1] = ptab[0];
    bcv2->cptab[bcv->ptNr-i1-2] = ptab[numpt-1];
    --numpt;
  }


  // Testausg:
  // DEB_dump_obj__ (Typ_CVBEZ2, bcv1, "  Out - bcv1:\n");
  // DEB_dump_obj__ (Typ_CVBEZ2, bcv2, "  Out - bcv2:\n");


  return 0;
}



/*=======================================================================*/
  int UT3D_pt_projptbez (int *np, Point *ptab, double *ttab, Memspc *memSeg1, 
                          CurvBez *bcv, Point *pt) {
/*======================
UT3D_pt_projptbez     project point onto Bezier curve

UT3D_pt_projptbez     Author: Thomas Backmeister       3.6.2003

Project a point onto a Bezier curve.

IN:
  CurvBez *bcv  ... Bezier curve
  Point *pt     ... point
OUT:
  int *np       ... number of projection points
  Point *ptab   ... array of projection points
  double *ttab  ... corresponding parameter values of projection points
                    (0.0 <= parameter <= 1.0)
Returncodes:
  0 = OK
 -1 = array for projection points too small
 <0 = internal error
*/



  int      rc, i1, i2, nz;
  Mat_4x3  mata;
  CurvBez  bcvt, bcvd;
  CurvBez2 bcv2;
  Point    ppt, *p1;
  Polynom3 polb1, polb2;
  Polynom1 polb, polm;
  double   *ztab;
  void     *memPos1;



  // printf("UT3D_pt_projptbez\n");
    // DEB_dump_obj__ (Typ_CVBEZ, bcv, "UT3D_pt_projptbez: bcv\n");
    // DEB_dump_obj__ (Typ_PT, pt, "   pt->bez:");



  // zuerst Endpunkte testen (da macht UT3D_pt_projptbspl leider Fehler !)
  p1 = &bcv->cptab[0];
  if(UT3D_comp2pt(pt,p1,UT_TOL_cv) == 1) {
    // printf(" UT3D_pt_projptbez -start %f\n",bcv->va);
    *np = 1;
    ptab[0] = *p1;
    ttab[0] = 0.;
    // ttab[0] = bcv->va;
    return 0;
  }
  p1 = &bcv->cptab[bcv->ptNr-1];
  if(UT3D_comp2pt(pt,p1,UT_TOL_cv) == 1) {
    // printf(" UT3D_pt_projptbez -end %f\n",bcv->vb);
    *np = 1;
    ptab[0] = *p1;
    ttab[0] = 1.;
    return 0;
  }



  // init
  *np = 0;

  // (affine) transformation for Bezier curve
  UT3D_m3_load (mata,  (Vector*)&UT3D_VECTOR_IX,
                       (Vector*)&UT3D_VECTOR_IY,
                       (Vector*)&UT3D_VECTOR_IZ);
  UT3D_m3_load_o (mata, pt);

  // transform Bezier curve
  rc = UT3D_bez_trabezm3 (&bcvt, memSeg1, bcv, mata);
  if (rc < 0) return rc;
    // Testausg. Kurve
    // DEB_dump_obj__ (Typ_CVBEZ, &bcvt, "nach trabezm3:\n");

  // 1. derivation of Bezier curve
  rc = UT3D_bez_deriv1 (&bcvd, memSeg1, bcv);
  if (rc < 0) return rc;
    // Testausg. Kurve
    // DEB_dump_obj__ (Typ_CVBEZ, &bcvd, "nach deriv1:\n");


  // x-, y- and z- polynomial of transformed Bezier curve
  // die Polynomdaten wird in memSeg1 abgelegt!
  rc = UT3D_pol_bez (&polb1, memSeg1, &bcvt);
  if (rc < 0) return rc;
    // Testausg. Polynom
    // DEB_dump_obj__ (Typ_Polynom3, &polb1, "Polynom3 polb1 =\n");


  // x-, y- and z- polynomial of 1. derivation Bezier curve
  rc = UT3D_pol_bez (&polb2, memSeg1, &bcvd);
  if (rc < 0) return rc;
    // Testausg. Polynom
    // DEB_dump_obj__ (Typ_Polynom3, &polb2, "Polynom3 polb2 =\n");


  // space fuer polm reservieren und init
  polm.polNr = bcvt.ptNr+bcvd.ptNr-1;
  polm.pol   = memSeg1->next;
  rc = UME_add (memSeg1, sizeof(double) * polm.polNr);
  if(rc < 0) return -1;

  // space fuer polb reservieren und init
  polb.polNr = bcvt.ptNr+bcvd.ptNr-1;
  polb.pol   = memSeg1->next;
  rc = UME_add (memSeg1, sizeof(double) * polb.polNr);
  if(rc < 0) return -1;



  // scalar product <polb1, polb2>
  for (i1=0; i1<polb.polNr; ++i1) polb.pol[i1] = 0.0;
  for (i1=0; i1<3; ++i1) {
    UTM_pol_polmultpol(polm.pol, bcvt.ptNr-1, polb1.pol[i1],
                                 bcvd.ptNr-1, polb2.pol[i1]);
    for (i2=0; i2<=bcvt.ptNr+bcvd.ptNr-2; ++i2) {
      polb.pol[i2] += polm.pol[i2];
    }
  }
    // Testausg. Polynom
    // DEB_dump_obj__ (Typ_Polynom1, &polb, "Polynom1 polb =\n");




  // 2D-Bezier curve from polynomial <polb1, polb2>
  rc = UT2D_bez_pol (&bcv2, memSeg1, &polb);
  if (rc < 0) return rc;
    // Testausg. Kurve
    // DEB_dump_obj__ (Typ_CVBEZ2, &bcv2, "nach pol:\n");


  // init ztab and reserve space
  ztab = memSeg1->next;
  rc = UME_add (memSeg1, sizeof(double) * UT_BEZDEG_MAX);
  if(rc < 0) return -1;


  // zeros of 2D-Bezier curve
  nz = 0;
  rc = UT2D_zeros_bez (&nz, ztab, memSeg1, &bcv2, UT_TOL_ln);
  if(rc != 0) return -1;
    // printf(" nach zeros_bez: nz=%d\n",nz);
    // if(nz>0)printf(" [0]=%f\n",ztab[0]);


  // evaluate zeros for original Bezier curve --> projection points
  for (i1=0; i1<nz; ++i1) {
    rc = UT3D_bez_eval_Pt (&ppt, bcv, ztab[i1]);
    if (rc != 0) continue;
    if (*np >= UT_BEZDEG_MAX) {
      printf("UT3D_pt_projptbez: array for projection points too small\n");
      return -1;
    }
    ptab[*np] = ppt;
    ttab[*np] = ztab[i1];
    ++(*np);
  }


  return 0;
}


//========================================================================
  int UT3D_bez_bspl__ (ObjGX *bezTab, Memspc *memSeg1, CurvBSpl *bspl) {
//========================================================================
/*
UT3D_bez_bspl__       Bezier curves <-- b-spline curve

UT3D_bez_bspl__       Author: Thomas Backmeister       20.5.2003

Computation of Bezier curves from b-spline curve.
Allocated memory for Bezier curves must be freed later!

IN:
  CurvBSpl *bspl  ... b-spline curve
  Memspc *memSeg1  ... space for data (controlpoints)
OUT:
  int *nbcv        ... number of Bezier curves
  CurvBez **bcvtab ... Bezier curves
Returncode:
  0 = OK
 -1 = Memspc too small
*/


# define  bcvtabSiz  256

  int      p, rc, nbcv, bcv_deg;
  long     nbmax;
  double   t, va, vb;
  Point    pt;
  CurvBez  bcv, bcvtab[bcvtabSiz];


  // init
  nbcv = 0;

  nbmax = bspl->ptNr - bspl->deg;


  // printf("UT3D_bez_bspl__ %d\n",bspl->ptNr - bspl->deg);
  // printf("  free tmpSpc=%d\n",UME_ck_free(memSeg1));
  // printf("  nbmax=%d ptNr=%d deg=%d\n",nbmax,bspl->ptNr,bspl->deg);



  for (p=0; p<nbmax; ++p) {

    // segment start- and endparameter
    va = bspl->kvTab[bspl->deg+p];
    vb = bspl->kvTab[bspl->deg+p+1];
/*
    2003-06-13: RF; raus; sonst keine Projektion ausserhalb aktiven Bereich!
    if (bspl->v0 >= vb) continue;
    if (bspl->v1 <= va) break;
    if (bspl->v0 > va) va = bspl->v0;
    if (bspl->v1 < vb) vb = bspl->v1;
*/
    // printf("........next seg p=%d va=%f vb=%f\n",p,va,vb);


    // Bezier curve from b-spline curve segment
    bcv.ptNr  = 0;
    bcv.va = va;
    bcv.vb = vb;
    bcv.cptab = memSeg1->next; // alle Punkte ab memSeg1->next speichern
    rc = UT3D_bez_bsplseg (&bcv, va, vb, 2*bspl->deg,
                           &(bspl->kvTab[p+1]), bspl->deg+1,
                           &(bspl->cpTab[p]));
    if (rc != 0) continue;


    // die Punkte der CurvBez liegen im memSeg1; dessen Pointer weiterzaehlen
    rc = UME_add (memSeg1, sizeof(Point) * bcv.ptNr);
    if(rc < 0) goto L_outOfMem;


    // next curve
    bcvtab[nbcv] = bcv;
    ++nbcv;
      // printf("  _bez_bspl__-nbcv=%d\n",nbcv);
    if(nbcv >= bcvtabSiz) goto L_outOfMem;


/*
    // ---- TEST the curve ----
    for (t=0.0; t<=1.0; t+=0.1) {
      UT3D_pt_evbcv (&pt, &bcv, t);
      fprintf(fpo,"P%d=P(%f,%f,%f)\n",ptoNr, pt.x, pt.y, pt.z);
      ++ptoNr;
    }
    // ---- TEST the curve ----
*/


  }


  // nun muss der tempspace bcvtab noch ins memSeg1 kopiert werden
  bezTab->typ  = Typ_CVBEZ;
  bezTab->form = Typ_CVBEZ;    // direkt; alles im memSeg1
  bezTab->siz  = nbcv;         // number of Bezier curves
  bezTab->data = UME_save (memSeg1, bcvtab, sizeof(CurvBez) * nbmax);


  return 0;




  //---------------------------------
  L_outOfMem:
    TX_Error("UT3D_bez_bspl__ EOM (out of mem)");
      printf(" UT3D_bez_bspl__ EOM (out of mem)\n");
    return -1;
}


//===================== EOF ===========================
