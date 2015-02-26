/*
 * gtk_base.c
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
Basefunctions, boxes


-----------------------------------------------------
OFFEN:

-----------------------------------------------------
Modifications:
2012-07-08 GUI_Win__ GUI_DATA_I1 added. RF.
2012-02-01 extracted from ut_gtk.c.  RF.


-----------------------------------------------------
*/
#ifdef globTag
void GUI_GTK(){}
#endif
/*!
\file ../gui_gtk2/gtk_base.c
\ingroup grp_gui
\code

=====================================================
List_functions_start:

GUI_Init__             must be 1. call ..

GUI_Win__              create new window
GUI_Win_ev_key         add callback for the key-events
GUI_Win_ev_button      add callback for the mouse-button event
GUI_Win_go             windowSetup finished; display it ..
GUI_Win_up             window always on top
GUI_WinTit             set titletext
GUI_Win_kill           destroy window
GUI_Win_exist          returns nr of windows with windowTitle <winTit>;
GUI_main_quit          close application

GUI_box_h              horizontal container for widgets
GUI_box_v              vertical container for widgets
GUI_box__              container for widgets 

GUI_sep_h              horiz. separationline (Ruler)
GUI_sep_v              vertical separationline (Ruler)
GUI_sep__              horiz.|vertical separationline (Ruler)

GUI_spc_h              horiz. (free) space
GUI_spc_v              vertical (free) space
GUI_spc__              horiz.|vertical (free) space

GUI_Tip                Tooltip (infotext for widget)
GUI_cursor_set         set/change cursor
GUI_key_set1           change uncommon keys

GUI_hide__             hide object
GUI_show__             show object
GUI_obj_focus          set focus to widget
GUI_set_show           show/hide object
GUI_set_enable         make (groups of) widgets unselectable disable enable
GUI_get_keys_mod       get state of modifier-keys;
GUI_is_alive           test if window win (GtkWidget) is active
GUI_update__           update all windows
GUI_idle__             register a idle-function
GUI_timer__            register a timer-function

GUI_SETDAT_E           create a GUI_DATA-block (event)
GUI_SETDAT_EI          create a GUI_DATA-block (event + integer-data)
GUI_SETDAT_ES          create a GUI_DATA-block (event + string-data)

GUI_OBJ_TYP            get gui-typ from MemObj
GUI_OBJ_SRC            get gui-src from MemObj (the gui-widget-nr)

GUI_get_version        returns guiSW, SW-mayor-version, SW-minor-version

GUI_Win_exit           INTERNAL callback from destroy window
GUI_Win_key            INTERNAL callback key_event
GUI_Win_button         INTERNAL callback button_press_event
GUI_packwn             INTERNAL pack widget normal
GUI_siz_set            INTERNAL set widget-size
GUI_siz_eval           INTERNAL eval widget-size
GUI_ck_version         INTERNAL check GTK_MAJOR_VERSION,GTK_MINOR_VERSION

List_functions_end:
=====================================================

\endcode *//*----------------------------------------

======================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // va_list va_arg


#include <gtk/gtk.h>

#include "../ut/ut_umem.h"                     // MemObj
#include "../ut/ut_umb.h"                  // UMB_pos__

#include "../gui/gui_types.h"                   // UI_Func
#include "../gui/gui_base.h"                  // GUI_obj_*

#include "../gui_gtk2/gtk_base.h"


// extern:
extern int       UI_fontsizX, UI_fontsizY;
extern GtkWidget *UI_MainWin;
extern GtkWidget *UI_act_wi;


// local:
static int       UI_act_Id;

  GtkStyle *UI_stylTab[3];                        // 0=default; 1=red; 2=blue
  GdkColor  colB1 = {0x0000, 0x0010, 0x0010, 0xc000};   // blue
  GdkColor  colB2 = {0x0000, 0xa000, 0xa000, 0xf000};   // blue-light
  GdkColor  colR = {0x0000, 0xc000, 0x1000, 0x1000};   // red
  //                        red     green   blue



//================================================================
  int GUI_key_set1 (int ikey) {
//================================================================
/// GUI_key_set1           change uncommon keys


  // translate some keys ..
  switch (ikey) {
    case 65434: // ff9a:
      ikey = GUI_KeyPageUp;
      break;
    case 65435: // ff9b:
      ikey = GUI_KeyPageDown;
      break;
  }
    // printf("GUI_gl_key key=%d ev=%d state=%d\n",ikey,typ,statMK);

  return ikey;

}


//================================================================
  int GUI_obj_focus (MemObj *mo) {
//================================================================
// GUI_obj_focus          set focus to widget

  Obj_Unknown *go;

  // printf("GUI_obj_focus FFFFFFFFFFFFF\n");

  go = GUI_obj_pos (mo);
  if(!go) return -1;


  gtk_widget_grab_focus (go->widget);

  return 0;

}


//================================================================
  int GUI_Win_exist (char *winTit) {
//================================================================
// returns nr of windows with windowTitle <winTit>;
// -1 = internal error.

  int     ii=0, iUmb=0, iStat;
  char    *cp1;
  void    *up;
  Obj_Win *ow;
  Memspc  *tm;

  // printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX \n");


  // test temporary block UI_tmpSpc
  tm = GUI_obj__ ();
  if(UME_get_used(tm) <= sizeof(int)) goto L_nxt;  // int 
    // UT3D_stru_dump (Typ_Memspc, tm, "UI_tmpSpc");


    // printf(" temp blockNr=%d\n",*((int*)tm->start));
  ow = (void*)((char*)tm->start + sizeof(int));

    if(ow->gio_typ == TYP_GUI_Win) {
      cp1 = GUI_Win_tit (ow->win);
      if(!strcmp (cp1, winTit)) ++ii;
        // printf("  winTit temp |%s|\n",cp1);

    } else {
      printf(" GUI_Win_exist E001\n");
    }


  //----------------------------------------------------------------
  // test all stored blocks ..
  L_nxt:
    iStat = UMB_stat__ (iUmb);
    if(iStat < 0) goto L_exit;

    // skip all not stored blocks
    if(iStat < 2) goto L_inc;


    up = UMB_pos__ (iUmb);
    if(!up) return -1;

    // skip first word (int) = blockNr
      // printf(" umbTab[%d] blockNr=%d\n",iUmb,*((int*)up));
    ow = (void*)((char*)up + sizeof(int));


    if(ow->gio_typ == TYP_GUI_Win) {
      cp1 = GUI_Win_tit (ow->win);
      if(!strcmp (cp1, winTit)) ++ii;
        // printf("  winTit block %d |%s|\n",iUmb,cp1);

    } else {
      printf(" GUI_Win_exist E002\n");
    }



    L_inc:
      ++iUmb;
      goto L_nxt;


  L_exit:
    // printf("ex GUI_Win_exist %d\n",ii);
  return ii;
}


//================================================================
  char* GUI_Win_tit (void *gtkWin) {
//================================================================
 
  return (char*)gtk_window_get_title (GTK_WINDOW(gtkWin));

}


//================================================================
  void GUI_Init_style (void *win1) {
//================================================================
/// INTERNAL


  GdkFont   *wFont;
  GtkStyle  *UI_style_act;


  //----------------------------------------------------------------
#ifdef gtk1
  // Style hat farben, Font GdkFont *font, BackgrndPixmap usw
  UI_style_act = gtk_widget_get_style (win1);
  // Breite/Hoehe eines einzelnen Char einlesen. = 7,21
  UI_fontsizX = gdk_char_width (UI_style_act->font, '0');
  UI_fontsizY = gdk_char_height (UI_style_act->font, '0');
#endif


  // Style hat farben, Font GdkFont *font, BackgrndPixmap usw
  UI_style_act = gtk_widget_get_style (win1);
  wFont = gtk_style_get_font (UI_style_act);
  // Breite/Hoehe eines einzelnen Char einlesen. = 7,21
  UI_fontsizX = gdk_char_width (wFont, '0');

  // UI_fontsizY = gdk_char_height (wFont, '0');
  // funktioniert in MS-Win leider nicht; liefert 21 statt 10
  // UI_fontsizY = wFont->ascent - wFont->descent;
  UI_fontsizY = wFont->ascent + wFont->descent;
  // UI_fontsizY = gdk_char_height (wFont, '0');

  UI_stylTab[0] = gtk_widget_get_default_style ();

  // 1=red
  UI_stylTab[1] = gtk_style_copy (UI_stylTab[0]);
  UI_stylTab[1]->fg[0] = colR; // normal
  // UI_stylTab[1]->fg[2] = colR; // normal
  UI_stylTab[1]->bg[0] = colR; // normal
  // UI_stylTab[1]->bg[2] = colR; // normal

  // 2=blue
    // printf(" sel colB\n");
  // colB.red = 0x1000; colB.green = 0x1000; colB.blue = 0xa000;
  UI_stylTab[2] = gtk_style_copy (UI_stylTab[0]);
  UI_stylTab[2]->fg[0] = colB1; // normal for labels only
  // UI_stylTab[2]->fg[1] = colB; // normal for ?
  // UI_stylTab[2]->fg[2] = colB; // normal for ?
  UI_stylTab[2]->bg[0] = colB2; // normal
  UI_stylTab[2]->bg[2] = colB2; // normal
  // UI_stylTab[2]->xthickness = 6;
  // UI_stylTab[2]->ythickness = 6;


    // printf("UI_fontsizX/Y=%d,%d\n",UI_fontsizX,UI_fontsizY);
    // printf("fontAsc=%d\n",wFont->ascent);
    // printf("fontDes=%d\n",wFont->descent);


}


//================================================================
  void GUI_Init__ (char *fRC) {
//================================================================
/// \code
/// GUI_Init__             must be 1. call ..
/// fRC                    UNUSED (style-info-file)
/// get local memSpc UI_tmpSpc & a new memBlockId <UI_umbId>
/// \endcode

  int    i1;


  // printf("GUI_Init__ |%s|\n",fRC);


  // gtk_set_locale ();
  gtk_disable_setlocale ();  // sonst Beistrich statt Decimalpunkt !! (LC_ALL)

  // gtk_init (&argc, &argv);
  i1 = 0;
  gtk_init (&i1, NULL);


/*
  // Die Styles (Farben, Fonts, Pixmaps ..) aus rc-Datei einlesen
  if (strlen(fRC) > 0) {
    gtk_rc_parse (fRC);
  }
*/

  //----------------------------------------------------------------
  // GUI_obj_init ();
  // UI_winID = 0;
  // UME_malloc (&UI_tmpSpc, 5000);
  // UI_winTab[UI_winID] = (void*)&UI_tmpSpc;


  GUI_ck_version (0, 0);

}


