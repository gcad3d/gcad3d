//            ut_project.c                      2007-08-11            RF
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
PT -> SURRU         offen UPRJ_app_pt
PT -> SURRV         offen UPRJ_app_pt

-----------------------------------------------------
Modifications:
  ..

-----------------------------------------------------
*/
/*!
\file  ../ut/ut_project.c
\brief project 
\code
=====================================================
List_functions_start:

UPRJ_def__           init projection with target & projectionDirection

UPRJ_app__           apply prj to struct (nicht ObjGX)
UPRJ_app_obj         apply transf. to to object (ObjGX-tree)
UPRJ_app_oTab        apply transf. to ObjGX-tabl
UPRJ_app_pt
UPRJ_app_vc
UPRJ_app_ci
UPRJ_app_el
UPRJ_app_plg
UPRJ_app_bsp

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


Ersetzt UTO_obj_appprj

prj_typ          type of targetObj (where to project);
prj_tg           targetObj
prj_vc           projectionDirection or vz of Plane


*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <stdio.h>                        /* printf ..   */
#include <stdlib.h>                       /* atoi ..     */
#include <string.h>                       /* strcmp ..   */
#include <math.h>                         /* sin ..      */


#include "../xa/xa_mem.h"                 // memspc*
#include "../ut/ut_geo.h"                 // Point-def ..
#include "../ut/ut_ox_base.h"             // OGX_SET_INDEX
#include "../ut/func_types.h"                  // UI_Func... SYM_..

static int    prj_typ;     // typ of targetObj; Typ_PLN or surface ...
static int    prj_nr;      // nr of objects in prj_tg
static void   *prj_tg;     // pointer to targetObj; typ=prj_typ.
static Vector prj_vc;      // projectionDirection





//================================================================
  int UPRJ_def__ (int typ, void *target, int oNr, Vector *vc1) {
//================================================================
/// \code
/// init projection with target & projectionDirection
/// INPUT:
///   typ     typ of projectionTarget; must be Typ_PLN
///   target  typ=Typ_PLN: (Plane*)
///           typ=Typ_SURMSH: (int*)target[0]=dbIndSurMesh;
///                                 target[1]=dbIndSurPtab.
///   vc1     projectionVector; if NULL: normal projection
/// see UTRA_def_...
/// \endcode


  // printf("UPRJ_def__ typ=%d oNr=%d\n",typ,oNr);
  // UT3D_stru_dump (typ, target, "target:");
  // UT3D_stru_dump (Typ_VC, vc1, "vc1:");


  prj_typ = typ;
  prj_tg  = target;
  prj_nr  = oNr;

  if(vc1) {
    prj_vc = *vc1;

  } else {      // Direction: normal
    if(prj_typ == Typ_PLN) {
      prj_vc = ((Plane*)prj_tg)->vz;
    } else {
      prj_vc = UT3D_VECTOR_NUL;
      // TX_Error("UPRJ_def__ E001");
      return -1;
    }
  }
  

  return 0;

}


