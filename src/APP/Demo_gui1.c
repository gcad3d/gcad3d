//  Simple-Gtk-Demo  (minimum)
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
\file  ../APP/DemoGtk1.c
\brief  Simple GUI Demo  (minimum)
\code
=====================================================
List_functions_start:
..
List_functions_end:
=====================================================

\endcode *//*----------------------------------------


*/


// definition "export"
#include "../xa/export.h"


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_cast.h"             // INT__PTR
#include "../ut/ut_types.h"               // INT_8 - UINT_64
#include "../ut/ut_umem.h"                // Memspc MemObj UME_*

#include "../gui/gui__.h"


//----------------------------------------------------------------
// EXPORTS to main-module
export int gCad_main ();
export int gCad_fini ();


//----------------------------------------------------------------
  static MemObj win0;





//=========================================================
  int gCad_main () {
//=========================================================
// user has selected this plugin; starting ...


  TX_Print("gCad_main DemoGtk1");

  win1__ (NULL, GUI_SETDAT_E (UI_FuncInit));  // create window
    // set EVENT=UI_FuncInit; see ../gui/gui_types.h

  return 0;

}


//=========================================================
  int gCad_fini () {
//=========================================================
// dll being unloaded - reset Input, kill all open windows !!!

  TX_Print("...... gCad_fini DemoGtk1");

  GUI_Win_kill (&win0);

  AP_User_reset ();                      // close application

  return 0;

}

//=========================================================
  int win1__ (MemObj *mo, void **data) {
//=========================================================

  MemObj box0;
  char   *cp1;



  printf("win1__ %d\n",GUI_DATA_EVENT);



  switch (GUI_DATA_EVENT) {

    case UI_FuncInit:
      win0 = GUI_Win__ ("Gtk1", win1__, "");
      box0 = GUI_box_v (&win0, "");
      GUI_button__ (&box0, "Test Button", win1__, &GUI_FuncUCB1, "");
      GUI_Win_go (&win0);
      break;

    case TYP_EventPress:                    // 99
      if(GUI_DATA_I1 == UI_FuncUCB1) {
        printf(" do UI_FuncUCB1\n");
      }
      break;

    case TYP_EventExit:     // from GUI_Win__
      gCad_fini ();
      break;

  }

  return 0;

}


//================  EOF  ==================================
