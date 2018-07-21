// ApplicationSpecific Textfunctions im Memory.
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
  ..

-----------------------------------------------------
*/
#ifdef globTag
 void APED(){}
 void OSRC(){}
#endif
/*!
\file  ../xa/xa_ed_mem.c
\brief ApplicationSpecific Textfunctions im Memory 
\code
=====================================================
List_functions_start:

APED_oid_dbo__             make name from typ and DB-Index
APED_oid_dbo_all           make name from typ and DB-index  (all types)

APED_find_dbo              Search last DefinitionLine for DB-obj
APED_search_defLn          Search last DefinitionLine for obj
APED_nxt_def_typ           Search next DefinitionLine of objectTyp
APED_defDbo_skip           skip definition-header (skip eg "P20=")

APED_objID_defLn           copy definitionHdr out (left of '=')
APED_ck_defLn              check if Line cBuf is a Definitionline
APED_dec_defLn             check if Line cBuf is a Definitionline; decode ..
APED_dec_txt               decode textLine, add all objs to group (oTab)
APED_decAdd                decode textLine, add all objs to group (oTab)

APED_oid_src1              get next objID in string
APED_search_objID          search objID otx in line ps[lLen]

APED_lNr_objNam            get sourcelineNr from ObjName
APED_onam_search           search objName between ps and pe
APED_onam_cut              cut/copy objName

APED_src_chg               deactivate line, insert modified line
// APED_undo_do               undo/redo (Delete/restore) line   UNUSED !
APED_update__              update display after APED_undo_redo

APED_find_dep__            find all depending sourceObjs (childs)
APED_find_dep_nxt          search & add all depending objects
APED_find_depLn_nxt        Search next depending Line 
APED_find_depLn_last       Search last depending Line 
APED_find_depRef           delete all lines for active constrPlane

APED_find_par__            find all parent-Objs
APED_find_par_nxt          search & add all parent objects

APED_pt_find               find position of next point in string
APED_pt_chg                change point from dbi in string
APED_nxt_obj               find next obj in string; skip brackets.

APED_search_dbLimits       search highest indices

APED_txo_srcLn__           analyze sourceline > ObjTXTSRC[]
APED_txo_modObj_get        get all modifyable-objects out of atomic-objects
APED_txo_find_parent       get 1. parent of subObj <ip> typ=Typ_PT
APED_txo_dump              dump ObjTXTSRC[]
APED_txo_find_ptVal        find points with values in tso
// APED_txo_cpy               copy iNr records of tso0 starting at iRec -> tso1

APED_VcTra_reset           delete Vec's&Tra's after progLine x in DB

OSRC_add__                 add record to ObjSRC-table
OSRC_find_odb              test if db-obj is already in ObjSRC-table

List_functions_end:
=====================================================

see also:
//   AP_search_obj / UTF_GetLinNr
//   AP_obj_delCk1 UTF_GetLinNr
APT_obj_expr      Create struct from ModelCode (text).
IE_decode_Ln
AP_search_obj
UTF_GetLinNr
AP_SMD_prv
UTF_
APED_dbo_oid
APT_decode_ausdr
APT_decode_inp
../xa/srcu.c         Dynamic update numeric strings  SRCU_..
SRC   ../xa/xa_src.c       create sourceCode from atomicObjs, dbo ..

\endcode *//*----------------------------------------


APED_ - functions search,modify sourceCodelines in memory.
see also ../xa/xa_ed.c   ED_
  StandardFunctions in Memory: UTF_.. in ../ut/ut_txfil.c


*/


#ifdef _MSC_VER
#include "MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>                     // isdigit ..



#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_txfil.h"            // UTF_GetnextLnPos
#include "../ut/ut_gtypes.h"           // AP_src_typMod
#include "../ut/ut_memTab.h"           // MemTab

#include "../ci/NC_apt.h"              // T_RBSP

#include "../xa/xa_mem.h"              // mem_cbuf1
#include "../xa/xa_sele.h"             // Typ_FncPrv
#define INCLUDE_FULL
#include "../xa/xa_ed_mem.h"           // ObjSRC OSRC_NUL
#undef INCLUDE_FULL
#include "../db/ut_DB.h"               // DB_


typedef_MemTab(ObjDB);


//================================================================
// EXTERNALS:
// aus xa.c:
extern char      AP_ED_oNam[128];        // objectName of active Line


// ex ../ut/ut_txfil.c
extern char *UTF_FilBuf0;
extern long UTF_FilBuf0Len;  // die aktuelle Filesize

extern char *UTF_FilBuf1;
extern long UTF_FilBuf1Len;  // die aktuelle Filesize


// ex ../ut/ut_gtypes.c
extern char *ObjCodTab[];
extern char *NcoTxtTab[];
extern char *GcoTxtTab[];
extern char *CopTxtTab[];
extern char  MOpTxtStr[];



//================================================================
  int APED_defDbo_skip (char **txso) {
//================================================================
// APED_defDbo_skip           skip definition-header (skip eg "P20=")

  char   *p1;

  p1 = strchr(*txso, '=');
  if(!p1) return -1;

  ++p1;  // skip '='
  while(*p1 == ' ') ++p1;    // skip blanks

  *txso = p1;

  return 0;

}


//================================================================
  char* APED_lNr_objNam (long *lNr, long *lLen, char **oNam,
                         char *wTab[], long lSta) {
//================================================================
/// \code
/// get next line with objName containing all filterwords (OR)
/// if wTab is empty, next srcline is returned.
/// Input:
///   wTab    filtertext (a list of words, terminated with NULL)
///   lSta    first sourcelinNr where to search; 
/// Output:
///   lNr     sourcelinNr found
///   lLen    length of sourceline (incl name)
///   oNam    pointer to found objName
/// RetCod:
///   NULL    no (more) line found
///   != NULL OK, startpos of sourceline found
///
/// wTab: use UTX_wTab_str
/// \endcode


  int     i1;
  char    *p1, *pNam, *actPos, *endPos, *posEof, **pa;


  // printf("APED_lNr_objNam %d\n",lSta);

  pNam = NULL;

  // get startPosition
  if(lSta < 1) lSta = 1;
  actPos = UTF_GetPosLnr (lLen, lSta);
  if(!actPos) return NULL;

  endPos = actPos + *lLen;
  UTF_GetPos2 (&p1, &posEof);
  goto L_test;


  // get next line    (see UTF_GetnextLnPos)
  L_nxt_ln:
    actPos = endPos + 1;
    if(actPos >= posEof) return NULL;
    ++lSta;
    endPos = strchr(actPos, '\n');
    *lLen = endPos - actPos;


  //----------------------------------------------------------------
  L_test:
    p1 = actPos;
    while (*p1  == ' ')  { ++p1; }
    // skip commentlines
    if(*p1 == ' ') goto L_nxt_ln;
    // skip deleted lines
    if(*p1 == '_') goto L_nxt_ln;
      // printf(" nxtLn: |");UTX_dump_cnl(actPos, 300);printf("|\n");
    // search for objectName (" #"); copy Name -> actNam
    pNam = APED_onam_search (actPos, endPos);

    // accept line if no namefilter given
    if(wTab[0] == NULL) goto L_exit;
    if(!pNam) goto L_nxt_ln;
      // printf(" lNr=%d objNam: |",lSta);UTX_dump_cnl(pNam, 100);printf("|\n");


  //----------------------------------------------------------------
  // found line with objName; test all filterWords.
  pa = wTab;
  while (*pa) {
      // printf(" test |%s|\n",*pa);
    // if(UTX_memstr(pNam, endPos, *pa) == NULL) goto L_nxt_ln;    // AND
    if(UTX_memstr(pNam, endPos, *pa) != NULL) goto L_exit;    // OR
    ++pa;
  }
  goto L_nxt_ln;  // OR


  //----------------------------------------------------------------
  L_exit:
    *lNr  = lSta;
    *oNam = pNam;

    return actPos;

}


//================================================================
  int APED_oid_src1 (int *typ, long *dbi, char **ps, long *ll) {
//================================================================
/// \code
/// APED_oid_src1                  get next objID in string
///  Used for get parents of object
///  Does not recognize DX RX ..
/// Input:
///   ps      startpos of sourcetext
///   ll      lineLength (excluding definitionHeader, objName)
/// Output:
///   typ,dbi dbo (if rc==0)
///   ps      startpos of next word (not yet analyzed ..)
/// RetCod:  
///            0   line contains objID <typ,dbi>
///           -1   Error
///
/// see also APED_search_objID
/// \endcode

// see also APED_nxt_obj

  // int    typ;
  // long   dbi;
  char   *p1, *p2, *pe;


  // printf("APED_oid_src1 %ld |",*ll);UTX_dump_cnl(*ps, 60); printf("|\n");

  p1 = *ps;
  pe = p1 + *ll;


  // skip deli
  L_1:
      // printf(" L_1: |");UTX_dump_cnl(p1, 60); printf("|\n");
    if(p1 >= pe) return -1;
    // p2 = strpbrk (p1, " ,()\"");
    p2 = strchr (" ,()\"", *p1);
    if(p2) {
      if(*p2 == '\"') {
        p1 = UTX_pos_skipStr (p1); // skip string ..
        if(!p1) return -1;
      }
      // p1 = p2 + 1;
      ++p1;
      goto L_1;
    }


  // test if its a obj-char
  *typ = AP_typ_typChar (*p1);
  if(*typ == Typ_Error) {
    // no objID -  must be func. find next delimiter.
    p2 = strpbrk (p1, " ,()\"");
    if(p2) {
      p1 = p2 + 1;
      goto L_1;
    }
    return -1;
  }


  // decode dbi
  ++p1;
  *dbi = strtol (p1, &p2, 10);
    // printf(" typ=%d dbi=%ld\n",*typ,*dbi);

  if(p2 == p1) {
    // no number; try again ..
    ++p1;
    goto L_1;
  }

  // dbi OK; 
  ++p2;   // skip delimiter; ')' or blank or , ...
  *ll -= p2 - *ps;
  *ps = p2;


  return 0;

}


