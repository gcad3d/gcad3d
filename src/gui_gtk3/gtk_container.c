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
2012-02-01 extracted from ut_gtk.c.  RF.

-----------------------------------------------------
*/
#ifdef globTag
void GUI_CONTAINER(){}
#endif
/*!
\file ../gui_gtk3/gtk_container.c
\brief frame, notebook, paned boxes, toolbox
\code

=====================================================
List_functions_start:

GUI_frame__            frame around widgets
// GUI_frame_styl         modify frame

GUI_toolbox__          container for widgets (replacement for GUI_toolbar__)

GUI_box_paned__        get 2 boxes with movable separation-line
GUI_box_paned_siz      modify size of primary paned-box

GUI_notebook__         notebook (tabControl, tabPages)
GUI_notebook_add       add a page to a notebook
GUI_notebook_set       activate page
GUI_notebook_get       query active page

GUI_notebook_CB        INTERNAL

List_functions_end:
=====================================================


\endcode *//*----------------------------------------


cc -c `pkg-config --cflags gtk+-3.0` -DGTK3 gtk_container.c

======================================================================
*/



// #include "../xa/MS_Def0.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <gtk/gtk.h>

#include "../ut/ut_umem.h" 

#include "../gui/gui_types.h"              // TYP_GUI_*
#include "../gui/gui_base.h"          // Obj_Unknown

#include "../gui_gtk3/gtk_base.h"
#include "../gui_gtk3/gtk_container.h"


// gtk_core.c:
extern GtkWidget *UI_act_wi;






//================================================================
int GUI_box_paned_siz (MemObj *mo, int siz) {
//================================================================
/// GUI_box_paned_siz        modify size of primary paned-box

  Obj_Unknown *go;


  // printf("GUI_box_paned_siz %d\n",siz);

  go = GUI_obj_pos (mo);
  if(!go) return 0;

  gtk_paned_set_position (GTK_PANED (go->widget), siz);

  return 0;

}


//=====================================================================
  MemObj GUI_box_paned__ (MemObj *box1, MemObj *box2,
                          MemObj *o_par, int mode, int resi, int siz1) {
//=====================================================================
/// \code
/// GUI_box_paned__          get 2 boxes with movable separation-line
///
/// Input:
///   o_par      parentBox
///   mode       0 = vertical separation-line, returns 2 horizontal boxes
///              1 = horizontal separation-line, returns 2 vertical boxes
///   resi       0 = make box1 fixed, box2 resizable
///              1 = make box1 resizable, box2 fixed.
///   siz1       initial size of box1 in pixels
/// Output:
///   box1
///   box2
/// \endcode


  int         iDir, iTyp, pTyp, border = 4;
  void        *w_par;
  GtkWidget   *w1 , *w2;
  GtkWidget   *hpaned;
  Obj_Unknown *go0, *go1, *go2;


  // printf("GUI_box_paned__ %d %d %d\n",mode,resi,siz1);


  // get gtk-widget for parentBox from MemObj
  w_par = GUI_obj_parent__ (&pTyp, o_par);
  if(!w_par) return (UME_obj_invalid_set (-3));


  // get address & memspc
  GUI_obj_spc ((void**)&go0, sizeof(Obj_Unknown));
  if(!go0) return (UME_obj_invalid_set (-1));


  // get address & memspc
  GUI_obj_spc ((void**)&go1, sizeof(Obj_Unknown));
  if(!go1) return (UME_obj_invalid_set (-1));


  // get address & memspc
  GUI_obj_spc ((void**)&go2, sizeof(Obj_Unknown));
  if(!go2) return (UME_obj_invalid_set (-1));


  if(mode == 0) {
    iDir = GTK_ORIENTATION_HORIZONTAL;
    iTyp = TYP_GUI_BoxH;
  } else {
    iDir = GTK_ORIENTATION_VERTICAL;
    iTyp = TYP_GUI_BoxV;
  }

  hpaned = gtk_paned_new (iDir);

  gtk_container_add (GTK_CONTAINER (w_par), hpaned);

  // gtk_container_set_border_width(GTK_CONTAINER(hpaned), abs(border));

  w1 = gtk_box_new (iDir, 0);
  w2 = gtk_box_new (iDir, 0);

/*
  if(resi == 0) {                        //  resize shrink
    gtk_paned_pack1 (GTK_PANED (hpaned), w1, TRUE,  TRUE);
    gtk_paned_pack2 (GTK_PANED (hpaned), w2, TRUE,  TRUE);

  } else {
    gtk_paned_pack1 (GTK_PANED (hpaned), w1, TRUE,  TRUE);
    gtk_paned_pack2 (GTK_PANED (hpaned), w2, TRUE,  TRUE);
  }
*/
    gtk_paned_pack1 (GTK_PANED (hpaned), w1, FALSE, TRUE);
    gtk_paned_pack2 (GTK_PANED (hpaned), w2, FALSE, TRUE);


  gtk_paned_set_position (GTK_PANED (hpaned), siz1);

  gtk_widget_show (w1);
  gtk_widget_show (w2);
  gtk_widget_show (hpaned);


  go1->gio_typ  = iTyp;
  go1->widget   = w1;

  go2->gio_typ  = iTyp;
  go2->widget   = w2;

  *box1 = go1->mem_obj;
  *box2 = go2->mem_obj;

  go0->gio_typ  = TYP_GUI_Unknown;
  go0->widget   = hpaned;


  return (((Obj_Unknown*)go0)->mem_obj);


}


