//**************************************************************************
/*
 *
 * Copyright (C) 2015 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
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
2007-08-15 UT3D_vc_evalparCv liefert falsche Tangenten bei Grad 1!

-----------------------------------------------------
Modifications:
2007-08-14 NEU: UCBS_NxtPtBspCrv.
2007-08-06 Änderung von Funktionsnamen:
           UT3D_pt_evparCrvBSpl in UCBS_EvPtBspCrv;
	   UT3D_testbspl        in UCBS_TestSegsBspCrv;
	   UT3D_testrbspl       in UCBS_TestSegsRBspCrv;
	   UT3D_testbsplbez     in UCBS_TestBezBspCrv.
2007-08-04 UCBS_IntpolBspCrv wird gelöscht.
2007-07-26 UCBS_BspCrvPts und UBS_ParVecPts: neuer Parameter pvType.
2007-07-26 UBS_CrssParVecBspCrvs in ubs.c --> UCBS_CrssParVecBspCrvs in ubscrv.c
2007-07-24 Änderung von Funktionsnamen:
           alle ubs_xxx, die bspline Kurven bearbeiten, in UCBS_xxx.
2007-07-18 Fehler in ubs_CtrlPtsIntpol behoben (Gewichte wTab).
2007-04-28 UT3D_pt_evparCrvBSpl in ut/ut_bspl.h eintragen!
2007-04-28 Änderung von Funktionsnamen:
           UT3D_bspl_crvisopar   in ubs_DirIndIsoBspSur
           UT3D_bspl_crvisoparuv in ubs_IsoBspCrvBspSur
           UT3D_bspl_offcvsurf   in ubs_OffSurBspCrv
           UT3D_bspl_crvsubdiv   in ubs_SubDivBspCrv
           UT3D_bspl_crvdiv1in3  in ubs_3BspCrvsBspCrv

-----------------------------------------------------
*/
/*!
\file  ../ut/ubscrv.c
\brief b spline curve functions
\code
=====================================================
List_functions_start:

UCBS_BspCrvPts            Bspline curve <-- points 
UCBS_CpyBspCrv            Copy <-- bspline curve 
UCBS_CrssParVecBspCrvs    Cross parametervector <-- bspline curves
UCBS_CtrlPtsIntpol        Controlpoints of interpolating bsp.crv. <-- points 
UCBS_DegElevBspCrv        Degree elevate <-- bspline curve 
UCBS_DifKntsKntVecBspCrv  Difference knots <-- knotvector, bspline curve 
UCBS_DirIndIsoBspSur      Dir. indep. isopar. bspline curve <-- bspline surface 
UCBS_EvPtBspCrv           Evaluate point <-- bspline curve
UCBS_IsoBspCrvBspSur      Isoparametric bspline curve <-- bspline surface
UCBS_MkeCmpBspCrvs        Compatible bspline curves <-- bspline curves 
UCBS_MrgKntVecBspCrvs     Merge knotvectors <-- bspline curves 
UCBS_NrmKntVecBspCrv      Normalize knotvector <-- bspline curve 
UCBS_NxtPtBspCrv          Next point <-- bspline curve & point
UCBS_OffSurBspCrv         Offset surface bspline curve <-- bspline curve
UCBS_PrepCoons3BspCrvs    Prepare Coonspatch <-- 3 bspline curves
UCBS_PtUBspCrv            Point <-- U-parameter bspline curve
UCBS_RefKntVecBspCrv      Refine knotvector <-- bspline curve 
UCBS_RevBspCrv            Reverse <-- bspline curve
UCBS_SubDivBspCrv         Subdivision <-- bspline curve
UCBS_TypCon2BspCrvs       Type of connection <-- 2 bspline curves
UCBS_XPtsBspCrvNet        Intersection points <-- bspline curve net
UCBS_3BspCrvsBspCrv       3 bspline curves <-- bspline curve

List_functions_end:
=====================================================
- test functions:
UCBS_TestBezBspCrv        Test Bezier curves <-- bspline curve
UCBS_TestSegsBspCrv       Test segments <-- bspline curve 
UCBS_TestSegsRBspCrv      Test segments <-- rational bspline curve 

\endcode *//*----------------------------------------



Returncodes:
 0 OK
>0 OK with additional info
<0 Error with additional info


compile with gcc:
cc -c -O0 -g3 -Wall ubscrv.c

Windows:
cl -c ubscrv.c

ctags -f ubscrv.tag ubscrv.c

*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

// #include "../ut/ut_umem.h"
#include "../ut/ut_geo.h"
#include "../ut/ut_math.h"
#include "../ut/ut_TX.h"
#include "../ut/ubs.h"
#include "../ut/ubscrv.h"



/*=======================================================================*/
  int UCBS_NxtPtBspCrv (Point *Pn, double *tn, CurvBSpl *cv, Point *P,
		        Memspc *workSeg) {
/*====================
UCBS_NxtPtBspCrv     Next point <-- bspline curve & point

UCBS_NxtPtBspCrv      Author: Thomas Backmeister       14.8.2007

Computation of the next point to the given point on a bspline curve.
The search works inside the curve and on both of its ends.
The limiting parameters of the curve are not considered.

ATTENTION: the function may give wrong results in case of a curve
with degree 1 which is NOT SMOOTH (not differenciable).
This special case can be solved in a future work (if necessary).
If however the curve has degree 1, but is SMOOTH, the algorithm
works correctly.

IN:
  CurvBSpl *cv     ... bspline curve
  Point *P         ... a point
  Memspc *workSeg  ... workspace
OUT:
  Point *Pn        ... next point
  double *tn       ... parameter of Pn
Returncodes:
  0 = OK
 -1 = internal error
*/

  int rc, nxp, i1;
  double ttab[10], dmin, d;
  Point ptab[10], pt;

  //printf("\nUCBS_NxtPtBspCrv: Next point <-- bspline curve & point\n");

  // next curve endpoint
  *tn = cv->kvTab[0];
  rc = UCBS_EvPtBspCrv (Pn, cv, cv->kvTab[0], workSeg);
  if (rc < 0) return -1;
  dmin = UT3D_len_2pt (P, Pn);
  rc = UCBS_EvPtBspCrv (&pt, cv, cv->kvTab[cv->ptNr+cv->deg], workSeg);
  if (rc < 0) return -1;
  d = UT3D_len_2pt (P, &pt);
  if (d < dmin) {
    *tn = cv->kvTab[cv->ptNr+cv->deg];
    *Pn = pt;
    dmin = d;
  }

  // project point onto bspline curve
  nxp = 10;
  rc = UT3D_pt_projptbspl (&nxp, ptab, ttab, cv, P);
  if (rc < 0) return -1;  // muss workSeg freigegeben werden ?

  // look for a closer point
  if (nxp > 0) {
    for (i1=0; i1<nxp; ++i1) {
      d = UT3D_len_2pt (P, &(ptab[i1]));
      if (d < dmin) {
        *tn = ttab[i1];
        *Pn = ptab[i1];	
        dmin = d;
      }
    }
  }

  return 0;
}



//================================================================
//================================================================
// TESTFUNKTIONEN:
//================================================================
//================================================================


#ifdef DEB


/*=======================================================================*/
  int UCBS_TestBezBspCrv (CurvBSpl *bspl, Memspc *workSeg) {
/*======================
UCBS_TestBezBspCrv     Test Bezier curves <-- bspline curve

UCBS_TestBezBspCrv      Author: Thomas Backmeister       6.6.2003

IN:
  CurvBSpl *bspl  ... bspline curve
  Memspc *workSeg ... work space
OUT:
Returncodes:
  0 = OK
 -1 = out of work space
 -2 = internal error
*/

  int rc, p;
  long nbseg;
  double va, vb, t, u;
  void *workstart;
  Point pt;
  CurvBez bcv;

  //printf("UCBS_TestBezBspCrv: Test Bezier curves <-- bspline curve\n");

  workstart = workSeg->next;

  nbseg = bspl->ptNr - bspl->deg;
  printf("number of segments= %ld\n", nbseg);

  for (p=0; p<nbseg; ++p) {

    // segment start- and endparameter
    va = bspl->kvTab[bspl->deg+p];
    vb = bspl->kvTab[bspl->deg+p+1];
    if (bspl->v0 >= vb) continue;
    if (bspl->v1 <= va) break;
    if (bspl->v0 > va) va = bspl->v0;
    if (bspl->v1 < vb) vb = bspl->v1;

    // Bezier curve from bspline curve segment
    bcv.ptNr  = 0;
    bcv.va = va;
    bcv.vb = vb;
    bcv.cptab = workSeg->next;
    rc = UME_add (workSeg, sizeof(Point) * (bspl->deg + 1));
    if (rc < 0) goto L_outOfMem;
    rc = UT3D_bez_bsplseg (&bcv, va, vb, 2*bspl->deg, &(bspl->kvTab[p+1]),
                           bspl->deg+1, &(bspl->cpTab[p]));
    if (rc != 0) continue;

    // ---- TEST bspline segment ----
    printf("\nB-SPLINE segment %d\n", p);
    for (t=0.0; t<=1.0; t+=0.20) {
      u = (1.0-t) * va + t * vb;
      rc = UCBS_EvPtBspCrv (&pt, bspl, u, workSeg);
      if (rc < 0) return -2;
      printf("t= %f  u= %f   pt= %f %f %f\n",t,u,pt.x,pt.y,pt.z);
      //cre_obj (Typ_PT, Typ_PT, 1, (void*)&pt);
    }

    // ---- TEST Bezier curve ----
    printf("\nBEZIER curve:\n");
    for (t=0.0; t<=1.0; t+=0.20) {
      UT3D_bez_eval_Pt (&pt, &bcv, t);
      printf("t= %f                pt= %f %f %f\n",t,pt.x,pt.y,pt.z);
      //cre_obj (Typ_PT, Typ_PT, 1, (void*)&pt);
    }
    printf("\n");

    // release memory
    workSeg->next = workstart;
  }

  return 0;

L_outOfMem:
    TX_Error("UCBS_TestBezBspCrv: out of work space");
    return -1;
}


/*=======================================================================*/
  int UCBS_TestSegsRBspCrv (CurvRBSpl *rbspl, Memspc *workSeg) {
/*========================
UCBS_TestSegsRBspCrv     Test segments <-- rational bspline curve 

UCBS_TestSegsRBspCrv      Author: Thomas Backmeister       25.9.2003

IN:
  CurvRBSpl *rbspl  ... rational bspline curve
  Memspc *workSeg   ... work space
OUT:
Returncodes:
  0 = OK
 -1 = internal error
*/

  int rc, p;
  long nbseg;
  double va, vb, t, u;
  Point pt;
  Vector tg;
  Line ln;
  CurvBSpl g, fd, gd;
  void *memstart;

  //printf("UCBS_TestSegsRBspCrv: Test segments <-- rational bspline curve\n");

  nbseg = rbspl->ptNr - rbspl->deg;
  printf("number of segments= %ld\n", nbseg);

  // "1. derivation" of rational bspline curve
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

    // ---- TEST rational bspline segment ----
    printf("\nRational B-SPLINE segment %d\n", p);
    for (t=0.0; t<=1.0; t+=0.2) {
      u = (1.0-t) * va + t * vb;

      // curve point
      rc = UT3D_pt_evparCrvRBSpl (&pt, rbspl, u, workSeg);
      if (rc < 0) return -1;
      printf("u= %f   pt= %f %f %f\n",u,pt.x,pt.y,pt.z);
      // cre_obj (Typ_PT, Typ_PT, 1, (void*)&pt);

      // curve tangent
      rc = UT3D_vc_evparCrvRBSpl (&tg, rbspl, &g, &fd, &gd, u, workSeg);
      if (rc < 0) return -1;
      printf("u= %f   tg= %f %f %f\n",u,tg.dx,tg.dy,tg.dz);
      ln.p1 = pt;
      ln.p2.x = pt.x + tg.dx;
      ln.p2.y = pt.y + tg.dy;
      ln.p2.z = pt.z + tg.dz;
      // cre_obj (Typ_LN, Typ_LN, 1, (void*)&ln);
    }
  }

  // release workspace
  workSeg->next = memstart;

  return 0;
}


