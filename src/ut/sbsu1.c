//  sbsu1.c
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
  ..

-----------------------------------------------------
*/
/*!
\file  ../ut/sbsu1.c
\brief evaluate sample-point on curve
\code
=====================================================
List_functions_start:

UT3D_pt_cisn               evaluate sample-point on circle
UT3D_pt_elsn               evaluate sample-point on ellipse
UT3D_pt_lnsn               evaluate sample-point on line
UT3D_pt_midnpt             center point <-- points
UT3D_pt_sampnr             evaluate sample-curve-point
UT3D_vc_tgsampnr           evaluate sample-tangent-vector

u3d_LinComb2Pts            po = a1 * p1 + a2 * p2

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



*/


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../ut/ut_geo.h"
// #include "../ut/ut_math.h"
// #include "../ut/ut_mem.h"
// #include "../ut/ut_TX.h"
// 
// #include "ut_pm.h"




//================================================================
  int UT3D_pt_sampnr (Point *po, void *cv, int cvtype, int np, int sn) {
//================================================================
// UT3D_pt_sampnr     Author: Thomas Backmeister       2.10.2003
// 
/// \code
/// UT3D_pt_sampnr     evaluate sample-curve-point (teilen)
/// 
/// Evaluate a point on a curve (line, circle, ellipse, b-spline,...)
/// with a sample-number.
/// From the sample-number the curve-parameter t follows by
/// t = a + sn * (b-a)/np, where [a,b] is the support of the curve.
/// 
/// IN:
///   void *cv     ... curve
///   int cvtype   ... type of curve
///   int np       ... number of point-positions on curve (without endpoint)
///   int sn       ... sample-number of point to evaluate (0 <= sn <= np)
/// OUT:
///   Point *po    ... evaluated point
/// Returncodes:
///   0 = OK
///  -1 = internal error
///  -2 = input error
/// \endcode

  int rc;
  double t;
  CurvBSpl *bspl;

  //printf("UT3D_pt_sampnr: evaluate sample-curve-point\n");

  switch (cvtype)
  {
    case Typ_LN:
      rc = UT3D_pt_lnsn (po, (Line*)cv, np, sn);
      if (rc < 0) return -1;
      break;

    case Typ_CI:
      rc = UT3D_pt_cisn (po, (Circ*)cv, np, sn);
      if (rc < 0) return -1;
      break;

    case Typ_CVELL:
      rc = UT3D_pt_elsn (po, (CurvElli*)cv, np, sn);
      if (rc < 0) return -1;
      break;

    case Typ_CVBSP:
      bspl = (CurvBSpl*)cv;
      t = bspl->v0 + (double)sn * ((bspl->v1 - bspl->v0) / (double)np);
      UT3D_pt_evalparCv (po, bspl, t);
      break;

    default:
      printf("UT3D_pt_sampnr: unexpected curve-type\n");
      return -2;
      break;
  }

  return 0;
}


