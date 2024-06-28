//**************************************************************************
/*
 *
 * Copyright (C) 2024 Thomas Backmeister           (thb@kilatasok.at)          
 * Copyright (C) 2024 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
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

-----------------------------------------------------
Modifications:
2024-05-13 created THB.

-----------------------------------------------------
*/
#ifdef globTag
void CBSP_tng(){}
#endif
/*!
\file  ../ut/cbsp_tng.c
\brief 3D modify curvature of 2 b-spline-curves
\code
=====================================================
List_functions_start:

CBSP_tng__               modify curvature of 2 b-spline-curves

CBSP_PosParCrv           position on curve <-- curve parameter
CBSP_KnotspanIndex       knotspan index <-- curve parameter
CBSP_vc_TgCrCv           tangent/curvature vector <-- curve parameter
CBSP_BaseDerivCv         values & derivative values <-- basefunctions
CBSP_TgOrCrMoveCtrlPts   move controlpoints <-- tangent OR curvature difference
CBSP_TgAndCrMoveCtrlPts  move controlpoints <-- tangent AND curvature difference
CBSP_bsp_ck_con          connection type <-- bspline curves
CBSP_vc_SmoothTg         common tangent direction <-- bspline curves
CBSP_vc_SmoothCr         assimilated curvature <-- bspline curves
CBSP_vc_NewTg            new common tangent <-- bspline curves
CBSP_vc_NewCr            new curvature <-- bspline curves
CBSP_MoveCtrlPts         move controlpoints <-- bspline curve

List_functions_end
=====================================================
*/


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_tol_const.h"        // Tolerances
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_ox_base.h"          // OGX_SET_OBJ
#include "../xa/xa_msg.h"              // MSG_*
#include "../db/ut_DB.h"               // DB_GetSur



