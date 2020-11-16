//      ut_hatch.c                                2003-08-21    RF
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
  ..

-----------------------------------------------------
*/
/*!
\file  ../ut/ut_hatch.c
\brief hatch 2D
\code
=====================================================
List_functions_start:

UT_hatch_2D      hatch polygon
UT_hatch_ccv     hatch CCV

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



#include "../ut/ut_geo.h"              // DL_Att
#include "../ut/func_types.h"               // SYM_TRI_S
#include "../ut/ut_memTab.h"           // MemTab

#include "../xa/xa_mem.h"             // memspc55



// Arraysize fuer Zusatzarrays zu den Lines fuer die gesamte Flaeche
#define TCLA_SIZ 1000






//=========================================================
  int UT_hatch_2D (int *lnNr, Line2  *ln2a,
               short unsigned *ln2ia, short unsigned *ln2ie,
               int ptNr, Point2 *pt2a, double soff) {
//=========================================================
/// \code
/// schraffieren des Polygons pt2a[ptNr].
/// Ausgabe der 2D-Linien -> ln2a[lnNr].
/// IN:   lnNr: max Anzahl von Lines.
///       soff: Abstand der Schraffurlinien.
/// \endcode


#define HATCH_SIZ_SPA  32    // Anzahl Schnittpunkte pro Horizontale.

  int     irc, i1, i2, i3, isp, lnMax, lni,
          spi[HATCH_SIZ_SPA],
          spi1[HATCH_SIZ_SPA],
          spi2[HATCH_SIZ_SPA];
  long    idl;
  double  Ymin, Ymax, Yact, d1;
  Point2  spa[HATCH_SIZ_SPA];

  // nur fuer tests:
  Point   pt1;
  Line    ln1;



  // printf("UT_hatch_2D %d %f\n",ptNr,soff);

  lnMax = *lnNr;
  lni = 0;


  // Ymin suchen
  Ymin = UT_VAL_MAX;
  for(i1=0; i1<ptNr; ++i1) {
    if(pt2a[i1].y < Ymin) Ymin = pt2a[i1].y;
  }


  // Ymax suchen
  Ymax = UT_VAL_MIN;
  for(i1=0; i1<ptNr; ++i1) {
    if(pt2a[i1].y > Ymax) Ymax = pt2a[i1].y;
  }


  Ymax += soff * 0.5;

  // printf("  Ymin=%f Ymax=%f\n",Ymin,Ymax);


  lni = 0;
  Yact = Ymin + (soff * 0.5);
  // Yact = Ymin - (10*UT_TOL_pt);
  Yact = Ymin + soff - (10*UT_TOL_pt);


  //-----------------------------------------------------
  // loop von Ymin bis Ymax step soff
  L_next_Y:

  // printf("L_next_Y - Yact=%f\n",Yact);


  // loop durch alle Polygonsegmente ..
  isp = 0;


  // Alle Schnittpunkte mit Y=Yact -> spa
  for(i1=1; i1<ptNr; ++i1) {
    // printf(" int [%d] %f %f\n",i1,pt2a[i1].x,pt2a[i1].y);

    // verschneiden
    irc = UT2D_pt_intlny(&spa[isp], &pt2a[i1-1], &pt2a[i1], Yact, UT_TOL_min1);
    if(irc < 0) continue;


    // nun doppelte Punkte eliminieren; Fehler durch folgendes Muster:
    //     _
    //   |/ |  |
    //   X  |  X   hier beim X sind jeweils 2 punkte, in der Mitte nur einer.
    //      |_/
    //
    // wenn 2 Punkte hintereinander ident:
    // wenn beide ausseren Endpunkte auf der gleichen Seite: beide Punkte weg
    //     \ /
    // -----X-----
    //
    // wenn die ausseren Endpunkte auf verschiedenen Seiten: skip this point.
    //       /
    // -----X----
    //     /

    if(i1 < 2) goto L_accept;
    if(isp < 1) goto L_accept;
    if(!UT2D_comp2pt(&spa[isp], &spa[isp-1], UT_TOL_pt)) goto L_accept;

    // i1 i2 die Indexe fuer die Aussenpunkte
    i2 = i1 - 2;

    // wenn beide ausseren Endpunkte auf der gleichen Seite: beide Punkte weg
    if(((pt2a[i1].y > Yact)&&(pt2a[i1].y > Yact)) ||
       ((pt2a[i1].y < Yact)&&(pt2a[i1].y < Yact))) {
      --isp;
      continue;
    }

    // die ausseren Endpunkte auf verschiedenen Seiten: skip this point.
    continue;



    L_accept:
    spi[isp] = i1;  // Segmentindex merken

    // Testdisplay intPoint
    // idl=-1;pt1=UT3D_pt_pt2(&spa[isp]);GR_CrePoint(&idl,Typ_Att_def,&pt1);
    if(isp < HATCH_SIZ_SPA) ++isp;
    else TX_Error("UT_hatch_2D E001");

  }


  // nun noch ersten und letzten Punkt testen
  if(!UT2D_comp2pt(&spa[0], &pt2a[0], UT_TOL_pt)) goto L_test3;
  if(UT2D_comp2pt(&spa[isp-1], &pt2a[0], UT_TOL_pt)) goto L_test3;

    // i1 i2 die Indexe fuer die Aussenpunkte
    i1 = 1;
    i2 = ptNr - 2;

    // wenn beide ausseren Endpunkte auf der gleichen Seite: beide Punkte weg
    if(((pt2a[i1].y > Yact)&&(pt2a[i1].y > Yact)) ||
       ((pt2a[i1].y < Yact)&&(pt2a[i1].y < Yact))) {
      isp -= 2;
      goto L_test3;
    }

    // die ausseren Endpunkte auf verschiedenen Seiten: skip this point.
    --isp;










  // nun sind isp Punkte auf der hor. Line gefunden;
  // printf(" Yact=%f found %d\n",Yact,isp);
  L_test3:
  if(isp < 2) goto L_nxt_off;



  // wenn Punkte genau im Schnittpunkt liegen, sind sie nun doppelt.


/*
  // nur Testausg
  // if(isp == 3) {
  if(UTP_comp2db(spa[0].y, -200.3, 1.)) {
    for(i1=0; i1<isp; ++i1) {
      printf(" [%d] %f %f\n",i1,spa[i1].x,spa[i1].y);
    }
  }
*/


  // sortieren
  for(i1=0; i1<isp; ++i1) { spi1[i1] = -1; } // init spi1

  for(i1=0; i1<isp; ++i1) {
    // printf("sort %d\n",i1);
    L_skip_this_point:
    d1 = UT_VAL_MAX;
    for(i2=0; i2<isp; ++i2) {
      if(spi1[i2] >= 0) continue;
      if(spa[i2].x < d1) { d1 = spa[i2].x; i3 = i2; }
    }
    // spa[i3] ist nun der am weitesten links liegende Punkt
    spi1[i3] = i1; // wenn Inhalt=0: erster punkt; wenn Inhalt=1: 2.punkt ...
    spi2[i1] = i3; // spi2[0] = ind.1.Punkt, spi2[1] = ind.2.Punkt ..
    // printf(" sort %d found P[%d] %f ip=%d\n",i1,i3,d1,spi[i3]);


    // identische Punkte einfach skippen
    for(i2=0; i2<isp; ++i2) {
      if(UTP_comp2db(d1, spa[i2].x, UT_TOL_pt)) {
        if(spi1[i2] >= 0) continue;
        // printf(" skip p[%d]=%f %f\n",i2,spa[i2].x,spa[i2].y);
        spi1[i2] = 99999;
        --isp;
        // goto L_skip_this_point;
      }
    }

  }



  // wird reduziert!
  if(isp < 2) goto L_nxt_off;




  // die outLines generieren
  for(i1=0; i1<isp-1; i1+=2) {
    if(lni > lnMax) { TX_Error("UT_hatch_2D E002"); return -1; }
    ln2a[lni].p1 = spa[spi2[i1]];
    ln2a[lni].p2 = spa[spi2[i1+1]];
    ln2ia[lni]   = spi[spi2[i1]];
    ln2ie[lni]   = spi[spi2[i1+1]];
    ++lni;
  }


  L_nxt_off:
  Yact += soff;
  if(Yact < Ymax) goto L_next_Y;

  *lnNr = lni;


