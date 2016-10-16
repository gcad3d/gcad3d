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
2003-11-06 UT3D_el_pt2vc2a zu. RF.
2003-07-05 neu ex ut_geo.h. RF

-----------------------------------------------------
*/
/*!
\file  ../ut/ut_elli.c
\brief ellipse
\code
=====================================================
List_functions_start:

UT3D_ptNr_ell             get nr of points for ellipse
UT3D_npt_ell              polygon from ellipse, fixed nr of points

UT3D_par1_el_pt           get parameter (0-1) for point on ellipse
UT3D_par1_angr_ell    UU  parametric-Angle --> par 0-1

UT3D_ck_el360             check if elli is 360-degree-elli
UT3D_angr_par1_ell        par 0-1  -->  parametric-Angle
UT3D_angr_elpt            real angle for point on elli
UT3D_angr_el_corr         get angle-circ from angle of elli.
UT3D_2angr_el             get starting- and opening angle of ellipse
UT3D_ango_el          UU  opening angle of ellipse
UT3D_lne_ell              get dist center-focus for ellipse
UT3D_pt_ell_lim_del       remove points outside limited ellipse

UT3D_ptvc_eval_ell_par    get point/tangentVector on ellipse from parameter
UT3D_pt_eval_ell_par1     pt <- parametric-Angle (0-1)
UT3D_pt_elangd            pt <- parametric-Angle.
UT3D_pt_ck_onel           check if point lies on an (full) ellipse
UT3D_pt_elfoc             focal points of ellipse
UT3D_pt_projptel          proj PT -> CRV-Ellipse
UT3D_pt_el_ptx            get y-coord of point on elli
UT3D_pt_el_pty            get x-coord of point on elli

UT3D_pt_intlnel__         intersect line and ellipse
UT3D_pt_intlnel_p         intersection LN-ELL (gives 0/1/2 points)
UT3D_pt_intplell          intersect plane ellipse

UT3D_vc_tangel            tangent thru point ON ellipse
UT3D_pt_tng_ell_vc__      tangents vector-ellipse.                   DO NOT USE
UT3D_vc_tng_elpt__        tangents point-ellipse                     DO NOT USE
UT3D_vc_tng_elptMaj       tangents point-ellipse; point on majorAxis DO NOT USE
UT3D_vc_tng_elptMin       tangents point-ellipse; point on minorAxis DO NOT USE

UT3D_el_ptc_ptx_pt        elli from centerPt,endPtMajAxis,ptOnElli.
UT3D_el_pt2vc2a           elli=center,vecMajAx,vecMinAx,AngStart,angEnd,dir.
UT3D_el_ci                change circ > ellipse
UT3D_el_projelpl          ellipse = project ELL -> PLN
UT3D_el_projcipl          ellipse = project CIR -> PLN
UT3D_el_elcoe             Ellipse <-- coeff-data (IGES)
UT3D_el_elpt180           change elli -> 180 deg-elli; keep dir
UT3D_el_el2pt             change start- and endpoint of elli
UT3D_elcoe_el             CoeffEllipse(IGES) <-- ellipse
UT3D_el_inv1              turn direction and p1/p2 (same display !)
UT3D_el_inv2              turn direction (do not swap p1,p2)

-------------- CurvEll2C -------------------------------------
UT2D_elc_el3              make 2D-ell in centerPos from 3D-ell
UT2D_angr_elc_pt          get angle for point on 2D-elli in centerPos.
UT2D_pt_el_ptx            get y-coord of point on elli
UT2D_pt_el_pty            get x-coord of point on elli
UT2D_pt_elangd            pt from angle, rMaj, rMin.

UT2D_vc_tang_el2          tangent thru point ON 2D-ellipse
UT2D_vc_tang_elc          tangent thru point ON 2D-ellipse/centerPos
UT2D_pt_int_elc_ln        intersection of ellipse - line (ell.in centerPos)
UT2D_pt_tng_ell_vc        get tangent vector-ellipse.
UT2D_pt_tng_ell_pt        get tangent point-ellipse.
UT2D_pt_tng_ell_ptMaj     tangents point-ellipse; point on majorAxis
UT2D_pt_tng_ell_ptMin     tangents point-ellipse; point on minorAxis

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


CurvElli   el
CurvEll2   el2
CurvEll2C  el2c


Returncodes:
 0 (meistens !) OK
>0 OK mit Zusatzinfo
<0 Error mit Zusatzinfo

  // Parameters = angle/2*PI (value 0-1).
  // angle refers to innerCircle of ellipse, not to point on ellipse !
  // the same point on ellipse and on its innerCircle is on a horizontal line.
  // y-values of the point on innerCircle and ellipse are identical.
  // Get x-value of point on ellipse from y on innerCircle with UT2D_pt_el_pty
  // Get x-val of point on circle from y-val on circle with UT2D_ptx_ci_pty
  // get y-value of point on ellipse = innerCircle from angle with sin(a)*r
  // get angle from point on innerCirc with UT2D_angr_vc

// elli in centerpos:
P20=P(0 0 0)
P21=P(200 0 0)
P22=P(0 100 0)
S20=ELL P20 P21 P22
C20=P20 P22
L20=P20 DX ANG(45)
P23=INT L20 C20
L21=P23 DX
L22=P20 DY
L23=P20 DX

  // UT3D_pt_3db (&el3->p2, 89.443,  89.443,    0.);  // Q1
  // UT3D_pt_3db (&el3->p2, -141.421,  70.711,    0.);  // Q2
  // UT3D_pt_3db (&el3->p2, -89.443,  -89.443,    0.);  // Q3
  // UT3D_pt_3db (&el3->p2, 141.421,  -70.711,    0.);  // Q4

// elli trimmed
S20=ELL P(300 200 0) P(600 100 0) P(550 250 0)
P20=P(S20 0.1)
P21=P(S20 0.4)
S21=CUT S20 P20 P21

compile mit gcc:
cc -c -O0 -g3 -Wall ut_elli.c

Windows:
cl -c ut_geo.c

*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


// #include "../ut/ut_umem.h"   // MEM_alloc_tmp
#include "../ut/ut_geo.h"
#include "../ut/ut_math.h"   // UTM_scale_4db

#include "../ut/func_types.h"     // SYM_TRI_S ..

// #include "../ut/ut_TX.h"    /* TX_Error */


#include "../ut/ut_elli.h"



//================================================================
  int UT3D_pt_ell_lim_del (int *nrp, Point *pa, double *va1,
                           CurvElli *cv1) {
//================================================================
/// \code
/// UT3D_pt_ell_lim_del       remove points outside limited ellipse
/// Input:
///   nrp    nr of points on unlimited ellipse cv1
///   pa     points
///   va1    size nrp, can be empty or NULL
/// Output:
///   nrp    nr of points inside trimmed ellipse cv1
///   va1    parameter on points;
/// 
/// see UT3D_pt_ln_lim_del UT3D_ck_npt_inCirc
/// \endcode


  int     i1;

  if(*nrp < 1) return 0;

  if(!va1) {
    // get local space
    va1 = MEM_alloc_tmp (sizeof(double) * *nrp);
  }

  for(i1=0; i1 < *nrp; ++i1) {
    // get parameter of point on elli
    UT3D_par1_el_pt (&va1[i1], cv1, &pa[i1]);
  }

  // delete all points & parameters not on obj
  UT3D_par_ck_inObj_del (nrp, pa, va1, &cv1->p1, &cv1->p2, 0., 1.);

  return 0;

}





//================================================================
  double UT3D_lne_ell (CurvElli *el1) {
//================================================================
/// UT3D_lne_ell                    get dist center-focus for ellipse

// lne = sqrt ((a*a) - (b*b));

  double da, db, de;


  da = UT3D_lenq_vc (&el1->va);
  db = UT3D_lenq_vc (&el1->vb);

  de = sqrt (da - db);

  return de;

}
 



//================================================================
  int UT3D_el_ci (CurvElli *eo, Circ *ci) {
//================================================================
/// \code
/// change circ > ellipse
/// vz of circ must have length=1 !
/// \endcode


  eo->pc = ci->pc;
  eo->p1 = ci->p1;
  eo->p2 = ci->p2;
  eo->vz = ci->vz;
  eo->dir = DSIGN(ci->rad);

  // vz,va,vb,dir
  UT3D_vc_2pt (&eo->va, &ci->pc, &ci->p1);
  UT3D_vc_perp2vc (&eo->vb, &ci->vz, &eo->va);
  // UT3D_vc_setLength (&eo->vb, &eo->vb, fabs(ci->rad));

    // UT3D_stru_dump (Typ_CVELL, eo, "ex UT3D_el_ci:");

  return 0;

}


//================================================================
  int UT3D_el_inv1 (CurvElli *el1) {
//================================================================
// UT3D_el_inv1              turn direction and p1/p2 (same display !)


  MEM_swap__(&el1->p1, &el1->p2, sizeof(Point));

  el1->dir = -el1->dir;

  return 0;

}


//================================================================
  int UT3D_el_inv2 (CurvElli *el1) {
//================================================================
/// UT3D_el_inv2              turn direction (do not swap p1,p2)


  el1->dir = -el1->dir;

  return 0;

}


//================================================================
  double UT3D_angr_par1_ell (double par1, CurvElli *el1) {
//================================================================
/// \code
/// UT3D_angr_par1_ell          par 0-1  -->  angle
/// par1 always 0-1 (relative); if el1 is relimited to 0deg-90deg, then
///   par1=0.5 gives 45deg.
/// par_Angle  = StartAngle + (OpeningAngle * par1)
/// \endcode
 
  double   aa, as, ango;
  CurvEll2C  el2c;


  // printf("UT3D_angr_par1_ell %f\n",par1);
  // UT3D_stru_dump (Typ_CVELL, el1, "");


  if(UT3D_ck_el360(el1) == 0) {
    aa = RAD_360 * par1;
    goto L_done;
  }

  // make el2c from ell
  UT2D_elc_el3 (&el2c, el1);

  // get as, ao
  UT3D_2angr_el (&as, &ango, &el2c);

  aa = as + (ango * par1);

    // printf("  ango=%lf as=%lf\n",ango,as);

  L_done:
    // printf("ex UT3D_angr_par1_ell %lf\n",aa);
  return aa;

}


/* UNUSED
//================================================================
  double UT3D_par1_angr_ell (double *lpar, CurvElli *el1) {
//================================================================
/// \code
/// UT3D_par1_angr_ell          angle in einen Parameterwert von 0-1 aendern
/// par1_Angle = (par_Angle - StartAngle) / OpeningAngle
/// \endcode

  double  a1, a2, ango, p1;
  Vector   v1, v2;

  // printf("UT3D_angr_par1_ell %f\n",*lpar);


  if(UT3D_ck_el360(el1) == 0) {
    p1 = *lpar / RAD_360;
    goto L_done;
  }

  // keine 360 Grad;
  // use p1 p2 pc vz dir
  UT3D_vc_2pt (&v1, &el1->pc, &el1->p1);
  UT3D_vc_2pt (&v2, &el1->pc, &el1->p2);

  // den StartAngle errechnen
  // aa = UT3D_angr_3vc__ (&el1->vz, &el1->va, &v1);
  a1 = UT3D_angr_elpt (&el1->p1, &el1->pc, &el1->va, &el1->vb);
  a2 = UT3D_angr_elpt (&el1->p2, &el1->pc, &el1->va, &el1->vb);

  // den OpeningAngle errechnen
  // ango = UT3D_angr_3vc__ (&el1->vz, &v1, &v2);
  // if(el1->dir < 0) ango = RAD_360 - ango;
  ango = UT2D_angr_2angr (a1,a2, el1->dir);

  p1 = (*lpar - a1) / ango;


  L_done:
  // printf("ex UT3D_par1_angr_ell %f %f %f %f\n",p1,ango,a1,a2);

  return p1;

}
*/

//======================================================================
  int UT3D_ck_el360 (CurvElli *el1) {
//======================================================================
/// \code
/// UT3D_ck_el360             check if elli is 360-degree-elli
/// RC =  1:   NO
/// RC =  0:   YES, is 360-deg-elli
/// RC = -2:   degenerated
/// if(UT3D_ck_el360 == YES) ...
/// \endcode

  int    irc;
  double d1;


  if(UT3D_lenB_vc(&el1->va) < UT_TOL_cv) return -2; // degenerated


  // irc = UT3D_comp2pt (&el1->p1, &el1->p2, UT_TOL_pt);
  d1 = UT3D_lenB_2pt (&el1->p1, &el1->p2);

  if(d1 < UT_TOL_pt) return 0;

  return 1;

}


//======================================================================
  int UT3D_el_elpt180 (CurvElli *el1, Point *p1) {
//======================================================================
// UT3D_el_elpt180           change elli -> 180 deg-elli; keep dir

  UT3D_stru_dump (Typ_PT, p1, "UT3D_el_elpt180");

  el1->p1 = *p1;
  UT3D_pt_opp2pt (&el1->p2, &el1->pc, &el1->p1);

  UT3D_stru_dump (Typ_CVELL, el1, "ex UT3D_el_elpt180\n");

  return 0;

}

//======================================================================
  int UT3D_el_el2pt (CurvElli *el1, Point *p1, Point *p2) {
//======================================================================
/// UT3D_el_el2pt             change start- and endpoint of elli

  el1->p1 = *p1;
  el1->p2 = *p2;

  // UT3D_stru_dump (Typ_CVELL, el1, "ex UT3D_el_el2pt\n");

  return 0;

}


//========================================================================
  double UT3D_angr_elpt (Point *pti,Point *ptc,Vector *va,Vector *vb) {
//========================================================================
/// \code
/// UT3D_angr_elpt            real angle for point on elli
/// real angle (not angle on innerCircle. Used for dxf).
/// \endcode

  double a1, dx, dy, lMaj, lMin;
  Vector vp;

  // printf("UT3D_angr_elpt p=%f,%f,%f\n",pti->x,pti->y,pti->z);
  // printf("               c=%f,%f,%f\n",ptc->x,ptc->y,ptc->z);
  // printf("               a=%f,%f,%f\n",va->dx,va->dy,va->dz);
  // printf("               b=%f,%f,%f\n",vb->dx,vb->dy,vb->dz);

  lMaj = UT3D_len_vc (va);
  lMin = UT3D_len_vc (vb);
  // printf("lMaj=%f lMin=%f\n",lMaj,lMin);

  UT3D_vc_2pt (&vp, ptc, pti);
  // printf("               vp=%f,%f,%f\n",vp.dx,vp.dy,vp.dz);

  dx = UT3D_acos_2vc(va, &vp) / lMaj;
  dy = UT3D_acos_2vc(&vp, vb) / lMin;

  // Winkel
  a1 = acos (dx / lMaj);

  if(dy < 0) a1 = RAD_360 - a1;

    // printf("ex UT3D_angr_elpt a=%f dx=%f dy=%f\n",a1,dx,dy);

  return a1;

}


