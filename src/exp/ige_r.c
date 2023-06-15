// Subroutines fuer IGES - Datei lesen                   RF  2000-04-28
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
Iges sollte (wie Step) alle Objekte mal im memory sichern;
  IGE_r_work_2 sollte rekursiv ein Obj abarbeiten wie bei Step !!!
  Man sollte auch die Views beruecksichtigen (ev eigene Submodels !!!)
Bei SubModels in Datei ausgeben; erst wenn alles im Buffer ist
  (also vor dem close).

-----------------------------------------------------
Modifications:
2010-01-02 ige_r_getP_  line-buffer corr. RF.
2005-10-22 Fehler bei Transformation in IGE_r_126 korr.
2005-03-03 AP_obj_2_txt ruft nun UTF_add1_line; frueher nicht.
2004-04-10 phys.Dep nicht ausgeben. RF.
2002-10-06 128 zu. RF.
2002-08-02 112 via PSP3. RF.
2002-04-24 oTab -> 3D. RF.
2002-03-17 B-Spline (126) zu. RF.
2001-02-21 Portierung -> W32. RF.
2001-06-17 Portierung -> Linux. RF.

-----------------------------------------------------
*/
/*!
\file  ../exp/ige_r.c
\brief read IGEs files 
\code
=====================================================
List_functions_start:

IGE_r__                  Mainentry
IGE_ckFileFormat         test if file is IGES-File
IGE_r_work__             abarbeiten; zuerst Subfigures, dann main
IGE_r_work_1             eine Subfig oder main abarbeiten
IGE_r_work_2             ein Obj abarbeiten
IGE_r_work_3             ein phys.Dep.Obj nachtraeglich noch generieren
IGE_r_G__
IGE_r_D__                alle D-Zeilen einlesen
IGE_r_P__                alle P-Zeilen einlesen
IGE_r_getP_              alle Parameter der naechsten P - Zeile linnr. into ra.
IGE_r_decodeP_           Decodiere alle Parameter der aktuellen P-Zeile into ra
IGE_r_tra__
IGS_out__                output obj - gcad-sourcecode

IGE_r_116
..

IGE_rw_102
..

IGE_r_allocD             realloc impTab
IGE_r_line
IGE_r_hide               hide obj
IGE_r_DmaxPmax           Anzahl D- und P-Zeilen aus letzte zeile einlesen
IGE_r_allocD             realloc impTab
IGE_r_ind2dNr            get D-lineNr from impTab-index
IGE_r_dNr2ind            D-Nr --> Index
IGE_r_ck_skip            check if entity is supported
IGE_r_skip_wd
IGE_r_dump_impTab        dump einen impTab-Record

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



=======================================
Ablauf:

Eingelesen wird ueber 3 Units; die erste (fp_in) liest die D-Zeilen,
  die 2. die P-Zeilen.
  fp_3 ist frei; liest D und P-Zeilen, wenn erfoderlich ein rewind !


1. Durchlauf: nur CCV's (102) einlesen; die bearbeiteten Objekte merken.
2: Durchlauf: Alle restlichen Obj. bearbeiten.

.....................
1) alle Records einlesen (D- und P-Block !) und zwischenspeichern

   D-Block einlesen; IGE_r_D__; 
     impTab hat je einen Record pro D-Record.

   P-Block einlesen; IGE_r_P__; 
     P-Record-Daten --> impSpc kopieren;
     Pointer dorhin in die impTab (D-Records) zufuegen.
     Bei Point, Line, vectro kann gleich fertig aufgeloest werden;
     bei zB 190-Plane(IGE_r_190) werden nur die Pointer gespeichert,
     spaeter wird mit IGE_rw_190 fertig aufgeloest und die Plane erzeugt.

   MatrixTransformationenn ausfuehren (IGE_r_tra__)



2) alle abarbeiten; zuerst subfigs, dann main. (IGE_r_work__)
   Obj mit Status phys.Dep nicht ausgeben; erst wenn ein normales Obj es
   benoetigt.

   Subfigs sind 308;
   Main sind alle uebriggebliebenen Objekte.


programFlow:
IGE_r__ main import iges
  IGE_r_D__           alle D-Zeilen einlesen
  IGE_r_P__           alle P-Zeilen einlesen
  IGE_r_tra__        transform
  IGE_r_work_2(-1)   init (resolve links ?)                                 <<<< ??
    IGE_rw_1*          eg IGE_rw_144 (resolv trimmedSurf)
      IGE_r_work_3       work until now unresolved objs
  IGE_r_work__         abarbeiten
    IGE_r_work_1         work active subfigs/main
      IGE_r_work_2(i)      write-out obj i                                  <<<< ??
        IGS_out__            save
          AP_obj_2_txt
          UTF_file_Buf1__      write srcCode into UTF_FilBuf1
  MDL_load_import_attr create PermanentAttributes - HIDE, G#, SSTYLS ..


Types:
116=PT 110=LN 100=CI
142=CCV (CurveOnParametricSurface)
124=TrMat
108=190=PLN
118=122=SRU 120=SRV 128=SRBS
143=144=TPS


----------------------------------------------------------
IGE_r_xxx      IGE_rw_xxx

IGE_r_P__
 IGE_r_xxx 
  Wenn Pointer im Record, werden zuerst nur die Pointer gespeichert.
  Die pointer werden in impSpc via UTO_obj_save gespeichert.

IGE_r_work_2
 IGE_rw_xxx
  Noch nicht fertig aufgeloeste Entities nun aufloesen.
  An impTab[ind].data liegen die Ursprungsdaten (meist PointerTabelle)
  Die Outputdaten static machen


Nach dem Abspeichern als APT-Code und in der DB wird ein Pointer
  auf ein OvjGX-Obj geliefert - wo ???






=======================================
Testverfahren:
 - ein nicht dargestelltes Objekt (blanked oder abhaengig) sichtbar machen:
   den Status auf "00000000" setzen.

 - Find d-LineNr of gCad-Typ/Ind: siehe "Find d-LineNr of gCad-Typ/Ind"



=======================================
Memory-Usage:

2 Memorybloecke erforderlich:
 1 fuer die allg. Recordinfos (impTab[], Inhalt = D-Zeile)
   (bei reallocate muesste Pointer data korrigiert werden) !
   Statt reallocate einfach neu beginnen ?

 2 fuer den zugehoerigen Datenblock (impSpc).
   (nicht reallokierbar da viele Pointer !)




  IG_oTab       memspc54;         Die Ausgabespeicher
  IG_ra =       memspc51;         Die P-Parameterzeilenwerte als doubles
  IGlnP =       mem_cbuf1         Die P-Parameterwertzeile als Text
  IG_pTab =     memspc53;         ??
  pTab1 =       memspc55;         ?? temp; Text f. GTXT







=======================================
Compile:
cc -c ige_r.c






===========================================================================

Format: 
  Generell 80 Zeichen.
  Spalte 73: der Blocktyp;  S,G,D,P oder T; in dieser Reihenfolge.
  Spalte 74-80 sind eine Zeilennummer (jeder Block beginnt mit 0).


S-Block:
  Mindestens eine Zeile, Kommentare.


G-Block:  Global Section.
  Freies Format, mehrzeilig, hat viele Felder, durch Beistrich getrennt.
  Feld #
  13  ModelSpaceScale
  14  Units; 1=Inch, 2=mm
  18  Date & Time
  19: Min. Resolution = Identical_Points_Tolerance; zB 0.001
  20: Max.Coord.Value  (hoechster Coord.Wert)                  zB 1000.0


---------------------------------------------------------------------------
D-Block ("Definition")
  Fixes Format: 9 Integers/Chars zu je 8 Chars, 2 Zeilen pro Objekt.

  Zeile 1:

  0:
    Entity-type. 100=AC, 110=LN, 116=PT, 124 = TrMat
  1:
    Die Zeilen# des zugehoerigen P-Blocks. Es gibt immer einen P-Teil !
    lineNr of obj in P-block
  2:
    Structure; Negated D-Pointer; ??
  3:
    FontPattern od Negated D-Pointer zu FontPattern. 
    Bei Transfor,property: 1=Solid, 2=dashed, 3=Phantom, 4=centerlin, 5=dotted.
  4:
    Level. (Layer-# ?) Negativ: Pointer > Property(406) mit List of Levels
           (damit ein Obj auf mehreren Levels moeglich ist).
  5:
    VIEW-Pointer.
  6:
    Matrix. 0: keine Transformation; sonst ein Zeiger auf die P-Block# der
    Tranformation  (diese ist ein Zeiger auf die D-Zeilen# der Transformation).
  7:
    LabelDisplayAssoc. Immer 0.
  8:
    Status ( 4 Statusse a 2 digits; 0-3 -> stat01; 3-7 -> stat02)
      01:  00=Visible, 01=Blanked(hide)                     Col 65-66 (first col 1)
      23:  subordinate switch indicate physical dependency. Col 67-68
           00=unabhaengig; (main-model); obj can exist alone.
           01=Phys.abhaengig,(SubFig-objects, CCV-objects, View-objects)
              cannot exist until its parent exists !
           02=log.abhaengig, (f.Ent.410?)
              can exist alone, but is referenced by a parent.
           03=phys.u.log.abhaeng.
      45:  00=Geometrie,                                     Col 69-70
           01=Annotation (Kommentar)
           02=Definition,
           03=Other,
           04=Logical/Positional,
           05=2D-Param,
           06=ConstructionGeometry
      67:  Hierarchie; 00=TopDown, 01 02                     Col 71-72

      00 01 00 01   part of subfig or CCV; visible.



  Zeile 2:

  0:
    Die Entity-Nr. (gleich wie in Zeile 1).
  1:
    "Weight" (Dicke in mue). 0, 200, 1000 ..
  2:
    "Farbe"; 0=keine, 1=Black, 2=rot, 3=gruen, 4=blau, 5=gelb,
             6=magenta, 7=cyan, 8=weiss.
    Negativ: Pointer auf Typ 314.
  3:
    Anzahl der Parameter-Zeilen (P-Zeilen).
  4:
    "Form" = Entity Subtyp; 
      0=? 16=Assoz.Instance, 
      3=LinetypBeschreibung, 6=GeneralNote,
      6004=Property ..
  5:
    nix
  6:
    nix
  7:
    Label, Textbezeichnung, kann leer sein.
  8:
    "Subscr"; ?




-------------------------------------------------------------------------------
P-Block: ("Parameter")
  Freies Format, variable Zeilenanzahl, Felder durch Beistriche getrennt.
  Die Daten stehen in Spalte 1 bis 64.
    Das erste Feld ist ebenfalls die Entity#.
  Spalte 65 bis 72 enthalten eine Obj.ID.
    Diese ist ident mit zugehoerigen D-Block-ZeilenNummer der D-Zeile 1.
    Bei Fortsetzungszeilen ist die ID gleich.
  Spalte 74-80 sind eine Zeilennummer (jeder Block beginnt mit 1).

  Alle graf Objekte muessen in einem Set (Ent. 402) enthalten sein.

-------------------


  Property = 0       Hat keine P-Zeile; nur D-Zeilen ! 
 

  Vector  (123)
    123, dx, dy, dz;


  Point:  (116)
    116, x,y,z, 0;
    Es koennen 1 bis 3 Nullen nach den Koord sein!
    D-Parameter 1,9 = 20|001  = dependent = in einem Set enthalten
      20|000 = independent = nicht in Set ?


  Transformationsmatrix:  (124)
    Liefert eine 3x3 Transformationsmatrix und einen (normalerweise nicht
    benutzten) Punkt.
    VX, VY, VZ sind die Vektoren des Achsensystems.
    124, VX.x,VY.x,VZ.x, Ori.x,  VX.y,VY.y,VZ.y, Ori.y, VX.z,VY.z,VZ.z, Ori.z;



----------- CURVES: -----------
    
  Line: (110)
    110, ax,ay,az, ex,ey,ez, 0,0;
    Die Nullen nach den Koord. koennen fehlen !


  Arc:  (100)
    Immer CCW; A und E sind relativ zum Mittelpunkt.

    Absolut, ohne Transformation (A und E hier absolut!) Wenn D-Par 1/9 0 ist.
    100, z-Wert, mx,my, ax,ay, ex,ey;  
    Relativ, mit Transformation (124) D-Par 1/9 ist 20001.


  ConicArc:  (104)
    Form; 1=Ellipse, 2=Hyperbel, 3=Parabel.




  Copious Data:   (106)
    Form 1 = 2D-Punktewolke
    Form 2 = 3D-Punktewolke
    Form 11 = 2D-Polygon
    Form 12 = 3D-Polygon
    Form 63 = wie 11

    2D-Punkte:
      106, 1, Z-Val, ptAnz, p1x,p1y, ...  pnx,pny;
    3D-Punkte:
      106, 2, ptAnz, p1x,p1y,p1z, ...  pnx,pny,pnz;


  GeneralNote: (212)
    212,1,Chranz,Hoehe,Breite,font,Slant,Rot,0,0,px,py,pz,text,0,0;
    Text: "xy" zB als ",2Hxy,"


  Leader (Arrow, 214)
    Form  1  Arrow head open
    Form  2  Arrow head closed
    Form  3  Arrow head filled
    Form  4  no head
    Form  5  circle outlined
    Form  6  circle filled
    Form  7  box outlined
    Form  8  box filled
    Form 11  Arrow head outlined
    Form 12  circle




 112 Parametric Spline Curve
    

 126 Rational B-Spline Curve
     SNR (int) Anzahl_Segmente = K-1  (K=Anzahl_Controlpoints)
     M (int) degree of basic functions
     Prop1 (int) 0=nonplanar, 1=planar
     Prop2 (int) 0=open, 1=closed curve.
     Prop3 (int) 0=rational, 1=polynomial
     Prop4 (int) 0=nonperodic, 1=periodic.
     T[]   (floats) KnotSequence Anzahl=K-M+1+2*M
     W[]   (floats)  Weigths, Anzahl=K
     X,Y,Z (3 floats) Punkte, Anzahl=K
     V(0)  (float) StartParameterWert
     V(1)  (float) EndParameterWert
     X,Y,Z (3 floats) Normalvektor (nur wenn Kurve planar)


 141 Boundary
     Typ(Int)  0=ModelSpaceCurves 1=ModelSpaceCurves & ParameterSpaceCurves
     PREF(Int) 0-3; Representation; unused.
     SPTR(Int) Pointer zur Stuetzflaeche (untrimmed)
     N(Int)    Anzahl der (nun folgenden) Boundaryelemente
       ID      Pointer zum Boundaryelement
       SENSE   Orientation; 1=normal, 2=reverse
       K       Anzahl of parameterCurves (0, wenn Typ=0)
       P[K]    Pointer zu den parameterCurves


 142 CurveOnParametricSurface
     (int) CreationFlag 0=unspec;1=proj.Curv->Surf;2=Inters.Surf/Surf;
             3=IsoparametricCurve.
     Pointer to Stuetzflaeche (Surf on which curv lies); zB auf 128
     Ptr auf die 2D-Ableitung der Curve (zB CCV - 102) od 126; keine: 0.
     Ptr auf die 3D-Curve Boundary (zB CCV - 102) od 100-Circ!
     (int)   Darstellung:0=unspec;1=SxB;2=C;3:C od SxB



//===================================================================
 SURFACES:


 108 Plane:   (108)   (BoundedPlane Surf.)
     108, A, B, C, D, Pointer_auf_begrenzende_Curve, X_Symbol, Y_Symbol, Z_Symbol
     Plane wird dr. 4 Parameter (A bis D) definiert. Fuer alle Punkte auf der
     Plane gilt:
     A * x + B * y + C * z = D


 190 Plane 
     Form=0:
       Ptr_Origin, Ptr_Z-Axis;
     Form=1:
       Ptr_Origin, Ptr_Z-Axis, Ptr_X-Axis;


 128 Rational B-Spline Surface
     K1 (int) upper index of sum
     K2 (int) upper index of sum
     M1 (int) degree of basic functions
     M2 (int) degree of basic functions
     Prop1 (int) 0=not closed in 1.dir, 1=closed
     Prop2 (int) 0=not closed in 2.dir, 1=closed
     Prop3 (int) 0=rational, 1=polynomial
     Prop4 (int) 0=nonperodic in 1.dir, 1=periodic.
     Prop5 (int) 0=nonperodic in 2.dir, 1=periodic.
     T[]   (floats) 1. KnotSequence Anzahl=K-M+1+2*M
     T[]   (floats) 2. KnotSequence Anzahl=K-M+1+2*M
     W[]   (floats) Weigths, Anzahl=K
     X,Y,Z (3 floats) CtrlPoints, Anzahl=K
     V(0)  (float) 1. StartParameterWert
     V(1)  (float) 1. EndParameterWert
     V(0)  (float) 2. StartParameterWert
     V(1)  (float) 2. EndParameterWert


 118 RuledSurface
     Pointer 1. curve
     Pointer 2. curve
     (int)   direction; 0=first to first, 1=first to last
     (int)   development flag: 1=developable, 0=possibly not


 120 Surface of Revolution
     Pointer to Axis (Line)
     Pointer to Contour (Line=Conus, Circle=Torus ..)
     StartAngle (0.)
     TerminateAngle (6.28)


 122 Zylinderflaeche
     (eine Line (generatrix) wird (immer parallel zu sich selbst) entlang
      einer Kurve (directrix) bewegt).
     Pointer directrix
     (3 x double) Endpunkt der generatrix (Startpunkt = Start der directrix)


 143 BoundedSurface (Bound = 141)
     Typ(Int)  0=ModelSpaceCurves 1=ModelSpaceCurves & ParameterSpaceCurves
     SPTR(Int) Pointer zur Stuetzflaeche (untrimmed)
     N(Int)    Anzahl der (nun folgenden) Kurven
     P[N]      N Pointers to Boundarycurves (Aussenkontur, Loecher ..)


 144 TrimmedParametricSurface (Bound = 142)
     Ptr auf die Stuetzflaeche; zB auf 102 od 128
     TrimmedFlag; 0=untrimmed, 1=trimmed
     IBN = Anzahl Inseln innerhalb der Boundary
     Ptr auf die Boundary; (-> 142)
     Ptr[IBN] auf die Inseln ..




//===================================================================


 102 Composite Curve (CCV, Kontur)
     Anzahl_Mitglieder (int)
     Liste_Mitglieder (int[]) (P-BlockObj.ID's)


 308 SubfigureDefinition
     P-Block:
       (int)    depth (levels)
       (string) name   (zB "8HTRIANGLE")
       (int)    anzahl objects
       (int[])  pointer to objects (auf die D-ZeilenNummern)



 408 SubfigureInstance   (Ditto)
     D-Block:
       Zeile1,Feld7: TraMat.
     P-Block:
       (int)       pointer zum P-label der SubFigDef(308)
       (double[3]) position
       (double)    scale



//===================================================================
 noch nicht implementiert - not supported:


 186   MSBO (ManifSolid B-Rep)

 192 CircularCylinder Surf.
 196 Sphaerical Surf.
 194 CircularConic Surf.
 198 Toroidal Surf.

 202 Angular Dimension
 206 Diameter Dimension
 210 General Label
 216 Linear Dimension
 218 Ordinate Dimension
 222 Radius Dimension
 228 GeneralSymbol

 230 sectioned area (hatch)

 314   ColorDefinitionEntity (Doku siehe unten)

 402   Assoziativity (Group,Set, ...)  (Doku siehe unten)
 404   Drawing
 406   Property  (Doku siehe unten)
 410   View (moeglicherweise nur eine Viewposition (Punkt u Vektor (Plane))
 416   ExternalReference 

 502   Vertex
 504   Edge
 508   Loop
 510   Farcve
 514/1 Shell (closed)
 514/2 Shell (open)



-------------------------------------------------------------
 314 ColorDefinitionEntity
     red (float)
     green (float)
     blue (float)
     colourName (string, optional)


 402 Assoziativity (Group,Set, ...):
    (Assoziative Instance, Form 7)  Bei Euclid Figur ?
    Fuer alle Objekte; Trans. muessen hier aber nicht rein.
    402,Anzahl,ind1,ind2 ... indn;  (index into D-LineNr)
    Enthaelt auch 102.
    D-Par1/9 = 301; bei den Objekten depandant - D-1/9=20001.
    Form  1: Group (Unordered group with back-pointers instance)
    Form  7: zB Layer (Unordered group without back-pointers instance)
    Form 12: External File-Reference !!! (siehe auch Form2, Ent.416)
    Form 13: Dimension_Assocativity (Zusammenhang der Dimension-Objekte)
    Form 14: Ordered group with backpointers instance
    Form 15: Ordered group without backpointers instance
    Form 16: Planar_Assoc. (objects are coplanar)


 406 Property (Text):
    Form 0 = Spacetext.
    Form 6 = GeneralNote (Draft); Drilled Hole ??
    Form 15  Name "4HMAIN" ?
    Form 16  Drawing Size (zB "11.")
    Form 17  "2HIN" ?





------------------------------------------------------------------------------
T-Block
  Statistikdaten; Anzahl der Zeile der Bloecke;
  Format ist 4(C1,I7).



//================================================================
Doku:
  igs/doc/version6.pdf
    Inhaltsverz: S14


//================================================================
Testmodels:
IGS/0.igs                 5K        OK
IGS/samp_ige_geo2.igs    14K        OK
IGS/SolidWorks0.igs      25K        OK
IGS/Rohr1.igs            29K        OK
IGS/su1.igs              35K        OK
IGS/wash1.igs            30K        OK
IGS/Rohr2.igs            35K        OK
IGS/nut1.igs             45K        OK
IGS/Starcd_1.igs         46K        OK
IGS/wash_M12.igs         62K        OK
IGS/Drehteil.igs        500K        OK
IGS/bspsur4.igs          71K        OK
IGS/Block.igs           2.1M        OK
IGS/Schuh.igs           420K        OK
IGS/Unfold.igs          375K        OK

IGS/1.igs
IGS/01.igs
IGS/DIM_TEST.igs
IGS/GelochteFlaeche2.igs
IGS/M4.igs
IGS/Plot_1_Modell.ig2
IGS/c109806.igs
IGS/ccv1.igs

IGS/f126x.igs
IGS/f128_2.igs
IGS/f408_2.igs
IGS/m2_2.igs
IGS/rhi5.igs
IGS/submod1.igs
IGS/sur_bspRev1.igs
IGS/bspsur_bsp.igs
IGS/tisler_01.igs

*/




