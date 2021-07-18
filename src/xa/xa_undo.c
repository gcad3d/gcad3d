//   Undo-Window-Funktions.             RF 2002-05-22
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
- commentlines of apps cannot be deleted; make "BLOCK APP .."
  - see INF_UNDO__

- for 2-line-records (u2=2) initial state of u1 seems to be wrong ...

-----------------------------------------------------
Modifications:
2015-08-17 UNDO_app__ new. RF.

-----------------------------------------------------
*/
#ifdef globTag
 void UNDO(){}
#endif
/*!
\file  ../xa/xa_undo.c
\brief undo-functions 
\code
=====================================================
List_functions_start:

UNDO_init1         clear undoTab
UNDO_init2         setup icons (delete=red-arroy; restore=green-arrow)
UNDO_clear         clear all groups

UNDO_grp_add       add new group to undoTab
UNDO_chg_add       save line-modified-record
UNDO_chg_ins       increment all linenumbers > lNr
UNDO_app__         handle undo for plugins (no redo)

UNDO_grp_undo      delete next group
UNDO_grp_redo      restore next group
UNDO_grp_del       delete group
UNDO_grp_res       restore group
UNDO_ln_del        delete sourceLine lNr
UNDO_ln_res        restore line

UNDO_recNr__       get recordNr of last group in table
UNDO_grp_ck        test if group already exists in undoTab
UNDO_del_active    check if one of the previous groups can be deleted
UNDO_res_active    check if one of the next groups can be restored
UNDO_set_bt        activate/disactivate  delete/restore-buttons
UNDO_upd_lb        update Label

UNDO_dump

List_functions_end:
=====================================================
// UNDO_upd_bt        activate/disactivate  delete/restore-buttons
// UNDO_lock          enable / disable icons
// UNDO_grp_clr       delete record and all following records
// UNDO_grpNr_recNr   get groupNr from recordnr
// UNDO_recNr_grpNr   get recordNr from groupNr
// UNDO_grp_upd       delete all following groups

\endcode *//*----------------------------------------




undoObj undoTab[]:
  grpNr    first obj of grp has grpNr=0; second obj of grp has grpNr=1 ..
  lNr      sourceLineNr
  u1       'a'=active; 'd'=deleted;
  u2       '1'=1-line-record; '2'=change-record(consists of 2 lines)


---------------------------
Types of undoTab-records:

 lNr=8 grpNr=0 u1=d u2=1       object lNr=8 is deleted;
 lNr=8 grpNr=0 u1=a u2=1       object lNr=8 is restored;

 lNr=7 grpNr=0 u1=d u2=1       group of 2 deleted objects
 lNr=8 grpNr=1 u1=d u2=1

 lNr=12 grpNr=0 u1=d u2=2      2-line-record, 1. line (11) is deleted; 
                                 2. line (12) is active

---------------------------
Change-Records (u2='2'):
  necessary if eg a value has be changed (eg P(L20 0.5) -> P(L20 0.75))
  Create change records by eg "edit point".
  A change-record has 2 sourceLines
  .lNr is the new sourceLine; .u2='2';
  the correponding old sourceLine is the line before (the first line).
  .u1 ('a' or 'd') refers to the new line (the second line = lNr).
  The sourceLine before .lNr must be treated in reverse.


---------------------------
Functions:
  Ctrl-Y      undo: restore, then delete, then restore last undoTab-record



---------------------------
Workflow:
// add undo-record:
UNDO_grp_add


// delete:
UNDO_grp_add
UNDO_grp_del


// Undo-Button:
UNDO_grp_undo    
  UNDO_grp_del
  UNDO_grp_res


// Redo-Button:
UNDO_grp_redo    



---------------------------
UNDO-Tests:
- add new obj; delete ..
- delete single obj; restore ..
- delete group of obj's; restore ..
- modify single obj (move point); restore ..
- modify obj (CAD-Edit); restore ..


*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>                      // memcmp memcpy

#include "../ut/ut_geo.h"
#include "../ut/ut_txt.h"
#include "../ut/ut_txfil.h"
#include "../ut/ut_memTab.h"           // MemTab
#include "../ut/ut_os.h"               // OS_get_ico_dir
#include "../ut/ut_TX.h"               // TX_Print

#include "../gui/gui__.h"         // incl. /gui/gui_types.h /gui/gui_base.h

// #include "../ut/func_types.h"               // UI_FuncSet

#include "../xa/xa.h"                        // AP_STAT
#include "../xa/xa_ui.h"
#include "../xa/xa_undo.h"
#include "../xa/xa_ato.h"              // ATO_getSpc_tmp__
#include "../xa/xa_ed_mem.h"              // typedef_MemTab(ObjSRC)
// #include "../xa/opar.h"                   // MemTab_ini_temp



#define   TRUE 1
#define   FALSE 0


// undoObj undoTab[]:
//   grpNr    first obj of grp has grpNr=0; second obj of grp has grpNr=1 ..
//   lNr      sourceLineNr
//   u1       'a'=active; 'd'=deleted;
//   u2       '1'=1-line-record;
//            '2'=change-record(consists of 2 lines)
typedef struct {long lNr; short grpNr; char u1, u2; }            undoObj;


typedef_MemTab(undoObj);



//================================================================
// EXTERNALS:

// aus xa.h:
extern AP_STAT   AP_stat;


//===========================================================================
// LOCALS:


static MemObj  wUndo, btUndo,  // delete-icon   red-arrow
                      btRedo;  // restore-icon  green-arrow

static MemTab(undoObj) undoTab = _MEMTAB_NUL;






//================================================================
  int UNDO_recNr__ () {
//================================================================
// UNDO_recNr__            get recordNr of last group in table
// find recordNr of last record in undoTab with .grpNr=0


  int iRec = undoTab.rNr - 1;

  while((MEMTAB__(&undoTab,iRec))->grpNr) --iRec;

  return iRec;

}


/* UNUSED
//================================================================
  int UNDO_lock (int mode) {
//================================================================
// 0=lock; 1=unlock
// see also UI_func_stat_set__ (APF_UNDO)

  printf("UNDO_lock %d\n",mode);


  if(mode == 0) {     // lock
    GUI_set_enable (&btUndo, FALSE);
    GUI_set_enable (&btRedo, FALSE);


  } else {            // unlock
    UNDO_set_bt ();

  }

  return 0;

}
*/

