// ut_DL.c    displayList functions     RF
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
2002-02-12 DL_disp_* zu. RF.
2001-06-06 DL_Lay_mod (war DB_Lay).
2001-05-04 DL_Delete, DL_GetTyp, DL_GetInd neu.
2000-10-17 Vereinheitlichung mit d. Unix-Variante.

-----------------------------------------------------
*/
#ifdef globTag
 void DL(){}
#endif
/*!
\file  ../gr/ut_DL.c
\brief displayList functions 
\code
=====================================================
List_functions_start:

DL_Init
DL_InitAttTab           load file ltyp.rc into GR_AttTab and create DL-record
DL_InitAttRec           define a new lineattributeRecord
DL_alloc__              realloc space fuer BasModelnames

DL_AttLn_store          store line-attribute in table GR_AttTab
AttLn_Set1              get line-attribute from 3 integers
AttLn_Get_ind           get line-attribute from GR_AttLnTab-index 
IndAttLn_Set1           set linetyp, limitTyp
IndAttLn_get_ltyp       get linetyp

DL_Redraw               redraw complete DL
DL_Draw_obj             redraw single obj, use existing DL-record
DL_Stat                 ausgabe Statistik
DL_DumpObjTab
DL_DumpObj__

DL_StoreObj             Den naechsten freien DispList-Platz belegen
DL_StoreAtt             store GR_Att in GR_AttTab
DL_SetObj               store DL-record (only for active vector)

DL_hili_on              set obj hilited
DL_hili_off             reset hilited
DL_disp_hili_last       (change) hilite last obj of DL
// DL_disp_hili            hilite Obj of line lNr

DL_hide__               change hidden/visible for single obj
DL_hide_all             change hidden/visible for all objs
DL_disp_def             fuer alle nun folgenden Obj GR_ObjTab.disp=mode setzen
DL_hide_unvisTypes      view or hide all joints,activities.

DL_disp_reset           delete all DL-objects starting from line-nr

DL_unvis_set            set visible / unvisible

DL_pick__               change all objects to pickable or unpickable
DL_pick_set

DL_parent_ck__          get all parents for obj (typ,dbi,dli)
DL_parent_ck_c          check if DL-record has a parent-object (check .sChd-Flag)
DL_parent_ck_p          check if DL-record is a parent-object (check .sPar-Flag)
DL_parent_hide
GA_parent_get           get DB-index of parentObj of DB-obj dbTyp/ind

DL_grp1_set             Zugehoerigkeit zu Group 1 setzen; ON od OFF
DL_grp1_get             get grp_1-bit
DL_grp1__               add / remove (change) Groupbit 1 of DL-Record ind
DL_grp1_copy            copy all DL-obj with groupBit ON --> GroupList

DL_get__                returns DispList
DL_GetAtt               get DL-record (DL_Att from GR_ObjTab[objInd])
DL_Get_GrAtt            get graf.Att (GR_Att from GR_AttTab[Ind])
DL_get_sStyl            get surfaceStyle (shaded|symbolic)
DL_get_iatt             returns iatt of DL-record
DL_set_iatt             modify iatt of DL-record
DL_GetTyp               get obj-typ from DL-ind
DL_GetInd               get DB-ind from DL-ind
DL_Get_oid_dli          get objName from DispListIndex
DL_Get_lNr_dli          get sourceLineNumber from DispListIndex
DL_Get_dli_lNr          get DispListIndex from sourceLineNumber
DL_GetTrInd             get refSys-Index from dli
DL_GetPick
DL_GetNrSur             get nr of surs in DispList
DL_find_smObj           get dispListIndex of DB-obj from typ/dbi/subModelNr
DL_find_obj             Objekt typ=typ APTind=ind in der DL suchen
DL_find_APPOBJ          find applicationObject
DL_find_sel             find selected object in DL

DL_SetInd               modify (do not create new DL-Record); set DL_ind_act=dli;

DL_Lay_act_g1           activate GroupBit1 des layer Nr. layNr
DL_Lay_typ_g1           grp_1 fuer alle Obj, die NICHT Typ i1 haben, auf OFF.
DL_Lay_col_g1           GroupBit1 mal f. alle Obj, die NICHT Farbe i1 haben, ON
DL_Lay_ltyp_g1
DL_Lay_thk_g1
DL_Lay_mod
DL_Lay_add

DL_txtgetInfo           Infos zu Tag/Image holen
DL_txtSelect            check if TextTag was selcted and add obj to tables
DL_IactSelect           check if Interactivity is connected
DL_setTagSiz            bei tags und Bitmaps die size im DL-record speichern

DL_sav_dynDat           save the DYNAMIC_DATA of the actual mainModel
DL_load_dynDat          reload the DYNAMIC_DATA
DL_wri_dynDat0
DL_wri_dynDat1

DL_ReScale__
DL_ReScale_Notes
DL_ReScale_pt_get       uxmin-uzmax erweitern um Box
DL_ReScalePoint
DL_ReScaleObj

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



============================================================================
DL_Att GR_ObjTab[GR_ATT_TAB_SIZ]

/// lNr    APTlineNr.
/// typ    DB-Typ
/// ind    DB-Index
/// irs    index RefSys
/// modInd ModelNr
/// iatt   for typ=LN/AC/Curve: LineTypNr.
///        for typ=TAG/IMG: sx/sy
///        for typ=Surf/Model: ColRGB
///        in Parent-state DL-index of child
/// disp, hili:
///        normal = ((disp == 0)&&(hili == 1))
///        hilite = ((disp == 1)&&(hili == 0))
///        hidden = ((disp == 1)&&(hili == 1))
/// pick   0=unpickable, 1=pickable
/// unvis  0=visible; 1=obj does not have graph. representation
/// sChd   0=independent; 1=Child-state is active; this obj has a parent.
/// sPar   0=independent; 1=Parent-state is active.
/// grp_1  0=belongs to active Group, 1=not


FOR EVERY DL-record a GL-record is necessary !



----------------------------------------------------------------------------
  Eine Tabelle, in der  Farbe, Linetyp und LineThickness .. liegen.
  Attind ist ein Index in die GR_AttTab; GL aktiviert die zugehörigen Werte.

  DL_InitAttRec (ind, col, ltyp, lthick); // ein neues Attribut definieren.
  // Die ersten 13 Entries sind vordefiniert; 
    DL_InitAttRec (Typ_Att_PT,    1, 0, 4);


  Die Standardattrib's    - see Func. DL_InitAttTab() ex File ltyp.rc
                 col typ thick
  0 Typ_Att_PT,    0, 0, 1  Normal
  1 Typ_Att_def,   0, 0, 0  dünn
  2 Typ_Att_go,    0, 2, 0  dünn strichliert
  3 Typ_Att_rp,    0, 1, 0  dünn strich-punkt
  4 Typ_Att_Fac,  11, 0, 1  blaugrau
  5 Typ_Att_Fac1, 12, 0, 1  blau                      DEFCOL
  6 Typ_Att_Fac2, 13, 0, 1  grau
  7 Typ_Att_Symb,  5, 0, 3  gelb,    voll, sehr dick
  8 Typ_Att_hili,  3, 0, 3  gruen,   voll, sehr dick
  9 Typ_Att_hili1, 9, 0, 4  ziegel,  voll, sehr dick
 10 Typ_Att_dim,  10, 0, 1  blaugrau,voll, duenn
 11 Typ_Att_top,   0, 0, 4  def=blau,voll, sehr dick
 12 -              2  2  2  dick rot strichliert

  Die Farben:
     0 COL_Default
     1 COL_BLACK
     2 COL_ROT
     3 COL_GRUEN
     4 COL_BLAU
     5 COL_GELB
     6 COL_MAGENTA
     7 COL_CYAN
     8 COL_WEISS
     9 hili
    10 COL_DIMMED
    11 blaugrau f. faces
    12 kupferrot f. faces
    13 grau f. faces
    14 dim

  Die Linientypen:
    0 voll
    1 str punkt
    2 strichl
    3 strichl. lang

  Die Liniendicken:
    1-6;    1=dünn, 6 ist am dicksten.



----------------------------------------------------------------------------
GR_ObjTab; enthaelt (8bit)typ, (long)ind, (short)RefsysNr, und (short) attrib
  von allen darzustellenden Objekten.
  ind ist der Index in den DB-Speicher; PT1 hat 1, LN1 ebenfalls 
   (sind aber durch den Typ unterschiedlich).
  Weiters gibts (8bit)layer, (1bit)display, (1bit) pick usw ...
  Der Index dieser Objektnummer ist gleichzeitig die DispListID in OpenGL.
  Er wird retourniert von DL_StoreObj und muss an GR_DrawLine weitergegeben
  werden !


------------------------------------------------------------------------
Ablauf:
1) Store Line in DB
  dlInd = DL_StoreObj (Typ_LN, objInd, attInd);
  // IN:  Objectindex objInd  und ObjTyp Typ_xx
  // OUT: DispListIndex dlInd


2) Draw Line mit APT_DrawLine
  GR_DrawLine (&dlInd, attInd, &ln1);
  // IN:  DispList #
  // OUT: nix.


3) Redraw:
  GL_Redraw1 greift selbst auf die GR_ObjTab zu.



---------------------------------------------------
Draw temporary objects: 
  for all objects:  use negative dli, eg -2L or -3L
  l1 = -2L;
  GR_Draw_rect1 (&l1, ..);
  ..
  GL_temp_Delete (2L);  // delete this temp obj


  GR_Disp ??


---------------------------------------------------
Compile:
cc -c -g3 -Wall ut_DL.c





*/


#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif


#include <string.h>                       /* strcmp ..   */
#include <math.h>                         /* fabs  ...   */
#include <stdio.h>                        /*  FILE .. */
#include <stdlib.h>                      // realloc


#include <GL/gl.h>
// #include <GL/glu.h>


#include "../ut/ut_geo.h"                /* Point-def   */
#include "../ut/ut_TX.h"                 /* TX_Error */
#include "../ut/ut_txfil.h"              // UTF_clear1
#include "../ut/ut_txt.h"                // fnam_del
#include "../ut/ut_os.h"                 // OS_ ..

#include "../gr/ut_UI.h"                 // Typ_Att_PT, SYM_TRI_S, ..
#include "../gr/ut_UI.h"                 // Typ_Att_PT, SYM_TRI_S, ..
#include "../gr/vf.h"                    // GL_vf1_CS

#include "../db/ut_DB.h"                 // DB_GetPoint ..


#include "../gr/ut_GL.h"
#include "../gr/ut_DL.h"
#include "../gr/ut_gr.h"

#include "../xa/xa_mem.h"                // mem_cbuf1
#include "../xa/xa_uid.h"                // UID_ckb_view
#include "../xa/xa_obj_txt.h"            // AP_obj_add_val
#include "../xa/xa.h"                    // AP_Get_ConstPl_Z



/*=============== Externe Variablen: =======================================*/
// aus xa.c:
extern AP_STAT    AP_stat;                    // sysStat,errStat..
extern  int       WC_modnr;
extern  int       WC_sur_ind;            // Index auf die ActiveConstrPlane
extern Plane      WC_sur_act;     // die Konstruktionsebene
extern double     WC_sur_Z;       // Konstruktionsebene in Z verschieben !
extern double     AP_txsiz;       // Notes-Defaultsize
extern double     AP_txdimsiz;    // Dimensions-Text-size
extern int        AP_txNkNr;            // Nachkommastellen

extern ColRGB     AP_defcol;


// alle aus ../ci/NC_Main.c:
extern double APT_ModSiz;
extern int        GR_pick;                     // NOPICK
extern int        GR_lay_act;
extern int        APT_line_act;                // die momentane APT-LineNr
extern int        UP_level;
extern int        AP_mdLev;
extern int        APT_dispPT, APT_dispPL;

// aus ../gr/ut_GL.c:
extern int        GL_modified;
// extern int        GL_vcSelStat;


// aus ../gr/ut_gtx.c:
extern double GR_tx_scale;





/*=============== Lokale Variablen: =======================================*/
DL_Att     *GR_ObjTab = NULL;            // das ist die DL
long       GR_TAB_SIZ = 0;               // momentane size of GR_ObjTab
long       GR_TAB_INC = 10000;           // beim realloc vergroessern um -
long       GR_TAB_IND = 0;               // naechster freier Index


Att_ln     GR_AttLnTab[GR_ATT_TAB_SIZ];  // line-attributes (dash, color ..)
// GR_Att     GR_AttTab[GR_ATT_TAB_SIZ];    // die AttributTabelle
static int GR_AttInd=0;                  // Anzahl = naechster freier Index





static double uxmin=0.,uymin=0.,uzmin=0.;      // links unten Userkoords
static double uxmax,   uymax,   uzmax;         // rechts oben in Userkoords


static int    DL_disp_act;          // der Status des Hide-Attribut .disp

       long   DL_ind_act=-1;        //  -1: create a new DL-Rec;
                                    // >=0: modify existing DL-Rec.
                                    // Default = creat new (-1)






//====================================================================
  int DL_txtgetInfo (int *typ, Point *p1,
                     int *sx, int *sy, int *dx, int *dy, long dli) {
//====================================================================
/// \code
/// Infos zu Tag/Image holen
/// Input:
///   dli    DL-Index of "N"ote
/// Output:
///   typ    Tagtyp; 1=Image ..
///   p1     Textposition of Text in Usercoords
///   sx, sy size of 2D-Field in screencoords
///   dx, dy offset from p1 to lower left 2D-corner in screencoords
/// \endcode


  ObjGX     *ox1;
  double    scl;
  uni_i4i2  *ia;


  // get Textpoint of Tag;
  ox1 = DB_GetGTxt (GR_ObjTab[dli].ind);
  if(ox1->typ == Typ_Error) return -1;


  // copy typ
  *typ = ((AText*)ox1->data)->typ; // 1=Image

  // if(*typ >= 5) return -1;   // symbols werden normal gefunden !


  // copy textposPoint
  *p1 = ((AText*)ox1->data)->p1;

  // get scale
  scl = ((AText*)ox1->data)->scl;


 // Tag/Image benutzt .refInd fuer sx; .attInd fuer sy.
 // new long .attInd == eine Union aus 2 shorts; s1 und s2
 // Fuer jedes Tag/Image ein ObjAtt anlegen;
 // gu.i4 mit uni_i4i2 fuer sx/sy benutzen.

  // copy Filedsize out
  ia = (void*)&(GR_ObjTab[dli].iatt);
  *sx = ia->i2[0];
  *sy = ia->i2[1];
  // printf(" tagSiz=%d %d\n",*sx,*sy);

  // *sx = GR_ObjTab[dli].refInd;
  // *sy = GR_ObjTab[dli].attInd;



  // Image-Bitmap: scalen;
  // Tag: NICHT scalen; dy um halbe Taghoehe nach unten verschieben.
  if(*typ == 1) {  // Bitmap: scalen;
    if(fabs(scl) < UT_TOL_min1) {
      scl = GL_get_Scale ();
    }
    *sx *= scl;
    *sy *= scl;
    *sx += 2;     // ??
    *dx = 0;
    *dy = 0;



  } else if(*typ >= 7) {
    *sx = 16;
    *sy = 12;
    *dx = -8;
    *dy = -8;


  } else if(*typ >= 5) {
    *sx = 12;
    *sy =  8;
    *dx = -6;
    *dy = -6;



  } else {                // Tags: um halbe Hoehe nach unten.
    *sx += 6;
    // *sy -= 4;              // den Ueberstand wieder weg;
    *dx  = -3;
    *dy  = -(*sy / 2) - 2;
  }

  // printf("ex DL_txtgetInfo %d / %d %d / %d %d\n",*typ,*sx,*sy,*dx,*dy);
  // UT3D_stru_dump(Typ_PT, p1, " TagposLU=");

  return 0;

}


