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
CTLG_fnWrite_modelnam    get filename for parameters to modify
CTLG_mnam_modelnam       find symDir/Modelname from partName
CTLG_PartID_mnam         returns PartID from internal Modelname
CTLG_Part_Ref1           create file tmp/<catalog>_<part>.write (modif. params)
CTLG_dat_del             delete all parameter-files
CTLG_getCatDir           return symbol "CATALOG" (directory for .ctlg-files)
CTLG_Lst_write           create tmp/Catalog.lst
CTLG_catParts__          create list of parts in file CatParts.lst

List_functions_end:
=====================================================
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


- using catalogparts  (Example: "M20=CTLG "Schrauben/SKS_6x30" P(100 0 0)")
  the format is:
  M# = CTLG "<catalogfilename>/<partName>" <position_orientation>

CTLG_Part_Ref1 
  Create file tmp/<catalog>_<part>.write (with parameters different to basModel)
  extracted parameters from catalog-file, overwrites values in baseModel.



----------------------------------------------------------------------
Wichtige Funktionen:

WC_Work__
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
#include "MS_Def0.h"
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../ut/ut_geo.h"             // ModelRef
#include "../ut/ut_txt.h"             // fnam_del
#include "../ut/ut_cast.h"             // INT_PTR

#include "../ut/func_types.h"              // UI_FuncSet
#include "../ut/ut_os.h"              // OS_get_bas_dir
#include "../db/ut_DB.h"              // DB_get_ModRef

#include "../ut/ut_memTab.h"           // MemTab
#include "../xa/xa_mem.h"             // memspc011
#include "../xa/xa_msg.h"             // MSG_ERR__





#ifdef globTag
 void CTLG(){}
#endif



static const char* CATLG_DATFIL_TYP = "ctlg_dat";


//================================================================
  int CTLG_get__ () {
//================================================================
// CTLG_Sel__               get catalog-part-selection from user
// CTLG_Sel_CB gets selected part

  int   i1;
  long l1;
  char  cfn[256], s1[256], ctlg[128];

  printf(" CTLG_get__ -------------------\n");


   // check if file tmp/Catalog.lst exists; no: create it.
  sprintf(cfn,"%sCatalog.lst",OS_get_tmp_dir());
  if(!OS_checkFilExist(cfn, 1)) {
    CTLG_Lst_write ();         // create <tmp>/Catalog.lst
    CTLG_catParts__ (NULL);    // create empty CatParts.lst
  }


  //================================================================
  // display list of active parts + "=== change catalog ==="
  L_start:
  // disp file CatParts.lst, let user select 
  sprintf(cfn,"%sCatParts.lst",OS_get_tmp_dir());
  i1 = GUI_listf1__ (s1, sizeof(s1), cfn,
                     "\"- select part or change catalog -\"", "\"x40,y40,a\"");
  if(i1 < 0) return -1;  // cancel or error

    printf(" ctlg-part-select |%s|\n",s1);

  if(strcmp(s1, "=== change catalog ===")) goto L_sel_part;


  //================================================================
  // catalog selected - change
  sprintf(cfn,"%sCatalog.lst",OS_get_tmp_dir());
  i1 = GUI_listf1__ (s1, sizeof(s1), cfn,
                     "\"- select catalogpart -\"", "\"x40,y40,a\"");
  if(i1 < 0) return -1;  // cancel or error
    printf(" ctlg-select |%s|\n",s1);

  // create list of parts of catalog <s1> in file CatParts.lst
  CTLG_catParts__ (s1);

  // display
  goto L_start;


  goto L_exit;


  //================================================================
  // part selected - report.
  L_sel_part:
    printf(" ctlg-part-select-part |%s|\n",s1);

  // get active catalogName
  sprintf(cfn,"%sCatalog.act",OS_get_tmp_dir());
  l1 = OS_FilSiz(cfn);
  UTX_str_file (ctlg, &l1, cfn);

  // remove filetype from catalogName ".ctlg"
  UTX_del_right (ctlg, 5);
    printf(" ctlg-part-select-part part,ctlg |%s|%s|\n",s1,ctlg);

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
    sprintf(cfn,"%sCatalog.act",OS_get_tmp_dir());
      printf(" cfn=|%s|\n",cfn);
    UTX_wrf_str (cfn, ctlgNam);
  }


  //----------------------------------------------------------------
  // write parts-file CatParts.lst
  // get oufilename
  sprintf(pfn,"%sCatParts.lst",OS_get_tmp_dir());
    printf(" pfn=|%s|\n",pfn);


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
  MDLFN_dirAbs_symDir (cfn, "CATALOG");    // get symbolic-directory CATALOG
  strcat(cfn, ctlgNam);
    printf(" cfn=|%s|\n",cfn);


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
      printf(" out |%s|\n",sPart);
  }

  L_done:
  fclose(fpo);

  return 0;

}


