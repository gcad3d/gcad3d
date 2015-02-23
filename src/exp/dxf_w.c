// DXF read, write  dxfr_ dxfw_                          RF  2001-06-17
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
export CCV as analytic obj's with group ?
ATTRIB-textblock:
  via N24=LDRS P(1.5 -36.5 0) 1 (als Symbol) ausgeben;
  dazu muesste N=LDRS aber zusaetzlich Text ausgeben koennen !
Offen Export:
 Toleranzen (Sub- und Superscript). Werden zB ex 
   $DIMTM / $DIMTP ?
   C5 macht eigenen Text f jede Toleranz!
Offen Import:
  Fuer Statistik: Liste benutzter Layers und Anzahl Elemente pro Layer.
  Man soll aktuellen Layer und Farbe und Linetyp via Query abfragen können.
  DIMENSION: sind manchmal in der BLOCKS-Section als BLOCK; dann sind die
    Punkte in der ENTITIES-Section leer. Ev alle BLOCKS einlesen und
    zwischenspeichern wie bei Iges.
    BLOCK: hat 8/Blockname; Zugehoerigkeit ebenfalss via 8/Blockname ?

-----------------------------------------------------
Modifications:
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

dxfw_init          write DXF-Prolog
dxfw_end           write Endrecord
dxfw_rec           write obj
dxfw_VERTEX        3D-pt
dxfw_VERTEX2       2D-pt
dxfw_hd_POLYLINE   wr header of POLYLINE
dxfw_POLYLINE      .
dxfw_fl_out        wr int + double
dxfw_point2        wr 2D-pt
dxfw_point         wr 3D-pt
dxfw_vector        wr vec
dxfw_load_mat      TrMat from vec

List_functions_end:
=====================================================

\endcode *//*----------------------------------------




==============================================================================
Memory-Usage  memspc (../xa/xa_mem.h):

dxf-Write:
   memspc55   f Text ..
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

  HEADER - Section (DXF-Versionsnummer, $TEXTSIZE, $DIMASZ ..)
  TABLES - Section (LineTypTable, LayerTable)
  BLOCKS - Section (??)
  ENTITIES - Section (die Objekte)
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
                         6=Magenta(violett), 7=Schwarz (=RGB-Mischung)
                         256=BYLAYER ?
                         manchmal als 6-Char-Int.
  EntityStart  66    zB "     1" (I6 ?) bei POLYLINE-Header.
  Int-Werte    70-78 bei POLYLINE-Header zB 17,6,18. ??
               70 bei DIMENSION: Typ.160 161 162 163 164
  ?            100   "AcDbLine" bei LINE ??
  Z-Vektor     210,220,230  Z-Vektor, wenn <> dem Haupt-Z-Vektor.
  Kommentar    999   Kommentarzeile.

  ?            370   Wet immer -1; whats that ?



Versionen (stehen in HEADER-Section):
----------
  9 / $ACADVER / 1 / AC1006  = R10
  9 / $ACADVER / 1 / AC1009  = R11 and R12
  9 / $ACADVER / 1 / AC1014  = R14
  9 / $ACADVER / 1 / AC1015  = AutoCAD 2000


Absolutes Minimum also:

  0
SECTION
  2
HEADER
  0
ENDSEC
  0
SECTION
  2
BLOCKS
  0
ENDSEC
  0
SECTION
  2
ENTITIES
  0
ENDSEC
  0
EOF


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
          drawn. The X axis of the UCS in effect when the Dimension
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


typedef enum { DXF_start         = 0,
         DXF_textval       = 1,
         DXF_name          = 2,
         DXF_othername1    = 3,
         DXF_othername2    = 4,
         DXF_entity_handle = 5,
         DXF_line_type     = 6,
         DXF_text_style    = 7,
         DXF_layer_name    = 8,
         DXF_var_name      = 9,
         DXF_primary_X     = 10,
         DXF_other_X_1     = 11,
         DXF_primary_Y     = 20,
         DXF_other_Y_1     = 21,
         DXF_primary_Z     = 30,
         DXF_other_Z       = 31,
         DXF_elevation     = 38,
         DXF_thickness     = 39,
         DXF_floatvals     = 40,
         DXF_repeat        = 49,
         DXF_angle1        = 50,  DXF_angle1        = 51,
         DXF_angle1        = 52,  DXF_angle1        = 53,
         DXF_angle1        = 54,  DXF_angle1        = 55,
         DXF_angle1        = 56,  DXF_angle1        = 57,
         DXF_angle1        = 58,
         DXF_colornum      = 62,
         DXF_entities_flg  = 66,
         DXF_ent_ident     = 67,
         DXF_view_state    = 69,
         DXF_SeventyFlag   = 70,  // I guess I have a horror of literal
         DXF_SeventyOneFlag= 71,  // constants.
         DXF_SeventyTwoFlag= 72,
         DXF_comment       = 999 } DXF_values;


