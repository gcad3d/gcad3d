//        BMP -Bitmap-FileFunktionen.   Franz Reiter 2005-03-15
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
2011-02-01  reading of 8-bit-colour added. RF.
2008-08-14  padding korrigiert. RF.
2005-03-15  Load 24-bit-colour bitmaps. New. RF.

-----------------------------------------------------
*/
/*!
\file  ../exp/bmp_ut.c
\brief BMP -Bitmap functions 
\code
=====================================================
List_functions_start:

bmp_load    load bmp-file into memory

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


Bits per Pixel   
   1 = monochrome palette. NumColors = 1   
   4 = 4bit palletized. NumColors = 16   
   8 = 8bit palletized. NumColors = 256  
  16 = 16bit RGB. NumColors = 65536 (?)  
  24 = 24bit RGB. NumColors = 16M

Type of Compression   
  0 = BI_RGB   no compression   
  1 = BI_RLE8 8bit RLE encoding   
  2 = BI_RLE4 4bit RLE encoding

\endcode *//*----------------------------------------

*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




#include "../ut/byteorder.h"

typedef struct {
  unsigned short it; unsigned long is; unsigned short iu1, iu2;
  unsigned long io;
} bitMapRec1;


typedef struct {
  unsigned long ish,iw,ih; unsigned long ipl,inr;
  unsigned long ic,isi,ix,iy,icu,ici;
} bitMapRec2;


typedef struct {                     /**** Colormap entry structure ****/
  unsigned char  rgbBlue;          /* Blue value */
  unsigned char  rgbGreen;         /* Green value */
  unsigned char  rgbRed;           /* Red value */
  unsigned char  rgbReserved;      /* Reserved */
} colRec1;                         // Windows: RGBQUAD oder so


  unsigned short bmp_MB='MB';


//================================================================
  int bmp_info1 (int *ix, int *iy, char *fnam) {
//================================================================
// unused ..

  int        pixSiz;
  bitMapRec1 rec1;
  bitMapRec2 rec2;


  if(CBO_open (fnam) < 0) {
    TX_Print("****** bmp_info1: OPEN ERROR FILE %s **********",fnam);
    return -1;
  }


  // BITMAPFILEHEADER bitMapRec1
  rec1.it  = CBO_r_ui2 ();  // 'BM'
  rec1.is  = CBO_r_ui4 ();  // File size in bytes
  rec1.iu1 = CBO_r_ui2 ();  // unused (=0)
  rec1.iu2 = CBO_r_ui2 ();  // unused
  rec1.io  = CBO_r_ui4 ();  // File offset to Raster Data
  // size of Rec1 = 14 !


  // printf(" |%2.2s|\n",&rec1.it);

  if(rec1.it != bmp_MB) {    // 'MB'
    TX_Print("****** NOT BMP-Format:  %s **********",fnam);
    goto L_err1;
  }

    // printf(" is=%d io=%d\n",rec1.is,rec1.io);  // FileSize,offset




  rec2.ish  = CBO_r_ui4 (); // Size of InfoHeader =40
  rec2.iw   = CBO_r_i4 ();  // Bitmap Width
  rec2.ih   = CBO_r_i4 ();  // Bitmap Height
  rec2.ipl  = CBO_r_ui2 (); // Number of Planes (=1)
  rec2.inr  = CBO_r_ui2 (); // Bits per Pixel 
 
  *ix = rec2.iw;
  *iy = rec2.ih;

  CBO_close ();
  return rec2.inr;


  //----------------------------------------------------------------
  L_err1:
    CBO_close ();
    return -1;
}