///================================================================
  int CTLG_fnWrite_modelnam (char *fnam, char *modelnam) {
///================================================================
/// catalog-Model: den zugehoerigen filename fuer das .write-File machen
///   (aus dem write-File, zeile 1 lesen)
/// Input:
///   modelnam; zB |Schrauben_SKS_6x30|  oder |Schrauben/SKS_6x30|
/// Output:
///   fnam         |/mnt/serv1/Devel/dev/gCAD3D/tmp/Schrauben_SKS_6x30.write|

// das

  char   *p1;


  // printf("CTLG_fnWrite_modelnam |%s|\n",modelnam);


  sprintf(fnam, "%s",OS_get_tmp_dir());
  p1 = UTX_pos_EOS (fnam);

  strcat(p1, modelnam);
  UTX_safeName (p1, 1); // change '. /\\' into '_'

  strcat(p1, ".");
  strcat(p1, CATLG_DATFIL_TYP);

    // printf("ex CTLG_fnWrite_modelnam |%s|%s|\n",fnam,modelnam);

  return 0;

}

 
///================================================================
  int CTLG_mnam_modelnam (char *mnam, char *modelnam) {
///================================================================
/// \code
/// catalog-Model: get directory/basicModelname from <catalog>_<part>
///   (read 1. line of file tmp/<catalog>_<part>.write)
/// Input:
///   modelnam; zB |Schrauben_SKS_6x30|
/// Output:
///   mnam         |Schrauben/SKS.gcad|  (symDir/Modelname)
/// \endcode

// das 

  int    irc;
  char   *p1;


  // printf("CTLG_mnam_modelnam |%s|\n",modelnam);

  // den zugehoerigen filename fuer das .write-File machen
  CTLG_fnWrite_modelnam (mnam, modelnam);

    // printf(" fn=|%s|\n",mnam);
  irc = UTX_fgetLine (memspc011, 256, mnam, 1);
    // printf(" memspc011=|%s|\n",memspc011);

  if(irc < 0) {
    TX_Error("Catalog-Model %s not yet loaded",modelnam);
    return -1;
  }

  strcpy(mnam, &memspc011[2]);

    // printf("ex-CTLG_mnam_modelnam |%s|%s|\n",mnam,modelnam);

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

  strcpy(PartID, mNam);

  // search for first "_"; replace with direcoryDelimiter
  p1 = strchr(PartID, '_');
  if(p1 == NULL) return -2;

  *p1 = '/';

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
  int CTLG_Part_Ref1 (char *mNam, char *catPart) {
//================================================================
// Prepare creation of catalogPart.
// Create file tmp/<catalog>_<part>.ctlg_dat (with parameters)
// return internal Modelname.
// 
// Input:
//   catPart   the declaration without CTLG | "Schrauben/SKS_10x45" ...|
//             first part is filename of catalogfile; second part ist the partName
// Output:
//   mNam = name of internal Model; size=128
// Retcod:
//    0 = success ( = type: catalogModel)
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
  char      cbuf1[256], cbuf2[256], ctlgNam[64], partNam[64], cfn[256], cpmn[256],
            *modDir, *modNam, *sPar, *p1, *p2;
  FILE      *fp1;
  ModelRef  *mr;
  ModelBas  *mb;


  // printf("CTLG_Part_Ref1 |%s|\n",catPart);

  // skip starting '"'
  catPart = strchr(catPart, '"');
  if(catPart == NULL) {irc = 0; goto L_Err__;}
  ++catPart;
  UTX_pos_skipLeadBlk (catPart);  // skip blanks 


  // test |M21=CTLG "M20" ...| if yes: basicModel already exists ..
  if(catPart[0] == 'M') {
    if(UTX_ck_num_i(&catPart[1]) > 0) {
      // irc = APED_dbo_oid (&i1, &dbi, catPart);
      // if(!irc) {irc = 12; goto L_Err__;}
      dbi = atol(&catPart[1]);
      mr = DB_get_ModRef (dbi);
      mb = DB_get_ModBas (mr->modNr);
      if(mb == NULL) {irc = 1; goto L_Err__;}
        // printf(" mNr=%d mnam=|%s|\n",mr->modNr,mb->mnam);
      strcpy(mNam, mb->mnam);
      goto L_OK;   // OK
    }
  }


  // get catalogName ctlgNam (first part of catPart);
  p1 = strchr(catPart, '/');
  i1 = p1 - catPart;
  if(i1 > 62) {irc = 2; goto L_Err__;}
  strncpy(ctlgNam, catPart, i1);
  ctlgNam[i1] = '\0';
    // printf(" ctlgNam=|%s|\n",ctlgNam);


  // get partName - following the '/'
  ++p1;
  p2 = strchr(p1, '"');
  if(p2 == NULL) {irc = -1; goto L_Err__;}
  i1 = p2 - p1;
  if(i1 > 62) {irc = -2; goto L_Err__;}
  strncpy(partNam, p1, i1);
  partNam[i1] = '\0';
    // printf(" partName=|%s|\n",partNam);


  // get cfn = catalogfilename
  sprintf(cbuf1, "CATALOG/%s.ctlg",ctlgNam);
    // printf(" cbuf1=|%s|\n",cbuf1);
  // resolv symDir CATALOG
  irc = MDLFN_dirAbs_symDir (cfn, cbuf1);
  if(irc < 0) {irc = -4; goto L_Err__;}
    // printf(" cfn=|%s|\n",cfn);


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
    partNam, ctlgNam);
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
  if(i1 > 128) {irc = 7; goto L_Err__;}
  strncpy(cbuf1, modDir, i1);
  cbuf1[i1] = '\0';
    // printf(" cbuf1=|%s|\n",cbuf1);

  // get catalogPartModelName cpmn 
  irc = MDLFN_dirAbs_symDir (cpmn, cbuf1);
  if(irc < 0) {irc = -8; goto L_Err__;}
  strcat(cpmn, ".gcad");
    // printf(" cpmn=|%s|\n",cpmn);


  //----------------------------------------------------------------
  // das basemodel modDir als internes Model umkopieren

  // check if Basemodel exists
  irc = OS_checkFilExist (cpmn, 2);
  if(irc == 0) {irc = 10; goto L_Err__;}

/*
  // create filename for internalModel
  sprintf(cbuf2,"%stmp%cModel_%s_%s",OS_get_bas_dir(),fnam_del,ctlgNam,modNam);
    printf(" fnim=|%s|\n",cbuf2);

  // copy baseModel > internalModel
  OS_file_copy (cbuf1, cbuf2);
*/


  //----------------------------------------------------------------
  // parameter sPar in Datei tmp/CTLG.write schreiben (am ';' trennen)

  // create filename for parameterfile - tmp/<catalog>_<part>.ctlg_dat
  // sprintf(cbuf2,"%stmp%c%s.write",OS_get_bas_dir(),fnam_del,mNam);
  sprintf(cbuf2,"%s%s_%s.%s",OS_get_tmp_dir(),ctlgNam,partNam,CATLG_DATFIL_TYP);
    // printf(" fnpar=|%s|\n",cbuf2);

  // open parameterfile
  if((fp1=fopen(cbuf2, "w")) == NULL) {irc = 11; goto L_Err__;}

  // write Modelname into first Line (used later in CTLG_mnam_modelnam)
  // fprintf(fp1, "# %s_%s.gcad\n",ctlgNam,modNam);
  // fprintf(fp1, "# %s/%s.gcad\n",modDir,modNam);
  fprintf(fp1, "# %s\n",cpmn);


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
  // fix the internal Modelname
  sprintf(mNam,"%s/%s",ctlgNam,partNam);              // 2011-12-18


  L_OK:
    // printf("ex CTLG_Part_Ref1 irc=0 mNam=|%s|\n",mNam);
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
/// \code
/// return symbol "CATALOG" (directory for .ctlg-files)
/// Output:
///   catDir         size >= 256
/// \endcode

  int irc;

  // symDir CATALOG holen ..
  irc = Mod_sym_get1 (catDir, "CATALOG/", 1);
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
  sprintf(cbuf, "%s*.%s",OS_get_tmp_dir(),CATLG_DATFIL_TYP);
  OS_file_delGrp (cbuf);

  return 0;

}


///================================================================
  int CTLG_Lst_write () {
///================================================================
/// create tmp/Catalog.lst

  char  fnam[256], dirNam[256];

  printf("CTLG_Lst_write \n");


  // outfilename
  sprintf(fnam,"%sCatalog.lst",OS_get_tmp_dir());
    // printf(" list=|%s|\n",fnam);


  if(CTLG_getCatDir (dirNam) < 0) return -1;
  

  // list dir/*.typ
  if(UTX_dir_listf (fnam, dirNam, NULL, ".ctlg") < 0) {
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
  MDLFN_dirAbs_symDir (cfn, "CATALOG");    // get symbolic-directory CATALOG
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
