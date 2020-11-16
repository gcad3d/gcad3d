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
- Clothoid cannot be trimmed yet !
- struct CurvClot does not have limit-parameters (v0,v1) !
  change 'clot' in functionnames into 'clt'

-----------------------------------------------------
Modifications:
2007-09-26 Neu: UCV_CltCrvCurvWind, UCV_CurvCirCltCrv.
2007-09-20 Neu: UCV_TrfMatCltCrv.
2007-09-19 Neu: UCV_EvPtTgCltCrv.
2007-09-18 Neu: UCV_Ev2DTgCltCrv.
2007-09-17 Neu: UCV_Ev2DNxtPtCltCrv, UCV_PlgCltCrv.
2007-04-30 ucv_OffSurCrv statt UT3D_cv_offsurf. 

-----------------------------------------------------
*/
/*!
\file  ../ut/cvClot.c
\brief clothoid curve functions
\code
=====================================================
List_functions_start:

UT3D_npt_clot           Polygon <-- clothoid curve
UT3D_pt_intclotptvc     intersect line(pt+vc) with Clothoid
UT3D_pt_prjclotpt       project point onto Clothoid
UT3D_ptvc_evparclot     get point&tangent on Clothoid from parameter
UT3D_par_clotpt         get parameter of clot from point
UT3D_park__par1_clot    get curvature from parameter 0-1

UCV_CurvCirCltCrv       Curvature circle <-- clothoid curve & parameter
UCV_EvPtTgCltCrv        Evaluate point / tangent <-- clothoid curve
UCV_Ev2DNxtPtCltCrv     Evaluate 2D-next point <-- clothoid curve
UCV_Ev2DTgCltCrv        Evaluate 2D-tangent <-- clothoid curve
UCV_OffSurCrv           Offset surface curve <-- curve
UCV_TrfMatCltCrv        Transformation matrix <-- clothoid curve
UCV_CltCrvCurvWind      Clothoid curve <-- curvature & wind

// UCV_PlgCltCrv           Polygon <-- clothoid curve

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



  Vector    v1={1,0,0}, v2={0,0,1};
  Point     p1={0,0,0};
  UME_init (&tmpSeg, memspc201, sizeof(memspc201));
  pTab = (void*)memspc55;
  ptNr = sizeof(memspc55) / sizeof(Point);

  clot.stv = v1;
  clot.plv = v2;
  clot.pc  = 10.;  // 2.5 - 10.
  clot.cs  = 0.;
  clot.ce  = 0.1;  // 0.3 - 0.1
  clot.lr  = 0;
  DEB_dump_obj__ (Typ_CVCLOT, &clot, "clot=");
  UT3D_npt_clot (pTab, &ptNr, Typ_CVCLOT, &clot, 0.01);
    GR_tDyn_pcv (pTab, ptNr, 9);



Returncodes:
 0 OK
>0 OK with additional info
<0 Error with additional info

compile with gcc:
cc -c -O0 -g3 -Wall ucv.c

Windows:
cl -c ucv.c

ctags -f ucv.tag ucv.c

*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../ut/ut_geo.h"
#include "../ut/ut_math.h"
#include "../ut/func_types.h"               // UI_Func... SYM_..

// #include "../ut/ut_TX.h"
// #include "../ubs/ubscrv.h"
// #include "../umt/umt.h"
// #include "ucv.h"





 
//========================================================================
  int UT3D_ptvc_evparclot (Point *pto, Vector *vco,
                            int vTyp, CurvClot *cv, double par) {
//========================================================================
// UT3D_ptvc_evparclot     get point&tangent on Clothoid from parameter
// Input:
//   par        parameter, type = vTyp
//   vTyp       0 par = curvature
//              1 par = value from 0-1. See INF_struct_par
// Output:
//   pto    point on Clothoid at parameter = <par>
//   vco    tangent-vector in pto if vco != NULL


  int      irc;
  Mat_4x3  mat;
  Point2   pts;
  Point    pt1;
  double   hv, ts, tc, te, cp, pp;


  // printf("UT3D_ptvc_evparclot vTyp=%d par%f\n",vTyp,par);


  if(vTyp == 0) par = UT3D_park__par1_clot (((CurvClot*)cv)->cs,
                                            ((CurvClot*)cv)->ce, par);



  // get the Transformation matrix > mc, the startpoint > pts and
  // the startParameter > cl
  UCV_TrfMatCltCrv (mat, &pts, &ts, cv);
    // printf(" cv->cs=%f cv->ce=%f\n",cv->cs,cv->ce);
  // if(mode < 2) return 0;

  // compute endParameter te
  hv = cv->pc / SR_PI;         // SQRT(PI)
  te = hv * cv->ce;

  if(vTyp) {
    // change 0-1-parameter par to parameter tc - curvature
    if (par < 0. || par > 1.) goto L_InErr;
    // tc = UTP_vx_vMin_vMax_par1 (ts, te, par);
    tc = UT3D_park__par1_clot (ts, te, par);
  } else tc = par;
    // printf(" ts=%f tc=%f te=%f\n",ts,tc,te);

  // get point on curv
  irc = UCV_EvPtTgCltCrv (&pt1, vco, cv, tc, mat, &pts, ts);

  if(pto) *pto = pt1;

    // GR_tDyn_symB__ (pto, SYM_STAR_S, 1);
    // if(mode == 1) GR_tDyn_vc__ (vco, pto, 9, 0);

  return irc;


L_InErr:
  TX_Error("UT3D_ptvc_evparclot: input error");
  return -1;

}


//================================================================
  int UT3D_pt_intclotptvc (int *nxp, Point *xpTab, double *tTab,
                           Point *lpt, Vector *lvc, CurvClot *cv,
                           double tol) {
//================================================================
/// \code
/// UT3D_pt_intclotptvc     intersect line(pt+vc) with Clothoid
/// gives also parameter of intersectionPoint (from 0-1)
/// see UT3D_pt_intlnbspl UT3D_npt_clot
///
/// Output:
///   tTab   parallel zu xptab die zugehoerigen parameter 
///          none if tTab == NULL
/// \endcode


  int      iTry, yFlip;
  Mat_4x3  mc, imc;
  Point2   ptOld, ptm, pt1, pt2, ptx;
  Point    xpt, xp1;
  Vector   xvc;
  Vector2  vc1, vc2;
  double   da1[3], da2[3], d1, dm, d2, t1, tm, t2, ts, te, tMin, tolq, dMin, dx, dd;


  // printf("UT3D_pt_intclotptvc %d tol=%f\n",*nxp,tol);
  // DEB_dump_obj__ (Typ_PT, lpt, " lpt:");
  // DEB_dump_obj__ (Typ_VC, lvc, " lvc:");
    // GR_tDyn_symB__ (lpt, SYM_STAR_S, 0);


  tolq = tol * tol;
    // printf(" tolq=%f\n",tolq);


  // Start- und Endpunkt holen, testen

  // get the Transformation matrix > mc, the startpoint > pts and
  // the startParameter > cl
  UCV_TrfMatCltCrv (mc, &pt1, &ts, (CurvClot*)cv);
    // DEB_dump_obj__ (Typ_PT2, &pt1, "pt1:");
    // GR_Disp_pt2 (&pt1, SYM_STAR_S, 0);


  // Endparameter from EndCurvatur
  te = ((CurvClot*)cv)->pc * ((CurvClot*)cv)->ce / SR_PI;
    // printf(" ts =%f te=%f\n",ts,te);


  // set yFlip; 0=not, 1=yes
  if ((((CurvClot*)cv)->lr == 0 && ts > te) ||
      (((CurvClot*)cv)->lr == 1 && ts < te))   yFlip = 1;
  else yFlip = 0;
    // printf(" yFlip=%d\n",yFlip);


  // translate point
  UT3D_m3_invm3 (imc, mc);             // get inv. mat
  UT3D_pt_tra_pt_m3 (&xp1, imc, lpt);    // translate point to search
  UT3D_vc_tra_vc_m3 (&xvc, imc, lvc);    // translate vector


  // check left/right bend of clothoid polygon
  if(yFlip) { xp1.y = -xp1.y; xvc.dy = -xvc.dy; }
    // DEB_dump_obj__ (Typ_PT, &xp1, "xp1:");
    // GR_Disp_pt2 (&xp1, SYM_STAR_S, 0);
    // GR_Disp_vc2 (&xvc, &xp1, 9, 0);


  // Endpunkt
  t1 = ts;
  // t2 = te - ts;
  t2 = te;
  UCV_Ev2DNxtPtCltCrv (&pt2, cv, ts, &pt1, t2);
    // DEB_dump_obj__ (Typ_PT2, &pt2, "pt2:");
    // GR_Disp_pt2 (&pt2, SYM_STAR_S, 0);

  dd = 0.5;


  //----------------------------------------------------------------
  iTry = 0;
  L_nxt_try:
  ++iTry;

  // Mittelpunkt holen, testen auf welcher Seite
  // tm = (t2 - t1) / 2 + t1;
  tm =  t1 + ((t2 - t1)  * dd);    // dd = 0.5;
    // printf(" iTry=%d t1=%f tm=%f t2=%f\n",iTry,t1,tm,t2);
  // get ptm using old point pt1 with its parameter t1
  UCV_Ev2DNxtPtCltCrv (&ptm, cv, t1, &pt1, tm);
    // DEB_dump_obj__ (Typ_PT2, &ptm, "ptm:");
    // GR_Disp_pt2 (&ptm, SYM_STAR_S, 1);


  // welcher der 3 Punkte pt1/ptm/pt2 liegt am naechsten lpt/lvc ?
  // UT3D_lenq_PtLn UT3D_nlenq_3pt UT3D_nlenq_2ptvc 
  UT2D_vc_2pt (&vc1, &pt1, &ptm);
  UT2D_2par_int2pt2vc (da1, &pt1, &vc1, &xp1, &xvc);
  UT2D_vc_2pt (&vc2, &ptm, &pt2);
  UT2D_2par_int2pt2vc (da2, &dx, &ptm, &vc2, &xp1, &xvc);
    // printf(" p1=%f p2=%f\n",p1,p2);
  d1 = fabs(da1[0] - 0.5);
  d2 = fabs(da2[0] - 0.5);
    // printf(" d1=%f d2=%f\n",d1,d2);



  if(d1 < d2) {   // next point between p1-pm
    // printf(" p1-pm\n");
    dd = da1[0];
    // compute intersectionpoint
    UT2D_pt_parvc_pt_vc (&ptx, &pt1, da1[0], &vc1);
    if(UT2D_comp2pt(&ptx, &ptm, tol)) goto L_finish;

    t2 = tm;

  } else {        // next point between pm-p2
    // printf(" pm-p2\n");
    dd = da2[0];
    // compute intersectionpoint
    UT2D_pt_parvc_pt_vc (&ptx, &ptm, da2[0], &vc2);
    if(UT2D_comp2pt(&ptx, &ptm, tol)) goto L_finish;

    pt1 = ptm;
    t1 = tm;
  }

  if(iTry < 10) {
    goto L_nxt_try;
  }

  // printf(" ts=%f tMin=%f te=%f\n",ts,tMin,te);
  L_finish:
  // get parameter von 0-1 from tm
  if(tTab) {
    // UTP_par1_vMin_vMax_vx (&tTab[0], ts, te, tm);
    tTab[0] = UTP_par1_vMin_vMax_vx (ts, te, tm);
  }

  UT3D_pt_pt2_0 (&xpt, &ptm);
  if(yFlip) xpt.y = -xpt.y;
  UT3D_pt_tra_pt_m3 (&xpTab[0], mc, &xpt);
    // GR_tDyn_symB__ (&xpTab[0], SYM_STAR_S, 1);
  *nxp = 1;


  // printf("ex UT3D_pt_intclotptvc par=%f\n",tTab[0]);
  // DEB_dump_obj__ (Typ_PT, &xpTab[0], " xp:");


  return 0;

}


//================================================================
  int UT3D_pt_prjclotpt (int *nxp, Point *xpTab, double *tTab,
                         Point *lpt, CurvClot *cv, double tol) {
//================================================================
// UT3D_pt_prjclotpt       project point onto Clothoid
// Input:
//   nxp     site of xpTab,tTab
//   lpt     point to project onto clothoid
//   cv      curve
// Output:
//   nxp     nr of points and parameters in xpTab,tTab
//   xpTab   points on curve
//   tTab    parameters of points on curve
//
// see UT3D_pt_intlnbspl UT3D_npt_clot


  
  int      iTry, yFlip;
  Mat_4x3  mc, imc;
  Point2   pt1, pt2;
  Point    xp1;
  double   d1, d2, t1, t2, tx1, tx2, ts, te, dd;


  // printf("UT3D_pt_prjclotpt %d tol=%f\n",*nxp,tol);
  // DEB_dump_obj__ (Typ_PT, lpt, "lpt:");
  // GR_tDyn_symB__ (lpt, SYM_STAR_S, 0);


  // get the Transformation matrix > mc, the startpoint > pts and
  // the startParameter > cl
  UCV_TrfMatCltCrv (mc, &pt1, &ts, (CurvClot*)cv);
    // DEB_dump_obj__ (Typ_PT2, &pt1, "pt1:");
    // GR_Disp_pt2 (&pt1, SYM_STAR_S, 0);


  // Endparameter from EndCurvatur
  te = ((CurvClot*)cv)->pc * ((CurvClot*)cv)->ce / SR_PI;
    // printf(" ts =%f te=%f\n",ts,te);


  // set yFlip; 0=not, 1=yes
  if ((((CurvClot*)cv)->lr == 0 && ts > te) ||
      (((CurvClot*)cv)->lr == 1 && ts < te))   yFlip = 1;
  else yFlip = 0;
    // printf(" yFlip=%d\n",yFlip);


  // translate point
  UT3D_m3_invm3 (imc, mc);             // get inv. mat
  UT3D_pt_tra_pt_m3 (&xp1, imc, lpt);    // translate point to search


  // check left/right bend of clothoid polygon
  if(yFlip) xp1.y = -xp1.y;
    // DEB_dump_obj__ (Typ_PT, &xp1, "xp1:");
    // GR_Disp_pt2 (&xp1, SYM_STAR_S, 0);


  // Endpunkt
  t1 = ts;
  t2 = te;
  UCV_Ev2DNxtPtCltCrv (&pt2, cv, ts, &pt1, t2);
    // DEB_dump_obj__ (Typ_PT2, &pt2, "pt2:");
    // GR_Disp_pt2 (&pt2, SYM_STAR_S, 0);


  iTry = 0;
  goto L_ck;



  //----------------------------------------------------------------
  L_nxt_try:
  ++iTry;

  // get two new points pt1 and pt2 from parameters d1 and d2;
  tx1 = t1;
  tx2 = t2;
  t1 =  tx1 + ((tx2 - tx1)  * d1);
  t2 =  tx1 + ((tx2 - tx1)  * d2);
    // printf(" iTry=%d t1=%f t2=%f d1=%f d2=%f\n",iTry,t1,t2,d1,d2);
  UCV_Ev2DNxtPtCltCrv (&pt1, cv, tx2, &pt2, t1);
  UCV_Ev2DNxtPtCltCrv (&pt2, cv, t1, &pt1, t2);
    // DEB_dump_obj__ (Typ_PT2, &pt1, "  pt1:");
    // DEB_dump_obj__ (Typ_PT2, &pt2, "  pt2:");
    // GR_Disp_pt2 (&pt1, SYM_STAR_S, 1);
    // GR_Disp_pt2 (&pt2, SYM_STAR_S, 1);

  L_ck:
  UT2D_par_3pt (&dd, &pt1, &pt2, &xp1);
    // printf(" dd=%f\n",dd);

  d1 = dd - 0.02;   // 0.2:14 trys; 0.1:8 trys; 0.05: 6 trys 0.01: 4 trys
  d2 = dd + 0.02;

  if(UT2D_comp2pt(&pt1, &pt2, tol)) {
    goto L_found;
  }

  // pmOld = ptm;

  if(iTry < 15) {
    // if(dMin > tolq)
    goto L_nxt_try;
  }


  // change tMin into parameter 0-1
  // printf(" ts=%f tMin=%f te=%f\n",ts,tMin,te);
  L_found:
  // UTP_par1_vMin_vMax_vx (&tx1, ts, te, t1);
  tx1 = UTP_par1_vMin_vMax_vx (ts, te, t1);


  L_finish:
  tTab[0] = tx1;
  if(yFlip) pt1.y = -pt1.y;
  UT3D_pt_pt2_0 (&xp1, &pt1);
  UT3D_pt_tra_pt_m3 (&xpTab[0], mc, &xp1);
    // GR_tDyn_symB__ (&xpTab[0], SYM_STAR_S, 1);
  *nxp = 1;


    // printf("ex UT3D_pt_prjclotpt par=%f\n",tTab[0]);
    // DEB_dump_obj__ (Typ_PT, &xpTab[0], " xp:");

  return 0;

}


// //====================================================================
//   int UT3D_park__par1_clot (double *park, CurvClot *cv, double par1) {
// //====================================================================
// // UT3D_park__par1_clot    get curvature from parameter 0-1
// // see UTP_vx_vMin_vMax_par1
//   *park = (cv->ce - cv->cs) * par1 + cv->cs;
//   return 0;
// }


//==========================================================================
  int UT3D_par_clotpt (double *par, Point *pti, CurvClot *cv, double tol) {
//==========================================================================
// get parameter of clot from point
// parameter is between 0 and 1.
// tol eg UT_DISP_cv

  int    ptNr;
  Point  ptx;

  // DEB_dump_obj__ (Typ_PT, pti, "UT3D_par_clotpt: ");

  ptNr = 1;
  UT3D_pt_prjclotpt (&ptNr, &ptx, par, pti, cv, tol);

    // printf("ex UT3D_par_clotpt %f\n",*par);

  return 0;

}


//================================================================
  int UT3D_ptNr_clot (int *pNr, CurvClot *clt, double tol) {
//================================================================
// estimation nr of points of polygon for clotoid

  double   hv, ts, te, tt;
  double   rm, aSeg;


  // DEB_dump_obj__ (Typ_CVCLOT, clt, "UT3D_ptNr_clot");
  // printf(" tol=%lf\n",tol);


  hv = clt->pc / SR_PI;         // SQRT(PI)
  ts = hv * clt->cs; // >=0
  te = hv * clt->ce; // >=0
  tt = fabs(te-ts);
    // printf(" ts=%lf te=%lf tt=%lf\n",ts,te,tt);


  rm = clt->pc;  // 100.; // medium radius
  aSeg = UT2D_angr_ciSec (tol, rm);
  *pNr =  tt / aSeg + 2;
    // printf(" pNr=%d aSeg=%lf\n",*pNr,aSeg);

  return 0;

}

 
//=======================================================================
  int UT3D_npt_clot (Point *pTab, int *ptNr, CurvClot *clt, double tol) {
//=======================================================================
/// \code
/// make polygon from planar curve (clotoid)
/// NOT YET SUPPORTED: parabola, hyperbola, ellipsis)
/// See also pspl_pol_psp bspl_pol_bsp UT3D_cv_rbsp
/// Input:
///   ptNr         max size of pTab
///   pTab[ptNr]   empty
/// Output:
///   ptNr         size of pTab
///   RetCod       0=OK; -1=Error
/// \endcode

// Vergroesserungsfaktor ...
#define  f1 0.965    // 0.92 geht bei 180 Grad nicht mehr !?
                     // 0.955 geht bei tol < 2 nicht mehr !

  int      i1, i2, ii, ptMax, iDir;
  Mat_4x3  mc;
  Point2   pt1, pt2;
  double   t1, t2, t3, td, dc, ts, te, tt, tol1, tol2;


  // printf("UT3D_npt_clot tol=%f\n",tol);

  tol1 = tol / 1.33;
  tol2 = tol * 1.33;
    // printf(" tol1=%f tol2=%f\n",tol1,tol2);
  ptMax = *ptNr;



  // get the Transformation matrix > mc, the startpoint > pts and
  // the startParameter > cl
  UCV_TrfMatCltCrv (mc, &pt1, &ts, clt);
    // printf(" cv.cs=%f cv.ce=%f\n",((CurvClot*)cv)->cs,((CurvClot*)cv)->ce);
    // DEB_dump_obj__ (Typ_M4x3, mc, "mc:");

  // Endparameter from EndCurvatur
  // te = ((CurvClot*)cv)->pc * ((CurvClot*)cv)->ce / SR_PI;
  te = clt->pc * clt->ce / SR_PI;



  // relative Abweichung schaetzen
  dc = (te - ts ) / 3;
    // printf(" ts=%f te=%f dc=%f\n",ts,te,dc);

  if(dc < 0.) iDir = 1;             // verkehrt; Curvatur decreasing
  else        iDir = 0;             // normal; Curvatur increasing

  ii = 0;
  t1 = 1.2;       // der Faktor mit dem die Reihe veraendert wird ..

  L_nxt_par:
    if(ii < 20) {
      ++ii;
      UCV_Ev2DNxtPtCltCrv (&pt2, clt, ts, &pt1, dc);
        // printf(" dc=%f pt2.y=%f\n",dc,pt2.y);
      if(fabs(pt2.y) < tol1) {dc *= t1; goto L_nxt_par;}
      if(fabs(pt2.y) > tol2) {dc /= t1; goto L_nxt_par;}
    } else TX_Print("UT3D_npt_clot I001");
      // printf(" ts=%f te=%f dc=%f iDir=%d\n",ts,te,dc,iDir);

  // den Startpunkt raus
  // pTab[0] = UT2D_pt_pt3 (&pt1);
  pTab[0] = UT3D_pt_pt2 (&pt1);


  tt = te-ts;   // total

  if(fabs(dc) > fabs(tt)) {   // nur eine Linie von Start- zum Endpunkt
    i2 = 0;
    t2 = ts;
    goto L_finish;
  }


  // so aufteilen, dass kein Rest bleibt.
  // ii = IABS((te-ts) / dc);
  t1 = f1;
  UTM_geomSeq_solv (0, &dc, &tt, &t1, &ii);  // get ii from dc, tt, t1;
  UTM_geomSeq_solv (1, &dc, &tt, &t1, &ii);  // get dc from tt, t1, ii
    // printf(" ii=%d dc=%f\n",ii,dc);


  i2 = 0;
  td = dc;
  t2 = ts;
  t3 = te - dc/2.;  // nur fuer save check
    // printf(" t2=%f td=%f t3=%f\n",t2,td,t3);


  L_nxt_pt:
    i1 = i2;
    ++i2;
    if(i2 >= ptMax) {TX_Error("UT3D_npt_clot E001"); return -1;}
    t1 = t2;
    t2 = t1 + td;
    if(iDir) { if(t2 < t3) goto L_finish;}
    else     { if(t2 > t3) goto L_finish;}
      // printf(" t2[%d]=%f\n",i1,t2);
    // compute next point
    UCV_Ev2DNxtPtCltCrv ((void*)&pTab[i2], clt, t1, (void*)&pTab[i1], t2);
      // GR_Disp_pt2 (&pTab[i2], SYM_STAR_S, 0);
    td *= f1;
    goto L_nxt_pt;



  // den letzten Punkt raus
  L_finish:
  UCV_Ev2DNxtPtCltCrv ((void*)&pTab[i2], clt, t1, (void*)&pTab[i1], te);
      // GR_Disp_pt2 (&pTab[i2], SYM_STAR_S, 0);
  ++i2;


  // check left/right bend of clothoid polygon
  if ((clt->lr == 0 && ts > te) ||
      (clt->lr == 1 && ts < te)) {
    for (i1=0; i1<i2; ++i1) pTab[i1].y = -pTab[i1].y;
      // printf(" change side\n");
  }


  // Punkte 2D -> 3D transformieren ..
  for(i1=0; i1<i2; ++i1) {
    pTab[i1].z = 0.;
    UT3D_pt_tra_pt_m3 (&pTab[i1], mc, &pTab[i1]);
      // GR_Disp_pt2 (&pTab[i1], SYM_STAR_S, 0);
  }
    // GR_tDyn_pcv (pTab, i2, 9);


  *ptNr = i2;


  return 0;

}


/*=======================================================================*/
  int UCV_CurvCirCltCrv (Circ *cc, CurvClot *cl, double c) {
/*=====================
UCV_CurvCirCltCrv    Curvature circle <-- clothoid curve & parameter

UCV_CurvCirCltCrv     Author: Thomas Backmeister       26.9.2007

Compute curvature circle of a clothoid curve at given positiv
curvature parameter.

IN:
  CurvClot *cl     ... clothoid curve
  double c         ... curvature parameter
                       (within [cl->cs,cl->ce] or [cl->ce,cl->cs])
OUT:
  Circ *cc         ... curvature circle at c
Returncodes:
  1 = circle with infinite radius
  0 = OK
 -1 = input error 
*/

  int rc;	  
  double ts, pp, rad;  
  Point pt, pm;	  
  Point2 pts;
  Vector tg, vc;
  Mat_4x3 mc;

  //printf("UCV_CurvCirCltCrv: Curvature circle <-- clot. curve & param.\n");

  // check parameter
  if (cl->cs < cl->ce) {
    if (c < cl->cs || c > cl->ce) goto L_InErr;
  }    
  else {
    if (c < cl->ce || c > cl->cs) goto L_InErr;
  }
  if (c == 0.0) return 1;
  
  // transformation matrix
  UCV_TrfMatCltCrv (mc, &pts, &ts, cl);


  // change curvature tp into parameter pp;
  pp = c * cl->pc / SR_PI;

  // evaluate curvepoint & tangent at c
  rc = UCV_EvPtTgCltCrv (&pt, &tg, cl, pp, mc, &pts, ts);
  if (rc < 0) return -1;

  // normalvector of tangentvector of length rad
  UT3D_vc_crossprod2vc (&vc, &(cl->plv), &tg);
  rad = 1.0 / c;
  UT3D_vc_setLength (&vc, &vc, rad);
  if (cl->lr != 0) {
    vc.dx = -vc.dx;
    vc.dy = -vc.dy;
    vc.dz = -vc.dz;
  }

  // define curvature circle
  UT3D_pt_traptvclen (&pm, &pt, &vc, rad);
  UT3D_ci_ptvcr (cc, &pm, &(cl->plv), rad);

  return 0;

L_InErr:
  TX_Error("UCV_CurvCirCltCrv: input error");
  return -1;
}



