// make -f tst.mak
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
\file  ../APP/tst.c
\brief plugin tests
\code
=====================================================
List_functions_start:
..
List_functions_end:
=====================================================

\endcode *//*----------------------------------------

make -f tst.mak
nmake -f tst.nmak


gCad_main
gCad_fini
tst_sel_CB


*/



#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                         // for ...


#ifdef _MSC_VER
// die folgenden 2 Funktionen exportieren (werden vom Main gerufen):
__declspec(dllexport) int gCad_main ();
__declspec(dllexport) int gCad_fini ();
// nachfolgende externals werden aus dem Main-Exe imported:
#define extern __declspec(dllimport)
#endif



#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_os.h"               // OS_get_bas_dir ..
#include "../ut/ut_obj.h"              // UTO_stru_2_obj
#include "../ut/ut_TX.h"               // TX_Print

#include "../gui/gui__.h"              // GUI_

// protos:
  int tst_sel_CB (int src, long dl_ind);
  int tst_key_CB (int key);



//================================================================
  int tst_tst__ () {
//================================================================
 
  char    s1[256];

#include "../xa/gcad_version.h"        // INIT_TXT

  strcpy(s1, "License: GPL-3");
  strcat(s1, "\nCopyright: 1999-2015 CADCAM-Services Franz Reiter");
  strcat(s1, "\n(support@gcad3d.org)");
  GUI_AboutInfo (INIT_TXT, s1, "http://www.gcad3d.org", "xa_logo.xpm");


  return 0;

}


//=========================================================
  int gCad_main () {
//=========================================================
// init userfunction


  TX_Print("-->> gCad_main aus tst; 2014-03-20.");
  printf("-->> gCad_main aus tst;\n");


  //================================================================
  // TEST EXPORT_DLL'S:
  // tst_exp_stp ();
  // tst_exp_dxf ();
  // tst_exp_vrml2 ();
  // tst_exp_stl ();
  // tst_exp_obj ();
  // tst_exp_svg ();
  //================================================================
  // TEST IMPORT_DLL'S:
  // UI_menCB (NULL, "new");
  // tst_imp_dxf();
  // tst_imp_igs ();
  // tst_imp_vrml1(); // test import VRML-1
  // tst_imp_vrml2(); // test import VRML-2
  tst_imp_stp (); // tst_imp_exp.c  Test Import/Export-functions
  // tst_imp_stl ();
  // tst_imp_obj ();
  // tst_imp_tess ();
  // LandXml-Import: in core (AP_ImportXML -> lxml_read); 
  //================================================================

  L_end:
  DL_Redraw ();

  L_fini:
  gCad_fini ();

  return 0;

}



//=========================================================
  int gCad_fini () {
//=========================================================
// dll being unloaded - reset Input, kill all open windows !!!

  printf("gCad_fini tst \n");

  TX_Print("---<<<<<< exit tst__ <<<<<<<---\n");

  // tst_ogl_reset ();
  // VcSel_exit ();

  AP_User_reset ();

  return 0;

}



//=========================================================
  int tst_sel_CB (int src, long dl_ind) {
//=========================================================
// userselection callback

  int   irc, typ;
  long  dbi;
  // char  so[512];
  char  *so;
  // ObjGX o1;


  printf("tst_sel_CB src=%d ind=%ld\n",src,dl_ind);

  return 0;

}


//=========================================================
  int tst_key_CB (int key) {
//=========================================================
// userKeyin callback


  printf("tst_key_CB %d\n",key);

  switch (key) {

    case GUI_KeyEsc:
    case 'q':
      // DL_hili_off (-1L);         //  -1 = unhilite all
      // DL_Redraw ();               // Redraw DispList
      gCad_fini ();
      // PED_CB1 (NULL, "Exit");
      break;

  }

  return 0;

}



//======== EOF =========
