//  Read LWO-File                   Reiter  2002-07-20
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
\file  ../exp/lwo_r.c
\brief read LWO file 
\code
=====================================================
List_functions_start:

lwo_read
read_pnts
read_pols
read_srfs
read_surf
read_char
read_short
read_long
read_float
read_string

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


OFFLINE - Tests:
main
lwo_cb_savePoint
lwo_cb_saveFace

\endcode *//*----------------------------------------


================= FORMAT LWO: ===========================
Chunk = Recordformat =
  I32  aus 4 characters = ID; zB 'FORM' od PNTS' ...
  I32  die Byteanzahl des Record (gesamt: +8 !)


IFF File; erster record=
  'FORM' size_of_file, 'LWOB'.
  File (=Model) muss EINEN PNTS-chunk, einen SRFS Chunk und einen POLS chunk
  enthalten.
  Kann 0-n SURF chunks enthalten, 0-1 CRVS chunk.


PNTS Chunk: a list of all the points.
  Als xyz-floats (= 12 Byte/Punkt).
  X=right=east, Y=up, Z=forward=north

SRFS Chunk
 = names of surfs

POLS Chunk: a list of all the polygons in an object.
  First polygon = convex, clockwise seen from the visible side of the polygon,
  single-sided, except for those whose surfaces have the doublesided flag set.
  vAnz        I16 Anzahl Punkte (max 200)
  vInd[vAnz]  I16 Punktindizes (first Ind = 0)
  sInd        I16 Surf-Index (first Ind = 1, negativ: detail polygons exist).

  DetailPolygon (immer nach einem negativen sInd):
  dAnz        I16 Anzahl DetailPolygons.
  <Polygon>   wie oben beschrieben


CRVS chunk: a list of all the spline curves.
  cAnz       I16 Anzahl Punkte
  cInd[cAnz] I16 Punktindizes der Kurve
             I16 Surf-Index
             I16 flag; If bit zero is set then the first point is a continuity
                 control point, and if bit one is set then the last point is.

SURF chunk: colour usw of a group of polygons


---------------------------------------------------------
TestCompile:
cc lwo_r.c -DOFFLINE && a.out



*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define MK_ID(a,b,c,d) ((((unsigned long)(a))<<24)| \
      (((unsigned long)(b))<<16)| \
      (((unsigned long)(c))<< 8)| \
      (((unsigned long)(d))    ))

#define LWO_FORM MK_ID('F','O','R','M')
#define LWO_LWOB MK_ID('L','W','O','B')
#define LWO_PNTS MK_ID('P','N','T','S')
#define LWO_SRFS MK_ID('S','R','F','S')
#define LWO_SURF MK_ID('S','U','R','F')
#define LWO_POLS MK_ID('P','O','L','S')
#define LWO_COLR MK_ID('C','O','L','R')


#define LW_MAX_POINTS   200
#define LW_MAX_NAME_LEN 500




static long  read_char(FILE *f);
static long  read_short(FILE *f);
static long  read_long(FILE *f);
static float read_float(FILE *f);
static long  read_string(FILE *f, char *s);


static long  vertex_cnt;

static char  txbuf[512];



// externe Funktionen:
int lwo_cb_savePoint (int ip, float *pt1);
int lwo_cb_saveFace  (int vNr, int *vTab);





//============================================================
static void read_pnts(FILE *f, long nbytes) {
//============================================================

  int   i;
  float ptAct[3];

  // printf("read_pnts %ld\n",nbytes);

  vertex_cnt = nbytes / 12;

  for (i=0; i<vertex_cnt; i++) {
    ptAct[0] = read_float(f);
    ptAct[2] = read_float(f);
    ptAct[1] = read_float(f);
    // callback appli
    lwo_cb_savePoint (i, ptAct);
  }
}



//============================================================
static void read_pols(FILE *f, long nbytes) {
//============================================================


  int i, det_cnt, cnt, index_cnt;
  int material;
  int vTab[LW_MAX_POINTS];


  // printf("read_pols %ld\n",nbytes);


  while (nbytes > 0) {

    /* number of points in this face */
    index_cnt = read_short(f);
    nbytes -= 2;


    /* read points in */
    for (i=0; i<index_cnt; i++) {
      vTab[i] = read_short(f);
      nbytes -= 2;
    }

    /* read surface material */
    material = read_short(f);
    nbytes -= 2;

    /* skip over detail  polygons */
    if (material < 0) {
      material = -material;
      det_cnt = read_short(f);
      nbytes -= 2;
      while (det_cnt-- > 0) {
        cnt = read_short(f);
        fseek(f, cnt*2+2, SEEK_CUR);
        nbytes -= cnt*2+2;
      }
    }
    material -= 1; // index korrigieren (beginnt mit 1 !)

    // callback appli
    lwo_cb_saveFace (index_cnt, vTab);

  }

}




//============================================================
static void read_srfs(FILE *f, long nbytes) {
//============================================================

  // printf("read_srfs %ld\n",nbytes);


  while (nbytes > 0) {

    /* read name */
    nbytes -= read_string(f, txbuf);

    /* defaults */
    // material->r = 0.7;
    // material->g = 0.7;
    // material->b = 0.7;
  }

}




