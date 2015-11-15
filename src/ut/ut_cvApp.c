// ut_cvApp.c
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
\file  ../ut/ut_cvApp.c
\brief change polygon -> circles, lines 
\code
=====================================================
List_functions_start:

cvApp_test_wr
cvApp_app            change polygon -> circles/lines
cvApp_reset_work
cvApp_work           test tangent
cvApp_check_dist
cvApp_check_nearest
cvApp_check_obj
cvApp_BB2
cvApp_BB1            3 circles - different directions
cvApp_2B
cvApp_1B1
cvApp_BL
cvApp_out_obj        copy out
cvApp_loadLastVec    get exiting vec
cvApp_delLastLine    eliminate line

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


-------------------------------------------------------------
Input:
 Punktetabelle pTab (ein 2D-Polygon = Kurve),
   mind. 3 Punkte


Output:
 ObjTab (eine 2D-Tabelle aus Kreisboegen, Linien).


Ablauf:




-------------------------------------------------------------
Strategie:
 3-n Punkte der Reihe nach testen; gegen Schluss zu testen, ob schon
 alle gehen.


Die 3 Punkte sind ps, pi, pe.
Die Strecke ps-pi ist ls, pi-pe ist le.


Wenn pe in Richtung vs hinter ps liegt: keine Loesung moeglich; skip Point.


Das vorherige Element verlaengern versuchen. Wenns nicht geht:


Wenn (ls > minl) und (le > minl) {

  (ls > 2*minl) oder (le > 2*minl): Cir-Ln bzw Ln-Cir machen

  Nun sind ls und le > minl < 2*minl;
    einen Bogen machen; den Auslaufwinkel ignorieren.

}

2 gegenlaeufige Kreisbogen errechnen (_BB1).






=================================================================

Offline testen:
cc ut_cvApp.c -DOFFLINE ut_txt.o ut_geo.o ut_bspl.o ut_bez.o ut_umem.o ut_math.o ut_TX.o -lm&&a.out




-------------------------------------------------------------


*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_geo.h"
#include "../ut/ut_cvApp.h"








// UT_TOL_ln war cvApp_lmin
// UT_TOL_cv war cvApp_tol
// double    cvApp_tol=0.005;  // Abweichung von Originalkurve
// double    cvApp_lmin=0.1;   // Approximation: MindestLineLen.









static int       oTab0Siz,  oTab0Nr;
static ObjG2     *oTab0;        // die (extrene) Ausgabetabelle


static ObjG2     oTab1[3];      // Hilfsausgabetabelle 1
static ObjG2     oTab2[3];      // Hilfsausgabetabelle 2




//========================================================
//========================================================
#ifdef OFFLINE

int main () {


  int      i1, p2TabNr, o2TabNr;
  Point2   *p2Tab;
  ObjG2    *o2Tab;
  char     memspc51[50000], memspc53[50000], cbuf[256];
  FILE     *fp1;


  printf("Start test cvApp_app\n");


  p2Tab = (Point2*)memspc51;
  p2TabNr = sizeof(memspc51) / sizeof(Point2);


  // fuer die Ergebnisobjekte (Circ's)
  o2Tab = (void*) memspc53;
  o2TabNr = sizeof(memspc53) / sizeof(ObjG2);


  // 2D-Punktetabelle einlesen
  fp1 = fopen("../../tmp/appr.lst", "r");
  i1=0;
  while (!feof (fp1)) {
    fscanf (fp1, "%lf %lf",&p2Tab[i1].x,&p2Tab[i1].y);
    // printf(" [%d] %lf %lf\n",i1,p2Tab[i1].x,p2Tab[i1].y);
    ++i1;
  }

  fclose(fp1);

  p2TabNr = i1 - 1;
  printf(" p2TabNr=%d\n",p2TabNr);


  // Tol setzen
  UT_TOL_cv = 0.0038;
  UT_TOL_ln = 0.2;


  // approxim.
  cvApp_app (o2Tab, &o2TabNr, p2Tab, p2TabNr);



  //---------------------------------------------------------
  // darstellen
  fp1=fopen("../tmp/xa.apt","w");

  fprintf(fp1, "P20=P(20 0)\n");


  for(i1=0; i1<o2TabNr; ++i1) {
    if(o2Tab[i1].typ != 4) printf("!!!!!!!!!!!!\n");

    sprintf(cbuf, "C%d=ARC P(%f %f) P(%f %f) P(%f %f)",
            i1+20,
            o2Tab[i1].p1.x,o2Tab[i1].p1.y,
            o2Tab[i1].p2.x,o2Tab[i1].p2.y,
            o2Tab[i1].pc.x,o2Tab[i1].pc.y);
    if(o2Tab[i1].rad < 0.) strcat(cbuf, " CW");

    fprintf(fp1, "%s\n",cbuf);
  }


  fprintf(fp1, ":DYNAMIC_AREA\n"),
  fprintf(fp1, "VIEW 1.4 0 0 26 10 0 603\n");

  fclose(fp1);




  return 0;

}
#endif
//========================================================
//========================================================