// definition "export"
#include "../xa/export.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>       // fuer Variad. Funktionen (...)
#include <ctype.h>                    // isdigit




// #include "../ut/ut_umem.h"             // UME_save
#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_cast.h"                // INT__PTR
#include "../ut/ut_ox_base.h"             // OGX_SET_INDEX

#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_obj.h"              // UTO_stru_2_obj UTO_obj_save
#include "../ut/ut_os.h"               // AP_get_bas_dir ..
#include "../ut/ut_TX.h"               // TX_Print
#include "../ut/func_types.h"               // UI_Func...
#include "../ut/ut_memTab.h"           // MemTab
#include "../ut/ut_itmsh.h"            // MSHIG_EDGLN_.. typedef_MemTab.. Fac3
#include "../ut/ut_deb.h"              // DEB_*

#include "../gr/ut_DL.h"               // DL_GetAtt
#include "../gr/ut_gr.h"               // GTX_..

#include "../db/ut_DB.h"               // DB_GetGTxt

// #include "../xa/xa_ui.h"               // UID_..
#include "../xa/xa_mem.h"              // memspc51, mem_cbuf1
#include "../xa/xa_obj_txt.h"          // AP_obj_add_dbo



//----------------------------------------------------------------
// EXPORTS to main-module
export int  IGE_r__ (void**);





// // ex ../gr/ut_GLU.c
// extern Point     *GLT_pta;
// extern long      GLT_pta_SIZ;


#define ENT_ERR  1        // EnytityNr fro not supported entities


static int   impStat=0;                     // Statuscode.
// 2 import sofort beenden.




// .fTyp      EntityNr
// .fInd      P-LineNr (im P-Block ganz rechts)
//            die D-LineNr (ganz rechts): dlNr = impInd*2
//            impInd=(D-Linenr+1)/2 (im D-Block ganz rechts)
// .trInd     TraMatNr
// .stat01    0=Visible, 01=Blanked(hide)
// .stat23    00=unabhaengig; (main-model); obj can exist alone.
// .siz       nr of records; eg for CCV ?
// .typ, form, ind: vom gespeicherten gCAD_Objekt!
//            308-Submodels: ind=BasModelNr
//            ind speichert vorher D-Lnr der TraMat
// .data      ObjGX of obj
// .done      0=no-gcadObj-created-yet
// .activ     1 = this obj is used in the active group - output it
// .stat__    128  (rbspl-curve) 1=planar, else 0
typedef struct {long ind, trInd; void *data;
                int fInd;
                unsigned short siz;
                unsigned short typ, form, fTyp;
                unsigned stat__:28,
                         stat01:1, stat23:1,
                         done:1, activ:1;}                           ImpObj;



static ImpObj *impTab=NULL;
static long   impTabSiz=0, impNr=0, impInd;

// impTab:
// impTabSiz  Anzahl allokierter structs
// impNr      Anzahl belegter Records (= Anzahl D-Bloecke)
// impInd     der aktuelle Recordindex;

// Nicht mehr used:
// ID         IG_recID=D-LineNr D-ganzRechts) gespeichert (fuer CCV)



static Memspc impSpc;
// soviel muss immer frei sein in impSpc
#define impSpc_INC  50000
#define impMDLNR    10000


static int    IG_dNr;        // active D-LineNr  (= ##P in P-Lines)
static int    IG_trNr;                // transformationsindex
static Mat_4x3 IG_trMat;

static char *IG_mainNam;        // mainmodelname
static char IG_modNam[256];     // active (sub)modelname

static int IG_mdli;             // index of subModel (refModels)



// Submodeldefinition
typedef struct {char *mnam; int siz; long *iTab;}                ImpSubmodel;






//=======================================================================
  int IGE_ckFileFormat (char *fnam) {
//=======================================================================
// RC = 0: Datei ist ein IGES-File
// RC = 1: Datei ist kein IGES-File

// wenn die Zeilen 2-7 80 chars haben



  int   irc, i1;
  FILE  *fpi;

  irc = 1;


  if((fpi = fopen (fnam, "r")) == NULL) {
    printf ("Fehler open Datei %s\n", fnam);
    goto L_go;
  }

  if(fgets(memspc51, 84, fpi) == NULL) goto L_clo;


  if(fgets(memspc51, 84, fpi) == NULL) goto L_clo;
  i1 = strlen(memspc51);
  if(i1 != 81) goto L_clo;

  if(fgets(memspc51, 84, fpi) == NULL) goto L_clo;
  i1 = strlen(memspc51);
  if(i1 != 81) goto L_clo;

  if(fgets(memspc51, 84, fpi) == NULL) goto L_clo;
  i1 = strlen(memspc51);   // printf("i1=%d\n",i1);
  if(i1 != 81) goto L_clo;



  irc = 0;


  L_clo:
  fclose(fpi);


  L_go:
  // printf("ex dxf_ckFileFormat |%s| %d\n",fnam,irc);
  return irc;  // Datei ist ein DXF-File

}


//===========================================================================
  int IGS_out__ (ObjGX *ox1) {
//===========================================================================
// Callback des Iges-Import;
// mit Open next Model
// mit obj ausgeben

// static FILE   *imp_lun = NULL;
// static int    imp_file = 0;  // 0=Ausgabe ins mem, 1=Ausgabe ins File.


  int         i1, i2, irc, typ;
  long        *indTab, ind;
  char        cbuf[512];
  ModelRef    *mr1;
  ImpSubmodel *sm1;


  // printf("IGS_out__ typ=%d siz=%d impInd=%ld\n",ox1->typ,ox1->siz,impInd);
  // printf(" IG_modNam=|%s| impStat=%d\n",IG_modNam,impStat);
  // if(impInd==5) DEB_dump_ox_0 (ox1, "iI=5");



  if(impStat != 0) return -1;
  
  if(impInd >= impTabSiz) {
    printf("***** Error: IGS_out__ E001\n");
    return -1;
  }

  // Typ_Done: skip this obj ..
  if(ox1->typ == Typ_Done) {
    return 0;



  //-----------------------------------------------------------
  // Typ_SubModel = Start new Model; data = Submodelname
  } else if(ox1->typ == Typ_SubModel) {
      // printf("Start-new-Model |%s|\n",(char*)ox1->data);

    IG_mdli = 20; // reset startIndex models

    // ein Submodel ist aktiv: den Buffer in die Datei schreiben,
      if(strlen(IG_modNam) > 0) {  // Main hat leeren Name
        UTF_add1_line ("# import end\n");
        sprintf(cbuf,"%sModel_%s",AP_get_tmp_dir(),IG_modNam);
        // write buffer-1 into file cbuf
        UTF_file_Buf1__ (cbuf);
      }

    // den Buffer zuruecksetzen.
    UTF_clear1 ();

    // start new (sub)model; main has empty modelname
    if(strlen(ox1->data) > 0) {
      strcpy(IG_modNam, (char*)ox1->data);  // merken fuer "write to file"
         // printf("start-Submodel |%s|\n",IG_modNam);

      sprintf(cbuf,"# Import Submodel %s",IG_modNam);
      UTF_add1_line (cbuf);  // noch ins Main !

    } else {
        // printf("start-mainmodel \n");
      // sprintf(cbuf,"# IGES-Import %s",fnam);
      // strcpy(IG_modNam, IG_mainNam);
      sprintf(cbuf,"# IGES-Import");
      UTF_add1_line (cbuf);

    }


    AP_obj_2_txt (NULL, 0L, NULL, 0L);  // reset Startindizes
    // GA_hide__ (-1, 0L, 0); // geht hier nicht - loescht vorhandene hides !!

    return 0;



  //-----------------------------------------------------------
  } else {

    if(ox1->typ == Typ_Model) {
      // write out call subModel from Typ_Model = ModelReference
      // eg "M#="<mdlNam>" <rfesys>"
      mr1 = (ModelRef*)ox1->data;
      i1 = mr1->modNr;
      sm1 = (ImpSubmodel*)impTab[i1].data;
      if(!sm1->iTab) {
        // model not found ..
        TX_Error("IGS_out__ E1 model M%d not found",IG_mdli);
        return -1;
      }
        // printf("  modelRef-1 |%s|\n",sm1->mnam);
        // DEB_dump_obj__ (ox1->form, ox1->data, "mr" );
      sprintf(cbuf, "M%d=\"%s\"", IG_mdli,sm1->mnam);
      // add refSys from pos, vz vx
      AP_obj_add_pln1 (cbuf, &mr1->po, &mr1->vz, &mr1->vx);
        // printf(" modelRef-2 |%s|\n",cbuf);
      ++IG_mdli;
      UTF_add1_line (cbuf);  // write
      goto L_exit;
    }



    // Objekt in Text umwandeln
    // irc = AP_obj_2_apt (mem_cbuf1, mem_cbuf1_SIZ, &elT, NULL);
    irc = AP_obj_2_txt (mem_cbuf1, mem_cbuf1_SIZ, ox1, -1L);
    if(irc < 0) return irc;
      // printf(" -nxt-|%s|\n",mem_cbuf1);
      // printf(" impInd=%ld\n",impInd);


    L_cont9:
      // typ und APT-index of generated object eintragen !
      // AP_obj_2_txt_query (&impTab[impInd].typ, &impTab[impInd].ind);
      AP_obj_2_txt_query (&typ, &ind);
      impTab[impInd].typ = typ;
      impTab[impInd].ind = ind;
        // printf("  out %d: typ=%d ind=%ld\n",impInd,
               // impTab[impInd].typ,impTab[impInd].ind);

    // TEST    Find d-LineNr of gCad-Typ/Ind
    // if((typ == Typ_SUR)&&(ind == 4)) exit(0);
    // if(((typ == Typ_AC)||(typ == Typ_CI))&&(ind == 25)) {
    // if((typ == Typ_LN)&&(ind == 1487)) {
      // printf(" obj ex ind=%d %dP \n",impInd,IGE_r_ind2dNr(impInd)); exit(0);}
    // // TEST 


    // hide ..
    if(impTab[impInd].stat01 != 0) {
      // printf(" hide1 %d typ=%d ind=%d stat01=%d\n",impInd,typ,ind,
                    // impTab[impInd].stat01);
      // GA_hide__ (0, ind, typ);
      // GA_view__ (-1, 1, typ, ind);
      IGE_r_hide (typ, ind);
    }




  //-----------------------------------------------------------
  }

  // ObjektZeile raus;
/*
2005-03-03: raus; AP_obj_2_txt ruft nun UTF_add1_line; frueher nicht.
OFFEN: bei SubModels in Datei ausgeben; erst wenn alles im Buffer ist
  (also vor dem close).


  if(imp_file == 0) {  // Ausgabe ins mem
    irc = UTF_add1_line  (mem_cbuf1);  // 0 od -1
    if(irc < 0) impStat = 2;

  } else {  // Ausgabe ins File
    fprintf(imp_lun, "%s\n",mem_cbuf1);
  }
*/

  L_exit:
    // Next_Obj:;
    // ++impNr;
    // if(impNr >= impTabSiz) break;

      // printf("ex-IGS_out__\n");

    return 0;

}


//===========================================================================
  int IGE_r__ (void* pBlk[2])  {
//===========================================================================
// IGE_r__     main import iges
// mode3d: ON  = 0 = Ja,   als 2D-Mode behandeln;
// mode3d: OFF = 1 = Nein, als 3D-Mode behandeln;
// Input:
//   pBlk[0]    fnam = full Igesfilename
//   pBlk[1]    fnam = full outfilename  = IG_mainNam

// Subfigures werden -> datei imp_lun ausgegeben, die mainfig -> memory.

  int     irc;
  long    dSiz, pSiz, l1;
  char    *fnam;
  double  mSiz;
  FILE    *fp1 = NULL;


  fnam       = (char*)pBlk[0];
  IG_mainNam = (char*)pBlk[1];
  IG_modNam[0] = '\0';


  // printf("\n\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII\n");
  // printf("IGE_r__  |%s|\n",fnam);
  // printf("  IG_mainNam |%s|\n",IG_mainNam);
  // AP_debug__("IGE_r__-in");


  impStat = 0;
  IG_mdli = 20;

  // Init Objektindexe
  AP_obj_2_txt (NULL, 0L, NULL, 0L); // reset Startindizes

  // UtxTab_init__ (&IG_mNames);

  // IG_cBuf   = memspc50;   // use 5012 Byte ?

    // IGE_test (0);

  // Zwischenspeicher UTF_FilBuf1 loeschen
  UTF_clear1 ();


  // sprintf(mem_cbuf1,"# IGES-Import %s",fnam);
  // UTF_add1_line (mem_cbuf1);


  // Open for read
  if((fp1=fopen(fnam,"r")) == NULL) {
    TX_Print("****** OPEN ERROR FILE %s **********\n",fnam);
    goto L_exit;
  }


  // Read G-Zeile; get ModelSize
  irc = IGE_r_G__ (&mSiz, fp1);
    // IGE_test (1);


  // change ModelSize
  if(irc >= 0) {
    mSiz = UTP_db_rnd5 (mSiz);
    NC_setModSiz (mSiz);
  }
    // IGE_test (2);


  // Anzahl D-zeilen und Anzahl P-Zeilen aus igesfile/letzte zeile einlesen
  if(IGE_r_DmaxPmax (&dSiz, &pSiz, fp1) < 0) goto L_exit;

  dSiz = (dSiz / 2 ) + 10;


  // alloc space for impTab (D-zeilen)
  // impTab = (ImpObj*)memspc101;
  // impTabSiz = sizeof(memspc101) / sizeof(ImpObj);
  if(IGE_r_allocD (dSiz) < 0) goto L_err1;



  // alle D-Zeilen einlesen
  if(IGE_r_D__ (fp1) < 0) goto L_exit;
  // printf("ex IGE_r_D__ %d recs geladen\n",impNr);
  TX_Print("IgesImport: %d Records loaded",impNr);
    // IGE_test (3);

  // printf("impTabSiz=%ld impNr=%ld impInd=%ld\n",impTabSiz,impNr,impInd);
  // printf("impTab tot=%ld\n",impNr*sizeof(ImpObj));


  // init dataspace
  // UME_malloc (&impSpc, impNr * 300);
  // l1 = (impSpc_INC * 3) + (pSiz * 70);
  l1 = (impSpc_INC * 3) + (pSiz * 80);
  irc = UME_malloc (&impSpc, l1, impSpc_INC);
  if(irc < 0) goto L_exit;
    // IGE_test (4);


  // alle P-Zeilen einlesen
  if(IGE_r_P__ (fp1) < 0) goto L_exit;
    // printf(" foll-IGE_r_P__ IG_modNam=|%s|\n",IG_modNam);
    // IGE_test (5);


  // alle transformieren
  if(IGE_r_tra__ () < 0) goto L_exit;
    // printf(" igi02\n");
    // IGE_test (6);


  // UME_dump (&impSpc, "Dump impSpc: ");

  // init
  IGE_r_work_2 (-1);
    // printf(" igi03\n");
    // IGE_test (7);

  
  // TESTAUGABEN__________________
  // DEB_dump_ox_0 (impTab[0].data, "iI=0");
  // IGE_r_dump_impTab (0);
  // return 0;
  // TESTAUGABEN__________________



  // abarbeiten
  IGE_r_work__ ();
    // printf(" igi04\n");



  //---------------------------------------------------------
  FERTIG:
  // 3D ausschalten
  // UTF_add1_line ("MODE 2D");

  // if(imp_lun != NULL) {
    // fclose (imp_lun);
  // }

  sprintf(mem_cbuf1,"# End IGES-Import");
  // an UTF_FilBuf1 hintanfuegen
  UTF_add1_line (mem_cbuf1);
    // printf(" igi05\n");




  // gesamte Datei schreiben
  Fertig_1:

  // TX_Print("****  Einlesen fertig");
  fclose(fp1);

  // write attributes and code in buffer-1 into file IG_mainNam
  MDL_load_import_attr (IG_mainNam, mSiz);

    // TESTBLOCK
    // UTF_dump__ ("ex-IGE_r__");
    // printf(" ex-IGE_r__\n");
    // END TESTBLOCK
 

  irc = 0;


  //----------------------------------------------------------------
  L_exit:
    // printf(" free impTab\n");
    if(impTab) free (impTab);
    impTab = NULL;

    // printf(" free impSpc\n");
    UME_free (&impSpc);

    // UtxTab_free (&IG_mNames);
    // Exitmessage
    IGE_r_work_2 (-2);
      // printf(" igi07\n");

      // TESTBLOCK
      // sprintf(memspc50,"cat %s",IG_mainNam); system(memspc50);
      // END TESTBLOCK

    return irc;


  //----------------------------------------------------------------
  L_err1:
    irc = -1;
    goto L_exit;


}


//===========================================================================
  int IGE_r_work__ () {
//===========================================================================
// abarbeiten; zuerst Subfigures, dann main.    (war IGE_r_sf_find)

  int     i1;


  // printf("0000000000000000000 IGE_r_work__ %d\n",impNr);




  //=====================================================================
  // alle subfigs abarbeiten;
  impInd = -1;


  L_next:
    ++impInd;
    if(impInd >= impNr) goto L_main;

    if(impTab[impInd].fTyp != 308) goto L_next;  // find next Subfig
    if(impTab[impInd].done == 1) goto L_next;

    // dNr = impInd * 2 + 1;

    // printf(" next SubFig %d %d\n",impInd,impTab[impInd].fTyp);

    // get SubFig-Name -> memspc55 and activate members
    IGE_rw_308 (impInd);


    // mark 308-line done
    impTab[impInd].done = 1;


    // die mit impTab[].activ markierten Lines ausgeben und als
    // done markieren
    IGE_r_work_1 ();
    if(impStat > 1) return -1;


    // reset .activ
    for(i1=0; i1<impNr; ++i1) {
      // Nach jedem UP alle typ und ind loeschen!
      // impTab[i1].typ   = Typ_Error;
      impTab[i1].activ = 0;
    }

    impInd = -1;  // next Subfig ..
    goto L_next;



  //=====================================================================
  // main abarbeiten;
  L_main:

  // printf(" next Main %d %d\n",impInd,impTab[impInd].fTyp);

  // alle main-obj aktiv setzen
  for(impInd=0; impInd<impNr; ++impInd) {
    if(impTab[impInd].done == 0) {
      // printf(" ... nxt mem impInd=%d fTyp=%d fInd=%d\n",impInd,
             // impTab[impInd].fTyp,impTab[impInd].fInd);
      impTab[impInd].activ = 1;
    }
  }

  // nam main = ""
  strcpy(memspc55, "");
  IGE_r_work_1 ();


  // printf("0000000000000000 ex IGE_r_work__ %d\n",impNr);

  return 0;

}


//===========================================================================
  int IGE_r_work_1 () {
//===========================================================================
// eine Subfig oder main abarbeiten;                  war IGE_r_sf_work
// die zugehoerigen Obj haben impTab[].activ = 1;


  int    irc, i1, dNr;
  ObjGX  ox1;

  // printf("111111111111111111 IGE_r_work_1 |%s|\n",memspc55);

  // Start new Model - Callback
  // ox1 = UTO_stru_2_obj (Typ_SubModel, Typ_SubModel, memspc55);
  OGX_SET_OBJ (&ox1, Typ_SubModel, Typ_SubModel, 1, memspc55);
  IGS_out__ (&ox1);


  //--------------------------------------
  impInd = -1;

  L_next:
    ++impInd;
    if(impInd >= impNr) goto L_fertig;

    if(impTab[impInd].activ != 1) goto L_next;  // find next member
      // printf("\n... work_1 nxt impInd=%ld fTyp=%d fInd=%d\n",impInd,
               // impTab[impInd].fTyp,impTab[impInd].fInd);

    // obj ausgeben
    IGE_r_work_2 (impInd);

    // mark line done
    impTab[impInd].done = 1;

    
    if(impStat < 2) goto L_next;



  //----------------------------------------------------
  L_fertig:
    // printf("111111111111111 ex IGE_r_work_1 |%s|\n",memspc55);
  return 0;

}


