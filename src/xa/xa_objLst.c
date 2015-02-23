//  gtk_mcl.c            Search/Name              Reiter Franz.    11.09.2008
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
2012-07-23 ported -> GUI_list2__. RF.
2008-09-11 New. RF.

-----------------------------------------------------
*/
/*!
\file  ../xa/xa_objLst.c
\brief MultiColumnList (Search/Name)
\code
=====================================================
List_functions_start:

UI_mcl_ctrl          control window

UI_mcl__             Mainwindow
UI_mcl_load          fill List
UI_mcl_init_selected put sel.objects into list
UI_mcl_CBL           CB of list-selection; create popup
UI_mcl_CB_popup      CB of popup-selection
UI_mcl_CBB           CB of objType-radioButton
UI_mcl_CB1           CB of list-buttons
UI_mcl_cbFunc        save userCallback-function

UI_mcl_add__         MultiColumnList-Add line
UI_mcl_ck_lNr_exist  test if lNr already exists
UI_mcl_update        raise

UI_mcl_modNam        modify ObjName

UI_mcl_get_win       returns window

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



*/


#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // va_list
#include <math.h>


#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_BitTab.h"           // bit-arrays
#include "../ut/ut_txfil.h"            // UTF_GetPosLnr ..
#include "../ut/ut_txt.h"              // UTX_pos_skipLeadBlk
#include "../ut/ut_gtypes.h"           // AP_src_typ__
#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_txTab.h"            // TxtTab UtxTab

#include "../gui/gui__.h"              // Gtk3

#include "../db/ut_DB.h"               // DB_Get..
#include "../xa/xa_mem.h"              // memspc51, mem_cbuf1, mem_cbuf1
#include "../xa/xa_uid.h"              // UI_MODE_..
#include "../xa/xa_ed_mem.h"           // APED_onam_search
#include "../xa/xa_msg.h"              // Messages

// #include "../gr/ut_UI.h"               // UI_Func..


#define   TRUE 1
#define   FALSE 0



typedef_MemTab(int);


//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// EXTERNAL:
// ../gui/gui_base.c:
extern int UI_fontsizX, UI_fontsizY;

// aus xa_ui.c:
extern int       UI_InpMode;

// ex ../xa/xa_objmenu.c
extern TxtTab MenTab;



//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// LOCAL:
// static GIO_WinTree   mcl_lst;
// static GtkListStore *mcl_list;

static MemObj  mcl_win = GUI_OBJ_NEW;
static MemObj  mcl_dep__;
static MemObj  mcl_txt__;
static MemObj  mcl_cba[13];
static MemObj  mcl_rbP;             // radiobutton "is Parent"
static MemObj  mcl_rbC;             // radiobutton "is Child"

// static GtkTreeIter  mcl_iter;
// static int          (*mcl_func)() = NULL;


static int          mcl_typ = -1;              // the active ObjectTyp
static long         mcl_dbi;                   // DB-index
static char         mcl_ID[32] = "";           // the ID of the active obj
static char         mcl_nam[64] = "";          // the name of the active obj
static long         mcl_rNr;                   // sourceLineNr of selected obj

// static MemTab(int)  mcl_lNrTab = MemTab_empty; // list of lineNrs

static BitTab       (mcl_typTab, 256);

static char         mcl_text[128] = "";        // the Filtertext
static char         mcl_dept[32] = "";         // depending objID
static char         *mcl_wTab[20];             // pointerarray words in mcl_text

static int          mclPopIA[100];   // index to text
static char        *mclPopSA[100];   // pointers to popup-text 

/*
static char         *optLst[]={
                    "copy to model",   // mclPop_copy,
                    "modify name",     // mclPop_modId
                    "edit object",     // mclPop_edit
                    "delete object",   // mclPop_dele
                    "analyze object"}; // mclPop_anal

enum {
  mclPop_copy,  mclPop_modId, mclPop_edit,  mclPop_dele,  mclPop_anal,    //  0-
  mclPop_SIZE
};

static int mcl_pt=Typ_PT,      mcl_ln=Typ_LN,      mcl_ac=Typ_CI,
           mcl_cv=Typ_CV,      mcl_pln=Typ_PLN,    mcl_sur=Typ_SUR,
           mcl_sol=Typ_SOL,    mcl_var=Typ_VAR,    mcl_vc=Typ_VC,
           mcl_txt=Typ_Note,   mcl_act=Typ_Activ,  mcl_tra=Typ_Tra,
           mcl_mod=Typ_Model;
*/

static int mclTab[]={
        Typ_PT,    Typ_LN,    Typ_CI,    Typ_CV,    Typ_PLN,
        Typ_SUR,   Typ_SOL,   Typ_Model, Typ_VAR,   Typ_VC,
        Typ_Note,  Typ_Activ, Typ_Tra,   Typ_Error};


// Protos:
int UI_mcl_CB1 (MemObj *mo, void **data);
int UI_mcl_CBB (MemObj *mo, void **data);
int UI_mcl_CBL (MemObj *mo, void **data);
// int UI_EdKeyPress (void *parent, void *event);



//================================================================
//================================================================

/*
#ifdef OFFLINE


//================================================================
int f_CB (char *cBuf) {
//================================================================

  printf("f_CB |%s|\n",cBuf);

  return 0;

}


//================================================================
int b1_CB (void *parent, void *data) {
//================================================================
// (re)start win

  printf("b1_CB\n");

  // (re)Init win
  UI_mcl__ (NULL, PTR_INT(UI_FuncInit));

  // set callback-function
  UI_mcl_cbFunc (f_CB);

  return 0;

}


//================================================================
int b2_CB (void *parent, void *data) {
//================================================================
// add a line

  printf("b2_CB\n");

  UI_mcl_add ("abc", "def", "ghi");

  return 0;

}

//================================================================
int UI_mcl_cbFunc (void *f_CB) {
//================================================================
// save userCallback-function. Selection is reported as:
// ?

  mcl_func = f_CB;

  return 0;

}


//================================================================
int main (int argc, char *argv[]) {
//================================================================

  printf("xa_objLst\n");

  void *window, *wb1, *box1;

  gtk_init (&argc, &argv);

  //------------------------------------------------
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_signal_connect (GTK_OBJECT(window), "destroy",
                     (GtkSignalFunc)gtk_exit, NULL);

  //------------------------------------------------
  box1 = gtk_vbox_new (FALSE, 1);
  gtk_container_add (GTK_CONTAINER (window), box1);
  gtk_widget_show (box1);

  //------------------------------------------------
  wb1 = gtk_button_new_with_label ("......go 1.......");
  gtk_signal_connect (GTK_OBJECT (wb1), "clicked",
                      GTK_SIGNAL_FUNC(b1_CB), NULL);
  gtk_container_add (GTK_CONTAINER (box1), wb1);
  gtk_widget_show (wb1);


  wb1 = gtk_button_new_with_label ("......go 2.......");
  gtk_signal_connect (GTK_OBJECT (wb1), "clicked",
                      GTK_SIGNAL_FUNC(b2_CB), NULL);
  gtk_container_add (GTK_CONTAINER (box1), wb1);
  gtk_widget_show (wb1);

  gtk_widget_show (window);

  gtk_main ();

}


#endif
*/


