//          Planar & Revolved-Surface-Functions    RF.
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
2004-06-17 ut_surpln.c includiert. RF.
2003-06-30 (bcv->ptNr >= UT_BEZDEG_MAX) -> (bcv->ptNr > UT_BEZDEG_MAX + 1) TB.
2003-06-02 Neu.  TB/RF.

-----------------------------------------------------
*/
/*!
\file  ../ut/ut_surstd.c
\brief Planar & Revolved-Surface-Functions 
\code
=====================================================
List_functions_start:

----------- all surfaces --------------------

UTO_get_suppSurf          get supportSurface of SUTP <surTps>
UTO_ck_surfTyp            returns surfSubTyp      ((../ut/ut_obj.c)
SUR_ck_typ                get typ & index of supportSurf, nr of contours.
SPU_ckMinSiz              Kontur testen ob groesser als tol

SUP_orient                Orientieren der Konturen
SUP_load_c                alle Konturen laden
SUP_addBound              Tabelle aus 3D-Punkten aufbauen
SUR_pt2_prjptsur          project 3D-point on surface (get 2D-point)
UT3D_ptvc_sus             Point+Vector from SurStd  (Normalvec auf Flaeche)

----------- SURCIR (tesselated fan) ---------
UT3D_cv_scir__            get boundary-points from SURCIR (tesselated fan)

----------- Planar --------------------------
UT3D_cv_pls__             get boundary-points from planar surface
SUP_sup_ContTab           make planar surf from ContTab
SUP_ContTab_pTab          make ContTab from points

----------- Conus --------------------------
UT3D_parscon_pt           u/v-Parameter from  3D-conic-surfacePoint
UT3D_pt_evparscon         3D-point <-- evaluate conic-surfaceParameters
UT3D_pt_projptbcon        project PT -> Conus
UT3D_bcon_pt2rd2          Conus from 2 points + 2 radius

----------- Torus --------------------------
UT3D_parstor_pt           u/v-Parameter from  3D-toric-surfacePoint
UT3D_pt_evparstor         3D-point <-- evaluate toric-surfaceParameters
UT3D_pt_projptbtor        project PT -> Torus

----------- Sphere -------------------------
UT3D_parsph_pt            u/v-Parameter from sphere
UT3D_pt_evparsph          3D-point <-- evaluate spher-surfaceParameters
UT3D_angsph_pt            compute hor/vert angle of point on sphere

List_functions_end:
=====================================================
// SUR_Disp_Sur              temp. Display Surfaces
- see also:
../ut/ut_srv.c (Revolved-Surface-Functions)
../ut/ut_sru.c (Ruled-Surface-Functions)

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
#include "../ut/ut_cast.h"                // INT_PTR
#include "../ut/ut_ox_base.h"             // OGX_SET_INDEX

#include "../db/ut_DB.h"                  // DB_GetCurv
#include "../ut/func_types.h"                  // SYM_STAR_S



//================================================================
  int UTO_get_suppSurf (void **ssDat, ObjGX *surTps) {
//================================================================
// get supportSurface of SUTP <surTps>
// Output:
//   retCod   Typ_PLN|Typ_SURBSP|Typ_SURRV|Typ_SURRU..
//   ssDat    Typ_SURBSP:   SurBSpl*
//            Typ_SURRU:    2 ObjGX-objects
//
// see also UTO_ck_surfTyp SUR_ck_typ

  int     ssTyp, ssForm;
  long    l1;
  ObjGX   *sso, *os;



  printf("UTO_get_suppSurf %d %d %d\n",surTps->typ,surTps->form,surTps->siz);


  if(surTps->form != Typ_ObjGX) return Typ_Error;

  sso = &((ObjGX*)surTps->data)[0];


  L_1:
  ssTyp = sso->typ;
  ssForm = sso->form;
    printf(" sTyp=%d ssForm%d\n",ssTyp,ssForm);




  //----------------------------------------------------------------
  if(ssTyp == Typ_SUR) {

    if(ssForm == Typ_Index) {  // 100
      l1 = LONG_PTR(sso->data);
      // printf("  StuetzFl.A%d\n",i2);
      sso = DB_GetSur (l1, 0);
      goto L_1;

    } else {TX_Error("UTO_get_suppSurf E001"); return -1;}

  }


  //----------------------------------------------------------------
  switch (ssTyp) {

    case Typ_Typ:
      // if(ssForm == Typ_Typ)
      ssTyp = Typ_SURPLN;
      break;

    case Typ_SURRU:
    case Typ_SURRV:
    case Typ_SURBSP:
      *ssDat = sso->data;
      break;

    default:
      TX_Error("UTO_get_suppSurf E002 %d %d",ssTyp,ssForm);
      return -1;
  }


  return ssTyp;

}


//================================================================
  int SUR_pt2_prjptsur (Point *pt2o, Point *pt3i, ObjGX *oSur) {
//================================================================
/// \code
/// SUR_pt2_prjptsur        project 3D-point on surface (get 2D-point)
/// = get 2D-parameters of 3D-point on surface
/// \endcode

  int       sTyp;

  Point     pt1; // TEST ONLY


   // DEB_dump_obj__(Typ_PT, pt3i, "SUR_pt2_prjptsur: ");


  // get type of surface
  sTyp = UTO_ck_surfTyp (oSur);
    // printf(" sTyp=%d\n",sTyp);



  //----------------------------------------------------------------
  // transform pt1 into 2D-point on surface
  //----------------------------------------------------------------
  // handle SBSP
  if(sTyp == Typ_SURBSP) {                 printf(" Typ_SURBSP\n");

    // intersect line - ruledSurface  ../ci/NC_apt.c  :8945
    // UT3D_pt_intlnsbs     (Intersect-Line)

    // init sur
    TSU_tr_init_ (oSur);

    // get 2D-coords of pt1
    TSU_tr_2D_3D_sbsp (pt2o, pt3i);
      // DEB_dump_obj__(Typ_PT2, pt2o, "");

    // // TEST-ONLY
    // TSU_tr_3D_2D_sbsp (&pt1, pt2o);
      // GR_Disp_pt (&pt1, SYM_STAR_S, 2);



  //----------------------------------------------------------------
  // handle SRU
  } else if(sTyp == Typ_SURRU) {           printf(" Typ_SURRU\n");

    // intersect line - ruledSurface  ../ci/NC_apt.c  :8945

    // init sur
    TSU_tr_init_ (oSur);

    // get 2D-coords of pt1
    // UT3D_parsru_pt (&pt2o->x, &pt2o->y, pt3i);
    TSU_tr_2D_3D_sru (pt2o, pt3i);
      // DEB_dump_obj__(Typ_PT2, pt2o, "ex TSU_tr_2D_3D_sru:");

    // // TEST-ONLY
    // TSU_tr_3D_2D_sru (&pt1, pt2o);
      // GR_Disp_pt (&pt1, SYM_STAR_S, 2);



  //----------------------------------------------------------------
  // handle SRV
  } else if(sTyp == Typ_SURRV) {           printf(" Typ_SURRV\n");

    // project pt1 -> RevolvedSur

    // init sur
    TSU_tr_init_ (oSur);

    // get 2D-coords of pt1
    TSU_tr_2D_3D_srv (pt2o, pt3i);
      // DEB_dump_obj__(Typ_PT2, pt2o, "");

    // // TEST-ONLY
    // TSU_tr_3D_2D_srv (&pt1, pt2o);
      // GR_Disp_pt (&pt1, SYM_STAR_S, 2);

    pt2o->x /= RAD_360;  // x=angle; -> value 0-1



  //----------------------------------------------------------------
  // handle SPLN
  } else goto  Err2;


  return 0;


  Err2:
    // TX_Error("SUR_pt2_prjptsur E001\n");
    printf("SUR_pt2_prjptsur E001 %d\n",sTyp);
    return -1;


}


//===========================================================================
  int UT3D_cv_scir__ (int *pNr, Point *pa, int paSiz, ObjGX *oxi, int mode) {
//===========================================================================
/// \code
/// UT3D_cv_scir__            get boundary-points from SURCIR (tesselated fan)
///   mode   0=perm, fix PRCV; 1=temp, do not use PRCV; 2=unknown
/// returns array of points; max. 4
/// see also UT3D_cv_sru_ UT3D_cv_pls__
/// \endcode

  int       i1, ityp;
  long      dbi;
  ObjGX     *oxp1, *oa;
  Point     *pTab;


  // printf("UT3D_cv_scir__ %d\n",oxi->typ);
  // DEB_dump_ox_s_ (oxi, "RCIR");


  //----------------------------------------------------------------
  if(oxi->typ != Typ_SURCIR) goto L_err1;

  return UT3D_npt_obj (pNr, pa, paSiz,
                       Typ_ObjGX, oxi->data, oxi->siz, UT_DISP_cv, mode);


/*
  oa = oxi->data;

  if(oxi->siz < 3) goto L_not_supp;
  if(oxi->siz > 4) goto L_not_supp;

  *pNr = oxi->siz;

  // get all points of RCIR > pTab
  for(i1=0; i1<oxi->siz; ++i1) {

    if(oa->form == Typ_Index) {
      OGX_GET_INDEX (&ityp, &dbi, oa);
      pa[i1] = DB_GetPoint (dbi);
        // DEB_dump_obj__(Typ_PT, &pa[i1], "  pa[%d]:",i1);

    } else {
      TX_Error("UT3D_cv_scir__ E_CIR %d %d",oxi->form,oxi->siz);
      return -1;
    }
    ++oa;
  }


  //----------------------------------------------------------------
  return 0;


  //----------------------------------------------------------------
  L_not_supp:
  TX_Error("UT3D_cv_scir__ E002 %d",oxi->typ);
  return -1;
*/

  L_err1:
  TX_Error("UT3D_cv_scir__ E001 %d %d",oxi->form,oxi->siz);
  return -1;

}
 

