//     ../xa/mdlfn.c         MoDeL-FileName processing functions
/*
 *
 * Copyright (C) 2020 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
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
=====================================================
TODO:
-----------------------------------------------------
- add "expand shell variables ("$" -> OS_filnam_eval)"
  // resolve shell variables
    // test if dir has "$"
    strcpy (s2, p1);
      // printf(" cb_dirsym2-s1 |%s|\n",s1);
    p2 = strchr (s2, '$');
    if(p2) {
      // expand shell variables
      irc = OS_filnam_eval (s2, s2, sizeof(s2));
    }

-----------------------------------------------------
- replace Mod_sym_* functions with MDLFN_* functions 

=====================================================
Modifications:
2020-04-27 New from Mod_sym_* functions. RF.


*/
#ifdef globTag
 void MDLFN(){}
#endif
/*!
\file  ../xa/mdlfn.c
\brief ModeL-FileName processing functions                     see INF_symDir
\code
=====================================================
List_functions_start:

MDLFN_safMain          get complete safeName for mainModel

MDLFN_oFn_user         get fileName-obj from user (file-select)
MDLFN_oFn_fNam         get fileName-obj from (unknown) filename
MDLFN_ofn_set          set fileName-obj from strings

MDLFN_syFn_f_name      get filename of file cfg/dir.lst (list of symbolic dir's)
MDLFN_syFn_oFn         get symbolic-filename from filenameObject
MDLFN_syFn_safNam      get symbolic-filename from safNam
MDLFN_syFn_AP          get symbolic-filename of active mainModel
MDLFN_syFn_f_add       add symbolic path to File cfg/dir.lst
MDLFN_syFn_f_del       delete symbolic path in file cfg/dir.lst
MDLFN_syFn_f_dump      dump all symbolic directories

MDLFN_fDir_syFn        get absolute-direcory from symbolic-filename
MDLFN_fDir_syDir       get absolute-direcory from symbolic-directory

MDLFN_syDir_user_mNam  get symbolic directory from user from filname
MDLFN_syDir_user_lst   get symbolic directory from user (from list)
MDLFN_symDir__fnAbs    get symbolID and symbol-path for filename-absolute

MDLFN_ffNam_fNam       get full-filename from (unknown) filename
MDLFN_ffNam_oFn        get full-filename from filenameObject
MDLFN_ffNam_AP         get full-filename of active mainModel
MDLFN_ffNam_syFn       get full-filename from symbolic-filename

MDLFN_get__            get a copy of AP_mod_*
MDLFN_set__            set AP_mod_* (eg restore)

MDLFN_dump__           dump AP_mod_*
MDLFN_dump_ofn         dump struct stru_FN
MDLFN_test1
MDLFN_test2            test MDLFN_ffNam_fNam

List_functions_end:
=====================================================
UU:
Mod_fNam_set           get symbolic-filename or absolute-filename from global-vars
MDLFN_fNam_resolv      get sym,dir,fnam,ftyp from symbolic|rel|abs.filename

See also functions Mod_sym_*


=========================================================
struct stru_FN  has separate spaces for symbolic-directory, filename, filetyp ..

 ffnam  full-filename (/path/filename.filetyp)
 syFn   symbolic-filename, eg "Data/Niet1.gcad";  "Data" = syDir
 syDir  symbolic-directory; list is ../../gCAD3D/cfg_Linux/dir.lst
 oFn    filename-obj (stru_MDLFN)


Extrapath:
 eg. Data/symEl1/res1.gcad
   .symDir AP_mod_sym   Data/symEl1            symbolic-directory + extrapath
   .fDir   AP_mod_dir   ~/gCAD3D/dat/symEl1/   full-directory + extrapath
   .fNam   AP_mod_fnam  res1                   filename
   .fTyp   AP_mod_ftyp  gcad                   filetyp



Sizes of filenamefields: see ../xa/mdl__.h
complete filename            320
symbolic|safe-modelnames     SIZMFTot   256


=========================================================
*/


#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
// #include <ctype.h>              // islower ..
// #include <stdarg.h>             // va_list


#include "../ut/ut_geo.h"              // ModelBas
#include "../ut/ut_txt.h"                 // fnam_del
#include "../ut/ut_os.h"                  // AP_get_bas_dir ..
#include "../xa/mdl__.h"                  // SIZMFSym stru_FN



// EXTERNALS:
// ../xa/xa.c
extern char AP_mod_fnam[SIZMFNam];
extern char AP_mod_dir[SIZMFNam];
extern char AP_mod_ftyp[SIZMFTyp]; 
extern char AP_mod_sym[SIZMFSym];
extern char AP_mod_iftyp;
extern char AP_symDir_fnam[128];  // filename active SymbolDirFile





//================================================================
  int MDLFN_ffNam_AP (char *ffNam) {
//================================================================
// MDLFN_ffNam_AP       get full-filename of active mainModel
// Input:
//   AP_mod_sym,AP_mod_dir,p1,AP_mod_fnam,AP_mod_ftyp
// Output:
//   ffNam      full-filename
//
// see also MDLFN_ffNam_oFn

  char     *p1;


  // printf("MDLFN_ffNam_AP\n");
  // printf("  AP_mod_sym  |%s|\n",AP_mod_sym);
  // printf("  AP_mod_dir  |%s|\n",AP_mod_dir);
  // printf("  AP_mod_fnam |%s|\n",AP_mod_fnam);
  // printf("  AP_mod_ftyp |%s|\n",AP_mod_ftyp);


  sprintf(ffNam, "%s%s.%s",AP_mod_dir,AP_mod_fnam,AP_mod_ftyp);


/*
  // check for extraPath in symDir
  p1 = strchr (AP_mod_sym, '/');
  if(p1) {
    // fDir + extraPath + fNam + fTyp
    ++p1;
    sprintf(ffNam, "%s%s/%s.%s", AP_mod_dir,p1,AP_mod_fnam,AP_mod_ftyp);

  } else {
    // no extraPath
    sprintf(ffNam, "%s%s.%s",AP_mod_dir,AP_mod_fnam,AP_mod_ftyp);
  }
*/

    // printf(" ex-MDLFN_ffNam_AP |%s|\n",ffNam);

  return 0;

}

//================================================================
  int MDLFN_syFn_AP (char *sfNam) {
//================================================================
// MDLFN_syFn_AP          get symbolic-filename of active mainModel


  if(!AP_mod_sym[0]) {
    TX_Print("***** no symbolic direcory defined - using default");
    strcpy(AP_mod_sym, "Data");
  }

  sprintf(sfNam, "%s/%s.%s",AP_mod_sym,AP_mod_fnam,AP_mod_ftyp);


  return 0;

}


