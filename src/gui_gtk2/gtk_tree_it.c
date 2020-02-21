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
  ..

-----------------------------------------------------
Modifications:
2012-03-10 port Gtk3. RF
2009-11-06 Created. RF.

----------------------------------------------------- */
#ifdef globTag
void GUI_Tree(){}
#endif
/*!
\file ../gui_gtk2/gtk_tree_it.c
\brief treeview with icon + text
\code

=====================================================
List_functions_start:

GUI_tree1__                  create Tree with icon + text
GUI_tree1_row_add            create row & set rowMode
GUI_tree1_remove__           remove row and its childs
GUI_tree1_clear              clear whole tree

GUI_tree1_expand__           expand row or complete tree
GUI_tree1_ck_expanded        test if row is expanded

GUI_tree1_row_set            change rowMode to active,normal,passive,selected ..
GUI_tree1_unselect_all       unselect all objects of tree

GUI_tree1_iter_string        find row that starts with <txt>
GUI_tree1_childNr            get nr of childs of row
GUI_tree1_par                get parentNode of node.
GUI_tree1_lev                get level of a node
GUI_tree1_cmp_row            compare 2 rows if identical
GUI_tree1_ndPos              get position of a node; returns string (eg "0:2")
GUI_tree1_analyz1            Resolv List.

GUI_tree1_set_selMode        deactivate / reactivate selection
GUI_tree1_childs_set         modify mode (active|passive) of all childs of a row

GUI_tree1_decode             INTERNAL
GUI_tree1_childs_remove      INTERNAL remove all childs of row
GUI_tree1_cbSel              INTERNAL  callback list selection
GUI_tree1_cbMouse            INTERNAL

List_functions_end:
=====================================================

\endcode *//*


-----------------------------------------------------------
*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// ../gtk/ut_gtk.c:
extern int UI_fontsizX, UI_fontsizY;


#include <gtk/gtk.h>

#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_umem.h"                     // Memspc

#include "../gui/gui_types.h"      // UI_Func..
#include "../gui/gui_base.h"                  // GUI_obj_*


#include "../gui_gtk2/gtk_tree_it.h"


// extern vars:
extern GdkPixbuf **IcoTab;


// local vars:
static GtkWidget    *GUI_tree1_tree;
static GtkTreeView  *GUI_tree1_view;
static GtkTreeModel *GUI_tree1_model;
static GtkTreeStore *GUI_tree1_store;
static int           GUI_tree1_msbt;
static Obj_gui2     *GUI_tree1_ActObj;

// prototypes:




//=====================================================================
  MemObj GUI_tree1__ (MemObj *o_par, void *selCB, char *opts) {
//=====================================================================
/// \code
/// create Tree with icon + text.
///  Icons must be loaded with GUI_Ico_init < iSizYIco_init
/// Input:
///   o_par      parentBox
///   selCB      callbackfunktion for the selection-event
///   opts       options; (HorSiz,VertSiz)
///              HorSiz,VertSiz:  size in characters; default is automatic size.
///                              'e' = expand widget; default is fixed size.
///              Examples: "" or "10" or "10e,e"
///                "10e,e" horiz. size 10 chars, hor. and vert. expandable.
/// Output:
///   returns    tree
///
/// prototyp selCB:
/// int selCB (void *parent, void **data);
///   // data=table of 6 pointers;
///   GUI_DATA_EVENT=*(int*)data[0]=TYP_EventPress|TYP_EventRelease
///   GUI_DATA_I1   =*(int*)data[1]=GUI_MouseL|      left mousebutton
///                                 GUI_MouseM|      mid mousebutton
///                                 GUI_MouseR|      right mousebutton
///   (TreeNode*)data[2]           =TreeNode of selected row
///   GUI_DATA_S3   =(char*)data[3]=text (col.1) of selected row
///   GUI_DATA_I4   =*(int*)data[4]=icon-nr of selected row
///   GUI_DATA_I5   =*(int*)data[5]=state of selected row 1=active, 0=passive
///
/// see also UI_mcl__ GUI_mList__
/// \endcode


  int  i1, iSizY, pTyp;
  char *sp1, cbuf[256];
  void **vp1;
  int  ih=0,  iv=0;

  GtkWidget         *tree = NULL;
  GtkWidget         *scrolled_win;
  GtkTreeSelection  *select;
  GtkTreeViewColumn *column;
  GtkCellRenderer   *renderer;
  GtkTreeStore      *store;
  GError            *error = NULL;

  GtkTreeIter       rowChd;

  void              *w_par;
  Obj_gui2          *go;



  // printf("GUI_tree1__ |%s|\n",opts);
  // printf(" sizeof(GtkTreeIter)=%d\n",(int)sizeof(GtkTreeIter));


  // get gtk-widget for parentBox from MemObj
  w_par = GUI_obj_parent__ (&pTyp, o_par);
  if(!w_par) return (UME_obj_invalid_set (-3));


  // get address & memspc
  GUI_obj_spc ((void**)&go, sizeof(Obj_gui2));
  if(!go) return (UME_obj_invalid_set (-1));


  //----------------------------------------------------------------
#ifdef _MSC_VER
  iSizY = UI_fontsizY;
#else
  iSizY = UI_fontsizY + (UI_fontsizY / 3);  // was 4
#endif
  // iSizY = UI_fontsizY;
  // iSizY = 15;
    // printf(" iSizY=%d\n",iSizY);


  //----------------------------------------------------------------
  // init IcoTab
  if(!IcoTab) {
    TX_Error ("GUI_tree1__ - GUI_Ico_init not initialized");
    return (UME_obj_invalid_set (-2));
  }


  //----------------------------------------------------------------
  // define nr & meaning of the columns
  store = gtk_tree_store_new (4, GDK_TYPE_PIXBUF,      // 0=icon
                                 G_TYPE_STRING,        // 1=text
                                 G_TYPE_BOOLEAN,       // 2=active/inactive
                                 G_TYPE_INT);          // 3=iconNr
  // visibility is used for icon & text !


  //----------------------------------------------------------------
  // Create a view
  tree = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));

  // winBrw->store = store;
  // winBrw->tree = tree;
  

  g_object_unref (G_OBJECT (store));


  //----------------------------------------------------------------
  // 1. column (icons); attibutes: "sensitive"
  renderer = gtk_cell_renderer_pixbuf_new();
  // gtk_cell_renderer_set_fixed_size (renderer, -1, iSizY);
  // g_object_set (G_OBJECT (renderer), "ypad", 0, NULL);
  g_object_set (G_OBJECT(renderer), "height", iSizY, NULL);
  column = gtk_tree_view_column_new_with_attributes ("0", renderer,
                                                     "pixbuf", 0,
                                                     "sensitive", 2,
                                                     NULL);
  // Add the column to the view.
  gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);


  //----------------------------------------------------------------
  // 2 column (text); attibutes: "sensitive"
  renderer = gtk_cell_renderer_text_new ();
  // gtk_cell_renderer_set_fixed_size (renderer, -1, iSizY);
  // g_object_set (G_OBJECT (renderer), "ypad", 0, NULL);
  g_object_set (G_OBJECT(renderer), "height", iSizY, NULL);
  // g_object_set (renderer, "foreground", "blue", "foreground-set", FALSE, NULL);
  // geht nicht; auch nicht ""cell-background": Don't know color ..
  // g_object_set (renderer, "weight",PANGO_WEIGHT_BOLD,"weight-set",FALSE,NULL);
  // geht:, PANGO_WEIGHT_BOLD|LIGHT|NORMAL
  column = gtk_tree_view_column_new_with_attributes ("1", renderer,
                                                     "text", 1,
                                                     "sensitive", 2,
                                                     // "weight", 3,
                                                     NULL);
  // Add the column to the view.
  gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);

  // g_object_set (G_OBJECT (column), "sizing", GTK_TREE_VIEW_COLUMN_FIXED, NULL);


  //----------------------------------------------------------------
  // create adjustment
  scrolled_win = gtk_scrolled_window_new (NULL, NULL);

  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_win),
                                  GTK_POLICY_AUTOMATIC,
                                  GTK_POLICY_ALWAYS);

  // gtk_box_pack_start (GTK_BOX (parent), scrolled_win, TRUE, TRUE, 0);


  // gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled_win),
                                         // tree);

  // gtk_container_set_focus_vadjustment (GTK_CONTAINER(tree),
                     // gtk_scrolled_window_get_vadjustment
                     // (GTK_SCROLLED_WINDOW (scrolled_win)));


  //----------------------------------------------------------------
  // Setup the selection handler
  if(selCB) {

    select = gtk_tree_view_get_selection (GTK_TREE_VIEW (tree));

    // can select group of objects ..
    gtk_tree_selection_set_mode (select, GTK_SELECTION_MULTIPLE);

    // gtk_tree_selection_set_mode (select, GTK_SELECTION_BROWSE);
    // gtk_tree_selection_set_mode (select, GTK_SELECTION_NONE);
    // GTK_SELECTION_NONE GTK_SELECTION_SINGLE GTK_SELECTION_MULTIPLE

    g_signal_connect (select, "changed",    // "select-cursor-row",
                      G_CALLBACK (GUI_tree1_cbSel),
                      PTR_MEMOBJ(go->mem_obj));


    // gtk_widget_add_events (GTK_WIDGET(tree),
                           // GDK_BUTTON_PRESS_MASK |
                           // GDK_BUTTON_RELEASE_MASK);

    // connect Mausbuttons
    // g_signal_connect_after does not work ..
    // comes before "changed"
    g_signal_connect (tree,
                      "button_press_event",
                      G_CALLBACK (GUI_tree1_cbMouse),
                      PTR_MEMOBJ(go->mem_obj));

    // useless; comes before GUI_tree1_cbSel !
    // g_signal_connect (tree, //scrolled_win,
                      // "button_release_event",
                      // G_CALLBACK (GUI_tree1_cbMouse),
                      // PTR_MEMOBJ(go->mem_obj));

  }


  //----------------------------------------------------------------
  // disable Headerline
  gtk_tree_view_set_headers_visible (GTK_TREE_VIEW(tree), FALSE);

  // // indent
  // gtk_tree_view_set_level_indentation (GTK_TREE_VIEW(tree), 0);

  // lines make it more clear ..
  gtk_tree_view_set_enable_tree_lines (GTK_TREE_VIEW(tree), TRUE);



  //----------------------------------------------------------------
  gtk_container_add (GTK_CONTAINER (scrolled_win), tree);
  gtk_widget_show (tree);
  // GUI_packwn (w_par, scrolled_win, hsiz, vsiz);
  GUI_w_pack1 (pTyp, w_par, scrolled_win, opts);


  //----------------------------------------------------------------
  go->gio_typ  = TYP_GUI_Tree;
  go->widget   = tree;
  go->uFunc    = selCB;
  go->data     = PTR_INT(0); // selection active


  return (((Obj_Unknown*)go)->mem_obj);

}


//=============================================================================
  int GUI_tree1_row_set (MemObj *mo, TreeNode *row,
                          int ico, char *txt, int mode) {
//=============================================================================
/// \code
/// GUI_tree1_row_set            change rowMode to active, normal, passive
/// Input:
///   ico        index of icon; -1 = do not modify.
///   txt        text for row; NULL = do not modify.
///   mode  -2   unselected
///         -1   passive (icon & text dimmed)
///          0   do not modify
///          1   active (normal)
///          2   selected            see GUI_tree1_set_selMode().
/// \endcode

  int   iCol, iDim;
  GtkTreeSelection *sel1;


  // printf("GUI_tree1_row_set %d |%s| %d\n",ico,txt,mode);
// return 0;


  // set GUI_tree1_tree, GUI_tree1_view, GUI_tree1_model and GUI_tree1_store
  if(mo) {   // for internal call: mo=NULL
    if(GUI_tree1_decode(mo)) return -1;
  }

/*
  //----------------------------------------------------------------
  if(mode == -3) {      // TEST ONLY
    gtk_tree_store_set (GUI_tree1_store, (GtkTreeIter*)row,
                                               1, txt,
                                               // 3, ico,
                                              -1);
    return 0;
  }
*/

  //----------------------------------------------------------------
  if(mode == -2) {
    sel1 = gtk_tree_view_get_selection (GUI_tree1_view);
    gtk_tree_selection_unselect_iter (sel1, (GtkTreeIter*)row);
    return 0;
  }


  //----------------------------------------------------------------
  if(mode == 2) {
    sel1 = gtk_tree_view_get_selection (GUI_tree1_view);
    // GUI_tree1_set_selMode (mo, 1);  // do not report following selections
    GUI_tree1_ActObj->data = PTR_INT(1);  // do not report following selections
    gtk_tree_selection_select_iter (sel1, (GtkTreeIter*)row);
    // GUI_tree1_set_selMode (mo, 0);  // report all following selections
    GUI_tree1_ActObj->data = PTR_INT(0);  // report all following selections
    return 0;
  }


  //----------------------------------------------------------------
  if(ico >= 0) {
      gtk_tree_store_set (GUI_tree1_store, (GtkTreeIter*)row,
                                               0, IcoTab[ico],
                                               3, ico,
                                              -1);
  }


  //----------------------------------------------------------------
  if(txt) {
      gtk_tree_store_set (GUI_tree1_store, (GtkTreeIter*)row,
                                               1, txt,
                                              -1);
  }


  //----------------------------------------------------------------
  if(mode == 0) return 0;
  if(mode == -1) iDim = FALSE;   // not sensitive
  else           iDim = TRUE;    // normal=sensitive  (mode=1)
      gtk_tree_store_set (GUI_tree1_store, (GtkTreeIter*)row, 
                                               2, iDim,
                                              -1);

  return 0;

}