//================================================================
  int UI_mcl_ck_obj_exist (char *oid) {
//================================================================
// test if line with obj <oid> already exists in list
// RetCod:
//     0      oid already exists
//    -1      oid not yet exists

  int     i1, ii;
  char    s1[32];


  printf("UI_mcl_ck_obj_exist |%s|\n",oid);

  ii = 0;

  // mode, iCol,
  L_nxt:
    i1 = GUI_tree1_analyz1 (s1, ii, 0, &mcl_win);
    if(i1 < 0) return -1;
  
      printf(" pTab[0]=|%s|\n",s1);
    if(!strcmp(oid, s1)) return 0;

    ++ii;
    goto L_nxt;

}


//==============================================================================
  int UI_mcl_load (int iMod, int iTxt, int iOp1, int iOp2, int iGrp) {
//==============================================================================
// fill List.
// Inputs:
//   mcl_typTab    table of requested objTypes
//   mcl_text      filterText
//   mcl_dept      obj to find childs or parents
//   iMod          0=newGrp; 1=Add; 2=Rem
//   iTxt          0=normal, 1=not (filterText)
//   iOp1          0=ignore case (filterText); 1=not
//   iOp2          0="is child of"; 1="is parent of"
//   iGrp          0, 1=set group

// iStx: 0=yes, -1 = no

  int      irc, i1, typ, ls, iSt1, iSt2;
  long     l1, lNr, lSta, lLen, dbi, dli;
  char     *oTxt, *oSrc, *oNam, *oEnd, actNam[64], oid[32];
  ObjSRC   *osa, *os1;

static MemTab(ObjSRC) parTab = MemTab_empty;


  // printf("==================================================== \n");
  // printf("UI_mcl_load |%s|%s|\n",mcl_text,mcl_dept);
  // printf(" iMod=%d iTxt=%d iOp1=%d iOp2=%d iGrp=%d\n",
         // iMod,iTxt,iOp1,iOp2,iGrp);


  AP_SRC_mem_edi ();  // MAN, modified: copy from Editor -> memory.


  // find childs or find parents ?
  if(mcl_dept[0]) {
    // decode mcl_dept
    i1 = APED_dbo_oid (&typ, &dbi, mcl_dept);
      // printf(" i1=%d typ=%d dbi=%ld\n",i1,typ,dbi);
    if(i1 < 0) return i1;

    if(iOp2 == 0) goto L_chd1;  // find childs
    if(iOp2 == 1) goto L_par1;  // find parents
  }


  
  //================================================================
  // filter with words in mcl_text
  //----------------------------------------------------------------
  MemTab_ini (&parTab, sizeof(ObjSRC), Typ_ObjSRC, 400);

  lNr = 1;     // starting line nr


  // get next line with objName containing all filterwords
  // if wTab is empty, next srcline is returned.
  L_nxt_ln:
  // get next sourceline
  oSrc = UTF_GetPosLnr (&lLen, lNr);
  if(!oSrc) goto L_done;
    // printf(" lNr=%ld ll=%ld\n",lNr,lLen);


  // get lineTyp & objectTyp of sourceObj
  // skip all but definition-lines
  irc = APED_dec_defLn (&oTxt, &typ, &dbi, oSrc);
  if(irc < 0) goto L_cont;
    // printf(" lNr=%ld %d typ=%d dbi=%d\n",lNr,irc,typ,dbi);
    // printf(" oSrc=|\n");UTX_dump_cnl(oSrc, 40);printf("|\n");


  // test if bit for its objType is set ..
  i1 = BitTab_get (mcl_typTab, typ);
    // printf(" typ=%d bit=%d\n",typ,i1);
  if(i1 == 0) goto L_cont;


  // searchText empty: add line
  if(!mcl_wTab[0]) goto L_add_ln;


  // find last position of code / startposition of objName
  oEnd = oSrc + lLen;
  oNam = APED_onam_search (oTxt, oEnd);
  if(!oNam) goto L_cont;   // no name ..
    // printf("lNr %ld |",lNr);UTX_dump_cnl(oNam,60);printf("|\n");

  iSt1 = -1;  // 0=yes, -1 = no

  // filter words
  if(iOp1) {   // 1=ignore case, 0=not
    // ignore case
    // make a uppercase-copy of objName
    UTX_cp_word_term (actNam, oNam, '\n');
    UTX_cp_word_2_upper (actNam, actNam);
      // printf(" actNam=|%s|\n",actNam);
    oEnd = UTX_pos_EOS (actNam);
      // printf(" ianz=%d\n",oEnd - actNam);
    iSt1 = UTX_ck_wTab (actNam, oEnd, mcl_wTab); // (i1 >==0) found
  } else {
    // do not ignore case
    iSt1 = UTX_ck_wTab (oNam, oEnd, mcl_wTab); // (i1 >==0) found
  }

  // fix iSt1 to 0 or -1  (0=yes, else no)
  iSt1 = ILIMm10 (iSt1);                     // -1 | 0

  // invert iSt1 if not is active
  if(iTxt) iSt1 = ICHG0x1 (iSt1);
    // printf(" iTxt=%d iSt1=%d\n",iTxt,iSt1);

  // skip obj if iSt1 != 0
  if(iSt1 != 0) goto L_cont;


/*
  // found a word; if OR skip searching for depObj 
  if(iSt1 >= 0) {
    if(iOp1 == Typ_ope_or) goto L_add_ln;
  }
*/


  //----------------------------------------------------------------
  // add this obj to list ..
  L_add_ln:
    // printf(" L_add_ln: |%s|%s|%s|\n",oSrc, oTxt, oNam);
    // printf(" add %ld %d %ld\n",lNr,typ,dbi);
    // printf(" Ln: |");UTX_dump_cnl(oTxt, 100);printf("|\n");
    // printf(" lLen=%ld diff=%d\n",lLen,oTxt - oSrc);
  
    // printf(" iSt1=%d iOp1=%d iSt2=%d\n",iSt1,iOp1,iSt2);
/*
  if(iOp1 == Typ_ope_or) {
    if((iSt1 != 0) && (iSt2 != 0)) goto L_cont;

  } else {    // Typ_ope_and
    if((iSt1 != 0) || (iSt2 != 0)) goto L_cont;
  }
*/

  // add to group: test if obj already exists in list
  if(iMod == 1) {
    APED_oid_dbo__ (oid, typ, dbi);
    if(!UI_mcl_ck_obj_exist (oid)) goto L_cont;
    // irc = OSRC_find_odb (&parTab, &typ, &dbi); // -1 not, else index
    // if(irc >= 0) goto L_cont;
  }


  // get dispListIndex from sourceLineNr
  l1 = lNr;
  // do not get dli from Typ_VAR,Typ_VC ..
  if((typ == Typ_VAR)||(typ == Typ_VC)) {
    dli = -1L;
  } else {
    irc = DL_Get_dli_lNr (&dli, &l1);
      // printf(" _Get_dli_lNr irc=%d dli=%ld lNr=%ld\n",irc,dli,lNr);
  }

  // add object
  l1 = -1L;
  OSRC_add__ (&parTab, &typ, &dbi, &dli, &lNr, &l1);


  //----------------------------------------------------------------
  L_cont:
    lNr += 1;
      // printf(" nxt lNr %ld\n",lNr);
    goto L_nxt_ln;



  //----------------------------------------------------------------
  L_done:
  goto L_disp_srcTab;



  //================================================================
  L_chd1:       // find childs

  // get all childs (depending objects)
  APED_find_dep__ (&parTab, typ, dbi);
  goto L_disp_srcTab;



  //================================================================
  L_par1:       // find parents

  // get all parents into parTab
  APED_find_par__ (&parTab, typ, dbi);



  //================================================================
  // display parTab
  L_disp_srcTab:
  osa = parTab.data;
    // MemTab_dump (&parTab);

  for(i1=0; i1<parTab.rNr; ++i1) {
    os1 = &osa[i1];
    // add to group: test if obj already exists in list
    if(iMod == 1) {
      APED_oid_dbo__ (oid, os1->typ, os1->dbi);
      if(!UI_mcl_ck_obj_exist (oid)) continue;
    }
    // get position of line os1->lNr
    oSrc = UTF_GetPosLnr (&lLen, os1->lnr);
    // add obj to list
    UI_mcl_add__ (os1->typ, os1->dbi, os1->lnr, oSrc);
    // add to group & hilite
    if(iGrp) {
      if(os1->dli >= 0) DL_hili_on (os1->dli);
      Grp_add__ (os1->typ, os1->dbi, os1->dli,  1);
    }
  }

  MemTab_free (&parTab);

  if(iGrp) {
    Grp_upd (0);  // Redraw & update GrpNr-label
  }

  return 0;

}