//================================================================
  int UNDO_init1 (MemObj *parent) {
//================================================================
// delete all undoRecords.


  // printf("UNDO_init1 \n");


  // if(undoTab == NULL) undoTab = malloc (sizeof(undoObj) * 100);
  MemTab_ini__ (&undoTab, sizeof(undoObj), Typ_Group, 100);


  // create Label wUndo (display nr of records in undoTab)
  wUndo = GUI_label__ (parent, "  0", "r");

  return 0;

}


//================================================================
  int UNDO_init2 (MemObj *parent) {
//================================================================
// setup icons (delete=red-arroy; restore=green-arrow)

  char   fn[128];


  // printf("UNDO_init2 \n");


  sprintf (fn, "%sArrowl.png", OS_get_ico_dir());
  btUndo = GUI_button_img (parent, fn, UI_menCB, (void*)"ObjDelete", "e");
    MSG_Tip ("MMundo");


  sprintf (fn, "%sArrowr.png", OS_get_ico_dir());
  btRedo = GUI_button_img (parent, fn, UI_menCB, (void*)"ObjRestore", "e");
    MSG_Tip ("MMredo");


  // deaktivieren
  UNDO_set_bt (0);


  return 0;

}


//================================================================
  int UNDO_grp_range_add (long lNr1, long lNr2) {
//================================================================
// UNDO_grp_range_add       add undo-records for src-lines <lNr1> - <lNr2> 
// - not including <lNr2>
// TODO: see INF_UNDO__
 
  long   l1;
  int    grpNr=0;

  printf("UNDO_grp_range_add %ld - %ld\n",lNr1,lNr2);

  for(l1=lNr1; l1<lNr2; ++l1) {
// TODO: test if line <l1> is not comment-line or empty
    UNDO_grp_add (l1, grpNr);
    ++grpNr;
  }

  return 0;

}


//================================================================
  int UNDO_grp_add (long lNr, int grpNr) {
//================================================================
/// \code
/// add sourceObj to do/undo-list 
/// for delete:  call UNDO_grp_del() after UNDO_grp_add().
/// for restore: call UNDO_grp_res() after UNDO_grp_add().
/// Input:
///   lNr       lineNr of sourceObj
///   grpNr     0 for first obj to add to list, 1 for second ..
/// Output:
///   retCod    iRec of new group
/// \endcode

// iUpd    0=update display; 1=do not update display (yet)
// UNDO_upd__ ();     // update buttons necessary !
// was UI_undo_add_


  long      ld;
  undoObj   o1;


  // printf("--------- UNDO_grp_add %ld %d\n",lNr,grpNr);
  // UNDO_dump ("UNDO_grp_add");


  // test if group already exists in undoTab; yes: delete.
  UNDO_grp_ck (lNr);
    // UNDO_dump ();


/*
  // delete all following groups
  undoTab.rNr = UNDO_grp_upd ();
    // UNDO_dump ();
*/


  // add new record to undoTab
  o1.lNr   = lNr;
  o1.grpNr = grpNr;
  o1.u1    = 'a';
  o1.u2    = '1';
  MemTab_sav (&undoTab, &ld, &o1, 1);


    // printf("============== exit UNDO_grp_add =====\n");
    // UNDO_dump ("ex-_grp_add");


  return 0;

}


