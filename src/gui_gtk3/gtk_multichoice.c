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
\file ../gui_gtk3/gtk_multichoice.c
\brief radiobuttons, popup, combo
\code

=====================================================
List_functions_start:

GUI_radiobutt__        RadioButton
GUI_radiobutt_get      test a radiobutton if it is active
GUI_radiobutt_set      set a radiobutton to state = active

GUI_popup__            popup-window with multiple-choice-comboBox.

GUI_optmen__           option-menu (combo-box)
GUI_optmen_set         modify the basic option-menu-item
GUI_optmen_chg         populate option-menu

GUI_optmen_go          INTERNAL callback selection OptionMenu
GUI_optmen_pos         INTERNAL callback OptionMenu - set the position
GUI_optmen_sel         INTERNAL

List_functions_end:
=====================================================


\endcode *//*----------------------------------------


cc -c `pkg-config --cflags gtk+-3.0` -DGTK3 gtk_multichoice.c

======================================================================
*/



// #include "../xa/MS_Def0.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>

#include "../ut/ut_cast.h"              // INT_PTR
#include "../ut/ut_umem.h"                     // Memspc
#include "../gui/gui_types.h"            // TYP_Event..
#include "../gui/gui_base.h"

#include "../gui_gtk3/gtk_base.h"
#include "../gui_gtk3/gtk_button.h"
#include "../gui_gtk3/gtk_multichoice.h"



       void      (*UI_uf_popup)();       // Popupmenu-UserCallbackFunction
static char     **UI_opt_lst;

static Obj_gui2   *UI_optmen_act;        // active GUI_OptMen__


extern int       UI_fontsizX;
extern GtkWidget *UI_act_wi;

extern GdkRGBA   *UI_stylTab[3];


// prototypes internal:




//================================================================
  void GUI_optmen_sel (void *parent, void *data) {
//================================================================
/// INTERNAL;  see GUI_popup_cb1

  int   lNr, iEv=TYP_EventPress;
  void  *pTab[3];


  pTab[2] = (char*)gtk_menu_item_get_label (GTK_MENU_ITEM(parent));
  lNr = INT_PTR(data);

      // printf("GUI_optmen_sel %d |%s|\n",lNr,(char*)pTab[2]);

  // set label = itemText
  gtk_label_set_text (GTK_LABEL(UI_optmen_act->data), pTab[2]);

  // report selection -> user
  pTab[0] = &iEv;
  pTab[1] = &lNr;

  UI_optmen_act->uFunc (UI_optmen_act, pTab);

  return;

}


//=============================================================================
  void GUI_optmen_pos (void* menu, int *x, int *y, int *ii, void *data) {
//=============================================================================
/// INTERNAL callback OptionMenu - set the position of the popup

  // printf("optMenPos\n");

  // gtk_widget_translate_coordinates (
           // gtk_widget_get_toplevel(bt1), bt1, 0, 0, x, y);

  // get position of parent-button
  gdk_window_get_origin (gtk_widget_get_window(data), x, y);
    // printf(" pos=%d %d\n",*x,*y);


  *ii = TRUE; // move window if it gets outside visible area ..

  return;

}


//================================================================
  int GUI_optmen_go (void *parent, void *event, MemObj mo) {
//================================================================
/// \code
/// INTERNAL callback selection OptionMenu
/// activate & place popup (OptionMenu).
/// \endcode

  int        ii;
  Obj_gui2   *go;

  // printf("GUI_optmen_go\n");

  go = GUI_obj_pos (&mo);
  if(!go) return 0;

  UI_optmen_act = go;

  // find place & display a popup-menu
  gtk_menu_popup (GTK_MENU(go->widget), NULL, NULL,
                  (GtkMenuPositionFunc)GUI_optmen_pos, parent, 0, 0);

  gtk_widget_show (go->widget);

  return 0;

}