//================================================================
  int UI_mcl_modNam () {
//================================================================
// modify ObjName
// ObjName: add " #<name>" at end of code
 
  int  irc;
  long lNr, ll;
  char *p1; //, s1[80];


  // ll = strlen(s1);
  // if(ll > 60) ll = 60;
  // // strncpy(mcl_nam, s1, ll);
  // UTX_cp_nchr_2_upper (mcl_nam, s1, ll);  // uppercase !
  // mcl_nam[ll] = '\0';


  // strcpy(s1, mcl_nam);
  irc = GUI_Dialog_e2b ("modify name/info:", mcl_nam, 80, "OK", "Cancel");
  if(irc != 0) return -1;

    // printf("UI_mcl_modNam |%s| %ld\n",mcl_nam,mcl_rNr);


  // Get line, cut name, add new name,
  p1 = UTF_GetLinNr (mem_cbuf1, &ll, mcl_rNr);
  ll = APED_onam_cut (mem_cbuf1);
  // strcpy(&mem_cbuf1[ll], (char*)data);
  strcat(mem_cbuf1, " #");
  strcat(mem_cbuf1, mcl_nam);


  // modify line in memory
  UTF_chg_line (mcl_rNr, mem_cbuf1);


  // modify list
  // UI_mcl_modC (2, mcl_nam);
  GUI_list2_modCol (&mcl_win, 2, mcl_nam);


  return 0;

}


//================================================================
  int UI_mcl_get_win (MemObj **mo) {
//================================================================
/// UI_mcl_get_win                   returns window

  *mo = &mcl_win;

  return 0;

}

 
//=========================================================================
  int UI_mcl_CB_popup (MemObj *mo, void **data) {
//=========================================================================
// line isel in popup-menu selected.

  int    iEv, isel, i1;
  long   lNr, dbi;
  char   s1[32];


  iEv = GUI_DATA_EVENT;
  if(iEv != TYP_EventPress) return 0;

  
  i1 = GUI_DATA_I1;
  isel = mclPopIA[i1];
    // printf("UI_mcl_CB_popup %d %d\n",isel,i1);

  dbi = atol(&mcl_ID[1]);



  //================================================================
  // handle modify
  if(isel >= 100) {
    i1 = isel - 100;
    SRCU_win_var (mcl_typ, dbi, i1);
    return 0;
  } 
    


  //================================================================
  switch (isel) {
    //----------------------------------------------------------------
    case MSG_cpyMdl:        // "copy to model"
      // report to CAD:
      if(UI_InpMode == UI_MODE_CAD) {
        IE_sel_CB__ (mcl_typ, dbi, mcl_ID);
        return 0;
      }

      // report to MAN:
      if(UI_InpMode == UI_MODE_MAN) {
        ED_add_Text (mcl_typ, dbi, mcl_ID);
        ED_update (0L);    // update Window ..
        return 0;
      }
      TX_Print("copy objID to CAD or MAN; VWR: unused ..");
      return 0;


    //----------------------------------------------------------------
    case MSG_modNam:        // "modify name"
      UI_mcl_modNam ();
      // GUI_GetText("modify name/info ",
                  // mcl_nam, -300, UI_mcl_modNam);
      break;


    //----------------------------------------------------------------
    case MSG_edit:        // "modify object"
      // CAD must be active
      if(UI_InpMode != UI_MODE_CAD) {
        MSG_pri_0("ERRCAD0");
        return 0;
      }
      ED_set_lnr_act (mcl_rNr);
      IE_activate ();
      break;


    //----------------------------------------------------------------
    case MSG_delete:        // "delete object"
        // printf(" del obj %d %ld\n",mcl_typ,dbi);

      Del_obj__ (mcl_typ, dbi);
        // printf(" nach Del_obj__\n");


      // update
      UI_mcl_update ();

/*
      // make the listRow unselectable
      // - zu kompliziert (see Brw_obj_mod GUI_TreeIT_row_set)
      // delete row
      // gtk_widget_set_sensitive (widget, FALSE);  // HÄNGT!
      // gtk_list_store_set (mcl_list, &iter, 0, c1, 1, c2, 2, c3, -1);
      gtk_list_store_remove  (mcl_list, &mcl_iter);
*/
      break;


    //----------------------------------------------------------------
    case MSG_inspect:        // "analyze object"
        // printf(" analyze obj %d %ld\n",mcl_typ,mcl_dbi);
      OVR_add__ (mcl_typ, mcl_dbi);
      break;


    case MSG_getPar:          // find parent-objects
      APED_oid_dbo__ (s1, mcl_typ, mcl_dbi);
      UI_mcl_ctrl (1, s1);
      break;


    case MSG_getChd:          // find child-objects
      APED_oid_dbo__ (s1, mcl_typ, mcl_dbi);
      UI_mcl_ctrl (2, s1);
      break;


    //----------------------------------------------------------------
  }

  return 0;

}


