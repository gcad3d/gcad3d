//  symbolic surfaces.                           Reiter Franz 2006-03-09
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
 GL_sSym_spl - Planaren Flaechen symbolisch darstellen  / 'Schraffuralg.'
 Dzt nur Aussenkontur und Innenkonturen;
 OFFEN: eine Box ueber die Aussenkontur; ein Kreuz durch die
   Boxmitte, das Kreuz mit GLU begrenzen und ausgeben.
   (try to draw lines statt contours wie in GLT_comm_work)
   intersect line polygon
   Oder immer Streifen machen; Streifenbreite = Schraffurabstand.
   Streifen liefert also 2 Linien plus die Aussenkontur dazwischen.
   Ev kann man sich zu den Aussenkonturelementen Flags liefern lassen ?

-----------------------------------------------------
Modifications:
  ..

-----------------------------------------------------
*/
/*!
\file  ../gr/gl_sSym.c
\brief display surfaces symbolic
\code
=====================================================
List_functions_start:

GL_sSym_srus         RuledSurfaces / supported (nicht getrimmt)
GL_sSym_spl          trimmed/punched surfs
GL_sSym_srv          RevolvedSurface
GL_sSym_sbsp
GL_sSym_srbsp

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



*/

#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>


#include "../ut/ut_geo.h"
#include "../ut/ut_ox_base.h"             // OGX_SET_INDEX
#include "../ut/ut_obj.h"                 // UTO_obj_load
#include "../ut/func_types.h"                  // UI_Func... SYM_..
#include "../ut/ut_memTab.h"           // MemTab

#include "../db/ut_DB.h"                  // DB_GetObjGX
#include "../gr/ut_GL.h"                  //

#include "../xa/xa_mem.h"                 // memspc51..



// ex ../ci/NC_Main.c
extern long      AP_dli_act;      // index dispList


// aus ../gr/ut_GLU.c:
extern Point     *GLT_pta;
extern long      GLT_pta_SIZ;


// aus ../gr/tess_su.c:
extern int TSU_mode;   // 0=normal darstellen; 1=speichern



