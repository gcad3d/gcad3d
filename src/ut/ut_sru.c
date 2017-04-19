//          Ruled-Surface-Functions    RF.
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
UT3D_sru_ck_planar

-----------------------------------------------------
Modifications:
2004-06-13 neu ex ut_surstd.c. RF.

-----------------------------------------------------
*/
/*!
\file  ../ut/ut_sru.c
\brief Ruled-Surface-Functions 
\code
=====================================================
List_functions_start:

UT3D_sru_ck_planar        check if Ruled_Sur = planar
UT3D_parsru_pt            u/v-Parameter from  3D-ruled-surfacePoint
UT3D_pt_evparsru          3D-point <-- evaluate ruled-surfaceParameters
UT3D_cv_sru               polygons <-- Randkurven
SRU_pt_evparcrv           einen Punkt auf curve errechnen (UT3D_sru_init !)
SRU_CvIso_parsru          get iso-curve from RuledSurface
UT3D_sru_init             init transformationen ..

List_functions_end:
=====================================================

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
#include "../ut/func_types.h"               // UI_Func... SYM_..




static int    SRU_typ1,  SRU_typ2;
static int    SRU_dir1,  SRU_dir2, SRU_clos;
// static ObjX   *SRU_obj1, *SRU_obj2;
static void   *SRU_obj1, *SRU_obj2;
static double SRU_d1, SRU_d2;

static Line   SRU_ln0,SRU_ln1,SRU_ln25,SRU_ln50,SRU_ln75,SRU_ln99,SRU_ln100;

double SRU_parsru_utPt2 ();
double SRU_parsru_utLn2 ();




//============================================================================
  int SRU_CvIso_parsru (void *objo, int *oTyp, Memspc *memSeg,
                      ObjGX *sru, double dp, int iDir) {
//============================================================================
// get iso-curve from RuledSurface
// Input:
//   sru        RuledSurface
//   dp         parameter 0-1
//   iDir       1=along, 2=across
// Output:
//   oTyp       Typ_LN|..
//   RetCod:

// see also APT_iso_cktypSRU

  int     irc, i1, typ1, typ2;
  long    oSiz;
  ObjGX   *oTab;
  void    *dat1, *dat2; //*oDat;
  Point   p1, p2, p3, p4;
  Vector  vc1;


  // printf("SRU_CvIso_parsru %f %d \n",dp,iDir);

  // across: always line
  // along: 0=bottomObj; 1=TopObj; between=CvBspl

  oTab = (ObjGX*)sru->data;

  //------ 2 = across always line --------------------------------------
  if(iDir == 2) {
    *oTyp = Typ_LN;
    if(objo == NULL) goto L_exit;

    // get the parametric points on both baseCurves
    // create line from this points
    // 1. obj.
      UT3D_stru_dump (Typ_ObjGX, &oTab[0], "obj:\n");
    typ1 = UTO_obj_getp (&dat1, &i1, &oTab[0]);  // get data-struct
      UT3D_stru_dump (typ1, dat1, "typ1=%d obj:\n",typ1);
    UT3D_pt_evparcrv (&((Line*)objo)->p1, dp, typ1, dat1);
    // 2. obj.
    typ1 = UTO_obj_getp (&dat1, &i1, &oTab[1]);
    if(typ1 == Typ_VC) {
      // translate p1 um vector ..
      UT3D_pt_traptvc (&((Line*)objo)->p2, &((Line*)objo)->p1, (Vector*)dat1);
    } else {
      UT3D_pt_evparcrv (&((Line*)objo)->p2, dp, typ1, dat1);
    }
    goto L_exit;
  }



  //------ 1 = along --------------------------------------
  //  dp=0
  if(dp < UT_TOL_min1) {
    // get type of SRU-obj1
    *oTyp = UTO_obj_getp (&dat1, &i1, &oTab[0]);  // get data-struct
    if(objo == NULL) goto L_exit;
    // copy obj1
    i1 = UTO_siz_stru (*oTyp);
      printf(" typ=%d siz=%d\n",oTab[0].form,i1);
    memcpy (objo, dat1, i1);


  //----------------------------------------------------------------
  // dp=1
  } else if (dp > 1. - UT_TOL_min1) {
    // get type of SRU-obj2
    *oTyp = UTO_obj_getp (&dat2, &i1, &oTab[1]);  // get data-struct
    // SRU from obj & Vector:
    if(*oTyp == Typ_VC) {
      vc1 = *(Vector*)dat2;   // copy vector
        UT3D_stru_dump (Typ_VC, &vc1, "vc1:");
      // get obj1
      *oTyp = UTO_obj_getp (&dat1, &i1, &oTab[0]);  // get data-struct
      if(objo == NULL) goto L_exit;
      // translate obj1
      UTRA_def__ (1, Typ_VC, &vc1);
      UTRA_app__ (objo, *oTyp, *oTyp, 1, dat1, memSeg);
      goto L_exit;
    }
    // copy obj2
    if(objo == NULL) goto L_exit;
    i1 = UTO_siz_stru (*oTyp);
      printf(" typ=%d siz=%d\n",oTab[0].form,i1);
    memcpy (objo, dat2, i1);



  //----------------------------------------------------------------
  // dp>0; dp<1
  } else {

    typ1 = UTO_obj_getp (&dat1, &i1, &oTab[0]);  // get data-struct
    typ2 = UTO_obj_getp (&dat2, &i1, &oTab[1]);  // get data-struct
      // UT3D_stru_dump (typ1, dat1, "dat1:\n");
      // UT3D_stru_dump (typ2, dat2, "dat2:\n");
      printf(" typ1=%d typ2=%d\n",typ1,typ2);


    // obj & vector -> obj ..
    if(typ2 == Typ_VC) {
      *oTyp = typ1;
      if(objo == NULL) goto L_exit;
      // modify vector
      UT3D_vc_multvc (&vc1, (Vector*)dat2, dp);
      // translate obj1
      UTRA_def__ (1, Typ_VC, &vc1);
      UTRA_app__ (objo, *oTyp, *oTyp, 1, dat1, memSeg);
      goto L_exit;
    }


    // line|point & line|point -> line ..
    if(((typ1 == Typ_LN) && (typ2 == Typ_PT))  ||
       ((typ1 == Typ_PT) && (typ2 == Typ_LN))  ||
       ((typ1 == Typ_LN) && (typ2 == Typ_LN))) {
      *oTyp = Typ_LN;
      if(objo == NULL) goto L_exit;
      // Startpunkt & Endpunkt von obj1
      // UTO_2pt_limstru (&p1, &p2, NULL, NULL, typ1, dat1);
      irc = UT3D_ptvcpar_std_obj (&p1, NULL, NULL, Ptyp_0, typ1, dat1);
      irc = UT3D_ptvcpar_std_obj (&p2, NULL, NULL, Ptyp_1, typ1, dat1);
      // Startpunkt & Endpunkt von obj2
      // UTO_2pt_limstru (&p3, &p4, NULL, NULL, typ2, dat2);
      irc = UT3D_ptvcpar_std_obj (&p3, NULL, NULL, Ptyp_0, typ2, dat2);
      irc = UT3D_ptvcpar_std_obj (&p4, NULL, NULL, Ptyp_1, typ2, dat2);
      // modify p1-p2 with parameter dp
      UT3D_pt_evpar2pt (&((Line*)objo)->p1, dp, &p1, &p3);
      UT3D_pt_evpar2pt (&((Line*)objo)->p2, dp, &p2, &p4);
      goto L_exit;
    }


    // circ & circ -> circ ..
    if(((typ1 == Typ_CI) && (typ2 == Typ_PT))  ||
       ((typ1 == Typ_PT) && (typ2 == Typ_CI))  ||
       ((typ1 == Typ_CI) && (typ2 == Typ_CI))) {
      *oTyp = Typ_CI;
      if(objo == NULL) goto L_exit;
      // 2 circ's: axis must be equal
      if((typ1 == Typ_CI) && (typ2 == Typ_CI)) {
        if(UT3D_comp2vc_d(&((Circ*)dat1)->vz,
                          &((Circ*)dat2)->vz,UT_TOL_cv) == 0) goto L_bspl;
      }
      // Startpunkt & Endpunkt von obj1
      // UTO_2pt_limstru (&p1, &p2, NULL, NULL, typ1, dat1);
      irc = UT3D_ptvcpar_std_obj (&p1, NULL, NULL, Ptyp_0, typ1, dat1);
      irc = UT3D_ptvcpar_std_obj (&p2, NULL, NULL, Ptyp_1, typ1, dat1);
      // Startpunkt & Endpunkt von obj2
      // UTO_2pt_limstru (&p3, &p4, NULL, NULL, typ2, dat2);
      irc = UT3D_ptvcpar_std_obj (&p3, NULL, NULL, Ptyp_0, typ2, dat2);
      irc = UT3D_ptvcpar_std_obj (&p4, NULL, NULL, Ptyp_1, typ2, dat2);
      // vz1-vz2 antiparallel ?
      if(UT3D_sid_2vc(&((Circ*)dat1)->vz, &((Circ*)dat2)->vz) < 0)
        MEM_swap__(&p3, &p4, sizeof(Point));
      // modify p1-p2 with parameter dp
      UT3D_pt_evpar2pt (&((Circ*)objo)->p1, dp, &p1, &p3);
      UT3D_pt_evpar2pt (&((Circ*)objo)->p2, dp, &p2, &p4);
      if(typ1 == Typ_CI) p1 = ((Circ*)dat1)->pc;  // else keep PT
      if(typ2 == Typ_CI) p3 = ((Circ*)dat2)->pc;  // else keep PT
      UT3D_pt_evpar2pt (&((Circ*)objo)->pc, dp, &p1, &p3);
      // fix radius & angle
      ((Circ*)objo)->rad = UT3D_len_2pt (&((Circ*)objo)->pc, &((Circ*)objo)->p1);
      if(((Circ*)dat1)->rad < 0.) ((Circ*)objo)->rad *= -1.;
      if(typ1 == Typ_CI) ((Circ*)objo)->vz = ((Circ*)dat1)->vz;
      else               ((Circ*)objo)->vz = ((Circ*)dat2)->vz;
      UT3D_ci_setangr (objo);
      goto L_exit;
    }

    L_bspl:
    *oTyp = Typ_CV;   // B-Spl
    if(objo == NULL) goto L_exit;
    TX_Error("not yet implemented ..");
    return -1;
  }




  //----------------------------------------------------------------
  L_exit:

    // GR_Disp_ost (objo, *oTyp, memSeg, 9);
    printf(" ex SRU_CvIso_parsru %d\n",*oTyp);

  return 0;

}


//=========================================================
  double UT3D_sru_ck_planar (ObjGX *oxi) {
//=========================================================



  int     typ1, typ2, io1=0, io2=1, rNr;
  ObjGX   *oa;
  void    *od1, *od2;


  printf("UT3D_sru_ck_planar typ=%d form=%d siz=%d\n",
             oxi->typ,oxi->siz,oxi->form);

  printf("***** ERROR UT3D_sru_ck_planar *******\n");

  printf("Sur_RU_ck_pln \n");


  // get types of 2 ruler-objects
  oa = oxi->data;
  typ1 = oa[io1].typ;
  typ2 = oa[io2].typ;
    printf(" typ1=%d typ2=%d\n",typ1,typ2);


  // sort types ascending
  if(typ2 > typ1) {
    MEM_swap_int (&typ1, &typ2);  // swap types
    MEM_swap_int (&io1, &io2);    // swap indices
  }


  // get datastructs -> od1, od2
  // curves: get curve-subtyp (Typ_CVELL,Typ_CVBSP)
  typ1 = UTO_obj_getp (&od1, &rNr, &oa[io1]);
  typ2 = UTO_obj_getp (&od2, &rNr, &oa[io2]);
  // if(typ1 == Typ_CV) typ1 = UTO_obj_getp (&od1, &oa[io1]);
  // if(typ2 == Typ_CV) typ2 = UTO_obj_getp (&od2, &oa[io2]);
    printf(" typ1=%d io1=%d typ2=%d io2=%d\n",typ1,io1,typ2,io2);
    UT3D_stru_dump (typ1, od1, "  od1:");
    UT3D_stru_dump (typ2, od2, "  od2:");


  // get 2 polygons of datastructs od1, od2



  //================================================================
  if(typ1 == Typ_PT) {

    if(typ2 == Typ_LN) {
      // always coplanar
      goto L_nyi;

    } else if(typ2 == Typ_CI) {
      // test if point is in plane of circ
      goto L_nyi;

    } else if(typ2 == Typ_VC) {
      // test if vec is in plane of circ
      goto L_nyi;

    } else if(typ2 == Typ_CVBSP) {
      // test if pt is in plane of curv
      goto L_nyi;

    } else if(typ2 == Typ_CVELL) {
      // test if pt is in plane of curv
      goto L_nyi;
    }

  //================================================================
  } else if(typ1 == Typ_LN) {

    if(typ2 == Typ_LN) {
      // LN-LN; check for coplanar
      goto L_nyi;

    } else if(typ2 == Typ_CI) {
      goto L_nyi;
    } else if(typ2 == Typ_VC) {
      goto L_nyi;
    } else if(typ2 == Typ_CVBSP) {
      goto L_nyi;
    } else if(typ2 == Typ_CVELL) {
      goto L_nyi;
    }

  //================================================================
  } else if(typ1 == Typ_CI) {

    if(typ2 == Typ_CI) {
      goto L_nyi;
    } else if(typ2 == Typ_VC) {
      goto L_nyi;
    } else if(typ2 == Typ_CVBSP) {
      goto L_nyi;
    } else if(typ2 == Typ_CVELL) {
      goto L_nyi;
    }

  //================================================================
  } else if(typ1 == Typ_VC) {

    if(typ2 == Typ_CVBSP) {
      goto L_nyi;
    } else if(typ2 == Typ_CVELL) {
      goto L_nyi;
    }


  //================================================================
  } else if(typ1 == Typ_CVBSP) {

    if(typ2 == Typ_CVBSP) {
      goto L_nyi;
    } else if(typ2 == Typ_CVELL) {
      goto L_nyi;
    }

  //================================================================
  } else if(typ1 == Typ_CVELL) {

    if(typ2 == Typ_CVELL) {
      goto L_nyi;
    }

  }


  return 0;

  L_nyi:
    printf("***** Sur_RU_ck_pln NYI %d %d\n",typ1,typ2);
    return -1;

}


//==============================================================================
  int UT3D_sru_init (int typ1, int dir1, void *obj1,
                     int typ2, int dir2, void *obj2, int closed) {
//==============================================================================
// closed: 0=not closed; 1=closed curve.


  // printf("UT3D_sru_init typ1=%d dir1=%d typ2=%d dir2=%d clo=%d\n",
          // typ1,dir1,typ2,dir2,closed);



  SRU_typ1 = typ1;
  SRU_typ2 = typ2;

  SRU_dir1 = dir1;
  SRU_dir2 = dir2;

  SRU_obj1 = obj1;
  SRU_obj2 = obj2;

  SRU_clos = closed;


  // if(SRU_typ1 == Typ_CVBSP) {
    // printf(" v0=%f v1=%f\n",SRU_obj1->bsp->v0,SRU_obj1->bsp->v1);
    // SRU_d1 = (oxp->bsp->v1 - oxp->bsp->v0);
  // }


  return 0;

}
 

//==================================================================
  int UT3D_cv_sru_ (Point *p1Tab, Point *p2Tab, int *ptNr, int ptMax) {
//==================================================================
// Randkurven in Polygone zerlegen
// Sonderfall: Typ_PT nur 1 pt (irc = 1/2).
// BRAUCHT UT3D_sru_init !!
// irc   0  beide p1Tab befuellt mit mind. 2 Punkten
//       1  p1Tab hat nur 1 Punkt
//       2  p2Tab hat nur 1 Punkt

 
  int    i1, irc;


  // printf("UT3D_cv_sru_ %d %d\n",*ptNr,ptMax);
  // printf(" SRU_typ1=%d SRU_typ2=%d\n",SRU_typ1,SRU_typ2);



  // BSP + BSP sofort
  if((SRU_typ1 == Typ_CVBSP)&&(SRU_typ2 == Typ_CVBSP)) {
    bspl_pol2_bsp2(p1Tab,p2Tab,ptNr,ptMax,
                   (CurvBSpl*)SRU_obj1,(CurvBSpl*)SRU_obj2,UT_DISP_cv);
    return 0;
  }



  //----------------------------------------------------------------
  // get obj1-polygon -> p1Tab
  UT3D_cv_sruCv (p1Tab, ptNr, ptMax, SRU_typ1, SRU_dir1, SRU_obj1);


  if(SRU_typ2 == Typ_VC) {
    // alle Punkte um Vector vc1 translieren
    for(i1=0; i1<*ptNr; ++i1) {
      UT3D_pt_traptvc (&p2Tab[i1], &p1Tab[i1], (Vector*)SRU_obj2);
    }

  } else {
    // get obj2-polygon -> p2Tab
    UT3D_cv_sruCv (p2Tab, ptNr, ptMax, SRU_typ2, SRU_dir2, SRU_obj2);
  }

  if(SRU_typ1 == Typ_PT)      irc = 1;
  else if(SRU_typ2 == Typ_PT) irc = 2;
  else                        irc = 0;

  // printf("ex UT3D_cv_sru_ %d %d %d\n",irc,*ptNr,ptMax);
  return irc;

}


//==================================================================
  int UT3D_cv_sruCv (Point *pTab, int *ptNr, int ptMax,
                     int typ, int dir, void *oxx) {
//==================================================================
// Obj oxx in ein Polygon mit fixer Punkteanzahl ptNr umwandeln.
// dir=direction; 0=normal, 1=reverse.


  int    i1, irc;


  // printf("UT3D_cv_sruCv ptNr=%d ptMax=%d typ=%d dir=%d\n",
                    // *ptNr,ptMax,typ,dir);


  if(typ == Typ_PT) {
    // Sonderfall; FAN stat STRIP; nur ein Punkt notwendig
    pTab[0] = *(Point*)oxx;
  

  //-------------------------------
  } else if(typ == Typ_LN) {
    UT3D_cv_ln (pTab, ptNr, &((Line*)oxx)->p1, &((Line*)oxx)->p2);


  //-------------------------------
  } else if(typ == Typ_CI) {
    UT3D_cv_ci (pTab, ptNr, (Circ*)oxx, *ptNr, -1.);


  //-------------------------------
  } else if(typ == Typ_CVELL) {
    // UT3D_cv_ell (pTab, ptNr, (CurvElli*)oxx, ptMax, -1.);
    UT3D_npt_ell (*ptNr, pTab,  (CurvElli*)oxx);


  //-------------------------------
  } else if(typ == Typ_CVPOL) {
    if(ptMax < ((CurvPoly*)oxx)->ptNr) goto L_err_pTab;
    *ptNr = ((CurvPoly*)oxx)->ptNr;
    memcpy (pTab, ((CurvPoly*)oxx)->cpTab, sizeof(Point) * *ptNr);


  //-------------------------------
  } else if(typ == Typ_CVBSP) {   // oxx->bsp->v1 ..
    // bspl_pol_bsp (ptNr, pTab, oxx->bsp, ptMax, UT_DISP_cv);
    bspl_cv_bsp (ptNr, pTab, (CurvBSpl*)oxx);
    // irc = bspl_cvpol_cvbsp(



  //-------------------------------
  } else if(typ == Typ_ObjGX) {
      // UTO_dump_s_  ((ObjGX*)oxx, " CCV:");
      // UTO_dump__ ((ObjGX*)oxx, " CCV:");
    *ptNr = ptMax;
    irc = UT3D_pta_ox_lim (ptNr, pTab, (ObjGX*)oxx, 0, NULL, UT_DISP_cv);
    if(irc < 0) return -1;
      // GR_Disp_pTab (*ptNr, pTab, SYM_STAR_S, 2);
      // for(i1=0;i1<*ptNr;++i1) UT3D_stru_dump(Typ_PT,&pTab[i1]," pt[%d]=",i1);


  //-------------------------------
  } else {
    TX_Error("UT3D_cv_sruCv E001 %d",typ);
    return -1;
  }


  // Testdisp.
  // GR_Disp_pTab (*ptNr, pTab, SYM_TRI_S, 4);
  // printf("ex UT3D_cv_sruCv %d\n",*ptNr);


  if(dir != 0) UT3D_cv_inv (*ptNr, pTab);    // PunkteTab. umdrehen
  return 0;
   



  L_err_pTab:
    TX_Error("UT3D_cv_sruCv E002 - too much points");
    return -1;

}

/*
//=====================================================================
  int UT3D_parsru_pt (double *parU, double *parV, Point *pt3) {
//=====================================================================
// einen 3D-Punkt einer RuledSurf auf 2D-Werte umrechnen
// ACHTUNG: init mit UT3D_sru_init und UT3D_parsru_pt (..NULL);
// 3D-Point pt3 -> 2D-Point pt2 fuer RuledSurf
// Methode: 2 Verbindungslinien generieren; den Normalabst von pt3 an
// beide errechnen; je nach abstand (dp1/dp2) neue Mittellinie errechnen
// und Zonenbreite entsprechend Abstand. Zone = die beiden neuen
// Verbindungslinien - naechster Durchlauf.

// Besser waere: (Variante 3) Aufgrund der letzten beiden Versuche die
// neue Verbindungslinienposi errechen (wie rMid aus Vers. 2);
// den aeltesten Versuch verwerfen usw.



static Point  pa, pe;
static Vector vca, vce;

  int    irc, iloop;
  double dp1, dp2, u1, u2, uMid, uVar, dTot, uTot;
  Point  pb1, pb2, px1, px2;
  Vector vc1, vc2;



  // Ausgabe erst nach Init!



  //---------------------------------------------------------
  if(pt3 == NULL) {  // INIT; Start und Endlinie setzen
    irc = SRU_pt_evparcrv (&pa,  0., 1);
    irc = SRU_pt_evparcrv (&pe,  1., 1);
    irc = SRU_pt_evparcrv (&px1, 0., 2);
    irc = SRU_pt_evparcrv (&px2, 1., 2);
    UT3D_vc_2pt (&vca, &pa, &px1);
    UT3D_vc_2pt (&vce, &pe, &px2);
      UT3D_stru_dump (Typ_VC, &vca, " vca=");
      UT3D_stru_dump (Typ_VC, &vce, " vce=");
      // GR_Disp_vc (&vca, &pa, 2, 1);
      // GR_Disp_vc (&vce, &pe, 2, 1);
    return irc;
  }



  //---------------------------------------------------------
  UT3D_stru_dump (Typ_PT, pt3, "......................\nUT3D_parsru_pt ");
  GR_Disp_pt (pt3, SYM_STAR_S, 2);






  printf("ex UT3D_parsru_pt %f %f\n",*parU,*parV);

  return irc;

}
*/



