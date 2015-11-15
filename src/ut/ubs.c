//**************************************************************************
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
2007-07-26 Änderung von Funktionsnamen:
           UBS_CrssParVecBspCrvs (ubs.c) in UCBS_CrssParVecBspCrvs (ubscrv.c)
           alle allgemeinen bspline utility-Funktionen ubs_xxx in UBS_xxx.
2007-04-28 Änderung von Funktionsnamen:
           UT3D_bspl_ctrlptsdiv in ubs_DivCtrlPtsBsp
           UT3D_bspl_knotvecdiv in ubs_DivKnotVec

-----------------------------------------------------
*/
/*!
\file  ../ut/ubs.c
\brief b spline surface functions
\code
=====================================================
List_functions_start:

UBS_DivCtrlPtsBsp       Divide <-- bspline controlpoints
UBS_DivKnotVec          Divide <-- knot vector
UBS_FdSpnKntVec         Find span <-- knotvector, parameter 
UBS_KntVecParVec        Knotvector <-- parametervector
UBS_ParVecPts           Parametervector <-- points
UBS_ParVecUVPts         UV-parametervector <-- rectangular arranged points

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



Returncodes:
 0 OK
>0 OK with additional info
<0 Error with additional info

compile with gcc:
cc -c -O0 -g3 -Wall ubs.c

Windows:
cl -c ubs.c

ctags -f ubs.tag ubs.c

*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../ut/ut_geo.h"
#include "../ut/ut_math.h"
#include "../ut/ut_TX.h"

#include "ubs.h"


/*=======================================================================*/
  int UBS_FdSpnKntVec (double *kvTab, int deg, int ptNr, double t) {
/*===================
UBS_FdSpnKntVec      Find span <-- knotvector, parameter 

UBS_FdSpnKntVec     Author: Thomas Backmeister       25.6.2007

Find the span within a knotvector to a parameter t.
The function works with any parameter interval [a,b] of the knotvector.

IN:
  double *kvTab   ... knotvector
  int deg         ... degree
  int ptNr        ... number of controlpoints
  double t        ... parameter
OUT:
Returncode:
  i = index such that t is in [kvTab[i], kvTab[i+1]).
*/

  int mid, low, high;


  // printf("UBS_FdSpnKntVec deg=%d ptNr=%d t=%f\n",deg,ptNr,t);
  // {int i1;for(i1=0;i1<=ptNr;++i1)printf(" kv[%d]=%f\n",i1,kvTab[i1]);}


  if (t > kvTab[ptNr]) { mid = ptNr - 1; goto L_exit; }
  if (t < kvTab[0]) { mid = 0; goto L_exit; }             // 2011-08-22

  low = deg;
  high = ptNr;
  mid = (low + high) / 2;
    // printf(" low=%d high=%d mid=%d\n",mid,low,high);


  while (t < kvTab[mid] || t >= kvTab[mid+1]) {
    if (t < kvTab[mid]) high = mid;
    else low = mid;
    mid = (low + high) / 2;
  }


  L_exit:
    // printf("ex UBS_FdSpnKntVec %d\n",mid);

  return mid;
}


