// ut_DL.c    displayList functions     RF
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
...................
Display passive-objects following active-objects in gray;
  - GL_Redraw: ignore subModels - records with (modInd >= 0);
    - display all (DL-recods <= AP_lnr_act) active;
    - display all (DL-recods > AP_lnr_act) inActive;

...................
Images in subModels cannot be selected at the moment (seeDL_txtSelect)



-----------------------------------------------------
Modifications:
2002-02-12 DL_disp_* zu. RF.
2001-06-06 DL_Lay_mod (war DB_Lay).
2001-05-04 DL_Delete, DL_dbTyp__dli, DL_get_dbi neu.
2000-10-17 Vereinheitlichung mit d. Unix-Variante.

-----------------------------------------------------
*/
#ifdef globTag
 void DL(){}
#endif
/*!
\file  ../gr/ut_DL.c
       ../gr/ut_DL.h
\brief displayList functions                              see INF_DL__
\code
=====================================================
List_functions_start:

DL_Init
DL_InitAttTab           load file ltyp.rc into GR_AttTab and create DL-record
DL_InitAttRec           define a new lineattributeRecord
DL_alloc__              realloc space fuer BasModelnames

DL_perm_init            create DL-record for permanent storage
DL_tDyn_init            create DL-record for temporary-dynamic storage
DL_temp_init            get GL-index for temporary storage; glNewList();
DL_temp_ind_set         set DL_temp_ind
DL_att_temp             set temporary-obj to hilite or dim
DL_att_mdr              set perm/-refModels to hilite or dim

DL_dli_get              get DispListIndex for permanent obj
DL_set__                store DL-record
DL_SetInd               modify (do not create new DL-Record); set DL_perm_ind=dli;
DL_SetObj               get or set disp-list-record.
DL_StoreObj             Den naechsten freien DispList-Platz belegen
DL_SetTmpObj            store DL-record (only for active vector)

DL_AttLn_store          store line-attribute in table GR_AttTab
AttLn_Set1              get line-attribute from 3 integers
AttLn_Get_ind           get line-attribute from GR_AttLnTab-index 
IndAttLn_Set1           set linetyp, limitTyp
IndAttLn_get_ltyp       get linetyp

DL_Redraw               redraw complete DL
DL_Draw_obj             redraw single obj, use existing DL-record
DL_Stat                 ausgabe Statistik

DL_get__                returns DispList
DL_dlRec__dli           get DL-record (DL_Att from GR_ObjTab[objInd]) from dli
DL_oSrc_dli             get ObjSRC from DispListRecord (DL_Att)
DL_get_dbi              get DB-index from DL-Index
DL_Get_GrAtt            get graf.Att (GR_Att from GR_AttTab[Ind])
DL_get_col              get ColRGB* of DL-record (surf)
DL_get_sStyl            get surfaceStyle (shaded|symbolic)
DL_get_iatt             returns iatt of DL-record
DL_set_iatt             modify iatt of DL-record
DL_get_irs              get active ConstrPlane (.irc of last obj)
DL_dbTyp__dli           get obj-typ from DL-ind
DL_dli__oid             get DispListIndex and source-line-nr from obj-ID
DL_oid__dli             get objName from DispListIndex
DL_Get_lNr_dli          get sourceLineNumber from DispListIndex
DL_Get_dli_lNr          get DispListIndex from sourceLineNumber
DL_GetTrInd             get refSys-Index from dli
DL_GetPick              get if pickable; 0=unpickable, 1=pickable
DL_GetNrSur             get nr of surs in DispList
DL_dli__dbo             Objekt typ=typ APTind=ind in der DL suchen
DL_find_smObj           get dispListIndex of DB-obj from typ/dbi/subModelNr
DL_find_APPOBJ          find applicationObject
DL_lnr_incr             increment all (lineNrs > lNrX)

DL_hili_on              set obj hilited
DL_hili_off             reset hilited
DL_OBJ_IS_HILITE        check if DL-obj is hilited
// DL_disp_hili_last       (change) hilite last obj of DL
// DL_disp_hili            hilite Obj of line lNr

DL_dim_on               set obj dimmed
DL_dim_off              reset obj dimmed -> normal
DL_dim_all              set all objs dimmed

DL_hide__               change hidden/visible for single obj
DL_hide_chg             change hidden/visible for all objs
DL_hide_all             change all active, visible objs to hidden|visible
DL_disp_def             fuer alle nun folgenden Obj GR_ObjTab.disp=mode setzen
DL_hide_unvisTypes      view or hide all joints,activities.

DL_del_last             free (delete) last DL-obj
DL_disp_reset           delete all DL-objects starting from line-nr
DL_unvis__              set visible / unvisible

DL_pick__               change all objects to pickable or unpickable
DL_pick_set

DL_parent_ck_c          check if DL-record has a parent-object (check .sChd-Flag)
DL_parent_ck_p          check if DL-record is a parent-object (check .sPar-Flag)
DL_parent_hide
DL_parent_set           set/reset isParent-bit;
DL_child_set            set/reset isChild-bit;

DL_grp1_set             Zugehoerigkeit zu Group 1 setzen; ON od OFF
DL_grp1_get             get grp_1-bit
DL_grp1__               add / remove (change) Groupbit 1 of DL-Record ind
DL_grp1_copy            copy all DL-obj with groupBit ON --> GroupList
DL_grp1_nr_get          count nr of objs in group

DL_ck_typ_dbi           test DisplistRecord - typ and dbi
DL_dbi_is_visTyp        test if typ == visual typ (VC is not)
DL_IS_HIDDEN            test if obj is hidden                           INLINE
DL_OBJ_IS_HIDDEN        test if obj is hidden                           INLINE
DL_OBJ_IS_ACTMDL        test if obj belongs to active model             INLINE

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
// DL_setTagSiz            bei tags und Bitmaps die size im DL-record speichern

DL_sav_dynDat           save the DYNAMIC_DATA of the actual mainModel
DL_load_dynDat          reload the DYNAMIC_DATA
DL_wri_dynDat0
DL_wri_dynDat1

DL_scale_grp            scale group
DL_ReScale__            rescale and redraw
DL_ReScale_Notes
DL_ReScale_pt_get       uxmin-uzmax erweitern um Box
DL_ReScalePoint
DL_ReScaleObj

DL_DumpObjTab           dump complete DL
DL_DumpObj__            dump single DL-record

List_functions_end:
=====================================================
UNUSED
DL_find_sel             find selected object in DL

\endcode */
#ifdef globTag
 void INF_DL__(){}
#endif
/*
//================================================================
  DL-displist
//================================================================

GR_ObjTab[GR_TAB_IND] is the DL-displist.
  each record keeps the objType (.typ) and its DB-index (.ind) ..
    Struct = see DL_Att  Size = GR_ATT_TAB_SIZ
  FOR EVERY DL-record a GL-record is necessary !
    the GL-index is set by GL_fix_DL_ind();
    create the GL-record with glNewList
    GL can redraw this record.

dli is index of a displist-record;  -1L - dli is not valid

Normal obj's have a DB-record and a DL-record;
  The GL-index of this objects is (GL-ind + DL_base__)

Temporary obj's have no DB-record and no DL-record but a GL-record.
  The GL-index for temp-obj's is (> 1; < DL_base_font1).

Functions: DL_SetInd      preset DL-index (for using existing DL-record)
           DL_StoreObj    create or overwrite DispList-record; return its index
           GL_fix_DL_ind  get GL-index from DL-index (add eg. DL_base__)
                          see INF_GL_offTab

testoutput of DL-displist with Alt-shift-D

GL:  see INF_GL__ INF_GL_offTab


//================================================================
INIT:

  DL_InitAttRec (ind, col, ltyp, lthick); // ein neues Attribut definieren.
  // Die ersten 13 Entries sind vordefiniert; 
    DL_InitAttRec (Typ_Att_def,    1, 0, 4);

  Die Standardattrib's    - see Func. DL_InitAttTab() ex File ltyp.rc

----------------------------------------------------------------------------
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
  GR_temp_rect1 (&l1, ..);
  ..
  GL_temp_del_1 (2L);  // delete this temp obj


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
#include "../ut/ut_cast.h"               // INT_PTR
#include "../ut/ut_memTab.h"           // MemTab
#include "../ut/ut_TX.h"                 /* TX_Error */
#include "../ut/ut_txfil.h"              // UTF_clear1
#include "../ut/ut_txt.h"                // fnam_del
#include "../ut/ut_os.h"                 // OS_ ..
#include "../ut/ut_col.h"                // COL_INT32

#include "../ut/func_types.h"                 // Typ_Att_def, SYM_TRI_S, ..
#include "../ut/func_types.h"                 // Typ_Att_def, SYM_TRI_S, ..
#include "../gr/vf.h"                    // GL_vf1_CS

#include "../db/ut_DB.h"                 // DB_GetPoint ..


#include "../gr/ut_GL.h"
#include "../gr/ut_DL.h"
#include "../gr/ut_gr.h"

#include "../xa/xa_mem.h"              // mem_cbuf1
#include "../xa/xa_uid.h"              // UID_ckb_view
#include "../xa/xa_obj_txt.h"          // AP_obj_add_val
#include "../xa/xa.h"                  // AP_Get_ConstPl_Z
#include "../xa/xa_app.h"              // PRC_IS_ACTIVE
#include "../xa/xa_msg.h"              // DEB_mcheck__
#include "../xa/xa_ed_mem.h"           // typedef_MemTab(ObjSRC)
// #include "../xa/opar.h"                // MemTab_ini_temp


/*=============== Externe Variablen: =======================================*/
// ex ../xa/xa.h:
extern AP_STAT    AP_stat;                    // sysStat,errStat..
extern  int       WC_sur_ind;            // Index auf die ActiveConstrPlane
extern Plane      WC_sur_act;     // die Konstruktionsebene
extern double     AP_txsiz;       // Notes-Defaultsize
extern double     AP_txdimsiz;    // Dimensions-Text-size
extern int        AP_txNkNr;            // Nachkommastellen

extern ColRGB     AP_defcol;


// ex ../ci/NC_Main.c:
extern double APT_ModSiz;
extern int        GR_pick;                     // NOPICK
// extern int        GR_lay_act;
extern int     APT_lNr;            // LineNr of last-processed-obj
extern int     APT_hidd;           // if last-processed-obj is hidden;
                                   // -1=unknown,0=normal-not-hidden, 1=hidden
extern long    AP_dli_act;      // index dispList
extern int     UP_level;
extern int     AP_mdLev;
extern int     APT_dispPT, APT_dispPL;


// ex ../gr/ut_GL.c:
extern int     GL_modified;
extern double  GL2D_Scale;
extern long    DL_base__;        // first index of normal objects


// ex ../gr/ut_gtx.c:
extern double GR_tx_scale;


// ex ../xa/xa_ui.c:
extern int       UI_InpMode;


// ex ../xa/xa_ui_gr.c
extern long   GR_dli_hili;     // the active (mouse-over) object of selection-list




/*=============== Lokale Variablen: =======================================*/
DL_Att     *GR_ObjTab = NULL;            // das ist die DL
long       GR_TAB_SIZ = 0;               // momentane size of GR_ObjTab
long       GR_TAB_INC = 10000;           // beim realloc vergroessern um -
long       GR_TAB_IND = 0;               // next free rec in GR_ObjTab

int    DL_tempLst[DL_base_font1];    // dispList for temporary-objs
long   DL_temp_nxt = 1;       // next free temp-index (1-DL_base_font1)
long   DL_temp_ind = 0;      // if(>0) fixed temp-index to use; 0: get next free.

long   DL_perm_ind = -1L;    // if(>=0) fixed perm-index to use; -1=get next free