//================================================================
int UI_mcl_CBL (MemObj *mo, void **data) {
//================================================================
// UI_mcl_CBL           CB of list-selection; create popup
// CB of GUI_list2__  GUI_DATA_I1=mouseButtonNr;
// selection in MultiColumnList-Callback.

// see OMN_popup_Brw OMN_selMen_MAN_M3_empty


  int          iPop, i1, i2;
  long         dli;
  double       d1;
  // char         s1[64];



  // printf("UI_mcl_CBL \n");

    // get mcl_ID = col1; mcl_nam=col3;
    strcpy(mcl_ID,  GUI_DATA_S3);         // sel.Text col.1
      // printf(" mcl_ID=|%s|\n",mcl_ID);
    strncpy(mcl_nam, GUI_DATA_S5, 62);
    mcl_nam[63] = '\0';
      // printf(" mcl_nam=|%s|\n",mcl_nam);



    //----------------------------------------------------------------
    //decode Index of selected obj
    mcl_typ = AP_typ_typChar (mcl_ID[0]);
    mcl_dbi = atol(&mcl_ID[1]);
      // printf(" typ=%d dbi=%ld\n",mcl_typ,mcl_dbi);

    // find lineNr 
    i1 = APED_find_dbo (&dli, &mcl_rNr, mcl_typ, mcl_dbi);


    iPop = 0;

    //----------------------------------------------------------------
    // activeTyp=Typ_VAR:     den Inhalt als Text anzeigen
    if(mcl_typ != Typ_VAR) goto L_typ_vc;
      d1 = DB_GetVar (mcl_dbi);
      if(d1 == UT_VAL_MAX) {printf("UI_mcl_CBL E001\n"); return -1;}
      // display value
      TX_Print("%s = %f", mcl_ID, d1);
      goto L_pop9;


    //----------------------------------------------------------------
    // activeTyp=Typ_VC:      den Vector temporaer anzeigen
    L_typ_vc:
    if(mcl_typ != Typ_VC) goto L_typ_def;
      UI_disp_vec1 (Typ_Index, (void*)mcl_dbi, NULL);
      goto L_pop9;
      // return 0;



    //----------------------------------------------------------------
    // activeTyp=Typ_PT|Typ_PLN: das Obj hiliten
    L_typ_def:
      // get il = DispListIndex from objID
      dli = DL_find_obj (mcl_typ, mcl_dbi, -1L);
      // hilite obj
      DL_hili_on (-1L); // loeschen der Hili-Tabelle
      if(dli >= 0) DL_hili_on (dli);
      DL_Redraw ();

  // }


  //----------------------------------------------------------------
  // test for right mouseButton
  L_pop9:
  if(GUI_DATA_EVENT != TYP_EventPress) goto L_exit;
  if(GUI_DATA_I1 != 3) goto L_exit;                    // M1: hilite only ..

  // add to popup
  // only with CAD or MAN:
  if((UI_InpMode == UI_MODE_CAD)     ||
     (UI_InpMode == UI_MODE_MAN)) {
    if(UI_InpMode == UI_MODE_CAD) {
      if(IE_get_Func() >= 0) {
        mclPopIA[iPop] = MSG_cpyMdl;  ++iPop;
      }
    }
    mclPopIA[iPop] = MSG_edit;  ++iPop;
  }

  mclPopIA[iPop] = MSG_delete;   ++iPop;
  mclPopIA[iPop] = MSG_modNam;   ++iPop;   // "modify name"
  mclPopIA[iPop] = MSG_inspect;  ++iPop;
  mclPopIA[iPop] = MSG_getPar;   ++iPop;   // "find parent-objects"
  mclPopIA[iPop] = MSG_getChd;   ++iPop;   // "find child-objects"



  //----------------------------------------------------------------
  // create the list of menutextpointers
  for(i1=0; i1<iPop; ++i1) {
    // get index of i2 in MenTab
    // i2 = UTI_ind_iTab_i (MenInd[i1], MenIa, MenSiz);
    i2 = mclPopIA[i1];
    if(i2 < 0) continue;
    // mclPopSA[i1] = optLst[i2];
    mclPopSA[i1] = UtxTab__ (i2, &MenTab);
      // printf("  mclPopSA[%d] %d |%s|\n",i1,i2,mclPopSA[i1]);
  }



  // add modify-value         see also OMN_popup_Br_modVals
  iPop = UI_mcl_CB_modVals (iPop);


  mclPopSA[iPop] = NULL;  // terminate List
  GUI_popup__ (mclPopSA, NULL, 0, UI_mcl_CB_popup, NULL);


  //----------------------------------------------------------------
  L_exit:
  return 0;

}


//================================================================
  int UI_mcl_CB_modVals (int ii) {
//================================================================
// add menuentries for all values of definitionline of object mcl_typ,mcl_dbi

  int          itsMax, itsAct, i1, i2, ll;
  long         lNr;
  ObjTXTSRC    *tso;
  char         *s1, *p1, *p2, oid[32];

  // static   char *sa1[]={"X","Y","Z"};



  // printf("UI_mcl_CB_modVals ii=%d\n",ii);
  // printf(" typ=%d dbi=%ld\n",mcl_typ, mcl_dbi);


  // get codeline for obj mcl_typ,mcl_dbi
  // get definition-line for db-obj typ,dbi;
  APED_oid_dbo__ (oid, mcl_typ, mcl_dbi);
  i1 = APED_search_defLn (&p1, &lNr, &ll, oid, -1L, 0);
  if(i1 < 0) goto L_exit;
    // printf(" lNr=%ld ll=%d |",lNr,ll);
    // UTX_dump_cnl (p1, 60); printf("|\n");

  // skip def.header
  p2 = strchr (p1, '=');
  if(!p2) {TX_Print("UI_mcl_CB_modVals E001"); goto L_exit;}
  ++p2;  // skip '='
  // terminate code
  p1[ll] = '\0';


  itsMax = SRCU_tsMax (p2);
    // printf(" itsMax=%d siz=%d\n",itsMax,itsMax * sizeof(ObjTXTSRC));
  tso = UME_alloc_tmp (itsMax * sizeof(ObjTXTSRC));

  itsAct = APED_txo_srcLn__ (tso, itsMax, p2);
    // printf(" _txo_srcLn__ %d\n",itsAct);
    // SRC_txo_dump (tso, p2);

  // restore code
  p1[ll] = '\n';

  // get list of all value-objects
  SRCU_list_vals__ (tso, p2);

  // make list
  s1 = memspc011;
  p1 = s1 + sizeof(memspc011) - 32; // eos

  i1 = 0;
  for(;;) {
    if(tso[i1].typ == TYP_FuncEnd) break;
    SRCU_txt_popCoord (s1, tso[i1].typ, tso[i1].ipar, i1);
    mclPopIA[ii] = 100 + i1;
    mclPopSA[ii] = s1;
    s1 += strlen(s1) + 1;  // next free spc in s1 ..
    ++ii;
    ++i1;
    if(s1 >= p1 ) break;
    if(ii >= 100) break;
  }


  L_exit:
  return ii;

}