//================================================================
  int GUI_tree1_decode (MemObj *mo) {
//================================================================
/// \code
/// INTERNAL
/// set GUI_tree1_tree, GUI_tree1_view, GUI_tree1_model and GUI_tree1_store
/// \endcode
  


  // printf("GUI_tree1_decode %d %d\n",mo->mbID,mo->ioff);


  GUI_tree1_ActObj = GUI_obj_pos (mo);
  if(!GUI_tree1_ActObj) return -1;

  if(GUI_tree1_ActObj->widget != GUI_tree1_tree)  {
    GUI_tree1_tree  = GUI_tree1_ActObj->widget;
      // printf(" GUI_tree1_tree=%p\n",GUI_tree1_tree);
    GUI_tree1_view  = GTK_TREE_VIEW (GUI_tree1_tree);           // view  < tree
    GUI_tree1_model = gtk_tree_view_get_model (GUI_tree1_view); // model < view
    // sele = gtk_tree_view_get_selection (view);
    GUI_tree1_store = GTK_TREE_STORE (GUI_tree1_model);         // store < model
  }
  
  return 0;
}


//================================================================
  int GUI_tree1_row_add (TreeNode *nodeChd,
                         MemObj *mo,
                         TreeNode *nodePar,
                         int icoNr, char *txt, int mode) {
//================================================================
/// \code
/// add (one more) childnode to parentNode <rowPar>
/// Input:
///   mo         tree-obj
///   nodePar    parent-row-node; NULL for primary obj
///   icoNr      index of icon; -1 = do not modify.  see eg ICO_Var
///   txt        text for row; NULL = do not modify.
///   mode  -1   passive (icon & text dimmed)
///          0   normal (text colored)
///          1   active (text normal)
/// Output:
///   nodeChd      child-row-node
/// \endcode


  // printf("GUI_tree1_row_add %d |%s| %d\n",icoNr,txt,mode);


  // set GUI_tree1_tree, GUI_tree1_view, GUI_tree1_model and GUI_tree1_store
  if(GUI_tree1_decode(mo)) return -1;


/*
  if(nodePar) {
  ii = gtk_tree_model_get_iter_from_string (GUI_tree1_model,
                                            &iPar, nodePar);
    if(ii != FALSE) {
     ip = &iPar;
       printf(" exists ..\n");
    }
  } 
*/

  // append new row to nodePar; output = nodeChd
  gtk_tree_store_append (GUI_tree1_store,
                         (GtkTreeIter*)nodeChd,
                         (GtkTreeIter*)nodePar);

  // GUI_tree1_row_set (tree, &iChd, icoNr, txt, mode);
  GUI_tree1_row_set (mo, nodeChd, icoNr, txt, mode);
    // printf(" n.row_set\n");

  return 0;

}


