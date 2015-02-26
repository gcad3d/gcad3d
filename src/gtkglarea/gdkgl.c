/*
 * Copyright (C) 1998 Janne Löf <jlof@mail.student.oulu.fi>
 *           (c) 2008, 2009 Sam Hocevar <sam@hocevar.net>
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

#include <string.h>

// #include "gdkgl.h"
#include <gtkgl/gdkgl.h>

#include <GL/gl.h>

//if defined GDK_WINDOWING_WIN32
#include <gdk/gdkwin32.h>
#define PLATFORM "GDK_WINDOWING_WIN32"


/*
 *  The GdkGLContext class
 */
struct _GdkGLContext {
  GObject     parent;
  gboolean  initialised;
  HGLRC     hglrc;
  HDC       hdc;
  HWND      hwnd;
  GdkGLContext *share;
  PIXELFORMATDESCRIPTOR pfd;
};

struct _GdkGLContextClass {
  GObjectClass parent_class;
};
typedef struct _GdkGLContextClass GdkGLContextClass;

static GObjectClass *glcontext_parent_class;
static void gdk_gl_context_class_init (GdkGLContextClass *class);

/*
 *  The GdkGLPixmap class
 */
struct _GdkGLPixmap {
  GObject   parent;
  gboolean  initialised;
  HDC       hdc;
  HBITMAP   hbitmap;
  GdkPixmap *pixmap;
};

struct _GdkGLPixmapClass {
  GObjectClass parent_class;
};
typedef struct _GdkGLPixmapClass GdkGLPixmapClass;

static GObjectClass *glpixmap_parent_class;
static void gdk_gl_pixmap_class_init (GdkGLPixmapClass *class);

/*
 *  Local helper functions
 */
static void fill_pfd(PIXELFORMATDESCRIPTOR *pfd, int *attriblist);



//================================================================
//  Generic GL support
gint gdk_gl_query(void)
{
  // test for unresolved
  // cannot_be_resolved ();

  return TRUE;
}


//================================================================
gchar *gdk_gl_get_info()
{
  char const *vendor, *version, *extensions;

  vendor = glGetString (GL_VENDOR);
  version = glGetString (GL_VERSION);
  extensions = glGetString (GL_EXTENSIONS);
  return g_strdup_printf("VENDOR     : %s\n"
                         "VERSION    : %s\n"
                         "EXTENSIONS : %s\n",
                         vendor, version, extensions);
}


//================================================================
GdkVisual *gdk_gl_choose_visual(int *attrlist)
{

  // printf("gdk_gl_choose_visual \n");

  return gdk_visual_get_system ();

}


//================================================================
int gdk_gl_get_config(GdkVisual *visual, int attrib)
{
  g_warning ("gdk_gl_get_config not implemented on " PLATFORM);
  return 0;
}



//================================================================
GType gdk_gl_context_get_type (void)
//================================================================
{
  static GType object_type = 0;

  // printf("gdk_gl_context_get_type \n");

  if (!object_type)
    {
      static const GTypeInfo object_info =
      {
        sizeof (GdkGLContextClass),
        (GBaseInitFunc) NULL,
        (GBaseFinalizeFunc) NULL,
        (GClassInitFunc) gdk_gl_context_class_init,
        NULL,           /* class_finalize */
        NULL,           /* class_data */
        sizeof (GdkGLContext),
        0,              /* n_preallocs */
        (GInstanceInitFunc) NULL,
      };

      object_type = g_type_register_static (G_TYPE_OBJECT,
                                            "GdkGLContext",
                                            &object_info, 0);
    }
  return object_type;
}


//================================================================
static void gdk_gl_context_finalize (GObject *object)
{
  GdkGLContext *context;

  // printf("gdk_gl_context_finalize \n");

  context = GDK_GL_CONTEXT(object);

  if (context->hglrc == wglGetCurrentContext ())
    wglMakeCurrent (NULL, NULL);

  wglDeleteContext (context->hglrc);

  if (context->hwnd)
    ReleaseDC (context->hwnd, context->hdc);
  else
    DeleteDC (context->hdc);

  (* glcontext_parent_class->finalize)(object);
}