//================================================================
  int CBSP_tng__ (CurvBSpl *cv1, CurvBSpl *cv2,
                  int opt, int c1se, double pc, Memspc *memSeg) {
//================================================================
// Input:
//   cv1         curve-1 to modify
//   cv1         curve-2 to modify
//   opt         1=G1; 2=G2; 3=G2-keep tangent of curve1
//   c1se        0=modify at START of curve 1
//               1=modify at END of curve 1
//   pc          deviation tangent degree; -90 to +90; 0 = no deviation;
// Output:
//   cv1         modified curve-1
//   cv1         modified curve-2
//
// ****************************************************************
// It is assumed that each curve's start and end parameter (v0, v1)
// is IDENTICAL to the smallest or highest value respectively of
// its knotvector !
//   NECESSARY for BSpline-Curves: v0 = START-Knot, v1 = END-Knot !
// ****************************************************************




  int        irc, i1; //, typ1, typ2, dbi1, dbi2, opt;
  int        con, ind1, ind2, pos1, pos2;
  void       *memStart;
  double     uPar1, uPar2, len;
  double     *c1D0, *c1D1, *c1D2, *c2D0, *c2D1, *c2D2;
  Vector     tg1, tg2, dTg1, dTg2;
  Vector     cr1, cr2, dCr1, dCr2;


  // TESTBLOCK
  printf("\nCBSP_tng__ opt=%d c1se=%d pc=%f\n",opt,c1se,pc);
  DEB_dump_obj__ (Typ_CVBSP, cv1, "CBSP_tng__-in-cv1");
  DEB_dump_obj__ (Typ_CVBSP, cv2, "CBSP_tng__-in-cv2");
  // printf("\nCurve 1: numCp= %d  degree= %d\n", cv1->ptNr, cv1->deg);
  // for(i1=0; i1<cv1->ptNr; ++i1) {
    // printf("CPt %d: %f %f %f\n", i1,
    // cv1->cpTab[i1].x, cv1->cpTab[i1].y, cv1->cpTab[i1].z);
  // }
  // for(i1=0; i1<=cv1->ptNr+cv1->deg; ++i1) {
    // printf("Knot %d: %f\n", i1, cv1->kvTab[i1]);
  // }
  // printf("\nCurve 2: numCp= %d  degree= %d\n", cv2->ptNr, cv2->deg);
  // for(i1=0; i1<cv2->ptNr; ++i1) {
    // printf("CPt %d: %f %f %f\n", i1,
    // cv2->cpTab[i1].x, cv2->cpTab[i1].y, cv2->cpTab[i1].z);
  // }
  // for(i1=0; i1<=cv2->ptNr+cv2->deg; ++i1) {
    // printf("Knot %d: %f\n", i1, cv2->kvTab[i1]);
  // }
  // END TESTBLOCK


  // degree -> percent
  pc = pc * 0.55555555 + 50.;     printf(" pc(deg) = %f\n",pc);


  //================================================================
  // check how the two curves are connected at chosen curve end
  // con = 0 ... start-start connection
  // con = 1 ... start-end connection
  // con = 2 ... end-start connection
  // con = 3 ... end-end connection
  irc = CBSP_bsp_ck_con (&con, &uPar1, &uPar2, cv1, cv2, c1se);
  if (irc < 0) goto L_Err1;

    // TESTBLOCK
    // printf("\n");
    // if (con == 0) printf("START-START connection\n");
    // if (con == 1) printf("START-END connection\n");
    // if (con == 2) printf("END-START connection\n");
    // if (con == 3) printf("END-END connection\n");
    // printf("uPar1= %f, uPar2= %f\n", uPar1, uPar2);
    // END TESTBLOCK


  //================================================================
  // modification position for both curves
  // (0/1 ... start/end)
  if (con == 0) {
    pos1 = 0;
    pos2 = 0;
  }
  else if (con == 1) {
    pos1 = 0;
    pos2 = 1;
  }
  else if (con == 2) {
    pos1 = 1;
    pos2 = 0;
  }
  else if (con == 3) {
    pos1 = 1;
    pos2 = 1;
  }

    // TESTBLOCK
    // printf("\n");
    // if (pos1 == 0) printf("modification at START of curve 1\n");
    // if (pos1 == 1) printf("modification at END of curve 1\n");
    // if (pos2 == 0) printf("modification at START of curve 2\n");
    // if (pos2 == 1) printf("modification at END of curve 2\n");
    // END TESTBLOCK


  //================================================================
  // knotspan indices of uPar1 and uPar2
  ind1 = CBSP_KnotspanIndex (cv1, uPar1);
  if (ind1 < 0) goto L_Err1;
  ind2 = CBSP_KnotspanIndex (cv2, uPar2);
  if (ind2 < 0) goto L_Err1;

    // printf("ind1= %d\n", ind1);
    // printf("ind2= %d\n", ind2);


  //================================================================
  // values and derivatives of the relevant basefunctions
  // of both curves
  memStart = memSeg->next;
  c1D0 = NULL;
  c1D1 = NULL;
  c1D2 = NULL;
  irc = CBSP_BaseDerivCv (&c1D0, &c1D1, &c1D2, memSeg, cv1, uPar1, ind1);
  if (irc < 0) goto L_Err1;

  c2D0 = NULL;
  c2D1 = NULL;
  c2D2 = NULL;
  irc = CBSP_BaseDerivCv (&c2D0, &c2D1, &c2D2, memSeg, cv2, uPar2, ind2);
  if (irc < 0) goto L_Err1;


  //================================================================
  // OPTIONS:
  L_OPT:
  // 1 ... create tangent continuity
  // 2 ... create tangent & curvature continuity
  // 3 ... create curvature continuity with unchanged tangent
    // printf("\n");
    // if (opt == 1) printf("create TANGENT continuity\n");
    // if (opt == 2) printf("create TANGENT & CURVATURE continuity\n");
    // if (opt == 3) printf("create CURVATURE continuity with unchanged tangent\n");



  //================================================================
  if (opt == 1) {
    // 1 ... create TANGENT continuity

    // new common tangent
    // ------------------
    irc = CBSP_vc_NewTg (&dTg1, &dTg2, cv1, ind1, c1D1, cv2, ind2, c2D1, con, pc);
    if (irc < 0) goto L_Err1;

      //----------------------------------------------------------------
      // only for TEST:
      // TESTBLOCK
      // // Curve 1: curvature vector at uPar1
      // printf("\nOnly TEST:\n");
      // irc = CBSP_vc_TgCrCv (&cr1, cv1, ind1, c1D2);
      // if (irc < 0) goto L_Err1;
      // len = UT3D_len_vc (&cr1);
      // printf("cr1= %f, %f, %f\n", cr1.dx, cr1.dy, cr1.dz);
      // printf("length of cr1= %f\n", len);
      // // Curve 2: curvature vector at uPar2
      // irc = CBSP_vc_TgCrCv (&cr2, cv2, ind2, c2D2);
      // if (irc < 0) goto L_Err1;
      // len = UT3D_len_vc (&cr2);
      // printf("cr2= %f, %f, %f\n", cr2.dx, cr2.dy, cr2.dz);
      // printf("length of cr2= %f\n", len);
      // printf("END of TEST\n");
      // END TESTBLOCK
  }


  //================================================================
  else if (opt == 2) {
    // 2 ... create TANGENT & CURVATURE continuity

    // new common tangent
    // ------------------
    irc = CBSP_vc_NewTg (&dTg1, &dTg2, cv1, ind1, c1D1, cv2, ind2, c2D1, con, pc);
    if (irc < 0) goto L_Err1;
  }


  //================================================================
  else if (opt == 3) {
    // 3 ... create CURVATURE continuity with unchanged tangent

      //----------------------------------------------------------------
      // only for TEST:
      // TESTBLOCK
      // // Curve 1: tangent vector at uPar1
      // printf("\nOnly TEST:\n");
      // irc = CBSP_vc_TgCrCv (&tg1, cv1, ind1, c1D1);
      // if (irc < 0) goto L_Err1;
      // printf("tg1= %f, %f, %f\n", tg1.dx, tg1.dy, tg1.dz);
      // len = UT3D_len_vc (&tg1);
      // printf("length of tg1= %f\n", len);
      // // Curve 2: curvature vector at uPar2
      // irc = CBSP_vc_TgCrCv (&tg2, cv2, ind2, c2D1);
      // if (irc < 0) goto L_Err1;
      // printf("tg2= %f, %f, %f\n", tg2.dx, tg2.dy, tg2.dz);
      // len = UT3D_len_vc (&tg2);
      // printf("length of tg2= %f\n", len);
      // END TESTBLOCK

    // new curvature
    // -------------
    irc = CBSP_vc_NewCr (&dCr1, &dCr2, pc, cv1, ind1, c1D2, cv2, ind2, c2D2);
    if (irc < 0) goto L_Err1;

    // unchanged tangents
    // ------------------
    dTg1.dx = 0.0;
    dTg1.dy = 0.0;
    dTg1.dz = 0.0;

    dTg2.dx = 0.0;
    dTg2.dy = 0.0;
    dTg2.dz = 0.0;
  }


  //================================================================
  // move relevant controlpoints of curve 1
    // printf("\nModify CURVE 1:\n");
    // printf("===============\n");
  irc = CBSP_MoveCtrlPts (cv1, pos1, ind1, c1D1, c1D2, &dTg1, &dCr1, opt);
  if (irc < 0) goto L_Err1;

  // move relevant controlpoints of curve 2
    // printf("\nModify CURVE 2:\n");
    // printf("===============\n");
  irc = CBSP_MoveCtrlPts (cv2, pos2, ind2, c2D1, c2D2, &dTg2, &dCr2, opt);
  if (irc < 0) goto L_Err1;


  //================================================================
  if (opt == 2) {
    opt = 3;
    // go back and create also curvature continuity (with unchanged tangents)
    goto L_OPT;
  }


  //================================================================
  irc = 0;

    // TESTBLOCK
    DEB_dump_obj__ (Typ_CVBSP, cv1, "ex-CBSP_tng__-cv1");
    DEB_dump_obj__ (Typ_CVBSP, cv2, "ex-CBSP_tng__-cv2");
    // printf("\nCurve 1:\n");
    // for(i1=0; i1<cv1->ptNr; ++i1) {
      // printf("CPt %d: %f %f %f\n", i1,
      // cv1->cpTab[i1].x, cv1->cpTab[i1].y, cv1->cpTab[i1].z);
      // //store_and_disp_point (&(cv1->cpTab[i1]), -2L, ATT_PT_BLACK);
    // }
    // printf("\nCurve 2:\n");
    // for(i1=0; i1<cv2->ptNr; ++i1) {
      // printf("CPt %d: %f %f %f\n", i1,
      // cv2->cpTab[i1].x, cv2->cpTab[i1].y, cv2->cpTab[i1].z);
      // //store_and_disp_point (&(cv2->cpTab[i1]), -2L, ATT_PT_BLACK);
    // }
    // END TESTBLOCK


  //================================================================
  L_exit:
      printf("exit-CBSP_tng__\n\n");

    // Release memspace
    memSeg->next = memStart;

    return 0;

  //----------------------------------------------------------------
  L_Err1:
    TX_Error("ERR1 test_cbsp_tng");
    goto L_exit;
}


//=============================================================
  int CBSP_PosParCrv (CurvBSpl *cv, double uPar) {
//=============================================================
// CBSP_PosParCrv     position on curve <-- curve parameter
// 
// CBSP_PosParCrv     Author: Thomas Backmeister   27.4.2024 
// 
// Computation of the position on a bspline curve to a given
// parameter. 
//
// IN:
//   CurvBSpl *cv      b-spline curve
//   double uPar       parameter value
// OUT:
//
// Return:
//   position: 0 / 1 / 2 ... start / end / middle 

  int pos;

  // printf("\nCBSP_PosParCrv - start\n");

  if (UTP_comp2db (uPar, cv->kvTab[cv->deg], UT_TOL_min1)) pos = 0;
  else if (UTP_comp2db (uPar, cv->kvTab[cv->ptNr], UT_TOL_min1)) pos = 1;
  else pos = 2;

  return pos;
}