//=====================================================================
  int SUP_sup_ContTab (void *oSpc, int *oSiz, Memspc *memSeg1,
                       ContTab *cTab, int cNr) {
//=====================================================================
// SUP_sup_ContTab           make planar surf from ContTab
// oSpc:    hierher kommen die oGX-Records
// memSeg1: hierher kommen die Points

  int      irc, i1;
  void     *pDat;
  ObjGX    *ox1, *ox2;
  CurvPoly *cv1;


  // printf("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS\n");
  // printf("SUP_sup_ContTab cNr=%d oSiz=%d\n",cNr,*oSiz);

  ox1 = oSpc;
  // (char*)ox2  = (char*)ox1 + sizeof(ObjGX);
  // (char*)pDat = (char*)ox2 + ((cNr+1) * sizeof(ObjGX));
  ox2  = (ObjGX*)((char*)ox1 + sizeof(ObjGX));
  pDat = (ObjGX*)((char*)ox2 + ((cNr+1) * sizeof(ObjGX)));

  *oSiz = *oSiz - ((cNr+2) * sizeof(ObjGX));
  if(*oSiz < 1024) goto L_EOM;

  ox1->typ  = Typ_SUR;
  ox1->form = Typ_ObjGX;
  ox1->siz  = cNr + 1;
  ox1->data = ox2;


  OGX_SET_INT (Typ_Typ, Typ_SURPLN, ox2);
  // ox2->typ  = Typ_Typ;
  // ox2->form = Typ_SURPLN;
  // ox2->siz  = 1;
  // ox2->data = NULL;


  for(i1=0; i1<cNr; ++i1) {

    // (char*)ox2 += sizeof(ObjGX);
    ox2  = (ObjGX*)((char*)ox2 + sizeof(ObjGX));

    ox2->typ  = Typ_CV;
    ox2->form = Typ_CVPOL;
    ox2->siz  = 1;
    ox2->data = pDat;

    cv1 = pDat;
    irc = UT3D_plg_pta (cv1, cTab[i1].pa, cTab[i1].iNr, memSeg1);
    if(irc < 0) goto L_EOM;
      // DEB_dump_obj__ (Typ_CVPOL, cv1, "Cv[%d]:",i1);

    *oSiz = *oSiz - sizeof(CurvPoly);
    if(*oSiz < 1024) goto L_EOM;

  }


  // DEB_dump_ox_s_ (oSpc, "ex sup_ContTab");
  // DEB_dump_ox_0 (oSpc, "ex sup_ContTab");

  // UTO_obj_Disp__ (oSpc, NULL, 0);
  // SUR_Disp_Sur (oSpc, memSeg1, 0);


  return 0;

  L_EOM:
    return -1;

}


//=========================================================================
  int SUP_ContTab_pTab (ContTab *cTab, int pNr, Point *pTab, Vector *vcn) {
//=========================================================================
// make ContTab from points

  cTab->iNr = pNr;
  cTab->pa  = pTab;
  cTab->cTyp = Typ_modOUT;

  // feststellen Umdrehungssinn; unbedingt; sonst sieht ma gar nix !!
  SUP_orient (vcn, cTab, 1);

  return 0;

}