//=====================================================================
  int UT3D_parsru_pt (double *parU, double *parV, Point *pt3) {
//=====================================================================
// einen 3D-Punkt einer RuledSurf auf 2D-Werte umrechnen
// ACHTUNG: init mit UT3D_sru_init und UT3D_parsru_pt (..NULL);
// 3D-Point pt3 -> 2D-Point pt2 fuer RuledSurf
// Methode: 2 Verbindungslinien generieren; den Normalabst von pt3 an
// beide errechnen; je nach abstand (dp1/dp2) neue Mittellinie errechnen
// und Zonenbreite entsprechend Abstand. Zone = die beiden neuen
// Verbindungslinien - naechster Durchlauf.

// Basic: die beiden Kurve sind parallel zur X-Achse.
// parU=Abst auf den Kurven; (0-1)
// parV= Abst auf der verbindungsgerade (0-1)


// closed: es wird eine Ebene durch die Startlinie ln0 und den Startpunkt der
// ln50 (genau mittig) gelegt; hat den Z-Vektor vcn. Nun testen, ob pt3 ueber
// oder unter der Plane liegt.


// Manchmal bleibt uMid genau auf 0.5000 und nix geht mehr .. (schon unter
// der Aufloesungsgrenze ?

// Andere Technik waere:
// Immer nur eine Testlinie machen, aber mit den Normalvektor testen.
// Wie bekommt man einen Normalvektor ? Eigentlich gar nicht !


//                  .p2        .p2              
//         -----------------|-----------------
//         |          |     |    |           |
//         |          |     |    |           |
//         |          |     |    |           |
//        ln0       ln1    ln50 ln2         ln100
//         |          |     |    |           |
//         |       px1|     |    |px2        |
//         |          x  X  |    x           |
//         |          | pt3 |    |           |
//         -----------------|-----------------
//         pa       .p1         .p1          pe



// static Point  pa, pe;
// static Vector vcn;
static Line   ln0, ln10, ln25, ln50, ln75, ln90, ln100;
// static double cloTot;
// static double oldTot;

  int    irc, iloop, i1, i2;
  double d1, d2, dp1, dp2, u1, u2, uMid, uVar, dTot, uTot, uRange;
  double dTab[5];
  Point  px1, px2, pTab[5];
  Line   ln1, ln2;
  Vector vc1, vc2, vc3, vc4, vv;



  // Ausgabe erst nach Init!


  //---------------------------------------------------------
  if(pt3 == NULL) {  // INIT; Start und Endlinie setzen

    // printf(" clo=%d tol=%f\n",SRU_clos,UT_TOL_pt);

    // NUR TEST !!!!!!!!
// irc = SRU_pt_evparcrv (&ln50.p1, 0.5, 1);
// exit(0);
    // END TEST !!!!!!!!


    irc = SRU_pt_evparcrv (&ln0.p1, 0.0, 1);
    irc = SRU_pt_evparcrv (&ln0.p2, 0.0, 2);
    irc = SRU_pt_evparcrv (&ln25.p1, 0.25, 1);
    irc = SRU_pt_evparcrv (&ln25.p2, 0.25, 2);
    irc = SRU_pt_evparcrv (&ln50.p1, 0.5, 1);
    irc = SRU_pt_evparcrv (&ln50.p2, 0.5, 2);
    irc = SRU_pt_evparcrv (&ln75.p1, 0.75, 1);
    irc = SRU_pt_evparcrv (&ln75.p2, 0.75, 2);

    // UT3D_stru_dump (Typ_LN, &ln0, " ln0=\n");
    // UT3D_stru_dump (Typ_LN, &ln25, " ln25=\n");
    // UT3D_stru_dump (Typ_LN, &ln50, " ln50=\n");
    // UT3D_stru_dump (Typ_LN, &ln75, " ln75=\n");

/*
      // printf("UT3D_parsru_pt Init %d %d\n",SRU_typ1,SRU_typ2);
    // Start- u Endpt BottomCurve
    irc = SRU_pt_evparcrv (&pa,  0., 1);
    irc = SRU_pt_evparcrv (&pe,  1., 1);
    // Start- u Endpt TopCurve
    irc = SRU_pt_evparcrv (&px1, 0., 2);
    irc = SRU_pt_evparcrv (&px2, 1., 2);
    // vecs der Verbindungslinien
    UT3D_vc_2pt (&vca, &pa, &px1);
    UT3D_vc_2pt (&vce, &pe, &px2);
      // UT3D_stru_dump (Typ_VC, &vca, " vca=");
      // UT3D_stru_dump (Typ_VC, &vce, " vce=");
      // GR_Disp_vc (&vca, &pa, 2, 1);
      // GR_Disp_vc (&vce, &pe, 2, 1);
*/

    if(SRU_clos == 0) {         // normal - not closed
      irc = SRU_pt_evparcrv (&ln100.p1, 1.0, 1);
      irc = SRU_pt_evparcrv (&ln100.p2, 1.0, 2);

    } else {   // closed;
      // durch die Startlinie ln0 und den Startpunkt der ln50 eine Plane -
      // vcn = Z-Vektor dieser Plane.
      // UT3D_vc_perp3pt (&vcn, &ln0.p1, &ln50.p1, &ln0.p2);
      // UT3D_vc_perp3pt (&vcn, &ln0.p1, &ln0.p2, &ln50.p1);
      irc = SRU_pt_evparcrv (&ln10.p1, 0.1, 1);
      irc = SRU_pt_evparcrv (&ln10.p2, 0.1, 2);
      irc = SRU_pt_evparcrv (&ln90.p1, 0.9, 1);
      irc = SRU_pt_evparcrv (&ln90.p2, 0.9, 2);
    }

/*
    // closed: uTot und Start- und Endwinkel bestimmen
      if(SRU_typ1 == Typ_CVBSP) {
        pTab = SRU_obj1->bsp->cpTab;
        i1 = SRU_obj1->bsp->ptNr;
        // printf(" 1-ptNr=%d\n",i1);
        if(SRU_dir1 == 0) { UT3D_vc_2pt(&vc1, &pTab[0], &pTab[1]);}
        else              { UT3D_vc_2pt(&vc1, &pTab[i1-2], &pTab[i1-1]);}
        d1 = 0.;
        for(i2=1; i2<i1; ++i2) d1 += UT3D_len_2pt(&pTab[i2-1], &pTab[i2]);

      } else if(SRU_typ1 == Typ_CI) { 
          d1 = 0.;
          d2 = 1.;

      } else { 
        TX_Error ("UT3D_parsru_pt E511_%d",SRU_typ1);
        return -1;
      }

      if(SRU_typ2 == Typ_CVBSP) {
        pTab = SRU_obj2->bsp->cpTab;
        i1 = SRU_obj2->bsp->ptNr;
        // printf(" 2-ptNr=%d\n",i1);
        if(SRU_dir2 == 0) { UT3D_vc_2pt(&vc2, &pTab[0], &pTab[1]); }
        else              { UT3D_vc_2pt(&vc2, &pTab[i1-2], &pTab[i1-1]); }
        d2 = 0.;
        for(i2=1; i2<i1; ++i2) d2 += UT3D_len_2pt(&pTab[i2-1], &pTab[i2]);

      } else if(SRU_typ2 != Typ_CI) {
        TX_Error ("UT3D_parsru_pt E512_%d",SRU_typ2);
        return -1;

      }

      UT3D_vc_merge2vc(&cloVc0, &vc1, &vc2);
      // UT3D_vc_merge2vc(&cloVc1, &vc3, &vc4);
      cloTot = (d1 + d2) / 2.;
      // UT3D_stru_dump(Typ_VC, &cloVc0,"cloTot=%f cloVc0=",cloTot);
    }
*/

    return irc;
  }



  //---------------------------------------------------------
  // printf("============================================= \n");
  // UT3D_stru_dump (Typ_PT, pt3, "UT3D_parsru_pt:");
  // GR_Disp_pt (pt3, SYM_TRI_S, 2);

  irc = 0;

  iloop  = 0;  // Loopcounter; abbrechen nach 15 Versuchen.



  // u1 und u2 fuer Start feststellen.
  // closed ist ein Problem; hier sind pa/pe und px1/px2 ident.
  // Man muss (besonders wichtig in der Naehe von par=0=1) feststellen,
  //   auf welcher Haelfte der Kurve.

    // Abstaende dp1 und dp2 von den Punkte px1/px2;
    UT3D_pt_projpt2pt (&pTab[0], &dTab[0], pt3, &ln0.p1, &ln0.p2);   // 0.
    UT3D_pt_projpt2pt (&pTab[1], &dTab[1], pt3, &ln25.p1, &ln25.p2); // 0.25
    UT3D_pt_projpt2pt (&pTab[2], &dTab[2], pt3, &ln50.p1, &ln50.p2); // 0.5
    UT3D_pt_projpt2pt (&pTab[3], &dTab[3], pt3, &ln75.p1, &ln75.p2); // 0.75
    if(SRU_clos == 0) {   // normal - not closed
      UT3D_pt_projpt2pt (&pTab[4], &dTab[4], pt3, &ln100.p1, &ln100.p2); // 1.0
    } else {
      ln100 = ln0;
      pTab[4] = pTab[0]; // ln100 == ln0
      dTab[4] = dTab[0];
    }



    // nun den geringsten Abst. suchen
    i1 = UTP_min (5, dTab);

    // printf(" i1=%d\n",i1);
    // for(i2=0;i2<5;++i2) printf(" dTab[%d]=%f\n",i2,dTab[i2]);


    // closed: Sonderbehandlung wenn i1=0 oder i1=3
    if(SRU_clos != 0) {
      if((i1 == 0)||(i1 == 4)) {

        // printf(" closed-near-start/end\n");
        UT3D_pt_projpt2pt (&px1, &d1, pt3, &ln10.p1, &ln10.p2); // 0.1
        UT3D_pt_projpt2pt (&px2, &d2, pt3, &ln90.p1, &ln90.p2); // 0.9
        // printf(" d1=%f d2=%f\n",d1,d2);
        if(d1 < d2) {
          u1 = 0.; u2 = 0.25;
          px1 = pTab[0];
          px2 = pTab[1];
        } else {
          u1 = 0.75; u2 = 1.0;
          px1 = pTab[3];
          px2 = pTab[4];
        }
        if(fabs(d1) < UT_TOL_pt) {*parU = 0.; ln1 = ln0; goto L_comp_V;}
        uRange = 0.13;
        uTot = UT3D_len_2pt (&px1, pt3) + UT3D_len_2pt (&px2, pt3);
        goto L_next_x;
      }
    }

    if(i1 == 0) {                 // 0 - 0.25
      u1 = 0.; u2 = 0.25; uRange = 0.13;
      px1 = pTab[0];
      px2 = pTab[1];
      d1 = UT3D_len_2pt (&px1, pt3);
      if(fabs(d1) < UT_TOL_pt) {*parU = 0.; ln1 = ln0; goto L_comp_V;}

    } else if(i1 == 1) {          // 0 - 0.5
      u1 = 0.; u2 = 0.5; uRange = 0.25;
      px1 = pTab[0];
      px2 = pTab[2];

    } else if(i1 == 2) {          // 0.25 - 0.75
      u1 = 0.25; u2 = 0.75; uRange = 0.25;
      px1 = pTab[1];
      px2 = pTab[3];

    } else if(i1 == 3) {          // 0.5 - 1.0
      u1 = 0.5; u2 = 1.0; uRange = 0.25;
      px1 = pTab[2];
      px2 = pTab[4];

    } else if(i1 == 4) {          // 0.75 - 1.0
      u1 = 0.75; u2 = 1.0; uRange = 0.13;
      px1 = pTab[3];
      px2 = pTab[4];  // not-closed: pTab[4] !
      d1 = UT3D_len_2pt (&px2, pt3);
      if(fabs(d1) < UT_TOL_pt) {*parU = 1.; ln1 = ln100; goto L_comp_V;}
    }

    // uTot = UT3D_len_2pt (&px1, &px2);
    uTot = UT3D_len_2pt (&px1, pt3) + UT3D_len_2pt (&px2, pt3);
    goto L_next_x;


    


  //----------------------------------------------------------------
  // Zuerst U-Parameter suchen (Offset Kontur);
  // V-Par. (offset auf der Verbindungslinie) erst wenn U-Par. gefunden.
  // pt3 = Inputpoint
  // dp1 = Strecke vom pt3 zu einem Normalpunkt px1 auf Verb.Linie ln1
  // dp2 = Strecke vom pt3 zu einem Normalpunkt px2 auf Verb.Linie ln2
  // uTot = Strecke px1-px2; wenn < TolPT: U-Par. gefunden.
  // uRange = der HALBE Parameterbereich von ln1 bis ln2 (px1 - px2)

  L_next_x:
  ++iloop;

  // printf(" parsru_pt i=%d u1=%f u2=%f\n",iloop,u1,u2);

  // ab hier muss gegeben sein:
  // uRange - der Bereich
  // u1, u2 - die U-Parameter zu den Punkten px1, px2
  // px1, px2 - die Punkte
  // uTot - Abst. px1-px2



  // uVar ermitteln. uVar zeigt an, wo sich pt3 zwischen px1-px2 befindet.
  //   zB uVar=0.5  - pt3 ist mittig zw px1-px2
  // 
  // Bereich noch gross genug fuer Vektor in Richtung Kontur ?
  // wenn nicht, einfach den alten Vektor benutzen ..
  // if(uTot > UT_TOL_PAR) {
  if(uTot > UT_TOL_min1) {
    UT3D_vc_2pt (&vv, &px1, &px2);
    i1 = UT3D_sid_ptptvc (pt3, &px1, &vv);  // ist pt3 vor oder hinter px1
    // wenn i1 == 0 dann sollte uMid=u1 gesetzt werden...
    if(i1 == 0) {uMid = u1; uRange /= 4.; goto L_mid;}

    // den abst. px1-pt3 in Relation zum Abstand px1-pt3-px2 setzen;
    // Achtung: nur der Abstand von pt3 auf der Achse px1px2 ist zu ungenau (es
    // kommt zum schwingen !)
    d1 = UT3D_len_2pt (&px1, pt3);           // dist px1-pt3
    if(d1 < UT_TOL_pt) goto L_done;

    uVar = d1 / uTot * i1;                    // signed dist.


  } else {
    // px1-px2 sind nun ganz eng beisammen. Vec. vv sollte aber stimmen.
    d1 = UT3D_slen_2ptvc (&px1, pt3, &vv);
    if(fabs(d1) < UT_TOL_pt) goto L_done;
    uVar = d1 / uTot;                    // signed dist.
  }


  // printf(" uTot=%f d1=%f uVar=%f uRange=%f i1=%d\n",uTot,d1,uVar,uRange,i1);

  

  // neuen Mittelwert uMid errechnen. 
  L_start:

  // wenn ca in der Mitte den bereich verkleinern.
  if(uRange > 0.0000001) {
    if     ((uVar > 0.48)&&(uVar < 0.52)) uRange /= 6.;   // 12
    else if((uVar > 0.45)&&(uVar < 0.55)) uRange /= 5.;   // 8
    else if((uVar > 0.4)&&(uVar < 0.6))   uRange /= 4.;   // 6
    else if((uVar > 0.3)&&(uVar < 0.7))   uRange /= 3.;   // 4
    else if((uVar > 0.1)&&(uVar < 0.9))   uRange /= 2.;   // 2
  }
       if(uVar >  3.0) uVar =  3.0;
  else if(uVar < -3.0) uVar = -3.0;





  uMid = u1 + ((u2-u1)*uVar);
  if     (uMid > 1.0) uMid = 1.0;
  else if(uMid < 0.0) uMid = 0.0;


  // Ermitteln von 2 Testparameterwerten auf den Curves (u1 und u2).
  // u1, u2 aus uMid und uRange errechnen.
  L_mid:
  u1 = uMid - uRange;
  u2 = uMid + uRange;

  if(u1 < 0.) {u2 += u1/2.; u1=0.; uRange=(u2-u1)/2.;}  // u2 verkleinern !
  if(u2 > 1.) {u1 += (u2-1.)/2.; u2=1.; uRange=(u2-u1)/2.;}  // u1 vergroessern

  // printf(" u1=%f u2=%f  uMid=%f uR=%f\n",u1,u2,uMid,uRange);



  //----------------------------------------------------------------
  // u1, u2 sind nun 2 Testparameter auf den Curves.
  // Die zugehoereigen Punkte auf Bottom- und TopCurve errechnen,
  // daraus die Linien ln1/ln2 generieren.
  irc = SRU_pt_evparcrv (&ln1.p1, u1, 1);
  irc = SRU_pt_evparcrv (&ln2.p1, u2, 1);
  irc = SRU_pt_evparcrv (&ln1.p2, u1, 2);
  irc = SRU_pt_evparcrv (&ln2.p2, u2, 2);
    // GR_Disp_pt (&ln1.p1, SYM_TRI_S, 1);
    // GR_Disp_pt (&ln1.p2, SYM_TRI_S, 1);
    // GR_Disp_pt (&ln2.p1, SYM_TRI_S, 2);
    // GR_Disp_pt (&ln2.p2, SYM_TRI_S, 2);

  // aus den 4 Punkten 2 Verbindungslinien machen
  UT3D_vc_2pt (&vc1, &ln1.p1, &ln1.p2);
  UT3D_vc_2pt (&vc2, &ln2.p1, &ln2.p2);
    // UT3D_stru_dump (Typ_VC, &vc1, " vc1=");
    // UT3D_stru_dump (Typ_VC, &vc2, " vc2=");
    // GR_Disp_vc (&vc1, &ln1.p1, 1, 1);
    // GR_Disp_vc (&vc2, &ln2.p1, 1, 2);


  // haben nun 2 Verbindungslinien; ln1 u ln2
  // pt3 auf beide projizieren; ergibt:
  // Punkte px1/px2; das sind Punkt pt3 projiziert auf ln1/ln2.
  // Abstaende dp1 und dp2 von den Punkte px1/px2;
  UT3D_pt_projptptvc (&px1, &dp1, NULL, pt3, &ln1.p1, &vc1);
  UT3D_pt_projptptvc (&px2, &dp2, NULL, pt3, &ln2.p1, &vc2);
    // GR_Disp_pt (&px1, SYM_STAR_S, 1);
    // GR_Disp_pt (&px2, SYM_STAR_S, 2);



  if(iloop > 12) {
    printf("UT3D_parsru_pt E001\n");
    // exit(0); // return -1;

  } else {

    // uTot = dist der auf die beiden Linien proj. Punkte px1/px2
    // uTot = UT3D_len_2pt (&px1, &px2);
    d1 = UT3D_len_2pt (&px1, pt3);
    if(d1 < UT_TOL_pt) goto L_done;
    d2 = UT3D_len_2pt (&px2, pt3);
    uTot = d1 + d2;
    // printf("<<<<<<<<< dp1=%f dp2=%f uTot=%f\n",dp1,dp2,uTot);
    // wenn pt3 nicht genau auf der Flaeche liegt, loopt die Funktion !
    d1 = UT3D_len_2pt (&px1, &px2);
    if(d1 > UT_TOL_pt) goto L_next_x;
    // if(iloop < 6) goto L_next_x; // TEST
    // TX_Error("UT3D_parsru_pt E001");return -1; // TEST
  }


  //----------------------------------------------------------------
  // gefunden
  L_found:
  *parU = (u1 + u2) / 2.;
  irc = SRU_pt_evparcrv (&ln1.p1, *parU, 1);
  irc = SRU_pt_evparcrv (&ln1.p2, *parU, 2);
  goto L_comp_V;

  L_done:
  *parU = u1;


  L_comp_V:
  // errechnet *parV aus ln1 und pt3.
  // px1 = proj. pt3 -> ln1;
  UT3D_vc_2pt (&vc1, &ln1.p1, &ln1.p2);
  UT3D_pt_projptptvc (&px1, &dp1, NULL, pt3, &ln1.p1, &vc1);
  *parV = UT3D_len_2pt (&ln1.p1, &px1) / UT3D_len_2pt (&ln1.p1, &ln1.p2);



  // testdisplay PT
  // printf("ex UT3D_parsru_pt %f %f iloop=%d\n",*parU,*parV,iloop);

  return irc;

}



