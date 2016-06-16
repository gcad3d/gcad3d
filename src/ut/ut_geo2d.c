//**************************************************************************
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
  ..

-----------------------------------------------------
Modifications:
2003-05-23 UT3D_vc_tangci statt UT3D_vc_cistart. RF.
2003-04-13 Neu: UT3D_vc_perp1vc UT3D_pl_plcoe UT3D_plcoe_pl
2003-03-28 Bei mehreren 3D_pl_ war p nicht belegt. RF.
2003-03-23 UT3D_pt_projptln, UT3D_pt_projptptvc: neuer Par. len zu !! RF.
2002-04-21 UT3D_m3_load_povxvz,UT3D_pt_rotptptvcangr neu. RF.
2002-03-20 UT2D_ci.. zu. RF.
2002-02-12 UT3D_angr_2vc__: UTP_comp_0 -> fabs. RF.
2001-10-16 UT3D_ci_obj2 u. UT3D_ci_obj: vz zu. RF.
2001-04-22 UT3D_vc_cistart,UT3D_pt_traptptlen neu zu.
2001-04-21 UT3D_vc_travcm3 korr.
2001-04-19 UT3D_vc_travcm3,UT3D_pl_XYZ neu zu.
2001-04-18 Unterschiede zu HNT:\wc3\wincut_ut_geo.cpp:
           UT3D_vc_crossprod2vc; ident mit UT3D_vc_perp2vc

-----------------------------------------------------
*/
#ifdef globTag
void UT2D(){}
#endif
/*!
\file  ../ut/ut_geo2d.c
\brief 2D geometric point vector line circle 
\code
=====================================================
List_functions_start:

-------------- 1D ----------------------------------
UT1D_ndb_npt_bp           copy the backplane-part of a pointTable

-------------- 2D ----------------------------------
UT_RADIANS                angle (radians) = angle (degrees) (inline)
UT_DEGREES                angle (degrees) = angle (radians) (inline)
UT2D_angr_set_2angr       set aa following as. Do not modify as.
UT2D_angr_set             change to val >= 0 <= RAD_360
UT2D_angd_set             change to val >= 0 <= 360
UT2D_2angr_set            change to consecutive vals from -RAD_360 to RAD_360
UT2D_angr_2angr           angle between 2 angles
UT2D_ptNr_ci              Anzahl Ecken circ berechnen
UT2D_comp2angd_p          compare 2 angles (deg) for parallel

-------------- triangles ----------------------------------
UT2D_solvtriri_a          right-angled tri: a from sides b and c
UT2D_solvtriri_c_abfc     scalene tri: c from a and x; b=x*c
UT2D_solvtri_abc          scalene tri: q and hc from sides a, b and c
UT2D_solvtriri_p_q_hc     right-angled tri: get p from q and hc
UT2D_solvtriri_p_b_q      right-angled tri: get p from b and q
UT2D_solvtriri_bc         right-angled tri: q and hc from sides b and c

-------------- side -------------------------------------
UT2D_sid_2vc              check vc for left (CCW, above) or right (CW, below)
UT2D_sidPerp_2vc          check vc for inFront, perp.(normal), or behind
UT2D_sid_3pt              compare if pt is on, above or below line (p1-p2)
UT2D_sid_ptvc             compare if pt is on, above or below line (pt+vc)
UT2D_sidPerp_3pt          check if p3 is in, before or behind perp-line tru p2
UT2D_sidPerp_ptvc         compare if pt is right/on/left of a normal to pt+vc

-------------- angles -------------------------------------
UT2D_crossprod_2vc        crossprod of 2 2D-Vectors
UT2D_acos_2vc             cos of opening angle of 2 vecs (dot=scalarprod)
UT2D_angr_angd            angle (radians) = angle (degrees)
UT2D_angd_angr            angle (degrees) = angle (radians)
UT2D_angd_invert          ang(deg.) invert (change direction)
UT2D_angr_triri_ab        right-angled tri: angle alfa from sides a and b
UT2D_angr_ptpt            angle (radians) = pt > pt
UT2D_angr_vc              angle (radians) = vector (0-360)
UT2D_angr_2ln             angle (rad.) between 2 Lines
UT2D_angr_2vc             angle (rad.) between 2 Vectors
UT2D_angr_2vc_sr          get opening angle of 2 vectors with sense of rot
UT2D_angr_3ptdr           Oeffnungswinkel von 2 Vektoren entspr. Umlaufsinn
UT2D_angr_ci              openingAngle of circle (pa,pe,pc,rad/dreh)
UT2D_angr_perpangr        angle + 90 degree (perp. to angle)
UT2D_circQuad23_angr      returns if angle is quadrant 2 or 3 (CCW from 1-4)
UT2D_circQuad34_angr      returns if angle is quadrant 3 or 4 (CCW from 1-4)

-------------- length -------------------------------------
UT2D_len_vc               length of 2D-vector             INLINE
UT2D_lenq_vc              Quadr.Vectorlength              INLINE
UT2D_lenS_vc              dx+dy-distance 2D-vector        INLINE
UT2D_len_2pt              distance pt - pt
UT2D_lenB_2pt             max(dx|dy)-distance point-point
UT2D_lenS_2pt             dx+dy-distance point-point      INLINE
UT2D_lenq_2pt             quadr. distance pt - pt         INLINE
UT2D_len_ptln             minimal dist. from point to line
UT2D_3len_ptln            minimal dist. from point to line + normalDist
UT2D_slen_vc_vc__         signed length of vector on vector
UT2D_slen_vc_vcNo         signed length of vector on norm.vector
UT2D_slen_pt_pt_vc__      signed length of point onto vector
UT2D_slen_nor2vc          signed length of normal of 2 vectors
UT2D_slen_nor_vc_vcNo     signed length of normal of 2 vectors (1 normalized)
UT2D_slenq_ptptvc         signed quadr.Distance pt-pt in Richtung vec
UT2D_slen_nor_2pt_vc__    signed length of normal of point onto vector
UT2D_slen_nor_2pt_vcNo    signed length of normal of point onto norm.vector
UT2D_slen_nor3pt          signed length of normal of point - line
UT2D_nlenq_3pt            give quadr.Distance from point to line
UT2D_nlenq_2vc            give quadr.Distance from vec on vec (point to line)
UT2D_2slen_2pt_vc__       relative coords (dx,dy) of point along/normal to line
UT2D_2slen_vc_vc__        relative coords (dx,dy) of vector on vector
UT2D_minLenB_4pt          min lenght between 2 lines
UT2D_len_cir              length circular arc (pa,pe,pc,rad/dreh)
UT2D_len_cia              length arc (from rad & angle)        INLINE
UT2D_sDist_ciSeg          height of segment of circle from angle

-------------- parameter -------------------------------------
// UT2D_parLn_pt2pt          get parameter (0-1) of point px along line p1-p2
UT2D_parvc_2vc            parameter of distance of vec1 projected on vec2
UT2D_par_3pt              parameter of point projected on line from 2 points

-------------- area --------------------------------------
UT2D_ar_3pt               get (signed) area of triangle

-------------- points --------------------------------------
UT2D_comp2pt              compare 2 2D-points
UT2D_comp4pt              compare 4 2D-points
UT2D_pt_ck_onLine         check if point is on 2D-linesegment
UT2D_pt_ck_inLine         check 2D-point on line segment or beyond limits
UT2D_pt_ck_inplg          Test if Point ptx is inside polygon pTab
UT2D_pt_ck_linear         check straightness of points (if points are linear)
UT2D_pt_cknear_npt        return index of nearest Point from n points
//UT2D_pt_ck_in3pt          check if px is between lines po-p1, po-p2
UT2D_pt_ck_inpt2vc        check if px is between vectors po-v1, po-v2
UT2D_pt_ck_inAc           check if pt is in Arc(Segment)
UT2D_pt_ck_inCv3          check if point is inside polygon
UT2D_pt_ck_int4pt         check if 2 lines intersect/touch
UT2D_i4pt_npt             find indices of extreme-points;

UT2D_pt_pt                2D-Point = 3D-Point                          INLINE
UT2D_npt_npt              tab of 2D-Points = tab of 3D-Points (copy X,Y)
UT2D_pt_pt3               2D-Point = 3D-Point                          INLINE
UT2D_pt_2db               2D-Point = 2 doubles (x, y)                  INLINE
UT2D_pt_pt3bp             2D-Point = 3D-Point on Backplane (inline)
UT2D_pt_addpt             Add two points:      po += p1                INLINE
UT2D_pt_opp2pt            opposite point (p1 = center)
UT2D_pt_mid2pt            midpoint between 2 points
UT2D_pt_mid_npt           midpoint from n points
UT2D_pt_parvc_pt_vc       point on Linesegment + parametric value.
UT2D_pt_cic_par           get 2D-point from par on 2D-circ in centerPos
UT2D_pt_par_ci            point on circle from parameter
UT2D_ptx_ci_pty           x-val of point on circle from y-val on circle
UT2D_pt_traptvc           2D-Point = 2D-Point + 2D-Vector     INLINE
UT2D_pt_traptvclen        transl. point into dir vc dist. lenv
UT2D_pt_tra2ptlen         transl. point p1 into dir p1 -> p2 dist. lenv
UT2D_pt_traPtPtPar        transl. pt with parameter along p1-p2
UT2D_pt_tra3ptlen         transl. point p1 into dir p2 -> p3 dist. lenv
UT2D_pt_tra2pt2len        transl. point into dir p1 -> p2 dist. dx and normal dy
UT2D_pt_tra2pt3len        dir=from p1 on p1->p2 offset dx and normal dy; length
UT2D_pt_traptangrlen      transl. point into dir ang dist. lenv
UT2D_pt_tranor2ptlen      transl. point normal to p1 - p2 dist. lenv
UT2D_pt_tranorptvclen     transl. point normal to vec dist. lenv
UT2D_pt_traptvc2len       transl. point into dir vc dist. dx and normal dy
UT2D_pt_tracirlen         transl. point circular length
UT2D_pt_rotptangr         rotate a point around centerpoint
UT2D_pt_projptptvc        point = project point to line (pt+vc)
UT2D_pt_projpt2pt         pp = project point pt to lineSegment p1-p2
UT2D_pt_projptln          point = project point to line
UT2D_2pt_projptci         project point to circle
UT2D_pt_projptci          project point to Arc (segment of circle)
UT2D_pt_int4pt            intersection 2 lines
UT2D_pt_intptvcy          intersection line (pt-vc) - horizontal line
UT2D_pt_intlny            intersection linesegment - horizontal line
UT2D_pt_intlnx            intersection linesegment - vertical line
UT2D_pt_intlnln           intersect Line x Line; both limited or unlimited
UT2D_pt_int2ln            point = intersection of 2 lines
UT2D_2par_int2pt2vc       intersect 2 unlimitedLines; gives 2 parameters
UT2D_pt_int2pt2vc         point = intersection of 2 vectors + 2 Points
UT2D_pt_int2vc2pt         point = intersection of 2 vectors + 2 Points
UT2D_2pt_intciptvc        intersect Circle - Line (pt+vc); both unlimited
UT2D_2pt_intlnci          intersect Line x Circ; both limited or unlimited
UT2D_2pt_intcici          intersect 2 (unlimited) Circles
UT2D_2pt_int2ci           intersect Circ x Circ; both limited or unlimited
UT2D_ptx_tang_cic_pt      get x-value where Y=0 of tangent to circle
UT2D_pt_tng_ci_vc         tangent with fixed vector to circ
UT2D_pt_tng_ci_pt         tangent from point (outside circ) to circ
UT2D_pt_traptm2           einen 2D-Punkt mit 3x2-Matrix transformieren
UT2D_pt_obj2              2D-Obj > 2D-Point
UT2D_pt_traptm3           apply transformation to point (from 4x3-matrix)

-------------- lines -------------------------------------
UT2D_ln_ptpt              2D-Line from 2 2D-points                   INLINE
UT2D_ln_ptvc              2D-Line from 2D-point and 2D-vector        INLINE
UT2D_ln_ln3               2D-Line aus 3D-Line
UT2D_ln_4db               2D-Line from 4 doubles
UT2D_ln_obj2              2D-Obj > 2D-Line
UT2D_lncoe_ln             coefficients of 2D Line (k, d)
UT2D_ln_pts_dmax          line <-- points with max. distance of pointTab
UT3D_ln_inv               invert (change p1, p2)

UT2D_ptvc_ck_int2pt       check if Line|Ray Pt-Vec intersects Line Pt-Pt

-------------- vectors -------------------------------------
UT2D_comp2vc              compare 2 vectors for parallel and antiparallel
UT2D_comp2vc_p            compare 2 vectors for parallel
UT2D_ckvc_hori            test if vector is horizontal
UT2D_ckvc_vert            test if vector is vertical
UT2D_ckvc_parl            test if 2 vectors parallel
UT2D_ckvc_in2vc           check if v2 is between v1 and v3
UT2D_ckvc_inA_vc_vc       check if v2 is between v1 and v3 (parall.& antiparall.)
UT2D_parvc_2vc            parameter of distance of vec1 projected on vec2
UT2D_2parvc_3vc           project end of vec1 along vec2 on vec3
UT2D_vc_pt                Vector from 0,0 -> point
UT2D_vc_2db               2D-Vector = 2 doubles (x, y)
UT2D_vc_vc3               2D-Vector from 3D-Vector                  INLINE
UT2D_vc_angr              2D-Vector =  angle (radians)
UT2D_vc_angrlen           2DVector =  angle (radians) + Length
UT2D_vc_vc3BP             2D-Vector = 3D-Vector on Backplane (inline)
UT2D_vc_2pt               2D-Vector = 2D-Point -> 2D-Point   INLINE
UT2D_vc_2pt3              2D-Vector = 3D-Point -> 3D-Point
UT2D_vc_2pt3_bp           2D-Vector = (3D-Point -> 3D-Point) on Backplane
UT2D_vc_2ptlen            2D-Vector = 2D-Point -> 2D-Point, set Length
UT2D_vc_ln                2D-Vector = 2D-LineStartPoint -> 2D-LineEndPoint
UT2D_vc_cipt              Tang.Vektor an einem Punkt pt des Kreises errechnen
UT2D_vc_tng_ci_pt         tangent to circ tru point on circ
UT2D_vc_invert            2D-Vector invert (change direction)
UT2D_vc_add2vc            v3 = v1 + v2                ADD           INLINE
UT2D_vc_sub2vc            v3 = v1 - v2                SUB           INLINE
UT2D_vc_multvc            vo = vi * d                 MULT          INLINE
UT2D_vc_perpvc            vector = perpendic. to vector ( + 90 degrees)
UT2D_vc_perp2pt           vector = perpendic. to Line ( + 90 degrees)
UT2D_vc_normalize         change to length 1
UT2D_vc_setLength         change 2D-Vectorlength
UT2D_vc_mid2vc            vector = middle between 2 vectors
UT2D_vc_merge2vc          merge 2 vectors   vNew = (v1 + v2) / 2
UT2D_vc_rotangr           rotate a 2D_vector
UT2D_vc_travcm2           2D-vector transformation (3x2-matrix)
UT2D_vc_tra_vcx_vcy       translate vector int refsys from vcx,vcy

-------------- Circ2C --------------------------------------
UT2D_cic_ci3              get 2D-circle in centerPosition from 3D-circ

-------------- Circ2 --------------------------------------
UT2D_angr_ciSec           opening angle of Secant of Circ
UT2D_len_ciSec            cmp length of secant from radius + heght of secant
UT2D_compPtOnAc           is Point on Arc
UT2D_ck_ci360             check if circ is 360-degree-circ
UT2D_ci_ci3               2D-Circ = 3D-Circ
UT2D_ci_ptrd              360-deg 2D-Circ from center, radius
UT2D_ci_pt2vcrd           circ from center, 2 vectors & radius
UT2D_ci_ptvcpt            durch Punkt+Richtung, durch 2. Punkt
UT2D_ci_ptvcptvc          durch Punkt+Richtung, tangential an Linie (pt+vc)
UT2D_ci_ptvcci            durch Punkt+Richtung, tangential an Circ (pc+r)
UT2D_ci_obj2              2D-Circ = 2D-ObjG
UT2D_ci_2vc2ptrd          2D-Circ; verrundet 2 Lines; 4 Lösungen
UT2D_ci_ciptvcrd          2D-Circ; verrundet Circ/Line; 4 Lösungen

-------------- GrafObj - (see also UTO_) ---------------
UT2D_obj_obj3             change 3D-Obj > 2D-Obj (remove Z-val)
UT2D_obj_pt3              change 3D-Point > 2D-Obj
UT2D_obj_ln3              change 3D-Line > 2D-Obj
UT2D_obj_ci2              change 2D-Circ > 2D-Obj
UT2D_obj_ci3              change 3D-Circ > 2D-Obj
UT2D_obj_cv3              change 3D-Curv > 2D-Obj

UT2D_void_obj2            change ObjG2-Objekt -> data-Obj

-------------- polygon --------------------------------
UT2D_cv_ci                change circle > polygon (schnell)  DO NOT USE
UT2D_cv_ci_               get circular polygon from circle
UT2D_cv_ln                Linearstueck -> Polygon / Anzahl
UT2D_npt_ci               circular polygon
UT2D_cv_cin               Circ -> Polygon / Anzahl
UT2D_cv_ci360             Vollkreis --> Polygon
UT2D_cv_inv               Punktetabelle umdrehen
UT2D_cv3_linear           delete unnecesary straight points
UT2D_srar_3pt             Umlaufsinn und Flaeche eines 2D-triangle
UT2D_srar_polc            Umlaufsinn und Flaeche eines closed polygon
UT2D_srar_inpt            Umlaufsinn und Flaeche eines indexed closed polygon
UT2D_srar_inpt3           Umlaufsinn und Flaeche eines indexed closed 3D-polygon

-------------- transformation ------------------------------
UT2D_m2_load              2D-Achsensystem (Verdrehvektor, Ori.) into 3x2 Mat
UT2D_m2_loadtravcm2       load 2D-vector trafo (3x2 matrix)
UT2D_m2_invtravcm2        invert 2D-vector transformation
UT2D_m3_init_rot          Initialize a 3x2 - matrix with rotation

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



new struct Circ2C (circle in centerPosition, ci2c, Typ_CI2C) ?
typedef struct {Point2 p1, p2; double double rad, ango;}      Circ2C;
  similar CurvEll2C (ellipse in centerPosition, ell2c, Typ_CVELL2C)



=====================================================
*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


#include <stdarg.h>


#include "../ut/func_types.h"                  // SYM_SQUARE ..
 
// #include "../ut/ut_umem.h"             // UME_reserve
#include "../ut/ut_geo.h"
#include "../ut/ut_ox_base.h"             // OGX_SET_INDEX
#include "../ut/ut_math.h"
#include "../ut/ut_bspl.h"                // UT3D_par_par1BSp
#include "../ut/ut_plg.h"                 // UT3D_par_par1plg
#include "../ut/ut_elli.h"                // UT3D_angr_par1_ell
#include "../ut/ut_TX.h"                  // TX_Error

#define INCLUDE_FULL
#include "../ut/ut_geo_const.h"           // the constants ..
#undef INCLUDE_FULL
#include "../ut/ut_tol_const.h"           // the constants ..
#include "../ut/ut_const.h"               // the constants ..








//==========================================================================
  int UT2D_npt_npt (Point2 *p2a, Point *pa, int pNr) {
//==========================================================================
/// \code
/// UT2D_npt_npt              tab of 2D-Points = tab of 3D-Points (copy X,Y)
/// \endcode

// es sollte Normalvektor benutzt werden; dzt copy X- & Y-Coord only.

  int    i1;

  for(i1=0; i1<pNr; ++i1) UT2D_pt_pt (&p2a[i1], &pa[i1]);

  return 0;

}


//===========================================================================
  int UT2D_ptvc_ck_int2pt (int mode, Point2 *p1s, Vector2 *v1,
                                     Point2 *p2s, Point2 *p2e) {
//===========================================================================
/// \code
/// UT2D_ptvc_ck_int2pt       check if Line|Ray Pt-Vec intersects Line Pt-Pt
/// Line = both sides limited; Ray = one side unlimited.
///
/// Input:
///   mode = 0  p1s-v1: both sides unlimited; p2s-p2e: both sides limited.
///   mode = 1  p1s-v1: one side unlimited; p2s-p2e: both sides limited.
///
/// Returncodes:
/// -1 = no intersection between unlimited Line p1s-v1 and segment p2s-p2e.
///  0 = OK; lines intersect.
///  1 = p1s-v1 goes tru p2s
///  2 = p1s-v1 goes tru p2e
///  3 = p1s-v1 and p2s-p2e are parallel; no intersection-checks yet !
///  4 = p1s lies exact on p2s-p2e.     (ONLY if mode=1)
///  5 = p1s and p2s are equal          (ONLY if mode=1)
///  6 = p1s and p2e are equal          (ONLY if mode=1)
/// \endcode

// see UT2D_pt_ck_int4pt UT2D_ckvc_in2vc

  int       irc, s2sv1, s2ev1, s3;
  double    d1;
  Vector2   v1s2s, v1s2e, v2;


  // printf("UT2D_ptvc_ck_int2pt %d\n",mode);
  // UT3D_stru_dump (Typ_PT2, p1s, " p1s:");
  // UT3D_stru_dump (Typ_VC2, v1, " v1:");
  // UT3D_stru_dump (Typ_PT2, p2s, " p2s:");
  // UT3D_stru_dump (Typ_PT2, p2e, " p2e:");
  // GR_Disp_pt2 (p1s, SYM_STAR_S, 0);
  // GR_Disp_vc2 (v1, p1s, 0, 0);
  // GR_Disp_ln2 (p2s, p2e, 9);


  // set s2sv1 = side of p2s along v1
  UT2D_vc_2pt (&v1s2s, p1s, p2s);
  d1 = UT2D_sar_2vc (v1, &v1s2s);
  s2sv1 = DSIGTOL (d1, UT_TOL_min0);      // pos: p2s is left of p1s-v1
    // printf(" s2sv1=%d d1=%f\n",s2sv1,d1);


  // set s2ev1 = side of p2e along v1
  UT2D_vc_2pt (&v1s2e, p1s, p2e);
  d1 = UT2D_sar_2vc (v1, &v1s2e);
  s2ev1 = DSIGTOL (d1, UT_TOL_min0);      // pos: p2e is left of p1s-v1
    // printf(" s2ev1=%d d1=%f\n",s2ev1,d1);



  if((s2sv1 != 0)&&(s2ev1 != 0)) {
    if(s2sv1 == s2ev1) {irc = -1; goto L_exit;}
  }



  if(mode == 0) {      // v1: both sides unlimited
    if(s2sv1 == 0) {       // p1s-v1 tru p2s
      if(s2ev1 == 0) {     // p1s-v1 tru p2e
        irc = 3; goto L_exit;   // p1s-v1 and p2s-p2e are parallel.
      }
      // if(s3 == 0) {irc = 5; goto L_exit;}
      irc = 1; goto L_exit;     // p1s-v1 goes tru p2s
    }
    if(s2ev1 == 0) {  
      // if(s3 == 0) {irc = 6; goto L_exit;}
      irc = 2; goto L_exit;     // p1s-v1 goes tru p2e
    }
    irc = 0; goto L_exit;
  }


  // intersection-test if p1s-v1 is limited ray:

  // set v2 = vec(p2s, p2e)
  // set s3 = side of p1s along v2
  if((s2sv1 != 0)||(s2ev1 != 0)) {
    UT2D_vc_2pt (&v2, p2s, p2e);
    d1 = UT2D_sar_2vc (&v2, &v1s2s);
    s3 = DSIGTOL (d1, UT_TOL_min0);      // pos: p1s is left of p2s-v2
      // printf(" s3=%d d1=%f\n",s3,d1);
  }


  //-------------- p1s-v1 and p2s-p2e are parallel; 3 or -2.
  if(s2sv1 != 0) goto L_S2;
  if(s2ev1 != 0) goto L_S1;
    // s2sv1=0; s2ev1=0;
    // UT2D_vc_2pt (&v2, p2s, p2e);
    // s3 = UT2D_sidPerp_ptvc (p1s, p2s, v2); 
    irc = 3;
    goto L_exit;
 

  //-------------- p1s-v1 tru p2s; irc=1.
  L_S1:
    // s2sv1=0; s2ev1!=0;
    if(s3 == 0) {irc = 5; goto L_exit;}
    if(s2ev1 == s3) {irc = -1; goto L_exit;}
    irc = 1;
    goto L_exit;


  //-------------- p1s-v1 tru p2e; irc=2.
  L_S2:
  if(s2ev1 != 0) goto L_S3;
    // s2sv1!=0; s2ev1=0;
    if(s3 == 0) {irc = 6; goto L_exit;}
    if(s2sv1 != s3) {irc = -1; goto L_exit;}
    irc = 2;
    goto L_exit;


  //-------------- p1s lies exact on p2s-p2e. irc=4.
  L_S3:
  if(s3 != 0) goto L_SN;
    irc = 4;
    goto L_exit;

   
  //-------------- normal intersection.
  L_SN:
    // if p2s=right, p2e=left: s3 = side of p2s at p2s-p2e
    // if p2s=left, p2e=right: s3 = side of p2e at p2e-p2s
    // s3 = left: intersection ok.
    // s3 = right: no intersection.
    // s3 = ON: p1s lies exact on line.
    if(s2sv1 < 0) {
      if(s3 > 0) {irc = -1; goto L_exit;}
    } else {
      if(s3 < 0) {irc = -1;goto L_exit;}
    }
    irc = 0;


  L_exit:
    // printf("ex UT2D_ptvc_ck_int2pt %d\n",irc);
  return irc;

}



//================================================================
  int UT1D_ndb_npt_bp (double *da, Point *pa, int pNr, int bp) {
//================================================================
/// UT1D_ndb_npt_bp          copy the backplane-part of a pointTable

  int    i1;

  // printf("UT1D_ndb_npt_bp %d %d \n",pNr,bp);


  for(i1=0; i1<pNr; ++i1)

  if(bp == BCKPLN_YZ) {            // 0  =  Y-Z-Plane, use X
    for(i1=0; i1<pNr; ++i1)    da[i1] = pa[i1].x;

  } else if(bp == BCKPLN_XZ) {     // 1  =  X-Z-Plane, use Y
    for(i1=0; i1<pNr; ++i1)    da[i1] = pa[i1].y;

  } else if(bp == BCKPLN_XY) {     // 2  =  X-Y-Plane, use Z
    for(i1=0; i1<pNr; ++i1)    da[i1] = pa[i1].z;
  }


  // for(i1=0; i1<pNr; ++i1) printf(" da[%d]=%f\n",i1,da[i1]);

  return 0;

}


//================================================================
  double UT2D_angr_set_2angr (double as, double aa, int sr) {
//================================================================
/// \code
/// UT2D_angr_set_2angr        set aa following as. Do not modify as.
///   as         startAngle; not modified.
///   aa         following angle; must be > as for CCW; must be < as for CW.
///   sr         1=CCW; else CW
/// Output:
///   retVal    if(sr==1) then retVal > as;
///             else           retVal < as;
///  as and retVal may have identical position.
/// Examples:
///   as=0.; aa=1; sr=1;  retVal=-5
/// \endcode

// see also UT2D_angr_set UT2D_angr_2angr UT2D_2angr_set


  // printf("UT2D_angr_set_2angr as=%lf aa=%lf sr=%d\n",as,aa,sr);

  if(sr > 0) {     // CCW; aa must be > as
    if(aa < as)              aa += RAD_360;
    // else if(aa > as+RAD_360) aa -= RAD_360;   // EXAMPLE ?? // 2014-04-10 raus.

  } else {         // CW; aa must be < as
    if(aa > as)              aa -= RAD_360;
    // else if(aa > as-RAD_360) aa += RAD_360;   // EXAMPLE ?? // 2014-04-10 raus.
  }

    // printf("ex UT2D_angr_set_2angr as=%lf aa=%lf sr=%d\n",as,aa,sr);

  return aa;

}


//================================================================
  int UT2D_angr_set (double *angr) {
//================================================================
/// UT2D_angr_set          change to val >= 0 <= RAD_360

  double    *tol = &UT_TOL_PAR;

  // printf("UT2D_angr_set %f\n",*angr);


  // make 0.00001 --> 0.
  if(fabs(*angr) < *tol) { *angr = 0.; goto L_fertig; }

  // make 360.001  -->  360.
  if(fabs(fabs(*angr) - RAD_360) < *tol) {*angr = RAD_360;goto L_fertig;}

  // make -90. --> +270.
  while (*angr < 0.) *angr += RAD_360;

  // make 361  -->  1.
  while (*angr > RAD_360) *angr -= RAD_360;

  // if (*angr < UT_TOL_min1) *angr = 0.;
  // else if (*angr > (RAD_360 - UT_TOL_min1)) *angr = 0.;


  L_fertig:
  // printf("ex UT2D_angr_set %f\n",*angr);
  return 0;
}


//================================================================
  int UT2D_angd_set (double *angd) {
//================================================================
/// UT2D_angd_set          change to val >= 0 <= 360

  double    *tol = &UT_TOL_Ang1;

  // printf("UT2D_angd_set %f\n",*angr);


  // make 0.00001 --> 0.
  if(fabs(*angd) < *tol) { *angd = 0.; goto L_fertig; }

  // make 360.001  -->  360.
  if(fabs(fabs(*angd) - 360.) < *tol) {*angd = 360.;goto L_fertig;}

  // make -90. --> +270.
  while (*angd < 0.) *angd += 360.;

  // make 361  -->  1.
  while (*angd > 360.) *angd -= 360.;


  L_fertig:
  // printf("ex UT2D_angr_set %f\n",*angr);
  return 0;
}



//================================================================
  int UT2D_2angr_set (double *ang1, double *ang2, int irot) {
//================================================================
/// \code
/// UT2D_2angr_set     change consecutive values from -RAD_360 to RAD_360
/// if irot == 1 (CCW) ang2 > ang1
/// if irot == -1 (CW) ang2 < ang1
/// \endcode

  double    da;
  double    *tol = &UT_TOL_0Cos; //&UT_TOL_Ang1;


  // printf("UT2D_2angr_set %f %f %d\n",*ang1,*ang2,irot);
  // printf("  tol=%lf\n",*tol);


  da = *ang2 - *ang1;
  if(fabs(da + RAD_360) < *tol) {
    if(irot < 0) {   // CW;
      *ang2 = *ang1 - RAD_360;
    } else {         // CCW
      *ang2 = *ang1 + RAD_360;
    }
    goto L_exit;
  }

  
  UT2D_angr_set (ang1);
  UT2D_angr_set (ang2);
    // printf(" ang1=%lf ang2=%lf da=%lf\n",*ang1,*ang2,da);


  if(irot < 0) {   // CW;
    if(*ang1 < *ang2) *ang2 -= RAD_360;


  } else {         // CCW
    if(*ang2 < *ang1) *ang2 += RAD_360;        // 2014-02-20
  }

  L_exit:
    // printf("ex UT2D_2angr_set %f %f %d\n",*ang1,*ang2,irot);
  return 0;
}


//================================================================
  double UT2D_angr_2angr (double ang1, double ang2, int irot) {
//================================================================
/// \code
/// UT2D_angr_2angr                   angle between 2 angles (opening-angle)
/// irot =  1   CCW
/// irot = -1   CW
/// \endcode

  double a1, a2, da;

  a1 = ang1;
  a2 = ang2;
  UT2D_2angr_set (&a1, &a2, irot);

  if(irot < 0) {
    da = a1 - a2;
  } else {
    da = a2 - a1;
  }

  // printf("ex UT2D_angr_2angr %f %f %f %d\n",da,ang1,ang2,irot);

  return da;

}

/*
//================================================================
  int UT2D_comp2angd__ (double d1, double d2) {
//================================================================
// UT2D_comp2angd__      compare 2 angles (deg) for parallel and antiparallel

  int  irc;

  printf("ex UT2D_comp2angd__ %d %d %d\n",irc,d1,d2);

  // wie UT2D_comp2angd_p nur 2 x 180 Grad addieren ..

  return irc;

}
*/

//================================================================
  int UT2D_comp2angd_p (double d1, double d2, double tol) {
//================================================================
/// \code
/// UT2D_comp2angd_p      compare 2 angles (deg) for parallel
/// Retcode 0 = no, angles not parallel
/// Retcode 1 = yes, angles are parallel
/// \endcode

  int  i1;

  // printf("UT2D_comp2angd_p %f %f %f\n",d1,d2,tol);

  UT2D_angr_set (&d1);
  UT2D_angr_set (&d2);

  // test  0 - 0
  i1 = UTP_comp2db(d1,d2,tol);
  if(i1 != 0) goto L_ex;             // jes, parallel.

  // test  0 - 360
  if(d1 < d2) d1 += RAD_360;
  else        d2 += RAD_360;

  i1= UTP_comp2db(d1,d2,tol);;

  L_ex:
  // printf("ex UT2D_comp2angd_p %d %f %f\n",i1,d1,d2);

  return i1;

}


/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/


//================================================================
  int UT2D_solvtriri_a (double *a, double b, double c) {
//================================================================
/// \code
/// UT2D_solvtriri_a         right-angled tri: a from sides b and c
///
///               C              Der rechte Winkel liegt im Punkt C.
///             / . \
///           /       \
///         b           a
///       /               \
///     A -------c-------- B      c = hypotenuse
///
///  a*a + b*b = c*c
///  a = sqrt (c*c - b*b)
///
///
/// Input: the length of the sides  b, c. 
///   c    length of baseline, from point A to point B;
///   b    length of side left, from point A to point C;
/// 
/// Output:
///   a    length of side right, from point B to point C;
/// 
/// Retcodes:
///   0    OK
/// \endcode


  if(b >= c) *a = 0.;

  else       *a = sqrt (c * c - b * b);


  // printf("ex UT2D_solvtriri_a a=%f b=%f c=%f\n",*a,b,c);
  return 0;

}


//================================================================
  int UT2D_solvtriri_c_abfc (double *c, double a, double x) {
//================================================================
/// \code
/// UT2D_solvtriri_c_abfc          scalene tri: c from a and x; b=x*c;
/// Allgemeines Dreieck. a ist gegeben, b = x * c; c ist gesucht.
/// Used for Rat.B-spl.Crv --> Circ (UT3D_ci_rbspl3)
///
///       +
///       |\
///       | \
///      a|  \c
///       |   \
///       +----+
///         b
/// b=x*c;
/// a*a + x*x*c*c = c*c
/// c=sqrt(a*a/((x*x)-1)
///
/// Input:
///     a = length of side a
///     x = factor;   b = c * x;
/// Output:
///     c = length of side c
///
/// /// Retcodes:
///   0    OK
///  -1    Error (x=1)

/// \endcode

  int     irc;
  double  x2;

  // printf("UT2D_solvtriri_c_abfc %f %f\n",a,x);

  x2 = (x * x) - 1.;
  if(fabs(x2) < UT_TOL_min2) {
    *c = UT_VAL_MAX;
    irc = -1;

  } else {

    *c = sqrt(fabs(a * a / x2));
    irc = 0;
  }

  // printf("ex UT2D_solvtriri_c_abfc %f %d %f\n",*c,irc,x2);

  return irc;

}


//================================================================
  int UT2D_solvtriri_bc (double *q,double *hc,double b,double c) {
//================================================================
/// \code
/// UT2D_solvtriri_ac         right-angled tri: q and hc from sides b and c.
/// 
///                C           Right angle is in point C
///              / | \
///            /   |   \
///          b    hc     a
///        /       |       \
///      A -------c-------- B
///            q      p
/// 
/// Input: the length of the sides a, b, c.
///   c    is the baseline, from point A to point B;
///   b    length of side left, from point A to point C;
/// 
/// Output:
///   q    the length of b projected to c; (with direction - can be negativ !)
///   hc   the length of the normal standing on c
///        hc parts c into q (lying under b) and p (lying under a)
/// 
/// Retcodes:
///   0    OK
///  -1    Error (b>c)
/// \endcode



  b = fabs(b);
  c = fabs(c);


  if (UTP_comp_0 (b)) {
    *hc = 0.;
    *q = 0.;


  } else if (UTP_comp2db (b, c, UT_TOL_min1)) {
    *hc = 0.;
    *q = c;


  } else if ((UTP_comp_0 (c)) ||
             (b > c)) {
      printf("*** Error UT2D_solvtriri_ac\n");
      return -1;


  } else {
    *q = b*b / c;
    *hc = sqrt(*q * (c - *q));

  }

    // printf("UT2D_solvtriri_ac %f %f %f %f\n",*q,*hc,b,c);

  return 0;

}


