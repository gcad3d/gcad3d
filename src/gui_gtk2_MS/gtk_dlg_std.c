/*
 * gtk_dlg_std.c
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
Standard-dialogs; slider, color-selection.

-----------------------------------------------------
Modifications:
2012-02-01 extracted from ut_gtk.c.  RF.



-----------------------------------------------------
*/
#ifdef globTag
void GUI_DIALOG(){}
#endif
/*!
\file ../gui_gtk2/gtk_dlg_std.c
\ingroup grp_gui
\code


=====================================================
List_functions_start:

GUI_MsgBox             display text, OK-Button, also if GTK is not yet up

GUI_DialogYN           dialogWindow w. OK-Button, Cancel-Button, callback-func
// GUI_Dialog_cb_e2b      dialogWindow w. entry, 2 Buttons, callback-func

GUI_DialogEntry        dialogWindow 1-n buttons, entry (opt.), waiting.
GUI_Dialog_2b          dialogWindow w. 2 Buttons, waiting.
GUI_Dialog_e2b         dialogWindow w. entry, 2 Buttons, waiting.

GUI_Color_select       select-color-Dialog. Modal (wait).

GUI_Slider__           Slider = AdjustmentControl
GUI_Slider_set         change slider-value
GUI_Slider_get         query slider-value

GUI_DialogYN_CB        INTERNAL callback of GUI_DialogYN
GUI_DialogEntryCB      INTERNAL callback of GUI_DialogEntry
GUI_Slider_cb1         INTERNAL slider callback

List_functions_end:
=====================================================

needs gtk_entry


\endcode *//*----------------------------------------

gcc `pkg-config --cflags gtk+-3.0` gtk_dialog.c


======================================================================
*/


// ex ../ut/ut_geo.h:
#define IMIN(x,y)  (((x)<(y))?(x):(y))



// #include "../xa/MS_Def0.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>

#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_umem.h"

#include "../gui/gui_types.h"           // TYP_Event..
#include "../gui/gui_base.h"            // TYP_Event..

#include "../gui_gtk2_MS/gtk_base.h"
#include "../gui_gtk2_MS/gtk_entry.h"
#include "../gui_gtk2_MS/gtk_label.h"
#include "../gui_gtk2_MS/gtk_button.h"
#include "../gui_gtk2_MS/gtk_dlg_std.h"


// extern:
extern int       UI_fontsizX, UI_fontsizY;
extern GtkWidget *UI_MainWin;
extern GtkWidget *UI_act_wi;




// local:
static GtkWidget *UI_DialogYNWin=NULL;
static void      (*UI_DialogYNFunc)();

static MemObj UI_DialogEntryWin;





//=====================================================================
  int GUI_DialogYN_CB (void *parent, void *data) {
//=====================================================================
/// INTERNAL callback of GUI_DialogYN

  int          idat;
  void         *pTab;



  printf("GUI_DialogYN_CB %d\n",INT_PTR(data));


  // if ((GTK_TOGGLE_BUTTON (parent)->active) == 0) {
    // printf("skip disactivation\n");
    // return 0;
  // }


   gtk_widget_destroy (UI_DialogYNWin);
   UI_DialogYNWin=NULL;


  // GUI_update__(); // update pending events
  // raus 2009-01-30 - blockiert Fun

  idat = UI_FuncCancel;

  if(INT_PTR(data) == GTK_RESPONSE_OK) {
      idat = UI_FuncOK;
    // case GTK_RESPONSE_CANCEL:
    // case GTK_RESPONSE_DELETE_EVENT:
  }

  pTab = &idat;
  (*UI_DialogYNFunc) (NULL, &pTab);

  return 0;
}


//================================================================
  int GUI_DialogEntryCB (void *parent, void **data) {
//================================================================
/// INTERNAL callback of GUI_DialogEntry


  printf("GUI_DialogEntryCB %d\n",GUI_DATA_EVENT);


  if(GUI_DATA_EVENT == TYP_EventExit) {
    OS_file_sig_cre (2, (void*)-1);             // cancel
    return 0;
  }




  // create signalFile (creator of windows waits for this file ..)
  OS_file_sig_cre (2, GUI_DATA_PTR_I1);

  return 0;

}


