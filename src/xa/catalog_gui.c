//              CATALOG-GUI                         2007-04-09   RF
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
verschachtelte Normteile  (dzt nicht moeglich)
  Testbeipiel; Mod_get_namSub  muss man dazu korrigieren

-----------------------------------------------------
Modifications:
  ..

-----------------------------------------------------
*/
/*!
\file  ../xa/catalog_gui.c
\brief CATALOG-GUI
\code
=====================================================
List_functions_start:

CTLG_Cre__               create new catalogFile
CTLG_Mod__               display List of <symDir-CATALOG>*.ctlg for userSelection
CTLG_Mod_CB              Catalog selected
CTLG_Del__               display List of <symDir-CATALOG>*.ctlg for userSelection

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


====================================================================
tmp/CatParts.lst   = name of active catalog
tmp/catParts.lst   = childList for active catalog

Die catalogModels werden wie externe Models behandelt;
  es gibt also kein internes subModel;

RefMod:
  enthaelt die Nr des basMod; die Position (po,vx,vz);
  nicht den basicModelname (ist NULL)

BasMod:
  Typ = -2; Modelname ist <catalogName>_<partName>
  Das zugehoerige Modelfile wird durch die Funktion CTLG_mnam_modelnam
  geholt (in der 1. zeile des -write-File steht der ModelfileName).



CTLG_Cre__
// create new catalogFile
// check if symDir CATALOG exists; wenn nein: anfordern !!
// catalogname  vom user abfragen
// Create file <symDir-CATALOG>/<catalogname>.ctlg (copy the template)


CTLG_Mod__
// display Liste of <symDir-CATALOG/>*.ctlg for userSelection
// Edit selected file


CTLG_Del__
// display Liste of <symDir-CATALOG/>*.ctlg for userSelection
// delete selected file mit Rueckfrage


create ModelReference:
user-select Part:
 - Auswahl via Browser; muss die kataloge als Directory, die darin
   enthaltenen parts als Liste anbieten.
  Die dafuer erforderlichen Listen beim allerersten Programmanfang erstellen
    und dann nur mehr nach jedem Catalog-Modify



----------------------------------------------------------------------
Wichtige Funktionen:

WC_Work__
  CTLG_Part_Ref1
  Mod_load_allsm
    Mod_load_sm
      Mod_get_namSub

APT_decode_model
  CTLG_mnam_PartID

APT_work_PrgCmd   :4217
  CALL CTLG






*/

#ifdef _MSC_VER
#include "MS_Def0.h"
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../ut/ut_geo.h"              // ModelRef
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_memTab.h"

#include "../gui/gui__.h"              // Gtk3

#include "../ut/func_types.h"              // UI_FuncSet
#include "../xa/xa_mem.h"              // memspc011
#include "../xa/xa_msg.h"              // MSG_cancel,

#include "../ut/ut_os.h"               // OS_get_bas_dir
#include "../db/ut_DB.h"               // DB_get_ModRef






#ifdef globTag
 void CTLG(){}
#endif


 

//================================================================
  int CTLG_Cre__ () {
//================================================================
// CTLG_Cre__                  get new catalog-name from user
// create new catalogFile
// check if symDir CATALOG exists; wenn nein: anfordern !!
// Create file <symDir-CATALOG>/<catalogname>.ctlg (copy the template)
//
// RetCode:  0=OK; -1=user-abort



  int    irc;
  char   cbuf0[320], *cbuf1, cNam[64], s1[400];



  //----------------------------------------------------------------
  // get name for new catalog into cbuf1
  irc = AP_GUI__ (cbuf0, sizeof(cbuf0), "GUI_dlg1", "dlgbe",
                "\" name for new catalog: \"",
                "Cancel",
                "OK",
                "--ent \"myCatalog1\" 16",
                NULL);
  if(irc < 0) return -1;
  if(UTX_IS_EMPTY(cbuf0)) return -1;    // cancel
    // printf(" f-AP_GUI__ |%s|\n",cbuf0);
  if(cbuf0[0] != '1') return -1;

  cbuf1 = &cbuf0[1];


  //----------------------------------------------------------------
  // make safeName
  UTX_safeName (cbuf1, 1); // change '. /\\'


  // check maxLen; max 62 chars
  if(strlen(cbuf1) > 62) {
    TX_Print("max Laenge = 62 chars; gekuerzt !!");
    cbuf1[62] = '\0';
  }
  strcpy(cNam, cbuf1);


  // symDir CATALOG holen ..
  if(CTLG_getCatDir (cbuf0) < 0) return -1;
    // printf(" CATALOG=|%s|\n",cbuf1);
  strcat(cbuf0, cNam);
  strcat(cbuf0, ".ctlg");


  // copy <cfg>/Default.ctlg <CATALOG>/<cNam>.ctlg
  sprintf(s1,"%sDefault.ctlg",OS_get_cfg_dir());
  OS_file_copy (s1, cbuf0);    // copy from to
  TX_Print("create catalog-File %s",cbuf1);


  // create tmp/Catalog.lst
  CTLG_Lst_write ();


  // kill window
  L_exit:
  // GUI_GetText_CB (NULL, (void*)UI_FuncKill);

  return 0;

}


