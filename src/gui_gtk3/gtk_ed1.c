/*
 *
 * Copyright (C) 2015 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
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
2014-01-17 GUI_edi__ bugfix. RF.
2012-07-12 First version; extracted from Gtk-toolbox. RF.

-----------------------------------------------------
*/
#ifdef globTag
void GUI_EDI(){}
#endif
/*!
\file ../gui_gtk3/gtk_ed1.c
\brief texteditor
\code

=====================================================
List_functions_end:

GUI_edi__              create editor-window
GUI_edi_Insert         insert text
GUI_edi_InsFile        insert file at curPos, make text selected
GUI_edi_Write          write memSpc -> editor
GUI_edi_Read           get text between 2 positions
GUI_edi_RdChr          get char near charPos
GUI_edi_RdLn           get line (copy)
GUI_edi_mod_ln         modify line
GUI_edi_del            delete text between 2 positions

GUI_edi_ck_cPos_ln     test cursorPosition in active line
GUI_edi_getEof         get FileSize (total-chars-nr, EOF-Position)
GUI_edi_getLnMax       get total-lines-nr
GUI_edi_getLnr         get LineNr of act.Curpos
GUI_edi_setLnr         .
GUI_edi_getCpos        get Cursorposition as charOffset
GUI_edi_setCpos        set to curPos and view text at curPos
GUI_edi_getLsta        get startPos of Line

GUI_edi_sel__          select from-charOffset to-charOffset
GUI_edi_sel_get        get start/endpos of selected-text
GUI_edi_sel_wrf        write selected -> file
GUI_edi_sel_ln         select line
GUI_edi_sel_del        delete selected-text

GUI_edi_scroll_s       scroll to act.charOffset (= selected)
GUI_edi_Focus          set focus to EditWindow
GUI_edi_getModif       get if buffer is modfied 

GUI_ed1_cb1            INTERNAL
GUI_ed1_cb2            INTERNAL
GUI_ed1_cb3            INTERNAL
GUI_ed1_decode         INTERNAL

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


Editor is UTF8;     charOffsets ARE NOT BYTE-OFFSETS !

lnr < iter            gtk_text_iter_get_line
iter < lnr            gtk_text_buffer_get_iter_at_line

byte-offset-functions exist only for the active line !
  byte-offset < iter:   gtk_text_iter_get_line_index
  iter < byte-offset    gtk_text_iter_set_line_index

char-offset < iter    gtk_text_iter_get_offset
iter < char-offset    gtk_text_buffer_get_iter_at_offset,gtk_text_iter_set_offset


2 markers: "insert" and "selection_bound"-marker.

make run -f tst_ed1.mak


-----------------
g_utf8_strlen
g_utf8_validate
g_utf8_offset_to_pointer
g_utf8_find_prev_char
g_convert
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>

#include "../ut/ut_umem.h"                    // Memspc
#include "../ut/ut_mem.h"                     // MEM_*

#include "../gui/gui_types.h"                 // TYP_Event..
#include "../gui/gui_base.h"                  // GUI_obj_*

#include "../gui_gtk3/gtk_base.h"
#include "../gui_gtk3/gtk_ed1.h"

// #include "../gui/gui__.h"




static MemObj        wEd;
static GtkWidget     *GUI_ed1_view;
static GtkTextBuffer *GUI_ed1_buff;
static Obj_gui1      *GUI_ed1_ActObj;
static long          GUI_ed1_cPos;
static int           GUI_ed1_stat=0;
static int           GUI_ed1_MB1_state = 0;   // 0=mousebutton1-released; 1=pressed
static int           GUI_ed1_shift_state = 0; // 0=shift-released; 1=pressed
static int           GUI_ed1_cntrl_state = 0; // 0=control-released; 1=pressed
static int           GUI_ed1_alt_state = 0;   // 0=alt-released; 1=pressed



// Pointer to locale charSet; NULL if UTF8 !
static char  *GUI_ed1_lcSet;




//================================================================
  int GUI_edi_getModif (MemObj *mo) {
//================================================================
/// GUI_edi_getModif       get if buffer is modfied 
/// if buffer is modied: save; this call resets the state.
/// returns     1=modified; 0=unmodified.

  GtkTextBuffer  *tb;
  int   imod;

//   // set GUI_ed1_view GUI_ed1_buff
//   if(mo) {   // for internal call: mo=NULL
//     if(GUI_ed1_decode(mo)) return -1;
//   }

  // GtkTextBuffer <- GtkTextView
  tb = gtk_text_view_get_buffer (GTK_TEXT_VIEW (GUI_ed1_view));


  imod = gtk_text_buffer_get_modified  (tb);
  gtk_text_buffer_set_modified (tb, FALSE);

  return imod;

}


//==========================================================================
  long GUI_edi_getEof (MemObj *mo) {
//==========================================================================
/// GUI_edi_getEof
/// get total nr of chars (not bytes)

/// gtk_text_buffer_get_char_count      get nr of chars, not nr of bytes !


  long fSiz, p1;


  // set GUI_ed1_view GUI_ed1_buff
  if(mo) {   // for internal call: mo=NULL
    if(GUI_ed1_decode(mo)) return -1;
  }

  // get nr of chars in buffer, not nr of bytes !
  fSiz = gtk_text_buffer_get_char_count (GUI_ed1_buff);

  // get startPos of text; gtk_text_iter_get_text
  // fSiz = g_utf8_offset_to_pointer (ptrStart, nrChar);

  // printf("ex GUI_Ed_getEof %ld\n",fSiz);

  return fSiz;

}


//================================================================
  long GUI_edi_getLnMax (MemObj *mo) {
//================================================================
/// GUI_edi_getLnMax       get total-lines-nr

  long        lNr;


  // set GUI_ed1_view GUI_ed1_buff
  if(mo) {   // for internal call: mo=NULL
    if(GUI_ed1_decode(mo)) return -1;
  }

  // check eof
  lNr = gtk_text_buffer_get_line_count (GUI_ed1_buff);

    // printf("ex GUI_edi_getLnMax %ld\n",lNr);


  return (lNr);

}



//===================================================================
  int GUI_ed1_cb3 (GtkTextBuffer *textbuffer, GtkClipboard *arg1, MemObj mo) {
//===================================================================
// paste-done (user did insert text)

  Obj_gui1     *go;
  int          iEv;
  long         cpos, lnr;
  void         *pTab[3];


  printf("GUI_ed1_cb3 (paste-done)\n");


  cpos = GUI_edi_getCpos (&mo);
  lnr = GUI_edi_getLnr (&mo);
    printf(" cpos=%ld lnr=%ld\n",cpos,lnr);

  iEv = TYP_EventEnter;

  go = GUI_obj_pos (&mo);
  if(!go) return 0;
  if(!go->uFunc) return 0;

  pTab[0] = &iEv;
  pTab[1] = &cpos;
  pTab[2] = &lnr;

  // report -> user
  go->uFunc (go, pTab);

  return  (FALSE);

}


//===================================================================
  int GUI_ed1_cb2 (void *parent, void *iter, void *mark, MemObj mo) {
//===================================================================
/// \code
/// 'set-mark'-callback (user clicked into editor-line)
/// \endcode

  Obj_gui1     *go;
  long         cpos, lnr;
  int          ikey, imod, iEv;
  const char   *p1;
  void         *pTab[3];


  // printf("GUI_ed1_cb2 (set-mark) GUI_ed1_stat=%d\n", GUI_ed1_stat);
  // return (FALSE);

  // get state of shift,ctrl,alt-keys
  imod = GUI_get_keys_mod ();
     // printf(" ed1_cb2-modKeyStat = %d\n",imod);


  // skip userCalls during internal operations
  // app makes "select-line"; skip this
  if(GUI_ed1_stat) return TRUE;

  // skip set-mark with shift-key-ON (block-selection)
  // if(imod & 1) return TRUE;
  GUI_ed1_MB1_state = imod & 1;
  if(GUI_ed1_MB1_state) return (FALSE);


  cpos = gtk_text_iter_get_offset (iter);
  if(cpos == GUI_ed1_cPos) return (FALSE);
  GUI_ed1_cPos = cpos;
    // printf("GUI_ed1_cb2 %ld\n",cpos);


  // skip mark != "insert"
  p1 = gtk_text_mark_get_name (mark);
  if(!p1) return (FALSE);
    // printf("  mark-name = |%s|\n",p1);
  if(strcmp(p1, "insert")) return (FALSE);   // TRUE: do no defaultOperations

  // AP_debug__ ("GUI_ed1_cb2");


  go = GUI_obj_pos (&mo);
  if(!go) return 0;
  if(!go->uFunc) return 0;


  iEv = TYP_EventEnter;

  // lnr = gtk_text_buffer_get_line_count (parent);
  lnr = gtk_text_iter_get_line (iter);
  ++lnr;   // first line = 1 !
    // printf(" cpos=%ld lnr=%ld\n",cpos,lnr);


  pTab[0] = &iEv;
  pTab[1] = &cpos;
  pTab[2] = &lnr;

  go->uFunc (go, pTab);


  return (FALSE);   // TRUE: do no defaultOperations

}


/*
//================================================================
  int GUI_ed1_cb_me (void *parent, void *event, MemObj mo) {
//================================================================

  printf("GUI_ed1_cb_me %d %d\n",mo.mbID,mo.ioff);

 return (FALSE);   // TRUE: do no defaultOperations
}
*/