//================================================================
  int GUI_tree1_clear (MemObj *mo) {
//================================================================
/// clear whole tree

  // printf("GUI_tree1_clear\n");

  // set GUI_tree1_tree, GUI_tree1_view, GUI_tree1_model and GUI_tree1_store
  if(GUI_tree1_decode(mo)) return -1;

  gtk_tree_store_clear (GUI_tree1_store);    // clear whole tree

  return 0;

}


//================================================================
  int GUI_tree1_iter_string (TreeNode *it,
                             char *txt, TreeNode *itPar, MemObj *mo) {
//================================================================
/// \code
/// get iter of row that starts with <txt> among childs of itPar.
/// Check only childs of one level.
/// Input:
///   txt        objId + " "    eg "M21 "
///   itPar      parentNode; check only its childs; NULL for topLevel
/// Output:
///   it         node found
///   RetCode:   0 = ok, found;  row <it> starts with <txt>
///             -1 = not found
///
/// \endcode

  int           i1, ii, irc, il;
  char          cbuf[80];
  gchar         *txt1;
  GtkTreeModel  *model;


  // printf("GUI_tree1_iter_string |%s|\n",txt);
  // printf(" itPar=%p\n",itPar);

  if(GUI_tree1_decode(mo)) return -1;

  il = strlen(txt);


  // TEST:
  // gtk_tree_model_get_iter_from_string (GUI_tree1_model, itPar, "0");
    // printf(" itPar=%p\n",itPar);


  // get nr of childNodes of parent-node node
  ii = gtk_tree_model_iter_n_children (GUI_tree1_model, (GtkTreeIter*)itPar);
    // printf(" n_ch=%d\n",ii);


  // loop tru childs
  for(i1=0; i1<ii; ++i1) {

    irc = gtk_tree_model_iter_nth_child (GUI_tree1_model,
                                         (GtkTreeIter*)it,
                                         (GtkTreeIter*)itPar, i1);
    if(irc == 0) continue;

    // get value of iter of column 1
    gtk_tree_model_get (GUI_tree1_model, (GtkTreeIter*)it, 1, &txt1, -1);
    if(txt1) strcpy(cbuf, txt1);
    else cbuf[0] = '\0';
    g_free (txt1);
      // printf("ex GUI_tree1_iter_string |%s|\n",cbuf);

    if(!strncmp (cbuf, txt, il)) return 0;

  }

    // printf("ex GUI_tree1_iter_string -1\n");

  return -1;

}


