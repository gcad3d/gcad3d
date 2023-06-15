//              CATALOG-UtilityProgs                2007-04-09   RF
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
\file  ../xa/catalog.c
\brief  CATALOG-UtilityProgs
\code
=====================================================
List_functions_start:

CTLG_get__               get catalog-part-selection from user
CTLG_fnMdl_safPartNam       find symDir/Modelname from partName
CTLG_PartID_mnam         returns PartID from internal Modelname
CTLG_Part_Ref1           create file tmp/<catalog>_<part>.write (modif. params)
CTLG_dat_del             delete all parameter-files
CTLG_getCatDir           return symbol "CATALOG" (directory for .ctlg-files)
CTLG_Lst_write           create tmp/Catalog.lst
CTLG_catParts__          create list of parts in file CatParts.lst

List_functions_end:
=====================================================
OBSOLETE:
// CTLG_fnWrite_modelnam    get filename for parameters to modify
// CTLG_mnam_PartID         returns internal Modelname
// CTLG_pNam__ctlg_part     get part-name (second word of part-record in ctlgfile)

\endcode *//*----------------------------------------


====================================================================
tmp/CatParts.lst   = name of active catalog
tmp/catParts.lst   = childList for active catalog

Die catalogModels werden wie externe Models behandelt;
  es gibt also kein internes subModel;

RefMod:
  enthaelt die Nr des basMod; die Position (po,vx,vz);

BasMod:
  Typ = MBTYP_CATALOG; Modelname is "<catalogName>/<partName>.ctlg"
  Das zugehoerige Modelfile wird durch die Funktion CTLG_fnMdl_safPartNam
  geholt (in der 1. zeile des parameter-File steht der ModelfileName).



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
Files:
<tmp>/Catalog.lst     list of all catalogFiles 
<tmp>/Catalog.act     active catalogFileName
<tmp>/CatParts.lst    list of parts of the active catalogFileName


----------------------------------------------------------------------
Funktionsweise:          see also ../../doc/html/Catalog_en.htm
----------------------------------------------------------------------
- baseModel    (Example: Schrau/SKS.gcad)
  is a gcad-model;
  must have this line: "CALL CTLG"
    (at runtime now the parametfile is excuted)

- catalogFile  (Example: ctlg/Schrauben.ctlg)
  one line for each part; the format is:
  partName;baseModel;parameter[;parameter ..];
  catalog-files are in symbolic directory "CATALOG"


- using catalogparts  (Example: "M20="Schrauben/SKS_6x30.ctlg" P(100 0 0)")
  the format is:
  M# ="<catalogfilename>/<partName>.ctlg" <position_orientation>

CTLG_Part_Ref1 
  Create file tmp/<catalog>_<part>.write (with parameters different to basModel)
  extracted parameters from catalog-file, overwrites values in baseModel.



----------------------------------------------------------------------
Wichtige Funktionen:

WC_Work1
  CTLG_Part_Ref1
  Mod_load_allsm
    Mod_load_sm
      Mod_get_namSub

APT_decode_model
  CTLG_mnam_PartID

APT_work_PrgCodTab
  APT_work_TPC_CALL "CALL CTLG"
    CTLG_fnWrite_modelnam
    APT_UP_up
    return (PrgMod_skip_until_file)   // 6


*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../ut/ut_geo.h"             // ModelRef
#include "../ut/ut_txt.h"             // fnam_del
#include "../ut/ut_cast.h"             // INT__PTR

#include "../ut/func_types.h"              // UI_FuncSet
#include "../ut/ut_os.h"              // AP_get_bas_dir
#include "../db/ut_DB.h"              // DB_get_ModRef

#include "../ut/ut_memTab.h"           // MemTab
#include "../xa/xa_mem.h"             // memspc011
#include "../xa/xa_msg.h"             // MSG_ERR__
#include "../xa/mdl__.h"                   // SIZMFNam




#ifdef globTag
 void CTLG(){}
#endif



static const char* CATLG_DATFIL_TYP = "ctlg_dat";


