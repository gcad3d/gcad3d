//          B-Spline-Surface-Functions    RF.
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
 SEE ../ut/ut_bspl_utils.c  (unused functions !!!!)
 Noch nicht verwendet:
 UT3D_bspl_surcrvcrv aus xa_ut.c     b-spline surface <-- two b-spline curves

-----------------------------------------------------
Modifications:
2004-06-01 neu ex ../ut/ut_bspl.c. RF.

-----------------------------------------------------
*/
/*!
\file  ../ut/ut_sbsp.c
\brief B-Spline-Surface-Functions 
\code
=====================================================
List_functions_start:

UT3D_sbs_ck_planar        check if B-SplSurf = planar
UT3D_sbs_ck_cycX          check if B-SplSurf is closed in X (U)
UT3D_sbs_ck_closed        check if B-SplSurf is closed (soll cycY werden)
UT3D_uvNr_sbsp            optimale Anzahl von Punkten in U/V-Richtung ermitteln
SUSbsp_ck_tol             optimale Anzahl von Punkten in U/V-Richtung ermitteln
UT3D_obj_cnvt_sbsp        degrade B-SplSur --> PlanarSurf / RuledSurf / Torus

UT3D_sbsp_2ncv            create bspl-surf from 2 groups of curves
UT3D_sbsp_ncv             create bspl-surf from 2-n across-curves

UT3D_pt_evparsbsp         point <-- evaluate surface parameter
UT3D_pt_intlnsbs          intersect Line - BspSur
UT3D_ptgrid_sbsp          Grundraster generieren (f UT3D_parsbsp_pt ..)

UT3D_parsbsp_pt           u/v-Parameter from  3D-B-SplineSurfPoint
UT3D_uparmid_sbsp         mittleren U-Wert
UT3D_vparmid_sbsp         mittleren V-Wert

UT3D_ptvc_evparsbsp       Point/Normalvektor <-- evaluate surface parameter
UT3D_bspU_sbsp_v          create B-Spline in direction U with par. u
UT3D_bspV_sbsp_u          create B-Spline in direction V with par. v
UT3D_bsp_sbspU            get U-Basiscurve from B-SplSurf
UT3D_bsp_sbspV            get V-Basiscurve from B-SplSurf

UT3D_cv_sbspout           outline (polygon) of B-SplSurf
UT3D_cv_sbspU             Isocurve (polygon) from U-wert
UT3D_cv_sbspV             Isocurve (polygon) from V-wert

List_functions_end:
=====================================================
- test functions:
UT3D_testsurf
- see also:
../ut/ubssur.c    USBS_..
UT3D_sbs_ck_planar

\endcode *//*----------------------------------------



B-Spline-Surf   Typ_SURBSP

struct SurBSpl:
// ptUNr, ptVNr, degU, degV;
// cpTab[ptUNr*ptVNr]    u0v0,u1v0..uUv0;u1v0,u1v1..uUv1;..;uUv0,uUv1..uUvV;
// kvTabU[ptUNr+degU+1]
// kvTabV[ptVNr+degV+1]

Beispiel ptUNr=2 ptVNr=4:
 u0v0 u1v0;
 u0v1 u1v1;
 u0v2 u1v2;
 u0v3 u1v3;

 es gibt 2 U-Kurven a 4 Punkte; 
 es gibt 4 V-Kurven a 2 Punkte.


*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_geo.h"
#include "../ut/ut_bspl.h"
#include "../ut/ut_sbsp.h"

#include "../ut/func_types.h"            // SYM.

#include "../xa/xa_mem.h"                 // memspc51-55






// extern double UT_TOL_cv;    // die max erlaubte Abweichung von der
                               // Originalkurve





  // double bsp_dist1(Point *pts, Point *pt1, Point *pte);



//================================================================
  int UT3D_sbsp_ncv (SurBSpl *su1, Memspc *tbuf1,
               int cvNr, CurvBSpl **cvtab, int deg,
               Memspc *tbuf2, Memspc *tbuf3) {
//================================================================
// create bspl-surf from 2-n across-curves
// data-out in tbuf1
// tempWorkspace is tbuf2, tbuf3
// Input:
//   cvNr   nr of input-curves; 2-n
//   cvtab  <cvNr> bSpline-curves

  int       irc, dirUV, i1, i2;
  double    pvT;
  CurvBSpl  **cv1tab;


  // printf("UT3D_sbsp_ncv %d %d\n",cvNr,deg);

  if(cvNr < 2) return -1;

  // fix degree if not yet defined ..
  if(deg == 0) deg = 2;   // < IMAX(20,cvNr) !

  if(cvNr < 3) deg = 1;   // 2 curves = ruled


/*
  // check for SRU (2 curves with deg=1)
  if(cvNr == 2) {
    i1 = cvtab[0]->ptNr;
    i2 = cvtab[1]->ptNr;
    // make ruled surface with one point
    if((i1 == 1)||(i2 == 1)) {
        printf(" s-ruled %d %d\n",i1,i2);
      goto L_exit;
    }
  }
*/


  // make space for cv1tab = cvNr new curves in tbuf1
  cv1tab = tbuf1->next;
  UME_add (tbuf1, cvNr * sizeof(CurvBSpl));


  // make bspline curves compatible
  irc = UCBS_MkeCmpBspCrvs (cv1tab, tbuf1, cvNr, cvtab, 0, tbuf3);
  if (irc < 0) return -1;



  // skinned surface through compatible bspline curves
  dirUV = 1;
  // 0=pvT unused
  irc = USBS_SkinSurBspCrvs (su1, tbuf1, cvNr, cv1tab, deg,
                dirUV, &pvT, 0, tbuf3);


  L_exit:

    // DEB_dump_obj__ (Typ_SURBSP, su1, "ex UT3D_sbsp_ncv");

  return irc;

}


//================================================================
  int UT3D_sbsp_ck_net (int cv1Nr, CurvBSpl **cv1tab,
                        int cv2Nr, CurvBSpl **cv2tab) {
//================================================================
// check if endpoints of curves lie on surface-boundaries
// RetCod:
//   -99   curves do not end on surface - boundaries


  Point    *pu0, *pv0;
  CurvBSpl *cu0, *cu1, *cv0, *cv1;


  // printf("UT3D_sbsp_ck_net %d %d\n",cv1Nr,cv2Nr);


  cu0 = cv1tab[0];           // first U-curve
  cv0 = cv2tab[0];           // first V-curve

  cu1 = cv1tab[cv1Nr - 1];   // last U-curve
  cv1 = cv2tab[cv2Nr - 1];   // last V-curve


  // startpoint of first U-curve = startpoint of first V-curve
  pu0 = &cu0->cpTab[0];
  pv0 = &cv0->cpTab[0];
    // DEB_dump_obj__ (Typ_PT, pu0, "  suf");
    // DEB_dump_obj__ (Typ_PT, pv0, "  svf");
  if(!UT3D_comp2pt (pu0, pv0, UT_TOL_cv)) {
    goto L_err;
  }


  // endpoint of first U-curve = startpoint of last V-curve
  pu0 = &cu0->cpTab[cu0->ptNr - 1];
  pv0 = &cv1->cpTab[0];
    // DEB_dump_obj__ (Typ_PT, pu0, "  euf");
    // DEB_dump_obj__ (Typ_PT, pv0, "  svl");
  if(!UT3D_comp2pt (pu0, pv0, UT_TOL_cv)) {
    goto L_err;
  }


  // startpoint of last U-curve = endpoint of first V-curve
  pu0 = &cu1->cpTab[0];
  pv0 = &cv0->cpTab[cv0->ptNr - 1];
    // DEB_dump_obj__ (Typ_PT, pu0, "  sul");
    // DEB_dump_obj__ (Typ_PT, pv0, "  ev0");
  if(!UT3D_comp2pt (pu0, pv0, UT_TOL_cv)) {
    goto L_err;
  }


  // endpoint of last U-curve = endpoint of last V-curve
  pu0 = &cu1->cpTab[cu1->ptNr - 1];
  pv0 = &cv1->cpTab[cv1->ptNr - 1];
    // DEB_dump_obj__ (Typ_PT, pu0, "  eul");
    // DEB_dump_obj__ (Typ_PT, pv0, "  evl");
  if(!UT3D_comp2pt (pu0, pv0, UT_TOL_cv)) {
    goto L_err;
  }


  return 0;


  L_err:
    // curves do not end on surface - boundaries");
    TX_Print("*** position %f,%f %f - %f,%f,%f",
             pu0->x,pu0->y,pu0->z,
             pv0->x,pv0->y,pv0->z);
    return -99;

}