//================================================================
  int GUI_tree1_childNr (MemObj *mo, TreeNode *it) {
//================================================================
/// GUI_tree1_childNr        get nr of childs of it

  int           ii;


  // set GUI_tree1_tree, GUI_tree1_view, GUI_tree1_model and GUI_tree1_store
  if(GUI_tree1_decode(mo)) return -1;

  // get nr of childNodes
  ii = gtk_tree_model_iter_n_children (GUI_tree1_model, (GtkTreeIter*)it);

    // printf("ex GUI_tree1_childNr %d\n",ii);

  return ii;

}


//================================================================
  int GUI_tree1_remove__ (MemObj *mo, TreeNode *it) {
//================================================================
/// remove node it and its childs


  // set GUI_tree1_tree, GUI_tree1_view, GUI_tree1_model and GUI_tree1_store
  if(GUI_tree1_decode(mo)) return -1;

  GUI_tree1_childs_remove (NULL, it);

  gtk_tree_store_remove (GUI_tree1_store, (GtkTreeIter*)it);

  return 0;

}


//================================================================
  int GUI_tree1_childs_remove (MemObj *mo, TreeNode *it) {
//================================================================
/// \code
/// remove all childs of node; but not node.
/// recursiv
/// \endcode


  int           irc, ii;
  GtkTreeIter   rowChd;


  // set GUI_tree1_tree, GUI_tree1_view, GUI_tree1_model and GUI_tree1_store
  if(mo) {          // internal call: mo=NULL
    if(GUI_tree1_decode(mo)) return -1;
  }


  L_nxt:
    irc = gtk_tree_model_iter_children (GUI_tree1_model, &rowChd,
                                        (GtkTreeIter*)it);
    if(irc == 0) return 0;

    // get nr of childNodes
    ii = gtk_tree_model_iter_n_children (GUI_tree1_model, &rowChd);

    // remove childs of child (recursion)
    if(ii > 0) GUI_tree1_childs_remove (NULL, (TreeNode*)&rowChd);

    // remove child
    gtk_tree_store_remove (GUI_tree1_store, &rowChd);
    goto L_nxt;

}


