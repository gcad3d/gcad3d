//        Import WaveFront-OBJ-Files;        Franz Reiter   2005-03-23
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
2013-08-15 obj_r_face,obj_r_f__: skip faces with 2 vertices (from blender). RF.
2009-11-26 obj_r_face() can now load "f 1//1 2//2 3//3"      RF
2005-03-23 first version. RF.

-----------------------------------------------------
*/
/*!
\file  ../exp/obj_ut.c
\brief read OBJ file (WaveFront) 
\code
=====================================================
List_functions_start:

gCad_main            read WaveFront-OBj-File (for mockup)
obj_readTess__       read WaveFront-OBj-File (for mockup)
obj_read__           read WaveFront-OBj-File (for model)
obj_r_v__            read vertex (for mockup)
obj_r_f__            read face (for model)
obj_r_vert           read vertex, change orientation
obj_r_face           read face (for mockup)
obj_r_f_r            read 3 od 4 indices (for face)
obj_r_mtllib         read .mtl-file; build material-table.
obj_r_usemtl__       activate color (material) for following faces

List_functions_end:
=====================================================

\endcode *//*----------------------------------------




---------------------------------------------------------
Spec OBJ-File:
---------------------------------------------------------
Filetypes:
  .obj = ascii;
  .mod = binary.
  .mtl = material-library; ascii.

------------ .obj - spec: ---------------
# Kommentarzeile


v xCoord yCoord zCoord
# vertex-coords; orientation: cad-z = obj-y; cad-y = obj-z * -1



f vertexNr1 vertexNr2 vertexNr3 [vertexNr4]
# Face with 3 or 4 vertices. Vertexdelimiter is a blank
# f 1//1 2//2 3//3    indexes to vertexes/textureCoords/normals


vt   TexturVertices
vn   VertexNormalvektoren
vp   parameter space vertices   (f Freiformkurven)
g    group name
f    Face
surf surface

o name
# user-defined object name for all following objects

s off
# smoothing group; 0 or off is of; else group-nr.

# define material-library
mtllib xx.mtl

# activate material with matl-name "red" for all following faces:
usemtl red
# All colors are in extra-file (material-library); <fnam>.mtl
# Each face can have a different material.

UNUSED:
p  point
sp special-point
l  line




------------ .mtl - spec: ---------------
Format material-file .mtl:

newmtl Material.001
# mtl-nam kann auch leer sein !

Ns 96.078431
# specular exponent, 0 to 1000

Ka 0.000000 0.000000 0.000000
# ambient reflectivity using RGB values.

Kd 0.800000 0.612323 0.438305
# diffuse reflectivity using RGB values

Ks 0.500000 0.463523 0.478091
# specular reflectivity using RGB values

Ni 1.000000
# optical_density, 0.001 to 10.

d 0.810647
# 0.0=completely_transparent; 1.0=fully_opaque

illum 2
# illumination model;



*/



// #ifdef _MSC_VER                        // nur fuer die gl.h
// #include <windows.h>
// #endif

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
#define GL_TRIANGLE_FAN                   0x0006


#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_txt.h"              // UTX_db_tx
#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_txTab.h"            // TxtTab

#include "../xa/xa_mem.h"              // memspc51, mem_cbuf1
// MS-Win:
// static char mem_cbuf1[512];
// static int mem_cbuf1_SIZ = 512;



//================================================================
// ex ../xa/xa.c
extern char    AP_mod_dir[128];     // directory for OPEN
extern ColRGB  AP_defcol;



//================================================================
static int     vNr;

static ObjGX   ooxp, ooxs;
static Point   opt;

static int     colNr;         // nr of defined colors (colNam, colTab)
static int     colInd;        // index to active color (colNam, colTab)
static ColRGB  colAct;

static Memspc  *impSpc;

static UtxTab_NEW (colNam);                // stringtable
// static TxtTab  colNam;                // stringtable

typedef_MemTab(ColRGB);
static MemTab(ColRGB) colTab = _MEMTAB_NUL;