//================================================================
  int UT3D_sbsp_2ncv (SurBSpl *su1, Memspc *tbuf1,
                int cv1Nr, CurvBSpl **cv1tab, int deg1,
                int cv2Nr, CurvBSpl **cv2tab, int deg2,
                Memspc *tbuf2, Memspc *tbuf3) {
//================================================================
// create bspl-surf from 2 groups of curves
// RetCod:
//   -99        curves do not end on surface - boundaries

// die Zwischenkurven der Gruppe1 müssen nicht am Netz enden;
// - können länger sein ! Die äußeren Netzgrenzen müssen aber
// genau passen !


  int      irc, i1;
  CurvBSpl *cv3tab[2];
  Point    pt1;


  // printf("===================================================== \n");
  // printf("UT3D_sbsp_2ncv nr1=%d nr2=%d\n",cv1Nr,cv2Nr);
  // printf(" deg1=%d deg2=%d\n",deg1,deg2);
  // for(i1=0;i1<cv1Nr;++i1) DEB_dump_obj__(Typ_CVBSP,cv1tab[i1], "1-%d",i1);
  // for(i1=0;i1<cv2Nr;++i1) DEB_dump_obj__(Typ_CVBSP,cv2tab[i1], "2-%d",i1);


  // check for degenerated (1-curve only)
  if(cv1Nr > 1) goto L_ckdeg2;
  if(cv2Nr > 2) goto L_err_1;

  // 1 x 2 curves;
  // swap tables.
  MEM_swap_int (&cv1Nr, &cv2Nr);
  MEM_swap_int (&deg1, &deg2);
  MEM_swap_2vp (&cv1tab, &cv2tab);
  goto L_deg;


  L_ckdeg2:
  if(cv2Nr > 1) goto L_nxn;
  if(cv1Nr > 2) goto L_err_1;



  //----------------------------------------------------------------
  // Degenerated; 3 inputcurves; 2 in cv1tab, 1 in cv2tab.
  L_deg:
  return -1;
/*
  // fix orientation of cv1tab


  // get pt1 = the commonPoint;
  // is the common point at v0 or v1
  irc = UT3D_pt_evparCrvBSpl (&pt1, bsp1, bsp1->v1, tbuf3);
  if (irc < 0) return -1;

  // create cv3tab with 2 curves;
  cv3tab[0] = cv2tab[0];
  // make ? = a copy of curve cv2tab[0]
  irc = UCBS_CpyBspCrv (&cv3tab[1], tbuf1, &cv2tab[0]);
  // set all controlpoints to the commonPoint
  for(i1=0; i1<bspTV[1].ptNr; ++i1) bspTV[1].cpTab[i1] = pt1;

  cv2tab = cv3tab;



  // irc = UCBS_PrepCoons3BspCrvs (bspTU, bspTV, &mem2Seg, cvTab1);
  // if (irc < 0) return -1;



  cv1Nr = 2;
  cv2Nr = 2;
  deg1 = 1;
  deg2 = 1;
  goto L_create;
*/



  //----------------------------------------------------------------
  L_nxn:

  // fix degrees if not yet defined ..
  if(deg1 == 0) deg1 = 2;
  if(deg2 == 0) deg2 = 2;

  // kreuzweise !!
  if(cv1Nr < 3) deg2 = 1;
  if(cv2Nr < 3) deg1 = 1;



  // check if endpoints of curves lie on surface-boundaries
  irc = UT3D_sbsp_ck_net (cv1Nr, cv1tab, cv2Nr, cv2tab);
  if(irc < 0) return irc;  // curves do not end on surface - boundaries



  // birail-curves: rails must be group 2 !
  if((cv1Nr == 2)&&(cv2Nr > 2)) {
    // change groups
    MEM_swap_int (&cv1Nr, &cv2Nr);
    MEM_swap_int (&deg1, &deg2);
    MEM_swap_2vp (&cv1tab, &cv2tab);
  }



  // create surf
  L_create:
  irc = USBS_GordSurBspCrvNet (su1, tbuf1, cv1Nr, cv1tab, cv2Nr, cv2tab,
            deg1, deg2, tbuf2, tbuf3);
    // printf("ex USBS_GordSurBspCrvNet %d\n",irc);



  return irc;



  //----------------------------------------------------------------
  L_err_1:
    TX_Error("BsplSur degen. should have 2 x 1 curves");
    return -1;

}


//================================================================
  int UT3D_obj_cnvt_sbsp (ObjGX *ox1, SurBSpl *su1, Memspc *memSeg1) {
//================================================================
// SBS in ruled oder planar degradieren
// entweder ptUNr oder ptVNr muss 2 sein
// RetCod: 0  OK, bspl converted.
//        -1  No; cannot convert bspl.


  int      irc, i1;
  ObjGX    *oTab;
  Line     *ln1, *ln2;
  CurvBSpl *cv1, *cv2;
  double   d1;
  char     *cbuf;


  // printf("================================================\n");
  // printf("UT3D_obj_cnvt_sbsp ptUNr=%d ptVNr=%d\n",su1->ptUNr,su1->ptVNr);
  // DEB_dump_obj__ (Typ_SURBSP,  su1, "UT3D_obj_cnvt_sbsp\n");



  // Degree 1/1: Ruled surf od Polygon machen.
  if((su1->degU != 1)||(su1->degV != 1)) goto L_ck_circ;



  //================================================================
  // Degrees sind 1/1;  Testen ob planar.
  // add oTab[2] to memSpc
  oTab = (ObjGX*)memSeg1->next;
  irc = UME_add (memSeg1, sizeof(ObjGX)*2);
  if(irc < 0) return -1;


  ox1->typ  = Typ_SURRU;
  ox1->form = Typ_ObjGX;
  ox1->siz  = 2;
  ox1->data = (void*)oTab;
  ox1->dir  = 0;



  //================================================================
  if((su1->ptUNr != 2)||(su1->ptVNr != 2)) goto L_U2;

  // DEB_dump_obj__(Typ_SURBSP, su1, " BSP 2x2");

  // Planar ?          A20=S(POL ...)
  d1 = UT3D_sbs_ck_planar (su1);
  if(d1 < UT_DISP_cv) {
    ox1->typ  = Typ_SUR;
    ox1->form = Typ_Txt;
    ox1->data = memSeg1->next;
    cbuf      = memSeg1->next;   
    cbuf[0] = '\0';
    strcat(cbuf, "S(POL");
    AP_obj_add_pt (cbuf, &su1->cpTab[0]);
    AP_obj_add_pt (cbuf, &su1->cpTab[1]);
    AP_obj_add_pt (cbuf, &su1->cpTab[3]);
    AP_obj_add_pt (cbuf, &su1->cpTab[2]);
    AP_obj_add_pt (cbuf, &su1->cpTab[0]);
    strcat(cbuf, ")");
    i1 = strlen(cbuf);
    irc = UME_add (memSeg1, i1+1);
    if(irc < 0) return -1;
    ox1->siz  = i1 + 1;
    // printf(" A=|%s|\n",cbuf); // exit(0);
    goto L_fertig;
  }



  // add 2 Lines to memSpc
  ln1 = (Line*)memSeg1->next;
  irc = UME_add (memSeg1, sizeof(Line));
  if(irc < 0) return -1;
  ln2 = (Line*)memSeg1->next;
  irc = UME_add (memSeg1, sizeof(Line));
  if(irc < 0) return -1;


  // controlpoints (su1->cpTab[0-3]) als 2 Lines ausgeben
  ln1->p1 = su1->cpTab[0];
  ln1->p2 = su1->cpTab[1];
  ln2->p1 = su1->cpTab[2];
  ln2->p2 = su1->cpTab[3];

  oTab[0].typ  = Typ_LN;
  oTab[0].form = Typ_LN;
  oTab[0].siz  = 1;
  oTab[0].data = (void*)ln1;

  oTab[1].typ  = Typ_LN;
  oTab[1].form = Typ_LN;
  oTab[1].siz  = 1;
  oTab[1].data = (void*)ln2;

  // GR_Disp_ln (ln1, 2);
  // GR_Disp_ln (ln2, 2);
  // GR_Disp_sru (ox1, 5);

  goto L_fertig;




  //================================================================
  // Deg 1/1; aber ptNr != 2.
  L_U2:


  // SRU Bsp1 Bsp2
  // add 2 Bspl-Curves to memSpc
  cv1 = (CurvBSpl*)memSeg1->next;
  irc = UME_add (memSeg1, sizeof(CurvBSpl));
  if(irc < 0) return -1;
  cv2 = (CurvBSpl*)memSeg1->next;
  irc = UME_add (memSeg1, sizeof(CurvBSpl));
  if(irc < 0) return -1;




  //================================================================
  // if(su1->ptUNr != 2) goto L_V2;
  if(su1->degU != 1) goto L_V2;

  // copy Bsp-Curve out of Bsp-Surf
  UT3D_bsp_cpsbsp (cv1, su1, 'U', 0, memSeg1);

  // copy Bsp-Curve out of Bsp-Surf
  // UT3D_bsp_cpsbsp (cv2, su1, 'U', 1, memSeg1);
  UT3D_bsp_cpsbsp (cv2, su1, 'U', su1->ptUNr-1, memSeg1);

  goto L_out_2cv;




  //================================================================
  L_V2:
  // if(su1->ptVNr != 2) return 0;
  if(su1->degV != 1) return 0;

  // copy Bsp-Curve out of Bsp-Surf
  UT3D_bsp_cpsbsp (cv1, su1, 'V', 0, memSeg1);

  // copy Bsp-Curve out of Bsp-Surf
  // UT3D_bsp_cpsbsp (cv2, su1, 'V', 1, memSeg1);
  UT3D_bsp_cpsbsp (cv2, su1, 'V', su1->ptVNr-1, memSeg1);



  L_out_2cv:
  oTab[0].typ  = Typ_CVBSP;
  oTab[0].form = Typ_CVBSP;
  oTab[0].siz  = 1;
  oTab[0].data = (void*)cv1;

  oTab[1].typ  = Typ_CVBSP;
  oTab[1].form = Typ_CVBSP;
  oTab[1].siz  = 1;
  oTab[1].data = (void*)cv2;

  // DEB_dump_ox_0 (ox1, "ex UT3D_obj_cnvt_sbsp:");
  // GR_Disp_sru (ox1, 5);

  goto L_fertig;







  //================================================================
  L_ck_circ:     // not yet impl, see UT3D_obj_cnvt_srbsp
  return -1;     // cannot (yet) convert




  //================================================================
  L_fertig:
  return 0;

}


