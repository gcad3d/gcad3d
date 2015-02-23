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
Gtk3: replacing gtk_menu_item_set_right_justified does not work ..

-----------------------------------------------------
Modifications:
2012-02-01 extracted from ut_gtk.c.  RF.

-----------------------------------------------------
*/
#ifdef globTag
void GUI_CONTAINER(){}
#endif
/*!
\file ../gui_gtk3/gtk_menu.c
\brief menus
\code

=====================================================
List_functions_start:

GUI_menu__             create menu or submenu
GUI_menu_entry         create menu-item in menu

GUI_menu_radiobutt__
GUI_menu_radiobutt_get
GUI_menu_radiobutt_set

GUI_menu_checkbox__
GUI_menu_checkbox_get
GUI_menu_checkbox_set

List_functions_end:
=====================================================

GUI_MenuSub UNUSED

\endcode *//*----------------------------------------


cc -c `pkg-config --cflags gtk+-3.0` -DGTK3 gtk_menu.c

# menu unvisible:     UBUNTU_MENUPROXY=libappmenu.so
if [ -n "$UBUNTU_MENUPROXY" ]; then
  echo "disactivate ubuntu-global-menu"
  export UBUNTU_MENUPROXY=
fi


======================================================================
*/

// ex ../ut/ut_geo.h:


// #include "../xa/MS_Def0.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <gtk/gtk.h>

#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_umem.h"

#include "../gui/gui_types.h"
#include "../gui/gui_base.h"

#include "../gui_gtk3/gtk_button.h"
#include "../gui_gtk3/gtk_multichoice.h"
#include "../gui_gtk3/gtk_menu.h"


// gtk_core.c:
extern GtkWidget *UI_act_wi;

static GSList *UI_menGrp;




//=====================================================================
  MemObj GUI_menu_radiobutt__ (MemObj *o_par, char* ltxt, int ii,
                               void *funcnam, void *data) {
//=====================================================================
/// \code
/// Radiobuttons.
/// The callbackfunctions provides signal "activated" and "disactivated"
///
/// Input:
///   o_par      parentBox
///   ii         0=start new radioButtonChain; 1=add button to existing chain
///   ltxt       caption
///   funcnam    callback-function or NULL; returns data of selected RB
///   data       this data will be provided by the callback-function
///
/// funcnam prototyp:
/// int funcnam (void *parent, void **data);
///   // data=table of 2 pointers;
///   GUI_DATA_EVENT =*(int*)data[0]=TYP_EventPress|TYP_EventRelease
///   GUI_DATA_I1=*(int*)data[2] or GUI_DATA_S1=(char*)data[2]  user-data
///   GUI_OBJ_TYP(mo)  = TYP_GUI_MenuItem
///
/// Example:
///   GUI_menu_radiobutt__ (&box0, "button1", 0, cb_rb1, (void*)"rb1", 0);
///   GUI_menu_radiobutt__ (&box0, "button2", 1, cb_rb1, (void*)"rb2", 0);
///   GUI_menu_radiobutt__ (&box0, "button3", 1, cb_rb1, (void*)"rb3", 0);
///
///   int cb_rb1 (void *parent, void **data) {
///     // skip disactivation
///     if(GUI_DATA_EVENT == TYP_EventRelease) return 0;
///     printf(" being activated: |%s|\n",GUI_DATA_S1;
///   }
///
/// activate:     GUI_menu_radiobutt_set
/// check state:  GUI_menu_radiobutt_get
/// \endcode

// gtk_toggle_button_get_active ()

  static GtkWidget  *rbGrp;

  int           pTyp;
  void          *w_par;
  Obj_gui2      *go, *g_par;


  // printf("GUI_menu_radiobutt__ %d |%s|\n",ii,ltxt);


  // get parent-typ & parent-obj
  GUI_obj_typ (&pTyp, (void*)&g_par, o_par);
  if(!pTyp) return (UME_obj_invalid_set (-3));
    // printf(" pTyp=%d\n",pTyp);
  w_par = g_par->widget;


  // get address & memspc
  GUI_obj_spc ((void**)&go, sizeof(Obj_gui2));
  if(!go) return (UME_obj_invalid_set (-1));


  if(ii == 0) {
    UI_act_wi = gtk_radio_menu_item_new_with_label (NULL, ltxt);
    rbGrp = UI_act_wi;

  } else {
    UI_act_wi = gtk_radio_menu_item_new_with_label_from_widget (
                  GTK_RADIO_MENU_ITEM (rbGrp), ltxt);
  }

  if(funcnam) {
    gtk_widget_set_events (GTK_WIDGET(UI_act_wi),
                      GDK_VISIBILITY_NOTIFY_MASK);
    g_signal_connect (UI_act_wi, "activate",
                      G_CALLBACK(GUI_radiobutt_press),
                      PTR_MEMOBJ(go->mem_obj));
  }


  gtk_menu_shell_append (GTK_MENU_SHELL (w_par), UI_act_wi);
  gtk_widget_show (UI_act_wi);


  // fill obj
  go->gio_typ  = TYP_GUI_MenuItem;
  go->widget   = UI_act_wi;
  go->uFunc    = funcnam;
  go->data     = data;

  return (((Obj_Unknown*)go)->mem_obj);


}


