// Berechnungsfunktionen fuer polynomische splines.      RF  2000-04-28
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
2001-04-28 Neu. RF.

-----------------------------------------------------
*/
/*!
\file  ../ut/ut_pspl.c
\brief polynom. splines
\code
=====================================================
List_functions_start:

psp_psp3_tra_m3  transform Polynom.Spline with matrix
pspl_pol_psp     Polynom.Spline -> Polygon
psp_dist1        normalSist pt - line
psp_ioff_psp     get segmentoffset & segmentNr
psp_pt_psp       get pt from dist

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
#include "../ut/ut_txt.h"

#include "../gr/ut_UI.h"               // SYM_..


#include "../xa/xa.h"
#include "../xa/xa_mem.h"



  double psp_dist1(Point *pts, Point *pt1, Point *pte);



//================================================================
  int psp_psp3_tra_m3 (ObjGX *cvo, ObjGX *cvi, Mat_4x3 trmat) {
//================================================================
/// psp_psp3_tra_m3  transform Polynom.Spline with matrix
/// cvo must have enough size for complete curve (ox + polynoms)


  int        i1;
  char       *pe;
  Point      pt0, pt1;
  polynom_d3 *pola, pol1;
  ObjGX      *ox1;


  if(cvi->form != Typ_CVPSP3) return -1;

  // pointer to outputfield
  ox1 = cvo;

  // copy primary ObjGX
  memcpy(ox1, cvi, sizeof(ObjGX));

  // set startAdress of Data to space behind primary ObjGX
  pe = (char*)ox1 + sizeof(ObjGX);

  // correct the data-position
  ox1->data = pe;



  pola = cvi->data;

  UT3D_pt_traptm3 (&pt0, trmat, (Point*)&UT3D_PT_NUL);

    for(i1=0; i1<cvi->siz; ++i1) {
      pol1 = pola[i1];

      pt1.x = pol1.x.a;
      pt1.y = pol1.y.a;
      pt1.z = pol1.z.a;
      UT3D_pt_traptm3 (&pt1, trmat, &pt1);
      pol1.x.a = pt1.x;
      pol1.y.a = pt1.y;
      pol1.z.a = pt1.z;

      pt1.x = pol1.x.b;
      pt1.y = pol1.y.b;
      pt1.z = pol1.z.b;
      UT3D_pt_traptm3 (&pt1, trmat, &pt1);
      pol1.x.b = pt1.x - pt0.x;
      pol1.y.b = pt1.y - pt0.y;
      pol1.z.b = pt1.z - pt0.z;

      pt1.x = pol1.x.c;
      pt1.y = pol1.y.c;
      pt1.z = pol1.z.c;
      UT3D_pt_traptm3 (&pt1, trmat, &pt1);
      pol1.x.c = pt1.x - pt0.x;
      pol1.y.c = pt1.y - pt0.y;
      pol1.z.c = pt1.z - pt0.z;

      pt1.x = pol1.x.d;
      pt1.y = pol1.y.d;
      pt1.z = pol1.z.d;
      UT3D_pt_traptm3 (&pt1, trmat, &pt1);
      pol1.x.d = pt1.x - pt0.x;
      pol1.y.d = pt1.y - pt0.y;
      pol1.z.d = pt1.z - pt0.z;

      memcpy(pe, &pol1, sizeof(polynom_d3));
      pe += sizeof (polynom_d3);
    }

  return 0;

}


//=====================================================================
  int pspl_pol_psp (int *ptNr, Point *pTab,
                    ObjGX *cv1, int ptMax, double tol) {
//=====================================================================
// Polynom.Spline -> Polygon. Nur durch Generieren Mittelpunkte und 
// Abweichung messen.
// See also  bspl_pol_bsp UT3D_cv_rbsp

// ACHTUNG: pTab=memspc55

#define STACKSIZ 30

  int        i1, is, ptIn, ptOut, ptStackNr, pTab1Max, PolCoNr;
  double     us, ue, ps, pu, dist, uStack[STACKSIZ];
  Point      pt1, pts, pte, ptStack[STACKSIZ];
  polynom_d3 *PolCo;


  // printf("pspl_pol_psp ptMax=%d tol=%f\n",ptMax,tol);
  // UTO_dump__ (cv1, "pspl_pol_psp");
  


  PolCoNr = cv1->siz;
  PolCo   = cv1->data;

  // printf("PolCoNr=%d\n",PolCoNr);
  // printf("Startpkt=%f,%f,%f\n",PolCo[0].x.a,PolCo[0].y.a,PolCo[0].z.a);



  // den ersten Pt sofort raus
  ptOut = 0;
  i1 = 0;
  pTab[ptOut].x = PolCo[i1].x.a;
  pTab[ptOut].y = PolCo[i1].y.a;
  pTab[ptOut].z = PolCo[i1].z.a;
  ++ptOut;          // der aktuelle Punkt ist immer pTab[ptOut-1].


  // den letzen Punkt auf den Stack.
  ptStackNr = 0;
  i1 = cv1->siz-1;
  uStack[ptStackNr] = 1.0;
  ptStack[ptStackNr].x = PolCo[i1].x.a;
  ptStack[ptStackNr].y = PolCo[i1].y.a;
  ptStack[ptStackNr].z = PolCo[i1].z.a;
  ++ptStackNr;

  pts = pTab[0];  // Startpunkt
  us = 0.;


  //-------------------------------------------
  // der Startpunkt ist pts; den naechsten Endpunkt vom ptStack holen;
  // dessen u-Wert ist uStack.

  L_next_inPoint:
  // den naechsten Endpunkt vom Stack -> pte holen
  pte = ptStack[ptStackNr-1];
  ue  = uStack[ptStackNr-1];

  // u-Mittelwert errechnen
  pu = (us + ue) / 2.0;

  // printf("next pu=%f ue=%f\n",pu,ue);

  // den Segmentindex und den Segmentoffset holen
  psp_ioff_psp (&is, &ps, pu, PolCo, PolCoNr);

  // den Punkt dazu holen (Zwischenpunkt)
  psp_pt_psp (&pt1, ps, &PolCo[is]);


  // die Abweichung feststellen
  dist = psp_dist1(&pts, &pt1, &pte);
  // if(ptStackNr > 1) {dist = 0.001;} else {dist = 10.0;} // TEST
  // see also UT2D_nlenq_3pt UT2D_len_ptln UT2D_slen_nor3pt

  if(dist > tol) {
    // Abweichung zu gross; Punkt speichern
    ptStack[ptStackNr] = pt1;
    uStack[ptStackNr]  = pu;
    ++ptStackNr;
    if(ptStackNr >= STACKSIZ) return -1;
    goto L_next_inPoint;
  }

  // Abweichung tolerierbar. pte raus.
  pTab[ptOut] = pte; // copy Point
  ++ptOut;
  --ptStackNr;
  pts = pte;
  us = ue;
  if(ptStackNr > 0) goto L_next_inPoint;


  //-------------------------------------------
  L_fertig:

  *ptNr = ptOut;

    // printf("ex pspl_pol_psp ptNr=%d\n",*ptNr);
    // GR_Disp_pTab (ptOut, pTab, SYM_STAR_S, 2);
    // GL_Disp_cv (ptOut, pTab);

  return 0;

}


//=====================================================================
  double psp_dist1(Point *pts, Point *pt1, Point *pte) {
//=====================================================================
// Normalabst von pt1 von Linie pts-pte

  double d1;
  Point  pti;
  Vector vc1;


  // printf("psp_dist1\n");


  UT3D_vc_2pt (&vc1, pts, pte);


  UT3D_pt_projptptvc (&pti, &d1, NULL, pt1, pts, &vc1);
  // d1 = UT3D_len_2pt (&pti, pt1);


  // printf(" ex psp_dist1 %f\n",d1);

  return d1;

}

/*
//=====================================================================
  int IGE_r_decpol2_112 (polynom_d3 *PolCo, int PolCoNr) {
//=====================================================================

  int     i1, is, ianz;
  double  ps, pu, pui;
  Point   pt1;


  // printf("IGE_r_decpol2_112\n");

  // Anzahl Punkte fuer den gesamten Spline
  ianz = 64 * PolCoNr;    // 10 Punkte pro Segment

  pui  = 1. / ianz;      // das u-Increment
  pu   = 0.;


  printf("ianz=%d\n",ianz);

  for(i1=0; i1<ianz; ++i1) {

    // den Segmentindex und den Segmentoffset holen
    psp_ioff_psp (&is, &ps, pu, PolCo, PolCoNr);

    // den Punkt dazu holen
    psp_pt_psp (&pt1, ps, &PolCo[is]);

    // Binaer schreiben (3 doubles)
    // fwrite (&pt1, sizeof(pt1), 1, IG_auxFilLun);
    IG_r_poly_addpt (&pt1);

    pu += pui;
  }

  // der letzte Punkt:
  pt1.x = PolCo[PolCoNr].x.a;
  pt1.y = PolCo[PolCoNr].y.a;
  pt1.z = PolCo[PolCoNr].z.a;
  // fwrite (&pt1, sizeof(pt1), 1, IG_auxFilLun);
  IG_r_poly_addpt (&pt1);



  return 0;

}
*/

