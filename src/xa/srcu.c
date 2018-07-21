// Dynamic update numeric strings.
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
UI_GR_Sel_Filter (21);  // unused; to be removed ..
SRCU_fakt/SRCU_val_cen sollte abhängig sein vom active scale.

-----------------------------------------------------
Modifications:
2015-08-25 init SRCU_isModified; RF.
2013-09-10 neu erstellt. RF.

-----------------------------------------------------
*/
/*!
\file  ../xa/srcu.c
\brief  Dynamic update numeric strings
\code
=====================================================
List_functions_start:

SRCU_win_var         init window for update variable
SRCU_win__           create window (toolbar)
SRCU_win_e2          set / get  act. value
SRCU_key_CB
SRCU_sel_CB
SRCU_sli_CB          callback slider
SRCU_ent_CB
SRCU_tmr_CB__        timer callback
SRCU_exit__
SRCU_work_upd        modify value and update
SRCU_work__
SRCU_list_vals__     get list of all value-objects

SRCU_ck_act__
SRCU_analyz__        find definition-line; analyze line; find value # <valNr>
SRCU_analyz_val      find value nr <valNr>
SRCU_tsMax           returns max nr of sourceObjects in sourceString
SRCU_mod__           update model
SRCU_pri_mod         modify the primary-sourceline
SRCU_txt_popCoord    fix string for popup-menu for modify-value

List_functions_end:
=====================================================

\endcode *//*----------------------------------------

see also:
../xa/xa_src.c       create sourceCode

*/

#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>



#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_gtypes.h"           // AP_src_typ__
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_txTab.h"            // TxtTab
#include "../ut/ut_txfil.h"            // UTF_GetPosLnr
#include "../ut/ut_memTab.h"           // MemTab
#include "../ut/ut_BitTab.h"           // BitTab
#include "../ut/ut_os.h"               // UI_Func..
#include "../ut/ut_cast.h"

#include "../gui/gui__.h"             // GUI_*

#include "../db/ut_DB.h"               // DB_

#include "../xa/xa.h"                  // AP_STAT AP_mod_fnam
#include "../xa/xa_ed_mem.h"           // ObjSRC APED_onam_search
#include "../xa/xa_ui.h"               // APF_EndTab,
#include "../xa/xa_msg.h"              // MSG_read
#include "../xa/xa_sele.h"             // Typ_goGeom
#include "../xa/xa_mem.h"              // IE_outTxt



//================================================================
// ex ../xa/xa_ui.c
extern MemObj  UIw_Box_TB;    // toolbarBox

// Externals aus ../ci/NC_Main.c:
// extern double    APT_ModSiz;

// ex ../xa/xa_ui_cad.c
extern int       IE_modify;

// ex ../ut/ut_gtypes.c
extern char *ObjCodTab[];
extern char *NcoTxtTab[];
extern char *GcoTxtTab[];
// extern char *MOpTxtTab[];


//================================================================
// LOCAL:
static MemObj SRCU_tb=GUI_OBJ_NEW;   // Toolbar
static MemObj SRCU_e2, SRCU_b1, SRCU_b3, SRCU_sl1;

static int    SRCU__obj_stat;           // old APT_obj_stat (before, after)
static int    SRCU__ED_stat;            // old ED_mode
static int    SRCU_tmr_stat = 0;        // 0 stops timer;
static int    SRCU_valNr = 0;           // nr of value in def.ln to be modified
static char   SRCU_val_def[40];         // default-value (string)
static double SRCU_fakt = 0.;           // modify active value by SRCU_fakt
static double SRCU_scal;                // scale
static ObjSRC SRCU_obj_pri;             // primary obj
static ObjTXTSRC SRCU_val_pri;
static int    SRCU_codeOff;             // dist lineHdr - codeStart in chars
static char   *SRCU_ln_pri;             // the def.ln of the prim.obj
static int    SRCU_val_siz;             // size of the active value in chars
static int    SRCU_isModified;          // 0=modif; 1=unmodif
static double SRCU_val_cen;             // centervalue


//================================================================
// PROTOS:
  int SRCU_sel_CB (int src, long dli);
  int SRCU_key_CB (int key);
  int SRCU_ent_CB (MemObj *mo, void **data);
  int SRCU_win__ (MemObj *parent, void **data);
  int SRCU_tmr_CB__ (void *data);
  int SRCU_sli_CB (MemObj *mo, void **data);