//================================================================
  int UT2D_solvtriri_p_q_hc (double *p, double q, double hc) {
//================================================================
/// \code
/// UT2D_solvtriri_p_q_hc     right-angled tri: get p from q and hc
/// 
///                C           Right angle is in point C
///              / | \
///            /   |   \
///          b    hc     a
///        /       |       \
///      A -------c-------- B
///            q      p
/// 
/// Input: the length of the sides a, b, c.
///   q    lenght from point A to intersection with hc
///   hc   length of the normal standing on c
/// 
/// Output:
///   p    the length of a projected to c
/// 
/// Retcodes:
///   0    OK
///  -1    Error (b>c)
/// \endcode

// hc * hc = p * q


  *p = (hc * hc) / q;

    printf("ex UT2D_solvtriri_p_q_hc %lf %lf %lf\n",*p,q,hc);

  return 0;

}


//================================================================
  int UT2D_solvtriri_p_b_q (double *p, double b, double q) {
//================================================================
/// \code
/// UT2D_solvtriri_p_b_q     right-angled tri: get p from b and q
/// 
///                C           Right angle is in point C
///              / | \
///            /   |   \
///          b    hc     a
///        /       |       \
///      A -------c-------- B
///            q      p
/// 
/// Input: the length of the sides a, b, c.
///   q    lenght from point A to intersection with hc
///   hc   length of the normal standing on c
/// 
/// Output:
///   p    the length of a projected to c
/// 
/// Retcodes:
///   0    OK
///  -1    Error (b>c)
/// \endcode

// b * b = c * q

  double c;


  c = (b * b) / q;


  *p = c - q;

    // printf("ex UT2D_solvtriri_p_b_q p=%lf b=%lf q=%lf c=%lf\n",*p,b,q,c);

  return 0;

}


//============================================================ 
  int UT2D_solvtri_abc (double *q, double *hc,
                        double  a, double  b, double  c) {
//============================================================ 
/// \code
/// UT2D_solvtri_abc          scalene tri: q and hc from sides a, b and c.
/// 
/// 
///                C               Allgemeines Dreieck. Kein rechter Winkel.
///              / | \
///            /   |   \
///          b    hc     a
///        /       |       \
///      A -------c-------- B
///            q      p
/// 
/// 
/// Input: the length of the sides a, b, c.
///   c    is the baseline, from point A to point B;
///   b    length of side left, from point A to point C;
///   a    length of side right, from point B to point C;
/// 
/// Output:
///   q    the length of b projected to c; (with direction - can be negativ !)
///   hc   the length of the normal standing on c
///        hc parts c into q (lying under b) and p (lying under a)
/// 
/// Retcodes:
///   0 - Ok.
///  -1 - error; c = 0.;
///  -2 - error; b > a + c;
///  -3 - error; a > b + c;
///   
/// 
/// \endcode

    int    rc;
    double a_2, b_2, c_2;

/* ---------------------------------------------------------------------- */

  *hc = 0.;
  *q = 0.;


  /* c cannot be 0 ! */
  if (UTP_comp_0 (c)) {
      return -1;
  }


  a = fabs(a);
  b = fabs(b);
  c = fabs(c);


  /* c = b+a || c = b-a */
  if ((UTP_comp2db (c, b+a, UT_TOL_min1)) ||
      (UTP_comp2db (c, b-a, UT_TOL_min1))) {
    *q = b;
    rc = 0;
    goto Fertig;
  }

  /* c = a-b */
  if  (UTP_comp2db (c, a-b, UT_TOL_min1)) {
    *q = -b;
    rc = 0;
    goto Fertig;
  }


  a_2 = a*a;
  b_2 = b*b;
  c_2 = c*c;


  /* B**2 > A**2 + C**2 */
  if (b_2 > (a_2 + c_2)) {
    /* printf("rechts\n"); */
    if (c + a < b) {
      rc = -2;
      goto Fertig;
    }
    /* rc = 1; */
    *q = c + (b_2 - a_2 - c_2) / (c * 2.);


  /* A**2 > B**2 + C**2 */
  } else if (a_2 > (b_2 + c_2)) {
    /* printf("links\n"); */
    if (c + b < a) {
      rc = -3;
      goto Fertig;
    }
    /* rc = 2; */
    *q = c + (b_2 - a_2 - c_2) / (c * 2.);


  } else {
    /* printf("mittig\n"); */
    *q = (c_2 + b_2 - a_2) / (c * 2.);

  }


  rc = 0;
  *hc = sqrt (fabs(b_2 - *q * *q));


  Fertig:
  /* printf ("UT2D_solvtri_abc %f %f %d\n", *q, *hc, rc); */
  return rc;

}




/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/



//================================================================
  int UT2D_sidPerp_3pt (Point2 *p1, Point2 *p2, Point2 *p3) {
//================================================================
/// \code
/// UT2D_sidPerp_3pt        check if p3 is in, before or behind perp-line tru p2
/// Ist p3 vor oder hinter der Linie, die durch p2 geht u.normal auf p1-p2 steht
/// RC  -1 p3 liegt vor Ebene p1-p2 (vis-a-vis)
/// RC   0 p3 liegt genau in der Ebene
/// RC   1 liegt hinter Ebene p1-p2 (in der Naehe von p2)
///
///             p3     |
///             x      |
///             .      |
///             . -1   0    +1
///             .      |
///     x--------------x
///    p1              p2
///
/// see also UT3D_acos_2vc
/// \endcode

  int     irc;
  double  sk;
  Vector2 v1, v2;


  // UT3D_stru_dump (Typ_PT2, p3, "UT2D_sidPerp_3pt  p3");
  // UT3D_stru_dump (Typ_PT2, p1, "p1");
  // UT3D_stru_dump (Typ_PT2, p2, "p2");


  UT2D_vc_2pt (&v1, p1, p2);
  UT2D_vc_2pt (&v2, p2, p3);

  sk = v1.dx * v2.dx + v1.dy * v2.dy;     // UT2D_skp_2vc

  // irc = UTP_COMP_0(sk);

  if(sk < -UT_TOL_min2)     irc = -1;
  else if(sk > UT_TOL_min2) irc = 1;
  else                      irc = 0;

    // printf("ex UT2D_sidPerp_3pt %d %f\n",irc,sk);

  return irc;

}


/*
//================================================================
  double UT2D_acos_2vc (Vector2 *v1, Vector2 *v2) {
//================================================================
// UT2D_acos_2vc             cos of opening angle of 2 vecs (scalarproduct)
//
// scalarproduct

// Oeffnungswinkel von 2 Vektoren:
//   Oeffnungswinkel = ACOS(UT2D_acos_2vc (&v1, &v2));
//   Vorher unbedingt UT2D_vc_normalize !
//   Der Oeffnungswinkel ist immer positiv und max 180 Grad !

// 2D-Projektionslaenge eines Vektors auf einen anderen Vektor.
//  v2 wird auf v1 projiziert; out Laengenparamter (mit Vorzeichen!)
//  Wenn v1 == X-Achse, waere Laenge der DX-Wert des v2 in der X-Y-Plane.

//  Wahre_Laenge = sk / len_v1
//    oder v1 normieren.


  return (v1->dx * v2->dx + v1->dy * v2->dy);
}
*/



//======================================================================
  double UT2D_angr_angd (double angd) {
//======================================================================
/// UT2D_angr_angd           angle (radians) = angle (degrees)
///

  double angr;

  /* -----------------------------------------*/

  /* angr = angd * RAD_180 / 180.0; */

  angr = angd * RAD_1;


  /* printf("UT2D_angr_angd %f %f\n",angr,angd); */
  return angr;
}




//======================================================================
  double UT2D_angd_angr (double angr) {
//======================================================================
/// UT2D_angd_angr           angle (degrees) = angle (radians)



  double angd;

  /* -----------------------------------------*/

  /* angd = angr / RAD_180 * 180.0; */

  angd = angr / RAD_1;

  /* printf("UT2D_angr_angd %f %f\n",angd,angr); */
  return angd;
}


//================================================================
  double UT2D_angd_invert (double *andi) {
//================================================================
/// UT2D_angd_invert          ang(deg.) invert (change direction)

  double angdo;

  if(*andi >= 180.)  angdo = *andi - 180.;
  else               angdo = *andi + 180.;

  printf("ex UT2D_angd_invert %f %f\n",*andi,angdo);
  return angdo;

}


//=============================================================
  double UT2D_angr_triri_ab (double b, double a) {
//=============================================================
/// \code
/// UT2D_angr_triri_ab        right-angled tri: angle alfa from sides a and b.
/// 
/// c = hypotenuse (baseline),
/// b = side left,
/// a = side right,
/// the sides b - a include the right-angle.
/// 
/// Returns the angle alfa (in rad; which is included by the sides b and c).
/// \endcode


  double aa;

  a = fabs(a);
  b = fabs(b);

  if ((a < UT_TOL_min1) ||
      (b < UT_TOL_min1)) {
    aa = 0.;
    goto Fertig;
  }

  if (a < b) {
      aa = atan (a / b);
  } else {
      aa = RAD_90 - atan (b / a);
  }

  Fertig:
  /* printf("UT2D_angr_triri_ab %f %f %f %f\n",aa,UT_DEGREES(aa),a,b); */
  return aa;

}


//======================================================================
  double UT2D_angr_ptpt (Point2 *pt1, Point2 *pt2) {
//======================================================================
/// UT2D_angr_ptpt            angle (radians) = pt > pt


  Vector2  vc1;

  vc1.dx = pt2->x - pt1->x;
  vc1.dy = pt2->y - pt1->y;

  return UT2D_angr_vc (&vc1);


}


//======================================================================
  double UT2D_angr_vc (Vector2 *vc) {
//======================================================================
/// \code
/// UT2D_angr_vc              angle (radians) = vector (0-360)
/// returns 0 - RAD_360 (PI * 2)
/// Example: (1,1) returns RAD_45 (0.785)
/// \endcode



  double angr;

  /* parallel to Y - */
  /* if (vc->dx == 0.0) { */
  if (UTP_comp_0 (vc->dx)) {
    if (vc->dy > 0.0 ) {
      angr = RAD_90;
    }
    else if (vc->dy < 0.0 ) {
      angr = RAD_180 + RAD_90;
    }
    else {
      angr = 0.;
    }
    goto Fertig;
  }


  /* parallel to X - */
  /* if (vc->dy == 0.0) { */
  if (UTP_comp_0 (vc->dy)) {
    if (vc->dx > 0.0 ) {
      angr = 0.;
    }
    else if (vc->dx < 0.0 ) {
      angr = RAD_180;
    }
    else {
      angr = 0.;
    }
    goto Fertig;
  }


  if ( fabs(vc->dx) < fabs(vc->dy)) {
    /* printf(" > 45 \n"); */
    angr = atan (vc->dy / vc->dx);
    if ( vc->dx < 0.0 ) angr = angr + RAD_180;
  } else {
    /* printf(" < 45\n"); */
    angr = RAD_90 - (atan (vc->dx / vc->dy));
    if ( vc->dy < 0.0 ) angr = angr + RAD_180;
  }

  if ( angr < 0.0 ) angr = angr + RAD_360;    /* + 360 */



  Fertig:
  /* printf("UT2D_angr_vc %f %f\n",angr, angr/RAD_1); */

  return angr;
}



//====================================================================
  double UT2D_angr_2ln (Line2 *ln1, Line2 *ln2) {
//====================================================================
/// UT2D_angr_2ln             angle (rad.) between 2 Lines


  Vector2   vc1, vc2;

  /* ------------------------------------------ */

  /*  Lines > Vectors */
  UT2D_vc_ln (&vc1, ln1);
  UT2D_vc_ln (&vc2, ln2);

  return UT2D_angr_2vc (&vc1, &vc2);

}


//================================================================
  double UT2D_angr_2vc (Vector2 *vc1i, Vector2 *vc2i) {
//================================================================
/// \code
/// UT2D_angr_2vc             angle (rad.) between 2 Vectors
/// Die Vektoren muessen CCW liegen; vc1 entspricht der X-Achse; vc2 CCW davon.
/// \endcode

  double    angr;
  Vector2   vc1, vc2;



  /*  > Länge 1 */
  UT2D_vc_setLength (&vc1, vc1i, 1.);
  UT2D_vc_setLength (&vc2, vc2i, 1.);

  // printf("UT2D_angr_2vc vc1=%f,%f\n",vc1.dx,vc1.dy);
  // printf("              vc2=%f,%f\n",vc2.dx,vc2.dy);


  /*  Check for 180 Grad */
  if((UTP_comp_0(vc1.dx+vc2.dx))&&(UTP_comp_0(vc1.dy+vc2.dy))) {
    /* TX_Print(" 180 Grad!"); */
    return RAD_180;
  }


  //  Skalarprodukt = Winkel; pos im Q1 & 3.
  angr = acos(UT2D_acos_2vc (&vc1, &vc2));
    // printf(" angr=%f\n",angr);


  //  oben - unten mit Z-Vektor; neg wenn vc2 > 180 deg (vc1=X-achse)
  // liefert von 0 bis 180 rad normal;
  // ab 180 Grad werden negative Werte geliefert,
  // aber als rest auf 360 Grad. Also kommt bei 190 Grad -170!
    // printf(" Xprod=%f\n",UT2D_crossprod_2vc(&vc1, &vc2));
  // if (UT2D_crossprod_2vc(&vc1, &vc2) < 0.)  angr = RAD_360 - angr;
  if (UT2D_sar_2vc(&vc1, &vc2) < 0.)  angr = RAD_360 - angr;

  // printf("ex UT2D_angr_2ln %f\n",angr);

  return angr;

}


//===========================================================================
  double UT2D_angr_3ptdr (Point2 *pa, Point2 *pe, Point2 *pc, int dreh) {
//===========================================================================
/// DO NOT USE; use UT2D_angr_ci
// UT2D_angr_3ptdr        Öffnungswinkel von 2 Vektoren entsprechend Umlaufsinn
// dreh: CW od CCW
// 
// Den Öffnungswinkel von 2 Vektoren (Kreis) entsprechend Umlaufsinn errechnen;
// Der Winkel ist immer positiv !


  double  wa, we, wd;


  wa = UT2D_angr_ptpt (pc, pa);
  we = UT2D_angr_ptpt (pc, pe);


  /*  Befindet sich Winkel w1 zwischen Winkel wa und we ?? */
  if(dreh == CCW) {

    while (we < wa)  { we += RAD_360; }
    wd = we - wa;


  } else {
    while (wa < we)  { wa += RAD_360; }
    wd = wa - we;

  }


  // printf("ex UT2D_angr_3ptdr %f %f %f\n",wd,wa,we);

  return wd;

}








/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/



/*
//====================================================================
  double UT2D_len_vc (Vector2 *vc) {
//===============
// UT2D_len_vc              2D-Vectorlength of 2D-vector

  return (sqrt(vc->dx*vc->dx + vc->dy*vc->dy));

}


//================================================================
  void UT2D_lenq_2pt (double *lenq, Point2 *p1,Point2 *p2) {
//================================================================
// UT2D_lenq_2pt        quadr. distance pt - pt

  double dx, dy;

  dx = p2->x - p1->x;
  dy = p2->y - p1->y;

  lenq = dx*dx + dy*dy;

}
*/

 
//====================================================================
  double UT2D_len_2pt (Point2 *p1,Point2 *p2) {
//====================================================================
/// UT2D_len_2pt              distance pt - pt

  double dx, dy;

  dx = p2->x - p1->x;
  dy = p2->y - p1->y;

  // printf("UT2D_len_2pt dx=%f dy=%f\n",dx,dy);


  return (sqrt(dx*dx + dy*dy));

}


//============================================================================
  double UT2D_len_ptln (Point2 *pt, Point2 *pa, Point2 *pe) {
//============================================================================
/// \code
/// UT2D_len_ptln      get (signed)  minDist from pt to Line pa-pe
/// \endcode
// zuerst pt auf Line projizieren;
// feststellen, ob der normalpunkt auf oder ausserhalb Line ist
// Auf Line: den Abstand pt-NormalPt liefern (mit Vorzeichen)
// nicht auf Line: den Abstand zum naeheren Endpunkt liefern


  int     irc;
  double  d1, dx, dy;


  // UT3D_stru_dump (Typ_PT2, pt, "UT2D_len_ptln  pt");
  // UT3D_stru_dump (Typ_PT2, pa, "  pa");
  // UT3D_stru_dump (Typ_PT2, pe, "  pe");


  irc = UT2D_3len_ptln (&d1, &dx, &dy, pt, pa, pe);

  return d1;

}


//============================================================================
  int UT2D_3len_ptln (double *d1, double *dx, double *dy,
                      Point2 *pt, Point2 *pa, Point2 *pe) {
//============================================================================
/// \code
/// ck minimalAbst von pt auf Line pa,pe
/// feststellen, ob der Normalpunkt auf oder ausserhalb Line ist
///   (Normalpunkt = Projektion von pt auf Line pa-pe)
/// Auf Line: den Abstand pt-NormalPt liefern (mit Vorzeichen)
/// nicht auf Line: den Abstand zum naeheren Endpunkt liefern
/// d1: Normalabstand od MindestAbstand
/// dx: LaengsAbstand (der Abstand des NormalPunktes von pa)
/// dy: der Normalabstand von pt zur Line pa-pe;
///     Vorzeichen positiv: pt links von pa; negativ: rechts von pa.
///
/// RetCod:
///  -1 = vor pa;         dy ist nicht gueltig !
///   0   zwischen pa-pe; dx,dy sind gueltig
///   1   ausserhalb pe;  dy ist nicht gueltig !
/// \endcode




  int     irc;
  double  lenl;
  Vector2 vl, vp;


  // UT3D_stru_dump (Typ_PT2, pt, "UT2D_3len_ptln  pt");
  // UT3D_stru_dump (Typ_PT2, pa, "  pa");
  // UT3D_stru_dump (Typ_PT2, pe, "  pe");



  UT2D_vc_2pt (&vl, pa, pe);

  // vp = pl -> pt
  UT2D_vc_2pt (&vp, pa, pt);


  // len of line
  lenl = UT2D_len_vc (&vl);
  // printf(" lenl=%f\n",lenl);

  if(lenl < UT_TOL_pt) {
    // printf("UT2D_len_ptln E001\n");
    *dy = UT2D_len_2pt (pa, pt);
    *dx = 0.;
    *d1 = *dx;
    irc = 0;
    goto L_fertig;
  }


  // d1 = Abst des pt in Richtg vl von pa weg (mit Vorzeichen).
  *dx = (vp.dx * vl.dx + vp.dy * vl.dy) / lenl;
    // printf("  DX=%f lenl=%f\n",*dx,lenl);


  // ck Normalpunkt total ausserhalb Line
  if(*dx < -UT_TOL_pt) goto L_out_L;
  if(*dx > lenl+UT_TOL_pt)  goto L_out_R;


  // der Normalpt ist innerhalb Line
  // den Normalabstand errechnen
  // printf(" pt ist innen ..\n");

  *dy = (vp.dy * vl.dx - vp.dx * vl.dy) / lenl;
    // printf("  DY=%f\n",*dy);
  *d1 = fabs(*dy);
  irc = 0;
  goto L_fertig;



  //==========================================================
  // der Normalpt ist ausserhalb Line links von pa
  L_out_L:
  // printf(" pt ist links ..\n");
  *d1 = UT2D_len_2pt (pa, pt);
  irc = -1;
  goto L_fertig;


  // der Normalpt ist ausserhalb Line rechts
  L_out_R:
  // printf(" pt ist rechts ..\n");
  *d1 = UT2D_len_2pt (pe, pt);
  irc = 1;
  // goto L_fertig;



  //==========================================================
  L_fertig:
    // printf("ex UT2D_3len_ptln %d %f %f %f\n",irc,*d1,*dx,*dy);
  return irc;

}


//====================================================================
  int UT2D_slenq_ptptvc (double *qlen,
                         Point2 *pa, Point2 *pb, Point2 *pc) {
//====================================================================
/// \code
/// UT2D_slenq_ptptvc         signed quadr.Distance pt-pt in Richtung vec
/// pa - pb gives a line, pc is projected onto this line, giving e.
/// Returns the quadr. length pa - e.
///
///             pc
///              |
///              |
///       qlen   |
///    pa--------+-----pb
///              e
///
/// RetCod erweitern;
///   -1   pc is left of pa       (pc - pa   -    pb)
///    0   pc is between pa -pb        (pa - pc - pb)
///    1   pc is right of pb           (pa   -    pb - pc)
///
/// see UT2D_sidPerp_3pt
/// \endcode


  int       irc;
  double    s_ab_ab, s_ab_ac, pe_ab;
  Vector2   vab, vac, vae;


  // UT3D_stru_dump (Typ_PT2, pa, "  pa");
  // UT3D_stru_dump (Typ_PT2, pb, "  pb");
  // UT3D_stru_dump (Typ_PT2, pc, "  pc");


  UT2D_vc_2pt (&vab, pa, pb);           // vab = Vector a-b
  UT2D_vc_2pt (&vac, pa, pc);           // vac = Vector a-c

  s_ab_ab = UT2D_skp_2vc (&vab, &vab);  // skp mit sich selbst = Laenge^2
  s_ab_ac = UT2D_skp_2vc (&vab, &vac);  // gibt Wert fuer e relativ zu s_ab_ab
    // printf("s_ab_ab=%f s_ab_ac=%f \n",s_ab_ab,s_ab_ac);


  // Parameterwert von e zwischen a-b
  pe_ab = s_ab_ac / s_ab_ab;
    // printf(" pe_ab=%f\n",pe_ab);

  // Multip. des Vektors a-b mit Parameterwert von e ergibt den Vektor a-e
  UT2D_vc_multvc (&vae, &vab, pe_ab);

  // Quadr. Abstand des Punktes e von a:
  // skp a-e mit sich selbst ergibt das Quadrat der Laenge;
  // wahre Laenge = sqrt(*qlen)
  irc = DSIGN(pe_ab);  // >=0 ergibt +1; else -1
  *qlen = UT2D_skp_2vc (&vae, &vae) * irc;

  if(irc > 0) {
    if(pe_ab <= 1.) irc = 0;
  }

    // printf("ex UT3D_slenq_ptptvc %d %f\n",irc,*qlen);
    // printf("   sqrt-qlen=%f\n",sqrt(*qlen));

  return irc;

}


//================================================================
  double UT2D_slen_vc_vc__ (Vector2 *vac, Vector2 *vab) {
//================================================================
/// \code
/// UT2D_slen_vc_vc__         signed length of vector on vector
/// both vector need not to be normalized.
/// If vab = normalized use UT2D_slen_vc_vcNo.
///
///             C 
///           . |                   A-C  = vector vac
///         .   |                   A-B  = vector vab
///        .    |
///      .      |
///    A-----_--+---------B
///    |--slen--|
///
///  Returns:
///    slen;   negative if C is left of A-B
/// \endcode

  Vector2 vnab;

  UT2D_vc_setLength (&vnab, vab, 1.);
    // printf(" vnab=%f,%f\n",vnab.dx,vnab.dy);

  return UT2D_slen_vc_vcNo (vac, &vnab);

}


//========================================================================
  double UT2D_slen_nor3pt (Point2 *p1, Point2 *p2, Point2 *p3) {
//========================================================================
/// \code
/// UT2D_slen_nor3pt          signed length of normal point - line
/// gibt den Abstand den p2 auf die Linie p1 - p3 hat mit Vorzeichen.
///            p2
///            |
///        slen|
///            |
///    p1------+--------p3
///
/// \endcode


  double  d1;
  Vector2 v1, v2;

  // UT3D_stru_dump (Typ_PT2, p1, "UT2D_slen_nor3pt");
  // UT3D_stru_dump (Typ_PT2, p2, "             p2 ");
  // UT3D_stru_dump (Typ_PT2, p3, "             p3 ");



  // v1 = p1 -> p3
  v1.dx = p3->x - p1->x;
  v1.dy = p3->y - p1->y;

  // v2 = p1 -> p2
  v2.dx = p2->x - p1->x;
  v2.dy = p2->y - p1->y;

  UT2D_slen_nor2vc (&d1, &v1, &v2);

  return d1;

}


//================================================================
  double UT2D_slen_nor_2pt_vcNo (Vector2 *v1, Point2 *p1, Point2 *p2) {
//================================================================
/// \code
/// UT2D_slen_nor_2pt_vcNo    signed length of normal of point onto norm.vector
/// For true length v1 must be normalized !
/// see UT2D_acos_2vc
///
///                    . p2
///                  .   .
///                .     .
///              .       .
///           v2         .   sk = normal-distance of p2 onto vector p1-v1
///          .           .
///        .             .
///      .               .
///    X-----------------.------->
///    p1            v1
///
/// \endcode


  double   sk;
  Vector2  v2;

  // UT3D_stru_dump (Typ_VC2, v1, "UT2D_slen_nor_2pt_vcNo\n");

  // Vec v2 = p1 -> p2 (UT2D_vc_2pt)
  UT2D_vc_2pt (&v2, p1, p2);
  // UT3D_stru_dump (Typ_VC2, &v2, "");

  sk = v1->dx * v2.dy - v1->dy * v2.dx;
  // printf(" sk=%f\n",sk);

  return sk;

}


//========================================================================
  int UT2D_slen_nor2vc (double *slen, Vector2 *v1, Vector2 *v2) {
//========================================================================
/// \code
/// UT2D_slen_nor2vc          signed length of normal of 2 vectors
/// both vectors need not to be normalized.
/// If v1 = normalized use UT2D_slen_nor_vc_vcNo.
///            x
///           /|
///          / |
///       V2/  |
///        /   |
///       /    |slen
///      /     |
///     x------+-------x
///              V1
///
/// RetCode:   0  OK
///           -1  length v1 = 0
/// \endcode


  double  d1;
  Vector2 nv2;

  // UT3D_stru_dump (Typ_PT2, p1, "UT2D_slen_nor2vc");
  // UT3D_stru_dump (Typ_PT2, p2, "             p2 ");
  // UT3D_stru_dump (Typ_PT2, p3, "             p3 ");

  // d1 = Laenge(v1)
  d1 = sqrt(v1->dx*v1->dx + v1->dy*v1->dy);
    // printf("UT2D_slen_nor2vc d1=%f\n",d1);


  // wenn p1 und p3 ganz dicht beisammenliegen ..
  if(d1 < UT_TOL_min0) {
    *slen = 0.;
    return -1;
  }

  // nv2 = Normalvektor (v2)
  nv2.dx = v2->dy;
  nv2.dy = -v2->dx;

  // abst = skalarprod (v1, nv2) / Laenge(v1)
  // d1 = fabs(v1.dx * nv2.dx + v1.dy * nv2.dy) / d1;
  *slen = (v1->dx * nv2.dx + v1->dy * nv2.dy) / d1;

    // printf("ex UT2D_slen_nor2vc %f\n",d1);

  return 0;

}

//========================================================================
  int UT2D_slen_nor_vc_vcNo (double *slen, Vector2 *v1, Vector2 *v2) {
//========================================================================
/// \code
/// UT2D_slen_nor_vc_vcNo     signed length of normal of 2 vectors (1 normalized)
/// v1 must be normalized     (else slen *= length_of_V1)
///            x
///           /|
///          / |
///       V2/  |
///        /   |
///       /    |slen
///      /     |
///     x------+-------x
///              V1
///
/// RetCode:   0  OK
///           -1  length v1 = 0
/// \endcode


  Vector2 nv2;

  // UT3D_stru_dump (Typ_PT2, p1, "UT2D_slen_nor2vc");
  // UT3D_stru_dump (Typ_PT2, p2, "             p2 ");
  // UT3D_stru_dump (Typ_PT2, p3, "             p3 ");

  // nv2 = Normalvektor (v2)
  nv2.dx = v2->dy;
  nv2.dy = -v2->dx;

  // dist = skalarprod (v1, nv2) / Laenge(v1)
  *slen = v1->dx * nv2.dx + v1->dy * nv2.dy;

    // printf("ex UT2D_slen_nor_vc_vcNo %f\n",d1);

  return 0;

}


//================================================================
  int UT2D_2slen_vc_vc__ (double *dx, double *dy,
                          Vector2 *vp, Vector2 *vl) {
//================================================================
/// \code
/// UT2D_2slen_vc_vc__     relative coords (dx,dy) of vector on vector
/// vl need not to be normalized.
///
///            x
///           /|
///          / |
///       vp/  |
///        /   |
///       /    |dy
///      /     |
///     x------+---vl---->
///     |  dx  |   
///
/// RetCode:   0  OK
///           -1  length vl = 0
/// \endcode

  double  d1;
  Vector2 nv2;

  // d1 = Laenge(v1)
  d1 = sqrt(vl->dx*vl->dx + vl->dy*vl->dy);
    // printf(" len-vl-d1=%f\n",d1);

  // wenn p1 und p3 ganz dicht beisammenliegen ..
  if(d1 < UT_TOL_min0) {
    *dx = 0.;
    *dy = 0.;
    return -1;
  }

  // length along pl-vl:
  *dx = (vp->dx * vl->dx + vp->dy * vl->dy) / d1;

  // length normal to pl-vl:
  // nv2 = Normalvektor (v2)
  nv2.dx = vp->dy;
  nv2.dy = -vp->dx;
    // printf(" nv2 %f %f\n",nv2.dx,nv2.dy);

  // abst = skalarprod (v1, nv2) / Laenge(v1)
  // d1 = fabs(v1.dx * nv2.dx + v1.dy * nv2.dy) / d1;
  *dy = (vl->dx * nv2.dx + vl->dy * nv2.dy) / d1;

  return 0;

}


//================================================================
  int UT2D_2slen_2pt_vc__ (double *dx, double *dy,
                           Point2 *px, Point2 *pl, Vector2 *vl) {
//================================================================
/// \code
/// UT2D_2slen_2pt_vc__     relative coords (dx,dy) of point along/normal to line
/// vl need not to be normalized.
///
///            px
///           /|
///          / |
///       vp/  |
///        /   |
///       /    |dy
///      /     |
///    pl------+---vl---->
///     |  dx  |   
///
/// RetCode:   0  OK
///           -1  length vl = 0
/// \endcode


  Vector2    vp;


  // vp = vector pl-px
  UT2D_vc_2pt (&vp, pl, px);

  // relative coords (dx,dy) of point along/normal to line
  return UT2D_2slen_vc_vc__ (dx, dy, &vp, vl);

}


//====================================================================
  int UT2D_nlenq_3pt (double *qlen,
                      Point2 *pa, Point2 *pb, Point2 *pc) {
//====================================================================
/// \code
/// UT2D_nlenq_3pt                 give quadr.Distance from point to line
/// pa - pb gives a line, pc is projected onto this line, giving e.
/// Returns the qadr.length pc - e.
///
///           pc
///            |
///         len|
///            |
///    pa------+-------pb
///            e
/// \endcode



  double    s_ab_ab, s_ab_ac, pe_ab;
  Vector2   vab, vac, vae, vec;


  UT2D_vc_2pt (&vab, pa, pb);           // vab = Vector a-b
  UT2D_vc_2pt (&vac, pa, pc);           // vac = Vector a-c

  s_ab_ab = UT2D_skp_2vc (&vab, &vab);  // skp mit sich selbst = Laenge^2
  s_ab_ac = UT2D_skp_2vc (&vab, &vac);  // gibt Wert fuer e relativ zu s_ab_ab

  // Parameterwert von e zwischen a-b
  pe_ab = s_ab_ac / s_ab_ab;

  // Multip. des Vektors a-b mit Parameterwert von e ergibt den Vektor a-e
  UT2D_vc_multvc (&vae, &vab, pe_ab);

  // Subtraktion (Vec-a-c - Vec-a-e) = Vec-e-c
  UT2D_vc_sub2vc (&vec, &vac, &vae);

  *qlen = UT2D_skp_2vc (&vec, &vec);

    // printf("ex UT2D_nlenq_3pt %f\n",*qlen);

  return 0;

}


//====================================================================
  int UT2D_nlenq_2vc (double *qlen, Vector2 *vab, Vector2 *vac) {
//====================================================================
/// \code
/// UT2D_nlenq_2vc            give quadr.Distance from vec on vec (point to line)
/// pa - pb gives a line, pc is projected onto this line, giving e.
/// Returns the qadr.length pc - e.
///
///           pc
///           /|
///      vac/  |
///       /    |qlen
///     /      |
///    pa------+-------pb
///           vab
/// -  0      0.5      1.0     1.5
/// \endcode



  double    s_ab_ab, s_ab_ac, pe_ab;
  Vector2   vae, vec;


  s_ab_ab = UT2D_skp_2vc (vab, vab);  // skp mit sich selbst = Laenge^2
  s_ab_ac = UT2D_skp_2vc (vab, vac);  // gibt Wert fuer e relativ zu s_ab_ab

  // Parameterwert von e zwischen a-b
  pe_ab = s_ab_ac / s_ab_ab;

  // Multip. des Vektors a-b mit Parameterwert von e ergibt den Vektor a-e
  UT2D_vc_multvc (&vae, vab, pe_ab);

  // Subtraktion (Vec-a-c - Vec-a-e) = Vec-e-c
  UT2D_vc_sub2vc (&vec, vac, &vae);

  *qlen = UT2D_skp_2vc (&vec, &vec);

    // printf("ex UT2D_nlenq_3pt %f\n",*qlen);

  return 0;

}


/* BUG: WORKS ONLY FOR p1-p2 X-parallel or Y-parallel ..
//=======================================================================
  int UT2D_parLn_pt2pt (double *d1, Point2 *p1, Point2 *p2, Point2 *px) {
//=======================================================================
/// get parameter (0-1) of point px along line p1-p2

  double   dx, dy;

  // test if dx > dy
  dx = p2->x - p1->x;
  dy = p2->y - p1->y;
    printf(" dx=%f dy=%f\n",dx,dy);

  // fix parameter d1
  if(fabs(dx) > fabs(dy))  {
    UTP_param_p0p1px (d1, p1->x, p2->x, px->x);
  } else {
    UTP_param_p0p1px (d1, p1->y, p2->y, px->y);
  }
    printf("ex UT2D_parLn_pt2pt %f\n",*d1);

  return 0;

}
*/

//====================================================================
  void UT2D_2len_ptvc (double *dx, double *dy,
                       Point2 *pt, Point2 *pb, Vector2 *vc) {
//====================================================================
/// \code
/// UT2D_2len_ptvc            dx,dy = dist pt - pb along / normal to vector vc
/// 
/// Input:
///   pb,vc  - define the axissystem (origin and x-axis)
///   pt     - point in this axissystem
/// 
/// Output:
///   dx     - gives the distance along vc from pb to pt (can be negativ !)
///   dy     - gives the distance normal to vc from pb to pt (can be negativ !)
/// \endcode
 

  Point2  ph;

  // project pt onto vc
  UT2D_pt_projptptvc (&ph, pt, pb, vc);

  // dx = length pb - ph
  *dx = UT2D_len_2pt (pb, &ph);

  if ((UT2D_sidPerp_ptvc (pt, pb, vc)) < 0) *dx = - *dx;

  // dy = length pt - ph
  *dy = UT2D_len_2pt (pt, &ph);

  if ((UT2D_sid_ptvc (pt, pb, vc)) < 0) *dy = - *dy;

  // printf("UT2D_2len_ptvc %f %f\n",*dx,*dy);

}


//=======================================================================
  int UT2D_angr_2vc_sr (double *ao, Vector2 *vc1, Vector2 *vc2, int sr) {
//=======================================================================
/// \code
/// UT2D_angr_2vc_sr        get opening angle of 2 vectors with sense of rot
/// Input:
///   sr      1=CCW; else CW
/// \endcode


  if(sr > 0) { // CCW
    *ao = UT2D_angr_2vc (vc1, vc2);

  } else {     // CW
    *ao = UT2D_angr_2vc (vc2, vc1) * -1.;
  }
    printf("ex UT2D_angr_2vc_sr %lf\n",*ao);

  return 0;

}