//================================================================
  int APED_search_objID (char *ps, long lLen, char *otx) {
//================================================================
/// \code
/// search objID otx in line ps[lLen]
/// Input:
///   ps      startpos of line, including line header
///   lLen    lineLength (including objName)
///   otx     what to search; objID; eg "P1"
/// RetCod:    1   line is definition of otx  ("<otx> = ..")
///            0   line contains otx as parent-obj (".. = .. <otx> .. ")
///           -1   line does not contain otx
///           -2   line is a commentline
///           -3   line is not active
/// \endcode


  int    otxLen, irc;
  long   l1, ipos;
  char   *pe, *px, *pd, *py;


  // printf("APED_search_objID %ld |%s|",lLen,otx);
  // UTX_dump_cnl(ps, 60); printf("|\n");



  pe = ps + lLen;

  while (*ps  == ' ')  { ++ps; }

  if(*ps == '#') return -2;      // skip comment-line
  if(*ps == '_') return -3;      // skip disactivated line


  // do not search in nameField ("P1=0,0,0 # Origin");
  //   set px=startPos of Info
  px = APED_onam_search (ps, pe);
  if(px == NULL) px = pe;
    // printf(" px=|%s|\n",px);


  // search otx between p2, px
    // printf(" search |%s| in |",otx);UTX_dump_c__(ps,px-ps);printf("|\n");
  pd = UTX_memstr (ps, px, otx);
  if(!pd) return -1;
    // printf(" pd=|%s|\n",pd);


  // OK, found dep.obj otx
  // das nachfolgende Zeichen MUSS ein Trennzeichen sein
    // printf(" deli=|%c|\n",p2[otxLen]);
    // printf(" found |%s| in |",otx);UTX_dump_c__(p2,p4-p2);printf("|\n");
  otxLen = strlen(otx);
  py = &pd[otxLen];
  if(isdigit(*py)) return -1;  // eg search "P1" found "P123"

  while (*py  == ' ')  { ++py; }
  if(*py == '=') return 1;

  return 0;

}


//================================================================
  char* APED_onam_search (char *ps, char *pe) {
//================================================================
/// \code
/// APED_onam_search           search objName between ps and pe
/// returns position of 1.char after " #"  or NULL
/// \endcode

  char      *p1;


  p1 = ps;


  L_search_nxt:
    p1 = strchr(p1, ' ');
    if(p1) {
      if(p1 >= pe) goto L_exit;
        // printf("|");UTX_dump_cnl(p1,40);printf("|\n");
      // blank gefunden; test following char
      if(*(p1+1) != '#') {++p1; goto L_search_nxt;}
      // Name found
      p1 += 2;    // skip ' #'
      UTX_pos_skipLeadBlk (p1); // skip blanks
        // printf("ex APED_onam_search |");UTX_dump_cnl(p1,40);printf("|\n");
      return p1;
    }


  L_exit:
    // printf("ex APED_onam_search NULL\n");
  return NULL;

}


//================================================================
  int APED_onam_cut (char *cbuf, char *onam) {
//================================================================
/// \code
/// APED_onam_cut              cut/copy objName
/// cut cbuf at " #"  and copy the following text -> onam
/// onam size 128
/// MODIFIES cbuf !
/// \endcode

// TODO: add AP_ED_oNam|NULL as parameter 

  // int       ll;
  char      *p1;


  AP_ED_oNam[0] = '\0';
  p1 = cbuf;


  L_search_nxt:
    p1 = strchr(p1, ' ');
    if(p1) {
        // printf("|");UTX_dump_cnl(p1,40);printf("|\n");
      // blank gefunden; test following char
      if(*(p1+1) != '#') {++p1; goto L_search_nxt;}
      // Name found
      *p1 = '\0'; // cut off
      if(onam) {
        p1 += 2;    // skip ' #'
        if(strlen(p1) > 128) {TX_Error("APED_onam_cut E1"); return -1;}
        strcpy(onam, p1);
      }
        // printf(" extract AP_ED_oNam |%s|\n",AP_ED_oNam);
    }

  // ll = p1 - cbuf;

  return 0;

}


//================================================================
  int APED_src_chg (long lNr, char *sNew) {
//================================================================
/// \code
/// APED_src_chg               deactivate line, insert modified line
/// and report change -> UNDO.
/// Input:
///   sNew    new line, complete. \0-terminated;
///             must be a string with at least 2 free characters
///
/// RetCod:   0   Ok, line changed ...
///           1   Error; newLine == activeLine;
///
/// creates a additional line !     Update: APED_update__
/// \endcode

// used in IE_cad_OK, PED_exit

  long    llact, llnew, lNew, llnxt;
  char    *cPos1, *cPos2, cs;


  llnew = strlen(sNew);

  printf("APED_src_chg lNr=%ld llnew=%ld\n",lNr,llnew);
  printf(" new |%s|\n",sNew);
  // printf(" new |");UTX_dump_cnl(sNew,60);printf("|\n");



  WC_set_obj_stat (0);    // save permanent


  // get line-pos
  cPos1 = UTF_GetPosLnr (&llact, lNr);
    // printf(" old ll=%ld |%s|\n",llact,cPos1);
    // printf(" old |");UTX_dump_cnl(cPos1,60);printf("| ll=%d\n",llact);


  // test if sNew != actLine. If yes: no modifications.
  if(llnew == llact) {
    if(!strncmp(sNew, cPos1, llnew)) return 1;
  }
    // printf("  SRC-modified\n");


  // deactivate line
  cs = '_';
  UTF_comment_line (cPos1, 1, &cs);


  //----------------------------------------------------------------
  // insert line sNew
  // get pos of nxt line
  // cPos = UTF_GetnextLnPos (cPos);
  lNew = lNr + 1;
  cPos2 = UTF_GetPosLnr (&llnxt, lNew);


  // terminate new line with \n\0
  // last line: \0    (no \n)
  llnew = strlen(sNew);
  if(cPos2) { sNew[llnew] ='\n'; ++llnew; }   // 2013-09-24
  sNew[llnew] ='\0';
    // printf(" new str |%s|\n",cBuf);


  // insert new line
  //  - insert new code hinter altem Code
  // beim alten modelPositionEditor suchen ..
  if(cPos2 == NULL) {
    UTF_add_line (sNew);                           // add new line
  } else {
    UTF_insertTxt (cPos2, llnew, sNew);            // insert new code
  }


  //----------------------------------------------------------------
  // update DL: increment all (lineNrs >= lNew)
  DL_lnr_incr (lNew);

 
  // report changed-record to UNDO
  UNDO_chg_add (lNr, 0);

  return 0;

}

/*
//================================================================
  int APED_undo_do (int lNr, char *lPos, int mode) {
//================================================================
/// \code
/// APED_undo_do                  undo/redo (Delete/remove) line
/// lPos: startposition of a APT-line in memory.
/// mode: 0=Restore; 1=Delete
/// 
/// see also AP_obj_del2 UI_undo_chg_set
/// \endcode

  int  i1, i2, typ, dbi;
  char *cpe, *cpn;
  // long l1;


  printf("APED_undo_do lNr=%d mode=%d\n",lNr,mode);
  
  if(lPos == NULL) return -1;

  // l1 = ED_get_lnr_act();     // get ED_lnr_act

  if(mode == 0) goto L_rest__;


  //===================================================================
  // Delete: insert '_'
  UTF_comment_line (lPos, mode, "_");

  // TX_Print("Line %d (%12.12s) deleted; Ctrl-Y to restore",lNr,lPos);
  i1 = sprintf (mem_cbuf1, "Line %d (", lNr);
  i2 = UTX_cp_Line(&mem_cbuf1[i1], &cpn, lPos+1, 16) + i1;
  strcat(&mem_cbuf1[i2], ") deleted; Ctrl-Y to restore");
  TX_Print(mem_cbuf1);

  goto L_update;



  //===================================================================
  // Restore: remove first char of line ('_'); add ' ' as last char of line.
  L_rest__:

  // remove first char; add ' ' as last char
  UTF_comment_line (lPos, mode, "_");

  // TX_Print("Line %d (%12.12s) restored",lNr,lPos);
  i1 = sprintf (mem_cbuf1, "Line %d (", lNr);
  i2 = UTX_cp_Line(&mem_cbuf1[i1], &cpn, lPos, 16) + i1;
  strcat(&mem_cbuf1[i2], ") restored");
  TX_Print(mem_cbuf1);

  // ++lNr; // fuers ED_work_CurSet .. ??
  // ++l1;

  
  //----------------------------------------------------------------
  L_update:
  // get typ, dbi
  cpn = lPos;
  if(mode != 0) ++cpn;
  APED_dbo_oid (&typ, &dbi, cpn);


  // delete obj in DB
  if(mode == 1) {
    DB_Delete (typ, dbi);
  }



  //----------------------------------------------------------------
  // update Brw-window
  Brw_obj_upd (typ, dbi, mode);


  return 0;

}
*/

//================================================================
  int APED_update__ (long lNr) {
//================================================================
/// \code
/// APED_update__              update display starting from lineNr
/// set back to <lNr>, update to end of model.
/// lNr -1: start from current lineNr (eg after UTF_insert1)
/// \endcode

// used after APED_undo_redo
 
  long     l1;


  if(lNr < 0) {
    lNr = ED_get_lnr_act();     // get ED_lnr_act
  }
  

  // printf("APED_update__ lNr=%ld\n",lNr);


  // L_update:
  // update editor-window

  WC_set_obj_stat (0);  // 0=permanent


  AP_SRC_edi_mem ();         // copy mem -> Editor & ED_lnr_act=0

  // ED_set_lnr_act (l1);       // reset ED_lnr_act f ED_work_CurSet
  ED_work_CurSet (lNr-1);       // ruecksetzen auf lNr

  // wieder vor bis l1
  // ED_work_CurSet (l1);

  // set cursor
  // ED_setLnr (l1-1);
  ED_work_CurSet (999999);


  // auf die aktuelle zeileNr setzen, update display ..
  // // imply END-Button (Redraw)
  // // UI_AP (UI_FuncSet, UID_but_END, NULL);
  // ED_work_END (0);
  // ED_setLnr (lNr-1);
  // WC_set_obj_stat (0);  // 0=perm
  // ED_enter();
  // WC_set_obj_stat (1);  // 1=workmode


  return 0;

}


//================================================================
  int APED_VcTra_reset (long lNr) {
//================================================================
/// APED_VcTra_reset            delete Vec's&Tra's after progLine x in DB

  int   it;
  long  ll, dbi;
  char  *p1, *p2;

  // printf("VVVVVVVVVVVVVVVVVV APED_VcTra_reset %d VVVVVVVVVVV\n",lNr);

  // scan all Lines > lNr
  L_nxt:
  p1 = UTF_GetPosLnr (&ll, lNr);
  if(p1 == NULL) goto L_done;
    // printf(" _VcTra %d |",lNr);UTX_dump_cnl(p1,50);printf("|\n");

  if((p1[0] != 'D')&&(p1[0] != 'T')&&(p1[0] != 'V')) goto L_weiter;
  p2 = strchr(p1, '=');
  if((p2 == NULL)||((p2-p1) > 12)) goto L_weiter;

  APED_dbo_oid (&it, &dbi, p1);
  DB_Delete (it, dbi);

  L_weiter:
  ++lNr;
  goto L_nxt;


  L_done:
  return 0;

}


