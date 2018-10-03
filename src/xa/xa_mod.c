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
use MBTYP_EXTERN/MBTYP_INTERN/MBTYP_CATALOG 0/-1/-2
Mirrorflag -> ModelRef
Ditto-GL: Inhaltelemente brauchen kein DL-Index !

-----------------------------------------------------
Modifications:
2003-12-10 Erstmalig erstellt. RF.

-----------------------------------------------------
*/
/*!
\file  ../xa/xa_mod.c
\brief subModel-managment-functions without GUI
\code
=====================================================
List_functions_start:

Mod_load_all        unused
Mod_load__          load native model
Mod_load_allsm      load Models of mdb_dyn-Records;
Mod_load_sm         load a subModel (ED_work_file / ED_Run)
Mod_SM_add_file     add subModel from file
Mod_cre__           create new SubModel
Mod_cre_CB
Mod_ren__           rename Submodel
Mod_ren_CB
Mod_del__           delete Submodel
Mod_del_CB
Mod_chg__           activate other Submodel
Mod_chg_CB          load selected SubModel
Mod_chg_x           activate internal subModel from modelname
Mod_m2s__           move Mainmodel > Submodel
Mod_m2s_CB
Mod_sav_ck          save Model+Submodel -> File native; compare with last saved
Mod_sav_cmp__         compare <tmpDir>/Model mit <tmpDir>/Mod_in
Mod_sav__           save Model+Submodel -> File native
Mod_sav_i           save Model+Submodels into tempDir as "Model" native. Intern
Mod_sav_tmp         save the active Submodel AP_modact_nam -> TempFile
Mod_sav2file__      save the active Submodel AP_modact_nam -> permanent File
Mod_sav2file_CB
Mod_file2model      save submodel > tmp/Model_<name>
Mod_savSubBuf1      save Submodel in Buffer1 + active Hidelist
Mod_LoadFile__      Model als Submodel laden
Mod_LoadSubmodel    ?

Mod_smNam_get       get new subModelname from user
Mod_ck_isMain       check if the active model is the mainmodel

Mod_sym_dir__       get symbolic-directory from any-directory
Mod_sym_get__       get symbolic-directory & absolute-path from filename
Mod_sym_get1        get path from symbol; resolv "symbol/fnam" or "fnam"
Mod_sym_get2        get symbol from path
Mod_sym_add         register symbolic path
Mod_sym_del         delete symbolic path
Mod_symOpen_set     register symbolic path
Mod_get_path        resolv "symbol/fnam" or "fnam"
Mod_sym_getAbs      get abs.dir from symbolic-dir
Mod_fNam_sym        get absolute-filename from symbolic-filename
Mod_fNam_get        get AP_mod_dir,sym,fnam,ftyp from symbolic- or abs.filename
Mod_fNam_set        get symbolic-filename or absolute-filename
Mod_sym_dump        dump all symbolic directories

Mod_get_ftyp1       give Modeltyp from Filename
Mod_get_typ1        give modeltyp from definition Line and extract Modelname
Mod_get_typ2        give modeltyp from (complete) Modelname
Mod_get_names       scan gesamten APT-Buffer nach Modelnames.
Mod_get_namStore    save new BaseModel as mdb_dyn-Record
Mod_kill__          delete all tmp/Model_*
Mod_chg_tit         display submodelname im Titlebar
Mod_mkList          make list of all Submodels

DB_get_ModRef       get referenceModel from refModelNr
Mod_mdbi_mdri       get basicModelNr of refModelNr
Mod_m3_mdr          get matrix of referenceModel
Mod_mNam_mdr        get ModbasName from ModRefNr
Mod_mNam_mdb        get ModbasName from basicModelNr

Mod_fget_names__    get lists of all subModels, PTAB-Surfs and MSH-Surfs
Mod_fget_names_0    get all subModels, PTAB-Surfs and MSH-Surfs from file
Mod_get_namAll      scan rekursiv die SourceFiles aller basicModels;
Mod_get_namSub      scan file of Model mb->mnam for Submodels; add Submodels
Mod_get_namFil      scan File fNam and save all Submodelcalls as level iLev

Mod_allmod_MS       in all subMods modify ModSiz

List_functions_end:
=====================================================
see also ../xa/xa_mod_gui.c   Mod_*

SEE ALSO:
DB_get_ModRef      get Ditto from Index
DB_get_ModBas      returns ModelBas from its index
DB_get_ModNr       get (basic-) ModelNr from Modelname
DB_list_ModBas     Modelnames in korrekter Reihenfolge -> Datei
DB_mdlNam_iBas     get the name of a basic-model from its basicModelNr
DB_mdlNam_iRef     get Modelname from DB-index of the Modelreference

\endcode *//*----------------------------------------



=====================================================
Probleme / Fehler:
=====================================================


  Externe Models duerfen nur Mockup-Model, jedoch keine subModels und keine
  externen-native-Models haben !

Istzustand:
Beim Laden eines externen Models mit subModels werden sofort
  subModels angelegt; das MainModel geht aber in die Binsen !
  (sollte eigentlich auch als subModel angelegt werden ...)
Was wenn in einem externen Model ein externes Model ?
  Muesste eigentlich auch vorher gesucht und geladen werden (genauso wie
    die subModels !

Was wenn es nun doppelte subModelNames gibt ?
  Wenn schon vorhanden, nicht laden.




=====================================================
Funktionsweise:
=====================================================
Siehe Progr.Doku "Files im temp-Directory"


  M1="filename" point [Scale] [vz [vx]]
  M1="filename" point [Scale] refsys

filename:
  "symbolic_Dir/modelName.modelTyp"        mit Pfad ('/'): external Model.
  "internal_subModelName"                  keine Pfadangabe: internal subModel
  "Catalogname/Partname"                   Catalogpart
  
  symbolic_Dir: see file dir.lst   (Format: "symbol pfad")




..................................
Es wird eine Liste aller Submodels gespeichert; das sind die BasicModels.
 Es wird auch fuer externe Models und fuer (externe) Mockup-Models (zB .wrl)
 ein BasicModel angelegt.
 Diese Liste ist permanent und wird nur bei OpenFile geloescht.

Funktionen zu BasicModels:   (DB-Tab mdb_dyn[DYN_MB_SIZ])
 DB_StoreModBas
 DB_get_ModBas
 DB_get_ModNr ..




..................................
Die Submodelaufrufe (Dittos) werden als ModelRef-structs gespeichert.

Funktionen zu ModelRefs:   (DB-Tab mdr_tab[APT_MR_SIZ])
 DB_StoreModel
 DB_get_ModRef
 

..................................
Die Modelnames werden in der DB in einem eigenen Bereich char mdb_nam[]
  gespeichert.



..................................
Ablauf resolv internalModel:

UI_butCB |butEND|
 ED_work_END
  ED_work_CurSet
   ED_Run          <------------RRR
    ED_Read_Line
    WC_Work__
     Mod_load_allsm
      Mod_load_sm
       ED_Run      >-------------RRR
     WC_Work1



..................................

Abarbeitung external-native:
  Mod_get_names
    speichert es als BaseModel, typ=0.
  Mod_load_allsm>Mod_load_sm
    arbeitet Model (native) ab, stellt es aber nicht dar (invis. Ditto).
  APT_DrawModel>GR_DrawModel>GL_DrawModel
    stellt das fertige Ditto dar

Abarbeitung wrl-Mockup:
  Mod_get_names
    speichert es als BaseModel, typ=10.
  Mod_load_allsm>Mod_load_sm          - tut nix.
  APT_DrawModel>GR_DrawModel>GL_DrawModel
    Beim ersten Call: Mockup from File laden (TSU_imp_tess)
    further calls:   stellt das fertige Ditto dar







...............................................................
1) vor dem Normalen Ablauf:
 - alle Filenames fuer Dittos in eine Liste;
   textbuffer (Filenames, (mit Pfad)
   Liste der Filenameadressen und des typs; Typ = intern oder File.
   Oder: 1, libpath, 2=Filename, 3=interner name, 0 (end of List)
  #define ModNamSiz 1024
  char ModNamTab[ModNamSiz]

  Funktion Mod_init_path ()
   alle Pfade aus Datei path.setup auslesen und speichern
  Funktion Mod_get_path (char out_path, char in_symbol);
   // returns "../dat/" if (in_symbol == NULL)
  Funktion Mod_get_names ();
    scan gesamten APT-Buffer nach Modelnames.
  Funktion Mod_add_nam (int mode)
    add libpath, Filename oder name of internal Model to ModNamTab.
    add name also to ModBasTab (if not yet present).


2)
 - alle Dittos Laden, aber nicht anzeigen.
   Tabelle anlegen;
   #define ModBasSiz 500;
   ModelBas ModBasTab[ModBasSiz];
   int      ModBasNr;
   Die DL-Startindizes und DL-Size in eigener Ditto-Tabelle merken.
   Der nun aktuelle DL-Index ist der normale DL-Startindex (die Dittos bis
   hierher schuetzen - nur zum Redraw benutzen).

  Funktion Mod_load_all ()
    Alle Models der ModBasTab laden und DLind und DLsiz setzen.



3) waehrend des normale Ablaufs (im Run:
  Spaeter wird das Model geladen; dann wird DL-StartIndex und DL-Size
  gespeichert. In dazugehoerigen Model-Record !


  Funktion Mod_get_modNr (char modelname);
   "get ModelNr from Modelname" (Index -> ModBasTab).
   APT_DrawModel  ...


=====================================================
subModeltypes:
MBTYP_EXTERN usw.




=====================================================
*/


#ifdef _MSC_VER
#include "MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>                    // isdigit
#include <stdarg.h>

#include "../gui/gui_types.h"          // UI_Func..

#include "../ut/func_types.h"          // UI_FuncSet
#include "../ut/ut_geo.h"
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_itmsh.h"            // MSHIG_EDGLN_.. typedef_MemTab.. Fac3
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_txfil.h"            // UTF_GetnextLnPos
#include "../ut/ut_txTab.h"            // TxtTab
#include "../ut/ut_os.h"
#include "../ut/ut_cast.h"             // INT_PTR

#include "../db/ut_DB.h"               // DB_cPos

#include "../xa/xa_uid.h"              // UID_Main_title
#include "../xa/xa_msg.h"              // MSG_ok,
#include "../xa/xa_mem.h"              // mem_cbuf1
#include "../xa/xa_ed_mem.h"           // APED_..

#include "../xa/xa.h"                  // AP_mod_fnam



// typedef_MemTab(int);
// typedef_MemTab(Point);
// typedef_MemTab(Fac3);
// typedef_MemTab(EdgeLine);



//-------------------------------
// aus xa.c:
extern  Point     WC_mod_ori;            // der Model-Origin
// extern char      AP_mod_dir[128], AP_mod_sym[64]; // Verzeichnis fuer OPEN
// extern char      AP_mod_fnam[128];
// extern char      AP_modact_nam[128];
extern  int       WC_modact_ind;         // -1=primary Model is active;
                                         // else subModel is being created

extern  int       AP_src;                // AP_SRC_MEM od AP_SRC_EDI
extern ColRGB     AP_defcol;


// aus xa_ui.c:
extern int        UI_InpMode;





static ModelBas   *Mod_father;


static char       sSecEnd[]="SECTIONEND";



//-------------------------------
// lokale prototypen:
  void Mod_chg_CB (char *modNam);



//================================================================
   int Mod_ck_isMain () {
//================================================================
/// Mod_ck_isMain       check if the active model is the mainmodel
/// retCode      0   subModel is active
///              1   the main-model is active

// test if AP_modact_nam == ""  (mainmodel = ""; else subModelname)

  return ((AP_modact_nam[0] == 0) ? 1 : 0);

}


//================================================================
  int Mod_mdbi_mdri (long ir) {
//================================================================
// get basicModelNr of refModel <ir>


  ModelRef  *mr1;

  mr1 = DB_get_ModRef (ir);

  return mr1->modNr;

}


//================================================================
  int Mod_mNam_mdb (char *mNam, int *sSiz, int *typ, int imdb) {
//================================================================
// Mod_mNam_mdb        get ModbasName from basicModelNr
// sSiz unused

  ModelBas  *mb1;

  // printf("Mod_mNam_mdb imdb=%ld siz=%d\n",imdb,*sSiz);

  mb1 = DB_get_ModBas (imdb);
    // printf(" mnam=|%s| typ=%d\n",mb1->mnam,mb1->typ);

  strcpy (mNam, mb1->mnam);
  *typ = mb1->typ;

  return 0;

}

//================================================================
  int Mod_mNam_mdr (char *mNam, int *sSiz, int *typ, long irMdl) {
//================================================================
// Mod_mNam_mdr              get ModbasName & modelTyp from ModRefNr

  ModelRef  *mr1;

  // printf("Mod_mNam_mdr mr=%ld siz=%d\n",irMdl,*sSiz);

  // get refMdl
  mr1 = DB_get_ModRef (irMdl);

  return (Mod_mNam_mdb (mNam, sSiz, typ, mr1->modNr));

}
 

//================================================================
  int Mod_m3_mdr (Mat_4x3 mat1, long irMdl) {
//================================================================
/// Mod_m3_mdr          get matrix of referenceModel

  ModelRef  *mr1;

  // get refMdl
  mr1 = DB_get_ModRef (irMdl);

  // get matrix of referenceModel 
  UT3D_m3_load_povxvz (mat1, &mr1->po, &mr1->vx, &mr1->vz);

  return 0;

}


//====================================================================
  int Mod_file2model (char *buf, FILE *fpi) {
//====================================================================
/// \code
/// save submodel > tmp/Model_<name>
/// Input: buf="SECTION MODEL modelNam"
/// copy all lines from file fpi into tmp/Model_<name>
/// irc < 0 = Error;
/// \endcode

  char fnam[256];
  FILE *fpo;


  // printf("Mod_file2model |%s|\n",&buf[6]);

  // buf = "SECTION MODEL mnam"
         // 012345678901234

  sprintf(fnam,"%sModel_%s",OS_get_tmp_dir(),&buf[14]);
  if((fpo=fopen(fnam,"w")) == NULL) {
    TX_Print("Mod_file2model E001 %s",fnam);
    return -1;
  }



  for(;;) {
    if(fgets(buf, mem_cbuf1_SIZ, fpi) == NULL) return -1;
    // printf("   -smLine: |%s|\n",buf);
    // if(!strncmp(buf, "MODEL ", 6)) break;
    if(!strncmp(buf, "SECTION", 7)) break;
    fprintf(fpo,"%s",buf);
  }


  fclose(fpo);

  return 0;

}


//====================================================================
  int Mod_init__ () {
//====================================================================
/// Bei jedem Init und File/new:
/// - clear all tmp/Model_*
/// - set AP_modact_nam="" (main)


  DB_StoreModBas (0, NULL);     // delete all basic models  2015-11-28

  AP_modact_nam[0] = '\0';
  WC_modact_ind    = -1;

  return 0;

}