/*
//================================================================
  int UNDO_recNr_grpNr (int iGrp) {
//================================================================
/// \code
/// get recordNr from groupNr
/// returns the index into undoTab
/// \endcode

  int    i1, ig, ir;

  // printf("UNDO_recNr_grpNr %d\n",actGrp);
    
  if(iGrp < 1) return 0;

  ig = 0;
  ir = undoTab.rNr - 1;

  for(i1 = 0; i1 < undoTab.rNr; ++i1) {
    if(undoTab.data[i1].grpNr != 0) continue;
    if(ig == iGrp) {ir = i1; break; }
    ++ig;
  }
  
    printf("ex UNDO_recNr_grpNr ir=%d ig=%d\n",ir,iGrp);

  return ir;

}

//================================================================
  int UNDO_grpNr_recNr (int iRec) {
//================================================================
// get groupNr from recordnr
// groupNr = nr of (grpNr==0) - records in undoTab

  int    i1, ig;

  printf("UNDO_grpNr_recNr %d\n",undoTab.rNr);

  if(undoTab.rNr < 1) return -1;

  ig = -1;
  for(i1 = 0; i1 < undoTab.rNr; ++i1) {
    if(undoTab.data[i1].grpNr != 0) continue;
    ++ig;
    if(i1 == iRec) break;
  }

    printf("ex UNDO_grpNr_recNr %d ir=%d\n",ig,iRec);

  return ig;

}
*/


//================================================================
  int UNDO_grp_ck (long lNr) {
//================================================================
// test if group already exists in undoTab
// if-yes: delete group. (find first obj of this group ..)

  int   i1, i2, iRec;


  // printf("UNDO_grp_ck %d\n",lNr);
  // UNDO_dump ();
// return 0;  // test

  for(i1=0; i1 < undoTab.rNr; ++i1) {
    if(undoTab.data[i1].lNr != lNr) continue;
    iRec = i1;
    goto L_find_0;
  }
  // lNr not yet exists
  return 0;


  //----------------------------------------------------------------
  // obj exists in undoTab; find its recordIndex with grpNr=0
  L_find_0:
  // delete this record
  MemTab_del (&undoTab, iRec, 1);

  // if this record was group-start-record: set nxt record grpNr=0
  // if(undoTab.data[iRec].grpNr != 0) undoTab.data[iRec].grpNr = 0;
  L_nxt:
  if(undoTab.data[iRec].grpNr == 0) goto L_exit;
  undoTab.data[iRec].grpNr -= 1;
  ++iRec;
  if(iRec < undoTab.rNr) goto L_nxt;




/*
  if(i1 < 0) {printf("*** UNDO_grp_ck E001\n"); return -1;}
  if(undoTab.data[i1].grpNr != 0) { --i1; goto L_find_0;}
    // printf(" rNr=%d i1=%d\n",undoTab.rNr,i1);
  //----------------------------------------------------------------
  // find i2 = nr of records of this group
  for(i2 = i1 + 1; i2 < undoTab.rNr; ++i2) {
    if(undoTab.data[i2].grpNr != 0) continue;
    break;
  }
  i2 -= i1;
    // printf(" rNr=%d i1=%d i2=%d\n",undoTab.rNr,i1,i2);
  //----------------------------------------------------------------
  // delete this group (i2 records; first=i1).
  MemTab_del (&undoTab, i1, i2);
*/

  // printf(" dump after _grp_ck:\n");
  // UNDO_dump ();


  L_exit:
  // printf("-------- ex UNDO_grp_ck %d\n",lNr);

  return 0;

}


