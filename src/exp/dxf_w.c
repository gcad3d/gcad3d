// SVG export.                                    RF 2010-09-22
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


List_functions_end:
=====================================================

\endcode *//*----------------------------------------


make -f xa_dxf_w.mak

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



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef _MSC_VER
#define _CRT_SECURE_NO_DEPRECATE
__declspec(dllexport) int DXFW__ (char*);
#define extern __declspec(dllimport)
#endif


#include "../ut/ut_geo.h"
#include "../ut/ut_txt.h"
#include "../ut/ut_TX.h"
#include "../ut/ut_os.h"               // OS_get_bas_dir ..
#include "../gr/ut_DL.h"                  // DL_GetAtt
#include "../gr/ut_GL.h"                  // GL_GetCen
#include "../gr/ut_gr.h"                  // GTX_chh_
#include "../gr/ut_UI.h"               // SYM_SQUARE ..

#include "../db/ut_DB.h"                  // DB_GetObjGX

#include "../xa/xa_mem.h"                 // memspc501
#include "../xa/xa.h"                     // AP_STAT


//===========================================================================
// EXTERNALS:
// from ../xa/xa.c:

extern double     AP_txsiz;       // Notes-Defaultsize
extern double     AP_txdimsiz;    // Dimensions-Text-size
extern AP_STAT    AP_stat;


//===========================================================================
// LOCALS:


static int       dxf_version;     // 0=R12; 1=2000