//================================================================
  int MDLFN_ffNam_fNam (char *ffNam, char *fn) {
//================================================================
// MDLFN_ffNam_fNam       get full-filename from (unknown) filename
// no '/' and no '.' - returns <tmpDir>/Model_<fn> (internal modelname)
// was Mod_get_path
// see also UTX_ffnam_fNam

  stru_FN   ofn;

  // printf("MDLFN_ffNam_fNam |%s|\n",fn);

  // get fileName-obj from (unknown) filename
  MDLFN_oFn_fNam (&ofn, fn);
    // MDLFN_dump_ofn (&ofn, "ffNam_fNam");

  // get full-filename from filenameObject
  MDLFN_ffNam_oFn (ffNam, &ofn);

    // printf(" ex-MDLFN_ffNam_fNam |%s|\n",ffNam);

  return 0;

}


//================================================================
  int MDLFN_ffNam_oFn (char *ffNam, stru_FN *ofn) {
//================================================================
// MDLFN_ffNam_oFn       get full-filename from filenameObject
// no '/' and no '.' - returns <tmpDir>/Model_<fn> (internal modelname)


  char     *p1;

  // MDLFN_dump_ofn(ofn, "MDLFN_ffNam_oFn");


  if(ofn->fTyp[0]) {
    sprintf(ffNam, "%s%s.%s", ofn->fDir,ofn->fNam,ofn->fTyp);
  } else {
    sprintf(ffNam, "%s%s", ofn->fDir,ofn->fNam);
  }

    // printf(" ex-MDLFN_ffNam_oFn |%s|\n",ffNam);

  return 0;

}


//================================================================
  int MDLFN_ffNam_syFn (char *ffNam, char *syFn) {
//================================================================
// MDLFN_ffNam_syFn       get full-filename from symbolic-filename


  int    irc;
  char   s1[320], path[256], *fNam, *p1;

  // printf("MDLFN_ffNam_syFn-in |%s|\n",syFn);

  // cut symDir / fileNam
  strcpy(s1, syFn);
#ifdef _MSC_VER
  fNam = UTX_find_strrchrn(s1, "/\\");
#else
  fNam = strrchr(s1, fnam_del);
#endif
  if(!fNam) {
      // printf("*** internal modelname\n");
    sprintf(ffNam, "%s%s",AP_get_tmp_dir(),syFn);
    goto L_exit;
  }

  *fNam = '\0'; // cut
  ++fNam;

  // get path from symbolic-filename
  irc = MDLFN_fDir_syFn (path, s1);
  if(irc < 0) goto L_exit;

  // full-filename = path + filename
  sprintf(ffNam, "%s%s", path, fNam);


  L_exit:
    // printf(" ex-MDLFN_ffNam_syFn %d |%s|\n",irc,ffNam);
  return irc;

}


//================================================================
  int MDLFN_syFn_oFn (char *symNam, stru_FN *ofn) {
//================================================================
// MDLFN_syFn_oFn        get symbolic-filename from filenameObject
// - symbolic-filename is "symDir[/extraPath/]fileNam.fileTyp"


  char     *pxp, *px1;

  // MDLFN_dump_ofn (ofn, "MDLFN_syFn_oFn");


  if(ofn->symDir[0]) {
    // external with symbolic-dir
    sprintf(symNam, "%s%c%s.%s", ofn->symDir,fnam_del,ofn->fNam,ofn->fTyp);
    goto L_exit;
  }


  //----------------------------------------------------------------
  if(ofn->fDir[0]) {
    // no symDir, external
    sprintf(symNam, "%s%s.%s", ofn->fDir,ofn->fNam,ofn->fTyp);
    goto L_exit;
  } 


  //----------------------------------------------------------------
  // internal
  sprintf(symNam, "%s.%s", ofn->fNam,ofn->fTyp);


  //----------------------------------------------------------------
  L_exit:
    // printf(" ex-MDLFN_syFn_oFn |%s|\n",symNam);
  return 0;

}


//================================================================
  int MDLFN_safMain (char *safMain) {
//================================================================
// MDLFN_safMain              get complete safeName for mainModel

  char    s1[320];


  sprintf(s1, "%s_%s_%s",AP_mod_sym,AP_mod_fnam,AP_mod_ftyp);

  if(strlen(s1) >= 128)
    {TX_Print("***** MDLFN_safMain E1 %s",AP_mod_fnam); return -1;}

  strcpy(safMain, s1);

    // printf(" ex-MDLFN_safMain |%s|\n",safMain);

  return 0;

}


//================================================================
  int MDLFN_syFn_safNam (char *syFn, char *safNam) {
//================================================================
// MDLFN_syFn_safNam       get symbolic-filename from safNam
// 

  int      irc, ii, mdbNr;
  char     dbNam[128];
  ModelBas *mdba;

  // loop tru all DB-baseModels; make safNam, compare ..
  DB_get_mdb (&mdbNr, &mdba);
  for(ii=0; ii<mdbNr; ++ii) {
      // printf(" syFn_safNam- %d |%s|\n",ii,mdba[ii].mnam);
    strcpy(dbNam, mdba[ii].mnam);
    UTX_safeName (dbNam, 1);  // change '. /'
    if(!strcmp(dbNam, safNam)) {
      strcpy(syFn, mdba[ii].mnam);
      irc = 0;
      goto L_exit;
    }
  }
  TX_Print("***** MDLFN_syFn_safNam ERR %s",safNam);
  irc = -1;


  L_exit:
    // printf(" ex-MDLFN_syFn_safNam  %d |%s|%s|\n",irc,syFn,safNam);

  return irc;

}


//========================================================================
  int MDLFN_ofn_set (stru_FN *mfn, char *symDir, char *fNam, char *fTyp) {
//========================================================================
// MDLFN_ofn_set          set fileName-obj from strings
// - get mfn->fDir from symbolic-directory symDir


  // printf("MDLFN_ofn_set |%s|%s|%s|\n",symDir,fNam,fTyp);


  strcpy(mfn->symDir, symDir);
  MDLFN_fDir_syDir (mfn->fDir, symDir);
  strcpy(mfn->fNam, fNam);
  strcpy(mfn->fTyp, fTyp);
  mfn->iTyp = AP_iftyp_ftyp (fTyp);

    // MDLFN_dump_ofn (mfn, "ex-MDLFN_ofn_set");
 
  return 0;

}