//=====================================================================
  MemObj GUI_toolbox__ (MemObj *o_par) {
//=====================================================================
// \code
/// GUI_toolbar__            removable container for widgets
///   parent and child must be a box.
///
/// Example:
///  tb = GUI_toolbox__ (&parentBox);
///  box0 = GUI_box_h (&tb, "");
///  GUI_button__ (&box0, ..);
///  GUI_set_show (&tb, 1);
///
/// hide / restore toolbox:   GUI_set_show
/// GUI_toolbox__ can be used after activating its parent-window (GUI_Win_go)
/// \endcode


  int         Id;
  GtkWidget   *box1;
  void        *w_par;
  Obj_Unknown *go;


  // printf("GUI_toolbox__ \n");


  // reload existing window-data     2012-08-23
  Id = GUI_obj_reload (o_par);   // UI_act_Id
  if(Id < 0) return (UME_obj_invalid_set (-4));


  // get gtk-widget for parentBox from MemObj
  w_par = GUI_obj_parentBox (o_par);
  if(!w_par) return (UME_obj_invalid_set (-3));


  // get address & memspc
  GUI_obj_spc ((void**)&go, sizeof(Obj_Unknown));
  if(!go) return (UME_obj_invalid_set (-1));


  // box1 = gtk_toolbar_new ();
  // gtk_toolbar_set_style(GTK_TOOLBAR(box1), GTK_TOOLBAR_TEXT);

  // box1 = gtk_handle_box_new ();
  box1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);

  // gtk_container_add (GTK_CONTAINER (parent), box1);   NO !
  gtk_box_pack_start (GTK_BOX (w_par), box1, FALSE, FALSE, 0);


  gtk_widget_show (box1);

  go->gio_typ  = TYP_GUI_Toolbar;
  go->widget   = box1;


  return (((Obj_Unknown*)go)->mem_obj);

}


