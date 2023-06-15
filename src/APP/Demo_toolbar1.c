//  Toolbar-Demo  (minimum)
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
2015-07-02 Created RF.

-----------------------------------------------------
*/
/*!
\file  ../APP/Demo_toolbar1.c
\brief   Toolbar-Demo  (minimum)
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
//---------------- external vars: ---------------------

// ../xa/xa.c
extern char APP_act_nam[128];    // name of active script- or plugin-program

// ex xa_ui.c:
extern MemObj    UIw_Box_TB;    // toolbarBox





//----------------------------------------------------------------
//---------------- local vars: ---------------------
static MemObj tbApp=GUI_OBJ_NEW;    // GIS-Toolbar
static MemObj wVal;



//----------------------------------------------------------------
//---------------- prototypes: ---------------------
  int ui_init (MemObj *parent);
  int ui_CB1 (MemObj *mo, void **data);
  int ui_CB2 (MemObj *mo, void **data);




//=========================================================
  int gCad_main () {
//=========================================================
// user has selected this plugin; starting ...

  TX_Print("gCad_main Demo_toolbar1");

  // init & display Toolbar
  ui_init (&UIw_Box_TB);

  return 0;

}


//=========================================================
  int gCad_fini () {
//=========================================================
// dll being unloaded - reset Input, kill all open windows !!!

  int    ii;

  TX_Print("...... gCad_fini Demo_toolbar1");

  // do nothing, if already unloaded.
  if(!GUI_OBJ_IS_VALID(&tbApp)) return 0;

  // kill toolbar
  tbApp = GUI_toolbox_del (&tbApp);

  AP_User_reset ();             // close application

  return 0;

}

//=========================================================
  int ui_init (MemObj *parent) {
//=========================================================
// init & display Toolbar

  MemObj box0, box2, wm0, wm1;
  char *optLst[]={"option 1",
                  "option 2",
                  NULL};


  printf("ui_ini \n");


  tbApp = GUI_toolbox__ (parent);

  box0 = GUI_box_v (&tbApp, "");
  box2 = GUI_box_h (&box0, "");

  // GUI_label_htm__ (&box2, "<b> Demo_toolbar1: </b>", "");
  GUI_label__ (&box2, "Demo_toolbar1 ", "l");


  wm0 = GUI_menu__ (&box2, NULL, 0);
  wm1 = GUI_menu__ (&wm0, "Menu", 0);
  GUI_menu_entry (&wm1, "menOpt 1", ui_CB1, (void*)"men1");
  GUI_menu_entry (&wm1, "menOpt 2", ui_CB1, (void*)"men2");


  // vertical separationline
  GUI_sep__ (&box2, 1, 6);

  GUI_optmen__  (&box2, optLst[0], optLst, NULL, ui_CB2, "");


  GUI_radiobutt__ (&box2, "rb0 ", 0, ui_CB1, (void*)"rb0", "");
  GUI_radiobutt__ (&box2, "rb1 ", 1, ui_CB1, (void*)"rb1", "");


  // vertical separationline
  GUI_sep__ (&box2, 1, 6);


  wVal = GUI_entry__ (&box2, " value: ", "", NULL, NULL, "9");
  GUI_entry_set (&wVal, "0.1");


  GUI_button__ (&box2, "Help", ui_CB1,    (void*)"Help", "");
  GUI_button__ (&box2, "Exit", ui_CB1,    (void*)"Exit", "");


  GUI_set_show (&tbApp, 1);    // show


  return 0;

}

//=====================================================================
  int ui_CB2 (MemObj *mo, void **data) {
//=====================================================================

  int   ii;
  char  *cp1;

  ii = GUI_DATA_I1;
  cp1 = GUI_DATA_S2;

  printf("ui_CB2 %d |%s|\n",ii,cp1);

  return 0;

}


//=====================================================================
  int ui_CB1 (MemObj *mo, void **data) {
//=====================================================================


  long l1;
  char *cp1, cbuf1[256];
  
  
  // cp1 = (char*)data;
  cp1 = GUI_DATA_S1;

  printf("ui_CB1  |%s|\n",cp1);

  

  //-------------------------------------------------
  if(!strcmp(cp1, "Help")) {
    APP_Help (APP_act_nam, "");

    cp1 = GUI_entry_get (&wVal);
    TX_Print (" Value is now %s\n",cp1);



  //-------------------------------------------------
  } else if(!strcmp(cp1, "Exit")) {
    gCad_fini ();
  }


  L_exit:
    return 0;


}


//================  EOF  ==================================