//=====================================================================
  int UT3D_pt_evparsru (Point *pto, double parU, double parV) {
//=====================================================================
// 2D-Paramter auf die 3D-RuledSurface umrechnen
// Init with TSU_tr_init_().

// Example:
// TSU_tr_init_ (ox1p);
// irc = UT3D_pt_evparsru (&pt1, .5, .5);


  Point  pt1;
  Line   ln1;


  // printf("UT3D_pt_evparsru %f %f\n",parU,parV);


  // pt2->x entlang curve1 abtragen -> pi1
  SRU_pt_evparcrv (&ln1.p1, parU, 1);


  // pt2->x entlang curve2 abtragen -> pi2
  SRU_pt_evparcrv (&ln1.p2, parU, 2);


  // pt2->y entlang pi1-pi2 abtragen -> pt3
  UT3D_pt_evparln (pto, parV, &ln1);


  return 0;

}


//===================================================================
  int SRU_pt_evparcrv (Point *pto, double lpar, int cvNr) {
//===================================================================
// einen Punkt auf linear obj ox1 errechnen;
// lpar = Abstandswert 0 - 1.
// cvNr = 1 (BottomCurve) oder 2 (TopCurve)
// siehe GR_DrawSur
// ACHTUNG: vorher UT3D_sru_init.



  int    typ, irc, trFlag;
  // ObjX   *oxp;
  void   *oxp;


  // printf("SRU_pt_evparcrv lpar=%f cvNr=%d\n",lpar,cvNr);
  // printf("   dir=%d,%d\n",SRU_dir1,SRU_dir2);



  irc = 0;



  // Achtung Typ_VC: ist nur als Obj 2 erlaubt; use Obj1 & translate Result.
  if((cvNr == 2)&&(SRU_typ2 == Typ_VC)) {
     cvNr   = 1;
     trFlag = 1;
  } else {
    trFlag = 0;
  }


  //----------------------------------------------------------------
  if(cvNr == 1) {
    typ = SRU_typ1;
    oxp = SRU_obj1;

    if(typ == Typ_CVBSP) {
      // printf(" p=%f k=%f d=%f\n",lpar,SRU_dat[0],SRU_dat[1]);
      // printf(" v0=%f v1=%f\n",oxp->bsp->v0,oxp->bsp->v1);
      // lpar = (lpar * SRU_dat[0]) + SRU_dat[1];
      if(SRU_dir1 != 0) {
        lpar = 1. - lpar;
        // lpar = (lpar * (oxp->bsp->v0 - oxp->bsp->v1)) + oxp->bsp->v1;
      } else {
        // lpar = (lpar * (oxp->bsp->v1 - oxp->bsp->v0)) + oxp->bsp->v0;
      }


    } else {
      if(SRU_dir1 != 0) lpar = 1 - lpar;  // umdrehen
    }




  //----------------------------------------------------------------
  } else {
    typ = SRU_typ2;
    oxp = SRU_obj2;

    if(typ == Typ_CVBSP) {
      // printf(" p=%f k=%f d=%f\n",lpar,SRU_dat[2],SRU_dat[3]);
      // printf(" v0=%f v1=%f\n",oxp->bsp->v0,oxp->bsp->v1);
      // lpar = (lpar * SRU_dat[2]) + SRU_dat[3];
      if(SRU_dir2 != 0) {
        lpar = 1. - lpar;
        // lpar = (lpar * (oxp->bsp->v0 - oxp->bsp->v1)) + oxp->bsp->v1;
      } else {
        // lpar = (lpar * (oxp->bsp->v1 - oxp->bsp->v0)) + oxp->bsp->v0;
      }


    } else {
      if(SRU_dir2 != 0) lpar = 1 - lpar;  // umdrehen
    }

  }

  // printf("     typ=%d lpar=%f trFlag=%d\n",typ,lpar,trFlag);



  // irc = UT3D_pt_evparcrv (pto, lpar, typ, oxp);
  // ACHTUNG: *oxp entspricht *void; (oxp == **void)
  irc = UT3D_pt_evparcrv (pto, lpar, typ, oxp);
  // UT3D_stru_dump (Typ_PT, pto, " evpar %d",irc);
  if(irc < 0) return irc;

  // translate
  if(trFlag == 1) {
    // UT3D_stru_dump (Typ_VC, SRU_obj2->vc, "  transl:");
    UT3D_pt_traptvc (pto, pto, (Vector*)SRU_obj2);
  }


  // UT3D_stru_dump (Typ_PT, pto, "ex SRU_pt_evparcrv %d",irc);

  return irc;
}