//================================================================
  int UNDO_grp_undo () {
//================================================================
/// Ctrl-Z pressed or Undo-Button pressed; delete nxt group


  int   iRec;


  printf("UNDO_grp_undo\n");
  UNDO_dump ("UNDO_grp_undo");


  // undo only in CAD and with empty fields
  if(IE_undo() >= 0) {
    // UNDO_set_bt (0);  // disactivate btUndo btRedo
    return 0;
  }


  //----------------------------------------------------------------
  // undo
  iRec = UNDO_del_active (); 
    // printf(" iRec=%d\n",iRec);
  if(iRec < 0) {
    TX_Print ("***** undo-group is empty ..");
    return 0;
  } 
    
  UI_block__ (1, 1, 1);  // block UI


  if(undoTab.data[iRec].u1 == 'a') {
    UNDO_grp_del (iRec);

//   } else if(undoTab.data[iRec].u1 == 'P') {
//     UNDO_app__ (2);                           // delete application-output

  } else {
    UNDO_grp_res (UNDO_res_active ());
  }


  UI_mcl_update ();                       // update Search/Name

  UI_block__ (0, 0, 0);                   // reset UI

  UNDO_set_bt (2);                        // update buttons

  UNDO_upd_lb ();                         // update label (nr-undo-records)

  return 0;

}


//================================================================
  int UNDO_grp_redo () {
//================================================================
/// CB Redo-Button - restore next group

  int   iRec;


  printf("UNDO_grp_redo\n");
  UNDO_dump ("UNDO_grp_redo");


  // do CAD-undo;
  if(IE_undo() >= 0) {
    // UNDO_set_bt (0);  // disactivate btUndo btRedo
    return 0;
  }


  //----------------------------------------------------------------
  iRec = UNDO_res_active ();
    // printf(" iRec=%d\n",iRec);
  if(iRec < 0) {
    TX_Print ("***** redo-group is empty ..");
    return 0;
  }

  UI_block__ (1, 1, 1);  // block UI


  if(undoTab.data[iRec].u1 == 'a') {
    UNDO_grp_del (iRec);

  } else {
    UNDO_grp_res (iRec);
  }

  UI_mcl_update ();                       // update Search/Name

  UI_block__ (0, 0, 0);                   // reset UI

  UNDO_set_bt (2);                        // update buttons

  return 0;

}


/*
//================================================================
  int UNDO_grp_clr (int iRec) {
//================================================================
// remove the UNDO-record with group-nr iGrp and all following records.
// used for removing 'P'-records
 

  int    ii;


  // get recordNr from groupNr
  // ii = UNDO_recNr_grpNr (iGrp);


  if(iRec < 0) return -1;
    printf("UNDO_grp_clr iRec=%d\n",iRec);

  // MemTab_del (&undoTab, ii, 1);
  undoTab.rNr = ii;

  return 0;

}
*/

//================================================================
  int UNDO_grp_del (int ii) {
//================================================================
// UNDO_grp_del     delete grp <ii> & update display
// change u1 -> 'd'
// Input  ii    index in undoTab

// was UI_undo_do(1)

  int      typ, ll, iNr;
  long     dbi, lNr, lnMin=UT_INT_MAX;
  char     *lPos;


  printf("======================= UNDO_grp_del %d\n",ii);
  UNDO_dump ("UNDO_grp_del");

  if(ii < 0) return -1;

  WC_set_obj_stat (0);          // CAD: perm.

  iNr = 0;



  //----------------------------------------------------------------
  // delete obj from undoTab[ii]
  L_nxt:
  ++iNr;

  lNr = undoTab.data[ii].lNr;
    // printf(" lNr=%ld\n",lNr);


  undoTab.data[ii].u1 = 'd';     // set group(Member)=deleted

  if(undoTab.data[ii].u2 == '1') {
    UNDO_ln_del (lNr);  // delete line
  }

  // test if this is a change-record
  if(undoTab.data[ii].u2 == '2') {
    // ii is a change-record; delete line (lNr+1).
    UNDO_ln_del (lNr-1, ii);
    UNDO_ln_res (lNr);
    lnMin = lNr;
    goto L_update;
  }


  // test if there are more groupMembers
  ++ii;
  lnMin = IMIN (lNr, lnMin);     // get lowest lNr
  
  if(ii >= undoTab.rNr) goto L_update;
  if(undoTab.data[ii].grpNr == 0) goto L_update;
  goto L_nxt;



  //----------------------------------------------------------------
  L_update:
    // printf(" iNr=%d\n",iNr);
  TX_Print ("- remove %d objects ..",iNr);

  APED_update__ (lnMin);         // update display starting at line nr <lnMin>

  return 0;

}