//=====================================================================
  int IGE_r_work_2 (int ind) {
//=====================================================================
// write out Obj;                   war IGE_r_work_2
// ind ist Pointer -> impTab.

static  int oCnt1, oCnt2;

  int     i1, dNr, IG_typ_act;
  ObjGX   ox1, *oxp1, *oxp2;



  // reset
  if(ind < 0) {
    if(ind == -1) { oCnt1 = 0; oCnt2 = 0;}
    // TX_Print("    %d objects imported ..",oCnt1);  // ouLine reservieren
    return 0;
  }

  ++oCnt1;  i1 = oCnt1 / 5000;
  if(i1 != oCnt2) {
    oCnt2 = i1;
    UI_winTX_rmLast ();
    TX_Print("    %d objects imported ..",oCnt1);
  }



  //----------------------------------------------------------------
  IG_typ_act = impTab[ind].fTyp;
  dNr = IGE_r_ind2dNr (ind);   // i*2+1



  // printf(">>>>>> IGE_r_work_2 impInd=%d dNr=%d fTyp=%d\n",ind,dNr,IG_typ_act);




  // skip not supported ent's
  if(IGE_r_ck_skip (IG_typ_act) != 0) {
    // printf(" skip impNr=%d dNr=%d fTyp=%d\n",impNr,dNr,IG_typ_act);
    impTab[impInd].fTyp = ENT_ERR;
    impTab[impInd].ind  = 0;
    return 0;
  }


  // its obj phys.Dep ?
  if(impTab[ind].stat23 != 0) {
    // printf(" skip phys.Dep: \n");
    // den Vater dieses Objekts suchen; wenn Vaterobj not supported: skip Obj.
    return 0;
  }



  // // find P-Line <plNr>
  // IGE_r_getPP (IG_ra, IG_ra_siz, dNr, fp_3);


  // EntityNr (zB. 116=Punkt) in gCAD-Typ Typ_PT umschluesseln


  // skip erraneous objects
  if(impTab[ind].typ == Typ_Error) return 0;


  // skip obj's already created
  if(impTab[ind].ind > 0) return 0;



  ox1.typ  = impTab[ind].typ;
  ox1.form = impTab[ind].form;
  ox1.siz  = impTab[ind].siz;
  ox1.data = impTab[ind].data;

  // printf("    typ=%d form=%d\n",ox1.typ,ox1.form);
  // printf("    ftyp=%d ind=%ld\n",impTab[ind].fTyp,impTab[ind].ind);



  //======================================================================
  // Links aufloesen; es gibt teilweise gespeichert Indices auf
  // impTab-Objekte; diese Indices umwandeln in APT-Typ/APT-Ind (objekt muss
  // in gCAD schon generiert worden sein !

  // Typ 118: die impTab-indices noch in typ/ind umwandeln
  // printf("vor Ig_CB: typ=%d form=%d entNr=%d\n",ox1.typ,ox1.form,
                     // impTab[ind].fTyp);

  if(ox1.typ == Typ_SURRU) {
    if(impTab[ind].fTyp == 128) { // degraded BSP-Sur
      // printf(" degraded 128\n");
      // die Subelemente der SRU generieren

    } else if(impTab[ind].fTyp == 122) {
      IGE_rw_122 (&ox1);

    } else {
      oxp1 = (ObjGX*)ox1.data;
      oxp2 = &oxp1[0];
      i1 = INT__PTR(oxp2->data);
      if(impTab[i1].typ == Typ_Error) goto L_err1;
        // printf(" SRU ind1=%d %d %d\n",i1,impTab[i1].typ,impTab[i1].ind);
      oxp2->typ  = impTab[i1].typ;
      if(impTab[i1].ind == 0) IGE_r_work_3 (i1);
      if(impTab[i1].ind == 0) goto L_err2;
      oxp2->data = PTR_LONG(impTab[i1].ind);
      // printf(" impTab[%d].ind1=%d\n",i1,impTab[i1].ind);

      oxp2 = &oxp1[1];
      i1 = INT__PTR(oxp2->data);
      if(impTab[i1].typ == Typ_Error) goto L_err1;
        // printf(" SRU ind2=%d %d %d\n",i1,impTab[i1].typ,impTab[i1].ind);
      oxp2->typ  = impTab[i1].typ;
      if(impTab[i1].ind == 0) IGE_r_work_3 (i1);
      if(impTab[i1].ind == 0) goto L_err2;
      oxp2->data = PTR_LONG(impTab[i1].ind);
        // printf(" impTab[%d].ind2=%d\n",i1,impTab[i1].ind);
    }


  // 190 = PLN
  } else if(ox1.typ == Typ_PLN) {
    if(impTab[ind].fTyp == 190) {
      i1 = IGE_rw_190 (&ox1);
      if(i1 < 0) return i1;
    }



  // 102 = CCV
  } else if(ox1.typ == Typ_CVTRM) {
    if((impTab[ind].fTyp == 102)   ||
       (impTab[ind].fTyp == 141))     {
      i1 = IGE_rw_102 (&ox1);
      if(i1 < 0) return i1;
    }



  // 120 = RevSur
  } else if(ox1.typ == Typ_SURRV) {
    if(impTab[ind].fTyp == 120) {
      i1 = IGE_rw_120 (&ox1);
      if(i1 < 0) return i1;
    }


  // 144 oder 143 = Begrenzte Flaeche
  } else if(ox1.typ == Typ_SUR) {
      // printf(" work_2-Typ_SUR-typ=%d\n",impTab[ind].fTyp);

    if(impTab[ind].fTyp == 143) {
      i1 = IGE_rw_143 (&ox1);
      if(i1 < 0) return i1;
      goto L_OK;   // fertig

    } if(impTab[ind].fTyp == 144) {
      i1 = IGE_rw_144 (&ox1);
      if(i1 < 0) return i1;
      goto L_OK;   // fertig

    } if(impTab[ind].fTyp == 120) {     // neu zu 2007-07-07
      i1 = IGE_rw_120 (&ox1);
      if(i1 < 0) return i1;
      goto L_OK;   // fertig

    } else if(impTab[ind].fTyp == 128) {
      // 128=BSplSurf can be planar, used for planar trimmed surface.
      // Do not output it if planar ..
        // TESTBLOCK
        // IGE_r_dump_impTab (ind);
        // DEB_exit();
        // END TESTBLOCK
      goto L_OK;   // fertig

    } else goto L_err3;



  // 408 = SubfigureInstance
  } else if(ox1.typ == Typ_Model) {
    if(impTab[ind].fTyp == 408) {
      i1 = IGE_rw_408 (&ox1);             // useless .. ??
      if(i1 < 0) return i1;
    }


  // } else {
  // typ-22 Typ_CVPSP3 OK
    // TX_Error("IGE_r_work_2 E004 %dP typ=%d fTyp=%d siz=%d\n",
            // IGE_r_ind2dNr(ind),ox1.typ,impTab[ind].fTyp,impTab[ind].siz);
  }


  L_OK:

  // mark line done
  impTab[ind].done = 1;


  //======================================================================
  return IGS_out__ (&ox1);    // save


  //-------------------------------------
  L_err1:
  TX_Error("IGE_r_work_2 E001");
  return -1;

  //-------------------------------------
  L_err2:
  TX_Error("IGE_r_work_2 E002");
  return -1;

  //-------------------------------------
  L_err3:
  TX_Error("IGE_r_work_2 E003 %dP typ=%d fTyp=%d\n",
            IGE_r_ind2dNr(ind),ox1.typ,impTab[ind].fTyp);
// exit(0);
  return -1;

}


//=====================================================================
  int IGE_r_work_3 (int ind) {
//=====================================================================
// ein phys.Dep.Obj nachtraeglich noch generieren

  int   i1;

  // printf("IGE_r_work_3 dNr=%d fTyp=%d\n",IGE_r_ind2dNr(ind),impTab[ind].fTyp);

  impTab[ind].stat23 = 0;  // sonst wirds wieder geskippt!

  i1 = impInd;
  impInd = ind;

  // if(impInd >= impTabSiz) exit(0);  // nur debug

  IGE_r_work_2 (ind);      // generieren ..

  impInd = i1;     // reset impInd

  return 0;
}


//===========================================================================
  int IGE_r_D__ (FILE *fp_in) {
//===========================================================================
// alle D-Zeilen einlesen, EntityTyp (fTyp) und P-LineNr (fInd) merken
// als Index fuer impTab wird die (D-Zeilenummer+1)/2 verwendet;
//   (es gibt pro Obj. 2 D-Zeilen).
// fTyp = EntityNr
// fInd = P-ZeilenNr (im P-Block ganz rechts)
// Die D-ZeilenNr ist somit (Index*2)-1



  int    i1;
  char   IGlnD1[88], IGlnD2[88];


  // printf(">>>>>>>>>>>>>>>>>>>>>>>> IGE_r_D__\n");


  impNr = 0;


  // vorlesen bis zur ersten D-Zeile
  L_vor_next:

  // Erste D-Zeile einlesen
  if (fgets (IGlnD1, 84, fp_in) == NULL) return -2;

  // Skip S u. G  Block
  if(IGlnD1[72] != 'D') goto L_vor_next;

  goto L_Next_;   // erste Line schon gelesen



  //========================================================
  L_NextLine:
  // printf("--------L_NextLine\n");

  // IGlnD1 = Erste D-Zeile einlesen
  if (fgets (IGlnD1, 84, fp_in) == NULL) return -2;
  if(strlen(IGlnD1) < 72) {
    TX_Error("IGES-Formatfehler E001");
    return -1;
  }


  L_Next_:
  if(impNr >= impTabSiz) {
    if(IGE_r_allocD (impNr) < 0) return -1;
  }


  if(IGlnD1[72] != 'D') goto L_fertig;

  // IGlnD2 = zweite D-Zeile einlesen
  //TX_Print(" vor read 2.D-Zeile");
  if (fgets (IGlnD2, 84, fp_in) == NULL) return -2;
    // printf("      D2=/%s/\n", IGlnD2);

  if(strlen(IGlnD2) < 72) {
    TX_Error("IGES-Formatfehler E003");
    return -1;
  }

  // somit kanns nur eine D-Zeile sein;
  if(IGlnD2[72] != 'D') {
    TX_Error("IGES-Formatfehler E004");
    return -1;
  }

  // fTyp = EntityNr (D1-Feld 0)
  impTab[impNr].fTyp = atoi(&IGlnD1[0]);

  // fInd = P-LineNr (D1-Feld 1)
  impTab[impNr].fInd = atoi(&IGlnD1[8]);

  // TraMat = D1-Feld 6
  if(IGlnD1[55] != ' ') {
    impTab[impNr].ind  = atoi(&IGlnD1[48]);   // wozu wird ind gesetzt ??
    impTab[impNr].trInd  = impTab[impNr].ind;
  } else {
    impTab[impNr].ind = 0;
    impTab[impNr].trInd  = 0;
  }
    // printf(" trInd[%d]=%d\n",impNr,impTab[impNr].trInd);

  // Form = D2-Feld 5
  impTab[impNr].form  = IMIN(atoi(&IGlnD2[32]), 255);

  // stat01 - view / blank (hide)
  impTab[impNr].stat01 = 0;
  if(IGlnD1[65] == '1') impTab[impNr].stat01 = 1;

  // stat23 - 0=unabhaengig
  impTab[impNr].stat23 = 0;
  if(IGlnD1[67] == '1') impTab[impNr].stat23 = 1;

  // // stat45
  // impTab[impNr].stat45 = IMAX((IGlnD2[69] - '0'), 0);  // Ascii-Wert !

  // printf(" IgsObj[%d] Ent=%d pNr=%d\n",impNr,
           // impTab[impNr].fTyp,impTab[impNr].fInd);

  impTab[impNr].done   = 0;
  impTab[impNr].activ  = 0;
  impTab[impNr].typ    = Typ_Error;
  impTab[impNr].ind    = 0;
  impTab[impNr].stat__ = 0;


  ++impNr;


  goto L_NextLine;




  //----------------------------------------------------
  L_fertig:



  // printf("ex IGE_r_D__ %d recs geladen\n",impNr);

  return 0;

}


//=======================================================================
  int IGE_r_allocD (long Ind) {
//=======================================================================
// realloc impTab

#define impTab_INC 1000

  long i1, newSiz;


  if((Ind == 0)&&(impTab != NULL)) {
    newSiz = impTabSiz;
    impTabSiz = 0;
    goto L_init;
  }

  newSiz = impTabSiz + impTab_INC;

  printf("::::IGE_r_allocD %ld %ld %ld %d\n",Ind,impTabSiz,newSiz,impTab_INC);

  while (Ind >= newSiz) newSiz += impTab_INC;
  // printf("  %d %d %d\n",Ind,impTabSiz,newSiz);

  impTab = (ImpObj*)realloc(impTab, newSiz*sizeof(ImpObj));

  if(impTab == NULL) {
    TX_Error ("******** out of memory - IGE_r_allocD *********");
    return -1;
  }

  L_init:
  impTabSiz = newSiz;

  // printf("ex IGE_r_allocD %d %d %d\n",Ind,impTabSiz,newSiz);

  return 0;

}


//================================================================
  int IGE_r_tra__ () {
//================================================================
// alle transformieren

  int       ii, i1, dNr, trNr, trInd;
  void      *vp1;
  Point     pt1, *pp1;
  ModelRef  *mr1;
  CurvBSpl  *cbs1;
  ObjGX     ox1, *pox1;
  

  // printf(">>>>>>>>>>>>> IGE_r_tra__ %ld\n",impNr);


  ii = 0;


  L_next:
      // printf(" impTab[%d].fTyp = %d trInd = %ld\n",ii,
               // impTab[ii].fTyp,impTab[ii].trInd);


    if(impTab[ii].trInd == 0) goto L_continue;

    trNr  = impTab[ii].trInd;
    trInd = IGE_r_dNr2ind (trNr);
    if(trInd >= impNr) {TX_Print("IGE_r_P__ E003"); goto L_continue;}
      // DEB_dump_obj__ (Typ_PLN, impTab[trInd].data, " _tra__R %d",trInd);

    UT3D_m3_loadpl (IG_trMat, impTab[trInd].data);
    vp1 = impTab[ii].data;


    //----------------------------------------------------------------
    if(impTab[ii].fTyp == ENT_ERR) {
      // unsupported object ...
      goto L_continue;


    //----------------------------------------------------------------
    } else if(impTab[ii].fTyp == 124) {  // trMat
      dNr = IGE_r_ind2dNr(ii);
      printf("Rec=%d RecTyp=124 Matrix=%d unsupported\n",dNr,trNr);
      goto L_continue;


    //----------------------------------------------------------------
    } else if(impTab[ii].fTyp == 116) {   // point
      UTO_obj_tra_obj_m3 (vp1, Typ_PT, vp1, IG_trMat);
      goto L_continue;


    //----------------------------------------------------------------
    } else if(impTab[ii].fTyp == 110) {   // Line
      UTO_obj_tra_obj_m3 (vp1, Typ_LN, vp1, IG_trMat);
      goto L_continue;


    //----------------------------------------------------------------
    } else if(impTab[ii].fTyp == 100) {   // circ
      // DEB_dump_obj__ (Typ_CI, vp1, " vor ");
      UTO_obj_tra_obj_m3 (vp1, Typ_CI, vp1, IG_trMat);
      goto L_continue;


    //----------------------------------------------------------------
    } else if(impTab[ii].fTyp == 104) {    // elli
      UTO_obj_tra_obj_m3 (vp1, Typ_CVELL, vp1, IG_trMat);
      goto L_continue;


    //----------------------------------------------------------------
    } else if(impTab[ii].fTyp == 212) {  // text
      UTO_obj_tra_obj_m3 (&((GText*)vp1)->pt, Typ_PT, &((GText*)vp1)->pt, IG_trMat);
      goto L_continue;


    //----------------------------------------------------------------
    } else if(impTab[ii].fTyp == 214) {   //  Leader / Arrrow
      UTO_obj_tra_obj_m3 (vp1, Typ_LN, vp1, IG_trMat);
      goto L_continue;


    //----------------------------------------------------------------
    } else if(impTab[ii].fTyp == 106) {    // 2D/3D-polygon
      pp1 = vp1;  // Point[]
      for(i1=0; i1<impTab[ii].siz; ++i1) {
        UT3D_pt_tra_pt_m3 (&pp1[i1], IG_trMat, &pp1[i1]);
      }
      goto L_continue;



    //----------------------------------------------------------------
    } else if(impTab[ii].fTyp == 108) {     // plane
      UTO_obj_tra_obj_m3 (vp1, Typ_PLN, vp1, IG_trMat);
      goto L_continue;


    //----------------------------------------------------------------
    } else if(impTab[ii].fTyp == 112) {     // Parametric Spline Typ_CVPSP3
      ox1.typ  = impTab[ii].typ;
      ox1.form = impTab[ii].form;
      ox1.siz  = impTab[ii].siz;
      ox1.data = impTab[ii].data;
      pox1 = &ox1;
      // UTO_ox_tra (&pox1, &ox1, IG_trMat);
      CVPSP_psp3_tra_m3 (&ox1, &ox1, IG_trMat);
      goto L_continue;



    //----------------------------------------------------------------
    } else if(impTab[ii].fTyp == 126) {  // Rational B-Spline
      cbs1 = vp1;
      // if((cbs1->deg == 1)&&(cbs1->ptNr == 2)) {        // Line !!
      if(impTab[ii].typ == Typ_LN) {        // Line !!
        UTO_obj_tra_obj_m3 (vp1, Typ_LN, vp1, IG_trMat);
        goto L_continue;
      }
      for(i1=0; i1<cbs1->ptNr; ++i1) {
        pp1 = &cbs1->cpTab[i1];
        UT3D_pt_tra_pt_m3 (pp1, IG_trMat, pp1);
      }
      goto L_continue;



    //----------------------------------------------------------------
    } else if(impTab[ii].fTyp == 408) {   // SubfigureInst
      mr1 = vp1;
      // Nullpunkt addieren
      UT3D_m3_get ((void*)&pt1, 3, IG_trMat);
      mr1->po.x += pt1.x;
      mr1->po.y += pt1.y;
      mr1->po.z += pt1.z;
      // Z u X-Vec uebernehmen
      UT3D_m3_get ((void*)&mr1->vx, 0, IG_trMat);
      UT3D_m3_get ((void*)&mr1->vz, 2, IG_trMat);
      // mr1.vx = ?? // IG_trMat
      // UTO_obj_tra_obj_m3 ((void*)&p1, Typ_PT, (void*)ra, IG_trMat);
      goto L_continue;


    }


    //----------------------------------------------------------------
    L_err1:
      if(IGE_r_ck_skip(impTab[ii].fTyp) != 0) goto L_continue;
      dNr = IGE_r_ind2dNr(ii);
      TX_Print("***** Error: IGE_r_tra__ D-LineNr=%d Typ=%d",
               dNr,impTab[ii].fTyp);


    //----------------------------------------------------------------
    L_continue:
      ++ii;
      if(ii < impNr) goto L_next;




  //================================================================
  L_done:


  return 0;

}