//================================================================
  int GUI_ed1_cb1 (void *parent, void *event, MemObj mo) {
//================================================================
/// \code
/// INTERNAL
/// returns  0=event   TYP_EventEnter|TYP_EventRelease
///          1=keyvalue
///          2=modifier
/// \endcode
// Events:
//   GDK_BUTTON_PRESS  = 4,
//   GDK_BUTTON_RELEASE  = 7,
//   GDK_KEY_PRESS = 8
//   GDK_KEY_RELEASE = 9
//   GDK_FOCUS_CHANGE  = 12
//   GDK_SHIFT_MASK = 1
//   GDK_CONTROL_MASK = 4
// retCode:
//   TRUE:  do no defaultOperations
//   FALSE: continue with defaultOperations
//
// skip "do defaultOperations" (return TRUE) only with key-press-event


  int          ii, ev_in;
  long         ikey, imod;
  long         iEv;
  void         *pTab[3];
  Obj_gui1     *go;
  GdkModifierType mods;



  // printf("GUI_ed1_cb1 %d %d\n",mo.mbID,mo.ioff);
  // printf("  ev=%d\n",((GdkEvent*)event)->type);


  go = GUI_obj_pos (&mo);
  if(!go) return 0;
  if(!go->uFunc) return 0;


  ev_in = ((GdkEvent*)event)->type;



  //----------------------------------------------------------------
  if(ev_in == GDK_FOCUS_CHANGE) {       // ev = 12
    iEv = TYP_EventEnter;
    // GdkEventFocus 
    ikey = 0;
    imod = 0;
    // test "has-focus" property
    // g_object_get (parent, "has-focus", &imod, NULL);   always 1
      // printf(" TYP_EventEnter %d %d\n",ikey,imod);
    goto L_call_user;
  }


  //----------------------------------------------------------------
  ikey = ((GdkEventKey*)event)->keyval;
  mods = gtk_accelerator_get_default_mod_mask ();
  imod = ((GdkEventKey*)event)->state & mods;
    // printf(" ed1_cb1-ikey=%d imod=%d\n",ikey,imod);


  // block Delete-key (unDelete not possible)
  if((ikey == GUI_KeyDel)||(ikey == GUI_KeyNumDel)) {
    // check if something is selected; 1=yes, 0=no
    ii = gtk_text_buffer_get_has_selection (GUI_ed1_buff);
      // printf(" ed1_cb1-ii=%d\n",ii);
    if(ii) {
      TX_Print("**** delete - use Ctrl-X / Ctrl-V");
      return (TRUE);  // TRUE: do no defaultOperations
    } else {
      return FALSE;  // do normal default-operations
    }
  }

  // do not report keys > ffe0 (Shift, Ctrl, Alt, ..)
  if(ikey > 0xffe0) return FALSE; // do normal default-operations


/*
  //----------------------------------------------------------------
  if(ev_in == GDK_BUTTON_PRESS) { 
      printf(" GDK_BUTTON_PRESS\n");
    // GUI_ed1_MB1_state = 1;
    // return (FALSE);

    iEv = TYP_EventEnter;
    // L1=act.cursorPosition (ikey)
    ikey = GUI_edi_getCpos (NULL);
    // L2=act.lineNr.        (imod)
    imod = GUI_edi_getLnr (NULL);
      printf(" cpos=%ld lnr=%ld\n",ikey,imod);
    return (FALSE);    // FALSE: continue with defaultOperations
    // goto L_call_user;

  }


  //----------------------------------------------------------------
  if(ev_in == GDK_BUTTON_RELEASE) {  // GDK_FOCUS_CHANGE
      printf(" GDK_BUTTON_RELEASE\n");
    // GUI_ed1_MB1_state = 0;
    // return (FALSE);

    iEv = TYP_EventEnter;
    // L1=act.cursorPosition (ikey)
    ikey = GUI_edi_getCpos (NULL);
    // L2=act.lineNr.        (imod)
    imod = GUI_edi_getLnr (NULL);
      printf(" pos=%ld lnr=%ld\n",ikey,imod);
    return (FALSE);    // FALSE: continue with defaultOperations
    // goto L_call_user;
  }
*/

  //----------------------------------------------------------------
  if(ev_in == GDK_KEY_PRESS) {        // ev = 8
    // iEv  = TYP_EventPress;
      // printf(" TYP_EventPress %d %d\n",ikey,imod);

    // report Esc-key to user
    if(ikey == GUI_KeyEsc) {
      UI_Escape_set (0);  // ON
    }

    return FALSE;  // do normal default-operations
  }

  //----------------------------------------------------------------
  if(ev_in == GDK_KEY_RELEASE) {     // ev = 9
    iEv  = TYP_EventRelease;
    // if(ikey == GUI_KeyEsc) UI_Escape_set (OFF);

    GUI_ed1_shift_state = imod&1;
    GUI_ed1_cntrl_state = imod&4;
      // printf(" ed1_cb1-ikey=%ld GUI_ed1_shift_state=%d GUI_ed1_cntrl_state=%d\n",
             // ikey,GUI_ed1_shift_state,GUI_ed1_cntrl_state);

    goto L_default;
  }


  //----------------------------------------------------------------
  // unknown / unused event
    printf("**** GUI_ed1_cb1 unknown / unused event %d\n",ev_in);
  return FALSE;  // do normal default-operations
 

  //================================================================
  L_default:
  // fix keys ..
  if(ikey == 0xffbe) ikey = GUI_KeyF1;  // ff91 <- ffbe (XK_KP_F1=XK_F1)

  // does not activate unDelete (Ctrl-Z)
  // if(ikey == GUI_KeyNumDel) return FALSE;

  // do not report events with shift-down
  if(GUI_ed1_shift_state) return FALSE;

  // Ctrl-down: report only Ctrl-Esc, Ctrl-s; Ctrl-p see UI_key_spcCtrl
  if(GUI_ed1_cntrl_state) {
    if(ikey == GUI_KeyEsc) goto L_call_user;
    if(ikey == 's') goto L_call_user;
    if(ikey == 'p') goto L_call_user;
    if(ikey == 'f') goto L_call_user;
    if(ikey == 'l') goto L_call_user;
    return FALSE;  // do normal default-operations
  }

  // Alt x


  L_call_user:

  pTab[0] = &iEv;
  pTab[1] = &ikey;
  pTab[2] = &imod;

  go->uFunc (go, pTab);
    // printf("ex-GUI_ed1_cb1 imod=%ld\n",imod);

  // if(ikey == GUI_KeyNumDel) return TRUE;


    // - NOT with key-release-event (too late ..?)
  return (FALSE);    // TRUE: do no defaultOperations
                     // FALSE: continue with defaultOperations
}