//================================================================
  int gCad_main (void *fdat) {
//================================================================
// import OBJ as tesselated; see also obj_read__
// Input:
// fdat besteht aus 3 objects;
// 1) Typ_Int4   mode; 1=work, 3=free.
// 2) Typ_Txt    filename
// 3) Typ_Memspc outSpc


  int     irc, mode;
  char    *fnam;
  ObjGX   *oTab;


  printf("gCad_main ex  xa_obj_r.dll\n");
  // DEB_dump_ox_s_ (fdat, "gCad_main ex DLL xa_WRL_R");

  oTab   = ((ObjGX*)fdat)->data;
  mode   = INT_PTR(oTab[0].data);  // 1) Typ_Int4   mode; 1=work, 3=free.
  fnam   = oTab[1].data;           // 2) Typ_Txt    filename
  impSpc = oTab[2].data;           // 3) Typ_Memspc outSpc

  // printf("gCad_main/xa_WRL_R mode=%d fnam=|%s|\n",mode,fnam);



  //----------------------------------------------------------------
  if(mode != 1) goto L_free;                 // WORK
  // printf(" OBJ-read-work\n");

  irc = obj_readTess__ (fnam);
  // printf("ex wrl_readTess__ %d\n",irc);
  return irc;



  //----------------------------------------------------------------
  L_free:                                    // FREE
  if(mode != 3) return 0;
  // printf(" OBJ-read-Exit/free\n");
  return 0;

}



//================================================================
  int obj_read__ (char *fnam) {
//================================================================
// read WaveFront-OBj-File
// see also obj_readTess__

  int  i1, oCnt;
  long ioffP;
  char cbuf1[256];
  FILE *fpi;

  printf("========================================\n");
  printf("obj_read__ |%s|\n",fnam);


  ooxp.typ  = Typ_PT;
  ooxp.form = Typ_PT;
  ooxp.siz  = 1;
  ooxp.data = &opt;

  ooxs.typ  = Typ_SURCIR;
  ooxs.form = Typ_Txt;
  ooxs.data = cbuf1;   // hier Zusatztext

  vNr  = 0;
  oCnt = 0;



  // Zwischenspeicher UTF_FilBuf1 loeschen
  UTF_clear1 ();

  // clear stringtable colNam
  UtxTab_init__ (&colNam);

  // clear colortable colTab
  MemTab_ini (&colTab, sizeof(ColRGB), Typ_Color, 30);


  sprintf(mem_cbuf1,"# WaveFront-OBJ-Import %s",fnam);
  UTF_add1_line (mem_cbuf1);


  ioffP = DB_dbo_get_free (Typ_PT);    // P
    // printf(" ioffP=%ld\n",ioffP);


  // Init Objektindexe fuer AP_obj_2_txt-Funktion
  AP_obj_2_txt (NULL, 0L, NULL, 0L);

  // als offset erforderlich
  ioffP = DB_dbo_get_free (Typ_PT);    // P
    printf(" ioffP=%ld\n",ioffP);

  colInd = -1;
  colNr  = -1;


  if ((fpi = fopen (fnam, "r")) == NULL) {
    TX_Print("****** OPEN ERROR FILE %s **********",fnam);
    return -1;
  }


  while (!feof (fpi)) {
    if(fgets (cbuf1, 256, fpi) == NULL) break;
    if(cbuf1[0] == '#') continue;

    // display progress ..
    i1 = vNr / 1000;
    if(i1 != oCnt) {
      oCnt = i1;
      UI_winTX_rmLast ();
      TX_Print("    %d Objekte importiert ..",oCnt*1000);
    }

    // work ..
    if(!strncmp(cbuf1, "v ", 2)) {obj_r_v__(cbuf1); continue;}
    if(!strncmp(cbuf1, "f ", 2)) {obj_r_f__(cbuf1,ioffP); continue;}
    if(!strncmp(cbuf1, "usemtl ", 7)) {obj_r_usemtl__(cbuf1,ioffP); continue;}
    if(!strncmp(cbuf1, "mtllib ", 7)) {obj_r_mtllib(cbuf1); continue;}

    // unused:
    // printf(" uu: %s",cbuf1);

  }



  fclose (fpi);


  if(colNr >= 0) {
    UtxTab_free (&colNam);
    MemTab_free (&colTab);
  }


  sprintf(mem_cbuf1,"#");
  UTF_add1_line (mem_cbuf1);


  // UTF_FilBuf1 an die Hauptdatei (UTF_FilBuf0, nur memory!) anfuegen
  UTF_insert1 (-1L);


  return 0;

}


