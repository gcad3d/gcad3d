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
2012-09-14 GUI_entry__ TYP_EventPress Returncodes added; RF.
2012-02-01 extracted from ut_gtk.c.  RF.

-----------------------------------------------------
*/
#ifdef globTag
void GUI_ENTRY(){}
#endif
/*!
\file ../gui_gtk3/gtk_entry.c
\brief Entry (input)
\code

=====================================================
List_functions_start:

GUI_entry__            inputfield for text
GUI_entry_styl         modify frame of entry
GUI_entry_get          get text from entryField - gives back a Pointer
GUI_entry_copy         get a copy of the text from entryField
GUI_entry_set          set text in entryField
GUI_entry_pos_get      get cursorPosition
GUI_entry_pos_set      set cursorPosition
GUI_entry_sel_del      clear selected text

GUI_entry_cb           INTERNAL

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



======================================================================
*/





// #include "../xa/MS_Def0.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <gtk/gtk.h>


#include "../ut/ut_umem.h"                     // Memspc

#include "../gui/gui_types.h"                  // TYP_Event..
#include "../gui/gui_base.h"

#include "../gui_gtk3/gtk_base.h"
#include "../gui_gtk3/gtk_entry.h"


// gtk_core.c:
extern GtkWidget *UI_act_wi;
extern int       UI_fontsizX, UI_fontsizY;



//=====================================================================
  int GUI_entry_styl (MemObj *o_par,  int imod) {
//=====================================================================
/// \code
/// GUI_entry_styl    modify frame of entry;
/// imod      0=GUI_STYL_normal=def_frame;  1=GUI_STYL_activ=red_frame.
/// \endcode

// works not: gtk_widget_override_background_color
// works:     gtk_widget_override_color

// see GUI_button_styl

  
  static GtkCssProvider *provider = NULL;

  GtkStyleContext *context;


  int         pTyp, i1;
  Obj_Unknown *go;



  GUI_obj_typ (&pTyp, (void**)&go, o_par);
  if(!pTyp) return (-1);

  // gtk_widget_set_can_focus(go->widget, TRUE);
  // gtk_widget_set_can_default (go->widget, TRUE);
  // gtk_widget_has_default (go->widget);
  // gtk_entry_set_has_frame (GTK_ENTRY(go->widget), TRUE);

  context = gtk_widget_get_style_context (go->widget);

  if(!provider) {
    provider = gtk_css_provider_new ();
    gtk_css_provider_load_from_data (GTK_CSS_PROVIDER (provider),
      "* {border-color: #CC0000}",
      -1, NULL);
  }


  if(imod > 0) {   // GUI_STYL_activ

    gtk_style_context_add_provider (context,
                                GTK_STYLE_PROVIDER (provider),
                                GTK_STYLE_PROVIDER_PRIORITY_USER); // =highest

    // gtk_entry_set_has_frame (GTK_ENTRY(go->widget), FALSE);
    // g_object_set (G_OBJECT(go->widget),"shadow-type", i1, NULL);
    // gtk_widget_restore_default_style (go->widget);

  } else {         // GUI_STYL_normal
    gtk_style_context_remove_provider (context, GTK_STYLE_PROVIDER(provider));
    // gtk_entry_set_has_frame (GTK_ENTRY(go->widget), TRUE);
    // gtk_widget_override_color (go->widget, GTK_STATE_NORMAL, &col1);
    // gtk_widget_override_background_color (go->widget, GTK_STATE_NORMAL, &col1);
    // gtk_widget_grab_focus (go->widget);
    // g_object_set (G_OBJECT(go->widget),"shadow-type", i1, NULL);
  }


  return 0;

}


//================================================================
   int GUI_entry_pos_get (MemObj *o_par) {
//================================================================
/// \code
/// GUI_entry_pos_get   get cursorPosition
/// retCod = position; 0=leftmost.
/// \endcode

  int         pTyp;
  Obj_Unknown *go;

  GUI_obj_typ (&pTyp, (void**)&go, o_par);
  if(!pTyp) return (-1);


  return gtk_editable_get_position ((GtkEditable*)go->widget);

}