//=======================================================================
  int SUP_orient (Vector *vcz, ContTab *cTab, int cNr) {
//=======================================================================
// Orientieren der Konturen fuer alle BasisKonturen erforderlich;
//  Jede 2D-Kontur testen ob CW/CCW; wenn erforderlich die 2D-Kontur
//  und die 3D-Kontur umdrehen.


  int    i1, i2, ipa, iNr, plMain;
  Point  *pta;


  // printf("SUP_orient %d\n",cNr);
  // DEB_dump_obj__ (Typ_VC, vcz, "    vcz:\n");


  plMain = UT3D_bp_perp_vc (NULL, vcz); // Berechnungsebene definieren


  for(i1=0; i1<cNr; ++i1) {

    iNr = cTab[i1].iNr - 1;  // Anzahl Punkte ohne den letzten == ersten Punkt !
    // ipa = cTab[i1].ipa;
    pta = cTab[i1].pa;

    // wenn keine Kontur vorhanden:
    if(pta == NULL) continue;

    // printf("  c%d start=%d anz=%d\n",i1,ipa,iNr);

    // i2 = UT3D_sr_polc (iNr, &pTab[ipa], vcz);
    i2 = UT3D_sr_polc (iNr, pta, vcz, plMain);     // test direction
    // 1=CCW, -1=CW;
    // printf(" %d orient=%d\n",i1,i2);
    // if(i2 < 0) return -1;

    // test Aussenkontur; muss CCW sein
    if(i1 == 0) {      // erste = aussen
      cTab[i1].cTyp = Typ_modOUT;
      if(i2 < 0) {
        // printf(" inv con %d\n",i1);
        // UT3D_cv_inv (iNr, &pTab[ipa]);
        UT3D_cv_inv (iNr+1, pta);
      }
      cTab[i1].dir = '-';  // '+'=counterclockwise

    // test Innenkontur; muss CW sein
    } else {
      cTab[i1].cTyp = Typ_modIN;
      if(i2 > 0) {
        // printf(" inv con %d\n",i1);
        // UT3D_cv_inv (iNr, &pTab[ipa]);
        UT3D_cv_inv (iNr+1, pta);
      }
      cTab[i1].dir = '-';  // '-'=clockwise
    }
  }



  return 0;

}


//=========================================================
  int SUP_load_c (ContTab *cTab, int *cNr, int cMax,
                  Point *pTab, int pMax,
                  ObjGX *oxi, int sTyp) {
//=========================================================
// alle Konturen aus oxi -> cTab ...
// Output:
//   cTab    contour (closed; last pt = first pt)
//   RC: 0 = OK;
//       1 = Aussenkontur klein (planar darstellen)
//       2 = Aussenkontur zu klein (gar nix darstellen)
// ACHTUNG: bei Konus wird an Spitze ein zusaetzlicher Punkt mit
//          X=VAL_MAX eingefuegt !


  int    i1, i2, irc;
  long   ind;
  ObjGX  ox1, *oxTab, *oxp1;


  // printf("SUP_load_c cMax=%d pMax=%d sTyp=%d\n",cMax,pMax,sTyp);
  // printf(" typ=%d form=%d siz=%d\n",oxi->typ,oxi->form,oxi->siz);
  // printf(" pTab-posi=%p\n",pTab);


  if(oxi->siz > cMax) {TX_Error("SUP_load_c E001"); return -1;}

  oxTab = oxi->data;
  // printf(" oxTab-posi=%p\n",oxTab);
  *cNr = 0;

  for(i1=0; i1<oxi->siz; ++i1) {

    // printf(" load %d cNr=%d\n",i1,*cNr);

    oxp1 = &oxTab[i1];
    // printf(" oxTabi[%d]-posi=%p\n",i1,oxp1);
    // printf("_load_c[%d] typ=%d form=%d\n",i1,oxp1->typ,oxp1->form);


    // skip stuetzflaeche
    if(i1 == 0) {
        // printf(" Stuetz:typ=%d form=%d\n",oxp1->typ,oxp1->form);
      if(oxp1->typ == Typ_Typ) continue;
      if(oxp1->typ == Typ_SUR) continue;
      if(oxp1->typ == Typ_SURBSP) continue;
      if(oxp1->typ == Typ_SURRBSP) continue;
      if(oxp1->typ == Typ_SURRU) continue;
      if(oxp1->typ == Typ_SURRV) continue;
      if(oxp1->typ == Typ_SOL) continue;  // 34 - Conus, Torus ..



    } else if(i1 == 1) {  // Aussenkontur
      // printf(" Aussen:typ=%d form=%d\n",oxp1->typ,oxp1->form);

      if((oxp1->typ == Typ_SUR)      ||
         (oxp1->typ == Typ_SURBSP)   ||
         (oxp1->typ == Typ_SURRBSP)  ||
         (oxp1->typ == Typ_SURRV))    {
        // Aussenkontur einer PlanarTrimmedSurf: normal holen !!!
        // Aussenkontur einer SRV: automat via 2D-Kontur von 0-1;
        if(sTyp != Typ_SURPLN) {
          // eine Dummykontur anlegen mit ptNr=0; (nur 2D-Kontur generieren)
          // printf(" make %d dummyCont.\n",*cNr);
          cTab[*cNr].iNr = 5;
          cTab[*cNr].pa  = NULL;
          ++(*cNr);
          continue;
        }
      }


    // } else {
      // TX_Error("SUP_load_c E001");
      // return -1;
    }


    // obj aus DB holen
    if(oxp1->form == Typ_Index) {
      ind = LONG_PTR(oxp1->data);
      ox1 = DB_GetObjGX (oxp1->typ, ind);
    } else ox1 =  *oxp1;
    // printf("   c1: typ=%d form=%d\n",ox1.typ,ox1.form);
    // DEB_dump_ox_0 (&ox1, "in SUP_load_c");

    // add Contour -> pTab u icTab
    irc = SUP_addBound (cTab,cNr,cMax, pTab,pMax, &ox1, UT_DISP_cv);
    if(irc < 0) return -1;

    // Aussenkontur:
    if(*cNr == 1) {  // wurde in SUP_addBound auf 1 erhoeht !
      if(irc > 0) return irc;    // Kontur zu klein
    }


    // pTab weitersetzen
    i2 = cTab[*cNr-1].iNr;
    pMax -= i2;
    pTab = &pTab[i2];

  }


  //================================================================
  // wenn gar keine Kontur (keine Aussenkontur, keine Loecher:
  if(*cNr > 0) goto L_fertig;

  cTab[*cNr].iNr = 5;
  cTab[*cNr].pa  = NULL;
  ++(*cNr);




  //================================================================
  // Testausg:
  L_fertig:
  // printf("ex SUP_load_c %d\n",*cNr);
  // for(i1=0;i1<*cNr;++i1) printf(" k[%d] siz=%d\n",i1,cTab[i1].iNr);


  return 0;

}