//================================================================
  int GUI_Dialog_2b (char *txt, char *tb1, char *tb2) {
//================================================================
/// \code
/// GUI_Dialog_2b          dialogWindow w. 2 Buttons, waiting.
/// Input
///   txt         text label
///   tb1         caption button 1
///   tb2         caption button 2
/// RetCod        nr of button pressed or UI_FuncKill (delete window)
///               0 means first button pressed, 1=second ..
///               -1 = dialog cancelled
/// \endcode

  char   *buttons[3];


  printf("GUI_Dialog_2b |%s|%s|%s|\n",txt,tb1,tb2);


  buttons[0] = tb1;
  buttons[1] = tb2;
  buttons[2] = NULL;

  return GUI_DialogEntry (txt, NULL, 0, buttons, 2);

}
 

//=============================================================================
  int GUI_Dialog_e2b (char *txt, char *entry, int eSiz, char *tb1, char *tb2) {
//=============================================================================
/// \code
/// GUI_Dialog_e2b         dialogWindow w. entry, 2 Buttons, waiting.
///
/// Input
///   txt         label
///   entry       entrytext, in & out
///   eSiz        size of inputfield entry (max. nr of chars)
///   tb1         caption button 1
///   tb2         caption button 2
/// Output:
///   entry       entrytext
/// RetCod        nr of button pressed or UI_FuncKill (delete window)
///               0 means first button pressed, 1=second ..
///               -1 = dialog cancelled
/// \endcode

  char   *buttons[3];   
                        

  printf("GUI_Dialog_e2b |%s|%s|%s|%s|\n",txt,entry,tb1,tb2);


  buttons[0] = tb1;
  buttons[1] = tb2;    
  buttons[2] = NULL;
  
  return GUI_DialogEntry (txt, entry, eSiz, buttons, 2);

}
 

//=============================================================================
  int GUI_DialogEntry (char* label, char *entry, int eSiz,
                        char *buttons[], int border) {
//=============================================================================
/// \code
/// GUI_DialogEntry       text, 1-n buttons; entry optional; waits for return.
///
/// Input
///   label       text
///   entry       userinput - defaultText;  NULL = no userinput (only buttons)
///   eSiz        size of inputfield entry (max. nr of chars)
///   buttons     pointerarray of buttontext, NULL-terminated.
///   border      size around button
/// Output:
///   entry       content of inputfield on exit
///   eSiz        nr of chars in inputfield entry
/// RetCod        nr of button pressed or -1 = dialog cancelled
///               0 means first button pressed, 1=second ..
///
/// Example without entry:
///   char   *buttons[]={"YES","NO","Cancel",NULL}, s1[256];
///   irc = GUI_DialogEntry ("Save model -", NULL, 0, buttons, 2);
///   // YES returns 0, NO returns 1, Cancel returns 2; X returns UI_FuncKill.
///
/// Example with entry:
///  strcpy (s1, "myName");
///  irc = GUI_DialogEntry ("GUI_DialogEntry", s1, 250, buttons, 2);
///
/// \endcode

//    int    irc, sizText;
//    char   sLabel[200], *buttons[4], sbt[3][64];
//    sprintf(sbt[0], "Cancel");
//    sprintf(sbt[1], "NO");
//    sprintf(sbt[2], "YES");
//    buttons[0] = sbt[0];
//    buttons[1] = sbt[1];
//    buttons[2] = sbt[2];
//    buttons[3] = NULL;
//    irc = GUI_DialogEntry ("Save model -", NULL, NULL, buttons);


  MemObj    box0, box1, w_entry, mo1;
  void      *wo;

  int        i1, irc, *ia, bNr;
  // GtkWidget  *box1, *w_entry;
  char       fn[240], *p1;


  // printf("GUI_DialogEntry |%s|\n",label);
  // if(entry)printf(" DW entry |%s| %d\n",entry,eSiz);
  // i1=0;while(buttons[i1]){printf(" DW butt[%d]=|%s|\n",i1,buttons[i1]);++i1;}


  // not necessary if modal ..
  // // if(UI_DialogEntryWin) {
  // if(UI_DialogEntryWin.mbID > 0) {
    // // gdk_window_raise (GDK_WINDOW(UI_DialogEntryWin));
      // printf(" GUI_DialogEntry already exists ..\n");
    // return 0;
  // }


  UI_DialogEntryWin = GUI_Win__ ("", GUI_DialogEntryCB, "");
  // box0 = GUI_box__ (&UI_DialogEntryWin, 1, 0, 0);
  box0 = GUI_box_v (&UI_DialogEntryWin, "");
  GUI_spc__ (&box0, 1, 4);
  GUI_label__ (&box0, label, "");


  // create entry .....................
  if(entry) {
    // i1 = strlen(entry) + 10;
    w_entry = GUI_entry__ (&box0, NULL, entry, NULL, NULL, "e");
      // printf("GUI_DialogEntry |%s| %d\n",entry,eSiz);
  }

  // create buttons .....................
  GUI_sep__ (&box0, 0, 4);
  // box1 = GUI_box__ (&box0, 0,  0, 0);
  box1 = GUI_box_h (&box0, "");


  // get i1 = nr of buttons;
  bNr = 0;
  while(buttons[bNr]) ++bNr;
    printf(" bNr=%d\n",bNr);


  // get static list of integers for user-data
  ia = UME_alloc_tmp (sizeof(int) * bNr);

  for(i1=0; i1<bNr; ++i1) {
    if(border > 0) GUI_spc__ (&box1, 0, border);
      printf(" butt[%d]=|%s|\n",i1,buttons[i1]);
    ia[i1] = i1;   // the id must be static !
    GUI_button__ (&box1, buttons[i1], GUI_DialogEntryCB, PTR_INT(ia[i1]), "e");
  }

  if(border > 0) GUI_spc__ (&box1, 0, border);


  GUI_Win_go (&UI_DialogEntryWin);
  GUI_Win_up (NULL, &UI_DialogEntryWin, 1);
  // GUI_update__ ();


  // wait ..
  OS_file_sig_wait (2, (void*)&irc);
    printf(" irc=%d\n",irc);


  // get entry back
  // if(GTK_IS_WINDOW(UI_DialogEntryWin)) {        // delete destroys win0 
  if(irc >= 0) {
    if(entry) {
      p1 = GUI_entry_get (&w_entry);
      i1 = eSiz - 1;
      eSiz = IMIN (strlen(p1), i1);
      strncpy(entry, p1, eSiz);
      entry[eSiz] = '\0';
        printf(" entry: |%s| %d\n",entry,eSiz);
    }

    GUI_Win_kill (&UI_DialogEntryWin);
  }     


  return irc;

}


