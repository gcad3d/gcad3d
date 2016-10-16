// DXF read, write  dxfr_ dxfw_                          RF  2001-06-17
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
2016-03-11 INSERT 210,220,230=z-vector. RF.
2015-01-07 ACAD_PROXY_ENTITY. RF.
2013-05-13 SPLINE added. RF.
2004-11-12 DIMENSION, LWPOLYLINE zu. RF.
2003-11-06 ELLIPSE zu. RF.
2002-12-16 Neue Curve-Datenstruktur.RF.
2002-02-21 Portierung -> W32. RF.
2001-06-17 Portierung -> Linux. RF.

-----------------------------------------------------
*/
/*!
\file  ../exp/dxf_r.c
\brief read, write DXF-files 
\code
=====================================================
List_functions_start:

DXF_r__            mainEntry
dxfr_rec__         read next record
dxf_r_src_out      translate obj & export obj
dxfr_out_txt       create gcad-obj txt

dxf_ckFileFormat   .
dxfr_init          open, skip until ENTITIES-section
dxfr_head__        decode headersection
dxfr_block_find    find Block
dxfr_section_skip  skip unsupported sections
dxfr_block_skip    skip unsupported blocks
dxfr_rec_read      read next 2 lines
dxfr_tra_arc       transform circle
dxf_log            display Logmessages
dxf_ac_bulge_2pt   create 2D-cirle from bulge-value and 2 points

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



==============================================================================
Memory-Usage  memspc (../xa/xa_mem.h):

dxf-Read:

   memspc501   in via wrkSpc in dxfr_rec__; sollte fuer alle verwendet werden !!
   memspc011   convert DXF-text -> gcad-text
   memspc012   dxf_linbuf
   memspc201   dxf_pa
   memspc101   dxf_da
   memspc102   Transf.


                   

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
           42=bulge-value; 0=straightLine; 1=180-deg-circle; 1-maxVal=circ > 180
           70=1: closed. (last point is missing !)
  INSERT   10,20,30=Origin; 2=BlockNam; 41=X-Scale; 42=Y-Scale; 43=Z-Scale.
           50=RotAng; 70/71=column/row-count; 44/45=column/row-spacing.
           Blocks are in "SECTION BLOCKS"; starting with "BLOCK",
           EndOfBlock = ENDBLK
           66=AttribsFollowFlag(1=Yes; Atribs end with SEQEND).
           blocks  (mit ASHADE und AVE_RENDER, ... glass.dxf) dzt skippen.
  ATTRIB   ein Textattribut; im Anschluss an INSERT,
           Ende mit SEQEND oder einem neuen ATTRIB.
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
  9 / $ACADVER / 1 / AC1015  (AutoCAD 2002)


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
Views:

-------------
$VIEWCTR 10, 20 XY center of current view on screen
$VIEWDIR 10, 20, 30 Viewing direction (direction from target in WCS)
$VIEWSIZE 40 Height of view

-------------
  0
SECTION
  2
TABLES

  0
VPORT
  2
*ACTIVE


10,20-11,21 Lower-left & Upper-right corner of viewport
12,22       center in ucs
16,26,36    View direction from target point
17,27,37    View target point
40          View height
42 Lens length
43 Front clipping plane (offset from target point)
44 Back clipping plane (offset from target point)
51 View twist angle
71 View mode (see VIEWMODE system variable)
72 (circle zoom percent)





-------------------------------------------------------------------
*/
#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// NUR FUER DLL  (nicht im Batch):
// geht leider interaktiv ned mit ifdef; was anderes erfinden ?
// #ifdef GTK2
// die folgenden Funktionen exportieren (werden vom Main gerufen):
#ifdef _MSC_VER
__declspec(dllexport) int DXF_r__ (char*);
// nachfolgende externals werden aus dem Main-Exe imported:
#define extern __declspec(dllimport)
#endif
// #endif



// #include "../ut/ut_umem.h"            // UME_reserve
#include "../ut/ut_geo.h"
#include "../ut/ut_txt.h"
#include "../ut/ut_TX.h"
#include "../ut/ut_txTab.h"               // UtxTab
#include "../ut/ut_os.h"                  // OS_
#include "../ut/ut_cast.h"                // INT_PTR
#include "../ut/ut_ox_base.h"             // OGX_SET_INDEX

#include "../ut/func_types.h"              // SYM_SQUARE ..

#include "../db/ut_DB.h"               // DB_mdlNam_iBas

#include "../xa/xa.h"                 // ?
#include "../xa/xa_mem.h"             // memspc55 mem_cbuf1
#include "../xa/xa_sele.h"            // Typ_go*


// #include "../ut/ut_crv.h"


#include "../exp/dxf_r.h"


//================================================================
// EXTERNALS:

// from ../xa/xa.c
extern double     AP_txsiz;       // Notes-Defaultsize
extern double     AP_txdimsiz;    // Dimensions-Text-size

// from ../ci/NC_Main.c
extern double APT_ModSiz;


//================================================================
 





#define DXF_FuncEnd     1234
#define DXF_fakt_txtSiz   32.0       // Scalefaktor fuer TEXT
#define DXF_SIZ_TXBUF   10000        // size (chars) for Textbuffer


// 1000  CIRCLE
// 1001  LEADER
// 1002  LWPOLYLINE

static
int  dxf_TypTab[] ={Typ_PT,     Typ_PT,      Typ_LN,    1000,      Typ_CI,
                    Typ_CVPOL,  DXF_FuncEnd, Typ_LN,    Typ_Txt,   Typ_Group,
                    Typ_GTXT,   Typ_CVELL,   Typ_Dimen, Typ_SURCIR,1001,
                    Typ_Txt,    1002,        Typ_CVBSP, Typ_SURHAT,Typ_SOL,
                    Typ_G_Att,  Typ_apDat,   Typ_Error, Typ_Error, Typ_Error,
                    Typ_Error,  Typ_Error,   Typ_Error, Typ_Error, -1};
static
char *dxf_NamTab[]={"POINT",    "VERTEX",    "LINE",     "CIRCLE", "ARC",
                    "POLYLINE", "SEQEND",    "3DLINE",   "TEXT",   "INSERT",
                    "TEXT",     "ELLIPSE",   "DIMENSION","3DFACE", "LEADER",
                    "MTEXT",    "LWPOLYLINE","SPLINE",   "HATCH",  "SOLID",
                    "ATTRIB",   "ATTDEF",    "IMAGE",    "VIEWPORT","SPLINE",
                    "HATCH",    "SOLID",     "ATTDEF",   "3DSOLID",""};



static  int    dxf_siz_linbuf;
static  char   *dxf_linbuf, dxf_numbuf[64], BlockNam[80], *dxf_ptr;
static  int    dxf_rectyp;

static  long   dxf_LineNr;           // ZeilenNr Inputfile

static  int    dxf_GroupTyp = 0;     // 0=noGroup; 1=POLYLINE, 2=ATTRIB
static  int    dxF_VertAnz;          // Anz Vertices einer Polyline

static  Point  *dxf_pa;
static  int    dxf_paSiz, dxf_paNr;

static  double *dxf_da;
static  int    dxf_daSiz, dxf_daNr;

static  ObjGX  dxf_ox1;             // f.3DFACE

static  int    dxf_blk_lev, dxf_blk_tab[10];

static  int    dxf_iTra = 0;        // 0=no translation; 1=apply transf.

// values 
static  char   dxfr_s_1[256];      //  1       text
static  double dxfr_d_40;          // 40
static  Point  dxfr_pta_10[10];    // 10-39    points

// static  FILE *dxf_auxFilLun;

#define SPC_TRA_SIZ 500000
static Memspc  DXFR_SPC_TRA;


static  int    dxf_unsupp[8];
// counter unsupported objectTypes;
// 0=SPLINE; 1=HATCH; 2=SOLID; 3=ATTRIB; 4=ATTDEF 5=VIEWPORT 6=IMAGE
// 7=3DSOLID


static UtxTab_NEW (dxfr_blockTab);   // list of names of used blocks