//====================================================================
  int DL_txtSelect (int iNr, ObjDB **dlTab) {
//====================================================================
/// check if TextTag was selcted and add obj to tables
// Output:
//   dlTab      a pointer to GR_selTab

  int    irc, i1, mx, my, sx, sy, dx, dy, tx, ty, typ;
  long   l1;
  double px, py, pz;
  Point  p1;


  // printf("DL_txtSelect %d\n",iNr);
  // GL_sel_dump (iNr);


  // active Mousepos in ScreenCoords is GL_mouse_x_act/GL_mouse_y_act
  GL_GetCurPosSc (&mx, &my);
    // printf(" mousePos=%d %d\n",mx,my);


  // check if tag are in DL
  for(l1=0; l1<GR_TAB_IND; ++l1) {
    if(GR_ObjTab[l1].typ  != Typ_Tag) continue;
/*
    // test if belongs to subModel
    if((signed short)GR_ObjTab[l1].modInd != WC_modnr) {
        printf(" ************ belongs to subModel %d\n",
               (signed short)GR_ObjTab[l1].modInd);
      iNr = GL_sel_add_DL (dlTab, l1);
      continue;
    }
*/
    // skip hidden obj
    if((GR_ObjTab[l1].disp==1)&&(GR_ObjTab[l1].hili == 1)) continue;
    // if(GR_ObjTab[l1].del  != 0) continue;         // skip deleted
    // if(GR_ObjTab[l1].pick != 0) continue;         // skip unpickable


    // SizeInfo zu Tag/Image holen
    irc = DL_txtgetInfo (&typ, &p1, &sx, &sy, &dx, &dy, l1);
    if(irc < 0) continue;  // zB SymbolTags; werden normal auch gefunden.

    // change Textpoint --> Screencoords
    GL_Uk2Sk (&px, &py, &pz, p1.x, p1.y, p1.z);
      // printf(" px=%f py=%f pz=%f\n",px,py,pz);

    // ty,ty --> lower left corner
    tx = px + dx;
    ty = py + dy;
    // printf(" tx=%d ty=%d\n",tx,ty);

    // check if selectPosition is in Texlabel.
    // mx,my ist nun links unten; sx,sy ist size.
    // printf(" siz=%d %d\n",GR_ObjTab[l1].refInd,GR_ObjTab[l1].attInd);
    if(mx < tx) continue;                         // Maus links vom Label
    if(mx > (tx+sx)) continue;  // Maus rechts vom Label
    if(my < ty) continue;  // Maus unterhalb vom Label
    if(my > (ty+sy)) continue;  // Maus oberhalb vom Label


    // add selection
      // printf(" add1 sel dli=%d\n",l1);

    // printf(" IIIIIIIIIIIIII found N%d IIIIIIIIII %d %d\n",
             // GR_ObjTab[l1].ind,dx,dy);
    // check if selction already defined; passiert bei Tags;
    // Punkt und Tag (hier!) werden gefunden !
    for(i1=0; i1<iNr; ++i1) {
        // printf(" dl[%d]=%d\n",i1,(*dlTab)[i1].dlInd);
      if((*dlTab)[i1].dlInd == l1) goto L_9;                      // 2010-10-21
      // if(indTab[i1] != GR_ObjTab[l1].ind) continue;
      // if(AP_typ_2_bastyp(typTab[i1]) != Typ_GTXT) continue;
      // typ = (*dlTab)[i1].typ;
        // printf(" typ=%d\n",typ);
      // if(AP_typ_2_bastyp(typ) != Typ_Note) continue;        // 2009-12-31
      // printf(" double sel.typ=%d ind=%d\n",typTab[i1],indTab[i1]);
      // goto L_9;
    }

      // printf("- DL_txtSelect add2 sel dli=%d\n",l1);
    // if(iNr < 0) iNr = 0;
    // if(iNr > 50) break;
    iNr = GL_sel_add_DL (dlTab, l1);
    // typTab[iNr] = Typ_Tag;
    // indTab[iNr] = GR_ObjTab[l1].ind;
    // dlTab[iNr]  = l1;
    // ++iNr;

    L_9:
    continue;
  }


  // printf("ex DL_txtSelect %d\n",iNr);
  // for(l1=0; l1<iNr; ++l1)
    // printf(" typ=%d ind=%d dl=%d\n",typTab[l1],indTab[l1],dlTab[l1]);
    // GL_sel_dump (iNr);

  return iNr;

}


//====================================================================
  // int DL_IactSelect (int iNr, int *typTab, long *indTab, long *dlTab) {
  int DL_IactSelect (int iNr, ObjDB **dlTab) {
//====================================================================
/// check if Interactivity is connected to one of the selected objects


  int      i1, iEnd, irc;
  Activity *act1;


  // printf("DL_IactSelect %d\n",iNr);


  iEnd = iNr; // iNr wird veraendert !
  for(i1=0; i1<iEnd; ++i1) {
    // printf("   _IactSelect [%d] %d %d\n",i1,typTab[i1],indTab[i1]);

    // query DB, if a interactivity is connected to this obj
    // irc = DB_QueryActiv (&act1, typTab[i1], indTab[i1]);
    irc = DB_QueryActiv (&act1, (*dlTab)[i1].typ, (*dlTab)[i1].dbInd);
    if(irc < 0) continue;

    // printf("DL_IactSelect sel |%s|\n",act1->data);


    // add selection
    // printf(" IIIIIIIIIIIIII found I%d IIIIIIIIII %d %d\n",
             // GR_ObjTab[l1].ind,dx,dy);
    iNr = GL_sel_add_DB (dlTab, irc, Typ_Activ);
/*
    if(iNr < 0) iNr = 0;
    if(iNr > 50) break;
    typTab[iNr] = Typ_Activ;
    indTab[iNr] = irc;
    dlTab[iNr]  = -1;
    ++iNr;
*/
  }

  return iNr;

}


//================================================================
  int DL_setTagSiz (long dli, int xsiz, int ysiz) {
//================================================================
/// bei tags und Bitmaps die size im DL-record speichern

  uni_i4i2  *ia;

  // printf("DL_setTagSiz %d %d %d\n",dli,xsiz,ysiz);

  if(dli < 0) return -1;       // skip temporary objects

  if(dli > GR_TAB_IND) {TX_Error("DL_setTagSiz E002"); return -1;}

 // Tag/Image benutzt .attInd fuer sx und sy.
 // new long .attInd == eine Union aus 2 shorts; s1 und s2
 // Fuer jedes Tag/Image ein ObjAtt anlegen;
 // gu.i4 mit uni_i4i2 fuer sx/sy benutzen.

  ia = (void*)&(GR_ObjTab[dli].iatt);
  ia->i2[0] = xsiz;
  ia->i2[1] = ysiz;


  // GR_ObjTab[dli].refInd = xsiz;
  // GR_ObjTab[dli].attInd = ysiz;

  return 0;

}
 

//===============================================================
  int DL_Stat () {
//===============================================================
/// ausgabe Statistik

  printf("DL_Stat \n");

  TX_Print(" DL used %d von %d",GR_TAB_IND,GR_TAB_SIZ);

  // printf(" GL_ActInd=%d\n",GL_GetActInd());

  return 0;

}


//===============================================================
  void DL_disp_def (int mode) {
//===============================================================
/// \code
/// fuer alle nun folgenden Obj GR_ObjTab.disp=mode setzen.
/// Geht nur, wenn DispListIndex mit DL_StoreObj generiert wird !
/// \endcode

// mode = ON(0) oder OFF(1)



  // printf("DL_set_disp %d %d\n",ind,mode);
  DL_disp_act = mode;


}


//===============================================================
  int DL_sav_dynDat () {
//===============================================================
/// save the DYNAMIC_DATA of the actual mainModel


  char   fnam[256];
  FILE   *fp1;
  double view[10];


  // printf("DL_sav_dynDat\n");


  sprintf(fnam, "%sdyn.dat",OS_get_tmp_dir());

  // Achtung: fuer MS MUSS write und read BINARY sein; sonst kommt Mist !!!
  if((fp1=fopen(fnam,"wb")) == NULL) {
    TX_Print("DL_sav_dynDat E001\n",fnam);
    return -1;
  }

  // save view
  GL_View_get(view);
  fwrite(view, sizeof(double), 10, fp1);

  // save MODSIZ / APT_ModSiz UT_TOL_cv UT_DISP_cv
  fwrite(&APT_ModSiz, sizeof(double), 1, fp1);
  fwrite(&UT_TOL_cv, sizeof(double), 1, fp1);
  fwrite(&UT_DISP_cv, sizeof(double), 1, fp1);

  // save Textparameters DEFTX
  fwrite(&AP_txsiz, sizeof(double), 1, fp1);
  fwrite(&AP_txdimsiz, sizeof(double), 1, fp1);
  fwrite(&GR_tx_scale, sizeof(double), 1, fp1);
  fwrite(&AP_txNkNr, sizeof(int), 1, fp1);

  // save DEFCOL
  fwrite(&AP_defcol, sizeof(ColRGB), 1, fp1);

  // save hidetable
  GA_hide_fil__ (1, fp1);

  fclose(fp1);


  return 0;

}


//===============================================================
  int DL_load_dynDat () {
//===============================================================
/// reload the DYNAMIC_DATA of the actual mainModel


  char   fnam[256];
  FILE   *fp1;
  double view[10], da[3];

  // printf("DL_load_dynDat\n");

  sprintf(fnam, "%sdyn.dat",OS_get_tmp_dir());

  // Achtung: fuer MS MUSS write und read BINARY sein; sonst kommt Mist !!!
  if((fp1=fopen(fnam,"rb")) == NULL) {
    TX_Print("DL_load_dynDat E001\n",fnam);
    return -1;
  }

  
  // load view
  fread(view, sizeof(double), 10, fp1);

  // load MODSIZ / APT_ModSiz UT_TOL_cv UT_DISP_cv
  fread(da, sizeof(double), 3, fp1);

  // load Textparameters
  fread(&AP_txsiz, sizeof(double), 1, fp1);
  fread(&AP_txdimsiz, sizeof(double), 1, fp1);
  fread(&GR_tx_scale, sizeof(double), 1, fp1);
  fread(&AP_txNkNr, sizeof(int), 1, fp1);

  // load DEFCOL
  fread(&AP_defcol, sizeof(ColRGB), 1, fp1);

  // load hidetable
  GA_hide_fil__ (2, fp1);


  fclose(fp1);



  // execute
  GL_View_set (view);
  // UI_AP (UI_FuncSet, UID_ouf_vwz, (void*)&view[5]); // in box raus

  APT_ModSiz = da[0];
  GL_InitModelSize (APT_ModSiz, 1);
  UT_TOL_cv  = da[1];
  UT_DISP_cv = da[2];

  // printf("ex DL_load_dynDat UT_DISP_cv=%f\n",UT_DISP_cv);

  return 0;

}


//===============================================================
  int DL_wri_dynDat0 (FILE *fpo) {
//===============================================================
/// write :DYNAMIC_DATA

  int i1;
  // double view[10];

  // printf("DL_wri_dynDat0 \n");

  // write MODSIZ DEFTX DEFCOL VIEW CONST_PL  :DYNAMIC_DATA
  DL_wri_dynDat1 (fpo);

  // add GA-Table (HIDE/G#/..)
  GA_fil_wri (fpo, 0, 0, 1);

  fprintf(fpo, ":DYNAMIC_DATA\n");

  // printf("ex DL_wri_dynDat0 \n");

  return 0;

}


//===============================================================
  int DL_wri_dynDat1 (FILE *fpo) {
//===============================================================
/// \code
/// write MODSIZ DEFTX DEFCOL VIEW CONST_PL  :DYNAMIC_DATA
/// add :DYNAMIC_DATA  without HIDE
/// \endcode

  int i1;
  double view[10];

  // printf("DL_wri_dynDat \n");



  // add Tolerances
    strcpy(mem_cbuf1, "MODSIZ");
    AP_obj_add_val (mem_cbuf1, APT_ModSiz);
    AP_obj_add_val (mem_cbuf1, UT_TOL_cv);
    AP_obj_add_val (mem_cbuf1, UT_DISP_cv);
    // UTF_add1_line (mem_cbuf1);
    fprintf(fpo, "%s\n", mem_cbuf1);

  // add Texsizes
    strcpy(mem_cbuf1, "DEFTX");
    AP_obj_add_val (mem_cbuf1, AP_txsiz);
    AP_obj_add_val (mem_cbuf1, AP_txdimsiz);
    AP_obj_add_val (mem_cbuf1, GR_tx_scale);
    AP_obj_add_int (mem_cbuf1, AP_txNkNr);
    fprintf(fpo, "%s\n", mem_cbuf1);

  // add DefaultColor
    sprintf(mem_cbuf1, "DEFCOL %d %d %d",
      AP_defcol.cr,AP_defcol.cg,AP_defcol.cb);
    fprintf(fpo, "%s\n", mem_cbuf1);


  // add VIEW-parameter
    // query Viewparameters (Scale, Center, ..)
    AP_view_2_txt (mem_cbuf1);
    fprintf(fpo, "%s\n", mem_cbuf1);


  // add ConstructionPlane
    strcpy(mem_cbuf1, "CONST_PL");
    AP_obj_add_pt (mem_cbuf1, &WC_sur_act.po);
    AP_obj_add_vc (mem_cbuf1, &WC_sur_act.vx);
    AP_obj_add_vc (mem_cbuf1, &WC_sur_act.vy);
    AP_obj_add_vc (mem_cbuf1, &WC_sur_act.vz);
    AP_obj_add_val (mem_cbuf1, WC_sur_Z);
    strcat(mem_cbuf1, " ");
    strcat(mem_cbuf1, AP_Get_ConstPl_Z(Typ_PLN));
    // UTF_add1_line (mem_cbuf1);
    fprintf(fpo, "%s\n", mem_cbuf1);


  // save hide-points
  if(APT_dispPT == OFF) {
    fprintf(fpo, "MODE DISP_PT OFF\n");
  }


  // save hide-planes
  if(APT_dispPL == OFF) {
    fprintf(fpo, "MODE DISP_PL OFF\n");
  }


  return 0;

}

/*
//===============================================================
  int DL_save_DYNAMIC_AREA () {
//===============================================================
// alte version; ersetzt durch DL_wri_dynDat
  
  int    i1;
  double view[10];


  // printf("SSSSSSSSSSSSSSS DL_save_DYNAMIC_AREA\n");


  // Zwischenbuffer loeschen
  UTF_clear1();

  // Block "DYNAMIC_AREA" loeschen
  UTF_del_rest(":DYNAMIC_AREA");

  // add "DYNAMIC_AREA:"
  UTF_add1_line (":DYNAMIC_AREA");


  // add Tolerances
    strcpy(mem_cbuf1, "MODSIZ");
    AP_obj_add_val (mem_cbuf1, APT_ModSiz);
    AP_obj_add_val (mem_cbuf1, UT_TOL_cv);
    AP_obj_add_val (mem_cbuf1, UT_DISP_cv);
    UTF_add1_line (mem_cbuf1);


  // add VIEW-parameter
    // query Viewparameters (Scale, Center, ..)
    strcpy(mem_cbuf1, "VIEW");
    GL_View_get(view);
    for(i1=0; i1<10; ++i1) {
      AP_obj_add_val (mem_cbuf1, view[i1]);
    }
    UTF_add1_line (mem_cbuf1);


  // add ConstructionPlane
    strcpy(mem_cbuf1, "CONST_PL");
    AP_obj_add_pt (mem_cbuf1, &WC_sur_act.po);
    AP_obj_add_vc (mem_cbuf1, &WC_sur_act.vx);
    AP_obj_add_vc (mem_cbuf1, &WC_sur_act.vy);
    AP_obj_add_vc (mem_cbuf1, &WC_sur_act.vz);
    AP_obj_add_val (mem_cbuf1, WC_sur_Z);
    strcat(mem_cbuf1, " ");
    strcat(mem_cbuf1, AP_Get_ConstPl_Z(Typ_PLN));
    UTF_add1_line (mem_cbuf1);


  // add hidden objects
  GA_hide__ (2, 0, 0);

  // Zwischenbuffer im Textfenster hintanfuegen
  // UTF_add_nl ();  // add \n als last char 
  UTF_insert1(-1);

  return 0;
}
*/