//================================================================
   int GUI_entry_pos_set (int cPos, MemObj *o_par) {
//================================================================
/// GUI_entry_pos_set   set cursorPosition
/// cPos -1 = last pos.

  int         pTyp;
  Obj_Unknown *go;

  GUI_obj_typ (&pTyp, (void**)&go, o_par);
  if(!pTyp) return (-1);


  gtk_editable_set_position ((GtkEditable*)go->widget, cPos);

  return 0;

}


//================================================================
  int GUI_entry_copy (char *cbuf, int sSiz, MemObj *mo) {
//================================================================
/// \code
/// GUI_Entry_Get get text from entryField - copy
/// sSiz    size of cbuf
/// \endcode

  char         *cp1;                                                  
  Obj_Unknown  *go;
  
  go = GUI_obj_pos (mo);
  if(!go) return -1;

  cp1 = (char*)gtk_entry_get_text(GTK_ENTRY(go->widget));

  if(strlen(cp1) >= sSiz) return -1;

  strcpy(cbuf, cp1);

  return 0;

}


//================================================================
  char* GUI_entry_get (MemObj *mo) {
//================================================================
/// \code
/// GUI_entry_get get text from entryField - get pointer
/// \endcode

  void   *go, *wo;
  
  go = GUI_obj_pos (mo);
  if(!go) return NULL;
  wo = ((Obj_Entry*)go)->entry;

  return (char*)gtk_entry_get_text(GTK_ENTRY(wo));

}


//================================================================
  int GUI_entry_set (MemObj *mo, char* text) {
//================================================================
/// set text in entryField

  void   *go, *wo;

  go = GUI_obj_pos (mo);
  if(!go) return -1;
  wo = ((Obj_Entry*)go)->entry;

  gtk_entry_set_text (GTK_ENTRY(wo), text);

  return 0;

}


