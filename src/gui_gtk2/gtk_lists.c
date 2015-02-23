/*
 *
 * Copyright (C) 2012 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
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
- gtk_window_set_title (GTK_WINDOW (UI_list1_win), "MultiColumnList");

-----------------------------------------------------
Modifications:
2012-02-01 extracted from ut_gtk.c.  RF.

-----------------------------------------------------
*/
#ifdef globTag
void GUI_LIST(){}
#endif
/*!
\file ../gui_gtk2/gtk_lists.c
\brief lists with 1 or 2 columns
\code

=====================================================
List_functions_start:

GUI_list1__          List from file, callback. 1 or 2 columns.
GUI_list1_clear      also for GUI_list2__
GUI_list1_add_f      add lines from file to list
GUI_list1_add_l      add line to list (1,2 or 3 columns). Also for GUI_list2.

GUI_list2__          list from file; 2 or 3 columns(val.s with blanks); callback.
GUI_list2_add_f      add lines from file to list
GUI_list2_modCol     modify column # <iCol> of active (selected) Line

GUI_list1_dlg__      List from file in new window, callback.
GUI_list1_dlg_del    delete list-window GUI_list1_dlg
GUI_list1_dlg_w      list from file in a new window, waiting.
GUI_list1_h2__       make hierarchical SelectList.

GUI_list1_f          INTERNAL  populate list
GUI_list2_f          INTERNAL  populate list
GUI_list1_cbSel      INTERNAL  callback list selection
GUI_list1_dlg_cbw    INTERNAL  callback from GUI_list1_dlg_w
GUI_list1_dlg_exit   INTERNAL list-window exiting

List_functions_end:
=====================================================

\endcode *//*----------------------------------------

gtk_tree_selection_set_select_function
  provides also a callback before selection occurs ..

get model from treeview:        gtk_tree_view_get_model ()
get treeModel from treeview:    GTK_TREE_MODEL(go->data)
get selection from treeview:    gtk_tree_view_get_selection
get treeview from selection:    gtk_tree_selection_get_tree_view

get treePath as string (LineNr) gtk_tree_model_get_string_from_iter 


Compile Linux:
gcc -c `pkg-config --cflags gtk+-3.0` -DGTK3 gtk_lists.c
Compile MS-Win:
cl -c /I ..\include ut_gtk.c



======================================================================
*/




// #include "../xa/MS_Def0.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>         // f. GDK_Return

#include "../ut/ut_cast.h"                     // INT_PTR
#include "../ut/ut_umem.h"                     // Memspc
#include "../ut/ut_os.h"                       // OS_ ..
#include "../gui/gui_types.h"                  //  Typ_
#include "../gui/gui_base.h"                   // GUI_obj_*

#include "../gui_gtk2/gtk_base.h"
#include "../gui_gtk2/gtk_lists.h"  




static  GtkWidget    *UI_list1_win=NULL;
static  void         *UI_List_p1, *UI_List_p2, *UI_List_p3,
                     *UI_List_p4, *UI_List_p5;

static  GtkListStore *UI_ListStore;
static int           GUI_list1_msbt; // mouseButtonNr(1|2|3),ReturnKey=13;
static int           GUI_list1_evt;  // TYP_EventPress(MB,Enter-Key)
                                     // TYP_EventEnter (Up,Down,Page-keys)
static int           GUI_list1_stat; // 1=childList active;
                                     // 2=parentList active;
static char          GUI_list1_s1[256], GUI_list1_s2[256],
                     GUI_list1_s3[256], GUI_list1_s4[256];

// ex gtk_core:
extern int       UI_fontsizX, UI_fontsizY;
extern GtkWidget *UI_act_wi;
extern GtkWidget *UI_MainWin;


typedef struct {
  int  nrVal;      // nr of values per line
  char **pTab;     // table of <nrVal> pointers; strings; for out and in.
  char delVal;     // value-delimiter-char; comma or semicolon
  } CSV_struct;






//================================================================
  int GUI_list2_modCol (MemObj *mo, int iCol, char *txt) {
//================================================================
/// \code
/// modify column # <iCol> of active (selected) Line
/// Input:
///   iCol        ColNr; 0=first !
///   txt         ColText
/// \endcode


  int          i1;
  Obj_gui2     *go;
  void         *selection;
  GtkTreeIter  it1;
  GtkTreeModel *model;



  // printf("GUI_list2_modCol %d |%s|\n",iCol,txt);

  go = GUI_obj_pos (mo);
  if(!go) return 0;


  selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(go->widget));

  i1 = gtk_tree_selection_get_selected (selection, &model, &it1);
  if(!i1) {
    printf("GUI_list2_modCol E001 - no row selected ..\n");
    return -1;
  }


  // modify column iCol only   (GtkListStore,
  gtk_list_store_set (go->data, &it1, iCol, txt, -1);


  return 0;

}


//================================================================
  int GUI_list1_dlg_w (char *sels, int sSiz,
                      void *parWin, char *winTit, char *fNam,
                      char *mode, char *titLst[], char *opts) {
//================================================================
/// \code
/// GUI_list1_dlg_w                 list from file in a new window, waiting.
///
/// Inputs:
///   sSiz       size of sels in characters
///   parWin,    parent-window; NULL for primary window
///   winTit     window-title
///   fNam       filename of list to display; mode=nr of words per line
///   mode       "1"  selection returns 1 word;
///              "2" selection returns 2 words;
///   titLst     1 or 2 headertitles; NULL = none
///   opts       options; (HorSiz,VertSiz)
///              HorSiz,VertSiz:  size in characters; default is automatic size.
///                              'e' = expand widget; default is fixed size.
///              Examples: "" or "10" or "10e,e"
///                "10e,e" horiz. size 10 chars, hor. and vert. expandable.
/// Output:
///   sels        selected string, 1 or 2 words, separated by \n
///   RetCod:   0  OK, data valid
///            -1  cancelled by user
///
/// TODO:
/// add option filter to GUI_List
/// \endcode

  int        i1;
  char       fn[240], *p1;


  GUI_list1_dlg__ (parWin, winTit, fNam,
                  mode, titLst, GUI_list1_dlg_cbw, opts);

  // wait for signalfile & get its content
  return OS_file_sig_wait (1, sels);


}



//================================================================
  int GUI_list1_dlg_exit (void *parent, void *data) {
//================================================================
/// \code
/// INTERNAL
/// list-window exiting; report to user ..
/// \endcode

  int          iEv=TYP_EventExit;
  void         (*uFunc)();
  void         *pTab[1];
  MemObj       mo;
  Obj_Unknown  *go;

extern Memspc UI_tmpSpc;
extern int    UI_umbId;
  

  // printf("GUI_list1_dlg_exit\n");


  mo = GUI_obj_tmp ((void**)&go, sizeof(Obj_Unknown));

  go->gio_typ = TYP_GUI_List;
  go->gio_src = -1;

  pTab[0] = &iEv;

  // report user
  uFunc = data;
  uFunc (&mo, pTab);

  return FALSE;

}