/*
  printf("ex UT_hatch_2D %d\n",*lnNr);

  // TEST
  for(i1=0; i1<lni; ++i1) {
    DEB_dump_obj__ (Typ_LN2,&ln2a[i1],"L[%d] %d %d\n",i1,ln2ia[i1],ln2ie[i1]);
    // Testdisplay Line
    UT3D_ln_2pt2(&ln1,&ln2a[i1].p1,&ln2a[i1].p2);
    idl=0; GR_CreLine(&idl,1,&ln1);
  }
*/

  return 0;

}


//=========================================================
  int UT_hatch__ (int *lnNr, Line *lTab,
                  int dbTyp, long dbInd,
                  double hoff, double hdir) {
//=========================================================
/// hatch CCV

  int     i1, ptNr;
  Point   *pta;


  // printf("UT_hatch__ %d %ld %f %f\n",dbTyp,dbInd,hoff,hdir);

  // prepare space for 3D-points
  pta = (Point*)memspc54;
  ptNr = sizeof(memspc54) / sizeof(Point);

/*
  ox1  = DB_GetObjGX (Typ_CVTRM, suh->cvID);

  // CCV -> 3D-Polygon umwandeln
  i1 = UT3D_pta_ox_lim (&ptNr, pta, ox1->data, 0, NULL, UT_DISP_cv);
    printf(" _pta_ccv %d %d\n",i1,ptNr);
  if(i1 < 0) return i1;
*/

  i1 = UT3D_pta_dbo (&pta, &ptNr, dbTyp, dbInd, UT_DISP_cv);
  if(i1 < 0) return i1;


  return UT_hatch_pta (lnNr, lTab, ptNr, pta, hoff, hdir);

}


