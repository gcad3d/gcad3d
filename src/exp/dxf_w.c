// DXF export.                                    RF 2010-09-22
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
Resolv DIMENSIONs into dummy-blocks "*D#"  (DXFW_DIM())
  ..

-----------------------------------------------------
Modifications:
2016-06-02 write mockup-models -> 3DFACE. RF.
2015-10-21 completely rewritten. RF.
2004-11-12 DIMENSION, LWPOLYLINE zu. RF.
2003-11-06 ELLIPSE zu. RF.
2002-12-16 Neue Curve-Datenstruktur.RF.
2002-02-21 Portierung -> W32. RF.
2001-06-17 Portierung -> Linux. RF.

-----------------------------------------------------
*/
/*!
\file  ../exp/dxf_w.c
\brief write DXF
\code
=====================================================
List_functions_start:

DXFW__           main entry
DXFW_prolog
DXFW_blk_ini
DXFW_main
DXFW_Mdl_gcad
DXFW_Mdl_tess
DXFW_ox
dxfw_ELLIPSE
dxfw_SPLINE
DXFW_VERTEX3
DXFW_VERTEX2
DXFW_CI
DXFW_TEXT
DXFW_POLYLN3
DXFW_POLYLN2
DXFW_3DFACE__
DXFW_3DFACE_out
DXFW_INSERT

DXFW_point3
DXFW_point2
DXFW_vector
DXFW_fl_out

dxfw_load_mat  make matrix for  DXF-ECS

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


make -f xa_dxf_w.mak

==============================================================================
Memory-Usage  memspc (../xa/xa_mem.h):

dxf-Write:
   memspc55   list_of_subModels dxfw_smTab
   memspc011  f Text ..
   memspc201  f curves ..


==============================================================================
DXF-Out:
 Alle Punkte und Linien gehen als 3D-Koord. raus;
 Kreise liegen (ausser X-Y-planar) auf einer Ebene, von der der Abstand zum absol.
 Nullpunkt und der Normalvektor angegeben werden. Daher alle Obj in 3D (fertig
 tranformiert).


INSERT:
Das sind in etwa Details; die Entities eines Insert-Blocks sind in der
BLOCKS-Section definiert und in das gesamte Detail wird (mit eigenem
Nullpunkt und Verdrehwinkel) in der ENTITIES-Section ein- oder mehrmals
mit INSERT aufgerufen.


0/BLOCK/../2/blockname   (... in der BLOCKS-Section)
 wird auch fuer DIMENSIONS verwendet.
0/DIMENSION/../2/blockname   (in ENTITIES-Section; der Rest hier useless).



Offen:
$DIMTM MinusTol
$DIMTP PlusTol
$DIMTAD ?



==============================================================================
DXF-Dateiformat:
================

Prinzipiell besteht jede Information aus 2 Zeilen.
  Die 1. ist der Recordtyp (3-stelliges Int), die 2. zeile hat 3 versch. Formate:
  Text (linksbündig, bis 255 Char's. zB Layernames ..)
  Integers (6-stellig rechtsbündig)
  Real (Freiformat, zB 0.0)


Die Datei besteht aus mehreren Sections (SECTION ... ENDSEC), dann "EOF".

  HEADER   - Section (DXF-Versionsnummer, $TEXTSIZE, $DIMASZ ..)
  TABLES   - Section (LineTypTable, LayerTable)
  BLOCKS   - Section (subModel-objects)
  ENTITIES - Section (objects mainModel)
  EOF - Record.


Die folgenden Entities haben alle Recordtyp (= erste Zeile) 0. Es folgt dann als
  2. Zeile der Text (zB POINT), dann kommen die Zusatzinfos.

  POINT    aus Pos (10/20/30)
  LINE     aus Anfpt. (10/20/30) und Endpkt (11/21/31)
  ARC      aus Mittelpunkt (10/20/30),Radius(40), Anf.Winkel(50), Endwinkel(51).
           Immer CCW!
  CIRCLE   aus Mittelpunkt (10/20/30) und ?? (34) und Radius (40)
  TEXT     aus Position (10/20/30), Höhe (40), Text (1) und Winkel (50)
  POLYLINE besteht aus  Punkt ?? (10/20/30), dann
           mehreren 0 / VERTEX bis 0 / SEQEND.
           Code70: Bit1=closed-Info (VERTEX can have also its Code70 !)
  LWPOLYLINE  Code90 gives Nr of following points; all points come as 10,20.
  INSERT   10,20,30=Origin; Code2=BlockNam; 41=X-Scale; 42=Y-Scale; 43=Z-Scale.
           50=RotAng; 70/71=column/row-count; 44/45=column/row-spacing.
           Blocks are in "SECTION BLOCKS"; starting with "BLOCK",
           EndOfBlock = ENDBLK
           66=AttribsFollowFlag(1=Yes; Atribs end with SEQEND).
           blocks  (mit ASHADE und AVE_RENDER, ... glass.dxf) dzt skippen.
  ATTRIB   ein Textattribut; im Anschluss an INSERT, Ende mit SEQEND.
           10,20,30=textPosition; 40=textHeight;  2=AttributeID; 1=Value;
           41=relativeScale;  50=Angle
  VERTEX   Pos (10/20/30) fuer zB POLYLINE
  SEQEND   Ende POLYLINE

  3DFACE   4 Punkte; (10/20/30-13/23/33)
           8=Layer, 62=Color.

  DIMENSION  Text(1) ist leer ! 10/20, 13/23, 14/24, 50 70 39 62

  Ab Autocad-R13:

  ELLIPSE  CenterPt (10/20/30) relativer Endpt der Hauptachse (11/21/31)
           Z-Vektor (210/220/230)
           Faktor LaengeHauptachse/LaengeNebenachse (40)
           Startwinkel,Endwinkel (0.-2Pi) (41/42)
           Immer CCW!

--------------- skip this records: -----------------------------------
ATTDEF    Definitionsbeschreibung;  skip it ..
          1=defaultValue; 3=promptString; 2=AttributeID;
          10,20,30=textPosition; 40=textHeight;
          41=relativeScale;  50=Angle


-----------------------------------------------------------------------
Zusatzinfos: Fast fuer jedes Ent. gibts den Layer; 8 / 3  (zwei Zeilen)
  definiert Layer # 3.
  0-9 String; 10-59 float; 60-79 int; ..

  Text          1    zu Typ "TEXT"
  Name          2    zB BLOCK-Name (="Detail"-Name)
  Ent.Name      5    zB "     B" oder "    4C". Für LINE und VERTEX.
  LinetypName   6    (Char, zB CONTINUOUS)
  Textstyle     7    zB "normal#0.300000#0.900000" od cursive#0.500000#3.000000
  Layernummer   8    (Int)
  X-Koord      10-18
  Y-Koord      20-28
  Z-Koord      30-37
  Dicke        39    nur wenn ungleich 0.0
  allg. Werte  40-48 Radius für CIRCLE und ARC.
                     ?? für VERTEX (40 UND 41 !)   TEXT-size
  ?            48    0-1 (0.098159) bei Line; Display-Z-Value ?
               49    Tabellenwerte (wiederholen sich)
  Winkel       50-58 Degree.
  Farbnummer   62    (Int); 1=Rot, 2=Gelb, 3=Gruen, 4=Cyan, 5=Blau,
                         6=Magenta(violett), 7=Schwarz (see DXF_colACI_colRGB)
                         256=BYLAYER ?
                         manchmal als 6-Char-Int.
  EntityStart  66    zB "     1" (I6 ?) bei POLYLINE-Header.
  Int-Werte    70-78 bei POLYLINE-Header zB 17,6,18. ??
               70 bei DIMENSION: Typ.160 161 162 163 164
  ?            100   "AcDbLine" bei LINE ??
  Z-Vektor     210,220,230  Z-Vektor, wenn <> dem Haupt-Z-Vektor.
  Kommentar    999   Kommentarzeile.

  -            370   Lineweight enum value; none = -1; (TABLE/LAYER)



Versionen (stehen in HEADER-Section):
----------                                    dxfw_subtyp = AP_stat.subtyp
  9 / $ACADVER / 1 / AC1006  = R10            0
  9 / $ACADVER / 1 / AC1009  = R11 and R12    1
  9 / $ACADVER / 1 / AC1014  = R14            2
  9 / $ACADVER / 1 / AC1015  = AutoCAD 2000   3
  no SECTION HEADER ..                      >=90




==========================================================================
SECTION TABLES:

LAYER:
2 / Layername
70 / 1=frozen, 4=locked, 16=external (xref)a, 32=resolved xref
62 / color; if negative: off !
6 / Lintypename
370 / Lineweight enum value


TABLE 2 VPORT .. def of viewport ???
TABLE 2 LTYPE .. def of all linetypes ?
TABLE 2 LAYER
TABLE 2 STYLE
TABLE 2 VIEW
TABLE 2 UCS
TABLE 2 DIMSTYLE
TABLE 2 BLOCK_RECORD

  0 ENDTAB 0 ENDSEC






==========================================================================
Koordinatensystem:
Alle DXF-Typen 10 bis 37 sind immer im WCS (WorldCoordinateSystem = Hauptachsen).
  POINT und LINE immer WCS.

CIRCLE und ARC brauchen ein ECS (EntityCoordinateSystem). Dieses wird mit einem
  Vektor 210,220,230 definiert.



For a given Z axis (or extrusion) direction, there is an infinite number of
coordinate systems, defined by translating the origin in 3D space and by
rotating the X and Y axes around the Z axis.  However, for the same Z axis
direction, there is only one Entity Coordinate System.  It has the follow-
ing properties:

  o  Its origin coincides with the WCS origin.

xxxxxxxxxxxxxxxxxxx
Given a unit-length vector to be used as the Z axis of a coordinate system,
the arbitrary axis algorithm generates a corresponding X axis for the coor-
dinate system.  The Y axis follows by application of the right hand rule.

The method is to examine the given Z axis (also called the normal vector)
and see if it is close to the positive or negative World Z axis.  If it is,
cross the World Y axis with the given Z axis to arrive at the arbitrary X
axis.


1) Der Vektor sei die neue ECS-Z-Achse.
2) Die neue X-Achse ist das Kreuzprodukt der World-Z-Achse mit der neuen Z-Achse
3) Die neue Y-Achse ist das Kreuzprodukt der neuen Z-Achse mit der neuen X-Achse

If not, cross the World Z axis with the given Z axis to arrive at
the arbitrary X axis.
    Let the given normal vector be called N.
    Let the World Y axis be called Wy, which is always (0,1,0).
    Let the World Z axis be called Wz, which is always (0,0,1).

We are looking for the arbitrary X and Y axes to go with the normal N.
They'll be called Ax and Ay.  N could also be called Az (the arbitrary Z
axis).

       Ax = Wz * N.
    Scale Ax to unit length.

The method of getting the Ay vector would be:

    Ay = N * Ax.
    Scale Ay to unit length.


          Point entities have an optional 50 group that determines
          the orientation of PDMODE images. The group value is the
          negative of the Entity Coordinate Systems (ECS) angle of
          the UCS X axis in effect when the point was drawn. The X
          axis of the UCS in effect when the point was drawn is
          always parallel to the XY plane for the point's ECS, and
          the angle between the UCS X axis and the ECS X axis is a
          single 2D angle. The value in group 50 is the angle from
          horizontal (the effective X axis) to the ECS X axis.
          Entity Coordinate Systems (ECS) are described later in
          this section.

          In addition, all dimension types have an optional group
          (code 51) that indicates the horizontal direction for the
          Dimension entity. This determines the orientation of
          dimension text and dimension lines for horizontal,
          vertical, and rotated linear dimensions. The group value
          is the negative of the Entity Coordinate Systems (ECS)
          angle of the UCS X axis in effect when the Dimension was
          was drawn is always parallel to the XY plane for the
          Dimension's ECS, and the angle between the UCS X axis and
          the ECS X axis is a single 2D angle. The value in group 51
          is the angle from horizontal (the effective X axis) to the
          ECS X axis. Entity Coordinate Systems (ECS) are described
          later in this section.

          Linear dimension types with an oblique angle have an
          optional group (code 52). When added to the rotation angle
          of the linear dimension (group code 50) this gives the
          angle of the extension lines. The optional group code 53
          is the rotation angle of the dimension text away from its
          default orientation (the direction of the dimension line).




-------------------------------------------------------------------
*/

