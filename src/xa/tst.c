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

. ../options.sh && make -f tst.mak
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

#include <GL/gl.h>                     // GL_TRIANGLES


// #include <errno.h>
// #include <dlfcn.h>           // Unix: dlopen


#ifdef _MSC_VER
// die folgenden 2 Funktionen exportieren (werden vom Main gerufen):
__declspec(dllexport) int gCad_main ();
__declspec(dllexport) int gCad_fini ();
// nachfolgende externals werden aus dem Main-Exe imported:
#define extern __declspec(dllimport)
#endif



#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_geo_const.h"        // UT3D_CCV_NUL
#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_txTab.h"            // TxtTab
#include "../ut/ut_os.h"               // OS_get_bas_dir ..
#include "../ut/ut_obj.h"              // UTO_stru_2_obj
#include "../ut/ut_TX.h"               // TX_Print
#include "../ut/ut_plg.h"              // UT3D_par1_parplg
#include "../ut/func_types.h"               // SYM_..
#include "../ut/ut_ox_base.h"          // OGX_SET_INDEX
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../db/ut_DB.h"               // DB_get_..
#include "../gr/tess_su.h"             // TypTsuSur
#include "../gr/ut_GL.h"               // GL_get_Scale
#include "../gui/gui__.h"              // GUI_
#include "../xa/xa_msg.h"              // MSG_*
#include "../xa/xa_mem.h"              // memspc51, mem_cbuf1

#include "../ci/NC_apt.h"                 // T_BSP0


// Externals aus ../ci/NC_Main.c:
extern double    APT_ModSiz;

// Externals aus ../xa/xa.c:
extern char      AP_mod_fnam[128];
extern Plane     WC_sur_act;            // die aktive Plane
extern int       WC_modact_ind;         // -1=primary Model is active;
                                        // else subModel is being created


// protos:
  int tst_sel_CB (int src, long dl_ind);
  int tst_key_CB (int key);



//=========================================================
  int gCad_main () {
//=========================================================
// init userfunction

  int   i1;


  TX_Print("-->> gCad_main aus tst; 2017-04-18.");
  printf("-->> gCad_main aus tst;\n");

  // attach KeyIn - connect KeyIn -> func dia_KeyIn
  AP_UserKeyIn_get (tst_key_CB);


  //================================================================
  i1 = tst__ (0);       goto L_end;   // // exit plugin
  // i1 = tst_surfaces (0);       goto L_end;   // // exit plugin
  // if(i1 == 0) return 0;     // keep plugin alive
  //================================================================
  // TEST EXPORT_DLL'S:
  // tst_print__ ();  // OS_dll_do ("xa_print__", "PRI__"
  // tst_print_pdf ();
  // tst_exp_stp ();
  // tst_exp_dxf ();
  // tst_exp_vrml2 ();
  // tst_exp_stl ();
  // tst_exp_obj ();
  // tst_exp_svg ();
  //================================================================
  // TEST IMPORT_DLL'S:
  // UI_menCB (NULL, "new");
  // tst_imp_gcad ();
  // tst_imp_dxf ();
  // tst_imp_igs ();
  // tst_imp_vrml1 (); // test import VRML-1
  // tst_imp_vrml2 (); // test import VRML-2
  // tst_imp_stp (); // tst_imp_exp.c  Test Import/Export-functions
  // tst_imp_stl ();
  // tst_imp_obj ();
  // tst_imp_tess ();
  // LandXml-Import: in core (AP_ImportXML -> lxml_read); 
  //================================================================
  // AP_print__ (); // test feedbackbuffer - out -> <tempDir>/print.tmp

  L_end:
  DL_Redraw ();

  L_fini:
  gCad_fini ();

  return 0;

}

//=========================================================
  int tst_sel_CB (int src, long dl_ind) {
//=========================================================
// userselection callback

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
    case 'l':
      tst__ (0);
      break;
    case 'u':
      tst__ (1);
      break;
    case 't':
      printf(" GUI_Win_exist=%d\n",GUI_Win_exist ("Export/Print"));
      break;
  }

  return 0;

}

//=========================================================
  int gCad_fini () {
//=========================================================
// dll being unloaded - reset Input, kill all open windows !!!

  TX_Print("...... gCad_fini tst");
  AP_User_reset ();      // close application
  return 0;

}


// EOF