/*
//=====================================================================
  void GUI_Dialog_cb_e2b (char *txt, char *entry, int eSiz,
                          char *tb1, char *tb2,
                          void *funcnam ) {
//=====================================================================
/// GUI_Dialog_cb_e2b      dialogWindow w. entry, 2 Buttons, callback-func
///
/// Input
///   txt         label 
///   entry       entrytext, in & out
///   tb1         caption button 1
///   tb2         caption button 2
/// Output:
///   entry       entrytext
///
/// funcnam prototyp:
/// int funcnam (MemObj *parent, void **data);
///   GUI_DATA_EVENT =*(int*)data[0]=UI_FuncOK|UI_FuncCancel
///
/// \endcode
*/



//=====================================================================
  void GUI_DialogYN (char* text, void *funcnam ) {
//=====================================================================
/// \code
/// GUI_DialogYN   dialogWindow with text, OK-Button and Cancel-Button.
/// <funcnam> is called with int-value UI_FuncOK or UI_FuncCancel.
///
/// Input:
///   text      multiple lines with \n
///   funcnam   callbackfunktion for button-press
///
/// funcnam prototyp:
/// int funcnam (MemObj *parent, void **data);
///   GUI_DATA_EVENT =*(int*)data[0]=UI_FuncOK|UI_FuncCancel
///
/// Example:
///   GUI_DialogYN ("OK to save file xy", f_CB_save);
///
///
/// //=====================================================================
///   int f_CB_save (MemObj *mo, void **data) {
/// //=====================================================================
///
///   int idat;
///
///   idat = GUI_DATA_EVENT;
///   printf("f_CB_save %d\n",idat);
///
///   switch(idat) {
///     case UI_FuncOK:
///       // save ...
///       break;
///     case UI_FuncCancel:
///       // skip saving ...
///       break;
///   }
///
///   return 0;
///
/// }
/// \endcode


  int         iRes;     // response
  GtkWidget   *wdlg;


  // printf("UI_DialogYN |%s|\n",text);


  if(UI_DialogYNWin) {     // cannot use GUI_is_alive
    // gdk_window_raise (UI_MainWin);
    return;
  }



  wdlg = gtk_message_dialog_new (
           GTK_WINDOW (UI_MainWin),  // keep on top of this else NULL
           GTK_DIALOG_DESTROY_WITH_PARENT,
           GTK_MESSAGE_QUESTION,
           GTK_BUTTONS_OK_CANCEL,
           "%s",text);


  gtk_window_set_transient_for (GTK_WINDOW(UI_MainWin), GTK_WINDOW(wdlg));
  gtk_window_set_modal (GTK_WINDOW (wdlg), TRUE);


  // does not wait (not modal):
  gtk_widget_show (wdlg);
  g_signal_connect_swapped (wdlg, "response",
                          G_CALLBACK (GUI_DialogYN_CB),
                          wdlg);

  UI_DialogYNWin  = wdlg;
  UI_DialogYNFunc = funcnam;

  return;

}


