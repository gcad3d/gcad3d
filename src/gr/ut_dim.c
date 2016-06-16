// GraficDimensions-Utilities.        Franz Reiter     2010-09-30
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
\file  ../gr/ut_dim.c
\brief GraficDimensions-Utilities. 
\code
=====================================================
List_functions_start:

GR_dim_lin__          ?
GR_dim_dmr__          ?
GR_dim_rad__          ?
GR_dim_ang__          ?

List_functions_end:
=====================================================
see also dxfw_gxt

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
#include "../ut/ut_TX.h"
#include "../ut/func_types.h"                 // Typ_Att_PT, SYM_TRI_S, ..

#include "../db/ut_DB.h"      // DB_GetVar

#include "../gr/ut_gr.h"      // eigene Prototypen


// ======= Externe Variablen: ==========
// ex ut_gtx.c:
extern double GR_tx_ldExt;            // Ueberlaenge der Leaderlines
extern double GR_tx_chh;              // Characterheight (incl Abstand)
extern int    GR_tx_nkNr;             // Anzahl Nachkommastellen beim Text


// aus xa.c
extern double     AP_txdimsiz;    // Dimensions-Text-size






//=======================================================================
  int GR_dim_lin__ (Line2 *ll1, Line2 *ll2, Line2 *lnd,
                    SymRef2 *hd1, SymRef2 *hd2, Vector2 *vct,
                    double *dval, char *outText,
                    Dimen *dim1) {
//=======================================================================
// create objects for linear dimension.
// components of dimension:
// Output:
//   2 leaderlines (ll1, ll2);
//   1 dimensionLine (lnd);
//   2 symbols at the ends of the dimensionLine (hd1, hd2);
//   direction of symbols and text (vct);
//   value of the dimemsion (distanceValue; dval).
//   the decoded text (outText)
//   RetCod:  BlockColor; 0=keine, 1-9 ist Farbe und wird als Block dargestellt.

// see GL_DrawDimen SVG_w_dim__
 
  int        i12, i13, i23, bMod;
  double     txAng, ldAng, d12, dtx;
  char       dimText[256];
  Point2     *pt1, *pt2, *pt3;
  Vector2    vcl;


  // printf("GR_dim_lin__ \n");

  pt1 = (Point2*)&dim1->p1;
  pt2 = (Point2*)&dim1->p2;
  pt3 = (Point2*)&dim1->p3;


  txAng = dim1->a1;               // Winkel der Textzeile
  ldAng = txAng - 90.;            // Winkel der Leaderlines
    // printf(" txAng=%f ldAng=%f\n",txAng,ldAng);

  // Richtung Leaderline:     vcl
  UT2D_vc_angr (&vcl, UT_RADIANS(ldAng));

  // Richtung Text u. Arrows: vct
  UT2D_vc_angr (vct, UT_RADIANS(txAng));
    
  // Leaderlines.Den pt3 auf pt1/pt2-vcl projizieren.
  UT2D_pt_projptptvc (&hd1->pt, pt3, pt1, &vcl);
  UT2D_pt_projptptvc (&hd2->pt, pt3, pt2, &vcl);


  // compute distanceValue
  *dval = UT2D_len_2pt (&hd1->pt, &hd2->pt);
    // printf(" dval = %f\n",*dval);


  // create/decode text; value=dVal
  GR_gxt_prep1 (dimText, *dval);
    // printf(" tx1: |%s|\n",dimText);

  // substitute [% by dimText
  bMod = GR_gxt_subst1 (outText, dim1->txt, dimText);
    // printf(" tx2: |%s|\n",outText);



  // Leaderlines um Texthoehe verlaengern.
  // wenn pt3a oben ist: Abstand umdrehen !
  ll1->p1 = UT2D_pt_pt3 (pt1);
  if(UT2D_sidPerp_ptvc (&hd1->pt, pt1, &vcl)> 0) {
    d12 = GR_tx_ldExt;
  } else {
    d12 = -GR_tx_ldExt;
  }
  UT2D_pt_traptvclen (&ll1->p2, &hd1->pt, &vcl, d12);



  // ebenso fuer ll2:
  ll2->p1 = UT2D_pt_pt3 (pt2);
  if(UT2D_sidPerp_ptvc (&hd2->pt, pt2, &vcl)> 0) {
    d12 = GR_tx_ldExt;
  } else {
    d12 = -GR_tx_ldExt;
  }
  UT2D_pt_traptvclen (&ll2->p2, &hd2->pt, &vcl, d12);



  // get Headtypes
  hd2->typ = dim1->hd % 10;
  // *sTyp1 = (dim1->hd - *sTyp2) / 10;
  hd1->typ = dim1->hd  / 10;
    // printf("Headtypes = %d,%d\n", hd1->typ, hd2->typ);



  // test if text is outside of the leaderlines;
  // if yes: if Headtypes not given: fix Headtypes
  //    the dimensionLine starts at p3; extend with half blocksize
  i13 = UT2D_sidPerp_ptvc (&hd1->pt, pt3, vct); // s1 rechts od links von pt3
  i23 = UT2D_sidPerp_ptvc (&hd2->pt, pt3, vct); // s2 rechts od links von pt3
    // printf(" i13 %d i23 %d\n",i13,i23);


  
  if((i13 + i23) == 0) {  // different: text is inside !
    lnd->p1 = hd1->pt;
    lnd->p2 = hd2->pt;
    // test if ll2 is right of ll1; if not: change symboltypes !
    // d12 = UT2D_len_ptlnX(&lnd->p1, &lnd->p2, vct);
    if(i23 < 0) {
      MEM_swap_int (&hd1->typ, &hd2->typ);
      // printf(" SWAP SYMBOLTYPES !\n");
    }
    goto L_9;
  }


  // Text ist aussen: p3 um halbe Blockbreite nach aussen schieben
  lnd->p1 = hd1->pt;
  lnd->p2 = hd2->pt;
  i12 = UT2D_sidPerp_ptvc (&hd1->pt, &hd2->pt, vct); // s1 rechts od links von s2
    // printf(" dimText outside %d %d\n",i13,i23);
  d12 = GR_gtx_ckBlockWidth (outText, AP_txdimsiz) / 2.;
    // printf(" d12=%f\n",d12);


  if(i12 < 0) {        //   1 2
    MEM_swap_int (&hd1->typ, &hd2->typ);
    if(i13 > 0) {      // 3 1 2
      UT2D_pt_traptvclen (&lnd->p1, pt3, vct, -d12);
    } else {           //   1 2 3
      UT2D_pt_traptvclen (&lnd->p2, pt3, vct, d12);
    }

  } else {             //   2 1
    if(i13 > 0) {      // 3 2 1
      UT2D_pt_traptvclen (&lnd->p2, pt3, vct, -d12);
    } else {           //   2 1 3
      UT2D_pt_traptvclen (&lnd->p1, pt3, vct, d12);
    }


  }


  L_9:
  return bMod;

}