//===============================================================
  void DL_hide__ (long ind, int mode) {
//===============================================================
/// \code
/// DL_hide__     change hidden/visible for single obj
/// mode = ON(0) oder OFF(1,hide)
/// modify DL.disp, DL.hili
/// \endcode

///        hidden = ((disp == 1)&&(hili == 1))            // ON=0   OFF=1


// zB:
// DL_set_disp (GL_GetActInd(), OFF);


  // printf("DL_hide__ dli=%ld mode=%d\n",ind,mode);
  // if(ind == 25L) gdb_halt ();


  GR_ObjTab[ind].disp = mode;   // ON=0=view; OFF=1=hide

  // wenn obj gehilitet ist, muss das zurueckgesetzt werden !
  // (gehilitete werden in eigener Schleife gezeichnet)
  if(mode == OFF) {                         // OFF=1
    // make hidden: reset also hili (else displayed hilited)
    GR_ObjTab[ind].hili  = OFF;
  }

}


//===============================================================
  int DL_hide_all () {
//===============================================================
/// DL_hide_all      change hidden/visible for all objs

// ON=0   OFF=1

  long  ind;


  // printf("DL_hide_all\n");


  for(ind=0; ind<GR_TAB_IND; ++ind) {

    if(GR_ObjTab[ind].disp == OFF) {
      // make visible
      GR_ObjTab[ind].disp  = ON;
    } else {
      // make hidden
      GR_ObjTab[ind].disp  = OFF;
    }
  }

  return 0;

}


//================================================================
  int DL_disp_PL (int mode) {
//================================================================
// DL_disp_PL                view or hide all planes
//   mode                    0=view, 1=hide.
// see also DL_hide_unvisTypes


  long      ind;


  // printf("DL_disp_PL %d\n",mode);


  for(ind=0; ind<GR_TAB_IND; ++ind) {

    // // skip subModels
    // if((signed short)GR_ObjTab[ind].modInd != -1) continue;

    // skip all but Typ_PLN
    if(GR_ObjTab[ind].typ != Typ_PLN)  continue;

    GR_ObjTab[ind].disp = mode;  // 0=view, 1=hide.
    GR_ObjTab[ind].hili = 1;     // hili OFF; else cannot be hidden ..
  }

  return 0;

}


//================================================================
  int DL_hide_unvisTypes (int mode) {
//================================================================
// DL_hide_unvisTypes        view or hide all joints,activities.
//   mode        0=view, 1=hide.

// VWR:         1   hide joints & activities; hide planes if APT_dispPL=OFF
// CAD & MAN:   0   view all

// see also DL_disp_PL

  long      ind;


  // printf("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD \n");
  // printf("DL_hide_unvisTypes %d\n",mode);


  if(mode == 0) {      // view
    GL_InitPtAtt (0);

  } else {             // hide
    if(APT_dispPT == OFF) GL_InitPtAtt (1);

  }


  for(ind=0; ind<GR_TAB_IND; ++ind) {

    if(APT_dispPL == OFF) {
      if(GR_ObjTab[ind].typ == Typ_PLN) {
        GR_ObjTab[ind].disp = mode;     
        GR_ObjTab[ind].hili = 1;     // else cannot be hidden ..
        continue;
      }
    }

    // skip subModels
    if((signed short)GR_ObjTab[ind].modInd != -1) continue;

    // skip all but Typ_Activ,Typ_Joint
    if((GR_ObjTab[ind].typ != Typ_Activ)  &&
       (GR_ObjTab[ind].typ != Typ_Joint))       continue;

    GR_ObjTab[ind].disp = mode;  // 0=view, 1=hide.
    GR_ObjTab[ind].hili = 1;     // else cannot be hidden ..
  }

  return 0;

}


/*
//================================================================
  int DL_unvis__ (long i1, long iNr) {
//================================================================
/// alle objekte von i1 unvisible machen ...

  long  i2, ind;

  i2 = i1 + iNr;

  for(ind=i1; ind<i2; ++ind) {
    // DL_unvis_set (dli, 1);
    GR_ObjTab[ind].unvis = 1;
  }
  return 0;

}
*/

 
//===============================================================
  int DL_unvis_set (long ind, int mode) {
//===============================================================
/// \code
/// unvis: 0 = visible; 1 = obj is unvisible
/// set objects with no visual representation to unvisible;
///   eg joints, application-data, but not curves, surfaces, ..
/// \endcode


  // printf("UUUUUUUUUUUUUUUUUUUUUUUUU\n");
  // printf("DL_unvis_set %ld %d\n",ind,mode);

  GR_ObjTab[ind].unvis = mode;

  return 0;

}

//=====================================================================
  int DL_hili_on (long ind) {
//=====================================================================
/// \code
/// DL_hili_on              set obj hilited
/// ind >= 0: Hili Obj.
/// ind = -1: gesamte Hili-Tabelle loeschen
/// ind = -2: das zuletzt bearb. Elem. hiliten
/// RetCod:
///   -1      hidden object; not hilited ..
///
/// No Redraw !
/// Ein schon vorhandenes Objekt hiliten.
/// ObjID merken fuer Redraw.
/// Hier wird zusätzlich das DISP-Bit auf OFF gesetzt,
/// um das Obj nicht doppelt zu zeichnen. 
/// \endcode



  long l1;


  // printf("DL_hili_on %ld von %ld\n",ind,GR_TAB_IND);
  // printf(" hili=%d disp=%d\n",GR_ObjTab[ind].hili,GR_ObjTab[ind].disp);
  // printf(" typ=%d\n",GR_ObjTab[ind].typ);


  if(ind >= 0) {         // Hili Obj.
    if(ind >= GR_TAB_IND) ind = GR_TAB_IND-1;

    // ignore ApplicationObj's
    if(GR_ObjTab[ind].typ == Typ_APPOBJ) return 0;      //2011-02-16

    // do not hilite if hidden !   2010-10-10
    if((GR_ObjTab[ind].hili == 1)&&(GR_ObjTab[ind].disp == 1)) return -1;


    GR_ObjTab[ind].hili  = ON;   // ON=0
    GR_ObjTab[ind].disp  = OFF;  // OFF=1

    // // ACHTUNG: Parentobjects haben defektes iatt !!
    // if(GR_ObjTab[ind].sPar == 1) {
      // l1 = GR_ObjTab[ind].iatt;
      // GR_ObjTab[ind].iatt = GR_ObjTab[l1].iatt;
      // GR_ObjTab[ind].sPar = 0;
    // }

    return 0;
  }



  if(ind == -1) {         // Tabelle löschen
    DL_hili_off (ind);
    return 0;
  }



  if(ind == -2) {         // das zuletzt gearbeitete Obj hiliten
    if(GR_TAB_IND > 0) {
      l1 = GR_TAB_IND - 1;

      // test for VC
      if(GR_ObjTab[l1].typ == Typ_VC) {
        UI_disp_vec1 (Typ_Index, (void*)GR_ObjTab[l1].ind, NULL);
      } else {
        // printf("  _HiliObj l1=%d\n",l1);
        GR_ObjTab[l1].hili  = ON;    // hi=0; di=1;
        GR_ObjTab[l1].disp  = OFF;
      }
    }
    return 0;
  }


  printf("DL_hili_on Err %ld\n",ind);

  return -2;

}


//=====================================================================
  int DL_hili_off (long ind) {
//=====================================================================
/// \code
/// DL_hili_off             reset hilited
/// ind >= 0: unhilite single obj
/// ind = -1: unhilite all hilited objects
///           returns nr of hilited objs
/// ind = -2: das zuletzt bearb. Elem. unhiliten
///
/// No Redraw !
/// see also GL_temp_Delete GL_temp_delete
/// \endcode


  int   ii;
  long  l1;


  // printf("DL_hili_off %ld\n",ind);
  // if(ind>=0)printf(" hili=%d disp=%d\n",GR_ObjTab[ind].hili,GR_ObjTab[ind].disp);



  if(ind >= 0) {         // UnHili Obj.
    GR_ObjTab[ind].hili  = OFF;   // OFF=1
    GR_ObjTab[ind].disp  = ON;    // ON=0
    return 0;
  }




  if(ind == -1) {         // Tabelle löschen
    ii = 0;
    for(l1=0; l1<GR_TAB_IND; ++l1) {
      //TX_Print("reset hili=%d disp=%d",GR_ObjTab[l1].hili,GR_ObjTab[l1].disp);
      if(GR_ObjTab[l1].hili == ON) {
        GR_ObjTab[l1].hili  = OFF;
        // hier sollte man nachsehen ob Layer ueberhaupt aktiv ist usw !!
        GR_ObjTab[l1].disp  = ON;
        ++ii;
      }
    }
    return ii;
  }


  if(ind == -2) {         // das zuletzt gearbeitete Obj unhiliten
    l1 = GR_TAB_IND-1;
      // printf("DL_hili_off  -2 %ld\n",l1);
    if(l1 >= 0) {
      GR_ObjTab[l1].hili  = OFF;
      GR_ObjTab[l1].disp  = ON;
    }
    return 0;
  }

    // printf("DL_hili_off Err %ld\n",ind);

  return -1;

}



//===============================================================
  int DL_pick__ (int mode) {
//===============================================================
/// \code
/// DL_pick__               change all objects to pickable or unpickable
/// pickable=1;   unpickable=0.
/// \endcode

  long  l1;


  // printf("DL_pick__ %d\n",mode);

  // sele_reset ();  // selectFiletr nothing ..
  // removed 2010-09-11; for modify texure 

  for(l1=0; l1<GR_TAB_IND; ++l1) {
    GR_ObjTab[l1].pick = mode;
  }


  return 0;
}


//===============================================================
  int DL_pick_typ (int typ, int mode) {
//===============================================================
/// \code
/// DL_pick_typ             change pickability of all objects of typ <typ>
/// pickable=1;   unpickable=0.
/// \endcode

  long  l1;


  // printf("DL_pick_typ %d\n",mode);

  for(l1=0; l1<GR_TAB_IND; ++l1) {
    if(GR_ObjTab[l1].typ  != typ) continue;
    GR_ObjTab[l1].pick = mode;
  }


  return 0;
}


//===============================================================
  int DL_pick_set (long ind, int mode) {
//===============================================================
/// \code
/// Selektierbarkeit; ON od OFF.
/// Default == pickable == 1;   not pickable==0.
/// ind=-1: modify last object of DispList
/// \endcode


  // printf("DL_pick_set %d %d\n",ind,mode);

  if(ind < 0) {
    ind = GR_TAB_IND - 1;
  }

  GR_ObjTab[ind].pick = mode;

  return 0;
}


//===============================================================
  int DL_grp1__ (long ind, char *cbuf1, int mode, int iUpd) {
//===============================================================
/// \code
/// add / remove (change) Groupbit 1 of DL-Record ind
/// Input:
///   ind       DispListIndex of obj
///   cbuf1     Infotext; NULL for silent
///   mode      0=switch; 1=add to group; -1=remove from group; 2=add all
///   iUpd      0=update display; 1=do not update display (yet)
/// \endcode


  long    l1;
  unsigned short modnr;


  // printf("DL_grp1__ dli=%ld mode=%d %d\n",ind,mode,iUpd);


  if(mode == 0) {
    if(GR_ObjTab[ind].grp_1 == ON) mode = -1;    // remove from group
    else                           mode = 1;     // add to group
  }


  //----------------------------------------------------------------
  if(mode < 0) {                                 // remove from group
    // DL_grp1_set (ind, OFF);
    GR_ObjTab[ind].grp_1 = OFF;
    DL_hili_off (-1L);             // unhilte (after selection necessary)
    if(cbuf1) TX_Print ("remove obj %s from group",cbuf1);
    Grp_del (GR_ObjTab[ind].typ, GR_ObjTab[ind].ind, iUpd);


  //----------------------------------------------------------------
  } else if(mode == 1) {                           // add to group
    // DL_grp1_set (ind, ON);
    GR_ObjTab[ind].grp_1 = ON;
    if(cbuf1) TX_Print ("add obj %s to group",cbuf1);
    Grp_add__ (GR_ObjTab[ind].typ, GR_ObjTab[ind].ind, ind, iUpd);


  //----------------------------------------------------------------
  } else if(mode == 2) {                           // add all to group
    // loop tru DL
    // if(cbuf1)
    TX_Print ("add all visible objs to group");
    modnr = WC_modnr;
    for(l1=0; l1<GR_TAB_IND; ++l1) {
      // skip hidden
      if((GR_ObjTab[l1].disp==1)&&(GR_ObjTab[l1].hili == 1)) continue;
      // skip subModels
      if(GR_ObjTab[l1].modInd != modnr) continue;
      // skip deleted obj
      if(GR_ObjTab[l1].typ    == 0) continue;
      // add ..
      GR_ObjTab[l1].grp_1 = ON;
      Grp_add__ (GR_ObjTab[l1].typ, GR_ObjTab[l1].ind, l1, 0);
    }
    Grp_upd (1);  // update GrpNr-label
    // Grp_dump ();
  }

  //----------------------------------------------------------------
  if(iUpd == 0) DL_Redraw (); // hilite / unhilite

  return 0;
}


//===============================================================
  int DL_grp1_set (long ind, int mode) {
//===============================================================
/// \code
/// Zugehoerigkeit zu Group 1; ON od OFF.
/// Default == nicht_zugehoerig == OFF == 1;   zugehoerig == ON == 0.
///   ind       -1    all objects
/// \endcode

  long    i1;


  // printf("DL_grp1_set ind=%ld mode=%d %d\n",ind,mode,GR_ATT_TAB_SIZ);


  if(ind >= 0) {
    GR_ObjTab[ind].grp_1 = mode;
    return 0;
  }


  // ind < 0: set alle groupBits.
  for(i1=0; i1<GR_TAB_IND; ++i1) {
    GR_ObjTab[i1].grp_1  = mode;
    // printf(" gr[%d]=%d\n",i1,GR_ObjTab[i1].grp_1);
  }

  Grp_init();

  return 0;
}


//================================================================
  int DL_grp1_get (long dli) {
//================================================================
// DL_grp1_get             get grp_1-bit


  return GR_ObjTab[dli].grp_1;

}

  
//================================================================
  int DL_grp1_copy () {
//================================================================
/// copy all DL-obj with groupBit ON --> GroupList.

  int     typ;
  long    i1;

  // printf("GGGGGGG DL_grp1_copy GGGGGGGGG \n");


  Grp_init (); // init obj-list

  for(i1=0; i1<GR_TAB_IND; ++i1) {
    if(GR_ObjTab[i1].grp_1 == OFF) continue;

    typ = AP_typ_2_bastyp (GR_ObjTab[i1].typ);
    Grp_add__ (typ, GR_ObjTab[i1].ind, i1, 1);
  }
  Grp_upd (1);  // update GrpNr-label

  return 0;

}


