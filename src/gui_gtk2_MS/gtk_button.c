/*
 * gtk_button.c
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


-----------------------------------------------------
Buttons, Checkbox.



-----------------------------------------------------
Modifications:
2012-02-01 extracted from ut_gtk.c.  RF.

-----------------------------------------------------
*/
#ifdef globTag
void GUI_BUTTON(){}
#endif
/*!
\file ../gui_gtk2/gtk_button.c
\ingroup grp_gui
\code

=====================================================
List_functions_start:

GUI_button__           create Button
GUI_button_mod         modify buttontext (caption)
GUI_button_styl        modify buttonbackgroundColor

GUI_button_img         create Button with image

GUI_ckbutt__           create Checkbox
GUI_ckbutt_get         returns state of checkbox; 0=not sel, 1=selected.
GUI_ckbutt_set         activate/disactivate Checkbox, Radiobutton
GUI_ckbutt_set_noCB    set Checkbox, but do NOT call userCB

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

#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_umem.h"                     // Memspc

#include "../gui/gui_types.h"                   // TYP_Event..
#include "../gui/gui_base.h"                  // GUI_obj_*

#include "../gui_gtk2/gtk_base.h"
#include "../gui_gtk2/gtk_button.h"



// gtk_core.c:
extern GtkWidget *UI_act_wi;
extern int       UI_fontsizY;
extern Memspc    UI_tmpSpc;


static int UI_but_noCB = 0;




//=====================================================================
  MemObj GUI_button_img (MemObj *o_par, char* filnam,
                         void *funcnam, void *data, char *opts) {
//=====================================================================
/// \code
/// create button with pixmap
/// Input:
///   o_par      parentBox
///   filnam     image
///   funcnam    callbackfunktion for the press-button-event
///   data       get this data from the callbackfunktion; NULL for none
///   opts       options; (HorSiz,VertSiz)
///              HorSiz,VertSiz:  size in characters; default is automatic size.
///                              'e' = expand widget; default is fixed size.
///                               negative values: size in pixels.
///              Examples: "" or "10" or "10e,e"
///                "10e,e" horiz. size 10 chars, hor. and vert. expandable.
///
///  RetCod:     NULL = Error (filnam not exist)
///
/// funcnam prototyp: 
///   see GUI_button__ ()
/// \endcode
    

  int          pTyp;
  GtkWidget    *w_bt;
  GtkWidget    *img1;
  void         *w_par;
  Obj_Button   *go;

  // printf("GUI_button_img |%s|%s|\n",filnam,opts);

  if(OS_checkFilExist(filnam, 0) == 0) {
    // TX_Error (" Imagefile %s does not exist",fNam);
    MSG_err_1 ("NOEX_fil", "%s", filnam);
    return (UME_obj_invalid_set (-2));
  }


  // get gtk-widget for parentBox from MemObj
  w_par = GUI_obj_parent__ (&pTyp, o_par);
  // if(!w_par) return (UME_obj_invalid_set(-2));
  if(!w_par) return (UME_obj_invalid_set (-3));


  // get address & memspc
  GUI_obj_spc ((void**)&go, sizeof(Obj_Button));
  if(!go) return (UME_obj_invalid_set (-1));


  UI_act_wi = gtk_button_new ();

  if(funcnam) {
    // data (last par.) is not a pointer, its the memObj !
    g_signal_connect (UI_act_wi, "clicked", // G_CALLBACK(funcnam), data);
                      G_CALLBACK(GUI_button_press),
                      PTR_MEMOBJ(go->mem_obj));
  }

  img1 = gtk_image_new_from_file (filnam);
  gtk_button_set_image (GTK_BUTTON(UI_act_wi), img1);

  // GUI_packwn (w_par, UI_act_wi, hsiz, vsiz);
  GUI_w_pack2 (pTyp, w_par, UI_act_wi, opts);


  go->gio_typ  = TYP_GUI_Button;
  go->button   = UI_act_wi;
  go->uFunc    = funcnam;
  go->data     = data;

  return (((Obj_Unknown*)go)->mem_obj);

}


//================================================================
  int GUI_button_mod (MemObj *mo, char* ltext) {
//================================================================
/// GUI_Butt_Label         modify buttontext (caption)

  Obj_Unknown *go;


  // printf("GUI_button_mod |%s|\n",ltext);

  go = GUI_obj_pos (mo);
  if(!go) return 0;

  gtk_button_set_label (GTK_BUTTON(go->widget), ltext);

  return 0;

}


//================================================================
  int GUI_button_styl (MemObj *mo, int iCol) {
//================================================================
/// \code
/// GUI_button_styl        modify buttonbackgroundColor
/// 0=default; 1=red; 2=blue
/// \endcode

// see GUI_label_styl

// GdkColor  colB = {0x0000, 0x0010, 0x0010, 0xc000};   // blue

  extern GtkStyle *UI_stylTab[];      // 0=default; 1=red; 2=blue

  Obj_Unknown *go;


  // printf("GUI_button_styl %d\n",iCol);

  go = GUI_obj_pos (mo);
  if(!go) return 0;

  gtk_widget_set_style (go->widget, UI_stylTab[iCol]);
  // gtk_widget_modify_fg (go->widget, 0, &colB);
  // gtk_widget_modify_base (go->widget, 0, &colB);

  return 0;

}


//=====================================================================
  MemObj GUI_button__ (MemObj *o_par,  char* ltext,
                       void *funcnam, void *data, char *opts) {
//=====================================================================
/// \code
/// Button.
/// Input:
///   o_par      parentBox
///   ltext      caption (button-text)
///   funcnam    callbackfunktion for the press-button-event
///   data       get this (static) data from the callbackfunktion; NULL for none
///   opts       options;  (HorSiz,VertSiz)
///              HorSiz,VertSiz:  size in characters; default is automatic size.
///                              'e' = expand widget; default is fixed size.
///              Examples: "" or "10" or "10e,e"
///                "10e,e" horiz. size 10 chars, hor. and vert. expandable.
/// Output:
///   gBut       must exist as long as button is alive.
/// 
/// funcnam prototyp: 
///   int funcnam (MemObj *mo, void **data);
///   // data=table of 2 pointers;
///   GUI_DATA_EVENT =*(int*)data[0]=TYP_EventPress
///   GUI_DATA_I1=*(int*)data[1] or GUI_DATA_S1=(char*)data[1]  user-data
///   GUI_OBJ_TYP(mo)  = TYP_GUI_Button
///
/// Example with string-data:
/// GUI_button__ (box0, "TestButton", UI_func1, "b1-clicked", "");
///
/// int UI_func1 (MemObj *mo, void **data) {
///     printf(" typ=%d\n",GUI_OBJ_TYP(mo));
///     printf(" button |%s|\n", GUI_DATA_S1);
///   ..
///
/// Example with integer-data:
/// static int icb = UI_FuncUCB1;
/// GUI_button__ (box0, "TestButton", UI_func1, (void*)&icb, "");
///
/// int UI_func1 (MemObj *mo, void **data) {
///     if(GUI_DATA_EVENT == TYP_EventPress) {
///       if(GUI_DATA_I1 == UI_FuncUCB1) {
///         ..
///
/// Disactivate:
///   GUI_set_enable (&wButt, 0); //1=activ, 0=inaktiv
///
/// Change text/caption/label:
///   GUI_button_mod (&wButt, " newText ");     // gtk_button_set_label
///
/// \endcode



  int        is, pTyp;
  void       *w_par, *wb;
  Obj_Button *go;


  // printf("GUI_button__ |%s|%s|\n",ltext,opts);
  // printf("  data=%p %d\n",data, INT_PTR(data));


  // get gtk-widget for parentBox from MemObj
  w_par = GUI_obj_parent__ (&pTyp, o_par);
  if(!w_par) return (UME_obj_invalid_set (-3));


  // get address & memspc
  GUI_obj_spc ((void**)&go, sizeof(Obj_Button));
  if(!go) return (UME_obj_invalid_set (-1));


  UI_act_wi = gtk_button_new_with_label (ltext);
  // gtk_widget_set_name (UI_act_wi, "Style1");


  if(funcnam) {
    g_signal_connect (UI_act_wi, "clicked",
                      G_CALLBACK(GUI_button_press),
                      PTR_MEMOBJ(go->mem_obj));
  }


  // GUI_w_pack1 (pTyp, w_par, UI_act_wi, opts); // makes always expandable !
  GUI_w_pack2 (pTyp, w_par, UI_act_wi, opts);


  go->gio_typ  = TYP_GUI_Button;
  go->button   = UI_act_wi;
  go->uFunc    = funcnam;
  go->data     = data;


  return (((Obj_Unknown*)go)->mem_obj);

}


//================================================================
  int GUI_button_press (void *parent, MemObj mo) {
//================================================================
// INTERNAL
// Button and Menu: nur activate


  int        pTyp, iTyp=TYP_EventPress;
  void       *pTab[2];
  // MemObj     (*uFunc)(MemObj*, void**);
  // Obj_Button *go;
  Obj_gui2   *go;


  // printf("GUI_button_press %d %d\n",mo.mbID,mo.ioff);
  // UMB_dump__ ();
  // GUI_obj_dump__ ();
  // GUI_obj_dump_mo (&mo);


  // skip unwanted reaction
  if(UI_but_noCB) return (0);


  // go = GUI_obj_pos (&mo);     
  // if(!go) return 0;

  GUI_obj_typ (&pTyp, (void**)&go, &mo);
  if(!pTyp) return (-1);
    // printf(" pTyp = %d\n",pTyp);


  if(pTyp == TYP_GUI_CheckBox) {
    if(!GTK_TOGGLE_BUTTON (parent)->active) {
      iTyp = TYP_EventRelease;
        // printf(" not active ..\n");
    }
  }

    // GUI_obj_dump_o (go);
    // printf("  data=%p\n",go->data);


  pTab[0] = &iTyp;
  pTab[1] = go->data;

  go->uFunc (&mo, pTab);
  // uFunc = go->uFunc;
  // uFunc (&mo, pTab);

  return (0);

}


//================================================================
  int GUI_ckbutt_get (MemObj *mo) {
//================================================================
/// returns state of checkbox; 0=not sel, 1=selected.

  Obj_Unknown *go;

  // printf("GUI_ckbutt_get\n");

  go = GUI_obj_pos (mo);
  if(!go) return -1;

  // return GTK_TOGGLE_BUTTON (cb)->active;
  return gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (go->widget));

}


//================================================================
  int GUI_ckbutt_set (MemObj *mo, int mode) {
//================================================================
/// \code
/// activate/disactivate Checkbox, Radiobutton.
/// Input:
///   mode    1 = TRUE  = ON
///           0 = FALSE = OFF
/// \endcode

  Obj_Unknown *go;

  // printf("GUI_ckbutt_set %d\n",mode);

  go = GUI_obj_pos (mo);
  if(!go) return -1;

  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (go->widget), mode);

  return 0;

}


//================================================================
  int GUI_ckbutt_set_noCB (MemObj *mo, int mode) {
//================================================================
/// \code
/// activate/disactivate Checkbox, Radiobutton but do not call userCB
/// Input:
///   mode    1 = TRUE  = ON
///           0 = FALSE = OFF
/// \endcode
  
      
  // printf("GUI_ckbutt_set_noCB %d\n",mode);
    
  UI_but_noCB = 1;

  GUI_ckbutt_set (mo, mode);

  UI_but_noCB = 0;

  return 0;

}


//=====================================================================
  MemObj GUI_ckbutt__ (MemObj *o_par, char* ltxt, int state,
                       void *funcnam, void *data, char *opts) {
//=====================================================================
/// \code
/// Checkbox.
///   o_par      parentBox
///   ltext      caption (button-text)
///   state:     0=FALSE: not checked; 1=TRUE: checked.
///   funcnam    callbackfunktion for the select/deselect-event
///   data       get this (static) data from the callbackfunktion; NULL for none
///   opts       options; (HorSiz,VertSiz)
///              HorSiz,VertSiz:  size in characters; default is automatic size.
///                              'e' = expand widget; default is fixed size.
///              Examples: "" or "10" or "10e,e"
///                "10e,e" horiz. size 10 chars, hor. and vert. expandable.
///
/// funcnam prototyp:
///   int funcnam (MemObj *mo, void **data); 
///   // data=table of 2 pointers;
///   GUI_DATA_EVENT =*(int*)data[0]=TYP_EventPress|TYP_EventRelease
///   GUI_DATA_I1=*(int*)data[1] or GUI_DATA_S1=(char*)data[1]  user-data
///   GUI_OBJ_TYP(mo)  = TYP_GUI_CheckBox
///
/// ---------------------------
/// Example without callback:
///
///   int istate;
///   MemObj  cb1;
///   // create checkbox
///   cb1 = GUI_ckbutt__ (&box0, "ckb 1", TRUE, NULL, NULL, "");
///   // query state
///   istate = GUI_ckbutt_get (&cb1)        // 0=not sel, 1=selected.
///
///
/// ---------------------------
/// Example with callback:
///
///   GUI_ckbutt__ (&box0, "ckb 1", 0, f_ckButt_CB, NULL, "");
///
/// int f_ckButt_CB, (MemObj *parent, void **data) {
///
///   if(GUI_ckbutt_get(parent)) goto L_activate;
///   printf("f_ckbutt disactivate\n");
///   ..
///   L_activate:
///   ..
/// }
///
/// ---------------------------
/// Optional:
///
/// activate with:
///   GUI_ckbutt_set (&cb1, TRUE);
/// \endcode



  int        i1, pTyp;
  void       *w_par, *wb;
  Obj_Button *go;


  // printf("GUI_ckbutt__ |%s| %d |%s|\n",ltxt,state,opts);


  // get gtk-widget for parentBox from MemObj
  // w_par = GUI_obj_parentBox (o_par);
  w_par = GUI_obj_parent__ (&pTyp, o_par);
  if(!w_par) return (UME_obj_invalid_set (-3));


  // get address & memspc
  GUI_obj_spc ((void**)&go, sizeof(Obj_Button));
  if(!go) return (UME_obj_invalid_set (-1));


  UI_act_wi = gtk_check_button_new_with_label (ltxt);

  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (UI_act_wi), state);


  if (funcnam) {
    g_signal_connect (UI_act_wi, "clicked",
                      G_CALLBACK(GUI_button_press),
                      PTR_MEMOBJ(go->mem_obj));
  }


  // GUI_packwn (w_par, UI_act_wi, 0,0);
  GUI_w_pack1 (pTyp, w_par, UI_act_wi, opts);


  go->gio_typ  = TYP_GUI_CheckBox;
  go->button   = UI_act_wi;
  go->uFunc    = funcnam;
  go->data     = data;


  return (((Obj_Unknown*)go)->mem_obj);

}


// EOF
