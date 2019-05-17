// Import VRML-2                               2010-01-21     RF
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
ImageTexture not yet implemented.
  Textures noch nicht ausgewertet.
Flächen mit weniger als 100 Punkten deren Indextabelle linear ist,
  ausgeben als Polygon und Surf=Polygon.
  A=RCIR geht nur fuer konkave Flaechen.
EXTERNPROTO unfertig.

-----------------------------------------------------
Modifications:
2010-01-21 first version. RF.

-----------------------------------------------------
*/
#ifdef globTag
 void VR2(){}
#endif
/*!
\file  ../exp/vr2_r.c
\brief read VRML-2 file 
\code
=====================================================
List_functions_start:

VR2_r__         mainentry.

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



-------------------------------------------
EXTERNPROTO:
unfertig; VR2_r_dec_EPROT
 speichern in ptTab mit .ext='E';
 bei Aufruf ein M#="./modelname.gcad" absetzen.

 "xx.wrl" muesste umgewandelt werden. Mit weiteren EXTERNPROTO darin schwierig ..
 suchen, obs ein .gcad gibt.

t_extprot1|2|3.wrl



...................................................
Damit man beim Schreiben VR.-File nicht "als PROTO raus" angeben muss:
Methode suchen, wie man ein VR-File schreibt, das als primaryModel und
auch als PROTO brauchbar ist.
Ev vornweg das Model als PROTO;
hinterher ein Aufruf dazu ?

Moeglicherweise kann man dann das PROTO nicht editieren ?
EXTERNPROTO "fnam.wrl#modelname"
und ins protofile auch eine proto-Aufruf rein !


------------------------------------------------
PROTO:
 example wie man Farbe an proto übergibt:  5.wrl.



...........................................
Textures:
 t_tex1.wrl..wrl
  WWWTest1.wrl

Texturen werden tw gedreht !
Es wird eine bounding box errechnet; 
The longest dimension of the bounding box defines the S coordinates,
and the next longest defines the T coordinates.
Gleiche Seitenlängen: Reihenfolge X Y Z.
S ranges from 0 to 1,
T ranges between 0 and the ratio of the second greatest dimension.



...........................................
A Transform defines a referenceSystem use for 1-n Shapes.

A Shape has one object; a Cone or a Box or a Sphere or a IndexedFaceSet ..
Obj is complete and written when Shape-bracket is closed.

Shapes:
  IndexedFaceSet IndexedLineSet Sphere Cylinder Extrusion Box Cone Text




=====================================================
CoordinateSystem VR:    VR_pt_vr2cad() VR_vc_vr2cad()

  TopView:
 
     O---- X             
     |
     |
     Z

IsoView:

      y
      |  -z
      | /
      |/
      O------X
     /
    /
   z


CAD-Y wird zu VR-Z-neg
CAD-Z wird zu VR-Y.



=====================================================
Gesamtaufbau:
EXTERNPROTO prtnam [
  ..
] <url>

# Standardcolour
DEF <name> <code>

Transform { children [
  translation 0 0 2
  rotation 0 0 1 0
  scale 0.03 0.03 0.03
  children [
    Shape {
      ..
    }
    Shape {
      ..
    }
  }
]}


---------------------------
PROTO obj_1 [
 field SFFloat radius 1
]
{
 Sphere {
   radius IS radius
 }
}
..
    Shape {
     geometry obj_1 { radius 0.5 }
    }
..

  In PROTO [ .. ] are the Defaultparameters ?



---------------------------
Geometry:
  Shape {
    appearance Appearance { .. }
    geometry ..
  }

Spec IndexedLineSet:
  mehrere Polygone in einem Set werden durch -1 getrennt.
  geometry IndexedLineSet {
    coord Coordinate {
      point [ <points> ]
    } 
    coordIndex [ <indexes> ] 
  } 
    

Spec IndexedFaceSet:
  geometry IndexedFaceSet {
    solid FALSE
    coord Coordinate {
      point [ <points> ]
    } 
    coordIndex [ <indexes> ] 
    color Color {
      color [ <colors> ]
    } 
  }


---------------------------
Group { children [
 ..
] }
# skip it ..


---------------------------
Farbe oder Texture in Appearance:
  Farbe:
  appearance Appearance {
    material Material {
      diffuseColor 0.85 0.85 0.20
      emissiveColor 0.000000  0.000000  0.000000
      specularColor 0.75 0.67 0.34
      shininess 0.3
      transparency  0.000000
    }
  }

  Texture:
  appearance Appearance {
    texture ImageTexture {
      url "fn.jpg"
      repeatS FALSE
      repeatT FALSE
    }
    textureTransform TextureTransform {
      rotation 0
      scale 2 2
    }
  }



---------------------------
PROTO mat001 [] {
    Material { diffuseColor 1 0.85 0.7 }
}
appearance Appearance { material txt001 {} }




---------------------------
Billboard {
  axisOfRotation 0 0 0
  children [
    Shape { .. }
  [
}


---------------------------
Anchor {
  url "otherFile.wrl#NiceView"
  description "Link to a nice view of the scene"

Load new Scene on clicking the Obj.
NiceView is a DEF for the root-obj in otherFile.wrl.

  parameter "target=name_of_frame"  wird fuer html-Aufrufe benutzt; ..??


---------------------------
  DirectionalLight {
    direction -0.3 -0.5 -0.5
    intensity 0.7
  }


---------------------------
Viewpoint {
 position 2.91571 14.0838 -1.51428 
 orientation -0.308759 -0.932644 -0.186661 1.03687 
 description "AnsichtNr 1"
}



---------------------------
skip all following:
  WorldInfo  { .. }
  NavigationInfo { .. }
  Background { .. }
  ProximitySensor { .. }
  TimeSensor { .. }
  PlaneSensor { .. }
  TouchSensor { .. }
  VisibilitySensor { .. }
  Collision { .. }
  DirectionalLight { .. }
  Viewpoint { .. }
  ColorInterpolator { .. }
  OrientationInterpolator { .. }
  Extrusion { .. }                     Geom.Obj !
  Script { .. }
  Switch { .. }
  ROUTE <event> TO <event>



========================================================================
method:
========================================================================

read complete file (without line-terminators & comments) into memory;
make a pointer-table to all DEF's


------------------------------------------------
HANDLING DEF / USE    VR2_r_dec_DEF VR2_r_dec_USE VR2_dump_use
------------------------------------------------

DEF-USE is a goto.  Return after corresponding bracket.
  Is allowed everywhere !
  DEF must also be executed (eg. "DEF ROOT Group {"; PROTO not).

---------------------------
useTab[iUse-1]
useLevTab[]

VR2_r_dec_DEF set name in defTab
VR2_r_dec_USE find name in defTab; call VR2_r_add_USE
VR2_r_add_USE add returnaddress to useTab & level to useLevTab


---------------------------
DEF Beispiel:
DEF c_main Appearance {
  material Material { #diffuseColor 0.85 0.85 0.20  }
}
Shape {
  appearance USE c_main
  geometry ..
}
...........
USE Transform {
USE Shape {
appearance USE Appearance
material USE Material
texture USE ImageTexture
geometry USE ..                                       <<<<<<<<<<<<<<<<   ??
coord USE Coordinate   (30.wrl)                       <<<<<<<<<<<<<<<<   ??
DEF my_camera Inline { url [ "camera.wrl" ] }





------------------------------------------------
HANDLING PROTO / EXTERNPROTO
------------------------------------------------
PROTO:
 alle protos (mit geometry) als gcad-interne-subModels anlegen.
 EXTERNPROTO als externe subModels.

Wenn Proto "Shape" enthaelt: als gcad-internes-subModel anlegen / rufen.
else wie DEF benutzen (Material ..)

Beim PROTO-Aufruf wird ein subModel-Aufruf (M#) erzeugt;
  Beim Aufruf werden nur folgende Parameter verarbeitet:
  protoname { pInspoint x y z pScalefact x y z pRotation x y z a }
  pInspoint, pRotation werden als origin/orientation angelegt.

Alle Felder in Definitionsblock werden ignoriert; z.B:
  [
    field SFVec3f pInspoint
    field SFVec3f pScalefact
    field SFRotation pRotation
  ]

................................................
VR2_r_dec_PROTO
  save its name in List of existing protos (prTab);
  test if it is a inline-proto or a model-proto;
    (model-proto contains "Shape")

VR2_r_ck_PROTO
  test if ps is the name of a proto;
  return prTab-Index

Entering proto:
  enter Block, work block
  // model-proto: enter Block.
  // inline-proto: add returnAddress (wie bei USE); enter Block.

Exiting proto-block:
  exiting inline-proto:    (t_prot2.wrl, Gelaende1.wrl)
    add returnAddress (wie bei USE); jump into PROTO.
    return erfolgt wie bei USE;
  exiting model-proto:
    neues VR2_r_wri_PROTO
    als Model ausgeben; (M# = "protoname" Posi Orientation)

After end-of-main: Resolv_protos;
  work protos; loop tru List of existing protos;
  save each proto as a separate subModel.




========================================================================
Box { size <x_size> <y_siz> <z_siz> }

Sphere { radius r }

Cone {
          bottomRadius  1       # SFFloat
          height        2       # SFFloat
          side   TRUE
          bottom FALSE
     }

Cylinder {
          radius  1     # SFFloat
          height  2     # SFFloat
          side   TRUE
          bottom FALSE
          top    TRUE
     }


IndexedLineSet {
  coord Coordinate {
    point [
      0.0267537   0.145415   -5.93382 ,
      0.117418    1.49511   -5.69627
    ]
  }
  coordIndex [ 0 , 1 , 2 ]
}

PointSet {
#         startIndex  0 # SFLong
#         numPoints   -1        # SFLong
        coord Coordinate {
          point [
             0.333548  0.0632862  -0.662062 ,
            -0.0942663  -0.985753   -0.69329 ]

       }
     }

Text { string [] length }

Extrusion ..
  2D-Kontur entlang spine ...


Normal { vector  [] }
 (f IndexedFaceSet and ElevationGrid).

IndexedFaceSet { coord coordIndex [] }

IndexedLineSet { coord coordIndex [] }

PointSet { coord }

Shape { appearance geometry }

Transform { center children rotation scale scaleOrientation
  translation <dx dy dz>
}

ElevationGrid
  ein mesh mit z_values auf einem regelmaessigen Gitter ?


children Transform { translation -C children [...] }





*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>              // for ...
#include <ctype.h>               // isalpha


#ifdef _MSC_VER
// die folgenden Funktionen exportieren (werden vom Main gerufen):
__declspec(dllexport) int VR2_r__ (char*);
// nachfolgende externals werden aus dem Main-Exe imported:
#define extern __declspec(dllimport)
#endif

// #include "../ut/ut_umem.h"             // UME_
#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_itmsh.h"            // MSHIG_EDGLN_.. typedef_MemTab.. Fac3
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_os.h"               // OS_get_bas_dir ..
#include "../ut/ut_obj.h"              // UTO_stru_2_obj
#include "../ut/ut_txfil.h"            // UTF_GetPosLnr

#include "../xa/xa_mem.h"              // memspc51, mem_cbuf1


// typedef_MemTab(int);
// typedef_MemTab(Point);
// typedef_MemTab(Fac3);
// typedef_MemTab(EdgeLine);


//----------------------------------------------------------------
// local vars:
#define siz_useTab  12
#define siz_levPrt  32
#define inc_pTab 10000
#define inc_fTab 10000
// #define inc_eTab 10


static char    mdlNam[80];

static char    *memSpc;          // a copy of the whole wrl-file

static int     level, levPrt[siz_levPrt];

static char    **defTab;         // memspc101
static int     defTabSiz, defNr;

static int     useLevTab[siz_useTab], iUse;
static char    *useTab[siz_useTab];


static ColRGB  newCol, actCol;

static MemTab(Point) pTab = _MEMTAB_NUL;        // table of Point's

static int     *iTab, iTabSiz, iTabNr;           // memspc501


static MemTab(Fac3) fTab = _MEMTAB_NUL;         // table of Fac3's
static MemTab(EdgeLine) eTab = _MEMTAB_NUL;     // table of EdgeLine's
static MemTab(int) eDat = _MEMTAB_NUL;          // dataSpace for all EdgeLines
// static int     *iTab=NULL;                       // memspc201
// static int     vTabSiz, vTabNr;

// typedef struct {Mat_4x3 ma; int stat;}                    maRec;
// static maRec  *maTab;            // memspc55
static Mat_4x3 *maTab;            // memspc55
static int     siz_maTab;
static Mat_4x3 trMat;


typedef struct {char *name, *ret; char ext, mdl;}             protoRec;
// .name    pointer to the protoname, terminated with ' '
// .ret     returnto-Adress;
// .ext     'P' = PROTO; 'E' = EXTERNPROTO
// .mdl     'M' = subModel (proto has Shape); 'I' = Inline; eg. Material

static protoRec *prTab;
static int      siz_prTab, prNr, iPro;
static int      prAct;
static int      mdlAct;          // -1=primary Model; 0-n: index into prTab.


static long    iLin;             // index of first new Line
static long    iCrv;             // index of first new curve
static long    iSur;             // index of first new surf
static long    iSol;             // index of first new body
static long    iMdl;             // index of first new subModel

static int     newTyp;
static ColRGB  newCol, actCol;
static double  newGeo[3];  // size & height & radius & bottomRadius
static double  newTra[3], newpTra[3];  // translation
static double  newScl[3];  // translation
static double  newRot[4], newpRot[4];  // rotation



//----------------------------------------------------------------
// Protos:
  long VR2_loadwrl (char *fnam);
  int UTX_pos_skipBrackX (char **ps, char *pe, char ce);
  char* VR2_r_ck_primaryBlock (int *iStat, char *ps, char *pe);
  char* VR2_r_skip_word (int inr, char *p1);
  char* VR2_r_dec_points (char *ps);
  char* VR2_r_dec_index (char *ps);
  char* VR2_r_dec_cold (char *ps);
  char* VR2_r_dec_floats (double *da, char *ps, int vNr);
  char* VR2_r_dec_DEF (char *ps);
  char* VR2_r_dec_USE (char *ps, char *pe);
  char* VR2_r_dec_PROTO (char *ps, char *pe);
  char* VR2_r_dec_EPROT (char *ps, char *pe);
  char* VR2_r_skip_expr (char *ps, char ce);
  char* VR2_r_skip_proto1 (char *ps, char *pe);
  int VR2_r_wri_att_col (long dbi, char cTyp);
  int VR2_r_wri_att_hide (long dbi, char cTyp);




//================================================================
  int VR2_r__ (char *fnam) {
//================================================================
// mainentry. 
// 1) Einlesen, intern speichern;
// 2) abarbeiten ..

  int    irc;
  long   fSiz;
  char   *ps, *pe;


  // printf("==================================\n");
  printf("VR2_r__ |%s|\n",fnam);


  // extract modelname
  UTX_fnam_s (mdlNam, fnam);
  // remove filetyp
  UTX_ftyp_cut (mdlNam);
    printf(" mdlNam = |%s|\n",mdlNam);


  Mod_kill__ ();   // alle tmp/Model_* loeschen


  // Textbuffer 1 loeschen
  UTF_clear1();


  // initMessage
  sprintf(mem_cbuf1,"# VR2-Import %s",fnam);
  UTF_add1_line (mem_cbuf1);


    // TEST ONLY
    // UTF_add1_line ("P12=100 200 5");


  // get next free surf-index
  iLin = DB_dbo_get_free (Typ_LN) + 1;
  iCrv = DB_dbo_get_free (Typ_CV) + 1;
  iSur = DB_dbo_get_free (Typ_SUR) + 1;
  iSol = DB_dbo_get_free (Typ_SOL) + 1;
  iMdl = DB_dbo_get_free (Typ_Model) + 1;
    // printf(" iCrv=%d iSur=%d iSol=%d iMdl=%d\n",iCrv,iSur,iSol,iMdl);


  // load file into memory.
  fSiz = VR2_loadwrl (fnam);
  if(fSiz < 0) return -1;



  // connect mem
  MemTab_ini__ (&pTab, sizeof(Point), Typ_PT, inc_pTab);
  MemTab_ini__ (&fTab, sizeof(Fac3), Typ_Fac3, inc_fTab);
  MemTab_ini__ (&eTab, sizeof(EdgeLine), Typ_EdgeLine, 5);
  MemTab_ini__ (&eDat, sizeof(int), Typ_Int4, 5);


  iTab    = (int*)memspc501;
  iTabSiz = (sizeof(memspc501) / sizeof(int)) - 2;
  iTabNr  = 0;


  // table for DEF-USE and PROTO'S
  defTab    = (char**)memspc101;
  defTabSiz = (sizeof(memspc101) / sizeof(char*)) - 2;
  defNr  = 0;
  iUse   = 0;


  // active transformationmatrix is iTra
  maTab     = (void*)memspc55;
  siz_maTab = (sizeof(memspc55) / sizeof(Mat_4x3)) - 2;

  prTab = (protoRec*)memspc54;
  siz_prTab = (sizeof(memspc54) / sizeof(protoRec)) - 2;
  prNr  = 0;

  mdlAct = -1;            // -1=primary Model


  //----------------------------------------------------------------
  // init for nxt model
  L_nxt_mdl:
    // printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX \n");
    // printf(" nxt model: %d |%s|\n",mdlAct,mdlNam);

  // default-color
  Col_set_3db (&newCol, 0.5, 0.5, 0.5);


  // load default-matrix
  UT3D_m3_copy (&maTab[0], UT3D_MAT_4x3);
    // DEB_dump_obj__ (Typ_M4x3, &maTab[0], "maTab[0]");


  // decode
  level = -1;
  prAct = -1;
  iPro  = -1;
  levPrt[0] = -1;    // not a proto
  ps = memSpc;
  pe = memSpc + fSiz;
  // work ..
  irc = VR2_r_dec_blk ("", &ps, pe, 0, 0);



  //----------------------------------------------------------------
  // save model
  if(mdlAct < 0) {    // save primary model --------------------------
    // exitMessage
    UTF_add1_line ("# end of VR2-Import");

    // add sourceBuffer to primarySource
    UTF_insert1 (-1L);


  } else {            // save subModel ---------------------------------
    // save -> file
    sprintf(memspc011,"%sModel_%s",OS_get_tmp_dir(),mdlNam);
      // printf(" save model |%s|\n",memspc011);

    UTF_file_Buf1__ (memspc011);
  }




  //----------------------------------------------------------------
  // setup for next subModel
    // VR2_r_dump_prTab ();
  L_nxt_prt:
  ++mdlAct;
  if(mdlAct >= prNr) goto L_exit;
  if(prTab[mdlAct].ext == 'P') {                  // P|E
    if(prTab[mdlAct].mdl == 'I') goto L_nxt_prt;  // M|I
    // set modelname
    UTX_cp_word__(mdlNam, prTab[mdlAct].name);
    iLin = 1;
    iCrv = 1;
    iSur = 1;
    iSol = 1;
    iMdl = 1;
    UTF_clear1();  // Textbuffer 1 loeschen
    goto L_nxt_mdl;

  } else if(prTab[mdlAct].ext == 'E') {           // externproto
    TX_Print("Externproto not yet supported");
    goto L_nxt_mdl;
  }



  //----------------------------------------------------------------
  L_exit:
  MemTab_free (&pTab);
  MemTab_free (&fTab);
  MemTab_free (&eTab);
  MemTab_free (&eDat);

  // if(iTab) free (iTab);
  // iTab = NULL;

  free(memSpc);


  return 0;

}


//================================================================
  long VR2_loadwrl (char *fnam) {
//================================================================
// allocate memSpc; load file fnam into memSpc
// (remove commentlines, \r and \n)
// mbuf must be freed !
 
  long   i1, fSiz;
  char   c1, c2;
  FILE   *fpi;

  // printf("VR2_loadwrl |%s|\n",fnam);


  // get filesize
  fSiz = OS_FilSiz (fnam);
  if(fSiz < 1) {TX_Print("VR2_loadwrl FileExist E001 %s",fnam); return -1;}
    printf(" fSiz=%ld\n",fSiz);

  if((fpi=fopen(fnam,"rb")) == NULL) {
    TX_Print("VR2_loadwrl Open E002 %s",fnam);
    return -1;
  }

  // allocat memspc fuer gesamte Datei
  memSpc = malloc (fSiz + 64);
  if(memSpc == NULL) {
    TX_Error ("VR2_loadwrl out of memory ***");
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
              (c1 == '\t') ||
              (c1 == ','))   c1 = ' ';
    if((c1 == ' ')&&(c2 == ' ')) continue;
    c2 = c1;
    memSpc[i1] = c1;
    ++i1;
    if(i1 > fSiz) {TX_Error("VR2_loadwrl ERX"); return -1;}
    // printf("%c",c1);
  }

  fclose (fpi);

  memSpc[i1] = '\0';

  // printf("ex VR2_loadwrl %d\n",i1);
  // printf("ex VR2_loadwrl %d |%s|\n",i1,memSpc);

  return i1;

}


//================================================================
  int VR2_r_dec_blk (char *blkTyp, char **ps, char *pe, int iTra) {
//================================================================
// decode block (eg "{ .. }")
// Input:
//   blkTyp     blocktyp; "" is primary block;
//              else it is the word left of '{'
//   ps         starting position - normally the '{'
//   iTra       index to active Transformation into maTab
// RetCod:  -2  unrecoverable Err.

// if(mdlAct < 0) resolv primary model
// else resolv proto prTab[mdlAct].name

// was wrl1_r_dec1


# define wMax 24
  int     irc, i1, ii, wNr, funcNr;
  char    *p1, *p2, *p3;
  char    *wa[wMax], cbuf1[80];



  //----------------------------------------------------------------
  ++level;
  levPrt[level] = -1;    // not a proto

  L_restart:
    // printf(">>>>>>>>>>>>> |"); UTX_dump_word (blkTyp, " {[");
    // printf("|{|"); UTX_dump_s__ (*ps, 40);
    // printf("|\n  VR2_r_dec_blk lev=%d tra=%d use=%d\n",level,iTra,iUse);

  p1  = *ps;
  while (*p1  == ' ')  { ++p1; }


  // init new block ..
  if       (!strncmp(blkTyp, "IndexedLineSet", 14)) { newTyp = Typ_CVPOL;
  } else if(!strncmp(blkTyp, "IndexedFaceSet", 14)) { newTyp = Typ_SUR;
  } else if(!strncmp(blkTyp, "Box",             3)) { newTyp = Typ_PRI;
  } else if(!strncmp(blkTyp, "Cone",            4)) { newTyp = Typ_CON;
  } else if(!strncmp(blkTyp, "Cylinder",        8)) { newTyp = Typ_SOL;
  } else if(!strncmp(blkTyp, "Sphere",          6)) { newTyp = Typ_SPH;
  } else if(!strncmp(blkTyp, "Shape",           5)) { newGeo[0] = UT_DB_LEER;
  } else if(!strncmp(blkTyp, "Transform",       9)) {
    newTra[0] = UT_DB_LEER;
    newRot[0] = UT_DB_LEER;
    if(iTra < siz_maTab) ++iTra;
    else { TX_Print("**** VR2_r_dec_blk E1Tra"); return -2; }
    UT3D_m3_copy (&maTab[iTra], &maTab[iTra-1]);
  }

  
  //----------------------------------------------------------------
  L_next_block:
  wNr = 0;

  // test primaryBlock:
  if(level == 0) {
    p1 = VR2_r_ck_primaryBlock (&i1, p1, pe);
    if(p1 == NULL) return -2;
      // printf(" n. ck_primaryBlock |"); UTX_dump_s__ (p1, 40);printf("|\n");
    if(i1 == 1) { *ps = p1; goto L_restart; }     // block skipped: retry .
    while (*p1  == ' ')  { ++p1; }
  }


  //----------------------------------------------------------------
  L_next_word:
  // set p1 = nextWordPos
  if(p1 >= pe) {     printf("ex VR21_r_dec1 - finished.\n");
    level = -1;
    return 0;
  }
  if(*p1 == '\0') {TX_Error("VR2_r_dec_blk E000"); return -1;}
  if(*p1 == ' ') {++p1; goto L_next_word;}

    // printf("  _dec1 next_word: %d|",p1-memSpc); UTX_dump_s__ (p1, 40);
    // printf(" lev=%d tra=%d use=%d\n",level,iTra,iUse);
    // VR2_dump_wa (wNr, wa);


  //----------------------------------------------------------------
  // Ende [-Klammer; maybe from "children[..]"; skip it ..
  if(*p1 == ']') {
    ++p1;
    while (*p1  == ' ')  { ++p1; }
    goto L_next_word;
  }



  //----------------------------------------------------------------
  // Ende }-Klammerausdruck -
  if(*p1 == '}') {


    ++p1;         // skip the '}'
    *ps = p1;
    // --level;
      // printf("----- exit } lev=%d levPrt=%d wNr=%d iUse=%d\n",
             // level,levPrt[level],wNr,iUse);
      // VR2_dump_wa (wNr, wa);


    --level;
      // printf("<<<<<<<<<<<<< exit } |");
      // UTX_dump_s__(*ps, 32);
      // printf("| %d\n",p1-memSpc);

    return 0;
  }



  //================================================================
  // Beginn einer [ -Klammer   (*p1 == '[')
  // arrayTyp [ ... ]
  if(*p1 == '[') {

    // ignore the whole block ?
                  //    1234567890123
    if(wNr > 0) {
      if(!strncmp(wa[0], "texCoordIndex", 13)) {
        // skip this block ..
        p1 = VR2_r_skip_expr (p1, ']');
        goto L_next_block;
      }
    }

    // ignor bracket ..
    ++p1;
    while (*p1  == ' ')  { ++p1; }
    goto L_next_word;
  }



  //----------------------------------------------------------------
  // add next word --> wa
  if(*p1 != '{') {

    // find wordEnd
    p2 = UTX_find_Del1 (p1);
    while (*p2  == ' ')  { ++p2; }


    // check if we can ignore this word
                 //   1234567890
    if((!strncmp(p1, "appearance", 10))       ||
       (!strncmp(p1, "material",    8))       ||
       (!strncmp(p1, "texture",     7))       ||
       (!strncmp(p1, "texCoord ",   9))       ||
       (!strncmp(p1, "geometry",    8))       ||
       (!strncmp(p1, "coord ",      6))       ||
       (!strncmp(p1, "children",    8)))         {
        // printf(" skip word |");UTX_dump_s__ (p1, 16); printf("|\n");
      p1 = p2;
      goto L_next_word;
    }


    // check if this is a unsupported block
               //   12345678901234
    if(!strncmp(p1,"Background",    10)) goto L_skip_block;
    if(!strncmp(p1,"WorldInfo",      9)) goto L_skip_block;
    if(!strncmp(p1,"NavigationInfo",14)) goto L_skip_block;
    if(!strncmp(p1,"Viewpoint",      9)) goto L_skip_block;
    if(!strncmp(p1,"Script",         6)) goto L_skip_block;



    // check if this is a function
                    //   123456789012
    p3 = p2;
    if     (!strncmp(p1,"translation",  11)) p3=VR2_r_dec_floats(newTra, p2, 3); 
    else if(!strncmp(p1,"pInspoint",     9)) p3=VR2_r_dec_floats(newpTra, p2, 4); 
    else if(!strncmp(p1,"rotation",      8)) p3=VR2_r_dec_floats(newRot, p2, 4); 
    else if(!strncmp(p1,"pRotation",     9)) p3=VR2_r_dec_floats(newpRot, p2, 4); 
    else if(!strncmp(p1,"diffuseColor", 12)) p3=VR2_r_dec_cold (p2); 
    else if(!strncmp(p1,"point",         5)) p3=VR2_r_dec_points (p2); 
    else if(!strncmp(p1,"coordIndex",   10)) p3=VR2_r_dec_index (p2); 
    else if(!strncmp(p1,"radius",        6)) p3=VR2_r_dec_floats(newGeo, p2, 1); 
    else if(!strncmp(p1,"bottomRadius", 12)) p3=VR2_r_dec_floats(newGeo, p2, 1); 
    else if(!strncmp(p1,"size",          4)) p3=VR2_r_dec_floats(newGeo, p2, 3); 
    else if(!strncmp(p1,"height",        6)) p3=VR2_r_dec_floats(&newGeo[1],p2,1);
    else if(!strncmp(p1,"scale",         5)) p3=VR2_r_dec_floats(newScl, p2, 3);
    else if(!strncmp(p1,"pScalefact",   10)) p3=VR2_r_dec_floats(newScl, p2, 3);
    else if(!strncmp(p1,"DEF ",          4)) p3=VR2_r_dec_DEF (p2);
    else if(!strncmp(p1,"USE ",          4)) p3=VR2_r_dec_USE (p2, pe);
    else if(!strncmp(p1,"PROTO",         5)) p3=VR2_r_dec_PROTO (p2, pe);
    else if(!strncmp(p1,"EXTERNPROTO",  11)) p3=VR2_r_dec_EPROT (p2, pe);
    //  functions not yet supported:
                    //   1234567890123
    else if(!strncmp(p1,"ROUTE",         5)) p3=VR2_r_skip_word (3, p2); 
    else if(!strncmp(p1,"ccw ",          4)) p3=VR2_r_skip_word (1, p2); 
    else if(!strncmp(p1,"solid",         5)) p3=VR2_r_skip_word (1, p2); 
    else if(!strncmp(p1,"side",          4)) p3=VR2_r_skip_word (1, p2); 
    else if(!strncmp(p1,"bottom",        6)) p3=VR2_r_skip_word (1, p2); 
    else if(!strncmp(p1,"diffuseColor", 12)) p3=VR2_r_skip_word (3, p2); 
    else if(!strncmp(p1,"emissiveColor",13)) p3=VR2_r_skip_word (3, p2); 
    else if(!strncmp(p1,"specularColor",13)) p3=VR2_r_skip_word (3, p2); 
    else if(!strncmp(p1,"transparency", 12)) p3=VR2_r_skip_word (1, p2); 
    else if(!strncmp(p1,"shininess",     9)) p3=VR2_r_skip_word (1, p2); 
    else if(!strncmp(p1,"top ",          4)) p3=VR2_r_skip_word (1, p2); 
    else if(!strncmp(p1,"collide",       7)) p3=VR2_r_skip_word (1, p2); 
    if(p3 == NULL) return -2;
    if(p3 != p2) { p1 = p3; goto L_next_word; }


    // add word to wa
    if(wNr >= wMax) {  TX_Error("VR21_r_dec1 E008"); return -2; }

    // printf(" word |%s| wordlen=%d\n",p1,p2-p1);
    wa[wNr] = p1;
      // printf(" wa[%d]=|",wNr); UTX_dump_s__ (p1, 16); printf("|\n");

    ++wNr;
    p1 = p2;

    goto L_next_word;
  }


  //================================================================
  // Beginn einer { -Klammer   (*p1 == '{')
  // alle words liegen im wordArray wa; p1 zeigt auf '{'
    // printf(" enter {-block |"); UTX_dump_s__ (wa[0], 32);
    // printf("| lev=%d\n",level);

  if(wNr < 1) {
    // proto-resolving starts with "{ transform .."
      // printf("  blockstart without parameter\n");
    // TX_Error("VR21_r_dec1 E010"); return -2; }
    wa[0] = mdlNam;
    goto L_recurse;
  }

  // blocks to recurse: 
                 //    12345678901234
  if((!strncmp(wa[0], "Group",           5))      ||
     (!strncmp(wa[0], "Appearance",     10))      ||
     (!strncmp(wa[0], "Material",        8))      ||
     (!strncmp(wa[0], "IndexedLineSet", 14))      ||
     (!strncmp(wa[0], "IndexedFaceSet", 14))      ||
     (!strncmp(wa[0], "Box",             3))      ||
     (!strncmp(wa[0], "Cone",            4))      ||
     (!strncmp(wa[0], "Cylinder",        8))      ||
     (!strncmp(wa[0], "Sphere",          6))      ||
     (!strncmp(wa[0], "Shape",           5))      ||
     (!strncmp(wa[0], "Transform",       9))      ||
     (!strncmp(wa[0], "Collision",       9))      ||
     (!strncmp(wa[0], "Coordinate",     10))) {
    goto L_recurse;
  }


  // test if it is a PROTO
  prAct = VR2_r_ck_PROTO (wa[0]);
  if(level >= siz_levPrt) printf("VR21_r_dec1 E009\n");
  else levPrt[level] = prAct;
  if(prAct >= 0) {
    newpTra[0] = UT_DB_LEER;
    newpRot[0] = UT_DB_LEER;
      // printf(" set levPrt[%d] = %d\n",level, prAct);
    goto L_recurse;
  }

  // this is a unknown block; skip it.
  goto L_skip_block;




  //================================================================
  // work new block: recurse.
  L_recurse:

  // update before entering inner blocks:
  // update transformation
  if(!strncmp(blkTyp, "Transform",       9)) VR2_r_upd_tra (iTra);


  ++p1;                                     // skip '{'
  while (*p1  == ' ')  { ++p1; }            // skip all blanks
  irc = VR2_r_dec_blk (wa[0], &p1, pe, iTra);   // RECURSE
  if(irc < 0) return irc;


  L_block_exit:
  // continue after recursion ..
    // printf("----- resume lev=%d |",level);UTX_dump_s__(p1,40);printf("|\n");
    // printf("<<<<<<<<<<<<< exit } |");UTX_dump_s__(wa[0], 30);printf("|\n");
    // printf("                     |");UTX_dump_s__(p1, 30);printf("|\n");
    // printf("  level=%d iUse=%d levPrt[level] = %d\n",level,iUse,levPrt[level]);

    // VR2_dump_wa (wNr, wa);
  // printf("     weiter |");UTX_dump_s__(p1);printf("|\n");


    // test if end of USE-block is reached
    if(iUse > 0) {
        // VR2_dump_use (iUse);
      if(useLevTab[iUse-1] == (level) ) {
        --iUse;
        p1 = useTab[iUse];
           // printf("  return after USE |");UTX_dump_s__(*ps, 32);printf("|\n");
           // printf("  return to        |");UTX_dump_s__(p1, 32);printf("|\n");
        goto L_next_block;
      }
    }


    // exiting_proto
    if(levPrt[level] >= 0) {
        // printf("  exiting proto %d\n",levPrt[level]);
      prAct = levPrt[level];
      levPrt[level] = -1;
      if(prTab[prAct].mdl == 'I') {
        // exiting_proto (inline-proto):
        // save returnAdress p1; jumpto (protoAdress + 1 word)
        VR2_r_add_USE (p1);      
        // prTab[prAct].ret = p1;
        //skip the name and the parameters ([..])
        p1 = VR2_r_skip_proto1 (prTab[prAct].name, pe);
        if(p1 == NULL) return -2;
        ++p1;    // skip the '{'
        goto L_next_block;

      } else {
        // write model-call
        newTyp = Typ_Model;
        VR2_r_wri_shape (iTra);
      }
    }


  // exiting-block: WORK (Shape ausgeben)
                //    12345678901234
  if       (!strncmp(wa[0], "Shape",           5)) {VR2_r_wri_shape (iTra);
  } else if(!strncmp(wa[0], "Transform",       9)) {
    newTra[0] = UT_DB_LEER;
    newRot[0] = UT_DB_LEER;
  }

  // if(level > 0) goto L_next_word;
  if(level >= 0) goto L_next_block;

  return 0;



  //================================================================
  // skip unknown block  "<wa[0] { .. }"
  L_skip_block:
      // printf(" skip unknown block |");
      // UTX_dump_s__ (p1, 32); printf("| lev=%d\n",level);
    p1 = strchr (p1, '{');
    if(p1 == NULL) return -2;
    UTX_pos_skipBrackX (&p1, pe, '}'); // skip the rest of the block
    ++p1;  // skip '}'
    goto L_next_block;



}


//================================================================
  char* VR2_r_ck_primaryBlock (int *iStat, char *ps, char *pe) {
//================================================================
//  test if this block belongs to the active model
//    is this block a PROTO or does it belong to the primaryModel
//      block is a PROTO:
//        - resolv only PROTO with active modelName;
//        - skip all primaryBlocks.
//      block belongs to the primaryModel:
//        - create a list of all PROTOs, do not resolv protos; 
//        - resolv all primaryBlocks.
//
// Output:
//   iStat  0 = continue resolving;
//          1 = skip; redo VR2_r_ck_primaryBlock

// mdlAct   -1 = work primaryModel; 0-1 = index into prTab (work this PROTO)
// mdlNam   name of model to work

  int     lc;
  char    *p1, *p2;


  // printf("VR2_r_ck_primaryBlock mdlAct=%d |",mdlAct);
  // UTX_dump_s__(ps, 32);
  // printf("|\n");
  // printf("  mdlNam = |%s|\n",mdlNam);


    //           12345
  if(strncmp(ps,"PROTO",         5)) goto L_main;


  //----------------------------------------------------------------
  // handle PROTO
  ps += 5;
  while (*ps  == ' ')  { ++ps; }

  lc = strlen(mdlNam);
  if(mdlAct < 0) goto L_pr_ne;    // resolv only primaryModel; skip all protos.

  // compare with active modelName;
  if(strncmp(mdlNam, ps, lc)) goto L_pr_skp;
  if(UTX_ckc_Del1 (ps[lc]) != 0) goto L_pr_skp;

  // resolv this PROTO: skip name & [..]; return with '{'
  p1 = VR2_r_skip_proto1 (ps, pe);
  // ++p1;  // skip '{'
  *iStat = 0; // resolv
  return p1;


  //----------------------------------------------------------------
  L_pr_skp:  // skip the proto; goto char after closing '}'
  p1 = VR2_r_skip_proto1 (ps, pe);
  if(p1 == NULL) {TX_Print("VR2_r_ck_primaryBlock E001"); return NULL;}
  // skip the { .. }
  UTX_pos_skipBrackX (&p1, pe, '}');
  ++p1;  // skip '}'
  *iStat = 1; // block skipped; redo VR2_r_ck_primaryBlock
  return p1;


  //----------------------------------------------------------------
  // ps="PROTO"; active model = primaryModel.
  // add protoname to prTab;
  // skip this PROTO: goto char after closing '}'
  L_pr_ne:
  p1 = VR2_r_dec_PROTO (ps, pe);
  *iStat = 1; // block skipped; redo VR2_r_ck_primaryBlock
  return p1;



  //----------------------------------------------------------------
  L_main:
  // handle normal primaryBlock (Group, Transform ..)

  // resolv or not ? test mdlAct
  if(mdlAct < 0) {
    *iStat = 0; // resolv, continue ..
    return ps;
  }



  // resolv not: skip the whole block ..
  // find next '{'; skip block; return with char '}'.
  p1 = strchr(ps, '{');
  if(p1 == NULL) return p1;
  UTX_pos_skipBrackX (&p1, pe, '}');
  ++p1;
  *iStat = 1; // block skipped; redo VR2_r_ck_primaryBlock
  return p1;

}


//================================================================
  int VR2_dump_wa (int wNr, char *wa[]) {
//================================================================

  int   i1;

  for(i1=0; i1<wNr; ++i1) {
    printf(" wa[%d]=|",i1);UTX_dump_s__ (wa[i1], 40); printf("|\n");
  }

  return 0;

}


//================================================================
  int VR2_dump_use (int iUse) {
//================================================================

  int   i1;

  for(i1=0; i1<iUse; ++i1) {
    printf(" use[%d] lev=%d |",i1,useLevTab[i1]);
    if(useTab[i1]) UTX_dump_s__ (useTab[i1], 40);
    printf("|\n");
  }

  return 0;

}


//================================================================
  char* VR2_r_dec_DEF (char *ps) {
//================================================================
// name in defTab speichern
// Kann mehrmals gesetzt werden; einfach überschreiben !
// DEF gilt im gesamten File !

// Input: ps is a pointer to a DEF-name in memSpc.
// add it to defTab.
// defTab is a list of pointers to the DEF-names in memSpc.

  int   i1, lc;
  char  *p1, cbuf1[128];
        
        
  // printf("VR2_r_dec_DEF %d |",defNr);
  // UTX_dump_s__(ps, 32);
  // printf("|\n");
        
        
  UTX_cp_word__(cbuf1, ps);
  lc = strlen(cbuf1);


  // check if already stored
  for(i1=0; i1<defNr; ++i1) {
    // printf(" %d=|%s|\n",i1,defTab[i1]);
    if(!strncmp(cbuf1,defTab[i1],lc)) {
      p1 = defTab[i1];
      if(UTX_ckc_Del1 (p1[lc]) == 0) {
        // printf(" -------|%s| already stored as %d\n",cbuf1,i1);
        // overwrite existing record
        defTab[i1] = ps;
        goto L_exit;
      }
    }
  }


  // save new record
  if(defNr >= defTabSiz) {TX_Error("VR2_r_dec_DEF E001"); return NULL;}

  defTab[defNr] = ps;
    // printf(" defTab[%d]=|",defNr);
    // UTX_dump_s__(defTab[defNr], 32); printf("|\n");

  ++defNr;


  L_exit:

  ps += lc;

  return ps;
}


//================================================================
  int VR2_r_dump_prTab () {
//================================================================
 
  int   i1;

  for(i1=0; i1<prNr; ++i1) {
    printf(" prTab[%d] %c %c ",i1,prTab[i1].ext,prTab[i1].mdl);
    UTX_dump_word (prTab[i1].name, " ["); printf("|\n");
  }

  return 0;

}

//================================================================
  char* VR2_r_skip_proto1 (char *ps, char *pe) {
//================================================================
//skip the name and the parameters ([..]) and return '{'

  char *p1, *p2;


  // printf(" skip_proto1-s |");UTX_dump_s__(ps, 36);printf("|\n");

  p1 = strchr(ps, '[');
  if(p1 == NULL) {TX_Print("VR2_r_skip_proto1 E001"); return NULL;}
  p2 = p1;
  UTX_pos_skipBrackX (&p2, pe, ']');

  p2 = strchr(p2, '{');
  if(p2 == NULL) {TX_Print("VR2_r_skip_proto1 E002"); return NULL;}

  // printf(" skip_proto1-e |");UTX_dump_s__(p2, 36);printf("|\n");

  return p2;

}


//================================================================ 
  char* VR2_r_dec_EPROT (char *ps, char *pe) {
//================================================================
// add EXTERNPROTO to List & skip block.

  char *p1;

  p1 = ps;
  if(*p1 == ' ') ++p1;

  // printf("VR2_r_dec_EPROT prNr=%d iPro=%d |",prNr,iPro);
  // UTX_dump_s__(p1, 32); printf("|\n");


  // // isolate proto-name -> cbuf
  // UTX_cp_word__(cbuf, ps);
  // lc = strlen(cbuf); 


  // add proto to List: see func VR2_r_dec_PROTO


  // skip the proto; skip block [ .. ]
  p1 = strchr (p1, ']');
  if(p1 == NULL) goto L_err;
  ++p1;
  while (*p1  == ' ') { ++p1; }


  // get url; if there is a [..] - block we have more than 1 url

  if(*p1 == '[') {
    // more than 1 urls with []
    p1 = strchr (p1, ']');
    if(p1 == NULL) goto L_err;
    ++p1;
    goto L_exit;
  }


  // single url without []
  if(*p1 != '"') goto L_err;

  // skip word ..
  ++p1;      // skip first '"'
  p1 = strchr (p1, '"');
  if(p1 == NULL) goto L_err;
  ++p1;


  L_exit:
  // printf("ex VR2_r_dec_EPROT prNr=%d iPro=%d |",prNr,iPro);
  // UTX_dump_s__(p1, 32); printf("|\n");

  return p1;


  L_err:
    TX_Print("**** VR2_r_dec_EPROT E_%4.4s",p1);
    return NULL;
}


//================================================================ 
  char* VR2_r_dec_PROTO (char *ps, char *pe) { 
//================================================================
// do not execute the proto; save its name in prTab.
// ext:   'P'|'E'

  int   i1, lc, ii; 
  char  *p1, *p2, *p3, *p4, cbuf[80];


  if(*ps == ' ') ++ps;

  // printf("VR2_r_dec_PROTO prNr=%d iPro=%d |",prNr,iPro);
  // UTX_dump_s__(ps, 32);
  // printf("|\n");
        

  // isolate proto-name -> cbuf
  UTX_cp_word__(cbuf, ps);
  lc = strlen(cbuf); 


  // test if proto already exists (overwrite)
  for(i1=0; i1<prNr; ++i1) {
    if(!strncmp(cbuf,prTab[i1].name,lc)) {
      ii = i1;
      goto L_prot_5;
    }
  }


  // add proto to List of existing protos.
  if(prNr >= siz_prTab) {TX_Error("VR2_r_dec_PROTO E001"); return NULL;}
  prTab[prNr].name = ps;
    // printf(" prTab[%d]=|",prNr);
    // UTX_dump_word (prTab[prNr].name, " ["); printf("|\n");
  ii = prNr;
  ++prNr;


  // skip the proto; skip block [ .. ] and then block { .. }
  L_prot_5:
  p2 = VR2_r_skip_proto1 (ps, pe);
  p3 = p2;
  if(p3 == NULL) {TX_Print("VR2_r_dec_PROTO E001"); return NULL;}

  // skip the { .. }
  i1 = UTX_pos_skipBrackX (&p3, pe, '}');
  if(i1 < 0) return NULL;


  // set .ext ('P' = PROTO; 'E' = EXTERNPROTO)
  prTab[ii].ext = 'P';

  // test if it is a inline-proto or a model-proto (model-proto contains "Shape")
  // .mdl='I' = Inline;
  // .mdl='M' = Model.
  *p3 = '\0';   // terminate space wher to search
  p4 = strstr(p2, "Shape");
  if(p4) prTab[ii].mdl = 'M';
  else   prTab[ii].mdl = 'I';
  *p3 = '}';

  ++p3;       // skip '}'

    // VR2_r_dump_prTab ();

  return p3;

}


//================================================================
  int VR2_r_ck_PROTO (char *ps) {
//================================================================
// test if ps is the name of a proto;
// RetCod
//   0=yes,ps is a proto
//  -1=yes,ps is unknown

  int   i1, lc;
  char  *p1, cbuf1[128];


  // printf("VR2_r_ck_PROTO |");UTX_dump_word(ps, " ["); printf("|\n");

  UTX_cp_word__(cbuf1, ps);
  lc = strlen(cbuf1);


  for(i1=0; i1<prNr; ++i1) {
    if(!strncmp (cbuf1, prTab[i1].name,lc)) {
        // printf(" is proto # %d\n",i1);
      return i1;
    }
  }



  return -1;

}
 

/*
//================================================================
  char* VR2_r_dec_inlinePROTO (char *ps, char *pe) {
//================================================================
// save returnaddress of inline-proto to use-stack.
// Input:
//   ps        '{' behind proto-call


  VR2_r_add_USE (p1);


  return -1;

}
*/