/* REPLACED WITH UT3D_2angr_el
//================================================================
  double UT3D_ango_el (CurvElli *el1) {
//================================================================
/// UT3D_ango_el              opening angle of ellipse

  double    d1, d2, ao;


  d1 = UT3D_angr_elpt (&el1->p1, &el1->pc, &el1->va, &el1->vb);
  d2 = UT3D_angr_elpt (&el1->p2, &el1->pc, &el1->va, &el1->vb);

  if(el1->dir > 0) {     // for CCW:
    if(d2 < d1) d2 += RAD_360;        // make a2 > a1
    ao = d2 - d1;                     // ao = a2 - a1
  } else {               // for CW:
    if(d1 < d2) d1 += RAD_360;        // make a1 > a2
    ao = d1 - d2;                     // ao = a1 - a2
  }

  // if(ao > RAD_360) d3 -= RAD_360

  return ao;

}
*/


//===================================================================
  int UT3D_ptvc_eval_ell_par (Point *pto, Vector *vct,
                        CurvElli *el1, int pTyp, double par) {
//===================================================================
/// \code
/// UT3D_ptvc_eval_ell_par       get point/tangentVector on ellipse from parameter
///
/// Input:
///   pto        point; NULL for no output
///   vct        tangent-vector; NULL for no output
///   pTyp       type of parameter;
///                 0=normalized parameter (0-1)
///                 1=native parameter (angle); any value ..
/// \endcode


// was UTO_pt_evpar1ost
// see also UTO_pt_par1_dbo UT3D_ptvc_ox UT3D_ptvc_sur UT3D_ptvc_sus
//   UTO_vc_tng_obj_par
  
  
  int      irc=0, ii;
  double   lx, ly;
  Point    ptt;
  Point2   pt20;
  

  // printf("UT3D_ptvc_eval_ell_par par=%lf pTyp=%d\n",par,pTyp);
  
  
  //----------------------------------------------------------------
  // point;    yet need point for vector.
    
  if(pTyp == 0) 
    par = UT3D_angr_par1_ell (par, el1);  // angle from 0-1-parameter
      // printf(" angr=%f\n",par);
  

  lx = UT3D_len_vc (&el1->va);  // Laenge hauptachse
  ly = UT3D_len_vc (&el1->vb);  // length 

  // 2D-Punkt errechnen
  UT2D_pt_elangd (&pt20, lx, ly, par);
    // printf(" pt20 = %lf %lf\n",pt20.x,pt20.y);

  // um dx, dy verschieben
  UT3D_pt_trapt2vc2len(&ptt, &el1->pc, &el1->va, pt20.x, &el1->vb, pt20.y);
    // GR_Disp_pt (&ptt, SYM_STAR_S, 2);

  // copy point out
  if(pto) *pto = ptt;


  //----------------------------------------------------------------
  // vector
  if(vct) UT3D_vc_tangel (vct, &ptt, el1);
    // GR_Disp_vc (vct, &ptt, 9, 0);

  return 0;

}


//====================================================================
  int UT3D_pt_eval_ell_par1 (Point *pto, CurvElli *el1, double par1) {
//====================================================================
/// \code
/// UT3D_pt_eval_ell_par1          get point on ellipse from parameter 0-1
/// par1     parameter; 0-1
/// see UT3D_pt_elangd
/// \endcode

  double   lx, ly, angr;
  Point2   pt20;


  // printf("UT3D_pt_eval_ell_par1 %f\n",par1);
  // UT3D_stru_dump (Typ_CVELL, el1, " el1");

  return UT3D_ptvc_eval_ell_par (pto, NULL, el1, 0, par1);
 
}


//========================================================================
  int UT3D_pt_elangd (Point *pto,
                      Point *ptc, Vector *va, Vector *vb, double angr) {
//========================================================================
/// UT3D_pt_elangd            pt <- parametric-Angle.
/// Input:
///   angr       in rad

  double   lx, ly;
  Point2   pt20;

  lx = UT3D_len_vc (va);  // Laenge hauptachse
  ly = UT3D_len_vc (vb);

  // 2D-Punkt errechnen
  UT2D_pt_elangd (&pt20, lx, ly, angr);

  // um dx, dy verschieben
  UT3D_pt_trapt2vc2len(pto, ptc, va,pt20.x, vb,pt20.y);

  // Test rueckrechnen:
  // printf("par=%f\n\n",UT3D_angr_elpt(pto,ptc,va,vb));

  return 0;

}


//========================================================================
  int UT2D_pt_elangd (Point2 *pto, double lx, double ly, double angr) {
//========================================================================
/// \code
/// UT2D_pt_elangd            pt from angle, rMaj, rMin.
/// get point on ellipse from angle and length of axes.
/// Input:
///   angr     angle in rad
///     lx     length major axis
///     ly     length minor axis
/// \endcode


  int    q23, q34;
  double ac;

  // get corrected angle for angr
  ac = UT3D_angr_el_corr (angr, lx, ly);


  // q23 = UT2D_circQuad23_angr (&angr);
  // q34 = UT2D_circQuad34_angr (&angr);
// 
  // aq = fabs(atan(lx * tan(angr) / ly));  // get angle for outerCirc ?

  // printf("UT2D_pt_elangd lx=%f ly=%f angr=%lf ac=%lf\n",lx,ly,angr,ac);
  // // printf("  q23=%d q34=%d\n",q23,q34);


  pto->x = lx * cos(ac);
  pto->y = ly * sin(ac);

  // if(q23) pto->x *= -1.;
  // if(q34) pto->y *= -1.;

    // printf("ex UT2D_pt_elangd %f %f\n",pto->x,pto->y);

  return 0;

}


//============================================================================
  int UT3D_el_ptc_ptx_pt (CurvElli *el1, Point *ptc, Point *ptx, Point *pt1) {
//============================================================================
/// UT3D_el_ptc_ptx_pt        elli from centerPt,endPtMajAxis,ptOnElli.

  Vector    v1;
  double    dx, dy, da, db;


  // printf("UT3D_el_ptc_ptx_pt \n");
  // UT3D_stru_dump(Typ_PT, ptc, "  PTC:");
  // UT3D_stru_dump(Typ_PT, ptx, "  PTX:");
  // UT3D_stru_dump(Typ_PT, pt1, "  PT1:");


  el1->pc = *ptc;
  el1->p1 = *ptx;
  el1->p2 = *ptx;
  UT3D_vc_2pt (&el1->va, ptc, ptx);
  UT3D_vc_2pt (&v1, ptc, pt1);

  // project pt1 onto ptc-va to get dx, dy
  dx = fabs(UT3D_slen_projvcvc (&el1->va, &v1));
  dy = UT3D_nlen_2ptvc (pt1, ptc, &el1->va);
    // printf(" dx=%f dy=%f\n", dx, dy);

  // get b from formula: b=sqrt((y*y)/(1-((x*x)/(a*a))))
  da = UT3D_len_vc (&el1->va);
  db = sqrt((dy * dy) / (1. - ((dx * dx) / (da * da))));
    // printf(" da=%f db=%f\n", da, db);

  // get vz from va, v(ptc,pt1):
  UT3D_vc_perp2vc (&el1->vz, &el1->va, &v1);
  UT3D_vc_setLength (&el1->vz, &el1->vz, 1.);

  // get vb (side of pt1)
  UT3D_vc_perp2vc (&el1->vb, &el1->vz, &el1->va);
  UT3D_vc_setLength (&el1->vb, &el1->vb, db);

  // UT3D_stru_dump(Typ_CVELL, el1, "ex UT3D_el_ptc_ptx_pt:");

  return 0;

}
 

//=======================================================================
  int UT3D_el_pt2vc2a (CurvElli *el, Point *ptc, Vector *vcx, Vector *vcy,
                       double a1, double a2, int idir) {
//=======================================================================
/// \code
/// UT3D_el_pt2vc2a           elli=center,vecMajAx,vecMinAx,AngStart,angEnd,dir.
/// idir: CCW (0)  od CW(1)
/// \endcode


  // UT3D_stru_dump(Typ_PT,ptc,"                CEN=");
  // UT3D_stru_dump(Typ_PT,vcx,"                VCX=");
  // UT3D_stru_dump(Typ_PT,vcy,"                VCY=");
  // printf("                a1=%f a2=%f dir=%d\n",a1,a2,idir);


  el->pc = *ptc;
  el->va = *vcx;
  el->vb = *vcy;

  UT3D_vc_perp2vc (&el->vz, vcx, vcy);

  UT3D_pt_elangd (&el->p1,ptc,vcx,vcy,a1);
  UT3D_pt_elangd (&el->p2,ptc,vcx,vcy,a2);

  el->dir = idir;

  // UT3D_stru_dump(Typ_CVELL,el"ex UT3D_el_pt2vc2a");


  return 0;

}


//================================================================
  int UT3D_pt_ck_onel (Point *pt, CurvElli *el) {
//================================================================
// UT3D_pt_ck_onel     Author: Thomas Backmeister       2.7.2003
// 
/// \code
/// UT3D_pt_ck_onel     check if point lies on an (full) ellipse
/// 
/// Check if a point lies on an (full) ellipse.
/// It is assumed that the point lies in the plane of the ellipse.
/// 
/// IN:
///   Point *pt    ... 3D-point
///   CurvElli *el  ... 3D-ellipse
/// OUT:
/// Returncodes:
///   0 = point is not on the ellipse
///   1 = point is on the ellipse
///  -1 = internal error
/// \endcode

  int rc;
  double la, d1, d2;
  Point fp1, fp2;


  // focal points of ellipse
  rc = UT3D_pt_elfoc (&fp1, &fp2, el);
  if (rc < 0) return -1;

  // length of ellipse main axis
  la = UT3D_len_vc (&(el->va));

  // distances of point to focal points
  d1 = UT3D_len_2pt (pt, &fp1);
  d2 = UT3D_len_2pt (pt, &fp2);

  if (UTP_comp2db (2.0*la, d1+d2, UT_TOL_pt)) return 1;
  else return 0;
}


//================================================================
  int UT3D_pt_elfoc (Point *fp1, Point *fp2, CurvElli *el) {
//================================================================
// UT3D_pt_elfoc     Author: Thomas Backmeister       2.7.2003
// 
/// \code
/// UT3D_pt_elfoc     focal points of ellipse
/// 
/// Computation of the focal points of an ellipse.
/// 
/// IN:
///   CurvElli *el  ... 3D-ellipse
/// OUT:
///   Point *fp1   focal point 1 (in direction of main axis); NULL = none
///   Point *fp2   focal point 2                              NULL = none
/// Returncodes:
///   0 = OK
///  -1 = input error
/// \endcode

  double la, lb, le;
  Vector iva;

  // length of ellipse axes
  la = UT3D_len_vc (&(el->va));
  lb = UT3D_len_vc (&(el->vb));

  // focal distance
  le = la * la - lb * lb;
  if (le <= 0.0) {
    TX_Error("UT3D_pt_elfoc: input error");
    return -1;
  }
  le = sqrt(le);

  // focal points
  if(fp1) UT3D_pt_traptvclen (fp1, &(el->pc), &(el->va), le);
  UT3D_vc_invert (&iva, &(el->va));
  if(fp2) UT3D_pt_traptvclen (fp2, &(el->pc), &iva, le);

  return 0;
}

/* UNUSED
//=================================================================
  int UT3D_pt_tng_ell_vc__ (Point *pto,
                            Vector *vct, CurvElli *cv1, int isol) {
//=================================================================
/// \code
/// get tangent vector-ellipse.
/// ellipse and vector must be planar to constr.Plane.
///   isol    0=solution-1; 1=solution-2;
/// 2013-04-23 Backmeister, Reiter
///     0      OK
///    -1      Error isol
///    -2      internal error
///    -3      EOM
/// \endcode


  double     aa, bb, k, kk;
  Vector2    v2t, v2ct, el2va;
  Point2     el2pc, p2co, p2o;
  CurvEll2C  elc;
  Point      pt1;
  Vector     vc1;


  printf("UT3D_pt_tng_ell_vc__ %d\n",isol);
    // UT3D_stru_dump (Typ_VC, vct, "  vct:");
    // UT3D_stru_dump (Typ_CVELL, cv1, "  bsp:");


// get ellipseCenter & vector onto constructionPlane
  pt1 = UTRA_pt_abs2rel__ (&cv1->pc);
  vc1 = UTRA_vc_abs2rel__ (vct);

// ell.center & vector -> 2D
  el2pc = UT2D_pt_pt3 (&pt1);
  UT2D_vc_vc3 (&v2t, &vc1);
    // UT3D_stru_dump (Typ_VC2, &v2t, "  v2t:");
    // GR_Disp_vc2 (&v2t, &el2pc, 7, 0);

// get majAx in 2D (not centerPos)
  vc1 = UTRA_vc_abs2rel__ (&cv1->va);    // abs -> rel
  UT2D_vc_vc3 (&el2va, &vc1);            // 3D -> 2D
    // UT3D_stru_dump (Typ_VC2, &el2va, "  el2va:");

// get vector to centerposition
  UT2D_2slen_vc_vc__ (&v2ct.dx, &v2ct.dy, &v2t, &el2va);
    // UT3D_stru_dump (Typ_VC2, &v2ct, "  v2ct:");

// normalize v2t
  UT2D_vc_setLength (&v2ct, &v2ct, 1.);
    // UT3D_stru_dump (Typ_VC2, &v2ct, " norm-v2ct:");

// get ellipse in centerposition
  elc.a = UT3D_len_vc(&cv1->va);
  elc.b = UT3D_len_vc(&cv1->vb);

  aa = elc.a * elc.a;
  bb = elc.b * elc.b;


//----------------------------------------------------------------
// compute tangentPoint

// test vector for (1,0)
  if(UT2D_ckvc_hori(&v2ct) == 0) {
      // printf(" tangent=hori\n");
    p2co.x = 0.;
    if(isol == 0) p2co.y = elc.b;
    else          p2co.y = -elc.b;
    goto L_out;
  }

// test vector for (0,1)
  if(UT2D_ckvc_vert(&v2ct) == 0) {
      // printf(" tangent=vert\n");
    if(isol == 0) p2co.x = elc.a;
    else          p2co.x = -elc.a;
    p2co.y = 0.;
    goto L_out;
  }

// vector v2ct must have format (1,k);
// set v2ct -> format (1,k);
  v2ct.dy /= v2ct.dx;
  v2ct.dx = 1.;
    // UT3D_stru_dump (Typ_VC2, &v2ct, " 1,k-v2ct:");

  k = v2ct.dy;
  kk = k * k;

// 1.solution:
  if(isol == 0) {
    p2co.x = -aa * k / sqrt(aa * kk + bb);
    p2co.y = bb / sqrt(aa * kk + bb);

// 2.solution:
  } else {
    p2co.x = aa * k / sqrt(aa * kk + bb);
    p2co.y = -bb / sqrt(aa * kk + bb);
  }


//----------------------------------------------------------------
  L_out:
// transform point p2co back from centerposition to constructionPlane
    // UT3D_stru_dump (Typ_PT2, &p2co, "  p2co:");
// p2o.x along va; p2o.y normal to va
  UT2D_pt_traptvc2len (&p2o, &el2pc, &el2va, p2co.x, p2co.y);
    // GR_Disp_pt2 (&p2o, SYM_STAR_S, 0);

// transform p2o point back to absolute
  pt1 = UT3D_pt_pt2 (&p2o);
  *pto = UTRA_pt_rel2abs__ (&pt1);

    // UT3D_stru_dump (Typ_PT, pto, "  pto:");

  return 0;

}
*/

