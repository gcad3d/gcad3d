//   GraficText-Utilities.        Franz Reiter     2004-11-11
/*
 *
 * Copyright (C) 2015 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
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
\file  ../gr/ut_gtx.c
\brief graphic text functions 
\code
=====================================================
List_functions_start:

GR_InitGFPar          Init Graf.Fontparameters
GTX_scale             returns total scale
GTX_siz__             returns scale from charWidth
GTX_chw_              returns (charWidth + spcWidth) scaled
GTX_cw_               returns (charWidth + spcWidth)
GTX_chh_              returns charHeight scaled
GTX_chhl2             returns (charHeight + (interlinespace/2)) scaled
GTX_ardxy             returns arrowSize in x & y
GR_gtx_ckBlockWidth   Breite eines Textblocks errechnen
GR_gxt_subst1         substitute [% by text
GR_gxt_prep1          prepare dimText
GR_gxt_prep_Pt        prepare dimText
GR_gxt_nkNr           extract nkNr
GR_gxt_dxfin          DXF => gCad
GR_gxt_dxfout         gCad => DXF

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
#include "../ut/ut_TX.h"

#include "../db/ut_DB.h"      // DB_GetVar


#include "../gr/ut_gr.h"      // eigene Prototypen
#include "../gr/vf.h"         // GL_vf1_CW ..




/*=============== Externe Variablen: =======================================*/
// ex xa.c:
extern double     AP_txsiz;             // Notes-Defaultsize
extern double     AP_txdimsiz;          // Dimensions-Text-size
extern int        AP_txNkNr;            // Nachkommastellen




/*=============== Lokale Variablen: =======================================*/
double GR_tx_scale = 1.;       // Scalefaktor Text (was txSiz)
double GR_tx_ldExt;            // Ueberlaenge der Leaderlines
double GR_tx_ardx;             // Size Arrowhead in X-Dir
double GR_tx_ardy;             // Size Arrowhead in Y-Dir
// double GR_tx_ardp;             // Size Arrowhead in Y-Dir
double GR_tx_chw;              // Characterwidth (incl Abstand)
double GR_tx_chh;              // Characterheight (incl Abstand)

int    GR_tx_nkNr;             // Anzahl Nachkommastellen beim Text

double GR_fontSiz = 0.055;     // bringt den aktuellen Font auf Site 1 mm.



// Prototypen in ut_gr.h



//=================================================================
  int GR_gxt_nkNr (char *fmt) {
//=================================================================
// GR_gxt_nkNr           extract nkNr
// [0 bis [9 sind die Anzahl Nachkommastellen
// Default ist GR_tx_nkNr

  int irc;

  irc = GR_tx_nkNr;         // Default

  // nkNr muss ganz am Beginn stehen:
  if(fmt == NULL) goto L_def;
  if(fmt[0] == '[') {
    if((fmt[1] >= '0')&&(fmt[1] <= '9')) {
      irc = fmt[1] - '0';
      // fmt += 2;
    }
  }

  L_def:
  return irc;

}