//================================================================
static void gdk_gl_context_class_init (GdkGLContextClass *class)
{
  GObjectClass *gobject_class;

  // printf("gdk_gl_context_class_init \n");

  gobject_class = G_OBJECT_CLASS(class);

  glcontext_parent_class = g_type_class_peek_parent (class);

  gobject_class->finalize = gdk_gl_context_finalize;
}


//================================================================
GdkGLContext * gdk_gl_context_new (GdkVisual *visual)
{
  // printf("gdk_gl_context_new \n");

  return gdk_gl_context_share_new(visual, NULL, FALSE);
}


//================================================================
GdkGLContext* 
gdk_gl_context_share_new (GdkVisual *visual, GdkGLContext *sharelist, gint direct)
{
  GdkGLContext *context;

  // printf("gdk_gl_context_share_new \n");

  g_return_val_if_fail (visual != NULL, NULL);

  context = g_object_new (GDK_TYPE_GL_CONTEXT, NULL);
  if (!context) return NULL;


  context->initialised = FALSE;
  context->hglrc   = NULL;
  context->hdc     = NULL;
  context->hwnd    = NULL;
  context->share   = sharelist ? g_object_ref(sharelist) : NULL;

  memset (&(context->pfd), 0, sizeof(PIXELFORMATDESCRIPTOR));

  /* if direct is TRUE, we create a context which renders to the screen,
     otherwise we create one to render to an offscreen bitmap */
  context->pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  context->pfd.nVersion = 1;
  if (direct)
    context->pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
  else
    context->pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_BITMAP | PFD_SUPPORT_GDI;

  context->pfd.iPixelType = PFD_TYPE_RGBA;
  context->pfd.cColorBits = 24;
  context->pfd.cDepthBits = 24; //32;
  context->pfd.iLayerType = PFD_MAIN_PLANE;

  return context;
}


//================================================================
GdkGLContext *gdk_gl_context_attrlist_share_new (int *attrlist, GdkGLContext *sharelist, gint direct)
{
  GdkGLContext *context;

  // printf("gdk_gl_context_attrlist_share_new \n");

  g_return_val_if_fail(attrlist != NULL, NULL);

  context = g_object_new(GDK_TYPE_GL_CONTEXT, NULL);
  if (!context) return NULL;

  context->initialised = FALSE;
  context->hglrc    = NULL;
  context->hdc      = NULL;
  context->hwnd     = NULL;
  context->share    = sharelist ? g_object_ref(sharelist) : NULL;

  fill_pfd (&context->pfd, attrlist);

  return context;
}


//================================================================
  int gdk_gl_init_style () {
//================================================================
// WS_CLIPCHILDREN | WS_CLIPSIBLINGS CS_PARENTDC


  WNDCLASSEX wcl;

  // printf("gdk_gl_init_style \n");
  // printf("  CS_PARENTDC = %d\n",CS_PARENTDC);


  GetClassInfoEx (NULL, "gdkWindowChild", &wcl);

  // printf(" wcl.style=%d\n",wcl.style);

  wcl.style &= ~CS_PARENTDC;

  return 0;

}


//================================================================
gint gdk_gl_make_current (GdkDrawable *drawable, GdkGLContext *context)
{

  // printf("gdk_gl_make_current \n");

  g_return_val_if_fail (GDK_IS_DRAWABLE(drawable), FALSE);
  g_return_val_if_fail (GDK_IS_GL_CONTEXT(context), FALSE);

  if (!context->initialised)
  {
    int pf;
    HWND hwnd = (HWND) gdk_win32_drawable_get_handle (drawable);

    context->hdc = GetDC (hwnd);

    pf = ChoosePixelFormat (context->hdc, &context->pfd);

    if (pf != 0)
      {
        SetPixelFormat (context->hdc, pf, &context->pfd);
        context->hglrc = wglCreateContext (context->hdc);
      }

    if (context->share)
      {
        if (context->share->hglrc)
          {
            if (wglShareLists (context->share->hglrc, context->hglrc) != TRUE)
                g_warning ("failed sharing context");
          }
        g_object_unref (context->share);
      }

    context->initialised = TRUE;
  }

  g_return_val_if_fail (context->hdc    != NULL, FALSE);
  g_return_val_if_fail (context->hglrc  != NULL, FALSE);

  wglMakeCurrent (context->hdc, context->hglrc);

  return TRUE;

}