/*=======================================================================*/
  int UCBS_TestSegsBspCrv (CurvBSpl *bspl, Memspc *workSeg) {
/*=======================
UCBS_TestSegsBspCrv     Test segments <-- bspline curve 

UCBS_TestSegsBspCrv      Author: Thomas Backmeister       12.9.2003

IN:
  CurvBSpl *bspl  ... bspline curve
  Memspc *workSeg ... work space
OUT:
Returncodes:
  0 = OK
*/

  int rc, p;
  long nbseg;
  double va, vb, t, u;
  Point pt;

  //printf("UCBS_TestSegsBspCrv: Test segments <-- bspline curve\n");

  nbseg = bspl->ptNr - bspl->deg;
  printf("number of segments= %ld\n", nbseg);

  for (p=0; p<nbseg; ++p) {

    // segment start- and endparameter
    va = bspl->kvTab[bspl->deg+p];
    vb = bspl->kvTab[bspl->deg+p+1];
    if (bspl->v0 >= vb) continue;
    if (bspl->v1 <= va) break;
    if (bspl->v0 > va) va = bspl->v0;
    if (bspl->v1 < vb) vb = bspl->v1;

    // ---- TEST bspline segment ----
    printf("\nB-SPLINE segment %d\n", p);
    //for (t=0.0; t<=1.0; t+=0.20) {
    for (t=0.0; t<=1.00001; t+=0.05) {
      u = (1.0-t) * va + t * vb;
      rc = UCBS_EvPtBspCrv (&pt, bspl, u, workSeg);
      if (rc < 0) return -1;
      printf("u= %f   pt= %f %f %f\n",u,pt.x,pt.y,pt.z);
      // cre_obj (Typ_PT, Typ_PT, 1, (void*)&pt);
    }
  }

  return 0;
}

#endif


/*=======================================================================*/
  int UCBS_EvPtBspCrv (Point *pt, CurvBSpl *bspl, double u, Memspc *workSeg) {
/*===================
UCBS_EvPtBspCrv     Evaluate point <-- bspline curve

UCBS_EvPtBspCrv      Author: Thomas Backmeister       12.9.2003

Evaluate a point on a bspline curve.
The underlying algorithm is "de Boor".
The point is computed for any parameter value within the maximal
support of the curve (we do not consider the limit parameters
bspl->v0 and bspl->v1).

This function evaluates without using polynomials.
(in contrast to UT3D_pt_evalparCv)
It needs work space of size (curve degree * sizeof(Point)).

IN:
  CurvBSpl *bspl  ... bspline curve
  double u        ... parameter value
  Memspc *workSeg ... work space (control points)
OUT:
  Point *pt       ... curve point
Returncodes:
  0 = OK
 -1 = out of work space
 -2 = input error
*/

  int rc, d, n, m, r, s;
  int i1, i2, ih, jh, n1;
  double a, b;
  void *memstart;
  Point *ptab;

  //printf("UCBS_EvPtBspCrv: Evaluate point <-- bspline curve\n");

  // some abbreviations
  d = bspl->deg;
  n = bspl->ptNr - 1;
  m = n + d + 1;

  // check parameter
  if (u < bspl->kvTab[d]) {
    //printf("UCBS_EvPtBspCrv: parameter below curve support!\n");
    u = bspl->kvTab[d];
  } 
  if (u > bspl->kvTab[bspl->ptNr]) {
    //printf("UCBS_EvPtBspCrv: parameter above curve support!\n");
    u = bspl->kvTab[bspl->ptNr];
  }

  // determine segment of parameter
  r = -1;
  for (s=d; s<m-d; ++s) {
    if (bspl->kvTab[s] <= u && u <= bspl->kvTab[s+1]) {
      r = s;
      break;
    }
  }
  if (r == -1) {
    printf("UCBS_EvPtBspCrv: input error\n");
    return -2;
  }

  // provide work space
  memstart = workSeg->next;
  ptab = workSeg->next;
  rc = UME_add (workSeg, sizeof(Point) * (d+1));
  if (rc < 0) goto L_outOfWorkSpace;
 
  // init control points
  ih = r - d;
  for (i1=0; i1<d+1; ++i1) ptab[i1] = bspl->cpTab[ih+i1];

  // compute curve point
  n1 = d;
  for (i1=0; i1<d; ++i1) {
    ih = r + 1;
    jh = ih - n1;
    for (i2=0; i2<n1; ++i2) {
      a = (u - bspl->kvTab[jh+i2]) / (bspl->kvTab[ih+i2] - bspl->kvTab[jh+i2]);
      b = 1.0 - a;
      ptab[i2].x = b * ptab[i2].x + a * ptab[i2+1].x;
      ptab[i2].y = b * ptab[i2].y + a * ptab[i2+1].y;
      ptab[i2].z = b * ptab[i2].z + a * ptab[i2+1].z;
    }
    --n1;
  }
  *pt = ptab[0];

  // release work space
  workSeg->next = memstart;

  return 0;

L_outOfWorkSpace:
  TX_Error("UCBS_EvPtBspCrv: out of work space");
  return -1;
}


/*=======================================================================*/
  int UCBS_CrssParVecBspCrvs (double **pvTab, Memspc *memSeg, int ncv,
		              CurvBSpl *bspTab) {
/*==========================
UCBS_CrssParVecBspCrvs     Cross parametervector <-- bspline curves 

UCBS_CrssParVecBspCrvs      Author: Thomas Backmeister       9.5.2007

Cross parametervector of a set of bspline curves.
This is an array of values in [0,1] reflecting the averaged distances
between the input curves, computed from their controlpoints.
All curves must have the same number of controlpoints.

IN:
  int ncv           ... number of bspline curves
  CurvBSpl *bspTab  ... array of bspline curves
OUT:
  double **pvTab    ... parametervector (ncv)
  Memspc *memSeg    ... data space of pvTab (and workspace)
Returncodes:
  0 = OK
 -1 = out of space
*/

  int rc, i1, i2;
  double *dTab;
  void *memStart;

  //printf("UCBS_CrssParVecBspCrvs: Cross parametervector <-- bspline curves\n"); 

  // space for pvTab
  *pvTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * ncv);
  if (rc < 0) goto L_outOfSpace;

  // start of workspace
  memStart = memSeg->next;

  // space for dTab
  dTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * bspTab[0].ptNr);
  if (rc < 0) goto L_outOfSpace;

  // compute dTab
  for (i2=0; i2<bspTab[0].ptNr; ++i2) dTab[i2] = 0.0;
  for (i1=1; i1<ncv; ++i1) {
    for (i2=0; i2<bspTab[0].ptNr; ++i2)
      dTab[i2] +=
        UT3D_len_2pt (&(bspTab[i1-1].cpTab[i2]), &(bspTab[i1].cpTab[i2]));  
  }

  // compute pvTab
  (*pvTab)[0] = 0.0;
  (*pvTab)[ncv-1] = 1.0;
  for (i1=1; i1<ncv-1; ++i1) {
    (*pvTab)[i1] = 0.0;; 
    for (i2=0; i2<bspTab[0].ptNr; ++i2) {
      if (UTP_comp_0 (dTab[i2])) continue;
      (*pvTab)[i1] = (*pvTab)[i1] +
        (UT3D_len_2pt (&(bspTab[i1-1].cpTab[i2]), &(bspTab[i1].cpTab[i2]))) /
        dTab[i2];
    }
    (*pvTab)[i1] /= (double)bspTab[0].ptNr;
    (*pvTab)[i1] += (*pvTab)[i1-1];
  }

  // release workspace
  memSeg->next = memStart;

  return 0;
 
L_outOfSpace:
  TX_Error("UCBS_CrssParVecBspCrvs: out of space");
  return -1;
}


/*=======================================================================*/
  int UCBS_PrepCoons3BspCrvs (CurvBSpl *bspTU, CurvBSpl *bspTV,
		              Memspc *memSeg, CurvBSpl **bspT) {
/*==========================
UCBS_PrepCoons3BspCrvs     Prepare Coonspatch <-- 3 bspline curves

UCBS_PrepCoons3BspCrvs      Author: Thomas Backmeister       23.7.2007

Three given bspline curves are supposed to form a closed contour.
Reorder, possibly reverse some curves, and create a degenerated
fourth bspline curve at the end of curve 0, such that the four
resulting curves can be used to create a (degenerated) Coons patch.

IN:
  CurvBSpl **bspT    ... bspline curves (3)
OUT:
  CurvBSpl *bspTU    ... bspline curves in U-direction (2)
  CurvBSpl *bspTV    ... bspline curves in V-direction (2)
                         (bspTV[1] degenerated)
  Memspc *memSeg     ... data space of bspTU and bspTV (& workspace)
Returncodes:
  0 = OK
 -1 = internal error
 -2 = input error
*/

  int rc, rc01, rc12, i1;
  Point pt;
  CurvBSpl bsp;

  //printf("UCBS_PrepCoons3BspCrvs: Prepare Coonspatch <-- 3 bspline curves\n");

  // type of connection of input curves
  rc01 = UCBS_TypCon2BspCrvs (bspT[0], bspT[1], memSeg);
  if (rc01 < 0) return -1;
  rc12 = UCBS_TypCon2BspCrvs (bspT[1], bspT[2], memSeg);
  if (rc12 < 0) return -1;

  // endpoint of 1. curve
  rc = UCBS_EvPtBspCrv (&pt, bspT[0], bspT[0]->v1, memSeg);
  if (rc < 0) return -1;

  // 1. U-curve
  rc = UCBS_CpyBspCrv (&bspTU[0], memSeg, bspT[0]);
  if (rc < 0) return -1;

  // 2. U-curve and 1. V-curve 
  if (rc01 == 1) {
    rc = UCBS_CpyBspCrv (&bspTV[0], memSeg, bspT[1]);
    if (rc < 0) return -1;
    if (rc12 == 3) {
      rc = UCBS_CpyBspCrv (&bspTU[1], memSeg, bspT[2]);
      if (rc < 0) return -1;
    }
    else if (rc12 == 4) {
      rc = UCBS_RevBspCrv (&bspTU[1], memSeg, bspT[2]);
      if (rc < 0) return -1;
    }	   
    else return -2; 
  }    
  else if (rc01 == 2) {
    rc = UCBS_RevBspCrv (&bspTV[0], memSeg, bspT[1]);
    if (rc < 0) return -1;
    if (rc12 == 1) {
      rc = UCBS_CpyBspCrv (&bspTU[1], memSeg, bspT[2]);
      if (rc < 0) return -1;
    }	  
    else if (rc12 == 2) {
      rc = UCBS_RevBspCrv (&bspTU[1], memSeg, bspT[2]);
      if (rc < 0) return -1;
    }	    
    else return -2; 
  }
  else if (rc01 == 3) {
    rc = UCBS_RevBspCrv (&bspTU[1], memSeg, bspT[1]);
    if (rc < 0) return -1;
    if (rc12 == 3) {
      rc = UCBS_RevBspCrv (&bspTV[0], memSeg, bspT[2]);
      if (rc < 0) return -1;
    }	    
    else if (rc12 == 4) {
      rc = UCBS_CpyBspCrv (&bspTV[0], memSeg, bspT[2]);
      if (rc < 0) return -1;
    }	   
    else return -2; 
  }
  else if (rc01 == 4) {
    rc = UCBS_CpyBspCrv (&bspTU[1], memSeg, bspT[1]);
    if (rc < 0) return -1;
    if (rc12 == 1) {
      rc = UCBS_RevBspCrv (&bspTV[0], memSeg, bspT[2]);
      if (rc < 0) return -1;
    }
    else if (rc12 == 2) {
      rc = UCBS_CpyBspCrv (&bspTV[0], memSeg, bspT[2]);
      if (rc < 0) return -1;
    }
    else return -2;
  }
  else return -2;

  // 2. V-curve (degenerated)
  rc = UCBS_CpyBspCrv (&bspTV[1], memSeg, &bspTV[0]);
  if (rc < 0) return -1;
  for (i1=0; i1<bspTV[1].ptNr; ++i1) bspTV[1].cpTab[i1] = pt;

  return 0;
}