//================================================================
  void GUI_list1_dlg_del () {  
//================================================================
/// GUI_list1_dlg_del    delete list-window GUI_list1_dlg

// CANNOT delete this window - crashes updating after selection.


  GtkWidget *w1;

  // printf("GUI_list1_dlg_del \n");

  if(UI_list1_win) {

    w1 = gtk_widget_get_toplevel (UI_list1_win);  // Gtk3

    // gtk_widget_destroy (w1);
    gtk_widget_hide (w1);

  }

  return;

}


//============================================================================
  int GUI_list1_dlg__ (MemObj *o_par, char *winTit, char *fNam,
                       char *mode, char *titLst[], void *funcnam, char*opts) {
//============================================================================
/// \code
/// GUI_list1_dlg__             list from file in a new window, modal, callback.
///
/// Inputs:
///   o_par      parent-window; NULL for primary window
///   winTit     window-title
///   fNam       filename of list to display; mode=nr of words per line
///   mode       "1"  selection returns 1 word;
///              "2" selection returns 2 words;
///   titLst     1 or 2 headertitles; NULL = none
///   funcnam    callback-function of selection 
///   opts       options; (HorSiz,VertSiz)
///              HorSiz,VertSiz:  size in characters; default is automatic size.
///                              'e' = expand widget; default is fixed size.
///              Examples: "" or "10" or "10e,e"
///                "10e,e" horiz. size 10 chars, hor. and vert. expandable.
///
/// funcnam prototype: 
///   int funcnam (MemObj *mo, void **data);
///    // data=table of 5 pointers;
///                0=TYP_EventPress|TYP_EventExit        GUI_DATA_EVENT
///                1=mousButtonNr(1|2|3)                 GUI_DATA_I1
///                2=colNr(1|2)                          GUI_DATA_I2
///                3=sel.Text col.1;                     GUI_DATA_S3
///                4=sel.Text col.2 (only if colNr=2)    GUI_DATA_S4
///   GUI_OBJ_TYP(mo)  = TYP_GUI_List
///
///  TYP_EventExit is returned from cancel list; no further data.
///
/// Example see GUI_list1__
/// \endcode
 
  int        i1;
  GtkWidget  *box0;
  void       *w_par, *parWin;
  Obj_gui2   *go;
  GtkWidget  *GUI_file_get();  


  // printf("GUI_list1_dlg__ |%s|%s|%s|\n",fNam,mode,opts);
  // printf("  UI_list1_win=%p\n",UI_list1_win);



  // get gtk-widget for parentBox from MemObj
  if(o_par) {
    // w_par = GUI_obj_parentBox (o_par);
    // if(!w_par) return -1; // (UME_obj_invalid_set (-2));
    go = GUI_obj_pos (o_par);
    if(!go) return -1;

    parWin = gtk_widget_get_toplevel (go->widget);      // 2013-05-13

  } else {
    parWin = GUI_file_get ();      // 2013-05-13
  }


  // // get address & memspc
  // GUI_obj_spc ((void**)&go, sizeof(Obj_gui2));
  // if(!go) return (UME_obj_invalid_set (-3));



  // test if a List is already active
  i1 = GUI_is_win_alive(UI_list1_win);
  if(i1) {
    printf(" GUI_list1_dlg__ already active ..\n");
    // must kill old list; new list can have different colNr, callback ..
    gtk_widget_destroy (UI_list1_win);
    // re-populate list
    // GUI_list1_f (UI_ListStore, fNam, mode);
    // goto L_dlg_lst_cr;
  }


  // open window
  UI_list1_win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (UI_list1_win), winTit);
  g_signal_connect (UI_list1_win, "destroy",
                    G_CALLBACK(GUI_list1_dlg_exit), funcnam);


  // vertical box for rows
  gtk_container_set_border_width(GTK_CONTAINER(UI_list1_win), 4);

  box0 = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (UI_list1_win), box0);
  gtk_widget_show (box0);


  // create list, connect callback
  UI_act_wi = box0;
  GUI_list1__ (NULL, fNam, mode, titLst, funcnam, opts);


  L_dlg_lst_cr:
  gtk_window_set_modal (GTK_WINDOW (UI_list1_win), TRUE);
  gtk_window_set_transient_for (GTK_WINDOW(UI_list1_win), GTK_WINDOW(parWin));


  gtk_widget_show (UI_list1_win);


  return 0;

}


