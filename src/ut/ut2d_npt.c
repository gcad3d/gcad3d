// ../ut/ut2d_npt.c                  RF                     2018-12-20
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
2018-12-20 Extracted from ../ut/ut_geo2d.c. RF.

-----------------------------------------------------
*/
#ifdef globTag
void UT2D_npt(){}
#endif
/*!
\file  ../ut/ut2d_npt.c
\brief polygon (list of points) from curves, surfaces ... 
\code
=====================================================
List_functions_start:

UT2D_npt_npt3              tab of 2D-Points = tab of 3D-Points (copy X,Y)

UT2D_cv_ln                Linearstueck -> Polygon / Anzahl
UT2D_npt_obj              polygon from bin-obj (typ,struct)
UT2D_npt_otb              polygon from binary-object-table
UT2D_npt_sym_ci           get symbolic polygon from 2D-circ
UT2D_npt_plg              polygon (table of 2D-points) from polygonCurve (CurvPol2)
UT2D_npt_tra_npt3_rsys    transf. 3D-points => 2D-points

UT2D_cv_ci                change circle > polygon (schnell)         DO NOT USE
UT2D_cv_ci_               get circular polygon from circle
UT2D_npt_ci               circular polygon
UT2D_cv_cin               Circ -> Polygon / Anzahl
UT2D_cv_ci360             Vollkreis --> Polygon
UT2D_cv_inv               Punktetabelle umdrehen

UT2D_cv3_linear           delete unnecesary straight points
UT2D_is_convex_inpt       check if indexed contour is convex | concave

List_functions_end:
=====================================================
see also:

\endcode *//*----------------------------------------



==============================================================
*/




#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_geo.h"



//================================================================
  int UT2D_npt_plg (int *ptNr, Point2 *pTab, CurvPol2 *plg) {
//================================================================
// UT2D_npt_plg          polygon (table of 2D-points) from polygonCurve (CurvPol2)
// see also UT3D_pta_plg


  int   bNr;


  *ptNr =  plg->ptNr;


  bNr = sizeof(Point2) *  plg->ptNr;
  memcpy (pTab, &plg->pTab, bNr);
  // UT2D_npt_npt3 (pTab, &plg->cpTab, plg->ptNr);

  return 0;

}


//=======================================================================
  int UT2D_npt_sym_ci (int *ptNr, Point2 *pta, Circ2 *ci1, double *tol) {
//=======================================================================
// get symbolic polygon from 2D-circ
// only for detecting sense-of-rotation
// Input:
//   ptNr     nr of already used points in pta
//   pta      size >= 5 free points 
// Output:
//   pta
//   retCod   0 OK
//            1 arc too short  (< tol)
// see also UT2D_cv_ci UT2D_npt_ci

  int      irc = 0, sr;
  double   rdc;
  Point2   ptMid;
  Vector2  vc1, vc2;


  // DEB_dump_obj__ (Typ_CI2, ci1, " UT2D_npt_sym_ci");
  // GR_Disp_ci2 (ci1, Typ_Att_blue);

  rdc = fabs(ci1->rad);
  sr  = DLIM01(ci1->rad);                            // printf(" sr=%d\n",sr);

  // add startpoint
  pta[*ptNr] = ci1->p1;
  *ptNr += 1;


  //----------------------------------------------------------------
  // 0 - 135 deg: midpoint on circ
  if(fabs(ci1->ango) > RAD_135) goto L_135;

  // get midpoint p1-p2 on arc
  irc = UT2D_pt_mid_ci_2pt (&pta[*ptNr], &ci1->p1, &ci1->p2, &ci1->pc, &rdc, tol);
  if(irc) goto L_exit;
  *ptNr += 1;
  goto L_exit;



  //----------------------------------------------------------------
  L_135:  // 135 - 225 degree; add +90

    // get point rotated 90 deg from p1
    UT2D_pt_rot90_ci_pt (&pta[*ptNr], &ci1->pc, &ci1->p1, sr);
    *ptNr += 1;
    if(fabs(ci1->ango) < RAD_225) goto L_exit;


  //----------------------------------------------------------------
    // 315 - 360 degree; add +90, +180, -90
    if(fabs(ci1->ango) > RAD_315) {
      // +180
      UT2D_pt_rot180_ci_pt (&pta[*ptNr], &ci1->pc, &ci1->p1);
      *ptNr += 1;
    }


  //----------------------------------------------------------------
  L_225:  // 225 - 315 degree; add +90 and -90
    // get point rotated -90 deg from p2
    UT2D_pt_rot90_ci_pt (&pta[*ptNr], &ci1->pc, &ci1->p2, ICHG01(sr));
    *ptNr += 1;


  //----------------------------------------------------------------
  L_exit:
    // add exit-point
    pta[*ptNr] = ci1->p2;
    *ptNr += 1;

    // TESTBLOCK
    // printf("ex-npt_sym_ci ptNr=%d\n",*ptNr);
    // DEB_dump_nobj__ (Typ_PT2, *ptNr, pta, " npt_sym_ci");
    // GR_Disp_cv2 (pta, *ptNr, Typ_Att_hili1);
    // END TESTBLOCK


  return 0;

}