//=================================================================
  int UT2D_vc_tang_el2 (Vector2 *vct, Point2 *pt1, CurvEll2 *el2) {
//=================================================================
/// UT2D_vc_tang_el2             tangent thru point ON 2D-ellipse


// Die Tangente an den Kreis mit Radius va und die tangente an die Elli
// gehen durch den gleichen Punkt auf der Hauptachse.
// Die Tangentenpunkte am Kreis und an der Ellipse haben gleichen X-Wert.
// - vertikal !
// see also UT3D_vc_tangel


  double    a, b;
  Vector2   vcx, vcy, vcp, vc1;
  Point2    pt2;


  printf("UT2D_vc_tang_el2 pt %lf %lf\n",pt1->x,pt1->y);
  UT3D_stru_dump (Typ_CVELL2, pt1, "UT3D_vc_tangel pt");

  // get length va, vb
  a = UT2D_len_vc (&el2->va);
  b = UT2D_len_vc (&el2->vb);

  // get va, vb with length=1
  UT2D_vc_div_d (&vcx, &el2->va, a);
  UT2D_vc_div_d (&vcy, &el2->vb, b);

  // get dx,dy of pt1 as pt2 on refsys va,vb
  UT2D_vc_2pt (&vcp, &el2->pc, pt1);
  UT2D_2par_vc_vcx_vcy (&pt2.x, &pt2.y, &vcp, &vcx, &vcy);

  // get tangent
  UT2D_vc_tang_elc (&vc1, &pt2, a);

  // transfer tangent back
  UT2D_vc_tra_vcx_vcy (vct, &vc1, &vcx, &vcy);

    printf("ex UT2D_vc_tang_el2 %lf %lf\n",vct->dx,vct->dy);

 return 0;

}


//================================================================
  int UT2D_vc_tang_elc (Vector2 *vc21, Point2 *p21, double da) {
//================================================================
/// UT2D_vc_tang_elc             tangent thru point ON 2D-ellipse/centerPos
///   da      major length

// Die Tangente an den Kreis mit Radius va und die tangente an die Elli
// gehen durch den gleichen Punkt auf der Hauptachse.
// Die Tangentenpunkte am Kreis und an der Ellipse haben gleichen X-Wert.
// - vertikal !
// see also UT3D_vc_tangel

  int      irc;
  double   x0;
  Point2   p22;

  printf("UT2D_vc_tang_elc pt %lf %lf da %lf \n",p21->x,p21->y,da);


  // get x-value where tangent to outer-circle of elli in centerPos crosses
  // the line y=0.  1=vert; 2=hor.
  irc = UT2D_ptx_tang_cic_pt (&p22.x, p21->x, da);
  if(irc == 1) *vc21 = UT2D_VECTOR_Y;
  if(irc == 2) *vc21 = UT2D_VECTOR_X;
  else {
    p22.y = 0.;
    UT2D_vc_2pt (vc21, p21, &p22);
    UT2D_vc_setLength (vc21, vc21, 1.);
  }

    printf("ex UT2D_vc_tang_elc vc1 %lf %lf\n",vc21->dx,vc21->dy);

  return 0;

}


//===============================================================
  int UT3D_vc_tangel (Vector *vc1, Point *pt1, CurvElli *el1) {
//===============================================================
/// UT3D_vc_tangel     tangent thru point ON ellipse

// Die Tangente an den Kreis mit Radius va und die tangente an die Elli
// gehen durch den gleichen Punkt auf der Hauptachse.
// der zugehoerige Punkt am Kreis ist der mit dem gleichen X-Wert.



  int    i1;
  double a, x1, dx, dy, d1;
  Vector evz;
  Point  ph, ps;


  // UT3D_stru_dump (Typ_PT, pt1, "UT3D_vc_tangel pt");


  // ph = pt1 auf die hauptachse projizieren
  UT3D_pt_projptptvc (&ph, &d1, NULL, pt1, &el1->pc, &el1->va);
  // printf(" ph=%f %f %f\n",ph.x,ph.y,ph.z);


  // dx ist der X-Abstand des Punktes auf der hauptachse
  // dx = UT3D_len_2pt (&ph, &el1->pc);
  dx = UT3D_slen_2ptvc (&el1->pc, pt1, &el1->va);
    // printf(" dx=%f\n",dx);


  if(UTP_compdb0(d1, UT_TOL_pt)) {   // Punkt auf hauptachse
    *vc1 = el1->vb;
    UT3D_vc_setLength (vc1, vc1, 1.);    // 2013-03-14
    if(dx < 0.) UT3D_vc_invert (vc1, vc1);
    return 0;
  }


  // is pt1 above or below pc-va
  UT3D_vc_perp2vc (&evz, &el1->vb, &el1->va);
  i1 = UT3D_sid_pt_pt2vc(&el1->pc, &el1->va, &evz, pt1);
  // -1=below, 1=above, 0=inPlane
    // printf(" i1=%d\n",i1);


  if(UTP_compdb0(fabs(dx), UT_TOL_pt)) {   // Punkt auf nebenachse
    *vc1 = el1->va;
    UT3D_vc_setLength (vc1, vc1, 1.);    // 2013-03-14
    if(i1 > 0) UT3D_vc_invert (vc1, vc1);
    goto L_done;
  }


  // a ist die Laenge der hauptachse,
  // printf(" va=%f %f %f\n",el1->va.dx,el1->va.dy,el1->va.dz);
  a = UT3D_len_vc (&el1->va);


  x1 = (a*a - dx*dx) / dx;
  // printf(" dx=%f a=%f x1=%f\n",dx,a,x1);


  // (dx + x1) ist der Abstand des Schnittpkt mit der Hauptachse.

  // ps = von ph weg entlang Hauptachse Abst = x1
  // UT3D_pt_traptptlen (&ps, &el1->pc, &ph, dx+x1);
  UT3D_pt_traptvclen (&ps, &el1->pc, &el1->va, dx+x1);

  // der vec geht nun von pt1 -> ps
  UT3D_vc_2pt (vc1, pt1, &ps);

  UT3D_vc_setLength (vc1, vc1, 1.);    // 2013-03-14

  // change direction in parameter-area 0.0-0.25 and 0.5 - 0.75    // 2013-03-14
  if(i1 > 0) {
    // above
    if(dx < 0.) goto L_done;
  } else {
    // below
    if(dx > 0.) goto L_done;
  }
  UT3D_vc_invert (vc1, vc1);

  L_done:
  return 0;


}


//================================================================
  int UT3D_ptNr_ell (CurvElli *el3, double tol) {
//================================================================
// UT3D_ptNr_ell                 get nr of points for ellipse

  int         pNr;
  double      as, ao, lMin;
  CurvEll2C   el2c;


  // UT3D_stru_dump (Typ_CVELL, el3, "UT3D_ptNr_ell:");


  // if(UT3D_ck_el360 (el3) == 0) { ao = RAD_360; goto L_1;}
  // ao = _UT3D_ango_el (el3);

  // make el2c from ell
  UT2D_elc_el3 (&el2c, el3);

  // get as & ao
  UT3D_2angr_el (&as, &ao, &el2c);

  L_1:
  lMin = UT3D_len_vc (&el3->vb);  // length minor axis

  pNr = UT2D_ptNr_ci (fabs(lMin), fabs(ao), tol);

    // printf("ex UT3D_ptNr_ell pNr=%d ao=%lf lMin=%lf\n",pNr,ao,lMin);

  return pNr;

}


//================================================================
  double UT3D_angr_el_corr (double aa, double da, double db) {
//================================================================
/// \code
/// UT3D_angr_el_corr     get angle-circ from angle of elli.
/// compute point on elli from corrected angle with sin|cos * lmay|lmin
/// \endcode

  int     q23, q34;
  double  ac;


  q23 = UT2D_circQuad23_angr (&aa);
  q34 = UT2D_circQuad34_angr (&aa);

  ac = fabs(atan(da * tan(aa) / db));

  if(q34) ac *= -1.;
  if(q23) ac = RAD_180 - ac;
    // printf(" UT3D_angr_el_corr ac=%lf q23=%d q34=%d\n",ac,q23,q34);
  return ac;

}

 
//================================================================
  int UT3D_npt_ell (int ptNr, Point *pa, CurvElli *el3) {
//================================================================
// UT3D_npt_ell         polygon from ellipse, fixed nr of points
// get ptNr from UT3D_ptNr_ell

  int        i1, ip, is360;
  int        q23, q34;
  double     dx, dy, xPar, yPar;
  double     as, ae, ao, ai, aa;
  double     acs, ace;
  Vector     vcx, vcy;
  Point2     pt21;
  CurvEll2C  el2c;


  // printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX \n");
  // UT3D_stru_dump (Typ_CVELL, el3, "UT3D_npt_ell %d",ptNr);
  // printf("UT3D_npt_ell %d\n",ptNr);


  if(ptNr < 2) return -1;

  // make el2c from ell
  UT2D_elc_el3 (&el2c, el3);
    // UT3D_stru_dump (Typ_CVELL2C, &el2c, "  el2c");

  // get va, vb with length=1
  UT3D_vc_div_d (&vcx, &el3->va, el2c.a);
  UT3D_vc_div_d (&vcy, &el3->vb, el2c.b);

  // get as, ao
  // as=angle at startpoint; ao=opening-angle; ao = endAngle
  UT3D_2angr_el (&as, &ao, &el2c);
  ae = as + ao;
    // printf(" _npt_ell as=%lf ae=%lf ao=%lf\n",as,ae,ao);


  // check for 360-deg 0=not, 1=yes
  is360 = UTP_comp2db (RAD_360, fabs(ao), UT_TOL_Ang1);

  // get corrected angle for as
  acs = UT3D_angr_el_corr (as, el2c.a, el2c.b);
    // printf(" acs=%lf as=%lf\n",acs,as);

  // get corrected angle for ae
  ace = UT3D_angr_el_corr (ae, el2c.a, el2c.b);
    // printf(" ace=%lf ae=%lf\n",ace,ae);

  if(is360) {
     // printf(" ell=360; sr=%d\n",el2c.sr);
   if(el2c.sr == 1) ace = acs + RAD_360;  // 1=CCW, else CW.
   else             ace = acs - RAD_360;

  } else {
    // make ace following acs
    ace = UT2D_angr_set_2angr (acs, ace, el2c.sr);
  }
    // printf(" korr.acs=%lf ace=%lf ae=%lf sr=%d\n",acs,ace,ae,el2c.sr);

  
  ao = ace - acs;
  ai = ao / (ptNr-1);
    // printf(" ao=%lf ai=%lf\n",ao,ai);


//----------------------------------------------------------------
  ip = 0;
  aa = acs;

  // copy first point
  pa[0] = el3->p1;
  --ptNr;     // catch last point - to copy
  goto L_nxtPt1;

  L_nxtPt0:
      // printf(" ip=%d aa=%lf\n",ip,aa);

    // UT2D_pt_elangd (&pt21, el2c.a, el2c.b, aa);
    // get point on 2D-elli
    pt21.x = el2c.a * cos(aa);
    pt21.y = el2c.b * sin(aa);
      // if(ao < 6)
      // GR_Disp_pt (&pt21, SYM_STAR_S, 2);

    // parametric-point (0-1): multiply va,vb, done !
    // move from pc along vcx, vcx and multiply
    UT3D_pt_tra_pt_2vc_2par (&pa[ip], &el3->pc,
                              &vcx, pt21.x,
                              &vcy, pt21.y);
      // if(ao < 6)
      // GR_Disp_pt (&pa[ip], SYM_TRI_S, ATT_COL_YELLOW);


    L_nxtPt1:
      ++ip;
      aa += ai;
      if(ip < ptNr) goto L_nxtPt0;


  // copy last point
  pa[ptNr] = el3->p2;
  ++ptNr;

    // GR_Disp_cv (pa, ptNr, 8);
    // UT3D_nstru_dump (Typ_PT, ptNr, pa, "pa:");

  return 0;

}