//=======================================================================
  int DL_alloc__ (long Ind) {
//=======================================================================
/// \code
/// realloc space fuer DL; wird nicht gesichert !
/// Ind = 0      Reset DL
/// Ind = 1      increase DL (add GR_TAB_INC elements)
/// Ind > 1      set size
/// \endcode

  long   i1, newSiz;


  // Reset DL
  if((Ind == 0)&&(GR_ObjTab != NULL)) {
    // printf("DL_alloc__ reset\n");
    GL_Init1 ();    // GR_TAB_IND = 0;
    return 0;
  }


  newSiz = GR_TAB_SIZ + GR_TAB_INC;

  if(Ind > 1) {
    if(Ind < GR_TAB_SIZ) return 0;
    while (Ind >= newSiz) newSiz += GR_TAB_INC;
  }

  printf("::::DL_alloc__ %ld\n",newSiz);

  GR_ObjTab = (DL_Att*)realloc((void*)GR_ObjTab, newSiz*sizeof(DL_Att));

  if(GR_ObjTab == NULL) {
    TX_Error ("******** out of memory - DL_alloc__ *********");
    return -1;
  }
  GR_TAB_SIZ = newSiz;


  return 0;

}


//============================================================
   void DL_Init () {
//============================================================


  // printf("LLLLLLLLLLLLLL DL_Init LLLLLLLLLLLLLLLL\n");

  GL_InitCol (10);


  // Nicht im "store-tesselated-Vertices"-Modus
  if(TSU_get_mode() != 0) return;


  DL_disp_act = ON;


  DL_alloc__ (0L);


  DL_InitAttTab ();

  // GA_parent_init (); in GR_Init1


  /*  Auch die DispList löschen */
  GR_Init1 ();


}


//================================================================
  int DL_GetAttNr () {
//================================================================
/// Anzahl definierter Attribute
 
  return GR_AttInd;

}


//================================================================
  int AttLn_Get_ind (int *col, int *dash, int *thick, int ii) {
//================================================================
/// \code
/// AttLn_Get_ind          get line-attribute from GR_AttLnTab-index 
/// Input:
///   col      3 digits RGB
///   dash     0=full, 1=dash-dot, 2=shortDash, 3=longDash
///   thick    1-6, thickness in pixels
///
/// defaultvalues from ~/gCAD3D/cfg/ltyp.rc
/// \endcode


  Att_ln     *att;


  if((ii < 0)||(ii >= GR_ATT_TAB_SIZ)) return -1;


  att = &GR_AttLnTab[ii];

  *col   = (att->cr * 100) + (att->cg * 10) + att->cb;
  *dash  = att->dash;
  *thick = att->thick;

    // printf("AttLn_Get1 %d %d %d\n",*col,*dash,*thick,ii);
  return 0;

}


//================================================================
  int AttLn_Set1 (Att_ln *atto, int col, int dash, int thick) {
//================================================================
/// \code
/// AttLn_Set1          get line-attribute from 3 integers
/// Input:
///   col      3 digits RGB
///   dash     0=full, 1=dash-dot, 2=shortDash, 3=longDash
///   thick    1-6, thickness in pixels
///
/// defaultvalues from ~/gCAD3D/cfg/ltyp.rc
/// \endcode

  int      ia[3];

  // printf("AttLn_Set1 col=%d dash=%d thick=%d\n",col,dash,thick);

  if((col < 0)||(col > 999)) goto L_err;
  if((dash < 0)||(dash > 3)) goto L_err;
  if((thick < 0)||(thick > 6)) goto L_err;

  UTI_ndig_int (ia, 3, col);

  atto->cr     = ia[2];
  atto->cg     = ia[1];
  atto->cb     = ia[0];
  atto->dash   = dash;
  atto->thick  = thick;

  return 0;

  L_err:
    TX_Error("AttLn_Set1 parametererror");
    return -1;

}


//================================================================
  int IndAttLn_get_ltyp (void *att1) {
//================================================================
/// IndAttLn_get_ltyp      get linetyp

  return (((Ind_Att_ln*)att1)->indAtt);

}


//================================================================
  int IndAttLn_Set1 (Ind_Att_ln *att1, int ltyp, int lim) {
//================================================================
/// \code
/// IndAttLn_Set1           set linetyp, limitTyp
/// Input:
///   ltyp        do not change if -1
///   lim         do not change if -1
/// \endcode

// see also GL_Redraw - hili


  printf("IndAttLn_Set1  %d %d\n",ltyp,lim);


  if(ltyp >= 0) att1->indAtt = ltyp;

  if(lim >= 0) att1->lim = lim;

  return 0;

}


//================================================================
  int DL_AttLn_store (int attInd, Att_ln *atto) {
//================================================================
/// DL_AttLn_store                store line-attribute in table GR_AttLnTab


  if((attInd < 0)||(attInd >= GR_ATT_TAB_SIZ)) {
    TX_Error("DL_AttLn_store E001 %d",attInd);
    return -1;
  }

  GR_AttLnTab[attInd] = *atto;

  return 0;

}


//============================================================
   void DL_InitAttTab () {
//============================================================
/// \code
/// DL_InitAttTab       load file ltyp.rc into GR_AttLnTab and create DL-record
/// Create line-attributes. File is  ~/gCAD3D/cfg/ltyp.rc
/// \endcode

//  wird schon früher als DL_Init gebraucht !!!

  int    i1, i2, i3, i4, irc;
  char   cbuf1[256], *p1, *p2;
  Att_ln att1;
  FILE   *fp1;


  // printf("DDDDDDDDDDDDDDDDDD DL_InitAttTab DDDDDDDDDDDDDDDDDDDDD \n");

  i2 = 0;
  for(i1=0; i1<GR_ATT_TAB_SIZ; ++i1) {
    memcpy (&GR_AttLnTab[i1], &i2, sizeof(Att_ln));
  }

  GR_AttInd = 0;


  // try to open inListe
  sprintf(cbuf1,"%sltyp.rc",OS_get_cfg_dir());

  if((fp1=fopen(cbuf1,"r")) == NULL) {
    TX_Print("***** DL_InitAttTab E001 %s",cbuf1);
    return;
  }


  while (!feof (fp1)) {
    if (fgets (cbuf1, 252, fp1) == NULL) break;
    if(cbuf1[0] == '#') continue;
      // printf(" ltyp=|%s|\n",cbuf1);

    i1 = strtol(cbuf1, &p1, 10); // ind
    i2 = strtol(p1, &p2, 10);    // col
    i3 = strtol(p2, &p1, 10);    // typ
    i4 = strtol(p1, &p2, 10);    // thick
      // printf(" ltyp=%d %d %d %d\n",i1,i2,i3,i4);

    // load attr. -> att1
    // store att1 -> GR_AttLnTab
    // activate DL-record
    DL_InitAttRec (i1, i2, i3, i4);

  }

  fclose(fp1);

  return;
}



//============================================================
  int DL_InitAttRec (int ind, int col, int typ, int thick) {
//============================================================
/// \code
/// DL_InitAttRec              define a new lineattributeRecord
/// Input:
///   ind    Recordnr; first=0; use as attInd in GL_DrawLine
///   col    color 2=red, 3=green, 4=blue, 5=yellow
///   ltyp   0=full, 1=dash-dot, 2=shortDash, 3=longDash
///   lthick 1-6, thickness in pixels
///
/// defaultvalues from <tempDir>/ltyp.rc    
/// \endcode

// colors: ../cfg/ltyp.rc
//  wird schon vor DL_Init gebraucht !!!


  int    irc;
  Att_ln att1;


  if((ind < 0)||(ind >= GR_ATT_TAB_SIZ)) {
    TX_Error("DL_InitAttRec E001 %d",ind);
    return -1;
  }


  if(GR_AttInd <= ind) GR_AttInd = ind + 1;



    // load attr. -> att1
    AttLn_Set1 (&att1, col, typ, thick);

    // store att1 -> GR_AttLnTab
    irc = DL_AttLn_store (ind, &att1);
    if(irc < 0) return -1;

    // activate GR_AttLnTab
    GL_InitNewAtt (0, ind);

  return 0;

}


/*
//==============================================================
  int DL_tmp_Redraw () {
//==============================================================
// redraw temp DL.  GEHT NED ..

  // printf("DL_Redraw\n");

  UI_GR_DrawInit ();
  GL_tmp_Redraw ();
  UI_GR_DrawExit ();

  return 0;

}
*/

 
//================================================================
  int DL_Draw_obj (long dli, int typ, long dbi) {
//================================================================
// redraw single obj, use existing DL-record


  // modify (do not create new DL-Record); set DL_ind_act=dli;
  DL_SetInd (dli);

  // call GL-Draw (with modified attribute iatt) ..
  return APT_Draw__ (GR_ObjTab[dli].iatt, typ, dbi);

}

 
//==============================================================
  int DL_Redraw () {
//==============================================================
// redraw complete DL.

  // printf("DL_Redraw\n");

  if(AP_stat.sysStat < 2) return 0;

  // UI_GR_setKeyFocus ();       // set focus to glarea widget 2011-01-28
// 
  // nimmt den focus von den CAD-Eingabefeldern weg ...
  // UI_block_glEvents (1);
  UI_GR_DrawInit ();
  GL_Redraw ();
  UI_GR_DrawExit ();
  // UI_block_glEvents (0);


  // focus back to editor or CAD-input; see UI_GR_Draw

  return 0;

}


//================================================================
  int DL_SetLnr (long dli, long lNr) {
//================================================================
/// Overwrite the APT-LinNr bei Modify

  if((dli < 0)||(dli > GR_TAB_IND)) {
    printf("DL_SetLnr I001\n");
    return 0;
  }


  GR_ObjTab[dli].lNr = lNr;

  return 0;

}


//================================================================
  int DL_SetInd (long dli) {
//================================================================
/// \code
/// -1 = default = create new DL-Record.
/// if(dli >= 0): DL_StoreObj does not creat new DL-Record !! (modify object)
/// DL_SetInd (-1L); // to reset (after GL_fix_DL_ind )
///  DL_StoreObj restores to -1
/// \endcode

  // printf("########################## DL_SetInd %ld\n",dli);

  DL_ind_act = dli;

  return 0;

}
 

//============================================================
   long DL_StoreObj (int Typ, long DBInd, int AttInd) {
//============================================================
/// \code
/// Den naechsten freien DispList-Platz belegen mit Typ, DBInd, att ....
/// In der (hier lokalen) Displayliste, DBInd und Att speichern.
/// Wird benutzt von GL zum Redraw, beim Select.
/// if(DL_ind_act >= 0) use this (existing) DL-record; (preset with DL_SetInd)
/// 
/// Dynam. Objekte, die ueberhaupt nicht in der APT-DB gespeichert werden,
///  erhalten als DBInd den negativen DispList-Index.
/// 
///  DBInd  0:    definiert temp. Obj 8 
///  DBInd -1:    temp. Obj m ind. -GR_TAB_IND
/// 
//   AttInd   ?   2 ? 5 ?
/// 
///   dlInd = DL_StoreObj (Typ_LN, objInd, attInd);
///   // IN:  Objectindex objInd  und ObjTyp Typ_xx
///   // OUT: DispListIndex dlInd
/// 
/// see also:
///   DL_SetInd   preset DL-index (to modify DL-record)
/// \endcode


// Werden GL_-Routinen direkt gerufen (fuer Tests), kann man die hier
//   retournierte dlInd selbst weiterzaehlen.
//   zB: long dli=0; ++dli; GL_DrawPoint (&dli, 0, &pt1);



  long   dlInd;
  int    lNr, itemp;


  // printf("DL_StoreObj typ=%d DBind=%ld AttInd=%d - IND=%ld SIZ=%ld\n",
          // Typ, DBInd,AttInd,GR_TAB_IND,GR_TAB_SIZ);
  // printf("    DL_ind_act=%ld\n",DL_ind_act);
  // printf("    DLind=%d SIZ=%d\n",GR_TAB_IND,GR_TAB_SIZ);
  // printf("    WC_sur_ind=%d\n",WC_sur_ind);
  // printf("DL_StoreObj pick = %d\n",GR_pick);
  // if(Typ == Typ_Model) printf(" ***** Typ_Model %ld\n",DBInd);




  // realloc, wenn zu klein
  if(GR_TAB_IND >= GR_TAB_SIZ) {
    if(DL_alloc__ (1L) < 0) return -1;
  }



  if(DBInd == 0) {       // 2011-10-18
    // not for Typ_Ditto+Typ_Mock (GR_DrawModel)
    // - cannot hilite tempDLobjs
    if((Typ != Typ_Ditto)   && 
       (Typ != Typ_Model))         {
      dlInd = DLI_TMP;
      goto L_done;
    }
  }



  // dynam. Obj. erhalten die neg. DL-ID als DB-index in ihrem DL-Record.
  if(DBInd < 0) {
    DBInd = -GR_TAB_IND;
  }


  // ist DL-Rec bereits vorhanden ? Dann den DL-Index des existing Record
  // liefern; wurde von DL_SetInd() gesetzt !
  // printf(" DL_ind_act=%d\n",DL_ind_act);
  if(DL_ind_act >= 0) {
    // printf(" XXXXXXXXXXX overwrite; DL_ind_act=%d\n",DL_ind_act);
    dlInd = DL_ind_act;
    // GL_Del0 (dlInd);
    goto L_done;
  }


  // DBInd = 0: ein temp obj mit index 8 (INPRECANZ)
  // if(DBInd == 0)
  dlInd = GR_TAB_IND;


  // fix lNr
    // printf(" _StoreObj AP_mdLev=%d UP_level=%d\n",AP_mdLev,UP_level);
  if(AP_mdLev >= 0) {
    AP_mdGet (&lNr);

  } else if(UP_level >= 0) {
    lNr = APT_UP_get ();
    // printf(" calling lNr = %d\n",lNr);

  } else {
    lNr = APT_line_act;
  }


  ++GR_TAB_IND;

    // printf(" dlInd=%ld lNr=%d IND=%ld\n",dlInd,lNr,GR_TAB_IND);

    GR_ObjTab[dlInd].lNr    = lNr;           // die momentane APT-LineNr
    GR_ObjTab[dlInd].typ    = Typ;           // zB Typ_LN
    GR_ObjTab[dlInd].ind    = DBInd;
    GR_ObjTab[dlInd].iatt   = AttInd;

    GR_ObjTab[dlInd].irs    = WC_sur_ind;

    GR_ObjTab[dlInd].modInd = WC_modnr;     // (signed short)
    GR_ObjTab[dlInd].unvis  = 0;            // Default = visible
    GR_ObjTab[dlInd].sChd   = 0;            // Default = indep.
    GR_ObjTab[dlInd].sPar   = 0;            // Default = indep.

    GR_ObjTab[dlInd].disp   = DL_disp_act;  // ON od OFF

    // if(AttInd == Typ_Att_hili1) {
      // GR_ObjTab[GR_TAB_IND].hili   = ON;
    // } else {
    GR_ObjTab[dlInd].hili   = OFF;
    // }
    GR_ObjTab[dlInd].dim    = OFF;
    GR_ObjTab[dlInd].grp_1  = OFF;
    GR_ObjTab[dlInd].pick   = GR_pick;
    // GR_ObjTab[dlInd].lay    = GR_lay_act;
    // GR_ObjTab[dlInd].temp   = itemp;

  // if(DBInd > 0)


  L_done:
  DL_ind_act  = -1;    // preset default = create new DL-Record.
  GL_modified = 1;
  DL_disp_act = 0;     // default = ON          2010-12-20


    // DL_DumpObj__ (dlInd);
    // printf("ex DL_StoreObj %d\n",dlInd);

 
  // Typ_apDat: dieses obj bekommt kein sichtbares GL-obj;
  // daher muss ein bereits in der GL-DispList befindliches Objekt
  // (zB aus einem anderen subModel) geloescht werden !
  if(Typ == Typ_apDat) {
    GL_Del1 (dlInd);
  }

    // printf("ex DL_StoreObj %ld IND=%ld\n",dlInd,GR_TAB_IND);

  return dlInd;

}


