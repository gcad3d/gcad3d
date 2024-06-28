// Programming-functions.                           2008-04-05     RF
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
- add Name to formentries: problem; Ausgabe = EntryInhalt; da fehlt aber
  der Name !
- sollte ebenso wie PED die userselection umleiten !
  (statt mit UI_GR_Sel_Filt_set (17) usw..)

-----------------------------------------------------
Modifications:
2008-10-14  PRG_key_CB  if(FormTab.rNr < 1) return 0;  RF.
2008-07-08  Erste Version ex xa_dlg.c. RF.

-----------------------------------------------------
*/
#ifdef globTag
void PRG(){}
#endif
/*!
\file  ../xa/xa_prg.c
\brief Programming-functions                INF_PRG__
\code
=====================================================
List_functions_start:

PRG_Ed                 Edit active program
PRG_Loa                make List of all available programs
PRG_start              (re)run program

PRG_get_prgDir         get symDir "APPLI"
PRG_win__              main-DLG-Window
PRG_ButtonPress
PRG_CB                 CB from List-selection
PRG_key_CB             special-key pressed in mainWin
PRG_KeyPress           special-key pressed in programWin
PRG_ent_CB                 enter inputField-callback
PRG_ent_msg                new inputField; display prompt-message
PRG_sel_CB                 selection-callback; CAD-Core reports selection
PRG_inp_nxt                change Inputfield
PRG_dec_defLn              check if Line cBuf is a Definitionline & decode
PRG_eval_subst             evaluate String; eg change ""P<V1>=.." -> "P5=.."
PRG_eval_expr              evaluate IF-expression
PRG_clear              clear and unload plugin <APP_act_nam>

PRG_update                 read data from form; copy data -> prog.
PRG_dlg__                  fill FormTab
PRG_def_ini                decode definitionLine, update dialog-window
PRG_addRec                 add record to FormTab
PRG_addWp                  add widget-pointer to last record

PRG_sel_disp               display obj temporary
PRG_inp_nxt                change Inputfield
PRG_ckb_len                get lenght of this CheckboxChain;
PRG_ckb_mod                test if CheckboxChain has changed ..
PRG_dumpRec                dump FormTab 
PRG_intern_exit            kill all internal vars

PRG_Cre__              create new (empty) prg
PRG_Del__              delete prg

IO_wri_wri
IO_wri_ope

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


Method:
  Program from File into memSpc UTF_FilBuf1 einlesen;
    nur einmal in APP_prg_start, nicht mehr bei update.
  Loop tru Program (in UTF_FilBuf1);
  Output des Prog in Datei ../tmp/PRG_ACT.gcad schreiben;
    mit OK dieses outFile ans Mainmodel hintanhaengen.
  Alle vom User korrigierten Werte im UTF_FilBuf1 aendern;
    beim next Update werden alle neu in die Dialogfelder eingefuegt.

--------------------------------------------
Debug Programs:
PRI "V7=" V7
DEBUG ON


--------------------------------------------
AP_dir_prg     (ex tmp/prgdir.lst)
APP_act_nam


*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include "../ut/ut_umem.h"             // UME_save
#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_os.h"               // AP_get_bas_dir ..
#include "../ut/ut_obj.h"              // UTO_stru_2_obj UTO_obj_save
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_itmsh.h"            // MSHIG_EDGLN_.. typedef_MemTab.. Fac3
#include "../ut/ut_txfil.h"            // UTF_GetnextLnPos
#include "../ut/ut_cast.h"             // INT__PTR
#include "../ut/ut_gtypes.h"           // AP_src_typ__
#include "../ut/ut_TX.h"               // TX_Print
#include "../ut/func_types.h"          // UI_Func...

#include "../gui/gui__.h"              // Gtk3

#include "../gr/ut_DL.h"               // DL_GetAtt
#include "../gr/ut_GL.h"               // GL_GetCen
#include "../gr/ut_gr.h"               // GTX_..

#include "../db/ut_DB.h"               // DB_get_VC


// #include "../xa/xa_ui.h"               // UID_..
#include "../xa/xa_edi__.h"               // ED_get_actPos
#include "../xa/xa_mem.h"              // memspc51, mem_cbuf1, mem_cbuf1
#include "../xa/xa_ui_gr.h"            // UI_GR_get_actPos_
#include "../xa/xa.h"                  // AP_dir_prg AP_sym_prg APP_act_*
#include "../xa/xa_ato.h"              // ATO_getSpc_tmp__
#include "../xa/xa_uid.h"              // UI_MODE_..
#include "../xa/xa_msg.h"              // MSG_cancel,


#define CKBTYP 1
#define RDBTYP 2
#define INPTYP 3


typedef struct {MemObj wp;
                long sInd, slNr;
                unsigned char rTyp, vTyp, iStat;}    FormVar;
// rTyp   typ of record: 1=CKBTYP;   2=RDBTYP; 3=INPTYP;
// wp     widget-pointer
// sInd   CKB: index of DB-Var for output of state of checkbox
//        INP: index of DB-Var for output of obj.
// vTyp   INP: objTyp; eg Typ_VAR|Typ_PT|Typ_VC
// iStat  CKB: status of checkbox; 0=Off; 1=On.
// slNr   CKB: Modelcode-LineNr in der die OutputVar (sInd) auszugeben ist


typedef_MemTab(FormVar);



//===============================================================
// Externe Variablen:

// ex ../gr/ut_DL.c
extern long DL_temp_ind;        // if(>0) fixed temp-index to use; 0: get next free


//===============================================================
// Interne Variablen:

  static FILE      *IO_fp=NULL;
  static FILE      *PRG_fp = NULL;
  static MemObj    PRG_win0, PRG_box;

  static MemTab(FormVar) FormTab = _MEMTAB_NUL;
  static MemTab(IndTab) PRG_internTab = _MEMTAB_NUL;


  static int       PRG_dbg=0;   // 0=off; 1=On.
  static int       PRG_iAct;

  static int       PRG_stat=0;  // 0 = kein window aktiv;
                                // 1 = window (PRG_win0) is active
                                // 2 = Update is active
  static int       PRG_mode=0;  // 0 = normal update; 1=cancel; 2=save.
  static int       PRG_ED_stat;

  static int       PRG_key_ctrl=0;  // 0=off; 1=On.
  static long      PRG_dli;
  static long      PRG_lNr;


//===============================================================
// Prototypes:
  // gint PRG_ButtonPress (void *widget, void *event, gpointer data);
  // gint PRG_KeyPress (void *parent, void *event);
  // gint PRG_KeyRelease (void *parent, void *event);
  int PRG_ent_CB (MemObj *mo, void **data);
  int PRG_sel_CB (long dli, int typ, long ind, char *sbuf);



//================================================================
  char* PRG_get_prgDir () {
//================================================================
// PRG_get_prgDir         get symDir "APPLI"
// returns pointer to extern char AP_dir_prg[SIZMFTot]
//   = full path of prgDir (symDir "APPLI")

  char   *spd;


  MDLFN_fDir_syDir (AP_dir_prg, "APPLI");

    // printf(" ex-PRG_get_prgDir |%s|\n",AP_dir_prg);

  return AP_dir_prg;

}


//================================================================
  int PRG_Cre__ () {
//================================================================

  int    irc;
  char   s1[SIZFNam], fnam[SIZFNam];


  PRG_get_prgDir ();

  printf("PRG_Cre__ |%s|\n",AP_dir_prg);

  if(IE_cad_exitFunc()) return -1;


  strcpy(s1, "APP_1");
  irc = GUI_dlg_e2b (s1, 80, "name for new program:", "OK", "Cancel");
  if(irc != 0) return -1;


  // make safeName
  UTX_safeName (s1, 1); // change '. /\\'

  // check maxLen; max 62 chars
  if(strlen(s1) > 62) {
    TX_Print("max Laenge = 62 chars; gekuerzt !!");
    s1[62] = '\0';
  }

  APP_act_typ = 1;
  UI_Set_typPrg ();
  strcpy(APP_act_nam, s1);
  UI_Set_actPrg (APP_act_nam, 2);

  printf(" prgnam |%s|\n",APP_act_nam);

  // create file
  sprintf(fnam, "%s%s.gcap",AP_dir_prg,APP_act_nam);
  if(!OS_checkFilExist(fnam,1)) {
    sprintf(s1,"# %s.gcap",APP_act_nam);
    UTX_wrf_str (fnam, s1);
  }

  PRG_Ed ();

  return 0;

}


/*
//================================================================
  int PRG_Cre__ () {
//================================================================

  printf("PRG_Cre \n");

  // prgNam  vom user abfragen
  GUI_GetText ("name for new program: ",
              "", -300, PRG_Cre_CB);    // defaultname = old name


  return 0;

}

//=================================================================
  int PRG_Del_Del_CB (MemObj *mo, void **data) {
//=================================================================

  char  fnam[256];

  // printf("PRG_Del_Del_CB %d\n",(int)data);


  if(GUI_DATA_EVENT != UI_FuncOK) return 0;

  // delete file
  sprintf(fnam,"%s%s",AP_dir_prg,memspc011);
  TX_Print("delete program-File %s",fnam);
    printf(" _Del_Del_CB |%s|\n",fnam);
  OS_file_delete (fnam);

  return 0;

}

//====================================================================
  int PRG_Del_CB (char *cNam) {
//====================================================================
// Catalog selected;

  char   cbuf[256];

  if(cNam == NULL) return 0;  // cancel

  // printf("CTLG_Del_CB |%s|\n",cNam);
  strcpy(memspc011, cNam);

  // delete Catalog
  sprintf(cbuf,"  OK to delete Program %s ?  ",cNam);

  GUI_DialogYN  (cbuf, PRG_Del_Del_CB);

  return 0;

}
*/

