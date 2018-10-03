// ut_plg.c                   RF                     2003-07-01
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
2003-07-01 Neu.  RF.

-----------------------------------------------------
*/
/*!
\file  ../ut/ut_plg.c
\brief PolygonCurve 
\code
pta = pointTable
plg = struct CurvPoly; pointTable + lengthTable + limitParameters v0,v1.

=====================================================
List_functions_start:

UT3D_ck_plgclo             check if polygon is closed and untrimmed
UT3D_ck_plgparlim          check if knotVal == start | endPt
UT3D_ck_plgpar1lim         check if par == start | endPt
UT3D_ck_pta_maxDev         get max.deviation of a pointTable
UT3D_ck_plg_trim           check if polygon is limited
UT3D_ck_plg_par_trm        test if point is on clipped parts
UT3D_pt_evalplg            Punkt auf PolygonCurve aus Parameterwert
UT3D_pt_projptplg          proj PT -> PolygonCurve
UT3D_pt_plg_lim            get limits (startpoint, endpoint, parameters)
UT3D_pt_intlnplg           intersect line and polygon
UT3D_pt_intciplg           intersect circle and polygon
UT3D_pt_intelplg           intersect ellipse and polygon
UT3D_pt_intbspplg          intersect b-spline curve and polygon
UT3D_pt_intplplg           intersect Plane polygon
UT3D_pt_int2plg            intersect 2 polygons

UT3D_plg_pta               PolygonCurve aus Punktetabelle
UT3D_plg_lvTab             create new lengthTable lvTab
UT3D_plg_projplgpln        Polygon = project Polygon -> Plane
UT3D_plg_parl_pln          Polygon = parallel to polygon in plane, dist

UT3D_parplg_plgpt          get knotvalue from point on polygon
UT3D_par_par1plg           get knotvalue from parameter 0-1
UT3D_par1_plg_pt           get parameter 0-1 for point on polygon
UT3D_par1_parplg           get parameter 0-1 from knotvalue
UT3D_vc_evalplg            Tangente an PolygonCurve aus Parameterwert
UT3D_pta_plg     (ut_npt.c)  Punktetabelle (relimited) aus PolygonCurve
UT3D_pta_ccw_plg           Punktetabelle (relimited) aus PolygonCurve
UT3D_plg_npar1_npar        get knotTable (length) with values from 0-1 for polygon
UT3D_2pt_plg_iseg            get segment from polygon

UPLG_2par_iseg             get parameters of single segment of polygon
UPLG_iseg_par              get segmentNr from parameter
UPLG_pare_unl              get unlimited endParameter of CurvPoly  INLINE

ULN3_segNr_par_prj_pt_nln  get segNr & parameter from prj point onto lnTab
UT3D_2pt_segnln            get segment from lineTable

List_functions_end:
=====================================================
see also UT3D_pt_ck_on_pta UT3D_isConvx_ptTab UT3D_ipt2_npt UT3D_pt_mid_pta
  UT3D_2vc_pta UT3D_vc_perpptai
  UT3D_pta_ck_planar UT3D_rMin_pta UT3D_pta_dbo UT3D_cv_*
  UT3D_npt_ci UT3D_pta_rot__ UT3D_sr_polc
  UT3D_npt_ox__ UT3D_pta_sus
  
\endcode *//*----------------------------------------


*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




#include "../ut/ut_geo.h"
#include "../ut/ut_plg.h"

#include "../ut/func_types.h"                  // SYM_SQUARE ..



//====================================================================
  int UPLG_2par_iseg (double *p1, double *p2, int is, CurvPoly *plg) {
//====================================================================
// UPLG_2par_iseg        get parameters of single segment of polygon
// Input:
//   is    segmentNr to get parameters; 0=first (the segment with v0)
//   plg   polygon-curve
// Output:
//   p1    parameter of startpoint of segment
//   p2    parameter of endpoint of segment
// see UT3D_2pt_plg_iseg CVTRM__plg_iseg

  int   iis, iie, iseg, ipt, itot;


  // UT3D_stru_dump (Typ_CVPOL, plg, " UPLG_2par_iseg %d",is);


  itot = plg->ptNr - 1;
  if(is >= itot) return -1;


  // get startSegNr, endSegNr. 0=first.
  UPLG_iseg_par (&iis, plg->v0, plg);
  UPLG_iseg_par (&iie, plg->v1, plg);
    // printf(" iis=%d iie=%d is=%d\n",iis,iie,is);



  //----------------------------------------------------------------
  if(plg->dir) goto L_rev;

  // fwd
  if(is == 0) {
    *p1 = plg->v0;
  } else {
    ipt = is + iis;
    if(ipt == itot) ipt = 0;
    *p1 = plg->lvTab[ipt];
  }


  iseg = iis + is;
  if(iseg >= itot) iseg -= itot;
  if(iseg == iie) {
    *p2 = plg->v1;
  } else {
    *p2 = plg->lvTab[is + iis + 1];
  }
  goto L_exit;


  //----------------------------------------------------------------
  L_rev:
   printf(" _2par_iseg-rev\n");

  if(is == 0) {
    *p1 = plg->v0;
  } else {
    ipt = iis - is + 1;
    if(ipt <= 0) ipt += itot;
      printf(" p1-ipt=%d\n",ipt);
    *p1 = plg->lvTab[ipt];
  }


  iseg = iis - is;
  if(iseg < 0) iseg += itot;
    printf(" p2-iseg=%d\n",iseg);
  if(iseg == iie) {
    *p2 = plg->v1;
  } else {
    *p2 = plg->lvTab[iseg];
  }


  //----------------------------------------------------------------
  L_exit:

    // printf("ex _2par_iseg p1=%lf p2=%lf\n",*p1,*p2);

  return 0;

}

//================================================================
  int UT3D_pt_plg_lim (Point *p1, Point *p2, double *v1, double *v2,
                       CurvPoly *cv1) { 
//================================================================
/// \code
/// UT3D_pt_plg_lim         get limits (startpoint, endpoint, parameters)
/// 
/// Output: 
///   p1        startpoint (none if p1 == NULL)
///   p2        endpoint (none if p2 == NULL)
///   v1        parameter (0-1) for startpoint (v1=NULL: none)
///   v2        parameter (0-1) for endpoint (v2=NULL: none)
/// 
/// 
/// see also UTO_2pt_lim_ox UTO_2pt_lim_dbo UTO_pt_ox
/// \endcode
    
// TODO: use it in UTO_2pt_limstru
    

    
  printf("UT3D_pt_plg_lim \n");


  if(p1) UT3D_pt_evalplg (p1, cv1, cv1->v0);
  if(p2) UT3D_pt_evalplg (p2, cv1, cv1->v1);
  // if(v1) *v1 = cv1->v0;
  if(v1) *v1 = UT3D_par1_parplg (&cv1->v0, cv1);
  // if(v2) *v2 = cv1->v1;
  if(v2) *v2 = UT3D_par1_parplg (&cv1->v1, cv1);

    if(v1) printf(" v1=%lf\n",*v1);
    if(v2) printf(" v2=%lf\n",*v2);

  return 0;

}


//==========================================================================
  int ULN3_segNr_par_prj_pt_nln (int *is, double *ps, 
                                 Point *pti, int lNr, Line* lTab) {
//==========================================================================
/// ULN3_segNr_par_prj_pt_nln  get segNr & parameter from prj point onto lnTab

  int      i1;
  double   d1, d2, dd;
  Vector   v1, v2;


  printf("ULN3_segNr_par_prj_pt_nln %d\n",lNr);
  UT3D_stru_dump (Typ_PT, pti, " pti");
  for(i1=0;i1<lNr;++i1) UT3D_stru_dump (Typ_LN,&lTab[i1]," ln[%d]",i1);


  dd = UT_VAL_MAX;
  *is = -1;

  // loop tru lines, find parameter .
  for(i1=0; i1 < lNr; ++i1) {

    // get parameter
    UT3D_vc_2pt (&v1, &lTab[i1].p1, pti);
    UT3D_vc_2pt (&v2, &lTab[i1].p1, &lTab[i1].p2);
    UT3D_parvc_2vc (&d1, &v1, &v2);
      printf(" d1 von %d = %f\n",i1,d1);

    // skip line if parameter outside
    if(d1 < 0.) continue;
    if(d1 > 1.) continue;

    // find quadr. distance
    UT3D_nlenq_2vc (&d2, &v1, &v2);
    // skip line if distance not less .. 
    if(d2 >= dd) continue;

    dd = d2;
    *is = i1 + 1;  // nr of selected segment
    *ps = d1;      // selected parameter
  }


  printf("ex _segNr_par_prj_pt_nln %d %f\n",*is,*ps);

  if(*is < 0) return -1;

  return 0;

}


//================================================================
  double UT3D_ck_pta_maxDev (int ptNr, Point *pTab) {
//================================================================
/// \code
/// UT3D_ck_pta_maxDev         get max.deviation of a pointTable
///
///         /p2\          |
///       /      \        |  deviation = dist p2 to p1-p2-line
///   p1/          \p3    |
/// \endcode

  int    i1, i2;
  double dev, d1;


  // printf("UT3D_ck_pta_maxDev %d\n",ptNr);

  if(ptNr < 3) return 0.;


  // test max deviation polygon pTab
  dev = 0.;
  i2  = 0;

  for(i1=2; i1<ptNr; ++i1) {
      // UT3D_stru_dump (Typ_PT, &pTab[i1], " p[%d]",i1);
    UT3D_nlenq_3pt (&d1, &pTab[i1 - 2], &pTab[i1 - 1], &pTab[i1]);
    if(d1 > dev) {
      dev = d1;
      i2 = i1;
    }
  }
  dev = UT3D_nlen_3pt (&pTab[i2 - 2], &pTab[i2 - 1], &pTab[i2]);

    // printf(" i2=%d dev=%f\n",i2,dev);

  return dev;

}


//================================================================
  int UT3D_ck_plg_par_trm (double *par, CurvPoly *plg) {
//================================================================
// UT3D_ck_plg_par_trm            test if point is on clipped parts
//   par       length absolut (lvTab)
//   Retcod:
//        0=on midpart part of plg; 
//        1=on clipped part of plg or outside plg

  double   *v0, *v1;

  // printf("UT3D_ck_plg_par_trm %lf v0=%lf v1=%lf dir=%d\n",*par,
         // plg->v0,plg->v1,plg->dir);

  if(!plg->dir) { v0 = &plg->v0; v1 = &plg->v1; }
  else          { v0 = &plg->v1; v1 = &plg->v1; }

  if(*par < (*v0 - UT_TOL_pt)) return 1;
  if(*par > (*v1 + UT_TOL_pt)) return 1;

  return 0;

}


//================================================================
  int UT3D_ck_plg_trim (CurvPoly *cv1) {
//================================================================
// UT3D_ck_plg_trim         check if polygon is limited
//   RetCod
//      0         polygon is unlimited
//     >0         polygon is limited;  nr of points (not exactly)
//
// see also UT3D_pta_plg

  int   i1, i2;

  // printf("UT3D_ck_plg_trim %f %f\n",cv1->v0,cv1->v1);


  if(UTP_comp2db (cv1->lvTab[0], cv1->v0, UT_TOL_pt) == 0) goto L_lim;
  i1 = cv1->ptNr - 1;
  if(UTP_comp2db (cv1->lvTab[i1], cv1->v1, UT_TOL_pt) == 0) goto L_lim;
  // polygon is unlimited
  return 0;


  // polygon is limited; compute nr of points
  L_lim:
  i2 = 0;
  for(i1=1; i1<cv1->ptNr; ++i1) {
    if(cv1->lvTab[i1] < cv1->v0) continue;
    if(cv1->lvTab[i1] > cv1->v1) break;
    ++i2;
  }

  i2 += 2; // + start-, endPt

    // printf("ex UT3D_ck_plg_trim %d\n",i2);


  return i2;

}


//================================================================
  int UT3D_ck_plgpar1lim (CurvPoly *plg, double par1) {
//================================================================
// UT3D_ck_plgpar1lim          check if par == start | endPt
// retCod:
//   0   par liegt inmitten des plg ..
//   1   par == lvTab[0]     == Startpunkt
//   2   par == lvTab[pNr-1] == Endpunkt

  double par_;

  par_ = UT3D_par_par1plg (par1, plg);

  return (UT3D_ck_plgparlim (plg, par_));
}



//================================================================
  int UT3D_ck_plgparlim (CurvPoly *plg, double par) {
//================================================================
// UT3D_ck_plgparlim          check if knotVal == start | endPt
// retCod:
//   0   par liegt inmitten des plg ..
//   1   par == lvTab[0]     == Startpunkt
//   2   par == lvTab[pNr-1] == Endpunkt
//  -1   par = < v0
//  -2   par = > v1

  int  ii;

  printf("UT3D_ck_plgparlim %lf \n",par);


  if(UTP_comp2db (par, plg->v0, UT_TOL_pt) == 1) return 1;   // ident
  if(par < plg->v0) return -1;

  if(UTP_comp2db (par, plg->v1, UT_TOL_pt) == 1) return 2;   // ident
  if(par > plg->v1) return -2;

/*  2011-01-11
  // test if par == startpt
  if(UTP_comp2db (par, plg->lvTab[0], UT_TOL_pt) == 1) return 1;   // ident
  ii = plg->ptNr - 1;
  if(UTP_comp2db (par, plg->lvTab[ii], UT_TOL_pt) == 1) return 2;   // ident
*/
  return 0;
}