//=================================================================
  int GR_InitGFPar (double txdimsiz) {
//=================================================================
// Graf.FontParameter setzen
// Schrifthoehe soll 2.5 mm sein; GR_tx_scale entspr setzen.

  // double fontSiz = 0.055;


  // printf("GR_InitGFPar txdimsiz=%f GR_tx_scale=%f\n",txdimsiz,GR_tx_scale);
  // printf(" AP_txdimsiz=%f\n",AP_txdimsiz);

/*
  GR_tx_scale =  0.125;   // Scalefaktor Text (was txSiz)
  GR_tx_ldExt =  2.6;     // Ueberlaenge der Leaderlines
  GR_tx_ardx  =  2.6;     // Size Arrowhead in X-Dir
  GR_tx_ardy  =  0.6;     // Size Arrowhead in Y-Dir
  GR_tx_ardp  =  1.0;     // Size Arrowhead in Y-Dir
*/

  // fuer Inch: siz=0.2; fuer mm: siz=2.5
  // GR_tx_scale = fontSiz * txdimsiz;
  // GR_tx_scale = fontSiz;

  // Size Arrowhead in X-Dir
  GR_tx_ardx  = GR_tx_scale * GR_fontSiz * txdimsiz * 20.;

  GR_tx_ardy  = GR_tx_ardx * 0.25; // Size Arrowhead in Y-Dir
  // GR_tx_ardp  = GR_tx_ardx * 0.4;  // Size Arrowhead in Y-Dir??
  GR_tx_ldExt = GR_tx_ardx;


  GR_tx_chw   = GL_vf1_CW + GL_vf1_CS; // Characterwidth (incl Abst.) was xDist
  GR_tx_chh   = GL_vf1_CH + GL_vf1_LS; // Characterheigth (incl Abst.) was xDist

  GR_tx_nkNr  =  AP_txNkNr;            // Anzahl Nachkommastellen beim Text

  return 0;

}


//================================================================
  double GTX_scale (double size) {
//================================================================
// returns total scale

  return GR_tx_scale * GR_fontSiz * size;

}



//================================================================
  double GTX_siz__ (double cw1) {
//================================================================
// liefert scale; Input ist Breite eines chars incl intercharSpc.

  double siz;

  siz = cw1 / (GR_tx_chw * GR_tx_scale * GR_fontSiz);

  printf("ex GTX_siz__ %f %f\n",siz,cw1);

  return siz;

}


//================================================================
  double GTX_chw_ (double size) {
//================================================================
// Breite character + intercharspace
// #define GTX_chw_ (scale) (GR_tx_scale * (scale))

  // printf("GTX_chw_ %f\n",size);
  // printf("  GR_tx_chw=%f scale=%f fontSiz=%f\n",
    // GR_tx_chw,GR_tx_scale,GR_fontSiz);
  // printf("ex GTX_chw_ %f\n",GR_tx_chw * GR_tx_scale * GR_fontSiz * size);


  return GR_tx_chw * GR_tx_scale * GR_fontSiz * size;

}


//================================================================
  double GTX_cw_ () {
//================================================================
/// Breite character + intercharspace

// GTX_chw_ liefert manchmal zu kleinen Wert ... ???

  return GR_tx_chw;

}



//================================================================
  double GTX_chh_ (double size) {
//================================================================
/// \code
/// Hoehe character
/// Example:
///   GTX_chh_ (AP_txdimsiz);
/// \endcode

// #define GTX_chhl2 (scale)((GL_vf1_CH+(GL_vf1_LS/2.))*GR_tx_scale*(scale))

  return GL_vf1_CH * GR_tx_scale * GR_fontSiz * size;

}


//================================================================
  double GTX_chhl2 (double size) {
//================================================================
/// Hoehe character + halber interlinespace
// #define GTX_chhl2 (scale)((GL_vf1_CH+(GL_vf1_LS/2.))*GR_tx_scale*(scale))

  return (GL_vf1_CH+(GL_vf1_LS/2.)) * GR_tx_scale * GR_fontSiz * size;

}


//================================================================
  int GTX_ardxy (double *ardx, double *ardy) {
//================================================================
/// GTX_ardxy             returns arrowSize in x & y


  *ardx = GR_tx_ardx;
  *ardy = GR_tx_ardy;

  return 0;

}


//================================================================
  double GR_gtx_ckBlockWidth (char *txt, double size) {
//================================================================
/// Breite eines Textblocks in Usercoords errechnen

// sollte nach dem subst [% erfolgen !!

 
  int    i1;
  double dx;


  i1 = strlen(txt) - UTX_cnr_chr (txt, '[');

  // dx = i1 * GR_tx_chw * GR_tx_scale * size;
  dx = i1 * GTX_chw_ (size);

  return dx;

}