//===========================================================================
  int DXF_r__ (char* fnam)  {
//===========================================================================
/// load DXF-file -> memory
/// All coordinates are translated !

// TrVec: vc1 (from $EXTMIN bis $EXTMAX)

// Memory-Usage:
// memspc51    ox1
// memspc55    namTab
// memspc101   translated objData
// memspc102   ox2
// memspc202   dxf_da
// memspc501   objData
// mem_cbuf1   act codeline

  int      i1, ism, irc, oSiz, iaErr[3], iaImp[8], iaAux[8], mr_ind;
  double   d1, d2, d3, min_dist, dTab[10];

  char     *pf, cbuf[256], *spc_tra, *cp1;
  Point    pt1;
  Vector   vc1;

  FILE     *fp = NULL, *fp1 = NULL;
  FILE     *dxf_auxFilLun;

  Memspc   wrkSpc;
  ObjGX    *ox1, *ox2;
  Plane    plOff;
  Mat_4x3  trOff;
  ModelRef *mr;


  for(i1=0; i1<3; ++i1) iaErr[i1] = 0;
  for(i1=0; i1<8; ++i1) iaImp[i1] = 0;


  // get net free modelIndex of AP_stru_2_txt
  mr_ind = DB_dbo_get_free (Typ_Model);
  if(mr_ind < 20) mr_ind = 20;

  //-----------------------------------------------------

  // printf("AP_ImportDxf %d |%s|%s|\n",mode,off,fnam);
  // printf("======================= \n");
  // printf("DXF_r__ |%s|\n",fnam);


  Mod_kill__ ();   // alle tmp/Model_* loeschen


  // // TEST-DEBUG-ON
  // AP_deb_stat (1);


  // Init Objektindexe
  ox1 = (ObjGX*)memspc51;
  AP_obj_2_txt (NULL, 0L, NULL, 0L);



  // pt -> Refsys
  UT3D_pl_XYZ (&plOff);              // load DefaultRefsys

  // UT3D_pt_txt (&plOff.po, off);      // set RefsysOrigin = inputPoint

  // TX_Print(" Origin: %f, %f, %f",plOff.po.x, plOff.po.y, plOff.po.z);
  if(UT3D_comp2pt(&plOff.po, &UT3D_PT_NUL, UT_TOL_min1)) {
    dxf_iTra = 0;
  } else {
    dxf_iTra = 1;
    UT3D_m3_loadpl (trOff, &plOff);
  }



  // check Lic
  // Testversion: Kein Check on ReadDXF.
  //if(ED_ckl() != 0) goto FERTIG;



  // Open for read
  if((fp=fopen(fnam,"r")) == NULL) {
    TX_Print("****** OPEN ERROR INPUT-FILE %s **********\n",fnam);
    return -1;
  } else {
    fp1=fopen(fnam,"r");
  }


  // Pfad weg.
  pf = strrchr(fnam, fnam_del);
  if(pf) {
    ++pf; // letzten \ weg
  } else {
   pf=fnam;
  }
  

  // Textbuffer 1 loeschen
  UTF_clear1();

  // init StringList for subModelnames
  UtxTab_init_spc (&dxfr_blockTab, memspc55, sizeof(memspc55));

  sprintf(mem_cbuf1,"# DXF-Import %s",pf);
  UTF_add1_line (mem_cbuf1);

  // Notes- und Dimensions-Text-Defaultsize setzen
  for(i1=0; i1<10; ++i1) dTab[i1] = 0;

  i1 = dxfr_init (dTab, fp, fp1);
  if(i1 != 0) {irc = -2; goto Fertig_1;}

  if(dTab[0] > 0.01) AP_txsiz    = dTab[0];
  if(dTab[1] > 0.01) AP_txdimsiz = dTab[1];

  GR_InitGFPar (AP_txdimsiz);


  // die DXF-Input-Boxgrenzen sind dTab[2-4] / dTab[5-7] ($EXTMIN bis $EXTMAX)
  // Abstaende vom 0-Punkt erechnen
  d1 = UT3D_len_2pt ((Point*)&dTab[2], (Point*)&UT3D_PT_NUL);
  d2 = UT3D_len_2pt ((Point*)&dTab[5], (Point*)&UT3D_PT_NUL);
  // printf(" d1=%f d2=%f d3=%f\n",d1,d2,APT_ModSiz);

  // d1 = DMIN (fabs(d1), fabs(d2));
  if((fabs(d1) > APT_ModSiz)||(fabs(d2) > APT_ModSiz)) {
    UT3D_pt_mid2pt (&pt1, (Point*)&dTab[2], (Point*)&dTab[5]);
    // UT3D_stru_dump (Typ_PT, &pt1, "imp-center");


    if(dxf_iTra == 0) {
      TX_Print("Model out of ModelSize - translated");
/*
      UT3D_pl_XYZ (&plOff);
      // UT3D_pt_txt (&plOff.po, off);
      plOff.po.x = -pt1.x;
      plOff.po.y = -pt1.y;
      plOff.po.z = -pt1.z;
      TX_Print(" Origin: %f, %f, %f",plOff.po.x, plOff.po.y, plOff.po.z);
      UT3D_m3_loadpl (trOff, &plOff);
*/
      UT3D_vc_pt (&vc1, &pt1);
      UT3D_vc_invert (&vc1, &vc1);
      // UT3D_stru_dump (Typ_VC, &vc1, "transl.Vec:");
      UTRA_def__ (1, Typ_VC, &vc1);
      // report the transl.Vec to the user
      sprintf(mem_cbuf1,"# DXF-IMPORT-TRANSLATION-VECTOR:");
      UTF_add1_line (mem_cbuf1);
      OGX_SET_OBJ (ox1, Typ_VC, Typ_VC, 1, &vc1); // ox1 < vc1
      dxf_r_src_out (ox1);
      dxf_iTra = 1;
    }

    // $EXTMIN 1.000000E+20         $EXTMAX -1.000000E+20
    if(UT3D_compvc0(&vc1, 1) == 1) {
      dxf_iTra = 0;
    }
  }


  // dxf_iTra = 0;  // TEST ONLY !


  // get spc for DXFR_SPC_TRA
  spc_tra = (char*) malloc (SPC_TRA_SIZ);
  if(!spc_tra) {
    TX_Print("****** DXF_r__ EOM %s ******",fnam);
    return -1;
  }



  //---------------------------------------------------------
  for(i1=0; i1<1000000; ++i1) {
      // printf("DXF_r__ ===== next: =====\n");

    UME_init (&wrkSpc, memspc501, sizeof(memspc501));
      // printf(" wrkSpc-free %d\n",UME_ck_free(&wrkSpc));
    UME_init (&DXFR_SPC_TRA, spc_tra, SPC_TRA_SIZ);
      // printf(" DXFR_SPC_TRA-free %d\n",UME_ck_free(&DXFR_SPC_TRA));

    // get next record (object) from dxf-file;
    irc = dxfr_rec__ (&ox1, fp, fp1, &wrkSpc);
      // printf(" irc=%d typ=%d form=%d\n",irc,ox1->typ,ox1->form);

    if(irc > 1) goto FERTIG;   // 2 = FERTIG
    if(irc > 0) continue;   // minor error
    if(irc < 0) goto FERTIG;   // -1: unrecov. Err
    if(ox1->typ == Typ_Error) continue;   // minor error
    if(ox1->typ == Typ_Done) continue;    // obj already exported

    if(ox1->typ == Typ_Model) {
      // subModel-call 
      dxfr_sm__ (&mr_ind, ox1->data);
      continue;
    }

/*
    // subModelCalls: save subModelname in StringList dxfr_blockTab
    if(ox1->typ == Typ_Model) {
      mr = ox1->data;
        // printf(" >>>>>>>>>>>>>>>>>> Model |%s|\n",mr->mnam);
      // get modelname from basic-model-nr
      UtxTab_add_uniq__ (&dxfr_blockTab, DB_mdlNam_iBas(mr->modNr));
      UtxTab_add_uniq__ (&dxfr_blockTab, BlockNam);
    }
*/
    // Objekt in Text umwandeln & via UTF_add1_line ausgeben
    irc = dxf_r_src_out (ox1);
    if(irc == -1) {iaErr[0] += 1; continue;}      // Error
    if(irc == -2) goto FERTIG;   // grober Fehler


    Next_Obj:
    continue;
  }







  //---------------------------------------------------------
  FERTIG:
  sprintf(mem_cbuf1,"### End DXF-Import");
  UTF_add1_line (mem_cbuf1);

  Fertig_1:


  // query DB-indices
  AP_stru_2_txt (NULL, 0L, iaImp, 1L);
    // printf(" iaImp %d %d %d %d\n",iaImp[0],iaImp[1],iaImp[2],iaImp[3]);


  // write code -> file
  if(irc > -2) {
    // Textbuffer 1 in die Hauptdatei einfuegen
    // UTF_insert1(-1);

    sprintf(cbuf, "%s%cModel_",OS_get_tmp_dir(),fnam_del);
    UTF_file_Buf1__ (cbuf);

  }



  //================================================================
    // TESTBLOCK
    printf(" nr subModels = %d\n",dxfr_blockTab.iNr);
    // END TESTBLOCK


  //================================================================
  // output subModels
  ism = -1;

  L_nxt_sm:
  ism += 1;
  if(ism >= dxfr_blockTab.iNr) goto L_exit;

  pf = UtxTab__ (ism, &dxfr_blockTab);     // get string (subModelName)
    printf(" nxt sm %d |%s|\n",ism,pf);

  // reset DB
  WC_Init_all (0);
  GA_hide__ (-1, 0L, 0);               // reset HideList
  AP_obj_2_txt (NULL, 0L, NULL, 0L);    // reset objIndices

  // init dxf (find start of block <pf>)
  dxfr_block_find (fp, fp1, pf);

  // start 
  UTF_clear1();  // Textbuffer 1 loeschen

  sprintf(mem_cbuf1,"# DXF-Import BLOCK %s",pf);
  UTF_add1_line (mem_cbuf1);
    // printf (" add1_line |%s|\n",mem_cbuf1);


  // loop tru block
  for(i1=0; i1<250000; ++i1) {
    UME_init (&wrkSpc, memspc501, sizeof(memspc501));
    UME_init (&DXFR_SPC_TRA, spc_tra, SPC_TRA_SIZ);
    irc = dxfr_rec__ (&ox1, fp, fp1, &wrkSpc);
    if(irc > 1) goto L_sm_done;           // 2 = FERTIG
    if(irc > 0) continue;
    if(ox1->typ == Typ_Done) continue;    // already exported
    if(ox1->typ == Typ_Error) continue;   // minor error

    if(ox1->typ == Typ_Model) {
      // subModel-call 
      dxfr_sm__ (&mr_ind, ox1->data);
      continue;
    }

/*
    // subModelCalls: save subModelname in StringList dxfr_blockTab
    if(ox1->typ == Typ_Model) {
      mr = ox1->data;
      // get modelname from basic-model-nr
      cp1 = DB_mdlNam_iBas(mr->modNr);
      UtxTab_add (&dxfr_blockTab, cp1);
        printf(" >>>>>>>>>>>>>>>>>> Model |%s|\n",cp1);
    }
*/

    // Objekt in Text umwandeln
    irc = AP_obj_2_txt (mem_cbuf1, mem_cbuf1_SIZ, ox1, -1L);
    if(irc == -1) {iaErr[0] += 1; continue;}      // Error
    if(irc == -2) goto L_sm_done;         // grober Fehler

  }



  // write code -> file
  L_sm_done:

  // write code -> file
  if(irc > -2) {
    // Textbuffer 1 in die Hauptdatei einfuegen
    // UTF_insert1(-1);

    sprintf(cbuf, "%s%cModel_%s",OS_get_tmp_dir(),fnam_del,pf);
    UTF_file_Buf1__ (cbuf);

  }

  // query DB-indices
  AP_stru_2_txt (NULL, 0L, iaAux, 1L);
  for(i1=0; i1<8; ++i1) iaImp[i1] += iaAux[i1];


  // UtxTab_nr    // get nr of strings UtxTab_nr
  goto L_nxt_sm;



  //================================================================
  L_exit:

  if(fp)fclose(fp);
  if(fp1)fclose(fp1);
    // printf("exit AP_ImportDxf \n");

  // report unsupported objects
  dxf_log();

  free (spc_tra);


  // // TEST-DEBUG-OFF
  // AP_deb_stat (0);


  // report Errors
  if(iaErr[0] > 0)
    TX_Print("*** %d objects with errors (not imported)",iaErr[0]);


  // report nr of imported objects
  sprintf(cbuf,
    "imported: %d points, %d lines, %d circles, %d curves, %d surfaces, %d notes, %d subModels",
               iaImp[6],  iaImp[5], iaImp[4],   iaImp[0],  iaImp[1],   iaImp[3],  iaImp[7]);
  TX_Print("%s",cbuf);

  return 0;


}


//================================================================
  int dxfr_sm__ (int *mr_ind, ModelRef *mdr) {
//================================================================
// cannot use AP_stru_2_txt -
// ModelBas does not exist;
// mdr->modNr = index into dxfr_blockTab

  char   *pmn;


  // UT3D_stru_dump (Typ_Model, mdr, "dxfr_sm__ - ");


  // get modelname
  pmn = UtxTab__ (mdr->modNr, &dxfr_blockTab);
    // printf(" pmn |%s|\n",pmn);

  sprintf (mem_cbuf1, "M%d = \"%s\"", *mr_ind, pmn);
  *mr_ind += 1;

// see also AP_stru_2_txt: (typ == Typ_Model)

    // add origin
    AP_obj_add_pt (mem_cbuf1, &mdr->po);
      // printf(" M+po: |%s|\n",ED_buf1);

    // scale; default = 1
    if(fabs(mdr->scl - 1.) > 0.1) {
      AP_obj_add_val (mem_cbuf1, mdr->scl);
    }

    // wenn vx != UT3D_VECTOR_X muss auch vz raus !
    // if((&mdr->vz == &UT3D_VECTOR_Z)&&(&mdr->vx == &UT3D_VECTOR_X)) goto L_fertig;
    AP_obj_add_vc (mem_cbuf1, &mdr->vz);
      // printf(" M+vz: |%s|\n",ED_buf1);

    // if(&mdr->vx == &UT3D_VECTOR_X) goto L_fertig;
    AP_obj_add_vc (mem_cbuf1, &mdr->vx);
      // printf(" ModRef = |%s|\n",ED_buf1);



  UTF_add1_line (mem_cbuf1);

  return 0;

}
 

//=======================================================================
  int dxf_ckFileFormat (char *fnam) {
//=======================================================================
// RC = 0: Datei ist ein DXF-File
// RC = 1: Datei ist kein DXF-File

// wenn die ersten 4 Zeilen sind:
// 0
// SECTION
// 2
// HEADER


  int   irc;
  FILE  *fpi;

  irc = 1;


  if((fpi = fopen (fnam, "r")) == NULL) {
    printf ("Fehler open Datei %s\n", fnam);
    goto L_go;
  }

  if(fgets(dxfr_s_1, 256, fpi) == NULL) goto L_clo;
  if(atoi(dxfr_s_1) != 0) goto L_clo;

  if(fgets(dxfr_s_1, 256, fpi) == NULL) goto L_clo;
  UTX_CleanCR(dxfr_s_1);
  if(strcmp(dxfr_s_1,"SECTION")) goto L_clo;

  if(fgets(dxfr_s_1, 256, fpi) == NULL) goto L_clo;
  if(atoi(dxfr_s_1) != 2) goto L_clo;

  if(fgets(dxfr_s_1, 256, fpi) == NULL) goto L_clo;
  UTX_CleanCR(dxfr_s_1);
  if(strcmp(dxfr_s_1,"HEADER")) goto L_clo;

  irc = 0;



  L_clo:
  fclose(fpi);


  L_go:
  // printf("ex dxf_ckFileFormat |%s| %d\n",fnam,irc);
  return irc;  // Datei ist ein DXF-File

}


//================================================================
  int dxfr_init (double *dTab, FILE *fp_in, FILE *fp1) {
//================================================================
//  load header-section; set to ENTITIES-SECTION




  /* Local variables */
  int i1;


  /* -------------------------------------------------------- */
  //TX_Print("dxfr_init");


  /* ---- SKIP ALL SECTIONS BIS ZUR ENTITIES - SECTION----------- 
  Da stehen jeweils 2 Zeilen drin; ein Int und ein Wert (Text od. Real od Int).
  fscanf uebersprigt leerzeilen - unbrauchbar.
  */


  // Init
  dxf_linbuf  = memspc012;
  dxf_siz_linbuf = sizeof(memspc012);
  dxf_LineNr  = 0;
  dxf_blk_lev = -1;

  for(i1=0; i1<8; ++i1) dxf_unsupp[i1] = 0;


  while (!feof (fp_in))  {

    // die naechsten 2 Zeilen lesen -
    if ((dxfr_rec_read (fp_in, fp1)) != 0) goto L99601;
    /* TX_Print(" =========== %s %s", dxf_numbuf, dxf_linbuf); */


    // suchen nur / 0 / SECTION /
    if ((dxf_rectyp == 0) && (!strncmp (&dxf_linbuf[0], "SECTION", 7)))    {
      // printf(" found section\n");


      // Nach SECTION kommt der SectionTyp
      if ((dxfr_rec_read (fp_in, fp1)) != 0) goto L99601;


      //--------------------------------------------------------------
      // Ist es Section / 2 / ENTITIES / - fertig.
      if (dxf_rectyp == 2) {

        if (!strncmp (dxf_linbuf, "HEADER", 6)) {
          // headersection: look for $TEXTSIZE $DIMASZ ..
          if(dxfr_head__ (dTab, fp_in, fp1) != 0) goto L99601;


        } else if (!strncmp (dxf_linbuf, "ENTITIES", 8)) {
          // printf(" found section ENTITIES\n");

          // nun noch den ersten Headerrecord einlesen
          if ((dxfr_rec_read (fp_in, fp1)) != 0) goto L99601;


          //TX_Print(" exit dxfr_init");
          return 0;
        }
      }
    }
  }


  

  /* ------- ERRORS ---------------------- */
  L99601:
  TX_Print("READ - ERROR dxfr_init");
  return -1;

}