#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif

#define DXFW_VERSION "gCAD3D-DXFW 2016-03-11"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef _MSC_VER
__declspec(dllexport) int DXFW__ (char*);
#define extern __declspec(dllimport)
#endif


#include "../ut/ut_geo.h"
#include "../ut/ut_txt.h"
#include "../ut/ut_TX.h"
#include "../ut/ut_os.h"                  // OS_get_bas_dir ..
// #include "../ut/ut_txTab.h"               // TxtTab
#include "../ut/ut_memTab.h"              // MemTab_..

#include "../gr/ut_DL.h"                  // DL_GetAtt
#include "../gr/ut_GL.h"                  // GL_GetCen
#include "../gr/ut_gr.h"                  // GTX_chh_
#include "../ut/func_types.h"                  // SYM_SQUARE ..
#include "../ut/gr_types.h"               // SYM_* ATT_* LTYP_*
#include "../gr/tess_su.h"                // TypTsuSur

#include "../db/ut_DB.h"                  // DB_GetObjGX

#include "../xa/xa_msg.h"                 // MSG_ERR_typ_*
#include "../xa/xa_mem.h"                 // memspc501
#include "../xa/xa.h"                     // AP_STAT



typedef_MemTab(int);


//===========================================================================
// EXTERNALS:

// from ../xa/xa.c:
extern Plane      WC_sur_act;     // constPln
extern double     AP_txsiz;       // Notes-Defaultsize
extern double     AP_txdimsiz;    // Dimensions-Text-size
extern AP_STAT    AP_stat;



//===========================================================================
// LOCALS:

static int       dxfw_subtyp;
static int       dxfw_errNr;
static int       dxfw_objNr;

static FILE *fpo1;                             // output
static MemTab(int) dxfw_smTab = _MEMTAB_NUL;  // list of subModels used



//===========================================================================
// PROTOTYPES:

int DXFW_tess_CB (ObjGX*);







//===========================================================================
int DXFW_test (char *txt1) {
//===========================================================================

  printf("DXFW_test %s UT3D_VECTOR_Y = %f %f %f\n", txt1,
           UT3D_VECTOR_Y.dx, UT3D_VECTOR_Y.dy, UT3D_VECTOR_Y.dz);
  printf(" UT3D_VECTOR_Y=%p\n",&UT3D_VECTOR_Y);
  fflush(stdout);
  return 0;
}


//===========================================================================
  int DXFW__ (char* fnam)  {
//===========================================================================
// DXFW__                     export as DXF
//   if group is active: export group only; else export all objs.
// Input:

  int       i1, i2, *ip, mTyp, oNr, mode;
  long      l1;
  char      *p1, s1[256], *fBuf;
  ObjDB     *oTab;
  FILE      *fp1, *fp2=NULL;
  ModelBas  *mbo;


  printf("DXFW__ vers=%d |%s|\n",AP_stat.subtyp,fnam);


// - Liste auszugebender objekte (typ+dbi)
//   - wenn grp aktiv: grp
//   - else all obj's
// - export all objs of list



  //----------------------------------------------------------------
  // init
  dxfw_errNr = 0;
  dxfw_objNr = 0;

  // get dxf-version
  dxfw_subtyp = AP_stat.subtyp;

  // store active DB
  DB_save__ ("");


  // init stringList subModelnames
#define SIZ_SMTAB 1000
  MemTab_ini__ (&dxfw_smTab, sizeof(int), Typ_Int4, SIZ_SMTAB);




  //----------------------------------------------------------------
  // open file ENTITIES
  sprintf(s1,"%sdxfw_main",OS_get_tmp_dir());
  if ((fpo1 = fopen (s1, "w+")) == NULL) {
    TX_Error ("open file %s",s1);
    return -1;
  }


  // export mainModel
  fprintf(fpo1,"0\nSECTION\n");
  fprintf(fpo1,"2\nENTITIES\n");




  //----------------------------------------------------------------
  // get nr of objs of group
  oNr = Grp_get__ (&oTab);
  if(oNr > 0) {
    mode = 0;
    goto L_exp;
  }
  

  L_all:  // group is empty; add all objs to group
    mode = 2;
    Grp_add_all1 (-1);


  L_exp:
      // Grp_dump ();


  // export main - all obj's in group. Add subModel -> dxfw_smTab
  // DXFW_w_grp (fnam);
  i1 = DXFW_main ();


  // clear group
  if(mode == 2) Grp_init ();


  fprintf(fpo1,"0\nENDSEC\n");
  fprintf(fpo1,"0\nEOF\n");
  fclose (fpo1);


  //----------------------------------------------------------------
  // export subModels as BLOCKS; loop tru dxfw_smTab
  sprintf(s1,"%sdxfw_blocks",OS_get_tmp_dir());
  if ((fpo1 = fopen (s1, "w+")) == NULL) {
    TX_Error ("open file %s",s1);
    return -1;
  }

  fprintf(fpo1,"0\nSECTION\n");
  fprintf(fpo1,"2\nBLOCKS\n");


  // add defaultblocks eg *D0
  DXFW_blk_ini ();


  i2 = MEMTAB_IND (&dxfw_smTab);  // get nr of used subModels

  for(i1 = 0; i1 < i2; ++i1) {
    ip = MEMTAB__ (&dxfw_smTab, i1);
    mbo = DB_get_ModBas (*ip);
    mTyp = mbo->typ;   // get modeltype (native or mockup)
    strcpy(s1, mbo->mnam);
    // for mockup-models: remove filetyp from modelname
    if(mTyp >= Mtyp_DXF) UTX_ftyp_cut (s1);
    // change all '/' of mNam into '_' - else no correct filename possible
    UTX_safeName (s1, 2);
    p1 = s1;
      printf("\n++++++++++++++++++++++++++++\n nxt blk: %d %d |%s|\n",
             *ip,mTyp,p1);


    fprintf(fpo1,"0\nBLOCK\n");
    fprintf(fpo1,"2\n%s\n",p1);


    // check subModel-type; native or mockup
    // i3 = Mod_get_ftyp1 (p1);
      // printf(" ftyp = %d\n",i3);



    if(mTyp < Mtyp_DXF) {
      // native subModel (Mtyp_Gcad)
      // load DB
      DB_load__ (p1);
      // export subModel
      DXFW_Mdl_gcad (0);

    } else {
      // Mockup-model
      DXFW_Mdl_tess (p1);
    }


    fprintf(fpo1,"0\nENDBLK\n");

  }

  fprintf(fpo1,"0\nENDSEC\n");
  fclose (fpo1);

  MemTab_free (&dxfw_smTab);

  DB_load__ ("");   // reload main-DB


  //----------------------------------------------------------------
  // join files
  if ((fpo1 = fopen (fnam, "w+")) == NULL) {
  // if ((fpo1 = fopen (fnam, "wb")) == NULL) {     // crash in MS !
    TX_Error ("open file %s",fnam);
    return -1;
  }

  fprintf(fpo1,"999\n%s\n",DXFW_VERSION);


  // write SECTION HEADER
  // write SECTION TABLES
  if(dxfw_subtyp < 90) DXFW_prolog ();

  // add BLOCKS
  sprintf(s1,"%sdxfw_blocks",OS_get_tmp_dir());
    printf(" cat_file |%s|\n",s1);
  DXFW_cat_file (fpo1, s1);


  // add ENTITIES
  sprintf(s1,"%sdxfw_main",OS_get_tmp_dir());
    printf(" cat_file |%s|\n",s1);
  DXFW_cat_file (fpo1, s1);
  
  fclose (fpo1);


  // sprintf(s1,"%sdxfw_*",OS_get_tmp_dir());
  // OS_file_delGrp (s1);



  //----------------------------------------------------------------
  if(AP_errStat_get() == 0) {
    TX_Print("%s exported ",fnam);
  }




  return 0;


}