//================================================================
  int UT3D_2pt_segnln (Point *p1, Point *p2, int is, Line *lna) {
//================================================================
/// UT3D_2pt_segnln            get segment from lineTable
        
      
  // int   i1;
      

  // printf("UT3D_2pt_segnln %d\n",is);
  // UT3D_stru_dump(Typ_LN, plg, "  ");
    
  *p1 = lna[is].p1;
  *p2 = lna[is].p2;

  return 0;

}


//=====================================================================
  int UT3D_2pt_plg_iseg (Point *p1, Point *p2, int is, CurvPoly *plg) {
//=====================================================================
// UT3D_2pt_plg_iseg            get segment from polygon
// Input:
//   is     segment-nr, seg.1 is first = point[0]-point[1]


  int     irc;
  // int     iis, iie, ii, ib, ips, ipe;
  double  par1, par2;


  // printf("UT3D_2pt_plg_iseg %d\n",is);
  // UT3D_stru_dump(Typ_CVPOL, plg, "UT3D_2pt_plg_iseg");


  if(is < 1) {
    TX_Error("UT3D_2pt_plg_iseg E001");
    return -1;
  }

  // get parameters of seg. is
  --is;    // set 0=first
  irc = UPLG_2par_iseg (&par1, &par2, is, plg);
  if(irc < 0) return -1;


  // get point for p1
  UT3D_pt_evalplg (p1, plg, par1);

  // get point for p2
  UT3D_pt_evalplg (p2, plg, par2);


    // UT3D_stru_dump(Typ_PT, p2, "_2pt_segplg-p2");


  return 0;



}


