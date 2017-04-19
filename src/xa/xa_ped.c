// xa_ped.c                                      2009-11-20  RF
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
  L20=P(-641.99 -498.59 0) P(-214.33 2.82 0)
  L21=P(L20 MOD(2)) P(421.56 -108.68 0)
  L20 MOD(2) ist kein verschiebbarer Punkt !!!!!!!!
  see APT_decode__ und "PEDlink",
    ..

-----------------------------------------------------
Modifications:
2009-11-20 neu erstellt. RF.

-----------------------------------------------------
*/
#ifdef globTag
 void PED(){}
#endif
/*!
\file  ../xa/xa_ped.c
\brief PointEditor 
\code
=====================================================
List_functions_start:

PED_init         decode obj; init toolbar
PED_win__        create Toolbar
PED_win_act      activate/deactivate Insert/Delete
PED_lock         lock mainMenu; unlock Save|cancel
PED_unlock       unlock mainMenu; lock Save|cancel
PED_CB1          callbacks
PED_points       temppoint-functions
PED_save__       save (modify src & write -> tempfile)
PED_exit         insert new code, redraw all
PED_restore1     overwrite DB-points UNUSED
PED_restore2     get sourceline, execute .. UNUSED
PED_restore3     update model starting with line lNr
PED_src_mod      compare all points; modify points
PED_src_del      delete point actInd in srcLine
PED_src_ins      insert point in srcLine
PED_compare      compare points - create new one
PED_replace      replace or skip point in ObjSRC  UNUSED
PED_get_ato      get & decode the sourceObj
PED_get_src      get the sourceObj from tempfile
PED_get_ptab     get all points of actObj into actPta
PED_get_tabInd   get index into PED_atab from pointNr actInd
PED_wri_src      save sourceLine -> tempfile
PED_rea_src      read file into memstring

PED_get_offObj   get offsetObject (Plane or Vector) out of PED_w_off
PED_del__        delete point actInd
PED_ins_pt       get segmentnr where to insert point & save point

PED_key_CB
PED_sel_CB       userselection callback
PED_mousemove_CB callback mousemovement
PED_off_CB       cb of entry PED_w_off
PED_len_CB       cb of entry PED_offLen
PED_upd_stop     exit update proc
PED_newPos__     project cursorPosition onto offsetvector

PED_dump_ptab
PED_dump_atab

List_functions_end:
=====================================================
see also
UI_WinTra__       alter modelPositionEditor

\endcode *//*----------------------------------------



----------------------------------
PED - Pointeditor:
Is using Selection-callback & Mousemove-callback (like plugins !)

Startup:   PED_init
Shutdown:  PED_key_CB | PED_sel_CB > PED_CB1


Input: object with points to be modified.
- get sourceline, copy it to file ../tmp/selection.txt
- decode sourceline into atomicObj, keep im memory in PED_atyp,PED_atab[PED_anr].
- get all points, keep in memory in actPta.
- display points as yellow start

Modify, Delete:
- user selects point; set actPtp = pointer into DB for this point
- modify this point, redraw obj with PED_points(6);
  - APT_store_obj, APT_Draw__.
- save: modify the sourceline (in tempfile)

Insert:
- user selects positon on curve; get parametric description of point,
  get segment on curve from its parameter, insert new point.


PED_init
  APED_find_dep__
  PED_dep_sav
  PED_get_ptab
    PED_get_ato
      PED_get_src
      APT_decode__


PED_mousemove_CB
  PED_points (6, 9);
    APT_Draw__
    PED_mod_dep_draw__


*/

#ifdef _MSC_VER
#include "MS_Def0.h"
#endif


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // va_list
#include <ctype.h>                     // isdigit



#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_txfil.h"            // UTF_GetnextLnPos
#include "../ut/ut_txTab.h"            // TxtTab
#include "../ut/ut_memTab.h"           // MemTab
#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_os.h"               // OS_ ..

#include "../gui/gui__.h"              // Gtk3

#include "../db/ut_DB.h"               // DB_

#include "../ut/func_types.h"               // Typ_Att_hili
#include "../gr/ut_GL.h"               // GL_get_Scale
#include "../xa/xa_ed_mem.h"           // ObjSRC
#include "../xa/xa_mem.h"              // memspc51, mem_cbuf1
#include "../xa/xa_ui.h"               // UI_MODE_CAD APF_TB_CAD,
#include "../xa/xa_msg.h"              // MSG_..
#include "../xa/xa_sele.h"             // Typ_go*


#define   TRUE 1
#define   FALSE 0


#define PED_SYM_COL   5                // color for pointSymbols (star) GL_col_tab
#define PED_SYM_ID    SYM_TRI_B        // ID for pointSymbols



//================================================================
// ex ../xa/xa.c
extern int       WC_sur_ind;            // Index auf die ActiveConstrPlane
extern Plane     WC_sur_act;            // Constr.Plane
extern Mat_4x3   WC_sur_mat;            // TrMat of ActiveConstrPlane
extern Mat_4x3   WC_sur_imat;           // inverse TrMat of ActiveConstrPlane


// ex ../xa/xa_ui.c
extern MemObj    UIw_Box_TB;    // toolbarBox


// External from ../ci/NC_Main.c:
extern char       *APT_defTxt;          // die Textzeile ohne N#=

// // ex ../xa/xa_ui_gr.c
// extern int KeyM1;


//================================================================
// LOCAL:
static MemObj PEDtb=GUI_OBJ_NEW;  // Toolbar

static int   actTyp;            // typ of curve to modify
static long  actDbi;            // DB-index
static void  *actObj;           // data-struct of curve to modify
static Point *actPta=NULL;      // pointarray
static Point *actPtp;           // addres of active point in DB
static Point actCurPos;         // 
static Point actInsPt;         // 
static long  actPti;            // index of active point in DB / PED_atab
static int   actPtn;            // nr of points of actPta
static int   actInd;            // point to modify; index into actPta
static long  actDli;            // dispListIndex
static long  dynPti;

static int    *PED_atyp;        // type of objects of sourcecode to edit ..
static double *PED_atab;        // data of objects of sourcecode to edit ..
static int    PED_anr = 0;      // nr of objects in PED_atyp,PED_atab

static int    PED_mode=-1;      // 0=modify, 1=insert, 2=delete
static int    PED_stat=0;       // 0=move is not active; 1=active;
                                // 2=SaveCancel is active.
static int    PED_changed=0;

static Plane  PED_offObj;       // vector or plane
static int    PED_offTyp;
static double PED_offLen = 0.;

static MemObj PED_w_off;    // entry offsetObj
static MemObj PED_w_len;    // entry length
static MemObj PED_f_mod;    // frame Modify
static MemObj PED_f0;       // primary frame
static MemObj PED_f_sc;     // frams Save/Cancel
static MemObj PED_wa[4];    // Radiobutts 0=Delete 1=Insert; 2=Modify; 3=Exit

static MemTab(ObjSRC) depTab = MemTab_empty;

static FILE      *PED_fp_dep = NULL;

// PROTOS:
  MemObj PED_win__ (MemObj *parent, void *data);
  int PED_CB1 (MemObj *mo, void **data);
  int PED_off_CB (MemObj *mo, void **data);
  int PED_len_CB (MemObj *mo, void **data);
  int PED_mousemove_CB (int dx, int dy);
  int PED_key_CB (int key);
  int PED_sel_CB (int src, long dl_ind);
  int PED_link_CB (MemObj *mo, void **data);
  int PED_get_src (char *cBuf, int bufSiz);
  Point PED_newPos__ ();




