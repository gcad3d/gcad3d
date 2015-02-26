// test gtk + glx direct (without gtkgl, gtkglext, sdl).

// 2012-01-31  gtkglarea removed. RF

// Gtk2 + OpenGL:
// gcc -c `pkg-config --cflags gtk+-2.0` -DGTK2 tst_GL_gtk_1.c
// gcc tst_GL_gtk_1.o gtk_gl_x11.o `pkg-config --libs gtk+-2.0` -lGL
// ./a.out


// Gtk3 + OpenGL:
// gcc -c `pkg-config --cflags gtk+-3.0` -DGTK3 tst_GL_gtk_1.c
// gcc tst_GL_gtk_1.o gtk_gl_x11.o `pkg-config --libs gtk+-3.0` -lGL
// ./a.out


// -lGL glXChooseVisual



#include <stdio.h>
#include <stdlib.h>

#include <gtk/gtk.h>
#include <gtkgl/gtkglarea.h>

#include <GL/gl.h>
#include <GL/glu.h>
// #include <GL/glx.h>



/*
// gtk3_gl_x11:
  GLXContext  GLB_x_context;
  Display     *GLB_display;
  int         GLB_x_id;
*/

// prototypes:
  GtkWidget*  GLB_Create ();
  void        GLB_DrawExit ();

  int area_config (GtkWidget *widget, GdkEventConfigure *event, void *data);
  int area_realiz (GtkWidget *widget, void *data);
  int area_expose (GtkWidget *widget, GdkEventExpose *event, void *data);


// dummy-funcs
int GUI_Tip (char *key) { return 0; }
int GUI_update__ () { return 0; }




//===================================================================
  int main(int argc, char **argv) {
//===================================================================

  GtkWidget *window;
  GtkWidget *area;
  GtkWidget *vbox;
  GtkWidget *button;

  int         i1;


  printf("t1 main ----------------\n");

  gtk_init(&argc, &argv);

  // test if OpenGl is supported
  if(GLB_Query()) {printf("ERROR: OpenGl not supported.\n"); exit(0);}

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Test1 GLB (GTK3-OpenGL-binding)");

  // create opengl-area
  area = GLB_Create ();

  // connect opengl-callBack-functions
  g_signal_connect (area, "configure_event", G_CALLBACK (area_config), NULL);
  g_signal_connect (area, "realize", G_CALLBACK (area_realiz), NULL);

//Gtk2:
// #ifdef gtk2
  g_signal_connect (area, "expose_event", G_CALLBACK (area_expose), NULL);
// #endif

//Gtk3:
// #ifdef gtk3
  // g_signal_connect (area, "draw", G_CALLBACK (area_expose), NULL);
// #endif

  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  gtk_container_add (GTK_CONTAINER (window), area);
  gtk_widget_show_all (window);

  gtk_main ();

  return 0;

}


//==============================================================
  int area_expose (GtkWidget *widget, GdkEventExpose *event, void *data)
//==============================================================
{

  printf("area_expose\n");

  printf("  event->count %d\n",event->count);
  // if (event->count > 0) return TRUE;

  GLB_DrawInit (widget);

    glClear (GL_DEPTH_BUFFER_BIT);
  
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,100, 100,0, -1,1);
  
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1,1,1);
    glBegin(GL_TRIANGLES);
    glVertex2f(10,10);
    glVertex2f(10,90);
    glVertex2f(90,90);
    glEnd();

  GLB_DrawExit ();

  return TRUE;
}


//==============================================================
  int area_config (GtkWidget *widget, GdkEventConfigure *event, void *data) {
//==============================================================

  GtkAllocation allocation;

  printf("area_config\n");

  GLB_DrawInit (widget);

    gtk_widget_get_allocation (widget, &allocation);
    glViewport (0, 0, allocation.width, allocation.height);

  return TRUE;
}


//==============================================================
  int area_realiz (GtkWidget *widget, void *data)
//==============================================================
{

  printf("area_realiz\n");

  GLB_DrawInit (widget);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);
    glEnable (GL_CULL_FACE);
    glCullFace (GL_BACK);
    glDisable (GL_DITHER);
    glShadeModel (GL_SMOOTH);

  return TRUE;
}


// EOF