/* UNUSED
//================================================================
  int UT3D_vc_tgsampnr (Vector *tg, void *cv, int cvtype, Point *ps, int np,
                        int sn) {
//================================================================
// UT3D_vc_tgsampnr     Author: Thomas Backmeister       10.10.2003
// 
/// \code
/// UT3D_vc_tgsampnr     evaluate sample-tangent-vector (teilen)
/// 
/// Evaluate the tangent-vector in a point on a curve
/// (line, circle, ellipse, b-spline,...) with a sample-number.
/// From the sample-number the curve-parameter t follows by
/// t = a + sn * (b-a)/np, where [a,b] is the support of the curve.
/// 
/// IN:
///   void *cv     ... curve
///   int cvtype   ... type of curve
///   Point *ps    ... curve-point with sample-number sn
///   int np       ... number of sample-points on curve (without endpoint)
///   int sn       ... sample-number of position of tangent (0 <= sn <= np)
/// OUT:
///   Vector *tg   ... tangent vector (not normalized)
/// Returncodes:
///   0 = OK
///  -1 = input error
/// \endcode

  double t;
  Vector v1;
  Line *ln;
  Circ *ci;
  CurvElli *el;
  CurvBSpl *bspl;

  //printf("UT3D_vc_tgsampnr: evaluate sample-tangent-vector\n");

  // check input
  if (np < 1) {
    printf("UT3D_vc_tgsampnr: input error\n");
    return -1;
  }

  switch (cvtype)
  {
    case Typ_LN:
      ln = (Line*)cv;
      tg->dx = ln->p2.x - ln->p1.x;
      tg->dy = ln->p2.y - ln->p1.y;
      tg->dz = ln->p2.z - ln->p1.z;
      break;

    case Typ_CI:
      ci = (Circ*)cv;
      UT3D_vc_2pt (tg, &(ci->pc), ps);
      if (ci->rad > 0.0)
        UT3D_vc_rotvcangr (tg, -RAD_90, tg, &(ci->vz)); // CCW
      else
        UT3D_vc_rotvcangr (tg, RAD_90, tg, &(ci->vz));  // CW
      break;

    case Typ_CVELL:  // ThB: schlecht: beim Selektieren hat man Typ_CV !!!
      el = (CurvElli*)cv;
      UT3D_vc_tangel (tg, ps, el);
// ThB: nach Korrektur von UT3D_vc_tangel wieder entfernen:
      // check direction of tangent-vector
      UT3D_vc_2pt (&v1, &(el->pc), ps);
      UT3D_vc_perp2vc (&v1, &v1, tg);
      if (el->dir > 0) {
        if (UT3D_acos_2vc (&v1, &(el->vz)) < 0.0) UT3D_vc_invert (tg, tg);
      }
      else {
        if (UT3D_acos_2vc (&v1, &(el->vz)) > 0.0) UT3D_vc_invert (tg, tg);
      }
// ThB: nach Korrektur von UT3D_vc_tangel wieder entfernen
      break;

    case Typ_CVBSP:
      bspl = (CurvBSpl*)cv;
      t = bspl->v0 + (double)sn * ((bspl->v1 - bspl->v0) / (double)np);
      UT3D_vc_evalparCv (tg, bspl, t);
      break;

    default:
      printf("UT3D_vc_tgsampnr: unexpected curve-type\n");
      return -1;
      break;
  }

  return 0;
}
*/


//================================================================
  int UT3D_pt_lnsn (Point *po, Line *ln, int np, int sn) {
//================================================================
// UT3D_pt_lnsn     Author: Thomas Backmeister       15.10.2003
// 
/// \code
/// UT3D_pt_lnsn     evaluate sample-point on line (teilen)
/// 
/// Evaluate a point on a 3D-line by a sample-number.
/// The sample-number is the number of the point between ln->p1 and ln->p2
/// in relation to the total number of sample-points.
/// 
/// IN:
///   Line *ln     ... 3D-line
///   int np       ... total number of sample-points (without ci->p2)
///   int sn       ... sample-number of point to evaluate (0 <= sn <= np)
/// OUT:
///   Point *po    ... evaluated point
/// Returncodes:
///   0 = OK
///  -1 = input error
/// \endcode

  double d;
  Vector vl;

  //printf("UT3D_pt_lnsn: evaluate sample-point on line\n");

  // check input
  if (np < 1) {
    printf("UT3D_pt_lnsn: input error\n");
    return -1;
  }

  UT3D_vc_2pt (&vl, &(ln->p1), &(ln->p2));
  d = UT3D_len_vc (&vl);
  UT3D_vc_setLength (&vl, &vl, (double)sn*(d/(double)np));
  UT3D_pt_traptvc (po, &(ln->p1), &vl);
  
  return 0;
}



