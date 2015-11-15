// Subroutines fuer IGES - Datei schreiben         RF  2000-04-28
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
2014-02-09 Typ_AC -> Typ_CI
2002-08-19 B-Spline zu. RF.
2002-07-19 Div. RF.
2002-04-24 oTab -> 3D. RF.
2002-03-17 B-Spline (126) zu. RF.
2001-02-21 Portierung -> W32. RF.
2001-06-17 Portierung -> Linux. RF.

-----------------------------------------------------
*/
/*!
\file  ../exp/ige_w.c
\brief write iges files 
\code
=====================================================
List_functions_start:

IGE_defMat        .
IGE_w_mat
IGE_w_rec        wr obj
IGE_w_obj        wr obj
IGE_w_124        wr TrMat
IGE_w_D_Ln       wr D-Block
IGE_w_rD         wr D-Block
IGE_w_rP         wr P-Blockline
IGE_w_rPP        wr pt in P-line
IGE_w_rPT        wr text ("H")
IGE_w_rP1        Add Parameter to P-Line
IGE_w_rP_out     wr line
IGE_w_init       wr H-block
IGE_w_rT         wr T-block, join.
IGE_w_savPtr
IGE_w_getPtr     get iges-ptr of cad-obj
IGE_w_dNr2ind
IGE_w_dump_expTab

IGE_w_P_MD       wr 308. (SubfigDef.)
IGE_w_P_MR       wr 408. (SubfigInst.)
IGE_w_P_SRC      wr 122 Zylinderflaeche
IGE_w_142        wr 142
IGE_w_P_142      wr 142 CurveOnParametricSurface
IGE_w_P_144      wr 144 Trimmed Parametric Surface
IGE_w_P_SRV      wr 120 Revolved Surf
IGE_w_P_SBS      wr 128 Rational B-Spline Surface
IGE_w_P_SRU      wr 118 RuledSurface
IGE_w_P_POL      wr PolygonCurve
IGE_w_P_POL2     2D
IGE_w_P_BS       wr bspl-cv
IGE_w_P_PLN      wr plane
IGE_w_P_ELL      wr ellipse
IGE_w_P_CCV      wr CCv - Ent.102
IGE_w_P_TX       Text
IGE_w_P_PS       polynomial curve
IGE_w_subObjs    wr all SubObjs
IGE_w_SUR        wr SubObjs for 144
IGE_w_CVCCV      wr SubObjs for CCV

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



=======================================
Ablauf:
 D-Block und P-Block gleichzeitig schreiben, hinterher zusammenhaengen.
 AptTyp und AptInd werden in expTab gespeichert (Index = Iges-D-Index)


IGE_w_rec
 IGE_w_subObjs


TrMat:
 Man kann zu allen Obj. TrMat mitgeben; bei Kreisen ist unbedingt TrMat
 erforderlich. Zuerst alle TrMat's raus; ihre Iges-Indexe hier merken.
 Dann mit den Objekten diese mitgeben. Bei 3D-Kreisen ohne eine TrMat
 muss dynam. (vom IGE-Out) eine trMat generiert werden.
 Info, ob Tra schon draussen ist.

IGES-Formatbeschreibung siehe ige_r.c
Doku: ../exp/ige/version6.pdf


--------------------------------------------------------------
  // Testrahmen fuer IGE-Write

  FILE     *fp1, *fp2;

  int      i1, dreh;
  long     l1;
  Point    po;
  Vector   vx,vy,vz;
  Mat_4x3  m1;
  ObjG     o1;



  TX_Print("test_cons1");

  fp1=fopen("t1.igs", "w+");
  fp2=fopen("t1.ig1", "w+");

  IGE_w_init (fp1);



  //---------------------------------------------------------
  // eine TrMat raus
  UT3D_vc_3db (&vx,  1.,    0.,        0.);
  UT3D_vc_3db (&vy,  0.,    0.7071,    0.7071);
  UT3D_vc_3db (&vz,  0.,   -0.7071,    0.7071);
  UT3D_m3_load (m1, &vx, &vy, &vz); 
  UT3D_pt_3db (&po,  100.,  100., 0.);
  UT3D_m3_load_o (m1, &po);

  l1=IGE_w_124 (m1, fp1, fp2);



  // Punkt mit Transl
  o1.typ = Typ_PT;
  o1.p1.x=100.;
  o1.p1.y=100.;
  o1.p1.z=100.;
  IGE_w_rec (&o1, l1, fp1, fp2);



  // Kreis um diesen Punkt mit Tra.
  o1.typ = Typ_CI;
  o1.pc.x=100.;
  o1.pc.y=100.;
  o1.pc.z=0.;
  o1.p1.x=120.;
  o1.p1.y=100.;
  o1.p1.z=0.;
  o1.p2  =o1.p1;
  o1.vz  =vz;
  o1.rad = 20.;
  IGE_w_rec (&o1, l1, fp1, fp2);


  // Kreis um diesen Punkt ohne Tra.
  o1.typ = Typ_CI;
  o1.pc.x=100.;
  o1.pc.y= 70.71;
  o1.pc.z= 70.71;
  o1.p1.x=120.;
  o1.p1.y= 70.71;
  o1.p1.z= 70.71;
  o1.p2  =o1.p1;
  o1.vz  =vz;
  o1.rad = 20.;
  IGE_w_rec (&o1, 0, fp1, fp2);


  // eine Line raus
  o1.typ = Typ_LN;
  o1.p2.x=112.;
  o1.p2.y=34.;
  o1.p2.z=56.;
  IGE_w_rec (&o1, fp1, fp2);


  // zusammenkopieren und Schlussrecord
  IGE_w_rT (fp1, fp2);


  // Close u. Hilfsdatei loeschen
  fclose(fp1);
  fclose(fp2);
  OS_file_delete ("t1.ig1");


  //system("type t1.igs");
  system("notepad t1.igs");

*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_geo.h"
#include "../ut/ut_ox_base.h"             // OGX_SET_INDEX
#include "../ut/ut_txt.h"
#include "../ut/ut_TX.h"

// #include "../ut/ut_crv.h"
#include "../ut/ut_bspl.h"
#include "../db/ut_DB.h"                  // DB_GetCurv

#include "../gr/vf.h"                     // GL_vf1_CS

#include "../xa/xa_mem.h"                 // memspc101
#include "../xa/xa_sele.h"                // Typ_go*

#include "../exp/ige.h"






typedef struct {long ind; unsigned char typ;}                         ExpObj;
typedef struct {long modNr; long lNr;}                                ModPos;




#define IG_EntPT  116
#define IG_EntLN  110
#define IG_EntCCV 102
#define IG_EntPOL 106
#define IG_EntPLN 108
#define IG_EntELL 104
#define IG_EntPS  112
#define IG_EntSRU 118
#define IG_EntSRC 122
#define IG_EntSRV 120
#define IG_EntBND 142  // use Typ_goGeo1 / Boundary = CurveOnParametricSurface
#define IG_EntSUR 144
#define IG_EntBS  126
#define IG_EntSBS 128
#define IG_EntCI  100
#define IG_EntTX  212
#define IG_EntMR  408
#define IG_EntMD  308






static  int  IG_D_line_nr, IG_S_line_nr, IG_G_line_nr, IG_P_line_nr;
static  int  IG_D_line_mod; // end of last submodel


static  int  IG_TypTab[] = {
  Typ_PT,      Typ_LN,      Typ_CI,      0,           Typ_CVPOL,
  Typ_CVPOL2,  Typ_CVPSP3,  Typ_CVBSP,   Typ_ATXT,    Typ_GTXT,
  Typ_CVELL,   Typ_CVCCV,   Typ_PLN,     Typ_SURRU,   Typ_SUR,
  Typ_SURRV,   Typ_SURBSP,  Typ_goGeo1,  Typ_Model,   Typ_SubModel,
  Typ_CVCLOT,  -99};

static  int  IG_EntTab[] = {
  IG_EntPT,    IG_EntLN,    IG_EntCI,    0,           IG_EntPOL, 
  IG_EntPOL,   IG_EntPS,    IG_EntBS,    IG_EntTX,    IG_EntTX,
  IG_EntELL,   IG_EntCCV,   IG_EntPLN,   IG_EntSRU,   IG_EntSUR,
  IG_EntSRV,   IG_EntSBS,   IG_EntBND,   IG_EntMR,    IG_EntMD,
  IG_EntPOL,   -99};

static  long IG_TrInd;

static  int  IG_Pa1[9], IG_Pa2[9];

static  char IGlnD1[88], IGlnD2[88];



static  int  IG_mode=0;   // 0=normales Obj; 1=SubObj (von CCV ..)


static Circ    IGE_ci1;
static CurvElli IGE_el1;



  long IGE_w_124 (Mat_4x3 m1, FILE *fp_o1, FILE *fp_o2);



static ExpObj   *expTab;
static long     expTabSiz, expNr, expInd;

#define IG_modMax 1000
static int    IG_modNr;
static ModPos IG_modTab[IG_modMax];


static int    *IG_subObjTab, IG_subObjNr;




//============================================================
// Prototypes:
  long IGE_w_getPtr (int typ, long ind);






//=======================================================================
  int IGE_defMat (Vector *vc1) {
//=======================================================================
// wenn VZ ein reiner Z-vektor, kann man CIRC u Ellipse ohne TrMat ausgeben
// RC=0: vc1 == ein reiner Z-vektor.

  // UT3D_stru_dump (Typ_VC, vc1, "IGE_defMat");

  if(fabs(vc1->dx) > UT_TOL_min1) return 1;
  if(fabs(vc1->dy) > UT_TOL_min1) return 1;
  if(vc1->dz < 0.) return 1;                  // 0 0 -1
  return 0;

}


//=======================================================================
  int IGE_w_mat (ObjGX *ox, FILE *fp_o1, FILE *fp_o2) {
//=======================================================================

  int         typ;
  double      d1, d2;
  Vector      vc1;
  Circ        *ci1;
  CurvElli    *el1;
  Plane       pl1;
  Mat_4x3     m1, im1;
  ModelRef    *mdr;


  // printf("IGE_w_mat typ=%d tri=%d\n",ox->form,IG_TrInd);


  // wenn IG_TrInd > 0, dann wurde die matrix bereits ausgegeben.... Wie ???
  // nix mehr tun.
  if (IG_TrInd > 0) return 0;




  //================ Kreis ======================================
  // 3D-Kreisboegen: TrMat generieren und raus.
  if(ox->form == Typ_CI) {

    ci1 = ox->data;

    // wenn keine Transf. angegeben: liegt der Kreis in X-Y ?
    // Ja: normal raus; keine Transf. erforderlich.
    if((IGE_defMat(&ci1->vz) == 0)&&(ci1->rad > 0.)) return 0;


    // Plane aus Nullpunkt, Z-Achse und Punkt auf X-Achse
    vc1 = ci1->vz;
    if(ci1->rad < 0.) UT3D_vc_invert (&vc1, &vc1);  // CW: verkehrt !!
    UT3D_pl_pto_vcz_ptx (&pl1, &ci1->pc, &vc1, &ci1->p1);

    // nun die Matrix aus der Plane erstellen
    UT3D_m3_loadpl (m1, &pl1);

    // die TrMat raus
    IG_TrInd = IGE_w_124 (m1, fp_o1, fp_o2);

    IGE_ci1.pc = UT3D_PT_NUL;

    d1 = fabs(ci1->rad);
    IGE_ci1.rad = d1;

    d2 = fabs(ci1->ango);
    IGE_ci1.ango = d2;

    IGE_ci1.p1.x = d1;
    IGE_ci1.p1.y = 0.;
    IGE_ci1.p1.z = 0.;

    IGE_ci1.p2.x = cos(d2) * d1;
    IGE_ci1.p2.y = sin(d2) * d1;
    IGE_ci1.p2.z = 0.;

/*
    // Nein: eine Transformation generieren.
    // es liegt ein schiefer Kreis ohne Transf.Matrix vor!
    // Es ist die Matrix zu generieren, invertieren, der Kreis auf die
    // Plane zu prjizieren.
    //    Zuerst eine Plane mit dem Z-Vektor konstruieren
    // printf(" ist 3D-Kreis\n");
    UT3D_pl_ptvc (&pl1, &ci1->pc, &ci1->vz);

    // nun die Matrix aus der Plane erstellen
    UT3D_m3_loadpl (m1, &pl1);

    // die TrMat raus
    IG_TrInd = IGE_w_124 (m1, fp_o1, fp_o2);

    // nun die inverse Matrix erstellen
    UT3D_m3_invm3 (im1, m1);

    // nun die 3 Punkte auf die Plane (im1) umrechnen
    UT3D_pt_traptm3 (&IGE_ci1.p1, im1, &ci1->p1);
    UT3D_pt_traptm3 (&IGE_ci1.p2, im1, &ci1->p2);
    UT3D_pt_traptm3 (&IGE_ci1.pc, im1, &ci1->pc);

    // Radius kopieren
    IGE_ci1.rad = ci1->rad;
    // wenn z negativ: p1 -p2 vertauschen
    if(ci1->vz.dz < 0.) {
       MEM_swap__ (&IGE_ci1.p1, &IGE_ci1.p2, sizeof(Point));
       IGE_ci1.rad = -ci1->rad;
    }
*/
    ox->data = (void*)&IGE_ci1;
    // Fertig.




  //================ Ellipse ======================================
  } else if (ox->form == Typ_CVELL) {

    el1 = (CurvElli*)ox->data;

    // if(IGE_defMat(&el1->vz) == 0) return 0;


    // coordinate system of the ellipse
    // UT3D_m3_load_povxvy (m1, &(el1->pc), &(el1->va), &(el1->vb));
    UT3D_m3_load (m1, &(el1->va), &(el1->vb), &(el1->vz));
    UT3D_m3_load_o (m1, &(el1->pc));

    // die TrMat raus
    IG_TrInd = IGE_w_124 (m1, fp_o1, fp_o2);

    // invert matrix
    UT3D_m3_invm3 (im1, m1);

    // nun die 3 Punkte auf die Plane (im1) umrechnen
    UT3D_pt_traptm3 (&IGE_el1.p1, im1, &el1->p1);
    UT3D_pt_traptm3 (&IGE_el1.p2, im1, &el1->p2);
    UT3D_pt_traptm3 (&IGE_el1.pc, im1, &el1->pc);
    UT3D_vc_travcm3 (&IGE_el1.va, im1, &el1->va);
    UT3D_vc_travcm3 (&IGE_el1.vb, im1, &el1->vb);
    IGE_el1.vz = UT3D_VECTOR_Z;

    // wenn z negativ: p1 -p2 vertauschen
    if(el1->vz.dz < 0.) {
      MEM_swap__ (&el1->p1, &el1->p2, sizeof(Point));
    } 

    ox->data = (void*)&IGE_el1;




  //================ SubfigInst ======================================
  } else if (ox->form == Typ_Model) {

    mdr = ox->data;

    UT3D_m3_load_povxvz (m1, &mdr->po, &mdr->vx, &mdr->vz);

    // die TrMat raus
    IG_TrInd = IGE_w_124 (m1, fp_o1, fp_o2);



  }


  return 0;

}


