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
static MemObj wsl1;   // slider

static double  cycStat;           // slider value


//----------------------------------------------------------------
//---------------- prototypes: ---------------------
  int ui_init (MemObj *parent);
  int ui_CB1 (MemObj *mo, void **data);
  int ui_CB2 (MemObj *mo, void **data);
  int sli_CB_1 (MemObj *mo, void **data);




//=========================================================
  int gCad_main () {
//=========================================================
// user has selected this plugin; starting ...

  TX_Print("gCad_main Demo_toolbar3");

  // init & display Toolbar
  ui_init (&UIw_Box_TB);

  return 0;

}


//=========================================================
  int gCad_fini () {
//=========================================================
// dll being unloaded - reset Input, kill all open windows !!!

  int    ii;

  TX_Print("...... gCad_fini Demo_toolbar3");

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


  // text for menu - terminate with NULL;
  char *optLst[]={"option 1",
                  "option 2",
                  NULL};


  printf("ui_ini \n");


  tbApp = GUI_toolbox__ (parent);

  box0 = GUI_box_v (&tbApp, "");
  box2 = GUI_box_h (&box0, "");   // the horizontal box with all inputs

  // GUI_label_htm__ (&box2, "<b> Demo_toolbar1: </b>", "");
  GUI_label__ (&box2, "Demo_toolbar1 ", "l");


  // menu with callbacks
  wm0 = GUI_menu__ (&box2, NULL, 0);
  wm1 = GUI_menu__ (&wm0, "Menu", 0);
  GUI_menu_entry (&wm1, "menOpt 1", ui_CB1, (void*)"men1");
  GUI_menu_entry (&wm1, "menOpt 2", ui_CB1, (void*)"men2");


  // vertical separationline
  GUI_sep__ (&box2, 1, 6);


  // simple optionmenu with callbacks
  GUI_optmen__  (&box2, optLst[0], optLst, NULL, ui_CB2, "");


  // radiobuttons: only one can be active
  GUI_radiobutt__ (&box2, "rb0 ", 0, ui_CB1, (void*)"rb0", "");
  GUI_radiobutt__ (&box2, "rb1 ", 1, ui_CB1, (void*)"rb1", "");



  // disp text
  GUI_label__ (&box2, "mySlider", "r");


  // slider minVal = 0.; maxVal=1.; startVal=0.5, size=10-chars
  wsl1 = GUI_Slider__ (&box2, 0, sli_CB_1, 0., 1., 0.5, "10");
  // display info (keep cursor 2 seconds above slider)
  GUI_Tip  ("increment / decrement value ..");


  // vertical separationline
  GUI_sep__ (&box2, 1, 6);


  // entryfield for textinput, size 9 chars
  wVal = GUI_entry__ (&box2, " value: ", "", NULL, NULL, "9");
  GUI_entry_set (&wVal, "0.1");


  // buttons with callback
  GUI_button__ (&box2, "Help", ui_CB1,    (void*)"Help", "");
  GUI_button__ (&box2, "Exit", ui_CB1,    (void*)"Exit", "");


  // show
  GUI_set_show (&tbApp, 1);

  // GUI_Slider_get

  return 0;

}


//=========================================================
  int sli_CB_1 (MemObj *mo, void **data) {
//=========================================================
// user-has-moved-slider-callback

  char      *cp1;
  char      cbuf[512];


  // printf("Ani_CB_2 %f\n",gtk_adjustment_get_value(adj1));

  if(GUI_DATA_EVENT != TYP_EventMove) return 0;  // 2013-09-10

  // cycStat = gtk_adjustment_get_value(adj1);
  cycStat = GUI_DATA_D1;
    printf(" sli_CB_1 new val %f\n",cycStat);


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


  long     l1;
  double   d1;
  char     *cp1, cbuf1[256];
  
  
  // cp1 = (char*)data;
  cp1 = GUI_DATA_S1;

  printf("ui_CB1  |%s|\n",cp1);

  

  //-------------------------------------------------
  if(!strcmp(cp1, "Help")) {
    APP_Help (APP_act_nam, "");

    cp1 = GUI_entry_get (&wVal);
    TX_Print (" Value is now %s\n",cp1);



  //-------------------------------------------------
  } else if(!strcmp(cp1, "rb0")) {
    d1 = GUI_Slider_get (&wsl1);
    printf(" act. slider-val is %f\n",d1);
   

  //-------------------------------------------------
  } else if(!strcmp(cp1, "rb1")) {
    GUI_Slider_set (&wsl1, 0.5);


  //-------------------------------------------------
  } else if(!strcmp(cp1, "Exit")) {
    gCad_fini ();


  //-------------------------------------------------
  } else TX_Print ("***** ERROR ui_CB1 |%s|",cp1);


  L_exit:
    return 0;


}


//================  EOF  ==================================