//================================================================
  int obj_readTess__ (char *fnam) {
//================================================================
// WaveFront-OBj-File einlesen und tesselated Surf generieren.
// 2 Durchlaufe:
//  1. Durchlauf: alle Punkte speichern      (v xCoord yCoord zCoord)
//  2. Durchlauf: alle Facelines abarbeiten. (f iv1 iv2 iv3 [iv4])
// see also obj_read__

  int   irc, i1, vSiz, vNr, fNr, pNr, fNxt, impSiz;
  long  fSiz, l1, ia[4];
  char  cbuf1[256];
  void  *pp;
  ObjGX *ox1, *ox2, ox3, oxc;
  Point *vTab = NULL, *pTab;
  FILE   *fpi;
  ColRGB col1;

// soviel muss in impSpc immer frei sein in impSpc
#define impSpc_INC 250000

// incr siz f vTab
#define vTab_INC  10000



  printf("obj_readTess__\n");


  // get filesize
  fSiz = OS_FilSiz (fnam);
  if(fSiz < 1) {TX_Print("wrl_readTess__ FileExist E001 %s",fnam); return -1;}
  printf(" fSiz=%ld\n",fSiz);


  if((fpi=fopen(fnam,"rb")) == NULL) {
    TX_Print("wrl_readTess__ Open E001 %s",fnam);
    return -1;
  }



  //----------------------------------------------------------------
  vNr  = 0;
  fNr  = 0;
  vSiz = 0;

  colInd = -1;
  colNr  = -1;



  //================================================================
  //  1. run: get all vertices into vTab; get Recordnumber: count faces+Colors.
  while (!feof (fpi)) {
    if(fgets (cbuf1, 256, fpi) == NULL) break;
    if(!strncmp(cbuf1, "mtllib ", 7)) {obj_r_mtllib(cbuf1); continue;}
    if(!strncmp(cbuf1, "f ", 2)) {++fNr; continue;}  // get nr of faces
    if(!strncmp(cbuf1, "usemtl ", 7)) {++fNr; continue;} // add color-records
    if(strncmp(cbuf1, "v ", 2)) continue;
    // vertex --> opt
    obj_r_vert (cbuf1);
    // opt --> vTab
    if(vNr >= vSiz) {
      vSiz += vTab_INC;
      vTab = (Point*)realloc(vTab, vSiz * sizeof(Point));
      if(vTab == NULL) {TX_Error("obj_readTess__ EOM"); return -1;}
    }
    vTab[vNr] = opt;
    // DEB_dump_obj__ (Typ_PT, &vTab[vNr], "V[%d]:",vNr);
    ++vNr;
  }
  rewind(fpi);
  printf(" 1.vNr=%d fNr=%d\n",vNr,fNr);

  if((vNr < 1)||(fNr < 1)) {
    TX_Print("ERROR: file %s is empty .. ",fnam);
    fclose (fpi);
    if(vTab) free(vTab);
    return -1;
  }




  //================================================================
  // init struct impSpc
  // impSiz = impSpc_INC * 2;
  impSiz = (fNr * sizeof(ObjGX))     +
           (fNr * 4 * sizeof(Point)) + impSpc_INC;
  // groesser (4 statt 3) damits kein realloc gibt ..


  irc = UME_malloc (impSpc, impSiz, impSpc_INC);
  if(irc < 0) goto L_exit;


  L_nxt_run:
  ox1 = impSpc->next;
  ox2 = (void*)((char*)ox1 + sizeof(ObjGX));
  pp  = (char*)ox2 + sizeof(ObjGX);


  // Create SizeRecord
  ox1->typ  = Typ_Size;
  ox1->form = Typ_Int4;
  ox1->siz  = 1;


  // Create sur
  ox2->typ  = Typ_GL_Sur;
  ox2->form = Typ_ObjGX;
  ox2->data = pp;


  // PP-Records anlegen
  impSpc->next = pp;
  // ACHTUNG: bei einem realloc muss man relocaten ! (UTO_reloc_obj,
  // see wrl_reloc__
  irc = UME_reall_add (&l1, impSpc, sizeof(ObjGX) * fNr);
  if(irc < 0) goto L_exit;


  // prepare
  ox3.typ  = Typ_GL_PP;
  ox3.form = Typ_PT;
  ox3.aux  = GL_TRIANGLE_FAN;

  oxc.typ  = Typ_Color;
  oxc.form = Typ_Int4;
  oxc.siz  = 1;



  //================================================================
  // read obj-file; add faces;
  fNr = 0;
  while (!feof (fpi)) {
    if(fgets (cbuf1, 256, fpi) == NULL) break;
    if(!strncmp(cbuf1, "usemtl ", 7)) {
      // get color-index into colInd
      obj_r_usemtl__ (cbuf1);
      // get color -> col1
      if(colInd < 0) col1 = AP_defcol;
      else col1 = colTab.data[colInd];
      // add color-record
      oxc.data = (void*)(*((long*)&col1));
      ((ObjGX*)pp)[fNr] = oxc;   // write PP-Record
      ++fNr;
      continue;
    }
    if(strncmp(cbuf1, "f ", 2)) continue;
    // faceIndices --> ia
    i1 = obj_r_face (ia, cbuf1);
    if(i1 < 0) continue;
    if(i1 == 0) pNr = 3;
    else        pNr = 4;

    // get memspc for the data-record
    pTab = impSpc->next;
    irc = UME_reall_add (&l1, impSpc, sizeof(Point) * pNr);
    if(irc < 0) goto L_exit;

    for(i1=0; i1<pNr; ++i1) {       // write pointTable
      pTab[i1] = vTab[ia[i1]];
      // DEB_dump_obj__ (Typ_PT, &pTab[i1], "P[%d]:",i1);
    }

    ox3.siz  = pNr;
    ox3.data = pTab;
    ((ObjGX*)pp)[fNr] = ox3;   // write PP-Record
    ++fNr;
    // if(fNr >= UT_I2_MAX) break; // pro Record max I2MAX faces ausgeben
  }

  ox2->siz = fNr;

  printf(" 2.fNr=%d\n",fNr);




  //================================================================
  // size setzen ..
  ox1->data = (void*)((long)((char*)impSpc->next - (char*)ox1 -
              sizeof(ObjGX)));  // - size-record
  printf(" recSiz = %d\n",INT_PTR(ox1->data));


  // alle ?
  if (!feof (fpi)) goto L_nxt_run;


  // Schlussrecord schreiben
  UTO_write_endRec (impSpc->next);


  // tess_dump_f_ (impSpc->start, "ex obj_readTess__");
  // DEB_dump_ox_s_ (impSpc->start, "ex obj_readTess__");
  // DEB_dump_ox_0 (impSpc->start, "ex obj_readTess__");
  // l1 = -1;
  // GL_DrawSur(&l1, Typ_Att_dash_long, impSpc->start+sizeof(ObjGX));
  // TSU_DrawSurTess (impSpc->start);




  L_exit:
  fclose (fpi);
  if(vTab) free(vTab);

  return 0;

}