//=====================================================================
  int IGE_w_rec (ObjGX *objIn, long TrInd, int apt_typ, long apt_ind,
                 FILE *fp_o1, FILE *fp_o2) {
//=====================================================================
/*
Obj. el ausgeben.
Wenn eine Transformation angegeben ist, dann muss das Obj ohne die Transformation sein !
(also 2D).
irc =  0 = OK;
irc = -1 = Obj Error (Obj skipped ..)
irc = -2 = Error; stop all.
*/

  int      irc;
  ObjGX    el;
  // Circ     *ci1;
  // Plane    pl1;
  // Mat_4x3  m1, im1;


  /* ------------------------------------------------------------------ */
  // printf("\n==============================================\n");
  // printf("IGE_w_rec typ=%d tr=%d %d %d\n",objIn->typ,TrInd,apt_typ,apt_ind);


  IG_TrInd = TrInd;
  el       = *objIn;


  //  Bestandteile von CCV's (und getrimmten Flaechen ...) ausgeben
  irc = IGE_w_subObjs (&el, fp_o1, fp_o2);
  if (irc < 0) return irc;

  IG_mode  = 0;             // 0=normales Obj; 1=SubObj (von CCV ..)


  // ein Obj ausgeben (TrMat, D-Zeilen, P-Zeile)
  irc = IGE_w_obj (&el, apt_typ, apt_ind, fp_o1, fp_o2);
  if (irc < 0) return irc;


  return 0;

}


//===========================================================================
  int IGE_w_obj (ObjGX *ox1,int aptTyp,long aptInd,FILE *fp_o1,FILE *fp_o2) {
//===========================================================================
// ein Obj ausgeben ..

  int    irc, oldTra;


  // printf("IGE_w_obj typ=%d ind=%d\n",aptTyp,aptInd);


  oldTra = IG_TrInd;

  // Ent. 124 f AC, CVELL
  IGE_w_mat (ox1, fp_o1, fp_o2);


  // D-Zeilen
  irc = IGE_w_rD (ox1, fp_o1);
  if(irc < 0) return irc;

  // P-Zeile
  IGE_w_rP (ox1, fp_o2);

  IG_TrInd = oldTra;


  // expNr ist die Iges-ObjID; aus IG_D_line_nr errchnen
  // expNr = IG_D_line_nr - 1;
  expNr = IGE_w_dNr2ind (IG_D_line_nr);


  // die zugehoerige gCad-typ- und Index speichern
  if(expNr >= expTabSiz) {
    TX_Error("IGE_w_D_Ln E001-overflow");
    return -1;
  }
  expTab[expNr].typ = aptTyp;
  expTab[expNr].ind = aptInd;
  ++expNr;  // sonst kein Zugriff auf letztes Obj !


  return 0;

}


/*=====================================================================*/
  long IGE_w_124 (Mat_4x3 m1, FILE *fp_o1, FILE *fp_o2) {
/*============
TrMat ausgeben
Retour: der Index.
*/

  static int igtyp = 124;





  /* ------------------------------------------------------------------ */
  // printf("IGE_w_124 %ld\n",IG_D_line_nr);



  //IGE_w_rD (el, fp_o1);
  IG_Pa1[0]=igtyp;

  /* Die P-Referenz */
  IG_Pa1[1] = IG_P_line_nr + 1;

  /* Font. 0=keiner, 1=Solid, 2=dashed, 4=centerline, 5=dotted. */
  IG_Pa1[3] = 1;

  /* TransformationsIndex */
  IG_Pa1[6] = 0;

  /* SUBORD.  independant */
  IG_Pa1[8] = 0;

  /* ----------------------------------------------------------- */
  IG_Pa2[0]=igtyp;

  /* Die Linethickness (Weight) in mikrometer */
  IG_Pa2[1] = 0;

  /* Die Farbe; 2=rot, 3=gruen, 4=blau, 5=gelb, 8=weiss. */
  IG_Pa2[2] = 0;

  /* 1, bei Text (E.212) 2. */
  IG_Pa2[3] = 1;

  IGE_w_D_Ln (fp_o1);



  //========================================================================= 
  /* Zugehoerige P-Zeile raus */
  //IGE_w_rP (el, fp_o2);
  /* clear line */
  IGE_w_rP1 (fp_o2, 0, 0, 0.);

  /* Zuerst EntityNr. */
  IGE_w_rP1 (fp_o2, 1, igtyp, 0.);

  // die Matrix raus
  IGE_w_rP1 (fp_o2, 2, 0, m1[0][0]);
  IGE_w_rP1 (fp_o2, 2, 0, m1[0][1]);
  IGE_w_rP1 (fp_o2, 2, 0, m1[0][2]);
  IGE_w_rP1 (fp_o2, 2, 0, m1[0][3]);

  IGE_w_rP1 (fp_o2, 2, 0, m1[1][0]);
  IGE_w_rP1 (fp_o2, 2, 0, m1[1][1]);
  IGE_w_rP1 (fp_o2, 2, 0, m1[1][2]);
  IGE_w_rP1 (fp_o2, 2, 0, m1[1][3]);

  IGE_w_rP1 (fp_o2, 2, 0, m1[2][0]);
  IGE_w_rP1 (fp_o2, 2, 0, m1[2][1]);
  IGE_w_rP1 (fp_o2, 2, 0, m1[2][2]);
  IGE_w_rP1 (fp_o2, 2, 0, m1[2][3]);



  /* zeilenrest raus */
  IGE_w_rP1 (fp_o2, 3, 0, 0.);


  // die P-Block# der Tranformation = die D-Zeilen# der Transformation.
  return IG_D_line_nr-1;

}




