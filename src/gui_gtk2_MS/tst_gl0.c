/* 

BASED ON GKTGLAREA of 1998 Janne Löf 

// test gtk + glx direct (without gtkgl, gtkglext, sdl).
// 2012-01-31  gtkglarea removed. RF


*/




#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>
#include <gtkgl/gtkglarea.h>

#include <GL/gl.h>
#include <GL/glu.h>



// dummy-funcs
int GUI_Tip (char *key) { return 0; }
int GUI_update__ () { return 0; }



gint init(GtkWidget *widget)
{
  /* OpenGL functions can be called only if make_current returns true */
  if (gtk_gl_area_make_current(GTK_GL_AREA(widget)))
    {
      glViewport(0,0, widget->allocation.width, widget->allocation.height);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(0,100, 100,0, -1,1);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
    }
  return TRUE;
}


/* When widget is exposed it's contents are redrawn. */
gint draw(GtkWidget *widget, GdkEventExpose *event)
{
  /* Draw only last expose. */
  if (event->count > 0)
    return TRUE;

  /* OpenGL functions can be called only if make_current returns true */
  if (gtk_gl_area_make_current(GTK_GL_AREA(widget)))
    {

      /* Draw simple triangle */
      glClearColor(0,0,0,1);
      glClear(GL_COLOR_BUFFER_BIT);
      glColor3f(1,1,1);
      glBegin(GL_TRIANGLES);
      glVertex2f(10,10);
      glVertex2f(10,90);
      glVertex2f(90,90);
      glEnd();

      /* Swap backbuffer to front */
      gtk_gl_area_swapbuffers(GTK_GL_AREA(widget));
        
    }

  return TRUE;
}

/* When glarea widget size changes, viewport size is set to match the new size */
gint reshape(GtkWidget *widget, GdkEventConfigure *event)
{
  /* OpenGL functions can be called only if make_current returns true */
  if (gtk_gl_area_make_current(GTK_GL_AREA(widget)))
    {
      glViewport(0,0, widget->allocation.width, widget->allocation.height);
    }
  return TRUE;
}


int main(int argc, char **argv)
{
  GtkWidget *window,*glarea;
  // gchar *info_str;

  /* Attribute list for gtkglarea widget. Specifies a
     list of Boolean attributes and enum/integer
     attribute/value pairs. The last attribute must be
     GDK_GL_NONE. See glXChooseVisual manpage for further
     explanation.
  */
  int attrlist[] = {
    GDK_GL_RGBA,
    GDK_GL_RED_SIZE,1,
    GDK_GL_GREEN_SIZE,1,
    GDK_GL_BLUE_SIZE,1,
    GDK_GL_DOUBLEBUFFER,
    GDK_GL_NONE
  };

  /* initialize gtk */
  gtk_init(&argc, &argv);


  /* Check if OpenGL is supported. */
// is not included in the dll .. ??
//if (gdk_gl_query() == FALSE) {
//  g_print("OpenGL not supported\n");
//  return 0;
//}


  /* Create new top level window. */
  window = gtk_window_new( GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Simple");
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  /* Quit form main if got delete event */
  gtk_signal_connect(GTK_OBJECT(window), "delete_event",
		     GTK_SIGNAL_FUNC(gtk_main_quit), NULL);


  /* You should always delete gtk_gl_area widgets before exit or else
     GLX contexts are left undeleted, this may cause problems (=core dump)
     in some systems.
     Destroy method of objects is not automatically called on exit.
     You need to manually enable this feature. Do gtk_quit_add_destroy()
     for all your top level windows unless you are certain that they get
     destroy signal by other means.
  */
  gtk_quit_add_destroy(1, GTK_OBJECT(window));


  /* Create new OpenGL widget. */
  glarea = GTK_WIDGET(gtk_gl_area_new(attrlist));
  /* Events for widget must be set before X Window is created */
  gtk_widget_set_events(GTK_WIDGET(glarea),
			GDK_EXPOSURE_MASK|
			GDK_BUTTON_PRESS_MASK);

  /* Connect signal handlers */
  /* Redraw image when exposed. */
  gtk_signal_connect(GTK_OBJECT(glarea), "expose_event",
		     GTK_SIGNAL_FUNC(draw), NULL);
  /* When window is resized viewport needs to be resized also. */
  gtk_signal_connect(GTK_OBJECT(glarea), "configure_event",
		     GTK_SIGNAL_FUNC(reshape), NULL);
  /* Do initialization when widget has been realized. */
  gtk_signal_connect(GTK_OBJECT(glarea), "realize",
		     GTK_SIGNAL_FUNC(init), NULL);

  /* set minimum size */
  gtk_widget_set_usize(GTK_WIDGET(glarea), 100,100);
		    
  /* put glarea into window and show it all */
  gtk_container_add(GTK_CONTAINER(window),GTK_WIDGET(glarea));
  gtk_widget_show(GTK_WIDGET(glarea));
  gtk_widget_show(GTK_WIDGET(window));

  /* vendor dependent version info string */
  // info_str = gdk_gl_get_info();
  // g_print(info_str);
  // g_free(info_str);


  printf("GL_RENDERER:    %s\n",glGetString(GL_RENDERER));
  printf("GL_VERSION:     %s\n",glGetString(GL_VERSION));
  printf("GL_EXTENSIONS:  %s\n",glGetString(GL_EXTENSIONS));

  printf("GLU_VERSION:    %s\n",gluGetString(GLU_VERSION));
  printf("GLU_EXTENSIONS: %s\n",gluGetString(GLU_EXTENSIONS));





  gtk_main();

  return 0;
}
