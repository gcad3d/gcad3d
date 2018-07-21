// xa_cved.c                                      2018-02-16  RF
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
    ..

-----------------------------------------------------
Modifications:
2018-02-16 neu erstellt. RF.

-----------------------------------------------------
*/
#ifdef globTag
 void CVED(){}
#endif
/*!
\file  ../xa/xa_ped.c
\brief CurveEditor 
\code
=====================================================
List_functions_start:

CVED_init         decode obj; init toolbar
CVED_win__        create Toolbar
CVED_win_act      activate/deactivate Insert/Delete
CVED_lock         lock mainMenu; unlock Save|cancel
CVED_unlock       unlock mainMenu; lock Save|cancel
CVED_CB1          callbacks
CVED_points       temppoint-functions
CVED_save__       save (modify src & write -> tempfile)
CVED_exit         insert new code, redraw all
CVED_restore1     overwrite DB-points UNUSED
CVED_restore2     get sourceline, execute .. UNUSED
CVED_restore3     update model starting with line lNr
CVED_src_mod      compare all points; modify points
CVED_src_del      delete point actInd in srcLine
CVED_src_ins      insert point in srcLine
CVED_compare      compare points - create new one
CVED_replace      replace or skip point in ObjSRC  UNUSED
CVED_get_ato      get & decode the sourceObj
CVED_get_src      get the sourceObj from tempfile
CVED_get_ptab     get all points of actObj into actPta
CVED_get_tabInd   get index into CVED_atab from pointNr actInd
CVED_wri_src      save sourceLine -> tempfile
CVED_rea_src      read file into memstring

CVED_get_offObj   get offsetObject (Plane or Vector) out of CVED_w_off
CVED_del__        delete point actInd
CVED_ins_pt       get segmentnr where to insert point & save point

CVED_key_CB
CVED_sel_CB       userselection callback
CVED_mousemove_CB callback mousemovement
CVED_off_CB       cb of entry CVED_w_off
CVED_len_CB       cb of entry CVED_offLen
CVED_upd_stop     exit update proc
CVED_newPos__     project cursorPosition onto offsetvector

CVED_dump_ptab
CVED_dump_atab

List_functions_end:
=====================================================
see also
UI_WinTra__       alter modelPositionEditor

\endcode *//*----------------------------------------



----------------------------------
PED - Pointeditor:
Is using Selection-callback & Mousemove-callback (like plugins !)

Startup:   CVED_init
Shutdown:  CVED_key_CB | CVED_sel_CB > CVED_CB1


Input: object with points to be modified.
- get sourceline, copy it to file ../tmp/selection.txt
- decode sourceline into atomicObj, keep im memory in CVED_atyp,CVED_atab[CVED_anr].
- get all points, keep in memory in actPta.
- display points as yellow start

Modify, Delete:
- user selects point; set actPtp = pointer into DB for this point
- modify this point, redraw obj with CVED_points(6);
  - APT_store_obj, APT_Draw__.
- save: modify the sourceline (in tempfile)

Insert:
- user selects positon on curve; get parametric description of point,
  get segment on curve from its parameter, insert new point.


CVED_init
  ACVED_find_dep__
  CVED_dep_sav
  CVED_get_ptab
    CVED_get_ato
      CVED_get_src
      APT_decode__


CVED_mousemove_CB
  CVED_points (6, 9);
    APT_Draw__
    CVED_mod_dep_draw__


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


#define CVED_SYM_COL   5                // color for pointSymbols (star) GL_col_tab
#define CVED_SYM_ID    SYM_TRI_B        // ID for pointSymbols



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
static long  actPti;            // index of active point in DB / CVED_atab
static int   actPtn;            // nr of points of actPta
static int   actInd;            // point to modify; index into actPta
static long  actDli;            // dispListIndex
static long  dynPti;

static int    *CVED_atyp;        // type of objects of sourcecode to edit ..
static double *CVED_atab;        // data of objects of sourcecode to edit ..
static int    CVED_anr = 0;      // nr of objects in CVED_atyp,CVED_atab

static int    CVED_mode=-1;      // 0=modify, 1=insert, 2=delete
static int    CVED_stat=0;       // 0=move is not active; 1=active;
                                // 2=SaveCancel is active.
static int    CVED_changed=0;    // 0=not-changed; nr-of-Saves

static Plane  CVED_offObj;       // vector or plane
static int    CVED_offTyp;
static double CVED_offLen = 0.;

static MemObj CVED_w_off;    // entry offsetObj
static MemObj CVED_w_len;    // entry length
static MemObj CVED_f_mod;    // frame Modify
static MemObj CVED_f0;       // primary frame
static MemObj CVED_f_sc;     // frams Save/Cancel
static MemObj CVED_wa[4];    // Radiobutts 0=Delete 1=Insert; 2=Modify; 3=Exit

static MemTab(ObjSRC) depTab = MemTab_empty;

static FILE      *CVED_fp_dep = NULL;

// PROTOS:
  MemObj CVED_win__ (MemObj *parent, void *data);
  int CVED_CB1 (MemObj *mo, void **data);
  int CVED_off_CB (MemObj *mo, void **data);
  int CVED_len_CB (MemObj *mo, void **data);
  int CVED_mousemove_CB (int dx, int dy);
  int CVED_key_CB (int key);
  int CVED_sel_CB (int src, long dl_ind);
  int CVED_link_CB (MemObj *mo, void **data);
  int CVED_get_src (char *cBuf, int bufSiz);
  Point CVED_newPos__ ();




//================================================================
  int CVED_init (int typ, long dbi, long dli) {
//================================================================
// main-entrypoint 'move_points_of_obj'
 
  int   irc, i1, form, lNr, parTyp;
  long  l1, l2, icp, parDbi;
  char  cbuf[160];


  return -1;

}