//================================================================
  int MDLFN_syDir_user_mNam (stru_FN *mfn) {
//================================================================
// MDLFN_syDir_user_mNam         get symbolic-directory from user
// Input:
//   ffnam     full filename of cad-model
// Output:
//   mfn       decoded filename
//   retCode   0=OK;  -1=cancel

  int    irc;
  char   s1[320], s2[400];


  // MDLFN_dump_ofn (mfn, "MDLFN_syDir_user_mNam");


  // TX_Print ("***** get symbolic name for %s\n",mfn->fDir);
  // GUI_update__ (); // display messageabove


  //----------------------------------------------------------------
  // no symbolic-pathname exist.
  L_start:
  // ask user: 
  //  - give symbolic-name for new symDir or -
  //  - copy model into existing modelDirectory
  strcpy(s2, "\" - give symbolic-name for new symDir or copy model into existing modelDirectory \"");

  irc = AP_GUI__ (s1, 120, "GUI_dlg1", "dlgbe",
                  s2,
                  "key-symDir",
                  "copy-file",
                  "cancel",
                  NULL);
  if(irc < 0) return -1;           // error
  if(s1[0] == '0') goto L_symDir;  // button "key-symDir"
  if(s1[0] == '1') goto L_copy;    // button "copy-file"
  return -1;  // cancel-button '2' or cancel-X


  //----------------------------------------------------------------
  // get symbolic-name for new symDir from user
  L_symDir:

  // strcpy(s2, "\" key name for directory %s \"",mfn->fDir);
  sprintf(s2, "\" key name for directory %s \"",mfn->fDir);

  irc = AP_GUI__ (s1, 120, "GUI_dlg1", "dlgbe",
                  s2,
                  "Cancel",
                  "OK",
                  "--ent \"my_model_dir1\" 16",
                   NULL);
  if(irc < 0) return -1;           // error
  if(s1[0] == '0') goto L_start;   // button "Cancel"
  strcpy(mfn->symDir, &s1[1]);

  // add symbol into symbol-file
  MDLFN_syFn_f_add (mfn->symDir, mfn->fDir);

  goto L_exit;


  //----------------------------------------------------------------
  // copy modelFile into existing modelDirectory;
  L_copy:

    // get full-filename from filenameObject
    irc = MDLFN_ffNam_oFn (s1, mfn);

    // set symDir from user
    irc = MDLFN_syDir_user_lst (mfn->symDir);
    if(irc < 1) goto L_start;

    // get directory for symDir
    irc = MDLFN_fDir_syFn (mfn->fDir, mfn->symDir);
    if(irc < 0) goto L_start;
      // printf(" UI_file_mNam-L2 |%s|%s|\n",mfn->symDir, mfn->fDir);

    // get outfilename
    sprintf(s2, "%s%s.%s", mfn->fDir, mfn->fNam, mfn->fTyp);
      // printf(" UI_file_mNam-L3 |%s|\n",s2);

    // copy file
// TODO: MDLFN_ffnam_oFn (s1, mfn);
    irc = OS_file_copy (s1, s2);   // copy (from to)
    if(irc) {
      TX_Print("***** ERROR cannot copy %s to %s ",s1,s2);
      goto L_start;
    }

    TX_Print(" - modelfile copied to %s",s2);


  //----------------------------------------------------------------
  L_exit:

    // MDLFN_dump_ofn (mfn, "ex-MDLFN_syDir_user_mNam");

  return irc;

}


//================================================================
  int MDLFN_syDir_user_lst (char *sSym) {
//================================================================
//  MDLFN_syDir_user_lst                 get symbolic directory from user
// Output
//   sSym     size >= 128
//   retCod   0=OK, -1=cancel, error
//
// see GUI_file_symdir__


  int     irc;
  char    sLst[256];


  MDLFN_syFn_f_name (sLst);     // get filename of list of symbolic directories

  return GUI_listf1__ (sSym, 120,
                       sLst,
                       "\"select symbolic directory \"",
                       "\"x40,y30\"");
  return 0;

}


//====================================================================
  int MDLFN_fDir_syFn (char *absDir, char *symPath) {
//====================================================================
// MDLFN_fDir_syFn    get absolute-direcory from symbolic-directory
// symdir must be terminated with '/' or '\0'
// - does add extraPath to absDir
// Input:
//   symPath   eg: "Data"  or "Data/tmp/" - without filename !
// Output:
//   absDir:   full path (from file xa/dir.lst)     Size 128.
//   RetCod
//     >= 0    OK; path in out_path; Linenumber of symbol in path.
//     -1      Symbol <symDir> not found in file
//     -2      path too long
//
// see MDLFN_fDir_syDir

  int     irc=0, i1, i2;
  char    fn[SIZFNam], symDir[SIZMFSym], *p1, *p2;


  // printf("MDLFN_fDir_syFn |%s|\n",symPath);


  // get symDir out of symPath (ending with first '/')
  p1 = strchr (symPath, '/');
  if(p1) {
    i1 = p1 - symPath;
    if(i1 > 126) goto L_err2;
    strncpy(symDir, symPath, i1);
    symDir[i1] = '\0';
    ++p1;  // skip '/'
    i2 = strlen(p1);
  } else {
    if(strlen(symPath) > 126) goto L_err2;
    strcpy(symDir, symPath);
    i2 = 0;
  }
    // printf(" symDir=|%s| i2=%d\n",symDir,i2);


// TODO: use _MDLFN_fDir_syDir
  // get filename of dir.lst
  MDLFN_syFn_f_name (fn);
    // printf(" _dirAbs_symDir-fn=|%s|\n",fn);

  // get absDir from symDir out of file
  irc = UTX_setup_get__ (absDir, symDir, fn);
  if(irc) goto L_err1;
    // printf(" _dirAbs_symDir-absDir1=|%s|\n",absDir);

  // test if absDir has "$"
  p2 = strchr (absDir, '$');
  if(p2) {
    // expand shell variables
    irc = OS_filnam_eval (absDir, absDir, 128);
  }
    // printf(" _dirAbs_symDir-absDir2=|%s|\n",absDir);


  // add rest of path (following p2)
  if(i2) {
    i1 = strlen(absDir) + i2;
    if(i1 > 128) goto L_err2;
    strcat(absDir, p1);
  }


  L_exit:
      // printf("ex-MDLFN_fDir_syFn %d |%s|%s|\n",irc,absDir,symPath);
    return irc;

  L_err1:
    TX_Print("***** symbolic dir. %s not found ..",symDir);
    goto L_exit;

  L_err2:
    TX_Print("***** symbolic dir. %s too long ..",symPath);
    irc = -2;
    goto L_exit;

}