//================================================================
  int PED_init (int typ, long dbi, long dli) {
//================================================================
// main-entrypoint 'move_points_of_obj'
 
  int   irc, i1, form, lNr, parTyp;
  long  l1, l2, icp, parDbi;
  char  cbuf[160];


  printf("PED_init typ=%d dbi=%ld dli=%ld mode=%d\n",typ,dbi,dli,PED_mode);


  // exit if PED is already active
  // man kann PED_mode in einer DLL nicht korrekt abfragen,
  // da es in der Dll eine lokale Kopie gibt.
  if(PED_mode >= 0) {
    MSG_pri_0 ("PEDe1"); // ERROR: PED is already active ..");
    return 0;
  }


  
  // if MAN: copy editor -> memory
  AP_SRC_edi_mem ();



  //----------------------------------------------------------------
  // test if inputObject has a parent
  //  but if inputObject has more than 1 parents ?
  if(DL_parent_ck_c (dli)) {
      printf(" .. has parent !\n");
    // get parentObj
// TODO: use AP_parent_get; can provide more than 1 parent; select parent ?
    irc = CVTRM_parent_ccv (&parTyp, &parDbi, typ, dbi);
    // irc = GA_parent_get (&parTyp, &parDbi, typ, dbi);
    // irc = GA_parent_get (&dbi, typ, dbi);
    if(irc < 0) { printf("parentError E001 %ld\n",dli); return -1; }
      printf(" parent-dbi=%ld\n",dbi);
    // get dli for dbo
    dli = DL_find_obj (parTyp, parDbi, dli);
    if(dli < 0) { printf("parentError E002 %ld\n",dbi); return -1; }
    TX_Print("***** using parent of selected object ..");
    dbi = parDbi;
    typ = parTyp;
  }



  // get actObj from DB -------------------------------------
  form = typ;
  irc = UTO_get_DB (&actObj, &i1, &form, dbi);
  if(irc < 0) return irc;

  actDli = dli;
  actDbi = dbi;
  actTyp = form;
    // printf(" form=%d\n",form);
    // UT3D_stru_dump (actTyp, actObj, "actObj: ");



  // get srcLineNr
  DL_Get_lNr_dli (&lNr, actDli);
  // save active line into tempFile
  sprintf(cbuf,"%sselection.txt",OS_get_tmp_dir());
  UTF_wrf_line (lNr, cbuf);


  // activate ConstrPlaneNr for this object (resets automatic)
  DL_setRefSys (actDli);


  // find all depending objects
  APED_find_dep__ (&depTab, actTyp, actDbi);



  // write sourcecodes of all depending objects -> file src_dep.txt
  irc = PED_dep_sav ();
  if(irc < 0) return irc;


  // get all points of actObj into actPta[actPtn]
  // get all atomicObjs into PED_atyp,PED_atab[PED_anr]
  irc = PED_get_ptab ();  // return nr of points
  if((irc < 0)||(actPtn < 1)) {MSG_pri_0 ("PEDnopt"); return -1;}
  MSG_pri_1 ("PEDnrpt", "%d", actPtn);  // Objekt hat # verschiebbare Punkte


  // lock some application-functions...
  UI_func_stat_set__ (-APF_TB_CAD,
                      -APF_MEN_FIL,
                      -APF_MEN_SM,
                      -APF_MEN_MDF,
                      -APF_MEN_APP,
                      // -APF_WIN_B_E,
                      -APF_HIDE,  
                      -APF_VWCADMAN,
                      -APF_BUT_GSE,
                      -APF_PLUGINS,
                      -APF_UNDO,
                      0);


  AP_UserKeyIn_get     (PED_key_CB);         // attach keystrokes
  AP_UserSelection_get (PED_sel_CB);         // attach grafic selections
  AP_UserMousemove_get (PED_mousemove_CB);   // attach mousemovements
  UI_block_group (1);                        // block groupSelections




  // init PED-toolbar
  if(GUI_OBJ_IS_VALID(&PEDtb)) {
    // GUI_obj_view (PEDtb);
    GUI_set_show (&PEDtb, 1);

  } else {
    PEDtb = PED_win__ (&UIw_Box_TB, NULL);
  }


  // activate/deactivate Insert/Delete according to type of sel. obj
  PED_win_act ();


  // init pointEditor, display points
  // DL_pick__ (0);            // make all objects unpickable
  PED_points (1, 0);        // draw points

  sele_reset ();
  sele_set_types (Typ_SymB,
                  Typ_VC,
                  Typ_PLN,
                  0);       // make selectable ...
  GL_vcSel_init (-1, 1);    // enable VectorSelector



  PED_get_offObj ();        // get offsetObject (Plane or Vector)
  PED_mousemove_CB (0,0);   // redraw modify-symbol

  PED_mode = 0;

  PED_msg__ ();


// autoselect ...  geht ned ...
  // DL_DumpObjTab ();
  if(actPtn < 2) {
    // PED_get_offObj ();
    // l1 = DL_get__ (NULL) - 1;
    // UI_GR_set_sel (l1);
    l1 = GL_GetActInd ();
    l2 = DL_get_dbi (l1);                     // get dl.ind
    UI_GR_set_selNam (Typ_SymB, l2, "");      // simulate selection
    PED_sel_CB (GUI_MouseL, l1);
    GUI_update__ ();
    DL_Redraw ();
  }



  // printf("ex PED_init mode=%d\n",PED_mode);

  return 0;

}


//================================================================
  int PED_key_CB (int key) {
//================================================================

  // printf("PED_key_CB %d\n",key);
  // printf("  PED_stat=%d PED_mode=%d actInd=%d\n",PED_stat,PED_mode,actInd);


  switch (key) {

    case GUI_KeyEsc:
    case 'q':
    case 's':
        // DL_DumpObjTab ();
      // printf(" Esc: md=%d stat=%d\n",PED_mode,PED_stat);

      if(PED_stat == 0) {           // Save|Cancel is active
        PED_CB1 (NULL, GUI_SETDAT_ES(TYP_EventPress,"Exit"));
      }

      if(PED_stat == 1) {         // modify, updating
        if(PED_mode == 0) {       // modify
          PED_upd_stop ();        // stop modify-update
        }
      }

      if(PED_stat == 2) {           // Save|Cancel is active
        if(key == 's') PED_CB1 (NULL, GUI_SETDAT_ES(TYP_EventPress,"Save"));
        else           PED_CB1 (NULL, GUI_SETDAT_ES(TYP_EventPress,"Canc"));
      }

      PED_msg__ ();

      return TRUE;  // do no defaultOperations


    default:
      break;
  }

    // printf(" exit PED_key_CB\n");

  return FALSE;  // do defaultOperations

}

 
//=========================================================
  int PED_sel_CB (int src, long dl_ind) {
//=========================================================
// userselection callback

  int    irc, styp, typ;
  long   dbi;
  char   *cp1;
  Point  pt1;



  // printf("PED_sel_CB src=%d ind=%ld\n",src,dl_ind);
  // printf("  PED_mode=%d, PED_stat=%d\n",PED_mode,PED_stat);
  // printf("  actTyp=%d actDbi=%ld actInd=%d\n",actTyp,actDbi,actInd);
  // printf("  PED_changed=%d\n",PED_changed);
  // printf("  PED_offTyp=%d\n",PED_offTyp);


  // if(iAct) return 0;
  // iAct = 1;



  // save &exit m. rechter maustaste (see also PED_key_CB)
  if(src == GUI_MouseR) {
    // if(dl_ind >= 0) return 0;     // skip obj sel with right mousebutt.
    PED_key_CB ('s');             // exit ..
    return 0;
  }


  

  // query selected DB-obj
  DBO_sel__ (&styp, &dbi, dl_ind);
    // printf(" sel styp=%d dbi=%ld\n",styp,dbi);


  // analyze curvepoint - get text
  UI_GR_get_selNam (&typ, &dbi, &cp1);
    // printf(" %d %ld selNam |%s|\n",typ, dbi,cp1);




  //----------------------------------------------------------------
  if(PED_mode == 1) {  // insert

    // filter=18 provides a point
    // test if curve was selected
    // if((styp != AP_typ_2_bastyp (actTyp))||(dbi != actDbi)) {
    if((styp != actTyp)||(dbi != actDbi)) {
      MSG_pri_0 ("PEDine");
      // PED_msg__ ();
      // UI_GR_Sel_Filter(18);          // restart insert
      return 0;
    }

    // get pointcoords
    // if(typ != Typ_PT) {TX_Print("PED_sel_CB E001"); return -1;}
    // APT_obj_expr (&pt1, typ, cp1);  // get mousepos -> pt1
    // UI_GR_get_actPosA (&pt1);
    sele_get_pos (&pt1);
    PED_ins_pt (&pt1);
    return 0;
  }


  //----------------------------------------------------------------
  if(PED_mode == 0) {           // modify
    if(PED_stat == 1) {         // updating
      PED_key_CB ('s');             // exit ..
      // PED_upd_stop ();          // stop modify-update
      // DL_Redraw ();             // only necessary for M1 ..
      // iAct = 0;
      return 0;
    }
  }


  //----------------------------------------------------------------
  // start move-update-cycle (yellow-star-symbol selected):
  if(typ == Typ_SymB) {     // curvepoint selected

    // skip selection in Save/Cancel-state
    if(PED_stat == 2) {
      if(PED_mode != 0) {
        MSG_pri_0 ("PEDsav");
        return 0;
      }
    }

    if(PED_mode == 0) {   // modify
      if(PED_offTyp == Typ_Error) {
        MSG_pri_0 ("PEDe2");
        return 0;
      }
    }

    PED_points (5, dbi);      // set pointIndex actInd & actPtp
    if(actInd < 0) return 0;



    if(PED_mode == 0) {       // modify ----------------------------
      // test if PED_offTyp=vektor & length is given
      if(PED_offTyp == Typ_VC) {
        PED_offObj.po = *actPtp;
        if(PED_offLen != 0.) {
          PED_stat = 1; //2;
          PED_mousemove_CB (0,0);   // start - draw curve
          PED_upd_stop ();
          return 0;
        }
      // } else {
        // if(actPtp) UT3D_pl_ptpl (&PED_offObj, actPtp);
      }
        // UT3D_stru_dump(PED_offTyp, &PED_offObj, "PED_offObj:");
      PED_mousemove_CB (0,0);          // start - draw curve
      DL_Redraw ();
      GUI_set_enable (&PED_f0, FALSE);  // disactivate main-functions
      PED_stat = 1;                    // 0=move is not active; 1=active
      // UI_GR_Sel_Filter (1);            // no selections; give position


    } else if(PED_mode == 2) {// delete ----------------------------
        printf(" del pt[%d]\n",actInd);
      PED_del__ ();
      // PED_test__ ();
    }

    return 0;
  }

  // only for modify: accept Plane|Vector -> PED_w_off
  if(PED_mode == 0)      {        // modify
    if((typ == Typ_PLN)||(typ == Typ_VC))    {
      GUI_entry_set (&PED_w_off, cp1);
      PED_off_CB (NULL, NULL);    // update
    } else {
      MSG_pri_0 ("PEDerrSel");
    }
  }


  // obj with wrong type selected ..
  PED_msg__ ();


  return 0;

}