//================================================================
  void GUI_Win_kill (MemObj *mo) {
//================================================================
/// GUI_Win_kill           destroy window from its MemObj

// calls GUI_Win_exit

  void   *wo, *ww, *w1;


  // printf("GUI_Win_kill %d %d\n",mo->mbID,mo->ioff);
  // UMB_dump__ ();

  wo = GUI_obj_pos (mo);          // get gui-widget-object
  if(!wo) return;
  ww = ((Obj_Win*)wo)->win;       // get gtk-window


  // w1 = gtk_widget_get_toplevel (ww);
  // w1 = gtk_widget_get_ancestor (ww, GTK_TYPE_WINDOW);
  // if(w1 == UI_MainWin) exit (0);    // GUI_main_quit ?

// 2012-06-15 raus ! comes from GUI_DialogEntry !
  // if(mo->mbID == 0) exit (0);    // GUI_main_quit ?


  gtk_widget_destroy (ww);

  return;
}


//==============================================================================
  MemObj GUI_Win__ (char *wtit, void *funcnam, char *opts) {
//==============================================================================
/// \code
///
/// Input:
///   wtit      WindowTitle. or NULL = none
///   funcnam   callback from DestroyWindow
///   opts       options; (HorSiz,VertSiz)
///              HorSiz,VertSiz:  size in characters; default is automatic size.
///                               negative values: size in pixels.
///              Examples:
///                ""           size automatic
///                "100,20"     horiz. size 10 chars, vertical 20 lines
///                "-1000,-500" horiz. size 1000 pixels, vertical 500 pixels
///
/// funcnam prototype:
/// int funcnam (MemObj *mo, void **data);
///   // data=table of 1 pointers;
///   GUI_DATA_EVENT =*(int*)data[0]=TYP_EventExit
///   GUI_DATA_I1    =*(int*)data[1]=UI_FuncExit
///   GUI_OBJ_TYP(mo)  = TYP_GUI_Win
///
/// Examples without callback:
///  MemObj   win0, box0;
///  win0 = GUI_Win__ ("testWindow", NULL, 300, 100);
///  box0 = GUI_box__ (&win0, 1, 1, 1);
///  lbl1 = GUI_label__ (&box0, "Label..", -1, 5);
///  GUI_button__ (&box0, " Exit ", f_cb_exit, "but3", 0, 0);
///  GUI_Win_go (win0);
///
/// Examples with callback:
///  win0 = GUI_Win__ ("testWindow", w_cb_exit, 0, 0);
///  
///
///  int w_cb_exit (MemObj *mo, void **data) {
///    printf("w_cb_exit %d\n",GUI_DATA_EVENT); // is TYP_EventExit from exit
///                                             // or TYP_EventPress from button
///    if(GUI_DATA_EVENT == TYP_EventExit) {
///    if(GUI_DATA_I1 == UI_FuncKill) {
///      .. do cleanup ..
///
///
///  int  win0Id;
///  win0Id = GUI_Win_Id ();
///  if(GUI_is_alive(win0Id)) printf(" testWindow is alive\n");
///
/// \endcode



  int        isx, isy, iex, iey;
  GtkWidget  *win1;
  Obj_Win    *gWin;


  // printf("GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG \n");
  // printf("GUI_Win__ |%s|%s|\n",wtit,opts);


  // start new window
  UI_act_Id = GUI_obj_init ();
  if(UI_act_Id < 0) return (UME_obj_invalid_set (-4));
  

  // get address & memspc
  GUI_obj_spc ((void**)&gWin, sizeof(Obj_Win));
  if(!gWin) return (UME_obj_invalid_set (-1));


  win1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);


  // if(wtit) GUI_WinTit (mo, wtit);
  if(wtit) gtk_window_set_title (GTK_WINDOW(win1), wtit);


  // if (border > 0) {
    // gtk_container_set_border_width (GTK_CONTAINER (win1), border);
  // }


  g_signal_connect (win1, "destroy",
                    G_CALLBACK(GUI_Win_exit),
                    PTR_MEMOBJ(gWin->mem_obj));
  // get mem_obj out of gWin; get vp out of union memobj_union.


  //----------------------------------------------------------------
  // if((hsiz == 1)&&(vsiz == 1))  gtk_window_set_resizable (GTK_WINDOW(win1), 1);
  // else if((hsiz)||(vsiz)) GUI_siz_set (win1, hsiz, vsiz);

  // decode opts-string (HorSiz,VertSiz)  and a,e
  GUI_w_pack_d (&isx, &isy, &iex, &iey, opts);

  if((isx)||(isy)) GUI_siz_set (win1, isx, isy);

  //----------------------------------------------------------------
  // get fontsize UI_fontsizX and UI_fontsizY
  if(!UI_MainWin) {
    GUI_Init_style (win1);
    UI_MainWin = UI_act_wi;
  }


  gWin->gio_typ  = TYP_GUI_Win;
  gWin->win      = win1;
  if(funcnam) {
    gWin->uFunc = funcnam;
  } else {
    gWin->uFunc = NULL;
  }

  // printf(" win %p %hd %hd\n",gWin->win,gWin->gio_typ,gWin->gio_src);


  UI_act_wi = win1;

  // return ((void*)gWin);     // get active window id & baseAddress
  return (((Obj_Unknown*)gWin)->mem_obj);

}