//================================================================
  int GUI_MsgBox (char* text) {
//================================================================
/// \code
/// display text, OK-Button.
/// Works also if GTK is not yet up.
/// \endcode

  int         iRes;     // response
  GtkWidget   *wdlg;
  char        s2[512];


  if(UI_MainWin) goto L_gtk;
  if(OS_get_dialog() < 0) {
    printf("GUI_MsgBox |%s|\n",text);
    return -1;
  }

  sprintf(s2,"zenity --error --text \'%s\'",text);
  system (s2);

  return 0;





  //----------------------------------------------------------------
  // Version gtk-direct:
  L_gtk:


  wdlg = gtk_message_dialog_new (
           GTK_WINDOW (UI_MainWin),  // keep on top of this else NULL
           GTK_DIALOG_DESTROY_WITH_PARENT,
           GTK_MESSAGE_INFO,
           GTK_BUTTONS_OK,
           "%s",text);

  // which icon:
  //   GTK_MESSAGE_INFO,GTK_MESSAGE_QUESTION
  //   GTK_MESSAGE_ERROR,GTK_MESSAGE_WARNING

  // Buttons:
  //   GTK_BUTTONS_OK, GTK_BUTTONS_CLOSE, GTK_BUTTONS_CANCEL
  //   GTK_BUTTONS_YES_NO, GTK_BUTTONS_OK_CANCEL

  // wait, modal:
  // start waiting; does not return until user clicks button.
  iRes = gtk_dialog_run (GTK_DIALOG (wdlg));    // wait (modal) !
    // printf(" iRes=%d\n",iRes);
  // GTK_RESPONSE_OK GTK_RESPONSE_YES GTK_RESPONSE_CANCEL ..

  // you can kill it with gtk_dialog_response ()

  gtk_widget_destroy (wdlg);



  //----------------------------------------------------------------
  // update mousebuttons; can eat the release mousebutton !
  GUI_gl_ev_butt_upd ();


  return 0;

}


//================================================================
  int GUI_Color_select (int *ir, int *ig, int *ib) {
//================================================================
/// \code
/// GUI_Color_select          select-color-Dialog. Modal (wait).
/// Returns 3 colors as 16-bit-integers.
/// Returncode   0=OK; -1=cancel.
/// \endcode

  int         iRes;     // response
  GtkWidget   *wdlg;
  GtkWidget   *wSel;
  GdkColor    rgb;      // 3 x short


  // printf("GUI_ColSel\n");


  wdlg = gtk_color_selection_dialog_new ("select color ..");

  // stay on top (modal)
  gtk_window_set_transient_for (GTK_WINDOW(wdlg), GTK_WINDOW(UI_MainWin));

  // start waiting; does not return until user clicks button.
  iRes = gtk_dialog_run (GTK_DIALOG (wdlg));    // wait (modal) !
    printf(" iRes=%d\n",iRes);
  // -6=_CANCEL, -5=_OK -4=

  if((iRes == GTK_RESPONSE_CANCEL)  || 
     (iRes == GTK_RESPONSE_DELETE_EVENT)) {
    gtk_widget_destroy (wdlg);
    return -1;
  }

  wSel = gtk_color_selection_dialog_get_color_selection (
             GTK_COLOR_SELECTION_DIALOG(wdlg));
  gtk_color_selection_get_current_color (GTK_COLOR_SELECTION (wSel), &rgb);
  *ir = rgb.red;
  *ig = rgb.green;
  *ib = rgb.blue;
    // printf(" sel.Col = %d %d %d\n",*ir,*ig,*ib);

  gtk_widget_destroy (wdlg);


/*
    col1=GTK_COLOR_SELECTION(GTK_COLOR_SELECTION_DIALOG(colSel)->colorsel);

    gtk_color_selection_get_color (col1, da);

    // printf(" %f %f %f\n",da[0],da[1],da[2],da[3]);
    cSel.cr = da[0] * 255;
    cSel.cg = da[1] * 255;
    cSel.cb = da[2] * 255;
    printf(" %d %d %d\n",cSel.cr,cSel.cg,cSel.cb);
    cb_func (&cSel);
    gtk_widget_destroy (colSel);
  }
  colSel = gtk_color_selection_dialog_new (" select Modelcolor");
  g_signal_connect(
    GTK_COLOR_SELECTION_DIALOG(colSel->ok_button),
    "clicked", G_CALLBACK(GUI_ColSel), (void*)1);
  g_signal_connect(
    GTK_COLOR_SELECTION_DIALOG(colSel->cancel_button),
    "clicked", G_CALLBACK(GUI_ColSel), (void*)2);
  gtk_widget_show_all(colSel);
  cb_func = mode; // die callback-func
*/

  return 0;

}