//================================================================
  int VR2_r_add_USE (char *ps) {
//================================================================
// add returnaddress to use-stack useTab.


  if(iUse >= siz_useTab) {
    TX_Print("VR2_r_add_USE E002");
    return -2;
  }

  // save active level
  useLevTab[iUse] = level;

  // save Ruecksprungadresse 
  useTab[iUse] = ps;

    // printf("  VR2_r_add_USE iUse=%d level=%d use |",iUse,level);
    // UTX_dump_s__(ps, 32);
    // printf("|\n");

  ++iUse;

  return 0;

}


//================================================================
  char* VR2_r_dec_USE (char *ps, char *pe) {
//================================================================
// ps is a pointer toa USE-name in meSpc.
// search the corresponding pointer in defTab; skip the name and 
// return the new startAdress.
// Skip the word in ps and save positon for returnadress in useTab.
// save the active level in useLevTab.
// was wrl1_r_dec_USE


  int   i1, lc;
  char  *p1, *p2, cbuf[80];


  if(*ps == ' ') ++ps;

  // printf("VR2_r_dec_USE def=%d use=%d |",defNr,iUse);
  // UTX_dump_s__(ps, 32);
  // printf("|\n");

        
  UTX_cp_word__(cbuf, ps);
  lc = strlen(cbuf); 


  // zugehoerigen Block suchen ...
  for(i1=0; i1<defNr; ++i1) {
    if(!strncmp(defTab[i1], cbuf, lc)) {
      p1 = defTab[i1];
      if(UTX_ckc_Del1 (p1[lc]) != 0) continue;
      // found DEF-pointer
      goto L_work;
    }
  }
  TX_Print("wrl1_r_dec_USE E001");
  return NULL;


  // skip name
  L_work:
  p1 = p1 + lc + 1;  // skip <name> in DEF
  p2 = ps + lc + 1;  // skip <name> in USE


  i1 = VR2_r_add_USE (p2);
  if(i1 < 0) return NULL;


  return p1;

}