//================================================================
  int UPRJ_app_pt (Point *p2, Point* p1) {
//================================================================
/// \code
/// apply projection to a point.
/// Input:
///   prj_typ extern; type of prj_tg
///   prj_tg  extern; where to project
///   p1      point to project onto prj_tg
/// Output
///   p2
///   retCod  1 = OK; -1=Error
/// see UTRA_app_pt UT3D_pt_projptpl UT3D_pt_intptvcpl_ UT3D_ptDi_intptvcpln
/// \endcode

// TODO: clot, ccv


  int      irc, i1, i2, i3, ii, sTyp;
  double   dist, da[16];
  Point    pt1, pa[16];
  Plane    pl1;
  Vector   vc1;
  Memspc   memTmp;
  ObjGX    *ox1, *ox2;
  void     *vp1, *cv1;
  CurvCCV  *ccva;


  // printf("UPRJ_app_pt prj_typ=%d\n",prj_typ);
  // UT3D_stru_dump (prj_typ, prj_tg, "target:");
  // UT3D_stru_dump (Typ_VC, &prj_vc, "  prj_vc:");


  //----------------------------------------------------------------
  if(prj_typ != Typ_LN) goto L_AC;
    // irc = UT3D_pt_projptln (p2, &dist, p1, prj_tg);
    UT3D_vc_2pt (&vc1, &((Line*)prj_tg)->p1, &((Line*)prj_tg)->p2);
    irc = UT3D_pt_projptptvc (p2, &dist, da, p1, &((Line*)prj_tg)->p1, &vc1);
    if(irc < 0) return -1;
    // check if point is on limited line from parameter and points
    irc = UT3D_par_ck_inObj__ (da[0], p2, &((Line*)prj_tg)->p1, 0., 
                                          &((Line*)prj_tg)->p2, 1.);
    if(!irc) return -1;
    return 1;


  //----------------------------------------------------------------
  L_AC:
  if(prj_typ != Typ_CI) goto L_CVPOL;
    irc = UT3D_pt_projptci (&pa[0], &pa[1], p1, prj_tg);
    if(irc < 0) return -1;
    // irc = UT3D_pt_ck_inCirc (prj_tg, p1, UT_TOL_cv);
    // if(irc < 0) return 0;
    i1 = 2;
    UT3D_pt_ci_lim_del (&i1, pa, da, (Circ*)prj_tg);
    *p2 = pa[0];
    return 1;


  //----------------------------------------------------------------
  L_CVPOL:
  if(prj_typ != Typ_CVPOL) goto L_CVELL;
    i1 = 16;   // maxPoints
    irc = UT3D_pt_projptplg (&i1, pa, da, prj_tg, p1);
      // printf(" irc=%d i1=%d\n",irc,i1);
      // for(i2=0; i2<i1; ++i2) 
      // UT3D_stru_dump (Typ_PT, &pa[i2], " pa[%d] - da=%lf",i2,&da[i2]);
    APT_set_modMax (i1);
    if(irc < 0) return -1;
    if(i1 < 1) return 0;
    // *p2 = pa[imod];
    // get nearest point
    *p2 = pa[UT3D_ipt_cknearn_npt (p1, pa, i1, APT_get_mod1())];
      // UT3D_stru_dump (Typ_PT, p2, " i1=%d p2:",i1);
    return 1;


  //----------------------------------------------------------------
  L_CVELL:
  if(prj_typ != Typ_CVELL) goto L_CVBSP;
    i1 = 16;   // maxPoints
    irc = UT3D_pt_projptel (&i1, pa, prj_tg, p1);
      // printf(" projptel-irc=%d i1=%d\n",irc,i1);
      // UT3D_stru_dump (Typ_PT, &pa[0], " p0:");
    APT_set_modMax (i1);
    if(irc < 0) return -1;
    if(i1 < 1) return 0;
    *p2 = pa[APT_get_mod1()];
    return 1;


  //----------------------------------------------------------------
  L_CVBSP:
  if(prj_typ != Typ_CVBSP) goto L_CVCLOT;
    // UME_init (&memTmp, memspc101, sizeof(memspc101));
    i1 = 16;   // maxPoints
    irc = UT3D_pt_projptbspl (&i1, pa, da, prj_tg, p1);
    APT_set_modMax (i1);
    if(irc < 0) return -1;
    if(i1 < 1) return 0;
    *p2 = pa[APT_get_mod1()];
    return 1;


  //----------------------------------------------------------------
  L_CVCLOT:
  if(prj_typ != Typ_CVCLOT) goto L_CVCCV;
    // UME_init (&memTmp, memspc101, sizeof(memspc101));
    i1 = 16;   // maxPoints
    irc = UT3D_pt_prjclotpt (&i1, pa, da, p1, prj_tg, UT_TOL_cv );
    if(irc < 0) return -1;
    if(i1 < 1) return 0;
    *p2 = pa[0];
    return 1;


  //----------------------------------------------------------------
  L_CVCCV:
  if(prj_typ != Typ_CVCCV) goto L_PLN;
    // keep prj_tg and prj_nr
    vp1 = prj_tg;
    i2 = prj_nr;
    // loop tru CCV & recurse ..
    ccva = prj_tg;
    ii = 0; // nr of output-points
    for(i1=0; i1<i2; ++i1) {
      // get space for curve
      i3 = UTO_siz_stru (ccva[i1].typ);
      cv1 = UME_alloc_tmp (i3);
      // make normal object of CCV[i1]; set prj_tg & prj_nr
      UTO_cv_cvtrm (&prj_typ, cv1, NULL, &ccva[i1]);
      prj_tg = cv1; 
      irc = UPRJ_app_pt (&pa[ii], p1);     // RECURS
      if(irc < 0) return -1;
      if(irc > 0) ii += irc;
        // printf(" _app_pt CCV[%d] irc=%d ii=%d\n",i1,irc,ii);
        // printf("--------------------------------------------- \n");
    }
    APT_set_modMax (ii); // set nr of solutions
    *p2 = pa[UT3D_ipt_cknearn_npt (p1, pa, ii, APT_get_mod1())];
    // *p2 = pa[APT_get_mod1()];
    return 1;



  //----------------------------------------------------------------
  // prj_tg=Plane; prj_vc
  L_PLN:
  if(prj_typ != Typ_PLN) goto L_SURMSH;
  irc = UT3D_ptDi_intptvcpln (p2, &dist, prj_tg, p1, &prj_vc);
  if(irc < 1) return -1;
    // UT3D_stru_dump (Typ_PT, p2, "  p2:");
  return 1;


  //----------------------------------------------------------------
  L_SURMSH:
  if(prj_typ != Typ_SURMSH) goto L_OGX;
  irc = MSH_pt_prjptmsh_ (p2, p1, ((int*)prj_tg)[0], ((int*)prj_tg)[1], UT_TOL_pt);
  if(irc < 0) return -1;
  return 1;


  //----------------------------------------------------------------
  L_OGX:  // surfaces are inside a ObjGX-Record
  if(prj_typ != Typ_ObjGX) goto E_NI;
  ox1 = (ObjGX*)prj_tg;
  

  //----------------------------------------------------------------
  if(ox1->typ != Typ_SURBSP) goto L_SUR;
  i1 = 8;
  UME_init (&memTmp, memspc201, sizeof(memspc201));
  irc = USBS_pt_projptsbsp (&i1, pa, da, &da[8], ox1->data,
                            p1, &memTmp);
  APT_set_modMax (i1);
  if(irc < 0) return -1;
  *p2 = pa[APT_get_mod1()];
  return 1;


  //----------------------------------------------------------------
  L_SUR:
  if(ox1->typ != Typ_SUR) goto E_NI;
  // i1 = UTO_ck_surfTyp (prj_tg);
    // printf(" surfTyp=%d\n",i1); // 162=Typ_SURTPS
  i1 = SUR_ck_typ (&sTyp, &ox2, &i2, ox1);
    // printf(" surfTyp=%d %d %d\n",sTyp,i1,i2); // 155=Typ_SURPLN
    // UT3D_stru_dump (Typ_ObjGX, ox2, "  suppSur ox2:");


  if(sTyp != Typ_SURPLN) goto E_NI;
  // get plane for SURPLN
  UT3D_ptvc_sur (&pt1, &pl1.vz, NULL, ox1);
    // UT3D_stru_dump (Typ_PT, &pt1, "  pt1:");
    // UT3D_stru_dump (Typ_VC, &pl1.vz, "  pl1.vz:");
  if(!UT3D_compvcNull(&prj_vc)) vc1 = prj_vc;
  else vc1 = pl1.vz;
    // UT3D_stru_dump (Typ_VC, &vc1, "  vc1:");
  UT3D_pl_ptpl (&pl1, &pt1);
  irc = UT3D_ptDi_intptvcpln (p2, &dist, &pl1, p1, &vc1);
  if(irc < 1 ) return -1;
    // UT3D_stru_dump (Typ_PT, p2, "  p2:");
  return 1;




  // Typ_SURRU, Typ_SURRV ?
  // irc = SUR_pt2_prjptsur (p2, p1, prj_tg);
  // i1 = 8;
  // UME_init (&memTmp, memspc201, sizeof(memspc201));
  // irc = USBS_pt_projptsbsp (&i1, pa, da, &da[8], ((ObjGX*)prj_tg)->data, 
                            // p1, &memTmp);
  // APT_set_modMax (i1);     
  // if(irc < 0) return -1;   
  // *p2 = pa[APT_get_mod1()];
  // return irc;


  //----------------------------------------------------------------
  E_NI:
  if(prj_typ == Typ_ObjGX) i1 = ((ObjGX*)prj_tg)->typ;
  else i1 = prj_typ;
  TX_Error("not implemented - UPRJ_app_pt %d",i1);
  return -1;

}