//================================================================
  int APED_dec_defLn (char **p2, int *typ, long *ind, char *p1) {
//================================================================
/// \code
/// APED_dec_defLn             check if Line p1 is a Definitionline;
///   if yes: decode its objID, return positon of objCode
/// lines "V=NEW()" sofort auswerten, nicht via WC_Work__;
/// Output:
///   p2          position of 1 char after '='
///   typ, ind    typ and DB-index of outputvar.
///   RetCod = 0: yes, p1 is a Definitionline.
///           -1: No.
/// \endcode

// see also APED_search_defLn APED_ck_defLn APED_txo_srcLn__

  char    cTyp, *iPos;


  // printf("APED_dec_defLn  |%s|\n",p1);

  while (*p1  == ' ') ++p1;

  // skip comments
  if(*p1 == '#') return -1;

  // skip deleted lines
  if(*p1 == '_') return -1;



  if(!isalpha(*p1)) return -1;

  cTyp = toupper(*p1);
  ++p1;
  iPos = p1;

  while (isdigit(*p1)) ++p1;

  while (*p1  == ' ') ++p1;

  if(*p1 != '=') return -1;
  ++p1; // skip '='


  while (*p1  == ' ') ++p1;
  *p2 = p1;
  *typ = AP_typ_typChar (cTyp);
  *ind = atoi (iPos);

    // printf("ex APED_dec_defLn %d %d |%s|\n",*typ,*ind,*p2);

  return 0;

}


//================================================================
  int APED_ck_defLn (char **p2, char *p1) {
//================================================================
/// \code
/// check if Line cBuf is a Definitionline;
/// find 1. delimiter; it must be '='.
/// Output:
///   p2       first char following '=' (only if retCod == 0)
///   RetCod   0: yes, p1 is a Definitionline.
///           -1: No.
///
/// see also APED_search_defLn PRG_ck_defLn APED_dec_defLn
/// \endcode


  while (*p1  == ' ') ++p1;

  // skip comments
  if(*p1 == '#') return -1;

  if(!isalpha(*p1)) return -1;

  ++p1;

  while (isdigit(*p1)) ++p1;

  while (*p1  == ' ') ++p1;

  if(*p1 == '=') {*p2 = ++p1; return 0;}

  return -1;

}


//================================================================
  int APED_dec_txt (char *txLn) {
//================================================================
/// \code
/// decode textLine, add all objsID's and codes to group (oTab)
/// see IE_decode_Ln 
/// \endcode

// see APED_txo_srcLn__

  int    irc, wNr, typ;
  long   ind, dbi;
  char   cBuf[256], tc, *tPos, *endPos;


  // printf("APED_dec_txt |%s|\n",txLn);

  tPos = txLn;
  wNr = 0;

  // naechstes Wort herausloesen und den Delimiter abfragen
  L_nxt:
  ++wNr;
  tc = UTX_cp_word_t (cBuf, &tPos);
  if(cBuf[0] == '\0') return 0;
    // printf(" ---- L_nxt: |%s| %c\n",cBuf,tc);

  // wenn Delimiter '=' ist, das erste Wort skippen (definitionObject)
  if(tc == '=') {
    if(wNr == 1) goto L_nxt;
  } // else eigentlich error ?


  // wenn Delimiter  '(' ist:
  // Functiontyp feststellen (P L ... ANG );
  // ')' mit '\0' ersetzen, abarbeiten (recurse), dann restore ')'.
  if(tc == '(') {
    // endPos = pos of closing bracket
    endPos = UTX_pos_skipBrack1 (tPos);     //2008-10-16
    if(*endPos != ')') {TX_Print("APED_dec_txt E001"); return -1;}
    // replace ')'by '\0'
    *endPos = '\0';
    // recurse
    if(APED_dec_txt (tPos) < 0) return -1;
    // restore ')'
    *endPos = ')';
    tPos = endPos;
    UTX_skip_1bl (&tPos); // skip the active char and following blanks
    goto L_weiter;
  }

  // test code for object  (zB "P21")
  if((isalpha(cBuf[0]))&&(isdigit(cBuf[1]))) {
    irc = APED_dbo_oid (&typ, &dbi, cBuf);
    Grp_add__ (typ, dbi, -2L, 0);   // add obj to grp
    goto L_weiter;
  }

  // test code for subcode (ARC FSUB BSP RBSP..)
  // Skip RBSP und BSP kompl (haben dzt keine Subelemente, nur Zahlenwerte)
  irc = UTX_cmp_word_wordtab (ObjCodTab, cBuf);
  if(irc >= 0) {
    // if(irc == T_RBSP) return 0;   // BSP P20 P(0 0 0) ...
    // if(irc == T_BSP) return 0;
    goto L_weiter;
  }


  // auf rein numer testen (Typ_Val);
  // UTX_ck_num_f


  // es fehlen noch DX RX.. HIX..LOXIN OUT CW CCW

  L_weiter:
  if(tc != '\0') goto L_nxt;

    // printf("ex APED_dec_txt\n");

  return 0;

}


//================================================================
  int APED_decAdd (char *cPos) {
//================================================================
/// \code
/// decode textLine, add all objs to group (oTab)
/// Line is \n terminated !
/// \endcode

// xamples:
// C1=ARC P(-0.06019875 -2.10770416 -0.77914322) P5 Dix
// C2=CUT C1 P1 P2
// S2=CCV L1 C2 S1 C4
// A1=SRU C(ARC P(1 2 3) P12 ..
// A2=SRV L(p1 p2) ...



  int       i1, *aus_typ, aus_anz;
  char      *aus_tab;  // strings a 256 chars


  // printf("APED_decAdd |");UTX_dump_cnl(cPos, 65);printf("|\n");

  // skip commentlines
  if(cPos[0] == '#') return 0;

  // copy line --> mem_cbuf1
  UTX_cp_word_term (mem_cbuf1, cPos, '\n');

  // Line zerlegen
  APED_dec_txt (mem_cbuf1);

  return 0;

}


//=======================================================================
  int APED_search_dbLimits (long *lTab) {
//=======================================================================
/// \code
/// APED_search_dbLimits       search highest indices
/// Output:
///   lTab[0]  nr of curves   (S)
///   lTab[1]  nr of surfaces (A)
///   lTab[2]  nr of bodies   (B)
///   lTab[3]  nr of notes    (N)
///   lTab[4]  nr of circles  (C)
///   lTab[5]  nr of lines    (L)
///   lTab[6]  nr of points   (P)
///   lTab[7]  nr of mdRefs   (M)
/// see also AP_stru_2_txt (NULL,,1);
/// \endcode

  int       ind;
  char      *p1, *p2, *pEnd, typ;

  p1 = UTF_FilBuf0;
  pEnd = UTF_FilBuf0 + UTF_FilBuf0Len;


  // printf(" UTF_FilBuf0Len=%d\n",UTF_FilBuf0Len);


  for(ind=0; ind<8; ++ind) lTab[ind] = 0;


  //----------------------------------------------------------------
  L_nxt:
  while (*p1  == ' ')  { ++p1; }

  // wenn Zeile mit "#" beginnt: weiter ..
  if(*p1 == '#') goto L_weiter;
  if(*p1 == '_') goto L_weiter;



  // der erste char sollte typ;
  p2 = p1 + 1;
  // ab char 1 sollten digits sein, terminiert mit blanks bzw =
  while(isdigit(*p2)) { ++p2; }
  while (*p2  == ' ')  { ++p2; }
  if(*p2 != '=') goto L_weiter;

  // decode typ, index
  if(islower(*p1)) {
    typ = toupper(*p1);
  } else {
    typ = *p1;
  }
  ind = atoi (&p1[1]);
  // printf(" %c %d\n",typ,ind);


  switch (typ) {
    case 'S':
      if(ind > lTab[0]) lTab[0] = ind;
      break;
    case 'A':
      if(ind > lTab[1]) lTab[1] = ind;
      break;
    case 'B':
      if(ind > lTab[2]) lTab[2] = ind;
      break;
    case 'N':
      if(ind > lTab[3]) lTab[3] = ind;
      break;
    case 'L':
      if(ind > lTab[5]) lTab[5] = ind;
      break;
    case 'C':
      if(ind > lTab[4]) lTab[4] = ind;
      break;
    case 'M':
      if(ind > lTab[7]) lTab[7] = ind;
      break;
    case 'P':
      if(ind > lTab[6]) lTab[6] = ind;
      break;
  }


  // nicht gefunden, goto next line ...
  L_weiter:
  p1 = strchr(p1, '\n');
  if(p1 != NULL) {
    ++p1;
    if(p1 < pEnd) goto L_nxt;
  }


  // printf("ex APED_search_dbLimits %d %d %d %d %d %d %d %d\n",
         // lTab[0],lTab[1],lTab[2],lTab[3],
         // lTab[4],lTab[5],lTab[6],lTab[7]);

  return 0;

}


//==================================================================
  int APED_nxt_def_typ (char **lSta, int *lNr, int *lLen, int typ) {
//==================================================================
/// \code
/// Search next DefinitionLine of objectTyp
/// In:
///   lSta   NULL=start search from beginning of model (first call).
///          DO NOT MODIFY lSta,lNr,lLen for incremental search !
///   typ    typ of obj to find; eg Typ_PT
/// Out:
///   lSta   startpos of line found
///   lNr    nr of line found
///   lLen   length of line incl \n (MS-Win incl \r)
///
/// RetCodes:
///    >=0   OK; DB-Index of obj; lSta, lNr, lLen set.
///    -1    no (more) line found
/// 
/// see also:
///   AP_search_def
///   AP_search_obj / UTF_GetLinNr
///   AP_obj_delCk1 UTF_GetLinNr
///   UTF_del_rest
///   UTF_find_tx1
/// \endcode

// Problem: Fuer einen Kreis nach C# UND nach A# suchen ???


  char    *pi, *p1, *p2, *pEnd,  cTyp;


  // printf("APED_search_defLn |%s| len=%d typ=%d\n",*lSta,*lLen,typ);

  // get char for typ
  cTyp = AP_typChar_typ (typ);


  // fix startPos p1
  if(*lSta == NULL) {
    p1   = UTF_FilBuf0;
    *lNr = 1;
      // printf(" _search_defLn init\n");
  } else {
    p1   = *lSta + *lLen + term_anz;
    *lNr += 1;
  }

  pEnd  = UTF_FilBuf0 + UTF_FilBuf0Len;
    // printf(" UTF_FilBuf0Len=%d\n",UTF_FilBuf0Len);
  if(p1 >= pEnd) return -1;


  // nxt line ..
  L_nxt_ln:
    // printf("|");UTX_dump_cnl(p1,40);printf("|\n");
  *lSta = p1;

  // wenn Zeile mit "#" beginnt: weiter ..
  while (*p1 == ' ') { ++p1; }
  if(*p1 == '#') goto L_skip_ln;
  if(*p1 == '_') goto L_skip_ln;

  // check if Line starts with typ
  if(toupper(*p1) != cTyp) goto L_skip_ln;
  ++p1;
  pi = p1;

  // skip ObjNr & blanks
  while (isdigit(*p1))  { ++p1; }
  while (*p1  == ' ')  { ++p1; }

  // next char muss trennzeichen sein (blank or '=')
  if(*p1 == '=') goto L_found;


  // nicht gefunden, goto next line ...
  L_skip_ln:
  p2 = strchr(p1, '\n');
  if(p2 == NULL) return -1;
  ++p2;
  if(p2 < pEnd) {
    p1 = p2;
    *lNr += 1;
    goto L_nxt_ln;
  }
  return -1;



  // OK, eins gefunden ...
  L_found:
  p2 = strchr(p1, '\n');
  if((p2 == NULL)||(p2 >= pEnd)) p2 = pEnd;


#ifdef LNTERM_CRLF
    *lLen = p2 - *lSta - 1;
#else
    *lLen = p2 - *lSta;
#endif

  // printf("ex APED_nxt_def_typ %d %d\n",*lNr,*lLen);

  // decode DB-Index
  return atoi(pi);

  // return 0;

}