//===================================================================
  int SUP_addBound (ContTab *cTab, int *cNr, int cMax,
                    Point *pTab, int pMax,
                    ObjGX *oxi,  double tol) {
//===================================================================
// Tabelle aus 3D-Punkten aufbauen
// (oxi in Polygon umwandeln, an pTab hintanhaengen, cTab updaten).
// cNr = Anzahl gespeicherter Konturen
// cTab = Konturinfos

  int     irc, i1, ptNr, ip;
  double  d1;


  // printf("SUP_addBound %d typ=%d form=%d tol=%f\n",*cNr,
                          // oxi->typ,oxi->form,tol);
  // printf(" pTab-posi=%p\n",pTab);
  // printf(" oxi-data-posi=%p\n",oxi->data);
  // DEB_dump_ox_s_ (oxi, "_addBound");


  if(!oxi->typ) return -1;  // no outer-boundary


  // noch Platz in icTab ?
  if(*cNr >= cMax-1) return -1;


  // den Startindex fuer die neuen points zusammenzaehlen ...
  // if(*cNr == 0) ip = 0;
  // else ip = cTab[*cNr-1].ipa + cTab[*cNr-1].iNr;
  // ip = 0;


  // cTab[*cNr].ipa = ip;

  // Restplatz
  // ptNr = pMax - ip;
  ptNr = pMax;



  // objekt -> Polygon umwandeln
  // i1 = UT3D_npt_ox__ (&ptNr, &pTab[ip], oxi, tol);
  irc = UT3D_npt_ox__ (&ptNr, pTab, oxi, tol);
  if(irc > 0) return 0; // skip Stuetzflaeche
  if(irc < 0) return irc;


// TODO: use PRCV_npt_dbo__
  // irc = PRCV_npt_dbo__ (&pTab, &ptNr, Typ_CVTRM, dbi, AP_modact_ind);
    // if(irc < 0) return -1;




  // check if boundary closed. If not: add pt.
  if(UT3D_comp2pt (&pTab[0], &pTab[ptNr-1], UT_DISP_cv) == 0) {
      // printf(" add closing pt..\n");
    if(ptNr >= pMax) return -1;
    pTab[ptNr] = pTab[0];
    ++ptNr;
  }


    // Testdisp.
    // printf(" 3D-Cont %d:\n",*cNr);
    // GR_tDyn_npt__ (ptNr, pTab, SYM_STAR_S, 2);  return -1;


  // wenn weniger als 4 Punkte, Kontur nur mehr Strich.
  if(ptNr < 4) { ptNr = 0; goto L_done; }


  // // Anzahl pt gesamt
  // pNr = ip + ptNr;


  //================================================================
  // Aussenkontur testen ob groesser als tol
  irc = SPU_ckMinSiz (ptNr, pTab); 
  if(irc > 0) goto L_done;



  //================================================================
  // beim Durchgang durch die Achse einen oder 2 Punkte einfuegen.
  TSU_fix_ax_crx (&ptNr, pTab);
    // for(i1=1; i1<ptNr; ++i1) {
      // DEB_dump_obj__(Typ_PT, &pTab[i1], "boundP[%d]");
    // }



  L_done:
  cTab[*cNr].iNr = ptNr;
  cTab[*cNr].pa  = pTab;
  ++(*cNr);

  // printf("ex SUP_addBound rc=%d\n",irc);
  return irc;

}


//===================================================================
  int SPU_ckMinSiz (int pNr, Point *pTab) {
//===================================================================
// Aussenkontur testen ob groesser als tol
// rc = 0   Kontur groesser als tol
// rc = 1   Kontur kleiner als tol
// rc = 2   Kontur kleiner als tol2

  int    i1, irc;
  double tol, tol2;
  Vector vc1;


  // printf("SPU_ckMinSiz %d\n",pNr);

  tol  = UT_DISP_cv * 2.;
  tol2 = UT_TOL_cv * 0.2;
  irc = 0;
  // printf("  tol=%f tol2=%f\n",tol,tol2);


  // den vom P0 groessten Abstand suchen;
  // wenn ein Abstand groesser als tol, sofort exit.
  L_test:
      // DEB_dump_obj__(Typ_PT, &pTab[0], "  p[0]");
  for(i1=1; i1<pNr; ++i1) {
      // DEB_dump_obj__(Typ_PT, &pTab[i1], "  p[%d]",i1);
    UT3D_vc_2pt (&vc1, &pTab[0], &pTab[i1]);
    if(fabs(vc1.dx) > tol) goto L_exit;
    if(fabs(vc1.dy) > tol) goto L_exit;
    if(fabs(vc1.dz) > tol) goto L_exit;
  }

  if(irc == 0) {
    irc = 1;
    tol = tol2;
    goto L_test;
  }
  irc = 2;


  L_exit:
  // printf("ex SPU_ckMinSiz %d\n",irc);
  return irc;

}


//===========================================================================
  int UT3D_ptvc_sur (Point *pto, Vector *vco, Memspc *memSeg1, ObjGX *su1) {
//===========================================================================
/// \code
/// UT3D_ptvc_sur             Point+normalVector from Surface
/// Input:
///   su1         surface
/// Output:
///   pto         centerpoint of surf
///   vco         normalvector of surf
///   RetCod      -1   not yet implemented
///                
/// \endcode


// see also UT3D_pta_sus
// see also UT3D_vcn_cvo


  int             i1, i2, sTyp, oTyp;
  long            dbi;
  void            *o1;
  ObjGX           *ox1p, *ox2p;



  // printf("UT3D_ptvc_sur typ=%d form=%d\n", su1->typ,su1->form);
  // DEB_dump_ox_0 (su1, "");


  // check surTyp
  i1 = SUR_ck_typ (&sTyp, &ox1p, &i2, su1);
    // printf(" surTyp=%d %d\n",i1,sTyp);


  if(sTyp == Typ_SURPLN) {
      // printf(" siz=%d\n",su1->siz);
    ox1p = su1->data;
    ox2p = &ox1p[1];   // Aussenkontur
      // DEB_dump_ox_0 (ox2p, "Boundary:");
    if(ox2p->form != Typ_Index) return -1;
    dbi = LONG_PTR (ox2p->data);

    // load Boundary from DB
    oTyp = DB_GetObjDat (&o1, &i2, ox2p->typ, dbi);
      // DEB_dump_obj__ (oTyp, o1, "o1:\n");

    if(oTyp == Typ_CI) {
      // get centerpoint & vz from Circ
      *pto = ((Circ*)o1)->pc;
      *vco = ((Circ*)o1)->vz;

    } else if(oTyp == Typ_CVPOL) {
      // get centerpoint & vz from polygon
      UT3D_vc_perppta (vco, ((CurvPoly*)o1)->ptNr, ((CurvPoly*)o1)->cpTab);
      *pto = ((CurvPoly*)o1)->cpTab[0];
      // UT3D_2vc_pta
      // UT3D_ChkPlanPts
      // UT3D_pta_plg

    } else if(oTyp == Typ_CVELL) {
      // get centerpoint & vz from ellipse
      *pto = ((CurvElli*)o1)->pc;
      *vco = ((CurvElli*)o1)->vz;

    } else return -1;


      // DEB_dump_obj__ (Typ_PT, pto, "pto:\n");
      // DEB_dump_obj__ (Typ_VC, vco, "vco:\n");
    return 0;


    //================================================================
    } else if(sTyp == Typ_SURRU) {
      // 2 boundaryCurves
      ox1p = &((ObjGX*)su1->data)[0];
      ox2p = &((ObjGX*)su1->data)[1];
        // DEB_dump_obj__ (Typ_ObjGX, ox1p, "ox1p:\n");
        // DEB_dump_obj__ (Typ_ObjGX, ox2p, "ox2p:\n");
      // if(ox1p->form == Typ_LN) {
        // if(ox1p->form == Typ_VC) {
          // // normalvec
          // // get objects (form=index)
          // pto = 
          // UT3D_vc_perpvc2pt (vco ..
        // }
      // }


  }

   // unknown surfTyp

  L_notYetImpl:
    TX_Print("- not yet implemented ..");
    return -1;

}


