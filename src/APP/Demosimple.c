//  Demosimple.c                   Simple-DLL-Demo  (minimum)
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
\file  ../APP/Demosimple.c
\brief  minimum example
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

#include "../xa/mdl__.h"               // SIZMF*
#include "../ut/ut_TX.h"               // TX_Print





#ifdef _MSC_VER
// export this functions
__declspec(dllexport) int gCad_main ();
__declspec(dllexport) int gCad_fini ();
// import functions exported from the core (see gCAD3D.def)
#define extern __declspec(dllimport)
#endif


// ext aus xa.c:
extern  char      AP_mod_fnam[SIZMFNam];  // der Modelname





//=========================================================
  int gCad_main () {
//=========================================================
// user has selected this plugin; starting ...


  printf("gCad_main Demosimple 1\n");

  // write to Main-Infowindow ..
  TX_Print("gCad_main out of Demosimple.dll");

  // get modelname from Mainprog
  printf("Modelname = %s\n",AP_mod_fnam);


  // finish application
  gCad_fini ();

  return 0;

}


//=========================================================
  int gCad_fini () {
//=========================================================
// dll being unloaded - reset Input, kill all open windows !!!

  printf("gCad_fini Demosimple\n");

  // write to Main-Infowindow ..
  TX_Print("...... gCad_fini Demosimple");

  AP_User_reset ();     // close application

  return 0;

}
//================  EOF  ==================================