//================================================================
  int GUI_tree1_unselect_all (MemObj *mo) {
//================================================================
/// unselect all objects of tree

  GtkTreeSelection *sele;

  if(GUI_tree1_decode(mo)) return -1;

  sele = gtk_tree_view_get_selection (GUI_tree1_view);

  gtk_tree_selection_unselect_all (sele);

  return 0;

}


//================================================================
  int GUI_tree1_expand__ (MemObj *mo, TreeNode *it) {
//================================================================
/// expand row  or all (if it == NULL)

  int              i1;
  GtkTreePath      *tpt;

  // set GUI_tree1_tree, GUI_tree1_view, GUI_tree1_model and GUI_tree1_store
  if(GUI_tree1_decode(mo)) return -1;


  if(!it) {
    gtk_tree_view_expand_all (GUI_tree1_view);
    return 0;
  }

  tpt = gtk_tree_model_get_path (GUI_tree1_model, (GtkTreeIter*)it);
  i1 = gtk_tree_view_expand_row (GUI_tree1_view, tpt, FALSE);
  gtk_tree_path_free (tpt);

  return i1;

}


//================================================================
  int GUI_tree1_ck_expanded (MemObj *mo, TreeNode *it) {
//================================================================
/// \code
/// test if row is expanded
/// RetCod:     0   row is not expanded
///             1   row is expanded
/// \endcode

// parentNode must be expanded !

  int              i1;
  GtkTreePath      *tpt;

  // printf("GUI_tree1_ck_expanded \n");

  // set GUI_tree1_tree, GUI_tree1_view, GUI_tree1_model and GUI_tree1_store
  if(GUI_tree1_decode(mo)) return -1;

  tpt = gtk_tree_model_get_path (GUI_tree1_model, (GtkTreeIter*)it);
  i1 = gtk_tree_view_row_expanded (GUI_tree1_view, tpt);
  gtk_tree_path_free (tpt);
    // printf(" expanded = %d\n",i1);

  return i1;

}