//================================================================
  int  DL_DumpObj__ (long idl) {
//================================================================
/// see GA_dump__

  short     i20;
  char      cbuf[256], cAtt[32];
  ColRGB    *col;
  stru_2i2  *i2att;

  // printf("DL_DumpObj__ %ld\n",idl);


  if((GR_ObjTab[idl].typ == Typ_SUR)  ||
     (GR_ObjTab[idl].typ == Typ_SOL)  ||
     (GR_ObjTab[idl].typ == Typ_Model))   {
    col = (ColRGB*)&GR_ObjTab[idl].iatt;
    if(col->vtex == 0) {
      // sprintf(cAtt, "r%dg%db%d,%d",
      sprintf(cAtt, "r%02xg%02xb%02x,t%d,s%d %d",
              col->cr,col->cg,col->cb,col->vtra,col->vsym,col->color);
    } else {
      sprintf(cAtt, "tex%d",((stru_c3c1*)col)->b123);
    }


  } else if(GR_ObjTab[idl].typ == Typ_Tag) {          // Tag,Images ...
    i2att = (stru_2i2*)&GR_ObjTab[idl].iatt;
    sprintf(cAtt, "%d/%d",i2att->i20,i2att->i21);


  } else {                              // curves
    sprintf(cAtt, "%ld",GR_ObjTab[idl].iatt);
  }

  i20 = GR_ObjTab[idl].modInd;

  sprintf(cbuf,
 "%ld Typ=%d Ind=%ld att=%s mod=%d rs=%ld uv=%d di=%d hi=%d pi=%d chd=%d par=%d grp=%d lNr=%ld",
        idl,
        GR_ObjTab[idl].typ,
        GR_ObjTab[idl].ind,
        cAtt,
        i20,
        GR_ObjTab[idl].irs,
        GR_ObjTab[idl].unvis,
        GR_ObjTab[idl].disp,
        GR_ObjTab[idl].hili,
        GR_ObjTab[idl].pick,
        GR_ObjTab[idl].sChd,
        GR_ObjTab[idl].sPar,
        GR_ObjTab[idl].grp_1,
        GR_ObjTab[idl].lNr);

  // puts (cbuf);
  printf("%s\n",cbuf);

  return 0;

}


//================================================================
  void   DL_DumpObjTab    () {
//================================================================

  long l1;

  TX_Print("#### DL_DumpObjTab %d",GR_TAB_IND);


  for(l1=0; l1<GR_TAB_IND; ++l1) DL_DumpObj__ (l1);

  // temp-Liste:
  // for(l1=1; l1<DL_Ind_tmp; ++l1) {
    // printf(" call temp.o %ld\n",l1);
    // glCallList ((GLuint)l1);                      // execute
  // }



}

//================================================================
  int DL_find_APPOBJ (long dbi, long att) {
//================================================================
// DL_find_APPOBJ                     find applicationObject

  long     l1, le;

  le = GR_TAB_IND - 1;

  for(l1=le; l1>=0; --l1) {
    if(GR_ObjTab[l1].typ    != Typ_APPOBJ) continue;
    if(GR_ObjTab[l1].ind    != dbi) continue;
    if(GR_ObjTab[l1].iatt   != att) continue;
    goto L_found;

  }
  l1 = -1L;

  L_found:
  return l1;

}


//================================================================
  long DL_find_obj (int typ, long DBind, long DLend) {
//================================================================
/// \code
/// get dispListIndex of (last) DB-obj from typ/dbi
/// Objekt typ=typ APTind=ind in der DL suchen.
/// liefert DisplayListIndex des letzen definierten Objekts
/// see AP_typ_2_bastyp AP_cmp_typ
/// Hidden objects haben -1 !!
/// Input:
///   typ     objTyp (Typ_PT ..)
///   DBind   dataBaseIndex of obj to search
///   DLend   last DL-Index to check;  -1L = search in complete DL
/// retCode = DispListIndex
///      -1   Error; objID not found in DL
/// \endcode

// ACHTUNG: kann keine Vecs & Vars finden !!!


  return DL_find_smObj (typ, DBind, DLend, WC_modnr);

}


//=============================================================
  long DL_find_smObj (int typ, long DBind, long DLend, int imod) {
//=============================================================
/// \code
/// get dispListIndex of (last) DB-obj from typ/dbi/subModelNr
/// Objekt typ=typ APTind=ind in der DL suchen.
/// liefert DisplayListIndex des letzen definierten Objekts
/// see AP_typ_2_bastyp AP_cmp_typ
/// Hidden objects haben -1 !!
/// Input:
///   typ     objTyp (Typ_PT ..)
///   DBind   dataBaseIndex of obj to search
///   DLend   last DL-Index to check;  -1L = search in complete DL
///   imod    WC_modnr 
/// retCode = DispListIndex
///      -1   Error; objID not found in DL
/// \endcode

// ACHTUNG: kann keine Vecs & Vars finden !!!


  int   typAct;
  unsigned short modnr;
  long  l1, DLind;


  typ = AP_typ_2_bastyp (typ);

  // ACHTUNG: WC_modnr in ein I2 kopieren, da in der DL als I2 gespeichert;
  // -1 == Main wird sonst nicht gefunden !!!
  modnr = imod;


  // printf("DL_find_obj typ=%d dbi=%d modnr=%d\n",typ,DBind,modnr);
  // DL_DumpObjTab ();



  // for(l1=0; l1<GR_TAB_IND; ++l1)i
  // von vorn weg suchen; geht nicht - findet parent statt child ..

  if(DLend < 0) DLend = GR_TAB_IND - 1;

  for(l1=DLend; l1>=0; --l1) {

      // printf("DL[%d] typ=%d dbi=%d\n",l1,DL_GetTyp(l1),DL_GetInd(l1));

    if(GR_ObjTab[l1].ind    != DBind) continue;
    if(GR_ObjTab[l1].modInd != modnr) continue;       // skip SubModels
    if(GR_ObjTab[l1].typ    == 0) continue;           // skip deleted obj

    
    typAct = GR_ObjTab[l1].typ;
      // printf(" dl[%d] typ=%d\n",l1,typAct);


    if(typAct == typ) {
      DLind = l1;
      goto L_found;
    }

    typAct = AP_typ_2_bastyp (typAct);
    if(typAct == typ) {
      DLind = l1;
      goto L_found;
    }

/*
    // } else if(typ == Typ_AC) {
      // DLind = l1;
      // goto L_found;

    
    } else if(typ == Typ_CV) {         // CV = CVCCV
      if((typAct == Typ_CV)     ||
         (typAct == Typ_CVCCV))    {
        DLind = l1;
        goto L_found;
      }


    } else if(typ == Typ_GTXT) {
      if((typAct == Typ_GTXT)     ||
         (typAct == Typ_ATXT)     ||
         (typAct == Typ_Dimen))    {
        DLind = l1;
        goto L_found;
      }

 
    } else if(typAct == Typ_SUR) {    // in der DL gibts nur Typ_SUR
      if(typ == Typ_SURRV) {
        DLind = l1;
        goto L_found;
      }


    } else if(typAct == Typ_Mock) {
      if(typ == Typ_Model) {
        DLind = l1;
        goto L_found;
      }
    }
*/


  }



  // printf(" cannot find obj typ=%d ind=%ld\n",typ,DBind);
  // DL_DumpObjTab ();


  DLind = -1;


  //=====================================
  L_found:
    // printf("ex DL_find_obj dli=%d\n",DLind);

  return DLind;

}


//================================================================
  int DL_find_sel (int *typ, long *ind) {
//================================================================
/// \code
/// find selected objectID in DispList
/// (first hilited obj in dispList)
/// \endcode

// wird genauso verwendet in GL_Redraw

  long l1;


  for(l1=0; l1<GR_TAB_IND; ++l1) {
    if(GR_ObjTab[l1].disp  != OFF) continue;
    if(GR_ObjTab[l1].hili  != ON)  continue;
    if(GR_ObjTab[l1].unvis != 0)   continue; // skip unvisible

    // printf(" _find_sel %d typ=%d ind=%d\n",l1,GR_ObjTab[l1].typ,GR_ObjTab[l1].ind);
    *typ = GR_ObjTab[l1].typ;
    *ind = GR_ObjTab[l1].ind;
    return 0;
  }

  return -1;

}


//============================================================
  int DL_GetTyp (long objInd) {
//============================================================
///  liefert den Typ eines DB-Obj (als return-Code)

 // Input: objInd; alle andern Out.


  int Typ;
  
  if(objInd < 0) return Typ_Error;
  if(objInd >= GR_TAB_IND) return Typ_Error;

  Typ = GR_ObjTab[objInd].typ;

  // printf ("DL_GetTyp typ=%d\n",Typ);

  return Typ;

}


//============================================================
  long DL_GetInd (long objInd) {
//============================================================
/// \code
///  liefert den DB-index eines DB-Obj (als return-Code)
/// 
///  Input: objInd; alle andern Out.
/// \endcode


  long ind;

  if(objInd < 0) return -1L;
  if(objInd >= GR_TAB_IND) return -1L;

  ind = GR_ObjTab[objInd].ind;

  // printf ("ex DL_GetInd ind=%d objInd=%ld\n",ind,objInd);

  return ind;

}


//================================================================
  int DL_Get_oid_dli (char *oid, long dli) {
//================================================================
/// DL_Get_oid_dli        get objName from DispListIndex


  // make name from typ and DB-Index
  APED_oid_dbo__ (oid, GR_ObjTab[dli].typ, GR_ObjTab[dli].ind);

    // printf("ex DL_Get_oid_dli |%s| %d\n",oid,dli);


  return 0;

}


//================================================================
   int DL_Get_lNr_dli (long *lNr, long dli) {
//================================================================
/// \code
/// DL_Get_lNr_dli          get source-lineNumber from DispListIndex
/// see AP_lNr_set
/// \endcode

  if((dli < 0)||(dli >= GR_TAB_IND)) return -1;

  *lNr = GR_ObjTab[dli].lNr;

  return 0;

}


//================================================================
  int DL_Get_dli_lNr (long *dli, long *lNr) {
//================================================================
// get DispListIndex from sourceLineNumber
// RetCod:
//    0     OK, lNr found
//    1     lNr not in DL; previous lNr returned
//   -1     DL empty.

  int    irc = -1;
  long   l1;


  // printf("DL_Get_dli_lNr %ld\n",*lNr);
  // printf(" WC_modnr=%d\n",WC_modnr);
  // DL_DumpObjTab ();

  if(GR_TAB_IND < 1) { *dli = 0; return -1; }

  // get dli from sourceLineNr
  for(l1 = GR_TAB_IND - 1; l1 >= 0; --l1) {
      // printf(" l1=%d\n",l1);

    if((INT_16)GR_ObjTab[l1].modInd != WC_modnr) continue;
    
    if(GR_ObjTab[l1].lNr > *lNr) continue;
    if(GR_ObjTab[l1].lNr != *lNr) irc = 1;
    else                          irc = 0;

    *lNr = GR_ObjTab[l1].lNr;
    *dli = l1;
    break;

  }

    // printf("ex DL_Get_dli_lNr %d %ld %ld\n",irc,*dli,*lNr);

  return irc;

}


//================================================================
  long DL_GetTrInd (long dli) {
//================================================================
/// get the refsys-nr (dbi of Plane) for DL-record <dli>

  long ind;

  if((dli < 0)||(dli > APT_PL_SIZ)) return -1;
  // ind = GR_ObjTab[objInd].refInd;

  // TX_Print ("DL_GetInd typ=%d\n",ind);

  return GR_ObjTab[dli].irs;

} 


//=============================================================
  int DL_GetPick (long objInd) {
//=============================================================

  return GR_ObjTab[objInd].pick;

}


//=============================================================
  DL_Att DL_GetAtt (long objInd) {
//=============================================================

  if(objInd < 0)           { TX_Error("DL_GetAtt E001"); return GR_ObjTab[0]; }
  if(objInd >= GR_TAB_IND) { TX_Error("DL_GetAtt E002"); return GR_ObjTab[0]; }

  return GR_ObjTab[objInd];

}



/*
//=============================================================
  int DL_GetObj (long objInd, long* aptInd, int* AttInd) {
//=============================================================
//                   in             out          out
// liefert den Typ eines DB-Obj (als return-Code) sowie seinen DB-Index
// und sein AttrInd.

// Input: objInd; alle andern Out.
//

  int Typ;


  // printf("DL_GetObj E001 %ld %ld\n",objInd,GR_TAB_IND);


  if((objInd < 0) || (objInd >= GR_TAB_IND)) {


    TX_Error("DL_GetObj E001 %ld %ld",objInd,GR_TAB_IND);
    Typ = Typ_Error;

    // DL_DumpObjTab ();



  } else {

    Typ = GR_ObjTab[objInd].typ;

    *aptInd  = GR_ObjTab[objInd].ind;
    *AttInd = GR_ObjTab[objInd].attInd;
  }



  // printf ("ex DL_GetObj %ld typ=%d apt=%ld att=%d\n",
            // objInd,Typ,*aptInd,*AttInd);


  return Typ;

}
*/

//================================================================
  int DL_set_iatt (long dli, unsigned long iatt) {
//================================================================
// DL_set_iatt             modify iatt of DL-record

  GR_ObjTab[dli].iatt = iatt;

  return 0;

}


//================================================================
  unsigned long DL_get_iatt (long dli) {
//================================================================
/// returns iatt of DL-record

  return GR_ObjTab[dli].iatt;

}

 
//================================================================
  int DL_get_sStyl (long dli) {
//================================================================
/// \code
/// get surfaceStyle (shaded|symbolic) of DispListObj
/// return 0=shaded; 1=symbolic
/// \endcode


  ColRGB *col;


  col = (ColRGB*)&GR_ObjTab[dli].iatt;

  return col->vsym;

}


/* replaced with AttLn_Get_ind
//===============================================================
  int DL_Get_GrAtt (GR_Att* att1, long Ind) {
//===============================================================
/// get graf.Att (GR_AttTab[Ind])


  // printf("DL_Get_GrAtt %ld siz=%d\n",Ind,GR_ATT_TAB_SIZ);

  if(Ind >= GR_ATT_TAB_SIZ) {
    printf("***** DL_Get_GrAtt E001 %ld\n",Ind);
    Ind = 0;
  }


  *att1 = GR_AttTab[Ind];

  return 0;
}
*/

//================================================================
  int DL_GetNrSur () {
//================================================================
/// DL_GetNrSur             get nr of sur's in DispList

  long   l1, i1;

  i1 = 0;
  for(l1=0; l1<GR_TAB_IND; ++l1) {
    if(GR_ObjTab[l1].typ < Typ_SUR) continue;
    if(GR_ObjTab[l1].typ > Typ_SURRV) continue;
    ++i1;
  }

  // printf("ex DL_GetNrSur %d\n",i1);

  return i1;

}


//================================================================
  int DL_GetNrSol () {
//================================================================
/// DL_GetNrSol             get nr of sol's in DispList

  long   l1, i1;

  i1 = 0;
  for(l1=0; l1<GR_TAB_IND; ++l1) {
    if(GR_ObjTab[l1].typ < Typ_SOL) continue;
    ++i1;
  }

  // printf("ex DL_GetNrSol %d\n",i1);

  return i1;

}