/*
///====================================================================
  int Mod_cre_CB (void *parent, void *data) {
///====================================================================
/// create new SubModel:
/// save new submodelname;
/// provide new (empty) Modelspace

  int    irc;
  char   cbuf1[256], mNam[64];


  // Cancel ?
  if(data == NULL) { irc = -1; goto L_exit; }

  irc = 0;

  printf("Mod_cre_CB |%s|\n",(char*)data);

  // provide new (empty) Modelspace
  UTF_clear_ ();                    // Clear Mem


  // make safeName from new subModelname
  // Mod_safeName (newNam, fnam);
  strcpy(cbuf1, (char*)data);
  UTX_safeName (cbuf1, 1); // change '. /\\'

  // check maxLen; max 62 chars
  if(strlen(cbuf1) > 62) {
    TX_Print("max Laenge = 62 chars; gekuerzt !!");
    cbuf1[62] = '\0';
  }
  strcpy(mNam, cbuf1);


  // create 1. Line
  sprintf(cbuf1, "# Submodel %s", mNam);
  UTF_add_line (cbuf1);
  ED_load__ ();  // Memory > Editfenster
  UNDO_clear ();                 // clear Undo-List
  AP_Init1 ();   // init Displist, WC_main ..


  // save active submodelname
  strcpy(AP_modact_nam, mNam);


  // display submodelname im Titlebar
  Mod_chg_tit ();


  // activate model in browser
  Brw_sMdl_act (mNam);


  L_exit:
  // remove window
  GUI_GetText_CB (NULL, (void*)UI_FuncKill);


  return irc;

}

///====================================================================
  int Mod_cre__ () {
///====================================================================
/// create new SubModel:
/// - save active Model (AP_modact_nam) -> tmp/Model_<submodelname>
/// - ask for Modelname; callback -> Mod_cre_CB

  printf("Mod_cre__\n");


  // save the active Submodel AP_modact_nam -> TempFile
  Mod_sav_tmp ();


  // ask for new Modelname
  GUI_GetText(" Name of Submodel: ","Submodel1", -200, Mod_cre_CB);

  return 0;

}



///====================================================================
  int Mod_m2s_CB (void *parent, void *data) {
///====================================================================
/// move Mainmodel > Submodel

static char *fnam;

  char  cbuf1[256], newNam[256], mNam[64];


  // Cancel ?
  
  if(data == NULL) goto L_exit;
  if(INT_PTR(data) == UI_FuncCancel) goto L_exit;


  printf("Mod_m2s_CB |%s|\n",(char*)data);

  fnam = data;



  // make safeName from new subModelname
  // Mod_safeName (newNam, fnam);
  strcpy(cbuf1, fnam);
  UTX_safeName (cbuf1, 1); // change '. /\\'

  // check maxLen; max 62 chars
  if(strlen(cbuf1) > 62) {
    TX_Print("max Laenge = 62 chars; gekuerzt !!");
    cbuf1[62] = '\0';
  }
  strcpy(mNam, cbuf1);


  if(INT_PTR(data) == UI_FuncOK) goto L_work;

  sprintf(newNam,"%sModel_%s",OS_get_tmp_dir(),mNam);
  if(OS_checkFilExist(newNam,1) == 1) {
    // sprintf(newNam,"  overwrite submodel %s ?  ",mNam);
    MSG_get_1 (newNam, 256, "OVER_mdl", "%s", mNam);
    GUI_DialogYN (newNam, Mod_m2s_CB);
    return 0;
  }



  // rename
  L_work:
  sprintf(cbuf1,"%sModel_%s",OS_get_tmp_dir(),AP_modact_nam);
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


  // remove entrywidget
  L_exit:
  GUI_GetText_CB (NULL, (void*)UI_FuncKill);

  return 0;

}


///====================================================================
  int Mod_m2s__ () {
///====================================================================
/// move Mainmodel > Submodel

  printf("Mod_m2s__\n");


  // save the active Submodel AP_modact_nam -> TempFile
  Mod_sav_tmp ();

  // ask for new Modelname
  GUI_GetText(" Name of Submodel: ","Submodel1", -200, Mod_m2s_CB);

  return 0;

}



///====================================================================
  int Mod_chg__ () {
///====================================================================
/// activate other Submodel
/// - save active Model (AP_modact_nam) -> tmp/Model_<submodelname>
/// - provide List of Submodelnames
/// - Selection -> Mod_chg_CB

  char  fnam[256];

  printf("Mod_chg__\n");


  // save the active Submodel AP_modact_nam -> TempFile
  Mod_sav_tmp ();


  //--------------------------------------------------------------
  // provide List of Submodelnames (<basedir>/tmp/Model_*) -> fnam
  Mod_mkList (0);


  //--------------------------------------------------------------
  // display List of Submodelnames
  GUI_List3 (6, "", NULL, NULL, NULL, NULL);   // clear Filtertext
  sprintf(fnam,"%sMod.lst",OS_get_tmp_dir());
  GUI_List1 ("select Model", fnam, Mod_chg_CB);



  return 0;

}
*/



//====================================================================
  int Mod_mkList (int mode) {
//====================================================================
/// make list of all Submodels -> <baseDir>/tmp/Mod.lst
/// mode = 0: mit "-main-"
/// mode = 1: ohne "-main-"
/// Mainmodel wird mit "-main-" in die Liste eingetragen !


  int   iNr;
  char  fnam[256], *p1;
  FILE  *fpo;

  // printf("Mod_mkList %d\n",mode);


  sprintf(fnam,"%sMod.lst",OS_get_tmp_dir());
    // printf(" listfile=|%s|\n",fnam);

  // try to open outfile
  if((fpo=fopen(fnam,"w")) == NULL) {
    TX_Print("Mod_chg__ E001 %s",fnam);
    return -1;
  }

  sprintf(fnam,"%s",OS_get_tmp_dir());
  // printf(" scan |%s|\n",fnam);


  iNr = 0;
  OS_dir_scan_ (fnam, &iNr);
  if(iNr > 0)  {

    for (;;) {

      OS_dir_scan_ (fnam, &iNr);
      if(iNr < 0) break;
        // printf(" nxt file:  %d |%s|\n",iNr,fnam);

      // strip directory - find last "/"
      p1 = strrchr (fnam, fnam_del);
      if(p1) ++p1;
      else p1 = fnam;
        // printf(" nxt file:  %d |%s|%s|\n",iNr,p1,fnam);

      // check "Model_"
      if(strncmp(p1, "Model_", 6)) continue;

      if(!strcmp(p1, "Model_")) {
        if(mode == 0) { fprintf(fpo,"-main-\n"); }
        continue;
      }

      fprintf(fpo,"%s\n",&p1[6]);
        // printf("   %d |%s|\n",iNr,&p1[6]);
    }
  }

  fclose(fpo);



  return 0;

}


//====================================================================
  void Mod_chg_CB (char *modNam) {
//====================================================================
/// \code
/// - load new active SubModel
/// - set AP_modact_nam = new active Modelname
/// - RUN
/// \endcode

  int     i1;


  // printf("Mod_chg_CB |%s|\n",modNam);

  i1 = Mod_get_typ2 (modNam);   // give typ from Modelname
  // must be internal = -1
  if(i1 != -1) {
    TX_Print ("*** this is an external model - use File/Open Model ..");
    return;
  }


  // load & run SubModel
  Mod_chg_x (modNam);

  // activate model in browser
  Brw_sMdl_act (modNam);

  return;

}


//====================================================================
  int Mod_chg_x (char *modNam) {
//====================================================================
/// Mod_chg_x        activate internal subModel from modelname
/// new Submodelname selected;
/// - load new active SubModel
/// - set AP_modact_nam = new active Modelname
/// - RUN

  // int    mTyp;
  char   fNam[256];

  if(modNam == NULL) return 0;  // cancel 


  // printf("Mod_chg_x |%s|\n",modNam);


  TX_Print("activate subModel %s", modNam);


  // set AP_modact_nam = new active Modelname
  if(!strcmp(modNam, "-main-")) {
    AP_modact_nam[0] = '\0';

  } else {
    strcpy(AP_modact_nam, modNam);
  }


  // load new active SubModel
  sprintf(fNam,"%sModel_%s", OS_get_tmp_dir(), AP_modact_nam);
    // printf(" load |%s|\n",fNam);
  ED_new_File (fNam); // Datei ins Memory einlesen


  // den DYNAMIC_DATA - Block suchen abarbeiten eliminieren
  ED_work_dyn ();


  // disp submodelname im Titlebar
  Mod_chg_tit ();


  // RUN (abarbeiten)
  ED_work_END (0);


  // im CAD und MAN-Modus:
  if(UI_InpMode != UI_MODE_VWR) ED_enter ();

  return 0;

}


//====================================================================
  int Mod_chg_tit () {
//====================================================================
/// display submodelname im Titlebar


  char   cbuf[256];


  strcpy(cbuf, AP_mod_fnam);

  // if(strlen(AP_modact_nam) > 0) {
  if(!Mod_ck_isMain()) {
    strcat(cbuf, " / ");
    strcat(cbuf, AP_modact_nam);
  }

  UI_AP (UI_FuncSet, UID_Main_title, cbuf);


  return 0;

}


//================================================================
  int Mod_sav_ck (int mode) {
//================================================================
/// \code
/// Mod_sav_ck          save Model+Submodel -> File native; compare with last saved
/// test if model is modified;
///  mode      0=init (copy Model -> Mod_in)
///            1=compare Model - Mod_in
///  retCode:  0=files_not_different, -1=different_files (only for mode=1)
///
/// When loading new Model, after saving modified model: copy -> Mod_in.
/// Check modified: compare active Model <tmpDir>/Model with <tmpDir>/Mod_in
/// \endcode

  int  irc;
  char fnTmp[256], fn1[256];


  // printf("Mod_sav_ck %d\n",mode);


  sprintf(fnTmp, "%sMod_in", OS_get_tmp_dir());
  sprintf(fn1,   "%sModel",  OS_get_tmp_dir());

  //----------------------------------------------------------------
  // mode=0   init (copy Model -> Mod_in)
  if(!mode) {
    OS_file_copy (fn1, fnTmp);
    return 0;
  }


  //----------------------------------------------------------------
  // mode=1   compare
  // save Model+Submodels into tempDir as "<tmpDir>/Model" native
  irc = Mod_sav_i (0);
  if(irc < 0) return irc;


  // compare Mod_in - Model
  // irc = OS_file_compare_A (fnTmp, fn1);
  irc = Mod_sav_cmp__ (fnTmp, fn1);
    // printf(" Mod_sav_ck compare=%d\n",irc);


  return irc;

}


//================================================================
  int Mod_sav_cmp_i (char *ln1) {
//================================================================
// test if line can be ignored
// retCode:   1=ignore_line;  0=test_line
// ignore also DEFTX DEFCOL CONST_PL (will be added to subModel when activated)

  if(!strncmp(ln1, "MODBOX ", 7))   return 1;
  if(!strncmp(ln1, "MODSIZ ", 7))   return 1;
  if(!strncmp(ln1, "VIEW ", 5))     return 1;

  if(!strncmp(ln1, "DEFCOL ", 7))   return 1;
  if(!strncmp(ln1, "DEFTX ", 6))    return 1;
  if(!strncmp(ln1, "CONST_PL ", 9)) return 1;


  return 0;

}

 
//================================================================
  int Mod_sav_cmp__ (char *fnM1, char *fnM2) {
//================================================================
/// \code
/// Mod_sav_cmp__         compare <tmpDir>/Model mit <tmpDir>/Mod_in
/// RetCode:  0=files_not_different, 1=different_files
/// \endcode

  int     irc = 0, ieof=0;
  long    llM1, llM2, lNrM1, lNrM2;
  char    *lnM1, *lnM2;  // linebuffers
  FILE    *fpM1 = NULL, *fpM2 = NULL;


  //----------------------------------------------------------------
  // test filesizes
  // PROBLEM: MODBOX VIEW records can make different filesize !
  // if(OS_FilSiz(fnM1) != OS_FilSiz(fnM2)) goto L_errEx;


  //----------------------------------------------------------------
  // open model1
  if ((fpM1 = fopen (fnM1, "r")) == NULL) {
    printf ("ERROR open %s\n",fnM1);
    goto L_errEx;
  }


  //----------------------------------------------------------------
  // open model2
  if ((fpM2 = fopen (fnM2, "r")) == NULL) {
    printf ("ERROR open %s\n",fnM2);
    goto L_errEx;
  }


  //----------------------------------------------------------------
  // get space for the next 2 lines
  lnM1 = APED_SRCLN_BUF__;
  // lnM2 = (char*) MEM_alloc_tmp (APED_SRCLN_BUFSIZ);
  lnM2 = memspc201;


  //----------------------------------------------------------------
  // get next line of model1
  L_nxtM1:
  if(fgets (lnM1, APED_SRCLN_BUFSIZ, fpM1) == NULL) {  // printf(" EOF M1\n");
    ++ieof; goto L_nxtM2; }
  llM1 = strlen(lnM1);
  if(llM1 <= 1) goto L_nxtM1;
    // printf(" M1 %ld |%s|",llM1,lnM1);
  // test if line can be ignored
  if(Mod_sav_cmp_i(lnM1)) goto L_nxtM1;


  //----------------------------------------------------------------
  // get next line of model2
  L_nxtM2:
  if (fgets (lnM2, APED_SRCLN_BUFSIZ, fpM2) == NULL) {  // printf(" EOF M2\n");
    ++ieof; goto L_nxtCmp; }
  llM2 = strlen(lnM2);
  if(llM2 <= 1) goto L_nxtM2;
    // printf(" M2 %ld |%s|",llM2,lnM2);
  // test if line can be ignored
  if(Mod_sav_cmp_i(lnM2)) goto L_nxtM2;


  //----------------------------------------------------------------
  // 2 lines to compare ?
  L_nxtCmp:
  if(ieof) {
    if(ieof == 1) goto L_errEx;             // ieof=1: premature end of 1 file
    goto L_exit;                            // ieof=2: finished, OK.
  }

  // compare
  if(llM1 != llM2) goto L_errEx;
  if(memcmp(lnM1, lnM2, llM1)) goto L_errEx;  // objDat already exists

  // ok, both lines equal;
  goto L_nxtM1;


  //----------------------------------------------------------------
  L_errEx:
  irc = 1;
    printf("  _sav_cmp__-1-|%s|\n",lnM1);
    printf("  _sav_cmp__-2-|%s|\n",lnM2);



  L_exit:
  if(fpM1) fclose (fpM1);
  if(fpM2) fclose (fpM2);

    printf(" ex Mod_sav_cmp__ %d\n",irc);


  return irc;

}


