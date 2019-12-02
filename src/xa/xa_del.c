// delete object                                  2009-11-06     RF
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
- Funktion : free delTab (call it in File/Open, File/New ..)
- can objects be doubly defined in delTab ? testmodel missing!

-----------------------------------------------------
Modifications:
2009-11-06 Created. RF.

-----------------------------------------------------
*/
#ifdef globTag
void Del(){}
#endif
/*!
\file  ../xa/xa_del.c
\brief delete object 
\code
=====================================================
List_functions_start:

Del_init       create & display list of all depending objs
Del_Win__      open Window; display List of objs
Del_Win_upd    populate window
Del_Win_add    populate window
Del_hili       hilite all objects-to-delete
Del_CB_sel     selection in tree-view; unused
Del_CB_del     delete one or all objs of List

Del_obj__      delete obj
Del_grp__      delete all objs in group
Del_tab__      delete objects

Del_dump       dump delTab
Del_free       free delTab   NOT YET IMPLEM.
Del_obj_add    add oRec to oTab if it does not alrey exist

List_functions_end:
=====================================================

\endcode *//*----------------------------------------




==========================================
OLD VERSION:
  AP_SMD_init0 ();
  AP_SMD_init1 (oNam);
  AP_SMD_del ();


*/



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_memTab.h"           // MemTab..
#include "../ut/ut_cast.h"             // INT_PTR

#include "../gui/gui__.h"              // Gtk3

#include "../ut/func_types.h"               // Typ_Att_hili

#include "../db/ut_DB.h"               // DB_

#include "../xa/xa_ed_mem.h"           // ObjSRC
#include "../xa/xa_mem.h"              // memspc51, mem_cbuf1
#include "../xa/xa_ui.h"               // UI_MODE_CAD APF_TB_CAD
#include "../xa/xa_brw.h"              // ICO_PT,


// typedef struct {long dbi, dli, lnr; short typ, iPar;}          stru_Del;
// typedef_MemTab(stru_Del);



// extern vars:


// local vars:
// static GIO_WinTree winDel;
static void *winDel;
// static MemTab(stru_Del) delTab = _MEMTAB_NUL;

static MemTab(ObjSRC) delTab = _MEMTAB_NUL;


// prototypes:
  // void Del_CB_sel (GtkTreeSelection *selection, gpointer data);
  // void Del_CB_del (void *parent, void *data);