//================================================================
  int VR2_r_wri_shape (int iTra) {
//================================================================
// write IndexedFaceSet

// was wrl1_r_dec_ifs

  int     i1, i2, ii, mdlNr;
  char    auxBuf[128];
  Point   pt1, pt2;
  Vector  vc1, vc2;
  Mat_4x3 ma1;


  // printf("================ VR2_r_wri_shape %d\n",newTyp);


  // // update transformation
  // VR2_r_upd_tra (iTra);



  //----------------------------------------------------------------
  // write geometry Typ_SUR Typ_SPH Typ_CON Typ_PRI Typ_CVPOL
  //----------------------------------------------------------------
  // Box:
  if(newTyp == Typ_PRI) {
    // S20=REC P(40 90 0) D(100 0 0) D(0 60 0)
    // B20=PRISM S20 30

    // write Attributes (color|texture):
    VR2_r_wri_att_col (iSol, 'B');

    // create points & vectors
    if(newGeo[0] == UT_DB_LEER) {
      newGeo[0] = 2.;
      newGeo[1] = 2.;
      newGeo[2] = 2.;
    }
      // printf(" - box: %f %f %f\n",newGeo[0],newGeo[1],newGeo[2]);
    pt1.x = -(newGeo[0] / 2.);
    pt1.y = -(newGeo[1] / 2.);
    pt1.z = -(newGeo[2] / 2.);

    UT3D_vc_3db (&vc1, newGeo[0], 0., 0.); // X-vektor
    UT3D_vc_3db (&vc2, 0., -newGeo[2], 0.); // CAD-Y-Vektor

    // transform points & vectors
    VR_pt_vr2cad (&pt1);
    UT3D_pt_tra_pt_m3 (&pt1, maTab[iTra], &pt1);
    UT3D_vc_tra_vc_m3 (&vc1, maTab[iTra], &vc1);
    UT3D_vc_tra_vc_m3 (&vc2, maTab[iTra], &vc2);

    sprintf(mem_cbuf1, "S%ld=REC ",iCrv);
    AP_obj_add_pt (mem_cbuf1, &pt1);
    AP_obj_add_vc0 (mem_cbuf1, &vc1);
    AP_obj_add_vc0 (mem_cbuf1, &vc2);
    UTF_add1_line (mem_cbuf1);
    ++iCrv;

    sprintf(mem_cbuf1, "B%ld=PRISM S%ld ",iSol,iCrv-1);
    AP_obj_add_val (mem_cbuf1, -newGeo[1]);   // CAD-Z-Value
    UTF_add1_line (mem_cbuf1);
    ++iSol;


  //----------------------------------------------------------------
  // Sphere:
  } else if(newTyp == Typ_SPH) {
    // B20=SPH P(-6.505 10.501 0) 3

    // write Attributes (color|texture):
    VR2_r_wri_att_col (iSol, 'B');

    // create points & vectors
    if(newGeo[0] == UT_DB_LEER) {
      newGeo[0] = 1.;
    }
      // printf(" - sph: %f\n",newGeo[0]);
    pt1 = UT3D_PT_NUL;

    // transform points & vectors
    UT3D_pt_tra_pt_m3 (&pt1, maTab[iTra], &pt1);

    sprintf(mem_cbuf1, "B%ld=SPH ",iSol);
    AP_obj_add_pt (mem_cbuf1, &pt1);
    AP_obj_add_val (mem_cbuf1, newGeo[0]);
    UTF_add1_line (mem_cbuf1);
    ++iSol;


  //----------------------------------------------------------------
  // Cone:
  } else if(newTyp == Typ_CON) {
    // B21=CON P20 P21 VAL(4) VAL(2)
    // write Attributes (color|texture):
    VR2_r_wri_att_col (iSol, 'B');

    // create points & vectors
    if(newGeo[0] == UT_DB_LEER) {
        // printf(" cone set defaults\n");
      newGeo[0] = 1.;
      newGeo[1] = 2.;   // height
    }
      // printf(" - cone: %f\n",newGeo[0]);
    pt1 = UT3D_PT_NUL;
    pt1.y = - (newGeo[1] / 2.);
    pt2 = UT3D_PT_NUL;
    pt2.y = newGeo[1] / 2.;

    // transform points & vectors
    VR_pt_vr2cad (&pt1);
    VR_pt_vr2cad (&pt2);
    UT3D_pt_tra_pt_m3 (&pt1, maTab[iTra], &pt1);
    UT3D_pt_tra_pt_m3 (&pt2, maTab[iTra], &pt2);

    sprintf(mem_cbuf1, "B%ld=CON ",iSol);
    AP_obj_add_pt (mem_cbuf1, &pt1);
    AP_obj_add_pt (mem_cbuf1, &pt2);
    AP_obj_add_val (mem_cbuf1, newGeo[0]);  // radius
    strcat (mem_cbuf1, " 0");
    UTF_add1_line (mem_cbuf1);
    ++iSol;


  //----------------------------------------------------------------
  // Cylinder:
  } else if(newTyp == Typ_SOL) {      // sym for cyl does not exist ..
    // B21=CON P20 P21 VAL(4) VAL(2)
    // write Attributes (color|texture):
    VR2_r_wri_att_col (iSol, 'B');

    // create points & vectors
    if(newGeo[0] == UT_DB_LEER) {
      newGeo[0] = 1.;
      newGeo[1] = 2.;    // height
    }
    pt1 = UT3D_PT_NUL;
    pt1.y = - (newGeo[1] / 2.);   // height
    pt2 = UT3D_PT_NUL;
    pt2.y = newGeo[1] / 2.;   // height

    // transform points & vectors
    VR_pt_vr2cad (&pt1);
    VR_pt_vr2cad (&pt2);
    UT3D_pt_tra_pt_m3 (&pt1, maTab[iTra], &pt1);
    UT3D_pt_tra_pt_m3 (&pt2, maTab[iTra], &pt2);

    sprintf(mem_cbuf1, "B%ld=CON ",iSol);
    AP_obj_add_pt (mem_cbuf1, &pt1);
    AP_obj_add_pt (mem_cbuf1, &pt2);
    AP_obj_add_val (mem_cbuf1, newGeo[0]);   // rad
    AP_obj_add_val (mem_cbuf1, newGeo[0]);   // rad
    UTF_add1_line (mem_cbuf1);
    ++iSol;


  //----------------------------------------------------------------
  // IndexedFaceSet:
  } else if(newTyp == Typ_SUR) {

    // transform points & vectors
    for(i1=0; i1<pTab.rNr; ++i1) {
      VR_pt_vr2cad (&pTab.data[i1]);
      UT3D_pt_tra_pt_m3 (&pTab.data[i1], maTab[iTra], &pTab.data[i1]);
    }

    // test PTAB or POLygon
    ii = VR2_wrsur_ck ();
      // printf(" typsur=%d\n",ii);
    if(ii > 0) goto L_wrsur_ptab;


    //----------------------------------------------------------------
    // create surface from S=POL & A=S

    // write Attributes (color|texture):
    VR2_r_wri_att_hide (iCrv, 'S');
    VR2_r_wri_att_col (iSur, 'A');


    // wr S#=POL# p p
    sprintf(mem_cbuf1, "S%ld=POL ",iCrv);
    for(i1=0; i1<pTab.rNr; ++i1)
      AP_obj_add_pt (mem_cbuf1, &pTab.data[i1]);
    UTF_add1_line (mem_cbuf1);

    // write A#=S#
    sprintf(mem_cbuf1, "A%ld=S%ld",iSur,iCrv);
    UTF_add1_line (mem_cbuf1);
    ++iCrv;
    ++iSur;
    return 0;


    //----------------------------------------------------------------
    // create surface from PTAB & MSH
    L_wrsur_ptab:

    // write Attributes (color|texture):
    // IndexedFaceSet: 2 surfaces (iSur + 1) !
    VR2_r_wri_att_hide (iSur, 'A');
    VR2_r_wri_att_col (iSur + 1, 'A');

    // write points -> PTAB
    // mdlNr = -1;
    // MSH_bsav_pTab (&pTab, mdlNr, iSur);
    MSH_bsav_pTab (&pTab, "", iSur);

    // write A1=PTAB "A1"
    sprintf(mem_cbuf1, "A%ld=PTAB \"A%ld\"",iSur,iSur);
    UTF_add1_line (mem_cbuf1);
    ++iSur;

    // write faces -> MSH
    // MSH_eTab_fix ?
    // MSH_bsav_fTab (&fTab, &eTab, &eDat, mdlNr, iSur);
    MSH_bsav_fTab (&fTab, &eTab, &eDat, "", iSur);

    // write A2=MSH A1
    sprintf(mem_cbuf1, "A%ld=MSH A%ld",iSur,iSur-1);
    UTF_add1_line (mem_cbuf1);
    ++iSur;



  //----------------------------------------------------------------
  // IndexedLineSet:
  } else if(newTyp == Typ_CVPOL) {
    // write Attributes (color|texture):
    // IndexedFaceSet: 2 surfaces (iSur + 1) !
    VR2_r_wri_att_col (iSur + 1, 'A');

    // transform points & vectors
    for(i1=0; i1<pTab.rNr; ++i1) {
      VR_pt_vr2cad (&pTab.data[i1]);
      UT3D_pt_tra_pt_m3 (&pTab.data[i1], maTab[iTra], &pTab.data[i1]);
    }

    // add -1 as last index
    if(iTab[iTabNr - 1] >= 0) { ++iTabNr; iTab[iTabNr] = -1;}


    // work
    // printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX \n");
    // printf(" LineSet: %d\n",iTabNr);
    i1 = 0;
    for(;;) {
      // find nr of points for nxt line
      for(i2=i1; i2<iTabNr; ++i2) {
        if(iTab[i2] >= 0) continue;
        ii = i2 - i1;
          // printf(" Line %d - %d\n",i1,i2);
        // write
        VR2_r_wri_Plg (i1, ii);
        // setup for nxt ln
        i1 = i2 + 1;
        break;
      }
      if(i2 >= iTabNr) break;
    }





  //----------------------------------------------------------------
  // subModel
  // M#="modelname" P(origin) D(vz) D(vx)
  } else if(newTyp == Typ_Model) {

    // create points & vectors
    if(newpTra[0] == UT_DB_LEER) {
      newpTra[0] = 0.;
      newpTra[1] = 0.;
      newpTra[2] = 0.;
    }
    if(newpRot[0] != UT_DB_LEER) {
        // printf(" newpRot %f %f %f %f\n",newpRot[0],newpRot[1],
                                        // newpRot[2],newpRot[3]);
      newRot[3] *= -1.;
      VR_vc_vr2cad ((Vector*)newpRot);
      UT3D_m3_rot_m3 (ma1, UT3D_MAT_4x3, (Vector*)newpRot, &newpRot[3]);
    } else {
      UT3D_m3_copy (ma1, UT3D_MAT_4x3);
    }
      // printf(" - subModel: %f %f %f\n",newpTra[0],newpTra[1],newpTra[2]);
    // transform points & vectors
    VR_pt_vr2cad (&newpTra);
    UT3D_pt_3db (&pt1, newpTra[0],newpTra[1],newpTra[2]);
    UT3D_pt_tra_pt_m3 (&pt1, maTab[iTra], &pt1);
    // get vz -> vc1, vx -> vc2
    // UT3D_m3_get (&vc1, 2, maTab[iTra]);
    // UT3D_m3_get (&vc2, 0, maTab[iTra]);
    UT3D_m3_get (&vc1, 2, ma1);
    UT3D_m3_get (&vc2, 0, ma1);

    // extract subModelname prTab[prAct].name
    UTX_cp_word__(auxBuf, prTab[prAct].name);
    sprintf(mem_cbuf1, "M%ld=\"%s\" ",iMdl,auxBuf);
    AP_obj_add_pt (mem_cbuf1, &pt1);
    AP_obj_add_vc0 (mem_cbuf1, &vc1);
    AP_obj_add_vc0 (mem_cbuf1, &vc2);
    UTF_add1_line (mem_cbuf1);
    ++iMdl;






  //----------------------------------------------------------------
  } else {
    TX_Error ("VR2_r_wri_shape E001",newTyp);
    return -1;
  }

  return 0;

}