//=======================================================================
  int GR_dim_dia__ (Line2 *lnd, SymRef2 *hd1, SymRef2 *hd2,
                    Vector2 *vct, double *atx,
                    double *dval, char *outText,
                    Dimen *dim1) {
//=======================================================================
// create objects for diameter-dimension.
// components of dimension:
// Output:
//   1 dimensionLine (lnd);
//   2 symbols at the ends of the dimensionLine (hd1, hd2);
//   direction of symbols and text (vct) as vector;
//   direction of symbols and text (atx) as angle;
//   value of the dimemsion (distanceValue; dval).
//   the decoded text (outText)
//   RetCod:  BlockColor; 0=keine, 1-9 ist Farbe und wird als Block dargestellt.

// see GL_DrawDimen

  double     d1, rdc;
  char       dimText[256];
  Point2     *pt1, *pt2, *pt3;


  // printf("GR_dim_dia__ \n");

  pt1 = (Point2*)&dim1->p1;     // pc = center of circ
  pt2 = (Point2*)&dim1->p2;     // point on circ on line pc-pt
  pt3 = (Point2*)&dim1->p3;     // pt - textposition (center)
  *atx = dim1->a1;               // angle of dimensionLine & text
  rdc = dim1->a2;               // radius of circ

  // inside-config: pc-pt-



  // den Text generieren;
  strcpy(dimText, "[d");  // Dmr-zeichen
  d1 = 2. * rdc;
  UTX_add_fl_f (dimText, d1, GR_tx_nkNr);
  UTX_del_foll0 (dimText);

  GR_gxt_subst1 (outText, dim1->txt, dimText);


  // Pfeil: (Typ=dim1->hd, Pos=dim1->p2 und p2o), Richtg=vc1
  hd2->typ = dim1->hd % 10;
  hd1->typ = dim1->hd / 10;


  // p2o = opposit pt am circ
  hd1->pt = *pt2;
  UT2D_pt_opp2pt (&hd2->pt, pt1, pt2);
  UT2D_vc_2pt (vct, &hd2->pt, pt2);


  // Maszlinie: p3a-p3e
  // innen od aussen ?
  d1 = UT2D_len_2pt (pt1, pt3);

  // innen:
  if(d1 < rdc) {
    lnd->p1 = hd2->pt;
    lnd->p2 = *pt2;
    MEM_swap_int (&hd1->typ, &hd2->typ);

  // aussen: 
  } else {
    lnd->p1 = hd2->pt;
    // p3e um halbe Blockbreite nach aussen schieben
    d1 = GR_gtx_ckBlockWidth (outText, AP_txdimsiz) / 2.;
    UT2D_pt_tra2ptlen (&lnd->p2, pt3, pt1, -d1);
  }


  // fix txAng; Masztext von -45-135 Grad aussen, else Innen.
  // if((txAng > RAD_135)&&(txAng < (RAD_360-RAD_45))) {
  if((*atx > 135.)&&(*atx < 315.)) {
    // printf(" >>>>>> Masztext auf die andere Seite <<<<<<<<<<<<\n");
    // txAng -= RAD_180;
    *atx -= 180.;
    UT2D_vc_invert (vct, vct);
    MEM_swap_int (&hd1->typ, &hd2->typ);
  }


  return 0;

}