//===========================================================================
  int UT3D_ptvc_sus (Point *pto, Vector *vco, Memspc *memSeg1, SurStd *sus) {
//===========================================================================
// UT3D_ptvc_sus             Point+Vector from SurStd  (Normalvec auf Flaeche)
// SurStd    boundary for surface
// see UT3D_pta_sus

// nur fuer planare Flaechen !?
// see also UT3D_vcn_cvo


  int    i1, ptNr;
  Point  *pTab, pt1;
  Circ   *cip;
  Plane  pl1;
  ObjGX  ox1;


  printf("UT3D_ptvc_sus typ=%d baseID=%ld cvTyp=%d cvID=%ld\n",
           sus->typ,sus->baseID,sus->cvTyp,sus->cvID);


  switch (sus->cvTyp) {

    //---------------------------------------------------------
    case Typ_CI:
      // get CI from DB
      cip = DB_get_CI (sus->cvID);
      *pto = cip->pc;
      *vco = cip->vz;
      goto L_fertig;



    case Typ_CV:

      // get CCV
      ox1  = DB_GetObjGX (Typ_CV, sus->cvID);
      // printf(" cvtyp=%d form=%d\n",ox1.typ,ox1.form);

      switch (ox1.typ) {

        case Typ_CVTRM:
          // Plane from CCV: see GR_DrawSur
          pTab = (Point*)memSeg1->start;
          ptNr = UME_ck_free(memSeg1) / sizeof(Point);
          // CCV -> 3D-Polygon umwandeln
          i1 = UT3D_pta_ox_lim (&ptNr, pTab, ox1.data, 0, NULL, UT_DISP_cv, 2);
          if(i1 < 0) return i1;
          // 3D-Polygon -> Plane
          UT3D_pl_pta (&pl1, ptNr, pTab);
          *pto = pl1.po;
          *vco = pl1.vz;
          goto L_fertig;


        case Typ_CVELL:
          *pto = ((CurvElli*)ox1.data)->pc;
          *vco = ((CurvElli*)ox1.data)->vz;
          // UT3D_vc_perp2vc (vco,
                           // &((CurvElli*)ox1.data)->va,
                           // &((CurvElli*)ox1.data)->vb);
          goto L_fertig;


        // case: Typ_CVBSP:
          // i1 = bspl_pol_bsp (ptNr, pTab, ox1.data, *ptNr, tol);
          // break;

        default:
          TX_Error("UT3D_ptvc_sus E002_%d",ox1.typ);
          return -1;
      }





    //---------------------------------------------------------
    default:
      TX_Error("UT3D_ptvc_sus E001_%d",sus->cvTyp);
      return -1;
  }


  L_fertig:
  // DEB_dump_obj__(Typ_PT, pto, "ex UT3D_ptvc_sus PT=");
  // DEB_dump_obj__(Typ_VC, vco, "                 VC=");

  return 0;

}



//=====================================================================
  int UT3D_parscon_pt (double *parU, double *parV, Point *pti, Mat_4x3 imat) {
//=====================================================================
// einen 3D-Konuspunkt auf den 2D-Konus umrechnen
// Der X-Wert des 2D-Punktes ist die Z-Hoehe vom Basiskreis des Konus;
// Der Y-Wert des 2D-Punktes ist der Winkel am Kreis; also 0-360 Grad.


  Point  pt31;


  // zuerst in den Nullpunkt transferieren ...
  UT3D_pt_tra_pt_m3 (&pt31, imat, pti);  // Punkt transformieren


  // X = Hoehe
  *parU = pt31.z;
  // printf("   x=%f h=%f\n",pt2o->x,SUP_con->h);


  // Y ist der Winkel = atan(pti.y/pti.x);
  *parV = UT2D_angr_vc((void*)&pt31); // uebergibt x,y als 2D-Vec.



  printf("ex UT3D_parscon_pt %f %f\n",*parU,*parV);

  return 0;

}


//=====================================================================
  int UT3D_pt_evparscon (Point *pto, Conus *con, double parU, double parV,
                         Mat_4x3 mat) {
//=====================================================================
// einen 2D-Konuspunkt auf den 3D-Konus umrechnen
// Der X-Wert des 2D-Punktes ist die Z-Hoehe vom Basiskreis des Konus;
// Der Y-Wert des 2D-Punktes ist der Winkel am Kreis; also 0-360 Grad.
// Abschliessend Transformation in die echte Lage.
// was SUP_tr_3D_2D_con

  double actR, cnf;
  Point  pt1;


  // printf("UT3D_pt_evparscon %f %f\n",parU,parV);


  // parU ist der Abstand vom Basiskreis entlang Achse bis zur Proj PT -> Achse.
  // aktueller Radius: actR = k * parU + r1     // y = kx + d
  // k = (r1 - r2) / h;  // Steigung
  cnf = (con->r2 - con->r1) / con->h;

  actR = (parU * cnf)  + con->r1;  // der Radius in der entsprechenden Hoehe

  // printf("  cnf=%f   actR=%f\n",cnf,actR);


  // in Zentrallage bringen
  pt1.x = actR * cos(parV);
  pt1.y = actR * sin(parV);
  pt1.z = parU;                  // die Hoehe
  // DEB_dump_obj__ (Typ_PT, &pt1, "           pt1: ");



  // RueckTrafo in die Normallage
  UT3D_pt_tra_pt_m3 (pto, mat, &pt1);  // Punkt transformieren
  // DEB_dump_obj__ (Typ_PT, &pt2, "           pt2: ");



  // DEB_dump_obj__ (Typ_PT, ptr, "ex SUScon_pt_pt2: ");

  return 0;

}