//===============================================================================
  MemObj GUI_edi__ (MemObj *o_par, void *funcnam, int mode, char *opts) {
//===============================================================================
/// \code
///   o_par       parentBox
///   funcnam     callback-function for Keypress & FocusIn-Events (none: NULL)
///   mode        0 = do not wrap lines;  1=yes, wrap lines
///   opts       options; (HorSiz,VertSiz)
///              HorSiz,VertSiz:  size in characters; default is automatic size.
///                               negative values: size in pixels.
///                              'e' = expand widget; default is fixed size.
///              Examples: "" or "10" or "10e,e"
///                "10e,e" horiz. size 10 chars, hor. and vert. expandable.
///
/// prototyp funcnam:
/// int funcnam (MemObj *mo, void **data); 
///   // data=table of 3 pointers;
///   GUI_DATA_EVENT =*(int*)data[0]=TYP_EventEnter
///   GUI_DATA_L1    =*(long*)data[1]=act.cursorPosition
///   GUI_DATA_L2    =*(long*)data[2]=act.lineNr.
///
///   GUI_DATA_EVENT =*(int*)data[0]=TYP_EventPress|TYP_EventRelease|TYP_EventEnter
///   GUI_DATA_L1    =*(long*)data[1]=keyvalue; eg 'a'
///   GUI_DATA_L2    =*(long*)data[2]=state of modifierkeys;
///                                  &1=shift; &4=ctrl; &8=alt.
///   GUI_OBJ_TYP(mo)  = TYP_GUI_Editor
/// 
/// Example without callback-function:
///   ed1 = GUI_edi__ (&box1, NULL, "");
///
///
/// Example with callback-function:
///   ed2 = GUI_edi__ (&box1, ed1_cb, "50,40");
///   ..
///   int ed1_cb (MemObj *mo, void **data) {
///     printf(" event=%d typ=%d\n",GUI_DATA_EVENT,GUI_OBJ_TYP(mo));
///     printf(" key=%d modi=%d\n",GUI_DATA_I1,GUI_DATA_I2);
///   }
///
/// \endcode

  int           i1, pTyp;
  void          *w_par, *wb;
  GtkWidget     *wev, *wsw;
  GtkTextBuffer *web;
  Obj_gui1      *go;
  // GdkEventAny  ev1;
  // gint         ig1;


  // printf("GUI_edi__ |%s|\n",opts);


  i1 = g_get_charset ((const char **)&GUI_ed1_lcSet);  // i1=TRUE if lcSet==UTF8.
    // printf(" charSet %d  |%s|\n",i1,GUI_ed1_lcSet);
  if(i1 != 0) GUI_ed1_lcSet = NULL;



  // get gtk-widget for parentBox from MemObj
  w_par = GUI_obj_parent__ (&pTyp, o_par);
  if(!w_par) return (UME_obj_invalid_set (-3));


  // get address & memspc
  GUI_obj_spc ((void**)&go, sizeof(Obj_gui1));
  if(!go) return (UME_obj_invalid_set (-1));



  wev = gtk_text_view_new ();

  web = gtk_text_view_get_buffer (GTK_TEXT_VIEW (wev));

  gtk_widget_set_can_focus(wev, TRUE);


  wsw = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (wsw),
          GTK_POLICY_AUTOMATIC,      // hor GTK_POLICY_NEVER,
          GTK_POLICY_AUTOMATIC);     // vert

  // gtk_widget_set_can_focus(wsw, TRUE);

  // wrap: GTK_WRAP_NONE GTK_WRAP_CHAR GTK_WRAP_WORD
  if(mode == 1)
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (wev), GTK_WRAP_CHAR);
  else
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (wev), GTK_WRAP_NONE);


  gtk_text_view_set_editable ((GtkTextView*)wev, TRUE);
  gtk_text_view_set_cursor_visible ((GtkTextView*)wev, TRUE);


  //----------------------------------------------------------------
  if (funcnam) {

    // PROBLEM: wev key-press/release: reports the old cursor-position
    gtk_widget_set_events (GTK_WIDGET(wev),
                           GDK_KEY_PRESS_MASK|
                           GDK_KEY_RELEASE_MASK);


    // wev: PROBLEM: key-press: reports the old cursor-position
    g_signal_connect (G_OBJECT (wev),
                        "key-press-event",
                        G_CALLBACK (GUI_ed1_cb1),
                        PTR_MEMOBJ(go->mem_obj));

    // focus-in-event erforderlich f CAD-select-objects
    g_signal_connect (G_OBJECT (wev),
                        "key-release-event",
                        G_CALLBACK (GUI_ed1_cb1),
                        PTR_MEMOBJ(go->mem_obj));

    // // report delete-action (Ctrl-X)
    // g_signal_connect (G_OBJECT (wev), "cut-clipboard",
                      // G_CALLBACK (GUI_ed1_cbDel),
                      // PTR_MEMOBJ(go->mem_obj));

    // // report delete-action (delete with backspace ..)
    // g_signal_connect (G_OBJECT (wev), "delete-from-cursor",
                      // G_CALLBACK (GUI_ed1_cbDel),
                      // PTR_MEMOBJ(go->mem_obj));



    //----------------------------------------------------------------
    // web-"mark-set" zu oft;
    g_signal_connect (G_OBJECT (web),  "mark-set",
                        G_CALLBACK (GUI_ed1_cb2),
                        PTR_MEMOBJ(go->mem_obj));

    // web-"paste-done"
    g_signal_connect (G_OBJECT (web),   // wev wsw
                        "paste-done",
                        G_CALLBACK (GUI_ed1_cb3),
                        PTR_MEMOBJ(go->mem_obj));



    //----------------------------------------------------------------
    // PROBLEM: wsw does NOT provide key-press-event (only key-release-event)
    gtk_widget_set_events (GTK_WIDGET(wsw),
                           // GDK_BUTTON1_MOTION_MASK|
                           // GDK_FOCUS_CHANGE_MASK|
                           // GDK_ENTER_NOTIFY_MASK |
                           GDK_SHIFT_MASK|
                           GDK_CONTROL_MASK|
                           GDK_KEY_PRESS_MASK|
                           GDK_KEY_RELEASE_MASK|
                           GDK_BUTTON_PRESS_MASK|
                           GDK_BUTTON_RELEASE_MASK);

    // web does not provide key-press/release-event
    // attach "enter-window" (for eg click into window).
    // geht nicht: wsw|web|wev"enter-notify-event";

    gtk_widget_set_can_focus (wsw, TRUE); 

  }


  gtk_widget_set_hexpand (wev, TRUE);             // else no resize !!
  gtk_widget_set_vexpand (wev, TRUE);             // else no resize !!


  //----------------------------------------------------------------
  gtk_container_add (GTK_CONTAINER (wsw), wev);
  gtk_widget_show (wev);

  // GUI_packwn (w_par, wsw, hsiz, vsiz);
  GUI_w_pack1 (pTyp, w_par, wsw, opts);


  //----------------------------------------------------------------
  // fill obj
  go->gio_typ  = TYP_GUI_Editor;
  go->widget   = wev; //wsw;
  go->uFunc    = funcnam;

  GUI_ed1_view = NULL;  // makes error after recalling this edi ! 2014-01-17

  // return memObj
  return (go->mem_obj);

}