//================================================================
  int UPRJ_app_vc (Vector *v2, Vector *v1) {
//================================================================
/// apply projection to a Vector.
/// using plane prj_tg, prj_vc.

// korr. 2011-08-06 RF

  double   dist;
  Point    ptx;

  // UT3D_stru_dump (Typ_VC, v1, "UPRJ_app_vc:");
  // UT3D_stru_dump (Typ_VC, &prj_vc, "  prj_vc:");
  // UT3D_stru_dump (Typ_PLN, prj_tg, "  Plane:");
  // GR_Disp_vc (v1, NULL, 8, 1);


  // intersect line-plane
  // UT3D_ptDi_intptvcpln (&ptx, &dist, prj_tg, (Point*)v1, &prj_vc);
  // UT3D_vc_2pt (v2, &((Plane*)prj_tg)->po, &ptx);

  UT3D_pt_projptptnvc ((Point*)v2, (Point*)v1, (Point*)&UT3D_PT_NUL, &prj_vc);
  // UT3D_vc_2pt (v2, &UT3D_PT_NUL, &ptx);
  // memcpy(v2, &ptx, sizeof(Vector));  // copy


    // UT3D_stru_dump (Typ_PT, &ptx, "  ptx:");
    // GR_Disp_vc (v2, NULL, 9, 1);
    // UT3D_stru_dump (Typ_VC, v2, "ex UPRJ_app_vc:");

  return 0;

}