//================================================================
  int UT3D_pt_projptbcon (Point *pto, Conus *co1, Point *pti) {
//================================================================
// UT3D_pt_projptbcon        project PT -> Conus



  double  ls, lc, la, lm, rs, ac, tan_ac, h_tot;
  Point   pts, pt1, pt0;
// nur Tests:
  long    dli;


  // DEB_dump_obj__ (Typ_CON, co1, "UT3D_pt_projptbcon");

  // pts = proj Pt -> Hauptachse.
  UT3D_pt_projptptvc (&pts, &ls, NULL, pti, &co1->pl.po, &co1->pl.vz);
  // dli=0;APT_disp_SymB (SYM_TRI_S, 2, &pts);



  //-------------------------------------------------
  // Sonderfall Zylinder:
  if(UTP_comp2db(co1->r1, co1->r2, UT_TOL_min1)) {

    // um den Radius verringert -> pts
    UT3D_pt_traptptlen (pto, pti, &pts, ls-co1->r1);
    goto L_fertig;
  }



  //-------------------------------------------------
  // Oeffnungswinkel Conus ac errechnen
  tan_ac = (co1->r1 - co1->r2) / co1->h;
  ac = atan (tan_ac);


  // die Konusspitzenpunkt pt0 konstruieren; er ist h_tot vom Nullpunkt
  // entfernt.
  h_tot = co1->r1 * co1->h / (co1->r1 - co1->r2);
  UT3D_pt_traptvclen (&pt0, &co1->pl.po, &co1->pl.vz, h_tot);
  // dli=0;APT_disp_SymB (SYM_TRI_S, 2, &pt0);

  // printf(" ls=%f ac=%f h_tot=%f\n",ls,UT_DEGREES(ac),h_tot);



  // den Radius (rs) am Punkt pts errechnen; aus Abstand von pt0 (lc) und Winkel
  lc = UT3D_len_2pt (&pt0, &pts);
  rs = fabs(tan_ac * lc);
  la = ls-rs;         // die Strecke pti -> pt1 (ausserhalb Con).
  // printf(" lc=%f rs=%f\n",lc,rs);



  // pt1 - der Durchstichpkt von pti-pts
  UT3D_pt_traptptlen (&pt1, pti, &pts, la);
  // dli=0;APT_disp_SymB (SYM_TRI_S, 2, &pt1);



  // den Abstand lm am mantel ab pt1 mit Winkel errechnen
  // sin(ac) = lm / la
  lm = sin(ac) * la;
  // printf(" lm=%f\n",lm);

  // wenn Konus Positiv:
  if(ac > 0.) lm = -lm;

  UT3D_pt_traptptlen (pto, &pt1, &pt0, lm);
  // dli=0;APT_disp_SymB (SYM_TRI_S, 2, pto);


  L_fertig:
  // DEB_dump_obj__ (Typ_PT, pto, "ex UT3D_pt_projptbcon");

  return 0;

}


//=========================================================
  int UT3D_bcon_pt2rd2 (Conus *co1, Point *pt1, Point *pt2,
                                    double rd1, double rd2) {
//=========================================================
// UT3D_bcon_pt2rd2          Conus from 2 points + 2 radius


  Vector  vz;


  UT3D_vc_2pt (&vz, pt1, pt2);

  UT3D_pl_ptvc (&co1->pl, pt1, &vz);

  co1->r1 = fabs(rd1);
  co1->r2 = fabs(rd2);
  co1->h  = UT3D_len_2pt (pt1, pt2);


  return 0;

}




//================================================================
//========= Sphere ===============================================
//================================================================




//=====================================================================
  int UT3D_parsph_pt (double *parU, double *parV, Point *pti,
                       double rds,  Mat_4x3 imat) {
//=====================================================================
// Winkel eines Punktes auf einer Kugel errechnen
// Der X-Wert des 2D-Punktes ist der UmfangsWinkel; 0-360 Grad.
// Der Y-Wert des 2D-Punktes ist der HoehenWinkel;  -180 > 180 Grad.

  Vector2  vc2;
  Point    pt31;

  // DEB_dump_obj__ (Typ_PT, pti, "UT3D_parsph_pt: ");



  // zuerst in den Nullpunkt transferieren ...
  UT3D_pt_tra_pt_m3 (&pt31, imat, pti);  // Punkt transformieren
    // DEB_dump_obj__ (Typ_PT, &pt31, "           pt31: ");



  // X = UmfangsWinkel; der Punkt liegt immer auf diesem Vektor
  *parU = UT2D_angr_vc((Vector2*)&pt31); // uebergibt x,y als 2D-Vec.
  // printf("   x=%f h=%f\n",pt2o->x,SUP_tor->h);


  // Y = HoehenWinkel: der Winkel jenes Dreickes, das der Hauptvektor mit
  // der Y-Wert ist die Hoehe von pt31;
  // der X-Wert ist der 2D-Abstand von pt31 in XY.
  vc2.dx = UT2D_len_vc ((Vector2*)&pt31);
  vc2.dy = pt31.z;

  // DEB_dump_obj__ (Typ_VC2, &vc2, "           vc2: ");

  *parV = UT2D_angr_vc(&vc2);
  // printf(" ang2=%f\n",pto->y);

  if(*parV >= RAD_180) *parV -= RAD_360;  // macht -180 > 180


  // printf("ex UT3D_parsph_pt: %f %f\n",*parU,*parV);

  return 0;

}


//=====================================================================
  int UT3D_pt_evparsph (Point *pto, double rds, double parU, double parV,
                         Mat_4x3 mat) {
//=====================================================================
// einen 2D-punkt auf den 3D-Sphere umrechnen
// Der X-Wert des 2D-Punktes ist der UmfangsWinkel; 0-360 Grad.
// Der Y-Wert des 2D-Punktes ist der HoehenWinkel;  -180 > 180 Grad.
// Bezug ist der Kugelmittelpunkt.

  double  sina, cosa, sini, cosi;
  Point   pt1;

  // printf("UT3D_pt_evparsph %f %f\n",parU,parV);

  sini = sin(parU);
  cosi = cos(parU);

  sina = sin(parV);
  cosa = cos(parV);


  // DEB_dump_obj__ (Typ_PT, &pt1, "           pt1: ");

  pt1.x = rds * cosa * cosi;
  pt1.y = rds * cosa * sini;
  pt1.z = rds * sina;

  // RueckTrafo in die Normallage
  UT3D_pt_tra_pt_m3 (pto, mat, &pt1);  // Punkt transformieren
  // DEB_dump_obj__ (Typ_PT, &pt2, "           pt2: ");

  // DEB_dump_obj__ (Typ_PT, pto, "ex UT3D_pt_evparstor: ");

  return 0;

}