//================================================================
  int GUI_edi_InsFile (MemObj *mo, char *fnam) {
//================================================================
/// GUI_edi_InsFile        insert file at curPos, make text selected
// see also ED_add_File
// was GUI_Ed_Ins_f

  GtkTextMark *mk1;
  GtkTextIter it1, it2;
  long        fSiz, cPos1;
  void        *fBuf;


  // set GUI_ed1_view GUI_ed1_buff
  if(mo) {   // for internal call: mo=NULL
    if(GUI_ed1_decode(mo)) return -1;
  }
    

  // get file -> fBuf
  MEM_alloc_file (&fBuf, &fSiz, fnam);
  UTX_str_file (fBuf, &fSiz, fnam);
  if(fSiz < 1) {
    TX_Print("GUI_Ed_Ins_f E001 |%s|",fnam);
    return -1;
  }

  // printf("fBuf=|%s|\n",(char*)fBuf);


  // // get get iters for "insert" & "selection_bound"
  // gtk_text_buffer_get_selection_bounds (wTx->text, &it1, &it2);
  // // insert at iterPos
  // gtk_text_buffer_insert (wTx->text, &it1, txbuf, -1);


  // get iter it1 at CurPos
  mk1 = gtk_text_buffer_get_mark (GUI_ed1_buff, "insert");
  // iter at CurPos
  gtk_text_buffer_get_iter_at_mark (GUI_ed1_buff, &it1, mk1);
  cPos1 = gtk_text_iter_get_offset (&it1);


  // insert text txbuf at curpos; 
  gtk_text_buffer_insert_at_cursor (GUI_ed1_buff, fBuf, fSiz);


  // get iter it2 at new CurPos
  mk1 = gtk_text_buffer_get_mark (GUI_ed1_buff, "insert");
  gtk_text_buffer_get_iter_at_mark (GUI_ed1_buff, &it2, mk1);

  // set to old curPos it1
  gtk_text_buffer_get_iter_at_offset (GUI_ed1_buff, &it1, cPos1);
  gtk_text_buffer_place_cursor (GUI_ed1_buff, &it1);

  // select region curPos - it2
  gtk_text_buffer_move_mark_by_name (GUI_ed1_buff, "selection_bound", &it2);

  return 0;

}


//====================================================================
  int GUI_edi_Insert (MemObj *mo, long cpos, char* txbuf, long txlen) {
//====================================================================
/// \code
/// text einfuegen
/// cpos zB von GUI_edi_getCpos
/// \endcode

  GtkTextIter   it1;


  // printf("GUI_edi_Insert %ld %ld |%s|\n",cpos,txlen,txbuf);

  // set GUI_ed1_view GUI_ed1_buff
  if(mo) {   // for internal call: mo=NULL
    if(GUI_ed1_decode(mo)) return -1;
  }


  // get iter from char-offset (nr of chars, not nr of bytes !)
  gtk_text_buffer_get_iter_at_offset (GUI_ed1_buff, &it1, cpos);

  // insert at iterPos
  gtk_text_buffer_insert (GUI_ed1_buff, &it1, txbuf, -1);

    
  return 0;

} 

 
//===========================================================================
  int GUI_edi_Write (MemObj *mo, char* txbuf, long *txlen, long maxlen) {
//===========================================================================
/// \code
/// das gesamte Editfenster neu schreiben (txbuf(=mem) -> Window).
/// ATTENTION: GUI_edi_Write can modify txbuf (changes into UTF8)
/// Modif.:
/// 2013-04-03 do not set cursor at eof.
/// \endcode

  int         i1, i2;
  long        l1, l2;
  char        *p1;
  GtkTextIter it1;


  // printf("GUI_edi_Write %ld %ld\n",*txlen,maxlen);
  // printf(" FilBuf0: %c %c %c %c\n",txbuf[0],txbuf[1], txbuf[2],txbuf[3]);
  // printf("|"); UTX_dump_c__(txbuf,(int)*txlen);printf("|");
  // printf("|"); UTX_dump_s__(txbuf,(int)*txlen);printf("|");
  // UTF_dump__ ();
  // i2 = *txlen;for(i1=0;i1<i2;++i1)printf("|%c| %d\n",txbuf[i1],txbuf[i1]);


  // set GUI_ed1_view GUI_ed1_buff
  if(mo) {   // for internal call: mo=NULL
    if(GUI_ed1_decode(mo)) return -1;
  }

  // test if input is valid UTF8; 1==OK, 0=must_convert.
  if(GUI_ed1_lcSet) {
    i1 = *txlen;
    i2 = g_utf8_validate (txbuf, i1, (const gchar**)&p1);  // 2014-01-19
      // printf(" g_utf8_validate-i2=%d\n",i2);
    if(i2 == 0) {
      // charsetto="UTF-8", charsetfrom="ISO-8859-1"|locale
      // GEHT NICHT mit UI_Ed_lcSet als charsetfrom !!!?
      p1 = g_convert (txbuf, *txlen, "UTF-8", "ISO-8859-1",
                      (gsize*)&l1, (gsize*)&l2, NULL);
        // printf(" g_convert %d %d\n",i1,i2);
      if(p1 == NULL) {TX_Error("GUI_edi_Write convert-ERR"); return -1;}
      if(l2 >= maxlen) {
        TX_Error("GUI_edi_Write convert-EOM"); g_free(p1); return -1;}
      strncpy(txbuf, p1, l2);
      txbuf[l2] = '\0';      // 2013-10-28
      *txlen = l2;
      g_free(p1);
    }
  }


  // provide text to editor
    // printf(" gtk_text_buffer_set_text\n");
  i1 = *txlen;
  gtk_text_buffer_set_text (GUI_ed1_buff, txbuf, i1);
  // statt txlen -1 = bis '\0'.

/*
  // curPos ans EOF setzen
  // set iter from cpos
  gtk_text_buffer_get_iter_at_offset (GUI_ed1_buff, &it1, *txlen);

  // moves the "insert" and "selection_bound" marks simultaneously
  gtk_text_buffer_place_cursor (GUI_ed1_buff, &it1);
*/

    // printf(" ex GUI_edi_Write\n");

  return 0;

}