//=====================================================================
  int GUI_optmen_chg (MemObj *mo,
                      char *optLst[], char *tipLst[]) {
//=====================================================================
/// GUI_optmen_chg  populate / Change the menu of existing OptionMenu.


  int         i1;
  GtkWidget   *menu, *item;
  Obj_Unknown *go;


  // set GUI_ed1_view GUI_ed1_buff
  if(mo) {   // for internal call: mo=NULL
    go = GUI_obj_pos (mo);
    if(!go) return 0;
    menu = go->widget;

  } else {
    menu = UI_act_wi;
  }


  i1=0;

  // poulate list
  if(optLst) {
    while (optLst[i1]) {
      if(strlen(optLst[i1]) < 1) break;
        // printf(" optLst[%d]=|%s|\n",i1,optLst[i1]);

      item = gtk_menu_item_new_with_label (optLst[i1]);  // Gtk2
      // gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(parent),
                                             // optLst[i1]);
      // add tooltip
      if(tipLst) {
        if(tipLst[i1]) gtk_widget_set_tooltip_text (item, tipLst[i1]);
      }

      // clicked,selection_changed gehen nicht?
      g_signal_connect (item, "activate",
                        G_CALLBACK(GUI_optmen_sel), PTR_INT(i1));

      // gtk_menu_item_toggle_size_allocate (item, 144);
      // gtk_widget_set_hexpand (item, FALSE);
      gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
      gtk_widget_show (item);

      ++i1;
    }

  }

  // gtk_widget_set_size_request (GTK_WIDGET(menu), 144, 0);
  // gtk_widget_set_hexpand (menu, FALSE);

  return;

}