//======================================================================
  MemObj GUI_list1__ (MemObj *o_par, char *fNam,
                      char *mode, char *titLst[], void *funcnam, char* opts) {
//======================================================================
/// \code
/// list from file; 1 or 2 columns; callback.
/// Values may not have blanks.
/// File with 2 columns: value1 - a single blank - value2
/// Input:
///   o_par       parentBox
///   fNam        filename of list to display; mode=nr of words per line
///   mode        "1"  selection returns 1 word;
///               "2" selection returns 2 words;
///   titLst      1 or 2 headertitles; NULL = none
///   funcnam     callback - returns the selected line (as list of words)
///   opts       options; (HorSiz,VertSiz)
///              HorSiz,VertSiz:  size in characters; default is automatic size.
///                              'e' = expand widget; default is fixed size.
///                               negative values: size in pixels.
///              Examples: "" or "10" or "10e,e"
///                "10e,e" horiz. size 10 chars, hor. and vert. expandable.
///
/// funcnam prototype:
/// int funcnam (void *parent, void **data);
///    // data=table of 5 pointers;
///                0=TYP_EventPress|TYP_EventEnter       GUI_DATA_EVENT
///                1=mouseButtonNr or keyCode            GUI_DATA_I1
///                2=colNr(1|2)                          GUI_DATA_I2
///                3=sel.Text col.1                      GUI_DATA_S3
///                4=sel.Text col.2 (only if colNr=2)    GUI_DATA_S4
///   GUI_OBJ_TYP(mo)  = TYP_GUI_List
///
/// Cursorkeys (up,down), page-keys (up,down) provide a usercall of type
///   TYP_EventEnter; GUI_DATA_I1 gives the keyCode (eg GUI_KeyCurUp)
/// MousButtonClicks provide a usercall of type TYP_EventPress;
///   GUI_DATA_I1 gives the mouseButtonNr (GUI_MouseL|GUI_MouseM|GUI_MouseR)
/// DoubleClick provides 3 userCalls of type TYP_EventPress; first and second
///   with GUI_MouseL, the third call with GUI_Mouse2L.
/// The Enter-key provides a usercall of type TYP_EventPress;
///    GUI_DATA_I1 = GUI_Mouse2L
///
/// Example:
/// GUI_list1__ (Box, "list2.dat", "2", NULL, cb_list_sel, "60,10");
///   printf(" mouseButtNr=%d selColNr=%d\n",GUI_DATA_I1,GUI_DATA_I2);
///   printf(" txt sel |%s|%s|\n",GUI_DATA_S3,GUI_DATA_S4);
/// }
///
/// Example with titles:
/// char           *tiTab[] = {"ID","Value"};
/// GUI_list1__ (Box, "list2.dat", "2", tiTab, cbl, "");
///
/// \endcode


  int          pTyp;
  GtkWidget    *box0, *w1;
  GtkListStore *list_store;
  GtkWidget    *treeView;
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *col1, *col2;
  GtkWidget    *scrolled_win;
  GtkTreeSelection *select;
  char         *p1;
  void         *w_par;
  Obj_gui2     *go;



  // printf("GUI_list1__ |%s|%s|%s|\n",fNam,mode,opts);


  // get gtk-widget for parentBox from MemObj
  if(o_par) {
    w_par = GUI_obj_parent__ (&pTyp, o_par);
    if(!w_par) return (UME_obj_invalid_set (-3));
    // get address & memspc; temp-space for modal object ..
    GUI_obj_spc ((void**)&go, sizeof(Obj_gui2));

  } else {
    w_par = UI_act_wi;    // used by GUI_list1_dlg__
    // get address & memspc; temp-space for modal object ..
    GUI_obj_tmp ((void**)&go, sizeof(Obj_gui2));
    pTyp = TYP_GUI_Win;
  }


  if(!go) return (UME_obj_invalid_set (-1));


  //----------------------------------------------------------------
  // create List
  if(mode[0] == '1') {
    list_store = gtk_list_store_new (1,
                                     G_TYPE_STRING);
  } else {
    list_store = gtk_list_store_new (2,
                                     G_TYPE_STRING,
                                     G_TYPE_STRING);
  }


  // populate list
  GUI_list1_f (list_store, fNam, mode);

  UI_ListStore = list_store;


  // create a new view of data
  treeView = gtk_tree_view_new_with_model (GTK_TREE_MODEL (list_store));
  g_object_unref (list_store);


  gtk_tree_view_set_enable_search (GTK_TREE_VIEW(treeView), FALSE);
  // else comes callback from keyIn = crash ! (disable edit lines)

  if(!titLst)        // remove headerbar
  gtk_tree_view_set_headers_visible (GTK_TREE_VIEW(treeView), FALSE);


  // create adjustments for treeView
  scrolled_win = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_win),
                                  GTK_POLICY_AUTOMATIC,
                                  GTK_POLICY_ALWAYS);


  // gtk_box_pack_start (GTK_BOX (w_par), scrolled_win, TRUE, TRUE, 0);

  // erste Spalte
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (G_OBJECT (renderer), "ypad", 0, NULL);

  // define columns; title, type.
  if(titLst) p1 = titLst[0];
  else       p1 = NULL;
  col1 = gtk_tree_view_column_new_with_attributes (p1,
                            renderer,
                            "text", 0,  // attributes ..
                            NULL);      // terminator
  gtk_tree_view_append_column (GTK_TREE_VIEW (treeView), col1);


  if(mode[0] == '2') {      // 2. column
    if(titLst) p1 = titLst[1];
    renderer = gtk_cell_renderer_text_new ();
    g_object_set (G_OBJECT (renderer), "ypad", 0, NULL);
    col2 = gtk_tree_view_column_new_with_attributes (p1,
                            renderer,
                            "text", 1,  // attributes ..
                            NULL);      // terminator
    gtk_tree_view_append_column (GTK_TREE_VIEW (treeView), col2);
  }



  // gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (treeView), TRUE);
  gtk_container_add (GTK_CONTAINER (scrolled_win), treeView);
  // gtk_box_pack_start (GTK_BOX (scrolled_win), treeView, TRUE, TRUE, 0);  ERR !


  //----------------------------------------------------------------
  // Setup the selection handler
  GUI_list1_msbt = 0;
  select = gtk_tree_view_get_selection (GTK_TREE_VIEW (treeView));

  // SINGLE does not select already selected !
  // gtk_tree_selection_set_mode (select, GTK_SELECTION_SINGLE);
  gtk_tree_selection_set_mode (select, GTK_SELECTION_BROWSE);

  // gtk_tree_selection_set_select_function (select, 
                                          // G_CALLBACK (GUI_list1_cbSel),
                                          // funcnam,
                                          // NULL);

  gtk_widget_set_events (GTK_WIDGET(treeView),
                         GDK_BUTTON_PRESS_MASK|
                         GDK_KEY_PRESS_MASK);

  // sig changed: from mouse-select, key-blank, key-PageDwn !
  // g_signal_connect_after (G_OBJECT (select), "changed",
  g_signal_connect (G_OBJECT (select), "changed",
                    G_CALLBACK (GUI_list1_cbSel),
                    PTR_MEMOBJ(go->mem_obj));


  g_signal_connect (treeView,
                    "key-press-event",
                    G_CALLBACK (GUI_list1_cbKey),
                    PTR_MEMOBJ(go->mem_obj));


  g_signal_connect (treeView,
                    "button_press_event",
                    G_CALLBACK (GUI_list1_cbMouse),
                    PTR_MEMOBJ(go->mem_obj));


  //----------------------------------------------------------------
  gtk_widget_show (treeView);

  // GUI_packwn (w_par, scrolled_win, hsiz, vsiz);
  GUI_w_pack1 (pTyp, w_par, scrolled_win, opts);


  gtk_tree_selection_unselect_all (select);   // geht ned ..

  go->gio_typ  = TYP_GUI_List;
  go->widget   = treeView;              //scrolled_win;
  go->uFunc    = funcnam;
  go->data     = list_store;


  return (((Obj_Unknown*)go)->mem_obj);

}


