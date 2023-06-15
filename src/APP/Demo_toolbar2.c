//  Toolbar-Demo-2
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
2015-09-11 Created RF.

-----------------------------------------------------
*/
/*!
\file  ../APP/Demo_toolbar2.c
\brief   Toolbar-Demo-2
\code
=====================================================
List_functions_start:
..
List_functions_end:
=====================================================

make -f Demo_toolbar2.mak

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
static MemObj wVal, wLb1;
static int    actMod;         // active mainFunc (notebook-page)



//----------------------------------------------------------------
//---------------- prototypes: ---------------------
  int ui_init (MemObj *parent);
  int ui_CB1 (MemObj *mo, void **data);
  int ui_CB2 (MemObj *mo, void **data);
  int ui_ntb_CB (MemObj *mo, void **data);




//=========================================================
  int gCad_main () {
//=========================================================
// user has selected this plugin; starting ...

  TX_Print("gCad_main Demo_toolbar2 - 2023");

  UI_block__ (1, 0, 0);           // block functions

  // init & display Toolbar
  ui_init (&UIw_Box_TB);

  return 0;

}


//=========================================================
  int gCad_fini () {
//=========================================================
// dll being unloaded - reset Input, kill all open windows !!!

  int    ii;

  TX_Print("...... gCad_fini Demo_toolbar2");

  // do nothing, if already unloaded.
  if(!GUI_OBJ_IS_VALID(&tbApp)) return 0;

  // kill toolbar
  tbApp = GUI_toolbox_del (&tbApp);

  AP_User_reset ();             // close application
  UI_block__ (0, 0, 0);         // reset blocking

  return 0;

}

//=========================================================
  int ui_init (MemObj *parent) {
//=========================================================
// init & display Toolbar
//
//   ,-----------------------------------------------------------,
//   |,--------------------,,----------------,,-----------------,|
//   || boxh1v             ||   notebook     ||      boxh3v     ||
//   |'--------------------''----------------''-----------------'|
//   '-----------------------------------------------------------'
//


  MemObj boxh, boxh1v, wNtb1, boxh3v, w1, w2, w3;
  MemObj boxh1v1, boxh1v2;
  char *optLst[]={"option 1",
                  "option 2",
                  "option 3",
                  NULL};


  printf("ui_init \n");


  tbApp = GUI_toolbox__ (parent);

  // primary box: group of boxes side-by-side
  boxh = GUI_box_h (&tbApp, "");

  // 1. box: group of boxes box-below-box
  boxh1v = GUI_box_v (&boxh, "");

  // 2. box: notebook.
  GUI_spc__ (&boxh, 0,  5);     // 5 pixels right
  wNtb1 = GUI_notebook__ (&boxh, ui_ntb_CB);

  // 3. box
  GUI_spc__ (&boxh, 0,  5);     // 5 pixels right
  boxh3v = GUI_box_v (&boxh, "");


  //----------------------------------------------------------------
  // 1. vert. box boxh1v
  GUI_spc__ (&boxh1v, 1,  5);     // 5 pixels down
  // boxh1v1 = GUI_box_h (&boxh1v, "");  // for labels

  GUI_label_htm__ (&boxh1v, "<b> Demo_toolbar2: </b>", "");
  GUI_spc__ (&boxh1v, 1,  5);     // 5 pixels down
  wLb1 = GUI_label__ (&boxh1v, " test notebook ..", "");


  //----------------------------------------------------------------
  // notebook:
  // create notepook-page-1 with 3 buttons in hor. box ..
  w1 = GUI_notebook_add (&wNtb1, "ntb1");
  w2 = GUI_box_h (&w1, "");
  GUI_button__  (&w2, "B1-ntb1", ui_CB1,    (void*)"ntb1-B1", "");
  GUI_button__  (&w2, "B2-ntb1", ui_CB1,    (void*)"ntb1-B2", "");
  GUI_button__  (&w2, "B3-ntb1", ui_CB1,    (void*)"ntb1-B3", "");


  //----------------------------------------------------------------
  // create notepook-page-2 with entry in hor. box ..
  w1 = GUI_notebook_add (&wNtb1, "ntb2");
  w2 = GUI_box_h (&w1, "");
  wVal = GUI_entry__ (&w2, " value: ", "", NULL, NULL, "9");
  GUI_entry_set (&wVal, "0.1");
  GUI_button__  (&w2, "  test .. ", ui_CB1,    (void*)"ntb2-B1", "");


  //----------------------------------------------------------------
  // create notepook-page-3 with optMen
  w1 = GUI_notebook_add (&wNtb1, "ntb3");
  GUI_spc__ (&w1, 1,  5);     // 5 pixels down
  w2 = GUI_box_h (&w1, "");
  GUI_label__ (&w2, " active option: ", "l");
  w3 = GUI_optmen__  (&w2, optLst[0], optLst, NULL, ui_CB2, "");
  // GUI_optmen_set (&w3, 1, NULL);


  //----------------------------------------------------------------
  // create notepook-page-4
  w1 = GUI_notebook_add (&wNtb1, "ntb4");


  //----------------------------------------------------------------
  // 3. box
  GUI_spc__ (&boxh3v, 1,  5);     // 5 pixels down
  GUI_button__ (&boxh3v, " Help ", ui_CB1,    (void*)"Help", "");
  GUI_button__ (&boxh3v, " Exit ", ui_CB1,    (void*)"Exit", "");


  //----------------------------------------------------------------
  GUI_set_show (&tbApp, 1);    // show


  return 0;

}

//=====================================================================
  int ui_ntb_CB (MemObj *mo, void **data) {
//=====================================================================
// notebook-callbacks

  int      newMod, i1, i2;
  char     s1[120];


  printf("ui_ntb_CB \n");

  if(GUI_DATA_EVENT != TYP_EventEnter) return 0;
  newMod = GUI_DATA_I1;


  printf("================================= \n");
  printf("i3_ntb_CB newMod=%d actMod=%d\n",newMod,actMod);

  sprintf(s1, "new ntb-Page %d", newMod);

  GUI_label_mod (&wLb1, s1);

  actMod = newMod;

  return 0;

}


//=====================================================================
  int ui_CB1 (MemObj *mo, void **data) {
//=====================================================================
// callbacks


  long l1;
  char *cp1, cbuf1[256];
  
  
  // cp1 = (char*)data;
  cp1 = GUI_DATA_S1;

  printf("ui_CB1  |%s|\n",cp1);

  

  //-------------------------------------------------
  if(!strcmp(cp1, "Help")) {
    APP_Help (APP_act_nam, "");


  //-------------------------------------------------
  } else if(!strcmp(cp1, "ntb2-B1")) {
    cp1 = GUI_entry_get (&wVal);
    TX_Print (" Value is now %s\n",cp1);


  //-------------------------------------------------
  } else if(!strcmp(cp1, "Exit")) {
    gCad_fini ();
  }


  L_exit:
    return 0;


}


//=====================================================================
  int ui_CB2 (MemObj *mo, void **data) {
//=====================================================================
// callbacks

  int   ii;
  char  *cp1;

  ii = GUI_DATA_I1;
  cp1 = GUI_DATA_S2;

  printf("ui_CB2 %d |%s|\n",ii,cp1);

  return 0;

}



//================  EOF  ==================================