//================================================================
  int PRG_Del__ () {
//================================================================
// AP_dir_prg

  int    i1;
  char   fnam[SIZFNam+SIZMFNam], s1[256], s2[320], *p1;


  PRG_get_prgDir ();

  printf("PRG_Del |%s|\n",AP_dir_prg);


  if(IE_cad_exitFunc()) return -1;

  // create List of all programs ...
  sprintf(fnam,"%sProgram.lst",AP_get_tmp_dir());

  // list dir/*.typ
  if(UTX_dir_listf (fnam, AP_dir_prg, NULL, ".gcap", 0) < 0) {
    TX_Error("PRG_Del__ E001");
    return 0;
  }

  // display Liste of <symDir-CATALOG>/*.ctlg for userSelection
  i1 = GUI_listf1__ (s1, sizeof(s1), fnam, "- delete Application -", "x40,y30");
  if(i1 < 0) return -1;
    // printf(" PRG_Del__-L1 |%s|\n",s1);


  // ask user - delete ? yes / no
  sprintf(s2, " delete Applic.script %s ?", s1);
  i1 = GUI_dlg_2b (s2, MSG_const__(MSG_ok), MSG_const__(MSG_no));
    // printf(" PRG_Del__-L2 %d\n",i1);
  if(i1 != 0) return -1;            // error or cancel
  

  // delete file
  sprintf(fnam,"%s%s",AP_dir_prg,s1);
  TX_Print("delete program-File %s",fnam);
  OS_file_delete (fnam);


  // if deleted file is active: unload.
  p1 = strchr (s1, '.');
  if(p1) {
    i1 = p1 - s1;
      // printf(" PRG_Del__-i1 %d\n",i1);
    if(strncmp(APP_act_nam, s1, i1)) goto L_exit;
  }

  // unload active prg, clear
  PRG_clear ();


  L_exit:
  return 0;

}


//================================================================
   int PRG_clear () {
//================================================================
// PRG_clear       clear and unload plugin <APP_act_nam>

  printf("PRG_clear \n");

  // unload active prg, clear
  DLL_plu_unl ();
  APP_act_typ = 0;
  UI_Set_typPrg ();
  strcpy(APP_act_nam, "-");
  UI_Set_actPrg (APP_act_nam, 0);  // reset active program-name

  return 0;

}


//================================================================
  int PRG_win__ (MemObj *mo, void **data) {
//================================================================
// PRG_win__             kill active Form ???
 
  static long dlAct;
  static long lnAct;

  int         iFunc, i1, i2;
  char        cbuf[SIZFNam], *cPos;
  MemObj      box0;


  iFunc = GUI_DATA_I1;

  // printf("PRG_win__ %d %d\n",iFunc,PRG_stat);


  //----------------------------------------------------------------
  if(iFunc != UI_FuncInit) goto L_UCB1;
      sprintf(cbuf,"%s%s.gcap",AP_dir_prg,APP_act_nam);
      printf("PRG_win__ - start application %s\n",cbuf);
    if(PRG_stat != 0) {           // Win schon vorhanden ? Kill .
      // MemTab_free (&FormTab);
      // MemTab_free (&PRG_internTab);
      // return 0;
      GUI_Win_kill (&PRG_win0);
    }

    PRG_get_prgDir ();   // get symDir "APPLI"

    PRG_stat = 1;
    
    dlAct = DL_get__ (NULL);
    lnAct = ED_get_lnr_act ();

    // init win
    PRG_win0 = GUI_Win__ (APP_act_nam, PRG_win__, "");

    // create vertical box
    PRG_box = GUI_box_v (&PRG_win0, "");

    // loop tru program; find & create widgets
    PRG_dlg__ ();

    // finish & display form
    GUI_sep_h (&PRG_box, 5);

    box0 = GUI_box_h (&PRG_box, "");

    // create Update-Button
    GUI_button__ (&box0, " Update ",   PRG_win__, &GUI_FuncUCB1, "e");
      GUI_Tip ("update Model (right Mousebutton)");

    // create Cancel-Button
    GUI_button__ (&box0, " Cancel ",   PRG_win__, &GUI_FuncUCB2, "e");
      GUI_Tip ("exit Form");

    // create OK-Button
    GUI_button__ (&box0, "   OK   ",     PRG_win__, &GUI_FuncUCB3, "e");
      GUI_Tip ("update and copy objects to Model (Ctrl + right Mousebutton)");


    // always on top
    GUI_Win_up (NULL, &PRG_win0, 0);
    GUI_Win_go (&PRG_win0);

    // now activate Inputmode "Form"
    UI_GR_Sel_Filt_set (17);             // activate selectionFilter
    DB_set_state (0);                  // save all DB-pointers

    PRG_mode = 0; // 0 = normal update; 1=cancel; 2=save.

    return (0);
    // goto L_Exit1;



  //----------------------------------------------------------------
  L_UCB1:      // Update: rerun
    if(iFunc != UI_FuncUCB1) goto L_UCB2;
    // read data from form; copy data -> prog.
    PRG_update ();
    if(AP_errStat_get()) return -1;
    // update all
    PRG_start ();
    return (0);



  //----------------------------------------------------------------
  L_UCB2:      // Cancel:    exit;
    if(iFunc != UI_FuncUCB2) goto L_UCB3;
    PRG_mode = 1; // 0 = normal update; 1=cancel; 2=save.
    if(PRG_stat != 0) goto L_Exit0;
    return (0);
    // goto L_Cancel;


  //----------------------------------------------------------------
  L_UCB3:      // OK: add to Mainprg, exit.
    if(iFunc != UI_FuncUCB3) goto L_Kill;
    PRG_mode = 2; // 0 = normal update; 1=cancel; 2=save.

    // update
    PRG_win__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB1));
    // clear DL (remove all output from macro) 
    GL_Delete (dlAct);
    // fix filename
    sprintf(cbuf, "%sPRG_ACT.gcad",AP_get_tmp_dir());
    // add macroSource to mem
    UTF_add_file (cbuf);
    // if MAN is active: update (editor -> mem)
    AP_SRC_edi_mem ();
    goto L_Exit0;


  //----------------------------------------------------------------
  L_Kill:      // kill win
    if((iFunc != UI_FuncKill)   &&
       (iFunc != UI_FuncExit))       return (-1);

  L_Exit0:  // Cancel(PRG_mode=1) and OK (PRG_mode=2)
    if(!GUI_OBJ_IS_VALID(&PRG_win0)) return (-1);
      // printf(" L_Exit0: PRG_stat=%d PRG_mode=%d\n",
              // PRG_stat,PRG_mode);

    // kill all dynamic (internal) vars in DB
    PRG_intern_exit ();

    // del all temp obj's
    GL_temp_del_all ();
    // unhilite all
    DL_hili_off (-1l);   // only for VWR

    if(PRG_mode < 2) {
      // Cancel, not OK: reset DL and DB
      GL_Delete (PRG_dli);  // alle Objekte ab dli loeschen
      DB_set_state (1);     // alle neu erzeugten DB-objekte loeschen
      DL_Redraw ();         // Redraw DispList
    }


    if(PRG_stat != 0) {
      PRG_stat = 0; // gtk_widget_destroy signals again !
      GUI_Win_kill (&PRG_win0);
      UI_GR_Sel_Filt_reset ();      // reset selectionFilter
      // AP_UserKeyIn_reset ();      // reset  KeyIn (give back to mainApp)
      // AP_UserSelection_reset ();  // reset  select (give back to mainApp)
    }

    MemTab_free (&FormTab);
    MemTab_free (&PRG_internTab);

    // Reset Errors
    if(AP_errStat_get() != 0) {
      AP_errStat_set (0);
      DL_Redraw ();               // Redraw DispList
    }

    UI_Set_actPrg (NULL, 0);      // reset label active program-name
    AP_stat.APP_stat = 0;
    // only if OK is active: update browser
    if(iFunc == UI_FuncUCB3) {
      // OK ..
      Brw_Mdl_upd ();
      WC_set_obj_stat (0);             // set obj-status permanent
      ED_work_CurSet (UT_INT_MAX);     // work new created lines - until end
      // UNDO_app__ (1);             // create undo-record
      UNDO_grp_range_add (lnAct, ED_get_lnr_act());      // create undo-record
    }
    ED_set_mode (PRG_ED_stat);         // reset ED_mode
    return (0);

}

/*
//================================================================
  gint PRG_ButtonPress (void *widget,
                        void *event, gpointer data) {
//================================================================
// funktionier nur LabelZonen, nicht CKB, InputFields.
// M3: update

  // state = event->state;
  // typ   = event->type;

  printf("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB \n");
  printf("PRG_ButtonPress \n");

  if(((GdkEventButton*)event)->button == 3) {
    printf(" M3\n");
    PRG_win__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB1));       // update
  }

  Fertig:
  // printf("ex UI_GR_ButtonPress %d %d %d\n",x, y, state);
  return FALSE;
  // return TRUE;
}
*/
/* UNUSED
//=====================================================================
  gint PRG_KeyRelease (void *parent, void *event) {
//=====================================================================

  int   iKey;

  printf("KKKKKKKKKKKKKKKKKKKKKKKKKKKKKK \n");
  printf("PRG_KeyRelease \n");


  iKey = ((GdkEventKey*)event)->keyval;

  if((iKey == GDK_Control_L)||(iKey == GDK_Control_R)) {
    PRG_key_ctrl = 0;  // 0=off; 1=On.
      // printf(" ctrl-off\n");
  }

  return FALSE;  // continue with defaultHandler

}
*/
/* UNUSED
//=====================================================================
  gint PRG_KeyPress (void *parent, void *event) {
//=====================================================================
// Key pressed in programWin
// Offen: soll ev PRG_key_CB aufrufen ..

  int   iKey;

  iKey = ((GdkEventKey*)event)->keyval;

  printf("KKKKKKKKKKKKKKKKKKKKKKKKKKKKKK \n");
  printf("PRG_KeyPress %x typ=%d\n",((GdkEventKey*)event)->keyval,
                                    ((GdkEventAny*)event)->type);

  // skip if prog is running ..
  if (PRG_stat > 1) return TRUE;    // skip defaultHandler


  if(iKey == GDK_Return) {
    PRG_win__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB1));
    goto L_skip_handler;

  } else if((iKey == GDK_Control_L)||(iKey == GDK_Control_R)) {
    PRG_key_ctrl = 1;  // 0=off; 1=On.
    printf(" ctrl-on\n");

  } else if(iKey == GDK_Page_Up) {
    PRG_key_CB ("PgUp");
    goto L_skip_handler;

  } else if(iKey == GDK_Page_Down) {
    PRG_key_CB ("PgDwn");
    goto L_skip_handler;

  } else if(iKey == GDK_Tab) {
    PRG_key_CB ("Tab");
    goto L_skip_handler;

  } else if(iKey == GDK_Escape) {
    PRG_key_CB ("Esc");
    goto L_skip_handler;

  }

  return FALSE;  // continue with defaultHandler


  L_skip_handler:
  return TRUE;  // skip defaultHandler

}
*/