//   Notes: Words that are in capitols are tokens (see tokens.h),
//          that is, values returned by the scanner.
//          Words that start with "DXF_" are DXF integer values
//          with special meanings.  Curley braces ("{" and "}")
//          mean "one or more occurances".
//
// DXF_Section = DXF_start SECTION DXF_name SecName SecData SecEnd
// SecName = HEADER | TABLES | ENTITIES
// SecData = DXF_start SecStuff
// SecEnd = DXF_start ENDSEC
// SecStuff = TableSec | EntitiesSec
// TableSec = TABLE TableGroup DXF_start ENDTAB
// TableGroup = LayerGroup | OtherGroup
// OtherGroup = DXF_name GroupName GroupData
// LayerGroup = DXF_name LAYER DXF_SeventyFlag MaxItems {LayerDesc}
// LayerDesc = DXF_start LAYER DXF_name LayerName
//             DXF_SeventyFlag FlagVals DXF_colornum
//             ColorNum DXF_line_type LineType
// LineType = CONTINUOUS
// EntitiesSec = {DXF_start EntType DXF_layer_name EntName EntData}
// EntTyp = THREE_D_FACE
// EntData = ThreeD_data
// ThreeD_data = {Face}
// Face = Set1 Set2 Set3 Set4 EndSet
// EndSet = 62 0
//

-----------------------------------------------
   Process a DXF file POLYLINE section and enter the points in the
   polygon face list.

   An attempt at explaining the POLYLINE format is shown below:

     PolyLineHeader PolyVert MeshVals
     PolyLineHeader = DXF_start POLYLINE DXF_name EntName
     PolyVert = DXF_entities_flg PolyElev
     PolyElev = 10 0 20 0 30 Elevation
     MeshVals = 70 0 71 PolygonCount 72 PointCount
     EntName = string

   The code below skips the POLYLINE header and the POLYLINE vertices
   section.  It looks for the "72" marker, which indicates the number
   of points per polygon.  Or at least that is what I think it means
   and what this object uses it for.

   Following the 71 and 72 groups is a sequence of VERTEX objects, where
   each VERTEX describes one 3D point.  It is these points that are
   entered into the polygon face list.

   I have not been able to get POLYLINE objects to display properly, so
   there is clearly something wrong with my interpretation of the
   DXF file format spec.

-----------------------------------------------
   A 3DFACE section consists of either three or four 3D points.  Each
   "face" is read in and added to the face list for the object.
  // The last token fetched should be 3DFACE.  This is followed by some
  // optional information about the face (which this program ignores),
  // followed by the point descriptions.
  //
  // The points are preceeded by the integers {10, 20,  30}, {11, 21, 31},
  // {12, 22, 32},  {13, 23, 33}.  There will always be four points.
  // if the polygon is a triangle, the third and fourth points will be
  // the same.






-------------------------------------------------------------------
*/
#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// #include "../ut/ut_umem.h"            // UME_reserve
#include "../ut/ut_geo.h"
#include "../ut/ut_txt.h"
#include "../ut/ut_TX.h"

#include "../gr/ut_UI.h"              // SYM_SQUARE ..

#include "../xa/xa.h"          // mem_cbuf1_SIZ
#include "../xa/xa_mem.h"      // memspc55


// #include "../ut/ut_crv.h"


#include "../exp/dxf_w.h"


extern double     AP_txsiz;       // Notes-Defaultsize
extern double     AP_txdimsiz;    // Dimensions-Text-size



static int       dxf_version;     // 0=R12; 1=2000