//======================================================================
  MemObj GUI_list2__ (MemObj *o_par, char *fNam,
                      char *mode, char *titLst[], void *funcnam, char *opts) {
//======================================================================
/// \code
/// list from file; 2 or 3 columns; callback.
/// Values may have blanks ..
/// Fileformat: csv ..
/// Input:
///   o_par       parentBox 
///   fNam        filename of list to display; mode=nr of vals; NULL = none.
///   mode        "2"  selection returns 2 words;
///               "3" selection returns 3 words;
///   titLst      2 or 3 headertitles or NULL = no headers
///   funcnam     callback - returns the selected line (as list of words)
///   opts       options; (HorSiz,VertSiz)
///              HorSiz,VertSiz:  size in characters; default is automatic size.
///                               negative values: size in pixels.
///                              'e' = expand widget; default is fixed size.
///              Examples: "" or "10" or "10e,e"
///                "10e,e" horiz. size 10 chars, hor. and vert. expandable.
/// 
/// funcnam prototype:
/// int funcnam (void *parent, void **data);
///    // data=table of 5 pointers;
///                0=TYP_EventPress|TYP_EventEnter       GUI_DATA_EVENT
///                1=mouseButtonNr or keyCode            GUI_DATA_I1
///                2=colNr(1|2)                          GUI_DATA_I2
///                3=sel.Text col.1                      GUI_DATA_S3
///                4=sel.Text col.2 (only if colNr=2)    GUI_DATA_S4
///                5=sel.Text col.2 (only if colNr=3)    GUI_DATA_S5
///   GUI_OBJ_TYP(mo)  = TYP_GUI_List
///
/// Cursorkeys (up,down), page-keys (up,down) provide a usercall of type
///   TYP_EventEnter; GUI_DATA_I1 gives the keyCode (eg GUI_KeyCurUp)
/// MousButtonClicks or the Enter-key provide a usercall of type TYP_EventPress;
///   GUI_DATA_I1 gives the mouseButtonNr (1|2|3) or DoubleClick (4) or
///   ReturnKey (13).
/// DoubleClick provides 3 userCalls of type TYP_EventPress; first and second
///   with mouseButton 1, the third call with mouseButton=4.
///
/// Example:
/// GUI_list2__ (Box, "list3.csv", "3", NULL, cb_list_sel, "60,10");
/// ..
/// int cb_list_sel (MemObj *mo, void **data) {
///   printf(" mouseButtNr=%d selColNr=%d\n",GUI_DATA_I1,GUI_DATA_I2);
///   printf(" txt sel |%s|%s|%s|\n",GUI_DATA_S3,GUI_DATA_S4,GUI_DATA_S5);
/// }
///
/// Example with titles:
/// char           *tiTab[] = {"ID","Value","Info"};
/// GUI_list2__ (Box, "list2.dat", "3", tiTab, cbl, "60,10");
///
/// \endcode


  GtkWidget    *box0, *w1;
  GtkListStore *list_store;
  GtkWidget  *treeView;
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *col1, *col2, *col3;
  GtkWidget  *scrolled_win;
  GtkTreeSelection *select;

  int        iSizY, pTyp;
  char       *p1;
  void       *w_par;
  Obj_gui2   *go;



  // printf("GUI_list2__ |%s|%s|%s|\n",fNam,mode,opts);


  // get gtk-widget for parentBox from MemObj
  if(o_par) {
    w_par = GUI_obj_parent__ (&pTyp, o_par);
    if(!w_par) return (UME_obj_invalid_set (-3));
    // get address & memspc; temp-space for modal object ..
    GUI_obj_spc ((void**)&go, sizeof(Obj_gui2));

  } else {
    w_par = UI_act_wi;    // used by GUI_list2_dlg__
    // get address & memspc; temp-space for modal object ..
    GUI_obj_tmp ((void**)&go, sizeof(Obj_gui2));
  }


  if(!go) return (UME_obj_invalid_set (-1));


  //----------------------------------------------------------------
  // create List
  if(mode[0] == '2') {
    list_store = gtk_list_store_new (2,
                                     G_TYPE_STRING,
                                     G_TYPE_STRING);
  } else {
    list_store = gtk_list_store_new (3,
                                     G_TYPE_STRING,
                                     G_TYPE_STRING,
                                     G_TYPE_STRING);
  }


  // populate list
  if(fNam) GUI_list2_f (list_store, fNam, mode);

  UI_ListStore = list_store;


  // create a new view of data
  treeView = gtk_tree_view_new_with_model (GTK_TREE_MODEL (list_store));
  g_object_unref (list_store);


  gtk_tree_view_set_enable_search (GTK_TREE_VIEW(treeView), FALSE);
  // else comes callback from keyIn = crash ! (disable edit lines)

  if(!titLst)        // remove headerbar
  gtk_tree_view_set_headers_visible (GTK_TREE_VIEW(treeView), FALSE);


  // create adjustments for treeView
  scrolled_win = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_win),
                                  GTK_POLICY_AUTOMATIC,
                                  GTK_POLICY_AUTOMATIC); //GTK_POLICY_ALWAYS);


  // gtk_box_pack_start (GTK_BOX (w_par), scrolled_win, TRUE, TRUE, 0);



#ifdef _MSC_VER
      iSizY = UI_fontsizY;
#else
      iSizY = UI_fontsizY + (UI_fontsizY / 4);