//================================================================
  int PED_msg__ () {
//================================================================

  // printf("PED_msg__ \n");


  if(PED_mode == 0)      {    
    if(AP_typ_2_bastyp (actTyp) == Typ_CV) {
      MSG_pri_0 ("PEDmod1");
    } else {
      MSG_pri_0 ("PEDmod2");
    }
  } else if(PED_mode == 1)   MSG_pri_0 ("PEDins");
  else if(PED_mode == 2)     MSG_pri_0 ("PEDdel");

  return 0;

}


//=========================================================
  int PED_mousemove_CB (int dx, int dy) {
//=========================================================
// callback mousemovement
// modifys the active point (actPta[actInd])

  // int       mode;
  long      dli;
  Point     pt2;


  // printf("------------------------------------------ \n");
  // printf("PED_mousemove_CB %d %d PED_mode=%d\n",dx,dy,PED_mode);


  if(PED_offTyp == Typ_Error) {   // not initialized ..
    // MSG_pri_0 ("PEDe2");
    return 0;
  }


  if(PED_mode == 0) {    // 0=modify
    dli = -4;
    UI_GR_get_actPosA (&actCurPos);   // get GR_CurUk
    UTRA_pt_ucs2wcs (&actCurPos);

    if(PED_offTyp == Typ_VC) {
      if(PED_offLen == 0.) {
        // GL_DrawVc1 (&dli, PED_SYM_COL, &actCurPos, &PED_offObj.vz);
        GR_Draw_vc (&dli, &PED_offObj.vz, &actCurPos, PED_SYM_COL, 0);

      } else {
        GL_DrawVec (&dli, PED_SYM_COL, &actCurPos, &PED_offObj.vx);
      }
    } else {
      GL_DrawSymV3 (&dli, SYM_SQUARE, PED_SYM_COL, &actCurPos, &PED_offObj.vz, 1);
    }
  }


  if(PED_stat != 1) goto L_exit;     // must be 1

  if(actTyp == 0) goto L_exit;

  if(actInd < 0) {PED_msg__ (); goto L_exit;}



  // get the new point
  // Pointaddress can hve changed by realloc !
    // printf(" actPtp=%p\n",actPtp);
  actPtp = DB_get_PT (actPti);
    // UT3D_stru_dump (Typ_PT, actPtp, "  actPtp-1:");
  *actPtp = PED_newPos__ ();
    // UT3D_stru_dump (Typ_PT, actPtp, "  actPtp-2:");


  // redraw red circ at newPos
  if(PED_mode == 0) PED_points (4, 0);


  // redraw actObj hilited
  PED_points (6, 9);


  L_exit:
  DL_Redraw ();

  return 0;

}


//================================================================
  int PED_upd_stop () {
//================================================================
// exit update proc

  printf("PED_upd_stop \n");

  GL_temp_Delete (4L);      // delete temp-vec or plane
  PED_points (3,  0);       // update pointPositions
  MSG_pri_0 ("PEDmod3");
  PED_stat = 2;             // stop update

  GUI_set_enable (&PED_f_sc, TRUE);      // activate Save|cancel

  return 0;

}


//================================================================
  Point PED_newPos__ () {
//================================================================
// project cursorPosition onto offsetvector

  double   d1;
  Point    *pt1, pt2, ptx, pto;
  Vector   *vc1, vc2;


  // printf("PED_newPos__ \n");
  // printf("  PED_offTyp=%d\n",PED_offTyp);
  // UT3D_stru_dump(PED_offTyp, &PED_offObj, "  PED_offObj:");



  // get eye-point & eye-vector
  UI_GR_get_actPosA (&pt2);   // get GR_CurUk
    // UT3D_stru_dump (Typ_PT, &pt2, "  inPos: ");
  vc2 = GL_GetEyeX ();
    // UT3D_stru_dump (Typ_VC, &vc2, "  eyeVec: ");



  //================================================================
  // intersect with vector
  if(PED_offTyp == Typ_VC) {

    if(PED_offLen == 0.) {   // move with cursor
      // intersect eyeLine X offsetLine
      UT3D_pt_int2pt2vc (&pto, &ptx, &d1, &PED_offObj.po, &PED_offObj.vz,
                                          &pt2, &vc2);

    } else {                 // move fixed length
      // UT3D_pt_traptvc (&pto, actPtp, &PED_offObj.vz);
      UT3D_pt_traptvclen (&pto, actPtp, &PED_offObj.vz, PED_offLen);
        // UT3D_stru_dump (Typ_PT, &pto, "  pto: ");
    }


  
  //================================================================
  // intersect with plane ..
  } else {

    if(WC_sur_ind) {
      // a RefSys is active ..
      pto = actCurPos;

    } else {
      // intersect eyeVector - plane                       2010-10-07
      UT3D_pt_intptvcpl_ (&pto, &PED_offObj, &pt2, &vc2);
    }
  }


  // printf("ex PED_newPos__ %f %f %f\n",pto.x,pto.y,pto.z);

  return pto;

}


