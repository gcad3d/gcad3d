// ../xa/xa_vwpt.c
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
2013-06-18 Extracted from xa_ui.c. RF.

-----------------------------------------------------
*/
#ifdef globTag
void vwpt(){}
#endif
/*!
\file  ../xa/xa_vwpt.c
\brief Viewport-functions 
\code
=====================================================
List_functions_start:

vwpt_UI
vwpt_opt_CB
vwpt__

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


*/



#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_os.h"               // OS_get_bas_dir

#include "../xa/xa_uid.h"              // UID_ouf_vwz

#include "../gui/gui__.h"              // Gtk3


//================================================================
// ex ../xa/xa_ui.c
extern MemObj  winMain;    // toolbarBox



//================================================================
// prototypes:
  int vwpt_opt_CB (MemObj *mo, void **data);





//================================================================
  int vwpt_UI () {
//================================================================
// display optionmenu

  char    *optLst[]={"save (Ctrl-V)",                        // 0
                     "restore next     (Ctrl-CursorRight)",  // 1
                     "restore previous (Ctrl-CursorLeft)",   // 2
                     NULL};


  // printf("vwpt_UI \n");

  GUI_popup__ (optLst, NULL, 0, vwpt_opt_CB, NULL);
 
  return 0;

}



//===================================================================
  int vwpt_opt_CB (MemObj *mo, void **data) {
//===================================================================

  int   iEv, isel;


  iEv = GUI_DATA_EVENT;
  if(iEv != TYP_EventPress) return 0;

  isel = GUI_DATA_I1;
    // printf("vwpt_opt_CB %d\n",isel);

  vwpt__ (isel);

  return 0;

}


//================================================================
  int vwpt__ (int mode) {
//================================================================
// save/restore viewport
// file ~/gCAD3D/tmp/view.dat
// mode   -1   clear all viewports (file-new)
//         0   save viewport
//         1   restore previous viewport
//         2   restore next viewport
//
// TODO: save ~/gCAD3D/tmp/view.dat into model;
//   restore ~/gCAD3D/tmp/view.dat at laod-model.
//   Make ~/gCAD3D/tmp/view.dat ascii (not binary) ?

 
  int    i1;
  double view[10];
  char   txbuf[200];
  FILE   *fp1;

  static int stat= 9999;


  // printf("vwpt__ %d\n",mode);

  sprintf(txbuf, "%sview.dat", OS_get_tmp_dir());



  //----------------------------------------------------------------
  if(mode == -1) {                 // clear all viewports
    OS_file_delete (txbuf);
    stat = -2;
    return 0;
  }


  //----------------------------------------------------------------
  if(mode == 0) {                 // save
    // get Viewparameters (Scale, Center, ..)
    GL_View_get (view);
    // save Viewparameters (add to file)
    fp1 = fopen (txbuf, "ab");   // append bin
    fwrite ((void*)view, sizeof(view), 1, fp1);
    fclose (fp1);
    stat = 9999;
    TX_Print("....new Viewport stored ....");
    return 0;
  }


  //----------------------------------------------------------------
  if(mode == 1) {          // restore previous (Ctrl-CursorRight)
    ++stat;
    goto L_get_vp;
  }

  //----------------------------------------------------------------
  if(mode == 2) {          // restore next     (Ctrl-CursorLeft)
    --stat;
    if(stat < 0) stat = 0;
    goto L_get_vp;

  }


  //----------------------------------------------------------------
  // get viewport from line <stat>
  L_get_vp:
      // printf(" vwpt get %d\n",stat);
    TX_Print("....restore Viewport %d ....",stat);


    // restore Viewparameters (Scale, Center, ..)
    fp1=fopen (txbuf, "rb");
    if(!fp1) goto L_no_vw;
    i1 = -1;

    L_r_nxt:
      ++i1;
      fread ((void*)view, sizeof(view), 1, fp1);
      if(feof (fp1)) { stat = i1 - 1; goto L_r_exit; }
      if(i1 < stat) goto L_r_nxt;

    L_r_exit:
      fclose (fp1);
        // printf(" vwpt i1=%d stat=%d\n",i1,stat);


    // restore view
    GL_View_set(view);
    UI_AP (UI_FuncSet, UID_ouf_vwz, (void*)&view[5]); // in box raus
    DL_Redraw ();

    return 0;


  L_no_vw:
    TX_Print("**** no viewpoint stored (store viewpoint with Ctrl-V)");
    return 0;

}


// EOF