//====================================================================
  int Mod_sav__ (int iCompress, char *fnOut, int savActMdl) {
//====================================================================
/// Mod_sav__           save Model+Submodel -> File native
/// Input:
///   filNam     outfilename absolute
///   iCompress  0=do not compress; else compress.
///   savActMdl  0=save active model, 1=do not save active model

  int  irc;
  char fnTmp[256], fn1[256];


  sprintf(fnTmp, "%sModel", OS_get_tmp_dir());

  // save Model+Submodels into tempDir as "<tmpDir>/Model" native
  irc = Mod_sav_i (savActMdl);
  if(irc < 0) return irc;

  // make a copy (copy Model -> Mod_in) for check-modified
  Mod_sav_ck (0);


  TX_Print("Save File %s",fnOut);


  //================================================================
  // compress ?

  if(iCompress) {
    // compress, native.
    // change filetyp "gcad" -> "gcaz"
    strcpy(fn1, fnOut);
    fn1[strlen(fn1) - 1] = 'z';
    OS_file_zip (0, fnTmp, fnOut);


  } else {
    // copy tempFile -> outfile
    OS_file_copy (fnTmp, fnOut);
  }

    // printf("ex Mod_sav__\n");

  return 0;

}


//====================================================================
  int Mod_sav_i (int savActMdl) {
//====================================================================
/// \code
/// Mod_sav_i         save Model+Submodels into tempDir as "Model" native
/// Input:
///   filNam     outfilename absolute
///   savActMdl  0=save active model, 1=do not save active model
///
/// - savActMdl=0:    save the active Submodel AP_modact_nam -> File
/// - join all files tmp/Model_* into file tmp/Model
/// - mainModel = tmp/Model_; subModels = tmp/Model_*
/// \endcode

  int    irc, i1, ii;
  long   l1;
  char   cbuf[256], fnam0[256], fnam1[256], *p1;
  FILE   *fp1, *fpo;
  Memspc mSpc1;
  // void  *vp1;

  UtxTab_NEW (sTab1);                 // processes
  UtxTab_NEW (mdlTab);                // stringtable
  UtxTab_NEW (surPtab);               // stringtable
  UtxTab_NEW (surMsh);                // stringtable

  MemTab(Point) pTab = _MEMTAB_NUL;
  MemTab(Fac3) fTab = _MEMTAB_NUL;
  MemTab(EdgeLine) eTab = _MEMTAB_NUL;
  MemTab(int) eDat = _MEMTAB_NUL;


  printf("Mod_sav_i savActMdl=%d\n",savActMdl);

  irc = -1;

  UI_block__ (1, 1, 1);  // block all


  // fix box if necessary
  if(AP_mdlbox_invalid_ck()) {
    // get box of active model
    UT3D_box_mdl__ (&AP_box_pm1, &AP_box_pm2, -1, 0);
    AP_mdlbox_invalid_reset ();
  }


  // save the active sub- or mainmodel AP_modact_nam -> TempFile
  if(!savActMdl) Mod_sav_tmp ();


  // get lists of all subModels, PTAB-Surfs and MSH-Surfs of subModelfiles
  irc = Mod_fget_names__ (&mdlTab, &surPtab, &surMsh);
  if(irc < 0) {
      // printf("ex Mod_sav__ -1\n");
    UI_block__ (0, 0, 0);  // unblock all
    return -1;
  }


  // set temp.outfilename <fNam>
  sprintf(fnam0,"%sModel",OS_get_tmp_dir());


  // try to open outfile
  if((fpo=fopen(fnam0,"w")) == NULL) {
    TX_Print("Mod_sav_i E001 %s",fnam0);
    goto L_err1;
  }



  //================================================================
  // add all Submodels to file fnam0
  if(mdlTab.iNr < 1) goto L_PTAB;

  for(ii = 0; ii < mdlTab.iNr; ++ii) {
    p1 = UtxTab__ (ii, &mdlTab);      // get word from index

    sprintf(fnam1,"%sModel_%s",OS_get_tmp_dir(),p1);
    if(OS_checkFilExist(fnam1, 1) == 0) {
      sprintf(cbuf, " submodel %s not found ..",p1);
      GUI_MsgBox (cbuf);
      continue;
    }

    fprintf(fpo, "SECTION MODEL %s\n",p1);

    // cat file tmp/Model_<mdlnam>
    if(UTX_cat_file (fpo, fnam1) < 0) {
      TX_Print("Mod_sav_i E003 %s",fnam1);
      return -1;
    }

    fprintf(fpo,"%s\n",sSecEnd);

  }


  //================================================================
  // add PTAB's
  L_PTAB:
  if(surPtab.iNr < 1) goto L_MSH;

  MemTab_ini (&pTab, sizeof(Point), Typ_PT, 10000);

  for(ii = 0; ii < surPtab.iNr; ++ii) {
    p1 = UtxTab__ (ii, &surPtab);      // get word from index

    sprintf(fnam1,"%s%s.ptab",OS_get_tmp_dir(),p1);
      printf(" PTAB[%d] |%s|%s|\n",ii,p1,fnam1);

    MSH_bload_pTabf (&pTab, fnam1);

    // save ptab ascii
    MSH_asav_pTabf (pTab.data, pTab.rNr, p1, fpo);
  }
  MemTab_free (&pTab);


  //================================================================
  // add MSH's
  L_MSH:
  if(surMsh.iNr < 1) goto L_procs;

  MemTab_ini (&fTab, sizeof(Fac3), Typ_Fac3, 10000);
  MemTab_ini (&eTab, sizeof(EdgeLine), Typ_EdgeLine, 10);
  MemTab_ini (&eDat, sizeof(int), Typ_Int4, 50);

  for(ii = 0; ii < surMsh.iNr; ++ii) {
    p1 = UtxTab__ (ii, &surMsh);      // get word from index

    sprintf(fnam1,"%s%s.msh",OS_get_tmp_dir(),p1);
      printf(" MSH[%d] |%s|%s|\n",ii,p1,fnam1);
    MSH_bload_fTabf (&fTab, &eTab, &eDat, fnam1);

    // save mesh ascii
    MSH_asav_fTabf (&fTab, &eTab, &eDat, p1, fpo);

  }

  MemTab_free (&eDat);
  MemTab_free (&eTab);
  MemTab_free (&fTab);



  //================================================================
  // add processes
  L_procs:
  i1 = PRC_lst_processes ();
  if(i1 < 1) goto L_appDat;

  // get list of all processes
  // UtxTab_add_file (&sTab, fnam1);
  UME_init (&mSpc1, memspc55, sizeof(memspc55));
  UtxTab_init_Memspc (&sTab1, &mSpc1);

  // add file to stringTab
  sprintf(fnam1, "%sprocesses.lst",OS_get_tmp_dir());
  i1 = UtxTab_add_file (&sTab1, fnam1);
  if(i1 < 0) goto L_appDat;
    // UtxTab_dump (&sTab1);

  // loop tru sTab1; cat file tmp/Model_<mdlnam>
  for(i1=0; i1 < UtxTab_nr(&sTab1); ++i1) {
    fprintf(fpo, "SECTION PROCESS %s\n",UtxTab__(i1, &sTab1));
    sprintf(fnam1, "%s%s",OS_get_tmp_dir(),UtxTab__(i1, &sTab1));
    if(UTX_cat_file (fpo, fnam1) < 0) {
      TX_Print("Mod_sav_i E-prc-1 %s",fnam1);
    }
    fprintf(fpo, "%s\n",sSecEnd);
  }


  //================================================================
  // add ApplicationData
  L_appDat:

  // get a list of files "<temp>/*.appdat" -> sTab
  sprintf(cbuf, "%slst.dat", OS_get_tmp_dir());
  ii = UTX_dir_listf (cbuf, OS_get_tmp_dir(), NULL, ".appdat");
  if(ii < 1) goto L_main;
    printf("add -appDat-\n");

  if((fp1=fopen(cbuf,"r")) == NULL) {
    TX_Print("Mod_sav_i E008 %s",cbuf);
    goto L_main;
  }

  while (!feof (fp1)) {
    if (fgets (cbuf, 250, fp1) == NULL) break;
    UTX_CleanCR (cbuf);
    UTX_ftyp_cut (cbuf);      // remove filetype

    // skip file if size = 0
    sprintf(fnam1,"%s%s.appdat",OS_get_tmp_dir(),cbuf);
    l1 = OS_FilSiz (fnam1);
    if(l1 < 3) continue;

    // cat file tmp/<appnam>.appdat
    fprintf(fpo, "SECTION APPDAT %s %ld\n",cbuf,l1);
    if(UTX_cat_file (fpo, fnam1) < 0) {
      TX_Print("Mod_sav__ E007 %s",fnam1);
      return -1;
    }

    // fprintf(fpo,"%s\n",sSecEnd);
    fprintf(fpo,"\n%s\n",sSecEnd);
  }



  //================================================================
  // add -main-
  L_main:
  // cat file tmp/Model_
    // printf("add -main-\n");
  sprintf(fnam1,"%sModel_",OS_get_tmp_dir());
  if(UTX_cat_file (fpo, fnam1) < 0) {
    TX_Print("Mod_sav_i E006 %s",fnam1);
    goto L_err2;
  }



  irc = 0;
  if(fpo) {fclose(fpo); fpo = NULL;}


  //================================================================
  L_err2:
  if(fpo) fclose(fpo);

  L_err1:
  UtxTab_free (&mdlTab);              // free mem
  UtxTab_free (&surPtab);             // free mem
  UtxTab_free (&surMsh);              // free mem


  // if editor is active: reload
  EDE_reload ();


  UI_block__ (0, 0, 0);  // unblock all

  return irc;

}


//================================================================
  int Mod_savSubBuf1 (char *modNam, int modSiz) {
//================================================================
/// save Submodel in Buffer1 + active Hidelist --> File tmp/Model_<modNam>
/// used by Step-Import (schreibt mit UTF_add1_line --> UTF_FilBuf1)
/// see also Grp_exp ..


  char     fNam[256], s1[160];
  FILE     *fp1;


  // printf("Mod_savSubBuf1 |%s|\n",modNam);


  sprintf(fNam,"%sModel_%s",OS_get_tmp_dir(),modNam);
    // printf(" fNam=|%s|\n",fNam);

  // write Line -> File
  if((fp1=fopen(fNam,"wb")) == NULL) {
    TX_Print("Mod_savSubBuf1 E001 |%s|",fNam);
    return -1;
  }

  sprintf(s1, "MODSIZ %d\n",modSiz);
  fputs(s1, fp1);


  // write out the PermanentAttributes - HIDE, G#, SSTYLS ..
  // do not check if att exists (error with STEP-Import)
  GA_fil_wri (fp1, 0, 1, 0);
    // printf(" nach GA_fil_wri\n");

  fputs(":DYNAMIC_DATA\n", fp1);

  UTF_file_Buf1_w (fp1);
    // printf(" nach UTF_file_Buf1_w\n");

  fclose(fp1);

  // printf("ex Mod_savSubBuf1 |%s|\n",modNam);
  // sprintf(s1,"cat %s",fNam);OS_system (s1);printf("eof %s\n",fNam);


  return 0;

}
 

//=====================================================================
  int Mod_sav2file_CB (char *fnam,char *dirNam) {
//=====================================================================
// hier fehlt check for overwrite File ...
// OFFEN:
//   sollte alle erforderlichen subModels auch mitkopieren !!!


  char  oldNam[256], newNam[256];


  if(fnam == NULL) return 0;   // cancel

  printf("Mod_sav2file_CB |%s|%s|\n",dirNam,fnam);

  Mod_sav_tmp ();

  // copy <tmp/Model_<AP_modact_nam>> -> <dirNam,fnam>
  sprintf(oldNam,"%sModel_%s",OS_get_tmp_dir(),AP_modact_nam);

  sprintf(newNam,"%s%s",dirNam,fnam);
  UTX_ftyp_cut (newNam); // remove Filetyp
  strcat(newNam, ".gcad");

  TX_Print("Save File %s",newNam);
  OS_file_copy (oldNam, newNam);

  return 0;

}


//====================================================================
  int Mod_sav2file__ () {
//====================================================================
/// save the active Submodel AP_modact_nam -> permanent File

  int  irc;


  printf("Mod_sav2file__\n");

  // if(strlen(AP_modact_nam) < 1) {
  if(Mod_ck_isMain()) {
    GUI_MsgBox ("ERROR: subModel must be active ..");
    // TX_Error("es ist kein Submodel aktiv ..");
    return -1;
  }

/*
  // get Filename
  AP_get_fnam_symDir (cbuf);   // get filename of dir.lst
  // sprintf(cbuf,"%sxa%cdir.lst",OS_get_bas_dir(),fnam_del);
  GUI_save__ ("save model as",   // titletext
            AP_mod_dir,           // path
            cbuf,                  // directoryList
            AP_modact_nam,             // defaultModelname
            (void*)Mod_sav2file_CB);
*/
  irc = AP_save__ (2, "gcad");
  // if(irc < 0) return -1;


  return 0;

}



//=====================================================================
  int Mod_LoadSubmodel (char *fnam,char *dirNam) {
//=====================================================================
/// Models/load Submodel from File
/// does: AP_mod_dir = dirNam

// TODO: use Mod_SM_add_file

  int   ift;
  char  oldNam[256], newNam[256], smNam[256];


  if(fnam == NULL) return 0;

  // printf("Mod_LoadSubmodel |%s|%s|\n",fnam,dirNam);


  // check Filetyp; muss native sein
  ift = Mod_get_ftyp1 (fnam);
  if(ift != 0) {
    // GUI_Dialog (NULL, "ERROR:  Submodels must be native Modeldata ");
    TX_Print ("ERROR:  Submodels must be native Modeldata ");
    OS_beep ();
    return 0;
  }


  strcpy(AP_mod_dir, dirNam);

  // remove Filtyp from Filname --> smNam
  strcpy(smNam, fnam);
  UTX_ftyp_cut (smNam);


  sprintf(oldNam,"%s%s",dirNam,fnam);


  // if new submodel has submodels, must explode it !
  // get first line; test for "SECTION "
  UTX_fgetLine (newNam, 256, oldNam, 1);
  if(!strncmp (newNam, "SECTION ", 8)) {
    GUI_MsgBox ("selected model has submodels, cannot (yet) load ..");
    return 0;
  }



  // copy <dirNam><fnam>   <tmp/Model_<fnam>
  sprintf(newNam,"%sModel_%s",OS_get_tmp_dir(),smNam);
  TX_Print("Load Submodel %s  from File  %s",smNam,newNam);
  OS_file_copy (oldNam, newNam);

  // update Mod.lst
  Mod_mkList (0);
  // update BrowserWindow
  Brw_Mod_add (smNam);

  return 0;

}


//====================================================================
  int Mod_LoadFile__ () {
//====================================================================
/// Model als Submodel laden

  char cbuf[256];

  printf("Mod_LoadFile__\n");

/*
  AP_get_fnam_symDir (cbuf);   // get filename of dir.lst
  // sprintf(cbuf,"%sxa%cdir.lst",OS_get_bas_dir(),fnam_del);
  GUI_List2 ("load Submodel from File",    // titletext
            AP_mod_dir,       // Pfadname des activeDirectory
            cbuf,              // Liste der directories
            (void*)Mod_LoadSubmodel);
*/

  APP_Open ("load Submodel from File", "*.gcad", Mod_LoadSubmodel);

  return 0;

}