//================================================================
   int SRCU_ck_act__ () {
//================================================================
/// if(SRCU_ck_act__()) printf(" is active ..\n");


  return (GUI_OBJ_IS_VALID(&SRCU_tb));

}


//================================================================
  int SRCU_win_var (int typ, long dbi, int valNr) {
//================================================================
/// \code
/// init window for update variable
///   valNr  the number of the value in the definitionline; first = 0.
///
/// Example:
///   SRCU_win_var (typ_PT, 20L, 1);
///   // modify second value of "P20=P(100 200 0)"
/// \endcode


  int    i1;
  double  d1, d2;


  // printf("SRCU_win_var %d %ld %d\n",typ,dbi,valNr);

  SRCU_obj_pri.typ = typ;
  SRCU_obj_pri.dbi = dbi;
  SRCU_valNr = valNr;


  // get position and lenght of value nr <valNr> in the codeline.
  // sets SRCU_val_pri
  i1 = SRCU_analyz__ (typ, dbi, valNr);
  if(i1 < 0) {
    TX_Print("***** SRCU: cannot find value in objectcode..");
    return -1;
  }


// TODO: if window already exists, goto L_init.
  SRCU_isModified = 1;   // 0=modif; 1=unmodif.



  //----------------------------------------------------------------
  // lock some application-functions...
  UI_func_stat_set__ (-APF_TB_CAD,
                      -APF_MEN_FIL,
                      -APF_MEN_SM,
                      -APF_MEN_MDF,
                      -APF_MEN_APP,
                      -APF_WIN_B_E,
                      -APF_HIDE,
                      -APF_VWCADMAN,
                      -APF_BUT_GSE,
                      -APF_PLUGINS,
                      -APF_UNDO,
                      0);

  AP_UserKeyIn_get     (SRCU_key_CB);         // attach keystrokes
  AP_UserSelection_get (SRCU_sel_CB);         // attach grafic selections

  // sele_set__ (Typ_PT);                          // get points from selections
  // sele_reset_type (Typ_Model);


  // init toolbar
  if(GUI_OBJ_IS_VALID(&SRCU_tb)) {
    // GUI_set_show (&SRCU_tb, 1);
    // // clear inputfields
    // GUI_entry_set (&SRCU_e2, "");
    return -1;

  } else {
    SRCU_win__ (&UIw_Box_TB, GUI_SETDAT_EI(TYP_EventPress,UI_FuncInit));
  }



  SRCU__obj_stat = WC_get_obj_stat ();
  WC_set_obj_stat (0);  // 0=perm

  SRCU__ED_stat = ED_query_mode ();
  // ED_enter ();
  // ED_go ();
    // printf(" ED_mode=%d\n",SRCU__ED_stat);


  // IE_modify = 1;


  // die erste Selektion geht sonst ned (das Bild wird kleiner, weil unten
  //   der Toolbar erzeugt wird !))
  // DL_Redraw();
  Grp_Clear (1);



  //----------------------------------------------------------------
  L_init:
  // if(typ != Typ_VAR) {
    // TX_Print("***** SRCU: until now only variables implemented ..");
    // return 0;
  // }


  // get default-value
  SRCU_val_cen = atof (SRCU_val_def);

  GL_GetViewSizU (&d1, &d2);
  SRCU_scal = (d1 + d2) / 8.;
    // printf(" SRCU_scal=%lf\n",SRCU_scal);


  // show defVal in e2
  SRCU_win_e2 (&SRCU_val_cen, 1);

  // init 
  SRCU_work__ (0);


  return 0;

}