//============================================================
static void read_surf(FILE *f, long nbytes) {
//============================================================

  float r,g,b;


  // printf("read_surf %ld\n",nbytes);


  /* read surface name */
  nbytes -= read_string(f, txbuf);


/*
  // find material
  for (i=0; i< lwo->material_cnt; i++) {
    if (strcmp(lwo->material[i].name,name) == 0) {
      material = &lwo->material[i];
      break;
    }
  }
  // g_return_if_fail(material != NULL);
*/


  /* read values */
  while (nbytes > 0) {
    long id = read_long(f);
    long len = read_short(f);

    nbytes -= 6 + len + (len%2);

    switch (id) {

    case LWO_COLR:
      r = read_char(f) / 255.0;
      g = read_char(f) / 255.0;
      b = read_char(f) / 255.0;
      read_char(f); /* dummy */
      // printf("LWO_COLR=%f,%f,%f\n",r,g,b);
      break;

    default:
      fseek(f, len+(len%2), SEEK_CUR);
    }
  }
}



//============================================================
int lwo_read (const char *lw_file) {
//============================================================

  FILE *f = NULL;

  long form_bytes;
  long read_bytes;
  long id;
  long nbytes;


  /* open file */
  f = fopen(lw_file, "rb");
  if (f == NULL) {
    printf("can't open file %s", lw_file);
    return -1;
  }


  /* check for headers */
  if (read_long(f) != LWO_FORM) {
    printf("file %s is not an IFF file", lw_file);
    fclose(f);
    return -2;
  }


  form_bytes = read_long(f);
    // printf("form_bytes=%ld\n",form_bytes);


  if (read_long(f) != LWO_LWOB) {
    printf("file %s is not a LWOB file", lw_file);
    fclose(f);
    return -3;
  }



  /* read chunks */
  read_bytes = 0;
  // form_bytes -= 4;

  while (read_bytes < form_bytes) {
    id     = read_long(f);
    nbytes = read_long(f);
    read_bytes += 8 + nbytes + (nbytes%2);

    switch (id) {


    case LWO_PNTS:
      read_pnts(f, nbytes);
      break;


    case LWO_POLS:
      read_pols(f, nbytes);
      break;


    case LWO_SRFS:
      read_srfs(f, nbytes);
      break;


    case LWO_SURF:
      read_surf(f, nbytes);
      break;


    default:  // skip rec
      fseek(f, nbytes + (nbytes%2), SEEK_CUR);
    }
  }


  fclose(f);
  return 0;
}








//============================================================
static long read_char(FILE *f) {
//============================================================

  int c = fgetc(f);
  // g_return_val_if_fail(c != EOF, 0);
  return c;
}


//============================================================
static long read_short(FILE *f) {
//============================================================

  return (read_char(f)<<8) | read_char(f);
}


//============================================================
static long read_long(FILE *f) {
//============================================================
  return (read_char(f)<<24) |
         (read_char(f)<<16) |
         (read_char(f)<<8)  |
         read_char(f);
}


//============================================================
static float read_float(FILE *f) {
//============================================================

  long x = read_long(f);
  return *(float*)&x;
}


//============================================================
static long read_string(FILE *f, char *s) {
//============================================================

  long c;
  long cnt = 0;

  do {
    c = read_char(f);
    if (cnt < LW_MAX_NAME_LEN)
      s[cnt] = c;
    else
      s[LW_MAX_NAME_LEN-1] = 0;
    cnt++;
  } while (c != 0);

  /* if length of string (including \0) is odd skip another byte */
  if (cnt%2) {
    read_char(f);
    cnt++;
  }

  // printf("ex read_string %d |%s|\n",cnt,s);

  return cnt;
}




//========================================================
//========================================================
#ifdef OFFLINE


static FILE *fpo;


//============================================================
  int main (int argc, char *argv[]) {
//============================================================

  FILE *fp;
  char lwobject_name[80];
  int  i1;


  printf("lwo1\n");

  strcpy(lwobject_name, "../xa/dat/penguin.lwo");
  // strcpy(lwobject_name, "alien.lwo");


  // open Outfile
  fpo=fopen("t1.dat","w");

  // work
  i1 = lwo_read (lwobject_name);
  if (i1 < 0) {
    printf("Can't read LightWave 3D object %s\n", lwobject_name);
    return 0;
  }

  fclose(fpo);

  return 0;

}

//============================================================
int lwo_cb_savePoint (int ip, float *pt1) {
//============================================================

  int ptNr;

  // printf("lwo_cb_savePoint %d %f,%f,%f\n",ip,pt1[0],pt1[1],pt1[2]);

  ptNr = ip + 20;

  // printf("P%d=%f,%f,Z(%f)\n",ptNr,pt1[0],pt1[2],pt1[1]);
  fprintf(fpo, "P%d=%f,%f,Z(%f)\n",ptNr,pt1[0],pt1[2],pt1[1]);


  return 0;
}

//============================================================
int lwo_cb_saveFace (int vNr, int *vTab) {
//============================================================

  static int   arNr = 20;

  int   i1;
  char  auxBuf[32];


  // printf("lwo_cb_saveFace %d  ",vNr);

  sprintf(txbuf, "A%d=RCIR",arNr);
  ++arNr;


  for(i1=0; i1<vNr; ++i1) {
    // printf("%4d  ",vTab[i1]);
    sprintf(auxBuf, ",P%d",vTab[i1]+20);
    strcat(txbuf, auxBuf);
  }
  strcat(txbuf, "\n");

  // printf(txbuf);
  fprintf(fpo, txbuf);

  return 0;
}


#endif
//======================= EOF ================================