//================================================================
  int GUI_edi_Read (char* txbuf, long *txlen, long p1, long p2, MemObj *mo) {
//================================================================
/// \code
/// get text between 2 positions
/// p1, p2  character-offset, not byte offsets !
/// get whole window with: p1=0L, p2=-1L
/// Input:
///   txlen         size of txbuf
/// \endcode

  
  int         irc;
  char        *text;
  long        sSiz, l1, l2;
  GtkTextIter it1, it2;


  // printf("GUI_edi_Read siz=%ld %ld %ld\n",*txlen,p1,p2);

  // set GUI_ed1_view GUI_ed1_buff
  if(mo) {   // for internal call: mo=NULL
    // GUI_obj_dump_mo (mo); // dump MemObj
    if(GUI_ed1_decode(mo)) return -1;
  }

  
  gtk_text_buffer_get_iter_at_offset (GUI_ed1_buff, &it1, p1);
  

  // if(p2 < 0) {
    // // get offset at eof
    // gtk_text_buffer_get_end_iter (GUI_ed1_buff, &it2);
  // } else {
    gtk_text_buffer_get_iter_at_offset (GUI_ed1_buff, &it2, p2); // -1 gives EOF
  // }

  sSiz = GUI_edi_Rd2it (txbuf, *txlen, &it1, &it2, NULL);
  if(sSiz < 0) return -1;
  else *txlen = sSiz;
  return 0;

/*
  // get textPointer;
  text = gtk_text_iter_get_text (&it1, &it2);


  sSiz = strlen(text);   // g_utf8_strlen
    printf(" sSiz=%ld\n",sSiz);


  if(sSiz < *txlen) {
    // copy out
    strncpy(txbuf, text, sSiz);
    txbuf[sSiz] = '\0';
    *txlen = sSiz;
    irc = 0;

  } else {
    *txlen = sSiz;
    irc = -1;
  }

  g_free (text);

  printf("ex GUI_edi_Read %ld\n",*txlen);
  // printf("|"); UTX_dump_s__(txbuf,*txlen+1);printf("|");

  return irc;
*/

}


//================================================================
  int GUI_edi_mod_ln (MemObj *mo, long lNr, char *newLn) {
//================================================================
/// change line lNr into <newLn>

  GtkTextIter it1, it2;


  // printf("GUI_edi_mod_ln %ld |%s|\n",lNr,newLn);


  if(mo) {   // for internal call: mo=NULL
    if(GUI_ed1_decode(mo)) return -1;
  }


  // DOES NOT WORK FOR LAST LINE !
  // also used in GUI_edi_RdLn
  if(lNr == gtk_text_buffer_get_line_count (GUI_ed1_buff)) {
    gtk_text_buffer_get_end_iter (GUI_ed1_buff, &it2);
  } else {
    gtk_text_buffer_get_iter_at_line (GUI_ed1_buff, &it2, lNr);
    gtk_text_iter_backward_char (&it2);  // skip '\n'
  }

  --lNr;
  gtk_text_buffer_get_iter_at_line (GUI_ed1_buff, &it1, lNr);


  // del
  gtk_text_buffer_delete (GUI_ed1_buff, &it1, &it2);


  // insert at iterPos
  gtk_text_buffer_insert (GUI_ed1_buff, &it1, newLn, -1);


  return 0;

}


//================================================================
  long GUI_edi_RdLn (char *sOut, int sSiz, long lNr, MemObj *mo) {
//================================================================
/// \code
/// GUI_edi_RdLn           get line (copy)
/// RetCod:  size of line or -1 (EOF)
/// \endcode

// was GUI_Ed_RdLnAct

  int         i1;
  GtkTextIter it1, it2;


  // set GUI_ed1_view GUI_ed1_buff
  if(mo) {   // for internal call: mo=NULL
    if(GUI_ed1_decode(mo)) return -1;
  }

  // printf("GUI_edi_RdLn %ld\n",lNr);
  // printf("  lMax=%d\n",gtk_text_buffer_get_line_count (GUI_ed1_buff));


  i1 = gtk_text_buffer_get_line_count (GUI_ed1_buff); 
    // printf("GUI_edi_RdLn %ld tot=%d\n",lNr,i1);


  if(lNr > i1) {
    return -1;      // past EOF

  } else if(lNr == i1) {
    // last line
    gtk_text_buffer_get_end_iter (GUI_ed1_buff, &it2);
  } else {
    gtk_text_buffer_get_iter_at_line (GUI_ed1_buff, &it2, lNr);
  } 

  --lNr;
  gtk_text_buffer_get_iter_at_line (GUI_ed1_buff, &it1, lNr);

  return GUI_edi_Rd2it (sOut, sSiz, &it1, &it2, NULL);

}


//================================================================
  int GUI_edi_Rd2it (char* txbuf, long txlen,
                     GtkTextIter *it1, GtkTextIter *it2) {
//================================================================
/// \code
/// get text between 2 iters INTERNAL
/// p1, p2  character-offset, not byte offsets !
/// get whole window with: p1=0L, p2=-1L
/// Input:
///   txlen         size of txbuf
/// \endcode


  char        *text;
  long        sSiz;


  // printf("GUI_edi_Rd2it %ld\n",txlen);

  // get textPointer;
  text = gtk_text_iter_get_text (it1, it2);
  if(!text) {  // 2013-12-29
    TX_Error("GUI_edi_Rd2it E001");
    return -1;
  }


  sSiz = strlen(text);   // g_utf8_strlen
    // printf(" sSiz=%ld\n",sSiz);


  if(sSiz < txlen) {
    // remove '\n'
    // if(text[sSiz - 1] == '\n') --sSiz;  // 2012-11-18 removed
    // copy out
    strncpy(txbuf, text, sSiz);
    txbuf[sSiz] = '\0';

  } else {
    sSiz = -1;
  }

  g_free (text);

  // printf("ex GUI_edi_Rd2it %ld\n",sSiz);
  // printf("|"); UTX_dump_s__(txbuf,*txlen+1);printf("|");

  return sSiz;

}