//================================================================
  int SRCU_win__ (MemObj *parent, void **data) {
//================================================================
// create Toolbar

  int     irc, ifunc;
  char    *p1, s1[64], s2[32];
  MemObj  box0, box1, box2, wh;




  //----------------------------------------------------------------
  ifunc = GUI_DATA_I1;
    // printf("SRCU_win__ %d\n",ifunc);

  switch (ifunc) {

    //---------------------------------------------------------
    case UI_FuncInit:  // 90
      if(GUI_OBJ_IS_VALID(&SRCU_tb)) goto L_exit;

      // SRCU_tb = GUI_toolbar__ (parent);
      SRCU_tb = GUI_toolbox__ (parent);

      box0 = GUI_box_h (&SRCU_tb, "e");

      APED_oid_dbo__ (s2, SRCU_obj_pri.typ, SRCU_obj_pri.dbi);
      sprintf (s1, "<b>modify value %s val.%d: </b>", s2, SRCU_valNr);
        // printf(" s2=|%s| s1=|%s|\n",s2,s1);
      GUI_label_htm__ (&box0, s1, "");

      SRCU_e2 = GUI_entry__ (&box0,"     value:  ","",SRCU_ent_CB,"e2","16e");

      GUI_spc__ (&box0, 0, 2);
      // SRCU_sl1 = GUI_Slider__ (&box0, 0, SRCU_sli_CB, 0.0001, 1.9999,1.,"12");
      // SRCU_sl1 = GUI_Slider__ (&box0, 0, SRCU_sli_CB, -2., 2., 0., "12");
      SRCU_sl1 = GUI_Slider__ (&box0, 0, SRCU_sli_CB, -1.5, 1.5, 0., "40");
      GUI_Tip  ("increment / decrement value ..");
      GUI_spc__ (&box0, 0, 2);

      SRCU_b1 = GUI_button__ (&box0, " Reset ",                  // RESET-button
                         SRCU_win__, &GUI_FuncUCB1, "");
                GUI_button__ (&box0, MSG_const__(MSG_cancel),    // CANCEL-button
                         SRCU_win__, &GUI_FuncKill, "");
      SRCU_b3 = GUI_button__ (&box0, MSG_const__(MSG_save),      // SAVE-button
                         SRCU_win__, &GUI_FuncExit, "");

      // block RESET-button & SAVE-button
      GUI_set_enable (&SRCU_b1, 0);
      GUI_set_enable (&SRCU_b3, 0);


      L_exit:
      // GUI_Win_go (&SRCU_tb);
      GUI_set_show (&SRCU_tb, 1);  // show


      // MSG_pri_0 ("SRCU_in"); // SRCUure: select 2 points ..
      return (0);


    //---------------------------------------------------------
    case UI_FuncUCB1:  // - RESET
        // printf(" reset.val.\n");
 
      // show defVal in e2
      SRCU_val_cen = atof (SRCU_val_def);
      SRCU_win_e2 (&SRCU_val_cen, 1);

      // update model
      SRCU_mod__ (SRCU_val_def);

      // // work including 1. line
      // SRCU_work__(2);

      GUI_set_enable (&SRCU_b1, 0);
      GUI_set_enable (&SRCU_b3, 0);
      SRCU_isModified = 1;
      return 0;
  

    //---------------------------------------------------------
    case UI_FuncUCB2:  // isModified
      GUI_set_enable (&SRCU_b1, 1);
      GUI_set_enable (&SRCU_b3, 1);
      SRCU_isModified = 0;   // 0=modif; 1=unmodif.
      return 0;


    //---------------------------------------------------------
    case UI_FuncExit:  // save & exit
        // printf(" save & exit\n");
      // get p1 = active value
      p1 = GUI_entry_get (&SRCU_e2);
      // modify line
      SRCU_pri_mod (p1);
      // change line in memory
      APED_src_chg (SRCU_obj_pri.lnr, SRCU_ln_pri);
      // man muss nun echtes execute fahren,
      // weil sonst die lineNrs in der dl-tab falsch sind ..
      ED_work_CurSet ((int)SRCU_obj_pri.lnr);  // reset to line 
      ED_work_CurSet (999999);                 // work until end
      SRCU_work__(9); // free
      SRCU_exit__ ();
      return (0);
 

    //---------------------------------------------------------
    case UI_FuncKill:  // cancel & exit
        // printf(" cancel & exit\n");
      SRCU_work__(2); // work including 1. line
      SRCU_work__(9); // free
      SRCU_exit__ ();
      return (-1);
  }

  return (-1);

}


//================================================================
  int SRCU_tsMax (char *sSrc) {
//================================================================
// returns max nr of sourceObjects in sourceString <sSrc> 
// sSrc must be '\0'-terminated

  // int   its;
  // its = 10 + strlen(sSrc) / 2;
  // its = 8 + (strlen(sSrc)/3);

    // printf("ex SRCU_tsMax %d\n",its);

  return 6 + UTX_chrNr (sSrc) / 2;

}