//=============================================================
  int CBSP_KnotspanIndex (CurvBSpl *cv, double uPar) {
//=============================================================
// CBSP_KnotspanIndex     knotspan index <-- curve parameter
// 
// CBSP_KnotspanIndex     Author: Thomas Backmeister   10.4.2024 
// 
// Computation of the knotspan index of a bspline curve parameter.
// (NURBS-book p. 68)
//
// IN:
//   CurvBSpl *cv      b-spline curve
//   double uPar       parameter value
// OUT:
//
// Return:
//   knotspan index
//   -1 ... input error

  int low, high, mid;

  // printf("\nCBSP_KnotspanIndex - start\n");

  if (uPar < cv->kvTab[cv->deg] || uPar > cv->kvTab[cv->ptNr]) goto L_InErr;

  if (UTP_comp2db (uPar, cv->kvTab[cv->ptNr], UT_TOL_min1))
    return cv->ptNr - 1;
  low = cv->deg;
  high = cv->ptNr;
  mid = (low + high) / 2; 
  while (uPar < cv->kvTab[mid] || uPar >= cv->kvTab[mid+1]) {
    if (uPar < cv->kvTab[mid]) high = mid;
    else low = mid;
    mid = (low + high) / 2;
  }
  return mid;  

L_InErr:
  TX_Error("CBSP_KnotspanIndex: input error");
  return -1;
}



//=============================================================
  int CBSP_vc_TgCrCv (Vector *tgcr, CurvBSpl *cv, int ind,
                      double *D12) {
//=============================================================
// CBSP_vc_TgCrCv     tangent/curvature vector <-- curve parameter
// 
// CBSP_vc_TgCrCv     Author: Thomas Backmeister    28.4.2024 
// 
// Computation of the tangent or curvature vector at a given
// parameter of a B-Spline curve.
// The curve must be at least of degree 2 !
// (NURBS-book p. 91)
//
// IN:
//   CurvBSpl *cv      b-spline curve
//   int ind           knotspan index of parameter
//   double *D12       first or second derivatives of relevant
//                     base functions at parameter
// OUT:
//   Vector *tgcr      tangent or curvature vector
//
// Returncode:
//   0 = OK
//  -1 = input error

  int deg, k1, l1;
  Point PP, P1;

  // printf("\nCBSP_vc_TgCrCv - start\n");

  deg = cv->deg;
  if (deg < 2) goto L_InErr;

  UT3D_pt_3db (&PP, 0.0, 0.0, 0.0);
  k1 = 0;
  for (l1=ind-deg; l1<=ind; ++l1) {
    UT3D_pt_3db (&P1, 0.0, 0.0, 0.0);
    UT3D_pt_multpt (&P1, &(cv->cpTab[l1]), D12[k1]);
    UT3D_pt_addpt (&PP, &P1);
    ++k1;
  }
  UT3D_vc_pt (tgcr, &PP);

  return 0;

L_InErr:
  TX_Error("CBSP_vc_TgCrCv: input error");
  return -1;
}



//=============================================================
  int CBSP_BaseDerivCv (double **D0, double **D1, double **D2,
                        Memspc *memSeg, CurvBSpl *cv,
                        double uPar, int ind) {
//=============================================================
// CBSP_BaseDerivCv     values & derivative values <-- basefunctions
// 
// CBSP_BaseDerivCv     Author: Thomas Backmeister    9.3.2024 
// 
// Compute the values and the first and second derivative values
// of the deg+1 relevant basefunctions at the desired parameter value.
// The curve degree must be at least 2!
//
// IN:
//   CurvBSpl *cv      b-spline curve
//   double uPar       curve parameter
//   int ind           knot span index of uPar
//
// OUT:
//   double **D0       basefunction values
//   double **D1       first derivatives
//   double **D2       second derivatives 
//   Memspc *memSeg    space for D0, D1, D2
//
// Returncode:
//   0 = OK
//  -1 = input error
//  -2 = out of space

  int deg, fl, p1, l1;
  double *Ra, *Ro, *Roo, f1, f2;

  // printf("\nCBSP_BaseDerivCv - start\n");

  deg = cv->deg;
  if (deg < 2) goto L_InErr;

  // Reserve memspace for D0, D1, D2
  *D0 = (double*) UME_reserve (memSeg, (deg+2)*sizeof(double));
  if (*D0 == NULL) goto L_outOfSpace;
  *D1 = (double*) UME_reserve (memSeg, (deg+2)*sizeof(double));
  if (*D1 == NULL) goto L_outOfSpace;
  *D2 = (double*) UME_reserve (memSeg, (deg+2)*sizeof(double));
  if (*D2 == NULL) goto L_outOfSpace;

  // Init D0, D1, D2
  for (l1=0; l1<=deg+1; ++l1) {
		(*D0)[l1] = 0.0;
		(*D1)[l1] = 0.0;
		(*D2)[l1] = 0.0;
  }
  (*D0)[deg] = 1.0;

  // Compute basefunction values for deg, deg-1, deg-2
  // (script 7.3.24)
  for (p1=1; p1<=deg; ++p1) {
    if ((p1 % 3) == 0) {
      Ro = *D2;
      Ra = *D0;
    }
    else if ((p1 % 3) == 1) {
      Ro = *D0;
      Ra = *D1;
    }
    else if ((p1 % 3) == 2) {
      Ro = *D1;
      Ra = *D2;
    }
		// Compute Ra from Ro
    for (l1=deg-p1; l1<=deg; ++l1) {
      fl = l1 + ind - deg;
      f1 = cv->kvTab[fl+p1] - cv->kvTab[fl];
      if (UTP_comp_0(f1)) f1 = 0.0;
      else f1 = (uPar - cv->kvTab[fl]) / f1;
      f2 = cv->kvTab[fl+p1+1] - cv->kvTab[fl+1];
      if (UTP_comp_0(f2)) f2 = 0.0;
      else f2 = (cv->kvTab[fl+p1+1] - uPar) / f2;
      Ra[l1] = f1 * Ro[l1] + f2 * Ro[l1+1];
    }
  }
  if ((deg % 3) == 0) Roo = *D1;
  else if ((deg % 3) == 1) Roo = *D2;
  else if ((deg % 3) == 2) Roo = *D0;

/*
  printf("\n(deg-2)-basevalues: ");
  for (l1=0; l1<=deg+1; ++l1) printf("%f  ", Roo[l1]);
  printf("\n");
  printf("(deg-1)-basevalues: ");
  for (l1=0; l1<=deg+1; ++l1) printf("%f  ", Ro[l1]);
  printf("\n");
  printf("(deg)-basevalues: ");
  for (l1=0; l1<=deg+1; ++l1) printf("%f  ", Ra[l1]);
  printf("\n");
*/

  // Compute first derivatives for deg and deg-1
  // (script 8.3.24)
  // (deg-1)-basevalues => (deg)-first derivatives 
  for (l1=0; l1<=deg; ++l1) {
    fl = l1 + ind - deg;
    f1 = cv->kvTab[fl+deg] - cv->kvTab[fl];
    if (UTP_comp_0(f1)) f1 = 0.0;
    else f1 = deg / f1;
    f2 = cv->kvTab[fl+deg+1] - cv->kvTab[fl+1];
    if (UTP_comp_0(f2)) f2 = 0.0;
    else f2 = deg / f2;
    Ro[l1] = f1 * Ro[l1] - f2 * Ro[l1+1];
  }
  // (deg-2)-basevalues => (deg-1)-first derivatives 
  for (l1=0; l1<=deg; ++l1) {
    fl = l1 + ind - deg;
    f1 = cv->kvTab[fl+deg-1] - cv->kvTab[fl];
    if (UTP_comp_0(f1)) f1 = 0.0;
    else f1 = (deg-1) / f1;
    f2 = cv->kvTab[fl+deg] - cv->kvTab[fl+1];
    if (UTP_comp_0(f2)) f2 = 0.0;
    else f2 = (deg-1) / f2;
    Roo[l1] = f1 * Roo[l1] - f2 * Roo[l1+1];
  }

/*    
  printf("\n(deg-1)-first derivatives: ");
  for (l1=0; l1<=deg+1; ++l1) printf("%f  ", Roo[l1]);
  printf("\n");
  printf("(deg)-first derivatives: ");
  for (l1=0; l1<=deg+1; ++l1) printf("%f  ", Ro[l1]);
  printf("\n");
*/

  // Compute second derivatives for deg
  // (script 8.3.24)
  // (deg-1)-first derivatives => (deg)-second derivatives 
  for (l1=0; l1<=deg; ++l1) {
    fl = l1 + ind - deg;
    f1 = cv->kvTab[fl+deg] - cv->kvTab[fl];
    if (UTP_comp_0(f1)) f1 = 0.0;
    else f1 = deg / f1;
    f2 = cv->kvTab[fl+deg+1] - cv->kvTab[fl+1];
    if (UTP_comp_0(f2)) f2 = 0.0;
    else f2 = deg / f2;
    Roo[l1] = f1 * Roo[l1] - f2 * Roo[l1+1];
  }

/*
  printf("\n(deg)-second derivatives: ");
  for (l1=0; l1<=deg+1; ++l1) printf("%f  ", Roo[l1]);
  printf("\n");
*/

  // Output
  *D0 = Ra;
  *D1 = Ro;
  *D2 = Roo;

    // TESTBLOCK
    // Output-Check
    // printf("\n(deg)-basevalues: ");
    // for (l1=0; l1<=deg+1; ++l1) printf("%f  ", (*D0)[l1]);
    // printf("\n");
    // printf("(deg)-first derivatives: ");
    // for (l1=0; l1<=deg+1; ++l1) printf("%f  ", (*D1)[l1]);
    // printf("\n");
    // printf("(deg)-second derivatives: ");
    // for (l1=0; l1<=deg+1; ++l1) printf("%f  ", (*D2)[l1]);
    // printf("\n");
    // END TESTBLOCK

  return 0;


L_InErr:
  TX_Error("CBSP_BaseDerivCv: input error");
  return -1;

L_outOfSpace:
  TX_Error("CBSP_BaseDerivCv: out of space");
  return -2;
}