/*=======================================================================*/
  int UBS_ParVecUVPts (double **pvTab, Memspc *memSeg, int npu, int npv,
		       Point *pTab, int dirUV) {
/*===================
UBS_ParVecUVPts     UV-parametervector <-- rectangular arranged points

UBS_ParVecUVPts      Author: Thomas Backmeister       18.6.2007

U- or V-parametervector of a rectangular arranged set of points.
pTab[i*npv+j], 0 <= i < npu, 0 <= j < npv.
The total chord length of a row can be zero; so the algorithm works 
also for 3-sided surfaces. 
(See L.Piegl, The NURBS Book, p. 377)

IN:
  int npu            ... number of points in U-direction
  int npv            ... number of points in V-direction
  Point *pTab        ... array of points
  int dirUV          ... 0/1 <=> U/V-direction
OUT:
  double **pvTab     ... parametervector (npu, npv resp.)
  Memspc *memSeg     ... data space of pvTab (and workspace)
Returncodes:
  0 = OK
 -1 = out of space
 -2 = input error
*/

  int rc, num, k, l;
  double total, *cds, d;
  void *memStart;

  //printf("UBS_ParVecUVPts: UV-parametervector <-- rect. arranged points\n");

  // space for pvTab
  *pvTab = memSeg->next;
  if (dirUV == 0) rc = UME_add (memSeg, sizeof(double) * npu);
  else rc = UME_add (memSeg, sizeof(double) * npv);
  if (rc < 0) goto L_outOfSpace;

  // start of workspace
  memStart = memSeg->next;

  // workspace cds
  cds = memSeg->next;
  if (dirUV == 0) rc = UME_add (memSeg, sizeof(double) * (npu-1));
  else rc = UME_add (memSeg, sizeof(double) * (npv-1));
  if (rc < 0) goto L_outOfSpace;

  if (dirUV == 0) {  // U-direction
    num = npv;
    (*pvTab)[0] = 0.0;
    (*pvTab)[npu-1] = 1.0;
    for (k=1; k<npu-1; k++) (*pvTab)[k] = 0.0;
    for (l=0; l<npv; l++) {
      total = 0.0;
      for (k=1; k<npu; k++) {
        cds[k] = UT3D_len_2pt (&(pTab[k*npv+l]), &(pTab[(k-1)*npv+l]));
        total += cds[k];
      }
      if (UTP_comp_0 (total)) num = num - 1;
      else {
        d = 0.0;
        for (k=1; k<npu-1; k++) {
          d += cds[k];
	  (*pvTab)[k] = (*pvTab)[k] + d / total;
        }		
      } 
    }
    if (num == 0) return -2;
    for (k=1; k<npu-1; k++) (*pvTab)[k] /= (double)num; 
  }
  else {             // V-direction
    num = npu;
    (*pvTab)[0] = 0.0;
    (*pvTab)[npv-1] = 1.0;
    for (l=1; l<npv-1; l++) (*pvTab)[l] = 0.0;
    for (k=0; k<npu; k++) {
      total = 0.0;
      for (l=1; l<npv; l++) {
        cds[l] = UT3D_len_2pt (&(pTab[k*npv+l]), &(pTab[k*npv+l-1]));
        total += cds[l];
      }
      if (UTP_comp_0 (total)) num = num - 1;
      else {
        d = 0.0;
        for (l=1; l<npv-1; l++) {
          d += cds[l];
	  (*pvTab)[l] = (*pvTab)[l] + d / total;
        }		
      } 
    }
    if (num == 0) return -2;
    for (l=1; l<npv-1; l++) (*pvTab)[l] /= (double)num; 
  }

  // release workspace
  memSeg->next = memStart;

  return 0;
 
L_outOfSpace:
  TX_Error("UBS_ParVecUVPts: out of space");
  return -1;
}


/*=======================================================================*/
  int UBS_ParVecPts (double **pvTab, Memspc *memSeg, int np, Point *pTab,
		     int pvType) {
/*=================
UBS_ParVecPts     Parametervector <-- points

UBS_ParVecPts      Author: Thomas Backmeister       13.6.2007

Parametervector of a set of points.
This is an array of values in [0,1] either evenly distributed
(standard uniform) or taking care of the distances between the
input points (optimized).

IN:
  int np            ... number of points
  Point *pTab       ... array of points
  int pvType        ... type of parameter vector
                        (0/1 <=> standard uniform / optimized)
OUT:
  double **pvTab    ... parametervector (np)
  Memspc *memSeg    ... data space of pvTab
Returncodes:
  0 = OK
 -1 = out of space
*/

  int rc, i1;
  double dsum, d;

  //printf("UBS_ParVecPts: Parametervector <-- points\n"); 

  // space for pvTab
  *pvTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * np);
  if (rc < 0) goto L_outOfSpace;

  if (pvType == 0) {
    // standard uniform parameter vector	  
    d = 1.0 / (double)(np - 1);
    (*pvTab)[0] = 0.0;
    (*pvTab)[np-1] = 1.0;
    for (i1=1; i1<np-1; ++i1) (*pvTab)[i1] = (*pvTab)[i1-1] + d;
    goto L_End;
  }	  

  // compute sum of distances between all points
  dsum = 0.0;
  for (i1=1; i1<np; ++i1) dsum += UT3D_len_2pt (&(pTab[i1-1]), &(pTab[i1]));

  // compute pvTab
  (*pvTab)[0] = 0.0;
  (*pvTab)[np-1] = 1.0;
  if (UTP_comp_0 (dsum)) {
    d = 1.0 / ((double)np-1.0);	  
    for (i1=1; i1<np-1; ++i1) {
      (*pvTab)[i1] = d;
      (*pvTab)[i1] += (*pvTab)[i1-1];
    }
  }
  else {
    for (i1=1; i1<np-1; ++i1) {
      (*pvTab)[i1] = (UT3D_len_2pt (&(pTab[i1-1]), &(pTab[i1]))) / dsum;
      (*pvTab)[i1] += (*pvTab)[i1-1];
    }
  }