//================================================================
  int PRG_ent_CB (MemObj *mo, void **data) {
//================================================================
// enter inputField-callback

  int    i1;

  // if(event->type != GDK_FOCUS_CHANGE) return 0;
  if(GUI_DATA_EVENT != TYP_EventEnter) return 0;


  // PRG_iAct = find field-Nr of new active field
  for(i1=0; i1<FormTab.rNr; ++i1) {
    // if(FormTab.data[i1].wp != parent) continue;
    if(GUI_obj_cmp(&(FormTab.data[i1].wp), mo)) continue;
    if(i1 == PRG_iAct) return 0;
    PRG_iAct = i1;
    break;
  }
    // printf("PRG_ent_CB enter field PRG_iAct=%d\n",PRG_iAct);



  // new inputField; display prompt-message
  PRG_ent_msg ();

  return 0;

}


//================================================================
  int PRG_ent_msg () {
//================================================================
// new inputField; display prompt-message


  // if(FormTab.data[PRG_iAct].rTyp == CKBTYP) return 0;
  if(FormTab.data[PRG_iAct].rTyp != INPTYP) return 0;


  // see also IE_cad_msg_typ
  if(FormTab.data[PRG_iAct].vTyp == Typ_VAR) {
    TX_Print ("Key Value");

  } else if(FormTab.data[PRG_iAct].vTyp == Typ_PT) {
    TX_Print ("Point: Sel Pt,Crv / Indicate Pt");

  } else if(FormTab.data[PRG_iAct].vTyp == Typ_VC) {
    TX_Print ("Vector: Key PgUp/PgDwn / D[I]X/Y/Z / dx dy [dz] /"
      " Sel. LN/Cir/PLN ");

  }


  return 0;

}


//================================================================
  int PRG_sel_CB (long dli, int typ, long ind, char *sbuf) {
//================================================================
// selection-callback; CAD-Core reports selection
//
// Input:   DL-Index, DB-Typ, DB-Index and obj-ID of selected Obj
//

  int       irc, typReq;
  char      cbuf[256], namBuf[32];


  printf("PRG_sel_CB typ=%d ind=%ld dli=%ld |%s|\n",typ,ind,dli,sbuf);

  // skip if prog is running ..
  if (PRG_stat > 1) {irc = 0; goto L_exit;}


  // fix typReq = requested objectTyp; TypVAR|Typ_PT|Typ_VC
  typReq = FormTab.data[PRG_iAct].vTyp;
    // printf(" typReq=%d\n",typReq);

  if((typReq == Typ_PT)&&(typ == Typ_TmpPT)) goto L_add_OK;

  if(typReq == typ) goto L_add_OK;


  // if(typReq == Typ_VAR) {
    // TX_Print("extract Var not yet supported");
    // return 0;
  // }


  // create ModelCode of typ <typReq> from selection (typSel, ind)
  // irc = AP_src_selObj (cbuf, typReq, typ, ind);
  cbuf[0] = '\0';
  // irc = SRC_dbo (cbuf, typReq, typ, ind);
  irc = SRC_src_pt_dbo (cbuf, sizeof(cbuf), typReq, NULL, typ, ind);
    // printf(" ex _dbo %d |%s| %d\n",irc,cbuf,typReq);

  if(irc < 0) {
    if(typ != Typ_TmpPT) APED_oid_dbo__ (namBuf, typ, ind);
    else strcpy(namBuf, "pointPosition");
    sprintf(cbuf,"***** cannot use selection of %s for requested %s",
            namBuf, AP_src_typ__(typReq));
    TX_Print(cbuf);
    irc = -1;
    goto L_exit;
  }
  goto L_done;


  //----------------------------------------------------------------
  L_add_OK:
    strcpy (cbuf, sbuf);


  // display & goto next
  L_done:
  PRG_sel_disp (cbuf, 1);

  irc = 0;


  //----------------------------------------------------------------
  L_exit:
      printf(" ex-PRG_sel_CB %d\n",irc);
    return irc;

}


//================================================================
  int PRG_key_CB (char *keyCode) {
//================================================================
// DLG_ctrlkey_CB       special-key pressed in mainWin | called by PRG_KeyPress


  int   i1, i2;
  char  cbuf[32];


  // printf("PRG_key_CB |%s| PRG_iAct=%d\n",keyCode,PRG_iAct);
  // printf(" rNr=%d _stat=%d _dbg=%d\n",FormTab.rNr,PRG_stat,PRG_dbg);
  // PRG_dumpRec ();


  // skip if prog is running ..
  if (PRG_stat > 1) return 0;


  //----------------------------------------------------------------
  if(!strcmp(keyCode, "Esc")) {              // ESC - Key
    // im Debug-Mode nix tun
    if(PRG_dbg != 0) return 0;
    i1 = -1; goto L_chg_field;


  //----------------------------------------------------------------
  } else if(!strcmp(keyCode, "Tab")) {              // TAB - Key
    i1 = 1; goto L_chg_field;


  //----------------------------------------------------------------
  } else if(!strcmp(keyCode, "PgUp")) {              // PgUp - Key
    if(FormTab.rNr < 1) return 0;
    if(FormTab.data[PRG_iAct].vTyp != Typ_VC) return 0;
    i1 = -1; goto L_chg_vec;


  //----------------------------------------------------------------
  } else if(!strcmp(keyCode, "PgDwn")) {              // PgUp - Key
    if(FormTab.rNr < 1) return 0;
    if(FormTab.data[PRG_iAct].vTyp != Typ_VC) return 0;
    i1 = 1; goto L_chg_vec;


  //----------------------------------------------------------------
  } else if(!strcmp(keyCode, "MBR")) {        // right MouseButton
    if(PRG_key_ctrl == 1) i1 = UI_FuncUCB3;
    else                  i1 = UI_FuncUCB1;
    PRG_win__ (NULL, GUI_SETDAT_EI(TYP_EventPress,i1));


  //----------------------------------------------------------------
  // in MS kommt Ctrl direkt
#ifndef _MSC_VER
  } else if(!strcmp(keyCode, "CMBR")) {        // Ctrl + right MouseButton
    PRG_win__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB3));
#endif

  }

  return 0;


  L_chg_field:
    if(FormTab.rNr < 1) return 0;
    // activate previous inputField
    PRG_inp_nxt (i1);
    // focus to this inputField
    // gtk_widget_grab_focus (FormTab.data[PRG_iAct].wp);
    GUI_obj_focus (&(FormTab.data[PRG_iAct].wp));
    // new inputField; display prompt-message
    PRG_ent_msg ();
    return 0;



  L_chg_vec:
    // get next Vec from DB..
    i1 = AP_get_nxtVec (i1);
    APED_oid_dbo__ (cbuf, Typ_VC, i1);   // text < typ, dbi
    // display & goto next
    PRG_sel_disp (cbuf, 0);
    return 0;

}


//================================================================
  int PRG_sel_disp (char *cbuf, int mode) {
//================================================================
// display obj temporary
// copy ModelCode -> Inputfield
// mode : 1=proceed to next InputField, else not.


  int       ind, typReq;
  char      auxObj[OBJ_SIZ_MAX];
  void      *vp1;
  Point     pt1;


  printf("PRG_sel_disp |%s| %d\n",cbuf,mode);


  // fix typReq = requested objectTyp; TypVAR|Typ_PT|Typ_VC
  typReq = FormTab.data[PRG_iAct].vTyp;


  // Create struct from ModelCode.
  APT_obj_expr (auxObj, typReq, cbuf);


  //----------------------------------------------------------------
  // delete temp.symbol for this inputField
//   l1 = -2 - PRG_iAct;
  DL_temp_ind = PRG_iAct + 1;
  GL_temp_del_1 (DL_temp_ind);


  // display circle-symbol at pos. pt1
  if(typReq == Typ_PT) {
//     GL_DrawSymB (&l1, 2, SYM_CIR_S, (Point*)auxObj);
    GR_temp_symB ((Point*)auxObj, SYM_CIR_S, ATT_COL_RED);


  } else if(typReq == Typ_VC) {
    pt1 = GL_GetCen();   // ScreenCenter in UserCoords
//     GL_DrawSymV3 (&l1, SYM_ARROW, 9, &pt1, (Vector*)auxObj, 20.);
    GR_temp_symV3 (SYM_ARROW, Typ_Att_hili1, &pt1, (Vector*)auxObj, 20.);


  } else if(typReq == Typ_LN) {
//     GR_tDyn_pcv (&l1, 9, 2, (Point*)auxObj);
    GR_temp_ln ((Line*)auxObj, ATT_COL_RED); 
  }


  DL_Redraw ();               // Redraw DispList


  //----------------------------------------------------------------
  // copy ModelCode -> Inputfield
  // GUI_Entry_set (FormTab.data[PRG_iAct].wp, cbuf);
  GUI_entry_set (&FormTab.data[PRG_iAct].wp, cbuf);

  // activate next Inputfield
  if(mode == 1) {
    PRG_inp_nxt (1);
    // gtk_widget_grab_focus (FormTab.data[PRG_iAct].wp);
    GUI_obj_focus (&(FormTab.data[PRG_iAct].wp));

    // new inputField; display prompt-message
    PRG_ent_msg ();
  }

  return 0;

}


//================================================================
  int PRG_inp_nxt (int mode) {
//================================================================
// mode =  1    change PRG_iAct to next Inputfield
// mode = -1    change PRG_iAct to previous Inputfield

  int   i1, iTot;

  // printf("PRG_inp_nxt %d\n",mode);


  // find total # of inputFields
  iTot = 0;
  for(i1=0; i1<FormTab.rNr; ++i1) if(FormTab.data[i1].rTyp == INPTYP) ++iTot;
  if(iTot < 1) return 0;


  //----------------------------------------------------------------
  // goto previous inputField
  if(mode > 0) goto L_nxt;
  L_prv:
  --PRG_iAct;
  if(PRG_iAct < 0) PRG_iAct = FormTab.rNr - 1;
  if(FormTab.data[PRG_iAct].rTyp != INPTYP) goto L_prv;
    // printf(" new PRG_iAct=%d\n",PRG_iAct);

  return 0;



  //----------------------------------------------------------------
  // goto nxt inputField
  L_nxt:
  ++PRG_iAct;
  if(PRG_iAct >= FormTab.rNr) PRG_iAct = 0;
  if(FormTab.data[PRG_iAct].rTyp != INPTYP) goto L_nxt;
    // printf(" new PRG_iAct=%d\n",PRG_iAct);

  return 0;

}


