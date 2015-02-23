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
  ..

-----------------------------------------------------
Modifications:
2007-08-17 NEU: UT3D_Add2Pts (u3d/u3d.h), UT3D_GravCentPts, UT3D_ChkPlanPts,
           UT3D_ch_lnbox (sollte eigentlich schon eingebaut sein,
		       da es in UT3D_pt_intlnbcv gerufen wird, oder ??!!!).
2007-08-16 Fehler in UT3D_ln_pts: if (dist >= tol) ersetzen durch
                                  if (distmax >= tol).
2007-08-08 NEU: UT3D_Ln2Pts, UT3D_pt_multpt.
           Aenderung von Funktionsnamen:
	         u3d_LinComb2Pts in UT3D_LinComb2Pts (u3d.h). 

-----------------------------------------------------
*/
/*!
\file  ../ut/u3d.c
\brief general 3d-geometric functions
\code
=====================================================
List_functions_start:

UT3D_ChkPlanPts      Check planarity <-- points
UT3D_ch_lnbox        check line-X with axis-parallel box
UT3D_CompPts         Compare <-- points
UT3D_GravCentPts     Gravitation center <-- points
UT3D_VcPts2Crvs      Vector <-- corresponding points on two curves

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


Returncodes:
 0 OK
>0 OK with additional info
<0 Error with additional info

compile with gcc:
cc -c -O0 -g3 -Wall u3d.c

Windows:
cl -c u3d.c

ctags -f u3d.tag u3d.c

*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../ut/ut_geo.h"
#include "../ut/ut_math.h"
#include "../ut/ut_TX.h"
#include "../ut/u3d.h"


extern double UT_TOL_0Cos;



//================================================================
  int UT3D_ch_lnbox (Line *ln, Point *pmin, Point *pmax, double tol) {
//================================================================
// UT3D_ch_lnbox        Author: Thomas Backmeister       27.5.2003
// 
/// \code
/// UT3D_ch_lnbox        check line-X with axis-parallel box
/// 
/// Check if a line intersects an axis-parallel box.
/// 
/// IN:
///   Line *ln    ... line
///   Point *pmin ... endpoint 1 of box diagonal
///   Point *pmax ... endpoint 2 of box diagonal
///   double tol  ... tolerance for line intersecting box (>0)
/// OUT:
/// Returncodes:
///   0 = the line does not intersect the box
///   1 = the line intersects the box
/// \endcode

  int rc, i1;
  double a, b, c, d, h, a1, b1, vh;
  Vector vl, vp1, vp2;

  //printf("UT3D_ch_lnbox: check line-X with axis-parallel box\n");

  // init
  rc = 1;

  // line vector
  UT3D_vc_ln (&vl, ln);

  // 2 vectors
  UT3D_vc_2pt (&vp1, &(ln->p1), pmin);
  UT3D_vc_2pt (&vp2, &(ln->p1), pmax);


  // check X of line and box
  // - - - - - - - - - - - -
  a = UT_VAL_MIN;
  b = UT_VAL_MAX;
  a1 = vp1.dx;
  b1 = vp2.dx;
  vh = vl.dx;

  for (i1=0; i1<3; ++i1) {
    if (i1 == 1) {
      a1 = vp1.dy;
      b1 = vp2.dy;
      vh = vl.dy;
    }
    else if (i1 == 2) {
      a1 = vp1.dz;
      b1 = vp2.dz;
      vh = vl.dz;
    }
    if (UTP_compdb0 (vh, UT_TOL_min2)) {
      // 0 not in interval [a1,b1] -> no line-box X
      if (b1 < -tol || a1 > tol) {
        rc = 0;
        break;
      }
    }
    else {
      // intersection of [a,b] with [c,d] empty -> no line-box X
      c = a1 / vh;
      d = b1 / vh;
      if (d < c) {
        h = c;
        c = d;
        d = h;
      }
      if (c > a) a = c;  
      if (d < b) b = d;
      if (a > (b + tol)) {
        rc = 0;
        break;
      }
    }
  }

  return rc;
}


//================================================================
  int UT3D_GravCentPts (Point *gc, int np, Point *ptab) {
//================================================================
// UT3D_GravCentPts         Author: Thomas Backmeister       17.8.2007
// 
/// \code
/// UT3D_GravCentPts        Gravitation center <-- points
/// 
/// Center of gravitation of a set of points.
/// 
/// IN:
///   int np      ... number of points
///   Point *ptab ... points
/// OUT:
///   Point *gc   ... gravitation center
/// Returncode:
///   0 = OK
/// \endcode

  int i1;	  

  //printf("UT3D_GravCentPts: Gravitation center <-- points\n");

  *gc = ptab[0];	  
  for (i1=1; i1<np; ++i1) UT3D_pt_add2pt (gc, gc, &(ptab[i1]));
  UT3D_pt_multpt (gc, gc, (1.0/(double)np)); 
  return 0;
}


//================================================================
  int UT3D_ChkPlanPts (int piT[3], int np, Point *ptab) {
//================================================================
// UT3D_ChkPlanPts       Author: Thomas Backmeister       17.8.2007
// 
/// \code
/// UT3D_ChkPlanPts      Check planarity <-- points
/// 
/// Check if the given points lie on a plane.
/// 
/// IN:
///   int np       ... number of points
///   Point *ptab  ... points
/// OUT:
///   int piT[3]   ... point indices rel. ptab defining plane
///                    (if returncode < 2) 
/// Returncodes:
///   0 = points are not planar
///   1 = points are planar
///   2 = points are collinear
///   3 = points are identic
/// \endcode

  int rc, i1, i2;
  double dist, distmax, ar, armax;
  Vector V1, V2, V3, Vn;


  //printf("UT3D_ChkPlanPts: Check planarity <-- points\n");

  // check input
  if (np <= 1) return 3;
  if (np < 3) return 2;

  // find the most distant points
  distmax = 0.0;
  piT[0] = -1;
  piT[1] = -1;
  for (i1=0; i1<np; ++i1) {
    for (i2=i1+1; i2<np; ++i2) {
      dist = UT3D_len_2pt (&ptab[i1], &ptab[i2]);
      if (dist > distmax) {
        distmax = dist;
        piT[0] = i1;
        piT[1] = i2;
      }
    }
  }
  if (distmax == 0.0) return 3;

  // find 3. point
  armax = 0.0;
  UT3D_vc_2pt (&V1, &(ptab[piT[0]]), &(ptab[piT[1]]));
  for (i1=0; i1<np; ++i1) {
    if (i1 == piT[0] || i1 == piT[1]) continue;
    UT3D_vc_2pt (&V2, &(ptab[piT[0]]), &(ptab[i1]));
    UT3D_vc_crossprod2vc (&V3, &V1, &V2);
    ar = UT3D_acos_2vc (&V3, &V3);
    if (ar > armax) {
      piT[2] = i1;
      armax = ar;
      Vn = V3;
    }	    
  }    
  if (armax == 0.0) return 2;

  // check all points
  UT3D_vc_normalize (&Vn, &Vn);
  for (i1=0; i1<np; ++i1) {
    if (i1 == piT[0] || i1 == piT[1] || i1 == piT[2]) continue;
    UT3D_vc_2pt (&V1, &(ptab[piT[0]]), &(ptab[i1]));
    UT3D_vc_normalize (&V1, &V1);
    if (fabs(UT3D_acos_2vc (&Vn, &V1)) >= UT_TOL_0Cos) return 0;
  }	  

  return 1;
}


/*
replace with UT3D_ln_ptpt
/=====================================================================
  void UT3D_Ln2Pts (Line *ln, Point *p1, Point *p2) {
/================
UT3D_Ln2Pts         3D-Line <-- two 3D-Points

UT3D_Ln2Pts          Author: Thomas Backmeister            8.8.2007

3D-Line from 2 3D-Points.

IN:
  Point *p1  ...  1. point
  Point *p2  ...  2. point
OUT:
  Line *ln   ...  3D-line
/

  ln->p1.x = p1->x;
  ln->p1.y = p1->y;
  ln->p1.z = p1->z;
  ln->p2.x = p2->x;
  ln->p2.y = p2->y;
  ln->p2.z = p2->z;
  return;
}
*/

