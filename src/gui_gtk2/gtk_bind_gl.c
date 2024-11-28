// ../gui_gtk2/gtk_bind_gl.c
// myOldVersion/gui_gtk2_MS/gtk_gl_MS.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <gtk/gtk.h>
#include <gtkgl/gtkglarea.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "../gui_gtk2_MS/GLB.h"

static GtkWidget   *glarea;






//================================================================
  void* UI_GR_WinInit (int width, int height, int border) {
//================================================================
// GraphicWindow
// siehe glXChooseVisual u /usr/include/gtkgl/gdkgl.h
// glXGetConfig glXCreateContext


  int    i1;
  // char   *p1, cbuf1[128], cbuf2[128], cbuf3[128];


  printf("UI_GR_WinInit\n");


  // create GL-widget glarea
  UI_GR_DrawCreate ();


  GTK_WIDGET_SET_FLAGS  (glarea, GTK_CAN_FOCUS);
  // see UI_GR_focus


  // Events for widget must be set before X Window is created
  // vi /usr/include/gdk/gdktypes.h
  gtk_widget_set_events (GTK_WIDGET(glarea),
                        // GDK_EXPOSURE_MASK|
                        GDK_VISIBILITY_NOTIFY_MASK|
                        GDK_ENTER_NOTIFY_MASK|
                        GDK_BUTTON_PRESS_MASK|
                        GDK_BUTTON_RELEASE_MASK|
                        GDK_KEY_PRESS_MASK|
                        GDK_KEY_RELEASE_MASK|
                        GDK_SCROLL_MASK|
                        GDK_POINTER_MOTION_MASK|
                        GDK_POINTER_MOTION_HINT_MASK);
                        // GDK_LEAVE_NOTIFY_MASK|

  // GDK_BUTTON1_MOTION_MASK ?
  // GDK_BUTTON2_MOTION_MASK ?
  // KeyPress u. Release muessen hier aktiv sein, damit es das EditWin,
  // das den Focus hat, bekommen kann.



  // spricht nur mit dem kompletten Fenster an (nicht NUR beim GL-Fenster)
                        // GDK_FOCUS_CHANGE|
  // gtk_signal_connect (GTK_OBJECT(glarea), "focus_in_event",
                      // GTK_SIGNAL_FUNC(UI_GR_Enter), NULL);

  // Redraw image when exposed.
  // gtk_signal_connect (GTK_OBJECT(glarea), "realize",
                     // GTK_SIGNAL_FUNC(UI_GR_GLInit), NULL);

  // Linux only (nicht MS-Win):
  // g_signal_connect_after (GTK_OBJECT(glarea), "visibility_notify_event",
  // Linux and MS-Win:
  g_signal_connect_after (GTK_OBJECT(glarea), "expose_event",
                      GTK_SIGNAL_FUNC(UI_GR_Draw), NULL);

  // gtk_signal_connect (GTK_OBJECT(glarea), "configure_event",
  g_signal_connect_after (GTK_OBJECT(glarea), "configure_event",
                      GTK_SIGNAL_FUNC(UI_GR_Reshape), NULL);

  // gtk_signal_connect (GTK_OBJECT(glarea), "motion_notify_event",
                      // GTK_SIGNAL_FUNC(UI_GR_MotionNotify), NULL);
  g_signal_connect_after (G_OBJECT(glarea), "motion_notify_event",
                    G_CALLBACK(UI_GR_MotionNotify), NULL);

  // enter  u leave
  g_signal_connect (GTK_OBJECT(glarea), "enter_notify_event",
                      GTK_SIGNAL_FUNC(UI_GR_Enter), NULL);

  // gtk_signal_connect (GTK_OBJECT(glarea), "leave_notify_event",
                      // GTK_SIGNAL_FUNC(UI_GR_Leave), NULL);

  g_signal_connect (GTK_OBJECT(glarea), "scroll_event",
                      GTK_SIGNAL_FUNC(UI_GR_Scroll), NULL);


  // die Mausbuttons
  g_signal_connect (GTK_OBJECT(glarea), "button_press_event",
                      GTK_SIGNAL_FUNC(UI_GR_ButtonPress), NULL);

  g_signal_connect (GTK_OBJECT(glarea), "button_release_event",
                      GTK_SIGNAL_FUNC(UI_GR_ButtonRelease), NULL);

//
  // // 2009-02-22 raus (Gtk-CRITICAL beim startup)
  // gtk_signal_connect (GTK_OBJECT(glarea), "draw",
                      // GTK_SIGNAL_FUNC(UI_GR_Redraw), NULL);
  // printf(" draw\n");
//

//
  // // die Keys; gehen nur, wenn GL-Win Focus hat - derzeit aber gar nicht ..
  // gtk_signal_connect (GTK_OBJECT(glarea), "key_press_event",
                      // GTK_SIGNAL_FUNC(UI_GR_KeyPress), NULL);

  // gtk_signal_connect (GTK_OBJECT(glarea), "key_release_event",
                      // GTK_SIGNAL_FUNC(UI_GR_KeyRelease), NULL);
//

//
  // gtk_signal_connect (GTK_OBJECT(glarea), "destroy",
                      // GTK_SIGNAL_FUNC (UI_GR_Destroy), NULL);



  // set minimum size
  // gtk_widget_set_usize (GTK_WIDGET(glarea), width, height);

  // put glarea into window and show it all
  // gtk_container_add (GTK_CONTAINER(frm1),GTK_WIDGET(glarea));
  // gtk_widget_show (GTK_WIDGET(glarea));
  // gtk_widget_show (frm1);

  // set focus to glarea widget (nur f. KeyEvents erforderl)
  // UI_GR_focus ();


  printf("ex UI_GR_WinInit\n");

  return glarea;

}


//================================================================
  GtkWidget*  GLB_Create () {
//================================================================
/// GLB_Create            create opengl-window (used by GUI_gl__)

  int attrlist[] = {
    GDK_GL_RGBA,
    GDK_GL_RED_SIZE,1,
    GDK_GL_GREEN_SIZE,1,
    GDK_GL_BLUE_SIZE,1,
    GDK_GL_DOUBLEBUFFER,
    GDK_GL_NONE
  };


  glarea = GTK_WIDGET(gtk_gl_area_new(attrlist));

  gtk_widget_set_events(glarea,
                                          GDK_EXPOSURE_MASK|
                                          GDK_BUTTON_PRESS_MASK);
  return glarea;

}


//================================================================
  void GLB_DrawInit () {
//================================================================

  // printf("GLB_DrawInit %d\n");

  gtk_gl_area_make_current (GTK_GL_AREA(glarea));

  // gtk_gl_area_begingl (GTK_GL_AREA(glarea));


  return;
}



//================================================================
  void GLB_DrawExit () {
//================================================================

  // printf("GLB_DrawExit %d\n");

  // GC_GR_Do_Display ();

  // gtk_gl_area_endgl (GTK_GL_AREA(glarea));
  gtk_gl_area_swap_buffers (GTK_GL_AREA(glarea));


  return;
}


// EOF
