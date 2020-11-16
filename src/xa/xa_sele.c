//   xa_sele.c         selection functions             2010-04-29   RF
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
sele_ck_subCurv
  how to deactivate ? sele_set_subCurv ? see sele_setNoConstrPln
sele_set_types (Typ_PT .. liefert dzt alle Punkte und auch Kurven & Flächen
  (der Punkt wird abgeleitet);
  wenn man NUR echte Punkte will geht das ned !!!!!!!
  Alle aktivieren geht auch nicht;
  ev eigenen Switch ableiten od nicht ?
    braucht man zu Point; eigentlich auch Vektor usw !

-----------------------------------------------------
Modifications:
2000-05-01 Neu ex gcad; erster Entwurf vom 1.8.98. RF.

-----------------------------------------------------
*/
#ifdef globTag
 void sele(){}
#endif
/*!
\file  ../xa/xa_sele.c
\brief obj. selections 
\code
=====================================================
List_functions_start:

sele_ck_typ         Test if obj of typ iTyp is a requested typ.
sele_ck_ConstrPln   Test if must add "ConstrPlane" to option-menu
sele_ck_NoParents   test if must add parents to selection-list

sele_set_icon       add 2D-icon to list
sele_src_cnvt__     get sourceCodes of all useful components of selected obj
// sele_ck_subCurv     test if component of obj is useful
// sele_decode         decode selected object; change sel_object into req_object

sele_reset          reset selectionFilter
sele_reset_type     reset a single bit
sele_set__          set selectionFilter
sele_set_add        add obj to selectionfilter; typeGroups can be used;
sele_set_types      set selectionFilters
sele_set_pos        save GR_selPos__ and GR_selPos_CP
sele_setNoConstrPln disable selection of point on ConstrPln
sele_setNoParents   add or do not add parents to selection-list
sele_get_pos__      return GR_selPos__
sele_get_pos_CP     return GR_selPos_CP
sele_get_selPos     returns selected position as point on selected object
sele_get_reqTyp     return GR_reqTyp

UI_GR_set_selNam
UI_GR_get_selNam    return typ, dbi & name of the selected obj
UI_GR_set_sel__     set GR_selNam, GR_selTyp, GR_selDbi
UI_GR_set_sel_obj   set GR_selNam, GR_selTyp, GR_selDbi
UI_GR_set_selKey    GUI_MouseL | GUI_MouseR

sele_save
sele_restore

sele_dump1          print reqObjTab
sele_dump2          print GR_selNam, GR_selTyp, GR_selDbi

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


- define all selectabele objects with sele_set_add
- provide requested object from selected obj with sele_decode





-------------------------------------------------
UI_GR_ButtonPress
  UI_GR_Select1
    UI_GR_Select2              obj from list selected ..
      OMN_popup_Brw            create M3-menu
      UI_GR_Select_work1
        UI_GR_Select_work2
  UI_GR_Select2                one single obj selected ..

OMN_CB_popup                   M3-menuLine selected ..


-------------------------------------------------
M1: normale obj-selektion;
M3: Selektion einer dem Obj zugeordneten Funktion !
    mehrere Objekte übereinander: zuerst obj-selektion; dann Menü-Anzeige.



Mode VWR:
   M3-Menü eines nicht selektierten Objs:
      ausblenden,
      analysieren,
      zur Gruppe zufügen   (nur wenn eine group existiert)

   M3-Menü eines in der Gruppe befindlichen Objs:
      aus Gruppe entfernen
      Gruppe ausblenden
      Gruppe löschen
      analysieren

   M3-Menü Indicate:
      gesamte Gruppe ausblenden
      gesamte Gruppe löschen


Mode CAD (CAD-Funktion aktiv):
  wenn eine CAD_Funktion aktiv ist (via CAD-Typ_Group/IE_cad_Inp1_Grp):
    kein M3-Menü.
  wenn keine CAD_Funktion aktiv ist:
    (CAD-Funktion deaktivieren mit Esc)


Mode VWR | CAD (keine CAD-Funktion aktiv) | MAN |:
   M3-Menü eines nicht selektierten Objs:
      Editieren
      Löschen
      Punkte verschieben
      ausblenden
      analysieren
      zur Gruppe zufügen   (nur wenn eine group existiert)

   M3-Menü eines in der Gruppe befindlichen Objs:
      aus Gruppe entfernen
      gesamte Gruppe ausblenden
      gesamte Gruppe löschen

   M3-Menü Indicate:
      gesamte Gruppe ausblenden
      gesamte Gruppe löschen



-------------------------------------------------



select only Vertex:
  sele_set__ (Typ_Vertex);  // init and set selectionFilter
    GR_reqTyp = primary requested typ;
    sele_set_types sets bit-table reqObjTab bit-table;
       reqObjTab is a  bit-table, 1 bit for one typ; eg reqObjTab[Typ_PT]

sele_ck_typ         Test if obj of typ iTyp is a requested typ.


see also UI_GR_Sel_Filt_set  // set GR_Sel_Filter to eg modSstyl ..


GR_selPos__ (out via sele_get_pos__) ist mouseposition in userCoords
GR_selPos_CP (from sele_get_pos_CP) ist mouseposition on constrPlane in userCoords


*/

#ifdef _MSC_VER
#include "MS_Def0.h"
#endif


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>                    // va_list
#include <string.h>
#include <math.h>
#include <ctype.h>                     // islower ..

#include "../ut/ut_geo.h"              // OFF, ON ..
#include "../ut/ut_txt.h"              // UTX_Clear
#include "../ut/ut_BitTab.h"           // BitTab
#include "../ut/ut_gtypes.h"           // AP_src_typMod

#include "../gui/gui_types.h"

#include "../ut/func_types.h"          // Typ_Att_hili1
#include "../gr/ut_DL.h"               // DL_GetAtt
#include "../gr/ut_GL.h"               // GL_GetConstrPos

#include "../db/ut_DB.h"               // DB_GetPoint

#include "../xa/xa_sele.h"             // Typ_go*
#include "../xa/xa_uid.h"              // UI_MODE_CAD
#include "../xa/xa_ato.h"              // ATO_getSpc_tmp__
#include "../xa/xa_msg.h"              // MSG_*




// 
//================================================================
// EXTERN VARS:
// aus xa.c:
extern int       WC_sur_ind;            // Index auf die ActiveConstrPlane
extern Mat_4x3   WC_sur_imat;           // inverse TrMat of ActiveConstrPlane

// ex xa_ui.c:
extern int       UI_InpMode;

// ex ../xa/xa_ui_gr.c
extern int       GR_Event_Act;

// ex ../gr/ut_GL.c



//================================================================
// LOCAL VARS:
static BitTab (reqObjTab, TYP_SIZ);
static BitTab (bck_ObjTab, TYP_SIZ);

static int    GR_selTmpStat;     // 0=not saved, 1=point stored..

       int    GR_selBasTyp;      // selected object-typ
       int    GR_selTyp;         // modified selected object-typ
       long   GR_selDbi;         // dbi
       long   GR_selDli;         // dli
       char   GR_selNam[128];    // objname of selected object

static Point  GR_selPos__;         // mouseposition in userCoords of last selection
static Point  GR_selPos_CP;        // mouseposition on constPlane in userCoords

static int    GR_reqTyp;         // requested typ
static int    GR_NoConstrPln;    // disable selection of point on ConstrPln
static int    GR_NoParents=0;    // 0=add-parents; 1=do-not-add-parents
static int    bck_GR_NoConstrPln;

// static char   GR_actPos[60];     // cursorPos as "P(x y z)"




//================================================================
  int sele_get_pos_CP (Point *ptSelCP) {
//================================================================
// sele_get_pos_CP     return GR_selPos_CP
// get the cursor-position of last selection at the constrPlane
// see UI_GR_get_actPosA
// see sele_set_pos GR_set_constPlnPos GL_set_viewPlnPos
 
  // printf("sele_get_pos_CP \n");

  
  // input is absolute; if constrPlane is active, transfer input into UCS
  UTRA_UCS_WCS_PT (ptSelCP, &GR_selPos_CP);

    // DEB_dump_obj__ (Typ_PT, &GR_selPos_CP, "sele_get_pos_CP-GR_selPos_CP");
    // DEB_dump_obj__ (Typ_PT, ptSelCP, "ex-sele_get_pos_CP-ptSelCP");

  return 0;

}


//=============================================================================
  int sele_src_cnvt_add (subCvTab *sCva, int typ, char *so) {
//=============================================================================
// sele_src_cnvt_add               add a record into sCva
// retCod          0=OK; -9=EOM

  int   iNr;

  // printf("sele_src_cnvt_add typ=%d |%s|\n\n",typ,so);

  iNr = sCva->iNr;

  if(iNr < SUBCVTABSIZ) {
    sCva->cva[iNr].typ = typ;
    strcpy(sCva->cva[iNr].oid, so);
    ++sCva->iNr;

  } else {
    TX_Print("***** sele_src_cnvt_add EOM");
    return -9;
  }

  return 0;

}


//===========================================================================
  int sele_src_cnvt__ (subCvTab *sCva,
                      int selTyp, long dbi, int reqTyp, Point *selPos) {
//===========================================================================
/// \code
// sele_src_cnvt__      get sourceCodes of all useful components of selected obj
// replacing sele_ck_subCurv
//
// Input:
//   tabSiz       size of output table sca
//   selTyp,dbi   DB-obj to check (selected obj)
//   selPos       selection-point 
// Output:
//   sca       sourceCode for useful subObjs for selected obj (typ,dbi)
//   retCod    >=0  OK, nr of subObjs added
//             -1   Err - conversion Error
//             -2   Err - cannot create oTyp from iTyp,iDbi
//             -9   ERR - no more space in sCva
//
// get eg selected part of CCV or polygon if Line wanted
//
/// \endcode
//
//   reqTyp    selTyp          out
//   Typ_LN    Typ_CVPOL       L(S SEG)
//
//   Typ_LN    Typ_CI          L(C PTS) | L(C par) & L(C)   tangent & axis
//   Typ_LN    Typ_PLN         L(R)
//
//   Typ_VC    Typ_CVPOL       D(S SEG)
//   Typ_VC    Typ_CI          D(C par) & D(C)              tangent & axis
//   Typ_VC    Typ_LN          D(L)
//   Typ_VC    Typ_PLN         D(R)
//   Typ_VC    Typ_Model       D(M)
//   Typ_VC    Typ_Dimen       D(N)
//
//   Typ_PT    Typ_CCV         P(S SEG par)    
//   Typ_PT    Typ_CVPOL       P(S PTS) | P(S par)          cornerPt | paramPt
//   Typ_PT    Typ_CI          P(L PTS) | P(C par) & P(C)   endPt|paramPt & cenPt
//   Typ_PT    Typ_LN          P(L PTS) | P(L par)           
//   Typ_PT    Typ_PLN         P(R)
//   Typ_PT    Typ_Model       P(M)
//   Typ_PT    Typ_Dimen       P(N PTS)                     3 points 2D
//   Typ_PT    Typ_ATXT        P(N PTS)                     2 points
//   Typ_PT    Typ_GTXT        P(N)                         1 point
//
//   Typ_Val|Typ_XVal|Typ_YVal|Typ_ZVal
//             Typ_PT|Typ_VC|Typ_LN
//                             X(P|D|L) & Y(P|D|L) & Z(P|D|L)
//
// TODO: bei selektion von surf, sol muesste selMenu sein: selPos and ConstrPlane
//       dzt SRC_src_pt_dbo


  int     irc, i1, iNr, basTyp;
  char    so[200];
  Point   pt1;
  // int     irc, iVc, iLn=0, iCi=0, iCv, iPt, ii, oTyp, iTyp;
  // char    so[128], cto;


  // printf("--- sele_src_cnvt__ selTyp=%d dbi=%ld reqTyp=%d\n",selTyp,dbi,reqTyp);
  // DEB_dump_obj__ (Typ_PT, selPos, " selPos");
  // GR_tDyn_symB__ (selPos, SYM_STAR_S, ATT_COL_RED);


  iNr = 0;
  irc = 0;

  basTyp = selTyp;

  if(selTyp == Typ_CV) {
    selTyp = DB_get_typ_cv (dbi);

  } else if(selTyp == Typ_Note) {
    selTyp = DB_get_typ_note (dbi);
  }
    // printf(" src_cnvt__-selTyp=%d\n",selTyp);



  if(reqTyp == selTyp) goto L_exit;  // no components


  // test if selTyp is active from groupTyp;
  // if(reqTyp >= Typ_goGeom) {
    // if(reqTyp == Typ_goGeom) goto L_exit;  // all; no components 
    // if(BitTab_get (reqObjTab, selTyp)) goto L_exit;
  // }



  //================================================================
  // make simple conversions direct (was sele_decode)
  //================================================================
  // simple-conversion: eg requested=vector; selected=line; get L(D)

  if(selTyp == Typ_TmpPT) {
    // indicate
    //.......................................
    if((reqTyp == Typ_PT)   ||
       (reqTyp == Typ_VC))      {
      // make "<xval yval zVal>"
      sele_get_pos_CP (&pt1);
      SRC_src_pt3_10 (sCva->cva[0].oid, &pt1);
      // sprintf(sca[0].oid, "P(%s)", so);
      goto L_add_1;

    //.......................................
    } else return 0;


  //================================================================
  } else if(selTyp == Typ_PT) {
    //.......................................
    if(reqTyp == Typ_VC) {
      // D(P)
      sprintf(sCva->cva[0].oid, "P%ld", dbi);   // NOT YET COMPLETE ..
      // IE_inp_chg (-3); // do NOT proceed to next inputfield
      goto L_add_1;

    //.......................................
    } else if(reqTyp == Typ_LN)   {
      // L(P)
      sprintf(sCva->cva[0].oid, "P%ld", dbi);   // NOT YET COMPLETE ..
      // IE_inp_chg (-3); // do NOT proceed to next inputfield
      goto L_add_1;

    //.......................................
    } else if(TYP_IS_VAL(reqTyp)) {   // Typ_Val|Typ_XVal|Typ_YVal|Typ_ZVal
      // add X(P) + Y(P) + (Z(P)
      sprintf(sCva->cva[0].oid, "X(P%ld)", dbi);
      sprintf(sCva->cva[1].oid, "Y(P%ld)", dbi);
      sprintf(sCva->cva[2].oid, "Z(P%ld)", dbi);
      iNr = 3;
      goto L_add__;

    //.......................................
    } else return 0;


  //================================================================
  } else if(selTyp == Typ_LN) {
    //.......................................
    if(reqTyp == Typ_VC) {
      // D(L)
      sprintf(sCva->cva[0].oid, "D(L%ld)", dbi);
      goto L_add_1;

    //.......................................
    // LN selected: for Typ_XVal|Typ_YVal|Typ_ZVal add distance ..
    } else if(TYP_IS_VAL(reqTyp)) {   // Typ_Val|Typ_XVal|Typ_YVal|Typ_ZVal
    // add X(L) + Y(L) + (Z(L)
      sprintf(sCva->cva[0].oid, "L%ld", dbi);
      sprintf(sCva->cva[1].oid, "X(L%ld)",dbi);
      sprintf(sCva->cva[2].oid, "Y(L%ld)",dbi);
      sprintf(sCva->cva[3].oid, "Z(L%ld)",dbi);
      iNr = 3;
      goto L_add__;
    }


  //----------------------------------------------------------------
  } else if(selTyp == Typ_CI) {
    //.......................................
      // // D(L)
      // sprintf(sca[0].oid, "D(C%ld)", dbi);
      // goto L_add_1;

    //.......................................
    // CI is selected: for TYP_IS_VAL add radius ..
    if(TYP_IS_VAL(reqTyp)) {   // Typ_Val|Typ_XVal|Typ_YVal|Typ_ZVal
    // add VAL(C)
      sprintf(sCva->cva[0].oid, "VAL(C%ld)",dbi);
      goto L_add_1;
    }


  //----------------------------------------------------------------
  // plane selected -
  } else if(selTyp == Typ_PLN) {
    //.......................................
    //.......................................
    if(BitTab_get(reqObjTab, Typ_PT)) {
      // P(R)
      sprintf(sCva->cva[0].oid, "P(R%ld)", dbi);
      goto L_add_1;

/*
    //.......................................
    } else if(BitTab_get(reqObjTab, Typ_VC)) {
      // D(R)
      sprintf(sCva->cva[0].oid, "D(R%ld)", dbi);


    if(reqTyp == Typ_LN) {
      // L(R)
      sprintf(sCva->cva[0].oid, "L(R%ld)", dbi);
      goto L_add_1;

    //.......................................
    } else if(BitTab_get(reqObjTab, Typ_VC)) {
      // D(R)
      sprintf(sCva->cva[0].oid, "D(R%ld)", dbi);
      goto L_add_1;

    //.......................................
    } else if(BitTab_get(reqObjTab, Typ_PT)) {
      // P(R)
      sprintf(sCva->cva[0].oid, "P(R%ld)", dbi);
      goto L_add_1;
*/

    //.......................................
    } else return 0;


  //----------------------------------------------------------------
// TODO: get points / vector of Typ_Dimen
   } else if((selTyp == Typ_Dimen) ||
             (selTyp == Typ_ATXT))     {
     return 0;


  //----------------------------------------------------------------
  } else if(selTyp == Typ_GTXT) {
    //.......................................
    if(BitTab_get(reqObjTab, Typ_PT)) {
      // P(N)
      sprintf(sCva->cva[0].oid, "P(N%ld)", dbi);
      goto L_add_1;

    //.......................................
    } else return 0;

/*
  //----------------------------------------------------------------
  } else if(selTyp == Typ_Dimen) {
    //.......................................
    if(reqTyp == Typ_VC) {
      // D(N)
      sprintf(sCva->cva[0].oid, "D(N%ld)", dbi);
      goto L_add_1;
    }

    } else return 0;


  //----------------------------------------------------------------
  } else if(selTyp == Typ_ATXT) {
    //.......................................
    if(reqTyp == Typ_PT) {
      // P(N PTS)
      sprintf(sCva->cva[0].oid, "P(N%ld)", dbi);
      goto L_add_1;

    //.......................................
    } else return 0;
*/


  //----------------------------------------------------------------
  } else if(selTyp == Typ_Model) {          // P D
    //.......................................
    if(reqTyp == Typ_VC) {
      // D(M)
      sprintf(sCva->cva[0].oid, "D(M%ld)", dbi);
      goto L_add_1;

    //.......................................
    } else if(reqTyp == Typ_PT) {
      // P(M)
      sprintf(sCva->cva[0].oid, "P(M%ld)", dbi);
      goto L_add_1;

    //.......................................
    } else return 0;



/*
  } else if((selTyp == Typ_Model)||(selTyp == Typ_Mock)) {
    if((reqTyp == Typ_CtlgPart)||(reqTyp == TYP_FilNam))
    ii = sizeof(GR_selNam);
    Mod_mNam_mdr (GR_selNam, &ii, &iTyp, GR_selDbi);
    // strcpy(GR_selNam, "symEl1/res1.gcad");
    // strcpy(GR_selNam, "Data/Niet1.gcad");
    if(iTyp == MBTYP_CATALOG)  {
      // catalogpart selected;
      if(GR_reqTyp == Typ_CtlgPart) {
        GR_selTyp = Typ_CtlgPart;
        goto L_exit;
      }
      UTX_ins_add (GR_selNam, sizeof(GR_selNam), "CTLG \"", "\"");
    }
    GR_selTyp = Typ_SubModel;
*/


  // //----------------------------------------------------------------
  // } else if(selTyp == Typ_SUR) ...



  }


  //================================================================
  // add subObjs
  //================================================================
  // requested=point; selected=line; get P(line, parameter)
  //                  selected=lineInCCV; get P(CCV subCurvInd parameter)
  // requested=line; selected=polygen; get L(polygone, subCurvInd)


  // test if only single-type requested; eg reqTyp=line, selected=lineInCCV
  if(reqTyp < Typ_goGeom) goto L_resolve; // single-type requested



  // compound-type; check all types in reqObjTab and add
  if(SELE_SRC_CNVT_NXT (Typ_VC) < -2) goto L_exit;   // calls sele_src_cnvt_test
  if(SELE_SRC_CNVT_NXT (Typ_PT) < -2) goto L_exit;   // calls sele_src_cnvt_test
  if(SELE_SRC_CNVT_NXT (Typ_LN) < -2) goto L_exit;
  if(SELE_SRC_CNVT_NXT (Typ_CI) < -2) goto L_exit;

  goto L_add_extra;

/*
  // only this obj's can have subCurves:
  if(selTyp == Typ_LN) {
     // L can only provide P
     // if(sele_ck_typ (Typ_PT)) goto L_resolve;
     // if(sele_ck_typ (Typ_TmpPT)) goto L_resolve;
     // if(BitTab_get (reqObjTab, Typ_TmpPT)) {reqTyp = Typ_TmpPT; goto L_resolve;}
     if(BitTab_get (reqObjTab, Typ_PT))    {reqTyp = Typ_PT; goto L_resolve;}
// Typ_Vertex ? Typ_EyePT ? 
     goto L_exit;


  } else if(selTyp == Typ_CI) {
     // C can only provide P or tangentVec|Line
     // // if(sele_ck_typ (Typ_PT)) goto L_resolve;
     // // if(sele_ck_typ (Typ_TmpPT)) goto L_resolve;
     // // if(BitTab_get (reqObjTab, Typ_TmpPT)) {reqTyp = Typ_TmpPT; goto L_resolve;}
     if(BitTab_get (reqObjTab, Typ_PT)) {
       iNr = sele_src_cnvt_do (Typ_PT, sca, tabSiz, iNr, selTyp, dbi, selPos);
       if(iNr >= tabSiz) goto L_exit;
     }
     if(BitTab_get (reqObjTab, Typ_VC)) {
       iNr = sele_src_cnvt_do (Typ_VC, sca, tabSiz, iNr, selTyp, dbi, selPos);
       if(iNr >= tabSiz) goto L_exit;
     }
     if(BitTab_get (reqObjTab, Typ_LN)) {
       iNr = sele_src_cnvt_do (Typ_LN, sca, tabSiz, iNr, selTyp, dbi, selPos);
       if(iNr >= tabSiz) goto L_exit;
     }
// Typ_Vertex ? Typ_EyePT ? 
     goto L_add_extra;


  } else if(selTyp == Typ_CV) {
     selTyp = DB_get_typ_cv (dbi);
     // if(BitTab_get (reqObjTab, Typ_TmpPT)) { reqTyp = Typ_TmpPT; goto L_resolve;}
     if(BitTab_get (reqObjTab, Typ_PT)) {
       iNr = sele_src_cnvt_do (Typ_PT, sca, tabSiz, iNr, selTyp, dbi, selPos);
       if(iNr >= tabSiz) goto L_exit;
     }

     // if((selTyp == Typ_CVPOL)   ||
        // (selTyp == Typ_CVBSP))      {
     if(selTyp != Typ_CVTRM)   {
       // - can only provide vec or line;
     if(BitTab_get (reqObjTab, Typ_VC)) {
       iNr = sele_src_cnvt_do (Typ_VC, sca, tabSiz, iNr, selTyp, dbi, selPos);
       if(iNr >= tabSiz) goto L_exit;
     }
     if(BitTab_get (reqObjTab, Typ_LN)) {
       iNr = sele_src_cnvt_do (Typ_LN, sca, tabSiz, iNr, selTyp, dbi, selPos);
       if(iNr >= tabSiz) goto L_exit;
     }

     
     } else {
       // CCV, trimmed-curve: LN or CI or curve
       iNr = sele_src_cnvt_do (reqTyp, sca, tabSiz, iNr, selTyp, dbi, selPos);
       // iNr = sele_src_cnvt_do (Typ_goGeo1,sca,tabSiz,iNr,selTyp,dbi,selPos);
       if(iNr >= tabSiz) goto L_exit;
     }

     goto L_exit;
  }
  goto L_exit;
*/



  //================================================================
  L_resolve:
  // create src-obj of type <reqTyp> from selected obj <selTyp,dbi> and
  //   select-position <selPos>
  // reqTyp = SRC_src_pt_dbo (so, 200, reqTyp, selPos, selTyp, dbi);
    // printf(" f-src_pt_dbo reqTyp=%d so=|%s|\n",reqTyp,so);
  // // add src-obj to sca
  // iNr = sele_src_cnvt_add (sca, tabSiz, iNr, reqTyp, so);
  irc = sele_src_cnvt_do (sCva, reqTyp, selTyp, dbi, selPos);
  if(irc < 0) return irc;   // -9 - no more space in sCva
  // if(iNr >= tabSiz) goto L_exit;



  //================================================================
  // add additional objs to selection:
  // eg add centerpoint of Circle; but before the parametric selection-point 
  //    must have been added.
  L_add_extra:

  // if CI is selected: add axis for L and D; add centerPt for P
  if(selTyp == Typ_CI) {

    if(reqTyp == Typ_LN) {
      sprintf(so,"L(C%ld)",dbi);

    } else if(reqTyp == Typ_VC) {
      sprintf(so,"D(C%ld)",dbi);

    } else if(reqTyp == Typ_PT) {
      sprintf(so,"P(C%ld)",dbi);

    } else goto L_exit;

    // add src-obj to sca
    irc = sele_src_cnvt_add (sCva, reqTyp, so);
    if(irc < 0) goto L_exit;

  }

  //----------------------------------------------------------------
  L_exit:

    // TESTBLOCK
    // printf(" ex-sele_src_cnvt__ irc=%d iNr=%d\n",irc,sCva->iNr);
    // if(sCva->iNr > 0) { for(i1=0; i1<sCva->iNr; ++i1) {
      // printf(" sele_src_cnvt__-sca[%d] = %d |%s|\n",
             // i1,sCva->cva[i1].typ,sCva->cva[i1].oid); }}
    // END TESTBLOCK


  if(irc >= 0) irc = sCva->iNr;
  return irc;


  //----------------------------------------------------------------
  L_add_1:
    iNr = 1;

  L_add__:
    for(i1=0; i1<iNr; ++i1) {
      // sca[0].oid already filled ..
      sCva->cva[i1].typ = reqTyp;
    }
    sCva->iNr = iNr;
    goto L_exit;

}


//========================================================================
  int sele_src_cnvt_do (subCvTab *sCva,
                        int reqTyp, int selTyp, long dbi, Point *selPos) {
//========================================================================
// sele_src_cnvt_do              create src-obj of type <reqTyp>  and add to sca
/// retCod:  0       output of nearest selected point; not parametric ..
///         -1       conversion Error
///         -2       cannot create oTyp from iTyp,iDbi
///         -9       EOM - no space in sCva


  char    so[200];


  // printf("sele_src_cnvt_do reqTyp=%d selTyp=%d dbi=%ld\n",reqTyp,selTyp,dbi);


  // create src-obj of type <reqTyp> from selected obj <selTyp,dbi> and
  //   select-position <selPos>
  reqTyp = SRC_src_pt_dbo (so, 200, reqTyp, selPos, selTyp, dbi);
    // printf(" f-src_cnvt_do reqTyp=%d so=|%s|\n",reqTyp,so);

  if(reqTyp <= 0) return reqTyp;


  // add src-obj to sca
  return sele_src_cnvt_add (sCva, reqTyp, so);

}


//========================================================================
  int sele_src_cnvt_test (subCvTab *sCva,
                          int xTyp, int selTyp, long dbi, Point *selPos) {
//========================================================================
// sele_src_cnvt_test                resolv typ
// retCod    0=OK, -1=Error
// Input:
//   xTyp             outTyp
//   selTyp           selected type
// Output:
//   retCod   0       OK
//           -2       no solution for xTyp-selTyp
//           -9       unrecov. error
 
  int   iSel;

  // printf("sele_src_cnvt_test xTyp=%d selTyp=%d dbi=%ld\n",xTyp,selTyp,dbi);

  // skip xTyp==selTyp
  if(xTyp==selTyp) return 0;

  // test if xTyp is activated in reqObjTab
  iSel = sele_ck_typ (xTyp);
  if(!sele_ck_typ (xTyp)) return 0;
    // printf("sele_src_cnvt_test-ok %d\n",xTyp);


  // skip objects that cannot give xTyp
  if(xTyp == Typ_CI) {
    if((selTyp == Typ_LN)    ||
       (selTyp == Typ_CVELL) ||
       (selTyp == Typ_CVPOL) ||
       (selTyp == Typ_CVBSP))  return 0;
  }
   
  // yes, xTyp is wanted. Add it into sca.
  return sele_src_cnvt_do (sCva, xTyp, selTyp, dbi, selPos);

}


//================================================================
  // int sele_src_cnvt_init (subCvTab *sCva) {
//================================================================
  // return 0;
// }


/* replaced by sele_ck_subCurv

//===========================================================================
  int sele_ck_subCurv (subCurv sca[3], int typ, long dbi, Point *selPos) {
//===========================================================================
/// \code
// sele_ck_subCurv     test if component of obj is useful
/// get (max.) 3 subcurves for a selected curve.
///   sca[0]   subCurve
///   sca[1]   point
///   sca[2]   vector 
//
/// Input:
///   typ,dbi   DB-obj to check (selected obj)
///   selPos    selection-point 
/// Output:
///   sca       max 3 sub-parts of obj typ,dbi
///   retCod    nr of objs out
///
/// get eg selected part of CCV or polygon
/// \endcode

// array (max 3) typ und oid (for GR_selNam) liefern ?
//   sca soll output sein und typ und oid enthalten !
//   out-dbi immer 0, out-dli immer -1 ?

// TODO: contour inside contour; output of eg P(S# MOD(#1) MOD(#2) par)
//       // #1 is segNr of contour; #2 is segNr inside contour #1


  int     irc, iVc, iLn=0, iCi=0, iCv, iPt, ii, oTyp, iTyp;
  char    so[128], cto;


  printf("--- sele_ck_subCurv typ=%d dbi=%ld\n",typ,dbi);
  // DEB_dump_obj__ (Typ_PT, selPos, " selPos");


  for(ii=0; ii<3; ++ii) sca[ii].typ = Typ_Error;
  ii = 0;


  // test if selection of PT,VC wanted; 0 is no, else yes
  iPt = sele_ck_typ (Typ_PT);
  iVc = sele_ck_typ (Typ_VC);
    // printf(" ck_subCurv-iPt=%d iVc=%d\n",iPt,iVc);



  //----------------------------------------------------------------
  // is input curve or surf
  iTyp = AP_typDB_typ (typ);
    // printf(" iTyp=%d iPt=%d iVc=%d\n",iTyp,iPt,iVc);
  if((iTyp == Typ_LN) ||
     (iTyp == Typ_CI) ||
     (iTyp == Typ_CV))    goto L_ck_cv;

  if(iTyp == Typ_SUR) goto L_ck_su;
  goto L_err1;


  //================================================================
  // input = A|R
  L_ck_su:
  if(typ == Typ_PLN) {
    // D(R)  and P(R)
    APED_oid_dbo__ (so, typ, dbi); // so = "R#"
    if(iPt)        cto = AP_typChar_typ (Typ_PT);
    else if(iVc)   cto = AP_typChar_typ (Typ_VC);
    else goto L_err1;
    sprintf(sca[ii].oid, "%c(%s)",cto,so);
    sca[ii].typ = typ;
    ii = 1;
    goto L_ck_ex;
  }

  // surfs not yet implemented ?
  goto L_err1;



  //================================================================
  // input = P|L|C|S
  L_ck_cv:
  // get exact typ of inputCurve
  if(iTyp == Typ_CV) iTyp = DB_get_typ_cv (dbi);
    // printf(" ck_subCurv-iTyp=%d\n",iTyp);


  //----------------------------------------------------------------
  // 1. step: get subCurve
  // test if L or C or S requested; 0=no, else yes.
  iLn = sele_ck_typ (Typ_LN); // linear (LN)
  iCi = sele_ck_typ (Typ_CI); // circular (CI)
  iCv = sele_ck_typ (Typ_CV); // curved (CV)
    // printf(" ck_subCurv-iLn=%d iCi=%d iCv=%d\n",iLn,iCi,iCv);


  // only this obj's can have subCurves:
  if((iTyp == Typ_CVTRM)  ||
     (iTyp == Typ_CVPOL)) {
    if((iLn)||(iCi)||(iCv)) goto L_ck_1;
  }

  // not LN,CI,bspl: get P,D
  goto L_ck_3;


  //----------------------------------------------------------------
  L_ck_1:
    // printf("L_ck_1: iTyp=%d iLn=%d iCi=%d iCv=%d\n",iTyp,iLn,iCi,iCv);
  // get subCurve of CCV:  L|C|S, dann P,D
  // if L in CCV selected: OK; if C in CCV selcted: oTyp=0.
  if(iTyp == Typ_CVPOL) oTyp = Typ_LN; // 2017-09-26
  else oTyp = Typ_goGeo1;
  // get src-obj so of type=oTyp from selPos and db-object (typ,dbi)
  oTyp = SRC_src_pt_dbo (so, 200, oTyp, selPos, typ, dbi);
    // printf(" ck_subCurv-Ln-CCV oTyp=%d so=|%s|\n",oTyp,so);
  if(oTyp >= 0) {   // 2015-10-24
// TODO: test if useful (if oTyp is wanted - LN or CI or curve)
//       oTyp should be type of basic-curve for trimmed-curve
    sca[ii].typ = oTyp;
    strcpy(sca[ii].oid, so);
    ++ii;
  }
  // goto L_ck_3;

//
//   //----------------------------------------------------------------
//   // plg: L, dann P,D
//   // LN from curve or subcurve
//   L_ck_2:
//   if(iLn) {
//     oTyp = Typ_CV; // oTyp = Typ_LN;
//     oTyp = SRC_src_pt_dbo (so, 200, oTyp, selPos, typ, dbi);
//       // printf(" ck_subCurv-Ln_POL oTyp=%d so=|%s|\n",oTyp,so);
//     if(oTyp >= 0) {   // 2015-10-24
//       sca[ii].typ = oTyp;
//       strcpy(sca[ii].oid, so);
//       ++ii;
//     }
//   }
//
    // printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX \n");


  //----------------------------------------------------------------
  // 3. step: get Point from curve or subcurve
  L_ck_3:
    // printf(" sel PT iPt=%d\n",iPt);
  if(iPt) {
    oTyp = Typ_PT;
    oTyp = SRC_src_pt_dbo (so, 200, oTyp, selPos, typ, dbi);
      // printf(" ck_subCurv-Pt oTyp=%d so=|%s|\n",oTyp,so);
    if(oTyp >= 0) {   // 2015-10-24
      sca[ii].typ = oTyp;
      strcpy(sca[ii].oid, so);
      ++ii;
    }
  }
    // printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX \n");



  //----------------------------------------------------------------
  // get Vc from curve or subcurve
  if(iVc) {
    if(typ == Typ_VC) goto L_ck_ex;
    // add Vector to list
    oTyp = Typ_VC;
    oTyp = SRC_src_pt_dbo (so, 200, oTyp, selPos, typ, dbi);
      // printf(" ck_subCurv-Vc oTyp=%d so=|%s|\n",oTyp,so);
    if(oTyp >= 0) {   // 2015-10-24
      sca[ii].typ = oTyp;
      strcpy(sca[ii].oid, so);
      ++ii;
    }
  }

  //----------------------------------------------------------------
  L_ck_ex:

    // TESTBLOCK:
    { int i1,typ; long dbi; ObjAto    ato;
      // ATO_getSpc__ (&ato); //ato.ilev = memspc012;
      for(i1=0; i1<3; ++i1) {
        if(sca[i1].typ == Typ_Error) continue;
        // printf(" AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
        printf(" sca[%d] typ=%d oid=|%s|\n",i1,sca[i1].typ,sca[i1].oid);
        // ATO_clear__ (&ato);
        // ATO_ato_srcLn__ (&ato, sca[i1].oid);
        // ATO_dump__ (&ato, sca[i1].oid);
        // display subCurv
        // typ = ato.typ[0]; dbi = ato.val[0]; GR_tDyn_dbo (typ, dbi, 9, 0);
    }}
    // END TESTBLOCK:



    printf("ex-SRC_src_pt_dbo irc=%d so=|%s|\n",ii,so);


  return ii;

  L_err1:
    TX_Error ("sele_ck_subCurv E001_%d",iTyp); 
    return -1;

}
*/


//================================================================
  int sele_set_icon (int *ib, int iTyp) {
//================================================================
// add 2D-GL-button(s) to list
// Input:
//   ib      button-index to use, clearing all following buttons
// Output:
//   ib      ib += 1;
//
// see also APED_oid_dbo_all (get iTxt from iTyp)


  char      *p1;


  // printf("sele_set_icon %d %d\n",*ib, iTyp);

  // if not CAD: return
  if(UI_InpMode != UI_MODE_CAD) return -1;


  //----------------------------------------------------------------
  // add group of buttons
  if(iTyp == Typ_VAR) {
    sele_set_icon (ib, Typ_FncVAR2);
    sele_set_icon (ib, Typ_FncVAR1);
    return 0;


  } else if(iTyp == Typ_VC) {
    sele_set_icon (ib, Typ_FncVC2);   // VC- previous
    sele_set_icon (ib, Typ_FncVC1);   // VC+ next
    return 0;


  } else if(iTyp == Typ_mod1) {
    sele_set_icon (ib, Typ_FncPrv);
    sele_set_icon (ib, Typ_FncNxt);
    return 0;


  } else if(iTyp == Typ_FncDirX) {
    sele_set_icon (ib, Typ_modCX);
    sele_set_icon (ib, Typ_modPARL);
    return 0;
  }


  //----------------------------------------------------------------
  // add single button
  // get text
  p1 = AP_src_typMod (iTyp);
  if(!p1) {TX_Error("sele_set_icon E001 %d",iTyp); return -1;}


  // add GL-2D-button
  GLBT_but_add (ib, iTyp, p1);


  return 0;

}


//================================================================
  int UI_GR_set_sel_obj (int typ, long dbi) {
//================================================================
// set GR_selTyp,GR_selDbi,GR_selNam
//
// see also UI_GR_set_sel_obj UI_GR_set_selNam
 
  // GL_sel_dump ();

  GR_selTyp = typ;
  GR_selDbi = dbi;
  APED_oid_dbo__ (GR_selNam, GR_selTyp, GR_selDbi);

  return 0;

}


//================================================================
  int UI_GR_set_sel__ (long objInd) {
//================================================================
// set GR_selNam, GR_selTyp, GR_selDbi
// see also UI_GR_set_sel_obj UI_GR_set_selNam


  DL_Att    objAtt;


  // printf("UI_GR_set_sel__ dli=%ld\n",objInd);

  //----------------------------------------------------------------
  // -1 = clear selection
  if(objInd < 0) {         // eg a not pickable obj sel ..
    GR_selDli = 0L;
    GR_selTyp = 0;
    GR_selBasTyp = 0;
    GR_selDbi = 0;
    GR_selNam[0] = '\0';


  //----------------------------------------------------------------
  } else {
    GR_selDli = objInd;
    // objAtt = DL_GetAtt (GR_selDli);
    DL_dlRec__dli (&objAtt, GR_selDli);
    GR_selTyp = objAtt.typ;
    GR_selDbi = objAtt.ind;
    APED_oid_dbo__ (GR_selNam, GR_selTyp, GR_selDbi);
  }

  // printf("ex UI_GR_set_sel__ typ=%d ind=%d |%s|\n",GR_selTyp,GR_selDbi,GR_selNam);

  return 0;

}


//================================================================
  int UI_GR_set_selKey (int key) {
//================================================================
// GUI_MouseL | GUI_MouseR

  GR_Event_Act = key;

  return 0;

}

 
//================================================================
  int UI_GR_set_selNam (int selTyp, long selDbi, char *selNam) {
//================================================================
// see also UI_GR_set_sel__ UI_GR_set_sel_obj

  // printf("UI_GR_set_selNam %d %ld |%s|\n",selTyp,selDbi,selNam);


  if(selTyp >= 0) GR_selTyp = selTyp;
  GR_selDbi = selDbi;
  if(selNam)      strcpy (GR_selNam, selNam);

  // printf("ex UI_GR_set_selNam typ=%d ind=%d |%s|\n",
         // GR_selTyp,GR_selDbi,GR_selNam);

  return 0;

}


//================================================================
  int sele_get_selPos (Point *pts) {
//================================================================
// UNUSED IN CORE !
/// \code
/// returns selected position as point. 
/// selectionFilter must have been set to point (sele_set__ (Typ_PT) !)
/// RetCod:
///  -2       point not requested ..
///  -1       cannot convert to point
///   0       indicate; nothing selected ..
///   1       point-object selected;
///   2       curve- or surface-obj selected; selection-position computed
/// \endcode

  int       irc, typ, i1;
  Point     pSel;



  // printf("sele_get_selPos \n");
  // printf("  GR_reqTyp=%d \n",GR_reqTyp);
  // printf("  GR_selBasTyp=%d \n",GR_selBasTyp);
  // printf("  GR_selTyp=%d \n",GR_selTyp);
  // printf("  GR_selDbi=%ld \n",GR_selDbi);
  // printf("  GR_selNam=|%s|\n",GR_selNam);



  // 2015-09-01
  if(!strcmp(GR_selNam, "ConstrPlane") )  {
    UI_GR_get_actPosA (&pSel);
    sprintf(GR_selNam, "P(%f %f %f)", pSel.x, pSel.y, pSel.z);

  } else if(!strcmp(GR_selNam, "selPos") )  {
    sele_get_pos__ (&pSel);
    sprintf(GR_selNam, "P(%f %f %f)", pSel.x, pSel.y, pSel.z);
  }



  if(GR_selBasTyp == Typ_PT) {
    // point selected:
    typ = Typ_PT;
    // UTO_obj_dbo (pts, &i1, &typ, GR_selDbi);
    *pts = DB_GetPoint (GR_selDbi);
    irc = 1;

  } else {

    irc = APT_obj_expr (pts, Typ_PT, GR_selNam);
    if(irc >= 0) irc = 2;
    // TX_Print("**** selection of this objTyp not supported ..");
    // return -1;
  }

    // DEB_dump_obj__ (Typ_PT, pts, "ex sele_get_selPos %d",irc);

  return irc;

}


//================================================================
  int UI_GR_get_selNam (int *selTyp, long *selDbi, char **selNam) {
//================================================================
/// \code
/// UI_GR_get_selNam     return typ, dbi & name of the selected obj
/// eg "P(S20 0.478)"
/// get position with sele_get_pos__
/// Indicate: selNam is empty; get it with UI_GR_get_actPos_().
/// \endcode

// see also UI_GR_set_sel

  Point  pSel;

  *selTyp = GR_selTyp;
  *selDbi = GR_selDbi;
  *selNam = GR_selNam;

  if(!strcmp(GR_selNam, "ConstrPlane") )  {
    // UI_GR_get_actPos_ (&selNam);
    UI_GR_get_actPosA (&pSel);
    sprintf(GR_selNam, "P(%f %f %f)", pSel.x, pSel.y, pSel.z);
    *selNam = GR_selNam;
  }

  // printf("ex UI_GR_get_selNam typ=%d ind=%ld |%s|\n",
         // GR_selTyp,GR_selDbi,GR_selNam);

  return 0;

}


//================================================================
  int sele_set_pos (Point *spt) {
//================================================================
// sele_set_pos                  set GR_selPos__ and GR_selPos_CP

  GR_selPos__ = *spt;

  GR_selPos_CP = GL_GetConstrPos (spt);

    // DEB_dump_obj__ (Typ_PT, &GR_selPos__, "ex-sele_set_pos ");
    // DEB_dump_obj__ (Typ_PT, &GR_selPos_CP, "ex-sele_set_pos-CP");

  return 0;

}


//================================================================
  int sele_get_reqTyp () {
//================================================================
// sele_get_reqTyp    return GR_reqTyp

  return GR_reqTyp;

}


//================================================================
  int sele_get_pos__ (Point *pto) {
//================================================================
/// \code
/// return mouseposition in userCoords
/// position when mousebutton was pressed 
/// \endcode

// GL_MousePos GL_GetActSelPos UI_GR_get_actPosA sele_get_pos__ GR_selPos__

  *pto = GR_selPos__;

  return 0;

}


//================================================================
  int sele_ck_typ (int iTyp) {
//================================================================
// Test if obj of typ iTyp is a requested typ.
//  RetCod=0; no; else yes
// reqObjTab is set by func sele_set__

  int  i1;

  //----------------------------------------------------------------
  // test if selected is requested
  // printf("sele_ck_typ iTyp=%d isOn=%d\n",iTyp,BitTab_get(reqObjTab,iTyp));
  // BitTab_dump (reqObjTab, TYP_SIZ);


  // if GR_Sel_Filter==18 (parametric point) keep selection
  i1 = UI_GR_Sel_Filt_set (-1);   // query
  if(i1 == 18) return 1;   // 2013-04-17

  // if (GR_Sel_Filter==1) give temporary-point from cursorposition on constrPlane
  // get point on constrPlane
  if(i1 == 1)  return 1;   // 2013-04-17


  return BitTab_get (reqObjTab, iTyp);  // !=0 is set

}


//================================================================
  int sele_setNoParents (int mode) {
//================================================================
/// \code
/// sele_setNoParents   add or do not add parents to selection-list
/// mode: 0=add-parents; 1=do-not-add-parents
///       (parentObjs are unvisible)
/// \endcode

  GR_NoParents = mode;

  return 0;

}

 
//================================================================
  int sele_ck_NoParents () {
//================================================================
/// \code
/// sele_ck_NoParents   test if must add parents to selection-list
/// mode: 0=add-parents; 1=do-not-add-parents
/// \endcode

  return GR_NoParents;

}

 
//================================================================
  int sele_ck_ConstrPln () {
//================================================================
// Test if must add "ConstrPlane" to option-menu. RetCod=0; no; else yes, 
// Test if reqObj == point;  else return No.
// List_of_selectable_objects == empty: return "point on constrPlane";
//
// RetCod:
//   0       no "ConstrPlane" necessary
//   else    yes, add "ConstrPlane" to objMenu

// see sele_setNoConstrPln

  int  i1 = 0;


  // printf("sele_ck_ConstrPln %d\n",GR_NoConstrPln);
  // sele_dump1 ();


  if(GR_NoConstrPln) goto L_exit;


  //----------------------------------------------------------------
  // test if Typ_PT | Typ_TmpPT is requested
    // printf("sele_ck_ConstrPln %d\n",BitTab_get(reqObjTab,Typ_PT));
  i1 = BitTab_get (reqObjTab, Typ_PT);           // !=0 is set
  if(i1) goto L_exit;

  i1 = BitTab_get (reqObjTab, Typ_TmpPT);  // !=0 is set


  L_exit:
    // printf("ex-ck_ConstrPln %d\n",i1);
  return i1;

}

//================================================================
  int sele_dump2 () {
//================================================================
// sele_dump2          print GR_selNam, GR_selTyp, GR_selDbi

  int    i1, i2, ii;

  printf("sele_dump2:\n");

  printf(" GR_selNam=|%s| GR_selTyp=%d GR_selDbi=%ld\n",
         GR_selNam, GR_selTyp, GR_selDbi);


  return 0;

}


//================================================================
  int sele_dump1 (char *txt) {
//================================================================
// sele_dump1          print reqObjTab

  int    i1, i2, ii;

  printf("sele_dump1 - %s\n",txt);


  // test if all objs are on
  for(i1=0; i1<TYP_SIZ; ++i1) {
    if(BitTab_get (reqObjTab, i1) == 0) goto L_1;
  }
  printf(" all objects are selectable ..........\n");
  return 0;


  L_1:
  i2 = 0;
  for(i1=0; i1<TYP_SIZ; ++i1) {
    ii = BitTab_get (reqObjTab, i1);
    if(ii) {
      printf("reqObjTab[%d] = %d\n",i1,ii);
    }
/*
    ++i2;
    if(i2 >= 20) {
      i2 = 0;
      printf("      %d \n",i1);
    }
*/
  }
  printf("\n");

  return 0;

}


/* UNUSED ..
//================================================================
  int sele_decode () {
//================================================================
// decode selected object; change selected_object into requested_object

// Input:
//   GR_selTyp   eg Typ_PT
//   GR_selDbi   DB-index
//   GR_selNam   eg "P(S20 MOD(2) 0.123)"
//   GR_reqTyp
// Output:
//   GR_selTyp   eg Typ_PT
//   GR_selDbi   DB-index
//   GR_selNam   eg "P(S20 MOD(2) 0.123)"

// SpecialCase:
//  if(GR_selTyp==Typ_TmpPT): point on ConstrPlane in GR_selNam; GR_selDbi is 0.

// AP_src_sel_fmt AP_src_selObj ?
// SRC_dbo SRC_fmt_sub SRC_fmt ??


  int     irc, i1, ii, iTyp, typBas;
  long    l1;
  double  d1;
  char    *sp1, s1[64];
  Point   pt1, pt2;
  ObjAto  ato;                  // only temp


  printf("XXXXXXXXXXX sele_decode typ=%d dbi=%ld dli=%ld |%s| XXXXXXXXXXXX\n",
         GR_selTyp, GR_selDbi, GR_selDli, GR_selNam);
  // printf(" req=%d\n",GR_reqTyp);
  // printf(" GR_selTmpStat=%d\n",GR_selTmpStat);
  // sele_dump1 ();


  //----------------------------------------------------------------
  if(!strcmp(GR_selNam, "ConstrPlane") )  {
    UI_GR_get_actPosA (&pt1);
    // write "P(<x> <y> <z>)"
    SRC_src_pt3_10 (GR_selNam, &pt1);

  } else if(!strcmp(GR_selNam, "selPos") )  {
    sele_get_pos__ (&pt1);
    // write "P(<x> <y> <z>)"
    SRC_src_pt3_10 (GR_selNam, &pt1);
  }



  //----------------------------------------------------------------
  // execute functions-only

  if(GR_selTyp == TYP_FilNam) {           // 2D-button
      IE_cad_selM2 (0);                     // disp. fileList
      goto L_null_obj;

  } else  if(GR_selTyp == Typ_SubModel) { // 2D-button
      IE_cad_selM2 (0);                     // disp. fileList
      goto L_null_obj;

  } else  if(GR_selTyp == Typ_CtlgPart) { // 2D-button
    CTLG_Sel__ ();                          // display catalog-list and part-list
      goto L_null_obj;
  }



  //----------------------------------------------------------------
  if(GR_selTyp >= Typ_FncVAR1) {
    IE_set_inpSrc (3);
    if(GR_selTyp == Typ_FncVAR1) IE_cad_Inp1_nxtVal (1);
    if(GR_selTyp == Typ_FncVAR2) IE_cad_Inp1_nxtVal (-1);
    if(GR_selTyp == Typ_FncVC1)  IE_cad_Inp1_nxtVec (1);
    if(GR_selTyp == Typ_FncVC2)  IE_cad_Inp1_nxtVec (-1);
    if(GR_selTyp == Typ_FncPrv)  {
      if(GR_reqTyp == Typ_mod1) i1 = 1;
      else i1 = 2;
      IE_cad_Inp1_nxtMod (i1, -1);  // 2013-03-16
    }


    if(GR_selTyp == Typ_FncNxt)  {
      if(GR_reqTyp == Typ_mod1) i1 = 1;
      else i1 = 2;
      IE_cad_Inp1_nxtMod (i1, 1);
    }

    // if(GR_selTyp == Typ_FncDirX) IE_cad_Inp1_DirX ();

    if(GR_selTyp == Typ_FncPtOnObj)  UI_GR_Sel_Filt_set (18); // 2013-04-17

    if(GR_selTyp == Typ_FncPtOnCP)   UI_GR_Sel_Filt_set (1);
    goto L_null_obj;
  }

  // skip normal modifiers
  if(TYP_IS_MOD(GR_selTyp)) goto L_exit;




  //----------------------------------------------------------------
  // decode
  GR_selBasTyp = GR_selTyp;      // selected object-typ

  // requested nothing ..
  if(GR_reqTyp == Typ_Error) goto L_exit;

  // if (requested_typ == selected_typ):  nothing to do; create objname.
  if(GR_selTyp == GR_reqTyp) {
    if(GR_selTmpStat == 0) goto L_exit;
  }



  //================================================================
  // Vertex|Angle|Tra requested: ignore ..
  //================================================================
  if(GR_reqTyp == Typ_Vertex) goto L_exit;
  if(GR_reqTyp == Typ_Angle)  goto L_exit;
  if(GR_reqTyp == Typ_Tra)    goto L_exit;


  //================================================================
  // Typ_goGeom REQUESTED .. (alle)
  //================================================================
  if((GR_reqTyp == Typ_goGeom)  || (GR_reqTyp == Typ_Group))     {
    if(GR_selTyp == Typ_TmpPT)   goto L_pt_conv;
    // all other objs can be used directly:
    goto L_exit;


  //================================================================
  // Typ_goPrim REQUESTED ..
  //================================================================
  } else if(GR_reqTyp == Typ_goPrim) {
    // these objs can be used directly:
    if((GR_selTyp == Typ_PT)    ||
       (GR_selTyp == Typ_LN)    ||
       (GR_selTyp == Typ_CI)    ||
       (GR_selTyp == Typ_CVPOL) ||         // 2011-08-05 was Typ_CV
       (GR_selTyp == Typ_CVBSP) ||
       (GR_selTyp == Typ_CVELL) ||
       (GR_selTyp == Typ_CVCLOT))          // geht no ned ..
      goto L_exit;
    // these objs can be converted:
    if((GR_selTyp == Typ_TmpPT))   goto L_pt_conv;


  //================================================================
  // Typ_goGeo1 REQUESTED ..
  //================================================================
  } else if(GR_reqTyp == Typ_goGeo1) {
    // these objs can be used directly:
    if((GR_selTyp == Typ_LN)    ||
       (GR_selTyp == Typ_CI)    ||
       (GR_selTyp == Typ_CVPOL) ||         // 2011-08-05 was Typ_CV
       (GR_selTyp == Typ_CVBSP) ||
       (GR_selTyp == Typ_CVELL) ||
       (GR_selTyp == Typ_CVCLOT)||         // geht no ned ..
       (GR_selTyp == Typ_CVTRM) ||
       (GR_selTyp == Typ_PLN)   ||
       (GR_selTyp == Typ_SUR)   ||
       (GR_selTyp == Typ_SOL))    goto L_exit;
    // these objs can be converted:


  //================================================================
  // Typ_goGeo2 REQUESTED ..
  //================================================================
  } else if(GR_reqTyp == Typ_goGeo2) {
    // these objs can be used directly:
    if((GR_selTyp == Typ_CI)     ||
       (GR_selTyp == Typ_CVELL)) goto L_exit;     // 2011-08-05 was Typ_CV
    // these objs can be converted:


  //================================================================
  // Typ_go_PD REQUESTED ..
  //================================================================
  } else if(GR_reqTyp == Typ_go_PD) {
    // these objs can be used directly:
    if((GR_selTyp == Typ_PT)     ||
       (GR_selTyp == Typ_VC)) goto L_exit;
    // these objs can be converted:



  //================================================================
  // Typ_goGeo5 REQUESTED ..
  //================================================================
  } else if(GR_reqTyp == Typ_goGeo5) {
    // these objs can be used directly:
    if((GR_selTyp == Typ_CI)    ||
       (GR_selTyp == Typ_CVPOL) ||         // 2011-08-05 was Typ_CV
       (GR_selTyp == Typ_CVBSP) ||
       (GR_selTyp == Typ_CVELL) ||
       (GR_selTyp == Typ_CVTRM)) goto L_exit;
    // these objs can be converted:



  //================================================================
  // Typ_goGeo6 REQUESTED ..
  //================================================================
  } else if(GR_reqTyp == Typ_goGeo6) {
    // these objs can be used directly:
    if((GR_selTyp == Typ_PT)    ||
       (GR_selTyp == Typ_LN)    ||
       (GR_selTyp == Typ_CI))     goto L_exit;
    // these objs can be converted:
    if(GR_selTyp == Typ_TmpPT)    goto L_pt_conv;
    if(GR_selTyp == Typ_CVPOL)    goto L_ln_conv;
    if(GR_selTyp == Typ_CVTRM)    goto L_LnAc_conv;
    
                      

  //================================================================
  // Typ_goGeo7 REQUESTED 
  //================================================================
  } else if(GR_reqTyp == Typ_goGeo7) {
    // these objs can be used directly:
    if((GR_selTyp == Typ_VAR)   ||
       (GR_selTyp == Typ_VC)    ||
       (GR_selTyp == Typ_PT))     goto L_exit;
    // these objs can be converted:
    // make point:
    if((GR_selTyp == Typ_TmpPT))   goto L_pt_conv;
    // make vector:
    if((GR_selTyp == Typ_CVPOL) ||
       (GR_selTyp == Typ_LN)    ||
       (GR_selTyp == Typ_CVTRM))   goto L_vc_conv;



  //================================================================
  // Typ_goGeo8 REQUESTED 
  //================================================================
  } else if(GR_reqTyp == Typ_goGeo8) {
    // these objs can be used directly:
    if((GR_selTyp == Typ_VAR)   ||
       (GR_selTyp == Typ_PT))     goto L_exit;
    // these objs can be converted:
    if((GR_selTyp == Typ_TmpPT) ||
       (GR_selTyp == Typ_LN))   goto L_pt_conv;   // 2017-05-28; L -> P((L par)
    if((GR_selTyp == Typ_CI))      goto L_VAL_C;



  //================================================================
  // Typ_go_LR REQUESTED 
  //================================================================
  } else if(GR_reqTyp == Typ_go_LR) {
    goto L_exit;



  //================================================================
  // Typ_go_LCS REQUESTED ..    LN|AC|Ell|Bsp|Plg    Not Sur|Sol; Konturobj RevSur
  //================================================================
  } else if(GR_reqTyp == Typ_go_LCS) {
    // these objs can be used directly:
    if((GR_selTyp == Typ_LN)    ||
       (GR_selTyp == Typ_CI)    ||
       (GR_selTyp == Typ_CVPOL) ||         // 2011-08-05 was Typ_CV
       (GR_selTyp == Typ_CVBSP) ||
       (GR_selTyp == Typ_CVELL) ||
       (GR_selTyp == Typ_CVCLOT)||         // geht no ned ..
       (GR_selTyp == Typ_CVTRM))
      goto L_exit;
    // these objs can be converted:



  //================================================================
  // Typ_go_RA REQUESTED ..
  //================================================================
  } else if(GR_reqTyp == Typ_go_RA) {
    // these objs can be used directly:
    if((GR_selTyp == Typ_SUR)   ||
       (GR_selTyp == Typ_PLN))     goto L_exit;
    // these objs can be converted:



  //================================================================
  // Typ_goGeoSUSU REQUESTED ..
  //================================================================
  } else if(GR_reqTyp == Typ_goGeoSUSU) {
    // these objs can be used directly:
    if((GR_selTyp == Typ_SUR)   ||
       (GR_selTyp == Typ_SOL))     goto L_exit;
    // these objs can be converted:



  //================================================================
  // Typ_PLN REQUESTED ..
  //================================================================
  } else if(GR_reqTyp == Typ_PLN) {
    // these objs can be used directly:
    if (GR_selTyp == Typ_PLN)     goto L_exit;
    // these objs can be converted:
    if((GR_selTyp == Typ_PT)    ||
       (GR_selTyp == Typ_TmpPT) ||
       (GR_selTyp == Typ_LN))     goto L_exit;
// do not yet make "R(<selObj>)" of "<selObj>"
       // (GR_selTyp == Typ_LN))     goto L_REF__;
    // these objs can be used additional:
    if(GR_selTyp == Typ_VC)       goto L_exit;

    // Typ_CI ?
    // Typ_Model ?



  //================================================================
  // Typ_goAxis REQUESTED ..
  //================================================================
  } else if(GR_reqTyp == Typ_goAxis) {
    // these objs can be used directly:
    if((GR_selTyp == Typ_VC)   ||
       (GR_selTyp == Typ_PT)   ||
       (GR_selTyp == Typ_LN)   ||
       (GR_selTyp == Typ_PLN))     goto L_exit;
    // these objs can be converted:
    if((GR_selTyp == Typ_TmpPT))   goto L_pt_conv;





  // //================================================================
  // // Typ_goRadius REQUESTED ..
  // //================================================================
  // // TODO: returns obj directly; should return "Val(obj)" or DX(2 points) ...
  // } else if(GR_reqTyp == Typ_goRadius) {
    // // these objs can be used directly:
    // if((GR_selTyp == Typ_VAR)  ||
       // (GR_selTyp == Typ_PT)   ||
       // (GR_selTyp == Typ_CI))     goto L_exit;
    // // these objs can be converted:
    // if((GR_selTyp == Typ_TmpPT))   goto L_pt_conv;



  //================================================================
  // Typ_Val REQUESTED ..
  //================================================================
  } else if(GR_reqTyp == Typ_Val) {
    // these objs can be used directly:
    if((GR_selTyp == Typ_VAR))     goto L_exit;
    // these objs can be converted:
    if((GR_selTyp == Typ_TmpPT)    ||
       (GR_selTyp == Typ_PT))        goto L_val_conv;




  //================================================================
  // Typ_XVal REQUESTED ..
  //================================================================
  } else if(GR_reqTyp == Typ_XVal) {
    // these objs can be used directly:
    if((GR_selTyp == Typ_VAR)  ||
       (GR_selTyp == Typ_PT))      goto L_exit;
    // these objs can be converted:
    if((GR_selTyp == Typ_TmpPT))   goto L_pt_conv;
    // if((GR_selTyp == Typ_PT)) {
      // GR_selTyp = Typ_VC;
      // SRC_fmt_tab (GR_selNam, Typ_XVal, Typ_PT, (double)GR_selDbi, 0);
      // goto L_exit;
    // }



  //================================================================
  // Typ_YVal REQUESTED ..
  //================================================================
  } else if(GR_reqTyp == Typ_YVal) {
    // these objs can be used directly:
    if((GR_selTyp == Typ_VAR)  ||
       (GR_selTyp == Typ_PT))     goto L_exit;
    // these objs can be converted:
    if((GR_selTyp == Typ_TmpPT))   goto L_pt_conv;



  //================================================================
  // Typ_ZVal REQUESTED ..
  //================================================================
  } else if(GR_reqTyp == Typ_ZVal) {
    // these objs can be used directly:
    if((GR_selTyp == Typ_VAR)  ||
       (GR_selTyp == Typ_PT))     goto L_exit;
    // these objs can be converted:
    if((GR_selTyp == Typ_TmpPT))   goto L_pt_conv;



  //================================================================
  // Typ_Angle REQUESTED ..
  //================================================================
  } else if(GR_reqTyp == Typ_Angle) {
    // these objs can be used directly:
    if((GR_selTyp == Typ_VAR)  ||
       (GR_selTyp == Typ_VC)   ||
       (GR_selTyp == Typ_LN))
      goto L_exit;
    // these objs can be converted: gives D()
    if((GR_selTyp == Typ_CVPOL) ||
       (GR_selTyp == Typ_CVTRM))   goto L_vc_conv;
    if((GR_selTyp == Typ_TmpPT) ||
       (GR_selTyp == Typ_PT))      goto L_val_conv;



  //================================================================
  // Typ_VC VECTOR REQUESTED ..
  //================================================================
  } else if(GR_reqTyp == Typ_VC) {
    // these objs can be used directly:
    if((GR_selTyp == Typ_LN)    ||
       (GR_selTyp == Typ_CI)    ||
       (GR_selTyp == Typ_PLN))
      goto L_exit;
    // these objs can be converted:
    if((GR_selTyp == Typ_CVPOL) ||
       // (GR_selTyp == Typ_CVBSP) ||
       // (GR_selTyp == Typ_CVELL) ||
       // (GR_selTyp == Typ_CVCLOT)||
       (GR_selTyp == Typ_CVTRM) ||
       (GR_selTyp == Typ_SUR)   ||
       (GR_selTyp == Typ_Model))
      goto L_vc_conv;
    if((GR_selTyp == Typ_PT)    ||
       (GR_selTyp == Typ_TmpPT))  goto L_pt_conv;
    // if(GR_selTyp == Typ_TmpPT)   goto L_pt_conv;



  //================================================================
  // POINT REQUESTED ..
  //================================================================
  } else if(GR_reqTyp == Typ_PT) {
    // these objs can be used directly:
    // these objs can be converted:
    if((GR_selTyp == Typ_TmpPT) ||
       (GR_selTyp == Typ_LN)    ||
       (GR_selTyp == Typ_CI)    ||
       (GR_selTyp == Typ_CVLNA) ||         // 2011-09-07
       (GR_selTyp == Typ_CVPOL) ||         // 2011-08-05 was Typ_CV
       (GR_selTyp == Typ_CVBSP) ||
       (GR_selTyp == Typ_CVRBSP)||
       (GR_selTyp == Typ_CVELL) ||
       (GR_selTyp == Typ_CVCLOT)||
       (GR_selTyp == Typ_CVTRM) ||
       (GR_selTyp == Typ_PLN)   ||         // 2011-09-08
       (GR_selTyp == Typ_SUR)   ||
       (GR_selTyp == Typ_SOL)   ||
       (GR_selTyp == Typ_Model))   goto L_pt_conv;



  //================================================================
  // Typ_EyePT REQUESTED ..
  //================================================================
  } else if(GR_reqTyp == Typ_EyePT) {
    if(GR_selTyp == Typ_TmpPT) goto L_pt_conv;




  //================================================================
  // LINE REQUESTED ..
  //================================================================
  } else if(GR_reqTyp == Typ_LN) {

    if(GR_selTmpStat == 1) {             // 1 obj already stored
      if((GR_selTyp == Typ_TmpPT)  ||
         (GR_selTyp == Typ_PT)     ||
         (GR_selTyp == Typ_LN)     ||
         (GR_selTyp == Typ_VC))       goto L_ln_conv;
      goto L_exit;
    }

    // these objs can be used directly:
    if((GR_selTyp == Typ_PT)    ||
       (GR_selTyp == Typ_TmpPT))    goto L_tmpPt;

    // these objs can be converted:
    if((GR_selTyp == Typ_CVPOL) ||
       (GR_selTyp == Typ_CVTRM) ||
       (GR_selTyp == Typ_CVLNA) ||
       (GR_selTyp == Typ_PLN))      goto L_ln_conv;      // 2011-09-07

    // these objs cannot be used alone:
    if(GR_selTyp == Typ_VC)         goto L_err_ptBeforeVc;



  //================================================================
  // CIRC REQUESTED ..
  //================================================================
  } else if(GR_reqTyp == Typ_CI) {
    // these objs can be used directly:
    if((GR_selTyp == Typ_TmpPT)    ||
       (GR_selTyp == Typ_PT)       ||
       (GR_selTyp == Typ_Val)      ||
       (GR_selTyp == Typ_VAR))        goto L_ac_conv;
    // these objs can be converted:
    if(GR_selTyp == Typ_CVTRM)   { ii = Typ_CI; goto L_LnAc_1; }



  //================================================================
  // CURVE REQUESTED ..
  //================================================================
  } else if(GR_reqTyp == Typ_CV) {
    // these objs can be used directly:
    // these objs can be converted:



  //================================================================
  // SURF REQUESTED ..
  //================================================================
  } else if(GR_reqTyp == Typ_SUR) {
    // these objs can be used directly:
    // these objs can be converted:



  //================================================================
  // SOLID REQUESTED ..
  //================================================================
  } else if(GR_reqTyp == Typ_SOL) {
    // these objs can be used directly:
    // these objs can be converted:



  //================================================================
  // MODEL REQUESTED ..
  //================================================================
  } else if(GR_reqTyp == Typ_SubModel) {      // basicModel wanted
    // these objs can be converted:
    if((GR_selTyp == Typ_Model)  ||
       (GR_selTyp == Typ_Mock))        {
      goto L_mdl_conv1;   // get ModbasName from ditto
    }

  } else if(GR_reqTyp == Typ_Model)    {      // ModelReference wanted
    // these objs can be used directly:
    if(GR_selTyp == Typ_Mock)     goto L_exit;



  //================================================================
  // Filename REQUESTED ..
  //================================================================
  } else if(GR_reqTyp == TYP_FilNam) {
    // these objs can be converted:
    if((GR_selTyp == Typ_Model)  ||
       (GR_selTyp == Typ_Mock))        {
      goto L_mdl_conv1;   // get ModbasName from ditto
    }



  //================================================================
  // catalogPartNam REQUESTED ..
  //================================================================
  } else if(GR_reqTyp == Typ_CtlgPart) {
    // these objs can be converted:
    if((GR_selTyp == Typ_Model)  ||
       (GR_selTyp == Typ_Mock))        {
      goto L_mdl_conv1;   // get ModbasName from ditto
    }




  //================================================================
  } else {
    printf("sele_decode I001 %d\n",GR_reqTyp);
  }

  printf("***** sele_decode E001-%d-%d\n",GR_reqTyp,GR_selTyp);
  return -1;




  //================================================================
  // convert ModelReference -> ModbasName
  //================================================================
  L_mdl_conv1:
    // get ModbasName from ModRefNr
    ii = sizeof(GR_selNam);
    Mod_mNam_mdr (GR_selNam, &ii, &iTyp, GR_selDbi);
    // strcpy(GR_selNam, "symEl1/res1.gcad");
    // strcpy(GR_selNam, "Data/Niet1.gcad");
    if(iTyp == MBTYP_CATALOG)  {
      // catalogpart selected;
      if(GR_reqTyp == Typ_CtlgPart) {
        GR_selTyp = Typ_CtlgPart;
        goto L_exit;
      }
      UTX_ins_add (GR_selNam, sizeof(GR_selNam), "CTLG \"", "\"");
    }
    GR_selTyp = Typ_SubModel;
    goto L_exit;




  //================================================================
  // save 1. tempPoint;
  // Line from 2 tempPoints;
  //================================================================
  L_tmpPt:
      printf(" L_tmpPt: %d %d\n",GR_selTmpStat,GR_selTyp);

    if(GR_selTmpStat == 0) {

      if(GR_selTyp == Typ_TmpPT) {
        GR_selNam[0]='\0';
        UI_GR_get_actPosA  (&pt1);
        AP_obj_add_pt_sp (GR_selNam, &pt1);
      }
      IE_cad_Inp_disp_pt (&pt1, IE_get_inpInd());
      GR_selTmpStat = 1;
      goto L_exit;

/
    // } else {                    // ??
      // GR_selNam[0] = '\0';
      // // AP_obj_add_ln (GR_selNam, 0, &GR_selTmpPt, &GR_selPos__);
      // strcpy (GR_selNam, "L(");
      // strcat (GR_selNam, GR_selTmpID);
      // strcat (GR_selNam, ")");
      // GR_selTyp = Typ_LN;
      // GR_selDbi = 0L;
      // return 0;
/
    }




  //================================================================
  // convert obj -> VECTOR
  //================================================================
  L_vc_conv:
    // standardplanes: return its Z-vector
    if((GR_selTyp == Typ_PLN) && (GR_selDbi < 0)) {
      GR_selTyp = Typ_VC;         // output - exact.
      GR_selNam[0] = 'D';         // RX/Y/Z -> DX/Y/Z
      return 0;
    }

    // if upper inputField is a point: get this point, else get the
    // mouseposition.
    // get typ & content of previous (upper) inputField
/
    // IE_get_inp_t (&ii, &sp1, -1);
    // if(ii == Typ_PT) {
      // // get pointPos
      // // DOES NOT WORK FOR PointGroup, ERROR !
      // APT_obj_expr (&pt1, Typ_PT, sp1);
    // } else {
/
      // pt1 = GR_selPos__;   // get selected Mousepos in userCoords
    // }
      // printf(" pt1  %f %f %f\n",pt1.x,pt1.y,pt1.z);

    // create vector ..
    // typBas = AP_typDB_typ (GR_selTyp);
    // irc = SRC_vc_ptDbo (GR_selNam, &pt1, typBas, GR_selDbi);
    irc = SRC_src_pt_dbo (GR_selNam, sizeof(GR_selNam), Typ_VC,
                          &GR_selPos__, GR_selTyp, GR_selDbi);
    if(irc < 0) {
      printf(" cannot (yet) create vector from obj ..\n");
      goto L_null_obj;
    }
      // printf("  GR_selNam=|%s|\n",GR_selNam);
    GR_selTyp = Typ_VC;         // output - exact.
    GR_selDbi = 0L;             // create new obj
    return 0;



  //================================================================
  // convert obj -> POINT
  //================================================================
  L_pt_conv:

    if(GR_selTyp == Typ_PLN) {
      // skip selection of std-plane
      if((GR_selTyp == Typ_PLN) &&
         (GR_selDbi < 0))           goto L_null_obj;

      // i1 = GR_selTyp;
      // d1 = GR_selDbi;
      // SRC_fmt__ (GR_selNam, Typ_PT, 1, &i1, &d1);
      ATO_getSpc_tmp__ (&ato, 6);
      ATO_ato_expr_add (&ato, GR_selTyp, (double)GR_selDbi, 0);
      SRC_src_ato (GR_selNam, sizeof(GR_selNam), Typ_PT, &ato);
      GR_selTyp = Typ_PT;
      GR_selDbi = 0L;
      return 0;
    }

    // test if ConstrPlane was selected ...
    if(GR_selTyp == Typ_TmpPT) {
      // get current curPos in userCoords on constructionPlane 
      UI_GR_get_actPosA  (&pt1);

      // display position
      if((GR_reqTyp == Typ_PLN)||(GR_reqTyp == Typ_VC))
        IE_cad_Inp_disp_pt (&pt1, IE_get_inpInd());
/
// raus 2011-07-29
        // // der Punkt ist absolutKoordinaten; umrechnen in relative Koordinaten
      // // invert transformation if ConstrPln is set;
      // //   will be inverted in APT_decode_pt
      // if(AP_IS_2D) {
        // UT3D_pt_tra_pt_m3 (&pt1, WC_sur_imat, &pt1);
      // }
/
      UTX_Clear (GR_selNam);
      AP_obj_add_pt_sp (GR_selNam, &pt1);
      return 0;
    }

    //----------------------------------------------------------------
    // get parametric-point from selected-point and selected obj
    // irc = SRC_parPt_ptDbo (GR_selNam, &pt1, GR_selTyp, GR_selDbi);
    irc = SRC_src_pt_dbo (GR_selNam, sizeof(GR_selNam), Typ_PT,
                          &GR_selPos__, GR_selTyp, GR_selDbi);
    GR_selDbi = 0L;             // create new obj
    GR_selTyp = Typ_PT;         // output - exact.
    if(irc < 0) {
      printf(" cannot (yet) create parametric point from obj ..\n");
      GR_selTyp = Typ_Vertex;
      // goto L_point_GL;
    }
      // printf("  GR_selNam=|%s| GR_selTyp=%d GR_selDbi=%ld\n",
             // GR_selNam,GR_selTyp,GR_selDbi);

    return 0;


    //----------------------------------------------------------------
    // get selected point on object from GL
    L_point_GL:
      // get selected Mousepos in userCoords
      UTX_Clear (GR_selNam);
      AP_obj_add_pt_sp (GR_selNam, &pt1);
      GR_selTyp = Typ_TmpPT;
      GR_selDbi = 0L;
      return 0;



  //================================================================
  // convert obj -> L()
  //================================================================
  L_ln_conv:

    if(GR_selTmpStat == 1) {
      char    s1[128];
      // line from indicate + direction
      // create vector ..
      pt1 = GR_selPos__;
      typBas = AP_typDB_typ (GR_selTyp);

      if(typBas == Typ_TmpPT)  {
        UI_GR_get_actPosA  (&pt1); // get current curPos
        UTX_Clear (s1);
        AP_obj_add_pt_sp (s1, &pt1);

      } else if((typBas == Typ_PT) ||
                (typBas == Typ_VC))    {
        strcpy(s1, GR_selNam);

      } else {
        // get vc from db-obj (curve)
        // irc = SRC_vc_ptDbo (s1, &pt1, typBas, GR_selDbi);
        irc = SRC_src_pt_dbo (GR_selNam, sizeof(GR_selNam), Typ_VC,
                       &GR_selPos__, typBas, GR_selDbi);
        if(irc < 0) { 
          printf(" cannot (yet) create vector from obj ..\n");
          goto L_null_obj;
        }
      }
        printf("  vector s1=|%s|\n",s1);
      GR_selNam[0]='\0';
      strcat (GR_selNam, s1);
        printf("  GR_selNam out |%s|\n",GR_selNam);
      goto L_exit;
    }


    if(GR_selTyp == Typ_PLN) {
      // i1 = GR_selTyp;
      // d1 = GR_selDbi;
      // SRC_fmt__ (GR_selNam, Typ_LN, 1, &i1, &d1);
      ATO_getSpc_tmp__ (&ato, 6);
      ATO_ato_expr_add (&ato, GR_selTyp, (double)GR_selDbi, 0);
      SRC_src_ato (GR_selNam, sizeof(GR_selNam), Typ_LN, &ato);
      GR_selTyp = Typ_LN;
      GR_selDbi = 0L;
      return 0;
    }

    // ii = 1;  // 1=line only
    ii = Typ_LN;  // 1=line only
    goto L_LnAc_1;


  //================================================================
  // convert obj -> L() or C()
  //================================================================
  L_LnAc_conv:
    // creat parametric sourceCode for LN or CI, what is selected.
    // ii = 2;    // 2=line or circ
    ii = Typ_goGeo1;   // 2=line or circ

  L_LnAc_1:    // 1=L, 2=L|C, 3=C,
    // pt1 = GR_selPos__;
    // typBas = AP_typDB_typ (GR_selTyp);
    // irc = SRC_LnAc_ptDbo (ii, GR_selNam, &pt1, typBas, GR_selDbi);
    irc = SRC_src_pt_dbo (GR_selNam, sizeof(GR_selNam), ii,
                          &GR_selPos__, GR_selTyp, GR_selDbi);
    if(irc < 0) {
      printf(" cannot (yet) create line or circ from obj ..\n");
      goto L_null_obj;
    }
    GR_selTyp = irc;         // output - line or circ
    return 0;



  //================================================================
  // convert obj -> C()
  //================================================================
  L_ac_conv:
    if(GR_selTmpStat == 0) {
      if(GR_selTyp == Typ_TmpPT) {
        GR_selNam[0]='\0';
        UI_GR_get_actPosA  (&pt1);
        AP_obj_add_pt_sp (GR_selNam, &pt1);
      }
      IE_cad_Inp_disp_pt (&pt1, IE_get_inpInd());
      GR_selTmpStat = 1;
      goto L_exit;

    } else if(GR_selTmpStat == 1) {
      if(GR_selTyp == Typ_TmpPT) {
        GR_selNam[0]='\0';
        UI_GR_get_actPosA  (&pt1);
        AP_obj_add_pt_sp (GR_selNam, &pt1);
      }
      goto L_exit;
    }



  //================================================================
  // convert obj -> VAL()
  //================================================================
  L_val_conv:
    if(GR_selTmpStat == 0) {
      if(GR_selTyp == Typ_TmpPT) {
        GR_selNam[0]='\0';
        UI_GR_get_actPosA  (&pt1);
        AP_obj_add_pt_sp (GR_selNam, &pt1);
      }
      IE_cad_Inp_disp_pt (&pt1, IE_get_inpInd());
      GR_selTmpStat = 1;
      goto L_exit;
  
    } else if(GR_selTmpStat == 1) {
      if(GR_selTyp == Typ_TmpPT) {
        GR_selNam[0]='\0'; 
        UI_GR_get_actPosA  (&pt1);
        AP_obj_add_pt_sp (GR_selNam, &pt1);
      } 
      goto L_exit;
    }
    return 0;



  //================================================================
  // convert obj -> VAL()
  //================================================================
  L_VAL_C:

    sprintf  (GR_selNam, "VAL(C%ld)", GR_selDbi);
    GR_selTyp = Typ_Val;
    GR_selDbi = 0L;
    return 0;



// UNUSED;
  //================================================================
  // convert obj -> R()
  //================================================================
  // L_REF__:
    // sprintf (s1, "R(%s)",GR_selNam);
    // strcpy (GR_selNam, s1);
    // GR_selTyp = Typ_PLN;
    // GR_selDbi = 0L;
    // return 0;



  //================================================================
  L_exit:
      printf("ex sele_decode %d %ld |%s|\n",
             GR_selTyp, GR_selDbi, GR_selNam);
    return 0;


  //================================================================
  L_err_ptBeforeVc:
    TX_Print(".. select point before vector ..");

  L_null_obj:                            // useless ..
    GR_selTyp = Typ_NULL;
    return 0;
}
*/


//================================================================
  int sele_reset () {
//================================================================
// reset selectionFilter.

  // printf("sele_reset \n");

  BitTab_ini0 (reqObjTab);

  GR_reqTyp = Typ_Error;         // requested nothing ..
  GR_NoConstrPln = 0;            // normal, enabled.

  return 0;

}


//================================================================
  int sele_reset_type (int ib) {
//================================================================

  // printf("sele_reset_type %d\n",ib);

  BitTab_clr (reqObjTab, ib);

  return 0;

}

 
//================================================================
  int sele_set_types (int i0, ...) {
//================================================================
/// \code
/// sele_set_types     add additional objecttypes to selectionfilter
/// LAST ARGUMENT MUST BE 0 !
/// calls BitTab_set with each type ..
/// Example:
///   sele_set__ (Typ_APPOBJ);       // init 
///   sele_set_types (Typ_PLN, 0);   // add CAD-planes
///   sele_setNoConstrPln ();        // allow selection of point on ConstrPln
//
/// \endcode


  int     i1;
  va_list va;

  // printf("sele_set_types %d\n",i0);

  va_start (va, i0);
  goto L_work;

  L_nxt_arg:
    i0 = va_arg (va, int);
      // printf("       _stat_set__ %d\n",i1);
    if(i0 == 0) {
      va_end (va);
      return 0;
    }

    L_work:
      if(i0 > TYP_SIZ) {TX_Error("sele_set_types E001 %d",i0);}
      else BitTab_set (reqObjTab, i0);    // WORK
        // printf(":::::::: BitTab_set %d\n",i0);
      goto L_nxt_arg;

}


//================================================================
  int sele_set__ (int rTyp) {
//================================================================
/// \code
/// init and set selectionFilter 
///
/// Input:
///   rTyp = GR_reqTyp = requestedTyp;  see ../xa/xa_sele.h
///
/// Example:
///   sele_set__ (Typ_PT);           // Typ_PT+Typ_SUR+Typ_SOL+Typ_Model
///   sele_set_types (Typ_PLN, 0);   // add CAD-planes
/// 
/// Example without sele_set__:
///   sele_reset ();
///   sele_set_types (Typ_TmpPT, 0); // only indicate on active constrPln
///
/// set reqObjTab = selectableTypes
///     reqObjTab = all types that can be converted into rTyp
/// \endcode

// sele_set__ defines which objects subsequent can be selected;
// sele_decode converts the selected obj into a requested obj
// 2015-09-02 sele_set_add extracted. RF


  // printf("sele_set__ %d\n",rTyp);


  GR_NoConstrPln = 0;       // 0=enabled;  1=disabled; reset.

  GR_reqTyp = rTyp;         // primary requested typ

  GR_selTmpStat = 0;        // reset temp-indicate.         UNUSED ..

  GLBT_but_ini ();          // I2D_iNr = 0;



  // reset = clear selectionFilter  (see also sele_reset)
  memset (reqObjTab, 0x00, sizeof(reqObjTab));

  sele_set_add (rTyp);

  return 0;

}


//================================================================
  int sele_set_add (int rTyp) {
//================================================================
/// \code
/// sele_set_add        add obj to selectionfilter; eg Typ_go_PD = P+D
/// eg Typ_go_LCS = LN/CI/CV
/// GR_reqTyp is not set !
// - transformation into requested obj is done in func sele_decode
//
// selectionfilter = reqObjTab                  see INF_workflow_select
// test selectionfilter: if(sele_ck_typ (typ)) - yes, is on
/// \endcode
 
  int     i1, i2, i2Dbutts = 0;


  // printf("sele_set_add %d\n",rTyp);


  switch (rTyp) {
   
    case Typ_Val:
      sele_set_types (Typ_VAR, 
                      Typ_Val,
                      0);
                      // Typ_PT,               // dist (P P)
                      // Typ_LN,               // Length, dist (P L)
                      // Typ_CI,               // Radius 
      sele_set_icon (&i2Dbutts, Typ_VAR);   // V+ V-
      break;


    case Typ_XVal:
    case Typ_YVal:
    case Typ_ZVal:
      sele_set_types (Typ_VAR, 
                      Typ_Val,
                      0);
                      // Typ_PT,
                      // Typ_LN,
      sele_set_icon (&i2Dbutts, Typ_VAR);   // V+ V-
      break;


    case Typ_Angle:   // VC|LN|CV|Var|Val    for angle
      sele_set_types (Typ_VAR, 
                      Typ_Val,
                      Typ_PT,
                      Typ_VC,
                      Typ_LN,
                      Typ_CI,
                      Typ_CVPOL,
                      Typ_CVTRM,
                      0);
      sele_set_icon (&i2Dbutts, Typ_modREV);
      sele_set_icon (&i2Dbutts, Typ_VAR);         // V+ V-
      sele_set_icon (&i2Dbutts, Typ_VC);          // VC+ VC-
      break;


    case Typ_VC:
      sele_set_types (Typ_VC, 
                      0);
                      // Typ_PT,    // 2016-05-16; for PT-PT, 2 indicates
                      // Typ_LN,                   // 2017-03-02
                      // Typ_PLN,                  // 2015-07-06
                      // Typ_SUR,
                      // Typ_Model,
                      // Typ_modREV,
      sele_set_icon (&i2Dbutts, Typ_modREV);
      sele_set_icon (&i2Dbutts, Typ_VC);          // VC+ VC-
      break;

    case Typ_PT:
      sele_set_types (Typ_PT, 
                      0);
  // Typ_LN-Typ_CVTRM needed for eg 'PT cartes' - BasePoint 2017-04-06
                      // Typ_LN,
                      // Typ_CI,
                      // Typ_CVBSP,
                      // Typ_CVRBSP,              // 2014-10-17
                      // Typ_CVLNA,               // 2011-09-07
                      // Typ_CVELL,
                      // Typ_CVCLOT,
                      // Typ_CVPOL,               // 2014-12-19
                      // Typ_CVTRM,               // 2014-12-19
                      // Typ_PLN,                 // 2016-10-04
                      // Typ_Model,
                      // Typ_SUR,
                      // Typ_SOL,
      break;

    case Typ_LN:
      sele_set_types (Typ_LN,
                      0);
                      // Typ_PT,                  // 2011-09-29
                      // Typ_VC,
                      // Typ_CVLNA,               // 2011-09-08
                      // // Typ_CVPOL, 
                      // Typ_CVTRM,
                      // Typ_PLN,                 // 2011-09-08
      sele_set_icon (&i2Dbutts, Typ_VC);          // VC+ VC-
      sele_set_icon (&i2Dbutts, Typ_FncPtOnObj);
      break;


    case Typ_CI:
      sele_set_types (Typ_CI,
                      0);
                      // Typ_PT,                  // 2011-12-05
                      // Typ_Val,                 // 2011-12-05
                      // Typ_VAR,                 // 2011-12-05
                      // Typ_CVTRM,
      break;


    case Typ_PLN:
      sele_set_types (Typ_PLN, 
                      Typ_PT,
                      Typ_VC,
                      Typ_LN, 
                      Typ_CI,
                      Typ_Model,
                      0);
/*
                      Typ_Val,
*/
      sele_set_icon (&i2Dbutts, Typ_VC);          // VC+ VC-
      sele_set_icon (&i2Dbutts, Typ_modPERP);
      break;

    case Typ_SubModel:
      sele_set_types (Typ_SubModel, 
                      Typ_Mock,
                      Typ_Model,
                      0);
      sele_set_icon (&i2Dbutts, GR_reqTyp);
      break;


    case Typ_goGeom:    // all types
    case Typ_Group:     // all types
      memset (reqObjTab, 0xff, sizeof(reqObjTab));
      // but not modifiers !
      BitTab_clr (reqObjTab, Typ_modREV);
      BitTab_clr (reqObjTab, Typ_modCWCCW);
      BitTab_clr (reqObjTab, Typ_modCTRL);
      BitTab_clr (reqObjTab, Typ_modCX);
      if(IE_get_Func() >= 0)
      sele_set_icon (&i2Dbutts, Typ_VC);          // VC+ VC-
      break;

    case Typ_goPrim:      // PT|LN|AC|CV
      sele_set_types (Typ_PT, 
                      Typ_LN,
                      Typ_CI, 
                      Typ_CV,
                      Typ_CVBSP,
                      Typ_CVRBSP,
                      Typ_CVPOL,               // 2011-08-05 was Typ_CV
                      Typ_CVELL,
                      Typ_CVCLOT,
                      0);
      break;

    case Typ_go_LCS:      // LN/CI/CV
    case Typ_go_lf1:      // L|C|curve, but only single curve, not contour
      sele_set_types (Typ_LN, 
                      Typ_CI, 
                      Typ_CV,
                      Typ_CVBSP,
                      Typ_CVRBSP,
                      Typ_CVPOL,
                      Typ_CVELL,
                      Typ_CVCLOT,
                      Typ_CVTRM,
                      0);
      break;


    case Typ_go_lf2:      // L|C|curve, but no CCV (trimmed-curve)
      sele_set_types (Typ_LN,
                      Typ_CI,
                      Typ_CV,
                      Typ_CVBSP,
                      Typ_CVRBSP,
                      Typ_CVPOL,
                      Typ_CVELL,
                      Typ_CVCLOT,
                      0);
      break;


    case Typ_goGeo1:    // /LN/CI/CV/PLN/SUR/SOL/            NOT PT
      sele_set_types (Typ_LN, 
                      Typ_CI,
                      Typ_CVBSP,
                      Typ_CVRBSP,
                      Typ_CVPOL,               // 2011-08-05 was Typ_CV
                      Typ_CVELL,
                      Typ_CVCLOT,
                      Typ_CVTRM,
                      Typ_PLN,
                      Typ_SUR,
                      Typ_SOL,
                      0);
      break;

    case Typ_goGeo2:    // /CI/CV-ELL/
      sele_set_types (Typ_CI, 
                      Typ_CVELL,
                      0);
      break;

    case Typ_go_PD:    // PT|VC
      sele_set_types (Typ_PT, 
                      Typ_VC,
                      0);
      sele_set_icon (&i2Dbutts, Typ_VC);          // VC+ VC-
      break;

/*
    case Typ_goGeo4:    // VC|LN|Pln
      sele_set_types (Typ_VC, 
                      Typ_PT,                  // 2011-09-29
                      Typ_CVLNA,               // 2011-09-29
                      Typ_CVPOL,
                      Typ_CVTRM,
                      Typ_LN,
                      Typ_PLN,
                      0);
      sele_set_icon (&i2Dbutts, Typ_VC);          // VC+ VC-
      break;
*/
    case Typ_goGeo5:    // AC|CV-Ell|CV-Plg|CV_CCV
      sele_set_types (Typ_CI, 
                      Typ_CVPOL,               // 2011-08-05 was Typ_CV
                      Typ_CVBSP,
                      Typ_CVELL,
                      Typ_CVTRM,
                      0);
      break;


    case Typ_goGeo6:    // PT|LN|AC direkt;
      sele_set_types (Typ_PT,
                      Typ_LN,
                      Typ_CI,
                      Typ_CVPOL,
                      Typ_CVTRM,
                      0);
      break;


    case Typ_goGeoSUSU: // Sur|Sol
      sele_set_types (Typ_SUR, 
                      Typ_SOL, 
                      0);
      break;


    case Typ_goGeo7:    // Val|PT|VC
      sele_set_types (Typ_VAR, 
                      Typ_Val,
                      Typ_PT,
                      Typ_VC,
                      Typ_LN,
                      Typ_CVPOL, 
                      Typ_CVTRM,
                      Typ_modREV,
                      0);
      sele_set_icon (&i2Dbutts, Typ_modREV);
      sele_set_icon (&i2Dbutts, Typ_VAR);         // V+ V-
      sele_set_icon (&i2Dbutts, Typ_VC);          // VC+ VC-
      break;


    case Typ_goGeo8:    // Val|PT
      sele_set_types (Typ_VAR,          // VAL(V)
                      Typ_Val,          // direct
                      Typ_PT,           // direct
                      Typ_LN,           // P(L par)
                      Typ_CI,
                      0);
      sele_set_icon (&i2Dbutts, Typ_VAR);   // V+ V-
      break;


    case Typ_go_LR:    // Line,Plane
      sele_set_types (Typ_PLN,
                      Typ_LN,
                      0);
      break;


    case Typ_go_RA:    // Plane,surface
      sele_set_types (Typ_PLN,
                      Typ_SUR,
                      0);
      break;


    case Typ_goAxis:    // LN|PT+PT|PT+LN|PT+VC|PLN   (RotAxis)
      sele_set_types (Typ_VC, 
                      Typ_PT,
                      Typ_LN,
                      Typ_PLN, 
                      0);
      sele_set_icon (&i2Dbutts, Typ_VC);          // VC+ VC-
      break;


    case Typ_FncDirX:    // "PARL"|"CX"       parallel (U) or across (V)
      sele_set_types (Typ_modPARL,
                      Typ_modCX,
                      0);
      sele_set_icon (&i2Dbutts, Typ_FncDirX);          // "PARL"|"CX"
      break;



/*
    case Typ_goRadius:  // VAR|Val|PT|AC              (Radius)
      sele_set_types (Typ_VAR, 
                      Typ_Val,
                      Typ_PT, 
                      Typ_CI,
                      0);
      break;
*/



    case TYP_FilNam:            // Filename
      sele_set_types (TYP_FilNam, Typ_Model, 0);
      sele_set_icon (&i2Dbutts, GR_reqTyp);
      break;


    case Typ_CtlgPart:            // Filename
      sele_set_types (Typ_CtlgPart, Typ_Model, 0);
      sele_set_icon (&i2Dbutts, GR_reqTyp);
      break;


    case Typ_modREV:
      sele_set_types (GR_reqTyp, 0);
      sele_set_icon (&i2Dbutts, GR_reqTyp);
      break;


    case Typ_modCTRL:
      sele_set_types (GR_reqTyp, 0);
      sele_set_icon (&i2Dbutts, Typ_modCTRL);
      break;


    case Typ_modCX:
      sele_set_types (GR_reqTyp, 0);
      sele_set_icon (&i2Dbutts, Typ_modCX);
      break;


    case Typ_modCWCCW:
      sele_set_types (GR_reqTyp, 0);
      sele_set_icon (&i2Dbutts, Typ_modCWCCW);
      break;


    case Typ_mod1:
      sele_set_types (GR_reqTyp, 0);
      sele_set_icon (&i2Dbutts, Typ_mod1);  // NXT PRV
      break;


    case Typ_modPERP:
      sele_set_types (GR_reqTyp, 0);
      sele_set_icon (&i2Dbutts, Typ_modPERP);
      break;


    case Typ_go_JNT:            // Joint:
      sele_set_types (Typ_PT,
                      Typ_PLN,
                      // Typ_GTXT,
                      0);


    case Typ_EyePT:  // 
      sele_set_types (Typ_PT,
                      0);
      break;



    default:            // a single type
      // set requested selectiontyp = rTyp
      if(rTyp > TYP_SIZ) {
        TX_Error("sele_set__ E001 %d",rTyp);

      } else {
          // printf(" BitTab_set reqObjTab %d\n",rTyp);
        BitTab_set (reqObjTab, rTyp);
      }
  }


  //----------------------------------------------------------------
  // activate/deActivate 2D-butts
  if(UI_InpMode == UI_MODE_CAD) {
    // activate VectorSelector
    i1 = -1;  // not active
    // activate PlaneSelector
    i2 = -1;  // not active
    if(IE_get_Func() >= 0) {          // only if a cad-function is active
        // printf(" IE_cad_typ=%d\n",IE_get_Func());
      if(BitTab_get(reqObjTab, Typ_VC)  != 0) i1 = 0;
      // -1 = display defaultVectors, but no DB-vector.
      if(BitTab_get(reqObjTab, Typ_PLN) != 0) i2 = 0;
    }
    GLBT_vcSel_set (i1, 1);        // activate VectorSelector, Redraw
    GLBT_plnSel_set (i2, 1);       // activate PlaneSelector, Redraw
  }


  // only if CAD is active:
  if(IE_get_Func() >= 0) {
    // set status of 2D-icons
    GLBT_set_stat (IE_get_inpInd ());
  }

  if(UI_InpMode == UI_MODE_CAD)   // 2013-05-06
    DL_Redraw ();                 // necessary for remove tag ..

  return 0;

}


//================================================================
  int sele_setNoConstrPln () {
//================================================================
// used in sele_ck_ConstrPln
// 0 = normal, add "ConstrPln" to selelection-menu.
// 1 = do NOT add "ConstrPln" to selelection-menu
 
  GR_NoConstrPln = 1;            // 0=enabled;  1=disabled !

  return 0;

}


//================================================================
  int sele_save () {
//================================================================
// save active reqObjTab & GR_NoConstrPln
// TODO: make stack of 4 ..

  // BitTab_test1 (reqObjTab);
  
  // copy reqObjTab
  BitTab_cpy (bck_ObjTab, reqObjTab);
  bck_GR_NoConstrPln = GR_NoConstrPln;
  

  return 0;

}


//================================================================
  int sele_restore () {
//================================================================
// restore saved reqObjTab & GR_NoConstrPln
 
  // copy reqObjTab back
  BitTab_cpy (reqObjTab, bck_ObjTab);
  GR_NoConstrPln = bck_GR_NoConstrPln;
  
  return 0;

} 


// EOF