//================================================================
  int DXFW_prolog () {
//================================================================
//   DXF-Prolog ausgeben.
//   dxfw_subtyp



/*
  int i1;
  char *dxfi1[] = {
    "0","SECTION","2","HEADER","9","$ACADVER","1","AC1009","0","ENDSEC",
    "0","SECTION","2","TABLES","0","ENDSEC",
    "0","SECTION","2","BLOCKS","0","ENDSEC",
    "0","SECTION","2","ENTITIES",0
  };

  for (i1=0; dxfi1[i1] != '\0'; i1++)
    fprintf(fpo1,"%s\n",dxfi1[i1]);
*/


  printf("DXFW_prolog %d\n",dxfw_subtyp);


  //------------------------------------
  fprintf(fpo1,"0\nSECTION\n");
  fprintf(fpo1,"2\nHEADER\n9\n$ACADVER\n");

  if(dxfw_subtyp == 0) {             // 0=R10 
    fprintf(fpo1,"1\nAC1006\n");

  } else if(dxfw_subtyp == 1) {      // 1=R12
    fprintf(fpo1,"1\nAC1009\n");

  } else if(dxfw_subtyp == 2) {      // 2=R14
    fprintf(fpo1,"1\nAC1014\n");

  } else {                            // 3=AC2000
    fprintf(fpo1,"1\nAC1015\n");
  }

  fprintf(fpo1,"9\n$TEXTSIZE\n40\n%f\n",AP_txsiz);
  fprintf(fpo1,"9\n$DIMTXT\n40\n%f\n",AP_txdimsiz);
  fprintf(fpo1,"9\n$DIMASZ\n40\n%f\n",AP_txdimsiz);

  // anzahl nachkommastellen f dimWerte
  fprintf(fpo1,"9\n$DIMDEC\n70\n2\n");
  fprintf(fpo1,"9\n$DIMADEC\n70\n2\n");  // fuer DIM-Angles

  fprintf(fpo1,"0\nENDSEC\n");


  //------------------------------------
  fprintf(fpo1,"0\nSECTION\n");
    fprintf(fpo1,"2\nTABLES\n");

    // Defaultlayer 0; fuer POINT LINE ARC ... via 8/0
    fprintf(fpo1,"0\nTABLE\n2\nLAYER\n70\n1\n");

    // 2=Layername; 70=typ, 620col, 6=Linetyp
    fprintf(fpo1,"0\nLAYER\n2\n0\n70\n0\n62\n7\n6\nCONTINUOUS\n");

    fprintf(fpo1,"0\nENDTAB\n");

  fprintf(fpo1,"0\nENDSEC\n");
  //------------------------------------
  // SECTION ENTITIES

  return 1;

}


//================================================================
  int DXFW_blk_ini () {
//================================================================

    // defaultblock *D0  f DIM's via 2/*D0
    fprintf(fpo1,"0\nBLOCK\n2\n*D0\n70\n1\n62\n7\n");
    fprintf(fpo1,"10\n0\n20\n0\n30\n0\n");
    fprintf(fpo1,"0\nENDBLK\n");

  return 0;

}
 

/*
//================================================================
  int DXFW_w_BLOCK (char *blkNam) {
//================================================================
// export subModel <blkNam>


  printf("DXFW_w_BLOCK |%s|\n",blkNam);

  DXFW_Mdl_gcad (0);

  return 0;

}
*/
 
//================================================================
  int dxfw_ELLIPSE (CurvElli *cv1, FILE *fp_in) {
//================================================================
// see ../formate/dxf/doc/DXF2000_ellipse.txt

  double     d1;


  // printf("dxfw_ELLIPSE \n");
  // DEB_dump_obj__ (Typ_CVELL, cv1, "cv1:");


  fprintf(fp_in,"0\nELLIPSE\n");

  // 10,20,30 = centerpoint
  DXFW_point3 (0, &cv1->pc, fp_in);

  // 11,21,31 = Endpoint of major axis, relative to the center.
  DXFW_fl_out (11, cv1->va.dx, fp_in);
  DXFW_fl_out (21, cv1->va.dy, fp_in);
  DXFW_fl_out (31, cv1->va.dz, fp_in);

  // 210,220,230 = Extrusion direction
  DXFW_vector (&cv1->vz, fp_in);

  // 40 = Ratio of minor axis to major axis
  d1 = UT3D_len_vc(&cv1->vb) / UT3D_len_vc(&cv1->va);
  DXFW_fl_out (40, d1, fp_in);

  // 41 = Start parameter (this value is 0.0 for a full ellipse)
  // 42 = End parameter (this value is 2pi for a full ellipse)
  if(UT3D_comp2pt(&cv1->p1, &cv1->p2, UT_TOL_pt) != 0) {
    DXFW_fl_out (41, 0., fp_in);
    DXFW_fl_out (42, RAD_360, fp_in);

  } else {
    // parametric-Angle <- pt
    d1 = UT3D_angr_elpt (&cv1->p1, &cv1->pc, &cv1->va, &cv1->vb);
    DXFW_fl_out (41, d1, fp_in);
    d1 = UT3D_angr_elpt (&cv1->p2, &cv1->pc, &cv1->va, &cv1->vb);
    DXFW_fl_out (42, d1, fp_in);

  }

  return 0;

}


//================================================================
  int dxfw_SPLINE (CurvBSpl *cv1, FILE *fp_in) {
//================================================================
// see ../formate/dxf/doc/DXF2000_spline.txt

  int      i1, i2;

  // 5 8 62 370


  printf("dxfw_SPLINE \n");
  // DEB_dump_obj__ (Typ_CVBSP, cv1, "cv1:");

  fprintf(fp_in,"0\nSPLINE\n");


  // 210,220,230 Normal vector (omitted if the spline is nonplanar)



  // 70 = Spline flag (bit coded):
  //   1 = Closed spline
  //   2 = Periodic spline
  //   4 = Rational spline
  //   8 = Planar
  //   16 = Linear (planar bit is also set)


  // 71 = deg
  fprintf(fp_in,"71\n%d\n",cv1->deg);

  // 72 = Number of knots
  i2 = cv1->ptNr + cv1->deg + 1;
  fprintf(fp_in,"72\n%d\n",i2);

  // 73 = Number of control points
  fprintf(fp_in,"73\n%d\n",cv1->ptNr);

  // 40 = Knot value
  for(i1=0; i1<i2; ++i1) DXFW_fl_out (40, cv1->kvTab[i1], fp_in);

  // export points  10,20,30
  i2 = cv1->ptNr;
  for(i1=0; i1<i2; ++i1) DXFW_point3 (0, &cv1->cpTab[i1], fp_in);

  return 0;

}


//================================================================
  int DXFW_VERTEX3 (int typ, Point *pti, FILE *fp_in) {
//================================================================
// typ: 8=Spline, 32=Polyline


  fprintf(fp_in,"0\nVERTEX\n");
  fprintf(fp_in,"8\n0\n");            // Layer
  DXFW_point3 (0, pti, fp_in);
  fprintf(fp_in,"70\n%d\n",typ);         // TypVertex;

  return 0;

}


//================================================================
  int DXFW_VERTEX2 (int typ, Point2 *pti, FILE *fp_in) {
//================================================================
// typ: 8=Spline, 32=Polyline
/*
 0 VERTEX
   70 bit(8)   Spline vertex
      bit(16)  Spline frame control point
      bit(32)  3D-Polyline vertex
      bit(64)  3D polygonMesh vertex
      bit(128) PolyfaceMesh vertex
*/


  fprintf(fp_in,"0\nVERTEX\n");
  fprintf(fp_in,"8\n0\n");               // Layer
  DXFW_point2 (0, pti, fp_in);
  fprintf(fp_in,"70\n%d\n",typ);         // TypVertex;

  return 0;

}

//================================================================
  int dxfw_hd_POLYLINE (FILE *fp_in) {
//================================================================

        fprintf(fp_in,"0\nPOLYLINE\n");
        fprintf(fp_in,"8\n0\n");    // def.Layer
        fprintf(fp_in,"66\n1\n");   // indicates that VERT's will follow
        DXFW_point3 (0, (Point*)&UT3D_PT_NUL, fp_in);  // Nullpunkt
        fprintf(fp_in,"70\n8\n");   // bit(8)   3D-Polygon

  return 0;

}


/*
//================================================================
  int dxfw_POLYLINE (int pNr, int typ, Point *pTab, FILE *fp_in) {
//================================================================
// typ: 8=Spline, 32=Polyline

  int   i1;


  dxfw_hd_POLYLINE (fp_in); // wr header of POLYLINE


  for(i1=0;i1<pNr; ++i1) {
    // printf(" dxfw-p[%d]=%f %f %f\n",i1,pTab[i1].x,pTab[i1].y,pTab[i1].z);
    DXFW_VERTEX3 (typ, &pTab[i1], fp_in);
  }

  fprintf(fp_in,"0\nSEQEND\n");  // end of VERT's after 66/1

  return 0;

}
*/