//======================================================================
  double UT2D_angr_ci (Point2 *pa,Point2 *pe,Point2 *pc,double rad_in) {
//======================================================================
/// \code
///
/// UT2D_angr_ci              openingAngle of circle (pa,pe,pc,rad/dreh)
/// 
/// Der Winkel ist bei CW-Kreisen immer negativ (-360 - 0 - 360).
/// 
/// 
/// Achtung: rad gibt auch die Drehrichtung !
///   CCW = rad > 0 ;
///   CW  = rad < 0 ;
/// \endcode

/*
  Point2 p1={5,0}, p2={4,3}, pc={0,0}; double rd=5; // CCW 0 / 36 (36)
  Point2 p1={5,-0.1}, p2={4,-3}, pc={0,0}; double rd=5; // CCW 1-324
  Point2 p1={5,0}, p2={0,5}, pc={0,0}; double rd=5; // CCW 0 / 90 (90)
  Point2 p1={-4,-3}, p2={4,-3}, pc={0,0}; double rd=5; // CCW 230 / 330 (106)
  Point2 p1={5,0}, p2={-4,3}, pc={0,0}; double rd=5; // CCW 0 / 143  (143)
  Point2 p1={5,0}, p2={-4,-3}, pc={0,0}; double rd=5; // CCW 0 / 216 (216)

  Point2 p1={5,0.1}, p2={4,3}, pc={0,0}; double rd=-5; // CW 0 / 36 (-323)
  Point2 p1={5,0}, p2={0,5}, pc={0,0}; double rd=-5; // CW 0 / 90 (-270)
  Point2 p1={5,0}, p2={-4,3}, pc={0,0}; double rd=-5; // CW 0 / 143 (-216)
  Point2 p1={5,0}, p2={-4,-3}, pc={0,0}; double rd=-5; // CW 0 / 216 (-143)
  Point2 p1={-4,-3}, p2={4,-3}, pc={0,0}; double rd=-5; // CW 230 / 330 (-253)
  Point2 p1={-3,4}, p2={3,4}, pc={0,0}; double rd=-5;   //  CW 230 / 60

  Point2 p1={5,0}, p2={-5,0}, pc={0,0}; double rd=-5; //  180 / -180
  Point2 p1={5,0}, p2={5,0}, pc={0,0}; double rd=5; // 360 / -360

  ao = UT2D_angr_ci (&p1,&p2,&pc, rd);


*/

  double   ao;
  Vector2  vc1, vc2;


  // printf("UT2D_angr_ci pa=%f,%f pe=%f,%f\n",pa->x,pa->y,pe->x,pe->y);
  // printf("             pc=%f,%f r=%f\n",pc->x,pc->y,rad_in);



  if((UTP_compdb0(pa->x-pe->x,UT_TOL_pt))    &&
     (UTP_compdb0(pa->y-pe->y,UT_TOL_pt))) {
    // printf("Vollkreis\n");
    if(rad_in > 0.) ao = RAD_360;
    else ao = -RAD_360;
    goto Fertig;
  }



  UT2D_vc_2pt (&vc1, pc, pa);
  UT2D_vc_2pt (&vc2, pc, pe);


  /* TX_Print("     vc1=%f,%f",vc1.dx,vc1.dy); */
  /* TX_Print("     vc2=%f,%f",vc2.dx,vc2.dy); */



  /*  ao = Oeffnungswinkel - see UT2D_angr_2vc_sr */
  if(rad_in > 0.) {
    ao  = UT2D_angr_2vc (&vc1, &vc2);
  } else  {
    ao  = UT2D_angr_2vc (&vc2, &vc1);
    ao = -ao;
  }
    // printf("  ao=%f %f\n",ao,UT_DEGREES(ao));


  Fertig:

    // printf("ex UT2D_angr_ci %f / %f\n",ao,UT_DEGREES(ao));

  return ao;

}

//================================================================
  int UT2D_circQuad23_angr (double *ar) {
//================================================================
/// \code
/// UT2D_circQuad23_angr   returns if angle is quadrant 2 or 3 (CCW from 1-4)
/// RetCod:  0 = angle is in quadrant 1 or 4
///          1 = angle is in quadrant 2 or 3  (>= 90 and <= 270 deg)
/// \endcode


  double aa;

  // make ar 0<RAD_360
  aa = *ar;
  UT2D_angr_set (&aa);


  if((aa < RAD_90)||(aa > RAD_270)) return 0;
  else return 1;

}



//================================================================
  int UT2D_circQuad34_angr (double *ar) {
//================================================================
/// \code
/// UT2D_circQuad34_angr   returns if angle is quadrant 3 or 4 (CCW from 1-4)
/// RetCod:  0 = angle is in quadrant 1 or 2
///          1 = angle is in quadrant 3 or 4  (>= 180 and <= 360 deg)
/// \endcode


  double aa;

  // make ar 0<RAD_360
  aa = *ar;
  UT2D_angr_set (&aa);


  if(aa < RAD_180) return 0;
  return 1;

}


//================================================================
  double UT2D_angr_perpangr (double *ai) {
//================================================================
/// UT2D_angr_perpangr        angle + 90 degree (perp. to angle)

  double ao;

  ao = *ai + RAD_90;    // ao = ai + 90 degree

  UT2D_angr_set (&ao);  // change to val >= 0 <= RAD_360

  // printf("ex UT2D_angr_perpangr %f %f\n",ao,*ai);

  return ao;

}


//====================================================================
  double UT2D_len_cir (double *angOpe,
                       Point2 *pa,Point2 *pe,Point2 *pc,double rad_in) {
//====================================================================
/// \code
/// UT2D_len_cir              length circular arc (pa,pe,pc,rad/dreh)
/// 
/// Laenge immer positiv (angOpe nicht !)
/// 
/// Liefert auch den Oeffnungswinkel angOpe.
/// 
/// Achtung: rad gibt auch die Drehrichtung !
///   CCW = rad > 0 ;
///   CW  = rad < 0 ;
/// \endcode


  double   ao, dl;


  /* TX_Print("UT2D_len_cir pa=%f,%f pe=%f,%f",pa->x,pa->y,pe->x,pe->y); */
  /* TX_Print("             pc=%f,%f r=%f",pc->x,pc->y,rad_in); */

  // Oeffnungswinkel holen
  ao = UT2D_angr_ci (pa, pe, pc, rad_in);



  dl = rad_in * ao;


  // printf("ex UT2D_len_cir %f ao=%f / %f\n",dl,ao,UT_DEGREES(ao));

  *angOpe = ao;

  return dl;

}





/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/


//=======================================================================
  double UT2D_slen_pt_pt_vc__ (Point2 *pc,  Point2 *pa, Vector2 *vab) {
//=======================================================================
/// \code
///
/// UT2D_slen_pt_pt_vc__      signed length of point onto vector
/// both vector need not to be normalized.
///
///             pc
///           . |                   
///         .   |                   
///        .    |
///      .      |
///   pa--------+-----vab-->
///    |--slen--|
/// 
///  Returns:
///    slen;   negative if C is left of A-B
/// 
/// \endcode


  Vector2  vac;

  UT2D_vc_2pt (&vac, pa, pc);

  return UT2D_slen_vc_vc__ (&vac, vab);

}


//=======================================================================
  double UT2D_slen_nor_2pt_vc__ (Point2 *pt,  Point2 *pl, Vector2 *vl) {
//=======================================================================
/// \code
///
/// UT2D_slen_norvc2pt           signed length of normal point onto vector
/// vl must not be normalized.
/// if vl = normalized: use UT2D_slen_nor_2pt_vcNo
///
///                    . pt 
///                  .   .
///                .     .
///              .       .
///           vp         .   sk = normal-distance of pt onto vector pl-vl
///          .           . 
///        .             .
///      .               .
///    X-----------------.------->
///    pl            vl
/// 
/// \endcode


  Vector2  vp, vb;

  UT2D_vc_setLength (&vb, vl, 1.);

  UT2D_vc_2pt (&vp, pl, pt);

  return (vp.dx * vb.dy - vb.dx * vp.dy);

}


//================================================================
  int UT2D_sid_3pt (Point2 *pt,  Point2 *p1, Point2 *p2) {
//================================================================
/// \code
/// UT2D_sid_3pt               compare if pt is on, above or below line (p1-p2)
///
///                   X
///          vp  .     pt    1
///        . 
///   p1------------vl--------p2->
///            
///                   X
///                   pt   -1
/// 
/// RetCode:
///   0   pt is on vector vl
///   1   pt is above vector vl (left side)
///  -1   pt is below vector vl (right side)
///
/// side before,behind: UT2D_sidPerp_3pt
/// \endcode

  int      irc;
  double   d1;
  Vector2  vl, vp;


  UT2D_vc_2pt (&vl, p1, p2);

  UT2D_vc_2pt (&vp, p1, pt);

  d1 = vp.dx * vl.dy - vl.dx * vp.dy;

  if (fabs(d1) < UT_TOL_min2) irc =  0;

  else if (d1 < 0.)    irc =  1;

  else                 irc = -1;


  // printf("UT2D_sid_ptvc %d %f\n",irc,d1);
  return irc;


}


//======================================================================
  int UT2D_sid_ptvc (Point2 *pt,  Point2 *pl, Vector2 *vl) {
//======================================================================
/// \code
/// UT2D_sid_ptvc             compare if pt is on, above or below line (pl+vl)
///
/// was UT2D_dir_ptvc
///
///                       X
///                       pt      1
///           pl
/// ----------X---vl----------->  0
///            
///                       X
///                       pt     -1
/// 
/// retcode:
///   0   pt is on vector vl
///   1   pt is above vector vl (left side)
///  -1   pt is below vector vl (right side)
/// \endcode


  int      rc;
  double   d1;
  Vector2  vp;


  // UT3D_stru_dump (Typ_PT2, pt, "UT2D_sid_ptvc pt");
  // UT3D_stru_dump (Typ_PT2, pl, "  pl");
  // UT3D_stru_dump (Typ_VC2, vl, "  vl");


  UT2D_vc_2pt (&vp, pl, pt);
    // UT3D_stru_dump (Typ_VC2, &vp, "  vp");

  d1 = vp.dx * vl->dy - vl->dx * vp.dy;
    // printf(" d1=%f\n",d1);


  if (fabs(d1) < UT_TOL_min1) rc =  0;
  else if (d1 < 0.)           rc =  1;
  else                        rc = -1;

    // printf("UT2D_sid_ptvc rc=%d d1=%f\n",rc,d1);

  return rc;

}


//======================================================================
  int UT2D_sidPerp_ptvc (Point2 *pt,  Point2 *pl, Vector2 *vl) {
//======================================================================
/// \code
/// UT2D_sidPerp_ptvc        compare if pt is right/on/left of a normal to pl+vc
///
///              
///   pt      |           pt
///   -1      |           1
///           |  
///           |--------->vl
/// ----------X----------------
///           pl
///           |
///
/// retcode:
///   0   pt is on a normal to line pl-vl
///   1   pt is before pl (to the right side of a normal to line pl-vl)
///  -1   pt is behind pl (to the left side of a normal to line pl-vl)
/// \endcode


  int      rc;
  double   d1;
  Vector2  vp;

  /* ------------------------------------------------------------------- */
  // UT2D_vc_invert (&vn, vl);

  UT2D_vc_2pt (&vp, pl, pt);

  d1 = vp.dx * vl->dx + vp.dy * vl->dy;

  // if (UTP_comp_0 (d1)) rc =  0;
  // else if (d1 < 0.)    rc = -1;
  // else                 rc =  1;
  rc = DSIGTOL (d1, UT_TOL_min0);         // UT_TOL_min1


  // printf("UT2D_sidPerp_ptvc %d %f\n",rc,d1);

  return rc;

}






/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/


/*
//================================================================
  int UT2D_comp2pt (Point2 *p1, Point2 *p2, double tol) {
//================================================================
 
// UT2D_comp2pt              compare 2 2D-points

// Retcode 0 = Abstand der Punkte > tol
        // 1 = Abstand der Punkte < tol

// if(UT2D_comp2pt(&p1,&p2,tol)) printf("p1 und p2 sind gleich !\n");


  if(fabs(p1->x - p2->x) > tol) return 0;
  if(fabs(p1->y - p2->y) > tol) return 0;

  return 1;

 
  // return ((UTP_comp2db (p1->x, p2->x, tol)) &&
          // (UTP_comp2db (p1->y, p2->y, tol)));

}
*/


//=============================================================================
  int UT2D_comp4pt (Point2 *p1a,Point2 *p1e,Point2 *p2a,Point2 *p2e,double tol){
//=============================================================================
/// \code
/// UT2D_comp4pt              compare 4 2D-points
/// Ob 2 der 4 Punkte zusammenpassen
///   (zum Test ob 2 Lines einen gemeinsamen Punkt haben).
/// Retcode 0 = Abstand aller Punkte > tol
///         1 = Abstand (p1e - p2a) < tol
///         2 = Abstand (p1a - p2a) < tol  (Obj 1 verkehrt)
///         3 = Abstand (p1e - p2e) < tol  (Obj 2 verkehrt)
///         4 = Abstand (p1a - p2e) < tol  (Obj 1 und 2 verkehrt)
/// \endcode

  if(UT2D_comp2pt(p1e, p2a, tol) == 1) return 1;
  if(UT2D_comp2pt(p1a, p2a, tol) == 1) return 2;
  if(UT2D_comp2pt(p1e, p2e, tol) == 1) return 3;
  if(UT2D_comp2pt(p1a, p2e, tol) == 1) return 4;


  return 0;

}




/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/



/*
//================================================================
  Point2 UT2D_pt_pt3 (Point *pt) {
//================================================================
// UT2D_pt_pt3               2D-Point = 3D-Point

  Point2  pt20;

  pt20.x = pt->x;
  pt20.y = pt->y;

  return pt20;

}

//====================================================================
  void UT2D_pt_2db (Point2 *pt, double x, double y) {
//====================================================================
// UT2D_pt_2db              2D-Point < 2 doubles (x, y)


  pt->x = x; pt->y = y;

  return;

}
*/


/* inline ..
  void UT2D_pt_opp2pt (Point2 *po, Point2 *p1, Point2 *p2) {
UT2D_pt_opp2pt            opposite point (p1 = center)

  po->x = p1->x - (p2->x - p1->x);
  po->y = p1->y - (p2->y - p1->y);

  // printf("UT2D_opp2pt %f %f\n",po->x,po->y);
  // printf("            %f %f\n",p1->x,p1->y);


  return;
}
*/



/* inline ..
  void UT2D_pt_mid2pt (Point2 *po, Point2 *p1, Point2 *p2) {
UT2D_pt_mid2pt                  midpoint between 2 points


  po->x = (p1->x + p2->x) / 2.0;
  po->y = (p1->y + p2->y) / 2.0;

  // printf("UT2D_midPoint %f %f\n",po->x,po->y);


  return;
}
*/


/*
//================================================================
  void UT2D_pt_traptvc (Point2 *po, Point2 *pi, Vector2 *vc) {
//================================================================
// UT2D_pt_traptvc          2D-Point = 2D-Point + 2D-Vector


  po->x = pi->x + vc->dx;
  po->y = pi->y + vc->dy;

  // printf("UT2D_pt_vc %f %f\n",po->x,po->y);
  // printf("           %f %f\n",pi->x,pi->y);
  // printf("           %f %f\n",vc->dx,vc->dy);

  return;
}
*/


//========================================================================
  void UT2D_pt_tra2ptlen (Point2 *po, Point2 *p1, Point2 *p2, double lenv) {
//========================================================================
/// UT2D_pt_tra2ptlen         transl. point p1 into dir p1 -> p2 dist. lenv


  double    l1, lvc;
  Vector2   vc;


  vc.dx = p2->x - p1->x;
  vc.dy = p2->y - p1->y;

  lvc= UT2D_len_vc (&vc);
  if(lvc < UT_TOL_min2) {
    *po = *p1;
    return;
  }
  l1 = lenv / lvc;

  po->x = p1->x + vc.dx * l1;
  po->y = p1->y + vc.dy * l1;

  /* printf("UT2D_pt_traptvclen %f %f\n",po->x,po->y); */
  /* printf("          %f %f\n",p1->x,p1->y); */
  /* printf("          %f %f\n",p2->x,p2->y); */
  /* printf("      len %f\n",lenv); */

}


//=========================================================================
  void UT2D_pt_traPtPtPar (Point2 *po, Point2 *p1, Point2 *p2, double d1) {
//=========================================================================
/// \code
/// UT2D_pt_traPtPtPar      transl. pt with parameter along p1-p2
///
/// see UT3D_pt_evpar2pt UT3D_vc_multvc UT2D_pt_traptvc
/// \endcode

  // printf("UT2D_pt_traPtPtPar %f\n",d1);
  // UT3D_stru_dump (Typ_PT2, p1, " p1");
  // UT3D_stru_dump (Typ_PT2, p2, " p2");

  po->x = p1->x + (d1 * (p2->x - p1->x));
  po->y = p1->y + (d1 * (p2->y - p1->y));

    // UT3D_stru_dump (Typ_PT2, po, "ex UT2D_pt_traPtPtPar ");

  return;

}


//==========================================================================
  void UT2D_pt_tra3ptlen(Point2 *po,
                         Point2 *p1, Point2 *p2, Point2 *p3, double lenv) {
//==========================================================================
/// UT2D_pt_tra3ptlen         transl. point p1 into dir p2 -> p3 dist. lenv

  double    l1, lvc;
  Vector2   vc;


  vc.dx = p3->x - p2->x;
  vc.dy = p3->y - p2->y;

  lvc= UT2D_len_vc (&vc);
  if(lvc < UT_TOL_min2) {
    *po = *p1;
    return;
  }
  l1 = lenv / lvc;

  po->x = p1->x + vc.dx * l1;
  po->y = p1->y + vc.dy * l1;

  /* printf("UT2D_pt_traptvclen %f %f\n",po->x,po->y); */
  /* printf("          %f %f\n",p1->x,p1->y); */
  /* printf("          %f %f\n",p2->x,p2->y); */
  /* printf("      len %f\n",lenv); */

}



//==========================================================================
  void UT2D_pt_tra2pt2len(Point2 *po,Point2 *p1,Point2 *p2,double dx,double dy){
//==========================================================================
/// UT2D_pt_tra2pt2len     transl. point into dir p1 -> p2 dist. dx and normal dy


  double    lvc, l1;
  Vector2   vc;


  vc.dx = p2->x - p1->x;
  vc.dy = p2->y - p1->y;

  lvc = UT2D_len_vc (&vc);
  if(lvc < UT_TOL_min2) {
    *po = *p1;
    return;
  }

  l1 = dx / lvc;

  po->x = p1->x + vc.dx * l1;
  po->y = p1->y + vc.dy * l1;

  l1 = dy / lvc;
  po->x = po->x - vc.dy * l1;
  po->y = po->y + vc.dx * l1;


  /* printf("UT2D_pt_tra2pt2len %f %f\n",po->x,po->y); */
  /* printf("          %f %f\n",p1->x,p1->y); */
  /* printf("          %f %f\n",p2->x,p2->y); */
  /* printf("      dx, dy %f %f\n",dx,dy); */

}




//=========================================================================
  void UT2D_pt_tra2pt3len (Point2 *po, Point2 *p1, Point2 *p2,
                           double dx, double dy, double lenv) {
//=========================================================================
/// UT2D_pt_tra2pt3len  move p1; Baseline p1-p2; Dir dx/dy; length lenv.


  Point2    pt1;

  /*  einen Punkt in diese Richtung */
  UT2D_pt_tra2pt2len (&pt1, p1, p2, dx, dy);

  /*  nun in diese Richtung lenv abschlagen */
  UT2D_pt_tra2ptlen (po, p1, &pt1, lenv);


  /* printf("UT2D_pt_tra2pt3len %f %f\n",po->x,po->y); */
  /* printf("                   %f %f\n",p1->x,p1->y); */
  /* printf("                   %f %f\n",p2->x,p2->y); */
  /* printf("       dx, dy, len %f %f\n",dx,dy,lenv); */

}


//========================================================================
  void UT2D_pt_traptvclen (Point2 *po,Point2 *pi,Vector2 *vc,double lenv) {
//========================================================================
/// UT2D_pt_traptvclen    move pi along vc length lenv

  double    l1, lvc;


  lvc = UT2D_len_vc (vc);
  if(lvc < UT_TOL_min2) {
    *po = *pi;
    return;
  }

  l1 = lenv / lvc;

  po->x = pi->x + vc->dx * l1;
  po->y = pi->y + vc->dy * l1;

  /* printf("UT2D_pt_traptvclen %f %f\n",po->x,po->y); */
  /* printf("          pt in %f %f\n",pi->x,pi->y); */
  /* printf("          vc %f %f  - len %f\n",vc->dx,vc->dy,lenv); */

}


//========================================================================
  void UT2D_pt_traptangrlen (Point2 *po,Point2 *pi,double ar,double lenv) {
//========================================================================
/// UT2D_pt_traptangrlen      move point; direction from angle, distance=lenv


  po->x = pi->x + (lenv * cos(ar));
  po->y = pi->y + (lenv * sin(ar));

/*
  printf("UT2D_pt_traptangrlen %f %f\n",po->x,po->y);
  printf("               pt in %f %f\n",pi->x,pi->y);
  printf("                     %f %f\n",ar,lenv);
*/

}


//===========================================================================
  void UT2D_pt_tranor2ptlen (Point2 *po, Point2 *p1, Point2 *p2, double lenv) {
//===========================================================================
/// \code
/// UT2D_pt_tranor2ptlen      transl. point normal to p1 - p2 dist. lenv
///   Center is p1; p1-p2 gives the baseline
/// \endcode


  Vector2   vc1;

  /*  vc1 = ein Vektor normal auf p1 -> p2  */
  vc1.dx = -(p2->y - p1->y);
  vc1.dy = p2->x - p1->x;

  UT2D_pt_traptvclen (po, p1, &vc1, lenv);

  /* printf("UT2D_pt_tranor2ptlen %f %f\n",po->x,po->y); */
  /* printf("           %f %f\n",p1->x,p1->y); */
  /* printf("           %f %f\n",p2->x,p2->y); */
  /* printf("           %f\n",lenv); */

}



//===========================================================================
  void UT2D_pt_tranorptvclen (Point2 *po, Point2 *p1, Vector2 *v1, double lenv) {
//===========================================================================
/// \code
/// UT2D_pt_tranorptvclen     transl. point normal to vec dist. lenv
///   Center is p1; p1-v1 gives the baseline
/// \endcode


  Vector2   vc1;

  /*  vc1 = ein Vektor normal auf v1  */
  vc1.dx = -v1->dy;
  vc1.dy = v1->dx;

  UT2D_pt_traptvclen (po, p1, &vc1, lenv);

  /* printf("UT2D_pt_tranor2ptlen %f %f\n",po->x,po->y); */
  /* printf("           %f %f\n",p1->x,p1->y); */
  /* printf("           %f %f\n",p2->x,p2->y); */
  /* printf("           %f\n",lenv); */

}



//========================================================================
  void UT2D_pt_traptvc2len (Point2 *po, Point2 *pi,
                            Vector2 *vc, double dx, double dy) {
//========================================================================
/// \code
/// UT2D_pt_traptvc2len       transl. point into dir vc; dist. dx and normal dy
/// Move point into direction vc with distance dx and normal to vc -
/// with distance dy.
/// \endcode


  double    lvc, l1;

  lvc = UT2D_len_vc (vc);

  l1 = dx / lvc;
  po->x = pi->x + vc->dx * l1;
  po->y = pi->y + vc->dy * l1;

  l1 = dy / lvc;
  po->x = po->x - vc->dy * l1;
  po->y = po->y + vc->dx * l1;

  /* printf("UT2D_pt_traptvc2len %f %f\n",po->x,po->y); */
  /* printf("          pt in %f %f\n",pi->x,pi->y); */
  /* printf("          vc %f %f  - len %f %f\n",vc->dx,vc->dy,dx,dy); */

}



//======================================================================
  void UT2D_pt_tracirlen(Point2 *pe, Point2 *pa, Point2 *pc,
                         double rad, double clen) {
//======================================================================
/// \code
/// UT2D_pt_tracirlen         transl. point circular length
///
/// rad  = Radius; Pos.Val = CCW; neg.Val =CW.
/// clen = arcLenght
/// \endcode



  double   ao;


  /*  Oeffnungswinkel */
  ao = clen/rad;

  UT2D_pt_rotptangr (pe, pc, pa, ao);

  /* TX_Print(" ex UT2D_pt_tracirlen %f,%f oe=%f",pe->x,pe->y,ao); */

  return;

}


//======================================================================
  void UT2D_pt_rotptangr (Point2 *p2, Point2 *pc, Point2 *p1, double ar) {
//======================================================================
/// \code
/// UT2D_pt_rotptangr         rotate a point around centerpoint
/// 
/// p2 darf ident mit p1 sein.
/// \endcode

// Do not use for multiple points - use UT2D_m2_init_rot + UT2D_pt_traptm2


  double cos_a, sin_a, dx, dy;

  /* ---------------------------------------------------------------- */

  dx = p1->x - pc->x;
  dy = p1->y - pc->y;

  sin_a = sin(ar);
  cos_a = cos(ar);

  p2->x = pc->x + (dx * cos_a - dy * sin_a);
  p2->y = pc->y + (dx * sin_a + dy * cos_a);

  /* printf("UT2D_pt_rotptangr %f,%f\n",p2->x,p2->y); */

  return;

}


//=========================================================================
  int UT2D_par_2vc (double *pl, Vector2 *v1, Vector2 *v2) {
//=========================================================================
/// \code
/// UT2D_parvc_2vc    parameter of distance of vec1 projected on vec2
///
///                             V1,V2 have same startpoint s.
///                 x
///                /|
///               / |
///             V1  |
///             /   |
///            /   .|
///           s-----x---V2------->
///     < 0   0     pl=0.4       1         > 1
///
/// RetCodes:
///    0  OK
///   -1  v1-v2 aufeinander Normal; pl = 0.
/// \endcode


  double  Det, dd;


  Det = v2->dx * v2->dx + v2->dy * v2->dy;

  if(Det == 0.0) {
    // printf("UT3D_parvc_2vc - No solution!\n");
    *pl = 0.;
    return -1;
  }

  // Skalarprodukt
  dd  = v1->dx * v2->dx + v1->dy * v2->dy;

  *pl = dd/Det;

  // printf("ex UT3D_parvc_2vc pl=%f Det=%f dd=%f\n",*pl,Det,dd);

  return 0;

}


//================================================================
  int UT2D_par_3pt (double *pl, Point2 *p1, Point2 *p2, Point2 *p3) {
//================================================================
/// \code
/// UT2D_par_3pt    parameter of point projected on line from 2 points
///
///                             V1,V2 have same startpoint s.
///                 p3
///                /|
///               / |
///             V1  |
///             /   |
///            /   .|
///          p1-----x---V1-------p2
///     < 0   0     pl=0.4       1         > 1
///
/// RetCodes:
///    0  OK
///   -1  v1-v2 aufeinander Normal; pl = 0.
///
/// see also UT3D_parpt_3pt
/// \endcode



  Vector2    v1, v2;

  UT2D_vc_2pt (&v1, p1, p3);
  UT2D_vc_2pt (&v2, p1, p2);

  return UT2D_par_2vc (pl, &v1, &v2);


}


//=============================================================================
  int UT2D_pt_projpt2pt(Point2 *pp,  double *len,
                        Point2 *pt, Point2 *p1, Point2 *p2) {
//=============================================================================
/// \code
/// UT2D_pt_projpt2pt                pp = project point pt to lineSegment p1-p2
///
/// p1-p2 is a line, pt is projected onto this line.
/// len ist the len pt-pp  (if not NULL)
///
///  -1   1       0         2   -2         retcod
///       |       pt        |
///               |
///            len|
///               |
///       p1------+--------p2
///               pp
///
/// Input:
///   len    NULL or address
/// Output:
///   pp     pr projected to p1-p2
///   len    distance pt-pp if not NULL on input
///   retcod 0=OK, pp is on line, between p1-p2
///          1 OK; pp = p1
///          2 OK; pp = p2
///         -1 outside; pt is before line
///         -2 outside; pt if after line
///         -3 Input-Error; p1==p2
///
/// see also UT3D_pt_ck_ptvc UT3D_pt_ck_onLine UT3D_nlen_2ptvc UT3D_pt_projptptvc
///   UT3D_pt_projptln
/// \endcode



   int     irc;
   double  sprod,lenl;
   Point2  pe;
   Vector2 vl, vp;



  // UT3D_stru_dump (Typ_PT2, pt, "UT2D_pt_projpt2pt");
  // UT3D_stru_dump (Typ_PT2, p1, "    p1:");
  // UT3D_stru_dump (Typ_PT2, p2, "    p2:");
  // printf("   UT_TOL_min1=%f\n",UT_TOL_min1);
  // printf("   UT_TOL_pt=%f\n",UT_TOL_pt);


  UT2D_vc_2pt (&vl, p1, p2);

  // vp = linstartpoint > ext.point
  UT2D_vc_2pt (&vp, p1, pt);
    // UT3D_stru_dump (Typ_VC2, &vp, "    vp:");

  // length of Line
  lenl = UT2D_len_vc (&vl);
    // printf(" lenl=%f\n",lenl);

  // wenn lenl==0 dann pp=pt
  if(lenl < UT_TOL_min1) {
    *pp = *p1;
    // if(len) *len = 0.;
    irc = -3;
    goto L_exit;
  }

  // Project vp on vl.
  sprod = (vl.dx*vp.dx + vl.dy*vp.dy) / lenl;
  sprod /= lenl;
    // printf(" lenl=%f sprod=%f\n",lenl,sprod);


  // compute prjPt on p1+vector
  pp->x = p1->x + (vl.dx * sprod);
  pp->y = p1->y + (vl.dy * sprod);
    // UT3D_stru_dump (Typ_PT2, pp, "    pp:");


  // signed Normalabstand
  if(len) {
    *len = (vl.dx * vp.dy - vl.dy * vp.dx) / lenl;
    // printf(" len(perp)=%f\n",*len);
  }


  // fix retcode ..
  irc = 0;
  if(UT2D_comp2pt(p1,pp,UT_TOL_pt) == 1) {
    irc = 1;
    *pp = *p1;
    goto L_exit;
  }


  if(UT2D_comp2pt(p2,pp,UT_TOL_pt) == 1) {
    irc = 2;
    *pp = *p2;
    goto L_exit;
  }

  if(sprod < 0.) irc = -1;
  if(sprod > 1.) irc = -2;


  L_exit:

    // printf("ex UT2D_pt_projpt2pt %d %f %f\n",irc,pp->x,pp->y);
    // if(len) printf(" len(perp)=%f\n",*len);


  return irc;
}


//================================================================
  int UT2D_pt_projptptvc (Point2 *pp, Point2 *pt, Point2 *pl, Vector2 *vl) {
//================================================================
/// \code
/// UT2D_pt_projptptvc        point = project point to line (pt+vc)
/// Project a point onto a line given by point and vector.
/// pl - vl gives a line, pt is projected onto this line.
///
/// IN:
///   Point2 *pt   ... point to project on line pl-vl
///   Point2 *pl   ... point on line
///   Vector2 *vl  ... vector of line
/// OUT:
///   Point2 *pp   ... normal projection of pt onto line
/// Returncodes:
///   0 = OK
///   1 = point is on line
/// \endcode


   double  sprod,lenl;
   Vector2 vp;


  /* change linstartpoint > ext.point into vector vp */
  UT2D_vc_2pt (&vp, pl, pt);

  /* length of Line */
  lenl = UT2D_len_vc (vl);

  /* wenn lenl==0 dann pp=pt */
  if(lenl < UT_TOL_min2) {
    *pp = *pt;
    return 1;
  }

  /* Project vp on vl. */
  sprod = (vl->dx*vp.dx + vl->dy*vp.dy) / lenl;

  /* printf(" %f %f\n",lenl,sprod); */

  /* compute prjPt on vector */
  sprod /= lenl;
  vp.dx = vl->dx * sprod;
  vp.dy = vl->dy * sprod;

  UT2D_pt_traptvc (pp, pl, &vp);


  /* printf("UT2D_pt_projptptvc %f %f\n",pp->x,pp->y); */
  /* printf("      pt %f %f\n",pt->x,pt->y); */
  /* printf("      pl %f %f\n",pl->x,pl->y); */
  /* printf("      vl %f %f\n",vl->dx,vl->dy); */

  return UT2D_comp2pt(pt, pp, UT_TOL_pt);
  // return UT2D_comp2pt(pt, pp, UT_TOL_min1);

}


//======================================================================
  int UT2D_pt_projptln (Point2 *pp, Point2 *pt, Line2 *ln) {
//======================================================================
/// UT2D_pt_projptln                        point = project point to line


   Vector2 vl;


  /* change line into vector vl */
  UT2D_vc_ln (&vl, ln);

  UT2D_pt_projptptvc (pp, pt, &ln->p1, &vl);

  /* printf("UT2D_pt_projptln %f %f\n",pp->x,pp->y); */

  return(0);
}


//===========================================================================
  int UT2D_pt_intptvcy (Point2 *pto, Point2 *ptl, Vector2 *vcl, double yVal) {
//===========================================================================
/// \code
/// UT2D_pt_intptvcy            intersection line (pt-vc) - horizontal line
/// Schnittpunkt eines Linesegmentes ln1 mit einer unendlich langen,
/// horiz. Geraden mit Y=yVal errechnen.
/// Retcod= 0: Schnittpunkt in pto
/// Retcod= 1: Linie vertical; beliebig viele Schnittpunkte
/// \endcode
// Achtung: wenn der Schnittpunkt sich ausserhalb des Geradensegmentes
//   befindet, wird kein Schnittpunkt geliefert!


  double    k1;

  k1 = vcl->dy / vcl->dx;

  // Linie horizontal; nur Schnittpunkte; irc = 1, Schnittpkt = Mittelpkt
  if(fabs(k1) < UT_TOL_min1) {
    *pto = *ptl;
    return 1;
  }

  pto->x = ptl->x + ((yVal - ptl->y) / k1);
  pto->y = yVal;


  // UT3D_stru_dump (Typ_PT2, pto, "ex UT2D_pt_intptvcy:  ");
  // GR_Disp_pt2 (pto, SYM_STAR_S, 0);

  return 0;

}


//=======================================================================
  int UT2D_pt_intlny (Point2 *pto, Point2 *lp1, Point2 *lp2, double yVal) {
//=======================================================================
/// \code
/// UT2D_pt_intlny            intersection linesegment - horizontal line
/// Schnittpunkt eines Linesegmentes ln1 mit einer unendlich langen,
/// horizontalen Geraden mit Y=yVal errechnen.
/// Retcod= 0: Schnittpunkt in pto
/// Retcod=-1: kein Schnittpunkt
/// Retcod= 1: Linie horizontal; beliebig viele Schnittpunkte
/// \endcode
// Achtung: wenn der Schnittpunkt sich ausserhalb des Geradensegmentes
//   befindet, wird kein Schnittpunkt geliefert!
// ACHTUNG: fuer UT_hatch_2D sind SEHR geringe Tol. erforderlich !


  // Lineseg oberhalb
  // if(((lp1->y-UT_TOL_min1) > yVal)&&((lp2->y-UT_TOL_min1) > yVal)) return -1;
  if((lp1->y > yVal)&&(lp2->y > yVal)) return -1;


  // Lineseg unterhalb
  // if(((lp1->y+UT_TOL_min1) < yVal)&&((lp2->y+UT_TOL_min1) < yVal)) return -1;
  if((lp1->y < yVal)&&(lp2->y < yVal)) return -1;


  // Linie horizontal; nur Schnittpunkte; irc = 1, Schnittpkt = Mittelpkt
  if(UTP_comp2db(lp1->y, lp2->y, UT_TOL_min1)) {
    UT2D_pt_mid2pt (pto, lp1, lp2);
    return 1;
  }


  // Line schneidet;
   pto->x = lp1->x + ((yVal - lp1->y) / (lp2->y - lp1->y) *
                               (lp2->x - lp1->x));
   pto->y = yVal;


  // UT3D_stru_dump (Typ_PT2, pto, "ex UT2D_pt_intlny:  ");
  return 0;

}



//=======================================================================
  int UT2D_pt_intlnx (Point2 *pto, Point2 *lp1, Point2 *lp2, double xVal) {
//=======================================================================
/// \code
/// UT2D_pt_intlnx            intersection linesegment - vertical line
/// Schnittpunkt eines Linesegmentes ln1 mit einer unendlich langen,
/// vertical Geraden mit X=xVal errechnen.
/// Retcod= 0: Schnittpunkt in pto
/// Retcod=-1: kein Schnittpunkt
/// Retcod= 1: Linie vertical; beliebig viele Schnittpunkte
/// \endcode
// Achtung: wenn der Schnittpunkt sich ausserhalb des Geradensegmentes
//   befindet, wird kein Schnittpunkt geliefert!
// ACHTUNG: fuer UT_hatch_2D sind SEHR geringe Tol. erforderlich !


  // Lineseg oberhalb
  // if(((lp1->x-UT_TOL_min1) > xVal)&&((lp2->x-UT_TOL_min1) > xVal)) return -1;
  if((lp1->x > xVal)&&(lp2->x > xVal)) return -1;


  // Lineseg unterhalb
  // if(((lp1->x+UT_TOL_min1) < xVal)&&((lp2->x+UT_TOL_min1) < xVal)) return -1;
  if((lp1->x < xVal)&&(lp2->x < xVal)) return -1;


  // Linie horizontal; nur Schnittpunkte; irc = 1, Schnittpkt = Mittelpkt
  if(UTP_comp2db(lp1->x, lp2->x, UT_TOL_min1)) {
    UT2D_pt_mid2pt (pto, lp1, lp2);
    return 1;
  }


  // Line schneidet;
   pto->x = xVal;
   pto->y = lp1->y + ((xVal - lp1->x) * (lp2->y - lp1->y) /
                               (lp2->x - lp1->x));


  // UT3D_stru_dump (Typ_PT2, pto, "ex UT2D_pt_intlnx:  ");
  return 0;

}