/* REPLACED BY UT3D_npt_ell
//================================================================
  int UT3D_cv_ell (Point cv[],int *numpt,CurvElli *el,int ptmax,double stol) {
//================================================================
// UT3D_cv_ell           Author: Thomas Backmeister         25.2.2003
// 
/// \code
/// UT3D_cv_ell           Polygon <- Ellipse
/// 
/// Polygonial curve from 3D-Ellipse.
/// 
/// IN:
///   CurvElli *el  ...  3D-ellipse
///   int ptmax     ...  maximal number of polygonpoints
///   double stol   ...  maximal deviation of secant;
///                      if(stol<0)then return fixed nr of points
///   int *numpt    ...  number of outputpoints (only if stol<0)
/// OUT:
///   Point cv[]    ...  array of polygonpoints; if NULL: return only pointNr
///   int *numpt    ...  number of polygonpoints
/// Returncodes:
///   0  = OK
///   1  = ellipse is a line (va = 0)
///   2  = ellipse is a line (vb = 0)
///   -1 = degeneration of ellipse (a = b = 0)
/// \endcode

  int     i1, rc;
  double  la, lb, lai, lbi, a1, b1, d1;
  Mat_4x3 ma, ima;
  Point   p1, p2;
  Point2  *pa2;
  Vector  v1, v2, vt, vp1, vp2, va, vb;
  Circ    ci;
  Circ2   ci2;



  // printf("UT3D_cv_ell ptmax=%d stol=%f dir=%d\n",ptmax,stol,el->dir);
  // printf(" *numpt=%d\n",*numpt);
  // UT3D_stru_dump(Typ_CVELL, el, "");

  // init
  rc = 0;

  // length of ellipse axes
  la = UT3D_len_vc (&(el->va));
  lb = UT3D_len_vc (&(el->vb));
    // printf(" la=%lf lb=%lf\n",la,lb);


  // ---------------- check special cases ------------------
  if (UTP_compdb0 (la, UT_TOL_min1)) {

    if (UTP_compdb0 (lb, UT_TOL_min1)) {
      // both vectors are nearly 0
      printf("ERROR: degeneration of ellipse: a = b = 0\n");
      return -1;


    } else {
      // ellipse is a line (va = 0)    
      rc = 1;

      if(!cv) {   // get only nr of points
        *numpt = 2;
        return rc;
      }

      // create coordinate system for "ellipse" with la=0
      UT3D_vc_perp2vc (&va, &(el->vb), &(el->vz));      
      UT3D_m3_load (ma, &va, &(el->vb), &(el->vz)); 
      // UT3D_m3_load_povxvy (ma, &UT3D_PT_NUL, &(el->va), &(el->vb));
    }


  } else {
    if (UTP_compdb0 (lb, UT_TOL_min1)) {
      // ellipse is a line (vb = 0)
      rc = 2;

      if(!cv) {   // get only nr of points
        *numpt = 2;
        return rc;
      }

      // create coordinate system for "ellipse" with lb=0
      UT3D_vc_perp2vc (&vb, &(el->vz), &(el->va));      
      UT3D_m3_load (ma, &(el->va), &vb, &(el->vz)); 
    }
  }

  if (rc > 0) {
    // ellipse = line

    // invert matrix
    UT3D_m3_invm3 (ima, ma);

    // make vectors from ellipse start- and endpoint
    UT3D_vc_2pt (&v1, &(el->pc), &(el->p1));
    UT3D_vc_2pt (&v2, &(el->pc), &(el->p2));

    // transform ellipse start- and endpoint
    UT3D_vc_travcm3 (&vt, ima, &v1);
    UT3D_pt_3db (&p1, vt.dx, vt.dy, vt.dz);
    UT3D_vc_travcm3 (&vt, ima, &v2);
    UT3D_pt_3db (&p2, vt.dx, vt.dy, vt.dz);

    if (UT3D_comp2pt (&p1, &p2, UT_TOL_pt)) {
      // endpoints equal
      if (rc == 1) {
        // a = 0
        cv[0].x = 0.0;
        cv[0].y = lb;
        cv[0].z = 0.0;
        cv[1].x = 0.0;
        cv[1].y = -lb;
        cv[1].z = 0.0;
      }
      else {
        // b = 0
        cv[0].x = la;
        cv[0].y = 0.0;
        cv[0].z = 0.0;
        cv[1].x = -la;
        cv[1].y = 0.0;
        cv[1].z = 0.0;
      }
    }
    else {
      // endpoints not equal
      if (rc == 1) {
        // a = 0
        cv[0].x = 0.0;
        cv[0].y = p1.y;
        cv[0].z = 0.0;
        cv[1].x = 0.0;
        cv[1].y = p2.y;
        cv[1].z = 0.0;
      }
      else {
        // b = 0
        cv[0].x = p1.x;
        cv[0].y = 0.0;
        cv[0].z = 0.0;
        cv[1].x = p2.x;
        cv[1].y = 0.0;
        cv[1].z = 0.0;
      }
    }
    *numpt = 2;

    // transform points to original coordinate system
    for (i1=0; i1<*numpt; ++i1) {
      UT3D_pt_traptm3 (&cv[i1], ma, &cv[i1]);
      cv[i1].x += el->pc.x;
      cv[i1].y += el->pc.y;
      cv[i1].z += el->pc.z;
    }
    return rc;
  }

  // ------------ end: check special cases -----------------
  

  // inverse length of axes
  lai = 1.0 / la;
  lbi = 1.0 / lb;

  // coordinate system of the ellipse
  UT3D_m3_load (ma, &(el->va), &(el->vb), &(el->vz)); 
  // UT3D_m3_load_povxvy (m1, &UT3D_PT_NUL, &(el->va), &(el->vb));

  // invert matrix
  UT3D_m3_invm3 (ima, ma);

  // make vectors from ellipse start- and endpoint
  UT3D_vc_2pt (&v1, &(el->pc), &(el->p1));
  UT3D_vc_2pt (&v2, &(el->pc), &(el->p2));

  // define corresponding circle in the ellipse coordinate system
  // ci.rad = lb;
  ci.rad = la * 2.0;
  ci.pc = UT3D_PT_NUL;
  ci.vz = UT3D_VECTOR_Z;

  // transform ellipse start- and endpoint
  UT3D_vc_travcm3 (&vp1, ima, &v1);
  vp1.dx *= lai;
  vp1.dy *= lbi;
  // UT3D_pt_traptvclen (&ci.p1, (Point*)&UT3D_PT_NUL, &vt, ci.rad);

  UT3D_vc_travcm3 (&vp2, ima, &v2);
  vp2.dx *= lai;
  vp2.dy *= lbi;
  // UT3D_pt_traptvclen (&ci.p2, (Point*)&UT3D_PT_NUL, &vt, ci.rad);


  // umdrehen ?
  if(el->dir < 0) {
    ci.rad  = -ci.rad;
    // MEM_swap__(&ci.p1, &ci.p2, sizeof(Point));
  }


// Version 3D
  // // openingAngle
  // if(UT3D_comp2pt(&el->p1, &el->p2, UT_TOL_pt) == 1) {
    // if(el->dir < 0) ci.ango = -RAD_360;
    // else            ci.ango = RAD_360;
  // } else {
    // ci.ango = UT3D_angr_ci (&ci);
  // }
  // // UT3D_stru_dump(Typ_AC, &ci, "f ango f elli");


  // // 2005.10.20; RF - verbessert Ungenauigkeiten ..
  // ci.p1.z = 0.;
  // ci.p2.z = 0.;

  // // polygonize circle
  // UT3D_cv_ci (cv, numpt, &ci, ptmax, stol);

  // // corresponding ellipse polygon in original coordinate system
  // a1 = la / fabs(ci.rad);
  // b1 = lb / fabs(ci.rad);
  // for (i1=0; i1<*numpt; ++i1) {
    // cv[i1].x *= a1;
    // cv[i1].y *= b1;
    // UT3D_pt_traptm3 (&cv[i1], ma, &cv[i1]);
    // cv[i1].x += el->pc.x;
    // cv[i1].y += el->pc.y;
    // cv[i1].z += el->pc.z;
      // // GR_Disp_pt (&cv[i1], SYM_STAR_S, 2);
      // // GR_Disp_txi (&cv[i1], i1, 0);
  // }
//

  // circ from center, 2 vectors & radius
  UT2D_ci_pt2vcrd (&ci2, &UT2D_PT_NUL, &vp1, &vp2, &ci.rad);
    UT3D_stru_dump (Typ_CI2, &ci2, " ell-ci2=");

  if(UT3D_comp2pt(&el->p1, &el->p2, UT_TOL_pt) == 1) {
    if(el->dir < 0) ci.ango = -RAD_360;
    else            ci.ango = RAD_360;
  }

  // nr of outputpoints
  if(stol > 0.0) {
    *numpt = UT2D_ptNr_ci (fabs(ci2.rad), fabs(ci2.ango), stol);
      // printf(" ell-rad=%lf ango=%lf tol=%lf\n",
        // fabs(ci2.rad),fabs(ci2.ango),stol);

  }
  if(*numpt > ptmax) {
    printf("UT3D_cv_ell I001 reduced\n");
    *numpt = ptmax;
  }
    // printf(" ell-numpt=%d\n",*numpt);

  // only get pointNr: done ..
  if(!cv) return 0;

  i1 = *numpt * sizeof(Point2);
  if(i1 < sizeof(Point2)) {printf("***** UT3D_cv_ell E001\n");return -1;}
  pa2 = (Point2*) MEM_alloc_tmp (i1);

  // circ -> polygon
  UT2D_cv_ci_ (pa2, numpt, &ci2, *numpt, -1.);

//
  // *numpt = ptmax;
  // d1 = stol * 10000.;
  // i1 = -d1;
  // pa2 = (void*)cv;
  // UT2D_cv_cin (numpt, pa2,
        // (Point2*)&ci.p1,
        // (Point2*)&ci.p2,
        // (Point2*)&ci.pc, ci.rad, i1);
//

  // corresponding ellipse polygon in original coordinate system
  a1 = la / fabs(ci.rad);
  b1 = lb / fabs(ci.rad);
  p1.z = 0.;
  // verkehrt; sonst werden 2D-Punkte ueberschrieben
  for (i1 = *numpt-2; i1 > 0; --i1) {   
    // UT3D_stru_dump (Typ_PT2, &pa2[i1], "P%d:\n",i1);
    p1.x = pa2[i1].x * a1;
    p1.y = pa2[i1].y * b1;
    UT3D_pt_traptm3 (&cv[i1], ma, &p1);
    cv[i1].x += el->pc.x;
    cv[i1].y += el->pc.y;
    cv[i1].z += el->pc.z;
      // GR_Disp_pt (&cv[i1], SYM_STAR_S, 2);
      // GR_Disp_txi (&cv[i1], i1, 0);
  }

  // replace 1. u last point
  cv[0] = el->p1;
  cv[*numpt-1] = el->p2;



  //-- TESTDISP ----------------------------------------------------
  // printf("ex UT3D_cv_ell %d\n",*numpt);
  // for(i1=0;i1<*numpt; ++i1) {
    // GR_Disp_pt (&cv[i1], SYM_STAR_S, 2);
    // GR_Disp_txi (&cv[i1], i1, 0); }




  return 0;
}
*/

//================================================================
  int UT3D_el_projelpl (CurvElli *elo, Plane *pl, CurvElli *eli) {
//================================================================
/// \code
/// UT3D_el_projelpl          ellipse = project ELL -> PLN
/// 
/// see also UT3D_el_projcipl UPRJ_app_ci
/// 
/// \endcode

	Point     pt1, pt2;


/*
  printf("UT3D_el_projcipl pc=%f,%f,%f\n",eli->pc.x,eli->pc.y,eli->pc.z);
  printf("        p1=%f,%f,%f\n",eli->p1.x,eli->p1.y,eli->p1.z);
  printf("        p2=%f,%f,%f\n",eli->p2.x,eli->p2.y,eli->p2.z);
  printf("        va=%f,%f,%f\n",eli->va.dx,eli->va.dy,eli->va.dz);
  printf("        vb=%f,%f,%f\n",eli->vb.dx,eli->vb.dy,eli->vb.dz);
  printf("        vz=%f,%f,%f\n",eli->vz.dx,eli->vz.dy,eli->vz.dz);
*/



  // points of ellipse
  UT3D_pt_projptpl (&(elo->pc), pl, &(eli->pc));
  UT3D_pt_projptpl (&(elo->p1), pl, &(eli->p1));
  UT3D_pt_projptpl (&(elo->p2), pl, &(eli->p2));

	UT3D_pt_traptvc (&pt1, &(eli->pc), &eli->va);
  UT3D_pt_projptpl (&pt2, pl, &pt1);
	UT3D_vc_2pt (&(elo->va), &(elo->pc), &pt2);

	UT3D_pt_traptvc (&pt1, &(eli->pc), &eli->vb);
  UT3D_pt_projptpl (&pt2, pl, &pt1);
	UT3D_vc_2pt (&(elo->vb), &(elo->pc), &pt2);


	// UT3D_vc_projvcpl (&(elo->va), pl, &(eli->va));
	// UT3D_vc_projvcpl (&(elo->vb), pl, &(eli->vb));

	elo->vz = pl->vz;


/* 
  printf("ex UT3D_el_projcipl pc=%f,%f,%f\n",elo->pc.x,elo->pc.y,elo->pc.z);
  printf("        p1=%f,%f,%f\n",elo->p1.x,elo->p1.y,elo->p1.z);
  printf("        p2=%f,%f,%f\n",elo->p2.x,elo->p2.y,elo->p2.z);
  printf("        va=%f,%f,%f\n",elo->va.dx,elo->va.dy,elo->va.dz);
  printf("        vb=%f,%f,%f\n",elo->vb.dx,elo->vb.dy,elo->vb.dz);
  printf("        vz=%f,%f,%f\n",elo->vz.dx,elo->vz.dy,elo->vz.dz);
*/


  return 0;

}


//================================================================
  int UT3D_el_projcipl (CurvElli *el, Plane *pl, Circ *ci) {
//================================================================
// UT3D_el_projcipl          Author: Thomas Backmeister         26.3.2003
// 
/// \code
/// UT3D_el_projcipl          ellipse = project CIR -> PLN
/// 
/// Project a circle onto a plane.
/// 
/// IN:
///   Plane *pl    ... projection plane
///   Circ *ci     ... 3D - circle
/// OUT:
///   CurvElli *el  ... ellipse
/// Returncodes:
///   0 = OK
/// 
/// see also UPRJ_app_el
/// \endcode

  int    rc;
  Point  pa, pb;
  Vector va, vb;
  Line   ln;
  Plane  plci;


  // UT3D_stru_dump (Typ_AC, ci, "UT3D_el_projcipl:");


  // determine plane of circle
	UT3D_pl_ptvc (&plci, &(ci->pc), &(ci->vz));

  // intersect plane with plane of circle
  rc = UT3D_ln_int2pl (&ln, pl, &plci);

  if (rc == 1) {
    // planes parallel --> ellipse = circle

    // projection of ci->p1 as 1. vertex point 
    pa = ci->p1;

    // vector ci->pc nach pa
    UT3D_vc_2pt (&va, &(ci->pc), &pa);

    // point on circle whose projection is 2. vertex point
    UT3D_vc_perp2vc (&vb, &(ci->vz), &va);
    UT3D_pt_traptvclen (&pb, &(ci->pc), &vb, ci->rad);
  }
  else {
    // planes not parallel

    // point on circle whose projection is 1. vertex point
    UT3D_vc_ln (&va, &ln);
    UT3D_pt_traptvclen (&pa, &(ci->pc), &va, ci->rad);

    // point on circle whose projection is 2. vertex point
    UT3D_vc_perp2vc (&vb, &(ci->vz), &va);
    UT3D_pt_traptvclen (&pb, &(ci->pc), &vb, ci->rad);
  }  

  // center point of ellipse
  UT3D_pt_projptpl (&(el->pc), pl, &(ci->pc));

  // start point on ellipse
  UT3D_pt_projptpl (&(el->p1), pl, &(ci->p1));

  // end point on ellipse
  UT3D_pt_projptpl (&(el->p2), pl, &(ci->p2));

  // normal vector to ellipse
  el->vz = pl->vz;

  // vector from center point to 1. vertex point
  UT3D_pt_projptpl (&pa, pl, &pa);
  UT3D_vc_2pt (&(el->va), &(el->pc), &pa);

  // vector from center point to 2. vertex point
  UT3D_pt_projptpl (&pb, pl, &pb);
  UT3D_vc_2pt (&(el->vb), &(el->pc), &pb);

  // revers ? RF 2006-04-15
  if(ci->rad < 0.) {
    // el->dir = -1;
    MEM_swap__ (&el->p1, &el->p2, sizeof(Point));
  // } else {
    // el->dir = 1;
  }


  // UT3D_stru_dump (Typ_CVELL, el, "ex UT3D_el_projcipl:\n");

  return 0;
}