/*
//================================================================
  gint Del_EdKeyPress (void *parent, void *event) {
//================================================================
// callback from keys in SurfacColorWindow

  Del_key_CB (((GdkEventKey*)event)->keyval);

  return TRUE;  // prevent the default handler from being run

}


//================================================================
  int Del_key_CB (int key) {
//================================================================
// used by SurMod_EdKeyPress & Main-keystroke-dispatcher


  printf("Del_key_CB %d\n",key);
  // printf("  PED_stat=%d actInd=%d\n",PED_stat,actInd);


  switch (key) {


    case 'q':
    case 's':
    case GDK_Escape:
      Del_Win__ (NULL, (void*)UI_FuncExit);         // init window
      return TRUE;  // do no defaultOperations

    case GDK_Return:
      Del_CB_del (NULL, NULL);                     // OK- delete.
      return TRUE;  // do no defaultOperations

    default:
      break;
  }

  return FALSE;  // do defaultOperations

}


//=========================================================
  int Del_sel_CB (int src, long dli) {
//=========================================================
// userselection callback

  int    irc, i1, styp, typ;
  long   dbi;
  char   snam[64];
  // Point  pt1;


  printf("Del_sel_CB src=%d ind=%ld\n",src,dli);


  // save &exit m. rechter maustaste (see also PED_key_CB)
  if(src == GUI_MouseR) {
    Del_CB_del (NULL, NULL);                     // OK- delete.
    return 0;
  }

  return 0;

}
*/
/*
//================================================================
  int Del_init (int typ, long dbi, long dli) {
//================================================================
/// Del_init       create & display list of all depending objs
/// UNUSED

  long      ld, lnr;
  char      s1[256], oNam[32];
  ObjSRC  o1;


  printf("Del_init %d %ld %ld\n",typ,dbi,dli);

 
  // search & add all depending objects
  APED_find_dep__ (&delTab, typ, dbi);
    // printf(" _find %d\n",delTab.rNr);

  if(delTab.rNr < 1) {
    TX_Print("ERROR: Obj %d %d not found ..",typ,dbi);
    return -1;
  }


  if(delTab.rNr < 2) {    // delete immediate
    Del_CB_del (NULL, NULL);
    return 0;
  }


  UI_func_stat_set__ (-APF_TB_CAD,
                      -APF_MEN0,
                      -APF_HIDE,
                      -APF_VWCADMAN,
                      -APF_BUT_GSE,
                      -APF_PLUGINS,
                      -APF_UNDO,
                      0);

  AP_UserKeyIn_get     (Del_key_CB);         // attach keystrokes
  AP_UserSelection_get (Del_sel_CB);         // attach grafic selections

  DL_pick__ (0);               // make all objects unpickable


  // Del_dump ();   // dump delTab




  // hilite all objects-to-deleted
  Del_hili (&delTab);


  // display objects-to-delete in treeView
  APED_oid_dbo__ (oNam, typ, dbi);
  sprintf(s1, "%s has %d depending objects -",oNam,delTab.rNr - 1);
  Del_Win__ ((void*)s1, (void*)UI_FuncInit);         // init window
  Del_Win_upd (&delTab);                             // populate window


  return 0;
}
*/
/*
//================================================================
  int Del_Win__ (void *parent, void *data) {
//================================================================
/// Del_Win__      open Window
/// UNUSED

  static void *win0=NULL;

  int       i1;
  void *box0, *wact;


  printf("Del_Win__ %d\n",INT_PTR(data));

  i1 = INT_PTR(data);

  switch (i1) {


    //---------------------------------------------------------
    case UI_FuncInit:

      if(win0 != NULL) {           // Win schon vorhanden ?
        gtk_widget_destroy (win0);
      }

      win0 = GUI_Window ("Delete obj",Del_Win__,(void*)UI_FuncKill,0);
      box0 = GUI_Vbox (win0, 0);

      GUI_Label_2 (box0, (void*)parent, -1, 5);


      GUI_TreeIT__ (&winDel, Del_CB_sel, NULL, NULL);
      gtk_box_pack_start (GTK_BOX (box0), winDel.win, TRUE, TRUE, 1);
      gtk_widget_show (winDel.win);
      gtk_widget_set_usize (winDel.win, 180, 100);          
      winDel.stat = 1;

      GUI_Butt (box0, " delete all ", Del_CB_del, NULL, 5);
      // gtk_widget_set_uposition (win0, 0, 0);  // Pos of mainwin
      gtk_widget_set_events (win0, 
                             GDK_KEY_PRESS_MASK);
      gtk_signal_connect (GTK_OBJECT(win0),"key_press_event",  // GTK2
                           GTK_SIGNAL_FUNC(Del_EdKeyPress), NULL);

      GUI_WinUp (NULL, win0);
      gtk_widget_show (win0);
      break;


    //---------------------------------------------------------
    case UI_FuncExit:  // 102
      // send destroy-signal
      // if(win0) gtk_signal_emit_by_name (GTK_OBJECT(win0),"destroy");
      // return 0;



    //---------------------------------------------------------
    case UI_FuncKill:  // 99
      // unhilite all
      DL_hili_off (-1L);

      // restore ..
      UI_func_stat_set__ (APF_TB_CAD,
                      APF_MEN0,
                      APF_HIDE,
                      APF_VWCADMAN,
                      APF_BUT_GSE,
                      APF_PLUGINS,
                      APF_UNDO,
                      0);
      AP_User_reset ();               // reset keyBd & selections ...
      DL_pick__ (1);                  // reset selectionfilter

      gtk_widget_destroy (win0);
      win0 = NULL;

      // // disactiv. S/M
      // UI_AP (UI_FuncSet, UID_ckb_search, (void*)FALSE);

  }

  return 0;

}

//================================================================
  int Del_Win_upd (MemTab(ObjSRC) *oa) {
//================================================================
/// populate window


  Del_Win_add (NULL, -1);

  // expand all
  GUI_Tree_expand_all (winDel);

  return 0;

}

  
//================================================================
  int Del_Win_add (GtkTreeIter *ndPar, int iPar) {
//================================================================
/// populate window

  int         i1, ico, iNr;
  char        oNam[32];
  ObjSRC      *oTab, *o1;

  GtkTreeIter it1;

  
  // printf("Del_Win_add %d\n",iPar);


  MemTab_load ((void**)&oTab, &iNr, (MemTab*)&delTab);


  // loop tru memtab; add next obj with iPar==iNr
  for(i1=0; i1 < iNr; ++i1) {

    o1 = &oTab[i1];

    if(o1->iPar != iPar) continue;

    ico = Ico_icoNr_typ (o1->typ);

    APED_oid_dbo__ (oNam, o1->typ, o1->dbi);

    GUI_TreeIT_row_add (&it1, winDel, ndPar, ico,  oNam, 1);

    // add all childs of record i1 (recurse)
    Del_Win_add (&it1, i1);

  }


  return 0;

}
*/