//=====================================================================
  int DXFW_CI (Circ *ci1, FILE *fp_in) {
//=====================================================================
// ARC:
// centerpt 10/20/30
// radius   40
// startAng 50
// endAng   51


// CIRC:
// centerpt 10/20/30
// ?        34
// radius   40


  int      zparl;
  double   d1, d2, d3;
  Point2   pt2c, pt21;
  Point    ptc, pt1, pt2;
  Mat_4x3  m1, im1;


      // Feststellen, ob es ein Vollkreis ist
      if(!UT3D_comp2pt(&ci1->p1, &ci1->p2, UT_TOL_pt)) goto L_AC;



      //========== es ist ein Vollkreis =======================
      fprintf(fp_in,"0\nCIRCLE\n");
      fprintf(fp_in,"8\n0\n"); // def.Layer
      //TX_Print("    pc=%f,%f,%f",ci1->pc.x,ci1->pc.y,ci1->pc.z);
      //TX_Print("    vz=%f,%f,%f",ci1->vz.dx,ci1->vz.dy,ci1->vz.dz);


      // Feststellen ob die Achse in +Z oder in -Z geht
      // ist vz parallel zum Z-Vektor -
      //if(!((UTP_comp2db(ci1->vz.dx,0.,UT_TOL_pt))   &&
      //     (UTP_comp2db(ci1->vz.dy,0.,UT_TOL_pt))))   {
      if((ci1->vz.dz > 1.-UT_TOL_min1)||(ci1->vz.dz < -1.+UT_TOL_min1)) {
        zparl = ON;
        // Ausgeben eines X-Y-parallelen Vollkreises.
        pt1=ci1->pc;
        goto L_CI_out;
      }


      // schiefer Vollkreis
      zparl = OFF;
      UT3D_vc_normalize (&ci1->vz, &ci1->vz);
      dxfw_load_mat (m1, &ci1->vz);  // make TrMat nach DXF-Konvention
      UT3D_m3_invm3 (im1, m1);       // RuecktransformationsMat. generieren
      UT3D_pt_tra_pt_m3 (&pt1, im1, &ci1->pc);
      //TX_Print(" pt1=%f,%f,%f",pt1.x,pt1.y,pt1.z);


      // Ausgabe Vollkreis
      L_CI_out:
      DXFW_point3 (0, &pt1, fp_in);                  // Mittelpunkt als 10,20,30
      fprintf(fp_in,"40\n%f\n",fabs(ci1->rad));      // Radius 
      if(zparl == OFF) DXFW_vector (&ci1->vz, fp_in);  // 210,220,230
      return 0;





      //========== es ist ein Kreisbogen =======================
      L_AC:

      fprintf(fp_in,"0\nARC\n");
      fprintf(fp_in,"8\n0\n"); // def.Layer


      // Arc umdrehen, wenn er genau in -Z zeigt.
      // wenn vz = 0,0,-1 dann vz und p1/p2 umdrehen.
      //if((UTP_comp2db(ci1->vz.dx, 0.0, UT_TOL_pt))  &&
      //   (UTP_comp2db(ci1->vz.dy, 0.0, UT_TOL_pt))  &&
      //   (ci1->vz.dz < 0.0))                 {
      if(ci1->vz.dz < -1.+UT_TOL_min1) {
        //TX_Print("umdrehen");
        //pt1=ci1->p1; ci1->p1=ci1->p2; ci1->p2=pt1;
        ci1->rad = -ci1->rad;
        UT3D_vc_invert(&ci1->vz, &ci1->vz);

      }


      // Feststellen ob die Achse in +Z geht
      // ist vz parallel zum Z-Vektor -
      //if(!((UTP_comp2db(ci1->vz.dx,0.,UT_TOL_pt))   &&
      //     (UTP_comp2db(ci1->vz.dy,0.,UT_TOL_pt))))   goto L_AC_schief;
      if(ci1->vz.dz > 1.-UT_TOL_min1) {
        zparl = ON;
        ptc=ci1->pc;
        pt1=ci1->p1;
        pt2=ci1->p2;
        goto L_AC_out;
      }




      L_AC_schief:
      zparl = OFF;
      UT3D_vc_normalize (&ci1->vz, &ci1->vz);
      dxfw_load_mat (m1, &ci1->vz);  // make TrMat nach DXF-Konvention
      UT3D_m3_invm3 (im1, m1);       // RuecktransformationsMat. generieren
      UT3D_pt_tra_pt_m3 (&ptc, im1, &ci1->pc);
      UT3D_pt_tra_pt_m3 (&pt1, im1, &ci1->p1);
      UT3D_pt_tra_pt_m3 (&pt2, im1, &ci1->p2);
      //TX_Print(" pt1=%f,%f,%f",pt1.x,pt1.y,pt1.z);



      L_AC_out:
      DXFW_point3 (0, &ptc, fp_in);  // Mittelpunkt

      // Radius
      fprintf(fp_in,"40\n%f\n",fabs(ci1->rad));


      // AnfWin
      // AnfWin
      pt2c = UT2D_pt_pt3 (&ptc);
      pt21 = UT2D_pt_pt3 (&pt1);
      d1 = UT_DEGREES(UT2D_angr_ptpt(&pt2c, &pt21));

      // EndWin
      pt21 = UT2D_pt_pt3 (&pt2);
      d2 = UT_DEGREES(UT2D_angr_ptpt(&pt2c, &pt21));

      // CW -> CCW
      if(ci1->rad < 0.) {
        d3=d1; d1=d2; d2=d3;
      }

      fprintf(fp_in,"50\n%f\n",d1);
      fprintf(fp_in,"51\n%f\n",d2);


      if(zparl==OFF) DXFW_vector (&ci1->vz, fp_in);

  return 0;

}


//=====================================================================
  int DXFW_fl_out (int recID, double fVal, FILE *fp_in) {
//=====================================================================
// einen Record mit double ausgeben

  char   cbuf[32];


  cbuf[0] = '\0';
  UTX_add_fl_u(cbuf, fVal);

  fprintf(fp_in,"%d\n%s\n",recID, cbuf);

  return 0;

}


//=====================================================================
  int DXFW_point2 (int pnum, Point2 *point, FILE *fp_in) {
//=====================================================================
/*
 2D-Punktkoord. ausgeben.
 pnum=0: erster Punkt  (10, 20, 30);
 pnum=1: zweiter Punkt (11, 21, 31)
usw.
*/


/*
  fprintf(fp_in,"%d\n%f\n%d\n%f\n",
    10+pnum, point->x,
    20+pnum, point->y);
*/

  DXFW_fl_out (10+pnum, point->x, fp_in);
  DXFW_fl_out (20+pnum, point->y, fp_in);


  return 1;
}


//=====================================================================
  int DXFW_point3 (int pnum, Point *point, FILE *fp_in) {
//=====================================================================
/*
 3D-Punktkoord. ausgeben.   
 pnum=0: erster Punkt  (10, 20, 30);
 pnum=1: zweiter Punkt (11, 21, 31)
usw.
*/

  DXFW_fl_out (10+pnum, point->x, fp_in);
  DXFW_fl_out (20+pnum, point->y, fp_in);
  DXFW_fl_out (30+pnum, point->z, fp_in);

  return 1;
}


//=====================================================================
  int DXFW_vector (Vector* vc1, FILE *fp_in) {
//=====================================================================


  fprintf(fp_in,"%d\n%f\n%d\n%f\n%d\n%f\n",
    210, vc1->dx,
    220, vc1->dy,
    230, vc1->dz);

  return 1;
}


//===========================================================================
  int dxfw_load_mat (Mat_4x3 m1, Vector* vz) {
//===========================================================================
/* die Mat fuer ein  DXF-ECS bestimmen. Input nur der Vektor (eines Kreises).

 Der Nullpunkt (Origin) ist immer ident mit dem Hauptnullpunkt !

1) Der Vektor sei die neue ECS-Z-Achse.
2) Die neue X-Achse ist das Kreuzprodukt der World-Z-Achse mit der neuen Z-Achse
3) Die neue Y-Achse ist das Kreuzprodukt der neuen Z-Achse mit der neuen X-Achse


*/

  Vector    vx, vy;
  Plane     pl1;


  //TX_Print("dxfw_load_mat vz=%f,%f,%f",vz->dx,vz->dy,vz->dz);


  // Die neue X-Achse ist das Kreuzprodukt der World-Z-Achse mit der neuen Z-Achse
  //UT3D_vc_perp2vc (&vx, vz, &UT3D_VECTOR_Z);
  UT3D_vc_perp2vc (&vx, (Vector*)&UT3D_VECTOR_Z, vz);
  //TX_Print("vx=%f,%f,%f",vx.dx,vx.dy,vx.dz);

  // Die neue Y-Achse ist das Kreuzprodukt der neuen Z-Achse mit der neuen X-Achse
  UT3D_vc_perp2vc (&vy, vz, &vx);
  //TX_Print("vy=%f,%f,%f",vy.dx,vy.dy,vy.dz);


  Load_Matrix:
  pl1.po = UT3D_PT_NUL;
  pl1.vx = vx;
  pl1.vy = vy;
  pl1.vz = *vz;

  UT3D_m3_loadpl (m1, &pl1);
/*
  TX_Print(" matVX=%f,%f,%f",m1[0][0],m1[1][0],m1[2][0]);
  TX_Print(" matVY=%f,%f,%f",m1[0][1],m1[1][1],m1[2][1]);
  TX_Print(" matVZ=%f,%f,%f",m1[0][2],m1[1][2],m1[2][2]);
  TX_Print(" matPO=%f,%f,%f",m1[0][3],m1[1][3],m1[2][3]);
*/

  return 0;

}

//================================================================
  int DXFW_TEXT (GText *tx1, FILE *fpo) {
//================================================================
 

  int            i1;
  double         d1;


  // DEB_dump_obj__ (Typ_GTXT, tx1, "DXFW_TEXT");


  fprintf(fpo,"0\nTEXT\n");
  fprintf(fpo,"8\n0\n"); // def.Layer
  
  // posi
  DXFW_point3 (0, &tx1->pt, fpo);

  // 7=Textstyle "normal#0.300000#0.900000" od cursive#0.500000#3.000000

  // 40-Textsize (characterhoehe)
  // Notes haben size -1!
  d1 = tx1->size;
  if(d1 < 0) d1 = 1.;;
  // d1 *= DXF_fakt_txtSiz;
  DXFW_fl_out (40, d1, fpo);

  // 50-Textdirection
  DXFW_fl_out (50, tx1->dir, fpo);

  // 1-Text
  dxfw_gxt (0, memspc011, tx1->txt);
  fprintf(fpo,"1\n%s\n",memspc011);

  // 39 = Thickness; 1 oder 2
  i1 = 1;
  if(tx1->size > 1.) i1 = 2;
  fprintf(fpo,"39\n%d\n",i1);

  // 62 = colornum = 0
  fprintf(fpo,"62\n0\n");


  return 0;

}