//=============================================================
  int CBSP_TgOrCrMoveCtrlPts (CurvBSpl *cv, int pos, int ind,
                              double *D12, Vector *dTgCr) {
//=============================================================
// CBSP_TgOrCrMoveCtrlPts     move controlpoints <-- tangent OR
//                                                   curvature 
//                                                   difference
// 
// CBSP_TgOrCrMoveCtrlPts     Author: Thomas Backmeister    26.4.2024 
// 
// Move the relevant control points P(ind-deg),...,P(ind) to obtain
// the desired tangent OR (exclusive OR) curvature vector.
// (script 3.3.24 and 19.3.24)
// If P(ind-deg) is the FIRST of all control points, it will not be moved.
// If P(ind) is the LAST of all control points, it will not be moved.
// D12 = FIRST derivatives must correspond to TANGENT vector difference.
// D12 = SECOND derivatives must correspond to CURVATURE vector difference.
// The b-spline curve degree must be at least 2!
//
// IN:
//   CurvBSpl *cv      b-spline curve
//   int pos           0 / 1 / 2 ... curve's start / end / middle
//   int ind           relevant knot span index
//   double *D12       first OR second derivatives of relevant
//                     base functions
//   Vector *dTgCr     tangent OR curvature vector difference
// OUT:
//   CurvBSpl *cv      b-spline curve
//
// Returncode:
//   0 = OK
//  -1 = input error
//  -2 = internal error

  int deg, l1, k1, ls, ll;
  double sn;
  Vector DD;

  // printf("\nCBSP_TgOrCrMoveCtrlPts - start\n");

  deg = cv->deg;
  if (deg < 2) goto L_InErr;

  if (pos == 0) {  // FIRST control point may not be moved
    sn = D12[1] * D12[1];
    for (l1=2; l1<=deg; ++l1) sn += D12[l1] * D12[l1];
    if (UTP_comp_0(sn)) goto L_IntErr;
    sn = 1.0 / sn;
		k1 = 1;
    ls = ind - deg + 1;
    ll = ind;
  }
  else if (pos == 1) {  // LAST control point may not be moved 
    sn = D12[0] * D12[0];
    for (l1=1; l1<=deg-1; ++l1) sn += D12[l1] * D12[l1];
    if (UTP_comp_0(sn)) goto L_IntErr;
    sn = 1.0 / sn;
    k1 = 0;
    ls = ind - deg;
    ll = ind - 1;
  }
  else {  // ALL relevant control points can be moved
    sn = D12[0] * D12[0];
    for (l1=1; l1<=deg; ++l1) sn += D12[l1] * D12[l1];
    if (UTP_comp_0(sn)) goto L_IntErr;
    sn = 1.0 / sn;
    k1 = 0;
    ls = ind - deg;
    ll = ind;
  }

  // Move controlpoints
  for (l1=ls; l1<=ll; ++l1) {
    DD = *dTgCr;
    UT3D_vc_mult_d (&DD, &DD, D12[k1]*sn);
    UT3D_pt_traptvc (&(cv->cpTab[l1]), &(cv->cpTab[l1]), &DD);
    ++k1;
  }

  return 0;

L_InErr:
  TX_Error("CBSP_TgOrCrMoveCtrlPts: input error");
  return -1;

L_IntErr:
  TX_Error("CBSP_TgOrCrMoveCtrlPts: internal error");
  return -2;
}