//================================================================
  int VR2_r_wri_att_hide (long dbi, char cTyp) {
//================================================================
// cTyp:  'A' or 'B'
    
  char  cbuf[80];

  //----------------------------------------------------------------
  // write attributes (colour or texture)
  // Color: ":ATTRIB:A20 C39cc30"

    sprintf(cbuf, ":ATTRIB:%c%ld H",cTyp,dbi);
    UTF_add1_line (cbuf);

  return 0;

}


//================================================================
  int VR2_r_wri_att_col (long dbi, char cTyp) {
//================================================================
// cTyp:  'A' or 'B'

  char  cbuf[80];


  //----------------------------------------------------------------
  // write attributes (colour or texture)
  // Color: ":ATTRIB:A20 C39cc30"
    
    sprintf(cbuf, ":ATTRIB:%c%ld C%02x%02x%02x",cTyp,dbi,
            newCol.cr,newCol.cg,newCol.cb);

    UTF_add1_line (cbuf);

  return 0;

}


//================================================================
  int VR2_r_wri_Plg (int is, int in) {
//================================================================
// write Line or Polygon
 
  int  i1;

  // printf("VR2_r_wri_Plg %d %d\n",is,in);


  if(in > 2) goto L_plg;

  sprintf(mem_cbuf1, "L%ld=",iLin);
  ++iLin;

  AP_obj_add_pt  (mem_cbuf1, &pTab.data[is]);
  ++is;
  AP_obj_add_pt  (mem_cbuf1, &pTab.data[is]);
  goto L_exit;


  //----------------------------------------------------------------
  L_plg:
  sprintf(mem_cbuf1, "S%ld=POL ",iCrv);
  ++iCrv;

  for(i1=0; i1<in; ++i1) {
    AP_obj_add_pt  (mem_cbuf1, &pTab.data[is]);
    ++is;
  }



  //----------------------------------------------------------------
  L_exit:
  UTF_add1_line (mem_cbuf1);

  return 0;

}