//=====================================================================
  MemObj GUI_optmen__ (MemObj *o_par, char *mtxt,
                       char *optLst[], char *tipLst[],
                       void* funcnam, char *opts) {
//=====================================================================
/// \code
/// GUI_OptMen__           option-menu (combo-box)
/// 
/// Input:
///   o_par      parentBox 
///   mtxt       primary labeltext
///   optLst     items; a null-terminated list of words (must not be static)
///   tipLst     list of tooltips or NULL.
///   funcnam    callback-function for selection (returns lNr & selected text)
///   opts       options; (HorSiz,VertSiz)
///              HorSiz,VertSiz:  size in characters; default is automatic size.
///                              'e' = expand widget; default is fixed size.
///              Examples: "" or "10" or "10e,e"
///                "10e,e" horiz. size 10 chars, hor. and vert. expandable.
///
/// funcnam prototype:
///   int funcnam (MemObj *mo, void **data);
///   // data=table of 3 pointers;
///   GUI_DATA_EVENT =*(int*)data[0]=TYP_EventPress  selection
///   GUI_DATA_I1    =*(int*)data[1]=line-nr; first is 0
///   GUI_DATA_S2    =(char*)data[2]=text of selected menu-item
///
///
/// Example:
///   static GIO_OptMen om1;
///   char *optLst[]={"Jaenner","Februar","Maerz",NULL};
///
///   GUI_OptMen__  (&om1, box0, optLst, NULL, OptMen_CB, "");
///   ..
///   int OptMen_CB (void *parent, void **data) {
///     printf("OptMen_CB sel. Line = %d |%s|\n",GUI_DATA_I1,GUI_DATA_S2);
///     GUI_OptMenSet  (parent, 1, GUI_DATA_S2);    // change text of basic item
///     return 0;
///   }
/// \endcode

// Gtk3 cannot trim a label; so trimming is to be done here.



  int         im=0, pTyp;
  int         irc, isx, isy, iex, iey, chrNr;
  void        **p1, *w_par;
  // char        s1[128];
  GtkWidget   *wm1, *eb1, *lb1, *fr1, *box1;
  Obj_gui2    *go;


  // printf("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO \n");
  // printf("GUI_optmen__ |%s|%s|\n",mtxt,opts);
  // UTX_ptab_dump (optLst); // dump optLst


  // get gtk-widget for parentBox from MemObj
  w_par = GUI_obj_parent__ (&pTyp, o_par);
  if(!w_par) return (UME_obj_invalid_set (-3));


  // get address & memspc
  GUI_obj_spc ((void**)&go, sizeof(Obj_gui2));
  if(!go) return (UME_obj_invalid_set (-1));



  // get size
  irc = GUI_w_pack_d (&isx, &isy, &iex, &iey, opts);
  if(isx) chrNr = isx / UI_fontsizX;
  else chrNr = strlen (mtxt);
    // printf(" optMen chrNr=%d\n",chrNr);




  //----------------------------------------------------------------
  // create menu
  UI_act_wi = gtk_menu_new ();
  // gtk_widget_set_hexpand (UI_act_wi, FALSE);
  // gtk_widget_set_vexpand (UI_act_wi, TRUE);
  // gtk_widget_set_size_request (GTK_WIDGET(UI_act_wi), 144, 0);
  // gtk_widget_show (w1);
  // gtk_box_set_child_packing (GTK_BOX(w_par),UI_act_wi,FALSE,FALSE,0,GTK_PACK_START);


  // label = top-level-menu-item
  // strcpy (s1, mtxt);
  // s1[chrNr] = '\0';
  lb1 = gtk_label_new (mtxt);  // optLst[0]);




  go->gio_typ  = TYP_GUI_OptMen;
  go->widget   = UI_act_wi;
  go->uFunc    = funcnam;
  go->data     = lb1;


  // populate list
  GUI_optmen_chg (NULL, optLst, tipLst);


  //----------------------------------------------------------------
  // Gtk3 need eventbox to change background-colour
  eb1 = gtk_event_box_new ();


  // GUI_w_pack1 (pTyp, w_par, eb1, opts);
  // gtk_widget_set_hexpand (eb1, FALSE);
  // gtk_widget_set_vexpand (eb1, TRUE);
  // gtk_widget_set_size_request (GTK_WIDGET(eb1), 144, 0);
  gtk_widget_show (eb1);



  fr1 = gtk_frame_new (NULL);
  // gtk_widget_set_hexpand (fr1, FALSE);
  // gtk_widget_set_vexpand (fr1, FALSE);
  // gtk_widget_set_size_request (GTK_WIDGET(fr1), 144, 0);
  gtk_container_add (GTK_CONTAINER(eb1), fr1);
  gtk_widget_show (fr1);


  // box1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  box1 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  if(iex == 0) {
    gtk_widget_set_hexpand (box1, TRUE);
  } else {
    gtk_widget_set_hexpand (box1, FALSE);
  }
  if(iey == 0) {
    gtk_widget_set_vexpand (box1, TRUE);
  } else {
    gtk_widget_set_vexpand (box1, FALSE);
  }
  gtk_widget_set_size_request (GTK_WIDGET(box1), isx, 0);
  // // makes the label use only one line ..
  // gtk_box_set_homogeneous (GTK_BOX (box1), TRUE);
  gtk_container_add (GTK_CONTAINER(fr1), box1);
  gtk_widget_show (box1);


  gtk_widget_set_tooltip_text (lb1, mtxt);
  gtk_label_set_width_chars (GTK_LABEL(lb1), chrNr);
  gtk_label_set_max_width_chars (GTK_LABEL(lb1), chrNr);
  gtk_label_set_single_line_mode (GTK_LABEL(lb1), TRUE);
  // // NECESSARY: else it takes as much space as necessary .
  gtk_label_set_ellipsize (GTK_LABEL(lb1), PANGO_ELLIPSIZE_END);
  // gtk_label_set_line_wrap (GTK_LABEL(lb1), FALSE);
  // gtk_label_set_justify (GTK_LABEL(lb1), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (lb1), 0.f, 0.5f); // left
  // gtk_widget_set_hexpand (lb1, FALSE);
  // gtk_widget_set_size_request (GTK_WIDGET(lb1), 144, 0);
  // gtk_box_pack_start (GTK_BOX (box1), lb1, FALSE, FALSE, 0);
  // gtk_box_set_child_packing (GTK_BOX (box1),lb1,FALSE,FALSE,0,GTK_PACK_START);
  gtk_container_add (GTK_CONTAINER(box1), lb1);
  gtk_widget_show (lb1);


  gtk_container_add (GTK_CONTAINER (w_par), eb1);
  // gtk_box_pack_start (GTK_BOX (w_par), eb1, FALSE, FALSE, 0);



  // selection of active menu-item: create the menu
  g_signal_connect (GTK_WIDGET (eb1), "button-press-event",
                        G_CALLBACK (GUI_optmen_go),
                        PTR_MEMOBJ(go->mem_obj));

  return (go->mem_obj);

}