//================================================================
  int UT3D_ck_plgclo (CurvPoly *plg) {
//================================================================
/// \code
/// UT3D_ck_plgclo             check if polygon is closed and untrimmed
/// rc =  1:  No, polygon not closed
/// rc =  0:  YES, polygon is closed AND endpoint == end_of_curve
/// rc = -1:  NO, polygon is closed BUT endpoint is trimmed
/// rc = -2:  degenerated
/// \endcode


  int    ii;
  double d1, vTot;

  // printf("UT3D_ck_plgclo %f %f\n",plg->v1,plg->lvTab[plg->ptNr-1]);

  ii = plg->ptNr - 1;

  if(plg->lvTab[ii] < UT_TOL_cv) return -2; // degenerated

  if(UT3D_comp2pt(&plg->cpTab[0], &plg->cpTab[ii], UT_TOL_pt) == 0) return 1;

  // yes closed (first and last point is identical)

  // check difference v1-v0
  vTot = plg->lvTab[ii];
  d1 = fabs(plg->v1 - plg->v0);
  if(UTP_comp2db(d1, vTot, UT_TOL_pt)) return 0; // 1=closed > 0

/*
  // is endpoint == end of curve ?
  if(UTP_comp2db(plg->v0, plg->lvTab[0], UT_TOL_pt) == -1) return -1;   // 0=diff
  if(UTP_comp2db(plg->v1, plg->lvTab[ii], UT_TOL_pt) == -1) return -1;
  // printf(" plgclo 0\n");
*/

  return -1; // closed, but trimmed

}


//===============================================================================
  int UT3D_plg_parl_pln (CurvPoly *plgo, CurvPoly *plgi, Vector *vz, double dist,
                         Memspc *memSeg1) {
//===============================================================================
// UT3D_plg_parl_pln          Polygon = parallel to polygon on plane, dist
// Input:
//   vz    normalvektor of plane for plgo

  int    irc, ptNr, ptNo, iClo;
  double *vTabi, *vTabo, d1, d2;
  Point  *pTabi, *pTabo;
  Vector vc1, vc2;


  UT3D_stru_dump (Typ_CVPOL, plgi, "UT3D_plg_parl_pln");

  *plgo = *plgi;

  ptNr  = plgi->ptNr;
  pTabi = plgi->cpTab;
  vTabi = plgi->lvTab;
  // iClo  = plgi->clo;
  iClo = UT3D_ck_plgclo (plgi);

  // get spc f ptNr points in memSeg1
  pTabo = UME_reserve (memSeg1, sizeof(Point) * ptNr);
  if(!pTabo) {
    TX_Error("Out of tempSpace in UT3D_plg_parl_pln");
    return -1;
  }

  // get spc f ptNr doubles in memSeg1
  vTabo = UME_reserve (memSeg1, sizeof(double) * ptNr);
  if(!vTabo) {
    TX_Error("Out of tempSpace in UT3D_plg_parl_pln");
    return -1;
  }


  //----------------------------------------------------------------
  // update cpTab pTabo
  // offset pTabi -> pTabo
  irc = UT3D_npt_parl_pln (pTabo, &ptNo, pTabi, ptNr, vz, dist, iClo);
  if(irc < 0) return -1;


  //----------------------------------------------------------------
  // update lvTab vTabo
  d1 = UT3D_plg_lvTab (vTabo, pTabo, ptNo);
  d2 = d1 / plgi->v1;

  // update trimValues
  plgo->v0 = plgi->v0 * d2;
  plgo->v1 = plgi->v1 * d2;

  plgo->cpTab = pTabo;
  plgo->lvTab = vTabo;

  plgo->ptNr = ptNo;

    UT3D_stru_dump (Typ_CVPOL, plgo, " ex-UT3D_plg_parl_pln");

  return 0;

}


//=========================================================================
  int UT3D_plg_projplgpln (CurvPoly *plgo, CurvPoly *plgi, Plane *pln, 
                           Memspc *memSeg1) {
//=========================================================================
// UT3D_plg_projplgpln        Polygon = project Polygon -> Plane

// ACHTUNG: dzt fehlt check ob Punkte ausreichend Abstand voneinender haben !!

  int    i1, ptNr;
  Point  *pTabi, *pTabo;



  ptNr  = plgi->ptNr;
  pTabi = plgi->cpTab;



  // noch Platz f ptNr Punkte im memSeg1 ?
  pTabo = memSeg1->next;
  i1 = UME_add (memSeg1, sizeof(Point) * ptNr);
  if(i1 < 0) {
    TX_Error("Out of tempSpace in UT3D_plg_projplgpln");
    return -1;
  }



  // translate Points
  for(i1=0; i1<ptNr; ++i1) {
    UT3D_pt_projptpl (&pTabo[i1], pln, &pTabi[i1]);
  }


  // Points -> PolygonCurve.
  return UT3D_plg_pta (plgo, pTabo, ptNr, memSeg1);

}


//=========================================================================
  int UT3D_pt_evalplg (Point *pto, CurvPoly *plg, double parPlg) {
//=========================================================================
/// \code
/// UT3D_pt_evalplg            get point on polygon from parameter
/// parPlg    parameter; distance from startpoint (not 0-1)
/// \endcode

  int    i1;
  double *lTab, d1;
  Point  *pTab;


  // printf("UT3D_pt_evalplg par=%lf\n",parPlg);
  // UT3D_stru_dump (Typ_CVPOL, plg, "  plg:");


  pTab = plg->cpTab;
  lTab = plg->lvTab;



  // im oder vor dem ersten segment-
  if(parPlg < lTab[1]) {
    UT3D_pt_traptptlen (pto, &pTab[0], &pTab[1], parPlg);
    // printf(" 1.seg \n");
    goto L_exit;
  }


  // am oder nach dem letzten segment-
  if(parPlg > lTab[plg->ptNr-2]) {
    i1 = plg->ptNr-1;  // index auf letzten
    d1 = parPlg - lTab[i1-1];
    UT3D_pt_traptptlen (pto, &pTab[i1-1], &pTab[i1], d1);
    // printf(" last seg \n");
    goto L_exit;
  }


  // dazwischen ..
  for(i1=1; i1<plg->ptNr; ++i1) {
    if(lTab[i1] < parPlg) continue;
    d1 = parPlg - lTab[i1-1];
    // printf(" seg %d d1=%f\n",i1,d1);
    UT3D_pt_traptptlen (pto, &pTab[i1-1], &pTab[i1], d1);
    break;
  }


  L_exit:
    // printf("ex _evalplg %lf %lf %lf\n",pto->x,pto->y,pto->z);
  return 0;

}


//================================================================
  int UT3D_pt_intlnplg (int *nxp, Point *xptab, double *vtab,
                        Line *ln, CurvPoly *plg, double tol) {
//================================================================
/// \code
/// UT3D_pt_intlnplg       intersect line and polygon (both limited)
/// 
/// UT3D_pt_intlnplg       Author: Thomas Backmeister       2.7.2003
/// 
/// IN:
///   int *nxp        ... maximal number of output points
///   Line *ln        ... 3D-line
///   CurvPoly *plg    ... points of polygon
/// OUT:
///   int *nxp        ... number of intersection points
///   Point *xptab    ... intersection points
///   double *vtab    ... parameter of intersectionpoint (can be NULL)
/// Returncodes:
///   0 = OK
///  -1 = out of mem (xptab too small)
/// \endcode


  int    ptxMax, rc, i1;
  double dist;
  Point  ip1, ip2;
  Line   lnp;


  // UT3D_stru_dump (Typ_CVPOL, plg, "UT3D_pt_intlnplg ");
  // UT3D_stru_dump (Typ_LN, ln, " ln ");


  // init
  ptxMax = *nxp;
  *nxp   = 0;

  for (i1=0; i1<plg->ptNr-1; ++i1) {

    // line from 2 consecutive polygon points
    lnp.p1 = plg->cpTab[i1];
    lnp.p2 = plg->cpTab[i1+1];
      // UT3D_stru_dump (Typ_LN, &lnp, " lnp[%d]",i1);

    // intersect lnp with line
    // rc = UT3D_pt_int2ln (&ip1, &ip2, &dist, &lnp, ln);
    // if (rc < 1) continue;
    rc = UT3D_pt_int2pt2pt_lim (&ip1, NULL, NULL,
                                &lnp.p1, &lnp.p2, &ln->p1, &ln->p2, tol);
    if(rc < 0) continue;


    // // check if X-point is inside polygon line segment
    // if (!UT3D_pt_ck_inLine (&(lnp.p1), &(lnp.p2), &ip1, UT_TOL_min1)) continue;


    // wenn vorheriger Point gleich ist - skippen
    // passiert wenn pt = ControlPt.
    if(*nxp > 0) {
      if(UT3D_comp2pt (&xptab[(*nxp)-1], &ip1, UT_TOL_pt)) continue;
    }

    // save intersection point
    if (*nxp >= ptxMax) {
      TX_Error("UT3D_pt_intlnplg E002");
      return -1;
    }

    // parameter berechnen
    if(vtab) {
      dist = UT3D_len_2pt (&ip1, &lnp.p1);
      vtab[*nxp] = plg->lvTab[i1] + dist;
		  // printf(" v=%f voff[%d]=%f\n",vtab[*nxp],i1,plg->lvTab[i1]);
    }


    xptab[*nxp] = ip1;
    ++(*nxp);
  }

  return 0;
}