//================================================================
  int GUI_edi_ck_cPos_ln (MemObj *mo, int *lPos, int *fPos) {
//================================================================
// GUI_edi_ck_cPos_ln       test cursorPosition in active line
//   following blanks are ignored !
// output:    cursor is -
//   lPos     0    at start of line (in column 1)
//            1    between start of line and end of line
//            2    at end of line
//            3    line is empty
//   fPos     0    at start of file
//            1    between start of file and end of file
//            2    at end of file
//            3    file is empty


  long        c1, c2;
  gboolean    b1, b2;
  GtkTextMark *mk1;
  GtkTextIter it1;
  // char        c1;


  // printf("GUI_edi_ck_cPos_ln\n");

  // set GUI_ed1_view GUI_ed1_buff
  if(mo) {   // for internal call: mo=NULL
    if(GUI_ed1_decode(mo)) return -1;
  }

  
  // get mark at CurPos
  mk1 = gtk_text_buffer_get_mark (GUI_ed1_buff, "insert");

  // iter at CurPos
  gtk_text_buffer_get_iter_at_mark (GUI_ed1_buff, &it1, mk1);

  // get unicode-char at right of curpos
  c2 = gtk_text_iter_get_char (&it1);
    // printf(" edi_ck_cPos_ln-c2 = %ld %c\n",c2,(int)c2);
  if(c2 == 0)  {
    // cursor is at end of file
    *fPos = 2; *lPos = 2;
  } else if(c2 == 10) {
    // cursor is at end of line
    *fPos = 1; *lPos = 2;
  } else {
    // cursor is between start and end of line
    *fPos = 1; *lPos = 1;
  }

  // move iter back
  b1 = gtk_text_iter_backward_chars (&it1, 1);
  // false if left of 1. char; returns Null !.
  if(b1 == FALSE) {
    // cur at start of file
    if(*fPos == 2) {
      *fPos = 3; // file is empty - cur is at start and at end of file
      *lPos = 3; // line is empty - cur is at start and at end of file
    } else {
      *fPos = 0; // cur is at start of file
      *lPos = 0; // cur is at start of line
    }
    goto L_exit;
  }

  // get char left of cursor
  c1 = gtk_text_iter_get_char (&it1);
    // printf(" edi_ck_cPos_ln-c1 = %ld %c\n",c1,(int)c1);
  if(c1 == 10) {
    // left char = start of line
    if(*lPos == 2) {
      // right char is end of line
      *lPos = 3; // line is empty
    } else {
      // right char is between start and end of line
      *lPos = 0; // cursor is in column 1
    }
  // } else {
    // // left char = between start and end of line
  }

  L_exit:
    // printf(" ex-GUI_edi_ck_cPos_ln fPos = %d lPos = %d\n",*fPos,*lPos);
  return 0;

}


//==========================================================================
  char GUI_edi_RdChr (MemObj *mo, int offset) {
//==========================================================================
/// \code
/// read char near cursor;
/// offset  0 = char right of cursor
/// offset -1 = char left of cursor
/// returns  '\0' for out-of-file-positions.
/// returns  -1 for a multibyte-character
/// \endcode

  GtkTextMark *mk1;
  GtkTextIter it1;
  gboolean    b1;
  long        uc;
  // char        c1;


  // printf("GUI_edi_RdChr |%c| %d\n",c1,cpos);

  // set GUI_ed1_view GUI_ed1_buff
  if(mo) {   // for internal call: mo=NULL
    if(GUI_ed1_decode(mo)) return -1;
  }


  // get mark at CurPos
  mk1 = gtk_text_buffer_get_mark (GUI_ed1_buff, "insert");

  // iter at CurPos
  gtk_text_buffer_get_iter_at_mark (GUI_ed1_buff, &it1, mk1);

  // move iter back
  if(offset < 0) {
    b1 = gtk_text_iter_backward_chars (&it1, -offset);
    // false if left of 1. char; returns Null !.
    if(b1 == FALSE) return '\0';
  }

  // move iter forw
  if(offset > 0) {
    b1 = gtk_text_iter_forward_chars (&it1, offset);
    // false if left of 1. char; returns Null !.
    if(b1 == FALSE) return '\0';
  }


  // get unicode-char at curpos
  uc = gtk_text_iter_get_char (&it1);

  if((uc > 127)||(uc < 0)) return -1;

  // printf("ex GUI_edi_RdChr |%ld|\n",uc);

  return uc;

}


//==========================================================================
  long GUI_edi_getCpos (MemObj *mo) {
//==========================================================================
/// \code
/// get cursorposition as offset from start-of-file.
/// 0=startpos; Linedelimiters '\n' is one char.
/// cpos -> Zeilennummer: UTF_GetLnrPos
/// \endcode

  GtkTextMark *mk1;
  GtkTextIter it1;

  long cpos;

  // printf("GUI_edi_getCpos \n");

  // set GUI_ed1_view GUI_ed1_buff
  if(mo) {   // for internal call: mo=NULL
    if(GUI_ed1_decode(mo)) return -1;
  }

  // get mark at CurPos
  mk1 = gtk_text_buffer_get_mark (GUI_ed1_buff, "insert");

  // iter at CurPos
  gtk_text_buffer_get_iter_at_mark (GUI_ed1_buff, &it1, mk1);

  // get offset of CurPos
  cpos = gtk_text_iter_get_offset (&it1);
  // cpos = gtk_text_iter_get_line_index (&it1);  // byte-offset in line

  // printf("ex GUI_edi_getCpos %d\n",cpos);

  return cpos;

}


//==========================================================================
  int GUI_edi_setCpos (MemObj *mo, long cpos) {
//==========================================================================
/// \code
/// set to curPos and view text at curPos
/// cpos  -1L  is end of file
/// \endcode

  GtkTextIter it1;

  // printf("GUI_edi_setCpos %ld\n",cpos);

  // set GUI_ed1_view GUI_ed1_buff
  if(mo) {   // for internal call: mo=NULL
    if(GUI_ed1_decode(mo)) return -1;
  }

  // set iter from cpos
  gtk_text_buffer_get_iter_at_offset (GUI_ed1_buff, &it1, cpos);

  // moves the "insert" and "selection_bound" marks simultaneously
  gtk_text_buffer_place_cursor (GUI_ed1_buff, &it1);

  // scroll & focus
  GUI_edi_scroll_s (NULL);

  return 1;

}