//================================================================
  int PED_points (int mode, int ii) {
//================================================================
// point-editor: draw points, delete points
// mode=1    draw points (init)
// mode=2    delete points
// mode=3    redraw point with index actInd
// mode=4    create temporary red circ at actPtp
// mode=5    set pointIndex actInd
// mode=6    redraw curve
// mode=7    redraw all temporary points
// mode=8    delete temporary point actInd
// mode=9    restore original points (if Links were broken)


  static long   startInd = -1L;
  int    i1, i2, i3, basTyp;
  long   dli, l1, iatt;
  char   sBuf[200], sNam[32];
  ObjGX  ox1;


  // printf("PED_points %d %d\n",mode,ii);
    // PED_dump_atab ();
    // PED_dump_ptab ();



  //----------------------------------------------------------------
  if(mode != 1) goto L_mode2; // 1 = init; draw points
  startInd = GL_Get_DLind();
  PED_points (7, 0);          // draw all temp. points
  PED_points (6, 9);          // display curve to modify temporary
  actInd = -1;
  // TX_Print("select point (yellow star) to modify ...");
  goto L_exit;


  //----------------------------------------------------------------
  L_mode2:
    if(mode != 2) goto L_mode3;
    // mode=2    delete points
    if(startInd >= 0) {
      // printf(" ... delete von %ld\n",startInd);
      GL_Delete (startInd);
    }
    // delete temporary obj -2 (actObj)
    GL_temp_Delete (2L);
    // delete temporary obj -3 (pointSymbol)
    GL_temp_Delete (3L);
    GL_temp_Delete (4L);       // modify-symbol
    goto L_exit;


  //----------------------------------------------------------------
  L_mode3:
    if(mode != 3) goto L_mode4;
    // mode=3    delete point index actInd
    GL_temp_Delete (3L);  // delete temp circ
    // redraw at new PointPosition
    if(actInd >= 0) {
      dli = startInd + actInd;
      DL_SetInd (dli);               // overwrite this DL-record
      APT_disp_SymB (PED_SYM_ID, PED_SYM_COL, actPtp);  // replace point
    }
    goto L_exit;


  //----------------------------------------------------------------
  L_mode4:
    if(mode != 4) goto L_mode5;
    // mode=4    create temporary pointSymbol -3 at pos actPta[actInd]
    dli = -3;
    // GL_DrawSymB (&dli, 2, SYM_CIR_S, &actPta[actInd]);
    GL_DrawSymB (&dli, PED_SYM_COL, SYM_CIR_S, actPtp);
    goto L_exit;


  //----------------------------------------------------------------
  L_mode5:
    if(mode != 5) goto L_mode6;
    // mode=5    set pointIndex actInd; input: ii=neg.DL-Index of new point
    i2 = -ii - startInd;
    i1 = PED_get_tabInd (i2);
    if(i1 < 0) return -1;
    actPti = PED_atab[i1];                  // get index into PED_atab
      // printf(" actPti = %d\n",actPti);
    actInd = i2;
    actPtp = DB_get_PT (actPti);            // get DB-index of point
      // UT3D_stru_dump(Typ_PT, actPtp, "  actPtp:");

    // fix plane (set point = planeOrigin)
    if(PED_offTyp == Typ_PLN) {
      UT3D_pl_ptpl (&PED_offObj, actPtp);
        // UT3D_stru_dump(PED_offTyp, &PED_offObj, "PED_offObj:");
    }

    // warn if link exists for that point
    if(actPti >= 0) {
      i3 = PED_atyp[i1];
      APED_oid_dbo__ (sNam, i3, actPti);
      MSG_get_1 (sBuf, 200, "PEDlink", "%s", sNam);
      GUI_DialogYN (sBuf, PED_link_CB);     // "break link ?"
      goto L_exit;
    }

      // printf(" actInd=%d startInd=%d\n",actInd,startInd);
    // change active point into yellow star
    dli = startInd + actInd;
    DL_SetInd (dli);
    APT_disp_SymB (SYM_STAR_S, PED_SYM_COL, &actPta[actInd]);

    // display active mousePos with temporary circ
    PED_points (4,  0);
    if(PED_mode == 0) MSG_pri_0 ("PEDmov");
    goto L_exit;


  //----------------------------------------------------------------
  L_mode6:
    if(mode != 6) goto L_mode7;
    // mode=6    redraw curve with attribut ii
    // UTO_obj_Draw__ geht nur fuer points, polygons ..;
    // nicht fuer B-Splines, bei denen die Durchgangspunkte -
    // (und nicht die gespeicherten Kontrollpunkte gegeben sind).
    
    WC_set_obj_stat (1); // alle Displays mit WC_Work__ ab nun nur temporaer ..


    DB_CSEG__ (0);          // save DB_CSEG-state
    
    basTyp = AP_typ_2_bastyp (actTyp);          // get basictype
    // decode obj from atomic and store in DB.
    APT_store_obj (&basTyp, &actDbi, PED_anr, PED_atyp, PED_atab);

    if(AP_errStat_get ()) {
      // AP_errStat_set (0);    // reset error
      AP_errStat_reset (1);     // reset error
      return 0;
    }

    DL_SetInd (actDli);      // overwrite existing DL-record 

    iatt = DL_get_iatt (actDli);
      // printf(" draw %d %d %d\n",iatt,basTyp,actDbi);

    // display modified DB-obj <basTyp, actDbi>
    APT_Draw__ (iatt, basTyp, actDbi);


    // redraw all depending objects
    PED_mod_dep_draw__ ();

    DB_CSEG__ (1);                     // reset DB_CSEG
    DB_dyn__ (2, Typ_PT, dynPti);      // reset state of dyn-points

      // PED_dump_atab ();
      // PED_dump_ptab ();

    // DL_DumpObjTab ();

/* fuer points, polygons
    OGX_SET_OBJ (&ox1, actTyp, actTyp, 1, actObj);
    dli = -2;  // fixer index
    UTO_obj_Draw__ (&dli, 9, &ox1);
    goto L_exit;
*/
    goto L_exit;



  //----------------------------------------------------------------
  L_mode7:             // 7 = redraw all temporary points
    if(mode != 7) goto L_mode8;
    l1 = startInd;
      // DL_DumpObjTab ();
      // printf(" startInd=%d\n",startInd);

    for(i1=0; i1<actPtn; ++i1) {
      APT_disp_SymB (PED_SYM_ID, PED_SYM_COL, &actPta[i1]);
      // DL_pick_set (-1L, 1);    // make this (last) obj pickable
    }

    // keep all points made in upper loop !
    dynPti = DB_dyn__ (0, Typ_PT, 0L);       // save state of dyn-points

    goto L_exit;


  //----------------------------------------------------------------
  L_mode8:
    if(mode != 8) goto L_mode9;
    // mode=8    delete temporary point actInd
    GL_Del1 (startInd + actInd);
    goto L_exit;


  //----------------------------------------------------------------
  L_mode9:
    // mode=9    restore original points (if Links were broken)
    i2 = -1;
    for(i1=0; i1<PED_anr; ++i1) {
      if(PED_atyp[i1] != Typ_PT) continue;
      l1 = PED_atab[i1];
      ++i2;
        // UT3D_stru_dump (Typ_PT, &actPta[i2], " save actPta %d -> %d",i2,l1);
      DB_StorePoint (l1, &actPta[i2]);
    }


  //----------------------------------------------------------------
  L_exit:
    // printf("ex PED_points\n");

  return 0;

}


//================================================================
  MemObj PED_win__ (MemObj *parent, void *data) {
//================================================================
// create Toolbar

  static  MemObj tbc = GUI_OBJ_NEW;

  MemObj  box0, box1, w1;


  printf("PED_win__ \n");

  if(GUI_OBJ_IS_VALID(&tbc)) goto L_exit;
    
  tbc = GUI_toolbox__ (parent);

  box1 = GUI_box_h (&tbc, "");

  GUI_label_htm__ (&box1, "<b> PointEditor: </b>", "");

  GUI_button__ (&box1, MSG_const__(MSG_help), PED_CB1, (void*)"Help", "");
  GUI_spc_h (&box1, 5);

  PED_f_mod = GUI_box_h (&box1, "");
  PED_w_off = GUI_entry__(&PED_f_mod, "Plane/Vector", "RZ", 
                          PED_off_CB, NULL, "10");
  PED_w_len = GUI_entry__(&PED_f_mod, MSG_const__(MSG_length),"",
                          PED_len_CB, NULL, "10");
  GUI_set_enable (&PED_w_len, FALSE);
  GUI_spc_h (&box1, 5);

  PED_f_sc = GUI_box_h (&box1, "");
  GUI_button__ (&PED_f_sc, MSG_const__(MSG_save), PED_CB1,    (void*)"Save", "");
  GUI_Tip (MSG_get_0("PEDsav"));

  GUI_button__ (&PED_f_sc, MSG_const__(MSG_cancel), PED_CB1,  (void*)"Canc", "");
  GUI_Tip (MSG_get_0("PEDcanc"));

  GUI_set_enable (&PED_f_sc, FALSE);
  GUI_spc_h (&box1, 5);

  PED_f0 = GUI_box_h (&box1, "");
  PED_wa[0] = GUI_radiobutt__
                (&PED_f0, MSG_const__(MSG_delete), 0, PED_CB1, (void*)"Del", "");
  PED_wa[1] = GUI_radiobutt__
                (&PED_f0, MSG_const__(MSG_insert), 1, PED_CB1, (void*)"Ins", "");
  PED_wa[2] = GUI_radiobutt__
                (&PED_f0, MSG_const__(MSG_modify), 1, PED_CB1, (void*)"Mod", "");
  PED_wa[3] = GUI_radiobutt__
                (&PED_f0, MSG_const__(MSG_exit),   1, PED_CB1, (void*)"Exit","");
  // GUI_Butt (PED_f0, "Exit", PED_CB1,    (void*)"Exit", 0);


  L_exit:

  PED_mode = 0;

  return tbc;

}


//================================================================
  int PED_win_act () {
//================================================================
// PED_win_act      activate/deactivate Insert/Delete depending on baseObj

  if(AP_typ_2_bastyp (actTyp) == Typ_CV) {
    GUI_set_enable (&PED_wa[0], TRUE);
    GUI_set_enable (&PED_wa[1], TRUE);

  } else {
    GUI_set_enable (&PED_wa[0], FALSE);
    GUI_set_enable (&PED_wa[1], FALSE);

  }

  GUI_ckbutt_set (&PED_wa[2], 1);   // -> ON


  // if Modify: activate ?; else deactivate.
  GUI_set_enable (&PED_f_mod, TRUE);

  return 0;

}