//=====================================================================
  int GUI_optmen_set (MemObj *mo, int mode, void *data) {
//=====================================================================
/// \code
/// GUI_OptMenSet          modify the basic option-menu-item
/// Input:
///   mo      the option-menu
///   mode    1=set background-color=activated;
///           0=set to standard background-color, normal.
///           2=disactivate
///           3=reactivate
///   data    text to display; NULL=do not change.
/// 
/// Example see GUI_OptMen__
/// \endcode
    

  Obj_gui2   *go;

  // printf("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS\n");
  // printf("GUI_optmen_set %d |%s|\n",mode,(char*)data);


  go = GUI_obj_pos (mo);
  if(!go) return 0;



  // set startindex; 0=erster, 2=dritter ...
  // gtk_option_menu_set_history (GTK_OPTION_MENU (w1), ind);

  if(data)
    gtk_label_set_text (GTK_LABEL(go->data), data);

  if(mode == 1) {
    gtk_widget_override_color (go->data, 0, UI_stylTab[2]);


  } else if(mode == 0) {
    gtk_widget_override_color (go->data, 0, NULL);


  } else if(mode == 2) {
    gtk_widget_set_sensitive (go->widget, FALSE);


  } else if(mode == 3) {
    gtk_widget_set_sensitive (go->widget, TRUE);

  }


}


