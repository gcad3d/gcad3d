//  ../xa/xa_edi__.c                        Reiter 2012-07-30
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
2012-07-30 ex ../xa/xa_ed.c            RF

-----------------------------------------------------
*/
#ifdef globTag
void EDI(){}
#endif
/*!
\file  ../xa/xa_edi__.c
\brief  Neutral EditorFunctions (not Gtk-specific).
\code
=====================================================
List_functions_start:

ED_GR_CB1__         callback from grafic-window (selection)
ED_GR_CB2__         callback from grafic-window (mouse-key press)
ED_Esc_CB__         callback Esc-key
ED_Del_CB__         callback Delete-key
EDI_CB__            editor-callback of GUI_edi__ (mouse/key into editor)

ED_load__           mem -> editor
ED_unload__         editor -> memory
ED_save_file        save editor -> file
EDI_set_lNr__       set to lineNr
EDI_set_oid_ui      set to oid of definitionLine from GUI
EDI_set_lNr_UI      get lineNr from user and set to lineNr
EDI_set_src_UI      get sourceText to find from user and set to this line

EDI_sel_lnr         select Line, set Curpos to Line
ED_goto__           goto curPos in editor

ED_sysed__          use system-editor for active line

List_functions_end:
=====================================================
See also APED_ (APED_dec_defLn ..)

// EDI_set_lnr         set ED_lnr_act, goto Line, display lNr
// EDI_goto_lnr        goto Line
\endcode *//*----------------------------------------


 Neutral EditorFunctions (not Gtk-specific).
../xa/xa_edi__.c            this source
../xa/xa_ed.c
../xa/xa_cad_ed.c
../xa/xa_ed_mem.c
AP_SRC_*
AP_src*
AP_lNr_*
AP_APT_*


*/




#ifdef _MSC_VER
#include "MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>                    // isdigit


#include "../ut/ut_geo.h"
// #include "../ut/ut_umem.h"                // Memspc MemObj UME_*
#include "../ut/ut_cast.h"                // INT_PTR
#include "../ut/ut_txfil.h"               // UTF_*
#include "../ut/ut_os.h"          // OS_get_tmp_dir

#include "../gui/gui__.h"         // GUI_ UI_Func..

#include "../xa/xa_ui.h"          //  UI_men__
#include "../xa/xa_uid.h"         //  UI_MODE_MAN
#include "../ut/ut_memTab.h"           // MemTab
#include "../xa/xa_mem.h"         //  mem_cbuf1
#include "../xa/xa_app.h"         // PRC_IS_ACTIVE
#include "../xa/xa_msg.h"               // MSG_ERR__
#include "../xa/xa.h"             // APP_act_nam

 





//===========================================================================
// Externals:

// aus xa_ui.c:
extern MemObj    winED;
extern MemObj    winGR;
extern int       UI_InpMode;
extern int       UI_EditMode;
extern int       xa_fl_TxMem;
extern char      UI_stat_view, UI_stat_hide;

// aus ../ut/ut_txfil.c:
extern char       *UTF_FilBuf0;
extern long       UTF_FilBuf0Siz;
extern long       UTF_FilBuf0Len;  // die aktuelle Filesize

// from ../xa/xa_ed.c:
extern int       ED_lnr_act;
extern int       ED_lnr_max;        // total nr of lines

// from ../xa/xa.c
// extern long       AP_ED_lNr;
extern int        AP_src;                // AP_SRC_MEM od AP_SRC_EDI
//int       AP_ED_cPos;
//char      AP_ED_oNam;

// from ../ci/NC_Main.c
extern int       APT_dispPL;
extern int     APT_obj_stat;
// replace ED_lnr_act mit AP_ED_lNr


// from ../xa/xa_ed.c
extern int       APT_mac_fil; // 1=InputFromMemory; 0=InputFromFile.


//===========================================================================
// Locals:



//================================================================
  int ED_sysed__ (int mode) {
//================================================================
/// \code
/// ED_sysed__          use system-editor for active line
/// mode    1   edit single line (lnNr <ED_lnr_act>)
/// \endcode

// see also AP_APT_sysed AP_src_mod_ed

  int    s1Siz;
  char   *s1, fnam[256];
  long   lNr;


  printf("ED_sysed__ %d\n",mode);

  s1 = mem_cbuf1;
  s1Siz = mem_cbuf1_SIZ;
  lNr = ED_lnr_act;
  sprintf(fnam, "%sselection.txt", OS_get_tmp_dir());

  // get line into buffer
  s1Siz = GUI_edi_RdLn (s1, s1Siz, lNr, &winED);
  if(s1Siz < 1) goto L_err1;


  // Zeile in ein File rausschreiben    see also UTF_wrf_line
  UTX_wrf_str (fnam, s1);
  TX_Print("Edit line %d spawned ..",lNr);


  // modify File m. Editor
  APP_edit (fnam, 0);
  TX_Print("Edit done ..");


  // load modified textline
  s1Siz = OS_FilSiz (fnam);
  if(s1Siz > mem_cbuf1_SIZ) goto L_err1;
  UTX_str_file (s1, &s1Siz, fnam);


  // modify line in gcad-editor
  GUI_edi_mod_ln (&winED, lNr, s1);


  return 0;

  L_err1:
    TX_Print(".. line empty ..");
    return -1;

}

 
//================================================================
  int EDI_sel_lnr (long lNr) {
//================================================================
// EDI_sel_lnr         select Line, set Curpos to Line

  int   irc;

  irc = GUI_edi_sel_ln (&winED, lNr);
    printf(" sel_lnr-irc=%d\n",irc);
  if(irc < 0) return -1;

  // already in line <lNr>
  ED_lnr_act = lNr;

  // disp LineNr
  UI_AP (UI_FuncSet, UID_ouf_lNr, PTR_INT((long)ED_lnr_act));

  return 0;
}


/* replaced with EDI_set_lNr__
//================================================================
  int EDI_set_lnr (long lNr) {
//================================================================
/// EDI_set_lnr         set ED_lnr_act, goto Line, display lNr

// was GUI_Ed_setCnxtL

  // printf("EDI_set_lnr %ld\n",lNr);

  // goto <lNr> in editor; display <lNr>
  EDI_goto_lnr (lNr);

  // set ED_lnr_act = lNr
  ED_set_lnr_act (lNr);

    // printf("ex-EDI_set_lnr %ld\n",lNr);

  return 0;

}
*/
/* replaced with EDI_set_lNr__
//================================================================
  int EDI_goto_lnr (long lNr) {
//================================================================
/// EDI_goto_lnr        goto Line, display lNr
// DO NOT USE - use EDI_set_lNr__


  long   i1;

  // printf("EDI_goto_lnr %ld\n",lNr);

  GUI_edi_setLnr (&winED, lNr);

  i1 = lNr + 1;
  UI_AP (UI_FuncSet, UID_ouf_lNr, (void*)i1);
  
    // printf("ex-EDI_goto_lnr %ld\n",lNr);

  return 0;

}
*/

//================================================================
  int ED_Del_CB__ (int ctrlOn) {
//================================================================
// ED_Del_CB__          callback Delete-key
// Input:    ctrlOn    state control-key; 0=OFF, else on

  int     i1;
  char    s1[256];

  printf("ED_Del_CB__ %d\n",ctrlOn);
  printf("  ED_lnr_act=%d\n",ED_lnr_act);


    // fix filename for selected text
    sprintf(s1,"%sselection.txt",OS_get_tmp_dir());
      printf(" ED_Del_-fnam:|%s|\n",s1);

    if(ctrlOn == 0) {
      // write selected text -> file
      i1 = GUI_edi_sel_wrf (&winED, s1);
      if(i1 < 1) goto L_del1;
      GUI_edi_sel_del (&winED);         // delete selected text in Editor
      TX_Print("**** selected Text deleted; undelete: Ctrl-delete.");

    }  else {
      GUI_edi_InsFile (&winED, s1);
    }


/*
    // printf("UI_EdKeyPress GDK_Delete\n");
    xa_fl_TxMem = 1;   // src modified merken

    // fix filename for selected text
    sprintf(cbuf,"%sselection.txt",OS_get_tmp_dir());
      // printf(" fnam:|%s|\n",cbuf);

    if(KeyStatCtrl == ON) {
      GUI_edi_InsFile (&winED, cbuf);

    }  else {
      // write selected text -> file
      i1 = GUI_edi_sel_wrf (&winED, cbuf);
      if(i1 < 1) goto L_del1;
      GUI_edi_sel_del (&winED);         // delete selected text in Editor
      TX_Print("Text deleted; undelete: Ctrl-delete.");
    }

    // l1 = GUI_Ed_getCpos (&winED);  // get cursorposi
    // l2 = UTF_GetLnrPos (l1);          // get APT-LineNr from cursorposi
    l2 = GUI_edi_getLnr (&winED);
    // printf(" l1=%d l2=%d\n",l1,l2);
    ED_work_CurSet (l2);              // work bis l2

    // Edi -> Memory (nach dem Loeschen)
    ED_unload__ ();

    // goto Changed;
      // printf(" dddddddddddddelete done\n");
    goto AllDone;
*/


    L_del1:
      // goto Changed;  // Standardbehandlung
      return 0;

}