//================================================================
  int PRG_dec_defLn (char *cbuf) {
//================================================================
// cbuf is a definitionline; test for NEW
// lines "V=NEW()" sofort auswerten, nicht via WC_Work1;
// RetCod = 0: yes, p1 is a "NEW"-Line
//         -1: No.

// Example NEW:
// # get a new pointindex for the outputpoints
// V3=NEW(P,10)
// # evaluate
// P<V3>=P(<X(P1)> 0 0)


// see also APED_search_defLn APED_ck_defLn


  int    irc, iTyp, iOff;
  long   ind;
  char   *cp1, *cp2;


  // printf("PRG_dec_defLn PRG_stat=%d |%s|\n",PRG_stat,cbuf);


  // check for NEW
  irc = UTX_ck_casenChr(cbuf, "NEW", 3);  // 0=equal
  if(irc != 0) return -1;


  // work NEW: remove NEW.  substitute with value.
  // cp1 += 3;
  cp1 = cbuf + 3;
  UTX_pos_skipLeadBlk(cp1);
  
  if(*cp1 != '(') {TX_Error("PRG_dec_defLn E001"); return -1;}
  ++cp1;
  UTX_pos_skipLeadBlk(cp1);  // proceed to typChar

  cp2 = cp1 + 1;
  cp2 = UTX_pos_skipDeli1 (cp2);  // skip delimiter

  iOff = atoi(cp2);

  ind = DB_QueryNxtFree (AP_typ_typChar(*cp1), iOff);     // 2013-10-06
    // printf("ex NxtInd %ld\n",ind);


  // sprintf(cbuf, "V%d=%d",ind,irc);
  sprintf(cbuf, "%ld",ind);
    // printf("ex PRG_dec_defLn |%s|\n",cbuf);

  return 0;

}


//================================================================
  int PRG_eval_expr (char **cmd, char *cbuf) {
//================================================================
// evaluate IF-expression.
// Output:
//   cmd        points to the command if RetCod=0
//   RetCod:
//     -1       Error.
//     0        skip command; continue (expr-result is no)
//     1        command (eg "V1=10"), continue
//     2        execute jump

// see also APT_work_TPC_IF



  int       irc, aus_anz;
  int       aus_typ[10];
  double    aus_tab[10];
  char      *p1, *p2;
  ObjAto    ato1;



  // printf("PRG_eval_expr |%s|\n",cbuf);

  p1 = cbuf + 2;

  // search ';' - terminate expression.
  p2 = strchr(p1, ';');
  if(p2 == NULL) {TX_Error("cannot find ; in IF-Exprssion"); return -1;}
  *p2 = '\0';
  ++p2;
  UTX_pos_skipLeadBlk(p2); // p2 is the following command now


    // for(irc=0; irc<aus_anz; ++irc) {
      // printf(" %d typ=%d tab=%f\n",irc,aus_typ[irc],aus_tab[irc]);
    // }



  //----------------------------------------------------------------
  // evaluate command; -1=Error, 0=no, 1=yes.
  ATO_getSpc_tmp__ (&ato1, 6);
  // decode data
  ATO_ato_srcLn__ (&ato1, p1);

  // work IF
  irc = APT_eval_if (ato1.typ, ato1.val);

  if(irc < 0) goto L_Error;
  if(irc == 0) return 0;    // no

  // 1, yes; check for jump
  if(UTX_ck_casenChr(p2, "JUMP" , 4) == 0) {
    p2 += 4;   // skip the "JUMP"
    UTX_pos_skipLeadBlk(p2);
    irc = 2;
    *cmd = p2;
  }


    // printf("ex PRG_eval_expr %d |%s|\n",irc,*cmd);

  return irc;


  L_Error:
    TX_Error("cannot evaluate IF-Expression |%s|",p1);
    return -1;

}


//================================================================
  int PRG_eval_subst (char *cbuf) {
//================================================================
// evaluate String; eg change ""P<V1>=.." -> "P5=.."

  int    irc, iNr;
  long   lTot, lExpr, lNew, dbi;
  char   *cp1, *cp2, sExpr[256], sNew[80];
  double d1;
  ObjAto ato;


  // printf("PRG_eval_subst |%s|\n",cbuf);


  //----------------------------------------------------------------
  // die Klammern suchen (die innersten zuerst; Startpos, Laenge),
  // see UTX_cp_expr UTX_pos_skipBrack UTX_CleanBracks
  iNr = 0;


  // copy expr (text between <>) into sExpr
  // find next '<'
  L_next:
  cp1 = strchr(cbuf, '<');
  if(cp1 == NULL) return iNr;

  cp2 = strchr(cp1, '>');
  if(cp1 == NULL) {TX_Error("PRG_eval_subst E001"); return -1;}

  lExpr = cp2 - cp1 + 1;
  if(lExpr > 255) {TX_Error("PRG_eval_subst E002"); return -1;}

  strncpy(sExpr, &cp1[1], lExpr-2);  // skip brackets
  sExpr[lExpr-2] = '\0';
    // printf(" _eval_subst lExpr=%ld sExpr=|%s|\n",lExpr,sExpr);



  // decode expr
  ATO_getSpc__ (&ato);  // get memspc55,memspc54,memspc53
  irc = ATO_ato_srcLn__ (&ato, sExpr);
    // printf(" irc=%d\n",irc);
    // ATO_dump__ (&ato);

  // get sourceObject from dbo
  SRC_src_isol_ato1 (sNew, ato.typ[0], &ato.val[0]);
    // printf(" _eval_subst-f-isol_ato1 sNew=|%s|\n",sNew);



/*
  // evaluate sExpr into sNew
  if(sExpr[0] == 'P') {
      printf(" eval point ..\n");
    // sprintf(sNew, "P(1 2 3)");
    // SRC_isol_dbo (sNew, typ, dbi);
  
  } else if(sExpr[0] == 'V') {
    // den Inhalt decodieren (zB V10)  oder V10+1) usw ...
    // see APT_decode_var
    irc = APT_obj_expr (&d1, Typ_Val, sExpr);
    if(irc != 0) {TX_Error("PRG_eval_subst E003"); return -1;}
      // printf(" decode %f\n",d1);
    sNew[0] = '\0';
    UTX_add_fl_u (sNew, d1);

  } else {
    TX_Error("PRG_eval_subst E004");
    return -1;
  }
*/


  // modify, austauschen.
  lTot = strlen(cbuf);  // total length
  lNew = strlen(sNew);
  MEM_chg_rec (cbuf, &lTot, sNew, lNew, cp1, lExpr);
    // printf(" _eval_subst-f-chg_rec |%s|\n",cbuf);

  // check again ..
  ++iNr;
  goto L_next;

}


//================================================================
  int PRG_dec_dbg (char *cBuf) {
//================================================================
// 0=off; 1=On.

  int   irc;

  UTX_pos_skipLeadBlk(cBuf);

  irc = UTX_ck_casenChr(cBuf, "ON", 2);  // 0=equal
 
  if(irc == 0) PRG_dbg = 1;
  else         PRG_dbg = 0;

  if(PRG_dbg == 0) {
    TX_Print("DEBUG OFF");
  }

    // printf("ex PRG_dec_dbg %d\n",PRG_dbg);

  return 0;

}


