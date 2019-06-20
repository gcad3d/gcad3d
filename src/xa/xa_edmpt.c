// ../xa/tst_edmpt.c                              2018-02-16  RF
/*
 *
 * Copyright (C) 2018 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
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
    replace actPta (use DB-points) EDMPT_get_tabInd gives index ..
      status point-modified parallel to pMod ?

-----------------------------------------------------
Modifications:
2018-02-16 new - replacing xa_ped.c RF.

-----------------------------------------------------
*/
#ifdef globTag
 void EDMPT(){}
#endif
/*!
\file  ../xa/xa_ped.c
\brief CurveEditor 
\code
=====================================================
List_functions_start:

EDMPT_init         decode obj; init toolbar
EDMPT_win__        create Toolbar
EDMPT_key_CB       callback user keypress
EDMPT_sel_CB       userselection callback
EDMPT_CB1          toolbar-widgets-callbacks
EDMPT_off_CB       cb of entry Plane - EDMPT_w_off
EDMPT_len_CB       cb of entry vectorlength - EDMPT_offLen
EDMPT_mousemove_CB callback mousemovement

EDMPT_lock         lock delete,insert,modify,exit; unlock Save|cancel
EDMPT_unlock       unlock mainMenu; lock Save|cancel
EDMPT_upd_stop     exit modify-update; activate Save|cancel
EDMPT_fixed_0      fix widgets for modeify with fixed-vector
EDMPT_msg_mode     message about EDMPT_mode
EDMPT_get_offObj   get offsetObject (Plane or Vector) out of EDMPT_w_off

EDMPT_linked__     point <actPti> is linked to parentObj ..
EDMPT_restart_lnk  restart with new obj
EDMPT_exit_1       insert new code, redraw all
EDMPT_exit_2       update model starting with line lNr

EDMPT_wri_src      save sourceLine -> tempfile
EDMPT_rea_src      get the sourceObj from tempfile into memstring
EDMPT_src_get_src  get source of active obj; skip, defHdr; cut objName
EDMPT_src_get__    get source of active obj
EDMPT_src_save     copy selection.txt > selection1.txt or back
EDMPT_save__       save (modify src & write -> tempfile)
EDMPT_src_mod      compare all points; modify points
EDMPT_src_del      delete point actInd in srcLine
EDMPT_src_ins      insert point in srcLine

EDMPT_dep_sav      write sourcecodes of all depending objects (childs) -> file
EDMPT_mod_dep_draw__ redraw all depending objects

EDMPT_obj_get      decode actObj/actAto, store in DB, reload (bin.obj) actObj
EDMPT_ptab_get     get actObj, all points of actObj into actPta
EDMPT_ptab_3D_2D   actObj is on 2D-plane. Transfer points into 3D
EDMPT_get_tabInd   get index into EDMPT_atab from pointNr actInd
EDMPT_points       point-functions
EDMPT_del__        delete point actInd

EDMPT_ins_pt       get segmentnr where to insert point & save point
EDMPT_newPos__     get cursorposition (mode=0; projected onto plane|vector)
EDMPT_curSym_set   set symbol cursor plane|vector

EDMPT_mem_free     free memSpaces, remove selectionPoints ..
EDMPT_dump_ptab
EDMPT_dump_atab


------------------------------------------
EDMPT_newPos__     get cursorposition (mode=0; projected onto plane|vector)

List_functions_end:
=====================================================
see also
UI_WinTra__       alter modelPositionEditor

\endcode *//*----------------------------------------

KERNELMODE -> TESTMODE:
cp ../xa/xa_edmpt.c ../xa/tst_edmpt.c
in ../xa/tst_edmpt.c  change "int EDMPT_init" -> "int _EDMPT_init"


TESTMODE -> KERNELMODE:
cp ../xa/tst_edmpt.c ../xa/xa_edmpt.c
in ../xa/xa_edmpt.c change "int _EDMPT_init" -> "int EDMPT_init"



----------------------------------
PED - Pointeditor:
Is using Selection-callback & Mousemove-callback (like plugins !)

Startup:   EDMPT_init
Shutdown:  EDMPT_key_CB | EDMPT_sel_CB > EDMPT_CB1


Input: object with points to be modified.
- get sourceline, copy it to file ../tmp/selection.txt
- decode sourceline into atomicObj, keep im memory in actAto
- get all points, keep in memory in actPta.
- display points as yellow start

Modify:
- user selects point; set actPtp = pointer into DB for this point
- modify the point in DB, redraw obj with EDMPT_points(6);
  - APT_store_obj, APT_Draw__.

Save modified curve:
- save: modify the sourceline (in tempfile)

Delete:

Insert:
- user selects positon on curve; get parametric description of point,
  get segment on curve from its parameter, insert new point.


EDMPT_init
  AEDMPT_find_dep__
  EDMPT_dep_sav
  EDMPT_ptab_get
    EDMPT_get_ato
      EDMPT_rea_src
      APT_decode__


EDMPT_mousemove_CB
  EDMPT_points (6, 9);
    APT_Draw__
    EDMPT_mod_dep_draw__


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
#include "../ut/ut_geo_const.h"        // _MAT_4x3
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
#include "../xa/xa_ato.h"              // ATO_getSpc_tmp__
// #include "../xa/opar.h"                // MemTab(ObjSRC) MEMTAB_tmpSpc_get


#define   TRUE 1
#define   FALSE 0


#define EDMPT_SYM_COL   ATT_COL_RED // color for pointSymbols (star) GL_col_tab
#define EDMPT_SYM_ID    SYM_SQU_B   // ID for pointSymbols


#define DLI_TMP_POS   -2L           // position (small red circle)
#define DLI_TMP_CUR   -3L           // symbol cursor (plane|vector)
#define DLI_TMP_CV    -4L           // obj/curve
#define DLI_TMP_OFFO  -5L           // offsetObject




//================================================================
// ex ../xa/xa.c
extern int       WC_sur_ind;            // Index auf die ActiveConstrPlane
extern Plane     WC_sur_act;            // Constr.Plane
extern Mat_4x3   WC_sur_mat;            // TrMat of ActiveConstrPlane
extern Mat_4x3   WC_sur_imat;           // inverse TrMat of ActiveConstrPlane


// ex ../xa/xa_ui.c
extern MemObj    UIw_Box_TB;    // toolbarBox
extern int       KeyStatAlt;  
extern int       KeyStatShift;


// External from ../ci/NC_Main.c:
extern char       *APT_defTxt;          // die Textzeile ohne N#=


// // ex ../xa/xa_ui_gr.c
// extern int KeyM1;



typedef_MemTab(ObjDB);
// typedef_MemTab(ObjTXTSRC);



//================================================================
// LOCAL:
static MemObj PEDtb=GUI_OBJ_NEW;  // Toolbar

static int   actTyp;            // typ of obj to modify
static int   basTyp;            // basic typ of obj to modify
static long  actDbi;            // DB-index
static long  actDli;            // dispListIndex
static long  actlNr;            // lineNr of  obj to modify
static void  *actObj;           // data-struct of obj to modify

static struct {
    long irs;                    // refsys-Index (index of constrPln of actObj)
    Mat_4x3  mat1, mat2;         // matrix, inverse-matrix of plane irs
    UINT_32 dlAtt;               // dispListattribute iatt
  } EDMDAT = {
    0L,                          // irs
    _MAT_4x3, _MAT_4x3,          // mat1, mat2
    0                            // dlAtt
};

static ObjAto actAto = _ATO_NUL; // typ,tab
static ObjTXTSRC *actTxo=NULL;  // source-objects
static Point *actPta=NULL;      // pointarray original (unmod, for restore)
static int   *pMod=NULL;        // is subObj modifyable and point or value;
static int   actPtn;            // nr of points of actPta
static long  actPti;            // index of active point in DB / EDMPT_atab
static Point *actPtp;           // address of the active point in DB
static int   actInd;            // point to modify; index into actPta
// static unsigned int actAtt;     // original attribute
static long  dynPti;            // index of 1. dyn.point of point-symbols

// storage for the atomic-objects of the inputObj:
static MemTab(ObjDB) EDMPT_ptDba;

// static int    *EDMPT_atyp=NULL;   // type of objects of sourcecode to edit ..
// static double *EDMPT_atab=NULL;   // data of objects of sourcecode to edit ..
// static int    EDMPT_anr=0;        // nr of objects in EDMPT_atyp,EDMPT_atab

static Point actCurPos;         // 
static Point actInsPt;         // 

static int    EDMPT_mode=-1;      // 0=modify, 1=insert, 2=delete
static int    EDMPT_stat=0;       // 0=move is not active; 1=active;
                                 // 2=SaveCancel is active.
                                 // 3=fixed (vector + length)
static int    EDMPT_changed=0;    // 0=not-changed; nr-of-Saves

static int    EDMPT_offTyp;       // Typ_VC|Typ_PLN
static Plane  EDMPT_offObj;       // vector or plane
static double EDMPT_offLen = 0.;

static MemObj EDMPT_f_pln;    // frame Save/Cancel
static MemObj EDMPT_f_mod;    // frame Modify Indert Delet
static MemObj EDMPT_f_sc;     // frams Save/Cancel
static MemObj EDMPT_b_ex;     // button exit
static MemObj EDMPT_w_off;    // entry offsetObj
static MemObj EDMPT_f_len;    // frame length
static MemObj EDMPT_e_len;    // entry length
// static MemObj EDMPT_wa1[3];   // Radiobutts 0=Delete 1=Insert; 2=Modify; 3=Exit

static ObjSRC oLnk1;          // eg linked point to modify

static int    EDMPT_depNr;     // nr of childs of prim.obj

static FILE      *EDMPT_fp_dep = NULL;

// PROTOS:
  MemObj EDMPT_win__ (MemObj *parent, void *data);
  int EDMPT_CB1 (MemObj *mo, void **data);
  int EDMPT_off_CB (MemObj *mo, void **data);
  int EDMPT_len_CB (MemObj *mo, void **data);
  int EDMPT_mousemove_CB (int dx, int dy);
  int EDMPT_key_CB (int key);
  int EDMPT_sel_CB (int src, long dl_ind);
  Point EDMPT_newPos__ ();




//================================================================
  int EDMPT_init (int typ, long dbi, long dli) {
//================================================================
// main-entrypoint 'move_points_of_obj'
 
  int   irc, i1, form, parTyp;
  long  l1, l2, icp, parDbi;
  char  cbuf[160];
  Plane ors;

  MemTab(ObjSRC) depTab = _MEMTAB_NUL;


  // printf("EDMPT_init typ=%d dbi=%ld dli=%ld mode=%d\n",typ,dbi,dli,EDMPT_mode);

/*
  // exit if typ != curve
  if(!TYP_IS_CV(typ)) {
    TX_Print("***** selected obj is not curve ---");
    return -1;
  }
*/

  // exit if PED is already active
  // man kann EDMPT_mode in einer DLL nicht korrekt abfragen,
  // da es in der Dll eine lokale Kopie gibt.
  if(EDMPT_mode < 0) {
    // if MAN: copy editor -> memory
    AP_SRC_edi_mem ();

  } // else {
    // MSG_pri_0 ("PEDe1"); // ERROR: PED is already active ..");
    // return 0;
  // }


  //----------------------------------------------------------------
  L_ini_obj:
  // get dli if not given
  if(dli < 0) dli = DL_find_obj (typ, dbi, -1L);
    // printf(" dli=%ld\n",dli);

  // get srcLineNr
  DL_Get_lNr_dli (&actlNr, dli);
    // printf(" actlNr=%ld\n",actlNr);

  // set basic type
  basTyp = AP_typ_2_bastyp (typ);
    // printf(" basTyp=%d\n",basTyp);
    // DEB_dump_obj__ (actTyp, actObj, "actObj: ");


  // get actObj from DB (for insert points into POL, BSP)
  form = basTyp;
  irc = UTO_objDat_dbo (&actObj, &i1, &form, dbi);
  if(irc < 0) return irc;

  actDli = dli;
  actDbi = dbi;
  actTyp = form;

  // get the dispListattribute
  EDMDAT.dlAtt = DL_get_iatt (actDli);
  // get the refsys-Index
  EDMDAT.irs = DL_GetTrInd (actDli);

  // if actObj is on 2D-plane, then get the fwd- and bwd-matrix
  if(EDMDAT.irs) {
    DB_GetRef (&ors, EDMDAT.irs);
    // get mat1 = 2D->3D-matrix from plane
    UT3D_m3_loadpl (EDMDAT.mat1, &ors);
    // get mat2 = 3D->2D-matrix
    UT3D_m3_invm3 (EDMDAT.mat2, EDMDAT.mat1);
  }


    // TESTBLOCK
    // printf("############# L_ini_obj: typ=%d dbi=%ld dli=%ld irs=%ld\n",
           // typ,dbi,dli,EDMDAT.irs);
    // DEB_dump_obj__ (Typ_M4x3, EDMDAT.mat1, "EDMDAT.mat1");
    // DEB_dump_obj__ (Typ_M4x3, EDMDAT.mat2, "EDMDAT.mat2");
    // END TESTBLOCK



  // save active line into <tmpDir>/selection.txt
  sprintf(cbuf,"%sselection.txt",OS_get_tmp_dir());
  UTF_wrf_line (actlNr, cbuf);


  // // activate ConstrPlaneNr for this object (resets automatic)
  // DL_setRefSys (actDli);  2019-04-28


  // find all depending objects (childs)
  APED_find_dep__ (&depTab, actTyp, actDbi);
    // MemTab_dump (&depTab, "  EDMPT_init-_find_dep__");


  // write sourcecodes of all depending objects -> file <tmpDir>/src_dep.txt
  irc = EDMPT_dep_sav (&depTab);
  if(irc < 0) return irc;

  EDMPT_depNr = depTab.rNr;
    // printf(" EDMPT_init-depNr = %d\n",EDMPT_depNr);


  MemTab_free (&depTab);        // free mem
      // printf(" free depTab\n");


  // get all points of actObj into actPta[actPtn]
  // get all atomicObjs into EDMPT_atyp,EDMPT_atab[EDMPT_anr]
  irc = EDMPT_ptab_get ();  // return nr of points
  if(irc < 0) {MSG_pri_0 ("PEDnopt"); return -1;}
  MSG_pri_1 ("PEDnrpt", "%d", actPtn);  // Objekt hat # verschiebbare Punkte


    // TESTBLOCK
    // EDMPT_dump_ptab (); // dump actPta[actPtn]
    // END TESTBLOCK


  // if actObj is on 2D-plane, then transfer points into 3D
  if(EDMDAT.irs > 0) EDMPT_ptab_3D_2D ();             // see AP_IS_2D


  // copy selection.txt -> <tmpDir>/selection1.txt
  EDMPT_src_save (0);

  if(actPtn > 0) goto L_init_ui;


  //----------------------------------------------------------------
  // test if inputObject has a parent; if yes: use the parentObj to modify !
  //  but if inputObject has more than 1 parents ?
  if(DL_parent_ck_c (dli)) {
      // printf(" .. has parent !\n");
    {
      ObjSRC oPar;
      MemTab(ObjSRC) mtPar = _MEMTAB_NUL;
      // get ObjSRC from DL_att:
      DL_oSrc_dli (&oPar, dli);
      // get tempSpc for 128 mtPar-records
      MEMTAB_tmpSpc_get (&mtPar, 128);
      if(MEMTAB_RMAX(&mtPar) != 128) {TX_Error("*** EDMPT_init-EOM"); return -1;}
      // get parent
      OPAR_get_src (&mtPar, &oPar);
      // can provide more than 1 parent; select 1. parent
      if(mtPar.rNr < 1) goto L_par_err;
      APED_oid_dbo__ (&cbuf[0], typ, dbi);
      typ = (MEMTAB__(&mtPar, 0))->typ;
      dbi = (MEMTAB__(&mtPar, 0))->dbi;
      dli = (MEMTAB__(&mtPar, 0))->dli;
      APED_oid_dbo__ (&cbuf[16], typ, dbi);
      sprintf(memspc011,"*** using parent %s of selected object %s ..",
              &cbuf[16], &cbuf[0]);
      TX_Print(memspc011);
      MEMTAB_tmpSpc_free (&mtPar);
      goto L_ini_obj;

      L_par_err:
        TX_Print("*** EDMPT_init-ERR-dli %ld ",dli);
        MEMTAB_tmpSpc_free (&mtPar);
        return -1;
    }
  }



  //----------------------------------------------------------------
  L_init_ui:
    // printf(" L_init_ui:\n");

  // test if gui already exists
  if(EDMPT_mode >= 0) goto L_init_ini;

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


  AP_UserKeyIn_get     (EDMPT_key_CB);         // attach keystrokes
  AP_UserSelection_get (EDMPT_sel_CB);         // attach grafic selections
  AP_UserMousemove_get (EDMPT_mousemove_CB);   // attach mousemovements
  UI_block_group (1);                        // block groupSelections




  //----------------------------------------------------------------
  // init PED-toolbar
  if(GUI_OBJ_IS_VALID(&PEDtb)) {
    // GUI_obj_view (PEDtb);
      // printf(" ---------- show PEDtb\n");
    GUI_set_show (&PEDtb, 1);

  } else {
    PEDtb = EDMPT_win__ (&UIw_Box_TB, NULL);
  }


  //----------------------------------------------------------------
  L_init_ini:
  // activate/deactivate Insert/Delete according to type of sel. obj
  // EDMPT_win_act ();


  // init pointEditor, display points
  // DL_pick__ (0);            // make all objects unpickable
  EDMPT_points (1, 0);        // draw points

  sele_reset ();

  EDMPT_sel__ (0); // enable selection of Plane + point

  sele_setNoConstrPln ();
  // GL_vcSel_init (-1, 1);    // enable VectorSelector


  // init Plane
  // if actObj is on 2D-plane, then preset this plane as offsetPlane
  if(EDMDAT.irs) {
    EDMPT_offTyp = Typ_PLN;
    APED_oid_dbo__ (cbuf, Typ_PLN, EDMDAT.irs);
    GUI_entry_set (&EDMPT_w_off, cbuf);
  } else {
    EDMPT_offTyp = Typ_PLN;
    GUI_entry_set (&EDMPT_w_off, "RZ");
  }


  // get EDMPT_offObj = offsetObject (Plane or Vector)
  EDMPT_get_offObj ();  


  // redraw modify-symbol
  EDMPT_mousemove_CB (0,0);

  EDMPT_mode = 0;

  EDMPT_msg_mode ();