//================================================================
  int CTLG_get__ () {
//================================================================
// CTLG_Sel__               get catalog-part-selection from user
// CTLG_Sel_CB gets selected part
//
// Files in <tmpDir>:
// Catalog.act   // filename of active-catalog-file eg test_ctlg1.ctlg
// Catalog.lst
// CatParts.lst  // "change catalog" and list of parts of active-catalog-file



  int   i1;
  long  l1;
  char  cfn[256], cpn[256], s1[256], ctlg[256];


  // printf(" CTLG_get__ -------------------\n");


  // check if file tmp/Catalog.lst exists; no: create it.
  sprintf(cfn,"%sCatalog.lst",AP_get_tmp_dir());
  if(!OS_checkFilExist(cfn, 1)) {
    CTLG_Lst_write ();         // create <tmp>/Catalog.lst
    CTLG_catParts__ (NULL);    // create empty CatParts.lst
  }


  //================================================================
  // display list of active parts + "=== change catalog ==="
  L_start:

  // check if file CatParts.lst, exists; if not - goto select catalog
  sprintf(cpn,"%sCatParts.lst",AP_get_tmp_dir());
  if(!OS_checkFilExist(cpn, 1)) goto L_sel_ctlg;

  // let user select catPart or other catalog
  i1 = GUI_listf1__ (s1, sizeof(s1), cpn,
                     "- select part or change catalog -", "x40,y40,a");
  if(i1 < 0) return -1;  // cancel or error
    // printf(" ctlg-part-select |%s|\n",s1);

  if(strcmp(s1, "=== change catalog ===")) goto L_sel_part;


  //================================================================
  // catalog selected - change
  L_sel_ctlg:
  sprintf(cfn,"%sCatalog.lst",AP_get_tmp_dir());
  i1 = GUI_listf1__ (s1, sizeof(s1), cfn,
                     "- select catalogpart -", "x40,y40,a");
  if(i1 < 0) return -1;  // cancel or error
    // printf(" ctlg-select |%s|\n",s1);

  // create list of parts of catalog <s1> in file CatParts.lst
  CTLG_catParts__ (s1);

  // display
  goto L_start;


  goto L_exit;


  //================================================================
  // part selected - report.
  L_sel_part:
    // printf(" ctlg-part-select-part |%s|\n",s1);

  // get active catalogName
  sprintf(cfn,"%sCatalog.act",AP_get_tmp_dir());
  l1 = OS_FilSiz(cfn);
  UTX_str_file (ctlg, &l1, cfn);

  // remove filetype from catalogName ".ctlg"
  UTX_del_right (ctlg, 5);
    // printf(" ctlg-part-select-part part,ctlg |%s|%s|\n",s1,ctlg);

  // report to CAD: partName, catalogName,
  IE_cad_selM2_CB (s1, ctlg);


  //================================================================
  L_exit:
  return 0;

}


/*
//================================================================
  int CTLG_path_catPart (char *fNam, char *catPart) {
//================================================================
// In:   "catalog/part"
// Out:  der komplette Filename (Path + Filename + Filetyp).
// RetCod:  0 = OK; -1 = Symbol_not_found


  CTLG_mnam_PartID (fNam, catPart);

  return 0;

}
*/


//================================================================
  int CTLG_catParts__ (char *ctlgNam) {
//================================================================
// CTLG_catParts__             create list of parts in file CatParts.lst
//   and write ctlgNam into file Catalog.act
// Input:
//   ctlgNam      catalogname  (CATALOG/<ctlgNam>.ctlg)
//                NULL: init file



  char  cfn[256], pfn[256], sPart[1024], *p1;
  FILE  *fpi, *fpo;


  //----------------------------------------------------------------
  // write file Catalog.act
  if(ctlgNam) {
    sprintf(cfn,"%sCatalog.act",AP_get_tmp_dir());
      printf(" cfn=|%s|\n",cfn);
    UTX_wrf_str (cfn, ctlgNam);
  }


  //----------------------------------------------------------------
  // write parts-file CatParts.lst
  // get oufilename
  sprintf(pfn,"%sCatParts.lst",AP_get_tmp_dir());
    // printf(" _catParts__-pfn=|%s|\n",pfn);


  if(!ctlgNam) {
    // test if file exists; yes: return
    if(OS_checkFilExist(pfn,1)) return 0;
  }


  // open partFile pfn for write
  if((fpo=fopen(pfn,"w")) == NULL) {
    TX_Print("CTLG_catParts__ E001 %s",pfn);
    return -1;
  }

  fprintf(fpo, "=== change catalog ===\n");

  if(!ctlgNam) goto L_done;

  // get infilename cfn
  MDLFN_fDir_syFn (cfn, "CATALOG");    // get symbolic-directory CATALOG
  strcat(cfn, ctlgNam);
    // printf(" _catParts__-cfn=|%s|\n",cfn);


  // open catalogfile cfn for read
  if((fpi=fopen(cfn,"r")) == NULL) {
    TX_Print("CTLG_catParts__ E002 %s",cfn);
    fclose(fpo);
    return -1;
  }

  // create new childList pfn
  while (!feof (fpi)) {
    if (fgets (sPart, 1020, fpi) == NULL) {
      fclose(fpi);
      break;
    }
    if(sPart[0] == '#') continue;  // skip comment
    // get first word
    p1 = strchr (sPart, ';');
    if(p1 == NULL) continue;
    *p1 = '\0';
    fprintf(fpo, "%s\n",sPart);
      // printf(" out |%s|\n",sPart);
  }

  L_done:
  fclose(fpo);

  return 0;

}


