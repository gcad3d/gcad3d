// test gtk + glx direct (without gtkgl, gtkglext, sdl).

// 2012-01-31  gtkglarea removed. RF
// 2002-09-28  Reiter Franz


// gcc -c tst_trackball.c
// gcc -c tst_surf2.c

// Gtk2:
// gcc -c `pkg-config --cflags gtk+-2.0` -DGTK2 gtk_gl_x11.c
// gcc -c `pkg-config --cflags gtk+-2.0` -DGTK2 tst_GL_gtk_2.c
// gcc tst_GL_gtk_2.o gtk_gl_x11.o tst_trackball.o tst_surf2.o `pkg-config --libs gtk+-2.0` -lGL -lGLU


// Gtk3:
// gcc -c `pkg-config --cflags gtk+-3.0` -DGTK3 gtk_gl_x11.c
// gcc -c `pkg-config --cflags gtk+-3.0` -DGTK3 tst_GL_gtk_2.c
// gcc tst_GL_gtk_2.o gtk_gl_x11.o tst_trackball.o tst_surf2.o `pkg-config --libs gtk+-3.0` -lGL -lGLU

// ./a.out

// Using: M1-drag = rotate
//        M2-drag = zoom
//        M3-drag = pan


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <gtk/gtk.h>
#include <gtkgl/gtkglarea.h>

#include <GL/gl.h>
#include <GL/glu.h>


#include "GLB.h"               // GLB_Create
#include "tst_geo.h"           // Pointf
#include "tst_trackball.h"     // trackball_..
// #include "tst_gui_gl2.h"



#define VIEW_ASPECT 1.3



int   do_init;        /* true if initgl not yet called */
float beginx,beginy;  /* position of mouse */
float quat[4];        /* orientation of object */
float zoom;           /* field of view in degrees */

Pointf cenPt;         // view-center in user-coords
// Modelsize; box from 2 points; all coords must be inside this box.
Pointf mdlMax = {30.0, 30.0,  30.0};
Pointf mdlMin = {-30.0, -30.0,  -30.0};



// Protos
int  showObjects();
void initgl(void);
gint glarea_configure(GtkWidget *widget, GdkEventConfigure *event);
gint glarea_expose(GtkWidget *widget, GdkEventExpose *event);
gint glarea_motion_notify(GtkWidget *widget, GdkEventMotion *event);
gint glarea_button_press(GtkWidget *widget, GdkEventButton *event);
void gl_init_trackball ();

 int tst_CB_b1 (GtkWidget *parent, void *data);
 int tst_CB_b2 (GtkWidget *parent, void *data);


extern  void tst_surf2_draw ();




// dummy-funcs
int GUI_Tip (char *key) { return 0; }
int GUI_update__ () { return 0; }




//====================================================================
int main (int argc, char **argv) {
//====================================================================

  printf("tst_GL_gtk_2.c\n");

  // initialize gtk
  gtk_init( &argc, &argv );

  // test if OpenGl is supported
  if(GLB_Query()) {printf("ERROR: OpenGl not supported.\n"); exit(0);}

  showObjects();

  gtk_main();

  return 0;
}


//====================================================================
int showObjects () {
//====================================================================

  GtkWidget *window, *glarea, *box1, *box2, *bt1, *bt2;


  // create opengl-area
  glarea = GLB_Create ();
  if (glarea == NULL) {
    g_print("Can't create GtkGLArea widget\n");
    return FALSE;
  }

  gtk_widget_set_events (GTK_WIDGET (glarea),
                         GDK_EXPOSURE_MASK |
                         GDK_BUTTON_PRESS_MASK |
                         GDK_BUTTON_RELEASE_MASK |
                         GDK_POINTER_MOTION_MASK |
                         GDK_POINTER_MOTION_HINT_MASK |
                         GDK_KEY_PRESS_MASK |
                         GDK_KEY_RELEASE_MASK);


  // set up events and signals for OpenGL widget
  g_signal_connect (glarea, "configure_event",
          G_CALLBACK(glarea_configure), NULL);

//Gtk2:
#ifdef gtk2
  g_signal_connect (glarea, "expose_event", G_CALLBACK(glarea_expose), NULL);
#endif

//Gtk3:
#ifdef gtk3
  g_signal_connect (glarea, "draw", G_CALLBACK(glarea_expose), NULL);
#endif


  g_signal_connect (glarea, "motion_notify_event",
          G_CALLBACK(glarea_motion_notify), NULL);

  g_signal_connect (glarea, "button_press_event",
          G_CALLBACK(glarea_button_press), NULL);


  gtk_widget_set_size_request (glarea, 500, 500/VIEW_ASPECT);


  gl_init_trackball ();     // set up trackball


  // create new top level window
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW(window), "Test");
  gtk_container_set_border_width (GTK_CONTAINER(window), 4);

  // box1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  box1 = gtk_vbox_new (FALSE, 1);
  gtk_container_add (GTK_CONTAINER (window), box1);

  // box2 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  box2 = gtk_hbox_new (FALSE, 1);
  gtk_container_add (GTK_CONTAINER (box1), box2);

  bt1 = gtk_button_new_with_label (" Button-1 ");
  g_signal_connect (bt1, "clicked", G_CALLBACK(tst_CB_b1), NULL);
  gtk_container_add (GTK_CONTAINER (box2), bt1);

  bt2 = gtk_button_new_with_label (" Button-2 ");
  g_signal_connect (bt2, "clicked", G_CALLBACK(tst_CB_b2), NULL);
  gtk_container_add (GTK_CONTAINER (box2), bt2);

  gtk_container_add (GTK_CONTAINER (box1), glarea);

  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);


  gtk_widget_show_all (window);

  return TRUE;

}