//=======================================================================
  int cvApp_test_wr (Point2 *p2Tab,  int ptNr) {
//=======================================================================

  int  i1;
  FILE *fpo;


  fpo = fopen("../tmp/appr.lst", "w");

  for(i1=0; i1<ptNr; ++i1) {
    fprintf(fpo, "%f %f\n",p2Tab[i1].x,p2Tab[i1].y);
  }
  fclose(fpo);


  return 0;

}


//=======================================================================
  int cvApp_app (ObjG2 oTab[], int *oTabNr, Point2 pTab[], int pTabNr) {
//=======================================================================
/// \code
/// Polygon pTab in Kreisbogen/Linien (oTab) umwandeln
/// 
/// Input:
///   oTabNr: beim Entry Size of oTab!
/// \endcode


  int     i1, irc, istart, iend, ifertig, ifound, ipos;
  Point2  pts;
  Vector2 vcs;



  oTab0Siz = *oTabNr;
  oTab0Nr  = 0;
  oTab0    = oTab;


  // printf("cvApp_appr_s siz=%d pTabNr=%d\n",oTab0Siz,pTabNr);
  // printf("      tol=%f lmin=%f\n",UT_TOL_cv,UT_TOL_ln);


  if(pTabNr < 3) {
    TX_Error("cvApp: zu wenig Eingabepunkte");
    goto L_KO;
  }



  istart = 0;
  ifertig = pTabNr - 1;

  UT2D_vc_2pt (&vcs, &pTab[0], &pTab[1]);
  UT2D_vc_normalize (&vcs, &vcs);

  pts = pTab[0];



  //=================================================================
  // wenn weniger als 20 Punkte: testen ob Schlusszyklus moeglich
  L_next_Teilstueck:

  ifound = 0;  // keine verfuegbare Loesung


  if((pTabNr - istart) < 50) {
    iend = pTabNr - 1;
    irc = cvApp_work (istart, iend, &pts, &vcs, pTab);
    if(irc == 0) {
      ifound = iend;
      memcpy (oTab2, oTab1, sizeof(oTab1));
      goto L_speichern;
    }
  }


  iend = istart + 1;



  //==================================================================
  // normaler Zyklus
  L_next_Point:
  ++iend;
  // if(iend >= ifertig) {
  if(iend > ifertig) {
    if(ifound != 0) goto L_speichern;
    TX_Error("Endzyklus nicht loesbar");
    goto L_KO;
  }


  // if(iend > 100) goto L_fertig; // nur Test Test Test


  irc = cvApp_work (istart, iend, &pts, &vcs, pTab);
  if(irc == 0) {
    ifound = iend;     // erste bzw naechste verfuegbare Loesung
    memcpy (oTab2, oTab1, sizeof(oTab1));
    goto L_next_Point;
  }
  // Fehler aufgetreten. -1: Toleranz ueberschritten, -2: keine Loesung moeglich
  // if(irc < -2) goto L_fertig;     // -3 = Testswitch

/*
  if(iend >= ifertig) {
    TX_Error("Endzyklus nicht loesbar");
    irc = -1;
    goto L_fertig;
  }
*/

  // if((iend - istart) < 150) goto L_next_Point; // check 150 points
  if(ifound == 0) goto L_next_Point;



  //==================================================================
  // letzte verfuegbare Loesung abspeichern.
  L_speichern:

  // wenn Objekt geaendert wurde:
  if(oTab2[2].typ != 0) {
    --oTab0Nr;
    cvApp_out_obj (&oTab2[2]);
  }
  // die neuen zufuegen
  if(oTab2[0].typ != 0) cvApp_out_obj (&oTab2[0]);
  if(oTab2[1].typ != 0) cvApp_out_obj (&oTab2[1]);


  // if(oTab0Nr > 1) goto L_fertig; // nur Test Test Test


  // ausser wenn ganz fertig eine following Line eliminieren.
  if(ifound < ifertig) {
    cvApp_delLastLine (&iend, pTab);
  }


  // neuen Startpunkt und Startvektor laden
  istart = ifound + 1;

  cvApp_loadLastVec (&vcs);
  UT2D_vc_normalize (&vcs, &vcs);

  pts = oTab0[oTab0Nr-1].p2;

  if(istart < ifertig) goto L_next_Teilstueck;



  irc = 0;



  //==================================================================
  L_fertig:
  *oTabNr = oTab0Nr;
  // printf("ex cvApp_app rc=0\n");


/*
  //---------- nur Testausgaben ---------------------
  for(i1=0; i1<oTab0Nr; ++i1) {
    printf("%02d typ=%d p1=%9.3f,%9.3f p2=%9.3f,%9.3f r=%9.3f\n",
            i1,oTab[i1].typ,
            oTab[i1].p1.x,oTab[i1].p1.y,
            oTab[i1].p2.x,oTab[i1].p2.y,  oTab[i1].rad);
  }
  //---------- nur Testausgaben ---------------------
*/




  return 0;



  //==================================================================
  L_KO:
  *oTabNr = oTab0Nr;
  // printf("ex cvApp_app rc=-1\n");

  return -1;


}