/*
// autoselect ...  geht ned ...
  // DL_DumpObjTab ();
  if(actPtn < 2) {
    // EDMPT_get_offObj ();
    // l1 = DL_get__ (NULL) - 1;
    // UI_GR_set_sel (l1);
    l1 = GL_GetActInd ();
    l2 = DL_get_dbi (l1);                     // get dl.ind
    UI_GR_set_selNam (Typ_SymB, l2, "");      // simulate selection
    EDMPT_sel_CB (GUI_MouseL, l1);
    GUI_update__ ();
    DL_Redraw ();
  }
*/


    // printf("ex-EDMPT_init mode=%d\n",EDMPT_mode);

  return 0;

}


//================================================================
  int EDMPT_key_CB (int iKey) {
//================================================================

  // printf("EDMPT_key_CB %d\n",iKey);
  // printf("EDMPT_stat=%d EDMPT_mode=%d actInd=%d\n",EDMPT_stat,EDMPT_mode,actInd);


  switch (iKey) {

    case GUI_KeyEsc:
    case 'q':
    case 's':
        // DL_DumpObjTab ();
      // printf(" Esc: md=%d stat=%d\n",EDMPT_mode,EDMPT_stat);

      if(EDMPT_stat == 0) {           // Save|Cancel is active
        EDMPT_CB1 (GUI_ES("Exit"));
      }

      if(EDMPT_stat == 1) {         // 1=move is active
        if(EDMPT_mode == 0) {       // 0=modify
          EDMPT_upd_stop ();        // stop modify-update
        }
      }

/*
      if(EDMPT_stat == 2) {           // Save|Cancel is active
        if(key == 's') EDMPT_CB1 (NULL, GUI_SETDAT_ES(TYP_EventPress,"Save"));
        else           EDMPT_CB1 (NULL, GUI_SETDAT_ES(TYP_EventPress,"Canc"));
      }
*/

      EDMPT_msg_mode ();

      return TRUE;  // do no defaultOperations


    default:
      break;
  }



  if(KeyStatAlt == ON) {

    if(KeyStatShift == ON) {UI_key_spcShAlt (iKey); goto L_exit;}

    // do special-function with Alt ON
    UI_key_spcAlt (iKey);
  }





  L_exit:
    // printf(" exit EDMPT_key_CB\n");
  return FALSE;  // do defaultOperations

}

 
//=========================================================
  int EDMPT_sel_CB (int src, long dl_ind) {
//=========================================================
// userselection callback

  int    irc, styp, typ;
  long   dbi, dli;
  char   *cp1;



  // printf("EDMPT_sel_CB src=%d ind=%ld\n",src,dl_ind);
  // printf("  EDMPT_mode=%d, EDMPT_stat=%d\n",EDMPT_mode,EDMPT_stat);
  // printf("  actTyp=%d actDbi=%ld actInd=%d\n",actTyp,actDbi,actInd);
  // printf("  EDMPT_changed=%d\n",EDMPT_changed);
  // printf("  EDMPT_offTyp=%d\n",EDMPT_offTyp);


  // if(iAct) return 0;
  // iAct = 1;



  // save &exit m. rechter maustaste (see also EDMPT_key_CB)
  if(src == GUI_MouseR) {
    // if(dl_ind >= 0) return 0;     // skip obj sel with right mousebutt.
    EDMPT_key_CB ('s');             // exit ..
    return 0;
  }


  

  // query selected DB-obj
  if(dl_ind >= 0) {
    DBO_sel__ (&styp, &dbi, dl_ind);
      // printf(" sel styp=%d dbi=%ld\n",styp,dbi);

    // analyze curvepoint - get text
    UI_GR_get_selNam (&typ, &dbi, &cp1);
      // printf(" %d %ld selNam |%s|\n",typ, dbi,cp1);
  }




  //----------------------------------------------------------------
  if(EDMPT_stat == 3) {           // fixed
    if(typ != Typ_SymB) { printf(" EDMPT_sel_CB E-3-1\n"); return -1;}
    EDMPT_points (5, dbi);      // set pointIndex actInd & actPtp
    *actPtp = EDMPT_newPos__ ();   // translate
    // EDMPT_curSym_set (actPtp);  // reposition the cursorSymbol
    EDMPT_points (6, 9);        // redraw actObj hilited
    EDMPT_points (3, EDMPT_SYM_COL); // update pointPositions (redraw pt actInd)
    return 0;
  }


  //----------------------------------------------------------------
  if((EDMPT_mode == 1)  ||                     // 1=insert, 2=delete
     (EDMPT_mode == 2))      { 
    // insert | delete - position on curve selected
   

    // insert: get actInsPt = pointcoords of selected point at curve
    // and set symbol at actInsPt
    if(EDMPT_mode == 1) {
      irc = EDMPT_ins_pt ();
      if(irc < 0) return -1;
    }

    // delete:
    if(EDMPT_mode == 2) {
      // ignore selection (not point-symbol)
      if(dl_ind < 0) return 0;
      // set pointIndex actInd & actPtp
      EDMPT_points (5, dbi);
    }

    // update source of active obj in file
    EDMPT_save__ ();

    // update actPta pMod actAto actTxo from selection.txt
    EDMPT_ptab_get ();

    // disp points
    EDMPT_points (1, 0);

    // update curve
    EDMPT_points (6, 0);

    if(EDMPT_mode == 1) {
      dli = DLI_TMP_POS;
      GL_DrawSymB (&dli, EDMPT_SYM_COL, SYM_STAR_S, &actInsPt);
    }

    // activate Save/Cancel
    // EDMPT_upd_stop ();    // EDMPT_stat_set (2);
    // lock delete,insert,modify,exit; unlock Save|cancel
    EDMPT_lock ();

    return 0;
  }


  //----------------------------------------------------------------
  if(EDMPT_mode == 0) {           // modify plane or vector
    if(EDMPT_stat == 1) {         // updating
      // stop modify-update
      EDMPT_upd_stop ();      
      EDMPT_msg_mode ();
      return 0;
    }
  }


  //----------------------------------------------------------------
  // start move-update-cycle (yellow-star-symbol selected):
  if(typ == Typ_SymB) {     // curvepoint selected

    // skip selection in Save/Cancel-state
    if(EDMPT_stat == 2) {
      if(EDMPT_mode != 0) {
        MSG_pri_0 ("PEDsav");
        return 0;
      }
    }

    if(EDMPT_mode == 0) {   // modify
      if(EDMPT_offTyp == Typ_Error) {
        MSG_pri_0 ("PEDe2");
        return 0;
      }
    }

    // end move-update-cycle
    // start Save/Cancel-state
    EDMPT_points (5, dbi);      // set pointIndex actInd & actPtp
    if(actInd < 0) return 0;

    GUI_set_enable (&EDMPT_f_pln, FALSE);   // lock pln,vec,len

    EDMPT_sel__ (1); // enable selection of point only

    // modify ----------------------------
    if(EDMPT_mode == 0) { 
      // test if EDMPT_offTyp=vektor & length is given
      if(EDMPT_offTyp == Typ_VC) {
        EDMPT_offObj.po = *actPtp;
        if(EDMPT_offLen != 0.) {
          EDMPT_stat = 1; //2;
          EDMPT_mousemove_CB (0,0);   // start - draw curve
          EDMPT_upd_stop ();
          return 0;
        }
      // } else {
        // if(actPtp) UT3D_pl_ptpl (&EDMPT_offObj, actPtp);
      }
        // DEB_dump_obj__(EDMPT_offTyp, &EDMPT_offObj, "EDMPT_offObj:");

      // start modify-curve
      // EDMPT_mousemove_CB (0,0);          // start - draw curve
      // DL_Redraw ();
      // disactivate main-functions delete insert modify
      GUI_set_enable (&EDMPT_f_mod, FALSE);
      // disable select point-symbol
      // sele_reset ();                        // no point selectable
      // UI_GR_Sel_Filter (1);              // no selections; give position
      // activate modify-curve
      EDMPT_stat = 1;                        // 0=move is not active; 1=active


    // delete ----------------------------
    } else if(EDMPT_mode == 2) {
        printf(" del pt[%d]\n",actInd);
      EDMPT_del__ ();
      // EDMPT_test__ ();
    }

    return 0;
  }


  //----------------------------------------------------------------
  // only for modify: accept Plane|Vector -> EDMPT_w_off
  if(EDMPT_mode == 0)      {        // modify
    if((typ == Typ_PLN)||(typ == Typ_VC))    {
      GUI_entry_set (&EDMPT_w_off, cp1);
      EDMPT_off_CB (NULL, NULL);    // update
    } else {
      MSG_pri_0 ("PEDerrSel");
    }
  }


  // obj with wrong type selected ..
  EDMPT_msg_mode ();


  return 0;

}