//================================================================
  int GUI_Win_Id () {
//================================================================
/// \code
/// get id for active window;
/// must be called after GUI_Win__() and before GUI_Win_go().
/// Use id with GUI_is_alive()
/// \endcode

  return UI_act_Id;

}

//================================================================
  int GUI_Win_raise (MemObj *mo) {
//================================================================
/// \code
/// raise window
/// \endcode

// this function cannot inhibit deletion of window !


  Obj_Win  *go;


  // printf("GUI_Win_raise %d %d\n",mo->mbID,mo->ioff);

  // get gui-object from MemObj
  go = GUI_obj_pos (mo);
  if(!go) return FALSE;      // do defaultOperations

  gdk_window_raise (gtk_widget_get_window (go->win));

  return 0;

}


//================================================================
  int GUI_Win_exit (void *parent, MemObj mo) { 
//================================================================
/// \code
/// INTERNAL
/// callback from destroy window.
/// \endcode

// this function cannot inhibit deletion of window !


  short    i1;
  int      irc;
  int      iTyp=TYP_EventExit, iDat=UI_FuncExit;
  void     *pTab[2];
  Obj_Win  *go;


  // printf("GUI_Win_exit %d %d\n",mo.mbID,mo.ioff);

  // get gui-object from MemObj
  go = GUI_obj_pos (&mo);
  if(!go) return FALSE;      // do defaultOperations


  if(go->uFunc) {
    pTab[0] = &iTyp;   // TYP_EventExit
    pTab[1] = &iDat;   // UI_FuncExit
    
    // call userfunction
    go->uFunc (&mo, pTab);   // int uFunc 
    // uFunc = go->uFunc;
    // uFunc (&mo, pTab);
    // if(irc < 0) return (TRUE); // skip defaultOperations
  }



  // if this is the main-window: end prog.
  if(go->win == UI_MainWin) {
      // printf(" GUI_Win_exit: exit ..\n");

    // TODO: free all memblocks
    GUI_obj_destroy (go);
    GUI_main_quit ();


  } else {
    GUI_obj_destroy (go);
  }


  return FALSE;      // do defaultOperations

}


//================================================================
  int GUI_main_quit () {
//================================================================
/// GUI_main_quit          close application

  // printf("GUI_main_quit \n");

  exit(0);
  // gtk_main_quit();

}


//=========================================================================
  int GUI_WinTit (MemObj *mo, char *wtit) {
//=========================================================================
/// GUI_WinTit             set titletext

  Obj_Win  *go;

    
  // printf("GUI_WinTit |%s|\n",wtit);

  // get gui-object from MemObj
  go = GUI_obj_pos (mo);
  if(!go) return FALSE;      // do defaultOperations
    

  gtk_window_set_title (GTK_WINDOW (go->win), wtit);

  return 0;

}


//================================================================
  int GUI_Win_ev_key (MemObj *o_par, void *fKey) {
//================================================================
/// \code
/// add callback for the key-events
/// Input:
///   o_par      parentWindow
///   fKey       callback-function key-events
///
/// prototyp fKey:
/// int fKey (void *parent, void **data);
///   // data=table of pointers to 4 ints;
///   // data[0]=(int*)=type of event; TYP_EventPress|TYP_EventRelease
///   // data[1]=(int*)=device; TYP_DeviceKeyb
///   // data[2]=(int*)=keyvalue; eg 'a'
///   // data[3]=(int*)=state of modifierkeys; &1=shift; &4=ctrl; &8=alt.
///
///   printf(" key=%d\n",*(int*)data[2]);
///
/// Example see GUI_entry__
/// \endcode


  int     pTyp;
  Obj_Win *g_win;


  // printf("GUI_Win_ev_key \n");

  GUI_obj_typ (&pTyp, (void**)&g_win, o_par);
  if(!pTyp) return -1;

  gtk_widget_add_events (GTK_WIDGET(g_win->win),
                         GDK_ENTER_NOTIFY_MASK |
                         GDK_KEY_PRESS_MASK |
                         GDK_KEY_RELEASE_MASK);

  // g_signal_connect (g_win->win, "enter-notify-event",
                    // G_CALLBACK(gtk_widget_grab_focus), (void*)TRUE);

  g_signal_connect (g_win->win, "key-press-event",
                    G_CALLBACK(GUI_Win_key), fKey);

  g_signal_connect (g_win->win, "key-release-event",
                    G_CALLBACK(GUI_Win_key), fKey);

  return 0;

}


//================================================================
  int GUI_Win_key (void *parent, void *event, void *data) {
//================================================================
/// \code
/// INTERNAL
/// key_press_event: GdkEventKey GDK_KEY_PRESS or GDK_KEY_RELEASE
/// \endcode


  int      typ, iTyp, ikey, imod;
  void     *pTab[3];
  void         (*uFunc)();


  // printf("GUI_Win_key \n");

  typ  = ((GdkEventAny*)event)->type;

  if(typ == GDK_KEY_PRESS) {
    iTyp = TYP_EventPress;
  } else if(typ == GDK_KEY_RELEASE) {
    iTyp = TYP_EventRelease;
  } else return FALSE;  // TRUE=1   FALSE=0



  ikey = ((GdkEventKey*)event)->keyval;
  imod = ((GdkEventKey*)event)->state;
    // printf("GUI_Win_key key=%d ev=%d state=%d\n",ikey,typ,imod);

  pTab[0] = &iTyp;
  pTab[1] = &ikey;
  pTab[2] = &imod;

  uFunc = data;
  uFunc (parent, pTab);
  // UI_uf_key (parent, pTab);

  return FALSE;  // TRUE=1=stop further handling of event ..

}


//================================================================
  int GUI_Win_ev_button (MemObj *o_par, void *fButton) {
//================================================================
/// \code
/// add callback for the mouse-button event
/// Input:
///   o_par      parentWindow
///   fButton    callback-function mouse-button event
///
/// prototype:
/// int fButton  (void *parent, void **data);
///   // data=table of pointers to 3 ints;
///   // data[0]=(int*)=type of event; TYP_EventPress|TYP_EventRelease
///   // data[1]=(int*)=device; TYP_DeviceMouse
///   // data[2]=(int*)=button; GUI_MouseL|GUI_MouseM|GUI_MouseR - left,mid,right
///                             GUI_MouseScUp|GUI_MouseScDown - scroll up/down
///
///   printf(" button=%d\n",*(int*)data[2]);
///
/// \endcode

  int     pTyp;
  Obj_Win *g_win;


  // printf("GUI_Win_ev_button \n");

  GUI_obj_typ (&pTyp, (void**)&g_win, o_par);
  if(!pTyp) return -1;


  gtk_widget_add_events (GTK_WIDGET(g_win->win),
                         GDK_SCROLL_MASK |
                         GDK_BUTTON_PRESS_MASK |
                         GDK_BUTTON_RELEASE_MASK);

  g_signal_connect (g_win->win, "button_press_event",
                    G_CALLBACK(GUI_Win_button), fButton);

  g_signal_connect (g_win->win, "button_release_event",
                    G_CALLBACK(GUI_Win_button), fButton);

  g_signal_connect (g_win->win, "scroll_event",
                    G_CALLBACK(GUI_Win_button), fButton);

  // UI_uf_button = fButton;

  return 0;

}


