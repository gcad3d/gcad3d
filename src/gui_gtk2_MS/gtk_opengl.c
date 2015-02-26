/*
 * gtk_opengl.c
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
OpenGL-connect.


-----------------------------------------------------
TODO:

-----------------------------------------------------
Modifications:
2014-01-17 UI_GL_draw__ Enter-Event after change-virtual-screen added
           (gtk does not provide this). RF.
2013-01-04 GUI_gl_ev_butt_upd added; RF.
2012-09-14 GUI_gl_ev_key Returncodes added; RF.
2012-09-05 GUI_gl__ TYP_EventEnter added. RF.
2012-02-01 extracted from ut_gtk.c.  RF.


-----------------------------------------------------
*/
#ifdef globTag
void GUI_OPENGL(){}
#endif
/*!
\file ../gui_gtk2/gtk_opengl.c
\ingroup grp_gui
\code

=====================================================
List_functions_start:

GUI_gl__              get opengl-window; connect redraw- and resize-event
GUI_gl_ev_move        connect mouse-move
GUI_gl_ev_button      connect mouse-button
GUI_gl_ev_key         connect keyboard
GUI_gl_block          block (1) / unblock (0) input
GUI_gl_set_active     activate/disactivate output to gl-window

GUI_gl_config         INTERNAL callback
GUI_gl_draw           INTERNAL callback
GUI_gl_move           INTERNAL callback
GUI_gl_button         INTERNAL callback
GUI_gl_key            INTERNAL callback
GUI_gl_idle1          INTERNAL callback

INTERNAL:
GUI_gl_ev_butt_upd    update (release) mousebuttons
GUI_gl_ev_butt__      usercall mousebutton

List_functions_end:
=====================================================

\endcode *//*----------------------------------------

Test:
make -f tst_gui_core.mak && ./a.out


======================================================================
*/


// #include "../xa/MS_Def0.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>

#include "../ut/ut_umem.h"

#include "../gui/gui_types.h"                 // TYP_Event..
#include "../gui/gui_base.h"                  // GUI_obj_*

#include "../gui_gtk2/gtk_base.h"
#include "../gui_gtk2/gtk_opengl.h"
#include "../gui_gtk2/gtk_gl_x11.h"



// externals:
// ex gtk_core:
extern GtkWidget *UI_MainWin;
extern GtkWidget *UI_act_wi;


// local:
static Obj_GLwin *UI_win_gl;
static MemObj    UI_mo_gl;
static int       statM1=0, statM2=0, statM3=0;    // state mousebuttons
static int       statMK=0;                        // state modifier-keys





//================================================================
  int GUI_gl_set_active (int mode, MemObj *mo) {
//================================================================
/// \code
/// GUI_gl_set_active     activate/disactivate output to gl-window
///   mode        1=activate  (call before using gl-functions)
///               0=disactivate; (call after gl-functions = execute)
/// must be used for all gl*-functions
/// \endcode



  if(!mode)  {         // disactivate
    GLB_DrawExit ();
    return 0;
  }


  if(mo) {            // NULL if used internal
    UI_win_gl = GUI_obj_pos (mo);
    if(!UI_win_gl) return 0;
  }

  GLB_DrawInit (UI_win_gl->widget);

  return 0;

}