//=======================================================================
  int UT2D_pt_int4pt (Point2 *ps, double *dp1, double *dp2, double *tol,
                      Point2 *p1,Point2 *p2,Point2 *p3,Point2 *p4) {
//=======================================================================
/// \code
/// intersection of 2 limited lines, tolerance.
/// Info, ob Schnittpunkt ident mit p1 oder p2 (p3,p4 werden nicht getestet)
/// Info, ob p1-p2 und p3-p4 uebereinanderliegen (ueberlappen)
/// Output:
///   dp1       parameter along p1-p2
///   dp2       parameter along p3-p4
///   RC = -1   NO intersection.
///   RC =  0   Yes; intersect; ps=p1; dp1=0;
///   RC =  1   Yes; intersect; ps between p1-p2;
///             dp1 ist der Abstandswert (0-1) des Schnittpunktes entlang p1-p2
///   RC =  2   Yes; intersect; ps=p2; dp1=1;
///   RC =  3   Lines parallel - Endpoints touch (not overlap). ps, dp1 unused.
///   RC =  4   Lines parallel and overlap; ps, dp1 unused.
/// \endcode

//  (war:    Yes; overlap; all 4 point are on the same line; ps not used.
//           dp1 ist 0 oder 1 (wenn Linien ident = komplett ueberlappen)

// siehe auch UT2D_pt_ck_onLine UT2D_pt_ck_int4pt
// see also UT2D_pt_intlnln


  int     irc;
  double  q1, q2, qq, d1, d2, hx1,lx1,hy1,ly1, hx2,lx2,hy2,ly2;
  Vector2 vc1, vc2, vcs;

  //printf("    l1=%f,%f  - %f,%f\n",p1->x,p1->y,p2->x,p2->y);
  //printf("    l2=%f,%f  - %f,%f\n",p3->x,p3->y,p4->x,p4->y);


  // UT3D_stru_dump(Typ_PT2, p1," p1 ");
  // UT3D_stru_dump(Typ_PT2, p2," p2 ");
  // UT3D_stru_dump(Typ_PT2, p3," p3 ");
  // UT3D_stru_dump(Typ_PT2, p4," p4 ");


  //==================================================================
  // boxtest

  irc = -1;

  lx1 = DMIN (p1->x, p2->x);
  hx2 = DMAX (p3->x, p4->x);
  // if((lx1 + tol) > (hx2 - tol)) goto L_fertig;
  if(hx2 < lx1 - *tol) goto L_fertig;

  lx2 = DMIN (p3->x, p4->x);
  hx1 = DMAX (p1->x, p2->x);
  // if((lx2 + tol) > (hx1 - tol)) goto L_fertig;
  if(lx2 > hx1 + *tol) goto L_fertig;

  ly1 = DMIN (p1->y, p2->y);
  hy2 = DMAX (p3->y, p4->y);
  // if((ly1 + tol) > (hy2 - tol)) goto L_fertig;
  if(hy2 < ly1 - *tol) goto L_fertig;

  ly2 = DMIN (p3->y, p4->y);
  hy1 = DMAX (p1->y, p2->y);
  // if((ly2 + tol) > (hy1 - tol)) goto L_fertig;
  if(ly2 > hy1 + *tol) goto L_fertig;



  //==================================================================
  UT2D_vc_2pt (&vc1, p1, p2);
  UT2D_vc_2pt (&vc2, p3, p4);
  UT2D_vc_2pt (&vcs, p1, p3);


  // UT3D_stru_dump(Typ_VC2, &vc1," vc1 ");
  // UT3D_stru_dump(Typ_VC2, &vc2," vc2 ");
  // UT3D_stru_dump(Typ_VC2, &vcs," vcs ");


  q1 = vc1.dy * vc2.dx;
  q2 = vc1.dx * vc2.dy;
  qq = q1 - q2;
  // printf(" qq=%f q1=%f q2=%f\n",qq,q1,q2);


  // check for parallel
  if (fabs(qq) > 0.0001) goto L_nicht_parall;


  q1 = vc1.dy * vcs.dx;
  q2 = vc1.dx * vcs.dy;
  qq = q1 - q2;
  // printf(" qq=%f q1=%f q2=%f\n",qq,q1,q2);

  // check for collinear
  if (fabs(qq) > 0.0001) goto L_nicht_collin;


  //==================================================================
  // lines sind collinear (alle Punkte auf der gleichen Linie)
  L_collin:

  // die Linien ueberlappen teilweise ..
  irc = 3;

  // *dp1 = 0.;

  // Tests ob Linien beruehren ..
  if(fabs(hx1-lx2) < *tol) goto L_fertig;
  if(fabs(hy1-ly2) < *tol) goto L_fertig;

  irc  = 4;   // Teilueberdeckung, nicht nur Beruehrung
  *dp1 = 1.;

  goto L_fertig;




  //==================================================================
  L_nicht_collin:  // parallel, aber nicht auf der gleichen Linie

  // Normalabstand
  d1 = (vc1.dx*vcs.dy - vc1.dy*vcs.dx) / qq;
  // printf(" d1=%f\n",d1);

  if (fabs(d1) < *tol) goto L_collin;

  // parallel, aber nicht auf der gleichen Linie
  // Abstand ist > Tol; also keine Verbindung moeglich
  irc = -1;
  goto L_fertig;





  //==================================================================
  L_nicht_parall:

  // d1 ist der Abstand des Schnittpunkts auf vc1; 0=p1, 1=p2.
  // d2 ist der Abstand des Schnittpunkts auf vc2; 0=p3, 1=p4.

  d1 = (vc2.dx*vcs.dy - vc2.dy*vcs.dx) / qq;
  d2 = (vc1.dx*vcs.dy - vc1.dy*vcs.dx) / qq;
  // printf(" d1=%f d2=%f\n",d1,d2);




  // Endpunkte vergleichen
  if(d1 < 0.5) {              // SP bei p1 oder aussen
    if(d2 < 0.5) {            // SP bei p1 und p3; test p1=p3
      if(fabs(p1->x - p3->x) > *tol) goto L_test_sp;
      if(fabs(p1->y - p3->y) > *tol) goto L_test_sp;
      goto L_0;   // RC =  0   Yes; p1=p3;

    } else {                 // SP bei p1 und p4; test p1=p4
      if(fabs(p1->x - p4->x) > *tol) goto L_test_sp;
      if(fabs(p1->y - p4->y) > *tol) goto L_test_sp;
      goto L_0;   // RC =  0   Yes; p1=p4;

    }

  } else {                   // SP bei p2
    if(d2 < 0.5) {     // test p2=p3
      if(fabs(p2->x - p3->x) > *tol) goto L_test_sp;
      if(fabs(p2->y - p3->y) > *tol) goto L_test_sp;
      goto L_3;   // RC =  2   Yes; p2=p3;

    } else {           // test p2=p4
      if(fabs(p2->x - p4->x) > *tol) goto L_test_sp;
      if(fabs(p2->y - p4->y) > *tol) goto L_test_sp;
      goto L_4;   // RC =  2   Yes; p2=p4;
    }
  }


  // Endpunkte nicht ident; Test SP-Abstand
  L_test_sp:

  // errechnen Schnittpunkt
  ps->x = p1->x + vc1.dx * d1;
  ps->y = p1->y + vc1.dy * d1;
  // printf(" L_test_sp: %f,%f\n",ps->x,ps->y);

/*
  // ist ps naeher an p1 oder naeher an p2 ?
  if(d1 < 0.5) {               // PS bei p1
    if(fabs(ps->x - p1->x) > *tol) goto L_m1;
    if(fabs(ps->y - p1->y) > *tol) goto L_m1;
    goto L_0;   // RC =  0   Yes; ps=p1;
*/


  // ist Schnittpunkt innerhalb l1:
  if(d1 < 0.) {               // PS bei p1

    if(d2 < 0.)  goto L_m1;
    if(d2 > 1.)  goto L_m1;

    // PS bei p1 und in p3-p4
    if(fabs(ps->x - p1->x) > *tol) goto L_m1;
    if(fabs(ps->y - p1->y) > *tol) goto L_m1;
    goto L_0;   // RC =  0   Yes; ps=p1;


  } else if(d1 > 1.) {        // PS bei p2

    if(d2 < 0.)  goto L_m1;
    if(d2 > 1.)  goto L_m1;

    // PS bei p2 und in p3-p4
    if(fabs(ps->x - p2->x) > *tol) goto L_m1;
    if(fabs(ps->y - p2->y) > *tol) goto L_m1;
    // der Schnittpunkt liegt auf l1 bei p2.
    goto L_2;   // RC =  2   Yes; ps=p2;



  } else {                    // PS innerhalb p1-p2

    if(d2 < 0.)  {            // PS in l1 bei p3
      if(fabs(ps->x - p3->x) > *tol) goto L_m1;
      if(fabs(ps->y - p3->y) > *tol) goto L_m1;
      goto L_3;   // RC =  1   Yes; ps=p3 between p1-p2

    } else if(d2 > 1.) {      // PS in l1 bei p4
      if(fabs(ps->x - p4->x) > *tol) goto L_m1;
      if(fabs(ps->y - p4->y) > *tol) goto L_m1;
      goto L_4;   // RC =  1   Yes; ps=p4 between p1-p2


    } else {                  // PS in l1 und in l2
                              // ps kann immer noch bei p1 od p2 liegen
      if(d1 < 0.5) {               // PS bei p1
        if(fabs(ps->x - p1->x) > *tol) goto L_1;
        if(fabs(ps->y - p1->y) > *tol) goto L_1;
        goto L_0;   // RC =  0   Yes; ps=p1;

      } else {                     // PS bei p2
        if(fabs(ps->x - p2->x) > *tol) goto L_1;
        if(fabs(ps->y - p2->y) > *tol) goto L_1;
        goto L_2;   // RC =  2   Yes; ps=p2;
      }
    }
  }



  // den Laengsabstand auf p1-p2 errechnen
  // dl = vcs.dx * vc1.dx + vc1.dy * vcs.dy;
  // printf(" dl=%f\n",dl);



  L_0:
  // RC =  0   Yes; ps=p1;
  irc = 0;
  *dp1 = 0.;
  *dp2 = d2;
  ps->x = p1->x;
  ps->y = p1->y;
  goto L_fertig;


  L_1:
  // RC = 1   Yes; ps between p1-p2    ist sauberer schnitt.
  irc = 1;
  *dp1 = d1;
  *dp2 = d2;

  goto L_fertig;


  L_2:
  // RC =  2   Yes; ps=p2;
    // printf("L_2: %f %f\n",d1,d2);
    // printf("    l1=%f,%f  - %f,%f\n",p1->x,p1->y,p2->x,p2->y);
    // printf("    l2=%f,%f  - %f,%f\n",p3->x,p3->y,p4->x,p4->y);
  irc = 2;
  *dp1 = 1.;
  *dp2 = d2;
  ps->x = p2->x;
  ps->y = p2->y;
  goto L_fertig;


  L_3:
  // RC =  1   Yes; ps=p3;
  //printf("L_3: %f %f\n",d1,d2);
  //printf("    l1=%f,%f  - %f,%f\n",p1->x,p1->y,p2->x,p2->y);
  //printf("    l2=%f,%f  - %f,%f\n",p3->x,p3->y,p4->x,p4->y);
  irc = 1;
  *dp1 = d1;
  *dp2 = 0.;
  ps->x = p3->x;
  ps->y = p3->y;
  goto L_fertig;


  L_4:
  // RC =  1   Yes; ps=p4;
  //printf("L_4: %f %f\n",d1,d2);
  //printf("    l1=%f,%f  - %f,%f\n",p1->x,p1->y,p2->x,p2->y);
  //printf("    l2=%f,%f  - %f,%f\n",p3->x,p3->y,p4->x,p4->y);
  irc = 1;
  *dp1 = d1;
  *dp2 = 1.;
  ps->x = p4->x;
  ps->y = p4->y;
  goto L_fertig;





  //=====================================================
  L_m1:
  irc = -1;


  //=====================================================
  L_fertig:
  //printf("ex UT2D_pt_int4pt %d %f,%f\n",irc,ps->x,ps->y);
  return irc;

}


//=======================================================================
  int UT2D_pt_intlnln (Point2 *ip1,
                        Line2 *ln1, int ln1Mode,
                        Line2 *ln2, int ln2Mode) {
//=======================================================================
/// \code
/// UT2D_pt_intlnln         intersect Line x Line; both limited or unlimited
/// lnxMode 0=limited Line, 1=unlimited Line.
/// Retcod 0    OK
///       -1    no intersectionpoint
/// see also UT2D_pt_int2ln UT2D_pt_int4pt
/// \endcode


  int       irc1, irc2, i1;
  Vector2   vl1, vl2;



  printf("UT2D_pt_intlnln %d %d\n",ln1Mode,ln2Mode);
    UT3D_stru_dump(Typ_LN2, ln1, "");
    UT3D_stru_dump(Typ_LN2, ln2, "");


  irc2 = -1;

  // intersect unlimited line X unlimited line
  UT2D_vc_ln (&vl1, ln1);
  UT2D_vc_ln (&vl2, ln2);
  irc1 = UT2D_pt_int2vc2pt ((Point2*)ip1, &ln1->p1, &vl1, &ln2->p1, &vl2);
  if(irc1 <= 0) goto L_exit;


  //----------------------------------------------------------------
  if(ln1Mode > 0) goto L_ck_ip2;

  // ip1 must be inside both segments.
  // check if ip1 is on ln1; 1=yes, 0=no
  i1 = UT2D_pt_ck_inLine (&ln1->p1, &ln1->p2, ip1, UT_TOL_pt);
  if(i1 < 1) goto L_exit;
  irc2 = 0;

  //----------------------------------------------------------------
  L_ck_ip2:
  if(ln2Mode > 0) {irc2 = 0; goto L_exit;}
  // check if ip1 is on ln1; 1=yes, 0=no
  i1 = UT2D_pt_ck_inLine (&ln2->p1, &ln2->p2, ip1, UT_TOL_pt);
  if(i1 > 0) irc2 = 0;
    // printf(" ip1: %d\n",irc2);


  //----------------------------------------------------------------
  L_exit:
    printf("UT2D_pt_intlnln %d\n",irc2);
    if(irc2 > 0) printf("      pt %f %f\n",ip1->x,ip1->y);

  return irc2;

}


//======================================================================
  int UT2D_pt_int2ln (Point2 *ip, Line2 *l1, Line2 *l2) {
//======================================================================
/// \code
/// UT2D_pt_int2ln            point = intersection of 2 lines
/// 
/// Returncodes:
///  0 = Lines parallel or zero-length
///  1 = OK.
/// \endcode


  Vector2   v1, v2;

  /* TX_Print("UT2D_pt_int2ln %f,%f   %f,%f\n",l1->p1.x,l1->p1.y,l1->p2.x,l1->p2.y); */
  /* TX_Print("               %f,%f   %f,%f\n",l2->p1.x,l2->p1.y,l2->p2.x,l2->p2.y); */

  /* line 1 */
  UT2D_vc_2pt (&v1, &l1->p1, &l1->p2);

  /* line 2 */
  UT2D_vc_2pt (&v2, &l2->p1, &l2->p2);


  return UT2D_pt_int2pt2vc (ip, &l1->p1, &v1, &l2->p2, &v2);

}


//================================================================
  int UT2D_2par_int2pt2vc (double *par1, double *par2,
                          Point2 *pa, Vector2 *vab,
                          Point2 *pc, Vector2 *vcd) {
//================================================================
/// \code
/// UT2D_2par_int2pt2vc     intersect 2 unlimitedLines; gives 2 parameters
/// Input:
///   pa-vab   Line1 (pa-pb)
///   pc-vcb   Line2 (pc-pd)
/// Output:
///   par1 = Parameter of IntersectPt auf pa-pb
///   par2 = Parameter of IntersectPt auf pc-pd
///   irc = -1 = parallel
///   irc =  0 = OK;
///
///                         d
///                        /.
///                       / .
///                      /  .
///                     /   .
///                    c    .
///                   ..    .
///                  . .    .
///                 .  .    .
///          a-----+---e----f-----------------------b
///               ip
/// \endcode

  int       irc;
  double    s_ab_ab, s_ab_ac, s_ab_cd, s_ac_cd, s_cd_cd, q0, q1, q2;
  Vector2   vac;  //, vad;
  Point2    pm1, pm2;

  // printf("UT2D_2par_int2pt2vc \n");
  // UT3D_stru_dump (Typ_PT2, pa,  " pa =");
  // UT3D_stru_dump (Typ_VC2, vab, " vab =");
  // UT3D_stru_dump (Typ_PT2, pc,  " pc =");
  // UT3D_stru_dump (Typ_VC2, vcd, " vcd =");


  // check for lines parallel or antiparallel
  if (UT2D_comp2vc_p (vab, vcd, UT_TOL_min1)) {
    //printf("UT3D_pt_int2ln parallel error\n");
    irc = -1;
    // UT3D_pt_projptptvc (&ip2, dist, p1, p3, &v);
    goto Done;
  }


  UT2D_vc_2pt (&vac, pa, pc);           // vac = Vector a-c
    // UT3D_stru_dump (Typ_VC, &vac, " vac =");

  s_ab_ab = UT2D_skp_2vc (vab, vab);  // skp mit sich selbst = Laenge^2
  s_ab_ac = UT2D_skp_2vc (vab, &vac);  // gibt Wert fuer e relativ zu s_ab_ab
  s_ab_cd = UT2D_skp_2vc (vab, vcd);
  s_ac_cd = UT2D_skp_2vc (&vac, vcd);
  s_cd_cd = UT2D_skp_2vc (vcd, vcd);
    // printf(" s_ab_ab=%f\n",s_ab_ab);
    // printf(" s_ab_ac=%f\n",s_ab_ac);
    // printf(" s_ab_cd=%f\n",s_ab_cd);
    // printf(" s_ac_cd=%f\n",s_ac_cd);
    // printf(" s_cd_cd=%f\n",s_cd_cd);

  // q1 = param vom pi1 auf ab  (zw 0 und 1)
  // q2 = param vom pi2 auf cd  (zw 0 und 1)
  // q0 = (s_ab_ab * s_cd_cd) - (s_ab_cd * s_ab_cd);
  // if(fabs(q0) < UT_TOL_min1) return 1;
  // *par1 = ((s_ab_ac * s_cd_cd) - (s_ab_cd * s_ac_cd)) / q0;
  // *par2 = ((s_ab_ac * s_ab_cd) - (s_ab_ab * s_ac_cd))  / q0;

  q0 = s_ab_ab * s_cd_cd - s_ab_cd * s_ab_cd;

  if(fabs(q0) < UT_TOL_min2) {irc = -1; goto Done;}

  q1 = (s_ab_ac * s_cd_cd) - (s_ac_cd * s_ab_cd);

  *par1 = q1 / q0;

  *par2 = ((s_ab_cd * (*par1)) - s_ac_cd) / s_cd_cd;

  irc = 0;

/*
    // TESTBLOCK:
    {Point ip1, ip2;
      ip1.x = pa->x + *par1 * vab->dx;
      ip1.y = pa->y + *par1 * vab->dy;
      ip1.z = pa->z + *par1 * vab->dz;
      UT3D_stru_dump (Typ_PT, &ip1, " ip1 =");
  
      ip2.x = pc->x + *par2 * vcd->dx;
      ip2.y = pc->y + *par2 * vcd->dy;
      ip2.z = pc->z + *par2 * vcd->dz;
      UT3D_stru_dump (Typ_PT, &ip2, " ip2 =");
    }
    // TESTBLOCK:
*/

  Done:
    // printf("ex UT2D_2par_int2pt2vc irc=%d par1=%f par2=%f\n",irc,*par1,*par2);

  return irc;

}


//================================================================
  int UT2D_pt_int2pt2vc (Point2 *ip, Point2 *pt1, Vector2 *vc1,
                                     Point2 *pt2, Vector2 *vc2) {
//================================================================
/// \code
/// UT2D_pt_int2pt2vc         point = intersection of 2 vectors + 2 Points
/// intersection of 2 unlimited lines
///
/// Returncodes:
///  0 = Lines parallel or zero-length
///  1 = OK; ip = intersectionPoint;
///
/// see also UT2D_pt_int2vc2pt
/// \endcode


  Vector2    vs;
  double     q1, dist1, dist2;


  // printf("UT2D_pt_int2pt2vc %f,%f %f,%f\n",pt1->x,pt1->y,vc1->dx,vc1->dy);
  // printf("                  %f,%f %f,%f\n",pt2->x,pt2->y,vc2->dx,vc2->dy);


  q1 = vc1->dy * vc2->dx - vc1->dx * vc2->dy;

  // check for lines parallell
  if (fabs(q1) < UT_TOL_min1) { return 0; }    // 2007-11-10   was 0.0001

  // line1Start -> line2Start
  vs.dx = pt2->x - pt1->x;
  vs.dy = pt2->y - pt1->y;

  dist1 = (vc2->dx*vs.dy - vc2->dy*vs.dx) / q1;
  // printf("    dist1 %f\n",dist1);

  ip->x = pt1->x + vc1->dx * dist1;
  ip->y = pt1->y + vc1->dy * dist1;

  // printf(" ex UT2D_pt_int2vc2pt %d %f,%f %f\n",irc,ip->x,ip->y,dist1);

  return 1;

}


//================================================================
  int UT2D_pt_int2vc2pt (Point2 *ip, Point2 *pt1, Vector2 *vc1,
                                     Point2 *pt2, Vector2 *vc2) {
//================================================================
/// \code
/// UT2D_pt_int2vc2pt         point = intersection of 2 vectors + 2 Points
///
/// Es wird auch getestet, ob der Schnittpunkt ip sich auf oder ausserhalb des
///   vc1 / vc2 befindet.
///
/// Returncodes:
///  0 = Lines parallel or zero-length
///  1 = OK; Schnittpunkt befindet sich auf vc1 und auf vc2.
///  2 = OK; Schnittpunkt befindet sich nicht auf vc1 (vc2 wird nicht getestet)
///  3 = OK; Schnittpunkt befindet sich auf vc1, nicht auf vc2.
/// \endcode


  int        irc;
  Vector2    vs;
  double     q1, dist1, dist2;



  // printf("UT2D_pt_int2vc2pt %f,%f %f,%f\n",pt1->x,pt1->y,vc1->dx,vc1->dy);
  // printf("                  %f,%f %f,%f\n",pt2->x,pt2->y,vc2->dx,vc2->dy);


  q1 = vc1->dy * vc2->dx - vc1->dx * vc2->dy;

  // check for lines parallell
  if (fabs(q1) < UT_TOL_min1) { return 0; }    // 2007-11-10   was 0.0001


  // line1Start -> line2Start
  UT2D_vc_2pt (&vs, pt1, pt2);

  dist1 = (vc2->dx*vs.dy - vc2->dy*vs.dx) / q1;
  // printf("    dist1 %f\n",dist1);

  ip->x = pt1->x + vc1->dx * dist1;
  ip->y = pt1->y + vc1->dy * dist1;

  irc = 2;
  if((dist1 < 0.)||(dist1 > 1.)) goto L_fertig;


  dist2 = (vc1->dx*vs.dy - vc1->dy*vs.dx) / q1;
  // printf("    dist2 %f\n",dist2);


  irc = 3;
  if((dist2 < 0.)||(dist2 > 1.)) goto L_fertig;

  irc = 1;



  L_fertig:
  // printf(" ex UT2D_pt_int2vc2pt %d %f,%f %f\n",irc,ip->x,ip->y,dist1);

  return irc;
}



//=========================================================================
  void UT2D_2pt_projptci (Point2 *ip1,Point2 *ip2,Point2 *pt,
                         Point2 *cen, double rd1) {
//=========================================================================
/// \code
/// UT2D_2pt_projptci         project point to circle
/// 
/// ip1 ist the nearer solution,
/// ip2 is farer away from cen
/// \endcode


  Vector2 vc;

  /* ----------------------------------------------- */
  /* vc = cen -> pt */
  UT2D_vc_2pt (&vc, cen, pt);

  /* ip1 = cen -> pt; length rd1; */
  UT2D_pt_traptvclen (ip1, cen, &vc, rd1);

  /* ip2 = cen -> pt; length -rd1; */
  UT2D_pt_traptvclen (ip2, cen, &vc, -rd1);

  /* printf("UT2D_2pt_projptci %f %f\n",ip1->x,ip1->y); */
  /* printf("                 %f %f\n",ip2->x,ip2->y); */

  return;
}


//========================================================================
  int UT2D_pt_projptci (Point2 *pp, Point2 *pt, Circ2 *ci1) {
//========================================================================
/// \code
/// UT2D_pt_projptci         project point to Arc (segment of circle)
/// 2 solutions: der nearest point is selected.
/// RetCod: 0 = yes, pp is in Arcsegment;
///        -1 = no; none of the 2 projectionpoints is on the ArcSegment.
/// \endcode


  int     irc;
  Point2  p1;
  Vector2 vc;



  //-----------------------------------------------
  // check if vec cen-pt is inside arc
  p1 = *pt;
  irc = UT2D_pt_ck_inAc (&p1, ci1);
  if(irc == 0) goto L_1;


  //-----------------------------------------------
  // check if vec pt-cen is inside arc
  // p1 = pt opposit pc
  UT2D_pt_opp2pt (&p1, &ci1->pc, pt);
  irc = UT2D_pt_ck_inAc (&p1, ci1);
  if(irc != 0) {irc = -1; goto L_exit;}


  //-----------------------------------------------
  L_1:
  // vc = cen -> pt
  UT2D_vc_2pt (&vc, &ci1->pc, &p1);
  // ip1 = cen -> pt; length rd1;
  UT2D_pt_traptvclen (pp, &ci1->pc, &vc, fabs(ci1->rad));
  irc = 0;


  //-----------------------------------------------
  L_exit:
  printf("ex UT2D_pt_projptci %d %f %f\n",irc,pp->x,pp->y);

  return irc;
}


//==========================================================================
  int UT2D_2pt_intciptvc (Point2 *ip1,Point2 *ip2,Point2 *cen1,double rd1,
                                                Point2 *pl, Vector2 *vl) {
//==========================================================================
/// \code
/// UT2D_2pt_intciptvc        intersect Circle - Line (pt+vc)
/// 
/// ip1 immer jener Punkt, 
/// 
/// Return code:
///   0 ... normal intersection (2 Ergebnispunkte)
///   1 ... circle touches line
///   -1 .. no intersection
/// \endcode


  Vector2  vcc;
  Point2   phc;
  double   abst_c, dy, lp;
  int      rc;



  /* TX_Print("UT2D_2pt_intciptvc cen %f %f  rd=%f\n",cen1->x,cen1->y,rd1); */
  /* TX_Print(" pl=%f %f, vl=%f,%f\n",pl->x,pl->y,vl->dx,vl->dy); */


  /* project center to line */
  UT2D_pt_projptptvc (&phc, cen1, pl, vl);


  /* dist c1 - phc */
  abst_c = UT2D_len_2pt (cen1, &phc);

  /* the centerline */
  UT2D_vc_2pt (&vcc, cen1, &phc);

  if (UTP_comp_0 (abst_c)) {
    UT2D_vc_setLength (&vcc, vl, rd1);
    ip1->x = cen1->x - vcc.dx;
    ip1->y = cen1->y - vcc.dy;
    ip2->x = cen1->x + vcc.dx;
    ip2->y = cen1->y + vcc.dy;
    rc = 0;
    goto Fertig;
  }

  /* do they touch */
  if (UTP_comp2db (abst_c, rd1, UT_TOL_min1)) {
    lp = rd1 / abst_c;
    ip1->x = cen1->x + vcc.dx*lp;
    ip1->y = cen1->y + vcc.dy*lp;
    *ip2 = *ip1;
    rc = 1;
    goto Fertig;
  }

  /* no intersection at all */
  if (abst_c > rd1) {
    rc = -1;
    goto Fertig;
  }

  dy = sqrt(rd1*rd1 - abst_c*abst_c);


  /* ip = translate phc normal to dir vcc - length dy */
  /* Compute: */
  lp = dy / abst_c;

  ip1->x = phc.x - vcc.dy*lp;
  ip1->y = phc.y + vcc.dx*lp;

  ip2->x = phc.x + vcc.dy*lp;
  ip2->y = phc.y - vcc.dx*lp;

  rc = 0;

  Fertig:
/*
  TX_Print("    %f %f %f %f\n",abst_c,dy,phc.x,phc.y);
  TX_Print(" ex UT2D_2pt_intciptvc %d ip1=%f,%f ip2=%f,%f\n",rc,
                                      ip1->x,ip1->y,ip2->x,ip2->y);
*/

  return rc;

}


//==========================================================================
  int UT2D_2pt_intcici (Point2 *ip1,Point2 *ip2,Point2 *cen1,double rd1,
                                                Point2 *cen2,double rd2) {
//==========================================================================
/// \code
/// UT2D_2pt_intcici                intersect 2 Circles
///   
/// Return code:
///   0 ... normal intersection
///   1 ... circles touch each other
///   -1 .. circles do not intersect
/// \endcode


  Vector2  vcc;
  Point2   phc;
  double   abst_c, rd12, dx, dy, lp;
  int      rc;

  /* printf("UT2D_2pt_intcici %f %f %f\n",cen1->x,cen1->y,rd1); */
  /* printf("                 %f %f %f\n",cen2->x,cen2->y,rd2); */



  /* dist c1 - c2 */
  abst_c = UT2D_len_2pt (cen1, cen2);

  rd12 = rd1 + rd2;

  /* printf(" abst=%f\n",abst_c); */


  /* the centerline */
  UT2D_vc_2pt (&vcc, cen1, cen2);


  /* do they touch */
  if ((UTP_comp2db (abst_c, rd1+rd2, UT_TOL_min1)) ||        // berühren aussen
      (UTP_comp2db (abst_c, fabs(rd1-rd2), UT_TOL_min1))) {  //  berühren innen
    lp = rd1 / abst_c;
    ip1->x = cen1->x + vcc.dx*lp;
    ip1->y = cen1->y + vcc.dy*lp;
    *ip2 = *ip1;
    rc = 1;
    goto Fertig;
  }


  /* no intersection at all */
  if ((abst_c > rd12) ||
      ((abst_c + rd1) < rd2) ||
      ((abst_c + rd2) < rd1)) {
    rc = -1;
    goto Fertig;
  }


  /* compute triangle */
  UT2D_solvtri_abc (&dx, &dy, rd2, rd1, abst_c);
  /* printf("  dx,dy=%f %f\n",dx,dy); */


  /* phc = translate cen1 into dir vcc - length dx */
  lp = dx / abst_c;
  phc.x = cen1->x + vcc.dx*lp;
  phc.y = cen1->y + vcc.dy*lp;
  /* printf("  phc=%f %f\n",phc.x,phc.y); */


  /* ip = translate phc normal to dir vcc - length dy */
  lp = dy / abst_c;

  ip1->x = phc.x - vcc.dy*lp;
  ip1->y = phc.y + vcc.dx*lp;

  ip2->x = phc.x + vcc.dy*lp;
  ip2->y = phc.y - vcc.dx*lp;

  rc = 0;

  Fertig:
  /* printf("  %f %f %f %f %f %f\n",abst_c,rd12,dx,dy,phc.x,phc.y); */
  /* printf("ex UT2D_2pt_intcici %d %f,%f/%f,%f\n",rc,ip1->x,ip1->y,ip2->x,ip2->y); */
  return rc;

}


//=======================================================================
  int UT2D_2pt_intlnci (Point2 *ip1, Point2 *ip2,
                        Line2 *ln, int lnMode,
                        Circ2 *ci, int ciMode) {
//=======================================================================
/// \code
/// UT2D_2pt_intlnci         intersect Line x Circ; both limited or unlimited 
/// lnMode 0=limited Line, 1=unlimited Line.
/// ciMode 0=limited Circ, 1=unlimited Circ.
/// Retcod 1/2  number of intersectionpoints
///       -1    no intersectionpoint
/// \endcode
 
  int       irc1, irc2, i1;
  // Point2  ip1, ip2;
  Vector2   vl;


  // printf("UT2D_2pt_intlnci %d %d\n",lnMode,ciMode);
    // UT3D_stru_dump(Typ_LN2, ln, "");
    // UT3D_stru_dump(Typ_CI2, ci, "");



  irc2 = -1;

  // intersect 360-deg-circ X unlimited line
  UT2D_vc_ln (&vl, ln);
  irc1 = UT2D_2pt_intciptvc (ip1, ip2,
                            &ci->pc, fabs(ci->rad), &ln->p1, &vl);
  // -1=no, 0=yes, 1=touch
  if(irc1 < 0) goto L_exit;


  //----------------------------------------------------------------
  // ip1 testen;  0=lim; 1=unlim

  // ip1 must be inside both segments.
  // check if ip1 is on ln; 1=yes, 0=no
  L_ip1_ln:
  if(lnMode > 0) goto L_ip1_ci;
  i1 = UT2D_pt_ck_inLine (&ln->p1, &ln->p2, ip1, UT_TOL_pt);
  if(i1 < 1) goto L_ck_ip2;

  // check if ip1 is on ci; 0=yes, else no
  L_ip1_ci:
  if(ciMode > 0) {irc2 = 1; goto L_ck_ip2;}
  i1 = UT2D_pt_ck_inAc (ip1, ci);
  if(i1 == 0) irc2 = 1;
    //printf(" ip1: %d\n",irc2);


  //----------------------------------------------------------------
  // ip2 testen
  L_ck_ip2:
  if(lnMode > 0) goto L_ip2_ci;

  // check if ip2 is on ln; 1=yes, 0=no
  L_ip2_ln:
  i1 = UT2D_pt_ck_inLine (&ln->p1, &ln->p2, ip2, UT_TOL_pt);
    //printf(" i1-ln: %d\n",i1);
  if(i1 < 1) goto L_exit;

  // check if ip2 is on ci; 0=yes, else no
  L_ip2_ci:
  if(ciMode > 0) goto L_ip2_ok;
  i1 = UT2D_pt_ck_inAc (ip2, ci);
    //printf(" i1-ci: %d\n",i1);
  if(i1 != 0) goto L_exit;


  // ip2 is inside both segments.
  // if ip1 is not inside segments, then ip1 = ip2.
  L_ip2_ok:
  if(irc2 < 0) {
    UT2D_swap2pt (ip1, ip2);
    irc2 = 1;
  } else {
    irc2 = 2;
  }


  //----------------------------------------------------------------
  L_exit:

    // printf("ex UT2D_2pt_intlnci %d\n",irc2);
    // if(irc2 > 0) printf("      pt %f %f\n",ip1->x,ip1->y);
    // if(irc2 > 1) printf("      pl %f %f\n",ip2->x,ip2->y);
    // //getchar();

  return irc2;

}






//=======================================================================
  int UT2D_2pt_int2ci (Point2 *ip1, Point2 *ip2,
                       Circ2 *ci1, int ci1Mode,
                       Circ2 *ci2, int ci2Mode) {
//=======================================================================
/// \code
/// UT2D_2pt_int2ci         intersect Circ x Circ; both limited or unlimited
/// ciXMode 0=limited Circ, 1=unlimited Circ.
/// Retcod 1/2  number of intersectionpoints
///       -1    no intersectionpoint
/// \endcode

  int       irc1, irc2, i1;
  // Point2  ip1, ip2;
  Vector2   vl;

/*
  printf("UT2D_2pt_int2ci:\n");
  UT3D_stru_dump(Typ_CI2, ci1, "   ci1: mode=%d\n",ci1Mode);
  UT3D_stru_dump(Typ_CI2, ci2, "   ci2: mode=%d\n",ci2Mode);
*/

  irc2 = -1;

  // intersect unlimited; 0=ok;
  irc1 = UT2D_2pt_intcici (ip1, ip2,
                           &ci1->pc, fabs(ci1->rad),
                           &ci2->pc, fabs(ci2->rad));
  if(irc1 != 0) goto L_exit;


  //----------------------------------------------------------------
  // ip1 testen; 0=limited; 1=unlim.
  if(ci1Mode > 0) goto L_ip1_c2;

  // ip1 must be inside both segments.
  // check if ip1 is on ci1; 0=yes, else no
  L_ip1_c1:
  i1 = UT2D_pt_ck_inAc (ip1, ci1);
    printf(" i1c1=%d\n",i1);
//getchar();
  if(i1 != 0) goto L_ck_ip2;


  // check if ip1 is on ci2; 0=yes, else no
  L_ip1_c2:
  if(ci2Mode > 0) {irc2 = 1; goto L_ck_ip2;}
  i1 = UT2D_pt_ck_inAc (ip1, ci2);
    printf(" i1c2: %d\n",i1);
//getchar();
  if(i1 == 0) irc2 = 1;


  //----------------------------------------------------------------
  // ip2 testen
  L_ck_ip2:

  if(ci1Mode > 0) goto L_ip2_c2;

  // check if ip2 is on ci1; 1=yes, 0=no
  i1 = UT2D_pt_ck_inAc (ip2, ci1);
    printf(" i2c2: %d\n",i1);
  if(i1 != 0) goto L_exit;

  // check if ip2 is on ci2; 0=yes, else no
  L_ip2_c2:
  if(ci2Mode > 0) goto L_ip2_ok;
  i1 = UT2D_pt_ck_inAc (ip2, ci2);
    printf(" i2c2: %d\n",i1);
  if(i1 != 0) goto L_exit;

  // ip2 is inside both segments.
  // if ip1 is not inside segments, then ip1 = ip2.
  L_ip2_ok:
  if(irc2 < 0) {
    UT2D_swap2pt (ip1, ip2);
    irc2 = 1;
  } else {
    irc2 = 2;
  }


  //----------------------------------------------------------------
  L_exit:
/*
    printf("ex UT2D_2pt_int2ci %d\n",irc2);
    if(irc2 > 0) printf("      pt %f %f\n",ip1->x,ip1->y);
    if(irc2 > 1) printf("      pl %f %f\n",ip2->x,ip2->y);
    getchar();
*/

  return irc2;

}