//================================================================
  int SRCU_analyz__ (int typ, long dbi, int valNr) {
//================================================================
// sets SRCU_obj_pri & SRCU_val_pri   from obj typ,dbi,valNr
//
// find definition-line; analyze line; find value # <valNr>.
//
// SRCU_obj_pri = primary-obj (to be modified); position of codeline ..
// SRCU_codeOff = offset of value # <valNr> in codeline
// SRCU_val_pri = position and length of value in codeline
// SRCU_val_def = default-value (string)

  int       i1, ll, itsMax, itsAct, iLast;
  long      lNr;
  char      *p1, *p2, oid[32];
  ObjTXTSRC *tso;


  // printf("SRCU_analyz__ %d %ld %d\n",typ,dbi,valNr);


  // get definition-line for db-obj typ,dbi;
  APED_oid_dbo__ (oid, typ, dbi);
  iLast = APED_search_defLn (&p1, &lNr, &ll, oid, -1L, 0);
  if(iLast < 0) return -1;
    // printf(" lNr=%ld ll=%d iLast=%d|",lNr,ll,iLast);
    // UTX_dump_cnl(p1, 60);printf("|\n");
    // printf(" lyz |%s|\n",p1);


  SRCU_obj_pri.lPos = p1;
  SRCU_obj_pri.lnr  = lNr;
  SRCU_obj_pri.ll   = ll;


  // keep a copy of the definition-line of the primary obj
  SRCU_ln_pri = IE_outTxt;  // size is mem_cbuf1_SIZ
  strncpy (SRCU_ln_pri, p1, ll);
  SRCU_ln_pri[ll] = '\0';

  // cut objName of SRCU_ln_pri
  APED_onam_cut (SRCU_ln_pri, NULL);

  // skip definition-header
  p2 = strchr (p1, '=');
  if(!p2) {
    TX_Print("***** SRCU_analyz__: cannot find definition-header..");
    return -1;
  }
  ++p2; // skip '='
  // p2 is now the codeStartposition = offset (ObjTXTSRC.ioff) here is 0
  SRCU_codeOff = p2 - p1;

  // cut definitionline (change '\n' into '\0')
  p1[ll] = '\0';


  // get position and lenght of value nr <valNr> in the codeline.
  // pos: ObjTXTSRC.ioff; len: ObjTXTSRC.ilen.
  // SRCU_val_prir = ?


  // strcpy (s1, "P(-63 188 V20) ANG(360) Z(30 + SR_2) \"string\"");
  // strcpy (s1, "CUT A123 RAD_1 REV");
  itsMax = SRCU_tsMax (p2);
    // printf(" itsMax=%d siz=%d\n",itsMax,itsMax * sizeof(ObjTXTSRC));

  tso = MEM_alloc_tmp ((int)(itsMax * sizeof(ObjTXTSRC)));
  // SRC_txo_getSpc__ (&tso); // get memspc & init
  itsAct = APED_txo_srcLn__ (tso, itsMax, p2);
    // printf(" _txo_srcLn__ %d\n",itsAct);
    // if(itsAct > 0) APED_txo_dump (tso, p2);


  // restore definitionline (change '\0' into '\n')
  if(iLast == 0) p1[ll] = '\n';


  // find value nr <valNr>; save into SRCU_val_pri
  i1 = SRCU_analyz_val (tso, valNr);
  if(i1 < 0) return -1;


  // keep size of the value in characters
  SRCU_val_siz = SRCU_val_pri.ilen;


    // printf(" SRCU_val_pri ioff=%d ilen=%d _val_=|%s|\n",
           // SRCU_val_pri.ioff, SRCU_val_pri.ilen,SRCU_val_def);

  
  return 0;

}


