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
2016-04-12  bmp_save__ added. RF.
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
bmp_info1   test if file is bmp-file
bmp_save__  save pixmap as bmp-file

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


struct BMPHeader
{
    char bfType[2];       /* "BM" */
    int bfSize;           /* Size of file in bytes */
    int bfReserved;       /* set to 0 */
    int bfOffBits;        /* Byte offset to actual bitmap data (= 54) */
    int biSize;           /* Size of BITMAPINFOHEADER, in bytes (= 40) */
    int biWidth;          /* Width of image, in pixels */
    int biHeight;         /* Height of images, in pixels */
    short biPlanes;       /* Number of planes in target device (set to 1) */
    short biBitCount;     /* Bits per pixel (24 in this case) */
    int biCompression;    /* Type of compression (0 if no compression) */
    int biSizeImage;      /* Image size, in bytes (0 if no compression) */
    int biXPelsPerMeter;  /* Resolution in pixels/meter of display device */
    int biYPelsPerMeter;  /* Resolution in pixels/meter of display device */
    int biClrUsed;        /* Number of colors in the color table (if 0, use 
                             maximum allowed by biBitCount) */
    int biClrImportant;   /* Number of important colors.  If 0, all colors 
                             are important */
};


  unsigned short bmp_MB = 'MB';


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
    // printf(" is=%ld io=%ld\n",rec1.is,rec1.io);  // FileSize,offset




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

    // printf(" wi=%ld he=%ld siz=%ld\n",rec2.iw,rec2.ih,rec2.isi);
    // printf(" compr=%ld bits=%ld\n",rec2.ic,rec2.inr);
    // printf(" ipl=%ld ix=%ld iy=%ld\n",rec2.ipl,rec2.ix,rec2.iy);
    // printf(" col=%ld %ld\n",rec2.icu,rec2.ici);
    // printf(" ish=%ld siz=%ld\n",rec2.ish,rec2.isi);


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


//================================================================
  int bmp_save__ (char *fNam) {
//================================================================

  int    iw, ih, lSiz1, lSiz2, fSiz, i1, ii;
  void   *pm;
  char   *vp1;

  FILE   *file;
  struct BMPHeader bmph;

  void*  GL_Print1   (int *iw, int *ih);


  printf("bmp_save__ |%s|\n",fNam);

  pm = GL_Print1 (&iw, &ih);
  if(!pm) {TX_Error("bmp_save__ E001"); return -1;}
    // printf(" iw=%d ih=%d\n",iw,ih);

  // lSiz1 = iw;   // size of line - input grayscale; 1 byte per pixel
  lSiz1 = iw * 3;  // size of line - input BGR


  lSiz2 = (lSiz1 + 3) & ~3;   // size of line 4-bytes aligned
  fSiz  = lSiz2 * ih;
    // printf(" lSiz1=%d lSiz2=%d fSiz=%d\n", lSiz1, lSiz2, fSiz);fflush(stdout);


  // fill bitmap-header
  // strcpy(bmph.bfType, "BM");
  bmph.bfType[0] = 'B';
  bmph.bfType[1] = 'M';
  bmph.bfOffBits = 54;
  bmph.bfSize = bmph.bfOffBits + fSiz;
  bmph.bfReserved = 0;
  bmph.biSize = 40;           // Size of BITMAPINFOHEADER, in bytes
  bmph.biWidth = iw;
  bmph.biHeight = ih;
  bmph.biPlanes = 1;          // Number of planes in target device
  // bmph.biBitCount = 8;        // Bits per pixel - grayscale
  bmph.biBitCount = 24;       // Bits per pixel RGB
  bmph.biCompression = 0;
  bmph.biSizeImage = 0;       // Image size (0 = no compression)
  bmph.biXPelsPerMeter = 0;   // Resolution in pixels/meter of display device
  bmph.biYPelsPerMeter = 0;
  bmph.biClrUsed = 0;         // Number of colors in the color table
  bmph.biClrImportant = 0;    // Number of important colors


  file = fopen (fNam, "wb");
  if(!file) {TX_Error("bmp_save__ E002"); return -1;}

  // write bitmap-header
  fwrite(&bmph.bfType, 2, 1, file);
  fwrite(&bmph.bfSize, 4, 1, file);
  fwrite(&bmph.bfReserved, 4, 1, file);
  fwrite(&bmph.bfOffBits, 4, 1, file);
  fwrite(&bmph.biSize, 4, 1, file);
  fwrite(&bmph.biWidth, 4, 1, file);
  fwrite(&bmph.biHeight, 4, 1, file);
  fwrite(&bmph.biPlanes, 2, 1, file);
  fwrite(&bmph.biBitCount, 2, 1, file);
  fwrite(&bmph.biCompression, 4, 1, file);
  fwrite(&bmph.biSizeImage, 4, 1, file);
  fwrite(&bmph.biXPelsPerMeter, 4, 1, file);
  fwrite(&bmph.biYPelsPerMeter, 4, 1, file);
  fwrite(&bmph.biClrUsed, 4, 1, file);
  fwrite(&bmph.biClrImportant, 4, 1, file);


  // write line by line
  ii = 0;
  vp1 = pm;
  for (i1=0; i1 < ih; ++i1) {
    fwrite (vp1, lSiz2, 1, file);
    vp1 += lSiz1;
  }

  fclose(file);

  if(pm) free (pm);

  return 0;

}

//======================== EOF ======================
