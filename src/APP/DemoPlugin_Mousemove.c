// example user-callback from mousemove.
/*
 *
 * Copyright (C) 2015 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
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
  ..

-----------------------------------------------------
*/
/*!
\file  ../APP/DemoPlugin_Mousemove.c
\brief example user-callback from mousemove 
\code
=====================================================
List_functions_start:
..
List_functions_end:
=====================================================

\endcode *//*----------------------------------------


. ./devbase.sh && make -f DemoPlugin_Mousemove.mak


- user-callback mouse-buttons see ../APP/DemoPlugin_Selection.c
- user-callback KeyIn         see ../APP/DemoPlugin_KeyIn.c
- see also ../APP/DemoPlugin_Create.c

*/


// definition "export"
#include "../xa/export.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ut/ut_geo.h"              // DL_Att
#include "../ut/ut_geo_const.h"              // DL_Att
#include "../gr/ut_DL.h"               // DB_GetDLatt



//----------------------------------------------------------------
// EXPORTS to main-module
export int gCad_main ();
export int gCad_fini ();


//----------------------------------------------------------------
// externals:

// ../gr/ut_GL.c
extern double GL_Scr_Siz_X, GL_Scr_Siz_Y;




//----------------------------------------------------------------
// protos:
  int gui_CB_mm (int dx, int dy);                 // mousemove
  int gui_CB_key1 (int key);                      // keys







//=========================================================
  int gCad_main () {
//=========================================================


  // attach mousemovements
  AP_UserMousemove_get (gui_CB_mm);

  // attach KeyIn
  AP_UserKeyIn_get (gui_CB_key1);


  // display Info
  TX_Print("DemoPlugin_Mousemove: display mousecoords in consolewindow ..");
  TX_Print(">>> move mouse, press mousebuttons, key q = quit");



  return 0;
}


//=========================================================
  int gCad_fini () {
//=========================================================

  // display Info
  TX_Print(">>> exit DemoPlugin_Mousemove");

  AP_User_reset ();      // close application; resets callbacks

  return 0;

}


//=========================================================
  int gui_CB_mm (int dx, int dy) {
//=========================================================
// user-callback mouse-move

  int       xpos, ypos;
  Point     ptUsr, ptScr;
  Vector    vc2;



  // incremental mousemove in screencoords
  // printf("gui_CB_mm %d %d\n",dx,dy);

  // curPos in screen-coords
  GL_get_curPos_SC (&xpos, &ypos);
  printf(" src = %d %d\n", xpos, ypos);

  // scrSize in screencoords
  // printf("GL_Scr_Siz_X=%f GL_Scr_Siz_Y=%f\n", GL_Scr_Siz_X, GL_Scr_Siz_Y);

  // // curPos in world-coords on const-plane
  // GR_get_curPos_UC (&ptUsr);   // get GR_curPos_WC
  // printf(" usr = %f %f %f\n", ptUsr.x, ptUsr.y, ptUsr.z);


  // UTRA_pt_ucs2wcs (&pt1);
  // vc2 = GL_GetEyeX ();          // get vc2 =  eye-vector (GL_eyeX,GL_eyeY,GL_eyeZ)
  // GL_cen = centerpoint of the Viewport


  return 0;

}


//=========================================================
  int gui_CB_key1 (int key) {
//=========================================================
// userKeyin callback


  printf("gui_CB_key1 %d\n",key);

  switch (key) {

    case 'q':
      // DL_hili_off (-1L);         //  -1 = unhilite all
      // DL_Redraw ();               // Redraw DispList
      gCad_fini ();
      break;

    default:
      TX_Print("--- key %c pressed ..",key);
  }

  return 0;

}


//======================== EOF ======================