//================================================================
  int UPRJ_app_ci (void *obo, Circ* c1) {
//================================================================
/// \code
/// apply projection to a Circle.
///   RetCod = OutputObj is circ or ellipse
/// see UT3D_el_projcipl
/// \endcode

  int      typ;
  Point    pt1;
  Vector   vc1;
  Circ     *c2;
  CurvElli *e2;



  // UT3D_stru_dump (Typ_CI, c1, "UPRJ_app_ci:");

  e2 = obo;

  // transl pc
  UPRJ_app_pt (&e2->pc, &c1->pc);

  // minorAxis vb is the intersectionLine of Plane -circPlane
  UT3D_vc_perp2vc (&e2->vb, &((Plane*)prj_tg)->vz, &c1->vz);
    // UT3D_stru_dump (Typ_VC, &e2->vb, "_app_ci vb:");

  // check if vz is normal to projection-plane
  // jes: circle ..
  // check if planes are parallel:
  // if(UT3D_compvcNull(&e2->vb)) {
  if(UT3D_compvc0 (&e2->vb, UT_TOL_min1) == 1) { // 1:vc=0,0,0 planes parallel.
      // printf(" is circ ..\n");
    c2 = obo;
    UT3D_vc_2pt (&vc1, &c1->pc, &e2->pc);
    UT3D_ci_tracivc (c2, c1, &vc1);            // move circ
    // UT3D_el_ci (e2, &ch1);   // Circ -> Ellipse
      // UT3D_stru_dump (Typ_CI, e2, "_app_ci:");
    return Typ_CI;
  }

  // transl p1
  UPRJ_app_pt (&e2->p1, &c1->p1);

  if(UT3D_ck_ci360(c1) == 0)
    e2->p2 = e2->p1;             // yes, 360 deg: copy p1
  else                            // transl p2
    UPRJ_app_pt (&e2->p2, &c1->p2);

  // set length for vb
  UT3D_vc_setLength (&e2->vb, &e2->vb, fabs(c1->rad));

  // find direction of majorAxis
  UT3D_vc_perp2vc (&vc1, &e2->vb, &c1->vz);

  // point on Circ
  UT3D_pt_traptvclen (&pt1, &c1->pc, &vc1, fabs(c1->rad));

  // point > new plane
  UPRJ_app_pt (&pt1, &pt1);
  UT3D_vc_2pt (&e2->va, &e2->pc, &pt1);
    // UT3D_stru_dump (Typ_VC, &e2->va, "va:");



  L_done:
  e2->dir = DSIGN(c1->rad);
  e2->vz = ((Plane*)prj_tg)->vz;
  
    // UT3D_stru_dump (Typ_CVELL, e2, "Ell=");
    // GR_Disp_ell (e2, 9);

  return Typ_CVELL;

}