//=====================================================================
  MemObj GUI_entry__ (MemObj *o_par, char* ltxt, char* etxt,
                      void *funcnam, void *data, char *opts) {
//=====================================================================
///  
/// \code
/// Create text-inputfiled. Gtk-2: siza cannot be smaller than ~10 characters ..
///
///   ltxt:      Labeltext; NULL or "" for none
///   etxt:      Text in Entryfield; NULL oder "" for none
///   data       get this (static) data from the callbackfunktion; NULL for none
///   funcnam:   callback-function for Keypress & FocusIn-Events (none: NULL)
///   opts       options;  must have correct sequence (Frame Side HorSiz VertSiz)
///              Frame: 'f' make a frameless inputfield; default = with frame.
///              Side:  'r' labeltext is right of inputfield; default is left.
///              HorSiz,VertSiz:  size in characters; default is automatic size. 
///                              'e' = expand widget; default is fixed size.
///              Examples: "f,20" or "r" or "r,10" or "10e,e"
///                "r"     label right of inputfield; defaults.
///                "r,10"  label right, horiz. size 10 chars, vert. size autom.
///                "10e,e" horiz. size 10 chars, hor. and vert. expandable.
///
/// prototyp funcnam:
/// int funcnam (MemObj *mo, void **data); 
///   // data=table of 4 pointers;
///   GUI_DATA_EVENT =*(int*)data[0]=TYP_EventEnter|
///                                  TYP_EventPress|
///                                  TYP_EventRelease
///   GUI_DATA_S1    =(char*)data[1]=userdata
///   GUI_DATA_I2    =*(int*)data[2]=keyvalue; eg 'a'
///   GUI_DATA_I3    =*(int*)data[3]=state of modifierkeys;
///                                  see ../gui/gui_types.h  GUI_Modif_*
///                                  &1=shift; &4=ctrl; &8=alt.
///                                  &256=MB1; &512=MB2; &1024=MB3.
///   GUI_OBJ_TYP(mo)  = TYP_GUI_Entry
///   Returncodes:    0=continue with defaultOperations for this key.
///                   1=do no defaultOperations; skip handling this key
/// 
/// Example without callback-function:
///   we1 = GUI_entry__ (&box1, "0.0", "sum", NULL, NULL, "");
///
///   printf(" |%s|\n",GUI_entry_get(&we1));
///
/// Example with callback-function:
///   we2 = GUI_entry__ (&box1, "0.0", "sum", went_cb, NULL, "10");
///   ..
///   int we2_cb (MemObj *mo, void **data) {
///     printf(" event=%d typ=%d\n",GUI_DATA_EVENT,GUI_OBJ_TYP(mo));
///     if(GUI_DATA_I2 == GUI_KeyCurDown) return 1;       // skip handling
///     if(GUI_DATA_EVENT != TYP_EventRelease) return 0;  // continue normal
///     printf(" |%s|\n",GUI_entry_get(mo));
///     ..
///   }
///
/// \endcode



  int        is, i1, pTyp;
  char       c1;
  void       *w_par, *wb;
  GtkWidget  *w1, *w2, *wf, *box1;
  Obj_Entry  *gEnt;


  // printf("GUI_entry__ |%s|%s|%s|\n",ltxt,etxt,opts);

  // get gtk-widget for parentBox from MemObj
  w_par = GUI_obj_parent__ (&pTyp, o_par);
  if(!w_par) return (UME_obj_invalid_set (-3));


  // get address & memspc
  GUI_obj_spc ((void**)&gEnt, sizeof(Obj_Entry));
  if(!gEnt) return (UME_obj_invalid_set (-1));



  //----------------------------------------------------------------
  w1 = gtk_entry_new ();
  UI_act_wi = w1;              // GUI_Tip

  if (funcnam) {
    gtk_widget_set_events (GTK_WIDGET(w1),
                           GDK_FOCUS_CHANGE_MASK|
                           GDK_BUTTON_RELEASE_MASK);

    g_signal_connect (G_OBJECT (w1),
                        "key-press-event",
                        G_CALLBACK (GUI_entry_cb),
                        PTR_MEMOBJ(gEnt->mem_obj));

    g_signal_connect (G_OBJECT (w1),
                        "key-release-event",
                        // "key-press-event",
                        G_CALLBACK (GUI_entry_cb),
                        PTR_MEMOBJ(gEnt->mem_obj));    //(void*)gEnt);

    g_signal_connect (G_OBJECT (w1),
                        "focus-in-event",
                        G_CALLBACK (GUI_entry_cb),
                        PTR_MEMOBJ(gEnt->mem_obj));    //(void*)gEnt);
  }


  // preset Text
  i1 = 0;
  if(etxt) i1 = strlen(etxt);
  if(i1 > 0) {
    /* printf("preset Text /%s/\n",etxt); */
    gtk_entry_set_text (GTK_ENTRY(w1), etxt);
  }


  //----------------------------------------------------------------
  c1 = GUI_opts_get1 (&opts, "f");  // 1=default
  if(c1 == 'f') {
    gtk_entry_set_has_frame (GTK_ENTRY(w1), FALSE);
/*
  } else {
    gtk_entry_set_has_frame (GTK_ENTRY(w1), FALSE);
    wf = gtk_frame_new (NULL);
    gtk_container_add (GTK_CONTAINER (w_par), wf);
    gtk_frame_set_shadow_type (GTK_FRAME(wf), GTK_SHADOW_ETCHED_OUT);
    gtk_widget_set_can_focus(wf, TRUE);
    gtk_widget_set_can_default (wf, TRUE);
    gtk_frame_set_shadow_type (GTK_FRAME(wf), GTK_SHADOW_OUT);
    gtk_widget_show (wf);
    w_par = wf;
    pTyp = TYP_GUI_Frame;
*/
  }
  // gtk_entry_set_inner_border (GTK_ENTRY(w1), NULL);


  // pack & size without label
  if(ltxt) goto L_WL;

  GUI_w_pack1 (pTyp, w_par, w1, opts);

  goto L_done;



  //----------------------------------------------------------------
  // pack & size with label
  L_WL:


  // if parent is horiz.Box: OK; if it is vertic.Box: insert horiz.Box.
  if(pTyp != TYP_GUI_BoxH) {

    box1 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);

    gtk_container_add (GTK_CONTAINER (w_par), box1);
    // gtk_box_pack_start (GTK_BOX (w_par), box1, FALSE,FALSE, 0);
    gtk_widget_show (box1);

    w_par = box1;

  }



    // printf("Labelext /%s/\n",ltxt);
    w2 = gtk_label_new (ltxt);
    gtk_misc_set_alignment (GTK_MISC (w2), 0.f, 0.5f); // left

    c1 = GUI_opts_get1 (&opts, "lcr");  // 1=default
    if(c1 != 'r') {
      // pack label into box; on the left side of the inputfield.
      gtk_container_add (GTK_CONTAINER (w_par), w2);
      // gtk_box_pack_start (GTK_BOX (w_par), w2, TRUE,TRUE, 0);
      gtk_widget_show (w2);

      // pack entry w1 into box
      GUI_w_pack2 (pTyp, w_par, w1, opts);  // pack1 no fixed entry-length!

    } else {
      // pack entry w1 into box
      GUI_w_pack2 (pTyp, w_par, w1, opts); // pack1 no fixed entry-length!

      // label right of inputfield
      gtk_container_add (GTK_CONTAINER (w_par), w2);
      gtk_widget_show (w2);
    }


  //----------------------------------------------------------------
  L_done:
  // fill obj
  gEnt->gio_typ  = TYP_GUI_Entry;
  gEnt->entry    = w1;
  gEnt->data     = data;
  gEnt->uFunc    = funcnam;

  // return memObj
  return (((Obj_Unknown*)gEnt)->mem_obj);

}


