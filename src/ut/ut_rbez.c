// ut_rbez.c              Maintainer: Thomas Backmeister       26.6.2003
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
2003-06-30 Neu.  TB.

-----------------------------------------------------
*/
/*!
\file  ../ut/ut_rbez.c
\brief Rational-Bezier-Curve - Utilityfunctions
\code
=====================================================
List_functions_start:

UT3D_pt_intelel        intersection ellipse - ellipse
UT3D_pt_intciel        intersection circle - ellipse
UT3D_pt_intelbspl      intersection ellipse - B-spline
UT3D_pt_intcibspl      intersection circle - B-spline
UT3D_pta_rbez          make Polygon from rational-bezier-curve

UT3D_pt_int_ln_rbez    intersect line X rational-bezier-curve
UT3D_pt_int2rbez       intersect 2 rational Bezier curves
UT3D_pt_intbezrbez     intersect integral - rational Bezier curve

UT3D_rbez_ci__
UT3D_rbez_ci_0
UT3D_rbez_ci_180
UT3D_rbez_ci_360
UT3D_rbez_el           rational quadratic Bezier curves <-- 3D-ellipse
UT3D_rbez_rbsp         rational-bezier-curve from rational-B-spline

UT3D_rbez_subdiv       subdivide rational Bezier curve
UT3D_wPTn_ptn_wn       get wPoint-array from controlpoints and weights

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


Rational-Bezier-Curve must have POSITIVE weights !!


*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




#include "../ut/ut_geo.h"
#include "../ut/gr_types.h"               // SYM_* ATT_* Typ_Att_* LTYP_*





//======================================================================
  int UT3D_wPTn_ptn_wn (wPoint *wpa, int ptNr, Point *pta, double *wa) {
//======================================================================
/// \code
/// UT3D_wPTn_ptn_wn       get wPoint-array from controlpoints and weights
/// DOES MULTIPLY COORDS X WEIGHT
/// \endcode


  int    i1;


  // copy controlpoints and weights into wPoint-array
  // transfer rbsp-curve from format point[3] + weight into format point[4]
  for (i1=0; i1<ptNr; ++i1) {
    wpa[i1].x = pta[i1].x * wa[i1];
    wpa[i1].y = pta[i1].y * wa[i1];
    wpa[i1].z = pta[i1].z * wa[i1];
    wpa[i1].w = wa[i1];
      // DEB_dump_obj__ (Typ_WPT, &wpa[i1], "wpa [%d]=",i1);
  }

  return 0;

}


//================================================================
  int UT3D_pt_int_ln_rbez (int *nxp, Point *xptab, int ptxMax,
                           Line *ln1, CurvRBez *rbez,
                           Memspc *memSeg1, double tol) {
//================================================================
// UT3D_pt_int_ln_rbez    intersect line X rational-bezier-curve
// see UT3D_pt_intlnbcv
// UT3D_rbez_subdiv

  int      rc, i1;
  double   dist, tol_d;
  Point    pmin, pmax, xp1, *pp1, *pp2;
  CurvRBez bcv1, bcv2;


  // printf("UT3D_pt_intlnbcv %d\n",*nxp);


  // make bounding box of Bezier curve
  UT3D_box_pts (&pmin, &pmax, rbez->ptNr, rbez->cptab);
    // GR_Disp_box (&pmin, &pmax, 9);


  // if line does not intersect bounding box -> return
  if (!UT3D_box_ck_intLn (ln1, &pmin, &pmax, UT_TOL_pt)) return 0;


  // --- Bezier curve is no (not yet) line ---
  if (!UT3D_pt_ck_linear (rbez->ptNr, rbez->cptab, tol)) {

    // subdivide Bezier curve
    rc = UT3D_rbez_subdiv (&bcv1, &bcv2, memSeg1, rbez, 0.5);
    if(rc < 0) return rc;

    // repeat this function with bcv1 and bcv2 (recursion !)
    if(UT3D_pt_int_ln_rbez(nxp,xptab,ptxMax,ln1,&bcv1,memSeg1,tol) < 0) return rc;
    if(UT3D_pt_int_ln_rbez(nxp,xptab,ptxMax,ln1,&bcv2,memSeg1,tol) < 0) return rc;



  // --- Bezier curve is a (limited) line ---
  } else {
      // printf(" UT3D_pt_intlnbcv ===== test line\n");

    // line from Bezier curve
    pp1 = &rbez->cptab[0];
    pp2 = &rbez->cptab[rbez->ptNr - 1];

    // bereits ein Punkt: intersectPoint found.
    if(UT3D_len_2pt(pp1, pp2) < UT_TOL_pt) {
      xp1 = *pp1;
      goto L_found;
    }


    // intersect limited lines
    rc = UT3D_pt_int2pt2pt_lim (&xp1, NULL, NULL,
                            &ln1->p1, &ln1->p2, pp1, pp2, tol);
    if(rc < 0) return 0;
      // GR_Disp_pt (&xp1, SYM_STAR_S, ATT_COL_RED);


    // check distance to last found point (schleifende Schnitte entfernen)
    // see also UT3D_pt_int2rbez
    i1 = *nxp - 1;
    // if(rc > 0) {
      // this point is on 2 parallel lines
      if(i1 >= 0) {
        tol_d = tol * tol * 100.;
        // if(rc > 0) tol_d *= 1000.;
        tol_d *= 1000.;
        dist = UT3D_lenq_PtPt (&xp1, &xptab[i1]);
          // printf(" nxp=%d dist=%lf tol_d=%lf\n",*nxp,dist,tol_d);
        // if(sqrt(dist) < UT_DISP_cv * 10.) {
        if(dist < tol_d) {
          UT3D_pt_mid2pt (&xptab[i1], &xptab[i1], &xp1);
            // printf(" ********** remove schleifenden Schnitt ..\n");
          return 0;
        }
      }
    // }


    // save intersection point
      // printf(" xp[%d]=%f,%f,%f\n",*nxp,xp1.x,xp1.y,xp1.z);
    L_found:
    if (*nxp >= ptxMax) { TX_Error("UT3D_pt_intlnbcv E002"); return -2; }
    xptab[*nxp] = xp1;
    ++(*nxp);
  }

    // printf("ex _intlnbcv nxp=%d\n",*nxp);

  return 0;
}


//===================================================================
  int UT3D_pta_rbez (int *ptNr, Point *pTab, CurvRBez *rbez,
                    int ptMax, double tol) {
//===================================================================
// UT3D_pta_rbez             make Polygon from rational-bezier-curve
// not yet implem. Use UT3D_rbsp_rbez & UT3D_cv_rbsp
// see UT3D_pta_bez bspl_pol_bsp

  // irc = UT3D_bez_eval_Pt (&pt1, bez, 0.5);
  // GR_Disp_pt (&pt1, SYM_STAR_S, 0);

  int     ianz, irc, ptOut, ptStackNr;
  double  pVal, kvs, kve, us, ue, dist, uStack[20];
  Point   pt1, pt2, pts, pte, ptStack[20];


  DEB_dump_obj__ (Typ_CVRBEZ, rbez, "UT3D_pta_rbez ptMax=%d\n",ptMax);
  TX_Print("ERROR: UT3D_pta_rbez not yet implemented");

  return -1;

}


/*=======================================================================*/
  int UT3D_pt_intelel (int *np, Point xptab[], CurvElli *el1, CurvElli *el2,
                       Memspc *memSeg1) {
/*===================
UT3D_pt_intelel        intersection ellipse - ellipse

UT3D_pt_intelel        Author: Thomas Backmeister       27.6.2003

Intersection of two 3D-ellipses.

IN:
  CurvElli *el1    ... ellipse 1
  CurvElli *el2    ... ellipse 2
  Memspc *memSeg1 ... memory for temporary elements (ca 50 kb)
OUT:
  int *np         ... number of intersection points (maximum = 16)
  Point xptab[]   ... intersection points (maximum = 16)
Returncodes:
  0 = OK
 -1 = out of tempSpace
 <0 = internal error
*/

  int      rc, nxp, i1, i2, i3;
  Point    xp[4];
  CurvRBez rb1tab[4], rb2tab[4];
  void     *memPos0, *memPos1;



  // printf("UT3D_pt_intelel\n");


  // init
  rc = 0;
  *np = 0;
  memPos0 = memSeg1->next;


  // convert ellipse 1 to 4 rational Bezier curves
  rc = UT3D_rbez_el (rb1tab, memSeg1, el1);
  if (rc < 0) goto Fertig;
  // UME_dump (memSeg1);


  // convert ellipse 2 to 4 rational Bezier curves
  rc = UT3D_rbez_el (rb2tab, memSeg1, el2);
  if (rc < 0) goto Fertig;
  // UME_dump (memSeg1);    // exit(0);


  // --- compute intersection points ---
  memPos1 = memSeg1->next;

  for (i1=0; i1<4; ++i1) {
    for (i2=0; i2<4; ++i2) {
 
      // intersect 2 rational Bezier curves
      nxp = 0;
      rc = UT3D_pt_int2rbez (&nxp, xp, 4, &rb1tab[i1], &rb2tab[i2],
                             memSeg1, UT_TOL_ln);
      // printf(" nach UT3D_pt_int2rbez %d %d\n",rc,nxp);
      if (rc < 0) goto Fertig;

      if (nxp > 0) {
        // save X-points
        for (i3=0; i3<nxp; ++i3) {
          if (*np > 15) goto Fertig;
          xptab[*np] = xp[i3];
          *np += 1;  //++(*np);
        }
      }

      // reset memSeg1 (UT3D_pt_int2rbez needs only temp. objects)
      // UME_dump (memSeg1);
      memSeg1->next = memPos1;
    }
  }


  Fertig:
  // reset memSeg1
  memSeg1->next = memPos0;

  return rc;
}