//================================================================
  int dxfr_head__ (double *dTab, FILE *fp_in, FILE *fp1) {
//================================================================
// headersection: load for $TEXTSIZE $DIMASZ $EXTMIN $EXTMAX

  // printf("dxfr_head__ \n");

  L_next:
  if ((dxfr_rec_read (fp_in, fp1)) != 0) goto L99601;


  L_test:
  if(dxf_rectyp == 9) {
    if(!strncmp(dxf_linbuf,"$TEXTSIZE",9)) {
      if((dxfr_rec_read (fp_in, fp1)) != 0) goto L99601;
        dTab[0] = atof(dxf_linbuf);
        // printf(" TEXTSIZE=%f\n",&dTab[0]);

    } else if(!strncmp(dxf_linbuf,"$DIMASZ",7)) {
      if((dxfr_rec_read (fp_in, fp1)) != 0) goto L99601;
        dTab[1] = atof(dxf_linbuf);
        // printf(" DIMASZ=%f\n",&dTab[1]);

    } else if(!strncmp(dxf_linbuf,"$EXTMIN",7)) {  // 10,20,30 -> dTab[2,3,4]
      for(;;) {
        if((dxfr_rec_read (fp_in, fp1)) != 0) goto L99601;
        if(dxf_rectyp == 10) dTab[2] = atof(dxf_linbuf);
        else if(dxf_rectyp == 20) dTab[3] = atof(dxf_linbuf);
        else if(dxf_rectyp == 30) dTab[4] = atof(dxf_linbuf);
        else goto L_test;
      }

    } else if(!strncmp(dxf_linbuf,"$EXTMAX",7)) {  // 10,20,30 -> dTab[5,6,7]
      for(;;) {
        if((dxfr_rec_read (fp_in, fp1)) != 0) goto L99601;
        if(dxf_rectyp == 10) dTab[5] = atof(dxf_linbuf);
        else if(dxf_rectyp == 20) dTab[6] = atof(dxf_linbuf);
        else if(dxf_rectyp == 30) dTab[7] = atof(dxf_linbuf);
        else goto L_test;
      }
    }
  }


  if((dxf_rectyp == 0)&&(!strncmp(dxf_linbuf,"ENDSEC",7))) return 0;


  goto L_next;



  L99601:
  TX_Print("READ - ERROR dxfr_head__");
  return -1;


}


//=======================================================================
  int dxfr_block_find (FILE *fp_in, FILE *fp1, char *BlockNam) {
//=======================================================================
// search Block <BlockNam>
// How: proceed to SECTION BLOCKS; find "BLOCK, <BlockNam>;
// RetCod:
//   0         OK
//   -1        block not found

  int  rc;


  // printf("dxfr_block_find |%s|\n",BlockNam);


  rewind (fp_in);

/*
  if(dxf_blk_lev >= 10) {
    TX_Error("*** dxfr_block_find E001 **");
    return -1;
  } else {
    dxf_blk_lev  += 1;
  }
    printf(" start block lev=%d from lNr=%d\n",dxf_blk_lev,dxf_LineNr);
  dxf_blk_tab[dxf_blk_lev] = dxf_LineNr;
*/
  dxf_LineNr    = 0;


  // proceed to SECTION BLOCKS
  Find_Sec:
  rc = dxfr_rec_read (fp_in, fp1);
  if(rc < 0) goto L99601;

  if (dxf_rectyp != 0) goto Find_Sec;
  if (strcmp (dxf_linbuf, "SECTION")) goto Find_Sec;

  // next rec: secName
  rc = dxfr_rec_read (fp_in, fp1);
  if(rc < 0) goto L99601;
  if (strcmp (dxf_linbuf, "BLOCKS")) goto Find_Sec;





  // Read u analyze next Record (2 Lines)
  Find_Block:
  rc = dxfr_rec_read (fp_in, fp1);
  if(rc < 0) goto L99601;


  // suchen nur / 0 / BLOCK /
  if (dxf_rectyp != 0) goto Find_Block;
  if (strcmp (dxf_linbuf, "BLOCK")) goto Find_Block;



  // Read u analyze next Records to find Blocknam
  Find_Name:
  rc = dxfr_rec_read (fp_in, fp1);
  if(rc < 0) goto L99601;


  // suchen nur / 2 / blockname /
  if(dxf_rectyp != 2)  goto Find_Name;

  // // make safe subModelname
  UTX_CleanAN (dxf_linbuf);
  if(strcmp (dxf_linbuf, BlockNam)) goto Find_Block;


  // nun vor bis zum nächsten / 0 /
  Find_Rec:
  rc = dxfr_rec_read (fp_in, fp1);
  if(rc < 0) goto L99601;


  // suchen nur / 0 /
  if (dxf_rectyp != 0)  goto Find_Rec;
  //TX_Print(" found Rec /%s/",dxf_linbuf);


  
    // printf(" cont BLOCK |%s| LineNr=%d\n",BlockNam,dxf_LineNr);




  return 0;



  /* ------- ERRORS ---------------------- */
  L99601:
  TX_Error("dxfr_block_find: Block %s not found",BlockNam);
  return -1;



}


//=======================================================================
  int dxfr_section_skip (FILE *fp_in, FILE *fp1) {
//=======================================================================
// einen nicht unterstützten Block skippen; zB VIEWPORT;
// weiterlesen bis zum next Recordheader

  int  rc;

  /* Read u analyze next Record (2 Lines) */
  NoAmoi:
  rc = dxfr_rec_read (fp_in, fp1);
  if(rc < 0) goto L99601;


  if(dxf_rectyp != 0) goto NoAmoi;
  if(strcmp (dxf_linbuf, "ENDSEC")) goto NoAmoi;

  return 0;


  /* ------- ERRORS ---------------------- */
  L99601:
  TX_Print("**** - READ - ERROR in dxfr_section_skip");
  return -1;
}


//=======================================================================
  int dxfr_block_skip (FILE *fp_in, FILE *fp1) {
//=======================================================================
// einen nicht unterstützten Block skippen; zB VIEWPORT;
// weiterlesen bis zum next Recordheader

  int  rc;


  /* Read u analyze next Record (2 Lines) */
  NoAmoi:
  rc = dxfr_rec_read (fp_in, fp1);
  if(rc < 0) goto L99601;

  if (dxf_rectyp != 0) goto NoAmoi;

    // printf(" ex dxfr_block_skip Lnr=%d %d |%s|\n",
           // dxf_LineNr,dxf_rectyp,dxf_linbuf);


  return 0;



  /* ------- ERRORS ---------------------- */
  L99601:
  TX_Print("**** - READ - ERROR in dxfr_block_skip");
  return -1;
}





//=======================================================================
  int dxfr_rec_read (FILE *fp_in, FILE *fp1) {
//=======================================================================
// die naechsten 2 Zeilen lesen; Ausgabe von:
// dxf_rectyp INT  - der Typ
// dxf_linbuf CHR  - die zugehörige Datenzeile


  int  i1;

  // die next2 Lines
  if (fgets (dxf_numbuf, 60, fp_in) == NULL) goto L99601;
  if (fgets (dxf_linbuf, dxf_siz_linbuf, fp_in) == NULL) goto L99601;

  // if(strlen(dxf_linbuf) >= dxf_siz_linbuf - 10) {
   // printf("***** dxfr_rec_read E001 *****\n");
  // }

  dxf_rectyp = atoi(dxf_numbuf);

  UTX_CleanCR (dxf_linbuf);

  dxf_LineNr += 2;


    // printf(" ex dxfr_rec_read %d |%s| LineNr=%d\n",
           // dxf_rectyp,dxf_linbuf,dxf_LineNr);

  return 0;



  /* ------- ERRORS ---------------------- */
  L99601:
  TX_Error("**** - DXF-READ - Format-Error E001 ****");
  return -1;



}


//=======================================================================
  int dxf_log () {
//=======================================================================
// dxf_unsupp

  if(dxf_unsupp[0] > 0) {
  TX_Print("**** %d SPLINE - records skipped (not yet supported ..)",
         dxf_unsupp[0]);
  }

  if(dxf_unsupp[1] > 0) {
  TX_Print("**** %d HATCH - records skipped (not yet supported ..)",
         dxf_unsupp[1]);
  }

  if(dxf_unsupp[2] > 0) {
  TX_Print("**** %d SOLID - records skipped (not yet supported ..)",
         dxf_unsupp[2]);
  }

  if(dxf_unsupp[3] > 0) {
  TX_Print("**** %d ATTRIB - records skipped (not yet supported ..)",
         dxf_unsupp[3]);
  }

  if(dxf_unsupp[4] > 0) {
  TX_Print("**** %d ATTDEF - records skipped (not yet supported ..)",
         dxf_unsupp[4]);
  }

  if(dxf_unsupp[5] > 0) {
  TX_Print("**** %d VIEWPORT - records skipped (not yet supported ..)",
         dxf_unsupp[5]);
  }

  if(dxf_unsupp[6] > 0) {
  TX_Print("**** %d IMAGE - records skipped (not yet supported ..)",
         dxf_unsupp[6]);
  }

  if(dxf_unsupp[7] > 0) {
  TX_Print("**** %d 3DSOLID - records skipped (not yet supported ..)",
         dxf_unsupp[7]);
  }

  return 0;
}


//=======================================================================
  int dxfr_rec__ (ObjGX **el, FILE *fp_in, FILE *fp1, Memspc *wrkSpc)
//=======================================================================
// return next object as ObjGX**; read from file fp_in.
// Naechsten Datenrecord aus DXF-Datei einlesen
// Der Headerrecord (0/POINT oder 0/LINE) .. ist schon in dxf_rectyp/dxf_linbuf,
//   weil man ja nur nach dem Einlesen des naechsten Headerrecord den aktuellen
//   ausgeben kann.
// 
// Input:
//   dxf_rectyp/dxf_linbuf  global
// 
// Output:
//   Returncode
//     0 = OK
//    -1 = fatal error; stop.
//     1 = erraneous object, continue
//     2 = EOF
// 
// 
//     1 = EOF found
//     2 = ReadError
//     3 = Internal Error
//     4 = Unbekannter Recordtyp
// 
// 
// Memory-Usage:
// memspc501  objData   wrkSpc
// memspc011  convert DXF-text -> gcad-text
// memspc012  dxf_linbuf
// 