//================================================================
  int obj_r_v__ (char *cbuf) {
//================================================================
// normaler Vertex
 
  int   irc;


  obj_r_vert (cbuf);

  
  // Objekt in Text umwandeln und mit UTF_add1_line speichern
  // irc = AP_obj_2_txt (mem_cbuf1, mem_cbuf1_SIZ, &ooxp, -1);


  irc = AP_stru_2_txt (mem_cbuf1, mem_cbuf1_SIZ, &ooxp, -1L);
  if(irc < 0) return irc;


  irc = UTF_add1_line (mem_cbuf1); // add Lines -> Aux.Buffer
  if(irc < 0) return irc;



  ++vNr;

  return 0;

}


//================================================================
  int obj_r_f__ (char *cbuf, int ioffP) {
//================================================================
// face aus 3 od 4 indices  (for model)
// see also obj_r_face (for mockup)

  int    irc, i1;
  char   *p1, *p2;
  long   dbi, ia[4];
  ColRGB col1;


// OFFEN: use obj_r_face hier ...

  // printf("obj_r_f__ |%s| %d\n",cbuf,ioffP);


  p1 = &cbuf[2];

  irc = obj_r_f_r (ia, p1);
  if(irc < 0) return irc;


  for(i1=0; i1<4; ++i1) ia[i1] += ioffP;

  if(irc) {
    sprintf(cbuf, " P%ld P%ld P%ld P%ld",ia[0],ia[1],ia[2],ia[3]);
      // printf("f %ld %ld %ld %ld\n",l1,l2,l3,l4);

  } else {

    sprintf(cbuf, " P%ld P%ld P%ld",ia[0],ia[1],ia[2]);
  }

    // printf(" cbuf=|%s|\n",cbuf);

  ooxs.siz = strlen(cbuf);

  // Objekt in Text umwandeln und mit UTF_add1_line speichern
  irc = AP_obj_2_txt (mem_cbuf1, mem_cbuf1_SIZ, &ooxs, -1L);
  if(irc < 0) return irc;

  ++vNr; // progress ..


  // apply color
  AP_obj_2_txt_query (&i1, &dbi);   // get db-typ, db-ind
    // printf(" created: %d %ld\n",i1,l1);

  i1 = AP_typ_2_bastyp (i1); // get basic-type
  if(i1 == Typ_SUR) {
    // test if a user-defined-color is active
    if(colInd >= 0) {
      // apply color 
      APcol_actColTra (&colTab.data[colInd]);
      GA_Col__ (-1L, 0, Typ_SUR, dbi);
    }
  }


  return 0;

}