/*=======================================================================*/
  int UT3D_pt_intciel (int *np, Point xptab[], Circ *ci, CurvElli *el,
                       Memspc *memSeg1) {
/*===================
UT3D_pt_intciel        intersection lim. circle - unlim. ellipse
  

UT3D_pt_intciel        Author: Thomas Backmeister       27.6.2003

Intersection of 3D-circle and 3D-ellipse.

IN:
  Circ *ci        ... circle
  CurvElli *el     ... ellipse
  Memspc *memSeg1 ... memory for temporary elements   (ca 50 kb)
OUT:
  int *np         ... number of intersection points (maximum = 16)
  Point xptab[]   ... intersection points
Returncodes:
  0 = OK
 -1 = out of tempSpace
 <0 = internal error
*/

  int      rc, nxp, i1, i2, i3, cv1Nr, cv2Nr;
  Point    xp[2];
  CurvRBez rb1tab[3], rb2tab[4];
  void     *memPos0, *memPos1;




  // printf("\nUT3D_pt_intciel\n");


  // init
  rc = 0;
  *np = 0;
  memPos0 = memSeg1->next;


  // convert circle to 1-3 rational Bezier curves
  rc = UT3D_rbez_ci__ (&cv1Nr, rb1tab, memSeg1, ci);
  if (rc < 0) goto Fertig;
  // UME_dump (memSeg1,"n.conv.ci");


  // convert ellipse to 4 rational Bezier curves
  rc = UT3D_rbez_el (rb2tab, memSeg1, el);
  if (rc < 0) goto Fertig;
  cv2Nr = 4;
  // UME_dump (memSeg1,"n.conv.el");


  // --- compute intersection points ---
  memPos1 = memSeg1->next;
  for (i1=0; i1<cv1Nr; ++i1) {
    for (i2=0; i2<cv2Nr; ++i2) {
 
      // intersect 2 rational Bezier curves
      nxp = 0;
      rc = UT3D_pt_int2rbez (&nxp, xp, 2, &rb1tab[i1], &rb2tab[i2],
                             memSeg1, UT_TOL_ln);
      if (rc < 0) goto Fertig;

      if (nxp > 0) {
        // save X-points
        for (i3=0; i3<nxp; ++i3) {
          if (*np > 15) goto Fertig;
          xptab[*np] = xp[i3];
          ++(*np);
        }
      }

      // reset memSeg1 (UT3D_pt_int2rbez needs only temp. objects)
      // UME_dump (memSeg1);
      memSeg1->next = memPos1;
    }
  }

  Fertig:
  // reset memSeg1
  memSeg1->next = memPos0;

  return rc;
}




/*=======================================================================*/
  int UT3D_pt_intelbspl (int *np, Point xptab[], CurvElli *el, CurvBSpl *bspl,
                         Memspc *memSeg1) {
/*=====================
UT3D_pt_intelbspl        intersection ellipse - B-spline

UT3D_pt_intelbspl        Author: Thomas Backmeister       27.6.2003

Intersection of 3D-ellipse and B-spline curve.

IN:
  int *np         ... maximal number of output points
  CurvElli *el     ... ellipse
  CurvBSpl *bspl  ... B-spline curve
  Memspc *memSeg1 ... memory for temporary elements
OUT:
  int *np         ... number of intersection points
  Point xptab[]   ... intersection points
Returncodes:
  0 = OK
 -1 = out of tempSpace
 -2 = out of mem (xptab too small)
 <0 = internal error
*/

  int      rc, nxp, i1, i2, i3, nbcv, xptSiz;
  Point    xp[UT_BEZDEG_MAX];
  CurvBez  *ibtab;
  CurvRBez rbtab[4];
  ObjGX    beztab;
  void     *memPos0, *memPos1;


  // printf("\nUT3D_pt_intelbspl: intersection ellipse - B-spline\n");


  // init
  rc = 0;
  xptSiz = *np;
  *np = 0;
  memPos0 = memSeg1->next;

  // convert ellipse to 4 rational Bezier curves
  rc = UT3D_rbez_el (rbtab, memSeg1, el);
  if (rc < 0) goto Fertig;

  // convert b-spline to Bezier curves
  UT3D_bez_bspl__ (&beztab, memSeg1, bspl);
  if (beztab.data == NULL) {
    TX_Error("UT3D_pt_intelbspl: out of tempSpace");
    rc = -1;
    goto Fertig;
  }
  nbcv = beztab.siz;
  ibtab = (CurvBez*) (beztab.data);

  // --- compute intersection points ---
  memPos1 = memSeg1->next;
  for (i1=0; i1<nbcv; ++i1) {
    for (i2=0; i2<4; ++i2) {
 
      // intersect integral and rational Bezier curve
      nxp = 0;
      rc = UT3D_pt_intbezrbez (&nxp, xp, UT_BEZDEG_MAX, &ibtab[i1], &rbtab[i2],
                               memSeg1, UT_TOL_ln);
      if (rc < 0) goto Fertig;

      if (nxp > 0) {
        // save X-points
        if (*np + nxp >= xptSiz) {
          TX_Error("UT3D_pt_intelbspl E002");
          rc = -2;
          goto Fertig;
        }
        for (i3=0; i3<nxp; ++i3) {
          xptab[*np] = xp[i3];
          ++(*np);
        }
      }

      // reset memSeg1 (UT3D_pt_intbezrbez needs only temp. objects)
      // UME_dump (memSeg1);
      memSeg1->next = memPos1;
    }
  }

  Fertig:
  // reset memSeg1
  memSeg1->next = memPos0;

  return rc;
}




/*=======================================================================*/
  int UT3D_pt_intcibspl (int *np, Point xptab[], Circ *ci, CurvBSpl *bspl,
                         Memspc *memSeg1) {
/*=====================
UT3D_pt_intcibspl        intersection lim. circle - B-spline

UT3D_pt_intcibspl        Author: Thomas Backmeister       28.6.2003

Intersection of 3D-circle and B-spline curve.

IN:
  int *np         ... maximal number of output points
  Circ *ci        ... circle
  CurvBSpl *bspl  ... B-spline curve
  Memspc *memSeg1 ... memory for temporary elements      (ca 50 kb)
OUT:
  int *np         ... number of intersection points
  Point xptab[]   ... intersection points
Returncodes:
  0 = OK
 -1 = out of tempSpace
 -2 = out of mem (xptab too small)
 <0 = internal error
*/

  int      rc, nxp, i1, i2, i3, nbcv, xptSiz, rbNr;
  Point    xp[UT_BEZDEG_MAX];
  CurvBez  *ibtab;
  CurvRBez rbtab[3];
  ObjGX    beztab;
  void     *memPos0, *memPos1;



  // printf("\nUT3D_pt_intcibspl: intersection circle - B-spline\n");

  // init
  rc = 0;
  xptSiz = *np;
  *np = 0;
  memPos0 = memSeg1->next;


  // convert circle to 1-3 rational Bezier curves
  rc = UT3D_rbez_ci__ (&rbNr, rbtab, memSeg1, ci);
  if (rc < 0) goto Fertig;


  // convert b-spline to Bezier curves
  UT3D_bez_bspl__ (&beztab, memSeg1, bspl);
  if (beztab.data == NULL) {
    TX_Error("UT3D_pt_intcibspl: out of tempSpace");
    rc = -1;
    goto Fertig;
  }
  nbcv = beztab.siz;
  ibtab = (CurvBez*) (beztab.data);


  // --- compute intersection points ---
  memPos1 = memSeg1->next;
  for (i1=0; i1<nbcv; ++i1) {
    for (i2=0; i2<rbNr; ++i2) {
 
      // intersect integral and rational Bezier curve
      nxp = 0;
      rc = UT3D_pt_intbezrbez (&nxp, xp, UT_BEZDEG_MAX, &ibtab[i1], &rbtab[i2],
                               memSeg1, UT_TOL_ln);
      if (rc < 0) goto Fertig;

      if (nxp > 0) {
        // save X-points
        if (*np + nxp >= xptSiz) {
          TX_Error("UT3D_pt_intcibspl E002");
          rc = -2;
          goto Fertig;
        }
        for (i3=0; i3<nxp; ++i3) {
          xptab[*np] = xp[i3];
          ++(*np);
        }
      }

      // reset memSeg1 (UT3D_pt_intbezrbez needs only temp. objects)
      // UME_dump (memSeg1);
      memSeg1->next = memPos1;
    }
  }

  Fertig:
  // reset memSeg1
  memSeg1->next = memPos0;

  return rc;
}