//====================================================================
  int Mod_sav_tmp () {
//====================================================================
/// \code
/// save the active sub- or mainmodel AP_modact_nam -> TempFile
/// AP_modact_nam = "" - mainmodel; else submodel
/// \endcode

  extern int DL_wri_dynDat0 (FILE*);

  char  fnam[256];


  // printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX \n");
  // printf("Mod_sav_tmp |%s|\n",AP_modact_nam);


  // fix modelname
  sprintf(fnam,"%sModel_%s",OS_get_tmp_dir(),AP_modact_nam);
    // printf("WWWWWWWWWWWWWWWWWWWWWWWWWW  out_>|%s|\n",fnam);


  // vom Editor ins Memory kopieren
  if(AP_src == AP_SRC_EDI) ED_unload__ ();


  // // DYNAMIC_AREA (im Memory) zufuegen
  // DL_save_DYNAMIC_AREA ();

  // Memory in Datei rausschreiben
  // UTF_wri_file (fnam, 1);
  UTF_wri_file (fnam, DL_wri_dynDat0);

  // // DYNAMIC_AREA (im Memory) wieder wegloeschen
  // UTF_del_rest(":DYNAMIC_AREA");


  return 0;

}



//====================================================================
  int Mod_init_path () {
//====================================================================
// alle Pfade aus Datei path.setup auslesen und speichern
// beide speichern mit DB_cPos; die Pointer von beiden wo speichern ?

  // printf("Mod_init_path\n");

  return 0;

}


//====================================================================
  int Mod_get_path (char *out_path, char *in_path) {
//====================================================================
/// \code
/// Input:
///   in_path   pathSymbol/filename" or "filename"
///             without Symbol: "<bas_dir>dat/filename".
/// Output:
///   out_path  filename complete (Path + Filename + Filetyp).
///   RetCod    0 = OK;
///            -1 = Symbol_not_found
/// \endcode


  int    i1;
  char   cbuf[256], *p1;


  // printf("Mod_get_path |%s|\n",in_path);

  if(in_path == NULL) return -1;


  // InternalModelnames may not have char fnam_del
  p1 = strchr (in_path, '/');
  if(p1 != NULL) goto L_ext;


  //------------------------------------------------------------
  // InternalModel:
  sprintf(out_path,"%sModel_%s",OS_get_tmp_dir(),in_path);
  goto L_fertig;



  //------------------------------------------------------------
  // ExternalModel:  (has not '/')
  L_ext:
  // get path from symbol
  if(Mod_sym_get1 (cbuf, in_path, 0) < 0) return -1;
    // printf("ex-sym_get1 |%s|\n",cbuf);
  ++p1; // skip the fnam_del
  sprintf(out_path,"%s%s",cbuf,p1);




  //------------------------------------------------------------
  L_fertig:
    // printf("ex Mod_get_path |%s|\n",out_path);

  return 0;

}


//================================================================
   int Mod_get_ftyp1 (char *fnam) {
//================================================================
/// Mod_get_ftyp1                      give Modeltyp from Filename
/// RetCod=fTyp (see AP_iftyp_ftyp)


  int  ift;
  char ftyp[16];


  ift = UTX_ftyp_s (ftyp, fnam, 1);
  if(ift == 0) ift = AP_iftyp_ftyp (ftyp);
  else         ift = 0; // native

  // printf("ex Mod_get_ftyp1 %d |%s|",ift,fnam);

  return ift;

}


//================================================================
   int Mod_get_typ1 (char *txbuf, char *cbuf) {
//================================================================
/// give typ from definition Line and extract (complete) Modelname --> txbuf
/// Input:
///   cbuf   definitionLine; zB   M20=CTLG "Profile/4x4_60"  ..
/// OutPut:
///   RetCod:
///     -1 = internal model   MBTYP_INTERN
///     -2 = catalog-model    MBTYP_CATALOG
///     -3 = Error

// ATTENTION:
// MBTYP_INTERN can return a reference to the real Modelname eg "M20" !

// M#="mdlNam" refsys
// M#=CTLG "ctlgNam" refsys
// M#=MIR M# mirror


  int  mode, ityp;
  char *cp1, ftyp[16];


// neue Version 2006-02-09:  ohne MOCK



  // printf("Mod_get_typ1 |");UTX_dump_cnl (cbuf, 50);printf("|\n");


  cp1 = strchr(cbuf, '=');
  if(cp1 == NULL) return -3;
  ++cp1;


  // check for catalog-model  (M20=CTLG "...")
  UTX_pos_skipLeadBlk (cp1);
  if(!strncmp(cp1, "CTLG", 4)) {
    cp1 += 4;
    ityp = -2;
  } else {
    ityp = 0;
  }


  // auf next word nach "M#=" positionieren
  cp1 = strchr(cp1, '"');
  if(cp1 == NULL) {
    // if(strlen(cp1) < 2)
    return -3;
    // return -1;
  }
  ++cp1;


  // copy Modelname (incl.Path) --> txbuf
  UTX_cp_word_term (txbuf, cp1, '"');
    // printf("Mod_get_typ1 fnam=|%s|\n",txbuf);


  // test for "M20"; das waere eine bereits existierendes Model;
  // also kein neues Basismodel
  if(txbuf[0] == 'M') {
    // test if word is numeric
    if(UTX_ck_num_f (&cp1, &txbuf[1]) == 0) return -3;
  }



  if(ityp == -2) {
    return -2;
  }



  return Mod_get_typ2 (txbuf);

}


//================================================================
   int Mod_get_typ2 (char *txbuf) {
//================================================================
/// Mod_get_typ2  give typ from (complete) Modelname  (internal or external!)
/// (using filetype of modelname)
/// -3 = Error
/// -2 = catalog-model      MBTYP_CATALOG
/// -1     internal model   MBTYP_INTERN       // M1 = "Submodel1"
///  0     external/native  MBTYP_EXTERN
/// 1-9    external
/// 10-19  Mockup           Mtyp_WRL-Mtyp_WRL2
/// 20-29  Image            Mtyp_BMP-Mtyp_JPG
// 
// old Version:
// mode = 1 = internal model      // M1 = "Submodel1"
// mode = 2 = external model      // M1 = "Data/fnam.dat" ..
// mode = 3 = MockupModel         // M1 = MOCK "Data/xy.wrl" ..
// mode = 4 = Image-BMP     // unused; dzt via N20=IMG P(0 0 0) "Data/Bild1.bmp"

  int  mode;
  char *cp1, ftyp[16];



  // printf("Mod_get_typ2 |%s|\n",txbuf);

// neue Version 2006-02-09:  ohne MOCK

  // find '/' in Modelname: external; else internal
  cp1 = strchr (txbuf, '/');
  if(cp1 == NULL) {mode = -1; goto L_done;}        // else -1=internal


  // test filetyp
  UTX_ftyp_s (ftyp, txbuf, 1);         // get ftyp in UC
  mode = AP_iftyp_ftyp (ftyp);            // returns ftyp as int else 0

  // 0=native    
  // 1-9=Import 
  // 10-19=Mockup

  L_done:
    // printf("ex Mod_get_typ2 %d |%s|\n",mode, txbuf);
  return mode;



/*
  // auf next word nach "M#=" positionieren
  cp1 = strchr(cbuf, '=');
  if(cp1 == NULL) return -1;
  ++cp1;
  while (*cp1  == ' ')  { ++cp1; }


  mode = 1;

  if(!strncmp(cp1, "MOCK", 4)) {
    // printf(" Mockup-Model !\n");
    mode = 3;
    cp1 += 4;
    while (*cp1  == ' ')  { ++cp1; }
  }

  // copy Modelname (incl.Path) --> txbuf
  if(*cp1 == '"') {
    ++cp1;
    UTX_cp_word_term (txbuf, cp1, '"');

  } else {
    UTX_cp_word__ (txbuf, cp1);
  }


  // find '/' in Modelname: external; else internal
  if(mode == 1) {
    cp1 = strchr (txbuf, '/');
		if(cp1) mode = 2;         // 2=external, else 1=internal
  }

  // printf("ex Mod_get_typ1 %d |%s|\n",mode, txbuf);

  return mode;
*/
}


//====================================================================
  int Mod_get_names () {
//====================================================================
/// \code
/// scan gesamten APT-Buffer nach Modelnames.
/// siehe ED_work_END - DYNAMIC_AREA abarbeiten,
/// \endcode


  int  i1, i2, mode;
  char *cbuf, *c1buf, *cp1, txbuf[80];


  // printf("Mod_get_names\n");


  // Mode MAN: Update nach modification
  if(AP_src == AP_SRC_EDI)
    ED_unload__ (); // Editfenster > Memory



  // // delete Models (mdb_dyn-Records)
  // // reset den Name-Buffer in der DB
  // DB_StoreModBas (0, NULL);


  cbuf = UTF_GetnextLnPos (NULL); // Startpos

  for(;;) {
    c1buf = UTF_GetnextLnPos (cbuf);
    if(c1buf == NULL) break;
    cbuf = c1buf;
    if(cbuf[0] != 'M') continue;
    if(!isdigit(cbuf[1])) continue;
    // save modelname as mdb_dyn-Record
    Mod_get_namStore (cbuf, -1, 0);
  }

    DB_dump_ModBas ();
      // printf("ex Mod_get_names\n");

  return 0;

}


//================================================================
  int Mod_sym_dump () {
//================================================================
/// Mod_sym_dump        dump all symbolic directories

  char    s1[256];
  FILE    *fpi;


  AP_get_fnam_symDir (s1);   // get filename of dir.lst


  if((fpi=fopen(s1,"r")) == NULL) {
    return -1;
  }

  while (!feof (fpi)) {
    if(fgets (s1, 256, fpi)) {
      if(s1[0] != '#') printf("%s",s1);
    }
  }
  fclose(fpi);



  return 0;

}

//================================================================
  int Mod_sym_dir__ (char *sym, char *dir, char *basDir) {
//================================================================
// Mod_sym_dir__ get symbolic-directory from any-directory
// RF 2018-05-23
// Input:
//   dir     relative, absolute or symbolic directory; with closing '/'
//           size must be >= 128
//   sym     size must be >= 64
//   GLOBAL: basDir
// Output:
//   sym     name of symbolic-directory, empty = none;
//   dir     absolute directory
//   retCod  0=OK, -1=no-symDir, dir=path
//

  int  irc;
  char s1[128];


  // printf("----------------------------------- \n");
  // printf("Mod_sym_dir__ dir=|%s| basDir=|%s|\n",dir,basDir);

  sym[0] = '\0';


  //----------------------------------------------------------------
  // test if dir is absolute-dir; yes: make it absolute-dir
  // if(dir[0] == fnam_del) goto L_abs;
  if(!OS_ck_DirAbs(dir)) goto L_abs;

  // not absolute-dir; symbol or no directory at all -
  if(dir[0] != '.') {
    if(dir[0] == '\0') {
      // empty dir; use basDir
      strcpy(dir, basDir);
      UTX_add_fnam_del (dir);   // add closing "/"
      // get symbol
      goto L_abs;
    }
    // first char is not '.' - symbol ?
    // test if dir is listed in file <cfg>/dir.lst; yes: get dir -> s1
    strcpy (sym, dir);
#ifdef _MSC_VER
    UTX_del_foll_chrn (sym, "/\\");
#else
    UTX_endDelChar (sym, fnam_del);  // remove closing '/'
#endif
    irc = Mod_sym_getAbs (dir, sym);
      // printf("ex Mod_sym_getAbs %d |%s|%s|\n",irc,dir,sym);
    goto L_exit;
  }


  // first char is '.' - is relative-dir.
  // get s1 = absolute from relative.
  strcpy (s1, dir);
  UTX_fnam_fnrel (dir, 128, s1, basDir);



  //----------------------------------------------------------------
  L_abs:
  // dir is absolute-dir. Get symbol.
  // test if dir is listed in file <cfg>/dir.lst; yes: get symbol -> sym
  irc = Mod_sym_get2 (sym, dir, 0);
    // printf("ex-Mod_sym_get2 %d |%s|\n",irc,sym);


  //----------------------------------------------------------------
  L_exit:

    // printf("ex-Mod_sym_dir__ irc=%d sym=|%s| dir=|%s|\n",irc,sym,dir);

  return irc;

}


//================================================================
  int Mod_sym_get__ (char *symDir, char *absDir, char *fnam,
                     char *inPath) {
//================================================================
/// Mod_sym_get__       get symbolic-directory & absolute-path from filename
// DO NOT USE - replaced by Mod_sym_dir__
//
// Input:
//   inPath        filename; absolut, relativ or symbolic.
// Output:
//   symDir        maxSiz 64         eg AP_mod_sym
//   absDir        maxSiz 128        eg AP_mod_dir
//   fnam          maxSiz 128        eg AP_mod_fnam

// see also UTX_fnam_s UTX_fdir_s Mod_sym_get2
// check file /home/fwork/gCAD3D/cfg/dir.lst



  int      irc;
  char     s1[512], actDir[128], *p1;
#ifdef _MSC_VER
  char     *p2;
#endif


  // printf("Mod_sym_get__ |%s|\n",inPath);


  if(!inPath) goto L_err1;
  if(strlen(inPath) < 1) goto L_err1;


  // -) separate directory, filename
  // see also UTX_fnam_s
  strcpy(s1, inPath);

  p1 = strrchr(s1, '/');  // find last FilenamedelimiterChar
  if(!p1) {
    p1 = s1;
  } else {
    *p1 = '\0';
    ++p1;     // skip fnam_del
  }

#ifdef _MSC_VER
  p2 = strrchr(s1, '\\');
  if(p2) {
    if(p2 > p1) {
      *p2 = '\0';
      ++p2;
      p1 = p2;
    }
  }
#endif

  // copy filename out
  if(strlen(p1) > 128) goto L_err2;
  strcpy(fnam, p1);


  //----------------------------------------------------------------
  // -) check if rel. or abs. or symbolic directory

  // skip leading blanks
  p1 = s1;
  UTX_pos_skipLeadBlk (p1);


  // check for absolute path
  //   Linux: first char "/"
  //   MS: "/" or "\" or 2.char ":")
  if(*p1 == '/') goto L_dir_abs;
#ifdef _MSC_VER
  if(*p1 == '\\') goto L_dir_abs;
  if(p1[1] == ':') goto L_dir_abs;
#endif


  // check for relative path;
  // relative path must start with '.'
  if(*p1 == '.') goto L_dir_rel;


  // must be a symbolic path;
  goto L_dir_sym;



  //----------------------------------------------------------------
  L_dir_abs:
    // printf(" L_dir_abs:\n");

  if(strlen(p1) > 128) goto L_err3;
  strcpy(absDir, p1);
  UTX_add_fnam_del (absDir);   // add closing "/"

  // -) get symbolic-dir from abs.dir
  irc = Mod_sym_get2 (symDir, absDir, 0);
  goto L_exit;



  //----------------------------------------------------------------
  L_dir_rel:
    // printf(" L_dir_rel:\n");

  // use existing symbolic-path
  strcpy(actDir, absDir);

  // -) get abs.dir from rel.dir
  irc = UTX_fnam_rel2abs (absDir, 128, p1, actDir);


  goto L_exit;



  //----------------------------------------------------------------
  L_dir_sym:
    // printf(" L_dir_sym:\n");

  if(strlen(p1) > 64) goto L_err4;
  strcpy(symDir, p1);

  // -) get abs.dir from symbolic-dir.
  irc = Mod_sym_getAbs (absDir,  symDir);



  //----------------------------------------------------------------
  L_exit:
     // printf("ex-Mod_sym_get__ |%s|%s|%s|\n",absDir,symDir,fnam);

  return irc;


  L_err1:              // inPath empty
    return -1;
  L_err2:              // size fnam too small
    return -2;
  L_err3:              // size absDir too small
    return -3;
  L_err4:              // size symDir too small
    return -4;

}