//================================================================
  int bmp_load (void **mSpc, int *ix, int *iy, char *fnam) {
//================================================================
// Load bitmap into memspace. memspace must be freed !
// RetCod: nr of bytes per Pixel; 1 (8-bit) or 3 (24-bit).
// void   *mSpc;
// irc = bmp_load (&mSpc, &ix, &iy, "xx.bmp");
// if(irc < 0) return -1;
// glDrawPixels (ix, iy, .. mSpc);
// free (mSpc);


  int        i1, bmSiz, lbf, lbm, lbdiff, pixSiz;
  bitMapRec1 rec1;
  bitMapRec2 rec2;
  char       *po, pPad[24];

  colRec1    colTab[256];


  // printf("bmp_load |%s|\n",fnam);


  if(CBO_open (fnam) < 0) { 
    TX_Print("****** OPEN ERROR FILE %s **********",fnam);
    return -1;
  }


  // BITMAPFILEHEADER bitMapRec1
  rec1.it  = CBO_r_ui2 ();  // 'BM'
  rec1.is  = CBO_r_ui4 ();  // File size in bytes
  rec1.iu1 = CBO_r_ui2 ();  // unused (=0)
  rec1.iu2 = CBO_r_ui2 ();  // unused
  rec1.io  = CBO_r_ui4 ();  // File offset to Raster Data
  // size of Rec1 = 14 !


  // printf(" |%2.2s|\n",&rec1.it);

  if(rec1.it != bmp_MB) {   // 'MB'
    TX_Print("****** NOT BMP-Format:  %s **********",fnam);
    goto L_err1;
  }

    // printf(" is=%d io=%d\n",rec1.is,rec1.io);  // FileSize,offset




  rec2.ish  = CBO_r_ui4 (); // Size of InfoHeader =40
  rec2.iw   = CBO_r_i4 ();  // Bitmap Width
  rec2.ih   = CBO_r_i4 ();  // Bitmap Height
  rec2.ipl  = CBO_r_ui2 (); // Number of Planes (=1)
  rec2.inr  = CBO_r_ui2 (); // Bits per Pixel 
  rec2.ic   = CBO_r_ui4 (); // Type of Compression
  rec2.isi  = CBO_r_ui4 (); // (compressed) Size of Image
  rec2.ix   = CBO_r_i4 ();  // horizontal resolution: Pixels/meter
  rec2.iy   = CBO_r_i4 ();  // vertical resolution: Pixels/meter
  rec2.icu  = CBO_r_ui4 (); // Number of actually used colors
  rec2.ici  = CBO_r_ui4 (); // Number of important colors; 0 = all
  // size of Rec2 = 40 !

    // printf(" wi=%d he=%d siz=%d\n",rec2.iw,rec2.ih,rec2.isi);
    // printf(" compr=%d bits=%d\n",rec2.ic,rec2.inr);
    // printf(" ipl=%d ix=%d iy=%d\n",rec2.ipl,rec2.ix,rec2.iy);
    // printf(" col=%d %d\n",rec2.icu,rec2.ici);
    // printf(" ish=%d siz=%d\n",rec2.ish,rec2.isi);


  if(rec2.ic != 0) {
    TX_Print("****** cannot decode BMP-compression %s **********",fnam);
    goto L_err1;
  }


  // Die Farbanzahl rec2.inr muss mit OpenGL uebereinstimmen !
  // default ist 24 bit; reduced: 16 bit.
  pixSiz = rec2.inr / 8;   // size of 1 pixel in bytes
  if((pixSiz != 3)&&(pixSiz != 1)) {
    TX_Print("****** wrong colNr of texture (%d) %s ******",rec2.inr,fnam);
    goto L_err1;
  }

  



  //----------------------------------------------------------------
  // Colortable einlesen; unused .. ...
  // lbf = rec1.io - sizeof(bitMapRec1) - sizeof(bitMapRec2);
  lbf = rec1.io - 14 - 40;      // better use rec2.ish ?


  if (lbf > 0) {
    // printf(" lbf=%d io=%d\n",lbf,rec1.io);
    if(CBO_read(colTab, lbf) < lbf) {      // read lbf chars out of file
      TX_Print("****** bmp_load E001 %s ******",fnam);
      goto L_err1;
    }
  }



  //----------------------------------------------------------------
  // compute bitmapsize
/*
  bmSiz = rec2.isi;
  if(bmSiz == 0) {
    bmSiz = (rec2.iw * rec2.inr + 7) / 8 * abs(rec2.ih);
  }
  // printf(" bmSiz=%d\n",bmSiz);
  // lbf = bmSiz / abs(rec2.ih);        // wirkliche bytes pro zeile
  // lbm = (rec2.iw * rec2.inr) / 8;    // erforderliche bytes pro zeile
*/


  // lbm = size of one line in bytes in memory
  lbm = rec2.iw * pixSiz;  // gilt nur fuer 8 bit (1 byte pro pixel)
  // lbm = rec2.iw * 3;    // gilt nur fuer 24 bit (3 byte pro pixel)

  // lbf = size of one line in bytes in bmp-file
  // die Zeile muss mit 4 teilbar sein !
  lbf = UTI_div4up (lbm);
    // printf(" lbm=%d lbf=%d\n",lbm,lbf);


  // memSiz
  bmSiz = lbm * rec2.ih;


  // allocate mem
  *mSpc = malloc(bmSiz);
  if (*mSpc == NULL) {
    TX_Print("****** bmp_load EOM %s ******",fnam);
    goto L_err1;
  }


  // check padding:
  lbdiff = lbf - lbm;                 // differenz = padding
    // printf(" pad %d %d %d\n",lbdiff,lbm,lbf);



  // Bitmap einlesen
  if(lbdiff < 1) {

    // kein padding; gesamten Block einlesen
    if(CBO_read(*mSpc, bmSiz) < bmSiz) {
      free(*mSpc);
      TX_Print("****** bmp_load E002 %s ******",fnam);
      goto L_err1;
    }

  } else {

    // lbdiff characters padding
    po = *mSpc;
    for(i1=0; i1<rec2.ih; ++i1) {    // .ih = nr of lines
      // read lbm chars
      if(CBO_read(po, lbm) < lbm) {
        free(*mSpc);
        TX_Print("****** bmp_load E003 %s ******",fnam);
        goto L_err1;
      }
      po += lbm;
      // read lbdiff chars - but ignore them
      if(lbdiff > 0) CBO_read(pPad, lbdiff);
    }

  }


  *ix = rec2.iw;
  *iy = rec2.ih;


  CBO_close ();

    // printf("ex bmp_load pixSiz=%d wi=%d he=%d siz=%d\n",pixSiz,
                      // rec2.iw,rec2.ih,rec2.isi);

  return pixSiz;


  //----------------------------------------------------------------
  L_err1:
    CBO_close ();
    return -1;
}


//======================== EOF ======================