//===========================================================================
  int GL_sSym_srus (Point *p1Tab, Point *p2Tab, int ptNr, int sTyp,
                   int att, long ind) {
//===========================================================================
// RuledSurfaces / supported (nicht getrimmt)
// sTyp  0  beide p1Tab befuellt mit mind. 2 Punkten
//       1  p1Tab hat nur 1 Punkt
//       2  p2Tab hat nur 1 Punkt
// USING: memspc53

  int    i0, i1, i2, ii, ip, pMax;
  long   dli;
  Point  *pTab;
  ObjGX  oo, cvTab[6];


  // printf("GL_sSym_srus ptNr=%d sTyp=%d %d\n",ptNr,sTyp,ind);
    // GR_Disp_pTab (ptNr, p1Tab, SYM_TRI_S, 4);
    // GR_Disp_pTab (ptNr, p2Tab, SYM_TRI_S, 5);


  if(TSU_mode != 0) return 0;  // tessel -> mem: nothing to do.

  ii = -1;

  // check if p1Tab is a point
  // if(UT3D_comp2pt(&p1Tab[0],&p1Tab[ptNr-1],UT_TOL_cv) == 1) goto L_c2;
  if(sTyp == 1) goto L_c2;
  ++ii;
  // cvTab[ii] = UTO_obj_load (Typ_PT, Typ_PT, ptNr, p1Tab);
  OGX_SET_OBJ (&cvTab[ii], Typ_PT, Typ_PT, ptNr, p1Tab);



  // check if p2Tab is a point
  L_c2:
  // if(UT3D_comp2pt(&p2Tab[0],&p2Tab[ptNr-1],UT_TOL_cv) == 1) goto L_c3;
  if(sTyp == 2) goto L_c3;
  ++ii;
  // cvTab[ii] = UTO_obj_load (Typ_PT, Typ_PT, ptNr, p2Tab);
  OGX_SET_OBJ (&cvTab[ii], Typ_PT, Typ_PT, ptNr, p2Tab);



  // mittelLinie
  L_c3:
  pTab = (Point*)memspc53;    // ACHTUNG: TSU_DrawSurTRU hat memspc55,memspc54
  pMax = sizeof(memspc53) / sizeof(Point);
  if(pMax < (ptNr+6)) goto L_eom;

  i1 = 0;
  i2 = 0;
  for(i0=0; i0<ptNr; ++i0) {
    if(sTyp != 1) i1 = i0;
    if(sTyp != 2) i2 = i0;
    UT3D_pt_mid2pt (&pTab[i0], &p1Tab[i1], &p2Tab[i2]);
  }
  ++ii;
  // cvTab[ii] = UTO_obj_load (Typ_PT, Typ_PT, ptNr, pTab);
  OGX_SET_OBJ (&cvTab[ii], Typ_PT, Typ_PT, ptNr, pTab);


  // 1. querlinie
  pTab = &pTab[ptNr];
  pTab[0] = p1Tab[0];
  pTab[1] = p2Tab[0];
  ++ii;
  // cvTab[ii] = UTO_obj_load (Typ_PT, Typ_PT, 2, pTab);
  OGX_SET_OBJ (&cvTab[ii], Typ_PT, Typ_PT, 2, pTab);


  // 2. querlinie
  pTab = &pTab[2];
  i1 = ptNr-1; if(sTyp == 1) i1 = 0;
  pTab[0] = p1Tab[i1];
  i2 = ptNr-1; if(sTyp == 2) i2 = 0;
  pTab[1] = p2Tab[i2];
  ++ii;
  // cvTab[ii] = UTO_obj_load (Typ_PT, Typ_PT, 2, pTab);
  OGX_SET_OBJ (&cvTab[ii], Typ_PT, Typ_PT, 2, pTab);

  // mittlere querlinie
  pTab = &pTab[2];
  if(sTyp == 1) i1 = 0;
  else          i1 = ptNr / 2;
  if(sTyp == 2) i2 = 0;
  else          i2 = ptNr / 2;
    // printf(" mql: %d %d\n",i1,i2);
  if(i1 != 1) pTab[0] = p1Tab[i1];         // Line hat nur 2 Punkte !!
  else UT3D_pt_mid2pt (&pTab[0], &p1Tab[0], &p1Tab[1]);
  if(i2 != 1)  pTab[1] = p2Tab[i2];
  else UT3D_pt_mid2pt (&pTab[1], &p2Tab[0], &p2Tab[1]);
  ++ii;
  // cvTab[ii] = UTO_obj_load (Typ_PT, Typ_PT, 2, pTab);
  OGX_SET_OBJ (&cvTab[ii], Typ_PT, Typ_PT, 2, pTab);




  //----------------------------------------------------------------
  // use GL_Draw_obj here; see GL_tst_pta ..
  L_out:
  oo.typ  = Typ_GL_CV;
  oo.form = Typ_ObjGX;
  oo.siz  = ii+1;
  oo.data = cvTab;


  dli = DL_StoreObj (Typ_SUR, ind, att);

  GL_Draw_obj (&dli, Typ_Att_dash_long, &oo);

  AP_dli_act = dli;


  // printf("ex GL_sSym_srus\n");
  return 0;



  L_eom:
    TX_Error("GL_sSym_srus EOM");
    return -1;

}