//================================================================
  int obj_r_usemtl__ (char *cbuf) {
//================================================================
/// activate color (material) for following faces ..

  int    i1;


  cbuf += 7;
  UTX_CleanCR (cbuf);

  // printf("obj_r_usemtl__ |%s|\n",cbuf);

  // find name in nametable colNam
  for(i1=0; i1 < UtxTab_nr(&colNam); ++i1) {
      // printf(" |%s|\n",UtxTab__(i1,&colNam));
    if(!strcmp(cbuf,UtxTab__(i1,&colNam))) {
      goto L_found;
    }
  }

  i1 = -1;

  L_found:
    colInd = i1;

    // printf(" _usemtl_ %d\n",colInd);


  return 0;

}


//================================================================
  int obj_r_mtllib (char *cbuf) {
//================================================================
/// obj_r_mtllib     read .tl-file; build material-table.

  int     i1;
  long    ld;
  char    s1[256], *p1, *p2;
  double  d1, d2, d3;
  FILE    *fpi;
  ColRGB  col1;


  cbuf += 7;
  UTX_CleanCR (cbuf);

  // printf("obj_r_mtllib |%s|\n",cbuf);

  sprintf(s1, "%s%s",AP_mod_dir,cbuf);
    // printf(" s1=|%s|\n",s1);

  if((fpi=fopen(s1,"r")) == NULL) {
    TX_Print("**** CANNOT OPEN Materialfile %s",s1);
    return -1;
  }

  while (!feof (fpi)) {
    if (fgets (s1, 250, fpi) == NULL) break;
    UTX_CleanCR (s1);


    if(!strncmp(s1, "newmtl ", 7)) {
      p1 = &s1[7];
      if(strlen(p1) < 2) {
        colInd = -1;
        continue; // skip default-color
      }
      if(colNr < 0) {
        UtxTab_init__ (&colNam);
        MemTab_ini (&colTab, sizeof(ColRGB), Typ_Color, 30);
        colNr = 0;
      }
      UtxTab_add (&colNam, p1);
      colInd = colNr;
      ++colNr;
      Col_set__ (&col1, 0, 0, 0); // init col1
      MemTab_sav (&colTab, &ld, &col1, 1);
      continue;
    }



    if(!strncmp(s1, "d ", 2)) {
      // d: 0.0=completely_transparent; 1.0=fully_opaque
      if(colInd < 0) continue; // skip default-color
      p1 = &s1[2];
      d1 = UTX_db_tx (&p2, p1);
      if(d1 > 0.9) continue;  // not  transp     0.9 - 1.0
      if(d1 > 0.4) i1 = 1;    // semi transp  T1 0.4 - 0.9
      else         i1 = 2;    // full transp  T2 0.0 - 0.4
      colTab.data[colInd].vtra = i1;
        // printf(" set transp=%d\n",i1);
      continue;
    }


    if(!strncmp(s1, "Kd ", 3)) {
      // Kd: diffuse reflectivity using RGB values
      if(colInd < 0) continue; // skip default-color
      p1 = &s1[3];
      d1 = UTX_db_tx (&p2, p1);
      d2 = UTX_db_tx (&p2, p2);
      d3 = UTX_db_tx (&p2, p2);
      Col_set_3db (&colTab.data[colInd], d1, d2, d3);
      continue;
    }


  }

  if(fpi) {fclose(fpi); fpi = NULL;}

  colInd = -1;

  // UtxTab_dump (&colNam);
  // for(i1=0;i1<colNr;++i1)
    // DEB_dump_obj__(Typ_Color,&colTab.data[i1]," col%d=",i1);

  return 0;

}