//================================================================
  int GUI_menu_radiobutt_get (MemObj *mo) {
//================================================================
/// test a menu-radiobutton if it is active. 1=active, 0=not active.

  void   *go, *wo;

  go = GUI_obj_pos (mo);
  if(!go) return;
  wo = ((Obj_Unknown*)go)->widget;

  return gtk_check_menu_item_get_active (wo);

}


//================================================================
  void GUI_menu_radiobutt_set (MemObj *mo) {
//================================================================
/// set a menu-radiobutton to state = active.

  void   *go, *wo;

  // printf("GUI_menu_radiobutt_set \n");

  go = GUI_obj_pos (mo);
  if(!go) return;
  wo = ((Obj_Unknown*)go)->widget;

  gtk_check_menu_item_set_active (wo, TRUE);
  // gtk_toggle_button_set_state (GTK_TOGGLE_BUTTON (w2), FALSE);

  return;

}


//================================================================
  int GUI_menu_checkbox_get (MemObj *mo) {
//================================================================
/// returns state of menu-checkbox; 0=not sel, 1=selected.

  Obj_Unknown *go;

  // printf("GUI_menu_checkbox_get\n");

  go = GUI_obj_pos (mo);
  if(!go) return -1;

  // return GTK_TOGGLE_BUTTON (cb)->active;
  return gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM (go->widget));

}


//================================================================
  int GUI_menu_checkbox_set (MemObj *mo, int mode) {
//================================================================
/// \code
/// activate/disactivate menu-Checkbox, Radiobutton.
/// Input:
///   mode    1 = TRUE  = ON
///           0 = FALSE = OFF
/// \endcode

  Obj_Unknown *go;

  // printf("GUI_menu_checkbox_set %d\n",mode);

  go = GUI_obj_pos (mo);
  if(!go) return -1;

  gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (go->widget), mode);

  return 0;

}