//================================================================
  int GR_gxt_subst1 (char *txo, char *fmt, char *tx1) {
//================================================================
// GR_gxt_subst1         substitute [% by text
//
// [%   wird durch Masztext ersetzt. Wenn im Text kein [% enthalten ist,
//      wird der Masztext ganz an den Textbeginn gestellt.
// [-   keinen Masztext einfuegen.
// [V#] durch numer. Variable ersetzen
//
// fmt: "Hor. [% mm"
// tx1: "55"
// txo: "Hor. 55 mm"
// Retcod: BlockColor; 0=keine, 1-9 ist Farbe und wird als Block dargestellt.

  int    irc, i1, i2, i3, ilen;
  long   dbi;
  double d1;
  char   c1, *p1;


  // printf("GR_gxt_subst1 |%s|%s|\n",fmt,tx1);

  txo[0] = '\0';
  irc = 0;


  // BlockColor muss ganz am Beginn stehen:
  // see GR_gxt_nkNr
  if(fmt == NULL) goto L_add_tx1;
  if(fmt[0] == '[') {
    if((fmt[1] >= '0')&&(fmt[1] <= '9')) {
      irc = fmt[1] - '0';
      fmt += 2;
    }
  }


  // test if to start with tx1
  // wenn fmt kein "[%" und kein "[-" enthaelt: tx1 ganz an den Beginn.
  // L_start:
  if(strstr(fmt, "[-") != NULL) goto L_add_fmt;
  if(strstr(fmt, "[%") != NULL) goto L_add_fmt;



  // start with txt1
  L_add_tx1:
  if(tx1 == NULL) goto L_add_fmt;
  strcat(txo, tx1);


  // add fmt;  exclude "[-"; substitute "[%" by tx1
  L_add_fmt:
  if(fmt == NULL) goto L_exit;

  i1   = 0;
  ilen = strlen(fmt);


  //----------------------------------------------------------------
  NextChar:
  c1 = fmt[i1];
  // printf(" next char i1=%d c1=%d/%c\n",i1,c1,c1);

  if(c1 == '[') {                 // SONDERZEICHEN
    ++i1;
    if(i1 >= ilen) goto L_parErr;
    c1 = fmt[i1];

    if(c1 == '-') {                   // skip
      ++i1;
      goto NextChar;

    } else if(c1 == '%') {            // substitute
      ++i1;
      strcat(txo, tx1);
      goto NextChar;

    } else if(c1 == 'V') {            // expand parameter
      irc = APED_dbo_oid (&i2, &dbi, &fmt[i1]);
      if(irc < 0) goto L_parErr;
      p1 = strchr (&fmt[i1], ']');
      if(p1 == NULL) goto L_parErr;
      d1 = DB_GetVar (dbi);
      // printf(" V[%d]=%f\n",dbi,d1);
      UTX_add_fl_u (txo, d1);
      i1 += p1 - &fmt[i1];
      ++i1; // skip "]"
      goto NextChar;

    } else {                          // copy
      strcat(txo, "[");
    }
  }
  strncat(txo, &c1, 1);
  ++i1;
  if(i1 < ilen)  goto NextChar;


  L_exit:
  // printf("ex GR_gxt_subst1 |%s| %d\n",txo,irc);
  return irc;


  L_parErr:
  TX_Error("GR_gxt_subst1 E001");
  return -1;

}


//================================================================
  int GR_gxt_prep_Pt (char *txo, Point *pt, int nkz) {
//================================================================
// GR_gxt_prep1          prepare dimText
// print formatted Point pt --> txo


  if(nkz < 0) nkz = GR_tx_nkNr;

  txo[0] = '\0';
  UTX_add_fl_f (txo, pt->x, nkz);
  UTX_del_foll0 (txo);
  strcat (txo, " ");

  UTX_add_fl_f (txo, pt->y, nkz);
  UTX_del_foll0 (txo);
  strcat (txo, " ");

  UTX_add_fl_f (txo, pt->z, nkz);
  UTX_del_foll0 (txo);

  return 0;

}