//================================================================
  int PRG_start () {
//================================================================
// (re)run application <AP_dir_prg>/<APP_act_nam>.gcap
// Wird von PRG_CB (selection of program in List) gerufen.
// Wird auch vom Update-Button (UCB1) gerufen.
// Wird vom core via Keyb-CB von Ctrl-P gerufen.
// see also ED_Run
 
  static int     lNr, delay;

  int     irc, i1, bufSiz, plNr, iCod, iTyp;
  long    iInd;
  char    *cbuf, *lp1, *lp2, *cp1, s1[400];


  // printf("================================= \n");
  PRG_get_prgDir ();

  // printf("PRG_start stat=%d |%s|%s|\n",PRG_stat,AP_dir_prg,APP_act_nam);

  if(IE_cad_exitFunc()) return -1;


  // lineBuffer fuer programLines
  cbuf = mem_cbuf1;
  bufSiz = mem_cbuf1_SIZ;

  PRG_dbg = 0;
  WC_set_obj_stat (0);  // 0=perm


  
  //----------------------------------------------------------------
  // primary start or rerun ?
  if(PRG_stat != 0) goto L_reRun;

  // primary-start;
  // if CAD-menu is open: close it ..
  if(UI_get_InpMode() == UI_MODE_CAD) {
   IE_cad_exitFunc ();
  }

  // init PRG
  PRG_ED_stat = ED_get_mode();
  ED_enter ();  // display also temp.objects


  // get mem for FormTab or clear FormTab
  if(FormTab.data == NULL)
    MemTab_ini__ (&FormTab, sizeof(FormVar), 0, 100);
  // else
    // FormTab.rNr = 0;
    // printf(" FormTab.rMax=%d\n",FormTab.rMax);


  // get mem for PRG_internTab or clear PRG_internTab
  if(PRG_internTab.data == NULL)
    // MemTab_ini__ (&PRG_internTab, sizeof(ObjRange), Typ_ObjRange, 100);
    MemTab_ini__ (&PRG_internTab, sizeof(IndTab), Typ_IndTab, 100);
  // else
    // PRG_internTab.rNr = 0;
    // printf(" PRG_internTab.rMax=%d\n",PRG_internTab.rMax);


  // get mem for DLG-text
  // UtxTab_clear (&PRG_txt);



  //----------------------------------------------------------------
  // do primary start
  // create filename
  // get full-filename from AP_dir_prg
  strcpy(s1, AP_dir_prg);
  UTX_fdir_add_del (s1);          // add '/'
  MDLFN_ffNam_fNam (cbuf, s1);
  UTX_fdir_add_del (cbuf);
  strcat(cbuf, APP_act_nam);
  strcat(cbuf, ".gcap");
  // sprintf(cbuf, "%s%s.gcap",AP_dir_prg,APP_act_nam);
    // printf(" PRG_start-L1-|%s|\n",cbuf);


  // check if prg exists
  if(OS_checkFilExist(cbuf,1) == 0) {
    // TX_Print("***** programfile %d does not exist *****",cbuf);
    MSG_pri_1 ("NOEX_fil", "%s", cbuf);
    AP_stat.APP_stat = 0;  // reset prg-is-active
    PRG_stat = 0;
    return -1;
  }


  APP_act_typ = 1;
  UI_Set_typPrg ();
  UI_Set_actPrg (APP_act_nam, 2);  // display programName red

  TX_Print(" start program %s ..",APP_act_nam);



  //----------------------------------------------------------------
  // load program into UTF_FilBuf1
  UTF_clear1 ();
  UTF_add1_file (cbuf);


  TX_Print("Right Mousebutton to update form;"
           " Key Esc/Tab to change form-Input-field");


  delay = ED_get_delay();            // get delay-time
  PRG_lNr = ED_get_lnr_act() -1;     // get active line-Nr
  PRG_dli = GL_GetActInd() + 1;      // get active DL-index
  // DB_set_state (0);                  // save all DB-pointers
    // printf(" delay=%d lNr=%d PRG_dli=%ld\n",delay,lNr,PRG_dli);
  

  // UNDO_app__ (0);            // init undo (get act.lNr)

  // Create Form & FormTab;
  // loop tru program; find & create widgets
  PRG_win__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncInit));



  //================================================================
  L_reRun:
  //------------------
    // TESTONLY:
    // PRG_dumpRec ("L_reRun: ----------------------------");


  PRG_stat = 2;  // running; Update is active


  // // Find/Create Definitionline for variables
  // PRG_vars_ini ();


  // alle Objekte ab dli loeschen
  GL_Delete (PRG_dli);


  // alle neu erzeugten DB-objekte loeschen
  DB_set_state (1);


  // open outfile PRG_fp
  sprintf(cbuf, "%sPRG_ACT.gcad",AP_get_tmp_dir());
    // printf(" outfile |%s|\n",cbuf);
  if((PRG_fp=fopen(cbuf,"w")) == NULL) {
    TX_Print("PRG_start E001 %s",cbuf);
    goto L_err1;    // return -1;
  }

  fprintf(PRG_fp, "# PRGBEG %s\n", APP_act_nam);


  // loop tru program; read & execute all lines; skip DLG-lines.
  // rewind (PRG_fp);
  plNr = 0;                         // programLineNr; first=1
  lNr = PRG_lNr;

  // get startPos of buffer
  lp2 = UTF_get1_nxtLnPos (NULL);




  //----------------------------------------------------------------
  L_nxt_ln:
      // printf("PRG_start L_nxt_ln: xxxxxxxxxxxxxxxxxxxxx\n");
    lp1 = lp2;
    if(lp2 == NULL) goto L_done;
    lp2 = UTF_get1_nxtLnPos (lp1);

    ++plNr;

    // give chance to terminate a loop
    if(PRG_dbg == 0) {
      i1 = UI_askEscape();
      if(i1 < 0) goto L_done;
    }

    // read next line, copy line 
    ++lNr;
    UTX_cpLn_2cp (cbuf, lp1, lp2, bufSiz);
    UTX_CleanCR (cbuf);
    // APED_onam_cut (cbuf);     // remove name
    // skip empty lines
    if(cbuf[0] == '\0') goto L_nxt_ln;
    // skip comment-lines; copy "##"-Lines
    if(cbuf[0] == '#') {
      if(cbuf[1] == '#') {++cbuf; goto L_copyCopy;}
      goto L_nxt_ln;
    }
      // printf(" _start-nxt-Ln %d |%s|\n",plNr,cbuf);


    if(!strncmp(cbuf, "EXIT" , 4)) {
      if(PRG_dbg) TX_Print ("END OF PROGRAM ..");
      goto L_done;
    }

    if(!strncmp(cbuf, "DEBUG" , 5)) {
      cp1 = cbuf + 5;
      PRG_dec_dbg (cp1);
      goto L_nxt_ln;
    }

    if(PRG_dbg != 0) {
      TX_Print(cbuf);
    }

    // skip Labels
    if(cbuf[0] == ':') goto L_nxt_ln;

    // skip DLG-lines;
    if(!strncmp(cbuf, "DLG" , 3)) goto L_nxt_ln;
    if(!strncmp(cbuf, "INTERN" , 6)) goto L_nxt_ln;


    // skip PRI-lines if exiting ..
    if(PRG_mode == 2) {  // if exiting
      if(!strncmp(cbuf, "PRI " , 4)) goto L_nxt_ln;
    }


    // extract objectName -> AP_ED_oNam        2016-10-07
    // cut objectName
    APED_onam_cut (cbuf, NULL);


    // evaluate line (change <V#> into value of V#; eg "P<V1>=" -> "P3=")
    irc = PRG_eval_subst (cbuf);
    if(irc < 0) {TX_Print("PRG_start E002 %s",cbuf); goto L_err1;}  // return -1;
    if(irc > 0) {
      if(PRG_dbg != 0) {
        TX_Print(cbuf);
      }
    }
      // printf(" _start-nxt-f.eval_subst |%s|\n",cbuf);


    if(UTX_ck_casenChr(cbuf, "JUMP" , 4) == 0) {
      cp1 = cbuf + 4;
      UTX_pos_skipLeadBlk (cp1);
      goto L_jump;
    }

    // im debug-Mode: wait for Esc-Key;
    if(PRG_dbg != 0) {
      TX_Print("- stop in progLn. %d; continue with Escape",plNr);
      // Definition-Line: dump obj ?
      DL_Redraw ();
      UI_wait_Esc__ ();
    }

    // handle IF-lines
    if(UTX_ck_casenChr(cbuf, "IF" , 2) == 0) {
      irc = PRG_eval_expr (&cp1, cbuf);
      // -1= Error.
      // 0=skip command; continue
      // 1=command (eg "V1=10"), continue
      // 2=execute jump
      if(irc < 0) goto L_err1;    // return -1;
      if(irc == 0) goto L_nxt_ln;
      if(irc == 2) goto L_jump;
    }

    // is it a definitionLine of a Variable of Formtab ?
    iCod = APED_dec_defLn (&cp1, &iTyp, &iInd, cbuf);
    if(iCod == 0) {
      // 0=this is a definitionLine
      // lines "V=NEW()" sofort auswerten, nicht via WC_Work1;
      PRG_dec_defLn (cp1);
      // Yes: save LineNr, update Form.
      PRG_def_ini (iTyp, iInd, cp1, plNr);
    }


    // execute line
      // printf("WWWWWWWWWWWW _work1 %d |%s|\n",lNr,cbuf);
    ED_enter ();  // display also temp.objects
    i1 = WC_Work1 (lNr, cbuf);

    // check for error:
    if(AP_errStat_get() != 0) {
      TX_Print("***** Error in line |%s|",cbuf);
      goto L_done;
    }


    // copy line -> outFile
    // is defLine activ: no=copy.
    if(iCod != 0) goto L_copyCopy;

    // yes,defLn; test if obj is internal.
    irc = MemTab_IndTab_ck (&PRG_internTab, iTyp, iInd);
    if(irc == 0) goto L_copyDone;


    L_copyCopy:
        // printf(" _start-nxt-out |%s|\n",cbuf);
      fprintf(PRG_fp, "%s\n",cbuf);
  
  
    L_copyDone:
      if(delay != 0) {OS_Wait (delay);}
      goto L_nxt_ln;



  //----------------------------------------------------------------
  L_jump:      // continue with line after label cp1
    lp2 = UTF_get1_nxtLnPos (NULL);    // rewind
    i1 = strlen(cp1);

  L_jmp_nxt:
    lp1 = lp2;
    lp2 = UTF_get1_nxtLnPos (lp1);
    if(lp2 == NULL) {TX_Error("cannot find label |%s|",cp1); goto L_err1;}
    if(lp2[0] != ':') goto L_jmp_nxt;
    if(strncmp(&lp2[1], cp1, i1)) goto L_jmp_nxt;
    // label found ..
    goto L_nxt_ln;




  //----------------------------------------------------------------
  L_done:
  irc = 0;

  L_exit:
  if(PRG_fp) {
    fprintf(PRG_fp, "# PRGEND %s\n", APP_act_nam);
    fclose (PRG_fp);
  }
  DL_Redraw ();                      // display
  PRG_stat = 1;                      // active ..
  WC_set_obj_stat (1);               // 1=workmode

/*
    printf("=========================== \n");
    system("cat tmp/PRG_ACT.gcad");                 // ACHTUNG: nur Unix.
    printf("=========================== \n");
*/

    // printf("ex PRG_start %d\n",irc);
  return irc;


  //----------------------------------------------------------------
  L_err1:
    irc = -1;
    goto L_exit;

}


//================================================================
  int PRG_CB (char *fnam) {
//================================================================
// CB from List-selection
// OFFEN:
// im CB sollte auch immer das symbol geliefert werden !

  char cbuf[128];


  // printf("PRG_CB |%s|%s|\n",fnam,AP_dir_prg);


  if(fnam == NULL) return 0;  // cancel


  // no addOn-prog may be active.
  if(AP_stat.APP_stat != 0) {
    TX_Print ("***** disactivate active application / plugin ..");
    return -1;
  }


  // set APP_act_nam, disp prgNam
  strcpy(cbuf, fnam);
  UTX_ftyp_cut (cbuf);
  strcpy(APP_act_nam, cbuf);
  UI_Set_actPrg (APP_act_nam, 2);
  AP_stat.APP_stat = 1;


  // kill already active program
  PRG_win__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB2));

  PRG_start ();

    // printf("ex PRG_CB |%s|%s|\n",AP_dir_prg,APP_act_nam);

  return 0;

}