//=======================================================================
  int GR_dim_rad__ (Line2 *lnd, SymRef2 *hd1,
                    Vector2 *vct, double *atx,
                    double *dval, char *outText,
                    Dimen *dim1) {
//=======================================================================
// create objects for diameter-dimension.
// components of dimension:
// Output:
//   1 dimensionLine (lnd);
//   1 symbol at the ends of the dimensionLine (hd1);
//   direction of symbols and text (vct) as vector;
//   direction of symbols and text (atx) as angle;
//   value of the dimemsion (distanceValue; dval).
//   the decoded text (outText)
//   RetCod:  BlockColor; 0=keine, 1-9 ist Farbe und wird als Block dargestellt.

// see GL_DrawDimrad

  int        i12;
  double     d1, d2, rdc;
  char       dimText[256];
  Point2     *pt1, *pt2, *pt3;


  // printf("GR_dim_rad__ \n");

  pt1 = (Point2*)&dim1->p1;     // pc = center of circ
  pt2 = (Point2*)&dim1->p2;     // point on circ on line pc-pt
  pt3 = (Point2*)&dim1->p3;     // pt - textposition (center)
  *atx = dim1->a1;               // angle of dimensionLine & text
  rdc = dim1->a2;               // radius of circ


  // den Text generieren; dmr f oder R
  strcpy(dimText, "R");  // Dmr-zeichen
  d1 = rdc;
  UTX_add_fl_f (dimText, d1, GR_tx_nkNr);
  UTX_del_foll0 (dimText);

  GR_gxt_subst1 (outText, dim1->txt, dimText);


  // Pfeil: (Typ=dim1->hd, Pos=dim1->p2), Richtg=unused
  hd1->typ = dim1->hd;
  hd1->pt  = *pt2;


  UT2D_vc_2pt (vct, pt2, pt3);   // vct= 2 -> 3 passend f aussen (1 2 3)
  i12 = 1;


  // fix txAng; Masztext von -45-135 Grad aussen, else Innen.
  if((*atx > 135.)&&(*atx < 315.)) {
    // printf(" >>>>>> Masztext auf die andere Seite <<<<<<<<<<<<\n");
    *atx -= 180.;
    // Richtung Text. darf NICHT nach links zeigen !
    i12 = -1;
  }



  // Maszlinie: p3a-p3e
  lnd->p1 = *pt2;


  // innen od aussen ?
  d1 = UT2D_len_2pt (pt1, pt3);
    // printf(" d1=%f i12=%d\n",d1,i12);

  // halbe Blockbreite nach aussen schieben
  d2 = GR_gtx_ckBlockWidth (outText, AP_txdimsiz) / 2.;

  if(d1 < rdc) {           // inside;  1 3 2   oder 2 3 1
    if(i12 < 0) {          // 1 3 2
    } else {               // 2 3 1
      GR_dim_invHd1 (&hd1->typ);
      // if(hd1->typ == 1)      hd1->typ = 2;
      // else if(hd1->typ == 2) hd1->typ = 1;
      UT2D_vc_invert (vct, vct);
    }
    
  } else {               // outside; 1 2 3   oder 3 2 1
    if(i12 < 0) {          // 1 2 3
      GR_dim_invHd1 (&hd1->typ);
      // if(hd1->typ == 1)      hd1->typ = 2;
      // else if(hd1->typ == 2) hd1->typ = 1;
      d2 *=-1;
      UT2D_vc_invert (vct, vct);
    } else {               // 3 2 1
      d2 *=-1;
    }

  }

  UT2D_pt_tra2ptlen (&lnd->p2, pt3, pt1, d2);


  return 0;

}