Att_ln     GR_AttLnTab[GR_ATT_TAB_SIZ];  // line-attributes (dash, color ..)
// GR_Att     GR_AttTab[GR_ATT_TAB_SIZ];    // die AttributTabelle
static int GR_AttInd=0;                  // Anzahl = naechster freier Index





static double uxmin=0.,uymin=0.,uzmin=0.;      // links unten Userkoords
static double uxmax,   uymax,   uzmax;         // rechts oben in Userkoords


       int    DL_disp_act;          // der Status des Hide-Attribut .disp

static long   DL_hidden = -1L;



//================================================================
  int DL_lnr_incr (long lNrX) {
//================================================================
// DL_lnr_incr             increment all (lineNrs >= lNrX)

  long     l1, l2;


  // printf("DL_lnr_incr %ld\n",lNrX);

  for(l1=0; l1<GR_TAB_IND; ++l1) {
    if(GR_ObjTab[l1].lNr < lNrX) continue;
    for(l2=l1; l2<GR_TAB_IND; ++l2) GR_ObjTab[l2].lNr += 1;
    break;
  }

  return 0;

}


/* replaced by GR_img_get_dbi
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
///
/// See also DL_setTagSiz
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
    printf(" tagSiz=%d %d\n",*sx,*sy);

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
  // DEB_dump_obj__(Typ_PT, p1, " TagposLU=");

  return 0;

}
*/