//================================================================
  int EDMPT_ptab_3D_2D () {
//================================================================
// EDMPT_ptab_3D_2D         actObj is on 2D-plane. Transfer points into 3D
// transfer the SRC-coords (constrPln)  -->> DB-coords (world)
// Input:
//   actPtn   global    nr of points
//   actPta   global    points

  int    i1;

  printf("EDMPT_ptab_3D_2D actPtn=%d\n",actPtn);
  DEB_dump_nobj__ (Typ_PT, actPtn, actPta, "EDMPT_ptab_3D_2D");


  for(i1=0; i1<actPtn; ++i1) {
    UT3D_pt_tra_pt_m3 (&actPta[i1], EDMDAT.mat1, &actPta[i1]);
  }

    DEB_dump_nobj__ (Typ_PT, actPtn, actPta, "ex-EDMPT_ptab_3D_2D");

  return 0;

}

 
//================================================================
  int EDMPT_obj_get () {
//================================================================
// decode actObj/actAto, store in DB, reload (bin.obj) actObj
// (get binary obj actObj from actAto and the (modified) DB-points)

  int    irc, i1, form;


  APT_store_obj (&basTyp, &actDbi, actAto.nr, actAto.typ, actAto.val);
    // printf(" EDMPT_obj_get-basTyp=%d actTyp=%d actDbi=%ld\n",
            // basTyp,actTyp,actDbi);

  // get actObj from DB (for insert points into POL, BSP)
  basTyp = AP_typ_2_bastyp (actTyp);
  form = basTyp;
  irc = UTO_objDat_dbo (&actObj, &i1, &form, actDbi);
  if(irc < 0) return irc;

  return 0;

}

 
//================================================================
  int EDMPT_msg_mode () {
//================================================================

  // printf("EDMPT_msg_mode \n");


  if(EDMPT_mode == 0)      {    
    if(basTyp == Typ_CV) {
      MSG_pri_0 ("PEDmod1");
    } else {
      MSG_pri_0 ("PEDmod2");
    }
  } else if(EDMPT_mode == 1)   MSG_pri_0 ("PEDins");
  else if(EDMPT_mode == 2)     MSG_pri_0 ("PEDdel");

  return 0;

}


//================================================================
  int EDMPT_curSym_set (Point *pc) {
//================================================================
// EDMPT_curSym_set   set symbor cursor (plane|vector)
// Input:
//   EDMPT_offTyp        Typ_PLN|Typ_VC|Typ_Error
// actCurPos

                     
  long      dli = DLI_TMP_CUR;


  // printf("EDMPT_curSym_set %d %f\n",EDMPT_offTyp,EDMPT_offLen);
  // DEB_dump_obj__ (Typ_PT, pc, "  curSym_set-pc: ");

  // UI_GR_get_actPosA (&actCurPos);   // get GR_CurUk
  // UTRA_pt_ucs2wcs (&actCurPos);

  


  if(EDMPT_offTyp == Typ_VC) {
    //----------------------------------------------------------------
    // vector
    if(EDMPT_offLen == 0.) {
      // vector-move
      // GL_DrawVc1 (&dli, EDMPT_SYM_COL, &actCurPos, &EDMPT_offObj.vz);
      GR_Draw_vc (&dli, &EDMPT_offObj.vz, pc, EDMPT_SYM_COL, 0);


    } else {
      // vector-fixed
      GL_DrawVec (&dli, EDMPT_SYM_COL, pc, &EDMPT_offObj.vx);
    }



  } else {
    //----------------------------------------------------------------
    // plane
    GL_DrawSymV3 (&dli,SYM_SQUARE,EDMPT_SYM_COL,&actCurPos,&EDMPT_offObj.vz, 1);
  }


  L_exit:

  return 0;

}


//=========================================================
  int EDMPT_mousemove_CB (int dx, int dy) {
//=========================================================
// callback mousemovement
// modifys the active point (actPta[actInd])



  // printf("------------------------------------------ \n");
  // printf("EDMPT_mousemove_CB dx,dy=%d %d _mode=%d _stat=%d _offTyp=%d\n",
         // dx,dy,EDMPT_mode,EDMPT_stat,EDMPT_offTyp);
  // printf(" move_CB-actInd=%d actPti=%ld\n",actInd,actPti);



  // check if initialized ..
  if(EDMPT_offTyp == Typ_Error) {
    // MSG_pri_0 ("PEDe2");
    return 0;
  }

  // skip fixed
  if(EDMPT_mode == 3) goto L_exit;

  // stat must be 1 (1=move-is-active)
  if(EDMPT_stat != 1) goto L_exit;

  // check if curve is initialized ..
  if(actTyp == 0) goto L_exit;   // typ of curve

  // check if point to move is initialized ..
  if(actInd < 0) {EDMPT_msg_mode (); goto L_exit;}


  // UI_GR_get_actPosA (&actCurPos);   // get GR_CurUk
  // UTRA_pt_ucs2wcs (&actCurPos);


  // overwrite the active DB-point
  *actPtp = EDMPT_newPos__ ();
    // DEB_dump_obj__ (Typ_PT, actPtp, "  actPtp-2:");

  // reposition the cursorSymbol
  EDMPT_curSym_set (actPtp);

  // redraw position (small circ) at newPos actPtp (plane | vector)
  if(EDMPT_mode == 0) EDMPT_points (4, 0);

  // redraw actObj hilited and its child-objects
  EDMPT_points (6, 9);


  L_exit:
  DL_Redraw ();

  return 0;

}


//================================================================
  int EDMPT_upd_stop () {
//================================================================
// exit modify-update proc

  // printf("EDMPT_upd_stop \n");

  GL_temp_del_1 (DLI_TMP_CUR);   // delete temp-vec or plane

  if(EDMPT_mode == 0) {
    EDMPT_points (3, EDMPT_SYM_COL); // update pointPositions
  }

  MSG_pri_0 ("PEDmod3");
  EDMPT_stat = 2;                  // stop update

  GUI_set_enable (&EDMPT_f_sc, TRUE);      // activate Save|cancel
  GUI_set_enable (&EDMPT_b_ex, FALSE);     // disactivate Exit

  // EDMPT_points (10, ATT_COL_DIMMED);   // hide all points

  return 0;

}


//================================================================
  Point EDMPT_newPos__ () {
//================================================================
// EDMPT_newPos__     get cursorposition (mode=0; projected onto plane|vector)
// returns the new active-point-position

  double   d1;
  Point    *pt1, ptx, pto;
  Vector   *vc1, vc2;


  // printf("EDMPT_newPos__ \n");
  // printf("  EDMPT_offTyp=%d\n",EDMPT_offTyp);
  // DEB_dump_obj__(EDMPT_offTyp, &EDMPT_offObj, "  EDMPT_offObj:");


  if(EDMPT_stat == 3) {     // fixed: add vector to active-point-position
    UT3D_pt_traptvclen (&pto, actPtp, &EDMPT_offObj.vz, EDMPT_offLen);
    goto L_exit;
  }


  // get actCurPos = eye-point
  UI_GR_get_actPosA (&actCurPos);   // get GR_CurUk
  UTRA_pt_ucs2wcs (&actCurPos);
    // DEB_dump_obj__ (Typ_PT, &actCurPos, "  EDMPT_newPos__-inPos: ");

  // get vc2 =  eye-vector
  vc2 = GL_GetEyeX ();
    // DEB_dump_obj__ (Typ_VC, &vc2, "  eyeVec: ");



  //================================================================
  // intersect with vector
  if(EDMPT_offTyp == Typ_VC) {

    // project curPos onto line from actPos along vector
    // actPos = actPtp


    // // intersect eyeLine X offsetLine
    // UT3D_pt_int2pt2vc (&pto, &ptx, &d1, &EDMPT_offObj.po, &EDMPT_offObj.vz,
                                          // &actCurPos, &vc2);


  
  //================================================================
  // intersect with plane ..
  } else if(EDMPT_offTyp == Typ_PLN) {

/*
    if(WC_sur_ind) {
      // a RefSys is active ..
      pto = actCurPos;
    } else {
*/
      // intersect eyeVector - plane                       2010-10-07
      UT3D_pt_intptvcpl_ (&pto, &EDMPT_offObj, &actCurPos, &vc2);
    // }


  } else {
    TX_Error("EDMPT_newPos__ E1");
    return actCurPos;
  }


  L_exit:
    // printf("ex EDMPT_newPos__ %f %f %f\n",pto.x,pto.y,pto.z);

  return pto;

}