//=======================================================================
  int cvApp_reset_work () {
//=======================================================================


  oTab1[0].typ = 0;
  oTab1[1].typ = 0;
  oTab1[2].typ = 0;


  return 0;

}


//=======================================================================
  int cvApp_work (int istart, int iend, Point2 *pts, Vector2 *vcs, Point2 *pTab) {
//=======================================================================
// Anschlusstangente muss nach beiden Seiten stimmen
// Abweichung testen
// RC =  0: OK, Abweichung wird nicht ueberschritten.
// RC = -1: Abweichung wird ueberschritten.


  int     irc;
  double  ln_s, ln_e, d1;
  Point2  pti;
  Vector2 vce, vce_i;



  // printf("========================\ncvApp_work %d %d\n",istart,iend);



  // Wenn pe in Richtung vs hinter ps liegt: keine Loesung moeglich;
  // skip Point.
  d1 = UT2D_slen_pt_pt_vc__ (&pTab[iend], pts, vcs);
  if(fabs(d1) < UT_TOL_ln) {
    // printf(" ############# keine Loesung moeglich (1) ####### %f\n",d1);
    irc = -2;
    goto L_fertig;
  }
  


  cvApp_reset_work ();




  // Auslauftangente berechnen
  UT2D_vc_2pt (&vce, &pTab[iend-1], &pTab[iend]);
  UT2D_vc_normalize (&vce, &vce);




  // den Tangentenschnittpunkt pti und die Laengen dorthin berechnen
  UT2D_vc_invert (&vce_i, &vce); // inverse Auslauftangente
  UT2D_pt_int2pt2vc (&pti, pts, vcs, &pTab[iend-1], &vce_i);
  ln_s = UT2D_slen_pt_pt_vc__ (&pti, pts, vcs);
  ln_e = UT2D_slen_pt_pt_vc__ (&pTab[iend], &pti, &vce);


/*
  printf("   pts=%f,%f\n",pts->x,pts->y);
  printf("   pti=%f,%f\n",pti.x,pti.y);
  printf("   pte=%f,%f\n",pTab[iend].x,pTab[iend].y);
  printf("   vcs=%f,%f\n",vcs->dx,vcs->dy);
  printf("   vce=%f,%f\n",vce.dx,vce.dy);
  printf("   ls=%f le=%f\n",ln_s,ln_e);
*/




  //==========================================================
  // Das vorherige Element verlaengern versuchen.
  if(oTab0Nr > 0) {

    // an das letzte Element im Buffer anschliessen
    irc = cvApp_BB2 (pts, &pti, &pTab[iend]);
    if(irc < 0) {
      cvApp_reset_work ();
      goto L_1;
    }
    irc = cvApp_check_dist (istart, iend, pTab);
    if(irc < 0) {
      cvApp_reset_work ();
      goto L_1;
    }
    goto L_fertig;

  }



  //==========================================================
  L_1:
  // Sind die Tangenten parallel ?
  // 1 Grad = 0.017;     0.1 Grad = 0.0017
  if(UT2D_comp2vc_p (vcs, &vce, 0.0017) == 0) goto L_0;

    // Line od Bogen-Bogen
    irc = cvApp_2B (pts, &pTab[iend], &vce);
    if(irc < 0) {
      cvApp_reset_work ();
      goto L_0;
    }
    irc = cvApp_check_dist (istart, iend, pTab);
    if(irc < 0) {
      cvApp_reset_work ();
      goto L_0;
    }
    goto L_fertig;






  //==========================================================
  L_0:

  // sind ls und le mindestens UT_TOL_ln lang ?
  if((ln_e < UT_TOL_ln)||(ln_e < UT_TOL_ln)) goto L_3;



  // ist ls oder le laenger als 2 * lmin ?
  d1 = 2. * UT_TOL_ln;
  if((ln_e < UT_TOL_ln)||(ln_e < UT_TOL_ln)) goto L_2;



  // ls und le > lmin; ls und le < 2*lmin;
  // einen Bogen machen und den Auslaufwinkel ignorieren.
  cvApp_1B1 (pts, vcs, &pTab[iend]);
  goto L_check_dist;




  //==========================================================
  // ls und le > lmin; ls oder le > 2*lmin; make Ln-Cir od Cir-Ln
  L_2:
  // am laengeren Teilstueck eine Line, am kuerzeren einen Bogen ansetzen.
  cvApp_BL (pts, &pti, &pTab[iend]);
  goto L_check_dist;






  //==========================================================
  // ls oder le < lmin; da hilft nur 2 gegenlaeufige Kreisbogen errechnen.
  L_3:
  irc = cvApp_BB1 (pts, &pti, &pTab[iend]);
  if(irc < 0) {
    // printf(" ############# keine Loesung moeglich (2) #######\n");
    irc = -2;
    goto L_fertig;
  }
  goto L_check_dist;






  //==========================================================
  L_check_dist:
  irc = cvApp_check_dist (istart, iend, pTab);

  // return -3; // Test Test Test
  // exit(0);



  //==========================================================
  L_fertig:
  // printf("ex cvApp_work rc= %d\n",irc);
  return irc;

}