/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/




//=========================================================================
  void UT2D_ln_4db (Line2 *ln2, double xs, double ys, double xe, double ye) {
//=========================================================================
/// UT2D_ln_4db               2D-Line from 4 doubles

  ln2->p1.x = xs;
  ln2->p1.y = ys;
  ln2->p2.x = xe;
  ln2->p2.y = ye;

  return;

}


//====================================================================
  Line2 UT2D_ln_obj2 (ObjG2 *obj1) {
//====================================================================
/// \code
/// UT2D_ln_obj2              2D-Obj > 2D-Line
/// DO NOT USE
/// \endcode


  static Line2 ln1;


  ln1.p1 = obj1->p1;
  ln1.p2 = obj1->p2;

  /* printf("exit UT2D_obj_ln3 %f,%f bis %f,%f\n",o1.p1.x,o1.p1.y,o1.p2.x,o1.p2.y); */

  return ln1;

}







/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/


//======================================================================
  int UT2D_comp2vc (Vector2 *v1, Vector2 *v2, double tol) {
//======================================================================
/// \code
/// UT2D_comp2vc              compare 2 vectors for parallel and antiparallel
/// Vectors must not be normalized.
/// tol applies to length 1
/// Returns 1 if parallel=yes, else 0.
/// if(UT2D_comp2vc(&vc1, &vc2, UT_TOL_pt)) printf("  parallel=yes");
/// \endcode

  short  i1;
  Vector2 vc1, vc2;

  /* same length necessary */
  UT2D_vc_normalize (&vc1, v1);
  UT2D_vc_normalize (&vc2, v2);

  i1 =  (UTP_comp2db (vc1.dx, vc2.dx, tol)) &&
        (UTP_comp2db (vc1.dy, vc2.dy, tol));

  /* TX_Print("ex UT2D_comp2vc %d %f,%f %f,%f",i1,vc1.dx, vc1.dy,vc2.dx, vc2.dy); */

  if (i1) return i1;

  UT2D_vc_invert (&vc2, &vc2);

  return ((UTP_comp2db (vc1.dx, vc2.dx, tol)) &&
          (UTP_comp2db (vc1.dy, vc2.dy, tol)));

}


//======================================================================
  int UT2D_comp2vc_p (Vector2 *v1, Vector2 *v2, double tol) {
//======================================================================
/// \code
/// UT2D_comp2vc_p            compare 2 vectors for parallel
/// Compare directions; lenght is normalized (tol applies to length 1)
/// Returns 1 wenn parallel, else 0.
/// \endcode

  Vector2 vc1, vc2;

  /* same length necessary */
  UT2D_vc_normalize (&vc1, v1);
  UT2D_vc_normalize (&vc2, v2);

  return ((UTP_comp2db (vc1.dx, vc2.dx, tol)) &&
          (UTP_comp2db (vc1.dy, vc2.dy, tol)));
}


//================================================================
  int UT2D_sid_2vc (Vector2 *v1, Vector2 *v2, double tol) {
//================================================================
/// \code
/// UT3D_cklr_2vc        check vc for left (CCW, above) or right (CW, below)
/// Vektoren muessen nicht normiert sein !
/// RC   1   v2 is CCW from v1  (pos. angle)
/// RC   0   v1-v2 are parallel
/// RC  -1   v2 is CW from v1   (neg. angle)
/// 
///              X
///           v1/          1
///            /
/// ----------X---v2----------->  0
///            
///                
///                       -1
/// 
/// see also UT2D_sidPerp_2vc
/// \endcode

  int    irc;
  double sk;



  // UT3D_stru_dump (Typ_VC, v1, "UT3D_sid_2vc");
  // UT3D_stru_dump (Typ_VC, v2, "              ");


  // see UT2D_sar_2vc
  sk = v1->dx * v2->dy - v2->dx * v1->dy;

/*
  if(sk < -UT_TOL_min2)     irc = -1;
  else if(sk > UT_TOL_min2) irc = 1;
*/
  if(sk < -tol)     irc = -1;
  else if(sk > tol) irc = 1;
  else                      irc = 0;

    // printf("ex UT2D_sid_2vc %d %f\n",irc,sk);

  return irc;

}


//================================================================
  int UT2D_sidPerp_2vc (Vector *v1, Vector *v2) {
//================================================================
/// \code
/// UT2D_sidPerp_2vc        check vecs for inFront, perp.(normal), or behind
/// Ist v2 vor oder hinter einem Normalvektor auf v1 durch den Startpunkt von v1.
/// sind Vektoren eher parallel oder anpiparallel ..
/// Vektoren muessen nicht normiert sein !
/// RC  -1 vecs point into opposit direction (angle > 90 deg.)
/// RC   0 vecs are perpendicular  (angle = 90 deg)
/// RC   1 vecs point into same direction (angle < 90 deg)
/// 
/// see also UT2D_sid_2vc UT3D_acos_2vc
/// \endcode

  int    irc;
  double sk;



  // UT3D_stru_dump (Typ_VC, v1, "UT3D_sid_2vc");
  // UT3D_stru_dump (Typ_VC, v2, "              ");


  sk = v1->dx * v2->dx + v1->dy * v2->dy;

  // irc = UTP_COMP_0(sk);

  if(sk < -UT_TOL_min1)     irc = -1;
  else if(sk > UT_TOL_min1) irc = 1;
  else                      irc = 0;

    printf("ex UT2D_sidPerp_2vc %d %f\n",irc,sk);

  return irc;

}


//================================================================
  int UT2D_ckvc_hori (Vector2 *vc1) {
//================================================================
/// \code
/// UT2D_ckvc_hori         test if vector is horizontal
/// vc1 should be normalized
/// 
/// RetCod:
///   0 = yes; vc1 is horizontal
///   1 = no;  vc1 is not horizontal
/// \endcode


  // UT3D_stru_dump (Typ_VC2, vc1, "UT2D_ckvc_hori:");

// test vector for (1,0)
  if((fabs(vc1->dx) > UT_TOL_min0)&&(fabs(vc1->dy) < UT_TOL_min1)) return 0;

  return 1;

}


//================================================================
  int UT2D_ckvc_vert (Vector2 *vc1) {
//================================================================
/// \code
/// UT2D_ckvc_vert         test if vector is vertical
/// vc1 should be normalized
/// 
/// RetCod:
///   0 = yes; vc1 is vertical
///   1 = no;  vc1 is not vertical
/// \endcode


  // UT3D_stru_dump (Typ_VC2, vc1, "UT2D_ckvc_vert:");

// test vector for (0,1)
  if((fabs(vc1->dx) < UT_TOL_min1)&&(fabs(vc1->dy) > UT_TOL_min0)) return 0;

  return 1;

}


//===============================================================
  int UT2D_ckvc_in2vc (Vector2 *v1, Vector2 *v2, Vector2 *v3) {
//===============================================================
/// \code
/// UT2D_ckvc_in2vc         check if v2 is between v1 and v3
/// the opening-angle of v1-v3 is less than 180 deg.
/// 
/// RetCod:
///   0 = yes; v2 is between v1 and v3
///   1 = no;  v2 = above (left of) v1 and v3
///  -1 = no;  v2 = below (right of) v1 and v3
/// \endcode

  int   s1, s2, s3;


  // test if v3 is above (ccw) or below (cw) v1
  s1 = UT2D_sid_2vc (v1, v3, UT_TOL_min2);

  // test if v2 is above (ccw) or below (cw) v1
  s2 = UT2D_sid_2vc (v1, v2, UT_TOL_min2);
    // printf(" s1=%d s2=%d\n",s1,s2);

  if(s1 == -1) goto L_cw;


  //----------------------------------------------------------------
  if(s1 == 0) {
    // v1 = v3
    // if(s2 == 0) return 0;  // v1 = v2 = v3
    return s2;
  }


  //----------------------------------------------------------------
  // v3 is above v1.
  if(s2 == -1) return -1;  // v2 below v1; v3 above v1.

  // test if v2 is above (ccw) or below (cw) v3
  s3 = UT2D_sid_2vc (v3, v2, UT_TOL_min2);
    // printf(" s3=%d\n",s3);

  if(s3 == 1) return 1;  // v2 above v3; v3 above v1.

  return 0;


  //----------------------------------------------------------------
  // v3 is below v1
  L_cw:
  if(s2 == 1) return 1;  // v2 above v1; v3 below v1.

  // test if v2 is above (ccw) or below (cw) v3
  s3 = UT2D_sid_2vc (v3, v2, UT_TOL_min2);
    // printf(" s3=%d\n",s3);

  if(s3 == -1) return -1;  // v2 below v3; v3 below v1.

  return 0;  // yes, inside ..

}



/* inline ..
  void UT2D_vc_2db (Vector2 *vc, double x, double y) {
UT2D_vc_2db              2D-Vector < 3 doubles (x, y)

  vc->dx = x; vc->dy = y;

  return;

}



UT2D_vc_angr            2DVector =  angle (radians)

  vc->dx = cos (angr);
  vc->dy = sin (angr);

  // printf("UT2D_vcD_ang %f %f %f\n",vc->dx,vc->dy,angr);

  return;

}
*/


//====================================================================
  void UT2D_vc_angrlen (Vector2 *vc, double angr, double lenv) {
//====================================================================
/// UT2D_vc_angrlen            2DVector =  angle (radians) + Length


  /* printf("UT2D_vc_angrlen %f %f\n",angr,lenv); */

  vc->dx = cos (angr) * lenv;
  vc->dy = sin (angr) * lenv;

  /* printf("exit UT2D_vcD_angrlen %f %f %f\n",vc->dx,vc->dy,angr); */

  return;

}



/*
//================================================================
  void UT2D_vc_2pt (Vector2 *vc, Point2 *p1, Point2 *p2) {
//================================================================

// UT2D_vc_2pt              2D-Vector = 2D-Point -> 2D-Point


  vc->dx = p2->x - p1->x;
  vc->dy = p2->y - p1->y;


  // printf("UT2D_vc_2pt = %f, %f\n", vc->dx, vc->dy);

  return;

}
*/


//========================================================================
  void UT2D_vc_2pt3 (Vector2 *vc, Point *p1, Point *p2) {
//========================================================================
/// UT2D_vc_2pt3              2D-Vector = 3D-Point -> 3D-Point

  vc->dx = p2->x - p1->x;
  vc->dy = p2->y - p1->y;


  /* printf("UT2D_vc_2pt = %f, %f\n", vc->dx, vc->dy); */

  return;

}


//================================================================
  int UT2D_vc_2pt3_bp (Vector2 *vo, Point *p1, Point *p2, int bp) {
//================================================================
/// 2D-Vector = (3D-Point -> 3D-Point) on Backplane

  if       (bp == BCKPLN_XY) {         // skip z
    vo->dx = p2->x - p1->x;
    vo->dy = p2->y - p1->y;

  } else if(bp == BCKPLN_XZ) {         // skip y
    vo->dx = p2->x - p1->x;
    vo->dy = p2->z - p1->z;

  } else {                             // skip x
    vo->dx = p2->y - p1->y;
    vo->dy = p2->z - p1->z;
  }

  return 0;

}


//========================================================================
  void UT2D_vc_2ptlen (Vector2 *vc, Point2 *p1, Point2 *p2, double lenv) {
//========================================================================
/// UT2D_vc_2ptlen            2D-Vector = 2D-Point -> 2D-Point, set Length


  /* -------------------------------------------------- */
  vc->dx = p2->x - p1->x;
  vc->dy = p2->y - p1->y;

  UT2D_vc_setLength (vc, vc, lenv);

  /* printf("UT2D_vc_2ptlen %f, %f\n", vc->dx, vc->dy); */
/*
  printf("               %f %f\n",p1->x,p1->y);
  printf("               %f %f\n",p2->x,p2->y);
  printf("               %f\n",lenv);
*/

  return;

}


/*
//================================================================
 void UT2D_vc_invert (Vector2 *vco, Vector2 *vci ) {
//================================================================
// UT2D_vc_invert          2D-Vector invert (change direction)


  // Vector2 vcIn;

  // TX_Print("UT2D_vc_invert %f,%f",vci->dx,vci->dy);

  // vcIn = *vci;

  vco->dx = -vci->dx;
  vco->dy = -vci->dy;

  // TX_Print(" ex UT2D_vc_invert %f,%f",vco->dx,vco->dy);

  return;

}


//================================================================
  void UT2D_vc_perpvc (Vector2 *vp, Vector2 *vc) {
//================================================================
// UT2D_vc_perpvc            vector = perpendic. to vector ( + 90 degrees)


  double d1;

  d1 = vc->dx;   //  zwischenspeichern, denn vp=vc wäre sonst Problem !

  vp->dx = -vc->dy;
  vp->dy = d1;

  // printf("UT2D_vc_perpvc %f %f\n",vp->dx,vp->dy);
  // printf("               %f %f\n",vc->dx,vc->dy);

}


//================================================================
  void UT2D_vc_perp2pt (Vector2 *vp, Point2 *p1, Point2 *p2) {
//================================================================
// UT2D_vc_perp2pt           vector = perpendic. to Line ( + 90 degrees)


  double dx, dy;

  dx = p2->x - p1->x;
  dy = p2->y - p1->y;

  vp->dx = -dy;
  vp->dy = dx;

  // printf("UT2D_vc_perpvc %f %f\n",vp->dx,vp->dy);
  // printf("               %f %f\n",vc->dx,vc->dy);

}
*/



//==================================================================
  void UT2D_vc_normalize (Vector2 *vn, Vector2 *vc) {
//==================================================================
/// UT2D_vc_normalize         change to length 1



  UT2D_vc_setLength (vn, vc, 1.);

  return;
}


//====================================================================
void UT2D_vc_setLength (Vector2 *vco, Vector2 *vci, double new_len) {
//====================================================================
/// \code
/// UT2D_vc_setLength                change 2D-Vectorlength
/// vco and vci may be the same address
///
/// if length of vci is known:
///   UT2D_vc_multvc (&vco, &vci, newLengthOfVco / actLengthOfVci);
/// \endcode


  double len;

  len = UT2D_len_vc (vci);

  if (len != 0.0)
    {
      len = new_len / len;
      vco->dx = vci->dx*len;
      vco->dy = vci->dy*len;
      /* printf("UT2D_vc_setLength %f %f %f\n",vco->dx,vco->dy,new_len); */
    }
  else
    {
      /* printf("**** UT2D_vc_setLength error\n"); */
      vco->dx = 1.0;
      vco->dy = 0.0;
    }

}


//=====================================================================
  void UT2D_vc_mid2vc (Vector2 *vm, Vector2 *v1, Vector2 *v2) {
//=====================================================================
/// \code
/// UT2D_vc_mid2vc            vector = midVector between 2 vectors
/// \endcode
// 
// VM ist mittig zwischen v1 und v2 (CCW, vom Schnittpunkt aus).
// Die Vektoren normieren, dann addieren - ergibt winkelhalbierenden Vektor.
// 
// Achtung: gegenparallel . ergibt 0!


  Vector2 vc1, vc2;


  /* normalize vecs */
  UT2D_vc_normalize (&vc1, v1);
  UT2D_vc_normalize (&vc2, v2);

  vm->dx = vc1.dx + vc2.dx;
  vm->dy = vc1.dy + vc2.dy;

  /* 1,0 / 0,1 makes 0,0: fix that */
  if ((UTP_comp_0 (vm->dx)) &&
      (UTP_comp_0 (vm->dy))) {
    /* vm = normal to v1 */
    UT2D_vc_perpvc (vm, &vc1);
  }

  /* printf("UT2D_vc_mid2vc %f %f\n",vm->dx,vm->dy); */
  /* printf("               %f %f\n",vc1.dx,vc1.dy); */
  /* printf("               %f %f\n",vc2.dx,vc2.dy); */

  return;

}

//================================================================
  void UT2D_vc_merge2vc (Vector2 *vm, Vector2 *v1, Vector2 *v2) {
//================================================================
/// UT2D_vc_merge2vc          merge 2 vectors   vNew = (v1 + v2) / 2


  vm->dx = (v1->dx + v2->dx) / 2.;
  vm->dy = (v1->dy + v2->dy) / 2.;

  // UT3D_stru_dump (Typ_VC2, vm, "ex UT2D_vc_merge2vc:");

}


//====================================================================
  void UT2D_vc_rotangr (Vector2 *vco,Vector2 *vci,double alpha) {
//====================================================================
/// \code
/// UT2D_vc_rotangr                  rotate a 2D_vector
/// 
/// (vco and vci can be the same adress)
/// \endcode

  Vector2 vs;
  double  sin_alpha, cos_alpha;

  sin_alpha = sin(alpha);
  cos_alpha = cos(alpha);

  vs.dx = cos_alpha * vci->dx + sin_alpha * vci->dy;
  vs.dy = cos_alpha * vci->dy - sin_alpha * vci->dx;

  *vco = vs;

}


//===================================================================
  int UT2D_2par_vc_vcx_vcy (double *dx, double *dy,
                            Vector2 *vc1, Vector2 *vcx, Vector2 *vcy) {
//===================================================================
/// \code
/// UT2D_2par_vc_vcx_vcy    get parameters of vector projected onto vx,vy
/// vcx and vcy must have length=1
/// 
///   vcy
///    ^             
///    |
///    |---dx---|
///    |        
///    |        x   ---
///    |       /     |
///    |   vc1/      |
///    |    /        dy
///    |   /         |
///    | /           |
///    x---------------------> vcx
///
/// retour: UT3D_pt_tra_pt_2vc1_2par
/// \endcode

// see also UT2D_vc_tra_vcx_vcy UT3D_nlen_projvcvc UT3D_nlen_projvcvc


  UT3D_stru_dump(Typ_VC2, vc1, "UT2D_2par_vc_vcx_vcy vc1:");
  UT3D_stru_dump(Typ_VC2, vcx, "  vcx:");
  UT3D_stru_dump(Typ_VC2, vcy, "  vcy:");

  *dx = (vcx->dx * vc1->dx + vcx->dy * vc1->dy);
  *dy = (vcy->dx * vc1->dx + vcy->dy * vc1->dy);

    printf("ex UT2D_2par_vc_vcx_vcy %lf %lf\n",*dx,*dy);

  return 0;

}


//================================================================
  int UT2D_vc_tra_vcx_vcy (Vector2 *vco, Vector2 *vci,
                           Vector2 *vcx, Vector2 *vcy) {
//================================================================
/// UT2D_vc_tra_vcx_vcy       translate vector int refsys from vcx,vcy
/// Input:
///   vcx,vcy      refsys; vectorlenght 1.

// see also UT2D_2par_vc_vcx_vcy

  Vector2   vx1, vy1;

  UT2D_vc_multvc (&vx1, vcx, vci->dx);
  UT2D_vc_multvc (&vy1, vcy, vci->dy);

  UT2D_vc_add2vc (vco, &vx1, &vy1);

    printf("ex UT2D_vc_tra_vcx_vcy %lf %lf\n",vco->dx,vco->dy);

  return 0;

}


//====================================================================
  void UT2D_vc_travcm2 (Vector2 *vo, Mat_3x2 mata, Vector2 *vi) {
//====================================================================
/// \code
/// UT2D_vc_travcm2          2D-vector transformation (3x2-matrix)
/// 
/// UT2D_vc_travcm2          Author: Thomas Backmeister         9.4.2003
/// 
/// Transformation of a 2D-vector with a 3x2-matrix.
/// 
/// IN:
///   Mat_3x2 mata ... transformation matrix
///   Vector2 *vi  ... 2D-vector
/// OUT:
///   Vector2 *vo  ... transformed 2D-vector
/// \endcode


  double x, y;

  //printf ("UT2D_vc_travcm2 in %f %f\n",vi->dx,vi->dy);

  x = vi->dx;
  y = vi->dy;

  vo->dx = mata[0][0] * x + mata[0][1] * y + mata[0][2];
  vo->dy = mata[1][0] * x + mata[1][1] * y + mata[1][2];
}




/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/


//================================================================
  double UT2D_sDist_ciSeg (double rd, double as) {
//================================================================
/// \code
/// UT2D_sDist_ciSeg          height of segment of circle from angle
///
/// Winkel eines Kreisbogens von Sehnenhöhe.
///
/// Input:
///   rd   radius
///   as   angle of segment
/// Output:
///   ds   Sehnenhöhe (max Abweichnung vom Kreisbogen)
///
///
///             -----
///         --    |     --
///       -       |ds       -
///     ----------+-----------
///     \         |         /
///       \       |       /
///       rd     as     /
///           \   |   /
///             \ | /
///               x
/// \endcode

// see also UT2D_len_ciSec

  double  ds;


  ds = rd - (rd * cos(as/2));


    // printf("UT2D_sDist_ciSeg %f rd=%f as=%f\n",ds,rd,as);

  return ds;


}


//================================================================
  double UT2D_len_ciSec (double hSec, double rCi) {
//================================================================
/// \code
/// UT2D_len_ciSec          cmp length of secant from radius + height of secant.
/// 
///                                     Gegeben:
///          _  - + -  _                  x = hSec = Hoehe der sekante
///        ^      |x     ^                c = rCi  = radius Kreis
///      /        |        \
///     +---a-----+---a-----+           Gesucht:
///               |       /               a = lSec = halbe Laenge der Sekante
///              b|     /
///               |   /c                b = c - x
///               | /                   a*a + b*b = c*c
///               +                     a = sqrt (c*c - b*b)
///              CirCen
/// \endcode
//
//
// Fuer Toleranzrechnungen:
// hSec = tol;
// Anzahl Ecken einer Kurve = lenTot / UT2D_len_ciSec (UT_DISP_cv, rMin) * 2.;

// see UT2D_sDist_ciSeg UT2D_cv_cin UT3D_ptNr_ci UT2D_ptNr_ci UT2D_angr_ciSec


  double  b;

  if(hSec >= rCi) return rCi;

  b = rCi - hSec;

  // printf("ex UT2D_len_ciSec %f %f %f\n",sqrt((rCi*rCi)-(b*b)),rCi,hSec);

  return sqrt((rCi*rCi) - (b*b));

}


//===========================================================================
  double UT2D_angr_ciSec (double hc, double radc) {
//===========================================================================
/// \code
/// UT2D_angr_ciSec           opening angle of Secant of Circ
///                           for length of secant see UT2D_len_ciSec
/// \endcode

// Eine Sekante trennt ein Stueck vom Kreis.
// IN: Dessen Hoehe ist hc (Beispiel: Sehnentoleranz bei Kreisdarstellung).
//     Kreisradius radc
// OUT: der Winkel des Kreises, dessen Endpunkte mit der Sekante gleich sind.

// Errechnen der Bogenlaenge des Kreisstueckes (zum Errechnen der Anzahl der
//  erforderlichen Polygonpunkte): Bogenlaenge = Radius * Winkel_rad
// lsec = UT2D_angr_ciSec (tol, rad) * rad;   // Bogenlaenge mit Toleranz
// ptNr = UT3D_len_ci (&ci1) / lsec;          // Anzahl Polygonpunkte

  if(hc >= radc) return RAD_90;

  return acos((radc-hc)/radc) * 2.0;

}


//==========================================================================
  int UT2D_compPtOnAc (double w1, double wa, double we, int dreh) {
//==========================================================================
/// \code
///  Testen, ob sich ein Punkt auf einem Arc befindet. Die Winkel
///  sind Input (ev. aus UT2D_angr_ptpt).
///  Out als RetCod: YES oder NO.
/// \endcode

  int irc;

  /*  Befindet sich Winkel w1 zwischen Winkel wa und we ?? */
  if(dreh == CCW) {

    /*  w1 und wa müssen > we sein */
    while (wa < 0.0) { wa += RAD_360; }
    while (w1 < wa)  { w1 += RAD_360; }
    while (we < wa)  { we += RAD_360; }

    if((UTP_comp2db(w1, we, UT_TOL_min1))||(UTP_comp2db(w1, we, UT_TOL_min1))) {
      irc = YES;

    } else if((w1 < wa)||(w1 > we)) {
      irc = NO;

    } else {
      irc = YES;
    }




  } else if(dreh == CW) {

    /*  w1 und we müssen > wa sein */
    while (we < 0.0) { we += RAD_360; }
    while (w1 < we)  { w1 += RAD_360; }
    while (wa < we)  { wa += RAD_360; }

    if((UTP_comp2db(w1, we, UT_TOL_min1))||(UTP_comp2db(w1, we, UT_TOL_min1))) {
      irc = YES;

    } else if((w1 < we)||(w1 > wa)) {
      irc = NO;
    } else {
      irc = YES;
    }


  }


  /* TX_Print("ex UT2D_compPtOnAc: irc=%d,wa=%f, w1=%f, we=%f",irc,wa,w1,we); */

  return irc;
}


//======================================================================
  int UT2D_ck_ci360 (Circ *ci1) {
//======================================================================
/// \code
/// UT2D_ck_ci360             check if circ is 360-degree-circ
/// RC = 0:   YES, is 360DegCirc
/// RC = 1:   NO
/// if(UT2D_ck_ci360 == YES) ...
/// \endcode

  int    irc;
  double tol;

  // tol = UT_TOL_pt * fabs(ci1->rad);
  tol = UT_TOL_pt;

  if(UTP_comp2db (fabs(ci1->ango), RAD_360, tol) == 1) irc = YES; // 1=diff < tol
  else irc = NO;

  // UT3D_stru_dump (Typ_CI, ci1, "ex UT3D_ck_ci360 %d\n",irc);

  return irc;

}


//====================================================================
  Circ2  UT2D_ci_obj2 (ObjG2 *ci20) {
//====================================================================
/// UT2D_ci_obj2              2D-Circ = 2D-ObjG


  Circ2    ci1;


  ci1.rad  = ci20->rad;

  ci1.p1.x = ci20->p1.x;
  ci1.p1.y = ci20->p1.y;

  ci1.p2.x = ci20->p2.x;
  ci1.p2.y = ci20->p2.y;

  ci1.pc.x = ci20->pc.x;
  ci1.pc.y = ci20->pc.y;


/*
  printf("UT2D_ci_obj2 %f,%f bis %f,%f\n",ci1.p1.x,ci1.p1.y,ci1.p2.x,ci1.p2.y);
  printf("          um %f,%f rad=%f\n",ci1.pc.x,ci1.pc.y,ci1.rad);
*/

  return ci1;

}



//====================================================================
  int UT2D_ci_2vc2ptrd (Point2 pta[], Point2 *pt1, Vector2 *vc1,
                                      Point2 *pt2, Vector2 *vc2, double rad1) {
//====================================================================
/// \code
/// UT2D_ci_2vc2ptrd          2D-Circ; verrundet 2 Lines; 4 Lösungen
/// 
/// Achtung: es werden nur die Kreismittelpunkte (4) retourniert !
/// 
/// pta[0] ist zwischen vc1 und vc2 (CCW, vom Schnittpunkt aus).
/// pta[1] ist genau gegenüber pta[0]. 
/// pta[2] ist normal auf pta[0] (also um 90 Grad CCW weiter als pta[0])
/// pta[3] ist genau gegenüber pta[2]
/// 
/// Retcode: 0=Error (Lines parallel);
/// \endcode


  int      i1;
  double   d1, d2, d3, d4;
  Point2   pts;  /*  Schnittpunkt */
  Vector2  vcn;  /*  normal auf die Winkelhalbierende */

/*
  TX_Print("UT2D_ci_2vc2ptrd p1=%f,%f vc1=%f,%f",pt1->x,pt1->y,vc1->dx,vc1->dy);
  TX_Print("   p2=%f,%f vc2=%f,%f",pt2->x,pt2->y,vc2->dx,vc2->dy);
  TX_Print("   rad=%f",rad1);
*/

    /*  den Schnittpunkt der Lines > pt20 */
    i1 = UT2D_pt_int2pt2vc (&pts, pt1, vc1, pt2, vc2);
    if(i1 == 0) {
      TX_Error(" Lines parallel");
      return 0;
    }
  /* TX_Print(" pts=%f,%f",pts.x,pts.y); */


  /*  den Öffnungswinkel errechnen */
  d1 = UT2D_angr_2vc (vc1, vc2);

  /*  die Strecke zum Mittelpunkt = Radius/sin(Öffnungswinkel/2) */
  d2 = rad1 / sin(d1/2.0);

  /*  der Komplementärwinkel d3 (180 - Öff.wi) und der zugehörige Abstand d4 */
  d3 = RAD_180 - d1;
  d4 = rad1 / sin(d3/2.0);
  /* TX_Print("  d1-d4=%f,%f,%f,%f\n",d1,d2,d3,d4); */


  /*  einen Vektor in Richtung der Winkelhalbierenden */
  UT2D_vc_mid2vc (&vcn, vc2, vc1); /*  Winkelhalbierende > vc22 */
  /* TX_Print(" vcn=%f,%f",vcn.dx,vcn.dy); */

  /*  die Strecke von pt20 weg in Richtung vcn abtragen */
  UT2D_pt_traptvclen (&pta[0], &pts, &vcn, d2);
  UT2D_pt_opp2pt (&pta[1], &pts, &pta[0]);              /*  gegenüberliegend */


  /*  und auch normal dazu ... (von pt20 weg normal auf pt20->pta[0]) */
  UT2D_pt_tranor2ptlen (&pta[2], &pts, &pta[0], d4);
  UT2D_pt_opp2pt (&pta[3], &pts, &pta[2]);              /*  gegenüberliegend */

/*
  // nur Test
  for(i1=0; i1<4; ++i1) {
    TX_Print("   pta[%d]= %f,%f\n",i1,pta[i1].x,pta[i1].y);
  }
*/

  return 1;

}




//====================================================================
  int UT2D_ci_ciptvcrd (Point2 pta[], Point2 *ptc, double radc, int dir,
                                      Point2 *pt1, Vector2 *vc1, double rad1) {
//====================================================================
/// \code
/// UT2D_ci_ciptvcrd          2D-Circ; verrundet Circ/Line; 4 Lösungen
/// 
/// Es werden nur die neuen Kreismittelpunkte (4 od. 2 od. 0) retourniert !
/// dir: bestimmt ob die 4 inneren oder die 4 ausserern Mittelpunkte
///   geliefert werden!   (sonst wären es 8 Lösungspunkte)
/// dir = 0:  alle Lösungen sind INNERHALB Cir1
/// dir = 1:  alle Lösungen sind AUSSERHALB Cir1
/// 
/// pta[0] ist zwischen der Linie pt1/vc1 und dem Mittelpkt ptc,
///        vom ptc weg in Richtung des vc1.
/// pta[1] ist zwischen der Linie pt1/vc1 und dem Mittelpkt ptc,
///        aber hinter ptc (bezügl Richtung vc1)
/// pta[2] ist gegenüber dem Mittelpunkt ptc bezügl. der Linie pt1/vc1,
///        vom ptc weg in Richtung des vc1 (wie Lösung [0])
/// pta[3] ist gegenüber dem Mittelpunkt ptc bezügl. der Linie pt1/vc1,
///        aber hinter ptc (bezügl Richtung vc1, wie Lösung [1])
/// 
/// Retcode: 0=Error (Lines parallel);
/// \endcode


  int      i1, i2, side;
  double   d1;
  Point2   pt20, pt21;
  /* Vector2  vcn;  // normal auf die Winkelhalbierende */

/*
  TX_Print("UT2D_ci_ciptvcrd pc=%f,%f rc=%f dir=%d",ptc->x,ptc->y,radc,dir);
  TX_Print("   pt1=%f,%f vc1=%f,%f",pt1->x,pt1->y,vc1->dx,vc1->dy);
  TX_Print("   rad=%f",rad1);
*/


  /*  liegt ptc links oder rechts von pt1/vc1 ? */
  side = UT2D_sid_ptvc (ptc, pt1, vc1);

  if(side > 0) {  /*  auf die gleiche Seite .. */
    d1 = rad1;
  } else {
    d1 = -rad1;
  }

  /*  den Anfanspunkt der Line nun normal zu vc2 verschieben > pt20 / pt21 */
  /*  (Parallellinien im Abstand Radius; auf diesen Linien ist der gesuchte Mittelpkt) */
  UT2D_pt_tranorptvclen (&pt20, pt1, vc1, d1);
  UT2D_pt_tranorptvclen (&pt21, pt1, vc1, -d1);
  /* TX_Print(" pt20=%f,%f pt21=%f,%f",pt20.x,pt20.y,pt21.x,pt21.y); */


  /*  d1 wird der Radius; je nach IN oder OUT klein od groß */
  if(dir == 0) {         /*  innen */
    d1 = radc - rad1;
  } else {                /*  aussen */
    d1 = radc + rad1;
  }

  /*  Schnitt Kreis - Linie; die Lösungen, die näher beim ptc liegen. */
  if(side > 0) {  /*  auf die gleiche Seite .. */
    i1=UT2D_2pt_intciptvc(&pta[0], &pta[1], ptc, d1, &pt20, vc1);
  } else {
    i1=UT2D_2pt_intciptvc(&pta[1], &pta[0], ptc, d1, &pt20, vc1);
  }
    /*  i2 = Anzahl Lösungen */
    if(i1 == 0) {
      i2 = 2;
    } else if (i1 == 1) {
      i2 = 1;
    } else {
      i2 = 0;
    }

  if(side > 0) {  /*  auf die gleiche Seite .. */
    i1=UT2D_2pt_intciptvc(&pta[i2], &pta[i2+1], ptc, d1, &pt21, vc1);
  } else {
    i1=UT2D_2pt_intciptvc(&pta[i2+1], &pta[i2], ptc, d1, &pt21, vc1);
  }
    if(i1 == 0) {
      i2 = i2 + 2;
    } else if (i1 == 1) {
      i2 = i2 + 1;
    }


  // nur Testausgaben:
  printf("ex UT2D_ci_ciptvcrd: %d\n",i2);
  for(i1=0;i1<i2;++i1) {
    TX_Print(" sol %d %f,%f",i1,pta[i1].x,pta[i1].y);
  }



  return i2;

}


/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/


//================================================================
  int UT2D_obj_obj3 (ObjGX *oo, ObjGX *oi, Memspc *memSeg) {
//================================================================
/// \code
/// UT2D_obj_obj3             change 3D-Obj > 2D-Obj (remove Z-val)
/// 3D-Objekt in 2D-Objekte umwandeln (remove Z-Wert)
/// \endcode

  void    *vp;

  // UT3D_stru_dump (Typ_ObjGX, oi, "UT2D_obj_obj3:");


  if(oi->form == Typ_PT) {
    vp = UME_reserve (memSeg, sizeof(Point2));
    *((Point2*)vp) = UT2D_pt_pt3 ((Point*)oi->data);
    OGX_SET_OBJ (oo, Typ_PT2, Typ_PT2, 1, vp);

  } else if(oi->form == Typ_LN) {
    vp = UME_reserve (memSeg, sizeof(Line2));
    UT2D_ln_ln3 ((Line2*)vp, ((Line*)oi->data));
    OGX_SET_OBJ (oo, Typ_LN2, Typ_LN2, 1, vp);

  } else if(oi->form == Typ_CI) {
    vp = UME_reserve (memSeg, sizeof(Circ2));
    UT2D_ci_ci3 ((Circ2*)vp, (Circ*)oi->data);
    OGX_SET_OBJ (oo, Typ_CI2, Typ_CI2, 1, vp);

  } else {
    TX_Print("UT2D_obj_obj3 E001 %d",oi->form);
    return -1;
  }

  // UT3D_stru_dump (Typ_ObjGX, oo, "ex UT2D_obj_obj3:");

  return 0;

}


//====================================================================
  ObjG2  UT2D_obj_pt3 (Point *pt1) {
//====================================================================
/// UT2D_obj_pt3              change 3D-Point > 2D-Obj


  ObjG2 o1;


  o1.typ  = Typ_PT2;
  o1.att  = 0;

  o1.p1.x = pt1->x;
  o1.p1.y = pt1->y;

  o1.p2.x = 0.0;
  o1.p2.y = 0.0;

  o1.pc.x = 0.0;
  o1.pc.y = 0.0;

  /* printf("exit UT2D_obj_ln3 %f,%f bis %f,%f\n",o1.p1.x,o1.p1.y,o1.p2.x,o1.p2.y); */

  return o1;

}