//===========================================================================
  int DXFW__ (char* fnam)  {
//===========================================================================
/// export as DXF
/// Input:
///   AP_stat.subtyp  0 = DXF-R12
///                   1 = DXF-2000

  int       oNr, mode, dxfVersion;
  ObjDB     *oTab;


  printf("DXFW__ 18:55 |%s|\n",fnam);

// - Liste auszugebender objekte (typ+dbi)
//   - wenn grp aktiv: grp
//   - else all obj's
// - export all objs of list


  // get nr of objs of group
  oNr = Grp_get__ (&oTab);
  if(oNr > 0) {
    mode = 0;
    goto L_exp;
  }
  


  //----------------------------------------------------------------
  L_all:  // group is empty; add all objs to group
    mode = 2;
    Grp_add_all1 ();



  //----------------------------------------------------------------
  L_exp:
      Grp_dump ();


  // get dxf-version
  dxfVersion = AP_stat.subtyp;


  // export all obj's in group.
  DXFW_init__ (fnam, dxfVersion);


  // empty group
  if(mode == 2) Grp_init ();

  return 0;


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
  int DXFW_CI (Circ *ci1, FILE *fp_in) {
//=====================================================================


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
      dxfw_load_mat (m1, &ci1->vz);  // eine TrMat nach DXF-Konvention generieren
      UT3D_m3_invm3 (im1, m1);      // RücktransformationsMat. generieren
      UT3D_pt_traptm3 (&pt1, im1, &ci1->pc);
      //TX_Print(" pt1=%f,%f,%f",pt1.x,pt1.y,pt1.z);


      //Ausgabe Vollkreis
      L_CI_out:
      dxfw_point (0, &pt1, fp_in);                  // Mittelpunkt als 10,20,30
      fprintf(fp_in,"40\n%f\n",fabs(ci1->rad));      // Radius 
      if(zparl==OFF) dxfw_vector (&ci1->vz, fp_in);
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
  int DXFW_point (int pnum, Point *point, FILE *fp_in) {
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
  int DXFW_vector (Vector* vc1, FILE *fp_in)
//=====================================================================

{
  fprintf(fp_in,"%d\n%f\n%d\n%f\n%d\n%f\n",
    210, vc1->dx,
    220, vc1->dy,
    230, vc1->dz);

  return 1;
}


//===========================================================================
  int dxfw_load_mat (Mat_4x3 m1, Vector* vz) {
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


//=====================================================================
  int DXFW_ox (ObjGX *ox1, long TrInd, int typ, long dbi,
               FILE *fp_o1, FILE *fp_o2) {
//=====================================================================
// dxfw_rec

  int           i1, zparl, ptNr, pNr;
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


  printf("DXFW_ox typ=%d tr=%ld typ=%d dbi=%ld\n",ox1->typ,TrInd,typ,dbi);


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
      fprintf(fp_o1,"0\nPOINT\n");
      fprintf(fp_o1,"8\n0\n"); // def.Layer
      pa = ox1->data;
      DXFW_point (0, &pa[0], fp_o1);
      break;


    //=========================================================
    case Typ_LN:
      fprintf(fp_o1,"0\nLINE\n");
      fprintf(fp_o1,"8\n0\n"); // def.Layer
      ln1 = ox1->data;
      DXFW_point (0, &ln1->p1, fp_o1);
      DXFW_point (1, &ln1->p2, fp_o1);
      break;


    //=========================================================
    case Typ_CI:
      DXFW_CI (ox1->data, fp_o1);
      break;


    //=========================================================
    case Typ_CVPOL:
      dxfw_POLYLINE (((CurvPoly*)ox1->data)->ptNr, 32,
                     ((CurvPoly*)ox1->data)->cpTab, fp_o1);
      break;


    //=========================================================
    case Typ_CVBSP:
      if(dxf_version == 0) {    // 0=R12
        dxfw_obj2poly (ox1, fp_o1);
      } else {  // 1=2000
        dxfw_SPLINE (ox1->data, fp_o1);
      }
      break;


    //=========================================================
    case Typ_CVELL:
      if(dxf_version == 0) {    // 0=R12
        dxfw_obj2poly (ox1, fp_o1);
      } else {  // 1=2000
        dxfw_ELLIPSE (ox1->data, fp_o1);
      }
      break;


    //=========================================================
    case Typ_CVCLOT:
      dxfw_obj2poly (ox1, fp_o1);
      // pTab = (Point*)memspc201; 
      // pNr = sizeof(memspc201) / sizeof(Point);
      // // elli -> polygon -> mem?
      // UT3D_npt_ox (&pNr, pTab, ox1, UT_DISP_cv);
      // dxfw_POLYLINE (pNr, 32, pTab, fp_o1);
      break;


    //=========================================================
    case Typ_CVPOL2:
      dxfw_hd_POLYLINE (fp_o1); // wr header of POLYLINE


      // p2Tab = (Point2Tab*)ox1->data;
      // p2a   = (Point2*)p2Tab->ptTab;
      p2a   = (Point2*)ox1->data;
      for(i1=0;i1<ox1->siz; ++i1) {
        dxfw_VERTEX2 (32, &p2a[i1], fp_o1);
      }

      fprintf(fp_o1,"0\nSEQEND\n");
      break;


    //=========================================================
    case Typ_GTXT:
      fprintf(fp_o1,"0\nTEXT\n");
      fprintf(fp_o1,"8\n0\n"); // def.Layer

      tx1 = (GText*)ox1->data;

      // posi
      dxfw_point (0, &tx1->pt, fp_o1);

      // 7=Textstyle "normal#0.300000#0.900000" od cursive#0.500000#3.000000

      // 40-Textsize (characterhoehe)
      // Notes haben size -1!
      d1 = tx1->size;
      if(d1 < 0) d1 = 1.;;
      // d1 *= DXF_fakt_txtSiz;
      dxfw_fl_out (40, d1, fp_o1);

      // 50-Textdirection
      dxfw_fl_out (50, tx1->dir, fp_o1);

      // 1-Text
      GR_gxt_dxfout (0, memspc011, tx1->txt);
      fprintf(fp_o1,"1\n%s\n",memspc011);

      // 39 = Thickness; 1 oder 2
      i1 = 1;
      if(tx1->size > 1.) i1 = 2;
      fprintf(fp_o1,"39\n%d\n",i1);

      // 62 = colornum = 0
      fprintf(fp_o1,"62\n0\n");

      break;



    //=========================================================
    case Typ_Dimen:
      return DXFW_DIM (ox1, fp_o1);


    //=========================================================
    case Typ_CVCCV:
      return dxfw_ccv (ox1, fp_o1);


    //=========================================================
    default:
      printf("  dxfw skip %d\n",typ);
      return -1;

  }







/*
  //  Bestandteile von CCV's (und getrimmten Flaechen ...) ausgeben
  irc = IGE_w_subObjs (&el, fp_o1, fp_o2);
  if (irc < 0) return irc;

  IG_mode  = 0;             // 0=normales Obj; 1=SubObj (von CCV ..)


  // ein Obj ausgeben (TrMat, D-Zeilen, P-Zeile)
  irc = IGE_w_obj (&el, apt_typ, apt_ind, fp_o1, fp_o2);
  if (irc < 0) return irc;
*/

  return 0;

}


//=============================================================================
  int DXFW_Model (int modNr, FILE *fp1, FILE *fp2) {
//=============================================================================
// AP_ExportIges_Model

  long     gr_ind;
  long     l1, apt_ind;
  int      i1, irc, tra_ind, tra_act, apt_typ,  anz_obj=0,
           anz_pt=0, anz_ln=0, anz_ac=0, anz_cv=0;
  Point    pt1;
  Line     ln1;
  Circ     ci1;
  ObjG     o1;
  ObjGX    ox1, *op1;
  Plane    pl1;
  Mat_4x3  m1;
  DL_Att   dla;



  printf("DXFW_Model %d\n",modNr);


  //----------------------------------------------------------------
  apt_typ = Typ_PT;

  for(apt_ind=1; apt_ind<APT_PT_SIZ; ++apt_ind) {
    if(DB_QueryDef(apt_typ, apt_ind) < 0) continue;
    ox1 = DB_GetObjGX (apt_typ, apt_ind);
    if(ox1.typ == Typ_Error) continue;

    DXFW_ox (&ox1, tra_ind, apt_typ, apt_ind, fp1, fp2);
    ++anz_obj;
  }


  //-----------------------------------
  apt_typ = Typ_LN;

  for(apt_ind=1; apt_ind<APT_LN_SIZ; ++apt_ind) {
    if(DB_QueryDef(apt_typ, apt_ind) < 0) continue;
    ox1 = DB_GetObjGX (apt_typ, apt_ind);
    if(ox1.typ == Typ_Error) continue;

    DXFW_ox (&ox1, tra_ind, apt_typ, apt_ind, fp1, fp2);
    ++anz_obj;
  }


  //-----------------------------------
  apt_typ = Typ_CI;

  for(apt_ind=1; apt_ind<APT_CI_SIZ; ++apt_ind) {
    if(DB_QueryDef(apt_typ, apt_ind) < 0) continue;
    ox1 = DB_GetObjGX (apt_typ, apt_ind);
    if(ox1.typ == Typ_Error) continue;

    DXFW_ox (&ox1, tra_ind, apt_typ, apt_ind, fp1, fp2);
    ++anz_obj;
  }


  //-----------------------------------
  apt_typ = Typ_CV;

  for(apt_ind=0; apt_ind<APT_CV_SIZ; ++apt_ind) {
    if(DB_QueryDef(apt_typ, apt_ind) < 0) continue;
    ox1 = DB_GetObjGX (apt_typ, apt_ind);
    if(ox1.typ == Typ_Error) continue;

    DXFW_ox (&ox1, tra_ind, apt_typ, apt_ind, fp1, fp2);
    ++anz_obj;
  }


  //-----------------------------------
  apt_typ = Typ_GTXT;

  for(apt_ind=0; apt_ind<APT_TX_SIZ; ++apt_ind) {
    if(DB_QueryDef(apt_typ, apt_ind) < 0) continue;
    ox1 = DB_GetObjGX (apt_typ, apt_ind);
    if(ox1.typ == Typ_Error) continue;

    DXFW_ox (&ox1, tra_ind, apt_typ, apt_ind, fp1, fp2);
    ++anz_obj;
  }


  //-----------------------------------
  apt_typ = Typ_SUR;

  for(apt_ind=0; apt_ind<APT_SU_SIZ; ++apt_ind) {
    if(DB_QueryDef(apt_typ, apt_ind) < 0) continue;
    ox1 = DB_GetObjGX (apt_typ, apt_ind);
    if(ox1.typ == Typ_Error) continue;

    DXFW_ox (&ox1, tra_ind, apt_typ, apt_ind, fp1, fp2);
    ++anz_obj;
  }


  //-----------------------------------
  // SubfigInst; 
  apt_typ = Typ_Model;

  for(apt_ind=0; apt_ind<APT_MR_SIZ; ++apt_ind) {
    if(DB_QueryDef(apt_typ, apt_ind) < 0) continue;
    ox1 = DB_GetObjGX (apt_typ, apt_ind);
    if(ox1.typ == Typ_Error) continue;

    DXFW_ox (&ox1, tra_ind, apt_typ, apt_ind, fp1, fp2);
    ++anz_obj;
  }








  return anz_obj;

}


//================================================================
  int DXFW_init__ (char * fn, int dxfVersion) {
//================================================================
// Input:
//   dxfVersion    0=DxfR12  1=Dxf2000



  int      i1, anz_obj=0, ipos, modNr;
  FILE     *fp1, *fp2, *fpi;
  char     s1[256];


  printf("DXFW_init__ %d |%s|\n",dxfVersion,fn);


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

/*
    DB_Init (1);
    // RUN (abarbeiten)
    ED_Init ();
    if(ED_work_file (cbuf) < 0) {TX_Error("AP_ExportIges__: E003"); goto L_err;}
    ED_lnr_reset ();
    WC_Work__ (0, NULL);   // Init Levelcounter in WC_Work__
    ED_Run ();

    // work submodel
    modNr = DB_get_ModNr(&cbuf[ipos]);  // get ModelNr from Modelname
    if(modNr < 0) {TX_Error("AP_ExportIges__: E004"); goto L_err;}
    i1 = AP_ExportIges_Model (modNr, fp1, fp2);  NEW: DXFW_Model !
    anz_obj += i1;
*/

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


  i1 = DXFW_Model (-1, fp1, fp2);
  anz_obj += i1;


  fprintf(fp1,"0\nENDSEC\n");



  //----------------------------------------------------------------
  // join files

  fclose (fp1);





  //----------------------------------------------------------------
  // print statistics
  TX_Print("   Objekte exportiert:  %d",anz_obj);


  return 0;

}


// EOF