//================================================================
  int UCV_CltCrvCurvWind (CurvClot *cl, double cs, double ce, double wd,
		          Point *stp, Vector *stv, Vector *plv) {
//================================================================
// UCV_CltCrvCurvWind    Clothoid curve <-- curvature & wind

// UCV_CltCrvCurvWind     Author: Thomas Backmeister       26.9.2007

// Define a clothoid curve by curvature at start- and endpoint and the
// amount of winding of its tangentvector in degrees.

// IN:
//   double cs        ... curvature at startpoint (>=0)      ( 1 / radius)
//   double ce        ... curvature at endpoint (>=0, !=cs)  ( 1 / radius)
//   double wd        ... difference angle between tangent vector
//                        at start- and endpoint in rad;
// 		                   positiv: curve is CCW; negativ: CW.
//   Point *stp       ... startpoint of curve
//   Vector *stv      ... startvector of curve
//   Vector *plv      ... normalvector of plane of curve (not parallel stv)  
// OUT:
//   CurvClot *cl     ... clothoid curve
// Returncode:
//   0 = OK



  Plane pln;
  double hv;  

  //printf("UCV_CltCrvCurvWind: Clothoid curve <-- curvature & wind\n");

  // hv = RAD_180 / 90.0; cl->pc = sqrt(fabs(wd * hv / (ce * ce - cs * cs)));
  cl->pc = sqrt(fabs(wd * 2. / (ce * ce - cs * cs)));
  cl->cs = cs;
  cl->ce = ce;
  if (wd > 0) cl->lr = 0;
  else cl->lr = 1;

  // startpoint & planevector
  cl->stp = *stp;
  cl->plv = *plv;

  // startvector = projection of stv onto plane of curve
  UT3D_pl_ptvc (&pln, &(cl->stp), &(cl->plv));
  UT3D_vc_projvcpl (&(cl->stv), &pln, stv);

/*
  if (cl->lr == 0) printf("\nLeft CLOTHOID CURVE:\n");
  else printf("\nRight CLOTHOID CURVE:\n");
  printf("Curveparameter:    %f\n", cl->pc);
  printf("Startcurvature:    %f\n", cl->cs);
  printf("Endcurvature:      %f\n", cl->ce);
  printf("Tangent-Rotation:  %f degrees\n\n", wd);
*/

  return 0;
}