//=======================================================================
  int cvApp_check_dist (int istart, int iend, Point2 *pTab) {
//=======================================================================
// RC =  0: OK, Abweichung wird nicht ueberschritten.
// RC = -1: Abweichung wird ueberschritten.


  int    is, ie, irc;


  // printf("cvApp_check_dist %d - %d\n",istart,iend);


  // bei den letzten 3 Punkten nix mehr pruefen
  if(istart >= (iend-2)) goto L_OK;


  is = istart;


  // den Gueltigkeitsbereich von Obj 1 feststellen
  if(oTab1[1].typ == 0) {  // wenns nur 1 Objekt gibt
    ie = iend;
    goto L_co1;
  }


  // Index in pTab suchen, der dem Endpunkt von Obj 1 entspricht
  ie = cvApp_check_nearest (&oTab1[0].p2, pTab, iend);



  // Obj 1 testen
  L_co1:
  irc = cvApp_check_obj (&oTab1[0], is, ie, pTab);
  if(irc < 0) goto L_KO;



  if(oTab1[1].typ == 0) goto L_OK;


  // den Gueltigkeitsbereich von Obj 2 feststellen
  is = ie;
  ie = iend;


  // Obj 2 testen
  irc = cvApp_check_obj (&oTab1[1], is, ie, pTab);
  if(irc < 0) goto L_KO;


  L_OK:
  // printf("ex cvApp_check_dist 0\n");
  return 0;


  L_KO:
  // printf("ex cvApp_check_dist -1\n");
  return -1;

}