L_End:  
  return 0;
 
L_outOfSpace:
  TX_Error("UBS_ParVecPts: out of space");
  return -1;
}


/*=======================================================================*/
  int UBS_KntVecParVec (double **kvTab, Memspc *memSeg, int deg, int np,
		        double *pvTab) {
/*====================
UBS_KntVecParVec     Knotvector <-- parametervector

UBS_KntVecParVec      Author: Thomas Backmeister       9.5.2007

Compute a knotvector from a parametervector by averaging.
The limits of the knotvector are those from the parametervector.

IN:
  int deg           ... degree (of a bspline curve) (< np)
  int np            ... number of parameters (number of controlpoints) 
  double *pvTab     ... parametervector
OUT:
  double **kvTab    ... knotvector (np + deg + 1)
  Memspc *memSeg    ... data space of kvTab
Returncodes:
  0 = OK
 -1 = out of space
*/

  int rc, m, n, i1, i2;

  //printf("UBS_KntVecParVec: Knotvector <-- parametervector\n"); 

  // space for knotvector
  m = np + deg + 1;
  *kvTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * m);
  if (rc < 0) goto L_outOfSpace;

  // compute knotvector
  for (i1=0; i1<=deg; ++i1) (*kvTab)[i1] = pvTab[0];
  for (i1=np; i1<m; ++i1) (*kvTab)[i1] = pvTab[np-1];
  for (i1=deg+1; i1<np; ++i1) {
    (*kvTab)[i1] = 0.0;
    n = i1 - deg;
    for (i2=0; i2<deg; ++i2) (*kvTab)[i1] += pvTab[n+i2];
    (*kvTab)[i1] /= (double)deg;
  }

  return 0;
 
L_outOfSpace:
  TX_Error("UBS_KntVecParVec: out of space");
  return -1;
}


/*=======================================================================*/
  int UBS_DivKnotVec (double **kvTab1, Memspc *memSeg1,
                      double **kvTab2, Memspc *memSeg2,
                      int *r, double *kvTab, int m, int d, double t) {
/*==================
UBS_DivKnotVec     Divide <-- knot vector 

UBS_DivKnotVec      Author: Thomas Backmeister       24.10.2003

Divide the knotvector of a bspline curve.
The resulting knotvectors belong to the corresponding bspline curves
from subdividing the given curve.

IN:
  double *kvTab   ... knotvector of bspline curve
  int m           ... length of knotvector - 1
  int d           ... degree of bspline curve
  double t        ... division parameter (kvTab[d] < t < kvTab[m-d])
OUT:
  double **kvTab1 ... knotvector of bspline curve 1
  Memspc *memSeg1 ... data space of kvTab1
  double **kvTab2 ... knotvector of bspline curve 2
  Memspc *memSeg2 ... data space of kvTab2
  int *r          ... segment of division parameter:
                      kvTab[*r] <= t < kvTab[*r+1]
Returncodes:
  0 = OK
 -1 = input error
 -2 = out of space
*/

  int rc, i1, ih;

  //printf("UBS_DivKnotVec: Divide <-- knot vector\n"); 

  // segment [kvTab[*r],kvTab[*r+1]) of division parameter
  *r = -1;
  for (i1=d; i1<m-d; ++i1) {
    if (kvTab[i1] <= t && t < kvTab[i1+1]) {
      *r = i1;
      break;
    }
  }
  if (*r == d) {
    if (t == kvTab[d]) *r = -1;
  }
  if (*r == -1) {
    TX_Error("UBS_DivKnotVec: division parameter %f not valid", t);
    TX_Error("UBS_DivKnotVec: %f < divpar < %f necessary!",
             kvTab[d], kvTab[m-d]);
    return -1;
  }

  // space for kvTab1 and kvTab2
  ih = *r - d;
  *kvTab1 = memSeg1->next;
  rc = UME_add (memSeg1, sizeof(double) * (*r+d+2));
  if (rc < 0) goto L_outOfSpace;
  *kvTab2 = memSeg2->next;
  rc = UME_add (memSeg2, sizeof(double) * (m-ih+1));
  if (rc < 0) goto L_outOfSpace;

  // division of knotvector
  for (i1=0; i1<=(*r); ++i1) (*kvTab1)[i1] = kvTab[i1];
  for (i1=(*r)+1; i1<=(*r)+d; ++i1) (*kvTab1)[i1] = t;
  if (t > kvTab[*r]) (*kvTab1)[*r+d+1] = t;
  for (i1=0; i1<=d; ++i1) (*kvTab2)[i1] = t;
  for (i1=d+1;i1<=m-ih; ++i1) (*kvTab2)[i1] = kvTab[i1+ih];

  return 0;
 
L_outOfSpace:
  TX_Error("UBS_DivKnotVec: out of space");
  return -2;
}