//====================================================================
  int GUI_tree1_par (TreeNode *itPar, MemObj *mo, TreeNode *itChild) {
//====================================================================
/// \code
/// get parentNode of node.
/// Retcode:
///   0        NO PARENT EXIST
///   else  OK
/// \endcode

  if(GUI_tree1_decode(mo)) return -1;

  return gtk_tree_model_iter_parent (GUI_tree1_model, (GtkTreeIter*)itPar,
                                     (GtkTreeIter*)itChild);

}


//========================================================================
  int GUI_tree1_ndPos (char *ndPos, MemObj *mo, TreeNode *it) {
//========================================================================
/// \code
/// get position of a node; returns string.
/// Output:
///   ndPos       char[40]
/// returns string; eg  "0" or "0:2" or "0:2:1" ..
/// nodeName:  "0"   first group
///            "0:0" first subObj of first group
/// \endcode

  gchar            *txt1;

  if(GUI_tree1_decode(mo)) return -1;

  txt1 = gtk_tree_model_get_string_from_iter (GUI_tree1_model, (GtkTreeIter*)it);
    // printf(" treePosString |%s|\n",txt1);

  if(txt1) {
    strcpy(ndPos, txt1);
    g_free (txt1);

  } else {
    ndPos[0] = '\0';
    return -1;
  }


    // printf("GUI_tree1_ndPos %d\n",il);

  return 0;

}


//================================================================
  int GUI_tree1_lev (MemObj *mo, TreeNode *it) {
//================================================================
/// \code
/// get level of a node;
/// level=0=topNode; level=1 is a child of the topNode;
/// level=2 is a child of a child of the topNode.
/// level is the nr of ':'-chars in the treePosString;
///   "0"=0; "0:2"=1; "0:2:1"=2;
/// \endcode

  int              il;
  gchar            *txt1;

  if(GUI_tree1_decode(mo)) return -1;

  txt1 = gtk_tree_model_get_string_from_iter (GUI_tree1_model, (GtkTreeIter*)it);
    // printf(" treePosString |%s|\n",txt1);
  il = UTX_cnr_chr (txt1, ':');
  g_free (txt1);

    // printf("GUI_tree1_lev %d\n",il);


  return il;

}


//=================================================================
  int GUI_tree1_cmp_row (MemObj *mo, TreeNode *it1, TreeNode *it2) {
//=================================================================
/// compare 2 rows if identical;

  int     i1;
  gchar   *txt1, *txt2;

  // printf("GUI_tree1_cmp_row \n");

  if(GUI_tree1_decode(mo)) return -1;

  txt1 = gtk_tree_model_get_string_from_iter (GUI_tree1_model, (GtkTreeIter*)it1);
  txt2 = gtk_tree_model_get_string_from_iter (GUI_tree1_model, (GtkTreeIter*)it2);

  i1 = strcmp(txt1, txt2);

  g_free (txt1);
  g_free (txt2);

  return i1;

}