//====================================================================
  ObjG2  UT2D_obj_ln3 (Line *ln1) {
//====================================================================
/// \code
/// UT2D_obj_ln3              change 3D-Line > 2D-Obj
/// DO NOT USE
/// \endcode

  ObjG2 o1;


  o1.typ  = Typ_LN2;
  o1.att  = 0;

  o1.p1.x = ln1->p1.x;
  o1.p1.y = ln1->p1.y;

  o1.p2.x = ln1->p2.x;
  o1.p2.y = ln1->p2.y;


  /* printf("exit UT2D_obj_ln3 %f,%f bis %f,%f\n",o1.p1.x,o1.p1.y,o1.p2.x,o1.p2.y); */

  return o1;

}

//======================================================================
  ObjG2  UT2D_obj_ci2 (Circ2 *ci1) {
//======================================================================
/// \code
/// UT2D_obj_ci2              change 2D-Circ > 2D-Obj
/// DO NOT USE
/// \endcode

  ObjG2 o1;

  // printf("UT2D_obj_ci2 p1=%9.2f,%9.2f r=%9.2f\n",ci1->p1.x,ci1->p1.y,ci1->rad);


  o1.typ  = Typ_CI2;
  o1.att  = 0;

  o1.p1   = ci1->p1;
  o1.p2   = ci1->p2;
  o1.pc   = ci1->pc;

  o1.rad  = ci1->rad;

  return o1;

}


//====================================================================
  ObjG2  UT2D_obj_ci3 (Circ *ci1) {
//====================================================================
/// \code
/// UT2D_obj_ci3              change 3D-Kreis > 2D-Obj
/// DO NOT USE
/// \endcode
/*
  Circ     ci1;  double d1;
  d1=0.1;                       /  Sehnenfehler. /
  ci1.rad = 1.0;
  UT3D_vc_3db (&ci1.vz, 1.,0.,0.);
  UT3D_pt_3db (&ci1.pc, 0.,0.,0.);
  UT3D_pt_3db (&ci1.p1, 0.,1.,0.);

  UT3D_pt_3db (&ci1.p2, 0.,0.707,0.707);   /  45 Grad /
  UT3D_cv_ci (pa, &i1, &ci1, 140, d1);

  UT3D_pt_3db (&ci1.p2, 0.,0.,1.);         /  90 - Viertelkreis /
  UT3D_cv_ci (pa, &i1, &ci1, 140, d1);

  UT3D_pt_3db (&ci1.p2, 0.,-0.707,0.707);  /  135 Grad /
  UT3D_cv_ci (pa, &i1, &ci1, 140, d1);

  UT3D_pt_3db (&ci1.p2, 0.,-1.,0.);        /  180 - Halbkreis /
  UT3D_cv_ci (pa, &i1, &ci1, 140, d1);

  UT3D_pt_3db (&ci1.p2, 0.,-0.707,-0.707); /  225 Grad 
  UT3D_cv_ci (pa, &i1, &ci1, 140, d1);

  UT3D_pt_3db (&ci1.p2, 0.,0.,-1.);        /  270 - Dreiviertelkreis 
  UT3D_cv_ci (pa, &i1, &ci1, 140, d1);

  UT3D_pt_3db (&ci1.p2, 0.,0.707,-0.707);  /  315 Grad /
  UT3D_cv_ci (pa, &i1, &ci1, 140, d1);

  UT3D_pt_3db (&ci1.p2, 0.,1.,0.);         /  Vollkreis /
  UT3D_cv_ci (pa, &i1, &ci1, 140, d1);

*/

  ObjG2 o1;


  // printf("UT2D_obj_ci3 vz=%f,%f,%f\n",ci1->vz.dx,ci1->vz.dy,ci1->vz.dz);


  o1.typ  = Typ_CI2;
  o1.att  = 0;


  // wenn vz = 0,0,-1 umdrehen ?
  if(ci1->vz.dz == -1.) {
    printf("umdrehen\n");

    o1.p2.x = ci1->p1.x;
    o1.p2.y = ci1->p1.y;

    o1.p1.x = ci1->p2.x;
    o1.p1.y = ci1->p2.y;

    o1.rad  = ci1->rad;


  } else {

    o1.p1.x = ci1->p1.x;
    o1.p1.y = ci1->p1.y;

    o1.p2.x = ci1->p2.x;
    o1.p2.y = ci1->p2.y;

    o1.rad  = ci1->rad;

  }

  o1.pc.x = ci1->pc.x;
  o1.pc.y = ci1->pc.y;

  /* printf("exit UT2D_obj_ln3 %f,%f bis %f,%f\n",o1.p1.x,o1.p1.y,o1.p2.x,o1.p2.y); */

  return o1;

}



//=====================================================================
  ObjG2  UT2D_obj_cv3 (Curv *cv1) {
//=====================================================================
/// \code
/// UT2D_obj_cv3              change 3D-Curve > 2D-Obj
/// Die Indices (der Punktekette) sind in pc.x/pc.y.
/// DO NOT USE
/// \endcode


  ObjG2 o1;

  /* TX_Print("UT2D_obj_cv3 nr=%d ind1/2=%d/%d ID1/2=%d/%d", */
  /*           cv1->cvnr,cv1->ind1,cv1->ind2,cv1->ID1,cv1->ID2); */


  o1.typ  = Typ_CVPOL2;
  o1.att  = 0;

  o1.p1.x = cv1->p1.x;
  o1.p1.y = cv1->p1.y;

  o1.p2.x = cv1->p2.x;
  o1.p2.y = cv1->p2.y;

  o1.rad  = cv1->cvnr;

  o1.pc.x = cv1->ind1;
  o1.pc.y = cv1->ind2;

  o1.ID   = cv1->ID1;
  o1.ID1  = cv1->ID2;

  /* printf("exit UT2D_obj_ln3 %f,%f bis %f,%f\n",o1.p1.x,o1.p1.y,o1.p2.x,o1.p2.y); */

  return o1;

}


//====================================================================
  Point2 UT2D_pt_obj2 (ObjG2 *obj1) {
//====================================================================
/// \code
/// UT2D_pt_obj2              2D-Obj > 2D-Point
/// DO NOT USE
/// \endcode


  static Point2 pt1;


  pt1 = obj1->p1;

  /* printf("exit UT2D_obj_ln3 %f,%f bis %f,%f\n",o1.p1.x,o1.p1.y,o1.p2.x,o1.p2.y); */

  return pt1;

}



//=======================================================================
  int UT2D_void_obj2 (void *memObj, unsigned long *oSiz, ObjG2 *og2) {
//=======================================================================
/// \code
/// change ObjG2-Objekt -> data-Obj
/// DO NOT USE
/// \endcode

/*
  printf("UT2D_void_obj2 typ = %d p1=%9.2f,%9.2f r=%9.2f\n",
          og2->typ,og2->p1.x,og2->p1.y,og2->rad);
*/


  // int   typ;




  if((og2->typ == Typ_PT)||(og2->typ == Typ_PT2)) {
    *(Point2*)memObj = UT2D_pt_obj2(og2);
    *oSiz = sizeof(Point2);

	} else if((og2->typ == Typ_LN)||(og2->typ == Typ_LN2)) {
    *(Line2*)memObj = UT2D_ln_obj2(og2);
    *oSiz = sizeof(Line2);

  } else if((og2->typ == Typ_CI)||(og2->typ == Typ_CI2)) {
    *(Circ2*)memObj = UT2D_ci_obj2(og2);
    *oSiz = sizeof(Circ2);


  } else {
	  TX_Error("UT2D_void_obj2 typ = %d",og2->typ);
		return -1;
	}

  return 0;

}
					



/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/


//================================================================
  int UT2D_cv3_linear (int *pNr, Point *pTab, double tol) {
//================================================================
/// \code
/// UT2D_cv3_linear              delete unnecesary straight points
/// alle Punkte die innerhalb tol liegen aus pTab entfernen.
/// ACHTUNG: pTab sind 3D-Punkte; Z ignorieren !
/// \endcode


  int     ii, i1, i3, ie;
  double  dl;
  // char    ptStat[10000];   // max Anz Points pro Kontur
  char    *ptStat;
  // statt ptStat koennte man pt2a[x].x auf VAL_MAX setzen und dann danach suchen

  ie = *pNr - 1;  // index auf last point



  // printf("UT2D_cv3_linear %d\n",*pNr);


  if(*pNr < 3) return 0;

  // get memspc for ptStat
  ptStat = UME_alloc_tmp (*pNr);
  if(!ptStat) {TX_Error("UT2D_cv3_linear EOM"); return -1;}


  ptStat[0]  = 0;  // keep point
  ptStat[ie] = 0;  // keep point

  i1 = 0;
  i3 = 2;


  //----------------------------------------------------------------
  L_test:
  // printf("----- L_test: i2=%d i3=%d\n",i1,i3);

  for(ii=i1+1; ii<i3; ++ii) {
    dl = fabs(UT2D_slen_nor3pt ((void*)&pTab[i1],
                                (void*)&pTab[ii],
                                (void*)&pTab[i3]));
    // printf("   dl[%d]=%f\n",ii,dl);
    if(dl > tol) goto L_accept;
  }

  // alle Punkte zwischen i1 und i3 sind unnutz.
  ptStat[i3-1] = 1;   // delete point;
  i3 += 1;
  if(i3 <= ie) goto L_test;
  goto L_remove;


  //----------------------------------------------------------------
  L_accept:
  // der Punkt i3-1 muss bleiben.
  // Alle Punkte zwischen i1 und i3-1 entfernen.
  // i3-1 wird dann zu i1.
    // GR_Disp_pt (&pTab[i3-1], SYM_STAR_S, 2);
  ptStat[i3-1] = 0;  // keep point
  i1 = i3-1;
  i3 += 1;
  if(i3 <= ie) goto L_test;



  //----------------------------------------------------------------
  L_remove:
  UT3D_cv_delia (pNr, pTab, ptStat);

  // printf("ex UT2D_cv3_linear %d\n",*pNr);

  return 0;

}


//======================================================================
  int UT2D_cv_inv (int ptNr, Point2 *pta) {
//======================================================================
/// UT2D_cv_inv               Punktetabelle umdrehen

  int    inach, ivon, iSiz;


  // printf("UT2D_cv_inv %d\n",ptNr);

  iSiz = sizeof(Point2);
  inach = 0;
  ivon = ptNr - 1;


  L_umd_next:
  // printf("   vert. %d -> %d\n",ivon,inach);
  MEM_swap__ (&pta[inach], &pta[ivon], iSiz);
  ++inach;
  --ivon;
  if(inach < ivon) goto L_umd_next;


  return 0;

}


//=================================================================
  int UT2D_srar_polc (double *aro, int ptNr, Point2 *pa)  {
//=================================================================
/// \code
/// UT2D_srar_polc                         nach Karl Sauer 2004-04-07
/// Umlaufsinn (sr=sense of rotation) und Flaeche (ar=area) eines
/// geschlossenen 2D-Polygons (polc) berechnen.
/// 
/// ptNr = Anzahl Punkte ohne den letzten == ersten Punkt !
/// pa   = PunkteTabelle ohne den letzten == ersten Punkt !
/// RetCod = 1 = CCW
///         -1 = CW
/// \endcode
   
  double   FLAE;
  int      i1, i2, i3, ie, sr;


  FLAE = 0.;
  ie   = ptNr-1;

  for(i2=0; i2<ptNr; ++i2) {

    if(i2 > 0) i1 = i2-1;
    else       i1 = ie;

    if(i2 < ie) i3 = i2+1;
    else        i3 = 0;

    // nun ist p1,p2,p3 ein Dreieck aus dem polygon pa von 3
    // aufeinanderfolgenden Punkten; p2 ist der mittlere.
    FLAE += pa[i2].x * (pa[i3].y - pa[i1].y);
    // printf(" %d %d %d - %f\n",i1,i2,i3,FLAE);
  }


  if(FLAE < 0.) {
    sr = -1;
    *aro = FLAE / -2.;
  } else {
    sr = 1;
    *aro = FLAE / 2.0;
  }


  // printf("ex UT2D_poly_orient %d %f\n",sr,*aro);

  return sr;

}


//=================================================================
  int UT2D_srar_inpt (double *aro, int ptNr, int *ipa, Point2 *p2a)  {
//=================================================================
/// \code
/// UT2D_srar_inpt            Umlaufsinn und Flaeche eines indexed closed polygon
/// UT2D_srar_inpt                         nach Karl Sauer 2004-04-07
/// Umlaufsinn (sr=sense of rotation) und Flaeche (ar=area) eines
/// geschlossenen 2D-Polygons (polc) berechnen.
///
/// Input:
///   ptNr   Anzahl PunktIndices ohne den letzten == ersten Punkt !
///   ipa    Indexarray into pa;
///   pa     PunkteTabelle
/// Output:
///   RetCod = 1 = CCW
///           -1 = CW
/// \endcode
    
  double   FLAE;
  int      i1, i2, i3, ie, sr;
    

  FLAE = 0.;
  ie   = ptNr-1;

  for(i2=0; i2<ptNr; ++i2) {

    if(i2 > 0) i1 = i2-1;
    else       i1 = ie;

    if(i2 < ie) i3 = i2+1;
    else        i3 = 0;

    // nun ist p1,p2,p3 ein Dreieck aus dem polygon pa von 3
    // aufeinanderfolgenden Punkten; p2 ist der mittlere.
    // FLAE += pa[i2].x * (pa[i3].y - pa[i1].y);
    FLAE += p2a[ipa[i2]].x * (p2a[ipa[i3]].y - p2a[ipa[i1]].y);
    // printf(" %d %d %d - %f\n",i1,i2,i3,FLAE);
  }

  if(FLAE < 0.) {
    sr = -1;
    *aro = FLAE / -2.;
  } else {
    sr = 1;
    *aro = FLAE / 2.0;
  }

  // printf("ex UT2D_srar_inpt3 %d %f\n",sr,*aro);

  return sr;

}


//=================================================================
  int UT2D_srar_inpt3 (double *aro, int ptNr, int *ipa, Point *pa)  {
//=================================================================
/// \code
/// UT2D_srar_inpt3           Umlaufsinn und Flaeche eines indexed closed polygon
/// UT2D_srar_inpt3                        nach Karl Sauer 2004-04-07
/// Umlaufsinn (sr=sense of rotation) und Flaeche (ar=area) eines
/// geschlossenen 2D-Polygons (polc) berechnen.
///
/// Input:
///   ptNr   Anzahl PunktIndices ohne den letzten == ersten Punkt !
///   ipa    Indexarray into pa;
///   pa     PunkteTabelle
/// Output:
///   RetCod = 1 = CCW
///           -1 = CW
/// \endcode

  double   FLAE;
  int      i1, i2, i3, ie, sr;


  FLAE = 0.;
  ie   = ptNr-1;

  for(i2=0; i2<ptNr; ++i2) {

    if(i2 > 0) i1 = i2-1;
    else       i1 = ie;

    if(i2 < ie) i3 = i2+1;
    else        i3 = 0;

    // nun ist p1,p2,p3 ein Dreieck aus dem polygon pa von 3
    // aufeinanderfolgenden Punkten; p2 ist der mittlere.
    // FLAE += pa[i2].x * (pa[i3].y - pa[i1].y);
    FLAE += pa[ipa[i2]].x * (pa[ipa[i3]].y - pa[ipa[i1]].y);
    // printf(" %d %d %d - %f\n",i1,i2,i3,FLAE);
  }

  if(FLAE < 0.) {
    sr = -1;
    *aro = FLAE / -2.;
  } else {
    sr = 1;
    *aro = FLAE / 2.0;
  }

  // printf("ex UT2D_srar_inpt3 %d %f\n",sr,*aro);

  return sr;

}


//=====================================================================
  int UT2D_srar_3pt (double *aro, Point2 *p0, Point2 *p1, Point2 *p2) {
//=====================================================================
/// \code
/// UT2D_srar_3pt             Umlaufsinn und Flaeche eines 2D-triangle
/// UT2D_srar_3pt                          nach Karl Sauer 2004-04-07
/// Umlaufsinn (sr=sense of rotation) und Flaeche (ar=area) eines
/// geschlossenen 2D-Polygons (polc) berechnen.
///
/// Input:
/// Output:
///   RetCod = 1 = CCW
///           -1 = CW
/// \endcode

  double   FLAE;
  int      sr;


  FLAE = 0.;

  FLAE += p0->x * (p1->y - p2->y);
  FLAE += p1->x * (p2->y - p0->y);
  FLAE += p2->x * (p0->y - p1->y);


  if(FLAE < 0.) {
    sr = -1;
    *aro = FLAE / -2.;
  } else {

    sr = 1;
    *aro = FLAE / 2.0;
  }

  // printf("ex UT2D_srar_inpt3 %d %f\n",sr,*aro);

  return sr;

}
 

/*
//========================================================================
  int UT2D_cv_cin (int *ptNr, Point2 *cv,
                   Point2 *p1, Point2 *p2, Point2 *pc, double rdc,
                   int ptmax, double tol) {
//=========================================================================
/// Circ -> Polygon
/// Radius rdc > 0: CCW; else CW.
/// ptmax - In:  Max. Anzahl Points.
/// *ptNr - Out: Anzahl Points out.
/// tol   - if tol < 0. then polygon must have ptmax points
/// 
/// Retcode: 0 = OK,
///         -1 = cv zu klein

///  do not use; use UT2D_npt_ci

  int    pNr;
  Circ2  ci2;


  // create circ  UT2D_ci_3ptrd
  ci2.p1  = *p1;
  ci2.p2  = *p2;
  ci2.pc  = *pc;
  ci2.rad = rdc;
  ci2.ango = UT2D_angr_ci (p1, p2, pc, rdc);


  // fix nr of points according to modelsize
  if (tol < 0.0) {
    pNr = ptmax;
  } else {
    pNr = UT2D_ptNr_ci (fabs(rdc), fabs(ci2.ango), tol);
    if(pNr > ptmax)  pNr = ptmax;
  }
     // printf(" pNr=%d\n",pNr);


  // circ -> polygon
  UT2D_npt_ci (cv, pNr, &ci2);


  *ptNr = pNr;


// ab hier alte Version
  int     ind, iend, dreh, ptMax;
  double  d1, ad, ai, a1, lc, rad, tol, ao;

  ptMax = *ptNr;

  // printf("UT2D_cv_cin ptMax=%d ptAnz=%d r=%f\n",ptMax,ptAnz,rdc);
  // printf("        p1 %f,%f\n",p1->x,p1->y);
  // printf("        p2 %f,%f\n",p2->x,p2->y);
  // printf("        pc %f,%f\n",pc->x,pc->y);
  // GR_Disp_pt2 (p1, SYM_STAR_S, 2);
  // GR_Disp_pt2 (p2, SYM_STAR_S, 2);
  // GR_Disp_pt2 (pc, SYM_STAR_S, 2);

  // erster Pt
  cv[0] = *p1;

  // if(ptAnz < 2) {
    // ptAnz = 2;
    // ind = 0;
    // goto L_fertig;
  // }

  dreh = CCW;
  if(rdc < 0.) dreh = CW;

  lc = UT2D_len_cir (&ad, p1, p2, pc, rdc);

    // fixe punktezahl; Oeffnungswinkel
    // ad = UT2D_angr_3ptdr (p1, p2, pc, dreh);
    // if(dreh < 0) ad = -ad;

  if(ptAnz < 0) {

    // neg: Toleranz in -0.1 my (-10000 -> 1.; -100 -> 0.01; 
    d1 = -ptAnz;
    tol = d1 / 10000.;
    // printf(" tol=%f lc=%f ad=%f\n",tol,lc,ad);

    // Bogenlaenge = Radius * Winkel * 2
    rad = fabs(rdc);
    ao = acos((rad-tol) / rad) * 2.;  //  der Oeffnungswinkel
    if(ao > RAD_30)  ao = RAD_30;     // macht mind. 12 Ecken
    d1 = rad * ao;
    if(d1 < UT_TOL_min1) goto L_Err;
    ptAnz = (lc / d1) + 1;      // Anzahl Ecken d. Bogens

    // printf(" tol=%f lc=%f ad=%f d1=%f ptAnz=%d\n",tol,lc,ad,d1,ptAnz);

    // genug Platz im Ausgabefeld ?
    if(ptAnz > ptMax) {
      TX_Print("UT2D_cv_cin EOM");
      ptAnz = ptMax;
    }
    if(ptAnz < 2) ptAnz = 2;
  }

  // Punktanzahl ptAnz nun fix
  ai = ad / (ptAnz-1);

  ind  = 0;
  iend = ptAnz-2;
  a1 = 0.;
  // printf(" ai=%f ad=%f\n",ai,ad);

  if(ptAnz < 3) goto L_fertig;

  // Loop ueber die Punkte; incr.Winkel = ai
  L_nxt_pt:
  ++ind;
  a1 += ai;

  UT2D_pt_rotptangr (&cv[ind], pc, p1, a1);
    // printf(" a1=%f P[%d]=%f %f %f\n",a1,ind,cv[ind].x,cv[ind].y);

  if(ind < iend) goto L_nxt_pt;

  // letzter Punkt
  L_fertig:
  ++ind;
  cv[ind] = *p2;

  // // Testausg:
  // printf("ex UT2D_cv_cin %d\n",ptAnz);
  // for(ind=0; ind<ptAnz; ++ind) {
    // printf("        %d %f,%f\n",ind,cv[ind].x,cv[ind].y);
    // GR_Disp_pt2 (&cv[ind], SYM_TRI_S, 2);
    // GR_Disp_txi (&cv[ind], ind, 0);
  // }
  // // Testausg:

  *ptNr = ptAnz;
// bis hier alte Version



  return 0;



  L_Err:
   TX_Error("UT2D_cv_cin E001 (check Tolerance ?)");
   return -1;

}
*/


//================================================================
  int UT2D_npt_ci (Point2 *pa, int pNr, Circ2 *ci1) {
//================================================================
/// \code
/// UT2D_npt_ci               circular polygon
/// see also UT2D_cv_cin UT2D_pt_rotptangr
/// \endcode

  int        i1, segNr;
  double     ai;
  Mat_3x2    m1;


  // printf("UT2D_npt_ci %d\n",pNr);
  // UT3D_stru_dump (Typ_CI2, ci1, "");


  segNr = pNr - 1;

  if(pNr < 3) {
    pa[0] = ci1->p1;
    goto Fertig;
  }


  // den Incrementalwinkel ai errechnen
  ai = fabs(ci1->ango) / segNr;
  if(ci1->rad < 0.) ai = -ai;
    // printf(" ai=%f\n",ai);


  // init rotation around 0,0
  UT2D_m2_init_rot (m1, ai, (Point2*)&UT2D_PT_NUL);
  // UT2D_vc_angr (&vc1, ai);
  // UT2D_m2_load (m1, &vc1, &UT2D_PT_NUL);


  // startpt
  // rund um eine Achse im Nullpunkt drehen
  pa[0].x = ci1->p1.x - ci1->pc.x;
  pa[0].y = ci1->p1.y - ci1->pc.y;


  //  die Punkte rumdrehen
  for(i1=1; i1<segNr; ++i1) {
    UT2D_pt_traptm2 (&pa[i1], m1, &pa[i1-1]);
  }


  // Mittelpunkte aufaddieren
  for(i1=0; i1<segNr; ++i1) {
    pa[i1].x += ci1->pc.x;
    pa[i1].y += ci1->pc.y;
    // TX_Print(" %d %f,%f,%f",i1,cv[i1].x,cv[i1].y,cv[i1].z);
  }


  // Endpunkt
  Fertig:
  pa[segNr] = ci1->p2;


    // Testausgaben:
    // printf("ex UT3D_npt_ci %d\n",pNr);
    // for(i1=0; i1<pNr; ++i1) {
      // GR_Disp_pt2 (&pa[i1], SYM_TRI_S, 2);
      // GR_Disp_txi2 (&pa[i1], i1, 0);
      // GR_Disp_cv2 (pa, pNr, 9);
      // UT3D_stru_dump (Typ_PT, &cv[i1], " P[%d]=",i1);
    // }


  return 0;

}


//================================================================
  void UT2D_cv_ci360 (Point2 *cv, int ptNr, double rd, Point2 *ptCen) {
//================================================================
/// \code
/// Vollkreis --> Polygon.
/// insgesamt ptNr Punkt; erster und letzter sind gleich.
/// \endcode

  int    i1;
  double da, aa;
  Point2 *pta;


  // UT3D_stru_dump (Typ_PT2, ptCen, "UT2D_cv_ci360 %d %f",ptNr, rd);


  pta = cv;

  da = RAD_360 / (double)(ptNr-1);

  aa = 0.;

  for (i1=0; i1<ptNr; ++i1) {
    pta->x = sin(aa) * rd + ptCen->x;
    pta->y = cos(aa) * rd + ptCen->y;
    // GR_Disp_pt (pta, SYM_STAR_S, 2);
    aa += da;
    ++pta;
  }

  pta->x = rd + ptCen->x;
  pta->y = ptCen->y;

  // GR_Disp_cv2 (cv, ptNr, 5);
}


//===============================================================
  int UT2D_cv_ln (Point2 *cv, int ptAnz, Point2 *p1, Point2 *p2) {
//===============================================================
/// \code
/// UT2D_cv_ln         Linearstueck -> Polygon.
/// \endcode
// 
// In/Out: *ptAnz wird nur in 2 geandert, wenn < 2.


  int    ind, iend, ianz;
  double dx, dy;



  // printf("UT2D_cv_ln %d\n",ptAnz);
  // printf("           %f,%f\n",p1->x,p1->y);
  // printf("           %f,%f\n",p2->x,p2->y);


  // erster Pt
  cv[0] = *p1;


  if(ptAnz < 2) {
    ind = 0;
    goto L_fertig;
  }

  ianz = ptAnz - 1;
  dx = (p2->x - p1->x) / ianz;
  dy = (p2->y - p1->y) / ianz;

  ind  = 0;
  iend = ptAnz-2;


  L_nxt_pt:
  ++ind;

  cv[ind].x = p1->x + (dx * ind);
  cv[ind].y = p1->y + (dy * ind);

  if(ind < iend) goto L_nxt_pt;


  L_fertig:
  ++ind;
  cv[ind] = *p2;


/*
  // nur Testausg:
  printf("ex UT2D_cv_ln %d\n",ptAnz);
  for(ind=0; ind<ptAnz; ++ind) {
    printf("        %d %f,%f\n",ind,cv[ind].x,cv[ind].y);
  }
*/

  return 0;

}


//==========================================================================
  int UT2D_cv_ci_ (Point2 *pa, int *ptNr, Circ2 *ci1, int ptMax, double tol) {
//==========================================================================
///UT2D_cv_ci_               get circular polygon from circle

  int    pNr;


  // fix nr of points according to modelsize
  if (tol < 0.0) {
    pNr = ptMax;
  } else {
    pNr = UT2D_ptNr_ci (fabs(ci1->rad), fabs(ci1->ango), tol);
    if(pNr > ptMax)  pNr = ptMax;
  }
     // printf(" pNr=%d\n",pNr);


  // circ -> polygon
  UT2D_npt_ci (pa, pNr, ci1);


  *ptNr = pNr;

  return 0;

}
 

//=========================================================================
  int UT2D_cv_ci (Point2 cv[],int *ptanz,
                  Point2 *pa, Point2 *pe, Point2 *pm, int idiv) {
//=========================================================================
/// \code
/// UT2D_cv_ci                change circle > polygon
/// 
/// nur CCW. Max 64 Segments. Sehr schnell.
/// Erstes und letztes Segment haben unterschiedliche Groesse.
/// 
/// the circle is divided into 64/idiv segments.
/// \endcode

// idiv - divide nr of segments; 1 > max 64 segments, 2 > max 32 segments ..
//   idiv 1-4; 1=feines Polygon (64 Punkte f. Vollkreis), 2 groeber (32).
//   3 hat nur mehr 24 Ecken, 4 nur mehr 16.
// 
// max 10 vertices for 45 degrees; so a max of 65 vertices is possible.
// The first and the last segment have different sizes !


  static double sinwin[8] = { .0490677,.1467305,.2429802,.3368899,
            .4275551,.5141027,.5956993,.671559 };
  static double coswin[8] = { .9987955,.9891765,.9700313,.9415441,
            .9039893,.8577286,.8032075,.7409511 };

  static int astep = 6;
  static int ianz = 7;

  int       i1, i2, ia, ie, ipos, istep;
  double    wa, we, mx, my, crad, dx1[8], dy1[8];

  Vector2   va, ve;


// -------------------------------------------------------------
/*
  printf("UT2D_cv_ci %f,%f\n",pa->x,pa->y);
  printf("       %f,%f\n",pe->x,pe->y);
  printf("       %f,%f\n",pm->x,pm->y);
*/


  mx = pm->x;
  my = pm->y;

  if(idiv < 1) idiv = 1;
  if(idiv > 4) idiv = 4;

  /* idiv = 2; */

  istep = astep * idiv;
  /* TX_Print(" idiv=%d istep=%d",idiv,istep); */

  /* vectors to start - endpoint */
  UT2D_vc_2pt (&va, pm, pa);
  UT2D_vc_2pt (&ve, pm, pe);

  /* radius */
  crad = UT2D_len_vc (&va);
  /* angles */
  wa = UT2D_angd_angr (UT2D_angr_vc (&va));
  we = UT2D_angd_angr (UT2D_angr_vc (&ve));

  if (we <= wa) { we += 360.; }

  /* first seg min (istep/2)  max 1.5*istep */
  ia = (int) (wa + (istep / 2));
  ie = (int) (we - (istep / 2));


  /* copy 1. point */
  cv[0].x = pa->x;
  cv[0].y = pa->y;


  ipos = 0;


  /* 45 deg of cir */
  /* for (i1 = 0; i1 <= ianz; i1 += idiv) { */
  for (i1 = 0; i1 <= ianz; ++i1) {
        dx1[i1] = coswin[i1] * crad;
        dy1[i1] = sinwin[i1] * crad;
  }



  while (ie >= 0) {
    /* printf("  ia=%d, ie=%d\n",ia,ie); */


    /*  0-45  deg ------------------- */
    if ((ie >= 0) && (ia < 45)) {

      i1 = 0;
      for (i2 = 0; i2 < 45; i2 += istep) {
        if (ie < i2) { goto Done; }
        if (ia < i2) {
          ++ipos;
          cv[ipos].x = dx1[i1] + mx;
          cv[ipos].y = dy1[i1] + my;
        }
        i1 += idiv;;
      }
    }


    /* 45-90  deg ------------------- */
    if ((ie >= 45) && (ia < 90)) {

    i1 = ianz;
    for (i2 = 45; i2 < 90; i2 += istep) {
        if (ie < i2) { goto Done; }
        if (ia < i2) {
          ++ipos;
          cv[ipos].x = dy1[i1] + mx;
          cv[ipos].y = dx1[i1] + my;
        }
        i1 -= idiv;;
      }
    }


    /* 90-135  deg ------------------ */
    if ((ie >= 90) && (ia < 135)) {

    i1 = 0;
    for (i2 = 90; i2 < 135; i2 += istep) {
        if (ie < i2) { goto Done; }
        if (ia < i2) {
          ++ipos;
          cv[ipos].x = -dy1[i1] + mx;
          cv[ipos].y = dx1[i1] + my;
        }
        i1 += idiv;;
      }
    }


    /* 135-180  deg ------------------ */
    if ((ie >= 135) && (ia < 180)) {

    i1 = ianz;
    for (i2 = 135; i2 < 180; i2 += istep) {
        if (ie < i2) { goto Done; }
        if (ia < i2) {
          ++ipos;
          cv[ipos].x = -dx1[i1] + mx;
          cv[ipos].y = dy1[i1] + my;
        }
        i1 -= idiv;;
      }
    }


    /* 180-225  deg ------------------- */
    if ((ie >= 180) && (ia < 225)) {

    i1 = 0;
    for (i2 = 180; i2 < 225; i2 += istep) {
        if (ie < i2) { goto Done; }
        if (ia < i2) {
          ++ipos;
          cv[ipos].x = -dx1[i1] + mx;
          cv[ipos].y = -dy1[i1] + my;
        }
        i1 += idiv;;
      }
    }
    /* 225-270  deg ------------------- */
    if ((ie >= 225) && (ia < 270)) {

    i1 = ianz;
    for (i2 = 225; i2 < 270; i2 += istep) {
        if (ie < i2) { goto Done; }
        if (ia < i2) {
          ++ipos;
          cv[ipos].x = -dy1[i1] + mx;
          cv[ipos].y = -dx1[i1] + my;
        }
        i1 -= idiv;;
      }
    }


    /* 270-315  deg ------------------- */
    if ((ie >= 270) && (ia < 315)) {

    i1 = 0;
    for (i2 = 270; i2 < 315; i2 += istep) {
        if (ie < i2) { goto Done; }
        if (ia < i2) {
          ++ipos;
          cv[ipos].x = dy1[i1] + mx;
          cv[ipos].y = -dx1[i1] + my;
        }
        i1 += idiv;;
      }
    }


    /* 315-360  deg ------------------- */
    if ((ie >= 315) && (ia < 360)) {

    i1 = ianz;
    for (i2 = 315; i2 < 360; i2 += istep) {
        if (ie < i2) { goto Done; }
        if (ia < i2) {
          ++ipos;
          cv[ipos].x = dx1[i1] + mx;
          cv[ipos].y = -dy1[i1] + my;
        }
        i1 -= idiv;;
      }
    }


    /* Done -------------------- */
    Done:
    ia += -360;
    ie += -360;
  }

  /* copy last point */
  ++ipos;
  cv[ipos].x = pe->x;
  cv[ipos].y = pe->y;


  ++ipos;                   /*  Anzahl statt maxIndex */
  *ptanz = ipos;


/*
  // nur Testausgaben:
  for (i1=0; i1 < ipos; i1++) {
    TX_Print("%d %f,%f\n",i1,cv[i1].x,cv[i1].y);
  }
*/


  return 0;

}




/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/




//================================================================
  double UT2D_lenB_2pt (Point2 *p1, Point2 *p2) {
//================================================================
/// UT2D_lenB_2pt         longest dx/dy-distance point-point  (fast!)

  double dx, dy;


  dx = fabs(p2->x - p1->x);
  dy = fabs(p2->y - p1->y);

  if(dx > dy) return dx;
  else return dy;

}


//==============================================================================
  int UT2D_minLenB_4pt (double *dp,
                        Point2 *p1a,Point2 *p1e,Point2 *p2a,Point2 *p2e) {
//==============================================================================
/// \code
/// UT2D_minLenB_4pt          min lenght between 2 lines
/// dp ist Streckensumme entlang der Hauptachsen - nicht genau Laenge !
/// rc = 1  p1e - p2a hat geringsten Abstand
/// rc = 2  p1a - p2a hat geringsten Abstand
/// rc = 3  p1e - p2e hat geringsten Abstand
/// rc = 4  p1a - p2e hat geringsten Abstand
/// dx,dy addieren; den geringsten Wert auswaehlen.
/// \endcode


  int       ii;
  double    da[4];

  // UT3D_stru_dump(Typ_PT, p1a, "p1a:");
  // UT3D_stru_dump(Typ_PT, p1e, "p1e:");
  // UT3D_stru_dump(Typ_PT, p2a, "p2a:");
  // UT3D_stru_dump(Typ_PT, p2e, "p2e:");


  da[0] = fabs(p2a->x - p1e->x) + fabs(p2a->y - p1e->y);
  da[1] = fabs(p2a->x - p1a->x) + fabs(p2a->y - p1a->y);
  da[2] = fabs(p2e->x - p1e->x) + fabs(p2e->y - p1e->y);
  da[3] = fabs(p2e->x - p1a->x) + fabs(p2e->y - p1a->y);

  ii = UTP_min (4, da);

  *dp = da[ii];

  ++ii;

  // printf("ex UT3D_minLen_4pt %d %f %f %f %f %f\n",ii,*dp,
                             // da[0],da[1],da[2],da[3]);

  return ii;

}