//=======================================================================
  int UT2D_npt_obj (int *ptNr, Point2* pTab, int ptSiz,
                    int typ, void *data, int siz, double *tol, int mode) {
//=======================================================================
/// \code
/// UT2D_npt_obj               Polygon from bin-obj (typ,struct)
/// Input:
///   ptNr         nr of output-points already in pTab (index of 1. unused point)
///   ptSiz        size of pTab
///   typ          type of struct in data
///   data         curve
///   siz          nr of objects of type 'typ' in 'data'
///   mode         0  perm, fix PRCV;
///                1  temp, do not use PRCV;
///                2  symbolic,
///                -1 unknown
/// Output:
///   ptNr         nr of output-points in pTab
///   pTab[ptNr]   polygon
/// RetCod:  0=OK; -1=Error; -2=pTab_overflow;
/// \endcode


  int       i1, irc, ptMax, ptn, cvTyp;
  void      *xu;
  void      *vp1;
  Point     *pa;
  ObjGX     *oa;



  // printf("UT2D_npt_obj typ=%d siz=%d ptNr=%d ptSiz=%d tol=%lf mode=%d\n",
         // typ, siz, *ptNr, ptSiz, *tol, mode);
  // DEB_dump_obj__ (typ, data, " __npt_obj");


  irc = 0;
  // if(ptSiz - *ptNr < 2) goto L_EOM;


  //----------------------------------------------------------------
  if(typ == Typ_LN2) {
    // Line -> Polygon
    if((*ptNr + 2) > ptSiz) goto L_EOM; // outTab zu klein
    // 
    pTab[*ptNr] = ((Line2*)data)->p1;
    *ptNr += 1;
    pTab[*ptNr] = ((Line2*)data)->p2;
    *ptNr += 1;
      // printf(" new-ptNr=%d\n",*ptNr);



  //----------------------------------------------------------------
  } else if(typ == Typ_CI2) {
    // CIRC -> Polygon
    if(mode == 2) {   // symbolic
      if(ptSiz - *ptNr < 5) goto L_EOM;
      // get symbolic polygon from 2D-circ
      irc = UT2D_npt_sym_ci (ptNr, pTab, data, tol);

    } else {
      // temp, do not use PRCV;
      if(ptSiz - *ptNr < 16) goto L_EOM;
      if(((Circ2*)data)->rad > 0.) {
        UT2D_cv_ci (&pTab[*ptNr], &ptn,
                    &((Circ2*)data)->p1,&((Circ2*)data)->p2,&((Circ2*)data)->pc,4);
      } else {
        // for CW: change points, invert pTab
        UT2D_cv_ci (&pTab[*ptNr], &ptn,
                    &((Circ2*)data)->p2,&((Circ2*)data)->p1,&((Circ2*)data)->pc,4);
        UT2D_cv_inv (ptn, &pTab[*ptNr]);
      }
      *ptNr += ptn;
    // UT2D_npt_ci (&pta[ptNr], i2, (Circ*)cv1);
    }


  //----------------------------------------------------------------
  } else {
    TX_Error("UT2D_npt_obj E001-typ %d unsupp",typ);
    return -1;
  }


    // TESTBLOCK
    // if(*ptNr > 16) {
     // DEB_dump_nobj__ (Typ_PT2, *ptNr, pTab, "UT2D_npt_obj-in");
     // ERR_raise (""); }
    // GR_Disp_cv2 (pTab, *ptNr, Typ_Att_hili);
    // for(i1=0;i1<*ptNr;++i1) DEB_dump_obj__ (Typ_PT2, &pTab[i1]," pta[%d]\n",i1);
    // ERR_raise ("");
    // END TESTBLOCK

  return irc;


  //----------------------------------------------------------------
  L_EOM:
    TX_Error("UT2D_npt_obj pTab overflow");
    return -2;

}


//================================================================
  int UT2D_npt_otb (int *ptNr, Point2 *pTab, int ptMax,
                    ObjTab *otb1, double *tol, int mode) {
//================================================================
// UT2D_npt_otb               polygon from binary-object-table
// Input:
//   ptNr    nr of points already defined; must be set to 0 for 1. obj


  int    irc, i1;


  // printf("UT2D_npt_otb oNr=%d ptMax=%d tol=%f mode=%d\n",
         // otb1->oNr,ptMax,*tol,mode);


  // make a polygon from otb1 and disp objects
  for(i1=0; i1<otb1->oNr; ++i1) {
      // printf(";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; \n");
      // printf(" otb1[%d] typ=%d\n",i1,otb1->oTyp[i1]);
      // DEB_dump_obj__ (otb1->oTyp[i1], otb1->oDat[i1], " _Draw_otb-otb1 %d",i1);

    // connected-objects: remove last endPt (== next startPt)
    if(i1) *ptNr -= 1;

    // add obj as polygon to pta
    irc = UT2D_npt_obj (ptNr, pTab, ptMax,
                        otb1->oTyp[i1], otb1->oDat[i1], 1, tol, 2);
      // printf(" ex-npt_obj irc=%d pnr=%d\n", irc, *ptNr);
      // DEB_dump_nobj__ (Typ_PT, *ptNr, pTab, "pTab");
    if(irc < 0) return -1;

  }

    // TESTBLOCK
    // GR_Disp_cv2 (pTab, *ptNr, Typ_Att_hili);
    // for(i1=0;i1<*ptNr;++i1) DEB_dump_obj__ (Typ_PT2, &pTab[i1]," pta[%d]\n",i1);
    // ERR_raise ("");
    // END TESTBLOCK


  return 0;

}