/*=======================================================================*/
  int UCBS_TypCon2BspCrvs (CurvBSpl *bsp1, CurvBSpl *bsp2, Memspc *workSeg) {
/*=======================
UCBS_TypCon2BspCrvs     Type of connection <-- 2 bspline curves

UCBS_TypCon2BspCrvs      Author: Thomas Backmeister       21.7.2007

Check the type of connection of two bspline curves.
It is assumed that none of the two curves is closed. 

Notations for result:
Si...startpoint of curve i, Ej...endpoint of curve j.

IN:
  CurvBSpl *bsp1   ... bspline curve 1
  CurvBSpl *bsp2   ... bspline curve 2
  Memspc *workSeg  ... workspace
OUT:
Returncodes:
 -1 = internal error
  0 = unconnected 
  1 = only S1-S2 connected 
  2 = only S1-E2 connected
  3 = only E1-S2 connected
  4 = only E1-E2 connected
  5 = S1-S2 and E1-E2 connected
  6 = S1-E2 and E1-S2 connected
*/

  int rc;
  Point S1, S2, E1, E2;  

  //printf("UCBS_TypCon2BspCrvs: Type of connection <-- 2 bspline curves\n");

  rc = UCBS_EvPtBspCrv (&S1, bsp1, bsp1->v0, workSeg);
  if (rc < 0) return -1; 
  rc = UCBS_EvPtBspCrv (&S2, bsp2, bsp2->v0, workSeg);
  if (rc < 0) return -1; 
  rc = UCBS_EvPtBspCrv (&E1, bsp1, bsp1->v1, workSeg);
  if (rc < 0) return -1; 
  rc = UCBS_EvPtBspCrv (&E2, bsp2, bsp2->v1, workSeg);
  if (rc < 0) return -1; 

  if (UT3D_comp2pt (&S1, &S2, UT_TOL_pt)) {            // S1-S2
    if (UT3D_comp2pt (&E1, &E2, UT_TOL_pt)) return 5;    // E1-E2
    else return 1;                                       // !(E1-E2)
  }                                       
  else if (UT3D_comp2pt (&S1, &E2, UT_TOL_pt)) {       // S1-E2
    if (UT3D_comp2pt (&E1, &S2, UT_TOL_pt)) return 6;    // E1-S2
    else return 2;                                       // !(E1-S2)
  }
  else if (UT3D_comp2pt (&E1, &S2, UT_TOL_pt)) {       // E1-S2   
    if (UT3D_comp2pt (&S1, &E2, UT_TOL_pt)) return 6;    // S1-E2
    else return 3;                                       // !(S1-E2)
  }
  else if (UT3D_comp2pt (&E1, &E2, UT_TOL_pt)) {       // E1-E2
    if (UT3D_comp2pt (&S1, &S2, UT_TOL_pt)) return 5;    // S1-S2
    else return 4;                                       // !(S1-S2)
  }	  
  else return 0;
}


/*=======================================================================*/
  int UCBS_RevBspCrv (CurvBSpl *bspr, Memspc *memSeg, CurvBSpl *bsp) {
/*==================
UCBS_RevBspCrv     Reverse <-- bspline curve

UCBS_RevBspCrv      Author: Thomas Backmeister       21.7.2007

Reverse a bspline curve.
The parameterrange remains unchanged.

IN:
  CurvBSpl *bsp   ... bspline curve
OUT:
  CurvBSpl *bspr  ... reverse bspline curve
  Memspc *memSeg  ... data space of bspr
Returncodes:
  0 = OK
 -1 = internal error
*/

  int     rc, i1, i2, m;
  double  c;


  // printf("UCBS_RevBspCrv: Reverse <-- bspline curve\n");
  // UT3D_stru_dump(Typ_CVBSP, bsp, "bsp-in:");

  // make a copy of bsp
  rc = UCBS_CpyBspCrv (bspr, memSeg, bsp);
  if (rc < 0) return -1;
    // UT3D_stru_dump(Typ_CVBSP, bspr, "bsp-copied:");

  // knotvector
  c = bspr->kvTab[0] + bspr->kvTab[bspr->ptNr+bspr->deg];
  for (i1=1; i1<bspr->ptNr-bspr->deg; ++i1)
    bspr->kvTab[bspr->ptNr-i1] = c - bsp->kvTab[bsp->deg+i1];

  // control points - invert table
  // for (i1=0; i1<bsp->ptNr; ++i1)
    // bspr->cpTab[i1] = bsp->cpTab[bsp->ptNr-1-i1];	  
  MEM_inv_rtab (bsp->ptNr, bspr->cpTab, sizeof(Point));

  // start- and endparameter
  bspr->v0 = c - bsp->v1;
  bspr->v1 = c - bsp->v0;

/*
  // make a copy of bsp
  // rc = UCBS_CpyBspCrv (bspr, memSeg, bsp);
  // if (rc < 0) return -1;
  m = bsp->ptNr + bsp->deg + 1;
    printf(" m=%d\n",m);
  bspr->kvTab = memSeg->next;
  irc = UME_add (memSeg, sizeof(double) * m);
  if(irc < 0) return -1;
  bspr->cpTab = memSeg->next;
    printf(" ptnr=%d\n",bsp->ptNr);
  irc = UME_add (memSeg, sizeof(Point) * bsp->ptNr);
  if(irc < 0) return -1;



  // knotvector
  c = bsp->kvTab[0] + bsp->kvTab[bsp->ptNr+bsp->deg];
  for (i1=1; i1<bsp->ptNr-bsp->deg; ++i1)
    bspr->kvTab[bsp->ptNr-i1] = c - bsp->kvTab[bsp->deg+i1];

  // control points
  for (i1=0; i1<bsp->ptNr; ++i1)
    bspr->cpTab[i1] = bsp->cpTab[bsp->ptNr-1-i1];	  

  // start- and endparameter
  bspr->ptNr = bsp->ptNr;
  bspr->deg  = bsp->deg;
  bspr->v0 = c - bsp->v1;
  bspr->v1 = c - bsp->v0;
*/

  // UT3D_stru_dump(Typ_CVBSP, bspr, "bsp-inverted:");

  return 0;
}


/*=======================================================================*/
  int UCBS_XPtsBspCrvNet (Point **xpTab, double **pvTU, double **pvTV,
		          Memspc *memSeg, int cvNrU, CurvBSpl *bspTU,
                          int cvNrV, CurvBSpl *bspTV, Memspc *workSeg) {   
/*======================
UCBS_XPtsBspCrvNet     Intersection points <-- bspline curve net

UCBS_XPtsBspCrvNet      Author: Thomas Backmeister       19.7.2007

Intersection points of a U/V-net of bspline curves.
The U- resp. V-curves must be compatible.
It is assumed that each (U,V)-curve pair has exactly one intersection point.
It is also assumed that the intersection points of any V-curve with all
U-curves have the same U-parameter value and the intersection points
of any U-curve with all V-curves have the same V-parameter value.
Further it is assumed that all U-curves are parameterized in the same
direction with respect to the V-curves starting at the first V-curve
and that all V-curves are parameterized in the same direction with respect
to the U-curves starting at the first U-curve.

IN:
  int cvNrU         ... number of bspline curves in U-direction (> 1)
  CurvBSpl *bspTU   ... bspline curves in U-direction
  int cvNrV         ... number of bspline curves in V-direction (> 1)
  CurvBSpl *bspTV   ... bspline curves in V-direction
  Memspc *workSeg   ... workspace
OUT:
  Point **xpTab     ... intersection points (cvNrU * cvNrV)
                        (i*cvNrV+j, ij=00,01,...,10,11,...)
  double **pvTU     ... common U-parameters of intersection points
  double **pvTV     ... common V-parameters of intersection points
  Memspc *memSeg    ... data space of xpTab, pvTU, pvTV
Returncodes:
  0 = OK
 -1 = internal error
 -2 = input error
 -3 = out of space
*/

  int i1, i2, k1, nxp, rc;
  double pvT[10];
  Point xpT[10], *pt1;
  // Memspc   tmpSpc=UME_NEW;

  // printf("UCBS_XPtsBspCrvNet cvNrU=%d cvNrV=%d\n",cvNrU,cvNrV);

  // UME_alloca (&tmpSpc, 5000000);  // 5MB tempSpace
  // UME_alloca (&tmpSpc, 1000000);  // 1MB tempSpace (MS: Stack Overflow !)


  // space for intersection points
  *xpTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(Point) * cvNrU * cvNrV);
  if (rc < 0) goto L_outOfSpace;


  // space for U/V-parametervectors
  *pvTU = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * cvNrV);
  if (rc < 0) goto L_outOfSpace;
  *pvTV = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * cvNrU);
  if (rc < 0) goto L_outOfSpace;



  // test if 1.U-curve is a point;  2011-08-30 RF
  //   yes: copy point -> xpTab; pvTV=0/1.
  if(UT3D_bsp_ck_pt (&(bspTU[0])) == 0)  {
      // printf(" 1.U-curve is a point\n");
    pt1 = &bspTU[0].cpTab[0];
    for (i1=0; i1<cvNrV; ++i1) (*xpTab)[i1] = *pt1;
    (*pvTU)[0] = 0.;
    (*pvTU)[1] = 1.;
    goto L_pt_v0;
  }


  // intersection points with U-parameter values on 1. U-curve
  for (i1=0; i1<cvNrV; ++i1) {
    // get startpoints of all V-curves
    rc = UCBS_EvPtBspCrv (&((*xpTab)[i1]), &(bspTV[i1]), 0.0, workSeg);
    if (rc < 0) return -1;
    nxp = 10;	  
    // rc = UT3D_pt_projptbspl (&nxp, xpT, pvT, workSeg, &(bspTU[0]),
    rc = UT3D_pt_projptbspl (&nxp, xpT, pvT, &(bspTU[0]),
		             &((*xpTab)[i1]));
    if (rc < 0) return -1;
    if (nxp < 1) goto L_InErr;
    (*pvTU)[i1] = pvT[0];
      // UT3D_stru_dump (Typ_PT, &xpT[0], " U0-V%d pvTU=%f",i1,(*pvTU)[i1]);
  }



  // test if 1.V-curve is a point;              2011-08-30 RF
  //   yes: copy point -> xpTab; pvTV=0/1.
  L_pt_v0:
  if(UT3D_bsp_ck_pt (&(bspTV[0])) == 0)  {
      // printf(" 1.V-curve is a point\n");
    pt1 = &bspTV[0].cpTab[0];
    for (i1=0; i1<cvNrU; ++i1) (*xpTab)[i1*cvNrV] = *pt1;
    (*pvTV)[0] = 0.;
    (*pvTV)[1] = 1.;
    goto L_pt_uvr;
  }


  // intersection points with V-parameter values on 1. V-curve
  for (i1=0; i1<cvNrU; ++i1) {
    // get startpoints of all U-curves
    rc = UCBS_EvPtBspCrv (&((*xpTab)[i1*cvNrV]), &(bspTU[i1]), 0.0, workSeg);
    if (rc < 0) return -1;
    nxp = 10;	  
    // rc = UT3D_pt_projptbspl (&nxp, xpT, pvT, workSeg, &(bspTV[0]),
    rc = UT3D_pt_projptbspl (&nxp, xpT, pvT, &(bspTV[0]),
		             &((*xpTab)[i1*cvNrV]));
    if (rc < 0) return -1;
    if (nxp < 1) goto L_InErr;
    (*pvTV)[i1] = pvT[0];
      // UT3D_stru_dump (Typ_PT, &xpT[0], " U%d-V0 pvTV=%f",i1,(*pvTV)[i1]);
  }


  // remaining intersection points
  L_pt_uvr:
  for (i1=1; i1<cvNrU; ++i1) {
    k1 = i1 * cvNrV;	  
    for (i2=1; i2<cvNrV; ++i2) {
      rc = UCBS_EvPtBspCrv (&((*xpTab)[k1+i2]), &(bspTU[i1]), (*pvTU)[i2],
		            workSeg);
      if (rc < 0) return -1;
        // UT3D_stru_dump (Typ_PT, &((*xpTab)[k1+i2]), " V0-U%d pt[%d]",i2,k1+i2);
    }
  }

  return 0;


  L_InErr:
    TX_Error("UCBS_XPtsBspCrvNet: input error");
    return -2;

  L_outOfSpace:
    TX_Error("UCBS_XPtsBspCrvNet: out of space");
    return -3;
}