//=======================================================================
  int cvApp_check_nearest (Point2 *pt, Point2 pTab[], int ie) {
//=======================================================================
// den Punkt pt in der pTab suchen; und zwar von hinten nach vorn,
// beginnend mit pTab[ie].
// Abstaende sollten immer geringer werden; wenn der Abstand wieder
// groesser wird, ist der Punkt gefunden.


  int     iact;
  double  d1, ds;



  ds = UT2D_len_2pt (pt, &pTab[ie]);

  iact = ie;


  L_next:
  --iact;
  if(iact < 0) goto L_fertig;

  d1 = UT2D_len_2pt (pt, &pTab[iact]);
  if(d1 < ds) {
    ds = d1;
    goto L_next;
  }


  L_fertig:

  ++iact;

  // printf("ex cvApp_find_nearest %d <- %d\n",iact,ie);

  return iact;

}


//=======================================================================
  int cvApp_check_obj (ObjG2 *o1, int is, int ie, Point2 *pTab) {
//=======================================================================

  double    d1;
  Vector2   vl;


  // printf("cvApp_check_obj %d %d typ=%d\n",is,ie,o1->typ);


  // Vorbereitungen f Typ_LN:
  if(o1->typ == Typ_LN) {
    UT2D_vc_2pt (&vl, &o1->p1, &o1->p2);


  // } else {
    // TX_Error("cvApp_check_obj E1");
    // return -1;
  }


  --is;  // erster Punkt wird auch geprueft (2. von 2 Circ's)



  //==========================================================
  L_next_point:
  ++is;

  if(is > ie) return 0;  // letzter Punkt wird auch geprueft (1. von 2 Cir's)



  //---------- test Line ---------
  if(o1->typ == Typ_LN) {
    d1 =  UT2D_slen_nor_2pt_vc__ (&pTab[is], &o1->p1, &vl);
    // printf("  #=%d d1=%f\n",is,d1);
    if(fabs(d1) > UT_TOL_cv) return -1;




  //---------- test Circ ---------
  } else if (o1->typ == Typ_CI) {
    d1 =  UT2D_len_2pt (&pTab[is], &o1->pc) - fabs(o1->rad);
    // printf("  #=%d d1=%f\n",is,d1);
    if(fabs(d1) > UT_TOL_cv) return -1;





  //------------------------------
  } else {
    TX_Error("cvApp_check_obj E2");
    return -1;
  }


  goto L_next_point;


  return 0;

}


//=======================================================================
  int cvApp_BB2 (Point2 *pts, Point2 *pti, Point2 *pte) {
//=======================================================================
// an das letzte Element im Buffer anschliessen

  int     i1, idir;
  double  r1, r2, len_ac;
  Point2  c1, c2, ci, c1s;
  Vector2 vce, vce_i, vc1;
  // ObjG2   o1;



/*
  printf("cvApp_BB2  \n");
  printf("     ps=%f,%f\n",pts->x,pts->y);
  printf("     pi=%f,%f\n",pti->x,pti->y);
  printf("     pe=%f,%f\n",pte->x,pte->y);
*/



  // zuerst das vorherige Element zurueckholen
  i1  = oTab0Nr-1;
  c1s = oTab0[i1].p1;
  c1  = oTab0[i1].pc;
  r1  = oTab0[i1].rad;
  // printf("old - pc=%f,%f  r=%f\n",c1.x,c1.y,r1);


  // Auslauftangente berechnen
  UT2D_vc_2pt (&vce, pti, pte);
  UT2D_vc_normalize (&vce, &vce);
  UT2D_vc_invert (&vce_i, &vce); // invers weil aussen innen so definiert


  if(r1 > 0.) {
    r2 =  1.;   // vorher CCW -> 1=oberer
  } else {
    r2 = -1.;   // vorher CW -> -1=unterer Kr.
  }


  // Kreis errechnen, der den vorherigen Kreis tangiert und an den Endpunkt
  // anschliesst = Bogen/Bogen.
  // o2.rad = -o2.rad; // nur Test !
  i1 = UT2D_ci_ptvcci (&c2, &ci, &r2, pte, &vce_i, &c1, r1);

  if(i1 < 0) return -1; // eine Linie von o2.p1 -> pe; wuerde wieder
  // weggeloescht;

  // printf("     ci=%f,%f\n",ci.x,ci.y);


  // Umdrehungssinn von o2 noch fixieren; ist genau entgegen dem o1.
  // o2.rad ist positiv, wenn gegenlaeufig, negativ wenn gleichlaeufig.
  if(r1 > 0.) r2 = -r2;




  //--------------------------------------------
  // Test ob ein cir nun nicht nach hinten geht


  // Wie lang ist Arc1 ?
  // Kann leider auch retour gehen !
  idir = 1;
  if(r1 < 0.) idir = -1;
  UT2D_vc_cipt (&vc1, &c1, &c1s, idir);
  len_ac = UT2D_slen_pt_pt_vc__ (&ci, &c1s, &vc1);
  // printf(" BB2: len_ac1=%f\n",len_ac);
  if(len_ac < UT_TOL_ln) return -1;


  // Wie lang ist Arc2 ?
  // Kann leider auch retour gehen !
  idir = 1;
  if(r2 < 0.) idir = -1;
  UT2D_vc_cipt (&vc1, &c2, &ci, idir);
  len_ac = UT2D_slen_pt_pt_vc__ (pte, &ci, &vc1);
  // printf(" BB2: len_ac2=%f\n",len_ac);
  if(len_ac < UT_TOL_ln) return -1;






  //====================================================================
  // das korrigierte Obj raus
  oTab1[2].typ = Typ_CI;
  oTab1[2].p1  = c1s;
  oTab1[2].p2  = ci;
  oTab1[2].pc  = c1;
  oTab1[2].rad = r1;



  // das neue Obj raus
  oTab1[0].typ = Typ_CI;
  oTab1[0].p1  = ci;
  oTab1[0].p2  = *pte;
  oTab1[0].pc  = c2;
  oTab1[0].rad = r2;



  
  return 0;

}


