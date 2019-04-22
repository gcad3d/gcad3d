//  Read 3DS-File                           Reiter  2002-07-20
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
\file  ../exp/ds3_r.c
\brief Read 3DS-File 
\code
=====================================================
List_functions_start:

ds3_read
read_object
read_edit        read 0x3D3D - records
read_mesh
read_char
read_short
read_long
read_string

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


OFFLINE-Test:
main
ds3_cb_savePoint
ds3_cb_saveFace

-----------------------------------------------------
 Records ab Byte 16; jeder Record besteht aus:
   RecTyp   2-Byte
   RecLen   4-Byte
   RecDat   Laenge (RecLen-6)

   Jeder Datenrecord RecDat wieder aus:
     RecTyp   2-Byte
     RecLen   4-Byte
     RecDat   Laenge (RecLen-6)

   Vertices:
    Anzahl   2-Byte
    Coords   float[Anzahl][3]

   Faces:
    Anzahl   2-Byte
    Index    short[Anzahl][3]


\endcode *//*----------------------------------------



---------------------------------------------------------
TestCompile:
cc ds3_r.c -DOFFLINE && a.out

od -tx1 xx.3ds > t1.dat


---------------------------------------------------------
read-function:
  ds3_read
callback-functions:
  ds3_cb_savePoint
  ds3_cb_saveFace




================= FORMAT 3DS: ===========================


                  T_CONFIG1MAIN3D
                  |
                  +--EDIT3DS  (0x3D3D)
                  |  |
                  |  +--EDIT_MATERIAL (0xAFFF)
                  |  |
                  |  +--EDIT_CONFIG1  (0x0100)
                  |  |
                  |  +--EDIT_OBJECT   (0x4000)
                  |  |  |
                  |  |  +--OBJ_TRIMESH   (0x4100)
                  |  |  |  |
                  |  |  |  +--TRI_VERTEXL          (0x4110)
                  |  |  |  +--TRI_FACEL1           (0x4120)
                  |   
                  +--KEYF3DS (0xB000)
                  |   

*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



#define MAIN3DS       0x4D4D






static FILE *fpi = NULL;
static char txbuf[256];




static unsigned long  read_char();
static unsigned long  read_short();
static unsigned long  read_long();




//============================================================
static unsigned long read_char() {
//============================================================
// direkt mit fgetc oder getc ersetzen ..

  return fgetc(fpi);;
}


//============================================================
static unsigned long read_short() {
//============================================================
// ident mit CBO_r_ui2; ersetzen ..



  unsigned char c1, c2;

  c1 = read_char();
  c2 = read_char();

  // printf("read_short %x %x\n",c1,c2);

  return (c1 | c2<<8);

}


//============================================================
static unsigned long read_long() {
//============================================================
// ident mit CBO_r_ui4; ersetzen ..

  unsigned char c1, c2, c3, c4;

  c1 = read_char();
  c2 = read_char();
  c3 = read_char();
  c4 = read_char();

  // printf("read_long %x %x %x %x\n",c1,c2,c3,c4);

  return (c1 | c2<<8 | c3<<16 | c4<<24);


}


//============================================================
static int read_string() {
//============================================================

  int i1;


  i1 = 0;

  for (;;) {
    txbuf[i1] = read_char();
    if(txbuf[i1] == '\0') {
      ++i1;
      break;
    }
    ++i1;
  }

    // printf("read_string %d |%s|\n",i1,txbuf);

  return i1;

}


//============================================================
static int read_mesh (unsigned long clen) {
//============================================================

  int           i1;
  unsigned long id, ilen, pos, tlen, vNr, fNr, ii;
  float         ptAct[3];
  int           vTab[3];


  // printf("read_mesh %ld\n",clen);

  tlen = 0;

  for (;;) {

    id = read_short();
    ilen = read_long();
    tlen += ilen;
      // printf("read_mesh: id=%04x ilen=%ld tlen=%ld\n",id,ilen,tlen);
    if(ilen < 2) return -1;

    switch(id) {

      case 0x4110:  // vertices
        vNr = read_short();
          // printf("vNr=%ld\n",vNr);
        for(i1=0; i1<vNr; ++i1) {
          fread(&ptAct[0], sizeof(float), 1, fpi);
          fread(&ptAct[1], sizeof(float), 1, fpi);
          fread(&ptAct[2], sizeof(float), 1, fpi);
          ds3_cb_savePoint (i1, ptAct);
          // printf(" vert %d %f.%f,%f\n",i1,kx,ky,kz);
        }
        break;

      case 0x4120:  // faces
        fNr = read_short();
          // printf("fNr=%ld\n",fNr);
        for(i1=0; i1<fNr; ++i1) {
          vTab[0] = read_short();
          vTab[1] = read_short();
          vTab[2] = read_short();
          ii = read_short();
          // printf(" fac %d  %d,%d,%d %d\n",i1,ia,ib,ic,ii);
          
          // callback appli
          ds3_cb_saveFace (3, vTab);

        }
        // exit(0);
        break;



      default: // Unknown
          // printf("read_mesh: skip chunk id=%04x ilen=%ld\n",id,ilen);
        fseek(fpi, ilen-6, SEEK_CUR); // skip
        break;
    }

    if(tlen >= clen) break;
  }


  return 0;

}


