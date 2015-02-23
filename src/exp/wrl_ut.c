//    Read VRML-1-Files.               Franz Reiter     2005-04-01
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
2006-01-05 Transformationen nochmal ganz neu. RF.

-----------------------------------------------------
*/
/*!
\file  ../exp/wrl_ut.c
\brief read VRML-1 file
\code
=====================================================
List_functions_start:

wrl_readTess__
wrl1_r_dec1           main ?
wrl1_r_dec_ifs        IndexedFaceSet
wrl1_r_skip_words
wrl1_dump_wa
wrl_reloc__
wrl1_r_out_end
wrl1_r_skipBlock
wrl1_r_skipLev
wrl1_r_dec_iTab
wrl1_r_dec_vTab
wrl1_r_dec_Col1
wrl1_r_dec_Col2
wrl1_r_dec_Col_
wrl1_r_dec_DEF
wrl1_r_dec_USE
wrl1_r_dec_trl
wrl1_r_dec_scf
wrl1_r_dec_MatTra
wrl1_r_dec_matrix
wrl1_r_dec_Trf
wrl1_r_dec_Scale
wrl1_r_trm_Trl
wrl1_r_trm_upd
wrl1_r_ck_col
wrl1_r_dec_col
wrl1_r_dec_sph
wrl1_r_dec_cyl

List_functions_end:
=====================================================

\endcode *//*----------------------------------------





==========================================================================
# Comment  .. bis zum EOL ('\n')
Trennzeichen fuer Worte sind nur blank oder EOL
SFBool: 0 = FALSE, 1 = TRUE
string   "He said, \"Immel did it!\""


Separator { .. }
TransformSeparator { .. }
 execute am ende ...  aber eigentlich nix mehr to do !


TransformSeparator { .. }
  soll Gruppe mit gleicher Transformationsmatrix isolieren; deprecated weil
  das der normale Separator auch tun muss.


DEF <name> <nodeType> { <body> }
 execute and mark for reUse
 Position von Name in Tabelle defTab speichern; den Rest abarbeiten.


USE <name>
 reUse (execute)
 <nmae> in defTab suchen; <name> im DEF-Record skippen und den Rest ausfuehren
 (als Rekursion).




LOD { range [ r1 r2 r3 .. ] Sep { } Sep { } Sep { } }
 Innerhalb eines LOD-Blocks gibt es mehrere Darstellungen des Objekts;
 die erste ist am detailliertesten.
 Im LOD-Block: Status LOD=ON aktivieren; beim Verlassen des Level levLOD
 skip alle weiteren Blocks der gleichen Stufe.


Material { diffuseColor 0.6939 0.6111 0.5211
           ambientColor  0.800000  0.800000  0.800000
           specularColor 0.22 0.27 0.27
           emissiveColor 0 0 0
           transparency  0.0
}
# Meist nur diffuseColor, manchmal beide. Weiters auch emissiveColor,
#   shininess und transparency



---------------------------------------------------------
MatrixTransform { matrix .... }


Translation { translation  0 0 0  }    # Verschiebung; dx, dy, dz


Rotation { rotation  0 0 1  3.14159265 }
# Drehung { Achse Verdrehwinkel}
#  the amount of right-handed rotation about that axis, in radians)


Scale { scaleFactor 0.05 0.05 0.05 }


Transform {
    translation T1
    rotation R1
    scaleFactor S
    scaleOrientation R2
    center T2
  }


is equivalent to the sequence:

Translation { translation T1 }
Translation { translation T2 }
Rotation { rotation R1 }
Rotation { rotation R2 }
Scale { scaleFactor S }
Rotation { rotation -R2 }
Translation { translation -T2 }

Coordinate3 { point [ <points> ] }


---------------------------------------------------------
OBJECTS:   sofort ausfuehren !

IndexedFaceSet { coordIndex [ .. ] }

Cube { x_size y_siz z_siz }

Cone { which_parts bottomRadius height }

Sphere { radius <r> }                             // defRad = 1

Cylinder { which_parts radius <r> height <h> }
  centered around the y-axis
     SIDES   The cylindrical part
     TOP     The top circular face
     BOTTOM  The bottom circular face
     ALL     All parts

AsciiText {
          string         ""    # MFString
          spacing        1     # SFFloat
          justification  LEFT  # SFEnum
          width          0     # MFFloat
     }

Group { .. }
 ignore ..


===========================================================================
===========================================================================
Programmablauf:
APT_decode_mock
APT_DrawModel
 DLL_run1
  gCad_main
   wrl_readTess__     alle obj. aus Datei nach outSpc kopieren
 TSU_DrawSurTess





*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// NUR TEST:  GEHT NICHT MIT MS_WIN !!??
// #include <GL/gl.h>
// #include <GL/glu.h>



#ifdef _MSC_VER
// die folgenden Funktionen exportieren (werden vom Main gerufen):
__declspec(dllexport) int gCad_main (void*);
// nachfolgende externals werden aus dem Main-Exe imported:
#define extern __declspec(dllimport)
#endif


// #include <GL/gl.h>                     // GL_TRIANGLE_FAN
#define GL_TRIANGLE_FAN                   0x0006



#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_txt.h"              // UTX_pos_skipDeli1
#include "../ut/ut_cast.h"             // INT_PTR

#include "../gr/ut_UI.h"               // UI_Func... SYM_..

#include "../xa/xa_mem.h"              // memspc51, mem_cbuf1



extern Point     *GLT_pta;
extern long      GLT_pta_SIZ;








#define MAXLEV 64



static char    *ppt;   // point
static char    *pci;   // coordIndex

static Point   *vTab=NULL;       // memspc501
static int     *iTab=NULL;       // memspc201
static int     vTabSiz, iTabSiz, vTabNr, iTabNr;

static Mat_4x3 trMat;
static Vector  trVc;
static double  trSc[3];
static int     trMatStat, trVcStat, trScStat;

static char    **defTab;
static int     defTabSiz, defTabNr;

static int     level, useLevTab[12], useLevNr, lodLevel, iTest;

static ColRGB  newCol, actCol;

static int     err0Nr;


// soviel muss in outSpc immer frei sein
#define outSpc_FRE 500000

#define iTab_INC  50000


static Memspc  *outSpc;
static long    outSpc_INC;



typedef struct {Mat_4x3 ma; int stat;}                    maRec;

static maRec  *maTab;





//================================================================
  int gCad_main (void *fdat) {
//================================================================
// import VRML as tesselated; see also obj_read__
// Input:
// fdat besteht aus 3 objects;
// 1) Typ_Int4   mode; 1=work, 3=free.
// 2) Typ_Txt    filename
// 3) Typ_Memspc outSpc


  int     irc, mode;
  char    *fnam;
  ObjGX   *oTab;


  // printf("gCad_main ex DLL xa_WRL_R\n");
  // return 0;  // TEST
  // UTO_dump_s_ (fdat, "gCad_main ex DLL xa_WRL_R");

  oTab   = ((ObjGX*)fdat)->data;
  mode   = INT_PTR(oTab[0].data); // 1) Typ_Int4   mode; 1=work, 3=free.
  fnam   = oTab[1].data;          // 2) Typ_Txt    filename
  outSpc = oTab[2].data;          // 3) Typ_Memspc outSpc

  // printf("gCad_main/xa_WRL_R mode=%d fnam=|%s|\n",mode,fnam);



  //----------------------------------------------------------------
  if(mode != 1) goto L_free;                 // WORK
    // printf(" WRL-read-work\n");

    irc = wrl_readTess__ (fnam);
      // printf("ex wrl_readTess__ %d\n",irc);
    return irc;



  //----------------------------------------------------------------
  L_free:                                    // FREE
  if(mode != 3) return 0;
    if(iTab) free (iTab);
    iTab = NULL;
    if(outSpc->start) UME_free (outSpc);
    // printf(" WRL-read-Exit/free\n");
    return 0;

}


//================================================================
  char* wrl1_r_skip_words (int inr, char *p1) {
//================================================================
// skip entweder inr words oder if next char is [ skip whole block !
// oder if next char is '~' 

  char   *p2, *p3;


  p2 = UTX_pos_skipDeli1 (p1); // skip delimiter ' ' ',' '\n' '\r' '\t'




  // printf("wrl1_r_skip_words %d |",inr);
  // UTX_dump_s__ (p2);
  // printf("|\n");





  if(*p2 == '~') {    // zB ambientColor  ~
   ++p2;
   return p2;
  }


  if(*p2 == '[') {   // zB emissiveColor  [ 0.5 0.5 0.7 ] 
   p3 = strchr(p2, ']');
   if(p3 == NULL) {TX_Error("wrl1_r_skip_words E001"); return NULL;}
   ++p3;
   return p3;
  }


  p2 = UTX_pos_skipWords (inr, p1);

  return p2;
}


//================================================================
  int wrl1_dump_wa (int wNr, char *wa[]) {
//================================================================

  int   i1;

  for(i1=0; i1<wNr; ++i1) {
    printf(" wa[%d]=|",i1);UTX_dump_s__ (wa[i1], 40); printf("|\n");
  }

  return 0;

}
 

//================================================================
  int wrl_reloc__ (long addSiz) {
//================================================================
// see also tess_reloc_f_

  int    irc;
  long   rSiz, pDist;
  long   l1;
  void   *oldPos;
  ObjGX  *ox1;


  // printf("wrl_reloc__ %d\n",addSiz);

  oldPos = outSpc->start;

  // hier free space anfragen (damit unbedingt vergroessert wird) else loop!
  irc = UME_reall_add (&l1, outSpc, 0);
  // irc = UME_reall_add (outSpc, 250000);
  if(irc < 0) return irc;
  // (char*)outSpc->next -= 250000;  // war nur Test ob genug Platz ..


  // neue position ?
  ox1 = outSpc->start;
  if((char*)ox1 == (char*)oldPos) return 0;


  printf(" >>>>>>>>>>>>>>>>>>>>>>>>> wrl_reloc__\n");
  pDist = (char*)ox1 - (char*)oldPos;


  //================================================================
  L_next:
  if(ox1->typ  == Typ_Done)  goto L_fertig;

  // erster Record muss size of following Record sein
  if(ox1->typ  != Typ_Size) goto L_Err2;
  rSiz = (long)ox1->data;
  // printf("Record size=%d\n",rSiz);

  // (char*)ox1 += sizeof(ObjGX);
  ox1 = (void*)((char*)ox1 + sizeof(ObjGX));

  // relocate record
  UTO_reloc_obj (ox1, pDist);
  // UTO_dump__ (ox1, "nach UTO_reloc_obj");

  // (char*)ox1 += rSiz;
  ox1 = (void*)((char*)ox1 + rSiz);

  if((void*)ox1 < outSpc->next) goto L_next;


  L_fertig:
  return 0;


  L_Err1:
    TX_Error("wrl_reloc__ E001 form %d",ox1->form);
    return -1;

  L_Err2:
    TX_Error("wrl_reloc__ E002 typ %d",ox1->typ);
    return -1;

}


//================================================================
  int wrl1_r_out_end () {
//================================================================
// Schlussrecord schreiben

  UTO_write_endRec (outSpc->next);

  // ObjGX *ox1;
  // ox1 = outSpc->next;
  // ox1->typ  = Typ_Done;
  // ox1->form = Typ_Int4;
  // ox1->siz  = 1;
  // ox1->data = (void*)0L;

  return 0;

}



//================================================================
  int wrl1_r_skipBlock (char **ps, char *pe) {
//================================================================
// derzeitige Posi: '{'
// skip active Ebene (goto next '}')

  int  iLev;
  char *p1;


  p1   = *ps;
  iLev = 1;

    // printf("wrl1_r_skipBlock %d |",level);
    // UTX_dump_s__ (p1, 50);
    // printf("|\n");


  L_next:
  ++p1;

  if(p1 >= pe) {TX_Error("wrl1_r_skipBlock E001"); return -1;}

  if(*p1 == '{')  {++iLev; goto L_next;}
  if(*p1 == '}')  {--iLev; if(iLev < 1) goto L_found;}
  goto L_next;


  //----------------------------------------------------------------
  L_found:
  // printf("  skipBlock len=%d\n",p1-*ps);
  ++p1;

  *ps = p1;

    // printf("ex wrl1_r_skipBlock %d |",level);
    // UTX_dump_s__ (p1, 50);
    // printf("|\n");

  return 0;

}



//================================================================
  int wrl1_r_skipLev (char **ps, char *pe) {
//================================================================
// derzeitige Posi: '}'
// skip eine Ebene (goto next '}')

  int  iLev;
  char *p1;


  p1   = *ps;
  iLev = 1;

    // printf("wrl1_r_skipLev %d |",level);
    // UTX_dump_s__ (p1, 200);
    // printf("|\n");


  L_next:
  ++p1;
  if(p1 >= pe) {TX_Error("wrl1_r_skipLev E001"); return -1;}
  if(*p1 == '{')  {++iLev; goto L_next;}
  if(*p1 == '}')  {--iLev; if(iLev < 1) goto L_found;}
  goto L_next;

  //----------------------------------------------------------------
  L_found:
  // printf("  skipLev len=%d\n",p1-*ps);
  --p1;

  *ps = p1;

    // printf("ex wrl1_r_skipLev %d |",level);
    // UTX_dump_s__ (p1, 200);
    // printf("|\n");

  return 0;

}


//================================================================
  int wrl1_r_dec_iTab () {
//================================================================
// indices=pci.
// die Indices --> memInd
// returns Anzahl faces = fNr
// wird nur von IndexedFaceSet gerufen

  int   fNr;
  char  *p1, *p2, *pe;


  // printf("wrl1_r_dec_iTab %d\n",iTabNr);
  // printf("wrl1_r_dec_iTab |");
  // UTX_dump_s__ (pci);
  // printf("|\n");


  iTabNr = 0;
  fNr = 0;
  p1  = pci;
  pe  = strchr(p1, ']');
  if(pe == NULL) {TX_Error("wrl1_r_dec_iTab E001 %d",iTabNr); return -1;}


  L_nxt_ind:

  // iTab[iTabNr] = strtod (p1, &p2); // geht auch ...
  iTab[iTabNr] = strtol (p1, &p2, 10);
  // printf("  iTab[%d]=%d\n",iTabNr,iTab[iTabNr]);
  p1 = p2; ++p1;

  if(iTab[iTabNr] < 0) ++fNr;

  ++iTabNr;
  if(iTabNr >= iTabSiz) {
    iTabSiz  += iTab_INC;
    printf(" realloc iTab %d\n",iTabSiz);
    iTab = (int*)realloc (iTab, iTabSiz * sizeof(int));
    if(iTab == NULL) {
      TX_Error("wrl1_r_dec_iTab EOM %d %d",iTabNr,iTabSiz);
      return -1;
    }
  }


  p1 = UTX_pos_skipDeli1 (p1);  // skip delimiter ' ' ',' '\n' '\r' '\t'


  if(p1 < pe) goto L_nxt_ind;


  // printf("ex wrl1_r_dec_iTab %d\n",fNr);
  return fNr;

}


//================================================================
  int wrl1_r_dec_vTab () {
//================================================================
// points=ppt
// die Punkte --> memPT

  char  *p1, *p2, *pe;
  Point pt1;


  // printf("wrl1_r_dec_vTab |");
  // UTX_dump_s__ (ppt);
  // printf("|\n");


  p1 = UTX_pos_skipDeli1 (ppt);  // skip delimiter ' ' ',' '\n' '\r' '\t'
  vTabNr = 0;
  // p1  = ppt;
  pe  = strchr(p1, ']');
  if(pe == NULL) {TX_Error("wrl1_r_dec_vTab E001 %d",vTabNr); return -1;}

  L_nxt_pt:

  pt1.x = UTX_db_tx (&p1, p1); ++p1;
  pt1.y = UTX_db_tx (&p1, p1); ++p1;
  pt1.z = UTX_db_tx (&p1, p1); ++p1;
    // UT3D_stru_dump (Typ_PT, &pt1, "  vTab[%d]",vTabNr);

  vTab[vTabNr] = pt1;
  if(vTabNr < vTabSiz) ++vTabNr;
  else {TX_Error("wrl1_r_dec_vTab E002"); return -1;}

  p1 = UTX_pos_skipDeli1 (p1);  // skip delimiter ' ' ',' '\n' '\r' '\t'

  if(p1 < pe) goto L_nxt_pt;

  // printf("ex wrl1_r_dec_vTab %d\n",vTabNr);
  return 0;

}


//================================================================
  int wrl1_r_dec_Col1 (char *wa[]) {
//================================================================
// store defined color ...

  char   *p1;
  double d1, d2, d3;

  // printf("wrl1_r_dec_Col1 |%s|%s|%s|\n",wa[0],wa[1],wa[2]);

  d1 = UTX_db_tx (&p1, wa[0]);
  d2 = UTX_db_tx (&p1, wa[1]);
  d3 = UTX_db_tx (&p1, wa[2]);

  wrl1_r_dec_Col_ (d1, d2, d3);

  return 0;

}


//================================================================
  int wrl1_r_dec_Col2 (char *p1) {
//================================================================
// store defined color ...

  char   *p2;
  double d1, d2, d3;

  // printf("wrl1_r_dec_Col2 |%s|\n",p1);

  d1 = UTX_db_tx (&p2, p1);
  d2 = UTX_db_tx (&p2, p2);
  d3 = UTX_db_tx (&p2, p2);

  wrl1_r_dec_Col_ (d1, d2, d3);

  return 0;

}


//================================================================
  int wrl1_r_dec_Col_ (double d1, double d2, double d3) {
//================================================================
// store defined color ...

  // printf("wrl1_r_dec_Col_ d1=%f d2=%f d3=%f\n",d1,d2,d3);

  newCol.cr = d1 * 255;
  newCol.cg = d2 * 255;
  newCol.cb = d3 * 255;

  // 0 als Col ist Default - geht ned !!
  if(newCol.cr < 1) newCol.cr = 1;
  if(newCol.cg < 1) newCol.cg = 1;
  if(newCol.cb < 1) newCol.cb = 1;

  // newCol.typ = 0;   // diffuseColor
  newCol.color = 1;
  newCol.vtra  = 0;
  newCol.vsym  = 0;
  newCol.vtex  = 0;

  // printf(" newCol=%d %d %d\n",newCol.cr,newCol.cg,newCol.cb);

  return 0;

}


//================================================================
  int wrl1_r_dec_DEF (char *wa[]) {
//================================================================
// name in defTab speichern

  int   i1, lc;
  char  *p1, cbuf1[128];


  // printf("wrl1_r_dec_DEF %d |",defTabNr);
  // UTX_dump_s__(wa[0]);
  // printf("|\n");


  UTX_cp_word__(cbuf1, wa[0]);
  lc = strlen(cbuf1);

  for(i1=0; i1<defTabNr; ++i1) {
    // printf(" %d=|%s|\n",i1,defTab[i1]);
    if(!strncmp(cbuf1,defTab[i1],lc)) {
      p1 = defTab[i1];
      if(UTX_ckc_Del1 (p1[lc]) == 0) {
        // printf(" -------|%s| already stored as %d\n",cbuf1,i1);
        return 0;
      }
    }
  }

  if(defTabNr >= defTabSiz) {TX_Error("wrl1_r_dec_DEF E001"); return -1;}

  defTab[defTabNr] = wa[0];

  ++defTabNr;

  return 0;

}


//================================================================
  int wrl1_r_dec_USE (char *wa[], char *pe) {
//================================================================
// USE: execute DEF-Record; dann weiter ..

  int   i1, lc;
  char  *p1, cbuf[80];

  UTX_cp_word__(cbuf, wa[0]);
  lc = strlen(cbuf);

  // printf("wrl1_r_dec_USE |%s| lev=%d %d\n",cbuf,level,defTabNr);
  // for(i1=0; i1<defTabNr; ++i1) printf(" %d=|%s|\n",i1,defTab[i1]);

 

  // zugehoerigen Block suchen ...
  for(i1=0; i1<defTabNr; ++i1) {
    if(!strncmp(defTab[i1], cbuf, lc)) {
      p1 = defTab[i1];
      if(UTX_ckc_Del1 (p1[lc]) != 0) continue;
      goto L_work;
    }
  }
  TX_Print("wrl1_r_dec_USE E001");
  return -1;


  L_work:
  // printf(" wrl1_r_dec_USE pos |%s| %d\n",p1,i1);
  p1 += lc + 1;  // skip <name>

    // printf("-- wrl1_r_dec_USE |");
    // UTX_dump_s__ (p1, 26);
    // printf("|\n");


  // useLevel = level + 1;        // exit at this level 
  if(useLevNr >= 12) {
    TX_Print("wrl1_r_dec_USE E002");
    return -1;
  }

  useLevTab[useLevNr] = level;
  ++useLevNr;

  // maTab[level].stat = 0;
  // --level;  // wird dr. wrl1_r_dec1 erhoeht
  // maTab[level].stat = 0;


  i1 = wrl1_r_dec1 (&p1, pe);     // RECURSE
  --useLevNr;
  --level;  // wird dr. wrl1_r_dec1 erhoeht

  // printf(" return from USE lev=%d %d\n",useLevNr,level);

  return i1;

  // return 0;

}


//================================================================
   int wrl1_r_dec_trl (char *wa[]) {
//================================================================
// Translation { translation dx dy dz }

  char  *p1;

  if(trScStat == 0) {
    trVc.dx = UTX_db_tx  (&p1, wa[0]);
    trVc.dy = UTX_db_tx  (&p1, wa[1]);
    trVc.dz = UTX_db_tx  (&p1, wa[2]);
  } else {
    trVc.dx += UTX_db_tx  (&p1, wa[0]);
    trVc.dy += UTX_db_tx  (&p1, wa[1]);
    trVc.dz += UTX_db_tx  (&p1, wa[2]);
  }

  trVcStat = 1;

  // printf("ex wrl1_r_dec_trl %f %f %f\n",trVc.dx,trVc.dy,trVc.dz);

  return 0;

}


//================================================================
   int wrl1_r_dec_scf (char *wa[]) {
//================================================================
// scaleFactor

  char  *p1;

  if(trScStat != 0) printf("wrl1_r_dec_scf I001\n");


  trSc[0] = UTX_db_tx  (&p1, wa[0]);
  trSc[1] = UTX_db_tx  (&p1, wa[1]);
  trSc[2] = UTX_db_tx  (&p1, wa[2]);


  trScStat = 1;

  // printf("ex wrl1_r_dec_scf %f %f %f\n",trSc[0],trSc[1],trSc[2]);

  return 0;

}


//================================================================
   int wrl1_r_dec_MatTra () {
//================================================================
// MatrixTransform {matrix .. .. }
// Werte ex matrix sind in trMat

  Mat_4x3  newMat;


  if(trMatStat == 0) return 0;

    // UT3D_stru_dump (Typ_M4x3, &maTab[level-1].ma,
                    // "wrl1_r_dec_MatTra old %d\n",level-1);


  UT3D_m3_tra_m3 (newMat, trMat, maTab[level].ma);

  UT3D_m3_copy (maTab[level].ma, newMat);

  maTab[level].stat = 0;         // als veraendert markieren !

  // UT3D_stru_dump (Typ_M4x3, &maTab[level].ma,
                     // "ex wrl1_r_dec_MaTra %d\n",level);

  trMatStat = 0;

  return 0;

}


//================================================================
   int wrl1_r_dec_matrix (char *wa[]) {
//================================================================
// matrix .. ..   // nur Matrixwerte -> trmat laden.

  // int   i1;
  char     *p1;


  // printf("wrl1_r_dec_matrix lev=%d\n",level);
  // int i1;for(i1=0; i1<16; ++i1) printf(" wa[%d]=%s\n",i1,wa[i1]); geht ned !

  if(trMatStat != 0) {
    printf("wrl1_r_dec_matrix I001\n");
    // UT3D_stru_dump (Typ_M4x3, trMat, "wrl1_r_dec_matrix I001\n");
  }



  // VX
  trMat[0][0] = UTX_db_tx  (&p1, wa[0]);
  trMat[1][0] = UTX_db_tx  (&p1, wa[1]);
  trMat[2][0] = UTX_db_tx  (&p1, wa[2]);

  // VY
  trMat[0][1] = UTX_db_tx  (&p1, wa[4]);
  trMat[1][1] = UTX_db_tx  (&p1, wa[5]);
  trMat[2][1] = UTX_db_tx  (&p1, wa[6]);

  // VZ
  trMat[0][2] = UTX_db_tx  (&p1, wa[8]);
  trMat[1][2] = UTX_db_tx  (&p1, wa[9]);
  trMat[2][2] = UTX_db_tx  (&p1, wa[10]);

  // center
  trMat[0][3] = UTX_db_tx  (&p1, wa[12]);
  trMat[1][3] = UTX_db_tx  (&p1, wa[13]);
  trMat[2][3] = UTX_db_tx  (&p1, wa[14]);


  trMatStat = 1;

  // UT3D_stru_dump (Typ_M4x3, trMat, "ex wrl1_r_dec_matrix:\n");

  return 0;

}


//================================================================
  int wrl1_r_dec_Trf () {
//================================================================
// Transform

  wrl1_r_trm_Trl ();

  wrl1_r_dec_Scale ();

  return 0;

}


//================================================================
  int wrl1_r_dec_Scale () {
//================================================================
// translate active Scale
// activate mit wrl1_r_trm_upd

  int       i1;
  Vector    v1;

  if(trScStat == 0) return 0;

  // printf("wrl1_r_trm_Trl %d %f %f %f\n",level,trSc[0],trSc[1],trSc[2]);
  // UT3D_stru_dump (Typ_M4x3, maTab[level].ma, "wrl1_r_dec_Scale:\n");


  UT3D_m3_get ((double*)&v1, 0, maTab[level].ma);
  v1.dx *= trSc[0];
  v1.dy *= trSc[0];
  v1.dz *= trSc[0];
  UT3D_m3_set (maTab[level].ma, 0, &v1);

  UT3D_m3_get ((double*)&v1, 1, maTab[level].ma);
  v1.dx *= trSc[1];
  v1.dy *= trSc[1];
  v1.dz *= trSc[1];
  UT3D_m3_set (maTab[level].ma, 1, &v1);

  UT3D_m3_get ((double*)&v1, 2, maTab[level].ma);
  v1.dx *= trSc[2];
  v1.dy *= trSc[2];
  v1.dz *= trSc[2];
  UT3D_m3_set (maTab[level].ma, 2, &v1);

  trScStat = 0;

  // und die Matrix als veraendert markieren
  maTab[level].stat = 0;

  // UT3D_stru_dump (Typ_M4x3,maTab[level].ma,"ex wrl1_r_dec_Scale %d\n",level);

  return 0;

}

 
//================================================================
  int wrl1_r_trm_Trl () {
//================================================================
// translate active Matrix       "Translation"
// activate mit wrl1_r_trm_upd

  int       i1;
  Mat_4x3   *actMat;

  if(trVcStat == 0) return 0;

  // printf("wrl1_r_trm_Trl %d\n",level);
  // UT3D_stru_dump (Typ_M4x3, maTab[level].ma, "wrl1_r_trm_Trl:\n");


  // move origin
  UT3D_m3_traVc (&maTab[level].ma, &trVc);
  trVcStat = 0;

  // und die Matrix als veraendert markieren
  maTab[level].stat = 0;

  // UT3D_stru_dump (Typ_M4x3, maTab[level].ma, "ex wrl1_r_trm_Trl %d\n",level);

  return 0;

}


//================================================================
  int wrl1_r_trm_upd () {
//================================================================
// update transformation-matrix
// es soll die aktuelle Matrix mit UTRA_def__ geladen werden.

static int levOld = -1;

  int      i1;
  Vector   vx;

  // printf("  wrl1_r_trm_upd lev=%d stat=%d old=%d\n",level,
             // maTab[level].stat,levOld);



  if((level != levOld)||(maTab[level].stat != 1)) {

    // UT3D_stru_dump (Typ_M4x3, maTab[level].ma, "wrl1_r_trm_upd %d\n",level);

    UTRA_def__ (1, Typ_TraMat, maTab[level].ma);
    maTab[level].stat = 1;

    // den scale errechnen:
    UT3D_m3_get ((double*)&vx, 0, maTab[level].ma);
    trSc[0] = UT3D_len_vc (&vx);
    // printf(" xScal=%f\n",trSc[0]);

    levOld = level;

  }


  return 0;

}

//================================================================
  int wrl1_r_ck_col () {
//================================================================
// irc = 0 - wenn keine neue Colour definiert ist
// irc = 1 - neue Farbe aktivieren.



  if(strncmp((void*)&actCol,(void*)&newCol,sizeof(ColRGB)))
    return 1;
  else 
    return 0;

}


//================================================================
  int wrl1_r_dec_col (ObjGX **aox) {
//================================================================
// irc = 0 - wenn keine neue Colour definiert ist
// irc = 1 - neue Farbe aktivieren.


  ObjGX    *oxc;


  // return, wenn gleich.
  if(wrl1_r_ck_col() == 0) return 0;


  // printf(" NEW COLOUR:\n");
  // UT3D_stru_dump (Typ_Color, &newCol, "wrl1_r_dec_col: ");

  *((int*)&actCol) = *((int*)&newCol);

  oxc = *aox;

    oxc->typ  = Typ_Color;
    oxc->form = Typ_Int4;
    oxc->siz  = 1;
    // oxc->data = (void*)((long)newCol);
    // (long)oxc->data = *((long*)&newCol);
    oxc->data = (void*)(*((long*)&newCol));

  // (char*)*aox += sizeof(ObjGX);
  *aox = (ObjGX*)((char*)*aox + sizeof(ObjGX));

  return 1;
}


//================================================================
  int wrl1_r_dec_sph (double radius) {
//================================================================
// Sphere

  int      i1, iCol;
  Sphere   sph1;
  Point    ptc={0.,0.,0.};
  void     *dat;
  ObjGX    *ox1, *ox2, *ox3;


  wrl1_r_trm_upd ();     // update transformation-matrix


  // printf("wrl1_r_dec_sph %f\n",radius);

  radius *= trSc[0];

  // struct fuer sphere befuellen
  sph1.rad = radius;

  // if(maTab[level].stat > 0)
    UTRA_app_pt (&sph1.pc, &ptc);

    // UT3D_stru_dump (Typ_PT, &ptc, " ptc");
    // GR_Disp_pt (&sph1.pc, SYM_STAR_S, 2);


  iCol = wrl1_r_ck_col();

  ox1 = outSpc->next; // Startpos


  // ev hier (fixe) size testen; wenn erforderl: realloc ....
  // irc = UME_reall_add (outSpc, sizeof(ObjGX) * fNr, outSpc_INC);
  i1 = ((3 + iCol) * sizeof(ObjGX)) + sizeof(Sphere);
  if(!UME_reserve(outSpc, i1)) {
    TX_Error("wrl1_r_dec_sph E001 %d",i1);
    return -1;
  }



  // Create SizeRecord
  ox1->typ  = Typ_Size;
  ox1->form = Typ_Int4;
  ox1->siz  = 1;            // noch offen ..



  // Create sur
  // (char*)ox2 = (char*)ox1 + sizeof(ObjGX);
  ox2 = (void*)((char*)ox1 + sizeof(ObjGX));
  // (char*)ox3 = (char*)ox2 + sizeof(ObjGX);
  ox3 = (void*)((char*)ox2 + sizeof(ObjGX));
  ox2->typ  = Typ_GL_Sur;
  ox2->form = Typ_ObjGX;
  ox2->siz  = 1 + iCol;     // 1 or 2 patches: Colour + Sphere
  ox2->data = ox3;  // patchPosition


  // Farbe --> ox3, ox3 um sizeof(ObjGX) weitersetzen ..
  if(iCol != 0) wrl1_r_dec_col (&ox3);


  // Create sur
  // (char*)dat = (char*)ox3 + sizeof(ObjGX);
  dat = (void*)((char*)ox3 + sizeof(ObjGX));
  ox3->typ  = Typ_SPH;
  ox3->form = Typ_SPH;
  ox3->siz  = 1;
  ox3->data = dat;


  // copy sphere
  memcpy (dat, &sph1, sizeof(Sphere));


  // size setzen ..
  i1 = (char*)outSpc->next - (char*)ox2;
  // (long)ox1->data = (long)i1;
  ox1->data = (void*)((long)i1);
  // printf(" recSiz = %d\n",ox1->data);
  // if(i1 < 96) {TX_Error("wrl1_r_dec_sph E001 %d",i1); return -1;}



  return 0;

}


//================================================================
  int wrl1_r_dec_cyl (double radius, double height) {
//================================================================
 
  int      i1, iCol;
  Point    ptc1={0.,0.,0.}, ptc2={0.,0.,0.};
  Conus    con1;
  Vector   vz;
  void     *dat;
  ObjGX    *ox1, *ox2, *ox3;


  wrl1_r_trm_upd ();     // update transformation-matrix

  // printf("wrl1_r_dec_cyl %f %f\n",radius,height);
  // printf("   scale=%f\n",trSc[0]);

  radius *= trSc[0];
  height *= trSc[0];


  ptc1.y = -(height / 2.);
  ptc2.y = height / 2.;
  // UT3D_stru_dump (Typ_PT, &ptc1, " ptc1:");
  // UT3D_stru_dump (Typ_PT, &ptc2, " ptc2:");

  // if(maTab[level].stat > 0) {
    UTRA_app_pt (&ptc1, &ptc1);
    UTRA_app_pt (&ptc2, &ptc2);
  // }

  // vz.dx = 0.; 
  // vz.dy = 0.; 
  // vz.dz = 1.; 
  // UT3D_pt_traptvclen (&ptc1, &ptc1, &vz, -height/2.);



  // UT3D_stru_dump (Typ_PT, &ptc1, " ptc1:");
  // UT3D_stru_dump (Typ_PT, &ptc2, " ptc2:");
  // GR_Disp_pt (&ptc1, SYM_STAR_S, 2);
  // GR_Disp_pt (&ptc2, SYM_STAR_S, 2);


  // UT3D_stru_dump (Typ_PT, &ptc1, " ptc1");
  // UT3D_stru_dump (Typ_PT, &ptc2, " ptc2");
  // GR_Disp_ln1 (&ptc1, &ptc2, 2);


  // struct fuer Konus befuellen (pl, r1, r2, h)
  UT3D_vc_2pt  (&vz, &ptc1, &ptc2);
  UT3D_pl_ptvc (&con1.pl, &ptc1, &vz);
  con1.r1 = radius;
  con1.r2 = radius;
  con1.h  = height;
  

  iCol = wrl1_r_ck_col();

  ox1 = outSpc->next; // Startpos


  // ev hier (fixe) size testen; wenn erforderl: realloc ....
  // irc = UME_reall_add (outSpc, sizeof(ObjGX) * fNr, outSpc_INC);
  i1 = ((3 + iCol) * sizeof(ObjGX)) + sizeof(Conus);
  if(!UME_reserve(outSpc, i1)) {
    TX_Error("wrl1_r_dec_cyl E001 %d",i1);
    return -1;
  }


  // Create SizeRecord
  ox1->typ  = Typ_Size;
  ox1->form = Typ_Int4;
  ox1->siz  = 1;            // noch offen ..



  // Create sur
  // (char*)ox2 = (char*)ox1 + sizeof(ObjGX);
  ox2 = (void*)((char*)ox1 + sizeof(ObjGX));
  // (char*)ox3 = (char*)ox2 + sizeof(ObjGX);
  ox3 = (void*)((char*)ox2 + sizeof(ObjGX));
  ox2->typ  = Typ_GL_Sur;
  ox2->form = Typ_ObjGX;
  ox2->siz  = 1 + iCol;     // 1 or 2 patches: Colour + Sphere
  ox2->data = ox3;  // patchPosition


  // Farbe --> ox3, ox3 um sizeof(ObjGX) weitersetzen ..
  if(iCol != 0) wrl1_r_dec_col (&ox3);


  // Create sur
  // (char*)dat = (char*)ox3 + sizeof(ObjGX);
  dat = (void*)((char*)ox3 + sizeof(ObjGX));
  ox3->typ  = Typ_CON;
  ox3->form = Typ_CON;
  ox3->siz  = 1;
  ox3->data = dat;


  // copy obj
  memcpy (dat, &con1, sizeof(Conus));


  // size setzen ..
  i1 = (char*)outSpc->next - (char*)ox2;
  // (long)ox1->data = (long)i1;
  ox1->data = (void*)((long)i1);
  // printf(" recSiz = %d\n",ox1->data);
  // if(i1 < 96) {TX_Error("wrl1_r_dec_sph E001 %d",i1); return -1;}


  return 0;

}


//================================================================
  int wrl1_r_dec_ifs () {
//================================================================
// work IndexedFaceSet; points=ppt; indices=pci.

  int   irc, iCol, i1, i2, i3, ip1, ip2, fNr, pNr;
  long  addSpc;
  void  *pp;
  ObjGX *ox1, *ox2, ox3;
  Point *pTab;


  wrl1_r_trm_upd ();     // update transformation-matrix

  // printf("wrl1_r_dec_ifs lev=%d\n",level);


  // die Punkte --> memPT
  wrl1_r_dec_vTab ();

  // die Indices --> memInd
  fNr = wrl1_r_dec_iTab ();
  if(fNr < 1) return fNr;

  goto L_write;



  //================================================================
  // es ist zuwenig memspc avail. Memspc vergroessern und dann alles nochmal
  // ausgeben.
  L_Rewrite:
  outSpc->next = ox1;  // reset spc
  wrl1_r_out_end ();   // nur fuers relocate
  // reallocate
  irc = wrl_reloc__ (addSpc);
  if(irc < 0) goto L_exit;



  //================================================================
  // ausgeben --> outSpc
  L_write:

  // Create SizeRecord
  ox1 = outSpc->next;
  ox1->typ  = Typ_Size;
  ox1->form = Typ_Int4;
  ox1->siz  = 1;


  iCol = wrl1_r_ck_col();


  // Create sur
  // (char*)ox2 = (char*)ox1 + sizeof(ObjGX);
  ox2 = (void*)((char*)ox1 + sizeof(ObjGX));
  // (char*)pp  = (char*)ox2 + sizeof(ObjGX);
  pp  = (char*)ox2 + sizeof(ObjGX);
  ox2->typ  = Typ_GL_Sur;
  ox2->form = Typ_ObjGX;
  ox2->siz  = fNr + iCol;
  ox2->data = pp;


  // Farbe --> pp, pp um sizeof(ObjGX) weitersetzen ..
  // Color vor den PP_Records raus
  if(iCol != 0) wrl1_r_dec_col ((ObjGX**)&pp);


  // PP-Records anlegen fuer fNr faces.
  outSpc->next = pp;
  // irc = UME_reall_add (outSpc, sizeof(ObjGX) * fNr, outSpc_INC);
  addSpc = sizeof(ObjGX) * fNr;
  if(!UME_reserve(outSpc, addSpc)) goto L_Rewrite;


  // prepare
  ox3.typ  = Typ_GL_PP;
  ox3.form = Typ_PT;
  ox3.aux  = GL_TRIANGLE_FAN;


  // PP- und datenRecords ausgeben
  ip2 = -1;
  for(i1=0; i1<fNr; ++i1) {

    ip1 = ip2 + 1;
    L_nxt_pt:
    ++ip2;
    if(iTab[ip2] >= 0) goto L_nxt_pt;

    // occupy memspc for pointData
    pNr = ip2 - ip1;          // ohne terminating -1
    // printf(" next face %d - %d (%d)\n",ip1,ip2,pNr);
    pTab = outSpc->next;
    // irc = UME_reall_add (outSpc, sizeof(Point) * pNr, outSpc_INC);
    addSpc = sizeof(Point) * pNr;
    if(!UME_reserve(outSpc, addSpc)) goto L_Rewrite;


    // copy the points 
    i3 = 0;
    for(i2=ip1; i2<ip2; ++i2) {       // write pointTable
      // if(trStat == 0)
        // pTab[i3] = vTab[iTab[i2]];
      // else
        UTRA_app_pt (&pTab[i3], &vTab[iTab[i2]]);
      // UT3D_stru_dump (Typ_PT, &pTab[i3], "P %d %d %d",i3,iTab[i2],i2);
      ++i3;
    }


    ox3.siz  = pNr;
    ox3.data = pTab;
    ((ObjGX*)pp)[i1] = ox3;   // write PP-Record
  }


  // size setzen ..
  i1 = (char*)outSpc->next - (char*)ox2;
  // (long)ox1->data = (long)i1;
  ox1->data = (void*)((long)i1);
  // printf(" recSiz = %d\n",ox1->data);
  if(i1 < 96) {TX_Error("wrl1_r_dec_ifs E001 %d",i1); return -1;}


  // Ende nur fuers dump erforderl !
    // wrl1_r_out_end ();
    // tess_dump_f_ (ox1, "ex wrl1_r_dec_ifs");
    // UTO_dump_s_ (ox2, "ex wrl1_r_dec_ifs");


  L_exit:
  return irc;

}


//================================================================
  int wrl1_r_dec1 (char **ps, char *pe) {
//================================================================

# define wMax 24

// alle folgenden Worte koennen einen Block { .. } haben ..
static char *wTab1[]={
 "Separator","TransformSeparator",                  //  0,1   execute
 "Coordinate3",                                     //  2     Attribs
 "MatrixTransform","Transform",                     //  3,4   Attribs
 "Rotation","Scale","Translation",                  //  5-7   Attribs
 "IndexedFaceSet","IndexedLineSet","PointSet",      //  8,10  Objects
 "Sphere","Cone","Cube","Cylinder","AsciiText",     // 11-15  Objects
 "Shape",                                           // 16
 "Normal","Material","Info","ShapeHints",           // 17-    skip
 "NormalBinding","MaterialBinding","PointLight",
 "PerspectiveCamera","Group","Switch","LOD",
 "Node_Info","Node_info",
 ""};

// arraydata (word [ .. data .. ])
static char *wTab2[]={
 "point","coordIndex","range","diffuseColor",
 ""};
// unused: vector normalIndex


// funcs ohne [] 
static char *wTab3[]={
 "matrix","translation","radius","diffuseColor",
 ""};
// unused: center


static  double radius = 1.;
static  double height = 1.;


  int     irc, i1, ii, wNr, funcNr;
  char    *p1, *p2, *p3;
  char    *wa[wMax], cbuf1[80];





  L_start_block:


  p1  = *ps;
  while (*p1 == ' ')  { ++p1; }


  // printf("+++++++++++++ wrl1_r_dec1 %d |",level+1);
  // UTX_dump_s__ (p1,32); printf("|\n");



  // skip unused blocks1
  //                    12345678901234567
  if     (!strncmp(p1, "Header_Info",       11)) goto L_skip_block1;
  else if(!strncmp(p1, "Node_info",          9)) goto L_skip_block1;
  else if(!strncmp(p1, "PerspectiveCamera", 17)) goto L_skip_block1;
  goto L_decode;


  L_skip_block1:
      // printf(" skip block |");UTX_dump_s__ (*ps,32); printf("|\n");
    p1 = strchr(p1, '{');
    if(p1 == NULL) {printf("**** wrl1_r_dec1 E456\n"); return -2;}
    wrl1_r_skipBlock (&p1, pe);
    ++p1;
    *ps = p1;
    goto L_start_block;



  //================================================================
  L_decode:
  ++level;
  if(level > MAXLEV) {TX_Print("wrl1_r_dec1 E_lev"); return -1;}

  // if(maTab[level].stat < 0) {
  if(level > 1)
    UT3D_m3_copy (&maTab[level].ma, &maTab[level-1].ma);
  else
    UT3D_m3_copy (&maTab[level].ma, UT3D_MAT_4x3);
  // }
  // printf(" copy maTab[%d] = maTab[%d]\n",level,level-1);
  maTab[level].stat = 0;
  // for(i1=0;i1<MAXLEV;++i1) maTab[i1].stat = 0;


  wNr = 0;


  //================================================================
  L_next_word:
  if(p1 >= pe) return 0;
  if(*p1 == '\0') {TX_Error("wrl1_r_dec1 E000"); return -1;}
  if(*p1 == ' ') {++p1; goto L_next_word;}
  if(*p1 == ',') {++p1; goto L_next_word;}   // 2010-02-25 cadenas-problem !
    // printf(">>>>>> L_next_word: %d |",pe-p1);UTX_dump_s__(p1,32);printf("|\n");




  //----------------------------------------------------------------
  // Ende Klammerausdruck -
  if(*p1 == '}') {
    // am Ende eines LOD-Block: alle weiteren Blocks skippen ...
    if(lodLevel >= 0) {
      if(level == lodLevel) {
        wrl1_r_skipLev (&p1, pe);
        lodLevel = -1;
        goto L_blockend9;
      }
    }
/*
    if(wNr > 0) {
      if(!strncmp(wa[0], "USE", 3)) {     // { USE <name> }
        irc = wrl1_r_dec_USE (&wa[1], pe);
        if(irc < 0) return irc;
        goto L_blockend9;
      }
    }
*/
    // // hier oft Crash:
    // printf("CrashPos %d |",pe-p1);UTX_dump_s__(p1);printf("|\n");


    // word in wa ist nicht terminiert;
    if(wNr > 0) {
      UTX_cp_word__(cbuf1, wa[0]);
      i1 = UTX_cmp_word_wordtab (wTab3, cbuf1);
      if(i1 == 0) {         // matrix
        wrl1_r_dec_matrix (&wa[1]);
      }
    }

    // ende bearbeitung
    L_blockend9:
    ++p1;
    *ps = p1;
    --level;
    // printf("--------- exit dec1 } lev=%d\n",level);
    return 0;
  }


  //----------------------------------------------------------------
  // string
  if(*p1 == '\"') {
    L_str_nxt:
    ++p1;
    p2 = strchr(p1, '\"');
    if(p2 == NULL) {TX_Error("wrl1_r_dec1 E001"); return -1;}
    if(p2 >= pe) {TX_Error("wrl1_r_dec1 E002"); return -1;}
    p3 = p2 - 1;
    p1 = p2;
    if(*p3 == '\\') goto L_str_nxt;
    ++p1;
    goto L_next_word;
  }


  //----------------------------------------------------------------
  // arrayTyp [ ... ]
  if(*p1 == '[') {
    // printf(" Arr|");UTX_dump_s__ (p1); printf("|\n");
    p2 = strchr(p1, ']');
    if(p2 == NULL) {
      p2 = strchr(p1, '\0');
    // } else {
      // *p2 = '\0';
    }
    if(p2 >= pe) {TX_Error("wrl1_r_dec1 E004"); return -1;}
    // Test if arrayData is needed ..
    // printf(" check arrayTyp %d |%s|\n",wNr-1,wa[wNr-1]);
    if(wNr < 1) {TX_Error("wrl1_r_dec1 E006"); return -1;}
    UTX_cp_word__(cbuf1, wa[wNr-1]); // word in wa ist nicht terminiert;
    // printf("  Arrfunc=|%s|\n",cbuf1);
    i1 = UTX_cmp_word_wordtab (wTab2, cbuf1);
    // printf("   %d ex UTX_cmp_word_wordtab |%s|\n",i1,cbuf1);
    if(i1 == 0) {  // point
      ppt = p1 + 1;
      // printf(" sav point |%s|\n",ppt);
    } else if(i1 == 1) {  // coordIndex
      pci = p1 + 1;
      // printf(" sav coordIndex |%s|\n",pci);
    } else if(i1 == 2) {  // range
      // skippen (f LOD erforderl !!)
      --wNr;
    } else if(i1 == 3) {  // "diffuseColor [..]"  nur die ersten 3 words nehmen
      ++p1; // skip '['
      wrl1_r_dec_Col2 (p1);
      --wNr;
    } else {
      // printf("wrl1_r_dec1 unused %s",cbuf1); // zB normalIndex
      --wNr;
    }

    p1 = p2;
    ++p1;          // skip ']'

    goto L_next_word;
  }


  //----------------------------------------------------------------
  // copy next word --> w1
  if(*p1 != '{') {

    // skip unused words
    //                    12345678901234
    if     (!strncmp(p1, "Shape",          5)) {
      p1 += 6;
      *ps = p1;
      goto L_next_word;
    }


    // skip unused blocks2
    //                    12345678901234
    if     (!strncmp(p1, "Normal",         6)) goto L_skip_block2;
    else if(!strncmp(p1, "Node_Info",      9)) goto L_skip_block2;
    else if(!strncmp(p1, "NormalBinding", 13)) goto L_skip_block2;
    else if(!strncmp(p1, "normalIndex",   11)) goto L_skip_block3;
    goto L_copy_wd;

    L_skip_block2:
      p1 = strchr(p1, '{');
      if(p1 == NULL) {printf("**** wrl1_r_dec1 E456\n"); return -2;}
      wrl1_r_skipBlock (&p1, pe);
      ++p1;
      *ps = p1;
      goto L_next_word;
  
    L_skip_block3:
      p1 = strchr(p1, '[');
      if(p1 == NULL) {printf("**** wrl1_r_dec1 E457\n"); return -2;}
      UTX_pos_skipBrackX (&p1, pe, ']');
      ++p1;
      *ps = p1;
      goto L_next_word;


    // add next word ...
    L_copy_wd:
    p2 = UTX_find_Del1 (p1); // find wordEnd

    // printf(" word |%s| wordlen=%d\n",p1,p2-p1);
    wa[wNr] = p1;
       // printf(" new wd wa[%d]=|",wNr); UTX_dump_s__ (p1, 16); printf("|\n");

    ++wNr;
    if(wNr >= wMax) {  TX_Error("wrl1_r_dec1 E005");
      for(i1=0;i1<wNr;++i1) {
        printf(" wa[%d]=|",i1);UTX_dump_s__ (p1, 40);printf("|\n");
      }
      p1 = p2;
      return -1;
    }
    p1 = p2;
    // ++p1;


    // check for DEF
    if (wNr > 3) {
      if(!strncmp(wa[wNr-4], "center", 6)) {     // skip center
        wNr -= 4;

      } else if(!strncmp(wa[wNr-4], "translation", 11)) {
        wrl1_r_dec_trl (&wa[1]);
        wNr -= 4;


      } else if(!strncmp(wa[wNr-4], "scaleFactor", 11)) {
        wrl1_r_dec_scf (&wa[1]);
        wNr -= 4;


      } else if(!strncmp(wa[wNr-4], "diffuseColor", 12)) {     // skip
        irc = wrl1_r_dec_Col1 (&wa[wNr-3]);
        wNr -= 4;
      }

    } else if(wNr > 1) {
      if(!strncmp(wa[wNr-2], "DEF", 3)) {
        irc = wrl1_r_dec_DEF (&wa[wNr-1]);
        if(irc < 0) return irc;
        wNr -= 2;

      } else if(!strncmp(wa[wNr-2], "USE", 3)) {
        irc = wrl1_r_dec_USE (&wa[wNr-1], pe);
        if(irc < 0) return irc;
        // printf("------------- wrl1_r_dec1 %d nach USE\n",level);
        wNr -= 2;

      } else if(!strncmp(wa[wNr-2], "whichChild", 10)) {     // skip
        wNr -= 2;

      } else if(!strncmp(wa[wNr-2], "radius", 6)) {
        radius = UTX_db_tx  (&p1, wa[wNr-1]);
        // printf(" radius=%f\n",radius);
        wNr -= 2;

      } else if(!strncmp(wa[wNr-2], "height", 6)) {
        height = UTX_db_tx  (&p1, wa[wNr-1]);
        // printf(" height=%f\n",height);
        wNr -= 2;
      }


    } else { // das erste Wort
                       // 123456789012345678


      if(!strncmp(wa[0], "Separator", 9)) {
        wNr = 0;

      // // TransformSeparator; fasst mehrere Transformationen zusammen
      // } else if(!strncmp(wa[0], "TransformSeparator", 18)) {
        // UT3D_m3_copy (maTab[level-1].ma, maTab[level].ma);
        // printf(" coppy ma %d --> %d\n",level,level-1);
        // wNr = 0;

      // // Transform
      // } else if(!strncmp(wa[0], "Transform", 9)) {

                        // 1234567890123
      } else if((!strncmp(wa[0], "emissiveColor", 13)) ||
                (!strncmp(wa[0], "ambientColor",  12)) ||
                (!strncmp(wa[0], "specularColor", 13))) {
        // skip this and the next 3 words ...
        p1 = wrl1_r_skip_words (3, p1);
        wNr = 0;

                               // 1234567890123
      } else if((!strncmp(wa[0], "transparency", 12)) ||
                (!strncmp(wa[0], "shininess",     9))) {
        // skip this and the next 1 word ...
        p1 = wrl1_r_skip_words (1, p1);
        // printf(" nach str/shi:|");UTX_dump_s__ (p1); printf("|\n");
        wNr = 0;
      }
/*
      //                          123456789
      } else if(!strncmp(wa[0], "Node_info", 9)) {
         // skip unused block
         printf(" skip unused block\n");

      }
*/

    }

    goto L_next_word;
  }


  //----------------------------------------------------------------
  // (*p1 == '{') - Klammerausdruck -
  // printf(" enter { nex level %d\n",level);


  if(wNr > 0) {  // skip this blocks:
                        // 1234567890123456
    if((!strncmp(wa[wNr-1], "ShapeHints", 10))        ||
       (!strncmp(wa[wNr-1], "Texture2", 8))           ||
       (!strncmp(wa[wNr-1], "SpotLight", 9))          ||
       (!strncmp(wa[wNr-1], "DirectionalLight", 16))  ||
       (!strncmp(wa[wNr-1], "Normal", 6))             ||
       (!strncmp(wa[wNr-1], "IndexedLineSet", 14))    ||
       (!strncmp(wa[wNr-1], "BackgroundColor", 15))   ||
       (!strncmp(wa[wNr-1], "PerspectiveCamera", 17)) ||
       (!strncmp(wa[wNr-1], "Node_info", 9))          ||
       (!strncmp(wa[wNr-1], "Info", 4)))           {   // skip "{ .. }" - Block
        // printf(" skip |");UTX_dump_s__ (wa[wNr-1],32);printf("|\n");
      ++err0Nr;
      wrl1_r_skipBlock (&p1, pe);
      --wNr;
      goto L_next_word;
    }
  }


  ++p1;  // skip '{'
  if(wNr > 0) {  // skip this blocks:
    if(!strncmp(wa[0], "LOD", 3)) {
      lodLevel = level + 2;
      // printf("	startLOD at %d\n",lodLevel);
    }
  }


  L_recurse:
  irc = wrl1_r_dec1 (&p1, pe);   // RECURSE
  if(irc < 0) return irc;

  // printf(" resume lev=%d |",level);UTX_dump_s__(wa[0]);printf("|\n");
  // printf("     weiter |");UTX_dump_s__(p1);printf("|\n");
  if(level < 1) return 0;
  // if(level < 2) return 0;
  // if(level < 2) {TX_Error("wrl1_r_dec1 ELEV %d",level); return 0;}
 

  ii = 0;


  // nach recurse
  ++iTest;
    // printf(" work|..| %d\n",wNr); wrl1_dump_wa (wNr, wa);
  if(wNr < 1) goto L_work_done;