//================================================================
  int GUI_tree1_set_selMode (MemObj *mo, int mode) {
//================================================================
/// \code
/// deactivate / reactivate selection
/// use if select rows with GUI_tree1_childs_set(,,2)
/// GUI_tree1_set_selMode (tree, 1);  // do not report following selections
/// GUI_tree1_set_selMode (tree, 0);  // report all following selections
/// \endcode

  if(GUI_tree1_decode(mo)) return -1;

  GUI_tree1_ActObj->data = PTR_INT(mode);

}


//===============================================================================
  int GUI_tree1_childs_set (MemObj *mo, TreeNode *it, int mode) {
//===============================================================================
/// \code
/// modify mode (active|passive) of all childs of a row, but not of row.
///   mode      -1=passive; 1=active
/// recursive.
/// \endcode

  int           irc = -1, ii, i1;
  GtkTreeIter   rowChd;


  // printf("GUI_tree1_childs_set %d\n",mode);
 

  // set GUI_tree1_tree, GUI_tree1_view, GUI_tree1_model and GUI_tree1_store
  if(mo) {          // internal call: mo=NULL
    if(GUI_tree1_decode(mo)) goto L_exit;
  }

  // get nr of childNodes
  ii = gtk_tree_model_iter_n_children (GUI_tree1_model, (GtkTreeIter*)it);

  // loop tru childs of row it
  for(i1=0; i1<ii; ++i1) {
    // get childnode
    irc = gtk_tree_model_iter_nth_child (GUI_tree1_model, &rowChd,
                                         (GtkTreeIter*)it, i1);
      // printf(" irc=%d\n",irc);
    if(irc == 0) goto L_exit;

    // get nr of childNodes of childnode
    irc = gtk_tree_model_iter_n_children (GUI_tree1_model, &rowChd);
    // recurse childs of of child
    if(irc > 0) GUI_tree1_childs_set (NULL, (TreeNode*)&rowChd, mode);  //RECURS

    // modify child
    GUI_tree1_row_set (NULL, (TreeNode*)&rowChd, -1, NULL, mode);
  }



  L_exit:

  return irc;

}


//====================================================================
  int GUI_tree1_cbSel (void *selection, MemObj mo) {
//====================================================================
/// INTERNAL

// comes after GUI_tree1_cbMouse;

  int          i1, i2, i3, iEv=TYP_EventPress;
  gchar            *txt1, *txt2;
  GtkTreeIter      itAct;
  GtkTreeModel     *model;
  void         *pTab[7];
  Obj_gui2     *go;
  GList        *list;


  // printf("GUI_tree1_cbSel selMode=%d\n",INT_PTR(GUI_tree1_ActObj->data));

  if(INT_PTR(GUI_tree1_ActObj->data)) return FALSE;  //0=normal;1=skip selection

  // skip deselection  ONLY FOR SINGLE OR BROWSE !
  // if(!gtk_tree_selection_get_selected (selection, &model, &itAct)) {
    // return FALSE;
  // }

  go = GUI_obj_pos (&mo);
  if(!go) return 0;

  i1 = gtk_tree_selection_count_selected_rows (selection);
    // printf(" iNr=%d\n",i1);

  list = gtk_tree_selection_get_selected_rows (selection, &model);

  // g_list_foreach(tmplist, (GFunc)g_object_ref, NULL);
  list = g_list_first(list);

  while (list) {
  i1 = gtk_tree_model_get_iter (model, &itAct, list->data);
  if(i1 == FALSE) { printf("***** GUI_tree1_cbSel E001\n"); goto L_nxt; }



  // decode selected row - get data for first column
  gtk_tree_model_get (model, &itAct, 1, &txt1, -1);  // access Spalte 1=text
    // printf(" sel |%s|\n",txt1);

  gtk_tree_model_get (model, &itAct, 2, &i2, -1);  // access state 1=act, 0=pass
    // printf(" iStat=%d\n",i2);

  gtk_tree_model_get (model, &itAct, 3, &i3, -1);  // access iconNr
    // printf(" ico# = %d\n",i3);

  // // get nodeId of selected node
  // txt2 = gtk_tree_model_get_string_from_iter (model, &itAct);
    // // printf(" nodeID |%s|\n",txt2);


  // report to user
  pTab[0] = &iEv;
  pTab[1] = &GUI_tree1_msbt;
  pTab[2] = &itAct;  // TreeNode
  pTab[3] = txt1;    // text col.1
  pTab[4] = &i3;     // icon-nr
  pTab[5] = &i2;     // state

  go->uFunc (&mo, pTab);


  // g_free (txt2);
  g_free (txt1);

    L_nxt:
    list = g_list_next(list);


  }


  g_list_foreach (list, (GFunc) gtk_tree_path_free, NULL);
  g_list_free (list);

  return FALSE;  // TRUE=1=stop further handling of event ..

}