//================================================================
  int GL_sSym_spl (ContTab *cTab, int cNr, ObjGX *ox1, int att, long ind) {
//================================================================
// for trimmed/punched surfs.
// ACHTUNG: bei BSP-surfs (und RBP-surs ?) kommen keine Aussenkonturen;
//   in diesem Fall ist (cTab[0].pa == NULL) !!
// TODO: make line trough contour, but do do not cross boundaries/holes.


  int    irc, i1, ii, cvMax, sTyp, pNr, pMax, rNr;
  long   dli;
  void   *sur;
  Point  *pTab;
  ObjGX  oo, *cvTab;
  Memspc wrkSeg;


  // printf("ssssssssssssssssssssssssssssssssssssssssssssssssssss \n");
  // printf("GL_sSym_spl %d %ld %d %d\n",cNr,ind,ox1->typ,ox1->form);
  // for(i1=0; i1<cNr; ++i1) {
    // if(cTab[i1].pa == NULL) {printf("skip DUMMY!!\n");continue;}
    // printf("  [%d] iNr=%d\n",i1,cTab[i1].iNr);
    // GR_Disp_pTab (cTab[i1].iNr, cTab[i1].pa, SYM_TRI_S, 4);
  // }
  // return 0;



  if(TSU_mode != 0) return 0;  // tessel -> mem: nothing to do.



  //----------------------------------------------------------------
  // use GL_Draw_obj here; see GL_tst_pta ..
  
  // ACHTUNG: TSU_DrawSurTC:memspc55,memspc501,memspc101 used.
  //          TSU_DrawSurTP:memspc501,memspc201 used.
  cvTab = (ObjGX*)memspc54;
  cvMax = sizeof(memspc54) / sizeof(ObjGX);
  if(cvMax < cNr) goto L_eom;



  L_out:
  ii = 0;
  for(i1=0; i1<cNr; ++i1) {
    if(cTab[i1].pa == NULL) {
      // printf("skip DUMMY!!\n");
      UME_init (&wrkSeg, memspc102, sizeof(memspc102));
      pTab = (Point*)memspc53;
      pMax = sizeof(memspc53) / sizeof(Point);
      pNr = pMax;
      // get sur-record
      sTyp = UTO_objDat_ox (&sur, &rNr, ox1);
      if(sTyp != Typ_ObjGX) {TX_Error("GL_sSym_spl E001"); return -1;}
      // create polygon <= outLine of suppSur
      if(((ObjGX*)sur)->form == Typ_SURBSP) {
        irc = UT3D_cv_sbspout (&pNr, pTab, ((ObjGX*)sur)->data, &wrkSeg);
        if(irc < 0) return -1;
      } else if(((ObjGX*)sur)->form == Typ_SURRBSP) {
        irc = UT3D_cv_srbspout (&pNr, pTab, ((ObjGX*)sur)->data, &wrkSeg);
        if(irc < 0) return -1;
      } else {TX_Error("GL_sSym_spl E002"); return -1;}
      cTab[i1].iNr = pNr;
      cTab[i1].pa = pTab;
    }
    // printf(" %d add %d\n",i1,cTab[i1].iNr);
    cvTab[ii].typ  = Typ_PT;
    cvTab[ii].form = Typ_PT;
    cvTab[ii].siz  = cTab[i1].iNr;
    cvTab[ii].data = cTab[i1].pa;
    ++ii;
  }

  oo.typ  = Typ_GL_CV;
  oo.form = Typ_ObjGX;
  oo.siz  = ii;
  oo.data = cvTab;
    // DEB_dump_ox_0 (&oo, "sSym_spl-1 dbi=%ld:",ind);


  dli = DL_StoreObj (Typ_SUR, ind, att);

  GL_Draw_obj (&dli, Typ_Att_hili, &oo);

  AP_dli_act = dli;


  // printf("ex GL_sSym_spl\n");

  return 0;


  L_eom:
    TX_Error("GL_sSym_spl EOM");
    return -1;

}