/*=====================================================================*/
  int IGE_w_D_Ln (FILE *fp_o1) {
/*==============
  Einen (fertigen) D-Block rausschreiben (besteht aus zwei Einzelzeilen).
  Die Zeilen bestehen aus je 9 Zahlen a 8 Ziffern; (in der zweiten Zeile
    ist die vorletzte jedoch ein Text) Spalte 73=D; 74-80 die Line#.
*/


  int i1, i2;


  //TX_Print("IGE_w_D_Ln");


 
  /* --------------- Zeile 1 ------------------------------------- */
  /* Die 9 Parameter aus IG_Pa1 in die Zeile 1 einsetzen. */
  for (i1=0; i1<8; i1++)
  {
    i2=i1*8;
    sprintf(&IGlnD1[i2],"%8d",IG_Pa1[i1]);
  }

  // der letzte Parameter ist der Status; es gibt:
  // 01010000 fuer alle physDep Obj's (Bestandteile von CCV',s gelochten Flaech)
  // 00000000 fuer alle anderen
  // subordinate switch: (dig. 3 & 4 in Feld 9) indicate physical dependency.
  // CatV5: xx01xxxx bewirkt dass Obj gar nicht ausgegeben wird !!
  // CatV5: 01xxxxxx bewirkt dass Obj hidden wird ! (wird dzt nicht ausgegeben)
  if(IG_mode == 1) {
    sprintf(&IGlnD1[64],"01010000");
  } else {
    // sprintf(&IGlnD1[64],"00000000");
    sprintf(&IGlnD1[64],"        ");
  }


   /* Zeile 1: Von 74-80 die Zeilen# */
  strcpy(&IGlnD1[72],"D");
  IG_D_line_nr++;
  sprintf(&IGlnD1[73],"%7d",IG_D_line_nr);



  /* --------------- Zeile 2 ------------------------------------- */
  /* Die 9 Parameter aus IG_Pa2 in die Zeile 2 einsetzen. */
  /* Paramter # 6, 7 u. 8 sind immer Blanks. */
  for (i1=0; i1<10; i1++)
  {
    i2=i1*8;
    if ((i1 > 5) && (i1 < 9)) strcpy(&IGlnD2[i2],"        ");
    else sprintf(&IGlnD2[i2],"%8d",IG_Pa2[i1]);
  }


  /* Zeile 2: Von 74-80 die Zeilen# */
  strcpy(&IGlnD2[72],"D");
  IG_D_line_nr++;
  sprintf(&IGlnD2[73],"%7d",IG_D_line_nr);


  /* die 2 zeilen raus */
  // printf("out1: |%s|\n",IGlnD1);
  fprintf(fp_o1,"%s\n",IGlnD1);
  // printf("out1: |%s|\n",IGlnD2);
  fprintf(fp_o1,"%s\n",IGlnD2);


  return 1;
}




/*=====================================================================*/
  int IGE_w_rD (ObjGX *el, FILE *fp_o1)
/*==============

  Eine D-Blockzeile rausschreiben (besteht aus zwei Einzelzeilen).
  Die Zeilen bestehen aus je 9 Zahlen a 8 Ziffern; (in der zweiten Zeile
    ist die vorletzte jedoch ein Text) Spalte 73=D; 74-80 die Line#.
*/

{

  int   i2, cadtyp, igtyp;
  ObjGX *ox1, *ox2;


  /* -------------------------------------------------------------- */

  /* fix Entitytyp */
  // printf("IGE_w_rD typ=%d form=%d siz=%d\n",el->typ,el->form,el->siz);


  if(el->typ == Typ_ObjGX) goto L_1;




  // CCV wird einzeln ausgegeben
  if(el->typ == Typ_CVCCV2) return -1;   // 21


  cadtyp = el->typ;
  goto L_2;



  //
  //----------ObjGX--------------------
  L_1:
  cadtyp = el->form;


  L_2:
  igtyp  = -1;


  // den IG-Typ (Entity-Nr) aus dem gCAD-Typ bestimmen.
  for(i2=0; IG_TypTab[i2] != -99; i2++) {
    if(IG_TypTab[i2] == cadtyp) {
      igtyp = IG_EntTab[i2];
      break;
    }
  }
    printf(" cadtyp=%d igtyp=%d\n",cadtyp,igtyp);


  // gCAD-Typ Typ_SUR wird als IG_EntSRU oder als IG_EntSRC ausgegeben.
  if(igtyp == IG_EntSRU) {
    ox1 = el->data;
    ox2 = &ox1[1];  // obj # 2
    if(ox2->typ == Typ_VC) {
      igtyp = IG_EntSRC;
    }

  } else if(igtyp == IG_EntSRV) {
    ox1 = el->data;
  }




  if(igtyp < 0) {
    TX_Print("ERROR: IGE_w_rD - Typ %d - %d not supported",el->typ,el->form);
    return -1;
  }







  /* ----------------------------------------------------------- */
  L_3:
  IG_Pa1[0]=igtyp;

  /* Die P-Referenz */
  IG_Pa1[1] = IG_P_line_nr + 1;

  /* Font. 0=keiner, 1=Solid, 2=dashed, 4=centerline, 5=dotted. */
  IG_Pa1[3] = 1;

  /* TransformationsIndex */
  IG_Pa1[6] = IG_TrInd;

  /* SUBORD.  independant */
  IG_Pa1[8] = 0;

  /* ----------------------------------------------------------- */
  IG_Pa2[0]=igtyp;

  /* Die Linethickness (Weight) in mikrometer */
  IG_Pa2[1] = 0;

  /* Die Farbe; 2=rot, 3=gruen, 4=blau, 5=gelb, 8=weiss. */
  IG_Pa2[2] = 0;

  /* 1, bei Text (E.212) 2. */
  IG_Pa2[3] = 1;


  // if (igtyp == IG_EntPT) {
  // } else if (igtyp == IG_EntLN) {


  if(igtyp == IG_EntPOL) {  // form-number
    if((cadtyp == Typ_CVPOL)   ||
       (cadtyp == Typ_CVCLOT))    {
      IG_Pa2[4] = 12;         // copious data-form 12=3D-Polygon
    // } else if(cadtyp == Typ_CVPOL2) {
    } else {
      IG_Pa2[4] = 11;         // copious data-form 11=2D-polygon;
    }


  } else if(cadtyp == Typ_CVELL) {
    IG_Pa2[4] = 1;         // copious data-form 1=Ellipse;


  } else {
    IG_Pa2[4] = 0;
  }


  // den D-Block raus 
  IGE_w_D_Ln (fp_o1);


  return 1;
}



/*=====================================================================*/
  int IGE_w_rP (ObjGX *el, FILE *fp_o2)
/*===========

  Eine P-Blockzeile rausschreiben. Diese Zeile kann sich ueber mehrere
    Zeilen erstrecken.
*/

{

  int      igtyp, rNr;
  // long     l1;
  // double   pd;
  Point    pa, pe, pm;
  // Line     *ln1;
  Circ     *ci1;
  ObjGX    ox1;
  // ObjX     xu;
  void     *xu;



  /* --------------------------------------------------------- */
  igtyp = IG_Pa1[0];
  // printf("IGE_w_rP %d\n",igtyp);


  /* clear line */
  IGE_w_rP1 (fp_o2, 0, 0, 0.);

  /* Zuerst EntityNr. */
  IGE_w_rP1 (fp_o2, 1, igtyp, 0.);



  /* --------------- Point -------------------------------------- */
  if (igtyp == IG_EntPT) {
    // add Point-Coords
    UTO_obj_getp (&xu, &rNr, el);
    IGE_w_rPP (fp_o2, (Point*)xu);
    /* add (3.) int - 0 */
    IGE_w_rP1 (fp_o2, 1, 0, 0.);



  /* --------------- Line -------------------------------------- */
  } else if (igtyp == IG_EntLN) {
    UTO_obj_getp (&xu, &rNr, el);
    // ln1 = el->data;
    // IGE_w_rPP (fp_o2, &ln1->p1);
    // IGE_w_rPP (fp_o2, &ln1->p2);
    IGE_w_rPP (fp_o2, &((Line*)xu)->p1);
    IGE_w_rPP (fp_o2, &((Line*)xu)->p2);


 
  /* --------------- Arc -------------------------------------- */
  } else if (igtyp == IG_EntCI) {
    // ci1 = el->data;
    UTO_obj_getp (&ci1, &rNr, el);
    // UT3D_stru_dump (Typ_CI, ci1, " exp:");
    pm = ci1->pc;


    // CW-Kreise umdrehen
    // nein; dir trMAt umdrehen !!
    // if(ci1->rad > 0.) {
      pa = ci1->p1;
      pe = ci1->p2;

    // } else {
      // pa = ci1->p2;
      // pe = ci1->p1;
    // }


    /* 100, mz, mx,my,  ax,ay, ex,ey, 0,0; */
    IGE_w_rP1 (fp_o2, 2, 0, pm.z);

    IGE_w_rP1 (fp_o2, 2, 0, pm.x);
    IGE_w_rP1 (fp_o2, 2, 0, pm.y);

    IGE_w_rP1 (fp_o2, 2, 0, pa.x);
    IGE_w_rP1 (fp_o2, 2, 0, pa.y);

    IGE_w_rP1 (fp_o2, 2, 0, pe.x);
    IGE_w_rP1 (fp_o2, 2, 0, pe.y);





  //---------------- Plane --------------------------------
  } else if (igtyp == IG_EntPLN) {
    IGE_w_P_PLN (el, fp_o2);



  //---------------- Ellipse --------------------------------
  } else if (igtyp == IG_EntELL) {
    IGE_w_P_ELL (el, fp_o2);


  /* --------------- Polygon -------------------------------- */
  } else if (igtyp == IG_EntPOL) {
    if(IG_Pa2[4] == 11) {               // 2D-Polygon
      IGE_w_P_POL2 (el, fp_o2);
    } else {
      IGE_w_P_POL (el, fp_o2);          // 3D-Polygon
    }



  /* --------------- Param.Spline -------------------------------- */
  } else if (igtyp == IG_EntPS) {
    IGE_w_P_PS (el, fp_o2);


  /* --------------- B-Spline -------------------------------- */
  } else if (igtyp == IG_EntBS) {
    IGE_w_P_BS (el, fp_o2);


  // --------------- Composite Curve -------------------------
  } else if (igtyp == IG_EntCCV) {
    IGE_w_P_CCV (el, fp_o2);


  // --------------- General Note ---------------------------- 
  } else if (igtyp == IG_EntTX) {
    IGE_w_P_TX (el, fp_o2);


  // --------------- RuledSurf ----------------------------
  } else if (igtyp == IG_EntSRU) {
    IGE_w_P_SRU (el, fp_o2);


  // --------------- RevSurf ----------------------------
  } else if (igtyp == IG_EntSRV) {
    IGE_w_P_SRV (el, fp_o2);


  // --------------- BSP-Surf ----------------------------
  } else if (igtyp == IG_EntSBS) {
    IGE_w_P_SBS (el, fp_o2);


  // --------------- Surf ----------------------------
  } else if (igtyp == IG_EntSUR) {
    IGE_w_P_144 (el, fp_o2);


  // --------------- SurfBoundary ----------------------------
  } else if (igtyp == IG_EntBND) {
    IGE_w_P_142 (el, fp_o2);


  // --------------- CylindSurf ----------------------------
  } else if (igtyp == IG_EntSRC) {
    IGE_w_P_SRC (el, fp_o2);



  // --------------- SubfigInst. ----------------------------
  } else if (igtyp == IG_EntMR) {
    IGE_w_P_MR (el, fp_o2);



  // --------------- SubfigDef. ----------------------------
  } else if (igtyp == IG_EntMD) {
    IGE_w_P_MD (el, fp_o2);






  } else {
    TX_Error("IGE_w_rP - Typ %d nicht unterstuetzt",igtyp);
    return 1;
  }


 

  /* --------------- Fertig -------------------------------------- */
  /* add 2 int - 0 */
  //IGE_w_rP1 (fp_o2, 1, 0, 0.);
  //IGE_w_rP1 (fp_o2, 1, 0, 0.);

  /* zeilenrest raus */
  IGE_w_rP1 (fp_o2, 3, 0, 0.);

  return 1;
}