{
  static int    ii, ix, iy, iz, act_typ, mode3d;
  static int    i1, i2, tab70[10];
  // static double recx[10], recy[10], recz[10], recwin[10], a1, a2, a3;
  static double recwin[10], a1, a2, a3;
  static Vector vz;

  int           ptNr;
  int           flag1 = 0;   // POLYLINE: type
  double        d1, d2, d3, d4;
  Point2        p20, p21, p22, p23, p2c;
  Point         p30, pt1, *ptp;
  Line          *ln1;
  Circ          *ci1;
  CurvPoly      *cvPlg;
  CurvElli      *elli1;
  CurvBSpl      *cvbsp;
  Vector        vc1, vc2;
  Vector2       vc21, vc22;
  // CurvPoly      cvPlg;
  GText         *tx1;
  Mat_4x3       m1, m2;
  Dimen         *dim1;
  ObjGX         *ox1;
  ModelRef      *modr1;


  /* ---------------------------------------------------------- */

  // printf("==========dxfr_rec__ ==============\n");


  // clear buffers
  tab70[0]  = 0;
  recwin[0] = 0.;
  recwin[1] = 0.;
  dxfr_d_40    = 0.;
  dxfr_s_1[0] = '\0';
  ix = 0;
  iy = 0;
  iz = 0;

  for(i1=0; i1<7; ++i1) dxfr_pta_10[i1] = UT3D_PT_NUL;

  (*el)->typ = Typ_Error;

  dxf_GroupTyp = 0;
  mode3d       = OFF;


  //----------------------------------------------------------------
  L_DXF_NOAMOI:

  // printf("--------- dxfr_rec__ Lnr=%ld rectyp=%d |%s| GroupTyp=%d\n",
         // dxf_LineNr, dxf_rectyp, dxf_linbuf, dxf_GroupTyp);



  // Zu Beginn muss immer der neue Header in den Buffern sein !
  if (dxf_rectyp != 0) {
    TX_Print("**** Internal Error 1 dxfr_rec in Zeile %d ****",dxf_LineNr);
    return -1;
  }

  if (!strncmp (dxf_linbuf, "ENDBLK", 6)) {
    return 2;
/*
    dxf_LineNr = dxf_blk_tab[dxf_blk_lev];
    dxf_blk_lev -= 1;
      printf(" end block; new lev=%d new lNr=%d\n",dxf_blk_lev,dxf_LineNr);
    goto L_Skip_Block; // skip old INSERT-record
*/


  // Letzter record wurde mit dem ENDSEC schon ausgegeben
  } else if (!strncmp (dxf_linbuf, "ENDSEC", 6)) {
    return 2;

  } else if (!strncmp (dxf_linbuf, "EOF", 3)) {
    return 2;
  }



  // NEUEN RECTYP FESTSTELLEN
  i2 = UTX_cmp_word_wordtab (dxf_NamTab, dxf_linbuf);
    // printf(" i2=%d linbuf=|%s|\n",i2,dxf_linbuf);
  // unknown word: i1 = -1
  if(i2 < 0) {
    if(!strcmp(dxf_linbuf, "ACAD_PROXY_ENTITY"))  goto L_Skip_Block;
    goto L_readError0;
  }

  act_typ = dxf_TypTab[i2];
    // printf(" act_typ=%d\n",act_typ);
  if(act_typ > 0) goto L_InitRecord;



  //----------------------------------------------------------------
  // bekannte RECORDS die geskippt werden:
  if(!strcmp(dxf_linbuf, "SECTION")) goto L_Skip_Section;

  // dxf_unsupp: 0=SPLINE; 1=HATCH; 2=SOLID; 3=ATTRIB; 4=ATTDEF 5=VIEWPORT
  // if(!strcmp(dxf_linbuf, "SPLINE"))  {dxf_unsupp[0] += 1; goto L_Skip_Block;}
  if(!strcmp(dxf_linbuf, "HATCH"))   {dxf_unsupp[1] += 1; goto L_Skip_Block;}
  if(!strcmp(dxf_linbuf, "SOLID"))   {dxf_unsupp[2] += 1; goto L_Skip_Block;}
  // if(!strcmp(dxf_linbuf, "ATTRIB"))  {dxf_unsupp[3] += 1; goto L_Skip_Block;}
  if(!strcmp(dxf_linbuf, "ATTDEF"))  {dxf_unsupp[4] += 1; goto L_Skip_Block;}
  if(!strcmp(dxf_linbuf, "VIEWPORT")){dxf_unsupp[5] += 1; goto L_Skip_Block;}
  if(!strcmp(dxf_linbuf, "IMAGE"))   {dxf_unsupp[6] += 1; goto L_Skip_Block;}
  if(!strcmp(dxf_linbuf, "3DSOLID")) {dxf_unsupp[7] += 1; goto L_Skip_Block;}



  //----------------------------------------------------------------
  L_readError0:
  // TX_Print("**** Nicht unterstuetzter DXF-Datentyp %s in Zeile %d",
         // dxf_linbuf,dxf_LineNr);
  // printf("**** unsupported DXF-record |%s| in line %d\n",
  TX_Print("ReadError in line %d ..",dxf_LineNr);
  // this happens from a read-error;
  // read one line and try to restore
  L_readError1: // find next line with known word ..
  ++dxf_LineNr;
  if(fgets (dxf_linbuf, dxf_siz_linbuf, fp_in) == NULL) goto Err1;
  UTX_CleanCR (dxf_linbuf);
  i2 = UTX_cmp_word_wordtab (dxf_NamTab, dxf_linbuf);
  if(i2 < 0) goto L_readError1;
  dxfr_block_skip (fp_in, fp1);
  return 0;



  //----------------------------------------------------------------
  // skip this Block
  L_Skip_Block:
  i1 = dxfr_block_skip (fp_in, fp1);
  if(i1 < 0) return 4;
  goto L_DXF_NOAMOI;




  // skip this section (SECTION OBJECTS)
  L_Skip_Section:
  i1 = dxfr_section_skip (fp_in, fp1);
  if(i1 < 0) return 4;
  goto L_DXF_NOAMOI;



  //================================================================
  L_InitRecord:    // act_typ
  //---------------------------------------------------------------------
  if(act_typ == Typ_Group) {
    a1 = 1.0;

  //---------------------------------------------------------------------
  } else if(act_typ == Typ_CI) {
      mode3d = OFF;
      vz = UT3D_VECTOR_Z;


  //---------------------------------------------------------------------
  } else if(act_typ == Typ_CVBSP) {  // SPLINE
      // printf(" dxf-init-spline ..\n");
    // space for knotvalues
    dxf_da = (double*) memspc101;
    dxf_daSiz = sizeof(memspc101) / sizeof(double);
    dxf_daNr = 0;
    // space for controlpoints
    dxf_pa = (Point*) memspc201;
    dxf_paSiz = sizeof(memspc201) / sizeof(Point);
    dxf_paNr = 0;


  //---------------------------------------------------------------------
  } else if(act_typ == 1002) {  // LWPOLYLINE
    dxf_paSiz = UME_ck_free (wrkSpc) / sizeof(Point);
    dxf_pa = UME_reserve (wrkSpc, dxf_paSiz * sizeof(Point));  // points ..
    for(i1=0; i1<dxf_paSiz; ++i1) dxf_pa[i1].z = 0.;
    dxf_da = (double*) memspc201;
    dxf_daSiz = sizeof(memspc201) / sizeof(double);
      // printf(" dxf_paSiz=%d dxf_daSiz=%d\n",dxf_paSiz,dxf_daSiz);
    ii = -1;
    ptNr = 1;    // unused ..
      // printf(" start LWPOLYLINE siz=%d ii=%d\n",dxf_paSiz,ii);
  }



  //---------------------------------------------------------------------
  // now act_typ is the active RecordTyp (from dxf_NamTab);
  // eg Typ_CVPOL for "POLYLINE".
  L_Start:
    // printf(" L_Start:---------\n");
    // printf(" dxf_GroupTyp=%d dxf_paNr=%d dxF_VertAnz=%d\n",
           // dxf_GroupTyp,dxf_paNr,dxF_VertAnz);



  // Read u analyze next Record (2 Lines)
  // liefert dxf_rectyp (Zahl) und dxf_linbuf (String) und dxf_LineNr
  if ((dxfr_rec_read (fp_in, fp1)) != 0) goto Err1;
    // printf(" n. dxfr_rec_read typ=%d |%s| LineNr=%ld\n",
           // dxf_rectyp,dxf_linbuf,dxf_LineNr);




  /* ==== 1 = Text ============================= */
  if (dxf_rectyp == 1) {
    strcpy(dxfr_s_1, dxf_linbuf);
    // printf(" Text  |%s|\n",dxfr_s_1);
    goto L_Start;



  /* ==== 2 = Blocknames ============================= */
  } else if (dxf_rectyp == 2) {
    strcpy(BlockNam, dxf_linbuf);
    //TX_Print("  Blockname  |%s|;",BlockNam);
    goto L_Start;


  // ==== 3 3 (dimension style name) unused ..
  } else if (dxf_rectyp == 3) {
    goto L_Start;




  /* ==== 5 = unbekannt (mit LINES) ============================= */
  } else if (dxf_rectyp == 5) {
    //TX_Print("  Typ 5  |%s|;",dxf_linbuf);
    goto L_Start;



  /* ==== 6 = LINETYP ============================= */
  } else if (dxf_rectyp == 6) {
    //TX_Print("  Lintyp %d",dxf_rectyp);
    goto L_Start;



  // ==== 7 = text_style =============================
  // normal#0.400000#1.200000
  } else if (dxf_rectyp == 7) {
    //TX_Print("  Lintyp %d",dxf_rectyp);
    goto L_Start;



  /* ==== 8 = LAYER ============================= */
  } else if (dxf_rectyp == 8) {
    //TX_Print("  Layer  %d",dxf_rectyp);
    //el->layer = atoi(dxf_linbuf);
    goto L_Start;




  /* ==== 62 = FARBE ============================= */
  } else if (dxf_rectyp == 62) {
    //TX_Print("  Colour %d",dxf_rectyp);
    goto L_Start;




  /* ==== 66 = ?? ============================= */
  } else if (dxf_rectyp == 66) {
    //TX_Print("  ?? %d |%s|",dxf_rectyp,dxf_linbuf);
    goto L_Start;





  /* ==== 10-19 = X-KOORDINATE ======================== */
  } else if ((dxf_rectyp > 9) && (dxf_rectyp < 20)) {
    i2 = dxf_rectyp - 10;
    if(act_typ == 1001) {  // alle LEADERpunkte 10,20,30
      i2 = ix;
      if(i2 > 6) goto L_Start;
      ++ix;


    } else if(act_typ == 1002) {  // LWPOLYLINE
      ++ii;
      if(ii >= dxf_paSiz) {printf("**** overflow dxf_pa\n"); goto L_err_eom;}
      if(ii >= dxf_daSiz) {printf("**** overflow dxf_da\n"); goto L_err_eom;}
      dxf_pa[ii].x = atof(dxf_linbuf);
      dxf_da[ii] = 0.;  // bulge
      // printf(" p[%d].x=%f ii=%d\n",ii,dxf_pa[ii].x,ii);
      goto L_Start;


    } else if(act_typ == Typ_CVBSP) {  // SPLINE
        // printf(" dxf-spline-point-10\n");
      dxf_pa[dxf_paNr].x = atof(dxf_linbuf);
      ++dxf_paNr;
      goto L_Start;


    }

    // recx[i2]=atof(dxf_linbuf);
    dxfr_pta_10[i2].x = atof(dxf_linbuf);
    // printf("XKoord. %d = %f\n",i2,recx[i2]);
    goto L_Start;




  /* ==== 20-29 = Y-KOORDINATE ======================== */
  } else if ((dxf_rectyp > 19) && (dxf_rectyp < 30)) {
    i2 = dxf_rectyp - 20;

    if(act_typ == 1001) {
      i2 = iy;
      if(i2 > 6) goto L_Start;
      ++iy;


    } else if(act_typ == 1002) {  // LWPOLYLINE
      dxf_pa[ii].y = atof(dxf_linbuf);
      goto L_Start;


    } else if(act_typ == Typ_CVBSP) {  // SPLINE
        // printf(" dxf-spline-point-20\n");
      ii = dxf_paNr - 1;
      dxf_pa[ii].y = atof(dxf_linbuf);
      goto L_Start;

    }

    // recy[i2]=atof(dxf_linbuf);
    dxfr_pta_10[i2].y = atof(dxf_linbuf);
    // printf("YKoord. %d = %f\n",i2,recy[i2]);
    goto L_Start;




  /* ==== 30-35 = Z-KOORDINATE ======================== */
  } else if ((dxf_rectyp > 29) && (dxf_rectyp < 38)) {
    i2 = dxf_rectyp - 30;

    if(act_typ == 1001) {
      i2 = iz;
      if(i2 > 6) goto L_Start;
      ++iz;


    } else if(act_typ == 1002) {  // LWPOLYLINE
      dxf_pa[ii].z = atof(dxf_linbuf);
      goto L_Start;


    } else if(act_typ == Typ_CVBSP) {  // SPLINE
        // printf(" dxf-spline-point-30\n");
      ii = dxf_paNr - 1;
      dxf_pa[ii].z = atof(dxf_linbuf);
      goto L_Start;

    }

    // recz[i2]=atof(dxf_linbuf);
    dxfr_pta_10[i2].z = atof(dxf_linbuf);
    // printf("ZKoord. %d = %f\n",i2,recz[i2]);
    goto L_Start;



  // ==== 38 = DXF_elevation ==========================
  } else if (dxf_rectyp == 38) {
    //TX_Print("  ?? %d |%s|",dxf_rectyp,dxf_linbuf);
    goto L_Start;



  // ==== 39 = DXF_thickness ==========================
  } else if (dxf_rectyp == 39) {
    //TX_Print("  ?? %d |%s|",dxf_rectyp,dxf_linbuf);
    goto L_Start;



  // ==== 40 = RADIUS / Texthoehe ================== 
  } else if (dxf_rectyp == 40) {
    dxfr_d_40 = atof(dxf_linbuf);
    if(act_typ == Typ_CVBSP) {
        // printf(" spline-val-40\n");
      dxf_da[dxf_daNr] = dxfr_d_40;
      ++dxf_daNr;
    }
    // printf("Radius %f\n",dxfr_d_40);
    goto L_Start;



  // ==== 41 ============================
  } else if (dxf_rectyp == 41) {
    a1=atof(dxf_linbuf);
    //TX_Print("?? %d %f",dxf_rectyp, a1);
    goto L_Start;



  // ==== 42 ============================
  } else if (dxf_rectyp == 42) {
    a2 = atof(dxf_linbuf);
      // printf(" 42 %f typ=%d ii=%d\n",a2,act_typ,ii);
    if(act_typ == 1002) {  // LWPOLYLINE: bulge
      dxf_da[ii] = a2;
    }
    //TX_Print("?? %d %f",dxf_rectyp, a2);
    goto L_Start;



  // ==== 43 ======================== ??
  } else if (dxf_rectyp == 43) {
    a3 = atof(dxf_linbuf);
    goto L_Start;


  // ==== 44 ========================
  } else if (dxf_rectyp == 44) { goto L_Start;
  } else if (dxf_rectyp == 47) { goto L_Start;    // 47(HATCH)



  //===== 48 / skip
  } else if (dxf_rectyp == 48) {
    goto L_Start;



  // ==== 50-59 = WINKEL ============================  (degree)
  } else if ((dxf_rectyp > 49) && (dxf_rectyp < 60)) {
    i2 = dxf_rectyp - 50;
    recwin[i2]=atof(dxf_linbuf);
    // printf(" winkel[%d] = %f\n",i2,recwin[i2]);
    goto L_Start;



  // ==== 67 ====================================== 
  } else if (dxf_rectyp == 67) {
    goto L_Start;



  // ==== 70-79 => tab70[] ======================== 
  } else if ((dxf_rectyp > 69) && (dxf_rectyp < 80)) {
    i1 = dxf_rectyp - 70;  // 0-9
    i2 = atoi(dxf_linbuf);
    tab70[i1] = i2;
      // printf(" Code70+%d = %d act_typ=%d\n",i1,i2,act_typ);
    if(act_typ == Typ_CVPOL) {
      if(dxf_rectyp == 70) flag1 = i2;   // save 70 of POLYLINE
    }
    goto L_Start;



  // ==== 90 ========================
  } else if (dxf_rectyp == 90) {
    // Anzahl Punkte bei LWPOLYLINE
    ptNr = atoi(dxf_linbuf);
      // printf(" Code90-%d\n",ptNr);
    goto L_Start;



  // ==== 91-98 (HATCH) ======================== 
  } else if ((dxf_rectyp > 90) && (dxf_rectyp < 99)) {
    goto L_Start;


  // ==== 100 ========================
  } else if (dxf_rectyp == 100) {
    // Subtyp ??? unused; skip. Subclass marker "AcDbSpline"
      // printf(" Code100 |%s|\n",dxf_linbuf);
    goto L_Start;


  // ==== 102 ========================
  } else if (dxf_rectyp == 102) {
    // LINE, MTEXT
      // printf(" Code102 |%s|\n",dxf_linbuf);
    goto L_Start;



  /* ==== 210 ======================== */
  } else if (dxf_rectyp == 210) {
    mode3d = ON;
    vz.dx = atof(dxf_linbuf);
    //TX_Print(" vc.dx=%f  /%s/ ",vz.dx,dxf_linbuf);
    goto L_Start;



  /* ==== 220 ======================== */
  } else if (dxf_rectyp == 220) {
    vz.dy = atof(dxf_linbuf);
    //TX_Print(" vc.dy=%f  /%s/ ",vz.dy,dxf_linbuf);
    goto L_Start;



  /* ==== 230 ======================== */
  } else if (dxf_rectyp == 230) {
    vz.dz = atof(dxf_linbuf);
    //TX_Print(" vc.dz=%f  /%s/ ",vz.dz,dxf_linbuf);
    goto L_Start;



  // ==== 330 ========================
  } else if (dxf_rectyp == 330) {  // 3DFACE
        // printf(" Code330 |%s|\n",dxf_linbuf);
      goto L_Start;

  // ==== 370 ========================
  } else if (dxf_rectyp == 370) { goto L_Start;
  // ==== 1000 ========================
  } else if (dxf_rectyp == 1000) { goto L_Start;
  // ==== 1001 ========================
  } else if (dxf_rectyp == 1001) { goto L_Start;



  // ==== 1002 ========================
  } else if (dxf_rectyp == 1002) {
    // DimStlye; ein Block; beginnt mit 1002 "{" und endet mit 1002 "}"
    // unused; skip.
    if(!strcmp(dxf_linbuf, "{")) {
      L_1002_1:
        if ((dxfr_rec_read (fp_in, fp1)) != 0) goto Err1;
        if(dxf_rectyp != 1002) goto L_1002_1;
        // if(strcmp(dxf_linbuf, "}")) goto L_1002_1;
    }
    goto L_Start;


  } else if (dxf_rectyp == 1040) {         // TEXT
    goto L_Start;


  } else if (dxf_rectyp == 1070) {         // TEXT
    goto L_Start;


  } else if (dxf_rectyp == 1071) {
    goto L_Start;



  // ==== Unknown Recordnumber ======================
  } else if (dxf_rectyp != 0) {
    // printf("**** Unbekannte Recordnumber %d in dxfr_rec, Zeile %d\n",
              // dxf_rectyp,dxf_LineNr);
    // TX_Print("**** Unbekannte Recordnumber %d in dxfr_rec, Zeile %d",
              // dxf_rectyp,dxf_LineNr);
    printf("**** dxfr_rec: unsupp. code %d LineNr %ld Record %d\n",
              dxf_rectyp,dxf_LineNr,act_typ);
    goto L_Start;
  }








  //========= 0 = der naechste RecordHeader =======================
  // we got next RecordHeader; close active record act_typ
    // printf(" switch-act_typ %d\n",act_typ);


  switch (act_typ)  {



    case DXF_FuncEnd:   /*============== SEQEND ================*/
        // printf(" SEQEND in lNr=%ld GroupTyp=%d\n",dxf_LineNr,dxf_GroupTyp);
        // printf(" DXF_FuncEnd lNr=%ld dxf_GroupTyp=%d ptNr=%d\n",
               // dxf_LineNr,dxf_GroupTyp,dxF_VertAnz);

      if(dxf_GroupTyp == 0) return 1;   // nothing to do; eg SEQEND after INSERT

      if(dxF_VertAnz < 2) {
        if(dxf_GroupTyp != 2) {
          TX_Print("***** Error SEQEND E001-%ld",dxf_LineNr);
          return 1;
        }
      }


      //-------------------------------------------------
      if(dxf_GroupTyp == 1) {   // SEQEND of POLYLINE
        // Close Curve. if(flag1(bit1) == 1) then close ployLine.
          // printf(" cre polyline %d LineNr=%d\n",flag1,dxf_LineNr);
          // now we have first record after SEQEND!
        if((flag1 & 1) > 0) {
          // printf(" close polyline ..\n");
          memcpy(&dxf_pa[dxF_VertAnz], &dxf_pa[0], sizeof(Point));
          ++dxF_VertAnz;
        }

          // printf(" wrkSpc-free %d\n",UME_ck_free(&DXFR_SPC_TRA));
        cvPlg = UME_reserve (&DXFR_SPC_TRA, sizeof(CurvPoly));
        cvPlg->cpTab = dxf_pa;
        cvPlg->ptNr  = dxF_VertAnz;
        cvPlg->lvTab = NULL;
        cvPlg->v0    = 0.;
        cvPlg->v1    = 0.;
        cvPlg->dir   = 0;      // 0=fwd, 1=bwd.
        OGX_SET_OBJ (*el, Typ_CVPOL, Typ_CVPOL, 1, cvPlg);
          // UTO_dump__ (*el, "CVPOL-PT");
          // UTO_dump_s_ (*el, "CVPOL-PT");
        goto FERTIG;



      //-------------------------------------------------
      } else {

        TX_Print("**** Internal Error 2 dxfr_rec in Zeile %d ****",dxf_LineNr);
        goto FERTIG;
      }




    case Typ_Group:   /*========== INSERT ====================*/
      // printf("INSERT Block |%s| Zeile %ld\n",BlockNam,dxf_LineNr);

      // make safe subModelname
      UTX_CleanAN (BlockNam);
        // printf(" BlockNam corr |%s|\n",BlockNam);
        // printf(" scales: %f %f %f\n",a1,a2,a3);
        // printf(" angle: %f\n",recwin[0]);
        // printf(" count: %f %f %f\n",tab70[0],tab70[1]);


      // get pt1 = origin of submodel
      // UT3D_pt_3db (&pt1, recx[0], recy[0], recz[0]);
      pt1 = dxfr_pta_10[0];


      if(mode3d == ON) {
        // the Z-vector of the subModel is NOT parallel to parent-models Z-vector
        // get vc2 = the new Z-vector
        // get refSys m1 (use autocad-arbitrary-axis-algorithm)
        dxfr_load_mat (m1, &vz);
        // translate the origin into the new axisSystem
        UT3D_pt_traptm3 (&pt1, m1, &pt1);
        // rotate matrix if 50 = angle given
        UT3D_m3_get (&vc1, 0, m1);
        UT3D_m3_get (&vc2, 2, m1);
        if(UTP_comp_0(recwin[0])) {
          d1 = -UT_RADIANS(recwin[0]);
          UT3D_m3_inirot_angr (m2, NULL, &vc2, d1);
          UT3D_vc_travcm3 (&vc1, m2, &vc1);
        }

      } else {
        // the Z-vector of the subModel is parallel to parent-models Z-vector
        vc1 = UT3D_VECTOR_X;
        vc2 = UT3D_VECTOR_Z;
        // rotate X-vector from code 50                    2015-01-07
        if(!UTP_compdb0(recwin[0],UT_TOL_Ang1)) {
            // printf(" DXF-Model |%s| angr=%lf lNr=%ld\n",
                   // BlockNam,recwin[0],dxf_LineNr);
          d1 = -UT_RADIANS(recwin[0]);
          UT3D_vc_rotangr (&vc1, &vc1, &d1);
        }
      }

      // add <BlockNam> into list of used subModels
      i1 = UtxTab_add_uniq__ (&dxfr_blockTab, BlockNam);
        // printf(" INSERT |%s|\n",BlockNam);

      // create ModelRef-struct
      modr1 = UME_reserve (wrkSpc, sizeof(ModelRef));
 
      modr1->scl   = a1;               // 41
      modr1->po    = pt1;              // 10
      // modr1->po.y  = recy[0];
      // modr1->po.z  = recz[0];
      modr1->vx    = vc1;              // X-vector
      modr1->vz    = vc2;              // Z-vector
      // modr1->mnam  = BlockNam;
      modr1->modNr = i1;      // index in dxfr_blockTab
        // UT3D_stru_dump (Typ_Model, modr1, "Insert - ");

      (*el)->typ  = Typ_Model;
      (*el)->form = Typ_Model;
      (*el)->siz  = 1;
      (*el)->data = modr1;
        // UTO_dump__ (*el, "EL:");

      goto FERTIG;





    case Typ_CVPOL:   /*========= POLYLINE ===================*/
      dxf_GroupTyp = 1;    // 0=noGroup; 1=POLYLINE, 2=ATTRIB
        // printf(" start POLYLINE %ld\n",dxf_LineNr);

      // was mach ma mit dem ersten Punkt im POLY-Header ? Nix !
      // printf("poly-pt %f,%f,%f\n",recx[0],recy[0],recz[0]);

      // query max nr of points in &memSeg1
      dxf_paSiz = UME_ck_free (wrkSpc) / sizeof(Point);
      // reserve space for ipMax points in memSeg1
      dxf_pa = UME_reserve (wrkSpc, dxf_paSiz * sizeof(Point));

        // printf(" start POLYLINE maxPt=%d LineNr=%d\n",dxf_paSiz,dxf_LineNr);
        // now we have first VERTEX!

      dxF_VertAnz = 0;
      goto L_DXF_NOAMOI;






    case Typ_G_Att:   //========= ATTRIB ===================
        // printf(" ATTRIB lNr=%ld GroupTyp=%d\n",dxf_LineNr,dxf_GroupTyp);

      // return text
      dxfr_out_txt (el, wrkSpc);
      goto FERTIG;



    case 1002:   //======= LWPOLYLINE ===================
        // printf(" exit LWPOLYLINE ii=%d ptNr=%d\n",ii,ptNr);
        // if(ii < ptNr-1) goto L_Start;   // 2009-06-12 raus
        // printf("end LWPOLYLINE siz=%d ii=%d\n",dxf_paSiz,ii);
      ++ii;
      dxf_out_lwpoly (ii, tab70[0]);
      (*el)->typ  = Typ_Done;
      goto FERTIG;




    case Typ_PT:   /*========= VERTEX =====================*/
      // Typ_PT kommt bei Punkten und bei Polyline-Vertices
      if(dxf_GroupTyp == 1) {
        // Vertex in die CURVE-Datei ausgeben
          // printf(" add Vertex %d LineNr=%d\n",dxF_VertAnz,dxf_LineNr);

        if(dxF_VertAnz >= dxf_paSiz) {
          TX_Error("*** DXF-VERTEX-buffer too small ***)");

        } else {
          // dxf_pa[dxF_VertAnz].x = recx[0];
          // dxf_pa[dxF_VertAnz].y = recy[0];
          // dxf_pa[dxF_VertAnz].z = recz[0];
          dxf_pa[dxF_VertAnz] = dxfr_pta_10[0];
            // printf(" vertex[%d] %lf %lf %lf - ln %ld\n",dxF_VertAnz,
                   // recx[0],recy[0],recz[0],dxf_LineNr);
          ++dxF_VertAnz;
        }

        goto L_DXF_NOAMOI;



      } else {
        //TX_Print("Point %f %f %f",recx[0],recy[0],recz[0]);
        ptp    = UME_reserve (wrkSpc, sizeof(Point));
        // ptp->x = recx[0];
        // ptp->y = recy[0];
        // ptp->z = recz[0];
        *ptp = dxfr_pta_10[0];
        OGX_SET_OBJ (*el, Typ_PT, Typ_PT, 1, ptp);
        goto FERTIG;
      }






    case Typ_LN:   /*======================================*/
      //TX_Print("Line  %f %f %f",recx[0],recy[0],recz[0]);
      //TX_Print("      %f %f %f",recx[1],recy[1],recz[1]);

      // get line in memSpc
      ln1 = UME_reserve (wrkSpc, sizeof(Line));
      // ln1->p1.x=recx[0];
      // ln1->p1.y=recy[0];
      // ln1->p1.z=recz[0];
      ln1->p1 = dxfr_pta_10[0];

      // ln1->p2.x=recx[1];
      // ln1->p2.y=recy[1];
      // ln1->p2.z=recz[1];
      ln1->p2 = dxfr_pta_10[1];

      ln1->typ = 0;    // 2015-01-07

      OGX_SET_OBJ (*el, Typ_LN, Typ_LN, 1, ln1);

      goto FERTIG;






    case Typ_CI:   /*======================================*/
        // printf(" ARC  %f %f %f\n",recx[0],recy[0],recz[0]);
        // printf("  vz  %f,%f,%f\n",vz.dx,vz.dy,vz.dz);
        // printf("   r  %f\n",dxfr_d_40);
        // printf("   50=%f 51=%f\n",recwin[0],recwin[1]);

      ci1 = UME_reserve (wrkSpc, sizeof(Circ));

      if(mode3d == OFF) {

        // ci1->pc.x = recx[0];
        // ci1->pc.y = recy[0];
        // ci1->pc.z = recz[0];
        ci1->pc = dxfr_pta_10[0];

        p2c = UT2D_pt_pt3 (&ci1->pc);

        p20.x = ci1->pc.x + dxfr_d_40;
        p20.y = ci1->pc.y;

        UT2D_pt_rotptangr (&p21, &p2c, &p20, UT_RADIANS(recwin[0]));
        UT2D_pt_rotptangr (&p22, &p2c, &p20, UT_RADIANS(recwin[1]));
        ci1->p1 = UT3D_pt_pt2 (&p21);
        ci1->p2 = UT3D_pt_pt2 (&p22);

        // ci1->p1.z = recz[0];
        ci1->p1.z = dxfr_pta_10[0].z;
        // ci1->p2.z = recz[0];
        ci1->p2.z = dxfr_pta_10[0].z;

        ci1->vz   = UT3D_VECTOR_Z;

        ci1->rad  = dxfr_d_40;


      } else {
        // lokales Achsensystem nach Autocad-System berechnen:
        dxfr_load_mat (m1, &vz);     // vz=210,220,230

        // Arc umrechnen
        // pt1.x = recx[0];    // pt1=10,11,12
        // pt1.y = recy[0];
        // pt1.z = recz[0];
        dxfr_tra_arc (ci1, m1, &dxfr_pta_10[0], recwin[0],recwin[1],dxfr_d_40);
      }

      // Oeffnungswinkel ..
/*
      if(UTP_comp2db(recwin[0],recwin[1],UT_TOL_PAR) == 0)
        ci1->ango = UT3D_angr_ci(ci1);
      else ci1->ango = RAD_360;
*/

      ci1->ango = UT_RADIANS (recwin[1] - recwin[0]);


      OGX_SET_OBJ (*el, Typ_CI, Typ_CI, 1, ci1);

      goto FERTIG;




    //================================================================
    case 1000:
      // printf(" CIRCLE  %f %f %f\n",recx[0],recy[0],recz[0]);
      // printf("  vz  %f,%f,%f\n",vz.dx,vz.dy,vz.dz);
      // printf("   r  %f\n",dxfr_d_40);

      ci1 = UME_reserve (wrkSpc, sizeof(Circ));

      if(mode3d == OFF) {
        // ci1->pc.x = recx[0];
        // ci1->pc.y = recy[0];
        // ci1->pc.z = recz[0];
        ci1->pc = dxfr_pta_10[0];

        // ci1->p1.x = recx[0] + dxfr_d_40;
        // ci1->p1.y = recy[0];
        // ci1->p1.z = recz[0];
        ci1->p1 = dxfr_pta_10[0];
        ci1->p1.x += dxfr_d_40;

        ci1->vz   = UT3D_VECTOR_Z;
        ci1->p2 = ci1->p1;
        ci1->rad  = dxfr_d_40;


      } else {
        // lokales Achsensystem nach Autocad-System berechnen:
        dxfr_load_mat (m1, &vz);

        // Arc umrechnen
        // pt1.x = recx[0];
        // pt1.y = recy[0];
        // pt1.z = recz[0];
        dxfr_tra_arc (ci1, m1, &dxfr_pta_10[0], 0.0, 0.0, dxfr_d_40);
        act_typ = Typ_CI;  // Typ_CI waere falsch!
      }

      ci1->ango = RAD_360;

      OGX_SET_OBJ (*el, Typ_CI, Typ_CI, 1, ci1);

      goto FERTIG;



    //================================================================
    case Typ_CVBSP:   //========= SPLINE ====================
      // printf("SPLINE %f %f %f\n",recx[0],recy[0],recz[0]);

      cvbsp = UME_reserve (wrkSpc, sizeof(CurvBSpl));

      // 71 = degree
      // 72 = Number of knots
      // 73 = Number of control points
      cvbsp->ptNr = dxf_paNr;
      cvbsp->deg = tab70[1];
      cvbsp->v0 = dxf_da[0];
      cvbsp->v1 = dxf_da[dxf_daNr - 1];

      // 40 = Knot values
      cvbsp->kvTab = dxf_da;

      // 10,20,30 = controlpoints
      cvbsp->cpTab = dxf_pa;
        // UT3D_stru_dump (Typ_CVBSP, cvbsp, "");

      OGX_SET_OBJ (*el, Typ_CVBSP, Typ_CVBSP, 1, cvbsp);
      goto FERTIG;



    //================================================================
    case Typ_CVELL:   //========= ELLIPSE ====================
      // printf("ELLIPSE %f %f %f\n",recx[0],recy[0],recz[0]);

      // pt1.x = recx[0];
      // pt1.y = recy[0];
      // pt1.z = recz[0];
      pt1 = dxfr_pta_10[0];

      // vc1.dx = recx[1];
      // vc1.dy = recy[1];
      // vc1.dz = recz[1];
      vc1 = *((Vector*)&dxfr_pta_10[1]);
        // UT3D_stru_dump (Typ_VC, &vc1, "vc1");

      // vz = der z-vektor UT3D_VECTOR_Z
      UT3D_vc_perp2vc (&vc2, (Vector*)&UT3D_VECTOR_Z, &vc1);
        // UT3D_stru_dump (Typ_VC, &vc2, "vc2");

      // 40=dxfr_d_40=Hauptachse/Nebenachse
      d1 = UT3D_len_vc (&vc1) * dxfr_d_40;  // len. Nebenachse
      UT3D_vc_setLength (&vc2, &vc2, d1);

      // Ellipse=center,vecHauptachse,vecNebenachse,AngStart,angEnd,Richtg.
      // elli1 = pt1, vc1, vc2, a1, a2, CCW
      elli1 = UME_reserve (wrkSpc, sizeof(CurvElli));
      UT3D_el_pt2vc2a (elli1, &pt1, &vc1, &vc2, a1, a2, 1);

      OGX_SET_OBJ (*el, Typ_CVELL, Typ_CVELL, 1, elli1);

        // UT3D_stru_dump (Typ_CVELL, el, "");

      goto FERTIG;



    //================================================================
    case 1001:
      // printf("LEADER\n");
      dim1 = UME_reserve (wrkSpc, sizeof(Typ_Dimen));
      dim1->dtyp = 21;
      // UT2D_pt_2db (&dim1->p1, recx[0],recy[0]);
      dim1->p1 = UT2D_pt_pt3 (&dxfr_pta_10[0]);
      // UT2D_pt_2db (&dim1->p2, recx[1],recy[1]);
      dim1->p2 = UT2D_pt_pt3 (&dxfr_pta_10[1]);
      // ix = nr of coords in dxfr_pta_10;
      if(ix > 2) {
        // UT2D_pt_2db (&dim1->p3, recx[2],recy[2]);
        dim1->p3 = UT2D_pt_pt3 (&dxfr_pta_10[2]);
      } else {
        dim1->p3.x = UT_DB_LEER;
      }
      dim1->hd   = 1;
      dim1->txt  = NULL;
      OGX_SET_OBJ (*el, Typ_Dimen, Typ_Dimen, 1, dim1);
      goto FERTIG;



    //================================================================
    case Typ_Dimen:
      // printf("DIMENSION\n");
/*
      for(i1=0; i1<7; ++i1) {
        printf(" p%d = %f %f %f\n",i1,recx[i1],recy[i1],recz[i1]);
        UT2D_pt_2db (&p20, recx[i1],recy[i1]);
        GR_Disp_pt2 (&p20, SYM_STAR_S, 2);
        UT3D_pt_3db (&p30, recx[i1],recy[i1],recz[i1]);
        GR_Disp_txi (&p30, i1, 0);
      }
*/
      dim1 = UME_reserve (wrkSpc, sizeof(Typ_Dimen));

      // den text umwandeln ..
      dxfr_gxt (dxfr_s_1);
        // printf(" Text  |%s|\n",dxfr_s_1);
        dim1->txt  = dxfr_s_1;

      // dimtyp: lower 3 bits of 70
      i1 = tab70[0] & 7;
        // printf(" dimtyp=%d\n",i1);

      // 50=Winkel
      if(recwin[0] != UT_DB_LEER) dim1->a1 = recwin[0];
      else                        dim1->a1 = 0.;
      d1 = UT_RADIANS(dim1->a1);
        // printf(" a1=%f\n",dim1->a1);


      //----------------------------------------------------------------
      if(i1 < 2) {   // 0=Hor,Vert; 1=parallel
        dim1->dtyp = 0;
        // UT2D_pt_2db (&dim1->p1, recx[3],recy[3]);
        dim1->p1 = UT2D_pt_pt3 (&dxfr_pta_10[3]);
        // UT2D_pt_2db (&dim1->p2, recx[4],recy[4]);
        dim1->p2 = UT2D_pt_pt3 (&dxfr_pta_10[4]);
        // UT2D_pt_2db (&dim1->p3, recx[1],recy[1]);
        dim1->p3 = UT2D_pt_pt3 (&dxfr_pta_10[1]);
        dim1->hd   = 12;
        dim1->ld   = 11;

        if(i1 > 0) {      // 1=parallel
          d1 = UT2D_angr_ptpt (&dim1->p1, &dim1->p2);
          dim1->a1   = UT_DEGREES(d1);
        }
        if(dim1->a1 >= 179.9) dim1->a1 -= 180.;

        // wenn 11,21,31 == 0,0,0: den 10,20,30 nehmen
        if(UT2D_comp2pt(&dim1->p3,(Point2*)&UT2D_PT_NUL,UT_TOL_pt) == 1) {
          // Pt 0 auf ExtL1 u ExtL2 projizieren, Mitte nehmen ..
          // UT2D_pt_2db (&p21, recx[0],recy[0]);
          p21 =  UT2D_pt_pt3 (&dxfr_pta_10[0]);
            // UT3D_stru_dump (Typ_PT2, &p21, "~~~~~~~~~~~~~~~~~ dim-1");
          UT2D_vc_angr (&vc21, d1);
          UT2D_vc_perpvc (&vc21, &vc21);
          UT2D_pt_projptptvc (&p22, &p21, &dim1->p1, &vc21);
          UT2D_pt_projptptvc (&p23, &p21, &dim1->p2, &vc21);
            // UT3D_stru_dump (Typ_PT2, &p22, "         p22");
            // UT3D_stru_dump (Typ_PT2, &p23, "         p23");
          UT2D_pt_mid2pt (&dim1->p3, &p22, &p23);
        }


      //----------------------------------------------------------------
      } else if(i1 == 2) {    // WinkelMasz
        // Leaderlines: L1 = 13,23-14,24; L2 = 15,25-10,20;
        // Textpos: if(Code>128) p3=11,21 (user-defined);
        //          else p3=16,26 (defPosi)
        // im DXF scheinbar CW ! Siehe $ANGDIR
        // aus den ExtLines (3-4 und 5-0) werden die Winkel errechnet; das kann
        // nur gehen wenn die Winkel vom Schnittpunkt weg zeigen; ist aber nicht
        // immer der Fall, dann ist der Winkel 180 Grad falsch!
        dim1->dtyp = 3;
        // UT2D_pt_2db (&dim1->p1, recx[4],recy[4]);    // 14 24
        dim1->p1 = UT2D_pt_pt3 (&dxfr_pta_10[4]);
        // UT2D_pt_2db (&dim1->p2, recx[0],recy[0]);    // 10 20
        dim1->p2 = UT2D_pt_pt3 (&dxfr_pta_10[0]);
        // p3 = posi of text
        if(tab70[0] > 128) {                         // 70
          // UT2D_pt_2db (&dim1->p3, recx[1],recy[1]);
          dim1->p3 = UT2D_pt_pt3 (&dxfr_pta_10[1]);
        } else {
          // UT2D_pt_2db (&dim1->p3, recx[6],recy[6]);
          dim1->p3 = UT2D_pt_pt3 (&dxfr_pta_10[6]);
        }
            // UT3D_stru_dump (Typ_PT2, &dim1->p1, "~~~~~~~~~~~~~~~~~ dim2-4");
            // UT3D_stru_dump (Typ_PT2, &dim1->p2, "                  dim2-0");
            // UT3D_stru_dump (Typ_PT2, &dim1->p3, "                  dim2-1");

        // 3-4: extensionLine 1
        // UT2D_pt_2db (&p21, recx[3],recy[3]);
        p21 = UT2D_pt_pt3 (&dxfr_pta_10[3]);
        UT2D_vc_2pt (&vc21, &p21, &dim1->p1);
        // d1 = UT2D_angr_ptpt (&p21, &dim1->p1);

        // 5-0: extensionLine 2
        // UT2D_pt_2db (&p22, recx[5],recy[5]);
        p22 = UT2D_pt_pt3 (&dxfr_pta_10[5]);
        UT2D_vc_2pt (&vc22, &p22, &dim1->p2);
        // d2 = UT2D_angr_ptpt (&p22, &dim1->p2);

        // nun winkel a1 und a2 so rechnen, dass sie vom schnittpunkt nach
        // aussen zeigen. Bekannt sind: 2 Winkel und ein Punkt
        // in der Mitte des Kreisbogens (16,26).
        // Annahme erforderlich: Winkel kann 180 Grad nicht ueberschreiten !
        // UT2D_pt_2db (&p20, recx[6],recy[6]);
        p20 = UT2D_pt_pt3 (&dxfr_pta_10[6]);
            // UT3D_stru_dump (Typ_PT2, &p20, "16,26:");
        // get p2c=intersectionpoint
        UT2D_pt_int2pt2vc (&p2c, &p21, &vc21, &p22, &vc22);
        i1 = UT2D_sidPerp_ptvc (&p20, &p2c, &vc21);
        if(i1 < 0) UT2D_vc_invert (&vc21, &vc21);
        i1 = UT2D_sidPerp_ptvc (&p20, &p2c, &vc22);
        if(i1 < 0) UT2D_vc_invert (&vc22, &vc22);
        d1 = UT2D_angr_vc (&vc21);
        dim1->a1   = UT_DEGREES(d1);
        d2 = UT2D_angr_vc (&vc22);
        dim1->a2   = UT_DEGREES(d2);
          // printf(" a1=%f a2=%f\n",dim1->a1,dim1->a2);


        // nun muss p1-p2 noch CCW gemacht werden.
        d3 = UT2D_angr_2angr (d1, d2, 1);
          // printf(" ango=%f\n",d3);

        if(d3 > RAD_180) {
          MEM_swap_2db (&dim1->a1, &dim1->a2);
          MEM_swap__ (&dim1->p1, &dim1->p2, sizeof(Point2));
        }


        dim1->hd   = 12;
        dim1->ld   = 11;
/*
        // wenn 1 == 0,0,0
        if(UT2D_comp2pt(&dim1->p3,(Point2*)&UT2D_PT_NUL,UT_TOL_pt) == 1) {
          UT2D_pt_2db (&p20, recx[6],recy[6]);
            // UT3D_stru_dump (Typ_PT2, &p20, "                  dim2-6");
          // p2c = Schnittpunkt
          UT2D_pt_int4pt(&p2c,&d3,&d4,&dim1->p1,&p21,&dim1->p2,&p22);
          // Radius aus 6 berechnen
          d3 = UT2D_len_2pt (&p2c,&p20); // Radius
          //  den p1 mit Abst d3=Rad in die Mitte des Circ drehen => p3
          // d4=Oeffnungswinkel
          d4 = UT2D_angr_2angr (d1, d2, 1);
          // printf(" d1=%f d2=%f d4=%f\n",d1,d2,d4);
          d4 /= 2.;
          UT2D_pt_tra3ptlen(&p20,&p2c,&p21,&dim1->p1,d3);
          UT2D_pt_rotptangr(&dim1->p3,&p2c,&p20,d4);
        }
*/


      //----------------------------------------------------------------
      } else if(i1 == 3) {    // DurchmesserMasz
        dim1->dtyp = 1;
        // UT2D_pt_2db (&dim1->p1, recx[0],recy[0]); // gegenueber pick-point..
        dim1->p1 = UT2D_pt_pt3 (&dxfr_pta_10[0]);
        // UT2D_pt_2db (&dim1->p2, recx[5],recy[5]); // pick-point..
        dim1->p2 = UT2D_pt_pt3 (&dxfr_pta_10[5]);
        // UT2D_pt_2db (&dim1->p3, recx[1],recy[1]); // kann 0,0 sein!
        dim1->p3 = UT2D_pt_pt3 (&dxfr_pta_10[1]);
        d1 = UT2D_angr_ptpt (&dim1->p1, &dim1->p2);
        dim1->a1   = UT_DEGREES(d1);
        dim1->a2   = UT2D_len_2pt (&dim1->p1,&dim1->p2) / 2.; // Radius
        // wenn p3=NUL: um Radius raus.
        if(UT2D_comp2pt(&dim1->p3,(Point2*)&UT2D_PT_NUL,UT_TOL_pt) == 1) {
          UT2D_pt_tra2ptlen (&dim1->p3, &dim1->p2, &dim1->p1, -dim1->a2);
        }
        UT2D_pt_mid2pt (&p20, &dim1->p1, &dim1->p2);
        dim1->hd   = 12;


      //----------------------------------------------------------------
      } else if(i1 == 4) {    // RadiusMasz
        dim1->dtyp = 2;
        // UT2D_pt_2db (&dim1->p1, recx[0],recy[0]);  // center
        dim1->p1 = UT2D_pt_pt3 (&dxfr_pta_10[0]);
        // UT2D_pt_2db (&dim1->p2, recx[5],recy[5]);  // pick-point am circ
        dim1->p2 = UT2D_pt_pt3 (&dxfr_pta_10[5]);
        // UT2D_pt_2db (&dim1->p3, recx[1],recy[1]);  // kann 0,0 sein!
        dim1->p3 = UT2D_pt_pt3 (&dxfr_pta_10[1]);
        d1 = UT2D_angr_ptpt (&dim1->p1, &dim1->p2); // Angle
        dim1->a1   = UT_DEGREES(d1);
        dim1->a2   = UT2D_len_2pt (&dim1->p1,&dim1->p2); // Radius

        // Variante: 0=cen, 5=TextPt, 40=Radius
        // wenn p3=NUL: um Radius raus.
        if(fabs(dxfr_d_40) > 0.000001) {
          dim1->a2 = dxfr_d_40;
        }

        // kein p3 (1): p3=5; p2 errechnen
        if(UT2D_comp2pt(&dim1->p3,(Point2*)&UT2D_PT_NUL,UT_TOL_pt) == 1) {
          dim1->p3 = dim1->p2;
          UT2D_pt_tra2ptlen (&dim1->p2, &dim1->p1, &dim1->p3, dim1->a2);
        }
        dim1->hd   = 1;



      //----------------------------------------------------------------
      } else if(i1 == 5) {    // Angular 3-point
        dim1->dtyp = 3;
        // UT2D_pt_2db (&dim1->p1, recx[3],recy[3]); // 13,23,33
        dim1->p1 = UT2D_pt_pt3 (&dxfr_pta_10[3]);
        // UT2D_pt_2db (&dim1->p2, recx[4],recy[4]); // 14,24,34
        dim1->p2 = UT2D_pt_pt3 (&dxfr_pta_10[4]);
        // UT2D_pt_2db (&dim1->p3, recx[0],recy[0]); // 10,20,30
        dim1->p3 = UT2D_pt_pt3 (&dxfr_pta_10[0]);
        // 0-3 und 4-5 sind die Hilfslinien
        // UT2D_pt_2db (&p20, recx[5],recy[5]);  // 15,25,35
        p20 = UT2D_pt_pt3 (&dxfr_pta_10[5]);
        d1 = UT2D_angr_ptpt (&p20, &dim1->p1);
        dim1->a1   = UT_DEGREES(d1);
        d1 = UT2D_angr_ptpt (&p20, &dim1->p2);
        dim1->a2   = UT_DEGREES(d1);
        dim1->hd   = 12;
        dim1->ld   = 0;   // keine hilfslinien



      } else {
        TX_Error(" unsupported DIM typ=%d",tab70[0]);
        return 1;
      }

      OGX_SET_OBJ (*el, Typ_Dimen, Typ_Dimen, 1, dim1);
      goto FERTIG;




    //================================================================
    case Typ_SURCIR:
        // printf("3DFACE %ld\n",dxf_LineNr);

      dxf_pa = UME_reserve (wrkSpc, 4 * sizeof(Point));

      // die 3 od 4 Punkte speichern;
      for(i1=0; i1<4; ++i1) dxf_pa[i1] = dxfr_pta_10[i1];
        // dxf_pa[i1].x=recx[i1];
        // dxf_pa[i1].y=recy[i1];
        // dxf_pa[i1].z=recz[i1];
        // printf("3DFpt%d=%f,%f,%f\n",i1,dxf_pa[i1].x,dxf_pa[i1].y,dxf_pa[i1].z);
      // }
      dxF_VertAnz = 4;


      // sind Pt3 und Pt4 gleich: nur 3 Punkte.
      if(UT3D_comp2pt(&dxf_pa[2],&dxf_pa[3],UT_TOL_pt)) dxF_VertAnz = 3;


      act_typ  = Typ_SURCIR;

      ox1 = wrkSpc->next;
      UME_add (wrkSpc, sizeof(ObjGX));

      ox1->typ  = Typ_PT;
      ox1->form = Typ_PT;
      ox1->siz  = dxF_VertAnz;
      ox1->data = dxf_pa;

      (*el)->typ  = Typ_SURCIR;
      (*el)->form = Typ_ObjGX;
      (*el)->siz  = 1;
      (*el)->data = ox1;
      (*el)->dir  = 0;

      goto FERTIG;






    //================================================================
    case Typ_Txt:
      // printf("TEXT\n");

      tx1 = UME_reserve (wrkSpc, sizeof(GText));

      // tx1->pt.x=recx[0];   // Textpos
      // tx1->pt.y=recy[0];
      // tx1->pt.z=recz[0];
      tx1->pt = dxfr_pta_10[0];

      // 40-Textsize (characterhoehe)
      tx1->size=dxfr_d_40;  //  / DXF_fakt_txtSiz;

      // 50 = Winkel
      if(recwin[0] != UT_DB_LEER) tx1->dir = recwin[0];
      else tx1->dir = 0.;

      // 72, 73 = hor. vert. justification

      dxfr_gxt (dxfr_s_1);  // den text umwandeln ..
      tx1->txt = dxfr_s_1;

      OGX_SET_OBJ (*el, Typ_GTXT, Typ_GTXT, 1, tx1);

      goto FERTIG;




                                                            
/*
    //================================================================
    // Unknown Record
    default:
      TX_Print("**** Unbekannte DXF-Record %d |%s| Line=%d",
              dxf_rectyp,dxf_linbuf,dxf_LineNr);
*/
  }






  // Aktiven RECORD AUSGEBEN; der Header bleibt im Buffer.
  // printf("next header %d |%s|\n",act_typ,dxf_linbuf);

  // ENDSEC: fertig.
  if ((!strncmp (dxf_linbuf, "ENDSEC", 6)) ||
      (!strncmp (dxf_linbuf, "EOF", 3)))   {
    return 2;
  }






  /* ==== ein Element FERTIG ========================= */
  FERTIG:
  act_typ = 0;

    // TESTAUSG:
    // UTO_dump__ (*el, "DXF-out");

  return 0;






  /* ------- ERRORS ---------------------- */
  Err1:
    TX_Print("READ - ERROR");
    return 2;

  L_err_eom:
    TX_Print("EOM-Error obj=%d",dxf_rectyp);
    return 1;
}