//==========================================================================
  MemObj GUI_gl__ (MemObj *o_par, void *fDraw, char *opts) {
//==========================================================================
/// \code
/// GUI_gl__              get opengl-window; connect redraw- and resize-event
/// Input:
///   o_par      parentBox
///   fDraw      callback-function redraw-, resize-event
///   opts       options; (HorSiz,VertSiz)
///              HorSiz,VertSiz:  size in characters; default is automatic size.
///                               negative values: size in pixels.
///                              'e' = expand widget; default is fixed size.
///              Examples: "" or "-500,-500" or "10e,e"
///                "10e,e" horiz. size 10 chars, hor. and vert. expandable
///
/// Prototypes:
/// int fDraw (void *parent, void **data);
///   // data=table of 3 pointers;
///   GUI_DATA_EVENT=*(int*)data[0]=TYP_EventConfig|TYP_EventDraw|TYP_EventEnter
///     parameters for TYP_EventConfig and TYP_EventDraw:
///   GUI_DATA_I1   =*(int*)data[1]=window-width
///   GUI_DATA_I2   =*(int*)data[2]=window-height
///     parameters for TYP_EventEnter:
///   GUI_DATA_I1   =*(int*)data[1]=keystate GUI_Modif_*
///
///   GUI_OBJ_TYP(parent)  = TYP_GUI_BoxGL
///      
///
/// \endcode
 
// event_box hilft nix ..

  int        pTyp;
  void       *w_par;
  GtkWidget  *glarea;
  Obj_GLwin  *go;


  // printf("GUI_gl__ V1 TRUE=%d FALSE=%d\n",TRUE,FALSE);

  if(GLB_Query()) {
    printf("ERROR: OpenGl not supported.\n");
    return (UME_obj_invalid_set (-2));
  }


  // get gtk-widget for parentBox from MemObj
  w_par = GUI_obj_parent__ (&pTyp, o_par);
  if(!w_par) return (UME_obj_invalid_set (-3));


  // get address & memspc
  GUI_obj_spc ((void**)&go, sizeof(Obj_GLwin));
  if(!go) return (UME_obj_invalid_set (-1));


/*
  //----------------------------------------------------------------
  // set evenets for parent-box
  gtk_widget_set_events (GTK_WIDGET (w_par),
                         GDK_ALL_EVENTS_MASK);
                         // GDK_EXPOSURE_MASK);

  g_signal_connect_after (w_par, "configure-event",
                    G_CALLBACK (GUI_gl_draw), NULL);

  g_signal_connect_after (w_par, "expose",
                    G_CALLBACK (GUI_gl_draw), NULL);
*/

/*
  //----------------------------------------------------------------
  // create eventbox for glarea
  eb1 = gtk_event_box_new ();

  gtk_container_add (GTK_CONTAINER (w_par), eb1);

  gtk_widget_set_events (GTK_WIDGET (w_par),
                         GDK_ALL_EVENTS_MASK);
                         // GDK_EXPOSURE_MASK);

  g_signal_connect_after (w_par, "configure-event",
                    G_CALLBACK (GUI_gl_draw), NULL);
*/



  //----------------------------------------------------------------
  glarea = GLB_Create ();
  gtk_widget_set_app_paintable (glarea, TRUE);
  gtk_widget_set_redraw_on_allocate (glarea, FALSE);
  // gtk_widget_set_can_focus (glarea, TRUE);           // for keys necessary ..
  GTK_WIDGET_SET_FLAGS  (glarea, GTK_CAN_FOCUS);


  gtk_widget_set_events (GTK_WIDGET (glarea),
                         // GDK_VISIBILITY_NOTIFY_MASK|
                         // GDK_ENTER_NOTIFY_MASK|
                         // GDK_EXPOSURE_MASK);
                         GDK_ALL_EVENTS_MASK);

  g_signal_connect (glarea, "enter-notify-event",
                    G_CALLBACK (GUI_gl_draw),
                    PTR_MEMOBJ(go->mem_obj));

  // g_signal_connect_after (glarea, "configure-event",
  g_signal_connect (glarea, "configure-event",
                    G_CALLBACK (GUI_gl_draw),
                    PTR_MEMOBJ(go->mem_obj));

  // g_signal_connect_after (glarea, "expose-event",
  g_signal_connect (glarea, "expose-event",
                    G_CALLBACK (GUI_gl_draw),
                    PTR_MEMOBJ(go->mem_obj));

/*
  if((hsiz)||(vsiz)) GUI_siz_set (glarea, hsiz, vsiz);
  gtk_container_add (GTK_CONTAINER (w_par), glarea);
  gtk_widget_show_all (glarea);
*/
  GUI_w_pack1 (pTyp, w_par, glarea, opts);

  go->gio_typ  = TYP_GUI_BoxGL;
  go->widget   = glarea;
  go->uFuDraw  = fDraw;
  go->uFuMove  = NULL;
  go->uFuButt  = NULL;
  go->uFuKey   = NULL;

  // gdk_threads_add_idle (GUI_gl_idle1, NULL);


  return (((Obj_Unknown*)go)->mem_obj);

}