//================================================================
  int UT3D_pt_projptel (int *numpe, Point *pe, CurvElli *ell, Point *pt) {
//================================================================
// UT3D_pt_projptel     Author: Thomas Backmeister         14.3.2003
// 
/// \code
/// UT3D_pt_projptel               proj PT -> CRV-Ellipse
/// 
/// Project a point onto an ellipse.
/// All possible cases are considered!
/// 
/// IN:
///   CurvElli *ell ... a 3D - ellipse
///   Point *pt    ... a 3D - point
/// OUT:
///   int *numpe   ... number of points     (0-4 points out)
///   Point *pe    ... points on the ellipse all having the same minimal 
///                    distance to the input point
/// Returncodes:
///   0 = OK
///   1 = input error
///
/// TODO: gives only one solution !
/// \endcode

  int     i1, ind;
  double  a, b, u, v, h1, h2, e2;
  double  u2, v2, a2, a4, b2, b4;
  double  scf, tmax;
  Point   pp;
  Plane   ple;
  Mat_4x3 ma, ima;
  polcoeff_d4 pol;
  dcomplex zero[4];



/*
  printf("UT3D_pt_projptel: pt=%f,%f,%f\n",pt->x,pt->y,pt->z);
  printf("        pc=%f,%f,%f\n",ell->pc.x,ell->pc.y,ell->pc.z);
  printf("        p1=%f,%f,%f\n",ell->p1.x,ell->p1.y,ell->p1.z);
  printf("        p2=%f,%f,%f\n",ell->p2.x,ell->p2.y,ell->p2.z);
  printf("        va=%f,%f,%f\n",ell->va.dx,ell->va.dy,ell->va.dz);
  printf("        vb=%f,%f,%f\n",ell->vb.dx,ell->vb.dy,ell->vb.dz);
*/



  // init
  *numpe = 0;

	//
  // determine plane of ellipse
  // UT3D_pl_ell (&ple, ell);
	UT3D_pl_ptvc (&ple, &(ell->pc), &(ell->vz));


  // project point onto plane of ellipse
  UT3D_pt_projptpl (&pp, &ple, pt);

  // point = vector from center of ellipse to itself
  pp.x -= ell->pc.x;
  pp.y -= ell->pc.y;
  pp.z -= ell->pc.z;

  // coordinate system of the ellipse
  UT3D_m3_load (ma, &(ell->va), &(ell->vb), &(ell->vz)); 

  // invert matrix
  UT3D_m3_invm3 (ima, ma);

  // transform point
  UT3D_pt_traptm3 (&pp, ima, &pp);
  u = pp.x;
  v = pp.y; 

  // length of ellipse axes
  a = UT3D_len_vc (&(ell->va));
  b = UT3D_len_vc (&(ell->vb));
  if (UTP_compdb0 (a, UT_TOL_min1)) return 1;
  if (UTP_compdb0 (b, UT_TOL_min1)) return 1;

  if (UTP_compdb0 (v, UT_TOL_min1)) {
    a2 = a * a;
    e2 = a2 - b * b;
    if (UTP_compdb0 (u, UT_TOL_min1)) {
      // point = ellipse center point
      pe[0].x = 0.0;
      pe[0].y = b;
      pe[0].z = 0.0;
      pe[1].x = 0.0;
      pe[1].y = -b;
      pe[1].z = 0.0;
      *numpe = 2;
      if (UTP_compdb0 ((a-b), UT_TOL_min1)) {
        // ellipse is a circle
        pe[2].x = a;
        pe[2].y = 0.0;
        pe[2].z = 0.0;
        pe[3].x = -a;
        pe[3].y = 0.0;
        pe[3].z = 0.0;
        *numpe = 4;
      }
      goto L_AbsCoord;
    }
    else if (fabs(u) < (e2/a) + UT_TOL_min1) {
      // point on inner main axis inside (-e2/a, e2/a)
      if (!UTP_compdb0 ((a-b), UT_TOL_min1)) {
        // no circle
        if (!UTP_compdb0 (fabs(e2), UT_TOL_min2)) {
          pe[0].x = (a2 * u) / e2;
          h1 = a2 - pe[0].x * pe[0].x;
          if (h1 < 0.0) h1 = 0.0;
          pe[0].y = (b/a) * sqrt(h1);
          pe[0].z = 0.0;
          pe[1].x = pe[0].x;
          pe[1].y = -pe[0].y;
          pe[1].z = 0.0;
          *numpe = 2;
          goto L_AbsCoord;
        }
      }
    }
  }
        
  // scale down u, v, a, b
  UTM_scale_4db (&scf, &a, &b, &u, &v, 1.0);

  // coefficients of quartic polynomial
  u2 = u * u;
  v2 = v * v;
  a2 = a * a;
  a4 = a2 * a2;
  b2 = b * b;
  b4 = b2 * b2;
  pol.a = 1;
  pol.b = 2 * a2 + 2 * b2;
  pol.c = a4 + 4 * a2 * b2 + b4 - a2 * u2 - b2 * v2;
  pol.d = 2 * a4 * b2 + 2 * a2 * b4 - 2 * a2 * b2 * u2 - 2 * a2 * b2 * v2;
  pol.e = a4 * b4 - a2 * b4 * u2 - a4 * b2 * v2;

  // zeros of quartic polynomial
  UTM_zeros_quarticpol (zero, &pol);

  // closest point on ellipse
  tmax = UT_VAL_MIN;
  ind = -1;
  for (i1=0; i1<4; ++i1) {
    if (!UTP_compdb0 (zero[i1].b, UT_TOL_min1)) continue;
    h1 = a2 + zero[i1].a;
    h2 = b2 + zero[i1].a;
    if (UTP_compdb0 (h1, UT_TOL_min2)) continue;
    if (UTP_compdb0 (h2, UT_TOL_min2)) continue;
    if (zero[i1].a > tmax) {
      tmax = zero[i1].a;
      ind = i1;
    }
  }

  if (ind >= 0) {
    h1 = a2 + tmax;
    h2 = b2 + tmax;
    pe[0].x = (1.0/scf) * (a2 * u) / h1;
    pe[0].y = (1.0/scf) * (b2 * v) / h2;
    pe[0].z = 0.0;
    *numpe = 1;
  }

L_AbsCoord:

  // point(s) in absolute coordinates
  for (i1=0; i1<(*numpe); ++i1) {
    UT3D_pt_traptm3 (&pe[i1], ma, &pe[i1]);
    pe[i1].x += ell->pc.x;
    pe[i1].y += ell->pc.y;
    pe[i1].z += ell->pc.z;
  }


    // TEST
    // for (i1=0; i1<(*numpe); ++i1)
    // UT3D_stru_dump (Typ_PT, &pe[i1], " pe[%d]",i1);
    // printf("ex UT3D_pt_projptel\n");
    // END TEST



  return 0;
}



//================================================================
int UT3D_el_elcoe(CurvElli *obj,polcoeff_d5 *ec,Point2 *pa,Point2 *pe,double zt){
//================================================================
// UT3D_el_elcoe           Author: Thomas Backmeister         9.4.2003
// 
/// \code
/// UT3D_el_elcoe           Ellipse <-- coeff-data (IGES)
/// 
/// 
/// ellipse from IGES-data of type 104.
/// 
/// pa,pe  Beginn/Endpunkt der Elli in 2D-Zentrumslage (Center ist 0,0)
/// zt = Hoehe der Z-Plane der Elli im 2D
/// 
/// 
/// IN:
///   IgesConic *ig   ... IGES-data of type 104 (conic arc entity)
/// 
/// OUT:
///   Ellipse
/// 
/// Returncodes:
///   0 = OK and conic is ellipse
///  -1 = input error: data are no conic arc entity
/// \endcode

  static  CurvElli el;

  double b2, d2, e2, q1, q2, q3;
  Mat_3x2 ma, ima;
  Vector2 vx, vy, vt, va, vb, v0;
  Vector  v1, v2;



  // printf("UT3D_el_elcoe\n");
  // printf(" a=%f b=%f c=%f d=%f e=%f f=%f\n",
                     // ec->a,ec->b,ec->c,ec->d,ec->e,ec->f);


  // parameters q1, q2, q3
  b2 = ec->b / 2.0;
  d2 = ec->d / 2.0;
  e2 = ec->e / 2.0;
  q1 = (ec->a * ec->c * ec->f) + (b2 * e2 * d2) + (d2 * b2 * e2) -
       (d2 * ec->c * d2) - (ec->a * e2 * e2) - (b2 * b2 * ec->f);
  q2 = ec->a * ec->c - b2 * b2;
  q3 = ec->a + ec->c;
    // printf(" q1=%f q2=%f q3=%f\n",q1,q2,q3);

  if (q2 > 0.0) {
    if (q1 * q3 < 0.0) {
      // IGES-data describe an ellipse
      goto L_Ellipse;
    }
  }
/*
  else if (q2 < 0.0) {
    if (q1 != 0.0) {
      // IGES-data describe a hyperbola
      goto L_Hyperbola;
    }
  }
  else {
    if (q1 != 0.0) {
      // IGES-data describe a parabola
      goto L_Parabola;
    }
  }
*/
  printf("UT3D_el_elcoe input data errorn");
  return -1;



  //----------------------------------------------------
  L_Ellipse:

  // transformation of the ellipse to its main axes
  vx.dx = 1.0;
  vx.dy = 0.0;
  vy.dx = b2 / ec->a;
  vy.dy = 1.0;
  vt.dx = d2 / ec->a;
  vt.dy = (2.0 * ec->a * ec->e - ec->b * ec->d) / (4.0 * q2); 
    // printf(" vx = %f %f\n",vx.dx,vx.dy);
    // printf(" vy = %f %f\n",vy.dx,vy.dy);
    // printf(" vt = %f %f\n",vt.dx,vt.dy);
  UT2D_m2_loadtravcm2 (ma, &vx, &vy, &vt);
    // UT3D_stru_dump (Typ_M3x2, ma, "_elcoe/ma\n");

  // main axis vector
  va.dx = sqrt (-q1 / (ec->a * q2));
  va.dy = 0.0;
    // printf(" va = %f %f\n",va.dx,va.dy);

  // secondary axis vector
  vb.dx = 0.0;
  vb.dy = sqrt ((-q1 * ec->a) / (q2 * q2));
    // printf(" vb = %f %f\n",vb.dx,vb.dy);

  // inverse transformation
  UT2D_m2_invtravcm2 (ima, ma);

  // apply inverse trafo to origin, main and secondary axis vector
  UT2D_vc_travcm2 (&v0, ima, (Vector2*)&UT2D_VECTOR_NUL);
  // UT2D_vc_travcm2 (&va, ima, &va);    // raus 2005-06-08 RF
  // UT2D_vc_travcm2 (&vb, ima, &vb);    // raus 2005-06-08 RF
  
  // ellipse data
  el.pc.x = v0.dx;
  el.pc.y = v0.dy;
  el.pc.z = zt;

  el.va.dx = va.dx;
  el.va.dy = va.dy;
  el.va.dz = 0.0;

  el.vb.dx = vb.dx;
  el.vb.dy = vb.dy;
  el.vb.dz = 0.0;

  el.vz = UT3D_VECTOR_Z;

  el.p1.x = pa->x;
  el.p1.y = pa->y;
  el.p1.z = zt;

  el.p2.x = pe->x;
  el.p2.y = pe->y;
  el.p2.z = zt;


  // make vectors from ellipse start- and endpoint
  UT3D_vc_2pt (&v1, &(el.pc), &(el.p1));
  UT3D_vc_2pt (&v2, &(el.pc), &(el.p2));

  UT3D_pt_traptvc (&(el.p1), &(el.pc), &v1);
  UT3D_pt_traptvc (&(el.p2), &(el.pc), &v2);


  // UT3D_stru_dump (Typ_CVELL, &el, "");

  *obj = el;

  return 0;


}


//================================================================
  int UT3D_elcoe_el (polcoeff_d5 *ig, CurvElli *el) {
//================================================================
// UT3D_elcoe_el             Author: Thomas Backmeister         18.4.2003
// 
/// \code
/// UT3D_elcoe_el             CoeffEllipse(IGES) <-- ellipse
/// 
/// the ellipse lies in a plane parallel x,y-plane and its axes need not be 
/// parallel to x- resp. y- axis.
/// 
/// IN:
///   CurvElli *el   ...  ellipse in a plane parallel x,y-plane
/// 
/// OUT:
///   polcoeff_d5   ...  IGES-coefficients of the ellipse equation
/// 
/// Returncode:
///   0 = OK
///  -1 = input error
/// \endcode

// IGES-data of type 104 (conic arc entity) from an ellipse in general 2D-position,
  double la,lb,a2,b2;



  // printf("UT3D_elcoe_el: IGES-data <-- ellipse\n");
  // printf("el->p1=%f,%f,%f\n",el->p1.x,el->p1.y,el->p1.z);
  // printf("el->p2=%f,%f,%f\n",el->p2.x,el->p2.y,el->p2.z);
  // printf("el->pc=%f,%f,%f\n",el->pc.x,el->pc.y,el->pc.z);
  // printf("el->vz=%f,%f,%f\n",el->vz.dx,el->vz.dy,el->vz.dz);
  // printf("el->va=%f,%f,%f\n",el->va.dx,el->va.dy,el->va.dz);
  // printf("el->vb=%f,%f,%f\n",el->vb.dx,el->vb.dy,el->vb.dz);




  // length of ellipse axes
  la = UT3D_len_vc (&(el->va));
  lb = UT3D_len_vc (&(el->vb));

  // check la, lb
  if (la == 0.0 || lb == 0.0) {
    printf("Input Error: degenerate ellipse !\n");
    return -1;
  }


/*
  // some values
  a2 = la * la;
  b2 = lb * lb;

  printf(" la=%f lb=%f a2=%f b2=%f\n",la,lb,a2,b2);

  // IGES-coefficients
  ig->a = (b2/a2) * (el->va.dx * el->va.dx) + (el->va.dy * el->va.dy);
  ig->b = (2*lb/la)*(el->va.dx * el->vb.dx) + (2*la/lb)*(el->va.dy * el->vb.dy);
  ig->c = (el->vb.dx * el->vb.dx) + (a2/b2)*(el->vb.dy * el->vb.dy);
  ig->d = (2*b2/la)*(el->pc.x * el->va.dx) + (2*la)*(el->pc.y * el->va.dy);
  ig->e = (2*lb)*(el->pc.x * el->vb.dx) + (2*a2/lb)*(el->pc.y * el->vb.dy);
  ig->f = (b2*el->pc.x*el->pc.x) + (a2*el->pc.y*el->pc.y) - (a2*b2); 
*/


  ig->a = 1.0 / (la * la);
  ig->b = 0.0;
  ig->c = 1.0 / (lb * lb);
  ig->d = 0.0;
  ig->e = 0.0;
  ig->f = -1.0;


  // printf(" a=%f b=%f c=%f d=%f e=%f f=%f\n",
           // ig->a,ig->b,ig->c,ig->d,ig->e,ig->f);
  
  return 0;
}


//======================================================================
  int UT3D_pt_intplell (int *pNr, Point *pa, Plane *pln, CurvElli *el) {
//======================================================================
/// UT3D_pt_intplell          intersect plane ellipse
 
  Point  ptl;
  Vector vcl;
  Line   ln;

  // intersect 2 planes
  UT3D_ptvc_int2pln (&ptl,&vcl, &pln->po,&pln->vz, &el->pc,&el->vz);
    // GR_Disp_vc (&vcl, &ptl, 9, 0);

  UT3D_ln_ptvc (&ln, &ptl, &vcl);

  return UT3D_pt_intlnel_p (pNr, pa, &ln, el);

}