//===========================================================================
  void dxfr_tra_arc (Circ* ci1,Mat_4x3 m1,Point* ptc,
                     double wa,double we,double crd) {
//===========================================================================
// IN:
// ptc ist im ECS (EntityCoord.System).
// wa,we = Winkel am Anf / End (in Deg.),
// crd = Radius


  Point2    pt20, pt21;
  Point     pt1, pt2;


  //TX_Print("dxfr_tra_arc %f %f %f",wa,we,crd);

  // ci1-> typ = Typ_CI;


  UT3D_pt_traptm3 (&ci1->pc, m1, ptc);


  // Punkt = Drehung um Center pt20
  pt20.x = crd;
  pt20.y = 0.;
  UT2D_pt_rotptangr (&pt21, (Point2*)&UT2D_PT_NUL, &pt20, UT_RADIANS(wa));
  //TX_Print("rotiert: %f,%f",pt21.x,pt21.y);

  pt2.x  = ptc->x + pt21.x;
  pt2.y  = ptc->y + pt21.y;
  pt2.z  = ptc->z;

  UT3D_pt_traptm3 (&ci1->p1, m1, &pt2);

  UT2D_pt_rotptangr (&pt21, (Point2*)&UT2D_PT_NUL, &pt20, UT_RADIANS(we));
  //TX_Print("rotiert: %f,%f",pt21.x,pt21.y);

  pt2.x  = ptc->x + pt21.x;
  pt2.y  = ptc->y + pt21.y;
  pt2.z  = ptc->z;

  UT3D_pt_traptm3 (&ci1->p2, m1, &pt2);

  ci1->rad = crd;

  ci1->vz.dx = m1[0][2];
  ci1->vz.dy = m1[1][2];
  ci1->vz.dz = m1[2][2];

  //TX_Print("ex dxf_tra_arc vz=%f,%f,%f rad=%f",ci1->vz.dx,ci1->vz.dy,ci1->vz.dz,ci1->rad);

}