//=======================================================================
  int cvApp_BB1 (Point2 *pts, Point2 *pti, Point2 *pte) {
//=======================================================================
// 2 gegenlaeufige Kreisbogen errechnen;
// tangential an pts -> pti durch pts
// UND tangential an pti -> pte durch pte.


  int     irc, tryNr;
  double  r1, r2, dy, lg, d1;
  Point2  c1, c2, ci, ptm;
  Vector2 vcs, vce, vce_i;

/*
  printf("cvApp_BB1 \n");
  printf("   pts=%f,%f\n",pts->x,pts->y);
  printf("   pti=%f,%f\n",pti->x,pti->y);
  printf("   pte=%f,%f\n",pte->x,pte->y);
*/



  UT2D_vc_2pt (&vcs, pts, pti);
  UT2D_vc_2pt (&vce, pti, pte);
  UT2D_vc_normalize (&vcs, &vcs);
  UT2D_vc_normalize (&vce, &vce);
  // printf("   vcs=%f,%f\n",vcs.dx,vcs.dy);
  // printf("   vce=%f,%f\n",vce.dx,vce.dy);


  // den Radius schaetzen mit ca 50 Prozent pts-pte.
  lg = UT2D_len_2pt (pts, pte);
  r1 = lg / 2.;


  // die Gesamtlaenge muss mind 2 lmin sein
  if(lg < (UT_TOL_ln*2.)) return -2;


  // wenn vcs-vce spitz, geht nix.
  d1 = acos(UT2D_acos_2vc (&vcs, &vce));
  if(d1 > RAD_90) return -2;


  // befindet sich pte ueber oder unter der Linie pts-vcs
  dy = UT2D_slen_nor_2pt_vc__ (pte, pts, &vcs);

  tryNr = 0;


  L_noamoi:

  // c2 errechnen; oberer (1) oder unterer (-1) ?
  if(dy < 0) {
    r1 = -r1;
    r2 =  1.; // oberer
  } else {
    // r1 = -r1;
    r2 = -1.; // unterer
  }


  // pte oberhalb von pts-vcs: c1 nach unten.
  UT2D_pt_tranorptvclen (&c1, pts, &vcs, -r1);
  // printf("   c1=%f,%f\n",c1.x,c1.y);
  // printf("   r1=%f  dy=%f lg=%f\n",r1,dy,lg);





  UT2D_vc_invert (&vce_i, &vce); // inverse Auslauftangente
  irc = UT2D_ci_ptvcci (&c2, &ci, &r2, pte, &vce_i, &c1, r1);
  // printf("   c2=%f,%f\n",c2.x,c2.y);
  // printf("   ci=%f,%f\n",ci.x,ci.y);
  // printf("   r2=%f rc=%d\n",r2,irc);
  if(irc != 0) return -2;
  




  if(r2 < 0.) {
    if(r1 < 0.) {
      r2 = fabs(r2);
    } else {
      r2 = -fabs(r2);
    }
  } else {
    if(r1 < 0.) {
      r2 = -fabs(r2);
    } else {
      r2 = fabs(r2);
    }
  }






  // wenn die Laenge des c1 < als 40 prozent von ps-pe, dann r1 erhoehen.
  d1 = UT2D_len_2pt (pts, &ci);
  if(d1 < (lg*0.4)) {
    r1 = fabs(r1) * 2.;
    ++tryNr;
    // printf("  try=%d r1=%f\n",tryNr,r1);
    if(tryNr < 32) goto L_noamoi;
  }






  //------------------------------------------------------
  // fertig; ausgeben:


/*
  // nur Test: Schnittpunkt
  oTab1[0].typ = Typ_PT;
  oTab1[0].p1  = ci;
  cvApp_out_obj (&oTab1[0]);
*/



  // die Cirs raus
  oTab1[0].typ = Typ_CI;
  oTab1[0].p1  = *pts;
  oTab1[0].p2  = ci;
  oTab1[0].pc  = c1;
  oTab1[0].rad = -r1;


  oTab1[1].typ = Typ_CI;
  oTab1[1].p1  = ci;
  oTab1[1].p2  = *pte;
  oTab1[1].pc  = c2;
  oTab1[1].rad = r2;


  return 0;

}