/* UU
//================================================================
  int CTLG_fnWrite_modelnam (char *fnam, char *modelnam) {
//================================================================
// CTLG_fnWrite_modelnam            get full filename of catalog-datafile
// Input:
//   modelnam     symbolic modelname eg "Profile/4x4_60.ctlg"
// Output:
//   fnam         full filename of catalog-datafile;
//                  eg "<tmp>/Profile_4x4_60.ctlg_dat"


  char   safNam[SIZMFTot];


  printf("CTLG_fnWrite_modelnam |%s|\n",modelnam);


  strcpy (safNam, modelnam);
  UTX_safeName (safNam, 3);   // keep .

  sprintf(fnam, "%s%s_dat",AP_get_tmp_dir(),safNam);

    // printf("ex-CTLG_fnWrite_modelnam |%s|%s|\n",fnam,modelnam);

  return 0;

}
*/
 
//================================================================
  int CTLG_fnMdl_safPartNam (char *mdlNam, char *safPartNam) {
//================================================================
// CTLG_fnMdl_safPartNam          get full filename of catalog-modelfile
// catalog-Model: get directory/basicModelname from <catalog>_<part>
//   (read 1. line of catalog-datafile)
// Input:
//   safPartNam  safe, eg "Profile_4x4_60_ctlg"
// Output:
//   mdlNam      full filename of the modelfile - tmp/Model_Profile_4x4_60_ctlg


  // create filename tmp/<catPart>.dat - eg "Profile_6x4_150_ctlg.dat"
  sprintf(mdlNam, "%sModel_%s",AP_get_tmp_dir(),safPartNam);

    // printf("ex-CTLG_fnMdl_safPartNam |%s|\n",mdlNam);

  return 0;

}

//================================================================
  int CTLG_PartID_mnam (char *PartID, char *mNam) {
//================================================================
/// returns PartID from internal Modelname
///  
/// Input:
///   mNam      eg "Profile_4x4_80"
/// Output:
///   PartID    eg "Profile/4x4_80"
/// Retcod:
///   -1 = success ( = type: internal models)
///   -2 = Error



  char *p1;

  sprintf(PartID, "%s.ctlg", mNam);


//   strcpy(PartID, mNam);
//   // search for first "_"; replace with direcoryDelimiter
//   p1 = strchr(PartID, '_');
//   if(p1 == NULL) return -2;
//   *p1 = '/';

  // printf("ex-CTLG_PartID_mnam |%s|%s|\n",PartID, mNam);


  return -1;

}


/*  UU
//================================================================
  int CTLG_mnam_PartID (char *mNam, char *catPart) {
//================================================================
/// returns internal Modelname from PartID  (PartID = catalogName+PartName)
///  
/// Input:
///   catPart   the decalaration Line
/// Output:
///   mNam = name of internal Model; size=128
/// Retcod:
///   -1 = success ( = type: internal models)
///   -2 = Error


  int  irc, i1;
  char ctlgNam[64], *partNam;


  // printf("CTLG_mnam_PartID |%s|\n",catPart);


  // catalogName von catPart abtrennen; maxLen = 62 chars
  partNam = strchr(catPart, '/');
  if(partNam == NULL) {irc = 1; goto L_Err__;}
  i1 = partNam - catPart;
  if(i1 > 62) {irc = 2; goto L_Err__;}
  strncpy(ctlgNam, catPart, i1);
  ctlgNam[i1] = '\0';
    // printf(" ctlgNam=|%s|\n",ctlgNam);

  // fix partName
  ++partNam;
    // printf(" partName=|%s|\n",partNam);



  //----------------------------------------------------------------
  // return internal Modelname
  sprintf(mNam,"%s_%s",ctlgNam,partNam);
    // printf("ex CTLG_mnam_PartID |%s|\n",mNam);

  return -1;


  L_Err__:
    sprintf(memspc011, "CTLG_mnam_PartID E%d", irc);
    TX_Error(memspc011);
    return -2;

}
*/