//==========================================================================
  int dxf_ac_bulge_2pt (Circ2 *ci1, Point2 *p1, Point2 *p2, double *bulge) {
//==========================================================================
// dxf_ac_bulge_2pt    create 2D-cirle from bulge-value and 2 points
// RetCod:
//    0      OK
//   -1      no Circ - straight line.


// CW: bulge = negativ; CCW: bulge = positiv.
// bulge = 0: straight line;
// bulge = 1: 180-deg-circle.
// bulge = tan (oa/4)
// oa = OpeningAngle of arc
// bulge = (2 * h) / d
// d = len (p1, p2);
// r = s + h;  h = (bulge * d) / 2
//
//          ---|---
//        /    h    \
//     /       |       \
// p1 x--------x--d-----x p2
//           pm|
//      \      |      /
//       r     s     r
//        \    |    /
//             |
//             x pc


  int      irc, iDir, iTyp;
  double   blg, ao, dd, dh, ds, dr, d1;
  Vector2  vcm;
  Point2   pm;


  // printf("dxf_ac_bulge_2pt %f\n",*bulge);
  // printf("  p1 = %f %f\n",p1->x,p1->y);
  // printf("  p2 = %f %f\n",p2->x,p2->y);


  iDir = ISIGN(*bulge);
    // printf(" iDir=%d\n",iDir);

  blg = fabs(*bulge);
    // printf(" blg=%f\n",blg);

  if(blg < UT_TOL_min1) {       // straight line ..
    irc = -1;
    goto L_exit;
  } else {
    irc = 0;
  }

  ci1->p1 = *p1;
  ci1->p2 = *p2;
  dd = UT2D_len_2pt (p1, p2);
    // printf(" dd=%f\n",dd);


  UT2D_pt_mid2pt (&pm, p1, p2);


  d1 = fabs(blg - 1.);
  if(d1 < UT_TOL_min1) {       // 180-deg-circ
    ci1->pc   = pm;
    ci1->rad  = dd / 2.;
    ci1->ango = RAD_180 * iDir;
    goto L_exit;
  }


  ao = atan(blg) * 4.;
    // printf(" ao=%f\n",ao);


  if(blg > 1.) {
    iTyp = -1;
    dh = ( 1. / blg) * dd / 2.;
    ci1->ango = ao * iDir;
    ao = RAD_360 - ao;
  } else {
    iTyp = 1;
    dh = blg * dd / 2.;
    ci1->ango = ao * iDir;
  }
    // printf(" iTyp=%d ao=%f\n",iTyp,ao);  //1=normal=Arc<180; -1=Arc>180
    // printf(" dh=%f\n",dh);


  // ds = dd * 0.5 / tan(ao * 0.5);          // OK !
    // printf(" ds=%f\n",ds);


  d1 = dd / 2;
  dr = (((d1 * d1) + (dh * dh)) / dh) / 2;
  ds = dr - dh;
    // printf(" dr=%f ds=%f\n",dr,ds);


  if(blg > 1.) {   // Arc>180
    ci1->rad = (ds + dh ) * iDir;
    ds *= iDir;

  } else {         // Arc<180
    ci1->rad = (ds + dh ) * iDir;
    ds *= -iDir;
  }
  // ci1->rad = dd  * 0.5 / sin(ao * 0.5);   // OK !
    // printf(" rad=%f\n",ci1->rad);

  
  UT2D_vc_perp2pt (&vcm, p2, p1);

  UT2D_vc_multvc (&vcm, &vcm, ds / dd);      // UT2D_vc_setLength

  UT2D_pt_traptvc (&ci1->pc, &pm, &vcm);


  L_exit:
    // printf("ex dxf_ac_bulge_2pt %d\n",irc);
    // if(irc >= 0) {
    // printf(" pc = %f %f\n",ci1->pc.x,ci1->pc.y);
    // printf(" rd=%f ango=%f\n",ci1->rad,ci1->ango);
    // }

  return irc;

}