/*=======================================================================*/
  int UT3D_pt_intciplg (int *nxp, Point *xptab, double *vtab,
                        Circ *ci, CurvPoly *plg) {
/*====================
UT3D_pt_intciplg       intersect limited circle and polygon

UT3D_pt_intciplg       Author: Thomas Backmeister       2.7.2003

Intersection of circle and polygon.

IN:
  int *nxp        ... maximal number of output points
  Circ *ci        ... 3D-circle
  CurvPoly *plg    ... points of polygon
OUT:
  int *nxp        ... number of intersection points
  Point *xptab    ... intersection points
  double *vtab    ... parameters of intersection-points on polygon
Returncodes:
  0 = OK
 -1 = out of mem (xptab too small)
*/

  int    ptxMax, i1, i2, ii, nx;
  double dist;
  Point  xp[3];
  Line   lnp;

  // printf("\nUT3D_pt_intciplg: intersect circle and polygon\n");

  // init
  ptxMax = *nxp;
  *nxp   = 0;

  for (i1=0; i1<plg->ptNr-1; ++i1) {

    // line from 2 consecutive polygon points
    lnp.p1 = plg->cpTab[i1];
    lnp.p2 = plg->cpTab[i1+1];

    // intersect lnp with circle
    // if (UT3D_pt_intlnci__ (&nx, xp, &lnp, ci)) continue;
    nx = UT3D_pt_intlnci_l (xp, &lnp, 0, ci, 0);
    if(nx < 1) continue;


    for (i2=0; i2<nx; ++i2) {

      // // check if X-point is inside polygon line segment
      // if(!UT3D_pt_ck_inLine(&lnp.p1,&lnp.p2,&xp[i2],UT_TOL_min1)) continue;

      // wenn vorheriger Point gleich ist - skippen
      // passiert wenn pt = ControlPt.
      if(*nxp > 0) {
        if(UT3D_comp2pt (&xptab[(*nxp)-1], &xp[i2], UT_TOL_pt)) continue;
      }

      // save intersection point
      if (*nxp >= ptxMax) {
        TX_Error("UT3D_pt_intciplg E002");
        return -1;
      }

      // parameter berechnen
      dist = UT3D_len_2pt (&xp[i2], &lnp.p1);
      if(vtab) vtab[*nxp] = plg->lvTab[i1] + dist;
		  // printf(" v=%f voff[%d]=%f\n",vtab[*nxp],i1,plg->lvTab[i1]);

      xptab[*nxp] = xp[i2];
      ++(*nxp);
    }
  }

  // printf("ex UT3D_pt_intciplg: %d\n",*nxp);
  // for(i1=0;i1<*nxp;++i1)
    // UT3D_stru_dump (Typ_PT, &xptab[i1], " %d %f",i1,vtab[i1]);
  return 0;
}




//============================================================================
  int UT3D_pt_intelplg (int *nxp, Point *xptab, double *va_el, double *va_plg,
                        CurvElli *el, CurvPoly *plg) {
//============================================================================
/*
UT3D_pt_intelplg       intersect lim. ellipse and lim. polygon

UT3D_pt_intelplg       Author: Thomas Backmeister       2.7.2003

Intersection of ellipse and polygon.

IN:
  int *nxp         ... maximal number of output points (and parameters)
  CurvElli *el     ... ellipse
  CurvPoly *plg    ... polygon
OUT:
  int *nxp         ... number of intersection points
  Point *xptab     ... intersection points
  double *va_el    ... parameter of the intersection-points on ellipse;
                       none if va_el==NULL
  double *va_plg   ... parameter of the intersection-points on polygon;
                       none if va_plg==NULL
Returncodes:
  0 = OK
 -1 = out of mem (xptab too small)
 -2 = internal error
*/

  int    ptxMax, rc, i1, i2, nx;
  double dist, uu;
  Point  xp[3];
  Line   lnp;


  // printf("\nUT3D_pt_intelplg: intersect ellipse and polygon\n");

  // init
  ptxMax = *nxp;
  *nxp = 0;

  // get spc for parameters on elli
  if(!va_el) {
    va_el = (void*) MEM_alloc_tmp ((int)(ptxMax * sizeof(double)));
      // printf(" tmpSpc for va_el..\n");
  }


  for (i1=0; i1<plg->ptNr-1; ++i1) {

    // line from 2 consecutive polygon points
    lnp.p1 = plg->cpTab[i1];
    lnp.p2 = plg->cpTab[i1+1];

    // intersect lnp with ellipse
    rc = UT3D_pt_intlnel__ (&nx, xp, &lnp, el);
    if (rc < 0) return -2;
    if (rc == 1) continue;

    for (i2=0; i2<nx; ++i2) { 

      // check if X-point is inside polygon line segment
      if(!UT3D_pt_ck_inLine(&lnp.p1,&lnp.p2,&xp[i2],UT_TOL_min1)) continue;

      // wenn vorheriger Point gleich ist - skippen
      // passiert wenn pt = ControlPt.
      if(*nxp > 0) {
        if(UT3D_comp2pt (&xptab[(*nxp)-1], &xp[i2], UT_TOL_pt)) continue;
      }

      // save intersection point
      if (*nxp >= ptxMax) {
        TX_Error("UT3D_pt_intelplg E002");
        return -1;
      }

      // parameter on elli berechnen
      UT3D_par1_el_pt (&uu, el, &xp[i2]);
        // printf(" uu=%lf\n",uu);
      // test if point is on triommed ellipse
      if(!UT3D_par_ck_inObj__(uu, &xp[i2], &el->p1, &el->p2, 0., 1.)) continue;
      va_el[*nxp] = uu;

      // parameter on plg berechnen
      if(va_plg) {
        dist = UT3D_len_2pt (&xp[i2], &lnp.p1);
        va_plg[*nxp] = plg->lvTab[i1] + dist;
		      // printf(" v=%f voff[%d]=%f\n",vtab[*nxp],i1,plg->lvTab[i1]);
      }

      xptab[*nxp] = xp[i2];
      ++(*nxp);
    }
  }

  return 0;
}