//================================================================
  int SRCU_analyz_val (ObjTXTSRC *tso, int valNr) {
//================================================================
// find value nr <valNr>; save into SRCU_val_pri
//   valNr        first is 0

  int  its=-1, actValNr=0;
  char *p1;


  // printf("SRCU_analyz_val %d\n",valNr);

  for(;;) {
    ++its;
    if(tso[its].typ  == TYP_FuncEnd) break;
    if(tso[its].form != Typ_NumString) continue;
    if(actValNr < valNr) {
      ++actValNr;
      continue;
    }
    // found
      // printf(" found tso[%d]\n",its);
    SRCU_val_pri = tso[its];
    // copy the default-value -> SRCU_val_def
    p1 = SRCU_obj_pri.lPos + SRCU_codeOff + SRCU_val_pri.ioff;
    strncpy (SRCU_val_def, p1, SRCU_val_pri.ilen);
    SRCU_val_def[SRCU_val_pri.ilen] = '\0';
      // printf(" SRCU_val_def=|%s|\n",SRCU_val_def);
    return 0;
  }


  return -1;

}


//================================================================
  int SRCU_mod__ (char *newVal) {
//================================================================
// update model
 

  int   irc;


  // printf("============================================== \n");
  // printf("SRCU_mod__ \n");


    // modify the primary-sourceline SRCU_ln_pri
    SRCU_pri_mod (newVal);

    // execute the modified primary-sourceline
    ED_enter ();  // display also temp.objects
    irc = SRCU_work__ (3);
    if(irc) return -1;

    // rework all child-objects of the primary-obj
    ED_go ();     // do not display temp.objects
    SRCU_work__ (1);

  return 0;

}


//================================================================
  int SRCU_pri_mod (char *sNew) {
//================================================================
// modify the primary-sourceline SRCU_ln_pri
// see SRCU_analyz_mod

  int   il, pDel;
  long  l1, ld;
  char  *s1;


  // printf("SRCU_pri_mod |%s|\n",sNew);


  // change line in string
  pDel = SRCU_codeOff + SRCU_val_pri.ioff;  // start delete here
  ld = SRCU_val_siz;                        // delete-size
    // printf(" pDel=%d ld=%ld\n",pDel,ld);
  UTX_chg_str1 (&l1, SRCU_ln_pri, sNew, &SRCU_ln_pri[pDel], ld);
    // printf(" modLn=|%s|\n",SRCU_ln_pri);

  SRCU_val_siz = strlen (sNew);


  return 0;

}

/*
//================================================================
  int SRCU_analyz_mod (char *sNew) {
//================================================================
// save & exit

  int   il, pDel;
  long  l1, ld;
  char  *s1;


  printf("SRCU_analyz_mod |%s|\n",sNew);
  printf("  lnr=%ld\n",SRCU_obj_pri.lnr);       // lineNr
  printf("  len=%d\n",SRCU_obj_pri.ll);         // lineLength
  printf("  |");UTX_dump_cnl(SRCU_obj_pri.lPos,60);printf("|\n");
  printf("  off=%d pos=%d len=%d\n",SRCU_codeOff,
         SRCU_val_pri.ioff, SRCU_val_pri.ilen);


  // modify line
  SRCU_pri_mod (sNew);

  // change line in memory
  APED_src_chg (SRCU_obj_pri.lnr, SRCU_ln_pri);


  return -1;

}
*/
 
//================================================================
  int SRCU_win_e2 (double *dVal, int mode) {
//================================================================
/// \code
/// SRCU_win_e2         set / get  act. value
///  mode  0=get 1=set,
/// \endcode

  char     s1[32], *p1;


  // 0=get
  if(mode == 0) {
    p1 = GUI_entry_get (&SRCU_e2);
    *dVal = atof (p1);


  } else {
    // set act. value in e2
    s1[0] = '\0';
    UTX_add_fl_u (s1, *dVal);
    GUI_entry_set (&SRCU_e2, s1);
  }

  return 0;

}


//================================================================
  int SRCU_key_CB (int key) {
//================================================================
// GUI_KeyEsc: clear 

  char      *p1;


  printf("SRCU_key_CB %d\n",key);


  switch (key) {


    case GUI_KeyReturn:
    case GUI_KeyEsc:
    case 'q':
    case 's':
      // exit, do not save ..
      SRCU_exit__ ();
      return 1;  // do no defaultOperations

    default:
      break;
  }

  return 0;  // do defaultOperations

}