//=======================================================================
  int SRU_parsru_intObj (double *pU, double *pV, Point *ptx,
                        int typ,  void *objIn) {
//=======================================================================
// intersect line - ruledSurface
// INIT with TSU_tr_init_ !!
// OFFEN: solutionNr (MOD)
/// Input:
///   typ, objIn     ruledSurface
/// Output:
///   pU             parameter along curve (X)
///   pV             parameter along line (Y)
///   ptx            ?


//                  .p2        .p2
//         -----------------|-----------------
//         |          |     |    |           |
//         |          |     |    |           |
//         |          |     |    |           |
//        ln0       ln1    ln50 ln2         ln100
//         |          |     |    |           |
//         |       px1|     |    |px2        |
//         |          x  X  |    x           |
//         |          | pt3 |    |           |
//         -----------------|-----------------
//         pa       .p1         .p1          pe
                   

// ln1-ln2 sind 2 Linien mit den Parametern u1-u2
// px1 = der naeheste Punkt auf ln1
// px2 = der naeheste Punkt auf ln2

// see UT3D_parsru_pt



  int    irc, iloop, i1, i2;
  double d1, d2, dp1, dp2, u1, u2, uMid, uVar, dTot, uTot, uRange;
  double dTab[5], tolq;
  Point  px1, px2, pl1, pl2, psa[5], pia[5];
  Line   ln1, ln2;
  Vector vc1, vc2, vc3, vc4, vv;

  // double (*par_cmp1)();
  double (*par_cmp2)();



  // printf("SRU_parsru_intObj =========================\n");
  // UT3D_stru_dump (typ, objIn, "SRU_parsru_intObj - in");
  // if(typ == Typ_PT) GR_Disp_pt (objIn, SYM_STAR_S, 2);


  iloop  = 0;  // Loopcounter; abbrechen nach 15 Versuchen.
  tolq = UT_TOL_cv * UT_TOL_cv;



  // Startwerte wahlen (u1,u2,uVar.)
  // Find Point on surf: es gibt mehrere Loesungen;
  //   jene mit dem geringsten Abstand waehlen.
  // Find Line/Surf-Intersection: es gibt mehrere Loesungen;
  //   MOD(solutionNr) not yet supported;

  if(typ == Typ_PT) {
    par_cmp2 = SRU_parsru_utPt2;
    irc = SRU_parsru_utPtIni (&u1, &u2, &uVar, objIn);
    if(irc) {    // 2014-11-05
      // already found;
      irc = SRU_pt_evparcrv (&ln1.p1, u1, 1);
      irc = SRU_pt_evparcrv (&ln1.p2, u1, 2);
        // UT3D_stru_dump (Typ_LN, &ln1, "  utPtIni found ln1");
      *pU = u1;
      *ptx = *((Point*)objIn);
      goto L_comp_V;
    }

  } else if(typ == Typ_LN) {
    par_cmp2 = SRU_parsru_utLn2;
    SRU_parsru_utLnIni (&u1, &u2, &uVar, objIn);

  }



  //---------------------------------------------------------
  // printf("============================================= \n");
  L_t1:
  ++iloop;

  // ab hier notwendig: u1,u2,uVar
  // uVar: parameter of solution-point between u1-u2.

  uRange = (u2 - u1) / 2.;
  irc = 0;



  // u1 und u2 fuer nechsten Test feststellen.
  // closed ist ein Problem; hier sind pa/pe und px1/px2 ident.
  // Man muss (besonders wichtig in der Naehe von par=0=1) feststellen,
  //   auf welcher Haelfte der Kurve.


  // wenn ca in der Mitte den bereich verkleinern.
  // if(uRange > 0.0000001) {
  if(uRange > UT_TOL_min2) {
    // if     ((uVar > 0.48)&&(uVar < 0.52)) uRange /= 6.;   // 12
    // else if((uVar > 0.45)&&(uVar < 0.55)) uRange /= 5.;   // 8
    // else if((uVar > 0.4)&&(uVar < 0.6))   uRange /= 4.;   // 6
    // else if((uVar > 0.3)&&(uVar < 0.7))   uRange /= 3.;   // 4
    // else if((uVar > 0.1)&&(uVar < 0.9))   uRange /= 2.;   // 2

    if     ((uVar > 0.48)&&(uVar < 0.52)) uRange /= 7.;  // 12
    else if((uVar > 0.45)&&(uVar < 0.55)) uRange /= 6.;   // 8
    else if((uVar > 0.4)&&(uVar < 0.6))   uRange /= 5.;   // 6
    else if((uVar > 0.3)&&(uVar < 0.7))   uRange /= 4.;   // 4
    else if((uVar > 0.1)&&(uVar < 0.9))   uRange /= 3.;   // 2

  }

       if(uVar >  3.0) uVar =  3.0;
  else if(uVar < -3.0) uVar = -3.0;



  uMid = u1 + ((u2-u1)*uVar);
  // if     (uMid > 1.0) uMid = 1.0;
  // else if(uMid < 0.0) uMid = 0.0;


  // Ermitteln von 2 Testparameterwerten auf den Curves (u1 und u2).
  // u1, u2 aus uMid und uRange errechnen.
  L_mid:
  u1 = uMid - uRange;
  u2 = uMid + uRange;

  if(u1 < 0.) {          // u2 verkleinern !
    u1=0.;
  }


  if(u2 > 1.) {          // u1 vergroessern
    u2 = 1.;
  }

    // printf(" u1=%f u2=%f  uMid=%f uR=%f\n",u1,u2,uMid,uRange);

  // u1, u2 sind nun 2 Testparameter auf den Curves.
  // Die zugehoereigen Punkte auf Bottom- und TopCurve errechnen,
  // daraus die Linien ln1/ln2 generieren.
  SRU_pt_evparcrv (&ln1.p1, u1, 1);
  SRU_pt_evparcrv (&ln1.p2, u1, 2);
  SRU_pt_evparcrv (&ln2.p1, u2, 1);
  SRU_pt_evparcrv (&ln2.p2, u2, 2);

  // compute intersections on ln1, ln2
  uVar = par_cmp2 (&px1, &px2, ptx, &ln1, &ln2, objIn);


  if(iloop > 12) {
    printf("SRU_parsru_intObj E001\n");
    // UT3D_stru_dump (Typ_PT, &px1, "  px1");
    // UT3D_stru_dump (Typ_PT, &px2, "  px2");
    // UT3D_pt_mid2pt (ptx, &px1, &px2);

  } else {

    // Abstand ptx-px1  und ptx-px2 testen
    d1 = UT3D_lenq_PtPt (&px1, ptx);
    if(d1 < tolq) goto L_done;
    d1 = UT3D_lenq_PtPt (&px2, ptx);
    if(d1 < tolq) {u1 = u2; ln1 = ln2; goto L_done;}
    goto L_t1;
  }



  //----------------------------------------------------------------
  // kein Ergebnis ..
  L_found:
  *pU = u1 + ((u2-u1)*uVar);
  irc = SRU_pt_evparcrv (&ln1.p1, *pU, 1);
  irc = SRU_pt_evparcrv (&ln1.p2, *pU, 2);
  goto L_comp_V;


  L_done:
  *pU = u1;


  L_comp_V:
  // errechnet *parV aus ln1 und ptx.
  UT3D_parpt_3pt (pV, ptx, &ln1.p1,  &ln1.p2);


    // testdisplay PT
    // printf("ex SRU_parsru_intObj %d %f %f iloop=%d\n",irc,*pU,*pV,iloop);
    // UT3D_stru_dump (Typ_PT, ptx, "  ptx");

  return irc;

}