/* replaced
//============================================================
  int DL_StoreAtt (long Ind, GR_Att* att1) {
//============================================================
// store att in GR_AttTab and activate it in DL


  int    i1,i2,i3;


  // printf("DL_StoreAtt %d = %d %d %d\n",Ind,att1->col,att1->ltyp,att1->lthick);


  // ind must be less GR_ATT_TAB_SIZ
  if((Ind < 0)||(Ind >= GR_ATT_TAB_SIZ)) {
    TX_Error("- Zugriff auf Attribut %d ****",Ind);
    return -1;
  }


  //  Alle dürfen nur zwischen 0 und 15 sein!
  i1 = att1->col;
  i2 = att1->ltyp;
  i3 = att1->lthick;

  if((i1<0)||(i1>15)) {
    TX_Print(" Parameter Farbe nicht 0 bis 15");

  } else   if((i2<0)||(i1>15)) {
    TX_Print(" Parameter Linientyp nicht 0 bis 15");

  } else   if((i3<0)||(i1>15)) {
    TX_Print(" Parameter Liniendicke nicht 0 bis 15");
  }


  // store att in GR_AttTab and activate in DL
  GR_AttTab[Ind]        = *att1;
  GR_AttTab[Ind].used   = ON;


  //  das Attr nun in der DL definieren
  GL_InitNewAtt (0, Ind);


  return 0;

}
*/




//***********************************************************************
 void DL_Lay_act_g1 (int layNr, int mode) {
//***********************************************************************
/// activate GroupBit1 des layer Nr. layNr 

  long   l1;

  printf("DL_Lay_act_g1 E001 \n");

/*
  // das GroupBit1 mal fuer alle Obj, die NICHT in Lay layNr sind, ON setzen.
  if(mode == ON) {


    for(l1=0; l1<GR_TAB_IND; ++l1) {
      if(GR_ObjTab[l1].lay != layNr) {
        GR_ObjTab[l1].grp_1   = ON;
      } else {
        GR_ObjTab[l1].grp_1   = OFF;
      }
    }



  } else {
    // GroupBit1 fuer alle Obj, die NICHT im Layer i1 sind, auf OFF.
    for(l1=0; l1<GR_TAB_IND; ++l1) {
      if(GR_ObjTab[l1].grp_1 == ON) {
        if(GR_ObjTab[l1].lay != layNr) {
          GR_ObjTab[l1].grp_1   = OFF;
        }
      }
    }
  }
*/

}



//***********************************************************************
 void DL_Lay_typ_g1 (int typ, int mode) {
//***********************************************************************
/// grp_1 fuer alle Obj, die NICHT Typ i1 haben, auf OFF.

  long   l1;
  int    i2;


    for(l1=0; l1<GR_TAB_IND; ++l1) {
      i2=GR_ObjTab[l1].typ;
      //TX_Print(" Typ %d = %d",l1,i2);
      if(GR_ObjTab[l1].grp_1 == ON) {
        if(i2 != typ) {
          GR_ObjTab[l1].grp_1   = OFF;
        }
      }
    }
}



//***********************************************************************
 void DL_Lay_col_g1 (int col, int mode) {
//***********************************************************************
/// \code
/// das GroupBit1 fuer alle Obj, die NICHT Farbe i1 haben, ON setzen.
/// mode unused
/// \endcode
    
  long   l1;
  int    i1,i2,i3, ii;




  // if(mode == ON) {
    for(l1=0; l1<GR_TAB_IND; ++l1) {
      // i2=GR_AttTab[GR_ObjTab[l1].iatt].col;
      AttLn_Get_ind (&i1, &i2, &i3, l1);
      if(GR_ObjTab[l1].grp_1 == ON) {
        if(i1 != col) {
          GR_ObjTab[l1].grp_1   = OFF;
        }
      }
    }
}



//***********************************************************************
 void DL_Lay_ltyp_g1 (int ltyp, int mode) {
//***********************************************************************
/// grp_1 fuer alle Obj, die NICHT ltyp i1 haben, auf OFF.

  long   l1;
  int    i2;


    for(l1=0; l1<GR_TAB_IND; ++l1) {
      // i2=GR_AttTab[GR_ObjTab[l1].iatt].ltyp;
      i2=i2=GR_AttLnTab[GR_ObjTab[l1].iatt].dash;
      if(GR_ObjTab[l1].grp_1 == ON) {
        if(i2 != ltyp) {
          GR_ObjTab[l1].grp_1   = OFF;
        }
      }
    }
}


//***********************************************************************
 void DL_Lay_thk_g1 (int thk, int mode) {
//***********************************************************************
/// grp_1 fuer alle Obj, die NICHT Dicke i1 haben, auf OFF.

  long   l1;
  int    i2;

    for(l1=0; l1<GR_TAB_IND; ++l1) {
      // i2=GR_AttTab[GR_ObjTab[l1].iatt].lthick;
      i2=GR_AttLnTab[GR_ObjTab[l1].iatt].thick;
      if(GR_ObjTab[l1].grp_1 == ON) {
        if(i2 != thk) {
          GR_ObjTab[l1].grp_1   = OFF;
        }
      }
    }
}





//***********************************************************************
 int  DL_Lay_mod (int layNr, int func, int mode) {
//***********************************************************************
/// \code
/// modify layer
/// layNr  die Nr. des zu bearbeitenden Layers; -1 = alle Layers !
/// func   ...
/// mode   ON  oder OFF; entspr. func.
/// func: FUNC_Show, FUNC_Pick, FUNC_Hili, FUNC_Dim.
/// \endcode




  long   l1;

  //TX_Print ("DB_Lay %d %d %d",layNr, func, mode);

  printf("DL_Lay_mod E001 \n");
/*
  for(l1=0; l1<GR_TAB_IND; ++l1) {


    if(layNr >= 0) {
      if(GR_ObjTab[l1].lay != layNr) continue;
    }


    switch (func) {


      case FUNC_Show:
        GR_ObjTab[l1].disp   = mode;
        break;


      case FUNC_Pick:
        GR_ObjTab[l1].pick   = mode;
        break;

      case FUNC_Hili:
        if(mode == ON) {
          GR_ObjTab[l1].disp   = OFF;
        } else {
          GR_ObjTab[l1].disp   = ON;
        }
        GR_ObjTab[l1].hili   = mode;
        break;


      case FUNC_Dim:
        if(mode == ON) {
          GR_ObjTab[l1].disp   = OFF;
        } else {
          GR_ObjTab[l1].disp   = ON;
        }
        GR_ObjTab[l1].dim    = mode;
        break;

    }

  }
*/
  return 1;
}




//***********************************************************************
 int DL_Lay_add (int layNr) {
//***********************************************************************
   
  // nun sollen alle Obj, die das grp_1-Bit ON haben, dem Layer layNr
  // zugefuegt werden.

  long l1;

  printf("DL_Lay_add E001\n");
/*
  for(l1=0; l1<GR_TAB_IND; ++l1) {
    if(GR_ObjTab[l1].grp_1 == ON) {
      GR_ObjTab[l1].lay = layNr;
    }
  }
*/
  return 0;

}


//====================================================================
  int DL_ReScale_pt_get (Point *pt1, Point *pt2) {
//====================================================================
/// uxmin-uzmax erweitern um Box

  if(pt1->x < uxmin) uxmin = pt1->x;
  if(pt2->x > uxmax) uxmax = pt2->x;

  if(pt1->y < uymin) uymin = pt1->y;
  if(pt2->y > uymax) uymax = pt2->y;

  if(pt1->z < uzmin) uzmin = pt1->z;
  if(pt2->z > uzmax) uzmax = pt2->z;


  return 0;

}


//====================================================================
  void DL_ReScalePoint (Point *pt1) {
//====================================================================

  double   x1, y1, z1;
  Point    ptTr;


  // printf("DL_ReScalePoint %f,%f,%f\n",pt1->x,pt1->y,pt1->z);



  // if(APT_2d_to_3d_mode == OFF) {
    x1 = pt1->x;
    y1 = pt1->y;
    z1 = pt1->z;
  // } else {
    // UT3D_pt_traptm3 (&ptTr, APT_2d_to_3d_Mat, pt1);
    // x1 = ptTr.x;
    // y1 = ptTr.y;
    // z1 = ptTr.z;
  // }


  if(x1 < uxmin) uxmin = x1;
  if(x1 > uxmax) uxmax = x1;

  if(y1 < uymin) uymin = y1;
  if(y1 > uymax) uymax = y1;

  if(z1 < uzmin) uzmin = z1;
  if(z1 > uzmax) uzmax = z1;

} 


//====================================================================
  int DL_ReScaleObj (ObjGX *oxi) {
//====================================================================
  
  // UTO_dump__ (oxi, "DL_ReScaleObj");


   // DL_ReScalePoint(&pt1);

  return 0;

}


//====================================================================
  void DL_ReScale__ () {
//====================================================================
/// \code
/// Neue Version
/// Probleme mit d1: try
///  int    isiz;
///  float  feedBuffer[100];
///  isiz = 100;
///  GL_Feedback (&isiz, &feedBuffer);
///  printf(" isiz=%d\n",isiz);
///  isiz < 0 OK; mehr Vertices gefunden;
///  else increase d1 to
///  d1 = APT_ModSiz / 0.1;
/// \endcode






  int         i1, i2, ii, ivs;
  double      d1, d2, d3;
  Point       pb1, pb2, pm;


  // printf("DL_ReScale__ ModSiz=%f\n",APT_ModSiz);


  ivs = GL_vcSel_init (0);   // do not draw VectorSelector

  // es muessen alle Obj innerhalb Screen sein!
  // reset scale to 1 ?
  GL_DefineView (FUNC_ViewReset) ; // GL_initMode = 0; GL_Init_View ();


  // und in Draufsicht !!??
  // GL_DefineView (FUNC_ViewTop);
  // d1 =  APT_ModSiz * 10000.;
  // d1 =  1000000. / APT_ModSiz;    // bis 2005-10-27


  // Problem mit sehr kleinen Objekten; zB WRL/Assy6.wrl
  // d1 = APT_ModSiz / 0.001;

  // Sehr gr.Objekt ist IGS/Rahmen2; IGS/6001_3.igs
  // d1 = APT_ModSiz;  // 2011-01-09
  // startvalue must be fixed ! 2011-05-21   
  // 2011-06-27  ..Err3 geht ned mit d1=1000; erst mit 25000.
  d1 = 25000.; 
  ii = 0;


  L_retry:
  ++ii;
    // printf("L_retry d1=%f\n",d1);
  GL_Rescale (d1, (Point*)&UT3D_PT_NUL);   // macht ein GL_Redraw!

  // init box
  UT3D_box_ini0 (&pb1, &pb2);

  // box in Userkoords berechnen mit GL_FeedGet
  i1 = GL_Rescal0 (&pb1, &pb2);
    // printf(" d1=%f scal0-i1=%d\n",d1,i1);
  if(i1 == -2) {  // nix im buffer ..  // nix im buffer ..
    d1 *= 2.;
    if(ii < 10)  goto L_retry;
  }
    // UT3D_stru_dump (Typ_PT, &pb1, "-pb1:");
    // UT3D_stru_dump (Typ_PT, &pb2, "-pb2:");

  // printf("DL_ReScale__ L2\n");


  // zusatzl die Notes beruecksichtigen ..
  i2 = DL_ReScale_Notes (&pb1, &pb2);
    // printf(" d1=%f scalN-i2=%d\n",d1,i2);
    // UT3D_stru_dump (Typ_PT, &pb1, " pb1:");
    // UT3D_stru_dump (Typ_PT, &pb2, " pb2:");


  if((i1 < 0)&&(i2 < 0)) {
    // rescale failed; try Top / Scal.All
    TX_Print(" - rescale failed, reset ..");
    d1 = APT_ModSiz;
    pm = UT3D_PT_NUL;
    goto L_reset;
  }



  // get center of box
  UT3D_pt_mid2pt (&pm, &pb1, &pb2);

  // fix Usiz
  d1 = (pb2.x - pb1.x) / 1.;
  d2 = (pb2.y - pb1.y) / 1.;
  d3 = (pb2.z - pb1.z) / 1.;
    // printf(" d1=%f d2=%f d3=%f\n",d1,d2,d3);
  
  // den kleinsten waehlen
  if(d3 > d1) {
    d1 = d3;
  }
  if(d2 > d1) {
    d1 = d2;
  }


  L_reset:
    // UT3D_stru_dump (Typ_PT, &pm, " center pm=");
    // printf(" new scale = %f\n",d1);

  GL_vcSel_init (ivs, 0);      // reset VectorSelector

  // !! Hier wird die Z-Plane fuer den Viewer gesetzt !!
  GL_Rescale (d1, &pm);        // macht ein GL_Redraw!


  L_exit:
    // printf("ex DL_ReScale__ ModSiz=%f\n",APT_ModSiz);
  return;

}

//================================================================
  int DL_ReScale_Notes (Point *pb1, Point *pb2) {
//================================================================
/// \code
///  Notes/Images: es sollten die Eckpunkte lu und ro generiert werden und
///  wie normale Punkte beruecksichtigt werden.
/// \endcode

  int    sx, sy, dx, dy, typ, pNr;
  long   l1;
  double dsx, dsy, dsz;
  Point  p1, p2;


      // UT3D_stru_dump(Typ_PT, pb1, "  pb1i=");
      // UT3D_stru_dump(Typ_PT, pb2, "  pb2i=");

  pNr = -1;

  // loop tru all visible notes ..
  for(l1=0; l1<GR_TAB_IND; ++l1) {
    if(GR_ObjTab[l1].typ  != Typ_Tag) continue;
    if((GR_ObjTab[l1].disp==1)&&(GR_ObjTab[l1].hili == 1)) continue; // hidden

    // SizeInfo zu Tag/Image holen
    DL_txtgetInfo (&typ, &p1, &sx, &sy, &dx, &dy, l1);

    // use LU
    UT3D_box_extend (pb1, pb2, &p1);   ++pNr;

    // man kann nur autoZoomImages benutzen !
    if(typ != 1) continue;


    // change Textpoint --> Screencoords
    GL_Uk2Sk (&dsx, &dsy, &dsz, p1.x, p1.y, p1.z);
      // printf(" LU(ScrCoords) dsx=%f dsy=%f dsz=%f\n",dsx,dsy,dsz);

    dsx += sx + dx;
    dsy += sy + dy;

    // change RO --> UserCoords
    GL_Sk2Uk (&p2.x, &p2.y, &p2.z,  dsx, dsy, dsz);
      // UT3D_stru_dump(Typ_PT, &p2, " RO=");

    // use RO
    UT3D_box_extend (pb1, pb2, &p2);   ++pNr;
  }

      // UT3D_stru_dump(Typ_PT, pb1, "  pb1o=");
      // UT3D_stru_dump(Typ_PT, pb2, "  pb2o=");

  return pNr;

}