//=========================================================
  int SRCU_sel_CB (int src, long dli) {
//=========================================================
// userselection callback
// put selNam into first empty field; entry1 or entry2
//   if none is empty: overwrite entry2.
// if both fields not empty: call SRCU_work_pp



  // static Point  pt1 = UT3D_pt_NEW;
  // static Point  pt2 = UT3D_pt_NEW;


  int    irc, typ;
  long   dbi, dl1;
  char   *p1, *p2, *selNam, s1[256];


  printf("SRCU_sel_CB src=%d ind=%ld\n",src,dli);


  if(src == GUI_MouseR) {
    SRCU_exit__ ();
    return 0;
  }


  AP_errStat_reset (0);       // clear error

  return 0;

}


//===================================================================
  int SRCU_sli_CB (MemObj *mo, void **data) {
//===================================================================
// callback slider


  // double    d1, d2;


  //----------------------------------------------------------------
  if(GUI_DATA_EVENT == TYP_EventPress) {
      // printf(" slider-press\n");
    // SRCU_work_upd (GUI_DATA_D1);
    // start timer
    SRCU_tmr_stat = 1;
    GUI_timer__ (SRCU_tmr_CB__, 100, NULL);
    return 0;
  }
      

  //----------------------------------------------------------------
  if(GUI_DATA_EVENT == TYP_EventRelease) {
      // printf(" slider-release\n");
    // stop timer
    SRCU_tmr_stat = 0;
    return 0;
  }


  //----------------------------------------------------------------
  if(GUI_DATA_EVENT == TYP_EventMove) {
    SRCU_fakt = GUI_DATA_D1;
      // printf(" slider-move %lf\n",SRCU_fakt);
    return 0;
  }


  return 0;

}


//================================================================
  int SRCU_tmr_CB__ (void *data) {
//================================================================
// SRCU_tmr_CB__          timer callback
// return 0; stops timer.

  int       irc;
  double    d1, d2;
  char      *p1;



  // printf("SRCU_tmr_CB__ \n");

  // get active val
  SRCU_win_e2 (&d1, 0);


  if(SRCU_tmr_stat == 0) {
    // reset slider
    GUI_Slider_set (&SRCU_sl1, 0.);
    SRCU_fakt = 0.;
    // set SRCU_val_cen to active value !
    SRCU_val_cen = d1;


  //----------------------------------------------------------------
  } else {
    // update primary obj

    // compute new val d1
    if(fabs(SRCU_fakt) > 1.) {
      // move SRCU_val_cen
      SRCU_val_cen += (SRCU_scal * DSIGN (SRCU_fakt) * 0.1);
    }
    d1 = SRCU_val_cen + (SRCU_scal * SRCU_fakt);

    // display new val
    SRCU_win_e2 (&d1, 1);

    // update model
    SRCU_mod__ (GUI_entry_get (&SRCU_e2));

  }

  return SRCU_tmr_stat;  // stop|resume

}


//===================================================================
  int SRCU_ent_CB (MemObj *mo, void **data) {
//===================================================================
// new value entered into entryField


  int     irc, typ;
  long    dbi;
  double  d1;
  char    *p1;


  // printf("SRCU_ent_CB GUI_OBJ_TYP=%d\n",GUI_OBJ_TYP(mo));
  // printf(" event=%d\n",GUI_DATA_EVENT);
  // printf(" data=|%s|\n",GUI_DATA_S1);
  // printf(" key=%d\n",GUI_DATA_I2);


  if(GUI_DATA_EVENT == TYP_EventPress) {
    if(GUI_DATA_I2 == GUI_KeyReturn) {
      if(!strcmp(GUI_DATA_S1, "e2")) goto L_upd2;
      // else                           goto L_upd2;
    }
  }

  return 0;


  //----------------------------------------------------------------
  // CR in SRCU_e2: new value
  L_upd2:
  p1 = GUI_entry_get (mo);
  d1 = atof(p1);

  // show defVal in e2
  SRCU_win_e2 (&d1, 1);


// use SRCU_mod__  !
  // modify the primary-sourceline SRCU_ln_pri
  SRCU_pri_mod (p1);

  // execute the modified primary-sourceline
  SRCU_work__ (3);

  // rework all child-objects of the primary-obj
  SRCU_work__ (1);

  return 0;

}