//================================================================
  int UPRJ_app_el (CurvElli *eo, CurvElli *ei) {
//================================================================
/// \code
/// Project an ellipse onto a plane, orthogonally.
/// see UT3D_el_projcipl
/// \endcode

  double   d1, d2, da, db;
  Point    ph, p1, p2, p3, p4, pa, pb;
  Vector   *plvz, vr;

      // GR_Disp_vc (&ei->va, &ei->pc, 7, 1);
      // GR_Disp_vc (&ei->vb, &ei->pc, 7, 1);
      // GR_Disp_pt (&ei->pc, SYM_STAR_S, ATT_COL_RED);

  // copy Elli; copy vz
  *eo = *ei;
    // UT3D_stru_dump (Typ_CVELL, eo, "Ell=");

  // transf elli
  UPRJ_app_pt (&eo->pc, &ei->pc);
    // GR_Disp_pt (&eo->pc, SYM_STAR_S, ATT_COL_BLACK);

  UPRJ_app_pt (&eo->p1, &ei->p1);
  if(UT3D_ck_el360(ei) == 0)
    eo->p2 = eo->p1;
  else
    UPRJ_app_pt (&eo->p2, &ei->p2);


  // UPRJ_app_vc (&eo->va, &ei->va);
  // UPRJ_app_vc (&eo->vb, &ei->vb);
  plvz = &((Plane*)prj_tg)->vz;
  eo->vz = *plvz;


  // construct ellipse-axes system rytz;
  // get the endpoints of the old axes in plane as p1, p2
  UT3D_pt_traptvc (&ph, &ei->pc, &ei->va);
  UPRJ_app_pt (&p1, &ph);
    // UT3D_stru_dump (Typ_PT, &p1, "p1=");
    // GR_Disp_pt (&p1, SYM_STAR_S, ATT_COL_RED);

  UT3D_pt_traptvc (&ph, &ei->pc, &ei->vb);
  UPRJ_app_pt (&p2, &ph);
    // UT3D_stru_dump (Typ_PT, &p2, "p2=");
    // GR_Disp_pt (&p2, SYM_STAR_S, ATT_COL_RED);


  // dist pc-p1 must be longer than pc-p2
  d1 = UT3D_lenq_PtPt (&ei->pc, &p1);
  d2 = UT3D_lenq_PtPt (&ei->pc, &p2);
    // printf(" d1=%lf d2=%lf\n",d1,d2);
  if(d1 < d2) MEM_swap__(&p1, &p2, sizeof(Point));


  // check if pc-p1-p2 is normal; yes: axes are OK
  if(UT3D_pt_ck_perp_2pt (&eo->pc, &p1, &p2, UT_TOL_min0)) {
    // va = pc - p1
    UT3D_vc_2pt (&eo->va, &eo->pc, &p1);
    // vb = pc - p2
    UT3D_vc_2pt (&eo->vb, &eo->pc, &p2);
    goto L_exit;
  }


  // rotate p1 90-deg around plane-vz
// TODO: eigene func f 90/180/270 deg !
  UT3D_pt_rotptptvcangr (&p3, &p1, &eo->pc, plvz, RAD_90);
    // GR_Disp_pt (&p3, SYM_STAR_S, ATT_COL_GREEN);

  // get p4= midpoint p3-p2
  UT3D_pt_mid2pt (&p4, &p3, &p2);
    // GR_Disp_pt (&p4, SYM_STAR_S, ATT_COL_GREEN);

  // d1 = distance p4 - pc
  d1 = UT3D_len_2pt (&p4, &eo->pc);

  // point pa = from p4 along vr length d1
  UT3D_pt_traptptlen (&pa, &p4, &p3, d1);
    // GR_Disp_pt (&pa, SYM_STAR_S, ATT_COL_BLUE);

  // point pb = from p4 along -vr length d1
  UT3D_pt_traptptlen (&pb, &p4, &p2, d1);
    // GR_Disp_pt (&pb, SYM_STAR_S, ATT_COL_BLUE);

  // major-axis-length da = distance p2-pa
  da = UT3D_len_2pt (&pa, &p2);

  // minor-axis-length db = distance p2-pb
  db = UT3D_len_2pt (&pb, &p2);

  // major-axis va = pc -> pb length da
  UT3D_vc_2ptlen (&eo->va, &eo->pc, &pb, da);
    // GR_Disp_vc (&eo->va, &eo->pc, 5, 1);

  // minor-axis vb = pc -> pa length db
  UT3D_vc_2ptlen (&eo->vb, &eo->pc, &pa, db);
    // GR_Disp_vc (&eo->vb, &eo->pc, 5, 1);


  L_exit:
    // printf(" angr in  va-vb = %lf\n",UT3D_angr_2vc__(&ei->va,&ei->vb));
    // printf(" angr out va-vb = %lf\n",UT3D_angr_2vc__(&eo->va,&eo->vb));
    // UT3D_stru_dump (Typ_CVELL, eo, "ex UPRJ_app_el:");
    // GR_Disp_ell (eo, 9);

  return 0;

}