//================================================================
  int APED_find_dbo (long *dli, long *lNr, int typ, long dbi) {
//================================================================
/// \code
/// Search last DefinitionLine for DB-obj
/// Input:
///   typ,dbi    DB-object
/// Output:
///   dli   dispListIndex (-1 for Vecs, Vars)
///   lNr   Source-LineNr
/// RetCod: 0=OK; obj found;   -1=Err, obj not found
/// 
/// \endcode

// see also DL_Get_lNr_dli APED_search_defLn IE_edit_dbo UI_redo_obj__


  int      irc, lLen;
  char     *cp1, oNam[32];


  // printf("APED_find_dbo %d %ld\n",typ,dbi);


  //----------------------------------------------------------------
  // VAR & VC & Typ_Activ: get sourecLineNr in memory (no DL-record exists)
  if((typ != Typ_VAR)    &&
     (typ != Typ_VC))         goto L_DL; // 2014-12-30; Typ_Activ has DL-obj !
     // (typ != Typ_VC)     &&
     // (typ != Typ_Activ))      goto L_DL;


  // get sourecLineNr from memory
  // get objName
  APED_oid_dbo__ (oNam, typ, dbi);
    // printf("  _find_dbo typ=%d dbi=%ld oNam=|%s|\n",typ,dbi,oNam);

  // search definition-line 
  irc = APED_search_defLn (&cp1, lNr, &lLen, oNam, -1, 0);
    // printf(" irc=%d lNr=%ld |%s|\n",irc,*lNr,oNam);
  if(irc < 0) {TX_Print("APED_find_dbo E001"); return -1;}

  *dli = -1;

  goto L_exit;



  //----------------------------------------------------------------
  // get sourecLineNr via DL-record (faster)
  L_DL:

  // get dli from dbo
  *dli = DL_find_obj (typ, dbi, -1L);
  if(*dli < 0) {TX_Print("APED_find_dbo E2 %d %ld",typ,dbi); return -1;}

  // get lNr from dli
  irc = DL_Get_lNr_dli (lNr, *dli);
  if(irc < 0) {TX_Print("APED_find_dbo E003"); return -1;}



  //----------------------------------------------------------------
  L_exit:

    // printf("ex APED_find_dbo dli=%ld lNr=%ld\n",*dli,*lNr);

  return 0;

}


//================================================================
  int APED_objID_defLn (char *oid, char *defLn) {
//================================================================
/// \code
/// APED_objID_defLn           copy definitionHdr out (left of '=')
/// maxSiz of oid = 32 chars
/// \endcode

  int   i1;
  char  *p1;

  // printf("APED_objID_defLn |%s|\n",defLn);

  p1 = strchr(defLn, '=');
  if(!p1) return -1;


  i1 = p1 - defLn;
  if(i1 > 32) return -2;
  
  strncpy(oid, defLn, i1);
  oid[i1] = '\0';

    // printf("ex APED_objID_defLn |%s|\n",oid);


  return 0;

}

 
//================================================================
  int APED_search_defLn (char **lSta, long *lNr, int *lLen,
                         char *otx, long lNEnd, int src) {
//================================================================
/// \code
/// Search last DefinitionLine
/// In:
///   otx   zu suchendes Obj; zB "L20"
///   lNEnd ab (incl) dieser Zeile nicht mehr suchen. -1: bis zum Ende.
///   src   0=UTF_FilBuf0; 1=UTF_FilBuf1
/// Out:
///  lSta   Posi, wo spaeter weitergesucht wird ..
///           ist Pointer auf den beginn der gefundenen Zeile ..
///  lNr    gefundene zeilenNummer; erste Zeile == 1.
///  lLen   Laenge der Zeile incl \n (und bei MS-Win \r)
/// RetCod:
///  -1     Error, def.line does not exist.
///   0     OK, def.line found
///   1     OK, def.line found, but without ending '\n'
/// 
/// see also:
///   AP_search_def
///   AP_search_obj / UTF_GetLinNr
///   APED_nxt_def_typ
///   AP_obj_delCk1 UTF_GetLinNr 
///   UTF_del_rest
///   UTF_find_tx1
/// \endcode

// Problem: Fuer einen Kreis nach C# UND nach A# suchen ???
// search in whole memory ..


  int  ltx, irc;
  long l1;
  char *p1, *p2, *pEnd;


  // printf("APED_search_defLn |%s| %ld\n",otx,lNEnd);

  ltx = strlen(otx);

  if(src == 0) {
    // p1 = UTF_FilBuf0;
    // pEnd = UTF_FilBuf0 + UTF_FilBuf0Len;
    UTF_GetPos2 (&p1, &pEnd);

  } else {
    // p1 = UTF_FilBuf1;
    // pEnd = UTF_FilBuf1 + UTF_FilBuf1Len;
    UTF_Buf1_GetPos2 (&p1, &pEnd);
  }

  *lSta = NULL;
  l1 = 0;
  if(lNEnd < 1) lNEnd = UT_INT_MAX;
  // printf(" lNEnd=%d\n",lNEnd);

  
  //----------------------------------------------------------------
  // skip blanks
  L_nxt:
  ++l1;
  if(l1 >= lNEnd) goto L_fertig;
  p2 = p1;
  while (*p1  == ' ')  { ++p1; }


  // wenn Zeile mit "#" beginnt: weiter ..
  if(*p1 == '#') goto L_weiter;
  // if(*p1 == '_') goto L_weiter;  - could not search for deleted objects !


    // printf("L_nxt: %ld |",l1); UTX_dump_cnl (p1, 16); printf("|\n");


  // check if Line starts with otx
  // check first character - ignore case
  if(UTX_ck_caseChr(p1,otx) > 0) goto L_weiter;
  // check all following chars (should be numeric ..
  if(strncmp(&p1[1], &otx[1], ltx-1)) goto L_weiter;
  p1 += ltx;


  // next char muss trennzeichen sein (blank or '=')
  while (*p1  == ' ')  { ++p1; }
  if(*p1 == '=') {       // OK, eins gefunden ...
    *lNr  = l1;
    *lSta = p2;
  }


  //----------------------------------------------------------------
  // nicht gefunden, goto next line ...
  L_weiter:
  p1 = strchr(p1, '\n');
  if(p1 != NULL) {
    ++p1;
    if(p1 < pEnd) goto L_nxt;
  }


  L_fertig:
  irc = 0;
  if(*lSta) {
    p1 = strchr(*lSta, '\n');
    if(p1 == NULL) { irc = 1; p1 = pEnd; }        // 2013-09-24

#ifdef LNTERM_CRLF
    *lLen = p1 - *lSta - 1;
#else
    *lLen = p1 - *lSta;
#endif

  } else { 
      // printf("ex APED_search_defLn -1 |%s|\n",otx);
    return -1;
  }


    // printf("ex APED_search_defLn 0 |%s| %d %d\n",otx,*lNr,*lLen);

  return irc;

}


//============================================================================
  int APED_find_depLn_nxt (char **lSta,long *lNr,long *lLen,char *otx) {
//============================================================================
/// \code
/// Search next dependant Line;
/// lNr mit Startpos *lSta is a Definitionline (z.B. "L22=...")
/// search begins in next Line !
/// Input:
///   lNr   LineNr of Definitionline (where to start search ..)
///   lSta  Startposition of lNr; NULL == not known.
///   otx   what to search
/// Output:
///   lNr   LineNr of found depLn
///   lSta  Startposition of found depLn
///   lLen  length of found depLn
/// 
/// RetCod:  0   Ok, found ..
///         -1   no depending line exists for obj otx
/// \endcode

// Problem: Fuer einen Kreis nach C# UND nach A# suchen ???



  int    otxLen, irc;
  long   l1, ll, ipos;
  char   *p1, *p2, *p3, *p4, *pEnd;


  // printf("APED_find_depLn_nxt |%s| %ld \n",otx,*lNr);


  // get position & lenght of line <lNr>
  p1 = UTF_GetPosLnr (&l1, *lNr);
  *lLen = l1;
  if(p1 == NULL) {
    return -1;
  }


  // get position of next line
  p1 = UTF_GetnextLnPos (p1);
  if(p1 == NULL) {
    return -1;
  }

  l1 = *lNr;


  pEnd   = UTF_FilBuf0 + UTF_FilBuf0Len;
  *lSta  = NULL;


  // search starts at pos=p1 lNr=l1
  // skip blanks
  L_nxt:
  ++l1;
  p2 = p1;  // save linestartposition


  // zeilenende suchen
  p3 = strchr(p2, '\n');
  if(p3 == NULL) p3 = pEnd;


  // search depending objs
  ll = p3 - p2;
  irc = APED_search_objID (p2, ll, otx);
    // printf(" _search_objID %d |%s|\n",irc,otx);
  if(irc >= 0) {
    *lNr  = l1;
    *lSta = p1;
    goto L_fertig;
  }



  L_weiter:
  ++p3;
  p1 = p3;
  if(p3 < pEnd) goto L_nxt;



  L_fertig:
  if(*lSta) {
    p1 = strchr(*lSta, '\n');
    if(p1 == NULL) p1 = pEnd;

#ifdef LNTERM_CRLF
    *lLen = p1 - *lSta - 1;
#else
    *lLen = p1 - *lSta;
#endif

  } else {
    return -1;
  }

    // printf("ex APED_find_depLn_nxt %d %d\n",*lNr,*lLen);

  return 0;

}