//================================================================
  int VR2_r_upd_tra (int iTra) {
//================================================================

  // int  iAct;
  Quat q1;

  // printf("VR2_r_upd_tra %d\n",iTra);

  // iAct = *iTra + 1;

  if(newTra[0] == UT_DB_LEER) goto L_upd_rot;

  // move origin
  VR_vc_vr2cad ((Vector*)newTra);
    // printf(" tra move %f %f %f\n",newTra[0],newTra[1],newTra[2]);
  UT3D_m3_traVc (&maTab[iTra], (Vector*)newTra);

  newTra[0] = UT_DB_LEER;



  //================================================================
  L_upd_rot:
  if(newRot[0] == UT_DB_LEER) goto L_upd_exit;
  // if(iAct > *iTra) {
    // *iTra = iAct;
    // UT3D_m3_copy (&maTab[iAct], &maTab[iAct-1]);
  // }
    // printf(" newRot: %f %f %f %f\n",newRot[0],newRot[1],newRot[2],newRot[3]);


  // rotate matrix
  newRot[3] *= -1.;
  VR_vc_vr2cad ((Vector*)newRot);
    // printf(" tra rot  %f %f %f %f\n",newRot[0],newRot[1],newRot[2],newRot[3]);
  UT3D_m3_rot_m3 (&maTab[iTra], &maTab[iTra], (Vector*)newRot, &newRot[3]);

  newRot[0] = UT_DB_LEER;



  //================================================================
/*  scale ..
  UT3D_m3_get ((double*)&v1, 0, maTab[level].ma);
  v1.dx *= newScl[0];
  v1.dy *= newScl[0];
  v1.dz *= newScl[0];
  UT3D_m3_set (maTab[level].ma, 0, &v1);

  UT3D_m3_get ((double*)&v1, 1, maTab[level].ma);
  v1.dx *= newScl[1];
  v1.dy *= newScl[1];
  v1.dz *= newScl[1];
  UT3D_m3_set (maTab[level].ma, 1, &v1);

  UT3D_m3_get ((double*)&v1, 2, maTab[level].ma);
  v1.dx *= newScl[2];
  v1.dy *= newScl[2];
  v1.dz *= newScl[2];
  UT3D_m3_set (maTab[level].ma, 2, &v1);
*/



  //================================================================
  L_upd_exit:
    // DEB_dump_obj__ (Typ_M4x3, &maTab[iTra], "maTab[%d]",iTra);

  return 0;

}