#endif
        // printf(" fontsizY=%d iSizY=%d\n",UI_fontsizY,iSizY);




  if(titLst) p1 = titLst[0];
  else       p1 = NULL;

  // 1. column
  renderer = gtk_cell_renderer_text_new ();
  // g_object_set (G_OBJECT (renderer), "foreground", "red", NULL);
  // gtk_cell_renderer_set_fixed_size (renderer, -1, iSizY);
  g_object_set (G_OBJECT (renderer), "ypad", 0, NULL);
  col1 = gtk_tree_view_column_new_with_attributes (p1,
                            renderer,
                            "text", 0,  // attributes ..
                            NULL);      // terminator
  gtk_tree_view_append_column (GTK_TREE_VIEW (treeView), col1);


  // 2. column
    if(titLst) p1 = titLst[1];
    renderer = gtk_cell_renderer_text_new ();
    // gtk_cell_renderer_set_fixed_size (renderer, -1, iSizY);
    g_object_set (G_OBJECT (renderer), "ypad", 0, NULL);
    col2 = gtk_tree_view_column_new_with_attributes (p1,
                            renderer,
                            "text", 1,  // attributes ..
                            NULL);      // terminator
    gtk_tree_view_append_column (GTK_TREE_VIEW (treeView), col2);


  // 3. column
  if(mode[0] == '3') {
    if(titLst) p1 = titLst[2];
    renderer = gtk_cell_renderer_text_new ();
    // gtk_cell_renderer_set_fixed_size (renderer, -1, iSizY);
    g_object_set (G_OBJECT (renderer), "ypad", 0, NULL);
    col3 = gtk_tree_view_column_new_with_attributes (p1,
                            renderer,
                            "text", 2,  // attributes ..
                            NULL);      // terminator
    gtk_tree_view_append_column (GTK_TREE_VIEW (treeView), col3);
  }



  // gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (treeView), TRUE);
  gtk_container_add (GTK_CONTAINER (scrolled_win), treeView);
  // gtk_box_pack_start (GTK_BOX (scrolled_win), treeView, TRUE, TRUE, 0);  ERR !


  //----------------------------------------------------------------
  // Setup the selection handler
  GUI_list1_msbt = 0;
  select = gtk_tree_view_get_selection (GTK_TREE_VIEW (treeView));

  // SINGLE does not select already selected !
  // gtk_tree_selection_set_mode (select, GTK_SELECTION_SINGLE);
  gtk_tree_selection_set_mode (select, GTK_SELECTION_BROWSE);

  // gtk_tree_selection_set_select_function (select, 
                                          // G_CALLBACK (GUI_list1_cbSel),
                                          // funcnam,
                                          // NULL);

  // gtk_widget_set_events (GTK_WIDGET(scrolled_win),
                         // GDK_BUTTON_PRESS_MASK|
                         // GDK_KEY_PRESS_MASK);

  gtk_widget_set_events (GTK_WIDGET(treeView),
                         GDK_BUTTON_PRESS_MASK|
                         // GDK_SCROLL_MASK|
                         GDK_KEY_PRESS_MASK);

  // sig changed: from mouse-select, key-blank, key-PageDwn !
  // g_signal_connect_after (G_OBJECT (select), "changed",
  g_signal_connect (G_OBJECT (select), "changed",
                    G_CALLBACK (GUI_list1_cbSel),
                    PTR_MEMOBJ(go->mem_obj));


  g_signal_connect (treeView,
                    "key-press-event",
                    G_CALLBACK (GUI_list1_cbKey),
                    PTR_MEMOBJ(go->mem_obj));


  g_signal_connect (treeView,
                    "button-press-event",
                    G_CALLBACK (GUI_list1_cbMouse),
                    PTR_MEMOBJ(go->mem_obj));


  // g_signal_connect (GTK_OBJECT(GL_widget), "scroll_event",
                      // GTK_SIGNAL_FUNC(UI_GR_Scroll), NULL);


  //----------------------------------------------------------------
  gtk_widget_show (treeView);

  // GUI_packwn (w_par, scrolled_win, hsiz, vsiz);
  GUI_w_pack1 (pTyp, w_par, scrolled_win, opts);


  gtk_tree_selection_unselect_all (select);   // geht ned ..

  go->gio_typ  = TYP_GUI_List;
  go->widget   = treeView;           //scrolled_win;
  go->uFunc    = funcnam;
  go->data     = list_store;


  return (((Obj_Unknown*)go)->mem_obj);

}


//================================================================
  int GUI_list1_add_f (MemObj *mo, char *fNam, char *mode) {
//================================================================
/// \code
/// add lines from file to list
///   fNam        filename of list to display; mode=nr of words per line
///   mode        "1"  selection returns 1 word;
///               "2" selection returns 2 words;
/// \endcode


   Obj_gui2  *go;


  printf("GUI_list1_add_f\n");

  go = GUI_obj_pos (mo);
  if(!go) return 0;

  GUI_list1_f (go->data, fNam, mode);

  return 0;

}


//================================================================
  int GUI_list2_add_f (MemObj *mo, char *fNam, char *mode) {
//================================================================
/// \code
/// add lines from file to list
///   fNam        filename of list to display; mode=nr of words per line
///   mode        "1"  selection returns 1 word;
///               "2" selection returns 2 words;
/// \endcode


   Obj_gui2  *go;


  printf("GUI_list2_add_f\n");

  go = GUI_obj_pos (mo);
  if(!go) return 0;

  GUI_list2_f (go->data, fNam, mode);

  return 0;

}


//================================================================
  int GUI_list1_add_l (MemObj *mo, char *c1, char *c2, char *c3) {
//================================================================
/// \code
/// add line to list (1,2 or 3 columns)
/// Also for GUI_list2.
/// \endcode
 
  int          nCol;
  Obj_gui2     *go;
  GtkTreeIter  it1;


  // printf("GUI_list1_add_l |%s|%s|%s|\n",c1,c2,c3);

  go = GUI_obj_pos (mo);
  if(!go) return 0;


  // Add a new row to the model
    // printf(" add |%s|\n", cbuf);
  gtk_list_store_append (go->data, &it1);

  nCol = gtk_tree_model_get_n_columns (GTK_TREE_MODEL(go->data));

  if(nCol > 2)
    gtk_list_store_set (go->data, &it1, 0, c1, 1, c2, 2, c3, -1);

  if(nCol > 1)
    gtk_list_store_set (go->data, &it1, 0, c1, 1, c2, -1);


  return 0;

}


//================================================================
  int GUI_list1_clear (MemObj *mo) {
//================================================================

   Obj_gui2  *go;


  // printf("GUI_list1_clear\n");

  go = GUI_obj_pos (mo);
  if(!go) return 0;

  gtk_list_store_clear (go->data);

  return 0;

}


//===================================================================
  int GUI_list1_f (void *list_store, char *fnam, char *mode) {
//===================================================================
/// GUI_list1_f          INTERNAL  populate list
/// 1 or 2 columns from file (sep = blank)
 
  int         i1;
  GtkTreeIter iter;
  char        cbuf[256], *p1;
  FILE        *fpi;


  // printf("GUI_list1_f |%s|\n",fnam);


  if ((fpi = fopen (fnam, "r")) == NULL) {
    return -1;
  }

  i1 = 0;

  while (!feof (fpi)) {
    ++i1;
    if(i1 > 10000) {
      fclose(fpi);
      return -2;
    }

    if (fgets (cbuf, 256, fpi) == NULL) break;
    UTX_CleanCR (cbuf);
      // printf(" list add |%s|\n",cbuf);

    gtk_list_store_append (list_store, &iter);


    //----------------------------------------------------------------
    // 1 col-
    if(mode[0] == '1') {
      gtk_list_store_set (list_store, &iter,
                          0, cbuf,        // Col.0, data,
                          -1);            // EndOfParameters !

    //----------------------------------------------------------------
    // 2 cols
    } else {
      // find delimiter
      p1 = strchr (cbuf, ' ');
      if(p1) {
        *p1 = '\0';
        ++p1;
        while(*p1  == ' ') ++p1;
      // } else {
      }
      gtk_list_store_set (list_store, &iter,
                          0, cbuf,        // Col.0, data,
                          1, p1,          // Col.1, data,
                          -1);            // EndOfParameters !
    }
  }

  fclose(fpi);

  return 0;

}