//================================================================
  int UT3D_pt_intlnel__ (int *np, Point xp[], Line *ln, CurvElli *el) {
//================================================================
// UT3D_pt_intlnel__     Author: Thomas Backmeister       2.7.2003
// 
/// \code
/// UT3D_pt_intlnel__     intersect (unlim.) line and ellipse
/// 
/// Intersection of line and ellipse.
/// The line need NOT lie in the plane of the ellipse.
/// 
/// IN:
///   Line *ln     ... 3D-line
///   CurvElli *el  ... 3D-ellipse
/// OUT:
///   int *np      ... number of intersection points (0,1,2)
///   Point xp[]   ... array of intersection points
/// Returncodes:
///   0 = OK
///   1 = no intersection
///  -1 = ellipse degenerated
/// \endcode

  int    rc;
  double dist;
  Point  ip;
  Plane  pl;

  // init
  *np = 0;


  // plane of ellipse
  UT3D_pl_ptvc (&pl, &(el->pc), &(el->vz));

  // check line || plane of ellipse
  rc = UT3D_ln_ck_parpl (&dist, ln, &pl, UT_TOL_PAR);


  if (rc == 0) {
    // --- line NOT || plane of ellipse ---
    // intersect line with plane of ellipse
    rc = UT3D_pt_intlnpl (&ip, &pl, ln);
    if (rc == 0) return 1;

    // intersection point is on ellipse --> save it
    if (UT3D_pt_ck_onel (&ip, el)) {
      xp[*np] = ip;
      ++(*np);
    }
    else return 1;

  //----------------------------------------------------------------
  } else {
    // --- line || plane of ellipse ---
    if(fabs(dist) > UT_TOL_cv) return 1;
    // intersect line with ellipse
    // return (UT3D_pt_intlnel_p (np, xp, ln, el));
    rc = UT3D_pt_intlnel_p (np, xp, ln, el);
    return rc;
  }

  return 0;
}


//================================================================
  int UT3D_pt_intlnel_p (int *np, Point xp[], Line *ln, CurvElli *el) {
//================================================================
// UT3D_pt_intlnel_p       Author: Thomas Backmeister         29.4.2003
// 
/// \code
/// UT3D_pt_intlnel_p           intersection LN-ELL (gives 0/1/2 points)
///                             both unlimited !
/// 
///         !! The line must lie in the plane of the ellipse. !!
/// 
/// Intersection of 3D-line with 3D-ellipse.
/// !! The line must lie in the plane of the ellipse. !!
/// 
/// IN:
///   Line *ln    ...  3D-line
///   CurvElli *el ...  3D-ellipse
/// OUT:
///   int *np    ...  number of intersection points (0,1,2)
///   Point xp[] ...  array of intersection points
/// Returncodes:
///   0 = OK
///   1 = no intersection
///  -1 = ellipse degenerated
/// \endcode

  int     irc, i1;
  double  a, b, k, d, a2, b2, bam, disc;
  double  h1, h2, h3, h4;
  Mat_4x3 ma, ima;
  Line2   ln2;
  Point   p1, p2;

  //printf("UT3D_pt_intlnel_p: intersection of 3D-line with 3D-ellipse\n");
  /*
  printf("Line: p1= %f %f %f   p2= %f %f %f\n",ln->p1.x,ln->p1.y,ln->p1.z,
                                               ln->p2.x,ln->p2.y,ln->p2.z);
  printf("Ellipse: pc=%f,%f,%f\n",el->pc.x,el->pc.y,el->pc.z);
  printf("         p1=%f,%f,%f\n",el->p1.x,el->p1.y,el->p1.z);
  printf("         p2=%f,%f,%f\n",el->p2.x,el->p2.y,el->p2.z);
  printf("         va=%f,%f,%f\n",el->va.dx,el->va.dy,el->va.dz);
  printf("         vb=%f,%f,%f\n",el->vb.dx,el->vb.dy,el->vb.dz);
  printf("         vz=%f,%f,%f\n",el->vz.dx,el->vz.dy,el->vz.dz);
  */

  // init
  *np = 0;

  // length of ellipse axes
  a = UT3D_len_vc (&(el->va));
  b = UT3D_len_vc (&(el->vb));

  // check ellipse
  if (UTP_compdb0 (a, UT_TOL_min1)) {
    if (UTP_compdb0 (b, UT_TOL_min1)) {
      printf("ERROR: degeneration of ellipse: a = b = 0\n");
      irc = -1;
      goto L_fertig;
    }
  }

  // coordinate system of the ellipse
  UT3D_m3_load (ma, &(el->va), &(el->vb), &(el->vz));
  UT3D_m3_load_o (ma, &(el->pc));

  // invert matrix
  UT3D_m3_invm3 (ima, ma);

  // transform line points
  UT3D_pt_traptm3 (&p1, ima, &(ln->p1));
  UT3D_pt_traptm3 (&p2, ima, &(ln->p2));

  // 2D-line
  ln2.p1.x = p1.x;
  ln2.p1.y = p1.y;
  ln2.p2.x = p2.x;
  ln2.p2.y = p2.y;

  // line equation y = kx + d
  irc = UT2D_lncoe_ln (&k, &d, &ln2);

  if (irc == 1) {
    // line parallel secondary ellipse axis
    
    if (UTP_comp2db (fabs(p1.x), a, UT_TOL_min1)) {
      // 1 common point
      *np = 1;
      xp[0].x = p1.x;
      xp[0].y = 0.0;
      xp[0].z = 0.0;
    }
    else if (fabs(p1.x) > a) {
      // no intersection
      irc = 1;
      goto L_fertig;
    }
    else {
      // 2 intersection points
      *np = 2;
      h1 = (b / a) * sqrt(a * a - p1.x * p1.x);
      xp[0].x = p1.x;
      xp[0].y = h1;
      xp[0].z = 0.0;
      xp[1].x = p1.x;
      xp[1].y = -h1;
      xp[1].z = 0.0;
    }
  }
  else {
    
    // some values
    a2 = a * a;
    b2 = b * b;
    bam = b2 + a2 * k * k;
    disc = bam - d * d;

    if (UTP_compdb0 (disc, UT_TOL_min1)) {
      // 1 common point
      *np = 1;
      xp[0].x = -(a2 * k * d) / bam;
      xp[0].y = (b2 * d) / bam;
      xp[0].z = 0.0;
    }
    else if (disc < 0.0) {
      // no intersection
      irc = 1;
      goto L_fertig;
    }
    else {
      // 2 intersection points
      *np = 2;
      disc = sqrt(disc);
      h1 = a * b * disc;
      h2 = a2 * k * d;
      h3 = b2 * d;
      h4 = a * b * k * disc;
      xp[0].x = (h1 - h2) / bam;
      xp[0].y = (h3 + h4) / bam;
      xp[0].z = 0.0;
      xp[1].x = (-h1 - h2) / bam;
      xp[1].y = (h3 - h4) / bam;
      xp[1].z = 0.0;
    }
  }

  // transform intersection points to original coordinate system
  for (i1=0; i1<*np; ++i1) {
    UT3D_pt_traptm3 (&xp[i1], ma, &xp[i1]);
  }


  L_fertig:

  // nur Testausg:
  /*
  printf("ex UT3D_pt_intlnel_p %d\n",irc);
  if(*np > 0) printf("     %f %f %f\n",xp[0].x,xp[0].y,xp[0].z);
  if(*np > 1) printf("     %f %f %f\n",xp[1].x,xp[1].y,xp[1].z);
  */

  return irc;
}


//================================================================
  int UT2D_pt_int_elc_ln (Point2 *p1, Point2 *p2,
                           Line2 *ln1, CurvEll2C *el1) {
//================================================================
// intersection of ellipse - line
// ellipse is in centerPosition


  int     irc, i1;
  double  a, b, k, d, a2, b2, bam, disc;
  double  h1, h2, h3, h4;
  Mat_4x3 ma, ima;
  Line2   ln2;



    // length of ellipse axes
  a = el1->a;
  b = el1->b;

  // check ellipse
  if (UTP_compdb0 (a, UT_TOL_min1)) {
    if (UTP_compdb0 (b, UT_TOL_min1)) {
      printf("ERROR: degeneration of ellipse: a = b = 0\n");
      irc = -1;
      goto L_fertig;
    }
  }


  // line equation y = kx + d
  irc = UT2D_lncoe_ln (&k, &d, ln1);

  if (irc == 1) {
    // line parallel secondary ellipse axis

    if (UTP_comp2db (fabs(ln1->p1.x), a, UT_TOL_min1)) {
      // 1 common point
      irc = 1;
      p1->x = ln1->p1.x;
      p1->y = 0.0;
      // p1->z = 0.0;
    }
    else if (fabs(ln1->p1.x) > a) {
      // no intersection
      irc = 0;
      goto L_fertig;
    }
    else {
      // 2 intersection points
      irc = 2;
      h1 = (b / a) * sqrt(a * a - ln1->p1.x * ln1->p1.x);
      p1->x = ln1->p1.x;
      p1->y = h1;
      // p1->z = 0.0;
      p2->x = ln1->p1.x;
      p2->y = -h1;
      // p2->z = 0.0;
    }
  }
  else {

    // some values
    a2 = a * a;
    b2 = b * b;
    bam = b2 + a2 * k * k;
    disc = bam - d * d;

    if (UTP_compdb0 (disc, UT_TOL_min1)) {
      // 1 common point
      irc = 1;
      p1->x = -(a2 * k * d) / bam;
      p1->y = (b2 * d) / bam;
      // p1->z = 0.0;
    }
    else if (disc < 0.0) {
      // no intersection
      irc = 0;
      goto L_fertig;
    }
    else {
      // 2 intersection points
      irc = 2;
      disc = sqrt(disc);
      h1 = a * b * disc;
      h2 = a2 * k * d;
      h3 = b2 * d;
      h4 = a * b * k * disc;
      p1->x = (h1 - h2) / bam;
      p1->y = (h3 + h4) / bam;
      // p1->z = 0.0;
      p2->x = (-h1 - h2) / bam;
      p2->y = (h3 - h4) / bam;
      // p2->z = 0.0;
    }
  }

  L_fertig:

    // if(irc >= 1) UT3D_stru_dump (Typ_PT2, p1, "  pt1:");
    // if(irc >= 2) UT3D_stru_dump (Typ_PT2, p2, "  pt2:");

  return irc;

}
  

//================================================================
  int UT3D_vc_tng_elpt__ (Vector *vco,
                          Point *pt1, CurvElli *el1, int isol) {
//================================================================
/// \code
/// UT3D_vc_tng_elpt           tangents point - ellipse
/// 2013-04-17 Backmeister-Reiter
/// Input:
///   isol    0=solution-1; 1=solution-2;
/// RetCod:
///     1      OK, point is on ellipse
///     0      OK, point is outside ellipse
///    -1      Error: pt1 inside ellipse
/// \endcode
 
// see also UT3D_vc_tangel

  int        irc, sidMaj, sidMin;
  double     a, aa, b, bb, c, y;
  Point      po;
  Point2     pt, pi1, pi2;
  Line2      ln21;
  Vector     vna, vnb;    // axes of elli - normalized
  Vector     vcp, vhx, vhy;
  CurvEll2C  el2c;        // ellipse in centerPosition


  // printf("UT3D_vc_tng_elpt__ \n");
  // UT3D_stru_dump (Typ_PT, pt1, "  pt:");
  // UT3D_stru_dump (Typ_CVELL, el1, "  ell:");


  UT3D_vc_2pt (&vcp, &el1->pc, pt1);

// get sidMaj = above or below plane tru pln.vx=elli.va; pln.vz=elli.vb
// 0=normal; 1=quad. 1,2; -1=quad. 3,4
  sidMaj = UT3D_sid_2vc (&el1->vb, &vcp);

// get sidMin = above or below plane tru pln.vx=elli.vb; pln.vz=elli.va
// 0=normal; 1=quad. 1,4; -1=quad. 2,3
  sidMin = UT3D_sid_2vc (&el1->va, &vcp);
    // printf(" sidMaj=%d sidMin=%d\n",sidMaj,sidMin);

  if(sidMaj == 0) {
    // pt1 is on majorAxis
    return UT3D_vc_tng_elptMaj (vco, pt1, el1, sidMin, isol);
  } 
  if(sidMin == 0) {
    // pt1 is on minorAxis
    return UT3D_vc_tng_elptMin (vco, pt1, el1, sidMaj, isol);
  } 

// get c for ellipse from formula b*b * x*x + a*a * y*y = c
// a=length of majorAxis; b=length of minorAxis;
// using point x=a,y=0: c = b*b * a*a
  el2c.a = UT3D_len_vc(&el1->va);
  aa = el2c.a * el2c.a;
  el2c.b = UT3D_len_vc(&el1->vb);
  bb = el2c.b * el2c.b;
  c = aa * bb;
    // printf(" c=%f\n",c);

// get pt.x = offset of point pt1 from ell.center along majorAxis;
  pt.x = UT3D_slen_2ptvc (&el1->pc, pt1, &el1->va);

// get pt.y = offset of point pt1 from ell.center along minorAxis;
  pt.y = UT3D_slen_2ptvc (&el1->pc, pt1, &el1->vb);
    // printf(" pt.x=%f pt.y=%f\n",pt.x,pt.y);

  // test if pt1 is on ellipse
  UT3D_pt_el_ptx (&y, pt.x, el2c.a, el2c.b);
  if(UTP_comp2db (y, fabs(pt.y), UT_TOL_pt)) {
    // pt1 IS on elli
    UT3D_vc_tangel (vco, pt1, el1);
    if(isol > 0) UT3D_vc_invert (vco,vco);
    return 1;
  }

  // test if pt1 is inside ellipse
  if((fabs(pt.x) < el2c.a)&&(fabs(pt.y) < y)) {
    return -1;
  }

// get intersection-point distance (distance from ell.center) on x-axis
// formula: b*b * pt.x *x + a*a * pt.y * = c
// using y=0: x = c / (b*b * pt.x)
// using x=0: y = c / (a*a * pt.y)
  ln21.p1.x = c / (bb * pt.x);
  ln21.p1.y = 0.;
  ln21.p2.x = 0.;
  ln21.p2.y = c / (aa * pt.y);
    // UT3D_stru_dump (Typ_LN2, &ln21, "  ln21:");


// the intersectionLine ln21 is: p1=x,0  p2=0,y
// make 2D-intersection of ellipse with this line
  irc = UT2D_pt_int_elc_ln (&pi1, &pi2, &ln21, &el2c);


// get normalized 3D-vectors vna and vnb
  UT3D_vc_div_d (&vna, &el1->va, el2c.a);
  UT3D_vc_div_d (&vnb, &el1->vb, el2c.b);
    // UT3D_stru_dump (Typ_VC, &vna, "  vna:");
    // UT3D_stru_dump (Typ_VC, &vnb, "  vnb:");

// select solution
  if(sidMaj < 0) UT2D_swap2pt (&pi1, &pi2);
  if(isol > 0) pi1 = pi2;


// transfer pi1 back into 3D-position
  UT3D_vc_multvc (&vhx, &vna, pi1.x);
  UT3D_vc_multvc (&vhy, &vnb, pi1.y);
  UT3D_pt_trapt2vc (&po, &el1->pc, &vhx, &vhy);
    // UT3D_stru_dump (Typ_PT, &po, "  po:");


// return the vector pt1-po
  UT3D_vc_2pt (vco, pt1, &po);

  return 0;

}