//================================================================
  int GUI_gl_ev_move (MemObj *mo, void *fMove) {
//================================================================
/// \code
/// add callback for mouse-move-events in OpenGL-window
/// Input:
///   mo         OpenGL-window-obj
///   fMove      callback-function move-event
///
/// prototype:
/// int fMove   (MemObj *mo, void **data);
///   // data=table of 3 pointers;
///   GUI_DATA_EVENT=*(int*)data[0]=TYP_EventMove
///   GUI_DATA_I1   =*(int*)data[2]=x-val mousepos in screencoords
///   GUI_DATA_I2   =*(int*)data[3]=y-val mousepos in screencoords
///   GUI_OBJ_TYP(mo)  = TYP_GUI_BoxGL
///
///   printf(" x=%d y=%d\n",GUI_DATA_I1,GUI_DATA_I2);
///
/// \endcode


  void        *glArea;
  Obj_GLwin   *go;


  // printf("GUI_gl_ev_move \n");

  go = GUI_obj_pos (mo);
  if(!go) return 0;
  glArea = go->widget;

  gtk_widget_add_events (GTK_WIDGET(glArea),
                         GDK_POINTER_MOTION_MASK |
                         GDK_POINTER_MOTION_HINT_MASK);

  g_signal_connect (glArea, "motion_notify_event",
                    G_CALLBACK(GUI_gl_move),
                    PTR_MEMOBJ(go->mem_obj));

  go->uFuMove = fMove;

  return 0;

}


//================================================================
  int GUI_gl_ev_butt_upd () {
//================================================================
// update (release) mousebuttons; eg GUI_MsgBox can eat the release mousebutton !

  static int iTyp = TYP_EventRelease;


  // if M1=down: release M1
  if(statM1) {
    GUI_gl_ev_butt__ (&iTyp, &statM1);
    statM1 = 0;
  }
  if(statM2) {
    GUI_gl_ev_butt__ (&iTyp, &statM2);
    statM1 = 0;
  }
  if(statM3) {
    GUI_gl_ev_butt__ (&iTyp, &statM3);
    statM1 = 0;
  }

  return 0;

}


//================================================================
  int GUI_gl_ev_butt__ (int *iTyp, int *ikey) {
//================================================================
// GUI_gl_ev_butt__      usercall mousebutton

static int iDev=TYP_DeviceMouse;

  void         *pTab[3];

  pTab[0] = iTyp;
  pTab[1] = &iDev;
  pTab[2] = ikey;

  UI_win_gl->uFuButt (&UI_mo_gl, pTab);

  return 0;

}
 

//================================================================
  int GUI_gl_ev_button (MemObj *mo, void *fButton) {
//================================================================
/// \code
/// add callback for the mouse-button event
/// Input:
///   mo         OpenGL-window-obj
///   fButton    callback-function mouse-button event
///
/// prototype:
/// int fButton  (MemObj *mo, void **data);
///   // data=table of 2 pointers
///   GUI_DATA_EVENT=*(int*)data[0]=TYP_EventPress|TYP_EventRelease
///   GUI_DATA_I1   =*(int*)data[1]=TYP_DeviceMouse
///   GUI_DATA_I2   =*(int*)data[2]=GUI_MouseL|      left mousebutton
///                                 GUI_MouseM|      mid mousebutton
///                                 GUI_MouseR|      right mousebutton
///                                 GUI_MouseScUp|   mouse scroll up
///                                 GUI_MouseScDown  mouse scroll down
///   GUI_OBJ_TYP(mo)  = TYP_GUI_BoxGL
///
///   printf(" button=%d\n",GUI_DATA_I1);
///
/// \endcode

  void        *glArea;
  Obj_GLwin   *go;


  // printf("GUI_gl_ev_button \n");

  go = GUI_obj_pos (mo);
  if(!go) return 0;
  glArea = go->widget;


  gtk_widget_add_events (GTK_WIDGET(glArea),
                         GDK_SCROLL_MASK |
                         GDK_BUTTON_PRESS_MASK |
                         GDK_BUTTON_RELEASE_MASK);

  g_signal_connect (glArea, "button_press_event",
                    G_CALLBACK(GUI_gl_button),
                    PTR_MEMOBJ(go->mem_obj));

  g_signal_connect (glArea, "button_release_event",
                    G_CALLBACK(GUI_gl_button),
                    PTR_MEMOBJ(go->mem_obj));

  g_signal_connect (glArea, "scroll_event",
                    G_CALLBACK(GUI_gl_button),
                    PTR_MEMOBJ(go->mem_obj));

  go->uFuButt = fButton;

  return 0;

}


/*
GUI_gl_ev_test(MemObj *mo) {
  void        *glArea;
  Obj_GLwin   *go;

  go = GUI_obj_pos (mo);
  if(!go) return 0;
  glArea = go->widget;

  
  g_signal_connect (glArea, "enter-notify-event",
                    G_CALLBACK(gtk_widget_grab_focus), (void*)FALSE);

}
*/

