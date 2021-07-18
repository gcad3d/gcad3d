/*  Demoprog                                         RF     2004-11-05
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
\file  ../APP/DemoAssy1.c
\brief  demo build assembly
\code
=====================================================
List_functions_start:
..
List_functions_end:
=====================================================

\endcode *//*----------------------------------------


- clear model
- load 3 submodels
- place submodels
- redraw

*/


#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef _MSC_VER
// die folgenden 2 Funktionen exportieren (werden vom Main gerufen):
__declspec(dllexport) int gCad_main ();
__declspec(dllexport) int gCad_fini ();
// nachfolgende externals werden aus dem Main-Exe imported:
#define extern __declspec(dllimport)
#endif


#include "../ut/ut_umem.h"

#include "../gui/gui__.h"

#include "../xa/xa_ui.h"                     // UI_men__..




//=========================================================
  int gCad_fini () {
//=========================================================
// dll being unloaded - reset Input, kill all open windows !!!

  TX_Print("...... gCad_fini DemoAssy1");

  AP_User_reset ();      // close application

  return 0;

}


//=========================================================
  int gCad_main () {
//=========================================================
// init userfunction

  char   cBuf1[320];

  TX_Print("--->>>>>> gCad_main aus DemoAssy1 >>>>>>>---\n");


  // clear ("new")
  UI_men__ ( "new");

  // load 3 submodels from Directory "Data"
  MDL_load_mdl_f ("Data/sample_mod_blech1.gcad", cBuf1);
  MDL_load_mdl_f ("Data/sample_mod_screw1.gcad", cBuf1);
  MDL_load_mdl_f ("Data/sample_mod_nut1.gcad", cBuf1);


  // place submodels
  // init AuxBuf, add Lines -> Aux.Buffer
  UTF_clear1();
  UTF_add1_line ("M20=\"Data/sample_mod_blech1.gcad\" R(P(0 10 0))");
  UTF_add1_line ("M21=\"Data/sample_mod_screw1.gcad\" R(P(30 36.5 67))");
  UTF_add1_line ("M22=\"Data/sample_mod_nut1.gcad\" R(P(36 36.5 67))");


  // Add Aux.Buffer -> MainBuffer
  UTF_insert1 (-1L);
  ED_load__ ();


  // execute (=Press END-Button), redraw
  UI_but_END ();
  DL_Redraw ();

  gCad_fini ();           // close

  return 0;


}


//====================== EOF =============================