//================================================================
  int SRU_init_basPts () {
//================================================================
// load basicPoints (ln0, ln25,ln50, ln75 ..)


  // printf("SRU_init_basPts %d %d\n",SRU_typ1,SRU_typ2);


  // not for CCV's   (CCV: only for SRU)
  if(SRU_typ1 == Typ_ObjGX) return 0;

    SRU_pt_evparcrv (&SRU_ln0.p1, 0.0, 1);
    SRU_pt_evparcrv (&SRU_ln0.p2, 0.0, 2);
    SRU_pt_evparcrv (&SRU_ln1.p1, 0.01, 1);
    SRU_pt_evparcrv (&SRU_ln1.p2, 0.01, 2);
    SRU_pt_evparcrv (&SRU_ln25.p1, 0.25, 1);
    SRU_pt_evparcrv (&SRU_ln25.p2, 0.25, 2);
    SRU_pt_evparcrv (&SRU_ln50.p1, 0.5, 1);
    SRU_pt_evparcrv (&SRU_ln50.p2, 0.5, 2);
    SRU_pt_evparcrv (&SRU_ln75.p1, 0.75, 1);
    SRU_pt_evparcrv (&SRU_ln75.p2, 0.75, 2);
    SRU_pt_evparcrv (&SRU_ln99.p1, 0.99, 1);
    SRU_pt_evparcrv (&SRU_ln99.p2, 0.99, 2);
    SRU_pt_evparcrv (&SRU_ln100.p1, 1.0, 1);
    SRU_pt_evparcrv (&SRU_ln100.p2, 1.0, 2);
/*
    if(SRU_clos == 0) {         // normal - not closed
      SRU_pt_evparcrv (&SRU_ln100.p1, 1.0, 1);
      SRU_pt_evparcrv (&SRU_ln100.p2, 1.0, 2);

    } else {   // closed; SRU_ln0 und SRU_ln100 ident;
      // durch die Startlinie ln0 und den Startpunkt der ln50 eine Plane -
      // vcn = Z-Vektor dieser Plane.
      // UT3D_vc_perp3pt (&vcn, &ln0.p1, &ln50.p1, &ln0.p2);
      // UT3D_vc_perp3pt (&vcn, &ln0.p1, &ln0.p2, &ln50.p1);
      SRU_pt_evparcrv (&ln10.p1, 0.1, 1);
      SRU_pt_evparcrv (&ln10.p2, 0.1, 2);
      SRU_pt_evparcrv (&ln90.p1, 0.9, 1);
      SRU_pt_evparcrv (&ln90.p2, 0.9, 2);
    }
*/

  // UT3D_stru_dump (Typ_LN, &ln0, "ln0:\n");
  // UT3D_stru_dump (Typ_LN, &ln25, "ln25:\n");
  // UT3D_stru_dump (Typ_LN, &ln50, "ln50:\n");
  // UT3D_stru_dump (Typ_LN, &ln75, "ln75:\n");
  // UT3D_stru_dump (Typ_LN, &ln100, "ln100:\n");

  // GR_Disp_ln (&SRU_ln0, 9);
  // GR_Disp_ln (&SRU_ln1, 9);
  // GR_Disp_ln (&SRU_ln25, 9);
  // GR_Disp_ln (&SRU_ln50, 9);
  // GR_Disp_ln (&SRU_ln75, 9);
  // GR_Disp_ln (&SRU_ln99, 9);
  // GR_Disp_ln (&SRU_ln100, 8);

  return 0;

}


