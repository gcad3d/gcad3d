// Approximation Polygon -> B-Spline           Thomas Backmeister  Okt. 2003
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
 - set bsp->clo in UT3D_bspl_l2appr, UCBS_BspCrvPts

-----------------------------------------------------
Modifications:
  ..

-----------------------------------------------------
*/
/*!
\file  ../ut/ut_bsp_approx.c
\brief Approximation Polygon -> B-Spline
\code
=====================================================
List_functions_start:

UT3D_bsp_pta__          BSP-Curve from polygon (Approximation)
UT3D_bspl_l2appr        l2-approximating bspline-curve
UT3D_bspl_1Dl2appr      l2-approximating 1D-bspline-curve
UT3D_bspl_evBas         basis-bspline-values

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

// #include "../ut/ut_umem.h"             // UME_save
#include "../ut/ut_geo.h"
#include "../ut/ut_math.h"
// #include "../ut/ut_TX.h"
#include "../ut/ut_bspl.h"
#include "../ut/ut_plg.h"



//====================================================================
  int UT3D_bsp_pta__ (CurvBSpl *cvo, int ptNr, Point *pTab, double tol,
                      double smf, Memspc *memSeg, Memspc *workSeg) {
//====================================================================
/// UT3D_bsp_pta__      approximante polygon to b-spline-curve.
/// Input:
///   smf         smoothFactor; > 1 reduces nr-of-points

 
  int    irc, iDeg, iMod3D, ncp, i1, i2;
  double d0, d1, d2;


  // printf("UT3D_bsp_pta__ ptNr=%d tol=%f smf=%f\n",ptNr,tol,smf);


  iMod3D = 2;      // 1=2D (x,y); 2=3D.
  iDeg   = 2;      // degree out outCurve


  // test max deviation polygon pTab
  d0 = UT3D_ck_pta_maxDev (ptNr, pTab);
  d2 = ptNr / 8;  // 8
  d0 = tol / (d0 / d2);
    // printf(" d0=%f d2=%f\n",d0,d2);
  if(d0 > 1.) {
    d1 = ptNr;
    ncp =  d1 / d0;
    if((ncp > 3)&&(ncp < ptNr)) goto L_1;
  }


  // ncp = number of Controlpoints
  // ncp-min = deg + 1;
  // ncp-max = ptNr - 2; // damit gibts meist am Kurvenende eine Macke !!!
  i1 = iDeg + 1;
  i2 = ptNr - 2;
  ncp = i1 + ((i2-i1) / 2);  // anzahl gewuenschter ContrPoints



  //----------------------------------------------------------------
  L_1:
    // printf(" ncp=%d\n",ncp);

// ACHTUNG: mehr als 25 geht dzt nicht; sonst in USBS_GordSurBspCrvNet zu große
// L1- oder L2-Surface, diese machen dann out-of-mem !
    // if(ncp > 32) {
      // printf("UT3D_bsp_pta__ I001 ncp reduced\n");
      // ncp = 32;
    // }
    if(fabs(smf) > 0.1) {
      ncp *= fabs(smf);
        // printf(" bsp_pta__ncp > %d\n",ncp);
    }

    if(ncp > ptNr) ncp = ptNr;

    if(smf <= 0.)     iDeg = 1;
    else if(ncp > 25) iDeg = 3;

    // approximate
    irc = UT3D_bspl_l2appr (cvo, memSeg, ptNr, pTab,
                           iMod3D, iDeg, ncp, workSeg);
    if(irc < 0) return -1;


      // TESTBLOCK
      // DEB_dump_obj__ (Typ_CVBSP, cvo, "ex-UT3D_bsp_pta__");
      // END TESTBLOCK



  return 0;

}


//=========================================================================
  int UT3D_bspl_l2appr (CurvBSpl *crv, Memspc *memSeg, int np, Point *pTab,
									      int xy, int deg, int ptNr, Memspc *workSeg) {
//=========================================================================
// UT3D_bspl_l2appr     Author: Thomas Backmeister       5.12.2003
// 
/// \code
/// UT3D_bspl_l2appr     l2-approximating bspline-curve
/// 
/// Computation of a bspline-curve least-square-approximating a series
/// of 3D-points.
/// If the number of input points equals the number of controlpoints
/// of the curve, the curve is INTERPOLATING the input points.
/// 
/// IN:
///   int np             ... number of points to approximate
///   Point *pTab        ... array of points to approximate
/// 	int xy             ... =1: use only x- and y-component of points
///   int deg            ... desired degree of bspline-curve (<20 !)
/// 	int ptNr           ... desired number of controlpoints of bspline-curve
///   Memspc *workSeg    ... temporary workspace
/// OUT:
///   CurvBSpl *crv      ... approximating bspline-curve
/// 	Memspc *memSeg     ... space for bspline-curve
/// Returncodes:
///   0 = OK
///  -1 = out of space
///  -2 = out of work space
///  -3 = internal error
/// \endcode

	int rc, i1, j1, nrComp;
	double *pvTab, *wTab, step;
	double *p1Tab, *cp1Tab;
  void *memStart;
					
  // printf("\nUT3D_bspl_l2appr: np=%d ptNr=%d deg=%d",np,ptNr,deg);

	// space for knotvector
  crv->kvTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * (deg+ptNr+1));
  if (rc < 0) goto L_outOfSpace;

  // space for controlpoints
	crv->cpTab = memSeg->next;
	rc = UME_add (memSeg, sizeof(Point) * ptNr);
  if (rc < 0) goto L_outOfSpace;

	// init controlpoints
	for (i1=0; i1<ptNr; ++i1) crv->cpTab[i1] = UT3D_PT_NUL;

  // knotvector
	rc = bspl_knotvec__ (crv->kvTab, ptNr, deg);
  if (rc < 0) {
    TX_Error("UT3D_bspl_l2appr: curve degree >= number of controlpoints");
		return -3;
	}	

  // init curve
	crv->deg = deg;
	crv->ptNr = ptNr;
  crv->v0 = crv->kvTab[deg];
  crv->v1 = crv->kvTab[ptNr];
  crv->dir  = 0;                    // 0=fwd
  crv->clo  = -1;                   // -1=undef
  crv->trm  = 1;                    // 1=not-trimmed


	// --- provide WORKSPACE: --- ---
  // for parametervector
	memStart = workSeg->next;
	pvTab = workSeg->next;
	rc = UME_add (workSeg, sizeof(double) * np);
  if (rc < 0) goto L_outOfWorkSpace;

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
	rc = UME_add (workSeg, sizeof(double) * ptNr);
  if (rc < 0) goto L_outOfWorkSpace;
	// --- --- --- --- --- --- ---

  // parametervector:
	// uniform distributed values in [crv->kvTab[deg],crv->kvTab[ptNr]]
  step = (crv->kvTab[ptNr] - crv->kvTab[deg]) / (double)(np - 1);
	pvTab[0] = crv->kvTab[deg];
  for (i1=1; i1<np-1; ++i1) pvTab[i1] = pvTab[i1-1] + step;
  pvTab[np-1] = crv->kvTab[ptNr];

	// positive weights for points
	wTab[0] = 10000.0;
  for (i1=1; i1<np-1; ++i1) wTab[i1] = 1.0;
	wTab[np-1] = 10000.0;

	// --- COMPONENTWISE APPROXIMATION --- ---
	nrComp = 3; if (xy == 1) nrComp = 2;
	for (j1=0; j1<nrComp; ++j1) {
    if (j1 == 0) for (i1=0; i1<np; ++i1) p1Tab[i1] = pTab[i1].x;
    if (j1 == 1) for (i1=0; i1<np; ++i1) p1Tab[i1] = pTab[i1].y;
    if (j1 == 2) for (i1=0; i1<np; ++i1) p1Tab[i1] = pTab[i1].z;
    rc = UT3D_bspl_1Dl2appr (cp1Tab, np, pvTab, p1Tab, wTab,
	                           deg+1, ptNr, crv->kvTab, workSeg);
    if (rc < 0) return -3;
    if (j1 == 0) for (i1=0; i1<ptNr; ++i1) crv->cpTab[i1].x = cp1Tab[i1];
    if (j1 == 1) for (i1=0; i1<ptNr; ++i1) crv->cpTab[i1].y = cp1Tab[i1];
    if (j1 == 2) for (i1=0; i1<ptNr; ++i1) crv->cpTab[i1].z = cp1Tab[i1];
	}	
	// --- --- --- --- --- --- --- --- --- ---

	// release workspace
	workSeg->next = memStart;

  crv->dir = 0;     // 0=fwd
  crv->clo = -1;    // -1=undefined
  crv->trm = 1;     // 1=not_trimmed

  return 0;

L_outOfSpace:
  TX_Error("UT3D_bspl_l2appr: out of space");
  return -1;

L_outOfWorkSpace:
  TX_Error("UT3D_bspl_l2appr: out of work space");
  return -2;
}


//================================================================
  int UT3D_bspl_1Dl2appr (double *bcoef, int ntau, double *tau,
								        	double *gtau, double *weight, int k, int n,
												  double *t, Memspc *workSeg) {
//================================================================
// UT3D_bspl_1Dl2appr     Author: Thomas Backmeister       1.12.2003
// 
/// \code
/// UT3D_bspl_1Dl2appr     l2-approximating 1D-bspline-curve
/// 
/// Computation of a 1D-bspline-curve which is least-square-approximating a
/// given series of data points.
/// ATTENTION: space for bcoef must be provided by the calling function !
/// 
/// IN:
///   int ntau           ... number of data points
/// 	double *tau        ... parameter values of data points within interval
/// 	                       [t[k-1],t[n]] and strongly increasing.
///   double *gtau       ... data points
///   double *weight     ... positiv weights for data points
///   int k              ... order of bspline curve (= degree+1)
/// 	                       (must be <= 20 !)
/// 	int n              ... number of controlpoints of bspline curve
/// 	double *t          ... knotvector t[0],...,t[n+k-1] of bspline curve
///   Memspc *workSeg    ... temporary workspace
/// OUT:
///   double *bcoef      ... "1D-controlpoints" of bspline curve
/// 	                       (length = n)
/// Returncodes:
///   0 = OK
///  -1 = out of work space
///  -2 = internal error
/// \endcode

  //printf("UT3D_bspl_1Dl2appr: l2-approximating 1D-bspline-curve\n");

	int rc, i, j, left, leftmk, ll, mm, jj;
	double *q, *diag, biatx[20], dw;
	void *memStart;

	// save start of workspace
	memStart = workSeg->next;

	// provide workspace
  q = workSeg->next;
  rc = UME_add (workSeg, sizeof(double) * k * n);
  if (rc < 0) goto L_outOfWorkSpace;
  diag = workSeg->next;
  rc = UME_add (workSeg, sizeof(double) * n);
  if (rc < 0) goto L_outOfWorkSpace;

  // init
	for (i=0; i<n; ++i) bcoef[i] = 0.0;
	for (i=0; i<n*k; ++i) q[i] = 0.0;
  left = k;
	leftmk = 0;

	for (ll=0; ll<ntau; ++ll) {

	  // locate left such that tau[ll] in [t[left-1],t[left])
		for (;;) {
	    if (left == n) break;
		  if (tau[ll] < t[left]) break;
		  ++left;
		  ++leftmk;
    }

		// values of basis-bspline-curves in tau[ll]
		rc = UT3D_bspl_evBas (biatx, t, k, 1, tau[ll], left);
		if (rc < 0) return -2;

		// update right side of linear system (--> bcoef) and
		// update the k diagonals of C-transpose * W * C (--> q)
		for (mm=0; mm<k; ++mm) {
      dw = biatx[mm] * weight[ll];
			j = leftmk + mm;
			bcoef[j] += dw * gtau[ll];
			i = 0;
			for (jj=mm; jj<k; ++jj) {
				q[i*n+j] += biatx[jj] * dw;
				++i;
			}
		}
  }

	// Cholesky-factorization of matrix q
  rc = UTM_mat_CholFact (q, k, n, diag);
	if (rc < 0) return -2;

	// solve Cholesky linear system
  rc = UTM_mat_solveChol (bcoef, q, k, n);
	if (rc < 0) return -2;

  // release workspace
	workSeg->next = memStart;

  return 0;

L_outOfWorkSpace:
  TX_Error("UT3D_bspl_1Dl2appr: out of work space");
  return -1;
}


//================================================================
  int UT3D_bspl_evBas (double *biatx, double *t, int jhigh, int index,
									     double x, int left) {
//================================================================
// UT3D_bspl_evBas     Author: Thomas Backmeister       1.12.2003
// 
/// \code
/// UT3D_bspl_evBas     basis-bspline-values
/// 
/// Computation of all possibly nonzero basis-bspline-values at x of order
/// jout = max(jhigh, (j+2)*(index-1)) with knotvector t.
/// In general the function will be called with jhigh = order = degree+1
/// of the basis-bspline-curves and with index = 1.
/// 
/// IN:
/// 	double *t          ... knotvector of bspline curve
/// 	int jhigh, index   ... to determine the order jout
/// 	double x           ... parameter value
/// 	int left           ... such that x in [t[left-1],t[left])
/// OUT:
///   double *biatx      ... basis-bspline-values at x of order jout
/// 	                       (number = jout)
/// Returncode:
///   0 = OK
/// \endcode

	int jp1, i;
	double saved, term;
	static int j=0;
	static double deltal[20], deltar[20];

  //printf("UT3D_bspl_evBas: basis-bspline-values\n");

  if (index == 1) {
		// start from scratch			
		j = 0;
    biatx[0] = 1.0;
    if (j >= jhigh-1) return 0;
  }		

	for (;;) {
	  jp1 = j + 1;
    deltar[j] = t[left+j] - x;
	  deltal[j] = x - t[left-j-1];

	  saved = 0.0;
    for (i=0; i<=j; ++i) {
      term = biatx[i] / (deltar[i] + deltal[jp1-i-1]);
      biatx[i] = saved + deltar[i] * term;
		  saved = deltal[jp1-i-1] * term;
  	}	
  
    biatx[jp1] = saved;
    j = jp1;
    if (j >= jhigh-1) break;
	}

	return 0;
}


/*=============================== eof ===================================*/
