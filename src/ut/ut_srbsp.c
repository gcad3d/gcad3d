//          Rat.B-Spline-Surface-Functions    RF.
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
2004-06-01 neu ex ../ut/ut_bspl.c. RF.

-----------------------------------------------------
*/
/*!
\file  ../ut/ut_srbsp.c
\brief Rat.B-Spline-Surface-Functions 
\code
=====================================================
List_functions_start:

UT3D_pt_evparsrbsp        point <-- evaluate surface parameter
UT3D_ptgrid_srbsp         Grundraster generieren

UT3D_parsrbsp_pt          u/v-Parameter from  3D-Rat.B-SplineSurfPoint
UT3D_uvNr_srbsp           U/V-tolerance (see also SUSbsp_ck_tol)

UT3D_srbsp_ck_closed      check if srb cyclic in X
UT3D_rbsp_srbspU          get U-Basiscurve from rat.B-Spline
UT3D_rbsp_srbspV          get V-Basiscurve from rat.B-Spline

UT3D_cv_srbspout          outline (polygon) of rat.B-SplSurf
UT3D_cv_srbspU            Isocurve (polygon) from U-wert
UT3D_cv_srbspV            Isocurve (polygon) from V-wert

UT3D_uparmid_srbsp        mittleren U-Wert
UT3D_vparmid_srbsp        mittleren V-Wert

UT3D_obj_cnvt_srbsp       convert Rat.B-SplSur --> RuledSurf / Torus

------------------------- testfunctions:
UT3D_srbsp_tst_dspicv     disply isocurves
UT3D_srbsp_tst_load       load Surf.

List_functions_end:
=====================================================
- see also:
SUSbsp_ck_tol             optimale Anzahl von Punkten in U/V-Richtung ermitteln

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
#include "../ut/gr_types.h"               // SYM_* ATT_* Typ_Att_* LTYP_*
#include "../ut/func_types.h"            // SYM.
#include "../ut/ut_memTab.h"           // MemTab

#include "../gr/ut_gr.h"               // GR_tDyn_pcv
#include "../xa/xa.h"                  // aus_SIZ
#include "../xa/xa_mem.h"              // memspc51, mem_cbuf1





//=====================================================================
  int UT3D_obj_cnvt_srbsp (ObjGX *opo, SurRBSpl *spi, Memspc *mpWrk) {
//=====================================================================
// UT3D_obj_cnvt_srbsp          convert Rat.B-SplSur --> RuledSurf
// circ + line: convert to ruled surface (circ + circ + RuledSurf)
// circ + circ: convert to Torus
// RetCod: 0  OK, rbspl converted.
//        -1  No; cannot convert rbspl.

  int       irc, i1;
  double    d1, a1, a2;
  CurvRBSpl s1;
  ObjGX     *oTab, *op;
  char      *cBuf;

  void      *vp1;
  Circ      c1, *cp1;
  Point     pAx, p1;
  Vector    vAx;


  // printf("====================================== UT3D_obj_cnvt_srbsp in:\n");
  // DEB_dump_obj__ (Typ_SURRBSP, spi, "");
  // return -1;  // TEST ONLY
  irc = -1;


  // get spc for 2 ObjGX
  oTab = (ObjGX*)mpWrk->next;
  irc = UME_add (mpWrk, sizeof(ObjGX)*2);
  if(irc < 0) return -1;



  // Wenn ptUNr==2 oder ptVNr==2 koennte man es durch RuledSurf abbilden.
  // test ob u == linear
  if(spi->ptUNr != 2) goto L_ru_ckv;
  // yes; u hat nur 2 Punkte !
  // check, ob V aus 2 Kreisen besteht ...


  // load V-curve U=0
  irc = UT3D_rbsp_srbspV (&s1, 0, spi, mpWrk);
  if(irc < 0) return -1;
    // UT3D_rbspl_tst_cc (&s1);  // display


  // try to convert curve -> circ
  irc = UT3D_obj_cnvt_rbspl (&oTab[0], &s1, mpWrk);
  if(irc != Typ_CI) return -1;

  // load curve U=1
  irc = UT3D_rbsp_srbspV (&s1, 1, spi, mpWrk);
  if(irc < 0) return -1;
    // UT3D_rbspl_tst_cc (&s1);  // display

  // try to convert curve -> circ
  irc = UT3D_obj_cnvt_rbspl (&oTab[1], &s1, mpWrk);
  if(irc != Typ_CI) return -1;

  goto L_sru_OK;



  //================================================================
  // test ob v == linear
  L_ru_ckv:
  if(spi->ptVNr != 2) goto L_tor_0;
  // yes; v hat nur 2 Punkte !
  // check, ob U aus 2 Kreisen besteht ...
  TX_Error("UT3D_obj_cnvt_srbsp I001");
  return -1;




  //================================================================
  L_sru_OK:
  irc = 0;

  opo->typ   = Typ_SURRU;
  opo->form  = Typ_ObjGX;
  opo->siz   = 2;
  opo->dir   = 0;
  opo->data  = (void*)oTab;

  goto L_exit;




  //================================================================
  //================================================================
  // check for Torus
  L_tor_0:
  if(spi->ptUNr < 3) return -1;
  if(spi->ptVNr < 3) return -1;


  //----------------------------------------------------------------
  // check ob alle Kurven Kreise sind und auf parallelen Ebenen liegen;
  // wenn ja: Achse aus den Mittelpunkten machen.

  // check if V-Curves are circs and their z-vecs are parallel
    // printf("\n\n L_tor_ck0:   check if V-Curves\n");
  vp1 = mpWrk->next;
  for(i1=0; i1<spi->ptUNr; ++i1) {
    // reset memspc (remove old circs)
    mpWrk->next = vp1;
    // get isoCurve s1 from surf
    irc = UT3D_rbsp_srbspV (&s1, i1, spi, mpWrk);
    if(irc < 0) return -1;
    // try to convert curve -> circ
    irc = UT3D_obj_cnvt_rbspl (&oTab[0], &s1, mpWrk);
    if(irc != Typ_CI) return -1;
    // save pc & vz
    if(oTab[0].typ != Typ_CI) return -1;
    cp1 = oTab[0].data;
    if(i1 < 1) {         // copy centerPt & axis of first circ
      pAx = cp1->pc;
      vAx = cp1->vz;
    } else {
      // check if Vectors are parallel
      if(UT3D_vc_ck_parl_vc(&vAx, &cp1->vz, RAD_1) == 0) goto L_tor_ck1U;
      // check if points are straight; take extreme points.
      // printf(" check straight ...\n");
      UT3D_pt_projptptvc (&p1, &d1, NULL, &cp1->pc, &pAx, &vAx);
      // printf(" d1=%f\n",d1);
      if(d1 > UT_TOL_pt) goto L_tor_ck1U;
    }
  }
  goto L_tor_ck20;



  // check if U-Curves are circs and their z-vecs are parallel
  L_tor_ck1U:
    // printf("\n\n L_tor_ck1U: check if U-Curves\n");
  for(i1=0; i1<spi->ptVNr; ++i1) {
    // reset memspc (remove old circs)
    mpWrk->next = vp1;
    // get isoCurve s1 from surf
    irc = UT3D_rbsp_srbspU (&s1, i1, spi);
    if(irc < 0) return -1;
    // try to convert curve -> circ
    irc = UT3D_obj_cnvt_rbspl (&oTab[0], &s1, mpWrk);
    if(irc != Typ_CI) return -1;
    // save pc & vz
    if(oTab[0].typ != Typ_CI) return -1;
    cp1 = oTab[0].data;
    if(i1 < 1) {
      pAx = cp1->pc;
      vAx = cp1->vz;
    } else {
      // check if Vectors are parallel
      if(UT3D_vc_ck_parl_vc(&vAx, &cp1->vz, RAD_1) == 0) return -1;
      // check if points are straight; take extreme points.
      // printf(" check straight ...\n");
      UT3D_pt_projptptvc (&p1, &d1, NULL, &cp1->pc, &pAx, &vAx);
      // printf(" d1=%f\n",d1);
      if(d1 > UT_TOL_pt) return -1;
    }
  }
  goto L_tor_ck21;






  //----------------------------------------------------------------
  L_tor_ck20:
    // printf("\n\n L_tor_ck20:\n");
  // check ob alle Kurven Kreise sind und die Achse in der Kreisebene liegt

  vp1 = mpWrk->next;
  for(i1=0; i1<spi->ptVNr; ++i1) {
    // reset memspc (remove old circs)
    mpWrk->next = vp1;
    // try to convert curve -> circ
    irc = UT3D_rbsp_srbspU (&s1, i1, spi);
    if(irc < 0) return -1;
    // try to convert curve -> circ
    irc = UT3D_obj_cnvt_rbspl (&oTab[0], &s1, mpWrk);
    if(irc != Typ_CI) return -1;
    if(oTab[0].typ != Typ_CI) return -1;
    cp1 = oTab[0].data;
    // Kreis und Achse muessen in der gleichen Ebene liegen
    d1 = UT3D_slen_2ptvc (&cp1->pc, &pAx, &cp1->vz);
    if(d1 > UT_TOL_pt) return -1;
    if(i1 < 1) c1 = *cp1;
  }

  // ja; U-Kurven sind Kreise um die Achse pAx/vAx;
  goto L_tor_OK;




  L_tor_ck21:
    // printf("\n\n L_tor_ck21:\n");
  vp1 = mpWrk->next;
  for(i1=0; i1<spi->ptUNr; ++i1) {
    // reset memspc (remove old circs)
    mpWrk->next = vp1;
    // try to convert curve -> circ
    irc = UT3D_rbsp_srbspV (&s1, i1, spi, mpWrk);
    if(irc < 0) return -1;
    // try to convert curve -> circ
    irc = UT3D_obj_cnvt_rbspl (&oTab[0], &s1, mpWrk);
    if(irc != Typ_CI) return -1;
    if(oTab[0].typ != Typ_CI) return -1;
    cp1 = oTab[0].data;
    // Kreis und Achse muessen in der gleichen Ebene liegen
    d1 = UT3D_slen_2ptvc (&cp1->pc, &pAx, &cp1->vz);
    if(d1 > UT_TOL_pt) return -1;
    if(i1 < 1) c1 = *cp1;
  }
  // ja; V-Kurven sind Kreise um die Achse pAx/vAx;




  //----------------------------------------------------------------
  L_tor_OK:
    // printf("\n\n L_tor_OK: \n");
  // Achse ist pAx,vAx;   Konturkreis ist c1, Hauptkreis ist cp1
  // den Startwinkel a1 feststellen (vom c1)
  a1 = 0.;
  // den Endwinkel a2 feststellen (vom cp1)
  a2 = UT3D_angr_vc2pt (&pAx, &vAx, &cp1->p1, &cp1->p2);
    // printf(" a1=%f a2=%f\n",a1,a2);
  if(a2 < RAD_01) a2 = RAD_360;



  // Axis u Konturkreis als embedded Text erzeugen ; zB
  //     A20=SRV L(P(0 0 0) P(1 0 0)) C(P(10 75 0) 10 DZ) 0 90
  //     A= SRV <Axis> <Circ> <ang1> <ang2>
  // Var: Axis u Konturkreis als dynam.Objekte speichern, SurRev* erzeugen.
  //     see APT_decode_srv

  // reserve 256 bytes in mpWrk
  cBuf = mpWrk->next;
  irc = UME_add (mpWrk, 256);
  if(irc < 0) return -1;

  strcpy(cBuf, "SRV ");     // see AP_stru_2_txt
  // add axis pAx vAx
  // AP_obj_add_ln
  UT3D_pt_traptvc (&p1, &pAx, &vAx);
  AP_obj_add_ln (cBuf, 0, &pAx, &p1);
  // add circ c1
  AP_obj_add_ci_ (cBuf, &c1);
  // add angles
  AP_obj_add_val (cBuf, UT_DEGREES(a1));
  AP_obj_add_val (cBuf, UT_DEGREES(a2));
    // printf(" _srbsp-SRV |%s|\n",cBuf);

  opo->typ  = Typ_SUR;
  opo->form = Typ_Txt;
  opo->siz  = strlen(cBuf) + 1;
  opo->data = cBuf;

  irc = 0;


  //================================================================
  L_exit:

  // TESTOUT:
  // printf("ex UT3D_obj_cnvt_srbsp |%s|\n",cBuf);
  // if(irc == 0) DEB_dump_ox_s_ (opo, "ex UT3D_obj_cnvt_srbsp");
  // TESTOUT:

  return irc;

}


/*=======================================================================*/
  int UT3D_pt_evparsrbsp (Point *pt, SurRBSpl *sur, double u, double v,
                            Memspc *workSeg) {
/*========================
UT3D_pt_evparsrbsp    point <-- evaluate surface parameter

Evaluate a point on a Rat.b-spline surface.

IN:
  SurRBSpl *sur   ... rat.b-spline surface
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

  int       rc, i1, v1;
  void      *memstart;
  double    *wTab, wp;
  Point     *qTab;
  CurvRBSpl rbspl;


  // printf("zzzzzzzzzz  UT3D_pt_evparsrbsp %f,%f\n",u,v);
  // DEB_dump_obj__ (Typ_SURRBSP, sur, "");


  // space for v-curve control points
  memstart = workSeg->next;

  qTab = workSeg->next;
  rc = UME_add (workSeg, sizeof(Point) * sur->ptVNr);
  if (rc < 0) goto L_outOfWorkSpace;

  wTab = workSeg->next;
  rc = UME_add (workSeg, sizeof(double) * sur->ptVNr);
  if (rc < 0) goto L_outOfWorkSpace;



  // eine isocurve mit par. u generieren.
  // init b-spline u-curve
  rbspl.ptNr  = sur->ptUNr;
  rbspl.deg   = sur->degU;
  rbspl.kvTab = sur->kvTabU;
  rbspl.v0    = sur->kvTabU[0];
  rbspl.v1    = sur->kvTabU[sur->ptUNr+sur->degU];

  // Kontrollpunkte qTab der Curve mit uPar=u errechnen
  for (v1=0; v1<sur->ptVNr; ++v1) {

    // control points of u-curve
    rbspl.cpTab = &(sur->cpTab[v1*sur->ptUNr]);
    rbspl.wTab  = &(sur->wTab[v1*sur->ptUNr]);

    // evaluate point on u-curve
    rc = UT3D_pt_evparCrvRBSpl (&(qTab[v1]), &wTab[v1], &rbspl, u);
    if (rc < 0) return -2;
  }



  // init b-spline v-curve
  rbspl.ptNr  = sur->ptVNr;
  rbspl.deg   = sur->degV;
  rbspl.kvTab = sur->kvTabV;
  rbspl.cpTab = qTab;
  rbspl.wTab  = wTab;

  // evaluate point on v-curve
  rc = UT3D_pt_evparCrvRBSpl (pt, &wp, &rbspl, v);
  if (rc < 0) return -2;

  // release work space
  workSeg->next = memstart;

  // DEB_dump_obj__ (Typ_PT, pt, "ex UT3D_pt_evparsrbsp %f %f:",u,v);
    // exit(0);
  return 0;


L_outOfWorkSpace:
  TX_Error("UT3D_pt_evparsrbsp: out of work space");
  return -1;
}


//================================================================
  int UT3D_cv_srbspout (int *pNr, Point *pTab, SurRBSpl *sur,
                        Memspc *wrkSeg) {
//================================================================
// UT3D_cv_srbspout          outline (polygon) of rat.B-SplSurf
// see GR_sSym_sbsp

  int       i1, p1Nr, pMax;
  CurvRBSpl cv1;


  pMax = *pNr;
  *pNr = 0;


  // DEB_dump_obj__ (Typ_SURBSP, sur, "UT3D_cv_srbspout %d\n",pMax);


  //----------------------------------------------------------------
  // randKurve U0
  UT3D_rbsp_srbspU (&cv1, 0, sur);

  // CurvRBSpl -> polygon
  i1 = UT3D_cv_rbsp (&p1Nr, pTab, NULL, &cv1, pMax, UT_DISP_cv/3.);
  if(i1 < 0) return -1;
  pTab = &pTab[p1Nr];
  pMax -= p1Nr;
  *pNr += p1Nr;


  //----------------------------------------------------------------
  // randKurve V-aussen
  i1 = sur->ptUNr-1;
  i1 = UT3D_rbsp_srbspV (&cv1, i1, sur, wrkSeg);

  // CurvRBSpl -> polygon
  i1 = UT3D_cv_rbsp (&p1Nr, pTab, NULL, &cv1, pMax, UT_DISP_cv/3.);
  if(i1 < 0) return -1;
  pTab = &pTab[p1Nr];
  pMax -= p1Nr;
  *pNr += p1Nr;


  //----------------------------------------------------------------
  // randKurve U-aussen
  i1 = sur->ptVNr-1;
  UT3D_rbsp_srbspU (&cv1, i1, sur);

  // curve umdrehen !  (see UTO_stru_inv)
  MEM_swap_2db (&cv1.v0, &cv1.v1);

  // CurvRBSpl -> polygon
  i1 = UT3D_cv_rbsp (&p1Nr, pTab, NULL, &cv1, pMax, UT_DISP_cv/3.);
  if(i1 < 0) return -1;
  pTab = &pTab[p1Nr];
  pMax -= p1Nr;
  *pNr += p1Nr;


  //----------------------------------------------------------------
  // randKurve V0
  i1 = UT3D_rbsp_srbspV (&cv1, 0, sur, wrkSeg);

  // curve umdrehen !  (see UTO_stru_inv)
  MEM_swap_2db (&cv1.v0, &cv1.v1);

  // CurvRBSpl -> polygon
  i1 = UT3D_cv_rbsp (&p1Nr, pTab, NULL, &cv1, pMax, UT_DISP_cv/3.);
  if(i1 < 0) return -1;
  pTab = &pTab[p1Nr];
  pMax -= p1Nr;
  *pNr += p1Nr;


  return 0;

}


//=======================================================================
  int UT3D_cv_srbspU (int pNr, Point *pTab, double uu, SurRBSpl *sbs,
                      Memspc *workSeg) {
//=======================================================================
// UT3D_cv_srbspU            Isocurve (polygon) from U-wert
// see UT3D_ptgrid_srbsp


  int      ii, irc;
  double   vMin, vMax, vv, dv;


  // printf("UT3D_ptgrid_srbsp %d %d\n",iu,iv);


  vMin = sbs->kvTabV[0];
  vMax = sbs->kvTabV[sbs->ptVNr + sbs->degV];
    // printf("UT3D_ptgrid_srbsp uMin=%f uMax=%f  vMin=%f vMax=%f\n",
           // uMin, uMax, vMin, vMax);


  dv = (vMax - vMin) / (pNr - 1);

  vv = vMin;

  vMax += dv / 2.;

  ii = 0;


  L_nxt_v:

  irc = UT3D_pt_evparsrbsp (&pTab[ii], sbs, uu, vv, workSeg);
  if (irc < 0) return -1;
  ++ii;
    // printf("(u,v)= (%f,%f)   pt= %f %f %f\n",uu,vv,
           // pTab[ii].x,pTab[ii].y,pTab[ii].z);
    // APT_disp_SymB (SYM_TRI_S, 2, &pTab[ii]);

  vv += dv;
  if(vv < vMax) goto L_nxt_v;

  // printf("ex UT3D_ptgrid_srbsp du=%f dv=%f\n",*du,*dv);

  return 0;

}


//================================================================
  int UT3D_cv_srbspV (int pNr, Point *pTab, double vv, SurRBSpl *sbs,
                      Memspc *workSeg) {
//================================================================
// UT3D_cv_srbspV            Isocurve (polygon) from V-wert
// see UT3D_ptgrid_srbsp

  int      ii, irc;
  double   uMin, uMax, vMin, vMax, uu, du;


  // printf("UT3D_ptgrid_srbsp %d %d\n",iu,iv);


  uMin = sbs->kvTabU[0];
  uMax = sbs->kvTabU[sbs->ptUNr + sbs->degU];
    // printf("UT3D_ptgrid_srbsp uMin=%f uMax=%f  vMin=%f vMax=%f\n",
           // uMin, uMax, vMin, vMax);


  du = (uMax - uMin) / (pNr - 1);

  uu = uMin;

  uMax += du / 2.;

  ii = 0;


  L_nxt_u:
  irc = UT3D_pt_evparsrbsp (&pTab[ii], sbs, uu, vv, workSeg);
  if (irc < 0) return -1;
  ++ii;
    // printf("(u,v)= (%f,%f)   pt= %f %f %f\n",uu,vv,
           // pTab[ii].x,pTab[ii].y,pTab[ii].z);
    // APT_disp_SymB (SYM_TRI_S, 2, &pTab[ii]);

  uu += du;
  if(uu < uMax) goto L_nxt_u;

  // printf("ex UT3D_ptgrid_srbsp du=%f dv=%f\n",*du,*dv);

  return 0;

}


//================================================================
  int UT3D_uparmid_srbsp (double *pm, SurRBSpl *sbs) {
//================================================================
// mittleren U-Wert

  double   uMin, uMax;

  uMin = sbs->kvTabU[0];
  uMax = sbs->kvTabU[sbs->ptUNr + sbs->degU];

  *pm = (uMin + uMax) / 2.;

  // printf(" ex UT3D_uparmid_srbsp %f\n",*pm);
  return 0;

}


//================================================================
  int UT3D_vparmid_srbsp (double *pm, SurRBSpl *sbs) {
//================================================================
// mittleren v-Wert

  double   vMin, vMax;

  vMin = sbs->kvTabV[0];
  vMax = sbs->kvTabV[sbs->ptVNr + sbs->degV];

  *pm = (vMin + vMax) / 2.;

    // printf(" ex UT3D_vparmid_srbsp %f\n",*pm);

  return 0;

}

 
//======================================================================
  int UT3D_ptgrid_srbsp (Point *pTab, double *du, double *dv,
                        SurRBSpl *sbs, int iu, int iv, Memspc *workSeg) {
//======================================================================
// Ein Grundraster mit gleichmaessigen Parameterabstaenden generieren
// In:  iu, iv: Anzahl der gewuenschten Punkte in u/v-Richtung
// Out: pTab - iu * iv Punkte
//      *du, *dv - der Parameterabstand


  int      ii, irc;
  double   uMin, uMax, vMin, vMax, uu, vv;


  // printf("UT3D_ptgrid_srbsp %d %d\n",iu,iv);


  uMin = sbs->kvTabU[0];
  vMin = sbs->kvTabV[0];
  uMax = sbs->kvTabU[sbs->ptUNr + sbs->degU];
  vMax = sbs->kvTabV[sbs->ptVNr + sbs->degV];
    // printf("UT3D_ptgrid_srbsp uMin=%f uMax=%f  vMin=%f vMax=%f\n",
           // uMin, uMax, vMin, vMax);


  *du = (uMax - uMin) / (iu - 1);
  *dv = (vMax - vMin) / (iv - 1);

  uu = uMin;
  vv = vMin;

  uMax += *du / 2.;
  vMax += *dv / 2.;

  ii = 0;


  L_nxt_u:
  irc = UT3D_pt_evparsrbsp (&pTab[ii], sbs, uu, vv, workSeg);
  if (irc < 0) return -1;
  ++ii;
    // printf("(u,v)= (%f,%f)   pt= %f %f %f\n",uu,vv,
           // pTab[ii].x,pTab[ii].y,pTab[ii].z);
    // APT_disp_SymB (SYM_TRI_S, 2, &pTab[ii]);

  uu += *du;
  if(uu < uMax) goto L_nxt_u;

  uu = uMin;
  vv += *dv;
  if(vv < vMax) goto L_nxt_u;

  // printf("ex UT3D_ptgrid_srbsp du=%f dv=%f\n",*du,*dv);

  return 0;

}


//===========================================================================
  int UT3D_parsrbsp_pt (double *up, double *vp,
                        Point *pti,
                        Point *pTab, double dui, double dvi, int uNr, int vNr,
                        SurRBSpl *sbs, Memspc *wrkSpc) {
//===========================================================================
// ACHTUNG: ganz gleich wie UT3D_parsbsp_pt

// IN: pti - die u/v-Parameter dieses Punktes (*up, *vp) ermitteln
//     pTab[uNr,vNr] - Gitterraster auf der B-SplineSurf (UT3D_ptgrid_sbsp)
//     du,dv - entspricht dem Parameterabstand der Gitterpunkte
//     uMin = sbs->kvTabU[0];  vMin = sbs->kvTabV[0];
//
// Den kuerzesten Abstand zu pti im Gitterraster suchen; dann mit dem
// umliegenden Rechteck im Gitterraster beginnen; die relative Abweichung
// um Testviereck errechnen und ein neues Testviereck definieren.


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
  GR_tDyn_npt__ (uNr, pTab, SYM_TRI_S, 1);
  GR_tDyn_npt__ (uNr, &pTab[uNr], SYM_TRI_S, 2);
  GR_tDyn_npt__ (uNr, &pTab[uNr*2], SYM_TRI_S, 3);
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
  // GR_tDyn_pcv (pa, 4, 2);
  // GR_Disp_pt (&pa[0], SYM_TRI_S, 2);  GR_tDyn_txtA(&pa[0], " 0", 2);
  // GR_Disp_pt (&pa[1], SYM_TRI_S, 2);  GR_tDyn_txtA(&pa[1], " 1", 2);
  // GR_Disp_pt (&pa[2], SYM_TRI_S, 2);  GR_tDyn_txtA(&pa[2], " 2", 2);
  // GR_Disp_pt (&pa[3], SYM_TRI_S, 2);  GR_tDyn_txtA(&pa[3], " 3", 2);
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
      UT3D_2angr_vc__ (&az, &ay, &vcz);
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
  UT3D_pt_evparsrbsp (&pa[0], sbs, u0, v0, wrkSpc);
    // GR_Disp_pt (&pa[0], SYM_TRI_S, 2);
    // GR_tDyn_txtA(&pa[0], "0", 2);

  u1 = uc + du;
  v1 = vc - dv;
  UT3D_pt_evparsrbsp (&pa[1], sbs, u1, v1, wrkSpc);
    // GR_Disp_pt (&pa[1], SYM_TRI_S, 2);
    // GR_tDyn_txtA(&pa[1], "1", 2);

  u2 = uc + du;
  v2 = vc + dv;
  UT3D_pt_evparsrbsp (&pa[2], sbs, u2, v2, wrkSpc);
    // GR_Disp_pt (&pa[2], SYM_TRI_S, 2);
    // GR_tDyn_txtA(&pa[2], "2", 2);

  u3 = uc - du;
  v3 = vc + dv;
  UT3D_pt_evparsrbsp (&pa[3], sbs, u3, v3, wrkSpc);
    // GR_Disp_pt (&pa[3], SYM_TRI_S, 2);
    // GR_tDyn_txtA(&pa[3], "3", 2);
    // GR_tDyn_pcv (pa, 4, 3);


  if(iloop < 16) goto L_next_try;  // Anzahl Versuche Defaul=100
  // if(iloop < 2) goto L_next_try;  // Anzahl Versuche Defaul=100

  // keine Loesung nach n Versuchen ..
  // printf("UT3D_parsbsp_pt I000 Pos %f,%f,%f\n",pti->x,pti->y,pti->z);
  // TX_Error("UT3D_parsbsp_pt I000");
  // exit(0);


  //----------------------------------------------------------------
  L_FERTIG:
  *up = uc;
  *vp = vc;
  // printf("ex UT3D_parsbsp_pt uc=%f vc=%f\n\n",*up,*vp);


  return 0;

}


//=======================================================================
  int UT3D_uvNr_srbsp (int *iu, int *iv, SurRBSpl *sur, Memspc *wrkSeg) {
//=======================================================================
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
  CurvRBSpl rbspl;


  // space for v-curve control points
  memstart = wrkSeg->next;



  // compute U-tolerance
  rMin = UT_VAL_MAX;
  lTot = 0.;
  // i1 = 0; // TEST
  for(i1=0; i1<sur->ptVNr; ++i1) {
    // get rat.B-Spline U-curve from rat.B-SplSurf-ControlCurve
    UT3D_rbsp_srbspU (&rbspl, i1, sur);

    // // display ControlPoints/Curve:
    // UT3D_rbspl_tst_cc (&rbspl);

    // Crv hat nur 2 Punkte; somit muessen alle U-Kurven 2 Punkte haben..
    if(rbspl.ptNr < 3) { *iu = 2; goto L_V0;}

    // vom gesamten Polgon den kleinsten Radius errechnen
    // es muss von jeweils 3 Punkten der Kreisradius errechnet werden
    UT3D_rMin_pta (&d1, rbspl.ptNr, rbspl.cpTab);
    if(d1 < rMin) rMin = d1;

    // Gesamtlaenge errechnen
    d1 = 0.;
    for(i2=1; i2<rbspl.ptNr;  ++i2) {
      d1 += UT3D_len_2pt (&rbspl.cpTab[i2-1], &rbspl.cpTab[i2]);
    }
    if(d1 > lTot) lTot = d1;
  }

    // printf(" rMin=%f lTot=%f\n",rMin,lTot);
  *iu = lTot / (UT2D_len_ciSec (UT_DISP_cv, rMin) * 1.);   // sollte * 2 ?



  // compute V-tolerance
  L_V0:
  rMin = UT_VAL_MAX;
  lTot = 0.;
  for(i1=0; i1<sur->ptUNr; ++i1) {
    // get rat.B-Spline U-curve from rat.B-SplSurf-ControlCurve
    irc = UT3D_rbsp_srbspV (&rbspl, i1, sur, wrkSeg);
    if(irc < 0) goto L_eom;

    // // display ControlPoints/Curve:
    // UT3D_rbspl_tst_cc (&rbspl);

    // Crv hat nur 2 Punkte; somit muessen alle U-Kurven 2 Punkte haben..
    if(rbspl.ptNr < 3) { *iv = 2; goto L_done;}

    // vom gesamten Polgon den kleinsten Radius errechnen
    // es muss von jeweils 3 Punkten der Kreisradius errechnet werden
    UT3D_rMin_pta (&d1, rbspl.ptNr, rbspl.cpTab);
    if(d1 < rMin) rMin = d1;

    d1 = 0.;
    for(i2=1; i2<rbspl.ptNr;  ++i2) {
      d1 += UT3D_len_2pt (&rbspl.cpTab[i2-1], &rbspl.cpTab[i2]);
    }
    if(d1 > lTot) lTot = d1;

    // reset Workspace
    wrkSeg->next = memstart;
  }

    // printf(" rMin=%f lTot=%f\n",rMin,lTot);
  *iv = lTot / (UT2D_len_ciSec (UT_DISP_cv, rMin) * 1.);  // sollte * 2 ?


  L_done:
  if(*iu < 2) *iu = 2;
  if(*iv < 2) *iv = 2;

  // printf("ex UT3D_uvNr_srbsp iu=%d iv=%d\n",*iu,*iv);

  return 0;


  L_eom:
  TX_Print("UT3D_uvNr_srbsp EOM");
  return -1;

}


//=========================================================
  int UT3D_srbsp_ck_closed (SurRBSpl *su1) {
//=========================================================
// UT3D_srbsp_ck_closed         check if srb cyclic in X
// irc = 0 = sbs is NOT closed (normal)
// irc = 1 0 sbs is closed

// see also UT3D_sbs_ck_closed

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
  // printf("ex UT3D_srbsp_ck_closed %d\n",irc);

  return irc;

}


//================================================================
  int UT3D_rbsp_srbspU (CurvRBSpl *rbspl, int vNr, SurRBSpl *sur) {
//================================================================
// get rat.B-Spline U-Curve from rat.B-SplSurf-ControlCurve
// nur Basiscurve liefern; kein Isocurve errechnen.
// see also UT3D_pt_evparsrbsp



  // eine isocurve mit par. u generieren.
  // init b-spline u-curve
  rbspl->ptNr  = sur->ptUNr;
  rbspl->deg   = sur->degU;
  rbspl->kvTab = sur->kvTabU;
  rbspl->v0    = sur->kvTabU[0];
  rbspl->v1    = sur->kvTabU[sur->ptUNr+sur->degU];

  // control points of u-curve
  rbspl->cpTab = &(sur->cpTab[vNr*sur->ptUNr]);
  rbspl->wTab  = &(sur->wTab[vNr*sur->ptUNr]);

  // DEB_dump_obj__ (Typ_CVRBSP, rbspl, "ex UT3D_rbsp_srbspU");

  return 0;

}


//================================================================
  int UT3D_rbsp_srbspV (CurvRBSpl *rbspl, int uNr, SurRBSpl *sur,
                        Memspc *wrkSeg) {
//================================================================
// get rat.B-Spline V-Curve from rat.B-SplSurf-ControlCurve
// nur Basiscurve liefern; kein Isocurve errechnen.
// see also UT3D_pt_evparsrbsp
// cpTab und wTab werden im wrkSpc abgelegt !

  int    i1, i2;
  Point  *pta;
  double *wa;

  // printf("UT3D_rbsp_srbspV %d\n",uNr);

  // eine isocurve mit par. u generieren.
  // init b-spline u-curve
  rbspl->ptNr  = sur->ptVNr;
  rbspl->deg   = sur->degV;
  rbspl->kvTab = sur->kvTabV;
  rbspl->v0    = sur->kvTabV[0];
  rbspl->v1    = sur->kvTabV[sur->ptVNr+sur->degV];

  // reserve space for ptVNr Point and weights
  rbspl->cpTab = wrkSeg->next;
  i1 = UME_add (wrkSeg, sur->ptVNr * sizeof(Point));
  if(i1 < 0) return -1;

  rbspl->wTab  = wrkSeg->next;
  i1 = UME_add (wrkSeg, sur->ptVNr * sizeof(double));
  if(i1 < 0) return -1;


  // control points of u-curve
  pta = rbspl->cpTab;
  wa  = rbspl->wTab;
  for(i1=0; i1<sur->ptVNr; ++i1) {
    i2 = uNr + (sur->ptUNr * i1);
    // printf(" i1=%d i2=%d\n",i1,i2);
    pta[i1] = sur->cpTab[i2];
    wa[i1]  = sur->wTab[i2];
  }

  // DEB_dump_obj__ (Typ_CVRBSP, rbspl, "ex UT3D_rbsp_srbspV");

  return 0;

}



//================================================================
//================================================================
//    TESTFUNKTIONEN:
//================================================================
//================================================================


#ifdef DEB

//================================================================
  int UT3D_srbsp_tst_dspicv (SurRBSpl *sur) {
//================================================================
 
  int       u1, v1, u0, v0;
  double    du, dv;
  Point     *pta;
  Memspc    tbuf1, wrkSeg;
  ObjGX     su1;
  CurvRBSpl rbspl;


  printf("UT3D_srbsp_tst_dspicv \n");


  UME_init (&tbuf1, memspc501, sizeof(memspc501));  // hier die sur-daten
  UME_init (&wrkSeg, memspc201, sizeof(memspc201));


  // eine Rat.B-Spline Surf. laden
  UT3D_srbsp_tst_load (&su1, &tbuf1);
  // DEB_dump_obj__ (Typ_SURRBSP, su1.data, "");
  sur = su1.data;
  

  pta = (Point*)memspc55;


  //================================================================
  // die Kontrollpunkte als isoLinien darstellen ..
  for(u1=0; u1<sur->ptVNr; ++u1) {
    // u1 = 0; // Test
    u0 = u1 * sur->ptUNr;
    for(v1=0; v1<sur->ptUNr; ++v1) {
      pta[v1] = sur->cpTab[u0 + v1];
    }
    GR_tDyn_pcv (pta, sur->ptUNr, 2);   // 9=rot
  }


  for(v1=0; v1<sur->ptUNr; ++v1) {
    // v1 = 0; // Test
    for(u1=0; u1<sur->ptVNr; ++u1) {
      u0 = u1 * sur->ptUNr;
      pta[u1] = sur->cpTab[u0 + v1];
    }
    GR_tDyn_pcv (pta, sur->ptVNr, 1);    // 8=gruen
  }



  //================================================================
  // Ein Grundraster mit gleichmaessigen Parameterabstaenden generieren
  UT3D_ptgrid_srbsp (pta, &du, &dv, sur, 8,  8, &wrkSeg);

  GR_tDyn_nsymB (64, pta, SYM_STAR_S, 4);


  // //================================================================
  // // einzelne Punkte testen ...
  // UT3D_pt_evparsrbsp (pta, sur, 0., 0., &wrkSeg);

  // GR_tDyn_npt__ (1, pta, SYM_STAR_S, 4);



  return 0;

}


//================================================================
  int UT3D_srbsp_tst_load (ObjGX *ox1, Memspc *tbuf1) {
//================================================================
// eine Rat.B-Spline Surf. laden
// usage see UT3D_srbsp_tst_dspicv

  char      *p1;

  char sr[]="RBSP,9,5,2,2,0.063,0,0.06305,0.063,-0.063,0.06305,0,-0.063,0.06305,-0.063,-0.063,0.06305,-0.063,0,0.06305,-0.063,0.063,0.06305,0,0.063,0.06305,0.063,0.063,0.06305,0.063,0,0.06305,0.063,0,0.05905,0.063,-0.063,0.05905,0,-0.063,0.05905,-0.063,-0.063,0.05905,-0.063,0,0.05905,-0.063,0.063,0.05905,0,0.063,0.05905,0.063,0.063,0.05905,0.063,0,0.05905,0.059,0,0.05905,0.059,-0.059,0.05905,0,-0.059,0.05905,-0.059,-0.059,0.05905,-0.059,0,0.05905,-0.059,0.059,0.05905,0,0.059,0.05905,0.059,0.059,0.05905,0.059,0,0.05905,0.055,0,0.05905,0.055,-0.055,0.05905,0,-0.055,0.05905,-0.055,-0.055,0.05905,-0.055,0,0.05905,-0.055,0.055,0.05905,0,0.055,0.05905,0.055,0.055,0.05905,0.055,0,0.05905,0.055,0,0.06305,0.055,-0.055,0.06305,0,-0.055,0.06305,-0.055,-0.055,0.06305,-0.055,0,0.06305,-0.055,0.055,0.06305,0,0.055,0.06305,0.055,0.055,0.06305,0.055,0,0.06305,0,0,0,1.570796,1.570796,3.141593,3.141593,4.712389,4.712389,6.283185,6.283185,6.283185,0,0,0,1.570796,1.570796,3.141593,3.141593,3.141593 1 0.707106781 1 0.707106781 1 0.707106781 1 0.707106781 1 0.707106781 0.5 0.707106781 0.5 0.707106781 0.5 0.707106781 0.5 0.707106781 1 0.707106781 1 0.707106781 1 0.707106781 1 0.707106781 1 0.707106781 0.5 0.707106781 0.5 0.707106781 0.5 0.707106781 0.5 0.707106781 1 0.707106781 1 0.707106781 1 0.707106781 1 0.707106781 1";




int       aus_anz;
int       *aus_typ;
double    *aus_tab;
static ObjAto    ato1;

// aus_SIZ aus xa.h

// man sollte extern aus_SIZ = 400 setzen !

  p1 = sr;
  // aus_anz = APT_decode_ausdr (aus_typ, aus_tab, 400, &p1);
  // ATO_getSpc_tmp__ (&ato1, 400);
  ATO_malloc (&ato1, 400);

  ATO_ato_srcLn__ (&ato1, p1);
  aus_anz = ato1.nr;
  aus_typ = ato1.typ;
  aus_tab = ato1.val;
    printf(" aus_anz=%d\n",aus_anz);


  APT_decode_sur (ox1, aus_anz, aus_typ, aus_tab, tbuf1);

  ATO_free (&ato1);


  return 0;

}

#endif
 
//========================== EOF =============================
