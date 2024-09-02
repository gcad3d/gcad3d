// example user-callback from keypress in grafic-window;
/*
 *
 * Copyright (C) 2024 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
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
\brief example user-callback from keypress in grafic-window 
\code
=====================================================
List_functions_start:
..
List_functions_end:
=====================================================

\endcode *//*----------------------------------------


. ./devbase.sh && make -f DemoPlugin_KeyIn.mak


- user-callback mouse-buttons see ../APP/DemoPlugin_Selection.c
- user-callback mousemove     see ../APP/DemoPlugin_Mousemove.c
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
// protos:
  int gui_CB_key1 (int key);




//=========================================================
  int gCad_main () {
//=========================================================


  // attach KeyIn
  AP_UserKeyIn_get (gui_CB_key1);

  // display Info
  TX_Print("--- start DemoPlugin_KeyIn; key q = quit ---");

  return 0;
}




//=========================================================
  int gCad_fini () {
//=========================================================

  // display Info
  TX_Print(">>> exit DemoPlugin_KeyIn");

  AP_User_reset ();      // close application
  // does AP_UserKeyIn_reset - reset KeyIn;

  return 0;

}


//=========================================================
  int gui_CB_key1 (int key) {
//=========================================================
// userKeyin callback


  printf("gui_CB_key1 %d\n",key);

  switch (key) {

    case 'q':
      DL_hili_off (-1L);         //  -1 = unhilite all
      DL_Redraw ();               // Redraw DispList
      gCad_fini ();
      break;

    default:
      TX_Print("--- key %c pressed ..",key);
  }

  return 0;

}



//======================== EOF ======================
