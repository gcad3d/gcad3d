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

CTLG_fnWrite_modelnam    create .write-Filename from partName
CTLG_mnam_modelnam       find symDir/Modelname from partName
CTLG_mnam_PartID           returns internal Modelname
CTLG_PartID_mnam         returns PartID from internal Modelname
CTLG_Part_Ref1           create catalogreference
CTLG_getCatDir           return symbol "CATALOG" (directory for .ctlg-files)
CTLG_Sel_wPartLst        create new childList in  file tmp/catParts.lst
CTLG_Lst_write           create tmp/Catalog.lst

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
Funktionsweise:
----------------------------------------------------------------------
Format Makro:
ctlgNam;gcadPrgNam;parameters..

gcadPrg:
  enthält eine Zeile:
  CALL CTLG
  damit wird das <ctlgNam>.write ausgeführt !


es wird file /tmp/<ctlgNam>.write erzeugt;
  enthält alle v#=value -commands (aus dem ctlg-Makro)




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

#include "../ut/func_types.h"              // UI_FuncSet
#include "../xa/xa_mem.h"             // memspc011
#include "../ut/ut_os.h"              // OS_get_bas_dir
#include "../db/ut_DB.h"              // DB_get_ModRef






#ifdef globTag
 void CTLG(){}
#endif



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
  // DB_dump_ModRef (); // gibts keine ..

  sprintf(fnam, "%s",OS_get_tmp_dir());
  p1 = UTX_pos_EOS (fnam);

  // sprintf(p1, "%s.write",modelnam);
  strcat(p1, modelnam);
  UTX_safeName (p1, 1); // change '. /\\'
  strcat(p1, ".write");

    // printf("ex CTLG_fnWrite_modelnam |%s|\n",fnam);

  return 0;

}

 
///================================================================
  int CTLG_mnam_modelnam (char *mnam, char *modelnam) {
///================================================================
/// catalog-Model: den zugehoerigen Modelname suchen
///   (aus dem write-File, zeile 1 lesen)
/// Input:
///   modelnam; zB |Schrauben_SKS_6x30|
/// Output:
///   mnam         |Schrauben/SKS.gcad|  (symDir/Modelname)

// das 

  int    irc;
  char   *p1;


  // printf("CTLG_mnam_modelnam |%s|\n",modelnam);
  // DB_dump_ModRef (); // gibts keine ..

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

    // printf("ex CTLG_mnam_modelnam |%s|%s|\n",mnam,modelnam);

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

  printf("ex CTLG_PartID_mnam |%s|%s|\n",PartID, mNam);


  return -1;

}


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

 
///================================================================
  int CTLG_Part_Ref1 (char *mNam, char *catPart) {
///================================================================
/// Create reference of catalogPart.
/// basemodel als internes Model umkopieren -
/// Parameter des part als File xx.write erzeugen
/// returns internal Modelname.
/// 
/// Input:
///   catPart   the declaration without CTLG | "Schrauben/SKS_10x45" P(0 0 0)|
/// Output:
///   mNam = name of internal Model; size=128
/// Retcod:
///   -2 = success ( = type: catalogModel)
///   -3 = Error

// catPart decodieren in catalogName,PartName.
// den BasmodelName suchen
// die Parameter des part suchen (get PartLine in CatalogFile)
// alle parameter aus dem Catalog in eine Datei exportieren
// (ein tmp/CTLG.write schreiben mit allen Parametern (am ';' getrennt)
// see also Function "WRITE"
// das basemodel als internes Model umkopieren

// Beispiel partLine im catalog Schrau.ctlg:
// SKS_6x10;Schrau/SKS";V1=40;V2=40;V3=600;

  int       irc, i1;
  long      dbi;
  char      cbuf1[256], cbuf2[256], ctlgNam[64], partNam[64],
            *modDir, *modNam, *sPar, *p1, *p2;
  FILE      *fp1;
  ModelRef  *mr;
  ModelBas  *mb;


  // printf("CTLG_Part_Ref1 |%s|\n",catPart);


  // catalogName von catPart abtrennen; maxLen = 62 chars
  catPart = strchr(catPart, '"');  // find starting "
  if(catPart == NULL) {irc = 0; goto L_Err__;}
  ++catPart;
  p1 = strchr(catPart, '/');
  if(p1 == NULL) {
    // es kann |M21=CTLG "M20" ...| sein; in diesem Fall nix tun (es muss schon
    // eine Kopie vorhanden sein ..)
    irc = APED_dbo_oid (&i1, &dbi, catPart);
    if(irc != 0) {irc = 12; goto L_Err__;}
    mr = DB_get_ModRef (dbi);
    mb = DB_get_ModBas (mr->modNr);
    if(mb == NULL) {irc = 1; goto L_Err__;}
      // printf(" mNr=%d mnam=|%s|\n",mr->modNr,mb->mnam);
    strcpy(mNam, mb->mnam);
    return -2;   // OK
    
  }
  i1 = p1 - catPart;
  if(i1 > 62) {irc = 2; goto L_Err__;}
  strncpy(ctlgNam, catPart, i1);
  ctlgNam[i1] = '\0';
    // printf(" ctlgNam=|%s|\n",ctlgNam);

  // fix partName
  ++p1;
  p2 = strchr(p1, '"');
  if(p2 == NULL) {irc = -1; goto L_Err__;}
  i1 = p2 - p1;
  if(i1 > 62) {irc = -2; goto L_Err__;}
  strncpy(partNam, p1, i1);
  partNam[i1] = '\0';
    // printf(" partName=|%s|\n",partNam);



  //----------------------------------------------------------------
  // get path for catalogFile
  // irc = Mod_sym_get1 (memspc011, "CATALOG/", 0);
  irc = CTLG_getCatDir (memspc011);
  if(irc < 0) {irc = 3; goto L_Err__;}

  // fix catalogFileName
  strcat(memspc011, ctlgNam);
  strcat(memspc011, ".ctlg");
    // printf(" catalogFileName=|%s|\n",memspc011);


  // open catalogFileName
  if((fp1=fopen(memspc011, "r")) == NULL) {irc = 4; goto L_Err__;}


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
  *sPar = '\0';
  ++sPar;
    // printf(" baseMod=|%s|\n",modDir);
    // printf(" sPar=|%s|\n",sPar);



  //----------------------------------------------------------------
  // das basemodel modDir als internes Model umkopieren

  // get Symdir for baseModel
  irc = Mod_sym_get1 (cbuf1, modDir, 0);
  if(irc < 0) {irc = 7; goto L_Err__;}
    // printf(" symDir=|%s|\n",cbuf1);

  // decode symDir-modFilnam of baseMod
  modNam = strchr(modDir, '/');
  if(modNam == NULL) {irc = 8; goto L_Err__;}
  i1 = modNam - modDir;
  if(i1 > 62) {irc = 9; goto L_Err__;}
  *modNam = '\0';
  ++modNam;
    // printf(" modDir=|%s|\n",modDir);
    // printf(" modNam=|%s|\n",modNam);
  // make modNam > safeName
  UTX_safeName (modNam, 1); // change '. /\\'

  // create filename for baseModel
  i1 = strlen(cbuf1);
  sprintf(&cbuf1[i1], "%s.gcad",modNam);
    // printf(" fnbm=|%s|\n",cbuf1);

  // check if Basemodel exists
  irc = OS_checkFilExist (cbuf1, 2);
  if(irc == 0) {irc = 10; goto L_Err__;}

/*
  // create filename for internalModel
  // sprintf(cbuf2,"%stmp%cModel_%s",OS_get_bas_dir(),fnam_del,mNam);
    // printf(" fnim=|%s|\n",cbuf2);

  // create filename for internalModel
  sprintf(cbuf2,"%stmp%cModel_%s_%s",OS_get_bas_dir(),fnam_del,ctlgNam,modNam);
    printf(" fnim=|%s|\n",cbuf2);

  // copy baseModel > internalModel
  OS_file_copy (cbuf1, cbuf2);
*/


  //----------------------------------------------------------------
  // parameter sPar in Datei tmp/CTLG.write schreiben (am ';' trennen)

  // create filename for parameterfile - tmp/CTLG.write
  // sprintf(cbuf2,"%stmp%c%s.write",OS_get_bas_dir(),fnam_del,mNam);
  sprintf(cbuf2,"%s%s_%s.write",OS_get_tmp_dir(),ctlgNam,partNam);
    // printf(" fnpar=|%s|\n",cbuf2);

  // open parameterfile
  if((fp1=fopen(cbuf2, "w")) == NULL) {irc = 11; goto L_Err__;}

  // write Modelname into first Line (used later in CTLG_mnam_modelnam)
  // fprintf(fp1, "# %s_%s.gcad\n",ctlgNam,modNam);
  fprintf(fp1, "# %s/%s.gcad\n",modDir,modNam);


  // write parameters
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
  // sprintf(mNam,"%s_%s",ctlgNam,partNam);
  sprintf(mNam,"%s/%s",ctlgNam,partNam);              // 2011-12-18
  // sprintf(mNam,"%s_%s",ctlgNam,modNam);
    // printf("ex CTLG_Part_Ref1 |%s|\n",mNam);


  return -2;


  L_Err__:
    sprintf(memspc011, "CTLG_Part_Ref1 E%d", irc);
    TX_Error(memspc011);
    return -3;

}
 