/*=======================================================================*/
  int UT3D_pt_int2rbez (int *nxp, Point *xptab, int ptxMax,
                        CurvRBez *rbez1, CurvRBez *rbez2,
                        Memspc *memSeg1, double tol)        {
/*====================
UT3D_pt_int2rbez      intersect 2 rational Bezier curves

UT3D_pt_int2rbez      Author: Thomas Backmeister       26.6.2003

Intersection of two rational Bezier curves.

ATTENTION: all weights of the two curves must be >0 !!!
Reason: only then the Convex Hull Property is valid; the latter is
necessary for computing correct bounding boxes.

The function works recursively!
Befor calling set nxp = 0 !

IN:
  int ptxMax      ... maximal number of output points
  CurvRBez *rbez1 ... rational Bezier curve 1
  CurvRBez *rbez2 ... rational Bezier curve 2
  double tol      ... tolerance for considering rational Bezier curve as line
                      (as smaller tol as straighter the curve)
  Memspc *memSeg1 ... memory for temporary elements
OUT:
  int *nxp        ... number of intersection points
  Point *xptab    ... intersection points
Returncodes:
  0 = OK
 -1 = out of mem (Memspc too small)
 -2 = out of mem (xptab too small)
 -3 = input error
*/

  int      rc, i1;
  double   dist, tol_d;
  Point    pmin1, pmax1, pmin2, pmax2, xp1, xp2;
  Line     l1, l2;
  CurvRBez rbez3, rbez4;



  // printf("\nUT3D_pt_int2rbez: intersect 2 rational Bezier curves\n");


  // check positivity of all weights of the curves
  for (i1=0; i1<rbez1->ptNr; ++i1) {
    if (rbez1->wtab[i1] <= 0.0) {
      printf("UT3D_pt_int2rbez: input error\n");
      return -3;
    }
  }

  for (i1=0; i1<rbez2->ptNr; ++i1) {
    if (rbez2->wtab[i1] <= 0.0) {
      printf("UT3D_pt_int2rbez: input error\n");
      return -3;
    }
  }


  // bounding box of rational Bezier curve 1
  UT3D_box_pts (&pmin1, &pmax1, rbez1->ptNr, rbez1->cptab);

  // bounding box of rational Bezier curve 2
  UT3D_box_pts (&pmin2, &pmax2, rbez2->ptNr, rbez2->cptab);

  // bounding boxes do not intersect -> return
  if (pmin2.x > (pmax1.x + UT_TOL_min1) ||
      pmin2.y > (pmax1.y + UT_TOL_min1) ||
      pmin2.z > (pmax1.z + UT_TOL_min1) ||
      pmin1.x > (pmax2.x + UT_TOL_min1) ||
      pmin1.y > (pmax2.y + UT_TOL_min1) ||
      pmin1.z > (pmax2.z + UT_TOL_min1))
    return 0;



  // --- rational Bezier curve 1 is no line ---
  if (!UT3D_pt_ck_linear (rbez1->ptNr, rbez1->cptab, tol)) {

    // subdivide rational Bezier curve 1
    rc = UT3D_rbez_subdiv (&rbez3, &rbez4, memSeg1, rbez1, 0.5);
    if(rc < 0) return rc;

    // repeat this function with rbez3 and rbez4
    rc = UT3D_pt_int2rbez (nxp, xptab, ptxMax, &rbez3, rbez2, memSeg1, tol);
    if(rc < 0) return rc;

    rc = UT3D_pt_int2rbez (nxp, xptab, ptxMax, &rbez4, rbez2, memSeg1, tol);
    if(rc < 0) return rc;



  // --- rational Bezier curve 2 is no line ---
  } else if (!UT3D_pt_ck_linear (rbez2->ptNr, rbez2->cptab, tol)) {

    // subdivide rational Bezier curve 2
    rc = UT3D_rbez_subdiv (&rbez3, &rbez4, memSeg1, rbez2, 0.5);
    if(rc < 0) return rc;

    // repeat this function with rbez3 and rbez4
    rc = UT3D_pt_int2rbez (nxp, xptab, ptxMax, rbez1, &rbez3, memSeg1, tol);
    if(rc < 0) return rc;

    rc = UT3D_pt_int2rbez (nxp, xptab, ptxMax, rbez1, &rbez4, memSeg1, tol);
    if(rc < 0) return rc;



  // --- both rational Bezier curves are (limited) lines ---
  } else {

    // lines from rational Bezier curves
    l1.p1 = rbez1->cptab[0];
    l1.p2 = rbez1->cptab[rbez1->ptNr - 1];
    l2.p1 = rbez2->cptab[0];
    l2.p2 = rbez2->cptab[rbez2->ptNr - 1];


    // l1 ist bereits ein Punkt: liegt er auf l2 ?
    if(UT3D_len_2pt(&l1.p1, &l1.p2) < UT_TOL_min1) {
      xp1 = l1.p1;
      if(!UT3D_pt_ck_inLine (&l2.p1, &l2.p2, &xp1, UT_TOL_min1)) return 0;
      goto L_found;
    }


    // l2 ist bereits ein Punkt: liegt er auf l1 ?
    if(UT3D_len_2pt(&l2.p1, &l2.p2) < UT_TOL_min1) {
      xp1 = l2.p1;
      if(!UT3D_pt_ck_inLine (&l1.p1, &l1.p2, &xp1, UT_TOL_min1)) return 0;
      goto L_found;
    }

    // intersect limited lines  // 2014-08-05
    rc = UT3D_pt_int2pt2pt_lim (&xp1, NULL, NULL,
                            &l1.p1, &l1.p2, &l2.p1, &l2.p2, tol);
    if(rc < 0) return 0;
      // GR_Disp_pt (&xp1, SYM_STAR_S, ATT_COL_RED);


    // check distance to last found point (schleifende Schnitte entfernen)
    // see also UT3D_pt_intlnbcv
    i1 = *nxp - 1;
    // if(rc > 0) {
      // this point is on 2 parallel lines
      if(i1 >= 0) {
        tol_d = tol * tol * 100.;
        if(rc > 0) tol_d *= 1000.;
        dist = UT3D_lenq_PtPt (&xp1, &xptab[i1]);
          // printf(" nxp=%d dist=%lf tol_d=%lf\n",*nxp,dist,tol_d);
        // if(sqrt(dist) < UT_DISP_cv * 10.) {
        if(dist < tol_d) {
          UT3D_pt_mid2pt (&xptab[i1], &xptab[i1], &xp1);
            // printf(" ********** remove schleifenden Schnitt ..\n");
          return 0;
        }
      }
    // }


    // check array size for X-points
    L_found:
    if (*nxp >= ptxMax) {
      TX_Error("UT3D_pt_int2rbez E002_%d",*nxp);
      return -2;
    }

    // save intersection point
    xptab[*nxp] = xp1;
    ++(*nxp);
  }

  return 0;
}