/*=====================================================================*/
  int dxfw_init (FILE *fp_in, int dxfVersion) {
/*=============

  DXF-Prolog ausgeben.

*/

/*
  int i1;
  char *dxfi1[] = {
    "0","SECTION","2","HEADER","9","$ACADVER","1","AC1009","0","ENDSEC",
    "0","SECTION","2","TABLES","0","ENDSEC",
    "0","SECTION","2","BLOCKS","0","ENDSEC",
    "0","SECTION","2","ENTITIES",0
  };

  for (i1=0; dxfi1[i1] != '\0'; i1++)
    fprintf(fp_in,"%s\n",dxfi1[i1]);
*/

  printf("dxfw_init %d\n",dxfVersion);

  dxf_version = dxfVersion;


  //------------------------------------
  fprintf(fp_in,"0\nSECTION\n");
  fprintf(fp_in,"2\nHEADER\n9\n$ACADVER\n");

  if(dxf_version == 0) {              // 0=R12
    fprintf(fp_in,"1\nAC1009\n");

  } else {                            // 1=2000
    fprintf(fp_in,"1\nAC1015\n");
  }

  fprintf(fp_in,"9\n$TEXTSIZE\n40\n%f\n",AP_txsiz);
  fprintf(fp_in,"9\n$DIMTXT\n40\n%f\n",AP_txdimsiz);
  fprintf(fp_in,"9\n$DIMASZ\n40\n%f\n",AP_txdimsiz);

  // anzahl nachkommastellen f dimWerte
  fprintf(fp_in,"9\n$DIMDEC\n70\n2\n");
  fprintf(fp_in,"9\n$DIMADEC\n70\n2\n");  // fuer DIM-Angles

  fprintf(fp_in,"0\nENDSEC\n");


  //------------------------------------
  fprintf(fp_in,"0\nSECTION\n");
    fprintf(fp_in,"2\nTABLES\n");

    // Defaultlayer 0; fuer POINT LINE ARC ... via 8/0
    fprintf(fp_in,"0\nTABLE\n2\nLAYER\n70\n1\n");

    // 2=Layername; 70=typ, 620col, 6=Linetyp
    fprintf(fp_in,"0\nLAYER\n2\n0\n70\n0\n62\n7\n6\nCONTINUOUS\n");

    fprintf(fp_in,"0\nENDTAB\n");

  fprintf(fp_in,"0\nENDSEC\n");


  //------------------------------------
  fprintf(fp_in,"0\nSECTION\n");
  fprintf(fp_in,"2\nBLOCKS\n");

    // defaultblock *D0  f DIM's via 2/*D0
    fprintf(fp_in,"0\nBLOCK\n2\n*D0\n70\n1\n62\n7\n");
    fprintf(fp_in,"10\n0\n20\n0\n30\n0\n");
    fprintf(fp_in,"0\nENDBLK\n");

  fprintf(fp_in,"0\nENDSEC\n");


  //------------------------------------
  fprintf(fp_in,"0\nSECTION\n");
  fprintf(fp_in,"2\nENTITIES\n");


  return 1;

}






/*=====================================================================*/
  void dxfw_end (FILE *fp_in) {
/*=============

Endrecord ausgeben.

*/

  printf("dxfw_end\n");

  fprintf(fp_in,"0\nENDSEC\n0\nEOF\n");

}