//================================================================
  int UPRJ_app_plg (CurvPoly *cvo, CurvPoly *cvi, Memspc *objSpc) {
//================================================================
// apply projection to a Polygon

  int    i1, pNr;
  double d1, *da;
  Point  pt1, pt2, *pa1, *pa2;


  UT3D_stru_dump (Typ_CVPOL, cvi, "UPRJ_app_plg");

  *cvo = *cvi;      // copy ptNr, v0, v1, *lvTab

  pNr = cvi->ptNr;
  pa1 = cvi->cpTab;
    // GR_Disp_pTab (pNr, pa1, SYM_STAR_S, 2);


  // reserve Space for pNr points in objSpc
  pa2 = objSpc->next;
  if(UME_add (objSpc, pNr * sizeof(Point)) < 0) return -1;
  cvo->cpTab = pa2;


  // reserve Space for new lengthTable lvTab
  da = objSpc->next;
  if(UME_add (objSpc, pNr * sizeof(double)) < 0) return -1;
  cvo->lvTab = da;



  // project points
  for(i1=0; i1<pNr; ++i1) {
    UPRJ_app_pt (&pa2[i1], &pa1[i1]);
      // GR_Disp_pt (&pa2[i1], SYM_TRI_S, 2);
  }


  // create new lengthTable lvTab
  d1 = UT3D_plg_lvTab (da, pa2, pNr);       // create lvTab
    // for(i1=0;i1<pNr;++i1) printf(" lv[%d]=%lf\n",i1,da[i1]);
    // UT3D_stru_dump(Typ_CVPOL, objo, "plg-prj-tmp:");


  // check if plg is closed und untrimmed.
  if(UT3D_ck_plgclo(cvi) == 0) {
      // printf(" yes, closed ..\n");
    cvo->v0 = da[0];
    cvo->v1 = da[pNr-1];

  } else {

    // Start- und Endpunkt muessen neu errechnet werden !
    // dazu sind die Punkte zu transformieren und dann v0/v1 neu berechnen;
    UT3D_pt_evalplg (&pt1, cvi, cvi->v0);
    UPRJ_app_pt (&pt2, &pt1);
    UT3D_parplg_plgpt (&cvo->v0, &pt2, cvo);

    UT3D_pt_evalplg (&pt1, cvi, cvi->v1);
    UPRJ_app_pt (&pt2, &pt1);
    UT3D_parplg_plgpt (&cvo->v1, &pt2, cvo);
  }



  // UT3D_stru_dump (Typ_CVPOL, cvo, "Plg-out");
  // GR_Disp_pol (cvo, 9);

  return 0;

}


//================================================================
  int UPRJ_app_bsp (CurvBSpl *cvo, CurvBSpl *cvi, Memspc *objSpc) {
//================================================================
/// apply projection to a Polygon

  int    i1, pNr;
  Point  pt1, pt2, *pa1, *pa2;


  // UT3D_stru_dump (Typ_CVBSP, cvi, "UPRJ_app_bsp");


  // copy BSP (ptnr, deg, v0, v1, *kvTab) --> objo
  *cvo = *cvi;


  pNr = cvi->ptNr;
  pa1 = cvi->cpTab;
    // GR_Disp_pTab (i2, pa1, SYM_STAR_S, 2);


  // reserve Space for pNr points in objSpc
  pa2 = objSpc->next;
  if(UME_add (objSpc, pNr * sizeof(Point)) < 0) return -1;
  cvo->cpTab = pa2;


  // project points
  for(i1=0; i1<pNr; ++i1) {
    UPRJ_app_pt (&pa2[i1], &pa1[i1]);
      // GR_Disp_pt (&pa2[i1], SYM_TRI_S, 2);
  }

  // Start- und Endpunkt bleiben bestehen ..

  // UT3D_stru_dump (Typ_CVBSP, cvo, "Bsp-out");
  // GR_Disp_bsp (cvo, 9, objSpc);

  return 0;

}