//================================================================
  int GUI_Win_button (void *parent, void *ev, void *data) {
//================================================================
/// \code
/// INTERNAL
/// button_press_event: GdkEventButton
/// GDK_BUTTON_PRESS GDK_BUTTON_RELEASE GDK_SCROLL
/// \endcode


  int      typ, ikey, iTyp;
  void     *pTab[2];
  void         (*uFunc)();


  // printf("GUI_Win_button \n");


  typ  = ((GdkEventAny*)ev)->type;

  if(typ == GDK_BUTTON_PRESS) {
    iTyp = TYP_EventPress;
    ikey = ((GdkEventButton*)ev)->button;

  } else if(typ == GDK_BUTTON_RELEASE) {
    iTyp = TYP_EventRelease;
    ikey = ((GdkEventButton*)ev)->button;

  } else if(typ == GDK_SCROLL) {
    iTyp = TYP_EventPress;
    if(((GdkEventScroll*)ev)->direction == GDK_SCROLL_UP) ikey = GUI_MouseScUp;
    else                                                  iTyp = GUI_MouseScDown;

  } else return FALSE;  // TRUE=1   FALSE=0

    // printf("GUI_Win_button typ=%d but=%d\n",typ,ikey);


  pTab[0] = &iTyp;
  pTab[1] = &ikey;

  uFunc = data;
  uFunc (parent, pTab);
  // UI_uf_button (parent, pTab);

  return FALSE;  // TRUE=1=stop further handling of event ..

}


//================================================================
  int GUI_Win_go (MemObj *o_par) {
//================================================================
/// \code
/// GUI_Win_go                 windowSetup finished; display it ..
///
/// If this window is the first window, this function does not return.
/// \endcode


  int     pTyp;
  Obj_Win *g_win;


  // printf("GUI_Win_go \n");


  GUI_obj_typ (&pTyp, (void**)&g_win, o_par);
  if(!pTyp) return (-1);
  

  gtk_window_set_position (GTK_WINDOW(g_win->win), GTK_WIN_POS_MOUSE);

  gtk_widget_show (g_win->win);


  if(!UI_MainWin) {
      // printf(">>>>>>>>>>>>>>>> set UI_MainWin <<<<<<<<<<<<\n");
    UI_MainWin = g_win->win;
    gtk_main ();
  }

  return 0;

}



//=====================================================================
  void GUI_Tip (char* txt) {
//=====================================================================
/// \code
/// add infotext (tooltip) to last created widget.
/// \n starts new line.
/// \endcode

// Buch S.111

  // GtkTooltips *UI_tip;

  // printf("GUI_Tip |%s|\n",txt);


  gtk_widget_set_tooltip_text (UI_act_wi, txt);

// Gtk2:
  // // tip = gtk_tooltips_new();
  // gtk_tooltips_set_tip (UI_tip, UI_act_wi, txt, NULL);

}

//================================================================
  int GUI_is_win_alive (void *win) {
//================================================================
// test if window is alive from window-widget
// 0=no, >0=yes
 
  int         ii = 0;
  GList       *tmplist;

  if(win == NULL) return ii;

  // win = gtk_widget_get_toplevel (UI_ListWin);
  // if (gtk_widget_is_toplevel (toplevel))
  // gtk_widget_get_ancestor (widget, GTK_TYPE_WINDOW)
  // if(gtk_widget_is_drawable (w1) == FALSE) return 0;

  tmplist = gtk_window_list_toplevels();

  g_list_foreach (tmplist, (GFunc)g_object_ref, NULL);
  tmplist = g_list_first(tmplist);

  while (tmplist) {
    if (tmplist->data == win) {
      ++ii;
    }
    tmplist = g_list_next(tmplist);
  }

  // clear list ..
  g_list_foreach (tmplist, (GFunc)g_object_unref, NULL);

    // printf("GUI_is_alive %d\n",ii);

  return ii;

}


//================================================================
  int GUI_is_alive (int winId) {
//================================================================
/// \code
/// test if window mo is active
/// get winId with GUI_Win_Id() on init
/// 0=no, 1=yes
/// \endcode


  int         ii = 0;

  // printf("GUI_is_alive %d\n",winId);

  ii = UMB_is_alive (winId);


/*
  GList       *tmplist;
  void        *win;
  Obj_Unknown *go;


  GUI_obj_dump_mo (mo);


  go = GUI_obj_pos (mo);
  if(!go) return 0;
  win = go->widget;

  if(!win) return ii;

  // win = gtk_widget_get_toplevel (UI_ListWin);
  // if (gtk_widget_is_toplevel (toplevel))
  // gtk_widget_get_ancestor (widget, GTK_TYPE_WINDOW)
  // if(gtk_widget_is_drawable (w1) == FALSE) return 0;

  tmplist = gtk_window_list_toplevels();

  g_list_foreach(tmplist, (GFunc)g_object_ref, NULL);
  tmplist = g_list_first(tmplist);

  while (tmplist) {
    if (tmplist->data == win) {
      ++ii;
    }
    tmplist = g_list_next(tmplist);
  }

  g_list_foreach (tmplist, (GFunc)g_object_unref, NULL);
*/

    // printf("GUI_is_alive %d winId=%d\n",ii,winId);
    // UMB_dump__ ();

  return ii;

}


//=====================================================================
  int GUI_Win_up (MemObj *parentWin, MemObj *o_win, int typ) {
//=====================================================================
/// \code
/// window always on top ..
/// Input:
///   parentWin       NULL for top-level-parentwindow
///   o_win           window to be kept on top of parentWin
///   typ             0=normal, 1=modal (block all other windows)
/// 
/// DOES NOT WORK FOR THE PARENT-WINDOW: USE WM_raise_wTit
/// \endcode


  int     pTyp;
  Obj_Win *win1, *winp;
  void    *baseWin;

  // printf("GUI_Win_up \n");


  GUI_obj_typ (&pTyp, (void**)&win1, o_win);
  if(!pTyp) return (-1);

  
  if(!parentWin) {
    baseWin = UI_MainWin;
  } else {
    GUI_obj_typ (&pTyp, (void**)&winp, parentWin);
    baseWin = winp->win;
  }

  gtk_window_set_transient_for (GTK_WINDOW(win1->win), GTK_WINDOW(baseWin));

  if(typ) {
    gtk_window_set_modal (GTK_WINDOW (win1->win), TRUE);
  }

  return 0;

}


//================================================================
  int GUI_update__ () {
//================================================================
/// update all windows

// Achtung: löscht events !

  // printf("GUI_update__ \n");


  // Display zwischendurch updaten
  while (gtk_events_pending()) {
    gtk_main_iteration();
  }

  return 0;

}


//================================================================
  int GUI_idle__ (void *funcnam, void *data) {
//================================================================
/// \code
/// GUI_Idle               register a idle-function
/// Example:
///  GUI_idle__ (AP_Idle_CB, NULL);
///  int AP_Idle_CB (void *data) {
///    .. work ..
///    return 0; // removes idle-Call!
///    return 1; // keep idle-Call; call AP_Idle_CB again.
///  }
/// \endcode

  // g_idle_add (funcnam, data);          // 2012-03-27
  gdk_threads_add_idle (funcnam, data);   // 2012-07-03

  return 0;

}


//================================================================
  int GUI_timer__ (void *funcnam, int interval, void *data) {
//================================================================
/// \code
/// GUI_timer__            register a timer-function
/// interval        time in ms
///
///  (int) irc = funcnam ((void*)data) {..
///    return 0; // removes timer
///    return 1; // keep timer; call funcnam again.
/// \endcode


  // int    timerID;


  return g_timeout_add (interval, funcnam, data);

}


