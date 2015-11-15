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
#include "../db/ut_DB.h"                  // DB_GetObjGX
#include "../gr/ut_GL.h"                  //
#include "../gr/ut_UI.h"                  // UI_Func... SYM_..

#include "../xa/xa_mem.h"                 // memspc51..



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

  GL_Draw_obj (&dli, Typ_Att_Fac1, &oo);


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


  int    irc, i1, ii, cvMax, sTyp, pNr, pMax, rNr;
  long   dli;
  void   *sur;
  Point  *pTab;
  ObjGX  oo, *cvTab;
  Memspc wrkSeg;


  // printf("ssssssssssssssssssssssssssssssssssssssssssssssssssss \n");
  // printf("GL_sSym_spl %d %d %d %d\n",cNr,ind,ox1->typ,ox1->form);
  // for(i1=0; i1<cNr; ++i1) {
    // if(cTab[i1].pa == NULL) {printf("skip DUMMY!!\n");continue;}
    // printf("  [%d] iNr=%d\n",i1,cTab[i1].iNr);
    // GR_Disp_pTab (cTab[i1].iNr, cTab[i1].pa, SYM_TRI_S, 4);
  // }


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
      sTyp = UTO_obj_getp (&sur, &rNr, ox1);
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


  dli = DL_StoreObj (Typ_SUR, ind, att);

  GL_Draw_obj (&dli, Typ_Att_Fac1, &oo);


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


  int    irc, i1, ii, ip, pcNr, ppNr[6], pcMax, cTyp, rNr;
  long   dli, l1;
  double d1, a1, am, a2, v0, vm, v1;
  void   *oc;
  Point  pta, *pcTab, *ppTab[6], p1, p2, pm, pc;
  Vector vca;
  Circ   cic, ci1;
  ObjGX  oo, oco, oci, oc1, oc2, cvTab[6], *ocp;
  SurRev *srv1;
  Memspc wrkSeg;



  // printf("GL_sSym_srv %d\n",ind);

  if(TSU_mode != 0) return 0;  // tessel -> mem: nothing to do.



  UME_init (&wrkSeg, memspc201, sizeof(memspc201));

  pcTab = (Point*)memspc55;
  pcMax = sizeof(memspc55) / sizeof(Point);


  // KonturKurve & Achse extrahieren ...  see also TSU_tr_init_
  srv1 = oxi->data;
  a1 = srv1->ang1;
  a2 = srv1->ang2;
  v0 = srv1->v0;
  v1 = srv1->v1;


  // invert direction if dir=revers
  if(((SurRev*)oxi->data)->dir) {
    MEM_swap_2db (&a1, &a2);
  }


  // die Winkel korrigieren; muessen im Bereich -2Pi bis +2Pi sein
  // und aufsteigend.
  UT2D_2angr_set (&a1, &a2, 1);


  // Mittelwerte errechnen
  vm = (v0 + v1) / 2.;
  am = (a1 + a2) / 2.;   UT2D_2angr_set (&am, &am, 1);
    // printf(" ang1=%f am=%f ang2=%f v0=%f vm=%f v1=%f\n",a1,am,a2,v0,vm,v1);




  // -----------------------------------
    // Achse holen
    // printf("Cen-typ=%d ind=%d\n",srv1->typCen, srv1->indCen);
    oci = DB_GetObjGX ((int)srv1->typCen, srv1->indCen);
    if(oci.typ == Typ_Error) return -1;


    // rotAxis setzen; Punkt pta + Vector vca
    i1 = UT3D_ptvc_ox (&pta, &vca, &oci);
    if(i1 < 0) return i1;



  // -----------------------------------
  // get oci = obj to cut   srv1->typCov, srv1->indCov
  oci.typ   = srv1->typCov; // Typ_CI; // Typ_LN;
  oci.form  = Typ_Index;
  l1 = srv1->indCov;
  oci.data  = (void*)l1;


  // ACHTUNG vermutlich muss man hier auch TSU_srv_tor_01 benutzen uva ...
  // wenn oci ein Vollkreis ist, den Startpunkt ganz innen setzen ...
  cTyp = UTO_obj_getp ((void**)&ocp, &rNr, &oci);
  if(cTyp == Typ_CI) {
      // UT3D_stru_dump(Typ_CI, ocp, " srv-cont1:");
    UT3D_pt_projptptvc (&pta, &d1, NULL, &((Circ*)ocp)->pc, &pta, &vca);
      // GR_Disp_pt (&pta, SYM_STAR_S, 4);
    // SEE ALSO TSU_srv_tor_03 !
    if(UT3D_ck_ci360((Circ*)ocp) == 0) {
      cic = *(Circ*)ocp;
      // den Z-Vektor setzen
      UT3D_vc_perpvc2pt (&cic.vz, &vca, &pta, &cic.pc);
      UT3D_pt_traptptlen (&cic.p1, &cic.pc, &pta, fabs(cic.rad));
      cic.p2 = cic.p1;
        // UT3D_stru_dump(Typ_CI, &cic, " srv-cont2:");
        // GR_Disp_ac (&cic, 9);
        // GR_Disp_pt (&cic.p1, SYM_STAR_S, 3);
      oci.typ   = Typ_CI;
      oci.form  = Typ_CI;
      oci.data  = &cic;
      ocp = (void*)&cic;  // for midPoint ..
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
  irc = UTO_CUT__ (&oco, &oci, &oc1, &oc2, 0, &wrkSeg);
  if(irc < 0) return irc;
    // UTO_obj_Disp__ (&oco, &wrkSeg, 9);
    // UTO_dump__ (&oco, "contour:");


  // oco in Polygon pcTab umwandeln ...
  pcNr = pcMax;
  i1 = UT3D_npt_ox (&pcNr, pcTab, &oco, UT_DISP_cv*2.);
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

  // project endPoint -> pta+vca
  UT3D_pt_projptptvc (&pc, &d1, NULL, &p2, &pta, &vca);
  if(d1 < UT_TOL_cv) goto L_c3;

  // kreis um pta+vca mit StartPoint von a1 bis a2
  UT3D_ci_ptvcpt2angr (&ci1, &pc, &p2, &vca, a1, a2);

  // prepare next ptTab
  ++ii;
  ppTab[ii] = &pcTab[ip];
  ppNr[ii] = pcMax - ip;

  // circ -> polygon
  UT3D_cv_ci (ppTab[ii], &ppNr[ii], &ci1, ppNr[ii], UT_DISP_cv*2.);
  ip += ppNr[ii];



  // mittelLinie
  L_c3:

  // midPoint contour
  // UTO_pt_par1_dbo (&pm, vm, srv1->typCov, srv1->indCov);
  UT3D_pt_evparcrv (&pm, vm, cTyp, ocp);

  // project midPoint -> pta+vca
  UT3D_pt_projptptvc (&pc, &d1, NULL, &pm, &pta, &vca);
  if(d1 < UT_TOL_cv) goto L_out;

  // kreis um pta+vca mit StartPoint von a1 bis a2
  UT3D_ci_ptvcpt2angr (&ci1, &pc, &pm, &vca, a1, a2);

  // prepare next ptTab
  ++ii;
  ppTab[ii] = &pcTab[ip];
  ppNr[ii] = pcMax - ip;

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

  GL_Draw_obj (&dli, Typ_Att_Fac1, &oo);


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


  // UT3D_stru_dump (Typ_SURBSP, sur, "GL_sSym_sbsp:");


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
  GL_Draw_obj (&dli, Typ_Att_Fac1, &oo);


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


  // UT3D_stru_dump (Typ_SURRBSP, sur, "GL_sSym_srbsp:");


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

  GL_Draw_obj (&dli, Typ_Att_Fac1, &oo);


  // printf("ex GL_sSym_sbsp\n");
  return 0;



  L_eom:
    TX_Error("GL_sSym_srbsp EOM");
    return -1;

}


//====================== EOF =============================