/*=======================================================================*/
  int UCV_TrfMatCltCrv (Mat_4x3 mc, Point2 *pts, double *ts, CurvClot *cl) {
/*====================
UCV_TrfMatCltCrv    Transformation matrix <-- clothoid curve

UCV_TrfMatCltCrv     Author: Thomas Backmeister       20.9.2007

Compute the transformation matrix of a clothoid curve.
The transformation matrix moves the clothoid curve to the destination
defined in the clothoid curve structure.

IN:
  CurvClot *cl     ... clothoid curve
OUT:
  Mat_4x3 mc       ... transformation matrix
  Point2 *pts      ... startpoint of cl in quarterplane x>=0,y>=0,z=0
  double *ts       ... start parameter of cl (>=0)
Returncode:
  0 = OK
*/

  double hv, te;
  Point2 pt2;
  Point qts, rt;
  Vector2 tgs;
  Vector ths, vz;
  Mat_4x3 ma, ima, mb;

  //printf("UCV_TrfMatCltCrv: Transformation matrix <-- clothoid curve\n");

  // start- and endparameter
  hv = cl->pc / SR_PI;         // SQRT(PI)
  *ts = hv * cl->cs; // >=0
  te = hv * cl->ce; // >=0

  // startpoint
  *pts = UT2D_PT_NUL;
  if (*ts > 0) UCV_Ev2DNxtPtCltCrv (pts, cl, 0.0, pts, *ts);
  pt2 = *pts;

  // starttangent vector
  UCV_Ev2DTgCltCrv (&tgs, cl, *ts);

  // check left/right bend of clothoid curve
  if ((cl->lr == 0 && *ts > te) || (cl->lr == 1 && *ts < te)) {
    pt2.y = -pt2.y;
    tgs.dy = -tgs.dy;
  }    

  // transformation matrix from origin=stp, vx=stv, vz=plv
  qts = UT3D_pt_pt2 (&pt2);
  UT3D_vc_vc2 (&ths, &tgs);
  vz = UT3D_VECTOR_Z;
  UT3D_m3_load_povxvz (ma, &qts, &ths, &vz);
    // DEB_dump_obj__ (Typ_M4x3, ma, "ma:");
  UT3D_m3_invm3 (ima, ma);
    // DEB_dump_obj__ (Typ_M4x3, ima, "ima:");
  UT3D_m3_load_povxvz (mb, &(cl->stp), &(cl->stv), &(cl->plv));
    // DEB_dump_obj__ (Typ_M4x3, mb, "mb:");
  UT3D_m3_multm3 (mc, mb, ima);

  // rt = get origin of mat ima
  rt.x = ima[0][3];
  rt.y = ima[1][3];
  rt.z = ima[2][3];
    // DEB_dump_obj__ (Typ_PT, &rt, "rt1:");
  UT3D_pt_tra_pt_m3 (&rt, mb, &rt);
    // DEB_dump_obj__ (Typ_PT, &rt, "rt2:");
  UT3D_m3_load_o (mc, &rt);
    // DEB_dump_obj__ (Typ_M4x3, mc, "mc:");


  return 0;

L_InErr:
  TX_Error("UCV_TrfMatCltCrv: input error");
  return -1;
}