//================================================================
  int DXFW_DIM  (Dimen *dim1, FILE *fp_in) {
//================================================================
// Only librecad resolves old-style dimensions.
// For all others: create a dummy-block "*D#" for each dimension;
// Create all lines, arc, text (arrows as lines) in the dummy-block.

// tp    point text-position (midpoint of text)
// el1s  startpoint 1. extension-line
// dls   startpoint dimension-line
//

//  70-2  Angular
//  70-3  Diameter
//  70-4  Radius
//  70-5  Angular 3-point


  double        a1, d1, d2;
  Point         pt1, pt2;
  Point2        pt21, pt22, pt2c;
  Vector2       vc21, vc22;


  // DEB_dump_obj__ (Typ_Dimen, dim1, "=========== DXFW_DIM");



  //----------------------------------------------------------------
  if(dim1->dtyp != 21) {     // 21=Leader

    fprintf(fp_in,"0\nDIMENSION\n");
    fprintf(fp_in,"8\n0\n");                 // def.Layer
    fprintf(fp_in,"2\n*D0\n");               // def.Block *D0

    // 1-Text
    dxfw_gxt (1, memspc011, dim1->txt);
    fprintf(fp_in,"1\n%s\n",memspc011);

  } else {
    // fprintf(fp_in,"0\nLEADER\n");
    dxfw_hd_POLYLINE (fp_in); // wr header of POLYLINE
  }



  //----------------------------------------------------------------
  if(dim1->dtyp == 0) {     // 0=Linearmasz; a1 => ..
    //  70-0  subVersion horizontal:
    //         1                     
    //    |---------|0                3=el1s gc:p2
    //    |         |                 4=el2s gc:p1
    //    |3        |                 0=dls  -
    //              |4                1=tp   gc:p3
    //
    //  70-0  subVersion vertical:
    
    //  70-0  subVersion rotated:


    // 70=0+128 128 means: textPosition userDefined, not default position.
    fprintf(fp_in,"70\n%d\n",128);     // 70 = typ Dim; = 128+0=hor

    // p2 => 3 = el1s
    pt1 = UT3D_pt_pt2 (&dim1->p2);
    DXFW_point3 (3, &pt1, fp_in);
    DXFW_fl_out (50, dim1->a1, fp_in);  // 50 = angle

    // p1 => 4 = el2s
    pt1 = UT3D_pt_pt2 (&dim1->p1);
    DXFW_point3 (4, &pt1, fp_in);

    // 0: startPt of dimension-line
    a1 = UT_RADIANS(dim1->a1);
    UT2D_vc_angr (&vc21, a1);   // vc21 = along dimension-line
    UT2D_vc_rot_90_ccw (&vc22, &vc21);   // vc22 = extension-line
    UT2D_vc_invert (&vc22, &vc22); 
    // prj defPt1 -> extLine1
    UT2D_pt_projptptvc (&pt21, &dim1->p3, &dim1->p1, &vc22);
      // GR_Disp_pt2 (&pt21, SYM_STAR_S, 2);
    pt1 = UT3D_pt_pt2 (&pt21);
    DXFW_point3 (0, &pt1, fp_in);

    // 1: Punkt p3 = tp
    // dxf: centerpoint of textblock; gcad: centerpoint of block on dim-line
// TODO: move above dim-line (add text-height / 2)
    pt21 = dim1->p3;   // textpoint
    UT2D_pt_traptvclen (&pt22, &pt21, &vc22, -(AP_txdimsiz / 2.));
    DXFW_point2 (1, (Point2*)&pt2, fp_in);

    // 50 = Angle of dimension-line (deg)
    a1 = dim1->a1;
      // printf(" _DIM a1=%lf\n",a1);
    DXFW_fl_out (50, a1, fp_in);
    // optional 51 ndicates the horizontal direction; val=neg angle of x-axis
    // // 53=rotation angle of the dimension text
    // DXFW_fl_out (53, dim1->a1, fp_in);  // 53 = angle (f CatV5 u Rhino)



  //----------------------------------------------------------------
  } else if(dim1->dtyp == 1) {     // 1=Durchmesser
    fprintf(fp_in," 70\n%d\n",131); // 70 = typ Dim; = 128+3=Dmr
    DXFW_fl_out (53, dim1->a1, fp_in);  // 53 = angle (f CatV5 u Rhino)
      // 0 = opposit pt am circ
      UT2D_pt_opp2pt (&pt21, &dim1->p1, &dim1->p2);
      pt1 = UT3D_pt_pt2 (&pt21);
      DXFW_point2 (0, (Point2*)&pt1, fp_in);
      // 1 = Textpt
      pt1 = UT3D_pt_pt2 (&dim1->p3);
      DXFW_point2 (1, (Point2*)&pt1, fp_in);
      // 5 = pt am Circ
      pt1 = UT3D_pt_pt2 (&dim1->p2);
      DXFW_point2 (5, (Point2*)&pt1, fp_in);



  //----------------------------------------------------------------
  } else if(dim1->dtyp == 2) {     // 2=Radius
    fprintf(fp_in," 70\n%d\n",132); // 70 = typ Dim; = 128+4=Rad.
    DXFW_fl_out (53, dim1->a1, fp_in);  // 53 = angle (f CatV5 u Rhino)
      // 0 = CirCen
      pt1 = UT3D_pt_pt2 (&dim1->p1);
      DXFW_point2 (0, (Point2*)&pt1, fp_in);
      // 1 = Textpt
      pt1 = UT3D_pt_pt2 (&dim1->p3);
      DXFW_point2 (1, (Point2*)&pt1, fp_in);
      // 5 = pt am Circ
      pt1 = UT3D_pt_pt2 (&dim1->p2);
      DXFW_point2 (5, (Point2*)&pt1, fp_in);



  //----------------------------------------------------------------
  } else if(dim1->dtyp == 3) {     // 3=Angle
/*
    fprintf(fp_in," 70\n%d\n",133); // 70 = typ Dim; = 128+5=Ang.
    UT2D_vc_angr (&vc21, dim1->a1);
    UT2D_vc_angr (&vc22, dim1->a2);
    // inters. Pt
    UT2D_pt_int2pt2vc (&pt21,&dim1->p1,&vc21,&dim1->p2,&vc22);
    d1 = UT2D_len_2pt (&pt21,&dim1->p3); // Radius
    // 3=p1=ExtL1
    pt1 = UT3D_pt_pt2 (&dim1->p1);
    DXFW_point2 (3, &pt1, fp_in);
    // 4=p2=ExtL2
    pt1 = UT3D_pt_pt2 (&dim1->p2);
    DXFW_point2 (4, &pt1, fp_in);
    // 5=IntersectPt ExtL1-ExtL2
    pt1 = UT3D_pt_pt2 (&pt21);
    DXFW_point2 (5, &pt1, fp_in);
    // 0=Circ Intersect auf ExtL2
    UT2D_pt_traptvclen (&pt22,&pt21,&vc22,d1);
      GR_Disp_pt2 (&pt22, SYM_STAR_S, 3);
    pt1 = UT3D_pt_pt2 (&pt22);
    DXFW_point2 (0, &pt1, fp_in);
    // 1=TextPt
    pt1 = UT3D_pt_pt2 (&dim1->p3);
    DXFW_point2 (1, &pt1, fp_in);
*/
    fprintf(fp_in," 70\n%d\n",130); // 70 = typ Dim; = 128+2=Ang.
    UT2D_vc_angr (&vc21, dim1->a1);
    UT2D_vc_angr (&vc22, dim1->a2);
    // pt2c=Intersect ExtL1-ExtL2
    UT2D_pt_int2pt2vc (&pt2c,&dim1->p1,&vc21,&dim1->p2,&vc22); // center
    // d1 = Radius
    d1 = UT2D_len_2pt (&pt2c,&dim1->p3);
    // 53=Winkel Text (Winkel pt2c - p3)
    d2 = UT2D_angr_ptpt (&pt2c,&dim1->p3);
    d2 = UT2D_angr_perpangr (&d2);  // + 90 Grad
    fprintf(fp_in,"53\n%f\n",UT_DEGREES(d2));
    // ExtL1 = 3,4   (4=P1)
    UT2D_pt_traptvclen (&pt22,&dim1->p1,&vc21,-1.);
      // GR_Disp_pt2 (&pt22, SYM_STAR_S, 2);
    pt1 = UT3D_pt_pt2 (&pt22);
    DXFW_point2 (3, (Point2*)&pt1, fp_in);
    pt1 = UT3D_pt_pt2 (&dim1->p1);
    DXFW_point2 (4, (Point2*)&pt1, fp_in);
    // ExtL2 = 5,0   (0=P2)
    UT2D_pt_traptvclen (&pt22,&dim1->p2,&vc22,-1.);
      // GR_Disp_pt2 (&pt22, SYM_STAR_S, 3);
    pt1 = UT3D_pt_pt2 (&pt22);
    DXFW_point2 (5, (Point2*)&pt1, fp_in);
    pt1 = UT3D_pt_pt2 (&dim1->p2);
    DXFW_point2 (0, (Point2*)&pt1, fp_in);
    // 6 = Circ Intersect auf ExtL2
    UT2D_pt_traptvclen (&pt22,&pt2c,&vc22,d1);
      // GR_Disp_pt2 (&pt22, SYM_STAR_S, 3);
    pt1 = UT3D_pt_pt2 (&pt22);
    DXFW_point2 (6, (Point2*)&pt1, fp_in);
    // 1 = p3
    pt1 = UT3D_pt_pt2 (&dim1->p3);
    DXFW_point2 (1, (Point2*)&pt1, fp_in);



  //----------------------------------------------------------------
  } else if(dim1->dtyp == 21) {     // 21=Leader
/* Ordinate wird scheinbar nicht unterstuertzt!
    fprintf(fp_in," 70\n%d\n",134); // 70 = typ Dim; = 128+6=Ordinate
    pt1 = UT3D_pt_pt2 (&dim1->p1);
    DXFW_point2 (3, &pt1, fp_in);
    pt1 = UT3D_pt_pt2 (&dim1->p2);
    DXFW_point2 (4, &pt1, fp_in);
    // pt1 = UT3D_pt_pt2 (&dim1->p2);
    // DXFW_point2 (4, &pt1, fp_in);
    // DXFW_point2 (0, &UT2D_PT_NUL, fp_in);
*/
/*
    // LEADER + TEXT
    if(dim1->p3.x != UT_DB_LEER)
         fprintf(fp_in," 76\n3\n");  // Anzahl Punkte
    else fprintf(fp_in," 76\n2\n");
    pt1 = UT3D_pt_pt2 (&dim1->p1);
    DXFW_point2 (0, &pt1, fp_in);
    pt1 = UT3D_pt_pt2 (&dim1->p2);
    DXFW_point2 (0, &pt1, fp_in);
    if(dim1->p3.x != UT_DB_LEER) {
      pt1 = UT3D_pt_pt2 (&dim1->p3);
      DXFW_point2 (0, &pt1, fp_in);
    }
*/
    // VERTEXE zu POLYLINE
    DXFW_VERTEX2 (32, &dim1->p1, fp_in);
    DXFW_VERTEX2 (32, &dim1->p2, fp_in);
    if(dim1->p3.x != UT_DB_LEER) {
      pt1 = UT3D_pt_pt2 (&dim1->p3);
      DXFW_VERTEX2 (32, &dim1->p3, fp_in);
    } else {
      pt1 = UT3D_pt_pt2 (&dim1->p2);
    }
    fprintf(fp_in,"0\nSEQEND\n");

    // Text hier extra raus
    fprintf(fp_in,"0\nTEXT\n");
    fprintf(fp_in,"8\n0\n"); // def.Layer
    DXFW_fl_out (50, dim1->a1, fp_in); // 50-Textdirection
    DXFW_fl_out (40, AP_txdimsiz, fp_in); // 40-TextSize
    DXFW_point2 (0, (Point2*)&pt1, fp_in);
    dxfw_gxt (0, memspc011, dim1->txt);
    fprintf(fp_in,"1\n%s\n",memspc011);

  }
  // break;

  return 0;

}

 
//================================================================
  int DXFW_POLYLN2 (ObjGX *ox1, int typ, FILE *fp_in) {
//================================================================
// export curve as 3D-polygon
// TODO: export CCV as analytic obj's with group ?
// Input:
//   typ    8=Spline, 32=Polyline




  int       irc, i1, pNr;
  Point     *pTab;

  pTab = (Point*)memspc201;
  pNr = sizeof(memspc201) / sizeof(Point);


  irc = UT3D_npt_ox__ (&pNr, pTab, ox1, UT_DISP_cv);
  if(irc < 0) return irc;

  fprintf(fp_in,"0\nPOLYLINE\n");
  fprintf(fp_in,"8\n0\n");    // def.Layer
  fprintf(fp_in,"66\n1\n");   // indicates that VERT's will follow
  DXFW_point2 (0, (Point2*)&UT2D_PT_NUL, fp_in);  // Nullpunkt
  fprintf(fp_in,"70\n8\n");   // bit(8)   3D-Polygon

  for(i1=0;i1<pNr; ++i1) {
    // printf(" dxfw-p[%d]=%f %f %f\n",i1,pTab[i1].x,pTab[i1].y,pTab[i1].z);
    DXFW_VERTEX2 (typ, (Point2*)&pTab[i1], fp_in);
  }

  fprintf(fp_in,"0\nSEQEND\n");  // end of VERT's after 66/1

  return 0;

}