//================================================================
  int GUI_packwn (void *parent, void *wi, int hsiz, int vsiz) {
//================================================================
/// \code
/// INTERNAL
/// GUI_packwn             pack widget normal
/// Input:
///   parent     parentBox
///   hsiz       0,0  minimum size (size of caption)
///              0,1  maximum size (expand inside container)
///              >1  prepare size for <siz> nr of characters
///              <0  absolute size (in pixels)
/// \endcode


  int  ihs=0, ivs=0;

  // printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
  // printf("GUI_packwn %d %d\n",hsiz,vsiz);


  // 0 = minimum size
  if(hsiz == 0) {
    if(vsiz == 0) {
      gtk_box_pack_start (GTK_BOX (parent), wi, FALSE, FALSE, 0);
      goto L_done;


    // 1=maximum size
    } else if(vsiz == 1) {
      gtk_box_pack_start (GTK_BOX (parent), wi, TRUE, TRUE, 0);
      goto L_done;
    }

    // hsiz=0, vsiz not 0, not 1;
    // eg 0,10    (default-size in X, fixed size in Y
    hsiz = UI_fontsizX;  // minimum-size;
    // GUI_siz_eval (&ihs, &ivs, hsiz, vsiz);
    // GUI_siz_set (wi, ihs, ivs);  // set size
    // goto L_done;
  }



  // fixed size (< 0 || > 1)    
  GUI_siz_eval (&ihs, &ivs, hsiz, vsiz);

  gtk_box_pack_start (GTK_BOX (parent), wi, FALSE, FALSE, 0);

  GUI_siz_set (wi, ihs, ivs);  // set size


  L_done:
  gtk_widget_show (wi);


  return 0;

}


//================================================================
  int GUI_siz_eval (int *ihs, int *ivs, int hsiz, int vsiz) {
//================================================================
/// \code
/// INTERNAL
/// RetCod: 0 =  0,0  minimum size (size of caption)
///         1 =  0,1  maximum size (expand inside container)
///         2 =  ...  absolute size
/// \endcode



  if(hsiz == 0) {
    if(vsiz == 0) return 0;
    return 1;
  }



  if(hsiz < 0) {   // in pixels
    *ihs = -hsiz;
  } else {
    *ihs = hsiz * UI_fontsizX;
    *ihs *= 1.5;  // Ubu 11.10 Gtk2 not Gtk3
  }

  if(vsiz < 0) {   // in pixels
    *ivs = -vsiz;
  } else {
    ++vsiz;   // extraspace above below
    *ivs = vsiz * UI_fontsizY;
  }


  // printf("ex GUI_siz_eval %d %d\n",*ihs,*ivs);

  return 2;

}


//================================================================
  int GUI_siz_set (void *parent, int hSiz, int vSiz) {
//================================================================
/// \code
/// INTERNAL
/// set size of widget size in pixels x,y.
/// Input:
///   parent     parentBox
///   .Siz       0 = do not modify
/// \endcode

// gtk_window_set_default_size


  // printf("GUI_siz_set %d %d\n",hSiz,vSiz);

  // if(hSiz < 0) { TX_Error("GUI_siz_set %d",hSiz); return -1;}
  if(hSiz > 4000) { TX_Error("GUI_siz_set %d",hSiz); return -1;}

  // if(vSiz < 0) { TX_Error("GUI_siz_set %d",vSiz); return -1;}
  if(vSiz > 4000) { TX_Error("GUI_siz_set %d",vSiz); return -1;}

  // set minimum size;  0,0 = as small as possible;
  // -1 = unset
  if(hSiz < 1) hSiz = -1;
  if(vSiz < 1) vSiz = -1;
  gtk_widget_set_size_request (GTK_WIDGET(parent), hSiz, vSiz);
    // printf(" _set_size_request %d %d\n",hSiz, vSiz);


  return 0;

}


//================================================================
  MemObj GUI_box_h (MemObj *o_par, char *opts) {
//================================================================
/// \code
/// GUI_box_h              horizontal container for widgets
///   opts       options; (HorSiz VertSiz)
///              HorSiz,VertSiz:  size in characters; default is automatic size.
///                              'e' = expand widget; default is fixed size.
///                               negative values: size in pixels.
///              Examples: "" or "10" or "10e,e"
///                "10e,e" horiz. size 10 chars, hor. and vert. expandable.
/// \endcode


  return GUI_box__  (o_par, 0, opts);

}


//================================================================
  MemObj GUI_box_v (MemObj *o_par, char *opts) {
//================================================================
/// \code
/// GUI_box_v              vertical container for widgets
///   opts       options; (HorSiz VertSiz)
///              HorSiz,VertSiz:  size in characters; default is automatic size.
///                              'e' = expand widget; default is fixed size.
///                               negative values: size in pixels.
///              Examples: "" or "10" or "10e,e"
///                "10e,e" horiz. size 10 chars, hor. and vert. expandable.
/// \endcode

  return GUI_box__  (o_par, 1, opts);

}


//================================================================
  MemObj GUI_box__ (MemObj *o_par, int typ, char *opts) {
//================================================================
/// \code
/// GUI_box__              container for widgets 
/// Input:
///   o_par      must be a window or a box
///   typ        0=horizontal, 1=vertical
///   opts       options; (HorSiz VertSiz)
///              HorSiz,VertSiz:  size in characters; default is automatic size.
///                              'e' = expand widget; default is fixed size.
///                               negative values: size in pixels.
///              Examples: "" or "10" or "10e,e"
///                "10e,e" horiz. size 10 chars, hor. and vert. expandable.
/// \endcode


  int       irc, pTyp, ie, isx, isy, iex, iey;
  GtkWidget *box1;
  Obj_Box   *gBox;
  void      *w_par, *g_par;


  // printf("GUI_box__ %d |%s|\n",typ,opts);


  // get parent-typ & parent-obj
  GUI_obj_typ (&pTyp, &g_par, o_par);
  if(!pTyp) return (UME_obj_invalid_set (-3));
    // printf(" pTyp=%d\n",pTyp);


  if(pTyp == TYP_GUI_Win) {
    w_par = ((Obj_Win*)g_par)->win;
    goto L_get_spc;
  }

  if((pTyp == TYP_GUI_BoxH)    ||
     (pTyp == TYP_GUI_BoxV)    ||
     (pTyp == TYP_GUI_Toolbar) ||
     (pTyp == TYP_GUI_Frame))      {
    w_par = ((Obj_Box*)g_par)->box;
    goto L_get_spc;
  }

  printf("***** GUI_box__ parent must be window or box, not %d\n",pTyp);
  return (UME_obj_invalid_set (-3));


  //----------------------------------------------------------------
  L_get_spc:
      // printf(" w_par = %p\n",w_par);
  // get memspc for new widget
  GUI_obj_spc ((void**)&gBox, sizeof(Obj_Box));
  if(!gBox) return (UME_obj_invalid_set (-1));




  //----------------------------------------------------------------
  // decode size
  irc = GUI_w_pack_d (&isx, &isy, &iex, &iey, opts);


  //----------------------------------------------------------------
  // create box; TRUE=1=expand childrens in box; FALSE=0=minSpace for childs.
  // if(sTyp == 1)
    // ie = TRUE;
  // else
    ie = FALSE;
  if(typ == 0) {
    box1 = gtk_hbox_new (ie, 0);
    gBox->gio_typ    = TYP_GUI_BoxH;
  } else {
    box1 = gtk_vbox_new (ie, 0);
    gBox->gio_typ    = TYP_GUI_BoxV;
  }


  //----------------------------------------------------------------
  // pack & size
  if((pTyp == TYP_GUI_Win)         ||
     (pTyp == TYP_GUI_Frame)       ||
     (pTyp == TYP_GUI_Toolbar)) {
    // pack widget into window|frame|toolbar
    irc = GUI_w_pack_w (pTyp, w_par, box1, &isx, &isy, &iex, &iey);

  } else {
    // pack widget into box (not if parent is a window|frame|toolbar)
    irc = GUI_w_pack_b (pTyp, w_par, box1, &isx, &isy, &iex, &iey);
  }



  //----------------------------------------------------------------
  L_done:
  gBox->box      = box1;

    // printf("ex GUI_box__\n");

  // return ((void*)gBox);
  return (((Obj_Unknown*)gBox)->mem_obj);

}