//==========================================================================
  int UT2D_npt_npt3 (Point2 *p2a, Point *pa, int pNr) {
//==========================================================================
/// \code
/// UT2D_npt_npt3              tab of 2D-Points = tab of 3D-Points (copy X,Y)
/// \endcode

// es sollte Normalvektor benutzt werden; dzt copy X- & Y-Coord only.

  int    i1;

  for(i1=0; i1<pNr; ++i1) p2a[i1] = UT2D_pt_pt3 (&pa[i1]);

  return 0;

}


//================================================================
  int UT2D_is_convex_inpt (int pNr, int *ia, Point2 *pa)  {
//================================================================
// check if indexed contour is convex | concave
// Output:
//   retCod:  -1  contour is convex ("O")
//           >=0  contour is concave ("L"); pointIndex of first concave segments
//                - contour is concave arount point [ia[ii]]
// TESTPROG: UT2D_test_is_convex_inpt
// see also UT3D_isConvx_ptTab UFA_opt_ckCvx UFA_opt_ckOpt

  int      ii, i1, ie;
  double   d1;
  Vector2  v01, v12, ve0;


  // printf("UT2D_is_convex_inpt %d\n",pNr);
  // for(i1=0;i1<pNr;++i1)printf(" %d",ia[i1]);
  // printf(" \n");




  ie = pNr - 1;
  // closing segment
  ve0.dx = pa[ia[ie]].x - pa[ia[0]].x;
  ve0.dy = pa[ia[ie]].y - pa[ia[0]].y;
  v01 = ve0;

  i1 = -1;
  for(ii=1; ii < pNr; ++ii) {
    ++i1;
    v12.dx = pa[ia[i1]].x - pa[ia[ii]].x;
    v12.dy = pa[ia[i1]].y - pa[ia[ii]].y;
    // crossprod of v01 - v12
    d1 = -v01.dy * v12.dx + v01.dx * v12.dy;
      // printf(" isConvex v12=%d,%d  %f\n",i1,ii,d1);
    if(d1 < 0.) return i1;  // concave arount point [ia[ii]]
    v01 = v12;
  }
  // test last segment - closing segment
      // printf(" isConvex v12=%d,%d  %f\n",i1,ii,d1);
  d1 = -v01.dy * ve0.dx + v01.dx * ve0.dy;
  if(d1 < 0.) return ie;


  return -1;

}


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
  ptStat = MEM_alloc_tmp (*pNr);
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
  // DEB_dump_obj__ (Typ_CI2, ci1, "");


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
      // DEB_dump_obj__ (Typ_PT, &cv[i1], " P[%d]=",i1);
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


  // DEB_dump_obj__ (Typ_PT2, ptCen, "UT2D_cv_ci360 %d %f",ptNr, rd);


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
/// UT2D_cv_ci                change circle > polygon. ONLY CCW.
/// max 10 vertices for 45 degrees; so a max of 65 vertices is possible.
/// The first and the last segment have different sizes.
///
/// nur CCW. Max 64 Segments. Sehr schnell.
/// Erstes und letztes Segment haben unterschiedliche Groesse.
///
/// the circle is divided into 64/idiv segments.
/// Input:
///   idiv    circle is divided into 64/idiv segments; 1 = max 64 segments,
///           2 = max 32 segments; 3 = max 24 segments; 4 = max 16 segments.
/// \endcode


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


//=============================================================================
  int UT2D_npt_tra_npt3_rsys (int pNr, Point2 *pa2, Point *pa3, Refsys *rSys) {
//=============================================================================
/// UT2D_npt_tra_npt3_rsys           transf. 3D-points => 2D-points

  int    i1;

  if(rSys->bpi >= 0) {
    // points on Backplane
    for(i1=0; i1<pNr; ++i1)
      UT2D_pt_tra_pt3_bp (&pa2[i1], &pa3[i1], rSys->bpi);


  } else {
    // points on free plane (4x3-matrix)
    for(i1=0; i1<pNr; ++i1)
      UT2D_pt_tra_pt3_m3 (&pa2[i1], rSys->mat2, &pa3[i1]);
  }


  return 0;

}



// EOF