/*=======================================================================*/
  int UCBS_BspCrvPts (CurvBSpl *bsp, Memspc *memSeg, int np, Point *pTab,
                      int deg, int pvType, Memspc *workSeg) {
/*==================
UCBS_BspCrvPts     Bspline curve <-- points 

UCBS_BspCrvPts      Author: Thomas Backmeister       13.6.2007

Compute a bspline curve through a given set of points.
Parameter- and knotvector are computed inside the function.
An optimized parameter vector takes care of the distances between
the input points.

IN:
  int np          ... number of points to interpolate
  Point *pTab     ... array of points to interpolate
  int deg         ... degree of bspline curve (< min{20,np}!)
  int pvType      ... type of parameter vector
                      (0/1 <=> standard uniform / optimized)
                      optimized=genauer, aber f GordonSurfs nicht verwendbar
  Memspc *workSeg ... workspace
OUT:
  CurvBSpl *bsp   ... bspline curve (bsp->ptNr = np) 
  Memspc *memSeg  ... data space of bsp
Returncodes:
  0 = OK
 -1 = out of space
 -2 = internal error
*/

  int rc;
  double *pvTab;
  void *memStart;


  // printf("UCBS_BspCrvPts np=%d deg=%d typ=%d\n",np,deg,pvType);

  // start of workspace
  memStart = workSeg->next;

  // space for controlpoints
  bsp->cpTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(Point) * np);
  if (rc < 0) goto L_outOfSpace;

  // parameter values of points to interpolate (np)
  // (must be strongly increasing)
  rc = UBS_ParVecPts (&pvTab, workSeg, np, pTab, pvType);
  if (rc < 0) return -2;

  // knotvector of bspline curve (np + deg + 1)
  rc = UBS_KntVecParVec (&(bsp->kvTab), memSeg, deg, np, pvTab);
  if (rc < 0) return -2;

  // init curve
  bsp->deg = deg;
  bsp->ptNr = np;
  bsp->v0 = bsp->kvTab[deg];
  bsp->v1 = bsp->kvTab[np];

  // controlpoints of interpolating bspline curve
  rc = UCBS_CtrlPtsIntpol (bsp->cpTab, np, pTab, deg, bsp->kvTab,
	                   pvTab, workSeg);
  if (rc < 0) return -2;

  // release workspace
  workSeg->next = memStart;

  return 0;

L_outOfSpace:
  TX_Error("UCBS_BspCrvPts: out of space");
  return -1;
}


/*=======================================================================*/
  int UCBS_CtrlPtsIntpol (Point *cpTab, int np, Point *pTab, int deg,
                          double *kvTab, double *pvTab, Memspc *workSeg) {
/*======================
UCBS_CtrlPtsIntpol     Controlpoints of interpolating bsp.crv. <-- points 

UCBS_CtrlPtsIntpol      Author: Thomas Backmeister       18.6.2007

Compute the controlpoints of an interpolating bspline curve through
a given set of points.
In- and output-array may be the same.
The space for the controlpoints must be provided by the calling function.

IN:
  int np          ... number of points to interpolate
  Point *pTab     ... array of points to interpolate
  int deg         ... degree of bspline curve (< min{20,np}!)
  double *kvTab   ... knotvector of bspline curve (np + deg + 1) 
  double *pvTab   ... parameter values of points to interpolate (np)
                      (not out of limits of kvTab and strongly increasing)
  Memspc *workSeg ... workspace
OUT:
  Point *cpTab    ... array of control points (np) 
Returncodes:
  0 = OK
 -1 = out of workspace
 -2 = internal error
*/

  int rc, i1, j1;
  double *wTab;
  double *p1Tab, *cp1Tab;
  void *memStart;

  //printf("UCBS_CtrlPtsIntpol: Ctrl. pts. of interp. bsp.crv. <-- points\n");

  // check if points to interpolate are all equal
  if (1 == UT3D_CompPts (np, pTab, UT_TOL_pt)) {
    for (i1=0; i1<np; ++i1) cpTab[i1] = pTab[i1];
    goto L_End;
  }

  // --- provide WORKSPACE: --- ---
  memStart = workSeg->next;
  // for weights
  wTab = workSeg->next;
  rc = UME_add (workSeg, sizeof(double) * np);
  if (rc < 0) goto L_outOfWorkSpace;

  // for 1 component of input points
  p1Tab = workSeg->next;
  rc = UME_add (workSeg, sizeof(double) * np);
  if (rc < 0) goto L_outOfWorkSpace;

  // for 1 component of controlpoints
  cp1Tab = workSeg->next;
  rc = UME_add (workSeg, sizeof(double) * np);
  if (rc < 0) goto L_outOfWorkSpace;
  // --- --- --- --- --- --- ---

  // positive weights for points
  wTab[0] = 10000.0;
  for (i1=1; i1<np-1; ++i1) wTab[i1] = 1.0;
  wTab[np-1] = 10000.0;

  // --- COMPONENTWISE INTERPOLATION --- ---
  for (j1=0; j1<3; ++j1) {
    if (j1 == 0) for (i1=0; i1<np; ++i1) p1Tab[i1] = pTab[i1].x;
    if (j1 == 1) for (i1=0; i1<np; ++i1) p1Tab[i1] = pTab[i1].y;
    if (j1 == 2) for (i1=0; i1<np; ++i1) p1Tab[i1] = pTab[i1].z;
    rc = UT3D_bspl_1Dl2appr (cp1Tab, np, pvTab, p1Tab, wTab,
                             deg+1, np, kvTab, workSeg);
    if (rc < 0) return -2;
    if (j1 == 0) for (i1=0; i1<np; ++i1) cpTab[i1].x = cp1Tab[i1];
    if (j1 == 1) for (i1=0; i1<np; ++i1) cpTab[i1].y = cp1Tab[i1];
    if (j1 == 2) for (i1=0; i1<np; ++i1) cpTab[i1].z = cp1Tab[i1];
  }	
  // --- --- --- --- --- --- --- --- --- ---

  // release workspace
  workSeg->next = memStart;

L_End:
  return 0;

L_outOfWorkSpace:
  TX_Error("UCBS_CtrlPtsIntpol: out of workspace");
  return -1;
}


/*=======================================================================*/
  int UCBS_CpyBspCrv (CurvBSpl *bspo, Memspc *memSeg, CurvBSpl *bspi) {
/*==================
UCBS_CpyBspCrv     Copy <-- bspline curve 

UCBS_CpyBspCrv    Author: Thomas Backmeister       4.5.2007

Copy a bspline curve.

IN:
  CurvBSpl *bspi   ... bspline curve
OUT:
  CurvBSpl *bspo   ... copy of bspi
  Memspc *memSeg   ... data space of bspo
Returncodes:
  0 = OK
 -1 = out of space
*/

  int rc, m, i1;

  //printf("UCBS_CpyBspCrv: Copy <-- bspline curve\n");

  // copy the primary-struct
  *bspo = *bspi;

  // knotvector of bspo
  m = bspo->ptNr + bspo->deg + 1;
  bspo->kvTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * m);
  if (rc < 0) goto L_outOfSpace;
  for (i1=0; i1<m; ++i1) bspo->kvTab[i1] = bspi->kvTab[i1];
  // memcpy (bspo->kvTab, bspi->kvTab, sizeof(double) * m);

  // controlpoints of bspo
  bspo->cpTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(Point) * bspo->ptNr);
  if (rc < 0) goto L_outOfSpace;
  for (i1=0; i1<bspo->ptNr; ++i1) bspo->cpTab[i1] = bspi->cpTab[i1];
  // memcpy (bspo->cpTab, bspi->cpTab, sizeof(Point) * bspo->ptNr);
  
  return 0;

L_outOfSpace:
  TX_Error("UCBS_CpyBspCrv: out of space");
  return -1;
}


/*=======================================================================*/
  int UCBS_DifKntsKntVecBspCrv (int *nk, double **nkTab, Memspc *memSeg,
    		                int mk, double *mkTab, CurvBSpl *bsp) {
/*============================
UCBS_DifKntsKntVecBspCrv   Difference knots <-- knotvector, bspline curve 

UCBS_DifKntsKntVecBspCrv    Author: Thomas Backmeister       3.5.2007

Compute the difference of knotvectors mkTab - bsp->kvTab.
It is assumed that each knot in bsp->kvTab also occurs in
mkTab, considering multiplicities.
The difference knots are all knots in mkTab which are not in
bsp->kvTab, e.g.:
(0,0,0,0,1,1,2,2,2,3,4,4,4,4) - (0,0,0,0,1,2,2,4,4,4,4) = (1,2,3).

IN:
  int mk          ... number of knots
  double *mkTab   ... array of knots
  CurvBSpl *bsp   ... bspline curve
OUT:
  int nk          ... number of difference knots
  double *nkTab   ... array of difference knots
  Memspc *memSeg  ... data space of nkTab
Returncodes:
  0 = OK
 -1 = out of space
*/

  int i1, i2, pm, m1;
  void *rp;

  //printf("UCBS_DifKntsKntVecBspCrv: Diff. knots <-- knotvec., bspl. crv.\n");

  // init
  *nk = 0;
  *nkTab = memSeg->next;

  m1 = bsp->ptNr + bsp->deg + 1;
  pm = 0;
  for (i1=0; i1<m1; ++i1) {
    for (i2=pm; i2<mk; ++i2) {
      if (UTP_comp2db (bsp->kvTab[i1], mkTab[i2], UT_TOL_min1)) {
	if (i2-pm > 0) {      
          rp = UME_save (memSeg, &(mkTab[pm]), sizeof(double)*(i2-pm));
          if (rp == NULL) goto L_outOfSpace;
          *nk += (i2-pm);
        }
	pm = i2 + 1;
	break;
      }
    }
  }
  if (mk-pm > 0) { 
    rp = UME_save (memSeg, &(mkTab[pm]), sizeof(double)*(mk-pm));
    if (rp == NULL) goto L_outOfSpace;
    *nk += (mk-pm);
  }

  return 0;

L_outOfSpace:
  TX_Error("UCBS_DifKntsKntVecBspCrv: out of space");
  return -1;
}