//===================================================================
  int GUI_popup__ (char *optLst[], char *tipLst[], int itip,
                   void *funcnam, void *dataLst[]) {
//===================================================================
/// \code
/// create popup-window with multiple-choice-comboBox.
///
/// Inputs:
///   optLst     list of options (a menu)
///   tipLst     list of tooltips or NULL;
///   itip       0=tooltips from tipLst, 1=tooltips from dataLst (using MSG_Tip)
///   funcnam    callback-function (provides the Enter,Leave,Select-events)
///   dataLst    (void**) get this (static) data from the callbackfunktion;
///                NULL for none
/// 
/// prototyp funcnam: 
/// int funcnam (void *parent, void **data);
///   // data=table of 2 pointers;
///   GUI_DATA_EVENT =*(int*)data[0]=TYP_EventEnter|TYP_EventLeave|
///                                  TYP_EventPress  selection
///                                  TYP_EventUnmap  cancel menu, no selection
///   GUI_DATA_I1    =*(int*)data[1]=line-nr; first is 0  (TYP_EventPress)
///   GUI_DATA_S2    =(char*)data[2] user-data of selected item (TYP_EventPress)
///
/// Example:
///   char  *popTxt[]={"txt pop1", "txt pop2", "txt pop3", NULL};
///   char  *popTip[]={"tip pop1", "tip pop2", "tip pop3", NULL};
///   static char *popDat[]={"pop1", "pop2", "pop3", NULL};
///
///   GUI_popup__ (popTxt, popTip, 0, CB_popup, popDat);
/// 
/// int CB_popup (void *parent, void **data) {
///   // callback popup-window
///   int iEv, lNr;
///   // printf("cb_pop1 event=%d device=%d lNr=%d\n",
///     // GUI_DATA_EVENT,  *(int*)data[0]  TYP_EventEnter cursor over menu-item
///     //                                  TYP_EventLeave leave menu-item
///     //                                  TYP_EventPress selection
///     //                                  TYP_EventUnmap cancel menu
///     // GUI_DATA_I1,     *(int*)data[1]  line-nr of selected menu-item
///     // GUI_DATA_S2);    (char*)data[2]  text of selected menu-item
///   iEv = GUI_DATA_EVENT;
///   lNr = GUI_DATA_I1;
///   if(iEv == TYP_EventEnter)      printf(" enter popup-%d\n",lNr);
///   if(iEv == TYP_EventLeave)      printf(" leave popup-%d\n",lNr);
///   if(iEv == TYP_EventPress)      printf(" select popup-%d\n",lNr);
///
/// \endcode


// TODO: get space for tooltips with GUI_obj_spc (see MSG_Tip)


  int            i1, i2;
  GtkWidget      *wact, *menu;


  // printf("GUI_popup__\n");

  menu = gtk_menu_new ();

  i1=0;
  i2 = -3;

  while (optLst[i1]) {
    // printf(" optLst[%d]=|%s|\n",i1,optLst[i1]);
    if(strlen(optLst[i1]) < 1) break;

    wact = gtk_menu_item_new_with_label (optLst[i1]);


    // add tooltip
    if(itip == 0) {
      if(tipLst) {
        if(tipLst[i1]) gtk_widget_set_tooltip_text (wact, tipLst[i1]);
      }
    } else {
      if(dataLst) {
        UI_act_wi = wact;
        MSG_Tip (dataLst[i1]);
      }
    }



    // GTK1:
    // select-func
    // g_signal_connect ( wact, "activate",
                     // G_CALLBACK( funcnam1), (void*)i1);

    // preview
    if(funcnam) {
      // gtk_widget_set_events (GTK_WIDGET(wact),
                        // GDK_VISIBILITY_NOTIFY_MASK|
                        // GDK_FOCUS_CHANGE|
                        // GDK_ENTER_NOTIFY_MASK);

      // NOT: focus-in-event move-scroll cycle-focus activate
      g_signal_connect (wact, "select",
                     G_CALLBACK(GUI_popup_cb1), PTR_INT(i1));
      g_signal_connect (wact, "deselect",
                     G_CALLBACK(GUI_popup_cb1), PTR_INT(i2));

      // g_signal_connect (wact, "enter_notify_event",
      // g_signal_connect (wact, "leave_notify_event",
    }

    gtk_menu_shell_append (GTK_MENU_SHELL (menu), wact);
    // gtk_menu_append (GTK_MENU (menu), wact);  // Gtk2
    gtk_widget_show (wact);

    ++i1;
    --i2;
  }

  UI_uf_popup = funcnam;
  UI_opt_lst  = (char**)dataLst;



  if(funcnam) {
    gtk_widget_set_events (GTK_WIDGET(menu),
                      GDK_VISIBILITY_NOTIFY_MASK|
                      // GDK_FOCUS_CHANGE|
                      GDK_KEY_PRESS_MASK|
                      GDK_BUTTON_RELEASE_MASK|
                      GDK_STRUCTURE_MASK);                 // GDK_UNMAP

    g_signal_connect (menu, "unmap_event",    // "select-cursor-row",
                      G_CALLBACK (GUI_popup_cb2), (void*)-1);
    g_signal_connect (menu, "button_release_event",
                      G_CALLBACK(GUI_popup_cb2), NULL);
    g_signal_connect (menu, "key_press_event",
                      G_CALLBACK(GUI_popup_cb2), NULL);

  }

  gtk_menu_popup (GTK_MENU(menu), NULL,NULL,NULL,NULL,0,0);
  gtk_widget_show (menu);

  i1 = -1;   // init lNr; 2013-03-27
  GUI_popup_cb1 (menu, PTR_INT(i1));

  return 0;

}


//==========================================================================
  int GUI_popup_cb2 (void *parent, void *event, void *data) {
//==========================================================================
/// INTERNAL unmap popup-window callback 

//  7 = GDK_BUTTON_RELEASE
//  8 = GDK_KEY_PRESS
// 10 = GDK_ENTER_NOTIFY
// 11 = GDK_LEAVE_NOTIFY
// 15 = GDK_UNMAP

  int  typ, i1;

  // printf("GUI_popup_cb2 ev=%d\n",((GdkEventAny*)event)->type);
  // printf("GUI_popup_cb2 ev=%d lNr=%d\n",((GdkEventAny*)event)->type,
                                        // INT_PTR(data));
                  
  typ  = ((GdkEventAny*)event)->type;
  
  if(typ == GDK_KEY_PRESS) {
    i1 = ((GdkEventKey*)event)->keyval;
      // printf(" key = %d\n",i1);
    if(i1 != 0xff0d) return FALSE;             // GDK_Return
    i1 = -2;


  } else if (typ == GDK_BUTTON_RELEASE) {
      // printf(" key CR\n");
    // gtk_menu_popdown (parent);
    gtk_widget_hide (parent);
    i1 = -2;


  } else if (typ == GDK_UNMAP) {
      // printf(" unmap\n");
    i1 = -1;
  }

  return GUI_popup_cb1 (parent, PTR_INT(i1));     // (void*)i1);

}