//================================================================
   int PRG_Loa () {
//================================================================
// PRG_Loa                make/display List of all available *.gcap application
// Add selected Application to list app.lst.
// Activate selected Application.

// see APP_Open

  int    irc, i1;
  char   cbuf1[256];

  // printf("PRG_Loa \n");
  // printf(" AP_dir_prg=|%s|\n",AP_dir_prg);


  // no addOn-prog may be active.
  if(AP_stat.APP_stat != 0) {
    TX_Print ("***** disactivate active application / plugin ..");
    return -1;
  }

  if(IE_cad_exitFunc()) return -1;


/*
  sprintf(cbuf,"%sprgdir.lst",AP_get_tmp_dir());
  GUI_List2 ("select program",
           AP_dir_prg,           // Pfadname des activeDirectory
           cbuf,                 // Liste der directories
           (void*)PRG_CB);       // CallBack der Liste
*/
  // appNam
  cbuf1[0] = '\0';

//   // get cbuf2 = ffNam of AP_dir_prg
//   MDLFN_ffNam_fNam (cbuf2, AP_dir_prg);
//   // strcpy(cbuf2, AP_dir_prg);

  // get absolute-directory for symbol APPLI
  irc = MDLFN_fDir_syFn (AP_dir_prg, "APPLI");
  if(irc < 0) {
    irc = MDLFN_fDir_syFn (AP_dir_prg, "Data");
    if(irc < 0) {TX_Error("PRG_Loa E1"); return -1;}
#ifdef _MSC_VER
    strcat(AP_dir_prg, "prg\\");
#else
    strcat(AP_dir_prg, "prg/");
#endif
  }


  // Liste mit Dir-Auswahl
  i1 = AP_fnam_get_user_1 (2, cbuf1, AP_dir_prg, "select program", "*.gcap");
  if(i1 < 0) return -1;

  return PRG_CB (cbuf1);

}


//================================================================
  int PRG_dlg__ () {
//================================================================
// Declarations:
// fill FormTab (DLG) and PRG_internTab (INTERN)

  int       i1, bufSiz, vTyp, rNr;
  long      sInd, vInd;
  char      *cbuf, cmd[4], *cp1, *cp2, *cp3, *lp1, *lp2,
            cWd[128], cTmp[128], cPre[80], cDel;
  double    d1;
  MemObj    w1;


  // printf("PRG_dlg__ \n");

  cbuf = mem_cbuf1;
  bufSiz = mem_cbuf1_SIZ;

  lp2 = UTF_get1_nxtLnPos (NULL);     // get startPos of buffer

  // clear all panel-records
  FormTab.rNr = 0;

  vInd = 0;


  L_nxt_ln:
    lp1 = lp2;
    if(lp2 == NULL) goto L_done;
    lp2 = UTF_get1_nxtLnPos (lp1);
      // printf(" _dlg__ |");UTX_dump_cnl(lp1,50);printf("|\n");


    // check for INTERN
    if(!strncmp(lp1, "INTERN", 6)) {
      cp1 = lp1 + 6;
      UTX_cpLn_2cp (cbuf, cp1, lp2, 512);
      UTX_CleanCR (cbuf);                              // remove foll CR,LF ..
      ED_addRange (&PRG_internTab, cbuf);
      goto L_nxt_ln;
    }

    // skip all but DLG
    if(strncmp(lp1, "DLG", 3)) goto L_nxt_ln;

    // copy Line -> cbuf
    UTX_cpLn_2cp (cbuf, lp1, lp2, 512);
    UTX_CleanCR (cbuf);                              // remove foll CR,LF ..


    // cmd = next 3 chars
    strncpy (cmd, &cbuf[4], 3);
    cmd[3] = '\0';

    // cp1 = skip first 8 chars ("DLG cmd ")
    cp1 = &cbuf[8];
      // printf(" cmd:|%s| pars:|%s|\n",cmd,cp1);

    
    //----------------------------------------------------------------
    // --- = ruler
    if(!strcmp(cmd, "---")) {
      GUI_sep_h  (&PRG_box, 3);
      goto L_nxt_ln;
    }


    //----------------------------------------------------------------
    // TXT = Label
    if(!strcmp(cmd, "TXT")) {
      GUI_label__ (&PRG_box, cp1,  "l");
      goto L_nxt_ln;
    }


    //----------------------------------------------------------------
    // RDB = Radiobuttons
    if(!strcmp(cmd, "RDB")) {
        printf(" ------RDB- %s\n",cp1);

      // in: cp1; out: cp2=ptr_nxtWd; cWd=Copy_of_Wd
      i1 = UTX_get_word (&cp1, &cp2, cWd, &d1, &cDel);
        // printf(" i1=%d cWd=|%s| cp2=|%s| cDel=%d\n",i1,cWd,cp2,cDel);

      // decode objName cWd (must be variable for output)
      APED_dbo_oid (&vTyp, &sInd, cWd);
        // printf(" vTyp=%d vInd=%ld\n",vTyp,vInd);
      if(vTyp != Typ_VAR) goto L_parErr1;

      // cp2 = title; remove "
      cp2 = UTX_CleanBracks (cp2, '"', '"');

      rNr = FormTab.rNr;
        // printf(" _RDB: rNr=%d vInd=%ld sInd=%ld\n",rNr,vInd,sInd);
      if(vInd == sInd) {
        w1 = GUI_radiobutt__ (&PRG_box, cp2, 1, NULL, NULL, "");
      } else {
        w1 = GUI_radiobutt__ (&PRG_box, cp2, 0, NULL, NULL, "");
      }
      vInd = sInd;  // vInd=Index of variable for output of Checkbox.
      // add record to FormTab
      PRG_addRec (RDBTYP, vInd, Typ_VAR);
      PRG_addWp (rNr, &w1);       // add widget-pointer to last record
      goto L_nxt_ln;
    }


    //----------------------------------------------------------------
    // CKB = Checkbox
    if(!strcmp(cmd, "CKB")) {
        // printf(" ------CKB- %s\n",cp1);

      // in: cp1; out: cp2=ptr_nxtWd; cWd=Copy_of_Wd
      i1 = UTX_get_word (&cp1, &cp2, cWd, &d1, &cDel);
        // printf(" i1=%d cWd=|%s| cp2=|%s| cDel=%d\n",i1,cWd,cp2,cDel);

      // decode objName cWd (must be variable for output)
      APED_dbo_oid (&vTyp, &sInd, cWd);
        // printf(" vTyp=%d vInd=%ld\n",vTyp,vInd);
      if(vTyp != Typ_VAR) goto L_parErr1;

      // cp2 = title; remove "
      cp2 = UTX_CleanBracks (cp2, '"', '"');

      rNr = FormTab.rNr;
        // printf(" _CKB: rNr=%d vInd=%ld sInd=%ld\n",rNr,vInd,sInd);
      w1 = GUI_ckbutt__ (&PRG_box, cp2, 0, NULL, NULL, "");
      vInd = sInd;  // vInd=Index of variable for output of Checkbox.
      // add record to FormTab
      PRG_addRec (CKBTYP, vInd, Typ_VAR);
      PRG_addWp (rNr, &w1);       // add widget-pointer to last record
      goto L_nxt_ln;
    }


    //----------------------------------------------------------------
    // check for object-input
    // DLG V2 " X-Coord - " 150
    // DLG P3 " Origin  - " 150
    // DLG D4 " Direction " 150
    // in: cp1; out: cp2=ptr_nxtWd; cWd=Copy_of_Wd
    cp2 = &cbuf[4];
    i1 = UTX_get_word (&cp2, &cp1, cWd, &d1, &cDel);
      // printf(" i1=%d cWd=|%s| cp1=|%s| cDel=%d\n",i1,cWd,cp2,cDel);

    // decode output-obj
    i1 = APED_dbo_oid (&vTyp, &vInd, cWd);
      // printf(" _decode_obj irc=%d vTyp=%d vInd=%d |%s|\n",i1,vTyp,vInd,cWd);
    if(i1 != 0) goto L_E001;

    // decode InfoText // cp2 = title; remove "
    i1 = UTX_get_word (&cp1, &cp2, cWd, &d1, &cDel);
      // printf(" i1=%d cWd=|%s| cp2=|%s| cDel=%d\n",i1,cWd,cp2,cDel);
    cp1 = UTX_CleanBracks (cWd, '"', '"');
      // printf(" InfoText=|%s|\n",cp1);


    // decode FieldLenght
    i1 = UTX_get_word (&cp2, &cp3, cTmp, &d1, &cDel);
      // printf(" i1=%d cWd=|%s| cp2=|%s| cDel=%d\n",i1,cTmp,cp2,cDel);
    i1 = -d1;  // size in pixels
      // printf(" FieldLenght=%d\n",i1);


    rNr = FormTab.rNr;
    w1 = GUI_entry__ (&PRG_box, cp1, NULL, PRG_ent_CB, NULL, "r,20");//i1);
    // CB nicht via GUI_EntryL - sonst wird keybCB auch umgeleitet !
/*
      // printf("  vor focus-in-event\n");
    g_signal_connect (G_OBJECT(w1), "focus-in-event",
                      G_CALLBACK(PRG_ent_CB), NULL);
*/
    PRG_addRec (INPTYP, vInd, vTyp, vInd);
    PRG_addWp (rNr, &w1);       // add widget-pointer to last record

    goto L_nxt_ln;




  L_done:
      // DEB_dump_IndTab (&PRG_internTab);
      // printf(" ex PRG_dlg__\n");
    return 0;


  L_parErr1:
    TX_Error("PRG_input parameterError %s",cWd);
    return -1;

  L_E001:
    TX_Error("PRG_input unknown record %s",cmd);
    return -1;


}


//================================================================
  int PRG_addRec (int rTyp, long sInd, int vTyp) {
//================================================================
// add record
// rTyp   recordTyp; 1=Checkbox; 2=Inputfield
// vTyp   type of outputvar; 1=VAR, 2=PT, 3=LN ..
// vInd   DB-Index of outputvar.

  long     ld;
  FormVar  fRec1;


    // printf("PRG_addRec %d %ld %d %ld\n",rTyp,sInd,vTyp,vInd);


  fRec1.rTyp  = rTyp;
  fRec1.sInd  = sInd;
  fRec1.vTyp  = vTyp;
  fRec1.slNr   = -1;
  fRec1.iStat = 0;

  MemTab_sav (&FormTab, &ld, &fRec1, 1);
  if(FormTab.data == NULL) goto L_EOM;


  return 0;

  L_EOM:
    TX_Error("PRG_addRec EOM");
    return -1;

}