/*=======================================================================*/
  int UCV_EvPtTgCltCrv (Point *pt, Vector *tg, CurvClot *cl, double tc, 
		                    Mat_4x3 mc, Point2 *pts, double ts) {
/*====================
UCV_EvPtTgCltCrv    Evaluate point / tangent <-- clothoid curve

UCV_EvPtTgCltCrv     Author: Thomas Backmeister       19.9.2007

Evaluate point and/or tangent vector of a 3D-clothoid curve at
given curvature parameter.

IN:
  CurvClot *cl     ... clothoid curve
  double tc        ... parameter for point/tangent  (CHANGED !! was curvature)
  Mat_4x3 mc       ... transformation matrix of cl
  Point2 *pts      ... startpoint of cl in quarterplane x>=0,y>=0,z=0
  double ts        ... start parameter of cl (>=0)
OUT:
  Point *pt        ... point at curvature parameter c
  Vector *tg       ... tangent vector at c (length = 1)
Returncodes:
  0 = OK
 -1 = input error
*/

  double hv, te; //tc;
  Point2 qt2;
  Point qt, ptg;
  Vector2 vc2;
  Vector vc;

  //printf("UCV_EvPtTgCltCrv: Evaluate point / tangent <-- clothoid curve\n");

/*
  // check input
  if (cl->cs < cl->ce) {
    if (c < cl->cs || c > cl->ce) goto L_InErr;
  }    
  else {
    if (c < cl->ce || c > cl->cs) goto L_InErr;
  }
*/

  // end- and tc-parameter
  hv = cl->pc / SR_PI;         // SQRT(PI)
  te = hv * cl->ce; // >=0
  // tc = hv * c;      // >=0

  // curvepoint at c in quarterplane x>=0,y>=0,z=0
  UCV_Ev2DNxtPtCltCrv (&qt2, cl, ts, pts, tc);
  qt = UT3D_pt_pt2 (&qt2);

  if (tg) {
    // tangent endpoint at c in quarterplane x>=0,y>=0,z=0
    UCV_Ev2DTgCltCrv (&vc2, cl, tc);
    UT3D_vc_vc2 (&vc, &vc2);
    UT3D_pt_traptvc (&ptg, &qt, &vc);
  }    

  // check left/right bend of curve
  if ((cl->lr == 0 && ts > te) || (cl->lr == 1 && ts < te)) {
    qt.y = -qt.y;
    if (tg) ptg.y = -ptg.y;
  }    

  // transformed curvepoint at c
  UT3D_pt_tra_pt_m3 (pt, mc, &qt);

  if (tg) {
    // transformed tangent vector at c
    UT3D_pt_tra_pt_m3 (&ptg, mc, &ptg);
    UT3D_vc_2pt (tg, pt, &ptg);
    UT3D_vc_normalize (tg, tg);
  }	  

  return 0;

L_InErr:
  TX_Error("UCV_EvPtTgCltCrv: input error");
  return -1;
}