//================================================================
  int GUI_tree1_cbMouse (void *parent, void *ev, MemObj mo) {
//================================================================
/// INTERNAL

// PRESS comes before GUI_tree1_cbSel; only store buttonTyp.
// RELEASE comes also before GUI_tree1_cbSel;

  int      typ;

  typ  = ((GdkEventAny*)ev)->type;

  // printf("GUI_tree1_cbMouse typ=%d\n",typ);

  if(typ != GDK_BUTTON_PRESS) goto L_exit;

  GUI_tree1_msbt = ((GdkEventButton*)ev)->button;

    // printf("GUI_tree1_cbMouse typ=%d but=%d\n",typ,GUI_tree1_msbt);

  L_exit:
  return FALSE;  // TRUE=1=stop further handling of event ..

}
//====================================================================
  int GUI_tree1_analyz1 (char *sOut, int mode, int iCol, MemObj *mo) {
//====================================================================
/// \code
/// GUI_tree1_analyz1                  Resolv List.
/// Input:
///   mode    0=init-resolv; 1=get-next-listObj 2=get-active-listObj
/// Retcod:
///   >=0     OK, nr_of_columns;
///   -1      no more listObjs
/// \endcode


  int          i1;
  Obj_gui2     *go;
  gchar        *txt1, *txt2=NULL, *txt3=NULL;
  char         s1[128];

static int          nCol;
static GtkTreeIter  it1;


  // printf("GUI_tree1_analyz1 \n");


  //----------------------------------------------------------------
  // go = GUI_obj_pos (mo);
  // if(!go) return 0;
// go->widget = tree_view
// go->data   = list_store
// see GUI_tree1_decode

  if(mode != 0) goto L_1;
  GUI_tree1_ActObj = GUI_obj_pos (mo);
  if(!GUI_tree1_ActObj) return -1;

  if(GUI_tree1_ActObj->widget != GUI_tree1_tree)  {
    GUI_tree1_tree  = GUI_tree1_ActObj->widget;
    GUI_tree1_view  = GTK_TREE_VIEW (GUI_tree1_tree);           // view  < tree
    GUI_tree1_model = gtk_tree_view_get_model (GUI_tree1_view); // model < view
    // GUI_list1_store = GTK_LIST_STORE (GUI_tree1_model);         // list < model
  }

  // get nr of columns
  nCol = gtk_tree_model_get_n_columns (GUI_tree1_model);
    // printf(" nCol=%d\n",nCol);


  // get primary node
  i1 = gtk_tree_model_iter_nth_child (GUI_tree1_model, &it1, NULL, 0);
  // i1 = gtk_tree_model_get_iter_first (model, &itTop);
  if(i1 == 0) {TX_Error("GUI_tree1_analyz1 E001"); return -1;}
  goto L_2;


  //----------------------------------------------------------------
  // get next iter
  L_1:
    i1 = gtk_tree_model_iter_next (GUI_tree1_model, &it1);
      // printf(" i1=%d\n",i1);
    if(i1 == 0)  return -1;


  //----------------------------------------------------------------
  // get value of iter of column iCol
  L_2:
    gtk_tree_model_get (GUI_tree1_model, &it1, iCol, &txt1, -1);
    strcpy(sOut, txt1);
    g_free (txt1);

      // printf(" _analyz1 col[%d]=|%s|\n",iCol,sOut);

  return 0;

}


/* ------*--------- eof ------------------------- */