//================================================================
  int ED_Esc_CB__ (int ctrlOn) {
//================================================================
// ED_Esc_CB__          callback Esc-key
// Input:    ctrlOn    state control-key; 0=OFF, else on

static char sDel[80];
static long cPos;

  int    ii0, ii1, ii2;
  long   iLen, posL, posC, pos0;
  char   *s1, *s2, *p1;


  // printf("ED_Esc_CB__ %d\n",ctrlOn);
  // printf("  ED_lnr_act=%d\n",ED_lnr_act);

    
  // get curPos
  posC = GUI_edi_getCpos (&winED);

  // - test if shift-key down   -                                                  
  if(ctrlOn) goto L_restore;


  //----------------------------------------------------------------
  // select word left of cursor
  // get active line 
  iLen = GUI_edi_RdLn (mem_cbuf1, mem_cbuf1_SIZ, ED_lnr_act, &winED);
  if(iLen < 1) goto L_err1;
    // printf(" Esc_CB__-1|%s|\n",mem_cbuf1);

  // get ii1 = startpos of definition-header
  p1 = mem_cbuf1;
  APED_defDbo_skip (&p1);
  ii0 = p1 - mem_cbuf1;

  // get posL = startPos of ED_lnr_act
  posL = GUI_edi_getLsta (&winED, ED_lnr_act);

  // empty line ?
  if(posC == posL) return 0;

  // get ii2 = pos of cursor in line
  ii2 = posC - posL;
    // printf(" Esc_CB__ posL=%ld posC=%ld\n",posL,posC);

  // get ii1 = pos of begin of word left of cursor
  ii1 = UTX_pos_del_prev (mem_cbuf1, ii2);
    // printf(" Esc_CB__ ii0=%d ii1=%d ii2=%d\n",ii0,ii1,ii2);
    // printf(" Esc_CB__ s[ii1] |%s|\n",&mem_cbuf1[ii1]);
    // printf(" Esc_CB__ s[ii2] |%s|\n",&mem_cbuf1[ii2]);

  // test of only definition-header ("p1=")
  if(ii2 > ii0) {
    if(ii0 > ii1) {
      ii1 = ii0;
    }
  }

  // select from ii1 to ii2
  pos0 = posL + ii1;
    // printf(" pos0=%ld\n",pos0);

  // select_region from ii1 to ii2
  GUI_edi_sel__ (&winED, pos0, posC);

  // copy selected region into static sDel
  iLen = ii2 - ii1;
  strncpy(sDel, &mem_cbuf1[ii1], iLen);
  sDel[iLen] = '\0';
    // printf(" sDel |%s|\n",sDel);

  // // delete selected region
  // GUI_edi_sel_del (&winED);
  // TX_Print("**** restore delete with Ctrl-Esc");

  // store new curPos
  cPos = GUI_edi_getCpos (&winED);

  return 0;


  //----------------------------------------------------------------
  L_restore:
      // printf(" Esc_CB__-res %ld %ld\n",posC,cPos);


  // test if curPos is identical
  if(posC != cPos) {
    TX_Print("**** ERROR restore delete  text: cursor position changed ..");
    return -1;
  }

  // insert stored word at current position
  iLen = strlen(sDel);
  // GUI_edi_Insert (&winED, cPos, sDel, iLen);


/*
        DL_hili_off (-1L);
        // war zuletzt ein "create neue zeile" oder ein "delete Text"
        // i1 = UI_undo_get_ustat();
        // if(i1 == 1) {
          // UI_undo_work (0, 0);  // undo last entry .. changed to Ctrl-Z

        // } else if(i1 == 2) {  // last was "delete Text"; undelete it.
          // sprintf(cbuf,"%stmp%cselection.txt",OS_get_bas_dir(),fnam_del);
          // GUI_edi_InsFile (&winED, cbuf);
        // }
        xa_fl_TxMem = 1;         // src modified merken
        // AP_obj_del1 ();
        // AP_obj_del0 (); // geht no ned
        ED_work_exit (); // exit modify, work from curPos to end of model
*/


  return 0;

  L_err1:
    return -1;
}