//=======================================================================
  int GR_dim_ang__ (Line2 *ll1, Line2 *ll2, Circ2 *acd,
                    SymRef2 *hd1, SymRef2 *hd2,
                    Vector2 *vct, double *atx,
                    double *dval, char *outText,
                    Dimen *dim1) {
//=======================================================================
// create objects for linear dimension.
// components of dimension:
// Output:
//   2 leaderlines (ll1, ll2);
//   1 dimensionArc (acd);
//   2 symbols at the ends of the dimensionLine (hd1, hd2);
//   direction of symbols and text (vct);
//   value of the dimemsion (distanceValue; dval).
//   the decoded text (outText)
//   RetCod:  BlockColor; 0=keine, 1-9 ist Farbe und wird als Block dargestellt.

// see GL_DrawDima SVG_w_dim__

  int        i1;
  double     a1, a2, a3, a12, a13, a23, d0, d1, d2, d3;
  char       dimText[64];
  Point2     *pt1, *pt2, *pt3;
  Point2     ps;
  Vector2    vc1, vc2;


  // printf("GR_dim_ang__ ----------------------------\n");
  // UT3D_stru_dump (Typ_Dimen, dim1, "");

  pt1 = (Point2*)&dim1->p1;
  pt2 = (Point2*)&dim1->p2;
  pt3 = (Point2*)&dim1->p3;
    // GR_Disp_pt2 (pt1, SYM_TRI_S, 3);
    // GR_Disp_pt2 (pt2, SYM_TRI_S, 3);
    // GR_Disp_pt2 (pt3, SYM_TRI_S, 3);
    // UT3D_stru_dump (Typ_PT2, pt1, "pt1");
    // UT3D_stru_dump (Typ_PT2, pt2, "pt2");
    // UT3D_stru_dump (Typ_PT2, pt3, "pt3");


  // ps = den Schnittpunkt der beiden Hilfslinien errechnen
  a1 = dim1->a1;
  a2 = dim1->a2;
  UT2D_vc_angr (&vc1, a1);
  UT2D_vc_angr (&vc2, a2);
  UT2D_pt_int2pt2vc (&ps, pt1, &vc1, pt2, &vc2);
    // GR_Disp_pt2 (&ps, SYM_STAR_S, 2);


  // der Abstand des p3 vom Schnittpunkt ps ergibt die Pos. der Heads
  d3 = UT2D_len_2pt (&ps, pt3);
  UT2D_pt_traptvclen (&hd1->pt, &ps, &vc1, d3);
    // GR_Disp_pt2 (&hd1->pt, SYM_STAR_S, 2);

  UT2D_pt_traptvclen (&hd2->pt, &ps, &vc2, d3);
    // GR_Disp_pt2 (&hd2->pt, SYM_STAR_S, 2);


  // Maszpfeiltypen
  hd2->typ = dim1->hd % 10;
  hd1->typ = dim1->hd / 10;
    // printf("Headtypes = %d,%d\n", hd1->typ, hd2->typ);

  // Maszpfeilwinkel
  // hd1->ang = UT_DEGREES (UT2D_angr_perpangr (&a1));
  // hd2->ang = UT_DEGREES (UT2D_angr_perpangr (&a2));
  hd1->ang = UT2D_angr_perpangr (&a1);
  hd2->ang = UT2D_angr_perpangr (&a2);


  // Leaderlines um GR_tx_ldExt verlaengern.
  ll1->p1 = *pt1;
  // test ob pt1 vor oder hinter ps ist
  i1 = UT2D_sidPerp_ptvc (pt1, &hd1->pt, &vc1);
  if(i1 < 0) d0 = GR_tx_ldExt;
  else       d0 = -GR_tx_ldExt;
  UT2D_pt_traptvclen (&ll1->p2, &hd1->pt, &vc1, d0);
    // GR_Disp_ln2 (&ll1->p1, &ll1->p2, 8);


  ll2->p1 = *pt2;
  i1 = UT2D_sidPerp_ptvc (pt2, &hd2->pt, &vc2);
  if(i1 < 0) d0 = GR_tx_ldExt;
  else       d0 = -GR_tx_ldExt;
  UT2D_pt_traptvclen (&ll2->p2, &hd2->pt, &vc2, d0);
    // GR_Disp_ln2 (&ll2->p1, &ll2->p2, 9);



  // die Normale auf ps-p3 ergibt den TextWinkel txAng
  a3 = UT2D_angr_ptpt (&ps, pt3);
  d1 = UT2D_angr_perpangr (&a3);          // txAng = a3 + 90 deg
  UT2D_vc_angr (vct, d1);
  *atx = UT_DEGREES (d1);
    // printf(" a1=%f a2=%f a3=%f atx=%f\n",a1,a2,a3,*atx);


  // fix txAng; Masztext von 45-225 Grad aussen, else Innen.
  if((*atx > 135.)&&(*atx < 315.)) {
    // printf(" >>>>>> Masztext auf die andere Seite <<<<<<<<<<<<\n");
    *atx -= 180.;
    UT2D_vc_invert (vct, vct);
  }


  // liegt Textblock zwischen den Hilfslinien oder ausserhalb ?
  a12 = UT2D_angr_2angr (a1, a2, 1);       // ango p1-p2 (gesamt)
  a13 = UT2D_angr_2angr (a1, a3, 1);       // ango p1-p3
    // printf(" a12=%f a13=%f\n",a12,a13);

  acd->pc = ps;
  acd->rad = d3;

  if(a13 < a12) {                          // dazwischen; 1 3 2 od. 2 3 1
      // printf(" zwischen\n");

    acd->p1 = hd1->pt;
    acd->p2 = hd2->pt;
    acd->ango = a12;
    // if(a2 < a1) {                         // 2 3 1
      // UT2D_vc_invert (vct, vct);
    // }

  } else {                                 // 3 1 2    od 1 2 3
    MEM_swap_int (&hd1->typ, &hd2->typ);   // swap Maszpfeiltypen
    // d1 = RAD_360 - d1; // nun CW
    a23 = UT2D_angr_2angr (a2, a3, 1);
      // printf("a23=%f\n",a23);

    if(a23 < RAD_180) {      // p3 bei p2        1 2 3
        // printf(" aussen 123\n");
      acd->p1 = hd1->pt;
      acd->p2 = *pt3;
      acd->ango = a12 + a23;

    } else {           // p3 bei p1        3 1 2
        // printf(" aussen 312\n");
      acd->p1 = *pt3;
      acd->p2 = hd2->pt;
      acd->ango = RAD_360 - a23;
    }
  }
    // printf(" ango=%f \n",acd->ango);


  // den Text generieren
  GR_gxt_prep1 (dimText, UT_DEGREES(a12)); // dimText = a12
  strcat(dimText, "[g");


  // substitute [% by dimText
  GR_gxt_subst1 (outText, dim1->txt, dimText);



  // den winkel um die halbe Blocksize vergroessern ..


  return 0;

}