//================================================================
  int UNDO_grp_res (int ii) {
//================================================================
// UNDO_grp_res    restore grp <ii>
// Input  ii    index in undoTab

// was UI_undo_do(0) APED_undo_do


  int      typ, ll, iNr;
  long     dbi, lNr, lnMin=UT_INT_MAX;
  char     *lPos;


  printf("==================== UNDO_grp_res %d\n",ii);
  UNDO_dump ("UNDO_grp_res");


  // skip this in Applications
  // printf(" APP_act=%d\n",AP_stat.APP_act);
  if(AP_stat.APP_stat > 0) {
    TX_Print(" ****** undo / redo not supported in UserApplication *****");
    return 0;
  }


  WC_set_obj_stat (0);          // CAD: perm.

  if(ii < 0) return -1;

  iNr = 0;
    // printf(" ii=%d\n",ii);



  //----------------------------------------------------------------
  // restore obj from undoTab[ii]
  L_nxt:
  ++iNr;

  lNr = undoTab.data[ii].lNr;
    // printf(" lNr=%d\n",lNr);



  undoTab.data[ii].u1 = 'a';     // set group=restored

  if(undoTab.data[ii].u2 == '1') {
    UNDO_ln_res (lNr);  // restore line
  }


  // test if this is a change-record
  if(undoTab.data[ii].u2 == '2') {
    // ii is a change-record; delete line (lNr+1).
    UNDO_ln_res (lNr - 1);  // restore line
    UNDO_ln_del (lNr);
    lnMin = lNr;
    goto L_update;
  }


  // test if there are more groupMembers
  ++ii;
  lnMin = IMIN (lNr, lnMin);     // get lowest lNr

  if(ii >= undoTab.rNr) goto L_update;
  if(undoTab.data[ii].grpNr == 0) goto L_update;
  goto L_nxt;



  //----------------------------------------------------------------
  L_update:
    // printf(" iNr=%d\n",iNr);
  TX_Print ("- restore %d objects ..",iNr);


  APED_update__ (lnMin);           // update display

  return 0;

}


//================================================================
  int UNDO_ln_res (long lNr) {
//================================================================
// delete line (lNr+1).


  int      typ;
  long     dbi, ll;
  char     *lPos;


  // printf("UNDO_ln_res %ld\n",lNr);


  lPos = UTF_GetLinNr (NULL, &ll, lNr);
  if(!lPos) {printf("*** UNDO_ln_res E001\n"); return -1;}
    // printf("ln=|");UTX_dump_cnl(lPos,40);printf("|\n");


  // remove first char; add ' ' as last char
  UTF_comment_line (lPos, 0, "_");

  // // TX_Print("Line %d (%12.12s) restored",lNr,lPos);
  // i1 = sprintf (mem_cbuf1, "Line %d (", lNr);
  // i2 = UTX_cp_Line(&mem_cbuf1[i1], &cpn, lPos, 16) + i1;
  // strcat(&mem_cbuf1[i2], ") restored");
  // TX_Print(mem_cbuf1);


  APED_dbo_oid (&typ, &dbi, lPos);
  if(typ == Typ_Error) {printf("*** UNDO_ln_res E002\n"); return -1;}
    // printf(" typ=%d dbi=%d\n",typ,dbi);

  Brw_obj_upd (typ, dbi, 0);     // update Brw-window


  return 0;

}