//================================================================
  int SRCU_exit__ () {
//================================================================

  // printf("SRCU_exit__ \n");


  DL_hili_off (-1L);             //  -1 = unhilite all
  GL_temp_del_all ();              // unhilite vectors


  // remove Toolbar
  SRCU_tb = GUI_toolbox_del (&SRCU_tb);

  // restore functions...
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

  AP_User_reset ();

  // GL_temp_del_all (); // alle temp. obj loeschen ..

  // // uncheck checkbox "Measure"
  // UI_ckb_meas (FALSE);

  AP_errStat_reset (0);
  WC_set_obj_stat (SRCU__obj_stat);    // reset APT_obj_stat
  ED_set_mode (SRCU__ED_stat);         // reset ED_mode

    // printf("ex SRCU_exit__ \n");
  return 0;

}


//================================================================
  int SRCU_work_upd (double d1) {
//================================================================
// modify value and update

  int  i1;


  // printf("SRCU_work_upd %lf\n",d1);


  // check if mousebutton 1 is down ..
  L_nxt:
  i1 = GUI_get_keys_mod ();
    printf(" GUI_get_keys_mod %d\n",i1);


  // if(i1 & 256) { 
    // // printf(" mouseButt. 1 is on\n");
    // GUI_update__ ();
    // goto L_nxt;
  // }


  return 0;

}


//================================================================
  int SRCU_work__ (int mode) {
//================================================================
// Inputs:
//   mode       0=init
//              1=work all lines excluding 1.line (prim.obj)
//              2=work all lines including 1.line (prim.obj)
//              3=work modified primary-sourceline SRCU_ln_pri
//              9=close (free mem)
//   SRCU_obj_pri         primary obj
 

  int    typ, inr, irc, *ia1, *ia2;
  long   dbi, dli, lnr, l1, l2, iSta;
  double d1;
  char   *p1;


  static MemTab(ObjSRC) depTab = _MEMTAB_NUL;


  // printf("SRCU_work__ %d\n",mode);


  // set primary object:
  typ = SRCU_obj_pri.typ;
  dbi = SRCU_obj_pri.dbi;


  //================================================================
  if(mode != 0) goto L_upd1;    // 0=init

  // get all depending objects, their codeLineNrs, their dli's; not yet srcCode.
  MemTab_ini (&depTab, sizeof(ObjSRC), Typ_ObjSRC, 400);
  APED_find_dep__ (&depTab, typ, dbi);
    // MemTab_dump (&depTab);
  // first obj is primary object.


  // keep the dli of the primary-obj
  SRCU_obj_pri.dli = depTab.data[0].dli;
    // printf(" prim.dli=%ld\n",SRCU_obj_pri.dli);


  // get pointers to code for codelines
  inr = depTab.rNr;
  // spa = (char**)MEM_alloc_tmp (inr * sizeof(void*));
  // if(!spa) {TX_Error("funcXX EOM "); return -1;}
  for(l1=0; l1 < inr; ++l1) {
    lnr = depTab.data[l1].lnr; // lna[l1];
    // spa[l1] = UTF_GetLinNr (NULL, &l2, lnr);
      // printf("%ld |",l1);UTX_dump_cnl (spa[l1], 60); printf("|\n");
    depTab.data[l1].lPos = UTF_GetLinNr (NULL, &l2, lnr);
      // printf("%ld |",l1);UTX_dump_cnl(depTab.data[l1].lPos,60);printf("|\n");
  }


  // sort
  ia1 = (int*)MEM_alloc_tmp ((int)(inr * sizeof(int)));
  if(!ia1) {TX_Error("SRCU_work__ EOM1"); return -1;}
  ia2 = (int*)MEM_alloc_tmp ((int)(inr * sizeof(int)));
  if(!ia2) {TX_Error("SRCU_work__ EOM2"); return -1;}
  for(l1=0; l1 < inr; ++l1) ia1[l1] = depTab.data[l1].lnr;
  UTI_ni_ind_sort (ia2, ia1, inr); // sort
    // for(l1=0; l1 < inr; ++l1) printf(" ia2[%ld] = %d\n",l1,ia2[l1]);
  // save indexTab ia2 in depTab.iPar
  for(l1=0; l1 < inr; ++l1) depTab.data[l1].iPar = ia2[l1];

  return 0;



  //================================================================
  L_upd1:          // 3=work modified primary-sourceline SRCU_ln_pri
  if(mode != 3) goto L_upd2;    // 0=init

    dli = SRCU_obj_pri.dli;
    lnr = SRCU_obj_pri.lnr;
      // printf("  lnr=%ld dli=%ld\n",lnr,dli);
      // printf(" _ln_pri=|%s|\n",SRCU_ln_pri);


    if(dli >= 0) DL_SetInd (dli);
    irc = WC_Work_upd (lnr, SRCU_ln_pri);
    DL_Redraw ();
    GUI_update__ ();
    return irc;




  //================================================================
  L_upd2:          // 1,2 update; rework all codelines from depTab

  if(AP_errStat_get()) return -1;

  if(SRCU_isModified)
    SRCU_win__ (NULL,GUI_SETDAT_EI(0,UI_FuncUCB2));      // isModified
  if(mode == 1)      iSta = 1;
  else if(mode == 2) iSta = 0;
  else goto L_exit;

  inr = depTab.rNr;
  for(l1=iSta; l1 < inr; ++l1) {
    // reExecute line nr lna[l1]
    l2  = depTab.data[l1].iPar;   // get correct index
    dli = depTab.data[l2].dli;
    lnr = depTab.data[l2].lnr;
    p1  = depTab.data[l2].lPos;
      // printf(" update ii=%ld lnr=%ld dli=%ld\n",l2,lnr,dli);
    if(dli >= 0) DL_SetInd (dli);
    irc = WC_Work_upd (lnr, p1);
    if(irc) return irc;
  }
  DL_Redraw ();
  GUI_update__ ();
  return 0;



  //================================================================
  L_exit:
  // if(mode == 9) 
  MemTab_free (&depTab);
  return 0;


  //----------------------------------------------------------------
  // change line
  // APED_src_chg


  L_err: return -1;

}