//=============================================================
  int CBSP_TgAndCrMoveCtrlPts (CurvBSpl *cv, int pos, int ind,
                               double *D1, double *D2, Vector *dTg,
                               Vector *dCr) {
//=============================================================
// CBSP_TgAndCrMoveCtrlPts     move controlpoints <-- tangent AND
//                                                    curvature
//                                                    difference
// 
// CBSP_TgAndCrMoveCtrlPts     Author: Thomas Backmeister    19.3.2024 
// 
// Move the relevant control points P(ind-deg),...,P(ind) to obtain
// the desired tangent vector AND curvature vector.
// If P(ind-deg) is the FIRST of all control points, it will not be moved.
// If P(ind) is the LAST of all control points, it will not be moved.
// (script 19.3.24)
// The b-spline curve degree must be at least 2!
//
// IN:
//   CurvBSpl *cv      b-spline curve
//   int pos           0 / 1 / 2 ... curve's start / end / middle
//   int ind           relevant knot span index
//   double *D1        first derivatives of relevant base functions
//   double *D2        second derivatives of relevant base functions
//   Vector *dTg       tangent vector difference
//   Vector *dCr       curvature vector difference
// OUT:
//   CurvBSpl *cv      b-spline curve
//
// Returncode:
//   0 = OK
//  -1 = input error
//  -2 = internal error

  int deg, i1, il, l1, ls, ll, k1, rc;
  double a0, a1;
  Mat_2x2 BBt, BBtInv;
  Vector DD1, DD2, DD;


  // printf("\nCBSP_TgAndCrMoveCtrlPts - start\n");

  deg = cv->deg;
  if (deg < 2) goto L_InErr;

  // Compute BBt (= B * B-transpose)
  il = deg;
  if (pos == 0) {  // FIRST control point may not be moved
    BBt[0][0] = 0.0;
    BBt[0][1] = 0.0;
    BBt[1][1] = 0.0;
    k1 = 1;
    ls = ind - deg + 1;
    ll = ind;
  }
  else if (pos == 1) {  // LAST control point may not be moved
    il = deg - 1;
    BBt[0][0] = D1[0] * D1[0];
    BBt[0][1] = D1[0] * D2[0];
    BBt[1][1] = D2[0] * D2[0];
    k1 = 0;
    ls = ind - deg;
    ll = ind - 1;
  }
  else {  // ALL relevant control points can be moved
    BBt[0][0] = D1[0] * D1[0];
    BBt[0][1] = D1[0] * D2[0];
    BBt[1][1] = D2[0] * D2[0];
    k1 = 0;
    ls = ind - deg;
    ll = ind;
  }
  for (i1=1; i1<=il; ++i1) {
    BBt[0][0] += D1[i1] * D1[i1];
    BBt[0][1] += D1[i1] * D2[i1];
    BBt[1][1] += D2[i1] * D2[i1];
  }
  BBt[1][0] = BBt[0][1];

  // Invert BBt
  rc = UT3D_m2_inv__ (BBtInv, BBt);
  if (rc < 0) goto L_IntErr;

  // Move controlpoints
  for (l1=ls; l1<=ll; ++l1) {
    a0 = D1[k1] * BBtInv[0][0] + D2[k1] * BBtInv[1][0];
    a1 = D1[k1] * BBtInv[0][1] + D2[k1] * BBtInv[1][1];
    DD1 = *dTg;
    DD2 = *dCr;
    UT3D_vc_mult_d (&DD1, &DD1, a0);
    UT3D_vc_mult_d (&DD2, &DD2, a1);
    UT3D_vc_add2vc (&DD, &DD1, &DD2);
    UT3D_pt_traptvc (&(cv->cpTab[l1]), &(cv->cpTab[l1]), &DD);
    ++k1;
  }

  return 0;

L_InErr:
  TX_Error("CBSP_TgAndCrMoveCtrlPts: input error");
  return -1;

L_IntErr:
  TX_Error("CBSP_TgAndCrMoveCtrlPts: matrix inversion not possible!");
  return -2;
}



//=============================================================
  int CBSP_bsp_ck_con (int *con, double *uPar1, double *uPar2,
                       CurvBSpl *cv1, CurvBSpl *cv2, int c1se) {
//=============================================================
// CBSP_bsp_ck_con     connection type <-- bspline curves
// 
// CBSP_bsp_ck_con     Author: Thomas Backmeister    2.5.2024 
// 
// Check how two bspline curves are connected at desired end 
// of modification.
//
// It is assumed that each curve has its endings at its first
// respectively last control point.
// This means that parameters v0 and v1 of the curves are
// NOT considered.
//
// IN:
//   CurvBSpl *cv1     bspline curve 1
//   CurvBSpl *cv2     bspline curve 2
//   int c1se          0/1 ... start/end of curve 1 to be modified
//                             (in case of double connection)
// OUT:
//   int *con          0 ... start-start connection (cv1-cv2)
//                     1 ... start-end connection (cv1-cv2)
//                     2 ... end-start connection (cv1-cv2)
//                     3 ... end-end connection (cv1-cv2)
//                    -1 ... not connected
//   double *uPar1     parameter of curve 1 (where modification occurs)
//   double *uPar2     parameter of curve 2 (where modification occurs)              
//
// Returncode:
//   0 = OK

  Point ps1, pe1, ps2, pe2;


  // printf("\nCBSP_bsp_ck_con - start\n");

  // first and last control point of cv1 and cv2
  ps1 = cv1->cpTab[0];
  pe1 = cv1->cpTab[cv1->ptNr-1];
  ps2 = cv2->cpTab[0];
  pe2 = cv2->cpTab[cv2->ptNr-1];

  // type of connection
  if (UT3D_comp2pt (&ps1, &ps2, UT_TOL_pt)) {
    *con = 0;
    *uPar1 = cv1->kvTab[cv1->deg];
    *uPar2 = cv2->kvTab[cv2->deg];
    if (UT3D_comp2pt (&pe1, &pe2, UT_TOL_pt)) {
      printf("double connection\n");
      if (c1se == 1) {
        *con = 3;
        *uPar1 = cv1->kvTab[cv1->ptNr];
        *uPar2 = cv2->kvTab[cv2->ptNr];
      }
    }
  }
  else if (UT3D_comp2pt (&ps1, &pe2, UT_TOL_pt)) {
    *con = 1;
    *uPar1 = cv1->kvTab[cv1->deg];
    *uPar2 = cv2->kvTab[cv2->ptNr];
    if (UT3D_comp2pt (&pe1, &ps2, UT_TOL_pt)) {
      printf("double connection\n");
      if (c1se == 1) {
        *con = 2;
        *uPar1 = cv1->kvTab[cv1->ptNr];
        *uPar2 = cv2->kvTab[cv2->deg];
      }
    }
  }
  else if (UT3D_comp2pt (&pe1, &ps2, UT_TOL_pt)) {
    *con = 2;
    *uPar1 = cv1->kvTab[cv1->ptNr];
    *uPar2 = cv2->kvTab[cv2->deg];
  }
  else if (UT3D_comp2pt (&pe1, &pe2, UT_TOL_pt)) {
    *con = 3;
    *uPar1 = cv1->kvTab[cv1->ptNr];
    *uPar2 = cv2->kvTab[cv2->ptNr];
  }
  else *con = -1;

  return 0;
}