//================================================================
  int UNDO_ln_del (long lNr) {
//================================================================
// UNDO_ln_del             delete sourceLine lNr


  int      typ;
  long     dbi, ll;
  char     *lPos;


  printf("UNDO_ln_del %ld\n",lNr);
  // UTF_dump__ ();


  // get lPos = sourceline from sourcelineNr lNr
  lPos = UTF_GetLinNr (NULL, &ll, lNr);
  if(!lPos) {printf("*** UNDO_ln_del E001\n"); return -1;}
    // printf("ln=|");UTX_dump_cnl(lPos,40);printf("|\n");


  //----------------------------------------------------------------
  // get parents, redisplay unvis. parents, reset is-parent-bit of parents
  {
    int            i1;
    long           dli;
    char           *osrc;
    MemTab(ObjSRC) mtPar = _MEMTAB_NUL;
    // ObjAto         ato1;

    // check if Line cBuf is a Definitionline; get osrc = pos. of code
    i1 = APED_ck_defLn (&osrc, lPos);
    if(i1) {TX_Error("UNDO_ln_del E1"); return -1;}
    // get max nr of atomic-objs
    i1 = SRCU_tsMax (osrc);  // printf(" siz-mtPar = %d\n",i1);
      // printf(" _ln_del2|");UTX_dump_cnl(lPos,100);printf("|\n");
      // printf(" _ln_del2 i1=%d\n",i1); // exit(-1);
    // get temp-spc for atomic-objs 
    // ATO_getSpc_tmp__ (&ato1, i1);
    // if(!ATO_getSpc_tmp_ck(&ato1)) {TX_Error("UNDO_ln_del E2"); return -1;}
    // get temp-spc for parents-table
    MemTab_ini_temp (&mtPar, Typ_ObjSRC, i1);
    if(MEMTAB_RMAX(&mtPar) != i1) {TX_Error("UNDO_ln_del EOM"); return -1;}
    // get parents-table and atomic-objs
    // APT_ato_par_srcLn (&mtPar, &ato1, osrc);
      // ATO_dump__ (&ato1, "_ln_del");
    APT_ato_par_srcLn (&mtPar, NULL, osrc);
      // MemTab_dump (&mtPar, "_ln_del");
    // loop tru parents
    for(i1=0; i1<mtPar.rNr; ++i1) {
      dli = (MEMTAB__(&mtPar,i1))->dli;
      if(dli >= 0L) {
        // redisplay parent if unvis.
        DL_unvis__ (dli, 0);
        // reset is-parent-bit of parent
        DL_parent_set (dli, 0);
      }
    }
    MemTab_free (&mtPar);
  }




  //----------------------------------------------------------------
  // get dbo
  APED_dbo_oid (&typ, &dbi, lPos);
  if(typ == Typ_Error) {printf("UNDO_ln_del E4\n"); return -1;}
    // printf(" typ=%d dbi=%d\n",typ,dbi);


  // delete line
  UTF_comment_line (lPos, 1, "_");

  // i1 = sprintf (mem_cbuf1, "Line %d (", lNr);
  // i2 = UTX_cp_Line(&mem_cbuf1[i1], &cpn, lPos+1, 16) + i1;
  // strcat(&mem_cbuf1[i2], ") deleted; Ctrl-Y to restore");
  // TX_Print(mem_cbuf1);


  // DB_Delete (typ, dbi);          // delete obj in DB
  // problematisch; DB-Pos wird später wiederverwendet;
  // Obj-restore: nun ist DB-Pos doppelt verwendet ..


  Brw_obj_upd (typ, dbi, 1);     // update Brw-window

  // printf(" ex UNDO_ln_del:\n");
  // UNDO_dump ();

  return 0;

}


//================================================================
  int UNDO_chg_add (long lNr, int grpNr) {
//================================================================
// UNDO_chg_add     save line-modified-record  (u2="2")
// lnr is a line already deleted;
// lnr+1 is a line newly added, having the same type+dbi.
// save lNr+1 !

  long      ld;
  undoObj   o1;


  // printf("UNDO_chg_add %d %d\n",lNr,grpNr);


  // // delete all following groups
  // undoTab.rNr = UNDO_grp_upd ();


  // increment all linenumbers > lNr 
  // ( a new line has been inserted !
  UNDO_chg_ins (lNr+1);
  
  
  // test if group already exists in undoTab
  UNDO_grp_ck (lNr+1);
  
  
  // add obj to undoTab
  o1.lNr   = lNr + 1;
  o1.grpNr = grpNr;
  o1.u1    = 'd';
  o1.u2    = '2';
  MemTab_sav (&undoTab, &ld, &o1, 1);

  // fix buttons
  UNDO_set_bt (2);

  // fix label
  UNDO_upd_lb ();


  // UNDO_dump ();


  return 0;

}


//================================================================
  int UNDO_chg_ins (long newLnr) {
//================================================================
// increment all linenumbers > lNr 
// ( a new line has been inserted !

  int    i1;


  // printf("UNDO_grp_del %d\n",newLnr);

  for(i1=0; i1 < undoTab.rNr; ++i1) {
    if(undoTab.data[i1].lNr < newLnr) continue;
    undoTab.data[i1].lNr += 1;
  }

  return 0;

}  
  