/*=======================================================================*/
  int UCBS_MrgKntVecBspCrvs (int *mk, double **mkTab, Memspc *memSeg, 
		             int nbs, CurvBSpl *bspTab, Memspc *workSeg) {
/*=========================
UCBS_MrgKntVecBspCrvs      Merge knotvectors <-- bspline curves 

UCBS_MrgKntVecBspCrvs     Author: Thomas Backmeister       2.5.2007

Merge the knotvectors of an array of bspline curves.
The curves may have any parameter intervals.

IN:
  int nbs           ... number of bspline curves
  CurvBSpl *bspTab  ... array of bspline curves
  Memspc *workSeg   ... workspace
OUT:
  int *mk           ... number of knots in mkTab
  double **mkTab    ... knotvector from merging
  Memspc *memSeg    ... data space of mkTab
Returncodes:
  0 = OK
 -1 = out of space
 -2 = out of workspace
*/

  int rc, *p, *m, i1, done;
  double umin;
  void *memStart, *rp;

  //printf("UCBS_MrgKntVecBspCrvs: Merge knotvectors <-- bspline curves\n");

  // init
  *mk = 0;
  *mkTab = memSeg->next;

  // save start of workspace
  memStart = workSeg->next;

  // workspace and init
  m = workSeg->next;  // number of knots of each knotvector
  rc = UME_add (workSeg, sizeof(int) * nbs);
  if (rc < 0) goto L_outOfWorkSpace;
  for (i1=0; i1<nbs; ++i1) m[i1] = bspTab[i1].ptNr + bspTab[i1].deg + 1;
  p = workSeg->next;  // pointer for each knotvector
  rc = UME_add (workSeg, sizeof(int) * nbs);
  if (rc < 0) goto L_outOfWorkSpace;
  for (i1=0; i1<nbs; ++i1) p[i1] = 0;

  // -------------
  // compute mkTab
  // -------------
  while (1) {
    // check if done
    done = 1;
    for (i1=1; i1<nbs; ++i1) if (p[i1] < m[i1]) done = 0;
    if (done == 1) break;

    // minimal value of current knot positions
    umin = UT_VAL_MAX;
    for (i1=0; i1<nbs; ++i1) {
      if (p[i1] >= m[i1]) continue;
      umin = DMIN (umin, bspTab[i1].kvTab[p[i1]]);
    }

    // new knot into mkTab
    rp = UME_save (memSeg, &umin, sizeof(double));
    if (rp == NULL) goto L_outOfSpace;
    ++(*mk);

    // move all pointers with value umin
    for (i1=0; i1<nbs; ++i1) {
      if (p[i1] >= m[i1]) continue;
      if (UTP_comp2db (umin, bspTab[i1].kvTab[p[i1]], UT_TOL_min1))
        ++p[i1];
    }
  }
  // ----------------------------

  // release workspace
  workSeg->next = memStart;

  return 0;

L_outOfSpace:
  TX_Error("UCBS_MrgKntVecBspCrvs: out of space");
  return -1;

L_outOfWorkSpace:
  TX_Error("UCBS_MrgKntVecBspCrvs: out of workspace");
  return -2;
}


/*=======================================================================*/
  int UCBS_RefKntVecBspCrv (CurvBSpl *bspo, Memspc *memSeg, CurvBSpl *bspi,
		            int nk, double *nkTab) {
/*========================
UCBS_RefKntVecBspCrv      Refine knotvector <-- bspline curve 

UCBS_RefKntVecBspCrv     Author: Thomas Backmeister       2.5.2007

Refine the knotvector of a bspline curve.
The function works with any parameter interval [a,b] of the curve.
The new knots must be in (a,b).
A new knot may already occur in the curve's knotvector.

IN:
  CurvBSpl *bspi  ... bspline curve
  int nk          ... number of new knots (>0)
  double *nkTab   ... array of new knots
OUT:
  CurvBSpl *bspo  ... bspline curve with additional knots
  Memspc *memSeg  ... data space of bspo
Returncodes:
  0 = OK
 -1 = out of space
*/

  int rc, m, a, b, i, j, k, l, ind;
  double alfa;

  //printf("UCBS_RefKntVecBspCrv: Refine knotvector <-- bspline curve\n");

  // init bspo
  bspo->deg = bspi->deg;
  bspo->ptNr = bspi->ptNr + nk; 
  bspo->v0 = bspi->v0;
  bspo->v1 = bspi->v1;

  // space for knotvector of bspo
  bspo->kvTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * (bspi->ptNr+bspi->deg+1+nk));
  if (rc < 0) goto L_outOfSpace;

  // space for controlpoints of bspo
  bspo->cpTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(Point) * bspo->ptNr);
  if (rc < 0) goto L_outOfSpace;

  // refine knotvector with nkTab
  m = bspi->ptNr + bspi->deg;
  a = UBS_FdSpnKntVec (bspi->kvTab, bspi->deg, bspi->ptNr, nkTab[0]);
  b = UBS_FdSpnKntVec (bspi->kvTab, bspi->deg, bspi->ptNr, nkTab[nk-1]);
  b = b + 1;
  for (j=0; j<=a-bspi->deg; j++) bspo->cpTab[j] = bspi->cpTab[j];
  for (j=b-1; j<bspi->ptNr; j++) bspo->cpTab[j+nk] = bspi->cpTab[j];
  for (j=0; j<=a; j++) bspo->kvTab[j] = bspi->kvTab[j];
  for (j=b+bspi->deg; j<=m; j++) bspo->kvTab[j+nk] = bspi->kvTab[j];
 
  i = b + bspi->deg - 1;  k = b + bspi->deg + nk - 1;
  for (j=nk-1; j>=0; j--) {
    while (nkTab[j] <= bspi->kvTab[i] && i > a) {
      bspo->cpTab[k-bspi->deg-1] = bspi->cpTab[i-bspi->deg-1];
      bspo->kvTab[k] = bspi->kvTab[i];
      --k;  --i;
    }
    bspo->cpTab[k-bspi->deg-1] = bspo->cpTab[k-bspi->deg];
    for (l=1; l<=bspi->deg; l++) {
      ind = k - bspi->deg + l;
      alfa = bspo->kvTab[k+l] - nkTab[j];    
      if (UTP_comp_0 (alfa)) bspo->cpTab[ind-1] = bspo->cpTab[ind];
      else {
        alfa = alfa / (bspo->kvTab[k+l] - bspi->kvTab[i-bspi->deg+l]);
        UT3D_pt_LinComb2Pts (&(bspo->cpTab[ind-1]), alfa, &(bspo->cpTab[ind-1]),
	                  (1.0-alfa), &(bspo->cpTab[ind]));
      }
    }
    bspo->kvTab[k] = nkTab[j];
    --k;
  }

  return 0;

L_outOfSpace:
  TX_Error("UCBS_RefKntVecBspCrv: out of space");
  return -1;
}


/*=======================================================================*/
  int UCBS_NrmKntVecBspCrv (CurvBSpl *bspo, Memspc *memSeg, CurvBSpl *bspi) {
/*========================
UCBS_NrmKntVecBspCrv      Normalize knotvector <-- bspline curve 

UCBS_NrmKntVecBspCrv     Author: Thomas Backmeister       1.5.2007

Normalize the knotvector of a bspline curve to interval [0,1].

IN:
  CurvBSpl *bspi  ... bspline curve
OUT:
  CurvBSpl *bspo  ... bspline curve with knots in [0,1]
  Memspc *memSeg  ... data space of bspo
Returncodes:
  0 = OK
 -1 = out of space
*/

  int rc, m, i1;

  //printf("UCBS_NrmKntVecBspCrv: Normalize knotvector <-- bspline curve\n");

  // init bspo
  bspo->deg = bspi->deg;
  bspo->ptNr = bspi->ptNr; 

  // space for knotvector of bspo
  m = bspi->ptNr + bspi->deg;
  bspo->kvTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * (m+1));
  if (rc < 0) goto L_outOfSpace;

  // space for controlpoints of bspo
  bspo->cpTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(Point) * bspi->ptNr);
  if (rc < 0) goto L_outOfSpace;
  
  // normalize knotvector
  for (i1=0; i1<m+1; ++i1) 
    bspo->kvTab[i1] = (bspi->kvTab[i1] - bspi->kvTab[0]) /
	              (bspi->kvTab[m] - bspi->kvTab[0]);  

  // adjust start / end parameter
  bspo->v0 = (bspi->v0 - bspi->kvTab[0]) /
	     (bspi->kvTab[m] - bspi->kvTab[0]);
  bspo->v1 = (bspi->v1 - bspi->kvTab[0]) /
	     (bspi->kvTab[m] - bspi->kvTab[0]);

  // copy control points
  for (i1=0; i1<bspi->ptNr; ++i1)
    bspo->cpTab[i1] = bspi->cpTab[i1];	  

  return 0;

L_outOfSpace:
  TX_Error("UCBS_NrmKntVecBspCrv: out of space");
  return -1;
}