/*
  // DEF: in namTab speichern; dann bearbeiten.
  if(!strcmp(wa[ii], "DEF")) {
    irc = wrl1_r_dec_DEF (&wa[ii + 1]);
    if(irc < 0) return irc;
    ii += 2;
*/
/*
  // USE: execute DEF-Record; dann weiter ..
  if(!strncmp(wa[ii], "USE", 3)) {
    irc = wrl1_r_dec_USE (&wa[ii + 1], pe);
    if(irc < 0) return irc;
    printf("------------- wrl1_r_dec1 %d nach USE\n",level);
    ii += 2;
*/
/*
  // whichChild: hat dahinter eine zahl; skippen;
  } else if(!strcmp(wa[ii], "whichChild")) {
    // printf(" whichChild %d\n",wNr);
    ii += 2;
    // aber manchmal ist dann Ende.
    if(ii >= wNr) goto L_work_done;
    goto L_work__;     // wenn danach DEF kommt ..
*/

  // }


  // get wordIndex in wTab1
  UTX_cp_word__(cbuf1, wa[ii]); // word in wa ist nicht terminiert;
  funcNr = UTX_cmp_word_wordtab (wTab1, cbuf1);
  // printf(" Tab1func %d |%s|\n",funcNr,cbuf1);
  if(funcNr < 0) {
    ++err0Nr;
    // printf(">>>>>>>>> |");
    // UTX_dump_s__ (wa[ii], 40);
    // printf("| <<<<<<<<<<<< unknown\n");
    goto L_work_done;
  }

  // printf(" nxt func %s (%d) wa[%d]\n",wTab1[funcNr],funcNr,ii);

  // skip all functions >= 16
  if(funcNr > 16) goto L_work_done;



  //================================================================
  // 2-7 = Attribs; save Attrib
  if(funcNr > 7) goto L_work_Objects;
  if(funcNr < 2) goto L_work_Execute;
  // printf(" save Attrib %s (%d) wa[%d]\n",wTab1[funcNr],funcNr,ii);


  if(funcNr == 3) {         // MatrixTransform
    wrl1_r_dec_MatTra ();

  // if(funcNr == 2) {         // TransformSeparator
    // UT3D_m3_copy (maTab[level-1].ma, maTab[level].ma);
    // printf(" copy ma %d --> %d\n",level,level-1);

  } else if(funcNr == 4) {  // Transform
    wrl1_r_dec_Trf ();

  } else if(funcNr == 6) {  // Scale
    wrl1_r_dec_Scale ();

  } else if(funcNr == 7) {  // Translation
    wrl1_r_trm_Trl ();
  }

  goto L_work_done;




  //================================================================
  // 8-15 = Objects; save Object in ObjStack
  L_work_Objects:
  // printf(" work Object %s (%d) wa[%d]\n",wTab1[funcNr],funcNr,ii);
  if(funcNr == 8) {           // IndexedFaceSet
    wrl1_r_dec_ifs ();

  } else if(funcNr == 11) {   // Sphere
    wrl1_r_dec_sph (radius);

  } else if(funcNr == 14) {   // Cylinder
    wrl1_r_dec_cyl (radius, height);

  } else if(funcNr == 16) {   // Shape
    wrl1_r_dec_cyl (radius, height);
  }
  // printf(" skip func later |%s|\n",cbuf1);
  goto L_work_done;




  //================================================================
  // es verbleibt eine execute-function; aber eigentlich nix zu tun ..
  L_work_Execute:
  // Objects in ObjStack ausgeben.
  // printf(" work %s (%d) wa[%d]\n",wTab1[funcNr],funcNr,ii);




  //================================================================
  L_work_done:
  wNr = 0;   // Reset

  // return, wenn ein USE activ ist; aber nur im gleichenLevel.
  // printf(" act.USE %d %d\n",level,useLevel);
  if(useLevNr > 0) {
    if(useLevTab[useLevNr-1] == level-1) return 0;
  }

  if(level > 0) goto L_next_word;
  // if(level > 1) goto L_next_word;


  return 0;
}