//=========================================================
  int UT3D_sbs_ck_closed (SurBSpl *su1) {
//=========================================================
// UT3D_sbs_ck_closed        check if B-SplSurf is closed 
// irc = 0 = sbs is NOT closed (normal)
// irc = 1 0 sbs is closed

  int     i1, irc;


    // feststellen ob geschlossen
    // letzter Punkt der ersten Kurve
    i1 = su1->ptUNr * (su1->ptVNr - 1);
      // DEB_dump_obj__ (Typ_PT,  &su1->cpTab[i1], " p[i1]");

    if(UT3D_comp2pt(&su1->cpTab[0],
                    &su1->cpTab[i1], UT_DISP_cv) == 1) {
      // printf(" closed ..\n");
      irc = 1; // closed

    }  else irc = 0;

  // printf("ex UT3D_sbs_ck_closed %d\n",irc);

  return irc;

}


//=========================================================
  int UT3D_sbs_ck_cycX (SurBSpl *su1) {
//=========================================================
// UT3D_sbs_ck_cycX        check if B-SplSurf is closed in X (U)
// irc = 0:  sbs is NOT closed (normal)
// irc = 1:  yes, sbs is closed

// see also UT3D_srbsp_ck_closed

  int     i1, i2, irc;


  irc = 0;

  // feststellen ob geschlossen
  // letzter Punkt der ersten Kurve
  i1 = 0;
  i2 = su1->ptUNr - 1;
    // DEB_dump_obj__ (Typ_PT,  &su1->cpTab[i1], " p[i1]");

  if(UT3D_comp2pt(&su1->cpTab[i1],
                  &su1->cpTab[i2], UT_DISP_cv) == 0) goto L_exit;



  i1 = su1->ptUNr * (su1->ptVNr - 1);
  i2 = (su1->ptUNr * su1->ptVNr) - 1;
    // DEB_dump_obj__ (Typ_PT,  &su1->cpTab[i1], " p[i1]");

  if(UT3D_comp2pt(&su1->cpTab[i1],
                  &su1->cpTab[i2], UT_DISP_cv) == 0) goto L_exit;


  irc = 1;


  L_exit:

  // printf("ex UT3D_sbs_ck_cycX %d\n",irc);

  return irc;

}

  

//=========================================================
  double UT3D_sbs_ck_planar (SurBSpl *su1) {
//=========================================================
// check if b_SPL-Surf = planar;
// out: max Disp von Plane.

  int    i1, i2;
  double d1, dMax = 0.;
  Plane  pl1;
  Point  *pTab;


  // printf("UT3D_sbs_ck_planar ptUNr=%d ptVNr=%d\n",su1->ptUNr,su1->ptVNr);

  // DEB_dump_obj__(Typ_SURBSP, su1, "");

  pTab = su1->cpTab;
  i2 = su1->ptUNr * su1->ptVNr;
  i1   = i2 - (su1->ptUNr / 2);

  // plane aus 3 punkten
  UT3D_pl_3pt (&pl1, &pTab[0], &pTab[su1->ptUNr-1], &pTab[i1]);


  // loop durch alle Punkte; max. Abstand von Plane suchen.
  // hier koennte man nur square holen, erst am Ende sqrt machen.
  for(i1=0; i1<i2; ++i1) {
    d1 = fabs(UT3D_slen_ptpl (&pTab[i1], &pl1));
    // DEB_dump_obj__(Typ_PT, &pTab[i1], " d1=%f P[%d]=",d1,i1);
    if(d1 > dMax) dMax = d1;
  }

  // printf("ex UT3D_sbs_ck_planar %f\n",dMax);
  return dMax;

}


//=======================================================================
  int UT3D_uvNr_sbsp (int *iu, int *iv, SurBSpl *sur, Memspc *wrkSeg) {
//=======================================================================
// get nr of rows,columns for grid of points with tolerance < UT_DISP_cv
// die optimale Anzahl von Punkten in U- und V-Richtung ermitteln
// nr of points for bspl-surf
// es gibt <ptVNr>  U-Kurven; jede U-Kurve hat <ptUNr> Punkte.
// wenn ptUNr = 4 und ptVNr = 3:
// die erste u-Kurve:  0,1,2,3;
// die zweite u-Kurve: 4,5,6,7;
// die letzte u-Kurve: 8,9,10,11.

// neu aus UT3D_ptaNr_srbsp

  int       i1, i2, irc;
  double    d1, rMin, lTot;
  void      *memstart;
  Point     *pta;
  CurvBSpl  bspl;


  // space for v-curve control points
  memstart = wrkSeg->next;


  // compute U-tolerance
  rMin = UT_VAL_MAX;
  lTot = 0.;
  // i1 = 0; // TEST
  for(i1=0; i1<sur->ptVNr; ++i1) {
    // get rat.B-Spline U-curve from rat.B-SplSurf-ControlCurve
    UT3D_bsp_sbspU (&bspl, i1, sur);

    // display ControlPoints/Curve:
    // UT3D_bspl_tst_cc (&bspl);

    // Crv hat nur 2 Punkte; somit muessen alle U-Kurven 2 Punkte haben..
    if(bspl.ptNr < 3) { *iu = 2; goto L_V0;}

    // vom gesamten Polgon den kleinsten Radius errechnen
    // es muss von jeweils 3 Punkten der Kreisradius errechnet werden
    UT3D_rMin_pta (&d1, bspl.ptNr, bspl.cpTab);
    if(d1 < rMin) rMin = d1;
      // printf(" U-rMin=%f\n",rMin);

    // Gesamtlaenge errechnen
    d1 = 0.;
    for(i2=1; i2<bspl.ptNr;  ++i2) {
      d1 += UT3D_len_2pt (&bspl.cpTab[i2-1], &bspl.cpTab[i2]);
    }
    if(d1 > lTot) lTot = d1;
  }
  // printf(" U:rMin=%f lTot=%f\n",rMin,lTot);
  *iu = lTot / (UT2D_len_ciSec (UT_DISP_cv, rMin) * 2.);  // sollte * 2 !


  // compute V-tolerance
  L_V0:
  rMin = UT_VAL_MAX;
  lTot = 0.;
  for(i1=0; i1<sur->ptUNr; ++i1) {
    // get rat.B-Spline U-curve from rat.B-SplSurf-ControlCurve
    irc = UT3D_bsp_sbspV (&bspl, i1, sur, wrkSeg);
    if(irc < 0) goto L_eom;

    // display ControlPoints/Curve:
    // UT3D_bspl_tst_cc (&bspl);

    // Crv hat nur 2 Punkte; somit muessen alle U-Kurven 2 Punkte haben..
    if(bspl.ptNr < 3) { *iv = 2; goto L_done;}

    // vom gesamten Polgon den kleinsten Radius errechnen
    // es muss von jeweils 3 Punkten der Kreisradius errechnet werden
    UT3D_rMin_pta (&d1, bspl.ptNr, bspl.cpTab);
    if(d1 < rMin) rMin = d1;
      // printf(" V-rMin=%f\n",rMin);

    d1 = 0.;
    for(i2=1; i2<bspl.ptNr;  ++i2) {
      d1 += UT3D_len_2pt (&bspl.cpTab[i2-1], &bspl.cpTab[i2]);
    }
    if(d1 > lTot) lTot = d1;

    // reset Workspace
    wrkSeg->next = memstart;
  }

  // printf(" V:rMin=%f lTot=%f\n",rMin,lTot);
  *iv = lTot / (UT2D_len_ciSec (UT_DISP_cv, rMin) * 1.);  // sollte * 2 !

  L_done:
  if(*iu < 2) *iu = 2;
  if(*iv < 2) *iv = 2;

  // printf("ex UT3D_uvNr_sbsp iu=%d iv=%d\n",*iu,*iv);

  return 0;


  L_eom:
  TX_Print("UT3D_uvNr_sbsp EOM");
  return -1;

}