//================================================================
  int CTLG_Part_Ref1 (char *fnMdl, char *fnDat, stru_FN *ofn) {
//================================================================
// CTLG_Part_Ref1                  Prepare creation of catalogPart.
//   Create file fnDat (tmp/<catalog>_<part>.ctlg_dat (with parameters))
//   return filename of catalog-model
// 
// Input:
//   fnDat     filename of part-data;      (tmp/<catPart>.ctlg_dat)
//   ofn       filename-obj (from MDLFN_oFn_fNam); symDir=partName,
//             fDir=full-directory-catalog, fNam=catalog
// Output:
//   fnMdl     filename of catalog-model
// Retcod:
//    0 = OK
//   -1 = Error: cannot find part in catalog
//   -2 = Error: error open catalogFile
//   -3 = Error

// catPart decodieren in catalogName,PartName.
// den BasmodelName suchen
// die Parameter des part suchen (get PartLine in CatalogFile)
// alle parameter aus dem Catalog in eine Datei exportieren
// (ein tmp/<mnam>.write schreiben mit allen Parametern (am ';' getrennt)
// see also Function "WRITE"

// Beispiel partLine im catalog Schrau.ctlg:
// SKS_6x10;Schrau/SKS";V1=40;V2=40;V3=600;

  int       irc, i1;
  long      dbi;
  char      cbuf1[SIZMFTot], ctlgNam[64], *partNam, cfn[SIZFNam],
            *modDir, *modNam, *sPar, *p1, *p2;
  FILE      *fp1;
  ModelRef  *mr;
  ModelBas  *mb;


  // printf("CTLG_Part_Ref1 |%s|\n",fnDat);
  // MDLFN_dump_ofn (ofn, "_Part_Ref1");


  //----------------------------------------------------------------
  // get partName
  partNam = ofn->fNam;

  // get cfn = catalogfilename
  sprintf(cfn, "%s%s.ctlg",ofn->fDir,ofn->symDir);
    // printf(" _Part_Ref1-cfn=|%s|\n",cfn);


  //================================================================
 
  // open catalogFileName
  if((fp1=fopen(cfn, "r")) == NULL) {
    MSG_ERR__ (ERR_file_open, "'%s'", cfn);
    return -2;
  }

  // search partName-Line in catalogFile
  while (!feof (fp1)) {
    if (fgets (memspc011, 1020, fp1) == NULL) {
      fclose(fp1);
      break;
    }
    if(memspc011[0] == '#') continue;  // skip comment
    UTX_CleanCR (memspc011); // Delete Blanks, CR's u. LF's am Ende.
      // printf(" nxt line=|%s|\n",memspc011);
    modDir = strchr (memspc011, ';');
    if(modDir == NULL) {fclose (fp1); irc = 5; goto L_Err__;}
    *modDir = '\0';
      // printf(" nxt part=|%s\n",memspc011);
    if(!strcmp(memspc011, partNam)) goto L_found;
  }

  TX_Error("CTLG_Part_Ref - cannot find part %s in catalog %s",
    partNam, ofn->fNam);
  return -1;

  L_found:
  fclose(fp1);
  ++modDir;
    // printf(" partLine=|%s|\n",modDir);


  // die Parameter des part suchen
  sPar = strchr(modDir, ';');
  if(sPar == NULL) {irc = 6; goto L_Err__;}
  ++sPar;
    // printf(" baseMod=|%s|\n",modDir);
    // printf(" sPar=|%s|\n",sPar);


  i1 = sPar - modDir - 1;
  if(i1 > SIZMFNam) {irc = 7; goto L_Err__;}
  strncpy(cbuf1, modDir, i1);
  cbuf1[i1] = '\0';
    // printf(" cbuf1=|%s|\n",cbuf1);

  // get catalogPartModelName fnMdl 
  irc = MDLFN_fDir_syFn (fnMdl, cbuf1);
  if(irc < 0) {irc = -8; goto L_Err__;}
  strcat(fnMdl, ".gcad");
    // printf(" Part_Ref1-fnMdl=|%s|\n",fnMdl);


  //----------------------------------------------------------------
  // check if Basemodel exists
  irc = OS_checkFilExist (fnMdl, 2);
  if(irc == 0) {
    TX_Print("***** catalog-model %s not found",fnMdl);
    irc = 10;
    goto L_Err__;
  }

  //----------------------------------------------------------------
  // create file parameter-data

//   // create filename for parameterfile - tmp/<catalog>_<part>.ctlg_dat
//   sprintf(fnDat,"%s%s_%s.%s",AP_get_tmp_dir(),ctlgNam,partNam,CATLG_DATFIL_TYP);
//     printf(" fnDat=|%s|\n",fnDat);

  // open parameterfile
  if((fp1=fopen(fnDat, "w")) == NULL) {irc = 11; goto L_Err__;}

  // write Modelname into first Line (used later in CTLG_fnMdl_safPartNam)
  fprintf(fp1, "# %s\n",fnMdl);

  // write parameters into file
  p2 = UTX_pos_EOS(sPar);    // find end of string

  L_wr_nxt:
  p1 = strchr(sPar, ';');
  if(p1) *p1 = '\0';
  else p1 = UTX_pos_EOS(sPar);
    // printf(" nxtLn=|%s|\n",sPar);
  fprintf(fp1, "%s\n",sPar);        // write nxt parameter

  sPar = p1 + 1;
  if(sPar < p2) goto L_wr_nxt;

  fprintf(fp1, "# EOF\n");
  fclose(fp1);


  //----------------------------------------------------------------
  L_OK:
    // printf("ex-CTLG_Part_Ref1 |%s|%s|\n",fnMdl,fnDat);
  return 0;


  //----------------------------------------------------------------
  L_Err__:
    sprintf(memspc011, "CTLG_Part_Ref1 E%d", irc);
    TX_Error(memspc011);
    return -3;

}
 