//=====================================================================
  MemObj GUI_menu_checkbox__ (MemObj *o_par, char* ltxt, int state,
                              void *funcnam, void *data) {
//=====================================================================
/// \code
/// Checkbox.
///   o_par      parentBox
///   ltext      caption (button-text)
///   state:     0=FALSE: not checked; 1=TRUE: checked.
///   funcnam    callbackfunktion for the press-button-event
///   data       get this data from the callbackfunktion; NULL for none
///
/// funcnam prototyp:
///   int funcnam (MemObj *mo, void **data); 
///   // data=table of 2 pointers;
///   GUI_DATA_EVENT =*(int*)data[0]=TYP_EventPress|TYP_EventRelease
///   GUI_DATA_I1=*(int*)data[1] or GUI_DATA_S1=(char*)data[1]  user-data
///   GUI_OBJ_TYP(mo)  = TYP_GUI_MenuItem
///
/// ---------------------------
/// Example without callback:
///
///   int istate;
///   MemObj  cb1;
///   cb1 = GUI_menu_checkbox__ (&men1, "ckb 1", 0, NULL, NULL);
///   // query state
///   istate = GUI_menu_checkbox_get (&cb1)        // 0=not sel, 1=selected.
///
///
/// ---------------------------
/// Example with callback:
///
///  GUI_menu_checkbox__ (&men1, "ckb 1", 0, mywin3, (void*)"cb1");
///
///  int mywin3, (MemObj *parent, void **data) {
///
///    if(GUI_DATA_EVENT == TYP_EventPress) {
///      if(!strcmp(GUI_DATA_S1, "cb1")) {
///        ..
///
/// ---------------------------
/// Optional:
///
/// activate with:
///   GUI_menu_checkbox_set (&cb1, TRUE);
/// \endcode



  int        pTyp;
  void       *w_par;
  Obj_gui2   *go, *g_par;


  // printf("GUI_menu_checkbox__ |%s| %d\n",ltxt,state);


  // get parent-typ & parent-obj
  GUI_obj_typ (&pTyp, (void*)&g_par, o_par);
  if(!pTyp) return (UME_obj_invalid_set (-3));
    // printf(" pTyp=%d\n",pTyp);
  w_par = g_par->widget;


  // get address & memspc
  GUI_obj_spc ((void**)&go, sizeof(Obj_gui2));
  if(!go) return (UME_obj_invalid_set (-1));


  UI_act_wi = gtk_check_menu_item_new_with_label (ltxt);

  gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (UI_act_wi), state);


  if (funcnam) {
    g_signal_connect (UI_act_wi, "activate",
                      G_CALLBACK(GUI_radiobutt_press),
                      PTR_MEMOBJ(go->mem_obj));
  }

  gtk_menu_shell_append (GTK_MENU_SHELL (w_par), UI_act_wi);
  gtk_widget_show (UI_act_wi);


  go->gio_typ  = TYP_GUI_MenuItem;
  go->widget   = UI_act_wi;
  go->uFunc    = funcnam;
  go->data     = data;


  return (((Obj_Unknown*)go)->mem_obj);

}


//================================================================
  MemObj GUI_menu__ (MemObj *o_par, char *ltxt, int mode) {
//================================================================
/// \code
/// GUI_Menu__             create menu or submenu
///
/// Input:
///   o_par        parentbox or parent-menushell
///   ltxt         menu-title, NULL for creating the menu-bar
///   mode         0=normal; 'r'=right-justified; 's'=Submenu.
///                1 (ltxt=NULL only) expand inside parent-container
///
/// Example:
/// MemObj  menu_bar, men_fil, mns, men_hlp, wm2, wml;
/// menu_bar = GUI_menu__ (&box1, NULL, 0);               // create Menubar
/// men_fil = GUI_menu__ (&menu_bar, "File ", 0);         // first Menu on Menubar
/// men_hlp = GUI_Menu__ (&menu_bar, "Help ", 'r');       // Menu rechts
/// GUI_menu_entry (men_fil, "New", UI_CB, (void*)"new"); // add entry to Menu
///
/// mns = GUI_menu__ (&men_fil, "Save as ", 0);           // add submenu
/// GUI_menu_entry (&mns, "dxf", UI_CB, (void*)"saveDxf");
///
/// GUI_menu_entry  (&men_fil, "---", NULL, NULL);        // menu-separatorline
///
/// GUI_menu_checkbox__ (&men_fil, "wmcb1", 0, UI_CB, (void*)"wmcb1");
///
/// wm2 = GUI_menu__ (&menu_bar, "Lang ", 'r');
///       GUI_menu_radiobutt__ (&wm2, "de", 0, mywin3, "l-de", 0);
/// wml = GUI_menu_radiobutt__ (&wm2, "en", 1, mywin3, "l-en", 0);
/// GUI_menu_radiobutt_set (&wml);               // preset en
///
/// int UI_CB (MemObj *mo, void **data) {
///   printf("UI_CB %d\n",GUI_DATA_EVENT);
///   if(GUI_DATA_EVENT == TYP_EventPress) {
///     if(GUI_OBJ_TYP (mo) == TYP_GUI_MenuItem) {
///       if(!strcmp(GUI_DATA_S1, "new")) {
///       ..
///
/// \endcode


  int         pTyp;
  GtkWidget   *w1;
  void        *w_par;
  Obj_Unknown *g_par, *go;


  // printf("GUI_menu__ |%s| %d\n",ltxt,mode);


  // get parent-typ & parent-obj
  GUI_obj_typ (&pTyp, (void*)&g_par, o_par);
  if(!pTyp) return (UME_obj_invalid_set (-3));
    // printf(" pTyp=%d\n",pTyp);

  w_par = g_par->widget;

  // get address & memspc
  GUI_obj_spc ((void**)&go, sizeof(Obj_Unknown));
  if(!go) return (UME_obj_invalid_set (-1));


  // test if parent is a box; yes: add a menu-bar, else a menu.
  if((pTyp == TYP_GUI_BoxH)||(pTyp == TYP_GUI_BoxV)) goto L_menubar;
  goto L_submenu;


  //----------------------------------------------------------------
  L_menubar:
    // printf(" menu_bar\n");
  UI_act_wi = gtk_menu_bar_new ();
  gtk_container_add (GTK_CONTAINER (w_par), UI_act_wi);
  if(mode == 1) gtk_widget_set_hexpand (UI_act_wi, TRUE);  
  gtk_widget_show (UI_act_wi);
  goto L_done;



  //----------------------------------------------------------------
  L_submenu:
    // printf(" menu_shell\n");
  w1 = gtk_menu_item_new_with_label (ltxt);
  UI_act_wi = gtk_menu_new ();

  if(mode == 'r') {
    gtk_widget_set_hexpand (UI_act_wi, TRUE);
    gtk_widget_set_halign (UI_act_wi, GTK_ALIGN_END);
  }

  gtk_menu_item_set_submenu (GTK_MENU_ITEM (w1), UI_act_wi);

  if(!ltxt) {
    gtk_menu_shell_append (GTK_MENU_SHELL (w_par), w1);
  } else {
    gtk_menu_shell_append (GTK_MENU_SHELL (w_par), w1);
  }

  gtk_widget_show (w1);


  //----------------------------------------------------------------
  L_done:
  go->gio_typ  = TYP_GUI_MenuShell;
  go->widget   = UI_act_wi;


  return (go->mem_obj);

}