/*=======================================================================*/
  int UCBS_DegElevBspCrv (CurvBSpl *bspo, Memspc *memSeg, CurvBSpl *bspi,
                          int dup) {
/*======================
UCBS_DegElevBspCrv      Degree elevate <-- bspline curve 

UCBS_DegElevBspCrv     Author: Thomas Backmeister       30.4.2007

Degree elevation of a bspline curve.
The function works with any parameter interval [a,b] of the curve.

IN:
  CurvBSpl *bspi  ... bspline curve
  int dup (>= 1)  ... degree elevation deg --> deg + dup
OUT:
  CurvBSpl *bspo  ... degree elevated bspline curve
  Memspc *memSeg  ... data space of bspo (and temporary workspace)
Returncodes:
  0 = OK
 -1 = out of space
*/

  int rc, i, j, k, m, s1, s, ph2, mpi, mh, kind, cind;
  int lbz, rbz, RBZ, r, a, b, mul, oldr, save, first, last;
  int tr, kj;
  double ko, *alphas, inv, ua, ub, numer, alf, alfj;
  void *memStart;
  Polynom_ bezalfs;
  Point *bpts, *ebpts, *nextbpts;

  //printf("UCBS_DegElevBspCrv: Degree elevate <-- bspline curve\n");

  // m+1 = number of knots of bspi
  m = bspi->ptNr + bspi->deg;

  // number of different interior knots of bspi
  s1 = 0;
  ko = bspi->kvTab[0];
  for (i=bspi->deg+1; i<=bspi->ptNr-1; ++i) {
    if (bspi->kvTab[i] > ko) {
      ++s1;
      ko = bspi->kvTab[i];
    }
  }

  // init bspo
  bspo->deg = bspi->deg + dup;
  bspo->ptNr = bspi->ptNr + (s1 + 1) * dup; 
  bspo->v0 = bspi->v0;
  bspo->v1 = bspi->v1;

  // space for knotvector of bspo
  bspo->kvTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * (m+(s1+2)*dup+1));
  if (rc < 0) goto L_outOfSpace;

  // space for controlpoints of bspo
  bspo->cpTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(Point) * bspo->ptNr);
  if (rc < 0) goto L_outOfSpace;

  // save start of workspace
  memStart = memSeg->next;

  // workspace
  bezalfs.dim1 = bspo->deg + 1;
  bezalfs.dim2 = bspi->deg + 1;
  bezalfs.pol = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * bezalfs.dim1 * bezalfs.dim2);
  if (rc < 0) goto L_outOfSpace;
  bpts = memSeg->next;
  rc = UME_add (memSeg, sizeof(Point) * (bspi->deg+1));
  if (rc < 0) goto L_outOfSpace;
  ebpts = memSeg->next;
  rc = UME_add (memSeg, sizeof(Point) * (bspo->deg+1));
  if (rc < 0) goto L_outOfSpace;
  nextbpts = memSeg->next;
  rc = UME_add (memSeg, sizeof(Point) * (bspi->deg-1));
  if (rc < 0) goto L_outOfSpace;
  alphas = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * (bspi->deg-1));
  if (rc < 0) goto L_outOfSpace;
   
  // Bezier degree elevation coefficients
  ph2 = bspo->deg / 2.0;
  bezalfs.pol[0] = bezalfs.pol[bspo->deg*bezalfs.dim2+bspi->deg] = 1.0;
  for (i=1; i<=ph2; i++) {
    inv = 1.0 / UTM_db_bincoeff (bspo->deg, i);
    mpi = IMIN(bspi->deg, i);
    for (j=IMAX(0,i-dup); j<=mpi; j++)
      bezalfs.pol[i*bezalfs.dim2+j] = inv * UTM_db_bincoeff (bspi->deg, j) *
                                            UTM_db_bincoeff (dup, i-j);
  }
  for (i=ph2+1; i<=bspo->deg-1; i++) {
    mpi = IMIN(bspi->deg, i);
    for (j=IMAX(0,i-dup); j<=mpi; j++)
      bezalfs.pol[i*bezalfs.dim2+j] = 
      bezalfs.pol[(bspo->deg-i)*bezalfs.dim2+bspi->deg-j];
  }
  ua = bspi->kvTab[0];
  for (i=0; i<=bspo->deg; i++) bspo->kvTab[i] = ua;
  kind = bspo->deg + 1;
  bspo->cpTab[0] = bspi->cpTab[0];
  cind = 1;

  // init first Bezier segment 
  lbz = 1;  r = -1;
  for (i=0; i<=bspi->deg; i++) bpts[i] = bspi->cpTab[i];
  a = bspi->deg;  b = bspi->deg + 1; 

  // -------------------------------------
  // Big loop thru knotvector
  // -------------------------------------
  while (b < m) { 
    i = b;
    while (b < m && 
           UTP_comp2db (bspi->kvTab[b], bspi->kvTab[b+1], UT_TOL_min1)) ++b;
    mul = b - i + 1; 
    ub = bspi->kvTab[b];
    oldr = r;  

    // insert knot ub r times
    r = bspi->deg - mul;
    if (oldr > 0) lbz = (oldr + 2) / 2;
    else lbz = 1;
    if (r > 0) rbz = bspo->deg - (r+1) / 2;
    else rbz = bspo->deg;
    
    if (r > 0) {
      // insert knot to get Bezier segment
      numer = ub - ua;
      for (k=bspi->deg; k>mul; k--) alphas[k-mul-1] = 
	                            numer / (bspi->kvTab[a+k]-ua);
      for (j=1; j<=r; j++) {
        save = r - j;  s = mul + j;
        for (k=bspi->deg; k>=s; k--) {
          alf = alphas[k-s];
	  UT3D_pt_LinComb2Pts (&bpts[k], alf, &bpts[k], (1.0-alf), &bpts[k-1]);
        }
        nextbpts[save] = bpts[bspi->deg];
      }	
    }

    // degree elevate Bezier
    if (r > 0) RBZ = rbz + 1;
    else RBZ = rbz;
    for (i=lbz; i<= RBZ; i++) {
      // only points lbz,...,RBZ are used below
      ebpts[i] = UT3D_PT_NUL;
      mpi = IMIN(bspi->deg, i);
      for (j=IMAX(0,i-dup); j<=mpi; j++) {
        UT3D_pt_LinComb2Pts (&ebpts[i], 1.0, &ebpts[i],
	                  bezalfs.pol[i*bezalfs.dim2+j], &bpts[j]);
      }
    }

    if (oldr > 1) {
      // must remove knot u = bspi->kvTab[a] oldr times
      if (oldr > 2) {
        // alphas on the right do not change
	alfj = (ua - bspo->kvTab[kind-1]) / (ub - bspo->kvTab[kind-1]);
      }
      first = kind - 2;  last = kind;
      for (tr=1; tr<oldr; tr++) {
        // knot removal loop
	i = first;  j = last;
	kj = j - kind + 1;
	// loop and compute the new control points for one removal step
        while (j-i > tr) {
          if (i < cind) {
            alf = (ua - bspo->kvTab[i]) / (ub - bspo->kvTab[i]);
            UT3D_pt_LinComb2Pts (&(bspo->cpTab[i]), (1.0/alf), &(bspo->cpTab[i]),
	                      ((alf-1.0)/alf), &(bspo->cpTab[i-1]));
          }
          if (kj >= lbz) {
	    UT3D_pt_LinComb2Pts (&(ebpts[kj]), 1.0, &(ebpts[kj]),
            		      (alfj/(alfj-1.0)), &(ebpts[kj+1]));
          }
          ++i;  --j;  --kj;
	}	
        --first;  ++last;
      }	      
    } // end of removing knot u = bspi->kvTab[a]

    if (a != bspi->deg) {
      // load the knot ua
      for (i=0; i<bspo->deg-oldr; i++) {
        bspo->kvTab[kind] = ua;
	++kind;
      }
    }

    // load controlpoints into bspo->cpTab
    for (j=lbz; j<=rbz; j++) {
      bspo->cpTab[cind] = ebpts[j];
      ++cind;
    }	    

    if (b < m) {
      // set up for next pass thru loop
      lbz = 1;
      for (j=0; j<r; j++) bpts[j] = nextbpts[j];
      for (j=r; j<=bspi->deg; j++) bpts[j] = bspi->cpTab[b-bspi->deg+j];
      a = b;  ++b;  ua = ub;
    }
    else {
      // end knot
      for (i=0; i<=bspo->deg; i++) bspo->kvTab[kind+i] = ub;
    }
  }
  // -------------------------------------
  // End of big loop
  // -------------------------------------

  // release workspace
  memSeg->next = memStart;

  return 0;

L_outOfSpace:
  TX_Error("UCBS_DegElevBspCrv: out of space");
  return -1;
}



/*=======================================================================*/
  int UCBS_DirIndIsoBspSur (CurvBSpl *crv, Memspc *memSeg, int degC,
                            int ptNrC, double *kvTabC, int deg2, int ptNr2,
                            double *kvTab2, Point *cpTab, double t, int uv) {
/*========================
UCBS_DirIndIsoBspSur   Dir. ind. isopar. bspline curve <-- bspline surface 

UCBS_DirIndIsoBspSur     Author: Thomas Backmeister       1.11.2003

Direction-independent computation of an isoparametric bspline curve
on a bspline surface.

IN:
  int degC        ... surface degree 1     (= curve-degree)
  int ptNrC       ... number of surface controlpoints 1
                                           (= number of curve-controlpoints)
  double *kvTabC  ... surface knotvector 1 (= curve-knotvector)
  int deg2        ... surface degree 2
  int ptNr2       ... number of surface controlpoints 2
  double *kvTab2  ... surface knotvector 2
  Point *cpTab    ... surface controlpoints
  double t        ... isoparameter
  int uv          ... 1: t=u-value; 2: t=v-value
OUT:
  CurvBSpl *crv   ... bspline curve to isoparameter
  Memspc *memSeg  ... data space of curve (and temporary workspace)
Returncodes:
  0 = OK
 -1 = internal error
 -2 = out of space
*/

  int rc, i1, i2;
  void *memStart;
  CurvBSpl crvh;

  //printf("UCBS_DirIndIsoBspSur: Dir. ind. isop. bspl. crv. <-- bspl. surf.\n");

  // init isocurve
  crv->deg = degC;
  crv->ptNr = ptNrC;
  crv->v0 = kvTabC[degC];
  crv->v1 = kvTabC[ptNrC];

  // knotvector of isocurve
  crv->kvTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * (ptNrC+degC+1));
  if (rc < 0) goto L_outOfSpace;
  for (i1=0; i1<=ptNrC+degC; ++i1) crv->kvTab[i1] = kvTabC[i1];

  // space for controlpoints of isocurve
  crv->cpTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(Point) * ptNrC);
  if (rc < 0) goto L_outOfSpace;

  // save start of workspace
  memStart = memSeg->next;

  // bsplcrv in counterdirection 
  crvh.deg = deg2;
  crvh.ptNr = ptNr2;
  crvh.kvTab = kvTab2;

  if (uv == 2) {
    // space for controlpoints of counterdir.-curve
    crvh.cpTab = memSeg->next;
    rc = UME_add (memSeg, sizeof(Point) * ptNr2);
    if (rc < 0) goto L_outOfSpace;
  }

  for (i1=0; i1<ptNrC; ++i1) {

    // controlpoints of counterdir.-curve
    if (uv == 1) crvh.cpTab = &(cpTab[i1*ptNr2]);
    else {
      for (i2=0; i2<ptNr2; ++i2) crvh.cpTab[i2] = cpTab[i1+i2*ptNrC];
    }

    // evaluate point on counterdir.-curve
    rc = UCBS_PtUBspCrv (&(crv->cpTab[i1]), &crvh, t, memSeg);
    if (rc < 0) return -1;
  }

  // release workspace
  memSeg->next = memStart;

  return 0;

L_outOfSpace:
  TX_Error("UCBS_DirIndIsoBspSur: out of space");
  return -2;
}



/*=======================================================================*/
  int UCBS_IsoBspCrvBspSur (CurvBSpl *crv, Memspc *memSeg, SurBSpl *sur,
                            double t, int uv) {
/*========================
UCBS_IsoBspCrvBspSur    Isoparametric bspline curve <-- bspline surface 

UCBS_IsoBspCrvBspSur     Author: Thomas Backmeister       1.11.2003

Computation of an isoparametric bspline curve on a bspline surface.

// see also UT3D_bspU_sbsp_v UT3D_bspV_sbsp_u
// see also UT3D_bsp_sbspU UT3D_bsp_sbspV

IN:
  SurBSpl *sur    ... bspline surface
  double t        ... isoparameter
  int uv          ... 1: t=u-value; 2: t=v-value
OUT:
  CurvBSpl *crv   ... bspline curve to isoparameter t
  Memspc *memSeg  ... data space of curve (and temporary workspace)
Returncodes:
  0 = OK
 -1 = internal error
*/

  int rc;

  //printf("UCBS_IsoBspCrvBspSur: Isopar. bspl. curve <-- bspline surface\n");

  if (uv == 1) {

    // U-isoparametric curve
    rc = UCBS_DirIndIsoBspSur (crv, memSeg,
                               sur->degV, sur->ptVNr, sur->kvTabV,
                               sur->degU, sur->ptUNr, sur->kvTabU,
                               sur->cpTab, t, uv);
    if (rc < 0) return -1;
  }
  else {

    // V-isoparametric curve
    rc = UCBS_DirIndIsoBspSur (crv, memSeg,
                               sur->degU, sur->ptUNr, sur->kvTabU,
                               sur->degV, sur->ptVNr, sur->kvTabV,
                               sur->cpTab, t, uv);
    if (rc < 0) return -1;
  }

  return 0;
}