//=============================================================
  int CBSP_vc_SmoothTg (Vector *tg1D, Vector *tg2D, int meth,
                        int con, Vector *tg1, Vector *tg2,
                        double pc) {
//=============================================================
// CBSP_vc_SmoothTg     common tangent direction <-- bspline curves
// 
// CBSP_vc_SmoothTg     Author: Thomas Backmeister    3.5.2024 
// 
// Computation of a common tangent direction at the connection
// point of two bspline curves from their actual tangents there.
//
// IN:
//   int meth          1/2/3 ... apply method 1/2/3
//   int con           0 ... start-start connection (cv1-cv2)
//                     1 ... start-end connection (cv1-cv2)
//                     2 ... end-start connection (cv1-cv2)
//                     3 ... end-end connection (cv1-cv2)
//   Vector *tg1       tangent of curve 1
//   Vector *tg2       tangent of curve 2
//   double pc         1. method: percentage of normalized tg1 
//                     2. method: percentage of average length 
//                     (limits see below)
// OUT:
//   Vector *tg1D      smooth tangent in direction of curve 1
//   Vector *tg2D      smooth tangent in direction of curve 2
//                     (tg1D and tg2D are parallel of same length)
//
// Returncode:
//   0 = OK
//  -1 = input error

  double l1, l2, len;
  Vector t1, t2;


  // printf("\nCBSP_vc_SmoothTg - start\n");

  // current tangents (corresponding to identical direction of curves)
  t1 = *tg1;
  t2 = *tg2;
  if (con == 0 || con == 3) UT3D_vc_mult_d (&t2, &t2, -1.0);
//----------------------------------------------------------------

  if (meth == 1) goto L_method_1;
  if (meth == 2) goto L_method_2;
  if (meth == 3) goto L_method_3;
  goto L_InErr;
    
L_method_1:
  // 1. method: weighted linear combination of normalized t1 and t2;
  //            thus any direction between t1 and t2 is possible;
  //            the new lengths are those of t1 and t2 respectively.
  if (pc < 0.0 || pc > 100.0) {
    printf("percentage should be between 0 and 100 percent\n");
    goto L_InErr;
  }
  l1 = UT3D_len_vc (&t1);
  l2 = UT3D_len_vc (&t2);
  if (!UTP_comp_0(l1)) UT3D_vc_mult_d (&t1, &t1, 1.0/l1);
  if (!UTP_comp_0(l2)) UT3D_vc_mult_d (&t2, &t2, 1.0/l2);
  UT3D_vc_mult_d (&t1, &t1, pc/100.0);
  UT3D_vc_mult_d (&t2, &t2, (100.0-pc)/100.0);
  UT3D_vc_add2vc (tg1D, &t1, &t2);
  *tg2D = *tg1D;
  UT3D_vc_setLength (tg1D, tg1D, l1);
  UT3D_vc_setLength (tg2D, tg2D, l2);
  goto L_End;

//----------------------------------------------------------------

L_method_2:
  // 2. method: angle bisector of t1 and t2 with weighted average length
  if (pc < 5.0) {
    printf("weight should be at least 5 percent\n");
    goto L_InErr;
  }
  l1 = UT3D_len_vc (&t1);
  l2 = UT3D_len_vc (&t2);
  if (!UTP_comp_0(l1)) UT3D_vc_mult_d (&t1, &t1, 1.0/l1);
  if (!UTP_comp_0(l2)) UT3D_vc_mult_d (&t2, &t2, 1.0/l2);
  UT3D_vc_add2vc (tg1D, &t1, &t2);
  len = UT3D_len_vc (tg1D);
  if (UTP_comp_0(len)) {
    printf("tg1 and tg2 are PARALLEL ==> CUSP! Cannot be handled!\n");
    TX_Error("\nCUSP! Cannot be handled!\n");
    goto L_InErr;
  }
  UT3D_vc_setLength (tg1D, tg1D, pc/100.0*((l1+l2)/2.0));
  *tg2D = *tg1D;
  goto L_End;

//----------------------------------------------------------------

L_method_3:
  // 3. method: angle bisector of t1 and t2 with length of
  //            t1 and t2 respectively
  l1 = UT3D_len_vc (&t1);
  l2 = UT3D_len_vc (&t2);
  if (!UTP_comp_0(l1)) UT3D_vc_mult_d (&t1, &t1, 1.0/l1);
  if (!UTP_comp_0(l2)) UT3D_vc_mult_d (&t2, &t2, 1.0/l2);
  UT3D_vc_add2vc (tg1D, &t1, &t2);
  len = UT3D_len_vc (tg1D);
  if (UTP_comp_0(len)) {
    printf("tg1 and tg2 are PARALLEL ==> CUSP! Cannot be handled!\n");
    TX_Error("\nCUSP! Cannot be handled!\n");
    goto L_InErr;
  }
  *tg2D = *tg1D;
  UT3D_vc_setLength (tg1D, tg1D, l1);
  UT3D_vc_setLength (tg2D, tg2D, l2);
  goto L_End;

//----------------------------------------------------------------

L_End:
  if (con == 0 || con == 3) UT3D_vc_mult_d (tg2D, tg2D, -1.0);

  return 0;

L_InErr:
  TX_Error("CBSP_vc_SmoothTg: input error");
  return -1;
}



//=============================================================
  int CBSP_vc_SmoothCr (Vector *cr1D, Vector *cr2D,
                        int meth, Vector *cr1, Vector *cr2, 
                        double pc) {
//=============================================================
// CBSP_vc_SmoothCr     assimilated curvature <-- bspline curves
// 
// CBSP_vc_SmoothCr     Author: Thomas Backmeister    3.5.2024 
// 
// Computation of an assimilated curvature vector at the connection
// point of two bspline curves from their actual curvatures there.
//
// IN:
//   int meth          1/2/3 ... apply method 1/2/3
//   Vector *cr1       curvature of curve 1
//   Vector *cr2       curvature of curve 2
//   double pc         1. & 3. method: percentage of normalized cr1 
//                                     (limits see below)
// OUT:
//   Vector *cr1D      new curvature vector of curve 1
//   Vector *cr2D      new curvature vector of curve 2
//
// Returncode:
//   0 = OK
//  -1 = input error

  double l1, l2;
  Vector c1, c2;


  // printf("\nCBSP_vc_SmoothCr - start\n");

  c1 = *cr1;
  c2 = *cr2;
//----------------------------------------------------------------

  if (meth == 1) goto L_method_1;
  if (meth == 2) goto L_method_2;
  if (meth == 3) goto L_method_3;
  goto L_InErr;

L_method_1:
  // 1. method: weighted linear combination of normalized c1 and c2;
  //            thus any direction between c1 and c2 is possible;
  //            the new curvature gets equal average length.
  if (pc < 0.0 || pc > 100.0) {
    printf("percentage should be between 0 and 100 percent!\n");
    goto L_InErr;
  }
  l1 = UT3D_len_vc (&c1);
  l2 = UT3D_len_vc (&c2);
  if (!UTP_comp_0(l1)) UT3D_vc_mult_d (&c1, &c1, 1.0/l1);
  if (!UTP_comp_0(l2)) UT3D_vc_mult_d (&c2, &c2, 1.0/l2);
  UT3D_vc_mult_d (&c1, &c1, pc/100.0);
  UT3D_vc_mult_d (&c2, &c2, (100-pc)/100.0);
  UT3D_vc_add2vc (cr1D, &c1, &c2);
  UT3D_vc_setLength (cr1D, cr1D, (l1+l2)/2.0);
  *cr2D = *cr1D;
  goto L_End;

//----------------------------------------------------------------

L_method_2:
  // 2. method: the new curvatures get equal average length;
  //            their direction remains unchanged.
  l1 = UT3D_len_vc (&c1);
  l2 = UT3D_len_vc (&c2);
  UT3D_vc_setLength (cr1D, &c1, (l1+l2)/2.0);
  UT3D_vc_setLength (cr2D, &c2, (l1+l2)/2.0);
  goto L_End;

//----------------------------------------------------------------

L_method_3:
  // 3. method: weighted linear combination of normalized c1 and c2;
  //            thus any direction between c1 and c2 is possible;
  //            the new lengths are those of c1 and c2 respectively.
  if (pc < 0.0 || pc > 100.0) {
    printf("percentage should be between 0 and 100 percent!\n");
    goto L_InErr;
  }
  l1 = UT3D_len_vc (&c1);
  l2 = UT3D_len_vc (&c2);
  if (!UTP_comp_0(l1)) UT3D_vc_mult_d (&c1, &c1, 1.0/l1);
  if (!UTP_comp_0(l2)) UT3D_vc_mult_d (&c2, &c2, 1.0/l2);
  UT3D_vc_mult_d (&c1, &c1, pc/100.0);
  UT3D_vc_mult_d (&c2, &c2, (100-pc)/100.0);
  UT3D_vc_add2vc (cr1D, &c1, &c2);
  *cr2D = *cr1D;
  UT3D_vc_setLength (cr1D, cr1D, l1);
  UT3D_vc_setLength (cr2D, cr2D, l2);
  goto L_End;

//----------------------------------------------------------------

L_End:

  return 0;

L_InErr:
  TX_Error("CBSP_vc_SmoothCr: input error");
  return -1;
}