//============================================================================
  int APED_find_depLn_last (char **lSta,long *lNr,long *lLen,char *otx) {
//============================================================================
/// \code
/// Search last dependant Line;
/// lNr mit Startpos *lSta ist eine Definitionline (z.B. "L22=...")
/// search begins in next Line !
/// Input:
///   lNr   LineNr of Definitionline (where to start search ..)
///   lSta  Startposition of lNr; NULL == not known.
///   otx   what to search
/// Output:
///   lNr   LineNr of found depLn
///   lSta  Startposition of found depLn
///   lLen  length of found depLn
/// 
/// RetCod:  0   Ok, found ..
///         -1   es existiert keine depending line zu obj otx
/// \endcode

// Problem: Fuer einen Kreis nach C# UND nach A# suchen ???



  int    otxLen, irc;
  long   l1, ipos;
  char   *p1, *p2, *p3, *p4, *pEnd;


  // printf("APED_find_depLn_last |%s| %d |%s|\n",otx,*lNr,*lSta);

  p1 = UTF_GetPosLnr (&l1, *lNr);  // get position & lentght of line <lNr>
  *lLen = l1;
  if(p1 == NULL) {
    return -1;
  }

  p1 = UTF_GetnextLnPos (p1);
  if(p1 == NULL) {
    return -1;
  }

  l1 = *lNr;

/*
  if(*lSta == NULL) {  // vom Beginn weg suchen
    p1 = UTF_FilBuf0;
    l1 = 1;

  } else {
    p1 = *lSta;
    l1 = *lNr;
    // zeilenende suchen
    p1 = strchr(p1, '\n');
    if(p1 == NULL) {
      *lNr += 1;
      return -1;
    }
    ++p1;
  }
*/
  pEnd   = UTF_FilBuf0 + UTF_FilBuf0Len;
  *lSta  = NULL;
  otxLen = strlen(otx);




  // skip blanks
  L_nxt:
  ++l1;
  p2 = p1;  // save linestartposition
  while (*p2  == ' ')  { ++p2; }


  // zeilenende suchen
  p3 = strchr(p2, '\n');
  if(p3 == NULL) p3 = pEnd;


  // wenn Zeile mit "#" beginnt: weiter ..
  if(*p2 == '#') goto L_weiter;
  if(*p2 == '_') goto L_weiter;


  // do not search in nameField ("P1=0,0,0 # Origin");
  //   set p4=startPos of Info
  p4 = APED_onam_search (p2, p3);
  if(p4 == NULL) p4 = p3;

  // search otx between p2, p3
  p2 = UTX_memstr (p2, p4, otx);
  if(p2) {       // OK, eins gefunden ...
    // das nachfolgende Zeichen MUSS ein Trennzeichen sein
    if(isdigit(p2[otxLen])) goto L_weiter;
    // printf(" found |%s|",otx);UTX_dump_cnl (p1);printf("|\n");
    // printf(" deli=|%c|\n",p2[otxLen]);
    *lNr  = l1;
    *lSta = p1;
  }



  L_weiter:
  ++p3;
  p1 = p3;
  if(p3 < pEnd) goto L_nxt;



  L_fertig:
  if(*lSta) {
    p1 = strchr(*lSta, '\n');
    if(p1 == NULL) p1 = pEnd;

#ifdef LNTERM_CRLF
    *lLen = p1 - *lSta - 1;
#else
    *lLen = p1 - *lSta;
#endif

  } else {
    return -1;
  }

  // printf("ex APED_find_depLn_last %d %d\n",*lNr,*lLen);

  return 0;

}


//================================================================
  int APED_find_dep__ (MemTab(ObjSRC) *oa, int typ, long dbi) {
//================================================================
/// \code
/// APED_find_dep__            find all depending sourceObjs (childs)
///  Example:
/// #include "../ut/ut_memTab.h"           // MemTab
/// MemTab(ObjSRC) delTab = _MEMTAB_NUL;
/// APED_find_dep__ (&delTab, typ, dbi);
///   printf(" nr of dep.objs = %d\n",delTab.rNr);
/// MemTab_free (&delTab);
/// \endcode


  int        irc;
  long       ld, dli, lnr;
  ObjSRC     o1;


  // printf("APED_find_dep__ typ=%d dbi=%ld\n",typ,dbi);


  MemTab_ini (oa, sizeof(ObjSRC), Typ_ObjSRC, 400);


  // Search dli & last DefinitionLine for root-obj typ,dbi
  irc = APED_find_dbo (&dli, &lnr, typ, dbi);
  if(irc < 0) return irc;     // ?
    // printf(" _find_dbo dli=%d lnr=%d\n",dli,lnr);


  // add primary object
  o1.typ  = typ;
  o1.dbi  = dbi;
  o1.dli  = dli;
  o1.lnr  = lnr;
  o1.iPar = -1;
  MemTab_sav (oa, &ld, &o1, 1);


  // search & add all depending objects
  APED_find_dep_nxt (oa, typ, dbi, lnr, 0);

    // MemTab_dump (oa, "ex-_find_dep__"); // Testoutput only:


  return 0;

}


//============================================================================
  int APED_find_dep_nxt (MemTab(ObjSRC) *oa,
                         int typ, long dbi, long lnr, int iPar) {
//============================================================================
/// \code
/// APED_find_dep_nxt          search & add all depending objects
/// Del_find       search & add all depending objects
/// see also AP_SMD_nxt Del_find
/// \endcode
// TODO:
// replace Del_find


  int       irc, iChd, chdTyp, chdPar;
  long      ld, chdDli, chdLnr, l1, lLen, chdDbi;
  char      *cPos, oNam[32];
  ObjSRC    o1;


  // printf("APED_find_dep_nxt typ=%d dbi=%ld lnr=%ld iPar=%d\n",
             // typ,dbi,lnr,iPar);


  // get name
  APED_oid_dbo__ (oNam, typ, dbi);

  cPos = NULL;
  chdLnr = lnr;


  L_nxt:
  // Search next depending Line; obj=oNam, starting lineNr = chdLnr
  irc = APED_find_depLn_nxt (&cPos, &chdLnr, &lLen, oNam);
    // printf(" _depLn_nxt irc=%d chdLnr=%d |",irc,chdLnr);
    // if(cPos) UTX_dump_cnl (cPos, 40);
    // printf("\n");
  if(irc < 0) goto L_find_R;   // es gibt keine depending line mehr ..


  // extract typ, dbi, dli obj from cPos & lNr
  // get typ, dbi from cPos
  APED_dbo_oid (&chdTyp, &chdDbi, cPos);
    // printf(" chdTyp=%d chdDbi=%ld\n",chdTyp,chdDbi);



  // test if obj is already in list oa
  irc = APED_find_depLn_ck (oa, &chdTyp, &chdDbi); // 0=new, 1=already exists
    // printf(" ex _depLn_ck %d\n",irc);
  if(irc) goto L_nxt;


  // get dli from typ/dbi
  // chdDli = DL_find_obj (chdTyp, chdDbi, -1L);
  irc = APED_find_dbo (&chdDli, &l1, chdTyp, chdDbi);
    // printf("  _find_dbo %d dli=%ld lNr=%ld\n",irc,chdDli,l1);

  if(irc < 0) goto L_exit;

  // add found obj to oa
  o1.typ  = chdTyp;
  o1.dbi  = chdDbi;
  o1.dli  = chdDli;
  o1.lnr  = chdLnr;
  o1.iPar = iPar;
  chdPar = oa->rNr;                // indicate parent
  MemTab_sav (oa, &ld, &o1, 1);


  // test if new found obj has depending lines (recurse)
  APED_find_dep_nxt (oa, chdTyp, chdDbi, chdLnr, chdPar);


  // get next ..
  goto L_nxt;


  //----------------------------------------------------------------
  L_find_R:
  // if obj to delete a plane ?
  if(typ != Typ_PLN) goto L_exit;

  irc = APED_find_depRef (oa, dbi, lnr, iPar);


  //----------------------------------------------------------------
  L_exit:
    // printf("ex APED_find_dep_nxt %d\n",irc);
    // MemTab_dump (oa);


  return irc;

}


//=========================================================================
  int APED_find_depRef (MemTab(ObjSRC) *oa, long dbi, long lnr, int iPar) {
//=========================================================================
// delete all lines from activating as constrPlane until next constrPlane
// eg:  delete also R20 & p20= !
// R20=DX 12
// R20
// p20=..
// R0

  int       i1, iStat = 0, chdTyp, chdPar;
  long      lLen, chdDbi, ld;
  char      *p1, *p2, *actPos, *endPos, *posEof;
  ObjSRC    o1;


  // printf("APED_find_depRef: dbi=%d lnr=%d iPar=%d\n",dbi,lnr,iPar);
  // printf("  oa->rNr=%d\n",oa->rNr);

  ++lnr;    // plane already deleted ..


  o1.typ  = 0;
  o1.dbi  = 0;
  o1.iPar = iPar;                // indicate parent

  // get startPosition
  if(lnr < 1) lnr = 1;
  actPos = UTF_GetPosLnr (&lLen, lnr);
  if(!actPos) goto L_exit;

  endPos = actPos + lLen;
  UTF_GetPos2 (&p1, &posEof);
    // printf(" mem=|%s|\n",actPos);

  goto L_test;


  // get next line    (see UTF_GetnextLnPos)
  L_nxt_ln:
    // ++actPos;   // skip last \n
    actPos = endPos + 1;
      // printf(" nxtLn1: |");UTX_dump_cnl(actPos,60);printf("|\n");
    if(actPos >= posEof) goto L_exit;
    ++lnr;
    endPos = strchr(actPos, '\n');
    if(!endPos) endPos = posEof;
    // lLen = endPos - actPos;

    chdDbi = 0;


  //----------------------------------------------------------------
  L_test:
      // printf(" nxtLn2: |");UTX_dump_cnl(actPos,60);printf("|\n");
    p1 = actPos;
    while (*p1 == ' ')  { ++p1; }
    // get typ
    chdTyp = AP_typ_typChar (*p1);
    // test if this is a refSys-def
    if(chdTyp == Typ_Error) {chdTyp = Typ_cmdNCsub; chdDbi = 0; goto L_cont;}
    ++p1;
    if(!isdigit(*p1)) {chdTyp = Typ_cmdNCsub; chdDbi = 0; goto L_cont;}
    chdDbi = strtol (p1, &p2, 10);
      // printf("   chdTyp=%d chdDbi=%d p2=|",chdTyp,chdDbi);
        // UTX_dump_cnl(p2,60);printf("|\n");


    while (*p2 == ' ')  { ++p2; }
    if(*p2 == '=')  goto L_cont;
    // this is no definition-line, so no typ:
    if(chdTyp != Typ_PLN) {chdTyp = Typ_cmdNCsub; goto L_cont;}
    chdTyp = Typ_cmdNCsub;
    // activate refSys ?
      // printf(" p2=%d\n",*p2);
    if((*p2 == '\n')||(*p2 == '#')||(*p2 == '\0')) goto L_is_rs;
    goto L_cont;

      // found refSys-line
      L_is_rs:
          // printf(" refSys=%d\n",chdDbi);
        if(chdDbi != dbi) { iStat = 0; goto L_cont; }     // disactivate
        // delete all following lines ..
        iStat = 1;  // delete this line & all following lines ..

      L_cont:
      if(iStat) {
          // printf(" del lnr=%d |",lnr);UTX_dump_cnl(actPos,60);printf("|\n");
        o1.typ  = chdTyp;
        o1.dbi  = chdDbi;
        o1.lnr  = lnr;
        chdPar = oa->rNr;
        // delete line ..
        MemTab_sav (oa, &ld, &o1, 1);
        // test if this is a definition line
        if(chdTyp == Typ_cmdNCsub) goto L_nxt_ln;
        // search & add all depending objects
        APED_find_dep_nxt (oa, chdTyp, chdDbi, lnr, chdPar);
      }
      goto L_nxt_ln;  // OR


  //----------------------------------------------------------------
  L_exit:
  return 0;
}