/*=======================================================================*/
  int UCBS_PtUBspCrv (Point *pt, CurvBSpl *bspl, double u, Memspc *workSeg) {
/*==================
UCBS_PtUBspCrv   Point <-- U-parameter bspline curve

UCBS_PtUBspCrv    Author: Thomas Backmeister       12.9.2003

Evaluate a point on a bspline curve.
The underlying algorithm is "de Boor".
The point is computed for any parameter value within the maximal
support of the curve (we do not consider the limit parameters
bspl->v0 and bspl->v1).

This function evaluates without using polynomials.
(in contrast to UT3D_pt_evalparCv)
It needs work space of size (curve degree * sizeof(Point)).

IN:
  CurvBSpl *bspl  ... bspline curve
  double u        ... parameter value
  Memspc *workSeg ... work space (control points)
OUT:
  Point *pt       ... curve point
Returncodes:
  0 = OK
 -1 = out of work space
 -2 = input error
*/

  int rc, d, n, m, r, s;
  int i1, i2, ih, jh, n1;
  double a, b;
  void *memstart;
  Point *ptab;

  //printf("UCBS_PtUBspCrv: Point <-- U-parameter bspline curve\n");

  // some abbreviations
  d = bspl->deg;
  n = bspl->ptNr - 1;
  m = n + d + 1;

  // check parameter
  if (u < bspl->kvTab[d]) {
    // printf("UCBS_PtUBspCrv: parameter below %f\n",u);
    u = bspl->kvTab[d];
  } 
  if (u > bspl->kvTab[bspl->ptNr]) {
    // printf("UCBS_PtUBspCrv: parameter above %f\n",u);
    u = bspl->kvTab[bspl->ptNr];
  }

  // determine segment of parameter
  r = -1;
  for (s=d; s<m-d; ++s) {
    if (bspl->kvTab[s] <= u && u <= bspl->kvTab[s+1]) {
      r = s;
      break;
    }
  }
  if (r == -1) {
    printf("UCBS_PtUBspCrv: input error\n");
    return -2;
  }

  // provide work space
  memstart = workSeg->next;
  ptab = workSeg->next;
  rc = UME_add (workSeg, sizeof(Point) * (d+1));
  if (rc < 0) goto L_outOfWorkSpace;
 
  // init control points
  ih = r - d;
  for (i1=0; i1<d+1; ++i1) ptab[i1] = bspl->cpTab[ih+i1];

  // compute curve point
  n1 = d;
  for (i1=0; i1<d; ++i1) {
    ih = r + 1;
    jh = ih - n1;
    for (i2=0; i2<n1; ++i2) {
      a = (u - bspl->kvTab[jh+i2]) / (bspl->kvTab[ih+i2] - bspl->kvTab[jh+i2]);
      b = 1.0 - a;
      ptab[i2].x = b * ptab[i2].x + a * ptab[i2+1].x;
      ptab[i2].y = b * ptab[i2].y + a * ptab[i2+1].y;
      ptab[i2].z = b * ptab[i2].z + a * ptab[i2+1].z;
    }
    --n1;
  }
  *pt = ptab[0];

  // release work space
  workSeg->next = memstart;

  return 0;

L_outOfWorkSpace:
  TX_Error("UCBS_PtUBspCrv: out of work space");
  return -1;
}


/*=======================================================================*/
  int UCBS_OffSurBspCrv (CurvBSpl *offbspl, Memspc *memSeg1, CurvBSpl *bspl,
		         SurBSpl *sur, double offd, Memspc *memSegT[4],
		       	 Memspc *workSeg) {
/*=====================
UCBS_OffSurBspCrv    Offset surface bspline curve <-- bspline curve     

UCBS_OffSurBspCrv     Author: Thomas Backmeister       19.1.2007

Given a bspline curve lying on a bspline surface a new bspline curve with
same degree, knotvector and number of control points is computed such that
for every parameter the difference vector of the corresponding curve points
is normal to the supporting bspline surface.

IN:
  CurvBSpl *bspl     ... bspline curve
  SurBSpl *sur       ... supporting bspline surface
  double offd        ... offset distance
  Memspc *memSegT[4] ... temporary workspace
  Memspc *workSeg    ... temporary workspace
OUT:
  CurvBSpl *offbspl  ... offset bspline curve
  Memspc *memSeg1    ... data space of offbspl
Returncodes:
  0 = OK
 -1 = input error
 -2 = internal error
 -3 = out of space
*/

  int rc, i1, i2, m;
  double t1;
  Point pt;
  Vector nv;

  //printf("\nUCBS_OffSurBspCrv: Off. sur. bspline curve <-- bspline curve\n");

  //UT3D_stru_dump( 10, bspl, "" );  // bspline curve
  //UT3D_stru_dump( 158, sur, "" );  // bspline surface
  
  // init offbspl
  offbspl->ptNr = bspl->ptNr;
  offbspl->deg = bspl->deg;
  offbspl->v0 = bspl->v0;
  offbspl->v1 = bspl->v1;

  // knotvector of offbspl
  m = offbspl->ptNr + offbspl->deg;   // knotvector =: (t0, ..., tm)
  offbspl->kvTab = memSeg1->next;
  rc = UME_add (memSeg1, sizeof(double) * (m+1));
  if (rc < 0) goto L_outOfSpace;
  for (i1=0; i1<m+1; ++i1) offbspl->kvTab[i1] = bspl->kvTab[i1];
  
  // controlpoints of offbspl
  offbspl->cpTab = memSeg1->next;
  rc = UME_add (memSeg1, sizeof(Point) * offbspl->ptNr);
  if (rc < 0) goto L_outOfSpace;
  for (i1=0; i1<offbspl->ptNr; ++i1) offbspl->cpTab[i1] = bspl->cpTab[i1];

  // Move controlpoints with corresponding parameters in surface
  // normal direction
  for (i1=0; i1<bspl->ptNr; ++i1) {
    t1 = 0.0;
    for (i2=1; i2<=bspl->deg; ++i2) t1 = t1 + bspl->kvTab[i1+i2]; 	  
    t1 = t1 / (double)bspl->deg;
    rc = UT3D_pt_evalparCv (&pt, bspl, t1);
    if (rc < 0) return -2;
    rc = USBS_NvPtBspSur (&nv, sur, &pt, memSegT, workSeg);
    if (rc < 0) return -2;
    UT3D_pt_traptvclen (&(offbspl->cpTab[i1]), &(offbspl->cpTab[i1]),
		        &nv, offd); 
  }

  return 0;
 
L_outOfSpace:
  TX_Error("UCBS_OffSurBspCrv: out of space");
  return -3;
}


/*=======================================================================*/
  int UCBS_SubDivBspCrv (CurvBSpl *bspl1, Memspc *memSeg1,
                         CurvBSpl *bspl2, Memspc *memSeg2,
                         CurvBSpl *bspl, double t) {
/*=====================
UCBS_SubDivBspCrv    Subdivision <-- bspline curve 

UCBS_SubDivBspCrv     Author: Thomas Backmeister       22.10.2003

Subdivide a bspline curve into 2 bspline curves.
The composition of the two resulting curves is equal to the original curve.

IN:
  CurvBSpl *bspl  ... bspline curve
  double t        ... subdivision parameter
OUT:
  CurvBSpl *bspl1 ... bspline curve 1
  Memspc *memSeg1 ... data space of bspl1 (and temporary workspace)
  CurvBSpl *bspl2 ... bspline curve 2
  Memspc *memSeg2 ... data space of bspl2
Returncodes:
  0 = OK
 -1 = input error
 -2 = internal error
 -3 = out of space
*/

  int rc, m, r;

  //printf("\nUCBS_SubDivBspCrv: Subdivision <-- bspline curve\n");

  // check subdivision parameter
  if (t <= bspl->v0) {
    TX_Error("UCBS_SubDivBspCrv: divpar %f <= startpar %f not valid",
              t, bspl->v0);
    return -1;
  }
  if (t >= bspl->v1) {
    TX_Error("UCBS_SubDivBspCrv: divpar %f >= endpar %f not valid",
             t, bspl->v1);
    return -1;
  }

  // an abbreviation
  m = bspl->ptNr + bspl->deg;  // knotvector =: (t0, ..., tm)

  // init bspl1 and bspl2
  bspl1->deg = bspl->deg;
  bspl1->v0 = bspl->v0;
  bspl1->v1 = t;
  bspl2->deg = bspl->deg;
  bspl2->v0 = t;
  bspl2->v1 = bspl->v1;

  // divide knotvector of bspline curve
  rc = UBS_DivKnotVec (&(bspl1->kvTab), memSeg1, &(bspl2->kvTab),
                       memSeg2, &r, bspl->kvTab, m, bspl->deg, t);
  if (rc < 0) return -2;

  // number of controlpoints of bspl1 and bspl2
  bspl1->ptNr = r;
  if (t > bspl->kvTab[r]) ++bspl1->ptNr;
  bspl2->ptNr = m - r;

  // space for controlpoints of bspl1
  bspl1->cpTab = memSeg1->next;
  rc = UME_add (memSeg1, sizeof(Point) * bspl1->ptNr);
  if (rc < 0) goto L_outOfSpace;

  // space for controlpoints of bspl2
  bspl2->cpTab = memSeg2->next;
  rc = UME_add (memSeg2, sizeof(Point) * bspl2->ptNr);
  if (rc < 0) goto L_outOfSpace;

  // divide controlpoints of bspline curve
  rc = UBS_DivCtrlPtsBsp (bspl1->cpTab, bspl2->cpTab, bspl->deg,
                          bspl->kvTab, bspl->ptNr, bspl->cpTab,
                          t, r, 0, 1, memSeg1);
  if (rc < 0) return -2;

  return 0;
 
L_outOfSpace:
  TX_Error("UCBS_SubDivBspCrv: out of space");
  return -3;
}


/*=======================================================================*/
  int UCBS_3BspCrvsBspCrv (CurvBSpl *bsplTab[3], Memspc *memSeg1,
                           CurvBSpl *bspl, Memspc *workSeg) {
/*=======================
UCBS_3BspCrvsBspCrv    3 bspline curves <-- bspline curve 

UCBS_3BspCrvsBspCrv     Author: Thomas Backmeister       29.1.2007

Subdivide a bspline curve into 3 bspline curves of equal length
of parameter range.

IN:
  CurvBSpl *bspl        ... bspline curve
OUT:
  CurvBSpl *bsplTab[3]  ... 3 bspline curves
  Memspc *memSeg1       ... data space of bsplTab
                            (and temporary workspace)
  Memspc *workSeg       ... temporary workspace			    
Returncodes:
  0 = OK
 -1 = input error
 -2 = internal error
 -3 = out of space
*/

  int rc;
  double t;
  void *memStart;
  CurvBSpl bh; 

  printf("\nUCBS_3BspCrvsBspCrv: 3 bspline curves <-- bspline curve\n");

  memStart = workSeg->next;

  t = bspl->v0 + (bspl->v1 - bspl->v0) / 3.0;
  rc = UCBS_SubDivBspCrv (bsplTab[0], memSeg1, &bh, workSeg, bspl, t); 
  if (rc < 0) return rc;
  //UT3D_stru_dump( 10, bsplTab[0], "bsplTab[0]" );
  //UT3D_stru_dump( 10, &bh, "bh" );
  
  t = bh.v0 + (bh.v1 - bh.v0) / 2.0;
  rc = UCBS_SubDivBspCrv (bsplTab[1], memSeg1, bsplTab[2], memSeg1,
                          &bh, t); 
  if (rc < 0) return rc;

  workSeg->next = memStart;

  return 0;
}


