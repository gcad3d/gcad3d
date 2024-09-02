// example user-callback from selection in grafic-window.
/*
 *
 * Copyright (C) 2024 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
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
\file  ../APP/DemoPlugin_Selection.c
\brief example user-callback from selection in grafic-window
\code
=====================================================
List_functions_start:
..
List_functions_end:
=====================================================

\endcode *//*----------------------------------------


. ./devbase.sh && make -f DemoPlugin_Selection.mak

- user-callback mousemove see ../APP/DemoPlugin_Mousemove.c
- user-callback KeyIn     see ../APP/DemoPlugin_KeyIn.c
- see also ../APP/DemoPlugin_Create.c

*/


// definition "export"
#include "../xa/export.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ut/ut_geo.h"              // DL_Att
#include "../ut/ut_geo_const.h"              // DL_Att
#include "../gr/ut_DL.h"               // DB_GetDLatt

#include "../xa/xa_msg.h"              // MSG_*

#include "../gui/gui_types.h"          // GUI_Key*


//----------------------------------------------------------------
// EXPORTS to main-module
export int gCad_main ();
export int gCad_fini ();


//----------------------------------------------------------------
// external vars:

// ex ../xa/xa_sele.c
extern int    GR_selBasTyp;
extern int    GR_selTyp;
extern long   GR_selDbi;



//----------------------------------------------------------------
// protos:
  int gui_CB_sele1 (int src, long dl_ind);








//=========================================================
  int gCad_main () {
//=========================================================


  // attach grafic selections
  AP_UserSelection_get (gui_CB_sele1);

  // display Info
  TX_Print("DemoPlugin_Selection - select obj/position - for output see console");
  TX_Print("- key Esc to exit ..");

  return 0;
}


//=========================================================
  int gCad_fini () {
//=========================================================

  // display Info
  TX_Print(">>> exit DemoPlugin_Selection");

  // unhilite all ..
  DL_hili_off (-1L);
  DL_Redraw ();

  AP_User_reset ();      // close application

  return 0;

}


//=========================================================
  int gui_CB_sele1 (int src, long dl_ind) {
//=========================================================
// userselection callback
// Input:
//    src      keyID or mouse-buttonID;
//    dl_ind   dispListIndex - only for left mousebutton;
//
// get type of selected obj from global var GR_selTyp


  int     oTyp, oNr;
  double  uPar;
  void    *obj;
  Point   pt1, pt2;


  printf("gui_CB_sele1 source=%d ind=%ld\n",src,dl_ind);


  if(src == GUI_KeyEsc) return gCad_fini ();

  if(src == GUI_MouseL) printf(" left mousebutton\n");
  if(src == GUI_MouseM) printf(" mid mousebutton\n");
  if(src == GUI_MouseR) printf(" right mousebutton\n");



  if(src != GUI_MouseL) goto L_exit;


  // typ,dbi of selected obj:
  printf(" GR_selTyp=%d\n",GR_selTyp);
  printf(" GR_selDbi=%ld\n",GR_selDbi);

  // get select-point
  GR_get_curPos_UC (&pt1);
  DEB_dump_obj__ (Typ_PT, &pt1, "selpos (in constrPln) is:");
  if(GR_selTyp == Typ_TmpPT) goto L_exit;


  // Vertex (Typ_Vertex);
  if(GR_selTyp == Typ_Vertex) {
    // surf selected -
    printf("***** TODO - vertex-selection - see UI_disp_vert \n");
    goto L_exit;
  }
 

  // get data of selected DB-obj 
  oTyp = UTO__dbo (&obj, &oNr, GR_selTyp, GR_selDbi);
  DEB_dump_obj__ (oTyp, obj, "obj-data - nr = %d",oNr);


  if((GR_selTyp >= Typ_LN)&&(GR_selTyp < Typ_PLN)) {
    // linear figure: get selected parameter on curve
    UT3D_par_pt__pt_prj_cv (&uPar, &pt2, 1, 2, &pt1, oTyp, obj, UT_DISP_cv);
    printf(" uPar = %f\n",uPar);
    goto L_exit;
  }


  if((GR_selTyp >= Typ_PLN)&&(GR_selTyp < Typ_SOL)) {
    // surface: get selected parameters uPar, vPar on surf    TODO
    printf("***** TODO - surface: get selected parameters uPar, vPar on surf");
    goto L_exit;
  }


  L_exit:
    return 0;

}


//======================== EOF ======================
