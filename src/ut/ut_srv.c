//          Revolved-Surface-Functions    RF.
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
2008-12-21 neu ex ut_surstd.c. RF.

-----------------------------------------------------
*/
/*!
\file  ../ut/ut_srv.c
\brief Revolved-Surface-Functions 
\code
=====================================================
List_functions_start:

?                         u/v-Parameter from  3D-Revolved-Surface-Point
SRV_pt_evparsrv           create 3D-point from U,V-parameters
SRV_CvIso_parsrv          get iso-curve from RevolvedSurface
SRV_axis1                 get starting-Axis-origin & direction

SRV_pt_int_ln_srv         intersect line X Cyl|Cone
SRV_pt_int_ln_sCon        intersect line lnx X conicSurf
SRV_pt_int_ln_sCyl        intersect line lnx X cylinderSurface

List_functions_end:
=====================================================
- see also:
../ut/ut_surstd.c
../ut/ut_sru.c

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
#include "../ut/func_types.h"          // UI_Func... SYM_..
#include "../xa/xa_msg.h"              // MSG_* ERR_*




//============================================================================
  int SRV_pt_int_ln_sCon (int *nxp, Point *xpTab,
                          Line *lnx, CVLn3 *cvAx, CVLn3 *cvCov, SurRev *srv) {
//============================================================================
// SRV_pt_int_ln_sCon                intersect line lnx X conicSurf
// Input:
//   cvAx    axis
//   cvCov   contourline
// Output:
//   irc     0 = normal, 0 or 1 or 2 intersectionpoints
//           1 = line touches cylinder
//          -1   Error
//
//  cone; the startpoint of cvCov has ang=0 rad;
//
// - get top-of-cone-point
// - get plane plni from top-of-cone-point and line lnx
// - get ip1/ip2 from intersect cirBas with this plane plni
// - intersect line (ip1/ip2 - top-of-cone-point) with line lnx
//
// see ../../doc/geom/SRV_pt_int_ln_sCyl.png


// ../gr/tess_su.c:4020

  int      irc, ipa[2], ipBnr;
  double   d1, d2, tolcv, tolpt, parTopC, parTopA, rdc, angs, ango, angx,
           parLnx, parCov, parV0, parV1;
  Point    ptTop, ptBa[2];
  Line     lnib;
  Circ     ciB;
  Plane    plni, plBot;
  CVLn3    cvLnx, cvLni;
  Vector   vc1;


  // printf("SRV_pt_int_ln_sCon --------------\n");
  // DEB_dump_obj__ (Typ_CVLN3, cvAx, " cvAx");
  // DEB_dump_obj__ (Typ_CVLN3, cvCov, " cvCov");
  // DEB_dump_obj__ (Typ_SURRV, srv, " srv:");


  *nxp = 0;
  tolcv = UT_TOL_cv;
  tolpt = UT_TOL_pt;



  // get top-of-cone (intersect axis-contour)
  irc = UT3D_2par_int2pt2vc (&parTopC, &parTopA,
                             &cvCov->pt0, &cvCov->vcl,   // contour
                             &cvAx->pt0, &cvAx->vcl);    // axis
    // printf(" int2pt2vc irc=%d parTopC=%f, parTopA=%f\n",irc,parTopC,parTopA);

  // get ptTop = top-of-cone-point on contourLine
  UT3D_pt_traptmultvc (&ptTop, &cvCov->pt0, &cvCov->vcl, parTopC);
    // GR_tDyn_symB__ (&ptTop, SYM_SQU_B, 9);

  // get cvLnx = line lnx normalized ..
  irc = UT3D_cvLn_obj (&cvLnx, Typ_LN, lnx);
  if(irc < 0) return -1;

  // get plane plni from top-of-cone-point and line lnx
  UT3D_pl_pto_vcxn_pt (&plni, &cvLnx.pt0, &cvLnx.vcl, &ptTop);
    // GR_Disp_pln (&plni, 9, 4);

  // get plane plBot for bottom-circle
  //   projects startpoint of contour onto axis
  irc = UT3D_pl_lnz_ptx (&plBot, &rdc, cvAx, &cvCov->pt0);
    // GR_Disp_pln (&plBot, 9, 4);
    // DEB_dump_obj__ (Typ_PLN, &plBot, " plBot");

  // get line lnib = intersection plane plBot X plane plni
  irc = UT3D_ln_int2pl (&lnib, &plBot, &plni);
    // TESTBLOCK
    // { Line ltst; ltst = lnib; UT3D_ln_unlim (&ltst, 3);
    // GR_Disp_ln (&ltst, 10);}
    // DEB_dump_obj__ (Typ_LN, &lnib, " lnib");}
    // END TESTBLOCK

  // create bottom-circle ciB
  // UT3D_ci_ptvcr (&ciB, &cvAx->pt0, &cvAx->vcl, rdc);
  d1 = RAD_360;
  UT3D_ci_3pt_vc_rd_ango (&ciB, &cvCov->pt0, &cvCov->pt0, &plBot.po,
                          &cvAx->vcl, &rdc, &d1);
    // DEB_dump_obj__ (Typ_CI, &ciB, " ciB");
    // GR_Disp_ci (&ciB, 9);

  // get points ptb1 and ptb2 = intersection bottom-circle ciB X lnib
  ipBnr = UT3D_pt_intlnci_lp (ptBa, &lnib, 1, &ciB, 1);
    // printf(" intlnci_lp-ipBnr=%d\n",ipBnr);
  if(ipBnr < 0) goto L_exit;

    // TESTBLOCK
    // if(ipBnr > 0) GR_tDyn_symB__ (&ptBa[0], SYM_CIR_S, ATT_PT_GREEN);
    // if(ipBnr > 0) DEB_dump_obj__(Typ_PT, &ptBa[0], " ptBa[0]");
    // if(ipBnr > 1) GR_tDyn_symB__ (&ptBa[1], SYM_CIR_S, ATT_PT_GREEN);
    // if(ipBnr > 1) DEB_dump_obj__(Typ_PT, &ptBa[1], " ptBa[1]");
    // END TESTBLOCK


  //----------------------------------------------------------------
  // test if intersectionPoints are between angles ang1-ang2 of contourObj

  // get angs,ango = angles of circ from surface-ang1,ang2
  UT2D_2angr_2angr_ci (&angs, &ango, srv->ang1, srv->ang2, (int)srv->dir);


  // test if circle is 360-deg-circ; yes: skip angle-checks
  if(UTP_comp2db(fabs(ango), RAD_360, RAD_1)) goto L_int_L;

  // get angle of intersectionpoint-1
  if(ipBnr > 1) {
    angx = UT3D_angr_ci_p1_pt (&ciB, &ptBa[1]);
      // printf(" angr_ci_p1_pt-1 = %f\n",angx);

    // test if angx is between ang1-ang2; if not: ignore ptBa[1]
    if(UT2D_angr_ck_in_ci (&angx, &angs, &ango, (int)srv->dir)) ipBnr = 1;
  }


  // get angle of intersectionpoint-0
  angx = UT3D_angr_ci_p1_pt (&ciB, &ptBa[0]);
    // printf(" angr_ci_p1_pt-0 = %f\n",angx);

  // test if angx is between ang1-ang2; if not: ignore point ptBa[0]
  if(UT2D_angr_ck_in_ci (&angx, &angs, &ango, (int)srv->dir)) {
    // ptBa[0] is outside surface; ignore.
    if(ipBnr > 1) {
      ptBa[0] = ptBa[1];  // ptBa[1] is valid; keep 1, but overwrite 0 with 1
    } else {
      *nxp = 0;           // 0 and 1 invalid
      goto L_exit;
    }
  }

    // TESTBLOCK
    // printf("--- after-test_v0-v1-ipBnr=%d\n",ipBnr);
    // if(ipBnr > 0) DEB_dump_obj__(Typ_PT, &ptBa[0], " ptBa[0]");
    // if(ipBnr > 1) DEB_dump_obj__(Typ_PT, &ptBa[1], " ptBa[1]");
    // END TESTBLOCK


  //----------------------------------------------------------------
  L_int_L:
  // get point xpTab[0] = intersection line (ptTop,ptBa[0]) X lnx
  UT3D_vc_2pt (&vc1, &ptBa[0], &ptTop);
  irc = UT3D_2par_int2pt2vc (&parLnx, &parCov, 
                             &cvLnx.pt0, &cvLnx.vcl,
                             &ptBa[0], &vc1);
    // printf(" int2pt2vc-irc=%d %f %f\n",irc,parLnx,parCov);
  if(!irc) {
    *nxp = 1;
    UT3D_vc_multvc (&vc1, &cvLnx.vcl, parLnx);
    UT3D_pt_traptvc (&xpTab[0], &cvLnx.pt0, &vc1);
  }


  if(ipBnr > 1) {
    // get point xpTab[1] = intersection line (ptTop,ptBa[1]) X lnx
    UT3D_vc_2pt (&vc1, &ptBa[1], &ptTop);
    irc = UT3D_2par_int2pt2vc (&parLnx, &parCov,
                             &cvLnx.pt0, &cvLnx.vcl,
                             &ptBa[1], &vc1);
      // printf(" int2pt2vc-irc=%d %f %f\n",irc,parLnx,parCov);
    if(!irc) {
      UT3D_vc_multvc (&vc1, &cvLnx.vcl, parLnx);
      UT3D_pt_traptvc (&xpTab[*nxp], &cvLnx.pt0, &vc1);
      *nxp += 1;
    }
  }

    // TESTBLOCK
    // printf("--- after-int2pt2pt_lim-ipBnr=%d nxp=%d\n",ipBnr,*nxp);
    // if(*nxp > 0) GR_tDyn_symB__ (&xpTab[0], SYM_CIR_S, ATT_PT_BLACK);
    // if(*nxp > 0) DEB_dump_obj__(Typ_PT, &xpTab[0], " xpTab[0]");
    // if(*nxp > 1) GR_tDyn_symB__ (&xpTab[1], SYM_CIR_S, ATT_PT_BLACK);
    // if(*nxp > 1) DEB_dump_obj__(Typ_PT, &xpTab[1], " xpTab[1]");
    // END TESTBLOCK


  //----------------------------------------------------------------
  // test if intersectionPoints are inside v0-v1 of contourObj
  // get parameters of intersectionPoints xpTab along contour; compare with v0-v1

  // get parV0 = length from plBot - v0
  parV0 = cvCov->lnl * srv->v0;
  // get parV1 = length from plBot - v1
  parV1 = cvCov->lnl * srv->v1;
    // printf(" parV0=%f parV1=%f\n",parV0,parV1);


  if(*nxp < 2) goto L_ck_vp0;
  // test v0-v1 of xpTab[1]
  // set irc = direction of (ptBa[1]-xpTab[1]) along axis; 0=fwd; 1=bwd
  UT3D_vc_2pt (&vc1, &ptBa[1], &xpTab[1]);
  irc = UT3D_sid_2vc (&vc1, &cvAx->vcl);
  // get signed length
  d1 = UT3D_len_2pt (&xpTab[1], &ptBa[1]) * (double)irc;
    // printf(" v0-v1-xpTab[1]-d1=%f\n",d1);
  // test if parameter d1 is between v0-v1
  if(UTP_db_ck_in2dbTol (d1, parV0, parV1, tolcv)) *nxp = 1;


  L_ck_vp0:
  // test v0-v1 of xpTab[0]
  // set irc = direction of (ptBa[0]-xpTab[0]) along axis; 0=fwd; 1=bwd
  UT3D_vc_2pt (&vc1, &ptBa[0], &xpTab[0]);
  irc = UT3D_sid_2vc (&vc1, &cvAx->vcl);
  // get signed length
  d1 = UT3D_len_2pt (&xpTab[0], &ptBa[0]) * (double)irc;
    // printf(" v0-v1-xpTab[0]-d1=%f\n",d1);
  // test if parameter is between v0-v1
  if(UTP_db_ck_in2dbTol (d1, parV0, parV1, tolcv)) {
    // xpTab[0] is NOT on surf; delete.
    if(*nxp > 1) {
      xpTab[0] = xpTab[1];
      *nxp = 1;
    } else *nxp = 0;
  }


  //----------------------------------------------------------------
  L_exit:

    // printf("--- ex-SRV_pt_int_ln_sCon irc=%d nxp=%d\n",irc,*nxp);
    // if(*nxp > 0) GR_tDyn_symB__ (&xpTab[0], SYM_SQU_B, 9);
    // if(*nxp > 0) DEB_dump_obj__(Typ_PT, &xpTab[0], " xpTab[0]");
    // if(*nxp > 1) GR_tDyn_symB__ (&xpTab[1], SYM_SQU_B, 9);
    // if(*nxp > 1) DEB_dump_obj__(Typ_PT, &xpTab[1], " xpTab[1]");

  return 0;

}


//===========================================================================
  int SRV_pt_int_ln_sCyl (int *nxp, Point *xpTab,
                          Line *lnx, CVLn3 *cvAx, CVLn3 *cvCov, SurRev *srv) {
//===========================================================================
// SRV_pt_int_ln_sCyl                   intersect line lnx X cylinderSurface
// Input:
//   cvAx    axis
//   cvCov   contourline
// Output:
//   irc     0 = normal, 0 or 1 or 2 intersectionpoints
//           1 = line touches cylinder
//
//  cylinder; the startpoint of cvCov has ang=0 rad;
//
//  - get 2D-circle for cylinder and 2D-intersection-line 
//  - get 2D-intersection-points (parameters along line)
//  - transfer points back to 3D
//
// see ../../doc/geom/SRV_pt_int_ln_sCyl.png

// ../gr/tess_su.c:4020

  int      irc, ipa[2];
  double   tolcv, tolpt, rdc, para[2];
  Circ     ciBot;
  Plane    plBot;
  Refsys   rsBot;
  Line2    ln2Bot;
  Circ2    ci2Bot;
  Point2   pt2a[2];
  Point    ptx2D;


  // printf("SRV_pt_int_ln_sCyl --------------\n");
  // DEB_dump_obj__ (Typ_CVLN3, cvAx, " cvAx");
  // DEB_dump_obj__ (Typ_CVLN3, cvCov, " cvCov");
  // DEB_dump_obj__ (Typ_SURRV, srv, " srv:");


  tolcv = UT_TOL_cv;
  tolpt = UT_TOL_pt;


  // get plane for bottom-circle; z-axis = cvAx; x-axis goes tru cvCov->pt0
  irc = UT3D_pl_lnz_ptx (&plBot, &rdc, cvAx, &cvCov->pt0);

  // transfer to 2D; see INF_COORD_CONV__
  // get refsys / backplane
  UT3D_rsys_pl (&rsBot, &plBot);
    // DEB_dump_obj__ (Typ_Refsys, &rsBot, " rsBot");

  // get bottom-circle ciBot
  UT3D_ci_ptrd2vc2angr (&ciBot, &plBot.po, rdc, &plBot.vz, &plBot.vx,
                        srv->ang1, srv->ang2);
    // GR_Disp_ci (&ciBot, 9);

  // bottom-circle ciBot 3D->2D
  UT2D_ci_tra_ci3_rsys (&ci2Bot, &ciBot, &rsBot);
    // GR_Disp_ci2 (&ci2Bot, 9);

  // line 3D->2D
  UT2D_ln_tra_ln3_rsys (&ln2Bot, lnx, &rsBot);
    // GR_tDyn_ln2_2pt (&ln2Bot.p1, &ln2Bot.p2, 9);

  // intersect 2D
  irc = UT2D_2pt_par_int_ln_ci (ipa, pt2a, para, NULL, &ln2Bot, &ci2Bot, &tolcv);
    // printf(" UT2D_2pt_int_ln_ci__-irc=%d\n",irc);
  if(irc < 0) return 0;
    // if(ipa[0] >= 0) GR_Disp_pt2 (&pt2a[0], SYM_SQU_B, 9);
    // if(ipa[1] >= 0) GR_Disp_pt2 (&pt2a[1], SYM_SQU_B, 9);


  *nxp = 0;
  if(ipa[0] >= 0) {
    UT3D_pt_evpar2pt (&xpTab[0], para[0], &lnx->p1, &lnx->p2);
    // test if inside v0-v1 of contourObj
    // transfer point -> 2D with Z-value
    UT3D_pt_tra_pt_rsys (&ptx2D, &xpTab[0], &rsBot);
      // DEB_dump_obj__(Typ_PT, &xpTab[0], " xpTab[0]");
      // DEB_dump_obj__(Typ_PT, &ptx2D, " ptx2D");
    // test if ptx2D.z ist between v0, v1.
    if(!UTP_db_ck_in2dbTol (ptx2D.z, srv->v0, srv->v1, tolpt)) *nxp = 1;
  }
  if(ipa[1] >= 0) {
    UT3D_pt_evpar2pt (&xpTab[1], para[1], &lnx->p1, &lnx->p2);
    // test if inside v0-v1 of contourObj
    // transfer point -> 2D with Z-value
    UT3D_pt_tra_pt_rsys (&ptx2D, &xpTab[1], &rsBot);
      // DEB_dump_obj__(Typ_PT, &xpTab[1], " xpTab[1]");
      // DEB_dump_obj__(Typ_PT, &ptx2D, " ptx2D");
    // test if ptx2D.z ist between v0, v1.
    if(!UTP_db_ck_in2dbTol (ptx2D.z, srv->v0, srv->v1, tolpt)) {
      *nxp += 1;
      if(*nxp == 1) {  // move p1 -> p0
        ipa[0] = ipa[1];
        xpTab[0] = xpTab[1];
      }
    }
  }


  //----------------------------------------------------------------
  L_exit:

    // printf(" ex-SRV_pt_int_ln_sCyl irc=%d nxp=%d\n",irc,*nxp);
    // if(*nxp > 0) GR_tDyn_symB__ (&xpTab[0], SYM_STAR_S, 9);
    // if(*nxp > 0) DEB_dump_obj__(Typ_PT, &xpTab[0], " xpTab[0]");
    // if(*nxp > 1) GR_tDyn_symB__ (&xpTab[1], SYM_STAR_S, 9);
    // if(*nxp > 1) DEB_dump_obj__(Typ_PT, &xpTab[1], " xpTab[1]");

  return irc;


  //----------------------------------------------------------------
  L_err:
  TX_Error("SRV_pt_int_ln_sCyl E%d",irc);
  return -1;

}


//========================================================================
  int SRV_pt_int_ln_srv (int *nxp, Point *xpTab, Line *ln, SurRev *srv) {
//========================================================================
// SRV_pt_int_ln_srv         intersect line X Cyl|Cone
// Output:
//   nxp       nr of intersectionpoints
//   xpTab     intersectionpoints; max. 2
// see INF_FMTB_Surface_RV


  int    irc, oNr, typCen, typCov;
  void   *oCen, *oCov;
  CVLn3  lnCov, lnAx;
  // Plane  plAx;
  // double lLn;
  // Vector vcLn, vcAx, vcCov;
  // Point  ptAx;
  // SurRev


  printf("SRV_pt_int_ln_srv --------\n");
  // DEB_dump_obj__ (Typ_LN, ln, " ln:"); 
  // DEB_dump_obj__ (Typ_SURRV, srv, " srv:");


  // get AxisObj
  typCen = UTO__dbo (&oCen, &oNr, srv->typCen, srv->indCen);
  if(typCen < 0) {irc = 1; goto L_err;}
    // DEB_dump_obj__ (typCen, oCen, " oCen:");

  // get contourObj
  typCov = UTO__dbo (&oCov, &oNr, srv->typCov, srv->indCov);
  if(typCov < 0) {irc = 2; goto L_err;}
    // DEB_dump_obj__ (typCov, oCov, " oCov:");


  //----------------------------------------------------------------
  // get line from AxisObj; 
  irc = UT3D_cvLn_obj (&lnAx, typCen, oCen);
  if(irc < 0) {irc = 3; goto L_err;}


  // test if contour is line
  if(srv->typCov != Typ_LN) goto L_not_conic;


  //----------------------------------------------------------------
  // Cylinder or Cone (contour = Line)

  // get lnCov from contourObj oCov
  irc = UT3D_cvLn_obj (&lnCov, typCov, oCov);  // see UT3D_ptvc_ox
  if(irc < 0) {irc = 4; goto L_err;}

  // test if axis / contourLine parallel
  irc = UT3D_vc_ck_parl2_vc (&lnAx.vcl, &lnCov.vcl, RAD_1); // 0=yes

  if(irc)  {
    // intersect line X cylinderSurf
    return SRV_pt_int_ln_sCyl (nxp, xpTab, ln, &lnAx, &lnCov, srv);
  } else {
    // intersect line X conicSurf
    return SRV_pt_int_ln_sCon (nxp, xpTab, ln, &lnAx, &lnCov, srv);
  }


  //----------------------------------------------------------------
  L_not_conic:
  // test for torus (circ)
return MSG_ERR__ (ERR_TODO_I, "SRV_pt_int_ln_srv TODO-1");

  return 0;



  //----------------------------------------------------------------
  L_err:
  TX_Error("SRV_pt_int_ln_srv E%d",irc);
  return -1;

}


//================================================================
  int SRV_axis1 (Point *ptSpi, Vector *vcSpi, SurRev *oSrv) {
//================================================================
// SRV_axis1             get starting-Axis-origin & direction
 
  int      typ1, rNr;
  void     *pDat1;

  typ1 = oSrv->typCen;

  if(typ1 == Typ_PLN) {
    // get Plane
    typ1 = UTO__dbo (&pDat1, &rNr, oSrv->typCen, oSrv->indCen);
      // DEB_dump_obj__ (typ1, pDat1, "Spine:\n");
    *ptSpi = ((Plane*)pDat1)->po;
    *vcSpi = ((Plane*)pDat1)->vz;
  }

  
  return 0;

}


//============================================================================
  int SRV_CvIso_parsrv (void *objo, int *oTyp, Memspc *memSeg,
                        ObjGX *srv, double dp, int iDir) {
//============================================================================
// get iso-curve from RevolvedSurface
// Input:
//   srv        RevolvedSurface
//   dp         parameter 0-1
//   iDir       1=along, 2=across
// Output:
//   oTyp       type of outputObj; ed Typ_LN|..
//   objo       dataStruct of outputObj; maxSiz OBJ_SIZ_MAX.
//   RetCod:

// see APT_iso_cktypSRV
// see GR_sSym_srv           draw symbolic display of RevolvedSurface


  int      irc, i1, typ1, typ2, sDir, rNr;
  long     oSiz;
  double   d1, d2, drd;
  SurRev   *oSrv;
  Point    p1, p2;
  // ObjGX    ox1, ox2;
  void     *pDat1, *pDat2;
  Point    ptSpi;
  Vector   vcSpi;
  TraRot   dRot;
  char     oAux[OBJ_SIZ_MAX];


  printf("SRV_CvIso_parsrv %f %d \n",dp,iDir);
  // DEB_dump_ox_0 (srv, "");
  // DEB_dump_obj__ (Typ_SURRV, oSrv, "");


  oSrv = (SurRev*)srv->data;

    printf("    a1=%9.3f a2=%9.3f\n",oSrv->ang1,oSrv->ang2);
    printf("    v0=%9.3f v1=%9.3f\n",oSrv->v0,oSrv->v1);




  // Spine: typCen,indCen  Typ_PLN
    printf(" Spine: typ=%d ind=%ld\n",oSrv->typCen,oSrv->indCen);

  // Coverline: typCov,indCov    Typ_LN
    printf(" Cover: typ=%d ind=%ld\n",oSrv->typCov,oSrv->indCov);
    typ2 = UTO__dbo (&pDat2, &rNr, oSrv->typCov, oSrv->indCov);



  //------ 2 = across --------------------------------------
  if(iDir == 2) {
  // across: create circle
    *oTyp = Typ_CI;
    if(objo == NULL) goto L_exit;

    // direction of rotation ..
    if(oSrv->ang2 > oSrv->ang1)  sDir = 1;
    else                         sDir = -1;

    // den parameter umwandeln: v0 == 0; ,v1 == 1; 
    d1 = ((oSrv->v1 - oSrv->v0) * dp) + oSrv->v0;
      // d1 = 0.;
      // d1 = 1.;
      printf(" d1=%f\n",d1);

    // den Punkt auf der Coverline finden;
    // (Wenn Coverline = Radius: am Spine).
    typ2 = UTO__dbo (&pDat2, &rNr, oSrv->typCov, oSrv->indCov);
    // irc = UTO_pt_par1_obj (&p1, d1, typ2, pDat2);
    irc = UT3D_pt_vc__par_cv (&p1, NULL, typ2, pDat2, 1, d1);
    if(irc < 0) return irc;
      // GR_tDyn_symB__ (&p1, SYM_STAR_S, 2);

    irc = SRV_axis1 (&ptSpi, &vcSpi, oSrv);
    if(irc < 0) return irc;

    // den PunktAufCoverline auf den Spine projizieren.
    UT3D_pt_projptptvc (&p2, &drd, NULL, &p1, &ptSpi, &vcSpi);
      // GR_tDyn_symB__ (&p2, SYM_STAR_S, 2);


    // rotate p1 around p2/vcSpi -> objo->p1
    UT3D_pt_rotptptvcangr (&((Circ*)objo)->p1, &p1, &p2, &vcSpi, oSrv->ang1);

    // einen Kreis um p2 mit Z-Achse vcSpi mit Start/Endwinkeln ang1/ang2
    ((Circ*)objo)->vz = vcSpi;
    ((Circ*)objo)->pc = p2;
    ((Circ*)objo)->rad = sDir * UT3D_len_2pt (&((Circ*)objo)->pc,
                                              &((Circ*)objo)->p1);

    // rotate p1 around p2/vcSpi -> objo->p2
    d1 = oSrv->ang2 - oSrv->ang1;
    if(UTP_comp2db(d1, RAD_360, UT_TOL_Ang1) == 1) {
      ((Circ*)objo)->p2 = ((Circ*)objo)->p1;
      ((Circ*)objo)->ango = d1;
    } else {
      UT3D_pt_rotptptvcangr (&((Circ*)objo)->p2, &p1, &p2, &vcSpi, oSrv->ang2);
      UT3D_ci_setangr ((Circ*)objo);
    }
      // GR_Disp_ac (objo, 9);



  //------ 1 = along --------------------------------------
  // along: copy Coverline (MantelKurve)
  // see also SRU_CvIso_parsru - along
  } else {
    // *oTyp = oSrv->typCov;
    typ2 = UTO__dbo (&pDat2, &rNr, oSrv->typCov, oSrv->indCov);
    *oTyp = typ2;
    if(objo == NULL) goto L_exit;


    // die Mantelkurve trimmen
    irc = UTO_trim_2par1 (oAux, pDat2, typ2, oSrv->v0, oSrv->v1, memSeg);
    if(irc < 0) return irc;


    // den Verdrehwinkel bestimmen
    dRot.angr = oSrv->ang1 + ((oSrv->ang2 - oSrv->ang1) * dp);
      printf("    dRot.angr=%f\n",dRot.angr);


    // die Mantelkurve um Achse ptSpi/dRot.vz drehen
    irc = SRV_axis1 (&ptSpi, &vcSpi, oSrv);
    if(irc < 0) return irc;

    // if(typ1 == Typ_PLN) {
      // ptSpi = ((Plane*)pDat1)->po;
      // dRot.vz = ((Plane*)pDat1)->vz;
      // UT3D_vc_invert (&dRot.vz, &dRot.vz);     // ?
    // } else {
      // TX_Error("SRV_CvIso_parsrv E001");
      // return -1;
    // }
    UT3D_m3_inirot_angr (dRot.ma, &ptSpi, &dRot.vz, dRot.angr);
      // DEB_dump_obj__ (Typ_TraRot, &dRot, "TraRot:\n");
    UTRA_def__ (1, Typ_TraRot, &dRot);
    UTRA_app__ (objo, *oTyp, *oTyp, 1, oAux, memSeg);

      // GR_Disp_ost (objo, *oTyp, memSeg, 9);

  }



  //----------------------------------------------------------------
  L_exit:
    // printf(" ex SRV_CvIso_parsrv %d\n",*oTyp);
  return 0;

}


//================================================================
  int SRV_pt_evparsrv (Point *pto, double parU, double parV) {
//================================================================
// SRV_pt_evparsrv           create 3D-point from U,V-parameters
// Init with TSU_tr_init_().
// parU, parV      range 0-1 !


  Point2   pt20;

  // create a 2D-point from parameters ..
  pt20.x = parU * RAD_360;                 // around axis
  pt20.y = parV;                           // along axis

  // get 3D-pt from 2D-pt
  return TSU_tr_3D_2D_srv (pto, &pt20);

}


//========== EOF ==============