//====================================================================
  int DL_txtSelect (int iNr, ObjDB **dlTab) {
//====================================================================
// DL_txtSelect       check if image was selcted and add obj to tables
// - 2D-text, tags, symbols can be found by opengl-select ..
// - but images are not found; get here its position and extents
//   and check if curPos is inside image.
// TODO - BUG_Select-sm-image
//   if image is in subModel, position and extents cannot be found
//
// Input:
//   iNr        nr of records already in dlTab
// Output:
//   dlTab      a pointer to GR_selTab

  int    irc, i1, mx, my, sx, sy, dx, dy, tx1, ty1, tx2, ty2, typ;
  long   dli;
  double px, py, pz;
  Point  p1;


  // printf("DL_txtSelect %d\n",iNr);
  // GL_sel_dump (iNr);  // dump GR_selTab



  // active Mousepos in ScreenCoords is GL_mouse_x_act/GL_mouse_y_act
  GL_get_curPos_SC (&mx, &my);
    // printf(" mousePos=%d %d\n",mx,my);


  // check if tag are in DL
  for(dli=0; dli<GR_TAB_IND; ++dli) {

    // check for AText (2D-text, tag, bitmap, symbol ..)
    if(GR_ObjTab[dli].typ != Typ_ATXT) continue;
    // if(GR_ObjTab[dli].typ != Typ_Tag) continue;

      // DL_DumpObj__ (dli, "------------ txtSelect");

    // skip hidden obj
    // if((GR_ObjTab[dli].disp==1)&&(GR_ObjTab[dli].hili == 1)) continue;
    if(DL_IS_HIDDEN(GR_ObjTab[dli])) continue;    // skip hidden obj's
    // if(GR_ObjTab[dli].del  != 0) continue;         // skip deleted
    // if(GR_ObjTab[dli].pick != 0) continue;         // skip unpickable


    // check if belongs to subModel
    if(DL_OBJ_IS_ACTMDL(GR_ObjTab[dli])) {
      // TODO - BUG: wee cannot find position and extents .. skip it ..
        // printf(" ************ belongs to subModel %d\n",
               // (signed short)GR_ObjTab[dli].modInd);
      // iNr = GL_sel_add_DL (dlTab, dli);
      goto L_9;
    }


    // irc = DL_txtgetInfo (&typ, &p1, &sx, &sy, &dx, &dy, dli);
    // SizeInfo zu Tag/Image holen
    irc = GR_img_get_dbi (&typ, &p1, &sx, &sy, &dx, &dy, GR_ObjTab[dli].ind);
    if(irc < 0) continue;  // zB SymbolTags; werden normal auch gefunden.

      // printf(" tag-dli=%ld p1=%f,%f,%f\n",dli,p1.x,p1.y,p1.z);
      // printf(" tag-sx=%d sy=%d dx=%d dy=%d\n",sx,sy,dx,dy);

    // change Textpoint --> Screencoords
    GL_Uk2Sk (&px, &py, &pz, p1.x, p1.y, p1.z);
      // printf(" px=%f py=%f pz=%f\n",px,py,pz);

    // ty1,ty1 --> lower left corner
    tx1 = px + dx;
    ty1 = py + dy;
      // printf(" LL: tx1=%d ty1=%d\n",tx1,ty1);

    // tx2,ty2 - upper right corner
    tx2 = tx1 + sx;
    ty2 = ty1 + sy;
      // printf(" UR: tx2=%d ty2=%d\n",tx2,ty2);


    // check if selectPosition is in Texlabel.
    // mx,my ist nun links unten; sx,sy ist size.
    // printf(" siz=%d %d\n",GR_ObjTab[dli].refInd,GR_ObjTab[dli].attInd);
    if(mx < tx1) continue;                         // Maus links vom Label
    if(mx > tx2) continue;  // Maus rechts vom Label
    if(my < ty1) continue;  // Maus unterhalb vom Label
    if(my > ty2) continue;  // Maus oberhalb vom Label


    // printf(" IIIIIIIIIIIIII found N%d IIIIIIIIII %d %d\n",
             // GR_ObjTab[dli].ind,dx,dy);
    // check if selction already defined; passiert bei Tags;
    // Punkt und Tag (hier!) werden gefunden !
    for(i1=0; i1<iNr; ++i1) {
        // printf(" dl[%d]=%d\n",i1,(*dlTab)[i1].dlInd);
      if((*dlTab)[i1].dlInd == dli) goto L_9;                      // 2010-10-21
      // if(indTab[i1] != GR_ObjTab[dli].ind) continue;
      // if(AP_typDB_typ(typTab[i1]) != Typ_GTXT) continue;
      // typ = (*dlTab)[i1].typ;
        // printf(" typ=%d\n",typ);
      // if(AP_typDB_typ(typ) != Typ_Note) continue;        // 2009-12-31
      // printf(" double sel.typ=%d ind=%d\n",typTab[i1],indTab[i1]);
      // goto L_9;
    }


    // add selection
      // printf(" add1 sel dli=%d\n",dli);

      // printf("- DL_txtSelect add2 sel dli=%d\n",dli);
    // if(iNr < 0) iNr = 0;
    // if(iNr > 50) break;
    iNr = GL_sel_add_DL (dlTab, dli);
    // typTab[iNr] = Typ_Tag;
    // indTab[iNr] = GR_ObjTab[dli].ind;
    // dlTab[iNr]  = dli;
    // ++iNr;

    L_9:
    continue;
  }

    // TESTBLOCK
    // printf("ex-DL_txtSelect %d\n",iNr);
    // GL_sel_dump (iNr);
    // END TESTBLOCK

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

/*
//================================================================
  int DL_setTagSiz (long dli, int xsiz, int ysiz) {
//================================================================
/// \code
/// store size of tags and Bitmaps in the DL-record
/// See also DL_txtgetInfo
/// \endcode

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
*/

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

/*
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

  // save MODSIZ (APT_ModSiz) / MODBOX / UT_TOL_cv UT_DISP_cv
  fwrite(&APT_ModSiz, sizeof(double), 1, fp1);
  fwrite(&AP_box_pm1, sizeof(Point), 1, fp1);
  fwrite(&AP_box_pm2, sizeof(Point), 1, fp1);
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
  double view[10], d1, d2, d3;

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
  fread(&d1, sizeof(double), 1, fp1);            // APT_ModSiz
  fread(&AP_box_pm1, sizeof(Point), 1, fp1);
  fread(&AP_box_pm2, sizeof(Point), 1, fp1);
  fread(&d2, sizeof(double), 1, fp1);            // UT_TOL_cv
  fread(&d3, sizeof(double), 1, fp1);            // UT_DISP_cv

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

  APT_ModSiz = d1;
  GL_InitModelSize (APT_ModSiz, 1);
  UT_TOL_cv  = d2;
  UT_DISP_cv = d3;

  // printf("ex DL_load_dynDat UT_DISP_cv=%f\n",UT_DISP_cv);

  return 0;

}
*/

//===============================================================
  int DL_wri_dynDat0 (FILE *fpo) {
//===============================================================
/// write :DYNAMIC_DATA

  int i1;
  // double view[10];

  // printf("DL_wri_dynDat0 \n");

  // write MODSIZ DEFTX DEFCOL VIEW CONST_PL  :DYNAMIC_DATA
  DL_wri_dynDat1 (fpo, 0);

  // add GA-Table (HIDE/G#/..)
  GA_fil_wri (fpo, 0, 0, 1);

  fprintf(fpo, ":DYNAMIC_DATA\n");

    // printf("ex DL_wri_dynDat0 \n");

  return 0;

}


//===============================================================
  int DL_wri_dynDat1 (FILE *fpo, int mode) {
//===============================================================
/// \code
/// write MODSIZ DEFTX DEFCOL VIEW CONST_PL  :DYNAMIC_DATA
/// add :DYNAMIC_DATA  without attributes GA
/// Input mode: 0  write also MODBOX
///             1  skip MODBOX (group out)
/// \endcode

  int i1;
  double view[10];


  // printf("DL_wri_dynDat1 \n");


  // add Tolerances
    strcpy(mem_cbuf1, "MODSIZ");
    AP_obj_add_val (mem_cbuf1, APT_ModSiz);
    AP_obj_add_val (mem_cbuf1, UT_TOL_cv);
    AP_obj_add_val (mem_cbuf1, UT_DISP_cv);
      // printf(" _dynDat1|%s|\n",mem_cbuf1);
    fprintf(fpo, "%s\n", mem_cbuf1);


  // add boxPoints AP_box_pm1/2 -> file
  if(mode == 0) {       // not for group-export
    if(!UT3D_pt_isFree(&AP_box_pm1)) {
      strcpy(mem_cbuf1, "MODBOX");
      AP_obj_add_pt (mem_cbuf1, &AP_box_pm1);
      AP_obj_add_pt (mem_cbuf1, &AP_box_pm2);
        // DEB_dump_obj__ (Typ_PT, &AP_box_pm1, "AP_box_pm1");
        // DEB_dump_obj__ (Typ_PT, &AP_box_pm2, "AP_box_pm2");
        // printf(" _dynDat1|%s|\n",mem_cbuf1);
      fprintf(fpo, "%s\n", mem_cbuf1);
    }
  }

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
    AP_obj_add_val (mem_cbuf1, 0.);    // free; was WC_sur_Z);
    strcat(mem_cbuf1, " ");
    strcat(mem_cbuf1, AP_Get_ConstPl_Z(Typ_PLN));
      // DEB_dump_obj__ (Typ_PLN, &WC_sur_act, "WC_sur_act");
      // printf(" _dynDat1- WC_sur_Z=%lf\n",WC_sur_Z);
      // printf(" _dynDat1|%s|\n",mem_cbuf1);
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


//================================================================
  int DL_unvis__ (long dli, int mode) {
//================================================================
// DL_unvis__                   set/reset unvisible-bit;
// mode     1=unvisible, 0=visible=default

  // printf("DL_unvis__ %ld %d\n",dli,mode);
  // if(dli < 0) AP_debug__ ("DL_unvis__");

  GR_ObjTab[dli].unvis = mode; // unvis

  return 0;

}


//===============================================================
  void DL_hide__ (long ind, int mode) {
//===============================================================
/// \code
/// DL_hide__     change hidden/visible for single obj
///   mode    ON(0)    set visible
///           OFF(1)   set hidden
///
/// modify DL.disp, DL.hili
/// \endcode

//        hidden = ((disp == 1)&&(hili == 1))            // ON=0   OFF=1
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


//================================================================
  int DL_hide_all (int mode ) {
//================================================================
/// \code
/// DL_hide_all             change all active, visible objs to hidden|visible
/// mode     0 = hide_all;  1 = redisplay_all.
/// \endcode

  long  dli;

    
  // printf("DL_hide_all %d %d\n",mode,AP_modact_ibm);


  for(dli=0; dli<GR_TAB_IND; ++dli) {

    // skip invis. obj's
    if(GR_ObjTab[dli].unvis != 0) continue;

    // skip objects not in model <iMdl>:
    if((INT_16)GR_ObjTab[dli].modInd != AP_modact_ibm) continue;

    if(mode) {
      // redisplay: disp=0; hili=1: visible.
      GR_ObjTab[dli].disp  = 0;
      GR_ObjTab[dli].hili  = 1;
    } else {
      // hide: disp=1; hili=1: hidden.
      GR_ObjTab[dli].disp  = 1;
      GR_ObjTab[dli].hili  = 1;
    }
  }


  return 0;

}

 
//===============================================================
  int DL_hide_chg () {
//===============================================================
/// DL_hide_chg      change hidden/visible for all objs

// hidden: disp=1 
// ON=0   OFF=1

  long  ind;


  // printf("DL_hide_chg\n");


  for(ind=0; ind<GR_TAB_IND; ++ind) {

    if(GR_ObjTab[ind].disp == OFF) {
      if(GR_ObjTab[ind].hili == OFF) {
        // disp=1; hili=1: hidden; make visible
        GR_ObjTab[ind].disp  = ON;
        // disp=0; hili=1: Visible.
      } else {
        // disp=1; hili=0: hilited; make hidden
        GR_ObjTab[ind].hili  = OFF;
        // disp=1; hili=1. Hidden.
      }

    } else {
      // disp=0; hili=1: visible; make hidden
      GR_ObjTab[ind].disp  = OFF;
      // disp=1; hili=1. Hidden.
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
// DL_hide_unvisTypes        view or hide all points,planes,joints,activities.
// Input:
//   mode        0=view, 1=hide.
//   APT_dispPT  0=hide-points-in-VWR (Default)
//               1=display-points-in-VWR

// VWR:         1 = hide joints & activities;
//                  hide points if APT_dispPT=OFF
//                  hide planes if APT_dispPL=OFF
// CAD & MAN:   0 = view all

// see also DL_disp_PL

  int       iPL;   // 0=hide; 1=disp
  long      ind;


  // set iPL = mode for planes; 0=hide; 1=disp
  // CAD, MAN = 1=disp
  iPL = 1;
  if(UI_InpMode == UI_MODE_VWR) {
    if(APT_dispPL == ON) {
      iPL = 0; // hide
    }
  }

  // printf("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD \n");
  // printf("DL_hide_unvisTypes %d\n",mode);
  // printf("  UI_InpMode=%d APT_dispPT=%d APT_dispPL=%d iPL=%d\n",
         // UI_InpMode,APT_dispPT,APT_dispPL,iPL);


  //----------------------------------------------------------------
  // handle points
  if(UI_InpMode != UI_MODE_VWR) { 
    // CAD, MAN: view
    GL_InitPtAtt (0);

  } else {             // hide
    if(APT_dispPT == OFF) GL_InitPtAtt (0);   // view
    else                  GL_InitPtAtt (1);   // hide
  }



  //----------------------------------------------------------------
  // handle planes, joints ..
  for(ind=0; ind<GR_TAB_IND; ++ind) {

    // hide/view plane
    if(GR_ObjTab[ind].typ == Typ_PLN) {
      if(iPL) {
        // disp
        GR_ObjTab[ind].disp = 0;
        GR_ObjTab[ind].hili = 1;
      } else {
        // hide
        GR_ObjTab[ind].disp = 1;
        GR_ObjTab[ind].hili = 1;
      }
      continue;
    }

    // skip if hidden
    if(DL_IS_HIDDEN(GR_ObjTab[ind])) continue;

    // skip subModels
    // if((signed short)GR_ObjTab[ind].modInd != -1) continue;
    if(DL_OBJ_IS_ACTMDL(GR_ObjTab[ind])) continue;

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

  if(ind >= 0)
    GR_ObjTab[ind].unvis = mode;
  else
    printf("**** DL_unvis_set E001 %ld\n",ind);

  return 0;

}

//================================================================
  int DL_hili_MAN (int mode) {
//================================================================
// DL_hili_on           set last obj hilited / dimmed; do not Redraw yet.
// TODO: prepared for display objects following active object dimmed;
//       
// Input:
//   mode   TYP_FuncInit   set all hidden -> dimmed
//          TYP_FuncExit   set all dimmed -> hidden
//          TYP_FuncMod    set last obj hilited and reset previous hilited

  static long old = -1;
  static int  hid = -1;         // if old == hidden; 0=yes, 1=no.

  long     dli;

  // printf("--------------------------------- \n");
  // printf("DL_hili_MAN GR_TAB_IND=%ld old=%ld hid=%d mode=%d\n",GR_TAB_IND,
         // old,hid,mode);
  // DL_DumpObjTab ("st-hili_MAN");


  //----------------------------------------------------------------
  if(mode != TYP_FuncMod) goto L_init;

  // get last dli
  dli = GR_TAB_IND - 1L;
    // printf(" hili_MAN-dli = %ld APT_hidd = %d\n",dli,APT_hidd);
  GR_dli_hili = dli;

/*
  // set last hilited to normal or dimmed if hidden
  if(old >= 0) {
    // reset to normal
    GR_ObjTab[old].hili  = OFF;   // OFF=1
    GR_ObjTab[old].disp  = ON;    // ON=0
    // if hidden: set dimmed
    if(hid == 1) GR_ObjTab[old].dim = ON;
  }

  // if(old == dli) {
  if(APT_hidd < 0) {
    // active obj is comment or unvisble ...
    old = -1L;
    goto L_return1;
  }

  // ignore ApplicationObj's
  if(GR_ObjTab[dli].unvis) {
    old = -1L;
    goto L_return1;
  }

  // set to hilite, not dimmed
  GR_ObjTab[dli].hili  = ON;   // ON=0
  GR_ObjTab[dli].disp  = OFF;  // OFF=1
  GR_ObjTab[dli].dim   = OFF;  // OFF=1

  // keep dli and hidden-state of active-obj
  old = dli;
  hid = APT_hidd;
*/
    // DL_DumpObj__ (ind);
    // printf("ex-DL_hili_MAN\n");

  goto L_return1;


  //----------------------------------------------------------------
  L_init:
  if(mode != TYP_FuncInit) goto L_exit;
  // set all hidden -> dimmed
  for(dli=0; dli<GR_TAB_IND; ++dli) {
    if(DL_IS_HIDDEN(GR_ObjTab[dli])) {
      GR_ObjTab[dli].hili  = OFF;   // OFF=1
      GR_ObjTab[dli].disp  = ON;    // ON=0
      GR_ObjTab[dli].dim = ON;
    }
  }
  goto L_return0;



  //----------------------------------------------------------------
  L_exit:
  if(mode != TYP_FuncExit) goto L_return0;
  //  set all dimmed -> hidden
  // TX_Print ("********** TODO DL_hili_MAN - TYP_FuncExit");
  printf ("********** TODO DL_hili_MAN - TYP_FuncExit\n");


  //----------------------------------------------------------------
  L_return0:
  DL_Redraw ();

  L_return1:
    // printf("ex-hili_MAN old = %ld hidd = %d\n",old,hid);
    // DL_DumpObjTab ("ex-hili_MAN");
  return 0;

}

 
//=====================================================================
  int DL_hili_on (long ind) {
//=====================================================================
/// \code
/// DL_hili_on              set obj hilited
/// - does not hilite hidden objects (VWR, MAN)
/// - does not redraw !
/// Input:
///   ind >= 0: Hili Obj.
///         -1: clear all hilite-flags
///         -2: hilite last disp-List-obj
/// RetCod:
///   -1      hidden object; not hilited ..
///
/// \endcode


  long dli;


  // printf("DL_hili_on %ld of %ld DL_hidden=%ld\n",ind,GR_TAB_IND,DL_hidden);
  // printf(" hili=%d disp=%d\n",GR_ObjTab[ind].hili,GR_ObjTab[ind].disp);
  // printf(" typ=%d\n",GR_ObjTab[ind].typ);
  // if(ind >= 0) DL_DumpObj__ (ind, "DL_hili_on");


  //----------------------------------------------------------------
  if(ind >= 0) {         // Hili Obj.
    if(ind >= GR_TAB_IND) ind = GR_TAB_IND-1;

    // ignore ApplicationObj's
    // if(GR_ObjTab[ind].typ == Typ_APPOBJ) return 0;      //2011-02-16
    if(GR_ObjTab[ind].unvis) return 0;

    // do not hilite if hidden !   2010-10-10
    // if((GR_ObjTab[ind].hili == 1)&&(GR_ObjTab[ind].disp == 1)) return -1;
    if(DL_IS_HIDDEN(GR_ObjTab[ind])) return -1;

    GR_ObjTab[ind].hili  = ON;   // ON=0
    GR_ObjTab[ind].disp  = OFF;  // OFF=1

      // DL_DumpObj__ (ind, "ex-DL_hili_on");
      // printf("ex-DL_hili_on\n");


    return 0;
  }



  //----------------------------------------------------------------
  if(ind == -1) {         // Tabelle löschen
    DL_hili_off (ind);
    return 0;
  }



  //----------------------------------------------------------------
  if(ind == -2) {         // das zuletzt gearbeitete Obj hiliten
    if(GR_TAB_IND > 0) {
      dli = GR_TAB_IND - 1;

      // test for VC
      if(GR_ObjTab[dli].typ == Typ_VC) {
        UI_prev_vc (GR_TMP_I0, Typ_Index, PTR_LONG(GR_ObjTab[dli].ind),
                      NULL, ATT_COL_RED);  //Typ_Att_hili1);

      } else {
        // if dl[dli] == hidden (1,1): set DL_hidden = dli; 
        if(DL_IS_HIDDEN(GR_ObjTab[dli])) DL_hidden = dli;

        // set obj[dli] = hilite (1,0);
        GR_ObjTab[dli].hili  = ON;    // hi=0; di=1;
        GR_ObjTab[dli].disp  = OFF;
          // printf("  _HiliObj dli=%d\n",dli);
      }
    }
    return 0;
  }


  //----------------------------------------------------------------
  printf("DL_hili_on Err %ld\n",ind);

  return -2;

}


//=====================================================================
  int DL_hili_off (long ind) {
//=====================================================================
// \code
// DL_hili_off             reset hilited
// Input:
//   ind >= 0: unhilite single obj
//         -1: unhilite all hilited objects; return nr of hilited objs
//         -2: unhilite last obj;
//         -3: reset hidden-state of active-obj (last obj in DL) after hilite
// Output:
//   retCod -
//
// No Redraw !
// see also GL_temp_del_1 GL_temp_del_all
// \endcode


  int   ii;
  long  l1;


  // printf("DL_hili_off %ld of %ld DL_hidden=%ld\n",ind,GR_TAB_IND,DL_hidden);
  // if(ind>=0)printf("hi=%d di=%d\n",GR_ObjTab[ind].hili,GR_ObjTab[ind].disp);

  if(ind > GR_TAB_IND) {
    printf("***** DL_hili_off E1\n");
    return -1;
  }


  //----------------------------------------------------------------
  if(ind >= 0) {         // UnHili Obj.

    // skip hidden obj's; 2015-09-26
    if(DL_IS_HIDDEN(GR_ObjTab[ind])) return 0;

    // set normal (0,1)
    GR_ObjTab[ind].disp  = ON;    // ON=0
    GR_ObjTab[ind].hili  = OFF;   // OFF=1

    return 0;
  }


  //----------------------------------------------------------------
  if(ind == -1) {
    // reset all hilited to normal
    ii = 0;
    for(l1=0; l1<GR_TAB_IND; ++l1) {
      //TX_Print("reset hili=%d disp=%d",GR_ObjTab[l1].hili,GR_ObjTab[l1].disp);

      // skip hidden; 2015-09-26
      // if(DL_IS_HIDDEN(GR_ObjTab[ind])) continue;

      // change hilite (1,0)  --> normal (0,1)
      if(GR_ObjTab[l1].hili == ON) {

        // hier sollte man nachsehen ob Layer ueberhaupt aktiv ist usw !!
        GR_ObjTab[l1].disp  = ON;
        GR_ObjTab[l1].hili  = OFF;
        ++ii;
      }
    }

    // reset hidden-state (hidden=1,1) after hilite (hili=1,0)
    if(DL_hidden >= 0) DL_hili_off (-3);   // recurs

    return ii;
  }


  //----------------------------------------------------------------
  if(ind == -2) {         // das zuletzt gearbeitete Obj unhiliten

    // reset hidden-state (hidden=1,1) after hilite (hili=1,0)
    DL_hili_off (-3);

      // printf("DL_hili_off  -2 %ld\n",l1);
    DL_hili_off (GR_TAB_IND - 1);
/*
    l1 = GR_TAB_IND-1;
    if(l1 >= 0) {
      GR_ObjTab[l1].hili  = OFF;
      GR_ObjTab[l1].disp  = ON;
    }
*/
    return 0;
  }



  //----------------------------------------------------------------
  if(ind == -3) {  // reset hidden-state of active-obj (last obj in DL) after hilite
    // reset hidden-state (hidden=1,1) after hilite (hili=1,0)
    if(DL_hidden >= 0) {
      // set dl[DL_hidden] = hidden (1,1),
      GR_ObjTab[DL_hidden].hili  = OFF;
      GR_ObjTab[DL_hidden].disp  = OFF;
      DL_hidden = -1L;
    }
    return 0;
  }




  //----------------------------------------------------------------
  printf("**** DL_hili_off Err %ld\n",ind);

  return -1;

}


//=====================================================================
  int DL_dim_all () {
//=====================================================================
// DL_dim_all                  set all objs dimmed
//   dli      -1 - dim all objs

  int    i1;

  // printf("DL_dim_all\n");

  for(i1=0; i1<GR_TAB_IND; ++i1)
    GR_ObjTab[i1].dim = ON;

  return 0;

}


//=====================================================================
  int DL_dim_on (long dli) {
//=====================================================================
// DL_dim_on                   set obj dimmed
//   dli      -1 - dim all objs

  // printf("DL_dim_on %ld\n",dli);

  GR_ObjTab[dli].dim = ON;

  return 0;

}


//=====================================================================
  int DL_dim_off (long dli) {
//=====================================================================
// DL_dim_off                  reset obj dimmed -> normal

  // printf("DL_dim_off %ld\n",dli);

  GR_ObjTab[dli].dim = OFF;

  return 0;

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
/// add / remove (change) Groupbit 1 of DL-Record inda nd add to GrpTab
/// Input:
///   ind       DispListIndex of obj
///   cbuf1     Infotext; NULL for silent
///   mode      0=switch; 1=add to group; -1=remove from group; 2=add all objs
///   iUpd      0=update display; 1=do not update display (yet)
//
//  mode=1: do not check if hidden or in subModel
//  mode=2: add all if not hidden or in subModel
/// \endcode


  long    l1;
  unsigned short modnr;


  printf("DL_grp1__ dli=%ld mode=%d iUpd=%d\n",ind,mode,iUpd);

  if(ind < 0) return -1;


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
    if(cbuf1) TX_Print ("add obj %s to group",cbuf1);
    Grp_add__ (GR_ObjTab[ind].typ, GR_ObjTab[ind].ind, ind, iUpd);
    GR_ObjTab[ind].grp_1 = ON;


  //----------------------------------------------------------------
  } else if(mode == 2) {          // add all to group; eg from Ctrl-A
    // loop tru DL
    Grp_Clear (0);
    // if(cbuf1)
    TX_Print ("add all visible objs to group");
    modnr = AP_modact_ibm;
    for(l1=0; l1<GR_TAB_IND; ++l1) {
      // skip hidden
      // if((GR_ObjTab[l1].disp==1)&&(GR_ObjTab[l1].hili == 1)) continue;
      if(DL_IS_HIDDEN(GR_ObjTab[l1])) continue;
      // skip subModels
      // if(GR_ObjTab[l1].modInd != modnr) continue;
      if(DL_OBJ_IS_ACTMDL(GR_ObjTab[l1])) continue;
      // skip deleted obj
      if(GR_ObjTab[l1].typ    == 0) continue;
      // set groupBit in DL
      GR_ObjTab[l1].grp_1 = ON;
      Grp_add__ (GR_ObjTab[l1].typ, GR_ObjTab[l1].ind, l1, 2);
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
/// set if obj is part of group-1; ON od OFF.
/// do not update display, nr-of-groupObjs
/// Input:
///   ind     -1    all objects
///           >=0   modify single obj; ind = dli
///   mode    ON    = 0; set group = ON
///           OFF   = 1; set group = OFF
///           -1    change group (ON -> OFF | OFF -> ON)
///
/// grp_1  ON=0=belongs to active Group, 1=not
/// \endcode

  long    i1;


  // printf("DL_grp1_set ind=%ld mode=%d %d\n",ind,mode,GR_ATT_TAB_SIZ);


  //----------------------------------------------------------------
  // (mode >= 0): set ON | OFF
  if(mode >= 0) {

    if(ind >= 0) {
        GR_ObjTab[ind].grp_1 = mode;

    } else {
      // all DL-objs  see Grp_dlAdd ??
      for(i1=0; i1<GR_TAB_IND; ++i1) {
        GR_ObjTab[i1].grp_1 = mode;
        Grp_add__  (GR_ObjTab[i1].typ, GR_ObjTab[i1].ind, i1, 0);
      }
    }


  //----------------------------------------------------------------
  // (mode = -1): change groupBit
  } else {
  
    if(ind >= 0) {
      // single DL-rec: change group (ON -> OFF | OFF -> ON)
      if(GR_ObjTab[ind].grp_1) {
        GR_ObjTab[ind].grp_1 = ON;
        Grp_add__  (GR_ObjTab[ind].typ, GR_ObjTab[ind].ind, ind, 0);
        
      } else {
        GR_ObjTab[ind].grp_1 = OFF;
        Grp_del (GR_ObjTab[ind].typ, GR_ObjTab[ind].ind, 0);
      }

    } else {
      // all DL-objs: change group (ON -> OFF | OFF -> ON)
      Grp_init ();
      for(i1=0; i1<GR_TAB_IND; ++i1) {
        // skip obj if not in active submodel
        if(DL_OBJ_IS_ACTMDL(GR_ObjTab[i1])) continue;
        GR_ObjTab[i1].grp_1 = ICHG01 (GR_ObjTab[i1].grp_1);
        if(!GR_ObjTab[i1].grp_1)
          Grp_add__  (GR_ObjTab[i1].typ, GR_ObjTab[i1].ind, i1, 0);
      }
    }
  }

  // display nr of objs in group
  // Grp_init();

  return 0;
}


//================================================================
  int DL_grp1_get (long dli) {
//================================================================
// DL_grp1_get             get grp_1-bit


  return GR_ObjTab[dli].grp_1;

}

  
//================================================================
  int DL_grp1_nr_get () {
//================================================================
/// DL_grp1_nr_get       count nr of objs in group

  int   i1, ii = 0;


  for(i1=0; i1<GR_TAB_IND; ++i1)
    if(!GR_ObjTab[i1].grp_1) ++ii;

  return ii;

}


//================================================================
  int DL_grp1_copy () {
//================================================================
/// copy all DL-obj with groupBit ON --> GroupList GrpTab

  int     typ;
  long    i1;

  printf("GGGGGGG DL_grp1_copy GGGGGGGGG \n");


  Grp_init (); // init obj-list

  for(i1=0; i1<GR_TAB_IND; ++i1) {
    if(GR_ObjTab[i1].grp_1 == OFF) continue;

    typ = AP_typDB_typ (GR_ObjTab[i1].typ);
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
/// Ind = 0      Init or Reset DL
/// Ind = 1      increase DL (add GR_TAB_INC elements)
/// Ind > 1      set size
/// \endcode

  long   i1, newSiz;


  // printf("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD \n");
  // printf("... DL_alloc__ %ld ind=%ld siz=%ld %p\n",Ind,
         // GR_TAB_IND,GR_TAB_SIZ,GR_ObjTab);


  // Reset DL
  if((Ind == 0)&&(GR_ObjTab != NULL)) {
    // printf("DL_alloc__ reset\n");
    // GL_Init1 ();    // GR_TAB_IND = 0;
    GR_TAB_IND  = 0;
    return 0;
  }


  newSiz = GR_TAB_SIZ + GR_TAB_INC;

  if(Ind > 1) {
    if(Ind < GR_TAB_SIZ) return 0;
    while (Ind >= newSiz) newSiz += GR_TAB_INC;
  }

  i1 = newSiz * sizeof(DL_Att);
  printf("::::DL_alloc__ %ld %ld\n",newSiz,i1);

  GR_ObjTab = (DL_Att*)realloc((void*)GR_ObjTab, i1);

  if(GR_ObjTab == NULL) {
    TX_Error ("******** out of memory - DL_alloc__ *********");
    return -1;
  }
  GR_TAB_SIZ = newSiz;


    // printf(" ex-DL_alloc__\n");

  return 0;

}


//============================================================
   void DL_Init () {
//============================================================

  int    i1;


  // printf("LLLLLLLLLLLLLL DL_Init LLLLLLLLLLLLLLLL\n");

  // GL_InitCol (10);


  // Nicht im "store-tesselated-Vertices"-Modus
  if(TSU_get_mode() != 0) return;


  DL_disp_act = ON;

  DL_alloc__ (0L);

  DL_InitAttTab ();

  for(i1=0; i1<DL_base_font1; ++i1) DL_tempLst[i1] = 0;

  //  clear DispList
  // GR_Init1 ();


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
/// defaultvalues from ~/gCAD3D/cfg_Linux/ltyp.rc
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
/// defaultvalues from ~/gCAD3D/cfg_Linux/ltyp.rc
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

// TODO: change Att_ln -> Ind_Att_ln



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
/// Create line-attributes. File is  ~/gCAD3D/cfg_Linux/ltyp.rc
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
///   col    color   900=red, 090=green, 119=blue, 990=yellow
///   ltyp   linetyp LTYP_.. (../ut/func_types.h)
///            0=full, 1=dash-dot, 2=shortDash, 3=longDash
///   lthick 1-6, thickness in pixels
///
/// 
/// defaultvalues from <tempDir>/cfg_Linux/ltyp.rc (../../gCAD3D/cfg_Linux/ltyp.rc)   
/// \endcode

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

  GLB_DrawInit ();
  GL_tmp_Redraw ();
  GLB_DrawExit ();

  return 0;

}
*/

 
//================================================================
  int DL_Draw_obj (long dli, int typ, long dbi) {
//================================================================
// redraw single obj, use existing DL-record


  // modify (do not create new DL-Record); set DL_perm_ind=dli;
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

  // UI_GR_focus ();       // set focus to glarea widget 2011-01-28
// 
  // nimmt den focus von den CAD-Eingabefeldern weg ...
  // UI_block_glEvents (1);
  GLB_DrawInit ();     // GLB_DrawInit ();
  GL_Redraw ();
  GLB_DrawExit ();     // GLB_DrawExit ();
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
  long DL_perm_init (int typ, long dbi, int att) {
//================================================================
// DL_perm_init            create DL-record for permanent storage
// + glNewList();
//
// see DL_tDyn_init DL_temp_init


  long   dli, gli;

  // printf("DL_perm_init typ=%d dbi=%ld, att=%d\n",typ,dbi,att);


  //----------------------------------------------------------------
  // get next free index  (DL_perm_ind must be -1)
  dli = -1L;
  gli = DL_dli_get (&dli);

  // create DL-record
  AP_dli_act = DL_set__ (typ, dbi, dli, att);

  // start GL-list
  GL_list_open (gli);

    // printf("ex-DL_perm_init %ld\n",gli);

  return gli;

}


//================================================================
  long DL_tDyn_init (int att) {
//================================================================
// DL_tDyn_init                create DL-record for temporary-dynamic storage
// + glNewList();
//
// see DL_temp_init


  long   dli, gli;


  // printf("DL_tDyn_init \n");

  //----------------------------------------------------------------
  // get next free index  (DL_perm_ind must be -1)
  dli = -1L;
  gli = DL_dli_get (&dli);

  // create DL-record
  AP_dli_act = DL_set__ (Typ_dynSym, dli, dli, att);

  // start GL-list
  GL_list_open (gli);

    // printf("ex-DL_tDyn_init dli=%ld gli=%ld\n",dli,gli);

  return gli;

}


//================================================================
  void DL_att_mdr (long dli, int att) {
//================================================================
// DL_att_mdr            set perm/-refModels to hilite or dim
//   att     GR_TMP_DEF|GR_TMP_HILI|GR_TMP_DIM;


  // printf("DL_att_mdr dli=%ld att=%d \n",dli,att);


  if(att == GR_TMP_HILI) {
    DL_hili_on (dli);


  } else if(att == GR_TMP_DIM) {
    DL_dim_on (dli);


  } else {
    DL_hili_off (dli);
    DL_dim_off (dli);
  }

}


//================================================================
  void DL_att_temp (long gli, int mode) {
//================================================================
// DL_att_temp            set temporary-refModels to hilite or dim
//   mode    GR_TMP_DEF|GR_TMP_HILI|GR_TMP_DIM;
//           use for displaying models (GL_set_mdr - temp)

  printf("DL_att_temp gli=%ld mode=%d \n",gli,mode);

  DL_tempLst[gli] = mode;

}


//================================================================
  long DL_temp_init () {
//================================================================
// DL_temp_init                get GL-index for temporary storage; glNewList();
// Input:
//   ind      fixed index: 1 - (DL_base_font1 - 1)
//            0    get next free index
//
// replacing DL_fix_DL_ind for temporary-objects


  long   gli;

  // printf("DL_temp_init set=%ld nxtFree=%ld\n",DL_temp_ind,DL_temp_nxt);
  // printf("  APT_obj_stat=%d\n",WC_get_obj_stat());
  // if(DL_temp_ind==8) AP_debug__ ("DL_temp_init-1");


  //----------------------------------------------------------------
  if(DL_temp_ind) {
    // fixed index
    gli = DL_temp_ind;
    DL_temp_ind = 0;
    if(DL_temp_nxt <= gli) DL_temp_nxt = gli + 1;

  } else  {
    // get next free index
    gli = DL_temp_nxt;
    // printf(" tmp.ind=%d\n",DL_ind);
    ++DL_temp_nxt;
  }

  if(gli >= DL_base_font1) {
    TX_Print("********* DL_temp_init E2 - TempList overflow ************");
    gli = DL_base_font1 - 1;
    DL_temp_nxt = gli;
  }

  // set att = 1 = normal
  DL_tempLst[gli] = 1;

  // start GL-list
  GL_list_open (gli);

  L_exit:
    // printf("ex-DL_temp_init %ld\n",gli);
  return gli;

}


//================================================================
  void DL_temp_ind_set (int ti) {
//================================================================
// DL_temp_ind_set         set DL_temp_ind
//   set fixed GL-index for following obj / overwrite obj with this index.

  if(ti < DL_base_font1) DL_temp_ind = ti;

}


//================================================================
  long DL_dli_get (long *dli) {
//================================================================
// DL_dli_get             get DispListIndex for permanent obj
//   returns the GL-index 
// Input:
//   DL_perm_ind >= 0   fixed DispListIndex
//              -1     next free DispListIndex 
// Output:
//   dli        DL-index
//   retCode    the GL-index (DL_base__ added)
//              -1 Error
//
// DL_SetInd() sets DL_perm_ind = dli for overwrite-existing-objects
//
// was GL_fix_DL_ind

  long   gli;

  // printf("DL_dli_get\n");


  //----------------------------------------------------------------
  // overwrite existing dispListIndex (was set with DL_SetInd())
  if(DL_perm_ind >= 0L) {
      // printf(" XXXXXXX overwrite; DL_perm_ind=%d \n",DL_perm_ind);
    if(DL_perm_ind >= GR_TAB_SIZ) {TX_Error("DL_dli_get E1"); return -1L;}
    *dli = DL_perm_ind;
    DL_perm_ind  = -1L;    // preset default = create new DL-Record.
    goto L_exit;
  }


  //----------------------------------------------------------------
  // get next free dispListIndex
  // set DL-index is next free record
  *dli = GR_TAB_IND;
  ++GR_TAB_IND;
      // printf(" dlInd=%ld lNr=%d IND=%ld\n",dlInd,lNr,GR_TAB_IND);

  if(GR_TAB_IND >= GR_TAB_SIZ) {
    if(DL_alloc__ (1L) < 0) return -1L;
  }


  //----------------------------------------------------------------
  L_exit:

    // printf("ex-DL_dli_get %ld\n",*dli);

  return (*dli + DL_base__);
  // return (*dli);

}


//================================================================
  long DL_set__ (int typ, long dbi, long dli, int atti) {
//================================================================
// DL_set__                                   store DL-record
// Input:
//   GLOBAL: WC_sur_ind AP_modact_ibm DL_disp_act GR_pick
//
// replacing DL_StoreObj()

  int     lNr;
  DL_Att  dlRec;


  // printf("DL_set__ typ=%d dbi=%ld dli=%ld\n",typ,dbi,dli);


  //----------------------------------------------------------------
  // fix lNr
  if(AP_mdLev >= 0) {
    AP_mdGet (&lNr);

  } else if(UP_level >= 0) {
    lNr = APT_UP_get ();
    // printf(" calling lNr = %d\n",lNr);

  } else {
    lNr = APT_lNr;
  }
    // printf(" _StoreObj lNr=%d AP_mdLev=%d UP_level=%d APT_lNr=%d\n",
           // lNr, AP_mdLev, UP_level, APT_lNr);


    dlRec = DL_Att_NUL;   // OFF=1, ON=0
      // printf(" dim=%d\n",dlRec.dim);



  //----------------------------------------------------------------
    dlRec.lNr    = lNr;           // die momentane APT-LineNr
    dlRec.typ    = typ;           // zB Typ_LN
    dlRec.ind    = dbi;
    dlRec.iatt   = atti;

    dlRec.irs    = WC_sur_ind;

    dlRec.modInd = AP_modact_ibm;     // (signed short)
    // dlrec.unvis  = 0;            // Default = visible
    // dlrec.sChd   = 0;            // Default = indep.
    // dlrec.sPar   = 0;            // Default = indep.

    dlRec.disp   = DL_disp_act;  // ON od OFF
    // dlRec.hili   = OFF;
    // dlRec.dim    = OFF;
    // dlRec.grp_1  = OFF;
    dlRec.pick   = GR_pick;

    // dlrec.lay    = GR_lay_act;
    // dlrec.temp   = itemp;


  if(typ == Typ_apDat) dlRec.unvis = OFF;


  //----------------------------------------------------------------
  GR_ObjTab[dli] = dlRec;

      // DL_DumpObj__ (dli);

  return dli;

}


//================================================================
  int DL_SetInd (long dli) {
//================================================================
/// \code
/// DL_SetInd               modify (overwrite, do not create new DL-Record)
/// Input:
///   dli     -1  default = create new DL-Record. Reset.
///          <-1  temporary obj (without DL-record)
///           >0  DL_StoreObj does not creat new DL-Record  (modify object)
///
/// DL_StoreObj create or overwrite DispList-record and restores to -1
/// Add into already open DL-record: see GR_DrawSur, GR_Draw_spu.
/// \endcode

  // printf("########################## DL_SetInd %ld\n",dli);

  DL_perm_ind = dli;

  return 0;

}
 

//============================================================
  int DL_SetObj (long *dli, int typ, long dbi, int iAtt) {
//============================================================
/// \code
/// DL_SetObj      get or set disp-list-record.
/// Using DL_StoreObj / DL_SetInd.
/// Input:
///   dli     disp-list-record#; initialize with -1L.
///           if (dli < 0) - create new record (DL_StoreObj)
///           if (dli >= 0) - use the existing record (update obj)
///   typ,dbi,iAtt see DL_StoreObj
/// Output:
///   dli     disp-list-record# >= 0
///   retCod  0  DL-record did already exist
///           1  new DL-record created
/// \endcode


  if(*dli < 0) {
    *dli = DL_StoreObj (typ, dbi, iAtt);
    return 1;

  } else {
    // DL_perm_ind = *dli;
    return 0;
  }

}


//============================================================
   long DL_StoreObj (int Typ, long DBInd, int AttInd) {
//============================================================
/// \code
/// create new or overwrite DispList-record; returns its index.
///    (overwrite with DL_SetInd before)
/// DBInd=0: temp.obj; no DL-record
/// DBInd>0: write GR_ObjTab[DL-index]; return DL-index
/// 
///  Input:
///    Typ        type of obj; eg Typ_PT; see INF_OTYP
///    DBInd      0    temp. Obj 8 (returns -8L = GR_TMP_I0; no DL-record).
///               <0   temp. Obj m ind. -GR_TAB_IND
///               >0   GR_ObjTab-index
///    AttInd     attribute (GR_ObjTab[].iatt)
///               for Typ=Typ_apDat:  subType; eg Typ_constrPln
///  Global-input:
//     DL_perm_ind -1 = create new DL-record else overwrite existing DL-record
/// 
///  Output:
///    RetCod     DL-index   (GR_ObjTab)                see INF_DL__
///
/// see also:
///   GL_view_ini__  DL_set__
///   DL_SetInd   preset DL-index (to modify DL-record)
/// \endcode

// TODO: DBInd<0 - DL-record is written, but resolving buggy.
//       DL-record for GR_TMP_IDIR ?
//       results of selection of GR_TMP_I0-obj ? Of CAD-inputObjects -2 to -7 ?
//         -10 to DL_base_font1 (normal temp-obj's) ?

// DL_StoreObj returns dlInd = index into GR_ObjTab for active object
// GR_ObjTab is the DL-record-table
// GR_TAB_IND is the index of the next free DL-record



  long   dlInd;
  int    lNr, itemp;


  // printf("DL_StoreObj typ=%d DBind=%ld AttInd=%d - IND=%ld SIZ=%ld\n",
          // Typ, DBInd,AttInd,GR_TAB_IND,GR_TAB_SIZ);
  // printf("    DL_perm_ind=%ld\n",DL_perm_ind);
  // printf("    DLind=%d SIZ=%d\n",GR_TAB_IND,GR_TAB_SIZ);
  // printf("    WC_sur_ind=%d\n",WC_sur_ind);
  // printf("DL_StoreObj pick = %d\n",GR_pick);
  // if(Typ == Typ_Model) printf(" ***** Typ_Model %ld\n",DBInd);




  if(DBInd == 0) {       // 2011-10-18
    if (Typ != Typ_APPOBJ)          {
      dlInd = GR_TMP_I0;
      goto L_done;
    }
  }



  // dynam. Obj. erhalten die neg. DL-ID als DB-index in ihrem DL-Record.
  if(DBInd < 0) {
    DBInd = -GR_TAB_IND;
  }


  //----------------------------------------------------------------
  // ist DL-Rec bereits vorhanden ? Dann den DL-Index des existing Record
  // liefern; wurde von DL_SetInd() gesetzt !
  // printf(" DL_perm_ind=%d\n",DL_perm_ind);
  if(DL_perm_ind >= 0L) {
    // printf(" XXXXXXX overwrite; DL_perm_ind=%d \n",DL_perm_ind);
    dlInd = DL_perm_ind;
    // GL_Del0 (dlInd);
    goto L_done;
  }



  //----------------------------------------------------------------
  if(DL_perm_ind <= -2L) {
    // temporary obj (without a DL-record) wanted 
    dlInd = DL_perm_ind;
    DL_perm_ind = -1L;    // preset default = create new DL-Record.
      // printf("ex DL_StoreObj %ld IND=%ld\n",dlInd,GR_TAB_IND);
    goto L_exit;
  }


  //----------------------------------------------------------------
  // (DL_perm_ind is -1; create (add) new DL-record
  // realloc, wenn zu klein
  // if(GR_TAB_IND >= GR_TAB_SIZ) {
  if(GR_TAB_IND >= GR_TAB_SIZ - 8) {
    if(DL_alloc__ (1L) < 0) return -1L;
  }

  // set DL-index is next free record
  dlInd = GR_TAB_IND;
  ++GR_TAB_IND;
      // printf(" dlInd=%ld lNr=%d IND=%ld\n",dlInd,lNr,GR_TAB_IND);


  //----------------------------------------------------------------
  // fix lNr
  if(AP_mdLev >= 0) {
    AP_mdGet (&lNr);

  } else if(UP_level >= 0) {
    lNr = APT_UP_get ();
    // printf(" calling lNr = %d\n",lNr);

  } else {
    lNr = APT_lNr;
  }
    // printf(" _StoreObj lNr=%d AP_mdLev=%d UP_level=%d APT_lNr=%d\n",
           // lNr, AP_mdLev, UP_level, APT_lNr);


  //----------------------------------------------------------------
    GR_ObjTab[dlInd].lNr    = lNr;           // die momentane APT-LineNr
    GR_ObjTab[dlInd].typ    = Typ;           // zB Typ_LN
    GR_ObjTab[dlInd].ind    = DBInd;
    GR_ObjTab[dlInd].iatt   = AttInd;

    GR_ObjTab[dlInd].irs    = WC_sur_ind;

    GR_ObjTab[dlInd].modInd = AP_modact_ibm;     // (signed short)
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



  //----------------------------------------------------------------
  L_done:
  DL_perm_ind  = -1L;    // preset default = create new DL-Record.
  GL_modified = 1;      // UNUSED !
  DL_disp_act = 0;     // default = ON          2010-12-20


    // DL_DumpObj__ (dlInd);
    // printf("ex DL_StoreObj %d\n",dlInd);

 
  // Typ_apDat: dieses obj bekommt kein sichtbares GL-obj;
  // daher muss ein bereits in der GL-DispList befindliches Objekt
  // (zB aus einem anderen subModel) geloescht werden !
  if(Typ == Typ_apDat) {
    GL_Del1 (dlInd);
  }


  L_exit:

    // DL_DumpObj__ (dlInd, "ex-DL_StoreObj");
    // printf("ex-DL_StoreObj %ld IND=%ld\n",dlInd,GR_TAB_IND);
    // if(dlInd > GR_TAB_SIZ) AP_debug__ ("DL_StoreObj-E9");

  return dlInd;

}


//================================================================
  int  DL_DumpObj__ (long idl, char *inf) {
//================================================================
// DL_DumpObj__            dump single dispListrecord
//   inf        infotext, can be NULL
// see GA_dump__

  short     i20;
  char      cbuf[256], cAtt[32];
  ColRGB    *col;
  stru_2i2  *i2att;


  if(inf) printf("DL_DumpObj__ %ld    %s\n",idl,inf);


  if((GR_ObjTab[idl].typ == Typ_SUR)  ||
     (GR_ObjTab[idl].typ == Typ_SOL)  ||
     (GR_ObjTab[idl].typ == Typ_Model))   {
    col = COL_INT32(&GR_ObjTab[idl].iatt); // col = (ColRGB*)&GR_ObjTab[idl].iatt;
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
    sprintf(cAtt, "%d",GR_ObjTab[idl].iatt);
  }

  i20 = GR_ObjTab[idl].modInd;

  sprintf(cbuf,
          "%ld typ=%d dbi=%ld att=%s mod=%d\
 rs=%d uv=%d dis=%d hil=%d dim=%d pi=%d chd=%d par=%d grp=%d lNr=%ld",
        idl,
        GR_ObjTab[idl].typ,
        GR_ObjTab[idl].ind,
        cAtt,
        i20,
        GR_ObjTab[idl].irs,
        GR_ObjTab[idl].unvis,
        GR_ObjTab[idl].disp,
        GR_ObjTab[idl].hili,
        GR_ObjTab[idl].dim,
        GR_ObjTab[idl].pick,
        GR_ObjTab[idl].sChd,
        GR_ObjTab[idl].sPar,
        GR_ObjTab[idl].grp_1,
        GR_ObjTab[idl].lNr);

  printf("%s\n",cbuf);

  return 0;

}


//================================================================
  void   DL_DumpObjTab    (char *fInf) {
//================================================================
// DL_DumpObjTab           dump complete DL

  long   l1;
  char   s1[32];

  TX_Print("#### DL_DumpObjTab %ld    %s",GR_TAB_IND,fInf);


  
  printf("#### DL_DumpObjTab %ld AP_modact_ibm=%d AP_modact_nam=|%s|\n",
         GR_TAB_IND, AP_modact_ibm, AP_modact_nam);

  for(l1=0; l1<GR_TAB_IND; ++l1) DL_DumpObj__ (l1, NULL);

  printf(" next free temp-index DL_temp_nxt = %ld\n",GL_temp_iNxt());

  // temp-Liste:
  // for(l1=1; l1<DL_temp_nxt; ++l1) {
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
  long DL_dli__dbo (int typ, long DBind, long DLend) {
//================================================================
/// \code
/// get dispListIndex of (last) DB-obj from typ/dbi
/// Objekt typ=typ APTind=ind in der DL suchen.
/// liefert DisplayListIndex des letzen definierten Objekts
/// see AP_typDB_typ AP_cmp_typ
/// Hidden objects haben -1 !!
/// Input:
///   typ     objTyp (Typ_PT ..)
///   DBind   dataBaseIndex of obj to search
///   DLend   last DL-Index to check;  -1L = search in complete DL
/// retCode = DispListIndex
///      -1   Error; objID not found in DL
/// \endcode

// ACHTUNG: kann keine Vecs & Vars finden !!!


  return DL_find_smObj (typ, DBind, DLend, AP_modact_ibm);

}


//=============================================================
  long DL_find_smObj (int typ, long DBind, long DLend, int imod) {
//=============================================================
/// \code
/// get dispListIndex of (last) DB-obj from typ/dbi/subModelNr
/// Objekt typ=typ APTind=ind in der DL suchen.
/// liefert DisplayListIndex des letzen definierten Objekts
/// see AP_typDB_typ AP_cmp_typ
/// Hidden objects haben -1 !!
/// Input:
///   typ     objTyp (Typ_PT ..)
///   DBind   dataBaseIndex of obj to search
///   DLend   last DL-Index to check;  -1L = search in complete DL
///   imod    AP_modact_ibm 
/// retCode = DispListIndex
///      -1   Error; objID not found in DL
/// \endcode

// ACHTUNG: kann keine Vecs & Vars finden !!!


  int   typAct;
  unsigned short modnr;
  long  l1, DLind;


  typ = AP_typDB_typ (typ);

  // ACHTUNG: AP_modact_ibm in ein I2 kopieren, da in der DL als I2 gespeichert;
  // -1 == Main wird sonst nicht gefunden !!!
  modnr = imod;


  // printf("DL_find_smObj typ=%d dbi=%ld dle=%ld modnr=%d\n",typ,DBind,DLend,modnr);
  // DL_DumpObjTab ();



  // for(l1=0; l1<GR_TAB_IND; ++l1)i
  // von vorn weg suchen; geht nicht - findet parent statt child ..

  if(DLend < 0) DLend = GR_TAB_IND - 1;
    // printf(" DLend=%ld GR_TAB_IND=%ld\n",DLend,GR_TAB_IND);


  for(l1=DLend; l1>=0; --l1) {
      // printf("DL[%ld] typ=%d dbi=%ld\n",l1,DL_dbTyp__dli(l1),DL_get_dbi(l1));

    if(GR_ObjTab[l1].ind    != DBind) continue;
    // skip SubModels
    // if(GR_ObjTab[l1].modInd != modnr) continue;
    if(DL_OBJ_IS_ACTMDL(GR_ObjTab[l1])) continue;
    if(GR_ObjTab[l1].typ    == 0) continue;
    // skip deleted obj

    
    typAct = GR_ObjTab[l1].typ;
      // printf(" dl[%ld] typ=%d\n",l1,typAct);


    if(typAct == typ) {
      DLind = l1;
      goto L_found;
    }

    typAct = AP_typDB_typ (typAct);
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
         (typAct == Typ_CVTRM))    {
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
    // printf("ex-DL_find_smObj dli=%ld\n",DLind);

  return DLind;

}


/* UNUSED ..
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

      // printf(" _find_sel %d typ=%d ind=%d\n",
                // l1,GR_ObjTab[l1].typ,GR_ObjTab[l1].ind);
    *typ = GR_ObjTab[l1].typ;
    *ind = GR_ObjTab[l1].ind;
    return 0;
  }

  return -1;

}
*/

//============================================================
  int DL_dbTyp__dli (long objInd) {
//============================================================
///  liefert den Typ eines DB-Obj (als return-Code)

 // Input: objInd; alle andern Out.


  int Typ;
  
  if(objInd < 0) return Typ_Error;
  if(objInd >= GR_TAB_IND) return Typ_Error;

  Typ = GR_ObjTab[objInd].typ;

  // printf ("DL_dbTyp__dli typ=%d\n",Typ);

  return Typ;

}


//============================================================
  long DL_get_dbi (long dli) {
//============================================================
/// \code
///  liefert den DB-index eines DB-Obj (als return-Code)
/// 
///  Input: dli; alle andern Out.
/// \endcode


  if(dli < 0) return dli;
  if(dli >= GR_TAB_IND) return 0L;   //-1L;


  // printf ("ex DL_get_dbi dli=%ld dbi=%ld\n",ind,GR_ObjTab[dli].ind);

  return GR_ObjTab[dli].ind;

}


//================================================================
  int DL_dli__oid (long *dli, long *lNr, char *oid) {
//================================================================
// DL_dli__oid           get DispListIndex and source-line-nr from obj-ID


  int    typ;
  long   dbi;


  // get dbo (typ,dbi)
  APED_dbo_oid (&typ, &dbi, oid);

  // get dli & lNr from dbo
  APED_find_dbo (dli, lNr, typ, dbi);


    // printf("ex DL_oid__dli |%s| %d\n",oid,dli);


  return 0;

}


//================================================================
  int DL_oid__dli (char *oid, long dli) {
//================================================================
/// DL_oid__dli        get objName from DispListIndex


  // make name from typ and DB-Index
  APED_oid_dbo__ (oid, GR_ObjTab[dli].typ, GR_ObjTab[dli].ind);

    // printf("ex DL_oid__dli |%s| %d\n",oid,dli);


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
  // printf(" AP_modact_ibm=%d\n",AP_modact_ibm);
  // DL_DumpObjTab ();

  if(GR_TAB_IND < 1) { *dli = 0; return -1; }

  // get dli from sourceLineNr
  for(l1 = GR_TAB_IND - 1; l1 >= 0; --l1) {
      // printf(" l1=%d\n",l1);

    if((INT_16)GR_ObjTab[l1].modInd != AP_modact_ibm) continue;
    
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

  // TX_Print ("DL_get_dbi typ=%d\n",ind);

  return GR_ObjTab[dli].irs;

} 


//=============================================================
  int DL_GetPick (long objInd) {
//=============================================================

  return GR_ObjTab[objInd].pick;

}


/*
//=============================================================
  DL_Att DL_GetAtt (long objInd) {
//=============================================================

  if(objInd < 0)           { TX_Error("DL_GetAtt E001"); return GR_ObjTab[0]; }
  if(objInd >= GR_TAB_IND) { TX_Error("DL_GetAtt E002"); return GR_ObjTab[0]; }

  return GR_ObjTab[objInd];

}
*/


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
  int DL_set_iatt (long dli, unsigned int iatt) {
//================================================================
// DL_set_iatt             modify iatt of DL-record

  GR_ObjTab[dli].iatt = iatt;

  return 0;

}


//================================================================
  ColRGB* DL_get_col (long dli) {
//================================================================
/// DL_get_col           get ColRGB* of DL-record (surf)

  return COL_INT32(&GR_ObjTab[dli].iatt); // col = (ColRGB*)&GR_ObjTab[dli].iatt;

}


//================================================================
   int DL_get_irs () {
//================================================================
// DL_get_irs              get active ConstrPlane (.irc of last obj)

  DL_DumpObjTab ("DL_get_irs"); //           dump complete DL

  return GR_ObjTab[GR_TAB_IND - 1].irs;

}


//================================================================
  unsigned int DL_get_iatt (long dli) {
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


  col = COL_INT32(&GR_ObjTab[dli].iatt); // col = (ColRGB*)&GR_ObjTab[dli].iatt;

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
      i2 = GR_AttLnTab[GR_ObjTab[l1].iatt].dash;
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


/*
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
    // UT3D_pt_tra_pt_m3 (&ptTr, APT_2d_to_3d_Mat, pt1);
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
  
  // DEB_dump_ox_0 (oxi, "DL_ReScaleObj");


   // DL_ReScalePoint(&pt1);

  return 0;

}
*/

//================================================================
  int DL_scale_grp () {
//================================================================
// DL_scale_grp                           scale group

  // printf("DL_scale_grp \n");

  // get box for grp only
  UT3D_box_mdl__ (&AP_box_pm1, &AP_box_pm2, -1, 1);

  // view box
  DL_ReScale_box (&AP_box_pm1, &AP_box_pm2);

  // set invalid (must recompute for All)
  AP_mdlbox_invalid_set ();

  return 0;

}

 
//====================================================================
  int DL_ReScale__ () {
//====================================================================
// DL_ReScale__                 rescale and redraw active model


  Point    *bp1, *bp2;
  ModelBas *mb1;

  printf("DL_ReScale__ AP_modact_ibm=%d\n",AP_modact_ibm);




  if(AP_modact_ibm < 0) {
    // get box for main
    bp1 = &AP_box_pm1;
    bp2 = &AP_box_pm2;

  } else {
    // get box for subModel
    mb1 = DB_get_ModBas (AP_modact_ibm);
    bp1 = &mb1->pb1;
    bp2 = &mb1->pb2;
  }


  // get boxPoints
  UT3D_box_mdl__ (bp1, bp2, AP_modact_ibm, 0);


  // view box 
  DL_ReScale_box (bp1, bp2);




//   // test if AP_box_pm1 valid (AP_stat.mdl_box_valid)
//   if(AP_mdlbox_invalid_ck()) {
//       printf(" mdl_box not valid ..\n");
//     // get box of active model
//     UT3D_box_mdl__ (&AP_box_pm1, &AP_box_pm2, -1, 0);
//     // set modelbox valid ..
//     AP_mdlbox_invalid_reset ();
//     // get boxpoints for active modelsize (APT_ModSiz)
//     BBX_def__ (&pb1, &pb2);
//     // rescale and redraw
//     DL_ReScale_box (&pb1, &pb2);
//   } else {
//     // rescale and redraw
//     DL_ReScale_box (&AP_box_pm1, &AP_box_pm2);
//   }


  return 0;

}


//================================================================
  int  DL_ReScale_box (Point *pb1, Point *pb2) {
//================================================================
// DL_ReScale_box               view box pb1,pb2
// was DL_ReScale1

  double  sMin, dx, dy, dz, xSizU, scl;
  Point   pOri, pt1;


  printf("DL_ReScale_box \n");
  DEB_dump_obj__ (Typ_PT, pb1, "pb1");
  DEB_dump_obj__ (Typ_PT, pb2, "pb2");
    
    
  // get origin from box of tess-model; but only if very far outside ..
  // tess_origin_box (&pOri, pb1, pb2); 
    // DEB_dump_obj__ (Typ_PT, &pOri, "pOri");
    
    
  //----------------------------------------------------------------
  // if boxPoints empty then set box with modelsize around pb1
  // if(DB_isFree_PT (pb2)) {
  if(UT3D_comp2pt(pb1, pb2, UT_TOL_pt)) {    // UT_TOL_min0)) {
    pOri = *pb1;
    xSizU = APT_ModSiz / 2.;
    goto L_do;
  }


  //----------------------------------------------------------------
  // prepare for reScale; get pOri = midPoint of box
  UT3D_pt_mid2pt (&pOri, pb1, pb2);

  dx = pb2->x - pb1->x;
  dy = pb2->y - pb1->y;
  dz = pb2->z - pb1->z;


  // find max dist
  xSizU = UTP_max_d3 (&dx, &dy, &dz);
    // printf(" d1=%lf\n",d1);

  //----------------------------------------------------------------
  // set view, redraw
  L_do:

  // check minScal, maxScal
  // if    (Usiz < 0.000000001) Usiz = 0.000000001;
  sMin = APT_ModSiz / 2000.;  // UT_DISP_cv;    2017-04-10
  if    (xSizU < sMin) xSizU = sMin;
  else if(xSizU > 100000000.) xSizU =  100000000.;


  scl = APT_ModSiz / xSizU * 1.5;     // 1.25: fix into win ..


  GL_Rescale (scl, &pOri);

  return 0;

}


/* Version with glFeedbackBuffer
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
  // d1 =  1000000. / APT_ModSiz;    // bis 2005-10-27
  // Problem mit sehr kleinen Objekten; zB WRL/Assy6.wrl
  // d1 = APT_ModSiz / 0.001;
  // Sehr gr.Objekt ist IGS/Rahmen2; IGS/6001_3.igs
  // d1 = APT_ModSiz;  // 2011-01-09
  // startvalue must be fixed ! 2011-05-21   
  // 2011-06-27  ..Err3 geht ned mit d1=1000; erst mit 25000.
  // d1 = 25000.; 
  d1 = APT_ModSiz * 50;  // 2016-05-23
  ii = 0;


  L_retry:
  ++ii;
    printf("L_retry d1=%f\n",d1);
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
    // DEB_dump_obj__ (Typ_PT, &pb1, "-pb1:");
    // DEB_dump_obj__ (Typ_PT, &pb2, "-pb2:");

  // printf("DL_ReScale__ L2\n");


  // zusatzl die Notes beruecksichtigen ..
  i2 = DL_ReScale_Notes (&pb1, &pb2);
    // printf(" d1=%f scalN-i2=%d\n",d1,i2);
    // DEB_dump_obj__ (Typ_PT, &pb1, " pb1:");
    // DEB_dump_obj__ (Typ_PT, &pb2, " pb2:");


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
    // DEB_dump_obj__ (Typ_PT, &pm, " center pm=");
    // printf(" new scale = %f\n",d1);

  GL_vcSel_init (ivs, 0);      // reset VectorSelector

  // !! Hier wird die Z-Plane fuer den Viewer gesetzt !!
  GL_Rescale (d1, &pm);        // macht ein GL_Redraw!


  L_exit:
    // printf("ex DL_ReScale__ ModSiz=%f\n", APT_ModSiz);

  return;

}
*/

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


  printf("DL_ReScale_Notes \n");

      // DEB_dump_obj__(Typ_PT, pb1, "  pb1i=");
      // DEB_dump_obj__(Typ_PT, pb2, "  pb2i=");

  pNr = -1;

  // loop tru all visible notes ..
  for(l1=0; l1<GR_TAB_IND; ++l1) {

    if(GR_ObjTab[l1].typ  != Typ_Tag) continue;

    if(DL_IS_HIDDEN(GR_ObjTab[l1])) continue;        // skip hidden obj's
    // if((GR_ObjTab[l1].disp==1)&&(GR_ObjTab[l1].hili == 1)) continue; // hidden

    // SizeInfo zu Tag/Image holen
    // DL_txtgetInfo (&typ, &p1, &sx, &sy, &dx, &dy, l1);
    GR_img_get_dbi (&typ, &p1, &sx, &sy, &dx, &dy, GR_ObjTab[l1].ind);

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
      // DEB_dump_obj__(Typ_PT, &p2, " RO=");

    // use RO
    UT3D_box_extend (pb1, pb2, &p2);   ++pNr;
  }

      // DEB_dump_obj__(Typ_PT, pb1, "  pb1o=");
      // DEB_dump_obj__(Typ_PT, pb2, "  pb2o=");

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
          UT3D_box_ox (&pb1, &pb2, ox1);  // siehe UT3D_npt_ox__
          // if(ox1->form == Typ_SUR) {
            // man sollte eine Box von der ersten Kontur holen;
            // die Boxpunkte DL_ReScalePoint
            // siehe GR_DrawSur/SUP_load_c .., UT3D_npt_ox__
            // UT3D_box_ox (&pb1, &pb2, ox1->data);  // siehe UT3D_npt_ox__
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
  lPar = DL_dli__dbo (typ, iPar, lChild-1);
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
  int DL_parent_ck_c (long dli) {
//================================================================
// DL_parent_ck_c       check if DL-record has a child-object (check .sChd-Flag)
// RetCod: 1 - obj has child;
//         0 - obj does not have child.

  int    ichd;

  if(dli > GR_TAB_IND) return -1;

  ichd = GR_ObjTab[dli].sChd;

  return ichd;

}


//================================================================
  int DL_child_set (long dli, int mode) {
//================================================================
// DL_child_set                 set/reset isChild-bit;
// mode     1 obj is child - has parent(s)
//          0 obj is not child; has no parent(s)

  // printf("DL_child_set %ld %d\n",dli,mode);

  GR_ObjTab[dli].sChd = mode;

  return 0;
    
}   


//================================================================
  int DL_parent_set (long dli, int mode) {
//================================================================
// DL_parent_set                set/reset isParent-bit;
// mode     1 obj is parent - has child(s)
//          0 obj is not parent; has no child(s)

  // printf("DL_parent_set %ld %d\n",dli,mode);
  // if(dli < 0) AP_debug__ ("DL_parent_set");

  GR_ObjTab[dli].sPar = mode;

  return 0;

}


/*
//=======================================================================
  int DL_parent_set (long lChild, long lPar) {
//=======================================================================
/// \code
/// store childInfo, parentInfo in DL
/// \endcode


  printf("DL_parent_set lChild=%ld lPar=%ld\n",lChild,lPar);


  if(lChild >= 0L)
    GR_ObjTab[lChild].sChd = 1;   // obj is child - has parent(s)

  if(lPar >= 0L)
    GR_ObjTab[lPar].sPar = 1;     // obj is parent; has child(s)


  return 0;

}
*/

//=======================================================================
  int DL_parent_hide (long dli_par) {
//=======================================================================
/// DL_parent_hide     hide parent.


  // printf("DL_parent_hide %ld\n",dli_par);

  // muss unsichtbar sein; Schoener waere dim+noPick ?
  GR_ObjTab[dli_par].unvis = 1; // unvis
  // GR_ObjTab[lPar].dim  = 0; // ON
  // GR_ObjTab[lPar].pick = 0; // not pickable


  // hide 
  // im Main einfach hide; im subModel delete notwendig !
  // printf("HHHHHHHHHHHHHHHHHHHHHHH _hideParent %d\n",dli_par);

  if((INT_16)GR_ObjTab[dli_par].modInd == MDL_BMI_ACT) {
    // DL_hili_off (dli_par);    // hili  = OFF; disp  = ON;
    DL_hide__ (dli_par, 1);  // hide=OFF   hili = OFF; disp = OFF;

  } else {
    // Parent ist in der Kette von obj's fuer das Ditto; das per callList 
    // gerufen wird; muss daher explizit geloescht werden ...
    // glDeleteLists (dli_par+DL_base__, 1);
    GL_Del0 (dli_par);
  }


  return 0;

}


//================================================================
  int DL_del_last () {
//================================================================
// DL_del_last             free (delete) last DL-obj

  if(GR_TAB_IND > 0) {
    --GR_TAB_IND;
    GL_Del1 (GR_TAB_IND);
  }

  return 0;

}


//================================================================
  int DL_disp_reset (int lNr) {
//================================================================
// remove all display-objects following (including) APT-line-Nr <lNr>
// - clear refModels, baseModels; 
// - disp parents of deleted objects

#define oaSIZ 10

  int     ii, i3, parTyp;
  long    l0, l1, l2, dli, dbi, parDbi;
  ObjDB   parTab[oaSIZ];
  ObjSRC  oPar;
  MemTab(ObjSRC) mtPar = _MEMTAB_NUL;


  // printf("''''''''''' DL_disp_reset lNr=%d GR_TAB_IND=%ld\n",lNr,GR_TAB_IND);
  // printf(" AP_modact_ibm=%d\n",AP_modact_ibm);
  // DL_DumpObjTab ("DL_disp_reset");
  // SRC_dump__ (1);


  if(PRC_IS_ACTIVE) {                 // 2013-04-03
    l0 = APP_dli_start;
  } else {
    l0 = 0;
  }


  //----------------------------------------------------
  // get dli = first DL-record to delete (and all following DL-records)
  for(l1=l0; l1<GR_TAB_IND; ++l1) {
    // printf(" [%d]=%d\n",l1,GR_ObjTab[l1].lNr);

    // skip all objects not belonging to the active model
    // if((signed short)GR_ObjTab[l1].modInd != -1) continue;  // skip submodels
    if((INT_16)GR_ObjTab[l1].modInd != AP_modact_ibm) continue;

    if(GR_ObjTab[l1].lNr < lNr) continue;
    // if(GR_ObjTab[l1].lNr <= lNr) continue; // DO NOT DELETE LAST OBJ
    // if(GR_ObjTab[l1].typ == Typ_Color) continue;
    dli = l1;
    goto L_del1;
  }

  // nothing to delete ..
    // printf(" ex-disp_reset-nothing\n");
  return 0;




  //----------------------------------------------------
  L_del1:
    // printf(" disp_reset-dli=%ld\n",dli);
  // delete DL-record <dli> and all following.
  // - clear refModels
  // - redisplay parent-objects of deleted child-objects
  //   find DL-recs with sPar==1;
  //     iatt of this rec = DL-Ind of childRec (= following DL-Rec)
  //     wenn childRec geloescht wird (im zu loeschenden DL-Bereich liegt),
  //       muss man den alten reaktivieren.
  //     reaktivieren parentRec:
  //       iatt of parentRec = iatt of childRec;
  //       sPar of parentRec==0.

  // get spc for parents 
  MemTab_ini_temp (&mtPar, Typ_ObjSRC, 128); 
  if(MEMTAB_RMAX(&mtPar) != 128) {TX_Print("*** UI_GR_Select1 E1");  return -1;}

  // loop tru all rec's to delete, start at end
  for(l1=GR_TAB_IND-1; l1>=dli; --l1) {

      // DL_DumpObj__ (l1, "-------------- disp_reset");

    // baseModel --------------------------------------
    if((INT_16)GR_ObjTab[l1].modInd != AP_modact_ibm) {
      ii = (INT_16)GR_ObjTab[l1].modInd;
      // clear baseModel
      MDL_mdb_clear_1 (ii);
      // skip following objs belonging to this baseModel
      L_del_mb1:
      if((INT_16)GR_ObjTab[l1-1].modInd == ii) {
        --l1;
        if(l1 < dli) break;
        goto L_del_mb1;
      }
      continue;
    }

    // baseModel of mockup --------------------------------------
    if(GR_ObjTab[l1].typ == Typ_Ditto) {
      // clear baseModel 
      MDL_mdb_clear_1 ((int)GR_ObjTab[l1].ind);
      continue;
    }

    // refModel --------------------------------------
    if(GR_ObjTab[l1].typ == Typ_Model) {
      // clear refMdl
      MDL_mdr_clear (GR_ObjTab[l1].ind);
      continue;
    }

    // obj-with-parent --------------------------------------
    // skip objs without parent
    if(GR_ObjTab[l1].sChd != 1) continue;

    // get ObjSRC from DL_att
    DL_oSrc_dli (&oPar, l1);

    // clear tempSpc for 128 mtPar-records
    MEMTAB_CLEAR (&mtPar);

    // get parents
    OPAR_get_src (&mtPar, &oPar);
    ii = mtPar.rNr;
      // printf(" disp_reset-parNr=%d\n",ii);


    // get table of parents of this child
    // ii = 0;
    // OPAR_get_src (&ii, parTab, oaSIZ, GR_ObjTab[l1].typ,GR_ObjTab[l1].ind);

    for(i3=0; i3<ii; ++i3) {   // loop tru parent-obj's

      // get l2 = dli of  parent-obj 
      // l2 = DL_dli__dbo (parTab[i3].typ, parTab[i3].dbInd, dli);
      l2 = (MEMTAB__(&mtPar, i3))->dli;
      if(l2 < 0) continue; // {TX_Print("DL_disp_reset E002"); break;}

      // set parent visible
      GR_ObjTab[l2].sPar  = 0; // reset Parent-state
      GR_ObjTab[l2].unvis = 0; // vis
      DL_hide__ (l2, 0);       // set visible = do not hide
    }

  }

  MemTab_free (&mtPar);

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



  //----------------------------------------------------
  // und die DispList zuruecksetzen;
  // dli Record und alle folgenden werden geloescht!
  L_done:
  GL_Delete (dli);

    // printf(" ex-disp_reset %ld\n",dli);

  return 0;

}


//================================================================
  int DL_setRefSys (long dli) {
//================================================================
/// RefSys entprechend DL-Record setzen

  int    irs;

  // printf("DL_setRefSys %ld\n",dli);

  if(dli < 1) {
    if(CONSTRPLN_IS_OFF) return 0;
    NC_setRefsys (0L);
    return 0;
  }

  if(dli > GR_TAB_IND) {printf("DL_setRefSys E001 %ld\n",dli); return -1; }

  // printf("  _setRefSys irs=%d %d\n",GR_ObjTab[dli].irs,WC_sur_ind);

  if(GR_ObjTab[dli].irs == WC_sur_ind) return 0;

  irs = GR_ObjTab[dli].irs;
  NC_setRefsys ((long)irs);

  return 0;

}
 

//================================================================
  int DL_dlRec__dli (DL_Att *dla, long dli) {
//================================================================
/// \code
/// DL_dlRec__dli      get DispList-record (DL_Att) from dli
/// RetCod:    1 = OK
///            0 = ERROR
/// was DL_GetAtt
/// \endcode

  if(dli < 0)           { TX_Error("DL_dlRec__dli E001"); return 0; }
  if(dli >= GR_TAB_IND) { TX_Error("DL_dlRec__dli E002"); return 0; }

  *dla =  GR_ObjTab[dli];  

  return 1;
}


//================================================================
  int DL_oSrc_dli (ObjSRC *oSrc, long dli) {
//================================================================
// DL_oSrc_dli             get ObjSRC from DispListRecord (DL_Att)

  *oSrc = OSRC_NUL;

  oSrc->typ  = GR_ObjTab[dli].typ;
  oSrc->dbi  = GR_ObjTab[dli].ind;
  oSrc->dli  = dli;
  oSrc->lnr  = GR_ObjTab[dli].lNr;
  // oSrc->lPos = NULL;

  return 0;

}


//================================================================
  int DL_typ_is_visTyp (int typ) {
//================================================================
/// \code
/// DL_dbi_is_visTyp        test if typ == visual typ (VC is not)
/// retCod:  1   yes, typ can have a DL-record;
///          0   no, typ cannot have a DL-record;
/// \endcode


  if((typ == Typ_VC)     ||
     (typ == Typ_VAR))       return 0;

  return 1;

}


//================================================================
  long DL_get__ (DL_Att **dl) {
//================================================================
// DL_get__                returns DispList
// Output:
//   dl      point dispList
//   retcod  nr of records of dispList


  if(dl) *dl = GR_ObjTab;

  return GR_TAB_IND;

}

//================================================================
   int DL_ck_typ_dbi (long dli, int typ, long ind) {
//================================================================
// DL_ck_typ_dbi           test DisplistRecord - typ and ind
// retCode     0 Ok, typ and ind of GR_ObjTab[dli] match
//             1 No, typ or ind is different or dli does not exist


  if((dli >= GR_TAB_IND)           ||
     (GR_ObjTab[dli].typ != typ)   ||
     (GR_ObjTab[dli].ind != ind))  return 1;

  return 0;

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


/* UNUSED
//================================================================
  int DL_disp_hili_last (int mode) {
//================================================================
/// DL_disp_hili_last       hilite last obj of DL


  printf("DL_disp_hili_last \n");

  if(mode == ON) {
    // hilite last obj of DL
    DL_hili_on (-2L);

  } else {
    // unhilite 
    DL_hili_off (-2L);
  }

  return 0;

}


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
//       DEB_dump_obj__ (Typ_ObjGX, o1, "_tra=");
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
  long DL_SetTmpObj (int typ, long dbi) {
//================================================================
// DL_SetTmpObj            store DL-record (only for active vector)


  GR_ObjTab[GR_TAB_IND].typ    = typ;
  GR_ObjTab[GR_TAB_IND].ind    = dbi;


  return GR_TAB_IND;

}


/* ===================== EOF ======================== */