//================================================================
  int EDMPT_points (int mode, int ii) {
//================================================================
// point-editor: draw points, delete points
// mode=1    draw points (init)
// mode=2    delete; ii=1: all for exit; ii=0: only points
// mode=3    redraw point with index actInd
// mode=4    create temporary red circ at actPtp
// mode=5    set pointIndex actInd
// mode=6    redraw curve and its child-objects 
// mode=7    redraw all temporary points
// mode=8    delete temporary point actInd
// // mode=9    restore original points (if Links were broken)
// // mode=10   remove all pointSymbols (yellow rect's) in DL


  static long   startInd = -1L;
  int    i1, i2, i3, iatt;
  long   dli, l1;
  char   sBuf[200], sNam[32];
  ObjGX  ox1, *oxp;


  // printf("EDMPT_points %d %d\n",mode,ii);
    // EDMPT_dump_atab ();
    // EDMPT_dump_ptab ();



  //----------------------------------------------------------------
  if(mode != 1) goto L_mode2;  // 1 = init; draw points
  startInd = GL_Get_DLind();
  EDMPT_points (7, EDMPT_SYM_COL);          // draw all temp. points
  // EDMPT_points (6, 9);          // display curve to modify temporary
  actInd = -1;
  // TX_Print("select point (yellow star) to modify ...");
  goto L_exit;


  //----------------------------------------------------------------
  L_mode2:                     // mode=2    delete all for exit
    if(mode != 2) goto L_mode3;
    if(startInd >= 0) {
      // printf(" ... delete von %ld\n",startInd);
      GL_temp_del_1 (DLI_TMP_POS);     // position-circ
      GL_temp_del_1 (DLI_TMP_CV);      // curve
      GL_Delete (startInd);  // delete all points (yellow rect's in DL)
    }
    if(ii) {
      GL_temp_del_1 (DLI_TMP_CUR);     // plane|vec
      GL_temp_del_1 (DLI_TMP_OFFO);    // offsetObject
    }
    goto L_exit;


  //----------------------------------------------------------------
  L_mode3:                           // mode=3    delete point index actInd
    if(mode != 3) goto L_mode4;
    GL_temp_del_1 (DLI_TMP_POS);  // delete temp circ
    // redraw at new PointPosition
    if(actInd >= 0) {
      dli = startInd + actInd;
        // printf(" startInd=%ld actInd=%d\n",startInd,actInd);
      DL_SetInd (dli);               // overwrite this DL-record
      APT_disp_SymB (EDMPT_SYM_ID, ii, actPtp);  // replace point
    }
    DL_Redraw ();
    goto L_exit;


  //----------------------------------------------------------------
  L_mode4:             // mode=4    create temporary pointSymbol -3 at actPtp
    if(mode != 4) goto L_mode5;
    dli = DLI_TMP_POS;
    // GL_DrawSymB (&dli, 2, SYM_CIR_S, &actPta[actInd]);
    GL_DrawSymB (&dli, EDMPT_SYM_COL, SYM_CIR_S, actPtp);
    goto L_exit;


  //----------------------------------------------------------------
  L_mode5:             // mode=5    set pointIndex actInd;
                       // input: ii=neg.DL-Index of new point
    if(mode != 5) goto L_mode6;
    // get i2 = pointNr (index into pMod)
    i2 = -ii - startInd;
    i1 = EDMPT_get_tabInd (i2);
    if(i1 < 0) return -1;
    actPti = actAto.val[i1];                  // get index into EDMPT_atab
    actInd = i2;                            // index into actPta
      // printf(" actPti = %ld actInd=%d\n",actPti,actInd);
    actPtp = DB_get_PT (actPti);            // get DB-index of point
      // DEB_dump_obj__(Typ_PT, actPtp, "  actPtp:");

/* 2019-04-28
    // fix plane (set point = planeOrigin)
    if(EDMPT_mode == 0) {
      if(EDMPT_offTyp == Typ_PLN) {
        UT3D_pl_ptpl (&EDMPT_offObj, actPtp);
          // DEB_dump_obj__(EDMPT_offTyp, &EDMPT_offObj, "EDMPT_offObj:");
      }
    }
*/
    // warn if link exists for that point
    // if(actPti >= 0) {
    if(pMod[actInd] != 1) {
      EDMPT_linked__ ();
      goto L_exit;
    }
      // printf(" actInd=%d startInd=%d\n",actInd,startInd);

    // change active point into yellow star; not for fixed
    if(EDMPT_mode == 0) {
      if(EDMPT_stat != 3) { 
        dli = startInd + actInd;
        DL_SetInd (dli);             // overwrite
        APT_disp_SymB (SYM_STAR_S, EDMPT_SYM_COL, &actPta[actInd]);
      }
    }

    if(EDMPT_mode == 0) MSG_pri_0 ("PEDmov");
    goto L_exit;


  //----------------------------------------------------------------
  L_mode6:         // mode=6    redraw curve and its child-objects with attribut ii
    if(mode != 6) goto L_mode7;
    
    WC_set_obj_stat (1);    // all displays done by WC_Work__ temporary

    DB_CSEG__ (0);          // save DB_CSEG-state
    

    // decode obj from atomic (EDMPT_atyp, EDMPT_atab) and store in DB.
    // actAto points to the dynamic-points in DB;
    // APT_store_obj gets this points, build new struct eg Line 
    //   and stores this updated binary obj in DB. 
    // APT_store_obj (&basTyp, &actDbi, actAto.nr, actAto.typ, actAto.val);
      // printf(" basTyp=%d actTyp=%d actDbi=%ld\n",basTyp,actTyp,actDbi);
    // decode actAto, store in DB, reload (bin.obj) actObj
    EDMPT_obj_get ();


    if(AP_errStat_get ()) {
        printf(" reset err ..\n");
      // AP_errStat_set (0);    // reset error
      AP_errStat_reset (1);     // reset error
      return 0;
    }


    // set DL_ind_act (overwrite temporary)
    DL_SetInd (DLI_TMP_CV);

    // display temp. modified DB-curve <actDbi>
    iatt = 9; // hilited ..
    // iatt = actAtt;
    APT_Draw__ (iatt, basTyp, actDbi);
      // printf(" L_mode6-draw iatt=%d %d %ld\n",iatt,basTyp,actDbi);


    // redraw all depending objects
    EDMPT_mod_dep_draw__ ();

    DB_CSEG__ (1);                     // reset DB_CSEG
    DB_dyn__ (2, Typ_PT, dynPti);      // reset state of dyn-points

      // EDMPT_dump_atab ();
      // EDMPT_dump_ptab ();

    // DL_DumpObjTab ();
    DL_Redraw ();

    goto L_exit;



  //----------------------------------------------------------------
  L_mode7:             // 7 = redraw all temporary points - yellow rectangles
    if(mode != 7) goto L_mode8;
    l1 = startInd;
      // DL_DumpObjTab ();
      // printf(" startInd=%d actPtn=%d\n",startInd,actPtn);

    // points get DL-record; DL-index is (startInd + pointIndex)
    // set "Symbols-on-top"
    GL_att_OnTop_set (1);
    for(i1=0; i1<actPtn; ++i1) {
        // DEB_dump_obj__ (Typ_PT, &actPta[i1], "   actPta[%d]",i1);
      APT_disp_SymB (EDMPT_SYM_ID, EDMPT_SYM_COL, &actPta[i1]);
    }
    DL_Redraw ();  // resets "Symbols-on-top"


    goto L_exit;


  //----------------------------------------------------------------
  L_mode8:                 // delete temporary point actInd
    // if(mode != 8) goto L_mode9;
    GL_Del1 (startInd + actInd);
    goto L_exit;

/*
  //----------------------------------------------------------------
  L_mode9:          // mode=9    restore original points (for cancel)
    if(mode != 9) goto L_mode10;
    i2 = 0;
    for(i1=0; i1<actAto.nr; ++i1) {
      if(actAto.typ[i1] != Typ_PT) continue;
      l1 = actAto.val[i1];
        // DEB_dump_obj__ (Typ_PT, &actPta[i2], " save actPta %d -> %d",i2,l1);
      DB_StorePoint (l1, &actPta[i2]);
      ++i2;
    }


  //----------------------------------------------------------------
  L_mode10:   // remove all pointSymbols (yellow rect's) in DL
    // points get DL-record; DL-index is (startInd + pointIndex)
    GL_Delete (startInd);
    // DL_Redraw ();
*/

  //----------------------------------------------------------------
  L_exit:
    // printf("ex-EDMPT_points\n");

  return 0;

}


//================================================================
  int EDMPT_src_get_src (char **txso) {
//================================================================
// get unmodif. source of active obj; skip, defHdr; cut objName

  int   irc, siz;


  irc = EDMPT_src_get__ (txso, &siz);    // get unmodif. sourcecode of active-line
  if(irc < 0) {TX_Print("EDMPT_src_get_src E001"); return -1;}

  irc = APED_defDbo_skip (txso);  // skip definition-header
  if(irc < 0) {TX_Print("EDMPT_src_get_src E002"); return -1;}

  APED_onam_cut (*txso, NULL);      // cut off objName

  return 0;

}

 
//================================================================
  int EDMPT_src_get__ (char **txso, int *bufSiz) {
//================================================================
// get new SRC out of file selection.txt

  int    i1;

  // get memspc
  *txso = mem_cbuf1;  // mem_cbuf1
  *bufSiz = mem_cbuf1_SIZ;

  // get sourceObj into cBuf
  return EDMPT_rea_src (*txso, *bufSiz);

}


//================================================================
  int EDMPT_src_save (int mode) {
//================================================================
// EDMPT_src_save     copy selection.txt > selection1.txt or back
// mode = 0    save    - copy selection.txt  -> selection1.txt
// mode = 1    restore - copy selection1.txt -> selection.txt

  char   f1[160], f2[160];

  sprintf(f1,"%sselection.txt",OS_get_tmp_dir());
  sprintf(f2,"%sselection1.txt",OS_get_tmp_dir());

  if(!mode) OS_file_copy (f1, f2);
  else      OS_file_copy (f2, f1);

  return 0;

}


//================================================================
  int EDMPT_linked__ () {
//================================================================
// EDMPT_linked__     point <actPti> is linked to parentObj ..
// 

  int      i1, typ;
  long     dbi;
  char     sNam[32], sBuf[200], *txso;
  

   printf("EDMPT_linked__ actInd = %d\n",actInd);


   if(actPti >= 0) {
      // positiv: DB-point
      typ = Typ_PT;
      dbi = actPti;

   } else {
     // get unmodif. source of active obj; skip, defHdr; cut objName
     EDMPT_src_get_src (&txso);

     // get parent; let select if more than 1
     i1 = APED_txo_find_parent (&typ, &dbi, actTxo, txso, actInd);
     if(i1 < 0) return -1;
   }



      oLnk1.typ = typ;
      oLnk1.dbi = dbi;


      // MSG_get_1 (sBuf, 200, "PEDlink", "%s", sNam);
      // Dialog: modify parent ... or break link <sNam>
      // GUI_DialogYN (sBuf, EDMPT_link_CB);     // "break link ?"


      APED_oid_dbo__ (sNam, typ, dbi);  // get name, eg "P25"

      sprintf(sBuf," - this point is linked to %s -\n"
                   " - store modifications and modify %s ? ",sNam,sNam);
      i1 = GUI_Dialog_2b (sBuf, "YES", "NO");  // 0=yes
      if(i1 == 0) {
        EDMPT_save__ ();             // save
        EDMPT_restart_lnk ();
      }

      actInd = -1;  // exit in EDMPT_sel_CB

  return 0;

}