//===========================================================================
  int IGE_r_P__ (FILE *fp_in) {
//===========================================================================
// alle P-Zeilen einlesen; erste ist schon in ???

  double  *IG_ra;
  int     IG_ra_siz, IG_typ_act;

  int     i1, irc;
  long    l1, l2;
  // void    *op1;
  ObjGX   ox1;


  // printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> IGE_r_P__ %ld\n",impNr);



  // Init Speicherbereiche
  IG_ra = (void*) memspc51;
  IG_ra_siz = sizeof(memspc51) / sizeof(double);
  // printf("IG_ra_siz=%d\n",IG_ra_siz);


  // rewind (weil erste Zeile schon eingelesen wurde)
  rewind (fp_in);


  //--------------------------------------------------------
  L_next_rec:
  // if(impNr >= impTabSiz) {
    // if(IGE_r_allocP (impNr) < 0) return -1;
  // }


  // naechsten P-Record einlesen (alle Werte -> IG_ra)
  IG_dNr = IGE_r_getP_ (&IG_typ_act, IG_ra, IG_ra_siz, fp_in);
  if(IG_dNr < -1) return IG_dNr;
  if(IG_dNr < 0) goto L_work;


  // stimmt IG_typ_act ?   IG_dNr ist die lNR
  impInd = IGE_r_dNr2ind (IG_dNr);
    // printf(" typ=%d impInd=%d IG_dNr=%d\n",IG_typ_act,impInd,IG_dNr);
  if(impInd >= impNr) {
    TX_Error("IGE_r_P__ E001");
    // if(impInd >= impTabSiz) exit(0);  // nur debug
    goto L_weiter;
  }


  // printf(" .fTyp[%d] = %d\n",impInd,impTab[impInd].fTyp);
  if(impTab[impInd].fTyp != IG_typ_act) {
    TX_Error("IGE_r_P__ E002 %d",IG_dNr);
    goto L_weiter;
  }

/* 2007-07-06      ausgelagert nach IGE_r_tra__
  // transformiert wird sofort beim decodieren; 
  IG_trNr = impTab[impInd].trInd;
    printf(" impInd=%d IG_trNr=%d\n",impInd,IG_trNr);
  if(IG_trNr != 0) {             // IG_trMat laden
    // index from IG_dNr
    IG_dNr = IGE_r_dNr2ind (IG_trNr);
    if(IG_dNr >= impNr) {
      TX_Error("IGE_r_P__ E003");
      // printf(" impNr=%d impInd=%d IG_trNr=%d\n",impNr,impInd,IG_trNr);
      goto L_weiter;
    }
    // printf("  >>> IG_trNr=%d i=%d\n",IG_trNr,IG_dNr);
    // CRASH wenn trMat noch nicht geladen !
    if(impTab[IG_dNr].data != NULL) {
      DEB_dump_obj__ (Typ_PLN, impTab[IG_dNr].data, "_124");
      // Plane -> Mat_4x3
      UT3D_m3_loadpl (IG_trMat, impTab[IG_dNr].data);
    }
  }
*/


  // we skip this entities:
  if(IGE_r_ck_skip (IG_typ_act) != 0) goto L_next_rec;


  // decode Data. Die folgenden Funktionen liefern ein ObjGX ox1 mit den
  // P-Record-Daten; diese werden dann -> impSpc gespeichert.

  //---- Point; Typ 116 ------
  if(IG_typ_act == 116) {
    irc = IGE_r_116 (&ox1, IG_ra);


  //---- Vector; Typ 123 ------
  } else if (IG_typ_act == 123) {
    irc = IGE_r_123 (&ox1, IG_ra);


  //---- Line; Typ 110 ------
  } else if (IG_typ_act == 110) {
    irc = IGE_r_110 (&ox1, IG_ra);


  //---- Circ; Typ 100 ------
  } else if (IG_typ_act == 100) {
    irc = IGE_r_100 (&ox1, IG_ra);


  //---- TrMat = Plane; Typ 124 ------
  } else if (IG_typ_act == 124) {
    irc = IGE_r_124 (&ox1, IG_ra);


  //---- Ellipse; Typ 104 ------
  } else if (IG_typ_act == 104) {
    irc = IGE_r_104 (&ox1, IG_ra);
    if(irc < 0) goto L_weiter;


  //---- Copious Data; Typ 106
  } else if (IG_typ_act == 106) {
    irc = IGE_r_106 (&ox1, IG_ra);
    if(irc < 0) goto L_weiter;


  //---- Text; Typ 212 ------
  } else if (IG_typ_act == 212) {
    irc = IGE_r_212 (&ox1, IG_ra);
    if(irc < 0) goto L_weiter;


  //---- Leader; Typ 214 ------
  } else if (IG_typ_act == 214) {
    irc = IGE_r_214 (&ox1, IG_ra);
    if(irc < 0) goto L_weiter;


  //---- Parametric Spline Curve; Typ 112
  } else if(IG_typ_act == 112) {
    irc = IGE_r_112 (&ox1, IG_ra);
    if(irc < 0) goto L_weiter;


  //---- Rational B-Spline Curve; Typ 126
  } else if (IG_typ_act == 126) {
    irc = IGE_r_126 (&ox1, IG_ra);
    if(irc < 0) goto L_weiter;


  //---- CCV; Typ 102 ------
  } else if (IG_typ_act == 102) {
    irc = IGE_r_102 (&ox1, IG_ra, IG_ra_siz);
    if(irc < 0) goto L_weiter;


  //---- CCV; Typ 142 ------
  } else if (IG_typ_act == 141) {
    irc = IGE_r_141 (&ox1, IG_ra);
    if(irc < 0) goto L_weiter;


  //---- CCV; Typ 142 ------
  } else if (IG_typ_act == 142) {
    irc = IGE_r_142 (&ox1, IG_ra);
    if(irc < 0) goto L_weiter;



  //----------------------------------------------------------------
  //---- Plane; Typ 108 ------
  } else if (IG_typ_act == 108) {
    // // UG: skip this Plane if stat45 = 01 (Annotation)
    // printf("plan: stat45=%d\n",impTab[impNr].stat45);
    // if(impTab[impNr].stat45 == 1) return 0;
    irc = IGE_r_108 (&ox1, IG_ra);
    if(irc < 0) goto L_weiter;


  } else if (IG_typ_act == 190) {
    irc = IGE_r_190 (&ox1, IG_ra);
    if(irc < 0) goto L_weiter;


  //---- RuledSurface; Typ 118
  } else if (IG_typ_act == 118) {
    irc = IGE_r_118 (&ox1, IG_ra);
    if(irc < 0) goto L_weiter;


  //---- Zylinderflaeche; Typ 122
  } else if (IG_typ_act == 122) {
    irc = IGE_r_122 (&ox1, IG_ra);
    if(irc < 0) goto L_weiter;


  //---- Surface of Revolution; Typ 120
  } else if (IG_typ_act == 120) {
    irc = IGE_r_120 (&ox1, IG_ra);
    if(irc < 0) goto L_weiter;


  //---- Rational B-SplineSurf; Typ 128
  } else if (IG_typ_act == 128) {
    irc = IGE_r_128 (&ox1, IG_ra);
    if(irc < 0) goto L_weiter;
    if(irc == 1) impTab[impInd].stat__ = 1; // rbspl-curve is planar


  //---- BoundedSurf; Typ 143 ------
  } else if (IG_typ_act == 143) {
    irc = IGE_r_143 (&ox1, IG_ra);
    if(irc < 0) goto L_weiter;


  //---- TrimmedSurf; Typ 144 ------
  } else if (IG_typ_act == 144) {
    irc = IGE_r_144 (&ox1, IG_ra);
    if(irc < 0) goto L_weiter;



  //----------------------------------------------------------------
  //---- SubfigureDef; Typ 308 ------
  } else if (IG_typ_act == 308) {
    irc = IGE_r_308 (&ox1, IG_ra);
    if(irc < 0) goto L_weiter;


  //---- SubfigureInst; Typ 408 ------
  } else if (IG_typ_act == 408) {
    irc = IGE_r_408 (&ox1, IG_ra);
    if(irc < 0) goto L_weiter;






  //--------------------------------
  } else {

    // TX_Error("IGE_r_write_ skip typ %d\n",IG_typ_act);
    printf("***** Error: IGE_r_P__ skip D-Line %d Typ %d\n",IG_dNr,IG_typ_act);
    if(IG_typ_act == 186) { TX_Print(
  "Solid-Shell-Representation not yet supported; use Surface-Representation");
    }
    goto L_weiter;
  }


  // check size of memseg; muss mind 50K groesser sein
  l1 = UME_ck_free (&impSpc);
  if(l1 < impSpc_INC) {
    // UME_dump (&impSpc, "Dump impSpc: ");
    l1 = UME_ck_tot (&impSpc) + (impSpc_INC * 3);
    i1 = UME_realloc (&l2, &impSpc, l1);
    if(i1 < 0) return -1;
  }


  // add obj aus ox1 -> memspc
    // printf("  sav dat impInd=%ld typ=%d siz=%d\n",impInd,ox1.typ,ox1.siz);
  impTab[impInd].typ  = ox1.typ;
  impTab[impInd].form = ox1.form;
  impTab[impInd].siz  = ox1.siz;
  impTab[impInd].data = UTO_obj_save (&impSpc, &ox1);
  



  //--------------------------------------------------------
  L_weiter:
  if(irc == -4) return irc;


  // TESTAUGABE-------------------------
  // char cbuf[64]; sprintf(cbuf, "i%d",impInd); UME_dump (&impSpc, cbuf);
  // if(impInd == 3) {
    // IGE_r_dump_impTab (impInd);
    // // // DEB_dump_ox_0 (&ox1, "iI=2224");
    // DEB_dump_obj__ (impTab[impInd].form, impTab[impInd].data, "iI=2224");
    // exit(0);
  // }
  // if(impInd > 2224) {
    // i1 = 2224;
    // DEB_dump_obj__(Typ_PT,&((CurvBSpl*)impTab[i1].data)->cpTab[0],"2224.p0");
  // }
  // TESTAUGABE-------------------------


  goto L_next_rec;



  //--------------------------------------------------------
  L_work:

    // TESTBLOCK
    // printf(" ex-IGE_r_P__ impInd=%ld impNr=%ld\n",impInd,impNr);
    // IGE_r_dump_impTab (0);
    // END TESTBLOCK


  return 0;

}


//=====================================================================
  int IGE_r_getP_ (int *IG_typ_act, double ra[], int ra_siz, FILE *fp_3) {
//=====================================================================
// Hole alle Parameter der naechsten P - Zeile linnr. into ra.
// RetCod -1: End of P-Block; OK.
// RetCod -2: End of File; Error !


  int    i1, lNr, lnSiz, actSiz;
  char   line[84];
  char   *IGlnP;

  // ---------------------------------------------------------
  // printf("ige_r_getP_\n");

 
  // lnSiz    = sizeof(memspc52);
  // IGlnP    = memspc52;
  lnSiz    = mem_cbuf1_SIZ;
  IGlnP    = mem_cbuf1;


  IGlnP[0] = '\0';
  actSiz   = 0;
    // printf(" lnSiz=%d\n",lnSiz);

  NoAmoi:

    if (fgets (line, 84, fp_3) == NULL) {
      TX_Error("IGE_r_getP_ EOF - ERROR\n");
      return -2;
    }
    // printf(" next=|%s|\n",line);

    // skip alles bis zum P-Block
    if(line[72] == 'T') {
      // printf(" normal eof\n");
      return -1;
    }

    // vorlesen bis zum P-Block
    if(line[72] != 'P') goto NoAmoi;

    // Zeilenrest weg ..
    line[72] = '\0';
    lNr = atoi(&line[64]);   // die P-Line-Nr
    line[64] = '\0';

    UTX_CleanCR (line);       // remove blanks & tabs ..

    i1 = strlen(line);
    actSiz += i1;
    if(actSiz >= lnSiz) {TX_Error("IGE_r_getP_ E001"); return -2;}
    strcat(IGlnP, line);

    // printf(" line=|%s|\n",line);
    // printf("           i1=%d c=/%c/\n",i1,line[i1-1]);


    // nun noch last Char pruefen; wenn != ';' dann gibts Fortsetzung.
    if(line[i1-1] != ';') {
      // IGE_test (actSiz);
      goto NoAmoi;
    }


    *IG_typ_act = IGE_r_decodeP_ (ra, ra_siz, IGlnP);

    // Testausg. kompletterTextblock
    // printf("ex ige_r_getP_|%s|\n",IGlnP);
    // printf("ex IGE_r_getP_ typ=%d lNr=%d\n",*IG_typ_act,lNr);


    return lNr;
}


//=====================================================================
  int IGE_r_decodeP_ (double ra[], int raSiz, char *cbuf) {
//=====================================================================
//  Decodiere alle Parameter der aktuellen P - Zeile into ra.


  int  parInd, i1, IG_typ_act;
  char *ip1, *ip2;

  //--------------------------------------------------------
  // printf("ige_r_decodePP |%s|\n",cbuf);


  for (parInd=0; parInd < raSiz; ++parInd) ra[parInd] = 0.0;

  parInd=0;


  // 1. Wort = EntityNr.
  IG_typ_act = atoi(cbuf);
  // ip1 = &cbuf[4];
  ip2 = &cbuf[4];


  // die ersten raSiz Paramter into ra einlesen
  while (parInd < raSiz)
  {
    // printf(" next [%d]=|%s|\n",parInd,ip1);
    // ra[parInd] = strtod(ip1,&ip2);
    ra[parInd] = UTX_db_tx (&ip2, ip2);
    // printf(" ra[%d]=%f ip2=|%c|\n",parInd,ra[parInd],*ip2);


    // ist es ein Text ? zB "4HTest"
    if(*ip2 == 'H') {
      ++ip2;
      i1 = ra[parInd];
      // printf(" %d chars Text!\n",i1);
      strncpy(memspc55, ip2, i1);
      memspc55[i1] = '\0';
      ip2 += i1;
    }


    if(*ip2 == '\0') break;
    // if(*ip2 == '\r') break;
    // if(*ip2 == '\n') break;
    if(*ip2 == ';') break;        // ; ist  End Of Line !


    // printf("  ra[%d] = %f\n",parInd, ra[parInd]);
    // ip1 = ip2; ++ip1;              // skip ","
    ++ip2;
    parInd++;
  }

    // printf("ex IGE_r_decodeP_ %d %d\n",IG_typ_act,parInd);

  return IG_typ_act;
}


//================================================================
  int IGE_r_line (char *cbuf, int bufMax, char lTyp, FILE *fp_in) {
//================================================================
// eine (mehrzeile) Zeile einlesen, die mit ';' terminiert ist.
// Fuer S, G, P-Zeilen; nicht fuer D-Zeilen.
// Dzt macht IGE_r_getP_ das concat selbst !

  int    i1, bufSiz;
  char   *line, rTyp;


  // printf("IGE_r_line %c %d\n",lTyp,bufMax);

  bufSiz = 0;


  //----------------------------------------------------------------
  NoAmoi:
  line = &cbuf[bufSiz];
  

  if((bufSiz + 85) > bufMax) {
    TX_Error("IGE_r_line E001");
    return -2;
  }


  if (fgets (line, 84, fp_in) == NULL) {
    TX_Error("IGE_r_line E002");
    return -2;
  }
    // printf(" next=|%s|\n",line);


  // S-Zeile: nur eine Zeile einlesen, (obwohl S mehrzeilig);
  // beim G-Lesen dann alle weiteren S-zeilen skippen !
  if(lTyp == 'S') return 0;


  rTyp = line[72];

  if(rTyp != 'G') return 0;  // hat aber nun erste D-Zeile gefressen !!


  if(rTyp == 'T') {  // f P noch unused;fuer T gehts nicht !
    TX_Error("IGE_r_line E003");
    return -2;
  }

  // skip additional S-Lines before G
  if(rTyp != lTyp) goto NoAmoi;


  // gesuchte Zeile gefunden; zusammenfuegen.
  line[72] = '\0';           // Zeilenrest weg

  // remove blanks geht nicht, da in Kommentaren ("1H,,") ',' oder ';'sind !
  // remove following blanks, 
  // ( ##H.. hat auch blanks !)
  // UTX_CleanCR (line);
  // for(i1=71; i1>2; --i1) {
    // if(line[i1] == ' ') continue;
    // if((line[i1] == ',')||(line[i1] == ';')) {
      // line[i1] = '\0';
    // }
    // break;
  // }
  // i1 = strlen(line);
  i1 = 72;

  // printf(" r_line %c %d %c\n",line[i1-1],i1,rTyp);
  if(line[i1-1] != ';') {
    bufSiz += i1;
    goto NoAmoi;
  }



  //--------------------------------------------------------
  // printf("ex IGE_r_line |%s|\n",cbuf);

  return 0;

}


//=======================================================================
  int IGE_r_G__ (double *modSiz, FILE *fp_in) {
//=======================================================================
// Read G-Zeile; get ModelSize

  int    bufSiz, i1, iPos;
  char   *cbuf;
  double d1, d2;

  cbuf = memspc51;
  bufSiz = sizeof(memspc51);


  // komplette Zeilen einlesen ...
  // erste Zeile ist S, darauf folgt G-Zeile
  IGE_r_line (cbuf, bufSiz, 'S', fp_in);
  IGE_r_line (cbuf, bufSiz, 'G', fp_in);
  // printf(" G-Line=|%s|\n",cbuf);


  // die G-Zeile decodieren;
  // used werden Zeilen 18 & 19
  // printf("G-Line:|%s|\n",cbuf);
  // cp = strtok (cbuf1, ",");
  iPos = 0;
  for(i1=0; i1<21; ++i1) {
    // hier liegt wort # i1 an ..
    // printf(" g-word %d %d |%20.20s|\n",i1,iPos,&cbuf[iPos]);
    if(i1 == 18) {
      d1 = atof (&cbuf[iPos]);  // Resolution   = word # 18
    } else if(i1 == 19) {
      d2 = atof (&cbuf[iPos]);  // Max.Coord.Value  = word # 19
    }
    // skip this word
    IGE_r_skip_wd (&iPos, cbuf);
    // nun zeigt iPos auf ',' od ;
    if(cbuf[iPos] == ';') return -1;
    ++iPos;
  }
  *modSiz = d1 * d2 * 50.;
  printf(" ModSiz=%f %f %f\n",*modSiz,d1,d2);

  return 0;

}


//================================================================
  int IGE_r_skip_wd (int *iPos, char *cbuf) {
//================================================================
// skip word; ipos zeigt dann auf den zugehoerigen Delimiter.
// zB "1H,," oder "12,"

  int  i1;
  char *p1, *p2;


  // printf("IGE_r_skip_wd %d |%s|\n",*iPos,cbuf);


  if(isdigit(cbuf[*iPos])) {
    p1 = &cbuf[*iPos];
    while(isdigit(cbuf[*iPos])) ++(*iPos);
    if(cbuf[*iPos] == 'H') {
      i1 = atoi(p1);
      *iPos += i1 + 1;  // plus 'H'
      // printf(" i1=%d |%s|\n",i1,&cbuf[*iPos]);
      // return 0;
    }
  }


  // while(cbuf[*iPos] != ',') ++(*iPos);
  p1 = &cbuf[*iPos];
  p2 = strchr(p1, ',');
  if(p2 == NULL) {
  *iPos += strlen(cbuf);
    return -1;
  }

  *iPos += p2 - p1;

  return 0;

}


//=======================================================================
  int IGE_r_DmaxPmax (long *dSiz, long *pSiz, FILE *fp_in) {
//=======================================================================
// Anzahl D-zeilen und Anzahl P-Zeilen aus igesfile/letzte zeile einlesen

  char  cBuf[84];

  // find T-Line
  NoAmoi:

    if (fgets (cBuf, 84, fp_in) == NULL) {
      TX_Error("IGE_r_DmaxPmax EOF - ERROR\n");
      return -2;
    }
    // skip alles bis zum P-Block
    if(cBuf[72] != 'T') goto NoAmoi;



  // abtrennen
  cBuf[24] = '\0';
  cBuf[32] = '\0';

  // D-lineAnzahl 17-23
  *dSiz = atoi(&cBuf[17]);

  // P-lineAnzahl 25-31
  *pSiz = atoi(&cBuf[25]);

  rewind (fp_in);

  printf("ex IGE_r_DmaxPmax %ld %ld\n",*dSiz,*pSiz);

  return 0;
}


//=======================================================================
  int IGE_r_ind2dNr (int ind) {
//=======================================================================
// get D-lineNr from impTab-index
// Die D-ZeilenNr ist somit (Index*2)-1
// 0->1, 1->3, 2->5, ..

  int DLNr;

  DLNr = (ind * 2) + 1;

  return DLNr;

}


//=======================================================================
  int IGE_r_dNr2ind (int DLNr) {
//=======================================================================
// 1->0, 3->1, 5->2, ..

  int ind;

  ind = (DLNr - 1) / 2;

  return ind;

}


//=======================================================================
  int IGE_r_ck_skip (int typ_act) {
//=======================================================================
// check if entity is supported
// 0 = yes, -1 = no.



  if(typ_act < 100) return -1;      // skip 0-99 (0=Property !?)
  if(typ_act < 191) return 0;
  if(typ_act < 212) return -1;      // skip 192-211
  if(typ_act < 215) return 0;       // use 212,214 impl.
  if(typ_act < 308) return -1;      // skip 215-307
  if(typ_act == 308) return 0;      // use 308
  if(typ_act == 408) return 0;      // use 408
  return -1;

/*
  // we skip this entities:
  if((typ_act ==   0)  ||   // Property
     (typ_act == 192)  ||   // CircularCylinder Surf.
     (typ_act == 194)  ||   // CircularConic Surf.
     (typ_act == 196)  ||   // Sphaerical Surf.
     (typ_act == 198)  ||   // Toroidal Surf.
     (typ_act == 202)  ||   // Angular Dimension
     (typ_act == 206)  ||   // Diameter Dimension
     (typ_act == 210)  ||   // General Label
     (typ_act == 216)  ||   // Linear Dimension
     (typ_act == 218)  ||   // Ordinate Dimension
     (typ_act == 222)  ||   // Radius Dimension
     (typ_act == 228)  ||   // General Symbol
     (typ_act == 230)  ||   // sectioned area (hatch)
     (typ_act == 314)  ||   // Colour
     (typ_act == 402)  ||   // Set
     (typ_act == 404)  ||   // Drawg
     (typ_act == 406)  ||   // Property
     (typ_act == 410))    { // View

    return -1;
  }

  return 0;
*/
}


//================================================================
  int IGE_r_dump_impTab  (int ind) {
//================================================================
// IGE_r_dump_impTab        dump einen impTab-Record
// Input:
//   ind   impTabindex impTab; 

  int    i1;
  char   oid[40];

  printf("IGE_r_dump_impTab %d ======================= \n",ind);


  // printf(" D-Nr[%d]= %dP\n",ind,IGE_r_ind2dNr(ind));

  // so sieht die P-Zeile aus:
  printf("D%d P%d  %d,..\n",IGE_r_ind2dNr(ind),impTab[ind].fInd,impTab[ind].fTyp);

  printf(" fTyp=%d fInd=%d typ=%d form=%d ind=%ld siz=%d\n",
         impTab[ind].fTyp, impTab[ind].fInd,
         impTab[ind].typ, impTab[ind].form, impTab[ind].ind, impTab[ind].siz);

  // DEB_dump_obj__ (impTab[ind].form, impTab[ind].data, " [%d].data\n",ind);
  if(impTab[ind].form == Typ_Index) {
    for(i1=0;i1<impTab[ind].siz; ++i1) {
      APED_oid_dbo__ (oid, impTab[ind].typ, impTab[ind].ind);
      printf(" [%d] %s\n",i1,oid);
    }

  } else if(impTab[ind].form == Typ_Txt) {
    printf(" |%s| \n",(char*)impTab[ind].data);


  } else {
    DEB_dump_nobj__ (impTab[ind].form, impTab[ind].siz, impTab[ind].data, "");
  }

  return 0;

}



//=====================================================================
//=====================================================================
//=====================================================================
//=====================================================================


//=====================================================================
  int IGE_r_123 (ObjGX *el, double *ra) {
//=====================================================================
// Point

  static Vector  vc1;

  // printf("IGE_r_123 %f,%f,%f\n",ra[0],ra[1],ra[2]);

  vc1.dx = ra[0];
  vc1.dy = ra[1];
  vc1.dz = ra[2];

  // *el = UTO_stru_2_obj (Typ_VC, Typ_VC, &vc1);
  OGX_SET_OBJ (el, Typ_VC, Typ_VC, 1, &vc1);

  return 0;

}



//=====================================================================
  int IGE_r_116 (ObjGX *el, double *ra) {
//=====================================================================
// Point

  static Point   p1;


  // printf("IGE_r_116 %f,%f,%f\n",ra[0],ra[1],ra[2]);

  p1.x = ra[0];
  p1.y = ra[1];
  p1.z = ra[2];

  // *el = UTO_stru_2_obj (Typ_PT, Typ_PT, &p1);
  OGX_SET_OBJ (el, Typ_PT, Typ_PT, 1, &p1);

  return 0;

}