//====================================================================
  int MDLFN_fDir_syDir (char *absDir, char *symPath) {
//====================================================================
// MDLFN_fDir_syDir    get absolute-direcory from symbolic-directory
// symdir must be terminated with '/' or '\0'
// Input:
//   symPath   eg: "Data"  or "Data/tmp/" - without filename !
// Output:
//   absDir:   full path (from file xa/dir.lst)     Size must be >= SIZMFTot
//   RetCod
//     >= 0    OK; path in out_path; Linenumber of symbol in path.
//     -1      Symbol <symDir> not found in file
//     -2      path too long
//
// was MDLFN_fDir_syFn

  int     irc=0, i1;
  char    fn[SIZFNam], symDir[SIZMFSym], *p1, *p2;


  // printf("MDLFN_fDir_syDir |%s|\n",symPath);


  // get symDir out of symPath (ending with first '/')
#ifdef _MSC_VER
  p1 = strpbrk(symPath, "/\\");
#else
  p1 = strchr(symPath, '/');
#endif

  if(p1) {
    // cut off extraPath !
    i1 = p1 - symPath;
    if(i1 > SIZMFSym) goto L_err2;
    strncpy(symDir, symPath, i1);
    symDir[i1] = '\0';

  } else {
    if(strlen(symPath) > SIZMFSym) goto L_err2;
    strcpy(symDir, symPath);
  }
    // printf(" symDir=|%s|\n",symDir);


  // get filename of dir.lst
  MDLFN_syFn_f_name (fn);
    // printf(" _dirAbs_symDir-fn=|%s|\n",fn);

  // get absDir from symDir out of file
  irc = UTX_setup_get__ (absDir, symDir, fn);
  if(irc) goto L_err1;
    // printf(" _dirAbs_symDir-absDir1=|%s|\n",absDir);

  // test if absDir has "$"
  p2 = strchr (absDir, '$');
  if(p2) {
    // expand shell variables
    irc = OS_filnam_eval (absDir, absDir, SIZMFTot);
  }

  L_exit:
      // printf("ex-MDLFN_fDir_syDir %d |%s|%s|\n",irc,absDir,symPath);
    return irc;

  L_err1:
    TX_Print("***** symbolic dir. %s not found ..",symDir);
    goto L_exit;

  L_err2:
    TX_Print("***** symbolic dir. %s too long ..",symPath);
    irc = -2;
    goto L_exit;

}


/* UU  replaced by MDLFN_oFn_fNam
//=============================================================================
  int MDLFN_fNam_resolv (char *symDir, char *fDir, char *fNam, char *fTyp,
                            char *fnIn, char *basDir) {
//=============================================================================
// Mod_fNam_get        get sym,dir,fnam,ftyp from symbolic- or abs.filename
// If first directorypart is a symbolic directory, following subDirectories 
//   will be part of fnam.
// Example: Data/GIS1/bk1.gcad  resolves to:
//          symDir="Data"; fDir="/root/cadfiles/"; fNam="GIS1/bk1"; fTyp="gcad"
//
// Input:
//   fnIn
//   basDir     active directory; get it eg with AP_get_bas_dir()
// Output:
//   symDir     the symbolic-pathname; maxSiz 128
//   fDir       if symDir exists: the path of the symbolic-pathname; maxSiz 128
//              else the path without filename
//   fNam       the filename (no filetype); maxSiz 128
//              if symDir exists: fNam can start with subDirectories
//   fTyp       filetype, maxSiz 40
//   RetCod:    -1=no-symDir, dir=path; -2=internal-error 
//              >0=length of symbolic-path
//
// TODO: expand opsys-vars (eg $HOME) - see Mod_sym_get2 OS_filnam_eval


  int    irc;
  int    sdl, snl;
  char   *pfn, *pft, *pf1, fnTmp1[256], fnTmp2[256];


  // printf("_________________________________________________ \n");
  printf("MDLFN_fNam_resolv |%s|\n",fnIn);
  // printf(" basDir=|%s|\n",basDir);


  symDir[0] ='\0';
  fDir[0] ='\0';
  irc = 0;

  // skip leading blanks
  UTX_pos_skipLeadBlk (fnIn);

  // remove following blanks ..
  irc = UTX_del_follBl (fnIn);


  //---------------------------------------------------------------
  // test for absolute filepath
  // unix: starting with '/'; MS: second char ':'
  if(!OS_ck_DirAbs(fnIn)) {pf1 = fnIn; goto L_abs;}      // MS-Win-compat !


  // test for starting '.' - relative filepath
  if(fnIn[0] == '.') goto L_rel;


  // test for symbolic filepath
  // pfn = find first filename-delimiter
  // must check for '/' AND '\' (in MS '/' can come from out of source)
#ifdef _MSC_VER
  pfn = strpbrk(fnIn, "/\\");
#else
  pfn = strchr(fnIn, fnam_del);           // unix: '/'
#endif
  if(!pfn) goto L_rel;    // no symDir, no directory - relativ.

  // copy first word of filename -> symDir
  // copy symDir
  sdl =  pfn - fnIn;
  if(sdl >= 128) return -2;      // string too long
  strncpy(symDir, fnIn, sdl);
  symDir[sdl] = '\0';
    // printf(" _fNam_resolv-sym |%s|\n",symDir);


  // get fDir = path of symDir out of file
  irc = MDLFN_fDir_syFn (fDir, symDir);
  if(irc < 0) {symDir[0] ='\0'; goto L_rel;}


  ++pfn;  // skip deli
  goto L_fnam;



  //================================================================
  L_rel:
    // get absolute from relative
    irc = UTX_fnam_fnrel (fnTmp1, 256, fnIn, basDir);
      // printf(" abs-from-rel = |%s|\n",fnTmp1);
    pf1 = fnTmp1;   


  //================================================================
  L_abs:   // resolve absolute filenam; starts with /

    // test if filename-absolute starts with a symbolPath
    irc = MDLFN_symDir__fnAbs (symDir, fnTmp2, pf1);
    if(irc > 0) {
      // found symbol;
      strncpy(fDir, pf1, irc);
      fDir[irc] = '\0';
      pfn = fnTmp2;
      goto L_fnam;
    }
    // separate path - filename; find last delimiter
#ifdef _MSC_VER
    pfn = UTX_find_strrchrn(pf1, "/\\");
#else
    pfn = strrchr(pf1, fnam_del);
#endif
      // printf(" fnam1__-pfn|%s|\n",pfn);

    // copy fDir
    sdl =  pfn - pf1 + 1;
    if(sdl >= 128) return -2;      // string too long
    strncpy(fDir, pf1, sdl);
    fDir[sdl] = '\0';
    ++pfn;  // skip deli
    goto L_fnam;



  //================================================================
  L_fnam:
  // get filename, starting at pfn
  // find pft = filetype-delimiter
    // printf(" L_fnam: |%s|\n",pfn);
  pft = strrchr(pfn, '.');
  if(!pft) {
    // no filetype;
    fTyp[0] ='\0';
    // copy the filname
    strcpy(fNam, pfn);

  } else {
    snl = pft - pfn;   // printf(" snl=%d\n",snl);
    if(snl) {
      ++pft; // skip '.'
      // copy filetyp
      if(strlen(pft) >= 32) return -3;
      strcpy(fTyp, pft);
      // copy the filname
      if(snl >= 128) return -2;
      strncpy(fNam, pfn, snl);
      fNam[snl] = '\0';

    } else {
      // no filetype
      if(strlen(pft) >= 128) return -2;
      strcpy(fNam, pft);
    }
  }


  //================================================================
  L_exit:
    if(!irc) irc = strlen(symDir);

      printf("ex-MDLFN_fNam_resolv %d |%s|%s|%s|%s|\n",irc,symDir,fDir,fNam,fTyp);
      // exit(0);

  return irc;

}
*/