//===================================================================
  int IGE_w_subObjs (ObjGX *el, FILE *fp_o1, FILE *fp_o2) {
//===================================================================
// all SubObj ausgeben; 
// die Iges-ID in Tabelle zwischenspeichern und dann mit dem Vaterobjekt raus.

  int       irc, i1, oNr;
  ObjGX     *oTab, *ox2, *ox3;


  IG_mode  = 1;             // 0=normales Obj; 1=SubObj (von CCV ..)


  // printf("IGE_w_subObjs typ=%d form=%d\n",el->typ,el->form);


  IG_subObjTab = (int*)memspc51;
  IG_subObjNr = 0;


  if(el->typ == Typ_CVCCV) {
    return IGE_w_CVCCV (el, fp_o1, fp_o2);


  } else if(el->typ == Typ_SUR) {
    return IGE_w_SUR (el, fp_o1, fp_o2);


  }

  return 0;


}


//===================================================================
  int IGE_w_SUR (ObjGX *el, FILE *fp_o1, FILE *fp_o2) {
//===================================================================
// alle SubObj fuer 144 ausgeben;
// die Iges-ID's in Tabelle IG_subObjTab zwischenspeichern (werden dann mit
// dem Vaterobjekt ausgegeben


  int    ii, i1, typ, ptNr, iTab[2], ibs, ica, ici;
  long   ind;
  Point  *pTab, pt1;
  Vector vc1;
  Plane  pl1;
  ObjGX  ox1, *oxp, *oTab;
  SurStd *sus;
  Memspc memSeg1;


  // printf("IGE_w_SUR typ=%d form=%d siz=%d\n",el->typ,el->form,el->siz);



  //======================================================================
  // Stuetzflaeche -> IG_subObjTab[0]

  //------------------------------------------
  if(el->form == Typ_SUR) {

    UME_init (&memSeg1, memspc55, sizeof(memspc55));

    // Point+vektor von planarer Surface ..
    UT3D_ptvc_sus (&pt1, &vc1, &memSeg1, el->data);
    // Point+vektor -> Plane
    UT3D_pl_ptvc (&pl1, &pt1, &vc1);


    // el->data ist eine SurStd
    sus = el->data;
    // printf(" bound typ=%d ind=%d\n",sus->cvTyp,sus->cvID);

    // Iges-Pointer of Bound suchen ..
    ica = IGE_w_getPtr (sus->cvTyp, sus->cvID);
    if(ica < 0) {
      printf("**** IGE_w_SUR E001\n");
      return -1;
    }



  //================================================================
  // Flaeche mit Supportflaeche
  } else if(el->form == Typ_ObjGX) {

    // minimum: Stuetzfl u outer bound.
    if(el->siz < 2) {
      printf("**** IGE_w_SUR ObjGX siz<2\n");
      return -1;
    }

    pTab = (Point*)memspc201;
    ptNr = sizeof(memspc201) / sizeof(Point);


    oTab = el->data;



    //----------------------------------------------------------------
    // erstes Obj = Stuetzflaeche; dann outer bound; dann inner bounds.
    oxp = &oTab[0];
    // printf(" Stuetzflaeche: %d %d\n",oxp->typ,oxp->form);


    // Stuetzflaeche decodieren
    // Planare surf:
    ii = 0;
    if(oxp->typ == Typ_Typ) {
      ii = OGX_GET_INT (oxp);
    }
    if(((ii == Typ_SURPLN)) ||
       ((oxp->typ == Typ_CV)&&(oxp->form == Typ_Index))) {

      // Aussenkontur holen
      i1 = UT3D_npt_ox (&ptNr, pTab, &oTab[1], UT_DISP_cv); // Kontur 1
      if(i1 < 0) return i1;


      // Polygon -> Plane
      UT3D_pl_pta (&pl1, ptNr, pTab);

      // Plane -> Obj; ausgeben
      ox1.typ   = Typ_PLN;
      ox1.form  = Typ_PLN;
      ox1.siz   = 1;
      ox1.data  = (void*)&pl1;
      IGE_w_obj (&ox1, 0, 0, fp_o1, fp_o2);
    
      ibs = IG_D_line_nr-1;     // Index Stuetzflaeche (basic surf)

      if(oxp->typ == Typ_Typ) ica = 1;   // index of 1. contour
      else                    ica = 0;



    } else if((oxp->typ == Typ_SUR)&&(oxp->form == Typ_Index)) {

      // pointer der Stuetzflaeche beschaffen
      i1 = UTO_ind_get (&typ, &ind, oxp);
      if(i1 < 0) return i1;
      ibs = IGE_w_getPtr (typ, ind);
        if(ibs < 0) {
        TX_Error("**** IGE_w_SUR E002");
        return ibs;
      }
      ica = 1;   // index of 1. contour



    } else {

      // ibs = IGE_w_getPtr (oxp->typ, ind);
      // if(ibs < 0) {
        TX_Error("**** IGE_w_SUR E003 %d %d",oxp->typ,oxp->form);
        return -1;
      // }

    }


    //----------------------------------------------------------------
    // Aussen, Innenkonturen
    IG_subObjTab[0] = ibs;             // Index Stuetzflaeche
    IG_subObjNr = 1;

    for(ii=ica; ii<el->siz; ++ii) {
      oxp = &oTab[ii];
      // printf(" c[%d] typ=%d form=%d siz=%d\n",ii,oxp->typ,oxp->form,oxp->siz);

      // pointer der outer Bound beschaffen
      i1 = UTO_ind_get (&typ, &ind, oxp);
      if(i1 < 0) return i1;
      ici = IGE_w_getPtr (typ, ind);
        if(ici < 0) {
        TX_Error("**** IGE_w_SUR E005");
        return ici;
      }

      ici = IGE_w_142 (ibs, ici, fp_o1, fp_o2);
      if(ici < 0) return ici;

      // IGE_w_dump_expTab();
      IG_subObjTab[IG_subObjNr] = ici;
      ++IG_subObjNr;
    }

  }


  return 0;

}


//===================================================================
  int IGE_w_CVCCV (ObjGX *el, FILE *fp_o1, FILE *fp_o2) {
//===================================================================
// all SubObj ausgeben;
// die Iges-ID in Tabelle zwischenspeichern und dann mit dem Vaterobjekt raus.

  int       irc, i1, oNr;
  ObjGX     *oTab, *ox2, *ox3;


  // skip single obejcts ..
  if(el->siz < 2) return 0;


  oNr = el->siz;


  // printf("IGE_w_CVCCV typ=%d form=%d %d\n",el->typ,el->form,oNr);


  oTab  = el->data;


  // if(el->form == Typ_ObjGX) ??



  // siehe UT3D_pta_ccv
  for(i1=0; i1<oNr; ++i1) {

    ox2 = &oTab[i1];
    // printf(" CCV[%d] typ=%d form=%d\n",i1, ox2->typ,ox2->form);
    // UTO_dump__ (ox2, "CCV[%d]",i1);

    if(ox2->typ == Typ_PT) {
      IGE_w_obj (ox2, 0, 0, fp_o1, fp_o2);

    } else if(ox2->typ == Typ_LN) {
      IGE_w_obj (ox2, 0, 0, fp_o1, fp_o2);

    } else if(ox2->typ == Typ_CI) {
      IGE_w_obj (ox2, 0, 0, fp_o1, fp_o2);

    } else if(ox2->typ == Typ_CVELL) {
      ox3 = DB_GetCurv ((long)ox2->data);
      IGE_w_obj (ox3, 0, 0, fp_o1, fp_o2);

    } else if(ox2->typ == Typ_CVCLOT) {
      ox3 = DB_GetCurv ((long)ox2->data);
      IGE_w_obj (ox3, 0, 0, fp_o1, fp_o2);

    } else if(ox2->typ == Typ_CVBSP) {
      ox3 = DB_GetCurv ((long)ox2->data);
      IGE_w_obj (ox3, 0, 0, fp_o1, fp_o2);


    } else if(ox2->typ == Typ_CVPOL) {
      ox3 = DB_GetCurv ((long)ox2->data);
      IGE_w_obj (ox3, 0, 0, fp_o1, fp_o2);


    } else {
      TX_Error("**** IGE_w_CVCCV E001_%d",ox2->typ);
    }



    // save IgesObjID fuer nachfolgende ObjTabelle
    IG_subObjTab[IG_subObjNr] = IG_D_line_nr-1;
    ++IG_subObjNr;

  }

  return 0;

}


//===================================================================
  int IGE_w_P_MD (ObjGX *el, FILE *fp_o2) {
//===================================================================
// 308. (SubfigDef.)
// alle Pointer von IG_D_line_mod bis hierher ausgeben.


  int      i1, iNr;
  ModelBas *mdb;


  mdb = el->data;

  printf("IGE_w_P_MD |%s|\n",mdb->mnam);


  // Depth (immer 0)
  IGE_w_rP1 (fp_o2, 1, 0, 0.);

  // Name
  IGE_w_rPT (fp_o2, mdb->mnam);

  // momentane LineNr = IG_D_line_nr;
  printf("D_line_nr=%d\n",IG_D_line_nr);


  // number of pointers
  iNr = (IG_D_line_nr - IG_D_line_mod - 2) / 2;
  IGE_w_rP1 (fp_o2, 1, iNr, 0.);


  // pointers
  for(i1=0; i1<iNr; ++i1) {
    IGE_w_rP1 (fp_o2, 1, (i1 * 2) + IG_D_line_mod + 1, 0.);
  }

  // offset for netx subModel
  IG_D_line_mod = IG_D_line_nr;

  IG_modTab[IG_modNr].modNr = DB_get_ModNr (mdb->mnam);
  IG_modTab[IG_modNr].lNr   = IG_D_line_nr - 1;

  if(IG_modNr < IG_modMax) {
    ++IG_modNr;
  } else {
    TX_Error("IGE_w_P_MD E001-overflow");
  }

  return 0;

}