///================================================================
  int CTLG_getCatDir (char *catDir) {
///================================================================
/// \code
/// return symbold "CATALOG" (directory for .ctlg-files)
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


///================================================================
  int CTLG_Sel_wPartLst (char *partFilNam, char *actCatNam) {
///================================================================
/// \code
/// callback parent-sel (catalog selected)(); create childs-file (parts)
/// create new childList <tmp>/catParts.lst
/// save active catalog-name in file tmp/catParts.act
/// for catalog actCatNam
/// \endcode

  char   cfn[256], *p1;
  FILE   *fpi, *fpo;


  printf("CTLG_Sel_wPartLst |%s|%s|\n",partFilNam,actCatNam);


  if(CTLG_getCatDir (cfn) < 0) return -1;
  strcat(cfn, actCatNam);
    // printf(" CATALOG=|%s|\n",cfn);



  // open partFile
  if((fpo=fopen(partFilNam,"w")) == NULL) {
    TX_Print("CTLG_Sel_wPartLst E001 %s",partFilNam);
    return -1;
  }


  // open catalog
  if((fpi=fopen(cfn,"r")) == NULL) {
    TX_Print("CTLG_Sel_wPartLst E002 %s",cfn);
    fclose(fpo);
    return -1;
  }

  while (!feof (fpi)) {
    if (fgets (memspc011, 1020, fpi) == NULL) {
      fclose(fpi);
      break;
    }
    if(memspc011[0] == '#') continue;  // skip comment
    p1 = strchr (memspc011, ';');
    if(p1 == NULL) continue;
    *p1 = '\0';
    fprintf(fpo, "%s\n",memspc011);
  }

  fclose(fpo);

  // UTX_fsavLine (parent, cbuf, 1);
  sprintf(cfn,"%scatParts.act",OS_get_tmp_dir());
  fpo = fopen (cfn, "w");
  fprintf(fpo, "%s\n", actCatNam);
  fclose(fpo);


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
 


//==================== EOF =======================================