//=============================================================
  int CBSP_vc_NewTg (Vector *dTg1, Vector *dTg2,
                     CurvBSpl *cv1, int ind1, double *c1D1,
                     CurvBSpl *cv2, int ind2, double *c2D1,
                     int con, double pc) {
//=============================================================
// CBSP_vc_NewTg     new common tangent <-- bspline curves
// 
// CBSP_vc_NewTg     Author: Thomas Backmeister   9.5.2024 
// 
// Computation of a new common tangent at a connection point
// of two bspline curves.
// Its direction is each that of the corresponding curve.
// Output is the difference vector to the old tangent vector.
//
// IN:
//   CurvBSpl *cv1      b-spline curve 1
//   int ind1           knotspan index of cv1
//   double *c1D1       first derivatives of relevant base
//                      functions for ind1
//   CurvBSpl *cv2      b-spline curve 2
//   int ind2           knotspan index of cv2
//   double *c2D1       first derivatives of relevant base
//                      functions for ind2
//   int con            0 ... start-start connection (cv1-cv2)
//                      1 ... start-end connection (cv1-cv2)
//                      2 ... end-start connection (cv1-cv2)
//                      3 ... end-end connection (cv1-cv2)
//   double pc          0-100; deviation of tangent; default = 50 (0 for both curves)
// OUT:
//   Vector *dTg1       difference of new and old tangent (curve 1)
//   Vector *dTg2       difference of new and old tangent (curve 2)
//
// Return:
//   0 = OK
//  -1 = error


  int rc, tg_meth;
  double len;
  Vector tg1, tg2, tg1D, tg2D;


  printf("CBSP_vc_NewTg-in con=%d pc=%f\n",con,pc);

  // tangent vector of curve 1
  rc = CBSP_vc_TgCrCv (&tg1, cv1, ind1, c1D1);
  if (rc < 0) goto L_Err;
    // printf("tg1= %f, %f, %f\n", tg1.dx, tg1.dy, tg1.dz);
  len = UT3D_len_vc (&tg1);
    // printf("length of tg1= %f\n", len);

  // tangent vector of curve 2
  rc = CBSP_vc_TgCrCv (&tg2, cv2, ind2, c2D1);
  if (rc < 0) goto L_Err;
    // printf("tg2= %f, %f, %f\n", tg2.dx, tg2.dy, tg2.dz);
  len = UT3D_len_vc (&tg2);
    // printf("length of tg2= %f\n", len);

  // new common tangent: tg1D for curve 1, tg2D for curve 2
  //                     (tg1D and tg2D are parallel)
  // ------------------------------------------------------
  tg_meth = 1;       // METHOD 1/2/3
  // 1. method: weighted linear combination of normalized tg1 and tg2;
  //            thus any direction between tg1 and tg2 is possible;
  //            the new lengths are those of tg1 and tg2 respectively.
  // 2. method: angle bisector of tg1 and tg2 with weighted average length
  // 3. method: angle bisector of tg1 and tg2 with length of
  //            tg1 respectively tg2.

  // pc: 1. method: percentage of normalized tg1 (0<=pc<=100)
  //                (thus percentage of normalized tg2 is 100-pc)
  //     2. method: percentage of average length (>=5%, unlimited)

    // TESTBLOCK
    // printf("\n");
    // if (tg_meth == 1) {
      // printf("weighted linear combination of normalized tg1 and tg2;\n");
      // printf("thus any direction between tg1 and tg2 is possible;\n");
      // printf("the new lengths are those of tg1 and tg2 respectively.\n");
      // printf("percentage of normalized tg1 and tg2: %f  %f\n", pc, 100-pc);
    // }
    // if (tg_meth == 2) { 
      // printf("angle bisector of tg1 and tg2 with weighted average length;\n");
      // printf("weight= %f percent\n", pc);
    // }
    // if (tg_meth == 3) 
      // printf("angle bisector of tg1 and tg2 with length of tg1 respectively tg2\n");
    // END TESTBLOCK

  //----------------------------------------------------------------
  rc = CBSP_vc_SmoothTg (&tg1D, &tg2D, tg_meth, con, &tg1, &tg2, pc);
  if (rc < 0) goto L_Err;
  len = UT3D_len_vc (&tg1D);
  len = UT3D_len_vc (&tg2D);
    // printf("\n");
    // printf("tg1D= %f, %f, %f\n", tg1D.dx, tg1D.dy, tg1D.dz);
    // printf("tg2D= %f, %f, %f\n", tg2D.dx, tg2D.dy, tg2D.dz);
    // printf("length of tg1D= %f\n", len);
    // printf("length of tg2D= %f\n", len);

  // Curve 1: tangent vector difference
  UT3D_vc_sub2vc(dTg1, &tg1D, &tg1);
    // printf("dTg1= %f, %f, %f\n", dTg1->dx, dTg1->dy, dTg1->dz);

  // Curve 2: tangent vector difference
  UT3D_vc_sub2vc(dTg2, &tg2D, &tg2);
    // printf("dTg2= %f, %f, %f\n", dTg2->dx, dTg2->dy, dTg2->dz);

    // printf("\nexit-CBSP_vc_NewTg\n");

  return 0;

  //----------------------------------------------------------------
  L_Err:
    TX_Error("CBSP_vc_NewTg: error");
    return -1;
}



