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
*//*!
\file ../gui_gtk3/gtk_gl_x11.c
\brief Gtk-binding OpenGL Unix
\code

=====================================================
List_functions_start:

GLB_DrawInit          start OpenGL-commandblock
GLB_DrawExit          execute OpenGL-commandblock

GLB_Create            create opengl-window (used by GUI_gl__)
GLB_Query             display OpenGL-vendorstring (used by GUI_gl__)

List_functions_end:
=====================================================




// Gtk2 + OpenGL:
gcc -c `pkg-config --cflags gtk+-2.0` -DGTK2 gtk_gl_x11.c


// Gtk3 + OpenGL:
gcc -c `pkg-config --cflags gtk+-3.0` -DGTK3 gtk_gl_x11.c


======================================================================
*/


#include <stdio.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glx.h>

#include <gtk/gtk.h>
#include <gdk/gdkx.h>        // gdk_x11_get_default_xdisplay GDK_WINDOW_XID

// #include "GLB.h"             // GLB_Create
#include "../gui_gtk3/gtk_gl_x11.h"

// 
static  GLXContext  GLB_x_context;
static  Display     *GLB_display;
static  int         GLB_x_id = 0;



//================================================================
  void*  GLB_Create () {
//================================================================
/// GLB_Create            create opengl-window (used by GUI_gl__)
 
  GtkWidget   *area;
  int         xscreen, i1;
  GdkScreen   *screen;
  GdkVisual   *visual;
  Window      root;
  XVisualInfo *xvisual;
  Colormap    xcolormap;
  int         attributes[] = {
                GLX_RGBA,
                GLX_RED_SIZE, 1,
                GLX_GREEN_SIZE, 1,
                GLX_BLUE_SIZE, 1,
                GLX_DOUBLEBUFFER, True,
                GLX_DEPTH_SIZE, 24,
                None };

  GLB_x_id = 0;   // reset

  area = gtk_drawing_area_new ();
  gtk_widget_set_double_buffered (area, FALSE);

  GLB_display = gdk_x11_get_default_xdisplay ();
  xscreen = DefaultScreen (GLB_display);
  screen = gdk_screen_get_default ();
    // printf(" screenNr = %d\n",gdk_screen_get_number(screen));

  xvisual = glXChooseVisual (GLB_display, xscreen, attributes);
    // printf(" xvisualid=%d\n",xvisual->visualid);

  visual = gdk_x11_screen_lookup_visual (screen, xvisual->visualid);

  root = RootWindow (GLB_display, xscreen);
  xcolormap = XCreateColormap (GLB_display, root, xvisual->visual, AllocNone);

  glXGetConfig (GLB_display, xvisual, GLX_RED_SIZE, &i1);
    printf(" GLX_RED_SIZE=%d\n",i1);
  glXGetConfig (GLB_display, xvisual, GLX_DEPTH_SIZE, &i1);
    printf(" GLX_DEPTH_SIZE=%d\n",i1);

// Gtk2 only:
  //colormap = gdk_x11_colormap_foreign_new (visual, xcolormap);
  //gtk_widget_set_colormap (area, colormap);

  GLB_x_context = glXCreateContext (GLB_display, xvisual, NULL, TRUE);
  // free (xvisual);

  glXWaitX();
  glXWaitGL();

  return area;

}

/* UNUSED
//================================================================
  int GLB_pending () {
//================================================================

  return XPending(GLB_display);

}
*/
 
//================================================================
  int GLB_DrawInit (void *widget) {
//================================================================
/// GLB_DrawInit          start OpenGL-commandblock

  GdkWindow     *window;

  if(!GLB_x_id) {

    window = gtk_widget_get_window (widget);
    GLB_x_id = GDK_WINDOW_XID(window);                // causes expose!
      printf("  GLB_x_id=%d\n",GLB_x_id);
  }

  // GLB_x_context = glXCreateContext (GLB_display, xvisual, NULL, TRUE);

  glXMakeCurrent (GLB_display, GLB_x_id, GLB_x_context);

  return 0;

}


//================================================================
  void GLB_DrawExit () {
//================================================================
/// GLB_DrawExit          execute OpenGL-commandblock

  glXSwapBuffers (GLB_display, GLB_x_id);

  // glXDestroyContext(GLB_display, GLB_x_context);

  return;

}


//================================================================
  int GLB_Query () {
//================================================================
/// GLB_Query             display OpenGL-vendorstring (used by GUI_gl__)

  char  const  *p1, *p2;


  p1 = glXGetClientString (GLB_display, GLX_VENDOR);
  if(!p1) return -1;

  p2 = glXGetClientString(GLB_display, GLX_VERSION);
    printf("OpenGL Vendor %s  Version %s\n",p1,p2);

  // p1 = glXGetClientString(GLB_display, GLX_EXTENSIONS);
    // printf("Extensions: %s\n",p1);

  return 0;

}

/*
//================================================================
  int GLB_Config () {
//================================================================

  XVisualInfo *vi;
  int i1;

  vi = get_xvisualinfo(visual);

  glXGetConfig (GLB_display, vi, GLX_RED_SIZE, &i1);
    printf(" GDK_GL_RED_SIZE=%d\n",i1);

  return 0;

}
*/

// EOF