//============================================================================
  int UPRJ_app__ (ObjGX *oxo,
                  int iNr, void *obji, int ityp,
                  Memspc *objSpc) {
//============================================================================
/// \code
/// apply projection to struct; NOT object.
/// init with UPRJ_def__
/// Storage:
///   Normale structs werden nach objo ausgegeben;
///   ObjGX-Records: ObjGX-Records werden nach objo ausgegeben; die
///     zugehoerigen Daten (structs) werden in objSpc gespeichert.
///
/// Extern Vars:
///   APT_prim_seg      solutionNr; 0=first.
/// Input:
///   ityp     form of obji (eg Typ_ObjGX for surfaces)
///   iNr      nr of objects to transform
///   objSpc   get memspc for objo from objo
/// Output:
///   objo     output-obj; data is in objSpc
///   retCod   0=OK; -1=Error.
/// \endcode

// see UTRA_app__
// see UTO_obj_tra_m3
// see UTO_ox_tra
// see UTO_obj_appprj

  int     irc, i1, i2, otyp;
  void    *objo;


  printf("UPRJ_app__ %d %d\n",ityp,iNr);
  UT3D_stru_dump (ityp, obji, "obji:");

  irc = 0;

  otyp = ityp;

  switch (ityp) {



    //================================================================
    case Typ_PT:
        // UT3D_stru_dump (Typ_PT, obji, "trPTi:");
      objo = objSpc->next;
      if(UME_add (objSpc, iNr * sizeof(Point)) < 0) goto L_EOM;
      L_PT_nxt:
      irc = UPRJ_app_pt (objo, obji);
        // UT3D_stru_dump (Typ_PT, objo, "trPTo:");
        // printf(" app_tra PT siz=%d\n",*oSiz);
      if(iNr > 1) {  // obji hat mehrere points !
        --iNr;
        obji = (char*)obji + sizeof(Point);
        objo = (char*)objo + sizeof(Point);
        goto L_PT_nxt;
      }
      break;


    //================================================================
    case Typ_LN:
      if(iNr > 1) goto L_E_INR;
      objo = objSpc->next;
      if(UME_add (objSpc, sizeof(Line)) < 0) goto L_EOM;
        // UT3D_stru_dump (Typ_LN, obji, "trLNi:");
      UPRJ_app_pt (&((Line*)objo)->p1, &((Line*)obji)->p1);
      UPRJ_app_pt (&((Line*)objo)->p2, &((Line*)obji)->p2);
        // UT3D_stru_dump (Typ_LN, objo, "trLNo:");
        // printf("  UPRJ_app__ LN siz=%d\n",*oSiz);
      break;


    //================================================================
    case Typ_CI:
      if(iNr > 1) goto L_E_INR;
      objo = objSpc->next;
      if(UME_add (objSpc, sizeof(CurvElli)) < 0) goto L_EOM;  // elli > circ !
      otyp = UPRJ_app_ci (objo, obji);
      break;


    //================================================================
    case Typ_CVELL:
      if(iNr > 1) goto L_E_INR;
      objo = objSpc->next;
      if(UME_add (objSpc, sizeof(CurvElli)) < 0) goto L_EOM;
      irc = UPRJ_app_el (objo, obji);
      break;


    //================================================================
    case Typ_CVPOL:
      if(iNr > 1) goto L_E_INR;
      objo = objSpc->next;
      if(UME_add (objSpc, sizeof(CurvPoly)) < 0) goto L_EOM;
      irc = UPRJ_app_plg (objo, obji, objSpc);
      break;


    //================================================================
    case Typ_CVBSP:
      if(iNr > 1) goto L_E_INR;
      objo = objSpc->next;
      if(UME_add (objSpc, sizeof(CurvBSpl)) < 0) goto L_EOM;
      irc = UPRJ_app_bsp (objo, obji, objSpc);
      break;


    //================================================================
    default:
      TX_Error("UPRJ_app__ E001 %d",ityp);
      return -1;
  }

  if(irc < 0) return -1;

  OGX_SET_OBJ (oxo, otyp, otyp, 1, objo);
    // printf("ex UPRJ_app__ irc=%d oTyp=%d\n",irc,otyp);
    // UTO_dump__ (oxo, "ex UPRJ_app__ irc=%d");
  return irc;


  //----------------------------------------------------------------
  L_E002:
    TX_Error("UPRJ_app__ E002 %d",ityp);
    return -1;

  L_EOM:
    TX_Error("UPRJ_app__ EOM");
    return -1;

  L_E_INR:
    TX_Error("UPRJ_app__ E_INR %d %d",ityp,iNr);
    return -1;


  err_not_yet:
   TX_Error("UPRJ_app__ function not yet implemented");
      return -1;
}