//================================================================
  int ED_GR_CB2__ (int typ, long dbi, char *buf) {
//================================================================
// ED_GR_CB2__          callback from grafic-window (selection)
//   comes after ED_GR_CB1__


  int    irc, i1, i2;
  char   s1[256];


  // printf("ED_GR_CB2__ typ=%d dbi=%ld buf=|%s|\n",typ,dbi,buf);
  // printf(" APT_obj_stat=%d\n",APT_obj_stat);  // 1=temp, 0=perm


  // cStat = ED_ck_lnStart();
    // printf(" cStat=%d\n",cStat);

  GUI_edi_ck_cPos_ln (&winED, &i1, &i2);  // i1=pos-in-line,  i2=pos-in-file

  // if cursor is at start of line: do nothing; return -1
  if((i1 == 0)||(i1 == 3)) return -1;


  // test if curs is on end of file
  if(i2 == 2)  {
    // at eof; test if on end of line
    if(i1 == 2) goto L_addSel;        // at eol: add selection ..
  }

  // query insert selection
  sprintf(s1, " add / insert selected obj ? ");
  irc = GUI_Dialog_2b (s1, MSG_const__(MSG_ok), MSG_const__(MSG_no));
    // printf(" GR_CB2__-irc=%d\n",irc);

  if(irc) return 0;  // 0 means first button pressed


  L_addSel:
  ED_add_Text (typ, dbi, buf);  // add buf to editor-text
  GR_Disp_src (buf);            // preview buf
  DL_Redraw ();

/*
    // test if cursor is inside line or at startposition in line
    if(ED_ck_lnStart() == 0) {
      // yes, cursor is at first position in line
      goto L_addGrp;          // add obj to group


    } else {       // cursor is inside line
      
      if(typ == Typ_TmpPT) UI_disp_Pos (Typ_PT, &GR_CurUk);
    
      // add nur wenn akt. Line zB "L25=" ist ...
      // i1 = ED_query_CmdMode ();
      // if(i1 == 0) {  // 0=Defline
        ED_add_Text (typ, dbi, buf);
        // update Window ..
        // ED_update (0L);   // 2013-04-11 weg
  
        // es koennte ein Insert sein; LineNr ?
      i1 = ED_get_lnr_act ();  // sets also ED_lnr_act
      // disp LineNr
      UI_AP (UI_FuncSet, UID_ouf_lNr, PTR_INT(i1));
    }
*/

  return 0;
}


//================================================================
  int ED_GR_CB1__ (int GR_Event_Act) {
//================================================================
// ED_GR_CB1__          callback from grafic-window (mouse-key press)
//   comes before ED_GR_CB2__
// Input:
//   GR_Event_Act     GUI_MouseL | GUI_MouseR


  int    i1, i2;

  // printf("ED_GR_CB1__ %d\n",GR_Event_Act);

        // i1 = ED_query_CmdMode (); // analyze active line; -1=empty, 0=DefLn..
          // printf(" MAN; ev=%d i1=%d\n",GR_Event_Act,i1);
        GUI_edi_ck_cPos_ln (&winED, &i1, &i2);

        if(GR_Event_Act == GUI_MouseL) {
          // M1 in MAN
          if((UI_stat_hide)&&(UI_stat_view)) {
            // hide,view not active;
            if(i1 == 3) {
              // empty line, create def-menu (P L C ..)
              UI_GR_selMen_init (0);                  // MAN,M1,empty
              goto Fertig;
            }
          }

/*
        } else if(GR_Event_Act == GUI_MouseR) {
          // M3 in MAN
          // test if active line = definitionLine; if yes: activate menu
          // if process is active: do process-subMenu
          if(PRC_IS_ACTIVE) {
            sprintf(s1, "MBR_%d", i1);
              // printf(" subMen for process |%s|\n",s1);
            PRC__ (-1, s1);  // report M3 to process
            goto Fertig;
          }
          // M3: wait for selection of objects; goes -> OMN_selMen_MAN_M3_empty
*/
        }

  Fertig:

  return 0;
}