//===================================================================
  int IGE_w_P_MR (ObjGX *el, FILE *fp_o2) {
//===================================================================
// 408. (SubfigInst.)

  int        i1, iNr;
  double     d1;
  ModelRef   *mr1;


  mr1 = el->data;

  printf("IGE_w_P_MR\n");


  // die D-LineNr des Submodel mr1->modNr suchen
  for(i1=0; i1<IG_modNr; ++i1) {
    if(IG_modTab[i1].modNr == mr1->modNr) { iNr = IG_modTab[i1].lNr; break;}
  }

  // Pointer zur SubfigDefinition
  IGE_w_rP1 (fp_o2, 1, iNr, 0.);


  // Position 0,0,0  (via Trafo(124) setzen!)
  IGE_w_rP1 (fp_o2, 2, 0, 0.);
  IGE_w_rP1 (fp_o2, 2, 0, 0.);
  IGE_w_rP1 (fp_o2, 2, 0, 0.);


  // Scale
  d1 = mr1->scl;
  if(fabs(d1) < 0.0001) d1 = 1.;
  IGE_w_rP1 (fp_o2, 2, 0, d1);
  
  return 0;

}



//===================================================================
  int IGE_w_P_SRC (ObjGX *el, FILE *fp_o2) {
//===================================================================
// 122 Zylinderflaeche
//     Pointer to curve
//     Point to Endpunkt, wenn Startpunkt der Startpunkt der Curve waere.

  int    typ, rNr;
  long   ind, dNr;
  Point  pt1, pt2;
  // Vector vc1;
  ObjGX  *objTab, *obj1, *obj2;
  // ObjX   ox1;
  void   *ox1;


  objTab = el->data;
  obj1   = &objTab[0];
  obj2   = &objTab[1];

  // printf("IGE_w_P_SRC\n");
  // printf(" typ1=%d typ2=%d\n",obj1->typ,obj2->typ);
  // printf(" form=%d form=%d\n",obj1->form,obj2->form);



  if(obj1->form == Typ_Index) {
    typ = obj1->typ;
    ind = (long)obj1->data;
    // printf(" o1: typ=%d ind=%d\n",typ,ind);
    // get pointer to obj(typ/ind)
    dNr = IGE_w_getPtr (typ, ind);
    if(dNr < 0) {
      printf("**** IGE_w_P_SRU E002\n");
      return 0;
    }
    // write pointer
    IGE_w_rP1 (fp_o2, 1, dNr, 0.);
  } else {
    printf("**** IGE_w_P_SRU E001\n");
    return 0;
  }


  // den Endpunkt von Obj1 -> pt1
  UTO_2pt_lim_dbo (&pt1, typ, ind, 1);  //// mode; 1=Startpt 

  // den Vektor -> vc1
  // UTO_obj_get (&vc1, obj2);
  UTO_obj_getp (&ox1, &rNr, obj2);

  // pt1 + cv1 -> pt2
  // UT3D_pt_traptvc (&pt2, &pt1, &vc1);
  UT3D_pt_traptvc (&pt2, &pt1, (Vector*)ox1);

  // Point to Endpunkt
  IGE_w_rP1 (fp_o2, 2, 0, pt2.x);
  IGE_w_rP1 (fp_o2, 2, 0, pt2.y);
  IGE_w_rP1 (fp_o2, 2, 0, pt2.z);


  return 0;

}


//===================================================================
  int IGE_w_142 (int ibs, int ica, FILE *fp_o1, FILE *fp_o2) {
//===================================================================
// write 142

  int    iTab[2];
  ObjGX  ox1;

  iTab[0] = ibs; // Pointer to Stuetzflaeche
  iTab[1] = ica; // Pointer to 3D-Curve

  ox1.typ   = Typ_goGeo1;
  ox1.form  = Typ_Index;
  ox1.siz   = 2;
  ox1.data  = (void*)&iTab;
  IGE_w_obj (&ox1, 0, 0, fp_o1, fp_o2);


  return IG_D_line_nr-1;

}


//===================================================================
  int IGE_w_P_142 (ObjGX *el, FILE *fp_o2) {
//===================================================================
// 142 CurveOnParametricSurface
//     (int) CreationFlag 0=unspec;1=proj.Curv->Surf;2=Inters.Surf/Surf;
//             3=IsoparametricCurve.
//     Pointer to Stuetzflaeche (Surf on which curv lies); zB auf 108 od 128
//     Ptr auf die 2D-Ableitung der Curve; dzt immer 0.
//     Ptr auf die 3D-Curve Boundary (zB CCV - 102) od 100-Circ!
//     (int)   Darstellung:0=unspec;1=2D;2=3D;3:2D od 3D; dzt immer 3.

  int    *iTab;

  // printf("IGE_w_P_142\n");

  iTab = el->data;

  IGE_w_rP1 (fp_o2, 1, 0, 0.);       // CreationFlag: 0=unspec.
  IGE_w_rP1 (fp_o2, 1, iTab[0], 0.); // Pointer to Stuetzflaeche
  IGE_w_rP1 (fp_o2, 1, 0, 0.);       // Ptr auf die 2D-Ableitung = 0.
  IGE_w_rP1 (fp_o2, 1, iTab[1], 0.); // Ptr auf die 3D-Curve Boundary
  IGE_w_rP1 (fp_o2, 1, 2, 0.);       // Darstellung:  2=3D

  return 0;

}



//===================================================================
  int IGE_w_P_144 (ObjGX *el, FILE *fp_o2) {
//===================================================================
// 144 Trimmed Parametric Surface
//  0  Ptr auf die Stuetzflaeche; zB auf 102 od 128
//  1  TrimmedFlag; 0=untrimmed, 1=trimmed
//  2  IBN = Anzahl Inseln innerhalb der Boundary
//  3  Ptr auf die Boundary; (-> 142)
//     Ptr[IBN] auf die Inseln ..

  int    i1, *iTab, cNr;

  iTab = &IG_subObjTab[0];

  // printf("IGE_w_P_144 %d %d\n",iTab[0],iTab[1]);

  cNr  = IG_subObjNr - 2;
  

  IGE_w_rP1 (fp_o2, 1, iTab[0], 0.);      // Pointer to Stuetzflaeche
  IGE_w_rP1 (fp_o2, 1, 1, 0.);            // TrimmedFlag; 1=trimmed
  IGE_w_rP1 (fp_o2, 1, cNr, 0.);          // Anzahl Inseln
  IGE_w_rP1 (fp_o2, 1, iTab[1], 0.);      // Pointer to BoundaryAussen
  for(i1=0; i1<cNr; ++i1)
    IGE_w_rP1 (fp_o2, 1, iTab[i1+2], 0.); // Pointer to BoundaryInnen

  return 0;

}


//===================================================================
  int IGE_w_P_SRV (ObjGX *el, FILE *fp_o2) {
//===================================================================
// 120 Revolved Surf
//     Pointer to Axis (Line)
//     Pointer to Contour (Line=Conus, Circle=Torus ..)
//     StartAngle (0.)
//     TerminateAngle (6.28)

  int     typ;
  long    ind, dNr;
  SurRev  *srv;


  srv = el->data;

  // printf("IGE_w_P_SRV %f %f\n",srv->ang1,srv->ang2);
  // printf("   axis typ=%d ind=%d\n",srv->typCen,srv->indCen);
  // printf("   cont typ=%d ind=%d\n",srv->typCov,srv->indCov);


    // get pointer to obj(typ/ind)
    dNr = IGE_w_getPtr (srv->typCen,srv->indCen);
    if(dNr < 0) {
      printf("**** IGE_w_P_SRV E001\n");
      return 0;
    }
    // write pointer
    IGE_w_rP1 (fp_o2, 1, dNr, 0.);


    // get pointer to obj(typ/ind)
    dNr = IGE_w_getPtr (srv->typCov,srv->indCov);
    if(dNr < 0) {
      printf("**** IGE_w_P_SRV E002\n");
      return 0;
    }
    // write pointer
    IGE_w_rP1 (fp_o2, 1, dNr, 0.);


  // write angles
  IGE_w_rP1 (fp_o2, 2, 0, srv->ang1);
  IGE_w_rP1 (fp_o2, 2, 0, srv->ang2);



  return 0;

}



//===================================================================
  int IGE_w_P_SBS (ObjGX *el, FILE *fp_o2) {
//===================================================================
// 128 Rational B-Spline Surface
//     K1 (int) upper index of sum
//     K2 (int) upper index of sum
//     M1 (int) degree of basic functions
//     M2 (int) degree of basic functions
//     Prop1 (int) 0=not closed in 1.dir, 1=closed
//     Prop2 (int) 0=not closed in 2.dir, 1=closed
//     Prop3 (int) 0=rational, 1=polynomial
//     Prop4 (int) 0=nonperodic in 1.dir, 1=periodic.
//     Prop5 (int) 0=nonperodic in 2.dir, 1=periodic.
//     T[]   (floats) 1. KnotSequence Anzahl=K-M+1+2*M
//     T[]   (floats) 2. KnotSequence Anzahl=K-M+1+2*M
//     W[]   (floats) Weigths, Anzahl=K
//     X,Y,Z (3 floats) CtrlPoints, Anzahl=K
//     V(0)  (float) 1. StartParameterWert
//     V(1)  (float) 1. EndParameterWert
//     V(0)  (float) 2. StartParameterWert
//     V(1)  (float) 2. EndParameterWert


  int       i1, i2, i3;
  double    *dTab;
  Point     *pTab;
  SurBSpl   *sbs;
// typedef struct {long ptUNr, ptVNr, degU, degV;
                // Point *cpTab; double *kvTabU, *kvTabV;}             SurBSpl;

// cpTab[ptUNr*ptVNr]    u0v0,u1v0..uUv0;u1v0,u1v1..uUv1;..;uUv0,uUv1..uUvV;
// kvTabU[ptUNr+degU+1]
// kvTabV[ptVNr+degV+1]



  // printf("IGE_w_P_SBS %d %d\n",el->typ,el->form);


  if(el->form != Typ_SURBSP) {printf("*** IGE_w_P_SBS E001 ***\n");return -1;}

  sbs = el->data;

  // write sumIndex's
  IGE_w_rP1 (fp_o2, 1, sbs->ptUNr - 1, 0.);
  IGE_w_rP1 (fp_o2, 1, sbs->ptVNr - 1, 0.);

  // write degrees
  IGE_w_rP1 (fp_o2, 1, sbs->degU, 0.);
  IGE_w_rP1 (fp_o2, 1, sbs->degV, 0.);

  IGE_w_rP1 (fp_o2, 1, 0, 0.); // 0=not closed
  IGE_w_rP1 (fp_o2, 1, 0, 0.); // 0=not closed
  IGE_w_rP1 (fp_o2, 1, 1, 0.); // 1=polynom.
  IGE_w_rP1 (fp_o2, 1, 0, 0.); // 0=nonperodic
  IGE_w_rP1 (fp_o2, 1, 0, 0.); // 0=nonperodic

  // write U-knots
  i1 = sbs->ptUNr + sbs->degU + 1;
  dTab = sbs->kvTabU;
  for(i2=0; i2<i1; ++i2) {
    IGE_w_rP1 (fp_o2, 2, 0, dTab[i2]);
  }

  // write V-knots
  i1 = sbs->ptVNr + sbs->degV + 1;
  dTab = sbs->kvTabV;
  for(i2=0; i2<i1; ++i2) {
    IGE_w_rP1 (fp_o2, 2, 0, dTab[i2]);
  }

  // write weigths

  for(i1=0; i1<sbs->ptVNr; ++i1) {
    for(i2=0; i2<sbs->ptUNr; ++i2) {
      IGE_w_rP1 (fp_o2, 1, 1, 0.);
    }
  }



  // wr CtrlPoints
  pTab = sbs->cpTab;
  i3 = 0;
  for(i1=0; i1<sbs->ptVNr; ++i1) {
    for(i2=0; i2<sbs->ptUNr; ++i2) {
      IGE_w_rP1 (fp_o2, 2, 0, pTab[i3].x);
      IGE_w_rP1 (fp_o2, 2, 0, pTab[i3].y);
      IGE_w_rP1 (fp_o2, 2, 0, pTab[i3].z);
      ++i3;
    }
  }


  // wr U-start/endVal
  dTab = sbs->kvTabU;
  IGE_w_rP1 (fp_o2, 2, 0, dTab[0]);
  i1 = sbs->ptUNr + sbs->degU;
  IGE_w_rP1 (fp_o2, 2, 0, dTab[i1]);


  // wr V-start/endVal
  dTab = sbs->kvTabV;
  IGE_w_rP1 (fp_o2, 2, 0, dTab[0]);
  i1 = sbs->ptVNr + sbs->degV;
  IGE_w_rP1 (fp_o2, 2, 0, dTab[i1]);


  return 0;

}