//===================================================================
  int GUI_list2_f (void *list_store, char *fnam, char *mode) {
//===================================================================
/// GUI_list2_f          INTERNAL  populate list
/// 2 or 3 columns from csv-file
     
  int         i1, irc;
  GtkTreeIter iter;
  char        *pTab[3], s1[512];
  FILE        *fp1;
  CSV_struct  fTyp;

    
  // printf("GUI_list2_f |%s|\n",fnam);

  i1 = 0;
    

  if((fp1=fopen(fnam,"r")) == NULL) {
    TX_Error("GUI_list2_f error open file %s",fnam);
    return -1;
  }

  fTyp.delVal = ' ';   // init for first call
  fTyp.pTab   = pTab;
  fTyp.nrVal  = 3;     // size of pTab

  // get nr of values, value-delimiter-char.
  L_nxt:
    irc = CSV_read__ (&fTyp, s1, 512, fp1);
    if(!irc) {
        // printf(" s1=|%s|\n",s1);
        // printf(" nxt ln |%s|%s|%s|\n",fTyp.pTab[0],fTyp.pTab[1],fTyp.pTab[2]);


    ++i1;
    if(i1 > 10000) {
      fclose(fp1);
      return -2;
    }


    gtk_list_store_append (list_store, &iter);


    //----------------------------------------------------------------
    // 2 col-
    if(mode[0] == '2') {
      gtk_list_store_set (list_store, &iter,
                          0, fTyp.pTab[0],        // Col.0, data,
                          1, fTyp.pTab[1],        // Col.1, data,
                          -1);            // EndOfParameters !

    //----------------------------------------------------------------
    // 3 cols
    } else {
      gtk_list_store_set (list_store, &iter,
                          0, fTyp.pTab[0],        // Col.0, data,
                          1, fTyp.pTab[1],        // Col.1, data,
                          2, fTyp.pTab[2],        // Col.1, data,
                          -1);            // EndOfParameters !
    }


      goto L_nxt;
    }



  fclose(fp1);

  return 0;

}


//================================================================
  int GUI_list1_cbKey (void *parent, void *event, MemObj mo) {
//================================================================
/// \code           
/// INTERNAL        
/// \endcode

  int         ikey, imod;
  // void        *selection;
  // Obj_gui2    *go;

  // go = GUI_obj_pos (&mo);
  // if(!go) return 0;


  // if(((GdkEvent*)event)->type == GDK_KEY_PRESS) {

  ikey = ((GdkEventKey*)event)->keyval;
  imod = ((GdkEventKey*)event)->state;
    // printf("GUI_list1_cbKey key=%d state=%d\n",ikey,imod);


  if(ikey == GDK_Return) {
    GUI_list1_evt  = TYP_EventPress;
    GUI_list1_msbt = GUI_Mouse2L;
    // selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(go->widget));
    // GUI_list1_cbSel (selection, mo);

  } else {
    GUI_list1_evt  = TYP_EventEnter;
    GUI_list1_msbt = ikey;
  }

  return (FALSE);   // TRUE: do no defaultOperations

}


//================================================================
  int GUI_list1_cbMouse (void *parent, void *ev, MemObj mo) {
//================================================================
/// \code
/// INTERNAL
/// \endcode

  int         typ;
  void        *selection;
  Obj_gui2    *go;


  // printf("GUI_list1_cbMouse \n");

  // if(mo) {   // 2013-04-09
  if(GUI_OBJ_IS_VALID(&mo)) {   // 2013-04-10
    go = GUI_obj_pos (&mo);
    if(!go) goto L_exit;
  }


  typ  = ((GdkEventAny*)ev)->type;


  if(typ == GDK_2BUTTON_PRESS) {
      printf(" doubleClick\n");
    if(GUI_OBJ_IS_VALID(&mo)) {   // 2013-04-10
      GUI_list1_evt  = TYP_EventPress;
      GUI_list1_msbt = GUI_Mouse2L;
      selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(go->widget));
      GUI_list1_cbSel (selection, mo);
    }
    goto L_exit;
  }


  if(typ != GDK_BUTTON_PRESS) goto L_exit;

  GUI_list1_evt  = TYP_EventPress;
  GUI_list1_msbt = ((GdkEventButton*)ev)->button;

  // printf("GUI_list1_cbMouse typ=%d but=%d\n",typ,GUI_list1_msbt);


  L_exit:
  return (FALSE);  // FALSE = do further handling of event ..

}


//================================================================
  int GUI_list1_cbSel (void *selection, MemObj mo) {
//================================================================
/// \code
/// GUI_list1_cbSel      INTERNAL  callback list selection (list1 & list2)
///
/// sig changed: from mouse-select, key-blank, key-PageDwn !
/// isel always 0
/// calls user with 0=TYP_EventPress|TYP_EventEnter       GUI_DATA_EVENT
///                 1=mousButtonNr(1|2|3)Return(13)       GUI_DATA_I1
///                 2=columnNr(1|2|3)                     GUI_DATA_I2
///                 3=sel.Text column.1;                  GUI_DATA_S3
///                 4=sel.Text col.2 (only if colNr=2)    GUI_DATA_S4
///                 5=sel.Text col.3 (only if colNr=3)    GUI_DATA_S5
/// \endcode


  int          irc, nCol;
  GtkTreeIter  iter;
  GtkTreeModel *model;
  gchar        *txt1, *txt2=NULL, *txt3=NULL;
  void         *pTab[6];
  Obj_gui2     *go;


  // printf("GUI_list1_cbSel ev=%d msbt=%d\n",GUI_list1_evt,GUI_list1_msbt);
  // UME_obj_dump (&mo);
  if(!GUI_list1_msbt) return (FALSE);


  // if(!GUI_list1_msbt) return FALSE; // skip M2, M3
  // if(!data) return 1;

  go = GUI_obj_pos (&mo);
  if(!go) return 0;


  if(gtk_tree_selection_get_selected (selection, &model, &iter)) {

    // get nr of columns
    nCol = gtk_tree_model_get_n_columns (model);
      // printf(" nCol=%d\n", nCol);


    // // get treePath as string (LineNr)
    // txt1 = gtk_tree_model_get_string_from_iter (model, &iter);
      // printf(" treePath=|%s|\n",txt1);
    // g_free (txt1);


    pTab[0] = &GUI_list1_evt;
    pTab[1] = &GUI_list1_msbt;   
    pTab[2] = &nCol;


    // decode selected row
    // iCol = 0;      // get data for first column
    gtk_tree_model_get (model, &iter, 0, &txt1, -1);  // access Spalte 0
    pTab[3] = txt1;
      // printf ("GUI_list1_cbSel col0 |%s|\n", (char*)pTab[3]);

    if(nCol > 1) {
      gtk_tree_model_get (model, &iter, 1, &txt2, -1);  // access Spalte 1
      pTab[4] = txt2;
        // printf ("GUI_list1_cbSel col1 |%s|\n", (char*)pTab[4]);
    }

    if(nCol > 2) {
      gtk_tree_model_get (model, &iter, 2, &txt3, -1);  // access Spalte 2
      pTab[5] = txt3;
        // printf ("GUI_list1_cbSel col2 |%s|\n", (char*)pTab[5]);
    }



    // report user
    go->uFunc (go, pTab);

    g_free (txt1);
    if(txt2) g_free (txt2);
    if(txt3) g_free (txt3);


  // } else {
    // go->uFunc (NULL, NULL);
  }

  GUI_list1_msbt = 0;

  // GUI_update__ ();  // Gtk2 only - else crash deleting List !

  // return TRUE;  // skip the default handler
  return (FALSE);  // DONT skip the default handler

}