//================================================================
  int wrl_readTess__ (char *fnam) {
//================================================================

  int    irc, i1;
  long   fSiz;
  char   *memSpc, *p1, *pe, c1, c2, cBuf[80];
  FILE   *fpi;


  // printf("wrl_readTess__ |%s|\n",fnam);

  iTab = NULL;


  // get filesize
  fSiz = OS_FilSiz (fnam);
  if(fSiz < 1) {TX_Print("wrl_readTess__ FileExist E001 %s",fnam); return -1;}
  // printf(" fSiz=%d\n",fSiz);


  if((fpi=fopen(fnam,"rb")) == NULL) {
    TX_Print("wrl_readTess__ Open E001 %s",fnam);
    return -1;
  }



  // FileInhalt testen     nix "#VRML V2.0 utf8"
  fgets (cBuf, 12, fpi);
  rewind (fpi);

                //  1234567890
  if(strncmp(cBuf, "#VRML V1.0", 10)) {
    TX_Print("sorry - is not VRML-Version-1 - Model");
    fclose (fpi);
    return -1;
  }
  


  // allocat memspc fuer gesamte Datei
  memSpc = malloc (fSiz + 64);
  if(memSpc == NULL) {
    TX_Error ("******** out of memory - wrl1_r__ *********");
    return -1;
  }




  c2 = 'X';
  i1 = 0;
  while((c1 = fgetc (fpi)) != EOF)  {
    if(c1 == '#') {  // skip commentline
      L_nxt_comment:
        c1 = fgetc (fpi);
        if(c1 != '\n') goto L_nxt_comment;
        continue;
    }
    // fehler "Separator{"
    if(c1 == '{') {
      if(c2 != ' ') {memSpc[i1] = ' '; ++i1;}

    } else if((c1 == '\n') ||
              (c1 == '\r') ||
              (c1 == '\t'))   c1 = ' ';
    if((c1 == ' ')&&(c2 == ' ')) continue;
    c2 = c1;
    memSpc[i1] = c1;
    ++i1;
    if(i1 > fSiz) {TX_Error("wrl_readTess__ E00m"); return -1;}
    // printf("%c",c1);
  }
  fclose (fpi);
  memSpc[i1] = '\0';
  fSiz = i1;
  // return -1;






/*
  // gesamtes file als block lesen ..
  fread (memSpc, fSiz, 1, fpi);

  // terminieren - MS-Win Problem !
  memSpc[fSiz] = '\0';


  // FileInhalt testen     nix "#VRML V2.0 utf8"
  if(strncmp(memSpc, "#VRML V1.0", 10)) {
    TX_Print("sorry - is not VRML-Version-1 - Model");
    irc = -1;
    goto L_fertig;
  }
*/

  

  //================================================================
  // connect mem
  vTab      = (Point*)memspc501;
  vTabSiz   = sizeof(memspc501) / sizeof(Point);
  // vTab      = GLT_pta;
  // vTabSiz   = GLT_pta_SIZ;
  vTabNr    = 0;

  // iTab      = (int*)memspc201;
  // iTabSiz   = sizeof(memspc201) / sizeof(int);
  iTabSiz   = iTab_INC;
  iTab      = (int*)malloc (iTabSiz * sizeof(int));
  iTabNr    = 0;
  
  defTab    = (char**)memspc101;
  defTabSiz = sizeof(memspc101) / sizeof(char*);
  defTabNr  = 0;

  maTab     = (void*)memspc55;
  if((sizeof(memspc55) / sizeof(maRec)) < MAXLEV) 
    printf("wrl_readTess__ E001\n");


  // printf("vTabSiz=%d iTabSiz=%d defTabSiz=%d\n",vTabSiz,iTabSiz,defTabSiz);

  outSpc_INC = (fSiz * 1) + outSpc_FRE;

  irc = UME_malloc (outSpc, outSpc_INC, fSiz);
  if(irc < 0) goto L_fertig;


  // init ..
  err0Nr   = 0;
  level    = 0;
  useLevNr = 0;
  lodLevel = -1;
  iTest    = 0;

  trSc[0] = 1.;
  trSc[1] = 1.;
  trSc[2] = 1.;

  // alle Levels muessen neu laden !
  for(i1=0; i1<MAXLEV; ++i1) maTab[i1].stat = -1;

  trVcStat  = 0;
  trScStat  = 0;
  trMatStat = 0;

  actCol.cr  = 0;
  actCol.cg  = 0;
  actCol.cb  = 0;

  // actCol.color = 0;


  UT3D_m3_copy (trMat, UT3D_MAT_4x3);
  // UT3D_m3_copy (&maTab[0], UT3D_MAT_4x3);


  // decode file ..
  p1 = memSpc;
  pe = memSpc + fSiz;

  irc = wrl1_r_dec1 (&p1, pe);   // start - RECURSE
  // printf(" ... done\n");


  // Schlussrecord schreiben
  wrl1_r_out_end ();


  //================================================================
  L_fertig:
  free (memSpc);
  // printf("ex wrl_readTess__ %d iTest=%d\n",irc,iTest);

  if(err0Nr > 0) {
    sprintf(cBuf, "***** %d unsupported records *****",err0Nr);
    TX_Print (cBuf);
  }

  return irc;

}

//====================== EOF =============================