//================================================================
  MemObj EDMPT_win__ (MemObj *parent, void *data) {
//================================================================
// create Toolbar

  static  MemObj tbc = GUI_OBJ_NEW;

  MemObj  box0, box1, w1;


  printf("EDMPT_win__ \n");

  if(GUI_OBJ_IS_VALID(&tbc)) goto L_exit;
    
  tbc = GUI_toolbox__ (parent);

  box1 = GUI_box_h (&tbc, "");

  GUI_label_htm__ (&box1, "<b> CurveEditor: </b>", "");


  GUI_spc_h (&box1, 5);
  GUI_button__ (&box1, MSG_const__(MSG_help), EDMPT_CB1, (void*)"Help", "");
  GUI_sep__ (&box1, 1, 5);


  //----------------------------------------------------------------
  // Radiobuttons delete insert modify
  EDMPT_f_mod = GUI_box_h (&box1, "");
  GUI_radiobutt__
         (&EDMPT_f_mod, MSG_const__(MSG_modify), 0, EDMPT_CB1, (void*)"Mod", "");
  GUI_radiobutt__
         (&EDMPT_f_mod, MSG_const__(MSG_delete), 1, EDMPT_CB1, (void*)"Del", "");
  GUI_radiobutt__
         (&EDMPT_f_mod, MSG_const__(MSG_insert), 2, EDMPT_CB1, (void*)"Ins", "");

  // if(actAto.nr < 3)
    // GUI_set_enable (&EDMPT_f_mod, FALSE);  // mod,ins,del = OFF



  //----------------------------------------------------------------
  EDMPT_f_pln = GUI_box_h (&box1, "");
/*
  // radiobuttons plane / vec, entry plane/vector
  EDMPT_wa1[0] = GUI_radiobutt__
           (&EDMPT_f_pln, "plane",               0, EDMPT_CB1, (void*)"Pln", "");
  EDMPT_wa1[1] = GUI_radiobutt__
           (&EDMPT_f_pln, "vector",              1, EDMPT_CB1, (void*)"Vec", "");
*/

  // entry offsetObj (plane or vector)
  EDMPT_w_off = GUI_entry__(&EDMPT_f_pln, "  ", "", 
                          EDMPT_off_CB, NULL, "8");

  // entry vectorLength
  EDMPT_f_len = GUI_box_h (&box1, "");
  EDMPT_e_len = GUI_entry__(&EDMPT_f_len, MSG_const__(MSG_length),"",
                          EDMPT_len_CB, NULL, "8");
  GUI_set_enable (&EDMPT_f_len, FALSE);


  GUI_spc_h (&box1, 5);
  GUI_sep__ (&box1, 1, 5);



  //----------------------------------------------------------------
  // buttons Save/Cancel
  GUI_spc_h (&box1, 5);
  EDMPT_f_sc = GUI_box_h (&box1, "");
  GUI_button__ (&EDMPT_f_sc, MSG_const__(MSG_save), EDMPT_CB1,  (void*)"Save","");
  GUI_Tip (MSG_get_0("PEDsav"));

  GUI_button__ (&EDMPT_f_sc, MSG_const__(MSG_cancel), EDMPT_CB1,(void*)"Canc","");
  GUI_Tip (MSG_get_0("PEDcanc"));

  GUI_set_enable (&EDMPT_f_sc, FALSE);  // Save,Cancel = OFF


  GUI_spc_h (&box1, 5);
  GUI_sep__ (&box1, 1, 5);


  //----------------------------------------------------------------
  // button exit
  EDMPT_b_ex = GUI_button__
          (&box1, MSG_const__(MSG_exit), EDMPT_CB1, (void*)"Exit", "");
  GUI_spc_h (&box1, 5);


  L_exit:

  EDMPT_mode = 0;

  return tbc;

}


/*
//================================================================
  int EDMPT_win_act () {
//================================================================
// EDMPT_win_act      activate/deactivate Insert/Delete depending on baseObj

  if(AP_typ_2_bastyp (actTyp) == Typ_CV) {
    GUI_set_enable (&EDMPT_wa2[0], TRUE);
    GUI_set_enable (&EDMPT_wa2[1], TRUE);

  } else {
    GUI_set_enable (&EDMPT_wa2[0], FALSE);
    GUI_set_enable (&EDMPT_wa2[1], FALSE);

  }

  GUI_ckbutt_set (&EDMPT_wa2[2], 1);   // -> ON


  // if Modify: activate ?; else deactivate.
  GUI_set_enable (&EDMPT_f_mod, TRUE);

  return 0;

}
*/

//=====================================================================
  int EDMPT_CB1 (MemObj *mo, void **data) {
//=====================================================================

  int    i1, form;
  long   l1;
  char   *cp1, cbuf1[256];
  Vector *vcz;
  

  // skip disactivation of radiobutton
  if(GUI_DATA_EVENT == TYP_EventRelease) return 0;

  
  // cp1 = (char*)data;
  cp1 = GUI_DATA_S1;


  // printf("EDMPT_CB1 |%s| %d %d\n",cp1,actAto.nr,EDMPT_mode);


  // skip startup
  if(EDMPT_mode < 0) return 0;

  // if(actAto.nr < 1) return 0;
  // if(EDMPT_atyp == NULL) return 0;


  
  //-------------------------------------------------
  if(!strcmp(cp1, "Help")) {
    APP_Help ("EDMPT", "");

  
  //-------------------------------------------------
  } else if(!strcmp(cp1, "Pln")) {
    // display plane
    EDMPT_offTyp = Typ_PLN;
    // EDMPT_mousemove_CB (0,0);   // redraw modify-symbol
    GUI_set_enable (&EDMPT_f_len, FALSE);   // disactivate length
    GUI_set_enable (&EDMPT_f_mod, TRUE);     // activate delete insert modify
    actCurPos = EDMPT_newPos__ ();   // get actCurPos = cursor-Position 
    EDMPT_curSym_set (&actCurPos);   // set cursor-symbol vector
    DL_Redraw ();


  //-------------------------------------------------
  } else if(!strcmp(cp1, "Vec")) {
// TODO: 
TX_Print("*************** move along vector not yet implemented ....");
return 0;

    // user has activated checkbox "vector";
    EDMPT_offTyp = Typ_VC;
    // EDMPT_mousemove_CB (0,0);   // redraw modify-symbol
    GUI_set_enable (&EDMPT_f_len, TRUE);    // activate length
    // if(EDMPT_offLen == 0.) {
      // GUI_set_enable (&EDMPT_f_mod, TRUE);   // activate delete insert modify
    // } else {
    // EDMPT_mode = 3;
      // GUI_set_enable (&EDMPT_f_mod, FALSE);  // disactivate delete insert modify
    // }

    // get Z-vec of active surface
    vcz = &WC_sur_act.vz;
    // write Z-vec of active surface into EDMPT_w_off

    // display offsetVec as DLI_TMP_OFFO in screenCenter
    EDMPT_get_offObj ();


    actCurPos = EDMPT_newPos__ ();   // get actCurPos = cursor-Position 
    EDMPT_curSym_set (&actCurPos);   // set cursor-symbol vector
    DL_Redraw ();


  //-------------------------------------------------
  } else if(!strcmp(cp1, "Del")) {
    // start mode delete-curve-points
    // skip disactivation of delete
    // if(GTK_TOGGLE_BUTTON (parent)->active == 0) return 0;  // 1=ON;0=OFF
    if(EDMPT_mode == 2) goto L_exit;

    GL_temp_del_1 (DLI_TMP_CUR);      // delete temp-vec or plane
    // GUI_set_enable (&EDMPT_f_mod, FALSE); // disactivate modify,delete,insert
    GUI_set_enable (&EDMPT_f_pln, FALSE); // disactivate plane,vector,..

    EDMPT_mode = 2;                    // 0=modify, 1=insert, 2=delete
    EDMPT_msg_mode ();
    sele_reset ();
    EDMPT_sel__ (1); // enable selection of point only
    sele_setNoConstrPln ();
    // UI_CursorNo (0);


  //-------------------------------------------------
  } else if(!strcmp(cp1, "Ins")) {
    // start mode insert-curve-points
    if(EDMPT_mode == 1) goto L_exit;

    GL_temp_del_1 (DLI_TMP_CUR);    // delete temp-vec or plane
    // GUI_set_enable (&EDMPT_f_mod, FALSE); // disactivate modify,delete,insert
    GUI_set_enable (&EDMPT_f_pln, FALSE); // disactivate plane,vector,..

    EDMPT_mode = 1;                  // 0=modify, 1=insert, 2=delete

    // UI_GR_Sel_Filter(18);
    EDMPT_msg_mode ();

    // make the baseObj selectable ..
    // DL_pick_set (actDli, 1);
    sele_reset ();
    sele_set__ (Typ_TmpPT);   // enable selection only point-on-curve
    sele_setNoConstrPln ();



  //-------------------------------------------------
  } else if(!strcmp(cp1, "Mod")) {
    // start mode modify-curve-point
    // if(EDMPT_mode == 0) goto L_exit;

    EDMPT_mode = 0;       // 0=modify, 1=insert, 2=delete
    // enable Radiobuttons modify,delete,insert
    GUI_set_enable (&EDMPT_f_mod, TRUE);
    // enable Radiobuttons plane,vector,..
    GUI_set_enable (&EDMPT_f_pln, TRUE);
    EDMPT_msg_mode ();
    sele_reset ();
    EDMPT_sel__ (0); // enable selection of Plane + point
    sele_setNoConstrPln ();
    // GL_vcSel_init (-1, 1);          // enable VectorSelector


  //-------------------------------------------------
  } else if(!strcmp(cp1, "Save")) {

    if(EDMPT_mode == 0) {
      // update source of active obj in file selection.txt (read, modify write)
      EDMPT_save__ ();
    }

    if(EDMPT_mode != 0) {
      // copy selection.txt -> selection1.txt
      EDMPT_src_save (0);
      // insert,delete: keep curve, ..
      // draw all temp. points
      // EDMPT_points (7, EDMPT_SYM_COL);
    }

    // copy selection.txt -> selection1.txt
    EDMPT_src_save (0);

    if(EDMPT_stat != 3)
      EDMPT_unlock ();           // unlock mainMenu; lock Save|cancel (not fixed)

    EDMPT_sel__ (0); // enable selection of Plane + point
    EDMPT_msg_mode ();


  //-------------------------------------------------
  } else if(!strcmp(cp1, "Canc")) {
    // Cancel; restore last stored obj (selecetion1.txt)


    // if(EDMPT_mode != 0) {
      // insert and delete
      // restore original version of active obj -
      // copy selection1.txt -> selection.txt
      EDMPT_src_save (1);
      // update actPta pMod actAto actTxo
      EDMPT_ptab_get ();

      // update curve
      EDMPT_points (6, 0);
      // create points
      EDMPT_points (1, 0);
    // }

/*
    if(EDMPT_mode == 0) {
      // modify curve
      EDMPT_points (2, 0);          // delete all temp. points
      // update actPta pMod actAto actTxo  from file selection.txt
      EDMPT_ptab_get ();            // init (read src from file)
      // EDMPT_points (7, EDMPT_SYM_COL);          // draw all temp. points
      EDMPT_points (6, 0);          // display curve to modify temporary
      // create points
      EDMPT_points (1, 0);
      // actInd = -1;
      // AP_errStat_reset (1);        // 2011-08-31
      // DL_Redraw ();
    }
*/

    if(EDMPT_stat != 3)
      EDMPT_unlock ();            // unlock mainMenu; lock Save|cancel (not fixed)

    EDMPT_sel__ (0); // enable selection of Plane + point

    EDMPT_msg_mode ();


  //-------------------------------------------------
  } else if(!strcmp(cp1, "Exit")) {
    // save active-obj > codebase (if modif)
    if(EDMPT_changed > 0) {
      // get new SRC out of file selection.txt; change SRC in memSpc
      EDMPT_exit_1 ();
    }
    EDMPT_CB1 (GUI_ES("Shutdown"));
    

  //-------------------------------------------------
  } else if(!strcmp(cp1, "Shutdown")) {
    EDMPT_points (2, 1);            // remove all
    AP_UserKeyIn_reset ();
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

    // restore model
    EDMPT_exit_2 ();


    // free memSpaces, remove selectionPoints ..
    EDMPT_mem_free ();

    if(EDMPT_fp_dep) {
      fclose (EDMPT_fp_dep);
      EDMPT_fp_dep = NULL;
    }


    AP_SRC_mem_edi ();            // if MAN: copy Memory -> Editor
    AP_User_reset ();             // gCad_fini ();
    // DL_pick__ (1);                // reset selectionfilter
    sele_set__ (Typ_goGeom);      // enable selection of all types
    EDMPT_mode = -1;
  }


  L_exit:
    return 0;
}


//================================================================
  int EDMPT_sel__ (int mode) {
//================================================================
// mode: 0 = enable Plane + point
// mode: 1 = enable point only


  if(mode == 0) {
    // enable selection of Plane or point
    sele_set_types (Typ_SymB,
                    // Typ_VC,
                    Typ_PLN,
                    0);
  } else if(mode == 1) {
    // enable selection of point only
    sele_set_types (Typ_SymB,
                    0);
  }


  return 0;

}


//================================================================
  int EDMPT_off_CB (MemObj *mo, void **data) {
//================================================================
// callback key-in entry Plane/Vec
// check EDMPT_w_off; if first char == 'D' open EDMPT_e_len.

  int    irc;
  char   *cp1;

  // printf("EDMPT_off_CB \n");


  cp1 = GUI_entry_get (&EDMPT_w_off);
    // printf(" EDMPT_w_off |%s|\n",cp1);

  // if((*cp1 == 'D')||(*cp1 == 'd')) {
    // GUI_set_enable (&EDMPT_e_len, TRUE);
  // } else {
    // GUI_set_enable (&EDMPT_e_len, FALSE);
  // }

  irc = EDMPT_get_offObj ();
  if(irc >= 0) EDMPT_mousemove_CB (0, 0);  // update

  // return 0;
  // printf("exit EDMPT_off_CB \n");

  return FALSE;  // do defaultOperations

}