//================================================================
  int SRU_parsru_utPtIni (double *u1, double *u2, double *up,
                          void *objIn) {
//================================================================
// get startValues
/// Input:
///   objIn   point
/// Output:
///   u1, u2, up,
/// Retcod:
///   0       u1, u2, up set;
///   1       point objIn lies on line with parameter u1.


  double  par, dist, distold, dl1;
  Point   px1, px2, pl1, pl2;
  Vector  vc1;


  // UT3D_stru_dump(Typ_PT, objIn, "SRU_parsru_utPtIni");


  distold = UT_VAL_MAX;


  //----------------------------------------------------------------
  UT3D_pt_projpt2pt (&px2, &dl1, objIn, &SRU_ln0.p1, &SRU_ln0.p2);     // ln0
  if(dl1 < UT_TOL_cv) { *u1 = 0.; return 1; }



  //----------------------------------------------------------------
  // get nearestPoint on surfaceLine and on intersectionLine
  px1 = px2;
  UT3D_pt_projpt2pt (&px2, &dl1, objIn, &SRU_ln1.p1, &SRU_ln1.p2);

  // distance & parameter von point objIn auf Line px1-px2 errechnen
  SRU_parsru_utPtParDist (&par, &dist, &px1, &px2, objIn);
    // printf(" par1=%f dist=%f\n",par,dist);

  // auswaehlen, wenn objIn zwischen px1-px2 und Abstand kleiner distOld
  SRU_parsru_utPtSel (u1, u2, up, &par, &dist, &distold, 0., 0.01);


  //----------------------------------------------------------------
  // get nearestPoint on surfaceLine and on intersectionLine
  px1 = px2;
  UT3D_pt_projpt2pt (&px2, &dl1, objIn, &SRU_ln25.p1, &SRU_ln25.p2);
  if(dl1 < UT_TOL_cv) { *u1 = 0.25; return 1; }

  // distance & parameter von point objIn auf Line px1-px2 errechnen
  SRU_parsru_utPtParDist (&par, &dist, &px1, &px2, objIn);
    // printf(" par1=%f dist=%f\n",par,dist);

  // auswaehlen, wenn objIn zwischen px1-px2 und Abstand kleiner distOld
  SRU_parsru_utPtSel (u1, u2, up, &par, &dist, &distold, 0.01, 0.25);


  //----------------------------------------------------------------
  px1 = px2;
  UT3D_pt_projpt2pt (&px2, &dl1, objIn, &SRU_ln50.p1, &SRU_ln50.p2);
  if(dl1 < UT_TOL_cv) { *u1 = 0.50; return 1; }

  // distance & parameter von point objIn auf Line px1-px2 errechnen
  SRU_parsru_utPtParDist (&par, &dist, &px1, &px2, objIn);
    // printf(" par1=%f dist=%f\n",par,dist);

  // auswaehlen, wenn objIn zwischen px1-px2 und Abstand kleiner distOld
  SRU_parsru_utPtSel (u1, u2, up, &par, &dist, &distold, 0.25, 0.50);


  //----------------------------------------------------------------
  px1 = px2;
  UT3D_pt_projpt2pt (&px2, &dl1, objIn, &SRU_ln75.p1, &SRU_ln75.p2);
  if(dl1 < UT_TOL_cv) { *u1 = 0.75; return 1; }

  // distance & parameter von point objIn auf Line px1-px2 errechnen
  SRU_parsru_utPtParDist (&par, &dist, &px1, &px2, objIn);
    // printf(" par1=%f dist=%f\n",par,dist);

  // auswaehlen, wenn objIn zwischen px1-px2 und Abstand kleiner distOld
  SRU_parsru_utPtSel (u1, u2, up, &par, &dist, &distold, 0.50, 0.75);


  //----------------------------------------------------------------
  px1 = px2;
  UT3D_pt_projpt2pt (&px2, &dl1, objIn, &SRU_ln99.p1, &SRU_ln99.p2);

  // distance & parameter von point objIn auf Line px1-px2 errechnen
  SRU_parsru_utPtParDist (&par, &dist, &px1, &px2, objIn);
    // printf(" par1=%f dist=%f\n",par,dist);

  // auswaehlen, wenn objIn zwischen px1-px2 und Abstand kleiner distOld
  SRU_parsru_utPtSel (u1, u2, up, &par, &dist, &distold, 0.75, 0.99);


  //----------------------------------------------------------------
  px1 = px2;
  UT3D_pt_projpt2pt (&px2, &dl1, objIn, &SRU_ln100.p1, &SRU_ln100.p2);
  if(dl1 < UT_TOL_cv) { *u1 = 1.; return 1; }

  // distance & parameter von point objIn auf Line px1-px2 errechnen
  SRU_parsru_utPtParDist (&par, &dist, &px1, &px2, objIn);
    // printf(" par1=%f dist=%f\n",par,dist);

  // auswaehlen, wenn objIn zwischen px1-px2 und Abstand kleiner distOld
  SRU_parsru_utPtSel (u1, u2, up, &par, &dist, &distold, 0.99, 1.);


  //----------------------------------------------------------------
  // printf("ex SRU_parsru_utPtIni %f %f %f\n",*u1,*u2,*up);


  return 0;

}


