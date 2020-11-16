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

-----------------------------------------------------
*//*!
\file ../gui_gtk3/gtk_msgWin.c
\brief messagewindow
\code

=====================================================
List_functions_start:

GUI_msgwin__        Create messagewindow
GUI_msgwin_prt      wr string -> messagewindow
GUI_msgwin_prf      wr file -> messagewindow
GUI_msgwin_rmLast   remove last line

List_functions_end:
=====================================================

max linelength = 255

\endcode *//*----------------------------------------


cc -c `pkg-config --cflags gtk+-3.0` -DGTK3 gtk_msgWin.c
*/


// #include "../xa/MS_Def0.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <gtk/gtk.h>

#include "../ut/ut_umem.h"                     // Memspc

#include "../gui/gui_types.h"                   // TYP_Event..
#include "../gui/gui_base.h"                  // GUI_obj_*
#include "../gui_gtk3/gtk_base.h"
#include "../gui_gtk3/gtk_msgWin.h"

// ex gtk_core:
extern int       UI_fontsizX, UI_fontsizY;




//===========================================================================
// LOCALS:






//================================================================
  MemObj GUI_msgwin__ (MemObj *o_par, char *opts) {
//================================================================
/// \code
/// create textwindow
///   o_par      parentBox
///   opts       options; (HorSiz,VertSiz)
///              HorSiz,VertSiz:  size in characters; default is automatic size.
///                              'e' = expand widget; default is fixed size.
///                               negative values: size in pixels.
///              Examples: "" or "10" or "10e,e"
///                "10e,e" horiz. size 10 chars, hor. and vert. expandable.
/// \endcode

// Create ein Editfenster


  int           pTyp, i1;
  void          *w_par;
  Obj_Unknown   *go;
  GtkWidget     *sw;
  GtkTextBuffer *TxBuf;
  GtkWidget     *TxView;

  // printf("GUI_msgwin__ \n");


  // get gtk-widget for parentBox from MemObj
  w_par = GUI_obj_parent__ (&pTyp, o_par);
  if(!w_par) return (UME_obj_invalid_set (-3));


  // get address & memspc
  GUI_obj_spc ((void**)&go, sizeof(Obj_Unknown));
  if(!go) return (UME_obj_invalid_set (-1));


  //----------------------------------------------------------------
  TxView = gtk_text_view_new ();

  TxBuf = gtk_text_view_get_buffer (GTK_TEXT_VIEW (TxView));

  gtk_text_buffer_set_text (TxBuf, "", -1);


  //----------------------------------------------------------------
  sw = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw),
          GTK_POLICY_AUTOMATIC,  //  GTK_POLICY_NEVER
          GTK_POLICY_ALWAYS);

  gtk_container_add (GTK_CONTAINER (sw), TxView);
  gtk_widget_show (TxView);

  // gtk_widget_set_hexpand (sw, TRUE);
  // gtk_widget_set_vexpand (sw, TRUE);

  gtk_text_view_set_editable ((GtkTextView *)TxView, FALSE);
  gtk_text_view_set_cursor_visible ((GtkTextView *)TxView, FALSE);

  // gtk_scrolled_window_set_overlay_scrolling(GTK_SCROLLED_WINDOW(sw),FALSE);
  // gtk_scrolled_window_set_propagate_natural_width(GTK_SCROLLED_WINDOW(sw),FALSE);
  // i1 = gtk_scrolled_window_get_max_content_width (GTK_SCROLLED_WINDOW(sw));
    // printf(" msgwin-max_content_width %d\n",i1); - always gives -1

//> TODO: if text is longer than MessageWindowLineLength:
// Gtk-CRITICAL **: gtk_box_gadget_distribute: assertion 'size >= 0' failed in GtkScrollbar

 
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
      iv = UI_fontsizY * (vsiz + 1);
    }
  }


  if((ih) || (iv)) {
      // printf(" winSiz %d %d\n",ih,iv);
    GUI_siz_set (sw, ih, iv);
  }
*/
  GUI_w_pack2 (pTyp, w_par, sw, opts);

  
  // gtk_container_add (GTK_CONTAINER (w_par), sw);
  // gtk_widget_show (sw);


  go->gio_typ  = TYP_GUI_MsgWin;
  go->widget   = TxView;

    // printf("exit GUI_msgwin__ \n");

  return (((Obj_Unknown*)go)->mem_obj);

}


//================================================================
  int GUI_msgwin_prf (MemObj *mo, char* txbuf) {
//================================================================
/// write file to textwindow

// Datei txbuf ausgeben ...

  FILE        *fp1;
  char        cbuf[256];

  // printf("GUI_msgwin_prf |%s|\n",txbuf);

  if((fp1=fopen(txbuf,"r")) == NULL) {
    TX_Print("GUI_msgwin_prf open Error %s",txbuf);
    return -1;
  }


  while (!feof (fp1)) {
    if (fgets (cbuf, 256, fp1) == NULL) break;
    UTX_CleanCR (cbuf);
    // printf(" nxt: |%s|\n",cbuf);
    GUI_msgwin_prt (mo, cbuf);
  }

  fclose (fp1);
  return 0;

}