//================================================================
  int dxf_out_lwpoly (int pNr, int iClo) {
//================================================================
// polyline abarbeiten:
//   If (iClo == 1)  then add first point to polygon (closed curve).
//   create full circle for 2 points, closed, with bulge==1
//   create lines for 2 points with bulge==0
//   create Polygon for more then 3 points with bulge==0
//   create which circs for other bulge-values ?

 
  int      i1, iAct, iSta;
  Circ2    ci1;
  Line2    ln1;
  CurvPoly cv1;
  ObjGX    o1;



  // printf("LWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWPoly\n");
  // printf("dxf_out_lwpoly pNr=%d iClo=%d\n",pNr,iClo);


  //----------------------------------------------------------------
  // add last point if polygon is closed
  if(iClo != 1) goto L_1;
  if(UT3D_comp2pt(&dxf_pa[0], &dxf_pa[pNr - 1], UT_TOL_pt) != 0) goto L_1;
  if(pNr >= dxf_paSiz) {printf("**** overflow dxf_pa\n"); return -1;}
  dxf_pa[pNr] = dxf_pa[0];
  dxf_da[pNr] = 0.;
  ++pNr;



  //----------------------------------------------------------------
  // create full circle for 2 points, closed, with bulge==1
  L_1:
    // for(i1=0; i1<pNr; ++i1)
    // printf(" %d %f,%f %f\n",i1,dxf_pa[i1].x,dxf_pa[i1].y,dxf_da[i1]);

  if(iClo != 1) goto L_2;
  if(pNr != 3) goto L_2;
  // bulge=1 ? 
  if(UTP_comp_0(dxf_da[0] - 1.) != 1) goto L_2;
  if(UTP_comp_0(dxf_da[1] - 1.) != 1) goto L_2;
    // printf(" export 360-deg-circ\n");
  dxf_ac_bulge_2pt (&ci1, (Point2*)&dxf_pa[0], (Point2*)&dxf_pa[1], &dxf_da[0]);
  ci1.ango = RAD_360;
  ci1.p2  = ci1.p1;
  OGX_SET_OBJ (&o1, Typ_CI, Typ_CI2, 1, &ci1);

  return dxf_r_src_out (&o1);
  

  //================================================================
  L_2:
  // start loop tru all point of polygon ..
  iAct = -1;
  iSta = 0;

  //-----------------------------------yy-----------------------------
  L_3:
  ++iAct;
  if(iAct >= (pNr - 1)) goto L_plg_0;
  if(iAct < pNr) {
    if(UTP_comp_0(dxf_da[iAct]) == 1) goto L_3;
  }
  // iAct is a arc. Export points iSta-iAct as polygon,
  // then export iAct-(iAct+1) as arc. 


  //----------------------------------------------------------------
  // Export points iSta-iAct as line or polygon; 
  L_plg_0:
    // printf("  export polygon %d %d\n",iSta,iAct);
  if(iSta == iAct) goto L_arc_0;

  if((iAct - iSta) > 1) goto L_plg_2;
  // export Line (2 points only) ----------------------------
  UT2D_pt_pt (&ln1.p1, &dxf_pa[iSta]);
  UT2D_pt_pt (&ln1.p2, &dxf_pa[iSta+1]);
  OGX_SET_OBJ (&o1, Typ_LN, Typ_LN2, 1, &ln1);
  ln1.typ = 0;   // 2015-01-07
  dxf_r_src_out (&o1);
  goto L_plg_9;


  L_plg_2:   // export polygon (more than 2 points)--------------
  cv1.cpTab = &dxf_pa[iSta];
  cv1.ptNr  = iAct - iSta + 1;
  cv1.lvTab = NULL;
  cv1.v0    = 0.;
  cv1.v1    = 0.;
  cv1.dir   = 0;      // 0=fwd, 1=bwd.
  OGX_SET_OBJ (&o1, Typ_CVPOL, Typ_CVPOL, 1, &cv1);
  dxf_r_src_out (&o1);



  L_plg_9:
  if(iAct >= (pNr - 1)) return 0;


  //----------------------------------------------------------------
  // export iAct-(iAct+1) as arc.
  L_arc_0:
  if(UTP_comp_0(dxf_da[iAct]) == 1) goto L_arc_9;
    // printf("  export circ %d %d\n",iAct,iAct+1);

  dxf_ac_bulge_2pt (&ci1, (Point2*)&dxf_pa[iAct],
                          (Point2*)&dxf_pa[iAct+1], &dxf_da[iAct]);
  OGX_SET_OBJ (&o1, Typ_CI, Typ_CI2, 1, &ci1);
  dxf_r_src_out (&o1);


  L_arc_9:
  if((iAct+1) >= (pNr-1)) return 0;
  iSta = iAct;
  goto L_3;

}