//================================================================
  int GUI_list1_dlg_cbw (Obj_gui2 *go, void **data) {
//================================================================
/// \code
/// INTERNAL
/// callback from GUI_list1_dlg_w < GUI_list1__ < GUI_list1_cbSel
/// \endcode
 
  char s1[512];


  // printf("GUI_list1_dlg_cbw %d\n",GUI_DATA_EVENT);
 

  if(GUI_DATA_EVENT == TYP_EventPress) {
    // normal selection
    // create signalFile (creator of windows waits for this file ..)
    if(GUI_DATA_I2 == 1) {
        // printf(" sel 1 |%s|\n",GUI_DATA_S3);
      OS_file_sig_cre (1, GUI_DATA_S3);
    } else {
        // printf(" sel 1,2 |%s|%s|\n",GUI_DATA_S3,GUI_DATA_S4);
      sprintf(s1, "%s\n%s", GUI_DATA_S3, GUI_DATA_S4);
      OS_file_sig_cre (1, s1);
    }
    // kill window
    GUI_list1_dlg_del ();

  } else {    // TYP_EventExit
    // printf("GUI_list1_dlg_cbw list cancelled\n");
    // report cancel
    OS_file_sig_cre (99, NULL);
  }

  return 0;

}


//================================================================
  int GUI_list1_h2_cb1 (void *selection, void *data) {
//================================================================
/// \code
/// GUI_list1_cbSel      INTERNAL  callback list selection
///
/// sig changed: from mouse-select, key-blank, key-PageDwn !
/// isel always 0
/// calls user with 0=TYP_EventPress;                     GUI_DATA_EVENT
///                 2=mousButtonNr(1|2|3)                 GUI_DATA_I1
///                 3=colNr(1|2)                          GUI_DATA_I2
///                 4=sel.Text col.1;a                    GUI_DATA_S3
///                 5=sel.Text col.2 (only if colNr=2)    GUI_DATA_S4
/// \endcode


  int          irc, nCol, iEv=TYP_EventPress;
  // GtkWidget *w1;
  GtkTreeIter  iter;
  GtkTreeModel *model;
  gchar        *txt1, *txt2=NULL;
  void         *pTab[5];
  void         (*uFunc)();


  printf("GUI_list1_h2_cb1 bt=%d stat=%d\n",GUI_list1_msbt,GUI_list1_stat);

  if(!GUI_list1_msbt) return FALSE;
  GUI_list1_msbt = 0;



  if (gtk_tree_selection_get_selected (selection, &model, &iter)) {

    // decode selected row
    // iCol = 0;      // get data for first column
    gtk_tree_model_get (model, &iter, 0, &txt1, -1);  // access Spalte 0
      // printf ("GUI_list1_h2_cb1 c0 |%s|\n",txt1);

    if(GUI_list1_stat == 2) {
      // parent selected
      // copy parentTxt
      strcpy(GUI_list1_s3, txt1);
      g_free (txt1);
      // create new child-list
      uFunc = UI_List_p4;
      uFunc (GUI_list1_s2, GUI_list1_s3);
      // display new child-list
      gtk_list_store_clear (UI_ListStore);    // clear tree; RECURSION !
      // populate list with childfile
      irc = GUI_list1_f (UI_ListStore, GUI_list1_s2, "1");
      // change title -> child
      gtk_window_set_title (GTK_WINDOW (UI_list1_win), UI_List_p2);
      // unlock button
      gtk_widget_set_sensitive (UI_List_p3, TRUE);
      GUI_list1_stat = 1;

    } else {
      // child selected
      // copy childTxt
      strcpy(GUI_list1_s4, txt1);
      g_free (txt1);
      // report user
      uFunc = UI_List_p5;
      uFunc (GUI_list1_s3, GUI_list1_s4);
      GUI_list1_dlg_del (NULL);
    }

  }

  // GUI_update__ ();  // Gtk2 only - else crash deleting List !

  // return TRUE;
  return FALSE;

}


//================================================================
  int GUI_list1_h2_cb3 (void *parent, void *data) {
//================================================================
/// INTERNAL; parent-button pressed; display parentList fnamP

  int irc;

  printf("GUI_list1_h2_cb3 |%s|\n",TXT_PTR(data));

  gtk_list_store_clear (UI_ListStore);    // clear whole tree

  // populate list with parentfile
  irc = GUI_list1_f (UI_ListStore, GUI_list1_s1, "1");
    // printf(" irc=%d\n",irc);

  // block button
  gtk_widget_set_sensitive (UI_List_p3, FALSE);

  // change title -> parent
  gtk_window_set_title (GTK_WINDOW (UI_list1_win), UI_List_p1);

  GUI_list1_stat = 2;

  return 0;
}