/*
//==============================================================
  int SUSbsp_ck_tol (int *iu, int *iv, SurBSpl *sbs) {
//==============================================================
// die optimale Anzahl von Punkten in U- und V-Richtung ermitteln
// nr of points for bspl-surf
// es gibt <ptVNr>  U-Kurven; jede U-Kurve hat <ptUNr> Punkte.
// wenn ptUNr = 4 und ptVNr = 3:
// die erste u-Kurve:  0,1,2,3;
// die zweite u-Kurve: 4,5,6,7;
// die letzte u-Kurve: 8,9,10,11.


  int    i1, i2, i3, i4;
  double ld, lTot, lMax, d1, dMax;
  Vector vc1, vc2;


  // DEB_dump_obj__ (Typ_SURBSP, sbs, "SUSbsp_ck_tol");




  //===============================================================
  // Toleranz voff: loop durch U-Kurven, max. Abweichung pro Laenge suchen
  // die laengste Kurve und die max. Abweichung errechnen
  // laengste Kurve: die Teilstrecken zu den Kontrollpunkten addieren -> lTot
  i3   = 0;
  dMax = 0.;
  lMax = 0.;
  lTot = 0.;
  for(i1=0; i1<sbs->ptVNr; ++i1) {
    i3 += 2;
    // printf("------------ %d/%d\n",i3-2,i3-1);
    lTot = UT3D_len_2pt (&sbs->cpTab[i3-2], &sbs->cpTab[i3-1]);
    UT3D_vc_2pt (&vc2, &sbs->cpTab[i3-2], &sbs->cpTab[i3-1]);

    for(i2=2; i2<sbs->ptUNr; ++i2) {
      // DEB_dump_obj__(Typ_PT, &sbs->cpTab[i3], "P[%d][%d]=",i1,i2);
      vc1 = vc2;
      UT3D_vc_2pt (&vc2, &sbs->cpTab[i3-1], &sbs->cpTab[i3]);
      d1 = UT3D_angr_2vc__ (&vc1, &vc2);
      ld = UT3D_len_2pt (&sbs->cpTab[i3-1], &sbs->cpTab[i3]);
      lTot += ld;
      // printf("  d[%d/%d]=%f l=%f\n",i3-1,i3,UT_DEGREES(d1),ld);
      // d1 /= ld;  // normieren ; soll sein Anzahl Punkte / mm.
      if(d1 > dMax) dMax = d1;
      ++i3;
    }
    if(lTot > lMax) lMax = lTot;
  }

  // printf("  UNr=%d dMax=%f L=%f\n",sbs->ptUNr,UT_DEGREES(dMax),lMax);
  // d1 = sbs->ptUNr * (UT_DEGREES(dMax) / 4.);    // ptNr * (pro 20 Grad)
  // das werden zu viele ..
  // d1 = sbs->ptUNr * (dMax / pow(UT_DISP_cv, 0.8));    // ptNr * (pro 20 Grad)
  // d1 = dMax / pow(UT_DISP_cv, 1.3);

  d1 = (lMax/2.) * sin(dMax); // Hoehe gleichschenk. Dreieck
  d1 /= UT_DISP_cv;
  d1 = pow(d1, 0.75);

  *iu = d1;



  //================================================================
  // je flacher die Kurve, desto niedriger dMax; desto groesser voff.
  // *iu = lTot * dMax * 0.3 / pow(UT_DISP_cv, 0.13);
  // printf(">>>>>>>>>>>>\n");

  // Toleranz hoff: loop durch V-Kurven, max. Abweichung pro Laenge suchen
  i3   = sbs->ptUNr;
  dMax = 0.;
  lMax = 0.;
  lTot = 0.;
  for(i1=0; i1<i3; ++i1) {
    // printf("------------ %d/%d\n",i1,i1+i3);
    lTot = UT3D_len_2pt (&sbs->cpTab[i1],&sbs->cpTab[i1+i3]);
    UT3D_vc_2pt (&vc2, &sbs->cpTab[i1],&sbs->cpTab[i1+i3]);
    for(i2=2; i2<sbs->ptVNr; ++i2) {
      // DEB_dump_obj__(Typ_PT, &sbs->cpTab[i3], "P[%d][%d]=",i1,i2);
      vc1 = vc2;
      UT3D_vc_2pt (&vc2,&sbs->cpTab[((i2-1)*i3)+i1],&sbs->cpTab[(i2*i3)+i1]);
      d1 = UT3D_angr_2vc__(&vc1, &vc2);
      ld = UT3D_len_2pt(&sbs->cpTab[((i2-1)*i3)+i1],&sbs->cpTab[(i2*i3)+i1]);
      lTot += ld;
      // printf("  d1[%d/%d]=%f l=%f\n",((i2-1)*i3)+i1,(i2*i3)+i1,
                // UT_DEGREES(d1),ld);
      // d1 /= ld;
      if(d1 > dMax) dMax = d1;
    }
    if(lTot > lMax) lMax = lTot;
  }


  // lMax ist nun die Laenge einer Kurve,
  // dMax die max. Winkelabweichung zwischen 2 Kontrollpunkten !!
  // printf("  VNr=%d dMax=%f L=%f\n",sbs->ptVNr,UT_DEGREES(dMax),lMax);
  // d1 = sbs->ptVNr * (UT_DEGREES(dMax) / 4.);    // ptNr * (pro 20 Grad)
  // d1 = sbs->ptVNr * (dMax / pow(UT_DISP_cv, 0.8));    // ptNr * (pro 20 Grad)
  // d1 = dMax / pow(UT_DISP_cv, 1.3);
  // d1 = dMax / pow(lTot * UT_DISP_cv, 0.65);

  d1 = (lMax/2.) * sin(dMax); // Hoehe gleichschenk. Dreieck
  d1 /= UT_DISP_cv;
  d1 = pow(d1, 0.75);

  *iv = d1;

  // je flacher die Kurve, desto niedriger dMax; desto groesser voff.
  // *iv = lTot * dMax * 0.3 / pow(UT_DISP_cv, 0.13);




  if(*iu < 2) *iu = 2;
  if(*iv < 2) *iv = 2;

  printf("ex SUSbsp_ck_tol iu=%d iv=%d\n",*iu,*iv);

  return 0;

}
*/


/*=======================================================================*/
  int UT3D_pt_evparsbsp (Point *pt, SurBSpl *sur, double u, double v,
                            Memspc *workSeg) {
/*========================
UT3D_pt_evparsbsp    point <-- evaluate surface parameter

UT3D_pt_evparsbsp    Author: Thomas Backmeister       13.9.2003

Evaluate a point on a b-spline surface.

IN:
  SurBSpl *sur    ... b-spline surface
  double u        ... 1. coordinate parameter value
  double v        ... 2. coordinate parameter value
  Memspc *workSeg ... work space
OUT:
  Point *pt       ... surface point
Returncodes:
  0 = OK
 -1 = out of work space
 -2 = internal error
*/

  int rc, v1;
  void *memstart;
  Point *qTab;
  CurvBSpl bspl;


  // printf("\nzzzzzzzzzzUT3D_pt_evparsbsp %f,%f\n",u,v);


  // space for v-curve control points
  memstart = workSeg->next;
  qTab = workSeg->next;
  rc = UME_add (workSeg, sizeof(Point) * sur->ptVNr);
  if (rc < 0) goto L_outOfWorkSpace;

  // init b-spline u-curve
  bspl.ptNr = sur->ptUNr;
  bspl.deg = sur->degU;
  bspl.kvTab = sur->kvTabU;

  // Kontrollpunkte qTab der Curve mit uPar=u errechnen
  for (v1=0; v1<sur->ptVNr; ++v1) {

    // control points of u-curve
    bspl.cpTab = &(sur->cpTab[v1*sur->ptUNr]);

    // evaluate point on u-curve
    rc = UT3D_pt_evparCrvBSpl (&(qTab[v1]), &bspl, u);
    if (rc < 0) return -2;
  }

  // init b-spline v-curve
  bspl.ptNr = sur->ptVNr;
  bspl.deg = sur->degV;
  bspl.kvTab = sur->kvTabV;
  bspl.cpTab = qTab;

  // evaluate point on v-curve
  rc = UT3D_pt_evparCrvBSpl (pt, &bspl, v);
  if (rc < 0) return -2;

  // release work space
  workSeg->next = memstart;

  return 0;

L_outOfWorkSpace:
  TX_Error("UT3D_pt_evparsbsp: out of work space");
  return -1;
}



//======================================================================
  int UT3D_ptgrid_sbsp (Point *p3Tab, Point *p2Tab, Vector *vTab,
                        double *du, double *dv,
                        SurBSpl *sbs, int iu, int iv, Memspc *workSeg) {
//======================================================================
// Ein Grundraster mit gleichmaessigen Parameterabstaenden generieren
// In:  iu, iv: Anzahl der gewuenschten Punkte in u/v-Richtung
// Out: p3Tab       iu * iv points; NULL for none
//      vTab        normalvectors; NULL for none
//      *du, *dv    parameters


  int      ii, irc, mode;
  double   uMin, uMax, vMin, vMax, uu, vv, tol;


  // printf("UT3D_ptgrid_sbsp iu=%d iv=%d ii=%d\n",iu,iv,iu * iv);


  uMin = sbs->kvTabU[0];
  vMin = sbs->kvTabV[0];
  uMax = sbs->kvTabU[sbs->ptUNr + sbs->degU];
  vMax = sbs->kvTabV[sbs->ptVNr + sbs->degV];
    // printf("UT3D_ptgrid_sbsp uMin=%f uMax=%f  vMin=%f vMax=%f\n",
           // uMin, uMax, vMin, vMax);


  if(p3Tab) {
    if(vTab) mode = 0; // points & vectors
    else mode = 1;     // points
  } else {
    mode = 2;          // vectors
  }

  // tol = 0.2;
  // uMin -= tol;   uMax += tol;
  // vMin -= tol;   vMax += tol;



  *du = (uMax - uMin) / (iu - 1);
  *dv = (vMax - vMin) / (iv - 1);

  uu = uMin;
  vv = vMin;

  uMax += *du / 2.;
  vMax += *dv / 2.;

  ii = 0;


  L_nxt_u:
  if(p2Tab) {
    p2Tab[ii].x = uu;
    p2Tab[ii].y = vv;
    p2Tab[ii].z = 0.;
  }
    // printf(" L_nxt_u: %d u=%f v=%f\n",ii,uu,vv);
  irc = UT3D_ptvc_evparsbsp (&p3Tab[ii], &vTab[ii], mode, 0,
                             sbs, uu, vv, workSeg);
  // irc = UT3D_pt_evparsbsp (&p3Tab[ii], sbs, uu, vv, workSeg);
  if (irc < 0) return -1;
    // printf(" %d (u,v)= (%f,%f)   pt= %f %f %f\n",ii,uu,vv,
           // p3Tab[ii].x,p3Tab[ii].y,p3Tab[ii].z);
    // APT_disp_SymB (SYM_TRI_S, 2, &p3Tab[ii]);

  ++ii;
  uu += *du;
  if(uu < uMax) goto L_nxt_u;

  uu = uMin;
  vv += *dv;
  if(vv < vMax) goto L_nxt_u;

    // printf("ex UT3D_ptgrid_sbsp du=%f dv=%f ii=%d\n",*du,*dv,ii);

  return 0;

}