//=======================================================================
  int cvApp_2B (Point2 *pts, Point2 *pte, Vector2 *vc1) {
//=======================================================================
// 2 Boegen oder 1 Linie ansetzen
// Die Tangente an pts und an pte ist vc1 !

  double lne;
  Point2 pti, ptm;


  // printf("cvApp_2B \n");


  // wenn pte auf pts-vc1 liegt: einfache Verbindungslinie raus.
  // Normale von pte auf pts-vc1
  UT2D_pt_projptptvc (&pti, pte, pts, vc1);
  lne = UT2D_len_2pt (&pti, pte);

  if(lne > UT_TOL_pt) goto L_BogenBogen;
  return -1;

/*
  // macht am Beginn Probleme und wird sowieso eliminiert!
  // einfache Verbindungslinie raus.
  oTab1[0].typ = Typ_LN;
  oTab1[0].p1  = *pts;
  oTab1[0].p2  = *pte;
  return 0;
*/



  //--------------------------------------------------------------
  // 2 (gleichgrosse) Boegen errechnen
  L_BogenBogen:
  UT2D_pt_mid2pt (&ptm, pts, pte);

  oTab1[0].typ = Typ_CI;
  oTab1[0].p1  = *pts;
  oTab1[0].p2  = ptm;
  UT2D_ci_ptvcpt (&oTab1[0].pc, &oTab1[0].rad, pts, vc1, &ptm);

  oTab1[1].typ = Typ_CI;
  oTab1[1].p1  = ptm;
  oTab1[1].p2  = *pte;
  UT2D_pt_opp2pt (&oTab1[1].pc, &ptm, &oTab1[0].pc);
  oTab1[1].rad = -oTab1[0].rad;


  return 0;
}


//=======================================================================
  int cvApp_1B1 (Point2 *pts, Vector2 *vcs, Point2 *pte) {
//=======================================================================
// einen Bogen machen; den Auslaufwinkel ignorieren.



  // printf("cvApp_1B1\n");



  // comp. Mittelpunkt aus Starpunkt/Startvektor (p1/vc1) u einem Pt p2
  oTab1[0].typ = Typ_CI;
  oTab1[0].p1  = *pts;
  oTab1[0].p2  = *pte;
  UT2D_ci_ptvcpt (&oTab1[0].pc, &oTab1[0].rad, pts, vcs, pte);

  return 0;
}