/*=======================================================================*/
  int UBS_DivCtrlPtsBsp (Point *cpTab1, Point *cpTab2, int d,
                         double *kvTab, int cpNr, Point *cpTab,
                         double t, int r, int c1, int c2, Memspc *memSeg1) {
/*=====================
UBS_DivCtrlPtsBsp      Divide <-- bspline controlpoints 

UBS_DivCtrlPtsBsp       Author: Thomas Backmeister       24.10.2003

Divide the controlpoints of a bspline curve.
The resulting controlpoint-arrays belong to the corresponding bspline
curves from subdividing the given curve.
The underlying algorithm is "deBoor".

In case of a surface cpTab, cpTab1 and cpTab2 are ordered in ptVNr
sequences of ptUNr U-controlpoints (matrix structure).
To be able to access subsequent V-controlpoints in this case we need
c1 and c2.
In case of a curve the controlpoints have normal array structure,
thus c1 = 0 and c2 = 1.

ATTENTION: space for controlpoints must be provided already!

IN:
  int d           ... degree of bspline curve
  double *kvTab   ... knotvector of bspline curve
  int cpNr        ... number of controlpoints of bspline curve
  Point *cpTab    ... controlpoints of bspline curve
  double t        ... division parameter
  int r           ... segment of division parameter:
                      kvTab[r] <= t < kvTab[r+1]
  int c1,c2       ... index map i |--> c1 + i * c2 for accessing
                      the arrays cpTab, cpTab1, cpTab2
  Memspc *memSeg1 ... temporary workspace
OUT:
  Point *cpTab1   ... controlpoints of bspline curve 1
  Point *cpTab2   ... controlpoints of bspline curve 2
Returncodes:
  0 = OK
 -1 = out of space
*/

  int rc, i1, i2, ip1, ip2;
  int ih, jh, kh, n1;
  double a, b;
  void *memstart;
  Point *ptab;

  //printf("UBS_DivCtrlPtsBsp: Divide <-- bspline controlpoints\n");

  // some controlpoints remain unchanged
  ih = r - d;
  for (i1=0; i1<=ih; ++i1) {
    ip1 = c1 + i1 * c2;
    cpTab1[ip1] = cpTab[ip1];
  }
  for (i1=d; i1<cpNr-ih; ++i1) {
    ip1 = c1 + i1 * c2;
    ip2 = ip1 + ih * c2;
    cpTab2[ip1] = cpTab[ip2];
  }

  // work space
  memstart = memSeg1->next;
  ptab = memSeg1->next;
  rc = UME_add (memSeg1, sizeof(Point) * (d+1));
  if (rc < 0) goto L_outOfSpace;

  // copy controlpoints for deBoor
  for (i1=0; i1<=d; ++i1) {
    ip1 = c1 + (ih+i1) * c2;
    ptab[i1] = cpTab[ip1];
  }

  // deBoor
  n1 = d;
  for (i1=0; i1<d; ++i1) {
    kh = r + 1;
    jh = kh - n1;
    for (i2=0; i2<n1; ++i2) {
      a = (t - kvTab[jh+i2]) / (kvTab[kh+i2] - kvTab[jh+i2]);
      b = 1.0 - a;
      ptab[i2].x = b * ptab[i2].x + a * ptab[i2+1].x;
      ptab[i2].y = b * ptab[i2].y + a * ptab[i2+1].y;
      ptab[i2].z = b * ptab[i2].z + a * ptab[i2+1].z;
    }
    if (t > kvTab[r] || (t == kvTab[r] && i1 < d-1))
      cpTab1[c1+(ih+i1+1)*c2] = ptab[0];
    cpTab2[c1+(d-1-i1)*c2] = ptab[n1-1];
    --n1;
  }

  // release work space
  memSeg1->next = memstart;

  return 0;
 
L_outOfSpace:
  TX_Error("UBS_DivCtrlPtsBsp: out of space");
  return -1;
}


/*=============================== eof ===================================*/