//=====================================================================
  int PED_CB1 (MemObj *mo, void **data) {
//=====================================================================

  int  i1, form;
  long l1;
  char *cp1, cbuf1[256];
  
  // cp1 = (char*)data;
  cp1 = GUI_DATA_S1;

  
  printf("PED_CB1 |%s| %d %d\n",cp1,PED_anr,PED_mode);
  

  // skip startup
  if(PED_anr < 1) return 0;
  if(PED_atyp == NULL) return 0;
  if(PED_mode < 0) return 0;
  


  
  //-------------------------------------------------
  if(!strcmp(cp1, "Help")) {
    APP_Help ("PED", "");

  
  //-------------------------------------------------
  } else if(!strcmp(cp1, "Del")) {
    // // skip disactivation of delete
    // if(GTK_TOGGLE_BUTTON (parent)->active == 0) return 0;  // 1=ON;0=OFF

    GL_temp_Delete (4L);      // delete temp-vec or plane
    GUI_set_enable (&PED_f_mod, FALSE);
    PED_mode = 2;       // 0=modify, 1=insert, 2=delete
    PED_msg__ ();
    // sele_reset ();
    sele_set__ (Typ_SymB);      // enable selection of all types
    // UI_CursorNo (0);
    // UI_GR_Sel_Filter(0);


  //-------------------------------------------------
  } else if(!strcmp(cp1, "Ins")) {
    // // skip disactivation of insert
    // if(GTK_TOGGLE_BUTTON (parent)->active == 0) {          // 1=ON;0=OFF
      // // DL_pick_set (actDli, 0);  // make the baseObj unselectable
      // return 0;
    // }

    GL_temp_Delete (4L);            // delete temp-vec or plane
    GUI_set_enable (&PED_f_mod, FALSE);
    PED_mode = 1;                  // 0=modify, 1=insert, 2=delete
    // UI_GR_Sel_Filter(18);
    PED_msg__ ();
    // make the baseObj selectable ..
    // DL_pick_set (actDli, 1);
    sele_set__ (Typ_PT);
    sele_setNoConstrPln ();



  //-------------------------------------------------
  } else if(!strcmp(cp1, "Mod")) {
    // // skip disactivation of modify
    // if(GTK_TOGGLE_BUTTON (parent)->active == 0) return 0;  // 1=ON;0=OFF

    GUI_set_enable (&PED_f_mod, TRUE);
    PED_mode = 0;       // 0=modify, 1=insert, 2=delete
    PED_msg__ ();
    // UI_GR_Sel_Filter(0);
    // sele_set__ (Typ_goGeom);      // enable selection of all types
    // sele_set__ (Typ_SymB);           // enable selection of all types
    sele_reset ();
    sele_set_types (Typ_SymB,
                    Typ_VC,
                    Typ_PLN,
                    0);             // make selectable ...

    GL_vcSel_init (-1, 1);          // enable VectorSelector



  //-------------------------------------------------
  } else if(!strcmp(cp1, "Save")) {
    PED_save__ ();                  // save
    PED_CB1 (NULL, GUI_SETDAT_ES(TYP_EventPress,"Canc"));   // do also Canc
    // reload DB-Obj ?
    form = actTyp;
    UTO_get_DB (&actObj, &i1, &form, actDbi);
    // PED_msg__ ();


  //-------------------------------------------------
  } else if(!strcmp(cp1, "Canc")) {
    PED_points (2, 0);          // delete all temp. points
    // restore original points (if Links were broken); but not fro delete !
    if(PED_mode != 2)
    PED_points (9, 0);

    // DB_StorePoint (21L, &actPta[0]);
    PED_get_ptab ();            // init (read src from file)
    PED_points (7, 0);          // draw all temp. points
    PED_points (6, 9);          // display curve to modify temporary
    actInd = -1;
    PED_unlock ();              // unlock mainMenu; lock Save|cancel
    AP_errStat_reset (1);       // 2011-08-31
    DL_Redraw ();
    PED_msg__ ();



  //-------------------------------------------------
  } else if(!strcmp(cp1, "Exit")) {
    if(PED_changed > 0) {
      PED_exit ();
    }
    PED_CB1 (NULL, GUI_SETDAT_ES(TYP_EventPress,"Shutdown")); // Shutdown
    

  //-------------------------------------------------
  } else if(!strcmp(cp1, "Shutdown")) {
    PED_points (2, 0);              // remove selectionPoints
    AP_UserSelection_reset ();      // reset  select
    AP_UserMousemove_reset ();      // reset user-mouseMovementCallback
    UI_block_group (0);             // unblock groupSelections

    // restore some application-functions...
    // // GUI_obj_del (PEDtb);         // remove Toolbar
    // GUI_obj_hide (PEDtb);           // remove Toolbar
    GUI_set_show (&PEDtb, 0);           // remove Toolbar
    UI_func_stat_set__ (APF_TB_CAD, 
                      APF_MEN_FIL,
                      APF_MEN_SM,  
                      APF_MEN_MDF,
                      APF_MEN_APP,
                      APF_WIN_B_E,
                      APF_HIDE,
                      APF_VWCADMAN,
                      APF_BUT_GSE,
                      APF_PLUGINS,
                      APF_UNDO,
                      0);


    PED_restore3 ();                // restore model
    if(PED_fp_dep) {
      fclose (PED_fp_dep);
      PED_fp_dep = NULL;
    }
    if(actPta) {
      free (actPta);
      free (PED_atyp);
      free (PED_atab);
      actPta = NULL;
      PED_anr = 0;
    }

      // printf(" free depTab\n");
    MemTab_free (&depTab);        // free mem

    DL_hili_off (-1L);           // unhilite ..
    AP_SRC_mem_edi ();            // if MAN: copy Memory -> Editor
    AP_User_reset ();             // gCad_fini ();
    PED_mode = -1;
    // DL_pick__ (1);                // reset selectionfilter
    sele_set__ (Typ_goGeom);      // enable selection of all types
  }


  L_exit:
    return 0;
}


//================================================================
  int PED_off_CB (MemObj *mo, void **data) {
//================================================================
// check PED_w_off; if first char == 'D' open PED_w_len.

  int    irc;
  char   *cp1;

  // printf("PED_off_CB \n");


  cp1 = GUI_entry_get (&PED_w_off);
    // printf(" PED_w_off |%s|\n",cp1);

  if((*cp1 == 'D')||(*cp1 == 'd')) {
    GUI_set_enable (&PED_w_len, TRUE);
  } else {
    GUI_set_enable (&PED_w_len, FALSE);
  }

  irc = PED_get_offObj ();
  if(irc >= 0) PED_mousemove_CB (0, 0);  // update

  // return 0;
  // printf("exit PED_off_CB \n");

  return FALSE;  // do defaultOperations

}


//================================================================
  int PED_len_CB (MemObj *mo, void **data) {
//================================================================
// set PED_offLen

  char   *cp1;


  // printf("PED_len_CB \n");

  cp1 = GUI_entry_get (&PED_w_len);

  if(strlen(cp1) > 0) {
    PED_offLen = atof(cp1);
      // printf(" PED_len_CB %f |%s|\n",PED_offLen,cp1);

    // put vz with PED_offLen -> vx
    UT3D_vc_setLength (&PED_offObj.vx, &PED_offObj.vz, PED_offLen);

  } else {
    PED_offLen = 0.;
  }

  PED_get_offObj ();
  PED_mousemove_CB (0, 0);  // update

  return 0;

}


//================================================================
  int PED_link_CB (MemObj *mo, void **data) {
//================================================================
   int idat;
 
   idat = GUI_DATA_EVENT;

   printf("PED_link_CB %d\n",idat);
 
   switch(idat) {
 
     case UI_FuncOK:
       // save ...
       PED_points (4,  0);
       MSG_pri_0 ("PEDmov");
       break;
 
     case UI_FuncCancel:
       // skip saving ...
       PED_CB1 (NULL, GUI_SETDAT_ES(TYP_EventPress,"Canc"));
       break;
   }
 
   return 0;
 
}


//================================================================
  int PED_get_ptab () {
//================================================================
// get all points of actObj into actPta, actPtn
// Input: actTyp actDbi actDli

  int     irc, i1, i2, iNr, *atyp;
  double  *atab;
  long    li, lRef;
  Plane   *plnRef;
  Mat_4x3 mRef;



  // printf("PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP \n");
  // printf("PED_get_ptab %d\n",actTyp);


  // get & decode the sourceObj
  irc = PED_get_ato (&i1, &atyp, &atab);
  if(irc < 0) return irc;
    // printf(" i1=%d PED_anr=%d\n",i1,PED_anr);


  // return 0;


  // get memoryspace for actPta
  // if(i1 <= PED_anr) {
  if(actPta == NULL) {
    actPta = (Point*) malloc (i1 * sizeof(Point));
    PED_atyp = (int*) malloc (i1 * sizeof(int));
    PED_atab = (double*) malloc (i1 * sizeof(double));

  } else {
    actPta = (Point*) realloc (actPta, i1 * sizeof(Point));
    PED_atyp = (int*) realloc (PED_atyp, i1 * sizeof(int));
    PED_atab = (double*) realloc (PED_atab, i1 * sizeof(double));
  }

  PED_anr = i1;


  // // load refSys - if one exists
  // lRef = DL_GetTrInd (actDli);
  // if(lRef > 0) {
    // plnRef = DB_get_PLN (lRef);
    // UT3D_m3_loadpl (mRef, plnRef);
    // memcpy (APT_2d_to_3d_Mat, mRef, sizeof(Mat_4x3));
    // APT_2d_to_3d_mode = ON;
  // }


  // copy points from atomicObj -> actPta
  i2 = -1;
  for(i1=0; i1 < PED_anr; ++i1) {
      // printf(" PED[%d]= %d %f\n",i1,atyp[i1],atab[i1]);
    PED_atyp[i1] = atyp[i1];
    PED_atab[i1] = atab[i1];
    if(PED_atyp[i1] != Typ_PT) continue;
    ++i2;
    li = PED_atab[i1];
    actPta[i2] = DB_GetPoint (li);
      // UT3D_stru_dump (Typ_PT, &actPta[i2], "   DB-pt[%d]",li);
    // if refSys is active: transform ..
    // if(lRef > 0) UT3D_pt_traptm3 (&actPta[i2], mRef, &actPta[i2]);
  }
  actPtn = i2 + 1;


    // PED_dump_atab ();
    // PED_dump_ptab ();



  // printf("ex PED_get_ptab %d\n",actPtn);

  return 0;
}
  

//================================================================
  int PED_dump_atab () {
//================================================================
// dump actPta
  
  int    i1;
  long   dbi;
  Point  *pt1;

  printf("PED_dump_atab %d\n",actPtn);

  for(i1=0; i1 < PED_anr; ++i1) {
    printf(" PED_atyp[%d]=%d PED_atab=%f\n",i1,PED_atyp[i1],PED_atab[i1]);
    if(PED_atyp[i1] != Typ_PT) continue;
    dbi = PED_atab[i1];
    pt1 = DB_get_PT (dbi);
    UT3D_stru_dump (Typ_PT, pt1, "   DB-pt[%d]",dbi);
  }


  return 0;

}
 