//================================================================
  int GL_sSym_srv (ObjGX *oxi, int att, long ind) {
//================================================================
// draw symbolic display of RevolvedSurface
// RevolvedSurface from axis and contour.
// contour CANNOT be normal to axis;
//   eg contour of cylinder is line parallel to axis.
// Examples: "SRV L() L() ANG(a1) ANG(a2)"               // cyl
//           "SRV L(%s) C(P(C%d) %s) P(C%d) P(C%d) CW"   // torus

// display 3 circles starting at StartPoint/midPoint/endPoint ?


  int     irc, i1, ii, ip, pcNr, ppNr[6], pcMax, cTyp, rNr;
  long    dli, l1, dbi;
  double  d1, a1, am, a2, v0, vm, v1;
  void    *oc;
  Point   pta, *pcTab, *ppTab[6], p1, p2, pm, pc;
  Vector  vca;
  Circ    *cii, cic, ci1;
  ObjGX   oo, oxo, oci, oc1, oc2, cvTab[6];  // *ocp;   // oco
  CurvCCV oco;
  ObjDB   odbi;
  SurRev  *srv1;
  Memspc  wrkSeg;
  void    *cvCov;



  // printf("GL_sSym_srv %d\n",ind);

  if(TSU_mode != 0) return 0;  // tessel -> mem: nothing to do.



  UME_init (&wrkSeg, memspc201, sizeof(memspc201));

  pcTab = (Point*)memspc55;
  pcMax = sizeof(memspc55) / sizeof(Point);


  srv1 = oxi->data;    // get srv1 = revSur
  a1 = srv1->ang1;     // startAngle
  a2 = srv1->ang2;     // endAngle
  v0 = srv1->v0;       // startParameter along axis
  v1 = srv1->v1;       // endParameter along axis


  // invert direction if dir=revers
  if(((SurRev*)oxi->data)->dir) {
    MEM_swap_2db (&a1, &a2);
  }


  // set angles min -2Pi max +2Pi
  UT2D_2angr_set (&a1, &a2, 0);


  // get am = angle in the middle of the contour
  vm = (v0 + v1) / 2.;
  am = (a1 + a2) / 2.;   UT2D_2angr_set (&am, &am, 0);
    // printf(" ang1=%f am=%f ang2=%f v0=%f vm=%f v1=%f\n",a1,am,a2,v0,vm,v1);


  // get oci = axis-obj
  // printf("Cen-typ=%d ind=%d\n",srv1->typCen, srv1->indCen);
  oci = DB_GetObjGX ((int)srv1->typCen, srv1->indCen);
  if(oci.typ == Typ_Error) return -1;


  // get rotAxis point pta + Vector vca
  i1 = UT3D_ptvc_ox (&pta, &vca, &oci);
  if(i1 < 0) return i1;
    // DEB_dump_obj__(Typ_VC, &vca, " srv-vca:");


  // get (ObjDB) odbi = DB-obj contourelement
  ODB_set_odb (&odbi, (int)srv1->typCov, srv1->indCov);


  // get cTyp = type of contourelement
  // ACHTUNG vermutlich muss man hier auch TSU_srv_tor_01 benutzen uva ...
  cTyp = srv1->typCov;

  // get cvCov = data-struct of contourelement
  UTO_obj_dbo (&cvCov, &rNr, &cTyp, srv1->indCov);



  // if contourelement = Circ then 
  if(cTyp == Typ_CI) {

    cii = (Circ*)cvCov;
      // DEB_dump_obj__(Typ_CI, cii, " srv-cont1:");

    // Circ-vz may not be parallel vca
    if(UT3D_comp2vc_d(&vca, &cii->vz, UT_TOL_min1)) {
      TX_Error("GL_sSym_srv E1-parameter-error");
      return -1;
    }

    // get pta = CircCenter projected onto axis pta-vca
    UT3D_pt_projptptvc (&pta, &d1, NULL, &cii->pc, &pta, &vca);
      // GR_Disp_pt (&pta, SYM_STAR_S, 4);
    // SEE ALSO TSU_srv_tor_03 !

    if(UT3D_ck_ci360((Circ*)cvCov) == 0) {
      // yes, 360DegCirc
      // den Z-Vektor setzen
      cic = *cii;
      UT3D_vc_perpvc2pt (&cic.vz, &vca, &pta, &cic.pc);
      UT3D_pt_traptptlen (&cic.p1, &cic.pc, &pta, fabs(cic.rad));
      cic.p2 = cic.p1;
        // DEB_dump_obj__(Typ_CI, &cic, " srv-cont2:");
        // GR_Disp_ac (&cic, 9);
        // GR_Disp_pt (&cic.p1, SYM_STAR_S, 3);
      dbi = DB_StoreCirc (-1L, &cic);
      ODB_set_odb (&odbi, Typ_CI, dbi);
      // oci.typ   = Typ_CI;
      // oci.form  = Typ_CI;
      // oci.data  = &cic;
      // ocp = (void*)&cic;  // for midPoint ..
    }
  }

  
  // -----------------------------------
  // get oc1 = 1. cutting obj:
  oc1.typ  = Typ_Par1; // Typ_LN; // Typ_Val;
  oc1.form = Typ_Float8; // Typ_Index;  // Typ_Val;
  oc1.data = &v0; // (void*)(long)21L; // &v0;
  oc1.siz  = 1;

  // get oc2 = 2. cutting obj:
  oc2.typ  = Typ_Par1; // Typ_LN; // Typ_Val;
  oc2.form = Typ_Float8; // Typ_Index;  // Typ_Val;
  oc2.data = &v1; // (void*)(long)22L; // &v1;
  oc2.siz  = 1;


  // oco = Konturkurve holen und trimmen
  // irc = UTO_obj_trim (&oco, &oci, &oc1, 0, &oc2, 0, &wrkSeg);
  irc = UTO_CUT__ (&oco, &odbi, &oc1, &oc2, 0, &wrkSeg);
  // irc = UTO_CUT__ (&oco, &oci, &oc1, &oc2, 0, &wrkSeg);
  if(irc < 0) return irc;
  // ObjGX from CurvCCV
  OGX_SET_OBJ (&oxo, Typ_CVTRM, Typ_CVTRM, 1, &oco);
    // UTO_obj_Disp__ (&oco, &wrkSeg, 9);
    // DEB_dump_ox_0 (&oco, "contour:");


  // oxo in Polygon pcTab umwandeln ...
  pcNr = pcMax;
  i1 = UT3D_npt_ox__ (&pcNr, pcTab, &oxo, UT_DISP_cv*2.);
  if(i1 != 0) return 0;
  ip = pcNr;               // index auf den letzten punkt in pcTab
    // GR_Disp_cv (pcTab, pcNr, 9);


  // get start/endPoint of contourObj
  p1 = pcTab[0];
  p2 = pcTab[pcNr-1];



  //----------------------------------------------------------------
  // die Polygonpunkte von p1Tab rund um pta+vca drehen
  if((pcNr*3) > pcMax) goto L_eom;

  ii = 0;
  ppTab[ii] = &pcTab[ip];
  ppNr[ii] = pcNr;
  UT3D_pta_rot__ (ppTab[ii], pcTab, ppNr[ii], &pta, &vca, a1);
  ip += pcNr;
 

  // Mittellinie
  ++ii;
  ppTab[ii] = &pcTab[ip];
  ppNr[ii] = pcNr;
  UT3D_pta_rot__ (ppTab[ii], pcTab, ppNr[ii], &pta, &vca, am);
  ip += pcNr;


  // nur wenn (a1 != a2)
  if(UT2D_comp2angd_p(a1,a2,RAD_1) == 0) {
    ++ii;
    ppTab[ii] = &pcTab[ip];
    ppNr[ii] = pcNr;
    UT3D_pta_rot__ (ppTab[ii], pcTab, ppNr[ii], &pta, &vca, a2);
    ip += pcNr;
  }


  //----------------------------------------------------------------
  // 3 Kreise um StartPoint/midPoint/endPoint von a1-a2

  // project StartPoint -> pta+vca
  UT3D_pt_projptptvc (&pc, &d1, NULL, &p1, &pta, &vca);
  if(d1 < UT_TOL_cv) goto L_c2;

  // kreis um pta+vca mit StartPoint von a1 bis a2
  UT3D_ci_ptvcpt2angr (&ci1, &pc, &p1, &vca, a1, a2);

  // prepare next ptTab
  ++ii;
  ppTab[ii] = &pcTab[ip];
  ppNr[ii] = pcMax - ip;

  // circ -> polygon
  UT3D_cv_ci (ppTab[ii], &ppNr[ii], &ci1, ppNr[ii], UT_DISP_cv*2.);
  ip += ppNr[ii];



  L_c2:

  // get pc = 
  // project endPoint -> pta+vca
  UT3D_pt_projptptvc (&pc, &d1, NULL, &p2, &pta, &vca);
  if(d1 < UT_TOL_cv) goto L_c3;

  // get ci1 = circle starting at p2 around pc normal to vca
  // kreis um pta+vca mit StartPoint von a1 bis a2
  UT3D_ci_ptvcpt2angr (&ci1, &pc, &p2, &vca, a1, a2);


  // prepare next ptTab
  ++ii;
  ppTab[ii] = &pcTab[ip];
  ppNr[ii] = pcMax - ip;

  // add ci1 to polygon ppTab
  // circ -> polygon
  UT3D_cv_ci (ppTab[ii], &ppNr[ii], &ci1, ppNr[ii], UT_DISP_cv*2.);
  ip += ppNr[ii];



  // mittelLinie
  L_c3:

  // midPoint contour
  // UTO_pt_par1_dbo (&pm, vm, srv1->typCov, srv1->indCov);
  UT3D_pt_evparcrv (&pm, vm, cTyp, cvCov);

  // project midPoint -> pta+vca
  UT3D_pt_projptptvc (&pc, &d1, NULL, &pm, &pta, &vca);
  if(d1 < UT_TOL_cv) goto L_out;

  // get ci1 = circle starting at pm around pc normal to vca
  UT3D_ci_ptvcpt2angr (&ci1, &pc, &pm, &vca, a1, a2);

  // prepare next ptTab
  ++ii;
  ppTab[ii] = &pcTab[ip];
  ppNr[ii] = pcMax - ip;

  // add ci1 to polygon ppTab
  // circ -> polygon
  UT3D_cv_ci (ppTab[ii], &ppNr[ii], &ci1, ppNr[ii], UT_DISP_cv*2.);
  ip += ppNr[ii];





  //----------------------------------------------------------------
  // use GL_Draw_obj here; see GL_tst_pta ..
  L_out:
  for(i1=0; i1<=ii; ++i1) {
    cvTab[i1].typ  = Typ_PT;
    cvTab[i1].form = Typ_PT;
    cvTab[i1].siz  = ppNr[i1];
    cvTab[i1].data = ppTab[i1];
  }

  oo.typ  = Typ_GL_CV;
  oo.form = Typ_ObjGX;
  oo.siz  = ii+1;
  oo.data = cvTab;


  dli = DL_StoreObj (Typ_SUR, ind, att);

  GL_Draw_obj (&dli, Typ_Att_dash_long, &oo);

  AP_dli_act = dli;

  // printf("ex GL_sSym_srv\n");
  return 0;

  L_eom:
    TX_Error("GL_sSym_srv EOM");
    return -1;

}