//================================================================
  int GUI_gl_ev_key (MemObj *mo, void *fKey) {
//================================================================
/// \code
/// add callback for the key-events
/// Input:
///   mo         OpenGL-window-obj
///   fKey       callback-function key-events
///
/// prototyp fKey:
/// int fKey (MemObj *mo, void **data);
///   // data=table of 3 pointers
///   GUI_DATA_EVENT =*(int*)data[0]=TYP_EventPress|TYP_EventRelease
///   GUI_DATA_I1    =*(int*)data[1]=TYP_DeviceKeyb
///   GUI_DATA_I2    =*(int*)data[2]=keyvalue; eg 'a'
///   GUI_DATA_I3    =*(int*)data[3]=state of modifierkeys;
///                                  see ../gui/gui_types.h  GUI_Modif_*
///                                  &1=shift; &4=ctrl; &8=alt.
///   GUI_OBJ_TYP(mo)  = TYP_GUI_BoxGL
///   Returncodes:    0=continue with defaultOperations for this key.
///                   1=do no defaultOperations; skip handling this key
///
///   printf(" key=%d\n",GUI_DATA_I1);
///
/// Example see GUI_entry__
/// \endcode



  void        *glArea;
  Obj_GLwin   *go;
  

  // printf("GUI_gl_ev_key \n");

  go = GUI_obj_pos (mo);
  if(!go) return 0;
  glArea = go->widget;


  gtk_widget_add_events (GTK_WIDGET(glArea),
                         GDK_ENTER_NOTIFY_MASK |
                         GDK_KEY_PRESS_MASK |
                         GDK_KEY_RELEASE_MASK);

  // grab focus when entering the gl-window; else no keyboard-callback.
  g_signal_connect (glArea, "enter-notify-event",
                    G_CALLBACK(gtk_widget_grab_focus), (void*)TRUE);

  g_signal_connect (glArea, "key-press-event",
                    G_CALLBACK(GUI_gl_key),
                    PTR_MEMOBJ(go->mem_obj));

  g_signal_connect (glArea, "key-release-event",
                    G_CALLBACK(GUI_gl_key),
                    PTR_MEMOBJ(go->mem_obj));


  go->uFuKey = fKey;

  return 0;

}


//================================================================
  int GUI_gl_block (MemObj *mo, int mode) {
//================================================================
/// \code
/// block (1) or unblock (0) input
/// Blocks scroll-events, key-events, button-events and move-events.
///   mo         OpenGL-window-obj
/// \endcode

  int         i1;
  Obj_GLwin   *go;


  // printf("GUI_gl_block %d\n",mode);

  go = GUI_obj_pos (mo);
  if(!go) return 0;

  // if(!UI_MainWin) return 0;  // only for MS-Win !

  if(mode != 0) goto L_1;


  //----------------------------------------------------------------
  // unblock scroll
  if(go->uFuKey)
  g_signal_handlers_unblock_by_func (go->widget,
                                     G_CALLBACK(GUI_gl_key),
                                     PTR_MEMOBJ(go->mem_obj));
                                     

  // unblock grafic_selections
  if(go->uFuButt)
  g_signal_handlers_unblock_by_func (go->widget,
                                     G_CALLBACK(GUI_gl_button),
                                     PTR_MEMOBJ(go->mem_obj));
  // unblock mouse-movements
  if(go->uFuMove)
  // i1 =
  g_signal_handlers_unblock_by_func (go->widget,
                                     G_CALLBACK(GUI_gl_move),
                                     PTR_MEMOBJ(go->mem_obj));
    // printf(" move %d unblocked\n",i1);

  return 0;


  //----------------------------------------------------------------
  // 1 = block keystrokes & grafic_selections
  L_1:
  // block mouse-movements
  if(go->uFuMove)
  // i1 =
  g_signal_handlers_block_by_func (go->widget,
                                   G_CALLBACK(GUI_gl_move),
                                   PTR_MEMOBJ(go->mem_obj));
    // printf(" move %d blocked\n",i1);

  // block grafic selections
  if(go->uFuButt)
  g_signal_handlers_block_by_func (go->widget,
                                   G_CALLBACK(GUI_gl_button),
                                   PTR_MEMOBJ(go->mem_obj));
  // block keystrokes
  if(go->uFuKey)
  g_signal_handlers_block_by_func (go->widget,
                                   G_CALLBACK(GUI_gl_key),
                                   PTR_MEMOBJ(go->mem_obj));

  return 0;

}


//========================================================================
// INTERNAL CALLBACKS:
//========================================================================