//===================================================================
  int IGE_w_P_SRU (ObjGX *el, FILE *fp_o2) {
//===================================================================
// 118 RuledSurface
//     Pointer 1. curve
//     Pointer 2. curve
//     (int)   direction; 0=first to first, 1=first to last
//     (int)   development flag: 1=developable, 0=possibly not

  int    typ;
  long   ind, dNr;
  ObjGX  *objTab, *obj1, *obj2;


  objTab = el->data;
  obj1   = &objTab[0];
  obj2   = &objTab[1];

  printf("IGE_w_P_SRU dir=%d\n",obj2->dir);
  printf(" typ1 =%d typ2 =%d\n",obj1->typ,obj2->typ);
  printf(" form1=%d form2=%d\n",obj1->form,obj2->form);


  if(obj1->form == Typ_Index) {
    typ = obj1->typ;
    ind = (long)obj1->data;
    printf(" o1: typ=%d ind=%ld\n",typ,ind);
    // get pointer to obj(typ/ind)
    dNr = IGE_w_getPtr (typ, ind);
    if(dNr < 0) {
      printf("**** IGE_w_P_SRU E002\n");
      return 0;
    }
    // write pointer
    IGE_w_rP1 (fp_o2, 1, dNr, 0.);
  } else {
    printf("**** IGE_w_P_SRU E001\n");
    return 0;
  }


  if(obj1->form == Typ_Index) {
    typ = obj2->typ;
    ind = (long)obj2->data;
    printf(" o2: typ=%d ind=%ld\n",typ,ind);
    // get pointer to obj(typ/ind)
    dNr = IGE_w_getPtr (typ, ind);
    if(dNr < 0) {
      printf("**** IGE_w_P_SRU E004\n");
      return 0;
    }
    // write pointer
    IGE_w_rP1 (fp_o2, 1, dNr, 0.);
  } else {
    printf("**** IGE_w_P_SRU E003\n");
    return 0;
  }

  //     (int)   direction; 0=first to first, 1=first to last
  IGE_w_rP1 (fp_o2, 1, obj2->dir, 0.);  // add (int) 1

  IGE_w_rP1 (fp_o2, 1, 0, 0.);  // add (int) 0

  return 0;

}


//===================================================================
  int IGE_w_P_POL (ObjGX *el, FILE *fp_o2) {
//===================================================================

  int        irc, ipt, ipol, ptNr;
  // PointTab   *cv1;
  Point      *ptTab;
  CurvPoly   *plg1;
  // Memspc     tmpSeg;


  // printf("IGE_w_P_POL *** typ=%d form=%d IGtyp=%d\n",
          // el->typ,el->form,IG_Pa2[4]);
  // printf("v0=%f v1=%f\n",((CurvPoly*)el->data)->v0,((CurvPoly*)el->data)->v1);

  // ipol  = ((CurvPoly*)el->data)->ptNr;
  // ptTab = ((CurvPoly*)el->data)->cpTab;




  // PolygonCurve -> relimited Polygon
  ptTab = (Point*)memspc55;
  ptNr  = sizeof(memspc55) / sizeof(Point);


  if((el->form == Typ_CVPOL)||(el->form == Typ_CVPOL2))  {
    UT3D_pta_plg (&ptNr, ptTab, (CurvPoly*)el->data);


  } else if(el->form == Typ_CVCLOT) {
    // UME_init (&tmpSeg, memspc201, sizeof(memspc201));
    irc = UT3D_npt_curvp (ptTab, &ptNr,
                          Typ_CVCLOT, (CurvClot*)el->data, UT_DISP_cv);
    if (irc < 0) return -1;


  } else {
    TX_Error("IGE_w_P_POL E001 %d",el->form);
    return -1;
  }




  ipt  = 0;


  IGE_w_rP1 (fp_o2, 1, 2, 0.);       // 1=2D-points, 2=3D-Points
  IGE_w_rP1 (fp_o2, 1, ptNr, 0.);    // Nr of points


  // die Punkte raus ...
  L_next:
    IGE_w_rP1 (fp_o2, 2, 0, ptTab[ipt].x);
    IGE_w_rP1 (fp_o2, 2, 0, ptTab[ipt].y);
    IGE_w_rP1 (fp_o2, 2, 0, ptTab[ipt].z);
    ++ipt;
    if(ipt < ptNr) goto L_next;


  L_fertig:
  return 0;

}


//===================================================================
  int IGE_w_P_POL2 (ObjGX *el, FILE *fp_o2) {
//===================================================================

  int        ipt, ipol, ptNr;
  // Point2Tab  *cv1;
  Point2     *ptTab;


  printf("IGE_w_P_POL *** form=%d\n",IG_Pa2[4]);


  // cv1   = el->data;
  ipol  = el->siz;
  ptTab = el->data;


  ipt  = 0;



  IGE_w_rP1 (fp_o2, 1, 1, 0.);       // 1=2D-points, 2=3D-Points
  IGE_w_rP1 (fp_o2, 1, ipol, 0.);    // Nr of points
  IGE_w_rP1 (fp_o2, 2, 0, 0.);       // Z-Val (nur f. 2D-points)


  L_next:
  IGE_w_rP1 (fp_o2, 2, 0, ptTab[ipt].x);
  IGE_w_rP1 (fp_o2, 2, 0, ptTab[ipt].y);
  ++ipt;
  if(ipt < ipol) goto L_next;


  L_fertig:
  return 0;

}


//===================================================================
  int IGE_w_P_BS (ObjGX *el, FILE *fp_o2) {
//===================================================================

  int      i1, i2;
  double   d1;
  CurvBSpl *cvBSpl;

  cvBSpl = el->data;

  // printf("IGE_w_B_PS %d %d\n",cvBSpl->ptNr,cvBSpl->deg);


  i1 = cvBSpl->ptNr - 1;
  IGE_w_rP1 (fp_o2, 1, i1, 0.);    // I Anzahl points
  i1 = cvBSpl->deg;
  IGE_w_rP1 (fp_o2, 1, i1, 0.);    // I Degree
  IGE_w_rP1 (fp_o2, 1, 0, 0.);     // I nonplanar
  IGE_w_rP1 (fp_o2, 1, 0, 0.);     // I open curve
  IGE_w_rP1 (fp_o2, 1, 1, 0.);     // I polynomial
  IGE_w_rP1 (fp_o2, 1, 0, 0.);     // I non-periodic

  // Knotvektor
  i2 = cvBSpl->ptNr + cvBSpl->deg + 1;
  for(i1=0; i1<i2; ++i1) {
    IGE_w_rP1 (fp_o2, 2, 0, cvBSpl->kvTab[i1]);     // I non-periodic
  }

  // weigths
  i2 = cvBSpl->ptNr;
  for(i1=0; i1<i2; ++i1) {
    IGE_w_rP1 (fp_o2, 1, 1, 0.);
  }

  // controlpoints
  i2 = cvBSpl->ptNr;
  for(i1=0; i1<i2; ++i1) {
    IGE_w_rP1 (fp_o2, 2, 1, cvBSpl->cpTab[i1].x);
    IGE_w_rP1 (fp_o2, 2, 1, cvBSpl->cpTab[i1].y);
    IGE_w_rP1 (fp_o2, 2, 1, cvBSpl->cpTab[i1].z);
  }

  // start/end-u-val
  // d1 = cvBSpl->kvTab[0];
  d1 = cvBSpl->v0;
  IGE_w_rP1 (fp_o2, 2, 0, d1);
  // d1 = cvBSpl->kvTab[cvBSpl->ptNr + cvBSpl->deg];
  d1 = cvBSpl->v1;
  IGE_w_rP1 (fp_o2, 2, 0, d1);

  // normalvektor 
  IGE_w_rP1 (fp_o2, 2, 0, 0.);
  IGE_w_rP1 (fp_o2, 2, 0, 0.);
  IGE_w_rP1 (fp_o2, 2, 0, 0.);


  return 0;

}



//===================================================================
  int IGE_w_P_PLN (ObjGX *ox, FILE *fp_o2) {
//===================================================================

  int          i1;
  double       d1, d2;
  Plane        *pl1;
  polcoeff_d3  co3;


  // printf("IGE_w_P_PLN\n");


  pl1 = (Plane*)ox->data;


  UT3D_plcoe_pl (&co3, pl1);


  IGE_w_rP1 (fp_o2, 2, 0, co3.a);                // coeff
  IGE_w_rP1 (fp_o2, 2, 0, co3.b);                // coeff
  IGE_w_rP1 (fp_o2, 2, 0, co3.c);                // coeff
  IGE_w_rP1 (fp_o2, 2, 0, co3.d);                // coeff
  IGE_w_rP1 (fp_o2, 1, 0, 0.);                   // Pointer to boundary
  IGE_w_rP1 (fp_o2, 2, 0, pl1->po.x);            // posi of symbol
  IGE_w_rP1 (fp_o2, 2, 0, pl1->po.y);            // posi of symbol
  IGE_w_rP1 (fp_o2, 2, 0, pl1->po.z);            // posi of symbol
  IGE_w_rP1 (fp_o2, 2, 0, 1.);                   // size of symbol

  return 0;

}