//=============================================================
  int CBSP_vc_NewCr (Vector *dCr1, Vector *dCr2, double pc,
                     CurvBSpl *cv1, int ind1, double *c1D2,
                     CurvBSpl *cv2, int ind2, double *c2D2) {
//=============================================================
// CBSP_vc_NewCr     new curvature <-- bspline curves
// 
// CBSP_vc_NewCr     Author: Thomas Backmeister   10.5.2024 
// 
// Computation of a new curvature vector at a connection point
// of two bspline curves.
// Output is the difference vector to the old curvature vector.
//
// IN:
//   double pc          deviation tangent 0-100; 50 = no deviation;
//   CurvBSpl *cv1      b-spline curve 1
//   int ind1           knotspan index of cv1
//   double *c1D2       second derivatives of relevant base
//                      functions for ind1
//   CurvBSpl *cv2      b-spline curve 2
//   int ind2           knotspan index of cv2
//   double *c2D2       second derivatives of relevant base
//                      functions for ind2
// OUT:
//   Vector *dCr1       difference of new and old curvature (curve 1)
//   Vector *dCr2       difference of new and old curvature (curve 2)
//
// Return:
//   0 = OK
//  -1 = error

  int    rc, cr_meth;
  double len;
  Vector cr1, cr2, cr1D, cr2D;


  // printf("\nCBSP_vc_NewCr-in pc=%f\n",pc);

  // curvature vector of curve 1
  rc = CBSP_vc_TgCrCv (&cr1, cv1, ind1, c1D2);
  if (rc < 0) goto L_Err;
  len = UT3D_len_vc (&cr1);
    // printf("cr1= %f, %f, %f\n", cr1.dx, cr1.dy, cr1.dz);
    // printf("length of cr1= %f\n", len);

  // curvature vector of curve 2
  rc = CBSP_vc_TgCrCv (&cr2, cv2, ind2, c2D2);
  if (rc < 0) goto L_Err;
  len = UT3D_len_vc (&cr2);
    // printf("cr2= %f, %f, %f\n", cr2.dx, cr2.dy, cr2.dz);
    // printf("length of cr2= %f\n", len);


  // ===========================================================
  // desired smooth curvature at curve connection
  // (the curvature vector is INDEPENDENT of curve direction !!)
  // ===========================================================
  cr_meth = 3;      // METHOD 1/2/3
  // 1. method: weighted linear combination of normalized cr1 and cr2;
  //            thus any direction between cr1 and cr2 is possible;
  //            the new curvature gets average length.
  // 2. method: the new curvatures get equal average length;
  //            their direction remains unchanged.
  // 3. method: weighted linear combination of normalized cr1 and cr2;
  //            thus any direction between cr1 and cr2 is possible;
  //            the new lengths are those of cr1 and cr2 respectively.

  // pc  1.& 3. method: percentage of normalized cr1 (0<=pc<=100)
  //            (thus percentage of normalized cr2 is 100-pc)

/*
  printf("\n");
  if (cr_meth == 1) {
    printf("weighted linear combination of normalized cr1 and cr2;\n");
    printf("thus any direction between cr1 and cr2 is possible;\n");
    printf("the new curvature gets average length.\n");
    printf("percentage of normalized cr1 and cr2: %f  %f\n", pc, 100-pc);
  }
  if (cr_meth == 2) {
    printf("the new curvatures get equal average length;\n");
    printf("their direction remains unchanged.\n");
  }
  if (cr_meth == 3) {
    printf("weighted linear combination of normalized cr1 and cr2;\n");
    printf("thus any direction between cr1 and cr2 is possible;\n");
    printf("the new lengths are those of cr1 and cr2 respectively.\n");
    printf("percentage of normalized cr1 and cr2: %f  %f\n", pc, 100-pc);
  }
*/

  //----------------------------------------------------------------
  rc = CBSP_vc_SmoothCr (&cr1D, &cr2D, cr_meth, &cr1, &cr2, pc);
  if (rc < 0) goto L_Err;
  len = UT3D_len_vc (&cr1D);
  len = UT3D_len_vc (&cr2D);

    // TESTBLOCK
    // printf("\n");
    // printf("cr1D= %f, %f, %f\n", cr1D.dx, cr1D.dy, cr1D.dz);
    // printf("cr2D= %f, %f, %f\n", cr2D.dx, cr2D.dy, cr2D.dz);
    // printf("length of cr1D= %f\n", len);
    // printf("length of cr2D= %f\n", len);
    // END TESTBLOCK

  // Curve 1: curvature vector difference
  UT3D_vc_sub2vc(dCr1, &cr1D, &cr1);
  //printf("dCr1= %f, %f, %f\n", dCr1->dx, dCr1->dy, dCr1->dz);

  // Curve 2: curvature vector difference
  UT3D_vc_sub2vc(dCr2, &cr2D, &cr2);
  //printf("dCr2= %f, %f, %f\n", dCr2->dx, dCr2->dy, dCr2->dz);

    // printf("\nCBSP_vc_NewCr - END\n");

  return 0;

L_Err:
  TX_Error("CBSP_vc_NewCr: error");
  return -1;
}



//=============================================================
  int CBSP_MoveCtrlPts (CurvBSpl *cv, int pos, int ind,
                        double *D1, double *D2, Vector *dTg,
                        Vector *dCr, int opt) {
//=============================================================
// CBSP_MoveCtrlPts     move controlpoints <-- bspline curve
// 
// CBSP_MoveCtrlPts     Author: Thomas Backmeister    10.5.2024 
// 
// Move the relevant control points P(ind-deg),...,P(ind) to obtain
// the desired tangent or/and curvature continuity.
// If P(ind-deg) is the FIRST of all control points, it will not be moved.
// If P(ind) is the LAST of all control points, it will not be moved.
// The b-spline curve degree must be at least 2!
//
// IN:
//   CurvBSpl *cv      b-spline curve
//   int pos           0 / 1 / 2 ... curve's start / end / middle
//   int ind           relevant knot span index
//   double *D1        first derivatives of relevant base functions
//   double *D2        second derivatives of relevant base functions
//   Vector *dTg       tangent vector difference
//   Vector *dCr       curvature vector difference
//   int opt           1 ... create tangent continuity
//                     2 ... create tangent & curvature continuity
//                     3 ... create curvature continuity with unchanged tangent
// OUT:
//   CurvBSpl *cv      b-spline curve
//
// Returncode:
//   0 = OK
//  -1 = error
  int rc;
  double len;
  Vector tg, cr;


  // printf("CBSP_MoveCtrlPts - START\n");

  if (opt == 1) {
    //================================================================
    //create TANGENT continuity
    //================================================================
    rc = CBSP_TgOrCrMoveCtrlPts (cv, pos, ind, D1, dTg);
    if (rc < 0) goto L_Err;


  } else if (opt == 2) {
    //================================================================
    //create TANGENT & CURVATURE continuity
    //================================================================
    rc = CBSP_TgOrCrMoveCtrlPts (cv, pos, ind, D1, dTg);
    if (rc < 0) goto L_Err;


  } else if (opt == 3) {
    //================================================================
    //create CURVATURE continuity with unchanged tangent
    //================================================================
    rc = CBSP_TgAndCrMoveCtrlPts (cv, pos, ind, D1, D2, dTg, dCr);
    if (rc < 0) goto L_Err;
  }

  // Testdata:

  // tangent vector of modified curve
  rc = CBSP_vc_TgCrCv (&tg, cv, ind, D1);
  if (rc < 0) goto L_Err;
  len = UT3D_len_vc (&tg);
    // printf("new tg= %f, %f, %f\n", tg.dx, tg.dy, tg.dz);
    // printf("with length= %f\n", len);

  // curvature vector of modified curve
  rc = CBSP_vc_TgCrCv (&cr, cv, ind, D2);
  if (rc < 0) goto L_Err;
  len = UT3D_len_vc (&cr);
    // printf("new cr= %f, %f, %f\n", cr.dx, cr.dy, cr.dz);
    // printf("with length= %f\n", len);

    // printf("exit-CBSP_MoveCtrlPts\n");

  return 0;


L_Err:
  TX_Error("CBSP_MoveCtrlPts: error");
  return -1;
}



//====================== EOF ===========================