//================================================================
  int GUI_msgwin_prt (MemObj *mo, char* txbuf) {
//================================================================
/// \code
/// write textline to textwindow
/// skip msg, if first 60 chars ident.
/// \endcode

// Schreibt an das zuletzt mit GUI_msgwin__ generierte Window


static  char actBuf[256];

  int         i1, ilen, oldPos;
  GtkTextIter iter, iVon, iBis;
  GtkTextMark *mark;
  Obj_Unknown *go;

  GtkWidget     *msgView;
  // GtkTextView   *msgView;
  GtkTextBuffer *msgBuf;


  // test if window is up; 1=starting; 2=initialyze done
  // printf("GUI_msgwin_prt |%s|\n",txbuf);


  go = GUI_obj_pos (mo);
  if(!go) return -1;
  msgView = go->widget;


  // get textBuffer from textView
  msgBuf = gtk_text_view_get_buffer (GTK_TEXT_VIEW(msgView));


  // test if Gtk is already up
  // if(AP_stat.sysStat < 1) {
  if(!msgBuf) {
    // printf("***** %s\n",txbuf);
    return -1;
  }


  // gtk_widget_set_sensitive (TxView, FALSE);


  ilen = strlen(txbuf);
    // printf("ilen=%d\n",ilen);


  // skip msg, if first 60 chars ident.
  if(ilen >= 60) {
    // i1 = IMIN(60,ilen);
    i1 = ((60 < ilen)?60:ilen);
    if(!strncmp(actBuf, txbuf, i1)) return 0;
  } else {
    if(!strcmp(actBuf, txbuf)) return 0;
  }



  // size tot ?
  oldPos = gtk_text_buffer_get_char_count (msgBuf);
  // printf(" oldPos=%d\n",oldPos);


  // Wenn Text > 5000 Char, die ersten 1000 raus
  if(oldPos > 5000) {

    gtk_text_buffer_get_iter_at_offset (msgBuf, &iVon, 0);
    gtk_text_buffer_get_iter_at_offset (msgBuf, &iBis, 1000);
    // i1 = gtk_text_iter_get_line (&iBis); //ask LineNr
    gtk_text_iter_forward_line (&iBis);
    gtk_text_buffer_delete (msgBuf, &iVon, &iBis);
  }

  // get endPos
  gtk_text_buffer_get_end_iter (msgBuf, &iter);

  // moves the "insert" and "selection_bound" marks simultaneously
  gtk_text_buffer_place_cursor (msgBuf, &iter);

  // copy new text -> actBuf
  if(ilen > 240) ilen = 240;
  strncpy(actBuf, txbuf, ilen);
  actBuf[ilen] = '\n';
  // strcat(actBuf, "\n");

  // insert at endPos
  gtk_text_buffer_insert_at_cursor (msgBuf, actBuf, ilen + 1);

  //  change NL -> NULL (for following strcmp)
  actBuf[ilen] = '\0';


  // scroll to bottom
  mark = gtk_text_buffer_get_mark (msgBuf, "insert");
  // gtk_text_buffer_get_iter_at_mark (msgBuf, &iter, &mark)
  gtk_text_view_scroll_to_mark ((GtkTextView*)msgView, mark,
     0.1,              // within_margin
     FALSE,            // use_align
     0., 0.); //0.5, 0.0);        // xalign,yalign



  // spez. f GTK2 : GUI_update__ ();
  // while (gtk_events_pending()) {   // update display
    // gtk_main_iteration();
  // }

  // gtk_text_view_set_editable ((GtkTextView *)TxView, FALSE);
  // gtk_widget_set_sensitive (TxView, TRUE);


  return 0;

}


//================================================================
  int GUI_msgwin_rmLast (MemObj *mo) {
//================================================================
/// delete last textoutputline


  GtkTextIter   iVon, iBis;
  GtkTextBuffer *TxBuf;
  Obj_Unknown   *go;


  go = GUI_obj_pos (mo);
  if(!go) return 0;

  // get textBuffer from textView
  TxBuf = gtk_text_view_get_buffer (GTK_TEXT_VIEW(go->widget));


  gtk_text_buffer_get_end_iter (TxBuf, &iBis);

  iVon = iBis;
  gtk_text_iter_backward_line (&iVon);

  gtk_text_buffer_delete (TxBuf, &iVon, &iBis);

  // printf("GUI_msgwin_rmLast\n");

  return 0;
}


//================== EOF =========================================