//=============================================================================
  int MDLFN_symDir__fnAbs (char *symDir, char *fnIn) {
//=============================================================================
// Mod_symDir__fnAbs    get symbolID and symbol-path for filename-absolute
// - expands shell variables in fnIn and if directoryfile <cfgDir>/dir.lst
// 
// Input: 
//   fnIn        filename-absolute; size must be <= SIZFNam
// Output:
//   symDir      name of symbolic-directory [plus extraPath]
//               size must be SIZMFSym
//   RetCod 0    no symbolic-path in fnIn
//         -1    error stringLength
//         >0    length of symbolic-path found.
//
// Example: fnIn = "/mnt/serv2/devel/cadfiles/gcad/GIS1/bk1.gcad"
//          symDir = "Data/GIS1"    (/mnt/serv2/devel/cadfiles/gcad/)
//
// Example: fnIn   = "${DIR_DEV}cadfiles/gcad/"
//          symDir = "Data"    (/mnt/serv2/devel/cadfiles/gcad/)


  int     irc, lss, lsi, lsx;
  char    s1[512], s2[400], *p0, *p1, *p2;
  FILE    *fpi;


  // printf("MDLFN_symDir__fnAbs |%s|\n",fnIn);


  // expand shell variables in fnIn
  p2 = strchr (fnIn, '$');
  if(p2) irc = OS_osVar_eval (fnIn, SIZFNam);

  // get lsi = length of path of fnIn
  p0 = strrchr(fnIn, fnam_del);
  if(p0) lsi = p0 - fnIn;
  else   lsi = strlen(fnIn);


  // try to open symDirList
  MDLFN_syFn_f_name (s1);   // get filename of dir.lst

  if((fpi=fopen(s1,"r")) == NULL) {
    TX_Error("Mod_sym_get2 E001-file %s not found",s1);
    irc = -1;
    goto L_exit;
  }

  // loop tru file dir.lst
  while (!feof (fpi)) {
    // read s1 = next line = next |symbol directory| from file
    if (fgets (s1, 500, fpi) == NULL) break;
    p0 = UTX_pos_1n (s1); // skip blanks
    // skip comment
    if(*p0 == '#') continue;
    UTX_CleanCR (p0);
    // separate ID - path
    p1 = strchr (p0, ' ');
    if(p1 == NULL) continue;
    *p1 = '\0';     // cut symNam - pathNam
    ++p1;
    // skip all following blanks
    p1 = UTX_pos_1n (p1);
      // printf(" symDir__fnAbs-s_act=|%s|\n",p1);

    // test if symbolPath has "$"-variable
    p2 = strchr (s2, '$');
    // expand shell variables in symbolPath
    if(p2) irc = OS_osVar_eval (p1, sizeof(s2));
    // if(p2) irc = OS_filnam_eval (s2, p1, sizeof(s2));
    // else strcpy (s2, p1);
      // printf(" symDir__fnAbs-eval-p1 |%s|\n",p1);

    // compare symbolPath - inFilePath
    lss = strlen(p1);
    if(!strncmp (p1, fnIn, lss)) {
      if(lss >= SIZMFSym) {irc = -1; goto L_done;}
      strcpy(symDir, p0);
      irc = lss;
      // check for extraPath
        // printf(" symDir__fnAbs-lsi=%d lss=%d\n",lsi,lss);
      if(lsi > lss) {
        // add "/extraPath"
        lsx = lsi - lss;
        strcat(symDir, fnam_del_s);
        strncat(symDir, &fnIn[lss], lsx);
      }
      // strcat(symDir, fnam_del_s);
      goto L_done;
    }
  }

  // nothing found
  symDir[0] = '\0';
  irc = 0;

  L_done:
  fclose(fpi);


  //----------------------------------------------------------------
  L_exit:
  
    // printf("ex MDLFN_symDir__fnAbs %d |%s|\n",irc,symDir);

  return irc;

}