//================================================================
  int DXFW_POLYLN3 (ObjGX *ox1, int typ, FILE *fp_in) {
//================================================================
// export curve as 3D-polygon
// TODO: export CCV as analytic obj's with group ?
// Input:
//   typ    8=Spline, 32=Polyline

// wr header of POLYLINE
//  0 POLYLINE
//    70 bit(1)   closed polygon or polygonMesh - closed in M
//       bit(8)   3D-Polygon
//       bit(16)  PolygonMesh; details in groupCode 75
//       bit(32)  PolygonMesh - closed in N
//       bit(64)  PolyfaceMesh (PFACE); details via 71,72.



  int       irc, i1, pNr;
  Point     *pTab;

  pTab = (Point*)memspc201;
  pNr = sizeof(memspc201) / sizeof(Point);


  irc = UT3D_npt_ox__ (&pNr, pTab, ox1, UT_DISP_cv);
  if(irc < 0) return irc;

  fprintf(fp_in,"0\nPOLYLINE\n");
  fprintf(fp_in,"8\n0\n");    // def.Layer
  fprintf(fp_in,"66\n1\n");   // indicates that VERT's will follow
  DXFW_point3 (0, (Point*)&UT3D_PT_NUL, fp_in);  // Nullpunkt
  fprintf(fp_in,"70\n8\n");   // bit(8)   3D-Polygon
    
  for(i1=0;i1<pNr; ++i1) {
    // printf(" dxfw-p[%d]=%f %f %f\n",i1,pTab[i1].x,pTab[i1].y,pTab[i1].z);
    DXFW_VERTEX3 (typ, &pTab[i1], fp_in);
  } 
    
  fprintf(fp_in,"0\nSEQEND\n");  // end of VERT's after 66/1
    
  return 0;

}


/*
//================================================================
  int DXFW_obj2poly (ObjGX *ox1, FILE *fp_in) {
//================================================================
// export CCV as polygon
// TODO: export CCV as analytic obj's with group ?

  int       irc, pNr;
  Point     *pTab;

  pTab = (Point*)memspc201;
  pNr = sizeof(memspc201) / sizeof(Point);


  irc = UT3D_npt_ox__ (&pNr, pTab, ox1, UT_DISP_cv);
  if(irc < 0) return irc;

  return dxfw_POLYLINE (pNr, 32, pTab, fp_in);

}
*/


//================================================================
  int DXFW_3DFACE_out (Point *p1, Point *p2, Point *p3,
                       char *layNam, int icol) {
//================================================================
// Input:
//   icol     ACI (Autocad-Index-Color)


  char      cbuf[160];


  fprintf(fpo1,"0\n3DFACE\n");

  if(layNam[0])
  fprintf(fpo1,"8\n%s\n",layNam);

  if(icol >= 0) 
  fprintf(fpo1,"62\n%d\n",icol);

  sprintf(cbuf, "10\n%f\n20\n%f\n30\n%f",p1->x,p1->y,p1->z);
  fprintf(fpo1,"%s\n",cbuf);

  sprintf(cbuf, "11\n%f\n21\n%f\n31\n%f",p2->x,p2->y,p2->z);
  fprintf(fpo1,"%s\n",cbuf);

  sprintf(cbuf, "12\n%f\n22\n%f\n32\n%f",p3->x,p3->y,p3->z);
  fprintf(fpo1,"%s\n",cbuf);

  sprintf(cbuf, "13\n%f\n23\n%f\n33\n%f",p1->x,p1->y,p1->z);
  fprintf(fpo1,"%s\n",cbuf);


  return 0;

}


//================================================================
  int DXFW_3DFACE__ (ObjGX *ox1, int typ, long dbi, FILE *fpo) {
//================================================================
// typ:   SUR|SOL


  static char layNam[] = "0";

  int       irc, ii, triSiz, triNr, surSiz, surNr, icol;
  int       sTyp[1];
  long      sTab[1];
  char      cbuf[160];
  ObjGX     *oTab=NULL;
  Triang  *triTab, *tr1;
  TypTsuSur *surTab;
  Point     *p1;


  // printf("DXFW_3DFACE__ \n");

  sTyp[0] = typ;
  sTab[0] = dbi;

  // tesselate -> oTab
  TSU_tess_sTab (&oTab, sTyp, sTab, 1);

  // export all triangles of tess-model
  tess_res_CB__ (&oTab, DXFW_tess_CB);



/*
  // space for triangles --> triTab  (12bytes/Tria)
  triTab = (Triang*)memspc501;
  triSiz = sizeof(memspc501) / sizeof(Triang);
    // printf(" triSiz=%d\n",triSiz);

  surTab = (TypTsuSur*)memspc51;
  surSiz = sizeof(memspc51) / sizeof(TypTsuSur);
    // printf(" surSiz=%d\n",surSiz);

  // get triangles from spc1 --> triTab
  // (triangles are pointers into tesselated data !)
  surNr = surSiz;
  triNr = triSiz;
  irc = TSU_ntria_bMsh__ (surTab, &surNr, surSiz, triTab, &triNr, triSiz, oTab);
    // printf(" _tsu2tria__ irc=%d surNr=%d triNr=%d\n",irc,surNr,triNr);


    // write Triangs
    // for(ii=0;ii<triNr;++ii) DEB_dump_obj__ (Typ_Tria,&triTab[ii],"tria");
  icol = 173;  // TODO
  for(ii = 0; ii < triNr; ++ii) {
    tr1 = &triTab[ii];
    DXFW_3DFACE_out (tr1->pa[0], tr1->pa[1], tr1->pa[2], layNam, icol);
  }
*/

  // free tesselated data
  // if(oTab) free (oTab);
  TSU_free ();



  return 0;

}


//================================================================
  int  DXFW_INSERT (ObjGX *ox1, FILE *fpo) {
//================================================================
// write INSERT-record;
// add blockName to list of subModels
// 
//   INSERT   10,20,30=Origin; 2=BlockNam; 41=X-Scale; 42=Y-Scale; 43=Z-Scale.
//            50=RotAng; 70/71=column/row-count; 44/45=column/row-spacing.
//            Blocks are in "SECTION BLOCKS"; starting with "BLOCK",
//            EndOfBlock = ENDBLK
//            66=AttribsFollowFlag(1=Yes; Atribs end with SEQEND).
//            blocks  (mit ASHADE und AVE_RENDER, ... glass.dxf) dzt skippen.

  int           irc, mTyp, mbi;
  long          l1;
  char          s1[256];
  double        d1;
  Point         p1;
  Vector        *vz1, *vz2, vxn, vzn;
  Mat_4x3       m1, mi1;
  ModelRef      *mr;
  ModelBas      *mbo;


  printf(" DXFW_INSERT: ------------------------\n");


  mr = ox1->data;
  mbi = mr->modNr;
  mbo = DB_get_ModBas (mbi);
    // DEB_dump_obj__ (Typ_Model, mr, "mr:");
    // DEB_dump_obj__ (Typ_SubModel, mbo, "  mbo:");

  vz1 = &(WC_sur_act.vz);
  vz2 = &(mr->vz);   // vz of subModel
    // DEB_dump_obj__ (Typ_VC, vz1, "  vz:");
    // DEB_dump_obj__ (Typ_VC, vz2, "  vz-sm:");
    // DEB_dump_obj__ (Typ_VC, &mr->vx, "  vx-sm:");

  // get modeltype (native or mockup)
  mTyp = mbo->typ;
    // printf(" mTyp=%d\n",mTyp);


  // blockName from modelname
  strcpy(s1, mbo->mnam);
  // change all '/' of mNam into '_' - else no correct filename possible
  UTX_safeName (s1, 2);

  // add blockName to list of subModels
  // UtxTab_add_uniq__ (&dxfw_smTab, s1);
  MemTab_uniq_sav (&dxfw_smTab, &l1, &mbi);


  // subModelName; for mockup-models: remove filetype for INSERT
  if(mTyp >= Mtyp_DXF) {
    strcpy(s1, mbo->mnam);
    UTX_ftyp_cut (s1);
    UTX_safeName (s1, 2);
  }


  // change OCS(ECS) -> WCS
  irc = dxfw_load_mat (m1, vz2);
    // DEB_dump_obj__ (Typ_VC, vz2, " vz2:");
    // DEB_dump_obj__ (Typ_M4x3, m1, "m1:");
  if(irc) {
    // new Z not parallel to old Z
    // inverse matrix
    UT3D_m3_invm3 (mi1, m1);
    // translate the origin into the new axisSystem
    UT3D_pt_tra_pt_m3 (&p1, mi1, &mr->po);

  } else {
    p1 = mr->po;
  }


  fprintf(fpo,"0\nINSERT\n");


  // subModelName; for mockup-models: remove filetype
  fprintf(fpo,"2\n%s\n",s1);


  // origin
  DXFW_point3 (0, &p1, fpo);


  // scales
  if(!UTP_comp2db(mr->scl, 1.0, UT_TOL_Ang1)) {
    fprintf(fpo,"41\n%f\n",mr->scl);
    fprintf(fpo,"42\n%f\n",mr->scl);
    fprintf(fpo,"43\n%f\n",mr->scl);
  }


  // test if vz == 0,0,1. Yes: no z-axis-output. No: write Z-axis+angle.
  // if (UT3D_vc_ck_parl_vc (vz1, vz2, RAD_1)) {
  // printf(" dxf-vz is parl ..\n");
  // d1 = UT3D_angr_3vcn_CCW (vz1, &WC_sur_act.vx, &mr->vx);


  if(!irc) {
    // ONLY FOR Z-parallel
    // get the vectors of new refSys
    UT3D_m3_get (&vxn, 0, m1);
    UT3D_m3_get (&vzn, 2, m1);

    // get the angle between the vx-vectors; CCW, 0-2pi.
    d1 = UT3D_angr_3vcn_CCW (&vzn, &WC_sur_act.vx, &vxn);
      // printf(" angr=%f\n",d1);
    if(!UTP_compdb0(d1, RAD_1)) {
      // 50/rotAng
      fprintf(fpo,"50\n%f\n", UT_DEGREES(d1));
    }


  } else {
    // ONLY FOR NOT Z-parallel
    // add new Z-axis as 210,220,230
    fprintf(fpo,"210\n%f\n",vz2->dx);
    fprintf(fpo,"220\n%f\n",vz2->dy);
    fprintf(fpo,"230\n%f\n",vz2->dz);
  }

 

  return 0;

}