//================================================================
  int UI_mcl_ctrl (int mode, char *txt) {
//================================================================
/// \code
/// activate search-window from program
/// mode 1                  find parents of obj "<txt>"   
///      2                  find childs of obj "<txt>"   
///      UI_FuncUCB-        internal (1000-
/// \endcode


  // printf("UI_mcl_ctrl %d |%s|\n",mode,txt);


  //----------------------------------------------------------------
  // 1 = find parents of obj "<txt>"
  if((mode == 1)||(mode == 2)) {

    // start search-window / raise ..
    UI_mcl__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncInit));

    // activate all types
    UI_mcl_CBB (NULL, GUI_SETDAT_EI(TYP_EventPress,TYP_FuncInit1));

    // clear filterText mcl_txt__
    UI_mcl_ctrl (UI_FuncUCB1, "");

    // select box "parents of"  (_CB1 UCB13)
    if(mode == 1) UI_mcl_ctrl (UI_FuncUCB13, NULL);

    // select box "childs of"  (_CB1 UCB12)
    if(mode == 2) UI_mcl_ctrl (UI_FuncUCB12, NULL);

    // set objID into entry mcl_dep__ (childs/parents); eg "P21"
    UI_mcl_ctrl (UI_FuncUCB2, txt);

    // start search-update (_CB1 UCB9)
    UI_mcl_ctrl (UI_FuncUCB9, NULL);

    return 0;
  }



  //================================================================
  // set entry filterText
  if(mode == UI_FuncUCB1) {
    GUI_entry_set (&mcl_txt__, txt);
    return 0;
  }


  //----------------------------------------------------------------
  // set entry childs/parents
  if(mode == UI_FuncUCB2) {
    GUI_entry_set (&mcl_dep__, txt);
    return 0;
  }


  //----------------------------------------------------------------
  // start search-update (_CB1 UCB9)
  if(mode == UI_FuncUCB9) {
    UI_mcl_CB1 (NULL, GUI_SETDAT_EI(TYP_EventPress,mode));
    return 0;
  }


  //----------------------------------------------------------------
  // select box "childs of"  (_CB1 UCB12)
  if(mode == UI_FuncUCB12) {
    UI_mcl_CB1 (NULL, GUI_SETDAT_EI(TYP_EventPress,mode));
    return 0;
  }


  //----------------------------------------------------------------
  // select box "parents of"  (_CB1 UCB13)
  if(mode == UI_FuncUCB13) {
    UI_mcl_CB1 (NULL, GUI_SETDAT_EI(TYP_EventPress,mode));
    return 0;
  }


  return 0;
}


//================================================================
  int UI_mcl_CBB (MemObj *mo, void **data) {
//================================================================
// selection of objType-radioButton.
 
  int   i1, i2, ii;


  // i1 = INT_PTR(data);
  i1 = GUI_DATA_I1;

  // printf("UI_mcl_CBB %d %d\n",GUI_DATA_EVENT,i1);

  ii = sizeof(mcl_cba) / sizeof(MemObj);



  //----------------------------------------------------------------
  // clear all
  if(i1 == TYP_FuncInit) goto L_all_off;

  //----------------------------------------------------------------
  // activate all
  if(i1 == TYP_FuncInit1) goto L_all_on;

  //----------------------------------------------------------------
  // "all"-button selected
  if(i1 == Typ_ALL) {
    // test if more are on or off
    i2 = 0;
    for(i1=0; mclTab[i1] != Typ_Error; ++i1) {
      if(GUI_ckbutt_get(&mcl_cba[i1])) ++i2;
    }
    if(i2 < 6) goto L_all_on;
    else       goto L_all_off;
  }

  //----------------------------------------------------------------
  // activate (from program)
  if(!mo) {
    // find i1 in mclTab
    for(i2=0; mclTab[i2] != Typ_Error; ++i2) {
      if(mclTab[i2] == i1) {
        GUI_ckbutt_set (&mcl_cba[i2], TRUE);
        BitTab_set (mcl_typTab, i1);
      }
    }
    return 0;
  }



  //----------------------------------------------------------------
  // change (manually selected)
  // if(GTK_TOGGLE_BUTTON (parent)->active == 0) {
  if(GUI_ckbutt_get(mo)) {
      // printf(" set bit %d\n",i1);
    BitTab_set (mcl_typTab, i1);

  } else {
      // printf(" clr bit %d\n",i1);
    BitTab_clr (mcl_typTab, i1);
  }
    // BitTab_dump (mcl_typTab, BitTab_bNr (mcl_typTab));
  return 0;



  //----------------------------------------------------------------
  L_all_on:
      // printf(" all on ..\n");
    for(i1=0; i1<ii; ++i1) GUI_ckbutt_set (&mcl_cba[i1], TRUE);
    return 0;


  L_all_off:
      // printf(" all off ..\n");
    for(i1=0; i1<ii; ++i1) GUI_ckbutt_set (&mcl_cba[i1], FALSE);
    return 0;



}