//================================================================
  int MDLFN_ofn_user (stru_FN *mns1, char *wTit, char *filter) {
//================================================================
// MDLFN_ofn_user       open file, with directorySelect, filter, waiting.
//   does not modify globals AP_mod_dir ..
// Input:
//   fNam       startfile, can be empty
//   dNam       startdirectory (can be with filename)
//   title      of window
//   sf         filtertext
// Output:
//   retCode    0=OK, -1=Cancel, 1= no-symbolic-path, -2=fSiz/dSiz too small
// see AP_fnam_get_user_1 GUI_file_open__


#define FNSIZ 256

  int   irc;
  char  filNam[FNSIZ], dirLst[FNSIZ];


  // printf("MDLFN_ofn_user |%s|%s|\n",wTit,filter);
  // printf(" |%s|\n",AP_mod_dir);

  // get filename of dir.lst (<cfg>/dir.lst)
  MDLFN_syFn_f_name (dirLst);

  // set startDirectory = active model-directory
  strcpy(filNam, AP_mod_dir);

  // call GUI_file/save
  irc = AP_GUI__ (filNam, FNSIZ, "GUI_file", "open",
                  filNam,    // outDir/outfilename
                  dirLst,    // filename of symbolic-directories
                  filter,    // filterText  (eg "*")
                  wTit,      // window-title
                  NULL);
  if(irc < 0) goto L_exit;

  // split selected filename and fill mns1
  irc = MDLFN_oFn_fNam (mns1, filNam);
  if(irc == -1) irc = 1;  // no-symbolic-path


  //----------------------------------------------------------------
  L_exit:
    // printf("ex-AP_fnam_get_user_2__  %d |%s|\n",irc,filNam);

    // MDLFN_dump_ofn (mns1, "ex-MDLFN_ofn_user");

  return irc;

}


//================================================================
  int MDLFN_oFn_fNam (stru_FN *ofn, char *fnIn) {
//================================================================
// MDLFN_oFn_fNam         get fileName-obj from (unknown) filename
// - also expands shell-variables, internal-modelnames, catalog-parts,
//   and  symbolic-directories
// - internal-modelnames:  (no directory, no filetype) MBTYP_INTERN
// - catalog-parts: eg "fast/bolt/SKS_10x45.ctlg" ([dir/]catalog/part) MBTYP_CATALOG
// Input:
//   fnIn       any filename; not static (following blanks are removed)
// Output:
//   RetCod:    -1=no-symDir, dir=path; -2=internal-error; -3=filetyp-too-long
//               0=no-symDir;
//              >0=length of symbolic-path
//
// Example:    "Data/Niet1.gcad"  sets:
//   ofn->symDir |Data|                                   may NOT have ending '/'
//   ofn->fDir   |/home/fwork/devel/cadfiles/gcad/|       must have ending '/'
//   ofn->fNam   |Niet1|
//   ofn->fTyp   |gcad|
//   ofn->iTyp    0                  // Mtyp_Gcad
//
// Example:     "${DIR_DEV}cadfiles/gcad/symEl1/res1.gcad"
//   ofn->symDir |Data/symEl1|                            syDirNam/[extraPath/]
//   ofn->fDir   |/home/fwork/devel/cadfiles/gcad/|       directory Data
//   ofn->fNam   |res1|
//   ofn->fTyp   |gcad|
//   ofn->iTyp    0                  // Mtyp_Gcad
//
// CatalogPart:    "fast/bolt/SKS_10x45.ctlg"
// symDir |fast/bolt|                                     [extraPath/]catalogName
// fDir   |/mnt/serv2/devel/gcad3d/gCAD3D/ctlg/|          directory CATALOG
// fNam   |SKS_10x45|                                     partName
// fTyp   |ctlg|
// iTyp    -2            MBTYP_CATALOG
//
// Example: internal subModel "sm1"
//  symDir ||
//  fDir   |/mnt/serv2/devel/gcad3d/gCAD3D/tmp/|          tmpdir
//  fNam   |Model_sm1|
//  fTyp   ||
//  iTyp    -1                                            MBTYP_INTERN
//
// Example: // file in relative-path: 
// symDir ||
// fDir   |<pwd>[additional-path/]|
//
// Tests: see MDLFN_test2()
// was Mod_sym_get2 MDLFN_fNam_resolv


  int    irc, ls, sln;
  char   *pft, *pfn, *pfs, *pfp, *p1;
  char   s1[SIZFNam];


  // printf("_________________________________________________ \n");
  // printf("MDLFN_oFn_fNam |%s|\n",fnIn);

  ofn->symDir[0] = '\0';
  ofn->fDir[0] = '\0';
  ofn->fNam[0] = '\0';
  ofn->fTyp[0] = '\0';
  ofn->iTyp    = MBTYP_ERROR;   // no filetyp found
  irc = 0;

  // skip leading blanks
  UTX_pos_skipLeadBlk (fnIn);

  // copy filename
  sln = strlen(fnIn);
  if(sln >= SIZFNam) {TX_Error("MDLFN_oFn_fNam string too long"); return -1;}
  strcpy(s1, fnIn);
  fnIn = s1;

  // remove following blanks ..
  ls = UTX_del_follBl (fnIn);
  if(ls >= SIZFNam) goto L_err_ls;

  // remove a leading and a following '"'
  p1 = &s1[strlen(s1) - 1];
  if((s1[0] == '"')&&(*p1 == '"')) {
    *p1 = '\0';
    ++fnIn;
  }
    // printf(" oFn_fNam-fnIn=|%s|\n",fnIn);

  // expand shell variables
  p1 = strchr (fnIn, '$');
  if(p1) {
    // expand shell variables
    irc = OS_osVar_eval (fnIn, sizeof(s1));
    if(irc < 0) {TX_Error("MDLFN_oFn_fNam OS_osVar_eval"); return -1;}
      // printf(" oFn_fNam-exp=|%s|\n",fnIn);
  }

  //----------------------------------------------------------------
  // check for directory only
  // find pfn = last delimiter
  pfn = OS_FIND_STRR_DELI (fnIn);
    // printf(" oFn_fNam-pfn=|%s|\n",pfn);
  if(pfn) {
    sln = strlen(fnIn);
    // is pfn last char of string -
    if(pfn == &fnIn[sln - 1]) {
      *pfn = '\0';
      goto L_dir;
    }
  }
    // printf(" oFn_fNam-1 |%s|%s|\n",fnIn,pfn); fflush (stdout);


  //----------------------------------------------------------------
  // get filetyp
  pft = strrchr (fnIn, '.');        // pft = position-last-point

  if(pft) {
    // test for starting '.' - relative filepath without fileTyp
    if(pft > &fnIn[0]) {
      *pft = '\0';
      // skip '.'
      ++pft;
      if(strlen(pft) >= SIZMFTyp) {irc = -1; goto L_exit;}
      // copy filetyp
      strcpy(ofn->fTyp, pft);
      ofn->iTyp = AP_iftyp_ftyp (ofn->fTyp);
    }
  }


  //----------------------------------------------------------------
  // get filename
  // separate path - filename
  // check for internal-model
  if(!pfn) {
    // no name-delimiter -
    if(!pft) {
      // no name-delimiter, no type-delimiter = internal-model
      strcpy(ofn->fDir, AP_get_tmp_dir());
      sprintf(ofn->fNam, "Model_%s",fnIn);
      ofn->iTyp = MBTYP_INTERN;
      goto L_exit;
    }
    strcpy(ofn->fNam, fnIn);
    // fDir - pwd  ??
    goto L_exit;
  }


  // copy filename
  ++pfn;
  strcpy(ofn->fNam, pfn);
  *pfn = '\0';   // cut ctring, do NOT YET remove ending '/' from path
                 //  absolute path needs that


  //----------------------------------------------------------------
  // check for absolute path ..
  L_dir:
  // unix: starting with '/'; MS: second char ':'
  if(!OS_ck_DirAbs(fnIn)) {
    goto L_abs;    // MS-Win-compat !
  }
    // printf(" oFn_fNam-L_dir |%s|\n",fnIn); fflush (stdout);

  // check for relative path
  if(fnIn[0] == '.') goto L_rel;


  //----------------------------------------------------------------
  // remove ending '/' from path
  --pfn;
  if(*pfn == fnam_del) *pfn = '\0';

  // get pfp = pos of last delimeter '/'
  pfp = OS_FIND_STRR_DELI (fnIn);
  if(pfp) {
    if(strlen(pfp) == 1) {
      // remove last char from fnIn ('/')
      *pfp = '\0';
      pfp = NULL;
    }
  }

  // <symbol[/path]>      or <symbol>
  //  pfs    pfp              pfs,    pfp=""
    // printf(" oFn_fNam-sym fnIn=|%s| pfp=|%s|\n",fnIn,pfp);


  // check for catalog-part    eg <[dir/]catalog>/<part>.ctl
  if(ofn->iTyp == MBTYP_CATALOG) goto L_ctlg;


  //----------------------------------------------------------------
  // get path for symbol (without extraPath) -> ofn->fDir
  irc = MDLFN_fDir_syDir (ofn->fDir, fnIn);
  if(irc < 0) irc = -1; // no symbol, (relative) path - forbidden without "./"

  // add extraPath to fDir
  if((pfp)&&(strlen(pfp))) {strcat(ofn->fDir, &pfp[1]); strcat(ofn->fDir, fnam_del_s);}

  // get symDir (with extraPath)
  strcpy(ofn->symDir, fnIn);
    // printf(" oFn_fNam-symDir=|%s| fDir=|%s|\n",ofn->symDir,ofn->fDir);
  goto L_exit;


  //----------------------------------------------------------------
  L_ctlg:
    // catalogpart; 
      // printf(" oFn_fNam-ctlg fnIn=|%s| pfp=|%s|\n",fnIn,pfp); fflush (stdout);

    // get fDir = <CATALOG>/[dir/]
    irc = MDLFN_fDir_syFn (ofn->fDir, "CATALOG");
    if(irc < 0) {irc = -3; goto L_exit;}
      // printf("  oFn_fNam-L_ctlg2: fDir=|%s|\n",ofn->fDir); fflush (stdout);

    // add extraPath to fDir
    if(pfp) {strcat(ofn->fDir, &pfp[1]); strcat(ofn->fDir, fnam_del_s);}
      // printf("  oFn_fNam-L_ctlg3: fDir=|%s|\n",ofn->fDir);

    strcpy(ofn->symDir, fnIn);
    goto L_exit;



  //----------------------------------------------------------------
  // get absolute path - starting at fnIn, ending at at pfn;
  L_abs:
      // printf(" oFn_fNam-L_abs: |%s|\n",fnIn);
    // test if filename-absolute starts with a symbolPath  (<symDir>[/path])

    irc = MDLFN_symDir__fnAbs (ofn->symDir, fnIn);
    if(irc < 0) {irc = -1; goto L_exit;}
      // printf(" oFn_fNam-abs-irc=%d fnIn=|%s| symDir=|%s|\n",irc,fnIn,ofn->symDir);

    strcpy(ofn->fDir, fnIn);
    goto L_exit;


  //----------------------------------------------------------------
  L_rel:
    // relative path - starts at fnIn and ends at pfp;
      // printf("  oFn_fNam-L_rel: fnIn=|%s| pfp=|%s|\n",fnIn,pfp);

    strcpy(ofn->fDir, OS_get_dir_pwd ());
    UTX_add_fnam_del (ofn->fDir);  // add closing "/"
      // printf("  oFn_fNam-L_rel: pfs=|%s| pfp=|%s|\n",pfs,pfp);

    // add additional-path; no path: pfp="/"
    // fnIn += 2;  // skip "./"
    if(strlen(fnIn) > 2) strcat(ofn->fDir, &fnIn[2]);
//       if(pfp > fnIn) strcat(ofn->fDir, &fnIn[2]);
//     }



  //================================================================
  L_exit:

      // MDLFN_dump_ofn (ofn, "ex-MDLFN_oFn_fNam");
      // printf(" ex-MDLFN_oFn_fNam irc=%d |%s|\n",irc,fnIn);fflush(stdout);

    return irc;


  //----------------------------------------------------------------
  L_err_ls:
    TX_Print("***** MDLFN_oFn_fNam string too long %s",fnIn);
    irc = -1;
    goto L_exit;

}