//================================================================
  int UT3D_uparmid_sbsp (double *pm, SurBSpl *sbs) {
//================================================================
// mittleren U-Wert

  double   uMin, uMax;

  uMin = sbs->kvTabU[0];
  uMax = sbs->kvTabU[sbs->ptUNr + sbs->degU];

  *pm = (uMin + uMax) / 2.;

  // printf(" ex UT3D_uparmid_sbsp %f\n",*pm);
  return 0;

}


//================================================================
  int UT3D_vparmid_sbsp (double *pm, SurBSpl *sbs) {
//================================================================
// mittleren v-Wert

  double   vMin, vMax;

  vMin = sbs->kvTabV[0];
  vMax = sbs->kvTabV[sbs->ptVNr + sbs->degV];

  *pm = (vMin + vMax) / 2.;

  // printf(" ex UT3D_vparmid_sbsp %f\n",*pm);
  return 0;

}


//===========================================================================
  int UT3D_parsbsp_pt (double *up, double *vp,
                       Point *pti,
                       Point *pTab, double dui, double dvi, int uNr, int vNr,
                       SurBSpl *sbs, Memspc *wrkSpc) {
//===========================================================================
// IN: pti - die u/v-Parameter dieses Punktes (*up, *vp) ermitteln
//     pTab[uNr,vNr] - Gitterraster auf der B-SplineSurf (UT3D_ptgrid_sbsp)
//     du,dv - entspricht dem Parameterabstand der Gitterpunkte
//     uMin = sbs->kvTabU[0];  vMin = sbs->kvTabV[0];
//
// Den kuerzesten Abstand zu pti im Gitterraster suchen; dann mit dem
// umliegenden Rechteck im Gitterraster beginnen; die relative Abweichung
// um Testviereck errechnen und ein neues Testviereck definieren.
//
// see also SUR_pt2_prjptsur !
// TODO: du,dv,iu,iv, - eigentlich überbestimmt; ?


  int     i1, i2, i3, iu1, iv1, iloop, plMain, iul, iur, ivu, ivo;
  double  d1, ddu,ddv, dc, uc,vc,u0,v0,u1,v1,u2,v2,u3,v3, du, dv, du1, dv1;
  double  uTot,vTot, uMin, uMax, vMin, vMax, az, ay;
  Point   pc, pa[4];
  Vector  vc1, vc2, vc3, vc4, vc13, vc24, vcp;
  Vector  vcz;



  // // Bereich Knotvektor U:
  i2 = sbs->ptUNr + sbs->degU + 1;           // Anzahl U-Knots
  uMin = sbs->kvTabU[0];
  uMax = sbs->kvTabU[i2-1];

  // // Bereich Knotvektor V:
  i2 = sbs->ptVNr + sbs->degV + 1;           // Anzahl V-Knots
  vMin = sbs->kvTabV[0];
  vMax = sbs->kvTabV[i2-1];

  // uTot = uMax - uMin;
  // vTot = vMax - vMin;

/*
  //==== TESTAUSG: ================================
  DEB_dump_obj__(Typ_PT, pti,
         "\n=============================================\n"
         "UT3D_parsbsp_pt uNr=%d vNr=%d du=%f dv=%f\n",
          uNr,vNr,dui,dvi);

    GR_Disp_pt (pti, SYM_TRI_S, 1);

    printf(" kU von %f bis %f\n",uMin,uMax);
    printf(" kV von %f bis %f\n",vMin,vMax);

  // das PrimaerGrid darstellen
  GR_Disp_pTab (uNr, pTab, SYM_TRI_S, 1);
  GR_Disp_pTab (uNr, &pTab[uNr], SYM_TRI_S, 2);
  GR_Disp_pTab (uNr, &pTab[uNr*2], SYM_TRI_S, 3);
  //==== TESTAUSG: ================================
*/



  // den naechsten Punkt im existierenden Raster suchen
  i3 = 0;
  dc = UT_VAL_MAX;
  for(i1=0; i1<vNr; ++i1) {
    for(i2=0; i2<uNr; ++i2) {
      d1 = UT3D_lenq_PtPt (pti, &pTab[i3]);
      if(d1 < dc) {
        dc = d1;
        iv1 = i1; iu1 = i2;
        // printf(" near %d %d %d %f\n",i1,i2,i3,dc);
      }
      ++i3;
    }
  }

  // disp nearest Point
  // printf(" nearest: iu1=%d iv1=%d\n",iu1,iv1);
  // i3 = iv1*uNr+iu1; pc = pTab[i3]; GR_Disp_pt (&pc, SYM_STAR_S, 2);


  // uc,vc sind die Werte des gefundenen Punktes iu1,iv1.
  // uc = uMin + du * uNr;
  du = dui;
  dv = dvi;
  uc = uMin + du * iu1;
  vc = vMin + dv * iv1;
  // printf(" uc=%f vc=%f\n",uc,vc);


  // Randkorrekturen:
  iul = iu1-1;
  if(iul < 0) {
    // printf("iul < 0\n");
    iul = 0;
    du = dui / 2.;
    uc += du;
  }

  iur = iu1+1;
  if(iur > uNr-1) {
    // printf("iur > uNr\n");
    iur = uNr-1;
    du = dui / 2.;
    uc -= du;
  }

  ivu = iv1-1;
  if(ivu < 0) {
    // printf("ivu < 0\n");
    ivu = 0;
    dv = dvi / 2.;
    vc += dv;
  }

  ivo = iv1+1;
  if(ivo > vNr-1) {
    // printf("ivo > vNr\n");
    ivo = vNr-1;
    dv = dvi / 2.;
    vc -= dv;
  }
  // printf(" iul=%d iur=%d ivu=%d ivo=%d\n",iul,iur,ivu,ivo);
  // printf(" du=%f dv=%f\n",du,dv);



  // die Eckpunkte rund um den gefundenen punkt (iu1,iv1) nun nach pa laden
  // der Punkt links unter iu1/iv1
  // i1 = iu1-1 + (iv1-1)*uNr;
  pa[0] = pTab[iul + ivu*uNr];
  // printf(" ecke[0]: %d\n",iul + ivu*uNr);


  // der Punkt rechts unter iu1/iv1
  // i2 = iu1+1 + (iv1-1)*uNr;
  pa[1] = pTab[iur + ivu*uNr];
  // printf(" ecke[1]: %d\n",iur + ivu*uNr);


  // der Punkt rechts ueber iu1/iv1
  // i2 = iu1+1 + (iv1+1)*uNr;
  pa[2] = pTab[iur + ivo*uNr];
  // printf(" ecke[2]: %d\n",iur + ivo*uNr);


  // der Punkt links ueber iu1/iv1
  // i3 = iu1-1 + (iv1+1)*uNr;
  pa[3] = pTab[iul + ivo*uNr];
  // printf(" ecke[3]: %d\n",iul + ivo*uNr);


  //------ Disp new rectangle
  // GR_Disp_cv (pa, 4, 2);
  // GR_Disp_pt (&pa[0], SYM_TRI_S, 2);  GR_Disp_tx(&pa[0], " 0", 2);
  // GR_Disp_pt (&pa[1], SYM_TRI_S, 2);  GR_Disp_tx(&pa[1], " 1", 2);
  // GR_Disp_pt (&pa[2], SYM_TRI_S, 2);  GR_Disp_tx(&pa[2], " 2", 2);
  // GR_Disp_pt (&pa[3], SYM_TRI_S, 2);  GR_Disp_tx(&pa[3], " 3", 2);
  // return -1;
  //------


  iloop = 0;


/*
  // die mainPlane zum gegebenen Grundrasterpatch ermitteln
  // UT3D_vc_perp3pt (&vcz, &pa[1], &pa[0], &pa[2]);
  if(UT3D_comp2pt(&pa[1],&pa[2],UT_TOL_pt) == 1)
    UT3D_vc_perp3pt (&vcz, &pa[3], &pa[0], &pa[1]);
  else
    UT3D_vc_perp3pt (&vcz, &pa[1], &pa[0], &pa[2]);

  plMain = UT3D_bp_vcz (&vcz);
    printf(" plMain=%d\n",plMain);
*/
    


  //----------------------------------------------------------------------
  // ab hier allgemein:
  // u0,v0 sind die Parameterwerte fuer pa[0]
  // du ist der Parameterwert fuer den HALBEN Abstand von pa[0] bis pa[1]
  // uc,vc sind die Par.Werte des Punktes (im Zentrum von pa[0/1/2],
  //   von wo weg gemessen wird.



  L_next_try:
  ++iloop;

  // nun u und v fuer einen neuen Testpunkt bestimmen
  //
  //     pa[3]----------pa[2]
  //  ^     |            |
  //  |     |   P        |
  //  V   v3|  /vp       |
  //  |     | /          |
  //     pa[0]----v1---pa[1]  - U ->
  //
  // du1 = rel.Abstand in U-Richtung (0-1) errechnen.
  // du1=0.5 wenn Punkt genau in der Mitte zwischen pa[0] und pa[1] liegt


/*
  // wenn (pa[0] == pa[3]) && (pa[1] == pa[2])
  if((UT3D_comp2pt(&pa[0],&pa[3],UT_TOL_pt) == 1) &&
     (UT3D_comp2pt(&pa[1],&pa[2],UT_TOL_pt) == 1))   {

    UT3D_vc_2pt (&vcp, &pa[0], pti);
    UT3D_vc_2pt (&vc1, &pa[0], &pa[1]);
    UT3D_parvc_2vcbp (&du1, plMain, &vcp, &vc1);
    dv1 = 1.;
     

  } else {
*/
    // alle Vektoren setzen
    UT3D_vc_2pt (&vc1, &pa[0], &pa[1]);
    UT3D_vc_2pt (&vc2, &pa[1], &pa[2]);
    UT3D_vc_2pt (&vc3, &pa[3], &pa[2]);
    UT3D_vc_2pt (&vc4, &pa[0], &pa[3]);
    UT3D_vc_2pt (&vcp, &pa[0], pti);
    // vc1 und vc3 zu einem MittelwertsVektor vc13 mischen
    UT3D_vc_merge2vc (&vc13, &vc1, &vc3);
    // vc2 und vc4 zu einem MittelwertsVektor vc24 mischen
    UT3D_vc_merge2vc (&vc24, &vc2, &vc4);
    // Backplane suchen; nur beim ersten Mal notwendig
    if(iloop == 1) {
      UT3D_vc_perp2vc(&vcz, &vc13, &vc24);
      // GR_Disp_vc(&vcz, pti, 2, 1);
      UT3D_2angr_vc (&az, &ay, &vcz);
      // printf(" az=%f ay=%f\n",az,ay);
      plMain = UT3D_bp_perp_vc (NULL, &vcz);
      // printf(" plMain=%d\n",plMain);
    }

    // den vcp in die Ebene vc13 - vc24 runterprojizieren
    UT3D_vc_projvc2vc (&vcp, &vcp, &vc13, &vc24);
    // wo liegt vcp im parallelogramm vc13 - vc24
    UT3D_2parvc_3vcbp (&du1, &dv1, plMain, &vc13, &vcp, &vc24);

  // }


  // du1 = rel.Fehler in U-Richtung (0-1)
  // dv1 = rel.Fehler in V-Richtung (0-1)
  // dv1=0.5 wenn Punkt genau in der Mitte zwischen pa[1] und pa[2] liegt
  // printf(" du1=%f dv1=%f\n",du1,dv1);

/*
  // Kruemmungsfehler reparieren
  // bei halbkreisfoermiger Flaeche liegt die Schaetzung ziemlich falsch !
  if     (du1 < -3.)   du1 = -3.;
  else if(du1 >  3.)   du1 =  3.;

  if     (dv1 < -3.)   dv1 = -3.;
  else if(dv1 >  3.)   dv1 =  3.;
*/



  // uc,vc auf Punkt pa[0] (links unten) setzen)
  uc -= du ;
  vc -= dv ;
    // printf(" par.p[0] uc=%f vc=%f\n",uc,vc);
    // UT3D_pt_evparsbsp (&pc, sbs, uc, vc, wrkSpc);
    // GR_Disp_pt (&pc, SYM_TRI_S, 4);


  // neuen Mittelpunkt uc,cv errechnen
  uc += du * du1 * 2.;
  vc += dv * dv1 * 2.;
    // printf(" new:par uc=%f vc=%f\n",uc,vc);


  // uc/vc sollte die min/max-Werte nicht ueberschreiten
  if(uc < uMin)      uc = uMin;
  else if(uc > uMax) uc = uMax;

  if(vc < vMin)      vc = vMin;
  else if(vc > vMax) vc = vMax;




  // den neuen Punkt Testpunkt pc errechnen
  UT3D_pt_evparsbsp (&pc, sbs, uc, vc, wrkSpc);
    // GR_Disp_pt (&pc, SYM_STAR_S, 2);


  // dc = Abst gesuchter Punkt pti - neuer Punkt pc
  dc = UT3D_len_2pt (pti, &pc);
  if(dc < UT_TOL_cv) goto L_FERTIG;


  // den Bereich fuer ein neues Testrechteck errechnen
/*
  // Abst p0-p2 (eine Diagonale)
  d1 = UT3D_len_2pt (&pa[0], &pa[2]);
  if(d1 < UT_TOL_min2) d1 = UT_TOL_min2;

  // Soll-Ist-Fehler als differenz von p0-p2 und von soll-ist errechnen
  dd = dc/d1 * 3.; // wieweit wir naeher gekommen sind ..
  du = du * ddu;
  dv = dv * ddv;
*/


  // neues du errechen
  if     ((du1 > 0.3)&&(du1 < 0.7)) du /= 3.;
  else if((du1 > 0.0)&&(du1 < 1.0)) du /= 2.;

  if     ((dv1 > 0.3)&&(dv1 < 0.7)) dv /= 3.;
  else if((dv1 > 0.0)&&(dv1 < 1.0)) dv /= 2.;


  // printf(" iloop=%d du=%f dv=%f dc=%f d1=%f\n",iloop,du,dv,dc,d1);


  // nun ein neues Quadrat rund um den neuen Punkt pc legen;
  // die Punkte A,B,C -> pa.
  u0 = uc - du;
  v0 = vc - dv;
  UT3D_pt_evparsbsp (&pa[0], sbs, u0, v0, wrkSpc);
    // GR_Disp_pt (&pa[0], SYM_TRI_S, 2);
    // GR_Disp_tx(&pa[0], "0", 2);

  u1 = uc + du;
  v1 = vc - dv;
  UT3D_pt_evparsbsp (&pa[1], sbs, u1, v1, wrkSpc);
    // GR_Disp_pt (&pa[1], SYM_TRI_S, 2);
    // GR_Disp_tx(&pa[1], "1", 2);

  u2 = uc + du;
  v2 = vc + dv;
  UT3D_pt_evparsbsp (&pa[2], sbs, u2, v2, wrkSpc);
    // GR_Disp_pt (&pa[2], SYM_TRI_S, 2);
    // GR_Disp_tx(&pa[2], "2", 2);

  u3 = uc - du;
  v3 = vc + dv;
  UT3D_pt_evparsbsp (&pa[3], sbs, u3, v3, wrkSpc);
    // GR_Disp_pt (&pa[3], SYM_TRI_S, 2);
    // GR_Disp_tx(&pa[3], "3", 2);
    // GR_Disp_cv (pa, 4, 3);


  if(iloop < 16) goto L_next_try;  // Anzahl Versuche Defaul=100
  // keine Loesung nach n Versuchen ..
  printf("UT3D_parsbsp_pt I000 Pos %f,%f,%f\n",pti->x,pti->y,pti->z);
  // TX_Error("UT3D_parsbsp_pt I000");
  // exit(0);




  //----------------------------------------------------------------
  L_FERTIG:
  *up = uc;
  *vp = vc;
  // printf("ex UT3D_parsbsp_pt uc=%f vc=%f\n\n",*up,*vp);


  return 0;

}


