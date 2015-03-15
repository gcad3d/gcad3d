// example Hili.
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
  ..

-----------------------------------------------------
*/
/*!
\file  ../APP/DemoPlugin_Hili.c
\brief example hilite objects 
\code
=====================================================
List_functions_start:
..
List_functions_end:
=====================================================

\endcode *//*----------------------------------------


make -f DemoPlugin_Hili.mak

*/

#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../gui/gui_types.h"          // GUI_MouseL

#include "../ut/ut_geo.h"              // DL_Att
#include "../gr/ut_DL.h"               // DB_GetDLatt




static long last_obj;


// protos:
  int hili_sele1 (int src, long dl_ind);
  int hili_key1 (int key);



#ifdef _MSC_VER
// die folgenden 2 Funktionen exportieren (werden vom Main gerufen):
__declspec(dllexport) int gCad_main ();
__declspec(dllexport) int gCad_fini ();
// nachfolgende externals werden aus dem Main-Exe imported:
#define extern __declspec(dllimport)
#endif








//=========================================================
  int gCad_main () {
//=========================================================


  // attach grafic selections
  AP_UserSelection_get (hili_sele1);


  // attach KeyIn
  AP_UserKeyIn_get (hili_key1);

  // display Info
  TX_Print(">>> select object to hilite, Esc=Unhilite, q=quit");

  return 0;
}




//=========================================================
  int gCad_fini () {
//=========================================================

  // display Info
  TX_Print(">>> exit DemoPlugin_Hili");

  AP_User_reset ();      // close application

  return 0;

}




//=========================================================
  int hili_sele1 (int src, long dl_ind) {
//=========================================================
// userselection callback
// user selects obj.


  DL_Att att1;



  printf("hili_sele1 source=%d ind=%ld\n",src,dl_ind);


  if(src != GUI_MouseL) goto L_err_1; // skip all but left mousebutt.

  if(dl_ind < 0) goto L_err_1;          // skip indicate


  // get DL-record
  att1 = DL_GetAtt(dl_ind);
  printf("hili=%d\n",att1.hili);  // see hilite-bit


  last_obj = dl_ind;

  DL_hili_on (dl_ind);        // hilite obj
  DL_Redraw ();                 // Redraw DispList



  //-----------------------------------------------------
  L_err_1:

  return 0;

}



//=========================================================
  int hili_key1 (int key) {
//=========================================================
// userKeyin callback


  printf("hili_key1 %d\n",key);

  switch (key) {

    case GUI_KeyEsc:              // 0xff1b
      DL_hili_off (last_obj);    // unhilite last
      DL_Redraw ();               // Redraw DispList
      break;

    case 'q':
      DL_hili_off (-1L);         //  -1 = unhilite all
      DL_Redraw ();               // Redraw DispList
      gCad_fini ();
      break;

  }

  return 0;

}




//======================== EOF ======================