/*
//====================================================================
  void DL_ReScale1 () {
//====================================================================

  double d1, d2;
  long   l1, DBind, apt_ind, obj_ind, *ia;
  int    i1, i2, typ, typ1, att, iNr, ckb_view_stat;

  Point       pb1, pb2, pt1, pt2, *pta;
  Point2      p21, p22, p23, *pt2a;
  Line        ln1;
  Plane       pl1;
  Mat_4x3     m1;
  Circ        ci1;
  CurvElli     *el1;
  ObjGX       *cv1, *oTab;
  ObjGX       *ox1, o1;
  // ObjGX       *suc1;
  SurStripe   *sus1;
  SurBSpl     *sub1;
  SurRev      *srv;
  // CurvPol3    *cvpsp3;
  CurvBSpl    *cvpspl;
  polynom_d3  *cvpol3;
  GText       *tx1;
  Dimen       *dim1;
  ModelRef    *mr1;




  printf("DL_ReScale\n");

  //DL_DumpObjTab ();  // Testausgabe gesamte DB-Tab.


  // zuerst uxmin-max, uymin-max holen,
  uxmin =  UT_VAL_MAX;
  uxmax = -UT_VAL_MAX;
  uymin =  UT_VAL_MAX;
  uymax = -UT_VAL_MAX;
  uzmin =  UT_VAL_MAX;
  uzmax = -UT_VAL_MAX;

  UT3D_box_ini0 (&pb1, &pb2);

  DBind = GL_Get_DLind ();
  // printf(" - DL_ReScale: DBind=%d\n",DBind);

  if(DBind > 0 ) {

    for (obj_ind=0; obj_ind < DBind; ++obj_ind) {

      // skip hidden objects
      // printf(" %d disp=%d\n",obj_ind,GR_ObjTab[obj_ind].disp);
        if(GR_ObjTab[obj_ind].disp == OFF) {
          // printf("skip %d\n",obj_ind);
          continue;
        }


      // skip objects not in active model:
      if(GR_ObjTab[obj_ind].modInd != -1) continue;


      // typ = DL_GetObj (obj_ind, &apt_ind, &att);
      typ     = GR_ObjTab[obj_ind].typ;
      apt_ind = GR_ObjTab[obj_ind].ind;
      // printf(" ReScale %d typ=%d ind=%ld\n",obj_ind,typ,apt_ind);


      //TX_Print(" %d u.min= %f,%f  %f %f",obj_ind,uxmin,uymin,xScale,yScale);

      // skip temp obj's
      if(GR_ObjTab[obj_ind].temp != 0) {
        // printf("##### ist ein temp.obj\n");
        continue;
      }


      if (typ == Typ_PT) {
        // if(apt_ind >= dyn_pt_max) continue;
        pt1 = DB_GetPoint (apt_ind);
        DL_ReScalePoint(&pt1);


      } else if (typ == Typ_SymV) {
        if(apt_ind > 0) {
          pt1 = DB_GetPoint (apt_ind);
          DL_ReScalePoint(&pt1);
        }


      } else if (typ == Typ_LN) {
        // if(apt_ind >= dyn_ln_max) continue;
        ln1 = DB_GetLine (apt_ind);
        DL_ReScalePoint(&ln1.p1);
        DL_ReScalePoint(&ln1.p2);


      } else if ((typ == Typ_AC)||(typ == Typ_CI)) {
        // if(apt_ind >= dyn_ci_max) continue;
        ci1 = DB_GetCirc (apt_ind);
        DL_ReScalePoint(&ci1.p1);
        // p1=p2: Vollkreis; auch vis-vis.
        if(UT3D_comp2pt(&ci1.p1, &ci1.p2, UT_TOL_pt)) {
          UT3D_pt_opp2pt(&pt1, &ci1.pc, &ci1.p1);
          DL_ReScalePoint(&pt1);
        } else {
          DL_ReScalePoint(&ci1.p2);
        }




      } else if (typ == Typ_PLN) {
        // if(apt_ind >= dyn_ci_max) continue;
        DB_GetRef (&pl1, m1, apt_ind);
        DL_ReScalePoint(&pl1.po);



      //-----------------------------------------------------------------
      } else if (typ == Typ_CV) {
        cv1 = DB_GetCurv (apt_ind);
        typ1 = cv1->typ;


        // printf("........Typ_CV = %d ........\n",typ1);
        if(typ1 == Typ_CVPOL) {   // 7
          // pta = cv1->data;
          // iNr = cv1->siz;
          pta = ((CurvPoly*)cv1->data)->cpTab;
          iNr = ((CurvPoly*)cv1->data)->ptNr;
          for(i1=0;i1<iNr; ++i1) {
            DL_ReScalePoint(&pta[i1]);
          }




        } else if(typ1 == Typ_CVELL) {
          el1 = cv1->data;
          DL_ReScalePoint(&el1->p1);
          // p1=p2: Vollkreis; auch vis-vis.
          if(UT3D_comp2pt(&el1->p1, &el1->p2, UT_TOL_pt)) {
            UT3D_pt_opp2pt(&pt1, &el1->pc, &el1->p1);
            DL_ReScalePoint(&pt1);
          } else {
            DL_ReScalePoint(&el1->p2);
          }



        } else if (typ1 == Typ_CVPOL2) {  // 18
          // p2Tab = cv1.data;
          pt2a = cv1->data;
          iNr  = cv1->siz;
          for(i1=0;i1<iNr; ++i1) {
            pt1.x = pt2a[i1].x;
            pt1.y = pt2a[i1].y;
            pt1.z = 0.;
            DL_ReScalePoint(&pt1);
          }


        } else if (typ1 == Typ_CVBSP) {  // 9
          cvpspl = cv1->data;
          pta = cvpspl->cpTab;
          iNr = cvpspl->ptNr;
          for(i1=0;i1<iNr; ++i1) {
            DL_ReScalePoint(&pta[i1]);
          }



        } else if (typ1 == Typ_CVPSP3) {  // 11
          // cvpsp3 = cv1->data;
          cvpol3 = (polynom_d3*)cv1->data;
          iNr  = cv1->siz;
          for(i1=0;i1<iNr; ++i1) {
            pt1.x = cvpol3[i1].x.a;
            pt1.y = cvpol3[i1].y.a;
            pt1.z = cvpol3[i1].z.a;
            DL_ReScalePoint(&pt1);
            // printf(" seg %d %f %f %f\n",i1,pt1.x,pt1.y,pt1.z);
          }

        } else {
          // printf("**** DL_ReScale: unused Curvetyp %d\n",typ1);

        }






      //-----------------------------------------------------------------
      } else if ((typ == Typ_ATXT)  ||
                 (typ == Typ_GTXT)  ||
                 (typ == Typ_Dimen))    {
        // if(apt_ind >= APT_TX_SIZ) continue; // skip dynam. text
        ox1 = DB_GetGTxt (apt_ind);



        if(ox1->typ == Typ_Dimen) {
          dim1 = ox1->data;
          pt1 = UT3D_pt_pt2 (&dim1->p1);
          DL_ReScalePoint(&pt1);
          pt1 = UT3D_pt_pt2 (&dim1->p2);
          DL_ReScalePoint(&pt1);
          if(dim1->p3.x != UT_DB_LEER) {
            pt1 = UT3D_pt_pt2 (&dim1->p3);
            DL_ReScalePoint(&pt1);
          }


        } else if(ox1->typ == Typ_GTXT) {

        tx1 = ox1->data;

        DL_ReScalePoint(&tx1->pt);  // Startpt. links unten

        // Endpunkt errechnen
        // d1=TextBoxBreite, d2=TextBoxLaenge
        i1=strlen(tx1->txt);

        if(tx1->size < 0.) {   // Alfatext
          // geht nicht wirklich; 
          d1 = i1 * 3 / GL_get_Scale();
          d2 = 3 / GL_get_Scale();

        } else {              // 3D-Text
          d1 = i1 * (GL_vf1_CW + GL_vf1_CS);
          d2 = GL_vf1_CW + GL_vf1_CS;   // Höhe = Breite + space

          d1 *= tx1->size;
          d2 *= tx1->size;
        }
        // printf(" d1,d2=%f,%f dir=%f\n",d1,d2,tx1->dir);

        if(!UTP_comp_0(tx1->dir)) {
          p21.x = tx1->pt.x;
          p21.y = tx1->pt.y;
          p22.x = p21.x + d1;
          p22.y = p21.y + d2;
          UT2D_pt_rotptangr (&p23, &p21, &p22, UT_RADIANS(tx1->dir));
          pt1.x = p23.x;
          pt1.y = p23.y;
          pt1.z = tx1->pt.z;
        } else {
          pt1.x = tx1->pt.x + d1;
          pt1.y = tx1->pt.y + d2;
          pt1.z = tx1->pt.z;
        }
    
        // printf("pt1=%f,%f,%f\n",pt1.x,pt1.y,pt1.z);
        // l1 = 0; APT_DrawPoint(&l1, 0, &pt1); // nur Test
        DL_ReScalePoint(&pt1);               // Endpt rechts oben
        }




      //-----------------------------------------------------------------
      } else if (typ == Typ_SUR) {
        ox1 = DB_GetSur (apt_ind, 1);
        // printf(" ReScal Sur typ=%d form=%d\n",ox1->typ,ox1->form);

        if(ox1->typ == Typ_SUR) {
          UTO_box_obj (&pb1, &pb2, ox1);  // siehe UT3D_npt_ox
          // if(ox1->form == Typ_SUR) {
            // man sollte eine Box von der ersten Kontur holen;
            // die Boxpunkte DL_ReScalePoint
            // siehe GR_DrawSur/SUP_load_c .., UT3D_npt_ox
            // UTO_box_obj (&pb1, &pb2, ox1->data);  // siehe UT3D_npt_ox
          // } else if (ox1->form == Typ_ObjGX) {
          // }

        } else if(ox1->typ == Typ_SURBSP) {
          sub1 = ox1->data;
          l1 = sub1->ptUNr * sub1->ptVNr;
          for(i1=0; i1<l1; ++i1) {
            DL_ReScalePoint(&sub1->cpTab[i1]);
          }

        // } else if(ox1->typ == Typ_SURRU) {
          // oTab = ox1->data;
          // DL_ReScaleObj(&oTab[0]);
          // DL_ReScaleObj(&oTab[1]);


        // } else if(ox1->typ == Typ_SURRV) {
          // srv = ox1->data;
          // o1 = DB_GetObjGX ((int)srv->typCov, srv->indCov);
          // typ2 = UTO_obj_get (&o1, &o1);



        } else if(ox1->typ == Typ_SURSTRIP) {
          sus1 = ox1->data;
          l1 = sus1->ptUNr * sus1->ptVNr;
          ia = (long*)sus1->ptiTab;
          for(i1=0; i1<l1; ++i1) {
            pt1 = DB_GetPoint (ia[i1]);
            // printf(" %d pt %d = %f,%f,%f\n",i1,ia[i1],pt1.x,pt1.y,pt1.z);
            DL_ReScalePoint(&pt1);
          }

        } else if(ox1->typ == Typ_SURCIR) {
          // suc1 = ox1->data;
          ia   = (long*)ox1->data;
          for(i1=0; i1<ox1->siz; ++i1) {
            // printf(" %d ind=%ld\n",i1,ia[i1]);
            pt1 = DB_GetPoint (ia[i1]);
            // printf(" %d pt %d = %f,%f,%f\n",i1,ia[i1],pt1.x,pt1.y,pt1.z);
            DL_ReScalePoint(&pt1);
          }

        } else {
          printf("***** DL_ReScale: nicht bearb. SUR-typ %d\n",typ);
        }





      //-----------------------------------------------------------------
      } else if (typ == Typ_Model) {
        mr1 = DB_get_ModRef (apt_ind);
        DL_ReScalePoint (&mr1->po);





      //-----------------------------------------------------------------
      } else {
        // printf("**** DL_ReScale: nicht bearb. Objekttyp %d\n",typ);
      }
    }
  }


  // uxmin-uzmax erweitern um Box
  DL_ReScale_pt_get (&pb1, &pb2);


  // man muesste nun den obersten und den untersten Z-Punkt entlang der
  // Sichtlinie auf die aktuelle Plane werfen
  pt2.x = uxmax; pt2.y = uymax; pt2.z = uzmax;
  pt1 = GL_GetViewPt (&pt2);
  DL_ReScalePoint(&pt1);

  pt2.x = uxmin; pt2.y = uymin; pt2.z = uzmin;
  pt1 = GL_GetViewPt (&pt2);
  DL_ReScalePoint(&pt1);




  // noch eine wenig verkleinern
  d1 = (uxmax - uxmin) / 7.;
  uxmin = uxmin - d1;
  uymin = uymin - d1;

  uxmax = uxmax + d1;
  uymax = uymax + d1;



  // printf(" korr: min=%f,%f,%f max=%f,%f,%f\n",uxmin,uymin,uzmin,
    // uxmax,uymax,uzmax);



  // 2 * ModSiz !
  d1 = (uxmax-uxmin) / 2.5;
  d2 = (uymax-uymin) / 2.5;




  // den kleineren waehlen
  if(d2 > d1) {
    d1 = d2;
  }

  // noch den Usermittelpunkt bestimmen
  pt1.x = (uxmin+uxmax) / 2.0;
  pt1.y = (uymin+uymax) / 2.0;
  pt1.z = 0.0;


  // printf(" neu sc=%f ucen=%f,%f,%f\n",d1,pt1.x,pt1.y,pt1.z);


  // dem Maszstab setzen
  //GR_SetUScale (d1);


  // !! Hier wird die Z-Plane fuer den Viewer gesetzt !!
  GL_Rescale (d1, &pt1);   // macht ein GL_Redraw!

  //TX_Print(" u.min= %f,%f  %f %f",uxmin,uymin,xScale,yScale);

  return;

}

*/




//================================================================
//================================================================
// Funktionen zum Feld (DL_Att).lNr
//================================================================
//================================================================


/*
//================================================================
  long DL_parent_get (int dbTyp, long dbInd) {
//================================================================
/// get DB-index of parentObj of DB-obj dbTyp/ind (typ is equal)
/// parentRecords in DL:
///   DL_Att.disp    is 1 (parentRecords are invisible);
///   DL_Att.sPar    is 1 (.par of childs is 0)
///   DL_Att.iatt    gives the DL-recordNr of its child; use its att.
//
// see also DL_parent_hide


  long     l1, iChld;


  printf("DL_parent_get %d %d\n",dbTyp,dbInd);
  // printf("  GR_TAB_IND = %d\n",GR_TAB_IND);


  //----------------------------------------------------------------
  // first get DL-index of child dbTyp/dbInd
  for(l1=GR_TAB_IND-1; l1>=0; --l1) {
      printf("  %d .typ=%d .ind=%d .modInd=%d\n",l1,GR_ObjTab[l1].typ,
              GR_ObjTab[l1].ind,(signed short)GR_ObjTab[l1].modInd);
    if(GR_ObjTab[l1].typ != dbTyp) continue;
    if(GR_ObjTab[l1].ind != dbInd) continue;
    if((signed short)GR_ObjTab[l1].modInd != -1) continue;  // skip submodels
    iChld = l1;
    goto L_get_par;
  }

  TX_Error("DL_parent_get E001");
  return -1;

  //----------------------------------------------------------------
  L_get_par:
    printf("  iChld=%d\n",iChld);
  // get parentRecord of iChld;
  for(l1=iChld-1; l1>=0; --l1) {
    if(GR_ObjTab[l1].sPar != 1) continue;  // skip all child-records
    if(GR_ObjTab[l1].iatt != iChld) continue;  // skip all child-records
    if((signed short)GR_ObjTab[l1].modInd != -1) continue;  // skip submodels
      printf("  ex DL_parent_get %d %d %d\n",GR_ObjTab[l1].ind,dbTyp,dbInd);

    return GR_ObjTab[l1].ind;
  }



  TX_Error("DL_parent_get E002");
  return -1;

}


///================================================================
  int DL_parent_hide (int typ, long iPar) {
///================================================================
/// find parentRec in DL (= zu hidender DL-Rec; typ/Ind of parentObj);
/// iatt des parentRec erhaelt den DL-Index des neuen DL-Record.
/// hide parentRec: set autoHid of parentRec to ON;
/// Das reaktivate Parent macht DL_disp_reset().

  long   lPar, lChild;

  // printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n");
  // printf("DL_parent_hide %d %d\n",typ,iPar);


  // get lChild = active DL-Index = child-DL-Ind
  lChild = GL_GetActInd ();

  // get lPar = DL-Index of parent
  lPar = DL_find_obj (typ, iPar, lChild-1);
  if(lPar < 0) {
    // TX_Print("DL_parent_hide E001 %d %d",typ,iPar);
    // printf("DL_parent_hide I000 %d %d\n",typ,iPar);
    return -1;
  }

  GR_ObjTab[lPar].iatt = lChild;
  GR_ObjTab[lPar].sPar = 1;
  // muss unsichtbar sein; im View-mode kann es wegen des falschen iatt
  // nicht abgezeigt werden. Schoener ware dim+noPick; geht wegen iatt nicht.
  GR_ObjTab[lPar].unvis = 1; // unvis
  // GR_ObjTab[lPar].dim  = 0; // ON
  // GR_ObjTab[lPar].pick = 0; // not pickable


  // hide 
  // im Main einfach hide; im subModel delete notwendig !
  // printf("HHHHHHHHHHHHHHHHHHHHHHH _hideParent %d\n",lPar);

  
  if((signed short)GR_ObjTab[lPar].modInd == -1) {
    // DL_hili_off (lPar);    // hili  = OFF; disp  = ON;
    DL_hide__ (lPar, 1);  // hide=OFF   hili = OFF; disp = OFF;

  } else {
    // Parent ist in der Kette von obj's fuer das Ditto; das per callList 
    // gerufen wird; muss daher explizit geloescht werden ...
    // glDeleteLists (lPar+DL_base__, 1);
    GL_Del0 (lPar);
  }


  return 0;

}
*/