//=======================================================================
  int cvApp_BL (Point2 *pts, Point2 *pti, Point2 *pte) {
//=======================================================================
// am laengeren Teilstueck eine Line, am kuerzeren einen Bogen ansetzen.


  double  l1, l2;
  Point2  pth;
  Vector2 vcs;


/*
  printf("cvApp_BL s=%f,%f\n",pts->x,pts->y);
  printf("         i=%f,%f\n",pti->x,pti->y);
  printf("         e=%f,%f\n",pte->x,pte->y);
*/


  UT2D_vc_2pt (&vcs, pts, pti);

  l1 = UT2D_len_2pt (pts, pti);
  l2 = UT2D_len_2pt (pti, pte);
  // printf(" l1=%f l2=%f vcs=%f,%f\n",l1,l2,vcs.dx,vcs.dy);


  if(l2 > l1) goto L_Bogen_Line;




  //======================================================
  // zuerst Line, dann Bogen.
  UT2D_pt_traptvclen (&pth, pti, &vcs, -l2);

  oTab1[0].typ = Typ_LN;
  oTab1[0].p1  = *pts;
  oTab1[0].p2  = pth;

  oTab1[1].typ = Typ_CI;
  oTab1[1].p1  = pth;
  oTab1[1].p2  = *pte;
  UT2D_ci_ptvcpt (&oTab1[1].pc, &oTab1[1].rad, &pth, &vcs, pte);

  return 0;



  //======================================================
  // zuerst Bogen, dann Line.
  L_Bogen_Line:
  UT2D_pt_tra2ptlen (&pth, pti, pte, l1);

  oTab1[0].typ = Typ_CI;
  oTab1[0].p1  = *pts;
  oTab1[0].p2  = pth;
  UT2D_ci_ptvcpt (&oTab1[0].pc, &oTab1[0].rad, pts, &vcs, &pth);

  oTab1[1].typ = Typ_LN;
  oTab1[1].p1  = pth;
  oTab1[1].p2  = *pte;


  return 0;

}


//=======================================================================
  int cvApp_out_obj (ObjG2 *o1) {
//=======================================================================
// o1 in die Tabelle oTab kopieren

/*
  printf("cvApp_out_obj %d typ=%d\n",oTab0Nr,o1->typ);
  printf("    p1=%f,%f\n",o1->p1.x,o1->p1.y);
  printf("    p2=%f,%f\n",o1->p2.x,o1->p2.y);
  printf("    pc=%f,%f rad=%f\n",o1->pc.x,o1->pc.y,o1->rad);
*/



  oTab0[oTab0Nr] = *o1;


  if(oTab0Nr < oTab0Siz) {
    ++oTab0Nr;
  } else {
    printf(" cvApp_out_obj: oTab zu klein\n");
  }

  return 0;

}


//=======================================================================
  int cvApp_loadLastVec (Vector2 *vc1) {
//=======================================================================
// den Auslaufvektor des letzen Objekts

  int  i1, idir;


  i1 = oTab0Nr - 1;



  if(oTab0[i1].typ != Typ_LN) goto L_ci;
  UT2D_vc_2pt (vc1, &oTab0[i1].p1, &oTab0[i1].p2);
  goto L_fertig;



  //--------------------------------
  L_ci:

  if(oTab0[i1].typ != Typ_CI) {
    TX_Error("cvApp_loadLastVec E1");
    return -1;
  }
  idir = 1; if(oTab0[i1].rad < 0.) idir = -1;
  UT2D_vc_cipt (vc1, &oTab0[i1].pc, &oTab0[i1].p2, idir);



  L_fertig:
  // printf("ex cvApp_loadLastVec %f,%f\n",vc1->dx,vc1->dy);

  return 0;

}


//=======================================================================
  int cvApp_delLastLine (int *iend, Point2 *pTab) {
//=======================================================================
// eine following Line eliminieren.

  int  i1, ie;


  i1 = oTab0Nr - 1;


  if(oTab0[i1].typ != Typ_LN) return 0;

  // den dieser Linie folgenden pTab-Punkt suchen (ist nicht exakt !!)
  ie = cvApp_check_nearest (&oTab0[i1].p2, pTab, *iend);

  --oTab0Nr;


  L_fertig:
  // printf("ex cvApp_delLastLine %d <- %d\n",ie,*iend);

  return ie;

}


//========================= EOF ==============================