//===========================================================================
  int psp_ioff_psp (int *is, double *dus, double du,
                       polynom_d3 *polTab, int polNr) {
//===========================================================================
// Berechnen des Segmentoffsets dus und zu welchem Teilsegment er gehoert.
// Input: der absolute Offset du und die absoluten Offsets der einzelnen
//   Teilsegmente in polTab[0].u bis polTab[polNr].u.

// Out: der Punkt mit Offset du befindet sich zwischen Segment is und is+1.
//   dus ist das u innerhalb der Segementgrenzen

  int     i1;
  double  d_tot, d_m, ds;


  // die Gesamtspannweite
  d_tot = polTab[polNr-1].u - polTab[0].u;

  // das multiplizerte_u
  d_m = (du * d_tot) + polTab[0].u;

  // printf("psp_ioff_psp du=%f d_tot=%f d_m=%f\n",du,d_tot,d_m);


/*
  // Anf Testausg
  for(i1=0; i1<polNr; ++i1) {
    printf(" %d u=%f p=%f,%f,%f\n",i1,polTab[i1].u, 
            polTab[i1].x.a,polTab[i1].y.a,polTab[i1].z.a);
  }
  // End Testausg
*/


  // das multiplizerte_u innerhalb der Segmente suchen
  for(i1=0; i1<polNr; ++i1) {
    if(polTab[i1].u > d_m) {
      *is = i1-1;
      goto Found;
    }
  }
  *is = polNr;


  Found:
  // der Abstand vom Startsegment zum next Segment
  ds = polTab[*is].u / d_tot;
  // printf("  ds=%f\n",ds);
  *dus = (du - ds) * d_tot;


  // printf("ex psp_ioff_psp is=%d du1=%f du=%f %f-%f\n",*is,*dus,du,
                            // polTab[*is].u,polTab[(*is)+1].u);

  return 0;

}