//=====================================================================
  int IGE_r_110 (ObjGX *el, double *ra) {
//=====================================================================

  int    irc;
  static Line  ln1;

  // printf("IGE_r_110\n");


  // el.typ  = Typ_LN;
  ln1.p1.x = ra[0];
  ln1.p1.y = ra[1];
  ln1.p1.z = ra[2];

  ln1.p2.x = ra[3];
  ln1.p2.y = ra[4];
  ln1.p2.z = ra[5];


/* 2007-07-06      ausgelagert nach IGE_r_tra__
  // Transformation ?
  if(IG_trNr != 0) {
    UTO_obj_tra_obj_m3 ((void*)&ln1, Typ_LN, (void*)&ln1, IG_trMat);
  }
*/


/*
  // wenn CCV aktiv und was im PtBuffer: Line zu Polygon dazuaddieren.
  irc = IGE_r_decod_ccvl (&ln1.p1, &ln1.p2);
  if(irc == 0) return 0;
*/


  // *el = UTO_stru_2_obj (Typ_LN, Typ_LN, &ln1);
  OGX_SET_OBJ (el, Typ_LN, Typ_LN, 1, &ln1);

  // DEB_dump_obj__(Typ_LN, &ln1, "ex IGE_r_110 tr=%d\n",IG_trNr);

  return 0;

}


//=====================================================================
  int IGE_r_100 (ObjGX *el, double *ra) {
//=====================================================================

  static Circ    ci1;

  // printf("IGE_r_100\n");

  // Kreis -> ci1
  IGE_r_circ (&ci1, ra);


/* 2007-07-06      ausgelagert nach IGE_r_tra__
  // Transformation (D-Zeile Wert 7)?
  if(IG_trNr != 0) {
    // printf("    vz=%f,%f,%f\n",IG_trMat[0][2],IG_trMat[1][2],IG_trMat[2][2]);
    UTO_obj_tra_obj_m3 ((void*)&ci1, Typ_CI, (void*)&ci1, IG_trMat);
  }
*/



  // wenn CCV aktiv und was im PtBuffer: Line zu Polygon dazuaddieren.
  // irc = IGE_r_decod_ccvc (&ci1, trNr, zVal, IG_trMat);
  // if(irc == 0) return 0;


  // *el = UTO_stru_2_obj (Typ_CI, Typ_CI, &ci1);
  OGX_SET_OBJ (el, Typ_CI, Typ_CI, 1, &ci1);

  return 0;

}


//=====================================================================
  int IGE_r_circ (Circ *ci1, double *ra) {
//=====================================================================
// nur einen Kreis aus ra kopieren; transformation spaeter.


  ci1->pc.x = ra[1];
  ci1->pc.y = ra[2];
  ci1->pc.z = ra[0];

  ci1->p1.x = ra[3];
  ci1->p1.y = ra[4];
  ci1->p1.z = ra[0];

  ci1->p2.x = ra[5];
  ci1->p2.y = ra[6];
  ci1->p2.z = ra[0];

  ci1->vz = UT3D_VECTOR_Z;


  // den Radius messen
  ci1->rad = UT3D_len_2pt (&ci1->pc, &ci1->p1);

  // DEB_dump_obj__ (Typ_AC, ci1, "ex IGE_r_circ:\n");

  return 0;

}

//=====================================================================
  int IGE_r_124 (ObjGX *ox1, double *ra) {
//=====================================================================
// decode Plane    war IG_r_read_trMat

  static Plane        pl1;

  Point   pt1;
 

  // printf("IGE_r_124\n");


  UT3D_vc_3db (&pl1.vx, ra[0], ra[4], ra[8]);
  UT3D_vc_3db (&pl1.vy, ra[1], ra[5], ra[9]);
  UT3D_vc_3db (&pl1.vz, ra[2], ra[6], ra[10]);

  UT3D_pt_3db (&pt1, ra[3], ra[7], ra[11]);

  UT3D_pl_ptpl (&pl1, &pt1);



  // kein Ausgabeobjekt, nix tun.
  ox1->typ      = Typ_Done;
  ox1->form     = Typ_PLN;
  ox1->siz      = 1;
  ox1->data     = &pl1;

  // DEB_dump_obj__ (Typ_PLN, &pl1, "ex IGE_r_124:\n");

  return 0;

}


//=====================================================================
  int IGE_r_104 (ObjGX *ox, double *ra) {
//=====================================================================
// decode Ellipse

  static CurvElli     el1;

  int          irc, i1;
  double       valZ;
  polcoeff_d5  co5;
  Point2       pa2, pe2;



  // printf("IGE_r_104 D-LineNr=%d form=%d\n",IG_dNr,impTab[impInd].form);


  //decode form; 1=ellipse, 2=hyperbel, 3=parabel.
  if(impTab[impInd].form != 1) {
    printf("***** Error: IGE_r_104 Conic Arc - Form %d not supported\n",
                                impTab[impInd].form);
    impTab[impInd].fTyp = ENT_ERR;     // unsupported object
    return -2;
  }


  co5.a = ra[0];
  co5.b = ra[1];
  co5.c = ra[2];
  co5.d = ra[3];
  co5.e = ra[4];
  co5.f = ra[5];
    // printf(" co5 %f %f %f %f %f\n",co5.a,co5.b,co5.c,co5.d,co5.e,co5.f);

  // Z-Plane:
  valZ  = ra[6];
    // printf(" valZ %f\n",valZ);

  // Start/EndPt in 2D-Zentrumslage:
  pa2.x = ra[7];
  pa2.y = ra[8];

  pe2.x = ra[9];
  pe2.y = ra[10];

    // printf(" pa %f %f\n",pa2.x,pa2.y);
    // printf(" pe %f %f\n",pe2.x,pe2.y);


  irc = UT3D_el_elcoe (&el1, &co5, &pa2, &pe2, valZ);
  if(irc < 0) return -2;


    // DEB_dump_obj__ (Typ_CVELL, &el1, "vor UTO_obj_tra_obj_m3\n");


/* 2007-07-06      ausgelagert nach IGE_r_tra__
  // Transformation ?
  if(IG_trNr != 0) {
    UTO_obj_tra_obj_m3 ((void*)&el1, Typ_CVELL, (void*)&el1, IG_trMat);
  }
*/

    // DEB_dump_obj__ (Typ_CVELL, &el1, "nach UTO_obj_tra_obj_m3\n");

  // *ox = UTO_stru_2_obj (Typ_CVELL, Typ_CVELL, &el1);
  OGX_SET_OBJ (ox, Typ_CVELL, Typ_CVELL, 1, &el1);


  // printf("ex IGE_r_110 %d\n",IG_ia1[1]);
  return 0;

}


//=====================================================================
  int IGE_r_190 (ObjGX *ox1, double *ra) {
//=====================================================================
// decode Plane

  static double dTab[3];

  // printf("IGE_r_190 dNr=%d %f %f %f\n",IG_dNr,ra[0],ra[1],ra[2]);


  // Werte nur zwischenspeichern
  dTab[0] = ra[0];  // Pointer to Origin (Point)
  dTab[1] = ra[1];  // Pointer to Z-Axis (Vector)

  // Pointer to X-Axis (Vector)
  if(impTab[impInd].form == 0) {
    dTab[2] = -1.;
  } else {
    dTab[2] = ra[2];
  }


  ox1->typ  = Typ_PLN;
  ox1->form = Typ_Val;
  ox1->siz  = 3;
  ox1->data = (void*)dTab;

  return 0;

}


//=====================================================================
  int IGE_rw_190 (ObjGX *ox1) {
//=====================================================================

static Plane      pl1;

  int        iori, dori, ivz, dvz, ivx, dvx;
  double     *dTab;


  // 1.word = P-Index_of_Origin
  // 2.word = P-Index_of_Z-Axis
  // 3.word = P-Index_of_X-Axis (neg: no X-Axis given).

  
  dTab = ox1->data;

  dori = dTab[0];
  dvz  = dTab[1];
  dvx  = dTab[2];

  // printf("IGE_rw_190  (%d %d %d)\n",dori,dvz,dvx);


	// get origin
  iori = IGE_r_dNr2ind (dori);
  if(impTab[iori].ind == 0) {
    IGE_r_work_3 (iori);
    if(impTab[iori].ind == 0) {TX_Print("IGE_rw_190 E001"); return -2; }
  }
    // DEB_dump_obj__ (Typ_PT, impTab[iori].data, "_rw_190-%d-ori\n",dori);


  // get Z-Axis
  ivz = IGE_r_dNr2ind (dvz);
  if(impTab[ivz].ind == 0) {
    IGE_r_work_3 (ivz);
    if(impTab[ivz].ind == 0) {
      TX_Print("IGE_rw_190 E002 %dP",dvz);
      return -2;
    }
  }
    // DEB_dump_obj__ (Typ_VC, impTab[ivz].data, "_rw_190-%d-vz\n",dvz);


  // get X-Axis
  if(dvx >= 0) {
    ivx = IGE_r_dNr2ind (dvx);
    if(impTab[ivx].ind == 0) {
      IGE_r_work_3 (ivx);
      if(impTab[ivx].ind == 0) {TX_Print("IGE_rw_190 E003"); return -2; }
    }
      // DEB_dump_obj__ (Typ_VC, impTab[ivx].data, "_rw_190-%d-vx\n",dvx);
  }


  // pl1 = (void*)IG_cBuf;


  if(dvx >= 0) {
    // create Plane from origin, Z-vec and X-vec.
    UT3D_pl_pto_vcx_vcz (&pl1, impTab[iori].data, impTab[ivx].data,impTab[ivz].data);


  } else {
    // create Plane from origin and Z-vec
    UT3D_pl_ptvc (&pl1, impTab[iori].data, impTab[ivz].data);

  }
    // DEB_dump_obj__ (Typ_PLN, pl1, "_rw_190\n");


  // *ox1 = UTO_stru_2_obj (Typ_PLN, Typ_PLN, pl1);
  OGX_SET_OBJ (ox1, Typ_PLN, Typ_PLN, 1, &pl1);

  return 0;

}


//=====================================================================
  int IGE_r_108 (ObjGX *el, double *ra) {
//=====================================================================
// decode Plane

  static Plane        pl1;

  int          irc;
  Point        pt1;
  polcoeff_d3  co3;


  co3.a = ra[0];
  co3.b = ra[1];
  co3.c = ra[2];
  co3.d = ra[3];

  // 4 = pointer to boundary or zero
    // printf("IGE_r_108 %f %f %f %f\n",co3.a,co3.b,co3.c,co3.d);

  // posi of plane-symbol
  pt1.x = ra[5]; pt1.y = ra[6]; pt1.z = ra[7];
    // printf("IGE_r_108 symPos=%f %f %f\n",pt1.x,pt1.y,pt1.z);




  irc = UT3D_pl_plcoe (&pl1, &co3);
  if(irc < 0) return -2;


  // proj pt1 -> Plane = Origin
  UT3D_pt_intptvcpl_ (&pt1, &pl1, &pt1, &pl1.vz);
  UT3D_pl_ptpl (&pl1, &pt1);


/* 2007-07-06      ausgelagert nach IGE_r_tra__
  // Transformation ?
  if(IG_trNr != 0) {
    UTO_obj_tra_obj_m3 ((void*)&pl1, Typ_PLN, (void*)ra, IG_trMat);
  }
*/


  // *el = UTO_stru_2_obj (Typ_PLN, Typ_PLN, &pl1);
  OGX_SET_OBJ (el, Typ_PLN, Typ_PLN, 1, &pl1);

  // printf("ex IGE_r_108 %d\n",IG_ia1[1]);

  return 0;

}



//=====================================================================
  int IGE_r_106 (ObjGX *ox1, double *ra) {
//=====================================================================
// Punkteketten und Polygone ..


  int     i1, mode, ptAnz, iact, pTab1Nr, iInc;
  double  zVal;
  Point   *pTab1;


  // printf("IGE_r_106  0=%f 1=%f\n",ra[0],ra[1]);

  mode  = ra[0];  // 1 = 2D-Punkte, 2=3D-Punkte, 3=3D-Punkte+3D-Vektoren
  ptAnz = ra[1];


  if(mode == 1) {
    zVal = ra[2];
    iact = 3;
    iInc = 2;

  } else if(mode == 2) {
    iact = 2;
    iInc = 3;

  } else if(mode == 3) {
    iact = 2;
    iInc = 6;

  }

  // printf("IGE_r_106 mode=%d ptAnz=%d\n",mode,ptAnz);


  pTab1 = (void*) memspc55;
  pTab1Nr = sizeof(memspc55) / sizeof(Point);


  for(i1=0; i1<ptAnz; ++i1) {
    if(i1 >= pTab1Nr) {
      TX_Error("IGE_r_106 E001 - overflow\n");
      goto L_fertig;
    }
    pTab1[i1].x = ra[iact];
    pTab1[i1].y = ra[iact+1];

    if(mode == 1) {
      pTab1[i1].z = zVal;
    } else {
      pTab1[i1].z = ra[iact+2];
    }

    // printf(" pt %d = %f,%f,%f\n",i1,pTab1[i1].x,pTab1[i1].y,pTab1[i1].z);

    iact += iInc;
  }



  L_fertig:
/* 2007-07-06      ausgelagert nach IGE_r_tra__
  // Transformation ?
  if(IG_trNr != 0) {
    for(i1=0; i1<ptAnz; ++i1) UT3D_pt_tra_pt_m3 (&pTab1[i1],IG_trMat,&pTab1[i1]);
    // ox1 = memspc55;
    // UTO_ox_tra (&ox1, &el, IG_trMat);
  }
*/


  ox1->typ      = Typ_CVPOL;
  ox1->form     = Typ_PT;
  ox1->siz      = ptAnz;
  // ox1->data     = (void*)pTab1;  // memspc55 !!
  ox1->data     = impSpc.next;


  // die Punkte -> impSpc speichern
  UME_save (&impSpc, (void*)pTab1, ptAnz*sizeof(Point));



  return 0;

}


//=====================================================================
  int IGE_r_212 (ObjGX *el, double *ra) {
//=====================================================================
// resolv text

  static GText    IG_tx1;

  int      i1, cNr;
  char     *ptx;


  cNr = ra[1];                     // nr of chars

  // printf("IGE_r_212 %d |%s|\n",cNr,memspc55);


  IG_tx1.pt.x  = ra[9];
  IG_tx1.pt.y  = ra[10];
  IG_tx1.pt.z  = ra[11];


/* 2007-07-06      ausgelagert nach IGE_r_tra__
  // Transformation ?
  if(IG_trNr != 0) {
    UTO_obj_tra_obj_m3 ((void*)&IG_tx1.pt, Typ_PT, (void*)&IG_tx1.pt, IG_trMat);
  }
*/


  // Texhoehe = Boxbreite / Nr_of_chars / Charsiz
  // IG_tx1.size  = GTX_siz__ (ra[2] / cNr);
  IG_tx1.size  = UTP_db_rnd2sig (ra[2] / cNr);  // auf 2 signif. Stllen runden

  IG_tx1.dir   = UT_DEGREES(ra[6]);

  // IG_tx1.txt   = memspc55;  // Text liegt immer dort.
  IG_tx1.txt   = impSpc.next;



  // wenn Text mit " beginnt / endet - Entfernen.
  ptx = memspc55;
  cNr = strlen(ptx);
  if((ptx[cNr-1] == '"')||(ptx[cNr-1] == '\'')) {
    ptx[cNr-1] = '\0';
    cNr -= 1;
  }
  if((ptx[0] == '"')||(ptx[0] == '\'')) {
    ++ptx;
    cNr -= 1;
  }



  // den text -> impSpc speichern
  UME_save (&impSpc, ptx, cNr+1);


  // el.typ  = Typ_GTXT;
  // el.data = (void*)&IG_tx1;

/*
  printf("GTXT: pt=%f,%f,%f\n",IG_tx1.pt.x,IG_tx1.pt.y,IG_tx1.pt.z);
  printf("      siz=%f dir=%f\n",IG_tx1.size,IG_tx1.dir);
  printf("      txt=|%s|\n",IG_tx1.txt);
*/


  // *el = UTO_stru_2_obj (Typ_GTXT, Typ_GTXT, &IG_tx1);
  OGX_SET_OBJ (el, Typ_GTXT, Typ_GTXT, 1, &IG_tx1);

  return 0;

}


//=====================================================================
  int IGE_r_214 (ObjGX *el, double *ra) {
//=====================================================================
// resolv Leader / Arrrow
// ra: (214), AnzahlPunkteLeaderline, LaengeArrowhead, BreiteArrowhead,
//            Z-value, PT_Arrowhead, PT_Leadline[APL]
// (PT's bestehen aus X- und Y-wert).


  static Line  ln1;


  // printf("IGE_r_214 form=%d ptNr=%f\n",impTab[impInd].form,ra[0]);
  // printf("   headsiz = %f %f   Z = %f\n",ra[1],ra[2],ra[3]);
  // printf("   headPT  = %f %f\n",ra[4],ra[5]);
  // printf("   EndPT1  = %f %f\n",ra[6],ra[7]);


  ln1.p1.x = ra[4];
  ln1.p1.y = ra[5];
  ln1.p1.z = ra[3];

  ln1.p2.x = ra[6];
  ln1.p2.y = ra[7];
  ln1.p2.z = ra[3];


/* 2007-07-06      ausgelagert nach IGE_r_tra__
  // Transformation ?
  if(IG_trNr != 0) {
    UTO_obj_tra_obj_m3 ((void*)&ln1, Typ_LN, (void*)&ln1, IG_trMat);
  }
*/


  // *el = UTO_stru_2_obj (Typ_LN, Typ_LN, &ln1);
  OGX_SET_OBJ (el, Typ_LN, Typ_LN, 1, &ln1);

  return 0;

}


//=====================================================================
  int IGE_r_112 (ObjGX *ox1, double *ra) {
//=====================================================================
// 112 Parametric Spline Curve
// Curve = ObjGX; data (table of polynom_d3) goes -> impSpc


  CurvPsp3 cv1;

  int        irc, i1, polNr, polSiz;
  polynom_d3 *polTab;
  // ObjGX      el;



  // printf("IGE_r_112\n");


  // allg. Speicherbereich mit Tabelle PolCo belegen
  polTab = (void*) memspc55;
  polSiz = sizeof(memspc55) / sizeof(polynom_d3);
  // printf("PolCoSiz= %d IG_cv1.polTab=%d\n",PolCoSiz,sizeof(polynom_d3));


  // Poly.koeff. > IG_cv1.polTab
  polNr = IGE_r_dec_112 (polTab, polSiz, ra);
  if(polNr < 0) return -2;


    // TESTBLOCK
    // printf(" polNr=%d\n",polNr);
    // for(i1=0; i1<=polNr; ++i1)
      // DEB_dump_obj__ (Typ_polynom_d3, &polTab[i1], "pol3[%d]",i1);
    // END TESTBLOCK


  // see also APT_decode_psp3
  cv1.plyNr  = polNr;
  // cv1.plyTab = polTab;
  cv1.plyTab = impSpc.next;
  cv1.v0     = 0.;
  cv1.v1     = 1.;
  cv1.dir    = 0;
  cv1.clo    = -1;
  cv1.trm    = 1;


  // Poly.koeff. -> impSpc speichern
  UME_save (&impSpc, (void*)polTab, (polNr+1)*sizeof(polynom_d3));


  // alle Daten ins el packen
  ox1->typ  = Typ_CVPSP3;
  ox1->form = Typ_CVPSP3;       // Typ_polynom_d3;
  // ox1->siz  = polNr + 1;        // polNr = Anzahl Segmente !
  ox1->siz  = 1; 
  // ox1->data = polTab;
  // ox1->data = &cv1;
  ox1->data = impSpc.next;


  // pack cv1 into impSpc
  UME_save (&impSpc, (void*)&cv1, sizeof(CurvPsp3));


  // offen: hier testen, ob curve linear;
  // jes: degrade to Line (see UT3D_bsp_degrad)


    // TESTBLOCK
    // DEB_dump_ox_0 (ox1, "ex IGE_r_112");
    // END TESTBLOCK


/* 2007-07-06      ausgelagert nach IGE_r_tra__
  // Transformation ?
  if(IG_trNr != 0) {
    // ox1 = memspc55;
    UTO_ox_tra (&ox1, ox1, IG_trMat);
  }
*/


  return 0;

}


//=====================================================================
  int IGE_r_dec_112 (polynom_d3 *PolCo, int PolCoSiz, double ra[]) {
//=====================================================================

// die Polynome > PolCo[] einlesen
// Anzahl Segmente -> PolCoNr
// Breakpoints: je ein float-Wert pro Segment; zB 0., 1., 2., usw.
//   Sind der Abstand des Teilarcs vom Startpunkt.
//   Speichern zu jedem Segment (Teilarc) in PolCo[SegmentNr].u.
// Polynomkoeffizienten: jedes 3D-Segment hat 3 Polyn.Koeff;
//   je einen fuer x,y, und z.
//   Jeder Polyn.Koeff. hat Degree+1 Werte; fuer Deg.3 also 4 Werte; A-D.
//   Jedes 3D-Segment braucht also 3 * 4 Werte.
//   Koeff. speichern in PolCo[SegmentNr].x, y und z.
//   Der letzte Segmentstuetzpunkt steht direkt in den A-Teilkoeffizienten
//   des letzten Koeff.
//   Die erste Ableitung am Endpunkt (Auslauftangente ??) sind die B-Teilkoeff
//   des letzten Koeff (C ist 2. Ableitung, D die dritte Ableitung).



  int PolCoNr;
  int NDIM, i1, ia1, ia2;


  // printf("IGE_r_dec_112\n");


  // printf("CTYPE  = %f\n",ra[0]); // woraus Spline entstanden ist; unused
  // printf("deg. H = %f\n",ra[1]); // Degree of continuity; hier 1.; ???

  NDIM   = ra[2];                // muss hier 3 sein; 3=nonplanar; 2=planar.
  // if(NDIM != 3) {
    // TX_Error("*** Spline ist nicht nonplanar !!?? ***\n");
  // }

  PolCoNr = ra[3];                     // Nr of segments
  // printf("NDIM=%d Nr. N  = %d\n",NDIM, PolCoNr);


  if(PolCoNr+1 > PolCoSiz) {
    TX_Error("*** zu viele Splinesegmente ***\n");
    return -2;
  }

  ia2 = 4+PolCoNr+1;               // Startindex des ersten Polynomkoeff.
  ia1 = 4;                         // Startindex des ersten Breakpoint-Wertes

  for(i1=0; i1<=PolCoNr; ++i1) {   // Loop durch die Segment-Breakpoints

    PolCo[i1].u   = ra[ia1+0];     // akt. Breakpoint
    // PolCo[i1].u2  = ra[ia1+1];  // next Breakpoint
    ++ia1;

    PolCo[i1].x.a = ra[ia2+0];
    PolCo[i1].x.b = ra[ia2+1];
    PolCo[i1].x.c = ra[ia2+2];
    PolCo[i1].x.d = ra[ia2+3];

    PolCo[i1].y.a = ra[ia2+4];
    PolCo[i1].y.b = ra[ia2+5];
    PolCo[i1].y.c = ra[ia2+6];
    PolCo[i1].y.d = ra[ia2+7];

    PolCo[i1].z.a = ra[ia2+8];
    PolCo[i1].z.b = ra[ia2+9];
    PolCo[i1].z.c = ra[ia2+10];
    PolCo[i1].z.d = ra[ia2+11];
    ia2 += 12;

    // printf(" Seg.%d u=%f  a %f b %f c %f d %f\n",i1,PolCo[i1].u,
             // PolCo[i1].x.a,PolCo[i1].x.b,PolCo[i1].x.c,PolCo[i1].x.d);
    // printf(" Seg.%d u=%f  a %f b %f c %f d %f\n",i1,PolCo[i1].u,
             // PolCo[i1].y.a,PolCo[i1].y.b,PolCo[i1].y.c,PolCo[i1].y.d);
    // printf(" Seg.%d u=%f  a %f b %f c %f d %f\n",i1,PolCo[i1].u,
             // PolCo[i1].z.a,PolCo[i1].z.b,PolCo[i1].z.c,PolCo[i1].z.d);
  }

  return PolCoNr;

}


