// DXF utility funtions                                RF 2016-03-11
/*
 *
 * Copyright (C) 2016 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
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
-----------------------------------------------------
Modifications:
2016-03-11 import from xa_dxf_r.c. RF.
-----------------------------------------------------
*/
/*!
\file  ../exp/dxf_ut.c
\brief DXF utility funtions
\code
=====================================================
List_functions_start:

dxfr_load_mat       Arbitrary Axis Algorithm  - TrMat from vec
dxfr_gxt            text dxf -> gCad
dxfw_gxt            text gCad -> dxf
DXF_colACI_colRGB   returns ACI-color from RGB-color

List_functions_end:
=====================================================

\endcode *//*----------------------------------------

-------------------------------------------------------------------
*/


#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <stdarg.h>


#include "../ut/ut_geo.h"



//================================================================
  int DXF_colACI_colRGB (unsigned char rci,
                         unsigned char gci,
                         unsigned char bci) {
//================================================================
/// \code
/// DXF_colACI_colRGB          returns ACI-color from RGB-color
/// ACI = AutoCad Color Index
/// based on the work of moses@subatomic.com
/// \endcode


  static unsigned char Rca[] = {
    0, 255, 255, 0, 0, 0, 255, 255, 65, 128,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    191, 234, 141, 173, 96, 118, 78, 95, 59, 73,
    127, 212, 94, 157, 64, 107, 52, 86, 39, 66,
    63, 191, 46, 141, 31, 96, 25, 78, 19, 59,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    63, 191, 46, 141, 31, 96, 25, 78, 19, 59,
    127, 212, 94, 157, 64, 107, 52, 86, 39, 66,
    191, 234, 141, 173, 96, 118, 78, 95, 59, 73,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    51, 80, 105, 130, 190, 255};

  static unsigned char Gca[] = {
    0, 0, 255, 255, 255, 0, 0, 255, 65, 128,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    63, 191, 46, 141, 31, 96, 25, 78, 19, 59,
    127, 212, 94, 157, 64, 107, 52, 86, 39, 66,
    191, 234, 141, 173, 96, 118, 78, 95, 59, 73,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    191, 234, 141, 173, 96, 118, 78, 95, 59, 73,
    127, 212, 94, 157, 64, 107, 52, 86, 39, 66,
    63, 191, 46, 141, 31, 96, 25, 78, 19, 59,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    51, 80, 105, 130, 190, 255};

  static unsigned char Bca[] = {
    0, 0, 0, 0, 255, 255, 255, 255, 65, 128,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    0, 170, 0, 126, 0, 86, 0, 69, 0, 53,
    63, 191, 46, 141, 31, 96, 25, 78, 19, 59,
    127, 212, 94, 157, 64, 107, 52, 86, 39, 66,
    191, 234, 141, 173, 96, 118, 78, 95, 59, 73,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    255, 255, 189, 189, 129, 129, 104, 104, 79, 79,
    191, 234, 141, 173, 96, 118, 78, 95, 59, 73,
    127, 212, 94, 157, 64, 107, 52, 86, 39, 66,
    63, 191, 46, 141, 31, 96, 25, 78, 19, 59,
    51, 80, 105, 130, 190, 255};

  int     i1, id, iMin, iCol;


  // printf("DXF_colACI_colRGB %d %d %d\n",rci,gci,bci);


  iMin = 999;
  for(i1=0; i1<256; ++i1) {
    id = abs(rci - Rca[i1]) + abs(gci - Gca[i1]) + abs(bci - Bca[i1]);
    if(id < iMin) {
      iMin = id;
      iCol = i1;
    }
  }

  return iCol;

}


//================================================================
  int dxfw_gxt (int mode, char *txo, char *txi) {
//================================================================
// dxfw_gxt            text gCad -> dxf
// mode=0: nur Notes; kein masztext
// mode=1: Notes plus masztext

  int  i1, ilen;
  char c1;

  // printf("dxfw_gxt |%s|\n",txi);
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
  // printf("ex dxfw_gxt |%s|\n",txo);
  // exit(0);

  return 0;

}


//================================================================
  int dxfr_gxt (char *txt) {
//================================================================
// dxfr_gxt            text dxf -> gCad
  int  i1, ilen;
  char c1, cbuf[256];


  // printf("dxfr_gxt |%s|\n",txt);

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


  // printf("ex dxfr_gxt |%s|\n",txt);
  // exit(0);
  return 0;

}


//===========================================================================
  int dxfr_load_mat (Mat_4x3 m1, Vector* vz) {
//===========================================================================
/// \code
/// dxfr_load_mat       Arbitrary Axis Algorithm  - TrMat from vec
/// Input:
///   vz       old z-axis
/// Output:
///   m1       new refSys
///   retCod:  0: refSys is parallel to old z-axis
///            1: refSys is not parallel to old z-axis
/// \endcode

  // Autocad-System zur Berechnung des lokalen Achsensystems:
  // IN:  der Z-Vektor.
  // OUT: die Matrix.



  int       irc;
  double    d1;
  Vector    vx, vy;
  Plane     pl1;


  // printf("dxfr_load_mat \n");
  // DEB_dump_obj__ (Typ_VC, vz, "dxfr_load_mat vz:");
  // DXFW_test ("2");


  // Erstens: ist der neue Z-Vektor ident mit dem Haupt-Z-Vektor:
  // (wenn sein X-Anteil und sein Y-Anteil kleiner als 1/64 sind)
  d1 = 1. / 64.;
  if((fabs(vz->dx) > d1)||(fabs(vz->dy) > d1)) goto Nicht_Z_Parallel;

  // DXFW_test ("3");


  //----------------------------------------------------------------
  // // YES, IS PARALLEL
    // printf(" _load_mat - Z-parallel %lf\n",d1);
  irc = 0;
  // JA: Kreuzprod. von Haupt-Y-Vektor und Z-Vektor = neue X-Achse;
  UT3D_vc_perp2vc (&vx, (Vector*)&UT3D_VECTOR_Y, vz);
    // DEB_dump_obj__ (Typ_VC, &vx, " vx:");

  UT3D_vc_perp2vc (&vy, vz, &vx);

  goto Load_Matrix;


  //----------------------------------------------------------------
  // NOT PARALLEL
  // NEIN: Kreuzprod. von Z-Vektor und Haupt-Z-Vektor = neue X-Achse;
  Nicht_Z_Parallel:
    // printf(" _load_mat - NOT Z-parallel %lf\n",d1);

  irc = 1;
  UT3D_vc_perp2vc (&vx, (Vector*)&UT3D_VECTOR_Z, vz);

  UT3D_vc_perp2vc (&vy, vz, &vx);


  Load_Matrix:
  pl1.po = UT3D_PT_NUL;
  pl1.vx = vx;
  pl1.vy = vy;
  pl1.vz = *vz;

  UT3D_m3_loadpl (m1, &pl1);


    // TESTBLOCK
    // DEB_dump_obj__ (Typ_VC, &vx, " vx:");
    // DEB_dump_obj__ (Typ_VC, &vy, " vy:");
    // DEB_dump_obj__ (Typ_M4x3, m1, " irc=%d m1:",irc);
    // END TESTBLOCK


  return  irc;

}

// EOF