//================================================================
  int SRCU_list_vals__ (ObjTXTSRC *tso, char *sl) {
//================================================================
// SRCU_list_vals__              get list of all value-objects
// Deletes all not-value-records.

// see also SRC_txo_srcLn__


  int   its=0, i1, i2;
  char  fmtBuf[32];


  // printf("\nSRCU_list_vals__ |%s|\n",sl);
    // SRC_txo_dump (tso, sl);


  // get nr of records
  for(;;) {
    if(tso[its].typ == TYP_FuncEnd) break;

    // set ipar = parent(function)typ only for num.strings
    if(tso[its].form == Typ_NumString) {
      i2 = tso[its].ipar;
        // printf(" _list_vals__ its=%d i2=%d\n",its,i2);
      if(i2 >= 0) {
        tso[its].ipar = tso[i2].form;
      }
    }

    ++its;
  }
    // SRC_txo_dump (tso, sl);


  for(i1=0; i1<its; ++i1) {
    if(tso[i1].typ == TYP_FuncEnd) break;
    if(tso[i1].form == Typ_NumString) continue;

    // delete record
      // printf(" _list_vals__ del %d\n",i1);
    MEM_del_nrec (&its, tso, i1, 1, sizeof(ObjTXTSRC));

    --i1;  // test this record again ..
  }

  // must restore termination-record
  tso[its].typ = TYP_FuncEnd;

    // SRC_txo_dump (tso, sl);

  return 0;

}


//================================================================
  int SRCU_txt_popCoord (char *sMen, int typ, int parTyp, int iNr) {
//================================================================
// fix string for popup-menu for modify-value.
// "modify-Val/0"
// "modify-PT/X-coord"
// "modify-Vec/Y-coord"

  // static   char *sa1[]={"X","Y","Z"};

  int      ii;


  // printf("SRCU_txt_popCoord typ=%d parTyp=%d iNr=%d\n",typ,parTyp,iNr);

    if((parTyp == Typ_VC)||(parTyp == Typ_PT)) {
      // ii = IMIN(iNr, 2);
      ii = iNr + 1;
      sprintf (sMen,"modify-%s/%d/%s",AP_src_typ__(parTyp),ii,
               AP_src_typ__(typ)); //, sa1[ii]);

    } else {
      ii = iNr + 1;
      sprintf (sMen,"modify-%s/%d",AP_src_typ__(typ),ii);
    } 

  return 0;

}
 

// EOF