//================================================================
  int UT3D_vc_tng_elptMaj (Vector *vco,
                           Point *pt1, CurvElli *el1, int sid, int isol) {
//================================================================
/// UT3D_vc_tng_elptMaj           tangents point-ellipse; point on majorAxis
/// 2013-04-18 Reiter
//
// get tangent to circle with radius = elli.Maj.
// Tria.b(Circ.rad) = elli.a; 
// Tria.c=dist elli.pc - pt1
//               \
//                C = tangetPoint ptt (on Circ);   right angle is in point C
//              / | \
//            /   |   \
//          b    hc     a
//        /       |       \
//      A -------c-------- B = pt1
//           q
//
// pointposition dx,dy; dx=tria.q; dy=tria.hc
//
/// RetCod:
///     1      OK, point is on ellipse
///     0      OK, point is outside ellipse
///    -1      Error: pt1 inside ellipse


// see also UT3D_vc_tangel

  double     ea, eb, dx, dyc, dye, b, c;
  Vector     vna, vnb;    // axes of elli - normalized
  Vector     vhx, vhy;
  Point      po;


  // UT3D_stru_dump(Typ_PT, pt1, "UT3D_vc_tng_elptMaj");


  ea = UT3D_len_vc (&el1->va);
  eb = UT3D_len_vc (&el1->vb);
    // printf(" ea=%f eb=%f\n",ea,eb);


  b = ea;
  c = UT3D_len_2pt (&el1->pc, pt1);

  // test if pt1 is on ellipse
  if(UTP_comp2db(c, ea, UT_TOL_pt)) {
    *vco = UT3D_VECTOR_Y;
    if(sid < 0) UT3D_vc_invert (vco,vco);
    if(isol == 0) UT3D_vc_invert (vco,vco);
    return 1;
  }

  // test if pt1 is inside ellipse
  if(c < ea) return -1;

  UT2D_solvtriri_bc (&dx, &dyc, b, c);
    // printf(" dx=%f dyc=%f\n",dx,dyc);
    // UT3D_vc_pt3db (&vhx, pt1, dx, dyc, 0.);
    // GR_Disp_vc (&vhx, pt1, 8, 1);

  // change point on elli-circ-maj to point on elli
  UT3D_pt_el_ptx (&dye, dx, ea, eb);
    // printf(" dx=%f dye=%f\n",dx,dye);

// select solution
  if(isol == 0) dye *= -1.;

  // get normalized 3D-vectors vna and vnb
  UT3D_vc_div_d (&vna, &el1->va, ea);
  UT3D_vc_div_d (&vnb, &el1->vb, eb);
    // UT3D_stru_dump (Typ_VC, &vna, "  vna:");
    // UT3D_stru_dump (Typ_VC, &vnb, "  vnb:");

// transfer pi1 back into 3D-position
  UT3D_vc_multvc (&vhx, &vna, dx);
  UT3D_vc_multvc (&vhy, &vnb, dye);
  UT3D_pt_trapt2vc (&po, &el1->pc, &vhx, &vhy);
    // UT3D_stru_dump (Typ_PT, &po, "  po:");

// return the vector pt1-po
  UT3D_vc_2pt (vco, pt1, &po);

  return 0;

}


//================================================================
  int UT3D_vc_tng_elptMin (Vector *vco,
                           Point *pt1, CurvElli *el1, int sid, int isol) {
//================================================================
/// UT3D_vc_tng_elpt           tangents point-ellipse; point on minorAxis
/// 2013-04-18 Reiter
// see also UT3D_vc_tangel
//
//     + B=pt1
//     | \
//     |  \
//     |   \
//     |    \a
//     |     \
//     | hc   \
//  c  +-------+C=tangentPoint
//     |      /
//     |     /            hc =xCoord of tangentPoint 
//     |    /             q = yCoord of tangentPoint
//     |   /b             
//    q|  /               c = dist. pt1-ell.pc
//     | /                b = ell.b=radiusCirc
//     |/
//     + A=ell.pc
//
/// RetCod:
///     1      OK, point is on ellipse
///     0      OK, point is outside ellipse
///    -1      Error: pt1 inside ellipse

  double     ea, eb, dxc, dy, dxe, b, c;
  Vector     vna, vnb;    // axes of elli - normalized
  Vector     vhx, vhy;
  Point      po;

  // UT3D_stru_dump(Typ_PT, pt1, "UT3D_vc_tng_elptMin");


  ea = UT3D_len_vc (&el1->va);
  eb = UT3D_len_vc (&el1->vb);
    // printf(" ea=%f eb=%f\n",ea,eb);


  b = eb;
  c = UT3D_len_2pt (&el1->pc, pt1);

  // test if pt1 is on ellipse
  if(UTP_comp2db(c, eb, UT_TOL_pt)) {
    *vco = UT3D_VECTOR_X;
    if(sid < 0) UT3D_vc_invert (vco,vco);
    if(isol == 0) UT3D_vc_invert (vco,vco);
    return 1;
  }

  // test if pt1 is inside ellipse
  if(c < eb) return -1;
    
  UT2D_solvtriri_bc (&dy, &dxc,  b, c);
    // printf(" dxc=%f dy=%f\n",dxc,dy);
    // UT3D_vc_pt3db (&vhx, pt1, dxc, dy, 0.);
    // GR_Disp_vc (&vhx, pt1, 8, 1);

  // change point on elli-circ-min to point on elli
  UT3D_pt_el_pty (&dxe, dy, ea, eb);
    // printf(" dx=%f dye=%f\n",dxe,dy);

// select solution
  if(isol == 0) dxe *= -1.;

  // get normalized 3D-vectors vna and vnb
  UT3D_vc_div_d (&vna, &el1->va, ea);
  UT3D_vc_div_d (&vnb, &el1->vb, eb);
    // UT3D_stru_dump (Typ_VC, &vna, "  vna:");
    // UT3D_stru_dump (Typ_VC, &vnb, "  vnb:");

// transfer pi1 back into 3D-position
  UT3D_vc_multvc (&vhx, &vna, dxe);
  UT3D_vc_multvc (&vhy, &vnb, dy);
  UT3D_pt_trapt2vc (&po, &el1->pc, &vhx, &vhy);
    // UT3D_stru_dump (Typ_PT, &po, "  po:");

// return the vector pt1-po
  UT3D_vc_2pt (vco, pt1, &po);

  return 0;

}


//============================================================================
  int UT3D_pt_el_ptx (double *dye, double dx, double a, double b) {
//============================================================================
// get y-coord of point on elli
// ellipse-formula: y = sqrt((1. - (x*x) / (a*a))) * b;

  // printf("UT3D_pt_el_ptx dx=%f a=%f b=%f\n",dx,a,b);

  *dye = sqrt((1. - (dx*dx) / (a*a))) * b;

    // printf(" dye=%f\n",*dye);

  return 0;

}


//============================================================================
  int UT3D_pt_el_pty (double *dxe, double dy, double a, double b) {
//============================================================================
// get x-coord of point on elli
// ellipse-formula: x = sqrt((1. - (y*y) / (b*b))) * a;

  // printf("UT3D_pt_el_pty dy=%f a=%f b=%f\n",dy,a,b);

  *dxe = sqrt((1. - (dy*dy) / (b*b))) * a;

    // printf(" dxe=%f\n",*dxe);

  return 0;

}


//==================================================================
  int UT2D_pt_tng_ell_vc (Point2 *pto,
                            Vector2 *vct, CurvEll2 *cv1, int isol) {
//==================================================================
/// \code
/// get tangent vector-ellipse.
/// ellipse and vector must be planar to constr.Plane.
///   isol    0=solution-1; 1=solution-2;
/// 2013-04-23 Backmeister, Reiter
///     0      OK
///    -1      Error isol
///    -2      internal error
///    -3      EOM
/// \endcode

// see also UT3D_pt_tng_ell_vc__

  double     aa, bb, k, kk;
  Vector2    v2t, v2ct, el2va;
  Point2     el2pc, p2co, p2o;
  CurvEll2C  elc;
  Point      pt1;
  Vector     vc1;


  // printf("UT2D_pt_tng_ell_vc %d\n",isol);
    // UT3D_stru_dump (Typ_VC, vct, "  vct:");
    // UT3D_stru_dump (Typ_CVELL2, cv1, "  ell:");


// get vector to centerposition
  UT2D_2slen_vc_vc__ (&v2ct.dx, &v2ct.dy, vct, &cv1->va);
    // UT3D_stru_dump (Typ_VC2, &v2ct, "  v2ct:");

// normalize v2t
  UT2D_vc_setLength (&v2ct, &v2ct, 1.);
    // UT3D_stru_dump (Typ_VC2, &v2ct, " norm-v2ct:");

// get ellipse in centerposition
  elc.a = UT2D_len_vc(&cv1->va);
  elc.b = UT2D_len_vc(&cv1->vb);

  aa = elc.a * elc.a;
  bb = elc.b * elc.b;

//----------------------------------------------------------------
// compute tangentPoint

// test vector for (1,0)
  if(UT2D_ckvc_hori(&v2ct) == 0) {
      // printf(" tangent=hori\n");
    p2co.x = 0.;
    if(isol == 0) p2co.y = elc.b;
    else          p2co.y = -elc.b;
    goto L_out;
  }

// test vector for (0,1)
  if(UT2D_ckvc_vert(&v2ct) == 0) {
      // printf(" tangent=vert\n");
    if(isol == 0) p2co.x = elc.a;
    else          p2co.x = -elc.a;
    p2co.y = 0.;
    goto L_out;
  }

// vector v2ct must have format (1,k);
// set v2ct -> format (1,k);
  v2ct.dy /= v2ct.dx;
  v2ct.dx = 1.;
    // UT3D_stru_dump (Typ_VC2, &v2ct, " 1,k-v2ct:");

  k = v2ct.dy;
  kk = k * k;

// 1.solution:
  if(isol == 0) {
    p2co.x = -aa * k / sqrt(aa * kk + bb);
    p2co.y = bb / sqrt(aa * kk + bb);

// 2.solution:
  } else {
    p2co.x = aa * k / sqrt(aa * kk + bb);
    p2co.y = -bb / sqrt(aa * kk + bb);
  }


//----------------------------------------------------------------
  L_out:
// transform point p2co back from centerposition to constructionPlane
    UT3D_stru_dump (Typ_PT2, &p2co, "  p2co:");
// p2o.x along va; p2o.y normal to va
  UT2D_pt_traptvc2len (pto, &cv1->pc, &cv1->va, p2co.x, p2co.y);
    // GR_Disp_pt2 (pto, SYM_STAR_S, 0);
    // UT3D_stru_dump (Typ_PT, pto, "  pto:");

  return 0;

}


//============================================================================
  int UT2D_pt_el_ptx (double *dye, double dx, double a, double b) {
//============================================================================
// get y-coord of point on elli
// ellipse-formula: y = sqrt((1. - (x*x) / (a*a))) * b;

  // printf("UT2D_pt_el_ptx dx=%f a=%f b=%f\n",dx,a,b);

  *dye = sqrt((1. - (dx*dx) / (a*a))) * b;

    // printf(" dye=%f\n",*dye);

  return 0;

}


//============================================================================
  int UT2D_pt_el_pty (double *dxe, double dy, double a, double b) {
//============================================================================
// get x-coord of point on elli
// ellipse-formula: x = sqrt((1. - (y*y) / (b*b))) * a;

  // printf("UT2D_pt_el_pty dy=%f a=%f b=%f\n",dy,a,b);

  *dxe = sqrt((1. - (dy*dy) / (b*b))) * a;

    // printf(" dxe=%f\n",*dxe);

  return 0;

}


//===========================================================================
  int UT2D_pt_tng_ell_ptMaj (Point2 *ptg,
                             Point2 *pt1, CurvEll2 *el1, int sid, int isol) {
//===========================================================================
/// UT3D_vc_tng_elptMaj           tangents point-ellipse; point on majorAxis
/// 2013-04-18 Reiter
/// isol          0|1
//
// get tangent to circle with radius = elli.Maj.
// Tria.b(Circ.rad) = elli.a; 
// Tria.c=dist elli.pc - pt1
//               \
//                C = tangetPoint ptt (on Circ);   right angle is in point C
//              / | \
//            /   |   \
//          b    hc     a
//        /       |       \
//      A -------c-------- B = pt1
//           q
//
// pointposition dx,dy; dx=tria.q; dy=tria.hc
//
/// RetCod:
///     1      OK, point is on ellipse
///     0      OK, point is outside ellipse
///    -1      Error: pt1 inside ellipse

// see also UT3D_vc_tng_elptMaj UT3D_vc_tangel

  double     ea, eb, dx, dyc, dye, c;
  Vector2    vcp;

  // printf("UT2D_pt_tng_ell_ptMaj %d %d\n",sid,isol);
  // UT3D_stru_dump(Typ_PT, pt1, "  pt1:");
  // UT3D_stru_dump(Typ_CVELL2, el1, "  el1:");


  ea = UT2D_len_vc (&el1->va);
  eb = UT2D_len_vc (&el1->vb);
    // printf(" ea=%f eb=%f\n",ea,eb);

  c = UT2D_len_2pt (&el1->pc, pt1);
    // printf(" c=%f\n",c);


  // test if pt1 is on ellipse
  if(UTP_comp2db(c, ea, UT_TOL_cv)) {
    // *vco = UT2D_VECTOR_Y;
    c = ea / eb;
    if(isol) c *= -1.;
    UT2D_vc_multvc (&vcp, &el1->vb, c);
    UT2D_pt_traptvc (ptg, pt1, &vcp);
    return 1;
  }

  // test if pt1 is inside ellipse
  if(c < ea) return -1;

  // get offsets of tangent-point
  UT2D_solvtriri_bc (&dx, &dyc, ea, c);
    // printf(" dx=%f dyc=%f\n",dx,dyc);

  // change point on elli-circ-maj to point on elli
  UT2D_pt_el_ptx (&dye, dx, ea, eb);
    // printf(" dx=%f dye=%f\n",dx,dye);

// select solution
  dye *= -1.;
  if(isol) dye *= -1.;

  // get point on elli
  UT2D_pt_tra2pt2len (ptg, &el1->pc, pt1, dx, dye);

  return 0;

}