//================================================================
  int GR_gxt_prep1 (char *txo, double d1) {
//================================================================
// GR_gxt_prep1          prepare dimText


  txo[0] = '\0';
  UTX_add_fl_f (txo, d1, GR_tx_nkNr);
  UTX_del_foll0 (txo);

  return 0;

}


//================================================================
  int GR_gxt_dxfout (int mode, char *txo, char *txi) {
//================================================================
// GR_gxt_dxfout          gCad => DXF
// mode=0: nur Notes; kein masztext
// mode=1: Notes plus masztext

  int  i1, ilen;
  char c1;

  printf("GR_gxt_dxfout |%s|\n",txi);
  // strcpy(txo, txi);

  i1      = 0;
  txo[0] = '\0';


  if(mode == 0) {
    if(txi == NULL) goto L_fertig;


  } else {
    if(txi == NULL) {
      strcpy(txo, "<>");
      goto L_fertig;
    }

    // check if "[-" im Text (keinen masztext)
    if(strstr(txi, "[-") != NULL) goto L_go;
    if(strstr(txi, "[%") != NULL) goto L_go; // skip starting "[%"
    strcpy(txo, "<>");

  }


  L_go:
  ilen = strlen(txi);
  if(ilen < 1) return 0;



  NextChar:
  c1 = txi[i1];


  if (c1 == '[') {
    ++i1;

    if(txi[i1] == '[') {
      strcat(txo, "[");      // \P = newLine

    } else if(txi[i1] == '%') {
      strcat(txo, "<>");      // \P = newLine

    } else if(txi[i1] == 'n') {
      strcat(txo, "\\P");      // \P = newLine

    } else if(txi[i1] == 'd') {
      strcat(txo, "%%c");

    } else if(txi[i1] == 'g') {
      strcat(txo, "%%d");

    } else if(txi[i1] == '+') {
      strcat(txo, "%%p");
    }
    goto L_weiter;
  }

  strncat(txo, &c1, 1);

  L_weiter:
  ++i1;
  if(i1 < ilen)  goto NextChar;


  // if(!strcmp(txt, "[%")) txt[0] = '\0';

  L_fertig:
  printf("ex GR_gxt_dxfout |%s|\n",txo);
  // exit(0);

  return 0;

}


//================================================================
  int GR_gxt_dxfin (char *txt) {
//================================================================
// GR_gxt_dxfin          DXF => gCad

  int  i1, ilen;
  char c1, cbuf[256];


  // printf("GR_gxt_dxfin |%s|\n",txt);

  ilen = strlen(txt);
  if(ilen < 2) return 0;


  i1      = 0;
  cbuf[0] = '\0';

  NextChar:
  c1 = txt[i1];

  if(c1 == '<') {       // start of '<> ?
    if(txt[i1+1] == '>') {
      ++i1;
      strcat(cbuf, "[%");
      goto L_weiter;
    }

  } else if (c1 == '\\') {
    if(txt[i1+1] == 'P') {        // \P = newLine
      strcat(cbuf, "[n");
      ++i1;
      goto L_weiter;
    }



  } else if (c1 == '%') {
    if(txt[i1+1] == '%') {
      i1 += 2;
      if((txt[i1] == 'C')||(txt[i1] == 'c')) {        // %%c = Durchmesser
        strcat(cbuf, "[d");
      } else if((txt[i1] == 'P')||(txt[i1] == 'p')) { // %%p = PlusMinus
        strcat(cbuf, "[+");
      } else if((txt[i1] == 'D')||(txt[i1] == 'd')) { // %%d = Grad-Zeichen
        strcat(cbuf, "[g");
      }
      goto L_weiter;
    }
  }

  strncat(cbuf, &c1, 1);

  L_weiter:
  ++i1;
  if(i1 < ilen)  goto NextChar;


  strcpy(txt, cbuf);

  if(!strcmp(txt, "[%")) txt[0] = '\0';


  // printf("ex GR_gxt_dxfin |%s|\n",txt);
  // exit(0);
  return 0;

}


//========= EOF ==================================================
