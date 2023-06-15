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


make -f DemoPlugin_Mousemove.mak

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
static long last_obj;


// protos:
  int dmm_key1 (int key);
  int dmm_sele1 (int src, long dl_ind);
  int dmm_mm (int dx, int dy);








//=========================================================
  int gCad_main () {
//=========================================================


  // attach KeyIn
  AP_UserKeyIn_get (dmm_key1);

  // attach grafic selections
  AP_UserSelection_get (dmm_sele1);

  // attach mousemovements
  AP_UserMousemove_get (dmm_mm);

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

  AP_User_reset ();      // close application

  return 0;

}



//=========================================================
  int dmm_mm (int dx, int dy) {
//=========================================================
// userselection callback

  Point pt1;

  printf("dmm_mm %d %d\n",dx,dy);

  // GR_get_curPos_UC (&pt1);   // get GR_curPos_WC
  // printf(" pt1= %f %f %f\n",pt1.x,pt1.y,pt1.z);

  return 0;

}




//=========================================================
  int dmm_sele1 (int src, long dl_ind) {
//=========================================================
// userselection callback

  printf("dmm_sele1 source=%d ind=%ld\n",src,dl_ind);

  return 0;

}



//=========================================================
  int dmm_key1 (int key) {
//=========================================================
// userKeyin callback


  printf("dmm_key1 %d\n",key);

  switch (key) {

    case 'q':
      DL_hili_off (-1L);         //  -1 = unhilite all
      DL_Redraw ();               // Redraw DispList
      gCad_fini ();
      break;

  }

  return 0;

}




//======================== EOF ======================
