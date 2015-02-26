/*
 * gtk_gl_MS32.c
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
Gtk-OpenGL-Binding for Linux (x11).


-----------------------------------------------------
Modifications:
2012-02-01 extracted from ut_gtk.c.  RF.



-----------------------------------------------------
*//*!
\file ../gui_gtk2/gtk_gl_x11.c
\ingroup grp_gui
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

#include <gtk/gtk.h>
#include <gtkgl/gtkglarea.h>
// #include <gdk/gdkx.h>        // gdk_x11_get_default_xdisplay GDK_WINDOW_XID

#include <GL/gl.h>
#include <GL/glu.h>

// #include "GLB.h"             // GLB_Create
#include "../gui_gtk2_MS/GLB.h"

// 
// static  GLXContext  GLB_x_context;
// static  Display     *GLB_display;

// static  int         GLB_x_id = 0;
// static  long        GLB_x_id = 0;

static GtkWidget   *glarea;



// unused dummy:
int GLB_pending () { return 0; }


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


/*
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


  return gdk_gl_make_current(glarea->window,
                             GTK_GL_AREA(glarea)->glcontext);

}


//================================================================
  void GLB_DrawExit () {
//================================================================
/// GLB_DrawExit          execute OpenGL-commandblock

  // glXSwapBuffers (GLB_display, GLB_x_id);
  // glXDestroyContext(GLB_display, GLB_x_context);

  // gtk_gl_area_swapbuffers(GTK_GL_AREA(widget));
  gdk_gl_swap_buffers (glarea->window);


  return;

}


//================================================================
  int GLB_Query () {
//================================================================
/// GLB_Query             display OpenGL-vendorstring (used by GUI_gl__)

  char  const  *p1, *p2;



  printf("GL_RENDERER:    %s\n",glGetString(GL_RENDERER));
  printf("GL_VERSION:     %s\n",glGetString(GL_VERSION));
  printf("GL_EXTENSIONS:  %s\n",glGetString(GL_EXTENSIONS));

  printf("GLU_VERSION:    %s\n",gluGetString(GLU_VERSION));
  printf("GLU_EXTENSIONS: %s\n",gluGetString(GLU_EXTENSIONS));


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
