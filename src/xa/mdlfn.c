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
-----------------------------------------------------
TODO:
- replace Mod_sym_* functions with MDLFN_* functions 
- MDLFN_fNam_resolv not yet working with MS (abs. filenames with // or C:)

-----------------------------------------------------
Modifications:
2020-04-27 New from Mod_sym_* functions. RF.

-----------------------------------------------------
*/
#ifdef globTag
 void MDLFN(){}
#endif
/*!
\file  ../xa/mdlfn.c
\brief MoDeL-FileName processing functions
\code
=====================================================
List_functions_start:

MDLFN_fNam_resolv      get sym,dir,fnam,ftyp from symbolic|rel|abs.filename
MDLFN_symDir__fnAbs    get symbolID and symbol-path for filename-absolute
MDLFN_dirAbs_symDir    get absoute-direcory from symbolic-directory

MDLFN_symFilNam        get filename of list of symbolic directories
MDLFN_test1

List_functions_end:
=====================================================

See Functions Mod_sym_*


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


#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_os.h"               // OS_get_bas_dir ..



//====================================================================
  int MDLFN_dirAbs_symDir (char *absDir, char *symPath) {
//====================================================================
// MDLFN_dirAbs_symDir    get absoute-direcory from symbolic-directory
// symdir must be terminated with '/' or '\0'
// Input:
//   symPath   eg: "Data"  or "Data/tmp/"
// Output:
//   absDir:   full path (from file xa/dir.lst)     Size 128.
//   RetCod
//     >= 0    OK; path in out_path; Linenumber of symbol in path.
//     -1      Symbol <symDir> not found in file
//     -2      path too long
//
// was MDLFN_dirAbs_symDir

  int     irc=0, i1, i2;
  char    fn[256], symDir[128], *p1, *p2;


  // printf("MDLFN_dirAbs_symDir |%s|\n",symPath);


  // get symDir out of symPath (ending with first '/')
  p1 = strchr (symPath, '/');
  if(p1) {
    i1 = p1 - symPath;
    if(i1 > 126) return -2;
    strncpy(symDir, symPath, i1);
    symDir[i1] = '\0';
    ++p1;  // skip '/'
    i2 = strlen(p1);
  } else {
    if(strlen(symPath) > 126) return -2;
    strcpy(symDir, symPath);
    i2 = 0;
  }
    // printf(" symDir=|%s| i2=%d\n",symDir,i2);


  // get filename of dir.lst
  MDLFN_symFilNam (fn);
    // printf(" fn-dir.lst=|%s|\n",fn);


  // get absDir from symDir out of file
  irc = UTX_setup_get__ (absDir, symDir, fn);
  if(irc) return irc;


  // test if absDir has "$"
  p2 = strchr (absDir, '$');
  if(p2) {
    // expand shell variables
    irc = OS_filnam_eval (absDir, absDir, 128);
  }


  // add rest of path (following p2)
  if(i2) {
    i1 = strlen(absDir) + i2;
    if(i1 > 128) return -2;
    strcat(absDir, p1);
  }

    // printf("ex-MDLFN_dirAbs_symDir |%s|%s|\n",absDir,symPath);

  return irc;

}


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
//   basDir     active directory; get it eg with OS_get_bas_dir()
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


  int    irc;
  int    sdl, snl;
  char   *pfn, *pft, *pf1, fnTmp1[256], fnTmp2[256];


  // printf("_________________________________________________ \n");
  // printf("MDLFN_fNam_resolv |%s|\n",fnIn);
  // printf(" basDir=|%s|\n",basDir);


  symDir[0] ='\0';
  fDir[0] ='\0';
  irc = 0;

  UTX_pos_skipLeadBlk (fNam);


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

  // get fDir = path of symDir out of file
  irc = MDLFN_dirAbs_symDir (fDir, symDir);
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
     // printf("ex-MDLFN_fNam_resolv %d |%s|%s|%s|%s|\n",irc,symDir,fDir,fNam,fTyp);
  return irc;

}


//====================================================================
  int MDLFN_symDir__fnAbs (char *symDir, char *fnAp, char *fnIn) {
//====================================================================
// Mod_symDir__fnAbs    get symbolID and symbol-path for filename-absolute
// Example: fnIn = "/mnt/serv2/devel/cadfiles/gcad/GIS1/bk1.gcad"
//          symDir="Data", fDir="GIS1/bk1.gcad"
// Input: 
//   fnIn        filename-absolute
// Output:
//   symDir      symbolID (if fnIn start with symbol-path); else empty
//   fnAp        remaining part of fnIn; maxLen 256
//   RetCod 0    no symbolic-path in fnIn
//         -1    error stringLength
//         >0    length of symbolic-path found.

  
  int     irc, ls;
  char    s1[256], s2[256], *p0, *p1, *p2;
  FILE    *fpi;

  
  // printf("MDLFN_symDir__fnAbs |%s|\n",fnIn);
  
  
  // try to open symDirList
  MDLFN_symFilNam (s1);   // get filename of dir.lst
  // sprintf(s1,"%sxa%cdir.lst",OS_get_bas_dir(),fnam_del);

  if((fpi=fopen(s1,"r")) == NULL) {
    TX_Error("Mod_sym_get2 E001-file %s not found",s1);
    return -1;
  }

  // loop tru file dir.lst
  while (!feof (fpi)) {
    // read next line
    if (fgets (s1, 256, fpi) == NULL) {
      // no symbol found in fnIn
      symDir[0] = '\0';
      if(strlen(fnIn) > 256) return -1;
      strcpy(fnAp, fnIn);
      irc = 0;
      goto L_exit;
    }
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
      // printf(" s_act=|%s|\n",p1);
    // test if symbolPath has "$"-variable
    p2 = strchr (s2, '$');
    // expand shell variables in symbolPath
    if(p2) irc = OS_filnam_eval (s2, p1, sizeof(s2));
    else strcpy (s2, p1);
      // printf(" cb_dirsym2-s2 |%s|\n",s2);
    // compare symbolPath - inFilePath
    ls = strlen(s2);
    if(!strncmp (s2, fnIn, ls)) {
      if(strlen(p1) > 256) return -1;
      strcpy(symDir, p0);
      strcpy(fnAp, &fnIn[ls]);
      irc = ls;
      break;
    }
  }

  //----------------------------------------------------------------
  L_exit:
  fclose(fpi);

    // printf("ex MDLFN_symDir__fnAbs %d |%s|%s|\n",irc,symDir,fnAp);

  return irc;

}


//================================================================
  int MDLFN_symFilNam (char *cbuf) {
//================================================================
// MDLFN_symFilNam       get filename of list of symbolic directories
//   (<cfgDir>/dir.lst)
// was AP_get_fnam_symDir

  sprintf(cbuf,"%sdir.lst",OS_get_cfg_dir());

    // printf("MDLFN_symFilNam |%s|\n",cbuf);


  return 0;

}


//================================================================
  int MDLFN_test1 () {
//================================================================

  char   *sBas, s0[256], s1[128], s2[128], s3[128], s4[128];

  
  printf("MDLFN_test1\n");

  sBas = OS_get_bas_dir();

//   MDLFN_symDir__fnAbs (s1, s0, "/mnt/serv2/devel/cadfiles/gcad/GIS1/bk1.gcad");
//   MDLFN_symDir__fnAbs (s1, s0, "/mnt/serv1/devel/cadfiles/gcad/GIS1/bk1.gcad");
//   return 0;

  MDLFN_fNam_resolv (s1, s2, s3, s4, "/mnt/serv2/devel/cadfiles/gcad/GIS1/bk1.gcad",                     sBas);
  MDLFN_fNam_resolv (s1, s2, s3, s4, "Data/GIS1/bk1.gcad", sBas);
//   MDLFN_fNam_resolv (s1, s2, s3, s4, "/Data/GIS1/bk1.gcad", OsBas);
//   MDLFN_fNam_resolv (s1, s2, s3, s4, "../GIS1/bk1.gcad", sBas);
//   MDLFN_fNam_resolv (s1, s2, s3, s4, "../../GIS1/bk1.gcad", sBas);
//   MDLFN_fNam_resolv (s1, s2, s3, s4, "bk1.gcad", sBas);

  return 0;

}

// EOF