//================================================================
void gdk_gl_swap_buffers (GdkDrawable *drawable)
//================================================================
{
  static HDC   hdc = NULL;
  static HWND  hwnd;

  // printf("gdk_gl_swap_buffers \n");

  // g_return_if_fail (GDK_IS_DRAWABLE(drawable));

  if (hdc  == NULL) {
    hwnd = (HWND) gdk_win32_drawable_get_handle (drawable);
    hdc  = GetDC (hwnd);
  }

  if (hdc  == NULL) {
     g_warning ("gdk_gl_swap_buffers: GetDC failed");
     return;
  }

  SwapBuffers (hdc);

  // InvalidateRect (hwnd, NULL, FALSE);  //loop!

  // ReleaseDC (hwnd, hdc);

}


//================================================================
void gdk_gl_wait_gdk(void)
{
  GdiFlush();
}


//================================================================
void gdk_gl_wait_gl (void)
{
  glFinish();
}


//================================================================
GType gdk_gl_pixmap_get_type (void)
{
  static GType object_type = 0;

  // printf("gdk_gl_pixmap_get_type \n");

  if (!object_type)
    {
      static const GTypeInfo object_info =
      {
        sizeof (GdkGLPixmapClass),
        (GBaseInitFunc) NULL,
        (GBaseFinalizeFunc) NULL,
        (GClassInitFunc) gdk_gl_pixmap_class_init,
        NULL,           /* class_finalize */
        NULL,           /* class_data */
        sizeof (GdkGLPixmap),
        0,              /* n_preallocs */
        (GInstanceInitFunc) NULL,
      };

      object_type = g_type_register_static (G_TYPE_OBJECT,
                                            "GdkGLPixmap",
                                            &object_info, 0);
    }
  return object_type;
}


//================================================================
static void
gdk_gl_pixmap_finalize(GObject *object)
{
  GdkGLPixmap *pixmap;

  // printf("gdk_gl_pixmap_finalize \n");

  pixmap = GDK_GL_PIXMAP(object);

  glFinish ();
  SelectObject (pixmap->hdc, pixmap->hbitmap);
  gdk_pixmap_unref (pixmap->pixmap);

  (* glcontext_parent_class->finalize)(object);
}


//================================================================
static void
gdk_gl_pixmap_class_init(GdkGLPixmapClass *class)
{
  GObjectClass *gobject_class;

  // printf("gdk_gl_pixmap_class_init \n");

  gobject_class = G_OBJECT_CLASS(class);
  glpixmap_parent_class = g_type_class_peek_parent(class);

  gobject_class->finalize = gdk_gl_pixmap_finalize;
}


//================================================================
GdkGLPixmap * gdk_gl_pixmap_new (GdkVisual *visual, GdkPixmap *pixmap)
{
  GdkGLPixmap *glpixmap;
  unsigned int w_ret, h_ret, bw_ret, depth_ret;
  int x_ret, y_ret;

  // printf("gdk_gl_pixmap_new \n");

  g_return_val_if_fail(GDK_IS_VISUAL(visual), NULL);
  g_return_val_if_fail(GDK_IS_PIXMAP(pixmap), NULL);

  glpixmap = g_object_new(GDK_TYPE_GL_PIXMAP, NULL);
  if (!glpixmap)
    return NULL;

  glpixmap->initialised = FALSE;
  glpixmap->hdc = NULL;
  glpixmap->hbitmap = NULL;
  glpixmap->pixmap = gdk_pixmap_ref (pixmap);

  return glpixmap;
}