//================================================================
  int dxfw_rec (ObjGX *ox1, FILE *fp_in) {
//================================================================
// export obj into dxf-file
// 
// irc =  0 = OK;
// irc = -1 = Obj Error (Obj skipped ..)
// irc = -2 = Error; stop all.



  int           i1, typ, zparl, ptNr, pNr;
  double        a1, a2, d1, d2, d3;
  Point2        pt2c, pt21, pt22, *p2a;
  Point         ptc, pt1, pt2, *pa, *pTab;
  Vector2       vc21, vc22;
  Line          *ln1;
  Circ          *ci1;
  // Point2Tab     *p2Tab;
  // PointTab      *pTab;
  GText         *tx1;
  Dimen         *dim1;
  Mat_4x3       m1, im1;
  ObjG          *el;


/* -------------------------------------------------------- */
  printf("dxfw_rec typ=%d form=%d\n",ox1->typ,ox1->form);


  //dxfw_head (el, fp_in);



  if(ox1->typ == Typ_ObjGX) {
    typ = ox1->form;
  } else {
    // bei den Kurven ...
    typ = ox1->typ;
  }




  switch (typ) {


    case Typ_PT:
      fprintf(fp_in,"0\nPOINT\n");
      fprintf(fp_in,"8\n0\n"); // def.Layer
      pa = ox1->data;
      dxfw_point (0, &pa[0], fp_in);
      break;


    case Typ_LN:
      fprintf(fp_in,"0\nLINE\n");
      fprintf(fp_in,"8\n0\n"); // def.Layer
      ln1 = ox1->data;
      dxfw_point (0, &ln1->p1, fp_in);
      dxfw_point (1, &ln1->p2, fp_in);
      break;


    case Typ_CI:
      ci1 = ox1->data;
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
      dxfw_load_mat (m1, &ci1->vz);  // eine TrMat nach DXF-Konvention generieren
      UT3D_m3_invm3 (im1, m1);      // RücktransformationsMat. generieren
      UT3D_pt_traptm3 (&pt1, im1, &ci1->pc);
      //TX_Print(" pt1=%f,%f,%f",pt1.x,pt1.y,pt1.z);


      //Ausgabe Vollkreis
      L_CI_out:
      dxfw_point (0, &pt1, fp_in);                  // Mittelpunkt als 10,20,30
      fprintf(fp_in,"40\n%f\n",fabs(ci1->rad));      // Radius 
      if(zparl==OFF) dxfw_vector (&ci1->vz, fp_in);
      break;





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
      dxfw_load_mat (m1, &ci1->vz);  // eine TrMat nach DXF-Konvention generieren
      UT3D_m3_invm3 (im1, m1);      // RücktransformationsMat. generieren
      UT3D_pt_traptm3 (&ptc, im1, &ci1->pc);
      UT3D_pt_traptm3 (&pt1, im1, &ci1->p1);
      UT3D_pt_traptm3 (&pt2, im1, &ci1->p2);
      //TX_Print(" pt1=%f,%f,%f",pt1.x,pt1.y,pt1.z);



      L_AC_out:
      dxfw_point (0, &ptc, fp_in);  // Mittelpunkt

      // Radius
      fprintf(fp_in,"40\n%f\n",fabs(ci1->rad));


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


      if(zparl==OFF) dxfw_vector (&ci1->vz, fp_in);

      break;




    //=========================================================
    case Typ_CVPOL:
      dxfw_POLYLINE (((CurvPoly*)ox1->data)->ptNr, 32,
                     ((CurvPoly*)ox1->data)->cpTab, fp_in);
      break;



    //=========================================================
    case Typ_CVBSP:
      if(dxf_version == 0) {    // 0=R12
        dxfw_obj2poly (ox1, fp_in);
      } else {  // 1=2000
        dxfw_SPLINE (ox1->data, fp_in);
      }
      break;


    //=========================================================
    case Typ_CVELL:
      if(dxf_version == 0) {    // 0=R12
        dxfw_obj2poly (ox1, fp_in);
      } else {  // 1=2000
        dxfw_ELLIPSE (ox1->data, fp_in);
      }
      break;


    //=========================================================
    case Typ_CVCLOT:
      dxfw_obj2poly (ox1, fp_in);
      // pTab = (Point*)memspc201; 
      // pNr = sizeof(memspc201) / sizeof(Point);
      // // elli -> polygon -> mem?
      // UT3D_npt_ox (&pNr, pTab, ox1, UT_DISP_cv);
      // dxfw_POLYLINE (pNr, 32, pTab, fp_in);
      break;


    //=========================================================
    case Typ_CVPOL2:
      dxfw_hd_POLYLINE (fp_in); // wr header of POLYLINE


      // p2Tab = (Point2Tab*)ox1->data;
      // p2a   = (Point2*)p2Tab->ptTab;
      p2a   = (Point2*)ox1->data;
      for(i1=0;i1<ox1->siz; ++i1) {
        dxfw_VERTEX2 (32, &p2a[i1], fp_in);
      }

      fprintf(fp_in,"0\nSEQEND\n");
      break;





    //=========================================================
    case Typ_GTXT:
      fprintf(fp_in,"0\nTEXT\n");
      fprintf(fp_in,"8\n0\n"); // def.Layer


      tx1 = (GText*)ox1->data;

      // posi
      dxfw_point (0, &tx1->pt, fp_in);

      // 7=Textstyle "normal#0.300000#0.900000" od cursive#0.500000#3.000000

      // 40-Textsize (characterhoehe)
      // Notes haben size -1!
      d1 = tx1->size;
      if(d1 < 0) d1 = 1.;;
      // d1 *= DXF_fakt_txtSiz;
      dxfw_fl_out (40, d1, fp_in);

      // 50-Textdirection
      dxfw_fl_out (50, tx1->dir, fp_in);

      // 1-Text
      GR_gxt_dxfout (0, memspc011, tx1->txt);
      fprintf(fp_in,"1\n%s\n",memspc011);

      // 39 = Thickness; 1 oder 2
      i1 = 1;
      if(tx1->size > 1.) i1 = 2;
      fprintf(fp_in,"39\n%d\n",i1);

      // 62 = colornum = 0
      fprintf(fp_in,"62\n0\n");


      break;



    //=========================================================
    case Typ_Dimen:

      dim1 = (Dimen*)ox1->data;
      UT3D_stru_dump (Typ_Dimen, dim1, "dim1");

      if(dim1->dtyp != 21) {     // 21=Leader

        fprintf(fp_in,"0\nDIMENSION\n");
        fprintf(fp_in,"8\n0\n");   // def.Layer
        fprintf(fp_in,"2\n*D0\n"); // def.Block *D0

        // 1-Text
        GR_gxt_dxfout (1, memspc011, dim1->txt);
        fprintf(fp_in,"1\n%s\n",memspc011);

      } else {
        // fprintf(fp_in,"0\nLEADER\n");
        dxfw_hd_POLYLINE (fp_in); // wr header of POLYLINE
      }



      if(dim1->dtyp == 0) {     // 0=Linearmasz; a1 => ..
        fprintf(fp_in," 70\n%d\n",128); // 70 = typ Dim; = 128+0=hor
        dxfw_fl_out (50, dim1->a1, fp_in);  // 50 = angle
        dxfw_fl_out (53, dim1->a1, fp_in);  // 53 = angle (f CatV5 u Rhino)
        a1 = UT_RADIANS(dim1->a1);
        // p1 => 4
        pt1 = UT3D_pt_pt2 (&dim1->p1);
        dxfw_point (4, &pt1, fp_in);
        // p2 => 3
        pt1 = UT3D_pt_pt2 (&dim1->p2);
        dxfw_point (3, &pt1, fp_in);
        // 10: der Punkt p3 auf Hilfslinie 1
        UT2D_vc_angr (&vc21, a1);
        UT2D_vc_perpvc (&vc21, &vc21);
        UT2D_pt_projptptvc (&pt21,&dim1->p3, &dim1->p1,&vc21);
          // GR_Disp_pt2 (&pt21, SYM_STAR_S, 2);
        pt1 = UT3D_pt_pt2 (&pt21);
        dxfw_point (0, &pt1, fp_in);
        // 11: Punkt p3
        pt1 = UT3D_pt_pt2 (&dim1->p3);
        dxfw_point (1, &pt1, fp_in);


      //----------------------------------------------------------------
      } else if(dim1->dtyp == 1) {     // 1=Durchmesser
        fprintf(fp_in," 70\n%d\n",131); // 70 = typ Dim; = 128+3=Dmr
        dxfw_fl_out (53, dim1->a1, fp_in);  // 53 = angle (f CatV5 u Rhino)
          // 0 = opposit pt am circ
          UT2D_pt_opp2pt (&pt21, &dim1->p1, &dim1->p2);
          pt1 = UT3D_pt_pt2 (&pt21);
          dxfw_point (0, &pt1, fp_in);
          // 1 = Textpt
          pt1 = UT3D_pt_pt2 (&dim1->p3);
          dxfw_point (1, &pt1, fp_in);
          // 5 = pt am Circ
          pt1 = UT3D_pt_pt2 (&dim1->p2);
          dxfw_point (5, &pt1, fp_in);



      //----------------------------------------------------------------
      } else if(dim1->dtyp == 2) {     // 2=Radius
        fprintf(fp_in," 70\n%d\n",132); // 70 = typ Dim; = 128+4=Rad.
        dxfw_fl_out (53, dim1->a1, fp_in);  // 53 = angle (f CatV5 u Rhino)
          // 0 = CirCen
          pt1 = UT3D_pt_pt2 (&dim1->p1);
          dxfw_point (0, &pt1, fp_in);
          // 1 = Textpt
          pt1 = UT3D_pt_pt2 (&dim1->p3);
          dxfw_point (1, &pt1, fp_in);
          // 5 = pt am Circ
          pt1 = UT3D_pt_pt2 (&dim1->p2);
          dxfw_point (5, &pt1, fp_in);



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
        dxfw_point (3, &pt1, fp_in);
        // 4=p2=ExtL2
        pt1 = UT3D_pt_pt2 (&dim1->p2);
        dxfw_point (4, &pt1, fp_in);
        // 5=IntersectPt ExtL1-ExtL2
        pt1 = UT3D_pt_pt2 (&pt21);
        dxfw_point (5, &pt1, fp_in);
        // 0=Circ Intersect auf ExtL2
        UT2D_pt_traptvclen (&pt22,&pt21,&vc22,d1);
          GR_Disp_pt2 (&pt22, SYM_STAR_S, 3);
        pt1 = UT3D_pt_pt2 (&pt22);
        dxfw_point (0, &pt1, fp_in);
        // 1=TextPt
        pt1 = UT3D_pt_pt2 (&dim1->p3);
        dxfw_point (1, &pt1, fp_in);
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
        dxfw_point (3, &pt1, fp_in);
        pt1 = UT3D_pt_pt2 (&dim1->p1);
        dxfw_point (4, &pt1, fp_in);
        // ExtL2 = 5,0   (0=P2)
        UT2D_pt_traptvclen (&pt22,&dim1->p2,&vc22,-1.);
          // GR_Disp_pt2 (&pt22, SYM_STAR_S, 3);
        pt1 = UT3D_pt_pt2 (&pt22);
        dxfw_point (5, &pt1, fp_in);
        pt1 = UT3D_pt_pt2 (&dim1->p2);
        dxfw_point (0, &pt1, fp_in);
        // 6 = Circ Intersect auf ExtL2
        UT2D_pt_traptvclen (&pt22,&pt2c,&vc22,d1);
          // GR_Disp_pt2 (&pt22, SYM_STAR_S, 3);
        pt1 = UT3D_pt_pt2 (&pt22);
        dxfw_point (6, &pt1, fp_in);
        // 1 = p3
        pt1 = UT3D_pt_pt2 (&dim1->p3);
        dxfw_point (1, &pt1, fp_in);



      //----------------------------------------------------------------
      } else if(dim1->dtyp == 21) {     // 21=Leader
/* Ordinate wird scheinbar nicht unterstuertzt!
        fprintf(fp_in," 70\n%d\n",134); // 70 = typ Dim; = 128+6=Ordinate
        pt1 = UT3D_pt_pt2 (&dim1->p1);
        dxfw_point (3, &pt1, fp_in);
        pt1 = UT3D_pt_pt2 (&dim1->p2);
        dxfw_point (4, &pt1, fp_in);
        // pt1 = UT3D_pt_pt2 (&dim1->p2);
        // dxfw_point (4, &pt1, fp_in);
        // dxfw_point (0, &UT2D_PT_NUL, fp_in);
*/
/*
        // LEADER + TEXT
        if(dim1->p3.x != UT_DB_LEER) 
             fprintf(fp_in," 76\n3\n");  // Anzahl Punkte
        else fprintf(fp_in," 76\n2\n");
        pt1 = UT3D_pt_pt2 (&dim1->p1);
        dxfw_point (0, &pt1, fp_in);
        pt1 = UT3D_pt_pt2 (&dim1->p2);
        dxfw_point (0, &pt1, fp_in);
        if(dim1->p3.x != UT_DB_LEER) {
          pt1 = UT3D_pt_pt2 (&dim1->p3);
          dxfw_point (0, &pt1, fp_in);
        }
*/
        // VERTEXE zu POLYLINE
        dxfw_VERTEX2 (32, &dim1->p1, fp_in);
        dxfw_VERTEX2 (32, &dim1->p2, fp_in);
        if(dim1->p3.x != UT_DB_LEER) {
          pt1 = UT3D_pt_pt2 (&dim1->p3);
          dxfw_VERTEX2 (32, &dim1->p3, fp_in);
        } else {
          pt1 = UT3D_pt_pt2 (&dim1->p2);
        }
        fprintf(fp_in,"0\nSEQEND\n");

        // Text hier extra raus
        fprintf(fp_in,"0\nTEXT\n");
        fprintf(fp_in,"8\n0\n"); // def.Layer
        dxfw_fl_out (50, dim1->a1, fp_in); // 50-Textdirection
        dxfw_fl_out (40, AP_txdimsiz, fp_in); // 40-TextSize
        dxfw_point (0, &pt1, fp_in);
        GR_gxt_dxfout (0, memspc011, dim1->txt);
        fprintf(fp_in,"1\n%s\n",memspc011);

      }
      break;


    //=========================================================
    case Typ_CVCCV:
      return dxfw_ccv (ox1, fp_in);


    //=========================================================
    default:
      // printf("  dxfw skip %d\n",el->typ);
      return -1;

  }


  // printf(" ex dxfw_rec\n");

  return 0;
}