//================================================================
  int PED_dump_ptab () {
//================================================================
// dump actPta

  int i1;

  printf("PED_dump_ptab %d\n",actPtn);

  for(i1=0; i1 < actPtn; ++i1)
    UT3D_stru_dump (Typ_PT, &actPta[i1], "actPta[%d]", i1);


  return 0;

}
 

//================================================================
  int PED_get_ato (int *iNr, int **atyp, double **atab) {
//================================================================
// get & decode the sourceObj
// Output:
//   iNr    nr of points

  int     i1, lNr, bufSiz;
  char    *cPos, *cBuf, *cp1, *cp2;
  ObjAto  ato;

  // printf("PED_get_ato \n");



  // get sourceLine
  // cBuf = memspc201;  // mem_cbuf1
  // bufSiz = sizeof(memspc201);
  cBuf = mem_cbuf1;  // mem_cbuf1
  bufSiz = mem_cbuf1_SIZ;

  // get sourceObj into cBuf
  i1 = PED_get_src (cBuf, bufSiz);

  // decode old sourcecode
  cp1 = strchr(cBuf, '=');
  if(cp1 == NULL) {TX_Print("PED_get_ato E001"); return -1;}


  cp2 = cp1 + 1; // skip '='
  // cut off objName
  APED_onam_cut (cp2);
    // printf(" _get_ato code=|%s|\n",cp2);


  // decode into atomic
  // leider keine Info über die Art des Obj; als via 3 Values oder via
  // parametric point od via DirectPoint ..
  // *iNr = APT_decode__ (atyp, atab, &cp2);
  *iNr = APT_decode__ (&ato, &cp2);
    PED_dump_atab ();

  *atyp = ato.typ;
  *atab = ato.val;

  // TEST ONLY
    // for(i1=0; i1 < *iNr; ++i1)
      // printf("atomicObj[%d]=%d %f\n",i1,(*atyp)[i1],(*atab)[i1]);
    // printf("ex PED_get_ato iNr=%d cp2=|%s|\n",*iNr,cp2);
  // TEST ONLY


  return 0;
}


//================================================================
  int PED_get_src (char *cBuf, int bufSiz) {
//================================================================
// get sourceObj from tempfile into cBuf.

  long    i1, lNr;
  char    *cPos;
  // char    fnam[160];


  // printf("PED_get_src %d\n",bufSiz);


/*  2011-09-05 unnutz ?
  // get & decode old sourcecode
  DL_Get_lNr_dli (&lNr, actDli);
     // printf(" lNr=%d actDli=%d\n",lNr,actDli);

  cPos = UTF_GetLinNr (cBuf, &i1, lNr);           // get pos of sourcecode
     // printf(" lNr=%d ll=%d |%s|\n",lNr,i1,cBuf);
*/

  // read file into memstring
  PED_rea_src (cBuf, bufSiz);


  // // load file into buf1
  // sprintf(fnam,"%stmp%cselection.txt",OS_get_bas_dir(),fnam_del);
  // UTF_clear1 ();
  // *cBuf = UTF_add1_file (fnam);
  // UTX_CleanCR (*cBuf);   // remove LF ..

  // printf("ex PED_get_src |%s|\n",cBuf);


  return 0;
}


//================================================================
  int PED_wri_src (char *cBuf) {
//================================================================
// save sourceLine

  char    fnam[160];

  // printf("PED_wri_src |%s|\n",cBuf);

  sprintf(fnam,"%sselection.txt",OS_get_tmp_dir());
  UTX_wrf_str (fnam, cBuf);

  return 0;

}


//================================================================
  int PED_rea_src (char *cBuf, int bufSiz) {
//================================================================
// read file into memstring

  int     irc;
  long    l1;
  char    fnam[160];


  sprintf(fnam,"%sselection.txt",OS_get_tmp_dir());

  l1 = 0;
  irc = UTF_add_fil__ (cBuf, &l1, bufSiz, fnam);
  if(irc < 0) return irc;
  UTX_CleanCR (cBuf);            // remove LF
  return irc;

}


//================================================================
  int PED_exit () {
//================================================================

//  - alle Punkte testen, ob gleich od modifiziert;
//      gleich: den PunktText aus dem alten Source uebernehmen;
//      modifiziert: ersetzen durch den dynamischen Code P(x y z)
//  
//  - den existierenden Code deaktivieren (prepend "_")
//  - insert new code hinter altem Code
//  - redraw all starting with old codeline
  

  int     irc, i1, i2, cNr, bufSiz;
  long    l1, l2, lNr;
  char    *cPos, *cBuf, *cp1, *cp2;
  int     iNr;


  // printf("PED_exit %d\n",PED_anr);


  // get & decode the sourceObj
  cBuf = memspc201;  // mem_cbuf1      // get space for sourcecode
  bufSiz = sizeof(memspc201);

  i1 = PED_get_src (cBuf, bufSiz);     // load new line from file
  // bufSiz = UTF_GetLen1 ();
    // printf(" str |%s|\n",cBuf);


  DL_Get_lNr_dli (&lNr, actDli);       // get lNr from DL-ind


  irc = APED_src_chg (lNr, cBuf);      // deactivate line, insert modified line
                                       // report change -> UNDO.

  // // reset refSys
  // APT_2d_to_3d_mode = OFF;

  return 0;

}

//================================================================
  int PED_save__ () {
//================================================================

  int     irc, i1, bufSiz;
  char    *cBuf;


  // printf("PED_save__\n");

  // get sourceLine
  cBuf = memspc201;  // mem_cbuf1    // get space for sourcecode
  bufSiz = sizeof(memspc201);
  i1 = PED_get_src (cBuf, bufSiz);
  // bufSiz = UTF_GetLen1 ();


  // modify the sourceLine 
  if(PED_mode == 0)      irc = PED_src_mod (cBuf);
  else if(PED_mode == 1) irc = PED_src_ins (cBuf, bufSiz);
  else if(PED_mode == 2) irc = PED_src_del (cBuf);

  if(irc < 0) return -1;

  // save sourceLine
  PED_wri_src (cBuf);


  // increment change-flag
  ++PED_changed;

  // printf("ex PED_save__\n");

  return 0;

}
 

//================================================================
  int PED_src_mod (char *cBuf) {
//================================================================
// compare all points; modify points in src cBuf.

  int   irc, i1, i2, sLen, ii;
  long  dbi;
  char  *cp1, *cp2, *cp3;
  Point *pt1;


  // printf("PED_src_mod |%s|\n",cBuf);
  // PED_dump_ptab ();
  // PED_dump_atab ();


  // skip objnam
  cp1 = strchr(cBuf, '=');
  if(!cp1) {TX_Print("PED_src_mod E001"); return -1;}
  ++cp1;   // skip "="


  // compare points & create new sourcetext
  i2 = -1;
  for(i1=0; i1<PED_anr; ++i1) {
    if(PED_atyp[i1] != Typ_PT) continue;
    ++i2;                                   // index into actPta
    dbi = PED_atab[i1];                     // index of nxt point

    irc = APED_pt_find (&cp2, &sLen, &cp3, cp1);  // cp2 now is the next point
    if(irc < 0) {TX_Print("PED_src_mod E002"); return -1;}
      ii = cp3 - cp2 - sLen;                // nr of chars for deli, blanks

    irc = PED_compare (dbi, &actPta[i2]);   // compare points
    pt1 = DB_get_PT (dbi);
    UTRA_pt_wcs2ucs (pt1);

    if(irc == 0) {                          // 0=diffent points
      APED_pt_chg (&sLen, dbi, cp2, sLen);  // change point
      DB_StorePoint (dbi, &actPta[i2]);     // restore the original values
    }

    cp1 = cp2 + sLen + ii;
  }

  // printf("ex PED_src_mod |%s|\n",cBuf);

  return 0;

}


//================================================================
  int PED_src_del (char *cBuf) {
//================================================================
// delete point actInd in srcLine cBuf.


  int   irc, i1, i2, sLen;
  long  dbi;
  char  *cp1, *cp2, *cp3;


  // printf("PED_src_del %d\n",actInd);
  // printf("  cBuf in |%s|\n",cBuf);
  // PED_test__ ();


  // skip objnam
  cp1 = strchr(cBuf, '=');
  if(!cp1) {TX_Print("PED_src_mod E001"); return -1;}
  ++cp1;   // skip "="


  // compare points & create new sourcetext
  i2 = -1;
  for(i1=0; i1<PED_anr; ++i1) {
    if(PED_atyp[i1] != Typ_PT) continue;
    ++i2;                                   // index into actPta
    dbi = PED_atab[i1];                     // index of nxt point
    APED_pt_find (&cp2, &sLen, &cp3, cp1);  // cp2 now is the next point

    if(i2 != actInd) {
      cp1 = cp3;                            // skip this point
      continue;
    }

    MEM_del_str (cp2, sLen);                // delete point & done
    break;
  } 


  // printf("  cBuf out|%s|\n",cBuf);

  return 0;

}