//================================================================
  int Mod_fNam_sym (char *s2, char *s1) {
//================================================================
/// \code
/// Mod_fNam_sym                     get absolute-filename from symbolic-filename
/// copy absolute filenames, else resolve symbolic-directory
/// Input     s1    symbolic-filename or absolute-filename
/// Output    s2    absolute-filename
///           Retcod:   0     OK
///                    -1     filename not symbolic and not absolute.
/// \endcode

  int    il;
  char   *p1, ss[64];

  // printf("Mod_fNam_sym |%s|\n",s1);


  //----------------------------------------------------------------
  // test for absolute filename; do not modify
  if(s1[0] == fnam_del) {
    strcpy(s2,s1);
    goto L_exit;
  }

  //----------------------------------------------------------------
  // p1 = cut directory
#ifdef _MSC_VER
  // must check for '/' AND '\' (in MS '/' can come from out of source)
  p1 = strpbrk (s1, "/\\");
#else
  p1 = strchr (s1, fnam_del);
#endif
  if(!p1) return -1;

  il = p1 - s1;
  strncpy (ss, s1, il);
  ss[il] = '\0';

  // get s2 = absolute-directory
  Mod_sym_getAbs (s2, ss);

  // add filename
  il = strlen(s2);
  ++p1;
  strcpy(&s2[il], p1);

  //----------------------------------------------------------------
  L_exit:
    // printf(" ex-Mod_fNam_sym |%s|\n",s2);


  return 0;

}


//================================================================
  int Mod_fNam_get (char *fn) {
//================================================================
/// \code
/// Mod_fNam_get        get sym,dir,fnam,ftyp from symbolic- or abs.filename
/// Output:
///   global AP_mod_sym, AP_mod_dir, AP_mod_fnam, AP_mod_ftyp
///   RetCod:    0=OK, -1=no-symDir, dir=path
/// \endcode

  int   irc;

  // printf("Mod_fNam_get |%s|\n",fn);

  // separate/copy directory,fileName,fileTyp of full filename
  UTX_fnam__ (AP_mod_dir, AP_mod_fnam, AP_mod_ftyp, fn);
    // printf("ex-UTX_fnam__ |%s|%s|%s|\n",AP_mod_dir, AP_mod_fnam, AP_mod_ftyp);

  // get integer-filetyp of AP_mod_ftyp
  AP_mod_iftyp = AP_iftyp_ftyp (AP_mod_ftyp);
 
  // get  symDir from directory 
  irc = Mod_sym_dir__ (AP_mod_sym, AP_mod_dir, OS_get_bas_dir());
    // printf("ex-Mod_fNam_get irc=%d |%s|%s|\n",irc,AP_mod_sym,AP_mod_dir);

  return irc;

}


//================================================================
  int Mod_fNam_set (char *fNam, int mode) {
//================================================================
// Mod_fNam_set        get symbolic-filename or absolute-filename
//  full-filename: <directory|symbol>/<filename>.<filetyp>
// 2018-05-24 RF
//
// Input:
//   mode      0=make-absolute-filename; 1=make-symbolic-filename
// Output:
//   mode      0   absolute-filename
//             1   full-filename <directory|symbol>/<filename>.<filetyp>
//   fNam      <directory|symbol>/<filename>.<filetyp>    size >= 256
//             AP_mod_dir AP_mod_sym AP_mod_fnam AP_mod_ftyp


  // printf("Mod_fNam_set %d\n",mode);
  // printf("   |%s|%s|%s|\n",AP_mod_sym,AP_mod_fnam,AP_mod_ftyp);

  if((mode) && (AP_mod_sym[0])) {
    // make symbolic-filename
    if(AP_mod_ftyp[0])
      sprintf(fNam,"%s/%s.%s",AP_mod_sym,AP_mod_fnam,AP_mod_ftyp);
    else 
      sprintf(fNam,"%s/%s",AP_mod_sym,AP_mod_fnam);

  } else {
    // 0: make absolute-filename
    if(AP_mod_ftyp[0])
      sprintf(fNam,"%s%s.%s",AP_mod_dir,AP_mod_fnam,AP_mod_ftyp);
    else 
      sprintf(fNam,"%s%s",AP_mod_dir,AP_mod_fnam);
  }

    // printf("ex-Mod_fNam_set |%s|\n",fNam);

  return 0;

}


//====================================================================
  int Mod_sym_getAbs (char *absDir, char *symDir) {
//====================================================================
/// \code
/// Mod_sym_getAbs    get absoute-direcory from symbolic-directory
/// symdir must be terminated with '/'
/// Input:
///   symDir:   eg: "Data"
/// Output:
///   absDir:   full path (from file xa/dir.lst)     Size 128.
///   RetCod
///     >= 0    OK; path in out_path; Linenumber of symbol in path.
///     -1      Symbol from in_path not found in file
/// \endcode

  char    fn[256];


  // printf("Mod_sym_getAbs |%s|\n",symDir);

  AP_get_fnam_symDir (fn);   // get filename of dir.lst
    // printf(" fn-dir.lst=|%s|\n",fn);


  // return UTX_setup_get (absDir, symDir, fn, 0);
  return UTX_setup_get__ (absDir, symDir, fn);

}


//====================================================================
  int Mod_sym_get1 (char *out_path, char *in_path, int imod) {
//====================================================================
/// \code
/// get path from symbol
/// see also Mod_sym_getAbs
/// symdir must be terminated with '/'
/// Input:
///   in_path:  "<symbol>/<filename>"  eg: "Data/Niet1.dat"
///   imod:     0=get 1=check(no Msg)
/// Output:
///   out_path: full path (from file xa/dir.lst)  Size 256.
///             filename not added; use Mod_get_path for path/filename
///   RetCod
///     >= 0    OK; path in out_path; Linenumber of symbol in path.
///     -1      Symbol from in_path not found in file
/// \endcode

  int     lNr;
  char    cbuf[256], sbuf[256], s_act[128], *p1, *p2;
  FILE    *fpi;


  // printf("Mod_sym_get1 |%s| %d\n",in_path,imod);
  // printf("  AP_mod_dir=|%s|\n",AP_mod_dir);


  lNr = 0;


  // skip 1. char if it is |"|
  if(*in_path == '"') ++in_path;


  // copy the symbol -> sbuf
  strcpy(sbuf, in_path);
  p1 = strchr (sbuf, '/');
  if(p1 == NULL) p1 = strchr (sbuf, '\\');
  if(p1 == NULL) {
    TX_Error("Mod_sym_get1 E001");
    return -1;
  }
  *p1 = '\0';
    // printf(" sbuf=|%s|\n",sbuf);


  // replace symDir "." mit dem active directory
  if (!strcmp (sbuf,".")) {
    strcpy(out_path, AP_mod_dir);
    goto L_exit;
  }



  // try to open inListe
  AP_get_fnam_symDir (cbuf);   // get filename of dir.lst
  // sprintf(cbuf,"%sxa%cdir.lst",OS_get_bas_dir(),fnam_del);
  if((fpi=fopen(cbuf,"r")) == NULL) {
    TX_Print("Mod_sym_get1 E001-file %s not found",cbuf);
    return -1;
  }

  while (!feof (fpi)) {
    ++lNr;
    if (fgets (cbuf, 256, fpi) == NULL) {
      if(imod == 0) {
        TX_Error("Symbolic directory  \"%s\"  not found",sbuf);
        TX_Print("   (add it with Standards/Directories in dir.lst)",sbuf);
        printf ("**** Mod_sym_get1 E002\n");

      }
      fclose(fpi);
      return -1;
    }
       
    p1 = strchr (cbuf, ' ');
    if(p1 == NULL) continue;
    *p1 = '\0';
      // printf(" s_act=|%s|\n",cbuf);

    if(!strcmp(sbuf, cbuf)) break;
  }

  fclose(fpi);

  ++p1;
  p2 = UTX_pos_1n (p1);
  strcpy(out_path,p2);
  UTX_CleanCR (out_path);


  L_exit:
    // printf("ex Mod_sym_get1 %d |%s|%s|\n",lNr,out_path,in_path);

  return lNr;

}



//====================================================================
  int Mod_sym_get2 (char *out_sym, char * in_path, int imod) {
//====================================================================
/// \code
/// get symbol from path
/// Input:
///   in_path:    der Pfad (mit "/" am Ende)
///   imod:       0=get 1=check(no Msg)
/// Output:
///   out_sym:    das zugehoerige Symbol (aus datei xa/dir.lst)
///               filename not added; use Mod_get_path for path/filename
///   RetCod 0    OK; symTxt in out_sym;
///         -1    in_path not found in file
/// \endcode

  char    cbuf[256], *p1;
  FILE    *fpi;


  // printf("Mod_sym_get2 |%s|\n",in_path);


  // try to open inListe
  AP_get_fnam_symDir (cbuf);   // get filename of dir.lst
  // sprintf(cbuf,"%sxa%cdir.lst",OS_get_bas_dir(),fnam_del);

  if((fpi=fopen(cbuf,"r")) == NULL) {
    TX_Error("Mod_sym_get2 E001-file %s not found",cbuf);
    return -1;
  }

  while (!feof (fpi)) {
    if (fgets (cbuf, 256, fpi) == NULL) {
      if(imod == 0) {
        // TX_Error("Mod_sym_get2 E002-path %s not found",in_path);
        TX_Print("Mod_sym_get2 E002-path %s not found",in_path);
        AP_get_fnam_symDir (cbuf);   // get filename of dir.lst
        TX_Print("               in symFile %s",cbuf);
      }
      out_sym[0] = '\0';
      fclose(fpi);
      return -1;
    }
    UTX_CleanCR (cbuf);
    p1 = strchr (cbuf, ' ');
    if(p1 == NULL) continue;
    *p1 = '\0';
    ++p1;
    p1 = UTX_pos_1n (p1);
    // printf(" s_act=|%s|\n",p1);

    if(!strcmp(p1, in_path)) break;
  }

  fclose(fpi);

  strcpy(out_sym, cbuf);

  // printf("ex Mod_sym_get2 |%s|%s|\n",out_sym,in_path);

  return 0;

}


//================================================================
   int Mod_symOpen_set (char *sym, char *path) {
//================================================================
/// Mod_symOpen_set         set symbolic path
// see AP_set_dir_open

  strcpy(AP_mod_sym, sym);
  strcpy(AP_mod_dir, path);

  // update Title & Pfade oben auf den Mainwinrahmen
  UI_AP (UI_FuncSet, UID_Main_title, NULL);

  return 0;

}


//================================================================
  int Mod_sym_del (char *sym) {
//================================================================
/// Mod_sym_del         delete symbolic path

  char    fn[256];


  printf("Mod_sym_del |%s|\n",sym);

  AP_get_fnam_symDir (fn);   // get filename of dir.lst

  // delete sym
  UTX_setup_set (fn, sym, NULL);

  return 0;

}


//================================================================
   int Mod_sym_add (char *sym, char *dir) {
//================================================================
/// \code
/// Mod_sym_add         register symbolic path in File cfg/dir.lst
/// path comes from AP_mod_dir
/// \endcode

// see UI_open_symCB

  int    irc, ift, lNr;
  char   cbuf[256], fnam[256];


  printf("Mod_sym_add |%s|%s|\n",sym,dir);


  // copy symbol
  strcpy(cbuf, sym);


  // check if symbol already exists
  lNr = -1;
  strcat(cbuf, "/");
  lNr = Mod_sym_get1 (fnam, cbuf, 1);
    printf(" lNr Mod_sym_get1=%d\n",lNr);
  if(lNr >= 0) {
    TX_Print("**** Symbol %s already exists  overwritten !",cbuf);
    OS_beep ();
    // goto L_err1_sym;
  }




  // add symbol to file
  cbuf[strlen(cbuf)-1] = '\0';  // remove /
  strcat(cbuf, " ");
  strcat(cbuf, dir);

  AP_get_fnam_symDir (fnam);   // get filename of dir.lst
    // printf(" symfilnam=|%s|\n",fnam);
  // sprintf(fnam,"%sxa%cdir.lst",OS_get_bas_dir(),fnam_del);
  UTX_fsavLine (cbuf, fnam, 1024, lNr);


/*
  // check if symbol already exists
    irc = Mod_sym_get2 (cbuf, dir, 1);
    if(irc < 0) {
      TX_Print("**** Error: cannot resolv symbol for path |%s|",dir);
      OS_beep ();
    }
*/


  return 0;

}