//===========================================================================
  int UT3D_ptvc_evparsbsp (Point *pto, Vector *vn, int mode, int iDir,
                           SurBSpl *sbs, double pu, double pv, Memspc *workSeg){
//===========================================================================
/// \code
/// UT3D_ptvc_evparsbsp         get point/vector on bSplSur from U-V-params
/// Input:
///   mode   0=Point & vector  1=PointOnly 2=vectorOnly
///   iDir   0=perp; 1=parl, 2=across;
/// \endcode

  int      i2, irc;
  double   uMin, uMax, vMin, vMax;
  // Point    pt1;
  Vector   vtu, vtv;
  CurvBSpl bspl;
  void     *memstart;

  // printf("UT3D_ptvc_evparsbsp %d %f,%f\n",mode,pu,pv);
  // DEB_dump_obj__ (Typ_SURBSP, sbs, "  suBsp:");


  memstart = workSeg->next;


  //----------------------------------------------------------------
  // B-Spline in Richtg V mit U=pu erzeugen
  irc = UT3D_bspV_sbsp_u (&bspl, sbs, pu, workSeg);
  if(irc < 0) goto L_fertig;
    // DEB_dump_obj__ (Typ_CVBSP, &bspl, "  cvBspV:");


  // Punkt auf Curve
  if(mode < 2) {
    irc = UT3D_pt_evparCrvBSpl (pto, &bspl, pv);
    if(irc < 0) goto L_fertig;
      // GR_Disp_pt (pto, SYM_TRI_S, 1);
  }


  if(mode == 1) goto L_fertig; // Point only


  // Tangente in p=pu
  if(iDir < 2) {
    UT3D_vc_evalparCv (&vtv, &bspl, pv);
      // GR_Disp_vc (&vtv, pto, 1, 0);
      // DEB_dump_obj__ (Typ_VC, &vtv, "  vcv:");

    if(iDir == 1) {
      *vn = vtv;
      goto L_fertig;
    }
  }

  // release work space
  workSeg->next = memstart;


  //----------------------------------------------------------------
  // B-Spline in Richtg U mit v=pv erzeugen
  irc = UT3D_bspU_sbsp_v (&bspl, sbs, pv, workSeg);
  if(irc < 0) goto L_fertig;
    // DEB_dump_obj__ (Typ_CVBSP, &bspl, "  cvBspU:");


  // Tangente in p=pu
  UT3D_vc_evalparCv (&vtu, &bspl, pu);
    // GR_Disp_vc (&vtu, &pt1, 1, 0);
    // DEB_dump_obj__ (Typ_VC, &vtu, "  vcu:");

  if(iDir == 2) {
    *vn = vtu;
    goto L_fertig;
  }


  //----------------------------------------------------------------
  // Normalvektor auf die Tangenten
  UT3D_vc_perp2vc (vn, &vtu, &vtv);
  UT3D_vc_setLength (vn, vn, 1.);
    // GR_Disp_vc (vn, pto, 0, 0);
    // DEB_dump_obj__ (Typ_VC, vn, "  vn:");



  // release work space
  L_fertig:
  workSeg->next = memstart;



  return 0;


}