//================================================================
  int EDI_CB__ (MemObj *mo, void **data) {
//================================================================
/// EDI_CB__            editor-callback of GUI_edi__ (mouse/key into editor)
///   problem: reports only keyRelease, not keyPress.
// Input:
//   GUI_DATA_EVENT     TYP_EventEnter|TYP_EventPress|TYP_EventRelease
//   GUI_DATA_L1        Press|Release: key (ascii or eg GUI_KeyReturn)
//   GUI_DATA_L2        Press|Release: modifier-keys; see GUI_Modif_shift| ..
//   GUI_DATA_L2        Enter: line-nr


  int     i1, i2, ctrlOn, shiftOn, altOn;
  long    l1;
  char    *p1;



  // printf("EDI_CB__-------------------------------------------- \n");
  // printf("EDI_CB__ ev=%d\n",GUI_DATA_EVENT);
  // printf("  ED_lnr_act=%d\n",ED_lnr_act);
  // printf("  modifKeys=%ld\n",GUI_DATA_L2);
  // if(GUI_DATA_EVENT == TYP_EventPress)   printf("press-key = %lx\n",GUI_DATA_L1);
  // if(GUI_DATA_EVENT == TYP_EventRelease) printf("relea-key = %lx\n",GUI_DATA_L1);




  //----------------------------------------------------------------
  // enter line in editor - eg select line with mouse
  if(GUI_DATA_EVENT == TYP_EventEnter) {    // 400
      // printf(" EDI_CB__-Enter-shiftOn=%d ctrlOn=%d\n",shiftOn,ctrlOn);

    // get test cursorPosition in active line
    i1 = GUI_edi_ck_cPos_ln (mo, &i1, &i2);

    // // ignore shift (block-selection active)
    // if(shiftOn) goto L_ignore;

    // caused by mouseclick into editor; L1=curPos, L2=lineNr
    // but also by select-block-process !
    l1 = GUI_DATA_L2;  // line-nr
      // printf(" enter line %ld ED_lnr_act=%d\n",l1,ED_lnr_act);
    if(l1 == ED_lnr_act) goto L_ignore;  // skip identical line

    // ED_newPos ED_update
    ED_update (0L);  // editor -> memory if Filesize has changed ..
    // von der zuletzt bearbeiteten Zeile bis zu lNr anzeigen/abarbeiten
    ED_work_CurSet (l1);
    // GUI_edi_Focus (&winED);

    return 0;

    

/*
  //----------------------------------------------------------------
  } else if (GUI_DATA_EVENT == TYP_EventPress) {   // 402
    // key-press
      printf(" EDI_CB__-press-key= %x mod=%d\n",GUI_DATA_I1,GUI_DATA_I2);

    i2 = GUI_DATA_I2;  // modifier-keys
    shiftOn = i2&1;
    ctrlOn  = i2&4;
    altOn   = i2&4;
      printf(" EDI_CB__-press-shiftOn=%d ctrlOn=%d altOn=%d\n",
              shiftOn,ctrlOn,altOn);

    i1 = GUI_DATA_I1;  // key
    // if(i1 == GUI_KeyShift_L) {shiftOn = 1; goto L_ignore;}
    // if(i1 == GUI_KeyControl_L) {ctrlOn = 1; goto L_ignore;}
    

    // catch Ctrl-F
    if((ctrlOn)&&(i1 == 'f')) {
      EDI_set_oid_ui ();  // set to oid of definitionLine from GUI
      goto AllDone;
    }

    // catch Alt-x
    if(altOn) {
      if(shiftOn) UI_key_spcShAlt (i1);
      else UI_key_spcAlt (i1);
    }


    // if(GUI_DATA_I1 == GUI_KeyReturn) {
      // // UI_key_return ();
      // l1 = ED_get_lnr_act();
      // --l1;
      // ED_lnr_act = l1;
      // // l1 -= 2;
      // // ED_work_CurSet (l1);
      // // UI_EdKeyCR (1);
      // goto AllDone;
    // }

    // test if it is a special-char
    if(isascii(i1)) goto AllDone;
    i1 = UI_key_view__ (GUI_DATA_EVENT, GUI_DATA_I1);
    if(i1) goto AllDone;


    if(i1 == GUI_KeyEsc) {
      UI_key_escape ();


    } else if(i1 == GUI_KeyF1) {   // HELP-key
      // test if process is active
      if(PRC_IS_ACTIVE) {
        APP_Help (APP_act_proc, "");
      } else {
        APP_Help ("gCAD3D_startMAN", "");
      }


    } else if(i1 == GUI_KeyF3) {
      UI_men__ ("Edit");
      ED_update (0L);


    } else if(i1 == GUI_KeyF4) {
      AP_APT_clean ();
    }

    goto AllDone;
*/




  //----------------------------------------------------------------
  } else if (GUI_DATA_EVENT == TYP_EventRelease) {   // 403
      // printf(" EDI_CB__-rel-key=%c (%x)\n",GUI_DATA_I1,GUI_DATA_I1);
    
    i2 = GUI_DATA_L2;  // modifier-keys
    shiftOn = i2&1;
    ctrlOn  = i2&4;
    altOn   = i2&8;
      // printf(" EDI_CB__-rel-shiftOn=%d ctrlOn=%d altOn=%d\n",
              // shiftOn,ctrlOn,altOn);
      // printf(" EDI_CB__-relea-shiftOn=%d ctrlOn=%d\n",shiftOn,ctrlOn);


    i1 = GUI_DATA_L1;  // key
    if(i1 == GUI_KeyEsc) { ED_Esc_CB__ (ctrlOn); goto Finish; }
    // if(i1 == GUI_KeyNumDel) { ED_Del_CB__ (ctrlOn); goto Finish; }

    if(i1 == GUI_KeyShift_L) goto L_ignore;
    if(i1 == GUI_KeyControl_L) goto L_ignore;
    

    // delete char .. GUI_KeyBackSpace GUI_KeyDel GUI_KeyNumDel
    if((i1 == GUI_KeyBackSpace) ||
       (i1 == GUI_KeyDel))        goto L_ignore;


    // skip all not printable keys
    if(!isascii(i1)) {    // if(i1 < 127) {

      // necessary; keyDown is not activated (in last line) ?
      if((i1 == GUI_KeyDel)    ||
         (i1 == GUI_KeyReturn))     {
          // printf(" release-CR..\n");
        // bei CR am end of file kommt kein TYP_EventEnter new-Line !!!!!
        // UI_EdKeyCR (); geht nicht - liefert zusätzliches CR !

        ED_update (0L);
        l1 = GUI_edi_getLnr (mo);
        if(ED_lnr_act >= l1) ED_lnr_act = l1 - 1;
        // l1 = ED_lnr_act; --ED_lnr_act;
          // printf(" ED_lnr_act=%d l1=%ld\n",ED_lnr_act,l1);
        UI_AP (UI_FuncSet, UID_ouf_lNr, (void*)l1);
        GL_temp_del_all ();    // remove circle
        WC_set_obj_stat (0);  // 0=perm
        ED_work_CurSet (l1);
        l1 = ED_get_lnr_act() - 1;  // get lNr AP_ED_lNr
        // printf(" lastLn %d |%s|\n",l1,mem_cbuf1); // filled by ED_Run
        if(i1 == GUI_KeyReturn) {  // no undo for delete -operations
          UNDO_grp_add (l1, 0);             // add new codeline to undo-list
          UNDO_upd__ ();
        }


      } else {
        // modify view (move/pan/zoom)
        UI_key_view__ (GUI_DATA_EVENT, i1);
      }
      goto AllDone;
    }


    // Shift + Alt
    // if((GUI_DATA_I2 & GUI_Modif_shift) && (GUI_DATA_I2 & GUI_Modif_alt)) {
    if(shiftOn && altOn) {
      UI_key_spcShAlt (i1);
      goto AllDone;
    }


    // check modifier Ctrl
    // if(GUI_DATA_I2 & GUI_Modif_ctrl) {
    if(ctrlOn) {
      // work keys with Ctrl; eg Ctrl-p (start plugin)
      UI_key_spcCtrl ((int)GUI_DATA_L1);
      goto AllDone;
    }


    // check modifier ALT
    // if(GUI_DATA_I2 & GUI_Modif_alt) {
    if(altOn) {
        // printf(" ALT IS ON\n");  

      // Alt-P provides next empty point-ID
      i1 = toupper(i1);
      p1 = strchr("PLCSABDTVRINM",i1);

      if(p1) {
        i1 = AP_typ_typChar (i1);
          // printf(" i1=%d |%c|\n",i1,i1);
        // i1 = UI_creObjHdr (NULL, PTR_INT(i1));
        l1 = AP_cre_defHdr (i1, 20);
        // if(i1 > 0) goto AllDone;
  
      } else {
        if(i1 == 'F') {ED_add_Def ("FROM "); goto AllDone;}
        if(i1 == 'W') {ED_add_Def ("WORK "); goto AllDone;}
        if(i1 == 'X') AP_work__ ("crashEx", NULL);
      }
      goto Finish;
    }



  } else {                       // 403 = TYP_EventRelease = key-release
    // printf("***** EDI_CB__ - I1 -%d\n",GUI_DATA_EVENT);

  }


  //----------------------------------------------------------------
  Finish:
  AllDone:
    return 0;


  L_ignore:
    return 1;
}