//===================================================================
  int IGE_w_P_ELL (ObjGX *ox, FILE *fp_o2) {
//===================================================================

  CurvElli     *el1;
  polcoeff_d5 ec;


  // printf("IGE_w_P_ELL\n");


  el1 = (CurvElli*)ox->data;


  UT3D_elcoe_el (&ec, el1);


  IGE_w_rP1 (fp_o2, 2, 0, ec.a);                // coeff
  IGE_w_rP1 (fp_o2, 2, 0, ec.b);                // coeff
  IGE_w_rP1 (fp_o2, 2, 0, ec.c);                // coeff
  IGE_w_rP1 (fp_o2, 2, 0, ec.d);                // coeff
  IGE_w_rP1 (fp_o2, 2, 0, ec.e);                // coeff
  IGE_w_rP1 (fp_o2, 2, 0, ec.f);                // coeff
  IGE_w_rP1 (fp_o2, 2, 0, el1->pc.z);           // Z-Val
  IGE_w_rP1 (fp_o2, 2, 0, el1->p1.x);           // pa
  IGE_w_rP1 (fp_o2, 2, 0, el1->p1.y);           // pa
  IGE_w_rP1 (fp_o2, 2, 0, el1->p2.x);           // pe
  IGE_w_rP1 (fp_o2, 2, 0, el1->p2.y);           // pe




  return 0;

}


//===================================================================
  int IGE_w_P_CCV (ObjGX *el, FILE *fp_o2) {
//===================================================================
// Ent.102: Nr of Pointers, Objpointers.
// muss nicht mit Punkt beginnen und nicht mit Punkt enden;
// es muessen aber alle Teilstuecke lueckenlos aneinander anschliessen
// Line kann verkehrt sein
// Schnittpunkte werden nicht automatisch gebildet


  int       i1, oNr;

  // oNr = el->siz;
  oNr = IG_subObjNr;

  // printf("IGE_w_P_CCV %d\n",oNr);

  // IGE_w_dump_expTab();  // Testdisplay ausgegebene Object-ID's ..


  IGE_w_rP1 (fp_o2, 1, oNr, 0.);                   // Nr of members

  for(i1=0; i1<oNr; ++i1) {
    IGE_w_rP1 (fp_o2, 1, IG_subObjTab[i1], 0.);    // members
  }


  return 0;

}


//===================================================================
  int IGE_w_P_TX (ObjGX *el, FILE *fp_o2) {
//===================================================================

  int        i1;
  double     d1, d2;
  GText      *tx1;


  // printf("IGE_w_P_TX\n");


  tx1 = (GText*)el->data;
  printf(" size=%f dir=%f\n",tx1->size,tx1->dir);
  printf(" txt=|%s|\n",tx1->txt);


  // d1=TextBoxBreite, d2=TextBoxLaenge
  i1=strlen(tx1->txt);

  if(tx1->size < 0.) {   // Alfatext
    // geht nicht wirklich;
    // d1 = i1 * 3 / GL_get_Scale();
    // d2 = 3 / GL_get_Scale();
    d1 = i1 * 3;
    d2 = 3;

  } else {              // 3D-Text
    d1 = i1 * (GL_vf1_CW + GL_vf1_CS);
    d2 = GL_vf1_CW + GL_vf1_CS;   // Höhe = Breite + space

    d1 *= tx1->size;
    d2 *= tx1->size;
  }

  printf(" d1=%f, d2=%f dir=%f\n",d1,d2,tx1->dir);


  IGE_w_rP1 (fp_o2, 1, 1, 0.);                   // Nr of strings = 1
  IGE_w_rP1 (fp_o2, 1, strlen(tx1->txt), 0.);    // Nr of chars
  IGE_w_rP1 (fp_o2, 2, 0, d1);                   // TextBoxBreite
  IGE_w_rP1 (fp_o2, 2, 0, d2);                   // TextBoxLaenge
  IGE_w_rP1 (fp_o2, 1, 1, 0.);                   // FontCode = 1
  IGE_w_rP1 (fp_o2, 2, 1, RAD_90);               // SlantAngle = Pi/2
  IGE_w_rP1 (fp_o2, 2, 1, UT_RADIANS(tx1->dir)); // Angle
  IGE_w_rP1 (fp_o2, 1, 0, 0.);                   // no Mirror = 0
  IGE_w_rP1 (fp_o2, 1, 0, 0.);                   // horiz. = 0
  IGE_w_rP1 (fp_o2, 2, 1, tx1->pt.x);            // Pos
  IGE_w_rP1 (fp_o2, 2, 1, tx1->pt.y);            // Pos
  IGE_w_rP1 (fp_o2, 2, 1, tx1->pt.z);            // Pos
  IGE_w_rPT (fp_o2, tx1->txt);                   // Text


  return 0;

}


//===================================================================
  int IGE_w_P_PS (ObjGX *el, FILE *fp_o2) {
//===================================================================

  int         i1, iNr;
  polynom_d3  *polTab;
  // CurvPol3    *cvpsp3;


  // printf("IGE_w_P_PS\n");


  IGE_w_rP1 (fp_o2, 1, 3, 0.); // Sourcetyp = 3
  IGE_w_rP1 (fp_o2, 1, 1, 0.); // Continuity = 1
  IGE_w_rP1 (fp_o2, 1, 3, 0.); // non-Planar = 3


  // cvpsp3 = el->data;
  polTab = el->data;

  // iNr = cvpsp3->polNr;
  iNr = el->siz;
  IGE_w_rP1 (fp_o2, 1, iNr-1, 0.); // Anz.Segments


  // Die Segmentoffsets raus
  for(i1=0; i1<iNr; ++i1) {
    // printf("segOff %d %f\n",i1,polTab[i1].u);
    IGE_w_rP1 (fp_o2, 2, 0, polTab[i1].u);
  }


  for(i1=0; i1<iNr; ++i1) {
    // printf("seg %d\n",i1);
    // die X-Koeffizienten
    IGE_w_rP1 (fp_o2, 2, 0, polTab[i1].x.a);
    IGE_w_rP1 (fp_o2, 2, 0, polTab[i1].x.b);
    IGE_w_rP1 (fp_o2, 2, 0, polTab[i1].x.c);
    IGE_w_rP1 (fp_o2, 2, 0, polTab[i1].x.d);
    // die Y-Koeffizienten
    IGE_w_rP1 (fp_o2, 2, 0, polTab[i1].y.a);
    IGE_w_rP1 (fp_o2, 2, 0, polTab[i1].y.b);
    IGE_w_rP1 (fp_o2, 2, 0, polTab[i1].y.c);
    IGE_w_rP1 (fp_o2, 2, 0, polTab[i1].y.d);
    // die Z-Koeffizienten
    IGE_w_rP1 (fp_o2, 2, 0, polTab[i1].z.a);
    IGE_w_rP1 (fp_o2, 2, 0, polTab[i1].z.b);
    IGE_w_rP1 (fp_o2, 2, 0, polTab[i1].z.c);
    IGE_w_rP1 (fp_o2, 2, 0, polTab[i1].z.d);
  }

  return 0;

}


/*=====================================================================*/
  int IGE_w_rPP (FILE *fp_o2, Point* pt1) {
/*============

  Einen Punkt in die P zeile raus

*/

  // printf("IGE_w_rPP %f,%f,%f\n",pt1->x,pt1->y,pt1->z);


  IGE_w_rP1 (fp_o2, 2, 0, pt1->x);
  IGE_w_rP1 (fp_o2, 2, 0, pt1->y);
  IGE_w_rP1 (fp_o2, 2, 0, pt1->z);

  return 1;

}


//===================================================================
  int IGE_w_rPT (FILE *fp_o2, char *txt) {
//===================================================================
// Text ausg.

  int   i1, ltx, ltb;
  char  *p1;


  // printf("IGE_w_rPT |%s|\n",txt);

  ltx=strlen(txt);
  ltb=strlen(IGlnD1);

  sprintf(&IGlnD1[ltb],"%dH",ltx);   // Textheader  "#H"

  p1 = txt;



  L_next:
  // printf("...IGE_w_rPT |%s|\n",p1);
  ltb=strlen(IGlnD1);

  if((ltb+ltx) < 64) {
    sprintf(&IGlnD1[ltb],"%s,",p1);   // Textheader  "#H"
    return 0;
  }

  // zu lang; Teilen
  i1 = 64 - ltb;
  strncpy(&IGlnD1[ltb], p1, i1);

  IGE_w_rP_out (fp_o2); // ausgeben


  p1 = &txt[i1];
  ltx = strlen(p1);
  goto L_next;


}


/*=====================================================================*/
  int IGE_w_rP1 (FILE *fp_o2, int mode, int pi, double pd)
/*============

  Add Parameter to P-Line. Ist die Zeile voll, rausschreiben.
  mode = 0: clear Line.
         1: add int pi.
         2: add double pd.
         3: zeile raus.
*/

{

  int    i1, i2;
  char   auxbuf[40];



  /* --------------------------------------------------------- */
  // printf("IGE_w_rP1 %d %d %f\n",mode,pi,pd);


  if(mode == 0) goto L_CLEAR;
  if(mode == 3) goto L_AUSG;



  /* ----------- add parameter into Line -------------- */
  L_START:
  i2=strlen(IGlnD1);


  /* Wenn Zeile schon zu lang, vorher raus. */
  // Zahlenwerte bis max Spalte 64 (63 in C)
  if (i2 > 60) goto L_WRITE;

  

  //------------------------------------------------------------
  if(mode == 1) {                    // add Integerzahl pi
    // sprintf(&IGlnD1[i2],"%d",pi);
    auxbuf[0] = '\0';
    sprintf(auxbuf,"%d",pi);

    if((strlen(auxbuf) + i2) > 62) goto L_WRITE;
    strcat(IGlnD1, auxbuf);




  //------------------------------------------------------------
  } else if(mode == 2) {               // add realzahl pd
/*
    sprintf(&IGlnD1[i2],"%lf",pd);
    i2=strlen(IGlnD1);
    if (IGlnD1[i2-1] == '0') {
      while (IGlnD1[i2-2] == '0') {
        --i2;
        IGlnD1[i2] = '\0';
      }
    }
*/
    auxbuf[0] = '\0';
    UTX_add_fl_f  (auxbuf, pd, 10);
    UTX_del_foll0 (auxbuf);

    if((strlen(auxbuf) + i2) > 62) goto L_WRITE;
    strcat(IGlnD1, auxbuf);




  //-----------------------------------------------------------
  }


  /* Add "," */
  i2=strlen(IGlnD1);
  strcpy(&IGlnD1[i2],",");
  IGlnD1[i2+1] = '\0';

  return 1;





  /*------- fertige Zeile ausgeben ----------------- */
  L_AUSG:

  i2=strlen(IGlnD1);
  // printf("L_AUSG: %d |%s|\n",i2,IGlnD1);


  /* Am Ende ein ";" sonst ein "," als Trennzeichen */
  strcpy(&IGlnD1[i2-1],";");

  goto L_Raus;





  /* -------- zu lang gewordene Zeile ausgeben --------------- */
  L_WRITE:
  i2=strlen(IGlnD1);

  /* Am Ende ein ";" sonst ein "," als Trennzeichen */
  //strcpy(&IGlnD1[i2-1],";");


  L_Raus:
  /* bis 64 auf Blank setzen */
  for (i1=i2; i1 < 64; i1++)   { strcpy(&IGlnD1[i1]," "); }

  /* Die Zeilen# der zugehoerigen D-Zeile v. 65 - 72 zufuegen */
  sprintf(&IGlnD1[64],"%8d",IG_D_line_nr-1);



  /* Von 73-80 die Zeilen# */
  strcpy(&IGlnD1[72],"P");
  IG_P_line_nr++;
  sprintf(&IGlnD1[73],"%7d",IG_P_line_nr);

  // printf("out2: |%s|\n",IGlnD1);
  fprintf(fp_o2,"%s\n",IGlnD1);

  if(mode != 3) {
    IGlnD1[0] = '\0';
    /* strcpy(&IGlnD1[0]," "); */
    goto L_START;
  }
    
  return 1;






  /*-------- Die Zeile loeschen --------*/
  L_CLEAR:
  /* strcpy(&IGlnD1[0]," "); */
  IGlnD1[0] = '\0';
  return 1;

}