//==========================================================================
  long GUI_edi_getLsta  (MemObj *mo, long lNr) {
//==========================================================================
/// get startPos of Line; -1=line does not exist.

  GtkTextIter it1;
  long        cpos1;


  // set GUI_ed1_view GUI_ed1_buff
  if(mo) {   // for internal call: mo=NULL
    if(GUI_ed1_decode(mo)) return -1;
  }

  --lNr;  // first line = 1 !
  if(lNr < 0) return (-1L);

  // check eof
  if(lNr >= gtk_text_buffer_get_line_count (GUI_ed1_buff)) {
    gtk_text_buffer_get_end_iter (GUI_ed1_buff, &it1);
  } else {
    // does not find if lNr exists !
    // get iter at startPos of line from LineNr
    gtk_text_buffer_get_iter_at_line (GUI_ed1_buff, &it1, lNr);
  }

  // offset from iter
  cpos1 = gtk_text_iter_get_offset (&it1);

    // printf("ex GUI_edi_getLsta %ld %ld\n",cpos1,lNr);

  return cpos1;

}


//================================================================
  long GUI_edi_getLnr (MemObj *mo) {
//================================================================
/// \code
/// GUI_edi_getLnr        get LineNr of act.Curpos
/// First line = 1 !
/// \endcode
//  die Filesize und cPos ist unterschiedlich, weil gtk Sonderzeichen nur
//  als 1 char behandelt !!!
// Man darf daher die cPos nicht zum Positionieren im Mem benutzen !!!!

  int         lNr;
  GtkTextMark *mk1;
  GtkTextIter it1;

  // printf("GUI_edi_getLnr \n");

  // set GUI_ed1_view GUI_ed1_buff
  if(mo) {   // for internal call: mo=NULL
    if(GUI_ed1_decode(mo)) return -1;
  }

  // get mark at CurPos
  mk1 = gtk_text_buffer_get_mark (GUI_ed1_buff, "insert");

  // iter at CurPos
  gtk_text_buffer_get_iter_at_mark (GUI_ed1_buff, &it1, mk1);


  // get LineNr from iter
  lNr = gtk_text_iter_get_line (&it1);

  // gCAd beginnt mit LineNr 1 !!!
  ++lNr;

  // // offset from iter
  // cpos1 = gtk_text_iter_get_offset (&it1);


    // printf("ex GUI_edi_getLnr %d\n",lNr);

  return lNr;

}


//================================================================
  int GUI_edi_setLnr (MemObj *mo, long lNr) {
//================================================================
// returns total nr of lines

  int         totNr;
  GtkTextIter it1;

  // printf("GUI_edi_setLnr %ld\n",lNr);
  // if(lNr==5) AP_debug__ ("GUI_edi_setLnr");


  GUI_ed1_stat = 1;

  --lNr;    // first line = 1 // 2012-07-13

  // set GUI_ed1_view GUI_ed1_buff
  if(mo) {   // for internal call: mo=NULL
    if(GUI_ed1_decode(mo)) return -1;
  }


  // set iter from lNr
  // get total lineNr
  totNr = gtk_text_buffer_get_line_count (GUI_ed1_buff);
     // printf(" setLnr-total %d\n",totNr);


  if(lNr >= totNr) {
    // get iter at eof
    gtk_text_buffer_get_end_iter (GUI_ed1_buff, &it1);

  } else {
    // get iter at line <lNr>
    gtk_text_buffer_get_iter_at_line (GUI_ed1_buff, &it1, lNr);
  }

  // moves the "insert" and "selection_bound" marks simultaneously
  gtk_text_buffer_place_cursor (GUI_ed1_buff, &it1);

  // scroll & focus
  GUI_edi_scroll_s (NULL);

  GUI_ed1_stat = 0;

    // printf("ex-GUI_edi_setLnr %ld\n",lNr);

  return totNr;

}


//================================================================
  int GUI_edi_scroll_s (MemObj *mo) {
//================================================================
/// scroll to active insert-mark
/// makes unselect ?

  GtkTextMark *mk1;
  GtkTextIter it1;

  // printf("GUI_edi_scroll_s \n");

  if(mo) {   // for internal call: mo=NULL
    if(GUI_ed1_decode(mo)) return -1;
  }

  // scroll to insert-mark:
  mk1 = gtk_text_buffer_get_mark (GUI_ed1_buff, "insert");

  // gtk_text_view_scroll_to_iter (GTK_TEXT_VIEW(wTx->view), &it1,
  gtk_text_view_scroll_to_mark (GTK_TEXT_VIEW (GUI_ed1_view), mk1,
     0.1,              // within_margin
     FALSE,            // use_align
     0.5, 0.0);        // xalign,yalign


  // ohne foc selection nicht sichbar !!
  GUI_edi_Focus (NULL);


  return 0;

}


//===================================================================
  void GUI_edi_Focus (MemObj *mo) {
//===================================================================
/// den Focus auf EditWindow

  // printf("GUI_edi_Focus\n");
  // set GUI_ed1_view GUI_ed1_buff

//   if(mo) {   // for internal call: mo=NULL
//     if(GUI_ed1_decode(mo)) return;
//   }

  // gtk_widget_set_sensitive (wTx->win, TRUE);


  gtk_widget_grab_focus (GUI_ed1_view);
  // gtk_widget_grab_focus (wTx->text);
  // gtk_widget_grab_focus (wTx->win);


  gtk_text_view_set_cursor_visible ((GtkTextView*)GUI_ed1_view, TRUE);

}


//================================================================
  int GUI_edi_sel__ (MemObj *mo, long von, long bis) {
//================================================================
/// \code
/// select_region from-curPos to-curPos
/// if(from-curPos < 0)  - unsect all !
///   get offsets (von,bis) eg with UTF_offset_
/// \endcode

  GtkTextIter it1;
  GtkTextMark *mk1;


  // printf("GUI_edi_sel__ %d %d\n",von,bis);

  if(mo) {   // for internal call: mo=NULL
    if(GUI_ed1_decode(mo)) return -1;
  }


  // get iter from offset
  gtk_text_buffer_get_iter_at_offset (GUI_ed1_buff, &it1, von);

  // move "insert" & "selection_bound"-marks
  gtk_text_buffer_place_cursor (GUI_ed1_buff, &it1);

  // get iter from offset
  gtk_text_buffer_get_iter_at_offset (GUI_ed1_buff, &it1, bis);

  // set end - mark
  gtk_text_buffer_move_mark_by_name (GUI_ed1_buff, "selection_bound", &it1);


  return 0;
}