//================================================================
  int PED_src_ins (char *cBuf, int bufSiz) {
//================================================================
// insert point actPtp behind point # actInd in srcLine cBuf.

     
  int   irc, i1, i2, sLen;
  char  *cp1, *cp2, *cp3, s2[80];


  // printf("PED_src_ins actInd=%d bufSiz=%d\n",actInd,bufSiz);
  // printf("  cBuf in |%s|\n",cBuf);
  // printf("  WC_sur_ind=%d\n",WC_sur_ind);



  // skip objnam
  cp1 = strchr(cBuf, '=');
  if(!cp1) {TX_Print("PED_src_mod E001"); return -1;}
  ++cp1;   // skip "="


  // translate insertpoint actInsPt
    // UT3D_stru_dump (Typ_PT, &actInsPt, "   actInsPt:");
  if(WC_sur_ind) {
    UT3D_pt_traptm3 (&actInsPt, WC_sur_imat, &actInsPt);
      // UT3D_stru_dump (Typ_PT, &actInsPt, "   actInsPt-transl:");
  }
    

  // insertpoint -> s2
  s2[0] = '\0';
  AP_obj_add_pt (s2, &actInsPt);
  strcat(s2, " ");
    // printf("  s2=|%s|\n",s2);


    
  // compare points & create new sourcetext
  i2 = -1;
  for(i1=0; i1<PED_anr; ++i1) {
    if(PED_atyp[i1] != Typ_PT) continue;
    ++i2;                                   // index into actPta
    APED_pt_find (&cp2, &sLen, &cp3, cp1);  // cp2 now is the next point

    if(i2 <= actInd) {
      cp1 = cp3;                            // skip this point
      continue;
    }


    // insert s2, done
    sLen = cp2 - cBuf;
    // insert s2 at position cp2
    irc = MEM_ins_str (&sLen, cp2, s2, bufSiz - sLen);
    if(irc < 0) return irc;
    break;
  }

    // printf("  cBuf out|%s|\n",cBuf);

  return 0;

}


//================================================================
  int PED_compare (long pti, Point *ptNew) {
//================================================================
// compare points
// RetCod:  0=diffent points; 1=equal points

  Point  *pt1;


  // printf("PED_compare pti=%d pt=%f %f %f\n",pti,ptNew->x,ptNew->y,ptNew->z);


  // get point from DB
  pt1 = DB_get_PT (pti);
    // UT3D_stru_dump (Typ_PT, pt1, "DB-pt[%d]",pti);


  // compare point (rc=0=diffent points; 1=equal points
  if(UT3D_comp2pt(pt1, ptNew, UT_TOL_pt) != 0) {
    // // points are not equal; create new dynamic point
    // *pti = DB_StorePoint (-1L, ptNew);
      // printf(" equal points\n");
  
    return 1;
  }

  // // different points: we must restore the original values !
  // DB_StorePoint (pti, ptNew);

  // printf(" different points\n");


  return 0;

}
/*
//================================================================
  int PED_replace (long ip1, long ip2, char **cbuf) {
//================================================================
// replace or skip point
// replace point with index ip1 by point with index ip2
// if (ip1 == ip2) then skip point ..
// if (ip2 < 0) = dynamic-point: change ObjSRC into (x y z)

  int  il1;
  long l1;
  char *cp1, *cp2, *cp3, s1[80];
  void *vp1;


  cp1 = *cbuf;

  printf("PED_replace %d %d |%s|\n",ip1,ip2,cp1);


  // find position of next point
  if(ip1 >= 0) {
    L_pt1:
    cp2 = strstr(cp1, " P");
    if(!cp2) return -1;
    cp1 = cp2 + 2;
    if(!isdigit(cp2)) goto L_pt1;
    cp2 += 2;            // points now to the char behind 'P'
  } else {
    cp2 = strstr(cp1, " P(");
    if(!cp2) return -1;
    cp2 += 3;            // points now to the char behind '('
  }



  // find il = length of point-describing-charactes 
  if(ip1 >= 0) {
    // find nr of consecutive digits
    il1 = UTX_ck_num_digNr (&cp3, cp2);
  } else {
    // find corresponding ')'
    cp3 = UTX_pos_skipBrack1 (cp2);
    if(*cp3 != ')') return -1;
    il1 = cp3 - cp2;
  }
    printf(" il1=%d\n",il1);




  //----------------------------------------------------------------
  // replace word; old length = il1
  if(ip1 == ip2) goto L_skip;
  L_repl:
  if(ip2 >= 0) {
    sprintf(s1, "%d", ip2);
  } else {
    // use ip1; ip2 wurde neu angelegt (= OLD-POINT!)
    vp1 = (void*) DB_get_PT (ip1);
      UT3D_stru_dump (Typ_PT, vp1, "DB-pt[%d]",ip1);
    if(ip1 >= 0) strcpy(s1, "(");
    else         s1[0] = '\0';
    AP_obj_add_nval (s1, 3, vp1);
    if(ip1 >= 0) strcat(s1, ")");
  }
  // replace; at pos cp2
    printf(" vor replace |%s|%s|\n",cp2,s1);
  UTX_chg_str1 (&l1, cp2, s1, cp2, il1);
    printf(" nach replace |%s|\n",cp2);



  //----------------------------------------------------------------
  // skip point
  L_skip:
  if(ip1 >= 0) {
    cp2 = cp3;
  } else {
    cp2 = cp3 + 1;
  }



  //----------------------------------------------------------------
  L_exit:
  *cbuf = cp2;
  return 0;

}
*/

//================================================================
  int PED_get_tabInd (int ipt) {
//================================================================
// get index into PED_atab from pointNr actInd (index into actPta)

  int  i1, ii;

  // printf("PED_get_tabInd actInd=%d PED_anr=%d\n",actInd,PED_anr);


  ii = -1;
  for(i1=0; i1 < PED_anr; ++i1) {
    if(PED_atyp[i1] != Typ_PT) continue;
    ++ii;
    if(ii == ipt) goto L_exit;
  }

  TX_Error("PED_get_tabInd E001");
  ii = -1;

  L_exit:

    // printf("ex PED_get_tabInd %d %d\n",i1,ipt);

  return i1;

}

/*
//================================================================
  int PED_restore1 () {
//================================================================
// restore points
// overwrite DB-points (PED_atyp,PED_atab) with actPta-points


  int    i1, ii;
  long   l1;
  Point  *ptp;

  ii = -1;
  for(i1=0; i1 < PED_anr; ++i1) {
    if(PED_atyp[i1] != Typ_PT) continue;
    ++ii;
    // i1=ind into PED_atab;  ii=ind into actPta
    l1 = PED_atab[i1];
    ptp = DB_get_PT (l1);
    *ptp = actPta[ii];
  }


  // redraw actObj
  // get linetype: see APT_work_def GA_find__ ..
  PED_points (6, 0);


  return 0;

}


//================================================================
  int PED_restore2 () {
//================================================================
// restore points
// get old sourceline, execute ..

  int     i1, lNr, bufSiz;
  char    *cPos, *cBuf;


  // get the sourceObj
  cBuf = memspc201;  // mem_cbuf1    // get space for sourcecode
  bufSiz = sizeof(memspc201);
  i1 = PED_get_src (cBuf, bufSiz);
    printf("PED_restore2 old str |%s|\n",cBuf);

  WC_set_obj_stat (0);    // save permanent
  DL_Get_lNr_dli (&lNr, actDli);
  DL_SetInd (actDli);
  WC_Work1 (lNr, cBuf);

  return 0;

}
*/

//================================================================
  int PED_restore3 () {
//================================================================
// update program starting with line lNr

  long    lNr;

  // reset DL_ind_act
  DL_SetInd (-1L);  // else problem with move one point only

  // get the sourceObj
  WC_set_obj_stat (0);    // save permanent
  DL_Get_lNr_dli (&lNr, actDli);

  ED_work_CurSet (lNr);            // go back to line lNr
  ED_work_CurSet (UT_INT_MAX);     // work until end of model

    // printf("ex PED_restore3\n");

  return 0;

}


//================================================================
  int PED_dep_sav () {
//================================================================
// write sourcecodes of all depending objects -> file

  int      i1, i2, ie, ii, ix;
  long     ll;
  char     *cp1, fnam[160];
  ObjSRC   *sa;
  FILE     *fpo;


  // printf("PED_dep_sav %d\n",depTab.rNr);
  // MemTab_load (&sa, &ie, &depTab);
  // for(i1=0; i1 < ie; ++i1)
  // printf(" oa[%d] typ=%d dbi=%d lnr=%d\n",i1,sa[i1].typ,sa[i1].dbi,sa[i1].lnr);



  sprintf(fnam,"%ssrc_dep.txt",OS_get_tmp_dir());
  if(depTab.rNr < 2) {
    OS_file_delete (fnam);
    return 0;
  }


  if((fpo = fopen (fnam, "wb")) == NULL) {
    TX_Print("PED_dep_sav E001");
      printf(" **** PED_dep_sav fopen-err |%s|\n",fnam);
    return -1;
  }


  MemTab_load (&sa, &ie, &depTab);


  for(i1=1; i1 < ie; ++i1) {   // skip first (primary) obj)
    // printf(" oa[%d] typ=%d dbi=%d lnr=%d\n",i1,sa[i1].typ,sa[i1].dbi,sa[i1].lnr);

    // sort ascend. lnr 
    ii = UT_INT_MAX;
    for(i2=1; i2 < ie; ++i2) {
      if(sa[i2].lnr < ii) {ii = sa[i2].lnr; ix = i2;}
    }
      // printf(" i1=%d ii=%d\n",i1,ii);


    // get stringposition cp1 & strinLength ll in memory
    cp1 = UTF_GetPosLnr (&ll, sa[ix].lnr);

    // write -> file
    fwrite (&sa[ix].lnr, sizeof(long), 1, fpo);
    fwrite (&sa[ix].dli, sizeof(long), 1, fpo);
    fwrite (&ll, sizeof(long), 1, fpo);
    fwrite (cp1, ll, 1, fpo);
    // fputc ('\n',fpo);

    sa[ix].lnr = UT_INT_MAX;
  }

  fclose (fpo);

  // open for update ..
  if((PED_fp_dep = fopen (fnam, "rb")) == NULL) {
    TX_Print("PED_dep_sav E002");
    return -1;
  }

  return 0;

}


