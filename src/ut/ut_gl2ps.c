/* using 

 * GL2PS, an OpenGL to PostScript Printing Library
 * Copyright (C) 1999-2015 C. Geuzaine
 *

TODO:
- too slow for very complex surfaces ....


 */ 


#include "../gl2ps/gl2ps.h"

#include <math.h>
#include <string.h>
#include <sys/types.h>
#include <stdarg.h>
#include <time.h>
#include <float.h>




//==============================================================================
  int gl2ps_print3 (char *fNam, char *title, char *producer, void gl_redraw()) {
//==============================================================================
// Write file (postscript or pdf or svg) from current open-GL-window
// 2016-04-18   ReiterFranz
// Input:
//   fNam       outfilename; file-extension must be: "pdf"|"svg"|"eps"
//   title
//   producer
//   gl_redraw  function to redraw all openGl-objects
// Output:
//   file <fnam>
//   retCod     0=OK, else Error
//
// Example:
//   gl2ps_print3 ("test.eps", "test", "its me", display);
//     // display = pointer to function to redraw all openGl-objects

  int   i1, sort, format, opts, icol;
  int   state = GL2PS_OVERFLOW, buffsize = 0;
  FILE  *fp;
  char  ext[32], *cp1;
  GLint viewport[4];


  printf("gl2ps_print3 |%s|%s|%s|\n",fNam,title,producer);


  // extract ext from fNam
  cp1 = strrchr (fNam, '.');
  if(!cp1) {printf("**** gl2ps_print3 E001\n"); return -1;}
  ++cp1;
  strcpy(ext, cp1);


  // set format (GL2PS_PDF|GL2PS_SVG|GL2PS_TEX|GL2PS_PS)
  if(!strcmp ("pdf", ext)) {
    format = GL2PS_PDF;

  } else if(!strcmp ("eps", ext)) {
    format = GL2PS_EPS;

  } else if(!strcmp ("svg", ext)) {
    format = GL2PS_SVG;

  } else {printf("**** gl2ps_print3 E002\n"); return -2;}


  // sort = GL2PS_SIMPLE_SORT;
  sort = GL2PS_BSP_SORT;

  icol = 0;   // gray
  // opts = GL2PS_DRAW_BACKGROUND; // gray, no bitmaps

  // icol = 16;
  // opts = GL2PS_OCCLUSION_CULL;
  // opts = GL2PS_DRAW_BACKGROUND;
  // opts = GL2PS_BEST_ROOT;
  opts = GL2PS_DRAW_BACKGROUND | GL2PS_BEST_ROOT;
  // opts = GL2PS_DRAW_BACKGROUND | GL2PS_OCCLUSION_CULL | GL2PS_BEST_ROOT;

  // get window height, width
  glGetIntegerv (GL_VIEWPORT, viewport);

  // open outfile
  fp = fopen(fNam, "wb");
  if(!fp) {printf("**** gl2ps_print3 E003\n"); return -3;}


  (*gl_redraw) (); // draw all openGl-objects with white background ??

  // write outfile
  while (state == GL2PS_OVERFLOW) {
    // buffsize += 1024*1024;
    buffsize += 2056*2056;
    gl2psBeginPage (title, producer,
                    viewport,
                    format,
                    sort,
                    opts,
                    GL_RGBA, 0, NULL,
                    icol, icol, icol,
                    buffsize, fp, fNam);
    (*gl_redraw) ();           // redraw all openGl-objects
    state = gl2psEndPage();    // sort and write out
  }

  fclose(fp);

  return 0;

}


// EOF