//================================================================
  int UNDO_app__ (int mode) {
//================================================================
/// \code
/// UNDO_app__         handle undo for plugins (no redo)
/// Input
///   mode    0 store all infos before start of appli                   
///           1 end of appli: create undo-record; activate undo-button 
///           2 delete output of appli, remove undo-record            
///           3 remove undo-record
///  Output:  mode=1  returns lineNr at start of appli
///    retCod  
/// \endcode



static int    lNr0;
static long   dli, dbl;
  int         irc = 0, lNr1;
  long        ld;
  undoObj     o1;


  printf("UNDO_app__ %d\n",mode);
    // UNDO_dump ("UNDO_app__");


  //================================================================
  //        0 store all infos before start off appli
  if(mode != 0) goto L_app_1;

  // stor line-nr
  lNr0 = ED_get_lnr_act ();
    printf(" _app__-lNr0=%d\n",lNr0);
  goto L_app_ex;


  //================================================================
  //        1 end of appli: create undo-record; activate undo-button
  L_app_1:
  if(mode != 1) goto L_app_2;

  // check if output was created
  lNr1 = ED_get_lnr_act ();
    printf(" _app__-lNr1 = %d\n",lNr1);
  if(lNr1 <= lNr0) return -1;

  // create undo-record
  o1.lNr   = lNr0;
  o1.grpNr = 0;
  o1.u1    = 'P';
  o1.u2    = '1';
  MemTab_sav (&undoTab, &ld, &o1, 1);

  // activate undo-button
  // UNDO_upd__ ();
  UNDO_set_bt (2);
  UNDO_upd_lb ();

  irc = lNr0;
  goto L_app_ex;


  //================================================================
  //        2 delete output of appli, remove undo-record
  L_app_2:
  if(mode != 2) goto L_app_3;

  // delete modelcode
  irc = ED_del__ (lNr0);    // del all following
  if(irc < 0) goto L_E001;
  ED_work_CurSet (lNr0);    // reset to lNr (kill all following)
  APED_update__ (-1L);      // work new codes; updated lNr

  goto L_app_3;  // delete record


  //================================================================
  //        3 remove undo-record
  L_app_3:
//   // delete undo-record
//   // if(undoTab.rNr > 0) undoTab.rNr -= 1;
// // TODO: find last 'P' group, delete group
// 
//   goto L_app_ex;

  //================================================================
  L_app_ex:

    printf("ex-UNDO_app__ %d\n",irc);
    UNDO_dump ("ex-UNDO_app__");

  return irc;

  L_E001:
    printf("UNDO_app__ E001\n");
    UNDO_dump ("ex-UNDO_app__-E001");
    return -1;
}


//================================================================
  int UNDO_clear () {
//================================================================

  // printf("UNDO_clear \n");

  MEMTAB_CLEAR (&undoTab);

  UNDO_set_bt (2);         // fix buttons
  UNDO_upd_lb ();          // fix label

  return 0;

}


/*
//================================================================
  int UNDO_grp_upd () {
//================================================================
// UNDO_grp_upd       delete all following groups after actGrp
// returns first free recordnumber after group <actGrp>

  int    i1, ig;

  // printf("UNDO_grp_upd %d\n",actGrp);


  for(i1 = actGrp+1; i1 < undoTab.rNr; ++i1) {
    if(undoTab.data[i1].grpNr != 0) continue;
    break;
  }
    // printf("ex UNDO_grp_upd %d\n",i1);
  return i1;

}
*/

//================================================================
  int UNDO_upd__ () {
//================================================================
// fix buttons & label
 
  // printf("UNDO_upd__ \n");

    // fix buttons
    UNDO_set_bt (2);

    // fix label
    UNDO_upd_lb ();

  return 0;

}
  

//================================================================
  int UNDO_res_active () {
//================================================================
// check if one of the next groups can be restored
// first to restore = oldest = first in undoTab
// RetCod:  -1 no group to restore
//         >=0 index of next group to restore
// if(u1 == 'd') then group can be restored

  int  i1, iRes;


  if(undoTab.rNr < 1) goto L_no;

  // for(i1 = (undoTab.rNr - 1); i1 >= 0; --i1) {
  for(i1 = 0; i1 < undoTab.rNr; ++i1) {
    if(undoTab.data[i1].grpNr != 0) continue;
    if(undoTab.data[i1].u1 == 'd') return i1;
  }

  L_no:
  return -1;

}

 
//================================================================
  int UNDO_del_active () {
//================================================================
// check if one of the previous groups can be deleted
// first to delete = youngest = last in undoTab
// RetCod:  -1 no group to restore
//         >=0 index of next group to delete
// if(u1 == 'a') then group can be deleted

  int  i1, iRes;


  if(undoTab.rNr < 1) goto L_no;

  // for(i1 = 0; i1 < undoTab.rNr; ++i1) {
  for(i1 = (undoTab.rNr - 1); i1 >= 0; --i1) {
    if(undoTab.data[i1].grpNr != 0) continue;
    if(undoTab.data[i1].u1 == 'a') return i1;
  }

  L_no:
  return -1;

}