/*=======================================================================*/
  int UT3D_pt_intbezrbez (int *nxp, Point *xptab, int ptxMax,
                          CurvBez *ibcv, CurvRBez *rbez,
                          Memspc *memSeg1, double tol)        {
/*======================
UT3D_pt_intbezrbez      intersect integral - rational Bezier curve

UT3D_pt_intbezrbez      Author: Thomas Backmeister       28.6.2003

Intersection of an integral with a rational Bezier curve.
(integral := non rational)

ATTENTION: all weights of the rational curve must be >0 !!!
Reason: only then the Convex Hull Property is valid; the latter is
necessary for computing correct bounding boxes.

The function works recursively!
Befor calling set nxp = 0 !

IN:
  int ptxMax      ... maximal number of output points
  CurvBez *ibcv   ... integral Bezier curve
  CurvRBez *rbez  ... rational Bezier curve
  double tol      ... tolerance for considering rational Bezier curve as line
                      (as smaller tol as straighter the curve)
  Memspc *memSeg1 ... memory for temporary elements
OUT:
  int *nxp        ... number of intersection points
  Point *xptab    ... intersection points
Returncodes:
  0 = OK
 -1 = out of mem (Memspc too small)
 -2 = out of mem (xptab too small)
 -3 = input error
*/

  int      rc, i1;
  double   dist;
  Point    pmin1, pmax1, pmin2, pmax2, xp1, xp2;
  Line     l1, l2;
  CurvBez  ibcv3, ibcv4;
  CurvRBez rbez3, rbez4;



  //printf("\nUT3D_pt_intbezrbez: X integral - rational Bezier curve\n");

  // check positivity of all weights of the rational curve
  for (i1=0; i1<rbez->ptNr; ++i1) {
    if (rbez->wtab[i1] <= 0.0) {
      printf("UT3D_pt_intbezrbez: input error\n");
      return -3;
    }
  }

  // bounding box of integral Bezier curve
  UT3D_box_pts (&pmin1, &pmax1, ibcv->ptNr, ibcv->cptab);

  // bounding box of rational Bezier curve
  UT3D_box_pts (&pmin2, &pmax2, rbez->ptNr, rbez->cptab);

  // bounding boxes do not intersect -> return
  if (pmin2.x > (pmax1.x + UT_TOL_min1) ||
      pmin2.y > (pmax1.y + UT_TOL_min1) ||
      pmin2.z > (pmax1.z + UT_TOL_min1) ||
      pmin1.x > (pmax2.x + UT_TOL_min1) ||
      pmin1.y > (pmax2.y + UT_TOL_min1) ||
      pmin1.z > (pmax2.z + UT_TOL_min1))
    return 0;


  // --- integral Bezier curve is no line ---
  if (!UT3D_pt_ck_linear (ibcv->ptNr, ibcv->cptab, tol)) {

    // subdivide integral Bezier curve
    rc = UT3D_bez_subdiv (&ibcv3, &ibcv4, memSeg1, ibcv, 0.5);
    if(rc < 0) return rc;

    // repeat this function with ibcv3 and ibcv4
    rc = UT3D_pt_intbezrbez (nxp, xptab, ptxMax, &ibcv3, rbez, memSeg1, tol);
    if(rc < 0) return rc;

    rc = UT3D_pt_intbezrbez (nxp, xptab, ptxMax, &ibcv4, rbez, memSeg1, tol);
    if(rc < 0) return rc;



  // --- rational Bezier curve is no line ---
  } else if (!UT3D_pt_ck_linear (rbez->ptNr, rbez->cptab, tol)) {

    // subdivide rational Bezier curve
    rc = UT3D_rbez_subdiv (&rbez3, &rbez4, memSeg1, rbez, 0.5);
    if(rc < 0) return rc;

    // repeat this function with rbez3 and rbez4
    rc = UT3D_pt_intbezrbez (nxp, xptab, ptxMax, ibcv, &rbez3, memSeg1, tol);
    if(rc < 0) return rc;

    rc = UT3D_pt_intbezrbez (nxp, xptab, ptxMax, ibcv, &rbez4, memSeg1, tol);
    if(rc < 0) return rc;



  // --- both Bezier curves are (limited) lines ---
  } else {

    // lines from Bezier curves
    l1.p1 = ibcv->cptab[0];
    l1.p2 = ibcv->cptab[ibcv->ptNr - 1];
    l2.p1 = rbez->cptab[0];
    l2.p2 = rbez->cptab[rbez->ptNr - 1];

    // l1 ist bereits ein Punkt: liegt er auf l2 ?
    if(UT3D_len_2pt(&l1.p1, &l1.p2) < UT_TOL_min1) {
      xp1 = l1.p1;
      if(!UT3D_pt_ck_inLine (&l2.p1, &l2.p2, &xp1, UT_TOL_min1)) return 0;
      goto L_found;
    }

    // l2 ist bereits ein Punkt: liegt er auf l1 ?
    if(UT3D_len_2pt(&l2.p1, &l2.p2) < UT_TOL_min1) {
      xp1 = l2.p1;
      if(!UT3D_pt_ck_inLine (&l1.p1, &l1.p2, &xp1, UT_TOL_min1)) return 0;
      goto L_found;
    }

    // intersect lines
    rc = UT3D_pt_int2ln (&xp1, &xp2, &dist, &l1, &l2);
    if (rc < 1) return 0;

    // check if X-point is inside both line segments
    if(!UT3D_pt_ck_inLine (&(l1.p1), &(l1.p2), &xp1, UT_TOL_min1)) return 0;
    if(!UT3D_pt_ck_inLine (&(l2.p1), &(l2.p2), &xp1, UT_TOL_min1)) return 0;

    // check array size for X-points
    L_found:
    if (*nxp >= ptxMax) {
      TX_Error("UT3D_pt_intbezrbez E002");
      return -2;
    }

    // save intersection point
    xptab[*nxp] = xp1;
    ++(*nxp);
  }

  return 0;
}





/*=======================================================================*/
  int UT3D_rbez_subdiv (CurvRBez *rbez1, CurvRBez *rbez2, Memspc *memSeg1,
                        CurvRBez *rbez, double t) {
/*====================
UT3D_rbez_subdiv     subdivide rational Bezier curve

UT3D_rbez_subdiv     Author: Thomas Backmeister       26.6.2003

Subdivide a rational Bezier curve into 2 rational Bezier curves.
The composition of the two resulting curves is equal to the original curve.
The underlying algorithm is "de Casteljau".

IN:
  Memspc *memSeg1 ... memory for controlpoints and weights
  CurvRBez *rbez  ... rational Bezier curve
  double t        ... parameter of subdivision point on the curve
                      (from interval (0,1))
OUT:
  CurvRBez *rbez1 ... rational Bezier curve 1
  CurvRBez *rbez2 ... rational Bezier curve 2
Returncodes:
  0 = OK
  1 = degree of rational Bezier curve too large
 -1 = out of tempSpace
 -2 = input error
*/

  int    irc, i1, i2, numpt;
  double s, sw, tw, wnew;
  double wtab[UT_BEZDEG_MAX+1];
  Point  ptab[UT_BEZDEG_MAX+1];


  // printf("UT3D_rbez_subdiv: t=%f\n",t);

  if (t <= 0.0 || t >= 1.0) {
    printf("UT3D_rbez_subdiv: input error\n");
    return -2;
  }
  if (rbez->ptNr > UT_BEZDEG_MAX + 1) {
    printf("UT3D_rbez_subdiv: degree of rational Bezier curve too large\n");
    return 1;
  }

  // init
  rbez1->ptNr = rbez->ptNr;
  rbez1->va = 0.0;
  rbez1->vb = 1.0;
  rbez2->ptNr = rbez->ptNr;
  rbez2->va = 0.0;
  rbez2->vb = 1.0;

  // start addresses of controlpoints -> memSeg1.
  rbez1->cptab = memSeg1->next;
  //rbez2->cptab = memSeg1->next + (sizeof(Point) * rbez->ptNr);
  rbez2->cptab = (Point*)((char*)memSeg1->next + (sizeof(Point) * rbez->ptNr));

  // provide memory for controlpoints
  irc = UME_add (memSeg1, sizeof(Point) * rbez->ptNr * 2);
  if(irc < 0) {
    TX_Error("UT3D_rbez_subdiv: out of tempSpace");
    return -1;
  }

  // start addresses of weights -> memSeg1.
  rbez1->wtab = memSeg1->next;
  // rbez2->wtab = memSeg1->next + (sizeof(double) * rbez->ptNr);
  rbez2->wtab=(double*)((char*)memSeg1->next + (sizeof(double) * rbez->ptNr));

  // provide memory for weights
  irc = UME_add (memSeg1, sizeof(double) * rbez->ptNr * 2);
  if(irc < 0) {
    TX_Error("UT3D_rbez_subdiv: out of tempSpace");
    return -1;
  }

  // save 1. / last point & weight
  rbez1->cptab[0] = rbez->cptab[0];
  rbez1->wtab[0] = rbez->wtab[0];
  rbez2->cptab[rbez->ptNr - 1] = rbez->cptab[rbez->ptNr - 1];
  rbez2->wtab[rbez->ptNr - 1] = rbez->wtab[rbez->ptNr - 1];

  // copy Bezier control points & weights
  for (i1=0; i1<rbez->ptNr; ++i1) {
    ptab[i1] = rbez->cptab[i1];
    wtab[i1] = rbez->wtab[i1];
  }

  // subdivision
  s = 1.0 - t;
  numpt = rbez->ptNr - 1;
  for (i1=0; i1<rbez->ptNr - 1; ++i1) {
    for (i2=0; i2<numpt; ++i2) {
      wnew = s * wtab[i2] + t * wtab[i2+1];
      if (UTP_compdb0 (wnew, UT_TOL_min1)) {
        ptab[i2].x = s * ptab[i2].x + t * ptab[i2+1].x;
        ptab[i2].y = s * ptab[i2].y + t * ptab[i2+1].y;
        ptab[i2].z = s * ptab[i2].z + t * ptab[i2+1].z;
      } else {
        sw = s * wtab[i2] / wnew;
        tw = t * wtab[i2+1] / wnew;
        ptab[i2].x = sw * ptab[i2].x + tw * ptab[i2+1].x;
        ptab[i2].y = sw * ptab[i2].y + tw * ptab[i2+1].y;
        ptab[i2].z = sw * ptab[i2].z + tw * ptab[i2+1].z;
      }
      wtab[i2] = wnew;
    }

    // save new points and weights
    rbez1->cptab[i1+1] = ptab[0];
    rbez1->wtab[i1+1] = wtab[0];
    rbez2->cptab[rbez->ptNr-i1-2] = ptab[numpt-1];
    rbez2->wtab[rbez->ptNr-i1-2] = wtab[numpt-1];

    --numpt;
  }

  return 0;
}