/*
//====================================================================
  int Mod_load_bas (char *modNam, ModelRef *modR1) {
//====================================================================
// noch nicht geladenes Model laden
// (via ED_work_file abarbeiten; Memspc bleibt)
// - APT-Zeilennummer merken
// - DB -> Datei sichern (nicht BasisModelRecords)
// - RUN - abarbeiten
// - DB <- Datei rueckladen
// - APT-Zeilennummer ruecksetzen
// RetCod = die BasisModelNr des geladenen Model.


#define ILEV_MAX 10

static int ILEV;
static int lnr_act[ILEV_MAX];
static int modNr[ILEV_MAX];
static ModelRef modR2;

  char     fnam[256];
  ModelBas *mod1;



  if(modNam == NULL) {  // nur am beginn von RUN (ED_work_END)
    printf("Mod_load_bas INIT\n");
    ILEV = -1;
    return 0;
  }

  ++ILEV;
  if(ILEV >= ILEV_MAX) goto L_err3;

  printf("\n\nMod_load_bas |%s| ILEV=%d\n",modNam,ILEV);

  if(ILEV == 0) modR2 = *modR1;

  // merken; bei dieser Zeilennummer ED_lnr_act wieder weitertun ..
  // lnr_act[ILEV] = ED_get_lnr_act () - 1; // Zeile noch einmal !
  lnr_act[ILEV] = ED_get_lnr_act (); // Zeile noch einmal !
  printf(" lnr_act[%d]=%d\n",ILEV,lnr_act[ILEV]);


  // save gesamte DB -> File
  DB_save__ ();



  // neuer Modelname; store. (1=Filename)
  modNr[ILEV] = DB_StoreModBas (1, modNam);
  printf("  modNr[%d]=%d\n",ILEV,modNr[ILEV]);




  //---------------------------------------------------
  L_go_down:

  // ev hier DB loeschen ? Abarbeiten aus Datei jedesmal ganz von vorn !


  // die Adresse des BasisModelRecord holen
  mod1 = DB_get_ModBas (modNr[ILEV]);
  if(mod1 == NULL) goto L_err1;

  printf(" weiter m. BasModel %d |%s|\n",modNr[ILEV],mod1->mnam);


  // den Pfad holen
  // Mod_get_path (fnam, ModBasTab[iact].mnam);
  Mod_get_path (fnam, mod1->mnam);
  printf(" ModelFnam=|%s|\n",fnam);


  // DL-StartIndex speichern
  // ModBasTab[iact].DLind = GL_GetActInd();
  mod1->DLind = GL_Get_DLind();
  // vorab mal sichern (sonst bei kreuzweiser Verbindung problem !)
  // mod1->DLsiz = -modNr[ILEV]-1;   // -1 weil -0 nicht brauchbar !
  mod1->DLsiz = -1;
  printf("  mod1->DLind=%d siz=%d\n",mod1->DLind,mod1->DLsiz);


  DL_disp_def (OFF); // ab nun nix mehr anzeigen


  // RUN (abarbeiten)
  ED_Init ();
  if(ED_work_file (fnam) < 0) goto L_err2;
  ED_lnr_reset ();
  ED_Run ();


  // nach Run:
  printf("...............nach ED_Run ILEV=%d\n",ILEV);
  if(ILEV < 0) return -2;


  DL_disp_def (ON); // ab nun wieder alles anzeigen


  // DL-size speichern
  // ModBasTab[iact].DLsiz = GL_GetActInd() - ModBasTab[iact].DLind;
  mod1->DLsiz = GL_Get_DLind() - mod1->DLind;

  printf(">>>> Mod[%d]: mnam=|%s| DLind=%d DLsiz=%d\n",modNr[ILEV],
                 mod1->mnam,mod1->DLind,mod1->DLsiz);

  // Model-Origin speichern;
  // mod1->po = WC_mod_ori;
  mod1->po = UT3D_PT_NUL;



  // bei der alten Zeilennummer ED_lnr_act wieder weitertun ..
  ED_set_lnr_act (lnr_act[ILEV]);
  printf(">>>>>>>>>>>>>>>>>>>>>> down ILEV %d lNr %d\n",ILEV-1,lnr_act[ILEV]);
  --ILEV;


  if(ILEV >= 0) {
    goto L_go_down;
  }


  // L_fertig:
  // reload gesamte DB from File
  // wird hier Tabelle der BasModels geloescht ?
  DB_load__ ();


  // den ModRef wieder zurueckgeben ..
  *modR1 = modR2;


  L_exit:
  printf("ex Mod_load_bas %d\n",modNr[0]);
  return modNr[0];
  // return -2; // nix tun; Zeile muss nochmal bearbeitet werden !


  L_err1:
  TX_Error("Mod_load_bas E001");
  return -1;


  L_err2:
  TX_Error("Submodel %s does not exist",modNam);
  return -1;

  L_err3:
    TX_Error("Mod_load_bas E002-too many levels");
    return -1;

}
*/


//====================================================================
  int Mod_kill__ () {
//====================================================================
/// - alle tmp/Model_* loeschen
/// - alle tmp/*.tess loeschen
///
/// see also OS_file_delete OS_file_delGrp

  char  cbuf[256];


  // printf("Mod_kill__ \n");

  sprintf(cbuf,"%sMod.lst",OS_get_tmp_dir());
  OS_file_delete (cbuf);

  sprintf(cbuf, "%sModel_*",OS_get_tmp_dir());
  OS_file_delGrp (cbuf);

  sprintf(cbuf,"%sDB__*.dat",OS_get_tmp_dir());
  OS_file_delGrp (cbuf);

  sprintf(cbuf,"%sjoints",OS_get_tmp_dir());
  OS_file_delete (cbuf);

  // del process_* and processes.lst
  sprintf(cbuf, "%sprocess*",OS_get_tmp_dir());
  OS_file_delGrp (cbuf);

  sprintf(cbuf, "%s*.tess",OS_get_tmp_dir());
  OS_file_delGrp (cbuf);

  // catalog-parameterfiles
  CTLG_dat_del ();

  sprintf(cbuf, "%s*.ptab",OS_get_tmp_dir());
  OS_file_delGrp (cbuf);

  sprintf(cbuf, "%s*.msh",OS_get_tmp_dir());
  OS_file_delGrp (cbuf);

  sprintf(cbuf, "%s*.dlg",OS_get_tmp_dir());
  OS_file_delGrp (cbuf);

  sprintf(cbuf, "%s*.appdat",OS_get_tmp_dir());
  OS_file_delGrp (cbuf);

  sprintf(cbuf, "%s*.bmp",OS_get_tmp_dir());
  OS_file_delGrp (cbuf);

  // remove all PRCV-files
  sprintf(cbuf, "%s*.odat",OS_get_tmp_dir());
  OS_file_delGrp (cbuf);

  Tex_DelAll ();  // delete all OpenGL-textures



  return 0;

}


//====================================================================
  int Mod_load__ (int mode, char *filNam, int dbResiz) {
//====================================================================
/// \code
/// Mod_load__         load native model 
/// Ein ganz neues Model als MainModel oeffnen;
/// - alle tmp/Model_* loeschen
/// - file einlesen - aufteilen -> tmp/Model_*
/// - Submodel tmp/Model_ (-main-) laden
/// mode   0 = load new
/// mode   1 = insert; do not write Title & do not rescale
/// mode   2 = tmp/Model_ is ready to load         (reload)
/// - dbResiz = 0: increase DB-Size for new objects; 1 = not.
/// \endcode
 
  long  lTab[8], lNr, ll, i1;
  char  cbuf[256], *lBuf, *cp1;
  FILE  *fpi, *fpo=NULL;


  printf("Mod_load__ |%s| %d %d\n",filNam,mode,dbResiz);


  lBuf = mem_cbuf1;


  //----------------------------------------------------------
  // - alle tmp/Model_* loeschen
  if(mode == 0) Mod_kill__ ();

  if(mode == 2) goto L_load;

  
  
  //----------------------------------------------------------
  // if(mode == 0) || (mode == 1):
  // check for compressed Model
  UTX_cp_right (lBuf, filNam, 4);     // get 4 chars from right
  UTX_cp_word_2_upper (lBuf, lBuf);   // make uppercase
  if(strcmp(lBuf, "GCAZ") == 0) {
	// create fileName for decompressed model
    sprintf(cbuf,"%sModel",OS_get_tmp_dir());
    // decompress compressed model;
	  OS_file_zip(1, filNam, cbuf);
	  // file to load from ..
	  cp1 = cbuf;
	
  } else {
	  cp1 = filNam;
  }
  
  
  
  //----------------------------------------------------------
  // split file tmp/Model into tmp/Model_ (mainmodel) and 
  //   subModels tmp/Model_<subModelName>

  if((fpi=fopen(cp1,"r")) == NULL) {
    TX_Error("Mod_load__ E001 %s",cp1);
    return -1;
  }


  lNr = 0;
  while (!feof (fpi)) {

    if (fgets (lBuf, mem_cbuf1_SIZ, fpi) == NULL) break;
      // printf(" lBuf-len=%ld\n",strlen(lBuf));

    UTX_CleanCR (lBuf);
/*
      if((strlen(lBuf) > 128)&&(lBuf[0] != 'A')&&(lBuf[0] != 'S')) {
        printf("in:|%s|\n",lBuf);
        printf("len=%d\n\n",strlen(lBuf));
      }
*/

    // check first 10 Lines for binary chars
    // check only first 10 chars ..
    if(lNr < 1) {
      // skip checking commentLines (haben viele Sonderzeichen)
      // if(lBuf[0] == '#') goto L_cs;
      ll = strlen(lBuf);
      for(i1=0; i1<ll; ++i1) {
        // skip tab, lf, cr
        if(lBuf[i1] == '\t') continue;  // \n & \r check with UTX_CleanCR
    	  if((lBuf[i1] < 32)||(lBuf[i1] > 125)) {
          // printf(" lNr=%d ch=|%c| %d\n",lNr,lBuf[i1],lBuf[i1]);
    	    TX_Error("Model %s is not plain text",filNam);
    	    return -1;
    	  }
      }
    }
    ++lNr;


    L_cs:
    if(!strncmp(lBuf, "MODEL ", 6)) {
      Mod_aload_sm (0, lBuf, mem_cbuf1_SIZ, fpi);
      continue;
    }


    if(!strncmp(lBuf, "SECTION", 7)) {
      // printf(" SecLine=|%s|\n",lBuf);

      if(!strncmp(&lBuf[7], " PTAB ", 6)) {
        MSH_aload_pTab (lBuf, mem_cbuf1_SIZ, fpi);
        continue;
      }

      if(!strncmp(&lBuf[7], " MESH ", 6)) {
        MSH_aload_fTab (lBuf, mem_cbuf1_SIZ, fpi);
        continue;
      }

      // copy section "APPDAT" into file <temp>/<appNam>.appdat
      //   ascii|binary
      if(!strncmp(&lBuf[7], " APPDAT ", 8)) {
        appDat_aload (lBuf, mem_cbuf1_SIZ, fpi);
        continue;
      }

      // extract process -> <temp>process_<processName>
      if(!strncmp(&lBuf[7], " PROCESS ", 9)) {
        Mod_aload_prc (lBuf, mem_cbuf1_SIZ, fpi);
        continue;
      }

      // model-einlesen ev. aehnlich zusammenfassen wie MSH_aload_pTab ..
      if(!strncmp(&lBuf[7], " MODEL ", 7)) {
        Mod_aload_sm (1, lBuf, mem_cbuf1_SIZ, fpi);
        continue;
      }

      // skip unknown section
      TX_Print ("skip unknown %s",lBuf);
      while (!feof (fpi)) {
         if (fgets (lBuf, mem_cbuf1_SIZ, fpi) == NULL) break;
                         // 1234567890
         if(!strncmp(lBuf, sSecEnd, 10)) break;
      }

    }


    // open Mainmodel
    if(fpo == NULL) {
      sprintf(cbuf, "%sModel_",OS_get_tmp_dir());
        // printf(" mainmodel=|%s|\n",cbuf);
      if((fpo=fopen(cbuf,"w")) == NULL) {
        TX_Error("Mod_load__ E004 %s",cbuf);
        return -1;
      }
    }

    // Zeile raus ..
      // printf(" out=|%s|\n",lBuf);
    fprintf(fpo,"%s\n",lBuf);

  }

  fclose(fpi);
  if(fpo) fclose(fpo);



  //----------------------------------------------------------
  // Submodel tmp/Model_ (-main-) laden
  L_load:

  Mod_init__ (); // main  set AP_modact_nam ..


  // load new active SubModel
  sprintf(cbuf,"%sModel_",OS_get_tmp_dir());
    // printf("  _load_SM_ |%s|\n",cbuf);

  // get file <cbuf> into memory
  ED_new_File (cbuf);
    // printf("nach ED_new_File..\n");

  // work, then delete  DYNAMIC_DATA - Block
  ED_work_dyn ();


  if(dbResiz == 0) {
    APED_search_dbLimits (lTab);          // search highest indices in Model
    DB_size_set (lTab);                   // increase DB-size

  }


  return 0;

}


//================================================================
  int Mod_aload_prc (char *lBuf, int bufSiz, FILE *fpi) {
//================================================================
/// \code
/// read from fpi and write until SECTIONEND into file tmp/Model_<mdlNam>
/// lBuf is line "SECTION PROCESS process_<prcNam>"
///               01234567890123456
/// add proc to file <tmp>processes.lst
/// \endcode
        
  char    s1[256], s2[256], *p1;
  FILE    *fpo;


  // printf("Mod_aload_prc |%s|\n",lBuf);

  // extract processname
  strcpy(s1, &lBuf[16]);
  // p1 = strchr(s1, ' ');
  // if(p1) *p1 = '\0';
  // else {TX_Error("Mod_aload_prc E001\n"); return -1;}

  // sprintf(s2, "%sprocess_%s", OS_get_tmp_dir(), s1);
  sprintf(s2, "%s%s", OS_get_tmp_dir(), s1);
    // printf(" extract proc |%s|\n",s2);



  if((fpo=fopen(s2, "w")) == NULL) {
    TX_Error("Mod_aload_prc E002 %s", s2);
    return -1;
  }


  // add process to file  processes.lst
  sprintf(s2, "%sprocesses.lst", OS_get_tmp_dir());
  UTX_fsavLine (s1, s2, 80, -1);



  L_nxt:
    if (fgets (lBuf, bufSiz, fpi) == NULL) goto L_exit;
    UTX_CleanCR (lBuf);
    if(!strcmp(lBuf, sSecEnd)) goto L_exit;
    fprintf(fpo,"%s\n",lBuf);
    if(!feof (fpi)) goto L_nxt;


  L_exit:
  fclose (fpo);

  return 0;

}

      

//================================================================
  int Mod_aload_sm (int mode, char *lBuf, int bufSiz, FILE *fpi) {
//================================================================
/// \code
/// read from fpi and write until SECTIONEND into file tmp/Model_<mdlNam>
/// lBuf is line "SECTION MODEL <mdlNam>"
/// mode=0 old version; "MODEL modelname" bis "MODEL END"
/// mode=1 new version; "SECTION modelname" bis "SECTIONEND"
/// \endcode
 
  static char s0[]="MODEL END";
  int     namPos;
  char    fNam[256], *txEnd;
  FILE    *fpo;


  // "MODEL modelname"
  // "SECTION MODEL modelname"
  //  012345678901234
  if(mode == 0) {
    namPos = 6;
    txEnd = s0;
  } else {
    namPos = 14;
    txEnd = sSecEnd;
  }

  sprintf(fNam, "%sModel_%s",OS_get_tmp_dir(),&lBuf[namPos]);

    // printf("Mod_aload_sm |%s|\n",fNam);

  if((fpo=fopen(fNam,"w")) == NULL) {
    TX_Error("Mod_aload_sm E001 %s",fNam);
    return -1;
  }


  // i1 = 0;
  while (!feof (fpi)) {
    if (fgets (lBuf, bufSiz, fpi) == NULL) break;
    UTX_CleanCR (lBuf);
    if(!strcmp(lBuf, txEnd)) break;
    fprintf(fpo,"%s\n",lBuf);
    // ++i1;
  }
  // printf(" i1=%d\n",i1);

  fclose (fpo);

  return 0;

}