//================================================================
  int UI_mcl_CB1 (MemObj *mo, void **data) {
//================================================================
// update = UI_FuncWork = 98

  int    iCod, i1;
  char   *p1;

  static UINT_8 txtNot=0, ignCas=1, mode=0, op1=Typ_ope_or, op2=0, grp=1;


  // iCod = INT_PTR (data);
  iCod = GUI_DATA_I1;
    // printf("UI_mcl_CB1 %d\n",iCod);



  //----------------------------------------------------------------
  if(iCod == UI_FuncUCB1) {   // new group
    if(GUI_ckbutt_get(mo)) mode = 0;


  //----------------------------------------------------------------
  } else if(iCod == UI_FuncUCB2) {   // add to list
    if(GUI_ckbutt_get(mo)) mode = 1;


  // //----------------------------------------------------------------
  // } else if(iCod == UI_FuncUCB3) {   // remove from list
    // if(GUI_ckbutt_get(mo)) mode = 2;


  //----------------------------------------------------------------
  } else if(iCod == UI_FuncUCB4) {   // txtNot not
    txtNot = GUI_ckbutt_get(mo);


  //----------------------------------------------------------------
  } else if(iCod == UI_FuncUCB5) {   // ignore case
    ignCas = GUI_ckbutt_get(mo);


  //----------------------------------------------------------------
  // } else if(iCod == UI_FuncUCB6) {   // AND         UNUSED
    // if(GUI_ckbutt_get(mo)) op1 = Typ_ope_and;


  //----------------------------------------------------------------
  // } else if(iCod == UI_FuncUCB7) {   // OR         UNUSED
    // if(GUI_ckbutt_get(mo)) op1 = Typ_ope_or;


  //----------------------------------------------------------------
  } else if(iCod == UI_FuncUCB8) {    // Group
    grp = GUI_ckbutt_get(mo);


  //----------------------------------------------------------------
  } else if(iCod == UI_FuncUCB9) {   // update (search);

    // get text from entry  mcl_dep__ (child/parents)
    p1 = GUI_entry_get (&mcl_dep__);
    i1 = strlen(p1);
    if(i1 > sizeof(mcl_dept)) i1 = sizeof(mcl_dept);
    // strncpy(mcl_dept, p1, i1);
    UTX_cp_nchr_2_upper (mcl_dept, p1, i1);
    mcl_dept[i1] = '\0';
      // printf(" mcl_dept=|%s|\n",mcl_dept);
     

    // get text in mcl_txt__ (filtertext)
    p1 = GUI_entry_get (&mcl_txt__);
    i1 = strlen(p1);
    if(i1 > sizeof(mcl_text)) i1 = sizeof(mcl_text);
    if(ignCas) {   // 1=ignore case, 0=not
      UTX_cp_nchr_2_upper (mcl_text, p1, i1);
    } else {
      strncpy(mcl_text, p1, i1);
    }
    mcl_text[i1] = '\0';
      // printf(" mcl_text=|%s|\n",mcl_text);


    // make wordTable from filtertext
    UTX_wTab_str (&mcl_wTab, sizeof(mcl_wTab), mcl_text);
      // UTX_wTab_dump (&mcl_wTab);

    // mode: 0=new; 1=add
    if(mode == 0) { 

      // clear list
      GUI_list1_clear (&mcl_win);

      if(grp) {
        Grp_Clear (0);       // clear group
        DL_hili_off (-1L);  // unhilite all
      }
    }

    // work ..
    UI_mcl_load (mode, txtNot, ignCas, op2, grp);


  //----------------------------------------------------------------
  } else if(iCod == UI_FuncUCB10) {   // all on or all off
    UI_mcl_CBB (NULL, GUI_SETDAT_EI(TYP_EventPress,Typ_ALL));


  //----------------------------------------------------------------
  } else if(iCod == UI_FuncUCB11) {      // Help
    APP_Help ("Search", "");


  //----------------------------------------------------------------
  } else if(iCod == UI_FuncUCB12) {      // "is child of"
    if(!mo) {
      // activate from program
      GUI_ckbutt_set (&mcl_rbC, 1);
    } else {
      // manually checked
      if(GUI_ckbutt_get(mo)) op2 = 0;
    }


  //----------------------------------------------------------------
  } else if(iCod == UI_FuncUCB13) {      // "is parent of"
    if(!mo) {
      // activate from program
      GUI_ckbutt_set (&mcl_rbP, 1);
    } else {
      // manually checked
      if(GUI_ckbutt_get(mo)) op2 = 1;
    }


  //----------------------------------------------------------------
  }

    // printf(" ex UI_mcl_CB1 mode=%d txt=%d grp=%d\n", mode,txt,grp);
    // { char **pa; pa = mcl_wTab;
    // while (*pa) { printf(" |%s|\n",*pa); ++pa; } }


  return 0;

}

 
/*
//=====================================================================
  int UI_mcl_CBE (void *parent, GdkEventAny *event) {
//=====================================================================
/// \code
/// CB von Eingabe in das Feld Filtertext.
/// den Listeninhalt filtern und wieder ausgeben.
/// \endcode


  int  i1;
  void *ev_k;
  char *txt, **pa;


  printf("UI_mcl_CBE ev.typ%d\n",event->type);


  // if(event->type == GDK_FOCUS_CHANGE) return;        // 12
  if(event->type != GDK_KEY_RELEASE) return 0;          // 9       2011-03-01
  // if(event->type != GDK_KEY_PRESS) return 0;         // 8

  ev_k = (void*) event;
    // printf(" ev_k->keyval=%d\n",ev_k->keyval);


  switch (ev_k->keyval) {

    case GDK_Return:
    case GDK_Tab:

      txt = (char*)gtk_entry_get_text ((GtkEntry*) (parent));
        // printf(" Filtertext=|%s|\n",txt);

      // ----------------------- has infotext ---------------
      if(parent == mcl_txt__) {
        // copy Filtertext
        i1 = strlen(txt);
        if(i1 > sizeof(mcl_text)) i1 = sizeof(mcl_text);
        strncpy(mcl_text, txt, i1);
        mcl_text[i1] = '\0';

        // make wordTable from string
        UTX_wTab_str (&mcl_wTab, sizeof(mcl_wTab), mcl_text);
        // pa = mcl_wTab;
        // while (*pa) { printf(" |%s|\n",*pa); ++pa; }


      // ----------------------- depends on ---------------
      } else {
        printf(" depends on |%s|\n",txt);


      }

      // reCreate List
      UI_mcl_clear ();
      UI_mcl_load ();

  }

  return 0;
}


//================================================================
  int UI_mcl_clear () {
//================================================================

  gtk_list_store_clear (mcl_list);

  return 0;

}
*/

/*
//================================================================
  int UI_mcl_init_selected () {
//================================================================
// put sel.objects into list        UNUSED

  // test if group exists;
  // if yes: load objects into List.

  int     i1, GrpNr;
  long    lNr, lLen;
  char    *oSrc;
  ObjDB   *oTab;


  GrpNr = Grp_get__ (&oTab);

    printf("UI_mcl_init_selected %d\n",GrpNr);

  if(GrpNr < 1) return 0;


  // test if group exists;
  // if yes: load objects into List.


  for(i1=0; i1<GrpNr; ++i1) {
    // printf(" oTab[%d]  %d %ld %ld %d\n",i1,
      // oTab[i1].typ,oTab[i1].dbInd,oTab[i1].dlInd,oTab[i1].stat);

    // get lNr from dli
    DL_Get_lNr_dli (&lNr, oTab[i1].dlInd);       // get lNr from DL-ind

    // search obj in mem
    oSrc = UTF_GetPosLnr (&lLen, lNr);

    // add obj to list
    UI_mcl_add__ (oTab[i1].typ, oTab[i1].dbInd, lNr, oSrc);
  }


  return 0;

}
*/