/*=======================================================================*/
  int UCV_Ev2DNxtPtCltCrv (Point2 *pn, CurvClot *cl, double to, Point2 *po,
		           double t) {
/*=======================
UCV_Ev2DNxtPtCltCrv    Evaluate 2D-next point <-- clothoid curve

UCV_Ev2DNxtPtCltCrv     Author: Thomas Backmeister       17.9.2007

Evaluate a 2D-neighbour-point to a 2D-point on a clothoid curve.
The curve is assumed to lie in the quarterplane x>=0,y>=0,z=0.

IN:
  CurvClot *cl     ... clothoid curve
  double to        ... parameter of old point on cl (>=0)
  Point2 *po       ... old point on cl
  double t         ... parameter of neighbour-point of po (>=0, !=to)
OUT:
  Point2 *pn       ... neighbour-point of po on cl
Returncode:
  0 = OK
*/

  double hv, fci, fsi;

  //printf("UCV_Ev2DNxtPtCltCrv: Eval. 2D-next point <-- clothoid curve\n");

  UMT_FresnelIntgls (&fci, &fsi, to, t);
  hv = cl->pc * SR_PI;         // SQRT(PI)
  pn->x = po->x + hv * fci;	  
  pn->y = po->y + hv * fsi;
  return 0;
}