//================================================================
  int ED_goto__ (long curPos) {
//================================================================
/// ED_goto__           goto curPos
/// curPos -1L = goto EOF

// was UI_AP (UI_FuncSet, UID_Edit_Cpos);


  // printf("UI_AP UID_Edit_Cpos: CurPos -> %ld\n",l1);

  GUI_edi_setCpos (&winED, curPos);
  // AP_search_init ();

  return 0;

}


//================================================================
  int ED_save_file (char *fNam) {
//================================================================
/// ED_save_file        save editor -> file

  int     i1;
  long    l1, l2;
  char    *p1;


  // printf("ED_save_file |%s|\n",fNam);


  l1 = GUI_edi_getEof (&winED);
    // printf(" l1=%ld\n",l1);
  l2 = l1 + 100 + l1/4;  // for UTF8-characters


  // get tempspace
  p1 = MEM_alloc_tmp ((int)l2);


  // get editor -> tempspace
  i1 = GUI_edi_Read (p1, &l2, 0L, -1L, &winED);
  if(i1 < 0) {TX_Error("ED_save_file E001"); return -1;}

    // printf(" len-l2=%ld\n",l2);
  // necessary on MS:
  p1[l2] = '\0';  // 2013-04-11

  // add '\n' as last char
  UTX_endAddChar (p1, '\n');

  // save tempspace -> file
  UTX_wrf_str (fNam, p1);

    // printf(" fsiz=%ld\n",OS_FilSiz(fNam));

  return 0;

}