/*=======================================================================*/
  int UT3D_pt_intbspplg (int *nxp, Point *xptab, double *vtab, Memspc *memSeg1,
                         CurvBSpl *bspl, CurvPoly *plg) {
/*=====================
UT3D_pt_intbspplg       intersect b-spline curve and polygon

UT3D_pt_intbspplg       Author: Thomas Backmeister       2.7.2003

Intersection of b-spline curve and polygon.

IN:
  int *nxp        ... maximal number of output points
  Memspc *memSeg1 ... memory for temporary elements
  CurvBSpl *bspl  ... b-spline curve
  CurvPoly *plg    ... points of polygon
OUT:
  int *nxp        ... number of intersection points
  Point *xptab    ... intersection points
  double *vtab    ... parametervalue of the intersection-points on the CurvPoly
                      none if vtab == NULL
Returncodes:
  0 = OK
 -1 = out of tempSpace
 -2 = out of mem (xptab too small)
*/

  int     rc, i1, i2, i3, ptxMax, ptxNr;
  double  dist;
  CurvBez *btab;
  ObjGX   beztab;
  Point   pTab1[UT_BEZDEG_MAX+1];
  Line    lnp;
  void    *memPos0, *memPos1;


  printf("UT3D_pt_intbspplg %d\n",*nxp);

  // init
  rc = 0;
  ptxMax = *nxp;
  *nxp = 0;
  memPos0 = memSeg1->next;


  // convert b-spline to Bezier curves
  UT3D_bez_bspl__ (&beztab, memSeg1, bspl);
  if (beztab.data == NULL) {
    TX_Error("UT3D_pt_intbspplg: out of tempSpace");
    rc = -1;
    goto Fertig;
  }
  btab = (CurvBez*) (beztab.data);
  memPos1 = memSeg1->next;


  // --- for all polygon line segments ---
  for (i1=0; i1<plg->ptNr-1; ++i1) {

    // line from 2 consecutive polygon points
    lnp.p1 = plg->cpTab[i1];
    lnp.p2 = plg->cpTab[i1+1];
    

    // --- for all Bezier curves ---
    for (i2=0; i2<beztab.siz; ++i2) {

      // intersect line with Bezier curve
      ptxNr = 0;
      rc = UT3D_pt_intlnbcv (&ptxNr, pTab1, UT_BEZDEG_MAX,
                             &lnp, &btab[i2], memSeg1, UT_TOL_ln);
      if (rc < 0) goto Fertig;


      for (i3=0; i3<ptxNr; ++i3) { 

        // check if X-point is inside polygon line segment
        if(!UT3D_pt_ck_inLine (&(lnp.p1), &(lnp.p2), &(pTab1[i3]), UT_TOL_min1))
          continue;

        // wenn vorheriger Point gleich ist - skippen
        // passiert wenn pt = ControlPt.
        if(*nxp > 0) {
          if(UT3D_comp2pt (&xptab[(*nxp)-1], &pTab1[i3], UT_TOL_pt)) continue;
        }

        // save intersection point
        if (*nxp >= ptxMax) {
          TX_Error("UT3D_pt_intbspplg: xptab too small");
          rc = -2;
          goto Fertig;
        }

        // parameter berechnen
        if(vtab) {
          dist = UT3D_len_2pt (&pTab1[i3], &lnp.p1);
          vtab[*nxp] = plg->lvTab[i1] + dist;
		        // printf(" v=%f voff[%d]=%f\n",vtab[*nxp],i1,plg->lvTab[i1]);
        }

        xptab[*nxp] = pTab1[i3];
        ++(*nxp);
      }

      // reset memSeg1
      memSeg1->next = memPos1;
    }
  }


  Fertig:
  // reset memSeg1
  memSeg1->next = memPos0;

  return rc;
}



//=======================================================================
  int UT3D_pt_intplplg (int *pNr, Point *pa, Plane *pln, CurvPoly *plg) {
//=======================================================================
// UT3D_pt_intplplg           intersect Plane polygon

// Input:
// pNr = max.Nr.inters.points/parameters

  int    i1, i2, ii, pSiz;
  double dl, dn1, dn2;


  // printf("UT3D_pt_intplplg %d\n",plg->ptNr);


  pSiz  = *pNr-1;
  ii = 0;
  i2 = plg->ptNr-1;

  dn2 = UT3D_slen_ptpl (&plg->cpTab[0], pln);

  for (i1=0; i1<i2; ++i1) {
    // UT3D_vc_2pt (&vln, &plg->cpTab[i1], &plg->cpTab[i1+1]);
    // irc = UT3D_pt_intptvcpl_ (&pa[ii], pln, &pt1, &vln);
    dn1 = dn2;
    dn2 = UT3D_slen_ptpl (&plg->cpTab[i1+1], pln);
    UT3D_pt_intlnpl1 (&pa[ii], &dl,
                      &plg->cpTab[i1], &plg->cpTab[i1+1], dn1, dn2);
    // printf(" _pt_intplplg %d %f %f %f\n",i1,dl,dn1,dn2);
    if(dl < 0.) continue;
    if(dl > 1.) continue;
    if(ii > 0) {  // skip cornerpoint (comes 2. time)
      if(UT3D_comp2pt(&pa[ii], &pa[ii-1], UT_TOL_pt) == 1) continue;
    }
    if(ii >= pSiz) {TX_Error("UT3D_pt_intplplg EOM"); return -1;}
    ++ii;
  }

  *pNr = ii;
  // printf("ex UT3D_pt_intplplg %d\n",*pNr);
  return 0;

}


/*=======================================================================*/
  int UT3D_pt_int2plg (int *nxp, Point *xptab, double *vtab1, double *vtab2,
                       CurvPoly *plg1, CurvPoly *plg2) {
/*===================
UT3D_pt_int2plg       intersect 2 polygons

UT3D_pt_int2plg       Author: Thomas Backmeister       2.7.2003

Intersection of two polygons.

IN:
  int ptxMax      ... maximal number of output points
  CurvPoly *plg1   ... points of polygon
  CurvPoly *plg2   ... points of polygon
OUT:
  int *nxp        ... number of intersection points
  Point *xptab    ... intersection points
  vtab1                 
  double *vtab1    ... parameter of the intersection-points on plg1;
                       none if vtab1==NULL
  double *vtab2   ... parameter of the intersection-points on plg2;
                       none if vtab2==NULL

Returncodes:
  0 = OK
 -1 = out of mem (xptab too small)
*/

  int    ptxMax, rc, i1, i2;
  double dist;
  Point  ip1, ip2;
  Line   lnp1, lnp2;


  // init
  ptxMax = *nxp;
  *nxp = 0;

  for (i1=0; i1<plg1->ptNr-1; ++i1) {

    // line from 2 consecutive points of polygon 1
    lnp1.p1 = plg1->cpTab[i1];
    lnp1.p2 = plg1->cpTab[i1+1];

      for (i2=0; i2<plg2->ptNr-1; ++i2) {

      // line from 2 consecutive points of polygon 2
      lnp2.p1 = plg2->cpTab[i2];
      lnp2.p2 = plg2->cpTab[i2+1];

      // intersect both lines
      rc = UT3D_pt_int2ln (&ip1, &ip2, &dist, &lnp1, &lnp2);
      if (rc < 1) continue;

      // check if X-point is inside both polygon line segments
      if(!UT3D_pt_ck_inLine(&(lnp1.p1),&(lnp1.p2),&ip1,UT_TOL_min1)) continue;
      if(!UT3D_pt_ck_inLine(&(lnp2.p1),&(lnp2.p2),&ip1,UT_TOL_min1)) continue;

      // wenn vorheriger Point gleich ist - skippen
      // passiert wenn pt = ControlPt.
      if(*nxp > 0) {
        if(UT3D_comp2pt (&xptab[(*nxp)-1], &ip1, UT_TOL_pt)) continue;
      }

      // save intersection point
      if (*nxp >= ptxMax) {
        TX_Error("UT3D_pt_int2plg E002");
        return -1;
      }

      // parameter on plg1 berechnen
      if(vtab1) {
        dist = UT3D_len_2pt (&ip1, &lnp1.p1);
        vtab1[*nxp] = plg1->lvTab[i1] + dist;
        // printf(" v=%f voff[%d]=%f\n",vtab[*nxp],i1,plg->lvTab[i1]);
      }

      // parameter on plg2 berechnen
      if(vtab2) {
        dist = UT3D_len_2pt (&ip1, &lnp2.p1);
        vtab2[*nxp] = plg2->lvTab[i2] + dist;
        // printf(" v=%f voff[%d]=%f\n",vtab[*nxp],i1,plg->lvTab[i1]);
      }

      xptab[*nxp] = ip1;
      ++(*nxp);
    }
  }

  return 0;
}