//=========================================================================
  int GUI_popup_cb1 (void *parent, void *data) {
//=========================================================================
/// INTERNAL callback of GUI_Popup

// data: >=0  = activate line
//       -1   = unmap - cancel
//       -2   = select         (does not provide parent !)
//       <=-3 = disactivate line (l * -1 - 3)
  

static int  lNr;
static void *ptx;     // the active menu-txt
  int      ii, iEv=0;
  void     *pTab[4];


  ii = INT_PTR(data);

  // printf("GUI_popup_cb1 ii=%d\n",ii);


  if(ii >= 0) {
    // enter menu-item
      // printf(" GDK_ENTER_NOTIFY\n");
    iEv = TYP_EventEnter;
    lNr = INT_PTR(data);
    // get menu-item-text
    if(UI_opt_lst) {
      ptx = UI_opt_lst[lNr];
    } else {
      ptx = (char*)gtk_menu_item_get_label (GTK_MENU_ITEM(parent));
    }
      // printf(" sel lNr=%d |%s|\n",lNr,(char*)ptx);
    pTab[2] = ptx;


  } else if (ii == -1) {
    // unmap menu
    // skip unmap after select
    if(lNr == -2) return FALSE;
    iEv = TYP_EventUnmap;
    pTab[2] = NULL;
    lNr = -2;


  } else if (ii == -2) {
    // selection
    // skip selection if no menu-item is active
    if(lNr < 0) goto L_exit;
    iEv = TYP_EventPress;
    pTab[2] = ptx;


  } else {
    // leave menu-item
    // skip leave after selection
    if(lNr == -2) return FALSE;
    if(lNr < 0) {printf("**** GUI_popup_cb1 E002\n"); goto L_exit;}
    iEv = TYP_EventLeave;
    lNr = ii * -1 - 3;
    pTab[2] = ptx;
  }


  pTab[0] = &iEv;
  pTab[1] = &lNr;

  (*UI_uf_popup) (parent, pTab);


  L_exit:
  if(iEv == TYP_EventLeave) lNr = -1;
  if(iEv == TYP_EventPress) lNr = -2;

  return FALSE;
  // return TRUE;

}