//=====================================================================
  int IGE_r_126 (ObjGX *ox1, double *ra) {
//=====================================================================
// 126 Rational B-Spline Curve

  // static Line     ln1;
  static CurvBSpl IG_cvBSpl;

  int     ia1, K, M, A, N, knotNr;
  int     i1, i2, istart, ianz, irc, ideg, pTab2Nr;
  double  ps, pe, *vTab;
  Point   *pTab1, *pTab2, *pp1;
  Line    lnt;
  ObjGX   el, *ox2;
  void    *dPos;


  // printf("IGE_r_126\n");
  // printf("IGE_r_126     ind=%d %dP\n",impInd,IGE_r_ind2dNr(impInd));
  // if((impInd == 2232)||(impInd == 2466)) {
    // i1 = 2224;
    // printf(" [%d].data=%p\n",i1,impTab[i1].data);
    // DEB_dump_obj__ (impTab[i1].form, impTab[i1].data, "iI=2224");
  // }


  

  // ControlPoints > pTab1 (= Pointer into IG_ra !)
  // KnotVektor -> vTab (= Pointer into IG_ra !)
  // Anz. Segments -> ianz, Degree -> ideg.
  // IGE_r_decod1_126 (&ianz, &ideg, &ps, &pe, &pTab1, &vTab, ra);
  //=======================================================================

  K = ra[0];   // Anzahl Segmente (Anz Controlpoints - 1)
  M = ra[1];   // Degree; Catv5: 5
               // PROP1 - PROP4 sind in ra[2]-ra[5]
    // 0 PROP1   0=nonplanar     (1=planar)
    // 0 PROP2   0=openCurve     (1=closed Curve)
    // 1 PROP3   1=polynomial    (0=rational)
    // 0 PROP4   0=nonperiodic   (1=periodic)



  // printf("================================================\n");
  // printf("ind. K = %d\n",K);
  // printf("deg. M = %d\n",M);
  // printf("Prop1/2/3/4 = %f,%f,%f,%f\n",ra[2],ra[3],ra[4],ra[5]);


  ianz = K;  // war K+1 !! 2002.05.06
  ideg = M;


  N = K-M+1;
  A = N+2*M;

  // printf("N, A = %d,%d\n",N,A);


  // Degree+1 knotVals; CatV5: immer 0.0
  // (+ ptAnz knotVals)
  // printf(" %d + %d knots values\n",M+1,K+1);
  ia1 = 5;
  vTab = &ra[6];   // give back address of knotvector
  knotNr = M + K + 2;


/*
  // Testausg. knotvector  -----------------
  printf(" nun %d knots vector / knot sequence\n",knotNr);
  i2 = ia1;
  for(i1=0; i1<knotNr; ++i1) {
    ++i2;
    printf(" %d %d %f\n",i1,i2,ra[i2]);
  }
*/

  ia1 += knotNr;




/*
  // Testausg. weigths -----------------
  // ptAnz weigths; CatV5: immer 1.0
  printf(" nun %d weigths\n",K+1);
  for(i1=0; i1<=K; ++i1) {
    ++ia1;
    printf(" %d %d %f\n",i1,ia1,ra[ia1]);
  }
*/
  ia1 += K + 1;


  pTab1 = (Point*)&ra[ia1+1];        // give back address of controlpoints


/*
  // Testausg. controlpoints -----------------
  printf(" 3 x %d control points\n",K+1);
  i2 = ia1;
  for(i1=0; i1<=K; ++i1) {
    printf(" %d %d %f,%f,%f\n",i1,i2,ra[i2+1],ra[i2+2],ra[i2+3]);
    i2+=3;
  }
*/

  ia1 += (K + 1) * 3;


  // StartPar=0, EndPar= wie letzter knotVal
  ++ia1;
  ps = ra[ia1];
  // printf("Start/Endparameter = %f,%f (pos=%d)\n",ra[ia1],ra[ia1+1],ia1);
  ++ia1;
  pe = ra[ia1];


/*
  // Normalvektor; bei CatV5 0,0,0 (da nonplanar)
  ++ia1;
  printf("Unitnormal = %f,%f,%f (pos=%d)\n",ra[ia1],ra[ia1+1],ra[ia1+2],ia1);
*/

  // printf("anz=%d deg=%d\n",*ianz,*ideg);



  //=======================================================================

/*
  for(i1=0; i1<=ianz; ++i1) {
    printf(" 126-pt%d = %f,%f,%f\n",i1,pTab1[i1].x,pTab1[i1].y,pTab1[i1].z);
  }

  // nur Test: input-punkte raus
  IG_oTab = (void*) memspc51;
  IG_BufStat=1; IG_BufNr=ianz;
  for(i1=0; i1<ianz; ++i1) {
    IG_oTab[i1].typ = Typ_PT;
    IG_oTab[i1].p1  = UT2D_pt_pt3(&pTab1[i1]);
  }
  IG_BufInd=1; // das erste sofort raus
  *el = UT3D_obj_obj2 (&IG_oTab[0]);
*/




/*
  // nur Line
  if((ideg == 1)&&(ianz == 1)) {

    // // wenn CCV aktiv und was im PtBuffer: Line zu Polygon dazuaddieren.
    // irc = IGE_r_decod_ccvl (&pTab1[0], &pTab1[1]);
    // if(irc == 0) return 0;

    // als normale Line raus.
    lnt.p1   = pTab1[0];
    lnt.p2   = pTab1[1];
    if(IG_trNr != 0) {
      UTO_obj_tra_obj_m3 ((void*)&lnt, Typ_LN, (void*)&lnt, IG_trMat);
    }

    // mit ps, pe begrenzen
    // DEB_dump_obj__(Typ_LN, &lnt, "ex 126");
    // printf(" 126=Line; v0=%f v1=%f\n",ps,pe);
    UT3D_pt_evparln (&ln1.p1, ps, &lnt);
    UT3D_pt_evparln (&ln1.p2, pe, &lnt);

    *ox1 = UTO_stru_2_obj (Typ_LN, Typ_LN, &ln1);
    return 0;
  }
*/





  IG_cvBSpl.ptNr  = ianz + 1;  // Nr of points
  IG_cvBSpl.deg   = ideg;      // degree
  IG_cvBSpl.kvTab = vTab;      // knotvektors[ptNr + deg + 1]
  IG_cvBSpl.cpTab = pTab1;     // controlpoints[ptNr]
  IG_cvBSpl.v0    = ps;        // Startwert
  IG_cvBSpl.v1    = pe;        // Endwert


  ox1->typ  = Typ_CVBSP;
  ox1->form = Typ_CVBSP;
  ox1->siz  = 1;
  ox1->data = (void*)&IG_cvBSpl;


/* 2007-07-06      ausgelagert nach IGE_r_tra__
  // Transformation ?
  if(IG_trNr != 0) {
    // ox2 = (void*)memspc55;
    // ox2 = (void*)memspc101;
    // UTO_ox_tra (&ox2, ox1, IG_trMat);  MACHT GROBEN FEHLER !!!!
    // *ox1 = *ox2;
    for(i1=0; i1<IG_cvBSpl.ptNr; ++i1) {
      pp1 = &IG_cvBSpl.cpTab[i1];
      UT3D_pt_tra_pt_m3 (pp1, IG_trMat, pp1);
    }
  }
*/


  // linear: degrade ..
  if((ideg == 1)&&(ianz == 1))
    return UT3D_bsp_degrad (ox1, &IG_cvBSpl, &impSpc);



  // printf(" save knotNr=%d ptNr=%d\n",knotNr,ianz+1);

  // vTab u. pTab1 zeigen into IG_ra = memspc51; Felder -> impSpc kopieren.
  IG_cvBSpl.kvTab = impSpc.next;
  dPos = UME_save (&impSpc, (void*)vTab, knotNr*sizeof(double));
  if(!dPos) return -4;

  IG_cvBSpl.cpTab = impSpc.next;
  dPos = UME_save (&impSpc, (void*)pTab1, (ianz+1)*sizeof(Point));
  if(!dPos) return -4;





/*
  pTab2 = (void*) memspc55;
  pTab2Nr = sizeof(memspc55) / sizeof(Point);


  // aus dem B-Spline ein Polygon in p3Tab bilden; ptNr = Anzahl Punkte.
  irc=bspl_cvpol_cvbsp(&pTab2Nr,pTab2,ianz,ideg,ps,pe,pTab1,vTab);
  // printf(" pTab2Nr=%d irc=%d IG_pTabNr=%d\n",pTab2Nr,irc,IG_pTabNr);


  // Punktetabelle cv an die IG_pTab zufuegen
  IGE_r_poly_addpTab (pTab2, pTab2Nr);
*/


  // if((impInd == 2232)||(impInd == 2466)) {
    // i1 = 2224;
    // printf(" [%d].data=%p\n",i1,impTab[i1].data);
    // DEB_dump_obj__ (impTab[i1].form, impTab[i1].data, "iI=2224");
  // }
  

  return 0;

}


//=====================================================================
  int IGE_r_102 (ObjGX *ox1, double *ra, int ra_siz) {
//=====================================================================
// IN:  die D-LineNumbers der Iges-Objekte (stehen in D-Line ganz rechts).
// OUT: der Index des entsprechenden imTab-Record; i=(dNr-1)/2

  int    i1, i2, indNr;
  long   *iTab, dbi;


  indNr = ra[0];

  // printf("IGE_r_102 impInd=%d %d\n",impInd,indNr);


  iTab = (long*)memspc55;


  ox1->typ  = Typ_CVTRM;
  ox1->form = Typ_Index;
  ox1->siz  = indNr;


  // bei Size==1 ist Index in data !
  if(indNr == 1) {
    dbi = ra[1];
    ox1->data = PTR_LONG(dbi);  // see also OGX_SET_INDEX
    // ox1->data = (long)dbi;      // see also OGX_SET_INDEX

  } else {
    // Tabelle mitgeben; objIndices statt D-Line-Nr's
    ox1->data = iTab;
    for(i1=0; i1<indNr; ++i1) {
      // printf(" ra[%d]=%f\n",i1,ra[i1+1]);
      i2 = ra[i1+1];
      // iTab[i1] = IGE_r_dNr2ind (i2);   // D-LineNumber -> imTab-Record-Number
      iTab[i1] = i2;   // D-LineNumber
      // printf(" iTab[%d]=%d\n",i1,i2);
    }
  
    // // TestOnly:
    // for(i1=0;i1<indNr;++i1) printf("iTab[%d]=%d\n",i1,((long*)ox1->data)[i1]);
  }

  return 0;

}



//=====================================================================
  int IGE_rw_102 (ObjGX *ox1) {
//=====================================================================
// fix CCV

  int     i1, i2, ii, typ, ind;
  long    *iTab;
  char    *cBuf;

  cBuf = memspc55;
  iTab = ox1->data;

  // printf("IGE_rw_102 ================= siz=%d\n",ox1->siz);

  strcpy(cBuf, " ");


  // TestOnly:
  // if(ox1->siz == 1) {
    // printf(" [0]=%d\n",((long)ox1->data));
  // } else {
    // for(i1=0; i1<ox1->siz; ++i1) printf(" [%d]=%d\n",i1,iTab[i1]);
  // }


  // add all obj's
  for(i1=0; i1<ox1->siz; ++i1) {
    // bei Size==1 ist Index in data !
    if(ox1->siz == 1) {
      i2 = IGE_r_dNr2ind (LONG__PTR(iTab));
      // i2 = IGE_r_dNr2ind ((long)iTab);
    } else {
      i2 = IGE_r_dNr2ind (iTab[i1]);
    }
    // printf(" 102: %d %d typ=%d ind=%d\n",i1,i2,impTab[i2].typ,impTab[i2].ind);

    // wenn ind==0 wurde Obj noch nicht generiert - nachtraeglich generieren.
    if(impTab[i2].ind == 0) IGE_r_work_3 (i2);

    typ = impTab[i2].typ;
    ind = impTab[i2].ind;
    AP_obj_add_dbo (cBuf, typ, ind);
  }


  ox1->typ  = Typ_CVTRM;
  ox1->form = Typ_Txt;
  ox1->siz  = strlen(cBuf) + 1;
  ox1->data = (void*)cBuf;

    // TESTBLOCK
    // DEB_stop();
    // END TESTBLOCK
 

  return 0;

}


//=====================================================================
  int IGE_r_141 (ObjGX *ox1, double *ra) {
//=====================================================================
// 141 
// 141 Boundary
// 0   Typ(Int)  0=ModelSpaceCurves 1=ModelSpaceCurves & ParameterSpaceCurves
// 1   PREF(Int) 0-3; Representation; unused.
// 2   SPTR(Int) Pointer zur Stuetzflaeche (untrimmed)
// 3   N(Int)    Anzahl der (nun folgenden) Boundaryelemente
// 4 0   ID      Pointer zum Boundaryelement
//   1   SENSE   Orientation; 1=normal, 2=reverse
//   2   K       Anzahl of parameterCurves (0, wenn Typ=0)
//   3   P[K]    Pointer zu den parameterCurves


// dzt nur den BoundaryPointer einlesen, und dessen Ausgabeobjekte
// (typ und ind) auch fuer dieses Objekt setzen;
//  nachfolgend braucht Surf-143 diese Daten (143-Boundary zeig auf 141;
//  erste 141-Boundary zeigt auf die CCV)!

  int    io1, i1, i2, i3, cNr;
  long   *iTab;


  cNr = ra[3]; // Anzahl Boundaryelemente


  // printf("IGE_r_141 %d\n",cNr);

  if(cNr > 1) goto L_CCV;

    // impTab-Index of 3D-bound
    io1 = ra[4]; // Ptr auf die erste Boundary;
    i1 = IGE_r_dNr2ind (io1);

    // nur den Index merken; gCAD-Typ und Ind noch nicht verfuegbar.
    impTab[impInd].typ = Typ_Error;
    impTab[impInd].ind = i1;
    impTab[impInd].done = 0;

    // printf("  i1=%d impInd=%d\n",i1,impInd);
    // printf("  _142: %d t=%d i=%d\n",i1,impTab[i1].typ,impTab[i1].ind);


    // kein Ausgabeobjekt, nix tun.
    ox1->typ  = Typ_Done;

  return 0;




  //================================================================
  // es gibt mehrere Boundaryelemente; wie CCV
  L_CCV:

  iTab = (long*)memspc55;


  ox1->typ  = Typ_CVTRM;
  ox1->form = Typ_Index;
  ox1->siz  = cNr;
  ox1->data = iTab;


  // Tabelle mitgeben; objIndices statt D-Line-Nr's
  i1 = 0;
  i2 = 4;
  L_nxt:
    // printf(" ra[%d]=%f\n",i1,ra[i1+1]);
    i3 = ra[i2];
    iTab[i1] = i3;   // D-LineNumber
    // printf(" iTab[%d]=%d i2=%d\n",i1,i3,i2);

    ++i2;    // ra[i2] ist nun die Richtung; sollte als dir raus; wie ?
    ++i2; i3 = ra[i2];   // ra[i2] ist nun die Anzahl parameterCurves
    i2 += i3 + 1;        // skip parameterCurves

    ++i1;
    if(i1 < cNr) goto L_nxt;



  return 0;

}


//=====================================================================
  int IGE_r_142 (ObjGX *ox1, double *ra) {
//=====================================================================
// 142 CurveOnParametricSurface
//     (int) CreationFlag 0=unspec;1=proj.Curv->Surf;2=Inters.Surf/Surf;
//             3=IsoparametricCurve.
//     Pointer to Stuetzflaeche (Surf on which curv lies); zB auf 128
//     Ptr auf die 2D-Ableitung der Curve (zB CCV - 102) od 126; keine: 0.
//     Ptr auf die 3D-Curve Boundary (zB CCV - 102) od 100-Circ!
//     (int)   Darstellung:0=unspec;1=SxB;2=C;3:C od SxB

// dzt nur den BoundaryPointer einlesen, und dessen Ausgabeobjekte
// (typ und ind) auch fuer dieses Objekt setzen;
//  nachfolgend braucht Surf-144 diese Daten (144-Boundary zeig auf 142;
//  erste 142-Boundary zeigt auf die CCV)!

  int    io1, i1;


  io1 = ra[3]; // Ptr auf die Boundary;


  // printf("IGE_r_142 %d\n",io1);

  // impTab-Index of 3D-bound
  i1 = IGE_r_dNr2ind (io1);

  // nur den Index merken; gCAD-Typ und Ind noch nicht verfuegbar.
  impTab[impInd].typ = Typ_Error;
  impTab[impInd].ind = i1;

  impTab[impInd].done = 0;

  // printf("  i1=%d impInd=%d\n",i1,impInd);
  // printf("  _142: %d t=%d i=%d\n",i1,impTab[i1].typ,impTab[i1].ind);


  // kein Ausgabeobjekt, nix tun.
  ox1->typ  = Typ_Done;
  // ox1->form = Typ_Typ;

  return 0;

}


//=====================================================================
  int IGE_rw_141 (int ind) {
//=====================================================================
// eine Boundary fuer eine TrimmedSurface decodieren
// Den gCAD-Typ und Index der zugehoerigen CCV hier eintragen.

  int    i1, i2;



  // printf("IGE_rw_141 dNr=%d fTyp=%d ind=%d\n",IGE_r_ind2dNr(ind),
                        // impTab[ind].fTyp,impTab[ind].ind);



  if(impTab[ind].fTyp == 141) {          // 141 = Boundary

    // in ind wurde der Index der 3D-Bound. gespeichert.
    i1 = impTab[ind].ind;   // index der D-Bound


    if(i1 == 0) {   // wenn eine CCV ex 141, diese generieren.
      IGE_r_work_3 (ind);
      i1 = ind;
    }


      // printf(" bound %d fTyp=%d typ=%d ind=%d\n",i1,
                   // impTab[i1].fTyp,impTab[i1].typ,impTab[i1].ind);

    // wenn ind==0 wurde Obj noch nicht generiert - nachtraeglich generieren.
    if(impTab[i1].ind == 0) IGE_r_work_3 (i1);

    // gCAD-Typ und Index eintragen.
    impTab[ind].typ  = impTab[i1].typ;
    impTab[ind].ind  = impTab[i1].ind;
    impTab[ind].done = 1;

    // printf("ex IGE_rw_142 typ=%d ind=%d\n",impTab[ind].typ,impTab[ind].ind);

    return 0;
  }


  TX_Error("IGE_rw_141 E002 %d %d",ind,impTab[ind].fTyp);
  return -2;

}


//=====================================================================
  int IGE_rw_142 (int ind) {
//=====================================================================
// CurveOnParametricSurface
// eine Boundary fuer eine TrimmedSurface decodieren
// Den gCAD-Typ und Index der zugehoerigen CCV hier eintragen.

  int    i1, i2;


  // printf("IGE_rw_142 %d fTyp=%d ind=%ld\n",ind,
                        // impTab[ind].fTyp,impTab[ind].ind);
  // if(impTab[ind].done > 0) return 0;   // bereits erledigt



  if(impTab[ind].fTyp == 142) {          // 142 = CurveOnParametricSurface

    // in ind wurde der Index der 3D-Bound. gespeichert.
    i1 = impTab[ind].ind;   // index der D-Bound

    // printf(" bound %d fTyp=%d typ=%d ind=%d\n",i1,
                   // impTab[i1].fTyp,impTab[i1].typ,impTab[i1].ind);


    // wenn ind==0 wurde Obj noch nicht generiert - nachtraeglich generieren.
    if(impTab[i1].ind == 0) IGE_r_work_3 (i1);


    // gCAD-Typ und Index eintragen.
    impTab[ind].typ  = impTab[i1].typ;
    impTab[ind].ind  = impTab[i1].ind;
    impTab[ind].siz  = 1;                  // 2022-05-30
    impTab[ind].done = 1;

      // TESTBLOCK
      // printf("ex IGE_rw_142 typ=%d ind=%ld\n",impTab[ind].typ,impTab[ind].ind);
      // DEB_exit();
      // END TESTBLOCK

    return 0;
  }


  TX_Error("IGE_rw_142 E002 %d %d",ind,impTab[ind].fTyp);
  return -2;


}


