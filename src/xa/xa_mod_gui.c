//          Model-managment-functions.        Franz Reiter.  10.Dez.2003
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
2010-07-12 Created from xa_mod.c   RF

-----------------------------------------------------
*/
/*!
\file  ../xa/xa_mod_gui.c
\brief GUI Model-managment-functions 
\code
=====================================================
List_functions_start:

Mod_cre__           create new SubModel
Mod_cre_CB
Mod_m2s__           move Mainmodel > Submodel
Mod_m2s_CB
Mod_chg__           activate other Submodel
Mod_ren__           rename Submodel
Mod_ren_CB
Mod_del__           delete Submodel
Mod_del_CB

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



*/

#ifdef _MSC_VER
#include "MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>                    // isdigit


#include "../xa/xa_uid.h"             // UID_WinEdit
#include "../gr/ut_UI.h"              // UI_FuncSet
#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_os.h"               // OS_get_bas_dir ..
#include "../ut/ut_umem.h"

#include "../gui/gui__.h"              // Gtk3


// EXTERNALS:
// aus xa.c:
extern  char      WC_modact[128];



// PROTOS:
  // int Mod_cre_CB (void *parent, void *data);
  // int Mod_m2s_CB (void *parent, void *data);
  void Mod_chg_CB (char *modNam);
  // int Mod_ren_CB (void *parent, void *data);
  int Mod_del_CB (MemObj *mo, void **data);


/*
//====================================================================
  int Mod_cre__ () {
//====================================================================
/// create new SubModel:
/// - save active Model (WC_modact) -> tmp/Model_<submodelname>
/// - ask for Modelname; callback -> Mod_cre_CB

  printf("Mod_cre__\n");


  // save the active Submodel WC_modact -> TempFile
  Mod_sav_tmp ();


  // ask for new Modelname
  GUI_GetText(" Name of Submodel: ","Submodel1", -200, Mod_cre_CB);

  return 0;

}
*/


//====================================================================
  int Mod_cre__ () {
//====================================================================
/// create new SubModel:
/// save new submodelname;
/// provide new (empty) Modelspace

  int    irc;
  char   cbuf1[256], newNam[256], mNam[64];


  // save the active Submodel WC_modact -> TempFile
  Mod_sav_tmp ();

  L_name:
  strcpy(cbuf1, "submodel_1");
  irc = GUI_Dialog_e2b ("name for new submodel:", cbuf1, 250, "OK", "Cancel");
  if(irc != 0) return -1;

  printf("Mod_cre_CB |%s|\n",cbuf1);

  // make safeName from new subModelname
  // Mod_safeName (newNam, fnam);
  UTX_safeName (cbuf1, 1); // change '. /\\'

  // check maxLen; max 62 chars
  if(strlen(cbuf1) > 62) {
    TX_Print("max Laenge = 62 chars; gekuerzt !!");
    cbuf1[62] = '\0';
  }
  strcpy(mNam, cbuf1);


  // test if subModel mNam already exists 
  sprintf(newNam,"%sModel_%s",OS_get_tmp_dir(),mNam);
  if(OS_checkFilExist(newNam,1) == 1) {
    // sprintf(newNam,"  overwrite submodel %s ?  ",mNam);
    MSG_get_1 (newNam, 256, "OVER_mdl", "%s", mNam);
    irc = GUI_Dialog_2b (newNam, "OK", "Cancel");
    if(irc != 0) goto L_name;
  }



  // provide new (empty) Modelspace
  UTF_clear_ ();                    // Clear Mem

  // create 1. Line
  sprintf(cbuf1, "# Submodel %s", mNam);
  UTF_add_line (cbuf1);
  ED_load__ ();  // Memory > Editfenster
  UNDO_clear ();                 // clear Undo-List
  AP_Init1 ();   // init Displist, WC_main ..


  // save active submodelname
  strcpy(WC_modact, mNam);

  // display submodelname im Titlebar
  Mod_chg_tit ();

  // activate model in browser
  Brw_sMdl_act (mNam);

  return irc;

}


//====================================================================
  int Mod_m2s__ () {
//====================================================================
/// move Mainmodel > Submodel

  int   irc;
  char  cbuf1[256], newNam[256], mNam[64];

  // save the active Submodel WC_modact -> TempFile
  Mod_sav_tmp ();

  strcpy(cbuf1, "submodel_1");
  irc = GUI_Dialog_e2b (" name for new Submodel: ", cbuf1, 250, "OK", "Cancel");
  if(irc != 0) return -1;

  printf("Mod_m2s_CB |%s|\n",cbuf1);

  // make safeName from new subModelname
  UTX_safeName (cbuf1, 1); // change '. /\\'

  // check maxLen; max 62 chars
  if(strlen(cbuf1) > 62) {
    TX_Print("max Laenge = 62 chars; gekuerzt !!");
    cbuf1[62] = '\0';
  }
  strcpy(mNam, cbuf1);


  // test if subModel mNam already exists 
  sprintf(newNam,"%sModel_%s",OS_get_tmp_dir(),mNam);
  if(OS_checkFilExist(newNam,1) == 1) {
    // sprintf(newNam,"  overwrite submodel %s ?  ",mNam);
    MSG_get_1 (newNam, 256, "OVER_mdl", "%s", mNam);
    irc = GUI_Dialog_2b (newNam, "OK", "Cancel");
    if(irc != 0) return -1;
  }



  // rename
  L_work:
  sprintf(cbuf1,"%sModel_%s",OS_get_tmp_dir(),WC_modact);
  sprintf(newNam,"%sModel_%s",OS_get_tmp_dir(),mNam);
  rename (cbuf1, newNam);

  // disp msg
  TX_Print(" mainmodel moved to submodel %s",mNam);

  // clear mainModelspace
  UTF_clear_ ();                    // Clear Mem
  // init Modelspace with datum
  sprintf (cbuf1 , "# %s",OS_date1());
  UTF_add_line (cbuf1);

  ED_load__ ();  // Memory > Editfenster
  UNDO_clear ();                 // clear Undo-List
  GA_hide__ (-1, 0L, 0);           // clear Attribute-Table
  AP_Init1 ();                      // init Displist, WC_main ..


  // update Mod.lst
  Mod_mkList (0);
  // add new subModel in browserwindow
  Brw_Mod_add (mNam);

  return 0;

}