//================================================================
gint gdk_gl_pixmap_make_current (GdkGLPixmap *glpixmap, GdkGLContext *context)
{
  // Display  *dpy;
  // GLXPixmap glxpixmap;
  // GLXContext glxcontext;

  // printf("gdk_gl_pixmap_make_current \n");

  g_return_val_if_fail (GDK_IS_GL_PIXMAP(glpixmap), FALSE);
  g_return_val_if_fail (GDK_IS_GL_CONTEXT(context), FALSE);

  if (!context->initialised)
  {
    int pf;

    context->hdc = CreateCompatibleDC (NULL);
    glpixmap->hdc = context->hdc;
    glpixmap->hbitmap = SelectObject (context->hdc, (HBITMAP) gdk_win32_drawable_get_handle (glpixmap->pixmap));

    pf = ChoosePixelFormat (context->hdc, &context->pfd);

    if (pf != 0)
      {
        SetPixelFormat (context->hdc, pf, &context->pfd);
        context->hglrc = wglCreateContext (context->hdc);
      }

    if (context->share)
      {
        if (context->share->hglrc)
          {
            if (wglShareLists (context->share->hglrc, context->hglrc) != TRUE)
                g_warning ("failed sharing context");
          }
        gdk_gl_context_unref ((GdkGLContext*)context->share);
      }

    context->initialised = TRUE;
  }

  g_return_val_if_fail (context->hdc    != NULL, FALSE);
  g_return_val_if_fail (context->hglrc  != NULL, FALSE);

  wglMakeCurrent (context->hdc, context->hglrc);

  return TRUE;
}

/*
 *  Font support
 */


//================================================================
void gdk_gl_use_gdk_font(GdkFont *font, int first, int count, int list_base)
{
  HDC dc = CreateCompatibleDC (NULL);
  HFONT old_font = SelectObject (dc, (void *)gdk_font_id (font));

  // printf("gdk_gl_use_gdk_font \n");

  wglUseFontBitmaps (dc, first, count, list_base);

  SelectObject (dc, old_font);
  DeleteDC (dc);
}


//================================================================
static void fill_pfd (PIXELFORMATDESCRIPTOR *pfd, int *attriblist)
{
  /*
   * Ripped from glut's win32_x11.c
   */

  int *p = attriblist;

  // printf("fill_pfd \n");


  memset(pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
  pfd->nSize = (sizeof(PIXELFORMATDESCRIPTOR));
  pfd->nVersion = 1;

  /* Defaults. */
  pfd->dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
  pfd->iPixelType = PFD_TYPE_COLORINDEX;
  pfd->cColorBits = 24;
  pfd->cDepthBits = 24; //0;
  pfd->cAccumBits = 0;

  while (*p) {
    switch (*p) {
    case GDK_GL_USE_GL:
      pfd->dwFlags |= PFD_SUPPORT_OPENGL;
      break;
    case GDK_GL_BUFFER_SIZE:
      pfd->cColorBits = *(++p);
      break;
    case GDK_GL_LEVEL:
      /* the bReserved flag of the pfd contains the
         overlay/underlay info. */
      pfd->bReserved = *(++p);
      break;
    case GDK_GL_RGBA:
      pfd->iPixelType = PFD_TYPE_RGBA;
      break;
    case GDK_GL_DOUBLEBUFFER:
      pfd->dwFlags |= PFD_DOUBLEBUFFER;
      break;
    case GDK_GL_STEREO:
      pfd->dwFlags |= PFD_STEREO;
      break;
    case GDK_GL_AUX_BUFFERS:
      pfd->cAuxBuffers = *(++p);
      break;
    case GDK_GL_RED_SIZE:
      pfd->cRedBits = 8; /* Try to get the maximum. */
      ++p;
      break;
    case GDK_GL_GREEN_SIZE:
      pfd->cGreenBits = 8;
      ++p;
      break;
    case GDK_GL_BLUE_SIZE:
      pfd->cBlueBits = 8;
      ++p;
      break;
    case GDK_GL_ALPHA_SIZE:
      pfd->cAlphaBits = 8;
      ++p;
      break;
    case GDK_GL_DEPTH_SIZE:
      pfd->cDepthBits = 24; //32;
      ++p;
      break;
    case GDK_GL_STENCIL_SIZE:
      pfd->cStencilBits = *(++p);
      break;
    case GDK_GL_ACCUM_RED_SIZE:
    case GDK_GL_ACCUM_GREEN_SIZE:
    case GDK_GL_ACCUM_BLUE_SIZE:
    case GDK_GL_ACCUM_ALPHA_SIZE:
      /* I believe that WGL only used the cAccumRedBits,
         cAccumBlueBits, cAccumGreenBits, and cAccumAlphaBits fields
         when returning info about the accumulation buffer precision.
         Only cAccumBits is used for requesting an accumulation
         buffer. */
      pfd->cAccumBits += *(++p);
                break;
    }
    ++p;
  }
}


// EOF