//================================================================
  int APED_find_depLn_ck (MemTab(ObjSRC) *oa, int *typ, long *dbi) {
//================================================================
// test if obj is already in list oa
// RetCod: 0=new, 1=already exists

  int     i1;

  // printf("APED_find_depLn_ck %d %d rNr=%d\n",*typ,*dbi,oa->rNr);

  for(i1=0; i1 < oa->rNr; ++i1) {
      // printf(" oa[%d] %d %d\n",i1,oa->data[i1].typ,oa->data[i1].dbi);
    if(oa->data[i1].typ != *typ) continue;
    if(oa->data[i1].dbi != *dbi) continue;
      // printf(" ex APED_find_depLn_ck irc=1\n");
    return 1;
  }

    // printf(" ex APED_find_depLn_ck irc=0\n");
  return 0;

}


//===================================================================
 int APED_pt_find (char **cpPt, int *sLen, char **cpNxt, char *cps) {
//===================================================================
// find position of next point in string
// Input:
//   cps          string; startpos of obj; start here to find next point ...
// Output:
//   cpPt         startposition ('P') of first point-string in cps
//   sLen         length of point-string (including 'P')
//   cpNxt        startposition of next obj in cps
//   RetCod       -1  no more points in src
//                 0  explicit point found (eg "P12")
//                 1  implicit point found (eg "P(10 0 0)")
// Example:
// cp1 = cbuf;
// APED_pt_find (&cp2, &i1, cp1); cp1 = cp2 + i1;

  int   irc;
  char  *cp2, s1[80];


  // printf("APED_pt_find |%s|\n",cps);


  L_start:
  *cpPt = cps;
  if(*cps == 'P') {++cps; goto L_pt_imp;}

  L_nxt:
  cps = APED_nxt_obj (cps);
  if(cps == NULL) return -1;
  goto L_start;
 

  //----------------------------------------------------------------
  // P(..)
  L_pt_imp:
    if(*cps != '(') goto L_pt_exp;
    ++cps;
    cps = UTX_pos_skipBrack1 (cps); // find corresponding ')'
    if(*cps != ')') return -1;
    ++cps;  // skip ')'
    *sLen = cps - *cpPt;
    irc = 1;
    goto L_find_nxt;   // skip deli


  //----------------------------------------------------------------
  // P#
  L_pt_exp:
    if(!isdigit(*cps)) goto L_nxt;    // not numeric: skip this 'P'
    *sLen = UTX_ck_num_digNr (&cp2, cps); // find nr of consecutive digits
    *sLen += 1;                       // cp2 = first non-digit
    cps = *cpPt + *sLen;
    irc = 0;



  //----------------------------------------------------------------
  // find position of next obj
  L_find_nxt:
    *cpNxt = APED_nxt_obj (cps);
    // cp2 = UTX_pos_del_next (cps, cPos);


  //----------------------------------------------------------------
  L_exit:
  // printf("ex APED_pt_find %d pt=|%s| l=%d nxt=|%s|\n",irc,*cpPt,*sLen,*cpNxt);

  return irc;

}


//================================================================
  char* APED_nxt_obj (char *cps) {
//================================================================
/// \code
/// find next obj in string; skip brackets.
///
/// InPut:
///   cps      startpos (previous obj)
/// OutPut:
///   RetCod   startpos of next obj or NULL (end of string)
///
/// if it is '(' then skip brackets;
/// if its ' ' or ',' then skip this char; nxt obj found.
///
/// \endcode


// see also UTX_pos_del UTX_pos_del_next APT_cp_ausd
// see also UTX_pos_skipWord

  char    *cp1;

  L_nxt:
    if(*cps == '\0') return NULL;


    // find next delimiter
    cp1 = strpbrk (cps, " ,(#");                     //" \"'()*+,-/;<=>"
    if(cp1 == NULL) return NULL;


    // skip bracket
    if(*cp1 == '(') {
      ++cp1;
      cp1 = UTX_pos_skipBrack1 (cp1);
      if(*cp1 != ')') {printf("APED_nxt_obj E001\n"); return NULL;}
      cps = cp1 + 1;    // skip ')'
      goto L_nxt;
    }


    // skip deli
    cps = cp1 + 1;    // skip ')'
    // skip blanks
    UTX_pos_skipLeadBlk (cps);


    // printf("ex APED_nxt_obj |%s|\n",cps);

  return cps;

}


//================================================================
 int APED_pt_chg (int *lsn, long ipt, char *cps, int lso) {
//================================================================
// change point from dbi in string
// Input:
//   ipt        DB-index of new point
//   lso        length of old pointstring (to be replaced)
// Output:
//   lsn        new length of pointstring

  long  l1, ss;
  char  s1[80];
  Point *pt1;


  // printf("APED_pt_chge ipt=%ld cps=|%s| lso=%d\n",ipt,cps,lso);

  // if(ipt >= 0) {
    // sprintf(s1, "P%d", ipt);

  // } else {   // dynamic point
    pt1 = DB_get_PT (ipt);
      // UT3D_stru_dump (Typ_PT, pt1, "DB-pt[%d]",ipt);
    // see also AP_obj_add_pt
    strcpy(s1, "P(");
    AP_obj_add_nval (s1, 3, (double*)pt1);
    strcat(s1, ")");
  // }

  // replace; at pos cp2
  *lsn = strlen(s1);
  ss = strlen(cps);
    // printf(" vor replace |%s|%s|\n",cps,s1);
  // UTX_chg_str1 (&l1, cps, s1, cps, ls);
  MEM_chg_rec (cps, &ss, s1, *lsn, cps, lso);
    // printf(" nach replace |%s|\n",cps);


  // printf("ex APED_pt_chg |%s| l=%d\n",cps,*lsn);

  return 0;

}


//============================================================================
  int APED_find_par_nxt (MemTab(ObjSRC) *oa,
                         int typ, long dbi, long lnr, int iPar) {
//============================================================================
/// \code
/// APED_find_par_nxt          search & add all parent objects
/// find parents of obj <typ,dbi>; its definition-lineNr is <lnr>.
/// RetCod:   -1 Error (lnr does not exist)
///
/// see also APED_find_dep_nxt AP_SMD_nxt Del_find
/// iPar: -1=primary_obj; else index of parent into oa.
/// \endcode

// find all parents:
//   1) get next parent-obj in codepart of line <lnr>; add obj to parTab oa.
//   2) find the definitionline for the obj; goto 1) (recurse)
//
// TODO: check only all lines before the definition-line


  int       ii, irc;
  long      dli, ld, l1, ll;
  char      *p1, *p2, *p3;
  ObjSRC    o1;


  // printf("APED_find_par_nxt typ=%d dbi=%ld lnr=%ld iPar=%d\n",
                               // typ,dbi,lnr,iPar);


//   get all objs in codepart of the definitionLine; add them to oa.

  // get startpos of line <lnr>
  p1 = UTF_GetPosLnr (&ll, lnr);
  if(p1 == NULL) return -1;
     // printf(" find_par_nxt:\n");UTX_dump_cnl (p1, 60); printf("\n");


  // skip definitionHeader
  p2 = strchr(p1, '=');
  if(!p2) {TX_Print("APED_find_par_nxt E001"); return -2;}
  ++p2;   // skip "="
  ll -= p2 - p1;  // - def.Name


  // skip objName
  p3 = APED_onam_search (p2, &p2[ll]);
  if(p3) ll = p3 - p2 - 2;   // -2: remove also " #"


  //----------------------------------------------------------------
  // find all objects in codepart of line lnr; add tem to oa
  L_1:
    // get next parentobj into <typ,dbi>
    irc = APED_oid_src1 (&typ, &dbi, &p2, &ll);
      // printf(" irc=%d  typ=%d dbi=%ld ll=%ld\n",irc,typ,dbi,ll);
    if(irc < 0) goto L_9;  // line finished; exit.


    // test if obj is already in list oa
    ii = OSRC_find_odb (oa, &typ, &dbi); // -1 not, else index
      // printf(" ex _depLn_ck %d\n",irc);
    if(ii >= 0) goto L_1;


    // find the definitionline for <typ,dbi>
    irc = APED_find_dbo (&dli, &lnr, typ, dbi);
    if(irc < 0) return irc;     // ?
      // printf(" _find_dbo dli=%ld lnr=%ld\n",dli,lnr);


    // add this obj to oa
    OSRC_add__ (oa, &typ, &dbi, &dli, &lnr, &iPar);


    // add all its parents to oa (recurse)
    ii = oa->rNr - 1;  // index of last oa-record
    APED_find_par_nxt (oa, typ, dbi, lnr, ii);


    goto L_1;


  //----------------------------------------------------------------
  L_9:

  return 0;

}