//================================================================
  int GL_sSym_sbsp (SurBSpl *sur, int att, long ind) {
//================================================================
 

  int       irc, i1, uNr, vNr, ii, ip, pNr, pMax;
  long      dli;
  double    d1;
  Point     *pTab;
  CurvBSpl  cv1;
  ObjGX     oo, cvTab[6];
  Memspc    wrkSeg;


  // DEB_dump_obj__ (Typ_SURBSP, sur, "GL_sSym_sbsp:");


  ii = 0;
  ip = 0;

  UME_init (&wrkSeg, memspc201, sizeof(memspc201));

  pTab = (Point*)memspc501;
  pMax = sizeof(memspc501) / sizeof(Point);



  // randkurve
  pNr = pMax;
  irc = UT3D_cv_sbspout (&pNr, pTab, sur, &wrkSeg);
  if(irc < 0) return -1;
  ip += pNr;

  // polygon -> cvTab[ii]
  OGX_SET_OBJ (&cvTab[ii], Typ_PT, Typ_PT, pNr, pTab);
  ++ii;
  pTab = &pTab[pNr];
  pNr = pMax - ip;         // max nr of points



  //----------------------------------------------------------------
  // mittelKurve  U

  // die optimale Anzahl von Punkten in U- und V-Richtung ermitteln
  UT3D_uvNr_sbsp (&vNr, &uNr, sur, &wrkSeg);
    // printf(" n. UT3D_uvNr_srbsp uNr=%d vNr=%d pNr=%d\n",uNr,vNr,pNr);
  if(pNr < (uNr+vNr)) goto L_eom;

  // mittlerer U-Wert
  UT3D_uparmid_sbsp (&d1, sur);

  // Isocurve (polygon) from U-wert
  pNr = uNr;
  i1 = UT3D_cv_sbspU (pNr, pTab, d1, sur, &wrkSeg);
  if(i1 < 0) return -1;
  ip += pNr;

  // polygon -> cvTab[ii]
  OGX_SET_OBJ (&cvTab[ii], Typ_PT, Typ_PT, pNr, pTab);
  ++ii;
  pTab = &pTab[pNr];
  pNr = pMax - ip;




  //----------------------------------------------------------------
  // mittelKurve  V

  // mittlerer V-Wert
  UT3D_vparmid_sbsp (&d1, sur);

  // Isocurve (polygon) from V-wert
  pNr = vNr;
  i1 = UT3D_cv_sbspV (pNr, pTab, d1, sur, &wrkSeg);
  if(i1 < 0) return -1;
  ip += pNr;

  // polygon -> cvTab[ii]
  OGX_SET_OBJ (&cvTab[ii], Typ_PT, Typ_PT, pNr, pTab);
  ++ii;
  pTab = &pTab[pNr];
  pNr = pMax - ip;




  //----------------------------------------------------------------
  // use GL_Draw_obj here; see GL_tst_pta ..
  L_out:
  oo.typ  = Typ_GL_CV;
  oo.form = Typ_ObjGX;
  oo.siz  = ii;
  oo.data = cvTab;


  dli = DL_StoreObj (Typ_SUR, ind, att);
  GL_Draw_obj (&dli, Typ_Att_dash_long, &oo);
  AP_dli_act = dli;

    // printf("ex GL_sSym_sbsp\n");

  return 0;



  L_eom:
    TX_Error("GL_sSym_sbsp EOM");

    return -1;

}


