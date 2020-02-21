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

CTLG_Sel__               display catalog-list and part-list
CTLG_Sel_CB              callback child-selection (catalog-part selected)
CTLG_Cre__               create new catalogFile
CTLG_Cre_CB              users gives name for new catalog
CTLG_Mod__               display List of <symDir-CATALOG>*.ctlg for userSelection
CTLG_Mod_CB              Catalog selected
CTLG_Del_Del_CB          .
CTLG_Del_CB              Catalog selected
CTLG_Del__               display List of <symDir-CATALOG>*.ctlg for userSelection

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


====================================================================
tmp/catParts.act   = name of active catalog
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

#include "../ut/ut_geo.h"             // ModelRef
#include "../ut/ut_txt.h"             // fnam_del
#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_memTab.h"

#include "../gui/gui__.h"              // Gtk3

#include "../ut/func_types.h"              // UI_FuncSet
#include "../xa/xa_mem.h"             // memspc011
#include "../ut/ut_os.h"              // OS_get_bas_dir
#include "../db/ut_DB.h"              // DB_get_ModRef






#ifdef globTag
 void CTLG(){}
#endif


// ex catalog.c
int CTLG_Sel_wPartLst (char *partFilNam, char *actCatNam);


 
//=====================================================================
  int CTLG_Sel_CB (char *parentNam, char *childNam) {
//=====================================================================
/// \code
/// Callback child-selection (catalog-part selected)
/// Retcode: 0=keep Hlist; 1=destroy HList.
/// \endcode

  int  irc;
  char cbuf[256], actNam[256];


  printf("CTLG_Sel_CB |%s|%s|\n",parentNam,childNam);

  // callback vom Cancel-Button.
  if((childNam == NULL)&&(parentNam == NULL))  {
    printf(" CTLG_Sel_CB-Cancel\n");
    return 0;
  }


  // callback von catalog-selection: must create new ChildList !
  if(childNam == NULL) {
    printf(" CTLG_Sel_CB-create new List for |%s|\n",parentNam);
    sprintf(cbuf,"%scatParts.lst",OS_get_tmp_dir());
    CTLG_Sel_wPartLst (cbuf, parentNam);
    return 0;
  }


  // callback von catalog-Part-selection
  // printf("CTLG_Sel_CB |%s|%s|\n",parentNam,childNam);

  // parentNam ist nun Filename of catalog; remove Filetyp.
  UTX_ftyp_cut (parentNam);

  IE_cad_selM2_CB (childNam, parentNam);


  return 1;

}


//================================================================
  int CTLG_Sel__ () {
//================================================================
/// display catalog-list and part-list


  static char   parent[128] = "";
  char   fnam[256];


  printf("CTLG_Sel__ <<<<<<<<<<<< \n");

   // check if file tmp/Catalog.lst exists; no: create it.
  if(OS_checkFilExist(fnam, 1) != 1) {
    CTLG_Lst_write (); // create <tmpiDir>/Catalog.lst
  }

  // if file tmp/catParts.lst exists: get it -> parent
  sprintf(fnam,"%scatParts.act",OS_get_tmp_dir());
  if(OS_checkFilExist(fnam, 1)) {
    // parent lesen
    UTX_fgetLine (parent, 128, fnam, 1);
  }

  // Datei mit den Names aller catalog-Files
  sprintf(fnam,"%sCatalog.lst",OS_get_tmp_dir());
    printf(" _list1_h2__ parent=|%s|\n",parent);
    printf(" _list1_h2__ fnam=|%s|\n",fnam);

  // display List of parts ..
  GUI_list1_h2__ (
                 "select catalog",        // titletext parent
                 "select catalogPart",    // titletext child
                 "change catalog",        // caption button
                 parent,                  // active catalog
                 fnam,                    // filename catalogs
                 CTLG_Sel_wPartLst,  // callback parent-sel/create childs-file
                 CTLG_Sel_CB,        // callback child-selection
                 "60,40");                // hsiz, vsiz


    printf(" nach GUI_list1_h2__\n");


  return 0;

}


//================================================================
  // int CTLG_Cre_CB (void *parent, void *data) {
  int CTLG_Cre__ () {
//================================================================
/// \code
/// users gives name for new catalog
/// create new catalogFile
/// check if symDir CATALOG exists; wenn nein: anfordern !!
/// Create file <symDir-CATALOG>/<catalogname>.ctlg (copy the template)
/// \endcode


  int    irc;
  char   cbuf0[256], cbuf1[256], cNam[64];
  char   *buttons[]={"OK","Cancel",NULL};


  cbuf1[0] = '\0';
  irc = GUI_DialogEntry ("name for new catalog: ",cbuf1, 250, buttons, 2);
  if(irc != 0) return -1;  // cancel


  // make safeName
  UTX_safeName (cbuf1, 1); // change '. /\\'


  // check maxLen; max 62 chars
  if(strlen(cbuf1) > 62) {
    TX_Print("max Laenge = 62 chars; gekuerzt !!");
    cbuf1[62] = '\0';
  }
  strcpy(cNam, cbuf1);



  // symDir CATALOG holen ..
  if(CTLG_getCatDir (cbuf1) < 0) return -1;
    // printf(" CATALOG=|%s|\n",cbuf1);
  strcat(cbuf1, cNam);
  strcat(cbuf1, ".ctlg");



  // copy xa/Default.ctlg <CATALOG>/xx.ctlg
  sprintf(cbuf0,"%sDefault.ctlg",OS_get_cfg_dir());
  OS_file_copy (cbuf0, cbuf1);
  TX_Print("create catalog-File %s",cbuf1);


  // create tmp/Catalog.lst
  CTLG_Lst_write ();


  // kill window
  L_exit:
  // GUI_GetText_CB (NULL, (void*)UI_FuncKill);

  return 0;

}


/*
//================================================================
  int CTLG_Cre__ () {
//================================================================
/// create new catalogFile
// check if symDir CATALOG exists; wenn nein: anfordern !!
// catalogname  vom user abfragen
// Create file <symDir-CATALOG>/<catalogname>.ctlg (copy the template)

 
  // printf("CTLG_Cre__ \n");


  // catalogname  vom user abfragen
  GUI_GetText ("name for new catalog: ",
              "", -300, CTLG_Cre_CB);    // defaultname = old name

  return 0;

}
*/

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

  i1 = GUI_listf1__ (s1, sizeof(s1), fnam, "select Catalog", "40,30");
  if(i1 < 0) return -1;

  CTLG_Mod_CB (s1);

  return 0;

}


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


//================================================================
  int CTLG_Del__ () {
//================================================================
/// display Liste of <symDir-CATALOG>/*.ctlg for userSelection
/// delete selected file mit Rueckfrage

  int   i1;
  char  fnam[256], s1[256];

  // printf("CTLG_Del__ \n");

  CTLG_Lst_write (); // create tmp/Catalog.lst

  // display Liste of <symDir-CATALOG>/*.ctlg for userSelection
  sprintf(fnam,"%sCatalog.lst",OS_get_tmp_dir());

  // GUI_List1 (" delete Catalog", fnam, (void*)CTLG_Del_CB);

//   i1 = GUI_list1_dlg_w (s1, 256,
//                        NULL, " delete Catalog", fnam,
//                        "1", NULL, "60,40");

  i1 = GUI_listf1__ (s1, sizeof(s1), fnam, "delete Catalog", "40,30");
  if(i1 < 0) return -1;

  CTLG_Del_CB (s1);

  return 0;

}


//==================== EOF =======================================