//================================================================
  int EDI_set_src_UI () {
//================================================================
// EDI_set_src_UI      get sourceText to find from user and set to this line


  int      irc, lns;
  long     lNr, pos0, pos1;
  char     *p1;

  static char s1[40];


  irc = GUI_Dialog_e2b ("text to find:", s1, 40, "next", "Cancel");
  if(irc != 0) return -1;
  lns = strlen(s1);
  if(lns < 1) return -1;

  // find line-nr of text <s1>
  lNr = ED_lnr_act + 1;
  p1 = UTF_find_tx1 (&lNr, s1);

  // set to lineNr
  EDI_set_lNr__ (lNr);

  // select the text <s1>
  pos0 = UTF_offset_ (p1); // get pos0 = offset of text
  pos1 = pos0 + lns;
  GUI_edi_sel__ (&winED, pos0, pos1);
 
    // printf("ex-EDI_set_src_UI\n");

  return 0;

}


//================================================================
  int EDI_set_lNr_UI () {
//================================================================
// EDI_set_lNr_UI         get lineNr from user and set to lineNr


  int      irc;
  long     lNr;
  char     s1[40], *p1;


  s1[0] = '\0';

  irc = GUI_Dialog_e2b ("lineNr to find:", s1, 40, "OK", "Cancel");
  if(irc != 0) return -1;

  lNr = strtol (s1, &p1, 10);
  if(lNr < 1) {
    if(strlen(s1) > 0) {TX_Print("**** error: key line-number .."); return -1;}
    return -1;
  }


  // set to lineNr
  EDI_set_lNr__ (lNr);

    // printf("ex-EDI_set_lNr_UI\n");

  return 0;

}


//================================================================
  int EDI_set_lNr__ (long lNr) {
//================================================================
// EDI_set_lNr__         set to lineNr


  // printf("EDI_set_lNr__ %ld\n",lNr);

  // goto line lNr; set ED_lnr_max
  ED_lnr_max = GUI_edi_setLnr (&winED, lNr);

  if(lNr > ED_lnr_max) lNr = ED_lnr_max;

  ED_lnr_act = lNr;

  // UI_AP (UI_FuncSet, UID_ouf_lNr, (void*)i1);
  UI_AP (UI_FuncSet, UID_ouf_lNr, PTR_INT(lNr));


  return 0;

}


