/*
 * Copyright (C) 1997-1998 Janne Löf <jlof@mail.student.oulu.fi>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "config.h"

#include <stdarg.h>

#include <gtkgl/gdkgl.h>
#include <gtkgl/gtkglarea.h>

static void gtk_gl_area_class_init    (GtkGLAreaClass *klass);
static void gtk_gl_area_init          (GtkGLArea      *glarea);
static void gtk_gl_area_destroy       (GtkObject      *object); /* change to finalize? */

static GtkDrawingAreaClass *parent_class = NULL;



//================================================================
GType gtk_gl_area_get_type (void)
//================================================================
{
  static GType object_type = 0;

  // printf("gtk_gl_area_get_type\n");

  if (!object_type)
    {
      static const GTypeInfo object_info =
      {
        sizeof (GtkGLAreaClass),
        (GBaseInitFunc) NULL,
        (GBaseFinalizeFunc) NULL,
        (GClassInitFunc) gtk_gl_area_class_init,
        NULL,           /* class_finalize */
        NULL,           /* class_data */
        sizeof (GtkGLArea),
        0,              /* n_preallocs */
        (GInstanceInitFunc) gtk_gl_area_init,
      };

      object_type = g_type_register_static (GTK_TYPE_DRAWING_AREA,
                                            "GtkGLArea",
                                            &object_info, 0);
    }
  return object_type;
}


//================================================================
static void gtk_gl_area_class_init (GtkGLAreaClass *klass)
//================================================================
{
  GtkObjectClass *object_class;
// 
  printf("gtk_gl_area_class_init \n");

  parent_class = g_type_class_peek_parent(klass);
  object_class = (GtkObjectClass*) klass;

  object_class->destroy = gtk_gl_area_destroy;
}


//================================================================
static void gtk_gl_area_init (GtkGLArea *gl_area)
//================================================================
{
  // printf("gtk_gl_area_init \n");
  gl_area->glcontext = NULL;
  gtk_widget_set_double_buffered(GTK_WIDGET(gl_area), FALSE);
}


//================================================================
GtkWidget* gtk_gl_area_new_vargs(GtkGLArea *share, ...)
//================================================================
{
  GtkWidget *glarea;
  va_list ap;
  int i;
  gint *attrlist;

  // printf("gtk_gl_area_new_vargs \n");


  va_start(ap, share);
  i=1;
  while (va_arg(ap, int) != GDK_GL_NONE) // get number of arguments
    i++;
  va_end(ap);

  attrlist = g_new(int,i);

  va_start(ap,share);
  i=0;
  while ( (attrlist[i] = va_arg(ap, int)) != GDK_GL_NONE) // copy args to list
    i++;
  va_end(ap);

  glarea = gtk_gl_area_share_new(attrlist, share);

  g_free(attrlist);

  return glarea;
}


//================================================================
GtkWidget* gtk_gl_area_new (int *attrlist)
//================================================================
{
  // printf("gtk_gl_area_new \n");

  return gtk_gl_area_share_new (attrlist, NULL);
}


//================================================================
GtkWidget* gtk_gl_area_share_new (int *attrlist, GtkGLArea *share)
//================================================================
{
  static GdkGLContext *glcontext;
  static GtkGLArea *gl_area;


  // printf("gtk_gl_area_share_new \n");

  g_return_val_if_fail(share == NULL || GTK_IS_GL_AREA(share), NULL);

  glcontext = gdk_gl_context_attrlist_share_new (
      attrlist,
      share ? share->glcontext : NULL,
      TRUE);
  if (glcontext == NULL)
    return NULL;

  gl_area = g_object_new (GTK_TYPE_GL_AREA, NULL);
  gl_area->glcontext = glcontext;


  return GTK_WIDGET (gl_area);
}


//================================================================
static void gtk_gl_area_destroy(GtkObject *object)
//================================================================
{
  GtkGLArea *gl_area;

  // printf("gtk_gl_area_destroy \n");

  g_return_if_fail (object != NULL);
  g_return_if_fail (GTK_IS_GL_AREA(object));

  gl_area = GTK_GL_AREA(object);

  if (gl_area->glcontext)
    g_object_unref(gl_area->glcontext);
  gl_area->glcontext = NULL;

  if (GTK_OBJECT_CLASS (parent_class)->destroy)
    (* GTK_OBJECT_CLASS (parent_class)->destroy) (object);
}


//================================================================
gint gtk_gl_area_make_current(GtkGLArea *gl_area)
//================================================================
{
  // g_return_val_if_fail(gl_area != NULL, FALSE);
  // g_return_val_if_fail(GTK_IS_GL_AREA (gl_area), FALSE);
  // g_return_val_if_fail(GTK_WIDGET_REALIZED(gl_area), FALSE);

  // printf("gtk_gl_area_make_current \n");

  return gdk_gl_make_current(GTK_WIDGET(gl_area)->window, gl_area->glcontext);
}


//================================================================
void gtk_gl_area_swap_buffers (GtkGLArea *gl_area)
// void gtk_gl_area_swap_buffers (GtkWidget *gl_area)
//================================================================
{

  // g_return_if_fail(gl_area != NULL);
  // g_return_if_fail(GTK_IS_GL_AREA(gl_area));
  // g_return_if_fail(GTK_WIDGET_REALIZED(gl_area));

  // printf("gtk_gl_area_swap_buffers \n");

  gdk_gl_swap_buffers (GTK_WIDGET(gl_area)->window);
  // gdk_gl_swap_buffers (gl_area->window);

}

// EOF