//===================================================================
  int UT3D_angsph_pt (double *as, int mode, Plane *pls, Point *pt1) {
//===================================================================
// UT3D_angsph_pt          compute hor/vert angle of point on sphere
// mode=0  hor. angle (rad)
// mode=1  vert. angle (rad)


  Point  ptp;
  Vector vc1;


  if(mode == 0) {

    // ptp = project point to plane from origin and Z-vec
    UT3D_pt_projptptnvc (&ptp, pt1, &pls->po, &pls->vz);
      // GR_Disp_pt (&ptp, SYM_STAR_S, 3);
    // vc1 = po - ptp
    UT3D_vc_2pt (&vc1, &pls->po, &ptp);
    *as = UT3D_angr_3vc__ (&pls->vz, &pls->vx, &vc1);

  } else {

    UT3D_vc_2pt (&vc1, &pls->po, pt1);
    *as = UT3D_angr_2vc__ (&vc1, &pls->vz);
      // GR_tDyn_vc (&pls->vz, &ptp, 8, 1);
  }


  // printf("ex UT3D_angsph_pt a=%f mode=%d\n",*as,mode);
  return 0;

}


/*
//================================================================
  int APT_dec_Ang_Obj (double *ao, int mode,
                      int *aus_typ, double *aus_tab, Plane *pl1) {
//================================================================
// decode Angle ao from Value or Point (out als rad)
// mode=0 = gives horizontal-Angle (in X-Y-Plane of pl1)
// mode=1 = gives vertical-Angle (in X-Z-Plane of pl1)

  int    i1;
  Point  *pt1, ptp;
  Vector vc1;


  printf("APT_dec_Ang_Obj %d %d %f\n",mode,*aus_typ,*aus_tab);


    if(*aus_typ == Typ_Val) {
      *ao = UT_RADIANS(*aus_tab);


    } else if(*aus_typ == Typ_PT) {
      pt1 = DB_get_PT ((long)*aus_tab);
      // compute angle
      UT3D_angsph_pt (ao, mode, pl1, pt1);


    } else return -1;


  // printf("ex APT_dec_Ang_Obj %f\n",*ao);

  return 0;

}
*/


//================================================================
//================================================================
//================================================================


//=====================================================================
  int UT3D_parstor_pt (double *parU, double *parV, Point *pti,
                       Torus *tor,  Mat_4x3 imat) {
//=====================================================================
// einen 3D-Konuspunkt auf den 2D-Konus umrechnen
// Der X-Wert des 2D-Punktes ist die Z-Hoehe vom Basiskreis des Konus;
// Der Y-Wert des 2D-Punktes ist der Winkel am Kreis; also 0-360 Grad.


  Vector2  vc2;
  Point    pt31;



  // DEB_dump_obj__ (Typ_PT, pti, "           pti: ");


  // zuerst in den Nullpunkt transferieren ...
  UT3D_pt_tra_pt_m3 (&pt31, imat, pti);  // Punkt transformieren
  // DEB_dump_obj__ (Typ_PT, &pt31, "           pt31: ");


  // X = Winkel Aussenkreis; der Punkt liegt immer auf diesem Vektor
  *parU = UT2D_angr_vc((Vector2*)&pt31); // uebergibt x,y als 2D-Vec.
  // printf("   x=%f h=%f\n",pt2o->x,SUP_tor->h);


  // Y = Winkel Innenkreis: der Winkel jenes Dreickes, das der Hauptvektor mit
  // dem Vektor vom Hauptkreismittellpunkt zum Punkt bildet.
  // der Y-Wert des Vek ist der Z-Wert des Punktes; der X-Wert ist die Strecke
  // vom MittelPunkt am hauptkreis zum Punkt.
  // vc2.dx=UT2D_len_vc((void*)&pt3i)-SUP_tor->r1; // uebergibt x,y als 2D-Vec
  // uebergibt x,y als 2D-Vec
  vc2.dx = UT2D_len_vc ((Vector2*)&pt31) - tor->r1 + tor->r2;
  vc2.dy = pt31.z;

  // DEB_dump_obj__ (Typ_VC2, &vc2, "           vc2: ");

  *parV = UT2D_angr_vc(&vc2);
  // printf(" ang2=%f\n",pto->y);






  // printf("ex UT3D_parstor_pt %f %f\n",*parU,*parV);

  return 0;

}


//=====================================================================
  int UT3D_pt_evparstor (Point *pto, Torus *tor, double parU, double parV,
                         Mat_4x3 mat) {
//=====================================================================
// einen 2D-Toruspunkt auf den 3D-Torus umrechnen
// Der X-Wert des 2D-Punktes ist der Winkel am Aussenkreis; also 0-360 Grad.
// Der Y-Wert des 2D-Punktes ist der Winkel am Innenkreis; also 0-360 Grad.
// Radius Aussenkreis = SUP_tor->r1;
// Radius Innenkreis  = SUP_tor->r2;



  double  sina, cosa, sini, cosi;
  Point   pt1;


  // printf("UT3D_pt_evparstor %f %f\n",parU,parV);


  sini = sin(parU);
  cosi = cos(parU);

  sina = sin(parV);
  cosa = cos(parV);


  // DEB_dump_obj__ (Typ_PT, &pt1, "           pt1: ");

  pt1.x = ((tor->r1-tor->r2) + tor->r2 * cosa) * cosi;
  pt1.y = ((tor->r1-tor->r2) + tor->r2 * cosa) * sini;
  pt1.z = tor->r2 * sina;


  // RueckTrafo in die Normallage
  UT3D_pt_tra_pt_m3 (pto, mat, &pt1);  // Punkt transformieren
  // DEB_dump_obj__ (Typ_PT, &pt2, "           pt2: ");



  // DEB_dump_obj__ (Typ_PT, pto, "ex UT3D_pt_evparstor: ");

  return 0;

}
 

//================================================================
  int UT3D_pt_projptbtor (Point *pto, Torus *to1, Point *pti) {
//================================================================
// UT3D_pt_projptbtor        project PT -> Torus

  int     irc;
  Point   pt1;
  Circ    ci1;


  // DEB_dump_obj__ (Typ_TOR, to1, "UT3D_pt_projptbtor");

  // pti auf den hauptkreis projizieren, dann retour -> pti mit r2.

  // make circ ci1 mit hauptachse u. R = r1-r2
  UT3D_ci_ptvcr (&ci1, &to1->pl.po, &to1->pl.vz, to1->r1-to1->r2);
  // prj pti auf diesen circ ci1
  irc = UT3D_pt_projptci (&pt1, NULL, pti, &ci1);


  // wenn r2 > r1:  den vis-a-vis Punkt am Kreis
  if(to1->r1 < to1->r2) {
    UT3D_pt_opp2pt (&pt1, &ci1.pc, &pt1);
  }

  // wieder zurueck -> pti mit laenge r2
  UT3D_pt_traptptlen (pto, &pt1, pti, to1->r2);


  // DEB_dump_obj__ (Typ_PT, pto, "ex UT3D_pt_projptbtor");

  return 0;

}