//=========================================================================
  int UT3D_rbez_rbsp (int *tabSiz, CurvRBez **cvarb,
                      Memspc *memSeg1, CurvRBSpl *rbsp) {
//=========================================================================
/// \code
/// UT3D_rbez_rbsp         rational-bezier-curve from rational-B-spline
///                        Backmeister, Reiter    2014-09-11
/// save all output-curves (rbTab) in memSeg1
/// see also UT3D_rbez_el UT3D_bez_bspl__
/// \endcode


// deg   degree of rbsp
// nrps   nr of ctrlPoints of rBspl
// nrpb   nr of ctrlPoints of rBez
// U   knotArray
// Pw  ctrlPoint + weight - array 
// nb  nr of rbez-curves (out)
// Qw[j][k] rbez-curves; j=index_curves, k=index_points.
// Degree of rBez: nrpb - 1


  int      a, m, nrps, deg, b, nb, i, mult, j, save, r, s, k, nrpb, nrCv;
  int      ii, i1, i2;
  double   *U, w, numer, alpha, *alphas;
  wPoint   *Pw;
  wPoint   *Qw;
  CurvRBez *rbTab;


  // DEB_dump_obj__ (Typ_CVRBSP, rbsp, "UT3D_rbez_rbsp");


  // get spc for wPoints
  i1 = rbsp->ptNr * sizeof(wPoint);
  Pw = (void*) MEM_alloc_tmp (i1);


  // transfer rbsp-curve from format point[3] + weight into format point[4]
  UT3D_wPTn_ptn_wn (Pw, rbsp->ptNr, rbsp->cpTab, rbsp->wTab);


  //================================================================
  deg = rbsp->deg;    // get deg = degree
  nrps = rbsp->ptNr;  // nr of ctrlpoints
  U = rbsp->kvTab;    // knot values


  m = nrps + deg + 1;     // max nr of bezCurves
  nrpb = deg + 1;         // nr of ctrlPts of the bezCurves


  a = deg;
  b = deg + 1;         // nr of ctrlPts of the bezCurves
  nb = 0;


  // get space for Qw
  // b = nr of ctrlPts of the bezCurves is (degree + 1)
  // max nr of bezCurves is ptNr + degree + 1
  i1 = m * b * sizeof(wPoint);
  Qw = (void*) MEM_alloc_tmp (i1);
    // printf(" size Qw = %d * %d * %d\n",m,b,sizeof(wPoint));


  i1 = nrpb * sizeof(double);
  alphas = (void*) MEM_alloc_tmp (i1);


  for (i=0; i<=deg; i++) {
    // Qw[nb][i] = Pw[i];
    ii = (nb * nrpb) + i;
    Qw[ii] = Pw[i];
  }

  while (b < m) {
    i = b;
    while (b < m && U[b+1] == U[b]) b++;
    mult = b - i + 1;
    if (mult < deg) {
      numer = U[b] - U[a]; // Numerator of alpha (ein lokales Arbeitsarray) 
      for (j=deg; j>mult; j--)
        alphas[j-mult-1] = numer/(U[a+j]-U[a]);
      r = deg - mult;        // Insert knot r times
      for (j=1; j<=r; j++) {
        save = r - j;
        s = mult + j;      // This many new points
        for (k=deg; k>=s; k--) {
          alpha = alphas[k-s];
          // Qw[nb][k] = alpha * Qw[nb][k] + (1.0 - alpha) * Qw[nb][k-1];
          ii = (nb * nrpb) + k;
          i1 = ii - 1;
            // printf(" nb=%d k=%d set Qw-%d = Qw-%d\n",nb,k,ii,i1);
          Qw[ii].x = alpha * Qw[ii].x + (1.0 - alpha) * Qw[i1].x;
          Qw[ii].y = alpha * Qw[ii].y + (1.0 - alpha) * Qw[i1].y;
          Qw[ii].z = alpha * Qw[ii].z + (1.0 - alpha) * Qw[i1].z;
          Qw[ii].w = alpha * Qw[ii].w + (1.0 - alpha) * Qw[i1].w;
        }
        if (b < m)     // Control point of
          // Qw[nb+1][save] = Qw[nb][deg];  // next segment
          ii = ((nb+1) * nrpb) + save;
          i1 = (nb * nrpb) + deg;
            // printf(" cp1 Qw-%d -> Qw-%d\n",i1,ii);
          Qw[ii] = Qw[i1];  // next segment
      }
      nb += 1;     // Bezier segment completed
      if (b < m) {
        for (i=deg-mult; i<=deg; i++) {
          // Qw[nb][i] = Pw[b-deg+i];
          ii = (nb * nrpb) + i;
            // printf(" cp Pw-%d -> Qw-%d\n",b-deg+i,ii);
          Qw[ii] = Pw[b-deg+i];
        }
        a = b;
        ++b;
      }
    }
  }


  //================================================================
  // retransfer rbsp-curve from format point[4] into point[3] + weight
  // nrCv = nr of bezCurves
  // nrpb = nr of pts per bezCurv
  nrCv = nb + 1;
    // printf(" nrCv=%d nrpb=%d\n",nrCv,nrpb);

  // get <nb+1> curves in memSeg1
  ii = sizeof(CurvRBez) * nrCv;
  rbTab = UME_reserve (memSeg1, ii);
  if(!rbTab) {TX_Error("UT3D_rbez_rbsp EOM-1"); return -1;}


  // loop tru curves
  for(i1=0; i1<nrCv; ++i1) {   // b=nr bezCurves
    ii = sizeof(Point) * nrpb;
    rbTab[i1].cptab = UME_reserve (memSeg1, ii);
    if(!rbTab[i1].cptab) {TX_Error("UT3D_rbez_rbsp EOM-2"); return -1;}
    ii = sizeof(double) * nrpb;
    rbTab[i1].wtab = UME_reserve (memSeg1, ii);
    if(!rbTab[i1].wtab) {TX_Error("UT3D_rbez_rbsp EOM-3"); return -1;}

    rbTab[i1].ptNr = nrpb;
    rbTab[i1].va = 0.;
    rbTab[i1].vb = 1.;

    // loop tru points of curve <i1>
    for (i2=0; i2<nrpb; ++i2) {   // nr of pts per bezCurv
      // w = Qw[i1][i2].w;
      ii = (i1 * nrpb) + i2;
      w = Qw[ii].w;
      if(fabs(w) < UT_TOL_min1) {
        rbTab[i1].cptab[i2].x = UT_VAL_MAX;
        rbTab[i1].cptab[i2].y = UT_VAL_MAX;
        rbTab[i1].cptab[i2].z = UT_VAL_MAX;
      } else {
        rbTab[i1].cptab[i2].x = Qw[ii].x / w;
        rbTab[i1].cptab[i2].y = Qw[ii].y / w;
        rbTab[i1].cptab[i2].z = Qw[ii].z / w;
      }
      rbTab[i1].wtab[i2] = w;
        // DEB_dump_obj__ (Typ_PT, &rbTab[i1].cptab[i2], "c%d p%d",i1,i2);
        // printf(" w=%lf\n",rbTab[i1].wtab[i2]);
    }
  }

  *tabSiz = nrCv;
  *cvarb = rbTab;
  

  //================================================================
  // TEST ONLY:
/*
  for(i1=0; i1<nrCv; ++i1) {
    CurvRBSpl crbsp;
    DEB_dump_obj__ (Typ_CVRBEZ, &rbTab[i1], " rbez[%d]",i1);
    if(i1 != 3) continue;
    // make rbspl from rbez
    UT3D_rbsp_rbez (&crbsp, &rbTab[i1], memSeg1);
    GR_Disp_rbspl (&crbsp, 9);
  }
*/

  //================================================================
  return 0;

}


