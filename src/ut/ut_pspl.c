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
#ifdef globTag
void CVPSP(){}
#endif
/*!
\file  ../ut/ut_pspl.c
\brief polynom. splines
\code
=====================================================
List_functions_start:

CVPSP_psp3_tra_m3  transform Polynom.Spline with matrix
CVPSP_pol_oPsp3    get polygon from Polynom.Spline-obj (CurvPsp3)
CVPSP_pol_nPsp3    get polygon from Polynom.Spline (polynom_d3[])
CVPSP_ioff_psp     get offset for Polynom.Spline
CVPSP_pt_psp       compute point from offset on Polynom.Spline
CVPSP_ptNr_oPsp3   get nr of points for polygon for Polynom.Spline
CVPSP_box_oPsp3    extend box for Polynom.Spline

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

#include "../ut/func_types.h"               // SYM_..
#include "../ut/ut_deb.h"              // DEB_*


#include "../xa/xa.h"
#include "../ut/ut_memTab.h"           // MemTab
#include "../xa/xa_mem.h"



// externals:
// ../ci/NC_Main.c:
extern double    APT_ModSiz;


// prototypes:
  double UT3D_nlen_3pt (Point *pts, Point *pt1, Point *pte);



//================================================================
  int CVPSP_psp3_tra_m3 (ObjGX *cvo, ObjGX *cvi, Mat_4x3 trmat) {
//================================================================
/// CVPSP_psp3_tra_m3  transform Polynom.Spline with matrix
/// cvo must have enough size for complete curve (ox + polynoms)

/// TODO: change to new struct CurvPsp3
/// muss man ganz neu machen, braucht Memspc (UME_init)


  int        i1;
  char       *pe;
  Point      pt0, pt1;
  polynom_d3 *pola, pol1;
  // CurvPsp3   *pspi, *pspo;
  ObjGX      *ox1;


  TX_Print("********** TODO - CVPSP_psp3_tra_m3 **************");
  // DEB_dump_ox_0 (cvi, "CVPSP_psp3_tra_m3-in");


  if(cvi->form != Typ_CVPSP3) return -1;

  // pointer to outputfield
  ox1 = cvo;

  // copy primary ObjGX (cvo = cvi)
  memcpy(ox1, cvi, sizeof(ObjGX));

  // set startAdress of Data to space behind primary ObjGX (following cvo)
  pe = (char*)ox1 + sizeof(ObjGX);

  // correct the data-position
  ox1->data = pe;



  pola = cvi->data;

  UT3D_pt_tra_pt_m3 (&pt0, trmat, (Point*)&UT3D_PT_NUL);

    for(i1=0; i1<cvi->siz; ++i1) {
      pol1 = pola[i1];

      pt1.x = pol1.x.a;
      pt1.y = pol1.y.a;
      pt1.z = pol1.z.a;
      UT3D_pt_tra_pt_m3 (&pt1, trmat, &pt1);
      pol1.x.a = pt1.x;
      pol1.y.a = pt1.y;
      pol1.z.a = pt1.z;

      pt1.x = pol1.x.b;
      pt1.y = pol1.y.b;
      pt1.z = pol1.z.b;
      UT3D_pt_tra_pt_m3 (&pt1, trmat, &pt1);
      pol1.x.b = pt1.x - pt0.x;
      pol1.y.b = pt1.y - pt0.y;
      pol1.z.b = pt1.z - pt0.z;

      pt1.x = pol1.x.c;
      pt1.y = pol1.y.c;
      pt1.z = pol1.z.c;
      UT3D_pt_tra_pt_m3 (&pt1, trmat, &pt1);
      pol1.x.c = pt1.x - pt0.x;
      pol1.y.c = pt1.y - pt0.y;
      pol1.z.c = pt1.z - pt0.z;

      pt1.x = pol1.x.d;
      pt1.y = pol1.y.d;
      pt1.z = pol1.z.d;
      UT3D_pt_tra_pt_m3 (&pt1, trmat, &pt1);
      pol1.x.d = pt1.x - pt0.x;
      pol1.y.d = pt1.y - pt0.y;
      pol1.z.d = pt1.z - pt0.z;

      memcpy(pe, &pol1, sizeof(polynom_d3));
      pe += sizeof (polynom_d3);
    }

  return 0;

}


//================================================================
  int CVPSP_ptNr_oPsp3 (int *ptNr, CurvPsp3 *cv1, double tol) {
//================================================================
// CVPSP_ptNr_oPsp3         get nr of points for polygon for Polynom.Spline
// estimation ..

  *ptNr = (cv1->plyNr * (200./APT_ModSiz)) + 100;

    // printf(" ex-CVPSP_ptNr_oPsp3 %d\n",*ptNr);

  return 0;

}


//=====================================================================
  int CVPSP_pol_oPsp3 (int *ptNr, Point *pTab,
                      CurvPsp3 *cv1, int ptMax, double tol) {
//=====================================================================
// CVPSP_pol_oPsp3        get polygon from Polynom.Spline-obj (CurvPsp3)
// Polynom.Spline -> Polygon. Nur durch Generieren Mittelpunkte und 
// Abweichung messen.
// See also  bspl_pol_bsp UT3D_cv_rbsp

// ACHTUNG: pTab=memspc55

// #define STACKSIZ 30
// 
//   int        i1, is, ptIn, ptOut, ptStackNr, pTab1Max, PolCoNr;
//   double     us, ue, ps, pu, dist, uStack[STACKSIZ];
//   Point      pt1, pts, pte, ptStack[STACKSIZ];
//   polynom_d3 *PolCo;


  // CurvPsp3  *cvPsp;

  // printf("CVPSP_pol_oPsp3 ptMax=%d tol=%f\n",ptMax,tol);
  // DEB_dump_obj__ (Typ_CVPSP3, cv1, "CVPSP_pol_psp");
  // DEB_dump_ox_0 (cv1, "CVPSP_pol_psp");
  
  // cvPsp = cv1->data;

  return CVPSP_pol_nPsp3 (ptNr, pTab, cv1->plyTab, cv1->plyNr, ptMax, tol);

}


//============================================================================
  int CVPSP_pol_nPsp3 (int *ptNr, Point *pTab,
                      polynom_d3 *PolCo, int PolCoNr, int ptMax, double tol) {
//============================================================================
// CVPSP_pol_nPsp3        get polygon from Polynom.Spline (polynom_d3[])
// Polynom.Spline -> Polygon. Nur durch Generieren Mittelpunkte und 
// Abweichung messen.
// See also  bspl_pol_bsp UT3D_cv_rbsp

// ACHTUNG: pTab=memspc55

#define STACKSIZ 30

  int        irc, i1, is, ptIn, ptOut, ptStackNr, pTab1Max;
  double     us, ue, ps, pu, dist, uStack[STACKSIZ];
  Point      pt1, pts, pte, ptStack[STACKSIZ];


  // printf("CVPSP_pol_nPsp3 PolCoNr=%d ptMax=%d tol=%f\n",PolCoNr,ptMax,tol);
  // DEB_dump_nobj__ (Typ_polynom_d3, PolCoNr, PolCo, "CV:\n");

  // printf("Startpkt=%f,%f,%f\n",PolCo[0].x.a,PolCo[0].y.a,PolCo[0].z.a);


  // den ersten Pt sofort raus
  ptOut = 0;
  i1 = 0;
  pTab[ptOut].x = PolCo[i1].x.a;
  pTab[ptOut].y = PolCo[i1].y.a;
  pTab[ptOut].z = PolCo[i1].z.a;
  ++ptOut;          // der aktuelle Punkt ist immer pTab[ptOut-1].
  if(ptOut >= ptMax) goto L_eom1;


  // den letzen Punkt auf den Stack.
  ptStackNr = 0;
  i1 = PolCoNr - 1;
  uStack[ptStackNr] = 1.0;
  ptStack[ptStackNr].x = PolCo[i1].x.a;
  ptStack[ptStackNr].y = PolCo[i1].y.a;
  ptStack[ptStackNr].z = PolCo[i1].z.a;
  ++ptStackNr;
  if(ptStackNr >= STACKSIZ) goto L_eom2;

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
  CVPSP_ioff_psp (&is, &ps, pu, PolCo, PolCoNr);

  // den Punkt dazu holen (Zwischenpunkt)
  CVPSP_pt_psp (&pt1, ps, &PolCo[is]);


  // die Abweichung feststellen
  dist = UT3D_nlen_3pt (&pts, &pt1, &pte);
  // if(ptStackNr > 1) {dist = 0.001;} else {dist = 10.0;} // TEST
  // see also UT2D_nlenq_3pt UT2D_len_ptln UT2D_slen_nor3pt

  if(dist > tol) {
    // Abweichung zu gross; Punkt speichern
    ptStack[ptStackNr] = pt1;
    uStack[ptStackNr]  = pu;
    ++ptStackNr;
    if(ptStackNr >= STACKSIZ) goto L_eom2;
    goto L_next_inPoint;
  }

  // Abweichung tolerierbar. pte raus.
  pTab[ptOut] = pte; // copy Point
  ++ptOut;
  if(ptOut >= ptMax) goto L_eom1;
  --ptStackNr;
  pts = pte;
  us = ue;
  if(ptStackNr > 0) goto L_next_inPoint;

  irc = 0;

  //-------------------------------------------
  L_fertig:

  *ptNr = ptOut;

    // TESTBLOCK
    // printf("ex CVPSP_pol_psp ptNr=%d\n",*ptNr);
    // for(i1=0;i1<*ptNr;++i1) printf(" pTab[%d] %12.3f %12.3f %12.3f\n",i1,
        // pTab[i1].x,pTab[i1].y,pTab[i1].z);
    // GR_tDyn_npt__ (pTab, *ptNr, 0);
    // GL_set_pcv (ptOut, pTab);
    // DEB_exit();
    // END TESTBLOCK

  return irc;


  //-------------------------------------------
  L_eom1:
    irc = -1;
    TX_Error("CVPSP_pol_nPsp3 EOM-1");
    goto L_fertig;

  L_eom2:
    irc = -1;
    TX_Error("CVPSP_pol_nPsp3 EOM-2");
    goto L_fertig;

}


/*
//=====================================================================
  double CVPSP_dist1 (Point *pts, Point *pt1, Point *pte) {
//=====================================================================
// CVPSP_dist1      normal-distance point  line
// Normalabst von pt1 von Linie pts-pte

  double d1;
  Point  pti;
  Vector vc1;


  // printf("CVPSP_dist1\n");


  UT3D_vc_2pt (&vc1, pts, pte);


  UT3D_pt_projptptvc (&pti, &d1, NULL, pt1, pts, &vc1);
  // d1 = UT3D_len_2pt (&pti, pt1);


  // printf(" ex CVPSP_dist1 %f\n",d1);

  return d1;

}

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
    CVPSP_ioff_psp (&is, &ps, pu, PolCo, PolCoNr);

    // den Punkt dazu holen
    CVPSP_pt_psp (&pt1, ps, &PolCo[is]);

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
  int CVPSP_ioff_psp (int *is, double *dus, double du,
                       polynom_d3 *polTab, int polNr) {
//===========================================================================
// CVPSP_ioff_psp      get offset for Polynom.Spline
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

  // printf("CVPSP_ioff_psp du=%f d_tot=%f d_m=%f\n",du,d_tot,d_m);


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


  // printf("ex CVPSP_ioff_psp is=%d du1=%f du=%f %f-%f\n",*is,*dus,du,
                            // polTab[*is].u,polTab[(*is)+1].u);

  return 0;

}


//================================================================
  int CVPSP_box_oPsp3 (Point *pb1, Point *pb2, CurvPsp3 *cv1) {
//================================================================
// CVPSP_box_oPsp3    extend box for Polynom.Spline

  int        i1, iNr;
  polynom_d3 *plyTab;
  Point      pt1;

  // printf("CVPSP_box_oPsp3 \n");
  // DEB_dump_obj__ (Typ_CVPSP3, cv1, "CVPSP_box_psp");


  plyTab = cv1->plyTab;
  iNr  = cv1->plyNr; // + 1;

  for(i1=0;i1<iNr; ++i1) {
    pt1.x = plyTab[i1].x.a;
    pt1.y = plyTab[i1].y.a;
    pt1.z = plyTab[i1].z.a;
    UT3D_box_extend (pb1, pb2, &pt1);
      // printf(" seg %d %f %f %f\n",i1,pt1.x,pt1.y,pt1.z);
  }

  // printf("ex-CVPSP_box_oPsp3 \n");

  return 0;

}


//===========================================================================
  int CVPSP_pt_psp (Point *pt1, double du, polynom_d3 *pc) {
//===========================================================================
// CVPSP_pt_psp           compute point from offset on Polynom.Spline
// 
// IN:
//   du          ... der Abstand (0. - 1.)
//   pc          ... polynomCoefficient for a degree-3 polynom.
//     pc.x      ... coefficients of the X-polynomial (a-d)
//     pc.y      ... coefficients of the Y-polynomial (a-d)
//     pc.z      ... coefficients of the Z-polynomial (a-d)
// 
// OUT:
//   der 3D-Punkt pt1
// 


  double s, ss, sss;


  // printf("CVPSP_pt_psp du=%f uSeg=%f\n",du,pc->u);
  // printf("          %f %f %f %f\n",pc->x.a,pc->x.b,pc->x.c,pc->x.d);

  s   = du;
  ss  = s*s;
  sss = ss*s;

  pt1->x = pc->x.a + pc->x.b*s + pc->x.c*ss + pc->x.d*sss;
  pt1->y = pc->y.a + pc->y.b*s + pc->y.c*ss + pc->y.d*sss;
  pt1->z = pc->z.a + pc->z.b*s + pc->z.c*ss + pc->z.d*sss;

  // printf("ex CVPSP_pt_psp %f,%f,%f du=%f\n",pt1->x,pt1->y,pt1->z,du);

  return 0;
}




//=========== EOF ========================
