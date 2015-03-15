// GL_C-KIT.c
/*
 *
 * Copyright (C) 2015 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
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
2012-08-26 Extracted from GL_glarea.c. RF.

-----------------------------------------------------
*/
/*!
\file  ../gui_gtk2_MS/GL_C-KIT.c
\brief GTK-OpenGl-Bindings with lib ckitgui
\code
=====================================================
List_functions_start:

// UI_GR_DrawConfig      .
// UI_GR_DrawQuery       .
// UI_GR_DrawCreate      .
UI_GR_DrawInit        .
UI_GR_DrawExit        .

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



---------------------------------------------------
Compile:
cc -c GL_glarea.c `gtk-config --cflags`
cc -c GL_glarea.c `pkg-config --cflags gtkgl-2.0`

*/

//#include "MS_Def0.h"

#ifdef _MSC_VER
#include <windows.h>
#endif


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/gl.h>

#include "../ut/ut_umem.h"                     // Memspc


// EXTERNALS:
// ex ../xa/xa_ui_gr.c
extern MemObj    winGR;


// local vars
static int UI_GR_STAT = -1;






//================================================================
  void UI_GR_DrawInit () {
//================================================================
// 2007-04-20: fuer Linux nach wie vor erforderlich !!

static int iStat = -1;   // 0=

  // printf("{{{{{ UI_GR_DrawInit\n");

  if(UI_GR_STAT != 1) {
    UI_GR_STAT = 1;
    // gtk_gl_area_make_current (GTK_GL_AREA(GL_widget));
    GUI_gl_set_active (1, &winGR);

  } else {
    printf("UI_GR_DrawInit E001\n");
  }

  return;
}


//================================================================
  void UI_GR_DrawExit () {
//================================================================
// swap buffers (activate last drawing)

  // printf("}}}}} UI_GR_DrawExit\n");

  if(UI_GR_STAT != 0) {
    UI_GR_STAT = 0;
    // gtk_gl_area_swap_buffers (GTK_GL_AREA(GL_widget));
    GUI_gl_set_active (0, &winGR);

  } else {
    printf("!!!!!!!!!!!!!!!!!!!! UI_GR_DrawExit E001\n");
  }

  return;
}


//============= EOF ==============================================