//===================================================================
  double GUI_Slider_get (MemObj *mo) {
//===================================================================
/// query slider-value

  double  d1;
  void   *go, *wo;


  printf("GUI_Slider_get \n");
    UME_obj_dump (mo);

  go = GUI_obj_pos (mo);
  if(!go) return 0.;
    GUI_obj_dump_o (go);

  wo = ((Obj_Slider*)go)->slider;

  d1 = gtk_adjustment_get_value  (GTK_ADJUSTMENT(wo));

  // printf("GUI_Slider_get %lf\n",d1);

  return d1;

}


//===================================================================
  void GUI_Slider_set (MemObj *mo, double newVal) {
//===================================================================
/// change slider-value

  void   *go, *wo;

  go = GUI_obj_pos (mo);
  if(!go) return;
  wo = ((Obj_Unknown*)go)->widget;

  gtk_adjustment_set_value  (GTK_ADJUSTMENT(wo), newVal);


/*
  ((GtkAdjustment*)wsl1)->value = newVal;
  // update Slider
  gtk_adjustment_value_changed ((GtkAdjustment*)wsl1);
*/

  return;

}


//================================================================
  int GUI_Slider_cb2 (void *parent, void *ev, void *data) {
//================================================================
/// GUI_Slider_cb2          INTERNAL slider callback

  int          typ, ikey, iTyp;
  void         *pTab[2];
  // void         (*uFunc)();
  Obj_gui2     *go;
  MemObj       mo;

  GUI_MO_PTR (&mo, &data);   // copy ptr -> MemObj

  go = GUI_obj_pos (&mo);
  if(!go) return 0;
  if(!go->uFunc) return 0;

  typ  = ((GdkEventAny*)ev)->type;

  // printf("GUI_Slider_cb2 \n");

  if(typ == GDK_BUTTON_PRESS) {
    iTyp = TYP_EventPress;
    // ikey = ((GdkEventButton*)ev)->button;

  } else if(typ == GDK_BUTTON_RELEASE) {
    iTyp = TYP_EventRelease;
    // ikey = ((GdkEventButton*)ev)->button;

  } else return 0;


  pTab[0] = &iTyp;
  pTab[1] = NULL;

  // uFunc = data;
  // uFunc (parent, pTab);

  go->uFunc (go, pTab);

  return 0;  // TRUE=1=stop further handling of event ..
}