//============================================================================
  int UT2D_pt_ck_int4pt (Point2 *pi1, Point2 *pi2, Point2 *pi3, Point2 *pi4) {
//============================================================================
/// \code
/// test ob die Linien p1-p2 und p3-p4 sich schneiden/beruehren/parallel sind
/// rc=-1: kein Schnittpunkt
/// rc=0:  die  Linien beruehren sich
/// rc=1:  die Linien sind (teilweise) parallel
/// rc=2:  die Linien schneiden sich
/// see UT2D_pt_int2vc2pt
/// \endcode


  int     irc, i1;
  double  xmax, xmin, ymax, ymin;
  double  q1, q2, qq, d0, d1, d2, dmin, dmax;
  Point2  p1, p2, p3, p4;
  Vector2 vc1, vc2, vcs;


  // printf("UT2D_pt_ck_int4pt\n");
  // UT3D_stru_dump(Typ_PT2, pi1," p1=");
  // UT3D_stru_dump(Typ_PT2, pi2," p2=");
  // UT3D_stru_dump(Typ_PT2, pi3," p3=");
  // UT3D_stru_dump(Typ_PT2, pi4," p4=");


  irc = 0;
  if(UT2D_comp4pt(pi1,pi2,pi3,pi4,UT_TOL_pt) > 0) goto L_fertig;


  UT2D_vc_2pt (&vc1, pi1, pi2);
  UT2D_vc_2pt (&vc2, pi3, pi4);
  UT2D_vc_2pt (&vcs, pi1, pi3);


  // UT3D_stru_dump(Typ_VC2, &vc1," vc1 ");
  // UT3D_stru_dump(Typ_VC2, &vc2," vc2 ");
  // UT3D_stru_dump(Typ_VC2, &vcs," vcs ");


  q1 = vc1.dy * vc2.dx;
  q2 = vc1.dx * vc2.dy;
  qq = q1 - q2;
  // printf(" qq=%f q1=%f q2=%f\n",qq,q1,q2);



  if (fabs(qq) > 0.0001) goto L_nicht_parall;
  // lines sind parallel. sind sie teilweise uebereinander ?
  // Ist vcs (p1-p3) != vc1, dann koennen sie nicht beruehren.
  // den Normalabstand von p1 auf vc2 errechnen
  UT2D_vc_setLength (&vc2, &vc2, 1.);
  d1 = vcs.dx * vc2.dy - vc2.dx * vcs.dy;
  // printf(" normalabst=%f\n",d1);
  if (fabs(d1) > UT_TOL_pt) goto L_m1;


  // linien sind parallel und auf der gleichen Linie.
  d0 = sqrt(vc1.dx*vc1.dx + vc1.dy*vc1.dy); // vc1

  d1 = sqrt(vcs.dx*vcs.dx + vcs.dy*vcs.dy); // p1-p3

  UT2D_vc_2pt (&vcs, pi1, pi4);
  d2 = sqrt(vcs.dx*vcs.dx + vcs.dy*vcs.dy); // p1-p4
  // printf(" d0=%f d1=%f d2=%f\n",d0,d1,d2);


  // test auf ganz draussen
  dmax = d0+UT_TOL_pt;
  dmin = -UT_TOL_pt;
  if((d1 > dmax)&&(d2 > dmax)) goto L_m1;   // rechts draussen
  if((d1 < dmin)&&(d2 < dmin)) goto L_m1;   // links draussen

  // test auf beruehren
  dmax = d0-UT_TOL_pt;
  dmin = UT_TOL_pt;
  if((d1 > dmax)&&(d2 > dmax)) goto L_fertig;   // rechts draussen
  if((d1 < dmin)&&(d2 < dmin)) goto L_fertig;   // links draussen

  // somit parallel ..
  irc = 1;
  goto L_fertig;




  //==================================================================
  L_nicht_parall:

  d1 = (vc2.dx*vcs.dy - vc2.dy*vcs.dx) / qq;
  d2 = (vc1.dx*vcs.dy - vc1.dy*vcs.dx) / qq;
  // printf(" d1=%f d2=%f\n",d1,d2);

  // d1 ist der Abstand des Schnittpunkts auf vc1; 0=p1, 1=p2.
  // d2 ist der Abstand des Schnittpunkts auf vc2; 0=p3, 1=p4.

  // wenn d1 > 0 && < 1 und d2 ebenfalls, dann ist es ein sauberer Schnitt.
  if((d1 < 0.)||(d1 > 1.)) goto L_t1;
  if((d2 < 0.)||(d2 > 1.)) goto L_t1;


  // ist sauberer schnitt.
  irc = 2;
  goto L_fertig;


  // schleifender Schnitt
  // alle Verbindungen testen; wenn ein Minimaldist. < Tol: Beruehrt(0);
  // else kein Schnittpunkt (-1).
  L_t1:
/*
  // ck maxAbst von p1 auf p3-p4
  if(fabs(UT2D_len_ptln(pi1,pi3,pi4)) < UT_TOL_pt) goto L_fertig;

  // ck maxAbst von p2 auf p3-p4
  if(fabs(UT2D_len_ptln(pi2,pi3,pi4)) < UT_TOL_pt) goto L_fertig;

  // ck maxAbst von p3 auf p1-p2
  if(fabs(UT2D_len_ptln(pi3,pi1,pi2)) < UT_TOL_pt) goto L_fertig;

  // ck maxAbst von p4 auf p1-p2
  if(fabs(UT2D_len_ptln(pi4,pi1,pi2)) < UT_TOL_pt) goto L_fertig;
*/

  // kein Punkt nah genug. Basta.


  //==========================================
  L_m1:
  irc = -1;

  L_fertig:
  // printf("ex UT2D_pt_ck_int4pt %d\n",irc);
  return irc;

}


//==================================================================
  int UT2D_i4pt_npt (int *ixMin, int *ixMax, int *iyMin, int *iyMax,
                     int pNr, Point2 *ptab) {
//==================================================================
/// \code
/// UT2D_i4pt_npt                   find indices of extreme-points;
///   (highest, lowest, leftmost, rightmost)
///
/// see also UT3D_ipt2_nptvc UT3D_box_pts
/// \endcode


  int      i1;
  Point2   p1, p2, *pmin, *pmax;


  // printf("UT2D_i4pt_npt %d\n",pNr);
  // for(i1=0;i1<pNr;++i1) UT3D_stru_dump (Typ_PT2,&ptab[i1],"P[%d]",i1);


  if (pNr < 1) {
    printf("UT2D_i4pt_npt: input error\n");
    return -1;
  }

  pmin = &p1;
  pmax = &p2;
  pmin->x = UT_VAL_MAX;
  pmin->y = UT_VAL_MAX;
  pmax->x = UT_VAL_MIN;
  pmax->y = UT_VAL_MIN;

  for (i1=0; i1<pNr; ++i1) {
    if (ptab[i1].x < pmin->x) {pmin->x = ptab[i1].x; *ixMin = i1;}
    if (ptab[i1].y < pmin->y) {pmin->y = ptab[i1].y; *iyMin = i1;}
    if (ptab[i1].x > pmax->x) {pmax->x = ptab[i1].x; *ixMax = i1;}
    if (ptab[i1].y > pmax->y) {pmax->y = ptab[i1].y; *iyMax = i1;}
  }

  // printf("ex UT2D_i4pt_npt %d %d %d %d %d\n",*ixMin,*ixMax,*iyMin,*iyMax,pNr);

  return 0;

}


//==================================================================
  double UT2D_ar_3pt (Point2 *p1, Point2 *p2, Point2 *p3) {
//==================================================================
/// \code
/// UT2D_ar_3pt            get (signed) area of triangle
///
/// see also UT2D_srar_inpt3 UFA_fac_srar
/// \endcode

  double   d1;


  d1  = p1->x * (p2->y - p3->y);
  d1 += p2->x * (p3->y - p1->y);
  d1 += p3->x * (p1->y - p2->y);

  // UT2D_srar_inpt3 (ar, 3, (int*)fc, pa);

  return d1 / 2.;

}


//======================================================================
  int UT2D_pt_ck_onLine (Point2 *po,
                         Point2 *p1,Point2 *p2,Point2 *p3,
                         double tol) {
//======================================================================
/// \code
/// liegt p3 auf der Linie p1-p2 ?      (Tests mit UT_TOL_pt)
/// Output:
///   po        nearest point on line; (NULL; not computed)
///   retCod
///        -1   NO; Point outside Line p1-p2
///         0   Yes; p3=p1; po=p1.
///         1   Yes; po between p1-p2
///         2   Yes; p3=p2; po=p2.
/// \endcode


  // int        irc;
  double     dn, dl, l1, hx, lx, hy, ly;
  Vector2    vc1, vcs;


  // test ob voellig ausserhalb ..
  // if(UT2D_pt_ck_inBoxTol(p1,p2,p3,tol) == 1) return -1;

  // hx = DMAX (p1->x, p2->x);
  // if(hx < p3->x - tol) return -1;
  if(DMAX (p1->x, p2->x) < p3->x - tol) return -1;

  // lx = DMIN (p1->x, p2->x);
  // if(lx > p3->x + tol) return -1;
  if(DMIN (p1->x, p2->x) > p3->x + tol) return -1;

  // hy = DMAX (p1->y, p2->y);
  // if(hy < p3->y - tol) return -1;
  if(DMAX (p1->y, p2->y) < p3->y - tol) return -1;

  // ly = DMIN (p1->y, p2->y);
  // if(ly > p3->y + tol) return -1;
  if(DMIN (p1->y, p2->y) > p3->y + tol) return -1;



  // p3 = p1 ??
  if(UT2D_comp2pt (p1, p3, tol) == 1) {
    if(po) *po = *p1;
    return 0;
  }


  // p3 = p2 ??
  if(UT2D_comp2pt (p2, p3, tol) == 1) {
    if(po) *po = *p2;
    return 2;
  }



  UT2D_vc_2pt (&vc1, p1, p2);
  UT2D_vc_2pt (&vcs, p1, p3);

  UT2D_vc_setLength (&vc1, &vc1, 1.);


  // den Normalabstand von p1-p2 errechnen
  dn = vcs.dx * vc1.dy - vc1.dx * vcs.dy;
  // printf(" dn=%f\n",dn);

  if(fabs(dn) > tol) return -1;


  // den Laengsabstand auf p1-p2 errechnen
  if(po) {
    dl = vcs.dx * vc1.dx + vc1.dy * vcs.dy;
    // printf(" dl=%f\n",dl);

    // Endpunkt errechnen
    // l1 = ll / dl;
    po->x = p1->x + vc1.dx * dl;
    po->y = p1->y + vc1.dy * dl;
  }

  // if(po) printf("ex UT2D_pt_ck_onLine 0 %f,%f\n",po->x,po->y);

  return 1;

}


//======================================================================
  int UT2D_pt_ck_inLine (Point2 *p1, Point2 *p2, Point2 *p3, double tol) {
//======================================================================
// UT2D_pt_ck_inLine      Author: Thomas Backmeister       3.6.2003
/// \code
/// UT2D_pt_ck_inLine      check 2D-point on line segment
/// 
/// 
/// Check if a 3. 2D-point lies inside the line segment defined by two 2D-points.
/// It is assumed that the 2D-point is ON the line defined by the two 2D-points.
/// 
/// IN:
///   Point2 p1   ... 2D-point 1 defining the line segment
///   Point2 p2   ... 2D-point 2 defining the line segment
///   Point2 p3   ... 2D-point to check
///   double tol  ... tolerance for 2D-point to ly inside line segment (>0)
/// OUT:
/// Returncodes:
///   0 = the 2D-point lies outside the segment
///   1 = the 2D-point lies inside the segment
/// \endcode


  int rc;
  double l1, l2, l3, lt;
  Vector2 v1, v2, v3;

  //printf("UT2D_pt_ck_inLine: check 2D-point on line segment\n");

  // init
  rc = 0;

  // vectors from 2D-point to 2D-point
  UT2D_vc_2pt (&v1, p1, p2);
  UT2D_vc_2pt (&v2, p1, p3);
  UT2D_vc_2pt (&v3, p2, p3);

  // square of length of vectors
  l1 = v1.dx * v1.dx + v1.dy * v1.dy;
  l2 = v2.dx * v2.dx + v2.dy * v2.dy;
  l3 = v3.dx * v3.dx + v3.dy * v3.dy;

  // check 2D-point position
  lt = l1 + tol;
  if (l2 <= lt && l3 <= lt) rc = 1;

  return rc;
}



//================================================================
  int UT2D_pt_ck_inplg (Point2 * pTab, int pNr, Point2 *ptx) {
//================================================================
/// \code
/// UT2D_pt_ck_inplg       Test if Point ptx is inside polygon pTab
/// Input:
///   pTab   polygonPoints; closing edge is from pTab[pNr-1] to pTab[0]
/// Output:
///   RetCod 0     No,  point ptx is outside pTab;
///          else  Yes, point is inside.
/// \endcode

// test polygon-segment only, if the y-value of the testpoint is between
// the y-values of the segment. Else ignore the segment.

//       \i1           \ i1
//       \             \
//   ignore\      ptx    \ --wn           segments going down
//          \             \
//           \i1+1         \i1+1
//
//       \i1+1         \ i1+1
//        \             \
//   ignore\      ptx    \ ++wn           segments going up
//          \             \
//           \i1           \i1

// Test:
//  Point2 ptx={15.,15.};
//  Point2 pa[]={{10.,10.},{20.,10.},{20.,20.},{10.,20.},{10.,10.}};
//  irc = UT2D_pt_ck_inplg (pa, 5, &ptx);


//  Point2 pt1={15.,5.};   // out
//  Point2 pt2={15.,15.};  // in
//  Point2 pa[]={{10.,10.},{20.,10.},{20.,20.},{10.,20.},{10.,10.}};
//  printf(" p1=%d\n",UT2D_pt_ck_inplg (pa, 5, &pt1));
//  printf(" p2=%d\n",UT2D_pt_ck_inplg (pa, 5, &pt2));

  int     i1;
  int     wn;               // windingNr


  //for(i1=0; i1<pNr; ++i1) UT3D_stru_dump (Typ_PT2, &pTab[i1], "pa[%d]=",i1);
  //UT3D_stru_dump (Typ_PT2, ptx, "ptx=");

  --pNr;       // plg must be closed; last_point == first_point
  wn = 0;

  // loop through all edges of the polygon
  for (i1=0; i1<pNr; ++i1) {          // edge from pTab[i1] to pTab[i1+1]
    //printf("------------ p[%d] wn=%d\n",i1,wn);

    if (pTab[i1].y <= ptx->y) {
      // plg-pt is below testPt
      // test if plgSegment goes higher than testPt
      if (pTab[i1+1].y > ptx->y) {
        // test if point is left of segment; Yes: incr wn.
        if (UT2D_sid_3pt (ptx, &pTab[i1], &pTab[i1+1]) > 0) ++wn;
      }

    } else {
      // plgPt is above testPt
      // test if plgSegment goes lower than testPt
      if (pTab[i1+1].y <= ptx->y) {
        // yes, ptx.y is between Y-zone of segment.
        // if P is right of seg: --wn.
        // test if point is right of segment; Yes: incr wn.
        if (UT2D_sid_3pt (ptx, &pTab[i1], &pTab[i1+1]) < 0) --wn;
      }
    }
  }

  return wn;

}


//======================================================================
  int UT2D_pt_ck_linear (int np, Point2 *ptab, double tol) {
//======================================================================
// UT2D_pt_ck_linear       Author: Thomas Backmeister       3.6.2003
/// \code
/// UT2D_pt_ck_linear       check straight position of 2D-points
/// 
/// Check if the position of 2D-points is straight.
/// IN:
///   int np        ... number of 2D-points
///   Point2 *ptab  ... 2D-points
///   double tol    ... tolerance for straightness (>0)
/// OUT:
/// Returncodes:
///   0 = position of points is not straight
///   1 = position of points is straight
/// \endcode

  int rc, i1, i2, np1;
  double pn, pnmax;
  Point2 *ptab1;

  //printf("UT2D_pt_ck_linear: check straight position of 2D-points\n");

  // check input
  if (np < 3) return 1;

  // allocate memory
  ptab1 = (Point2*) malloc (np * sizeof(Point2));

  // copy points
  for (i1=0; i1<np; ++i1) {
    ptab1[i1] = ptab[i1];
  }

  // second forward differences of points
  np1 = np;
  for (i1=0; i1<2; ++i1) {
    --np1;
    for (i2=0; i2<np1; ++i2) {
      ptab1[i2].x = ptab1[i2+1].x - ptab1[i2].x;
      ptab1[i2].y = ptab1[i2+1].y - ptab1[i2].y;
    }
  }


  // measure the straightness
  pnmax = 0.0;
  for (i1=0; i1<np1; ++i1) {
    pn = sqrt (ptab1[i1].x * ptab1[i1].x + ptab1[i1].y * ptab1[i1].y);
    if (pn > pnmax) pnmax = pn;
  }
  if (np1 * (np1+1) * pnmax > tol) rc = 0;
  else rc = 1;

  // free memory
  if (ptab1 != NULL) free (ptab1);

  return rc;
}



/*
//=========================================================================
  int UT2D_pt_ck_in3pt (Point2 *px,  Point2 *po, Point2 *p1,  Point2 *p2) {
//=========================================================================
// DO NOT USE-Fehlerhaft !!!!!!!! USE UT2D_ptvc_ck_int2pt
// UT2D_pt_ck_in3pt    check if px is between lines po-p1, po-p2.
// -2   px is outside and opposite po-v1, pto-v2
// -1   px is outside po-v1, po-v2
//  0   px is between po-v1, po-v2
//  1   px is on po-v1
//  2   px is on po-v2
//  3   px == po

  Vector2  v1, v2;


  UT2D_vc_2pt (&v1, po, p1);
  UT2D_vc_2pt (&v2, po, p2);
    // UT3D_stru_dump (Typ_VC2, &v1, " v1");
    // UT3D_stru_dump (Typ_VC2, &v2, " v2");


  return UT2D_pt_ck_inpt2vc (px, po, &v1, &v2);

}
*/

//==============================================================================
  int UT2D_pt_ck_inpt2vc (Point2 *px,  Point2 *po, Vector2 *v1, Vector2 *v2) {
//==============================================================================
/// \code
/// UT2D_pt_ck_inpt2vc        check if px is between vectors po-v1, po-v2.
/// po = common point.  OpeningAngle v1-v2 must have < 180 deg.
/// -2   openingAngle is 0 or 180 deg;
/// -1   px is outside po-v1, po-v2
///  0   px is between po-v1, po-v2
///  1   px is on po-v1
///  2   px is on po-v2
///  3   px == po
///
///                         /
///                      v2/     px (inside)
///       (outside)       /
///                      /
///                  po +----------
///       (outside)             v1
///
/// see also UT2D_sid_3pt UT2D_sid_ptvc UT2D_sidPerp_2vc
/// \endcode


  int      irc, i1, i2;
  double   sk, d1, d2;
  Vector2  vp;


  // UT3D_stru_dump (Typ_VC2, v1, " v1");
  // UT3D_stru_dump (Typ_VC2, v2, " v2");


  // test for po = px
  if(UT2D_comp2pt(po, px, UT_TOL_pt) == 1) {irc = 3; goto L_exit;}


  UT2D_vc_2pt (&vp, po, px);
    // UT3D_stru_dump (Typ_VC2, &vp, " vp");


  // baseline po-v1: check if px is on the same side as v2
  d1 = v2->dx * v1->dy - v1->dx * v2->dy;   // side of v2 against v1
  d2 = vp.dx * v1->dy - v1->dx * vp.dy;     // side of px against v1

  if (fabs(d1) < UT_TOL_min1) i1 =  0;
  else if (d1 < 0.)           i1 =  1;
  else                        i1 = -1;
  if(i1 == 0) {irc = -2; goto L_exit;}      // openingAngle is 0 or 180 deg;

  if (fabs(d2) < UT_TOL_min1) i2 =  0;
  else if (d2 < 0.)           i2 =  1;
  else                        i2 = -1;
  if(i2 == 0) {irc = 1; goto L_exit;}       // px lies on v1
    // printf(" i1=%d i2=%d \n",i1,i2);

  // different sides = outside.
  if(i1 != i2) {irc = -1; goto L_exit;}     // ouside


  // baseline po-v2: check if px is on the same side as v1
  d1 = v1->dx * v2->dy - v2->dx * v1->dy;   // side of v1 against v2
  d2 = vp.dx * v2->dy - v2->dx * vp.dy;     // side of px against v2

  if (fabs(d1) < UT_TOL_min1) i1 =  0;
  else if (d1 < 0.)           i1 =  1;
  else                        i1 = -1;
  if(i1 == 0) {irc = -2; goto L_exit;}      // openingAngle is 0 or 180 deg;

  if (fabs(d2) < UT_TOL_min1) i2 =  0;
  else if (d2 < 0.)           i2 =  1;
  else                        i2 = -1;
  if(i2 == 0) {irc = 2; goto L_exit;}      // px lies on v2
    // printf(" i1=%d i2=%d \n",i1,i2);

  // different sides = outside.
  if(i1 != i2) {irc = -1; goto L_exit;}    // ouside

  irc = 0;


  L_exit:
    // printf("ex UT2D_pt_ck_inpt2vc %d %d %d\n",irc,i1,i2);
  return irc;

}


//================================================================
  int UT2D_pt_ck_inAc (Point2 *pt, Circ2 *ci) {
//================================================================
/// \code
/// UT2D_pt_ck_inAc          check if pt is in Arc(Segment)
/// Vektor Cen-Pt muss Arc schneiden; else not in Arc.
/// Es wird der Radius nicht kontrolliert; auch nicht die gegenUeberseite.
/// RetCod: 0 = yes, pt is in Arcsegment;
///        -1,-2,1,2 = no.
/// \endcode


  int      irc, irot;
  double   aa, ae, ap;
  Vector2  vc1;

  // aa = angle cen-StartPtAc
  UT2D_vc_2pt (&vc1, &ci->pc, &ci->p1);
  aa = UT2D_angr_vc (&vc1);

  // ap = angle cen-pt
  UT2D_vc_2pt (&vc1, &ci->pc, pt);
  ap = UT2D_angr_vc (&vc1);

  // check if ap is between aa & ae
  irot = UT2D_irot_r (ci->rad);
  UT2D_2angr_set (&aa, &ap, irot);

  ae = aa + ci->ango;
    // printf(" aa=%f ae=%f ap=%f\n",aa,ae,ap);


  return UTP_db_ck_in2db (ap, aa,ae);
}


/*
//================================================================
  int UT2D_pt_ck_inTriangle (Point2 *p1, Point2 *p2, Point2 *p3, Point2 *p) {
//================================================================
// UT2D_pt_ck_inTriangle        check if point is inside triangle

// UT2D_pt_ck_inTriangle        Author: Thomas Backmeister       27.8.2003
neuere Version UT2D_ck_pt_in_tria von Sauer/Reiter ..

// Check if a point is inside a triangle.

//IN:
//  Point2 *p1      ... 1. point of triangle
//  Point2 *p2      ... 2. point of triangle
//  Point2 *p3      ... 3. point of triangle
//  Point2 *p       ... point to be checked
//OUT:
//Returncodes:
//  0 = YES = point inside triangle
//  1 = NO  = point outside triangle


  int    or1, or2, or3;
  double d1, d2;

  // printf("UT2D_pt_ck_inTriangle: check point inside triangle\n");

  or1 = UT2D_orient_3pt (p1, p2, p); // 1=CW; 0=CCW; -1=colli.
  // printf(" or1=%d\n",or1);
  if(or1 < 0) {
    if(UT2D_pt_ck_inBox(p1, p2, p) == 0) return YES; // 1=equ
    return NO;
  }
    // d1 = UT2D_len_2pt (p1, p2);
    // d2 = UT2D_len_2pt (p1, p);
    // if((d2 < d2)&&(d2 > 0.)) or1 = 1;
    // else or1 = 0;
  // }

  or2 = UT2D_orient_3pt (p2, p3, p);
  // printf(" or2=%d\n",or2);
  if(or2 < 0) {
    if(UT2D_pt_ck_inBox(p2, p3, p) == 0) return YES; // 1=equ
    return NO;
  }
    // d1 = UT2D_len_2pt (p2, p3);
    // d2 = UT2D_len_2pt (p2, p);
    // if((d2 < d2)&&(d2 > 0.)) or2 = 1;
    // else or2 = 0;
  // }

  if(or1 != or2) return NO; // RF


  or3 = UT2D_orient_3pt (p3, p1, p);
  // printf(" or3=%d\n",or3);
  if(or3 < 0) {
    if(UT2D_pt_ck_inBox(p3, p1, p) == 0) return YES; // 1=equ
    return NO;
  }
    // d1 = UT2D_len_2pt (p3, p1);
    // d2 = UT2D_len_2pt (p3, p);
    // if((d2 < d2)&&(d2 > 0.)) or3 = 1;
    // else or3 = 0;
  // }

  // printf("UT2D_pt_ck_inTriangle %d %d %d\n",or1,or2,or3);

  if((or1 == or2)&&(or2 == or3)) return YES;

  return NO;
}
*/


//================================================================
  int UT2D_pt_ck_inCv3 (Point *ptx, int pNr, Point *pTab) {
//================================================================
/// \code
/// UT2D_pt_ck_inCv3              check if point is inside polygon
/// geschlossene Kontur erforderlich; pTab[pNr-1] muss gleich wie pTab[0] sein
/// irc  1   Punkt liegt innerhalb der Kontur
/// irc  0   Punkt liegt auf der Kontur
/// irc -1   Punkt liegt ausserhalb der Kontur
/// 
/// Schnittpunkte aller Kantenlinien mit Horzontale Linie durch ptx
/// rechts von ptx zaehlen; ungerade == innen.
/// \endcode

// see also /mnt/serv1/Devel/dev/math/unused/pip.c

  int    i1, i2, ii, in, ie, ix;
  double xx, xh, xl, yh, yl, d1, d2;


  // UT3D_stru_dump (Typ_PT, ptx, "UT2D_pt_ck_inCv3");
  // printf(" pNr=%d\n",pNr);

  xh = ptx->x + UT_TOL_pt;  // hi
  xl = ptx->x - UT_TOL_pt;  // lo

  yh = ptx->y + UT_TOL_pt;  // hi
  yl = ptx->y - UT_TOL_pt;  // lo

  ie = pNr - 1;
  in = 0;
  ix = 0;                    // Anz. Schnittpunkte rechts


  for(ii=0; ii<ie; ++ii) {

    ++in;

    // liegt Punkt rechts vom vertikalen Bereich der Kante: skip
    if((pTab[ii].x < xl)&&(pTab[in].x < xl)) continue;


    // liegt der Punkt im horizontalen Bereich der Kante -
    if((pTab[ii].y > yh)&&(pTab[in].y > yh)) continue;
    if((pTab[ii].y < yl)&&(pTab[in].y < yl)) continue;

    // ja; Kante ist im hor Bereich des Punktes und links !!
    // printf("  %d-%d bereich OK\n",ii,in);


    // ist Kante horizontale Kante -
    if(fabs(pTab[ii].y - pTab[in].y) < UT_TOL_pt) {
      // Kante ist hor. Linie;
      // printf(" %d-%d ist hor\n",ii,in);
      // wenn pt innerhalb Kante: on Kontur; else skip Kante
      if((pTab[ii].x > xh)&&(pTab[in].x > xh)) continue;
      if((pTab[ii].x < xl)&&(pTab[in].x < xl)) continue;
      goto L_exit_on;
    }


    // ist ptx ident mit dem Polygonpunkt: on Kontur.
    if(pTab[ii].x > xh) goto L_1;
    if(pTab[ii].x < xl) goto L_1;
    if(pTab[ii].y > yh) goto L_1;
    if(pTab[ii].y < yl) goto L_1;
    goto L_exit_on;


    L_1:
    // Schnittpunkt mit Horiz.Linie berechnen
    xx = pTab[ii].x + ((ptx->y - pTab[ii].y) / (pTab[in].y - pTab[ii].y) *
                       (pTab[in].x -pTab[ii].x));


    if(xx < xl) continue;   // skip Schnittpunkte die rechts der Kontur liegen

    // Schnittpunkt ist nun xx,ptx->y
    // printf(" %d-%d xx=%f,%f\n",ii,in,xx,ptx->y);


    // entspricht Schnittpunkt dem KantenEndpunkt -
    if((pTab[in].y < yh)&&(pTab[in].y > yl)) {
      // Schnittpunkt == KantenEndpunkt; skip immer
      // printf(" skip Endpt\n");
      continue;
    }


    // entspricht Schnittpunkt dem KantenStartpunkt -
    if(pTab[ii].y > yh) goto L_3;
    if(pTab[ii].y < yl) goto L_3;
    // wenn Kontur auf der gleichen Seite ("Zacke") bleibt, skip Schnittpunkt
    i2 = in;
    d1 = pTab[in].y - pTab[ii].y;
    L_2:
    ++i2;
    if(i2 > ie) i2 = 0;  // Uebernaechster Punkt
    d2 = pTab[in].y - pTab[i2].y;
    if(fabs(d2) < UT_TOL_pt) goto L_2;  // skip hor Kante
    i1 = (d1 >= 0.) ? 1 : -1;
    i2 = (d2 >= 0.) ? 1 : -1;
    if(i1 == i2) {
      // printf(" i1=%d i2=%d d1=%f d2=%f\n",i1,i2,d1,d2);
      continue;
    }

    // Schnittpunkt mit Konturlinie.
    L_3:
    ++ix;
    // printf(" ix=%d\n",ix);
  }

  // ungerade Anzahl Schnittpunkte: Punkt ist innen.
  if((ix % 2) == 1) goto L_exit_in;



  //----------------------------------------------------------------
  L_exit_out:        // Punkt liegt ausserhalb der Kontur
  // printf("ex UT2D_pt_ck_inCv3 -1 (out)\n");
  return -1;

  L_exit_on:         // Punkt liegt auf der Kontur
  // printf("ex UT2D_pt_ck_inCv3 0 (on)\n");
  return 0;

  L_exit_in:         // Punkt liegt innerhalb der Kontur
  // printf("ex UT2D_pt_ck_inCv3 1 (in)\n");
  return 1;

}


//========================================================================
  int UT2D_pt_cknear_npt (Point2 *p0, Point2 *ptTab, int ptNr) {
//========================================================================
/// \code
/// UT2D_pt_cknear_npt        return index of nearest Point from n points
/// 
/// use: pt1 = ptTab[UT2D_ipt_cknear_npt (&ptStart, ptTab, ptNr)];
/// \endcode



  int     i1, iind;
  double  d1, lmax;


  printf("UT2D_ipt_cknear_npt %f %f\n",p0->x,p0->y);



  if(ptNr < 2) return 0;

  lmax = UT_VAL_MAX;

  for(i1=0; i1<ptNr; ++i1) {
    printf(" near[%d] %f %f\n",i1,ptTab[i1].x,ptTab[i1].y);
    d1 = UT2D_len_2pt (p0, &ptTab[i1]);
    if(d1 < lmax) {
      iind = i1;
      lmax = d1;
    }
  }

  return iind;

}



//=======================================================================
  int UT2D_ci_ptvcci (Point2 *pc2, Point2 *pi, double *r2,
                      Point2 *pt, Vector2 *vct,
                      Point2 *pc1, double r1) {
//=======================================================================
/// \code
/// UT2D_ci_ptvcci            durch Punkt+Richtung, tangential an Circ (pc+r)
/// compute circle. Inputs:
///          pt = point on circ,
///          vct = tangent to circ tru point pt
///          touching circle (center=pc1, radius=r1)
///          *r2 > 0: Solution 1
///          *r2 < 0: Solution 2
/// 
/// Input:
///   pt    definiert mit vct einen Durchgangspunkt mit Tangente
///   vct   zeigt in Richtung pc1 (fuer oben/unten-Betrachtung)
///   r2    1:  oberer Kreis,
///         -1: unterer Kreis gewuenscht.
/// Out:
///   pc2   Mittelpunkt
///   pi    der gemeinsame Punkt
///   r2    Radius ist positiv, wenn gegenlaeufig,
///         Radius negativ wenn gleichlaeufig.
/// Returncode:
///   0     OK, Aussenkreis
///   1     OK, Innenkreis (ACHTUNG: oben/unten vertauscht)
///  -1     Radius unendlich = eine Line von pi - pt.
///  -2     pt liegt am Kreis pc1/r1; Loesung unmoeglich
/// \endcode

 
// Kreis errechnen, von dem ein Punkt und die Tangente in diesem Punkt (pt, vct)
// gegeben ist und der einen Kreis (pc1, r1) tangiert.
// 
// Da gibts 2 Loesungen; einen oberen Kreis und einen unteren.
// (Input via r2 !)
// pt wird in 0,0 betrachtet, vct auf 1,0; der Kreis pc1 liegt rechts vom pt.
// 
// ACHTUNG: wenn der gesuchte Kreis c2 sich innerhalb von c1 befindet,
// wird oben/unten fuer den ausgegeb. Kreis vertauscht;
// fuer Approximate so erforderlich !!
// 



  int     irc, side_c1, typ_c2, typ_dreh, typ_halb;
  double  y, x, rad2, d1;
  Vector2 vc2;
  Point2  p1;


  r1 = fabs(r1);


  if(*r2 > 0.) {
   typ_c2 = 1;             // oberer Kreis
  } else {
   typ_c2 = -1;            // unterer Kreis
  }


/*
  printf("UT2D_ci_ptvcci r2=%f\n",*r2);
  printf("     pt=%f,%f\n",pt->x,pt->y);
  printf("     vt=%f,%f\n",vct->dx,vct->dy);
  printf("     c1=%f,%f r1=%f r2=%f\n",pc1->x,pc1->y,r1,*r2);
  printf("     typ_c2=%d\n",typ_c2);
*/



  // Liegt pt innerhalb von Kreis pc1/r1 ?
  d1 = UT2D_len_2pt (pt, pc1) - r1;

  
  // liegt pt am Kreis: Loesung unmoeglich
  if(fabs(d1) < UT_TOL_pt) { irc = -2; goto L_fertig; }



  // p1 = pc1 auf Linie pt/vct projiziert
  UT2D_pt_projptptvc (&p1, pc1, pt, vct);
  // printf(" p1=%f,%f\n",p1.x,p1.y);



  // y=Normalabstand von pc1 auf vct
  // Abst (m. Vorzeichen) von pc1 in Bezug auf pt-vct holen
  // Neg: pc1 liegt unter pt-vct
  y = UT2D_slen_nor_2pt_vc__ (pc1,  pt, vct);

  if(y > 0.) {
    side_c1 = 1;         // pc1 oberhalb pt-vct
  } else {
    side_c1 = -1;        // pc1 unterhalb pt-vct
    y = fabs(y);
  }
  // printf(" y=%f side_c1=%d\n",y,side_c1);




  // x ist der Abst von pt -> p1
  x = UT2D_len_2pt (pt, &p1);
  // printf(" x=%f y=%f\n",x,y);



  // C2 innerhalb oder ausserhalb von C1 ?
  if(d1 > 0.) goto L_outside;









  //=======================================================================
  // pt innerhalb von c1

  typ_c2 = -typ_c2;




  //---------------- C1 oben --------------------------------
  if(side_c1 < 0) goto L_in_C1_unten;


  if(typ_c2 > 0)  {              //----- oben --------
    // (y-r2)**2 + x**2 = (r1-r2)**2
    // r2 = (x**2 + y**2 - r1**2) / 2.0 * (y - r1)
    rad2 = (x*x + y*y - r1*r1) / (2.0 * (y - r1));
    // printf(" IN oben oben rad2=%f\n",rad2);
    d1= rad2;  // pc2 = Richtung p1 ->pc1 


  } else {                      //----- unten --------
    // (y+r2)**2 + x**2 = (r1-r2)**2
    // r2 = -(x**2 + y**2 - r1**2) / 2.0 * (y + r1)
    rad2 = -(x*x + y*y - r1*r1) / (2.0 * (y + r1));
    // printf(" IN oben unten rad2=%f\n",rad2);
    d1= -rad2;  // pc2 = Richtung p1 ->pc1 


  }

  goto L_in_weiter;




  //---------------- C1 unten --------------------------------
  L_in_C1_unten:


  if(typ_c2 > 0)  {              //----- oben --------
    // (y+r2)**2 + x**2 = (r1-r2)**2
    // r2 = -(x**2 + y**2 - r1**2) / 2.0 * (y + r1)
    rad2 = -(x*x + y*y - r1*r1) / (2.0 * (y + r1));
    // printf(" IN unten oben rad2=%f\n",rad2);
    d1= -rad2;  // pc2 = Richtung p1 ->pc1 


  } else {                      //----- unten --------
    // (y-r2)**2 + x**2 = (r1-r2)**2
    // r2 = (x**2 + y**2 - r1**2) / 2.0 * (y - r1)
    rad2 = (x*x + y*y - r1*r1) / (2.0 * (y - r1));
    // printf(" IN unten unten rad2=%f\n",rad2);
    d1= rad2;  // pc2 = Richtung p1 ->pc1 


  }



  //-------------------------------------------------------------
  L_in_weiter:


  // pc2 = rad2 ab pt in Richtg p1-pc1
  UT2D_pt_tra3ptlen (pc2, pt, &p1, pc1, d1);



  // pi = r1 ab pc1 in Richtg pc2
  UT2D_pt_tra2ptlen (pi, pc1, pc2, r1);


  // Umdrehungssinn von c2 noch fixieren.
  // c2.rad ist positiv, wenn gegenlaeufig, negativ wenn gleichlaeufig.
  // Loesung immer gleichlaeufig.
  *r2 = -rad2;


  irc=1;
  goto L_fertig;






  //=======================================================================
  // pt liegt ausserhalb vom Kreis.
  L_outside:



  // ist das ganze eine Gerade ?
  d1 =  (y - r1);
  // printf(" d1=%f\n",d1);

  if(fabs(d1) < UT_TOL_min1) {
    irc = -1;
    *pi = p1;
    goto L_fertig;
  } else {
    irc=0;
  }





  // vc2 = eine Normale an vct (auf Linie pt-vc2 liegt pc2) - nach oben
  UT2D_vc_perpvc (&vc2, vct);


  // typ_dreh=1: gegenlaeufig;  -1=gleichlaeufig.
  typ_dreh = 1;


  // typ_halb=1, wenn der Kreis1 die pt-vce schneidet; sonst =0.
  if(y < r1) {
    typ_halb = 1;
  } else {
    typ_halb = 0;
  }




  //---------------- C1 oben --------------------------------
  if(side_c1 < 0) goto L_out_C1_unten;


  if(typ_c2 > 0)  {              //----- oben --------
    // (r2-y)**2 + x**2 = (r1+r2)**2
    // r2 = (x**2 + y**2 - r1**2) / 2.0 * (y + r1)
    rad2 = (x*x + y*y - r1*r1) / (2.0 * (y + r1));
    // printf(" oben oben rad2=%f\n",rad2);
    d1= r1;  // pi = Richtung c1 -> c2
    // if(typ_halb == 0) {
    // }


  } else {                      //----- unten --------
    // (r2-y)**2 + x**2 = (r2-r1)**2
    // r2 = (x**2 + y**2 - r1**2) / 2.0 * (y - r1)
    rad2 = (x*x + y*y - r1*r1) / (2.0 * (y - r1));
    // printf(" oben unten rad2=%f\n",rad2);
    d1= -r1;  // pi = Richtung c1 -> c2
    // typ_halb=1, wenn der Kreis1 die pt-vce schneidet; sonst =0.
    if(typ_halb == 0) {
      typ_dreh = -1; // typ_dreh = -1 = gleichlaeufig.
    } else {
      d1= r1;  // pi = Richtung c1 -> c2
    }


  }



  goto L_out_weiter;






  //---------------- C1 unten --------------------------------
  L_out_C1_unten:

  // vc2 = Normale an vct - nach unten !
  UT2D_vc_invert (&vc2, &vc2);



  if(typ_c2 > 0)  {              //----- oben --------
    // (r2-y)**2 + x**2 = (r2-r1)**2
    // r2 = (x**2 + y**2 - r1**2) / 2.0 * (y - r1)
    rad2 = (x*x + y*y - r1*r1) / (2.0 * (y - r1));
    // printf(" unten oben rad2=%f\n",rad2);
    d1= -r1;  // pi = Richtung c1 -> c2
    // typ_halb=1, wenn der Kreis1 die pt-vce schneidet; sonst =0.
    if(typ_halb == 0) {
      typ_dreh = -1; // typ_dreh = -1 = gleichlaeufig.
    } else {
      d1= r1;  // pi = Richtung c1 -> c2
    }


  } else {                      //----- unten --------
    // (r2-y)**2 + x**2 = (r1+r2)**2
    // r2 = (x**2 + y**2 - r1**2) / 2.0 * (y + r1)
    rad2 = (x*x + y*y - r1*r1) / (2.0 * (y + r1));
    // printf(" unten unten rad2=%f\n",rad2);
    d1= r1;  // pi = Richtung c1 -> c2


  }



  //-------------------------------------------------------------
  L_out_weiter:


  // pc2 = rad2 ab pt in Richtg vc2
  UT2D_pt_traptvclen (pc2, pt, &vc2, rad2);


  // Schnittpunkt pi:
  // printf("  d1=%f\n",d1);
  UT2D_pt_tra2ptlen (pi, pc1, pc2, d1);







  //----------------------------------------------------------------
  // Umdrehungssinn von c2 noch fixieren; ist genau entgegen dem c1.
  // c2.rad ist positiv, wenn gegenlaeufig, negativ wenn gleichlaeufig.
  // typ_dreh=1: gegenlaeufig;  -1=gleichlaeufig.

  if(typ_dreh > 0) {
    *r2 = fabs(rad2);

  } else {
    *r2 = -fabs(rad2);
  }




  L_fertig:
/*
  printf("ex UT2D_ci_ptvcci rc=%d  r2=%f\n",irc,*r2);
  printf("     pc2=%f,%f\n",pc2->x,pc2->y);
  printf("     pi=%f,%f\n",pi->x,pi->y);
*/


  return irc;

}