//================================================================
  int UT2D_pt_tng_ell_ptMin (Point2 *ptg,
                             Point2 *pt1, CurvEll2 *el1, int sid, int isol) {
//================================================================
/// UT3D_vc_tng_elpt           tangents point-ellipse; point on minorAxis
/// 2013-04-18 Reiter
///   isol      0|1
// see also UT3D_vc_tangel
//
//     + B=pt1
//     | \
//     |  \
//     |   \
//     |    \a
//     |     \
//     | hc   \
//  c  +-------+C=tangentPoint
//     |      /
//     |     /            hc =xCoord of tangentPoint 
//     |    /             q = yCoord of tangentPoint
//     |   /b             
// dy=q|  /               c = dist. pt1-ell.pc
//     | /                b = ell.b=radiusCirc
//     |/
//     + A=ell.pc
//
/// RetCod:
///     1      OK, point is on ellipse
///     0      OK, point is outside ellipse
///    -1      Error: pt1 inside ellipse

// see also UT3D_vc_tng_elptMin


  double     ea, eb, dxc, dy, dxe, c;
  Vector2    vcp;
  // Vector     vna, vnb;    // axes of elli - normalized
  // Vector     vhx, vhy;
  // Point      po;


  // printf("UT3D_vc_tng_elptMin %d %d\n",sid,isol);
  // UT3D_stru_dump(Typ_PT, pt1, "  pt1:");
  // UT3D_stru_dump(Typ_CVELL2, el1, "  el1:");


  ea = UT2D_len_vc (&el1->va);
  eb = UT2D_len_vc (&el1->vb);
    // printf(" ea=%f eb=%f\n",ea,eb);

  c = UT2D_len_2pt (&el1->pc, pt1);
    // printf(" c=%f\n",c);

  // test if pt1 is on ellipse
  if(UTP_comp2db(c, eb, UT_TOL_cv)) {
    // *vco = UT2D_VECTOR_Y;
    c = 1.;
    if(isol) c = -1.;
    UT2D_vc_multvc (&vcp, &el1->va, c);
    UT2D_pt_traptvc (ptg, pt1, &vcp);
    return 1;
  }

  // test if pt1 is inside ellipse
  if(c < eb) return -1;

  // get offsets of tangent-point
  UT2D_solvtriri_bc (&dy, &dxc, eb, c);
    // printf(" dxc=%f dy=%f\n",dxc,dy);

  // change point on elli-circ-min to point on elli
  UT2D_pt_el_pty (&dxe, dy, ea, eb);
    // printf(" dx=%f dye=%f\n",dxe,dy);

// select solution
  dxe *= -1.;
  if(isol) dxe *= -1.;
  // if(sid < 0) dxe *= -1.;

  // get point on elli
  UT2D_pt_tra2pt2len (ptg, &el1->pc, pt1, dy, dxe);

  return 0;

}


//============================================================================
  int UT2D_pt_tng_ell_pt (Point2 *ptg, CurvEll2 *el1, Point2 *pt1, int isol) {
//============================================================================
/// get tangent point-ellipse.
///   isol    0=solution-1; 1=solution-2; 
/// 2013-04-20 Reiter
///     1      OK, point is on ellipse
///     0      OK, point is not on ellipse
///    -2      internal error
///    -3      EOM

// see also UT3D_vc_tng_elpt__

  int        irc, sidMaj, sidMin;
  double     a, aa, b, bb, c, y;
  Point      po;
  Point2     pt, pi1, pi2;
  Line2      ln21;
  Vector2    vna, vnb;    // axes of elli - normalized
  Vector2    vcp, vhx, vhy;
  CurvEll2C  el2c;        // ellipse in centerPosition


  // printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX \n");
  // printf("UT2D_pt_tng_ell_pt isol=%d\n",isol);
  // UT3D_stru_dump (Typ_PT, pt1, "  pt:");
  // UT3D_stru_dump (Typ_CVELL2, el1, "  ell:");


  // vector vcp = elCen - pt
  UT2D_vc_2pt (&vcp, &el1->pc, pt1);

// get sidMaj = above or below plane tru pln.vx=elli.va; pln.vz=elli.vb
// 0=normal; 1=quad. 1,2; -1=quad. 3,4
  sidMaj = UT2D_sid_2vc (&el1->va, &vcp, UT_TOL_cv);

// get sidMin = above or below plane tru pln.vx=elli.vb; pln.vz=elli.va
// 0=normal; 1=quad. 1,4; -1=quad. 2,3
  sidMin = UT2D_sid_2vc (&el1->vb, &vcp, UT_TOL_cv);
    // printf(" sidMaj=%d sidMin=%d\n",sidMaj,sidMin);

  if(sidMaj == 0) {
    // pt1 is on majorAxis
    return UT2D_pt_tng_ell_ptMaj (ptg, pt1, el1, sidMin, isol);
  }
  if(sidMin == 0) {
    // pt1 is on minorAxis
    return UT2D_pt_tng_ell_ptMin (ptg, pt1, el1, sidMaj, isol);
  }


// get c for ellipse from formula b*b * x*x + a*a * y*y = c
// a=length of majorAxis; b=length of minorAxis;
// using point x=a,y=0: c = b*b * a*a
  el2c.a = UT2D_len_vc (&el1->va);
  aa = el2c.a * el2c.a;
  el2c.b = UT2D_len_vc (&el1->vb);
  bb = el2c.b * el2c.b;
  c = aa * bb;
    // printf(" c=%f\n",c);

// get pt.x = offset of point pt1 from ell.center along majorAxis;
  pt.x = UT2D_slen_pt_pt_vc__ (pt1, &el1->pc, &el1->va);

// get pt.y = offset of point pt1 from ell.center along minorAxis;
  pt.y = UT2D_slen_pt_pt_vc__ (pt1, &el1->pc, &el1->vb);
    // printf(" pt.x=%f pt.y=%f\n",pt.x,pt.y);

  // test if pt1 is on ellipse
  if((fabs(pt.x) <= el2c.a)&&(fabs(pt.y) <= el2c.b)) {
    // get pt.y on ell from pt.x
    UT2D_pt_el_ptx (&y, pt.x, el2c.a, el2c.b);
    if(UTP_comp2db (y, fabs(pt.y), UT_TOL_pt)) {
        // printf(" ON ELLIPSE %lf %lf\n",pt.x,pt.y);
      // inputputpoint pt1 is ON ellipse. Get tangent-vector, length vb
      UT2D_vc_tang_el2 (&vcp, pt1, el1);
      // make lentgh = va
      y = el2c.a;
      if(isol) y *= -1.;
      UT2D_vc_multvc (&vcp, &vcp, y);
      // ptg = pt1 + vcp
      UT2D_pt_traptvc (ptg, pt1, &vcp);
      return 1;
    }

  // // test if pt1 is inside ellipse
  // if((fabs(pt.x) < el2c.a)&&(fabs(pt.y) < y)) {
    // return -1;
  // }
  }

// get intersection-point distance (distance from ell.center) on x-axis
// formula: b*b * pt.x *x + a*a * pt.y * = c
// using y=0: x = c / (b*b * pt.x)
// using x=0: y = c / (a*a * pt.y)
  ln21.p1.x = c / (bb * pt.x);
  ln21.p1.y = 0.;
  ln21.p2.x = 0.;
  ln21.p2.y = c / (aa * pt.y);
    // UT3D_stru_dump (Typ_LN2, &ln21, "  ln21:");
    // GR_Disp_ln2 (&ln21.p1, &ln21.p2, 9);


// the intersectionLine ln21 is: p1=x,0  p2=0,y
// make 2D-intersection of ellipse with this line
  irc = UT2D_pt_int_elc_ln (&pi1, &pi2, &ln21, &el2c);
    // UT3D_stru_dump (Typ_PT2, &pi1, "  pi1:");
    // UT3D_stru_dump (Typ_PT2, &pi2, "  pi2:");

// select solution
  if(sidMaj < 0) UT2D_swap2pt (&pi1, &pi2);
  if(isol) pi1 = pi2;

  // get point on elli
  UT2D_pt_traptvc2len (ptg, &el1->pc, &el1->va, pi1.x, pi1.y);

  return 0;

}


//================================================================
  int UT2D_angr_elc_pt (double *aa, Point2 *p1, CurvEll2C *elc) {
//================================================================
/// \code
/// UT2D_angr_elc_pt    get angle for point on 2D-elli in centerPos.
/// returns the real-angle between the major-axis and p1
/// \endcode

  // Point2   pt21;


  // printf("UT2D_angr_elc_pt %lf,%lf\n",p1->x,p1->y);

  *aa = UT2D_angr_vc ((Vector2*)p1);

/*
  // get point on innerCirc from point on elli
  pt21.y = p1->y;
  UT2D_ptx_ci_pty (&pt21.x, &pt21.y, &elc->b);
  if(p1->x < 0) pt21.x *= -1.;
    printf(" pt21=%lf %lf\n",pt21.x,pt21.y);

  // get angle from point on innerCirc
  *aa = UT2D_angr_vc ((Vector2*)&pt21);
*/

    // printf(" aa-r=%lf aa-deg=%lf\n",*aa,UT_DEGREES(*aa));

  return 0;

}


//=================================================================
  int UT3D_2angr_el (double *angs, double *ango, CurvEll2C *el2c) {
//=================================================================
/// \code
/// UT3D_2angr_el              get starting- and opening angle of ellipse
/// angs    real-angle between major-axis and startpoint
/// ango    real-opening-angle between startpoint and endpoint 
///           CCW ?
/// \endcode
  
  double     ao, as, ae;


  // get as from p1
  UT2D_angr_elc_pt (&as, &el2c->p1, el2c);
    // printf(" as-1=%lf\n",as);


  // make ae following as corresponding to sense
  if(UT2D_comp2pt(&el2c->p1, &el2c->p2, UT_TOL_pt)) {
    if(el2c->sr > 0) {
      ae = as + RAD_360;
    } else {
      ae = as - RAD_360;
    }
    
  } else {
    // get ae from p2
    UT2D_angr_elc_pt (&ae, &el2c->p2, el2c);
      // printf(" as=%lf ae=%lf dir=%d\n",as,ae,el2c->sr);
    
    UT2D_2angr_set (&as, &ae, el2c->sr);
  }

  *angs = as;
  *ango = ae - as;

    // printf("ex UT3D_2angr_el as=%lf ao=%lf ae=%lf\n",*angs,*ango,ae);

  return 0;
}


//================================================================
  int UT2D_elc_el3 (CurvEll2C *el2c,  CurvElli *el3) {
//================================================================
// UT2D_elc_el3              make 2D-ell in centerPos from 3D-ell

  Vector      vc1, vcx, vcy;


  // printf("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE \n");
  // UT3D_stru_dump (Typ_CVELL, el3, "UT2D_elc_el3");
  // GR_Disp_vc (&el3->va, &el3->pc, 5, 1);
  // GR_Disp_vc (&el3->vb, &el3->pc, 5, 1);


  if(el3->dir > 0) el2c->sr = 1;
  else             el2c->sr = -1;

  el2c->a = UT3D_len_vc (&el3->va);
  el2c->b = UT3D_len_vc (&el3->vb);
    // printf(" a=%lf b=%lf\n",el2c->a,el2c->b);



  // TEST ONLY:
    // get parameters of el3->p1 on el3->va and el3->vb
    // el2c->p1.x = UT3D_slen_2ptvc (&el3->pc, &el3->p1, &el3->va);
    // el2c->p1.y = UT3D_slen_2ptvc (&el3->pc, &el3->p1, &el3->vb);
    // el2c->p2.x = UT3D_slen_2ptvc (&el3->pc, &el3->p2, &el3->va);
    // el2c->p2.y = UT3D_slen_2ptvc (&el3->pc, &el3->p2, &el3->vb);
      // printf(" p1 = %lf %lf\n",el2c->p1.x,el2c->p1.y);
      // printf(" p2 = %lf %lf\n",el2c->p2.x,el2c->p2.y);
  // TEST ONLY:


  UT3D_vc_div_d (&vcx, &el3->va, el2c->a);
  UT3D_vc_div_d (&vcy, &el3->vb, el2c->b);

  UT3D_vc_2pt (&vc1, &el3->pc, &el3->p1);
    // UT3D_stru_dump (Typ_VC, &vc1, " vc pc-p1:");
  UT3D_2par_vc_vcx_vcy (&el2c->p1.x, &el2c->p1.y, &vc1, &vcx, &vcy);
    // GR_Disp_vc (&vc1, &el3->pc, 7, 1);

  UT3D_vc_2pt (&vc1, &el3->pc, &el3->p2);
  UT3D_2par_vc_vcx_vcy (&el2c->p2.x, &el2c->p2.y, &vc1, &vcx, &vcy);

    // UT3D_stru_dump (Typ_CVELL2C, el2c, "ex UT2D_elc_el3:");

  return 0;

}


//================================================================
  int UT2D_pt_ptel3 (Point2 *pt2, Point *pt3, CurvElli *el3) {
//================================================================
/// UT2D_pt_ptel3     get pt on 2D-elli in centerPos from pt on 3D-elli


  pt2->x = UT3D_slen_2ptvc (&el3->pc, pt3, &el3->va);
  pt2->y = UT3D_slen_2ptvc (&el3->pc, pt3, &el3->vb);

  return 0;

}


//================================================================
  int UT3D_par1_el_pt (double *du, CurvElli *el1, Point *pt1) {
//================================================================
// UT3D_par1_el_pt        get parameter (0-1) for point on ellipse

  int        sp1;
  double     db, aa, as, ae, ao;
  Point2     pt21;
  Vector2    vc1;
  CurvEll2C  el2c;


  // printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX \n");
  // UT3D_stru_dump (Typ_CVELL, el1, "UT3D_par1_el_pt el1:");
  // UT3D_stru_dump (Typ_PT, pt1, "  pt1:");


  // make el2c from ell
  UT2D_elc_el3 (&el2c, el1);

  // get startAngle & opening-angle of elli
  UT3D_2angr_el (&as, &ao, &el2c);
    // printf(" as=%lf ao=%lf\n",as,ao);


/*
  // get sp1 = side of p1; 
  sp1 = UT3D_sid_ptptvc (pt1, &el1->pc, &el1->va);

  // get y 
  vc1.dy = UT3D_slen_2ptvc (&el1->pc, pt1, &el1->vb);

  // get dx on innerCirc of dy of p2
  UT2D_ptx_ci_pty (&vc1.dx, &vc1.dy, &el2c.b);
  if(sp1 < 0) vc1.dx *= -1.;
    printf(" vc1 = %lf %lf\n",vc1.dx,vc1.dy);
  aa = UT2D_angr_vc ((Vector*)&vc1);
*/

  // get pt on 2D-elli in centerPos from pt on 3D-elli
  UT2D_pt_ptel3 (&pt21, pt1, el1);
    // UT3D_stru_dump (Typ_PT2, &pt21, "  pt21:");

  // get angr on innerCirc
  aa = UT2D_angr_vc ((Vector2*)&pt21);

  // aa should be between as,ae. Do not modify as.
  aa = UT2D_angr_set_2angr (as, aa, el2c.sr);

  // change angr da into par1 du (du = ao / piTwo)
  ae = as + ao;
    // printf(" aa=%lf as=%lf ao=%lf ae=%lf\n",aa,as,ao,ae);

  UTP_param_p0p1px (du, as, ae, aa);

    // printf("ex UT3D_par1_el_pt du=%lf ao=%lf\n",*du,ao);

  return 0;

}


//====================== EOF ===========================