//===================================================================
  MemObj GUI_Slider__ (MemObj *o_par, int typ, void *funcnam,
                       double valMin, double valMax, double valStart,
                       char *opts) {
//===================================================================
/// \code
/// GUI_Slider__           Slider = AdjustmentControl
///
/// Input:
///   parent     parentBox
///   typ        0=horizontal, 1=vertical.
///   funcnam    callBack-function for moving the slider; NULL = no callback.
///   opts       options; (HorSiz,VertSiz)
///              HorSiz,VertSiz:  size in characters; default is automatic size.
///                              'e' = expand widget; default is fixed size.
///              Examples: "" or "10" or "10e,e"
///                "10e,e" horiz. size 10 chars, hor. and vert. expandable.
///
/// funcnam prototyp:
/// int funcnam (MemObj *mo, void **data);
///   // data=table of 2 pointers;
///   GUI_DATA_EVENT  = *(int*)data[0]    = TYP_EventMove
///                                         TYP_EventPress (left mousebutton)
///                                         TYP_EventRelease 
///   GUI_DATA_D1     = *(double*)data[1] = slider-value (only TYP_EventMove)
///   GUI_OBJ_TYP(mo) = TYP_GUI_Slider
///
/// Example without callback:
/// wsl1 = GUI_Slider__ (&box1, 0, NULL, 0., 1., 0.5, "8");
/// // query slider-value
/// printf(" sliVal=%f\n",GUI_Slider_get(&wsl1));
/// // change slider-value
/// GUI_Slider_set (&wsl1, 0.75);
///
/// Example with callback:
/// wsl1 = GUI_Slider__ (&box1, 0, sl_CB, 0., 1., 0.5, "8");
/// int sl_CB (MemObj *mo, void **data) {
///   printf("sl_CB %lf\n",GUI_DATA_D1);  // slider-value as double
/// }
/// \endcode


  int           pTyp;
  // GtkAdjustment *adj1;
  GtkObject     *adj1;
  GtkWidget     *wsc1;
  void          *w_par;
  double        d1;
  Obj_Slider    *go;


  // get gtk-widget for parentBox from MemObj
  w_par = GUI_obj_parent__ (&pTyp, o_par);
  if(!w_par) return (UME_obj_invalid_set (-3));


  // get address & memspc
  GUI_obj_spc ((void**)&go, sizeof(Obj_Slider));
  if(!go) return (UME_obj_invalid_set (-1));



  // Adjustment (StartVal minVal maxVal incr pageIncr pageSize)
  d1 = (valMax - valMin ) / 100.;
  adj1 = gtk_adjustment_new((gdouble)valStart,
                            (gdouble)valMin,
                            (gdouble)valMax,
                            (gdouble)d1,
                            (gdouble)0.,
                            (gdouble)0.);

  // callback for moving the control
  if(funcnam) {
    g_signal_connect (adj1, "value_changed",
                      G_CALLBACK(GUI_Slider_cb1),
                      PTR_MEMOBJ(go->mem_obj));
  }


  wsc1 = gtk_hscale_new (GTK_ADJUSTMENT(adj1));

  if(funcnam) { // 2013-09-09
    gtk_widget_add_events (GTK_WIDGET(wsc1),
                           GDK_BUTTON_PRESS_MASK |
                           GDK_BUTTON_RELEASE_MASK);

    g_signal_connect (G_OBJECT(wsc1), "button_press_event",
                      G_CALLBACK(GUI_Slider_cb2),
                      PTR_MEMOBJ(go->mem_obj));

    g_signal_connect (G_OBJECT(wsc1), "button_release_event",
                      G_CALLBACK(GUI_Slider_cb2),
                      PTR_MEMOBJ(go->mem_obj));
  }

  UI_act_wi = wsc1;              // GUI_Tip; 2013-09-09


  // do not display actual value
  gtk_scale_set_draw_value (GTK_SCALE(wsc1), FALSE);

  
  // // GUI_packwn (w_par, wsc1, siz, 0);
  // if(siz == 1) GUI_packwn (w_par, wsc1, 0, 1);
  // else         GUI_packwn (w_par, wsc1, siz, 0);
  GUI_w_pack1 (pTyp, w_par, wsc1, opts);


  // fill obj
  go->gio_typ  = TYP_GUI_Slider;
  go->slider   = adj1; //wsc1;
  go->uFunc    = funcnam;

  return (((Obj_Unknown*)go)->mem_obj);

}


//================================================================
  int GUI_Slider_cb1 (void *parent, MemObj mo) {
//================================================================
/// GUI_Slider_cb           INTERNAL slider callback

  int          iEv=TYP_EventMove;
  double       d1;
  void         *pTab[2];
  Obj_gui2     *go;

  go = GUI_obj_pos (&mo);
  if(!go) return 0;
  if(!go->uFunc) return 0;

  d1 = gtk_adjustment_get_value  (GTK_ADJUSTMENT(parent));

  pTab[0] = &iEv;
  pTab[1] = &d1;

  go->uFunc (go, pTab);

  return 0;
}

// EOF
