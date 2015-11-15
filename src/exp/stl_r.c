//       IMPORT STL-File                   2007-02-17               RF
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
wie kann man 3ecke nach gcad sichern - siehe lwo_read
  obj_readTess__ obj_read__
  lwo_cb_saveFace und ds3_cb_saveFace  A%d=RCIR" ....
  wrl_readTess__

-----------------------------------------------------
Modifications:
2012-06-18 stl_read_bin new. RF.

-----------------------------------------------------
*/
/*!
\file  ../exp/stl_r.c
\brief read STL file
\code
=====================================================
List_functions_start:

gCad_main
stl_read__
stl_read_bin
stl_r_init
stl_r_addTri
stl_r_close

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


./go comp new tst3

cc -DOFFLINE stl_r.c ../ut/ut_os_aix.o ../ut/ut_TX.o../ut/ut_txt.o&&./a.out


see also: lwo_read ds3_read STP_r__ IGE_r__




-------------------------------------------------------------
Stereolithography format files

oriented triangles (facets)
Each facet is described by the unit outward normal and three points listed in counterclockwise order.
the direction of the normal is outward.
the vertices are listed in counterclockwise order when looking at the object from the outside (right-hand rule).
The object represented must be located in the all-positive octant (all vertex coordinates must be positive).




================= Format-Ascii: ========================
solid irgendwelcher Text ...
  facet normal  dx dy dz
    outer loop
      vertex  x y z   // 3 mal;
    endloop
  endfacet
endsolid CATIA STL

Trennzeichen ist blank





================= Format-binary: ========================
Binary STL files consist of a 80 byte header line that can be interpreted as a comment string.
The following 4 bytes interpreted as a long integer give the total number of facets.
What follows is a normal and 3 vertices for each facet, each coordinate represented as a 4 byte floating point number (12 bytes in all).
There is a 2 byte spacer between each facet.
The result is that each facet is represented by 50 bytes, 12 for the normal, 36 for the 3 vertices, and 2 for the spacer.





-------------------------------------------------------------
*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef _MSC_VER
// die folgenden Funktionen exportieren (werden vom Main gerufen):
__declspec(dllexport) int gCad_main (void*);
__declspec(dllexport) int obj_read__ (char*);
// nachfolgende externals werden aus dem Main-Exe imported:
#define extern __declspec(dllimport)
#endif


// #include <GL/gl.h>                     // GL_TRIANGLE_FAN
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_FAN                   0x0006


#include "../ut/ut_geo.h"
#include "../ut/ut_cast.h"             // INT_PTR

#include "../ut/ut_txt.h"
#include "../gr/ut_UI.h"                 // Typ_Att_Fac1




typedef struct {Memspc *impSpc; int impSiz;
                ObjGX *ox1; ObjGX *ox2; int oNr; void *pDat;}   Tessdat;
static Tessdat impDat;


typedef struct { Vectorf vcf; Pointf p1f, p2f, p3f; short is;}     stlRecBin;







//========================================================
//========================================================
#ifdef OFFLINE
int main () {

  Memspc wrkSpc;
  stl_read__ (&wrkSpc, "../dat/Zettler1.stl");
  return 0;

}
#endif





//================================================================
  int gCad_main (void *fdat) {
//================================================================
// import STL as tesselated; see also obj_read__
// Input:
// fdat besteht aus 3 objects;
// 1) Typ_Int4   mode; 1=work, 3=free.
// 2) Typ_Txt    filename
// 3) Typ_Memspc outSpc


  int     irc, mode;
  char    *fnam;
  ObjGX   *oTab;
  Memspc  *impSpc;

  // printf("gCad_main ex DLL xa_stl_r\n");
  // UTO_dump_s_ (fdat, "gCad_main ex DLL xa_WRL_R");

  oTab   = ((ObjGX*)fdat)->data;
  mode   = INT_PTR(oTab[0].data);  // 1) Typ_Int4   mode; 1=work, 3=free.
  fnam   = oTab[1].data;       // 2) Typ_Txt    filename
  impSpc = oTab[2].data;       // 3) Typ_Memspc outSpc

  // printf("gCad_main/xa_WRL_R mode=%d fnam=|%s|\n",mode,fnam);



  //----------------------------------------------------------------
  if(mode != 1) goto L_free;                 // WORK
  // printf(" OBJ-read-work\n");

  irc = stl_read__ (impSpc, fnam);
    printf("ex stl_read__ %d\n",irc);
  return irc;



  //----------------------------------------------------------------
  L_free:                                    // FREE
  if(mode != 3) return 0;
  // printf(" OBJ-read-Exit/free\n");
  return 0;

}


//================================================================
  int stl_read__ (Memspc *wrkSpc, char *fNam) {
//================================================================
// Input:
//   wrkSpc = Adress of Memspc-Record where to store the MainObject
//   fSiz = Filesize of stl-Ascii-Inputfile
// Output:
//   wrkSpc->start    the tesselated MainObject
// RetCod ?

 
  int     irc = 0, ii, fSiz;
  FILE    *fpi;
  char    cBuf[256], *cp1, *cp2;
  Point   pta[3];


  printf("stl_read__ |%s|\n",fNam);



  // get filesize
  fSiz = OS_FilSiz (fNam);
  if(fSiz < 1) {TX_Print("stl_read__ FileExist E001 %s",fNam); return -1;}
  // printf(" fSiz=%d\n",fSiz);


  irc = stl_read_bin (wrkSpc, fNam);
  if(irc < 0) return irc;
  if(irc > 0) return irc;           // binary loaded


  // open File
  if((fpi=fopen(fNam,"r")) == NULL) {
    TX_Print("****** stl_read__ OPEN ERROR |%s| **********\n",fNam);
    return -1;
  }


  irc = stl_r_init (wrkSpc, fSiz);
  if(irc < 0) goto L_err_eom;



  //----------------------------------------------------------------
  // read stl-ascii
  // erste zeile muss "solid " sein 
  fgets (cBuf, 250, fpi);
  if(strncmp (cBuf, "solid ", 6)) goto L_err1;


  while (!feof (fpi)) {
    if (fgets (cBuf, 250, fpi) == NULL) break;

    // remove leading blanks
    cp1 = cBuf;
    UTX_pos_skipLeadBlk (cp1);
      // UTX_CleanCR(cp1);printf(" next Line:|%s|\n",cp1);

    // "outer loop"   starts new facet (3Eck)
    if(!strncmp (cp1, "outer", 5)) {
      ii = 0;

    // vertex - next point
    } else if(!strncmp (cp1, "vertex", 6)) {
      cp1 += 7;
        // printf(" vert. %d |%s|\n",ii,cp1);

      pta[ii].x = strtod(cp1, &cp2);
      if(!cp2) goto L_err1;
      cp1 = cp2;

      pta[ii].y = strtod(cp1, &cp2);
      if(!cp2) goto L_err1;
      cp1 = cp2;

      pta[ii].z = strtod(cp1, &cp2);

      ++ii;
      if(ii == 3) {
        irc = stl_r_addTri (pta);
        if(irc < 0) goto L_err_eom;
      }

    }
  }

  L_fertig:

  irc = stl_r_close ();
  if(irc < -1) {TX_Print(" stl-import: empty ..."); goto L_exit;}
  if(irc < 0) goto L_err_eom;


  L_exit:
  fclose(fpi);
  return irc;


  //----------------------------------------------------------------
  L_err1:
  irc = -1;
  TX_Print("****** stl_read__ FORMAT ERROR |%s| **********\n",fNam);
  goto L_fertig;



  //----------------------------------------------------------------
  L_err_eom:
  TX_Print("****** stl_read__ EOM ERROR **********\n");
  goto L_exit;


}


//================================================================
  int stl_read_bin (Memspc *wrkSpc, char *fNam) {
//================================================================
// >0  OK, file loaded , returns nr of faces ..
//  0  ascii not binary
// -1  FileOpenError;

  unsigned int  nf;
  char      s1[512];
  int       i1, ii, irc;
  stlRecBin r1;
  FILE      *fp1;
  Point     pta[3];


  printf("stl_read_bin |%s|\n",fNam);


  fp1 = fopen (fNam, "rb");
  if(!fp1) {
    printf("stl_read file open error \n");
    return -2;
  }


  // die ersten 500 bytes laden; wenn darin facet vorkommt: ascii ..
  fread (s1, 1, 510, fp1);
  // fgets (s1, 510, fp1);
    // UTX_dump_c__ (s1, 200);

  if(UTX_memFind1 (s1, 510, "facet", 5)) {
    printf(" ascii ...\n");
    fclose(fp1);
    return 0;
  }
    

  rewind (fp1);


    // read header
    if (fread (s1, 1, 80, fp1) != 80) {
      TX_Print ("stl_read_bin E002");
      return -2;
    }
      printf(" header |%s|\n", s1);
    if(strncmp (s1, "solid ", 5)) {
      printf(" stl_read_bin E003\n");
      TX_Print ("stl_read_bin E003");
      return -3;
    }


    // read nr of facets
    ii = fread (&nf, 4, 1, fp1);
      printf(" %d %d\n",ii,nf);
    if(ii != 1) {
      TX_Print ("stl_read_bin E004");
      return -4;
    }

  // 3 Points + 3 ints
  i1 = (nf * 24 * 3)  + (nf * 4 * 3) + 1200;
  irc = stl_r_init (wrkSpc, i1);
    // printf(" i1=%d\n",i1);

  if(irc < 0) goto L_err_eom;



  // read all facets
  for(i1=0; i1 < nf; ++i1) {

    ii = fread (&r1, 50, 1, fp1);
    if(ii != 1) {
      printf(" ReadError in facet %d\n",i1);
      return -1;
    }
    // printf(" P%d = %f %f %f\n",i1,r1.p1f.x, r1.p1f.y, r1.p1f.z);
    pta[0].x = r1.p1f.x;
    pta[0].y = r1.p1f.y;
    pta[0].z = r1.p1f.z;

    pta[1].x = r1.p2f.x;
    pta[1].y = r1.p2f.y;
    pta[1].z = r1.p2f.z;

    pta[2].x = r1.p3f.x;
    pta[2].y = r1.p3f.y;
    pta[2].z = r1.p3f.z;

    irc = stl_r_addTri (pta);
    if(irc < 0) goto L_err_eom;


  }


  irc = stl_r_close ();
  if(irc < -1) {TX_Print(" stl-import: empty ..."); goto L_exit;}
  if(irc < 0) goto L_err_eom;



  L_exit:
  fclose(fp1);
  return nf;



  //----------------------------------------------------------------
  L_err_eom:
  TX_Print("****** stl_read_bin EOM ERROR **********\n");
  goto L_exit;

}



//================================================================
  int stl_r_init (Memspc *wrkSpc, int fSiz) {
//================================================================
// Input:
//   wrkSpc = Adress of Memspc-Record where to store the MainObject
//   fSiz = Filesize of stl-Ascii-Inputfile
// Output:
//   wrkSpc->start    the tesselated MainObject


//    (sizeRecord)                             ox1
//    210 143 2 a1               Typ_GL_Sura   ox2
// a2 (strut PT * 3)                           pDat
// a3 (strut PT * 3)                           ..
// a1   211 2 3 a2               Typ_GL_PP     rDat
//      211 2 3 a3


// soviel muss in impSpc immer frei sein in impSpc
#define impSpc_INC 250000


  int    irc;


  impDat.impSpc = wrkSpc;


  // structSize berechnen
  // impDat.impSiz = (impDat.oNr * sizeof(ObjGX))     +
           // (impDat.oNr * 3 * sizeof(Point)) + impSpc_INC;
  impDat.impSiz = fSiz + impSpc_INC;
    // printf(" impSiz=%d\n",impDat.impSiz);


  // init struct
  irc = UME_malloc (impDat.impSpc, impDat.impSiz, impSpc_INC);
  if(irc < 0) return -1;


  impDat.ox1 = impDat.impSpc->next;

  // (char*)impDat.ox2  = (char*)impDat.ox1 + sizeof(ObjGX);
  impDat.ox2  = (void*)((char*)impDat.ox1 + sizeof(ObjGX));

  // (char*)impDat.pDat = (char*)impDat.ox2 + sizeof(ObjGX);
  impDat.pDat = (char*)impDat.ox2 + sizeof(ObjGX);

  impDat.impSpc->next = impDat.pDat;


  // Create SizeRecord
  impDat.ox1->typ  = Typ_Size;
  impDat.ox1->form = Typ_Int4;
  impDat.ox1->siz  = 1;
  // hier als long spaeter die size setzen.


  // Create sur
  impDat.ox2->typ  = Typ_GL_Sur;
  impDat.ox2->form = Typ_ObjGX;


  impDat.oNr = 0;


  return 0;

}


//================================================================
  int stl_r_addTri (Point *pta) {
//================================================================
// Input:
//   pta   ein 3Eck aus 3 punkten


  int     irc, i1;
  long    l1;
  Point   *pTab;
  ObjGX   ox3;


  // printf(" stl_r_addTri %d\n",impDat.oNr);

  // // TEST-ONLY
  // Triangle   tri1;
  // tri1.pa[0] = &pta[0];  tri1.pa[1] = &pta[1];  tri1.pa[2] = &pta[2];
  // GR_Disp_tria (&tri1, 9);



  // space for 3 points
  pTab = impDat.impSpc->next;
  irc = UME_reall_add (&l1, impDat.impSpc, sizeof(Point) * 3);
  if(irc < 0) return -1;

  for(i1=0; i1<3; ++i1) {       // copy pointTable
    pTab[i1] = pta[i1];
    // UT3D_stru_dump (Typ_PT, &pTab[i1], "P[%d]:",i1);
  }

  ++impDat.oNr;


  return 0;

}


//================================================================
  int stl_r_close () {
//================================================================

  int    i1, irc;
  long   l1;
  Point  *pTab;
  ObjGX  ox3, *rTab;


  // die gesamte ObjGX-Tabelle erzeugen; Anzahl ist oNr
  if(impDat.oNr < 1) return -2;


  // ACHTUNG: bei einem realloc muss man relocaten ! (UTO_reloc_obj,
  // see wrl_reloc__
  rTab = (ObjGX*)impDat.impSpc->next;  // StartAdress ObjGX-Records
  irc = UME_reall_add (&l1, impDat.impSpc, sizeof(ObjGX) * impDat.oNr);
  if(irc < 0) return -1;




  // prepare  ObjGX-Record for this patch
  ox3.typ  = Typ_GL_PP;
  ox3.form = Typ_PT;
  ox3.aux  = GL_TRIANGLE_FAN;
  // ox3.aux  = GL_TRIANGLES;
  ox3.siz  = 3;


  pTab = (Point*)impDat.pDat;
  for(i1=0; i1<impDat.oNr; ++i1) {
    ox3.data = &pTab[i1*3];
    rTab[i1] = ox3;                      // write PP-Record
  }


  // write nr of patch-records
  impDat.ox2->siz  = impDat.oNr;
  impDat.ox2->data = rTab;               // position of ObjGX-table
    // printf(" 2.impDat.oNr=%d\n",impDat.oNr);



  // write size
  impDat.ox1->data = (void*)((long)(
                      (char*)impDat.impSpc->next
                      - (char*)impDat.ox1
                      - sizeof(ObjGX)));                    // - size-record
    // printf(" recSiz = %d\n",impDat.ox1->data);




  // Schlussrecord schreiben (Typ_Done)
  UTO_write_endRec (impDat.impSpc->next);



  // tess_dump_f_ (impDat.impSpc->start, "ex obj_readTess__");
  // UTO_dump_s_ (impDat.impSpc->start, "ex obj_readTess__");
  // UTO_dump__ (impDat.impSpc->start, "ex obj_readTess__");
  // l1 = -1;
  // GL_DrawSur(&l1, Typ_Att_Fac1, impDat.impSpc->start+sizeof(ObjGX));
  // TSU_DrawSurTess (&l1, Typ_Att_Fac1, impDat.impSpc->start);



  return 0;

}


//===========  eof ============================================