//====================================================================
  int CTLG_Mod_CB (char *cNam) {
//====================================================================
/// Catalog selected;

  char   fNam[256], cfn[256];

  if(cNam == NULL) return 0;  // cancel

  // printf("CTLG_Mod_CB |%s|\n",cNam);

  if(CTLG_getCatDir (cfn) < 0) return -1;
  
  // edit Catalog
  sprintf(fNam, "%s%c%s",cfn,fnam_del,cNam);
  APP_edit (fNam, 0);


  return 0;

}


//================================================================
  int CTLG_Mod__ () {
//================================================================
/// display Liste of <symDir-CATALOG>/*.ctlg for userSelection
/// Edit selected file

  int   i1;
  char  fnam[256], s1[256];

  // printf("CTLG_Mod__ \n");

  CTLG_Lst_write (); // create tmp/Catalog.lst
  // display Liste of <symDir-CATALOG>/*.ctlg for userSelection
  sprintf(fnam,"%sCatalog.lst",OS_get_tmp_dir());

  // GUI_List1 (" select Catalog", fnam, (void*)CTLG_Mod_CB);

//   i1 = GUI_list1_dlg_w (s1, 256,
//                        NULL, " select Catalog", fnam,
//                        "1", NULL, "60,40");

  i1 = GUI_listf1__ (s1, sizeof(s1), fnam, "\"select Catalog\"", "\"x40,y30\"");
  if(i1 < 0) return -1;

  CTLG_Mod_CB (s1);

  return 0;

}


/* 
//=================================================================
  int CTLG_Del_Del_CB (MemObj *parent, void **data) {
//=================================================================

  char  fnam[256], cfn[256];


  printf("CTLG_Del_Del_CB %d\n",GUI_DATA_EVENT);


  if(GUI_DATA_EVENT != UI_FuncOK) return 0;

  if(CTLG_getCatDir (cfn) < 0) return -1;

  // delete file
  // sprintf(fnam,"%s%c%s",cfn,fnam_del,memspc011);
  sprintf(fnam,"%s%s",cfn,memspc011);
  TX_Print("delete catalog-File %s",fnam);
    printf(" _Del_Del_CB |%s|\n",fnam);
  OS_file_delete (fnam);

  // create tmp/Catalog.lst
  CTLG_Lst_write ();

  return 0;

}


//====================================================================
  int CTLG_Del_CB (char *cNam) {
//====================================================================
/// Catalog selected;

  char   cbuf[256];

  if(cNam == NULL) return 0;  // cancel

  // printf("CTLG_Del_CB |%s|\n",cNam);
  strcpy(memspc011, cNam);

  // delete Catalog
  sprintf(cbuf,"  OK to delete Catalog %s ?  ",cNam);

  GUI_DialogYN  (cbuf, CTLG_Del_Del_CB);

  return 0;

}
*/

//================================================================
  int CTLG_Del__ () {
//================================================================
/// display Liste of <symDir-CATALOG>/*.ctlg for userSelection
/// delete selected file mit Rueckfrage

  int   i1;
  char  fnam[256], s1[256], s2[256], cfn[256];

  // printf("CTLG_Del__ \n");

  CTLG_Lst_write (); // create tmp/Catalog.lst

  // let user select from list of <symDir-CATALOG>/*.ctlg
  sprintf(fnam,"%sCatalog.lst",OS_get_tmp_dir());
  i1 = GUI_listf1__ (s1, sizeof(s1), fnam, "\"delete Catalog\"", "\"x40,y30\"");
  if(i1 < 0) return -1;

  // CTLG_Del_CB (s1);


  // ask user - delete ? yes / no
  sprintf(s2, " OK to delete Catalog %s ? ", s1);
  i1 = GUI_dlg_2b (s2, MSG_const__(MSG_ok), MSG_const__(MSG_no));
    // printf(" PRG_Del__-L2 %d\n",i1);
  if(i1 != 0) return -1;            // error or cancel


  if(CTLG_getCatDir (cfn) < 0) return -1;
    printf(" CTLG_Del__-CATALOG = |%s|\n",cfn);


  // delete file
  // sprintf(fnam,"%s%c%s",cfn,fnam_del,memspc011);
  sprintf(fnam,"%s%s",cfn,s1);
  TX_Print("delete catalog-File %s",fnam);
    printf(" _Del_Del_CB |%s|\n",fnam);
  OS_file_delete (fnam);


  // update tmp/Catalog.lst
  CTLG_Lst_write ();



  return 0;

}


//==================== EOF =======================================