//================================================================
  int dxfw_obj2poly (ObjGX *ox1, FILE *fp_in) {
//================================================================
 
  int       irc, pNr;
  Point     *pTab;
  
  pTab = (Point*)memspc201; 
  pNr = sizeof(memspc201) / sizeof(Point);


  irc = UT3D_npt_ox (&pNr, pTab, ox1, UT_DISP_cv);
  if(irc < 0) return irc;

  return dxfw_POLYLINE (pNr, 32, pTab, fp_in);

}


//================================================================
  int dxfw_ELLIPSE (CurvElli *cv1, FILE *fp_in) {
//================================================================
// see ../formate/dxf/doc/DXF2000_ellipse.txt
      
  double     d1;
      
  printf("dxfw_ELLIPSE \n");
  UT3D_stru_dump (Typ_CVELL, cv1, "cv1:");


  fprintf(fp_in,"0\nELLIPSE\n");

  // 10,20,30 = centerpoint
  dxfw_point (0, &cv1->pc, fp_in);

  // 11,21,31 = Endpoint of major axis, relative to the center.
  dxfw_fl_out (11, cv1->va.dx, fp_in);
  dxfw_fl_out (21, cv1->va.dy, fp_in);
  dxfw_fl_out (31, cv1->va.dz, fp_in);

  // 210,220,230 = Extrusion direction
  dxfw_vector (&cv1->vz, fp_in);

  // 40 = Ratio of minor axis to major axis
  d1 = UT3D_len_vc(&cv1->vb) / UT3D_len_vc(&cv1->va);
  dxfw_fl_out (40, d1, fp_in);

  // 41 = Start parameter (this value is 0.0 for a full ellipse)
  // 42 = End parameter (this value is 2pi for a full ellipse)
  if(UT3D_comp2pt(&cv1->p1, &cv1->p2, UT_TOL_pt) != 0) {
    dxfw_fl_out (41, 0., fp_in);
    dxfw_fl_out (42, RAD_360, fp_in);

  } else {
    // parametric-Angle <- pt
    d1 = UT3D_angr_elpt (&cv1->p1, &cv1->pc, &cv1->va, &cv1->vb);
    dxfw_fl_out (41, d1, fp_in);
    d1 = UT3D_angr_elpt (&cv1->p2, &cv1->pc, &cv1->va, &cv1->vb);
    dxfw_fl_out (42, d1, fp_in);

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
  // UT3D_stru_dump (Typ_CVBSP, cv1, "cv1:");

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
  for(i1=0; i1<i2; ++i1) dxfw_fl_out (40, cv1->kvTab[i1], fp_in);

  // export points  10,20,30
  i2 = cv1->ptNr;
  for(i1=0; i1<i2; ++i1) dxfw_point (0, &cv1->cpTab[i1], fp_in);

  return 0;

}
 

//================================================================
  int dxfw_VERTEX (int typ, Point *pti, FILE *fp_in) {
//================================================================
// typ: 8=Spline, 32=Polyline


  fprintf(fp_in,"0\nVERTEX\n");
  fprintf(fp_in,"8\n0\n");            // Layer
  dxfw_point (0, pti, fp_in);
  fprintf(fp_in,"70\n%d\n",typ);         // TypVertex;

  return 0;

}


//================================================================
  int dxfw_VERTEX2 (int typ, Point2 *pti, FILE *fp_in) {
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
  dxfw_point2 (0, pti, fp_in);
  fprintf(fp_in,"70\n%d\n",typ);         // TypVertex;

  return 0;

}

//================================================================
  int dxfw_hd_POLYLINE (FILE *fp_in) {
//================================================================
/* wr header of POLYLINE
 0 POLYLINE
   70 bit(1)   closed polygon or polygonMesh - closed in M
      bit(8)   3D-Polygon
      bit(16)  PolygonMesh; details in groupCode 75
      bit(32)  PolygonMesh - closed in N
      bit(64)  PolyfaceMesh (PFACE); details via 71,72.
*/

        fprintf(fp_in,"0\nPOLYLINE\n");
        fprintf(fp_in,"8\n0\n");    // def.Layer
        fprintf(fp_in,"66\n1\n");   // indicates that VERT's will follow
        dxfw_point (0, (Point*)&UT3D_PT_NUL, fp_in);  // Nullpunkt
        fprintf(fp_in,"70\n8\n");   // bit(8)   3D-Polygon

  return 0;

}


//================================================================
  int dxfw_POLYLINE (int pNr, int typ, Point *pTab, FILE *fp_in) {
//================================================================
// typ: 8=Spline, 32=Polyline

  int   i1;


  dxfw_hd_POLYLINE (fp_in); // wr header of POLYLINE


  for(i1=0;i1<pNr; ++i1) {
    // printf(" dxfw-p[%d]=%f %f %f\n",i1,pTab[i1].x,pTab[i1].y,pTab[i1].z);
    dxfw_VERTEX (typ, &pTab[i1], fp_in);
  }

  fprintf(fp_in,"0\nSEQEND\n");  // end of VERT's after 66/1

  return 0;

}


//=====================================================================
  int dxfw_fl_out (int recID, double fVal, FILE *fp_in) {
//=====================================================================
// einen Record mit double ausgeben

  char   cbuf[32];


  cbuf[0] = '\0';
  UTX_add_fl_u(cbuf, fVal);

  fprintf(fp_in,"%d\n%s\n",recID, cbuf);

  return 0;

}


//=====================================================================
  int dxfw_point2 (int pnum, Point2 *point, FILE *fp_in) {
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

  dxfw_fl_out (10+pnum, point->x, fp_in);
  dxfw_fl_out (20+pnum, point->y, fp_in);


  return 1;
}



//=====================================================================
  int dxfw_point (int pnum, Point *point, FILE *fp_in) {
//=====================================================================
/*
 3D-Punktkoord. ausgeben.
 pnum=0: erster Punkt  (10, 20, 30);
 pnum=1: zweiter Punkt (11, 21, 31)
usw.
*/

/*
  fprintf(fp_in,"%d\n%f\n%d\n%f\n%d\n%f\n",
    10+pnum, point->x,
    20+pnum, point->y,
    30+pnum, point->z);
*/

  dxfw_fl_out (10+pnum, point->x, fp_in);
  dxfw_fl_out (20+pnum, point->y, fp_in);
  dxfw_fl_out (30+pnum, point->z, fp_in);


  return 1;
}


//=====================================================================
  int dxfw_vector (Vector* vc1, FILE *fp_in)
//=====================================================================

{
  fprintf(fp_in,"%d\n%f\n%d\n%f\n%d\n%f\n",
    210, vc1->dx,
    220, vc1->dy,
    230, vc1->dz);

  return 1;
}




//===========================================================================
  void dxfw_load_mat (Mat_4x3 m1, Vector* vz) {
//===========================================================================
/* die Mat für ein  DXF-ECS bestimmen. Input nur der Vektor (eines Kreises).

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

}


//================================================================
  int dxfw_ccv (ObjGX *ox1, FILE *fp_in) {
//================================================================
// export CCV as polygon
// TODO: export CCV as analytic obj's with group ?
 
  int       irc, pNr;
  Point     *pTab;
  
  pTab = (Point*)memspc201; 
  pNr = sizeof(memspc201) / sizeof(Point);


  irc = UT3D_npt_ox (&pNr, pTab, ox1, UT_DISP_cv);
  if(irc < 0) return irc;

  return dxfw_POLYLINE (pNr, 32, pTab, fp_in);
    
}
    

/*
//=====================================================================
  int dxfw_head (ObjG *el, FILE *fp_in)
//=============

//  Headerrecord eines Elementes schreiben ( 0 / POINT oder 0 / LINE ..)




{
  static int dxf_TypTab[] = {Typ_PT,Typ_LN,Typ_CI,Typ_CI,-99};
  static char *dxf_NamTab[] = {"POINT","LINE","CIRCLE","ARC"};
  int i1;

  //------------------------------------------------------------------
  //TX_Print("ObjG %d",act_typ);

  for(i1=0; dxf_TypTab[i1] != -99; i1++) {
    if(el->typ == dxf_TypTab[i1])
      fprintf(fp_in,"0\n%s\n",dxf_NamTab[i1]);
  }

  i1 = 0;  //i1 = el->layer;  // die Layernummer
  fprintf(fp_in,"8\n%d\n",i1);

  return 1;

}
*/

/* *********************** end of dxfw.c ********************************* */