/* replaced by GUI_toolbox__
//=====================================================================
  MemObj GUI_toolbar__ (MemObj *o_par) {
//=====================================================================
/// \code
/// GUI_toolbar__            removable container for widgets
///   parent and child must be a box.
///
/// Example:
///  tb = GUI_toolbar__ (&parentBox);
///  box0 = GUI_box_h (&tb, "");
///  GUI_button__ (&box0, ..);
///  GUI_set_show (&tb, 1);
///
/// hide / restore toolbox:   GUI_set_show
/// GUI_toolbar__ can be used after activating its parent-window (GUI_Win_go)
/// \endcode

// gtk_handle_box_set_handle_position
// gtk_handle_box_set_snap_edge

  int         Id;
  GtkWidget   *box1;
  void        *w_par;
  Obj_Unknown *go;


  printf("GUI_toolbar__ \n");


  // reload existing window-data     2012-08-23
  Id = GUI_obj_reload (o_par);   // UI_act_Id
  if(Id < 0) return (UME_obj_invalid_set (-4));
  

  // get gtk-widget for parentBox from MemObj
  w_par = GUI_obj_parentBox (o_par);
  if(!w_par) return (UME_obj_invalid_set (-3));


  // get address & memspc
  GUI_obj_spc ((void**)&go, sizeof(Obj_Unknown));
  if(!go) return (UME_obj_invalid_set (-1));



  // box1 = gtk_toolbar_new ();
  // gtk_toolbar_set_style(GTK_TOOLBAR(box1), GTK_TOOLBAR_TEXT);

  box1 = gtk_handle_box_new ();

  // gtk_container_add (GTK_CONTAINER (parent), box1);   NO !
  gtk_box_pack_start (GTK_BOX (w_par), box1, FALSE, FALSE, 0);


  gtk_widget_show (box1);

  go->gio_typ  = TYP_GUI_Toolbar;
  go->widget   = box1;


  return (((Obj_Unknown*)go)->mem_obj);

}
*/

//================================================================
  int GUI_frame_styl (MemObj *o_par,  int iCol) {
//================================================================
// GUI_frame_styl         modify frame

  int         pTyp, i1;
  Obj_Unknown *go;


  GUI_obj_typ (&pTyp, (void**)&go, o_par);
  if(!pTyp) return (-1);

  // gtk_widget_set_can_focus(go->widget, TRUE);
  // gtk_widget_set_can_default (go->widget, TRUE);
  // gtk_entry_set_has_frame (GTK_ENTRY(go->widget), TRUE);

  if(iCol < 1) {
    gtk_frame_set_shadow_type (go->widget, GTK_SHADOW_IN);
    // gtk_entry_set_has_frame (GTK_ENTRY(go->widget), FALSE);
    // i1 = GTK_SHADOW_IN;
    // i1 = GTK_SHADOW_ETCHED_IN;
    // g_object_set (G_OBJECT(go->widget),"shadow-type", i1, NULL);
    // gtk_widget_restore_default_style (go->widget);
    

  } else {
    gtk_frame_set_shadow_type (GTK_FRAME(go->widget), GTK_SHADOW_OUT);
    // gtk_widget_grab_focus (go->widget);
    // gtk_widget_has_default (go->widget);
    // gtk_entry_set_has_frame (GTK_ENTRY(go->widget), TRUE);
    // i1 = GTK_SHADOW_ETCHED_OUT;
    // i1 = GTK_SHADOW_OUT;
    // i1 = GTK_SHADOW_ETCHED_OUT;
  }
  return 0;

}