//================================================================
  int DL_parent_ck_p (long dli) {
//================================================================
// DL_parent_ck_p       check if DL-record is a parent-object (check .sPar-Flag)
// RetCod: 1 - obj is parent;
//         0 - obj is not parent.

  int    ipar;

  ipar = GR_ObjTab[dli].sPar;

  return ipar;

}


//================================================================
  int DL_parent_ck__ (int *tabSiz, ObjDB *parTab,
                      int typ, long dbi, long dli) {
//================================================================
/// \code
/// DL_parent_ck__          get all parents for obj (typ,dbi,dli)
/// Input:  
///   tabSiz          size of parTab
///   typ,dbi,dli
/// Output:
///   parTab[tabSiz]
/// \endcode

  int    irc, maxSiz;


  maxSiz = *tabSiz;
  *tabSiz = 0;


  // printf("DL_parent_ck__  typ=%d dbi=%ld dli=%ld\n",typ,dbi,dli);


  L_nxt:

  // search parent for obj dli
  if(DL_parent_ck_c (dli)) {
      // printf(" .. has parent !\n");

    // get parent-dbi for obj (typ,dbi)
    irc = GA_parent_get (&dbi, typ, dbi);
    if(irc < 0) { printf("parentError dbi %ld\n",dli); goto L_exit; }

    // get dli from (typ,dbi)
    dli = DL_find_obj (typ, dbi, dli);
    if(dli < 0L) { printf("parentError dli %ld\n",dli); goto L_exit; }

    // add obj to list
    parTab[*tabSiz].typ   = typ;
    parTab[*tabSiz].dbInd = dbi;
    parTab[*tabSiz].dlInd = dli;
    *tabSiz += 1;
    if(*tabSiz >= maxSiz) { printf("parentError maxSiz\n"); goto L_exit; }
    goto L_nxt;
  }


  L_exit:

    for(irc=0;irc<*tabSiz;++irc)printf(" parTab[%d] typ=%d dbi=%ld\n",
      irc, parTab[irc].typ, parTab[irc].dbInd);

  return 0;

}

 
//================================================================
  int DL_parent_ck_c (long dli) {
//================================================================
// DL_parent_ck_c       check if DL-record has a parent-object (check .sChd-Flag)
// RetCod: 1 - obj has parent;
//         0 - obj does not have parent.

  int    ichd;

  ichd = GR_ObjTab[dli].sChd;

  return ichd;

}


//================================================================
  int DL_parent_hide (int typ, long pInd, long cInd) {
//================================================================
/// store childInfo, parentInfo; hide parent.

  long   lPar, lChild;


  // printf("DL_parent_hide typ=%d cInd=%d pInd=%d\n",typ,cInd,pInd);


  // get lChild = active DL-Index = child-DL-Ind
  lChild = GL_GetActInd ();
    // printf(" lChild=%d\n",lChild);



  // set child-bit
  GR_ObjTab[lChild].sChd = 1;
    // DL_DumpObj__ (lChild);


  // get lPar = DL-Index of parent
  lPar = DL_find_obj (typ, pInd, lChild - 1);
    // printf(" lPar=%d\n",lPar);
  if(lPar < 0) {
    // TX_Print("DL_parent_hide E001 %d %d",typ,iPar);
    // printf("DL_parent_hide I000 %d %d\n",typ,iPar);
    return -1;
  }
    // DL_DumpObj__ (lPar);


  GR_ObjTab[lPar].sPar = 1;
  // muss unsichtbar sein; Schoener waere dim+noPick ?
  GR_ObjTab[lPar].unvis = 1; // unvis
  // GR_ObjTab[lPar].dim  = 0; // ON
  // GR_ObjTab[lPar].pick = 0; // not pickable


  // hide 
  // im Main einfach hide; im subModel delete notwendig !
  // printf("HHHHHHHHHHHHHHHHHHHHHHH _hideParent %d\n",lPar);

  if((signed short)GR_ObjTab[lPar].modInd == -1) {
    // DL_hili_off (lPar);    // hili  = OFF; disp  = ON;
    DL_hide__ (lPar, 1);  // hide=OFF   hili = OFF; disp = OFF;

  } else {
    // Parent ist in der Kette von obj's fuer das Ditto; das per callList 
    // gerufen wird; muss daher explizit geloescht werden ...
    // glDeleteLists (lPar+DL_base__, 1);
    GL_Del0 (lPar);
  }


  // create the PartTab-Record
  GA_parent_set (typ, cInd, pInd);

  return 0;

}


//================================================================
  int DL_disp_reset (int lNr) {
//================================================================
/// alle Ausgabeobjekte der APT-Zeilen ab (inclusive) lNr loeschen

  long    l0, l1, l2, dli, dbi;


  // printf("''''''''''''DL_disp_reset %d %d\n",lNr,GR_TAB_IND);
  // DL_DumpObjTab();


  if(PRC_IS_ACTIVE) {                 // 2013-04-03
    l0 = APP_dli_start;
  } else {
    l0 = 0;
  }


  //----------------------------------------------------
  // dli = DL-Index suchen, ab dem geloescht wird ..
  for(l1=l0; l1<GR_TAB_IND; ++l1) {
    // printf(" [%d]=%d\n",l1,GR_ObjTab[l1].lNr);

    // skip all objects not belonging to the active model
    // if((signed short)GR_ObjTab[l1].modInd != -1) continue;  // skip submodels
    if((signed short)GR_ObjTab[l1].modInd != WC_modnr) continue;

    if(GR_ObjTab[l1].lNr < lNr) continue;
    // if(GR_ObjTab[l1].lNr <= lNr) continue; // DO NOT DELETE LAST OBJ
    // if(GR_ObjTab[l1].typ == Typ_Color) continue;
    dli = l1;
    goto L_del1;
  }

  // nix zu loeschen ..
  // if(dli < 0) return 0;
  return 0;




  //----------------------------------------------------
  // find DL-recs with sPar==1;
  //   iatt of this rec = DL-Ind of childRec (= following DL-Rec)
  //   wenn childRec geloescht wird (im zu loeschenden DL-Bereich liegt),
  //     muss man den alten reaktivieren.
  //   reaktivieren parentRec:
  //     iatt of parentRec = iatt of childRec;
  //     sPar of parentRec==0.

  L_del1:
    // printf(" _disp_res dli=%d\n",dli);

/*  old version ..
  for(l1=dli-1; l1>=0; --l1) {
    if(GR_ObjTab[l1].sPar != 1) continue;
      // printf(" parentRec %d iatt=%d\n",l1,GR_ObjTab[l1].iatt);
    if(GR_ObjTab[l1].iatt < dli) continue;
    if((signed short)GR_ObjTab[l1].modInd != -1) continue;  // skip submodels

    // printf("RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR \n");
    l2 = GR_ObjTab[l1].iatt;     // l1=parent, l2=child.
    // printf(" - reactiv. %d %d\n",l1,l2);
    GR_ObjTab[l1].iatt  = GR_ObjTab[l2].iatt;
    GR_ObjTab[l1].sPar  = 0;
    GR_ObjTab[l1].unvis = 0; // vis
    DL_hide__ (l1, 0);  // hide=OFF   hili = OFF; disp = OFF;
  }
*/

  for(l1=GR_TAB_IND-1; l1>=dli; --l1) {
    if(GR_ObjTab[l1].sChd != 1) continue;
    // get dbi of parent of this child
    GA_parent_get (&dbi, GR_ObjTab[l1].typ, GR_ObjTab[l1].ind);
    if(dli < 0) continue; // {TX_Print("DL_disp_reset E001"); break;}
    // get dli of parent of this child
    l2 = DL_find_obj (GR_ObjTab[l1].typ, dbi, dli);
    if(l2 < 0) continue; // {TX_Print("DL_disp_reset E002"); break;}
    // reset parent
    GR_ObjTab[l2].sPar  = 0;
    GR_ObjTab[l2].unvis = 0; // vis
    DL_hide__ (l2, 0);  // hide=OFF   hili = OFF; disp = OFF;
  }





  //----------------------------------------------------
  // delete refModels
  for(l1=GR_TAB_IND-1; l1>=dli; --l1) {
    if(GR_ObjTab[l1].typ != Typ_Model) continue;
      // printf(" _disp_reset del RefMod %d\n",GR_ObjTab[l1].ind);
    DB_Delete (GR_ObjTab[l1].typ, GR_ObjTab[l1].ind);  // delete ModRef
  }




  //----------------------------------------------------
  // delete basModels
  // WIRD VON GL_Delete ERLEDIGT; DAHER HIER WIEDER RAUS ..

  // scheinbar bei allen DL-Records gleiche modnr; daher keine Loop
  // erforderlich ?

  // if((signed short)GR_ObjTab[dli].modInd == -1) goto L_done;
  // // delete basicmodel
  // l1 = GR_ObjTab[dli].modInd;
  // printf(" _disp_reset del BasMod %d\n",l1);
  // DB_del_ModBas (l1);




  //----------------------------------------------------
  // und die DispList zuruecksetzen;
  // dli Record und alle folgenden werden geloescht!
  L_done:
  GL_Delete (dli);


  return 0;

}


//================================================================
  int DL_setRefSys (long dli) {
//================================================================
/// RefSys entprechend DL-Record setzen


  // printf("DL_setRefSys %d\n",dli);

  if(dli < 1) {
    if(WC_sur_ind == 0) return 0;
    NC_setRefsys (0);
    return 0;
  }

  if(dli > GR_TAB_IND) {printf("DL_setRefSys E001 %ld\n",dli); return -1; }

  // printf("  _setRefSys irs=%d %d\n",GR_ObjTab[dli].irs,WC_sur_ind);

  if(GR_ObjTab[dli].irs == WC_sur_ind) return 0;

  NC_setRefsys (GR_ObjTab[dli].irs);

  return 0;

}
 

//================================================================
  int DL_get__ (DL_Att **dl) {
//================================================================
/// DL_get__                returns DispList


  if(dl) *dl = GR_ObjTab;

  return GR_TAB_IND;

}


/* UNUSED
//================================================================
  int DL_get_iLast (long *dli, int *typ, int lNr) {
//================================================================
/// find DL-index of last line before APT-LineNr lNr in DL.
/// typ unused.

  long   l1, lN1;


  lN1 = 0;
  for(l1=0; l1<GR_TAB_IND; ++l1) {
    if(GR_ObjTab[l1].lNr > lNr) continue;
    if((signed short)GR_ObjTab[l1].modInd != -1) continue;  // skip submodels
    if(GR_ObjTab[l1].lNr > lN1) {
      lN1 = GR_ObjTab[l1].lNr;
      *dli = l1;
    }
  }

  // printf("ex DL_get_iLast dli=%d lNr=%d\n",*dli,lNr);

  return 0;

}
*/


//================================================================
  int DL_disp_hili_last (int mode) {
//================================================================
/// DL_disp_hili_last       hilite last obj of DL


  if(mode == ON) {
    // hilite last obj of DL
    DL_hili_on (-2L);

  } else {
    // unhilite 
    DL_hili_off (-2L);
  }

  return 0;

}


/* UNUSED
//=======================================================================
  int DL_disp_hili (long lNr) {
//=======================================================================
/// \code
/// hilite Obj of line lNr
/// lNr ist die APT-LinNr
/// see also IE_cad_Inp_disp AP_hili_obj
/// see also DL_get_iLast DL_hili_on DL_hili_off
/// \endcode

// DL_disp_hili und AP_hili_obj machen einiges doppelt - sollten zu einer
//   Funktion gemacht werden !


  int    irc, i1, ind, typ;
  long   l1, l2, dli;
  char   *p1, *p2, o1[OBJ_SIZ_MAX];


  printf("````````````DL_disp_hili %ld\n",lNr);
  // DL_DumpObjTab ();
  // DL_DumpObj__ (lNr);



  // loeschen der Hili-Tabelle
  DL_hili_on (-1L);


  // get dli from sourceLineNr
  l1 = lNr;
  irc = DL_Get_dli_lNr (&dli, &l1);
    printf(" DL-ind of last obj = %ld\n",dli);


  if(irc == 0) {
    DL_hili_on (l1);

  } else {
    // Vectors, Trafos haben kein Obj in der DL.
    // printf("---- _hili %d\n",lNr);
    p1 = UTF_GetPosLnr (&l2, lNr);  // get startpos of Line ...
    if(p1 == NULL) return 0;

    // skip commentline
    if(*p1 == '#') goto L_done;
    // skip empty line
    if(*p1 == '\n') goto L_done;

    // test for Definitionline
    p2 = strchr(p1, '=');
    if(p2 == NULL) return 0;
    i1 = p2 - p1 + 1;     // das '=' muss auch mitgehen (f.VectorDisp)
    if(i1 > 32) return 0;
    strncpy(o1, p1, i1);  // copy ObjID --> o1
    o1[i1] = '\0';
    AP_hili_obj (o1);     // hilite obj m. ID o1

//     if(dli > 1020) return 0;
//     strncpy(cbuf, p1, dli);
//     cbuf[dli] = '\0';
//     p1 = cbuf;
//     i1 = APED_dbo_oid (&typ, &ind, p1);
//     if(i1 < 0) return 0;
//     p2 = strchr(p1, '=');
//     if(p2 == NULL) return 0;
//     ++p2;
//     // i1 = IE_decode_Ln (aus_typ, (void*)aus_tab, 10, p2);
//     i1 = APT_obj_expr (o1, typ, p2);
//     if(i1 < 0) return 0;
// 
//     if(typ == Typ_VC) {
//       // i1 = APT_decode_vc (o1, i1, aus_typ, aus_tab);
//       // if(i1 < 0) return 0;
//       dli = -2;
//       GL_DrawVc1 (&dli, 12, NULL, o1);
// 
//     } else if(typ == Typ_Tra) {
//       UT3D_stru_dump (Typ_ObjGX, o1, "_tra=");
//       UI_disp_tra (o1);
//     }

  }


  // L_fertig:
  L_done:
  // DL_Redraw();    // kommt eh noch ..

  return dli;

}
*/


//================================================================
  long DL_SetObj (int typ, long dbi) {
//================================================================
// DL_SetObj               store DL-record (only for active vector)


  GR_ObjTab[GR_TAB_IND].typ    = typ;
  GR_ObjTab[GR_TAB_IND].ind    = dbi;


  return GR_TAB_IND;

}


/* ===================== EOF ======================== */