//======================================================================
  int GUI_w_pack_d (int *isx, int *isy, int *iex, int *iey, char *siz) {
//======================================================================
// decode size-string;
// Examples:  "ae,a" or "50,a" or "50e,a" or "e" or ""
/// Input:
///   siz
///        automaticSize; expand;            ae
///        automaticSize; do not expand;     a
///        defined size, expandable          5e
///        defined size, do not expand.      5
/// Output:
///   isx    hor.size;     0=automatic; else size in pixels
///   iex    hor.expand;   0=expand=yes,   1=do not expand
///   isy    vert.size;    0=automatic; else size in pixels
///   iey    vert.expand;  0=expand=yes,   1=do not expand

// see also GUI_opts_get1


  int   iPos=0;
  char  *p1;


  // printf("GUI_w_pack_d |%s|\n",siz);

  *isx = 0;
  *iex = 1;

  *isy = 0;
  *iey = 1;

  if(!siz) { TX_Error("GUI_w_pack_d E001"); return -1; }

  if(strlen(&siz[iPos]) < 1) {
    goto L_exit;
  }

  // get x-siz
  if(UTX_ck_num_i (&siz[iPos]) > 0) {
    *isx = strtol (&siz[iPos], &p1, 10);
    iPos = p1 - siz;
  }

  if(siz[iPos] == 'e') {
    ++iPos;
    *iex = 0;
  } else if(siz[iPos] == 'a') {
    ++iPos;
  }


  //----------------------------------------------------------------
    // printf(" vor ySiz |%s|\n",&siz[iPos]);
  if(strlen(&siz[iPos]) < 1) {
    goto L_exit;
  }
  ++iPos;   // skip ','

  // get y-siz
  if(UTX_ck_num_i (&siz[iPos]) > 0) {
    *isy = strtol (&siz[iPos], &p1, 10);
    iPos = p1 - siz;
  }

  if(siz[iPos] == 'e') {
    // ++iPos;
    *iey = 0;
  }


  //----------------------------------------------------------------
  L_exit:
  // scale to size of characters
    // printf("  scale: sx=%d sy=%d\n",*isx,*isy);

  if(*isx > 0) {
    *isx *= UI_fontsizX;
    *isx *= 1.5;  // Ubu 11.10 Gtk2 not Gtk3
  } else if(*isy < 0) {
    *isx *= -1;
  }


  if(*isy > 0) {
    *isy += 1;   // extraspace above below
    *isy *= UI_fontsizY;
  } else if(*isy < 0) {
    *isy *= -1;
  }

    // printf("ex GUI_w_pack_d sx=%d sy=%d ex=%d ey=%d\n",*isx,*isy,*iex,*iey);

  return 0;

}


//================================================================
  int GUI_w_pack2 (int pTyp, void *pObj, void *cObj, char *opts) {
//================================================================
// pack widget (cObj) into parent(box) pObj
// pTyp must be box !
 
  int     isx, isy, iex, iey;

  // printf("GUI_w_pack2 %d\n",pTyp);


  // decode opts-string (HorSiz,VertSiz)  and a,e
  GUI_w_pack_d (&isx, &isy, &iex, &iey, opts);

  GUI_w_pack_b (pTyp, pObj, cObj, &isx, &isy, &iex, &iey);

  return 0;

}


//================================================================
  int GUI_w_pack1 (int pTyp, void *pObj, void *cObj, char *opts) {
//================================================================
// pack widget (cObj) into parent(box) pObj
// pTyp must be box !
// Entry without label.

  int     isx, isy, iex, iey;

  // printf("GUI_w_pack1 %d\n",pTyp);

  // decode opts-string (HorSiz,VertSiz)  and a,e
  GUI_w_pack_d (&isx, &isy, &iex, &iey, opts);

  GUI_w_pack_w (pTyp, pObj, cObj, &isx, &isy, &iex, &iey);

  return 0;

}


//================================================================
  int GUI_w_pack_w (int pTyp, void *pObj, void *cObj,
                    int *isx, int *isy, int *iex, int *iey) {
//================================================================
/// pack widget=box into window|frame|toolbar
/// gtk_box_pack_start does not work with parent = window|frame|toolbar
/// ONLY Gtk2

//   iex    hor.expand;   0=expand=yes,   1=do not expand
//   iey    vert.expand;  0=expand=yes,   1=do not expand

// mit gtk_container_add wird Box resizeable
// mit gtk_box_pack_start behaelt Box fixe size !!

// macht widgets in box gleich gross gtk_box_set_homogeneous

// erst mit Gtk3:    gtk_widget_set_hexpand, gtk_widget_set_vexpand

  int     i1, i2;

  // printf("GUI_w_pack_w typ=%d sx=%d sy=%d ex=%d ey%d\n",pTyp,
         // *isx,*isy,*iex,*iey);


  gtk_container_add (GTK_CONTAINER (pObj), cObj);

/*
  // gtk_box_set_homogeneous=TRUE: allow expand of the childs 
  if((*iex == 0)||(*iey == 0)) {
    // gtk_box_set_homogeneous (cObj, TRUE);
      printf(" _set_homogeneous\n");
    gtk_box_set_child_packing (pObj,cObj,TRUE,TRUE,0,GTK_PACK_START);
  }
*/

  //----------------------------------------------------------------
  // set size of child
  if((*isx > 0)||(*isy > 0)) 
    GUI_siz_set (cObj, *isx, *isy);


  gtk_widget_show (cObj);

  return 0;

}


//================================================================
  int GUI_w_pack_b (int pTyp, void *pObj, void *cObj,
                    int *isx, int *isy, int *iex, int *iey) {
//================================================================
/// pack widget into box (not if parent is a window|frame|toolbar)
/// ONLY Gtk2


// mit gtk_container_add wird Box resizeable
// mit gtk_box_pack_start behaelt Box fixe size !!

// macht widgets in box gleich gross gtk_box_set_homogeneous

// erst mit Gtk3:    gtk_widget_set_hexpand, gtk_widget_set_vexpand

  int     i1, i2;


  // printf("GUI_w_pack_b typ=%d sx=%d sy=%d ex=%d ey%d\n",pTyp,
         // *isx,*isy,*iex,*iey);

  //   iex    hor.expand;   0=expand=yes,   1=do not expand
  //   iey    vert.expand;  0=expand=yes,   1=do not expand
  // expand,fill:
  // FALSE,FALSE: minimum size, pack left.
  // TRUE,FALSE: minimum size (do not expand widget), but center in box.
  // TRUE,TRUE: expand widget in box.
  i1 = 0; i2 = 0;
  // if((*iex == 0)||(*iey == 0)) { i1 = 1; i2 = 1; } // cannot expand in y-only
  // check only expand in y; 2013-09-05
  if(*iey == 0) { i1 = 1; i2 = 1; } // cannot expand in y-only
  gtk_box_pack_start (GTK_BOX (pObj), cObj, i1, i2, 0);
    // printf(" _pack_start %d %d\n",i1,i2);


  //----------------------------------------------------------------
  // gtk_box_set_homogeneous=TRUE: allow expand of the childs 
  //if((*iex == 0)||(*iey == 0)) {
  // check only expand in y; 2013-09-05
  if(*iey == 0) {
    // gtk_box_set_homogeneous (cObj, TRUE);
      // printf(" _set_homogeneous\n");
    gtk_box_set_child_packing (pObj,cObj,TRUE,TRUE,0,GTK_PACK_START);
  }


  // set size of child
  if((*isx > 0)||(*isy > 0)) 
    GUI_siz_set (cObj, *isx, *isy);

  gtk_widget_show (cObj);

  return 0;

}
 