//=====================================================================
  int UI_mcl_add__ (int typ, long dbi, int lNr, char *oSrc) {
//=====================================================================
// add row to list
// Input:
//   typ,dbi     disp this objID as col.1
//   lNr         src-lineNr; used for getting dli for add-to-group
//   oSrc        codeline; 



// 1. Column: objID
// 2. Column: sourceCode (without "<objID>=")
// 3. Column: objName


  int      irc, ls;
  long     l1, dli;
  char     actObj[32], actVal[32], actNam[64], *oEnd, *oTxt, *oNam;


  // printf("UI_mcl_add__ %d %ld %d\n",typ,dbi,lNr);
  // printf("  _mcl_add__ oSrc|");UTX_dump_cnl(oSrc, 50);printf("|\n");


  // create objName from mcl_typ & idb
  APED_oid_dbo__ (actObj, typ, dbi);
    // printf(" actObj=|%s|\n",actObj);


  // skip hdr
  oTxt = strchr (oSrc, '=');
  if(!oTxt) { printf("UI_mcl_add__ E001\n"); return -1; }
  ++oTxt;

  // find last position of code (incl name)
  oEnd = strchr (oTxt, '\n');
  if(!oEnd) {l1 = strlen(oTxt); oEnd = &oTxt[l1]; }

  // find position of name
  oNam = strstr (oTxt, " #");
  if(!oNam) oNam = oEnd;

  // copy code > actVal (without name)
  ls = oNam - oTxt;
  ls = ILIM2 (ls, 0, 30);
  strncpy(actVal, oTxt, ls);
  actVal[ls] = '\0';

  // copy objName -> actNam
  ls = oEnd - oNam;
  if(ls > 0) {
    // name exists; remove " #"
    ls -= 2;
    oNam += 2;
    // skip blanks
    while(*oNam == ' ')  { ++oNam; --ls; }
    ls = ILIM2 (ls, 0, 60);
    strncpy(actNam, oNam, ls);
    actNam[ls] = '\0';
      // printf(" _mcl_add__ |%s|%s|%s|\n",actObj,actVal,actNam);
  } else {
    strcpy(actNam, "-");
  }
    // printf(" _mcl_add__ |%s|%s|%s|\n",actObj,actVal,actNam);


  // fill next row
  // UI_mcl_add (actObj, actVal, actNam);
  GUI_list1_add_l (&mcl_win, actObj, actVal, actNam);


  return 0;

}
 

//================================================================
  int UI_mcl_update () {
//================================================================
 

  // exit if window is not active
  if(UI_cb_search (4) == 0) return 0;

  printf(" ************** UI_mcl_update\n");


  // UI_mcl_CB1 (NULL, PTR_INT(UI_FuncWork));   // not yet implemented ! ?


  // bring window on top
  // GUI_WinRaise (mcl_win);
  GUI_Win_raise (&mcl_win);           // geht ned !


  return 0;

}


