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

// #include "../xa/xa_uid.h"              // UID_ouf_vwz

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
//         1   restore next viewport
//         2   restore previous viewport
//         3   delete last activated viewport
//
// todo: save ~/gcad3d/tmp/view.dat into model;
//   restore ~/gcad3d/tmp/view.dat at load-model.
//   make ~/gcad3d/tmp/view.dat ascii (not binary) ?
//
// file = 
// format: ??

 
#define vwSiz    10
  int    i1, vNr;
  long   fSiz, rSiz;
  double view[vwSiz];
  char   txbuf[200], fntmp[200];
  FILE   *fp1, *fp2;

  static int stat = 0;


  // printf("vwpt__ %d\n",mode);


  rSiz = (long)sizeof(view);

  sprintf(txbuf, "%sview.dat", OS_get_tmp_dir());
    // printf(" vwpt__ txbuf=|%s|\n",txbuf);
  fSiz = OS_FilSiz (txbuf);
  if(fSiz < rSiz) {
    vNr = 0;
  } else {
    vNr = fSiz / rSiz;
  }
    // printf(" vwpt__ fSiz=%ld stat=%d vNr=%d\n",fSiz,stat,vNr);


  //----------------------------------------------------------------
  if(mode == -1) {                 // clear all viewports
    OS_file_delete (txbuf);
    stat = 0;
    return 0;
  }


  //----------------------------------------------------------------
  if(mode == 0) {                 // add / save (Ctrl-CursorUp)
    // get Viewparameters (Scale, Center, ..)
    GL_View_get (view);
    // save Viewparameters (add to file)
    fp1 = fopen (txbuf, "ab");   // append bin
    fwrite ((void*)view, rSiz, 1, fp1);
    fclose (fp1);
    stat = vNr;
    ++vNr;
    TX_Print("....new Viewport %d stored ....", vNr);
    return 0;
  }


  //----------------------------------------------------------------
  if(vNr < 1) goto L_no_vw;

  if(mode == 1) {          // restore next (Ctrl-CursorRight)
    ++stat;
    goto L_get_vp;
  }

  //----------------------------------------------------------------
  if(mode == 2) {          // restore previous     (Ctrl-CursorLeft)
    --stat;
    goto L_get_vp;
  }

  //----------------------------------------------------------------
  if(mode == 3) {          // delete last activated viewport  (Ctrl-CursorDown)
    // remove ?
    TX_Print("....remove Viewport %d ....",stat);
    sprintf(fntmp, "%stemp.dat", OS_get_tmp_dir());

    fp1 = fopen (txbuf, "rb");
    fp2 = fopen (fntmp, "wb");
    if(!fp1) goto L_no_vw;
    i1 = 0;

    L_rem_nxt:
      ++i1;
      fread ((void*)view, rSiz, 1, fp1);
      if(feof (fp1)) { stat = i1 - 1; goto L_rem_exit; }
      if(i1 != stat) fwrite ((void*)view, rSiz, 1, fp2);
      goto L_rem_nxt;

    L_rem_exit:
      fclose (fp1);
      fclose (fp2);
      OS_file_rename (fntmp, txbuf);
      --vNr;
      stat = vNr;
  }


  //----------------------------------------------------------------
  // get viewport from line <stat>
  L_get_vp:
    if(stat > vNr) stat = 1;
    if(stat < 1) stat = vNr;
      // printf(" vwpt get %d\n",stat);
    TX_Print("....restore Viewport %d ....",stat);


    // restore Viewparameters (Scale, Center, ..)
    fp1=fopen (txbuf, "rb");
    if(!fp1) goto L_no_vw;
    i1 = 0;

    L_r_nxt:
      ++i1;
      fread ((void*)view, rSiz, 1, fp1);
      if(feof (fp1)) goto L_r_exit;
      if(i1 < stat) goto L_r_nxt;

    L_r_exit:
    fclose (fp1);
      // printf(" vwpt i1=%d stat=%d\n",i1,stat);

    // stat = i1 - 1;


    // restore view
    GL_View_set(view);
//     UI_AP (UI_FuncSet, UID_ouf_vwz, (void*)&view[5]); // in box raus
    DL_Redraw ();

    return 0;


  L_no_vw:
    TX_Print("**** no viewpoint stored (store viewpoint with Ctrl-cursorUp)");
    return 0;

}


// EOF