//================================================================
  int PRG_addWp (int rNr, MemObj *w1) {
//================================================================
// add widget-pointer to last record

  int   i1;


  // printf("PRG_addWp %d\n",rNr);

  // fuer alle records ab incl recNr rNr widgetPointer setzen
  for(i1=rNr; i1<FormTab.rNr; ++i1) {
    FormTab.data[i1].wp = *w1;
  }

  return 0;

}


/*
//================================================================
  int PRG_vars_ini () {
//================================================================
// Find & load Startvalues for the FormTab-Variables


  int       i1, ii, iNr;
  double    d1, *dp1;
  char      *cp1;
  FormVar   *vf1;


  printf("PRG_vars_ini %d\n",FormTab.rNr);




  //----------------------------------------------------------------
  // preset inputFields
  for(i1=0; i1<FormTab.rNr; ++i1) {
    if(FormTab.data[i1].rTyp != INPTYP) continue;
    // // get ModelCode of Line FormTab.data[i1].slNr
    // UTF_GetLinNr (mem_cbuf1, &i2, FormTab.data[i1].slNr);
// 
    // // strip Header
    // cp1 = strchr(mem_cbuf1, '=');
    // if(cp1 == NULL) continue;
    // ++cp1; // skip '='
// 
    // // copy -> inputField
    // GUI_Entry_set (FormTab.data[i1].wp, cp1);
  }





  //----------------------------------------------------------------
  // preset checkboxes
    PRG_dumpRec ();

    ii = 0;

  // loop tru all checkbox-chains;
  L_cbc:
    if(ii >= FormTab.rNr) goto L_cbc9;
    vf1 = &FormTab.data[ii];
    if(vf1->rTyp != CKBTYP) {++ii; goto L_cbc;}

    // get actual value of sInd; 1=first.
    dp1 = DB_get_Var (vf1->sInd);
    if(*dp1 == UT_VAL_MAX) {
      i1 = 1;                         // undefined !
    } else {
      i1 = *dp1;
    }
      printf(" preset ii=%d sInd=%d val=%d\n",ii,vf1->sInd,i1);

    // get lenght of this CheckboxChain;
    iNr = PRG_ckb_len (ii);

    if(i1 > iNr) i1 = iNr;   // fix maxInd
    --i1;  // make index from Nr.

    // activate
      printf(" ckb-activate %d\n",ii+i1);
    GUI_CkButt_set (FormTab.data[ii+i1].wp, 1);
    FormTab.data[ii+i1].iStat = 1;

    // proceed
    ii += iNr;
    goto L_cbc;

  L_cbc9: //done




  return 0;

}
*/


//================================================================
  int PRG_def_ini (int typ, long ind, char *lBuf, int lNr) {
//================================================================
// testen, ob in FormTab definiert;
// wenn ja: sofort im Dialog setzen und die Zeile merken.

  int     i1, ii;
  double  d1;
  char    *cp1;


  // printf("IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII \n");
  // printf("PRG_def_ini typ=%d ind=%ld lNr=%d |%s|\n",typ,ind,lNr,lBuf);
  // PRG_dumpRec ();


  // decode type & DB-index
  // APED_dbo_oid (&typ, &ind, lBuf);
    // printf(" typ=%d ind=%d\n",typ,ind);


  // test if obj is used in FormTab
  for(i1=0; i1<FormTab.rNr; ++i1) {
    if(FormTab.data[i1].vTyp != typ) continue;
    if(FormTab.data[i1].sInd != ind) continue;
    ii = i1;
    goto L_used;
  }
  return 0;   // unused; 



  //----------------------------------------------------------------
  // save lineNr for this obj
  L_used:
  FormTab.data[ii].slNr = lNr;
    // printf(" .slNr[%d] = %d\n",ii,lNr);

  // get text of var
  // cp1 = strchr(lBuf, '=');   // strip Header
  // ++cp1;
  cp1 = lBuf;
    // printf(" decode txt |%s|\n",cp1);



  //----------------------------------------------------------------
  // update dialog-window
  if(FormTab.data[i1].rTyp != CKBTYP) goto L_upd_rdb;
  // the variable of the checkbox has been set; activate corresponding checkbox.
  // ii is the FormTab-record using the variable ind for the first time

  // decode value of var -> d1
  APT_obj_expr (&d1, typ, cp1);
  i1 = d1;
    // printf(" CKB-val-i1=%d\n",i1);

  // if(i1 < 0) return 0;
  GUI_ckbutt_set (&(FormTab.data[ii].wp), i1);
  FormTab.data[ii].iStat = i1;
  return 0;



  //----------------------------------------------------------------
  // update dialog-window
  L_upd_rdb:
  if(FormTab.data[i1].rTyp != RDBTYP) goto L_upd_inp;
  // the variable of the checkbox has been set; activate corresponding checkbox.
  // ii is the FormTab-record using the variable ind for the first time

  // decode value of var -> d1
  APT_obj_expr (&d1, typ, cp1);
  i1 = d1;
  --i1;       // make index from Nr.
    // printf(" CKB-val-i1=%d\n",i1);

  // activate corresponding CKB; 1=on, 0=off
  if(i1 < 0) return 0;
  // GUI_ckbutt_set (&(FormTab.data[ii+i1].wp), 1);
  GUI_radiobutt_set (&(FormTab.data[ii+i1].wp));
  FormTab.data[ii+i1].iStat = 1;
  return 0;


  //----------------------------------------------------------------
  L_upd_inp:
  if(FormTab.data[i1].rTyp != INPTYP) {TX_Error("PRG_def_ini E01");return -1;}

  // copy -> inputField
    // printf(" _def_ini %d |%s|\n",ii,cp1);
  GUI_entry_set (&(FormTab.data[ii].wp), cp1);

  return 0;

}
 

//================================================================
  int PRG_update () {
//================================================================
// read data from form; copy data -> prog.
// modify line or create new outputline

  int       ii, i1, iNr, iAct;
  char      *cp1, cbuf[320], auxBuf[256];
  FormVar   *vf1;


  // printf("PRGUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU \n");
  // printf("PRG_update rNr=%d\n",FormTab.rNr);
  // PRG_dumpRec ("_update");
  // UTF_dump1__ ();

// was DLG_form2db


  // reset previous error
  if(AP_stat.errStat) AP_errStat_reset (0);


  // loop tru all records with index ii; vf1 as active record.
  ii = 0;
  L_nxt_rec:
    if(ii >= FormTab.rNr) goto L_exit;

    vf1 = &FormTab.data[ii];
      // printf("------- ii=%d rTyp=%d\n",ii,vf1->rTyp);



    //----Checkbox----------------------------
    if(vf1->rTyp != CKBTYP) goto L_RDB;
     // Checkbox or chain of checkboxes. Handle whole chain.
      // ii=first record; get lenght of this CheckboxChain;

      // test if CheckboxChain has changed and which CKB is active.
      // iAct=-1: unchanged; iAct=0=FirstLineNotActive; 1=firstLineActive ..
      i1 = GUI_ckbutt_get (&(FormTab.data[ii].wp));  // 0=not sel, 1=sel
        // printf(" _CKB[%d]=%d\n",ii,i1);
      if(FormTab.data[ii].iStat == i1) goto L_ckb_done;  // unchanged

      // modify sourceline vf1->slNr
      sprintf(cbuf, "V%ld=%d",vf1->sInd,i1);
      cp1 = UTF_get1_LnPos (vf1->slNr);
      UTF_chg1_Ln_cpCs (cp1, cbuf);

      L_ckb_done:
      ++ii;
      goto L_nxt_rec;



    //----Radiobox----------------------------
    L_RDB:
    if(vf1->rTyp != RDBTYP) goto L_INP;
     // Checkbox or chain of checkboxes. Handle whole chain.
      // ii=first record; get lenght of this CheckboxChain;
      iNr = PRG_ckb_len (ii);
        // printf(" _RDB[%d] iNr=%d\n",ii,iNr);


      // test if CheckboxChain has changed and which CKB is active.
      // iAct=-1: unchanged; iAct=0=FirstLineNotActive; 1=firstLineActive ..
      iAct = PRG_ckb_mod (ii, iNr);
      if(iAct < 0) goto L_cb9;  // unchanged

      // gesamte CheckboxChain off setzen;
      if(iAct >= 0) {
        for(i1=ii; i1<ii+iNr; ++i1) FormTab.data[ii+iNr].iStat = 0;
      }

      // Checkbox <iAct> aktiv setzen
      if(iAct >= 0) FormTab.data[ii+iAct-1].iStat = 1;

      // ModelCode aktualisieren
      // create modified lineText
      sprintf(cbuf, "V%ld=%d",vf1->sInd,iAct);
        // printf(" write %d |%s|\n",ii,cbuf);

      // modify sourceline vf1->slNr
      cp1 = UTF_get1_LnPos (vf1->slNr);
      UTF_chg1_Ln_cpCs (cp1, cbuf);


      L_cb9:
      ii += iNr;
      goto L_nxt_rec;





    //----Input----------------------------
    L_INP:
    if(vf1->rTyp != INPTYP) {TX_Error("PRG_update E01");return -1;}
    cp1 = GUI_entry_get (&(vf1->wp));
    strcpy(auxBuf, cp1);
    UTX_CleanCR (auxBuf);
      // printf(" _update: %d |%s|\n",ii,auxBuf);

    // create outputline
    if(vf1->vTyp == Typ_VAR) {
      sprintf(cbuf, "V%ld=%s",vf1->sInd,auxBuf);

    } else if(vf1->vTyp == Typ_PT) {
      sprintf(cbuf, "P%ld=%s",vf1->sInd,auxBuf);

    } else if(vf1->vTyp == Typ_VC) {
      sprintf(cbuf, "D%ld=%s",vf1->sInd,auxBuf);

    } else {
      printf("****** PRG_update E2 ********\n");
    }
      // printf(" _update slNr=%ld typ=%d |%s|\n",vf1->slNr,vf1->vTyp,cbuf);

    // modify source
    // APED_onam_cut (cbuf);     // remove name
    // find startpos of line in UTF_FilBuf1
    cp1 = UTF_get1_LnPos (vf1->slNr);
    // change line starting at cp1 in UTF_FilBuf1
    UTF_chg1_Ln_cpCs (cp1, cbuf);

    ++ii;
    goto L_nxt_rec;




  //----------------------------------------------------------------
  L_exit:

    // printf("ex PRG_update rNr=%d\n",FormTab.rNr);
    // UTF_dump1__ ();

  return 0;

}