//================================================================
  int UI_mcl__ (MemObj *mo, void **data) {
//================================================================
// Search/Name as MultiColumnList.
// Functions:
//   UI_FuncInit       create ListWindow
//   UI_FuncAdd        add Line
//   -                 modify Line
/*
boxv0
 boxh1 (buttonReihe1)
 boxh2 (buttonReihe2) bt
 bhc (horBox for all controls)
  bv1                     bv2                  bv3
   entry                   cb "not"             rb rb cb
   rb rb entry             cb "not"             bt  bt
 bvl
*/


static MemObj win0;


  MemObj   box0, boxv0, boxh1, boxh2, w1, w2;
  MemObj   bhc, bv1, bv2, bv3, bvl;

  int      i1, iCol, iSizY;
  char     s1[8], *tiTab[] = {"ObjID","Code","Name / Info"};



  i1 = GUI_DATA_I1;

  // printf("UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU \n");
  // printf("UI_mcl__ %d\n",i1);


  switch (i1) {


    //================================================================
    case UI_FuncExit:          // 102
        printf(" UI_FuncExit..\n");
      // GUI_list1_clear (&mcl_win);
      // GUI_hide__ (&win0);

      if(GUI_OBJ_IS_VALID(&mcl_win)) {
        GUI_Win_kill (&win0);
        mcl_win = GUI_OBJ_INVALID ();
        // deselect checkbox "Search/Name"
        UI_cb_search (2);
        DL_Redraw ();
      }
      return 0;
      


    //================================================================
    case UI_FuncInit:          // 90

      // Create Mainwindow

      // initial
      if(mcl_typ == -1) {
        // clear bitArray
        BitTab_ini1 (mcl_typTab);    // BitTab_ini0
        mcl_typ = 0;
      }

      if(GUI_OBJ_IS_VALID(&mcl_win)) {
        GUI_show__ (&win0);
        GUI_Win_raise (&win0);
        // gtk_widget_show (mcl_win);
        // GUI_show__ (&mcl_win);
/*
        // test if obj's in group; if yes: display group; else update ..
        if(Grp_get_nr() > 0) {
          UI_mcl_init_selected ();  // put sel.objects into list
        } else {
          UI_mcl_update();
        }
*/
        break;
      }

      win0  = GUI_Win__ ("ObjectList", UI_mcl__, "");
      boxv0 = GUI_box_v (&win0, "e,e");
      // 1. hor. box fuer die Buttons
      strcpy (s1, "e,a");

      boxh1 = GUI_box_h (&boxv0, s1);

      mcl_cba[0] =
      GUI_ckbutt__(&boxh1,"Points", BitTab_get(mcl_typTab,mclTab[0]), UI_mcl_CBB,
                            &mclTab[0],  s1);
      mcl_cba[1] =
      GUI_ckbutt__(&boxh1,"Lines",  BitTab_get(mcl_typTab,mclTab[1]), UI_mcl_CBB,
                            &mclTab[1],  s1);
      mcl_cba[2] =
      GUI_ckbutt__(&boxh1,"Circs",  BitTab_get(mcl_typTab,mclTab[2]), UI_mcl_CBB,
                            &mclTab[2],  s1);
      mcl_cba[3] =
      GUI_ckbutt__(&boxh1,"Curves", BitTab_get(mcl_typTab,mclTab[3]), UI_mcl_CBB,
                            &mclTab[3],  s1);
      mcl_cba[4] =
      GUI_ckbutt__(&boxh1,"Planes", BitTab_get(mcl_typTab,mclTab[4]), UI_mcl_CBB,
                            &mclTab[4], s1);
      mcl_cba[5] =
      GUI_ckbutt__(&boxh1,"Surfs",  BitTab_get(mcl_typTab,mclTab[5]), UI_mcl_CBB,
                            &mclTab[5], s1);
      mcl_cba[6] =
      GUI_ckbutt__(&boxh1,"Bodies", BitTab_get(mcl_typTab,mclTab[6]), UI_mcl_CBB,
                            &mclTab[6], s1);
      mcl_cba[7] =
      GUI_ckbutt__(&boxh1,"Models", BitTab_get(mcl_typTab,mclTab[7]), UI_mcl_CBB,
                            &mclTab[7], s1);


      // hor. box fuer die Buttons
      boxh2 = GUI_box_h (&boxv0, s1);
      
      mcl_cba[8] =
      GUI_ckbutt__(&boxh2,"Variables",BitTab_get(mcl_typTab,mclTab[8]),UI_mcl_CBB,
                            &mclTab[8], s1);
      mcl_cba[9] =
      GUI_ckbutt__(&boxh2,"Vectors",BitTab_get(mcl_typTab,mclTab[9]), UI_mcl_CBB,
                            &mclTab[9],  s1);
      mcl_cba[10] =
      GUI_ckbutt__(&boxh2,"Notes",  BitTab_get(mcl_typTab,mclTab[10]), UI_mcl_CBB,
                            &mclTab[10], s1);
      mcl_cba[11] =
      GUI_ckbutt__(&boxh2,"Activi", BitTab_get(mcl_typTab,mclTab[11]), UI_mcl_CBB,
                            &mclTab[11], s1);
      mcl_cba[12] =
      GUI_ckbutt__(&boxh2,"Transf", BitTab_get(mcl_typTab,mclTab[12]), UI_mcl_CBB,
                            &mclTab[12], s1);


      // GUI_button__(&boxh2,MSG_const__(MSG_all), UI_mcl_CB1,
      GUI_button__(&boxh2,"  all  ", UI_mcl_CB1, &GUI_FuncUCB10, "e,a");

      GUI_sep_h (&boxv0, 2);


      bhc = GUI_box_h (&boxv0, "a,a");
      bv1 = GUI_box_v (&bhc, "a,a");

        w2 = GUI_box_h (&bv1, "a,a");
          mcl_txt__ = GUI_entry__ (&w2, " has infotext:", "", NULL, NULL, "a,a");
          GUI_ckbutt__ (&w2, "not ", FALSE, UI_mcl_CB1,
                      &GUI_FuncUCB4, "a,a");
          MSG_Tip        ("SN_TxtNot");
          GUI_ckbutt__ (&w2, "ignore case ", TRUE, UI_mcl_CB1,
                      &GUI_FuncUCB5, "a,a");


        GUI_sep_h (&bv1, 2);
        w2 = GUI_box_h (&bv1, "a,a");

          // GUI_radiobutt__ (&w2, "or  ",       0, UI_mcl_CB1,
                           // &GUI_FuncUCB7, "a,a");
          // MSG_Tip ("SN_Add");
          // GUI_radiobutt__ (&w2, "and ",       1, UI_mcl_CB1,
                           // &GUI_FuncUCB6, "a,a");


          // GUI_sep_v (&w2, 2);
          mcl_rbC = GUI_radiobutt__ (&w2, "childs of  ",       0, UI_mcl_CB1,
                           &GUI_FuncUCB12, "a,a");
          // MSG_Tip ("SN_Add");
          mcl_rbP = GUI_radiobutt__ (&w2, "parents of  ",       1, UI_mcl_CB1,
                           &GUI_FuncUCB13, "a,a");



          // MSG_Tip ("SN_New");
          mcl_dep__ = GUI_entry__ (&w2, NULL, "", NULL, NULL, "a,a");

          GUI_sep_v (&bhc, 2);

/*
      bv2 = GUI_box_v (&bhc, "a,a");

        GUI_ckbutt__ (&bv2, " not              ", FALSE, UI_mcl_CB1,
                      &GUI_FuncUCB4, "a,a");
        MSG_Tip        ("SN_TxtNot");

        GUI_ckbutt__ (&bv2, " not              ", FALSE, UI_mcl_CB1,
                      &GUI_FuncUCB5, "a,a");
        MSG_Tip        ("SN_DepNot");
*/


      bv3 = GUI_box_v (&bhc, "a,a");
      w2 = GUI_box_h (&bv3, "a,a");

      GUI_radiobutt__ (&w2, "new ", 0, UI_mcl_CB1,
                       &GUI_FuncUCB1, "a,a");
      MSG_Tip ("SN_New");

      GUI_radiobutt__ (&w2, "add ",       1, UI_mcl_CB1,
                       &GUI_FuncUCB2, "a,a");
      MSG_Tip ("SN_Add");

      GUI_ckbutt__   (&w2, " Group ", TRUE, UI_mcl_CB1,
                      &GUI_FuncUCB8, "a,a");
      // MSG_Tip        ("SN_DepNot");
      GUI_Tip ("make all (visible) objects of list group-objects");




      // GUI_RadButtAdd (w2, "remove",      UI_mcl_CB1,
                      // PTR_INT(UI_FuncUCB3), 0);
      // MSG_Tip ("SN_Rem");



      w2 = GUI_box_h (&bv3, "a,a");
        GUI_button__(&w2, "Help",      UI_mcl_CB1, &GUI_FuncUCB11, "e,a");
        GUI_button__(&w2, "search/update", UI_mcl_CB1, &GUI_FuncUCB9, "e,a");
        GUI_Tip (MSG_const__(MSG_upd));

      // list
      bvl = GUI_box_v (&boxv0, "e,e");
        mcl_win = GUI_list2__ (&bvl, NULL, "3", tiTab, UI_mcl_CBL, "e,16e");
        GUI_Win_go (&win0);
      break;


/*
    //================================================================
    // case UI_FuncKill:  kommt nicht wegen delete-event !
    default:
      // printf("remove mcl\n");
      // window nicht deleten, clear Inhalt, hide.
      // clear mcl
      // UI_mcl_clear ();
      GUI_list1_clear (&mcl_win);

      // remove display
      // gtk_widget_hide (mcl_win);
      GUI_hide__ (&mcl_win);

      // deselect checkbox "Search/Name"
      UI_cb_search (2);
      DL_Redraw ();

      return TRUE;   // prevent the default handler from being run  !!!
*/
  }


  return (FALSE);

}


//======================= EOF ===========================