//===========================================================================
  int UT3D_bspV_sbsp_u (CurvBSpl *bspl, SurBSpl *sbs, double pu,
                         Memspc *workSeg) {
//===========================================================================
// B-Spline in Richtung V mit dem Par.Wert pu erzeugen
// die Kontrollpunkte werden im workSeg abgelegt !

  int   i1, irc;
  Point *qTab;

  // printf("UT3D_bspV_sbsp_u %f,%f\n",pu);

  // reserve space for Contr.points in workSeg
  qTab = workSeg->next;
  irc = UME_add (workSeg, sizeof(Point) * sbs->ptVNr);
  if(irc < 0) return -2;


  // loop tru fixed U-Curves and create point at U=pu
  bspl->ptNr  = sbs->ptUNr;
  bspl->deg   = sbs->degU;
  bspl->kvTab = sbs->kvTabU;

  for(i1=0; i1<sbs->ptVNr; ++i1) {

    // control points of u-curve
    // (eine fix definierte Reihe in Richtung U)
    bspl->cpTab = &(sbs->cpTab[i1*sbs->ptUNr]);

    // evaluate point on u-curve
    irc = UT3D_pt_evparCrvBSpl (&(qTab[i1]), bspl, pu);
    if(irc < 0) return -2;
  }

  // fix resulting v-curve
  bspl->ptNr  = sbs->ptVNr;
  bspl->deg   = sbs->degV;
  bspl->kvTab = sbs->kvTabV;
  bspl->cpTab = qTab;
  bspl->v0    = sbs->kvTabV[0];
  bspl->v1    = sbs->kvTabV[sbs->ptVNr + sbs->degV];


    // TESTDISP
    // GR_Disp_CvBSp (bspl, 2, workSeg);

  return 0;

}

//===========================================================================
  int UT3D_bspU_sbsp_v (CurvBSpl *bspl, SurBSpl *sbs, double pv,
                         Memspc *workSeg) {
//===========================================================================
// B-Spline in Richtung U mit dem Par.Wert pv erzeugen
// die Kontrollpunkte werden im workSeg abgelegt !

  int   i1, i2, i3, irc;
  Point *qTab, *vTab;

  printf("UT3D_bspU_sbsp_v %f\n",pv);

  // reserve space for Contr.points in workSeg
  qTab = workSeg->next;
  irc = UME_add (workSeg, sizeof(Point) * sbs->ptUNr);
  if(irc < 0) return -2;

  // noch Platz fuer eine V-Reihe holen
  vTab = workSeg->next;
  irc = UME_add (workSeg, sizeof(Point) * sbs->ptVNr);
  if(irc < 0) return -2;

  // loop tru fixed V-Curves and create point at V=pv
  bspl->ptNr  = sbs->ptVNr;
  bspl->deg   = sbs->degV;
  bspl->kvTab = sbs->kvTabV;
  bspl->cpTab = vTab;

  for(i1=0; i1<sbs->ptUNr; ++i1) {

    // control points of v-curve
    i3 = i1;
    for(i2=0; i2<sbs->ptVNr; ++i2) {
      vTab[i2] = sbs->cpTab[i3];
      // GR_Disp_pt (&vTab[i2], SYM_STAR_S, 0);
      i3 += sbs->ptUNr;
    }
    // evaluate point on u-curve
    irc = UT3D_pt_evparCrvBSpl (&(qTab[i1]), bspl, pv);
    if(irc < 0) return -2;
  }

  // fix resulting v-curve
  bspl->ptNr  = sbs->ptUNr;
  bspl->deg   = sbs->degU;
  bspl->kvTab = sbs->kvTabU;
  bspl->cpTab = qTab;
  bspl->v0    = sbs->kvTabU[0];
  bspl->v1    = sbs->kvTabU[sbs->ptUNr + sbs->degU];


    // TESTDISP
    // GR_Disp_CvBSp (bspl, 2, workSeg);

  return 0;

}


//================================================================
  int UT3D_bsp_sbspU (CurvBSpl *bspl, int vNr, SurBSpl *sur) {
//================================================================
// get B-Spline U-Curve from B-SplSurf-ControlCurve
// nur Basiscurve liefern; kein Isocurve errechnen.
// see also UT3D_pt_evparsrbsp



  // eine isocurve mit par. u generieren.
  // init b-spline u-curve
  bspl->ptNr  = sur->ptUNr;
  bspl->deg   = sur->degU;
  bspl->dir   = 0;                          // forward
  bspl->kvTab = sur->kvTabU;
  bspl->v0    = sur->kvTabU[0];
  bspl->v1    = sur->kvTabU[sur->ptUNr+sur->degU];

  // control points of u-curve
  bspl->cpTab = &(sur->cpTab[vNr*sur->ptUNr]);

  // DEB_dump_obj__ (Typ_CVRBSP, bspl, "ex UT3D_bsp_sbspU");

  return 0;

}


//================================================================
  int UT3D_cv_sbspout (int *pNr, Point *pTab, SurBSpl *sur,
                       Memspc *wrkSeg) {
//================================================================
// UT3D_cv_sbspout           outline (polygon) of B-SplSurf
// see GL_sSym_sbsp

  int       i1, p1Nr, pMax;
  CurvBSpl  cv1;


  pMax = *pNr;
  *pNr = 0;


  // DEB_dump_obj__ (Typ_SURBSP, sur, "UT3D_cv_sbspout %d\n",pMax);


  //----------------------------------------------------------------
  // randKurve  U (U0-U1 / V0)        // U along X; V along Y
  UT3D_bsp_sbspU (&cv1, 0, sur);

  // CurvRBSpl -> polygon
  i1 = bspl_pol_bsp (&p1Nr, pTab, &cv1, pMax, UT_DISP_cv/3.);
    // GR_Disp_npti (p1Nr, pTab, SYM_STAR_S, ATT_COL_RED, ATT_COL_YELLOW);
  if(i1 < 0) return -1;
  pTab = &pTab[p1Nr];
  pMax -= p1Nr;
  *pNr += p1Nr;


  //----------------------------------------------------------------
  // randKurve V-aussen (U0 / V0-V1)
  i1 = sur->ptUNr-1;
  i1 = UT3D_bsp_sbspV (&cv1, i1, sur, wrkSeg);
    // GR_Disp_CvBSp (&cv1, 9);

  // CurvRBSpl -> polygon
  i1 = bspl_pol_bsp (&p1Nr, pTab, &cv1, pMax, UT_DISP_cv/3.);
    // GR_Disp_npti (p1Nr, pTab, SYM_STAR_S, ATT_COL_RED, ATT_COL_YELLOW);
  if(i1 < 0) return -1;
  pTab = &pTab[p1Nr];
  pMax -= p1Nr;
  *pNr += p1Nr;


  //----------------------------------------------------------------
  // randKurve U-aussen (U1 / V0-V1)
  i1 = sur->ptVNr-1;
  UT3D_bsp_sbspU (&cv1, i1, sur);
    // GR_Disp_CvBSp (&cv1, 9);

  // curve umdrehen !  (see UTO_stru_inv)
  // MEM_swap_2db (&cv1.v0, &cv1.v1);

  // CurvRBSpl -> polygon
  i1 = bspl_pol_bsp (&p1Nr, pTab, &cv1, pMax, UT_DISP_cv/3.);
  if(i1 < 0) return -1;
  UT3D_cv_inv (p1Nr, pTab); //change dir
    // GR_Disp_npti (p1Nr, pTab, SYM_STAR_S, ATT_COL_RED, ATT_COL_YELLOW);
  pTab = &pTab[p1Nr];
  pMax -= p1Nr;
  *pNr += p1Nr;


  //----------------------------------------------------------------
  // randKurve V0 (U0-U1 / V0)
  i1 = UT3D_bsp_sbspV (&cv1, 0, sur, wrkSeg);
    // GR_Disp_CvBSp (&cv1, 9);

  // // curve umdrehen !  (see UTO_stru_inv)
  // MEM_swap_2db (&cv1.v0, &cv1.v1);

  // CurvRBSpl -> polygon
  i1 = bspl_pol_bsp (&p1Nr, pTab, &cv1, pMax, UT_DISP_cv/3.);
  if(i1 < 0) return -1;
  UT3D_cv_inv (p1Nr, pTab); //change dir
    // GR_Disp_npti (p1Nr, pTab, SYM_STAR_S, ATT_COL_RED, ATT_COL_YELLOW);
  pTab = &pTab[p1Nr];
  pMax -= p1Nr;
  *pNr += p1Nr;


    // printf("ex UT3D_cv_sbspout %d\n",*pNr);

  return 0;

}