//=======================================================================
  int GR_dim_ldr__ (Line2 *ll1, Line2 *ll2, SymRef2 *hd1, Point2 *ptx,
                    Vector2 *vct, double *atx,
                    char *outText,
                    Dimen *dim1) {
//=======================================================================
// create objects for linear dimension.
// components of dimension:
// Output:
//   2 leaderlines (ll1, ll2);
//   1 symbol
//   direction of text (vct and atx);
//   the decoded text (outText)
//   RetCod:  BlockColor; 0=keine, 1-9 ist Farbe und wird als Block dargestellt.

// see GL_DrawLdr SVG_w_dim__


  Point2     *pt1, *pt2, *pt3;



  pt1 = (Point2*)&dim1->p1;
  pt2 = (Point2*)&dim1->p2;
  pt3 = (Point2*)&dim1->p3;

  // den Text generieren
  GR_gxt_subst1 (outText, dim1->txt, NULL);
  
  
  // angle of head
  hd1->typ = dim1->hd % 10;
  hd1->pt  = *pt1;
  hd1->ang = UT2D_angr_ptpt (pt1, pt2);
  

  // Linie p1-p2
  ll1->p1 = *pt1;
  ll1->p2 = *pt2;
    
    
  // Linie p2-p3
  if(dim1->p3.x != UT_DB_LEER) {  // es gibt einen p3
    *ptx = *pt3;
    ll2->p1 = *pt2;
    ll2->p2 = *pt3;
    // liegt p2 links oder rechts von p3;
    // wenn rechts: den p3 um TextBlockBreite nach rechts schieben
    if(dim1->p2.x > dim1->p3.x) {
      ll2->p2.x += GR_gtx_ckBlockWidth (outText, AP_txdimsiz);
    }

  } else {
    *ptx = *pt2;
  }

  // angle of text:
  *atx = dim1->a1;      // angle of text
  UT2D_vc_2pt (vct, pt1, pt2);



  // fix txAng; Masztext von -45-135 Grad aussen, else Innen.
  // if((txAng > RAD_135)&&(txAng < (RAD_360-RAD_45))) {
  if((*atx > 135.)&&(*atx < 315.)) {
    // printf(" >>>>>> Masztext auf die andere Seite <<<<<<<<<<<<\n");
    // txAng -= RAD_180;
    *atx -= 180.;
    UT2D_vc_invert (vct, vct);
  }


  return 0;

}


//================================================================
  int GR_dim_invHd1 (int *hdTyp) {
//================================================================
// change 1 <-> 2

  if(*hdTyp == 1)      *hdTyp = 2;
  else if(*hdTyp == 2) *hdTyp = 1;

  return 0;

}


// EOF