/*
//===========================================================================
  MemObj GUI_box__ (MemObj *o_par, int typ, int hsiz, int vsiz) {
//===========================================================================
/// \code
/// GUI_box__              container for widgets 
/// Input:
///   o_par      must be a window or a box
///   typ        0=horizontal, 1=vertical
///   hsiz,vsiz  0,0  minimum size (size of caption)
///              0,1  maximum size (expand inside container hor. and vert.)
///              >1   prepare size for <siz> nr of characters
///              <0   absolute size (in pixels)
///
/// see also GUI_box_h|GUI_box_v ((MemObj *o_par, int hsiz, int vsiz);
/// \endcode


  int       pTyp, sTyp, ihs, ivs, ix;
  GtkWidget *box1;
  Obj_Box   *gBox;
  void      *w_par, *g_par;


  printf("GUI_box__ %d %d %d\n",typ,hsiz,vsiz);


  // get parent-typ & parent-obj
  GUI_obj_typ (&pTyp, &g_par, o_par);
  if(!pTyp) return (UME_obj_invalid_set (-3));
    printf(" pTyp=%d\n",pTyp);


  if(pTyp == TYP_GUI_Win) {
    w_par = ((Obj_Win*)g_par)->win;
    goto L_get_spc;
  }

  if((pTyp == TYP_GUI_BoxH)    ||
     (pTyp == TYP_GUI_BoxV)    ||
     (pTyp == TYP_GUI_Toolbar) ||
     (pTyp == TYP_GUI_Frame))      {
    w_par = ((Obj_Box*)g_par)->box;
    goto L_get_spc;
  }

  printf("***** GUI_box__ parent must be window or box, not %d\n",pTyp);
  return (UME_obj_invalid_set (-3));


  //----------------------------------------------------------------
  L_get_spc:
      // printf(" w_par = %p\n",w_par);
  // get memspc for new widget
  GUI_obj_spc ((void**)&gBox, sizeof(Obj_Box));
  if(!gBox) return (UME_obj_invalid_set (-1));


  sTyp = GUI_siz_eval (&ihs, &ivs, hsiz, vsiz);


  // create widget
  if(sTyp == 1) ix = TRUE;
  else          ix = FALSE;
  if(typ == 0) {
    box1 = gtk_hbox_new (ix, 0);
    gBox->gio_typ    = TYP_GUI_BoxH;
  } else {
    box1 = gtk_vbox_new (ix, 0);
    gBox->gio_typ    = TYP_GUI_BoxV;
  }


  //----------------------------------------------------------------
  // pack widget into window
    // gtk_box_pack_start does not work with parent = window | frame
  if((pTyp != TYP_GUI_Win)     &&
     (pTyp != TYP_GUI_Toolbar) &&
     (pTyp != TYP_GUI_Frame))      goto L_pack_win;

    gtk_container_add (GTK_CONTAINER (w_par), box1);

    if(sTyp == 2) GUI_siz_set (box1, ihs, ivs);
    gtk_widget_show (box1);

    goto L_done;


  //----------------------------------------------------------------
  // pack widget into box
  L_pack_win:
    GUI_packwn (w_par, box1, hsiz, vsiz);


  //----------------------------------------------------------------
  L_done:
  gBox->box      = box1;

    printf("ex GUI_box__\n");

  // return ((void*)gBox);
  return (((Obj_Unknown*)gBox)->mem_obj);


}
*/

//================================================================
  void GUI_spc__ (MemObj *o_par, int typ, int spc) {
//================================================================
/// \code
/// GUI_spc_h             make a horizontal|vertical (free) space
/// Input:
///   o_par      parentBox
///   typ        0=horizontal, 1=vertical
///   spc        size in pixels
/// \endcode

  GtkWidget  *box0;
  void       *w_par;


  // get gtk-widget for parentBox from MemObj
  w_par = GUI_obj_parentBox (o_par);
  if(!w_par) return;



  if(typ == 0) {
    box0 = gtk_hbox_new (FALSE, 0);
  } else {
    box0 = gtk_vbox_new (FALSE, 0);
  }
  gtk_box_pack_start (GTK_BOX (w_par),box0, FALSE, FALSE, spc);
  gtk_widget_show (box0);


  return;

}


//=====================================================================
  void GUI_sep__ (MemObj *o_par, int typ, int border) {
//=====================================================================
/// \code
/// create Vertical. separationline
/// Input:
///   o_par      parentBox
///   typ        0=horizontal, 1=vertical
///   border     distance left / right of the separationline.
/// \endcode


  GtkWidget  *w1;
  void       *w_par;


  // printf("GUI_sep__ %d %d\n",typ,border);


  // get gtk-widget for parentBox from MemObj
  w_par = GUI_obj_parentBox (o_par);
  if(!w_par) return;


  if(typ == 0) {
    w1 = gtk_hseparator_new ();
  } else {
    w1 = gtk_vseparator_new ();
  }


  // gtk_box_pack_start (GTK_BOX (parent), w1, TRUE, TRUE, border);
  gtk_box_pack_start (GTK_BOX (w_par), w1, FALSE, TRUE, border);
  // gtk_box_pack_start (GTK_CONTAINER (parent), w1, FALSE, TRUE, border);
  gtk_widget_show (w1);

  return;

}

//================================================================
  int GUI_set_enable (MemObj *mo, int mode) {
//================================================================
/// \code
/// GUI_set_enable   makes widgets / groups of widgets unselectable (disactivate)
///   mode        1=TRUE=active; 0=FALSE=unpickable, inactive; 2=query state
///   RetCode     for mode=2: state; 0=inactive, 1=active
/// \endcode


  Obj_Unknown *go;

  // printf("GUI_set_enable %d\n",mode);

  go = GUI_obj_pos (mo);
  if(!go) return 0;

  if(mode == 2) {
    // return gtk_widget_get_sensitive (go->widget);    // Gtk-2.?
    return gtk_widget_is_sensitive (go->widget);    // Gtk-2.24
    // return GTK_WIDGET_SENSITIVE (go->widget);    // Gtk-2.16
    

  }

  gtk_widget_set_sensitive (go->widget, mode);

  return 0;

}


//================================================================
  int GUI_set_show (MemObj *mo, int mode) {
//================================================================
/// \code
/// GUI_set_show      show/hide object
///   mode        0=hide obj, 1=show obj, 2=query state
///   RetCode     for mode=2: state; 0=hidden, 1=visible
/// \endcode

  Obj_Unknown *go;
  void        *wo;

  // printf("GUI_set_show %d\n",mode);

  go = GUI_obj_pos (mo);
  if(!go) return 0;
  wo = go->widget;


  if(mode == 0) gtk_widget_hide (wo);
  else if(mode == 1) gtk_widget_show (wo);
  else if(mode == 2) return gtk_widget_is_drawable (wo);  // Gtk-2.24
  // else if(mode == 2) return GTK_WIDGET_DRAWABLE (wo);        // Gtk-2.16

  return 0;

}


//================================================================
  int GUI_get_keys_mod () {
//================================================================
/// \code
/// GUI_get_keys_mod        get state of modifier-keys
///   (Mousebuttons, Shift, Ctrl, Alt);
///
///   int i1 = GUI_get_keys_mod ();
///   if(i1 & 1) printf(" shift is on\n");
///   if(i1 & 4) printf(" ctrl is on\n");
///   if(i1 & 8) printf(" alt  is on\n");
///   if(i1 & 256)  printf(" mouseButt. 1 is on\n");
///   if(i1 & 512)  printf(" mouseButt. 2 is on\n");
///   if(i1 & 1024) printf(" mouseButt. 3 is on\n");
/// \endcode

  int       i1, ix, iy;
  // GdkDevice *dev;


// Gtk2:
  // gdk_window_get_pointer (gtk_widget_get_window(UI_MainWin), &ix, &iy, &i1);

// Gtk3:

  // UI_dev = gtk_get_current_event_device ();
  // gdk_window_get_device_position (gtk_widget_get_window(UI_MainWin),
                                  // UI_dev,
                                  // NULL, NULL,
                                  // &i1);

  gtk_get_current_event_state ((GdkModifierType*)&i1);
    // printf(" GUI_get_keys_mod %d\n",i1);



  // shift=1, ctrl=4

  return i1;

}