//================================================================
  char* VR2_r_dec_points (char *ps) {
//================================================================
// "{ point [ <points> ] }"

  int     irc;
  char    *p1, *pe;
  long    ld;
  Point   pt1;


  // printf("VR2_r_dec_points |");UTX_dump_s__(ps, 32);printf("|\n");


  MEMTAB_CLEAR (&pTab);
    // printf(" pTab-rMax=%d\n",pTab.rMax);

  p1  = strchr(ps, '[');
  if(p1 == NULL) {TX_Error("VR2_r_dec_points E001"); return NULL;}
  ++p1;   // skip '['
  
  pe  = strchr(p1, ']');
  if(pe == NULL) {TX_Error("VR2_r_dec_points E002"); return NULL;}
  


  //----------------------------------------------------------------
  L_nxt_pt:

  pt1.x = UTX_db_tx (&p1, p1); ++p1;
  pt1.y = UTX_db_tx (&p1, p1); ++p1;
  pt1.z = UTX_db_tx (&p1, p1); ++p1;
    // DEB_dump_obj__ (Typ_PT, &pt1, "  vTab[%d]",vTabNr);


  // save point; realloc if necessary ..
  irc = MemTab_sav (&pTab, &ld, &pt1, 1);
  if(irc < 0) {TX_Error("VR2_r_dec_points EOM"); return NULL;}

  // vTab[vTabNr] = pt1;
  // if(vTabNr < vTabSiz) ++vTabNr;
  // else {TX_Error("VR2_r_dec_points EOM"); return NULL;}

  p1 = UTX_pos_skipDeli1 (p1);  // skip delimiter ' ' ',' '\n' '\r' '\t'

  if(p1 < pe) goto L_nxt_pt;



  //----------------------------------------------------------------
  p1 = pe;
  ++p1;

  return p1;

}