//================================================================
  int APED_find_par__ (MemTab(ObjSRC) *oa, int typ, long dbi) {
//================================================================
/// \code
/// APED_find_par__            find all parent-Objs
/// codeline:   "<defObj> = <parent-objects>,<functions>"
///
/// see also APED_find_dep__
///
///  Example:
/// #include "../ut/ut_memTab.h"           // MemTab
/// MemTab(ObjSRC) parTab = _MEMTAB_NUL;
/// APED_find_par__ (&parTab, typ, dbi);
/// MemTab_dump (&parTab);
/// MemTab_free (&parTab);
/// \endcode


  int        irc;
  long       l1=1L, dli, lnr;
  ObjSRC     o1;


  // printf("APED_find_par__ typ=%d dbi=%ld\n",typ,dbi);


  MemTab_ini (oa, sizeof(ObjSRC), Typ_ObjSRC, 400);


  // Search dli & last DefinitionLine for root-obj typ,dbi
  irc = APED_find_dbo (&dli, &lnr, typ, dbi);
  if(irc < 0) return irc;     // ?
    // printf(" _find_dbo dli=%ld lnr=%ld\n",dli,lnr);


  // add primary object
  OSRC_add__ (oa, &typ, &dbi, &dli, &lnr, &l1);


  // search & add all parent objects
  APED_find_par_nxt (oa, typ, dbi, lnr, 0);

    // MemTab_dump (oa);

  return 0;

}


//================================================================
  int OSRC_add__ (MemTab(ObjSRC) *oa, int *typ, long *dbi, long *dli,
                  long *lnr, int *iPar) {
//================================================================
/// \code
/// OSRC_add__                 add record to ObjSRC-table
/// Retcod: 0 = OK; -1 = outOfMemory-Error.
/// \endcode


  long      ld;
  ObjSRC    o1;


  // printf("OSRC_add__ typ=%d dbi=%ld lnr=%ld\n",*typ,*dbi,*lnr);

  o1.typ  = *typ;
  o1.dbi  = *dbi;
  o1.dli  = *dli;
  o1.lnr  = *lnr;
  o1.iPar = *iPar;

  return MemTab_sav (oa, &ld, &o1, 1);

}


//================================================================
  int OSRC_find_odb (MemTab(ObjSRC) *oa, int *typ, long *dbi) {
//================================================================
/// \code
/// OSRC_find_odb              test if db-obj is already in ObjSRC-table
/// RetCod: -1 not else index.
/// \endcode

  int     i1;

  // printf("OSRC_find_odb %d %d rNr=%d\n",*typ,*dbi,oa->rNr);

  for(i1=0; i1 < oa->rNr; ++i1) {
      // printf(" oa[%d] %d %d\n",i1,oa->data[i1].typ,oa->data[i1].dbi);
    if(oa->data[i1].typ != *typ) continue;
    if(oa->data[i1].dbi != *dbi) continue;
      // printf(" ex OSRC_find_odb %d\n",i1);
    return i1;
  }

    // printf(" ex APED_find_depLn_ck -1\n");
  return -1;

}


//================================================================
  int APED_txo_srcLn__ (ObjTXTSRC *tso, int itsMax, char *sln) {
//================================================================
/// \code
/// analyze sourceline sln; get source-objects.
/// analyze definition-sourceline; get typ,form,level.
///
/// Input:
///   sln        definition-sourcline, without def.Hdr, with name, 0-terminated.
///   itsMax     max nr of records in tso
/// Output:
///   tso        a list of all objects in sln
///   RetCod     nr of tso-records, -1=Err
///
/// tso.type      tso.form
/// Typ_NumString Typ_NumString num.value (eg  12 -23 .8)
/// Typ_ObjDB     Typ_ObjDB     (eg "V20" or "P12")
/// Typ_ConstVal  <codeOffset>  NcoTxtTab (num.constant: eg "PI")
/// Typ_ConstOG   <codeOffset>  GcoTxtTab (geom.constants: eg "DX")
/// Typ_FncNam    <fnc>         fnc: Fc1TxtTab Fc1TypTab FcmTxtTab Typ_Val
/// Typ_cmdNCsub  <codeOffset>  Function fnc: ObjCodTab ("CUT","REV" ..)
/// TYP_OpmPlus   <codeOffset>  MOpTxtStr (math.operator: + - / *)
/// Typ_String    Typ_String    string (eg "Submodel1")
/// Typ_Name      Typ_Name      (the objName eg "# info")
/// TYP_FuncEnd   -             end of list
///
/// tso.ilen   length in chars of obj
/// tso.ipar   index to parent (into tso); -1=primary level.
/// tso.ioff   offset in chars from start of codestring.
///
/// see also SRCU_analyz__
/// \endcode

// definition-sourcelines do NOT contain:
//   controlCodes     (IF JUMP DLG DEBUG INTERN EXIT ..)  PRI .. (PrgCmdTab)
//   controlOperators (EQ NE LT GT G_E L_E)  Typ_ope_eq

// TODO: cannot analyze dx (must be "DX")


  int     its, ii, i1, iLev, levTab[10], exprNr[10], fncAct, fncValNr;
  char    *cp1, *cp2, s1[64], *cpe;



  // printf("APED_txo_srcLn__ ,itsMax=%d\n",itsMax);
  // printf("  _txo_src |%s|\n",sln);


  its = 0;
  iLev = 0;           // active index into levTab
  levTab[0] = -1;     // parent-index-table
  exprNr[0] = 0;      // first,second or third value in bracket
  fncAct = 0;


  // find startpos of next expression;
  // see also UTX_get_word UTX_wTab_srcLn
  cp1 = sln;


  // get cpe = endPos of expression; '\0' or '\n'
  ii = strcspn(cp1, "\n");
  cpe = &sln[ii];



  //----------------------------------------------------------------
  L_nextExpr:
    tso[its].ipar = levTab[iLev];  // toplevel = -1
    while(*cp1 == ' ') ++cp1;      // skip leading blanks
    if(*cp1 == ',') {
      ++cp1;                       // skip one ','
      while(*cp1 == ' ') ++cp1;    // skip following blanks
    }
    if(cp1 >= cpe) {
      tso[its].typ  = TYP_FuncEnd;
      return its;
    }
    exprNr[iLev] += 1;
      // printf("L_nextExpr:its=%d cp1=|",its);UTX_dump_cnl(cp1,50);printf("|\n");


  //----------------------------------------------------------------
  // test delim. for '('
  if(*cp1 == '(') {
    // test if last record is a function; else add record Typ_FncNam,Typ_Val
    if(its < 1) goto L_fnc1;
    if(tso[its-1].typ != Typ_FncNam) goto L_fnc1;
    ++iLev;
    if(iLev >= 10) goto L_errLev;
    levTab[iLev] = its - 1;
    exprNr[iLev] = 0;
    ++cp1; // skip '('
    goto L_nextExpr;
  }


  //----------------------------------------------------------------
  // test delim. for ')'
  if(*cp1 == ')') {
    --iLev;
    if(iLev < 0) goto L_errLev;
    ++cp1; // skip ')'
    fncAct = 0;
    goto L_nextExpr;
  }


  //----------------------------------------------------------------
  // test delim. for objName (eg ' # sum')
  if(*cp1 == '#') {
    // rest of line is comment !
    tso[its].typ  = Typ_Name;
    tso[its].form = Typ_Name;
    cp2 = cpe;
    goto L_saveExpr;
  }



  //----------------------------------------------------------------
  // test delim. for string (eg '"Submodel1"')
  if(*cp1 != '\"') goto L_1;
      // printf(" handle string\n");
    cp2 = UTX_pos_skipStr (cp1);  // get pos of closing \"
      // printf(" _skipStr |%c|\n",*cp2);
    if(*cp2) {
      tso[its].typ  = Typ_String;
      tso[its].form = Typ_String;
    } else { // no closing \"
      goto L_err1;
    }
    ++cp2;  // skip "
    goto L_saveExpr;


  //----------------------------------------------------------------
  L_1:  // test for  math.oper     '+ - / *'
    if(cp1 <= sln) goto L_1_1;
    cp2 = strchr (MOpTxtStr, *cp1);
    if(cp2) {
      // its a math.oper ( + - / *).
      ii = cp2 - MOpTxtStr;    // printf(" ii=%d\n",ii);
      if(ii >= 2) goto L_1_sav; // '/' and '*' is always operator (not sign).
      // Check if its a sign or a math.oper.
      // if nxt char is blank, comma or '(' its a operator.
      if(strchr(" ,(", *(cp1+1))) goto L_1_sav;
      // last char must not be a delimiter (blank or comma) 
      // math.oper: "V10+1" "V10 + 1" "(3*3)-4"
      // sign: "V10 +1"
      // test if follow.char is digit; if yes: previous may not be delimiter.
      if(isdigit(*(cp1+1))) {
        // is eg "+1"; Its a sign if previous char is a blank or comma.
        if(strchr(" ,(", *(cp1-1))) goto L_1_1;
        // its eg "V10+1" ..
      }
      L_1_sav:
        tso[its].typ  = TYP_OpmPlus;
        tso[its].form = ii;
        cp2 = cp1 + 1;
        goto L_saveExpr;
    }


  L_1_1:
    // test for numVal; 12 -23 .8
    // see UTX_ck_num_i
    ii = UTX_ck_num_f (&cp2, cp1);
    if(ii < 0) goto L_2;
/*
      if((fncAct == Typ_VC)||(fncAct == Typ_PT)) {
           // printf(" _txo_src iLev=%d exprNr[iLev]=%d\n",iLev,exprNr[iLev]);
         if(exprNr[iLev] == 1)      i1 = Typ_ValX;
         else if(exprNr[iLev] == 2) i1 = Typ_ValY;
         else                       i1 = Typ_ValZ;
      } else {
*/
        i1 = Typ_NumString;
      // }
      tso[its].typ  = i1;
      tso[its].form = Typ_NumString;
      goto L_saveExpr;


  //----------------------------------------------------------------
  // 1. char is not digit. Test if its a objID (2. char numeric)
  L_2:
    cp2 = cp1;
    ++cp2;
    if(!isdigit(*cp2)) goto L_3;
      // yes, DB-obj
      ii = UTX_ck_num_digNr (&cp2, cp2);
      tso[its].typ  = Typ_ObjDB;
      tso[its].form = Typ_ObjDB;
      goto L_saveExpr;


  //----------------------------------------------------------------
  // 1. & 2. chars not digits. Isolate first word.
  L_3:
    // find cp2 = pos. of next delimiter (" (,\0)
    ii = strcspn (cp2, " (,)#+-/*\n");
    cp2 = &cp2[ii];
      // printf(" L_3:ii=%d deli=|%c|\n",ii,*cp2);

    // cp1=start of word; cp2=delimiter
    ii = cp2 - cp1;

    UTX_pos_skipLeadBlk (cp2);  // skip leading blanks
      // printf(" len ii: %d cp1 |",ii);UTX_dump_c__(cp1,ii);printf("|\n");



    // test for function: is delimiter "(" or " (" ?
    if(*cp2 != '(') {
      cp2 = cp1 + ii;  // without del.
      goto L_4;
    }


  //----------------------------------------------------------------
    // s1 is word. Test for known functions.
    // AP_typ_FncNam Fc1TxtTab Fc1TypTab FcmTxtTab
    i1 = AP_typ_FncNam (cp1, ii);
    if(i1 < 0) {TX_Error("APED_txo_srcLn__ E003 |%s|",s1); return -1;}
      // printf(" i1=%d deli=|%c|\n",i1,*cp2);

    tso[its].typ  = Typ_FncNam;
    tso[its].form = i1;
    fncAct = i1;
    goto L_saveExpr;



  //----------------------------------------------------------------
  L_4:
    // strncpy (s1, cp1, ii); s1[ii] = '\0';
    for(i1=0; i1<ii; ++i1) s1[i1] = toupper (cp1[i1]);
    s1[ii] = '\0';
      // printf(" _txo_srcLn_objectCode-s1=|%s|\n",s1);

  // test for objectCodes (CUT, REV, ...) ObjCodTab Typ_Cmd1
    i1 = UTX_cmp_word_wordtab (ObjCodTab, s1);
    if (i1 >= 0) {
      // printf(" found SubCmd %d\n",i1);
      tso[its].typ = Typ_cmdNCsub;
      tso[its].form = i1;
      goto L_saveExpr;
    }


  // test for num. constants; eg "RAD_180" "SR_3" "PI"
  // PI, RAD_1 RAD_90 RAD_180 RAD_360 SR_2 SR_3
  // see ATO_srcTxt
    i1 = UTX_cmp_word_wordtab (NcoTxtTab, s1);
    if (i1 >= 0) {
        // printf(" found SubCmd %d\n",i1);
      tso[its].typ = Typ_ConstVal;
      tso[its].form = i1;
      goto L_saveExpr;
    }


  // test for geom. constants: eg "DX" "RX"
  // DX DY DZ DIX DIY DIZ RX RY RZ
  // see ATO_srcTxt
    ii = UTX_cmp_word_wordtab (GcoTxtTab, s1);
    if (ii >= 0) {
      // printf(" found SubCmd %d\n",i1);
      tso[its].typ = Typ_ConstOG;
      tso[its].form = ii;
      goto L_saveExpr;
    }


  // test for controlOperators (EQ NE LT GT G_E L_E)  Typ_ope_eq ..
    ii = UTX_cmp_word_wordtab (CopTxtTab, s1);
    if (ii >= 0) {
      // printf(" found SubCmd %d\n",i1);
      tso[its].typ = Typ_ope__;
      tso[its].form = Typ_ope__ + 1 + ii;
      goto L_saveExpr;
    }




  goto L_err99;  // cannot analyze expr..



  //================================================================
  L_saveExpr:
      // printf(" L_saveExpr: %d |%s| l=%d\n",its,cp2,cp2 - cp1);
    // cp1 = first char of expr;
    // cp2 = last char of expr;
    tso[its].ioff = cp1 - sln;
    tso[its].ilen = cp2 - cp1;
    ++its;
    if(its >= itsMax) goto L_errIts;

    cp1 = cp2;
    goto L_nextExpr;



  //================================================================
  L_fnc1:
    // add function Typ_FncNam,Typ_Val (brackets without function; eg "(V10+1)"
    tso[its].typ = Typ_FncNam;
    tso[its].form = Typ_Val;
    cp2 = cp1;
    goto L_saveExpr;
   


  //================================================================
  L_err1: // string has no closing "
    strcpy(s1, "APED_txo_srcLn__: string not closed ..");
    goto L_errEx;

  L_err99:  // cannot analyze expr..
    strcpy(s1, "APED_txo_srcLn__: cannot analyze expr ..");
    goto L_errEx;

  L_errIts: // tso too small;
    strcpy(s1, "APED_txo_srcLn__: tso out of space ..");
    goto L_errEx;

  L_errLev: // tso too small;
    strcpy(s1, "APED_txo_srcLn__: lev out of space ..");
    // goto L_errEx;

  L_errEx:
    TX_Error(s1);
    tso[its].typ  = TYP_FuncEnd;
    return -1;
}