//================================================================
  int EDMPT_len_CB (MemObj *mo, void **data) {
//================================================================
// callback key-in entry length
// set EDMPT_offLen

  char   *cp1;


  printf("EDMPT_len_CB \n");

  cp1 = GUI_entry_get (&EDMPT_e_len);

  if(strlen(cp1) > 0) {
    EDMPT_offLen = atof(cp1);
      // printf(" EDMPT_len_CB %f |%s|\n",EDMPT_offLen,cp1);

    // put vz with EDMPT_offLen -> vx
    UT3D_vc_setLength (&EDMPT_offObj.vx, &EDMPT_offObj.vz, EDMPT_offLen);

  } else {
    EDMPT_offLen = 0.;

  }

    printf(" _len_CB-EDMPT_offLen = %lf\n",EDMPT_offLen);


  if(EDMPT_offLen == 0.) {
    
    EDMPT_stat = 0;
    EDMPT_unlock (); // // mod,ins,del ON; save,cancel OFF; exit ON;
    
  } else {
    EDMPT_stat = 3;
    EDMPT_fixed_0 ();   // mod,ins,del OFF; save,cancel,exit always ON
  }


  EDMPT_get_offObj ();
  EDMPT_mousemove_CB (0, 0);  // update

  return 0;

}


//================================================================
  int EDMPT_ptab_get () {
//================================================================
// get actObj, all points of actObj into actPta, actPtn
// Input: 
//   actTyp, actDbi
//   objSource in file (EDMPT_rea_src EDMPT_wri_src temp/selection.txt)
// Output:
//   actPta pMod actAto actTxo

// get all modifyable-points of sourcline as DB-points into ?
// - get tso from sourcline (APED_txo_srcLn__)
// - get all modifyable-points out of tso (APED_txo_modObj_get)
// - get DB-objects from modifyable-points (ATO_ato_eval__)
// - add dbo to ObjDB-list EDMPT_ptDba


  int       irc, i1, i2, bufSiz, itsMax, iNr, *atyp, modNr, txoNr, form;
  long      li;
  char      *cBuf, *txso, *cp1;
  ObjAto    ato2;
  ObjTXTSRC *ts0, *ts1;
  // MemTab(ObjTXTSRC) mt0, mt1;


  // printf("PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP \n");
  // printf("EDMPT_ptab_get typ=%d dbi=%ld\n",actTyp,actDbi);

  actPtn = 0;


  // free memSpaces, remove symbolic-points
  EDMPT_mem_free ();


  //----------------------------------------------------------------
// OLD VERSION
  // get & decode the sourceObj
  // irc = EDMPT_get_ato (&iNr, &atyp, &atab);
  // if(irc < 0) return irc;
    // printf("ex-get_ato: irc=%d iNr=%d\n",irc,iNr);


  //----------------------------------------------------------------
  // get subObjects of definitionLine (EDMPT_rea_src) into txso

  // get unmodif. source of active obj; skip, defHdr; cut objName
  EDMPT_src_get_src (&txso);
    // printf(" _get_ptab code=|%s|\n",txso);


  //----------------------------------------------------------------
  // get source-objects into tso0 from sourcline txso

  // get temp-space for ts0
  itsMax = SRCU_tsMax (txso);    // printf(" itsMax=%d\n",itsMax);
  i1 = itsMax * sizeof(ObjTXTSRC);  // bytes
  ts0 = (ObjTXTSRC*) MEM_alloc_tmp (i1);
  // MemTab_ini_fixed (&mt0, ts0, i1, sizeof(ObjTXTSRC), Typ_ObjTXTSRC);


  // analyze sourceline; get source-objects -> ts0
  txoNr = APED_txo_srcLn__ (ts0, itsMax, txso);
    // printf(" _txo_srcLn__ txoNr=%d\n",txoNr);
  if(txoNr < 1) { printf(" EDMPT_ptab_get ERR1 %d\n",txoNr); return txoNr; }
  // mt0.rNr = irc;
    // APED_txo_dump (ts0, txso, "_get_ptab-ts0");




  //----------------------------------------------------------------
  // make ato from txo
  // get tempSpace for ato2
  ATO_getSpc_tmp__ (&ato2, itsMax);
  // get ato2=atomic-objects of ts0
  irc = ATO_ato_txo__ (&ato2, ts0, txso);
  if(irc < 0) {TX_Print("EDMPT_ptab_get E3"); return -1;}
     // ATO_dump__ (&ato2, "_get_ptab-ato2-1");


  //----------------------------------------------------------------
  // - get DB-objects from modifyable-points (ATO_ato_eval__)
  irc = ATO_ato_eval__ (&ato2);
  if(irc < 0) {TX_Print("EDMPT_ptab_get E4"); return -1;}
     // ATO_dump__ (&ato2, "_get_ptab-ato2-2");
     

  //================================================================
  // get space for actPta, actAto, actTxo

  // i1 = EDMPT_anr;           // nr of existing obj's
  // EDMPT_anr = ato2.nr;
  i1 = ato2.nr;

  // if(EDMPT_anr > i1) {
    // get memoryspace for the points.
    // TODO: find nr of points (less than nr of atomic-objs)
    actPta = (Point*) realloc (actPta, i1 * sizeof(Point));
    if(!actPta) {TX_Print("EDMPT_ptab_get E5"); return -1;}

    pMod  = (int*) realloc (pMod, i1 * sizeof(int));
    if(!pMod) {TX_Print("EDMPT_ptab_get E8"); return -1;}

    txoNr += 1;  // add spc for the TYP_FuncEnd-record !
    actTxo = (ObjTXTSRC*) realloc (actTxo, txoNr * sizeof(ObjTXTSRC));
    if(!actPta) {TX_Print("EDMPT_ptab_get E5"); return -1;}

    ATO_malloc (&actAto, i1);
  // }


  //----------------------------------------------------------------
  // get modifyable points -> pMod
  modNr = APED_txo_modObj_get (pMod, ts0, i1, Typ_PT, txso);
  if(modNr < 1) { printf(" EDMPT_ptab_get ERR2 %d\n",modNr); return modNr; }
    // APED_txo_dump (ts1, txso, "_get_ptab-ts1");


  //----------------------------------------------------------------
  // // load refSys - if one exists
  // lRef = DL_GetTrInd (actDli);
  // if(lRef > 0) {
    // plnRef = DB_get_PLN (lRef);
    // UT3D_m3_loadpl (mRef, plnRef);
    // memcpy (APT_2d_to_3d_Mat, mRef, sizeof(Mat_4x3));
    // APT_2d_to_3d_mode = ON;
  // }


  //----------------------------------------------------------------
  // copy ts0 -> actTxo
  memcpy (actTxo, ts0, txoNr * sizeof(ObjTXTSRC));

  // get pointPositions into actPta

  // copy points from atomicObj -> actPta
  i2 = 0;
  for(i1=0; i1 < ato2.nr; ++i1) {
    // copy ato2 -> actAto   
    ATO_ato_expr_add (&actAto, ato2.typ[i1], ato2.val[i1], (int)ato2.ilev[i1]);

    if(ato2.typ[i1] != Typ_PT) continue;

    // copy point
    li = ato2.val[i1];              // get li = pointIndex
    actPta[i2] = DB_GetPoint (li);   // // copy point
    ++i2;
      // DEB_dump_obj__ (Typ_PT, &actPta[i2], "   DB-pt[%d]",li);

    // if refSys is active: transform ..
    // if(lRef > 0) UT3D_pt_tra_pt_m3 (&actPta[i2], mRef, &actPta[i2]);
  }

  actPtn = i2;

  // keep all points made in upper loop !
  dynPti = DB_dyn__ (0, Typ_PT, 0L);       // save state of dyn-points


    // TESTBLOCK
    // EDMPT_dump_atab ();
    // EDMPT_dump_ptab ();
    // printf("ex EDMPT_ptab_get %d\n",actPtn);
    // END TESTBLOCK


  return 0;
}
  

//================================================================
  int EDMPT_dump_atab (char *fncInf) {
//================================================================
// dump modified point (temporary dynamic points in DB)
  
  int    i1;
  long   dbi;
  Point  *pt1;

  printf("EDMPT_dump_atab %s actPtn=%d\n",fncInf,actPtn);

  for(i1=0; i1 < actAto.nr; ++i1) {
    printf(" actAto typ[%d]=%d val=%f\n",i1,actAto.typ[i1],actAto.val[i1]);
    if(actAto.typ[i1] != Typ_PT) continue;
    dbi = actAto.val[i1];
    pt1 = DB_get_PT (dbi);
    DEB_dump_obj__ (Typ_PT, pt1, "   DB-pt[%d]",dbi);
  }


  return 0;

}
 

//================================================================
  int EDMPT_dump_ptab () {
//================================================================
// dump actPta

  int i1;

  printf("----------EDMPT_dump_ptab %d\n",actPtn);

  for(i1=0; i1 < actPtn; ++i1)
    DEB_dump_obj__ (Typ_PT, &actPta[i1], "actPta[%d]", i1);


  return 0;

}
 

//================================================================
  int EDMPT_wri_src (char *cBuf) {
//================================================================
// save sourceLine

  char    fnam[160];

  // printf("EDMPT_wri_src |%s|\n",cBuf);

  sprintf(fnam,"%sselection.txt",OS_get_tmp_dir());
  UTX_wrf_str (fnam, cBuf);

  return 0;

}