//================================================================
  int GL_sSym_srbsp (SurRBSpl *sur, int att, long ind) {
//================================================================

  int       irc, i1, uNr, vNr, ii, ip, pNr, pMax;
  long      dli;
  double    d1;
  Point     *pTab;
  CurvRBSpl cv1;
  ObjGX     oo, cvTab[6];
  Memspc    wrkSeg;


  // DEB_dump_obj__ (Typ_SURRBSP, sur, "GL_sSym_srbsp:");


  ii = 0;
  ip = 0;

  UME_init (&wrkSeg, memspc201, sizeof(memspc201));

  pTab = (Point*)memspc501;
  pMax = sizeof(memspc501) / sizeof(Point);


  // randkurve
  pNr = pMax;
  irc = UT3D_cv_srbspout (&pNr, pTab, sur, &wrkSeg);
  if(irc < 0) return -1;
  ip += pNr;

  // polygon -> cvTab[ii]
  // cvTab[ii] = UTO_obj_load (Typ_PT, Typ_PT, pNr, pTab);
  OGX_SET_OBJ (&cvTab[ii], Typ_PT, Typ_PT, pNr, pTab);
  ++ii;
  pTab = &pTab[pNr];
  pNr = pMax - ip;




  //----------------------------------------------------------------
  // mittelKurve  U

  // die optimale Anzahl von Punkten in U- und V-Richtung ermitteln
  UT3D_uvNr_srbsp (&vNr, &uNr, sur, &wrkSeg);
    // printf(" n. UT3D_uvNr_srbsp %d %d %d\n",uNr,vNr,pNr);
  if(pNr < (uNr+vNr)) goto L_eom;

  // mittlerer U-Wert
  UT3D_uparmid_srbsp (&d1, sur);

  // Isocurve (polygon) from U-wert
  pNr = uNr;
  i1 = UT3D_cv_srbspU (pNr, pTab, d1, sur, &wrkSeg);
  if(i1 < 0) return -1;
  ip += pNr;

  // polygon -> cvTab[ii]
  // cvTab[ii] = UTO_obj_load (Typ_PT, Typ_PT, pNr, pTab);
  OGX_SET_OBJ (&cvTab[ii], Typ_PT, Typ_PT, pNr, pTab);
  ++ii;
  pTab = &pTab[pNr];
  pNr = pMax - ip;




  //----------------------------------------------------------------
  // mittelKurve  V

  // mittlerer V-Wert
  UT3D_vparmid_srbsp (&d1, sur);

  // Isocurve (polygon) from V-wert
  pNr = vNr;
  i1 = UT3D_cv_srbspV (pNr, pTab, d1, sur, &wrkSeg);
  if(i1 < 0) return -1;
  ip += pNr;

  // polygon -> cvTab[ii]
  // cvTab[ii] = UTO_obj_load (Typ_PT, Typ_PT, pNr, pTab);
  OGX_SET_OBJ (&cvTab[ii], Typ_PT, Typ_PT, pNr, pTab);
  ++ii;
  pTab = &pTab[pNr];
  pNr = pMax - ip;




  //----------------------------------------------------------------
  // use GL_Draw_obj here; see GL_tst_pta ..
  L_out:
  oo.typ  = Typ_GL_CV;
  oo.form = Typ_ObjGX;
  oo.siz  = ii;
  oo.data = cvTab;


  dli = DL_StoreObj (Typ_SUR, ind, att);

  GL_Draw_obj (&dli, Typ_Att_dash_long, &oo);
  AP_dli_act = dli;


  // printf("ex GL_sSym_sbsp\n");
  return 0;



  L_eom:
    TX_Error("GL_sSym_srbsp EOM");
    return -1;

}


//====================== EOF =============================