/*=======================================================================*/
  int UT3D_rbez_el (CurvRBez rbTab[], Memspc *memSeg1, CurvElli *el) {
/*================
UT3D_rbez_el           rational quadratic Bezier curves <-- 3D-ellipse

UT3D_rbez_el           Author: Thomas Backmeister       26.6.2003

Conversion of a 3D-ellipse into 4 rational quadratic Bezier curves.
The Bezier curves have POSITIVE weights.
The i-th curve corresponds to the ellipse arc in the i-th quarter
of the ellipse 2D-coordinate system.

IN:
  Memspc *memSeg1  ... memory for controlpoints and weights
  CurvElli *el      ... 3D-ellipse
OUT:
  CurvRBez rbTab[] ... 4 rational Bezier curves
Returncodes:
  0 = OK
 -1 = out of tempSpace
*/

  int     irc, i1, i2;
  double  a, b;
  Mat_4x3 ma;



  // printf("UT3D_rbez_el\n");



  // init
  for (i1=0; i1<4; ++i1) {
    rbTab[i1].ptNr = 3;
    rbTab[i1].va = 0.0;
    rbTab[i1].vb = 1.0;
  }

  // start addresses of controlpoints -> memSeg1.
  for (i1=0; i1<4; ++i1) {
    // rbTab[i1].cptab = memSeg1->next + i1 * sizeof(Point) * 3;
    rbTab[i1].cptab = (Point*)((char*)memSeg1->next + i1 * sizeof(Point) * 3);
  }

  // provide memory for controlpoints
  irc = UME_add (memSeg1, sizeof(Point) * 12);
  if(irc < 0) {
    TX_Error("UT3D_rbez_el: out of tempSpace");
    return -1;
  }

  // start addresses of weights -> memSeg1.
  for (i1=0; i1<4; ++i1) {
    // rbTab[i1].wtab = memSeg1->next + i1 * sizeof(double) * 3;
    rbTab[i1].wtab = (double*)((char*)memSeg1->next + i1 * sizeof(double) * 3);
  }


  // provide memory for weights
  irc = UME_add (memSeg1, sizeof(double) * 12);
  if(irc < 0) {
    TX_Error("UT3D_rbez_el: out of tempSpace");
    return -1;
  }


  // length of ellipse axes
  a = UT3D_len_vc (&(el->va));
  b = UT3D_len_vc (&(el->vb));
  // printf(" a=%f b=%f\n",a,b);



  // controlpoints and weights of rational Bezier curves
  for (i1=0; i1<4; ++i1) {

    if (i1 == 0) {
      UT3D_pt_3db (&(rbTab[i1].cptab[0]), a, 0.0, 0.0);
      UT3D_pt_3db (&(rbTab[i1].cptab[1]), a, b, 0.0);
      UT3D_pt_3db (&(rbTab[i1].cptab[2]), 0.0, b, 0.0);

    } else if (i1 == 1) {
      UT3D_pt_3db (&(rbTab[i1].cptab[0]), -a, 0.0, 0.0);
      UT3D_pt_3db (&(rbTab[i1].cptab[1]), -a, b, 0.0);
      UT3D_pt_3db (&(rbTab[i1].cptab[2]), 0.0, b, 0.0);

    } else if (i1 == 2) {
      UT3D_pt_3db (&(rbTab[i1].cptab[0]), -a, 0.0, 0.0);
      UT3D_pt_3db (&(rbTab[i1].cptab[1]), -a, -b, 0.0);
      UT3D_pt_3db (&(rbTab[i1].cptab[2]), 0.0, -b, 0.0);

    } else if (i1 == 3) {
      UT3D_pt_3db (&(rbTab[i1].cptab[0]), a, 0.0, 0.0);
      UT3D_pt_3db (&(rbTab[i1].cptab[1]), a, -b, 0.0);
      UT3D_pt_3db (&(rbTab[i1].cptab[2]), 0.0, -b, 0.0);
    }
    rbTab[i1].wtab[0] = b * b;
    rbTab[i1].wtab[1] = rbTab[i1].wtab[0];
    rbTab[i1].wtab[2] = 2.0 * rbTab[i1].wtab[0];
  }

  // coordinate system of the ellipse
  // UT3D_m3_load_povxvy (ma, &(el->pc), &(el->va), &(el->vb));
  UT3D_m3_load (ma, &(el->va), &(el->vb), &(el->vz));
  UT3D_m3_load_o (ma, &(el->pc));

  // transform control points
  for (i1=0; i1<4; ++i1) {
    for (i2=0; i2<3; ++i2) {
      UT3D_pt_tra_pt_m3 (&(rbTab[i1].cptab[i2]), ma, &(rbTab[i1].cptab[i2]));
    }
  }

  return 0;
}



/*
//================================================================
  int UT3D_rbez_ci (CurvRBez rbTab[], Memspc *memSeg1, Circ *ci) {
//================================================================
// UT3D_rbez_ci           rational quadratic Bezier curves <-- 3D-circle
// UT3D_rbez_ci           Author: Thomas Backmeister       27.6.2003
// Conversion of a 3D-circle into 4 rational quadratic Bezier curves.
// The Bezier curves have POSITIVE weights.
// Each curve corresponds to one quarter of the circle.
// IN:
//   Memspc *memSeg1  ... memory for controlpoints and weights
//   Circ *ci         ... 3D-circle
// OUT:
//   CurvRBez rbTab[] ... 4 rational Bezier curves
// Returncodes:
//   0 = OK
//  -1 = out of tempSpace


  int     irc, i1, i2;
  double  r;
  Vector  vx;
  Mat_4x3 ma;




  // printf("UT3D_rbez_ci\n");
  // UME_dump(memSeg1, "      memSeg1:");


  // init
  for (i1=0; i1<4; ++i1) {
    rbTab[i1].ptNr = 3;
    rbTab[i1].va = 0.0;
    rbTab[i1].vb = 1.0;
  }

  // start addresses of controlpoints -> memSeg1.
  for (i1=0; i1<4; ++i1) {
    // rbTab[i1].cptab = memSeg1->next + i1 * sizeof(Point) * 3;
    rbTab[i1].cptab = (Point*)((char*)memSeg1->next + i1 * sizeof(Point) * 3);
  }

  // provide memory for controlpoints
  irc = UME_add (memSeg1, sizeof(Point) * 12);
  if(irc < 0) {
    TX_Error("UT3D_rbez_ci: out of tempSpace");
    return -1;
  }

  // start addresses of weights -> memSeg1.
  for (i1=0; i1<4; ++i1) {
    // rbTab[i1].wtab = memSeg1->next + i1 * sizeof(double) * 3;
    rbTab[i1].wtab = (double*)((char*)memSeg1->next + i1 * sizeof(double) * 3);
  }

  // provide memory for weights
  irc = UME_add (memSeg1, sizeof(double) * 12);
  if(irc < 0) {
    TX_Error("UT3D_rbez_ci: out of tempSpace");
    return -1;
  }


  // controlpoints and weights of rational Bezier curves
  r = fabs(ci->rad);
  // printf("r = %f\n",r);


  for (i1=0; i1<4; ++i1) {
    if (i1 == 0) {
      UT3D_pt_3db (&(rbTab[i1].cptab[0]), r, 0.0, 0.0);
      UT3D_pt_3db (&(rbTab[i1].cptab[1]), r, r, 0.0);
      UT3D_pt_3db (&(rbTab[i1].cptab[2]), 0.0, r, 0.0);
    } else if (i1 == 1) {
      UT3D_pt_3db (&(rbTab[i1].cptab[0]), -r, 0.0, 0.0);
      UT3D_pt_3db (&(rbTab[i1].cptab[1]), -r, r, 0.0);
      UT3D_pt_3db (&(rbTab[i1].cptab[2]), 0.0, r, 0.0);
    } else if (i1 == 2) {
      UT3D_pt_3db (&(rbTab[i1].cptab[0]), -r, 0.0, 0.0);
      UT3D_pt_3db (&(rbTab[i1].cptab[1]), -r, -r, 0.0);
      UT3D_pt_3db (&(rbTab[i1].cptab[2]), 0.0, -r, 0.0);
    } else if (i1 == 3) {
      UT3D_pt_3db (&(rbTab[i1].cptab[0]), r, 0.0, 0.0);
      UT3D_pt_3db (&(rbTab[i1].cptab[1]), r, -r, 0.0);
      UT3D_pt_3db (&(rbTab[i1].cptab[2]), 0.0, -r, 0.0);
    }
    rbTab[i1].wtab[0] = r * r;
    rbTab[i1].wtab[1] = rbTab[i1].wtab[0];
    rbTab[i1].wtab[2] = 2.0 * rbTab[i1].wtab[0];
  }


  // coordinate system of the circle
  UT3D_vc_perp1vc (&vx, &(ci->vz));
  UT3D_m3_load_povxvz (ma, &(ci->pc), &vx, &(ci->vz));


  // transform control points
  for (i1=0; i1<4; ++i1) {
    for (i2=0; i2<3; ++i2) {
      UT3D_pt_tra_pt_m3 (&(rbTab[i1].cptab[i2]), ma, &(rbTab[i1].cptab[i2]));
    }
  }

  return 0;
}
*/