/*
///=================================================================
  int Mod_ren_CB (void *parent, void *data) {
///=================================================================
/// the callback if OK selected.

  char  cbuf1[256], newNam[256], mNam[64];

  if(data == NULL) return 0; // Cancel ?

  printf("Mod_ren_CB |%s|\n",(char*)data);

  if(strlen((char*)data) < 1) return -1;


  // make safeName from new subModelname
  // Mod_safeName (newNam, fnam);
  strcpy(cbuf1, (char*)data);
  UTX_safeName (cbuf1, 1); // change '. /\\'

  // check maxLen; max 62 chars
  if(strlen(cbuf1) > 62) {
    TX_Print("max Laenge = 62 chars; gekuerzt !!");
    cbuf1[62] = '\0';
  }
  strcpy(mNam, cbuf1);


  // rename <tmp/Model_<AP_modact_nam>> -> tmp/Model_<data>>
  sprintf(cbuf1,"%sModel_%s",OS_get_tmp_dir(),AP_modact_nam);
  sprintf(newNam,"%sModel_%s",OS_get_tmp_dir(),mNam);
  rename (cbuf1,newNam);

  // set name
  strcpy(AP_modact_nam, mNam);

  // fix title
  Mod_chg_tit ();

  // disp msg
  TX_Print(" subModel renamed to %s",mNam);


  // kill window
  GUI_GetText_CB (NULL, (void*)UI_FuncKill);


  // rename active subModel in browser
  Brw_sMdl_ren_CB (mNam);

  return 0;

}


///====================================================================
  int Mod_ren__ () {
///====================================================================
/// rename the active Submodel

  
  printf("Mod_ren__\n");

  if(strlen(AP_modact_nam) < 1) {
    TX_Error("es ist kein Submodel aktiv ..");
    return -1;
  }

  GUI_GetText(" new Submodelname: ",  AP_modact_nam, -200, Mod_ren_CB);


  return 0;

}
*/


//=====================================================================
  int Mod_del1__ (char *smNam) {
//=====================================================================
// the subModel to be deleted may NOT be active !

  int   idat;
  char  cbuf[256], *cp1;

  printf("Mod_del1__ |%s|\n",smNam);


  // del <tmp/Model_<AP_modact_nam>>
  sprintf(cbuf,"%sModel_%s",OS_get_tmp_dir(),smNam);
  printf("remove %s\n",cbuf);
  OS_file_delete (cbuf);
    
/*  ACHTUNG: das folgende loescht auch aus dem aktiven Modelspace !!!!!
  // clear
  UI_undo_clear ();                      // Clear Undo
  UTF_clear_ ();                         // Clear Mem
  ED_load__ (); // Memory > Editfenster
  AP_Init1 ();                           // clear DL, display
*/


  return 0;

}


/*
///=====================================================================
  int Mod_del_CB (void *parent, void *data) {
///=====================================================================

  int   idat;
  char  cbuf[256];

  printf("Mod_del_CB %d\n",INT_PTR(data));

  idat = INT_PTR(data);

  if(idat != UI_FuncOK) return -1;


  // del <tmp/Model_<AP_modact_nam>>
  Mod_del1__ (AP_modact_nam);


  // start "change submodel"
  Mod_mkList (0);
  // display new List of Submodelnames
  sprintf(cbuf,"%sMod.lst",OS_get_tmp_dir());
  GUI_List1 ("select Model", cbuf, Mod_chg_CB);


  return 0;

}

///====================================================================
  int Mod_del__ () {
///====================================================================

  char     cbuf[256];

  printf("Mod_del__\n");

  if(strlen(AP_modact_nam) < 1) {
    TX_Error("es ist kein Submodel aktiv ..");
    return -1;
  }

  sprintf(cbuf, "  delete Submodel %s  ",AP_modact_nam);
  GUI_DialogYN (cbuf, Mod_del_CB);

  return 0;

}



//====================================================================
  int Mod_load_all () {
//====================================================================
// Alle Models der ModBasTab laden und DLind und DLsiz setzen.
// DZT UNUSED

  int      irc, iact;
  char     fnam[128];
  ModelBas *mod1;


  printf("Mod_load_all\n");

  // return 0; // TEST

  // DL-Offset ganz an den Beginn (deletes Overlay-models)
  GL_fix_DL_base (0);

  // DL_disp_def (OFF); // ab nun nix mehr anzeigen


  iact = -1;

  
  //-------------------------------------------------------
  L_next:
  ++iact;

  // den BasisModelRecord holen
  mod1 = DB_get_ModBas (iact);
  if(mod1 == NULL) goto L_fertig;


  // den Pfad holen
  // Mod_get_path (fnam, ModBasTab[iact].mnam);
  Mod_get_path (fnam, mod1->mnam);
  // printf(" ...next %d |%s|\n",iact,fnam);

  
  // DL-StartIndex speichern
  // ModBasTab[iact].DLind = GL_GetActInd();
  mod1->DLind = GL_GetActInd();
  // printf("  mod1->DLind=%d\n",mod1->DLind);


  // RUN (abarbeiten)
  WC_mod_ori = UT3D_PT_NUL;

  ED_Init ();
  if(ED_work_file (fnam) < 0) goto L_next;

  ED_lnr_reset ();
  ED_Run ();


  // DL-size speichern
  // ModBasTab[iact].DLsiz = GL_GetActInd() - ModBasTab[iact].DLind;
  mod1->DLsiz = GL_GetActInd() - mod1->DLind;

  // printf(">>>> Mod.%d DLind=%d DLsiz=%d\n",iact,
         // mod1->DLind, mod1->DLsiz);

  // Model-Origin speichern;
  mod1->po = WC_mod_ori;



  goto L_next;
  //-------------------------------------------------------



  L_fertig:
  ED_Init ();
  ED_lnr_reset ();
  DL_disp_def (ON); // ab nun wieder alles anzeigen


  // den momentanen DL-Offset als Startindex fuer die normale Geometrie setzen
  GL_fix_DL_base (1);


  // test: display
  // Mod_test1();


  return 0;

}
*/


//================================================================
  int Mod_load_allsm () {
//================================================================
/// Load Models of mdb_dyn-Records; Load Models; Reihenfolge=seqNr.

  // static void *txTab1 = NULL;

  int      irc, actSeq, actMod, mbNr;
  int      il1;
  char     cbuf1[256];
  ModelBas *mb;


  // get nr of mb's
  mbNr = DB_get_ModBasNr();
    // printf("Mod_load_allsm %d\n",mbNr);

  // find highest seqNr
  actSeq = DB_ModBas_seqMax();


  // printf("============ Mod_load_allsm %d %d\n",mbNr,actSeq);

  if(mbNr < 1) return 0;   // nix to resolv ..

  actMod = WC_modact_ind;

  // UtxTab_clear (&txTab1);             // init (malloc ..)



  //----------------------------------------------------------------
  L_nxt_lev:
  // printf(" start load_allsm seq=%d\n",actSeq);

  // load all Models with seqNr=actSeq
  for(il1=0; il1<mbNr; ++il1) {
    mb = DB_get_ModBas (il1);
    if(mb->seqNr != actSeq) continue;

      // printf(" allsm nxt=%d mb.typ=%d nam=|%s|\n",il1,mb->typ,mb->mnam);

    // MockupModels werden on Demand (erst beim ersten use) geladen.
    if(mb->typ > 0) continue;

    // wenn das Model frueher schon geladen wurde -
    if(mb->DLind >= 0) continue;


    mb->DLind = GL_Get_DLind();

    WC_modact_ind = il1;          // die aktuelle ModelNr ! zum skip VIEW
    // UtxTab_add (txTab1, mb->mnam);    // add ModelName to textTable


    // catalog-Model: den zugehoerigen Modelname suchen
    // if(mb->typ == MBTYP_CATALOG) {
      // // CTLG_mnam_modelnam (cbuf1, mb->mnam);
      // // Mod_load_sm (mb->typ, cbuf1);              // load catalog-Model:
      // irc = Mod_load_sm (mb->typ, mb->mnam);           // load catalog-Model:
// 

    AP_box_pm1 = mb->pb1;
    AP_box_pm2 = mb->pb2;

    // set AP_stat.mdl_box_valid according to subModel
    if(UT3D_pt_isFree(&mb->pb1)) AP_mdlbox_invalid_set ();
    else                         AP_mdlbox_invalid_reset ();

    irc = Mod_load_sm (mb->typ, mb->mnam);           // load internal Model
    if(irc < 0) return irc;


    mb->DLsiz = GL_Get_DLind() - mb->DLind;

    // mb->defCol = AP_defcol;     // save default-Color

    // printf(">>>> modNr=%d mnam=|%s| DLind=%d DLsiz=%d\n",il1,
                 // mb->mnam,mb->DLind,mb->DLsiz);

    // Model-Origin speichern;
    // mod1->po = WC_mod_ori;
    mb->po = UT3D_PT_NUL;


      // TESTBLOCK
      // UT3D_stru_dump (Typ_SubModel, mb, "mb:\n");
      // DL_DumpObjTab ();
      // UT3D_stru_dump (Typ_PT, &AP_box_pm1, " pm1:");
      // UT3D_stru_dump (Typ_PT, &AP_box_pm2, " pm2:");
      // END TESTBLOCK


    // set boxpoints for active subModel
    // test if AP_box_pm1 valid
    if(AP_mdlbox_invalid_ck()) {
      UT3D_box_mdl__ (&mb->pb1, &mb->pb2, WC_modact_ind, 0);
      AP_mdlbox_invalid_reset ();
    } else {
      mb->pb1 = AP_box_pm1;
      mb->pb2 = AP_box_pm2;
    }


/* 2016-06-12
    // make it unvisible (else ditto & original obj's visible !)
    // printf(" make invis. %d - %d\n",mb->DLind,mb->DLind+mb->DLsiz-1);
    if(mb->DLsiz > 0) {
      for(il2=0; il2<mb->DLsiz; ++il2) {
        DL_unvis_set (mb->DLind+il2, 1);
      }
    }
*/
    // make it unpickable
    DL_pick_set (mb->DLind, ON);   // set to nopick

  }

  --actSeq;
  if(actSeq >= 0) goto L_nxt_lev;

  // WC_modact_ind = -1;    // wieder im primary Model
  WC_modact_ind = actMod;



  // printf("ex Mod_load_allsm %d\n",DB_get_ModBasNr());
  // DB_dump_ModBas ();
  // // DL_DumpObjTab ();
  // printf("============= ex Mod_load_allsm =======================\n");

  return 0;

}


//================================================================
  int Mod_SM_add_file (char *smNam, char *fNam) {
//================================================================
// Mod_SM_add_file     add subModel from file
// Input:
//   smNam    new subModelname
//   fNam     filename to use; must be native

  char   s1[256];

  // copy <fNam> -> /<tmpDir>/"Model_<subModelname>"
  // rename /<tmpDir>/selection1.gcad /<tmpDir>/"Model_<subModelname>"
  sprintf(s1, "%sModel_%s", OS_get_tmp_dir(), smNam);
    printf(" Mod_SM_add_file |%s|%s|\n",fNam,s1);
  OS_file_copy (fNam, s1);

  // update Mod.lst (find all "/<tmpDir>/"Model_*")
  Mod_mkList (0);

  // update BrowserWindow
  Brw_Mod_add (smNam);


  return 0;

}

 
//================================================================
  int Mod_load_sm (int mTyp, char *mnam) {
//================================================================
/// ein einzelnes subModel laden
/// mnam zB "Submodel1";
/// das subModel darf keine noch nicht existierenden subModels enthalten !!

  int     irc;
  char    fNam[256], cbuf1[256];


  // printf("Mod_load_sm %d |%s|=========================\n",mTyp,mnam);


  if(mnam == NULL) return -1;


  // TSU_mode=Store: Open subModel.
  if(TSU_get_mode() != 0) {       // 0=Normal 1=Store
    irc = TSU_exp_Open (mnam);
    if(irc < 0) return irc;
  }
 

  // catalog-Models:
  if(mTyp == MBTYP_CATALOG) {
      // printf(" catalog-Model |%s|\n",mnam);
    // CTLG_path_catPart (fNam, mnam);
    CTLG_mnam_modelnam (cbuf1, mnam);
    irc = Mod_get_path (fNam, cbuf1);

  } else {
    irc = Mod_get_path (fNam, mnam);
  }
  if(irc < 0) return irc;
    // printf(" fNam=|%s|\n",fNam);


  // clear Hidelist
  GA_hide__ (-1, 0L, 0);

  // clear DL; but do not delete basicModels
  DB_Init (1);

  AP_mdlbox_invalid_set ();

  ED_Init ();

  // abarbeiten eines internen oder externen nativen Model.
  if(ED_work_file (fNam) < 0) goto L_err2;  // define file to work ..

  ED_lnr_reset ();

  // der DYNAMIC_DATA - Block wird hier ganz normal abgearbeitet, da
  // der Input immer aus einer Datei kommt.
  UCS_Reset ();  // sm muss def.Refsys haben
  ED_Run ();


  // save DB                                                2015-11-19
  DB_save__ (mnam);



    // printf("ex Mod_load_sm |%s|========================\n",mnam);

  return 0;


  //----------------------------------------------------------------
  L_err2:
  // TX_Error("Mod_load_sm E001: Submodel %s does not exist",mnam);
  MSG_err_1 ("NOEX_mdl", "%s", mnam);
  printf("Mod_load_sm E001 %s\n",mnam);

  return -1;

}


//====================================================================
  int Mod_get_namAll () {
//====================================================================
/// scan rekursiv die SourceFiles aller dzt existierenden basicModels;
/// load Submodels as (zusaetzliche) basicModels.
/// basicModels =  mdb_dyn-Rec's (DB_StoreModBas)
/// Recurse tru alls Submodels.
// Ganz wichtig: die richtige Ladereihenfolge = seqNr !!!


  int      irc, i1, i2, iLev, oldLev, iNr, par;
  int      il1, ile;
  char     *cbuf, *c1buf, *cp1, txbuf[128];


  // printf("========== Mod_get_namAll ============================ \n");
  // DB_dump_ModBas ();


  iLev = 0;


  //================================================================
  // den naechten Level durchsuchen.
  // ilev ist immer oldLev + 1.
  L_nxt_lev:
  oldLev = iLev;
  ++iLev;
  if(iLev > 12) {
    TX_Error("nesting-level of subModel > 12; Mod_get_namAll-E001");
    return -1;
  }


  iNr = 0;
    // printf(" SBSBSBSBSB scan nxt lev %d\n",iLev);
  ile = DB_get_ModBasNr();  // ile = momentane Anzahl

  for(il1=0; il1<ile; ++il1) {
    Mod_father = DB_get_ModBas (il1);
    if(Mod_father->seqNr != oldLev) continue;
    // parentnode suchen ..
    // par = DB_ModNod_find (il1);
    par = il1;
    // find Models from Submodel; save as Level iLev
    irc = Mod_get_namSub (par, iLev);
    if(irc < 0) return irc;
    ++iNr;
  }

    // printf(" nach get_namAll iLev %d\n",iLev);
    // DB_dump_ModBas ();

  if(iNr > 0) goto L_nxt_lev;



  // printf("---------------- after Mod_get_namAll %d\n",DB_get_ModBasNr());
  // DB_dump_ModBas ();
  // // DL_DumpObjTab ();
  // printf("================ ex Mod_get_namAll =================== \n");
  // // exit(0);


  return 0;

}