//================================================================
  int UT3D_par1_plg_pt (double *par1, Point *pt, CurvPoly *plg) {
//================================================================
// UT3D_par1_plg_pt           get parameter 0-1 for point on polygon

  int   irc;

  // get length for point on plg
  irc = UT3D_parplg_plgpt (par1, pt, plg);
  if(irc < 0) return -1;
  // Knotenwert in einen Parameterwert von 0-1 aendern
  *par1 = UT3D_par1_parplg (par1, plg);

  return 0;

}


//================================================================
  int UT3D_parplg_plgpt (double *parplg, Point *pt, CurvPoly *plg) {
//================================================================
/// UT3D_parplg_plgpt           get knotvalue from point on polygon

  int   rc, i1;


  // UT3D_stru_dump(Typ_PT, pt, "UT3D_parplg_plgpt");
  // UT3D_stru_dump(Typ_CVPOL, plg, "  plg:");


  for (i1=0; i1<plg->ptNr-1; ++i1) {

    // UT3D_stru_dump(Typ_PT, &plg->cpTab[i1], " p[%d]",i1);
    // UT3D_stru_dump(Typ_PT, &plg->cpTab[i1+1], " p[%d]",i1+1);
    rc = UT3D_pt_ck_onLine (&plg->cpTab[i1], &plg->cpTab[i1+1], pt, UT_TOL_pt);
    // -1:Point outside 0:pt==cpTab[i1], 1:pt between, 2:pt==cpTab[i1+1]

    if(rc < 0) continue;


    if(rc == 0) {
      *parplg = plg->lvTab[i1];

    } else if(rc == 1) {
      *parplg = plg->lvTab[i1] + UT3D_len_2pt (&plg->cpTab[i1], pt);

    } else if(rc == 2) {       // pt ==  p2
      *parplg = plg->lvTab[i1+1];;
    }

      // printf("ex UT3D_parplg_plgpt %d %f\n",rc,*parplg);

    return 0;
  }


  // printf("ex UT3D_parplg_plgpt -1\n");

  return -1;

}


//===========================================================================
  int UT3D_vc_evalplg (Vector *vco, CurvPoly *plg, double pVal) {
//===========================================================================
// UT3D_vc_evalplg            Tangente an PolygonCurve aus Parameterwert


  int    i1;
  double *lTab;
  Point  *pTab;
  Vector vc1, vc2;


  // printf("UT3D_vc_evalplg %f\n",pVal);
  // UT3D_stru_dump(Typ_CVPOL, plg, "");


  pTab = plg->cpTab;
  lTab = plg->lvTab;



  // liegt pVal genau auf einem Knotenpunkt ?
  for(i1=1; i1<plg->ptNr-1; ++i1) {
    if(UTP_comp2db(pVal, lTab[i1], UT_TOL_pt)) {
      // printf(" _vc_ev 1\n");
      UT3D_vc_2pt (&vc1, &pTab[i1-1], &pTab[i1]);
      UT3D_vc_2pt (&vc2, &pTab[i1], &pTab[i1+1]);
      UT3D_vc_setLength (&vc1, &vc1, 1.);
      UT3D_vc_setLength (&vc2, &vc2, 1.);
      UT3D_vc_2vc (vco, &vc1, &vc2);
      goto L_done;
    }
  }


  // im ersten segment ?
  if(pVal < lTab[1]) {
    // printf(" _vc_ev 2\n");
    UT3D_vc_2pt (vco, &pTab[0], &pTab[1]);
    goto L_done;
  }


  // im letzten segment ?
  if(pVal > lTab[plg->ptNr-2]) {
    // printf(" _vc_ev 3\n");
    UT3D_vc_2pt (vco, &pTab[plg->ptNr-2], &pTab[plg->ptNr-1]);
    goto L_done;
  }


  // pVal liegt zwischen den Knotenpunkten (i1-1) und i1.
  for(i1=1; i1<plg->ptNr-1; ++i1) {
    if(pVal > lTab[i1]) continue;
    // printf(" _vc_ev 4 %d\n",i1);
    UT3D_vc_2pt (vco, &pTab[i1-1], &pTab[i1]);
    break;
  }

  L_done:
  // UT3D_stru_dump(Typ_VC, vco, "ex UT3D_vc_evalplg:");
  return 0;

}


/*
//===========================================================================
  int UT3D_pt_evalplg (Point *pto, CurvPoly *plg, double uVal) {
//===========================================================================

  int    i1, i2;
  double *lTab, d1;
  Point  *pTab;




  pTab = plg->cpTab;
  lTab = plg->lvTab;



  // Startpunkt berechnen.
  for(i1=1; i1<plg->ptNr; ++i1) {

    // printf(" l1:[%d] p=%f\n",i1,lTab[i1]);
    // printf(" pi[%d]=%f %f %f\n",i1,pTab[i1].x,pTab[i1].y,pTab[i1].z);

    if(lTab[i1] < uVal) continue;

    // Startpunkt erechnen.
    d1 = uVal - lTab[i1-1];
    UT3D_pt_traptptlen (pto, &pTab[i1-1], &pTab[i1], d1);
    break;
  }


  printf("ex UT3D_pt_evalplg %f %f %f\n",pto->x,pto->y,pto->z);

  return 0;

}
*/



//===========================================================================
  int UT3D_pta_ccw_plg (int *ptNr, Point *pta, CurvPoly *plg) {
//===========================================================================
/// \code
/// UT3D_pta_plg               Punktetabelle (relimited) aus PolygonCurve
/// Get (relimited) poylygon from PolygonCurve (eg for display).
/// PolygonCurve must be fwd (see UT3D_pta_plg).
/// 
/// Input:
///   ptNr          size of pta (should be plg->ptNr)
/// 
/// Returncodes:
///   0 = OK
///  -1 = out of tempSpace
/// 
/// 
/// TODO: add points to pta (problem UT3D_comp2pt(&pTab[i2], &pta[iOut - 1]...)
/// \endcode


  int    i1, i2, irc, maxPt, iOut, iClo;
  double *lTab, d1, d2, over;
  Point  pt1, *pTab;


  // printf("....................................................... \n");
  // printf("UT3D_pta_ccw_plg ptNr=%d v0=%f v1=%f\n",plg->ptNr,plg->v0,plg->v1);
  // UT3D_stru_dump(Typ_CVPOL, plg, "UT3D_pta_ccw_plg");
  

  maxPt = *ptNr;
  iOut  = 0;
  irc = 0;

  pTab = plg->cpTab;
  lTab = plg->lvTab;
  i2   = 1;


  if(maxPt < 1) return -1;

  // iClo = UT3D_ck_plgclo (plg);
    // printf(" iClo=%d\n",iClo);


  //------------------------------------------------------------------
  // find startpoint
  for(i1=1; i1<plg->ptNr; ++i1) {
    // printf(" l1:[%d] p=%f\n",i1,lTab[i1]);
    // printf(" pi[%d]=%f %f %f\n",i1,pTab[i1].x,pTab[i1].y,pTab[i1].z);
    if(lTab[i1] < plg->v0) continue;
    // Startpunkt erechnen.
    d1 = plg->v0 - lTab[i1-1];
    UT3D_pt_traptptlen (&pta[iOut], &pTab[i1-1], &pTab[i1], d1);
      // printf(" startPt pa[%d]=%f %f %f\n",iOut,
                // pta[iOut].x,pta[iOut].y,pta[iOut].z);
    ++iOut;
    i2 = i1;
    break;
  }

  // i2 is the index of the next point;
  // iOut is the nr of existing outputpoints


  //------------------------------------------------------------------
  // Zwischenpunkte kopieren.

  // v0,v1 identical ?
  if(UTP_comp2db(plg->v0, plg->v1, UT_TOL_pt)) {
    *ptNr = 1;
    goto L_exit;
  }


  // test if 1. point is identical
  if(UT3D_comp2pt(&pTab[i2], &pta[iOut - 1], UT_TOL_pt)) ++i2;

  // add following oints
  for(i1=i2; i1<plg->ptNr; ++i1) {
      // printf(" nxt l2:[%d] p=%f\n",i1,lTab[i1]);
    if(iOut >= maxPt) goto L_err1;  // overflow outArray 
    // Punkt kopieren
    if(lTab[i1] < plg->v1) {
      pta[iOut] = pTab[i1];
        // printf(" cp[%d]=%f %f %f\n",iOut,pta[iOut].x,pta[iOut].y,pta[iOut].z);
      ++iOut;
      continue;
    }
    i2 = i1;
    goto L_ende;
  }
    // printf(" all points done ..\n");

  *ptNr = iOut;

  // already at end ?
  if(UTP_comp2db(lTab[plg->ptNr - 1], plg->v1, UT_TOL_pt)) goto L_9;


  //------------------------------------------------------------------
  L_ende:
    // printf(" L_ende: i2=%d\n",i2);
  // test if endpoint
  // Endpunkt erechnen.
  d1 = plg->v1 - lTab[i2-1];
    // printf(" i2=%d d1=%f\n",i2,d1);
  UT3D_pt_traptptlen (&pta[iOut], &pTab[i2-1], &pTab[i2], d1);
  // printf(" pe[%d]=%f %f %f\n",iOut,pta[iOut].x,pta[iOut].y,pta[iOut].z);
  ++iOut;


  L_9:
  *ptNr = iOut;


  //------------------------------------------------------------------
  L_exit:
    // TEST-ONLY
    // for(i1=0;i1<*ptNr;++i1) UT3D_stru_dump(Typ_PT,&pta[i1],"pta[%d]",i1);
    // TEST-ONLY

  return 0;


  L_err1:  // overflow outArray 
    TX_Error ("UT3D_pta_plg overflow outArray");
    return -1;

}