//================================================================
  int Del_hili (MemTab(ObjSRC) *oa) {
//================================================================
// hilite all objects-to-delete

  int         i1, iNr;
  long        dli;
  char        oNam[32];
  ObjSRC       *oTab, *o1;


  DL_hili_on (-1L);        // loeschen der Hili-Tabelle

  MemTab_load ((void**)&oTab, &iNr, (MemTab*)oa);

  for(i1=0; i1 < iNr; ++i1) {
  
    o1 = &oTab[i1];

    // AP_hili_obj (cPos);
    DL_hili_on (o1->dli);      // hilite obj

  }

  return 0;

}


/* UNUSED
//====================================================================
  void Del_CB_sel (GtkTreeSelection *selection, gpointer data) {
//====================================================================
// Del_CB_sel     selection in tree-view; unused

  char             rowTxt[32];

  GtkTreeView      *view;
  GtkTreeModel     *model;
  GtkTreeIter      itAct;
  GtkTreePath      *tpt;
  gchar            *txt1;

  GtkTreeStore     *store;
  GtkTreeIter      rowChd;


  // printf("Del_CB_sel \n");


  // skip deselection
  if(!gtk_tree_selection_get_selected (selection, &model, &itAct)) {
    return;
  }

  // decode selected row - get data for first column
  // column-0 (icon) gives adress of icon; not useful ..
  gtk_tree_model_get (model, &itAct, 1, &txt1, -1);  // access Spalte 1=text 
  strcpy(rowTxt, txt1);
  g_free (txt1);
    // printf(" sel.txt |%s|\n",rowTxt);


  return;

}
*/
/*
//================================================================
  void Del_CB_del (void *parent, void *data) {
//================================================================
/// \code
/// Del_Del        delete one or all objs of List
/// callback of button "delete"
/// \endcode

  int         i1, iNr;
  int         iDel;     // Anzahl geloeschter Obj.
  long        dli;
  char        oNam[32];
  ObjSRC       *oTab, *o1;

  // printf("Del_CB_del \n");


  MemTab_load ((void**)&oTab, &iNr, (MemTab*)&delTab);

  iDel = 0;

  for(i1=0; i1 < iNr; ++i1) {
    o1 = &oTab[i1];
    UNDO_grp_add (o1->lnr, iDel);    // add to undoTab
    ++iDel;
  }
  UNDO_grp_del ();                 // delete last active grp & update display


  // close window
  Del_Win__ (NULL, (void*)UI_FuncExit);


  return;

}
*/

//================================================================
  int Del_obj_add (MemTab(ObjSRC) *oTab, ObjSRC *oRec) {
//================================================================
// add oRec to oTab if it does not alrey exist.


  int      i1, typ;
  long     dbi, ld;
  ObjSRC   *o1;


  if(!oRec) return -3;    // already deleted !


  // test if record i2 is already in oTab
  typ = oRec->typ;
  dbi = oRec->dbi;


    // printf(" Del_obj_add typ=%d dbi=%d\n",typ,dbi);


  for(i1=0; i1 < oTab->rNr; ++i1) {
    o1 = &(oTab->data[i1]);
    if(o1->typ != typ) continue;
    if(o1->dbi != dbi) continue;
    return -1;        // skip deleting (already in table)
  }

    // printf(" add new obj typ=%d dbi=%d\n",typ,dbi);

  MemTab_sav (oTab, &ld, oRec, 1); // add obj to delete-table

  return 0;

}


//================================================================
  int Del_obj__ (int typ, long dbi) {
//================================================================
// delete obj

  ObjDB    o1;


  o1.typ   = typ;
  o1.dbInd = dbi;

  Del_tab__ (1, &o1);

  return 0;

}

//================================================================
  int Del_grp__ () {
//================================================================
// delete all objs in group

  int      ii;
  ObjDB    *gTab;

  printf("Del_grp__ \n");


  // get group-of-selected-objs
  ii = Grp_get__ (&gTab);
    // printf(" ii=%d\n",ii);

  TX_Print ("- delete %d objects ..",ii);

  Del_tab__ (ii, gTab);

  return 0;

}