//=====================================================================
  MemObj GUI_menu_entry (MemObj *o_par, char* ltxt,
                         void *funcnam, void *data) {
//=====================================================================
/// \code
/// create menuItem or MenuSeparator (if ltxt == "---")
///
/// Input:
///   o_par       parentBox
///   ltxt        item-text
///   funcnam     callback from selection
///   data        get this data from the callbackfunktion; NULL for none
///
/// funcnam prototype:
///   int funcnam (void *parent, void **data);
///   // data=table of 2 pointers;
///   GUI_DATA_EVENT =*(int*)data[0]=TYP_EventPress
///   GUI_DATA_I1=*(int*)data[2] or GUI_DATA_S1=(char*)data[2]  user-data
///
///   RetCod:  gio_typ = TYP_GUI_OptMen
///
/// Example: see GUI_menu__
///
/// \endcode



  int         pTyp;
  void        *w_par;
  Obj_Unknown *g_par;
  Obj_gui2    *go;


  // printf("GUI_menu_entry |%s|\n",ltxt);


  // get parent-typ & parent-obj
  GUI_obj_typ (&pTyp, (void*)&g_par, o_par);
  if(!pTyp) return (UME_obj_invalid_set (-3));
    // printf(" pTyp=%d\n",pTyp);

  w_par = g_par->widget;

  // get address & memspc
  GUI_obj_spc ((void**)&go, sizeof(Obj_gui2));
  if(!go) return (UME_obj_invalid_set (-1));



  // kein ltxt: einen Separator
  if (!strcmp(ltxt, "---")) {
    UI_act_wi = gtk_menu_item_new ();

  } else {
    UI_act_wi = gtk_menu_item_new_with_label (ltxt);
    g_signal_connect ( UI_act_wi, "activate",
                      G_CALLBACK (GUI_button_press),
                      PTR_MEMOBJ(go->mem_obj));
  }


  gtk_menu_shell_append (GTK_MENU_SHELL (w_par), UI_act_wi);
  gtk_widget_show (UI_act_wi);

  go->gio_typ  = TYP_GUI_MenuItem;
  go->widget   = UI_act_wi;
  go->uFunc    = funcnam;
  go->data     = data;

  return (go->mem_obj);

}

// EOF