//=====================================================================
  MemObj GUI_radiobutt__ (MemObj *o_par, char* ltxt, int ii,
                          void *funcnam, void *data, char *opts) {
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
///   opts       options; (HorSiz,VertSiz)
///              HorSiz,VertSiz:  size in characters; default is automatic size.
///                              'e' = expand widget; default is fixed size.
///              Examples: "" or "10" or "10e,e"
///                "10e,e" horiz. size 10 chars, hor. and vert. expandable.
///
/// funcnam prototyp:
/// int funcnam (MemObj *mo, void **data);
///   // data=table of 2 pointers;
///   GUI_DATA_EVENT =*(int*)data[0]=TYP_EventPress|TYP_EventRelease
///   GUI_DATA_I1=*(int*)data[1] or GUI_DATA_S1=(char*)data[1]  user-data
///   GUI_OBJ_TYP(mo)  = TYP_GUI_Button
///
/// Example:
///   GUI_RadButt__  (&box0, "button1", 0, cb_rb1, (void*)"rb1", "");
///   GUI_RadButtAdd (&box0, "button2", 1, cb_rb1, (void*)"rb2", "");
///   GUI_RadButtAdd (&box0, "button3", 1, cb_rb1, (void*)"rb3", "");
///
///   int cb_rb1 (void *parent, void **data) {
///     // skip disactivation
///     if(GUI_DATA_EVENT == TYP_EventRelease) return 0;
///     printf(" being activated: |%s|\n",GUI_DATA_S1;
///   }
///
/// activate:     GUI_radiobutt_set
/// check state:  GUI_radiobutt_get
/// \endcode

// gtk_toggle_button_get_active ()

  static GtkWidget  *rbGrp;

  int           pTyp;
  void          *w_par;
  Obj_gui2      *go;


  // printf("GUI_radiobutt__ %d |%s|%s|\n",ii,ltxt,opts);


  // get gtk-widget for parentBox from MemObj
  w_par = GUI_obj_parent__ (&pTyp, o_par);
  if(!w_par) return (UME_obj_invalid_set (-3));


  // get address & memspc
  GUI_obj_spc ((void**)&go, sizeof(Obj_gui2));
  if(!go) return (UME_obj_invalid_set (-1));


  if(ii == 0) {
    UI_act_wi = gtk_radio_button_new_with_label (NULL, ltxt);
    rbGrp = UI_act_wi;

  } else {
    // UI_act_wi = gtk_radio_button_new_with_label (rbGrp, ltxt);
    UI_act_wi = gtk_radio_button_new_with_label_from_widget (
                  GTK_RADIO_BUTTON (rbGrp), ltxt);
  }

  if(funcnam) {
    g_signal_connect (UI_act_wi, "clicked",  // "group-changed"
                      G_CALLBACK(GUI_radiobutt_press),
                      PTR_MEMOBJ(go->mem_obj));
  }


  // GUI_packwn (w_par, UI_act_wi, siz, 0);
  GUI_w_pack1 (pTyp, w_par, UI_act_wi, opts);


  // fill obj
  go->gio_typ  = TYP_GUI_Button;
  go->widget   = UI_act_wi;
  go->uFunc    = funcnam;
  go->data     = data;

  return (((Obj_Unknown*)go)->mem_obj);


}


//================================================================
  int GUI_radiobutt_get (MemObj *mo) {
//================================================================
/// test a radiobutton if it is active. 1=active, 0=not active.

  void   *go, *wo;

  go = GUI_obj_pos (mo);
  if(!go) return;
  wo = ((Obj_Unknown*)go)->widget;

  return gtk_toggle_button_get_active (wo);

}


//================================================================
  void GUI_radiobutt_set (MemObj *mo) {
//================================================================
/// set a radiobutton to state = active.

  void   *go, *wo;

  go = GUI_obj_pos (mo);
  if(!go) return;
  wo = ((Obj_Unknown*)go)->widget;

  gtk_toggle_button_set_active (wo, TRUE);
  // gtk_toggle_button_set_state (GTK_TOGGLE_BUTTON (w2), FALSE);

  return;

}
// #endif


//================================================================
  int GUI_radiobutt_press (void *parent, MemObj mo) {
//================================================================
/// \code
/// INTERNAL
/// RadButt: activate UND disactivate
/// used by GUI_menu_radiobutt__ TYP_GUI_MenuItem
///       & GUI_menu_checkbox__  TYP_GUI_MenuItem
///       & GUI_radiobutt__      TYP_GUI_Button
/// \endcode


  int        i1, iTyp=TYP_EventPress;
  void       *pTab[2];
  Obj_gui2   *go;


  // printf("GUI_radiobutt_press %d %d\n",mo.mbID,mo.ioff);
  // UMB_dump__ ();
  // GUI_obj_dump__ ();
  // GUI_obj_dump_mo (&mo);

  go = GUI_obj_pos (&mo);
  if(!go) return 0;
    // GUI_obj_dump_o (go);


  if(go->gio_typ == TYP_GUI_Button) {
    // radiobuttons:
    i1 = gtk_toggle_button_get_active (go->widget);

  } else {
    // menu_checkbox | menu_radiobutt
    i1 = gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM (go->widget));
  }

    // printf(" data=|%s| istat=%d\n",(char*)go->data,i1);


  if(i1 == 0) {   // disactivate
    iTyp = TYP_EventRelease;
  } else {        // activate
    iTyp = TYP_EventPress;
  }


  pTab[0] = &iTyp;
  pTab[1] = go->data;

  go->uFunc (&mo, pTab);

}


// EOF
