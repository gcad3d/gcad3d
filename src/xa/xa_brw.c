//  Object-Browser.                                      2009-10-26  RF
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
Im Browser mit M3 (im browserbackground):
- create new process (select Prozessor;
- create new submodel
dzt (Gtk2) nicht möglich - GUI_tree1_cbMouse kommt immer vor GUI_tree1_cbSel

-----------------------------------------------------
Modifications:
2009-10-26 Created. RF.

-----------------------------------------------------
*/
#ifdef globTag
void Brw(){}
#endif
/*!
\file  ../xa/xa_brw.c
\brief Object-Browser 
\code
=====================================================
List_functions_start:

Brw_load_object1    add objRow to actNod

Brw_typeRow_sel     open or close TypeRow
Brw_typeRow_open    add objects to objectRow
Brw_typeRow_upd     update a typeRow
Brw_objRow_upd      update active obj's
Brw_load_object1    create a objectRow
Brw_close_typeRows  remove all childs of typeRows
Brw_childs_passive  make all childs passive
Brw_chg_mdl         activate subModel in Browser

Brw_obj_upd         update object-row (active or deleted)
Brw_obj_mod         modify object-row (active or deleted)

Brw_Mdl_init        create primary model & all subModels
Brw_sMdl_Init       create all subModelNodes from Mod.lst
Brw_Mdl_upd         update active model mdlNod
Brw_sMdl_act        activate subModel in Browser
Brw_Mod_add         add subModelRow
Brw_Mdl_createRow   create new (sub)ModelRow
Brw_sMdl_ren_CB     rename the active mdlNod to <smNam>
Brw_sMdl_ren__      rename the active mdlNod
Brw_sMdl_del_CB
Brw_sMdl_del__      delete model in browserWin
Brw_Mod_is_main_active  test is mainModel is active

Brw_CB_sel          callback tree-obj-selection 
// Brw_CB_mouse        callback-function mousebutton
Brw_CB_key          callback-function keyIn          NOT ACTIVE
Brw_Init            create Window
Brw_Clear           clear window
Brw_hili_dli        hilite row  (CB M1-selection in GR-Win)
Brw_unselect_all    unselect all

Brw_getAux          return rowAux;
Brw_getTxt          return text of active row;

List_functions_end:
=====================================================

\endcode *//*----------------------------------------




Icons: size = 24 x 16;

see also UI_mcl__ GUI_mList__  ../gtk/tst_tree_it.c
  dev/moavis/mo_src/mo_gui.c

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>                    // isdigit

#include "../ut/ut_geo.h" 
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_os.h"               // OS_ ..
#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_txTab.h"            // TxtTab

#include "../gui/gui__.h"              // Gtk3

#include "../db/ut_DB.h"               // DB_GetGTxt
#include "../ut/func_types.h"               // UI_FuncOK
#include "../xa/xa_brw.h"              // ICO_..
#include "../xa/xa.h"                  // AP_modact_nam
#include "../xa/xa_ico.h"              // ICO_PT,
#include "../xa/xa_mem.h"              // memspc55
#include "../ut/ut_memTab.h"           // MemTab
#include "../xa/xa_ed_mem.h"           // APED_..

// ex ../xa/xa_ui.c
// extern GIO_WinTree winBrw;
extern MemObj   winBrw;
extern int      winBrStat;


// local:

// static int Brw_stat=0;          // 0=select(M1); 1=popup-menu-requested(M3).
// static GtkTreeIter mdlNod;      // the active model-node
// static GtkTreeIter topNod;      // the active node (row)
// static GtkTreeIter actNod;      // the active node (row)
static TreeNode mdlNod;      // the active model-node
static TreeNode topNod;      // the active node (row)
static TreeNode actNod;      // the active node (row)


// extern - ex ../xa/xa.c:
// extern char AP_modact_nam[128];
// extern char AP_mod_fnam[128];


// local vars:
static int  rowAux;        // iconNr of actNod
static char rowTxt[256];   // text of actNod


static char *typTxt[]={    // titles of typeRows
  "Values ","Vectors ","Points ",  "Lines ",  "Circles ",   // 0-
  "Curves ","Planes ", "Surfaces ","Solids ", "Notes ",     // 5-
  "Models ","Models ", "Models ",  "Models "                // 10-
};

static int Brw_ope=1;  // operation; 0=update (skip selection process);
                       //            1=normal selection ..





//================================================================
  int Brw_sMdl_ren_CB (char *smNam) {
//================================================================
/// rename the active mdlNod to <smNam>


  GUI_tree1_row_set (&winBrw, &mdlNod, -1, smNam, 0);
  

  return 0;

}

//================================================================
  int Brw_sMdl_ren__ () {
//================================================================
/// rename the active mdlNod to <smNam>

  // int   ia;
  char cbuf[320];

  extern int Mod_ren_CB ();



  // if actNod is the active model: activate main 
  if(GUI_tree1_cmp_row(&winBrw, &actNod, &mdlNod)) {
    GUI_MsgBox ("cannot rename subModel if not active; activate this model ..");
    return 0;
  }

  Mod_ren__ ();

  return 0;

}



///=====================================================================
  int Brw_sMdl_del_CB (MemObj *mo, void **data) {
///=====================================================================

  int   idat;
  char  cbuf[256];


  idat = GUI_DATA_EVENT;

  printf("Brw_sMdl_del_CB %d\n",idat);

  if(idat != UI_FuncOK) return -1;


  Mod_del1__ (rowTxt);       // delete subModel

  // delete row actNod & its childs
  Brw_close_typeRows (&actNod);
  GUI_tree1_remove__ (&winBrw, &actNod);
  
  return 0;

}


//================================================================
  int Brw_sMdl_del__ () {
//================================================================
/// delete model in browserWin

  // int   ia;
  char cbuf[320];


  // if actNod is the active model: activate main 
  if(!GUI_tree1_cmp_row(&winBrw, &actNod, &mdlNod)) {
    GUI_MsgBox ("cannot delete active subModel; activate other model ..");
    return 0;
  }

  sprintf(cbuf, "  delete Submodel %s  ",rowTxt);
  GUI_DialogYN (cbuf, Brw_sMdl_del_CB);

  return 0;

}


//================================================================
  int Brw_Mdl_init () {
//================================================================
/// create primary model & all subModels

  int           irc;
  TreeNode      it1;


  // printf("Brw_Mdl_init %d\n",winBrStat);


  if(winBrStat < 1) return 0;

  GUI_tree1_clear (&winBrw); 
         
  // create primary node
  GUI_tree1_row_add (&topNod, &winBrw, NULL, ICO_natM,  AP_mod_fnam, 1);
  mdlNod = topNod;



  // if(strlen(AP_modact_nam) == 0) {
  if(Mod_ck_isMain()) {
    Brw_Mdl_upd ();       // fill mdlNod in browserWin
  }
// return 0;

  Brw_sMdl_Init ();     // create all subModelNodes from Mod.lst
  Brw_Prcs_Init ();     // create all processes

  // GUI_Tree_unselect_all (winBrw);
  // GUI_TreeIT_selRow (&topNod, winBrw);  // else error with primary M3


  // if(strlen(AP_modact_nam) > 0) {
  if(!Mod_ck_isMain()) {
    // activate submodel <AP_modact_nam>
    // find submodel
    irc = GUI_tree1_iter_string (&it1, AP_modact_nam, NULL, &winBrw);
    if(irc < 0) {printf("ERROR: Brw_Mdl_init E001\n"); return -1;}
    mdlNod = it1;
    GUI_tree1_row_set (&winBrw, &topNod, -1, NULL, -1);
    GUI_tree1_row_set (&winBrw, &mdlNod, -1, NULL, 1);
    Brw_Mdl_upd ();       // fill mdlNod in browserWin
  }

  // GUI_update__ ();

  // printf("ex Brw_Mdl_init\n");


  return 0;

}


//================================================================
  char* Brw_getTxt () {
//================================================================
/// Brw_getTxt          return text of active row;

  return rowTxt;

}


//================================================================
  int Brw_getAux () {
//================================================================
// 10 = ICO_refM

  return rowAux;

}


/*
//================================================================
  int Brw_subModel (char *mnam) {
//================================================================
// activate main or subModel in browserWindow
// OLD VERSION- replaced by ?

  int          irc, ild;
  GtkTreeIter  itPar;


  printf("Brw_subModel |%s|\n",mnam);

  if(!strcmp(mnam, "-main-")) {
    // clear and rebuild main
    Brw_Mdl_upd (NULL);
    return 0;
  }


  // make the old Parentnode normal
  GUI_TreeIT_row_set (winBrw, &mdlNod, -1, NULL, 0);

 
  // test if goto left or right
  // active modelNode=mdlNod, new modelNode will be actNod.
  ild = GUI_Tree_lev (winBrw, &actNod) - GUI_Tree_lev (winBrw, &mdlNod);
    printf(" ild=%d\n",ild);
  if(ild > 0) goto L_right;
  goto L_left;



  //----------------------------------------------------------------
  // goto right:
  L_right:
  // remove all childs of typeRows != "Submodels"
  Brw_close_typeRows (&mdlNod);

  // make all parents-childs passive
  Brw_childs_passive (&mdlNod);

  goto L_activate;



  //----------------------------------------------------------------
  // goto left:
  L_left:
  // get parent of  old (active) node
  if(ild < 0) {
    irc = GUI_Tree_par (winBrw, &itPar, &mdlNod);
    if(irc) mdlNod = itPar;
  }


  // remove all childs of old (active) node
  GUI_Tree_childs_remove (winBrw, &mdlNod);



  //----------------------------------------------------------------
  L_activate:
  // load model in browser; add to new node
  Brw_Mdl_upd (&actNod);

  mdlNod = actNod;

  return 0;
}
*/


//====================================================================
  int Brw_CB_sel (MemObj *mo, void **data) {
//====================================================================
/// \code
/// callback tree-obj-selection
/// Input:
///   data        pointer to winBrw
/// see also UI_mcl_CBL 
/// \endcode

  int          iStat, i1, iTyp;
  long         iInd, dli;
  long         ii;
  char         *cp1, s1[256], *oid;
  void         *vp1;
  int          Brw_stat;     // mouse-button, GUI_MouseL ..
  TreeNode     itAct;


  Brw_stat = GUI_DATA_I1;   // button 1|2|3; 0=row-update-operation


  // printf("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB \n");
  // printf("Brw_CB_sel stat=%d ope=%d\n",Brw_stat,Brw_ope);


  // if this is a row-update-operation, skip this (automatic) selection
  if(Brw_ope == 0) return -1;


  // ScreenCast ON: draw mouse ..
  if(AP_stat.TUT_stat) {
     GUI_TUT_m__ (Brw_stat);
  }


/*
  // skip deselection
  if(!gtk_tree_selection_get_selected (parent, &model, &itAct)) {
    // if(Brw_stat == 1) {
      // OMN_popup_Brw (Typ_SubModel, 0L, 0L, 2);
      // Brw_stat = 0;
      // GUI_TreeIT_selRow (&mdlNod, winBrw);  // else no M3-CB in open browserBG
    // }
    return;
  }

  actNod = itAct;  // the active node
*/

  actNod =*((TreeNode*)data[2]);

/*
  // decode selected row - get data for first column
  // column-0 (icon) gives adress of icon; not useful ..
  gtk_tree_model_get (model, &itAct, 1, &txt1, -1);  // access Spalte 1=text 
  strcpy(rowTxt, txt1);
  g_free (txt1);
*/

    // printf(" strlen-S3 = %d\n",strlen(GUI_DATA_S3));

  strcpy(rowTxt, GUI_DATA_S3);
  if(strlen(rowTxt) > 80) {
    TX_Print("***** rowTxt exceeds 80 chars - truncated ..");
    rowTxt[80] = '\0';
  }

  // get selectedObjID 
  UTX_cp_word__ (s1, rowTxt);
    // printf(" rowTxt=|%s| s1=|%s|\n",rowTxt,s1);

/*
  // decode row - active or passive
  gtk_tree_model_get (model, &itAct, 2, &iStat, -1);  // access Spalte 2=log 
  // iStat (column-2): 1=active, 0=passive
    // printf(" Brw_CB_sel=|%s| iStat=%d Brw_stat=%d\n",rowTxt,iStat,Brw_stat);
*/

   iStat = GUI_DATA_I5;  // 1=active/0=inactive
     // printf(" iStat=%d\n",iStat);

/*
  // decode row - get icon#  liefert nix verwertbares ..
  gtk_tree_model_get (model, &itAct, 3, &rowAux, -1);  // access Spalte 0=icon
    // printf(" ico# = %d\n",rowAux);
*/

   rowAux = GUI_DATA_I4;   // icon-nr
     // printf(" rowAux=%d\n",rowAux);


  // select (M1) of a passive object: unselect, return.
  if((iStat == 0)&&(Brw_stat == 1)) {
    // Brw_unselect_all ();
    return 0;
  }



  //----------------------------------------------------------------
  // if selection is a process: enable activate
  if(rowAux == ICO_proc) {  // process selected ..
    if(Brw_stat == 3) {     // 1=M3
      iInd = 1;  // 1=subModel
      OMN_popup_Brw (Typ_Process, iInd, 0L, 1);
    }
    return 0;
  }


  //----------------------------------------------------------------
  // if selection is a subModel: enable activate
  if(rowAux == ICO_natM) {  // mainModel or subModel selected ..
    if(Brw_stat == 3) {     // 1=M3
      iInd = 1;  // 1=subModel
      // check if it is main
      // GUI_tree1_ndPos (s1, &winBrw, &itAct); 
      GUI_tree1_ndPos (s1, &winBrw, &actNod); 
        // printf(" s1 = |%s|\n",s1);
      if(!strcmp(s1, "0")) iInd = 0;  // 0=main
      OMN_popup_Brw (Typ_SubModel, iInd, 0L, 1);
    }
    return 0;
  }


  //----------------------------------------------------------------
  // decode which type of row selected: ObjectRow or TypeRow
  if(isdigit(rowTxt[1])) {
    APED_dbo_oid (&iTyp, &iInd, rowTxt);

  } else {
    iTyp = 0;       // this is a typeRow !
    iInd = 0;
  }
    // printf(" iTyp=%d iInd=%ld\n",iTyp,iInd);


  //----------------------------------------------------------------
  if((iStat == 0)&&(Brw_stat == 3)) {  // M3: deleted obj selected (0=passive)
    // menu with undo
    OMN_popup_Brw (iTyp, iInd, 0L, 3);
    return 0;
  }



  //----------------------------------------------------------------
  // TypeRow: open row, display objectRows;
  if(iTyp == 0) {
    if(iStat == 1) {           //  1=active, 0=passive
      // active TypeRow:
      // if(Brw_stat == 3) {
        // open typeRow (display all objRows)
        Brw_typeRow_sel (rowTxt); 
      // select TypeRow mit M1: message "open node with right mousebutton"
      // }
      // } else TX_Print ("open node with right mousebutton ..");
    // } else {
      // passive typeRow:   ignore ..
    }
    return 0;
  }



  //----------------------------------------------------------------
  // it is a ObjectRow ...

  if(iStat == 0) {           //  1=active, 0=passive
    // passive objectRow; typ != TypModel: ignore ..
    if(iTyp != Typ_Model) return 0;
    // passive objectRow; typ == TypModel: OMN "Activate".
    OMN_popup_Brw (iTyp, iInd,  0L, 1);
    return 0;
  }



  //----------------------------------------------------------------
  // ObjectRow selected: hilite object.
  // hilite obj (if it is an active object)
  if(iStat == 1) {                  // if obj is active
    // change "M# name" into "M#=name"
    // add '=' behind objNam in rowtext;
    // find first blank; change -> "="
    cp1 = strchr(rowTxt, ' ');
    if(cp1) {
      *cp1 = '=';
    } else {
      strcat(rowTxt, "=");
    }

    dli = AP_hili_obj (rowTxt);
    // if(dli < 0) return 0;  do also make menu for vectors 2013-09-03
  }


  //----------------------------------------------------------------
  // ObjectRow selected with M3: display popup-menu.
  L_99:
  if(Brw_stat == 1) {
    if(iStat == 1) {
      // if(iTyp == Typ_VC) {
        // UI_GR_Select_work_vc (iInd);
      // } else {
        UI_GR_set_selKey (GUI_MouseL);
        UI_GR_set_selNam (iTyp, iInd, s1);
        UI_GR_Select_work1 (dli); // report selection ..
      // }
    }
    return 0;
  }


  //----------------------------------------------------------------
  // skip this for row being closed
  OMN_popup_Brw (iTyp, iInd, dli, 0);

  return 0;

}

/*
//============================================================
  gint Brw_CB_mouse (void *parent, void *event) {
//============================================================
/// callback-function mousebutton

  // GDK_BUTTON_PRESS  = 4,
  // GDK_BUTTON_RELEASE  = 7,



  // GtkTreeView      *view;
  // GtkTreeSelection *sele;

  printf("Brw_CB_mouse %d %d stat=%d\n",event->button,event->type,Brw_stat);

  // // unselect all; aber nur, wenn ein manuelles expand aktiv ist;
  // // nur bei "select_row_in_browser" kommt Brw_CB_mouse.
  // view = GTK_TREE_VIEW (winBrw.tree);
  // sele = gtk_tree_view_get_selection (view);
  // gtk_tree_selection_unselect_all (sele);    // sonst kein second select moegl
  // GUI_Tree_unselect_all (winBrw);


  //================================================================
  // right-mousebutton; popup window.
  if(((GdkEventButton*)event)->button == 1) {                   // left butt.
    Brw_stat = 0;

  } else if(((GdkEventButton*)event)->button == 3) {            // rightButt=3
    if(((GdkEventButton*)event)->type == GDK_BUTTON_PRESS) {    // press=4
      Brw_stat = 1;  // 1=popup-menu-requested.

    } else {                                 // 7=GDK_BUTTON_RELEASE
      if(Brw_stat == 1) {
        Brw_stat = 0;  // reset popup-menu-requested.
        if(rowAux == ICO_natM)    // mainModel or subModel selected ..
        OMN_popup_Brw (Typ_SubModel, 0L, 0L, 2);
      }
    }
  }

  // printf("ex Brw_CB_mouse stat=%d\n",Brw_stat);


  return FALSE;  // do defaultOperations: allow selection
  // return TRUE;  // do no defaultOperations (NO selection !)

}
*/
/*
//============================================================
  gint Brw_CB_key (void *parent, void *event) {
//============================================================
/// callback-function keyIn

// kommt in MS-Win auch aus dem Grafik-window !!!!

    // printf("Brw_CB_key ev=%d key=%d\n",event->type,event->keyval);


    // switch (event->keyval) {

    // case GDK_Shift_L,GDK_Shift_R,GDK_Control_L,GDK_Control_R,GDK_Alt_L,GDK_Alt_R,
         // GDK_Meta_L:GDK_Meta_R:GDK_Escape:GDK_Delete:GDK_KP_Delete:GDK_Return:
         // GDK_Tab:
         // GDK_ISO_Left_Tab:         // shift Tab
         // GDK_Up:GDK_Down:
         // 'a':'A': ..


    return TRUE;  // prevent the default handler from being run  !!!
    // return FALSE;  // continue with defaultHandler !!!
}


//================================================================
  int Brw_Init (GIO_WinTree *winBrw) {
//================================================================
/// \code
///   soll scrolled_win als winBrw.win retournieren !
/// \endcode


  // printf("Brw_Init \n");


  // gtk_box_pack_start (GTK_BOX (parent), scrolled_win, TRUE, TRUE, 0);
  // parent <- box2 !

  // GUI_TreeIT__ (winBrw, Brw_CB_sel, Brw_CB_key, Brw_CB_mouse);
  GUI_TreeIT__ (winBrw, Brw_CB_sel, NULL, Brw_CB_mouse);
  winBrw->stat = OFF;
 
  return 0;

}
*/

//================================================================
  int Brw_Clear () {
//================================================================

  if(winBrStat < 1) return 0;

  GUI_tree1_clear (&winBrw);

  return 0;

}


//================================================================
  int Brw_Mdl_del_sm (char *txt) {
//================================================================
// delete submodel in browser
 
  TreeNode it1;

  printf("Brw_Mdl_del_sm |%s|\n",txt);

  GUI_tree1_iter_string (&it1, txt, NULL, &winBrw);

  GUI_tree1_remove__ (&winBrw, &it1);

  return 0;

}


//================================================================
  int Brw_Mdl_upd () {
//================================================================
/// \code
/// mainNode: clear tree and update;
/// else      update tree.
/// Input:
///   parNd    mainNode or parentNode to be updated.
/// \endcode

  int         i1;
  TreeNode it1, *parNd;


  //----------------------------------------------------------------
  // printf("Brw_Mdl_upd %d %d\n",AP_stat.sysStat,winBrStat);

  if(AP_stat.sysStat < 2) return 0;
  if(winBrStat < 1) return 0;


  parNd = &mdlNod;
    // printf(" parNd=%p\n",parNd);


  L_sub:

  //----------------------------------------------------------------
  Brw_typeRow_upd (&i1, &it1, Typ_Model, parNd);        // modelReferences
  Brw_typeRow_upd (&i1, &it1, Typ_VAR, parNd);
  Brw_typeRow_upd (&i1, &it1, Typ_VC, parNd);
  Brw_typeRow_upd (&i1, &it1, Typ_PT, parNd);
  Brw_typeRow_upd (&i1, &it1, Typ_LN, parNd);
  Brw_typeRow_upd (&i1, &it1, Typ_CI, parNd);
  Brw_typeRow_upd (&i1, &it1, Typ_CV, parNd);
  Brw_typeRow_upd (&i1, &it1, Typ_PLN, parNd);
  Brw_typeRow_upd (&i1, &it1, Typ_SUR, parNd);
  Brw_typeRow_upd (&i1, &it1, Typ_SOL, parNd);
  Brw_typeRow_upd (&i1, &it1, Typ_GTXT, parNd);


  //----------------------------------------------------------------
  // change the primary node to active
  GUI_tree1_row_set (&winBrw, parNd, -1, NULL, 1);

  // expand the Parentnode
  GUI_tree1_expand__ (&winBrw, parNd);
 
  // // deselect new node
  // GUI_Tree_unselect_all (winBrw);

  return 0;

}

//================================================================
  int Brw_Prcs_Init  () {
//================================================================
/// create all processes

  int           i1, i2;
  char          s1[256];
  Memspc        mSpc1;
  UtxTab_NEW    (sTab1);

  // printf("Brw_Prcs_Init \n");

  // test if processes exist
  sprintf(s1, "%sprocesses.lst",OS_get_tmp_dir());
  if(OS_checkFilExist(s1,1) == 0) return 0;

  // get file <tmp>processes.lst into textTable
  UME_init (&mSpc1, memspc55, sizeof(memspc55));
  UtxTab_init_Memspc (&sTab1, &mSpc1);
  i1 = UtxTab_add_file (&sTab1, s1);
  if(i1 < 0) return -1;


  // add all processes as primary level-objects
  i2 = UtxTab_nr(&sTab1);
  for(i1=0; i1<i2; ++i1) {
    strcpy (s1, UtxTab__(i1, &sTab1));
    //   process_
    //   012345678
    Brw_Prcs_add (&s1[8]);
  }

  return 0;

}
  

//================================================================
  int Brw_Prcs_add (char *pNam) {
//================================================================
 
  int           ico, mode;
  TreeNode      it1;


  // printf("Brw_Prcs_add |%s|\n",pNam);

  ico = ICO_proc;
  mode = 1;

  GUI_tree1_row_add (&it1, &winBrw, NULL, ico, pNam, mode);

  return 0;

}


//================================================================
  int Brw_Prcs_upd (char *pNam, int mode) {
//================================================================
// activate/disactivate process in browser
// mode      1=activate, -1=disactivate
// see Brw_sMdl_act

  // int           ico;
  TreeNode      it1;


  printf("Brw_Prcs_upd |%s| %d\n",pNam,mode);

  // GUI_tree1_iter_string (&it1, pNam, NULL, &winBrw);
  // GUI_tree1_row_set (&winBrw, &it1, -1, NULL, mode);

  return 0;

}



//================================================================
  int Brw_Prcs_del (char *pNam) {
//================================================================

  TreeNode      it1;


  printf("Brw_Prcs_del |%s|\n",pNam);

  GUI_tree1_iter_string (&it1, pNam, NULL, &winBrw);

  GUI_tree1_remove__ (&winBrw, &it1);

  return 0;

}


//================================================================
  int Brw_sMdl_Init  () {
//================================================================
/// create all subModelNodes from Mod.lst

  char          cbuf[256];
  FILE          *fp1;
  TreeNode      it1;


  // printf("Brw_sMdl_Init \n");


  // open
  sprintf(cbuf,"%sMod.lst",OS_get_tmp_dir());
  if((fp1=fopen(cbuf,"r")) == NULL) return 0;


  // // skip first line ("-main-")
  // if (fgets (cbuf, 256, fp1) == NULL) goto L_done;


  // read Submodels
  // loop tru ../tmp/Mod.lst
  while (!feof (fp1)) {
    if (fgets (cbuf, 256, fp1) == NULL) break;
    UTX_CleanCR (cbuf);   // in cbuf ist nun Modename
      // printf(" - Brw_sMdl_Init |%s|\n",cbuf);

    if(!strcmp(cbuf, "-main-")) continue;  // skip main

    // create new row
    Brw_Mdl_createRow (&it1,  cbuf);
  }


  L_done:
  fclose(fp1);



  return 0;

}
 

//================================================================
  int Brw_Mdl_createRow (TreeNode *chdNd, char *mdlNam) {
//================================================================
/// create new (sub)ModelRow

  int           ico, mode;
  char          s1[96];


  // printf("Brw_Mdl_createRow |%s|\n",mdlNam);

  if(strlen(mdlNam) > 80) {
    TX_Print("**** modelname > 80 chars rejected -");
    strncpy(s1, mdlNam, 80);
    s1[80] = '\0';
    strcat(s1, "...");
    TX_Print(s1);
    return -1;
  }


  ico = ICO_natM;
  mode = -1;


    // create new row
    // GUI_TreeIT_row_create (chdNd, &winBrw, NULL);
    GUI_tree1_row_add (chdNd, &winBrw, NULL,
                       ico, mdlNam, mode);

    // GUI_Tree_remove (winBrw, &it1);

    // // set icon & text
    // GUI_tree1_row_set (&winBrw, chdNd, ico, mdlNam, mode);

  return 0;

}


//================================================================
  int Brw_Mod_add (char *mdlNam) {
//================================================================
/// see also Brw_sMdl_act       
 
  int           i1;
  TreeNode      it1;


  // printf("Brw_Mod_add |%s|\n",mdlNam);

  // text if model already exists
  i1 = GUI_tree1_iter_string (&it1, mdlNam, NULL, &winBrw);
  if(i1 >= 0) {
    sprintf(rowTxt, "**** subModel %s overwritten.",mdlNam);
    // GUI_MsgBox (rowTxt);
    TX_Print(rowTxt);
    return 0;
  }


  // add new row
  Brw_Mdl_createRow (&it1, mdlNam);

  return 0;

}


//================================================================
  int Brw_objRow_upd (int typ, long ind) {
//================================================================
/// \code
/// update a objRow (add obj to row; create row if it does not exist)
///  test if typeRow expanded
///    if yes: create new objRow;
///    if not: do nothing ..
/// \endcode

  int          irc, bti, ico;
  char         cbuf[32], *cp1;
  TreeNode     itTr, itOr;

  // printf("Brw_objRow_upd %d %ld\n",typ,ind);

  if(winBrStat < 1) return 0;

  // update typeRow;
  irc = Brw_typeRow_upd (&bti, &itTr, typ, &mdlNod);
  if(irc < 0) return 0;  // row removed


  //----------------------------------------------------------------
  // ab hier gleich wie in Brw_hili_dli

  Brw_ope = 0;  // skip autoselect in Brw_CB_sel

  
  // test if Parentnode is expanded
  irc = GUI_tree1_ck_expanded (&winBrw, &mdlNod);
  if(irc == 0) {
    GUI_tree1_row_set (&winBrw, &mdlNod, -1, NULL, 2);
    goto L_done;
    // is selected after expansion; no further selection possible. 2010-02-15
    // GUI_Tree_expand_row (winBrw, &mdlNod);
    // irc = GUI_Tree_ck_expanded (winBrw, &mdlNod);
    // if(irc == 0) return 0;   // no rows in Parentnode
 }


  // test if typeRow is expanded
  irc = GUI_tree1_ck_expanded (&winBrw, &itTr);
  // 0 = not expanded: hilite row, done.
    // printf(" expanded=%d\n",irc);
  if(irc == 0) {
    GUI_tree1_row_set (&winBrw, &itTr, -1, NULL, 2);
    goto L_done;
  }


  // test if this is a new obj
  APED_oid_dbo__ (cbuf, typ, ind);

  irc = GUI_tree1_iter_string (&itOr, cbuf, &itTr, &winBrw);
    // printf(" n _iter_string %d |%s|\n",irc,cbuf);
  if(irc < 0) {
    // create new objRow
    // GUI_TreeIT_row_create (&itOr, winBrw, &itTr);
    // GUI_TreeIT_row_set (winBrw, &itOr, bti, cbuf, 1);
    GUI_tree1_row_add (&itOr, &winBrw, &itTr, bti, cbuf, 1);
  }

  irc = GUI_tree1_row_set (&winBrw, &itOr, -1, NULL, 2);

  L_done:
  Brw_ope = 1;

  return 0;

}


//================================================================
  int Brw_typeRow_upd (int *bti, TreeNode *itTr,
                       int typ,  TreeNode *parNd) {
//================================================================
/// \code
/// update a TypeRow
/// - create row if it does not exist
/// - fix nr of objs for this typ.
/// Input:
///   typ
///   parNd     parent-node (model)
/// Output:
///   bti       index into typTxt, equals enums TT_* and ICO_*
///   itTr      typeRow
///   RetCod    -1   row has been removed !
///             -2   error
/// \endcode

  int           ico, mode, iNr, iCre;
  char          cbuf[80], *cp1;


  // printf("Brw_typeRow_upd %d\n",typ);
  // printf(" parNd=%p\n",parNd);


  // nr of objects for this typeRow
  iNr = DB_get_ObjNr (typ);
    // printf("  typ=%d _ObjNr=%d\n",typ,iNr);


  // find IconTypeIndex for type. (eg ICO_PT for Typ_PT)
  *bti = Ico_icoNr_typ (typ);         // is also IconNr
  if(*bti < 0) return -2;
  cp1 = typTxt[*bti];                 // local
    // printf(" typtxt=|%s| bti=%d\n",cp1,*bti);
  

  // create/modify/remove ?

  
  // find rowChd starting with s1; create if necessary
  iCre = GUI_tree1_iter_string (itTr, cp1, parNd, &winBrw);
    // printf("  ex _tree1_iter_string %d\n",iCre);

  if(iCre < 0) {
    if(iNr <= 0) return 0;
    ico = *bti;
    // mode = 1;
  } else {
    // mode=-2=not modify icon & mode ..
    ico = -1;  // -1=do not modify
  }
  
    mode = 1; // -2=do not modify; 1 =active

  // remove
  if(iNr <= 0) {
    // GUI_Tree_remove (winBrw, itTr);
    GUI_tree1_remove__ (&winBrw, itTr);
    return -1; 
  }


  // modify.
  // fix text
  sprintf(cbuf, "%s%d", cp1, iNr);
    // printf(" _upd-rowTxt=|%s|\n",cbuf);


  if(iCre < 0) {
    // create new row
    GUI_tree1_row_add (itTr, &winBrw, parNd, ico, cbuf, mode);
    // GUI_TreeIT_row_create (itTr, winBrw, parNd);
  } else {
    // modify existing row
    GUI_tree1_row_set (&winBrw, itTr, ico, cbuf, mode);
  }



  return 0;

}


//================================================================
  int Brw_typeRow_sel (char *txt) {
//================================================================
/// \code
/// open or close TypeRow
///   open row:  create & display objectRows;
///   close row: remove objectRows;
/// \endcode

  int    iTyp, i1;
  char   *cp1;


  printf("Brw_typeRow_sel |%s|\n",txt);


  // get objectTyp from TypeRowText
  // extract first word
  cp1 = strchr(txt, ' ');
  if(cp1 == NULL) {TX_Error("Brw_typeRow_sel E001"); return -1;}
  *cp1 = '\0';

  if(!strcmp (txt, "Models")) {
    iTyp = Typ_Model;

  } else if(!strcmp (txt, "Values")) {
    iTyp = Typ_VAR;

  } else if(!strcmp (txt, "Vectors")) {
    iTyp = Typ_VC;

  } else if(!strcmp (txt, "Points")) {
    iTyp = Typ_PT;

  } else if(!strcmp (txt, "Lines")) {
    iTyp = Typ_LN;

  } else if(!strcmp (txt, "Circles")) {
    iTyp = Typ_CI;

  } else if(!strcmp (txt, "Curves")) {
    iTyp = Typ_CV;

  } else if(!strcmp (txt, "Planes")) {
    iTyp = Typ_PLN;

  } else if(!strcmp (txt, "Surfaces")) {
    iTyp = Typ_SUR;

  } else if(!strcmp (txt, "Solids")) {
    iTyp = Typ_SOL;

  } else if(!strcmp (txt, "Notes")) {
    iTyp = Typ_GTXT;

  } else {
    TX_Error("Brw_typeRow_sel E001 |%s|",txt);
    return  -1;
  }


  //----------------------------------------------------------------
  // test if childs already exist
  i1 = GUI_tree1_childNr (&winBrw, &actNod);
    printf(" _childNr-i1 = %d\n",i1);
  if(i1 <= 0) goto L_add;

  // remove all childs
  GUI_tree1_childs_remove (&winBrw, &actNod);

  return 0;



  //----------------------------------------------------------------
  // create all childs
  L_add:
  Brw_typeRow_open (iTyp);

  // expand
  GUI_tree1_expand__ (&winBrw, &actNod);

  return 0;

}


//================================================================
  int Brw_typeRow_open (int typ) {
//================================================================
/// \code
/// open (expand) a typeRow; create all objs for this type.
/// the node of the typeRow = actNod = Input !
/// \endcode
 
  int         irc, i1, ico;
  long        ind, l1, l2;
  char        *cp1, cbuf[80];

  double      *vra;
  ModelRef    *mra;
  ModelBas    *mb;
  Vector      *vca;
  Point       *pta;
  Line        *lna;
  Circ        *cia;
  Plane       *pla;
  ObjGX       *cva, *sua, *soa, *gta;

  TreeNode    it1;


  // printf("Brw_typeRow_open %d\n",typ);

    
  switch (typ) {

    //----------------------------------------------------------------
    case Typ_Model:
      ico = ICO_refM;
      DB_Query__ (&mra, &l1, &l2, typ);  // startAdress of table
        // printf(" mra: %p %d %d\n",mra,l1,l2);

      for(i1=1; i1 <= l1; ++i1) {
        irc = DB_mdlNam_iRef (&cp1, i1);
        if(irc < 0) continue;
          // printf("M%d = %s\n",i1,cp1);
        sprintf(cbuf, "M%d %s",i1,cp1);
        // get basicModel
        mb = DB_get_ModBas (mra[i1].modNr);
        if(mb->typ >= MBTYP_EXTERN)       ico = ICO_extM;
        else if(mb->typ == MBTYP_INTERN)  ico = ICO_refM;
        else if(mb->typ == MBTYP_CATALOG) ico = ICO_libM;
        // create primary node
        GUI_tree1_row_add (&it1, &winBrw, &actNod, ico, cbuf, 1);
      }

      break;

         
    //----------------------------------------------------------------
    case Typ_VAR:
      ico = ICO_Var;
      DB_Query__ (&vra, &l1, &l2, typ);
      for(l2=1; l2<=l1; ++l2) {
        if(DB_isFree_Var (&vra[l2])) continue;
        Brw_objRow_add (typ, l2, ico);
      }
      break;

    //----------------------------------------------------------------
    case Typ_VC:
      ico = ICO_VC;
      DB_Query__ (&vca, &l1, &l2, typ);
      for(l2=1; l2<=l1; ++l2) {
        if(DB_isFree_VC (&vca[l2])) continue;
        Brw_objRow_add (typ, l2, ico);
      }
      break;


    //----------------------------------------------------------------
    case Typ_PT:
      ico = ICO_PT;
      DB_Query__ (&pta, &l1, &l2, typ);
      for(l2=1; l2<=l1; ++l2) {
        if(DB_isFree_PT (&pta[l2])) continue;
        Brw_objRow_add (typ, l2, ico);
      }
      break;


    //----------------------------------------------------------------
     case Typ_LN:
      ico = ICO_LN;
      DB_Query__ (&lna, &l1, &l2, typ);
      for(l2=1; l2<=l1; ++l2) {
        if(DB_isFree_LN (&lna[l2])) continue;
        Brw_objRow_add (typ, l2, ico);
      }
      break;

    //----------------------------------------------------------------
    case Typ_CI:
      ico = ICO_CI;
      DB_Query__ (&cia, &l1, &l2, typ);
      for(l2=1; l2<=l1; ++l2) {
        if(DB_isFree_CI (&cia[l2])) continue;
        Brw_objRow_add (typ, l2, ico);
      }
      break;

    //----------------------------------------------------------------
    case Typ_CV:
      ico = ICO_CV;
      DB_Query__ (&cva, &l1, &l2, typ);
      for(l2=1; l2<=l1; ++l2) {
        if(DB_isFree_CV (&cva[l2])) continue;
        Brw_objRow_add (typ, l2, ico);
      }
      break;

    //----------------------------------------------------------------
    case Typ_PLN:
      ico = ICO_PLN;
      DB_Query__ (&pla, &l1, &l2, typ);
      for(l2=1; l2<=l1; ++l2) {
        if(DB_isFree_PLN (&pla[l2])) continue;
        Brw_objRow_add (typ, l2, ico);
      }
      break;

    //----------------------------------------------------------------
    case Typ_SUR:
      ico = ICO_SUR;
      DB_Query__ (&sua, &l1, &l2, typ);
      for(l2=1; l2<=l1; ++l2) {
        if(DB_isFree_Sur (&sua[l2])) continue;
        Brw_objRow_add (typ, l2, ico);
      }
      break;

    //----------------------------------------------------------------
    case Typ_SOL:
      ico = ICO_SOL;
      DB_Query__ (&soa, &l1, &l2, typ);
      for(l2=1; l2<=l1; ++l2) {
        if(DB_isFree_Sol (&soa[l2])) continue;
        Brw_objRow_add (typ, l2, ico);
      }
      break;

    //----------------------------------------------------------------
    case Typ_GTXT:
      ico = ICO_Img;
      DB_Query__ (&gta, &l1, &l2, typ);
      for(l2=1; l2<=l1; ++l2) {
        if(DB_isFree_GTxt (&gta[l2])) continue;
        Brw_objRow_add (typ, l2, ico);
      }
      break;


    //----------------------------------------------------------------
    default:
      TX_Error("Brw_typeRow_open E001 %d",typ);
      return -1;
  }

  return 0;

}

  
//================================================================
  int Brw_objRow_add (int typ, long dbi, int ico) {
//================================================================
/// add objRow to actNod
/// Input:
///   mode    0=active, -1=deleted
 
  
  int         mode, ll, ln;
  long        dli, lNr;
  char        oid[80], *ps, *pn, *p1;
  TreeNode    it1;


  // printf("Brw_objRow_add %d %ld %d\n",typ,dbi,ico);


  // test if sourceline exists
  // mode = APED_find_dbo (&dli, &lNr, typ, dbi);

  // create obj-ID
  APED_oid_dbo__ (oid, typ, dbi);
    // printf(" Brw_objRow_add oid=|%s|\n",oid);


  // search def.Ln.
  mode = APED_search_defLn (&ps, &lNr, &ll, oid, -1L, 0);
  if(mode < 0) {
    printf("***** Brw_objRow_add ERR |%s|\n",oid);
    return -1;
  }


  pn = APED_onam_search (ps, &ps[ll]);
  if(pn) {
    p1 = &oid[strlen(oid)];
    *p1 = ' ';
    ++p1;
    // add objName to oid
    ln = &ps[ll] - pn;
      // printf(" ln=%d\n",ln);
    ln = IMIN (ln, 60);
    strncpy (p1, pn, ln);
    p1[ln] = '\0';
  }

  if(mode == 0) mode = 1;      // active=1;



  // mode: -1=dimmed; 1=active
  GUI_tree1_row_add (&it1, &winBrw, &actNod, ico, oid, mode);

  
  return 0;

}


//================================================================
  int Brw_load_object1 (int typ, long ind, int ico) {
//================================================================
/// add objRow to actNod
 
  char        cbuf[32];
  TreeNode    it1;


  // printf("Brw_load_object1 %d %ld %d\n",typ,ind,ico);


  APED_oid_dbo__ (cbuf, typ, ind);

  // GUI_TreeIT_row_create (&it1, winBrw, &actNod);
  // GUI_TreeIT_row_set (winBrw, &it1, ico, cbuf, 1);
  GUI_tree1_row_add (&it1, &winBrw, &actNod, ico, cbuf, 1);


  return 0;

}


//================================================================
  int Brw_hili_dli (long dli) {
//================================================================
/// \code
/// Brw_hili_dli            hilite row  (CB M1-selection in GR-Win)
/// see also Brw_objRow_upd !
/// \endcode


  int          irc, typ, ii;
  // long         ind;
  char         cbuf[80], *cp1;
  TreeNode     itTr, itOr;


  // printf("Brw_hili_dli %ld\n",dli);


  // exit if no Brw
  if(winBrStat < 1) return 0;


  typ = DL_GetTyp (dli);
  // ind = DL_get_dbi (dli);
    // printf(" typ=%d ind=%ld\n",typ,ind);



  // dynamic obj selected: unselect all
  // if(ind < 0) {
    GUI_tree1_unselect_all (&winBrw);
    // return -1;
  // }


  // find typeRow for type.
  ii = Ico_icoNr_typ (typ);
  if(ii < 0) return -1;
  cp1 = typTxt[ii];
    // printf(" typtxt=|%s| ii=%d\n",cp1,ii);


  // get typeRow
  irc = GUI_tree1_iter_string (&itTr, cp1, &mdlNod, &winBrw);
  if(irc < 0) {printf("Brw_hili_dli I001\n"); return 0;}  // typeRow exists not ?


  // test if typeRow is expanded
  irc = GUI_tree1_ck_expanded (&winBrw, &itTr);
  // 0 = not expanded: hilite row, done.
  if(irc == 0) {itOr = itTr; goto L_hili;}


  // create string "<objID>="  e.g. "M20" from objInd
  DL_Get_oid_dli (cbuf, dli);
    // printf(" obj=|%s|\n",cbuf);


  // get iter of objIdString 
  irc = GUI_tree1_iter_string (&itOr, cbuf, &itTr, &winBrw);
  if(irc < 0) {    // objRow exists not
    printf("Brw_hili_dli I002\n");
    // hilte the typeRow, not the objRow.
    itOr = itTr;
  }



  // select row from iter
  L_hili:
  irc = GUI_tree1_row_set (&winBrw, &itOr, -1, NULL, 2);

  return irc;

}


//================================================================
  int Brw_close_typeRows (TreeNode *parNd) {
//================================================================
/// \code
/// remove all childs of typeRows != "Models"
/// loop tru all childs of parNd; get its text; if it is "Models": continue;
/// else remove all childs of this node !
/// \endcode

  int              i1, i2, ii, irc;
  char             cbuf[256];
/*
  GtkTreeIter      chdNd;
  GtkTreeModel     *model;
  gchar            *txt1;
  GtkTreeView      *view;
*/

  // printf("Brw_close_typeRows \n");

  GUI_tree1_childs_remove (&winBrw, parNd);


/* 2012-07-30
  // model = GTK_TREE_MODEL(winBrw.store);

  view  = GTK_TREE_VIEW (winBrw);
  model = gtk_tree_view_get_model (view);


  // loop tru all childs of parNd
  // get nr of childs of parNod
  ii = gtk_tree_model_iter_n_children (model, parNd);

  // loop tru childs
  for(i1=0; i1<ii; ++i1) {

    // get childnode
    irc = gtk_tree_model_iter_nth_child (model, &chdNd, parNd, i1);
      // printf(" irc=%d\n",irc);
    if(irc == 0) continue;

    // get value of iter of column 1
    gtk_tree_model_get (model, &chdNd, 1, &txt1, -1);
    strcpy(cbuf, txt1);
    g_free (txt1);
      // printf(" pass.typ|%c|\n",c1);

    if(!strncmp(cbuf, "Models ", 7)) continue;

    // remove childs 
    GUI_Tree_childs_remove (winBrw, &chdNd);
  }
*/

  return 0;

}


//================================================================
  int Brw_childs_passive (TreeNode *parNd) {
//================================================================
/// make parents-childs !=models passive, "M"-nodes normal.


  // printf("Brw_childs_passive \n");

  // make the parent itself normal
  GUI_tree1_row_set (&winBrw, parNd, -1, NULL, 0);

  // make all childs passive
  GUI_tree1_childs_set (&winBrw, parNd, -1);

  return 0;

}


//================================================================
  int  Brw_Mod_is_main_active () {
//================================================================

  return GUI_tree1_cmp_row (&winBrw, &topNod, &mdlNod);

}


//================================================================
  int Brw_sMdl_act (char *mdlNam) {
//================================================================
/// \code
/// activate subModel in Browser
/// add SM if it does not exist
/// \endcode
 
  int           i1;
  TreeNode      it1;


  // printf("Brw_sMdl_act |%s|\n",mdlNam);

  if(winBrStat < 1) return 0;

  // remove all objRows
  Brw_close_typeRows (&mdlNod);


  //----------------------------------------------------------------
  // main: disactivate subModels, then activate main.
  if(strcmp(mdlNam, "-main-")) goto L_subModel;

  if(!Brw_Mod_is_main_active()) return 0;

  // disactivate subModel mdlNod & its childs
  GUI_tree1_childs_set (&winBrw, &mdlNod, -1);
  GUI_tree1_row_set (&winBrw, &mdlNod, -1, NULL, -1);


  // activate the node topNod  & its childs
  GUI_tree1_childs_set (&winBrw, &topNod, 1);
  GUI_tree1_row_set (&winBrw, &topNod, -1, NULL, 1);

  mdlNod = topNod;

  // test nr of childs of this node; (open node after enter brw from subModel)
  i1 = GUI_tree1_childNr (&winBrw, &mdlNod);
  if(i1 < 1) Brw_Mdl_upd ();


  return 0;



  //----------------------------------------------------------------
  // subModel:
  L_subModel:
  // disactivate main & its childs
  GUI_tree1_childs_set (&winBrw, &mdlNod, -1);
  GUI_tree1_row_set (&winBrw, &mdlNod, -1, NULL, -1);


  // find subModel; create if necessary
  i1 = GUI_tree1_iter_string (&it1, mdlNam, NULL, &winBrw);
  if(i1 < 0) {
    Brw_Mdl_createRow (&it1, mdlNam);
  }

  // activate new subModel <mdlNam>
  mdlNod = it1;
  Brw_Mdl_upd ();

  // expand
  GUI_tree1_expand__ (&winBrw, &mdlNod);

  return 0;

}


//================================================================
  int Brw_unselect_all () {
//================================================================
 
  // return GUI_Tree_unselect_all (winBrw);
  return GUI_tree1_unselect_all (&winBrw);

}


//================================================================
  int Brw_obj_mod (int typ, long dbi, int mode) {
//================================================================
// Brw_obj_mod         modify object-row (active or deleted)
// Input:
//   mode  1=normal, -1=passive
// RetCod:
//   -1    TypeRow does not exist
//    0    TypeRow is not open

  int         bti, i1;
  char        *cp1, objNam[32];
  TreeNode    itP, itC;


  // printf("Brw_obj_mod %d %ld %d\n",typ,dbi,mode);


  // get parentNode for typ
  bti = Ico_icoNr_typ (typ);         // is also IconNr
  if(bti < 0) return -1;             // no icon exists = typ is not supported

  cp1 = typTxt[bti];
    // printf(" typtxt |%s|\n",cp1);


  i1 = GUI_tree1_iter_string (&itP, cp1, &mdlNod, &winBrw);
  if(i1 < 0) return -1;

  // test if node is open
  i1 = GUI_tree1_childNr (&winBrw, &itP);
  if(i1 < 1) return 0;


  // get objRow
  APED_oid_dbo__ (objNam, typ, dbi);
  // strcat(objNam, " ");
    // printf(" objNam=|%s|\n",objNam);

  i1 = GUI_tree1_iter_string (&itC, objNam, &itP, &winBrw);
  if(i1 < 0) return -2;


  // change akt|pass
  GUI_tree1_row_set (&winBrw, &itC, -1, NULL, mode);


  return i1;

}


//================================================================
  int Brw_obj_upd (int typ, long dbi, int mode) {
//================================================================
// update object-row (active or deleted)
// Input:
//   oSrc     sourceline
//   mode      0=Restore; 1=Delete


  // printf("Brw_obj_upd %d %ld %d\n",typ,dbi,mode);

  if(winBrStat < 1) return 0;

  if(mode == 1) mode = -1;
  if(mode == 0) mode = 1;

  Brw_obj_mod (typ, dbi, mode);

  return 0;

}
 

//====================== EOF ===========================