//================================================================
  int Del_tab__ (int ii, ObjDB *gTab) {
//================================================================
// Del_tab__      delete objects

  int            i1, i2, i3, iRec, typ, iDel;
  long           dbi, ld;
  char           oNam[32];
  ObjSRC         *o2, *o3;
  MemTab(ObjSRC) dTab = _MEMTAB_NUL;
  MemTab(ObjSRC) oTab = _MEMTAB_NUL;


  // printf("Del_tab__ %d\n",ii);
  // for(i1=0;i1<ii;++i1)DEB_dump_obj__(Typ_Group,&gTab[i1]," gT[%d]\n",i1);


  UI_block__ (1, 1, 1);  // block UI


  // init table for objs to be deleted
  MemTab_ini__ (&oTab, sizeof(ObjSRC), Typ_ObjSRC, 100);


  // loop tru group-of-selected-objs
  for(i1=0; i1<ii; ++i1) {
    typ = AP_typDB_typ (gTab[i1].typ);
    dbi = gTab[i1].dbInd;

    // get list of depending objects
    APED_find_dep__ (&dTab, typ, dbi);
      // printf(" typ=%d dbi=%d dep=%d\n",typ,dbi,dTab.rNr);


      // TESTDISPLAY ONLY:-------------------
      // for(i2=0; i2 < dTab.rNr; ++i2) {
        // o2 = &(dTab.data[i2]);
        // printf(" dTab[%d] %d %d %d %d\n",i2,o2->typ,o2->dbi,o2->lnr,o2->iPar);
        // // APED_oid_dbo__ (oNam, o2->typ, o2->dbi);
        // // printf("[%d] %s par=%d\n",i2,oNam,o2->iPar);
      // }
      // TESTDISPLAY ONLY:-------------------


    // sort depending objects; next obj to be deleted may not have a
    // depending obj !
    L_nxt_i1:
    iDel = 0;
    for(i2=0; i2 < dTab.rNr; ++i2) {
      o2 = &(dTab.data[i2]);
      if(o2->iPar < 0) continue;
        // APED_oid_dbo__ (oNam, o2->typ, o2->dbi);
        // printf("[%d] %s par=%d\n",i2,oNam,o2->iPar);

      // test if an other object has i2 as its parent
      for(i3=0; i3 < dTab.rNr; ++i3) {
        o3 = &(dTab.data[i3]);
        if(o3->iPar == i2) {
            // printf(" %d is parent of %d - skip.\n",i3,i2);
          goto L_nxt_i2;
        }
      }
        // typ = o2->typ; dbi = o2->dbi;
        // printf(" delete next %d typ=%d dbi=%d\n",i2,typ,dbi);
      Del_obj_add (&oTab, o2);   // add obj to delete-table
      L_nxt_i3:
      o2->iPar = -2;
      ++iDel;

      L_nxt_i2:
      continue;
    }
    if(iDel > 0) goto L_nxt_i1;

      // printf(" delete last 0\n");
    Del_obj_add (&oTab, &(dTab.data[0]));  // add obj to delete-table

  }



/*
  // TESTDISPLAY ONLY:-------------------
  // dump oTab
  printf("Delete %d objects:\n",oTab.rNr);
  for(i2=0; i2 < oTab.rNr; ++i2) {
    o2 = &(oTab.data[i2]);
        printf(" oTab[%d] %d %d lnr=%d %d\n",i2,o2->typ,o2->dbi,o2->lnr,o2->iPar);
    // APED_oid_dbo__ (oNam, o2->typ, o2->dbi);
    // printf("[%d] %s lnr=%d\n",i2,oNam,o2->lnr);
  }
  // TESTDISPLAY ONLY:-------------------
*/


  // add all obj's of group to undoTab
  iDel = 0;
  for(i2=0; i2 < oTab.rNr; ++i2) {
    o2 = &(oTab.data[i2]);
      // printf("[%d] typ=%d dbi=%d\n",i2,o2->typ,o2->dbi);
    UNDO_grp_add (o2->lnr, iDel);    // add to undoTab
    ++iDel;
  }

  // get record #=0 of active group (last =iRec
  iRec = UNDO_recNr__ ();

  // delete the active undo-grp & update display
    // UNDO_dump ("Del_tab__");
  UNDO_grp_del (iRec);


  // reset selected-obj-group
  Grp_init ();


  MemTab_free (&dTab);
  MemTab_free (&oTab);

  UI_block__ (0, 0, 0);  // reset UI

  return 0;

}


//================================================================
  int Del_dump () {
//================================================================
/// dump delTab

  int            i1, iNr;
  ObjSRC       *oTab, *o1;


  MemTab_load ((void**)&oTab, &iNr, (MemTab*)&delTab);


  for(i1=0; i1 < iNr; ++i1) {

    o1 = &oTab[i1];

    printf("delTab[%d] typ=%d dbi=%ld dli=%ld lnr=%ld iPar=%d\n",i1,
        o1->typ,o1->dbi,o1->dli,o1->lnr,o1->iPar);
  }


  return 0;

}


// EOF