//================================================================
  char* VR2_r_dec_index (char *ps) {
//================================================================
// "coordIndex [ <indexes> ]"
// coordIndex [ 0, 1, 7, 6, -1, 1, 2, 8, -1 ]
// ein face darf beliebeig viele punkte haben, sie muessen aber kreisfoermig
// sein (nicht selfintersecting); also FAN.
// End of Fan ist ein -1; darf beim letzten Fan fehlen.

  int   irc;
  char  *p1, *pe;


  // printf("VR2_r_dec_index |");UTX_dump_s__(ps, 32);printf("|\n");

  iTabNr = -1;
  MEMTAB_CLEAR (&fTab);

  // search '['
  p1  = strchr(ps, '[');
  if(p1 == NULL) {TX_Error("VR2_r_dec_index E001"); return NULL;}
  ++p1;   // skip '['

  pe  = strchr(p1, ']');
  if(pe == NULL) {TX_Error("VR2_r_dec_index E002"); return NULL;}



  //----------------------------------------------------------------
  L_nxt_fac:
  // Polygons: add all into iTab; Surfaces: add one by one into fTab.
  if(newTyp != Typ_CVPOL) iTabNr = -1;


  L_nxt_ind:
  ++iTabNr;
  if(iTabNr >= iTabSiz) {TX_Error("VR2_r_dec_index E003"); return NULL;}


  iTab[iTabNr] = strtol (p1, &p1, 10); ++p1;

  if(p1 < pe) {
    if(iTab[iTabNr] >= 0) goto L_nxt_ind;
  }

  if(iTab[iTabNr] >= 0) ++iTabNr;     // end of polygon maybe without -1

  // store fan iTab as 1-n faces
  if(newTyp != Typ_CVPOL) {
    irc = VR2_r_sav_indFac ();
  }
  if(irc < 0) return NULL;


  if(p1 < pe) goto L_nxt_fac;



  //----------------------------------------------------------------

  ++pe;   // skip ']'

  return pe;

}