//=======================================================================
  int UT3D_cv_sbspU (int pNr, Point *pTab, double uu, SurBSpl *sbs,
                     Memspc *workSeg) {
//=======================================================================
// UT3D_cv_sbspU            Isocurve (polygon) from U-wert
// Input:
// see UT3D_ptgrid_sbsp


  int      ii, irc;
  double   vMin, vMax, vv, dv;


  // printf("UT3D_cv_sbspU %d %lf\n",pNr,uu);


  vMin = sbs->kvTabV[0];
  vMax = sbs->kvTabV[sbs->ptVNr + sbs->degV];
  dv = (vMax - vMin) / (pNr - 1);
    // printf("UT3D_cv_sbspU vMin=%f vMax=%f dv=%lf\n", vMin, vMax, dv);

  vv = vMin;

  vMax += dv / 2.;

  ii = 0;


  L_nxt_v:

  irc = UT3D_pt_evparsbsp (&pTab[ii], sbs, uu, vv, workSeg);
  if (irc < 0) return -1;
  ++ii;
    // printf("(u,v)= (%f,%f)   pt= %f %f %f\n",uu,vv,
           // pTab[ii].x,pTab[ii].y,pTab[ii].z);
    // APT_disp_SymB (SYM_TRI_S, 2, &pTab[ii]);

  vv += dv;
  if(vv < vMax) goto L_nxt_v;


    // printf("ex UT3D_cv_sbspU dv=%f ii=%d\n",dv,ii);
    // GR_Disp_npti (pNr, pTab, SYM_STAR_S, ATT_COL_RED, ATT_COL_YELLOW);

  return 0;

}


//================================================================
  int UT3D_cv_sbspV (int pNr, Point *pTab, double vv, SurBSpl *sbs,
                     Memspc *workSeg) {
//================================================================
// UT3D_cv_srbspV            Isocurve (polygon) from V-wert
// see UT3D_ptgrid_srbsp

  int      ii, irc;
  double   uMin, uMax, vMin, vMax, uu, du;


  // printf("UT3D_cv_sbspV %d %lf\n",pNr,vv);


  uMin = sbs->kvTabU[0];
  uMax = sbs->kvTabU[sbs->ptUNr + sbs->degU];
  du = (uMax - uMin) / (pNr - 1);
    // printf("UT3D_cv_sbspV uMin=%f uMax=%f du=%f\n", uMin, uMax, du);

  uu = uMin;

  uMax += du / 2.;

  ii = 0;


  L_nxt_u:
  irc = UT3D_pt_evparsbsp (&pTab[ii], sbs, uu, vv, workSeg);
  if (irc < 0) return -1;
  ++ii;
    // printf("(u,v)= (%f,%f)   pt= %f %f %f\n",uu,vv,
           // pTab[ii].x,pTab[ii].y,pTab[ii].z);
    // APT_disp_SymB (SYM_TRI_S, 2, &pTab[ii]);

  uu += du;
  if(uu < uMax) goto L_nxt_u;


    // printf("ex UT3D_cv_sbspV du=%lf\n",du);
    // GR_Disp_npti (pNr, pTab, SYM_STAR_S, ATT_COL_RED, ATT_COL_YELLOW);


  return 0;

}


//================================================================
  int UT3D_bsp_sbspV (CurvBSpl *bspl, int uNr, SurBSpl *sur,
                      Memspc *wrkSeg) {
//================================================================
// get B-Spline V-Curve from B-SplSurf-ControlCurve
// nur Basiscurve liefern; kein Isocurve errechnen.
// see also UT3D_pt_evparsrbsp
// cpTab und wTab werden im wrkSpc abgelegt !

  int    i1, i2;
  Point  *pta;

  // printf("UT3D_bsp_sbspV %d\n",uNr);

  // eine isocurve mit par. u generieren.
  // init b-spline u-curve
  bspl->ptNr  = sur->ptVNr;
  bspl->deg   = sur->degV;
  bspl->dir   = 0;                          // forward
  bspl->kvTab = sur->kvTabV;
  bspl->v0    = sur->kvTabV[0];
  bspl->v1    = sur->kvTabV[sur->ptVNr+sur->degV];

  // reserve space for ptVNr Point
  bspl->cpTab = wrkSeg->next;
  i1 = UME_add (wrkSeg, sur->ptVNr * sizeof(Point));
  if(i1 < 0) return -1;


  // control points of u-curve
  pta = bspl->cpTab;
  for(i1=0; i1<sur->ptVNr; ++i1) {
    i2 = uNr + (sur->ptUNr * i1);
    // printf(" i1=%d i2=%d\n",i1,i2);
    pta[i1] = sur->cpTab[i2];
  }

  // DEB_dump_obj__ (Typ_CVBSP, bspl, "ex UT3D_bsp_sbspV");

  return 0;

}


//================================================================
  int UT3D_pt_intlnsbs (int *nxp, Point *xpTab,
                        int xpSiz, Line *ln, SurBSpl *sbs,
                        Memspc *tmpSpc1) {
//================================================================
// UT3D_pt_intlnsbs          intersect Line - BspSur

// via USBS_IntsectLnBspSur

 
  int        irc;
  Memspc mem1Seg;
  Memspc mem2Seg;
  Memspc mem3Seg;
  Memspc mem4Seg;
  Memspc     *memSegT[4];


  // printf("UT3D_pt_intlnsbs siz=%d\n",xpSiz);
  // DEB_dump_obj__ (Typ_LN, ln, " ln:");
  // DEB_dump_obj__ (Typ_SURBSP, sbs, " sbs:");


  UME_init (&mem1Seg, memspc51, sizeof(memspc51));
  UME_init (&mem2Seg, memspc52, sizeof(memspc52));
  UME_init (&mem3Seg, memspc53, sizeof(memspc53));
  UME_init (&mem4Seg, memspc54, sizeof(memspc54));

  memSegT[0] = &mem1Seg;
  memSegT[1] = &mem2Seg;
  memSegT[2] = &mem3Seg;
  memSegT[3] = &mem4Seg;

  *nxp = 0;
  irc = USBS_IntsectLnBspSur (nxp, xpTab, xpSiz, ln, sbs, NULL, -1,
                     memSegT, tmpSpc1);
  if (irc < 0) return -1;


  return 0;

}


//================================================================
//================================================================
// Liste_TESTFUNKTIONEN:
//================================================================
//================================================================

/*

//================================================================
  int UT3D_testsurf (SurBSpl *sur, Memspc *workSeg) {
//================================================================
// UT3D_testsurf      B-spline surface patches

// UT3D_testsurf      Author: Thomas Backmeister       13.9.2003

// IN:
//   SurBSpl *sur    ... b-spline surface
//   Memspc *workSeg ... work space
// OUT:
// Returncodes:
//   0 = OK
//  -1 = internal error


  int rc, p, q;
  long nbsegU, nbsegV;
  double ua, ub, va, vb, s, t, u, v;
  Point pt;

  printf("\nUT3D_testsurf\n");

  nbsegU = sur->ptUNr - sur->degU;
  nbsegV = sur->ptVNr - sur->degV;
  printf("number of u-segments= %d\n", nbsegU);
  printf("number of v-segments= %d\n", nbsegV);


  for (q=0; q<nbsegV; ++q) {

    // v-segment start- and endparameter
    va = sur->kvTabV[sur->degV+q];
    vb = sur->kvTabV[sur->degV+q+1];

    for (p=0; p<nbsegU; ++p) {

      // u-segment start- and endparameter
      ua = sur->kvTabU[sur->degU+p];
      ub = sur->kvTabU[sur->degU+p+1];


      // ---- TEST (p,q)-PATCH ----
      printf("\n(p,q)-PATCH (%d, %d)\n", p, q);
      for (s=0.0; s<=1.0; s+=0.20) {
        v = (1.0-s) * va + s * vb;
        for (t=0.0; t<=1.0; t+=0.20) {
          u = (1.0-t) * ua + t * ub;
          rc = UT3D_pt_evparSurBSpl (&pt, sur, u, v, workSeg);
          if (rc < 0) return -1;
          printf("(u,v)= (%f,%f)   pt= %f %f %f\n",u,v,pt.x,pt.y,pt.z);
          //cre_obj (Typ_PT, Typ_PT, 1, (void*)&pt);
        }
      }
    }
  }

  return 0;
}

*/

//========================== EOF =============================