//================================================================
  int UT3D_VcPts2Crvs (Vector *vc, void *cv, int cvt, void *ocv, int np,
		       int sn) {
//================================================================
// UT3D_VcPts2Crvs     Author: Thomas Backmeister       26.1.2007
// 
/// \code
/// UT3D_VcPts2Crvs    Vector <-- corresponding points on two curves 
/// 
/// The two given curves have same type, orientation and parameterization and
/// should have no intersections.
/// The sample number determines a curve parameter from which corresponding
/// curve points are computed.
/// Their difference is the resulting vector.
/// 
/// From the sample-number the curve-parameter t follows by
/// t = a + sn * (b-a)/np, where [a,b] is the support of the curves.
/// 
/// IN:
///   void *cv     ... curve
///   int cvt      ... type of curve
///   void *ocv    ... offset curve (corr. to cv)
///   int np       ... number of point-positions on cv/ocv (without endpoint)
///   int sn       ... sample-number of points to evaluate (0 <= sn <= np)
/// OUT:
///   Vector *vc   ... vector
/// Returncodes:
///   0 = OK
///  -1 = internal error
/// \endcode

  //printf("UT3D_VcPts2Crvs: Vector <-- corresponding points on two curves\n");

  int rc;
  Point p1, p2;

  rc = UT3D_pt_sampnr (&p1, cv, cvt, np, sn);
  if (rc < 0) return -1;
  rc = UT3D_pt_sampnr (&p2, ocv, cvt, np, sn);
  if (rc < 0) return -1;
  UT3D_vc_2pt (vc, &p1, &p2);

  return 0;
}


//================================================================
  int UT3D_CompPts (int np, Point *pTab, double tol) {
//================================================================
// UT3D_CompPts      Author: Thomas Backmeister       15.6.2007
// 
/// \code
/// UT3D_CompPts     Compare <-- points
/// 
/// Compare the given points.
/// 
/// IN:
///   int np            ... number of points
///   Point *pTab       ... array of points
/// OUT:
/// Returncodes:
///   1/0 <=> points are all equal Y/N
/// \endcode

  int i1;

  //printf("UT3D_CompPts: Compare <-- points\n"); 

  for (i1=1; i1<np; ++i1)
    if (0 == UT3D_comp2pt(&pTab[0],&pTab[i1],tol)) return 0;
  return 1;
}


/*=============================== eof ===================================*/