//================================================================
  int GUI_list1_h2__ (char *titP, char *titC, char *lButP,
                     char *txtP, char *fnamP,
                     void *funcP, void *funcC, char *opts) {
//================================================================
/// \code
/// make hierarchical SelectList. Select childObject or 
///   select "Parents" with "<<--" -Button.
///   selecting parents-button changes childList -> parentList
///   selecting parentObject gets new childList by calling funcP
///   selecting childObject exits (provides parentText and childText by funcC)
/// 
/// Input:
///   titP     titletext for selection of parent
///   titC     titletext for selection of child
///   lButP    caption parents-button
///   txtP     active parent
///   fnamP    Name of file for parents
///   funcP    user has selected a parent; create childlist from selected parent.
///   funcC    user has selected a child; exit and report selection -> user.
///   opts       options; (HorSiz,VertSiz)
///              HorSiz,VertSiz:  size in characters; default is automatic size.
///                              'e' = expand widget; default is fixed size.
///              Examples: "" or "10" or "10e,e"
///                "10e,e" horiz. size 10 chars, hor. and vert. expandable.
/// Output:
///   RetCod:
///          1 - destroy HList;
///          0 - keep HList.
///         -2   file fnamP does not exist
/// 
///  funcP (char *fnamC, char *parentTxt) {}
/// 
///  funcC (char *parentTxt, char *childtxt) {}
///
/// \endcode

  int          pTyp=0;
  void         (*uFunc)();
  GtkWidget    *box0, *w1;
  GtkListStore *list_store;
  GtkWidget *treeView;
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *col1;
  GtkWidget *scrolled_win;
  GtkTreeSelection *select;
  char *p1;



  printf("GUI_list1_h2__ |%s|\n",opts);
  printf("  |%s|%s|\n",titP,titC);
  printf("  |%s|\n",fnamP);

  // parentsFile must exist
  if(OS_checkFilExist(fnamP, 1) != 1) return -2;


  UI_List_p1 = titP;
  UI_List_p2 = titC;
  UI_List_p4 = funcP;
  UI_List_p5 = funcC;
  GUI_list1_stat = 1;

  // copy parentFileName
  strcpy(GUI_list1_s1, fnamP);
  // create tempfilename chdFilNam
  sprintf(GUI_list1_s2, "%stmp.lst",OS_get_tmp_dir());


  // copy active parent
  strcpy(GUI_list1_s3, txtP);



  //----------------------------------------------------------------
  // get 1 line out of file fnamP; call funcP with it.
  if(strlen(GUI_list1_s3) < 1) {
    // get first catalog of dir > txtPar
    UTX_fgetLine (GUI_list1_s3, 128, fnamP, 1);
      // printf(" 1.ln |%s|\n",GUI_list1_s1);
  }


  // create new file tmp/catParts.lst for catalog actCatNam
  uFunc = funcP;
  uFunc (GUI_list1_s2, GUI_list1_s3);  // cfn ptxt


  //see GUI_list1_dlg__


  //----------------------------------------------------------------
  // open window
  UI_list1_win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (UI_list1_win), UI_List_p2);
  // g_signal_connect (UI_list1_win, "destroy",
                    // G_CALLBACK(GUI_list1_dlg_exit), funcnam);




  // vertical box for list
  gtk_container_set_border_width(GTK_CONTAINER(UI_list1_win), 4);

  box0 = gtk_vbox_new (FALSE, 0);

  gtk_container_add (GTK_CONTAINER (UI_list1_win), box0);
  gtk_widget_show (box0);




  //----------------------------------------------------------------
  list_store = gtk_list_store_new (1, G_TYPE_STRING);

  // populate list with childs
  GUI_list1_f (list_store, GUI_list1_s2, "1");

  UI_ListStore = list_store;


  // create a new view of data
  treeView = gtk_tree_view_new_with_model (GTK_TREE_MODEL (list_store));
  g_object_unref (list_store);


  // if(!titLst)        // remove headerbar
  gtk_tree_view_set_headers_visible (GTK_TREE_VIEW(treeView), FALSE);


  gtk_tree_view_set_enable_search (GTK_TREE_VIEW(treeView), FALSE);
  // else comes callback from keyIn = crash ! (disable edit lines)


  // create adjustments for tree
  scrolled_win = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_win),
                                  GTK_POLICY_AUTOMATIC,
                                  GTK_POLICY_ALWAYS);
  // add into box0 later with GUI_w_pack1

  // erste Spalte
  renderer = gtk_cell_renderer_text_new ();

  // define columns; title, type.
  // if(titLst) p1 = titLst[0];
  // else       p1 = NULL;
  col1 = gtk_tree_view_column_new_with_attributes (NULL,
                            renderer,
                            "text", 0,  // attributes ..
                            NULL);      // terminator
  gtk_tree_view_append_column (GTK_TREE_VIEW (treeView), col1);


  // gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (treeView), TRUE);
  // gtk_widget_set_hexpand (treeView, TRUE);             // else no resize !!
  // gtk_widget_set_vexpand (treeView, TRUE);             // else no resize !!
  gtk_container_add (GTK_CONTAINER (scrolled_win), treeView);


  // Setup the selection handler
  GUI_list1_msbt = 0;
  select = gtk_tree_view_get_selection (GTK_TREE_VIEW (treeView));
  gtk_tree_selection_set_mode (select, GTK_SELECTION_SINGLE);
  // gtk_tree_selection_set_mode (select, GTK_SELECTION_BROWSE);

  // gtk_tree_selection_set_select_function (select, 
                                          // G_CALLBACK (GUI_list1_cbSel),
                                          // funcnam,
                                          // NULL);

  gtk_widget_set_events (GTK_WIDGET(treeView),
                         GDK_KEY_PRESS_MASK);

  // sig changed: from mouse-select, key-blank, key-PageDwn !
  // g_signal_connect_after (G_OBJECT (select), "changed",
  g_signal_connect (G_OBJECT (select), "changed",
                    G_CALLBACK (GUI_list1_h2_cb1), NULL);

  g_signal_connect (treeView,
                    "button_press_event",
                    G_CALLBACK (GUI_list1_cbMouse), NULL);


  // create parent-button
  UI_List_p3 = gtk_button_new_with_label (lButP);
  g_signal_connect (UI_List_p3, "clicked",
                    G_CALLBACK(GUI_list1_h2_cb3), fnamP);

  // add button UI_List_p3 to box0
  GUI_w_pack1 (pTyp, box0, UI_List_p3, "");


/*
  //   hsiz,vsiz   size of window; >0=size in characters, <0=size in pixels
  // size
  if(hsiz != 0) {
    if(hsiz < 0) {
      ih = -hsiz;
    } else {
      ih = UI_fontsizX * hsiz;
    }
  }

  if(vsiz != 0) {
    if(vsiz < 0) {
      iv = -vsiz;
    } else {
      iv = UI_fontsizY * vsiz;
    }
  }


 if((ih) || (iv)) {
      printf(" lstsiz %d %d\n",ih,iv);
    GUI_siz_set (scrolled_win, ih, iv);
  }
*/

  // add scrolled_win into box0
  GUI_w_pack1 (pTyp, box0, scrolled_win, opts);


  // gtk_widget_show (scrolled_win);
  // gtk_widget_show (treeView);
  gtk_widget_show_all (scrolled_win);

  gtk_tree_selection_unselect_all (select);   // geht ned ..

  gtk_window_set_transient_for (GTK_WINDOW(UI_list1_win), GTK_WINDOW(UI_MainWin));

  gtk_widget_show (UI_list1_win);


  // return scrolled_win;
    printf("exit GUI_list1_h2__\n");

  return 0;

}


// EOF