/*
//================================================================
  int GUI_gl_pending () {
//================================================================
// gdkD=GdkDisplay from gdk_display_get_default
 
  // printf("pending %d\n",gdk_display_has_pending (gdkD));

  return gdk_display_has_pending (gdkD);

}
*/


//================================================================
  int GUI_gl_draw (void *parent, void *event, MemObj mo) {
//================================================================
/// INTERNAL   draw / expose & config-callback


  static int      ix=0, iy=0;
  int             i1, i2, iEv;
  GtkAllocation   allocation;
  void            *glArea, *pTab[3];
  Obj_GLwin       *go;
  GdkModifierType state;


  // printf("GUI_gl_draw \n");

  go = GUI_obj_pos (&mo);
  if(!go) return 0;

  glArea = go->widget;
  if(!glArea) return FALSE;  // FALSE=continue event handling ..

  // i1 = gtk_widget_is_drawable(glArea);
    // printf(" focus=%d\n",i1);



  // BUG: enter-event does not come after change-virtual-screen; 2014-01-17
  // only if keys are connected..
  if(go->uFuKey) {
    gdk_window_get_pointer (NULL, NULL, NULL, &state);
    i1 = state;
      // printf(" ik=%d i1=%d\n",ik,i1);
    if(i1 != statMK) {
      iEv = TYP_EventEnter;
      pTab[0] = &iEv;
      pTab[1] = &i1;
      pTab[2] = &i1;
      go->uFuDraw (&mo, pTab);  // report the modified state-keys !
      statMK = i1;
    }
  }



  if(event) {
    // printf(" eventType=%d\n",((GdkEventAny*)event)->type);


    //----------------------------------------------------------------
    // 13=GDK_CONFIGURE
    if(((GdkEventAny*)event)->type == GDK_CONFIGURE) {
  // Gtk2.24
      // gdk_window_get_origin(gtk_widget_get_window(UI_win_glBox),&xPos,&yPos);
      gtk_widget_get_allocation (glArea, &allocation);
      i1 = allocation.width;
      i2 = allocation.height;

  // Gtk2.16
      // i1 = ((GtkWidget*)glArea)->allocation.width;
      // i2 = ((GtkWidget*)glArea)->allocation.height;

        // printf("  new size %d %d\n", i1,i2);

      if((i1  != ix) || (i2 != iy)) {
        // size changed
        iEv = TYP_EventConfig;
        ix = i1;
        iy = i2;
    
        // necessary for clean redraw when resizing ..
        gtk_widget_queue_resize (glArea);

      } else {
        return 0;
      }


    //----------------------------------------------------------------
    // GDK_ENTER_NOTIFY  = 10,
    } else if(((GdkEventAny*)event)->type == GDK_ENTER_NOTIFY) {
      // get state of shift,ctrl,alt-keys
      gdk_window_get_pointer (NULL, NULL, NULL, &state);
        // printf(" enter .. %d\n",state);
      iEv = TYP_EventEnter;
      i1 = state;
      pTab[0] = &iEv;
      pTab[1] = &i1;
      pTab[2] = &i1;
      goto L_CU;


    //----------------------------------------------------------------
    // GDK_EXPOSE    = 2,
    } else {
      // draw
      iEv = TYP_EventDraw;
    }
  }


  pTab[0] = &iEv;
  pTab[1] = &ix;
  pTab[2] = &iy;


  L_CU:
  // GLB_DrawInit (glArea);
  go->uFuDraw (&mo, pTab);
  // GLB_DrawExit ();

  return FALSE;  // FALSE=continue event handling ..

}

/*
//========================================================================
  int GUI_gl_config (void *parent, void *ev, void *data) {
//========================================================================

  int           i1;
  void          (*uFunc)();
  GtkAllocation allocation;

  printf("GUI_gl_config %d %d %d %d %d\n",
         ((GdkEventConfigure*)ev)->send_event,
         ((GdkEventConfigure*)ev)->x,
         ((GdkEventConfigure*)ev)->y,
         ((GdkEventConfigure*)ev)->width,
         ((GdkEventConfigure*)ev)->height);

  gtk_widget_get_allocation (parent, &allocation);

  printf(" focus=%d\n",gtk_widget_is_drawable(UI_win_glArea));

  gtk_widget_set_size_request (UI_win_glArea, allocation.width, allocation.height);

  uFunc = data;
  uFunc (parent, allocation.width, allocation.height);

  return 0;  // TRUE=1=stop further handling of event ..

}
*/