//================================================================
  int SRU_parsru_utLnIni (double *u1, double *u2, double *up,
                          void *objIn) {
//================================================================
// get startValues

  double  par, dist, distold, dl1;
  Point   px1, px2, pl1, pl2, plm;
  Vector  vc1;


  distold = UT_VAL_MAX;

  UT3D_pt_int2ln (&px2, &pl2, NULL, &SRU_ln0, objIn);


  //----------------------------------------------------------------
  // get nearestPoint on surfaceLine and on intersectionLine
  px1 = px2;
  pl1 = pl2;
  UT3D_pt_int2ln (&px2, &pl2, NULL, &SRU_ln1, objIn);

  // distance & parameter von point objIn auf Line px1-px2 errechnen
  UT3D_pt_mid2pt (&plm, &pl1, &pl2);
  SRU_parsru_utPtParDist (&par, &dist, &px1, &px2, &plm);
    // printf(" par1=%f dist=%f\n",par,dist);

  // auswaehlen, wenn objIn zwischen px1-px2 und Abstand kleiner distOld
  SRU_parsru_utPtSel (u1, u2, up, &par, &dist, &distold, 0., 0.01);


  //----------------------------------------------------------------
  // get nearestPoint on surfaceLine and on intersectionLine
  px1 = px2;
  pl1 = pl2;
  UT3D_pt_int2ln (&px2, &pl2, NULL, &SRU_ln25, objIn);

  // distance & parameter von point objIn auf Line px1-px2 errechnen
  UT3D_pt_mid2pt (&plm, &pl1, &pl2);
  SRU_parsru_utPtParDist (&par, &dist, &px1, &px2, &plm);
    // printf(" par1=%f dist=%f\n",par,dist);

  // auswaehlen, wenn objIn zwischen px1-px2 und Abstand kleiner distOld
  SRU_parsru_utPtSel (u1, u2, up, &par, &dist, &distold, 0.01, 0.25);


  //----------------------------------------------------------------
  px1 = px2;
  pl1 = pl2;
  UT3D_pt_int2ln (&px2, &pl2, NULL, &SRU_ln50, objIn);

  // distance & parameter von point objIn auf Line px1-px2 errechnen
  UT3D_pt_mid2pt (&plm, &pl1, &pl2);
  SRU_parsru_utPtParDist (&par, &dist, &px1, &px2, &plm);
    // printf(" par1=%f dist=%f\n",par,dist);

  // auswaehlen, wenn objIn zwischen px1-px2 und Abstand kleiner distOld
  SRU_parsru_utPtSel (u1, u2, up, &par, &dist, &distold, 0.25, 0.50);



  //----------------------------------------------------------------
  px1 = px2;
  pl1 = pl2;
  UT3D_pt_int2ln (&px2, &pl2, NULL, &SRU_ln75, objIn);

  // distance & parameter von point objIn auf Line px1-px2 errechnen
  UT3D_pt_mid2pt (&plm, &pl1, &pl2);
  SRU_parsru_utPtParDist (&par, &dist, &px1, &px2, &plm);
    // printf(" par1=%f dist=%f\n",par,dist);

  // auswaehlen, wenn objIn zwischen px1-px2 und Abstand kleiner distOld
  SRU_parsru_utPtSel (u1, u2, up, &par, &dist, &distold, 0.50, 0.75);


  //----------------------------------------------------------------
  px1 = px2;
  pl1 = pl2;
  UT3D_pt_int2ln (&px2, &pl2, NULL, &SRU_ln99, objIn);

  // distance & parameter von point objIn auf Line px1-px2 errechnen
  UT3D_pt_mid2pt (&plm, &pl1, &pl2);
  SRU_parsru_utPtParDist (&par, &dist, &px1, &px2, &plm);
    // printf(" par1=%f dist=%f\n",par,dist);

  // auswaehlen, wenn objIn zwischen px1-px2 und Abstand kleiner distOld
  SRU_parsru_utPtSel (u1, u2, up, &par, &dist, &distold, 0.75, 0.99);


  //----------------------------------------------------------------
  px1 = px2;
  pl1 = pl2;
  UT3D_pt_int2ln (&px2, &pl2, NULL, &SRU_ln100, objIn);

  // distance & parameter von point objIn auf Line px1-px2 errechnen
  UT3D_pt_mid2pt (&plm, &pl1, &pl2);
  SRU_parsru_utPtParDist (&par, &dist, &px1, &px2, &plm);
    // printf(" par1=%f dist=%f\n",par,dist);

  // auswaehlen, wenn objIn zwischen px1-px2 und Abstand kleiner distOld
  SRU_parsru_utPtSel (u1, u2, up, &par, &dist, &distold, 0.99, 1.);


  //----------------------------------------------------------------
  // printf("ex SRU_parsru_utLnIni %f %f %f\n",*u1,*u2,*up);


  return 0;

}