//============================================================
static int read_object (unsigned long clen) {
//============================================================

  unsigned long id, ilen, pos, tlen;


  // printf("read_object %ld\n",clen);
  // fseek(fpi, clen-6, SEEK_CUR); // skip

  // zuerst den Namen lesen
  tlen = read_string() + 6; // total len


  for (;;) {

    id = read_short();
    ilen = read_long();
    tlen += ilen;
      // printf("read_object: id=%04x ilen=%ld tlen=%ld\n",id,ilen,tlen);
    if(ilen < 2) return -1;

    switch(id) {

      case 0x4100:  // material
        read_mesh(ilen);
        break;

      default: // Unknown
          // printf("read_object: skip chunk id=%04x ilen=%ld\n",id,ilen);
        fseek(fpi, ilen-6, SEEK_CUR); // skip
        break;
    }

    if(tlen >= clen) break;
  }

  return 0;

}


//============================================================
static int read_edit (unsigned long clen) {
//============================================================

  unsigned long id, ilen, tlen;


  printf("read_edit %ld\n",clen);

  tlen = ftell(fpi);

  for (;;) {

    id = read_short();
    ilen = read_long();
    tlen += ilen;
      // printf("read_edit: id=%04x ilen=%ld tlen=%ld\n",id,ilen,tlen);
    if(ilen < 2) return -1;

    switch(id) {

      case 0x4000:  // MESH
        read_object (ilen);
        break;

      case 0xAFFF:  // material
        fseek(fpi, ilen-6, SEEK_CUR); // skip
        break;

      default: // Unknown
          // printf("read_edit: skip chunk id=%04x ilen=%ld\n",id,ilen);
        fseek(fpi, ilen-6, SEEK_CUR); // skip
        break;
    }

    if(tlen >= clen) break;
  }


  return 0;

}


//============================================================
int ds3_read (const char *filNam) {
//============================================================

  long ilen;
  unsigned long l1, id, cSiz;


  printf("ds3_read |%s|\n",filNam);


  fpi = NULL;
  fpi=fopen (filNam,"rb");
  if (fpi==NULL) {
    printf("ds3_read E001\n");
    return -1;
  }


  // Byte 1 u 2:    0x4D4D
  if(read_short() != MAIN3DS) {
    printf("ds3_read E002\n");
    return -2;
  }


/*
  // test find version number (byte 29)
  // nur f. version 3.0 and higher
  fseek (fpi,28L,SEEK_SET);
  l1=read_short ();
  printf("Version=%04x\n",l1);

  if (l1<3) {
    printf("ds3_read E003\n");
    return -3;
  }
*/


  fseek (fpi,16,SEEK_SET);


  for (;;) {

    id = read_short();
    ilen = read_long();
      // printf("ds3_read: id=%04x ilen=%ld\n",id,ilen);

    if((id == 0xFFFF) || (ilen < 2)) break;   // EOF

    switch(id) {

      case 0x3D3D:          // Objects (EDIT3DS-chunk)
        read_edit(ilen);
        break;

      case 0xB000:          // KEYF3DS-chunk
      default: // Unknown
          // printf("ds3_read: skip chunk id=%04x ilen=%ld\n",id,ilen);
        fseek(fpi, ilen-6, SEEK_CUR); // skip
        break;
    }

  }


  fclose(fpi);

  return 0;

}


//========================================================
//========================================================
#ifdef OFFLINE


static FILE *fpo;


//============================================================
  int main (int argc, char *argv[]) {
//============================================================

  FILE *fp;
  char ds3_object_name[80];
  int  i1;


  printf("ds31\n");

  strcpy(ds3_object_name, "../xa/dat/romulan.3ds");
  // strcpy(ds3_object_name, "mailtruk.3ds");
  // strcpy(ds3_object_name, "shuttle1.3ds");


  // open Outfile
  fpo=fopen("t1.dat","w");

  // work
  i1 = ds3_read (ds3_object_name);
  if (i1 < 0) {
    printf("Can't read %s\n", ds3_object_name);
    return 0;
  }

  fclose(fpo);

  printf("done ...\n");

  return 0;

}


//============================================================
int ds3_cb_savePoint (int ip, float *pt1) {
//============================================================

  int ptNr;

  // printf("ds3_cb_savePoint %d %f,%f,%f\n",ip,pt1[0],pt1[1],pt1[2]);

  ptNr = ip + 20;

  fprintf(fpo, "P%d=%f,%f,Z(%f)\n",ptNr,pt1[0],pt1[1],pt1[2]);


  return 0;
}


//============================================================
int ds3_cb_saveFace (int vNr, int *vTab) {
//============================================================

  static int   arNr = 20;

  int   i1;
  char  auxBuf[32];


  // printf("ds3_cb_saveFace %d  ",vNr);

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
