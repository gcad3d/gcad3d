//          Revolved-Surface-Functions    RF.
/*
 *
 * Copyright (C) 2015 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
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
#include "../gr/ut_UI.h"               // UI_Func... SYM_..



//================================================================
  int SRV_axis1 (Point *ptSpi, Vector *vcSpi, SurRev *oSrv) {
//================================================================
// SRV_axis1             get starting-Axis-origin & direction
 
  int      typ1, rNr;
  void     *pDat1;

  typ1 = oSrv->typCen;

  if(typ1 == Typ_PLN) {
    // get Plane
    typ1 = DB_GetObjDat (&pDat1, &rNr, oSrv->typCen, oSrv->indCen);
      // UT3D_stru_dump (typ1, pDat1, "Spine:\n");
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
// see GL_sSym_srv           draw symbolic display of RevolvedSurface


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
  // UTO_dump__ (srv, "");
  // UT3D_stru_dump (Typ_SURRV, oSrv, "");


  oSrv = (SurRev*)srv->data;

    printf("    a1=%9.3f a2=%9.3f\n",oSrv->ang1,oSrv->ang2);
    printf("    v0=%9.3f v1=%9.3f\n",oSrv->v0,oSrv->v1);




  // Spine: typCen,indCen  Typ_PLN
    printf(" Spine: typ=%d ind=%ld\n",oSrv->typCen,oSrv->indCen);

  // Coverline: typCov,indCov    Typ_LN
    printf(" Cover: typ=%d ind=%ld\n",oSrv->typCov,oSrv->indCov);
    typ2 = DB_GetObjDat (&pDat2, &rNr, oSrv->typCov, oSrv->indCov);



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
    typ2 = DB_GetObjDat (&pDat2, &rNr, oSrv->typCov, oSrv->indCov);
    // irc = UTO_pt_par1_obj (&p1, d1, typ2, pDat2);
    irc = UT3D_ptvc_tng_crv_par (&p1, NULL, typ2, pDat2, 0, d1);
    if(irc < 0) return irc;
      // GR_Disp_pt (&p1, SYM_STAR_S, 2);

    irc = SRV_axis1 (&ptSpi, &vcSpi, oSrv);
    if(irc < 0) return irc;

    // den PunktAufCoverline auf den Spine projizieren.
    UT3D_pt_projptptvc (&p2, &drd, NULL, &p1, &ptSpi, &vcSpi);
      // GR_Disp_pt (&p2, SYM_STAR_S, 2);


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
    typ2 = DB_GetObjDat (&pDat2, &rNr, oSrv->typCov, oSrv->indCov);
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
      // UT3D_stru_dump (Typ_TraRot, &dRot, "TraRot:\n");
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