/*
//================================================================
  int GUI_cursor__ (MemObj *mo, int iCur) {
//================================================================
/// \code
/// GUI_cursor__              change cursor;
///   mo         window-obj; NULL = mainWindow
///   iCur       0=normal, 1=wait, 2=rotate, 3=hand, 4=size 5=blocked
/// \endcode

  Obj_Unknown *go;
  void        *win;

  printf("GUI_gl_cursor__ %d\n",iCur);

  if(mo) {
    go = GUI_obj_pos (mo);
    if(!go) return -1;
    win = go->widget;

  } else {
    win = UI_MainWin;
  }

  return GUI_cursor_set (win, iCur);

}
*/


//================================================================
  int GUI_get_version (char *SW, int *vMaj, int *vMin) {
//================================================================
/// \code
/// GUI_get_version     returns guiSW, SW-mayor-version, SW-minor-version
/// Output:
///   SW          "gtk"
///   vMaj        2 or 3
/// \endcode


  strcpy(SW, "gtk");
  *vMaj = GTK_MAJOR_VERSION;
  *vMin = GTK_MINOR_VERSION;

  return 0;

}
  

//================================================================
  int GUI_ck_version (int vMaj, int vMin) {
//================================================================
/// INTERNAL check GTK_MAJOR_VERSION,GTK_MINOR_VERSION


  // printf("GUI_ck_version %d %d\n",vMaj,vMin);


  printf("GTK_MAJOR_VERSION=%d GTK_MINOR_VERSION=%d\n",
          GTK_MAJOR_VERSION,GTK_MINOR_VERSION);


  if(GTK_MAJOR_VERSION < vMaj) return -1;

  if(GTK_MAJOR_VERSION == vMaj) {
    if(GTK_MINOR_VERSION < vMin) {
      return -1;
    }
  }

  return 0;

}


//================================================================
  int GUI_cursor__ (MemObj *mo, int iCur) {
//================================================================
/// \code
/// GUI_cursor__              change cursor;
///   mo         window-obj; NULL = mainWindow
///   iCur       0=normal, 1=wait, 2=rotate, 3=hand, 4=size 5=blocked
/// \endcode

  static int curTab[]={GDK_LEFT_PTR,       // 0 68   normal
                      GDK_WATCH,           // 1 150
                      GDK_EXCHANGE,        // 2 50   rotate
                      GDK_HAND1,           // 3 58   move
                      GDK_SIZING,          // 4 120
                      GDK_X_CURSOR};       // 5 0    GDK_MAN,GDK_PIRATE};
// GDK_DIAMOND_CROSS      36
// GDK_SB_V_DOUBLE_ARROW 116 
// GDK_MAN                80   no selections


  int         iNew;
  Obj_Unknown *go;
  void        *win;


  // printf("GUI_cursor__ %d\n",iCur);


  if(mo) {
    go = GUI_obj_pos (mo);
    if(!go) return -1;
    win = go->widget;

  } else {
    win = UI_MainWin;
  }
    

  if(iCur < 0) return -1;
  if(iCur > 5) return -1;

  iNew = curTab[iCur];

  if(gtk_widget_is_drawable (win) == FALSE) return 0;    // Gtk-2.24
  // if(GTK_WIDGET_DRAWABLE (win) == FALSE) return 0;       // Gtk-2.16


  gdk_window_set_cursor (gtk_widget_get_window(win),
                         gdk_cursor_new (iNew));

  // gdk_flush();
  // gdk_display_sync (gdk_display_get_default ());


  return 0;

}


//================================================================
  int GUI_OBJ_TYP (MemObj *mo) {
//================================================================
/// returns type of gui-obj

  Obj_Unknown   *go;

  go = GUI_obj_pos (mo);
  if(!go) return 0;

  return (go->gio_typ);

}


//================================================================
  int GUI_OBJ_SRC (MemObj *mo) {
//================================================================
/// returns gui-widget-nr

  Obj_Unknown   *go;

  go = GUI_obj_pos (mo);
  if(!go) return -1;

  return (go->gio_src);

}


//================================================================
  void** GUI_SETDAT_E (int ie) {
//================================================================
/// \code
/// GUI_SETDAT_E             create a GUI_DATA-block
/// Input:
///   ifunc        int GUI_DATA_EVENT
///
/// Usage:
///   cbFunc (NULL, GUI_SETDAT_E(TYP_EventExit));
///     // call cbFunc (mo, **data)
///     // with data[0] = GUI_DATA_EVENT = (int)TYP_EventExit
/// \endcode

  static void *pa[1];
  static int  ia0;


  // printf("GUI_SETDAT_E %d\n",ie);

  ia0 = ie;            // make it static
  pa[0] = &ia0;

  return (pa);

}


//================================================================
  void** GUI_SETDAT_ES (int ie, char *is) {
//================================================================
/// \code
/// GUI_SETDAT_ES             create a GUI_DATA-block
/// Input:
///   ie           int event; eg TYP_EventPress
///   is           stringpointer
///
/// Usage:
///   cbFunc (NULL, GUI_SETDAT_ES (TYP_EventPress,"Func1"));
///     // call cbFunc (mo, **data)
///     // with data[0] = GUI_DATA_EVENT = (int)TYP_EventPress
///     // with data[1] = GUI_DATA_S1    = (char*)data[1] = "Func1"
/// \endcode

  static void *pa[2];
  static int  ia0;
  // int ia[2];


  // printf("GUI_SETDAT_ES %d |%s|\n",ie,is);

  ia0 = ie;            // make it static
  pa[0] = &ia0;

  pa[1] = is;

  return (pa);

}


//================================================================
  void** GUI_SETDAT_EI (int ie, int ii) {
//================================================================
/// \code
/// GUI_SETDAT_EI             create a GUI_DATA-block
/// Input:
///   ie           int event; eg TYP_EventPress
///   ii           int data
///
/// Usage:
///   cbFunc (NULL, GUI_SETDAT_EI (TYP_EventPress,UI_FuncInit));
///     // call cbFunc (mo, **data)
///     // with data[0] = GUI_DATA_EVENT = (int)TYP_EventPress
///     // with data[1] = GUI_DATA_I1    = (int)UI_FuncInit
/// \endcode

  static void *pa[2];
  static int ia[2];


  // printf("GUI_SETDAT_EI %d %d\n",ie,ii);

  ia[0] = ie;        // make it static ..
  pa[0] = &ia[0];

  ia[1] = ii;
  pa[1] = &ia[1];

  return (pa);

}


/*
//================================================================
  void** GUI_DATA__ (int ifunc1, ...) {
//================================================================
/// \code
/// GUI_SETDAT_EI             create a GUI_DATA-block
/// Input:
///   ifunc        int GUI_DATA_EVENT
///
/// Usage:
///   cbFunc (NULL, GUI_SETDAT_EI (TYP_EventPress,UI_FuncInit, NULL));
///     // call cbFunc (mo, **data)
///     // with data[0] = GUI_DATA_EVENT = (int)TYP_EventPress
///     // with data[1] = GUI_DATA_I1    = (int)UI_FuncInit
/// \endcode

  static  void *pa[4];
  static  int ia[2];
  va_list va;
  void    *vp1;


  printf("GUI_SETDAT_EI %d\n",ifunc1);

  ia[0] = ifunc1;        // make it static ..
  pa[0] = &ia[0];

  va_start (va, ifunc1);
  ia[1] = va_arg (va, int);
    printf(" ia[1]=%d\n",ia[1]);


  if(ia[1]) {
    pa[1] = &ia[1];
  }

  va_end (va);

  return (pa);

}
*/

// EOF