//================================================================
  int GUI_gl_move (void *parent, void *event, MemObj mo) {
//================================================================
/// INTERNAL 
 
  int          i1, x, y, iEv=TYP_EventMove;
  void         *pTab[3];
  Obj_GLwin    *go;


  // printf("GUI_gl_move \n");

  go = GUI_obj_pos (&mo);
  if(!go) return 0;

  // if (event->is_hint) {
  gtk_widget_get_pointer (go->widget, &x, &y);
  // }

    // printf("GUI_gl_move %d %d\n",x,y);

  pTab[0] = &iEv;
  pTab[1] = &x;
  pTab[2] = &y;

  go->uFuMove (&mo, pTab);

  return FALSE;  // TRUE=1=stop further handling of event ..

}


//================================================================
  int GUI_gl_button (void *parent, void *ev, MemObj mo) {
//================================================================
/// INTERNAL 


  int          typ, ikey, iTyp;


  // printf("GUI_gl_button\n");

  UI_mo_gl  = mo;
  UI_win_gl = GUI_obj_pos (&mo);
  if(!UI_win_gl) return 0;

  typ  = ((GdkEventAny*)ev)->type;

  if(typ == GDK_BUTTON_PRESS) {
    iTyp = TYP_EventPress;
    ikey = ((GdkEventButton*)ev)->button;
    if(ikey == GUI_MouseL) statM1 = 1;
    if(ikey == GUI_MouseM) statM2 = 1;
    if(ikey == GUI_MouseR) statM3 = 1;


  } else if(typ == GDK_BUTTON_RELEASE) {
    iTyp = TYP_EventRelease;
    ikey = ((GdkEventButton*)ev)->button;
    if(ikey == GUI_MouseL) statM1 = 0;
    if(ikey == GUI_MouseM) statM2 = 0;
    if(ikey == GUI_MouseR) statM3 = 0;


  } else if(typ == GDK_SCROLL) {
      // printf(" scroll(%d) %d\n",GDK_SCROLL,((GdkEventScroll*)ev)->direction);
    iTyp = TYP_EventPress;
    if(((GdkEventScroll*)ev)->direction == GDK_SCROLL_UP)
      ikey = GUI_MouseScUp;
    else if(((GdkEventScroll*)ev)->direction == GDK_SCROLL_DOWN)
      ikey = GUI_MouseScDown;
    else goto L_exit;

  } else return FALSE;  // TRUE=1   FALSE=0

    // printf("GUI_gl_button typ=%d but=%d\n",typ,ikey);

  // call user
  GUI_gl_ev_butt__ (&iTyp, &ikey);


  L_exit:
  return FALSE;  // TRUE=1=stop further handling of event ..

}


//================================================================
  int GUI_gl_key (void *parent, void *event, MemObj mo) {
//================================================================
/// INTERNAL 


  int          typ, iTyp, ikey, ii, iDev=TYP_DeviceKeyb;
  void         *pTab[4];
  Obj_GLwin    *go;



  // printf("GUI_gl_key \n");
  
  go = GUI_obj_pos (&mo);
  if(!go) return 0;

  typ  = ((GdkEventAny*)event)->type;

  if(typ == GDK_KEY_PRESS) {
    iTyp = TYP_EventPress;       
  } else if(typ == GDK_KEY_RELEASE) {
    iTyp = TYP_EventRelease;
  } else return FALSE;  // TRUE=1   FALSE=0


  ikey = ((GdkEventKey*)event)->keyval;
  statMK = ((GdkEventKey*)event)->state;
    // printf("GUI_gl_key key=%d ev=%d state=%d\n",ikey,typ,imod);
                         
  pTab[0] = &iTyp;
  pTab[1] = &iDev;
  pTab[2] = &ikey;
  pTab[3] = &statMK;

  // go->uFuKey (&mo, pTab);
  // return FALSE;  // TRUE=1=stop further handling of event ..

  ii = go->uFuKey (&mo, pTab);
    // printf(" imod=%d\n",imod);

  return (ii);   // TRUE=1: do no defaultOperations

} 


//================================================================
  int GUI_gl_idle1 (void* data) {
//================================================================
/// \code
/// INTERNAL  draw must be last call 
///    return 0; // removes idle-Call!
///    return 1; // keep idle-Call; call AP_Idle_CB again.
/// \endcode

  // printf("GUI_gl_idle1 \n");

  GUI_gl_draw (NULL, NULL, MEMOBJ_PTR(NULL));

  return 0;  // close thread

}
 

// EOF