//=====================================================================
  int DXFW_ox (ObjGX *ox1, int TrInd, int typ, long dbi) {
//=====================================================================
// dxfw_rec
// typ ?



  int           i1, zparl, ptNr, pNr;
  double        a1, a2, d1, d2, d3;
  Point2        pt2c, pt21, pt22, *p2a;
  Point         ptc, pt1, pt2, *pa, *pTab;
  Vector2       vc21, vc22;
  Line          *ln1;
  Circ          *ci1;
  // Point2Tab     *p2Tab;
  // PointTab      *pTab;
  Mat_4x3       m1, im1;
  ObjG          *el;


  printf("DXFW_ox ox1-typ=%d ox1-form=%d tr=%d typ=%d dbi=%ld\n",
         ox1->typ,ox1->form,TrInd,typ,dbi);


  // IG_TrInd = TrInd;
  // el       = *objIn;


/*
  if(ox1->typ == Typ_ObjGX) {
    typ = ox1->form;
  } else {
    // bei den Kurven ...
    typ = ox1->typ;
  }
*/


  switch (ox1->typ) {


    //=========================================================
    case Typ_PT:
      fprintf(fpo1,"0\nPOINT\n");
      fprintf(fpo1,"8\n0\n"); // def.Layer
      pa = ox1->data;
      DXFW_point3 (0, &pa[0], fpo1);
      break;


    //=========================================================
    case Typ_LN:
      fprintf(fpo1,"0\nLINE\n");
      fprintf(fpo1,"8\n0\n"); // def.Layer
      ln1 = ox1->data;
      DXFW_point3 (0, &ln1->p1, fpo1);
      DXFW_point3 (1, &ln1->p2, fpo1);
      break;


    //=========================================================
    case Typ_CI:
      DXFW_CI (ox1->data, fpo1);
      break;


    //=========================================================
    case Typ_CVPOL:
      return DXFW_POLYLN3 (ox1, 32, fpo1);


    //=========================================================
    case Typ_CVBSP:
      if(dxfw_subtyp < 2) {    // ab 2 = R14
        return DXFW_POLYLN3 (ox1, 32, fpo1);
      } else {
        return dxfw_SPLINE (ox1->data, fpo1);
      }


    //=========================================================
    case Typ_CVELL:
      if(dxfw_subtyp < 2) {    // ab 2 = R14
        return DXFW_POLYLN3 (ox1, 32, fpo1);
      } else {
        return dxfw_ELLIPSE (ox1->data, fpo1);
      }


    //=========================================================
    case Typ_CVCLOT:
      return DXFW_POLYLN3 (ox1, 32, fpo1);


    //=========================================================
    case Typ_CVPOL2:
      return DXFW_POLYLN2 (ox1, 32, fpo1);


    //=========================================================
    case Typ_Note:
        printf(" dxfw-Typ_Note typ=%d\n",typ);

      if(typ == Typ_GTXT)  return DXFW_TEXT (ox1->data, fpo1);
      if(dxfw_subtyp >= 90) {
        // LOG_A__
        TX_Print("**** skip dimension (no DIMENSION with headerless dxf)");
        break;
      }
      if(typ == Typ_Dimen) return DXFW_DIM (ox1->data, fpo1);
      break;


    //=========================================================
    case Typ_CVTRM:
      return DXFW_POLYLN3 (ox1, 32, fpo1);

    //=========================================================
// Typ_PLN
    case Typ_SUR:
    case Typ_SURCON:
    case Typ_SURRU:
    case Typ_SURRV:
    case Typ_SURSWP:
    case Typ_SURBSP:
    case Typ_SURRBSP:
    case Typ_SURPLN:
    case Typ_SUTP:
// Typ_SURHAT
    case Typ_SURCIR:
    case Typ_SURSTRIP:
    case Typ_SURPMSH:
// Typ_SURBND Typ_SURPTAB
    // bodies:
    case Typ_SOL:
    case Typ_SPH:
    case Typ_CON:
    case Typ_TOR:
    case Typ_PRI:
      return DXFW_3DFACE__ (ox1, typ, dbi, fpo1);


    //=========================================================
    case Typ_Model:
    case Typ_Mock:
      // add to list of subModels
      return  DXFW_INSERT (ox1, fpo1);

    //=========================================================
    default:
      printf("  dxfw skip %d\n",ox1->typ);
      return -1;

  }



/*
  //  Bestandteile von CCV's (und getrimmten Flaechen ...) ausgeben
  irc = IGE_w_subObjs (&el, fpo1, fpo1);
  if (irc < 0) return irc;

  IG_mode  = 0;             // 0=normales Obj; 1=SubObj (von CCV ..)


  // ein Obj ausgeben (TrMat, D-Zeilen, P-Zeile)
  irc = IGE_w_obj (&el, apt_typ, apt_ind, fpo1, fpo1);
  if (irc < 0) return irc;
*/

  return 0;

}


//=============================================================================
  int DXFW_main () {
//=============================================================================
// export all objs in group

  int       oNr, i1, iTyp, tra_ind = 0;
  ObjDB     *oTab;
  ObjGX     ox1;


  oNr = Grp_get__ (&oTab);

  printf("DXFW_main ============================ %d\n",oNr);


  for(i1=0; i1<oNr; ++i1) {

    iTyp = oTab[i1].typ;


    // skip this types:
    if(iTyp == Typ_Ditto) continue;


    // get DB-obj
    ox1 = DB_GetObjGX (iTyp, oTab[i1].dbInd);
    if(ox1.typ == Typ_Error) {
      LOG_A__ (MSG_ERR_typ_ERR, "DXFW_main typ=%d dbi=%ld",
               oTab[i1].typ,oTab[i1].dbInd);
      ++dxfw_errNr;
      continue;
    }

    // export DB-obj
    DXFW_ox (&ox1, tra_ind, oTab[i1].typ, oTab[i1].dbInd);
    ++dxfw_objNr;

  }



  return 0;

}


//=============================================================================
  int DXFW_Mdl_tess (char *mdlNam) {
//=============================================================================
// AP_ExportIges_Model

  // Point    pb1, pb2, pOri;
  char     fNam[256];
  Memspc   impSpc;


  printf("DXFW_Mdl_tess |%s|\n",mdlNam);

  // fix filename for tess-model
  sprintf(fNam, "%s%s.tess",OS_get_tmp_dir(),mdlNam);
    printf(" fTess=|%s|\n",fNam);

  // load tess-model from file
  if(OS_checkFilExist (fNam, 1) == 0) {
    TX_Print("***** ERR DXFW_Mdl_tess %s",fNam);
    return -1;
  }
 
  // import tess-model into mem
  TSU_imp_tess (&impSpc, fNam);

  // export all triangles of tess-model
  tess_res_CB__ (&impSpc, DXFW_tess_CB);

  UME_free (&impSpc);

  return 0;

}


//================================================================
  int DXFW_tess_CB (ObjGX *oxi) {
//================================================================
  
  static int  iCol;
  static char layNam[] = "0";
  
  int       i1, triNr, triSiz;
  ColRGB    sCol, actCol, defCol;
  Triang  *triTab, *tr1;
  

  if(!oxi) {
    // INIT
      // printf("DXFW_tess_CB INIT\n");
    return -1;
  }

  // printf("DXFW_tess_CB %d\n",oxi->form);


  switch (oxi->form) {

    //----------------------------------------------------------------
    case Typ_PT:           // normal faces
      triTab = (Triang*)memspc501;
      triSiz = sizeof(memspc501) / sizeof(Triang);
      triNr = 0;
      // get triangles from tesselated face
      tess_ntri_tfac__ (triTab, &triNr, triSiz, oxi);
        // printf("  fac ptNr=%d glTyp=%d fNr=%d\n",oxi->siz,oxi->aux,triNr);
      if(triNr < 1) {
        printf("****** EMPTY FACE typ=%d form=%d glTyp=%d\n",
                oxi->typ, oxi->form, oxi->aux);
        return -1;
      }
      for(i1=0; i1<triNr; ++i1) {
        tr1 = &triTab[i1];
        DXFW_3DFACE_out (tr1->pa[0], tr1->pa[1],tr1->pa[2], layNam, iCol);
      }

      break;


    //----------------------------------------------------------------
    case Typ_Int4:
      //----------------------------------------------------------------
      if(oxi->typ == Typ_Color) {    // color of subsequent faces
        sCol = *((ColRGB*)&oxi->data);
        if(MEM_cmp__(&sCol, &actCol, sizeof(ColRGB))) {
          actCol = sCol;
          printf(" Col r%d g%d b%d\n",sCol.cr,sCol.cg,sCol.cb);
          // change RGB-color -> ACI-color
          iCol = DXF_colACI_colRGB (sCol.cr,sCol.cg,sCol.cb);
        }

      //----------------------------------------------------------------
      } else {
        printf(" DXFW_tess_CB UNUSED typ %d form Typ_Int4\n",oxi->typ);
      }
      break;

    //----------------------------------------------------------------
    default:
      printf(" DXFW_tess_CB UNUSED form %d\n",oxi->form);

  }

  return 0;

}