//====================================================================
  int UNDO_set_bt (int mode) {
//====================================================================
// UNDO_set_bt             activate/disactivate  delete/restore-buttons
// Input:
//   mode    0=lock; 1=unlock; 2=update


static int uAct=0, rAct=0;           // 1=TRUE(active), 0=FALSE(locked)
  int      uNew, rNew;
  int      uNr, rNr;


  // printf("UNDO_set_bt rNr=%d uAct=%d rAct=%d\n",undoTab.rNr,uAct,rAct);
  // UNDO_dump ("s-_set_bt");

  
  //----------------------------------------------------------------
  if(mode != 2) goto L_unlock;
  // update
  // test if group-to-undo exists; returns (nr-of-records - 1)
  uNr = UNDO_del_active ();
  // test if group-to-restore exists; returns (nr-of-records - 1)
  rNr = UNDO_res_active ();
    // printf(" _set_bt uNr=%d rNr=%d\n",uNr,rNr);

//TEST ONLY:
// uNr=1; rNr=1;

  if(uNr < 0) uNew = FALSE;
  else        uNew = TRUE;

  if(rNr < 0) rNew = FALSE;
  else        rNew = TRUE;
    // printf(" _set_bt uNew=%d rNew=%d\n",uNew,rNew);
  goto L_update;


  //----------------------------------------------------------------
  L_unlock:
  if(mode != 1) goto L_lock;
  uNew = TRUE;
  rNew = TRUE;
  goto L_update;


  //----------------------------------------------------------------
  L_lock:
  // if(mode != 0) goto ..
  uNew = FALSE;
  rNew = FALSE;


  //----------------------------------------------------------------
  L_update:
  if(uNew != uAct) {
    uAct = uNew;
    GUI_set_enable (&btUndo, uAct); //1=active, 0=inactive    RED
  }

  if(rNew != rAct) {
    rAct = rNew;
    GUI_set_enable (&btRedo, rAct); //1=active, 0=inactive    GREEN
  }

    // printf("ex-UNDO_set_bt \n");

  return 0;

}

/* UNUSED
//====================================================================
  int UNDO_upd_bt () {
//====================================================================
/// UNDO_upd_bt             activate/disactivate  delete/restore-buttons

  // int    iRec;
  int    iUndo, iRedo;    // 1=TRUE, 0=FALSE


  // printf("UNDO_upd_bt %d\n",undoTab.rNr);
  // UNDO_dump ("s-_upd_bt");


  iUndo = FALSE;
  iRedo = FALSE;

  if(undoTab.rNr < 1) goto L_disp;



  // check if one of the previous groups can be deleted
  iUndo = UNDO_del_active ();  // 1=delete-icon-active; 0=not.
    // printf(" iUndo=%d\n",iUndo);
  iUndo = ILIM01(++iUndo);

  // check if one of the next groups can be restored
  iRedo = UNDO_res_active ();  // 1=restore-icon-active; 0=not.
    // printf(" iRedo=%d\n",iRedo);
  iRedo = ILIM01(++iRedo);



  L_disp:
    // printf(" iUndo=%d iRedo=%d\n",iUndo,iRedo);
  GUI_set_enable (&btUndo, iUndo); //1=active, 0=inactive
  GUI_set_enable (&btRedo, iRedo);


  return 0;

}
*/

//====================================================================
  int UNDO_upd_lb () {
//====================================================================
/// display Label

  int  i1;
  char cBuf[32];


  i1 = undoTab.rNr;

  // sprintf(cBuf, "Undo   %d",undo_lNr);
  sprintf(cBuf, "  %d",i1);
               //012345


  // if(i1 > 0) {
    // if(undo_lact == UNDO_RESTORE) cBuf[5] = 'R';
    // else cBuf[5] = 'D';
  // }

  GUI_label_mod (&wUndo, cBuf);

  return 0;

}


//================================================================
  int UNDO_dump (char *info) {
//================================================================
/// dump delTab

  int          i1, iNr;
  undoObj      *oTab, *o1;


  MemTab_load ((void**)&oTab, &iNr, (MemTab*)&undoTab);

  printf("--------- UNDO_dump rNr=%d ---- %s\n",iNr,info);

  for(i1=0; i1 < iNr; ++i1) {
    o1 = &oTab[i1];
    printf("undoTab[%d] lNr=%ld grpNr=%d u1=%c u2=%c\n",i1,
        o1->lNr,o1->grpNr,o1->u1,o1->u2);
  }


  return 0;

}


// EOF