//=====================================================================
  MemObj GUI_frame__ (MemObj *o_par, char* ltxt, int border) {
//=====================================================================
/// \code
/// draws a frame around 1-n widgets. Parent & child must be a box.
/// 
///   ltxt       no Text: set to NULL
///   border     outside frame in pixels
/// 
/// Example:
///   w1 = GUI_box_h (&win, 1, "");
///   w2 = GUI_frame__ (&w1, "FrameTitel", 1);
///   w3 = GUI_box_h (&w2, "");
///   w_info = GUI_label__(&w3, "text", "");
/// \endcode


  // GtkWidget *frm1, *w1;
  GtkWidget   *w1;
  void        *w_par;
  Obj_Unknown *go;


  // printf("GUI_frame__ |%s| %d\n",ltxt,border);

  // get gtk-widget for parentBox from MemObj
  w_par = GUI_obj_parentBox (o_par);
  if(!w_par) return (UME_obj_invalid_set (-3));


  // get address & memspc
  GUI_obj_spc ((void**)&go, sizeof(Obj_Unknown));
  if(!go) return (UME_obj_invalid_set (-1));




  w1 = gtk_frame_new (ltxt);
  gtk_container_add (GTK_CONTAINER (w_par), w1);
  gtk_widget_show (w1);

  if (border > 0) {
    gtk_container_set_border_width (GTK_CONTAINER (w1), border);
  }


  // gtk_box_pack_start (GTK_BOX (parent), w1, FALSE, FALSE, 0);
  // gtk_widget_show (w1);

  // frm1 = gtk_hbox_new (FALSE, 0);
  // gtk_container_add (GTK_CONTAINER (w1), frm1);
  // gtk_widget_show (frm1);

/*
#else
  frm1 = gtk_frame_new (ltxt);
  GUI_packwn (parent, frm1, siz, 0);
#endif
*/

  go->gio_typ  = TYP_GUI_Frame;
  go->widget   = w1;


  return (((Obj_Unknown*)go)->mem_obj);

}


//================================================================
  MemObj GUI_notebook__ (MemObj *o_par, void *funcnam) {
//================================================================
/// \code
/// Create a new Notebook.
/// Add Pages with GUI_notebook_add.
/// funcnam is called when noteBook-pages change; first Page has Nr 0.
/// Input:
///   o_par      parentBox
///   funcnam    callbackfunktion for noteBook-pages changes.
///
/// funcnam prototyp:
/// int funcnam (MemObj *mo, void **data);
///     // data=table of 2 pointers;
///     GUI_DATA_EVENT   = TYP_EventEnter
///     GUI_DATA_I1      = pageNumber
///     GUI_OBJ_TYP(mo)  = TYP_GUI_Notebook;
///
/// Example:
///   MemObj ntb, box1, box2;
///   ntb = GUI_notebook__ (&box, my_Notebook_CB);
///   box1 = GUI_notebook_add (&ntb, "Page 1");
///   box2 = GUI_notebook_add (&ntb, "Page 2");
///   ..
///   int my_Notebook_CB (MemObj *mo, void **data) {
///     int     iTyp;
///     if(GUI_DATA_EVENT == TYP_EventRelease) return 0; // skip disactivate
///     iTyp = GUI_OBJ_TYP (mo);
///     if(GUI_DATA_EVENT == TYP_EventEnter) {
///       if(iTyp == TYP_GUI_Notebook)
///         printf(" page %d selected\n",GUI_DATA_I1);
///     }
///   }
///
/// change page:
///   GUI_notebook_set (&ntb, iPagNr);
///
/// \endcode

// Radiobuttons on inactive notebook-pages cannot be preset.


  void        *w_par;
  Obj_gui1    *go;


  // printf("GUI_Notebook__ \n");


  // get gtk-widget for parentBox from MemObj
  w_par = GUI_obj_parentBox (o_par);
  if(!w_par) return (UME_obj_invalid_set (-3));


  // get address & memspc
  GUI_obj_spc ((void**)&go, sizeof(Obj_gui1));
  if(!go) return (UME_obj_invalid_set (-1));


  UI_act_wi = gtk_notebook_new ();


  if (funcnam) {
    g_signal_connect_after (
      G_OBJECT (UI_act_wi), "switch-page",
      G_CALLBACK (GUI_notebook_CB),
      PTR_MEMOBJ(go->mem_obj));
  }

  gtk_container_add (GTK_CONTAINER(w_par), UI_act_wi);

  gtk_widget_show (UI_act_wi);

  go->gio_typ  = TYP_GUI_Notebook;
  go->widget   = UI_act_wi;
  go->uFunc    = funcnam;

  return (((Obj_Unknown*)go)->mem_obj);

}