//================================================================
  int UT3D_pt_cisn (Point *po, Circ *ci, int np, int sn) {
//================================================================
// UT3D_pt_cisn     Author: Thomas Backmeister       14.10.2003
// 
/// \code
/// UT3D_pt_cisn     evaluate sample-point on circle (teilen)
/// 
/// Evaluate a point on a 3D-circle by a sample-number.
/// The sample-number is the number of the point between ci->p1 and ci->p2
/// in relation to the total number of sample-points.
/// 
/// IN:
///   Circ *ci     ... 3D-circle
///   int np       ... total number of sample-points (without ci->p2)
///   int sn       ... sample-number of point to evaluate (0 <= sn <= np)
/// OUT:
///   Point *po    ... evaluated point
/// Returncodes:
///   0 = OK
///  -1 = input error
/// \endcode

  double sa;
  Point pt;
  Mat_4x4 m1;

  //printf("UT3D_pt_cisn: evaluate sample-point on circle\n");

  // check input
  if (np < 1) {
    printf("UT3D_pt_cisn: input error\n");
    return -1;
  }

  // angle between ci->p1 and sample-point
  sa = UT3D_angr_ci__ (ci) * ((double)sn/(double)np);
  if (ci->rad < 0.0) sa = -sa;

  // init rotation
  UT3D_m4_init_rot (m1, sa, &(ci->vz));

  // rotate
  pt.x = ci->p1.x - ci->pc.x;
  pt.y = ci->p1.y - ci->pc.y;
  pt.z = ci->p1.z - ci->pc.z;
  UT3D_pt_traptm4 (po, m1, &pt);

  // add circle center-point
  po->x += ci->pc.x;
  po->y += ci->pc.y;
  po->z += ci->pc.z;
  
  return 0;
}



//================================================================
  int UT3D_pt_elsn (Point *po, CurvElli *el, int np, int sn) {
//================================================================
// UT3D_pt_elsn     Author: Thomas Backmeister       14.10.2003
// 
/// \code
/// UT3D_pt_elsn     evaluate sample-point on ellipse (teilen)
/// 
/// Evaluate a point on a 3D-ellipse by a sample-number.
/// The sample-number is the number of the point between el->p1 and el->p2
/// in relation to the total number of sample-points.
/// 
/// IN:
///   CurvElli *el  ... 3D-ellipse
///   int np       ... total number of sample-points (without el->p2)
///   int sn       ... sample-number of point to evaluate (0 <= sn <= np)
/// OUT:
///   Point *po    ... evaluated point
/// Returncodes:
///   0 = OK
///  -1 = internal error
/// \endcode

  int rc;
  double la, lb, lai, lbi;
  Mat_4x3 ma, ima;
  Vector v1, v2, vt;
  Circ ci;

  //printf("UT3D_pt_elsn: evaluate sample-point on ellipse\n");

  // length of ellipse axes
  la = UT3D_len_vc (&(el->va));
  lb = UT3D_len_vc (&(el->vb));

  // inverse length of axes
  lai = 1.0 / la;
  lbi = 1.0 / lb;

  // coordinate-system of ellipse
  UT3D_m3_load (ma, &(el->va), &(el->vb), &(el->vz));

  // inverse matrix
  UT3D_m3_invm3 (ima, ma);

  // make vectors from ellipse start- and endpoint
  UT3D_vc_2pt (&v1, &(el->pc), &(el->p1));
  UT3D_vc_2pt (&v2, &(el->pc), &(el->p2));

  // corresponding circle in ellipse coordinate-system
  ci.rad = la * 2.0;
  ci.pc = UT3D_PT_NUL;
  ci.vz = UT3D_VECTOR_Z;

  // transform ellipse start- and endpoint
  UT3D_vc_travcm3 (&vt, ima, &v1);
  vt.dx *= lai;
  vt.dy *= lbi;
  UT3D_pt_traptvclen (&(ci.p1), (Point*)&UT3D_PT_NUL, &vt, ci.rad);
  UT3D_vc_travcm3 (&vt, ima, &v2);
  vt.dx *= lai;
  vt.dy *= lbi;
  UT3D_pt_traptvclen (&(ci.p2), (Point*)&UT3D_PT_NUL, &vt, ci.rad);

  // sample-point on corresponding circle
  if (el->srot) ci.rad = -ci.rad;
  rc = UT3D_pt_cisn (po, &ci, np, sn);
  if (rc < 0) return -1;

  // transform back sample-point into original coordinate-system
  po->x *= la / fabs(ci.rad);
  po->y *= lb / fabs(ci.rad);
  UT3D_pt_traptm3 (po, ma, po);

  // add center-point of ellipse
  // UT3D_pt_addwpt (po, &(el->pc), 1.0);
  *po = el->pc;

  return 0;
}


//====================== EOF ===========================