//=======================================================================
  int UT2D_ci_ptvcptvc (Point2 *pc, Point2 *p2, double *crad,
                        Point2 *p1, Vector2 *v1, Point2 *pt, Vector2 *vt) {
//=======================================================================
/// \code
///  Kreismittelpunkt jenes Kreises errechnen, der 
/// - durch Punkt p1 geht und in diesem Punkt Richtung vc1 hat und
/// - tangential an die Linie pt - vct ist.
/// 
/// Input:
///   p1
///   vc1
///   pt
///   vct   Richtung muss mit vc1 uebereinstimmen !
/// 
/// Output:
///   pc  Mittelpkt
///   p2  an diesem Punkt beruehrt der Kreis pt - vct
///   rad
/// 
/// Returncode:
///   0  Kreisbogen < 180 Grad
///  -1  Kreisbogen > 180 Grad
/// \endcode


  // int     i1;
  double  len_p1_pi;
  Vector2 vc1, vct;
  Point2  pi;




  UT2D_vc_setLength (&vc1, v1, 1.);
  UT2D_vc_setLength (&vct, vt, 1.);

  
/*
  printf("UT2D_ci_ptvcptvc\n");
  printf("    p1=%f,%f vc1=%f,%f\n",p1->x,p1->y,vc1.dx,vc1.dy);
  printf("    pt=%f,%f vct=%f,%f\n",pt->x,pt->y,vct.dx,vct.dy);
*/


  // pi = Schnitt vc1 - vct
  UT2D_pt_int2pt2vc (&pi, p1, &vc1, pt, &vct);
  // printf(" pi =%f,%f\n",pi.x,pi.y);


  // len_p1_pi = Abstand p1 - pi
  len_p1_pi = UT2D_len_2pt (p1, &pi);


  // Von pi weg len_p1_pi abschlagen in Richtg. vct ergibt p2
  UT2D_pt_traptvclen (p2, &pi, &vct, len_p1_pi);


  // Mittelpunkt via UT2D_ci_ptvcp
  UT2D_ci_ptvcpt (pc, crad, p1, &vc1, p2);


  // am vc1 muss pi vor p1 sein, sonst wirds ein Kreis > 180 Grad
  if(UT2D_sidPerp_ptvc (&pi, p1, &vc1) != 1) {
    printf("ex UT2D_ci_ptvcptvc  > 180 Grad\n");
    return -1;
  }


  return 0;

}


//=======================================================================
  int UT2D_vc_cipt (Vector2 *vc1, Point2 *pc, Point2 *pt, int cdir) {
//=======================================================================
/// den TangentialVektor an einem Punkt pt des Kreises errechnen


  UT2D_vc_2pt (vc1, pc, pt);

  UT2D_vc_perpvc (vc1, vc1);

  if(cdir < 0) UT2D_vc_invert (vc1, vc1);

  // printf("ex UT2D_vc_cipt %f,%f\n",vc1->dx,vc1->dy);

  return 0;

}


//================================================================
  int UT2D_cic_ci3 (Circ2C *ci2c, Plane *pl1, Circ *ci3) {
//================================================================
/// \code
/// UT2D_cic_ci3        get 2D-circle in centerPosition from 3D-circ
/// returns also its basPlne (use its vx, vy to transfer 2D-points -> 3D).
///
/// Example:
/// UT2D_cic_ci3               get 2D-circ and its plane from 3D-circ
/// UT2D_pt_cic_par            get point on 2D-circ
/// UT3D_pt_tra_pt_2vc1_2par   get 3D-point from 2D-point.
/// \endcode       
                   
                   
  // UT3D_stru_dump (Typ_CI, ci3, "UT2D_ci2c_ci3: ");
  
  
  // get basePlane of Circ; vx = pc-p1; vz = circ-vz.
  UT3D_pl_ci (pl1, ci3);
  
  // set radius, startAngle, openingAngle
  ci2c->rad  = ci3->rad;
  ci2c->angs = 0.;
  ci2c->ango = ci3->ango;

  return 0;

}


//================================================================
  int UT2D_ci_ci3 (Circ2 *ci2, Circ *ci3) {
//================================================================
/// UT2D_ci_ci3                  2D-Circ = 3D-Circ



  ci2->p1.x = ci3->p1.x;
  ci2->p1.y = ci3->p1.y;

  ci2->p2.x = ci3->p2.x;
  ci2->p2.y = ci3->p2.y;

  ci2->pc.x = ci3->pc.x;
  ci2->pc.y = ci3->pc.y;

  // CW ?
  if(ci3->vz.dz > 0.) {
    ci2->rad  = ci3->rad;
    ci2->ango = ci3->ango;
  } else {
    ci2->rad  = -ci3->rad;
    ci2->ango = -ci3->ango;
  }

  return 0;

}


//======================================================================
int UT2D_ci_ptrd (Circ2 *ci, Point2 *ptc, double rdc) {
//======================================================================
/// UT2D_ci_ptrd           360-deg 2D-Circ from center, radius

  ci->pc = *ptc;

  ci->p1 = *ptc;
  ci->p1.x += fabs(rdc);

  ci->p2 = ci->p1;

  ci->rad = rdc;
  ci->ango = RAD_360;

  return 0;

}


//================================================================
  int UT2D_ci_pt2vcrd (Circ2 *ci2, Point2 *pc,
                       Vector2 *vc1, Vector2 *vc2, double *rdc) {
//================================================================
// UT2D_ci_pt2vcrd           circ from center, 2 vectors & radius

  double rdca;


  rdca = fabs(*rdc);

  UT2D_pt_traptvclen (&ci2->p1, pc, vc1, rdca);
  UT2D_pt_traptvclen (&ci2->p2, pc, vc2, rdca);


  // create circ  UT2D_ci_3ptrd
  ci2->pc  = *pc;
  ci2->rad = *rdc;
  ci2->ango = UT2D_angr_ci (&ci2->p1, &ci2->p2, pc, *rdc);



  return 0;

}


//=======================================================================
  int UT2D_ci_ptvcpt (Point2 *pc, double *radc,
                      Point2 *ps, Vector2 *vcs, Point2 *pe) {
//=======================================================================
/// \code
///  circ from point and point+tangentVector
/// 
/// Input:
///   ps     = Startpunkt des Cir,
///   vcs    = Startvektor
///   pe     = ein Punkt am Kreis
/// Output:
///   pc     = Mittelpunkt
///   radc   = Radius; ACHTUNG: negativ bei CW
/// \endcode


  double  len_s_e, len_s_x;
  Vector2 vcs_n;
  Point2  ptx;



  // printf("UT2D_ci_ptvcpt ps=%f,%f vs=%f,%f\n",ps->x,ps->y,vcs->dx,vcs->dy);
  // printf("               pe=%f,%f\n",pe->x,pe->y);


  // Abst ps - pe
  len_s_e = UT2D_len_2pt (ps, pe);
  if(len_s_e < UT_TOL_min1) goto L_Err1;

  // Normalvektor auf vcs
  UT2D_vc_perpvc (&vcs_n, vcs);


  // pe auf die Normale zum ps projizieren
  UT2D_pt_projptptvc (&ptx, pe, ps, &vcs_n);


  // Abst. vom proj. Punkt zum Startpkt.
  len_s_x = UT2D_len_2pt (ps, &ptx);
  if(len_s_e < UT_TOL_min2) goto L_Err1;


  *radc = ((len_s_e * len_s_e) / len_s_x) / 2.;
  // printf("*radc %f %f %f\n",*radc, len_s_e, len_s_x);



  // liegt pe ueber oder unter ps -vcs ? + = CCw, - = CW.
  if(UT2D_sid_ptvc (pe, ps, vcs) < 0) *radc = -(*radc);



  // Radius in Richtg Mittelpkt ergibt Mittelpkt.
  UT2D_pt_traptvclen (pc, ps, &vcs_n, *radc);

  // printf("ex UT2D_ci_ptvcpt pc=%f,%f r=%f\n",pc->x,pc->y,*radc);


  return 0;



  //===========================================================
  L_Err1:
  printf("UT2D_ci_ptvcpt err\n");
  *pc = *ps;
  // exit(0);
  return -1;

}


//======================================================================
  int UT2D_m2_invtravcm2 (Mat_3x2 im1, Mat_3x2 m1) {
//======================================================================
// UT2D_m2_invtravcm2         Author: Thomas Backmeister         9.4.2003
/// \code
/// UT2D_m2_invtravcm2         invert 2D-vector transformation
/// 
/// Inversion of a 2D-vector transformation.
/// 
/// IN:
///   Mat_3x2 m1  ... transformation matrix
/// OUT:
///   Mat_3x2 im1 ... inverse transformation matrix
/// Returncodes:
///   0 = OK
///   1 = no inverse trafo
/// \endcode


  double det, tol = 0.0000001;

  //printf("UT2D_m2_invtravcm2: invert 2D-vector trafo\n");

  // determinant of m1 (m1 without last column)
  det = m1[0][0] * m1[1][1] - m1[1][0] * m1[0][1];
  if (fabs(det) < tol) {
    printf("UT2D_m2_invtravcm2: transformation has no inverse\n");
    return 1;
  }
  det = 1.0 / det;

  // inversion of vector 1 and 2
  im1[0][0] = det * m1[1][1];
  im1[1][0] = det * (-m1[1][0]);
  im1[0][1] = det * (-m1[0][1]);
  im1[1][1] = det * m1[0][0];

  // inversion of translation vector
  im1[0][2] = -(im1[0][0] * m1[0][2] + im1[0][1] * m1[1][2]);
  im1[1][2] = -(im1[1][0] * m1[0][2] + im1[1][1] * m1[1][2]);

  return 0;
}


//=====================================================================
  void UT2D_m2_load (Mat_3x2 ma,  Vector2 *vc1, Point2 *ori) {
//=====================================================================
/// UT2D_m2_load            2D-Refsys from baseDirection, basePoint.
// 
// vc1 ist der Vektor, der die Richtung der Drehung angibt (die basline)
// ori ist der Mittelpunkt, um den gedreht wird.


// Das neue 2D-Achsensystem definieren; 
// dann mit UT2D_pt_traptm2 transformieren.

/*
  Vector2   vx;
  Point2    p1, p2, p3;
  Mat_3x2   m1;

  // 2D-Transformation.
  UT2D_vc_2db (&vx, 0., 1.);
  UT2D_pt_2db (&p3, 10., 0.);
  UT2D_m2_load (m1, &vx, &p3);

  UT2D_pt_2db (&p1, 10., 0.);
  UT2D_pt_traptm2 (&p2, m1, &p1);

  UT2D_pt_2db (&p1, 10., 10.);
  UT2D_pt_traptm2 (&p2, m1, &p1);

*/

  Vector2   vcx;

  /* TX_Print(" vc1=%f,%f",vc1->dx,vc1->dy); */


  /* normieren (Länge 1) */
  UT2D_vc_normalize (&vcx, vc1);
  // TX_Print(" vcx=%f,%f",vcx.dx,vcx.dy);


  ma[0][0] =  vcx.dx;  /* cos */
  ma[1][0] =  vcx.dy;  /* sin */

  ma[0][1] = -vcx.dy;  /* -sin */
  ma[1][1] =  vcx.dx;  /* cos */


  ma[0][2] = ori->x;
  ma[1][2] = ori->y;


}


//=====================================================================
  void UT2D_m2_loadtravcm2 (Mat_3x2 ma, Vector2 *vx, Vector2 *vy, Vector2 *vt) {
//=====================================================================
// UT2D_m2_loadtravcm2      Author: Thomas Backmeister         9.4.2003
/// \code
/// UT2D_m2_loadtravcm2      load 2D-vector trafo (3x2 matrix)
/// 
/// Load a 2D-vector transformation with 3x2-matrix.
/// 
/// IN:
///   Vector2 *vx  ... vector 1
///   Vector2 *vy  ... vector 2
///   Vector2 *vt  ... translation vector
/// OUT:
///   Mat_3x2 ma   ... transformation matrix
/// \endcode


/*
  printf("UT2D_m2_loadtravcm2  vx=%f,%f\n", vx->dx, vx->dy);
  printf("                     vy=%f,%f\n", vy->dx, vy->dy);
  printf("                     vt=%f,%f\n", vt->dx, vt->dy);
*/

  ma[0][0] = vx->dx;
  ma[1][0] = vx->dy;

  ma[0][1] = vy->dx;
  ma[1][1] = vy->dy;

  ma[0][2] = vt->dx;
  ma[1][2] = vt->dy;
}


//====================================================================
  void UT2D_m2_init_rot (Mat_3x2 ma, double angr, Point2 *ptCen) {
//====================================================================
/// UT2D_m3_init_rot          Initialize a 3x2 - matrix with rotation
//
// Give back the matrix for a rotation about an axis.

  // see UT2D_npt_ci
  // see UT2D_npt_ci
  // Mat_3x2  m1;
  // UT2D_m3_init_rot (m1, UT_RADIANS (90.)); // 90 deg CCW
  // remove centerpoint
  // UT2D_pt_traptm3 (&p2, m1, &p1);          // apply
  // add centerpoint

// see also UT2D_m2_load


  double     dSin, dCos;
  // Vector2    vc1;


  dSin = sin(angr);
  dCos = cos(angr);


  // UT2D_vc_angr (&vc1, angle);

  // nur fuer rot um 0,0
  // vc1.dx = dCos;
  // vc1.dy = dSin;
  // UT2D_m2_load (ma, &vc1, ptCen);

  ma[0][0] = dCos;
  ma[0][1] = -dSin;

  ma[1][0] = dSin;
  ma[1][1] = dCos;

  ma[0][2] = ptCen->x;
  ma[1][2] = ptCen->y;

}


//=====================================================================
  void UT2D_pt_traptm2 (Point2 *p2, Mat_3x2 mata, Point2 *p1) {
//=====================================================================
/// \code
///  UT2D_pt_traptm2         einen 2D-Punkt mit 3x2-Matrix transformieren.
/// 
/// - transform point von Refsys ma nach absolut (mit normaler Matrix)
/// - transform point von absolut Refsys ima   (mit inverser Matrix)
/// 
///  Ausgabe: ein Punkt im 2D-System mata, gedreht um Center/Vektor in mata.
/// \endcode

  double x, y;


  // x = p1->x;
  // y = p1->y;
  x = p1->x - mata[0][2];
  y = p1->y - mata[1][2];


  // add Drehung + Origin
  p2->x = mata[0][0]*x + mata[0][1]*y + mata[0][2];
  p2->y = mata[1][0]*x + mata[1][1]*y + mata[1][2];

  /* p2->x = mata[0][0]*x + mata[0][1]*y; */
  /* p2->y = mata[1][0]*x + mata[1][1]*y; */




  /* TX_Print(" ex UT2D_pt_traptm2 %f,%f",p2->x,p2->y); */

}


//======================================================================
  int UT2D_lncoe_ln (double *k, double *d, Line2 *ln) {
//======================================================================
// UT2D_lncoe_ln       Author: Thomas Backmeister         29.4.2003
/// \code
/// UT2D_lncoe_ln       coefficients of 2D Line
/// 
/// Coefficients of 2D-Line equation y = k*x + d, if the line is
/// NOT parallel to the y-axis.
/// 
/// IN:
///   Line2 *ln    ...  2D-line
/// OUT:
///   double *k    ...  coefficient k
///   double *d    ...  coefficient d
/// Returncodes:
///   0 = OK
///   1 = line parallel y-axis
/// \endcode


  int irc;
  double h1, h2;

  //printf("UT2D_lncoe_ln: coefficients of 2D Line\n");

  irc = 0;

  // some values
  h1 = ln->p2.x - ln->p1.x;
  h2 = ln->p2.y - ln->p1.y;

  if (fabs(h1) < UT_TOL_min1) {
    // line parallel y-axis
    irc = 1;
  }
  else {
    *k = h2 / h1;
    *d = ln->p1.y - (*k) * ln->p1.x;
  }

  //printf("ex UT2D_lncoe_ln %d\n",irc);

  return irc;
}



//======================================================================
  int UT2D_ln_pts_dmax (Line2 *ln, int np, Point2 *ptab, double tol) {
//======================================================================
// UT2D_ln_pts_dmax         Author: Thomas Backmeister       4.6.2003
/// \code
/// UT2D_ln_pts_dmax          line <-- points with max. distance of pointTab.
/// 
/// Computation of a 2D-line from a set of 2D-points.
/// The line points will be such that their distance is maximal!
/// Die am weitesten auseinanderliegenden Punkte suchen, Linie davon machen.
/// 
/// IN:
///   int np       ... number of 2D-points
///   Point2 *ptab ... 2D-points
///   double tol   ... tolerance: minimal desired distance between
///                               2D-line points
/// OUT:
///   Line2 *ln    ... 2D-line with maximal point distance
/// Returncodes:
///   1: line exists
///   0: no line exists
/// \endcode


  int lnex, i1, i2, ind1, ind2;
  double dist, distmax;

  //printf("UT2D_ln_pts_dmax: 2D-line <-- set of 2D-points\n");

  // init
  lnex = 0;

  // check number of 2D-points
  if (np <= 1) {
    printf("UT2D_ln_pts_dmax: no line exists\n");
    return lnex;
  }

  // find the most distant 2D-points
  distmax = 0.0;
  ind1 = -1;
  ind2 = -1;
  for (i1=0; i1<np; ++i1) {
    for (i2=i1+1; i2<np; ++i2) {
      dist = UT2D_len_2pt (&ptab[i1], &ptab[i2]);
      if (dist > distmax) {
        distmax = dist;
        ind1 = i1;
        ind2 = i2;
      }
    }
  }

  // check 2D-point distance
  if (ind1 >= 0) {
    if (dist >= tol) {
      lnex = 1;
      ln->p1 = ptab[ind1];
      ln->p2 = ptab[ind2];
    }
  }

  return lnex;
}


//=============================================================================
  int UT2D_2parvc_3vc(double *dx,double *dy,Vector2 *v1,Vector2 *v2,Vector2 *v3){
//=============================================================================
/// \code
/// UT2D_2parvc_3vc project end of vec1 along vec2 on vec3
///
/// Zu gegebenen Vektoren v1, v2, v3 finden wir Zahlen x und y so,
/// dass x * v1 + y * v3 = v2.
///
/// dx/dy sind zwischen 0 bis 1; wenn dx=0.5 liegt der Endpunkt von V2
/// genau in der Mitte des Parallelogramss V1-V3.
///
///  y
///   \
///    \                        V1,V2,V3 have same startpoint s.
///  dy \-----------x
///      \         / \
///       \       /   \
///     V3 \    V2     \
///         \   /       \
///          \ /         \
///           s----------------------------x
///                      dx          V1
/// \endcode


  double Det, Dx, Dy;


  // printf("v1= %f %f\n", v1->dx, v1->dy);
  // printf("v2= %f %f\n", v2->dx, v2->dy);
  // printf("v3= %f %f\n", v3->dx, v3->dy);



  Det = v1->dx * v3->dy - v3->dx * v1->dy;

  Dx = v2->dx * v3->dy - v3->dx * v2->dy;
  Dy = v1->dx * v2->dy - v2->dx * v1->dy;

  if (Det != 0.0) {
    *dx = Dx/Det;
    *dy = Dy/Det;
    // printf("x= %f   y= %f\n", *dx, *dy);

  } else {
    printf("UT2D_2parvc_3vc - No solution!\n");
    // exit(0);
    return -1;
  }

  return 0;

}


//======================================================================
  int UT2D_ptNr_ci (double rdc, double ao, double tol) {
//======================================================================
/// \code
/// Anzahl Ecken circ berechnen
/// see also UT2D_angr_ciSec
/// \endcode

  int    iAnz;
  double a1, d1, d2, aTol, dCos;


  // printf("UT2D_ptNr_ci rd=%lf ao=%lf tol=%lf\n",rdc,ao,tol);


  a1 = rdc - tol;
  // printf("   tol=%f rd1=%f a1=%f\n",tol,rd1,a1);


  if(a1 < 0.) {   //  if Tol. > Radius
    aTol = RAD_45;
  } else {
    dCos = (a1/rdc);  //  der Oeffnungswinkel
    // damits etwa mit den Flaechen uebereinstimmt ...
    // aTol = ACOS(dCos) * 2.;
    aTol = ACOS(dCos);
    if(aTol > RAD_45)  aTol = RAD_45;
  }

  iAnz = (ao / aTol) + 1;
  if(iAnz < 2) iAnz = 2;


  // printf("ex UT2D_ptNr_ci %d\n");

  return iAnz;

}


//================================================================
  void UT2D_pt_traptm3 (Point2 *p2, Mat_4x3 mata, Point2 *p1) {
//================================================================
/// \code
/// UT2D_pt_traptm3           apply transformation to point (from 4x3-matrix)
/// p2 und p1 duerfen ident sein
/// \endcode

  double x, y;


  x = p1->x;
  y = p1->y;


  p2->x = mata[0][0]*x + mata[0][1]*y + mata[0][3];
  p2->y = mata[1][0]*x + mata[1][1]*y + mata[1][3];

}


//================================================================
  int UT2D_pt_cic_par (Point2 *pto, double *par, Circ2C *cic) {
//================================================================
/// UT2D_pt_cic_par       get 2D-point from par on 2D-circ in centerPos

// see also UT3D_pt_evparci
  
  double     aor, a1r, rdc, dsig;


  // printf("UT2D_pt_cic_par %lf %lf %lf\n",*par,cic->ango,cic->rad);

  dsig = 1.;
  rdc = cic->rad;
  aor = cic->ango;

  if(cic->rad < 0.) {
    dsig = -1.;
    rdc = fabs(rdc);
    aor = fabs(aor);
  }

  a1r = *par * aor + cic->angs;
    // printf(" a1r=%lf\n",a1r);

  pto->x = cos (a1r) * rdc;
  pto->y = sin (a1r) * rdc * dsig;

    // printf("ex UT2D_pt_cic_par %lf %lf\n",pto->x,pto->y);

  return 0;

}


//================================================================
  int UT2D_ptx_ci_pty (double *ptx, double *pty, double *rd) {
//================================================================
/// \code
/// UT2D_ptx_ci_pty       Get x-val of point on circle from y-val on circle
/// x = sqrt (r*r - y*y)  - phytagoras
/// y = sqrt (r*r - x*x)
/// ONLY QUADRANT 1 (use UT2D_circQuad23_angr)
/// \endcode

  *ptx = sqrt (*rd * *rd - *pty * *pty);

  return 0;

}


//================================================================
  int UT2D_pt_par_ci (Point2 *pto, double du, Circ2 *ci) {
//================================================================
/// UT2D_pt_par_ci            point on circle from parameter

  double   ad;


  printf("UT2D_pt_par_ci %lf\n",du);


  ad = ci->ango * du;

  UT2D_pt_rotptangr (pto, &ci->pc, &ci->p1, ad);
  
  return 0;

}
  

//==========================================================================
  int UT2D_pt_parvc_pt_vc (Point2 *pt, Point2 *p0, double lp, Vector2 *vc) {
//==========================================================================
// UT2D_pt_parvc_pt_vc  point on Linesegment + parametric value.
// 
// p0    startpoint of vc
// lp    parametric value of point on vc
//
//   p0          vc
//    x--lp--|---------------X
//          pt
//
// see UT2D_parvc_2vc

  Vector2    v1;


  // get parametric vector
  UT2D_vc_multvc (&v1, vc, lp);
    // printf("  v1 = %f %f\n",v1.dx,v1.dy);

  // add point + vector
  // UT2D_pt_ptvc (pt, p0, &v1);
  UT2D_pt_traptvc (pt, p0, &v1);

    // printf("ex UT2D_pt_parvc_pt_vc %f,%f\n",pt->x,pt->y);

  return 0;

}


//================================================================
  int UT2D_parvc_2vc (double *lp, Vector2 *v1, Vector2 *v2) {
//================================================================
/// \code
/// UT3D_parvc_2vc      parameter of distance of vec1 projected on vec2
/// Output:
///   lp         parameter; eg 0.5 = iproj. of end of v1 is in middle of v2
///   RetCode:
///     0  OK
///    -1  v1-v2 aufeinander Normal; pl = 0.
/// dl ist zwischen 0 bis 1; wenn dl=0.5 liegt der Endpunkt von V2
/// genau in der Mitte des Vektors V2. Mit Vorzeichen !
/// v1={0.5, 0.5, 0.}, v2={1., 0., 0.} gives pl = 0.5
/// v1={-0.5, 0.5, 0.}, v2={1., 0., 0.} gives pl = -0.5
/// v1={1.5, 0.5, 0.}, v2={1., 0., 0.} gives pl = 1.5
///
///    
///                             V1,V2 have same startpoint s.
///                 x
///                /| 
///               / |
///             V1  |
///             /   |
///            /    |
///           s---------V2-------x
///                 dl 
///
/// see UT2D_pt_parvc_pt_vc
/// \endcode



  // int       irc;
  double    dt, sk;



  // printf("v1 = %f %f\n",v1->dx,v1->dy);
  // printf("v2 = %f %f\n",v2->dx,v2->dy);


  dt = v2->dx * v2->dx + v2->dy * v2->dy;


  // exact normal:
  if (dt == 0.0) {
    *lp = 0.;

  } else {
    sk = v1->dx * v2->dx + v2->dy * v1->dy;
    *lp = sk / dt;
  }

    // printf("ex UT2D_parvc_2vc lp=%f dt=%f sk=%f\n",*lp,dt,sk);
// 
  return 0;

}


//===============================================================
  int UT2D_ckvc_inA_vc_vc (Vector2 *v1, Vector2 *v2, Vector2 *v3) {
//===============================================================
/// \code
/// UT2D_ckvc_inA_vc_vc     check if v2 is between v1 and v3
/// test parellel and antiparallel; if(v1-v2 > 180 deg) invert v2.
/// the opening-angle of v1-v3 is less than 180 deg.
/// 
/// RetCod:
///   0 = yes; v2 is between v1 and v3
///   1 = no;  v2 = above (left of) v1 and v3
///  -1 = no;  v2 = below (right of) v1 and v3
/// \endcode

  double  d1;
  Vector2 v2p;



  // test if v2 has more than 90-deg difference from v1; yes: invert v2.
  d1 = UT2D_slen_vc_vcNo (v1, v2);
  if(d1 < 0.) {
    UT2D_vc_invert (&v2p, v2);
  } else {
    v2p = *v2;
  }

  return UT2D_ckvc_in2vc (v1, &v2p, v3);

}


//================================================================
  int UT2D_ckvc_parl (Vector2 *v1, Vector2 *v2, double tol) {
//================================================================
/// \code
/// test if 2 vectors parallel
/// tol = max distance of v1 from v2.
/// BOTH VECTORS MUST BE NORMALIZED.
///
///                .       
///              / |
///            /   |
///         v1    tol
///        /       | 
///      X ---v2---x------->
/// 
///
/// RetCod:
///    0      OK; parallel.
///   -1      not parallel.
/// \endcode

// see also UT2D_ckvc_in2vc

  double   d1;

  // printf("UT2D_ckvc_parl %f\n",tol);
  // UT3D_stru_dump (Typ_VC2, v1, "  v1:");
  // UT3D_stru_dump (Typ_VC2, v2, "  v2:");

  UT2D_slen_nor_vc_vcNo (&d1, v1, v2);
    // printf(" _ck_parl-d1=%f\n",d1);

  if(fabs(d1) < tol) return 0;
  return -1;

}


//================================================================
  int UT2D_ptx_tang_cic_pt (double *x0, double xp, double rdc) {
//================================================================
/// \code
/// get x-value where Y=0 of tangent to circle
/// Used for tangent to circle tru point on circle.
/// Input:
///   xp      x-value of a point on the circle
///   rdc     circle radius
/// Output:
///   x0      x-value of teh point of the tangent to circle where y=0
/// retCod:   0=OK; x0 set.
///           1=vertical; xp==rdc
///           2=horizontal; xp==0
/// \endcode

  int      irc=0;
  double   yp, dx1;


  printf("UT2D_ptx_tang_cic_pt xp=%lf rdc=%lf\n",xp,rdc);

  if(UTP_compdb0(xp, UT_TOL_min0)) {irc = 2; goto L_exit;}


  if(UTP_comp2db (xp, rdc, UT_TOL_min0)) {irc = 1; goto L_exit;}


  // get y-value of point with x=xp on circle with rad=rdc
  UT2D_ptx_ci_pty (&yp, &xp, &rdc);
    printf(" yp=%lf\n",yp);

  // get dx right of point on circ
  UT2D_solvtriri_p_q_hc (&dx1, fabs(xp), yp);

  *x0 = dx1 + fabs(xp);

  if(xp < 0.) *x0 *= -1.;

  L_exit:
    printf("ex UT2D_ptx_tang_cic_pt irc=%d x0=%lf\n",irc,*x0);
  return 0;

}


//=======================================================================
  int UT2D_pt_tng_ci_vc (Point2 *ptg, Circ2 *ci, Vector2 *vt, int isol) {
//=======================================================================
// UT2D_pt_tng_ci_vc          tangent with fixed vector to circ
// imod      0 = point at y-axis (vc1=x-axis; ci1.vz=z-axis)
//           1 = point at negative y-axis


  double   d1;
  Vector2  vcn;


  // printf("UT2D_pt_tng_ci_vc %d\n",isol);

  // get normal to vc1 (normal=y from vc1=x and ci.vz
  UT2D_vc_perpvc (&vcn, vt);
    // UT3D_stru_dump (Typ_VC, &vcn, "  vcn:");


  // translate circ.pc length=radius along vcn
  d1 = fabs(ci->rad);
  if((isol % 2) > 0) d1 *= -1.;
  UT2D_pt_traptvclen (ptg, &ci->pc, &vcn, d1);
    // GR_Disp_pt (ptg, SYM_TRI_S, 2);
    // GR_Disp_vc (vc1, pto, 2, 0);

  return 0;

}


//=======================================================================
  int UT2D_pt_tng_ci_pt (Point2 *ptg, Circ2 *ci, Point2 *pt1, int isol) {
//=======================================================================

/// UT2D_pt_tng_ci_pt              tangent from point (outside circ) to circ
/// RetCod:
///    1      point is on circle
///    0      OK
///   -1      Error; pt1 inside circ.

// see also UT3D_pt_tangptci UT3D_vc_tng_ci_par1 UT3D_vc_tng_ci_pt

  int      irc, sr;
  double   dx, dy, d1, d2;
  Vector2  vc1;


  // printf("====================================== \n");
  // printf("UT2D_pt_tng_ci_pt %d\n",isol);
  // UT3D_stru_dump(Typ_PT2, pt1, "  pt1:");
  // UT3D_stru_dump(Typ_CI2, ci, "  ci:");

  // d1 = dist pt1-cen
  d1 = UT2D_len_2pt (pt1, &ci->pc);

  sr = DSIGN (ci->rad);
  d2 = fabs(ci->rad);

  // test if pt1 is on circle
  if(UTP_comp2db (d1, d2, UT_TOL_pt) == 1) {
      // printf(" PT ON CIRC\n");
    UT2D_vc_cipt (&vc1, &ci->pc, pt1, sr);
    if(isol) d2 *= -1.;   // invert direction
    UT2D_pt_traptvclen (ptg, pt1, &vc1, d2);
    // vom pc dx,dy in Richtg pc-pt1 abtragen
    // ptg = pt1 + vc1
    // UT2D_vc_2pt (&vc1, &ci->pc, pt1);
    // UT2D_pt_traptvc2len (ptg, &ci->pc, &vc1, dx, dy);
    irc = 1;
    goto Fertig;
  }

  // test if pt1 is inside circle
  if(d1 < d2) { irc = -1; goto Fertig; }

  // pt1 is outside
  irc = 0;

  // pt1 liegt ausserhalb Kreis; 2 tangenten durch pt1 an den Kreis.
  UT2D_solvtriri_bc   (&dx, &dy, d2, d1);
    // printf("P = P C dx=%f dy=%f d2=%f d1=%f\n",dx,dy,d2,d1);

  if(isol) dy *= -1.;

  // vom pc dx,dy in Richtg pc-pt1 abtragen
  UT2D_vc_2pt (&vc1, &ci->pc, pt1);
  UT2D_pt_traptvc2len (ptg, &ci->pc, &vc1, dx, dy);


  Fertig:
    // GR_Disp_pt2 (ptg, SYM_TRI_S, 2);
    // UT3D_stru_dump(Typ_PT2, ptg, "  ptg:");
    // printf("ex UT3D_pt_tangptci %d\n",irc);

  return irc;

}


//================================================================
  int UT2D_pt_mid_npt (Point2 *ptc, Point2 *npt, int ptNr) {
//================================================================
/// UT2D_pt_mid_npt           midpoint from n points

  int    i1;
  double dx=0., dy=0.;

  for(i1=0; i1<ptNr; ++i1){
    dx += npt[i1].x;
    dy += npt[i1].y;
  }

  ptc->x = dx / ptNr;
  ptc->y = dy / ptNr;
    // GR_Disp_pt2 (ptc, SYM_STAR_S, 0);

  return 0;

}


//================================================================
 