//================================================================
  int obj_r_vert (char *cbuf) {
//================================================================
// used by obj_r_v__ (Model) and obj_readTess__ (Mockup).

// normaler Vertex; decodieren --> opt

  char   *p1, *p2;
  double ptz;

  p1 = &cbuf[2];

  // gcad-x = obj-x
  opt.x = UTX_db_tx (&p1, p1); ++p1;
  // gcad-z = obj-y
  opt.z = UTX_db_tx (&p1, p1); ++p1;
  // gcad-y = obj-z * -1
  opt.y = UTX_db_tx (&p1, p1) *-1.; ++p1;


    // DEB_dump_obj__ (Typ_PT, &opt, "ex obj_r_vert");

  return 0;

}


//================================================================
  int obj_r_face (long *ia, char *cbuf) {
//================================================================
// face aus 3 od 4 indices  (for mockup)
// see also obj_r_f__ (for model)

// Input:
//   f 1//1 2//2 3//3
// indexes to vertexes/textureCoords/normals
// delimiter is a blank


  int   irc, i1;
  char  *p1;


  // p1 = &cbuf[2];
  // UTX_CleanCR (p1);

  // printf("obj_r_face |%s|\n",cbuf);

  p1 = &cbuf[2];
    
  irc = obj_r_f_r (ia, p1);
  if(irc < 0) return irc; 

  for(i1=0; i1<4; ++i1) ia[i1] -= 1;

  return irc;

}


//================================================================
  int obj_r_f_r (long *ia, char *cbuf) {
//================================================================
/// \code
/// read 3 od 4 indices 
/// see also obj_r_f__ (for model)
/// retCode:  -1 less than 3 vertices;
///            0   3 vertices
///            1   4 vertices
/// \endcode

// Input:  (f -line)
//   1 2 3
//   1//1 2//2 3//3
// delimiter is a blank


  char  *p1, *p2;


  UTX_CleanCR (cbuf);
  p1 = cbuf;
    
  // printf("obj_r_f_r |%s|\n",p1);

  ia[0] = strtol (p1, &p2, 10);
  p1 = strchr(p2, ' ');
  ++p1; 

  ia[1] = strtol (p1, &p2, 10);


  // blender provides faces with only 2 vertices; skip it ..  2013-08-15
    // printf(" p2=|%s|\n",p2);
  p1 = strchr(p2, ' ');
  if(!p1) return -1;
  ++p1;

  ia[2] = strtol (p1, &p2, 10);
  p1 = strchr(p2, ' ');

  if(p1) {
    ++p1;
    ia[3] = strtol (p1, &p2, 10);

      // printf("ex obj_r_f_r %ld %ld %ld %ld\n",ia[0],ia[1],ia[2],ia[3]);
    return 1;  // 4 Werte
  }


    // printf("ex obj_r_f_r %ld %ld %ld\n",ia[0],ia[1],ia[2]);
  return 0;    // 3 Werte
}


//============= EOF ==============================================