//================================================================
  int Mod_get_namSub (int parNr, int iLev) {
//================================================================
/// scan file of Model Mod_father->mnam for Submodels;
/// add Submodels as seqNr=iLev

  int    i1, i2;
  char   fNam[280];     // 2010-11-10
 

  // printf("Mod_get_namSub par=%d typ=%d lev=%d |%s|\n",parNr,
         // Mod_father->typ,iLev,Mod_father->mnam);


  // external Files haben keine Kinder (zumindest nicht die wrl-Files).
  // if(Mod_father->typ >= 0) return 0;
  if(Mod_father->typ > 0) return 0;  // 0=gcad; 2013-09-20
  // if(Mod_father->typ != 1) goto L_typ2;


  if(Mod_father->typ == -2) {     // fix modelname for catalog-Models !!
    // UTX_safeName (Mod_father->mnam, 1);
    return 0;
  }



  // fix FileName
  if(Mod_father->typ == 0) {
    // external subModel .gcad
    // get full filname from symb.name   2013-09-20
    // Mod_sym_get1 (fNam, Mod_father->mnam, 0); // symbol only
    Mod_get_path (fNam, Mod_father->mnam);

  } else {
    // internal submodel = -1, catalogpart = -2
    sprintf(fNam,"%sModel_%s",OS_get_tmp_dir(),Mod_father->mnam);
  }



  // scan File and save all Submodelcalls as level iLev
  return Mod_get_namFil (fNam, parNr, iLev);


  //================================================================
  // L_typ2:
  // TX_Print("Mod_get_namFil E002 %s",mb->mnam);
  // return -1;

}


//================================================================
  int Mod_get_namFil (char *fNam, int parNr, int iLev) {
//================================================================
/// scan File fNam and save all Submodelcalls as level iLev
/// irc = 0 = OK;
///      -1 = Err.

  int    irc;
  FILE   *fpi;

  // printf("Mod_get_namFil par=%d lev=%d |%s|\n",parNr,iLev,fNam);

  irc = 0;

  if((fpi=fopen(fNam, "r")) == NULL) {
    TX_Print("Mod_get_namFil E001 %s",fNam);
    printf("Mod_get_namFil E001 %s\n",fNam);
    return -1;
  }

  while (!feof (fpi)) {
    if (fgets (mem_cbuf1, mem_cbuf1_SIZ, fpi) == NULL) break;
    if(mem_cbuf1[0] != 'M') continue;
    if(!isdigit(mem_cbuf1[1])) continue;
    // save
    irc = Mod_get_namStore (mem_cbuf1, parNr, iLev);
    if(irc < 0) break;
  }

  fclose(fpi);

  // printf(" ex Mod_get_namFil |%s| %d\n",fNam,iLev);

  return irc;

}


//================================================================
  int Mod_get_namStore (char *cp, int parNr, int iLev) {
//================================================================
/// save new BaseModel as mdb_dyn-Record
/// Input: cp = line mit "M20=..."
/// basicModelNr of parent (father) == parNr;
/// SourcefileName ist Mod_father->mnam
//
// Mockup-Models wurden nicht gespeichert;


  int      irc, i1, i2, mTyp;
  long     il, dbi;
  char     txbuf[128], cBuf1[256];
  ModelBas *mb;

  
  // printf("Mod_get_namStore |");UTX_dump_cnl(cp,40);printf("| par=%d lev=%d\n",
          // parNr, iLev);


  if(APED_dbo_oid (&i1, &dbi, cp) < 0) goto L_done;
  if(i1 != Typ_Model) goto L_done;;

  // printf(" nxt Model %d %d |%s| lev=%d",i1,dbi,cp,iLev);
  mTyp = Mod_get_typ1 (txbuf, cp);
  if(mTyp < -2) return 0;  // -3 = kein neuer Modelname
    // printf(" _namStore mTyp=%d |%s|\n",mTyp,txbuf);

  // if(mTyp < 0) goto L_done;
  // skip MockupModel's und Images
  // if(mTyp > 9) goto L_done;

  // fuer catalog !!
  if(mTyp == -2) {

    // existiert das write-file schon ?
      // printf(" ctlg-part; txbuf=|%s|\n",txbuf);
    CTLG_fnWrite_modelnam (cBuf1, txbuf);
    if(OS_checkFilExist(cBuf1, 1) == 0) {  // 0=does not exist
      // create the catalog-part (the .write - file)
      irc = CTLG_Part_Ref1 (txbuf, cp);
    // } else {
      // UTX_safeName (txbuf, 1); // change / into _   raus 2013-09-20
    }
  }


  // neuer Modelname; store
  i1 = DB_StoreModBas (mTyp, txbuf);
  // printf(" |%s| is %d lev=%d\n",txbuf,i1,iLev);


  // zusaetzlich die levelNr speichern
  mb = DB_get_ModBas (i1);
  mb->seqNr = iLev;


  // create Eintrag bei den ModelNodes
  i2 = DB_StoreModNod (i1, parNr, dbi);
  if(i2 < 0) return -1;

// 2010-11-10 TEST
  if(iLev > 0) {
    // den aktuellen Zweig scannen, ob das Model schon vorkommt (Loop !)
    if(DB_ModNod_ckLoop_ (i2) < 0) return -1;
  }
//

  L_done:
  return 0;

}


//================================================================
  int Mod_allmod_MS (double d1, double d2, double d3) {
//================================================================
/// Mod_allmod_MS       in all subMods modify ModSiz

// in den tmp/Model_* steht:
// MODSIZ ...
// ersetzen durch 
// MODSIZ <d1> <d2> <d3>

  char  cbuf1[256], cbuf2[256], newLn[128];
  FILE  *fp1;


  // printf("Mod_allmod_MS %f %f %f\n",d1,d2,d3);

  sprintf(newLn, "MODSIZ %f %f %f",d1,d2,d3);

  // make list of all Submodels ohne "-main-" --> <baseDir>/tmp/Mod.lst
  Mod_mkList (1);

  // try to open inListe
  sprintf(cbuf1,"%sMod.lst",OS_get_tmp_dir());
  if((fp1=fopen(cbuf1,"r")) == NULL) {
    TX_Print("Mod_allmod_MS E002 %s",cbuf1);
    return -1;
  }

  while (!feof (fp1)) {
    if (fgets (cbuf1, 256, fp1) == NULL) break;
    UTX_CleanCR (cbuf1);
    sprintf(cbuf2,"%sModel_%s",OS_get_tmp_dir(),cbuf1);
    // printf(" mod sM=|%s|\n",cbuf2);
    // change MODSIZ Line (immer Ln # 1)
    UTX_fsavLine (newLn, cbuf2, mem_cbuf1_SIZ, 1);
  }


  fclose(fp1);

  return 0;

}


//================================================================
  int Mod_safeName (char *outNam, char *inNam) {
//================================================================
/// give back Filename im <base>tmp/<safeName>

  char   *p1;


  sprintf(outNam,"%sModel_",OS_get_tmp_dir());

  // einen Pointer auf das momentane StringEnde merken
  p1 = &outNam[strlen(outNam)];

  // new filNam hinten anhaengen
  strcpy(p1, inNam);

  // change filNam to safeName (do not mofify directory ..)
  UTX_safeName (p1, 1);  // change '. /'

  printf("ex Mod_safeName  |%s|\n",outNam);

  return 0;

}
 

//================================================================
  int Mod_fget_names_1 (TxtTab *mdlTab) {
//================================================================
 
  int           irc, ii;
  char          s1[256], s2[256];
  char          sbt[3][64], *buttons[4];
  FILE          *fp1;


  // printf("Mod_fget_names_1 \n");
  // UtxTab_dump (mdlTab);


  // create list of existing subModelFiles - <baseDir>/tmp/Mod.lst
  Mod_mkList (1);


  // loop tru fileList; check if subModels are in list mdlNam.
  // if not: ask user for save / delete
  // open
  sprintf(s1,"%sMod.lst",OS_get_tmp_dir());
  if((fp1=fopen(s1,"r")) == NULL) return 0;


  // read Submodels
  // loop tru ../tmp/Mod.lst
  while (!feof (fp1)) {
    if (fgets (s1, 256, fp1) == NULL) break;
    UTX_CleanCR (s1);   // in s1 ist nun Modename
      // printf(" - Mod_fget_names_1 test |%s|\n",s1);

    // check if subModel is in list mdlNam.
    ii = UtxTab_find (s1, mdlTab);
      // printf(" ex UtxTab_find %d\n",ii);
    if(ii >= 0) continue;

    // save or delete subModel <s1>
    // sprintf(s2, "Save unused submodel %s ?",s1);
    MSG_get_1 (s2, 256, "SAVusm", "%s", s1);

    strcpy(sbt[0],  MSG_const__(MSG_cancel));  // "Cancel");
    strcpy(sbt[1],  MSG_const__(MSG_no));      // "NO");
    strcpy(sbt[2],  MSG_const__(MSG_ok));      // "YES");

    buttons[0] = sbt[0];
    buttons[1] = sbt[1];
    buttons[2] = sbt[2];
    buttons[3] = NULL;
    irc = GUI_DialogEntry (s2, NULL, 0, buttons, 5);
      // printf("after GUI_DialogYN\n");

    if(irc == 2) {        // YES = SAVE
      UtxTab_add (mdlTab, s1);

    } else if(irc == 1) { // NO = DELETE file
      sprintf(s2, "%sModel_%s",OS_get_tmp_dir(), s1);
        // printf(" del file |%s|\n",s2);
      OS_file_delete (s2);

    } else {              // CANCEL
      fclose(fp1);
      return -1; 
    }
  }


  L_done:
  fclose(fp1);

    // printf("ex Mod_fget_names_1\n");

  return 0;

}


//========================================================================
  int Mod_fget_names_0 (char *mdlNam,
                        TxtTab *mdlTab, TxtTab *surPtab, TxtTab *surMsh) {
//========================================================================
/// \code
/// Find all subModels in modelfile of model <mdlNam>; add to List 'mdlTab'.
///   Find all PTAB-Surfs; add to List 'surPtab'.
///   Find all MSH-Surfs; add to List 'surMsh'.
/// \endcode

// see Mod_get_namFil Mod_get_namStore

  int    i1;
  char   *p1, *p2, *p3, s1[240], s2[64];
  FILE   *fpi;


  // printf("Mod_fget_names_0 |%s|\n", mdlNam);

  sprintf(s1, "%sModel_%s", OS_get_tmp_dir(), mdlNam);

  if((fpi=fopen(s1, "r")) == NULL) {
    TX_Print("Mod_fget_names_0 E001 %s",s1);
    return -1;
  }

  while (!feof (fpi)) {
    if (fgets (mem_cbuf1, mem_cbuf1_SIZ, fpi) == NULL) break;
    p1 = mem_cbuf1;
    while (*p1  == ' ')  { ++p1; }


    //----------------------------------------------------------------
    if(*p1 != 'M') goto L_surf;
    p2 = p1 + 1;
    if(!isdigit(*p2)) continue;
    p2 = strchr (p2, '=');
    if(p2 == NULL) continue;
    p3 = p2 + 1;
    while (*p3  == ' ')  { ++p3; }
    // extract modelname
    i1 = Mod_get_typ1 (s1, p1);
      // printf("ex Mod_get_typ1 %d |%s|%s|\n",i1,s1,p1);
    if(i1 != MBTYP_INTERN) continue;
    // skip copies (eg M22="M20" P(-13.72 -68.28 0))
    if(s1[0] == 'M') {
      i1 = UTX_ck_num_digNr (&p3, &s1[1]);
        // printf(" i1=%d\n",i1);
      if(s1[i1 + 1] == '\0') continue;           // ck following text ..
    }
    // save
    UtxTab_add_uniq__ (mdlTab, s1);
    continue;


    //----------------------------------------------------------------
    L_surf:
    if(*p1 != 'A') continue;
    p2 = p1 + 1;
    if(!isdigit(*p2)) continue;
    p2 = strchr (p2, '=');
    if(p2 == NULL) continue;
    p3 = p2 + 1;
    while (*p3  == ' ')  { ++p3; }
    if (strncmp (p3, "PTAB", 4)) goto L_MSH;         // A1=PTAB
    // save <mdlnam>_<objnam>
    UTX_cp_word__ (s2, p1);  // objnam
    sprintf(s1, "%s_%s", mdlNam, s2);
    UtxTab_add_uniq__ (surPtab, s1);
    continue;


    //----------------------------------------------------------------
    L_MSH:                      // A2=MSH A1
    if (strncmp (p3, "MSH", 3)) continue;
    // save <mdlnam>_<objnam>
    UTX_cp_word__ (s2, p1);  // objnam
    sprintf(s1, "%s_%s", mdlNam, s2);
    UtxTab_add_uniq__ (surMsh, s1);      // add a new subModel
    // continue;

  }

  fclose(fpi);

  // UtxTab_dump (mdlTab);
  // UtxTab_dump (surPtab);
  // UtxTab_dump (surMsh);
  // printf(" ex Mod_fget_names_0\n");

  return 0;

}


//========================================================================
  int Mod_fget_names__ (TxtTab *mdlTab, TxtTab *surPtab, TxtTab *surMsh) {
//========================================================================
/// \code
/// get lists of all subModels, PTAB-Surfs and MSH-Surfs out of
///   mainmodel and all subModelfiles
/// \endcode

  int    ii;
  char   *p1, mdlNam[80];


  // printf("Mod_fget_names__ \n");

  // int lists
  UtxTab_init__ (mdlTab);            // init (malloc ..)
  UtxTab_init__ (surPtab);           // init (malloc ..)
  UtxTab_init__ (surMsh);            // init (malloc ..)


  //----------------------------------------------------------------
  // Find all subModels im mainfile "Model_"; add to List 'mdlTab'.
  //   Find all PTAB-Surfs; add to List 'surPtab'.
  //   Find all MSH-Surfs; add to List 'surMsh'.
  mdlNam[0] = '\0';    // main = ""
  Mod_fget_names_0 (mdlNam, mdlTab, surPtab, surMsh);



  //----------------------------------------------------------------
  // Loop tru all subModelFiles (List 'mdlTab');
  //   Find all subModels; add to List 'mdlTab'.
  //   Find all PTAB-Surfs; add to List 'surPtab'.
  //   Find all MSH-Surfs; add to List 'surMsh'.
  ii = -1;
  L_nxt:
    ++ii;
    if(ii >= mdlTab->iNr) goto L_exit;
    p1 = UtxTab__ (ii, mdlTab);      // get word from index
      // printf(" mdl[%d]=|%s|\n",ii,p1);
    Mod_fget_names_0 (p1, mdlTab, surPtab, surMsh);

    goto L_nxt;




  //----------------------------------------------------------------
  L_exit:

  // check for unused subModels
  return Mod_fget_names_1 (mdlTab);

}


//=========== EOF ====================================================