//====================================================================
gint glarea_expose(GtkWidget *widget, GdkEventExpose *event) {
//====================================================================

  GLfloat m[4][4];

  /* draw only last expose */
  // if (event->count > 0) { return TRUE; }

  printf("glarea_expose \n");


  GLB_DrawInit (widget);

    /* basic initialization */
    if (do_init == TRUE) {
      initgl();
      do_init = FALSE;
    }

    // backgrnd
    glClearColor(.3,.4,.6,1);   // hellblau
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    trackball_persp (&cenPt, &zoom, quat, &mdlMax, &mdlMin);

    tst_surf2_draw();

    GLB_DrawExit ();

  return TRUE;
}


//====================================================================
gint glarea_motion_notify(GtkWidget *widget, GdkEventMotion *event) {
//====================================================================

  int x, y;
  GdkRectangle area;
  GdkModifierType state;
  GtkAllocation allocation;


  printf("glarea_motion_notify\n");


  // if (event->is_hint) {
    gtk_widget_get_pointer (widget, &x, &y);
    state = event->state;
  // }


  area.x = 0;
  area.y = 0;
  gtk_widget_get_allocation (widget, &allocation);
  area.width  = allocation.width;
  area.height = allocation.height;

      // printf(" state=%d pos %d %d old %.f %.f siz %d %d\n",
              // state,x,y,beginx,beginy,area.width,area.height);


  if (state & GDK_BUTTON1_MASK) {
    /* drag in progress, simulate trackball */
    trackball_rot (quat, beginx, beginy, x, y, area.width, area.height, &cenPt);
    glarea_expose (NULL, NULL);
  }

  if (state & GDK_BUTTON2_MASK) {
    /* zooming drag */
    trackball_zoom (&zoom, beginx, beginy, x, y, area.width, area.height);
    glarea_expose (NULL, NULL);
  }

  if (state & GDK_BUTTON3_MASK) {
    /* zooming drag */
    trackball_pan (&cenPt, beginx, beginy, x, y, area.width, area.height);
    glarea_expose (NULL, NULL);
  }



  beginx = x;
  beginy = y;

  return TRUE;
}



//====================================================================
gint glarea_button_press(GtkWidget *widget, GdkEventButton *event)
//====================================================================
{
  if (event->button == 1) {
    /* beginning of drag, reset mouse position */
    beginx = event->x;
    beginy = event->y;
    return TRUE;
  }
  return FALSE;
}


//====================================================================
gint glarea_configure(GtkWidget *widget, GdkEventConfigure *event) {
//====================================================================

  GtkAllocation allocation;

  printf("glarea_configure \n");

  GLB_DrawInit (widget);

    gtk_widget_get_allocation (widget, &allocation);
    glViewport (0, 0, allocation.width, allocation.height);

  return TRUE;
}


//================================================================
void gl_init_trackball () {
//================================================================

  // set up trackball
  do_init = TRUE;
  beginx = 0;
  beginy = 0;
  zoom   = 1.;   // 45.   // 45.
  cenPt.x  = 0.;
  cenPt.y  = 0.;
  cenPt.z  = 0.;  //-30.;
  trackball (quat , 0.0, 0.0, 0.0, 0.0);

  return;

}


//====================================================================
void initgl(void) {
//====================================================================


  GLfloat light0_pos[4]   = { -50.0, 50.0, 0.0, 0.0 };
  GLfloat light0_color[4] = { .6, .6, .6, 1.0 }; /* white light */
  GLfloat light1_pos[4]   = {  50.0, 50.0, 0.0, 0.0 };
  GLfloat light1_color[4] = { .4, .4, 1, 1.0 };  /* cold blue light */


  printf("initgl\n");

  /* remove back faces */
  glDisable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  // glDepthFunc(GL_LESS);

  /* light */
  glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_color);
  glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
  glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_color);
  glEnable(GL_LIGHT0);
  // glEnable(GL_LIGHT1);
  glEnable(GL_LIGHTING);

  glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
}


//================================================================
  int tst_CB_b1 (GtkWidget *parent, void *data) {
//================================================================
// callback-function

  printf("tst_CB_b1 \n");

  return 0;

}

//================================================================
  int tst_CB_b2 (GtkWidget *parent, void *data) {
//================================================================
// callback-function

  printf("tst_CB_b2 \n");

  return 0;

}

//================== EOF ==================================