//======================================================================
  MemObj GUI_notebook_add (MemObj *o_ntb, char *title) {
//======================================================================
/// \code
/// create new page in Notebook.
/// returns a VBOX in the new Notebook-Page.
/// Notebook-Page cannot be disabled (all its widgets are disabled)
///  only hidden
/// \endcode

  int         pTyp;
  void        *w_par, *g_par;
  GtkWidget   *wtmp1, *child;
  Obj_Unknown *go;

  // printf("GUI_notebook_add |%s|\n",title);


  // get parent-typ & parent-obj
  GUI_obj_typ (&pTyp, &g_par, o_ntb);
    // printf(" pTyp=%d\n",pTyp);
  if(pTyp != TYP_GUI_Notebook) {
    printf("***** GUI_notebook_add parent must be notebook ..\n");
    return (UME_obj_invalid_set (-3));
  }
  w_par = ((Obj_Unknown*)g_par)->widget;


  // get memspc for new widget
  GUI_obj_spc ((void**)&go, sizeof(Obj_Unknown));
  if(!go) return (UME_obj_invalid_set (-1));


  wtmp1 = gtk_label_new (title);    // label for notebook-page

  child = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);

  gtk_widget_show (child);

  gtk_notebook_append_page (GTK_NOTEBOOK(w_par), child, wtmp1);

  go->gio_typ  = TYP_GUI_BoxV;
  go->widget   = child;

    // printf("ex GUI_box__\n");

  // return ((void*)gBox);
  return (((Obj_Unknown*)go)->mem_obj);

}


//================================================================
  int GUI_notebook_get (MemObj *o_ntb) {
//================================================================
/// GUI_notebook_get       query active page

  void        *wb;
  Obj_Unknown *go;

  // printf("GUI_notebook_get\n");

  go = GUI_obj_pos (o_ntb);
  if(!go) return 0;
  wb = ((Obj_Unknown*)go)->widget;

  return gtk_notebook_get_current_page (GTK_NOTEBOOK(wb));

}

//================================================================
  void GUI_notebook_set (MemObj *o_ntb, int pgNr) {
//================================================================
/// activate notebook-page; first=0, next=1 ..

  void        *wb;
  Obj_Unknown *go;

  // printf("GUI_notebook_set %d\n",pgNr);

  go = GUI_obj_pos (o_ntb);
  if(!go) return;
  wb = ((Obj_Unknown*)go)->widget;


  gtk_notebook_set_current_page (GTK_NOTEBOOK(wb), pgNr);

}

/*
//================================================================
  int GUI_notebook_enable (MemObj *o_ntb, int pgNr, int mode) {
//================================================================
// does not disable notebook-page !

  GtkWidget   *ntbPg;
  void        *wb;
  Obj_Unknown *go;


  go = GUI_obj_pos (o_ntb);
  if(!go) return;
  wb = ((Obj_Unknown*)go)->widget;

  ntbPg = gtk_notebook_get_nth_page (GTK_NOTEBOOK(wb), pgNr);

  gtk_widget_set_sensitive (ntbPg, mode);

  return 0;

}
*/


//===================================================================
  int GUI_notebook_CB (void *ntb, void *nbp, int pgNr, MemObj mo) {
//===================================================================
/// INTERNAL callback of switch-notebook-page


  int         iTyp=TYP_EventEnter;
  void        *pTab[2];
  Obj_gui1    *go;



  // printf("GUI_notebook_CB %d\n",pgNr);


  go = GUI_obj_pos (&mo);
  if(!go) return 0;
    // GUI_obj_dump_o (go);


  pTab[0] = &iTyp;
  pTab[1] = &pgNr;

  go->uFunc (&mo, pTab);

  return TRUE;

}

// EOF
