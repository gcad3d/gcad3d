// 2D-functions for moebius circles          Thomas Backmeister
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
  ..

-----------------------------------------------------
*/
/*!
\file  ../ut/utemp.c
\brief 2D-functions for moebius circles
\code
=====================================================
List_functions_start:

UT2D_Comp2Lns        Compare <-- 2 lines
UT2D_Ci2PtsDiam      Circle <-- 2 points on diameter
UT2D_Ci2PtsCentPer   Circle <-- center & peripherical point
UT2D_Ci3Pts          Circle <-- 3 points
UT2D_FixPtsProd2Invs Fixpoints <-- product of 2 inversions
UT2D_TouchLnLnPt     Touching circles <-- 2 lines, 1 point
UT2D_CiInvertPoint   Invert at circle <-- point
UT2D_LnInvertPoint   Invert at line <-- point

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_os.h"               // OS_get_bas_dir ..
#include "../ut/ut_obj.h"              // UTO_stru_2_obj

#include "../ut/utemp.h"              // UTO_stru_2_obj



// UT2D_CiInvertPoint
// UT2D_LnInvertPoint

// includes from ../ut/ut_geo_const.h
extern const Point2  UT2D_PT_INFTY;







//================================================================
  int UT2D_Comp2Lns (Line2 *ln1, Line2 *ln2) {
//================================================================
// UT2D_Comp2Lns         Author: Thomas Backmeister       4.10.2007
// 
/// \code
/// UT2D_Comp2Lns        Compare <-- 2 lines
/// Test ob 2 Linien genau auf der gleichen unbegrenzten Geraden liegen.
/// 
/// Compare two 2D-lines.
/// 
/// IN:
///   Line2 *ln1  ... line 1
///   Line2 *ln2  ... line 2
/// OUT:
/// Returncodes:
///   0 = non-identical lines
///   1 = identical lines
/// \endcode

  int rc1, rc2;
  double k1, k2, d1, d2;

  //printf("UT2D_Comp2Lns: Compare <-- 2 lines\n");

  // coefficients of lines
  rc1 = UT2D_lncoe_ln (&k1, &d1, ln1);
  rc2 = UT2D_lncoe_ln (&k2, &d2, ln2);

  // compare
  if (rc1 == 0) {
    if (rc2 == 0) {
      if (UTP_comp2db (k1, k2, UT_TOL_min1) &&
          UTP_comp2db (d1, d2, UT_TOL_min1)) return 1;
      else return 0;
    }
    else return 0;
  }
  else { // ln1 parallel y-axis
    if (rc2 == 0) return 0;
    else { // ln2 parallel y-axis
      if (UTP_comp2db (ln1->p1.x, ln2->p1.x, UT_TOL_min1)) return 1;
      else return 0;
    }
  }
}


//================================================================
  int UT2D_Ci2PtsDiam (Circ2 *ci, Point2 *p1, Point2 *p2) {
//================================================================
// UT2D_Ci2PtsDiam         Author: Thomas Backmeister       9.10.2007
// 
/// \code
/// UT2D_Ci2PtsDiam        Circle <-- 2 points on diameter
/// 
/// Compute a circle from two points which are defining its diameter.
/// 
/// IN:
///   Point2 *p1  ... point 1
///   Point2 *p2  ... point 2
/// OUT:
///   Circ2 *ci   ... circle
/// Returncode:
///   0 = OK
/// \endcode

  //printf("UT2D_Ci2PtsDiam: Circle <-- 2 points on diameter\n");

  UT2D_pt_mid2pt (&(ci->pc), p1, p2);
  ci->p1 = *p1;
  ci->p2 = *p1;
  ci->rad = UT2D_len_2pt (p1, p2) / 2.0;
  ci->ango = RAD_360;
  return 0;
}


//================================================================
  int UT2D_Ci2PtsCentPer (Circ2 *ci, Point2 *pc, Point2 *pp) {
//================================================================
// UT2D_Ci2PtsCentPer         Author: Thomas Backmeister       9.10.2007
// 
/// \code
/// UT2D_Ci2PtsCentPer        Circle <-- center & peripherical point
/// 
/// Compute a circle from center and peripherical point.
/// 
/// IN:
///   Point2 *pc  ... center point
///   Point2 *pp  ... peripherical point
/// OUT:
///   Circ2 *ci   ... circle
/// Returncode:
///   0 = OK
/// \endcode

  //printf("UT2D_Ci2PtsCentPer: Circle <-- center & peripherical point\n");

  ci->pc = *pc;
  ci->p1 = *pp;
  ci->p2 = *pp;
  ci->rad = UT2D_len_2pt (pc, pp);
  ci->ango = RAD_360;
  return 0;
}


//================================================================
  int UT2D_Ci3Pts (Circ2 *ci, Point2 *p1, Point2 *p2, Point2 *p3) {
//================================================================
// UT2D_Ci3Pts         Author: Thomas Backmeister       29.10.2007
// 
/// \code
/// UT2D_Ci3Pts        Circle <-- 3 points
/// 
/// Compute a circle from three points.
/// The points may not be collinear.
/// 
/// IN:
///   Point2 *p1  ... point 1
///   Point2 *p2  ... point 2
///   Point2 *p3  ... point 3
/// OUT:
///   Circ2 *ci   ... circle
/// Returncodes:
///   0 = OK
///  -1 = input error
/// \endcode

  int rc;
  Point2 p12, p23;
  Vector2 v12, v23;

  //printf("UT2D_Ci3Pts: Circle <-- 3 points\n");

  // midnormal between p1,p2
  UT2D_pt_mid2pt (&p12, p1, p2);
  UT2D_vc_2pt (&v12, p1, p2);
  UT2D_vc_perpvc (&v12, &v12);

  // midnormal between p2,p3
  UT2D_pt_mid2pt (&p23, p2, p3);
  UT2D_vc_2pt (&v23, p2, p3);
  UT2D_vc_perpvc (&v23, &v23);

  // center of circle
  rc = UT2D_pt_int2pt2vc (&(ci->pc), &p12, &v12, &p23, &v23);
  if (rc < 1) return -1;

  // remaining circle data
  ci->rad = UT2D_len_2pt (&(ci->pc), p1);
  ci->p1 = *p1;
  ci->p2 = *p1;
  ci->ango = RAD_360;

  return 0;
}


//================================================================
  int UT2D_FixPtsProd2Invs (Point2 *E, Point2 *F, ObjGX *ox1, ObjGX *ox2) {
//================================================================
// UT2D_FixPtsProd2Invs         Author: Thomas Backmeister       18.10.2007
// 
/// \code
/// UT2D_FixPtsProd2Invs        Fixpoints <-- product of 2 inversions
/// 
/// Compute the two different fixpoints of a product of two inversions whose
/// Moebius circles (circle or line) have no common point
/// (UT2D_PT_INFTY included; thus the Moebius circles may not be two lines).
/// If the Moebius circles are concentric circles the fixpoints are their
/// common centerpoint and UT2D_PT_INFTY.
/// 
/// IN:
///   ObjGX *ox1  ... Moebius circle 1 (circle)
///   ObjGX *ox2  ... Moebius circle 2 (circle or line)
///                   (empty intersection with ox1!)
/// 		  ox1 und ox2 duerfen sich nicht schneiden !
/// OUT:
///   Point2 *E   ... fixpoint 1
///   Point2 *F   ... fixpoint 2 (may be UT2D_PT_INFTY)
/// Returncodes:
///   0 = OK
///  -1 = internal error
///  -2 = input error
/// \endcode

  int rc;
  double dm;
  Circ2 *ci1, *ci2, cm;
  Line2 *ln2, g1, h1;
  Vector2 vg, vn;
  Point2 p1, q1, r1, s1, m1, m1i;

  //printf("UT2D_FixPtsProd2Invs: Fixpoints <-- product of 2 inversions\n");

  if (ox1->typ == Typ_CI2) {
    ci1 = (Circ2*)ox1->data;
    if (ox2->typ == Typ_CI2) {
      ci2 = (Circ2*)ox2->data;
      // --------------
      // circle, circle
      // --------------

      // check if circles are concentric
      if (UT2D_comp2pt (&ci1->pc, &ci2->pc, UT_TOL_min1)) {
        *E = ci1->pc;
        *F = UT2D_PT_INFTY;
        goto L_Done;;
      }

      // line g1 through circle centerpoints & linevector
      UT2D_ln_ptpt (&g1, &ci1->pc, &ci2->pc);
      UT2D_vc_ln (&vg, &g1);

      // intersection of line g1 with circle 2
      UT2D_pt_traptvclen (&p1, &ci2->pc, &vg, fabs(ci2->rad));
      UT2D_pt_traptvclen (&q1, &ci2->pc, &vg, -fabs(ci2->rad));

      // a point on circle 1 not incident with g1
      UT2D_pt_tranor2ptlen (&r1, &ci1->pc, &ci2->pc, fabs(ci1->rad));

      // circle cm through p1,q1,r1
      rc = UT2D_Ci3Pts (&cm, &p1, &q1, &r1);
      if (rc < 0) return -1;

      // intersection of circle 1 with circle cm
      rc = UT2D_2pt_intcici (&r1, &s1, &(ci1->pc), fabs(ci1->rad),
                 &(cm.pc), fabs(cm.rad));
      if (rc != 0) return -1; // 2 different intersection points expected!

      // line h1 through r1,s1
      UT2D_ln_ptpt (&h1, &r1, &s1);

      // intersection of lines g1,h1
      rc = UT2D_pt_int2ln (&m1, &g1, &h1);
      if (rc < 1) return -1; // intersection point expected!

      // touchpoints on circle 1 of tangential lines through m1
      m1i = UT2D_CiInvertPoint (&m1, &(ci1->pc), fabs(ci1->rad));
      UT2D_vc_perpvc (&vn, &vg);
      rc = UT2D_2pt_intciptvc (&p1, &q1, &(ci1->pc), fabs(ci1->rad), &m1i, &vn);
      if (rc != 0) return -1; // 2 different intersection points expected!

      // points E,F
      dm = UT2D_len_2pt (&m1, &p1);
      UT2D_pt_traptvclen (E, &m1, &vg, dm);
      UT2D_pt_traptvclen (F, &m1, &vg, -dm);
    }
    else if (ox2->typ == Typ_LN2) {
      ln2 = (Line2*)ox2->data;
      // ------------
      // circle, line
      // ------------

      // normal projection of circlecenter onto line
      UT2D_vc_ln (&vn, ln2);
      rc = UT2D_pt_projptptvc (&m1, &(ci1->pc), &(ln2->p1), &vn);
      if (rc != 0) return -1; // empty intersection of ci1,ln2!

      // touchpoints on circle 1 of tangential lines through m1
      m1i = UT2D_CiInvertPoint (&m1, &(ci1->pc), fabs(ci1->rad));
      rc = UT2D_2pt_intciptvc (&p1, &q1, &(ci1->pc), fabs(ci1->rad), &m1i, &vn);
      if (rc != 0) return -1; // 2 different intersection points expected!

      // points E,F
      dm = UT2D_len_2pt (&m1, &p1);
      UT2D_vc_2pt (&vg, &(ci1->pc), &m1);
      UT2D_pt_traptvclen (E, &m1, &vg, dm);
      UT2D_pt_traptvclen (F, &m1, &vg, -dm);
    }
    else return -2;
  }
  else return -2;

L_Done:
  return 0;
}


//================================================================
  int UT2D_TouchLnLnPt (Circ2 *ci1, Circ2 *ci2,
		        Line2 *l1, Line2 *l2, Point2 *pt) {
//================================================================
// UT2D_TouchLnLnPt     Author: Thomas Backmeister       29.10.2007
// 
/// \code
/// UT2D_TouchLnLnPt    Touching circles <-- 2 lines, 1 point
/// 
/// Compute two circles which touch two different lines and a point.
/// It is assumed that the lines are not parallel and that the point is
/// not incident with any of the two lines.
/// 
/// IN:
///   Line2 *l1   ... line 1
///   Line2 *l2   ... line 2
///   Point2 *pt  ... a point
/// OUT:
///   Circ2 *ci1  ... touch circle 1
///   Circ2 *ci2  ... touch circle 2
/// Returncodes:
///   0 = OK
///  -1 = input error
/// \endcode

  int rc;
  double rad;
  Point2 Z, m1, pp, p1, p2;
  Vector2 v1, v2, vm, vz, vp;

  //printf("UT2D_TouchLnLnPt: Touching circles <-- 2 lines, 1 point\n");

  // intersection point of lines
  rc = UT2D_pt_int2ln (&Z, l1, l2);
  if (rc < 1) return -1;

  // linevectors
  UT2D_vc_ln (&v1, l1);
  UT2D_vc_ln (&v2, l2);

  // point on middleline of lines
  UT2D_vc_mid2vc (&vm, &v1, &v2);
  UT2D_pt_traptvc (&m1, &Z, &vm);

  // radius of a circle about m1 touching the lines
  rc = UT2D_pt_projptptvc (&pp, &m1, &Z, &v1);
  if (rc < 0) return -1;
  rad = UT2D_len_2pt (&m1, &pp);

  // intersection of touching circle with line (Z,pt)
  UT2D_vc_2pt (&vz, &Z, pt);
  rc = UT2D_2pt_intciptvc (&p1, &p2, &m1, rad, &Z, &vz);

  if (rc < 0) {
    // point on 2. middleline of lines
    UT2D_vc_perpvc (&vm, &vm);
    UT2D_pt_traptvc (&m1, &Z, &vm);

    // radius of a circle about m1 touching the lines
    rc = UT2D_pt_projptptvc (&pp, &m1, &Z, &v1);  // UT2D_PtProjPtPtVc
    if (rc != 0) return -1;
    rad = UT2D_len_2pt (&m1, &pp);

    // intersection of touching circle with line (Z,pt)
    rc = UT2D_2pt_intciptvc (&p1, &p2, &m1, rad, &Z, &vz);
  }
  if (rc != 0) return -1;

  // touch circle 1
  UT2D_vc_2pt (&vp, &m1, &p1);
  UT2D_pt_int2pt2vc (&(ci1->pc), &Z, &vm, pt, &vp);
  ci1->rad = UT2D_len_2pt (&(ci1->pc), pt);
  ci1->p1 = *pt;
  ci1->p2 = *pt;
  ci1->ango = RAD_360;

  // touch circle 2
  UT2D_vc_2pt (&vp, &m1, &p2);
  UT2D_pt_int2pt2vc (&(ci2->pc), &Z, &vm, pt, &vp);
  ci2->rad = UT2D_len_2pt (&(ci2->pc), pt);
  ci2->p1 = *pt;
  ci2->p2 = *pt;
  ci2->ango = RAD_360;

  return 0;
}


///================================================================
  Point2 UT2D_CiInvertPoint (Point2 *pt, Point2 *ci, double ri) {
//================================================================
// UT2D_CiInvertPoint     Author: Thomas Backmeister       9.10.2007
// 
/// \code
/// UT2D_CiInvertPoint    Invert at circle <-- point
///
/// Kreis aus ci-ri:
/// Wenn pt ausserhalb Kreis:
/// der Inverse Punkt ist der Mittelpunkt jener 2 Punkte, wo die
/// Tangenten vom Punkt pt an den Kreis den Kreis beruehren.
///
/// Wenn pt innerhalb Kreis: umgekehrt ...
/// Wenn pt genau am Kreis: Punkte ident ..
/// Wenn pt genau am KreisCenter: genau 1 Punkt im unendlichen ..
/// 
/// Invert (=mirror) a point at a circle.
/// The point may be infinite (=UT2D_PT_INFTY).
/// 
/// IN:
///   Point2 *pt  ... point
///   Point2 *ci  ... center of inversion (!=UT2D_PT_INFTY)
///   double ri   ... radius of inversion
/// OUT:
/// Returncode:
///   inverse point (may be infinite)
/// \endcode


  double d;
  Point2 pti;

  //printf("UT2D_CiInvertPoint: Invert at circle <-- point\n");

  if (UT2D_comp2pt (pt, &UT2D_PT_INFTY, UT_TOL_pt)) return *ci;
  if (UT2D_comp2pt (pt, ci, UT_TOL_min0)) return UT2D_PT_INFTY;
  d = ri * ri / UT2D_len_2pt (ci, pt);
  UT2D_pt_tra2ptlen (&pti, ci, pt, d);
  return pti;
}


//================================================================
  Point2 UT2D_LnInvertPoint (Point2 *pt, Line2 *ln) {
//================================================================
// UT2D_LnInvertPoint     Author: Thomas Backmeister       18.10.2007
// 
/// \code
/// UT2D_LnInvertPoint    Invert at line <-- point
/// 
/// Invert (=mirror) a point at a line.
/// The point may be infinite (=UT2D_PT_INFTY).
/// 
/// IN:
///  Point2 *pt  ... point
///  Line2 *ln   ... line of inversion
/// OUT:
/// Returncode:
///  inverse point (may be infinite)
/// \endcode


  Point2 pp, pti;
  Vector2 vc;

  //printf("UT2D_LnInvertPoint: Invert at line <-- point\n");

  if (UT2D_comp2pt (pt, &UT2D_PT_INFTY, UT_TOL_pt)) return *pt;
  UT2D_pt_projptln (&pp, pt, ln);
  UT2D_vc_2pt (&vc, pt, &pp);
  UT2D_pt_traptvc (&pti, &pp, &vc);
  return pti;
}

//=== EOF ===
