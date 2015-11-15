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
2015-07-07 Created RF.

-----------------------------------------------------
*/
/*!
\file  ../APP/Demo_appDat1.c
\brief   ApplicationData-Demo  (minimum)
\code
=====================================================
List_functions_start:
..
List_functions_end:
=====================================================

\endcode *//*----------------------------------------


*/

#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_types.h"               // INT_8 - UINT_64
#include "../ut/ut_umem.h"                // Memspc MemObj UME_*


#include "../gui/gui__.h"


#ifdef _MSC_VER
// die folgenden 2 Funktionen exportieren (werden vom Main gerufen):
__declspec(dllexport) int gCad_main ();
__declspec(dllexport) int gCad_fini ();
// nachfolgende externals werden aus dem Main-Exe imported:
#define extern __declspec(dllimport)
#endif


//----------------------------------------------------------------
//---------------- external vars: ---------------------

// ex xa_ui.c:
extern MemObj    UIw_Box_TB;     // toolbarBox





//----------------------------------------------------------------
//---------------- local vars: ---------------------
static MemObj tbApp=GUI_OBJ_NEW;    // GIS-Toolbar
static MemObj wNam1, wNam2;



//----------------------------------------------------------------
//---------------- prototypes: ---------------------
  int ui_init (MemObj *parent);
  int ui_CB1 (MemObj *mo, void **data);




//=========================================================
  int gCad_main () {
//=========================================================
// user has selected this plugin; starting ...

  TX_Print("gCad_main Demo_appDat1");

  // init & display appDat1
  ui_init (&UIw_Box_TB);

  return 0;

}


//=========================================================
  int gCad_fini () {
//=========================================================
// dll being unloaded - reset Input, kill all open windows !!!

  int    ii;

  TX_Print("...... gCad_fini Demo_appDat1");

  // do nothing, if already unloaded.
  if(!GUI_OBJ_IS_VALID(&tbApp)) return 0;


  // GUI_set_show (&tbApp, 0);    // hide
  tbApp = GUI_toolbox_del (&tbApp);

  AP_User_reset ();             // close application

  return 0;

}

//=========================================================
  int ui_init (MemObj *parent) {
//=========================================================
// init & display appDat1

  char   sNam1[256], sNam2[256];
  MemObj box0;
  FILE   *fpi;


  printf("ui_ini \n");


  // redisplay if alrady exists ..
  if(GUI_OBJ_IS_VALID(&tbApp)) {
    return 0;
  }


  //----------------------------------------------------------------
  // get data out of applicationData-file
  if(appdat_open__ (&fpi, "r") < 0) {
    strcpy(sNam1, "Franz"); 
    strcpy(sNam2, "Karl"); 

  } else {
    fscanf(fpi, "%s", sNam1);
    fscanf(fpi, "%s", sNam2);
    fclose (fpi);
  }


  //----------------------------------------------------------------
  // tbApp = GUI_appDat1 (parent);
  tbApp = GUI_toolbox__ (parent);        // 2014-11-05

  box0 = GUI_box_h (&tbApp, "");

  GUI_label_htm__ (&box0, "<b> Demo_appDat: </b>", "");

  wNam1 = GUI_entry__ (&box0, " Name1: ", "", NULL, NULL, "9");
  GUI_entry_set (&wNam1, sNam1);

  wNam2 = GUI_entry__ (&box0, " Name1: ", "", NULL, NULL, "9");
  GUI_entry_set (&wNam2, sNam2);

  GUI_button__ (&box0, "Help", ui_CB1,    (void*)"Help", "");
  GUI_button__ (&box0, "Exit", ui_CB1,    (void*)"Exit", "");


  GUI_set_show (&tbApp, 1);    // show


  return 0;

}


//=====================================================================
  int ui_CB1 (MemObj *mo, void **data) {
//=====================================================================


  FILE *fpo;
  char *cp1;

  
  cp1 = GUI_DATA_S1;

  printf("ui_CB1  /%s/\n",cp1);

  

  //-------------------------------------------------
  if(!strcmp(cp1, "Help")) {
    APP_Help ("Demo_appDat1", "");


  //-------------------------------------------------
  } else if(!strcmp(cp1, "Exit")) {


    //-----------------------------------------------
    // save all data into applicationData-file
    //   (<tmpdir>/<APP_act_nam>.appdat)
    appdat_open__ (&fpo, "w");
    // get & write name1
    cp1 = GUI_entry_get (&wNam1);
    fprintf(fpo, "%s\n",cp1);
    // get & write name2
    cp1 = GUI_entry_get (&wNam2);
    fprintf(fpo, "%s\n",cp1);
    fclose (fpo);


    //-----------------------------------------------
    // exit plugin
    gCad_fini ();
  }


  L_exit:
    return 0;

}


//================  EOF  ==================================