///================================================================
  int CTLG_getCatDir (char *catDir) {
///================================================================
// CTLG_getCatDir        get symbol "CATALOG" (directory for .ctlg-files)
// Output:
//   catDir         size >= 256

  int irc;

  // symDir CATALOG holen ..
//   irc = Mod_sym_get1 (catDir, "CATALOG/", 1);
  irc = MDLFN_fDir_syFn (catDir, "CATALOG");
  if(irc < 0) {
    GUI_MsgBox (
    "symbolic Directory CATALOG does not exist. Use Standards/Directory.");
    return -1;
  }
    // printf(" CATALOG=|%s|\n",catDir);

  return 0;

}


//================================================================
  int CTLG_dat_del () {
//================================================================
/// CTLG_dat_del             delete all parameter-files

  char  cbuf[256];


  // catalog-parameterfiles
  sprintf(cbuf, "%s*.%s",AP_get_tmp_dir(),CATLG_DATFIL_TYP);
  OS_file_delGrp (cbuf);

  return 0;

}


///================================================================
  int CTLG_Lst_write () {
///================================================================
/// create tmp/Catalog.lst

  char  fnam[256], dirNam[256];

  // printf("CTLG_Lst_write \n");


  // outfilename
  sprintf(fnam,"%sCatalog.lst",AP_get_tmp_dir());
    // printf(" list=|%s|\n",fnam);


  if(CTLG_getCatDir (dirNam) < 0) return -1;
  

  // list dir/*.typ
  if(UTX_dir_listf (fnam, dirNam, NULL, ".ctlg", 0) < 0) {
    TX_Error("CTLG_Lst_write E001");
    return 0;
  }


  return 0;

}
 

/* UU
//================================================================
  int CTLG_pNam__ctlg_part (char *pfn, char *ctlg, char *part) {
//================================================================
// CTLG_pNam__ctlg_part   get part-name (second word of part-record in catalogfile)

  int    irc;
  char   sln[1024], cfn[256], *p1, *p2;
  FILE   *fpi;


  printf("CTLG_pNam__ctlg_part |%s|%s|\n",ctlg,part);

  // get infilename cfn
  MDLFN_fDir_syFn (cfn, "CATALOG");    // get symbolic-directory CATALOG
  strcat(cfn, ctlg);
    // printf(" cfn=|%s|\n",cfn);

  // open catalogfile cfn for read
  if((fpi=fopen(cfn,"r")) == NULL) {
    TX_Print("CTLG_pNam__ctlg_part E001 %s",cfn);
    return -1;
  }

  // loop tru file <ctlg>, find part <part>, get second word -> pfn
  while (!feof (fpi)) {
    if (fgets (sln, 1020, fpi) == NULL) {
      fclose(fpi);
      goto L_err;
    }
    if(sln[0] == '#') continue;  // skip comment
    // get first word
    p1 = strchr (sln, ';');
    if(p1 == NULL) goto L_err;
    *p1 = '\0';
    if(!strcmp(sln, part)) {
      ++p1;
        // printf(" found1 |%s|\n",p1);
      p2 = strchr (p1, ';');
      if(p2 == NULL) goto L_err;
      *p2 = '\0';
        // printf(" found2 |%s|\n",p1);
      strcpy(pfn, p1);
      break;
    }
  }

  L_done:
  fclose(fpi);

    printf("ex-CTLG_pNam__ctlg_part |%s|\n",pfn);

  return 0;


  L_err:
    TX_Print("CTLG_pNam__ctlg_part E002 %s %s",ctlg,part);
    return -1;

}
*/

//==================== EOF =======================================