/*=======================================================================*/
  int UCBS_Comp2BspCrvs (CurvBSpl *cvo, Memspc *memSeg, CurvBSpl *cv1,
		         CurvBSpl *cv2, double smf, Memspc *workSeg) {
/*=====================
UCBS_Comp2BspCrvs     Compose <-- 2 bspline curves

UCBS_Comp2BspCrvs      Author: Thomas Backmeister       6.9.2007

Compose two bspline curves to one bspline curve.
The curves may have different degrees >= 1 and need not to be 
connected. 
The smoothness of the curve connection can be controlled with
the smooth factor (0...0% smoothness, 1...100% smoothness).

IN:
  CurvBSpl *cv1     ... bspline curve 1
  CurvBSpl *cv2     ... bspline curve 2
  double smf        ... smooth factor in [0,1]
  Memspc *workSeg   ... workspace
OUT:
  CurvBSpl *cvo     ... composite bspline curve
  Memspc *memSeg    ... data space of cvo
Returncodes:
  0 = OK
 -1 = internal error
 -2 = out of space
*/

  int rc, m1, k1, i1, deg2;
  double s1, s2;
  CurvBSpl bsp1, bsp2, bsp3, bsp4;
  Point ptm;
  void *workStart;


  // printf("UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU \n");
  // printf("UCBS_Comp2BspCrvs: Compose <-- 2 bspline curves\n");
  // UT3D_stru_dump(Typ_CVBSP, cv1, "  cv1:");
  // UT3D_stru_dump(Typ_CVBSP, cv2, "  cv2:");


  // save start of workspace
  workStart = workSeg->next;

  // transform knotvector of curve 1 to parameter interval [0,0.5]
  rc = UCBS_TrfKntVecBspCrv (&bsp1, workSeg, cv1, 0.5, 0.0);
  if (rc < 0) return -1;

  // transform knotvector of curve 2 to parameter interval [0.5,1.0]
  rc = UCBS_TrfKntVecBspCrv (&bsp2, workSeg, cv2, 0.5, 0.5);
  if (rc < 0) return -1;

  // make curve degrees equal 
  if (bsp1.deg < bsp2.deg) {
    rc = UCBS_DegElevBspCrv (&bsp3, workSeg, &bsp1, bsp2.deg-bsp1.deg);
    if (rc < 0) return -1;
    bsp4 = bsp2;


  } else if (bsp2.deg < bsp1.deg) {
    rc = UCBS_DegElevBspCrv (&bsp4, workSeg, &bsp2, bsp1.deg-bsp2.deg);
    if (rc < 0) return -1;
    bsp3 = bsp1;


  } else {
    if (bsp1.deg == 1 && smf > 0.0) {
      rc = UCBS_DegElevBspCrv (&bsp3, workSeg, &bsp1, 1);
      if (rc < 0) return -1;
      rc = UCBS_DegElevBspCrv (&bsp4, workSeg, &bsp2, 1);
      if (rc < 0) return -1;

    } else {    
      bsp3 = bsp1;
      bsp4 = bsp2;
    }      
  }	  

  // init cvo
  cvo->ptNr = bsp3.ptNr + bsp4.ptNr - 1;
  cvo->deg = bsp3.deg;
  cvo->v0 = bsp3.v0;
  cvo->v1 = bsp4.v1;

  // space for knotvector of cvo
  cvo->kvTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * (cvo->ptNr + cvo->deg + 1));
  if (rc < 0) goto L_outOfSpace;

  // space for controlpoints of cvo
  cvo->cpTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(Point) * cvo->ptNr);
  if (rc < 0) goto L_outOfSpace;

  // knotvector of cvo (main part)
  for (i1=0; i1<bsp3.ptNr; ++i1) cvo->kvTab[i1] = bsp3.kvTab[i1];
  m1 = bsp3.ptNr + bsp3.deg;
  k1 = bsp4.deg + 1;
  for (i1=0; i1<bsp4.ptNr; ++i1) cvo->kvTab[m1+i1] = bsp4.kvTab[k1+i1];

  // smoothness of curve connection (remaining middle knots)
  deg2 = cvo->deg / 2;
  s1 = smf * (0.5 - cvo->kvTab[bsp3.ptNr-1]) / (double)(deg2 + 1);
  s2 = smf * (cvo->kvTab[m1] - 0.5) / (double)(deg2 + 1);
  k1 = bsp3.ptNr + deg2;
  if (deg2 > 0) cvo->kvTab[k1-1] = 0.5 - s1;
  for (i1=2; i1<=deg2; ++i1) cvo->kvTab[k1-i1] = cvo->kvTab[k1-i1+1] - s1;
  if (cvo->deg % 2 == 0) {
    cvo->kvTab[k1] = 0.5 + s2;
    for (i1=1; i1<deg2; ++i1) cvo->kvTab[k1+i1] = cvo->kvTab[k1+i1-1] + s2; 

  } else {
    cvo->kvTab[k1] = 0.5;
    for (i1=1; i1<=deg2; ++i1) cvo->kvTab[k1+i1] = cvo->kvTab[k1+i1-1] + s2; 
  }	  

  // controlpoints of cvo
  k1 = bsp3.ptNr - 1;
  UT3D_pt_mid2pt (&ptm, &(bsp3.cpTab[k1]), &(bsp4.cpTab[0]));
  for (i1=0; i1<k1; ++i1) cvo->cpTab[i1] = bsp3.cpTab[i1];
  cvo->cpTab[k1] = ptm;
  for (i1=1; i1<bsp4.ptNr; ++i1) cvo->cpTab[k1+i1] = bsp4.cpTab[i1];


  // release workspace
  workSeg->next = workStart;

    // UT3D_stru_dump(Typ_CVBSP, cvo, "ex  cvo:");

  return 0;

L_outOfSpace:
  TX_Error("UCBS_Comp2BspCrvs: out of space");
  return -2;
}


/*=======================================================================*/
  int UCBS_MkeCmpBspCrvs (CurvBSpl *bspTab, Memspc *memSeg, int cvNr,
                          CurvBSpl **cvTab, int deg, Memspc *workSeg) {   
/*======================
UCBS_MkeCmpBspCrvs     Compatible bspline curves <-- bspline curves 

UCBS_MkeCmpBspCrvs      Author: Thomas Backmeister       14.6.2007

Make compatible some bspline curves, i.e. equalize parameter interval,
degree and knotvector of the curves.
The curves must be clamped, i.e. their knotvectors must look like
[a,...,a,u1,...,uk,b,...,b], where a and b occur degree times.

IN:
  int cvNr          ... number of bspline curves (> 1)
  CurvBSpl **cvTab  ... bspline curves
  int deg           ... desired degree for all bspline curves
                        (only effectiv if > max{curve degrees})
  Memspc *workSeg   ... workspace
OUT:
  CurvBSpl *bspTab  ... compatible bspline curves (size >= cvNr !)
  Memspc *memSeg    ... data space of bspTab (and workspace)
Returncodes:
  0 = OK
 -1 = internal error
 -2 = input error
*/

  int      i1, rc, mdeg, mk, nk;
  double   *mkTab, *nkTab;
  void     *memStart, *workStart, *workStart2;
  CurvBSpl bspT2[TabS1];

  //printf("UCBS_MkeCmpBspCrvs: Compatible bspline curves <-- bspline curves\n");

  if (TabS1 < cvNr) goto L_InputErr;

  // save start of mems
  memStart = memSeg->next;
  workStart = workSeg->next;

  // normalize all knotvectors to parameter interval [0,1]
  for (i1=0; i1<cvNr; ++i1) {
    rc = UCBS_TrfKntVecBspCrv (&(bspTab[i1]), memSeg, cvTab[i1], 1.0, 0.0);
    if (rc < 0) return -1;
  }

  // maximum of all curve degrees
  mdeg = bspTab[0].deg;
  if (deg > mdeg) mdeg = deg;
  for (i1=1; i1<cvNr; ++i1)
    if (bspTab[i1].deg > mdeg) mdeg = bspTab[i1].deg;

  // degree elevation for all curves with deg < mdeg
  for (i1=0; i1<cvNr; ++i1) {
    if (bspTab[i1].deg == mdeg)
      UCBS_CpyBspCrv (&(bspT2[i1]), workSeg, &(bspTab[i1]));
    else {
      rc = UCBS_DegElevBspCrv (&(bspT2[i1]), workSeg, &(bspTab[i1]),
                               mdeg-bspTab[i1].deg);
      if (rc < 0) return -1;
    }
  }

  // release memSpace
  memSeg->next = memStart;

  // merge all knotvectors --> mkTab
  rc = UCBS_MrgKntVecBspCrvs (&mk, &mkTab, workSeg, cvNr, bspT2, memSeg);
  if (rc < 0) return -1;

  // refine all curve knotvectors to mkTab
  workStart2 = workSeg->next;	  
  for (i1=0; i1<cvNr; ++i1) {
    rc = UCBS_DifKntsKntVecBspCrv (&nk, &nkTab, workSeg, mk, mkTab,
                                   &(bspT2[i1]));
    if (rc < 0) return -1;
    if (nk == 0) {
      UCBS_CpyBspCrv (&(bspTab[i1]), memSeg, &(bspT2[i1]));
    } else {
      rc = UCBS_RefKntVecBspCrv (&(bspTab[i1]), memSeg, &(bspT2[i1]),
                                 nk, nkTab);
      if (rc < 0) return -1;
    }
    workSeg->next = workStart2;
  }

  // release workspace
  workSeg->next = workStart;

  return 0;

L_InputErr:
  TX_Error("UCBS_MkeCmpBspCrvs: too many curves");
  return -2;
}


/*=======================================================================*/
  int UCBS_TrfKntVecBspCrv (CurvBSpl *bspo, Memspc *memSeg, CurvBSpl *bspi,
 	                    double scf, double trv) {
/*========================
UCBS_TrfKntVecBspCrv      Transform knotvector <-- bspline curve 

UCBS_TrfKntVecBspCrv     Author: Thomas Backmeister       1.5.2007

Transform the knotvector of a bspline curve to interval [trv,scf+trv].

IN:
  CurvBSpl *bspi  ... bspline curve
  double scf      ... scale factor
  double trv      ... translation value
OUT:
  CurvBSpl *bspo  ... bspline curve with knots in [trv,scf+trv]
  Memspc *memSeg  ... data space of bspo
Returncodes:
  0 = OK
 -1 = out of space
*/

  int rc, m, i1;

  //printf("UCBS_TrfKntVecBspCrv: Transform knotvector <-- bspline curve\n");

  // init bspo
  bspo->deg = bspi->deg;
  bspo->ptNr = bspi->ptNr; 

  // space for knotvector of bspo
  m = bspi->ptNr + bspi->deg;
  bspo->kvTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * (m+1));
  if (rc < 0) goto L_outOfSpace;

  // space for controlpoints of bspo
  bspo->cpTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(Point) * bspi->ptNr);
  if (rc < 0) goto L_outOfSpace;
  
  // scale and translate knotvector
  for (i1=0; i1<m+1; ++i1) {
    bspo->kvTab[i1] = (bspi->kvTab[i1] - bspi->kvTab[0]) /
	              (bspi->kvTab[m] - bspi->kvTab[0]);
    bspo->kvTab[i1] = scf * bspo->kvTab[i1] + trv;
  }    

  // adjust start / end parameter
  bspo->v0 = (bspi->v0 - bspi->kvTab[0]) /
	     (bspi->kvTab[m] - bspi->kvTab[0]);
  bspo->v0 = scf * bspo->v0 + trv;
  bspo->v1 = (bspi->v1 - bspi->kvTab[0]) /
	     (bspi->kvTab[m] - bspi->kvTab[0]);
  bspo->v1 = scf * bspo->v1 + trv;

  // copy control points
  for (i1=0; i1<bspi->ptNr; ++i1)
    bspo->cpTab[i1] = bspi->cpTab[i1];	  

  return 0;

L_outOfSpace:
  TX_Error("UCBS_TrfKntVecBspCrv: out of space");
  return -1;
}


/*=============================== eof ===================================*/


/*=============================== eof ===================================*/