//===========================================================================
  int psp_pt_psp (Point *pt1, double du, polynom_d3 *pc) {
//===========================================================================
/*

IN:
  du          ... der Abstand (0. - 1.)
  pc          ... polynomCoefficient for a degree-3 polynom.
    pc.x      ... coefficients of the X-polynomial (a-d)
    pc.y      ... coefficients of the Y-polynomial (a-d)
    pc.z      ... coefficients of the Z-polynomial (a-d)

OUT:
  der 3D-Punkt pt1

-----------------------------------------------------*/

  double s, ss, sss;


  // printf("psp_pt_psp du=%f uSeg=%f\n",du,pc->u);
  // printf("          %f %f %f %f\n",pc->x.a,pc->x.b,pc->x.c,pc->x.d);

  s   = du;
  ss  = s*s;
  sss = ss*s;

  pt1->x = pc->x.a + pc->x.b*s + pc->x.c*ss + pc->x.d*sss;
  pt1->y = pc->y.a + pc->y.b*s + pc->y.c*ss + pc->y.d*sss;
  pt1->z = pc->z.a + pc->z.b*s + pc->z.c*ss + pc->z.d*sss;

  // printf("ex psp_pt_psp %f,%f,%f du=%f\n",pt1->x,pt1->y,pt1->z,du);

  return 0;
}




//=========== EOF ========================