//================================================================
  int MDLFN_get__ (stru_FN *smfn) {
//================================================================
// MDLFN_get__            get a copy of AP_mod_*

  // printf("MDLFN_ofn_set |%s|%s|%s|%s| %d\n",
         // AP_mod_sym,AP_mod_dir,AP_mod_fnam,AP_mod_ftyp,AP_mod_iftyp);

  strcpy(smfn->symDir,  AP_mod_sym);
  strcpy(smfn->fDir,    AP_mod_dir);
  strcpy(smfn->fNam,    AP_mod_fnam);
  strcpy(smfn->fTyp,    AP_mod_ftyp);

  smfn->iTyp =          AP_mod_iftyp;

    // MDLFN_dump_ofn (smfn, "ex-MDLFN_get__");
 
  return 0;

}


//================================================================
  int MDLFN_set__ (stru_FN *smfn) {
//================================================================
// MDLFN_set__            set AP_mod_* (eg restore)

  strcpy(AP_mod_sym, smfn->symDir);
  strcpy(AP_mod_dir, smfn->fDir);
  strcpy(AP_mod_fnam, smfn->fNam);
  strcpy(AP_mod_ftyp, smfn->fTyp);

  AP_mod_iftyp = smfn->iTyp;


  // add closing '/' if necessary
  UTX_add_slash (AP_mod_dir);


  return 0;

}