/*=======================================================================*/
  int UCV_Ev2DTgCltCrv (Vector2 *tg, CurvClot *cl, double t) {
/*====================
UCV_Ev2DTgCltCrv    Evaluate 2D-tangent <-- clothoid curve

UCV_Ev2DTgCltCrv     Author: Thomas Backmeister       18.9.2007

Compute 2D-tangent vector of a clothoid curve at parameter value.
The curve is assumed to lie in the quarterplane x>=0,y>=0,z=0.

IN:
  CurvClot *cl     ... clothoid curve
  double t         ... parameter value (>=0)
OUT:
  Vector2 *tg      ... tangent vector at parameter t
Returncode:
  0 = OK
*/

  double hv;	  

  //printf("UCV_Ev2DTgCltCrv: Evaluate 2D-tangent <-- clothoid crv.\n");

  hv = RAD_180 * t * t * 0.5;
  tg->dx = cos(hv);
  tg->dy = sin(hv);
  if (cl->cs > cl->ce) {
    tg->dx = -tg->dx;
    tg->dy = -tg->dy;
  }    
  return 0;
}



//================================================================
  int UCV_PlgCltCrv (int *ptNr, Point **pTab, Memspc *memSeg, CurvClot *cl,
		     Memspc *workSeg) {
//================================================================
// UCV_PlgCltCrv    Polygon <-- clothoid curve

// UCV_PlgCltCrv     Author: Thomas Backmeister       17.9.2007

// Compute polygon from a clothoid curve.

// IN:
//   CurvClot *cl     ... clothoid curve
//   Memspc *workSeg  ... workspace
// OUT:
//   int ptNr         ... number of curve points
//   Point **pTab     ... array of curve points
//   Memspc *memSeg   ... space for pTab (and workspace)
// Returncodes:
//   0 = OK
//  -1 = out of space
//  -2 = out of workspace
//  -3 = input error


// maximal curvelength between two consecutive points
#define MAX_CVLEN2PTS 0.5

  int     rc, mm, i1, j1;	  
  double  ts, te, tm, hw, dt, dt2, t1;
  void    *memStart, *workStart;
  Point2  *p2Tab1, *p2Tab2, pts;
  Mat_4x3 mc;


  //printf("UCV_PlgCltCrv: Polygon <-- clothoid curve\n");

  // transformation matrix, startparameter and
  // startpoint (in quarterplane x>=0,y>=0,z=0)
  UCV_TrfMatCltCrv (mc, &pts, &ts, cl);


  // parameter of endpoint
  te = cl->pc * cl->ce / SR_PI;         // SQRT(PI) // >=0
  if (fabs(te-ts) == 0.0) goto L_InputErr;


  // save start of memories
  memStart = memSeg->next;
  workStart = workSeg->next;


  // space for 2D-startpolygon
  *ptNr = 3;
  p2Tab1 = workSeg->next;
  rc = UME_add (workSeg, sizeof(Point2) * (*ptNr));
  if (rc < 0) goto L_outOfWorkSpace;
  mm = 1;

  // 2D-startpolygon
  p2Tab1[0] = pts;
  tm = (ts + te) * 0.5;
  UCV_Ev2DNxtPtCltCrv (&(p2Tab1[1]), cl, ts, &(p2Tab1[0]), tm);
  UCV_Ev2DNxtPtCltCrv (&(p2Tab1[2]), cl, tm, &(p2Tab1[1]), te);
  hw = cl->pc * SR_PI;         // SQRT(PI)
  if (fabs((te-ts)*0.5*hw) < MAX_CVLEN2PTS) goto L_2D_Done;

  // 2D-polygon of desired accuracy
  for (;;) {
    *ptNr = 2 * (*ptNr) - 1;
    dt = (te - ts) / (double)(*ptNr - 1); // ><0
    dt2 = 2.0 * dt;
    t1 = ts + dt;
    if (mm == 1) {
      p2Tab2 = memSeg->next;
      rc = UME_add (memSeg, sizeof(Point2) * (*ptNr));
      if (rc < 0) goto L_outOfSpace;
      mm = 2;
      j1 = 0;
      for (i1=0; i1<(*ptNr); ++i1) {
        if (i1%2 == 0) {
          p2Tab2[i1] = p2Tab1[j1];
          ++j1;
        }
	else {
          UCV_Ev2DNxtPtCltCrv (&(p2Tab2[i1]), cl, t1-dt, &(p2Tab2[i1-1]), t1);
	  t1 += dt2;
        }		
      }	      
      workSeg->next = workStart;
    }
    else {
      p2Tab1 = workSeg->next;
      rc = UME_add (workSeg, sizeof(Point2) * (*ptNr));
      if (rc < 0) goto L_outOfWorkSpace;
      mm = 1;
      j1 = 0;
      for (i1=0; i1<(*ptNr); ++i1) {
        if (i1%2 == 0) {
          p2Tab1[i1] = p2Tab2[j1];
          ++j1;
        }
	else {
          UCV_Ev2DNxtPtCltCrv (&(p2Tab1[i1]), cl, t1-dt, &(p2Tab1[i1-1]), t1);
	  t1 += dt2;
        }		
      }	      
      memSeg->next = memStart;
    }	    
    if (fabs(dt*hw) < MAX_CVLEN2PTS) break;
  }


  // Done: 2D-polygon of desired accuracy

L_2D_Done:

  if (mm == 2) {
    p2Tab1 = workSeg->next;
    rc = UME_add (workSeg, sizeof(Point2) * (*ptNr));
    if (rc < 0) goto L_outOfWorkSpace;
    mm = 1;
    for (i1=0; i1<(*ptNr); ++i1) p2Tab1[i1] = p2Tab2[i1];
    memSeg->next = memStart;
  }	  

  // 2D -> 3D-polygon
  *pTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(Point) * (*ptNr));
  if (rc < 0) goto L_outOfSpace;
  for (i1=0; i1<(*ptNr); ++i1) (*pTab)[i1] = UT3D_pt_pt2 (&(p2Tab1[i1]));
  workSeg->next = workStart;

  // check left/right bend of clothoid polygon
  if ((cl->lr == 0 && ts > te) || (cl->lr == 1 && ts < te)) {
    for (i1=0; i1<(*ptNr); ++i1) (*pTab)[i1].y = -(*pTab)[i1].y;
  }    

  // transform 3D-polygon
  for (i1=0; i1<(*ptNr); ++i1)
    UT3D_pt_tra_pt_m3 (&((*pTab)[i1]), mc, &((*pTab)[i1]));

  return 0;

L_outOfSpace:
  TX_Error("UCV_PlgCltCrv: out of space");
  return -1;

L_outOfWorkSpace:
  TX_Error("UCV_PlgCltCrv: out of workspace");
  return -2;

L_InputErr:
  TX_Error("UCV_PlgCltCrv: input error");
  return -3;
}


/*=============================== eof ===================================*/