//================================================================
  int SRU_parsru_utPtParDist (double *par, double *dist,
                              Point *px1, Point *px2, Point *pt3) {
//================================================================

  Vector vc1;


  // den Parameter fuer point pt3 auf Strecke px1-px2 errechnen
  UT3D_vc_2pt (&vc1, px1, px2);
  UT3D_parpt_ptvc (par, pt3, px1, &vc1);

  // p1 = Abstand von point objIn vom vektor px1-px2
  UT3D_lenq_PtLn (dist, pt3, px1, px2);
    // printf(" par=%f dist=%f\n",*par,*dist);

  return 0;

}

//================================================================
  int SRU_parsru_utPtSel (double *u1, double *u2, double *up,
                          double *par, double *dist, double *distold,
                          double u1Act, double u2Act) {
//================================================================

  if((*par >= 0.)&&(*par <= 1.1)) {
    if(*dist < *distold) {
      *u1 = u1Act;
      *u2 = u2Act;
      *up = *par;
      *distold = *dist;
    }
  }

    // printf("ex SRU_parsru_utPtSel %f %f %f\n",*u1,*u2,*up);

  return 0;

}
 
 
//======================================================================
  double SRU_parsru_utPt2 (Point *px1, Point *px2, Point *ptx,
                           Line *ln1, Line *ln2, Point *pt3) {
//======================================================================
// get nearest positon of ln1-ln2 to pt3

  double   dl1, dl2, par1;
  Vector   vc1;
  Line     lnx;


  UT3D_pt_projpt2pt (px1, &dl1, pt3, &ln1->p1, &ln1->p2);   // ln1
  UT3D_pt_projpt2pt (px2, &dl2, pt3, &ln2->p1, &ln2->p2);   // ln1
    // GR_Disp_pt (px1, SYM_STAR_S, 2);
    // GR_Disp_pt (px2, SYM_STAR_S, 2);

  // den Parameter fuer ptx auf Strecke px1-px2 errechnen
  UT3D_vc_2pt (&vc1, px1, px2);
  UT3D_parpt_ptvc (&par1, pt3, px1, &vc1);
    // printf(" par1=%f\n",par1);

  // den punkt ptx via px1,px2, und par1 errechnen
  UT3D_pt_evparptcv (ptx, par1, px1, &vc1);

  return par1;

}


//======================================================================
  double SRU_parsru_utLn2 (Point *px1, Point *px2, Point *ptx,
                           Line *ln1, Line *ln2, Line *lni) {
//======================================================================
// get nearest positon of ln1-ln2 to pt3
// OFFEN: test ob mehrere (2) Intersectionpoints

  double   dl1, dl2, par1;
  Point    pl1, pl2;
  Line     lnx;


  UT3D_pt_int2ln (&pl1, px1, NULL, lni, ln1);
  UT3D_pt_int2ln (&pl2, px2, NULL, lni, ln2);

    // GR_Disp_pt (px1, SYM_STAR_S, 2);
    // GR_Disp_pt (px2, SYM_STAR_S, 2);
    // GR_Disp_pt (&pl1, SYM_STAR_S, 3);
    // GR_Disp_pt (&pl2, SYM_STAR_S, 3);

// HIER NICHT DEN MITTELPUNKT, sondern den naeheren Punkt waehlen ?????
  // Mittelpunkt auf der intersection-line
  UT3D_pt_mid2pt (ptx, &pl1, &pl2);
    // GR_Disp_pt (ptx, SYM_STAR_S, 3);

  // den Parameter fuer plm auf Strecke px1-px2 errechnen
  UT3D_parpt_3pt (&par1, ptx, px1, px2);
    // printf(" par1=%f\n",par1);


  return par1;

}


//========== EOF =================================================