/*  UNUSED; replaced by edit-button in Search/Name - UI_mcl_CB1/UI_FuncUCB14
//================================================================
  int EDI_set_oid_ui () {
//================================================================
// EDI_set_oid_ui       set to oid of definitionLine from GUI
// get oid from user
// find definition-lineNr of oid in memory
// set to definition-line in editor


  int    irc, dbTyp;
  long   dbi, dli, lNr;
  char   s1[128], oid[64], *buttons[]={"search ==>>",NULL};


  printf("EDI_set_oid_ui\n");


  // presets window
  strcpy(s1, "obj-to-find");
  strcpy(oid, "P20");


  L_nxt:
  // get oid from user
  // irc = GUI_Dialog_e2b (s1, s2, 64, "aa", NULL);
  irc = GUI_DialogEntry (s1, oid, 64, buttons, 0);
    printf(" ex-Dialog_e2b irc=%d |%s|\n",irc,oid);
  if(irc < 0) return 0;


  // get dbo from oid
  APED_dbo_oid (&dbTyp, &dbi, oid);


  // goto end of src
  irc = ED_work_CurSet (999999);
    // printf(" n._CurSet %d %d\n",irc,ED_lnr_act);
  if(irc < 0) return irc;


  // find definition-lineNr for objID <oid>
  irc = APED_find_dbo (&dli, &lNr, dbTyp, dbi);
  if(irc < 0) {
    sprintf(s1, "**** cannot find object \"%s\"",oid);
    TX_Print(s1);
    goto L_nxt;
  }
    // printf(" lNr=%ld dli=%ld\n",lNr,dli);

  ++lNr;  // set to line AFTER definition-lineNr - hilites (last) obj


  // goto line lNr
  // EDI_set_lnr (lNr);
  EDI_set_lNr__ (lNr);

    // printf("ex-EDI_set_oid_ui\n");

  return 0;

}
*/

//================================================================
  int ED_unload__ () {
//================================================================
/// ED_unload__         editor -> memory

// was UI_AP (UI_FuncGet,  UID_WinEdit, NULL);


  int   i1, i2;
  char  *p1, s1[256];


  // printf("UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU \n");
  // printf("ED_unload__ \n");


  // addOn-prog active ?
  if(PRC_IS_ACTIVE) {
      // printf(" _unload__ - write ed -> file\n");
    // save active process: copy editor -> file
    sprintf(s1, "%s%s", OS_get_tmp_dir(), APP_act_nam);
    ED_save_file (s1);

    goto L_exit;
  }


  //----------------------------------------------------------------
  // mormal: copy editor -> memory
  // Editfenster(gesamten APT-Text) > Memory kopieren
  // printf("Text->Mem modif=%d\n",xa_fl_TxMem);
  if(xa_fl_TxMem != 0) {      //Edittext - memory:  needUpdate
      // printf(" _unload__ - get ed -> mem\n");

    L_Get_WinEdit_1:
    UTF_FilBuf0Len = UTF_FilBuf0Siz;
    i1 = GUI_edi_Read (UTF_FilBuf0, &UTF_FilBuf0Len, 0L, -1L, &winED);
    if(i1 < 0) {
      i2 = UTF_alloc__ (UTF_FilBuf0Len);
      if(i2 < 0) { TX_Error("out of mem .."); return -1; }
      goto L_Get_WinEdit_1;
    }



   // cut at last \n (remove last unfinished line)  2012-11-18
   p1 = strrchr(UTF_FilBuf0, '\n');
   if(p1) {
       // printf(" bufLen=%ld %d\n",UTF_FilBuf0Len,strlen(UTF_FilBuf0));
       // printf(" buf=|%s|\n",UTF_FilBuf0);
     ++p1;
     *p1 = '\0';
     UTF_FilBuf0Len = p1 - UTF_FilBuf0;
   }

     // printf(" bufLen=%ld %d\n",UTF_FilBuf0Len,strlen(UTF_FilBuf0));
     // printf(" buf=|%s|\n",UTF_FilBuf0);
  }


  L_exit:
  xa_fl_TxMem = 0;

  return 0;

}

//================================================================
  int ED_load__ () {
//================================================================
/// ED_load__           mem -> editor



  // printf("ED_load__ %d\n",UI_InpMode);
  // printf(" UTF_FilBuf0Len=%ld\n",UTF_FilBuf0Len);
  // printf(" FilBuf0: %c %c %c %c\n",UTF_FilBuf0[0],UTF_FilBuf0[1],
                                   // UTF_FilBuf0[2],UTF_FilBuf0[3]);


  if(UI_InpMode != UI_MODE_MAN) return 0;


  // copy UTF_FilBuf0 -> Editor
  GUI_edi_Write (&winED, UTF_FilBuf0, &UTF_FilBuf0Len, UTF_FilBuf0Siz);
  // UI_Ed_fsiz = UTF_FilBuf0Len;
  ED_Reset ();  // ED_lnr_act = 0;

  return 0;

}


// EOF