//==============================================================================
  int UT3D_rbez_ci_180 (int *nrCv, CurvRBez rba[], Memspc *memSeg1, Circ *ci1) {
//==============================================================================
/// \code


  Vector   vcx, vcy;


  // DEB_dump_obj__ (Typ_CI, ci1, "UT3D_rbez_ci_180");


  if(fabs(ci1->ango) > RAD_270) *nrCv = 2;
  else *nrCv = 1;


  // vcx = vector pc -> p1
  UT3D_vc_2pt (&vcx, &ci1->pc, &ci1->p1);
    // DEB_dump_obj__ (Typ_VC, &vcx, "vcx");

  // vcy = normal to vcx and vz
  UT3D_vc_perp2vc (&vcy, &ci1->vz, &vcx);
    // DEB_dump_obj__ (Typ_VC, &vcy, "vcy");


  return UT3D_rbez_180 (nrCv, rba, memSeg1, &ci1->pc, &vcx, &vcy);

}


//==============================================================================
  int UT3D_rbez_el_180 (int *nrCv, CurvRBez rba[], Memspc *memSeg1,
                        CurvElli *el1) {
//==============================================================================
/// \code


  Vector   vcx, vcy;


  // DEB_dump_obj__ (Typ_CVELL, el1, "UT3D_rbez_el_180");


  // vcx = vector pc -> p1
  vcx = el1->va;
    // DEB_dump_obj__ (Typ_VC, &vcx, "vcx");

  // vcy = normal to vcx and vz
  vcy = el1->vb;
    // DEB_dump_obj__ (Typ_VC, &vcy, "vcy");


  // if p2 == p1 then 180-deg else 360-deg
  if(UT3D_comp2pt(&el1->p1, &el1->p2, UT_TOL_pt)) *nrCv = 2;
  else *nrCv = 1;
    // printf(" *nrCv = %d\n",*nrCv);


  return UT3D_rbez_180 (nrCv, rba, memSeg1, &el1->pc, &vcx, &vcy);

}


//==============================================================================
  int UT3D_rbez_180 (int *nrCv, CurvRBez rba[], Memspc *memSeg1,
                        Point *ptc, Vector *vcx, Vector *vcy) {
//==============================================================================
/// \code
/// UT3D_rbez_180         rational-bezier-curve for 180- or 360-deg-curve
/// Circle or ellipse.
///  
/// IN:
///   Memspc *memSeg1  ... receives controlpoints and weights
///   Circ *ci         ... 3D-circle
/// OUT: 
///   nrCv             ... nr of curves out; 1 or 2.
///   CurvRBez rba     ... rational Bezier curves, each having a 180 deg. circ.
/// \endcode
    
    
  int      iCv;
  Point    pt0, pt1, pt2, pt3;
  CurvRBez *rb1;
    

  // DEB_dump_obj__ (Typ_CI, ci1, "UT3D_rbez_ci_180");


  iCv = 0;


  //----------------------------------------------------------------
  L_nxt_cv:
  rb1 = &rba[iCv];

  // provide memory for controlpoints
  rb1->cptab = UME_reserve (memSeg1, sizeof(Point) * 4);
  if(!rb1->cptab) goto L_err1;

  // provide memory for weights
  rb1->wtab = UME_reserve (memSeg1, sizeof(double) * 4);
  if(!rb1->wtab) goto L_err1;


  // pt0 = ptc + vcx;
  UT3D_pt_traptvc (&pt0, ptc, vcx);

  // pt3 = ptc - vcx;
  UT3D_pt_traptmultvc (&pt3, ptc, vcx, -1.);

  // pt1 = pt0 + vcy * 2
  UT3D_pt_traptmultvc (&pt1, &pt0, vcy, 2.);
    // GR_Disp_pt (&pt1, SYM_STAR_S, ATT_COL_RED);

  // pt2 = pt3 + vcy * 2
  UT3D_pt_traptmultvc (&pt2, &pt3, vcy, 2.);
    // GR_Disp_pt (&pt2, SYM_STAR_S, ATT_COL_RED);


  rb1->ptNr = 4;
  rb1->va = 0;
  rb1->vb = 1.;

  rb1->cptab[0] = pt0;
  rb1->cptab[1] = pt1;
  rb1->cptab[2] = pt2;
  rb1->cptab[3] = pt3;

  rb1->wtab[0] = 1.;
  rb1->wtab[1] = 1. / 3.;
  rb1->wtab[2] = 1. / 3.;
  rb1->wtab[3] = 1.;


  //----------------------------------------------------------------
  if(*nrCv > 1) {
    ++iCv;
    if(iCv < *nrCv) { 
      MEM_swap__ (&pt0, &pt3, sizeof(Point));
      UT3D_vc_invert (vcx, vcx);
      UT3D_vc_invert (vcy, vcy);
      goto L_nxt_cv;
    }
  }


  return 0;

  L_err1:
    TX_Error("UT3D_rbez_ci_180: out of memspace");
    return -1;

}


//==================================================================
  int UT3D_rbez_ci_360 (CurvRBez *rb1, Memspc *memSeg1, Circ *ci1) {
//==================================================================


  Vector   vcx, vcy;


  // vcx = vector pc -> p1
  UT3D_vc_2pt (&vcx, &ci1->pc, &ci1->p1);
    // DEB_dump_obj__ (Typ_VC, &vcx, "vcx");

  // vcy = normal to vcx and vz
  UT3D_vc_perp2vc (&vcy, &ci1->vz, &vcx);
    // DEB_dump_obj__ (Typ_VC, &vcy, "vcy");

  return UT3D_rbez_360 (rb1, memSeg1, &ci1->pc, &vcx, &vcy);

}


//======================================================================
  int UT3D_rbez_el_360 (CurvRBez *rb1, Memspc *memSeg1, CurvElli *el1) {
//======================================================================

  Vector   vcx, vcy;


  vcx = el1->va;
    // DEB_dump_obj__ (Typ_VC, &vcx, "vcx");

  vcy = el1->vb;
    // DEB_dump_obj__ (Typ_VC, &vcy, "vcy");


  return UT3D_rbez_360 (rb1, memSeg1, &el1->pc, &vcx, &vcy);

}