//===========================================================================
  int UT3D_pt_projptplg (int *nxp, Point *pto, double *ttab,
                         CurvPoly *plg, Point *pt) {
//===========================================================================
/// \code
/// UT3D_pt_projptplg          proj PT -> PolygonCurve
///
/// Input:
///   nxp          size of outputarry pto and ttab
///   plg          polygon
///   pt           point; find nearest points to this point on polygon.
/// Output:
///   nxp          nr of outputpoints in pto and values in ttab
///   pto          nearest points on polygon; first=nearest.
///   ttab         distance from start of curve for points in pto (v)
///                NULL = no parameters
///   Returncodes:
///                0 = OK
///                1 = no projectionpoint exists; nearest point selected
///               -1 = out of tempSpace
///
/// Nearest point to <pt> is in pto[0];
/// \endcode


  int    i1, ii, irc, iOut, nxMax;
  double *lTab, d1, dd, lSeg;
  Point  *pTab;
  Vector vp;


  // printf("PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP \n");
  // printf("UT3D_pt_projptplg Pt = %f %f %f\n",pt->x,pt->y,pt->z);
  // UT3D_stru_dump (Typ_CVPOL, plg, " plg ");


  nxMax = *nxp;

  pTab = plg->cpTab;
  lTab = plg->lvTab;
  iOut = 0;

  // printf(" plg.cp[0]=%9.2f %9.2f %9.2f\n",pTab[0].x,pTab[0].y,pTab[0].z);
  // printf("   lTab[0]=%9.2f\n",lTab[0]);

  for(i1=1; i1<plg->ptNr; ++i1) {

    // printf(" plg.cp[%d]=%9.2f %9.2f %9.2f\n",i1,
            // pTab[i1].x,pTab[i1].y,pTab[i1].z);
    // printf("   lTab[%d]=%9.2f\n",i1,lTab[0]);

    if(iOut+1 >= nxMax) return -1;

    // die Laenge diese Segments
    lSeg = lTab[i1] - lTab[i1-1];

    // proj. pt1 -> (&pTab[i1], &pTab[i1-1])
    UT3D_vc_2pt (&vp, &pTab[i1-1], &pTab[i1]);
    irc =  UT3D_pt_projptptvc (&pto[iOut], &d1, &dd, pt, &pTab[i1-1], &vp);
      // printf(" _projptptvc seg=%d irc=%d par=%lf lSeg=%lf\n",i1,irc,dd,lSeg);
    // irc = UT3D_pt_projpt2pt (&pto[iOut], &d1, pt, &pTab[i1-1], &pTab[i1]);
    if(irc > 1) continue;

    // d1 = signed distance (pTab[i1-1] - pt)
    // d1 = UT3D_slen_3pt (&pTab[i1-1], &pTab[i1], &pto[iOut]);
    d1 = dd * lSeg;
      // printf(" d1[%d] = %lf\n",i1,d1);

    if(d1 < -UT_TOL_pt) continue;
    if(d1 > lSeg+UT_TOL_pt) continue;

      // printf(" found [%d] %f %f %f\n",i1,d1,lSeg,lTab[i1]);
      // GR_Disp_pt (&pto[iOut], SYM_STAR_S, 2);


    // wenn vorheriger Point gleich ist - skippen
    // passiert wenn pt = ControlPt.
    if(iOut > 0) {
      if(UT3D_comp2pt (&pto[iOut], &pto[iOut-1], UT_TOL_pt)) continue;
    }


    // set d1 = knotvalue for point
    d1 += lTab[i1-1];

    // test if point is on clipped parts
    irc = UT3D_ck_plg_par_trm (&d1, plg);
    // -1=outside plg; 0=on midpart part of plg; , 1=on clipped part of plg
    if(irc) continue;


    // speichern
    // printf(" save [%d] %f %f %f\n",i1,d1,lSeg,lTab[i1]);
    if(ttab) ttab[iOut] = d1;
    ++iOut;
  }

  *nxp = iOut;
  if(iOut < 1) goto L_noPt;

  
  // find point nearest to pt and make it the first outputPoint
  if(iOut < 2) return 0;
  dd = UT_VAL_MAX;
  for(i1=0; i1<iOut; ++i1) {
    d1 = UT3D_lenB_2pt (&pto[i1], pt);
      // UT3D_stru_dump (Typ_PT, &pto[i1], " pto[%d] ",i1);
      // printf(" %d _lenB_ = %f\n",i1,d1);
    if(d1 >= dd) continue;
      ii = i1;
      dd = d1;
  }

  // swap points
  if(ii != 0) {
      // printf(" swap 0 - %d\n",ii);
    MEM_swap__ (&pto[0], &pto[ii], sizeof(Point));
    if(ttab) MEM_swap_2db (&ttab[0], &ttab[ii]);
  }



    // TESTBLOCK
    // printf("PPPPPPPPPPPPPPPPPPP ex UT3D_pt_projptplg nxp=%d\n",*nxp);
    // for(i1=0; i1 < *nxp; ++i1) printf(" pa[%d] = %f %f %f ttab = %f\n",
                               // i1,pto[i1].x,pto[i1].y,pto[i1].z,ttab[i1]);

    // END TESTBLOCK


  return 0;




  //=========================================================
  // es wurde kein Punkt gefunden. Den naechsten Ctrlpt waehlen.
  L_noPt:
    // printf("L_noPt:\n");


  lSeg = UT_VAL_MAX;
  *nxp = 1;

  for(i1=0; i1<plg->ptNr; ++i1) {
    d1 = UT3D_len_2pt (pt, &pTab[i1]);
    if(d1 < lSeg) {
      lSeg = d1;
      pto[0]  = pTab[i1];
      if(ttab) ttab[0] = lTab[i1];
      irc = i1;
    }
  }
  TX_Print("**** PT -> Polygon: no prjPoint exists ..");
/*
  // Punkt 0: Tangente an das Startsegent.
  if(irc == 0) {
    UT3D_pt_projpt2pt (&pto[0], &d1, pt, &pTab[0], &pTab[1]);
    if(ttab) ttab[0] = UT3D_slen_3pt (&pTab[0], &pTab[1], &pto[0]);
    // printf(" slen=%f\n",ttab[0]);

  // letzter Punkt: Tangente an das Endsegment
  } else if(irc == plg->ptNr-1) {
    irc = plg->ptNr-1;
    UT3D_pt_projpt2pt (&pto[0], &d1, pt, &pTab[irc-1], &pTab[irc]);
    if(ttab) ttab[0] = UT3D_slen_3pt (&pTab[0], &pTab[1], &pto[0]) + lTab[irc];
    // printf(" slen=%f < %f\n",ttab[0], lTab[irc]);
  }
*/


    // printf("PPPPPPPPPPPPPPPPPPP ex UT3D_pt_projptplg nxp=%d\n",*nxp);


  return 1;

}