//================================================================
  int EDMPT_rea_src (char *cBuf, int bufSiz) {
//================================================================
// read file selection.txt into memstring cBuf

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
  int EDMPT_exit_1 () {
//================================================================

//  - alle Punkte testen, ob gleich od modifiziert;
//      gleich: den PunktText aus dem alten Source uebernehmen;
//      modifiziert: ersetzen durch den dynamischen Code P(x y z)
//  
//  - den existierenden Code deaktivieren (prepend "_")
//  - insert new code hinter altem Code
//  - redraw all starting with old codeline
  

  // int     irc, i1, i2, cNr, bufSiz;
  // long    l1, l2, lNr;
  // char    *cPos, *cBuf, *cp1, *cp2;
  // int     iNr;

  int   irc, siz;
  char  *cBuf;


  // printf("EDMPT_exit_1 %d\n",actAto.nr);

  // get (unmodifid) source of active obj
  EDMPT_src_get__ (&cBuf, &siz);

  DL_set_iatt (actDli, EDMDAT.dlAtt);        // restore original attribute


  // deactivate line, insert modified line cBuf
  // report change -> UNDO.
  irc = APED_src_chg (actlNr, cBuf);

  // // reset refSys
  // APT_2d_to_3d_mode = OFF;

  EDMPT_changed = 0;

    // printf("ex-EDMPT_exit_1\n");


  return 0;

}

//================================================================
  int EDMPT_save__ () {
//================================================================
// read, modify write file selection.txt

  int     irc, i1, siz;
  char    *cBuf;


  // printf("EDMPT_save__ EDMPT_mode=%d\n",EDMPT_mode);

  // get (unmodifid) source of active obj
  EDMPT_src_get__ (&cBuf, &siz);

  // modify the sourceLine 
  if(EDMPT_mode == 0)      irc = EDMPT_src_mod (cBuf, siz);
  else if(EDMPT_mode == 1) irc = EDMPT_src_ins (cBuf, siz);
  else if(EDMPT_mode == 2) irc = EDMPT_src_del (cBuf);

  if(irc < 0) return -1;

  // save sourceLine
  EDMPT_wri_src (cBuf);


  // increment change-flag
  ++EDMPT_changed;

  // printf("ex EDMPT_save__\n");

  return 0;

}
 

//================================================================
  int EDMPT_src_mod (char *cBuf, int siz) {
//================================================================
// compare all points; modify points in src cBuf.
// old pointPositions: actPta DB-point[actAto.val]
// new pointPositions: DB-point[actAto.val]
  // skip objnam


  int   irc, i1, iPt, sLen, ii;
  long  dbi;
  char  *cp1, *cp2, *cp3;
  Point pt1;


  // printf("EDMPT_src_mod |%s|\n",cBuf);
  // EDMPT_dump_ptab ();
  // EDMPT_dump_atab ();
  // ATO_dump__ (&actAto, "_src_mod");


  // skip definition-header
  cp1 = cBuf;
  APED_defDbo_skip (&cp1);


  // compare points & create new sourcetext
  iPt = -1;   // index into actPta


  // loop tru points (actPta = last stored, pt1 = new position)
  for(i1=0; i1<actAto.nr; ++i1) {
      // printf(" _src_mod-i1=%d typ=%d\n",i1,actAto.typ[i1]);
    if(actAto.typ[i1] != Typ_PT) continue;

    // set iPt = index into ato-points and actPta
    ++iPt;                                  

    // get pt1 = new position
    dbi = actAto.val[i1];                   // index of nxt point
    pt1 = DB_GetPoint (dbi);
      // DEB_dump_obj__ (Typ_PT, pt1, " _src_mod-pt1=%d;dbi=%d",iPt,dbi);

                                     
    // find cp2 = startPos of next implicit-point
    irc = APED_pt_find (&cp2, &sLen, &cp3, cp1);
    if(irc < 0) {TX_Print("PED_src_mod E002"); return -1;}
      // ii = cp3 - cp2 - sLen;     // nr of chars for deli, blanks
      // printf(" _src_mod-cp2=|%s|\n",cp2);


    // test if pointPos is modified
    irc = UT3D_comp2pt (&pt1, &actPta[iPt], UT_TOL_pt);

    if(irc == 0) {
        // printf(" _src_mod-modified: %d dbi=%ld\n",iPt,dbi);

      // check if modified obj is 2D or 3D
      if(EDMDAT.irs > 0) {   // see AP_IS_2D
        // transfer obj-coords from 3D back to 2D.
        UT3D_pt_tra_pt_m3 (&pt1, EDMDAT.mat2, &pt1);
          // DEB_dump_obj__ (Typ_PT, &pt1, "EDMPT_src_mod-pt %d %d",i1,iPt);
      }
        
      

      // 0=diffent points; change implicit-point starting at cp2 -> DB-point dbi
      // APED_pt_chg (&sLen, dbi, cp2, sLen);
      APED_pt_chg (&sLen, cp2, sLen, &pt1);

    }

    cp1 = cp2 + sLen;
  }

  // printf("ex PED_src_mod |%s|\n",cBuf);

  return 0;

}


/* Version get modified src from source-objects txa
//================================================================
  int EDMPT_src_mod (char *cBuf, int siz) {
//================================================================
// compare all points; modify points in src cBuf.
// old pointPositions: DB-point[actAto.val]
// new pointPositions: actPta

  int   irc, is, il, iTxo, iAto, iPt;
  long  dbi;
  char  *cp1, *so;
  Point *pt1;


  printf("EDMPT_src_mod |%s|\n",cBuf);
  EDMPT_dump_ptab ();
  EDMPT_dump_atab ();
  ATO_dump__ (&actAto, "_src_mod");


  // get tempSpace for output
  so = (char*) MEM_alloc_tmp (siz);
  if(!so) {TX_Error("EDMPT_src_mod E1"); return -1;}


  // skip definition-header
  cp1 = cBuf;
  APED_defDbo_skip (&cp1);

    APED_txo_dump (actTxo, cp1, "_src_mod");


  // compare points & create new sourcetext
  iTxo = -1;
  iAto = -1;
  iPt  = -1;
  so[0] = '\0';


  // loop tru actTx
  L_nxt_obj:
    ++iTxo;

  L_nxt_ck:
    if(actTxo[iTxo].typ == TYP_FuncEnd) goto L_exit;

    // find next primary obj
    if(actTxo[iTxo].ipar != -1) goto L_nxt_obj;
    ++iAto;

    // skip all but <typ>
    if(actTxo[iTxo].form == Typ_PT) ++iPt;   // index into actPta

      printf(" iTxo=%d iAto=%d iPt=%d pMod=%d\n",iTxo,iAto,iPt,pMod[iPt]);

    // test if ato is a modifyable point
    // if not -add text unmodif. to output
    if(pMod[iPt] != 1) goto L_add_unmodif;

    // test if pointPos is modified
    dbi = actAto.val[iAto];                   // DB-index of point
    pt1 = DB_get_PT (dbi);
    irc = UT3D_comp2pt (pt1, &actPta[iPt], UT_TOL_pt);
    if(irc) goto L_add_unmodif; // point is not modif- add text unmodif.

    // point is modif.
    // add point to output
      printf(" modif-pt %d\n",iPt);
    AP_obj_add_pt (so, pt1);
    strcat (so, " ");
    goto L_nxt_obj;


    L_add_unmodif:
      // get is = startPos of orig.txt in actTxo
      is = actTxo[iTxo].ioff;
      // find index of next primary-obj
      for(;;) {
        ++iTxo;
        if(actTxo[iTxo].typ == TYP_FuncEnd) goto L_add_end;
        if(actTxo[iTxo].ipar == -1) break;
      }
      // get il = length of orig.txt actTxo
      il = actTxo[iTxo].ioff - is;
      // il = actTxo[iTxo].ilen;
         printf(" add-unmodif: iTxo=%d is=%d il=%d\n",iTxo,is,il);
      // add to outString
      strncat (so, &cp1[is], il);
      goto L_nxt_ck;


    L_add_end:
      strcat (so, &cp1[is]);


  //----------------------------------------------------------------
  L_exit:

    // TESTBLOCK
    printf(" so=|%s|\n",so);

    // END TESTBLOCK

  // copy new string so -> cp1 (following defHdr)
  *cp1 = '\0';    // !
  strcat (cp1, so);

    printf("ex-EDMPT_src_mod |%s|\n",cBuf);

  return 0;

}
*/


//================================================================
  int EDMPT_src_del (char *cBuf) {
//================================================================
// delete point actInd in srcLine cBuf.


  int   irc, i1, i2, sLen;
  long  dbi;
  char  *cp1, *cp2, *cp3;


  printf("EDMPT_src_del %d\n",actInd);
  printf("  cBuf in |%s|\n",cBuf);
  // EDMPT_test__ ();


  // skip objnam
  cp1 = cBuf;
  irc = APED_defDbo_skip (&cp1);  // skip definition-header
  if(irc < 0) {TX_Print("EDMPT_src_mod E001"); return -1;}


  // compare points & create new sourcetext
  i2 = -1;
  for(i1=0; i1<actAto.nr; ++i1) {
    if(actAto.typ[i1] != Typ_PT) continue;
    ++i2;                                   // index into actPta
    dbi = actAto.val[i1];                     // index of nxt point
    APED_pt_find (&cp2, &sLen, &cp3, cp1);  // cp2 now is the next point

    if(i2 != actInd) {
      cp1 = cp3;                            // skip this point
      continue;
    }

    MEM_del_str (cp2, sLen);                // delete point & done
    break;
  } 


    printf("ex-_src_del |%s|\n",cBuf);

  return 0;

}


//================================================================
  int EDMPT_src_ins (char *cBuf, int bufSiz) {
//================================================================
// insert point actPtp behind point # actInd in srcLine cBuf.
// Input: actInsPt
//   cBuf  src

     
  int   irc, i1, i2, sLen;
  char  *cp1, *cp2, *cp3, s2[80];


  printf("EDMPT_src_ins actInd=%d siz=%d\n",actInd,bufSiz);
  printf("  cBuf in |%s|\n",cBuf);
  printf("  WC_sur_ind=%d\n",WC_sur_ind);


  // skip objnam
  cp1 = cBuf;
  irc = APED_defDbo_skip (&cp1);  // skip definition-header
  if(irc < 0) {TX_Print("EDMPT_src_ins E001"); return -1;}




  // translate insertpoint actInsPt
    // DEB_dump_obj__ (Typ_PT, &actInsPt, "   actInsPt:");
  if(WC_sur_ind) {
    UT3D_pt_tra_pt_m3 (&actInsPt, WC_sur_imat, &actInsPt);
      // DEB_dump_obj__ (Typ_PT, &actInsPt, "   actInsPt-transl:");
  }
    

  // insertpoint -> s2
  s2[0] = '\0';
  AP_obj_add_pt (s2, &actInsPt);
  strcat(s2, " ");
    // printf("  s2=|%s|\n",s2);


    
  // compare points & create new sourcetext
  i2 = -1;
  for(i1=0; i1<actAto.nr; ++i1) {
    if(actAto.typ[i1] != Typ_PT) continue;
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

    printf("ex-_src_ins |%s|\n",cBuf);

  return 0;

}


//================================================================
  int EDMPT_get_tabInd (int ipt) {
//================================================================
// get index into EDMPT_atab from pointNr actInd (index into actPta)

  int  i1, ii;

  // printf("EDMPT_get_tabInd actInd=%d actAto.nr=%d\n",actInd,actAto.nr);


  ii = -1;
  for(i1=0; i1 < actAto.nr; ++i1) {
    if(actAto.typ[i1] != Typ_PT) continue;
    ++ii;
    if(ii == ipt) goto L_exit;
  }

  TX_Error("EDMPT_get_tabInd E001");
  ii = -1;

  L_exit:

    // printf("ex EDMPT_get_tabInd %d %d\n",i1,ipt);

  return i1;

}


//================================================================
  int EDMPT_restart_lnk () {
//================================================================
// EDMPT_restart_lnk  restart with new obj

  printf(" EDMPT_restart_lnk\n");

  EDMPT_stat = 0; // reset
  actInd    = -1;

  // save active obj
  if(EDMPT_changed > 0) EDMPT_exit_1 ();


  EDMPT_unlock (); 


  // EDMPT_points (6, 0);
  // redisplay base-obj EDMPT_points (6, 
  WC_set_obj_stat (0);   // perm 
  DL_SetInd (actDli);
  APT_Draw__ (EDMDAT.dlAtt, basTyp, actDbi);


  // init parent of selected obj
  EDMPT_init (oLnk1.typ, oLnk1.dbi, -1L);

  return 0;

}


//================================================================
  int EDMPT_exit_2 () {
//================================================================
// update program starting with line actlNr

  // printf("----------------- EDMPT_exit_2 %ld\n",actlNr);

  // save permanent
  WC_set_obj_stat (0);

  ED_work_CurSet (actlNr);         // go back to line lNr
  ED_work_CurSet (UT_INT_MAX);     // work until end of model

    // printf("ex-EDMPT_exit_2\n");

  return 0;

}




//================================================================
  int EDMPT_mem_free () {
//================================================================
// free memSpaces, remove symbolic-points


  if(actPta) {
    EDMPT_points (2, 0);              // remove selectionPoints ..
    free (actPta);
    actPta = NULL;
  }

  if(pMod) free (pMod);
  pMod = NULL;

  ATO_free (&actAto);

  if(actTxo) free (actTxo);
  actTxo = NULL;

  return 0;

}


//================================================================
  int EDMPT_dep_sav (MemTab(ObjSRC) *depTab) {
//================================================================
// write sourcecodes of all depending objects -> file
// file src_dep.txt
// Format: bin - linNr, bin - disListIndex, bin - nr of src-chars, asc - src

  int      i1, i2, ie, ii, ix;
  long     ll;
  char     *cp1, fnam[160];
  ObjSRC   *sa;
  FILE     *fpo;


  // printf("EDMPT_dep_sav %d\n",EDMPT_depNr);
  // MemTab_load (&sa, &ie, &depTab);
  // for(i1=0; i1 < ie; ++i1)
  // printf(" oa[%d] typ=%d dbi=%d lnr=%d\n",i1,sa[i1].typ,sa[i1].dbi,sa[i1].lnr);



  sprintf(fnam,"%ssrc_dep.txt",OS_get_tmp_dir());
  if(depTab->rNr < 2) {
    OS_file_delete (fnam);
    return 0;
  }


  if((fpo = fopen (fnam, "wb")) == NULL) {
    TX_Print("EDMPT_dep_sav E001");
      printf(" **** EDMPT_dep_sav fopen-err |%s|\n",fnam);
    return -1;
  }


  // get ie = recNr and sa = data
  MemTab_load (&sa, &ie, depTab);


  for(i1=1; i1 < ie; ++i1) {   // skip first (primary) obj)
    // printf(" oa[%d] typ=%d dbi=%d lnr=%d\n",i1,sa[i1].typ,sa[i1].dbi,sa[i1].lnr);

    // sort ascend. lnr 
    ii = UT_INT_MAX;
    for(i2=1; i2 < ie; ++i2) {
      if(sa[i2].lnr < ii) {ii = sa[i2].lnr; ix = i2;}
    }
      printf(" _dep_sav-i1=%d ii=%d\n",i1,ii);


    // get stringposition cp1 & strinLength ll in memory
    cp1 = UTF_GetPosLnr (&ll, sa[ix].lnr);

    // write -> file
    fwrite (&sa[ix].lnr, sizeof(long), 1, fpo);     // bin - linNr
    fwrite (&sa[ix].dli, sizeof(long), 1, fpo);     // bin - disListIndex
    fwrite (&ll, sizeof(long), 1, fpo);             // bin - nr of src-chars
    fwrite (cp1, ll, 1, fpo);                       // asc - src

    sa[ix].lnr = UT_INT_MAX;
  }

  fclose (fpo);

  // open for update ..
  if((EDMPT_fp_dep = fopen (fnam, "rb")) == NULL) {
    TX_Print("EDMPT_dep_sav E002");
    return -1;
  }

  return 0;

}


//================================================================
  int EDMPT_mod_dep_draw__ () {
//================================================================
// redraw all depending objects

  long   lnr, dli, ll;

  // printf("EDMPT_mod_dep_draw__ -------- depNr=%d\n",EDMPT_depNr);


  if(EDMPT_depNr < 2) return 0;


  rewind (EDMPT_fp_dep);

  for(;;) {
    // if (fgets (mem_cbuf1, mem_cbuf1_SIZ, EDMPT_fp_dep) == NULL) break;
    // UTX_CleanCR (mem_cbuf1);
    ll = fread(&lnr, sizeof(long), 1, EDMPT_fp_dep);
    if(ll == 0) break;

    fread(&dli, sizeof(long), 1, EDMPT_fp_dep);
    fread(&ll, sizeof(long), 1, EDMPT_fp_dep);
    fread(mem_cbuf1, ll, 1, EDMPT_fp_dep);
    mem_cbuf1[ll] = '\0';
      // printf(" _dep_draw__ lnr=%ld dli=%ld |%s|\n",lnr,dli,mem_cbuf1);

    // redraw - overwrite, mode temporary
    DL_SetInd (dli);  // overwrite existing DL-record
    WC_Work1 (lnr, mem_cbuf1);
  }


  return 0;

}


//================================================================
  int EDMPT_get_offObj () {
//================================================================
// get offsetObject (Plane or Vector) out of EDMPT_w_off

  int      irc;
  long     l1, dbi, dli;
  char     *p1;
  Plane    *pl1;
  Point    pt1;


  p1 = GUI_entry_get (&EDMPT_w_off);


  // get oid of offsetObj (eg "RZ" or "DZ"
  // printf("EDMPT_get_offObj  EDMPT_w_off=|%s|\n",p1);


  irc = APED_dbo_oid (&EDMPT_offTyp, &dbi, p1);
    // printf(" _obj irc=%d %d dbi=%ld\n",irc,EDMPT_offTyp,dbi);


  if(irc < -1) goto L_err;
    // MSG_pri_0 ("PEDe2");
    // TX_Print("**** ERROR: cannot analyze Plane/Vector |%s| ..",p1);



  //----------------------------------------------------------------
  if(EDMPT_offTyp == Typ_VC) {
    if(irc < 0) {  // "D(..)"
      // wait until closing ")"
      if(p1[strlen(p1) - 1] != ')') goto L_err;
      irc = AP_vec_txt (&EDMPT_offObj.vz, p1);
      if(irc < 0) goto L_err;

    } else {       // "D123"
      if(DB_VC_isFree (dbi)) goto L_err;
      EDMPT_offObj.vz = DB_GetVector (dbi);
    }

    // display vector at dli = DLI_TMP_OFFO at screenCenter
    dli = DLI_TMP_OFFO;
    pt1 = GL_GetCen ();
    GR_Draw_vc (&dli, &EDMPT_offObj.vz, &pt1, Typ_Att_Symb, 0);

      // DEB_dump_obj__(Typ_PT, &pt1, "EDMPT_offObj_vc-pt1:");
      // DEB_dump_obj__(Typ_VC, &EDMPT_offObj.vz, "EDMPT_offObj.vz:");


  //----------------------------------------------------------------
  } else if(EDMPT_offTyp == Typ_PLN) {
    if(DB_PLN_isFree (dbi)) goto L_err;
    DB_GetRef (&EDMPT_offObj, dbi);

    // display plane at dli = DLI_TMP_OFFO
    dli = DLI_TMP_OFFO;
    GR_Draw_pln (&dli, &EDMPT_offObj, Typ_Att_Symb);

  }
  
    // TESTBLOCK
    // printf("ex EDMPT_get_offObj EDMPT_offTyp=%d\n",EDMPT_offTyp);
    // DEB_dump_obj__(EDMPT_offTyp, &EDMPT_offObj, "  EDMPT_offObj:");
    // END TESTBLOCK


  return 0;


  L_err:
    EDMPT_offTyp = Typ_Error;
    GL_temp_del_1 (DLI_TMP_CUR);  //delete temp-vec or plane
    DL_Redraw ();
    return irc;

}


//================================================================
  int EDMPT_del__ () {
//================================================================
// delete point actInd
// delete point # <actInd> in sourcecode & redraw

  int     i1, ii;


  // get index into EDMPT_atyp
  ii = EDMPT_get_tabInd (actInd);

    // printf("EDMPT_del__ %d %d\n",actInd,ii);


  // delete temp-point
  GL_temp_del_1 (3L);  // delete temporary obj -3 (circ)
  EDMPT_points (8, 0);       // remove 2011-10-07


  // delete point from atomicObj
  // MEM_del_nrec (&actPtn, actPta, ii, 1, sizeof(Point));
  // i1 = EDMPT_anr;
  // MEM_del_IndRec (&EDMPT_anr, EDMPT_atyp, ii);
  // MEM_del_DbRec (&i1, EDMPT_atab, ii);
  ATO_del_rec (&actAto, ii);


  EDMPT_stat = 1;
  EDMPT_mousemove_CB (0,0);   // redraw
  EDMPT_stat = 0;


  // EDMPT_points (7, 0);


  // lock mainMenu; unlock Save|cancel
  EDMPT_lock ();

  return 0;

}


//================================================================
  int EDMPT_ins_pt () {
//================================================================
// get segmentnr where to insert point & save point
// set actInd = after this point insert new point pt1
// save point in actPta[0]

#define pTabSiz 16
  int      irc, i1;
  long     dli;
  double   up, da[pTabSiz], d1;
  Point    pt1, pta[pTabSiz];
  // Memspc   tmpSpc1;


  // get mousepos -> pt1
  // APT_obj_expr (&pt1, typ, cp1);
  UI_GR_get_actPosA (&pt1);


  // DEB_dump_obj__ (Typ_PT, &pt1, "EDMPT_ins_pt: pt1");
  // printf("  actTyp=%d\n",actTyp);
  // printf("  UT_DISP_cv=%f\n",UT_DISP_cv);


  //----------------------------------------------------------------
  if(actTyp == Typ_CVBSP) {
      DEB_dump_obj__ (Typ_CVBSP, actObj, " _ins_pt-actObj");

    // project point onto curve
    i1 = pTabSiz;
    irc = UT3D_pt_projptbspl (&i1, pta, da, actObj, &pt1);
    if(irc < 0) {TX_Print("UT3D_pt_projptbspl Error %d",irc);return -1;}
      DEB_dump_obj__ (Typ_PT, &pta[0], " _ins_pt-prj");
    pt1 = pta[0];

    // get parameter from point
    // if(d1 > UT_DISP_cv * 2.) goto L_err1;
    // UME_init (&tmpSpc1, memspc201, sizeof(memspc201));
    irc = UT3D_parCv_bsplpt (&up, &d1, actObj, &pt1);
    if(irc < 0) {TX_Print("UT3D_parCv_bsplpt Error %d",irc);return -1;}
      // printf(" val. parameter=%f dist=%f\n",up,d1);

    // point on curve from parameter
    // UT3D_pt_evalparCv (&pt1, actObj, up);

    // change knotenwert into parameter
    // up = UT3D_par1_parBSp (&uk, actObj);
    // up = UT3D_par1_parcbsp (&uk, actObj);
      // printf(" up=%f\n",up);

    // get segmenNr from parameter
    // UME_init (&tmpSpc1, memspc201, sizeof(memspc201));
    bspl_segNr_par (&actInd, up, actObj);
      // printf(" segNr=%d\n",actInd);




  //----------------------------------------------------------------
  } else if(actTyp == Typ_CVPOL) {
    // pta[0] = projected point pt1 onto polygon actObj
    i1 = pTabSiz;
    irc = UT3D_pt_projptplg (&i1, pta, da, actObj, &pt1);
    if(irc < 0) {TX_Print("UT3D_pt_projptplg Error %d",irc);return -1;}

    // get parameter of point on polygon
    pt1 = pta[0];
    UT3D_parplg_plgpt (&up, &pt1, actObj);
    UPLG_iseg_par (&actInd, up, actObj);
      // printf(" actInd=%d up=%f\n",actInd,up);



  //----------------------------------------------------------------
  } else {
    goto L_err1;
  }


  //----------------------------------------------------------------
  // save insert point ..
  actInsPt = pt1;
    // DEB_dump_obj__ (Typ_PT, &actInsPt, "ex-_ins_pt");
  return 0;


  L_err1:
    MSG_pri_0 ("PEDe3"); // cannot insert points ..
    return -1;

}


//================================================================
  int EDMPT_lock () {
//================================================================
// lock delete,insert,modify,exit; unlock Save|cancel

  // printf("EDMPT_lock \n");


  GUI_set_enable (&EDMPT_f_mod, FALSE);       // disactivate delete insert modify
  GUI_set_enable (&EDMPT_f_sc, TRUE);         // activate Save|cancel
  GUI_set_enable (&EDMPT_b_ex, FALSE);        // disactivate exit
  MSG_pri_0 ("PEDmod3");    // save or cancel

  EDMPT_stat = 2;

  // UI_CursorNo (1);
  // UI_GR_Sel_Filter (20);  // no select

  return 0;

}


//================================================================
  int EDMPT_unlock () {
//================================================================
// unlock delete,insert,modify,exit; lock Save|cancel
    
  // printf("EDMPT_unlock \n");

  if(EDMPT_mode == 0) {
    // modify: unlock pln,vec,len
    GUI_set_enable (&EDMPT_f_pln, TRUE);     // unlock pln,vec,len
  }
  GUI_set_enable (&EDMPT_f_mod, TRUE);     // activate delete insert modify
  GUI_set_enable (&EDMPT_f_sc, FALSE);     // disactivate Save|cancel
  GUI_set_enable (&EDMPT_b_ex, TRUE);      // activate exit
  
  EDMPT_stat = 0;

  return 0;
    
}


//================================================================
  int EDMPT_fixed_0 () {
//================================================================
// EDMPT_fixed_0      fix widgets for modeify with fixed-vector
// fixed (Vector+length):
//  - block mod,ins,del
//  - save,cancel,exit always active
//  - select point = transfer immediate

  GUI_set_enable (&EDMPT_f_mod, FALSE);       // disactivate delete insert modify
  GUI_set_enable (&EDMPT_f_sc, TRUE);         // activate Save|cancel
  GUI_set_enable (&EDMPT_b_ex, TRUE);         // activate exit

  return 0;

}


//================================================================
  int EDMPT_test__ () {
//================================================================

  Point   pt1;

  printf("========================================= EDMPT_test__\n");


  pt1 = DB_GetPoint (22L);
  DEB_dump_obj__ (Typ_PT, &pt1, "ex DB_GetPoint 22");


  return 0;

}


// EOF