//=========================================================
  int UT_hatch_pta (int *lnNr, Line *lTab,
                    int ptNr,  Point *pta,
                    double hoff, double hdir) {
//=========================================================
/// hatch pta

  int     irc, i1, ln2Nr, lnMax;
  Point2  pt2a[TCLA_SIZ], pCen;
  Line    ln1;
  Line2   *ln2a;
  Plane   pl1;
  Mat_4x3 trmat, imat;
  short unsigned  ln2ia[TCLA_SIZ];
// n. f. Tests:
  long    dli;
  Point   pt1;



  // printf("UT_hatch_pta %d %f %f\n",ptNr,hoff,hdir);


  lnMax = *lnNr;

/*
  // nur TEST: Anzeige 3D-Polygon
  for(i1=0; i1<ptNr; ++i1) {
    DEB_dump_obj__ (Typ_PT, &pta[i1], "P[%d]=",i1);
    // APT_disp_SymB (SYM_TRI_S, 2, &pta[i1]);
  }
*/



  //----------------------------------------------------
  // ab hier gleich in SUS_tess_pln

  // die Plane aus dem Punktearray errechnen
  UT3D_pl_pta (&pl1, ptNr, pta);
    // DEB_dump_obj__ (Typ_PLN, &pl1, "HatchPlane:  ");


  // Plane -> Tramat
  UT3D_m3_loadpl (trmat, &pl1);


  // Matrix invertieren:
  UT3D_m3_invm3 (imat, trmat);



  // 3D-Polygon -> 2D-Polygon
  // printf(" vor ck_srot:\n");
  for(i1=0; i1<ptNr; ++i1) {

    UT3D_pt_tra_pt_m3 (&pt1, imat, &pta[i1]);
    pt2a[i1] = UT2D_pt_pt3 (&pt1);

    // TESTAUSG:
    // DEB_dump_obj__ (Typ_PT2, &pt2a[i1], "P2[%d]=",i1);
    // dli=0;pt1=UT3D_pt_pt2(&pt2a[i1]);APT_disp_SymB (SYM_TRI_S, 2, &pt1);
  }

  // NUR TESTAUSG:
  // dli=DL_StoreObj (Typ_CV, -1L, 2);GR_tDyn_pcv2D (&dli, 2, ptNr, pt2a, 0.);





/*
  // test Umdrehungssinn von pt2a; wenn nicht CCW umdrehen
  // scheint dzt unnutz; war SUStess_ck_srot_ in ut_tess.c
  // use UT2D_sr_npt
  irc = SUStess_ck_srot_ (ptNr, pt2a);

  // Umdrehen: 2D- und 3D-Punktetabelle umdrehen
  if(irc == CW) {
    UT2D_cv_inv (ptNr, pt2a);
    UT3D_cv_inv (ptNr, pta);
  }
*/

  // TEST NACH umdrehen:
  // printf(" nach ck_srot:\n");
  // for(i1=0; i1<ptNr; ++i1) DEB_dump_obj__ (Typ_PT2,&pt2a[i1],"P2[%d]=",i1);
  // dli=DL_StoreObj (Typ_CVPOL, -1L, 2);

  // bis hier gleich in SUS_tess_pln
  //----------------------------------------------------





  // das 2D-Polygon nun um hdir drehen
  hdir = UT_RADIANS(hdir);
  pCen = pt2a[0];
  for(i1=1; i1<ptNr-1; ++i1) { // skip erster = letzter Punkt
    UT2D_pt_rotptangr (&pt2a[i1], &pCen, &pt2a[i1], -hdir);
  }



  // prepare space for 2D-Lines
  ln2a = (Line2*)memspc53;
  ln2Nr = sizeof(memspc53) / sizeof(Line2);
  // printf("ln2Nr max=%d\n",ln2Nr);

  // ln2ia ist zu klein !!
  if(ln2Nr > TCLA_SIZ) ln2Nr = TCLA_SIZ;


  // 2D-Hatchlinien generieren (-> ln2a)
  // ln2ia wird nicht benutzt ..
  irc = UT_hatch_2D (&ln2Nr, ln2a, ln2ia, ln2ia, ptNr, pt2a, hoff);
  if(irc < 0) return irc;



/*
  // TESTAUSG hatchlines
  for(i1=0; i1<ln2Nr; ++i1) {
    DEB_dump_obj__ (Typ_LN2,&ln2a[i1],"L[%d] %d %d\n",i1,ln2ia[i1],ln2ia[i1]);
    UT3D_ln_2pt2(&ln1,&ln2a[i1].p1,&ln2a[i1].p2);
    dli=0; GR_CreLine(&dli,1,&ln1);
  }
*/



  // hatchlines from 2D -> 3D.
  for(i1=0; i1<ln2Nr; ++i1) {

    // zurueckdrehen
    UT2D_pt_rotptangr (&ln2a[i1].p1, &pCen, &ln2a[i1].p1, hdir);
    UT2D_pt_rotptangr (&ln2a[i1].p2, &pCen, &ln2a[i1].p2, hdir);

    ln1.p1 = UT3D_pt_pt2 (&ln2a[i1].p1);
    ln1.p2 = UT3D_pt_pt2 (&ln2a[i1].p2);

    UT3D_pt_tra_pt_m3 (&lTab[i1].p1, trmat, &ln1.p1);
    UT3D_pt_tra_pt_m3 (&lTab[i1].p2, trmat, &ln1.p2);

    // hatchlines anzeigen
    // DEB_dump_obj__ (Typ_LN,&lTab[i1],"L[%d]\n",i1);
    // dli = -1; GR_CreLine (&dli, 1, &lTab[i1]);
  }


  *lnNr = ln2Nr;
  // printf("ex UT_hatch_ccv %d\n",*lnNr);

  return 0;

}



//======================== EOF ======================