//==========================================================================
  int APED_txo_modObj_get (int *pma, ObjTXTSRC *soi, int rMax,
                           int typ, char *stx) {
//==========================================================================
// - get all modifyable-objects with typ=<typ> out of soi into pma
// modifyable-object: Typ_NumString
// Input: 
//   soi     1-n atomic-objects - eg from APED_txo_srcLn__
//   rMax    size of pma
//   typ     type of modifyable-objects to find; Typ_PT [not yet: Typ_Val]
//   stx     sourceText unused
// Output:
//   pma     list of ObjSRC-records with modifyable objs
//           0=not-modifyable; 1=point-3-values; [not yet: 2=value-single]
//   retCod  nr of objects in pma


  int      iri, ipar, oNr;


  printf("APED_txo_modObj_get typ=%d rMax=%d\n|%s|\n",typ,rMax,stx);
  APED_txo_dump (soi, stx, "_modObj_get-soi");

  // APED_txo_init (soo);  // txo[0].typ = TYP_FuncEnd;

  iri = -1;
  oNr = 0;

  // find next startPos ips of obj to find ..
  L_nxt_obj:
  ++iri;           printf(" nxt-iri=%d ipar=%d\n",iri,soi[iri].ipar);

  L_nxt_ck:

    if(soi[iri].typ == TYP_FuncEnd) goto L_exit;

    // find next primary obj
    if(soi[iri].ipar != -1) goto L_nxt_obj;

    // test if is a DB-obj of form <typ>
    if(soi[iri].form == Typ_ObjDB) {
      if(typ == DB_Typ_Char (&stx[soi[iri].ioff])) {
        ++iri;
        goto L_Mod;
      }
    }
  

    // skip all but <typ>
    if(soi[iri].form != typ) goto L_nxt_obj;  // L_notMod;



    ipar = iri;
    for(;;) {
      ++iri;
      if(soi[iri].ipar != ipar) break;              //  next obj
      if(soi[iri].form != Typ_NumString) goto L_notMod;
    }

    // all components of obj are modifyable
    L_Mod:
      pma[oNr] = 1;  //  0=not; 1=point-3-values; 2=value-single
      ++oNr;
      if(oNr > rMax) goto L_err1;
      goto L_nxt_ck;


    L_notMod:
      pma[oNr] = 0;  // not mod.
      ++oNr;
      if(oNr > rMax) goto L_err1;
      // continue - 
      goto L_nxt_obj;



  //----------------------------------------------------------------
  L_exit:

    // TESTBLOCK
    // APED_txo_dump (soo, stx, "_modObj_get-soo");
    printf("ex-_modObj_get oNr=%d\n",oNr);
    for(iri=0; iri<oNr;++iri) printf(" pma[%d] = %d\n",iri,pma[iri]);
    // END TESTBLOCK

  return oNr;


  //----------------------------------------------------------------
  L_err1:
    TX_Error("APED_txo_modObj_get E1");
    return -1;

}


//===============================================================
  int APED_txo_find_parent (int *typ, long *dbi,
                            ObjTXTSRC *tso, char *txso, int ip) {
//===============================================================
// APED_txo_find_parent        get 1. parent of subObj <ip> typ=Typ_PT
// Input:

  int  its=-1, npt=-1, ipar;
  char objID[64];


  printf("APED_txo_find_parent %d\n",ip);
  APED_txo_dump (tso, txso, "_find_parent-tso");


  for(;;) {
    ++its;      printf(" nxt-its=%d\n",its);
    if(tso[its].typ == TYP_FuncEnd) break;

    if(tso[its].form != Typ_PT) continue;     // skip all but points
    ++npt;
    if(npt < ip) continue;                    // find point # ip

    ipar = its;

    for(;;) {
      ++its;
      if(tso[its].ipar != ipar) break;         //  end of subObj

      if(tso[its].form == Typ_ObjDB) {
        // get objID out of txso
        strncpy (objID, &txso[tso[its].ioff], tso[its].ilen);
        objID[tso[its].ilen] = '\0';
          printf(" find_parent-objID=|%s|\n",objID);
        APED_dbo_oid (typ, dbi, objID);
        return 0;
      }
    }

  }

  *typ = Typ_Error;
  *dbi = 0L;

  return -1;

}


//================================================================
  int APED_txo_dump (ObjTXTSRC *tso, char *sl, char *auxTxt) {
//================================================================
/// sl: NULL or the corresponding codeline

  int   its=0, i1, i2;
  char  fmtBuf[32];


  printf("%s ======================= APED_txo_dump \n",auxTxt);


  for(;;) {
    if(tso[its].typ == TYP_FuncEnd) break;
    printf("tso[%d].typ=%d form=%d len=%d par=%d off=%d",its,
            tso[its].typ,
            tso[its].form,
            tso[its].ilen,
            tso[its].ipar,
            tso[its].ioff);
    if(sl) {
      i1 = tso[its].ilen;
      sprintf(fmtBuf, " |%%%d.%ds|",i1,i1);
      i2 = tso[its].ioff;
      printf(fmtBuf, &sl[i2]);
    }
    printf("\n");
      // printf("   %s\n",fmtBuf);


    ++its;
  }


  return 0;

}


/* UNUSED:
//========================================================================
  int APED_txo_add (ObjTXTSRC *tso, int tsoSiz,
                    ObjTXTSRC *tsi, int iRec, int iNr) {
//========================================================================
// APED_txo_cpy       copy iNr records of tso0 starting at iRec -> tso1
// TODO: update ipar-index


  int    i1, i2, ie;

  printf("APED_txo_add iRec=%d iNr=%d, siz=%d\n",iRec,iNr,tsoSiz);

  if(iRec + iNr >= tsoSiz) {TX_Error("APED_txo_add E1 %d",tsoSiz); return -1;}

  // find end in tso
  for(i1=0; i1<tsoSiz; ++i1) {
    if(tso[i1].typ == TYP_FuncEnd) {i2 = i1; goto L_cpy;}
  }
  TX_Error("APED_txo_add E2"); return -1;

  L_cpy:
  ie = iRec + iNr;
  for(i1=iRec; i1<ie; ++i1) { tso[i2] = tsi[i1]; ++i2; }
  tso[i2].typ = TYP_FuncEnd;

    // APED_txo_dump (tso1, "", "ex-_txo_cpy");

  return 0;

}
*/


//====================== EOF =============================