//================================================================
  int PED_mod_dep_draw__ () {
//================================================================
// redraw all depending objects

  long   lnr, dli, ll;

  // printf("PED_mod_dep_draw__ -------- \n");


  if(depTab.rNr < 2) return 0;


  rewind (PED_fp_dep);

  for(;;) {
    // if (fgets (mem_cbuf1, mem_cbuf1_SIZ, PED_fp_dep) == NULL) break;
    // UTX_CleanCR (mem_cbuf1);
    ll = fread(&lnr, sizeof(long), 1, PED_fp_dep);
    if(ll == 0) break;
    fread(&dli, sizeof(long), 1, PED_fp_dep);
    fread(&ll, sizeof(long), 1, PED_fp_dep);
    fread(mem_cbuf1, ll, 1, PED_fp_dep);
    mem_cbuf1[ll] = '\0';
      // printf(" _draw__ %d %d |%s|\n",lnr,dli,mem_cbuf1);

    // redraw
    PED_mod_dep_draw_l (lnr, dli, mem_cbuf1);
  }


  return 0;

}


//================================================================
  int PED_mod_dep_draw_l (int lNr, long dli, char *src) {
//================================================================
// redraw a single line temporary
    
  // int    lNr;
  // long   dli;

  // printf("PED_mod_dep_draw_l |%s|\n",src);


  // strcpy(src, "L24=P21 P23");
  // lNr = 12;
  // dli = 10;

  DL_SetInd (dli);  // overwrite existing DL-record
  WC_Work1 (lNr, src);
  
  return 0;
    
}   


//================================================================
  int PED_get_offObj () {
//================================================================
// get offsetObject (Plane or Vector) out of PED_w_off

  int      irc;
  long     l1, dbi;
  char     *p1;
  Plane    *pl1;


  p1 = GUI_entry_get (&PED_w_off);


    // printf("PED_get_offObj  PED_w_off=|%s|\n",p1);


  irc = APED_dbo_oid (&PED_offTyp, &dbi, p1);
    // printf(" _obj irc=%d %d %d\n",irc,PED_offTyp,dbi);
  if(irc < -1) goto L_err;
    // MSG_pri_0 ("PEDe2");
    // TX_Print("**** ERROR: cannot analyze Plane/Vector |%s| ..",p1);



  if(PED_offTyp == Typ_VC) {
    if(irc < 0) {  // "D(..)"
      // wait until closing ")"
      if(p1[strlen(p1) - 1] != ')') goto L_err;
      irc = AP_vec_txt (&PED_offObj.vz, p1);
      if(irc < 0) goto L_err;

    } else {       // "D123"
      if(DB_VC_isFree (dbi)) goto L_err;
      PED_offObj.vz = DB_GetVector (dbi);
    }
      // UT3D_stru_dump(Typ_VC, &PED_offObj.vz, "PED_offObj.vz:");


  } else if(PED_offTyp == Typ_PLN) {
    if(DB_PLN_isFree (dbi)) goto L_err;
    DB_GetRef (&PED_offObj, dbi);
  }
  

    // printf("ex PED_get_offObj PED_offTyp=%d\n",PED_offTyp);
    // UT3D_stru_dump(PED_offTyp, &PED_offObj, "  PED_offObj:");


  return 0;


  L_err:
    PED_offTyp = Typ_Error;
    GL_temp_Delete (4L);  //delete temp-vec or plane
    DL_Redraw ();
    return irc;

}


//================================================================
  int PED_del__ () {
//================================================================
// delete point actInd
// delete point # <actInd> in sourcecode & redraw

  int     i1, ii;


  // get index into PED_atyp
  ii = PED_get_tabInd (actInd);

    printf("PED_del__ %d %d\n",actInd,ii);


  // delete temp-point
  GL_temp_Delete (3L);  // delete temporary obj -3 (circ)
  PED_points (8, 0);       // remove 2011-10-07


  // delete point from atomicObj
  // MEM_del_nrec (&actPtn, actPta, ii, 1, sizeof(Point));
  i1 = PED_anr;
  MEM_del_IndRec (&PED_anr, PED_atyp, ii);
  MEM_del_DbRec (&i1, PED_atab, ii);


  PED_stat = 1;
  PED_mousemove_CB (0,0);   // redraw
  PED_stat = 0;


  // PED_points (7, 0);


  // lock mainMenu; unlock Save|cancel
  PED_lock ();

  return 0;

}


//================================================================
  int PED_ins_pt (Point *pt1) {
//================================================================
// get segmentnr where to insert point & save point
// set actInd = after this point insert new point pt1
// save point in actPta[0]

  int      i1;
  long     dli;
  double   up, da[3], d1;
  Point    pta[3];
  // Memspc   tmpSpc1;


  UT3D_stru_dump (Typ_PT, pt1, "PED_ins_pt: ");
  printf("  actTyp=%d\n",actTyp);
  printf("  UT_DISP_cv=%f\n",UT_DISP_cv);


  //----------------------------------------------------------------
  if(actTyp == Typ_CVBSP) {
    // get segmentnr for this point

    // get parameter from point
    // UME_init (&tmpSpc1, memspc201, sizeof(memspc201));
    UT3D_parCv_bsplpt (&up, &d1, actObj, pt1);
      printf(" val. parameter=%f dist=%f\n",up,d1);

    if(d1 > UT_DISP_cv * 2.) goto L_err1;


    // change knotenwert into parameter
    // up = UT3D_par1_parBSp (&uk, actObj);
    // up = UT3D_par1_parcbsp (&uk, actObj);
      // printf(" up=%f\n",up);

    // get segmenNr from parameter
    // UME_init (&tmpSpc1, memspc201, sizeof(memspc201));
    bspl_segNr_par (&actInd, up, actObj);
      printf(" segNr=%d\n",actInd);




  //----------------------------------------------------------------
  } else if(actTyp == Typ_CVPOL) {
    // project point onto polygon
    i1 = 3;
    UT3D_pt_projptplg (&i1, pta, da, actObj, pt1);
    // get parameter of point on polygon
    UT3D_parplg_plgpt (&up, pta, actObj);
    UPLG_iseg_par (&actInd, up, actObj);
      printf(" actInd=%d up=%f\n",actInd,up);




  //----------------------------------------------------------------
  } else {
    goto L_err1;
  }


  //----------------------------------------------------------------
  // insert point ..

  // save point
  // actPta[0] = *pt1;
  actInsPt = *pt1;

  // display position where to insert
  // actPtp = &actPta[0];
  // PED_points (4, 0);
  dli = -3;
  GL_DrawSymB (&dli, PED_SYM_COL, SYM_CIR_S, &actInsPt);
  DL_Redraw ();

  // lock MainMenu
  PED_lock ();

  return 0;


  L_err1:
    MSG_pri_0 ("PEDe3"); // cannot insert points ..
    return -1;

}


//================================================================
  int PED_lock () {
//================================================================
// lock mainMenu; unlock Save|cancel

  printf("PED_lock \n");


  GUI_set_enable (&PED_f0, FALSE);       // disactivate main-functions
  GUI_set_enable (&PED_f_sc, TRUE);      // activate Save|cancel
  MSG_pri_0 ("PEDmod3");    // save or cancel

  PED_stat = 2;

  // UI_CursorNo (1);
  // UI_GR_Sel_Filter (20);  // no select

  return 0;

}


//================================================================
  int PED_unlock () {
//================================================================
// unlock mainMenu; lock Save|cancel
// Save/cancel nur fuer delete points !
    
  printf("PED_unlock \n");

  GUI_set_enable (&PED_f0, TRUE);       // disactivate main-functions
  GUI_set_enable (&PED_f_sc, FALSE);      // disactivate Save|cancel
  // GUI_set_enable (&PED_f_sc, TRUE);      // mod.2013-09-05
  // TX_Print("select save or exit ..");
  
  PED_stat = 0;

  return 0;
    
}


//================================================================
  int PED_test__ () {
//================================================================

  Point   pt1;

  printf("========================================= PED_test__\n");


  pt1 = DB_GetPoint (22L);
  UT3D_stru_dump (Typ_PT, &pt1, "ex DB_GetPoint 22");


  return 0;

}


// EOF