//================================================================
  int GUI_entry_cb (void *parent, void *event, MemObj mo) {
//================================================================
/// \code
/// INTERNAL
/// returns 0=event   TYP_EventEnter|TYP_EventRelease
///         1=userdata
///         2=key
///         3=modfier
/// \endcode

  int          ikey, imod, iEv;
  void         *pTab[4];
  Obj_Entry    *go;
  int          (*func1)();


  // printf("GUI_entry_cb %d %d\n",mo.mbID,mo.ioff);
  // printf(" ev=%d\n",((GdkEvent*)event)->type);

  go = GUI_obj_pos (&mo);
  if(!go) return 0;
  if(!go->uFunc) return 0;


  if(((GdkEvent*)event)->type == GDK_FOCUS_CHANGE) {
    // GdkEventFocus 
    ikey = 0;
    // imod = ((GdkEventFocus*)event)->in;                always 1

    // test "has-focus" property
    // g_object_get (parent, "has-focus", &imod, NULL);   always 1
    imod = 0;

    iEv = TYP_EventEnter;
  }


  if(((GdkEvent*)event)->type == GDK_KEY_PRESS) {
    iEv  = TYP_EventPress;
    ikey = ((GdkEventKey*)event)->keyval;
    imod = ((GdkEventKey*)event)->state;
  }

  if(((GdkEvent*)event)->type == GDK_KEY_RELEASE) {
    iEv = TYP_EventRelease;
    ikey = ((GdkEventKey*)event)->keyval;
    imod = ((GdkEventKey*)event)->state;
  }

  pTab[0] = &iEv;
  pTab[1] = go->data;
  pTab[2] = &ikey;
  pTab[3] = &imod;

  // go->uFunc (go, pTab);
  // return (FALSE);   // TRUE: do no defaultOperations


  func1 = (void*)go->uFunc;
  imod = (*func1) (go, pTab);
    // printf(" imod=%d\n",imod);

  return (imod);   // TRUE: do no defaultOperations

}


//================================================================
  int GUI_entry_sel_del (MemObj *mo) {
//================================================================
/// \code
/// GUI_entry_sel_del      clear selected text
/// if inputfiled has selected text, remove this selection.
/// \endcode

  int         ii, i1, i2;
  int         pTyp;
  Obj_Unknown *go;


  GUI_obj_typ (&pTyp, (void**)&go, mo);
  if(!pTyp) return (-1);


  // clear selection
  //gtk_selection_owner_set(NULL,GDK_SELECTION_PRIMARY,GDK_SELECTION_TYPE_STRING);
  // geht ned ..



  // test if active inputfield has selected text
  ii = gtk_editable_get_selection_bounds ((GtkEditable*)go->widget, &i1, &i2);
    // printf("GUI_selection_clear ii=%d\n",ii);


  if(ii) {
    // gtk_editable_select_region ((GtkEditable*)go->widget, -1, -1);
    gtk_editable_delete_text  ((GtkEditable*)go->widget, i1, i2);
  }

  return 0;

}


// EOF