//=====================================================================
  int IGE_r_118 (ObjGX *ox1, double *ra) {
//=====================================================================
// 118 RuledSurface
//     Pointer 1. curve
//     Pointer 2. curve
//     (int)   direction; 0=first to first, 1=first to last
//     (int)   development flag: 1=developable, 0=possibly not


  static ObjGX objTab[2];

  long   io1, io2, i1, ind;
  int    if1, typ;


  // printf("IGE_r_118 =================\n");

  io1 = ra[0];  // Pointer 1. curve
  io2 = ra[1];  // Pointer 2. curve
  if1 = ra[2];  // direction; 0=first to first, 1=first to last

  // printf(" 118-%d %d %d\n",io1,io2,if1);


  // find apt_typ und apt_ind of io1, io2.
  // io -> index umwandeln;  typ,ind dieser zeile nehmen.
  i1 = IGE_r_dNr2ind (io1);
  typ = impTab[i1].typ;
  ind = impTab[i1].ind;
  // printf("  o1-%d typ=%d ind=%d\n",i1,typ,ind);
  if(typ == Typ_Error) {
    TX_Error("IGE_r_118 E001 %d",io1);
    return -1;
  }

  // if(ind == 0) IGE_r_work_3 (i1);   // NUR EINLESEN !



  objTab[0].typ  = Typ_Error; // typ/ind(=data) wird spaeter ersetzt
  objTab[0].form = Typ_Index;
  objTab[0].siz  = 1;
  objTab[0].data = PTR_LONG(i1);
  objTab[0].dir  = 0;


  i1 = IGE_r_dNr2ind (io2);
  typ = impTab[i1].typ;
  ind = impTab[i1].ind;
  // printf("  o2-%d typ=%d ind=%d\n",i1,typ,ind);
  if(typ == Typ_Error) {
    TX_Error("IGE_r_118 E002 %d",io2);
    return -1;
  }

  // if(ind == 0) IGE_r_work_3 (i1);       // NUR EINLESEN !

  objTab[1].typ  = Typ_Error;  // typ/ind(=data) wird spaeter ersetzt
  objTab[1].form = Typ_Index;
  objTab[1].siz  = 1;
  objTab[1].data = PTR_LONG(i1);
  objTab[1].dir  = if1;


  ox1->typ  = Typ_SURRU;
  ox1->form = Typ_ObjGX;
  ox1->siz  = 2;
  ox1->data = objTab;
  ox1->dir  = if1;

  return 0;

}


//=====================================================================
  int IGE_r_122 (ObjGX *ox1, double *ra) {
//=====================================================================

  static double dTab[4];


  // printf("IGE_r_122 %f\n",ra[0]);


  // Werte nur zwischenspeichern
  dTab[0] = ra[0]; // Kurve (directrix)
  dTab[1] = ra[1]; // Endpunkt generatrix
  dTab[2] = ra[2];
  dTab[3] = ra[3];

  ox1->typ  = Typ_SURRU;
  ox1->form = Typ_Val;
  ox1->siz  = 4;
  ox1->data = (void*)dTab;
  ox1->dir  = 0;

  return 0;

}


//=====================================================================
  int IGE_rw_122 (ObjGX *ox1) {
//=====================================================================
// 122 Zylinderflaeche
//     (eine Line (generatrix) wird (immer parallel zu sich selbst) entlang
//      einer Kurve (directrix) bewegt).
//     Pointer directrix
//     (3 x double) Endpunkt der generatrix (Startpunkt = Start der directrix)

  static char cBuf[128];

  int     i1, io1, typ, ind;
  double  *dTab;
  Point   pte;

  // printf("IGE_rw_122\n");

  dTab = ox1->data;


  io1   = dTab[0]; // Kurve (directrix)
  pte.x = dTab[1]; // Endpunkt generatrix
  pte.y = dTab[2];
  pte.z = dTab[3];




  // find apt_typ und apt_ind of io1.
  // io -> index umwandeln;  typ,ind dieser zeile nehmen.
  i1 = IGE_r_dNr2ind (io1);

  if(impTab[i1].ind == 0) IGE_r_work_3 (i1);

  typ = impTab[i1].typ;
  ind = impTab[i1].ind;


  cBuf[0] = '\0';

  AP_obj_add_dbo (cBuf, typ, ind);

  // ca. D(P(L20,MOD(1)) P(xyz))
  strcat(cBuf, " D(P(");
  AP_obj_add_dbo (cBuf, typ, ind);
  strcat(cBuf, " PTS(1))");

  AP_obj_add_pt (cBuf, &pte);

  strcat(cBuf, ")");
  i1 = strlen(cBuf);
  // printf(" _122:|%s|%d\n",cBuf,i1);


  ox1->typ  = Typ_SURRU;
  ox1->form = Typ_Txt;
  ox1->siz  = i1 + 1;
  ox1->data = (void*)cBuf;
  ox1->dir  = 0;

  return 0;

}


//=====================================================================
  int IGE_r_120 (ObjGX *ox1, double *ra) {
//=====================================================================
// 120 Surface of Revolution


  static double dTab[4];

  // printf("IGE_r_120 %f %f\n",ra[0],ra[1]);

  // Werte nur zwischenspeichern
  dTab[0] = ra[0];  // Pointer to Axis (Line)
  dTab[1] = ra[1];  // Pointer to Contour
  dTab[2] = ra[2];  // StartAngle
  dTab[3] = ra[3];  // TerminateAngle

  ox1->typ  = Typ_SURRV;
  ox1->form = Typ_Val;
  ox1->siz  = 4;
  ox1->data = (void*)dTab;

  return 0;

}


//=====================================================================
  int IGE_rw_120 (ObjGX *ox1) {
//=====================================================================
// 120 Surface of Revolution
//     Pointer to Axis (Line)
//     Pointer to Contour (Line=Conus, Circle=Torus ..)
//     StartAngle (0.)
//     TerminateAngle (6.28)
// Winkel: sind relativ; zuerst (CCW!) nach Winkel1 verdrehen,
//         dann nach Winkel2 weiterdrehen.

// noch nicht implem.:
// wenn Stat23=0(unabhaengig): als Body + Flaeche ausgeben;
// wenn Stat23>0(abhaengig): nur als Body ausgeben
//                           (es folgt noch Parent (Surf(144))


  static char   IG_cBuf[512];
  static Conus  sRev;
  static Torus  sTor;

  int    i_ax, i_cv, irc, i1, typ, iCen, iCov;
  long   ind;
  double a_s, a_e, d1, d2, *dTab;
  Point  ptc, pt1, pt2, pt3, pt4;
  Line   *lnp1;
  Vector vz, vc1, vc2;
  Circ   ci1, *cip1;



  // printf("IGE_rw_120 =================\n");


  dTab = ox1->data;

  i_ax = dTab[0];   // P-Nr der Drehachse
  i_cv = dTab[1];   // P-Nr der erzeugenden Curve
  a_s  = dTab[2];   // Startwinkel
  a_e  = dTab[3];   // Endwinkel

    // printf("       ax=%d cv=%d as=%f ae=%f\n",i_ax,i_cv,a_s,a_e);



  // Drehachse holen (immer Line)
  iCen = IGE_r_dNr2ind (i_ax);
  // printf(" i-ax: iCen=%d fTyp=%d ind=%d\n",iCen,
          // impTab[iCen].fTyp,impTab[iCen].ind);
  if(impTab[iCen].fTyp != 110) {
    printf("***** Error Drehachse einer RevSur ist keine Line !\n");
    return -1;
  }

  // Obj muss nicht gespeichert werden !
  // // wenn ind==0 wurde Obj noch nicht generiert - nachtraeglich generieren.
  // if(impTab[iCen].ind == 0) IGE_r_work_3 (iCen);

  // Line -> Punkt, Vektor
  lnp1 = (Line*)impTab[iCen].data;
  ptc = lnp1->p1;
  UT3D_vc_ln (&vz, lnp1);

  // Mittelpunkt und Achse:
  // printf("ptc=%f,%f,%f\n",ptc.x,ptc.y,ptc.z);
  // printf("vz=%f,%f,%f\n",vz.dx,vz.dy,vz.dz);


  // fix Centerline
  if(impTab[iCen].ind == 0) IGE_r_work_3 (iCen);
  // printf("cenTyp=%d cenInd=%d\n",impTab[iCen].typ,impTab[iCen].ind);
  if(impTab[iCen].ind == 0) {TX_Print("IGE_rw_120 E001"); return -2; }


  


  //----------------------------------------------------------------
  // Erzeugende holen
  iCov = IGE_r_dNr2ind (i_cv);
  // printf(" i-cv: iCov=%d fTyp=%d\n",iCov,impTab[iCov].fTyp);



  // fix Coverline
  if(impTab[iCov].ind == 0) IGE_r_work_3 (iCov);
  // printf("covTyp=%d covInd=%d\n",impTab[iCov].typ,impTab[iCov].ind);
  if(impTab[iCov].ind == 0) {TX_Print("IGE_rw_120 E002"); return -2; }




  //----------------------------------------------------------------
  // fix OutputText
  strcpy(IG_cBuf, "SRV");

  // add CenterObj
  AP_obj_add_dbo (IG_cBuf, impTab[iCen].typ, impTab[iCen].ind);

  // add CoverObj
  AP_obj_add_dbo (IG_cBuf, impTab[iCov].typ, impTab[iCov].ind);

  // add Angles
  // AP_obj_add_func1 (IG_cBuf, 'A', UT_DEGREES(a_s));
  // AP_obj_add_func1 (IG_cBuf, 'A', UT_DEGREES(a_e));
  AP_obj_add_val (IG_cBuf, UT_DEGREES(a_s));
  AP_obj_add_val (IG_cBuf, UT_DEGREES(a_e));


  ox1->typ  = Typ_SUR;
  ox1->form = Typ_Txt;
  ox1->siz  = strlen(IG_cBuf) + 1;
  ox1->data = (void*)IG_cBuf;





  //==================================================================
  // Line - Konus generieren (eine Mittelachse und eine Mantellinie)
  //==================================================================
  if(impTab[iCov].fTyp != 110) goto L_torus;

  // all should be OK ..

  return 0;



/* VERSION mit BODY
  lnp1 = (Line*)impTab[iCov].data;
  pt1 = lnp1->p1;
  pt2 = lnp1->p2;

  // die Endpunkte der erzeugenden Kontur-Line:
  // printf("pt1=%f,%f,%f\n",pt1.x,pt1.y,pt1.z);
  // printf("pt2=%f,%f,%f\n",pt2.x,pt2.y,pt2.z);


  // Version 2: einen Body-Konus ausgeben
  // B=CON center1 center2 radius1 radius2
  if(IG_trNr != 0) {
    UT3D_pt_tra_pt_m3 (&ptc, IG_trMat, &ptc);
    UT3D_vc_tra_vc_m3 (&vz,  IG_trMat, &vz);
    UT3D_pt_tra_pt_m3 (&pt1, IG_trMat, &pt1);
    UT3D_pt_tra_pt_m3 (&pt2, IG_trMat, &pt2);
  }

  // create Conus
  UT3D_pt_projptptvc (&pt3, &d1, &pt1, &ptc, &vz);
  UT3D_pt_projptptvc (&pt4, &d2, &pt2, &ptc, &vz);

  UT3D_bcon_pt2rd2 (&sRev, &pt3, &pt4, d1, d2);


  ox1->typ  = Typ_CON;
  ox1->form = Typ_CON;
  ox1->siz  = 1;
  ox1->data = &sRev;

  return 0;
*/



  //==================================================================
  // Circ - Torus generieren    (Line + Circ)
  //==================================================================
  L_torus:
  if(impTab[iCov].fTyp != 100) goto L_RevSur;

  // all should be OK ..

  return 0;



/* VERSION mit BODY

  // den Kreis holen;
  cip1 = (Circ*)impTab[iCov].data;
  // DEB_dump_obj__ (Typ_AC, cip1, "CI-TOR:");

  // Cir-Center auf die Drehachse projizieren
  UT3D_pt_projptptvc (&ptc, &sTor.r1, &cip1->pc, &ptc, &vz);
  sTor.r2 = fabs(cip1->rad);

  // printf(" rc=%f\n",cip1->rad);
  // printf(" r1=%f r2=%f\n",sTor.r1,sTor.r2);

  // r1 = ganz an der Aussenseite!
  // r2 = Radius_Nebenkreis
  // Modifikator
  // ist der ausserste Toruskreis kleiner oder groesser als der Torusring ?
  // Den Mittelpunkt der Circ ci1 suchen; ist der nicht auf der gleichen
  // seite wie der Kreismittelpunkt (Trennlinie ist die Hauptachse) dann
  // ist r1 = r2 - r1.
  // ci1-Center -> Hauptachse proj (ptc) -> vc1
  // ci1-Mittelpt -> Hauptachse proj -> vc2
  // wenn vc1 antiparallel zu vc2: verschiedene Seiten; r1=r2-r1.

  UT3D_vc_2pt (&vc1, &ptc, &cip1->pc);


  UT3D_pt_mid_ci (&pt1, cip1);
  UT3D_pt_projptptvc (&pt2, &d1, &pt1, &ptc, &vz);
  UT3D_vc_2pt (&vc2, &pt2, &pt1);

  // if antiparallel
  if(UT3D_comp2vc_p (&vc1, &vc2, UT_TOL_PAR) == 0) {
    sTor.r1 = sTor.r2 - sTor.r1;
  } else {
    sTor.r1 += sTor.r2;
  }

  // Test circ ausgeben.
  // *ox1 = UTO_stru_2_obj (Typ_CI, Typ_CI, &ci1);
  // return(0);


  // Plane sTor.pl
  UT3D_pl_ptvc (&sTor.pl, &ptc, &vz);

  ox1->typ  = Typ_TOR;
  ox1->form = Typ_TOR;
  ox1->siz  = 1;
  ox1->data = &sTor;

  return 0;
*/



  //==================================================================
  // B-Spl - RevSur generieren    (Line + B-Spl)
  //==================================================================
  L_RevSur:
  if(impTab[iCov].fTyp != 126) {
    TX_Print("***** RevSur unsupported fTyp %d\n",impTab[iCov].fTyp,iCov);
    return -2;
  }

  // all should be OK ..

  return 0;

}


//=====================================================================
  int IGE_r_128 (ObjGX *ox1, double *ra) {
//=====================================================================
// 128 Rational B-Spline Surface
// Datenfelder pTab, kTabU u kTabV zeigen into ra !!
// retCode: 0 OK
//          1 is planar-curve; data=text "S(POL..)"

// EINE FLAECHE MIT 2 X 2 Punkte koennte man als Rechteck ausgeben ???

  static SurBSpl  IG_suBSpl;

  int      i1, i2, ii, irc, iAct, KU, KV, MU, MV, NU, NV,
           kTabU_Nr, kTabV_Nr, ptU_Nr, ptV_Nr;
  double   *kTabU, *kTabV, ups, upe, vps, vpe;
  Point    *pTab;


  // printf("IGE_r_128\n");
  // printf("_128:%f,%f,%f,%f\n",ra[0],ra[1],ra[2],ra[3]);


  KU = ra[0];
  KV = ra[1];

  MU = ra[2];
  MV = ra[3];

  NU = 1+KU-MU;
  NV = 1+KV-MV;

  // printf("KU,KV=%d,%d MU,MV=%d,%d NU,NV=%d,%d\n",KU,KV,MU,MV,NU,NV);
  // printf(" deg MU=%d deg MV=%d\n",MU,MV);

  iAct = 9;

  // U-knots einlesen
  kTabU = &ra[iAct];
  kTabU_Nr = MU + KU + 2;



/*
  // Testausg. U-knotvector  -----------------
  printf(" knots vector U %d\n",kTabU_Nr);
  for(i1=0; i1<kTabU_Nr; ++i1) {
    printf(" %d %d %f\n",i1,i1+iAct,ra[i1+iAct]);
  }
*/

  iAct = iAct+kTabU_Nr;


  // V-knots einlesen
  kTabV = &ra[iAct];
  kTabV_Nr = MV + KV + 2;


/*
  // Testausg. V-knotvector  -----------------
  printf(" knots vector V %d\n",kTabV_Nr);
  for(i1=0; i1<kTabV_Nr; ++i1) {
    printf(" %d %d %f\n",i1,i1+iAct,ra[i1+iAct]);
  }
*/


  iAct = iAct+kTabV_Nr;


  // weights einlesen
  ptU_Nr = KU + 1;
  ptV_Nr = KV + 1;



  // Testausg. weigths -----------------
  // ptAnz weigths; CatV5: immer 1.0
  // printf(" (%d,%d) weigths\n",ptU_Nr,ptV_Nr);
  // ii = iAct;
  // for(i1=0; i1<ptV_Nr; ++i1) {
    // for(i2=0; i2<ptU_Nr; ++i2) {
      // printf(" (%d,%d) %d %f\n",i1,i2,ii,ra[ii]);
      // ++ii;
    // }
  // }


  iAct = iAct+(ptU_Nr*ptV_Nr);


  // Controlpoints einlesen; U rows, V cols
  pTab = (Point*)&ra[iAct];


/*
  // Testausg. controlpoints -----------------
  printf(" (ptU_Nr=%d,ptV_Nr=%d) control points\n",ptU_Nr,ptV_Nr);
  ii = iAct;
  for(i1=0; i1<ptV_Nr; ++i1) {
    for(i2=0; i2<ptU_Nr; ++i2) {
      printf(" (%d,%d) %d %f,%f,%f\n",i1,i2,ii,
             ra[ii],ra[ii+1],ra[ii+2]);
      ii += 3;
    }
  }
*/


  iAct = iAct+((ptU_Nr*ptV_Nr)*3);


  // U - StartPar, EndPar
  ups = ra[iAct];    ++iAct;
  upe = ra[iAct];    ++iAct;
  vps = ra[iAct];    ++iAct;
  vpe = ra[iAct];    ++iAct;
  // printf("U-Start/Endparameter = %f,%f\n",ups,upe);
  // printf("V-Start/Endparameter = %f,%f\n",vps,vpe);




  //==========================================================
  // U-Controlpoints als Polygon raus
  // ptTab.ptNr  = ptU_Nr;
  // ptTab.ptTab = pTab;


  IG_suBSpl.ptUNr  = ptU_Nr;  // Nr of points
  IG_suBSpl.ptVNr  = ptV_Nr;  // Nr of points
  IG_suBSpl.degU   = MU;      // degree
  IG_suBSpl.degV   = MV;      // degree
  IG_suBSpl.cpTab  = pTab;    // controlpoints[ptNr]
  IG_suBSpl.kvTabU = kTabU;   // knotvektors[ptNr + deg + 1]
  IG_suBSpl.kvTabV = kTabV;   // knotvektors[ptNr + deg + 1]



  ox1->typ  = Typ_SURBSP;
  ox1->form = Typ_SURBSP;
  ox1->siz  = 1;
  ox1->data = (void*)&IG_suBSpl;


  if(IG_trNr != 0) {
    TX_Error("IGES-transformation of BSPS not yet supported ****");
  }



  // Datenfelder pTab, kTabU u kTabV zeigen into ra !!
  IG_suBSpl.cpTab = impSpc.next;
  UME_save (&impSpc, (void*)pTab, ptU_Nr*ptV_Nr*sizeof(Point));

  IG_suBSpl.kvTabU = impSpc.next;
  UME_save (&impSpc, (void*)kTabU, kTabU_Nr*sizeof(double));

  IG_suBSpl.kvTabV = impSpc.next;
  UME_save (&impSpc, (void*)kTabV, kTabV_Nr*sizeof(double));


    // TESTBLOCK
    // DEB_dump_ox_0 (ox1, "IGE_r_128-L10");
    // END TESTBLOCK

  // degradieren ?
  // if((ptU_Nr == 2)||(ptV_Nr == 2)) UT3D_sbs_degrad (ox1, &IG_suBSpl, &impSpc);
  if((MU == 1)||(MV == 1)) UT3D_obj_cnvt_sbsp (ox1, &IG_suBSpl, &impSpc);


  // test if its planar; yes: irc = 1,
  if((ox1->typ == Typ_SUR)&&(ox1->form == Typ_Txt)) irc = 1;
  else irc = 0;
    


    // TESTBLOCK
    // printf("ex IGE_r_128 irc=%d\n",irc);
    // DEB_dump_ox_0 (ox1, "ex IGE_r_128");
    // DEB_exit();
    // END TESTBLOCK


  return irc;

}


//=====================================================================
  int IGE_r_143 (ObjGX *ox1, double *ra) {
//=====================================================================
// 143 Trimmed Parametric Surface
//  0  Flag mit od ohne 2D-Curves
//  1  Ptr auf die Stuetzflaeche; zB auf 102 od 128
//  2  IBN = GesamtAnzahl Curves (Boundary + Inseln)
//  3- Ptr auf die Curves u Inseln ..


  // nur die ersten 4 Zahlen als int merken

  int    i1, iNr;
  long   *iTab;

  iTab = (long*)memspc55;


  // printf("IGE_r_143 %f\n",ra[1]);


  iTab[0] = ra[1];     // Ptr auf die Stuetzflaeche;
  iTab[1] = ra[0];     // Flag
  iTab[2] = ra[2] - 1; // Anzahl Inseln

  iNr = ra[2];         // Anzahl Boundary + Inseln

  for(i1=0; i1<iNr; ++i1) {
    iTab[i1+3] = ra[i1+3];
      // printf(" %d iTab[%d] = %f\n",i1,i1+3,ra[i1+3]);
  }


  ox1->typ  = Typ_SUR;
  ox1->form = Typ_Index;
  ox1->siz  = iNr + 3;
// TODO: use Typ_Int8
  ox1->data = iTab;


  return 0;

}