//================================================================
  int SUR_ck_typ (int *suTyp, ObjGX **sSur, int *cNr, ObjGX *o1) {
//================================================================
/// \code
/// SUR_ck_typ                get typ & index of supportSurf, nr of contours.
/// Output:
///   suTyp    Typ supportSurface:
///     Typ_Error  = Error
///     Typ_SURPLN = Planar surf
///     Typ_SURRV  = Revolved Sur
///     Typ_SURRU  = Ruled Sur
///     Typ_SURBSP = B_Spline Surf
///                  nurb (SBSP)
///   sSur     the ObjGX-Record of the supportSurface
///   cNr      Anzahl Konturen: 0=unbegrenzte Stuetzflaeche; 1=nur Aussenkontur;
///            > 1 = Aussenkontur + (iCont-1) LochKonturen.
///   RetCod:  DB-Index of supportSurface.
/// \endcode

// see also UTO_ck_surfTyp
// see also ../flat/flat.c fl_get_typ_inpTab()

  long    i2;
  ObjGX   *o2, *os;


  *suTyp = Typ_Error;
  *cNr   = 0;


  // printf("SUR_ck_typ %d %d\n",o1->typ,o1->form);
  // DEB_dump_ox_s_ (o1, "SUR_ck_typ");

  *sSur = o1;


  if(o1->typ == Typ_SURRU) {         // Typ_SURRU / Typ_ObjGX
    *suTyp = Typ_SURRU;


  } else if(o1->typ == Typ_SURRV) {  // Typ_SURRU / Typ_ObjGX
    *suTyp = Typ_SURRV;


  } else if(o1->typ == Typ_SURBSP) {  // Typ_SURRU / Typ_ObjGX
    *suTyp = Typ_SURBSP;


  //================================================================
  } else if(o1->form == Typ_ObjGX) {        // FSUB

    // Typ_SUR / Typ_ObjGX

    // o2 = erster Subrecord (Stuetzflaeche)
    o2 = (ObjGX*)o1->data;
    // printf("  data[0].typ=%d form=%d\n",o2->typ,o2->form);


    //------------------------------------------------
    if(o2->typ == Typ_Typ) {  // 106

      i2 = OGX_GET_INT (o2);
      if(i2 == Typ_SURPLN) {
        *suTyp = Typ_SURPLN;
        *cNr   = o1->siz - 1;     // 1 Record ist Typ_Typ
        *sSur  = (ObjGX*)((char*)o2 + sizeof(ObjGX)); // 2.obj=suppSur 2011-08-27

      } else {TX_Error("SUR_ck_typ E006"); return -1;}


    //------------------------------------------------
    } else if(o2->typ == Typ_SUR) {

      if(o2->form == Typ_Index) {  // 100
        i2 = LONG_PTR(o2->data);
        // printf("  StuetzFl.A%d\n",i2);
        os = DB_GetSur (i2, 0);
        // printf("  typ=%d form=%d\n",os->typ,os->form);
        *sSur = os;   // 1. obj = suppSur


        // check Suetzflaeche os:
        if((os->typ == Typ_SURRV)&&(os->form == Typ_SURRV)) {  // 160
          *suTyp = Typ_SURRV;
          *cNr   = o1->siz - 1;

        } else if((os->typ == Typ_SURBSP)&&(os->form == Typ_SURBSP)) {
          *suTyp = Typ_SURBSP;
          *cNr   = o1->siz - 1;

        } else if((os->typ == Typ_SURRU)&&(os->form == Typ_ObjGX)) {
          *suTyp = Typ_SURRU;
          *cNr   = 0;


        } else {TX_Error("SUR_ck_typ E005"); return -1;}


      } else {TX_Error("SUR_ck_typ E004"); return -1;}



    //------------------------------------------------
    } else if(o2->typ == Typ_SURRU) {
      // Typ_SUR / Typ_ObjGX
      //   Typ_SURRU / Typ_ObjGX
      *suTyp = Typ_SURRU;
      *cNr   = o1->siz - 1;     // 1 Record ist Stuetzflaeche


    //------------------------------------------------
    } else if(o2->typ == Typ_SURRV) {
      // Typ_SUR / Typ_ObjGX
      //   Typ_SURRV / Typ_SURRV
      *suTyp = Typ_SURRV;
      *cNr   = o1->siz - 1;     // 1 Record ist Stuetzflaeche


    //------------------------------------------------
    } else if(o2->typ == Typ_SURBSP) {
      // Typ_SUR / Typ_ObjGX
      //   Typ_SURBSP / Typ_SURBSP
      *suTyp = Typ_SURBSP;
      *cNr   = o1->siz - 1;     // 1 Record ist Stuetzflaeche


    //------------------------------------------------
    } else {TX_Error("SUR_ck_typ E003"); return -1;}


  } else {TX_Error("SUR_ck_typ E002"); return -1;}



  // printf("ex SUR_ck_typ typ=%d cNr=%d\n",*suTyp,*cNr);

  return 0;

}


/*
//================================================================
  int SUR_Disp_Sur (ObjGX *o1, Memspc *wrkSpc, int att, ...) {
//================================================================
/// wrkSpc unused !

  int   irc, sTyp, cNr;
  ObjGX *sSur;


  // printf("SUR_Disp_Sur att=%d\n",att);


  // get Typ StuetzFlache, Anzahl Konturen
  irc = SUR_ck_typ (&sTyp, &sSur, &cNr, o1);
  if(irc < 0) return irc;


  switch(sTyp) {

    case Typ_SURPLN:
      GR_Disp_sur (o1, att);
      break;

    case Typ_SURRU:
      if(cNr < 1) GR_Disp_sru (o1, att);
      else GR_Disp_sur (o1, att);
      break;

    case Typ_SURBSP:
      if(cNr < 1) goto L_E001;
      else GR_Disp_sur (o1, att);
      break;

    case Typ_SURRV:
      if(cNr < 1) goto L_E001;
      else GR_Disp_sur (o1, att);
      break;

    default:
      goto L_E001;
  }


  return 0;


  L_E001:
    TX_Error("SUR_Disp_Sur E002_%d_%d",sTyp,cNr);
    return -1;

}
*/
//========== EOF =================================================