//=============================================================================
  int DXFW_Mdl_gcad (int modNr) {
//=============================================================================
// AP_ExportIges_Model

  long     gr_ind;
  long     l1, apt_ind;
  int      i1, irc, tra_act, apt_typ,  anz_obj=0, tra_ind = 0,
           anz_pt=0, anz_ln=0, anz_ac=0, anz_cv=0;
  Point    pt1;
  Line     ln1;
  Circ     ci1;
  ObjG     o1;
  ObjGX    ox1, *op1;
  Plane    pl1;
  Mat_4x3  m1;
  DL_Att   dla;



  printf("DXFW_Mdl_gcad %d\n",modNr);


  //----------------------------------------------------------------
  apt_typ = Typ_PT;

  for(apt_ind=1; apt_ind<APT_PT_SIZ; ++apt_ind) {
    if(DB_QueryDef(apt_typ, apt_ind) < 0) continue;
    ox1 = DB_GetObjGX (apt_typ, apt_ind);
    if(ox1.typ == Typ_Error) continue;

    DXFW_ox (&ox1, tra_ind, apt_typ, apt_ind);
    ++anz_obj;
  }


  //-----------------------------------
  apt_typ = Typ_LN;

  for(apt_ind=1; apt_ind<APT_LN_SIZ; ++apt_ind) {
    if(DB_QueryDef(apt_typ, apt_ind) < 0) continue;
    ox1 = DB_GetObjGX (apt_typ, apt_ind);
    if(ox1.typ == Typ_Error) continue;

    DXFW_ox (&ox1, tra_ind, apt_typ, apt_ind);
    ++anz_obj;
  }


  //-----------------------------------
  apt_typ = Typ_CI;

  for(apt_ind=1; apt_ind<APT_CI_SIZ; ++apt_ind) {
    if(DB_QueryDef(apt_typ, apt_ind) < 0) continue;
    ox1 = DB_GetObjGX (apt_typ, apt_ind);
    if(ox1.typ == Typ_Error) continue;

    DXFW_ox (&ox1, tra_ind, apt_typ, apt_ind);
    ++anz_obj;
  }


  //-----------------------------------
  apt_typ = Typ_CV;

  for(apt_ind=0; apt_ind<APT_CV_SIZ; ++apt_ind) {
    if(DB_QueryDef(apt_typ, apt_ind) < 0) continue;
    ox1 = DB_GetObjGX (apt_typ, apt_ind);
    if(ox1.typ == Typ_Error) continue;

    DXFW_ox (&ox1, tra_ind, apt_typ, apt_ind);
    ++anz_obj;
  }


  //-----------------------------------
  apt_typ = Typ_GTXT;

  for(apt_ind=0; apt_ind<APT_TX_SIZ; ++apt_ind) {
    if(DB_QueryDef(apt_typ, apt_ind) < 0) continue;
    ox1 = DB_GetObjGX (apt_typ, apt_ind);
    if(ox1.typ == Typ_Error) continue;

    DXFW_ox (&ox1, tra_ind, apt_typ, apt_ind);
    ++anz_obj;
  }


  //-----------------------------------
  apt_typ = Typ_SUR;

  for(apt_ind=0; apt_ind<APT_SU_SIZ; ++apt_ind) {
    if(DB_QueryDef(apt_typ, apt_ind) < 0) continue;
    ox1 = DB_GetObjGX (apt_typ, apt_ind);
    if(ox1.typ == Typ_Error) continue;

    DXFW_ox (&ox1, tra_ind, apt_typ, apt_ind);
    ++anz_obj;
  }


  //-----------------------------------
  // SubfigInst; 
  apt_typ = Typ_Model;

  for(apt_ind=0; apt_ind<APT_MR_SIZ; ++apt_ind) {
    if(DB_QueryDef(apt_typ, apt_ind) < 0) continue;
    ox1 = DB_GetObjGX (apt_typ, apt_ind);
    if(ox1.typ == Typ_Error) continue;

    DXFW_ox (&ox1, tra_ind, apt_typ, apt_ind);
    ++anz_obj;
  }




  //-----------------------------------
  return anz_obj;

}

//==========================================================================
  int DXFW_cat_file (FILE *fpo, char *fnam) {
//==========================================================================
/// \code
/// MICROSOFT-BUG: you may not write into a file opened in dll with core-function
/// UTX_cat_file           add file into open fileunit
/// add file 
/// \endcode



  long    l1;
  char    *fBuf;


  printf("DXFW_cat_file/UTX_cat_file |%s|\n",fnam);


  l1 = OS_FilSiz (fnam);
  fBuf = MEM_alloc_tmp ((int)(l1 + 128));
  MEM_get_file (fBuf, &l1, fnam);
  fwrite (fBuf, 1, l1, fpo);

  return 0;

}


/*
//================================================================
  int DXFW_init__ (char * fn) {
//================================================================
// Input:
//   dxfw_subtyp    0=DxfR12  1=Dxf2000



  int      i1, anz_obj=0, ipos, modNr;
  FILE     *fp1, *fp2, *fpi;
  char     s1[256];


  printf("DXFW_init__ %d |%s|\n",dxfw_subtyp,fn);


  if ((fp1 = fopen (fn, "w+")) == NULL) {
    TX_Error ("open file %s",fn);
    return;
  }


  //==================================================================
  // in BasicModels gibts Reihenfolgenummer seqNr.
  // Diese in korrekter Reihenfolge -> Datei <tmpdir>/Mod.lst ausgeben.
  DB_list_ModBas ();


  // try to open inListe
  sprintf(s1,"%sMod.lst",OS_get_tmp_dir());
  if((fpi=fopen(s1,"r")) == NULL) {
    TX_Print("AP_ExportIges__ E002 %s",s1);
    return;
  }

  sprintf(s1,"%sModel_",OS_get_tmp_dir());
  ipos = strlen(s1);


  // loop tru subModels
  while (!feof (fpi)) {
    if (fgets (&s1[ipos], 256, fpi) == NULL) break;
    UTX_CleanCR (s1);
      printf("nxt model |%s|\n",s1);

/
//     DB_Init (1);
//     // RUN (abarbeiten)
//     ED_Init ();
//     if(ED_work_file(cbuf) < 0){TX_Error("AP_ExportIges__: E003");goto L_err;}
//     ED_lnr_reset ();
//     WC_Work__ (0, NULL);   // Init Levelcounter in WC_Work__
//     ED_Run ();
// 
//     // work submodel
//     modNr = DB_get_ModNr(&cbuf[ipos]);  // get ModelNr from Modelname
//     if(modNr < 0) {TX_Error("AP_ExportIges__: E004"); goto L_err;}
//     i1 = AP_ExportIges_Model (modNr, fp1, fp2);  NEW: DXFW_Mdl_gcad !
//     anz_obj += i1;
/

  }
  fclose(fpi);


  //----------------------------------------------------------------
  // resolv main
  // DB_Init (1);
  // ED_Reset ();  // ED_lnr_act = 0; 2004-02
  // ED_work_END (0);

  //------------------------------------
  fprintf(fp1,"0\nSECTION\n");
  fprintf(fp1,"2\nENTITIES\n");


  // i1 = DXFW_Mdl_gcad (-1, fp1, fp2);
  i1 = DXFW_main (fp1, fp2);
  anz_obj += i1;


  fprintf(fp1,"0\nENDSEC\n");



  //----------------------------------------------------------------
  // join files

  fclose (fp1);





  //----------------------------------------------------------------
  // print statistics
  TX_Print("   Objekte exportiert:  %d",anz_obj);


// exit(0);

  return 0;

}



//================================================================
  int DXFW_w_grp (char * fn) {
//================================================================
// export main - all obj's in group.
// Input:
//   dxfw_subtyp    0=DxfR12  1=Dxf2000


  int      i1;  //, anz_obj=0, ipos, modNr;
  FILE     *fp1, *fp2;
  // char     s1[256];



  printf("DXFW_w_grp %d |%s|\n",dxfw_subtyp,fn);


  if ((fp1 = fopen (fn, "w+")) == NULL) {
    TX_Error ("open file %s",fn);
    return -1;
  }



  //------------------------------------
  // export mainModel
  fprintf(fp1,"0\nSECTION\n");
  fprintf(fp1,"2\nENTITIES\n");


  // i1 = DXFW_Mdl_gcad (-1, fp1, fp2);
  i1 = DXFW_main (fp1, fp2);


  fprintf(fp1,"0\nENDSEC\n");



  //----------------------------------------------------------------
  // export subModels
/
//   // loop tru subModels
// 
//   while (!feof (fpi)) {
//     if (fgets (&s1[ipos], 256, fpi) == NULL) break;
//     UTX_CleanCR (s1);
//       printf("nxt model |%s|\n",s1);
// 
//     DB_Init (1);
//     // RUN (abarbeiten)
//     ED_Init ();
//     if(ED_work_file (cbuf) < 0) {TX_Error("AP_ExportIges__: E003"); goto L_err;}
//     ED_lnr_reset ();
//     WC_Work__ (0, NULL);   // Init Levelcounter in WC_Work__
//     ED_Run ();
// 
//     // work submodel
//     modNr = DB_get_ModNr(&cbuf[ipos]);  // get ModelNr from Modelname
//     if(modNr < 0) {TX_Error("AP_ExportIges__: E004"); goto L_err;}
//     i1 = AP_ExportIges_Model (modNr, fp1, fp2);  NEW: DXFW_Mdl_gcad !
//     anz_obj += i1;
// 
//   }
/



  //----------------------------------------------------------------
  // join files

  fclose (fp1);



  //----------------------------------------------------------------
  // print statistics
  TX_Print("   Objekte exportiert:  %d",dxfw_objNr);


// exit(0);

  return 0;

}
*/

// EOF