//================================================================
  int VR2_r_sav_indFac () {
//================================================================
// store fan iTab as 1-n faces

// see GL_DrawFan

  int    i1, irc;
  long   ld;
  Fac3   fc1;


  // printf("VR2_r_sav_indFac iNr=%d newTyp=%d\n",iTabNr,newTyp);
  // for(i1=0; i1<iTabNr; ++i1) printf(" iTab[%d]=%d\n",i1,iTab[i1]);

  // remove last index, if it is equal as first index
  if(iTab[0] == iTab[iTabNr-1]) --iTabNr;

  // resolv fan
    fc1.i1 = iTab[0];
    for(i1=1; i1<iTabNr-1; ++i1) {
      fc1.i2 = iTab[i1];
      fc1.i3 = iTab[i1+1];
        // printf(" fc1 = %d %d %d\n",fc1.i1,fc1.i2,fc1.i3);
        irc = MemTab_sav (&fTab, &ld, &fc1, 1);
        if(irc < 0) return -1;
    }



  return 0;

}


//================================================================
  int VR2_wrsur_ck () {
//================================================================
// test PTAB or POLygon
// RetCod:  0    use POLygon
//          1    use PTAB & MSH
// use POLygon if:
//  - less than 100 points;
//  - all points are numbered subsequent.

  int   i1;


  // printf("VR2_wrsur_ck %d\n",iTabNr);
  // for(i1=0; i1<iTabNr; ++i1) printf(" iTab[%d]=%d\n",i1,iTab[i1]);


  if(iTabNr > 100) return 1;


  for(i1=0; i1<iTabNr; ++i1) {
    if(iTab[i1] != i1) return 1;
  }

  return 0;

}


//================================================================
  char* VR2_r_skip_expr (char *ps, char ce) {
//================================================================
// skip this block ..

  char    *p1;


  // printf("VR2_r_skip_expr |");UTX_dump_s__(ps, 32);printf("|\n");

  p1 = strchr(ps, ce);
  if(p1 == NULL) {TX_Error("VR2_r_skip_expr E001"); return NULL;}

  ++p1;  // skip ce

  return p1;

}


//================================================================
  char* VR2_r_skip_word (int inr, char *p1) {
//================================================================
// skip entweder inr words oder if next char is [ skip whole block !
// oder if next char is '~' 
// ret. Pos. of first char after word


  char   *p2, *p3;


  while (*p1  == ' ')  { ++p1; }
  p2 = p1;

  // printf("VR2_r_skip_word: skip %d words;\n",inr);
  // printf("VR2_r_skip_word %d |",inr);
  // UTX_dump_s__ (p2);
  // printf("|\n");


  if(*p2 == '~') {    // zB ambientColor  ~
   ++p2;
   return p2;
  }

  if(*p2 == '[') {   // zB emissiveColor  [ 0.5 0.5 0.7 ] 
   p3 = strchr(p2, ']');
   if(p3 == NULL) {TX_Error("VR2_r_skip_word E001"); return NULL;}
   ++p3;
   return p3;
  }


  p2 = UTX_pos_skipWords (inr, p1);

  return p2;
}


//================================================================
  char* VR2_r_dec_floats (double *da, char *ps, int vNr) {
//================================================================
// store double-values into double-array
// Test for "radius IS radius"
    
  char   *p2;

  // printf("VR2_r_dec_floats |");UTX_dump_s__ (ps, 32); printf("|\n");



  // "IS ": skip this and the next word.
  if(!strncmp(ps, "IS ", 3)) {
    ps += 3;
    p2 = VR2_r_skip_word (1, ps);
    return p2;
  }


  
  //----------------------------------------------------------------
  da[0] = UTX_db_tx (&p2, ps);
  if(vNr > 1)
    da[1] = UTX_db_tx (&p2, p2);
  if(vNr > 2)
    da[2] = UTX_db_tx (&p2, p2);
  if(vNr > 3)
    da[3] = UTX_db_tx (&p2, p2);

    // printf(" dec_floats: %f %f %f %f\n",da[0],da[1],da[2],da[3]);
  
  return p2;

} 


//================================================================
  char* VR2_r_dec_cold (char *ps) {
//================================================================
// store diffuseColor
// diffuseColor 0.18 0.88 0.18

// see wrl1_r_dec_Col_ Col_set__ !!!!!

  char   *p2;
  double d1, d2, d3;

  // printf("VR2_r_dec_cold |");UTX_dump_s__ (ps, 32); printf("|\n");

  d1 = UTX_db_tx (&p2, ps);
  d2 = UTX_db_tx (&p2, p2);
  d3 = UTX_db_tx (&p2, p2);

  Col_set_3db (&newCol, d1, d2, d3);

  return p2;

}


// EOF