//================================================================
   int PRG_Ed () {
//================================================================
// Edit active program.

  int    irc;
  char   cbuf[SIZFNam], s1[SIZMFSym];


  PRG_get_prgDir ();

  // printf("PRG_Ed %d\n",APP_act_typ);

  if(IE_cad_exitFunc()) return -1;

  if(APP_act_typ != 1) {
    TX_Print("*** no application is active ..");
    return -1;
  }


  // sprintf(s1, "%sgcap",AP_dir_prg,fnam_del,APP_act_nam);
  // irc = MDLFN_fDir_syDir (s1, AP_dir_prg);
  // if(irc < 0) return -1;
  sprintf(cbuf, "%s%c%s.gcap",AP_dir_prg,fnam_del,APP_act_nam);
  return APP_edit (cbuf, 1);  // do not wait


  // // modify File m. Editor
  // sprintf(cbuf, "%s %s%s.gcap",OS_get_edi(),AP_dir_prg,APP_act_nam);
  // OS_exec (cbuf);   // execute, do not wait for completion
    // printf(" edit |%s|\n",cbuf);
  // return 0;

}


//================================================================
  int PRG_ckb_len (int iSta) {
//================================================================
//  get lenght of this CheckboxChain;
// Input:
//   iSta    index of first CKB-Record of CheckboxChain;
//   retCod  nr of following CKB-records with same outputVar sInd

  int    ii, sInd;


  ii = iSta;
  sInd = FormTab.data[ii].sInd;

  L0_nxtRec:
    ++ii;
    if(ii >= FormTab.rNr) goto L0_exit;
    if(FormTab.data[ii].rTyp != RDBTYP) goto L0_exit;
    if(FormTab.data[ii].sInd != sInd) goto L0_exit;
    goto L0_nxtRec;

  L0_exit:
  ii -= iSta;
    // printf("ex PRG_ckb_len %d iSta=%d\n",ii,iSta);
  return ii;

}


//================================================================
  int PRG_ckb_mod (int iSta, int iNr) {
//================================================================
// test if CheckboxChain has changed and which CKB is active.
// IN: iSta=first ckb-record; iNr=nr of following ckb's
// iAct=-1: unchanged; iAct=0=FirstLineNotActive; 1=firstLineActive ..


  int    iAct, i1, i2;


  // printf("PRG_ckb_mod iSta=%d iNr=%d\n",iSta,iNr);
  // printf(" iStat=%d\n",FormTab.data[iSta].iStat);



  //----------------------------------------------------------------
  // iAct = find which Checkbox is active
  iAct = 0;

  for(i1=0; i1<iNr; ++i1) {
    i2 = GUI_ckbutt_get (&(FormTab.data[iSta+i1].wp));  // 0=not sel, 1=selected.
      // printf(" ckb[%d]=%d\n",iSta+i1,i2);
    if(i2 != 0) iAct = i1 + 1;
  }
    // printf(" iAct=%d\n",iAct);


  //----------------------------------------------------------------
  // test if Checkbox iAct has changed; if not: iAct=-1
  if(iAct == 0) {  // one CKB only and OFF
    if(FormTab.data[iSta].iStat == 0) iAct = -1;

  } else {
    if(FormTab.data[iSta+iAct-1].iStat == 1) iAct = -1;
  }


    // printf("ex PRG_ckb_mod %d\n",iAct);
  return iAct;

}


//================================================================
  int PRG_intern_exit () {
//================================================================
/// PRG_intern_exit            kill all internal vars

  int      i1;
  long     dbi, ie;
  IndTab   *or;


  if(PRG_internTab.data == NULL) return 0;

  for(i1=0; i1<PRG_internTab.rNr; ++i1) {
    or = &PRG_internTab.data[i1];
      // printf(" or[%d]=%ld %d %d\n",i1,or->ind,or->typi,or->oNr);
    ie = or->ibeg + or->iNr;
    for(dbi=or->ibeg; dbi<ie; ++dbi) {
      DB_Delete (or->typi, dbi);
    }

  }

  return 0;

}


//================================================================
  int PRG_dumpRec (char *sInf) {
//================================================================
// dump FormTab

  int   i1;


  printf("PRG_dumpRec %d     %s\n",FormTab.rNr,sInf);


  for(i1=0; i1<FormTab.rNr; ++i1) {
    printf(" FT[%d].rTyp=%d sInd=%ld vTyp=%d slNr=%ld iStat=%d\n",
             i1,
             FormTab.data[i1].rTyp,
             FormTab.data[i1].sInd,
             FormTab.data[i1].vTyp,
             FormTab.data[i1].slNr,
             FormTab.data[i1].iStat);
  }


  return 0;

}


//================================================================
  int IO_wri_wri (char *lBuf) {
//================================================================
// write into outputfile IO_fp
// RetCod:
//    1    Line written
//    0    Line written & File closed.

  int     i1, iNr;
  char    cmd[4], cbuf[512], *cp1;
  ObjAto  ato1;

  // printf("IO_wri_ope |%s|\n",lBuf);


  //----------------------------------------------------------------
  if(!strcmp(lBuf, "ENDWRITE")) {

    // close exportfile
    if(IO_fp) {fclose(IO_fp); IO_fp = NULL;}
/*
    // MAN: WRITE soll interaktiv sofort weiterlaufen bis ENDWRITE ....
    if(AP_src == AP_SRC_EDI) {                // MAN
      // den Cursor setzen.
      cpos = UTF_GetPosLnr (&ipos, lNr+1);
        // printf(" set cursor %d %c\n",lNr,*cpos);
      ipos = UTF_offset_(cpos);
      UI_AP (UI_FuncSet, UID_Edit_Cpos, (void*)ipos);
    }
*/
    return 0;      // fertig, normal weiter ...
  }



  //----------------------------------------------------------------
  // test for "PRI"; else write > File
  UTX_cp_nchr_2_upper (cmd, lBuf, 3);
  cmd[3] = '\0';


  // write line >> exportfile
  if(strcmp(cmd, "PRI")) {
    fprintf(IO_fp, "%s\n",lBuf);
    goto L_exit;
  }

  // execute PRInt
  cp1 = &lBuf[4];
  // APT_work_PrgCmd (cmd, &cp1);
  // iNr = APT_decode__ (&ato, &cp1);
  ATO_getSpc__ (&ato1);
  iNr = ATO_ato_srcLn__ (&ato1, cp1);
  if(iNr < 0) return -1;
    // for(i1=0;i1<iNr;++i1) printf("[%d] typ=%d tab=%f\n",i1,atyp[i1],atab[i1]);

  //decode
  APT_decode_print (cbuf, cp1, ato1.typ, ato1.val, ato1.nr);

  fprintf(IO_fp, "%s\n",cbuf);


  L_exit:
  return 1;

}
 

//================================================================
  int IO_wri_ope (char *lBuf) {
//================================================================
// open outputfile IO_fp
 
  int    i1;
  char   cbuf[256];


  // printf("IO_wri_ope |%s|\n",lBuf);

  // create filename in /tmp/
  i1 = APT_str2fnam (cbuf, &lBuf[6]);

  // open exportfile
  if(i1 == 1) {   // relativ --> tmp/
    UTX_safeName (cbuf, 0);  // change '. ', do not change '/'
    strcat(cbuf, ".write");
  }
    // printf(" exportfile=|%s|\n",ED_buf1);
  if((IO_fp=fopen(cbuf,"w")) == NULL) {
    TX_Error("Error open file |%s|",cbuf);
    // ED_lnr_act = -1;
    return -1;
  }

  return 0;

}


//=====================================================================
  int APT_decode_print (char *cbuf,
                        char *data, int *atyp, double *atab, int aNr) {
//=====================================================================
/// \code
/// APT_decode_print         sourceObj (text) from atomicObjs; resolv PT,VC,..
/// used for print content of PT, VC, .. with "PRI"
/// eg "x-coord,y-coord,z-coord" from point (no obj-type)
/// data can be NULL for pt,vc ..
/// see also SRC_dbo SRC_src_ato
/// \endcode

  int     i1;
  char    auxBuf[64], *cp1;
  void    *vp1;


  // printf("APT_decode_print %d |%s|\n",aNr,data);


    cbuf[0] = '\0';

    for(i1=0; i1<aNr; ++i1) {
      // printf ("  PRI: [%d] typ=%d tab=%f\n",i1,atyp[i1],atab[i1]);

      if(atyp[i1] == Typ_Val) {
        // sprintf(auxBuf,"% .2f",atab[i1]);
        // strcat (cbuf, auxBuf);
        UTX_add_fl_u (cbuf, atab[i1]);


      } else if(atyp[i1] == Typ_VC) {
          // printf(" pri typ_VC\n");
        vp1 = DB_get_VC ((long)atab[i1]);
        UTX_add_fl_u3 (cbuf,((Vector*)vp1)->dx,
                            ((Vector*)vp1)->dy,
                            ((Vector*)vp1)->dz, ',');


      } else if(atyp[i1] == Typ_PT) {
        vp1 = DB_get_PT ((long)atab[i1]);
        UTX_add_fl_u3 (cbuf,((Point*)vp1)->x,
                            ((Point*)vp1)->y,
                            ((Point*)vp1)->z, ',');


      } else if(atyp[i1] == Typ_Txt) {
        APT_get_Txt (auxBuf, data, atab[i1]);
        strcat (cbuf, auxBuf);


      } else if(atyp[i1] == Typ_String) {
        APT_get_String (auxBuf, data, atab[i1]);
        strcat (cbuf, auxBuf);


      } else {
        TX_Error("APT_decode_print E-%d",atyp[i1]);
        return -1;
      }

    }

  // printf("ex APT_decode_print |%s|\n",cbuf);

  return 0;

}


//=============== EOF ============================================