//==================================================================
  int UT3D_rbez_360 (CurvRBez *rb1, Memspc *memSeg1,
                     Point *ptc, Vector *vcx, Vector *vcy) {
//==================================================================
/// \code
///  
/// UT3D_rbez_ci_360         rational Bezier curves for 360-deg-circ
///  
/// IN:
///   Memspc *memSeg1  ... receives controlpoints and weights
///   Circ *ci         ... 3D-circle
/// OUT: 
///   CurvRBez *rb1    ... 1 rational Bezier curve with 6 points


  Point    pt0, pt1, pt2, pt3, pt4;



  // provide memory for ncp controlpoints
  rb1->cptab = UME_reserve (memSeg1, sizeof(Point) * 6);
  if(!rb1->cptab) goto L_err1;

  // provide memory for ncp weights
  rb1->wtab = UME_reserve (memSeg1, sizeof(double) * 6);
  if(!rb1->wtab) goto L_err1;

  // pt0 = ptc + vcx;
  UT3D_pt_traptmultvc (&pt0, ptc, vcx, 1.);

  // pt1 = ptc + vcx + (4 * vcy)
  UT3D_pt_traptvc (&pt1, ptc, vcx);
  UT3D_pt_traptmultvc (&pt1, &pt1, vcy, 4.);

  // pt2 = ptc + (-3 * vcx) + (2 * vcy)
  UT3D_pt_traptmultvc (&pt2, ptc, vcx, -3.);
  UT3D_pt_traptmultvc (&pt2, &pt2, vcy, 2.);

  // pt3 = mirr. pt2
  // pt3 = ptc + (-3 * vcx) + (-2 * vcy)
  UT3D_pt_traptmultvc (&pt3, ptc, vcx, -3.);
  UT3D_pt_traptmultvc (&pt3, &pt3, vcy, -2.);

  // pt4 = mirr. pt1
  // pt1 = ptc + vcx + (-4 * vcy)
  UT3D_pt_traptvc (&pt4, ptc, vcx);
  UT3D_pt_traptmultvc (&pt4, &pt4, vcy, -4.);


  // UT3D_pt_3db (&pt0, 100., 0., 0.);
  // UT3D_pt_3db (&pt1, 100., 400., 0.);
  // UT3D_pt_3db (&pt2, -300., 200., 0.);
  // UT3D_pt_3db (&pt3, -300., -200., 0.);
  // UT3D_pt_3db (&pt4, 100., -400., 0.);


  rb1->ptNr = 6;
  rb1->va = 0.;
  rb1->vb = 1.;

  rb1->cptab[0] = pt0;
  rb1->cptab[1] = pt1;
  rb1->cptab[2] = pt2;
  rb1->cptab[3] = pt3;
  rb1->cptab[4] = pt4;
  rb1->cptab[5] = pt0;

  rb1->wtab[0] = 5.;
  rb1->wtab[1] = 1.;
  rb1->wtab[2] = 1.;
  rb1->wtab[3] = 1.;
  rb1->wtab[4] = 1.;
  rb1->wtab[5] = 5.;

    // DEB_dump_obj__ (Typ_CVRBEZ, rb1, "UT3D_rbez_360");
    // { long dli = -1L; //GL_Draw_rbez (&dli, 1, rb1); }
      // GR_Disp_rbez (rb1, 1); }

  return 0;

  L_err1:
    TX_Error("UT3D_rbez_360: out of memspace");
    return -1;

}


//============================================================================
  int UT3D_rbez_ci_0 (int *nrCv, CurvRBez rba[], Memspc *memSeg1, Circ *ci1) {
//============================================================================
/// \code
/// UT3D_rbez_ci_0         rational-bezier-curve for circle (0-120 deg max)
///  
/// IN:
///   Memspc *memSeg1  ... receives controlpoints and weights
///   Circ *ci         ... 3D-circle
/// OUT: 
///   nrCv             ... number of ouput-curves
///   CurvRBez rb1     ... quadratic rational-bezier-curves
/// \endcode

// range degrees - nr of curves
// 0-120   deg     1 curve;
// 120-240 deg     2 curves, both with equal length
// 240-360 deg     3 curves, all with equal length
    
    
  int      iCv;
  Point    pts[3], pte[3], ptm, pt1;
  Vector   vc1, vcm;
  double   dp, dq, ang1, d1;
  CurvRBez *rb1;


  // printf("000000000000000000000000000000000000000 \n");
  // DEB_dump_obj__ (Typ_CI, ci1, "UT3D_rbez_ci_0");


  // get *nrCv = nr of curves
  *nrCv = 1 + fabs(ci1->ango) / RAD_120;

  // angle per curve
  ang1 = ci1->ango / *nrCv;
    // printf(" nrCv=%d ang1=%lf\n",*nrCv,ang1);


  pts[0] = ci1->p1; // startpoint first curve

  if(*nrCv == 1) {
    pte[0] = ci1->p2; // endpoint
    goto L_i1;
  }


  // vc1 = ptc - pts
  UT3D_vc_2pt (&vc1, &ci1->pc, &pts[0]);

  // rotate vc1 ang1 around Z
  UT3D_vc_rotvcvcangr (&vc1, &ci1->vz, &vc1, ang1);
  // pte = ptc + vc1
  UT3D_pt_traptvc (&pte[0], &ci1->pc, &vc1);

  if(*nrCv == 2) {
    // 2. curve
    pts[1] = pte[0];
    pte[1] = ci1->p2;
    goto L_i1;
  }


  pts[1] = pte[0];

  // rotate vc1 ang1 around Z
  UT3D_vc_rotvcvcangr (&vc1, &ci1->vz, &vc1, ang1);
  // pte = ptc + vc1
  UT3D_pt_traptvc (&pte[1], &ci1->pc, &vc1);


  // 3. curve
  pts[2] = pte[1];
  pte[2] = ci1->p2;



  //----------------------------------------------------------------
  L_i1:
  iCv = 0;


  //----------------------------------------------------------------
  L_nxt_cv:
  rb1 = &rba[iCv];

 
  // provide memory for ncp controlpoints
  rb1->cptab = UME_reserve (memSeg1, sizeof(Point) * 3);
  if(!rb1->cptab) goto L_err1;

  // provide memory for ncp weights
  rb1->wtab = UME_reserve (memSeg1, sizeof(double) * 3);
  if(!rb1->wtab) goto L_err1;


  // ptm = midpoint p1,p2
  UT3D_pt_mid2pt (&ptm, &pts[iCv], &pte[iCv]);

  // vcm = pc - ptm
  UT3D_vc_2pt (&vcm, &ci1->pc, &ptm);

  if(iCv < 1) {
    dq = UT3D_len_vc (&vcm);
    // get p from radius and length of vc1
    UT2D_solvtriri_p_b_q (&dp, fabs(ci1->rad), dq);

    // d1 = total-length-factor
    d1 = 1. + (dp / dq);
      // printf(" dq=%lf dp=%lf d1=%lf\n",dq,dp,d1);
  }

  // pt1 - set correct length (tangential point)
  UT3D_pt_traptmultvc (&pt1, &ci1->pc, &vcm, d1);
    // GR_Disp_pt (&pt1, SYM_STAR_S, ATT_COL_RED);


  rb1->ptNr = 3;
  rb1->va = 0.;
  rb1->vb = 1.;

  rb1->cptab[0] = pts[iCv];
  rb1->cptab[1] = pt1;
  rb1->cptab[2] = pte[iCv];

  rb1->wtab[0] = 1.;
  rb1->wtab[1] = 1.;
  rb1->wtab[2] = d1;


  ++iCv;
  if(iCv < *nrCv) goto L_nxt_cv;


  return 0;

  L_err1:
    TX_Error("UT3D_rbez_ci_1: out of memspace");
    return -1;

}


//============================================================================
  int UT3D_rbez_ci__ (int *nrCv, CurvRBez rba[], Memspc *memSeg1, Circ *ci) {
//============================================================================
/// \code
/// 
/// UT3D_rbez_ci__         rational Bezier curves <-- 3D-circle
/// 
/// Conversion of a 3D-circle into rational Bezier curves.
/// The Bezier curves have POSITIVE weights.
/// 
/// IN:
///   Memspc *memSeg1  ... receives controlpoints and weights
///   Circ *ci         ... 3D-circle
/// OUT:
///   CurvRBez rbTab[] ... 1-3 rational Bezier curves
/// Returncodes:
///   0 = OK
///  -1 = out of tempSpace
///
/// \endcode


  int    i1, irc;


  // printf("===================================== \n");
  // printf("UT3D_rbez_ci__\n");
  // DEB_dump_obj__ (Typ_CI, ci, "  ci");
  // UME_dump(memSeg1, "      memSeg1:");


  // test 180 or 360 deg
  if((!UT3D_ck_ci360(ci)) || (!UT3D_ck_ci180(ci))) {
    irc = UT3D_rbez_ci_180 (nrCv, rba, memSeg1, ci);

  } else {
    // create 1-3 curves
    irc = UT3D_rbez_ci_0 (nrCv, rba, memSeg1, ci);
  }

  if(irc < 0) return irc;

/*
  //----------------------------------------------------------------
  // TEST ONLY:
  for(i1=0; i1 < *nrCv; ++i1) {
    long dli = -1L;
    // DEB_dump_obj__ (Typ_CVRBEZ, &rba[i1], "ex rbez_ci");
    // GL_Draw_rbez (&dli, 9, &rba[i1]);
    GR_Disp_rbez (&rba[i1], 9);
  }
  //----------------------------------------------------------------
*/

  return 0;

}


//===================== EOF ===========================
