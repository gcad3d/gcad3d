//             Tesselate Surfaces                 2004-06-12   RF
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
 use TSU_obj1 for TSU_ln1 TSU_ac1 TSU_bs1 ..
..................d...........
 dzt crasht Mesa nach einem realloc TSU_store !!!!!!!!
 UTO_objx_get ersetzen durch UTO_obj_getp; dazu muss TSU_obj1, TSU_obj2
   in typ void* geaendert werden !

-----------------------------------------------------
Modifications:
2014-02-09 Typ_AC -> Typ_CI
2012-11-29 SRU C C - invert direction if necessary. RF.
2004-06-19  Neu ex t_tess.c, RF.

-----------------------------------------------------
*/
/*!
\file  ../gr/tess_su.c
\brief Tesselate Surfaces 
\code
=====================================================
List_functions_start:

TSU_DrawSurT_        Hauptentry
TSU_DrawSurTRV       unbegrenzte RevSur
TSU_DrawSurBsp       unbegrenzte BSP-Sur
TSU_DrawSurRBsp      unbegrenzte RBSP-Sur
TSU_DrawSurTC        getrimmte/gelochte Flaeche
TSU_tr_init_         init transformationen ..
TSU_DrawSurTBox      work aktuelle 2D-Rasterbox
TSU_DrawSurTess      draw tesselated surf

TSU_Init
TSU_tessObj

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


TSU_DrawSurT_ ist der Hauptentry.

Die ungelochten Surs werden durch eigene Funktionen dargestellt
  TSU_DrawSurTS  - Planar
  TSU_DrawSurTRU - Ruled
  TSU_DrawSurTRV - Revolved

Gelocht/getrimmt:
  TSU_DrawSurTP  - Planar
  TSU_DrawSurTC  - Complex (Ruled/Revolved/B-Spline)



TSU_DrawSurTC  - Hauptfunktion. Ablauf:
 TSU_tr_init_  init transformationen ..
 SUP_load_c    Aussen- und Innenkonturen laden
 TSU_tr_cont_  Alle Konturen -> 2D transformieren
 UT3D_box_pts  boundingBoxen fuer alle 2D-Konturen errechnen
 UT3D_sr_polc  Umdrehungsrichtung aller 2D-Konturen testen / korrigieren
 -             gesamte Flaeche in planare, rechteckige patches zerlegen
 TSU_DrawSurTBox  einen planaren Patch ausgeben
  GLT_comm_work Aussenkontur des Patch ermitteln
  TSU_box_vec       Normalvektor auf die Rasterbox errechnen
  UT2D_ckBoxinBox1  gibts Lochkonturen die diese Aussenkontur betreffen
  GLT_diff_work fertige Konture = Aussenkontur - holes
  TSU_box_Draw2      fertige Konturen -> 3D, tesselieren, speichern.
   TSU_tr_3D_2D_pt   2D-Punkte -> 3D ruecktransformieren
   GLT_spp__      fertige 3D-Konturen tesselieren u speichern


 GLT_start__       start tesselation
 GLT_spp_pp        activate new tesselation-patch & give vector
 GLT_spp__      tesselate ..

 GL_DrawSur          display tesselated surf




===================================================================
Funktionsweise tesselate:
  Die (meisten) Tesselierungsfunktion gehen nicht nach OpenGL, sondern es
  werden die Ergebnisse (Fans, Strips ..) in gSur gespeichert.
  Die Ausgabe --> OpenGL erfolgt dann mit GL_DrawSur/GL_Disp_sur.

  Sollen die tesselated triangles zB exportiert werden, so muessen die
  Daten aus gSur kopiert werden; Func. TSU_store.
  TSU_mSpc->mSpc ist der AusgabeSpace; wird in TSU_Init(1) allociert;
  das free erfolgt durch den Aufrufer nach Bearbeitung der Daten !


Wohin wird gSur gespeichert ?
ObjGX GLT_sur      Oberstes ObjGX
ObjGX GLT_ppa[]    Patches (mind 4000 ObjGX)
ObjGX GLT_cta[]    Contours (mind 8000 ObjGX)
Point *GLT_pta     Punkte (mind 15000)


Wie wird gSur gespeichert ?
 Dazu gibst die Funktionen:
  GLT_start__    gSur=GLT_sur; GLT_sur.typ=Typ_GL_Sur, .data=GLT_ppa
  GLT_spp_pp     GLT_ppa[act]=...  .data=&GLT_cta[GLT_cta_ind];
  GLT_spp__       Tesselate; Callback GLT_spp_vert
   GLT_spp_vert  copy Vertex --> GLT_pta[GLT_pta_ind]
   GLT_spp_end   fix Nr of Patches = GLT_cta[].siz
  GL_Disp_sur
  TSU_store      save Record (GLT_sur/GLT_ppa/GLT_pta...) --> TSU_mSpc->mSpc


Wie funktioniert GLT_stor_rec:
 Es wird ebenfalls eine Record in GLT_sur/GLT_ppa/GLT_pta gespeichert
 (GLT_cta wird aber nicht benutzt). Die Funktion hat "mode" fuer das
 Init, add_data und Close&Save.


Wie funktioniert TSU_store:
 Schreibt kompl. Rec --> TSU_mSpc->mSpc.
   1) Size-Record --> TSU_mSpc->mSpc
   2) copy gSur --> TSU_mSpc->mSpc
   3) correct Size-Record


Weitere Funktionen mit tesselated Surfaces:
  TSU_exp__          export
  TSU_exp_sur        export surf
  TSU_get_vec        get Nornamlvec of tess.surf
  tess_write_f_
  tess_read_f         read tesselated surf
  tess_reloc_f_       relocate tesselated surf


See Aufbei einer tesselated Surface in GL_Disp_sur




...............................................................
TSU_dir  bei Ausgabe die Richtung der Polygone komplett umdrehen
  (damit man den Normalvektor immer nach innen machen kann)



TSU_mode   ist der Ausgabemodus;
           0=normal an die OpenGL ausgeben (darstellen)
           1=speichern nach TSU_mSpc->mSpc.                 
TSU_mSpc->mSpc   ist die StartAdresse des Blocks
TSU_mSpc->mSiz   ist die Size des Block
TSU_mSpc->mPos   ist die naechste freie Position




===============================*/




#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "../ut/ut_geo.h"                 // Point ...
#include "../ut/ut_msh.h"                 // Fac3 ..
#include "../ut/ut_memTab.h"              // MemTab_..
#include "../ut/ut_ox_base.h"             // OGX_SET_INDEX
#include "../ut/ut_os.h"                  // OS_..
#include "../ut/ut_plg.h"                 // UT3D_par_par1plg
#include "../ut/ut_col.h"                 // COL_INT32
#include "../db/ut_DB.h"                  // DB_GetGTxt
#include "../ut/gr_types.h"                  // UI_Func... SYM_..
#include "../gr/tess_su.h"                // TypTsuSur
#include "../xa/xa_mem.h"                 // memspc51..
#include "../xa/xa.h"                     // AP_STAT
#include "../xa/xa_ga.h"                  // GA_tex_ga2tr


typedef_MemTab(int);
typedef_MemTab(Point);
typedef_MemTab(Fac3);
typedef_MemTab(EdgeLine);



//=============== extern glob. vars ======================
// aus ../ci/NC_Main.h
extern double  WC_ask_ModSiz ();
extern int     APT_dispSOL;           // 0=ON=shade; 1=OFF=symbolic

// aus ../gr/ut_GL.c:
extern ColRGB  GL_actCol;
extern ColRGB  GL_defCol;
extern int     GL_actTex;

// aus xa.c:
extern AP_STAT   AP_stat;
extern int       WC_modact_ind;        // the Nr of the active submodel; -1 = main.



// aus ../gr/ut_DL.c
extern DL_Att  *GR_ObjTab;
extern long    DL_ind_act;





//=============== local glob. vars =======================
  int (*TSU_tr_2D_3D_pt)(Point*, Point*);
  int (*TSU_tr_3D_2D_pt)(Point*, Point*);

  int TSU_tr_2D_3D_con   (Point *pt2, Point *pt3);
  int TSU_tr_3D_2D_con   (Point *pt3, Point *pt2);
  int TSU_tr_2D_3D_tor   (Point *pt2, Point *pt3);
  int TSU_tr_3D_2D_tor   (Point *pt3, Point *pt2);
  int TSU_tr_2D_3D_sru   (Point *pt2, Point *pt3);
  int TSU_tr_3D_2D_sru   (Point *pt3, Point *pt2);
  int TSU_tr_2D_3D_sph   (Point *pt2, Point *pt3);
  int TSU_tr_3D_2D_sph   (Point *pt3, Point *pt2);
  int TSU_tr_2D_3D_srv   (Point *pt2, Point *pt3);
  int TSU_tr_3D_2D_srv   (Point *pt3, Point *pt2);
  int TSU_tr_2D_3D_sbsp  (Point *pt2, Point *pt3);
  int TSU_tr_3D_2D_sbsp  (Point *pt3, Point *pt2);
  int TSU_tr_2D_3D_srbsp (Point *pt2, Point *pt3);
  int TSU_tr_3D_2D_srbsp (Point *pt3, Point *pt2);


static int     TSU_typ_, TSU_form_, TSU_formTyp, TSU_dir, TSU_sStyl;
static ObjGX   TSU_ox1, TSU_ox2;
// static ObjX    TSU_obj1, TSU_obj2;
static void    *TSU_obj1, *TSU_obj2;
static Conus   *TSU_con;
static Torus   *TSU_tor;
static SurBSpl *TSU_sbsp;
static SurRBSpl *TSU_srbsp;
static Point   TSU_prx;       // rotAxis
static Point   TSU_pint[2];   // intersection-points
static int     TSU_pintNr;    // # of intersection-points
// static int     TSU_pintIa[12]; // index of intersection-points in table
static int     TSU_cycX;      // closed (cyclic) in X (U)
// static int     TSU_pint1Flag;
// static int     TSU_pint2Flag;
static int     TSU_pintAct;   // correct angle or not
static Vector  TSU_vrx;       // rotAxis
static Vector  TSU_vn;        // Normalvektor
static Line    TSU_ln1;
static Circ    TSU_ac1;
static Plane   TSU_pln;
static CurvBSpl TSU_bs1;
static char     TSU_objCont[OBJ_SIZ_MAX];
static Point    *TSU_grid;     // die GitterpunkteTabelle
static int      TSU_gridMax;
static Mat_4x3  TSU_mat, TSU_imat;
static double   TSU_dat[8];             // auxDat
static double   uOff, uMin, uMax;
static double   vOff, vMin, vMax;
static double   TSU_tol2D;              // Tol. bei 2D-Image
static long     TSU_dli;


#define TSU_CONT_SIZ 100


#define TSU_VMEM_INC  2000000          // memsize
#define TSU_vMminSiz  500000           // 500000

       int      TSU_mode = 0;          // 0=Normal(Draw);
                                       // 1=Store -> TSU_mSpc->mSpc.

typedef struct {int sNr, mSiz, mFree; void *mSpc, *mPos;}          TSU_struct1;
// mSpc         // Startpos. memspc for faces; store Vertices here
// sNr          // nr of surfs in mSpc
// mSiz         // total size
// mFree        // free space in mSpc
// mPos         // next free position in mSpc

// mFree = mSiz - (mPos - mSpc)

static TSU_struct1 TSU_spc1, TSU_spc2;
static TSU_struct1 *TSU_mSpc;



//================================================================
  int TSU_free () {
//================================================================
// free space
// ATTENTION: MS-Win in dll MUST use this func

  if(TSU_mSpc->mSpc) free (TSU_mSpc->mSpc);
  return 0;

}

 
//================================================================
  int TSU_dli_get () {
//================================================================
// get active DispListIndex TSU_dli
 
  return TSU_dli;

}


//================================================================
  int TSU_Init (int mode, void **memSpc) {
//================================================================
// Input:
//   mode:
//     0 reset to normal; memSpc gives the memPos of the accumulated Vertices;
//     1 start accumulation of tesselated vertices for Export; do not Draw.
//             YOU MUST free memSpc with mode=0|3 !
//     2 reset_store to mem
//     3 get stored-data-pointer
//             YOU MUST free memSpc with mode=0|3 !
//     4 start accumulation of tesselated vertices for Intersect; do not Draw.
//             YOU MUST free memSpc with mode=0|3 !
// Output:
//   Retcod:  mode=0    returns the nr of tesselated triangles
//            mode=1    returns the nr of surfaces to tesselate
//----------------------------------------------------------------

// Inside Export (mode=1) comes Intersect (mode=4) !
// After reset from Intersect switch back to Export !

static int oldMode = 0;

  unsigned long  mSiz;

  int   irc = 0, i1;


  // printf("IIIIIIIIIIIIIII TSU_Init %d %d\n",mode,TSU_mode);


  //----------------------------------------------------------------
  if((mode == 1)||(mode == 4)) {           // start store to mem

    oldMode = TSU_mode;

    if(mode == 1) {
      // connect struct for Export-function
      TSU_mode   = 1;
      TSU_mSpc = &TSU_spc1;

      // get nr of surfs -> irc
      irc = DL_GetNrSur() + DL_GetNrSol();
        printf(" %d surf to export ..\n",irc);

      if(irc < 1) {
        // TX_Print("*********** no surfaces to export ....");
        TSU_mode = 0;                                   // 2011-06-22
        return 0;
      }


    } else {   // mode=4
      // connect struct for Intersect-function
      TSU_mode   = 2;
      TSU_mSpc = &TSU_spc2;
      irc = 20;
    }


    // Memspace fuer TSU_mSpc->mSpc reservieren.
    // Sollte eigentlich besser ueber eine Datei gehen !

    // anhand der besetzten Groesse von GLT_pta/GLT_cta/GLT_ppa geht ned;
    // beide wachsen parallel.
    // GLT_mSiz ();


    // malloc (nr of surfs * bytes pro surf)
    i1 = 7500;   // byte pro surf

    mSiz = i1 * irc;
    mSiz += TSU_vMminSiz;
      // printf(" TSU_Init malloc %d\n",mSiz);
    TSU_mSpc->mSpc = malloc(mSiz);
    TSU_mSpc->mSiz = mSiz;

    // noch reset_store notwendig
    mode = 2;
  }



  //----------------------------------------------------------------
  if(mode == 2) {           // reset_store to mem

      // printf(" reset sNr %d\n",TSU_mSpc->sNr);
    TSU_mSpc->sNr   = 0;
    TSU_mSpc->mFree = TSU_mSpc->mSiz;      // wieviel noch frei ist ..
    TSU_mSpc->mPos  = TSU_mSpc->mSpc;

    TSU_stor_defCol ();                    // save defCol

    return irc;
  }


  //----------------------------------------------------------------
  // 0 = reset all;
  // 3 = reset buffer, keep mode;
  // gives back number of records; must be > 1 (1 record is empty !)
  if((mode == 0)||(mode == 3)) {        // reset to draw

    if(TSU_mode == 0) {  // after EOM-error
        printf(" TSU_Init-EOM-Error ?\n");
      *memSpc = NULL;
      return -1;
    }

    // if(mode == 0)  TSU_mode = 0;       // 0 = exit StorMode

    // add finishing record
    UTO_write_endRec (TSU_mSpc->mPos);
    ++(TSU_mSpc->sNr);
      // printf(" ex TSU_Init-0 %d\n",TSU_mSpc->sNr);

    
    // get stored-data-pointer, user must free it !
    *memSpc  = TSU_mSpc->mSpc;
    irc = TSU_mSpc->sNr;
      // printf(" irc TSU_Init %d\n",irc);


    // mode=3: do not reset
    if(mode == 3 ) return irc;


    // reset from Export-mode:
    if(TSU_mode == 1) {
      TSU_mode = 0;


    // reset from Intersection-mode:
    } else if(TSU_mode == 2) {
      // continue with Export ..
      TSU_mSpc = &TSU_spc1;
      TSU_mode = oldMode;


    // } else if(TSU_mode == 3) {  // continue (with Export)
      
    }
    return irc;
  }


  //----------------------------------------------------------------
  TX_Print("TSU_Init E001 %d",mode);
  return 0;

}


//==================================================================
  int TSU_tess_sTab (ObjGX **spc1, int *sTyp, long *sTab, int sNr) {
//==================================================================
// tesselate Surf # i1 --> spc1
// see also INT_intplsur TSU_tsu2tria__
// Input:
//   sTyp           DB-types (Typ_SUR|Typ_SOL)
//   sTab           DB-indices
// Output:
//   spc1           malloc'd - must be freed with: free (spc1)

  int  i1, irc;

  // printf("TSU_tess_sTab %d %d\n",*sTyp,sNr);

  // Alle Surfaces einer Tabelle tesselieren.
  irc = TSU_tessObj (spc1, TYP_FuncInit, 0);        // malloc spc1

  for(i1=0; i1<sNr; ++i1) {
    // printf(" %d - tess  A%d\n",i1,sTab[i1]);
    irc = TSU_tessObj (spc1, sTyp[i1], sTab[i1]);    // tesselate -> spc1
  }

  return TSU_tessObj (spc1, TYP_FuncExit, 0);        // close tesselation

}
 

//================================================================
  int TSU_tessObj (ObjGX **spc1, int typ, long ind) {
//================================================================
/// \code
/// tesselate (add) obj into memspc spc1
/// Input:
///   typ    TYP_FuncInit          start/init
///          Typ_SUR od Typ_SOL    tesselate add to TSU_mSpc
/// \endcode

//          TYP_FuncExit          exit; provide data
// 
// TODO: add parameter mode; 0 = geometry only; without attributes;
//   1 = geometry + color;   2 = geometry + color + textures


  int    irc, i1, recNr, iAtt;
  long   gaNr;
  ObjAtt *ga1;


  printf("TSU_tessObj %d %ld\n",typ,ind);
  // UT3D_stru_dump (Typ_PLN, pln, "");


  // draw
  if(typ == TYP_FuncInit) {
    // malloc spc1=mem; tesselate Surf -> mem.
    TSU_Init (4, (void**)spc1);


  } else if(typ == Typ_SUR) {
    iAtt = GA_find_att_SU (typ, ind);  // get color
    APT_DrawSur (iAtt, ind);


  } else if(typ == Typ_SOL) {
    iAtt = GA_find_att_SU (typ, ind);  // get color
    APT_DrawSol (iAtt, ind);


  } else if(typ == TYP_FuncExit) {
    // TEST ONLY: // Start a Redraw (fills vTab)
      // ED_Reset (); ED_work_END (0);

    // reset tesselation; get accumulated Vertices.
    irc = TSU_Init (0, (void**)spc1);
      // printf(" irc=recNr=%d\n",irc);
    if(irc < 0) goto L_fertig;

    // TEST:
    // tess_dump_f_ (*spc1, "ex TSU_tessObj");



  } else {
    TX_Error("TSU_tessObj E001");
    return -1;
  }


  irc = 0;


  L_fertig:
  return irc;

}


//================================================================
  int TSU_tess_addf (ObjGX *oxi) {
//================================================================
// add Mockup-Filestruct oxi to existing Mockup-struct TSU_mSpc->mSpc
// see TSU_store


  int      rSiz;



  // i1 = 0;

  if(oxi->typ == Typ_Address) ++oxi;

  L_next:
  if(oxi->typ  == Typ_Done)  goto L_fertig;

  // erster Record muss size of following Record sein
  if(oxi->typ  != Typ_Size) goto L_Err2;
  rSiz = (long)oxi->data;
  // printf("Record %d size=%d\n",i1,rSiz);

  ++oxi;  //(char*)oxi += sizeof(ObjGX);

  TSU_store (oxi);

  oxi = (ObjGX*)((char*)oxi + rSiz);
  // ++i1;
  goto L_next;




  // tess_addf (xx, oxi);
  L_fertig:
  return 0;


  L_Err2:
    TX_Error("TSU_tess_addf E002 typ %d",oxi->typ);
    return -1;

}
 

//===========================================================================
  int TSU_tsu2tria__ (TypTsuSur *surTab, int *surNr, int surSiz,
                      Triangle *triTab, int *triNr, int triSiz, ObjGX *oxi) {
//===========================================================================
// make Triangles from Mockup-struct (TSU-Record (GL_Sur's + GL_PP's))
// Input:
//   oxi ist ein tesselated-dataBlock. See TSU_tess_sTab tess_dump_f_
// Output:
//   surTab[surNr]
//   triTab[triNr]
//   retCod         -1 triSiz too small
//                  -2 surSiz too small
// see TSU_exp_sur TSU_exp_fac GL_Disp_sur
//
// TypTsuSur:
//   p1,p2 unused
//   aux structure of data; 4=GL_TRIANGLES|5=GL_TRIANGLE_STRIP|6=GL_TRIANGLE_FAN


  int     i1, irc, i3, rSiz, totSiz, Snr, Pnr, pAct, cNr, cAct;
  int     surTyp;
  ObjGX   *Sdat, *Pdat, *Prec;
  ObjGX   *actPP, *actCont;


  // tess_dump_f_ (oxi, "TSU_tsu2tria__");
  // printf(" surSiz=%d triSiz=%d\n",surSiz,triSiz);
  // UTO_dump_s_ (oxi, "TSU_tsu2tria__");
  // UTO_dump_s_ (&oxi[0], "TSU_tsu2tria__-0");
  // UTO_dump_s_ (&oxi[1], "TSU_tsu2tria__-1");


  i1     = 0;
  *triNr = 0;
  totSiz = 0;
  *surNr = 0;  // -1;
  surTyp = Typ_SURPLN;

  surTab[0].ip1 = 0;


  // loop durch alle 3Ecke
  // die tess. data in einzelne 3Ecke umwandeln

  // skip Adress
  if(oxi->typ == Typ_Address) ++oxi;



  //---------------------------------------------------
  // Loop tru Records
  L_next_Rec:
    // printf("L_next_Rec typ=%d form=%d siz=%d\n",oxi->typ,oxi->form,oxi->siz);

  if(oxi->typ == Typ_Done)  goto L_fertig;

  // erster Record muss size of following Record sein; kein siz fuer diesen Rec!
  if(oxi->typ != Typ_Size) goto L_Err2;
  rSiz = (long)oxi->data;
    // printf(" totSize=%d\n",rSiz);
  // (char*)oxi += sizeof(ObjGX);  // proceed to data - skip over size
  ++oxi;  // proceed to next ObjGX - skip over size
    // printf(" data: typ=%d form=%d siz=%d\n",oxi->typ,oxi->form,oxi->siz);

  if(oxi->form != Typ_ObjGX) goto L_weiter;


  //---------------------------------------------------
  // Loop ueber alle Patches
  for(pAct=0; pAct<oxi->siz; ++pAct) {

    actPP = &((ObjGX*)oxi->data)[pAct];   // current Patch
      // printf(" pAct=%d typ=%d form=%d siz=%d\n",pAct,
            // actPP->typ,actPP->form,actPP->siz);

    if(actPP->typ == Typ_Typ) {
      // markiert den Start einer neuen surface ..
      surTyp = (long)actPP->data;

    } else if(actPP->form == Typ_PT) {
      // add points in actCont to surTab and triTab
      if(TSU_tsu2tria_sur (surTab, surNr, surSiz,
                           triTab, triNr, triSiz,
                           actPP, &surTyp) < 0) return -1;
    }

    if(actPP->form != Typ_ObjGX) goto L_nxt_pat;


    //---------------------------------------------------
    // Loop ueber alle Contours im current Patch
    for(cAct=0; cAct<actPP->siz; ++cAct) {

      actCont = &((ObjGX*)actPP->data)[cAct];
        // printf(" cAct=%d typ=%d form=%d siz=%d aux=%d\n",cAct,
               // actCont->typ,actCont->form,actCont->siz,actCont->aux);

      if(actCont->form == Typ_PT) {
        // add points in actCont to surTab and triTab
        if(TSU_tsu2tria_sur (surTab, surNr, surSiz,
                             triTab, triNr, triSiz,
                             actCont, &surTyp) < 0) return -1;
      }
    }
   
    L_nxt_pat:
    continue;

  }


  L_weiter:
  // (char*)oxi += rSiz;
  oxi = (ObjGX*)((char*)oxi + rSiz);
  goto L_next_Rec;




  //----------------------------------------------------------------
  L_fertig:
    if(*surNr < 1) *surNr = 1;
    surTab[*surNr-1].typ = surTyp;
    surTab[*surNr-1].ip2 = *triNr-1;


    // TESTBLOCK
    // printf("ex TSU_tsu2tria__ triNr=%d\n",*triNr);
    // for(i1=0; i1<*triNr; ++i1) {
      // printf(">>>> tria %d:\n",i1);
      // GR_Disp_tria (&triTab[i1], 9);
    // }
    // for(ii=0; ii<surNr; ++ii)
      // printf(" sur[%d].ip1=%d ip2=%d typ=%d\n",ii,
             // surTab[ii].ip1,surTab[ii].ip2,surTab[ii].typ);
    // for(ii=0;ii<triNr;++ii) UT3D_stru_dump (Typ_Tria,&triTab[ii],"tria");
    // END TESTBLOCK


  return 0;


  //----------------------------------------------------------------
  L_Err2:
    TX_Error("TSU_tsu2tria__ E002 typ %d",oxi->typ);
    return -1;

}


//=============================================================================
  int TSU_tsu2tria_sur (TypTsuSur *surTab, int *surNr, int surSiz,
                        Triangle *triTab, int *triNr, int triSiz,
                        ObjGX *oxi, int *surTyp) {
//=============================================================================
// add points in oxi to surTab and triTab
// 
// Input:
//   oxi     ist ein tesselated Patch.
//           oxi->aux is der GL-Typ;
//                   4=GL_TRIANGLES 5=GL_TRIANGLE_STRIP 6=GL_TRIANGLE_FAN
//   triSiz  size of triTab
// Output:
//   triTab  triangles
//   triNr   nr of triangles created


  // create triangles from points
  TSU_tsu2tria_rec (triTab, triNr, triSiz, oxi, surTyp);
  if(*triNr >= triSiz) {
    TX_Print("**** TSU_tsu2tria_sur E001");
    return -1;
  }


  surTab[*surNr].typ = *surTyp;
  surTab[*surNr].ip2 = *triNr - 1;
  *surNr += 1;
  if(*surNr >= surSiz) {
    TX_Print("**** TSU_tsu2tria_sur E002");
    return -2;
  }
  surTab[*surNr].ip1 = *triNr;


  return 0;

}


//=============================================================================
  int TSU_tsu2tria_rec (Triangle *triTab, int *triNr, int triSiz, ObjGX *oxi,
                        int *surTyp) {
//=============================================================================
// save ptTab in oxi as Triangles
// Input:
//   oxi     ist ein tesselated Patch.
//           oxi->aux is der GL-Typ;
//                   4=GL_TRIANGLES 5=GL_TRIANGLE_STRIP 6=GL_TRIANGLE_FAN
// Output:
//   triTab
//   triNr

// see also TSU_exp_objFac TSU_exp_wrl2Fac


  int   i0, i1, i2, i3, ia, ie;
  Point *pTab;




  // printf("TSU_tsu2tria_rec GL-Typ=%d siz=%d triNr=%d\n",
                           // oxi->aux, oxi->siz, *triNr);
  // UTO_dump_s_ (oxi, "TSU_tsu2tria_rec");



  if(oxi->siz < 3) {
    printf(" ********** TSU_tsu2tria_rec I001 ***************\n");
    return 0;
  }


  ia   = 0;
  ie   = oxi->siz - 1;
  pTab = oxi->data;


    // TESTBLOCK
    // for(i1=0; i1<ie; ++i1) {
      // UT3D_stru_dump (Typ_PT, &pTab[i1], "P[%d]",i1);
      // GR_Disp_pt (&pTab[i1], SYM_STAR_S, 2);
    // }
    // GR_Disp_pTab (oxi->siz, pTab, SYM_STAR_S, 2);
    // END TESTBLOCK



  switch (oxi->aux) {


    //================================================================
    case GL_TRIANGLES:  // 4

      i3 = ia - 1;

      L_t_nxt:
      i1 = i3 + 1;
      i2 = i1 + 1;
      i3 = i2 + 1;


      TSU_tsu2tria_add (triTab, triNr, triSiz, &pTab[i1],&pTab[i2],&pTab[i3]);
        // printf("  %d %d %d\n",i1,i2,i3);

      if(i3 >= ie) goto L_fertig;
      goto L_t_nxt;




    //================================================================
    case GL_TRIANGLE_STRIP:  // 5
      //
      //    1--3--5--7
      //    | /| /| /|
      //    |/ |/ |/ |
      //    2--4--6--8
      //
      // sollte so zerlegt werden:
      // 1 2 3
      // 3 2 4
      // 3 4 5
      // 5 4 6
      // 5 6 7
      // 7 6 8

      i1 = ia;
      i3 = i1 + 1;       // 1 - 2


      L_s_nxt:
      // i1 bleibt
      i2 = i3;
      ++i3;          // 1 2 3       3 4 5

      TSU_tsu2tria_add (triTab, triNr, triSiz, &pTab[i1],&pTab[i2],&pTab[i3]);
        // printf("  %d %d %d\n",i1,i2,i3);

      if(i3 >= ie) goto L_fertig;

      i1 = i3;
      // i2 bleibt
      ++i3;           // 3 2 4

      TSU_tsu2tria_add (triTab, triNr, triSiz, &pTab[i1],&pTab[i2],&pTab[i3]);
        // printf("  %d %d %d\n",i1,i2,i3);

      if(i3 >= ie) goto L_fertig;
      goto L_s_nxt;



    //================================================================
    case GL_FAC_PLANAR:      // 16:   Achtung: additinal for indexed 
      *surTyp = Typ_SUR;     // nicht planar ..
    case GL_TRIANGLE_FAN:        // 6
      //    1------2           ptAnz = 4
      //    |    /  |
      //    |   /   |
      //    |  /    |
      //    | /     |
      //    0.------3
      //      \     /
      //        \  /
      //         4
      //
      // sollte so zerlegt werden:
      // 0 1 2
      // 0 2 3
      // 0 3 4

      // ACHTUNG Fan: pTab kommt manchmal ohne Endpunkt !!
      if(UT3D_comp2pt(&pTab[ia],&pTab[ie],UT_TOL_pt) == 1) ie -= 1;
      // printf(" ie=%d\n",ie);

      // die ersten 4
      i0 = ia;
      i2 = i0 + 1;

      L_f_nxt:
      i1 = i2;
      ++i2;

      TSU_tsu2tria_add (triTab, triNr, triSiz, &pTab[i0],&pTab[i1],&pTab[i2]);
        // printf("  %d %d %d\n",i0,i1,i2);

      if(i2 < ie) goto L_f_nxt;

      goto L_fertig;



    //================================================================
    default:
      TX_Error("TSU_tsu2tria_rec E001 %d",oxi->aux);
      return -1;

  }


  //================================================================
  L_fertig:

    // printf(" ex TSU_tsu2tria_rec triNr=%d\n",*triNr);

  if(*triNr >= triSiz) return -1;

  return 0;

}


//================================================================
  int TSU_tsu2tria_add (Triangle *triTab, int *triNr, int triSiz,
                        Point *p1, Point *p2, Point *p3) {
//================================================================
// add 1 triangle       als 3 Pointer --> triTab[*triNr];
// der Pointer auf den 1. Punkt zB ist        triTab[i1].pa[0]

  int    it;


  it = *triNr;

  // printf("TSU_tsu2tria_add %d\n",it);
  // GR_Disp_pt (p1, SYM_STAR_S, 2);
  // GR_Disp_pt (p2, SYM_STAR_S, 2);
  // GR_Disp_pt (p3, SYM_STAR_S, 2);


  if(it >= triSiz) {
    TX_Error("TSU_tsu2tria_add EOM");
    return -1;
  }


  triTab[it].pa[0] = p1;
  triTab[it].pa[1] = p2;
  triTab[it].pa[2] = p3;

  ++it;
  *triNr = it;


  return 0;

}


//================================================================
  int TSU_get_mode () { return TSU_mode; }
//================================================================
// 0=Normal(Draw); 1=Store -> TSU_mSpc->mSpc.


//================================================================
  int TSU_get_vec  (Vector *vcn, ObjGX *os) {
//================================================================
// get Nornamlvec of tess.surf
// see GL_Disp_sur


  int     pAct, cAct, irc = -1;
  Vector  vc1;
  ObjGX   *actPP, *actCont;


  // Loop ueber alle Patches
  for(pAct=0; pAct<os->siz; ++pAct) {

    actPP = &((ObjGX*)os->data)[pAct];

      // printf(" patch %d: typ=%d form=%d siz=%d aux=%d\n",pAct,
             // actPP->typ,actPP->form,actPP->siz,actPP->aux);


    if(actPP->form == Typ_PT) {
      // Normalvektor auf planares Polygon
      if(UT3D_vc_perppta(&vc1, actPP->siz, actPP->data) < 0) continue;
          // UT3D_stru_dump (Typ_VC, &vc1, " p[%d]-v:",cAct);
      irc = 0;
      continue;
    }

    if(actPP->form != Typ_ObjGX) continue;


    //---------------------------------------------------
    // Loop ueber alle Contours im current Patch
    for(cAct=0; cAct<actPP->siz; ++cAct) {

      actCont = &((ObjGX*)actPP->data)[cAct];
      // printf(" pAct=%d cAct=%d typ=%d form=%d siz=%d\n",pAct,cAct,
               // actCont->typ, actCont->form, actCont->siz);


      // Normalvektor ?
      if(actCont->form == Typ_VC) {
          // UT3D_stru_dump (Typ_VC, actCont->data, "_DrawSur2: c%d",cAct);
        vc1 = *((Vector*)actCont->data);  // NUR TEST f GR_Disp_vc
          // UT3D_stru_dump (Typ_VC, &vc1, " c[%d]-v:",cAct);
        irc = 0;
        continue;
      }
    }


  }

  // copy back ..
  *vcn = vc1;


  if(irc != 0) printf("TSU_get_vec kein vector !!!!!!!!!!!!! \n");
  // else UT3D_stru_dump (Typ_VC, &vc1, "TSU_get_vec: ");

  return irc;

}
 

//================================================================
  int TSU_realloc__ () {
//================================================================

  TX_Error("TSU_realloc__");

  return -1;

}


 /*
schreibt direkt --> TSU_vM; geht ned. Umweg via GLT_ppa, GLT_pta ..
Noe GLT_stor_rec
//================================================================
  int TSU_stor_rec (Point *pa1, Point *pa2, int ptNr, int mode) {
//================================================================
// 1 Streifen, durch 2 Polygone beschrieben (ruled Surf) wie GL_DrawStrip2
// Subfunctions:
// mode = 0  init, ptNr ist Anzahl Patches
// mode = 1  close record
// mode = 2  add strip
// mode = 3  add fan

// see also obj_readTess__ wrl1_r_dec_sph wrl1_r_dec_cyl wrl1_r_dec_ifs


static ObjGX *SizRec;
static ObjGX *SurRec;
static ObjGX *PatRec;
static long  oldSiz = -1;
static int   patNr;     // nr of Patches

  int        i1, irc;
  long       recSiz;
  void       *sizPos, *oldBas;
  Point      *pTab;
  ObjGX      *ox1;



  printf("-------------------------------\n");
  printf("TSU_stor_rec %d %d\n",mode,oldSiz);


  //================================================================
  // close
    // ein ReInit braucht close
    if((mode == 0)&&(oldSiz >= 0)) goto L_close;

    if(mode != 1) goto L_init;

    if(oldSiz < 0) return 0;  // kein close erforderl.


    L_close:
    printf("TSU_stor_rec close %d\n",TSU_mSpc->sNr);

    // Size-Info of Size-Record korrigieren
    recSiz = TSU_mSpc->mSiz - TSU_mSpc->mFree - oldSiz - sizeof(ObjGX);  // used size
      printf(" -_store rec %d recSiz=%d\n",TSU_mSpc->sNr,recSiz);
    SizRec->data = (void*)recSiz;
      // geht so ned ..printf(" -_stored recSiz=%d\n",(long)(SurRec->data));

    ++TSU_mSpc->sNr;   // Anzahl Records
      // UTO_dump__ (TSU_mSpc->mSpc, "ex TSU_store");

    oldSiz = -1;

    if(mode == 1) return 0;  // weiter wenn ein Init requested ...


  //================================================================
  // INIT new tess-record
  L_init:
  if(mode != 0) goto L_strip;

    printf("TSU_stor_rec init %d\n",ptNr);

    // compute size of complete record
    // OGX-siz, OGX-Sur, OGX-PP * ptNr
    recSiz = sizeof(ObjGX) * (ptNr + 2);
    // check for enough space
    oldSiz = (char*)TSU_mSpc->mPos - (char*)TSU_mSpc->mSpc;
    TSU_mSpc->mFree = TSU_mSpc->mSiz - oldSiz;
      printf(" oldSiz=%d free=%d\n",oldSiz,TSU_mSpc->mFree);
    if(TSU_mSpc->mFree <= recSiz) TSU_realloc__ ();


    // add Size-Record
    SizRec = TSU_mSpc->mPos;
    SizRec->typ  = Typ_Size;
    SizRec->form = Typ_Int4;
    SizRec->siz  = 1;
    (char*)TSU_mSpc->mPos += sizeof(ObjGX);
    TSU_mSpc->mFree -= sizeof(ObjGX);


    // add Typ_GL_Sur - Record
    SurRec = TSU_mSpc->mPos;
    SurRec->typ  = Typ_GL_Sur;
    SurRec->form = Typ_ObjGX;
    SurRec->siz  = ptNr;
    (char*)TSU_mSpc->mPos += sizeof(ObjGX);
    TSU_mSpc->mFree -= sizeof(ObjGX);
    SurRec->data = TSU_mSpc->mPos;         // StartPos Patches


    // add ptNr Typ_GL_PP - Records;
    // Anzahl und Positionen der daten werden erst beim add data gesetzt.
    PatRec = TSU_mSpc->mPos;
    ox1    = PatRec;
    for(i1=0; i1<ptNr; ++i1) {
      ox1->typ  = Typ_GL_PP;
      ox1->form = Typ_PT;
      (char*)ox1 += sizeof(ObjGX);
    }
    recSiz = sizeof(ObjGX) * ptNr;
    (char*)TSU_mSpc->mPos += recSiz;
    TSU_mSpc->mFree -= recSiz;


    patNr = 0;

    return 0;



  //================================================================
  L_strip:
  if(mode != 2) goto L_fan;
  // add data
  printf("TSU_stor_rec strip %d\n",ptNr);
  // GR_Disp_pTab (ptNr, pa1, SYM_TRI_S, 3);
  // GR_Disp_pTab (ptNr, pa2, SYM_TRI_S, 3);



  // compute size of complete record
  recSiz = sizeof(Point) * ptNr * 2;

  // check for size
  if(TSU_mSpc->mFree <= recSiz) TSU_realloc__ ();



  // set Point-data-Position
  PatRec->data = TSU_mSpc->mPos;

  // set Point-Nr
  PatRec->siz  = ptNr * 2;

  // set patch-Typ
  PatRec->aux  = GL_TRIANGLE_STRIP;

  // add Point-data-Record
  pTab = TSU_mSpc->mPos;
  for(i1=0; i1<ptNr; ++i1) {       // write pointTable
    *pTab = pa1[i1];  (char*)pTab += sizeof(Point);
    *pTab = pa2[i1];  (char*)pTab += sizeof(Point);
    // UT3D_stru_dump (Typ_PT, &pTab[i1], "P[%d]:",i1);
  }

  (char*)TSU_mSpc->mPos += recSiz;
  TSU_mSpc->mFree -= recSiz;

  // proceed to next PatRec
  (char*)PatRec += sizeof(ObjGX);

  ++patNr;

  return 0;



  //================================================================
  L_fan:
  // if(mode != 1) goto L_fan;
  // add data
  printf("TSU_stor_rec fan %d\n",ptNr);
  GR_Disp_pTab (1, pa1, SYM_TRI_S, 3);
  GR_Disp_pTab (ptNr, pa2, SYM_TRI_S, 3);


  // compute size of complete record
  recSiz = sizeof(Point) * (ptNr + 1);

  // check for size
  if(TSU_mSpc->mFree <= recSiz) TSU_realloc__ ();



  // set Point-data-Position
  PatRec->data = TSU_mSpc->mPos;

  // set Point-Nr
  PatRec->siz  = ptNr + 1;

  // set patch-Typ
  PatRec->aux  = GL_TRIANGLE_FAN;

  // add Point-data-Record
  pTab = TSU_mSpc->mPos;
  *pTab = pa1[0];     (char*)pTab += sizeof(Point);
  for(i1=0; i1<ptNr; ++i1) {       // write pointTable
    *pTab = pa2[i1];  (char*)pTab += sizeof(Point);
    // UT3D_stru_dump (Typ_PT, &pTab[i1], "P[%d]:",i1);
  }

  (char*)TSU_mSpc->mPos += recSiz;
  TSU_mSpc->mFree -= recSiz;

  // proceed to next PatRec
  (char*)PatRec += sizeof(ObjGX);

  ++patNr;

  return 0;



  //================================================================
  L_err_eom:
    TX_Error("TSU_store EOM");
    TSU_Init (0, &sizPos);
    return -1;
}
*/


//================================================================
  int TSU_stor_defCol () {
//================================================================

  ObjGX  *HdrRec;
  ColRGB defCol;


  GL_DefColGet (&defCol);

  // einen Size-Record ausgeben;
  HdrRec = TSU_mSpc->mPos;
  HdrRec->typ  = Typ_Size;
  HdrRec->form = Typ_Int4;
  HdrRec->siz  = 1;
  // (long)HdrRec->data = sizeof(ObjGX);
  HdrRec->data = (void*)sizeof(ObjGX);

  // (char*)TSU_mSpc->mPos += sizeof(ObjGX);
  TSU_mSpc->mPos = (char*)TSU_mSpc->mPos + sizeof(ObjGX);
  TSU_mSpc->mFree -= sizeof(ObjGX);


  HdrRec = TSU_mSpc->mPos;
  HdrRec->typ  = Typ_Color;
  HdrRec->form = Typ_Int4;
  HdrRec->siz  = 1;
  // (long)HdrRec->data = *((long*)&defCol);
  HdrRec->data = (void*)(*((long*)&defCol));

  // (char*)TSU_mSpc->mPos += sizeof(ObjGX);
  TSU_mSpc->mPos = (char*)TSU_mSpc->mPos + sizeof(ObjGX);
  TSU_mSpc->mFree -= sizeof(ObjGX);

  return 0;

}
 

//================================================================
  int TSU_store (ObjGX *gSur) {
//================================================================
// store vertices --> TSU_mSpc->mSpc
// Input: gSur ist ein kompl. Typ_GL_Sur-Record.


  int   irc;
  long  recSiz, oldSiz;
  void  *sizPos, *oldBas;
  ObjGX *HdrRec;


  // printf("-------------------------------\n");
  // printf("TSU_store free=%d\n",TSU_mSpc->mFree);
  // UTO_dump_s_ (gSur, "TSU_store:");
  // UTO_dump__ (gSur, "TSU_store:");


  // NUR TEST:
  // irc=-1; GL_DrawSur (&irc, 5, gSur); return 0;


  // check size of gSur
  recSiz = 0;
  // UTO_siz_obj (&recSiz, gSur);
  recSiz = OGX_siz__ (gSur, 0) + TSU_vMminSiz;
  // recSiz = OGX_siz__ (gSur, 1);          - same size !
    // printf(" recSiz/0=%ld\n",recSiz);

    // OGX_DEB_dump_obj (gSur, Typ_ObjGX, 1, 0);



  L_more_mem:
  oldSiz = (char*)TSU_mSpc->mPos - (char*)TSU_mSpc->mSpc;  // used size
  TSU_mSpc->mFree = TSU_mSpc->mSiz - oldSiz;               // free size
    // printf("TSU_store free=%d totSiz=%d\n",TSU_mSpc->mFree,TSU_mSpc->mSiz);


  //----------------------------------------------------------------
  if(TSU_mSpc->mFree < recSiz) {
      // printf(" Siz tot=%d used=%d free=%d\n",
                // TSU_mSpc->mSiz,oldSiz,TSU_mSpc->mFree);

    TSU_mSpc->mSiz  += TSU_VMEM_INC;
    while((TSU_mSpc->mSiz - oldSiz) < recSiz) TSU_mSpc->mSiz  += TSU_VMEM_INC;
      // printf("   newSiz=%d\n",TSU_mSpc->mSiz);

    printf(" ::::realloc TSU_store %d\n",TSU_mSpc->mSiz);


    // add a termination-record (aber EndPointer nicht setzen)
    UTO_write_endRec (TSU_mSpc->mPos);

    // realloc
    oldBas = TSU_mSpc->mSpc;
    oldSiz = (char*)TSU_mSpc->mPos - (char*)TSU_mSpc->mSpc;
    TSU_mSpc->mSpc   = realloc(TSU_mSpc->mSpc, TSU_mSpc->mSiz);
    if(TSU_mSpc->mSpc == NULL) goto L_err_eom;
    // TSU_mSpc->mFree = TSU_mSpc->mSiz;
    TSU_mSpc->mPos  = (char*)TSU_mSpc->mSpc + oldSiz;
    TSU_mSpc->mFree = TSU_mSpc->mSiz - oldSiz;

    // die Basisadresse korrigieren
    oldSiz = (char*)TSU_mSpc->mSpc - (char*)oldBas;
    tess_reloc_f_ (TSU_mSpc->mSpc, oldSiz);

    // TSU_mSpc->mFree = TSU_mSpc->mSiz;
    // TX_Print ("realloc TSU_store - please wait");
    // AP_errStat_set (2);   // repeate gesamten RUN !
    // return -1;
  }


  //----------------------------------------------------------------
  // create Size-Record; set size (.data) later ..
  HdrRec = TSU_mSpc->mPos;
  HdrRec->typ  = Typ_Size;
  HdrRec->form = Typ_Int4;
  HdrRec->siz  = 1;
  // update pointer..
  TSU_mSpc->mPos = (char*)TSU_mSpc->mPos + sizeof(ObjGX);
  TSU_mSpc->mFree -= sizeof(ObjGX);



  // copy gSur --> TSU_mSpc->mSpc
  oldSiz = TSU_mSpc->mFree;
  irc = UTO_copy_obj (TSU_mSpc->mPos, &TSU_mSpc->mFree, gSur);
  if(irc < 0) goto L_err_eom;
  if(irc < 0) {       // LOOPT DERZEIT NOCH !!!!
    // wieder zurueck auf die alte Posi.
    TSU_mSpc->mPos = HdrRec;
    goto L_more_mem;
  }
    // UTO_dump_s_ (TSU_mSpc->mPos, "nach UTO_copy_obj");
    // UTO_dump__ (TSU_mSpc->mPos, "nach UTO_copy_obj");


  
  // verbleibenden memSpace korrigieren
  recSiz = oldSiz - TSU_mSpc->mFree;
  // (char*)TSU_mSpc->mPos +=  recSiz;
  TSU_mSpc->mPos =  (char*)TSU_mSpc->mPos + recSiz;
    // printf(" -_store rec %d recSiz=%d\n",TSU_mSpc->sNr,recSiz);




  // Size-Info of Size-Record korrigieren
  HdrRec->data = (void*)recSiz;
    // printf(" -_stored recSiz=%d\n",(long)(HdrRec->data));


  ++(TSU_mSpc->sNr);   // Anzahl
    // printf(" -_ex TSU_store %d\n",TSU_mSpc->sNr);

    // UTO_dump__ (TSU_mSpc->mSpc, "ex TSU_store");

  return 0;

  L_err_eom:
    TX_Error("TSU_store EOM");
    TSU_Init (0, &sizPos);
    return -1;
}


//================================================================
  int TSU_DrawSurT_ (ObjGX *oxi, int att, long apt_ind) {
//================================================================
// Tesselate / Display getrimmte/gelochte Flaechen
// typ  = Typ_SUR
// form = Typ_ObjGX
// siz  = Anzahl von ObjGX in data
// data = (ObjGX[0])=Stuetzflaeche
//        (ObjGX[1])=Aussenkontur
//        (ObjGX[2-n])=Innenkonturen
// ACHTUNG: bei planaren Flaechen keine Stuetzflaeche. Errechnen !
//          ObjGX[0] ist in diesem Fall Aussenkontur ..

  int  i1;


  // printf("============================================ \n");
  // printf("TSU_DrawSurT_ ind=%ld typ=%d form=%d siz=%d\n",apt_ind,
          // oxi->typ,oxi->form,oxi->siz);
  // UTO_dump__ (oxi, "Sur %d:",apt_ind);
  // for(i1=0; i1<oxi->siz; ++i1)
    // printf("SurT_ %d typ=%d form=%d\n",i1,
           // ((ObjGX*)oxi->data)[i1].typ,
           // ((ObjGX*)oxi->data)[i1].form);



  // Typ=Typ_SURRU: RuledFlaeche; ungetrimmt, ungelocht;
  if(oxi->typ == Typ_SURRU)  
    return TSU_DrawSurTRU (oxi, att, apt_ind);



  // Typ=Typ_SURRV: RevolvedFlaeche; ungetrimmt, ungelocht;
  if(oxi->typ == Typ_SURRV)
    return TSU_DrawSurTRV (oxi, att, apt_ind);



  // wenn Typ und form=Typ_SUR: Planare Flaeche; ungetrimmt, ungelocht;
  // wird scheinbar nicht mehr genutzt ??????
  if(oxi->form == Typ_SUR)  
    return TSU_DrawSurTS (oxi, att, apt_ind);


  // Typ_SURPLN: Planare Flaeche; getrimmt, gelocht.
  i1 = 0;
  if(((ObjGX*)oxi->data)[0].typ  == Typ_Typ) {
    i1 = OGX_GET_INT (&((ObjGX*)oxi->data)[0]);
  }
    // printf(" i1=%d\n",i1);


  // if((((ObjGX*)oxi->data)[0].typ  == Typ_Typ)     &&
     // (((ObjGX*)oxi->data)[0].form == Typ_SURPLN))
  if(i1 == Typ_SURPLN)
    return TSU_DrawSurTP (oxi, att, apt_ind);


  // wenn das Objekt Nr.1 eine Flaeche ist, ... noch nix tun..
  if((((ObjGX*)oxi->data)[0].typ == Typ_SUR)    ||       // SBSP od SRU
     (((ObjGX*)oxi->data)[0].typ == Typ_SURRU)  ||
     (((ObjGX*)oxi->data)[0].typ == Typ_SURRV)  ||
     (((ObjGX*)oxi->data)[0].typ == Typ_SURBSP) ||
     (((ObjGX*)oxi->data)[0].typ == Typ_SOL))            // Conus, Torus
    return TSU_DrawSurTC (oxi, att, apt_ind);

  else 
  // Objekt Nr.1 ist Curve od CCV: Planar.
    return TSU_DrawSurTP (oxi, att, apt_ind);


  return 0;

}


//================================================================
  int TSU_DrawRCIR (ObjGX *oxi, int att, long ind) {
//================================================================
// RCIR 3 or 4 points only.

  int       ptNr, i1;
  long      dli;
  Point     pa[5], *pTab;
  ObjGX     oo, cvTab;
  ColRGB    *col;



  // printf("TSU_DrawRCIR %d %d %d\n",oxi->typ,oxi->form,oxi->siz);


  TSU_sStyl = 0;
  col = COL_INT32(&att);   // col = (ColRGB*)&att;
    // printf(" col=%d tra=%d tex=%d sym=%d\n",
           // col->color,col->vtra,col->vtex,col->vsym);

  if(TSU_mode == 0)  {      // 0=draw; 1=store
    if((APT_dispSOL == OFF) ||
       (col->vsym   == 1))      TSU_sStyl = 1;   // 0=ON=shade; 1=OFF=symbolic
  }
    // printf(" TSU_sStyl=%d\n",TSU_sStyl);


  //----------------------------------------------------------------
  // get all points of RCIR > pTab
  ptNr = 0;
  i1 = UT3D_cv_scir__ (&ptNr, pa, 5, oxi);
  if(i1 < 0) return i1;
  pTab = pa;



  // draw symbolic   2013-08-22
  if(TSU_sStyl > 0) {
    // add endpoint = startpoint
    pa[ptNr] = pa[0];
    ++ptNr;
    // create obj pointarray
    OGX_SET_OBJ (&cvTab, Typ_PT, Typ_PT, ptNr, pa);
    // create obj GL-curve
    oo.typ  = Typ_GL_CV;
    oo.form = Typ_ObjGX;
    oo.siz  = 1;
    oo.data = &cvTab;

    dli = DL_StoreObj (Typ_SUR, ind, att);
    // draw GL-curve
    GL_Draw_obj (&dli, Typ_Att_Fac1, &oo);
    return 0;

  }


  // draw shaded
  GR_CreFan (&ind, att, pTab, ptNr-1, &pTab[1], 0);


  return 0;



  //----------------------------------------------------------------
  // L_EOM:
  // TX_Error("TSU_DrawRCIR EOM (increase Modelsize (Standards/Tolerances))");
  // return 0;

  // L_not_supp:
  // TX_Error("TSU_DrawRCIR E003 %d",oxp1->typ);
  // return -1;


}


//================================================================
  int TSU_DrawSurTS (ObjGX *oxi, int att, long apt_ind) {
//================================================================
// Tesselate / Display ungetrimmte/ungelochte Planare Flaeche
// typ  = Typ_SUR
// form = Typ_SUR
// data = (SurStd*)=Aussenkontur
// see also GR_DrawSup


  int       irc, i1, ptNr, cNr, plMain;
  Point     *pTab;
  Vector    vc1;
  Plane     pl1;
  SurStd    *sus;
  ObjGX     *oxp1, ox1, *gSur;
  ContTab   cTab;



  //----------------------------------------------------------------------
  sus = oxi->data;


  // printf("TSU_DrawSurTS  typ=%d baseID=%ld cvTyp=%d cvID=%ld\n",
            // sus->typ,sus->baseID,sus->cvTyp,sus->cvID);


  // Init Speicherbereich fuer Pointtabelle
  pTab = (void*) memspc501;
  ptNr = sizeof(memspc501) / sizeof(Point);


  // SurStd-Boundary -> Polygon umwandeln
  i1 = UT3D_pta_sus (&ptNr, pTab, sus, UT_DISP_cv);
  if(i1 < 0) return i1;


  // pTab in eine cTab;
  cNr = 1;
  cTab.iNr = ptNr;
  cTab.pa  = pTab;
  cTab.dir = '-';
  

  // Z-Vektor errechnen
  // TSU_tr_set_ ();
  irc = UT3D_pl_pta (&pl1, cTab.iNr, pTab); // anzahl Elemente 1. Aussenkontur
  if(irc < 0) return irc;
  vc1 = pl1.vz;


  // Orientieren der Konturen;
  // Umdrehungsrichtung feststellen und merken.
  // in cTab.dir; '+'=counterclockwise, '-'=clockwise
  plMain = UT3D_bp_vcz (&vc1); // Berechnungsebene definieren

  // UT3D_stru_dump (Typ_VC, &vc1, "plMain=%d; vc1=",plMain);

  i1 = UT3D_sr_polc (cTab.iNr, cTab.pa, &vc1, plMain);
  if(i1 > 0) cTab.dir = '-';
  else       cTab.dir = '+';
  // printf(" cTab.dir=%c\n",cTab.dir);


  // Start tesseleation , Pointer -> active surf. holen
  L_draw:
  GLT_start__ (&gSur);

  // activate new tesselation-patch
  i1 = GLT_spp_pp (&vc1);
  if(i1 < 0) { printf("TSU_DrawSurTS E001 %d\n",i1); return i1; }


  if(TSU_mode != 0)
    GLT_spp_sTyp (Typ_SURPLN);  // store surfTyp for intersect


  i1 = GLT_spp__ (&cTab, cNr, &vc1);
  if(i1 < 0) {
    if(i1 == -10) {
      GLT_exit ();
      GLT_init ();
      goto L_draw;          // realloc
    }
    // TX_Error("GLT_spp_pp E001 %d",i1);
    printf("TSU_DrawSurTS E002 %d",i1);
    return i1;
  }


  // Ausgeben
  if(TSU_mode == 0) {
    TSU_dli = DL_StoreObj (Typ_SUR, apt_ind, att);
    GL_DrawSur (&TSU_dli, att, gSur);

  } else {
    TSU_store (gSur);
  }

  return 0;

}


//================================================================
  int TSU_DrawSurTRU (ObjGX *oxi, int att, long apt_ind) {
//================================================================
// Tesselate / Display ungetrimmte/ungelochte Ruled Flaeche
// typ  = Typ_SURRU
//        form = Typ_ObjGX;
//        data = (ObjGX[2])Randkurven
// War GR_DrawSur:1649.
// USING: memspc55 memspc54 (cannot use local memspace !)


  int       sTyp, ptNr, p1Max, p2Max, iTex;
  int       iTexBas, iTexRef;
  Point     *p1Tab, *p2Tab;
  ColRGB    *col;
  TexBas    *btex;
  TexRef    *rtex;


  // printf("TSU_DrawSurTRU ind=%ld \n",apt_ind);
  // printf("  TSU_mode=%d\n",TSU_mode);  // 0=draw; 1=store
  // UTO_dump_s_ (oxi, "TSU_DrawSurTRU");


  // init, Anzahl von Polygonpunkten fuer beide Seiten bestimmen.
  TSU_tr_init_ (oxi);


  // Randkurven in Polygone zerlegen
  // UT3D_cv_ln UT3D_cv_ci bspl_pol_bsp bspl_pol2_bsp2 ?
  ptNr = 1. / uOff;
    // printf(" ptNr=%d\n",ptNr);



  // prepare space for polygons (cannot be local memspace !!)
  p1Tab = (Point*)memspc55;
  p1Max = sizeof(memspc55) / sizeof(Point);

  p2Tab = (Point*)memspc54;
  p2Max = sizeof(memspc54) / sizeof(Point);
    // printf(" p1Max=%d p2Max=%d\n",p1Max,p2Max);

  p1Max = DMIN (p1Max, p2Max);
  // if(ptNr > p1Max) ptNr = p1Max;
    // printf(" ptNr=%d p1Max=%d\n",ptNr,p1Max);


  // get 2 curves
  sTyp = UT3D_cv_sru_ (p1Tab, p2Tab, &ptNr, p1Max);




  //================================================================
  // fix style; shaded/symbolic
  TSU_sStyl = 0;
  col = COL_INT32(&att);    // col = (ColRGB*)&att;
  if(TSU_mode == 0)  {      // 0=draw; 1=store

    if((APT_dispSOL == OFF) ||
       (col->vsym   == 1))      TSU_sStyl = 1;   // 0=ON=shade; 1=OFF=symbolic

    // draw symbolic; done.
    if(TSU_sStyl > 0)
      return GL_sSym_srus (p1Tab, p2Tab, ptNr, sTyp, att, apt_ind);
  }



  //================================================================
  if(TSU_mode == 0)  {      // 0=draw; 1=store
    TSU_dli = DL_StoreObj (Typ_SUR, apt_ind, att);

    // init GL-obj Surf; mit od ohne texture ..
    // if(((ColRGB*)&att)->vtex != 0) {
    iTex = IF_TEXURE(att);
      // printf(" iTex=%d\n",iTex);
    if(iTex != 0) {
      // extract TexRef-RecordNr from iAtt
      // iTexRef = ((stru_c3c1*)&att)->b123;
      iTexRef = GA_getTexRefInd (att);
      // get TexRef-Record
      Tex_getRef (&rtex, iTexRef);
      // get TexBas-Record
      iTexBas = rtex->ibas;
      Tex_getBas__ (&btex, iTexBas);
      // nur wenn vx=NULL: vx,vy errechnen ..
      if(UT3D_compvcNull(&rtex->vx) == 1) {
        // Vektoren vx/vy, Scales scx/scy und  Offsets dx/dy setzen.
        GRU_tex_pos2 (rtex, p1Tab, p2Tab, ptNr, sTyp);
      }
      // init texture
      GL_Tex_Ini (&TSU_dli, btex, rtex);

    } else {
      // GL_Draw_Ini (&TSU_dli, att);
      GL_Surf_Ini (&TSU_dli, (void*)&att);
    }


  }




  //================================================================
  // draw
  if(sTyp < 1) {         // 0=stripe, als GL-STRIP ausgeben
    
    // GL_DrawStrip2 (p1Tab, p2Tab, ptNr);
    GR_DrawStrip (p1Tab, p2Tab, ptNr, 0);


  } else {              // als GL-FAN ausgeben
    // if(sTyp == 1) GL_DrawFan (p1Tab, ptNr, p2Tab, 1); // p1Tab hat nur 1 Pt
    // else          GL_DrawFan (p2Tab, ptNr, p1Tab, 1);
    if(sTyp == 1) GR_DrawFan (p1Tab, ptNr, p2Tab, 1, 0); // p1Tab hat nur 1 Pt
    else          GR_DrawFan (p2Tab, ptNr, p1Tab, 1, 0);
  }



  //================================================================
  L_fertig:
  if(TSU_mode == 0) {                                // 0=draw, 1=store
    // if(((ColRGB*)&att)->vtex != 0) GL_Tex_End ();
    if(iTex != 0)  GL_Tex_End ();
    else           GL_EndList ();
  }

  return 0;

}


//================================================================
  int TSU_DrawSurTRV (ObjGX *oxi, int att, long apt_ind) {
//================================================================
// Tesselate / Display ungetrimmte/ungelochte Revolved Flaeche
// typ  = Typ_SURRV
//        form = Typ_ObjGX;
//        data = (ObjGX[2])Randkurven
// War GR_DrawSur:1649.


  int     i1, irc, ptNr, p1Max, p2Max, p3Max, iNr, iAct, iTex;
  double  a2, aTot, aInc, aSta, aEnd, aa, v0, v1;
  Point   *p1Tab, *p2Tab, *p3Tab, prx;
  Vector  vrx;
  ColRGB  *col;

  TexBas    *btex;
  TexRef    *rtex;
  int       iTexBas, iTexRef;


  // printf("TSU_DrawSurTRV %d %d\n",oxi->typ,oxi->form);
  // printf(" v0=%lf\n",((SurRev*)oxi->data)->v0);
  // printf(" v1=%lf\n",((SurRev*)oxi->data)->v1);
  // UT3D_stru_dump (Typ_SURRV, oxi->data, "SRV:");   




  // fix style; shaded/symbolic
  TSU_sStyl = 0;
  col = COL_INT32(&att);    // col = (ColRGB*)&att;
  if(TSU_mode == 0)  {      // 0 = draw OpenGL

    if((APT_dispSOL == OFF) ||
       (col->vsym   == 1))      TSU_sStyl = 1;   // 0=ON=shade; 1=OFF=symbolic

    // draw symbolic; done.
    if(TSU_sStyl > 0) return GL_sSym_srv (oxi, att, apt_ind);

  }



  // init
  irc = TSU_tr_init_ (oxi);
  if(irc < 0) return irc;

  // UT3D_stru_dump (Typ_PT, &TSU_prx, "TRV: TSU_prx");
  // UT3D_stru_dump (Typ_VC, &TSU_vrx, "TRV: TSU_vrx");
  // UTO_dump__ (&TSU_ox2, "TRV: Contour");

  

  // prepare space for polygons
  p1Tab = (Point*)memspc55;
  p1Max = sizeof(memspc55) / sizeof(Point);

  p2Tab = (Point*)memspc54;
  p2Max = sizeof(memspc54) / sizeof(Point);

  p3Tab = (Point*)memspc53;
  p3Max = sizeof(memspc53) / sizeof(Point);


  // Polygon p1Tab[ptNr] aus Randkurve TSU_ox2 generieren ("Contour")
  // p1Tab ist das StartPolygon (bei Angle = 0)
  ptNr = p1Max;
  i1 = UT3D_npt_ox (&ptNr, p1Tab, &TSU_ox2, UT_DISP_cv * 2.);
    // printf(" irc=%d ptNr=%d\n",i1,ptNr);
  if(i1 != 0) return 0; // skip Stuetzflaeche




    // TestDisp Contour
    // GR_Disp_pTab (ptNr, p1Tab, SYM_TRI_S, 2);  return -1;
    // for(i1=0;i1<ptNr;++i1)UT3D_stru_dump (Typ_PT,&p1Tab[i1]," p1Tab[%d]:",i1);


  //----------------------------------------------------------------
  // den incrementalwinkel aInc  errechnen
  // correct aSta; fix aEnd; compute aTot
  // die Anzahl Stripes iNr errechnen (GesamtWinkel / incrementalwinkel)
  if(uOff > RAD_90) uOff = RAD_90;


  // Startwinkel, Endwinkel
  aSta = ((SurRev*)oxi->data)->ang1;
  a2 = ((SurRev*)oxi->data)->ang2;
    // printf(" aSta=ang1=%lf a2=ang2=%lf\n",aSta,a2);


  // invert direction if dir=revers
  if(((SurRev*)oxi->data)->dir) {
    MEM_swap_2db (&aSta, &a2);
  }


  UT2D_2angr_set (&aSta, &a2, 1);
    // printf(" aSta=%f a2=%f\n",aSta,a2);

  aTot = a2 - aSta;
    // printf(" aTot=%f uOff=%f\n",aTot,uOff);

  iNr = fabs(aTot / uOff);
  if(iNr > 0) {
    aInc = aTot / iNr;
    // aEnd = ((SurRev*)oxi->data)->ang2 + (aInc / 2.);
      // printf(" aInc=%f aEnd=%f iNr=%d\n",aInc,aEnd,iNr);
  } else {
    aInc = aTot;
  }


    // printf(" aSta=%f aInc=%f iNr=%d\n",aSta,aInc,iNr);


  //================================================================
  // p1Tab ist in NullLage; p2Tab = erste Reihe bilden (um Startwinkel drehen)
  // if(UTP_comp2db(aa, ) {
  for(i1=0; i1<ptNr; ++i1) {
    UT3D_pt_rotptptvcangr(&p2Tab[i1], &p1Tab[i1], &TSU_prx, &TSU_vrx, aSta);
  }
  // else {
    // memcpy(p2Tab, p1Tab, sizeof(Point)*ptNr);
  // }
  

  //================================================================
  if(TSU_mode == 0)  {      // 0 = draw OpenGL
    TSU_dli = DL_StoreObj (Typ_SUR, apt_ind, att);

    iTex = IF_TEXURE(att);
    if(iTex != 0) {
      // extract TexRef-RecordNr from iAtt
      // iTexRef = ((stru_c3c1*)&att)->b123;
      iTexRef = GA_getTexRefInd (att);
      // get TexRef-Record
      Tex_getRef (&rtex, iTexRef);
      // get TexBas-Record
      iTexBas = rtex->ibas;
      Tex_getBas__ (&btex, iTexBas);
      // nur wenn vx=NULL: vx,vy errechnen ..
      if(UT3D_compvcNull(&rtex->vx) == 1) {
        // Vektoren vx/vy, Scales scx/scy und  Offsets dx/dy setzen.
        // GRU_tex_pos2 (rtex, p2Tab, p3Tab, ptNr, 0);
        GRU_tex_pos2 (rtex, p1Tab, p2Tab, ptNr, 0);   // 2011-06-27
      }
      // init texture
      GL_Tex_Ini (&TSU_dli, btex, rtex);


    } else {
      // GL_Draw_Ini (&TSU_dli, att);
      GL_Surf_Ini (&TSU_dli, att);
      // GL_Surf_Ini (&TSU_dli, NULL);
    }

  } else {
    GLT_stor_rec (0, NULL, NULL, 0);
    GLT_stor_rec (6, NULL, NULL, Typ_SURRV);  // store surfTyp for intersect
  }



  //================================================================
  // draw
  // die Polygonpunkte von TSU_obj2 rund um TSU_obj1 drehen und als
  // Stripes darstellen.
    // printf("  L1 iAct=%d aa=%f\n",iAct,aa);
  aa   = aSta;   // Startwinkel
  aa   += aInc;
  // aa   += aInc;
  iAct = 0;


  L_nxt:
    // printf("  L2 iAct=%d aa=%f\n",iAct,aa);
  for(i1=0; i1<ptNr; ++i1) {
    UT3D_pt_rotptptvcangr(&p3Tab[i1], &p1Tab[i1], &TSU_prx, &TSU_vrx, aa);
  }

  // GR_Disp_pTab (ptNr, p2Tab, SYM_TRI_S, 2);
  // als GL-STRIP ausgeben
  // GL_DrawStrip2 (p2Tab, p3Tab, ptNr, 0);
  GR_DrawStrip (p2Tab, p3Tab, ptNr, 0);

  // Adressen vertauschen
  MEM_swap__ (&p2Tab, &p3Tab, sizeof(void*));

  aa += aInc;
  ++iAct;
  if(iAct < iNr) goto L_nxt;


  L_fertig:
  if(TSU_mode == 0) GL_EndList ();

  // GR_DrawStrip macht schon GLT_stor_rec (1, !!      2008-11-09
  // else              GLT_stor_rec (1, NULL, NULL, 0);


/*
  // Aussenkontur f SRV generieren
  UT3D_cv_boxxy(&p2Tab[0],TSU_dat[0],TSU_dat[1],TSU_dat[2],TSU_dat[3]);
  ptNr = 5;
  GR_Disp_pTab (ptNr, p2Tab, SYM_TRI_S, 2);
*/


  return 0;

}


//================================================================
  int TSU_DrawSurMsh (ObjGX *oxi, int att, long apt_ind) {
//================================================================
 // TSU_DrawSurMsh       untrimmed unperforated mesh  Typ_SURMSH


  int       i1, i2, i3, ii, is, *iTab, pgNr, pNr, fNr, styl, iTex;
  int       flag1, *iba, ibNr;
  long      dli;
  char      fNam[256];
  // Mesh      ms1;
  ColRGB    *col1;

  TexBas    *btex;
  TexRef    *rtex;
  int       iTexBas, iTexRef;


  MemTab(Point) pTab = MemTab_empty;
  MemTab(Fac3) fTab = MemTab_empty;

  MemTab_ini (&pTab, sizeof(Point), Typ_PT, 10000);
  MemTab_ini (&fTab, sizeof(Fac3), Typ_Fac3, 10000);


  // printf("TSU_DrawSurMsh A%d DL_ind_act=%d\n",apt_ind,DL_ind_act);
    // UT3D_stru_dump (Typ_ObjGX, oxi, "");

  // PointGroupNumber = (int)data (Typ_Index)
  pgNr = OGX_GET_INT (oxi);
    // printf(" PointGroupNumber A%d\n",pgNr);


  // load PointFile  (write: lxml_read) pTab=malloc !
  i1 = MSH_bload_pTab (&pTab, WC_modact_nam, pgNr);
  if(i1 < 0) {TX_Error("TSU_DrawSurMsh E001"); return -1;}




  // check if bin.meshfile exists
  // sprintf(fNam, "%sM%dA%ld.msh", OS_get_tmp_dir(), WC_modact_ind, apt_ind);
  sprintf(fNam, "%s%s_A%ld.msh", OS_get_tmp_dir(), WC_modact_nam, apt_ind);
    // printf(" fNam fc |%s|\n", fNam);
  if(OS_checkFilExist(fNam, 1) == 1) goto L_f_load;

  TX_Error("TSU_DrawSurMsh I001"); return -1;

/*

  // get space for boundary
  iba = (int*)memspc101;
  ibNr = sizeof(memspc101) / sizeof(int);    // 4 byte


  // create bin.meshfile for PTAB
  MSH_msh0__ (&ms1, iba, &ibNr, pTab, pNr);
  fTab = ms1.f->fTab;
  fNr = ms1.f->fNr;
  MSH_bsav_fTab (fTab, fNr, WC_modact_ind, apt_ind);
  flag1 = 0;
  goto L_draw;
*/



  // load MeshFile    fTab=malloc !
  L_f_load:
  // i1 = MSH_bload_fTab (&fTab, &eTab, &eDat, WC_modact_ind, apt_ind);
  i1 = MSH_bload_fTab (&fTab, NULL, NULL, WC_modact_nam, apt_ind);
  if(i1 < 0) {TX_Error("TSU_DrawSurMsh E002"); return -1;}
  flag1 = 1;



  // msh1.p = p;
  // msh1.f = f;
  // msh1.p.pNr = nr_pTab;
  // msh1.f.fNr = nr_fTab;

/*
  // display verts
  // GR_Disp_pTab (msh1->p->pNr, msh1->p->pTab, SYM_STAR_S, 0);

  // loop tru facetable and display faces
  pTab = msh1->p->pTab;
  iTab = msh1->f->fTab;

  is = 0;
  for(ii=0; ii<msh1->f->fNr; ++ii) {
    i1 = iTab[is];
    i2 = iTab[is+1];
    i3 = iTab[is+2];
      // printf(" t[%d]= %d %d %d\n",ii,i1,i2,i3);
    GR_Disp_fan (&pTab[i1], &pTab[i2], &pTab[i3], 1, 6);
    is += 3;
  }
*/


/*
  // fix style; shaded/symbolic
  col = (ColRGB*)&att;

  if((APT_dispSOL == OFF) ||
     (col->vsym   == 1))      TSU_sStyl = 1;   // 0=ON=shade; 1=OFF=symbolic
  else                        TSU_sStyl = 0;

  // draw symbolic; done.
  if(TSU_sStyl > 0) return GL_sSym_Msh (sub1, att, apt_ind);
*/



  //----------------------------------------------------------------
  L_draw:

/*  new style comes with att ..
  // get style (symbolic or shaded)
  if(DL_ind_act > -1) {
    printf("DL_ind_act = %d \n",DL_ind_act);
    dli = GL_fix_DL_ind (&DL_ind_act);
    printf(" i=%d\n",dli);
    styl = ((ColRGB*)&(GR_ObjTab[dli].iatt))->vsym;
    printf(" vsym=%d\n",styl);
  }
*/

  col1 = COL_INT32(&att);   // col1 = (ColRGB*)&att;
  styl = col1->vsym;
    // printf(" styl=%d\n",styl);


  if(TSU_mode == 0) {      // 0=normal darstellen; 1=speichern
    TSU_dli = DL_StoreObj (Typ_SUR, apt_ind, att);

    // init GL-obj Surf; mit od ohne texture ..
    // if(((ColRGB*)&att)->vtex != 0) {
    iTex = IF_TEXURE(att);
      // printf(" iTex=%d\n",iTex);
    if(iTex != 0) {
      // extract TexRef-RecordNr from iAtt
      // iTexRef = ((stru_c3c1*)&att)->b123;
      iTexRef = GA_getTexRefInd (att);
      // get TexRef-Record
      Tex_getRef (&rtex, iTexRef);
      // get TexBas-Record
      iTexBas = rtex->ibas;
      Tex_getBas__ (&btex, iTexBas);
      // nur wenn vx=NULL: vx,vy errechnen ..
      if(UT3D_compvcNull(&rtex->vx) == 1) {
        // Vektoren vx/vy, Scales scx/scy und  Offsets dx/dy setzen.
        GRU_tex_pos3 (rtex, pTab.data, pTab.rNr);
      }
      // init texture
      GL_Tex_Ini (&TSU_dli, btex, rtex);

    } else {

      // GL_Draw_Ini (&TSU_dli, att);
      GL_Surf_Ini (&TSU_dli, (void*)&att);
    }


  }



  // ausgeben ..
  GR_DrawFtab (pTab.data, fTab.data, fTab.rNr, col1);


  if(TSU_mode == 0) {                                // 0=draw, 1=store
    // if(((ColRGB*)&att)->vtex != 0)
    if(iTex != 0) GL_Tex_End ();
    else          GL_EndList ();
  }




  //----------------------------------------------------------------


/*
  is = 0;
  for(ii=0; ii<nr_fTab; ++ii) {
    i1 = fTab[ii].i1;
    i2 = fTab[ii].i2;
    i3 = fTab[ii].i3;
      // printf(" t[%d]= %d %d %d\n",ii,i1,i2,i3);
    GR_Disp_fan (&pTab[i1], &pTab[i2], &pTab[i3], 1, 6);
    is += 3;
  }
*/

    // printf(" <<<<<<<<<<<<<<<<<<<TSU_DrawSurMsh free pTab %p\n",pTab);

  // MemTab_free (&eDat);
  // MemTab_free (&eTab);
  MemTab_free (&fTab);
  MemTab_free (&pTab);

  return 0;


  L_EOM:
    printf("TSU_DrawSurMsh EOM\n");
    return -1;
}


//================================================================
  int TSU_DrawSurBsp (ObjGX *oxi, int att, long apt_ind) {
//================================================================
// TSU_DrawSurBsp       unbegrenzte BSP-Sur

  int       irow1, irow2, i1, i2;
  long      ptNr, dli;
  double    d1, d2;
  Point     *pTab, *pt1, *pt2;
  SurBSpl   *sub1;
  Memspc    workSeg;
  ColRGB    *col;


  sub1 = oxi->data;


  // printf("TSU_DrawSurBsp %ld\n",apt_ind);
  // UT3D_stru_dump (Typ_SURBSP, sub1, "SBS");
    ////  GR_Disp_pTab (sub1->ptUNr*sub1->ptVNr, sub1->cpTab, SYM_STAR_S, 2);
    // Version1: das Kontrollpolygon darstellen
    // GR_DrawTriaStrip (&dli, att, sub1->ptUNr, sub1->ptVNr, sub1->cpTab);


  // fix style; shaded/symbolic
  TSU_sStyl = 0;
  col = COL_INT32(&att);    // col = (ColRGB*)&att;
  if(TSU_mode == 0)  {      // 0 = draw OpenGL

    if((APT_dispSOL == OFF) ||
       (col->vsym   == 1))      TSU_sStyl = 1;   // 0=ON=shade; 1=OFF=symbolic

    // draw symbolic; done.
    if(TSU_sStyl > 0) return GL_sSym_sbsp (sub1, att, apt_ind);
  }




  //----------------------------------------------------------------
  if(TSU_mode == 0)      // 0=normal darstellen; 1=speichern
    dli = DL_StoreObj (Typ_SUR, apt_ind, att);


  // Init Speicherbereich fuer Pointtabelle memspc501 = 500K
  pTab = (void*) memspc501;
  ptNr = sizeof(memspc501) / sizeof(Point);



  //----------------------------------------------------------------
  // test if one of the curves is a point; make fan 

  if(sub1->ptUNr == 2) {
    pt1 = &sub1->cpTab[0];
    i2 = 0;
    // compare points 0 - 2 4 6 8 ..
    for(i1=1; i1<sub1->ptVNr; ++i1) {
      i2 += sub1->ptUNr;
      if(!UT3D_comp2pt(pt1, &sub1->cpTab[i2], UT_TOL_cv)) goto L_fan_ck2;
    }
    // copy points -> pTab
    L_fan_cp_U:
      printf(" L_fan_cp_U:\n");
    // copy points 1 3 5 7 9 .. -> pTab
    i2 = 1;
    for(i1=0; i1<sub1->ptVNr; ++i1) {
      pTab[i1] = sub1->cpTab[i2];
      i2 += sub1->ptUNr;
    }
    goto L_fan_disp;
    

    L_fan_ck2:
    pt1 = &sub1->cpTab[1];
    i2 = 1;
    // compare points 1 - 3 5 7 9 ..
    for(i1=1; i1<sub1->ptVNr; ++i1) {
      i2 += sub1->ptUNr;
      if(!UT3D_comp2pt(pt1, &sub1->cpTab[i2], UT_TOL_cv)) goto L_2_cv;
    }
    // copy points 0 2 4 6 8 .. -> pTab
    L_fan_cp_V:
      // printf(" L_fan_cp_V:\n");
    i2 = 0;
    for(i1=0; i1<sub1->ptVNr; ++i1) {
      pTab[i1] = sub1->cpTab[i2];
      i2 += sub1->ptUNr;
    }

    // display pt1 + pTab as fan
    L_fan_disp:
      // printf(" DrawTriaFan %ld\n",sub1->ptVNr);
    GR_DrawTriaFan (&dli, att, pt1, sub1->ptVNr, pTab);
    return 0;
  }




  //----------------------------------------------------------------
  // 2 curves - make strip.
  // die optimale Anzahl von Punkten in U- und V-Richtung ermitteln
  L_2_cv:
  UME_init (&workSeg, memspc55, sizeof(memspc55));
  UT3D_uvNr_sbsp (&irow1, &irow2, sub1, &workSeg);

    // printf(" DrawSurBsp uNr=%d vNr=%d ptNr=%ld\n",irow1,irow2,ptNr);
    // for(i1=0; i1<(irow1*irow2); ++i1)
    // UT3D_stru_dump (Typ_PT, &pTab[i1], "pTab[%d]",i1);


  if(ptNr < (irow1 * irow2)) {
    pTab = malloc(sizeof(Point)*irow1*irow2);
    if(pTab == NULL) goto L_EOM;
    i1 = 1;
  } else i1 = 0;





  // alle Punkte ermitteln  (get irow1*irow2 points)
  UT3D_ptgrid_sbsp (pTab, &d1,&d2, sub1, irow1,irow2, &workSeg);
    // for(i1=0; i1<(irow1*irow2); ++i1)
    // UT3D_stru_dump (Typ_PT, &pTab[i1], "pTab[%d]",i1);

  // ausgeben ..
  GR_DrawTriaStrip (&dli, att, irow1, irow2, pTab, Typ_SURBSP);

  if(i1 == 1) free (pTab);


  return 0;


  //----------------------------------------------------------------
  L_EOM:
  TX_Error("TSU_DrawSurBsp EOM (increase Modelsize (Standards/Tolerances))");
  return 0;

}


//================================================================
  int TSU_DrawSurRBsp (ObjGX *oxi, int att, long apt_ind) {
//================================================================
// TSU_DrawSurRBsp      unbegrenzte RBSP-Sur

  long      ptNr, dli;
  int       i1, irow1, irow2;
  double    d1, d2;
  Point     *pTab;
  SurRBSpl  *rsub1;
  Memspc    workSeg;
  ColRGB    *col;


  rsub1 = oxi->data;

    // UT3D_stru_dump (Typ_SURRBSP, rsub1, "SRBS");
    // GR_Disp_pTab (sub1->ptUNr*sub1->ptVNr, sub1->cpTab, SYM_STAR_S, 2);
    // GR_DrawTriaStrip (&dli, att, sub1->ptUNr, sub1->ptVNr, sub1->cpTab);



  // fix style; shaded/symbolic
  TSU_sStyl = 0;
  col = COL_INT32(&att);    // col = (ColRGB*)&att;
  if(TSU_mode == 0)  {      // 0 = draw OpenGL

    if((APT_dispSOL == OFF) ||
       (col->vsym   == 1))      TSU_sStyl = 1;   // 0=ON=shade; 1=OFF=symbolic

    // draw symbolic; done.
    if(TSU_sStyl > 0) return GL_sSym_srbsp (rsub1, att, apt_ind);
  }


  //----------------------------------------------------------------
  if(TSU_mode == 0)      // 0=normal darstellen; 1=speichern
    dli = DL_StoreObj (Typ_SUR, apt_ind, att);


  // Init Speicherbereich fuer Pointtabelle memspc501 = 500K
  pTab = (void*) memspc501;
  ptNr = sizeof(memspc501) / sizeof(Point);


  // die optimale Anzahl von Punkten in U- und V-Richtung ermitteln
  // UT3D_ptaNr_srbsp (&irow1, &irow2, rsub1);
  UME_init (&workSeg, memspc55, sizeof(memspc55));
  UT3D_uvNr_srbsp (&irow1, &irow2, rsub1, &workSeg);
    // printf(" n. UT3D_uvNr_srbsp %d %d %d\n",irow1,irow2,ptNr);

  if(ptNr < (irow1 * irow2)) {
    // printf(" U=%d V=%d\n",irow1,irow2);
    pTab = malloc(sizeof(Point)*irow1*irow2);
    if(pTab == NULL) goto L_EOM;
    i1 = 1;
  } else i1 = 0;


  // alle Punkte ermitteln
  UME_init (&workSeg, memspc55, sizeof(memspc55));
  UT3D_ptgrid_srbsp (pTab, &d1,&d2, rsub1, irow1,irow2, &workSeg);

  // ausgeben ..
  GR_DrawTriaStrip (&dli, att, irow1, irow2, pTab, Typ_SURRBSP);

  if(i1 == 1) free (pTab);

  return 0;


  //----------------------------------------------------------------
  L_EOM:
  TX_Error("TSU_DrawSurRBsp EOM (increase Modelsize (Standards/Tolerances))");
  return 0;

}


//================================================================
  int TSU_DrawSurTP (ObjGX *oxi, int att, long apt_ind) {
//================================================================
// Tesselate / Display getrimmte/gelochte Planare Flaeche
// typ  = Typ_SUR
// form = Typ_ObjGX
// siz  = Anzahl von ObjGX in data
// data = (ObjGX[0])=Aussenkontur
//        (ObjGX[1-n])=Innenkonturen

// ACHTUNG: bei planaren Flaechen keine Stuetzflaeche. Errechnen !

// ACHTUNG: benutzt  memspc501, memspc201 !


  int       i1, i2, irc, cNr, cMax, ptNr;
  long      l1, l2;
  Point     *pTab;
  Vector    vc1;
  Plane     pl1;
  ContTab   *cTab;
  ObjGX     *oTab, *oxp1, *gSur;
  ColRGB    *col;


  // printf("TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT \n");
  // printf("TSU_DrawSurTP ind=%ld typ=%d form=%d\n",apt_ind,
          // oxi->typ,oxi->form);
  // printf(" TSU_mode=%d\n",TSU_mode);
  // printf(" oxi-data-posi=%p\n",oxi->data);



  // fix style; shaded/symbolic
  TSU_sStyl = 0;
  col = COL_INT32(&att);   // col = (ColRGB*)&att;
    // printf(" SurTP col=%d tra=%d tex=%d\n",col->color,col->vtra,col->vtex);
    // printf("       GL_actTex=%d\n",GL_actTex);


  if(TSU_mode == 0)  {      // 0=draw; 1=store
    if((APT_dispSOL == OFF) ||
       (col->vsym   == 1))      TSU_sStyl = 1;   // 0=ON=shade; 1=OFF=symbolic
  }


  //----------------------------------------------------------------
  pTab = (void*) memspc501;
  ptNr = sizeof(memspc501) / sizeof(Point);
  // printf(" pTab-posi=%p\n",pTab);


  // prepare space for contours
  cTab = (ContTab*)memspc201;
  cMax = sizeof(memspc201) / sizeof(ContTab);
  cNr  = 0;


  TSU_typ_ = Typ_SURPLN;

  // Aussen- und Innenkonturen holen
  irc = SUP_load_c (cTab, &cNr, cMax, pTab, ptNr, oxi, TSU_typ_);
    // printf(" _load_c %d\n",irc);
  if(irc < 0) return irc;


    // TESTBLOCK 3D-Konturen
    // for(i1=0; i1<cNr; ++i1) {
      // if(cTab[i1].pa == NULL) {printf("skip DUMMY!!\n");continue;}
      // printf("  [%d] iNr=%d\n",i1,cTab[i1].iNr);
      // GR_Disp_pTab (cTab[i1].iNr, cTab[i1].pa, SYM_TRI_S, 4);
      // for(i2=0;i2<cTab[i1].iNr;++i2) GR_Disp_txi (&cTab[i1].pa[i2],i2,0);
    // }
    // return 0;
    // END TESTBLOCK



  // Z-Vektor errechnen
  // TSU_tr_set_ ();
  irc = UT3D_pl_pta (&pl1, cTab[0].iNr, pTab); // anzahl Elemente 1. Aussenkontur
    // printf(" _pl_pta %d\n",irc);
  if(irc < 0) return irc;
  vc1 = pl1.vz;

  // Orientieren der Konturen;
  // Aussenkontur muss CCW, alle Innenkonturen muessen CW sein.
  SUP_orient (&vc1, cTab, cNr);


  //----------------------------------------------------------------
  L_draw:
    // printf(" SurTP-TSU_sStyl=%d\n",TSU_sStyl);  // 1=symbolic; 0=shaded.

  // draw symbolic; done.
  if(TSU_sStyl > 0) {
    oTab = oxi->data; // oxi->data enthaelt die Stuetzflaeche und Konturen
    return GL_sSym_spl (cTab, cNr, &oTab[0], att, apt_ind);
  }


  // Start tesselation , Pointer -> active surf. holen
  GLT_start__ (&gSur);

  // activate new tesselation-patch
  i1 = GLT_spp_pp (&vc1);

  // Default- oder Spezialfarbe ?
  if((col->color != 0) || (col->vtra != 0)) {
    GLT_spp_Col (col);
    // GLT_spp_Col (&GL_actCol);

  } else if (col->vtex != 0) {
    if(GL_actTex >= 0) GLT_spp_Tex (GL_actTex);

  // } else {   // alles 0   // alles 0
    // GLT_spp_Col (&GL_defCol);

  }


  if(TSU_mode != 0)
    GLT_spp_sTyp (Typ_SURPLN);  // store surfTyp for intersect


  // tesselate planar patch
  i1 = GLT_spp__ (cTab, cNr, &vc1);
  if(i1 < 0) goto L_err;


  // Ausgeben
  if(TSU_mode == 0) {
    TSU_dli = DL_StoreObj (Typ_SUR, apt_ind, att);
    GL_DrawSur (&TSU_dli, att, gSur);

  } else {
    TSU_store (gSur);
  }


  return 0;


  L_err:
    if(i1 == -10) {
      GLT_exit ();
      GLT_init ();
      goto L_draw;          // realloc
    }
    // TX_Error("GLT_spp_pp E001 %d",i1);
    printf("GLT_spp_pp E001 %d",i1);
    return i1;

}


//================================================================
  int TSU_DrawSurTBSP (SurBSpl *sus, int att, long apt_ind) {
//================================================================
// disp SURBSP; nur Kontrollpunkte ..
// see also GR_Disp_sbsp
// see also GR_DrawSur (koennte man dort rausnehmen...
// memspc201 memspc501 memspc53 memspc54 memspc55

  int       ptNr, irow1, irow2;
  long      dli;
  double    d1, d2;
  Point     *pTab;
  Memspc    workSeg;


  // printf("TSU_DrawSurTBSP  unr=%d vnr=%d\n",
            // sus->ptUNr,sus->ptUNr);

  pTab = (void*) memspc501;
  ptNr = sizeof(memspc501) / sizeof(Point);


  dli = DL_StoreObj (Typ_SURBSP, apt_ind, att);



  // die optimale Anzahl von Punkten in U- und V-Richtung ermitteln
  // SUSbsp_ck_tol (&irow1, &irow2, sus);
  UME_init (&workSeg, memspc201, sizeof(memspc201));
  UT3D_uvNr_sbsp (&irow1, &irow2, sus, &workSeg);
  if(ptNr < (irow1 * irow2)) goto L_EOM;

  // alle Punkte ermitteln
  // SUSbsp_aux1 (pTab, &workSeg, irow1, irow2, sub1);
  UT3D_ptgrid_sbsp (pTab, &d1,&d2, sus, irow1,irow2, &workSeg);

  // ausgeben ..
  GR_DrawTriaStrip (&dli, att, irow1, irow2, pTab, Typ_SURBSP);


  return 0;

  L_EOM:
  TX_Error("TSU_DrawSurTBSP EOM (increase Modelsize (Standards/Tolerances))");
  return 0;

}


//================================================================
  int TSU_DrawSurTC (ObjGX *oxi, int att, long apt_ind) {
//================================================================
// Tesselate / Display getrimmte/gelochte Complex-Flaeche
// typ  = Typ_SUR
// form = Typ_ObjGX
// siz  = Anzahl von ObjGX in data
// data = (ObjGX[0])=Stuetzflaeche
//        (ObjGX[1])=Aussenkontur
//        (ObjGX[2-n])=Innenkonturen
/*
MemoryUsage TSU_DrawSurTC:

memspc51   -         TSU_DrawSurTBox: commonAussenkontur
memspc52   -         Tempspace Schnitt Plane-
memspc53   -         Tempspace zum generieren TSU_grid
memspc54   TSU_grid  Hilfsgitter nur f BSP

memspc55   cTab      ContourTables
memspc501  pcTab     3D-Punkte
memspc101  pc2Tab    2D-Punkte
memspc102  TSU_grid  Vergleichspunkteraster

*/


  int       i1, i2, irc, cNr, cMax, pcNr, pc2Nr, plMain, iu, iv, rNr;
  double    d1, u1, u2, v1, v2;
  ObjGX     *oTab, *oxp1, *gSur;
  Point     *pcTab, *pc2Tab, box1[6], *pp1;
  ContTab   *cTab;
  ColRGB    *col;



  // printf("TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT \n");
  // printf("TSU_DrawSurTC att=%d ind=%ld typ=%d form=%d\n",att,apt_ind,
          // oxi->typ,oxi->form);
  // // Testausgabe Konturen
  // for(i1=0; i1<oxi->siz; ++i1)
    // printf("typ[%d]=%d\n",i1,((ObjGX*)oxi->data)[i1].typ);
    // return 0;


  // oxi->data enthaelt die Stuetzflaeche und dahinter alle Konturen.
  oTab = oxi->data;


  // erstes Obj ist die Stuetzflaeche
  // UTO_obj_get (&oxp1, &oTab[0]);
  UTO_obj_getp (&oxp1, &rNr, &oTab[0]);       // 2014-02-08 (replaces UTO_obj_get)
    // printf(" Stuetz.typ=%d form=%d siz=%d\n",oxp1->typ,oxp1->form,oxp1->siz);

  // Planar ?
  if(oxp1->typ == Typ_SUR) {
    return TSU_DrawSurTP (oxi,att,apt_ind);
  }


  if((oxp1->typ == Typ_SURBSP)  ||
     (oxp1->typ == Typ_SURRBSP) ||
     (oxp1->typ == Typ_SURRV)   ||
     (oxp1->typ == Typ_CON)     ||
     (oxp1->typ == Typ_TOR)     ||
     (oxp1->typ == Typ_SURRU))       goto L_L1;

    TX_Error(" muss BSPS od CON od TOR od SRU sein");
    return -1;


  // data[0] ist eine brauchbare Stuetzflaeche
  L_L1:


  // prepare space for contours
  cTab = (ContTab*)memspc55;
  cMax = sizeof(memspc55) / sizeof(ContTab);
  // printf(" TC:cMax=%d\n",cMax);


  // Init Speicherbereich fuer 3D-KonturenPunkteTabelle
  pcTab = (Point*) memspc501;
  pcNr = sizeof(memspc501) / sizeof(Point);


  // Init Speicherbereich fuer 2D-KonturenPunkteTabelle
  pc2Tab = (Point*) memspc101;
  pc2Nr = sizeof(memspc101) / sizeof(Point);
  // if(pcNr > i1) pcNr = i1;


  // TEST: garnix tun f TSU_test_...
  // return 0;



  //-----------------------------------------------------------
  // shaded or symbolic
  TSU_sStyl = 0;
  col = COL_INT32(&att); // col = (ColRGB*)&att;
  if(TSU_mode == 0)  {      // 0=draw; 1=store

    if((APT_dispSOL == OFF) ||
      (col->vsym   == 1))      TSU_sStyl = 1;   // 0=ON=shade; 1=OFF=symbolic
  }



  //-----------------------------------------------------------
  // init 2D -> 3D -> 2D transformationen
  // und die optimale Anzahl von Punkten in U- und V-Richtung ermitteln
  if(TSU_tr_init_ (oxp1) < 0) return -1;
    // printf(" _init_ typ=%d\n",TSU_typ_);



  //-----------------------------------------------------------
  // load 3D-outer- and inner-boundaries
  irc = SUP_load_c (cTab, &cNr, cMax, pcTab, pcNr, oxi, TSU_typ_);
    // printf(" _load_c irc=%d cNr=%d 0.iNr=%d\n",irc,cNr,cTab[0].iNr);
  if(irc < 0) return irc;


  if(cNr < 1) { TX_Error("TSU_DrawSurTC E001"); return -1; }
  if(cTab[0].iNr < 4) return 0;   // Kontur schon degeneriert (zu klein .. )

  if(irc > 0) {       // Kontur zu klein ..
    // wenn kleiner als tol_pt gar nix tun.
    if(irc > 1) return 0;
    // wenn kleiner als tol einfach planar darstellen.
    // ACHTUNG: Dummykonturen werden nicht getestet (werden erst als 2D erzeugt)
    // siehe TSU_DrawSurTS
    // GR_Disp_spu (cTab[0].iNr, cTab[0].pa, 5);
    GR_Draw_spu (&apt_ind, att, cTab[0].iNr, cTab[0].pa);
    // GR_Disp_spu1 (cTab[0].iNr, cTab[0].pa, att);
    return 0;
  }




/*
  //-----------------------------------------------------------
  // Testdisp. 3D-Input-Konturen
  // erste Pt of 1.k.
  // GR_Disp_pTab (1,cTab[0].pa,SYM_TRI_B,2); // 1.Pt
  // GR_Disp_pTab (cTab[0].iNr,cTab[0].pa,SYM_STAR_S, 2); // letzter
  printf("---- %d contours with %d points \n",cNr,cTab[0].iNr);
  for(i1=0; i1<cNr; ++i1) {
    if(cTab[i1].pa == NULL) {printf("skip DUMMY!!\n");continue;}
    printf("  [%d] iNr=%d\n",i1,cTab[i1].iNr);
    GR_Disp_pTab (cTab[i1].iNr, cTab[i1].pa, SYM_TRI_S, 4);
    for(i2=0;i2<cTab[i1].iNr;++i2) GR_Disp_txi (&cTab[i1].pa[i2],i2,0);
  }
  return 0;
  //-----------------------------------------------------------
*/



  // draw symbolic; done.
  if(TSU_sStyl > 0)
    return GL_sSym_spl (cTab, cNr, &oTab[0], att, apt_ind);






  //-----------------------------------------------------------
  // SRV,Cone:  wenn Line zu lang: korrigieren
  //           Normalvektor TSU_vn auf Konturline errechnen
  if(TSU_typ_ == Typ_SURRV) {
    if(TSU_ox2.form == Typ_LN) TSU_srv_con_0 (cTab[0].iNr,cTab[0].pa);
  }





  // Alle Konturen -> 2D transformieren
  irc = TSU_tr_cont_ (pc2Tab, pc2Nr, cTab, &cNr);
    // printf(" _cont_ %d\n",irc);
  if(irc < 0) return irc;




  //-----------------------------------------------------------
/*
  // Testdisp. 2D-Konturen
  // printf(" disp 2D-Konturen vor reduce; %d\n",cNr);
  GR_Disp_message (0);
  for(i1=0; i1<cNr; ++i1) {
    printf("  c-%d %d pts\n",i1,cTab[i1].iNr);
    GR_Disp_cv (cTab[i1].p2a, cTab[i1].iNr, 2);
    for(i2=0;i2<cTab[i1].iNr;++i2) {
      printf("P[%d,%d] %lf %lf\n",i1,i2,cTab[i1].p2a[i2].x,cTab[i1].p2a[i2].y);
      GR_Disp_txi (&cTab[i1].p2a[i2],i2,0);
      // GR_Disp_pTab (cTab[i1].iNr, cTab[i1].p2a, SYM_STAR_S, 0);
    }
  }
  // return 0;
*/
/* 
  // TEST: 2D-Punkte retour -> 3D
  for(i1=0; i1<cNr; ++i1) {
    pp1 = cTab[i1].p2a;
    for(i2=0; i2<cTab[i1].iNr; ++i2) {
      (*TSU_tr_3D_2D_pt)(box1, &pp1[i2]);
      GR_Disp_pt (box1, SYM_STAR_S, 2);
      GR_Disp_txi (box1, i2, 0);
    }
  }
  return 0;
*/
  //-----------------------------------------------------------



  // // chg Pt NUR TEST
  // // UT3D_pt_3db (&pc2Tab[17], 0.000, 2.481, 0.);
  



  // Remove 2D-points, wenn die Abweichung < minTol
  // - 2D-Punkte haben 3D-Format ! (f GLU notwendig)
  // printf(" TSU_tol2D=%f\n",TSU_tol2D);
  for(i1=0; i1<cNr; ++i1) {
    if(cTab[i1].typ != 'A')        // skip AutomaticContour
      UT2D_cv3_linear (&cTab[i1].iNr, cTab[i1].p2a, TSU_tol2D);
  }



/*
  //-----------------------------------------------------------
  // Testdisp. 2D-Konturen nach Remove 2D-points
  printf(" disp 2D-Konturen nach reduce; %d\n",cNr);
  for(i1=0; i1<cNr; ++i1) {
    for(i2=0;i2<cTab[i1].iNr;++i2)
      printf(" p[%d,%d] %lf %lf\n",i1,i2,cTab[i1].p2a[i2].x,cTab[i1].p2a[i2].y);
  }
  for(i1=0; i1<cNr; ++i1) {
    printf("  Cont[%d] ptNr=%d\n",i1,cTab[i1].iNr);
    // GR_Disp_pTab (cTab[i1].iNr, cTab[i1].p2a, SYM_STAR_S, 2);
    for(i2=0;i2<cTab[i1].iNr;++i2) GR_Disp_txi (&cTab[i1].p2a[i2],i2,0);
    GR_Disp_cv (cTab[i1].p2a, cTab[i1].iNr, 2);
  }
  // return 0;

//
  // TEST: 2D-Punkte retour -> 3D
  GR_Disp_message (0);
  for(i1=0; i1<cNr; ++i1) {
    pp1 = cTab[i1].p2a;
    for(i2=0; i2<cTab[i1].iNr; ++i2) {
      (*TSU_tr_3D_2D_pt)(box1, &pp1[i2]);
      GR_Disp_pt (box1, SYM_STAR_S, 2);
      GR_Disp_txi (box1, i2, 0);
    }
  }
  return 0;
  //-----------------------------------------------------------
*/


  // cones & cylinders: can have 2 contours instead of a outer-boundary
  if((TSU_typ_ == Typ_SURRV)    &&
     (TSU_form_ == Typ_LN))         {
    irc = TSU_srv_con_2cylCon (cTab, &cNr);
      // printf(" ex _con_2cylCon %d\n",irc);
    if(irc < 0) return irc;
  }



  //-----------------------------------------------------------
  // je eine 2D-boundingBox fuer alle 2D-Konturen errechnen
  // UT2D_rect_pts (&p21, &p22, cTab[0].iNr, cTab[0].p2a);

  d1 = TSU_tol2D * 2.;
  for(i1=0; i1<cNr; ++i1) {

    UT3D_box_pts (&cTab[i1].p1, &cTab[i1].p2, cTab[i1].iNr, cTab[i1].p2a);
    cTab[i1].p1.x -= d1;
    cTab[i1].p1.y -= d1;
    cTab[i1].p2.x += d1;
    cTab[i1].p2.y += d1;


    // Testdisp boundingBoxen
    // printf(" boundingbox Kontur %d:\n",i1);
    // UT3D_stru_dump (Typ_PT2, &cTab[i1].p1, "P1[%d]=",i1);
    // UT3D_stru_dump (Typ_PT2, &cTab[i1].p2, "P2[%d]=",i1);
    // GR_Disp_box (&cTab[i1].p1, &cTab[i1].p2, 9);
    // GR_Disp_pt2 (&cTab[i1].p1, SYM_STAR_S, 4);
    // GR_Disp_pt2 (&cTab[i1].p2, SYM_STAR_S, 4);
  }


  // Automatisch generierte Aussenkontur: check MinSiz
  if(cTab[0].typ == 'A') {
    // Boxpunkte -> 3D
    (*TSU_tr_3D_2D_pt)(&box1[0], &cTab[0].p1);
    (*TSU_tr_3D_2D_pt)(&box1[1], &cTab[0].p2);
    // gesamte Box zu klein: nix tun.
    if(UT3D_len_2pt(&box1[0],&box1[1]) < UT_TOL_cv) {
      printf(" AutoKontur zu klein\n");
      return 0;
    }
  }





  //-----------------------------------------------------------
  // Umdrehungsrichtung aller 2D-Konturen feststellen und merken.
  // in cTab.dir; '+'=counterclockwise, '-'=clockwise
  // unused: TSU_orient
  // plMain = UT3D_bp_vcz (vcz); // Berechnungsebene definieren
  plMain = BCKPLN_XY; // Z

/*
  for(i1=0; i1<cNr; ++i1) {
    i2 = UT3D_sr_polc(cTab[i1].iNr,cTab[i1].p2a,(Vector*)&UT3D_VECTOR_Z,plMain);
    if(i2 > 0) cTab[i1].dir = '+';
    else       cTab[i1].dir = '-';
      printf(" c[%d].dir=%c\n",i1,cTab[i1].dir);
  }
  // Aussenkontur muss CCW, alle Innenkonturen muessen CW sein.
  if(cTab[0].dir == '-') {
    // printf(" AussenKontur umdrehen !!!!!!!!!!!!!!!!!!\n");
    UT3D_cv_inv (cTab[0].iNr, cTab[0].p2a);
  } else {
    cTab[0].dir = '-';
  }
*/


// 2014-01-07
  for(i1=0; i1<cNr; ++i1) {
    i2 = UT3D_sr_npt_bp (cTab[i1].iNr, cTab[i1].p2a, plMain);
    if(i2 > 0) cTab[i1].dir = '-';   // CW (-)
    else       cTab[i1].dir = '+';   // CCW (+)
      // printf(" c[%d].dir=%c\n",i1,cTab[i1].dir);
  }

  // outer-boundary must be CCW (+)
  // BUT real Z-vector goes down; not up ?
  if(cTab[0].dir == '-') {
      // printf(" rev outer-bound \n");
    UT3D_cv_inv (cTab[0].iNr, cTab[0].p2a);
    cTab[0].dir = '+';
  }


// all inner-boundaries must be CW (-); but will be used LIFO..
  // invert hole-contours, if CCW  // 2014-01-07
  if(cNr > 1) {
    for(i1=1; i1<cNr; ++i1) {
      if(cTab[i1].dir == '+') {
          // printf(" rev inner-bound %d\n",i1);
        UT3D_cv_inv (cTab[i1].iNr, cTab[i1].p2a);
        cTab[i1].dir = '-';
      }
    }
  }
//



  //-----------------------------------------------------------
  // test GLU-version, init memspc, init GLU-tesselator
  if(GLT_init() < 0) return -1;



  // Start tesseleation , Pointer -> active surf. holen
  L_draw:
  GLT_start__ (&gSur);


  // if(TSU_mode != 0)
    // GLT_spp_sTyp (Typ_SURPLN);  // store surfTyp for intersect



  // Default- oder Spezialfarbe ?
  // if((col->color != 0) || (col->vtra != 0))
    // GLT_spp_Col (col);
    // GL_ColSet (col);


  // loop tru grid ..
  // X: von uMin bis uMax inc uOff
  // Y: von vMin bis vMax inc vOff


  // Aussengrenzen des 2D-Feldes ermitteln.
  // boundingBox der Aussenkontur ergibt Min/Max-Werte der Flaeche.
  // uMin = cTab[0].p1.x - (uOff / 6.);   // 2. war 4.
  // vMin = cTab[0].p1.y - (vOff / 6.);   // 2. war 4.
  uMin = cTab[0].p1.x;   // 2. war 4.
  vMin = cTab[0].p1.y;   // 2. war 4.

  // uMax = cTab[0].p2.x + (uOff / 4.);
  uMax = cTab[0].p2.x;
  vMax = cTab[0].p2.y;

    // printf("  uMin=%f uMax=%f uOff=%f\n",uMin,uMax,uOff);
    // printf("  vMin=%f vMax=%f vOff=%f\n",vMin,vMax,vOff);
    // printf(" TSU_typ_=%d TSU_form_=%d TSU_formTyp=%d\n",
            // TSU_typ_,TSU_form_,TSU_formTyp);
        



  if(TSU_typ_ == Typ_SURRV) {

    // bei Torus sollten die Werte nie ueber die Pole hinausgehen !!
    // V sollte genau auf vMax treffen.
    i1 = (vMax - vMin) / vOff;  // printf(" i1=%d\n",i1);
    if(i1 < 1) i1 = 1;
    // vOff = (vMax - vMin) / (i1 + 1);
    vOff = (vMax - vMin) / i1;
    vMax += vOff * 0.5;   // kein Draw; nur fuer vergleich beenden.


    if((TSU_ox2.form == Typ_CI)||(TSU_ox2.form == Typ_CI))
      TSU_srv_tor_2 (vMin, 0); // fix NormalvektorProblem

    // fix vMin, vMax, uOff for SRV
    else if(TSU_ox2.form == Typ_LN) {
      vMin -= vOff * 0.25;
      TSU_srv_con_1 (cTab);
    }



// TEST:
// vMin = -0.8;
// vMax = -0.5;
// vOff = 0.3;
    // printf("  surRV: uMin=%f uMax=%f uOff=%f\n",uMin,uMax,uOff);
    // printf("  surRV: vMin=%f vMax=%f vOff=%f\n",vMin,vMax,vOff);


  } else if((TSU_typ_ == Typ_SURBSP) ||
            (TSU_typ_ == Typ_SURRBSP))  {
    TSU_vMinMax (cTab);


  } else if(TSU_typ_ == Typ_TOR) {
    vMax += vOff / 1.;   // kein Draw; nur fuer vergleich beenden.
  }


  // TEST:
  // uMin = -0.2;
  // uMin -= uOff/3.;
  // uMax += uOff;
  // uOff = 2.;
  // vMin -= vOff/4.;
  // vMax += vOff;
  // vMin = -0.05;
  // vMax = 1.58 + vOff;
  // vOff = 0.25;
  // TSU_dir = -1;

  if(uMax >= 99999999.) {
    goto L_EOM;
  }

    // printf("  uMin=%f uMax=%f uOff=%f\n",uMin,uMax,uOff);
    // printf("  vMin=%f vMax=%f vOff=%f\n",vMin,vMax,vOff);
    // printf("  TSU_dir=%d\n",TSU_dir);
    // Testdisplay MaxBounds
    // GR_Disp_4db (uMin,uMax, vMin,vMax, 9);




  u1 = uMin;   u2 = uMin + uOff;
  v1 = vMin;   v2 = vMin + vOff;
  iu = 0;      iv = 0;


  box1[0].z = 0;
  box1[1].z = 0;
  box1[2].z = 0;
  box1[3].z = 0;




  L_nextBox:
  box1[0].x = u1; box1[0].y = v1;
  box1[1].x = u2; box1[1].y = v1;
  box1[2].x = u2; box1[2].y = v2;
  box1[3].x = u1; box1[3].y = v2;
  box1[4] = box1[0];

  // printf(" nextBox %d %d u1=%f v1=%f u2=%f v2=%f\n",iu,iv,u1,v1,u2,v2);



  // Korrektur ?
  // if(TSU_pintNr > 0) {
    // if((TSU_pint1Flag > 0)&&(iv == 0)) TSU_pintAct = 1;
    // else if((TSU_pint2Flag > 0)&&(iv == iv)) TSU_pintAct = 1;
    // else TSU_pintAct = -1;
  // }

  // if((iu == 2)&&(iv > 7)&&(iv < 9))    // TEST
  // if((iu == 1)&&(iv == 1))              // TEST
  // if((iu == 0)&&(iv == 2))                  // TEST
  // if(iu == 5)                              // TEST
  i1 = TSU_DrawSurTBox (box1, cTab, cNr);
  if(i1 < 0) {
    if(i1 == -10) goto L_draw;  // nach GLT_alloc_pta reDraw
    // if(i1 == -10) goto L_start;  // nach GLT_alloc_pta reDraw
    goto L_exit;
  }
  // naeches Feld (naechsten U-wert)
  if(u2 < uMax) { u1 = u2; u2 += uOff; ++iu; goto L_nextBox; }


  // naechste V-Reihe; V
  u1 = uMin;
  u2 = uMin + uOff;
  iu = 0;
  v1 = v2;
  v2 += vOff;
  ++iv;
  if(TSU_typ_ == Typ_SURRV) TSU_srv_tor_2 (v1,iv); // fix NormalvektorProblem

  if(v2 < vMax) goto L_nextBox;



  //-----------------------------------------------------------
  L_fertig:

  // Ausgeben
  if(TSU_mode == 0) {
    TSU_dli = DL_StoreObj (Typ_SUR, apt_ind, att);
    GL_DrawSur (&TSU_dli, att, gSur);

  } else {
    TSU_store (gSur);
  }



  L_exit:
  return 0;



  L_EOM:
  TX_Error("TSU_DrawSurTC EOM");
  return 0;


}


//================================================================
  int TSU_srv_con_2cylCon (ContTab *cTab, int *cNr) {
//================================================================
// for cylindricSurfaces: test if contours are 2 circles or a closed surface
// If first 2 contours define the surface: join contours.
// Retcod: 0 = not contour from 2 circles;
//         1 = contour from 2 circles; ready to go.
  
  int      i1, i2, ic, ie, ii, ib1, ib2, iDir1, iDir2;
  double   d1 = 0.;
  Point    *pTab;
  Point    *tTab;
  int      *iTab;   // temp. nr of points
  
  
  // printf("TSU_srv_con_2cylCon cNr=%d\n",*cNr);

/*
  // TESTDISPLAY -------------
  for(i1=0; i1<*cNr; ++i1) {
    printf("  %d %d\n",i1,cTab[i1].iNr);
    for(i2=0;i2<cTab[i1].iNr;++i2)
      printf(" P[%d,%d] %lf , %lf\n",i1,i2,cTab[i1].p2a[i2].x,cTab[i1].p2a[i2].y);
  }
  // TESTDISPLAY end ---------
// exit(0);
*/

  //----------------------------------------------------------------
  // test if contour is a closed surface or a closed contour.
  // set cTab[ic].temp 'C'=contour, 'S'=surface
  // d1 = ((cTab[0].p2a[0].x + cTab[0].p2a[c1pNr - 1].x) / 2.);
  for(ic=0; ic<*cNr; ++ic) {
    ie = cTab[ic].iNr;

    // contour must have a x-range of 2 pi.
    d1 = fabs(cTab[ic].p2a[ie-1].x - cTab[ic].p2a[0].x);
      // printf(" C%d range = %lf\n",ic,d1);
    if(d1 < 6.2) {
      cTab[ic].temp = 'S';   // its a closed surface;
      continue;
    }

    if(ie == 2) {
      cTab[ic].temp = 'C';   // its a closed contour;
      continue;
    }

    // test intersections with a vertival line in the middle of the contour
    // compute mid-value
    d1 = cTab[ic].p2a[ie / 2].x;
      // printf(" mid-val = %lf\n",d1);
    i2 = 0;
    ii = -1;
      // printf(" p[0].x = %f\n",cTab[0].p2a[0].x);
    for(i1=1; i1<ie; ++i1) {
      if(UTN_LIMCK__(d1,cTab[ic].p2a[i1 - 1].x,cTab[ic].p2a[i1].x)) continue;
      // must test each point extra, since UTN_LIMCK__ provides this twice,
      // if the point fits exact.
      if(cTab[ic].p2a[i1].x == d1) continue;
      // test for 2 consecutive points with a different y
      if(i1 == ii+1) continue;
        // printf(" %d-%d %lf - %lf\n",i1-1,i1,
               // cTab[ic].p2a[i1 - 1].x,cTab[ic].p2a[i1].x);
      ++i2;
      ii = i1;
    }
      // printf(" intersections - C%d = %d\n",ic,i2);
    if(i2 >= 2) cTab[ic].temp = 'S';   // its a closed surface;
    else        cTab[ic].temp = 'C';
  }

  ib1 = -1;
  ib2 = -1;
  for(ic=0; ic<*cNr; ++ic) {
      // printf(" C[%d] typ = %c\n",ic,cTab[ic].temp);
    if(cTab[ic].temp != 'C') continue;
    if(ib1 < 0) ib1 = ic;
    else        ib2 = ic;
  }

    // printf(" ib1=%d ib2=%d\n",ib1,ib2);
  if(ib1 < 0) return 0; // no circles; normal type (1. cont = surface)
  if(ib2 < 0) {
    // need 2 conturs
    TX_Print("TSU_srv_con_2cylCon E001"); // was Error; 2014-09-08
    return -1;
  }


  //----------------------------------------------------------------
  // cTab[0] is not a closed surface. Join contours ib1-ib2 with lines.

  // 1. and 2. contour may not have same direction !
  i1 = cTab[ib1].iNr - 1;
  iDir1 = (cTab[ib1].p2a[0].x > cTab[ib1].p2a[i1].x) ? 1 : -1;
  i1 = cTab[ib2].iNr - 1;
  iDir2 = (cTab[ib2].p2a[0].x > cTab[ib2].p2a[i1].x) ? 1 : -1;
    // printf(" iDir1=%d iDir2=%d\n",iDir1,iDir2);
  if(iDir1 == iDir2)
    UT3D_cv_inv (cTab[ib2].iNr, cTab[ib2].p2a);


  // make same startpoint for both contours
  // set startpoint of ib2 to endpoint of ib1
  // can change nr of points !
  ie = cTab[ib1].iNr - 1;
  TSU_srv_con_cycX2_ (&cTab[ib2], cTab[ib1].p2a[ie].x);


  // TEST
  // for(i2=0;i2<cTab[ib2].iNr;++i2)
  // printf(" P[%d,%d] %lf , %lf\n",ib2,i2,cTab[ib2].p2a[i2].x,cTab[ib2].p2a[i2].y);


// exit(0);


  //----------------------------------------------------------------
  // join contours (copy into new tempspace)

  // get tempSpace
  ii = 0;
  for(ic=0; ic<*cNr; ++ic) {
    ii += (cTab[ic].iNr + 2);
  }
    // printf(" nr pts=%d\n",ii);
  tTab = (Point*)MEM_alloc_tmp (sizeof(Point) * (ii));
  if(!tTab) {TX_Error("TSU_srv_con_2cylCon EOM1"); return -1;}
  iTab = (int*)MEM_alloc_tmp (sizeof(int) * *cNr);
  if(!iTab) {TX_Error("TSU_srv_con_2cylCon EOM2"); return -1;}


  // join 1. and 2. contour

  // copy ib1
  i1 = 0;
  ii = cTab[ib1].iNr;
  memcpy (&tTab[i1], cTab[ib1].p2a, (sizeof(Point) * ii));
  i1 += ii;

  // add ib2
  ii = cTab[ib2].iNr;
  memcpy (&tTab[i1], cTab[ib2].p2a, (sizeof(Point) * ii));
  i1 += ii;

  // add first point again
  tTab[i1] = cTab[ib1].p2a[0];
  ++i1;

  i2 = 0;
  iTab[i2] = i1;
  ++i2;


    // printf("  C0 iNr = %d\n",cTab[0].iNr);
    // for(i2=0;i2<cTab[i1].iNr;++i2)
      // printf(" P[%d,%d] %lf , %lf\n",i1,i2,cTab[i1].p2a[i2].x,cTab[i1].p2a[i2].y);


  // copy 'S' contours
  for(ic=0; ic<*cNr; ++ic) {
    if(cTab[ic].temp == 'C') continue;

    // add ib2
    ii = cTab[ic].iNr;
    memcpy (&tTab[i1], cTab[ic].p2a, (sizeof(Point) * ii));
    i1 += ii;

    iTab[i2] = ii;
      // printf(" iTab[%d] = cTab[%d] iNr = %d\n",i2,ic,ii);
    ++i2;
  }


  pTab = cTab[0].p2a;
  memcpy (pTab, tTab, (sizeof(Point) * i1));


  ii = 0;
  for(ic=0; ic<i2; ++ic) {
    cTab[ic].iNr = iTab[ic];
    cTab[ic].p2a = &pTab[ii];
    ii += iTab[ic];
  }

  *cNr = i2;


/*
  // TESTDISPLAY -------------
  // cTab[0].p2a[2].y = -0.0725;
  printf("ex TSU_srv_con_2cylCon %d\n",*cNr);
  for(i1=0; i1<*cNr; ++i1) {
    printf("  C%d iNr = %d\n",i1,cTab[i1].iNr);
    for(i2=0;i2<cTab[i1].iNr;++i2)
      printf(" P[%d,%d] %lf , %lf\n",i1,i2,cTab[i1].p2a[i2].x,cTab[i1].p2a[i2].y);
  } 
  // TESTDISPLAY end ---------
*/

// exit(0);

  return 0;

}

/*
//================================================================
  int TSU_srv_con_2cylCon (ContTab *cTab, int *cNr) {
//================================================================
// for cylindricSurfaces: test if contours are 2 circles or a closed surface
// If first 2 contours define the surface: join contours.
// Retcod: 0 = not contour from 2 circles;
//         1 = contour from 2 circles; ready to go.

  int      i1, i2, ii, c1pNr, c2pNr, iDir1, iDir2;
  double   d1 = 0.;



  printf("TSU_srv_con_2cylCon cNr=%d\n",*cNr);


  // 1 contour only: must be a closed surface.
  if(*cNr < 2) return 0;

  c1pNr = cTab[0].iNr;
  c2pNr = cTab[1].iNr;



  // TESTDISPLAY:
    printf(" c1pNr=%d c2pNr=%d\n",c1pNr, c2pNr);
  for(i1=0; i1<c1pNr; ++i1) {
    // UT3D_stru_dump (Typ_PT2, &cTab[0].p2a[i1], "C1P[%d]=",i1);
    printf(" p[0,%d] %lf %lf\n",i1,cTab[0].p2a[i1].x,cTab[0].p2a[i1].y);
  }
  for(i1=0; i1<c2pNr; ++i1) {
    // UT3D_stru_dump (Typ_PT2, &cTab[1].p2a[i1], "C2P[%d]=",i1);
    printf(" p[1,%d].x = %f\n",i1,cTab[1].p2a[i1].x);
  }
// exit(0);


  //----------------------------------------------------------------
  // test if contour1 is a closed surface or a closed contour.
  // compute mid-value
  // d1 = ((cTab[0].p2a[0].x + cTab[0].p2a[c1pNr - 1].x) / 2.);
  d1 = cTab[0].p2a[c1pNr  / 2].x;
    printf(" mid-val = %lf\n",d1);
  i2 = 0;
  ii = -1;
    // printf(" p[0].x = %f\n",cTab[0].p2a[0].x);
  for(i1=1; i1<c1pNr; ++i1) {
    if(UTN_LIMCK__(d1,cTab[0].p2a[i1 - 1].x,cTab[0].p2a[i1].x)) continue;
    // must test each point extra, since UTN_LIMCK__ provides this twice,
    // if the point fits exact.
    if(cTab[0].p2a[i1].x == d1) continue;
    // test for 2 consecutive points with a different y
    if(i1 == ii+1) continue;
      printf(" %d-%d %lf - %lf\n",i1-1,i1,
             cTab[0].p2a[i1 - 1].x,cTab[0].p2a[i1].x);
    ++i2;
    ii = i1;
  }
    printf(" intersections - i2 = %d\n",i2);
// exit(0);

  // if(fabs(d1) < UT_DISP_cv) return 0;   // its a closed surface; OK; return ..
  if(i2 >= 2) return 0;   // its a closed surface; OK; return ..



  //----------------------------------------------------------------
  // this is not a closed surface. Join 2 contours with lines.

  // Contour2: make direction of x-values consecutive
  // TSU_srv_con_cyclic (cTab[1].p2a, cTab[1].iNr);  // MIST
  // TSU_srv_con_cycX__ (cTab[1].p2a, cTab[1].iNr);  // MIST
  TSU_srv_con_cycX1_ (cTab[1].p2a, cTab[1].iNr);

  // Contour2 must also be a closed_contour; width must be 360 deg !
  i1 = cTab[1].iNr - 1;
  if(fabs(cTab[1].p2a[0].x - cTab[1].p2a[i1].x) < 6) {
    TX_Print("***** Error in Cone-definition; TSU_srv_con_2cylCon");
    return -1;
  }


/
    // TESTDISPLAY:
    for(i1=0; i1<c1pNr; ++i1)
      printf(" p[0,%d].x = %f\n",i1,cTab[0].p2a[i1].x);
    for(i1=0; i1<c2pNr; ++i1)
      printf(" p[1,%d].x = %f\n",i1,cTab[1].p2a[i1].x);
/

  
  // 1. and 2. contour may not have same direction !
  i1 = c1pNr - 1;
  iDir1 = (cTab[0].p2a[0].x > cTab[0].p2a[i1].x) ? 1 : -1; 
  i1 = c2pNr - 1;
  iDir2 = (cTab[1].p2a[0].x > cTab[1].p2a[i1].x) ? 1 : -1;
    // printf(" iDir1=%d iDir2=%d\n",iDir1,iDir2);
  if(iDir1 == iDir2)
    UT3D_cv_inv (cTab[1].iNr, cTab[1].p2a);


  // make same startpoint for both contours
  // set startpoint of C2 to endpoint of C1
  i1 = c1pNr - 1;
  TSU_srv_con_cycX2_ (&cTab[1], cTab[0].p2a[i1].x);
  c2pNr = cTab[1].iNr;
  // d1 = cTab[0].p2a[i1].x - cTab[1].p2a[0].x;
    // printf(" d1 = %lf\n",d1);

  // add d1 to cTab[1]
  // for(i1=0; i1<c2pNr; ++i1)
    // cTab[1].p2a[i1].x += d1;
  

/
    // TESTDISPLAY:
    for(i1=0; i1<c1pNr; ++i1)
      printf(" p[0,%d].x = %f\n",i1,cTab[0].p2a[i1].x);
    for(i1=0; i1<c2pNr; ++i1)
      printf(" p[1,%d].x = %f\n",i1,cTab[1].p2a[i1].x);
// exit(0);
/

  
  // join 1. and 2. contour
  cTab[0].iNr = c1pNr + c2pNr;
  // must copy cTab[1] to overwrite free points of cTab[0]
  memmove(&cTab[0].p2a[c1pNr], cTab[1].p2a, (sizeof(Point) * c2pNr));


 // add first point again
 cTab[0].p2a[cTab[0].iNr] = cTab[0].p2a[0];
 cTab[0].iNr += 1;

 

  // move following contours
  for(i1=2; i1<*cNr; ++i1) {
    cTab[i1-1].iNr = cTab[i1].iNr;
    cTab[i1-1].p2a = cTab[i1].p2a;
  }



  *cNr -= 1;


  // TESTDISPLAY
  printf("ex TSU_srv_con_2cylCon %d\n",*cNr);
  for(i1=0; i1<cTab[0].iNr; ++i1)
    printf(" p[0,%d] %f %f\n",i1,cTab[0].p2a[i1].x,cTab[0].p2a[i1].y);
// exit(0);


  return 1;

}
*/


//================================================================
  double TSU_uOff () {
//================================================================

  return uOff;

}


//===================================================================
  int TSU_tr_init_ (ObjGX *oxi) {
//===================================================================
// Die Umwandlungsfunktionen tess2_tr_2D_3D_pt u tess2_tr_3D_2D_pt setzen
// 3D-Punktetabelle
// Input:
//   oxi    the ObjGX-record of the supportSurface

// TODO: zerlegen in analysefunktion fuer surfaces; 
// fuer SURRV: 
//  -covTypes: LN|AC|EL,PLG,BSPL
//  -surTypes: CYL|CON|SPH|TOR|SRV
//   Zu jeder surfTyp eine eigene struct mit infos.
//   wenn cov=Circ: SPH oder nicht



  int    irc, i1, i2, typ1, typ2, rNr;
  double d1, d2, d3;
  Point  pt1, pt2, *pta;
  Vector vc1, vc2;
  Circ   *ci1, *ci2;
  ObjGX  *ox1, *ox2, obj1;
  SurRev *srv1, srv0;
  Memspc wrkSpc;


  // printf("============================================ \n");
  // printf("TSU_tr_init_ typ=%d form=%d siz=%d dir=%d\n",oxi->typ,oxi->form,
                       // oxi->siz,oxi->dir);
  // UTO_dump__ (oxi, "tr_init:");



  irc = 0;
  TSU_dir = 1;    // nicht umdrehen
  TSU_pintNr = 0;






  //===================================================================
  if(oxi->typ == Typ_CON) {

    TSU_typ_ = Typ_CON;

    TSU_con  = oxi->data;

    TSU_tr_2D_3D_pt = TSU_tr_2D_3D_con;
    TSU_tr_3D_2D_pt = TSU_tr_3D_2D_con;

    // die 2D-Toleranz TSU_tol2D setzen
    TSU_tol2D =  UT_DISP_cv / WC_ask_ModSiz();   // APT_ModSiz;

    // printf(" CON: r1=%f r2=%f h=%f\n",TSU_con->r1,TSU_con->r2,
                  // TSU_con->h);

    // Konus ist im 2D in Zentrumslage;
    // zur Transformation in die echte Lage die Matrizen laden.
    UT3D_m3_loadpl (TSU_mat, &TSU_con->pl);
    // UT3D_stru_dump (Typ_M4x3,  TSU_mat, "              TSU_mat:\n");

    // Matrix invertieren
    UT3D_m3_invm3 (TSU_imat, TSU_mat);

    // Konus hat eine Basisebene, einen Radius r1 in dieser Basisebene,
    // eine Hoehe h und einen Radius r2 in Hoehe h.
    // Konus liegt so im 2D:
    //
    //      Y = Winkel in rad
    //      ^
    //      |
    //      |-------------------|
    //      |                   |
    //      |                   |
    //      |                   |
    //      |                   |
    //      |                   |
    //    --0-------------------|----------> X = Laengsachse Konus
    //                          h
    //

    // Steigung der Mantellinie k = (r1 - r2) / h;
    TSU_dat[6] = (TSU_con->r2 - TSU_con->r1) / TSU_con->h;
    // printf(" Steigung=%f\n",TSU_dat[6]);


    // init
    TSU_tr_2D_3D_con (NULL, NULL);

    uOff = TSU_con->h * 2.;   // U-Bereich von 0 - h

    // Toleranz
    if(TSU_con->r1 > TSU_con->r2) d1 = TSU_con->r1;
    else                          d1 = TSU_con->r2;

    vOff = UT2D_angr_ciSec (UT_DISP_cv, d1);

    vOff *= 1.5;




  //===================================================================
  } else if(oxi->typ == Typ_TOR) {

    TSU_typ_ = Typ_TOR;

    TSU_tor  = oxi->data;

    TSU_tr_2D_3D_pt = TSU_tr_2D_3D_tor;
    TSU_tr_3D_2D_pt = TSU_tr_3D_2D_tor;

    // die 2D-Toleranz TSU_tol2D setzen
    TSU_tol2D =  UT_DISP_cv / WC_ask_ModSiz();   // APT_ModSiz;

    // printf(" TOR: r1=%f r2=%f\n",TSU_tor->r1,TSU_tor->r2);

    // zuerst die Matrix laden
    UT3D_m3_loadpl (TSU_mat, &TSU_tor->pl);
    // UT3D_stru_dump (Typ_M4x3,  TSU_mat, "              TSU_mat:\n");
    // Matrix invertieren
    UT3D_m3_invm3 (TSU_imat, TSU_mat);




    // den hor. offset errechnen (abhaengig vom HauptkreisDmr)
    // Torus: Streifenbreite der  Querstreifen des Rings
    // hoff = 5.;
    // Radius fuer Toleranzberechn.
    d1 = TSU_tor->r1 + TSU_tor->r2;
    uOff = UT2D_angr_ciSec (UT_DISP_cv, d1);
    // uOff *= 1.5;


    // Toleranz vert
    // die Toleranz f die breite der Streifen parallel zum hauptkreis
    // Torus: Streifenbreite der Laengsstreifen des Rings
    // d1 = ((Torus*)sObj)->r1 * 20.;
    // d1 = (((Torus*)sObj)->r1+((Torus*)sObj)->r2) * 16.;
    d1 = TSU_tor->r2;
    // Toleranz
    vOff = UT2D_angr_ciSec (UT_DISP_cv, d1);
    // vOff *= 1.5;


    // init
    TSU_tr_2D_3D_tor (NULL, NULL);


    // printf("  uOff=%f, vOff=%f\n",uOff,vOff);





  //===================================================================
  } else if(oxi->typ == Typ_SURRV) {

/* Revolved Surface
TSU_typ_    = Typ_SURRV
TSU_form_   = Typ Konturkurve; zB Typ_LN fuer einen Cone
TSU_formTyp = Subtyp Konturkurve; 


rotAxis ist entlang Y-Achse = TSU_prx (PT) + TSU_vrx (VC)

X-Wert = U = Winkel am Umfang (um rotAxis); von ang1 bis ang2
Y-Wert = V = Abstand auf der Konturlinie, von v0-v1

TSU_pintNr  = Anzahl intersection-points Achse - Konturlinie
TSU_pint[0] = intersection-points Achse - Konturlinie

TSU_dat[5] Torus: 0=KonturcircleCenter is on axis;  1=not
TSU_dat[6] Torus: Hauptkreisradius
TSU_dat[6] Konus: Steigung in Relation zur Drehachse; 1 od -1.

TSU_dir    Richtung der KonturCurve in Relation zur Drehachse; 1 od -1.

uOff abhaengig von Aussenkonturtyp:
 bei LN nix; else ...

 vOff aus max.Rad errechnen.
*/
    TSU_typ_ = Typ_SURRV;


    // die 2D-Toleranz TSU_tol2D setzen
    TSU_tol2D =  UT_DISP_cv / WC_ask_ModSiz();   // APT_ModSiz;


    // die beiden Kurven (CenterLine und CoverCurve) holen
    // srv1 = oxi->data;
    memcpy (&srv0, oxi->data, sizeof(SurRev));  // 2008-12-28

/*
    // invert axis if dir=revers
    if(srv0.dir) {
      // UT3D_vc_invert (&TSU_vrx, &TSU_vrx)
      MEM_swap_2db (&srv0.ang1, &srv0.ang2);
      // MEM_swap_2db (&TSU_dat[0], &TSU_dat[1]);
      srv0.ang2 += RAD_360;
    }
      printf(" korr-init-a1=%lf a2=%lf\n",srv0.ang1,srv0.ang2);
*/

    srv1 = &srv0;

    TSU_dat[0] = srv1->ang1;
    TSU_dat[1] = srv1->ang2;
    TSU_dat[2] = srv1->v0;
    TSU_dat[3] = srv1->v1;
      // UT3D_stru_dump (Typ_SURRV, srv1, "dump SurRV:"); //  return -1;

    // die Winkel korrigieren; muessen im Bereich -2Pi bis +2Pi sein
    // und aufsteigend.
    UT2D_2angr_set (&TSU_dat[0], &TSU_dat[1], 1);
      // printf(" ang1=%f ang2=%f\n",TSU_dat[0],TSU_dat[1]);


    // Achse holen 
    // printf("Cen-typ=%d ind=%d\n",srv1->typCen, srv1->indCen);
    TSU_ox1 = DB_GetObjGX ((int)srv1->typCen, srv1->indCen);
    // if(TSU_ox1.typ == Typ_Error) return -1;
    typ1 = UTO_obj_getp (&TSU_obj1, &rNr, &TSU_ox1);
    if(typ1 < 0) return -1;
      // UTO_dump__ (&TSU_ox1, "Axis");
      // printf("  o1=%d\n",typ1);


    // Konturkurve holen
    // printf("Cov=%d %d\n",srv1->typCov, srv1->indCov);
    TSU_ox2 = DB_GetObjGX ((int)srv1->typCov, srv1->indCov);
    // if(TSU_ox2.typ == Typ_Error) return -1;
    typ2 = UTO_obj_getp (&TSU_obj2, &rNr, &TSU_ox2);
    if(typ2 < 0) return -1;
      // UTO_dump__ (&TSU_ox2, "Contour");
      // printf("  o2=%d\n",typ2);



    // rotAxis setzen; Punkt TSU_prx + Vector TSU_vrx
    irc = UT3D_ptvc_ox (&TSU_prx, &TSU_vrx, &TSU_ox1);
    if(irc < 0) return irc;


/*
    if(TSU_ox1.form == Typ_PT) {
     TSU_prx = *((Point*)TSU_ox1.data);
     TSU_vrx = UT3D_VECTOR_Z;
     // GR_Disp_vc (&TSU_vrx, &TSU_prx, 1, 0);

    } else if(TSU_ox1.form == Typ_LN) {
     TSU_prx = ((Line*)TSU_ox1.data)->p1;
     UT3D_vc_ln (&TSU_vrx, (Line*)TSU_ox1.data);
     UT3D_vc_setLength (&TSU_vrx, &TSU_vrx, 1.);  // f UT3D_pt_rotptptvcangr
     // UT3D_vc_invert (&TSU_vrx, &TSU_vrx);
     // GR_Disp_vc (&TSU_vrx, &TSU_prx, 1, 0);

    } else if(TSU_ox1.form == Typ_PLN) {
     // UT3D_stru_dump (Typ_PLN, TSU_obj1.vp, "Axis");
     TSU_prx = ((Plane*)TSU_obj1.vp)->po;
     TSU_vrx = ((Plane*)TSU_obj1.vp)->vz;
     // UT3D_vc_setLength (&TSU_vrx, &TSU_vrx, 1.);  // f UT3D_pt_rotptptvcangr


    } else {
      TX_Error("TSU_tr_init_ ESRV001");
      return -1;
    }
*/

    // UT3D_stru_dump (Typ_PT, &TSU_prx, "TSU_prx");
    // UT3D_stru_dump (Typ_VC, &TSU_vrx, "TSU_vrx");
    // GR_Disp_vc (&TSU_vrx, &TSU_prx, 1, 0);


    // den maximalen radius der CoverCurve suchen und daraus den vOff rechnen
    // den Konturstartpunkt auf pt1 setzen
    d1 = 0.;


    TSU_form_ = TSU_ox2.form;

    L_SRV___:
      // printf(" L_SRV___: TSU_form_=%d\n",TSU_form_);



    //----------------------------------------------------------------
    // SRV Contour = Line = Cylinder/Konus
    if(TSU_form_ == Typ_LN) {

      // copy Contourline -> TSU_ln1
      TSU_ln1 = *((Line*)TSU_ox2.data);  // copy line
      TSU_ox2.data = &TSU_ln1;


      // feststellen ob Contourline laenger als ModSiz.
      // Trimmen spaeter durch TSU_srv_con_0.
      UT3D_vc_2pt (&vc1, &TSU_ln1.p1, &TSU_ln1.p2);
      UT3D_bplen_vc (&d1, &vc1);
      d2 = WC_ask_ModSiz();
        // printf(" bplen d1=%f d2=%f\n",d1,d2);

      // TSU_dat[7] = 0: ContourLine < ModSiz;
      // TSU_dat[7] = 1: ContourLine > ModSiz (unbegrenzt !)
// Cones & cylinders only: y-value is the parameter along the contourCurve.
// TSU_dat[7]=oversize (contourCurve can be unlimited !)
// TODO: y-value is the distance from centerpoint prx along centervector vrx
      if(d1 > d2 * 10.) {
        TSU_dat[7] = 1.;    // ATTENTION: PROBLEM WITH SMALL CONES ..

      } else TSU_dat[7] = 0.;


      // Typ of cone in TSU_dat[6]:
      // -1 = cone kleiner werdend entlang Achse
      //  0 = cylinder
      // +1 = cone groesser werdend entlang Achse

      // compute intersection-point Achse - Konturlinie
      // d1 = min. Abst. Achse - Konturline
      irc = UT3D_pt_intptvcln (&TSU_pint[0],&TSU_pint[1],&d1,
                               &TSU_prx,&TSU_vrx, (Line*)TSU_ox2.data);
      // printf(" int %d %f\n",irc,d1);

      // if(d1 < UT_DISP_cv) TSU_pintNr = 1;
        // GR_Disp_pt (&TSU_pint[0], SYM_STAR_S, 2);

      if((irc < 0)  ||    // Cylind od Paraboloid
          ((irc == 0)&&(d1 > UT_DISP_cv))) {
        TSU_pintNr = 0;
        TSU_dat[6] = 0.;

      } else {             // Konus
        TSU_pintNr = 1;
      
        // uOff aus Hauptradius errechnen
        // proj KonturStartpunkt -> Achse
        UT3D_pt_projptptvc(&pt1,&d2, NULL,
                           &((Line*)TSU_ox2.data)->p1,&TSU_prx,&TSU_vrx);
        if(d2 > d1) d1 = d2;
        // proj KonturEndpunkt -> Achse
        UT3D_pt_projptptvc(&pt1,&d3, NULL,
                           &((Line*)TSU_ox2.data)->p2,&TSU_prx,&TSU_vrx);
        if(d3 > d1) d1 = d3;
      }
        // printf(" pintNr=%d dat[6]=%f\n",TSU_pintNr,TSU_dat[6]);


      // nun ist d1 der groesste Radius des cone
      uOff = UT2D_angr_ciSec (UT_DISP_cv, d1) / 2.;
        // printf(" uOff=%f d1=%f tol=%f\n",uOff,d1,UT_DISP_cv);


      // vOff = (srv1->ang2 - srv1->ang1) * 2.;
      vOff = 1.5;   // war 2.  1/6 = 0.25

      // Steigung (sonst nix sichtbar wegen falschem drehsinn)
      if(TSU_pintNr > 0) {   // nicht cyl
        if(d2 > d3) {
          TSU_dat[6] = -1.;
        } else {
          TSU_dat[6] = 1.;

        // nur bei unbegrenztem Konus:
        if(TSU_dat[7] > .5) uOff = RAD_30;

        }
      }

      // Fuer Normalvektor Richtung setzen
      // ist Konturline eher parall oder eher antiparall zur Achse ?
      i1 = UT3D_sid_2vc(&TSU_vrx, &vc1);
      if(i1 < 0) { // Umdrehen nur merken, am Ende alle Konturen umdrehen.
        TSU_dir = 1;
      } else {
        TSU_dir = -1;
      }
        // printf(" conetyp=%f uOff=%f vOff=%f dir=%d overSiz=%f\n",
               // TSU_dat[6],uOff,vOff,TSU_dir,TSU_dat[7]);


      // nun Line TSU_ln1 entsprechend v0/v1 kuerzen
      // printf(" v0=%f v1=%f\n",srv1->v0,srv1->v1);
      // if(fabs(srv1->v0) > UT_TOL_min1) 
      if(UTP_comp2db(srv1->v0, 0., UT_TOL_min1) == 0) {
        UT3D_pt_evparln (&pt1, srv1->v0, &TSU_ln1);
      } else pt1 = TSU_ln1.p1;

      // if(fabs(srv1->v1) > UT_TOL_min1) 
      if(UTP_comp2db(srv1->v1, 1., UT_TOL_min1) == 0) {
        UT3D_pt_evparln (&pt2, srv1->v1, &TSU_ln1);
      } else pt2 = TSU_ln1.p2;

      TSU_ln1.p1 = pt1;
      TSU_ln1.p2 = pt2;
        // UT3D_stru_dump (Typ_LN, &TSU_ln1, " -TSU_ln1-");
// exit(0);


    //----------------------------------------------------------------
    // SRV contour = Circ = Kugel/Torus
    } else if((TSU_form_ == Typ_CI)||(TSU_form_ == Typ_CI)) {

      // wenn Oeffnungswinkel zu klein == 2 Punkte nur -->> Konus
      if(UT3D_ptNr_ci(((Circ*)TSU_ox2.data),UT_DISP_cv) < 3) {
        TSU_ln1.p1 =  ((Circ*)TSU_ox2.data)->p1;
        TSU_ln1.p2 =  ((Circ*)TSU_ox2.data)->p2;
        TSU_ox2.typ  = Typ_LN;
        TSU_ox2.form = Typ_LN;
        TSU_ox2.data = &TSU_ln1;
        TSU_form_ = Typ_LN;
        // printf("  change AC -> LN\n");
        goto L_SRV___;
      }


      // DispKonturkreisStartpunkt und Z-vec
      // GR_Disp_pt(&((Circ*)TSU_ox2.data)->p1, SYM_TRI_B, 2);
      // GR_Disp_vc(&((Circ*)TSU_ox2.data)->vz,&((Circ*)TSU_ox2.data)->pc,2,0);


      // check if KonturcircleCenter is on axis;
      // TSU_dat[5]=0=yes Kugel Center on Axis.        TSU_formTyp=0 Sphere
      // TSU_dat[5]=1=no, Torus Center inside Torus !  TSU_formTyp=1 Tor/degen.
      // TSU_dat[5]=2=no, Torus Center inside Torus    TSU_formTyp=2 Tor/degen
      // TSU_dat[5]=3=no, Torus Center outside Torus.  TSU_formTyp=3 Torus
      // d2 = Konturkreisradius (Radius des Wurstquerschnittes)
      d2 = fabs(((Circ*)TSU_ox2.data)->rad);
      // d1 = dist KonturkreisCenter - Achse (Radius des WurstZentrumsKreises)
      UT3D_pt_projptptvc(&pt1, &d1, NULL,
                         &((Circ*)TSU_ox2.data)->pc, &TSU_prx,&TSU_vrx);
      if(d1 < UT_TOL_cv) {TSU_dat[5] = 0.; TSU_formTyp = 0;} // sphere
      else if(d1 <= d2)  {TSU_dat[5] = 1.; TSU_formTyp = 1;} // degen. Torus
      else               {TSU_dat[5] = 3.; TSU_formTyp = 3;} // normaler Torus
        // printf(" TSU_formTyp=%d %f %f\n",TSU_formTyp,d1,d2);

      TSU_prx    = pt1;    // Startpunkt Rot.Axis == Zentrum Torus
      TSU_dat[6] = d1;     // Torus: Hauptkreisradius


      // Kugel: den Konturkreis -> Halbkreis, begrenzt durch die Achse
      // sonst ParWert(Y)  > 1 und < 0 
      // behebt auch problem mit falschen Normalvektor bei AchsUmdrehen.
      if(TSU_formTyp == 0 ) {       // sphere

        // Wenn der Pol innerhalb der Flaeche liegt, funktioniert hier nix !
        // Die Flaechengrenze muss durch den Pol gehen und darf max 80 Grad
        // gross sein !
        // Kreisbogen darf nicht ueber die Achse drueberragen (max Halbkreis)
        // auch TSU_dir setzen
        i1 = TSU_sph_1 (&TSU_ac1, (Circ*)TSU_ox2.data);
        TSU_ox2.data = &TSU_ac1;
        // Circ invertiert; v0/v1 invertieren !
        if(i1 == 0) UTO_stru_inv (Typ_SURRV, srv1); // Achtung-Torus verkehrt !
        goto L_tor_5;
      }


      // Degenerated Torus (Achse inside Torus)
      // compute intersection-points Konturkreis - Achse
      // hier kann es Problem mit der verwendeten zu hohen toleranz geben !!!
      if(TSU_formTyp == 1) {

        // TSU_pint == die beiden Schnittpunkte Konturkreis-RotAxis suchen.
        TSU_srv_tor_i1 ((Circ*)TSU_ox2.data);
        // irc = UT3D_pt_intlnci__ (&TSU_pintNr, TSU_pint,
                                 // (Line*)TSU_ox1.data, (Circ*)TSU_ox2.data);
          // printf("  %d intersection-points:\n",i1);
          // GR_Disp_pTab (TSU_pintNr, TSU_pint, SYM_STAR_S, 4);
          // GR_Disp_tx (&TSU_pint[0], "1", 2);

        // einen Kreisbogen begrenzt durch die Intersectionpoints machen.
        i1 = TSU_srv_tor_01 (&TSU_ac1, (Circ*)TSU_ox2.data);
        TSU_ox2.data = &TSU_ac1;
        // Circ invertiert; v0/v1 invertieren !
        if(i1 != 0) UTO_stru_inv (Typ_SURRV, srv1);

        // Vollkreis: an Achse begrenzen (siehe TSU_sph_1)
        // TSU_srv_tor_1 (&TSU_ac1, (Circ*)TSU_ox2.data, &TSU_vrx);
        // TSU_ox2.data = &TSU_ac1;

        // TSU_dat[5]: 1:  Hauptradius < Nebenradius (Eifoermig)
        // TSU_dat[5]: 2:  Hauptradius > Nebenradius (standard)
        // pt2 als Mittelpunkt des Kreisbogens ..
        UT3D_pt_midci (&pt2, &TSU_ac1);
        // befindet sich Circ-Center auf gleicher Seite der Achse wie pt2 ?
        // ist winkel pt1-pc / pt1-pt2 groesser oder kleiner 180 ?
        i1 = UT3D_sid_3pt (&pt1, &TSU_ac1.pc, &pt2);
        if(i1 >= 0) {TSU_dat[5] = 2.; TSU_formTyp = 2;}

        // printf(" korr.typTorus=%d\n",TSU_formTyp);


      // Typ 3 - allgemeiner Torus (Center outside Torus)
      } else {

        // den Konturkreis auf Vollkreis mit Startpunkt mitte innen setzen
        TSU_srv_tor_03 (&TSU_ac1, (Circ*)TSU_ox2.data, &TSU_prx, &TSU_vrx);
        TSU_ox2.data = &TSU_ac1;

/* 
        // den Konturkreis auf Vollkreis mit Startpunkt mitte innen setzen
 Wozu ??
        // UT3D_pt_traptvc (&pt1, &TSU_ac1.pc, &TSU_vrx);
        // UT3D_pt_projptci (&pt2, &pt1, &TSU_ac1);
        UT3D_pt_projptci (&pt2, &pt1, &TSU_ac1);
        UT3D_ci_cipt360 (&TSU_ac1, &pt2);
        UT3D_stru_dump (Typ_CI, TSU_ox2.data, " KonturCirc SurRV:");
*/

        // echter Torus: TSU_dir setzen
        // fuer Tori mit Achse im Torus: TSU_srv_tor_2.
        TSU_srv_tor_3 ();
      }


      // uOff aus Hauptradius errechnen
      L_tor_5:
      // den Kreismittelpunkt auf die Achse, dessen Abstand + KreisRad = maxRad
      // UT3D_pt_projptptvc(&pt1,&d1,&((Circ*)TSU_ox2.data)->pc,&TSU_prx,&TSU_vrx);
      d1 = TSU_dat[6] + fabs(((Circ*)TSU_ox2.data)->rad);
      uOff = UT2D_angr_ciSec (UT_DISP_cv, d1) / 2.;
      // uOff = UT2D_angr_ciSec (UT_DISP_cv, d1);
      // printf(" uOff=%f\n",uOff);



      // nun Circ TSU_ac1 entsprechend v0/v1 kuerzen
      // v0/v1 sind Vertikalwinkel in Rad zur Achse Refsys.
      i1 = 0;
        // printf(" v0=%f v1=%f\n",srv1->v0,srv1->v1);
        // UT3D_stru_dump (Typ_CI, &TSU_ac1, " KonturCirc RV1:");
      if(fabs(srv1->v0) > UT_TOL_min1) {
        UT3D_pt_evparci (&pt1, srv1->v0, &TSU_ac1);
          // GR_Disp_pt (&pt1, SYM_STAR_S, 2);
        i1 = 1;
      } else pt1 = TSU_ac1.p1;

      if(fabs(srv1->v1 - 1.) > UT_TOL_min1) {
        UT3D_pt_evparci (&pt2, srv1->v1, &TSU_ac1);
          // GR_Disp_pt (&pt2, SYM_STAR_S, 3);
        i1 = 1;
      } else pt2 = TSU_ac1.p2;

      if(i1 > 0) {
        UT3D_ci_cip3 (&TSU_ac1, &pt1, &pt2);   // fix ango
        // TSU_ox2.data = &TSU_ac1;
      }
        // UT3D_stru_dump (Typ_CI, &TSU_ac1, " KonturCirc RV2:");


      // vOff aus dem Kreisradius errechnen
      d1 = fabs(((Circ*)TSU_ox2.data)->rad);
      vOff = UT2D_angr_ciSec (UT_DISP_cv, d1) / 2.;  // Winkel pro toleranz
      // vOff = UT2D_angr_ciSec (UT_DISP_cv, d1);  // Winkel pro toleranz
      i1 = 1 + fabs(((Circ*)TSU_ox2.data)->ango) / vOff; // anzahl PolygonPunkte
      vOff = 1. / i1;    // vOff fuer Bereich 0 - 1


      // nur SPHere: tol anpassen ..
      if(TSU_formTyp == 0 ) {       // sphere
        uOff *= 2.;
        vOff *= 4.;
      }

      // TSU_dat[6] = 1.;
      // printf(" SRV: uOff=%f vOff=%f\n",uOff,vOff);


        
    //----------------------------------------------------------------
    // SRV Polygon ..
    } else if(TSU_form_ == Typ_CVPOL) {

      // copy obj
      memcpy (TSU_objCont, TSU_ox2.data, sizeof(CurvPoly));
      TSU_ox2.data = TSU_objCont;

      // find knotVals from parVals
      d1 = UT3D_par_par1plg (TSU_dat[2], TSU_ox2.data);
      d2 = UT3D_par_par1plg (TSU_dat[3], TSU_ox2.data);
        // printf(" d1=%lf d2=%lf\n",d1,d2);

      // save limits
      ((CurvPoly*)TSU_objCont)->v0 = d1;
      ((CurvPoly*)TSU_objCont)->v1 = d2;


      // get d1 = maxDist from axis TSU_prx,TSU_vrx
      pta = ((CurvPoly*)TSU_ox2.data)->cpTab;
      i2 = ((CurvPoly*)TSU_ox2.data)->ptNr;
      d2 = 0.;
      for(i1=0; i1 < i2; ++i1) {
        UT3D_nlenq_2ptvc (&d1, &pta[i1], &TSU_prx, &TSU_vrx);
        if(d1 > d2) d2 = d1;
      }
      d1 = sqrt(d2);
      uOff = UT2D_angr_ciSec (UT_DISP_cv, d1);
      vOff = 1.5;   // war 2.  1/6 = 0.25
        // printf(" SRV-plg %lf %lf %lf\n",d1,uOff,vOff);




    //----------------------------------------------------------------
    // SRV B-Spline
    } else if(TSU_form_ == Typ_CVBSP) {

      TSU_bs1 = *(CurvBSpl*)TSU_ox2.data;  // copy spline
      TSU_ox2.data = &TSU_bs1;


      // man kann innen/aussen scheinbar nicht feststellen (Curve kann ja auch
      // kreisfoermig sein !)

      // UT3D_stru_dump (Typ_CVBSP, (CurvBSpl*)TSU_ox2.data, "");

      // v0/v1 auf die Werte des Spline korrigieren !
      TSU_dat[2] = ((CurvBSpl*)TSU_ox2.data)->v0;
      TSU_dat[3] = ((CurvBSpl*)TSU_ox2.data)->v1;
        // printf(" v0/v1=%f,%f\n",TSU_dat[2],TSU_dat[3]);

      // Loop durch die Stuetzpunkte der Curve, Normalabstand auf die Axis
      // d1 = max Normalabstand von der Axis suchen ..
      pta = ((CurvBSpl*)TSU_ox2.data)->cpTab;
      for(i1=0; i1<((CurvBSpl*)TSU_ox2.data)->ptNr; ++i1) {
        UT3D_pt_projptptvc (&pt1, &d2, NULL, &pta[i1], &TSU_prx, &TSU_vrx);
        if(d2 > d1) d1 = d2;
      }
      uOff = UT2D_angr_ciSec (UT_DISP_cv, d1);

      // v0/v1: wenn ein Wert veraendert: Kopie des Spline anlegen und nach
      // TSU_ox2 setzen
      // if((srv1->v0 != 0.)||(srv1->v1 != 1.)) 
      if((srv1->v0 != UT_DB_LEER)||(srv1->v1 != UT_DB_LEER)) {
        // printf(" srv.v0/v1=%f %f\n",srv1->v0,srv1->v1);
        d1 = TSU_bs1.v0;
        d2 = TSU_bs1.v1 - TSU_bs1.v0;
        // see also UT3D_par_par1BSp
        if(srv1->v0 != UT_DB_LEER) TSU_bs1.v0 = d1 + d2 * srv1->v0;
        if(srv1->v1 != UT_DB_LEER) TSU_bs1.v1 = d1 + d2 * srv1->v1;
          // printf(" v0/v1 korr:%f %f\n",TSU_bs1.v0,TSU_bs1.v1);
      }
      
      UT3D_ptNr_bsplTol (&i1, (CurvBSpl*)TSU_ox2.data, UT_DISP_cv);
      // printf(" i1 ex bsplTol = %d\n",i1);
      // Parameterbereich 
      vOff = (TSU_dat[3] - TSU_dat[2]) / i1;

      // feststellen ob geschlossen 


/* dreht vrx scheinbar um; dann sind aber die Winkel ang1/ang2 ziemlich KO !
      // die Richtung Achse/Kontur feststellen. Liegt Kontur eher paralle zur
      // Achse vrx oder eher antiparallel ?
      // TSU_dir = -1; funktioniert hier nicht - wieso ?
      d1 = (TSU_bs1.v0 + TSU_bs1.v1) / 2.;  // vec in mitte curve
      UT3D_vc_evalparCv (&vc1, &TSU_bs1, d1);
      i1 = UT3D_sid_2vc(&TSU_vrx, &vc1);
      if(i1 < 0) {
        printf(" >>>>>> invert BSP <<<<<<<<<<\n");
        UT3D_vc_invert (&TSU_vrx, &TSU_vrx);
      }
*/



    //----------------------------------------------------------------
    // SRV Rat.B-Spline
    } else if(TSU_form_ == Typ_CVRBSP) {
      TX_Error("TSU_tr_init_ ESRV_RBSP");
      return -1;




    //----------------------------------------------------------------
    } else {
      TX_Error("TSU_tr_init_ ESRV002");
      return -1;
    }



    //----------------------------------------------------------------
    // noch die Funktionen zuordnen und initialisieren ..
    if((TSU_form_ == Typ_CI) &&
       (TSU_formTyp == 0))                           {     // Sphere
      TSU_tr_2D_3D_pt = TSU_tr_2D_3D_sph;
      TSU_tr_3D_2D_pt = TSU_tr_3D_2D_sph;
      TSU_tr_2D_3D_sph (NULL, NULL);
      TSU_tr_3D_2D_sph (NULL, NULL);

      // Kugel braucht TSU_mat u TSU_imat
      // Plane aus TSU_ac1 / TSU_vrx
        // printf(" TSU_ox1.form=%d\n",TSU_ox1.form);
      if(TSU_ox1.form == Typ_PLN) {
        TSU_pln = *(Plane*)TSU_ox1.data;
        // set origin = center of Circ TSU_ac1
        UT3D_pl_ptpl (&TSU_pln, &TSU_ac1.pc);

      } else if(TSU_ox1.form == Typ_LN) {
        TSU_sph_pln (); // Plane fuer spher. surf machen

      } else {
        TX_Error("TSU_tr_init_ ESRV003 %d",TSU_ox1.form);
      }
      UT3D_m3_loadpl (TSU_mat, &TSU_pln);
      UT3D_m3_invm3 (TSU_imat, TSU_mat);
        // UT3D_stru_dump (Typ_M4x3, TSU_mat, " TSU_mat:");
        // UT3D_stru_dump (Typ_M4x3, TSU_imat, " TSU_imat:");


    } else {
      // die Transl.Funktionen zuordnen
      TSU_tr_2D_3D_pt = TSU_tr_2D_3D_srv;
      TSU_tr_3D_2D_pt = TSU_tr_3D_2D_srv;

      // init
      TSU_tr_2D_3D_srv (NULL, NULL);
      TSU_tr_3D_2D_srv (NULL, NULL);
    }

    // uOff=0.2;  vOff = 0.2;  // TEST
    // printf("  uOff=%f, vOff=%f\n",uOff,vOff);




  //===================================================================
  } else if(oxi->typ == Typ_SURRU) {
  //===================================================================

/* Ruled-Surface
    Es gibt 2 Kurven zwischen denen die Flaeche liegt;
     die beiden Kurven werden in 2 Polygone mit gleicher Punkteanzahl zerlegt,
     dazwischen wird mit Geraden verbunden.

    Eine Ruled-Sur ist im 2D immer ein Viereck von x=0,y=0 bis x=1,y=1.
     die Kontur 1 ist X-Achsparallel von x=0 bis x=1 mit y=0;
     die Kontur 2 ist X-Achsparallel von x=0 bis x=1 mit y=1;

    Der X-Wert ist der Abstandsparameter der Verbindungsgeraden, auf der der
     Punkt liegt, entlang der 1.Kurve UND entlang der 2. Kurve.
     X=0: die Verbindungsgerade verbindet den Startpunkt der 1.Kurve mit dem
      Startpunkt der 2.Kurve.
     Der Y-Wert ist der Abstand des Punktes auf der Verbindungsgeraden von
      der 1.Kurve weg in Richtung 2. Kurve - zwischen 0 und 1.
      Y=0: der Punkt liegt genau auf der 1.Kurve;
      Y=1: der Punkt liegt genau auf der 2.Kurve;

    Der 2D-Bereich ist als 0-1 / 0-1.
*/

    TSU_typ_ = Typ_SURRU;

    TSU_tr_2D_3D_pt = TSU_tr_2D_3D_sru; // translate-functions
    TSU_tr_3D_2D_pt = TSU_tr_3D_2D_sru;

    // die 2D-Toleranz TSU_tol2D setzen
    TSU_tol2D =  UT_DISP_cv / WC_ask_ModSiz();   // APT_ModSiz;


    ox1  = &((ObjGX*)oxi->data)[0];  // curve1
    ox2  = &((ObjGX*)oxi->data)[1];  // curve2 od Vec
      // printf(" typ1=%d typ2=%d\n",ox1->typ,ox2->typ);


    // set obj1=curve, obj2=point
    if(ox1->typ == Typ_PT) MEM_swap_2vp (&ox1, &ox2);
   


    typ1 = ox1->typ;
    typ2 = ox2->typ;

    // objDaten holen
    typ1 = UTO_obj_getp (&TSU_obj1, &rNr, ox1); //typ = nun spezieller Kurventyp
    if(typ1 < 0) return -1;
    typ2 = UTO_obj_getp (&TSU_obj2, &rNr, ox2);
    if(typ2 < 0) return -1;

      // printf(" typ1=%d typ2=%d\n",typ1,typ2);
      // printf(" 1: form=%d siz=%d dir=%d\n",ox1->form,ox1->siz,ox1->dir);
      // printf(" 2: form=%d siz=%d dir=%d\n",ox2->form,ox2->siz,ox2->dir);
      // UT3D_stru_dump (typ1, TSU_obj1, "ox1");
      // UT3D_stru_dump (typ2, TSU_obj2, "ox2");


    // uOff = 1 / Anzahl_Ecken;
    // vOff = Gesamtbereich (wie bei Konus)
    uOff=1.;  vOff=1.5;   // Startwerte



    //----------------------------------------------------------------
    // zerlegt wird in X-Richtung; uOff fuer die diversen Varianten errechnen
    if((typ1 == Typ_LN)&&(typ2 == Typ_LN)) {

      // den Oeffnungswinkel errechnen
      UT3D_vc_ln (&vc1, (Line*)TSU_obj1);
      UT3D_vc_ln (&vc2, (Line*)TSU_obj2);
      d1 = UT3D_angr_2vc__ (&vc1, &vc2);
      // printf(" OeffAng=%f\n",d1);

      // den Minimalabstand d2 entsprechend Tol. UT_DISP_cv errechnen
      if(d1 > UT_TOL_min1) {
        d2 = (UT_DISP_cv / tan (d1)) * 50.;
      } else {
        d2 = UT_VAL_MAX;
      }
      // printf(" Tol=%f dist=%f\n",UT_DISP_cv,d2);
      // Anzahl Teilstuecke: die kuerzere Strecke teilen.
      d1=UT3D_len_ln((Line*)TSU_obj1);
      d3=UT3D_len_ln((Line*)TSU_obj2);
      if(d3 < d1) d3 = d1;
      uOff = 1. / (d3 / d2);


    //----------------------------------------------------------------
    } else if((typ1 == Typ_LN)&&(typ2 == Typ_VC)) {
      uOff = 1.2;
      vOff = 1.2;



      //----------------------------------------------------------------
    } else {
      // ab hier curve 1
      if(typ1 == Typ_CI) {
        d1 = 1. / UT3D_ptNr_ci ((Circ*)TSU_obj1, UT_DISP_cv);
        if(d1 < uOff) uOff = d1;
        // vc1 = ((Circ*)TSU_obj1)->vz;

        // Closed TSU_pintNr: 1=closed, 0=open
        if(UT3D_ck_ci360((Circ*)TSU_obj1) == 0) TSU_pintNr = 1; // closed


      } else if(typ1 == Typ_CVELL) {
        i1 = UT3D_ptNr_ell ((CurvElli*)TSU_obj1, UT_DISP_cv);
        // UT3D_cv_ell (NULL, &i1, (CurvElli*)TSU_obj1, 10000, UT_DISP_cv);
        d1 = 1. / i1;
        if(d1 < uOff) uOff = d1;
        // vc1 = ((CurvElli*)TSU_obj1)->vz;


      } else if(typ1 == Typ_CVBSP) {
        // printf("  B1: v0=%f v1=%f\n",TSU_obj1.bsp->v0,TSU_obj1.bsp->v1);
        // umrechnen auf wert von 0 bis 1 (y=kx + d); k=TSU_dat[0], d=[1]
        // TSU_dat[0] = TSU_obj1.bsp->v1 - TSU_obj1.bsp->v0;
        // TSU_dat[1] = TSU_obj1.bsp->v0;
        // printf("  B1: k=%f d=%f\n",TSU_dat[0],TSU_dat[1]);

        UT3D_ptNr_bsplTol (&i1, (CurvBSpl*)TSU_obj1, UT_DISP_cv/4.);
          // printf("ex UT3D_ptNr_bsplTol o1: %d\n",i1);
        d1 = 1. / i1;
        if(d1 < uOff) uOff = d1;
          // printf(" uOff-obj1=%f i1=%d d1=%f\n",uOff,i1,d1);

        // Closed  TSU_pintNr: 1=closed, 0=open
        ox1->dir = 0;
        TSU_pintNr = UT3D_bsp_ck_closed_tr ((CurvBSpl*)TSU_obj1);
        if(TSU_pintNr) TSU_pintNr = 0;
        else           TSU_pintNr = 1;
        if(TSU_pintNr > 0) {  // if closed ..
          if(((CurvBSpl*)TSU_obj1)->v0 < ((CurvBSpl*)TSU_obj1)->v1) ox1->dir = 1;
        }
      }


      //----------------------------------------------------------------
      // ab hier curve 2
      if(typ2 == Typ_CI) {
        d1 = 1. / UT3D_ptNr_ci ((Circ*)TSU_obj2, UT_DISP_cv);
        if(d1 < uOff) uOff = d1;
        // vc2 = ((Circ*)TSU_obj2)->vz;


      } else if(typ1 == Typ_CVELL) {
        i1 = UT3D_ptNr_ell ((CurvElli*)TSU_obj2, UT_DISP_cv);
        // UT3D_cv_ell (NULL, &i1, (CurvElli*)TSU_obj2, 10000, UT_DISP_cv);
        d1 = 1. / i1;
        if(d1 < uOff) uOff = d1;
        // vc2 = ((CurvElli*)TSU_obj2)->vz;


      } else if(typ2 == Typ_CVBSP) {
        // printf("  B2: v0=%f v1=%f\n",TSU_obj2.bsp->v0,TSU_obj2.bsp->v1);
        // umrechnen auf wert von 0 bis 1 (y=kx + d); k=TSU_dat[2], d=[3]
        // TSU_dat[2] = TSU_obj2.bsp->v1 - TSU_obj2.bsp->v0;
        // TSU_dat[3] = TSU_obj2.bsp->v0;
        // printf("  B2: k=%f d=%f\n",TSU_dat[2],TSU_dat[3]);

        UT3D_ptNr_bsplTol (&i1, (CurvBSpl*)TSU_obj2, UT_DISP_cv);
        // printf("ex UT3D_ptNr_bsplTol o2: %d\n",i1);
        d1 = 1. / i1;
        if(d1 < uOff) uOff = d1;
          // printf(" uOff-obj2=%f\n",uOff);

        ox2->dir = 0;
        if(TSU_pintNr > 0)
          if(((CurvBSpl*)TSU_obj2)->v0 < ((CurvBSpl*)TSU_obj2)->v1) ox2->dir = 1;
          // if((ox1->dir == 1)&&(ox2->dir == 1)) {ox1->dir=0;ox2->dir=0;}
      }


      // only for 2 circles of 360 deg:
      // set startpoint of circ2 at same position as Circ1
      if((typ1 == Typ_CI)&&(typ2 == Typ_CI)) {
        ci1 = (Circ*)TSU_obj1;
        ci2 = (Circ*)TSU_obj2;
        if((!UT3D_ck_ci360(ci1)) && (!UT3D_ck_ci360(ci2))) {
          // set startpoint at same position as Circ1
          UT3D_vc_2pt (&vc1, &ci1->pc, &ci1->p1);
          UT3D_pt_traptvclen (&ci2->p1, &ci2->pc, &vc1, ci2->rad);
          ci2->p2 = ci2->p1;
          // let vz go into same direction
          i2 = UT3D_sid_2vc (&ci1->vz, &ci2->vz);
          if(i2 < 0) {
            UT3D_vc_invert (&ci2->vz, &ci2->vz);
          }
        }
      }

/*
      // need same direction around axis;
      // invert direction if z-vectors are different (more than 180 deg)
      if(((typ1 == Typ_CI)||(typ1 == Typ_CVELL))   &&
         ((typ2 == Typ_CI)||(typ2 == Typ_CVELL)))      {
          UT3D_stru_dump (Typ_CI, TSU_obj1, "C1:");
          UT3D_stru_dump (Typ_CI, TSU_obj2, "C2:");
        i2 = UT3D_sid_2vc (&vc1, &vc2);
        if(i2 < 0) {
          // ox2->dir = ICHG01 (ox2->dir);
            printf(" i2=%d dir=%d\n",i2,ox2->dir);
        }
      }
*/

    }


    // TEST:
    // if(typ2 == Typ_VC) UT3D_stru_dump (Typ_VC, TSU_obj2.vc, "VC:");



    //----------------------
    // init
    UT3D_sru_init (typ1, ox1->dir, TSU_obj1,
                   typ2, ox2->dir, TSU_obj2, TSU_pintNr);
    // irc = UT3D_parsru_pt (NULL, NULL, NULL);
    SRU_init_basPts();

    TSU_dat[0] = 0.; // uMin
    TSU_dat[1] = 1.; // uMax

      // printf(" SRU - uOff=%f, vOff=%f\n",uOff,vOff);




  //===================================================================
  } else if(oxi->typ == Typ_SURBSP) {  // Init B-SplSurf.
  //===================================================================

    TSU_typ_ = Typ_SURBSP;


    TSU_sbsp = oxi->data;
      // UT3D_stru_dump (TSU_typ_,  TSU_sbsp, "     init SurBSP\n");

    TSU_tr_2D_3D_pt = TSU_tr_2D_3D_sbsp;
    TSU_tr_3D_2D_pt = TSU_tr_3D_2D_sbsp;


    // BSP-Sur liegt so im 2D:
    // als Quadrat von 0-1 in u und in v Richtung.
    //
    //    ----------1
    //    |         |
    //    v         |
    //    |         |
    //    0-----u---- 
    

    // uMin
    TSU_dat[0] = TSU_sbsp->kvTabU[0];
    // uMax
    TSU_dat[1] = TSU_sbsp->kvTabU[TSU_sbsp->ptUNr + TSU_sbsp->degU];
    // vMin
    TSU_dat[2] = TSU_sbsp->kvTabV[0];
    // vMax
    TSU_dat[3] = TSU_sbsp->kvTabV[TSU_sbsp->ptVNr + TSU_sbsp->degV];
    // printf("uMin=%f uMax=%f  vMin=%f vMax=%f\n",TSU_dat[0],TSU_dat[1],
           // TSU_dat[2],TSU_dat[3]);


    // die optimale Anzahl von Punkten in U- und V-Richtung ermitteln
    // SUSbsp_ck_tol (&i1, &i2, TSU_sbsp);
    UME_init (&wrkSpc, memspc53, sizeof(memspc53));
    UT3D_uvNr_sbsp (&i1, &i2, TSU_sbsp, &wrkSpc);
    // printf("  iu=%d iv=%d\n",i1,i2);

    // die 2D-Toleranz TSU_tol2D setzen
    d1 = (((TSU_dat[3])-TSU_dat[2])+(TSU_dat[1]-TSU_dat[0])) / 2.;  // soll 2.!
    TSU_tol2D =  UT_DISP_cv * d1 / WC_ask_ModSiz();   // APT_ModSiz;
    // printf(" d1=%f TSU_tol2D=%f\n",d1,TSU_tol2D);


    // Init Speicherbereich fuer 3D-Gitterpunkte (dzt nur f Typ_SURBSP)
    TSU_grid = (Point*) memspc102;              // was memspc54 !!!
    TSU_gridMax = sizeof(memspc102) / sizeof(Point);
    // printf(" TSU_gridMax=%d\n",TSU_gridMax);


    L_sbsp_L1:
    if(i1*i2 > TSU_gridMax) {
      if((i1 > 4)&&(i2 > 4)) {
        d1 = i1; d1 *= 0.75; i1 = d1;
        d1 = i2; d1 *= 0.75; i2 = d1;
        // printf("  korr. iu=%d iv=%d\n",i1,i2);
        goto L_sbsp_L1;
      } else {
        TX_Error("TSU_tr_init_ E002");
        return -1;
      }
    }


    // feststellen ob in X geschlossen 
    TSU_cycX = UT3D_sbs_ck_cycX (TSU_sbsp);
    // feststellen ob in Y geschlossen 
    // muss neu gemacht werden mit TSU_cycY und UT3D_sbs_ck_cycY
    TSU_pintNr = UT3D_sbs_ck_closed (TSU_sbsp);


    uOff = (TSU_dat[1] - TSU_dat[0]) / (i1-1);  // du
    vOff = (TSU_dat[3] - TSU_dat[2]) / (i2-1);  // dv

    // hilft nix; wieso ?
    uMin = -0.25;   uMax = 1.5;
    vMin = -0.25;   vMax = 1.5;


    // ein Gitternetz fuer ersten Start generieren
    // printf("  ptu=%d ptv=%d\n",TSU_sbsp->ptUNr,TSU_sbsp->ptVNr);
    // nur die Kontrollpunkte sind zuwenig, wenn zB ein kreisformiges Stuck
    // gegeben ist.
    // i1 = TSU_sbsp->ptUNr;
    // i2 = TSU_sbsp->ptVNr;


    TSU_dat[4] = i1; // iu
    TSU_dat[5] = i2; // iv
    UME_init (&wrkSpc, memspc53, sizeof(memspc53));
    UT3D_ptgrid_sbsp (TSU_grid,&TSU_dat[6],&TSU_dat[7],TSU_sbsp,i1,i2,&wrkSpc);
    // printf("  du=%f dv=%f\n",TSU_dat[6],TSU_dat[7]);


    // init
    TSU_tr_2D_3D_sbsp (NULL, NULL);




  //===================================================================
  } else if(oxi->typ == Typ_SURRBSP) {  // Init Rat.B-SplSurf.

    TSU_typ_ = Typ_SURRBSP;


    TSU_srbsp = oxi->data;
      // UT3D_stru_dump (TSU_typ_,  TSU_sbsp, "     init SurBSP\n");

    TSU_tr_2D_3D_pt = TSU_tr_2D_3D_srbsp;
    TSU_tr_3D_2D_pt = TSU_tr_3D_2D_srbsp;


    // RBSP-Sur liegt so im 2D:
    // als Quadrat von 0-1 in u und in v Richtung.
    //
    //    ----------1
    //    |         |
    //    v         |
    //    |         |
    //    0-----u----


    // uMin
    TSU_dat[0] = TSU_srbsp->kvTabU[0];
    // uMax
    TSU_dat[1] = TSU_srbsp->kvTabU[TSU_srbsp->ptUNr + TSU_srbsp->degU];
    // vMin
    TSU_dat[2] = TSU_srbsp->kvTabV[0];
    // vMax
    TSU_dat[3] = TSU_srbsp->kvTabV[TSU_srbsp->ptVNr + TSU_srbsp->degV];
    // printf("uMin=%f uMax=%f  vMin=%f vMax=%f\n",TSU_dat[0],TSU_dat[1],
           // TSU_dat[2],TSU_dat[3]);


    // die optimale Anzahl von Punkten in U- und V-Richtung ermitteln
    // UT3D_ptaNr_srbsp (&i1, &i2, TSU_srbsp);
    UME_init (&wrkSpc, memspc53, sizeof(memspc53));
    UT3D_uvNr_srbsp (&i1, &i2, TSU_srbsp, &wrkSpc);
    // printf("  iu=%d iv=%d\n",i1,i2);

    // die 2D-Toleranz TSU_tol2D setzen
    d1 = (((TSU_dat[3])-TSU_dat[2])+(TSU_dat[1]-TSU_dat[0])) / 2.;  // soll 2.!
    TSU_tol2D =  UT_DISP_cv * d1 / WC_ask_ModSiz();   // APT_ModSiz;
    // printf(" d1=%f TSU_tol2D=%f\n",d1,TSU_tol2D);


    // Init Speicherbereich fuer 3D-Gitterpunkte (dzt nur f Typ_SURBSP)
    TSU_grid = (Point*) memspc102;              // was memspc54 !!!
    TSU_gridMax = sizeof(memspc102) / sizeof(Point);
    // printf(" TSU_gridMax=%d\n",TSU_gridMax);


    L_srbsp_L1:
    if(i1*i2 > TSU_gridMax) {
      if((i1 > 4)&&(i2 > 4)) {
        d1 = i1; d1 *= 0.75; i1 = d1;
        d1 = i2; d1 *= 0.75; i2 = d1;
        // printf("  korr. iu=%d iv=%d\n",i1,i2);
        goto L_srbsp_L1;
      } else {
        TX_Error("TSU_tr_init_ E002");
        return -1;
      }
    }


    // feststellen ob geschlossen
    // letzter Punkt der ersten Kurve
    // TSU_pintNr = UT3D_sbs_ck_closed (TSU_srbsp);
    TSU_cycX = UT3D_srbsp_ck_closed (TSU_srbsp);


    uOff = (TSU_dat[1] - TSU_dat[0]) / (i1-1);  // du
    vOff = (TSU_dat[3] - TSU_dat[2]) / (i2-1);  // dv

    // hilft nix; wieso ?
    uMin = -0.25;   uMax = 1.5;
    vMin = -0.25;   vMax = 1.5;


    // ein Gitternetz fuer ersten Start generieren
    // printf("  ptu=%d ptv=%d\n",TSU_sbsp->ptUNr,TSU_sbsp->ptVNr);
    // nur die Kontrollpunkte sind zuwenig, wenn zB ein kreisformiges Stuck
    // gegeben ist.
    // i1 = TSU_sbsp->ptUNr;
    // i2 = TSU_sbsp->ptVNr;


    TSU_dat[4] = i1; // iu
    TSU_dat[5] = i2; // iv
    UME_init (&wrkSpc, memspc53, sizeof(memspc53));
    UT3D_ptgrid_srbsp (TSU_grid,&TSU_dat[6],&TSU_dat[7],TSU_srbsp,i1,i2,&wrkSpc);
    // printf("  du=%f dv=%f\n",TSU_dat[6],TSU_dat[7]);


    // init
    TSU_tr_2D_3D_srbsp (NULL, NULL);





  //===================================================================
  } else {
    TX_Error("TSU_tr_init_ E001");
    return -1;

  }


    // printf("ex TSU_tr_init_ %d\n",irc);

  return irc;

}


//===================================================================
  int TSU_tr_cont_ (Point *p2Tab, int ip2Max, ContTab *cTab, int *cNr) {
//===================================================================
// Alle Konturen -> 2D
// Wenn keine Aussenkontur existiert: eine Standard-2D-Aussenkontur generieren

  int    irc, i1, i2, i3, i4, ip, intTab[10], intNr;
  double d0, d0t, d1, d2, dmax, dmaxt;
  Point  pt1, *pTab, *p0Tab;


  // cNr = *ctNr;

  // printf("TSU_tr_cont_ %d typ=%d max=%d\n",*cNr,TSU_typ_,ip2Max);
  // printf("   TSU_dat[0-3]=%f,%f,%f,%f\n",TSU_dat[0],TSU_dat[1],
             // TSU_dat[2],TSU_dat[3]);


  // loop tru all contours and get u/v-values from 3D-Points.
  // u/v-values als 2D-Punkte -> ? abspeichern.
  // Die 2D-Pointer in cTab[].p2a eintragen;

  ip = 0;

  for(i1=0; i1<*cNr; ++i1) {

    pTab = cTab[i1].pa;
    cTab[i1].p2a = &p2Tab[ip];  // start of 2D-points


    //================================================================
    // existiert keine 3D-Aussenkontur: 2D-Kontur generieren
    if(pTab == NULL) {
      cTab[i1].typ = 'A'; // A=Automatic
      pTab = &p2Tab[ip];
        // printf(" AutomaticContour %d\n",TSU_typ_);

      if((TSU_typ_ == Typ_SURBSP) ||
         (TSU_typ_ == Typ_SURRBSP))    {
        // Aussenkontur f BSPS generieren
        //                           xmin xmax ymin ymax
        UT3D_cv_boxxy(&p2Tab[ip],TSU_dat[0],TSU_dat[1],TSU_dat[2],TSU_dat[3]);

      } else if(TSU_typ_ == Typ_CON) {
        // Aussenkontur f Conus generieren
        // UT3D_cv_boxxy (&p2Tab[ip], 0., TSU_con->h, -RAD_180, RAD_180);
        UT3D_cv_boxxy (&p2Tab[ip], 0., TSU_con->h, 0., RAD_360);

      } else if(TSU_typ_ == Typ_TOR) {
        // Aussenkontur f Torus generieren
        UT3D_cv_boxxy (&p2Tab[ip], 0., RAD_360, 0., RAD_360);

      } else if(TSU_typ_ == Typ_SURRU) {
        // Aussenkontur f SRU generieren
        UT3D_cv_boxxy (&p2Tab[ip], 0., 1., 0., 1.);

      } else if(TSU_typ_ == Typ_SURRV) {
        // Aussenkontur f SRV generieren xMin      xMax       yMin       yMax
        UT3D_cv_boxxy(&p2Tab[ip],TSU_dat[0],TSU_dat[1],TSU_dat[2],TSU_dat[3]);

      } else {
        printf("TSU_tr_cont_ E001 %d\n",TSU_typ_);
        return -1;
      }

      ip += cTab[i1].iNr;
      continue;

    } else {
      cTab[i1].typ = 'C'; // C=Contour
    }



    //================================================================
    // printf(" TSU_typ_=%d TSU_pintNr=%d\n",TSU_typ_,TSU_pintNr);
    // UT3D_stru_dump (Typ_PT, TSU_pint, "TSU_pint[0]");
    // fuer alle Konturen 3D-Punkte -> 2D-Punkte
    intNr = 0;
    // TSU_pint1Flag = 0;
    // TSU_pint2Flag = 0;
    // loop tru all points of active contour i1

    if((ip + cTab[i1].iNr + 4) > ip2Max) {
      TX_Error("TSU_tr_init_ EOP2");
      return -1;
    }

    pTab = cTab[i1].pa;  // start of 3D-points


    // 2D_points from 3D-points
    for(i2=0; i2<cTab[i1].iNr; ++i2) {
        // UT3D_stru_dump(Typ_PT, &pTab[i2],"c%d p%d",i1,i2);
      irc = (*TSU_tr_2D_3D_pt)(&p2Tab[ip], &pTab[i2]);    // 3D-Pt -> 2D-Pt
        // printf(" P2[%d]=%lf,%lf\n",ip,p2Tab[ip].x,p2Tab[ip].y);

      // cylinders & cones: func = ? 

      // irc -1: delete this Point ..
      // if(irc == -1) MEM_del_nrec (&cTab[i1].iNr, pTab, i2, 1, sizeof(Point));
      // else {
        if(irc < 0) return irc;
        ++ip;
      // }
    }

    // keep 3 free points for each contour;  2012-11-11
    ip += 3;

/*
    //------- NUR TESTAUG ------------------
    printf("---- c%d nach transl:-------- intNr=%d\n",i1,TSU_pintNr);
    pTab = cTab[i1].p2a;
    for(i2=0; i2<cTab[i1].iNr; ++i2)
      printf(" c%d p%d %f %f\n",i1,i2,pTab[i2].x,pTab[i2].y);
    // TEST: 2D-Punkte retour -> 3D
    { Point box1[6];
      for(i2=0; i2<cTab[i1].iNr; ++i2) {
        if(i1 != 2) continue; // disp only cont2
        // (*TSU_tr_3D_2D_pt)(box1, &pTab[i2]);
        // GR_Disp_pt (box1, SYM_STAR_S, 2);
        // GR_Disp_txi (box1, i2, 0);
      }
    }
    // exit(0);
    //------- NUR TESTAUG ------------------
*/



    //================================================================
    // Durchstichpunkte Achse korrigieren (immer 2 gleiche punkte !!)
    // Sollte auch fuer Zonen in BSP-Surs funktionieren, wo die
    // Leitenkurven ident sind (zB Spitze). Noch offen.
/*
    if(intNr < 1) continue;
    for(i2=0; i2<intNr; ++i2) {

      // X-Wert(Winkel) fuer Durchstichpunkte korrigieren
      i3 = intTab[i2];
      printf("   fix inters %d i1=%d i2=%d\n",i2,i3,i3+1);

      // dem ersten Punkt das X vom vorherigen Punkt
      i4 = i3 - 1;
      if(i4 < 0) i4 = cTab[i1].iNr - 1;
      printf(" x%d = x%d\n",i3,i4);
      p2Tab[i3].x = p2Tab[i4].x;
  
      // dem 2. Punkt das X (den Winkel) vom uebernaechsten Punkt geben
      i4 = i3 + 2;
      if(i4 >= cTab[i1].iNr) i4 = 0;
      printf(" x%d = x%d\n",i3+1,i4);
      p2Tab[i3+1].x = p2Tab[i4].x;
    }
*/
    if(TSU_pintNr < 1) continue;

    // Fuer die Polpunkte die X-Werte korrigieren
    TSU_srv_con_top (cTab[i1].p2a, cTab[i1].iNr);

/*
    pTab = cTab[i1].p2a;


    // zuerst den ersten Punkt suchen; X(Angle) auf den vorherigen X-wert.
    // Wenn es gleich der erste ist, auf den Vorletzten X-Wert (der letzte
    // muss ident - somit auch ein Top-Point sein.
    printf("FFFFFFFFFFFFix top-pointsSSSSSSSSSS\n");
    if(pTab[0].x == UT_VAL_MAX) {
      pTab[0].x = pTab[cTab[i1].iNr-2].x;
      pTab[cTab[i1].iNr-1].x = pTab[cTab[i1].iNr-2].x;
      printf(" c%d fix top1: 0 & %d\n",cTab[i1].iNr-1);
      goto L_korr_top_2;
    }

    // es gibt nun x=UT_VAL_MAX und y=0 oder y=1; was ist mit Y ?
    L_korr_top_1:
    for(i2=1; i2<cTab[i1].iNr; ++i2) {
      if(pTab[i2].x != UT_VAL_MAX) continue;
      pTab[i2].x = pTab[i2-1].x;
      printf(" c%d fix top1: %d\n",i1,i2);
      goto L_korr_top_2;
    }
    goto L_korr_top_9;


    // dann den 2. Punkt auf den naechstfolgenden X-Wert setzen ..
    L_korr_top_2:
    for(i2=0; i2<cTab[i1].iNr; ++i2) {
      if(pTab[i2].x != UT_VAL_MAX) continue;
      pTab[i2].x = pTab[i2+1].x;
      printf(" c%d fix top2: %d\n",i1,i2);
      goto L_korr_top_1;
    }
    L_korr_top_9:;
*/
  }
// exit(0);


  //================================================================
  //================================================================
  // nur Testausgabe:
/*
  printf(" 2D-Kontur vor Polkorrektur:\n");
  for(i1=0; i1<*cNr; ++i1) {
    pTab = cTab[i1].p2a;
    GR_Disp_pTab (cTab[i1].iNr, pTab, SYM_STAR_S, 2);
    GR_Disp_cv (pTab, cTab[i1].iNr, 2);
    for(i2=0; i2<cTab[i1].iNr; ++i2) {
      // UT3D_stru_dump (Typ_PT, &pTab[i2], "k%d [%d]",i1,i2);
     GR_Disp_txi (&pTab[i2], i2, 1);
    }
  }
  // return -1;
  printf(" typ=%d TSU_pintNr=%d\n",TSU_typ_,TSU_pintNr);
    // exit(0);
*/




  //================================================================
  // Korrekturen spezifisch der Flaeche:
  //================================================================
  // printf(" Korrekturen Flaeche:\n");
  // printf(" TSU_typ_=%d TSU_pintNr=%d\n",TSU_typ_,TSU_pintNr);




  // Korrektur (geschlossene) BSP-Surfaces
  // if((TSU_typ_ != Typ_SURBSP)&&(TSU_typ_ != Typ_SURRBSP)) goto L_korr_RU;
  if(TSU_typ_ != Typ_SURBSP) goto L_korr_RBSP;
  if(TSU_cycX < 1) goto L_korr_BSPcY;             // nur closed korr.

  d1 = TSU_dat[1] - TSU_dat[0]; // uMax - uMin

  // Korrektur X-Werte;
  for(i1=0; i1<*cNr; ++i1) {
    // automat.generierte Kontur darf nicht korrigiert werden
    if(cTab[i1].typ == 'A') continue;
    // X-Werte zyklisch machen
    TSU_srv_con_cycX__ (cTab[i1].p2a, cTab[i1].iNr, d1);
  }


  goto L_99;


  //----------------------------------------------------------------
  L_korr_BSPcY:
  if(TSU_pintNr < 1) goto L_99;             // nur closed korr.
  // scheinbar eine Y-cyclic-Korrektur:

  // Korrektur Y-Werte;
  // wenn Y-Wert == 0 oder Y-Wert == ? auf den Wert des vorherigen Punkte setzen

  d0    = TSU_dat[2];                // vMin
  d0t   = TSU_dat[2] + TSU_tol2D*3;
  dmax  = TSU_dat[3];                // vMax
  dmaxt = TSU_dat[3] - TSU_tol2D*3;
  // printf(" BSP: d0=%f dmax=%f dmaxt=%f\n",d0,dmax,dmaxt);


  for(i1=0; i1<*cNr; ++i1) {

    // automat.generierte Kontur darf nicht korrigiert werden
    if(cTab[i1].typ == 'A') continue;

    pTab = cTab[i1].p2a;

    for(i2=0; i2<cTab[i1].iNr; ++i2) {
      if(pTab[i2].y < d0t) {
        // vMax wenn der vorherige Punkt naeher bei vMax als bei vMin
        if(i2 == 0) { // find den letzten, der zwischen d0-dmaxt liegt
          for(i3=cTab[i1].iNr-1; i3>=0; --i3) {
            if((pTab[i3].y > d0)&&(pTab[i3].y < dmaxt)) {
              d1=pTab[i3].y; break;
            }
          }
        } else        d1 = pTab[i2-1].y;
        if(UTP_db_cknear_2db(&d1, &d0, &dmax) == 1)
          pTab[i2].y = dmax;  // = vMax

      } else if(pTab[i2].y > dmaxt) {
        // 0 wenn der vorherige Punkt naeher bei 0 als bei 360
        if(i2 == 0) { // find den letzten, der zwischen d0-dmaxt liegt
          for(i3=cTab[i1].iNr-1; i3>=0; --i3) {
            if((pTab[i3].y > d0)&&(pTab[i3].y < dmaxt)) {
              d1=pTab[i3].y; break;
            }
          }
        } else        d1 = pTab[i2-1].y;
        if(UTP_db_cknear_2db(&d1, &d0, &dmax) == 0)
          pTab[i2].y = d0;  // = vMin
      }
    }
    // erster letzter Punkt sehr problemetisch; der letzte stimmt eher ..
    pTab[0].y = pTab[cTab[i1].iNr-1].y;
  }



  goto L_99;





  //================================================================
  // Korrekturen spezifisch der Flaeche:
  //================================================================
  // Korrektur (geschlossene) Rat.BSP-Surfaces
  L_korr_RBSP:
  if(TSU_typ_ != Typ_SURRBSP) goto L_korr_RU;
  if(TSU_cycX < 1) goto L_99;             // nur closed korr.

  // Korrektur X-Werte;

  // Korrektur X-Werte Torus
  for(i1=0; i1<*cNr; ++i1) {
    // automat.generierte Kontur darf nicht korrigiert werden
    if(cTab[i1].typ == 'A') continue;
    // X-Werte(Angles) zyklisch machen
    TSU_bsp_con_cyclic (cTab[i1].p2a, cTab[i1].iNr);
  }

  goto L_99;




  //================================================================
  // Korrektur (geschlossene) RuledSurfaces
  L_korr_RU:
  if(TSU_typ_ != Typ_SURRU) goto L_korr_RV;
  if(TSU_pintNr < 1) goto L_99;             // nur closed korr.

  // printf(" uMin=%f uMax=%f\n",TSU_dat[0],TSU_dat[1]); exit(0);

  d1 = TSU_dat[1] - TSU_dat[0]; // uMax - uMin

  // Korrektur X-Werte;
  for(i1=0; i1<*cNr; ++i1) {
    // automat.generierte Kontur darf nicht korrigiert werden
    if(cTab[i1].typ == 'A') continue;
    // X-Werte zyklisch machen
    TSU_srv_con_cycX__ (cTab[i1].p2a, cTab[i1].iNr, d1);
    // TSU_bsp_con_cyclic (cTab[i1].p2a, cTab[i1].iNr, d1);
  }

  goto L_99;


/*
  // Korrektur X-Werte;
  // wenn X-Wert == 0 oder Y-Wert == 1 auf den Wert des vorherigen Punkte setzen

  d0    = TSU_tol2D*3;  // vMin
  dmax  = 1.;           // vMax
  dmaxt = 1. - TSU_tol2D*3;
  // printf(" RU: d0=%f dmax=%f dmaxt=%f\n",d0,dmax,dmaxt);


  for(i1=0; i1<*cNr; ++i1) {

    // automat.generierte Kontur darf nicht korrigiert werden
    if(cTab[i1].typ == 'A') continue;

    pTab = cTab[i1].p2a;

    for(i2=0; i2<cTab[i1].iNr; ++i2) {
      if(pTab[i2].x < d0) {   // ist min
        if(i2 == 0) { // find den letzten, der zwischen d0-dmaxt liegt
          for(i3=cTab[i1].iNr-1; i3>=0; --i3) {
            if((pTab[i3].x > d0)&&(pTab[i3].x < dmaxt)) {
              d1=pTab[i3].x; break;
            }
          }
        } else        d1 = pTab[i2-1].x;
        if(UTP_db_cknear_2db(&d1, &d0, &dmax) == 1)
          pTab[i2].x = dmax;

      } else if(pTab[i2].x > dmaxt) {  // ist max
        // 0 wenn der vorherige Punkt naeher bei 0 als bei 360
        if(i2 == 0) { // find den letzten, der zwischen d0-dmaxt liegt
          for(i3=cTab[i1].iNr-1; i3>=0; --i3) {
            if((pTab[i3].x > d0)&&(pTab[i3].x < dmaxt)) {
              d1=pTab[i3].x; break;
            }
          }
        } else        d1 = pTab[i2-1].x;
        if(UTP_db_cknear_2db(&d1,(double*)&UT_DB_NULL,&dmax) == 0)
          pTab[i2].x = d0;
      }
    }
    // erster letzter Punkt sehr problemetisch; der letzte stimmt eher ..
    pTab[0].x = pTab[cTab[i1].iNr-1].x;
  }

  goto L_99;
*/





  //================================================================
  // Korrektur RevolvedSurfaces:
  L_korr_RV:
  if(TSU_typ_ != Typ_SURRV)  goto L_99;
    // printf(" KORR SURRV - form=%d formTyp=%d\n",TSU_form_,TSU_formTyp);


  if(TSU_form_ == Typ_LN) goto L_Xkorr_cone;   // cone & cylinder

  if(TSU_form_ == Typ_CI) {
    if(TSU_formTyp == 0) goto L_Xkorr_sph;
    goto L_Xkorr_tor;
  }

  // Cylinders: alle Konturen auf gleiche Seite ..
  if((TSU_form_ == Typ_LN)&&(TSU_pintNr == 0)) goto L_korr_a;


  if(TSU_form_ != Typ_CI) goto L_Xkorr_cone;

  L_korr_a:  //---- Cylinder ----------------------------------------
  // Test / Korrektur X-Werte (Angles) fuer Torus:
  // korrigieren Pole (0 Grad -> 360 Grad  oder 360 Grad -> 0 Grad)
  // test ob Punkte im Bereich 1-179 Grad liegen;
  //   wenn ja:   alle ang=360 -> ang=0
  //   wenn nein: alle ang=0   -> ang=360
  d0    = UT_TOL_cv;
  // d0    = UT_TOL_cv * 2.; // OK fuer > 4000
  // d0    = UT_TOL_cv * 20.; // OK fuer > 400
  d0    = RAD_10;
  dmax  = RAD_360;
  dmaxt = RAD_360 - UT_TOL_cv;
  // dmaxt = RAD_360;
  // dmaxt = RAD_360 - (UT_TOL_cv * 2.);  // OK fuer > 4000
  // dmaxt = RAD_360 - (UT_TOL_cv * 20.);  // OK fuer > 400 
  dmaxt = RAD_360 - RAD_10;

  for(i1=0; i1<*cNr; ++i1) {

    // automat.generierte Kontur darf nicht korrigiert werden
    if(cTab[i1].typ == 'A') continue;

    pTab = cTab[i1].p2a;

    for(i2=0; i2<cTab[i1].iNr; ++i2) {
      if(pTab[i2].x < d0) {
        // 360 wenn der vorherige Punkt naeher bei 360 als bei 0
        if(i2 == 0) { // find den letzten, der zwischen d0-dmaxt liegt
          for(i3=cTab[i1].iNr-1; i3>=0; --i3) {
            if((pTab[i3].x > d0)&&(pTab[i3].x < dmaxt)) {
              d1=pTab[i3].x; break;
            }
          }
        } else        d1 = pTab[i2-1].x;
        if(UTP_db_cknear_2db(&d1,(double*)&UT_DB_NULL,&dmax) == 1)
          pTab[i2].x += RAD_360;
      } else if(pTab[i2].x > dmaxt) {
        // 0 wenn der vorherige Punkt naeher bei 0 als bei 360
        if(i2 == 0) { // find den letzten, der zwischen d0-dmaxt liegt
          for(i3=cTab[i1].iNr-1; i3>=0; --i3) {
            if((pTab[i3].x > d0)&&(pTab[i3].x < dmaxt)) {
              d1=pTab[i3].x; break;
            }
          }
        } else        d1 = pTab[i2-1].x;
        if(UTP_db_cknear_2db(&d1,(double*)&UT_DB_NULL,&dmax) == 0)
          pTab[i2].x -= RAD_360;
      }
    }
    // erster letzter Punkt sehr problemetisch; der letzte stimmt eher ..
    pTab[0].x = pTab[cTab[i1].iNr-1].x;
  }



  
  // check if Startpoint is inside outerContour; NO: move complete contour.
  if(*cNr > 1) TSU_srv_con_inside (cTab, *cNr);

  goto L_XKoor_done;


/*
  d0   = UT_TOL_cv;
  d180 = RAD_180 - UT_TOL_cv;
  d360 = RAD_360 - UT_TOL_cv;

  for(i1=0; i1<*cNr; ++i1) {

    // automat.generierte Kontur darf nicht korrigiert werden
    if(cTab[i1].typ == 'A') continue;

    pTab = cTab[i1].p2a;

    for(i2=0; i2<cTab[i1].iNr; ++i2) {
      if(pTab[i2].x < d0) continue; // skip
      if(pTab[i2].x < d180) goto L_360_to_0;
    }
    L_0_to_360:  // alle ang=0   -> ang=360
    for(i2=0; i2<cTab[i1].iNr; ++i2) {
      if(pTab[i2].x < d0) pTab[i2].x = RAD_360;
    }
    continue;

    L_360_to_0:  // ang=360 -> ang=0
    for(i2=0; i2<cTab[i1].iNr; ++i2) {
      if(pTab[i2].x > d360) pTab[i2].x = 0.;
    }
  }
*/



  //================================================================
  // Test / Korrektur X-Werte (Angles) fuer sphere
  // (KonturCurve == Circ);
  L_Xkorr_sph:
  // printf("L_Xkorr_sph\n");

  for(i1=0; i1<*cNr; ++i1) {
    // automat.generierte Kontur darf nicht korrigiert werden
    if(cTab[i1].typ == 'A') continue;
    // X-Werte(Angles) zyklisch machen
    // TSU_sph_cyclic TSU_srv_con_cycX__ TSU_srv_con_cyclic
    if(TSU_sph_cyclic (cTab[i1].p2a, cTab[i1].iNr) < 0) return -1;
  }

  goto L_99;


  //================================================================
  // Test / Korrektur X-Werte (Angles) fuer Torus
  // (KonturCurve == Circ);
  L_Xkorr_tor:
  // printf("L_Xkorr_tor\n");


  // Korrektur X-Werte Torus
  for(i1=0; i1<*cNr; ++i1) {
    // automat.generierte Kontur darf nicht korrigiert werden
    if(cTab[i1].typ == 'A') continue;
    // X-Werte(Angles) zyklisch machen
    if(TSU_srv_con_cyclic (cTab[i1].p2a, cTab[i1].iNr) < 0) return -1;
  }

  goto L_99;



  //================================================================
  // Test / Korrektur X-Werte (Angles) fuer Cones und Cylinders
  // (KonturCurve == Line);
  L_Xkorr_cone:
    // printf("L_Xkorr_cone\n");


  // make make direction of x-values consecutive
  // if inside boundary are 360-deg-gaps: remove this gaps.
  for(i1=0; i1<*cNr; ++i1) {
    TSU_srv_con_cycX1_ (cTab[i1].p2a, cTab[i1].iNr);
  }


  //----------------------------------------------------------------
  // inner-boundaries must be inside box of outer-boundary.
  // get medium x-val of outer-boundary
  TSU_srv_con_xMid (&d0, cTab[0].p2a, cTab[0].iNr);

  // get medium x-val of inner-boundary; get distance to outer-boundary-center
  // if distance > 200 deg: add/subtract 360 deg.
  for(i1=1; i1<*cNr; ++i1) {
    TSU_srv_con_xMid (&d1, cTab[i1].p2a, cTab[i1].iNr);
    d2 = d0 - d1;
      // printf(" d0=%lf d1=lf d2=%lf\n",d0,d1,d2);
    if(fabs(d2) > 2.5) {
      if(d2 > 0.) d2 = RAD_360;
      else        d2 = -RAD_360;
      TSU_srv_con_mov_x (cTab[i1].p2a, cTab[i1].iNr, d2);
    }
  }



  //----------------------------------------------------------------
/* 2012-11-12
  // for cylindricSurfaces: test if contours are 2 circles or a closed surface
  // If first 2 contours define the surface: join contours.
  irc = TSU_srv_con_2cylCon (cTab, cNr);
  if(irc < 0) return irc;
  if(irc > 0) goto L_con_holes;  // L_XKoor_done;

  // Korrektur X-Werte
  for(i1=0; i1<*cNr; ++i1) {
    // automat.generierte Kontur darf nicht korrigiert werden
    if(cTab[i1].typ == 'A') continue;
    // X-Werte(Angles) zyklisch machen
    if(TSU_srv_con_cyclic (cTab[i1].p2a, cTab[i1].iNr) < 0) return -1;
  }

  L_con_holes:
  // check if Startpoint is inside outerContour; NO: move complete contour.
  // All holes must be in same space as the outerBound;
  // eg outerBound-space is 0-6.28; hole-space -6.28-0 is Error !
  if(*cNr > 1)  TSU_srv_con_inside (cTab, *cNr);
*/

/*
geht nicht. Raus, 2005-11-01

  // korrigieren Pole (0 Grad -> 360 Grad  oder 360 Grad -> 0 Grad)
  // test ob Punkte im Bereich 1-179 Grad liegen;
  //   wenn ja:   alle ang=360 -> ang=0
  //   wenn nein: alle ang=0   -> ang=360

  for(i1=0; i1<*cNr; ++i1) {

    // automat.generierte Kontur darf nicht korrigiert werden
    if(cTab[i1].typ == 'A') continue;

    pTab = cTab[i1].p2a;

    // if(*cNr == 1) {
      // i4 = intTab[0];
    // } else i4 = -1;
    // printf(" intInd=%d,%d\n",i4, i4+1);

    for(i2=1; i2<cTab[i1].iNr; ++i2) {

      // die Durchstichpunkte und die beiden Punkte links und rechts davon
      // darf man nicht testen !
      // Achtung: hier fehlt noch ein Test ob Richtige KonturNr.
      // if(i4 >= 0) {
        // // do net test i4-1, i4, i4+1, i4+2.
        // if((i2-1) == i4) continue;
        // if((i2) == i4) continue;
      // }

      d0 = pTab[i2-1].x-pTab[i2].x;

      // if(d0 > RAD_180) {
      if(d0 > 2.) {
        printf(" c%d +1 [%d]-[%d]\n",i1,i2,cTab[i1].iNr);
        for(i3=i2;i3<cTab[i1].iNr; ++i3) {
          pTab[i3].x += RAD_360;
        }

      // } else if(d0 < -RAD_180) {
      } else if(d0 < -2.) {
        printf(" c%d -1 [%d]-[%d]\n",i1,i2,cTab[i1].iNr);
        for(i3=i2;i3<cTab[i1].iNr; ++i3) {
          pTab[i3].x -= RAD_360;
        }
      }
    }
  }
*/


/* alte methode: check for next PT; geht nicht mit .t2
  dmin = UT_TOL_cv;
  dmax = RAD_360 - UT_TOL_cv;

  for(i1=0; i1<*cNr; ++i1) {
    pTab = cTab[i1].p2a;
    for(i2=0; i2<cTab[i1].iNr; ++i2) {

      // ang=0 -> ang=360
      if(pTab[i2].x < dmin) {
        // wenn der naechste Punkt naeher an RAD360 als an 0, dann x=RAD_360
        for(i3=i2+1; i3<cTab[i1].iNr; ++i3) {
          if(pTab[i3].x < dmin) continue; // skip 
          goto L_123;
        }
        i3 = 0;
        // next index is i3.
        L_123:
        printf(" check 360: %d %d\n",i2,i3);
        if(pTab[i3].x > RAD_180-0.1) {
          pTab[i2].x = RAD_360;
          printf(" set to 360: %d %d\n",i2,i3);
        }

      // ang=360 -> ang=0
      } else if(pTab[i2].x > dmax) {
        // wenn der naechste Punkt naeher an RAD360 als an 0, dann x=0
        for(i3=i2+1; i3<cTab[i1].iNr; ++i3) {
          if(pTab[i3].x > dmax) continue; // skip
          goto L_124;
        }
        i3 = 0;
        // next index is i3.
        L_124:
        printf(" check 0: %d %d\n",i2,i3);
        if(pTab[i3].x > RAD_180-0.1) {
          pTab[i2].x = 0.;
          printf(" set to 0: %d %d\n",i2,i3);
        }
      }
    }
  }
*/



  L_XKoor_done:
/*
  //----------------------------------------------------------------
  // nur Testausgabe:
  printf(" 2D-Kontur vor Y-korrektur:\n");
  for(i1=0; i1<*cNr; ++i1) {
    pTab = cTab[i1].p2a;
    for(i2=0; i2<cTab[i1].iNr; ++i2) {
      UT3D_stru_dump (Typ_PT, &pTab[i2], "k%d [%d]",i1,i2);
      // GR_Disp_pt (&pTab[i2], SYM_TRI_S, 2);
    }
  }
  exit(0);
*/


  // Display 3D:
  // for(i1=0; i1<TSU_pintNr; ++i1) {
    // irc = (*TSU_tr_2D_3D_pt)(&intTab[i1], &TSU_pint[i1]);
    // if(irc < 0) return irc;
    // GR_Disp_pt (&intTab[i1], SYM_TRI_S, 2);
  // }


/*
  //================================================================
  // Korrektur Y-Werte (Parameter)
  // nur SRV-Torus nicht Kugel !!
  // Konturlinie soll so angepasst werden, dass sie nie ueber den Pol
  // hinausgeht!
  // goto L_99;
  if(TSU_typ_ != Typ_SURRV)  goto L_99;
  if((TSU_ox2.form != Typ_CI)&&(TSU_ox2.form != Typ_CI)) goto L_99;
  if(TSU_dat[5] < 0.5) goto L_99;  // skip Kugel

  // den Y-Wert des KonturkreisStartpunktes besorgen. Sinnlos; immer 0,0.

  for(i1=0; i1<*cNr; ++i1) {

    // automat.generierte Kontur darf nicht korrigiert werden
    if(cTab[i1].typ == 'A') continue;

    pTab = cTab[i1].p2a;

    // check if Startpoint is inside outerContour; NO: move complete contour.
    if(i1 > 0) {
      p0Tab = cTab[0].p2a;
      i2 = UT2D_pt_ck_inCv3 (pTab, cTab[0].iNr, p0Tab);
      if(i2 < 0) {   // ausserhalb 
        d0 = pTab[0].y-p0Tab[0].y;
        if(d0 < 0.) d1 =  1.;
        else        d1 = -1.;
        for(i3=i2;i3<cTab[i1].iNr; ++i3) {  // alle Punkte verschieben
          pTab[i3].y += d1;
        }
      }
    }

    for(i2=1; i2<cTab[i1].iNr; ++i2) {
      d0 = pTab[i2-1].y-pTab[i2].y;
      // printf(" k[%d] p[%d] - DY=%f\n",i1,i2,d0);
      // diesen und alle folgenden ..
      if(d0 > 0.5) {
        // printf(" c%d +1 [%d]-[%d]\n",i1,i2,cTab[i1].iNr);
        for(i3=i2;i3<cTab[i1].iNr; ++i3) {
          pTab[i3].y += 1.;
        }
      } else if(d0 < -0.5) {
        // printf(" c%d -1 [%d]-[%d]\n",i1,i2,cTab[i1].iNr);
        for(i3=i2;i3<cTab[i1].iNr; ++i3) {
          pTab[i3].y -= 1.;
        }
      }
    }
  }
*/



  //================================================================
  L_99:


/*
  // nur Testausgabe:
  printf("--------------- ex TSU_tr_init_ ------------ cNr=%d\n",*cNr);
  for(i1=0; i1<*cNr; ++i1) {
    pTab = cTab[i1].p2a;
    for(i2=0; i2<cTab[i1].iNr; ++i2)
      printf("p[%d,%d] = %f %f\n",i1,i2,cTab[i1].p2a[i2].x,cTab[i1].p2a[i2].y);
    // GR_Disp_pTab (cTab[i1].iNr, pTab, SYM_TRI_S, 0);
    // GR_Disp_cv (pTab, cTab[i1].iNr, 2);
    // for(i2=0; i2<cTab[i1].iNr; ++i2) {
      // UT3D_stru_dump (Typ_PT, &pTab[i2], "k%d [%d]",i1,i2);
      // GR_Disp_pt (&pTab[i2], SYM_TRI_S, 0);
    // }
  }
// exit(0);
  // return -1;
*/


  return 0;

}


//===================================================================
  int TSU_tr_2D_3D_con (Point *pt2o, Point *pti) {
//===================================================================
// einen 3D-Konuspunkt auf den 2D-Konus umrechnen

// bei Konusspitze (R=0) konnt Mist; wird aber draussen repariert.


static double oldAng = 0.;
static double angOff = 0.;
static Point  ptOld;

  double   rMid;
  Point    pt31, pt32;


  // Init
  if(pti == NULL) {
    oldAng  = 0.;
    angOff  = 0.;
    ptOld.x = 0.;
    ptOld.y = 0.;
    ptOld.z = 0.;
    return 0;
  }



  // UT3D_pt_evparscon (&pt2o->x, &pt2o->y, pti, TSU_imat);
  
  // zuerst in den Nullpunkt transferieren ...
  UT3D_pt_traptm3 (&pt31, TSU_imat, pti);  // Punkt transformieren

  // X = Hoehe
  pt2o->x = pt31.z;

  // Y ist der Winkel = atan(pti.y/pti.x);
  pt2o->y = UT2D_angr_vc((void*)&pt31); // uebergibt x,y als 2D-Vec.



  //----------------------------------------------------------------
  // wenn die Kontur an die 360-Grad-Grenze geht, kommt sonst Ang=0 !!
  // if((oldAng > RAD_270)&&(pt2o->y < RAD_30)) pt2o->y += RAD_360;
  // if((oldAng < RAD_30)&&(pt2o->y > RAD_270)) pt2o->y -= RAD_360;
  // printf("  Hoehe(X)=%f Winkel(Y)=%f da=%f\n",pt2o->x,pt2o->y,oldAng-pt2o->y);

  // wenn 3D-Y-Wert von + nach - geht
  if(pt31.y < ptOld.y) {   // Y-neu < Y-old
    if((pt31.y < UT_TOL_min1)&&(ptOld.y > -UT_TOL_min1)) {
      // und wenn 2D-Winkel durch 0 geht
      if((oldAng - pt2o->y) < -RAD_180) {
        angOff -= RAD_360;
        // printf( " >>>>>>>>>>>>>> new angOff- %f\n",angOff);
        // printf( "                da=%f\n",oldAng - pt2o->y);
      }
    }

  // wenn 3D-Y-Wert von - nach + geht
  } else if(pt31.y > ptOld.y) {    // Y-neu > Y-old
    if((ptOld.y < UT_TOL_min1)&&(pt31.y > -UT_TOL_min1)) {
      // und wenn 2D-Winkel durch 0 geht
      if((oldAng - pt2o->y) > RAD_180) {
        angOff += RAD_360;
        // printf( " >>>>>>>>>>>>>> new angOff+ %f\n",angOff);
        // printf( "                da=%f\n",oldAng - pt2o->y);
      }
    }
  }

  ptOld  = pt31;
  oldAng = pt2o->y;

  pt2o->y += angOff;

  pt2o->z = 0.;


  // UT3D_stru_dump (Typ_PT2, &pt2o, "ex _2D_3D_con: ");
  // GR_Disp_pt (pt2o, SYM_STAR_S, 2);

  return 0;

}


//===================================================================
  int TSU_tr_3D_2D_con (Point *pt3, Point *pt2i) {
//===================================================================
// einen 2D-Konuspunkt auf den 3D-Konus umrechnen
// Der X-Wert des 2D-Punktes ist die Z-Hoehe vom Basiskreis des Konus;
// Der Y-Wert des 2D-Punktes ist der Winkel am Kreis; also 0-360 Grad.


  UT3D_pt_evparscon (pt3, TSU_con, pt2i->x, pt2i->y, TSU_mat);

  return 0;

}


//===================================================================
  int TSU_tr_2D_3D_tor (Point *pto, Point *pti) {
//===================================================================
// einen 3D-Toruspunkt auf den 2D-Torus umrechnen
// Der X-Wert des 2D-Punktes ist der Winkel am Aussenkreis; also 0-360 Grad.
// Der Y-Wert des 2D-Punktes ist der Winkel am Innenkreis; also 0-360 Grad.
// Radius Aussenkreis = TSU_tor->r1;
// Radius Innenkreis  = TSU_tor->r2;

static double angOff1, angOff2, oldAng1, oldAng2;
static Point  ptOld;

  double   rMid;
  Point    pt3i;
  Vector2  vc2;
  Mat_4x3  ma1, ima1;




  // Init
  if(pti == NULL) {
    // printf("SUStor2_pt_pt3 init\n");
    oldAng1 = 0.;
    oldAng2 = 0.;
    angOff1 = 0.;
    angOff2 = 0.;
    ptOld.x = 0.;
    ptOld.y = 0.;
    ptOld.z = 0.;
    return 0;
  }



  // UT3D_stru_dump (Typ_PT, pti, "\nSUStor2_pt_pt3: ");

  // UT3D_parstor_pt (&u, &v, pti, TSU_tor, TSU_imat);


  // zuerst in den Nullpunkt transferieren ...
  UT3D_pt_traptm3 (&pt3i, TSU_imat, pti);  // Punkt transformieren
  // UT3D_stru_dump (Typ_PT, &pt3i, "           pt3i: ");


  // X = Winkel Aussenkreis; der Punkt liegt immer auf diesem Vektor
  pto->x = UT2D_angr_vc((void*)&pt3i); // uebergibt x,y als 2D-Vec.
  // printf("   x=%f h=%f\n",pt2o->x,TSU_tor->h);


  // Y = Winkel Innenkreis: der Winkel jenes Dreickes, das der Hauptvektor mit
  // dem Vektor vom Hauptkreismittellpunkt zum Punkt bildet.
  // der Y-Wert des Vek ist der Z-Wert des Punktes; der X-Wert ist die Strecke
  // vom MittelPunkt am hauptkreis zum Punkt.
  // vc2.dx=UT2D_len_vc((void*)&pt3i)-TSU_tor->r1; // uebergibt x,y als 2D-Vec
  // uebergibt x,y als 2D-Vec
  vc2.dx = UT2D_len_vc ((Vector2*)&pt3i) - TSU_tor->r1 + TSU_tor->r2;
  vc2.dy = pt3i.z;

  // UT3D_stru_dump (Typ_VC2, &vc2, "           vc2: ");

  pto->y = UT2D_angr_vc (&vc2);
  // printf(" ang2=%f\n",pto->y);






  // wenn 3D-Y-Wert von + nach - geht
  if(pt3i.y < ptOld.y) {   // Y-neu < Y-old
    if((pt3i.y < UT_TOL_min1)&&(ptOld.y > -UT_TOL_min1)) {
      // und wenn 2D-Winkel durch 0 geht
      // printf( " ............    da=%f\n",oldAng1 - pto->y);
      if((oldAng1 - pto->x) < -RAD_180) {
        angOff1 -= RAD_360;
        // printf( " >>>>>>>>>>>>>> new angOff1- %f\n",angOff1);
        // printf( "                 da1=%f\n",oldAng1 - pto->x);
      }
    }

  // wenn 3D-Y-Wert von - nach + geht
  } else if(pt3i.y > ptOld.y) {    // Y-neu > Y-old
    if((ptOld.y < UT_TOL_min1)&&(pt3i.y > -UT_TOL_min1)) {
      // printf( " ............    da=%f\n",oldAng1 - pto->y);
      if((oldAng1 - pto->x) > RAD_180) {
        angOff1 += RAD_360;
        // printf( " >>>>>>>>>>>>>> new angOff1+ %f\n",angOff1);
        // printf( "                 da1=%f\n",oldAng1 - pto->x);
      }
    }
  }
  oldAng1 = pto->x;

  pto->x += angOff1;
  // pto->x *= rMid;    // Umfangsstrecke (fuer Toleranztest beim Tess.)





  // wenn 3D-X-Wert von + nach - geht
  if(pt3i.z < ptOld.z) {   // Y-neu < Y-old
    if((pt3i.z < UT_TOL_min1)&&(ptOld.z > -UT_TOL_min1)) {
      // und wenn 2D-Winkel durch 0 geht
      // if((oldAng > RAD_180)&&(pto->y < RAD_180)) {
      // printf( " ............    da=%f\n",oldAng - pto->y);
      if((oldAng2 - pto->y) < -RAD_180) {
        angOff2 -= RAD_360;
        // printf( " >>>>>>>>>>>>>> new angOff2- %f\n",angOff2);
        // printf( "                 da2=%f\n",oldAng2 - pto->y);
      }
    }
  // wenn 3D-X-Wert von - nach + geht
  } else if(pt3i.z > ptOld.z) {    // Y-neu > Y-old
    if((ptOld.z < UT_TOL_min1)&&(pt3i.z > -UT_TOL_min1)) {
      // if((oldAng < RAD_180)&&(pto->y > RAD_180)) {
      // printf( " ............    da=%f\n",oldAng - pto->y);
      if((oldAng2 - pto->y) > RAD_180) {
        angOff2 += RAD_360;
        // printf( " >>>>>>>>>>>>>> new angOff2+ %f\n",angOff2);
        // printf( "                 da2=%f\n",oldAng2 - pto->y);
      }
    }
  }
  oldAng2 = pto->y;

  pto->y += angOff2;
  // pto->y *= rMid; // Umfangsstrecke (fuer Toleranztest beim Tess.)


  pto->z = 0.;

  // UT3D_stru_dump (Typ_PT2, &pto-> "ex SUStor2_pt_pt3: ");
  ptOld   = pt3i;

  return 0;

}


//===================================================================
  int TSU_tr_3D_2D_tor (Point *pto, Point *pt2i) {
//===================================================================
// einen 2D-Toruspunkt auf den 3D-Torus umrechnen
// Der X-Wert des 2D-Punktes ist der Winkel am Aussenkreis; also 0-360 Grad.
// Der Y-Wert des 2D-Punktes ist der Winkel am Innenkreis; also 0-360 Grad.
// Radius Aussenkreis = TSU_tor->r1;
// Radius Innenkreis  = TSU_tor->r2;


  UT3D_pt_evparstor (pto, TSU_tor, pt2i->x, pt2i->y, TSU_mat);

  return 0;

}


//===================================================================
  int TSU_tr_2D_3D_sph (Point *p2, Point *p3) {
//===================================================================
// make 2D-parameters from 3D-Point on Sphere.

  static double rds;

  // Init
  if(p3 == NULL) {
    rds = fabs(((Circ*)TSU_ox2.data)->rad);
    // printf("TSU_tr_2D_3D_sph  init %f\n",rds);
    return 0;
  }

  // printf("================================================== \n");
  // UT3D_stru_dump(Typ_PT, p3, "TSU_tr_2D_3D_sph:");


  if(p3->x == UT_VAL_MAX) {           // Pol oben
    p2->x = UT_VAL_MAX;
    p2->y = RAD_90;
    goto L_exit;
  }

  if(p3->x == UT_VAL_MIN) {           // Pol unten
    p2->x = UT_VAL_MAX;
    p2->y = -RAD_90;
    goto L_exit;
  }

  UT3D_parsph_pt (&p2->x, &p2->y, p3, rds, TSU_imat);

  L_exit:
  p2->z=0.;
  // UT3D_stru_dump(Typ_PT, p2, "ex TSU_tr_2D_3D_sph:");


  return 0;

}


//===================================================================
  int TSU_tr_3D_2D_sph (Point *p3, Point *p2) {
//===================================================================
// 2D-Point  ->  3D-Point fuer sphere

  static double rds;

  // Init
  if(p3 == NULL) {
    rds = fabs(((Circ*)TSU_ox2.data)->rad);
    // printf("TSU_tr_3D_2D_sph  init %f\n",rds);
    return 0;
  }

  // UT3D_stru_dump(Typ_PT, p2, "TSU_tr_2D_3D_sph:");

  UT3D_pt_evparsph (p3, rds, p2->x, p2->y, TSU_mat);

  // UT3D_stru_dump(Typ_PT, p3, "ex TSU_tr_3D_2D_sph:");

  return 0;

}


//===================================================================
  int TSU_tr_2D_3D_srv (Point *pt2, Point *pti) {
//===================================================================
// 3D-Point pti -> 2D-Point pt2 (U/V-Paramter) fuer RevSur
// 2D-X ist der Winkel und 2D-Y ist der AbstandsParameter auf der Konturlinie
// X-Wert = Winkel am Umfang (um rotAxis); von ang1 bis ang2
// Y-Wert = Abstand auf der Konturlinie, von v0-v1

// Beim Sphere unklar:
//  jede Kugelflaeche laesst 2 Moeglichkeiten; kleinere od groessere Flaeche !
//  Bei Kreis als Flaechenbegrenzung zB 2 Halbkugeln. Welche ist die richtige ?


// ptc = proj PT -> Drehachse;
// Schnitt Plane (durch ptc, normal auf Drehachse) mit Konturkurve
// Schnittpunkt auf Konturkurve: Parameter suchen, -> Y
// Winkel zwischen pti und Schnittpunkt auf Konturkurve -> X

// Punkte genau im Schnittpunkt Konturlinie-Achse werden hier nicht korrigiert.


static int    RS_bp;
static double oldAng, oldPar, parOff;
// static double angOff;
// static double yTot;
// static Point  ptOld;
static Plane  RS_pl;

  int      pNr, iNr, i1, irc;
  double   rdc, d1, d2, da[20];
  Point    pt31, ptc, ptx, pa[20];
  Vector   vc1, vc2;
  Memspc   wrkSpc;


  // Init
  if(pti == NULL) {

/*
    // TESTDISPLAYS ------------
    printf("TSU_tr_2D_3D_srv init TSU_form_=%d\n",TSU_form_);
    printf(" tol=%f\n",UT_TOL_cv);
    UT3D_stru_dump(Typ_PT, &TSU_prx, "prx:");
    UT3D_stru_dump(Typ_VC, &TSU_vrx, "vrx:");
    if(TSU_form_==Typ_LN) UT3D_stru_dump(Typ_LN,TSU_ox2.data,"Cont:");
    if(TSU_form_==Typ_CI) UT3D_stru_dump(Typ_CI,TSU_ox2.data,"Cont:");
    if(TSU_form_==Typ_CI) GR_Disp_ac (TSU_ox2.data, 9);
    if(TSU_form_==Typ_CVBSP) UT3D_stru_dump(Typ_CVBSP,TSU_ox2.data,"Cont:");
    // TESTDISPLAYS end --------
*/
    // oldAng1 = 0.;
    // oldAng2 = 0.;
    // angOff1 = 0.;
    // angOff2 = 0.;
    // angOff  = 0.;
    // ptOld.x = 0.;
    // ptOld.y = 0.;
    // ptOld.z = 0.;

    oldAng  = UT_VAL_MAX;  // noch ungueltig
    oldPar  = UT_VAL_MAX;  // noch ungueltig
    parOff  = 0.;

    // RS_pl = Plane normal zur DrehAchse
    // yTot    = TSU_dat[3] - TSU_dat[2];  // v1-v0
    UT3D_pl_ptvc (&RS_pl, &TSU_prx, &TSU_vrx);
    // Line:
    UT3D_vc_ln (&vc1, (Line*)TSU_ox2.data);
    vc1 = TSU_vrx;   // Backplane der RotAxis besser ?
    RS_bp = UT3D_bp_vc_ (&vc1);
// exit(0);
    return 0;
  }



  // printf("================================================== \n");
  // UT3D_stru_dump(Typ_PT, pti, "TSU_tr_2D_3D_srv:");
  // printf("  TSU_pintAct=%d\n",TSU_pintAct);
  // GR_Disp_pt (pti, SYM_STAR_S, 2);


  // ptc = proj 3D-PT -> Ax;
  UT3D_pt_projptptvc (&ptc, &rdc, NULL, pti, &TSU_prx, &TSU_vrx);
  // GR_Disp_pt (&ptc, SYM_TRI_S, 2);
  // UT3D_stru_dump(Typ_PT, &ptc, "ptc");
  
  // create Plane RS_pl in ptc normal to axis
  UT3D_pl_ptpl (&RS_pl, &ptc);
  // UT3D_stru_dump(Typ_PLN, &RS_pl, "RS_pl");


  //----------------------------------------------------------------
  // Line
  if(TSU_form_ == Typ_LN) {

    // ptx = Schnitt Plane mit Konturkurve
    UT3D_pt_intlnpl (&ptx, &RS_pl, (Line*)TSU_ox2.data);
    // GR_Disp_pt (&ptx, SYM_STAR_S, 4);

    // Schnittpunkt auf Konturkurve: Parameter suchen, -> Y
    pt2->y = UT3D_parpt_lnbp (&ptx, (Line*)TSU_ox2.data, RS_bp);

    goto L_x_0;



  //----------------------------------------------------------------
  // Circ
  } else if(TSU_form_ == Typ_CI) {

/*
Besseres Verfahren waere:
 RS_pl ist Normalebene auf die RotAchse.
 Beim Init den Hauptkreis merken (liegt in der RS_pl).
 Den InputPunkt auf den Hauptkreis draufprojizieren; 
 X =  Winkel des InputPunkt auf der RS_pl.
 den Punkt auf die Drehachse projizieren; nun Winkel od Parameter am
   Konturkreis = Y.


       |        \            dx = ptx-ptc
       |         \           dy = (ptx-pti) - (ptc-c)
    ptx|          pti
       |           \
       |           |         Aus dx und dy kann man nun parameter am Circ
    ptc|    c      |         errechnen. dx und dy muessen signed sein !!
       |           /


*/

    // Methode Schnitt mit Plane zu langsam; eigenes Verfahren notwendig !!!
    // bekannt ist Cir komplett, Achse u Punkt aus Achse.
    // Gesucht der Normalpunkt zu Punkt/Achse am Circ.

    // Schnitt Plane mit Konturkurve
    pNr = UT3D_pt_intplnci (pa, &RS_pl, (Circ*)TSU_ox2.data, 1);
    // printf(" ex intplci %d %d %f %f %f\n",irc,pNr,rdc,d1,d2);
    if(pNr < 0) { // parallel planes
      TX_Error("TSU_tr_2D_3D_srv E002");
      return -1;
    }
    // GR_Disp_pt (&pa[0], SYM_STAR_S, 4);
    // GR_Disp_pt (&pa[1], SYM_STAR_S, 4);

    if(pNr < 1) { // Toleranzproblem; passiert bei den Polen.
      // printf("TSU_tr_2D_3D_srvn I001 \n");
      // if(irc == 2) {  goto L_c1; }
      if(pNr == 2) {  goto L_c1; }       // ?? 2010-12-12
        // // nochmal Abstand testen, ob in Tolerenz UT_DISP_cv
        // GR_Disp_pt (pa, SYM_STAR_S, 2);
        // // Beruehrungspunkt Circ-Plane ist in pa[0]
        // d1 = UT3D_len_2pt (pa, ((Circ*)TSU_ox2.data)->pc);
        // d2 = fabs((Circ*)TSU_ox2.data)->rad;
        // if(UTP_comp2db(d1,d2,UT_DISP_cv) == 1) {    // Beruehrt !!
          // goto L_c1;
        // }
      // }

      TX_Error("TSU_tr_2D_3D_srv E004");
      // GR_Disp_pln (&RS_pl, 2);
      // GR_Disp_pt (pti, SYM_STAR_S, 2);
      // UT3D_stru_dump(Typ_PT, pti, " E004-pos:");
      // pa[0] = TSU_pint[1];
      // Punkt rausloeschen mit MEM_del_nrec ??
      // pNr = 1;
      return -1;
    }

    // der Schnittpunkt muss gleichen Abstand von Achse wie pti haben
    L_c1:
    ptx = pa[0];
    if(pNr > 1) {            // es gibt 2 Schnittpunkte;

      // wenn Kreismittelpunkt auf der Hauptachse liegt sind beide Punkte gleich
      // weit entfernt;
      // if(TSU_dat[5] == 0.) {     // KonturcircleCenter is on axis !
        // // das folgende geht ned,wenn der Konturkreis Vollkreis ist !
        // irc = UT3D_pt_ck_inCirc ((Circ*)TSU_ox2.data, &pa[1], UT_TOL_cv);
        // if(irc >= 0) ptx = pa[1];


      // muss gleich weit entfernt sein wie InputPoint
      // } else {
        // ptc=Inputpoint on Axis
        d1 = UT3D_len_2pt (&ptc, &pa[0]);
        d2 = UT3D_len_2pt (&ptc, &pa[1]);
        // printf(" TSU_tr_2D_3D_srv rdc=%f d1=%f d2=%f\n",rdc,d1,d2);
        // da ist doppelte Tol notwendig (Tess_sur_rv1)   ???
        // if(UTP_comp2db(d1, rdc, UT_TOL_cv*4) == 0) ptx = pa[1];
        i1 = UTP_db_cknear_2db (&rdc, &d1, &d2);
        if(i1 == 1) ptx = pa[1];
      // }



    }
      // GR_Disp_pt (&ptx, SYM_STAR_S, 4);
      // return 0;
     

    // ptx ist nun der Inputpunkt auf der InputKonturkurve.
    // Schnittpunkt auf Konturkurve: Parameter suchen, -> Y
    pt2->y = UT3D_parpt_cipt (&ptx, (Circ*)TSU_ox2.data);
      // printf(" pary=%f\n",pt2->y);


    // Korrektur Y-Wert; Nulldurchgaenge korrigieren.
    if(oldPar == UT_VAL_MAX) goto L_ac_9;
    pt2->y += parOff;
    d1 = pt2->y - oldPar;
    // printf("  d1=%f oldPar=%f parOff=%f\n",d1,oldPar,parOff);
    if(d1 > 0.8)       {pt2->y -= 1.; parOff -= 1.;}
    else if(d1 < -0.8) {pt2->y += 1.; parOff += 1.;}


    L_ac_9:
    oldPar = pt2->y;
    goto L_x_0;



  //----------------------------------------------------------------
  // Polygon: get 2D-Y-value (parametric offset on contourCurve)
  } else if(TSU_form_ == Typ_CVPOL) {

    // intersect plane - contourCurve
    pNr = 20;
    UT3D_pt_intplplg (&pNr, pa, &RS_pl, (CurvPoly*)TSU_ox2.data);
    if(pNr < 1) { printf("TSU_tr_2D_3D_srv PlgE001\n"); return -1; }
    ptx = pa[0];

//TODO:
// crossing 0-360-degree-zone


    // get parameter from point (distance, not 0-1 !)
    UT3D_parplg_plgpt (&pt2->y, &ptx, (CurvPoly*)TSU_ox2.data);



  //----------------------------------------------------------------
  // B-Spline:
  } else if(TSU_form_ == Typ_CVBSP) {

    // Schnitt Plane mit Konturkurve
    pNr = 20; // max.Nr.inters.points/parameters
    UME_init (&wrkSpc, memspc52, sizeof(memspc52));
    UT3D_pt_intplbspl(&pNr,pa,&RS_pl,(CurvBSpl*)TSU_ox2.data,&wrkSpc, UT_TOL_cv);
    if(pNr < 1) { printf("TSU_tr_2D_3D_srv E003\n"); return -1; }

    // Schnittpunkt auf Konturkurve suchen, der Abstand muss ident sein !!
    if(pNr > 1) {
      for(i1=0; i1<pNr; ++i1) {
        d1 = UT3D_len_2pt (&ptc, &pa[i1]);
        if(UTP_comp2db(d1,rdc,UT_DISP_cv) == 1) {iNr = i1; break;}
      }
    } else {
      iNr = 0;
    }
    ptx = pa[iNr];
    // Parameterwert des Punktes -
    UT3D_parCv_bsplpt (&pt2->y, &d1, (CurvBSpl*)TSU_ox2.data, &ptx);




  //----------------------------------------------------------------
  } else {
    TX_Error("TSU_tr_2D_3D_srv E001 %d",TSU_form_);
    return -1;
  }





  L_x_0:
    // printf(" L_x_0:\n");
  // Winkel zwischen pti und Schnittpunkt auf Konturkurve -> X
  // alle 3 Punkte liegen in der Plane RS_pl
  //
  //         pti
  //  ptc
  //         ptx
  UT3D_vc_2pt (&vc1, &ptc, pti);
    // UT3D_stru_dump(Typ_VC, &vc1, "vc1:");
  UT3D_vc_2pt (&vc2, &ptc, &ptx);
    // UT3D_stru_dump(Typ_VC, &vc2, "vc2:");

  pt2->x = UT3D_angr_3vc__ (&TSU_vrx, &vc2, &vc1);

    // printf("_tr_2D_3D_srv x=%f\n",pt2->x);
    // printf("    2D_3D_srv U=%f V=%f\n",pt2->x,pt2->y);
    // if(pt2->x < 0.) pt2->x += RAD_360;

  pt2->z = 0.;



  //----------------------------------------------------------------
  // Korrektur X-Werte (Winkel): Die Nulldurchgaenge korrigieren
  //  5 > 1    +6  wird zu 5 >  7
  //  1 > 5    -6  wird zu 1 > -1
  // Winkel korrigieren: NICHT fuer Punkte auf der Achse moeglich !!!
  // (werden ausserhalb in TSU_tr_cont_ korrigiert)

  // Punkte auf der RotAchse unterbrechen die Kette; oldVal ungueltig setzen.
  // check if Punkt ist auf der RotAchse.
  if(UT3D_comp2pt(pti, &ptc, UT_TOL_cv) == 1) {
    // TSU_pintAct ..
    pt2->x = UT_VAL_MAX;
    oldAng = UT_VAL_MAX;
    goto L_exit;
  }

  // war vorher kein gueltiger Winkel: kein Vergleich moeglich.
  if(oldAng == UT_VAL_MAX) goto L_fertig;

  d1 = pt2->x - oldAng;
  // printf("  d1=%f ang=%f oldAng=%f\n",d1,pt2->x,oldAng);
  if(d1 > RAD_270)      pt2->x -= RAD_360;
  else if(d1 < -RAD_270) pt2->x += RAD_360;



  L_fertig:
  oldAng = pt2->x;


  L_exit:
  // GR_Disp_pt (pt2, SYM_STAR_S, 3);
  // printf(" ex 2D_3D_srv U=%f V=%f\n",pt2->x,pt2->y);

  return 0;



/*
  //----------------------------------------------------------------
  // wenn die Kontur an die 360-Grad-Grenze geht, kommt sonst Ang=0 !!
  // if((oldAng > RAD_270)&&(pt2o->y < RAD_30)) pt2o->y += RAD_360;
  // if((oldAng < RAD_30)&&(pt2o->y > RAD_270)) pt2o->y -= RAD_360;
  // printf("  Hoehe(X)=%f Winkel(Y)=%f da=%f\n",pt2o->x,pt2o->y,oldAng-pt2o->y);

  // wenn 3D-Y-Wert von + nach - geht
  if(pt31.x < ptOld.x) {   // Y-neu < Y-old
    if((pt31.x < UT_TOL_min1)&&(ptOld.x > -UT_TOL_min1)) {
      // und wenn 2D-Winkel durch 0 geht
      if((oldAng - pt2->x) < -RAD_180) {
        angOff -= RAD_360;
        // printf( " >>>>>>>>>>>>>> new angOff- %f\n",angOff);
        // printf( "                da=%f\n",oldAng - pt2o->y);
      }
    }


  //----------------------------------------------------------------
  // wenn 3D-Y-Wert von - nach + geht
  } else if(pt31.x > ptOld.x) {    // Y-neu > Y-old
    if((ptOld.x < UT_TOL_min1)&&(pt31.x > -UT_TOL_min1)) {
      // und wenn 2D-Winkel durch 0 geht
      if((oldAng - pt2->x) > RAD_180) {
        angOff += RAD_360;
        // printf( " >>>>>>>>>>>>>> new angOff+ %f\n",angOff);
        // printf( "                da=%f\n",oldAng - pt2o->y);
      }
    }
  }

  ptOld  = pt31;
  oldAng = pt2->x;

  pt2->x += angOff;

  pt2->z = 0.;


  return 0;
  // return UT3D_parsru_pt (&pt2->x, &pt2->y, pt3);
*/
}


//===================================================================
  int TSU_tr_3D_2D_srv (Point *pt3, Point *pt2) {
//===================================================================
// 2D-Point  ->  3D-Point fuer RevolvedSurf
// 2D-X ist der Winkel und 2D-Y ist der Parameter

// static Circ TSU_ci; // braucht pc,p1,vz.

  Point  pt1;


  // Init
  if(pt2 == NULL) {
    // TSU_ci.pc = TSU_prx;
    // TSU_ci.vz = TSU_vrx;
    return 0;
  }



  // UT3D_stru_dump (Typ_PT, pt2, "TSU_tr_3D_2D_srv %d",TSU_form_);




  //----------------------------------------------------------------
  // den Punkt ensprechend Parameter (Y) auf der Konturkurve ermitteln
  switch (TSU_form_) {

    // Line:     ((Line*)TSU_ox2.data)
    case Typ_LN:
      UT3D_pt_evparln (&pt1, pt2->y, (Line*)TSU_ox2.data);
      break;


    // Circ:      ((Circ*)TSU_ox2.data)
    case Typ_CI:
      UT3D_pt_evparci (&pt1, pt2->y, (Circ*)TSU_ox2.data);
      break;


    // Polygon:      ((CurvPoly*)TSU_ox2.data)
    case Typ_CVPOL:
        // printf(" plg-y=%lf\n",pt2->y);
      // parametervalues = distance, not 0-1 !
      UT3D_pt_evalplg (&pt1, (CurvPoly*)TSU_ox2.data, pt2->y);
      break;


    // B-Spline:      ((CurvBSpl*)TSU_ox2.data)
    case Typ_CVBSP:
      UT3D_pt_evalparCv (&pt1, (CurvBSpl*)TSU_ox2.data, pt2->y);
      break;


    default:
      TX_Error("TSU_tr_3D_2D_srv E001 %d",TSU_form_);
      return -1;
  }



  //----------------------------------------------------------------
    // UT3D_stru_dump (Typ_PT, &pt1, "     pt1=");


  // den Punkt pt1 um den Winkel X-Wert weiterdrehen
  UT3D_pt_rotptptvcangr (pt3, &pt1, &TSU_prx, &TSU_vrx, pt2->x);
  // GR_Disp_pt (pt3, SYM_STAR_S, 3);


  return 0;

}


//===================================================================
  int TSU_tr_2D_3D_sru (Point *pt2, Point *pt3) {
//===================================================================
// 3D-Point pt3 -> 2D-Point (U/V-Paramter) fuer RuledSurf
// ACHTUNG: init mit UT3D_sru_init und UT3D_parsru_pt (..NULL);

  Point  pt1;


  // UT3D_stru_dump(Typ_PT, pt3, "TSU_tr_2D_3D_sru: ");


  pt2->z = 0.;

  // return UT3D_parsru_pt (&pt2->x, &pt2->y, pt3);
  
  return SRU_parsru_intObj (&pt2->x, &pt2->y, &pt1, Typ_PT, (void*)pt3);

}


//===================================================================
  int TSU_tr_3D_2D_sru (Point *pt3, Point *pt2) {
//===================================================================
// 2D-Point  ->  3D-Point fuer RuledSurf
//  Der X-Wert ist der Abstand der Verbindungsgeraden, auf der Punkt liegt,
//   entlang der 1.Kurve UND entlang der 2. Kurve.
//   X=0: die Verbindungsgerade verbindet den Startpunkt der 1.Kurve mit dem
//    Startpunkt der 2.Kurve.
//   Der Y-Wert ist der Abstand des Punktes auf der Verbindungsgeraden von
//    der 1.Kurve weg in Richtung 2. Kurve - zwischen 0 und 1.
//    Y=0: der Punkt liegt genau auf der 1.Kurve;
//    Y=1: der Punkt liegt genau auf der 2.Kurve;


  // printf("TSU_tr_3D_2D_sru \n");

  UT3D_pt_evparsru (pt3, pt2->x, pt2->y);

  return 0;

}


//===================================================================
  int TSU_tr_2D_3D_sbsp (Point *pt2, Point *pti) {
//===================================================================
// 3D-Point  ->  2D-Point fuer BSPS

static int    iu,iv;
static double du,dv;

  int     irc;
  Memspc  wrkSpc;




  // Init dr TSU_tr_init_
  if(pti == NULL) {
    iu = TSU_dat[4];
    iv = TSU_dat[5];
    du = TSU_dat[6];
    dv = TSU_dat[7];
    return 0;
  }



  // UT3D_stru_dump(Typ_PT, pti,"==================\nTSU_tr_2D_3D_sbsp");

  UME_init (&wrkSpc, memspc53, sizeof(memspc53));

  irc = UT3D_parsbsp_pt (&pt2->x,&pt2->y, pti,
                         TSU_grid,du,dv,iu,iv, TSU_sbsp, &wrkSpc);
  if(irc < 0) return irc;

  pt2->z=0.;


  // testdisplay PT
  // printf("ex TSU_tr_2D_3D_sbsp %f,%f\n",pt2->x,pt2->y);
  // GR_Disp_pt (pt2, SYM_TRI_S, 2);


  return 0;

}


//===================================================================
  int TSU_tr_3D_2D_sbsp (Point *pt3, Point *pt2) {
//===================================================================
// 2D-Point  ->  3D-Point fuer BSPS

  int    irc;
  Point  pt1;
  Memspc    wrkSpc;


  // UT3D_stru_dump(Typ_PT, pt2,"TSU_tr_3D_2D_sbsp");

  UME_init (&wrkSpc, memspc53, sizeof(memspc53));

  irc = UT3D_pt_evparsbsp (pt3, TSU_sbsp, pt2->x, pt2->y, &wrkSpc);

  // UT3D_stru_dump(Typ_PT, pt3,"ex TSU_tr_3D_2D_sbsp");

  return irc;

}


//===================================================================
  int TSU_tr_2D_3D_srbsp (Point *pt2, Point *pti) {
//===================================================================
// 3D-Point  ->  2D-Point fuer RBSPS

static int    iu,iv;
static double du,dv;

  int     irc;
  Memspc  wrkSpc;




  // Init dr TSU_tr_init_
  if(pti == NULL) {
    iu = TSU_dat[4];
    iv = TSU_dat[5];
    du = TSU_dat[6];
    dv = TSU_dat[7];
    return 0;
  }



  // UT3D_stru_dump(Typ_PT, pti,"==================\nTSU_tr_2D_3D_srbsp");

  UME_init (&wrkSpc, memspc53, sizeof(memspc53));

  // find u/v-Parameter of 3D-point pti
  irc = UT3D_parsrbsp_pt (&pt2->x,&pt2->y, pti,
                          TSU_grid,du,dv,iu,iv, TSU_srbsp, &wrkSpc);
  if(irc < 0) return irc;

  pt2->z=0.;


  // testdisplay PT
  // printf("ex TSU_tr_2D_3D_srbsp %f,%f\n",pt2->x,pt2->y);
  // GR_Disp_pt (pt2, SYM_TRI_S, 2);


  return 0;

}


//===================================================================
  int TSU_tr_3D_2D_srbsp (Point *pt3, Point *pt2) {
//===================================================================
// 2D-Point  ->  3D-Point fuer BSPS

  int    irc;
  Point  pt1;
  Memspc    wrkSpc;


  // UT3D_stru_dump(Typ_PT, pt2,"TSU_tr_3D_2D_srbsp");

  UME_init (&wrkSpc, memspc53, sizeof(memspc53));

  irc = UT3D_pt_evparsrbsp (pt3, TSU_srbsp, pt2->x, pt2->y, &wrkSpc);

  // UT3D_stru_dump(Typ_PT, pt3,"ex TSU_tr_3D_2D_srbsp");

  return irc;

}


//==========================================================================
  int TSU_DrawSurTBox (Point *tBox, ContTab *cTab, int cNr) {
//==========================================================================
// tBox = aktuelle 2D-Rasterbox
//
//   3 -------- 2
//     |      |
//     |      |
//   0 -------- 1
//
// INPUT:
//   tBox     5 points - a CCW-box 


#define pTabSiz 2000


  int      i1, i2, i3, bcNr, bcAct, c1Nr, p1Nr, p1Max;
  int      crxNr;  // nr of crossing hole-Contours
  int      phNr;
  double   a1;
  Point    *p1Tab, boxp1, boxp2, phTab[pTabSiz];
  Vector   vc1;
  ContTab  bcTab[TSU_CONT_SIZ], c1Tab[TSU_CONT_SIZ], c2Tab;


  // usleep (10000); DL_Redraw();  //TESTWAIT

  // printf("================ TSU_DrawSurTBox %d\n",cNr);


  // memspc for common contour Aussenkontur + Rasterviereck
  p1Tab = (Point*)memspc51;
  p1Max = sizeof(memspc51) / sizeof(Point);
  p1Nr  = p1Max;




  //----------------------------------------------------------------
  // Testdisp 2D-Box
  // printf(" tBox 2D-contour:\n");
  // GR_Disp_pTab (5, tBox, SYM_STAR_S, 9);
  // GR_Disp_cv (tBox, 5, 8);
  // for(i1=0; i1<4; ++i1) GR_Disp_txi (&tBox[i1], i1, 1);

  // Transform Box -> 3D und display:
  // for(i1=0; i1<5; ++i1) (*TSU_tr_3D_2D_pt)(&p1Tab[i1], &tBox[i1]);
  // GR_Disp_cv (p1Tab, 5, 2);
  // return 0;

  // // TestDisplay BoxPoints Lochkonturen (Eckpunkte der 2D-Rechtecke):
  // for(i1=1;i1<cNr; ++i1) {
    // GR_Disp_pt(&cTab[i1].p1,SYM_TRI_B,2);
    // GR_Disp_pt(&cTab[i1].p2,SYM_TRI_B,2);
  // }
  // return 0;
  //----------------------------------------------------------------





  // Union Box und Aussenkontur. 2D.  (Innenkonturen spaeter!)
  // gemeinsamen Bereich von RasterBoxKontur und Aussenkontur in ein oder
  // mehrere gemeinsame Konturen zwischenspeichern. Ergebnis RasterKontur.
  bcNr = TSU_CONT_SIZ;  //max. Nr of Contours
  i1 = GLT_comm_work (&bcNr,bcTab,        // Output - 1-n common contours
                 &p1Nr,p1Tab,             // die points der common contours
                 5,tBox,                  // RasterBoxKontur - Input 1
                 cTab[0].iNr,cTab[0].p2a, // Aussenkontur - Input 2
                 &UT3D_VECTOR_Z);         // Normalvektor  (2D !)
  if(i1 < 0) return i1;
  // bcNr ist die Anzahl gefundener Aussenkonturen.
  // Zu jeder Aussenkontur koennen die Innenkonturen (cTab[1]-cTab[cNr]) kommen.


  // printf(" n.comm_work bcNr=%d p1Nr=%d\n",bcNr,p1Nr);

  // die Ergebniskonturen sind:
  // for(i1=0;i1<bcNr;++i1) bcTab[i1].dir = '+';        // CW ex comm !
  for(i1=0;i1<bcNr;++i1) bcTab[i1].dir = '-';        // CCW ex comm !


  if(p1Nr < 1) return 0;


  // einen Normalvektor vc1 auf die Rasterbox errechnen
  TSU_box_vec (&vc1, tBox);
    // (*TSU_tr_3D_2D_pt)(&boxp1, tBox);   // nur f. TESTDISP
    // GR_Disp_vc (&vc1, &boxp1, 9, 0);
    // printf(" nVc = %lf %lf %lf\n",vc1.dx,vc1.dy,vc1.dz);
    // return 0;  // TEST ONLY !!


  // activate new tesselation-patch
  i1 = GLT_spp_pp (&vc1);
  if(i1 < 0) {
    if(i1 != -10) printf("TSU_DrawSurTBox E001 %d\n",i1);
    return i1;
  }

  // Default- oder Spezialfarbe ?
  // if((col->color != 0) || (col->vtra != 0))
    GLT_spp_Col (&GL_actCol);

/*
  //----------------------------------------------------------------
  // Testdisp commonContours (2D-RasterKontur)
  // printf(" common 2D-contour:\n");
  GR_Disp_pTab (p1Nr, p1Tab, SYM_TRI_S, 4);
  // for(i1=0;i1<p1Nr; ++i1) GR_Disp_txi (&p1Tab[i1], i1, 0);
  for(i1=0;i1<bcNr;++i1) GR_Disp_cv (bcTab[i1].p2a,bcTab[i1].iNr,9);
  // for(i1=0;i1<bcNr;++i1) GR_Disp_spu (bcTab[i1].iNr,bcTab[i1].p2a,5);
  // GLT_spu_ geht nur ganz ohne eine GR_Disp_xx !!
  return 0;
*/
/*
  // Alle Punkte -> 3D
  // printf(" common 3D-contour:\n");
  for(i1=0;i1<bcNr;++i1) {
    p1Tab = bcTab[i1].p2a;
    for(i2=0;i2<bcTab[i1].iNr; ++i2) {
      (*TSU_tr_3D_2D_pt)(&phTab[i2],&p1Tab[i2]);
    }
    GR_Disp_pTab (bcTab[i1].iNr, phTab, SYM_STAR_S, 2);
    if(i1 == 1) GR_Disp_cv (phTab, bcTab[i1].iNr, 9);
  }
  // for(i1=0;i1<p1Nr; ++i1) {(*TSU_tr_3D_2D_pt)(&phTab[i1],&p1Tab[i1]);}
    // GR_Disp_pTab (p1Nr, phTab, SYM_STAR_S, 2);
  return 0;
  //----------------------------------------------------------------
*/



  // Keine Lochkonturen:
  if(cNr < 2) {
    // Konturen -> 3D und darstellen.
    for(i1=0;i1<bcNr;++i1) {
      TSU_box_Draw1 (bcTab[i1].iNr, bcTab[i1].p2a, bcTab[i1].dir, &vc1);
    }
    return 0;
  }




  //================================================================
  // Es gibt mind. 1 Lochkontur.
  // nun befinden sich bcNr Aussenkonturen in bcTab (p1Tab). 
  // Loop ueber alle Aussenkonturen:
  // - die Innenkontueren abziehen, dann ausgeben.
  bcAct = 0;



  L_next:

  // work Contour bcAct
  // printf(">>>>>>>>>>>>>>> work bcAct=%d\n",bcAct);

  // Boxpunkte der Rasterbox ermitteln (boxp1, boxp2)
  UT3D_box_pts (&boxp1, &boxp2, bcTab[bcAct].iNr, bcTab[bcAct].p2a);
    // GR_Disp_box (&boxp1, &boxp2, 9);
    // GR_Disp_pt(&boxp1, SYM_TRI_B, 3);
    // GR_Disp_pt(&boxp2, SYM_TRI_B, 3);
    // return 0;  // NUR TEST !!





  // Beruehren oder kreuzen activeAussenkontur - Innenkontur;
  // set deactivation-Flag.
  // cTab.use='A'=active, 'I'=inactive.
  crxNr = 0;
  for(i1=1; i1<cNr; ++i1) {

    // wenn keine Kontur vorhanden:
    if(cTab[i1].iNr < 4) {
      cTab[i1].use = 'I';   // I Inactive
      continue;
    }

    // Box testen
    i2 = UT2D_ckBoxinBox1 ((Point2*)&boxp1,       (Point2*)&boxp2,
                           (Point2*)&cTab[i1].p1, (Point2*)&cTab[i1].p2);
    if(i2 < 0){ cTab[i1].use = 'I';}  // I Inactive, not overlap
    else      { cTab[i1].use = 'A'; ++crxNr;}
  }
    // return 0;  // NUR TEST !!


  //----TESTDISP
  // printf("<<<<< crxNr=%d \n",crxNr);
  // for(i1=1; i1<cNr; ++i1) printf(" c[%d].use=%c\n",i1,cTab[i1].use);
  //----TESTDISP



  // keine active Innenkonturen: skip join Rasterkontur/Loecher
  if(crxNr < 1) {
    // Konturen -> 3D und darstellen.
    // draw
    TSU_box_Draw1 (bcTab[bcAct].iNr, bcTab[bcAct].p2a, bcTab[0].dir, &vc1);
    // TSU_box_Draw2 (bcNr, bcTab, &vc1);
    goto L_goto_next;
  }



  // reserve memspc for 2D-Points of (Aussenkontur - Innenkonturen)
  // dzt im Stack.


  // remove holes from contour (2D!)
  // Input:  die Aussenkontur in p1Tab[p1Nr]
  // Input:  die Innenkonturen in cTab/cNr (erst ab cTab[1])
  // Output: die fertigen Konturen in c1Tab, Punkte dazu in p2Tab
  c1Nr = TSU_CONT_SIZ;
  phNr = pTabSiz;
  GLT_diff_work (&c1Nr, c1Tab,    // Outputcontours 
                     &phNr, phTab,    // Outputpoints
                     bcTab[bcAct].iNr, bcTab[bcAct].p2a,      // Aussenkontur
                     cNr, cTab,       // Lochkonturen (skip erste=FaceBorder)
                     &UT3D_VECTOR_Z); // 2D !
  // ACHTUNG: kann auch 2 Aussenkonturen liefern !!
  //     +---+
  //     |   |
  //  +---------+
  //  |  |   |  | Loch  - zerschneidet cTab zu 2 Aussenkonturen
  //  +---------+
  //     |   |
  //     +---+
  //    Box/Aussenkontur = cTab
  //

  //     +---+
  //     |   |
  //     |+-+|
  //     || ||    Loch - bleibt Loch.
  //     |+-+|   
  //     |   |
  //     +---+
  //    Box/Aussenkontur = cTab
  //


  // Feststellen muss liefern ob Kontur aussen oder Innenkontur ist !!!!!!!
  // Kann man Umdrehungssinn im GL abfragen ? Ev gluGetTessProperty ?
  // es geht, weil GLT_diff_work den Umdrehungssinn richtig macht.

  // Der Umdrehungssinn sollte hier Aussen/Innnenkontur definieren !!!
  // die Ergebniskonturen sind ? !!
  // for(i1=0;i1<c1Nr;++i1) c1Tab[i1].dir = '-';
  // for(i1=0;i1<c1Nr;++i1) c1Tab[i1].dir = '+';



  // printf(" n.diff_work c1Nr=%d phNr=%d\n",c1Nr,phNr);


  if(c1Nr < 1) goto L_goto_next;



/*
  //----------------------------------------------------------------
  // TestDisp fertig gelochte Konturen (2D-Konturen)
  for(i1=0; i1<c1Nr; ++i1) {
    printf("  [%d] iNr=%d\n",i1,cTab[i1].iNr);
    GR_Disp_pTab (c1Tab[i1].iNr, c1Tab[i1].p2a, SYM_TRI_S, 4);
    GR_Disp_cv (c1Tab[i1].p2a, c1Tab[i1].iNr, 2);
    GR_Disp_spu (c1Tab[i1].iNr, c1Tab[i1].p2a, 5);
  }
*/
/*
  // Alle Punkte -> 3D
  Point    pxTab[pTabSiz];
  for(i1=0;i1<c1Nr;++i1) {
    p1Tab = c1Tab[i1].p2a;
    for(i2=0;i2<c1Tab[i1].iNr; ++i2) {
      (*TSU_tr_3D_2D_pt)(&pxTab[i2],&p1Tab[i2]);
    }
    GR_Disp_pTab (c1Tab[i1].iNr, pxTab, SYM_STAR_S, 2);
    GR_Disp_cv (pxTab, c1Tab[i1].iNr, 9);
  }
  return 0;
  //----------------------------------------------------------------
*/

  // Achtug: dir hier noch nicht definiert !!!
  TSU_box_Draw2 (c1Nr, c1Tab, &vc1);




  //================================================================
  L_goto_next:
  ++bcAct;
  if(bcAct < bcNr) goto L_next;


  return 0;

}


//================================================================
  int TSU_box_Draw1 (int ptNr, Point *p2Tab, char dir, Vector *vc1) {
//================================================================
// eine (ungelochte, fertige) Aussenkontur ausgeben.

  int        i1, poMax;
  Point      *poTab;
  ContTab    cTab;


  // printf("TSU_box_Draw1 %d dir=%d\n",ptNr,TSU_dir);
  // UT3D_stru_dump (Typ_VC, vc1, "   vc1");


  poTab = (Point*)memspc52;
  poMax = sizeof(memspc52) / sizeof(Point);

  if(ptNr > poMax) {TX_Error("TSU_box_Draw1 E001"); return -1;}


  for(i1=0;i1<ptNr; ++i1) {
    (*TSU_tr_3D_2D_pt)(&poTab[i1], &p2Tab[i1]);
  }


  if(TSU_dir < 0) {
      if(dir == '+') cTab.dir = '-';
      else           cTab.dir = '+';
  } else cTab.dir = dir;

    // GR_Disp_pTab (c1Tab[i1].iNr, pxTab, SYM_STAR_S, 2);
    // GR_Disp_cv (pxTab, c1Tab[i1].iNr, 9);


    cTab.iNr = ptNr;
    cTab.pa  = poTab;


    GLT_spp__ (&cTab, 1, vc1);

  return 0;

}

 
//================================================================
  int TSU_box_Draw2 (int cNr, ContTab *cTab, Vector *vc1) {
//================================================================
// Konturen von 2D -> 3D transformieren und ausgeben = darstellen.
// add 3D-Points -> ciTab[].pa
// ACHTUNG: do not use memspc53 - used by TSU_tr_2D_3D_sbsp
//          2D-Inputpoints (cTab[].p2a) liegen in memspc51

  int      i1, i2, i3, poMax;
  Point    *piTab, *poTab;


  // printf("TSU_box_Draw2 %d\n",cNr);


  // memspc for 3D-Punkte
  poTab = (Point*)memspc52;
  poMax = sizeof(memspc52) / sizeof(Point);



  // fertige Kontur -> 3D ruecktransformieren
  i3 = 0;
  for(i1=0; i1<cNr; ++i1) {
    // printf(" c[%d] nr=%d i3=%d\n",i1,cTab[i1].iNr,i3);
    piTab = cTab[i1].p2a;
    cTab[i1].pa  = &poTab[i3]; // Startpunkt Punkttabelle
    // cTab[i1].dir = '+';        // CW ex comm !
    for(i2=0; i2<cTab[i1].iNr; ++i2) {
      (*TSU_tr_3D_2D_pt)(&poTab[i3], &piTab[i2]);
      ++i3;
      if(poMax <= i3) {TX_Error("TSU_box_Draw2 E001"); return -1; }
    }
  }


  // umdrehen
  if(TSU_dir < 0) {
    for(i1=0; i1<cNr; ++i1) {
      if(cTab[i1].dir == '+') cTab[i1].dir = '-';
      else                    cTab[i1].dir = '+';
    }
  }

/*
  //--------TestDisplay Beg
  for(i1=0;i1<cNr;++i1) GR_Disp_pTab (cTab[i1].iNr,cTab[i1].pa,SYM_TRI_B,0);
  for(i1=0;i1<cNr;++i1) GR_Disp_cv (cTab[i1].pa, cTab[i1].iNr, 2);
  for(i1=0;i1<cNr;++i1) GR_Disp_spu (cTab[i1].iNr,cTab[i1].pa,5);
  // Test nVec:
  // UT3D_vc_perppta (vc1, cTab[0].iNr, cTab[0].pa);
  // GL_Disp_vc (cTab[0].pa, vc1);
  return 0;
  //--------TestDisplay End
*/

  // raus; 3D-Punkte in cTab[].pa
  GLT_spp__ (cTab, cNr, vc1);


  return 0;

}


//================================================================
  int TSU_box_vec (Vector *pVec, Point *tBox) {
//================================================================
// einen Normalvektor pVec auf die Rasterbox tBox errechnen
// Punkt pm als Zentrum der Rasterbox nur fuer Hilfsberechnungen und
//   zum Display.


  int       i1;
  double    d1;
  Point     pm, p0, p1, p2, p3;
  Vector    v1;
  Line      ln1;

 
  // TESTBLOCK
  // printf("TSU_box_vec typ=%d tol2D=%f\n",TSU_typ_,TSU_tol2D);
  // for(i1=0;i1<4;++i1) UT3D_stru_dump(Typ_PT,&tBox[i1]," P[%d]",i1);

  // TestDisplay 3D-Box
  Point pa[5];
  (*TSU_tr_3D_2D_pt)(&pa[0], &tBox[0]);
  (*TSU_tr_3D_2D_pt)(&pa[1], &tBox[1]);
  (*TSU_tr_3D_2D_pt)(&pa[2], &tBox[2]);
  (*TSU_tr_3D_2D_pt)(&pa[3], &tBox[3]); pa[4] = pa[0];
  // GL_Disp_cv (5, pa);
  // GR_Disp_pt (&pa[0], SYM_TRI_S, 1); GR_Disp_tx (&pa[0], "0", 1);
  // GR_Disp_pt (&pa[1], SYM_TRI_S, 1); GR_Disp_tx (&pa[1], "1", 1);
  // GR_Disp_pt (&pa[2], SYM_TRI_S, 1); GR_Disp_tx (&pa[2], "2", 1);
  // GR_Disp_pt (&pa[3], SYM_TRI_S, 1); GR_Disp_tx (&pa[3], "3", 1);
  // GR_Disp_cv (pa, 5, 2);
  // TESTBLOCK



  // printf(" typ_=%d form=%d Steig=%f\n",TSU_typ_,TSU_ox2.form,TSU_dat[6]);

  //----------------------------------------------------------------
  if(TSU_typ_ == Typ_CON) {
    // hier Fehler bei Conus, wenn 2 tBoxPunkte ident sind (=Conusspitze)
    // Conus: tBox[0], tBox[1] sind eine Mantellinie;
    // con.po ist center;
    (*TSU_tr_3D_2D_pt)(&p1, &tBox[0]);
    (*TSU_tr_3D_2D_pt)(&p2, &tBox[1]);

    if(TSU_dat[6] > 0.) {   // Steigung; ev tBox[0] u tBox[2] ident !!!
      (*TSU_tr_3D_2D_pt)(&p3, &tBox[2]);
    } else {
      (*TSU_tr_3D_2D_pt)(&p3, &tBox[3]);
    }
    UT3D_vc_perp3pt (pVec, &p1, &p2, &p3);
    goto L_vec_OK;




  //----------------------------------------------------------------
  } else if(TSU_typ_ == Typ_SURRV) {


    //------- SRV/KONUS ---------------
    // Vektor pVec soll nach innen zeigen !!
    if(TSU_ox2.form == Typ_LN) {
      // printf("TSU_box_vec SRV / LN typ=%f\n",TSU_dat[6]);

      if(TSU_dat[6] == 0.) {         // Cylinder !
        UT3D_pt_mid2pt (&p1, &tBox[0], &tBox[2]);
        // p1.x ist der Winkel, um den der Vektor zu drehen ist.
        UT3D_vc_rotvcvcangr (pVec, &TSU_vrx, &TSU_vn, p1.x);
        // (*TSU_tr_3D_2D_pt)(&pm, &p1);  // nur zum Display, sonst unnutz !

      } else if(TSU_dat[6] < 0.) {   // Steigung; ev tBox[0] u tBox[2] ident !!!
        UT3D_pt_mid2pt (&p1, &tBox[0], &tBox[1]);
        UT3D_vc_rotvcvcangr (pVec, &TSU_vrx, &TSU_vn, p1.x);
        // (*TSU_tr_3D_2D_pt)(&pm, &p1);  // nur zum Display, sonst unnutz !

      } else if(TSU_dat[6] == 1.) {
        UT3D_pt_mid2pt (&p1, &tBox[2], &tBox[3]);
        UT3D_vc_rotvcvcangr (pVec, &TSU_vrx, &TSU_vn, p1.x);
        // (*TSU_tr_3D_2D_pt)(&pm, &p1);  // nur zum Display, sonst unnutz !

      } else {      // Paraboloid !
        (*TSU_tr_3D_2D_pt)(&p0, &tBox[0]);
        (*TSU_tr_3D_2D_pt)(&p1, &tBox[2]);
        UT3D_vc_2pt (&v1, &p0, &p1);
        ln1.p1 = TSU_prx;
        UT3D_pt_traptvc (&ln1.p2, &TSU_prx, &TSU_vrx);
        UT3D_pt_intptvcln (&pm, &p2, NULL, &p0, &v1, &ln1);
        UT3D_vc_2pt (pVec, &pm, &p2);
      }

      goto L_vec_OK;



    //------- SRV/TORUS ----------
    } else if(TSU_ox2.form == Typ_CI) {

      // Torus == Kugel; KonturcircleCenter = &((Circ*)TSU_ox2.data)->pc
      if(TSU_dat[5] == 0.) {   // KonturcircleCenter is on axis !
        UT3D_pt_mid2pt (&p1, &tBox[0], &tBox[2]);
        (*TSU_tr_3D_2D_pt)(&pm, &p1);
        // Vektor nach innen !
        UT3D_vc_2pt (pVec, &pm, &((Circ*)TSU_ox2.data)->pc);
        goto L_vec_OK;


      } else {     // echter Torus (keine Kugel)
        // Mittelpunkt der Konturkurve um den X-Parameterwert drehen
        UT3D_pt_mid2pt (&p1, &tBox[0], &tBox[2]);
        (*TSU_tr_3D_2D_pt)(&pm, &p1);
          // GR_Disp_pt (&pm, SYM_TRI_S, 1);

        UT3D_pt_rotptptvcangr (&p3, &((Circ*)TSU_ox2.data)->pc,
                               &TSU_prx, &TSU_vrx, p1.x);
          // GR_Disp_pt (&p3, SYM_TRI_S, 1);
        // Vektor nach innen !
        UT3D_vc_2pt (pVec, &pm, &p3);
          // GR_Disp_vc (pVec, &p2, 2, 0);

        goto L_vec_OK;
      }



    // } else if(TSU_ox2.form == Typ_CVBSP) { //------- SRV/BSPL ----------
        // Mittelpunkt Box in 2D
        // Normalvektor
        // UT3D_vc_perppta (pVec, c1Tab[0].iNr, p3Tab);
        // Punkt
        // // Tangente auf den Bspl im Y-Parameterwert
        // UT3D_vc_evalparCv (&vc2, (CurvBSpl*)TSU_ox2.data, p1.y);
        // // Tangente um den X-Parameterwert drehen
        // UT3D_vc_rotvcangr (pVec, p1.x, &vc2, &TSU_vrx);
        // Normalvektor auf den Bspl im Y-Parameterwert
        // goto L_vec_OK;
    }


  }


  //----------------------------------------------------------------
  // normal ..  (f Typ_SURBSP ..)
  L_vec_std:
    // printf(" TSU_box_vec L_vec_std:\n");
  // im 2D kann man nicht feststellen ob 2 Punkte ident sind !!!!
  (*TSU_tr_3D_2D_pt)(&p0, &tBox[0]);
  (*TSU_tr_3D_2D_pt)(&p1, &tBox[1]);
  (*TSU_tr_3D_2D_pt)(&p2, &tBox[2]);
    // GR_Disp_pt (&p0, SYM_TRI_S, 1); // GR_Disp_tx (&p2, "0", 1);
    // GR_Disp_pt (&p1, SYM_TRI_S, 2); // GR_Disp_tx (&p1, "1", 1);
    // GR_Disp_pt (&p2, SYM_TRI_S, 3); // GR_Disp_tx (&p2, "2", 1);



  // wenn 0-1 ident: use 2-3 instead
  if(UT3D_comp2pt(&p0, &p1, UT_TOL_pt) == 1) {
    // printf(" 0-1 ident: use 1-2-3\n");
    (*TSU_tr_3D_2D_pt)(&p3, &tBox[3]);
    UT3D_vc_perp3pt (pVec, &p1, &p2, &p3); // 1-2-3


  // wenn 1-2 ident: use 1-3 instead
  } else if(UT3D_comp2pt(&p1, &p2, UT_TOL_pt) == 1) {
    // printf(" 1-2 ident: use 0-2-3\n");
    (*TSU_tr_3D_2D_pt)(&p3, &tBox[3]);
    UT3D_vc_perp3pt (pVec, &p0, &p2, &p3); // 0-2-3


  // 0-1 1-2 OK
  } else {
    // printf(" use 0-1-2\n");
    UT3D_vc_perp3pt (pVec, &p0, &p1, &p2); // 0-1-2
  }

  // // manchmal zu kurz !!
  // if(UT3D_compvc0(pVec, 0.1) == 1) UT3D_vc_setLength(pVec, pVec, 1.);

/*
  // vc1 und vc3 zu einem MittelwertsVektor vc13 mischen
  UT3D_vc_merge2vc (&vc13, &vc1, &vc3);
*/



  // NUR TEST:

/*
  pm = p1;
  // pm = Mittelpunkt der Box
  UT3D_pt_mid2pt (&p1, &tBox[0], &tBox[2]);
  (*TSU_tr_3D_2D_pt)(&pm, &p1);

  (*TSU_tr_3D_2D_pt)(&p1, &tBox[0]);
  // (*TSU_tr_3D_2D_pt)(&p2, &tBox[1]);
  (*TSU_tr_3D_2D_pt)(&p3, &tBox[3]);

  UT3D_vc_perp3pt (pVec, &p1, &pm, &p3); // n innen.
*/



  //----------------------------------------------------------------
  L_vec_OK:


  // TESTBLOCK
  // UT3D_vc_invert (pVec, pVec); // NUR TEST wenn Probleme ..
  // GL_Disp_vc (&pm, pVec);
  // GR_Disp_vc (pVec, &pm, 9, 0);
    // UT3D_vc_invert (pVec, pVec);
    // UT3D_stru_dump (Typ_VC, pVec, "ex TSU_box_vec");
    // if(UT3D_compvc0(pVec, 0.1) == 1) exit(0);
  // TESTBLOCK
   


  return 0;

}


//================================================================
  int TSU_fix_ax_crx (int *ptNr,  Point *pTab) {
//================================================================
// Nur SURRV, Torus/Konus:

// Konus:
// beim Durchgang durch die Achse einen oder 2 Punkte einfuegen.
// Der zusaetzliche Punkte erhaelt X=UT_VAL_MAX

// Torus:
// beim Durchgang durch die Achse einen zweiten Punkt auf der
//   gegenueberliegenden Seite mit gleichem Parameter aber dem
//   opposite Winkel einfuegen.
// Oder Kontur immer beim Achsdurchgang beginnen/beenden?




  int    i1, i2, irc;
  double d1, tol;
  Point  pta[2];


  // printf(" TSU_fix_ax_crx %d %d\n",TSU_typ_,TSU_sStyl);



  if(TSU_typ_ != Typ_SURRV) return 0;

  // printf("TSU_fix_ax_crx %d %d\n",*ptNr,TSU_pintNr);
  // GR_Disp_pTab (*ptNr, pTab, SYM_TRI_S, 2);
  // if(TSU_pintNr > 0) GR_Disp_pt (&TSU_pint[0], SYM_TRI_B, 1);
  // if(TSU_pintNr > 1) GR_Disp_pt (&TSU_pint[1], SYM_TRI_B, 1);


  // for symbolic surfaces: do not insert pole-extraPoints
  if(TSU_sStyl > 0) return 0;



  if(TSU_pintNr < 1) return 0;




  //======= Line = Konus ===========================================
  // Konus: an Konusspitze ist immer ein Punkt; diesen Punkt als 
  //   Konturstartpunkt = Konturendpunkt setzen.
  // Kein Punkt wenn Konus planar waere !!!
  if(TSU_ox2.form != Typ_LN) goto L_torus;

  if(TSU_pintNr < 1) return 0;  // kein intersection-point (cyl ?)

  i2 = *ptNr - 1;   // skip last point !!
  for(i1=0; i1<i2; ++i1) {
    if(UT3D_comp2pt (&pTab[i1], TSU_pint, UT_TOL_cv) == 1) {   // 1=dist<tol

      // gleichen Punkt nochmals einfuegen ..
      // printf(" 3D-TopPt %d\n",i1);
      MEM_ins_nrec (ptNr, pTab, i1, &pTab[i1], 1, sizeof(Point));

      // diesen Punkt markieren
      // pTab[i1].x   = UT_VAL_MAX;

      // i1 soll der Startpunkt der Kontur sein !!
      // UT3D_cv_realg (i2, pTab, i1);
      // pTab[i2] = pTab[0]; // letzter == erster punkt !

      goto L_done;
    }
  }

  // nix ..
  return 0;




  //======= Circ = Torus ===========================================
  L_torus:
  if((TSU_ox2.form != Typ_CI)&&(TSU_ox2.form != Typ_CI)) goto L_err1;

  // Kugel:
  // Bei einem Durchgang einer Konturlinie durch den Pol muss unbedingt genau
  // der Pol als Punkt eingefuegt werden (nur am Pol kommt ein korrekter Y-Wert
  // damit die gesamte Flaeche abgedeckt wird).

  // Einfach testen ob ein Polygonpunkt ident ist mit einem Polpunkt
  // geht nicht, weil der Polpunkt auf einer Geraden zwischen 2 Polygonpunkten
  // liegen kann.
  // Man muss testen, ob ein Geradenstueck durch die Achse geht und somit durch
  // den Pol geht.

  // Ein Pol kann auf einer kontur nur einmal vorkommen; wenn einer gefunden
  // kann man Suche beenden.

  // ersten Durchstichpunkt TSU_pint[0] testen
  i1 = 1;            // index auf Punkt
  i2 = 0;            // index auf den intersection-point
  d1 = UT_VAL_MAX;   // Markierwert
  // tol = UT_DISP_cv * 1.5;  // findet Schnittpunkt bei hoher Modsiz nicht
  tol = UT_DISP_cv;


  L_tor_1:
  // printf(" i1=%d intPt=%d tol=%f\n",i1,i2,tol);

   // Polpunkt mit segment testen .. 
   // irc = UT3D_pt_ck_onLine (&pTab[i1-1], &pTab[i1], &TSU_pint[i2], tol);
   irc = UT3D_pt_ck_onLnSeg (&pTab[i1-1], &pTab[i1], &TSU_pint[i2], tol);

    // test Segmentstartpunkt
    if(irc == 0)  {           // po=p1
      // UT3D_stru_dump (Typ_PT, &TSU_pint[i2], "  >>>  irc=0 [%d]",i1);
      // insert 1 new point; 1. pt has X=UT_VAL_MAX; 2. point has coords !
      pTab[i1-1] = TSU_pint[i2];  // replace point
      MEM_ins_nrec (ptNr, pTab, i1, &TSU_pint[i2], 1, sizeof(Point));
      pTab[i1-1].x = d1;
      pTab[i1].x = d1;
      i1 += 2;
      goto L_tor_pol_nxt;


    } else if(irc == 2)  {   // po=p2
      // UT3D_stru_dump (Typ_PT, &TSU_pint[i2], "  >>>  irc=2 [%d]",i1);
      // insert 1 new point; 1. pt has X=UT_VAL_MAX; 2. point has coords !
      pTab[i1] = TSU_pint[i2];  // replace point
      pTab[i1].x = d1;
      MEM_ins_nrec (ptNr, pTab, i1, &TSU_pint[i2], 1, sizeof(Point));
      pTab[i1].x = d1;
      i1 += 2;  // + 2 sonst wird PT mit irc=0 gefunden !
      goto L_tor_pol_nxt;



    // befindet sich Polpunkt auf dem LineSegment ?
    } else if(irc == 1) {    // between p1-p2
      // UT3D_stru_dump (Typ_PT, &TSU_pint[i2], "  >>>  irc=1 [%d]",i1);
      // insert 2 new points; 1. pt has X=UT_VAL_MAX; 2. point has coords !
      pta[0] = TSU_pint[i2];
      pta[0].x = d1;
      pta[1] = pta[0];
      MEM_ins_nrec (ptNr, pTab, i1, pta, 2, sizeof(Point));
      i1 += 2; // sonst wird PT mit irc=0 gefunden !
      goto L_tor_pol_nxt;
    }

  L_tor_seg_nxt:
  ++i1; if(i1 < *ptNr) goto L_tor_1;  // do not skip last point !!


  // 2. Durchstichpunkt TSU_pint[1] testen
  L_tor_pol_nxt:
  if((TSU_pintNr > 1)&&(i2 == 0)) {
    i1 = 1;
    i2 = 1;            // index auf den intersection-point
    d1 = UT_VAL_MIN;   // Markierwert
    goto L_tor_1;
  }

  // fertig ..
  goto L_done;



  //======= Err1 ===========================================
  L_err1:
    TX_Error("TSU_fix_ax_crx E001");
    return -1;



  //================================================================
  L_done:
  // printf("ex TSU_fix_ax_crx %d\n",*ptNr);
  // for(i1=0; i1<*ptNr; ++i1) UT3D_stru_dump (Typ_PT, &pTab[i1], " p[%d]",i1);
  // GR_Disp_pTab (*ptNr, pTab, SYM_TRI_S, 2);
  // exit(0);


  return 0;

}

/*
//================================================================
  int TSU_delPt (int *ptNr, Point *pt2a) {
//================================================================
// OFFEN: sollte eigentlich nicht immer 3 benachbarte Punkte testen,
// sondern, wenn einer wegfaellt, alle zwischenliegenden testen !???

// Remove 2D-points, wenn die Abweichung < minTol
// ptStat = 0: Punkt wird gebraucht
// ptStat = 1: Punkt loeschen

  int     i1, iNr, ivon, inach;
  double  dl;
  char    ptStat[10000];   // max Anz Points pro Kontur ..
  Vector2 vc1;
  Point2  pts;

// statt ptStat koennte man pt2a[x].x auf VAL_MAX setzen und dann danach suchen 

  iNr = *ptNr;

  printf("TSU_delPt %d\n",iNr);


  for(i1=2; i1<iNr; ++i1) {

    // Normalabst siehe UT3D_pt_projpt2pt  // give 3D-Point for 2D-point !!!
    dl = fabs(UT2D_slen_nor3pt ((void*)&pt2a[i1-2],
                                (void*)&pt2a[i1-1],
                                (void*)&pt2a[i1]));

    // UT2D_vc_2pt (&vc1, &pt2a[i1-2], &pt2a[i1]);
    // UT2D_pt_projptptvc(&pts, &pt2a[i1-1], &pt2a[i1-2], &vc1);
    // dl = UT2D_len_2pt (&pt2a[i1-1], &pts);

    printf(" delPt[%d]=%f %f dl=%f\n",i1,pt2a[i1].x,pt2a[i1].y,dl);
    if(dl > UT_TOL_min1) {
      ptStat[i1-1] = 0;
      continue;
    } else {
      ptStat[i1-1] = 1;
    }
  }

  ptStat[iNr-1] = 0; // keep last ..


  ivon = 1;
  inach = 1;


  L_cpNxt:
  if(ivon >= iNr) goto L_fertig;
  if(ptStat[ivon] == 0)  {   // Punkt wird gebraucht
    // printf(" cp %d = %d\n",inach,ivon);
    if(inach != ivon) pt2a[inach] = pt2a[ivon];
    ++ivon;
    ++inach;
    goto L_cpNxt;

  } else {                 // Punkt nicht mehr used
    ++ivon;
    goto L_cpNxt;
  }




  L_fertig:
  *ptNr = inach;
  // printf("ex TSU_delPt %d\n",inach);
  // for(i1=0; i1<inach; ++i1) UT3D_stru_dump(Typ_PT2,&pt2a[i1],"P2[%d]=",i1);

  return 0;

}
*/


//================================================================
  int TSU_test_sbsp () {
//================================================================

  int     irc, gridMax, iu,iv;
  long    dbInd;
  double  du, dv;
  Point   p21, p31, *grid;
  SurBSpl *sbsp;
  ObjGX   oxi, oxpi, *oTab;
  Memspc  wrkSpc;





  // BSP-STUETZFlaeche A# holen
  dbInd = 13;
  oxi = DB_GetObjGX (Typ_SUR, dbInd);

  printf("TSU_test_sbsp %d %d %d\n",oxi.typ,oxi.form,oxi.siz);

  sbsp = oxi.data;

  // UT3D_stru_dump (Typ_SURBSP, sbsp, "");



  // Funktionen der TSU_tr_init_
/*

    // Init Speicherbereich fuer 3D-Gitterpunkte
    grid = (Point*) memspc54;
    gridMax = sizeof(memspc54) / sizeof(Point);


    // ein Gitternetz fuer ersten Start generieren
    printf("  ptu=%d ptv=%d\n",TSU_sbsp->ptUNr,TSU_sbsp->ptVNr);

    // die optimale Anzahl von Punkten in U- und V-Richtung ermitteln
    SUSbsp_ck_tol (&iu, &iv, sbsp);
    printf("  iu=%d iv=%d\n",iu,iv);


    // iu = sbsp->ptUNr;
    // iv = sbsp->ptVNr;

    // printf("  iu=%d iv=%d\n",iu,iv);
    if(iu*iv > gridMax) {TX_Error("TSU_tr_init_ E003"); return -1;}

    UME_init (&wrkSpc, memspc53, sizeof(memspc53));


    UT3D_ptgrid_sbsp (grid,&du,&dv,sbsp,iu,iv,&wrkSpc);
    printf("  du=%f dv=%f\n",du,dv);
*/


  if(TSU_tr_init_ (&oxi) < 0) return -1;




  // UME_init (&wrkSpc, memspc53, sizeof(memspc53));
  // irc = UT3D_parsbsp_pt (&p21.x, &p21.y, &p31,
                         // grid, du,dv, iu,iv, sbsp, &wrkSpc);





  //----- 3D-Punkte -> 2D-Punkte------------------------------
  // UT3D_pt_3db (&p31, -21.281300,26.952000,3.309900);
  UT3D_pt_3db (&p31, -19.767331,28.314988,3.477292);
  GR_Disp_pt (&p31, SYM_TRI_B, 1);


  irc = TSU_tr_2D_3D_sbsp (&p21, &p31);
  UT3D_stru_dump(Typ_PT, &p21, " p21:");
  return 0;
  p21.z=0.;
  p21.x=1.01575;
  GR_Disp_pt (&p21, SYM_STAR_S, 2);



  //----- 2D-Punkte -> 3D-Punkte------------------------------
  irc = TSU_tr_3D_2D_sbsp (&p31, &p21);
  GR_Disp_pt (&p31, SYM_TRI_B, 2);

  DL_Redraw ();
  return 0;

}


//================================================================
  int TSU_test_con (ObjGX *oxi) {
//================================================================

  int     i1;
  double  d1, d2;
  Point   p21, p31;
  Vector  vc1;
  ObjGX   *ox1, *ox2;
  Conus   *con;
  Mat_4x3 mat, imat;


  printf("TSU_test_con %d %d %d\n",oxi->typ,oxi->form,oxi->siz);

  ox1 = oxi->data; // Stuetzflaeche = Conus
  printf("     ox1 %d %d %d\n",ox1->typ,ox1->form,ox1->siz);

  UTO_obj_getp (&ox2, &i1, ox1);
  printf("     ox2 %d %d %d\n",ox2->typ,ox2->form,ox2->siz);

  con = ox2->data;
  printf(" CON: r1=%f r2=%f h=%f\n",con->r1,con->r2,con->h);


  // Steigung der Mantellinie k = (r1 - r2) / h; (Konusachse == X-Achse)
  d1 = (con->r2 - con->r1) / con->h;
  printf(" Steigung=%f\n",d1);


  // h-Wert der Spitze ist -d/k;  d ist dabei con->r1.
  d2 = -con->r1 / d1; 
  printf(" h-Wert Spitze=%f\n",d2);


  // fix Matrix zur Tansformation in Nulllage
  UT3D_m3_loadpl (mat, &con->pl);
  // UT3D_stru_dump (Typ_M4x3,  mat, "              _mat:\n");
  UT3D_m3_invm3 (imat, mat);
  // UT3D_stru_dump (Typ_M4x3, imat, "              imat:\n");

  // einen Punkt des Konus -> Nullage transferieren
  UT3D_pt_traptvclen (&p31, &con->pl.po, &con->pl.vz, d2);
  UT3D_pt_traptm3 (&p21, imat, &p31);
  UT3D_stru_dump(Typ_PT, &p21, "Spitze:");





  //----- 3D-Punkte -> 2D-Punkte------------------------------
  UT3D_pt_3db (&p31, 200., 100., 0.);
  UT3D_pt_3db (&p31, 100., 200., 0.);
  UT3D_parscon_pt (&p21.x, &p21.y, &p31, imat);  p21.z=0.;
  GR_Disp_pt (&p21, SYM_STAR_S, 2);

  UT3D_pt_3db (&p31, 150., 100., 100.);
  UT3D_pt_3db (&p31, 100., 150., 100.);
  UT3D_parscon_pt (&p21.x, &p21.y, &p31, imat);  p21.z=0.;
  GR_Disp_pt (&p21, SYM_STAR_S, 2);

  return 0;


  //----- 2D-Punkte -> 3D-Punkte------------------------------
  UT3D_pt_evparscon (&p31, con, 0., RAD_90, mat);
  GR_Disp_pt (&p31, SYM_TRI_S, 2);

  UT3D_pt_evparscon (&p31, con, con->h, RAD_90, mat);
  GR_Disp_pt (&p31, SYM_TRI_S, 2);



  return 0;

}


//================================================================
  int TSU_test_tor (ObjGX *oxi) {
//================================================================

  int     i1;
  double  d1, d2;
  Point   p21, p31;
  Vector  vc1;
  ObjGX   *ox1, *ox2;
  Torus   *tor;
  Mat_4x3 mat, imat;


  printf("TSU_test_tor %d %d %d\n",oxi->typ,oxi->form,oxi->siz);

  ox1 = oxi->data; // Stuetzflaeche = Torus
  printf("     ox1 %d %d %d\n",ox1->typ,ox1->form,ox1->siz);

  UTO_obj_getp (&ox2, &i1, ox1);
  printf("     ox2 %d %d %d\n",ox2->typ,ox2->form,ox2->siz);

  tor = ox2->data;
  printf(" TOR: r1=%f r2=%f\n",tor->r1,tor->r2);


  // fix Matrix zur Tansformation in Nulllage
  UT3D_m3_loadpl (mat, &tor->pl);
  // UT3D_stru_dump (Typ_M4x3,  mat, "              _mat:\n");
  UT3D_m3_invm3 (imat, mat);
  // UT3D_stru_dump (Typ_M4x3, imat, "              imat:\n");



  //----- 3D-Punkte -> 2D-Punkte------------------------------
  // UT3D_pt_3db (&p31, 200., 100., 100.);
  UT3D_pt_3db (&p31, 180., 100., 110.);
  UT3D_parstor_pt (&p21.x, &p21.y, &p31, tor, imat);  p21.z=0.;
  GR_Disp_pt (&p21, SYM_STAR_S, 2);



  //----- 2D-Punkte -> 3D-Punkte------------------------------
  UT3D_pt_evparstor (&p31, tor, p21.x, p21.y, mat);
  GR_Disp_pt (&p31, SYM_TRI_S, 2);

  // UT3D_pt_evparscon (&p31, con, 0., RAD_90, mat);
  // GR_Disp_pt (&p31, SYM_TRI_S, 2);

  // UT3D_pt_evparscon (&p31, con, con->h, RAD_90, mat);
  // GR_Disp_pt (&p31, SYM_TRI_S, 2);


  return 0;

}


//=========================================================
  int TSU_test_srv () {
//=========================================================
// TEST eine Transformationsfunktion;
// rufen via Func. AP_test__
// siehe TSU_DrawSurTRV ..
/*  Testsequenz fuer tst1.c:
  {
    SurRev *srv1;
    ObjGX  TSU_ox2, TSU_ox1;
    Point   TSU_prx;       // rotAxis
    Vector  TSU_vrx;       // rotAxis

    // Flaeche A# holen (SRV- nicht FSUB-Flaeche!)
    dbInd = 20;
    ox1 = DB_GetObjGX (Typ_SUR, dbInd);
    // init
    TSU_tr_init_ (&ox1);
    srv1 = ox1.data;
    TSU_ox1 = DB_GetObjGX ((int)srv1->typCen, srv1->indCen);
    TSU_prx = ((Line*)TSU_ox1.data)->p1;
    UT3D_vc_ln (&TSU_vrx, (Line*)TSU_ox1.data);
    UT3D_vc_setLength (&TSU_vrx, &TSU_vrx, 1.);
    GR_Disp_vc (&TSU_vrx, &TSU_prx, 2, 0);
    TSU_ox2 = DB_GetObjGX ((int)srv1->typCov, srv1->indCov);

    ....
  {
*/



  int     irc;
  long    dbInd;
  Point   p21, p31;
  ObjGX   oxi, *ox1, *ox2;


  // Flaeche A# holen (SRV- nicht FSUB-Flaeche!)
  dbInd = 20;
  oxi = DB_GetObjGX (Typ_SUR, dbInd);

  printf("TSU_test_srv %d %d %d\n",oxi.typ,oxi.form,oxi.siz);


  // init
  irc = TSU_tr_init_ (&oxi);
  if(irc < 0) return irc;


  printf("=========================== \n");

  //----- 3D-Punkte -> 2D-Punkte------------------------------
  UT3D_pt_3db (&p31, 83.983,   75.000,   43.093);
  GR_Disp_pt (&p31, SYM_TRI_S, 2);
  TSU_tr_2D_3D_srv (&p21, &p31);
  // GR_Disp_pt (&p21, SYM_TRI_S, 2);
  // printf("=========================== \n");
  // return 0;


  //----- 2D-Punkte -> 3D-Punkte------------------------------
  // UT3D_pt_3db (&p21, 0., 0., 0.);
  TSU_tr_3D_2D_srv (&p31, &p21);
  GR_Disp_pt (&p31, SYM_TRI_S, 2);


  return 0;

}

 
//=========================================================
  int TSU_test_sru () {
//=========================================================
// TEST eine Transformationsfunktion;
// rufen via Func. AP_test__

  int     typ1, typ2, i1;
  long    dbInd;
  Point   p21, p31;
  ObjGX   *oTab, *oxp1, oxi;
  void    *ox1, *ox2;


  // Flaeche A20 holen (SRU- nicht FSUB-Flaeche!)
  dbInd = 8;
  oxi = DB_GetObjGX (Typ_SUR, dbInd);

  printf("TSU_test_sru %d %d %d\n",oxi.typ,oxi.form,oxi.siz);

  TSU_tr_init_ (&oxi);

  oTab = oxi.data;

  // SRU:
  typ1 = UTO_obj_getp (&ox1, &i1, &oTab[0]);
  typ2 = UTO_obj_getp (&ox2, &i1, &oTab[1]);
  // UT3D_stru_dump(typ1, ox1, "o1-----:");
  // UT3D_stru_dump(typ2, ox2, "o2-----:");




  //----- 3D-Punkte -> 2D-Punkte------------------------------
  UT3D_pt_3db (&p31,  21.282,   26.952,    3.310);
  // UT3D_pt_3db (&p31,  32.611,  -11.174,   -1.372);
  // UT3D_pt_3db (&p31,  0.000,  -34.243,   -4.205);
  // UT3D_pt_3db (&p31,   0.000,  -30.775,  -13.099);
  // UT3D_pt_3db (&p31,  25.771,  -12.566,  -20.182);
  // UT3D_pt_3db (&p31,  23.261,   20.718,  -16.095);
  UT3D_pt_3db (&p31,  -18.739,  -20.723,  -21.184);

  GR_Disp_pt (&p31, SYM_STAR_S, 2);

  // UT3D_pt_3db (&p31, 50.190,   19.922,   50.000); //.r1:1,1
  UT3D_parsru_pt (&p21.x, &p21.y, &p31);  p21.z=0.;
  GR_Disp_pt (&p21, SYM_STAR_S, 2);

  // return 0;
  // 0.634,    0.069,    0.000


  //----- 2D-Punkte -> 3D-Punkte------------------------------
  // UT3D_pt_3db (&p21, 0., 0., 0.);
  // p21.x = 0.8; p21.y = 0.;
  UT3D_pt_evparsru (&p31, p21.x, p21.y);
  GR_Disp_pt (&p31, SYM_TRI_S, 0);

  // p21.x = 0.8; p21.y = 1.;
  // UT3D_pt_evparsru (&p31, p21.x, p21.y);
  // GR_Disp_pt (&p31, SYM_TRI_S, 1);


  return 0;

}


//================================================================
  int TSU_srv_tor_3 () {
//================================================================
// NUR SRV-Torus, wenn Drehachse ausserhalb Torus ist

// allgemeiner Torus: Achse umdrehen (problem mit falschen Normalvektor)
// ein Achsensystem aus vz und pc-pa errichten;
// testen ob vec parallel od antiparall.
// fuer Tori mit Achse im Torus: TSU_srv_tor_2.


  int    i1;
  double d1;
  Point  pt1;
  Vector vc1, vc2;

/*
  UT3D_pt_projptptvc(&pt1,&d1,&((Circ*)TSU_ox2.data)->pc,&TSU_prx,&TSU_vrx);

  UT3D_vc_2pt (&vc1, &TSU_ac1.pc, &pt1);
  UT3D_stru_dump (Typ_VC, &vc1, "vc1");
  UT3D_vc_perp2vc (&vc2, &vc1, &((Circ*)TSU_ox2.data)->vz);
  UT3D_stru_dump (Typ_VC, &vc2, "vc2");
  // test ob gleiche Richtung ..
  d1 = UT3D_acos_2vc (&TSU_vrx, &vc2);
  UT3D_stru_dump (Typ_VC, &TSU_vrx, " acos vrx = %f",d1);
  if(d1 >= 0.) { // Umdrehen nur merken, am Ende alle Konturen umdrehen.
    TSU_dir = 1; // nicht umdrehen
  } else {
    TSU_dir = -1;
  }
  printf(" TSU_dir=%d CircTyp=%f\n",TSU_dir,TSU_dat[5]);
*/

  // UT3D_ci_inv1 (&TSU_ac1);  // bewirkt ebenfalls Richtungsumkehr

  // Circ-Z-Achse und Torushauptachse gleiche Richtung: OK; else umdrehen
  // ev bei Circ=CW nochmals umdrehen


  // CircCenter auf Achse projiz.
  UT3D_pt_projptptvc(&pt1,&d1,NULL, &TSU_ac1.pc,&TSU_prx,&TSU_vrx);
    // GR_Disp_pt(&pt1, SYM_TRI_B, 2);
  UT3D_vc_2pt (&vc1, &TSU_ac1.pc, &pt1);
    // UT3D_stru_dump (Typ_VC, &vc1, "vc1");
  UT3D_vc_perp2vc (&vc2, &vc1, &TSU_vrx);
    // GR_Disp_vc (&vc2, &TSU_ac1.p1, 2, 0);
    // GR_Disp_vc (&TSU_ac1.vz, &TSU_ac1.pc, 2, 0);

  // sind vc2-CircVz eher parall od eher antiparall
  i1 = UT3D_sid_2vc (&vc2, &TSU_ac1.vz);
  // printf(" sid=%d\n",i1);

  if(i1 >= 0) { // Umdrehen nur merken, am Ende alle Konturen umdrehen.
    TSU_dir = 1; // nicht umdrehen
  } else {
    TSU_dir = -1;
  }

    // printf("ex TSU_srv_tor_3 TSU_dir=%d\n",TSU_dir);


  return 0;

}



//================================================================
  int TSU_srv_con_0 (int pNr, Point *pTab) {
//================================================================
// wenn Line groesser als Modelsize 
// von allen 3D-Punkte eine Box machen und die KonturLine in diese Box pressen.
// Normalvektor TSU_vn auf Konturline errechnen

  double  d1;
  Point   pb1, pb2, pt1, pt2;
  Vector  vc1;


  // printf("TSU_srv_con_0 %f %f\n",TSU_dat[6],TSU_dat[7]);


  // kleiner als ModelSize
  if(TSU_dat[7] < 0.5) goto L_nVec;


  // 3D-Box machen
  UT3D_box_pts (&pb1, &pb2, pNr, pTab);
  // GR_Disp_pt(&pb1, SYM_TRI_B, 2);
  // GR_Disp_pt(&pb2, SYM_TRI_B, 2);


  // Box aufs doppelte vergroessern
  UT3D_pt_mid2pt (&pt1, &pb1, &pb2);
  UT3D_vc_2pt (&vc1, &pt1, &pb1);
  UT3D_pt_traptvc (&pb1, &pb1, &vc1);
  UT3D_vc_2pt (&vc1, &pt1, &pb2);
  UT3D_pt_traptvc (&pb2, &pb2, &vc1);



  // die Box um die Drehachse spiegeln = Box vergroessern
  UT3D_pt_oppptptvc (&pt1, &pb1, &TSU_prx, &TSU_vrx);
  UT3D_box_extend (&pb1, &pb2, &pt1);
  UT3D_pt_oppptptvc (&pt1, &pb2, &TSU_prx, &TSU_vrx);
  UT3D_box_extend (&pb1, &pb2, &pt1);
  // GR_Disp_pt(&pb1, SYM_TRI_B, 2);
  // GR_Disp_pt(&pb2, SYM_TRI_B, 2);
  // GR_Disp_box (&pb1, &pb2, 2);

  // KonturLine in dieser Box begrenzen
  UT3D_ln_intbox (&TSU_ln1, &pb1, &pb2);
  // GR_Disp_pt(&TSU_ln1.p1, SYM_TRI_B, 2);
  // GR_Disp_pt(&TSU_ln1.p2, SYM_TRI_B, 2);
  // GR_Disp_ln (&TSU_ln1, 2);
  // UT3D_stru_dump (Typ_LN, &TSU_ln1, "TSU_ln1");


  
  // test, ob Cylind od Paraboloid ?
  // (Paraboloid: das Errechnen des NormalVektors anders machen)
  if(TSU_dat[6] != 0.) goto L_nVec;


  // Oeffnungswinkel Achse / Konturline
  UT3D_vc_2pt (&vc1, &TSU_ln1.p1, &TSU_ln1.p2);
  UT3D_vc_setLength (&vc1, &vc1, 1.);
  d1 = fabs(ACOS(UT3D_acos_2vc (&TSU_vrx, &vc1)));
  if((d1 > UT_TOL_PAR)&&(d1 < RAD_180-UT_TOL_PAR)) TSU_dat[6] = 2.; // Parab.!!
  // printf(" typ=%f oAng=%f\n",TSU_dat[6],d1);




  //================================================================
  // TSU_vn =  Normalvektor auf den cone setzen
  L_nVec:

    UT3D_pt_mid2pt (&pt1, &TSU_ln1.p1,&TSU_ln1.p2); // Mittelpkt Konturlinie
      // GR_Disp_pt(&pt1, SYM_TRI_B, 2);
    UT3D_vc_2pt (&vc1, &TSU_ln1.p1,&TSU_ln1.p2);    // vec <- Konturlinie

    // Schnitt einer Normalebene auf Konturlinie mit Achse
    UT3D_pt_intperp2ln (&pt2, &pt1, &vc1, &TSU_prx, &TSU_vrx);
      // GR_Disp_pt(&pt2, SYM_TRI_B, 2);
    UT3D_vc_2pt (&TSU_vn, &pt1, &pt2);
      // GR_Disp_vc (&TSU_vn, &pt1, 2, 0);




  return 0;

}


//================================================================
  int TSU_srv_con_inside (ContTab *cTab, int cNr) {
//================================================================
 
  int    i1, i2, i3;
  double d0, d1;
  Point  *pTab, *p0Tab;

  // check if Startpoint is inside outerContour; NO: move complete contour.
  for(i1=1; i1<cNr; ++i1) {

    // automat.generierte Kontur darf nicht korrigiert werden
    if(cTab[i1].typ == 'A') continue;

    pTab = cTab[i1].p2a;

    p0Tab = cTab[0].p2a;
    i2 = UT2D_pt_ck_inCv3 (pTab, cTab[0].iNr, p0Tab);
    if(i2 < 0) {   // ausserhalb
      d0 = pTab[0].x - p0Tab[0].x;
      if(d0 < 0.) d1 =  RAD_360;
      else        d1 = -RAD_360;
      for(i3=0;i3<cTab[i1].iNr; ++i3) {  // alle Punkte verschieben
        pTab[i3].x += d1;
      }
    }
  }

  return 0;

}


//================================================================
  int TSU_srv_con_top (Point *pTab, int pNr) {
//================================================================
// Fuer die Polpunkte die X-Werte korrigieren.
// In der pTab hat ein Polpunkt immer  2 x hintereinander x=UT_VAL_MAX.
// Ersetzen durch den vorhergehenden und nachfolgenden X-Wert.
// (die 2 Polpunkte wurden von TSU_fix_ax_crx eingefuegt).

  int    i1, i2;



  // printf("XXXXXXXXXXXXX TSU_srv_con_top %d \n",pNr);
  // for(i1=0; i1<pNr; ++i1) printf(" %d %f %f\n",i1,pTab[i1].x,pTab[i1].y);


    // Variante erster == letzter Punkt sind ein Pol:
  if(pTab[0].x == UT_VAL_MAX) {

    if(pTab[pNr-1].x == UT_VAL_MAX) {
      pTab[0].x = pTab[1].x;
      pTab[pNr-1].x = pTab[pNr-2].x;
      // printf("  fix top0: 0 & %d\n",pNr-1);
    }


    // Variante 1. & 2. Punkt:
    if(pTab[1].x == UT_VAL_MAX) {
      pTab[0].x = pTab[pNr-1].x;
      pTab[1].x = pTab[2].x;
      // printf(" fix top1: 0 & 1\n");
    }
  }


    // Variante vorletzten und letzter Punkt:
  if(pTab[pNr-1].x == UT_VAL_MAX) {
    if(pTab[pNr-2].x == UT_VAL_MAX) {
      pTab[pNr-2].x = pTab[pNr-3].x;
      pTab[pNr-1].x = pTab[0].x;
      // printf(" fix top2: %d & %d\n",pNr-2,pNr-1);
    }
  }


  L_korr_top_2:

    for(i2=1; i2<pNr-1; ++i2) {
      if(pTab[i2].x != UT_VAL_MAX) continue;
      pTab[i2].x = pTab[i2-1].x;
      pTab[i2+1].x = pTab[i2+2].x;
      // printf(" fix top3: %d & %d\n",i2,i2+2);
    }



  return 0;

}

  
//================================================================
  int TSU_srv_con_cycX2_ (ContTab *cTab, double v0) {
//================================================================
// set startpoint of x-values of pTab to value vo

  int    i1, i2, ii, iNr, iDir;
  double d1, d2, x1, y1, dOff;
  Point  *pTab;
  Point  *tTab;


  pTab = cTab->p2a;
  iNr  = cTab->iNr;
  ii = 0;


  // 2 points: must be horiz !
  if(iNr == 2) {
    if(pTab[0].x < pTab[1].x) {
      d1 = RAD_360;
    } else {
      d1 = -RAD_360;
    }
    pTab[0].x = v0;
    pTab[1].x = v0 + d1;
    return 0;
  }


  L_sta0:
  // printf("TSU_srv_con_cycX2_ %d %lf\n",iNr,v0);
  // for(i1=0; i1<iNr; ++i1)  printf(" [%d] %lf %lf\n",i1,pTab[i1].x,pTab[i1].y);
  if(ii > 3) return -1; // exit(0);


  if(pTab[0].x < pTab[iNr-1].x) {
    // ascending values
      // printf(" asc.vals\n");
    iDir = 1;
  } else {
    // decending values
      // printf(" desc.vals\n");
    iDir = -1;
  }


  // test is v0 is in range of x-values
  if(!UTN_LIMCK__(v0,pTab[0].x,pTab[iNr-1].x)) goto L_sta1;
  // move range 
  if(iDir > 0) {
    // ascending values
    if(v0 > pTab[iNr-1].x) d1 = RAD_360;
    else                   d1 = -RAD_360;
  } else {
    // decending values
    if(v0 < pTab[0].x) d1 = -RAD_360;
    else               d1 = RAD_360;
  }
    // printf(" add tab d1=%lf\n",d1);
  for(i1=0; i1<iNr; ++i1)  pTab[i1].x += d1;
// exit(0);
  ++ii;
  goto L_sta0;


  // test if difference is a multiple of RAD_360;
  L_sta1:
    // printf(" L_sta1: (v0 in range ..)\n");
  d1 = pTab[0].x - v0;
  d2 = fmod(d1,RAD_360);
    // printf(" d1=%lf fmod=%lf v0=%lf\n",d1,d2,v0);
  if(fabs(d2) < UT_TOL_Ang1) {
    // distance v0 - P0 is exact n * 360 deg 
      // printf(" move exact %lf\n",d1);
    // add the differenc.
    for(i1=0; i1<iNr; ++i1)  pTab[i1].x -= d1;
    goto L_copy2;
  }

/*
  // test if v0 is inside x-range
  // if not: fix v-space
  if(!UTN_LIMCK__(v0,pTab[0].x,pTab[iNr-1].x)) goto L_find;
  if(v0 > pTab[iNr-1].x) d1 = RAD_360;
  else                   d1 = -RAD_360;

    printf(" add all points d1=%lf\n",d1);
  for(i1=0; i1<iNr; ++i1) pTab[i1].x += d1;
    printf(" ps.x=%lf pe.x=%lf\n",pTab[0].x,pTab[iNr-1].x);
  goto L_sta1;
*/


  // find v0 in x-values
  L_find:
    // printf(" L_find:\n");

  // get tempSpace
  tTab = (Point*)MEM_alloc_tmp (sizeof(Point) * (iNr+3));
  if(!tTab) {TX_Error("TSU_srv_con_cycX2_ EOM"); return -1;}

  for(i1=1; i1<iNr; ++i1) {
    if(UTN_LIMCK__(v0,pTab[i1 - 1].x,pTab[i1].x)) continue;
    if(UTP_comp2db(v0,pTab[i1 - 1].x,UT_TOL_Ang1)) {
      i2 = i1 - 1;  // index to start with ..
      goto L_copy0;
    }
    if(UTP_comp2db(v0,pTab[i1].x,UT_TOL_Ang1)) {
      i2 = i1;  // index to start with ..
      goto L_copy0;
    }
      // printf(" p[%d].x=%lf  %lf\n",i1-1,pTab[i1-1].x,pTab[i1-1].y);
      // printf(" p[%d].x=%lf  %lf\n",i1,pTab[i1].x,pTab[i1].y);

    // additional point; interpolate the new y-value
    x1 = v0;
    d1 = ((v0 - pTab[i1-1].x) / (pTab[i1].x - pTab[i1-1].x));
    y1 = pTab[i1-1].y + ((pTab[i1].y - pTab[i1-1].y) * d1);
      // printf(" d1=%lf x1=%lf y1=%lf\n",d1,x1,y1);
     
    // new pt = startpt

    // replace point [i1-1] ; start with this point.
    tTab[0].x = x1;
    tTab[0].y = y1;

    ii = 1;
    for(i2=i1; i2<iNr; ++i2) {
        // printf(" copy1 %d -> %d\n",i2,ii);
      tTab[ii] = pTab[i2];
      ++ii;
    }
    // skip 1. point (should be the same as the last point)
    if(iDir > 0) d1 = RAD_360;
    else         d1 = -RAD_360;
    for(i2=1; i2<=i1; ++i2) {
        // printf(" copy2 %d -> %d; add %lf\n",i2,ii,d1);
      tTab[ii] = pTab[i2];
      tTab[ii].x += d1;
      ++ii;
    }

    // but get last point again ..
    tTab[ii] = tTab[0];
    tTab[ii].x += d1;
    ++ii;
      // printf(" nr of points ii now %d\n",ii);

    // save new nr of points
    iNr = ii;
    cTab->iNr = ii;

    goto L_copy1;
  }


  // v0 not in v-space
  printf("****** TSU_srv_con_cycX2_ E001\n");
// exit(0);
  return -1;



  //----------------------------------------------------------------
  L_copy0:
  // start with i2; copy i2-iNr; then copy 0-(i2-1)
    // printf(" L_copy0 %d\n",i2);


  ii = 0;
  for(i1=i2; i1<iNr; ++i1) {
    tTab[ii] = pTab[i1];
    ++ii;
  }
  // skip 1. point (should be the same as the last point)
  // but get last point again ..
  // for(i1=1; i1<=i2; ++i1) {
  if(iDir > 0) d1 = RAD_360;
  else         d1 = -RAD_360;
  for(i1=1; i1<=i2; ++i1) {
    tTab[ii] = pTab[i1];
    tTab[ii].x += d1;
    ++ii;
  }
    // printf(" ii=%d\n",ii);

    // for(i1=0; i1<iNr; ++i1)  printf(" t[%d].x=%f\n",i1,tTab[i1].x);


  //----------------------------------------------------------------
  // copy back ..
  L_copy1:
  memcpy (pTab, tTab, sizeof(Point) * iNr);



  // // make direction of x-values consecutive (necess. after 
  // TSU_srv_con_cycX1_ (pTab, iNr);

  L_copy2:


    // TESTDISPLAY
    // printf("ex TSU_srv_con_cycX2_ %d %lf\n",iNr,v0);
    // for(i1=0; i1<iNr; ++i1) printf(" [%d] %lf %lf\n",i1,pTab[i1].x,pTab[i1].y);
// exit(0);


  return 0;

}


//================================================================
  int TSU_srv_con_xMid (double *dx, Point *pTab, int iNr) {
//================================================================
/// get medium x-val of boundary

  int    i1;
  double d1;
  
  
  // printf("TSU_srv_con_mov_x %d\n",iNr);
  // for(i1=0; i1<iNr; ++i1) printf(" [%d] %lf %lf\n",i1,pTab[i1].x,pTab[i1].y);
  
  d1 = 0.;
  for(i1=0; i1<iNr; ++i1) d1 += pTab[i1].x;
  *dx = d1 / iNr;
    // printf(" ex TSU_srv_con_xMid %lf\n",*dx);
  return 0;
 
}


//================================================================
  int TSU_srv_con_mov_x (Point *pTab, int iNr, double dd) {
//================================================================
// move whole boundary in x

  int    i1;


  // printf("TSU_srv_con_mov_x %d\n",iNr);
  // for(i1=0; i1<iNr; ++i1) printf(" [%d] %lf %lf\n",i1,pTab[i1].x,pTab[i1].y);

  for(i1=0; i1<iNr; ++i1) pTab[i1].x += dd;


  return 0;

}

 
//================================================================
  int TSU_srv_con_cycX1_ (Point *pTab, int iNr) {
//================================================================
// remove 360-deg-gaps.
// 1.step: find direction; up or down (ignore gaps > 360)
// 2.step: find gaps >360; if ..

  int      i1, i2, ii, iDir;
  double   d1, d2;


  // printf("TSU_srv_con_cycX1_ %d\n",iNr);
  // for(i1=0; i1<iNr; ++i1) printf(" [%d] %lf %lf\n",i1,pTab[i1].x,pTab[i1].y);


  // find direction iDir
  ii = 0;
  for(i1=1; i1<iNr; ++i1) {
    d1 = pTab[i1].x - pTab[i1 - 1].x;
      // printf(" d[%d-%d] = % lf\n",i1-1,i1,d1);
    // if(fabs(d1) > RAD_180) continue;
    if(fabs(d1) > 4.) continue;
    if(d1 > 0.) ++ii;
    else        --ii;
  }
  iDir = ISIGN (ii);
    // printf(" iDir = %d ii = %d\n",iDir,ii);


ii=0;
  L_noAmoi:
if(ii>10) {printf("***** TSU_srv_con_cycX1_ E001 \n"); return -1;}

  for(i1=1; i1<iNr; ++i1) {
    // find gap > 180 deg
    d1 = pTab[i1].x - pTab[i1 - 1].x;
    // if(fabs(d1) < RAD_180) continue;
    if(fabs(d1) < 4.) continue;
      // printf(" diff %d - %d = %lf\n",i1-1,i1,d1);
    if(d1 > 0.) d2 = -RAD_360;
    else        d2 = RAD_360;
      // printf(" from i1=%d add d2=%lf\n",i1,d2);

    for(i2=i1; i2<iNr; ++i2) {
      pTab[i2].x += d2;
    }
++ii;
    goto L_noAmoi;
  }


  // printf("ex TSU_srv_con_cycX1_:\n");
  // for(i1=0; i1<iNr; ++i1)  printf(" [%d].x=%f\n",i1,pTab[i1].x);

  return 0;

}


//================================================================
  int TSU_srv_con_cycX__ (Point *pTab, int iNr) {
//================================================================
// funktioniert nicht mit Modell Err/Tess_sur_sru7 !!!!!!!!!!
// nur eine SpaltDifferenz von dTot korrigieren !

// Nach einem gap von duTot muss der gesamte Pfad umgedreht werden bis zum
// Ruecksprung (hier ist wieder ein gap von duTot)

// Ev. sollte man ebenfalls an einer Stelle beginnen, wo
// minimales Increment ist.




  int       i1, i2, ig1, ig2, iDir, iLoop;
  double    dxTol, dx, dxTot, uMin, uMax;


  uMin = TSU_dat[0];
  uMax = TSU_dat[1];
  dxTot = uMax - uMin;


  // printf("TSU_srv_con_cycX__ %d min=%f max=%f\n",iNr,uMin,uMax);
  // for(i1=0; i1<iNr; ++i1)  printf(" [%d].x=%f\n",i1,pTab[i1].x);
  // for(i1=0; i1<iNr; ++i1) if(pTab[i1].x < 0.1) pTab[i1].x = 1.0;
  // return 0;

  // dxTol = dxTot / 16;
  dxTol = dxTot / 3;
    // printf(" dxTot=%f dxTol=%f\n",dxTot,dxTol);

  iLoop = 0;


  L_start:
  // if(iLoop > 1) {TX_Error("TSU_srv_con_cycX__ E001"); return 0;}


  i2 = 0;
  for(i1=1; i1<iNr; ++i1) {

    dx = fabs(pTab[i1].x - pTab[i2].x) + dxTol;
      // printf(" [%d].x=%f dx=%f\n",i1,pTab[i1].x,dx);
    if(dx > dxTot) {
      if(pTab[i1].x < pTab[i2].x) iDir = 1;
      else                        iDir = -1;
      ig1 = i1;
        // printf(" ig1=%d\n",ig1);
      goto L_ig2;
    }
    ++i2;
  }
  // TX_Print("TSU_srv_con_cycX__ I001");
  goto L_exit;


  L_ig2:

  i2 = ig1;
  for(i1=ig1+1; i1<iNr; ++i1) {

    dx = fabs(pTab[i1].x - pTab[i2].x) + dxTol;
    // printf(" [%d].x=%f dx=%f\n",i1,pTab[i1].x,dx);
    if(dx > dxTot) {
      ig2 = i1;
      // printf(" ig2=%d\n",ig2);
      goto L_ig3;
    }
    ++i2;
  }
  // TX_Print("TSU_srv_con_cycX__ I002");
  goto L_exit;



  // hier wird x-Wert zwischen ig1 - ig2 umgedreht.
  L_ig3:

  for(i1=ig1; i1<ig2; ++i1) {

    if(iDir > 0) {
      dx = pTab[i1].x - uMin;
      pTab[i1].x = uMax - dx;
    } else {
      dx = uMax - pTab[i1].x;
      pTab[i1].x = dx + uMin;
    }
    ++i2;
  }


  // wenn ein Vollkreis ist, koennen 2 durchlaeufe notwendig sein !
  ++iLoop;
  if(iLoop < 2) goto L_start;


  L_exit:
  // for(i1=0; i1<iNr; ++i1)  printf(" [%d].x=%f\n",i1,pTab[i1].x);
  // exit(0);

  return 0;

}


//================================================================
  int TSU_sph_cyclic (Point *pTab, int iNr) {
//================================================================
// Sphere.
// X-Werte bei einem 360-Grad-Sprung korrigieren.
// Problem ist, wo es eine Kreuzung oder eine Selbstueberdeckung der
// 2D-Konturelemente gibt.

// Wenn der Pol innerhalb der Flaeche liegt, funktioniert das alles nicht.

// Einen Spalt suchen, der mind 300 Grad grosz ist.
// Einen zweiten Spalt suchen, der mind 150 Grad gross ist.
// Alle Punkte zwischen den Spalten um 360 Grad verschieben.
// Wenn Spalt1 grosser 0 ist: um -360 verschieben;
// Wenn Spalt1 kleiner 0 ist: um +360 verschieben.



static double gap1 = RAD_180 - RAD_10; // war 360-60
static double gap2 = RAD_180 - RAD_10;

  int       i1, i2, is, ie, iLoop;
  double    d0, dd, da;


  // uMin = TSU_dat[0];
  // uMax = TSU_dat[1];
  // dxTot = uMax - uMin;


  // printf("TSU_sph_cyclic %d min=%f max=%f\n",iNr,uMin,uMax);
  // for(i1=0; i1<iNr; ++i1)  printf(" [%d].x=%f,%f\n",i1,pTab[i1].x,pTab[i1].y);
  // return 0;
  // printf(" gap1=%f gap2=%f\n",gap1,gap2);

  iLoop = 0;

  L_start:

  // Einen Spalt suchen, der mind 170 Grad grosz ist.
  for(i1=1; i1<iNr; ++i1) {
    d0 = pTab[i1].x-pTab[i1-1].x;
    // printf(" d0=%f dd=%f\n",d0,dd);
    if(d0 > gap1) {
      is = i1; da = -RAD_360; goto L_ck1;
    } else if(d0 < -gap1) {
      is = i1; da = RAD_360; goto L_ck1;
    }
  }
  return 0;


  L_ck1:     // ab incl. is verschieben.
  // printf(" Loop %d: is=%d da=%f\n",iLoop,is,da);


  // Einen zweiten Spalt suchen, der mind 170 Grad gross ist
  // und in die umgekehrte Richtung geht !!
  ie = -1;
  for(i1=1; i1<iNr; ++i1) {
    if(i1 == is) continue;
    // if(i1 == is+1) continue;
    d0 = pTab[i1].x-pTab[i1-1].x;
    // printf(" d0=%f dd=%f\n",d0,dd);
    if(da > 0) {
      if(d0 > gap2) ie = i1;
    } else {
      if(d0 < -gap2) ie = i1;
    }
  }
  if(ie < 0) {
    printf("TSU_sph_cyclic I001 \n");
    return 0;
  }
  // TX_Print("TSU_sph_cyclic E001"); return -1;


  L_ck2:
  // printf(" %d is=%d ie =%d da=%f\n",iLoop,is,ie,da);

  // Testen, ob zwischen is-ie ein Wert ist, der groesser als 180 ist;
  // wenn nein: nix tun; else modify.
  dd = RAD_180 + RAD_1;
  i2 = is -1;
  if(i2 < 0) i2 = iNr-1;
  if(ie > is) {
    for(i1=is+1; i1<ie; ++i1) {
      d0 = fabs(pTab[i2].x-pTab[i1].x);
      if(d0 > dd) goto L_move;
    }

  } else {
    for(i1=is+1; i1<iNr; ++i1) {
      d0 = fabs(pTab[i2].x-pTab[i1].x);
      if(d0 > dd) goto L_move;
    }
    for(i1=1; i1<ie; ++i1) {
      d0 = fabs(pTab[i2].x-pTab[i1].x);
      if(d0 > dd) goto L_move;
    }
  }
  goto L_exit;




  // Alle Punkte zwischen den Spalten um 360 Grad verschieben.
  // Von is bis ie um da verschieben.
  L_move:
  if(ie > is) {
    for(i1=is; i1<ie; ++i1) pTab[i1].x += da;
    goto L_exit;
  }


  // von (incl) is bis zum Ende verschiebn.
  for(i1=is; i1<iNr; ++i1) pTab[i1].x += da;


  // Vom beginn bis (excl) ie verschieben.
  for(i1=0; i1<ie; ++i1) pTab[i1].x += da;


  // kann 2 x notwendig sein (Tess_sph7)
  L_exit:
  ++iLoop;
  // if(iLoop < 3) goto L_start;



  // for(i1=0; i1<iNr; ++i1)  printf(" [%d].x=%f,%f\n",i1,pTab[i1].x,pTab[i1].y);
  // exit(0);
  // printf("ex TSU_sph_cyclic %d min=%f max=%f\n",iNr,uMin,uMax);
  // for(i1=0; i1<iNr; ++i1)  printf(" [%d].x=%f,%f\n",i1,pTab[i1].x,pTab[i1].y);

  return 0;

}


//================================================================
  int TSU_bsp_con_cyclic (Point *pTab, int iNr) {
//================================================================
// 2D-Feld zyklisch machen; muss aber innerhalb uMin-uMax bleiben.
// Wenn die Differenz zwischen 2 benachbarten X-Werten > als dTot/2, 
// dann ist der Wert dem vorherigen anzugleichen.
// Leider kann das Problem auch beim ersten Punkt schon existieren;
// beginnen erst nach einem normalen Punkt.


  int   i1, is, iLast;
  double dd, dMin, dMax, dTol, tol;

  dMin = TSU_dat[0];
  dMax = TSU_dat[1];
  dTol = fabs(dMax-dMin) / 2.;  // differenz fuer Vergleiche
  tol  = TSU_tol2D;


  // printf("TSU_bsp_con_cyclic min=%f max=%f\n",TSU_dat[0],TSU_dat[1]);
  // for(i1=0; i1<iNr; ++i1)  printf(" [%d].x=%f,%f\n",i1,pTab[i1].x,pTab[i1].y);
  // printf(" dTol=%f\n",dTol);



  // zuerst mit einem Wert beginnen, der nicht auf der Nahtstelle liegt.
  for(i1=0; i1<iNr; ++i1) {
    // ist der X-Wert nahe uMin od uMax ?
    if(UTP_comp2db(pTab[i1].x, dMin, tol) == 1) continue;
    if(UTP_comp2db(pTab[i1].x, dMax, tol) == 1) continue;
    is = i1;
    break;
  }

  // start at is
  iLast = is;
  // printf(" start %d\n",is);
  for(i1=is+1; i1<iNr; ++i1) {
    dd = pTab[i1].x - pTab[iLast].x;
    // ist der X-Wert nahe uMin od uMax ?
    if(UTP_comp2db(pTab[i1].x,dMin, tol) == 1) goto L_min1;
    if(UTP_comp2db(pTab[i1].x,dMax, tol) == 1) goto L_max1;
    iLast = i1;
    continue;

    L_min1:
      // war last val weiter entfernt als dTot/2: set to dMax.
      if(fabs(pTab[i1].x - pTab[iLast].x) > dTol) pTab[i1].x = dMax;
      continue;
    L_max1:
      // war last val weiter entfernt als dTot/2: set to dMin.
      if(fabs(pTab[i1].x - pTab[iLast].x) > dTol) pTab[i1].x = dMin;
      // continue;
  }



  // printf(" finish %d\n",is);
  for(i1=0; i1<is+1; ++i1) {
    dd = pTab[i1].x - pTab[iLast].x;
    // ist der X-Wert nahe uMin od uMax ?
    if(UTP_comp2db(pTab[i1].x,dMin, tol) == 1) goto L_min2;
    if(UTP_comp2db(pTab[i1].x,dMax, tol) == 1) goto L_max2;
    iLast = i1;
    continue;

    L_min2:
      // war last val weiter entfernt als dTot/2: set to dMax.
      if(fabs(pTab[i1].x - pTab[iLast].x) > dTol) pTab[i1].x = dMax;
      continue;
    L_max2:
      // war last val weiter entfernt als dTot/2: set to dMin.
      if(fabs(pTab[i1].x - pTab[iLast].x) > dTol) pTab[i1].x = dMin;
      // continue;
  }

 

  L_exit:
  // for(i1=0; i1<iNr; ++i1)  printf(" [%d].x=%f,%f\n",i1,pTab[i1].x,pTab[i1].y);
  // exit(0);

  return 0;

}


//================================================================
  int TSU_srv_con_cyclic (Point *pTab, int iNr) {
//================================================================
// 2D-Feld zyklisch machen.
// Ein Loch > 150 grad suchen; Punkte auf andere Seite.
// Die aussersten Punkte auf die andere Seite umschichten,
// wenn zwischen den aussersten Punkten und den naechstfolgenden mehr als 150
// Grad liegen ..
//     IN                OUT
//   0 3 4 5           6 3 4 5             // zw 0-3 ist ein 180-Grad-Loch
//   3 0 -1 -2 -3      -3 0 -1 -2 -3       // zw 3-0


  int       i1, ii, iold, imin, ind1, ind2, iloop;
  double    dd, d1, dmin, diff, gap, dTot;

  dTot = RAD_360;
  gap = (dTot / 2) - (dTot / 64);    // was RAD_180-RAD_10
  // gap = RAD_180-RAD_30;

  // printf("TSU_srv_con_cyclic %d %f %f\n",iNr,dTot,gap);
  // for(i1=0; i1<iNr; ++i1)  printf(" %d %f\n",i1,pTab[i1].x);
  // return 0;



  // min/max suchen;
  // Loch zw min/max suchen (sort; max Dist suchen; Mitte merken).
  // wenn maxDist < 150 Grad nix tun

  // min suchen
  imin = UTP_dbRec_min (iNr, pTab, sizeof(Point));
  dmin = pTab[imin].x;
  // printf(" min %d %f\n",imin,dmin);

  // naechstgrosseren Wert suchen;
  diff = 0.;
  iold = imin;
  iloop = -1;

  L_next:
  ii = -1;
  dd = UT_VAL_MAX;
  for(i1=0; i1<iNr; ++i1) {
    if(pTab[i1].x <= dmin) continue;
    d1 = pTab[i1].x - dmin;
    // printf(" d1=%f\n",d1);
    if(d1 > dd) continue;
    dd = d1;
    ii = i1;
  }
  // printf(" ii=%d dd=%f\n",ii,dd);
  if(ii >= 0) {
    if(dd > diff) {diff = dd; ind1 = iold; ind2 = ii;}
    dmin = pTab[ii].x;
    iold = ii;
    ++iloop;
    if(iloop < iNr) goto L_next;
    else {printf("TSU_srv_con_cyclic E001\n"); return -1;}
  }
    // printf(" ind1=%d ind2=%d diff=%f\n",ind1,ind2,diff);


  // das groesste Loch befindet sich zwischen ind1-ind2; 
  // Wert sind aufsteigend; Lochgroesse ist diff.
  // Loch kleiner 170 Grad: nix tun ...
  if(diff < gap) return 0;


  // wenn Mitte naeher bei min als bei max:
  // wenn Werte rechts groesser als links: aInc = +360; else aInc = -360
  // alle Werte < Mitte += aInc;
  // else alle Werte > Mitte -= aInc.

  // dd = Mittelwert errechnen
  dd = (pTab[ind1].x + pTab[ind2].x) / 2.;

  // d1 = Increment
  if(pTab[ind1].x < pTab[ind2].x) d1 = dTot;      // RAD_360
  else                            d1 = -dTot;     // -RAD_360
  // printf(" Mittelwert dd=%f inrc d1=%f\n",dd,d1);


  for(i1=0; i1<iNr; ++i1) {
    if(pTab[i1].x >= dd) continue;
    pTab[i1].x += d1;
  }


  // printf("ex TSU_srv_con_cyclic %d\n",iNr);
  // for(i1=0; i1<iNr; ++i1)  printf(" %d %f\n",i1,pTab[i1].x);

  return 0;

}


//================================================================
  int TSU_vMinMax (ContTab *cTab) {
//================================================================
// u und v geht NICHT immer nur von 0 bis 1.


  int      i1, i2;


  // den vMin u vMax aus der 2D-Aussenkontur suchen
  // printf(" iNr =%d\n",cTab[0].iNr);
  uMin = UT_VAL_MAX;
  uMax = UT_VAL_MIN;
  vMin = UT_VAL_MAX;
  vMax = UT_VAL_MIN;
  i2 = 0;
  for(i1=0;i1<cTab[i2].iNr;++i1) {
    // printf(" %d %f %f\n",i1,cTab[i2].p2a[i1].x,cTab[i2].p2a[i1].y);
    if(cTab[i2].p2a[i1].x < uMin) uMin = cTab[i2].p2a[i1].x;
    if(cTab[i2].p2a[i1].x > uMax) uMax = cTab[i2].p2a[i1].x;

    if(cTab[i2].p2a[i1].y < vMin) vMin = cTab[i2].p2a[i1].y;
    if(cTab[i2].p2a[i1].y > vMax) vMax = cTab[i2].p2a[i1].y;
  }

  // printf("ex TSU_vMinMax %f %f vOff=%f\n",vMin,vMax,vOff);


  uMin -= uOff/4.;
  uMax += uOff;

  vMin -= vOff/4.;
  vMax += vOff;


/*
  uMin = -0.25;
  uMax = 2.25;
  vMin = -0.25;
  vMax = 2.25;
*/


  return 0;

}


//================================================================
  int TSU_srv_con_1 (ContTab *cTab) {
//================================================================
// Nur SRV-Cones:
//  den mittleren Radius als Mittelwert der 2D-V-Werte errechen,
//  daraus nochmal den uOff errechnen.

  int      i1, i2;
  double   d1, d2, v1;
  Point    pt1, pt2;


  // printf("TSU_srv_con_1 vMin=%f vMax=%f\n",vMin,vMax);

  // // Mittelwert aus vMin/vMax
  // v1 = (vMin + vMax) / 2.;


  // den vMin u vMax aus der 2D-Aussenkontur suchen
  // printf(" iNr =%d\n",cTab[0].iNr);
  vMin = UT_VAL_MAX;
  vMax = UT_VAL_MIN;
  i2 = 0;
  for(i1=0;i1<cTab[i2].iNr;++i1) {
    // printf(" %d %f %f\n",i1,cTab[i2].p2a[i1].x,cTab[i2].p2a[i1].y);
    if(cTab[i2].p2a[i1].y < vMin) vMin = cTab[i2].p2a[i1].y;
    if(cTab[i2].p2a[i1].y > vMax) vMax = cTab[i2].p2a[i1].y;
  }



  // einen Punkt mit u=0, v=vMin -> 3D
  pt1.x = 0.;
  pt1.y = vMin;
  pt1.z = 0.;
  TSU_tr_3D_2D_srv (&pt2, &pt1);
    // GR_Disp_pt(&pt2, SYM_TRI_B, 2);

  // den Punkt auf die Achse projizieren == Radius d1
  UT3D_pt_projptptvc(&pt1,&d1,NULL, &pt2,&TSU_prx,&TSU_vrx);
    // GR_Disp_pt(&pt1, SYM_TRI_S, 2);


  // einen Punkt mit u=0, v=vMax -> 3D
  pt1.x = 0.;
  pt1.y = vMax;
  pt1.z = 0.;
  TSU_tr_3D_2D_srv (&pt2, &pt1);
    // GR_Disp_pt(&pt2, SYM_TRI_B, 2);

  // den Punkt auf die Achse projizieren == Radius d1
  UT3D_pt_projptptvc(&pt1,&d2,NULL, &pt2,&TSU_prx,&TSU_vrx);
    // GR_Disp_pt(&pt1, SYM_TRI_S, 2);

  if(d2 > d1) d1 = d2;

  d1 *= 2.;

  // nun ist d1 der groesste Radius des cone
  // printf(" TSU_srv_con_1-d1=%f\n",d1);
  uOff = UT2D_angr_ciSec (UT_DISP_cv, d1);
  // printf("ex TSU_srv_con_1 uOff=%f d1=%f\n",uOff,d1);

  // printf("        con_1 vMin=%f vMax=%f vOff=%f\n",vMin,vMax,vOff);

  vOff = (vMax - vMin);

  vMin -= vOff * 0.1;
  vMax += vOff;

  vOff *= 1.2;


  // TEST:
  // uMin -= 0.2;
  // uMax += 0.2;


  return 0;

}


//================================================================
  int TSU_srv_tor_2 (double vAct, int iv) {
//================================================================
// NUR SRV-Torus, wenn Drehachse innerhalb Torus ist, muss fuer Aussere/innere 
// Haelfte der TSU_dir fuer jede V-Reihe gesetzt werden !

static int   actDir;
static Point pta;

  int     sid;
  double  d1;
  Point   pt1, pt2;
  Vector  vc1;


  // printf("TSU_srv_tor_2 %d\n",iv);
  // printf("  form=%d typ=%f\n",TSU_ox2.form,TSU_dat[5]);


  if(TSU_dat[5] < 0.5) return 0;  // skip Kugel ..
  if(TSU_dat[5] > 2.5) return 0;  // skip normalen Torus


  if(iv == 0) {
    actDir = TSU_dir;
    UT3D_pt_projptptvc(&pta, &d1, NULL,
                       &((Circ*)TSU_ox2.data)->pc, &TSU_prx,&TSU_vrx);
      // GR_Disp_pt(&pta, SYM_TRI_B, 2);
  }


  // pt1 = 3D-Punkt mit aktuellem V-Wert vAct 
  pt1.x = 0.;
  pt1.y = vAct;
  pt1.z = 0.;
  TSU_tr_3D_2D_srv (&pt2, &pt1);
  // sid = UT3D_sid_3pt (&pta, &TSU_ac1.pc, &pt2);

  // Tangente ..
  UT3D_vc_tng_ci_pt (&vc1, &pt2, &TSU_ac1);

  sid = UT3D_sid_2vc (&vc1, &TSU_vrx);


  if(sid < 0) TSU_dir = actDir;
  else        TSU_dir = -actDir;


  // printf("ex TSU_srv_tor_2 iv=%d sid=%d\n",iv,sid);

  return 0;

}

/*
//================================================================
  int TSU_srv_tor_1 (Circ *cio, Circ *cii, Vector *vca) {
//================================================================
// Kreisbogen darf nicht ueber die Achse drueberragen (max Halbkreis)

  int     i1;
  Vector  vc1;
  Point   pt1, pt2;


  *cio = *cii; // copy cir

  // Vollkreis: in Halbkreis umwandeln
  if(UT3D_ck_ci360(cii) != YES) goto L_1;


  // project Endpt of vca -> Circ.
  UT3D_pt_traptvc (&pt1, &cio->pc, vca);
  UT3D_pt_projptci (&pt2, &pt1, cio);

  UT3D_ci_cipt180 (cio, &pt2);


  L_1:

  // UT3D_stru_dump (Typ_CI, cio, "ex TSU_sph_1");

  return 0;

}
*/

//================================================================
  int TSU_srv_tor_i1 (Circ *ci1) {
//================================================================
// die beiden Schnittpunkte Konturkreis- RotAxis suchen.
// TSU_prx ist der Kreismittelpkt auf die RotAxis projiziert.
// ci1 ist der Konturkreis.
// TSU_dat[6] ist der Abstand TSU_prx - ci1->pc.

  double d1;


  if(UTP_comp2db(TSU_dat[6],fabs(ci1->rad),UT_TOL_cv) == 1) {
    TSU_pintNr = 1;
    TSU_pint[0] = TSU_prx;
    return 0;
  }

  d1 = sqrt((ci1->rad * ci1->rad) - (TSU_dat[6]*TSU_dat[6]));

  
  TSU_pintNr = 2;
  UT3D_pt_traptvclen (&TSU_pint[0], &TSU_prx, &TSU_vrx, d1);
  UT3D_pt_opp2pt (&TSU_pint[1], &TSU_prx, &TSU_pint[0]);

  return 0;

}

//================================================================
  int TSU_srv_tor_01 (Circ *cio, Circ *cii) {
//================================================================
// eine Kreisbogen begrenzt durch die Intersectionpoints generieren
// TSU_vrx = Rotationsachse Torus.

  int    irc, i1;
  double d1, d2;
  Point  ptx, pTab[2], ptm;
  Line   lnx;


  // UT3D_stru_dump (Typ_CI, cii, "TSU_srv_tor_01:");
  // GR_Disp_ac (cii, 8);

  irc = 0;

  *cio = *cii; // copy cir

  cio->rad = fabs(cio->rad);


  // ist Kreisachse normal auf die Drehachse ?
  if(UT3D_vc_ck_perpvc (&cio->vz, &TSU_vrx, RAD_1) == 0) goto L_not_normal;



  //================================================================
  // ja, Kreisachse ist normal auf die Drehachse.
  // printf(" L_normal:\n");

  // beruehrungspunkt ?
  if(TSU_pintNr < 2) TSU_pint[1] = TSU_pint[0];


  // die 2 Intersectionpoints TSU_pint als Endpunkte setzen

  // normalvector auf die RotAxis
  // UT3D_vc_perp1vc (&cio->vz, &TSU_vrx);
  // UT3D_vc_invert (&cio->vz, &cio->vz);


  // // cio->vz ist zwar normal auf RotAxis TSU_vrx, kann aber verkehrt sein.
  // TSU_srv_ac_nvc (&cio->vz, &cio->pc);

/*
  // den vz umdrehen, wenn kreis auf anderer Seite liegt !
  // den Mittelpunkt des Inputkreises; 2 x crossprod = Normalvektor.
  UT3D_pt_midci (&ptm, cii);
    // GR_Disp_pt (&ptm, SYM_TRI_S, 4);
  UT3D_vc_perpvc2pt (&cio->vz, &TSU_vrx, &cii->pc, &ptm);
  UT3D_vc_setLength (&cio->vz, &cio->vz, 1.);
    // GR_Disp_vc (&cio->vz, &cii->pc, 9, 0);
*/

  // macht hier immer Halbkreis;
  // ist Problem; macht zB aus einem 30-Grad-Kreisbogen einen Halbkreis !
  // 1) den Kreisbogen an der Achse begrenzen, wenn notwendig
  // 2) den Kreisbogen umdrehen, wenn notwendig
  // ad 1: ArcMittelpunkt suchen; ArcStart- und Endpunkte muessen auf der
  //       gleichen Seite sein - else trimmen.
  // ad 2: ArcEndpunkte muss in Richtg RotAxis weiter weg sein als ArcStartpt;
  //       else Kreisbogen umdrehen.
  // cio->p1=cii->p2;
  // cio->p2=cii->p1;


  // der KreisbogenEndpunkt muss in Richtung TSU_vrx weiter weg sein von
  // TSU_prx als der KreisbogenStartpunkt; else Kreis umdrehen.
  irc = UT3D_cksid_civc (cii, &TSU_vrx);
  // irc=1 = verkehrt; man muss Circ umdrehen; man muss auch v0/v1 invertieren !
  if(irc != 0) UT3D_ci_inv3 (cio);


  // cio->p1   = TSU_pint[0];
  // cio->p2   = TSU_pint[1];
  // cio->ango = UT3D_angr_cipt (cio, &cio->p2);


  goto L_done;




  //================================================================
  L_not_normal:
  // printf(" L_not_normal:\n");
  // Die DrehAchse auf die Circ-plane projizieren;
  // den Circ mit dieser Line schneiden = Limitpoints
  UT3D_pt_projptptnvc (&lnx.p1, &TSU_prx, &cio->pc, &cio->vz);
  UT3D_pt_traptvc (&ptx, &TSU_prx, &TSU_vrx);
  UT3D_pt_projptptnvc (&lnx.p2, &ptx, &cio->pc, &cio->vz);

  UT3D_pt_intlnci_p (&i1, pTab, &lnx, cio);
  cio->p1   = pTab[0];
  cio->p2   = pTab[1];
  cio->ango = UT3D_angr_ci_p1_pt (cio, &cio->p2);




  //================================================================
  L_done:
  // Richtung TSU_dir setzen .
  TSU_dir = 1;

    // GR_Disp_ac (cio, 12);

  return irc;

}
 
/* ersetzt dr TSU_srv_ac_nvc
//================================================================
  int TSU_srv_ac_nvc (Vector *nvc, Point *pt1) {
//================================================================
// make normalvektor auf Rotationsachse - pt1.
// In:
//   pt1 ist der Konturkreismittelpunkt;
//   TSU_vrx ist RotAxis;
//   TSU_prx ist RotCenter;
// Out:
//   nvc    ist Z-Vec fuer Konturkreis, dessen Cen pt1 ist;
//          und ist normal auf RotAxis.


  Vector   vcc;

  // UT3D_stru_dump (Typ_PT, pt1, "TSU_srv_ac_nvc pc:");
  // UT3D_stru_dump (Typ_VC, &TSU_vrx, " TSU_vrx:");


  // vec CircCenter --> Achse
  // UT3D_vc_2pt (&vcc, pt1, &TSU_prx);
  UT3D_vc_2pt (&vcc, &TSU_prx, pt1);

  // Normalvec
  UT3D_vc_perp2vc (nvc, &TSU_vrx, &vcc);

  UT3D_vc_setLength (nvc, nvc, 1.);

  // UT3D_stru_dump (Typ_VC, nvc, "ex TSU_srv_ac_nvc:");

  return 0;

}
*/
 
//================================================================
  int TSU_srv_tor_03 (Circ *cio, Circ *cii, Point *ptc, Vector *vca) {
//================================================================
// den Konturkreis auf Vollkreis mit Startpunkt mitte innen setzen
// ptc ist der auf die Drehachse projizierte KonturkreisCenterPunkt
// vca ist Drehachse

  Vector   vcc;


  // UT3D_stru_dump (Typ_CI, cii, "TSU_srv_tor_03:");

  *cio = *cii; // copy cir

  TSU_pintNr = 0;


  // NUR wenn es ein Vollkreis ist (sonst zB so ein Viertelkreis moeglich
  if(UT3D_ck_ci360(cio) != 0) return 0;
  // return 0;


  // normalvector auf die RotAxis (braucht TSU_prx)
  // TSU_srv_ac_nvc (&cio->vz, &cio->pc);
  UT3D_vc_perpvc2pt (&cio->vz, &TSU_vrx, &TSU_prx, &cio->pc);


  // move KonturkreisCenterPunkt --> ptc
  UT3D_vc_2pt (&vcc, &cio->pc, ptc);
  UT3D_pt_traptvclen (&cio->p1, &cio->pc, &vcc, fabs(cio->rad));
  cio->p2   = cio->p1;
  cio->ango = RAD_360;

    // GR_Disp_ac (cio, 2);
    // UT3D_stru_dump (Typ_CI, TSU_ox2.data, " KonturCirc SurRV:");


  // // Richtung TSU_dir setzen .
  // TSU_dir = 1;

  // UT3D_stru_dump (Typ_CI, cio, "ex TSU_srv_tor_03:");

  return 0;

}


//================================================================
  int TSU_sph_pln () {
//================================================================
// Plane TSU_pln fuer spher. surf machen;
// origin = CircCenter;
// Z-vec  = rotAxis TSU_vrx
// X-vec  = Punkt am Circ. (crossVec circAxis-zVec)

  TSU_pln.po = TSU_ac1.pc;
  TSU_pln.vz = TSU_vrx; // rotAxis

  UT3D_vc_perp2vc (&TSU_pln.vx, &TSU_pln.vz, &TSU_ac1.vz);
  UT3D_vc_setLength (&TSU_pln.vx, &TSU_pln.vx, 1.);

  UT3D_vc_perp2vc (&TSU_pln.vy, &TSU_pln.vz, &TSU_pln.vx);

  // UT3D_stru_dump (Typ_PLN, &TSU_pln, "ex TSU_sph_pln:\n");

  return 0;

}


//================================================================
  int TSU_sph_1 (Circ *cio, Circ *cii) {
//================================================================
// Kreisbogen darf nicht ueber die Achse drueberragen (max Halbkreis)
// - Konturkreis cio erzeugen
// - die 2 Intersectionpoints TSU_pint erzeugen
// - Richtung TSU_dir setzen ..
// Inputs:
//   TSU_prx/TSU_vrx = Rotationsachse
//   cii   = Konturkreis
// Out:
//   cio - max Halbkreis


  int     irc, i1;
  Vector  vc1;
  Point   ptm;


  // UT3D_stru_dump (Typ_CI, cii, "TSU_sph_1");


  *cio = *cii; // copy cir

  cio->rad = fabs(cio->rad);

  // die 2 Intersectionpoints TSU_pint erzeugen
  TSU_pintNr = 2;
  UT3D_pt_traptvclen (&TSU_pint[0], &cio->pc, &TSU_vrx, cio->rad);
  UT3D_pt_opp2pt (&TSU_pint[1], &cio->pc, &TSU_pint[0]);


  // IMMER einen 180-Grad-kreis generieren;
  // KreisAchse normal auf die Rot-Achse setzen.
  // UT3D_vc_perp1vc (&cio->vz, &TSU_vrx);
  // TSU_srv_sph_nvc (&cio->vz, &cio->pc);
  
  // den vz umdrehen, wenn kreis auf anderer Seite liegt !
  // den Mittelpunkt des Inputkreises; 2 x crossprod = Normalvektor.
  // UT3D_pt_midci (&ptm, cii);
  // UT3D_vc_perpvc2pt (&cio->vz, &TSU_vrx, &cii->pc, &ptm);


  // der KreisbogenEndpunkt muss in Richtung TSU_vrx weiter weg sein von
  // TSU_prx als der KreisbogenStartpunkt; else Kreis umdrehen.
  irc = UT3D_cksid_civc (cii, &TSU_vrx);
  // irc=1 = verkehrt; man muss Circ umdrehen; man muss auch v0/v1 invertieren !
  if(irc == 0) UT3D_ci_inv3 (cio);  // Achtung - verkehrt wie bei Torus !



  cio->p1   = TSU_pint[0];
  cio->p2   = TSU_pint[1];
  cio->ango = RAD_180;


  // Richtung TSU_dir setzen .
  TSU_dir = 1;


/*
  // Vollkreis: in Halbkreis umwandeln
  if(UT3D_ck_ci360(cii) != YES) goto L_1;

  // project Endpt of &TSU_vrx -> Circ.
  UT3D_pt_traptvc (&pt1, &cio->pc, &TSU_vrx);
	UT3D_pt_projptci (&pt2, &pt1, cio);

  UT3D_ci_cipt180 (cio, &pt2);



  L_1:
  // tangentialvektor an einen Punkt in der Mitte des Circ;
  // ist dieser vec eher parallel zur Achse oder eher antiparallel ?
  UT3D_pt_midci (&pt1, cio);
  UT3D_vc_tng_ci_pt (&vc1, &pt1, cio);

  i1 = UT3D_sid_2vc(&vc1, &TSU_vrx);
  if(i1 < 0) { // Umdrehen nur merken, am Ende alle Konturen umdrehen.
    TSU_dir = 1;
  } else {
    TSU_dir = -1;
  }
  // printf(" TSU_dir=%d\n",TSU_dir);
*/


  // UT3D_stru_dump (Typ_CI, cio, "ex TSU_sph_1");

  return irc;

}


//================================================================
  int TSU_DrawSurTess (long *dli, int att, ObjGX *oxi) {
//================================================================
// draw Mockup-struct (tesselated surf)
// see also GL_DrawSur

  int    irc;
  int    rSiz;
  ObjGX  *ox1;


  // printf("TSU_DrawSurTess %d %d %d\n",oxi->typ,oxi->form,att);
  // return 0;


  att = Typ_Att_Fac1; // statt att kommt modNr; wer braucht diese ?

  if(oxi == NULL) {printf("TSU_DrawSurTess E000\n"); return -1;}


  // GL_Draw_Ini (dli, att);
  GL_Surf_Ini (dli, (void*)&att);

  GL_ColSet (&GL_actCol); // notwendig fuer den Fall, das spaeter keine
                          // eigene farbe kommt; sonst schwarz !

  // wenn das erste obj Adress: skip it
  if(oxi->typ == Typ_Address) ++oxi; // (char*)oxi += sizeof(ObjGX);
  // printf(" next: %d %d\n",oxi->typ,oxi->form);


  // tess_dump_f_ (oxi, "vor GL_DrawSur");

  L_next:
  // nun Recordsize
  if(oxi->typ  == Typ_Done)  goto L_fertig;

  if(oxi->typ  != Typ_Size) goto L_Err2;
  rSiz = (long)oxi->data;
  // printf(" sizRec %d %d %d\n",oxi->typ,oxi->form,rSiz);
  // (char*)oxi += sizeof(ObjGX); // skip SizeRecord
  ++oxi; // skip SizeRecord

    // UTO_dump_s_ (oxi, "TSU_DrawSurTess");
    // UTO_dump__ (oxi, "TSU_DrawSurTess");

  irc = GL_Disp_sur (oxi);

  // (char*)oxi += rSiz; // skip faces
  oxi = (ObjGX*)((char*)oxi + rSiz); // skip faces

  if(irc == 0) goto L_next;

  L_fertig:
  GL_EndList ();
  return 0;

  L_Err1:
    TX_Error("TSU_DrawSurTess E001 form %d",oxi->form);
    // UTO_dump__ (oxi, "TSU_DrawSurTess");
    return -1;

  L_Err2:
    TX_Error("TSU_DrawSurTess E002 typ %d",oxi->typ);
    // UTO_dump__ (oxi, "TSU_DrawSurTess");
    return -1;


}


//========== EOF =================================================