//====================================================================
  int Mod_chg__ (int mode) {
//====================================================================
/// \code
/// activate other Submodel
/// - save active Model (WC_modact) -> tmp/Model_<submodelname>
/// - provide List of Submodelnames
/// - Selection -> Mod_chg_CB
///   mode   0=save actice Submodel;  1=do not save active Submodel
/// \endcode

  int   i1;
  char  fnam[256], s1[256];

  printf("Mod_chg__\n");


  // save the active Submodel WC_modact -> TempFile
  if(mode == 0) Mod_sav_tmp ();


  //--------------------------------------------------------------
  // provide List of Submodelnames (<basedir>/tmp/Model_*) -> fnam
  Mod_mkList (0);


  //--------------------------------------------------------------
  // display List of Submodelnames
  // GUI_List3 (6, "", NULL, NULL, NULL, NULL);   // clear Filtertext
  sprintf(fnam,"%sMod.lst",OS_get_tmp_dir());

  // GUI_List1 ("select Model", fnam, Mod_chg_CB);

  i1 = GUI_list1_dlg_w (s1, 256,
                       NULL, " select Model", fnam,
                       "1", NULL, "60,40");
  if(i1 < 0) return -1;

  Mod_chg_CB (s1);

  return 0;

}

/*
//====================================================================
  int Mod_ren__ () {
//====================================================================
/// rename the active Submodel

  printf("Mod_ren__\n");
  if(strlen(WC_modact) < 1) {
    TX_Error("es ist kein Submodel aktiv ..");
    return -1;
  }
  GUI_GetText(" new Submodelname: ",  WC_modact, -200, Mod_ren_CB);
  return 0;
}
*/


///=================================================================
  int Mod_ren__ () {
///=================================================================
/// the callback if OK selected.

  int   irc;
  char  cbuf1[256], newNam[256], mNam[64];


  if(strlen(WC_modact) < 1) {
    TX_Error("es ist kein Submodel aktiv ..");
    return -1;
  }


  strcpy(cbuf1, WC_modact);
  irc = GUI_Dialog_e2b (" new Submodelname: ", cbuf1, 250, "OK", "Cancel");
  if(irc != 0) return -1;

  printf("Mod_ren_CB |%s|\n",cbuf1);

  if(strlen(cbuf1) < 1) return -1;


  // make safeName from new subModelname
  // Mod_safeName (newNam, fnam);
  UTX_safeName (cbuf1, 1); // change '. /\\'

  // check maxLen; max 62 chars
  if(strlen(cbuf1) > 62) {
    TX_Print("max Laenge = 62 chars; gekuerzt !!");
    cbuf1[62] = '\0';
  }
  strcpy(mNam, cbuf1);

  // rename <tmp/Model_<WC_modact>> -> tmp/Model_<data>>
  sprintf(cbuf1,"%sModel_%s",OS_get_tmp_dir(),WC_modact);
  sprintf(newNam,"%sModel_%s",OS_get_tmp_dir(),mNam);
  rename (cbuf1,newNam);

  // set name
  strcpy(WC_modact, mNam);

  // fix title
  Mod_chg_tit ();

  // disp msg
  TX_Print(" subModel renamed to %s",mNam);

  // rename active subModel in browser
  Brw_sMdl_ren_CB (mNam);

  return 0;

}


//====================================================================
  int Mod_del__ () {
//====================================================================

  char     cbuf[256];

  printf("Mod_del__\n");

  if(strlen(WC_modact) < 1) {
    TX_Error("es ist kein Submodel aktiv ..");
    return -1;
  }

  sprintf(cbuf, "  delete Submodel %s  ",WC_modact);
  GUI_DialogYN (cbuf, Mod_del_CB);

  return 0;

}


//=====================================================================
  int Mod_del_CB (MemObj *mo, void **data) {
//=====================================================================

  int   idat;
  char  cbuf[256];


  idat = GUI_DATA_EVENT;
  printf("Mod_del_CB %d\n",idat);
  if(idat != UI_FuncOK) return -1;

  // del <tmp/Model_<WC_modact>>
  strcpy (cbuf, WC_modact);


  Mod_chg__ (1); // do not save active Submodel

  Brw_Mdl_del_sm (cbuf);  // delete submodel in browser

  Mod_del1__ (cbuf);

  return 0;

}

// EOF