/* UNUSED
//===========================================================================r
  int UPRJ_app_obj (void *objo,int *oSiz,void *obji,Memspc *objSpc) {
//============================================================================
/// \code
/// UNUSED - see UPRJ_app__
/// apply projection to struct or object.
/// init mit UPRJ_def_.. erforderlich !!!
/// Storage:
///   Normale structs werden nach objo ausgegeben;
///   ObjGX-Records: ObjGX-Records werden nach objo ausgegeben; die
///     zugehoerigen Daten (structs) werden in objSpc gespeichert.
///
/// Input:
///   oSiz   size of objo; wird recursiv verkleinert !
///   obji   (ObjGX*)  (NUR (ObjGX*) moeglich !)
/// Output:
///   oSiz   freier Restplatz in objo
///   objSpc fuer Daten (structs) von oGX-Objekten
/// \endcode

// see UTRA_app_obj UTO_obj_tra_m3 oder UTO_ox_tra

  int   irc, oldSiz, iForm;
  char  *pi;


  // printf("UPRJ_app_obj %d\n",*oSiz);
  // printf("         obji-typ=%d form=%d siz=%d\n",((ObjGX*)obji)->typ,
            // ((ObjGX*)obji)->form,((ObjGX*)obji)->siz);
  // UTO_dump_s_ (obji, "UPRJ_app_obj in");
  // UTO_dump__ (obji, "UPRJ_app_obj in");


  iForm = ((ObjGX*)obji)->form;


  // duplicate/isolate object; dataStructAddresses bleiben !
  oldSiz = *oSiz;
  irc = UTO_isol__ (objo, oSiz, obji);
  if(irc < 0) return -1;
  pi = objo;
  // (char*)objo +=  oldSiz - *oSiz;
  objo =  (char*)objo + (oldSiz - *oSiz);


  // UTO_dump_s_ (pi, "Obj isolated");
  // UTO_dump__ (pi, "Obj isolated");
  // UTO_obj_Disp__ (pi, &objSpc, 2, SYM_STAR_S);


  // oberste struct ist ein ObjGX;
  irc = UPRJ_app_oTab (objo, oSiz, pi, objSpc);

  // UTO_dump__ (pi, "UPRJ_app_obj out");
  // UTO_dump_s_ (pi, "UPRJ_app_obj out");

  return irc;
}
*/

/* UNUSED
//============================================================================
  int UPRJ_app_oTab (void *objo,int *oSiz,void *obji,Memspc *objSpc) {
//============================================================================
/// \code
/// einen oGX-Block resolven
/// Input:
///   obji parentRecord (ObjGX-Record) dessen form ebenfalls oGX ist
/// \endcode

// see  UTRA_app_oTab DB_store_obj


  int    irc, i1, i2, iTyp, iForm, iSiz, oldSiz, siz;
  char   *pi;


  iSiz  = ((ObjGX*)obji)->siz;

  // printf("UPRJ_app_oTab %d\n",*oSiz);
  // printf("         obji-typ=%d form=%d siz=%d\n",((ObjGX*)obji)->typ,
            // ((ObjGX*)obji)->form,iSiz);

  iTyp  = ((ObjGX*)obji)->typ;
  iForm = ((ObjGX*)obji)->form;
  pi    = ((ObjGX*)obji)->data;


  if(iForm != Typ_ObjGX) {
    oldSiz = *oSiz;
    irc = UPRJ_app__ (objo, oSiz, &i1, iSiz, pi, iForm, objSpc);
    if(irc < 0) return -1;
    ((ObjGX*)obji)->data = objo;
    return 0;
  }


  // ist ein ObjGX;
  // loop tru obj and translate ..
  for(i1=0; i1<iSiz; ++i1) {
    iTyp  = ((ObjGX*)pi)->typ;
    iForm = ((ObjGX*)pi)->form;
    // printf(" tra2[%d] %d %d\n",i1,iTyp,iForm);

    if(iTyp == Typ_Typ) goto L_GX_nxt; // hat keine data !!

    if(iForm == Typ_ObjGX) { // recurse
      oldSiz = *oSiz;
      irc = UPRJ_app_oTab (objo, oSiz, pi, objSpc);
      if(irc < 0) return -1;
      // (char*)objo +=  oldSiz - *oSiz;
      objo =  (char*)objo + (oldSiz - *oSiz);


    // normale struct: transl.
    } else {
      oldSiz = *oSiz;
      siz = ((ObjGX*)pi)->siz;
      irc = UPRJ_app__ (objo,oSiz,&i2, siz, ((ObjGX*)pi)->data,iForm, objSpc);
      if(irc < 0) return -1;
      ((ObjGX*)pi)->data = objo;
      // (char*)objo +=  oldSiz - *oSiz;
      objo =  (char*)objo + (oldSiz - *oSiz);
    }


    L_GX_nxt:
    pi += sizeof(ObjGX);
  }


  return 0;

}
*/

//==================  EOF  =======================================
 