//===========================================================================
  double UT3D_plg_lvTab (double *lTab, Point *pTab, int ptNr) {
//===========================================================================
/* Polygon -> PolygonCurve; create lvTab for pta.
 size of lvTab is ptNr !

Returncode:
  totalLength = v1 = endParaamter; startPrameter v0 = 0.0.
*/

  int      i1;
  double   d1, lTot;


  lTot    = 0.;
  lTab[0] = 0.;

  for(i1=1; i1<ptNr; ++i1) {
    // printf(" plg.cp[%d]=%9.2f %9.2f %9.2f\n",i1,
            // pTab[i1].x,pTab[i1].y,pTab[i1].z);

    d1 = UT3D_len_2pt(&pTab[i1], &pTab[i1-1]);
    lTot += d1;
    lTab[i1] = lTot;
    // printf(" [%d] %f l=%f\n",i1,d1,lTot);
  }

  return lTot;

}


//===========================================================================
  int UT3D_plg_pta (CurvPoly *plg1, Point *pta, int ptNr, Memspc *memSeg1) {
//===========================================================================
/* Polygon -> PolygonCurve; von vo -> v1; vor und rueck (v0>v1)!
Returncodes:
  0 = OK
 -1 = out of tempSpace
*/

// cpTab: es werden alle Punkte -> memSeg1 kopiert.
// lvTab: wird in memSeg1 erstellt !

// // cpTab: es wird nur der pointer auf pta gespeichert;


  int     i1;
  long    oSiz;
  double  d1, lTot, *lTab;
  Point   *pTab;






  // provide memory for controlpoints
  pTab = memSeg1->next;
  oSiz = sizeof(Point) * ptNr;
  i1 = UME_add (memSeg1, oSiz);
  if(i1 < 0) goto L_eom;

  // copy points
  memcpy(pTab, pta, oSiz);


  // // nicht die Punkte kopieren; nur den Pointer merken.
  // pTab = pta;


  // provide memory for controlpoints
  lTab        = memSeg1->next;
  i1 = UME_add (memSeg1, sizeof(double) * ptNr);
  if(i1 < 0) goto L_eom;

  // laengen errechnen und -> lTab kopieren
  lTot = UT3D_plg_lvTab (lTab, pTab, ptNr);

/*
  lTot    = 0.;
  lTab[0] = 0.;
  
  for(i1=1; i1<ptNr; ++i1) {
    // printf(" plg.cp[%d]=%9.2f %9.2f %9.2f\n",i1,
            // pTab[i1].x,pTab[i1].y,pTab[i1].z);

    d1 = UT3D_len_2pt(&pTab[i1], &pTab[i1-1]);
    lTot += d1;
    lTab[i1] = lTot;
    // printf(" [%d] %f l=%f\n",i1,d1,lTot);
  }
*/



  plg1->ptNr  = ptNr;
  plg1->cpTab = pTab;
  plg1->lvTab = lTab;
  plg1->v0    = 0.;
  plg1->v1    = lTot;

  return 0;




  L_eom:
    TX_Error("UT3D_plg_pta: out of tempSpace");
    return -1;

}


//================================================================
  int UT3D_plg_npar1_npar (double *da, CurvPoly *cv1) {
//================================================================
/// get knotTable (length) with values from 0-1 for polygon

  int    i1;
  double ds, dd;


  // UT3D_stru_dump (Typ_CVPOL, cv1, "UT3D_plg_npar1_npar");


  ds = cv1->v0;
  dd = cv1->v1 - ds;

  for(i1=0; i1 < cv1->ptNr; ++i1) {
    da[i1] = (cv1->lvTab[i1] - ds) / dd;
  }

    // for(i1=0;i1<cv1->ptNr;++i1) printf(" da[%d]=%lf\n",i1,da[i1]);

  return 0;

}


//================================================================
  double UT3D_par_par1plg (double pv, CurvPoly* cv1) {
//================================================================
/// UT3D_par_par1plg           Parameterwert von 0-1 in einen Knotenwert aendern
// see UT3D_par1_parbsp

  // int    ii;
  double kv, kMax, kTot, k0, k1;


  // printf("UT3D_par_par1plg pv=%lf %lf %lf\n",pv,cv1->v0,cv1->v1);

  k0 = cv1->v0;
  k1 = cv1->v1;
    
  if(cv1->dir) {
    MEM_swap_2db (&k0, &k1);
    pv = 1. - pv;
  }
    

  // ii = cv1->ptNr - 1;
  kMax = cv1->lvTab[cv1->ptNr - 1];
    // printf(" lenTot = %f\n",kMax);


  if(k1 < k0) {
    kTot = kMax - k0 + k1;
  } else {
    kTot = k1 - k0;
  }

  kv = k0 + (kTot * pv);
  if(kv > kMax) kv = kv - kMax;


    // printf("ex UT3D_par_par1plg %lf %lf %lf\n",kv,cv1->v0,cv1->v1);

  return kv;
}




//================================================================
  double UT3D_par1_parplg (double *kv, CurvPoly* cv1) {
//================================================================
/// UT3D_par1_parplg           Knotenwert in einen Parameterwert von 0-1 aendern
// see UT3D_par_par1plg

  // int      ii;
  double   pv, k0, k1, kMax, kTot;

  // printf("UT3D_par1_parplg %f %f %f\n",*kv,cv1->v0,cv1->v1);

  k0 = cv1->v0;
  k1 = cv1->v1;

  if(cv1->dir) MEM_swap_2db (&k0, &k1);


  kMax = cv1->lvTab[cv1->ptNr - 1];
    // printf("UT3D_par1_parplg k0=%f k1=%f kMax=%f\n",k0,k1,kMax);

  if(k1 < k0) {
    // kmax - u1 + k1 - umin
    kTot = kMax - k0 + k1;
    if(*kv > k0) pv = (*kv - k0) / kTot;
    else         pv = (kMax - k0 + *kv) / kTot;

  } else {
    kTot = k1 - k0;
    pv = (*kv - k0) / kTot;
  }

  if(cv1->dir) pv = 1. - pv;

    // printf("ex UT3D_par1_parplg %f kv=%f kTot=%f\n",pv,*kv,kTot);

  return pv;

}


//================================================================
  int UPLG_iseg_par (int *segNr, double up, CurvPoly *cv1) {
//================================================================
// get segmentNr from parameter
//   up          parameter
//   segNr       first seg is 0 (cpTab[0]-cpTab[1]) ..

  int    i1;
  double u1;

  // printf("UPLG_iseg_par %f\n",up);
  // UT3D_stru_dump (Typ_CVPOL, cv1, "");

  *segNr = cv1->ptNr - 2;
  for(i1=1; i1 < cv1->ptNr - 1; ++i1) {
    u1 = cv1->lvTab[i1];
    if(u1 > up)  { *segNr = i1 - 1; break;}
  }

  // printf("ex UPLG_iseg_par %d\n",*segNr);

  return 0;

}




//===================== EOF ===========================