//=====================================================================
  int IGE_r_144 (ObjGX *ox1, double *ra) {
//=====================================================================
// 144 Trimmed Parametric Surface
//  0  Ptr auf die Stuetzflaeche; zB auf 102 od 128
//  1  TrimmedFlag; 0=untrimmed, 1=trimmed
//  2  IBN = Anzahl Inseln innerhalb der Boundary
//  3  Ptr auf die Boundary; (-> 142)
//     Ptr[IBN] auf die Inseln ..


  // nur die ersten 4 Zahlen als int merken

  int    i1, iNr;
  long   *iTab;

  iTab = (long*)memspc55;


  // printf("IGE_r_144 %f\n",ra[0]);


  iTab[0] = ra[0]; // Ptr auf die Stuetzflaeche;
  iTab[1] = ra[1]; // TrimmedFlag
  iTab[2] = ra[2]; // Anzahl Inseln
  iTab[3] = ra[3]; // Ptr auf die Boundary

  iNr = iTab[2];   // Anzahl Inseln

  for(i1=0; i1<iNr; ++i1) iTab[i1+4] = ra[i1+4];


  ox1->typ  = Typ_SUR;
  ox1->form = Typ_Index;
  ox1->siz  = iNr + 4;
// TODO: use Typ_Int8
  ox1->data = iTab;


  return 0;

}


//=====================================================================
  int IGE_rw_143 (ObjGX *ox1) {
//=====================================================================
// 143 BoundedSurface (Bound = 141)
//     Typ(Int)  0=ModelSpaceCurves 1=ModelSpaceCurves & ParameterSpaceCurves
//     SPTR(Int) Pointer zur Stuetzflaeche (untrimmed)
//     N(Int)    Anzahl der (nun folgenden) Kurven
//     P[N]      N Pointers to Boundarycurves (Aussenkontur, Loecher ..)




  static char IG_cBuf[1024];

  int     io1, io2, i1, i2, i3, irc, typ_bound, typ_body, cNr;
  long    l1, ind_bound, ind_body, id_body, *iTab;
  char    auxBuf [64];
  ObjGX   oxa1, *oxp;
  SurBSpl *su1;

  iTab = ox1->data;

  io1 = iTab[0]; // Ptr auf die Stuetzflaeche;
  cNr = iTab[2]; // Anzahl Inseln
  io2 = iTab[3]; // Ptr auf die Boundary


  // printf("IGE_rw_143 ============= Stuetz=%d Bound=%d cNr=%d\n",io1,io2,cNr);
  // for(i1=0; i1<cNr; ++i1) printf(" insel[%d]=%d\n",i1,iTab[i1+4]);



  //================================================================
  // decode Boundary
  i1 = IGE_r_dNr2ind (io2);

  // decode Boundary (typ u ind der CCV holen
  if(IGE_rw_141 (i1) < 0) return -1;

  typ_bound = impTab[i1].typ;
  ind_bound = impTab[i1].ind;
    // printf(" bound= %d %d\n",typ_bound,ind_bound);



  //================================================================
  // decode Body (Stuetzflaeche)
  id_body = IGE_r_dNr2ind (io1);

  if(impTab[id_body].ind == 0) IGE_r_work_3 (id_body);

  typ_body = impTab[id_body].typ;
  ind_body = impTab[id_body].ind;

  if(impTab[id_body].ind == 0) {
    TX_Print(" IGE_rw_143 E001 \n");
    return -2;
  }


  // hide Stuetzflaeche
  // GA_hide__ (0, ind_body,typ_body);
  // GA_view__ (-1, 1, typ_body, ind_body);
  IGE_r_hide (typ_body, ind_body);


    // printf(" TypSS=%d indSS=%d D=%d\n",typ_body,ind_body,id_body);




  //================================================================
  // load outputText
  if(typ_body == Typ_PLN) {
    IG_cBuf[0] = '\0';
  } else {
    strcpy(IG_cBuf, "FSUB");
    AP_obj_add_dbo (IG_cBuf, typ_body, ind_body);
  }
  // strcat(IG_cBuf, " ");
    // printf(" Fsub2=|%s|\n",IG_cBuf);


  // add Boundary
  AP_obj_add_dbo (IG_cBuf, typ_bound, ind_bound);
    // printf("  + bound-ID=|%s|\n",IG_cBuf);


  // add Inseln
  if(cNr > 0) {
    // if(typ_body != Typ_PLN) { printf("IGE_rw_144 E008\n"); return 0; }
    for(i1=0; i1<cNr; ++i1) {
      i2 = iTab[i1+4];                    // Ptr auf die Boundary
      i3 = IGE_r_dNr2ind (i2);            // Ptr -> Index
      if(IGE_rw_141 (i3) < 0) return -1;  // Boundary decodieren (142 -> Obj)
      typ_bound = impTab[i3].typ;
      ind_bound = impTab[i3].ind;
        // printf(" %d %d %d typ=%d ind=%d\n",i1,i2,i3,typ_bound,ind_bound);
      if(strlen(IG_cBuf) > 1000) { TX_Error("IGE_rw_143 E007"); break; }
      AP_obj_add_dbo (IG_cBuf, typ_bound, ind_bound);
        // printf(" bound[%d]=|%s| %d %d\n",i1,IG_cBuf,typ_bound,ind_bound);
    }
  }




  //----------------------------------------------------------------
  L_out:
  ox1->typ  = Typ_SUR;
  ox1->form = Typ_Txt;
  ox1->siz  = strlen(IG_cBuf) + 1;
  ox1->data = (void*)IG_cBuf;


  return 0;

}


//=====================================================================
  int IGE_rw_144 (ObjGX *ox1) {
//=====================================================================
// 144 Trimmed Parametric Surface
//  0  Ptr auf die Stuetzflaeche; zB auf 102 od 128
//  1  TrimmedFlag; 0=untrimmed, 1=trimmed
//  2  IBN = Anzahl Inseln innerhalb der Boundary
//  3  Ptr auf die Boundary; (-> 142)
//  4- Ptr[IBN] auf die Inseln ..

  static char IG_cBuf[1024];

  int     io1, io2, i1, i2, i3, irc, typ_bound, typ_body, cNr, id_bnd, id_body;
  long    l1, ind_bound, ind_body, *iTab;
  double  d1;
  char    auxBuf [64];
  ObjGX   oxa1, *oxp;
  SurBSpl *su1;

  iTab = ox1->data;

  io1 = iTab[0]; // Ptr auf die Stuetzflaeche;
  cNr = iTab[2]; // Anzahl Inseln
  io2 = iTab[3]; // Ptr auf die Boundary



  // printf("IGE_rw_144 ============= Stuetz=%d Bound=%d cNr=%d\n",io1,io2,cNr);
  // for(i1=0; i1<cNr; ++i1) printf(" insel[%d]=%ld\n",i1,iTab[i1+4]);


  // Ausgabeflaeche entspricht dem zugehoerigen Body.


  //================================================================
  // decode Boundary
  id_bnd = IGE_r_dNr2ind (io2);

  // decode Boundary (typ u ind der CCV holen
  if(IGE_rw_142 (id_bnd) < 0) return -1;

  typ_bound = impTab[id_bnd].typ;
  ind_bound = impTab[id_bnd].ind;

    // TESTBLOCK
    // printf(" rw_144-bnd typ=%d ind=%ld D=%d\n",typ_bound,ind_bound,id_bnd);
    // IGE_r_dump_impTab (id_bnd);
    // DEB_exit();
    // END TESTBLOCK



  //================================================================
  // decode suppSur (Stuetzflaeche)
  id_body = IGE_r_dNr2ind (io1);

    // printf(" rw_144-suppSur.stat__=%d\n",impTab[id_body].stat__);

  if(impTab[id_body].stat__ == 0) {
    // suppSur ist not planar - get outerBnd
    if(impTab[id_body].ind == 0) IGE_r_work_3 (id_body);

  } else {
    // suppSur planar; skip creation of outer boundary
    goto L_144_out;
  }


  typ_body = impTab[id_body].typ;
  ind_body = impTab[id_body].ind;

  if(impTab[id_body].ind == 0) {
    TX_Print(" IGE_rw_144 RevSur with unsupported RevSur \n");
    return -2;
  }

    // TESTBLOCK
    // printf(" rw_144-suppSur typ=%d ind=%ld D=%d\n",typ_body,ind_body,id_body);
    // IGE_r_dump_impTab (id_body);
    // DEB_exit();
    // END TESTBLOCK



  //================================================================
  L_144_out:
  // load outputText
  IG_cBuf[0] = '\0';
  // if(cNr > 0) strcpy(IG_cBuf, "FSUB");

  // add Boundary
  AP_obj_add_dbo (IG_cBuf, typ_bound, ind_bound);
    // printf(" rw_144-bound-ID1=|%s|\n",IG_cBuf);

  // add Inseln
  if(cNr > 0) {
    // if(typ_body != Typ_PLN) { printf("IGE_rw_144 E008\n"); return 0; }
    for(i1=0; i1<cNr; ++i1) {
      i2 = iTab[i1+4];                    // Ptr auf die Boundary
      i3 = IGE_r_dNr2ind (i2);            // Ptr -> Index
      if(IGE_rw_142 (i3) < 0) return -1;  // Boundary decodieren (142 -> Obj)
      typ_bound = impTab[i3].typ;
      ind_bound = impTab[i3].ind;
      // printf(" %d %d %d typ=%d ind=%d\n",i1,i2,i3,typ_bound,ind_bound);
      if(strlen(IG_cBuf) > 1000) { TX_Error("IGE_rw_144 E007"); break; }
      AP_obj_add_dbo (IG_cBuf, typ_bound, ind_bound);
      // printf(" bound[%d]=|%s| %d %d\n",i1,IG_cBuf,typ_bound,ind_bound);
    }
  }
    // printf(" rw_144-bound-ID2=|%s|\n",IG_cBuf);
    // DEB_exit();


  //===================================================
  // trimmed planar
  if(typ_body == Typ_PLN) {
    // Planar; raus.
    goto L_out;



  // //===================================================
  } else if(impTab[id_body].fTyp == 128) {
    // trimmedSurf
    // is supportSurf planar -
    if(impTab[id_body].stat__ == 1) {
      goto L_out;
    }
    

  //===================================================
  // trimmed B_SplineSurf
  } else if(typ_body == Typ_SURBSP) {
    // if Body=Planar: kein weiterer Text erforderlich; CCV genuegt.
    // oxa1 = DB_GetSur (ind_body); // geht ned; surf noch nicht in DB!

    su1 = (SurBSpl*)impTab[id_body].data;
    // printf("  degU,degV=%d %d\n",su1->degU, su1->degV);

    // hide the bodySurface (Typ_SURBSP, impTab[impInd].ind
    // printf("hide2 ind=%d typ=%d\n",ind_body,typ_body);
    // GA_hide__ (0, ind_body,typ_body);
    // GA_view__ (-1, 1, typ_body, ind_body);
    IGE_r_hide (typ_body, ind_body);


    // ist BSP-Surf planar ? dann (ohne BSP-Surf) ausgeben
    d1 = UT3D_sbs_ck_planar(su1);
      // printf(" rw_144-d1=%f tol=%f\n",d1,UT_TOL_cv);
    if(d1 < UT_TOL_cv) goto L_out;

    // trimmed BSP-Surf
    // printf("IGE_rw_144 E003 %d %d %d\n",typ_bound,typ_body,io1);

    // add FSUB Body
    strcpy(auxBuf, "FSUB");
    AP_obj_add_dbo (auxBuf, typ_body, ind_body);
    strcat(auxBuf, " ");
    // printf(" Fsub1=|%s|\n",auxBuf);
    // printf(" Fsub2=|%s|\n",IG_cBuf);
    l1 = strlen(IG_cBuf);
    UTX_insert1 (IG_cBuf, &l1, auxBuf, 0L);
    // printf(" Fsub3=|%s|\n",IG_cBuf);

    goto L_out;



  //===================================================
  // trimmed Ruled_Sur
  } else if(typ_body == Typ_SURRU) {

    // Boundary ?
    oxp = (ObjGX*)impTab[i1].data;

    // hide the bodySurface (Typ_SURBSP, impTab[impInd].ind
    // printf("hide3 ind=%d typ=%d\n",ind_body,typ_body);
    // GA_hide__ (0, ind_body,typ_body);
    // GA_view__ (-1, 1, typ_body, ind_body);
    IGE_r_hide (typ_body, ind_body);

    // check if Ruled_Sur planar ist
    // if(UT3D_sru_ck_planar(oxp) < UT_TOL_pt) goto L_out;

    // add FSUB Body
    strcpy(auxBuf, "FSUB");
    AP_obj_add_dbo (auxBuf, typ_body, ind_body);
    strcat(auxBuf, " ");
    // printf(" Fsub1=|%s|\n",auxBuf);
    // printf(" Fsub2=|%s|\n",IG_cBuf);
    l1 = strlen(IG_cBuf);
    UTX_insert1 (IG_cBuf, &l1, auxBuf, 0);
    // printf(" Fsub3=|%s|\n",IG_cBuf);

    goto L_out;

  }



  //===================================================
  // trimmed conus / torus
  if((typ_body != Typ_CON)    &&
     (typ_body != Typ_TOR)    &&
     (typ_body != Typ_SURBSP) &&
     (typ_body != Typ_SUR)) {
    TX_Error("***** Error IGE_rw_144 E005 %d %d %d",typ_bound,typ_body,io1);
    return -2;
  }

  // add FSUB Body
  strcpy(auxBuf, "FSUB");
  AP_obj_add_dbo (auxBuf, typ_body, ind_body);
  strcat(auxBuf, " ");
  // printf(" Fsub1=|%s|\n",auxBuf);
  // printf(" Fsub2=|%s|\n",IG_cBuf);
  l1 = strlen(IG_cBuf);
  UTX_insert1 (IG_cBuf, &l1, auxBuf, 0);
  // printf(" Fsub3=|%s|\n",IG_cBuf);


  // hide the bodySurface (Conus ..)
  // printf("hide4 ind=%d typ=%d\n",ind_body,typ_body);
  // GA_hide__ (0, ind_body,typ_body);
  // GA_view__ (-1, 1, typ_body, ind_body);
  IGE_r_hide (typ_body, ind_body);



  //----------------------------------------------------------------
  L_out:
  ox1->typ  = Typ_SUR;
  ox1->form = Typ_Txt;
  ox1->siz  = strlen(IG_cBuf) + 1;
  ox1->data = (void*)IG_cBuf;

    // TESTBLOCK
    // printf(" ex-IGE_rw_144 |%s|\n",IG_cBuf);
    // DEB_exit();
    // END TESTBLOCK
 

  return 0;

}


//=====================================================================
  int IGE_r_308 (ObjGX *ox1, double *ra) {
//=====================================================================
// 308 SubfigureDefinition
//       (int)    depth (levels)
//       (string) name   (zB "8HTRIANGLE")
//       (int)    anzahl objects
//       (int[])  pointer to objects (auf die D-ZeilenNummern)

static ImpSubmodel sm1;


  int    i1, iNr;
  long   *iTab;


  iNr = ra[2];

  // printf("IGE_r_308 |%s| objNr=%d\n",memspc55,iNr);
  // printf("  r_308-impInd=%ld\n",impInd);



  // Detailname speichern
  sm1.mnam = (char*)UME_save (&impSpc, memspc55, strlen(memspc55)+1);
  if(sm1.mnam == NULL) {TX_Error("IGE_r_308 E001"); return -1;}
  // make name safe
  UTX_safeName(sm1.mnam, 1);
    // printf(" r_308-mnam = |%s|\n",sm1.mnam);


  // ObjID's speichern
  iTab = (long*)memspc55;
  for(i1=0; i1<iNr; ++i1) {
    iTab[i1] = ra[i1 + 3];
      // printf("  r_308-o[%d]=%ld\n",i1,iTab[i1]);
  }

  sm1.siz  = iNr;
  sm1.iTab = (long*)UME_save (&impSpc, iTab, iNr*sizeof(long));


  // kein Ausgabeobjekt, nix tun.
  ox1->typ      = Typ_Done;
  ox1->form     = Typ_Txt;  // typ ImpSubmodel nur lokal
  ox1->siz      = sizeof(ImpSubmodel);
  ox1->data     = &sm1;


//   // store impInd-index in IG_sm_ind
//   if(IG_smNr >= impMDLNR) {TX_Error("IGE_r_308 EOM1"); return -1;}
//   IG_sm_ind[IG_smNr] = impInd;
//   ++IG_smNr;


  return 0;

}


//=====================================================================
  int IGE_rw_308 (int ind) {
//=====================================================================
// alle zum Detail gehoerigen Objekte aktiv setzen
// impTab[x].activ  = 1;
// impTab[x].stat23 = 0;

static ImpSubmodel *im1;

  int         i1, i3;
  long        *iTab;



  // printf("IGE_rw_308 %d\n",ind);

  im1  = impTab[ind].data;
  iTab = im1->iTab;
    // printf("  308-nam=|%s| siz=%d\n",im1->mnam,im1->siz);

  // copy name -> memspc55
  strcpy(memspc55, im1->mnam);


  for(i1=0; i1<im1->siz; ++i1) {
    i3 = IGE_r_dNr2ind (iTab[i1]);
    impTab[i3].activ  = 1;
    impTab[i3].stat23 = 0;  // sonst wirds wieder geskippt! 2004-07-27
      // printf(" 308-activate impTab[%d]=%ld %d\n",i3,iTab[i1],i1);
  }

  return 0;

}


//=====================================================================
  int IGE_r_408 (ObjGX *ox1, double *ra) {
//=====================================================================
// 408 SubfigureInstance   (Ditto)
//       (int)       pointer zum P-label der SubFigDef(308)
//       (double[3]) position
//       (double)    scale


  static ModelRef   mr1;
  ImpSubmodel       *sm1;

  int      i1, ind;
  Point    pt1;
  ModelBas *mdb1;


  // printf("IGE_r_408 %f Pos: %f,%f,%f, scl=%f\n",ra[0],ra[1],ra[2],ra[3],ra[4]);

  mr1.po.x  = ra[1];
  mr1.po.y  = ra[2];
  mr1.po.z  = ra[3];

  mr1.scl   = ra[4];

  mr1.vx    = UT3D_VECTOR_X;
  mr1.vz    = UT3D_VECTOR_Z;

  // ra[0] = D-LnNr of 308 (SubFigDef)
  // index ofimpTab:
  i1 = ra[0];
  ind = IGE_r_dNr2ind (i1);              // get impTab-index 
//     printf(" r_408-ind=%d\n",ind);       // D-LnNr of 308 (SubFigDef)
//   sm1 = impTab[ind].data;
//     printf(" r_408-nam |%s|\n",sm1->mnam);
// 
//   // get index=index of use subModels ..
//   for(i1=0; i1<IG_smNr; ++i1) {
//     if(IG_sm_ind[i1] == ind) goto L_mbi;
//   }
//   {TX_Error("IGE_r_408 E1"); return -1;}
//   L_mbi:
  mr1.modNr = ind;  // nur index des 308-record !!


  // den Name des BasModel erst spaeter
/*
  mdb1 = DB_get_ModBas (impTab[ind].ind);
  if(mdb1 == NULL) {
    TX_Error("IGE_r_decode_408 E001");
    return -1;
  }
  mr1.mnam = mdb1->mnam;
  printf(" ind=%d BMNr=%d Fignam=|%s|\n",ind,impTab[ind].ind,mr1.mnam);
*/


/* 2007-07-06      ausgelagert nach IGE_r_tra__
  // Transformation ?
  if(IG_trNr != 0) {
    // Nullpunkt addieren
    UT3D_m3_get ((void*)&pt1, 3, IG_trMat);
    mr1.po.x += pt1.x;
    mr1.po.y += pt1.y;
    mr1.po.z += pt1.z;
    // Z u X-Vec uebernehmen
    UT3D_m3_get ((void*)&mr1.vx, 0, IG_trMat);
    UT3D_m3_get ((void*)&mr1.vz, 2, IG_trMat);
    // mr1.vx = ?? // IG_trMat
    // UTO_obj_tra_obj_m3 ((void*)&p1, Typ_PT, (void*)ra, IG_trMat);
  }
*/


  ox1->typ  = Typ_Model;
  ox1->form = Typ_Model;
  ox1->siz  = 1;
  ox1->data = (void*)&mr1;



  return 0;

}


//=====================================================================
  int IGE_rw_408 (ObjGX *ox1) {
//=====================================================================
// nur mnam setzen
// useless func ..

  int         i1;
  ModelRef    *mr1;
  ImpSubmodel *im1;


  // printf("IGE_rw_408\n");

  mr1 = ox1->data;
  i1  = mr1->modNr;

  if((i1 < 0)||(i1 > impNr)) {
    TX_Error("IGE_rw_408 E001");
    return -1;
  }

  // printf(" rw_408: mNr=%d fTyp=%d\n",i1,impTab[i1].fTyp);

/*
  im1 = impTab[i1].data;
    // printf(" nam=|%s|\n",im1->mnam);
  mr1->mnam = im1->mnam;
*/

  return 0;

}


//================================================================
  int IGE_r_hide (int typ, long dbi) {
//================================================================
 
  GA_view__ (-1L, 1, typ, dbi);

/*
  char  s1[64], s2[32];

  APED_oid_dbo__ (s2, typ, dbi);

  sprintf(s1, ":ATTRIB:%s H", s2);

  UTF_add1_line (s1);
*/
  return 0;

}


//================================================================
  int IGE_test (int ii) {
//================================================================

  if(IG_modNam) {
    if(IG_modNam[0] != '\0') {
      printf(" error %d\n",ii);
      // exit (0);
      AP_debug__ ("IGE_test");
    }
  }

  return 0;

}

 
/* ======================== EOF ====================== */