//================================================================
  // int GUI_edi_sel_get (long *p1, long *p2, MemObj *mo) {
  int GUI_edi_sel_get (char **txt, MemObj *mo) {
//================================================================
/// \code
/// GUI_edi_sel_get        get start/endpos of selected-text
/// \endcode

  GtkTextIter it1, it2;
  long  i1;
  char  *text;


  // printf("GUI_edi_sel_get |%s|\n",fnam);

  // set GUI_ed1_view GUI_ed1_buff
  if(mo) {   // for internal call: mo=NULL
    if(GUI_ed1_decode(mo)) return -1;
  }

  // get get iters for "insert" & "selection_bound"
  gtk_text_buffer_get_selection_bounds (GUI_ed1_buff, &it1, &it2);

  // get offsets
  // *p1 = (long)gtk_text_iter_get_offset (&it1);
  // *p2 = (long)gtk_text_iter_get_offset (&it2);

  *txt = gtk_text_iter_get_text (&it1, &it2);

  i1 = strlen(*txt);

  return i1;

}


//================================================================
  int GUI_edi_sel_wrf (MemObj *mo, char *fnam) {
//================================================================
/// \code
/// GUI_edi_sel_wrf              write selected -> file
/// returns nr of chars
/// \endcode

  int  i1;
  char *text;
  FILE *fpo;
  GtkTextIter it1, it2;


  printf("GUI_Ed_sel_wrf |%s|\n",fnam);

  if(mo) {   // for internal call: mo=NULL
    if(GUI_ed1_decode(mo)) return -1;
  } 

  if((fpo=fopen(fnam,"wb")) == NULL) {
    TX_Print("GUI_Ed_sel_wrf E001 |%s|",fnam);
    return -2;
  }

  // get get iters for "insert" & "selection_bound"
  gtk_text_buffer_get_selection_bounds (GUI_ed1_buff, &it1, &it2);

  // get text
  text = gtk_text_iter_get_text (&it1, &it2);
    printf("GUI_Ed_sel_wrf |%s|\n",text);

  i1 = strlen(text);

  fwrite(text, i1, 1, fpo);

  g_free (text);

  fclose(fpo);

  return i1;

}


//================================================================
  int GUI_edi_sel_ln (MemObj *mo, long lNr) {
//================================================================
/// \code
/// GUI_edi_sel_ln        select Line, set Curpos to Line.
/// Erste Zeile ist 1
/// rc -1: lNr does not exist
/// ACHTUNG: die ZeilenStart/Endposition wird aus dem mem gelesen !!!!
/// \endcode

  int   i1, irc=0, cNr;
  GtkTextIter it1, it2;


  // printf("GUI_edi_sel_ln %ld\n",lNr);

  // set GUI_ed1_view GUI_ed1_buff
  if(mo) {   // for internal call: mo=NULL
    if(GUI_ed1_decode(mo)) return -1;
  }

  GUI_ed1_stat = 1;
  // g_signal_handlers_block_by_func (   // geht ned ..
      // G_OBJECT (GUI_ed1_buff),
      // G_CALLBACK (GUI_ed1_cb2), GUI_ed1_ActObj->uFunc);


  --lNr;


  // get Iter at Linestart
  // DOES NOT WORK FOR LAST LINE !
  if(lNr == gtk_text_buffer_get_line_count (GUI_ed1_buff)) {
    irc = -2;
    goto L_exit;
  } else {
    gtk_text_buffer_get_iter_at_line (GUI_ed1_buff, &it1, lNr);
  }


  // move "insert" & "selection_bound"-marks
  gtk_text_buffer_place_cursor (GUI_ed1_buff, &it1);


  // get Iter at start of next Line
  // ++lNr;
  if(lNr == gtk_text_buffer_get_line_count (GUI_ed1_buff)) {
    gtk_text_buffer_get_end_iter (GUI_ed1_buff, &it2);
  } else {
    // gtk_text_buffer_get_iter_at_line (GUI_ed1_buff, &it2, lNr);
    cNr = gtk_text_iter_get_bytes_in_line (&it1);
      // printf(" cNr = %d\n",cNr);

    --cNr;  // LF
    gtk_text_iter_forward_chars (&it1, cNr);
  }

  // set mark
  gtk_text_buffer_move_mark_by_name (GUI_ed1_buff, "selection_bound", &it1);

  // scroll & focus
  GUI_edi_scroll_s (NULL);




  L_exit:
  GUI_ed1_stat = 0;
  // g_signal_handlers_unblock_by_func (    // geht ned ..
    // G_OBJECT (GUI_ed1_buff),
    // G_CALLBACK (GUI_ed1_cb2), GUI_ed1_ActObj->uFunc);

  // printf("ex GUI_edi_sel_ln\n");


  return irc;

}


//================================================================
  int GUI_edi_sel_del (MemObj *mo) {
//================================================================
/// GUI_edi_sel_del        delete selected text

  // printf("GUI_Ed_sel_del \n");

  if(mo) {   // for internal call: mo=NULL
    if(GUI_ed1_decode(mo)) return -1;
  }

  gtk_text_buffer_delete_selection (GUI_ed1_buff, TRUE, TRUE);

  return 0;

}
 

//================================================================
  int GUI_edi_del (MemObj *mo, long von, long bis) {
//================================================================
/// GUI_edi_del            delete from-curPos to-curPos

  GtkTextIter it1, it2;
  int         txlen;


  // printf("GUI_edi_del %d %d %d\n",von,bis,txlen);

  if(mo) {   // for internal call: mo=NULL
    if(GUI_ed1_decode(mo)) return -1;
  }


  // get nr of chars in buffer
  txlen = gtk_text_buffer_get_char_count (GUI_ed1_buff);


  if(von >= txlen) return -1;
  if(bis > txlen) bis = txlen;

  // set iter from cpos
  gtk_text_buffer_get_iter_at_offset (GUI_ed1_buff, &it1, von);
  gtk_text_buffer_get_iter_at_offset (GUI_ed1_buff, &it2, bis);

  // del
  gtk_text_buffer_delete (GUI_ed1_buff, &it1, &it2);


  // // den Cursor noch in die naechste zeile bewegen
  // cpos = von + 1;
  // // gtk_editable_set_position ((GtkEditable*)text, cpos);
  // GUI_edi_scroll_u (text, cpos);

  return 0;

}


//================================================================
  int GUI_ed1_decode (MemObj *mo) {
//================================================================
/// \code
/// INTERNAL
/// set GUI_tree1_tree, GUI_tree1_view, GUI_tree1_model and GUI_tree1_store
/// \endcode



  // printf("GUI_ed1_decode\n");

  GUI_ed1_ActObj = GUI_obj_pos (mo);
  if(!GUI_ed1_ActObj) return -1;
    // printf(" GUI_ed1_ActObj=%p\n",GUI_ed1_ActObj);

  if(GUI_ed1_ActObj->widget != GUI_ed1_view)  {
    GUI_ed1_view = GUI_ed1_ActObj->widget;
      // printf(" GUI_ed1_view=%p\n",GUI_ed1_view);
    // GtkTextBuffer <- GtkTextView
    GUI_ed1_buff = gtk_text_view_get_buffer (GTK_TEXT_VIEW (GUI_ed1_view));
      // printf(" GUI_ed1_buff=%p\n",GUI_ed1_buff);
    if(!GUI_ed1_buff) {   // 2013-12-29
      TX_Error("GUI_ed1_decode E001");
      return -1;
    }
  }

  return 0;

}


// EOF