//================================================================
  int dxfr_out_txt (ObjGX **ox, Memspc *wrkSpc) {
//================================================================
// dxfr_out_txt       create gcad-obj txt


  int      i1;
  GText    *tx1;

  // printf("dxfr_out_txt lNr=%ld\n",dxf_LineNr);


  // printf(" start ATTRIB LineNr=%d\n",dxf_LineNr);
  tx1 = UME_reserve (wrkSpc, sizeof(GText));
  // reserve 10K for Text in wrkSpc;
  tx1->txt = UME_reserve (wrkSpc, DXF_SIZ_TXBUF);

  // copy 10,20,30 = textpos
  tx1->pt = dxfr_pta_10[0];

  // copy 40 = Textsize (characterhoehe)
  tx1->size = dxfr_d_40;  // DXF_fakt_txtSiz;

  // // 50 = Winkel
  // if(recwin[0] != UT_DB_LEER) tx1->dir = recwin[0];
  // else tx1->dir = 0.;
  tx1->dir = 0.;

  tx1->txt[0] = '\0';
  if (strlen(tx1->txt) > 2) {
    strcat(tx1->txt, "[n");
  }

  // change 1 = dxf-text into gcad-text
  dxfr_gxt (dxfr_s_1);

  // copy 1 = text
  strcat(tx1->txt, dxfr_s_1);

  // remove "[n" at end of textBuffer
  i1 = strlen(tx1->txt) - 2;
  if(!strcmp(&tx1->txt[i1], "[n")) tx1->txt[i1] = '\0';

  // create out-obj
  OGX_SET_OBJ (*ox, Typ_GTXT, Typ_GTXT, 1, tx1);

  return 0;

}


//================================================================
  int dxf_r_src_out (ObjGX *ox1) {
//================================================================
// translate obj & export obj.

  int      irc;
  ObjGX    ox2;


  // printf("\n\ndxf_r_src_out typ=%d form=%d siz=%d\n",
          // ox1->typ,ox1->form,ox1->siz);
  // gdb_halt ();
  // if((ox1->typ==Typ_CVPOL)&&(ox1->form==Typ_PT)) UTO_dump__ (ox1, "ox1-1");
  // if((ox1->typ >= 120)&&(ox1->typ <= Typ_Process)) {
  // UTO_dump__ (ox1, "ox1-1");  // TODO crash m Typ_CVPOL/CarveDiem
  // printf(" dxf_iTra=%d\n",dxf_iTra);
  // UTRA_dump__ ();  // disp translObj
  // }


  // Objekt translieren             // 2016-03-14
  if(dxf_iTra == 1) {
    irc = UTRA_app_ox (&ox2, ox1, &DXFR_SPC_TRA);
    if(irc < 0) return irc;
    ox1 = &ox2;
  }

    // TEST ONLY
    // if((ox1->typ==Typ_CVPOL)&&(ox1->form==Typ_PT)) UTO_dump__ (ox1, "ox1-2");
    // END TEST ONLY



  // change obj to text and save it
  irc = AP_obj_2_txt (mem_cbuf1, mem_cbuf1_SIZ, ox1, -1L);
  // AP_obj_2_txt + UTF_add1_line


    // TESTBLOCK
    // { int typ; long dbi; char oid[64];
    // AP_obj_2_txt_query (&typ, &dbi); // get last gcad-typ/ind
    // APED_oid_dbo__  (oid, typ, dbi);
      // printf(" lNr=%d created %s\n",dxf_LineNr, oid);
    // if((typ == Typ_CVPOL)&&(dbi == 1L)) gdb_halt ();
    // if(irc < 0) {
       // printf(" errObj dxf_r_src_out typ=%d form=%d |%s|\n",
         // ox1->typ,ox1->form,mem_cbuf1);
    // }
    // }
    // END TESTBLOCK


  return irc;

}


// EOF