//===================================================================
  int IGE_w_rP_out (FILE *fp_o2) {
//===================================================================
// Ausgabezeile rausschreiben.


  int i1, i2;


  // printf("IGE_w_rP_out |%s|\n",IGlnD1);


  i2 = strlen(IGlnD1);


  /* bis 64 auf Blank setzen */
  for (i1=i2; i1 < 64; i1++)   { strcpy(&IGlnD1[i1]," "); }

  /* Die Zeilen# der zugehoerigen D-Zeile v. 65 - 72 zufuegen */
  sprintf(&IGlnD1[64],"%8d",IG_D_line_nr-1);



  /* Von 73-80 die Zeilen# */
  strcpy(&IGlnD1[72],"P");
  IG_P_line_nr++;
  sprintf(&IGlnD1[73],"%7d",IG_P_line_nr);

  // printf("|%s|\n",IGlnD1);
  // printf("out2: |%s|\n",IGlnD1);
  fprintf(fp_o2,"%s\n",IGlnD1);


  /*-------- Die Zeile loeschen --------*/
  IGlnD1[0] = '\0';
  return 1;

}


/*=====================================================================*/
  int IGE_w_rT (FILE *fp_o1, FILE *fp_o2)
/*===========
  Den T-Block rausschreiben, dann beide Files zusammenmergen.
*/

{

  int i1;

  printf("IGE_w_rT.................\n");




  /* ---------------------------------------------------------- */
  // nun Datei 2 hinter Datei 1 anfuegen
  rewind (fp_o2);
  while(fgets (IGlnD1, 84, fp_o2) != NULL) {
    fprintf(fp_o1, "%s",IGlnD1);
  }





  //----------------------------------------------------------------------
   /* Schlusszeile */
  strcpy(&IGlnD1[0],"S");
  sprintf(&IGlnD1[1],"%7d",IG_S_line_nr);

  strcpy(&IGlnD1[8],"G");
  sprintf(&IGlnD1[9],"%7d",IG_G_line_nr);

  strcpy(&IGlnD1[16],"D");
  sprintf(&IGlnD1[17],"%7d",IG_D_line_nr);

  strcpy(&IGlnD1[24],"P");
  sprintf(&IGlnD1[25],"%7d",IG_P_line_nr);

   /* Von 32 bis 72 blanks */
  for (i1=32; i1<72; i1++)   { strcpy(&IGlnD1[i1]," "); }

  strcpy(&IGlnD1[72],"T");

   /* Von 74-80 die Anz. der Zeilen (1) */
  sprintf(&IGlnD1[73],"%7d",1);


  //TX_Print("/%s/",IGlnD1);
  fprintf(fp_o1,"%s\n",IGlnD1);

  return 1;

}



/*=====================================================================*/
  int IGE_w_init (FILE *fp_o1)
/*=============
  Den H-Block rausschreiben.
*/

{

  int i1;

  /* --------------------------------------------------------------------- */
  /* die ersten 68 Char's */
  fprintf(fp_o1,"%s",
   "START RECORD GO HERE                                                ");
  /*           1234567890123456789012   12345678    1234567890123456789    */
  /*1234567890123456789012345678901234567890123456789012345678901234567890 */
  /*         1         2         3         4         5         6           */

   /* die fehlenden 12 */
  fprintf(fp_o1,"%s",
   "    S      1\n");
  /*1234567890123456789012345678901234567890123456789012345678901234567890 */
  /*         1         2         3         4         5         6           */

/*
,,,8Hext1.IGS,24HCAD-CAM Services Reiter ,3H1.0,                        G 1
32,38,16,38,16,,1.,2,                                                   G 2
2HMM,450,4.5,                                                           G 3
15H20000329.171500,0.001,500.,,,11,0,                                   G 4
15H20000329.171500,;                                                    G 5
*/

  /* die ersten 72 Char's */
  /*   4=Filename, 5=SystemID, 6=InterfaceVersion               */
  fprintf(fp_o1,"%s",
   ",,,8HFILE.IGS,23HCAD-CAM Services Reiter,3H1.0,                         ");
  /*123456789012345678901234567890123456789012345678901234567890123456789012 */
  /*         1         2         3         4         5         6         7 */
  /* die fehlenden 8 */
  fprintf(fp_o1,"%s",
   "G      1\n");
  /*1234567890123456789012345678901234567890123456789012345678901234567890 */
  /*         1         2         3         4         5         6           */


  /* die ersten 72 Char's */
  /* 7=BitsOfInt,  ...      13=ModelSpaceScale 14=UnitFlag(2=mm) */
  fprintf(fp_o1,"%s",
   "32,38,16,38,16,,1.,2,                                                   ");
  /*123456789012345678901234567890123456789012345678901234567890123456789012 */
  /*         1         2         3         4         5         6         7 */
  /* die fehlenden 8 */
  fprintf(fp_o1,"%s",
   "G      2\n");
  /*1234567890123456789012345678901234567890123456789012345678901234567890 */
  /*         1         2         3         4         5         6           */



  /* die ersten 72 Char's */
  /* 15=Units, 16=Max.LineWeight, 17=Max.LineWidth */
  fprintf(fp_o1,"%s",
   "2HMM,450,4.5,                                                           ");
  /*123456789012345678901234567890123456789012345678901234567890123456789012 */
  /*         1         2         3         4         5         6         7 */
  /* die fehlenden 8 */
  fprintf(fp_o1,"%s",
   "G      3\n");
  /*1234567890123456789012345678901234567890123456789012345678901234567890 */
  /*         1         2         3         4         5         6           */


  /* die ersten 72 Char's */
  /* 18=GenerationDate, 19=Min.Resol, 20=Max.CoordVal, 23=IgesVersionNumber */
  fprintf(fp_o1,"%s",
   "15H20000329.171500,0.001,500.,,,6,0,                                    ");
  /*123456789012345678901234567890123456789012345678901234567890123456789012 */
  /*         1         2         3         4         5         6         7 */
  /* die fehlenden 8 */
  fprintf(fp_o1,"%s",
   "G      4\n");
  /*1234567890123456789012345678901234567890123456789012345678901234567890 */
  /*         1         2         3         4         5         6           */




  /* die ersten 72 Char's */
  /* 25=Date?? */
  fprintf(fp_o1,"%s",
   "15H20000329.171500,;                                                    ");
  /*123456789012345678901234567890123456789012345678901234567890123456789012 */
  /*         1         2         3         4         5         6         7 */
  /* die fehlenden 8 */
  fprintf(fp_o1,"%s",
   "G      5\n");
  /*1234567890123456789012345678901234567890123456789012345678901234567890 */
  /*         1         2         3         4         5         6           */





  //-----------------------------------------------------------------------
  IG_S_line_nr = 1;
  IG_G_line_nr = 5;

  IG_D_line_nr = 0;
  IG_P_line_nr = 0;

  IG_D_line_mod = 0; // end of last submodel


  for (i1=0; i1 < 9; ++i1) {
    IG_Pa1[i1]=0;
    IG_Pa2[i1]=0;
  }


  expTab = (void*)memspc101;
  expTabSiz = sizeof(memspc101) / sizeof(ExpObj);
  expNr  = 0;


  IG_modNr = 0;

  return 1;

}


//=======================================================================
  int IGE_w_savPtr (int typ, long ind) {
//=======================================================================

  int     i1;

  printf("IGE_w_savPtr %ld\n",expNr);


  // die zugehoerige gCad-typ- und Index speichern
  if(expNr >= expTabSiz) {
    TX_Error("IGE_w_D_Ln E001-overflow");
    return -1;
  }

  expTab[expNr].typ = typ;
  expTab[expNr].ind = ind;
  ++expNr;


  return 0;

}


//=======================================================================
  long IGE_w_getPtr (int typ, long ind) {
//=======================================================================
// errechnet den Iges-Pointer eines APT-Objekts;
// es muessen aber alle Records befuellt werden, sonst gehts nicht.
// waere da nicht besser, die IG_D_line_nr zu speichern ?

  int  btyp;
  long i1, dNr;

  // printf("IGE_w_getPtr %d %d\n",typ,ind);

  btyp = AP_typ_2_bastyp (typ);
  
  for(i1=0; i1<expNr; ++i1) {
    // printf(" ...%d %d %d\n",i1,expTab[i1].typ,expTab[i1].ind);
    if(expTab[i1].typ != btyp) continue;
    if(expTab[i1].ind != ind) continue;

    // das ist er ! 0->1, 1->3 ..
    dNr = i1 * 2 + 1;
    return dNr;
  }

  // IGE_w_dump_expTab();  // Testdisplay ausgegebene Object-ID's ..

  return -1;

}


//=======================================================================
  int IGE_w_dump_expTab () {
//=======================================================================

  int     i1;

  printf("====================== IGE_w_dump_expTab %ld\n",expNr);

  for(i1=0; i1<expNr; ++i1) {
    printf(" ...%d %d %ld\n",i1,expTab[i1].typ,expTab[i1].ind);
  }

  return 0;

}

/*
//=======================================================================
  int IGE_r_ind2dNr (int ind) {
//=======================================================================
// Die D-ZeilenNr ist somit (Index*2)-1
// 0->1, 1->3, 2->5, ..

  int DLNr;

  DLNr = (ind * 2) + 1;

  return DLNr;

}
*/


//=======================================================================
  int IGE_w_dNr2ind (int DLNr) {
//=======================================================================
// 1->0, 3->1, 5->2, ..

  int ind;

  ind = (DLNr - 1) / 2;

  return ind;

}


//============================ EOF ======================================