//================================================================
  int MDLFN_syFn_f_name (char *cbuf) {
//================================================================
// MDLFN_syFn_f_name       get filename of list of symbolic directories
// Output:
//   cbuf       size >= 256
//   (<cfgDir>/dir.lst)
// was AP_get_fnam_symDir

//   sprintf(cbuf,"%sdir.lst",AP_get_cfg_dir());
  sprintf(cbuf,"%s%s",AP_get_cfg_dir(),AP_symDir_fnam);

    // printf("ex-MDLFN_syFn_f_name |%s|\n",cbuf);

  return 0;

}


//================================================================
  int MDLFN_syFn_f_del (char *sym) {
//================================================================
// Mod_sym_del         delete symbolic path in file cfg/dir.lst
// Input:
//   sym         symbolic-directory to delete in active symbolic-directory-file

  char    fn[320];


  // printf("MDLFN_syFn_f_del |%s|\n",sym);


  MDLFN_syFn_f_name (fn);   // get filename of dir.lst
    // printf("  _syFn_f_name |%s|\n",fn);

  // delete sym
  UTX_setup_set (fn, sym, NULL);

  return 0;

}


//================================================================
   int MDLFN_syFn_f_add (char *sym, char *dir) {
//================================================================
// MDLFN_syFn_f_add         add symbolic path in File cfg/dir.lst
// Input:
//   sym         symbolic-directory to add to active symbolic-directory-file
//   dir         path of symbolic-directory sym
//
// symbolic-directory-file ../../gCAD3D/cfg_<os>/dir.lst


  int    irc;
  char   fn[320], safNam[SIZMFSym];


  // printf("MDLFN_syFn_f_add |%s|%s|\n",sym,dir);

  // make symbolic-directory safe, check length
  irc = MDL_safename (safNam, sym);
  if(irc < 0) return -1;


  // get filename of dir.lst
  MDLFN_syFn_f_name (fn);
    // printf(" _syFn_f_add-fn=|%s|\n",fn);

  //add sym
  UTX_setup_set (fn, safNam, dir);

  TX_Print ("----- symbolic-directory %s added ..");

  return 0;

}


//================================================================
  int MDLFN_syFn_f_dump () {
//================================================================
/// MDLFN_syFn_f_dump        dump all symbolic directories

  char    s1[256];
  FILE    *fpi;


  MDLFN_syFn_f_name (s1);   // get filename of dir.lst


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
  int MDLFN_dump__ (char *inf) {
//================================================================
// MDLFN_dump__               dump AP_mod_*


  printf("------------- MDLFN_dump__ %s\n",inf);
  printf(" symDir |%s|\n",AP_mod_sym);
  printf(" fDir   |%s|\n",AP_mod_dir);
  printf(" fNam   |%s|\n",AP_mod_fnam);
  printf(" fTyp   |%s|\n",AP_mod_ftyp);
  printf(" iTyp    %d\n",AP_mod_iftyp);

  return 0;

}


//================================================================
  int MDLFN_dump_ofn (stru_FN *smn, char *inf) {
//================================================================
// MDLFN_dump_ofn             dump struct stru_FN


  printf("------------- MDLFN_dump_ofn %s\n",inf);
  printf(" symDir |%s|\n",smn->symDir);
  printf(" fDir   |%s|\n",smn->fDir);
  printf(" fNam   |%s|\n",smn->fNam);
  printf(" fTyp   |%s|\n",smn->fTyp);
  printf(" iTyp    %d\n",smn->iTyp);

  return 0;

}



//================================================================
  int MDLFN_test1 () {
//================================================================

  char   *sBas, s0[256], s1[128], s2[128], s3[128], s4[128];

  
  printf("MDLFN_test1\n");

  sBas = AP_get_bas_dir();

//   MDLFN_symDir__fnAbs (s1, s0, "/mnt/serv2/devel/cadfiles/gcad/GIS1/bk1.gcad");
//   MDLFN_symDir__fnAbs (s1, s0, "/mnt/serv1/devel/cadfiles/gcad/GIS1/bk1.gcad");
//   return 0;

  // MDLFN_fNam_resolv (s1, s2, s3, s4, "/mnt/serv2/devel/cadfiles/gcad/GIS1/bk1.gcad",                     sBas);
  // MDLFN_fNam_resolv (s1, s2, s3, s4, "Data/GIS1/bk1.gcad", sBas);
//   MDLFN_fNam_resolv (s1, s2, s3, s4, "/Data/GIS1/bk1.gcad", OsBas);
//   MDLFN_fNam_resolv (s1, s2, s3, s4, "../GIS1/bk1.gcad", sBas);
//   MDLFN_fNam_resolv (s1, s2, s3, s4, "../../GIS1/bk1.gcad", sBas);
//   MDLFN_fNam_resolv (s1, s2, s3, s4, "bk1.gcad", sBas);

  return 0;

}


//================================================================
  int MDLFN_test2 () {
//================================================================
// test MDLFN_oFn_fNam


  char fn1[SIZFNam];


  MDLFN_test3 ("Data/gMsh1/");

  MDLFN_test3 ("Data/sample_dxf1.dxf");

  MDLFN_test3 ("Data/symEl1/res1.gcad");

  MDLFN_test3 ("${DIR_DEV}cadfiles/gcad/test_models_4.stp");

  MDLFN_test3 ( "${DIR_DEV}cadfiles/gcad/symEl1/res1.gcad");

  MDLFN_test3 ( "/home/fwork/old_fwork/fwork/gCAD3D/dat/sample_dxf1.dxf");

  MDLFN_test3 ( "Profile/6x4_150.ctlg");

  MDLFN_test3 ( "fast/bolt/SKS_10x4.ctlg");

  MDLFN_test3 ( "./xy");

  MDLFN_test3 ( "./xy.dat");

  MDLFN_test3 ( "./addPath/xy.dat");

  MDLFN_test3 ( "sm1");           // internal-model

  return 0;

}


//================================================================
  int MDLFN_test3 (char *fn1) {
//================================================================
// test MDLFN_oFn_fNam

  stru_FN ofn;
  char ffNam[SIZFNam];

  printf("\n\n=================== _test3 |%s|\n",fn1);


  MDLFN_oFn_fNam (&ofn, fn1);

  MDLFN_dump_ofn (&ofn, "  _test3");

  MDLFN_ffNam_oFn (ffNam, &ofn);
    printf(" ffNam=|%s|\n",ffNam);




  return 0;

}


// eof
