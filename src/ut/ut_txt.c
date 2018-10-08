//    UT_TXT.C                                    RF
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
2009-04-06  UTX_cp_Line korr. RF.
2001-10-25  UTX_pos_del korr. RF.

-----------------------------------------------------
*/
#ifdef globTag
void UTX(){}
#endif
/*!
\file  ../ut/ut_txt.c
\brief text manipulations 
\code
Needs:
../ut/ut_TX.h         ../ut/ut_ui_TX.c|../ut/ut_TX.c      TX_Error ..
../ut/ut_umb.h        ../ut/ut_umb.c                      UMB_..
../ut/ut_umem.h       ../ut/ut_umem.c                     UME_..
../ut/ut_txTab.h      ../ut/ut_txTab.c                    UtxTab_init_..
../ut/ut_os.h         ../ut/ut_os_aix.c                   OS_FilSiz ..
../xa/xa_msg.c        ../xa/xa_msg.h                      MSG_STD__ MSG_get_1 ..
../ut/ut_types.h      -                                   FLT_32_MAX
-                     ../ut/ut_mem.c                      MEM_chg_str ..
-                     ../ut/ut_utx_dummy.c
=====================================================
List_functions_start:

UTX_cp_word_2_upper    copy and change word into capitalletters
UTX_cp_nchr_2_upper    copy and change n chars into capitalletters
UTX_cp_word_term       copy word bis zum Terminator
UTX_cp_word__          copy next word
UTX_cp_word_nr1        copy word nr <wNr> out of string
UTX_cp_word_t          copy next word, give terminator
UTX_cp_expr            copy expression (without brackets)
UTX_cp_print           add maxLen printable characters from txi --> txo
// APT_cp_ausd         kopiert einen Ausdruck (kompletter Klammerinhalt)
UTX_CP__               copy n chars - terminate with '\0'                  INLINE
UTX_cp_Line            copy a Line (terminated by '\n')
UTX_cp_left            copy n chars from left side of string
UTX_cp_right           copy n chars from right side of string
UTX_cpLn_2cp           copy line between 2 pointers; del ending blanks, lf

UTX_db10__             double -> string; precision 10 digits
UTX_db15__             double -> string; precision 15 digits

UTX_add_i_u            add integer to string
UTX_add_db             add double unformatted; Trennzeichen ist blank
UTX_add_pt             add Pointcoords unformatted; Trennzeichen ist blank
UTX_add_fl_u           add double unformatted (del foll. 0s and ".")
UTX_add_fl_u2          add double,double unformatted (del foll. 0s and ".")
UTX_add_fl_u3          add 3 doubles unformatted (del foll. 0s and ".")
UTX_add_fl_ua          add n doubles unformatted (del foll. 0s and ".")
UTX_add_fl_1uf         add double with max. 1 digit after dec.point; unformatted
UTX_add_fl_f           add double with x digits after dec.point
UTX_add_fl_10          add double with 10 signif digits
UTX_add_fl_15          add double with 15 signif digits
UTX_add_slash          add closing "/" to string (for dirs)
UTX_add_fnam_del       add closing "/" or "\\" to string (filename-delimiter)

UTX_chrNr              get nr of chars to end of line
UTX_Clear              string = '\0'                                 INLINE
UTX_Siz                get length of rest of string                  INLINE
UTX_CleanCR            delete following Blanks, CRs and LFs
UTX_CleanLF            change lineEnd (Cr|CrLf ..) to single LF
UTX_CleanCommC         Clean C-Comments (remove foll. blanks & // comments)
UTX_Clean_db           clean doubles (change ',' into '.') See also UTX_chg_chr1
UTX_CleanSC            Change SpecialChars into '?' (alle „” -> ?)
UTX_CleanAN            Change all chars not alpha or numeric into '_'
UTX_CleanBracks        Remove first/last char (brackets, '"', ..)
UTX_endDelChar         if last char of string == chr: remove it
UTX_endDelWord         remove last word; delimiting char; keep or not.
UTX_endAddChar         if last char of string != chr: add it
UTX_del_right          delete last characters
UTX_del_chr            delete all characters <cx> out of string cbuf
UTX_del_follBl         delete following blanks, return strLen
UTX_del_foll0          Delete following 0s and following "."
UTX_del_foll_chrn      delete last char if it is in list
UTX_del_FilTx          delete all lines containing <dtxt>

UTX_chg_chr1           in cBuf alle oldChr aendern in newChr
UTX_chg_nr             change following int-number
UTX_chg_str1           change substring in string
UTX_chg_str2           change <sOld> into <sNew> in <sDat>
UTX_chg_right          change right side of string (replace equal nr of chars)
UTX_chg_2_upper        change string to uppercase; skip textStrings("")
UTX_chg_2_lower        change string to lowercase (see "_upper")
UTX_chg_wordn          change word nr wNr in string sOut into word sNew
UTX_utf_iso            convert ISO-8859-1 (1 byte = 1 char) into UTF-8

UTX_insert1            insert Textstring into Textbuffer
UTX_ins_s2             insert string left (s1=s2+s1)
UTX_ins_add            insert string left and add string right (s1=s2+s1+s3)

UTX_cut_word           remove word s1 (and its following delimiter) out of sBuf
UTX_cut1               cut Textstring out of Textbuffer
UTX_cut2               cut lDel chars out of string s1

UTX_len__              change stringlength to <len>; cut or add blanks

UTX_pos_EOS            find end of string ('\0')
UTX_pos_del            find next delimiter (blank or "'()*+,-/;<=>)
UTX_pos_delP           find next program-delimiter (',' or ')')
UTX_pos_del_next       find next delimiter (blank after cPos)
UTX_pos_del_prev       find previous delimiter (blank vor cPos)

UTX_pos_skipLeadBlk    skip blanks (returns Position of first non-blank) INLINE
UTX_pos_1n             REPLACED BY UTX_pos_skipLeadBlk
UTX_pos_skipTermWord   terminate & skip next word in string
UTX_pos_skipWord       skip Word (ret. Pos. of first char after word)
UTX_pos_skipWords      skip n Words (ret. Pos. of first char after word)
UTX_pos_skipBrackX     find corresponding bracket
UTX_pos_skipBrack      find corresponding ')' (returns Position of ')')
UTX_pos_skipBrack1     find corresponding ')' (startpos '(' not included)
UTX_pos_skipStr        find corresponding \" (skip string)
UTX_pos_skipDeli1      skip delimiter ' ' ',' '\n' '\r' '\t'
UTX_pos_skip_num       skip number
UTX_pos_skip_int       skip int-number
UTX_skip_1bl           skip this char and following blanks    INLINE

UTX_ck_caseChr         compare 2 characters - ignore case
UTX_ck_casenChr        compare n chars of 2 strings - ignore case
UTX_ck_caseStr         compare 2 strings - ignore case
UTX_ckc_Del1           check if char is a delimiter
UTX_ckc_Del2           check if char is a delimiter (blank|Tab)
UTX_ck_specChars       test if string contains specialCharacters
UTX_ck_num_i           test if word is numeric (int)
UTX_ck_num_f           test if word is numeric (float)
UTX_ck_num_digNr       returns nr of consecutive digits
UTX_ck_uml_c           check for Umlaut (ÄÖÜäöüß); change to normal char
UTX_ck_uml_s           change all umlaute of string
UTX_cmp_word_wordtab   check if word wd1 is in list wdtab
UTX_find_chr           find character in string (strchr)                 INLINE
UTX_find_Del1          // find next delimiter ' ' '=' ',' '\n' '\t' '{' ..
UTX_find_word1         Wort in Zeile suchen
UTX_find_wordNr        find word "was" in string "wo"; return nr of occurences
UTX_find_strrstr       find last occurence of str in cbuf
UTX_find_strrchrn      find last occurence of one of the chars of str2
UTX_strcmp_right       check if string1 ends with string2

UTX_wGrp_find          find word-position in group-of-words
UTX_wGrp_nxt           find position of next word in group-of-words
UTX_wTab_ck            test if wordTable wTab contains string w1
UTX_ck_wTab            test if string contains a word from wordTable
UTX_wTab_str           make table of words from string
UTX_wTab_srcLn         separate sourceline into expressions
UTX_wTab_file          make table of words from file.
UTX_wTab_dump          dump wordTable

UTX_memstr             den String sstr im Bereich von - bis suchen
UTX_memFind            Daten suchen
UTX_memFind1           Daten mit bestimmter Laenge suchen
UTX_memcmp             returns nr of equal characters of 2 strings
UTX_cnr_chr            returns nr of a certain char in string

UTX_wordnr             returns nr of words
UTX_get_word           get next word and its delimiter and typ of word
UTX_sget_nrRange       get nr or range out of textstring

UTX_db_tx              read float from ascii-string

UTX_fnam__             separate/copy directory,fileName,fileTyp of full filename
UTX_ftyp_s             get filetyp from filename (change => upper)
UTX_fnam_s             get fnam from string
UTX_fdir_s             get fileDirectory from string
UTX_ftyp_cut           remove Filetyp from string
UTX_safeName           make a safe modelname from a modelname
UTX_fnam_fnrel         make absolute filename from relative Filename and basDir
UTX_fnam_rel2abs       make absolute filename from relative Filename and actDir
UTX_fnam_abs2rel       make relative filename from absolutFilename and actDir
UTX_fdir_cut           cut last subpath from path

UTX_fgets              Zeile aus Datei lesen und CRs LFs am Ende deleten
UTX_fgetLine           Zeile Nr. <lNr> aus Datei <filNam> lesen
UTX_fsavLine           Zeile Nr. <lNr> into Datei <filNam> schreiben
UTX_fRevL              File revert lines; eine Datei zeilenweise umdrehen;
UTX_str_file           read file -> string; remove ending '\n'
UTX_wrf_str            write string -> file
UTX_f_lifo_add         add line as first line into file with maxLnr lines

UTX_setup_get__        get parameters (typ, value) from setup-file
UTX_setup_get          get parameters (typ, value) from setup-file (1.word only)
UTX_setup_set          add/replace parameter in file
UTX_setup_modw         add/replace word of value in param-value-file
UTX_setup_decs         decode setup-string; separates parameter, returns valPos.

UTX_dir_listf          Dateiliste in eine Datei ausgeben
UTX_cat_file           Datei in (offene) Datei ausgeben
UTX_ptab_f             Pointertabelle aus Datei erstellen
UTX_ptab_dump          dump table of text-pointers

UTX_htm_fop            open html-textfile
UTX_htm_fcl            close html-textfile

UTX_dump_cnl           dump String (printf) until next '\n'   (maxLen)
UTX_dump_word          print word delimited by one of sDel-chars
UTX_dump_c__           dump <cLen> chars - does not stop at \n
UTX_dump__             dump <cLen> chars (replace \n\r\t)  - stop at \0
UTX_dump_s__           dump <cLen> chars (replace \n\r\t\0)
UTX_dump_p1p2          dump string from ps to pe
UTX_dump_wTab          dumpt table of words

List_functions_end:
=====================================================
- see also:
UtxTab_*               Class for stacking Textstrings of variable length
UTI_iNr_chrNr          give nr of ints for n characters

\endcode *//*----------------------------------------



*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>                // f. isdigit ..
#include <math.h>


#include "../ut/ut_geo.h"
#include "../ut/ut_txt.h"
#include "../ut/ut_txTab.h"              // TxtTab
#include "../ut/ut_os.h"
#include "../xa/xa_msg.h"              // MSG_*



const char TX_NUL = '\0';



static char   TX_buf1[128];
static char   TX_buf2[128];



//================================================================
  int UTX_cp_word_nr1 (char *so, char *si, int wNr) {
//================================================================
// UTX_cp_word_nr1      copy word nr <wNr> out of string
// word-delimiter is a blank; words may not contain blanks !
// RetCode:   0  OK, word is in so.
//           -1  Error, not enough words in si.

  int    i1, ii;
  char   *p1, *p2;


  // printf("UTX_cp_word_nr1 |%s| %d\n",si,wNr);


  //----------------------------------------------------------------
  // skip words (p1 = UTX_pos_skipWords (si);)
  p2 = si;

  for(i1=0; i1 <= wNr; ++i1) {
    if(!p2) return -1;             // not found
    while (*p2 == ' ')  { ++p2; }  // skip leading blanks
    p1 = p2;
    // if(*p1  == '"') .. // skip string "xx" - not yet implemented
    p2 = strchr (p1, ' ');
      // printf(" p1=|%s| p2=|%s|\n",p1,p2);
  }


  //----------------------------------------------------------------
  // copy word (p1 = UTX_cp_word__)
  if(!p2) {
    strcpy (so,p1);
  } else {
    ii = p2 - p1;
      // printf(" ii=%d\n",ii);
    strncpy (so, p1, ii);
    so[ii] = '\0';
  }


  return 0;

}


/* UNUSED
//================================================================
  int UTX_utf_iso (char *sOut, int sLen, char *sIn) {
//================================================================
// convert ISO-8859-1 (1 byte = 1 char) into UTF-8
// see also iconv

  int     i1;
  // int     i2, i3;   // Linux32
  long    i2, i3;  // Linux64 ??
  char    *p1 = NULL;


  printf("UTX_utf_iso |%s|\n",sIn);



  sOut[0] = '\0';
  
  if(!sIn) return 0;


  // test for sonderzeichen              
  // test if input is valid UTF8; 1==OK, 0=must_convert.
  i2 = strlen(sIn);
  i1 = g_utf8_validate (sIn, i2, &p1);   // TRUE=already utf8
      printf(" g_utf8_validate-i1=%d\n",i1);
  if(i1 != 0) {                          // OK, keine Sonderzeichen ..
    strcpy(sOut, sIn);
    goto L_exit;
  } 
    
  // filename enthaelt Sonderzeichen; recode & copy string
  // len auch via -1
    // printf(" convert: |%s|\n",sIn);   // i2,i3: gsize=int|long ?
// L64: cast to pointer from integer of different size ??
  p1 = (char*)g_convert (sIn, (size_t)i2, "UTF-8", "ISO-8859-1",
                  (size_t*)&i2, (size_t*)&i3, NULL);
  if(p1 == NULL) {
    TX_Error("UTX_utf_iso E001");
    return -1;
  }
  if(i3 >= sLen) {
    TX_Error("UTX_utf_iso E002");
    g_free(p1);
    return -1;
  }

  strcpy(sOut, p1);
  g_free(p1);


  L_exit:
    // printf("ex UTX_utf_iso: |%s|\n",sOut);
  return 0;

}
*/

//================================================================
  int UTX_wTab_dump (char **wTab) {
//================================================================
/// UTX_wTab_dump                dump wordTable
 
  int i1 = 0;

  printf("UTX_wTab_dump: \n");

  while(wTab[i1]) {
    printf("  wTab[%d]=|%s|\n",i1,wTab[i1]);
    ++i1;
  }
  printf("end UTX_wTab_dump\n");


  return 0;

}


//=========================================================================
  char** UTX_wTab_file (char *memspc, int memSiz, char *fnam) {
//=========================================================================
/// \code
/// make table of words from file. End of List is NULL.
/// Input:
///   memspc, memSiz      where to save file and table. No reallocate !
/// \endcode

  int        i1, iNr;
  char       **wTab;
  Memspc     strSpc;
  UtxTab_NEW (txTab1);


  // printf("UTX_wTab_file siz=%d |%s|\n",memSiz,fnam);

  // read file, add each line to memspc.
  UME_init (&strSpc, memspc, memSiz);
  UtxTab_init_Memspc (&txTab1, &strSpc);

  // add file to stringTab
  i1 = UtxTab_add_file (&txTab1, fnam);
  if(i1 < 1) return NULL;
    // UtxTab_dump (&txTab1);

  // get next free address of txTab1
  wTab = (char**)UtxTab_pos_free(&txTab1);

  // need i1 bytes
  iNr = UtxTab_nr(&txTab1);
  i1 = sizeof(void*) * (iNr + 1);
    // printf(" i1=%d\n",i1);

  // check if enough free space
  if(i1 >= UtxTab_bytes_free(&txTab1)) {
    printf("***** Error UTX_wTab_file EOM \n");
    return NULL;
  }

  // loop tru wTab; add pointer to each line of file -> optLst.
  for(i1=0; i1<iNr; ++i1) wTab[i1] = UtxTab__(i1, &txTab1);
  wTab[iNr] = NULL;

    // UTX_wTab_dump (wTab);

  return wTab;

}


//=====================================================================
  int UTX_wTab_srcLn (char **spa, int *sSiz, int tabSiz, char *srcLn) {
//==================.==================================================
// separate sourceline into expressions
// srcLn must be terminated with '\0' (not '\n').
// Output:
//   spa        expression-strings (startPositions)
//   sSiz       length of expression-strings
//   RetCod:    nr of expressions (<= tabSiz)

// strings and functions may not be splitted !
// expressionDelimiters: blank or comma (','); 
// skip following blanks, but every comma delimits one expression.


  int      ii;
  char     *cp1, *cp2, *cp3;



  // printf("UTX_wTab_srcLn |%s|\n",srcLn);
  // for(ii=0;ii<12;++ii) printf(" %d %d\n",ii,srcLn[ii]);



  cp1 = srcLn;

  UTX_pos_skipLeadBlk (cp1);  // skip leading blanks

  spa[0] = cp1;
  ii = 1;

  L_nxt:
    // printf(" cp1=|%s| ii=%d\n",cp1,ii);

  // skip string
  if(*cp1 == '\"') {
    cp2 = UTX_pos_skipStr (cp1);
      // printf(" _skipStr |%c|\n",*cp2);
    if(*cp2) {
      ++cp2;  // skip "
      if(!*cp2) goto L_exit;
    } else {
      goto L_exit;
    }

  // find next delimiter
  } else {
    cp2 = strpbrk (cp1, " ,(\"");

  }

  if(!cp2) goto L_exit;
    // printf(" cp2=|%s| %d\n",cp2,*cp2);


  // skip function
  if(*cp2 == '(') {
    cp3 = UTX_pos_skipBrack (cp2);
    if(*cp3) cp2 = ++cp3;  // skip )
    else goto L_exit;
    if(!*cp2) goto L_exit;
  }


  // skip ' '||','
  sSiz[ii - 1] =  cp2 - cp1;
  cp1 = cp2 + 1;
  UTX_pos_skipLeadBlk (cp1);  // skip leading blanks
  spa[ii] = cp1;
  if(ii >= tabSiz) {TX_Error("UTX_wTab_srcLn E001"); return -1;}
  if(*cp1) { ++ii; goto L_nxt;}


  //----------------------------------------------------------------
  L_exit:
  sSiz[ii - 1] = strlen(spa[ii - 1]);


    // {printf("ex UTX_wTab_srcLn %d\n",ii); int i1;
    // for(i1=0;i1<ii;++i1){printf(" expr[%d] = |",i1);
      // UTX_dump_c__(spa[i1],sSiz[i1]); printf("|\n");}
    // }


  return ii;

}


//================================================================
  int UTX_wTab_str (char *wTab[], int tabSiz, char *s1) {
//================================================================
/// \code
/// make table of words from string
/// split s1 into words; add words into fwTab
/// s1 becomes modified (terminates words with \0) !
/// End of List is NULL.
///
/// Output:
///   wTab    table of pointers to words, end of table = NULL
///   retCod  nr of words
///
/// Usage:
/// char *wTab[20];
/// UTX_wTab_str (&wTab, sizeof(wTab), string);
/// char  **pa;
/// pa = wTab;
/// while (*pa) { printf(" |%s|\n",*pa); ++pa; }
///
/// using tempspace (exists only until active function returns):
/// #define WTAB_SIZ 10
/// char   **pa;
/// UTX_wTab_tmpSpc (pa, WTAB_SIZ);
/// \endcode
  
// see also UTX_pos_skipTermWord

  int   irc, wNr;
  char  *p1, *wNxt;
  

  // printf("UTX_wTab_str |%s|\n",s1);

  p1 = s1;
  wNr = 0;
    
      
  L_nxt:
    wNxt = strtok (p1, " ,");   // delimiters are blank OR comma
      // printf(" wNr=%d wNxt=|%s|\n",wNr,wNxt);
      
      
    if(wNxt) {
      wTab[wNr] = wNxt;
      ++wNr;
      p1 = NULL;
      if(tabSiz > wNr) goto L_nxt;
      printf("***** UTX_wTab_str overflow \n");
    }

    wTab[wNr] = NULL;

      // UTX_wTab_dump (wTab);

    return wNr;

}


//================================================================
  int UTX_wGrp_find (char *wGrp, char *wd) {
//================================================================
/// \code
/// UTX_wGrp_find          find word-position in group-of-words
/// group-of-words: words delimited by '|'
/// Example:
/// char  *wGrp = {"UNL|UNL1|UNL2"};
/// ii = UTX_wGrp_find (wGrp, "UN");       // returns -1;
/// ii = UTX_wGrp_find (wGrp, "UNL1");     // returns 4;
/// UTX_cp_word_term (so, &wGrp[ii], '|'); // copy -> so
/// \endcode


  int     i1, wl;
  char    *p1, *p2;


  // printf("UTX_wGrp_find |%s|\n",wGrp);
  // printf("  wd |%s|\n",wd);

  wl = strlen (wd);

  p1 = wGrp;
  i1 = -1;

  L_nxt:
    p2 = strstr (p1, wd);
      // printf(" p2 = |%s|\n",p2);
    if(!p2) goto L_none;

    // next char must be delimiter !
    if(p2[wl] == 0) goto L_exit;
    if(p2[wl] == '|') goto L_exit;
    // find next delimiter
    p1 = strchr (p2, '|');
    if(!p1) goto L_none;
    ++p1;
    goto L_nxt;


  L_exit:
    i1 = p2 - wGrp;

  L_none:
    // printf("ex UTX_wGrp_find %d\n", i1);

  return i1;

}


//================================================================
  int UTX_wGrp_nxt (char *wGrp, int wPos) {
//================================================================
/// \code
/// UTX_wGrp_nxt           find position of next word in group-of-words
/// group-of-words: words delimited by '|'
/// Example:
/// char  *wg = {"UNL|UNL1|UNL2"};
/// ii = UTX_wGrp_nxt (wg, 0);       // returns 4;
/// ii = UTX_wGrp_nxt (wg, 9);       // returns -1;
/// \endcode


  int     i1, wl;
  char    *p1, *p2;


  // printf("UTX_wGrp_nxt |%s| %d\n",wGrp,wPos);


  p1 = strchr (&wGrp[wPos], '|');

  if(!p1) return -1;

  ++p1;

  return (p1 - wGrp);

}


//================================================================
  int UTX_wTab_ck (char *wTab[], char *w1) {
//================================================================
/// \code
/// test if wordTable wTab contains string w1
/// RetCod:
///     -1    no word of wTab = w1
///    >=0    nr of word found
/// Example:
/// char     *ta[]={"Jaenner","Februar","Maerz",NULL};
/// printf(" %d\n", UTX_wTab_ck (ta, "Februar"));   // returns 1
/// see also UTX_ck_wTab
/// \endcode

  int      irc = 0;
  char     **pa;


  // printf("UTX_ck_swTab |%s|\n",w1);

  // found line with objName; test all filterWords.
  pa = wTab;
  while (*pa) {
      // printf(" test |%s|\n",*pa);
    if(! strcmp(*pa, w1)) return irc;
    ++pa;
    ++irc;
  }

  return -1;

}


//================================================================
  int UTX_ck_wTab (char *ps, char *pe, char *wTab[]) {
//================================================================
/// \code
/// test if string ps-pe contains a word from wordTable wTab
/// RetCod:
///     -1    no word of wTab in string ps-pe
///    >=0    nr of word found 
/// see also UTX_wTab_ck
/// \endcode

  int      irc = 0;
  char     **pa;


  // printf("UTX_ck_wTab\n");
  // UTX_dump_wTab (wTab);
  // printf(" ps-pe = |");UTX_dump_p1p2(ps,pe);printf("|\n");



  // found line with objName; test all filterWords.
  pa = wTab;
  while (*pa) {
      // printf(" test |%s|\n",*pa);
    // if(UTX_memstr(pNam, endPos, *pa) == NULL) goto L_nxt_ln;    // AND
    if(UTX_memstr(ps, pe, *pa) != NULL) return irc;    // OR
    ++pa;
    ++irc;
  }

  return -1;

}


//================================================================
  int UTX_len__ (char *txt, int sLen) {
//================================================================
/// UTX_len__    change stringlength to <len>; cut or add blanks


  int  i1, il;


  il = strlen(txt);

  if(il < sLen) {
    for(i1=il; i1<sLen; ++i1) {
      txt[i1] = ' ';
    }
  }

  txt[sLen] = '\0';

  return 0;

}


//================================================================
  int UTX_safeName (char *snam, int mode) {
//================================================================
/// \code
/// UTX_safeName           make a safe modelname from a modelname
/// mode:
///   0:  change '. ' to '_', do not change '/' (fuer absolute Filenames)
///   1:  change all '. ' and '/' to '_'
///   2:  change all '/' to '_'
/// \endcode


  int   iPos, iLen;

  // printf("UTX_safeName %d |%s|\n",mode,snam);

  iLen = strlen(snam);


  if(mode > 1) goto L_1_noAmoi;

  L_0_noAmoi:
    iPos = strcspn (snam, ". ");
    if(iPos < iLen) {
      snam[iPos] = '_';
      goto L_0_noAmoi;
    }

  if(mode < 1) return 0;


  //----------------------------------------------------------------
  L_1_noAmoi:
    iPos = strcspn (snam, ". /\\");
    if(iPos < iLen) {
      snam[iPos] = '_';
      goto L_1_noAmoi;
    }

  // printf("ex-UTX_safeName %d |%s|\n",mode,snam);

  return 0;

}


//================================================================
  int UTX_fdir_cut (char *fdir) {
//================================================================
/// \code
/// UTX_fdir_cut        cut last subpath from path
/// last char of fdir must be '/'
/// \endcode

  int    i1;
  char   *p1;

  // printf("UTX_fdir_cut |%s|\n",fdir);

  // last char must be '/'
  if(!fdir) return -1;
  i1 = strlen(fdir);
  if(i1 < 3) return -2;

  --i1;
  fdir[i1] = '\0';  // overwrite last delimiter

  // find last del
  p1 = strrchr (fdir, '/');
  if(p1) {
    ++p1;
    *p1 = '\0';
      // printf("ex UTX_fdir_cut |%s|\n",fdir);

    return 0;
  }

  return -1;

}


//=========================================================================
  int UTX_fnam_fnrel (char *fnAbs, int isiz, char *fnRel, char *basDir) {
//=========================================================================
/// UTX_fnam_rel2abs     make absolute filename from relative Filename and basDir
/// RF 2018-05-23

  int      ls, irc, fTmp = 0;
  char     *si, *so = NULL;


  // printf("UTX_fnam_fnrel %d basDir=|%s| fnRel=|%s|\n",isiz,basDir,fnRel);

  ls = strlen(basDir);   if(ls >= isiz) return -1;
  strcpy (fnAbs, basDir);
  UTX_add_fnam_del (fnAbs);   // add closing "/"


  si = fnRel;
  so = fnAbs;

  if(si == so) {
    so = MEM_alloc_tmp(isiz);
    so[0] = '\0';  //strcpy(so,si);
    fTmp = 1;
  }


  //----------------------------------------------------------------
  L_nxt:
  // "./" - skip this
  if(!strncmp(si, "./", 2)) {
    si += 2;
    goto L_nxt;
  }


  // "../"  go one back
  if(!strncmp(si, "../", 3)) {
    si += 3;
    // remove one path of outDir (cut last)
    irc = UTX_fdir_cut (so);
    if(irc < 0) return irc;
    goto L_nxt;
  }


  //----------------------------------------------------------------
  // add to absolute path
  ls += strlen(so) + 1;
  if(ls > isiz) {
    printf("***** UTX_fnam_fnrel overflow-1\n");
    return -1;
  }
  strcat(so, si);
  if(fTmp) strcpy(fnAbs, so);
  UTX_add_fnam_del (fnAbs);   // add closing "/"

    // printf("ex-UTX_fnam_fnrel fnAbs=|%s|\n",fnAbs);

  return 0;

}


//=========================================================================
  int UTX_fnam_rel2abs (char *fnAbs, int isiz, char *fnRel, char *actDir) {
//=========================================================================
/// UTX_fnam_rel2abs     make absolute filename from relative Filename and actDir
/// DO NOT USE - replaced by UTX_fnam_fnrel

  int      i1, irc;
  char     *p1;

  
  // printf("UTX_fnam_rel2abs %d |%s|%s|\n",isiz,fnRel,actDir);

  strcpy (fnAbs, actDir);

  p1 = fnRel;


  //----------------------------------------------------------------
  L_nxt:
  // "./" - skip this
  if(!strncmp(p1, "./", 2)) {
    p1 += 2;
    goto L_nxt;
  }


  // "../"  go one back
  if(!strncmp(p1, "../", 3)) {
    p1 += 3;
    // remove one path of fnAbs (cut last)
    irc = UTX_fdir_cut (fnAbs);
    if(irc < 0) return irc;
    goto L_nxt;
  }


  // add to absolute path
  i1 = strlen(fnAbs) + strlen(p1) + 1;
  if(i1 > 128) {
    printf("***** UTX_fnam_rel2abs overflow-1\n");
    return -1;
  }
  strcat(fnAbs, p1);
  UTX_add_fnam_del (fnAbs);   // add closing "/"


  return 0;

}


//================================================================
  int UTX_fnam_abs2rel (char *fnRel, char *fnAbs, char *actDir) {
//================================================================
/// \code
/// make relative filename from absolutFilename and currentDirectoryName
/// actDir    must end with closing "/"
/// \endcode

// Tests:
// UTX_fnam_abs2rel (s1, "/aa/bb/t1.dat", "/aa/bb/") > "t1.dat"
// UTX_fnam_abs2rel (s1, "/aa/bb/cc/t1.dat", "/aa/bb/") > "cc/t1.dat"
// UTX_fnam_abs2rel (s1, "/aa/bb/cc/dd/t1.dat", "/aa/bb/") > "cc/dd/t1.dat"

// UTX_fnam_abs2rel (s1, "/aa/cc/t1.dat", "/aa/bb/") > "../cc/t1.dat"
// UTX_fnam_abs2rel (s1, "/aa/cc/t1.dat", "/aa/bb/dd/") > "../../cc/t1.dat"


  int  i1, ie, id, inr;
#ifdef _MSC_VER
  char sdir[4] = "..\\";
#else
  char sdir[4] = "../";
#endif

  // printf("UTX_fnam_abs2rel |%s|%s|\n",fnAbs,actDir);


  // wie viele characters sind gleich ?
  ie = UTX_memcmp (fnAbs, actDir);

  id = strlen (actDir);
    // printf(" ie=%d id=%d\n",ie,id);


  if(ie >= id) {
    strcpy(fnRel, &fnAbs[ie]);
    goto L_exit;
  }

  // only i1 of id characters are equal; how much directories go down ?
    printf(" not equal: |%s|%s|\n",&actDir[ie],&fnAbs[ie]);
  // compute nr of delimiters fnam_del
  inr = UTX_cnr_chr (&actDir[ie], fnam_del);


  fnRel[0] = '\0';
  for(i1=0; i1<inr; ++i1) strcat(fnRel, sdir);
  strcat(fnRel, &fnAbs[ie]);


  //================================================================
  L_exit:
  // printf("ex UTX_fnam_abs2rel |%s|%s|%s|\n",fnRel,fnAbs,actDir);
  // printf(" ==========================\n");


  return 0;

}


//================================================================
  int UTX_fnam__ (char *fdir, char *fnam, char *ftyp, char *fnIn) {
//================================================================
/// \code
/// UTX_fnam__        separate/copy directory,fileName,fileTyp of full filename
/// Input:
///   fnIn      full filname; 
///   fdir      size must be >= 128
///   fnam      size must be >= 128
///   ftyp      size must be >= 32 
/// Output:
///   fdir      directory; can be NULL (no directory);
///   fnam      filname;
///   ftyp      fileTyp;  can be NULL (no fileTyp);
///
/// fdir out eg: "/xx" (absolute directory)
///          or  "Data" (symbolic-directory)
///          or  ".x" or "../x" or "."  (relative direcotory)
///
/// see also UTX_fdir_s UTX_fnam_s UTX_ftyp_s
/// \endcode


  int    sdl, snl;
  char   *pfn, *pft;

  fdir[0] ='\0';


  // printf("----------------------------------- \n");
  // printf("UTX_fnam__ |%s|\n",fnIn);

  // pfn = find last filename-delimiter
  // must check for '/' AND '\' (in MS '/' can come from out of source)
#ifdef _MSC_VER
  pfn = UTX_find_strrchrn(fnIn, "/\\");
#else
  pfn = strrchr(fnIn, fnam_del);
#endif
  if((pfn - fnIn) >= 256) return -1;
  if(!pfn) {
    // no directory;
    fdir[0] ='\0';
    pfn = fnIn;
  } else {
    sdl =  pfn - fnIn + 1;
    strncpy(fdir, fnIn, sdl);
    fdir[sdl] = '\0';
    ++pfn;  // skip deli
  }

  // find pft = filetype-delimiter
  pft = strrchr(pfn, '.');
  if(!pft) {
    // no filetype;
    ftyp[0] ='\0';
    // copy the filname
    strcpy(fnam, pfn);
  } else {
    snl = pft - pfn;   // printf(" snl=%d\n",snl);
    if(snl) {
      ++pft; // skip '.'
      // copy filetyp
      if(strlen(pft) >= 32) return -3;
      strcpy(ftyp, pft);
      // copy the filname
      if(snl >= 128) return -2;
      strncpy(fnam, pfn, snl);
      fnam[snl] = '\0';
    } else {
      // no filetype
      if(strlen(pft) >= 128) return -2;
      strcpy(fnam, pft);
    }
  }

    // printf("ex-_fnam__ |%s|%s|%s|\n",fdir,fnam,ftyp);

  return 0;

}


//================================================================
  int UTX_ftyp_cut (char *cbuf) {
//================================================================
/// UTX_ftyp_cut           remove Filetyp from string

  char   *p1;

  // printf("UTX_ftyp_cut |%s|\n",cbuf);


  // remove all leading '.'-s
  while(*cbuf == '.') {++cbuf;}


  p1 = strrchr (cbuf, '.');
  if(p1 == NULL) goto L_done;
  // if(p1 == cbuf) goto L_done;   // zB ".0"  beginnt mit und hat nur einen
  if(p1 > cbuf) {
    if(*(p1-1) == fnam_del) goto L_done; // "/dir/.fnam"  hat keinen
  }

  // foll. char darf nicht sein FilenamedelimiterChar (/)
  ++p1;
  if (*p1 == fnam_del) goto L_done;
  --p1;
  *p1 = '\0';


  L_done:
  // printf(" ex UTX_ftyp_cut |%s|\n",cbuf);
  return 0;

}


//================================================================
  int UTX_ftyp_s (char *ftyp, char *cbuf, int mode) {
//================================================================
/// \code
/// UTX_ftyp_s             get filetyp from filename (change => upper)
/// Input:
///   mode = 0  do not change ftyp-case
///   mode = 1  change ftyp > upperLetters
/// Output:
///   ftyp      uppercase
/// 
/// IRC  0: Filetyp out in ftyp (in Groszbuchstaben)
/// IRC -1: kein Filetyp gefunden ..
/// ".0" kein Filetyp !
/// \endcode

  char   *p1;


  // printf("UTX_ftyp_s |%s|\n",cbuf);

  p1 = strrchr(cbuf, '.');  // find last .

  if(p1 == NULL) goto L_err;


  // wenn filename mit . beginnt, gilt das als Filename und nicht als Filetyp !
  if(p1 == cbuf) goto L_err;
  if(*(p1-1) == fnam_del) goto L_err;   // z.B. "./.0"


  ++p1;   // skip "."
  if(mode == 1) UTX_cp_word_2_upper (ftyp, p1);
  else strcpy (ftyp, p1);

    // printf("ex UTX_ftyp_s 0 |%s|%s|\n",ftyp,cbuf);

  return 0;

  //----------------------------------------------------------------
  L_err:
    // printf("ex UTX_ftyp_s -1 |%s|\n",cbuf);
  return -1;
}



//================================================================
  int UTX_fnam_s (char *fnam, char *cbuf) {
//================================================================
/// \code
/// UTX_fnam_s        get fnam from string
/// alles nach dem letzten FilenamedelimiterChar fnam_del ist fileName;
/// kein fnam_del: fnam = s
/// IRC  0: OK, fnam out
/// IRC -1: kein Filename gefunden ..
/// \endcode

  int    i1;
  char   *p1, *p2;


  // printf("UTX_fnam_s |%s|\n",cbuf);

  if(strlen(cbuf) < 1) goto L_err;

  // p1 = strrchr(cbuf, fnam_del);  // find last FilenamedelimiterChar
  p1 = strrchr(cbuf, '/');  // find last FilenamedelimiterChar
  p2 = strrchr(cbuf, '\\');  // find last FilenamedelimiterChar

  if(p1 == NULL) p1 = cbuf;
  else ++p1;     // skip fnam_del

  if(p2 == NULL) p2 = cbuf;
  else ++p2;     // skip fnam_del

  if(p2 > p1) p1 = p2;

  strcpy(fnam, p1);

  // printf("ex UTX_fnam_s 0 |%s|%s|\n",fnam,cbuf);

  // remove last char if it is |"|
  i1 = strlen(fnam) - 1;
  if(fnam[i1] == '"') fnam[i1] = '\0';

  return 0;

  L_err:
  // printf("ex UTX_fnam_s -1 |%s|\n",cbuf);
  return -1;
}


//================================================================
  int UTX_fdir_s (char *fdir, char *cbuf) {
//================================================================
/// \code
/// UTX_fdir_s        get fileDirectory from string
/// last char ist immer der FilenamedelimiterChar !
/// alles vor dem letzten FilenamedelimiterChar fnam_del ist fileDir;
/// Wenn erster Char ist fnam_del: absolutes fileDir; else relativ.
/// relativ: das pwd (os_bas_dir) vorne weg ...
/// \endcode

  int    i1;
  char   *p1, *p2;


  // printf("UTX_fdir_s |%s|\n",cbuf);

  // skip 1. char if it is |"|
  if(*cbuf == '"') ++cbuf;


  // absoluter oder relativer Filename ?
  if(cbuf[0] == '\\') goto L_abs;     // zB "\\ooserv\...."
  if(cbuf[0] == '/')  goto L_abs;     // zB "/xx/yy/fn"

#ifdef _MSC_VER
  if(cbuf[1] == ':')  goto L_abs;
#endif

  L_rel: // relatives verzeichnis
    strcpy (fdir, OS_get_bas_dir ());
    p1 = cbuf;
    if((*p1 == '.')&&(*(p1+1) == fnam_del)) p1 += 2;  // skip "./"
    strcat (fdir, p1);
    goto L_cut;


  L_abs: // absolut (kompletter) filename
    strcpy (fdir, cbuf);





  L_cut:     // remove Filename
  // printf("      fnam tot=|%s|\n",fdir);

  p1 = strrchr(fdir, '/');  // find last FilenamedelimiterChar
  p2 = strrchr(fdir, '\\');  // find last FilenamedelimiterChar

  if(p1 == NULL) p1 = fdir;
  else ++p1;         // FilenamedelimiterChar soll last char bleiben

  if(p2 == NULL) p2 = fdir;
  else ++p2;         // FilenamedelimiterChar soll last char bleiben

  if(p2 > p1) p1 = p2;

  *p1 = '\0';     // cut off filename

  // printf("ex UTX_fdir_s 0 |%s|%s|\n",fdir,cbuf);

  return 0;

  L_err:
  // printf("ex UTX_fdir_s -1 |%s|\n",cbuf);
  return -1;
}


//==========================================================================
  int UTX_cat_file (FILE *fpo, char *fnam) {
//==========================================================================
/// \code
/// UTX_cat_file           add file into open fileunit
/// add file 
/// ATTENTION:
/// MICROSOFT-BUG: you may not write into a file opened in dll with core-function
/// \endcode

#define bSiz 4096

  int     i1;
  long    l1;
  char    *fBuf;
  div_t   div1;
  FILE    *fpi;


  // printf("UTX_cat_file |%s|\n",fnam);


  l1 = OS_FilSiz (fnam);


  div1 = div (l1, bSiz);
    // printf(" _cat_file %d %d\n",div1.quot,div1.rem);

  // get spc
  fBuf = MEM_alloc_tmp (bSiz + 32);

  if((fpi = fopen(fnam, "rb")) == NULL) return -1;

  if(div1.quot > 0) {
    for(i1=0; i1<div1.quot; ++i1) {
      // read into fBuf
      fread(fBuf, 1, bSiz, fpi);
      // write fBuf
      fwrite(fBuf, 1, bSiz, fpo);
    }
  }

  if(div1.rem > 0) {
    // read into fBuf
    fread(fBuf, 1, div1.rem, fpi);
    // write fBuf
    fwrite(fBuf, 1, div1.rem, fpo);
  }

  fclose(fpi);

    // printf("ex UTX_cat_file\n");

  return 0;

}


//================================================================
  int UTX_wrf_str (char *fnam, char *txt) {
//================================================================
/// \code
/// UTX_wrf_str            write string -> file
/// txt must be null-terminated
/// see UTF_wrf_line
/// \endcode

  FILE     *fp1;

  // printf("UTX_wrf_str |%s|\n",fnam);


  // write Line -> File
  // if((fp1=fopen(fnam,"w")) == NULL) {
  // dont usw "w"; Microsoft makes CRLF from LF;  2013-05-08
  if((fp1=fopen(fnam, "wb")) == NULL) {
    TX_Print("UTX_wrf_str E001 |%s|",fnam);
    return -1;
  }
  // fwrite(txt, strlen(txt)+1, 1, fp1);
  fprintf(fp1, "%s",txt);                        // 2012-07-14 \n removed
  fclose(fp1);

  return 0;

}


//======================================================================
  int UTX_str_file (char* txbuf, long *fSiz, char *fnam) {
//======================================================================
/// \code
/// UTX_str_file           read file -> string; remove ending '\n'
/// fSiz must have exact filesize; see OS_FilSiz.
/// See MEM_get_file()
/// \endcode

  int   irc;


  // get file
  irc = MEM_get_file (txbuf, fSiz, fnam);
  if(irc < 0) return irc;

  // remove ending '\n'
  if(txbuf[*fSiz - 1] == '\n') {    // LF
    *fSiz -= 1;
    txbuf[*fSiz] = '\0';
  }

  return 0;

}


//==========================================================================
  int UTX_dir_listf (char *outFilNam, char *fPath, char *fNam, char *fTyp) {
//==========================================================================
/// \code
/// Create list of files into file; filters: filename and/or filetyp.
/// Input:
///   fPath     dir to search; can have closing "/"
///   fNam      NULL or filename to search; do not use closing "*"
///   fTyp      NULL or filetyp to search; do not use starting "*"
/// Output:
///   outFilNam list
/// RetCode:    -1 = Error; else nr of files written.
///
/// Example:  ls ./xa_*.c
///   i1 = UTX_dir_listf ("lst.dat", ".", "xa_", ".c");
///  

/// eine Dateiliste in eine Datei ausgeben; nach Dateitypen gefiltert.
/// ftyp ohne Stern; also zB ".dat".
/// fPath mit oder ohne abschliessenden "/".
/// RetCode: -1 = Error; else nr of files written.
/// \endcode


  int   i1, ilenTyp, ilenNam, iNr, iOut;
  char  cbuf[256], *p1;
  FILE  *fpo;


  // printf("UTX_dir_listf |%s|%s|%s|%s|\n",outFilNam,fPath,fNam,fTyp);


  if(OS_checkDirExist (fPath) == 0) {
    TX_Print("UTX_dir_listf E001 %s",fPath);
    return -1;
  }


  // try to open outfile
  if((fpo=fopen(outFilNam,"w")) == NULL) {
    TX_Print("UTX_dir_listf E002 %s",outFilNam);
    return -1;
  }


  // filter filename
  if(fNam) ilenNam = strlen(fNam);
  else ilenNam = 0;
    // printf(" ilenNam = %d\n",ilenNam);


  // filter filetyp
  if(fTyp) ilenTyp = strlen(fTyp);
  else ilenTyp = 0;
    // printf(" ilenTyp = %d\n",ilenTyp);


  iNr = 0;
  strcpy(cbuf, fPath);  // in: Path; Out: File !
  OS_dir_scan_ (cbuf, &iNr);   // Init
  // printf(" nach OS_dir_scan_ %d\n",iNr);

  iOut = 0;

  if(iNr > 0)  {

    for (;;) {

      OS_dir_scan_ (cbuf, &iNr);
      // printf(" n.scan %d |%s|\n",iNr,cbuf);
      if(iNr < 0) break;


      // check filetyp; zB "*.dat"
      if(ilenTyp > 0) {
        i1 = strlen(cbuf);
        if(strncmp(&cbuf[i1-ilenTyp], fTyp, ilenTyp)) continue;
      }

      // skip directories
      if(OS_check_isDir(cbuf) == 0) continue;


      // strip directory - find last "/" (makes |t1| aus |/mnt/x/t1|)
      p1 = strrchr (cbuf, fnam_del);
      if(p1) ++p1;
      else p1 = cbuf;


      // check filename; zB "xa_*"
      if(ilenNam > 0) {
        i1 = strlen(cbuf);
        if(strncmp(p1, fNam, ilenNam)) continue;
      }


      // printf(" fNam %d |%s|%s|\n",iNr,cbuf,p1);
      fprintf(fpo,"%s\n",p1);
      ++iOut;

    }
  }


  fclose(fpo);

    // printf("ex UTX_dir_listf %d |%s|%s|%s|\n",iOut,outFilNam,fPath,fTyp);

  return iOut;

}


//=================================================================
  int UTX_ptab_f (void *outSpc, long spcSiz, char *inFilNam) {
//=================================================================
/// eine Pointertabelle (zB fuer GUI_Popup) aus einer Datei erstellen.
/// File has one entry per line.


  int    i1, pSiz, cSiz, cPos;
  FILE   *fp1;
  char   lbuf[256], **pTab, *cbuf;


  // printf("UTX_ptab %d |%s|\n",spcSiz,inFilNam);


  pTab = outSpc;
  pSiz = spcSiz / sizeof(char*) / 2;
  
  // in der mitte beginnt der Charbuf
  cbuf = &((char*)outSpc)[spcSiz/2];
  cSiz = spcSiz/2;


  if((fp1=fopen(inFilNam,"r")) == NULL) {
    TX_Print("UTX_ptab E001 %s",inFilNam);
    return -1;
  }


  i1 = 0;
  cPos = 0;
  cbuf[cPos] = '\0';

  while (!feof (fp1)) {
    if (fgets (lbuf, 250, fp1) == NULL) break;
    UTX_CleanCR (lbuf);
    // printf(" in:|%s|\n",lbuf);

    pTab[i1]=&cbuf[cPos];
    strcat(&cbuf[cPos], lbuf);

    cPos += strlen(lbuf);
    if(cPos >= cSiz) return -1;
    cbuf[cPos] = '\0';
    ++cPos;
    cbuf[cPos] = '\0';

    ++i1;
    pTab[i1]=NULL;
    if(i1 >= pSiz) return -1;

  }


  fclose(fp1);




/*
  // Nur Test:
  for(i1=0; i1<spcSiz; ++i1) {
    if(pTab[i1] == NULL) break;
    printf(" pTab[%d]=|%s|\n",i1,pTab[i1]);
  }
  // Nur Test:
*/



  return 0;

}


//================================================================
  int UTX_ptab_dump (char **pp1) {
//================================================================
/// UTX_ptab_dump          dump table of text-pointers

  printf("UTX_ptab_dump ---------------\n");

  while (*pp1) { printf(" |%s|\n",*pp1); ++pp1; }

  return 0;

}


//=================================================================
  char* UTX_find_word1 (char *was, char *wo) {
//=================================================================
/// \code
/// UTX_find_word1         Wort in Zeile suchen
/// das Wort was in der zeile wo suchen. WortTrennzeichen sind " ,()[]".
/// IRC = NULL:  ist nicht enthalten
/// IRC != NULL = position
/// \endcode

  static const char DelChars[] = {" ,()[]=\";"};

  char    *irc;
  char    *w_next, *w_end, *cp1, *cp2, *cp3, *cp4;


  // printf("UTX_find_word1 was |%s| wo |%s|\n",was,wo);

  irc = NULL;
  w_next = wo;
  w_end  = wo + strlen(wo);

  L_weiter:

  // scan words for txtOut
  cp1 = strstr(w_next, was);
  if(cp1 == NULL) goto L_exit;


  // printf(" found Obj in DefLn hier: |%s|\n",cp1);

  // nun muss vor und hinter dem gefundenen Obj Delimiter sein!
  if(cp1 == w_next) goto L_test_c3;   // gleich an erster Stelle
  cp2 = cp1 - 1;
  // printf("  vor: |%c|\n",*cp2);


  // test ob char ein Trennzeichen ist
  cp4 = strpbrk(cp2, DelChars);
  if(cp4 != cp2) goto L_fertig;   // muss ident mit angefragter Posi sein!


  L_test_c3:
  cp3 = cp1 + strlen(was);
  // if(*cp3 == NULL) goto L_found;
  if(*cp3 == '\0') goto L_found;
  // printf("  nach: |%c|\n",*cp3);
  cp4 = strpbrk(cp3, DelChars);
  if(cp4 != cp3) goto L_fertig;


  // gefunden!
  L_found:
  irc = cp1;
  goto L_exit;



  //--------------------------------------------
  L_fertig:
  w_next = cp1 + strlen(was) + 1;
  if(w_next < w_end) goto L_weiter;



  //--------------------------------------------
  L_exit:
  // printf("ex UTX_find_word1 rc=%s |%s|\n",irc,was);
  return irc;


}


//=================================================================
  int UTX_find_wordNr (int *maxLen, char *was, char *wo) {
//=================================================================
/// \code
/// find word "was" in string "wo"; return nr of occurences
///   and max size between (max len of line).
/// \endcode


  int  iNr, ii, lwas;
  char *p1, *p2;

  // printf("UTX_find_wordNr |%s|%s|\n",was,wo);

  if(wo == NULL) return 0;


  p1  = wo;
  iNr = 0;
  *maxLen = 0;
  lwas = strlen(was);

  L_nxt:
  p2 = strstr (p1, was);

  if(p2) {
    ii = p2 - p1;
    if(ii > *maxLen) *maxLen = ii;
    p1 = p2 + lwas;
    ++iNr;
    goto L_nxt;
  }

  ii = wo + strlen(wo) - p1;
  if(ii > *maxLen) *maxLen = ii;

  // printf("ex UTX_find_wordNr %d %d\n",iNr,*maxLen);
  return iNr;

}


//================================================================
  int UTX_strcmp_right (char *string, char *text) {
//================================================================
/// \code
/// UTX_strcmp_right              check if string1 ends with string2
/// RetCode: 0=equal; else=notEqual.
/// \endcode

  int   ll;
  char  *cp1;

  // printf("UTX_strcmp_right |%s|%s|\n",string,text);


  // get length of both strings
  ll = strlen(string) - strlen(text);
  if(ll < 0) return 1;


  cp1 = &string[ll];
    // printf(" cp1=|%s| %d\n",cp1,ll);


  return strcmp(cp1, text);

}


//================================================================
  char *UTX_find_strrstr (char *cbuf, char *str) {
//================================================================
/// \code
/// find last occurence of str in cbuf
/// NULL: nicht enthalten
/// \endcode

  int    iLen;
  char   *p1, *p2;

  // printf("UTX_find_strrstr |%s|%s| \n",cbuf,str);

  p1 = strstr (cbuf, str);
  if(!p1) return NULL;     // str ist nicht in cbuf enthalten

  iLen = strlen(str);

  p2 = p1;

  while (p2) {
    p1 = p2;
    p2 = strstr (p1+iLen, str);
  }

  // printf("ex UTX_find_strrstr |%s| \n",p1);

  return p1;

}


//================================================================
  int UTX_del_foll_chrn (char *cbuf, char *str) {
//================================================================
/// \code
/// UTX_del_foll_chrn           delete last char of cbuf if it is in str
/// \endcode

  int    ii;
  char   *p1;

  ii = strlen(cbuf) - 1;
  if(ii < 0) return -1;
  p1 = &cbuf[ii];

  printf("UTX_del_foll_chrn |%s|%c|\n",cbuf,*p1);

  ii = 0;

  while(str[ii]) {
    if(*p1 == str[ii]) {
      *p1 = '\0';
      break;
    }
    ++ii;
  }

  printf("ex-UTX_del_foll_chrn |%s|\n",cbuf);

  return 0;

}


//================================================================
  char *UTX_find_strrchrn (char *cbuf, char *str) {
//================================================================
/// \code
/// UTX_find_strrchrn        find last occurence of one of the chars of str2
/// returns NULL or the position of the last char in cbuf also found in str. 
///  (see strpbrk = find first)
/// NULL: nicht enthalten
/// \endcode

  int    ii;
  char   *p1, *p2;

  // printf("UTX_find_strrchrn |%s|%s| \n",cbuf,str);


  ii = 0;
  p1 = cbuf;

  while(str[ii]) {
    p2 = strrchr (p1, str[ii]);
    if(p2) {
      // found
      p1 = ++p2;  // start here
    }
    ++ii;
  }

  if(p1) --p1;

  // printf("ex-UTX_find_strrchrn |%s| \n",p1);

  return p1;

}


//================================================================
  int UTX_cmp_word_wordtab (char *wdtab[], char *wd1) {
//================================================================
/// \code
/// check if word wd1 is in list wdtab.
/// !!! wdtab must be terminated by "" !!!!
/// 
/// Retcod:
///   -1    wd1 not in wordList wdtab
///   >=0   wd1 is wdtab[Retcod]
/// 
/// char *wdtab[]={"CUT","TRIM","nix",""};
/// (int)wNr = UTX_cmp_word_wordtab (wdtab, "TRIMM");
/// \endcode


  int i1;

  if(wd1 == NULL) return -1;
  if(*wd1 == '\0') return -1;

  // printf("UTX_cmp_word_wordtab |%s|\n",wd1);

  i1 = 0;

  while (*wdtab[i1]) {

    if(!strcmp(wdtab[i1], wd1)) {
      // printf("ex UTX_cmp_word_wordtab %d |%s|\n",i1,wdtab[i1]);
      return i1;
    }
    ++i1;
  }

  // printf("ex UTX_cmp_word_wordtab -1 |%s|\n",wd1);
  return -1;

}


//================================================================
  int UTX_ck_num_i (char *s1) {
//================================================================
/// \code
/// UTX_ck_num_i          test if word is numeric (int)
/// RetCod:  -1   s1 does NOT start with an integer
///          else nr of characters (including sign)
/// \endcode

  int     iNr=0;

  while (*s1  == ' ') { ++s1; ++iNr; }


  if((*s1 == '-')||(*s1 == '+')) { 
    ++s1;
    ++iNr;
  }

  while(isdigit(*s1)) { 
    ++s1;
    ++iNr;
  }

  if(iNr < 1) return -1;

  return iNr;

}


//===================================================================
  int UTX_ck_num_f (char **pOut, char *cbuf) {
//===================================================================
/// \code
/// UTX_ck_num_f          test if word is numeric (float)
/// +-.  und Ziffern sind numer.!
/// Output:
///   pOut      delimitchar (first char after number)
///   retCod    0   yes, numeric
///            -1   no, is text ..
/// \endcode

  int   iNr = 0;
  char  *tpos;


  // printf("UTX_ck_num_f |%s|\n",cbuf);

  tpos = cbuf;

  // zuerst leading blanks weg
  while (*tpos  == ' ')  {
    ++tpos;
  }

  // nun darf ein Vor. sein
  if((*tpos == '-')||(*tpos == '+')) {
    ++tpos;
  }

  // Vorkommastellen
  while(isdigit(*tpos)) {
    ++iNr;
    ++tpos;
  }

  // nun darf ein Punkt sein
  //if(tpos == '.') {  // 2.10.03
  if(*tpos == '.') {
    ++tpos;
  }


  // Nachkommastellen
  while(isdigit(*tpos)) {
    ++iNr;
    ++tpos;
  }

  *pOut = tpos;

  // printf("ex UTX_ck_num_f   %d|\n",iNr);
  if(iNr > 0) return 0;

  return -1;

}


//================================================================
  int UTX_ck_num_digNr (char **cpo, char *cpi) {
//================================================================
/// \code
/// returns nr of consecutive digits starting at cpi
/// first character (*cpi) should be a digit (else returns 0)
/// cpi must be terminated with '\0'
/// cpo is the first non-digit-character
/// \endcode


  int    ii;


  // printf("UTX_ck_num_digNr |%s|\n",cpi);

  *cpo = cpi;
  goto L_start;

  L_nxt:
    ++cpi;
    L_start:
      if(isdigit(*cpi)) goto L_nxt;

  ii = cpi - *cpo;
  *cpo = cpi;

    // printf("ex UTX_ck_num_digNr %d |%s|\n",ii,*cpo);

  return ii;

}


//================================================================
  int UTX_ck_uml_c (char *iChar) {
//================================================================
/// \code
/// check for Umlaut (ÄÖÜäöüß); change to normal char
/// RetCod:
///   0=kein Umlaut;
///   1=Umlaut changed to normal char.
///
/// see also UTX_ck_uml_s UTX_CleanSC UTX_safeName
/// \endcode

  int    irc;
  char   *p1;
  static char strUm1[]={"AOUaous"}, strUm2[]={"ÄÖÜäöüß"};

  p1 = strchr(strUm2,*iChar);
  if(p1) {
    *iChar = strUm1[p1-strUm2];
    return 1;
  }

  return 0;

}


//================================================================
  int UTX_ck_uml_s (char *cbuf, int *lenS, int lenMax) {
//================================================================
/// \code
/// change all umlaute of string
/// RetCod = Anzahl gefundener Umlaute
///
/// see also UTX_ck_uml_c UTX_CleanSC UTX_safeName
/// \endcode


  int    ls, iPos, iChar, iNr;
  char   *p1, *p2, ci;
  static char strUm1[]={"AOUaous"}, strUm2[]={"ÄÖÜäöüß"};

  printf("UTX_ck_uml_s |%s| %d\n",cbuf,*lenS);

  ls = *lenS;
  iNr = 0;

  L_nxt_chr:
  p1 = strpbrk (cbuf, strUm2);
  if(p1) {    // p1 = gef. char in cbuf
    if(ls >= lenMax) {TX_Print("UTX EOM"); return -1;}
    iPos = p1-cbuf;
    p2 = strchr(strUm2,*p1);
    if(p2) {
      iChar = p2-strUm2;
      printf(" |%c| %d %d\n",*p2,iPos,iChar);
      cbuf[iPos] = strUm1[iChar];
      // insert 'e' od 's'
      if(iChar < 6) ci = 'e';
      else          ci = 's';
      memmove (&cbuf[iPos+1], &cbuf[iPos], ls-iPos-1);
      ++ls;
      ++iPos;
      cbuf[iPos] = ci;
    }
    ++iNr;
    goto L_nxt_chr;
  }

  *lenS = ls;
  printf("ex UTX_ck_uml_s %d %d\n",iNr,*lenS);
  return iNr;

}


//================================================================
  int UTX_chg_2_upper (char *cbuf) {
//================================================================
/// \code
/// UTX_chg_2_upper        change string to uppercase; skip textStrings("")
/// RetCod:      nr of modified characters
/// \endcode


  int    irc = 0;
  char   *p1;

  p1 = cbuf;
  while(*p1) {
    if(*p1 == '"') {   // do not modify strings
      p1 = UTX_pos_skipStr (p1);
    } else {
      if (islower (*p1)) { *p1 = toupper (*p1); ++irc; }
    }
    ++p1;
  }

  return irc;

}


//================================================================
  int UTX_chg_2_lower (char *cbuf) {
//================================================================
/// UTX_chg_2_lower        change string to lowercase


  int    irc;
  char   *p1;

  p1 = cbuf;
  while(*p1) {
    if (isupper (*p1)) *p1 = tolower (*p1);
    // if (islower (txtin[i1])) txtout[i1] = toupper (txtin[i1]);
    ++p1;
  }

  return 0;

}


//==================================================================
  int UTX_chg_wordn (char *sOut, char *wNew, int wNr, long lenMax) {
//==================================================================
/// \code
/// change word nr wNr in string sOut into word sNew
/// word-delimiter is blank.
/// Input:
///   sOut       string with 1-n words; delimiter = blank; 0-terminated. 
///   wNew       new word, 0-terminated
///   wNr        first word is 0
///   lenMax     max. length of string sOut
/// Output:
///   sOut       word modified
/// \endcode

  int     wAct;
  long    wLen1;
  char    *pos1, *pos2;

  // printf("UTX_chg_wordn %d |%s| %ld\n",wNr,wNew,lenMax);


  // pos1 = find startpos of word nr <wNr> in sOut
  wAct = 0;
  pos1 = sOut;

  while(wAct != wNr) {
    ++wAct;
    // find next delimiter (blank)
    pos1 = strchr(pos1, ' ');
    if(!pos1) {
      TX_Print("UTX_chg_wordn E001");
      return -1;
    }
    ++pos1;  // keep the delimiter
  } 

  // wLen = length of word starting at <pos1>
  pos2 = strchr(pos1, ' ');
  if(pos2) { 
    wLen1 = pos2 - pos1;
  } else { 
    wLen1 = strlen(pos1);
  }

  // change word starting at <pos1>  (wordlength wLen1) into word <wNew>
  lenMax -= (pos1 - sOut);
    // printf(" new lenMax=%ld\n",lenMax);
  return MEM_chg_str (pos1, lenMax, wLen1, wNew);

}


//=============================================================
  void UTX_cp_word_2_upper (char* txtout, char* txtin) {
//=============================================================
/// \code
/// UTX_cp_word_2_upper     copy and change word into capitalletters.
///   Max 80 char's.
///   txtout / txtin may be same address
/// \endcode

  int  i1, i2;

  /* printf("UTX_cp_word_2_upper |%s|\n",txtin); */


  i2 = strlen(txtin);
  if (i2 > 80) {
    i2 = 80;
    txtout[80] = '\0';
  }


  /* printf("UTX_cp_word_2_upper |%d|\n",i2); */


  for (i1=0; i1<=i2; i1++) {
    if (islower (txtin[i1])) {
      /* printf(" change %d |%c|\n",txtin[i1]); */
      txtout[i1] = toupper (txtin[i1]);
    } else {
      /* printf(" copy %d |%c|\n",i1,txtin[i1]); */
      txtout[i1] = txtin[i1];
    }
  }


  /* //printf("UTX_cp_word_2_upper |%s|%s|\n",txtout,txtin); */


  return;
}


//=============================================================
  int UTX_cp_nchr_2_upper (char* txtout, char* txtin, int iNr) {
//=============================================================
/// \code
/// UTX_cp_nchr_2_upper     copy and change n chars into capitalletters.
///   txtout / txtin may be same address
/// \endcode

  int  i1;

  // printf("UTX_cp_word_2_upper |%s|\n",txtin);


  for (i1=0; i1<=iNr; i1++) {
    if (islower (txtin[i1])) {
      txtout[i1] = toupper (txtin[i1]);
    } else {
      txtout[i1] = txtin[i1];
    }
  }


  //printf("UTX_cp_word_2_upper |%s|%s|\n",txtout,txtin);


  return 0;
}


//================================================================
  char* UTX_cp_word__ (char *txtOut, char *txtIn) {
//================================================================
/// UTX_cp_word__          copy next word

  int  i1;
  char *p1, *p2;


  // printf("UTX_cp_word__ |%s|\n",txtIn);


  if(txtIn == NULL) {*txtOut = '\0'; return txtIn;}
  if(txtIn == '\0') {*txtOut = '\0'; return txtIn;}


  // zuerst leading blanks skippen
  while (*txtIn  == ' ') ++txtIn;

  // terminator suchen
  p1 = strpbrk(txtIn, " =,;()\"\n\t\r{}[]");
  if(p1 == NULL) {
    i1 = strlen(txtIn);
    p1 = &txtIn[i1];
  } else {
    i1 = p1 - txtIn;
  }
    // printf(" i1=%d\n",i1);


  strncpy(txtOut, txtIn, i1);
  txtOut[i1] = '\0';

  // following blanks skippen
  while (*p1  == ' ') ++p1;

  // printf("ex UTX_cp_word__ |%s|\n",txtOut);

  return p1;

}


//================================================================
  char UTX_cp_word_t (char *txtOut, char **cBuf) {
//================================================================
/// \code
/// UTX_cp_word_t          copy next word, give terminator
/// see UTX_cp_word__ UTX_find_Del1 UTX_pos_del
/// 
/// Outputs:
///   Returns the delimiter found as single character
///   cBuf:   word, Terminator & following blanks skipped
///   txtOut: word (no leading/following blanks, no terminator) Max 252 chars.
///           can be empty (*txtOut == '\0')
/// 
/// Delimiterzeichen sind:
///  "'()*+,-/;<=>  und blank
/// 
/// Usage-example:
///   char cBuf[256], tc;
///   L_nxt:   tc = UTX_cp_word_t (cBuf, &tPos);
///     if(cBuf[0] == '\0') return 0;  // outputstring empty
///     ..
///     if(tc != '\0') goto L_nxt;     // inputstring empty
/// 
/// Nachfolgend kann man das Wort abtrennen durch
///   *cp2 = '\0';
/// \endcode


  int  i1;
  char *p1, *p2, tc;


  // printf("UTX_cp_word_t |%s|\n",txtIn);

  p1 = *cBuf;

  if(p1 == NULL) {tc = '\0'; p2 = p1; goto L_exit;}
  if(p1 == '\0') {tc = '\0'; p2 = p1; goto L_exit;}

  // zuerst leading blanks skippen
  while (*p1  == ' ') ++p1;

  // terminator suchen
  p2 = strpbrk(p1, " =,;()\"\n\t\r{}[]");
  if(p2 == NULL) {
    tc = '\0';
    i1 = strlen(p1);
  } else {
    tc = *p2;
    i1 = p2 - p1;
  }

  if(i1 > 252) i1 = 252;
  strncpy(txtOut, p1, i1);
  txtOut[i1] = '\0';

  // term & following blanks skippen
  if(tc != '\0')
    UTX_skip_1bl (&p2);

  L_exit:
  *cBuf = p2;
  // printf("ex UTX_cp_word_t |%s|%c|%s|\n",txtOut,tc,p2);
  return tc;

}


//=============================================================
  char* UTX_cp_word_term (char* txtOut, char* txtIn, char term) {
//=============================================================
/// \code
/// UTX_cp_word_term       copy word from startpos to (excluding) term
/// Returncode:
///   position after terminating character;
///   '\0', if terminating character <term> does not exist.
/// \endcode


  int     isl;
  char    *pe;

  // printf("UTX_cp_word_term |%s|%c|\n",txtIn,term);


  pe = strchr (txtIn, term);
  if(!pe) {
    // string ends with '\0'
    strcpy(txtOut, txtIn);
    isl = strlen(txtIn);
    goto L_exit;

  } else {
    isl = pe - txtIn;
  }
    // printf(" isl=%d\n",isl);


  strncpy (txtOut, txtIn, isl);
  txtOut[isl] = '\0';        //  terminate txtOut
  ++isl;

  L_exit:
    pe = &txtIn[isl];
      // printf("exit UTX_cp_word_term |%s|%s|\n",txtOut,pe);
  return (pe);
}


/*
//=============================================================
  char* UTX_cp_word_term (char* txtOut, char* txtIn, char term) {
//=============================================================
/// \code
/// UTX_cp_word_term       copy word bis zum Terminator.
/// returns Pos. HINTER dem Terminator.
/// Der term.Character wird nicht nach txtOut kopiert.
/// Wenn der Terminator nicht gefunden wird, wird auf das LineEnd gezeigt.
/// \endcode

// Koennte man schneller machen mit strchr und memcpy ...


  char    *iPos, *oPos;

  // printf("UTX_cp_word_term |%s|%c|\n",txtIn,term);

  iPos = txtIn;
  oPos = txtOut;

  // strcpy (txtOut, txtIn);


  while (*iPos != term) {

    *oPos = *iPos;

    // printf("next:|%c|%c|\n",*iPos,term);
    // ++i1;    if(i1 > 100) break;
    if(*iPos == '\0') {
      // printf("Error UTX_cp_word_term: Zeichen %c nicht gef.\n",term);
      --iPos;
      break;
    }
    ++iPos;
    ++oPos;
  }

  ++iPos;               //  skip auch den term
   *oPos = '\0';        //  terminate txtOut

  // printf("exit UTX_cp_word_term |%s|%s|\n",iPos,txtOut);

  return iPos;
}
*/

//================================================================
  char* UTX_cp_expr (char *cbo, char *cbi) {
//================================================================
/// \code
/// UTX_cp_expr        copy expression (without closing bracket)
/// !!! cbi may not include starting bracket !!!
/// cbi: |3 + 2)|            cbo: |3 + 2|      RetCod: ||
/// cbi: |4+(2*4))+2|        cbo: |4+(2*4)|    RetCod: |+2|
/// cbi: |sin(4+(2*4))+2|    cbo: |sin(4+(2*4)|    RetCod: |+2|
/// RetCode:
///   position of first char after closing bracket.
///   NULL     Error
/// \endcode

  int    il;
  char   *p1, *p2;


  // printf("UTX_cp_expr |%s|\n",cbi);

  // // find starting bracket
  // p1 = strchr(cbi, '(');
  // if(!p1) return NULL;
  // ++p1;
  p1 = cbi;

  p2 = UTX_pos_skipBrack1 (p1); // p1 points to corresponding ')'
  if(!p2) return NULL;
    // printf(" p1=|%s| p2=|%s|\n",p1,p2);


  il = p2 - p1;

  strncpy(cbo, p1, il);
  cbo[il] = '\0';
  ++p2;

    // printf("ex UTX_cp_expr cbo=|%s| rc=|%s|\n",cbo,p2);

  return p2;

}


//================================================================
  char* UTX_CleanBracks (char* txtbuf, char ch1, char ch2) {
//================================================================
/// \code
/// UTX_CleanBracks        Remove first/last char (brackets ..)
/// ACHTUNG: veraendert txtbuf; ueberschreibt ev. last char.
/// Example:
///   cp1 = UTX_CleanBracks (cp1, '(', ')');
/// \endcode

  char *p1;

  p1 = txtbuf;
  if(*p1 == ch1) ++p1;


  if (p1[strlen(p1)-1] == ch2) {
    p1[strlen(p1)-1] = '\0';
  }


  return p1;

}


//================================================================
  int UTX_chg_chr1 (char newChr, char oldChr, char *cBuf){
//================================================================
/// in cBuf alle oldChr aendern in newChr  (Replace)

  char   *p1;

  // printf("UTX_chg_chr1 %c %c |%s|\n",newChr,oldChr,cBuf);

  p1 = cBuf;

  L_next:
    p1 = strchr (p1, oldChr);

    if(p1) {
      *p1 = newChr;
      ++p1;
      goto L_next;
    }


  return 0;

}


//================================================================
  int UTX_chg_str1 (long *datSiz,
                    void *datStart, void *insDat, void *delPos, long delSiz) {
//================================================================
/// \code
/// UTX_chg_str1          change a part in a string
/// string (datStart) must be terminated with '\0'
/// 
/// Input:
///   datStart     Startposition des String
///   insDat       String to insert
///   delPos       Position where to delete and where to insert
///   delSiz       how much bytes to delete at Pos delPos
/// Output:
///   datSiz       new size of string (datStart)
/// 
/// 
///  strcpy(cbuf, "1234567890");   l1 = strlen(cbuf);
///  UTX_chg_str1 (&l1, cbuf, "AAA", &cbuf[4], 1); // 5 -> AAA
/// \endcode

  long    li;


  *datSiz = strlen(datStart);

  li = strlen(insDat);

  MEM_chg_rec (datStart, datSiz, insDat, li, delPos, delSiz);


  return 0;

}


//================================================================
  int UTX_chg_str2 (int sSiz, char *sDat, char *sOld, char *sNew) {
//================================================================
/// \code
/// UTX_chg_str2                 change <sOld> into <sNew> in <sDat>
/// RetCod:
///   -1     Error; sDat too small
///    0     OK; modified ..
///    1     OK but sOld did not exist in sDat
/// \endcode
  
// see also UTX_chg_str1
  
  char    *p1;
  long    lOld, lNew, lDat;


  // check if sOld exists in sDat
  p1 = strstr (sDat, sOld);
  if(p1 == NULL) return 1;
  
  // ok, change ..
  lDat = strlen (sDat);
  lOld = strlen (sOld);
  lNew = strlen (sNew);

  if((lDat + lNew - lOld) >= sSiz) return -1;

  MEM_chg_rec (sDat, &lDat,
               sNew, lNew,
               p1,   lOld);

  
  return 0;

}


//================================================================
  int UTX_chg_right (char *s1, char *s2) {
//================================================================
/// \code
/// UTX_chg_right          change right side of string (replace equal nr of chars)
/// change the right side of string s1 into s2
/// \endcode

  long  ls1, ls2;


  // printf("UTX_chg_right |%s|%s|\n",s1,s2);

  ls2 = strlen(s2);

  ls1 = strlen(s1);

  if(ls1 < ls2) return -1;

  strcpy(&s1[ls1 - ls2], s2);

    // printf("ex UTX_chg_right |%s|\n",s1);

  
  return 0;

}


//================================================================
  int UTX_chg_nr (char *txtOut, int iNr) {
//================================================================
/// \code
/// change int-number in line beginning at txtOut into iNr
/// Returncode = Laengendifferenz vorher - nachher
/// \endcode

  int    iLen, oldLen;
  char   *p1, cbuf[32];


  // printf("UTX_chg_nr |%s| %d\n",txtOut,iNr);


  sprintf(cbuf, "%d", iNr);
  iLen = strlen(cbuf);

  // find pos of first non-numeric character
  p1 = txtOut;
  UTX_pos_skip_int (&p1);

  // von rechts nach links !
  memmove (&txtOut[iLen], p1, strlen(p1));

  strncpy(txtOut, cbuf, iLen);

  oldLen = p1 - txtOut;

  // printf("ex UTX_chg_nr |%s| %d %d\n",txtOut,iLen,oldLen);

  return iLen - oldLen;

}


//=============================================================
  int UTX_del_right (char* txtbuf, int cNr) {
//=============================================================
/// UTX_del_right          delete last characters

  txtbuf[strlen(txtbuf)-cNr] = '\0';

  return 1;

}


//=============================================================
  int UTX_endDelChar (char* txtbuf, char chr) {
//=============================================================
/// UTX_endDelChar           if last char of string == chr: remove it.


  if (txtbuf[strlen(txtbuf)-1] == chr) {
    txtbuf[strlen(txtbuf)-1] = '\0';
  }

  /* printf("UTX_endDelChar |%s| %c\n", txtbuf,chr); */

  return 1;

}


//================================================================
  int UTX_endDelWord (char *s1, char deli, int mode) {
//================================================================
/// \code
/// UTX_endDelWord         remove last word; delimiting char; keep or not.
/// Input:
///   deli    delimiting char
///   mode    0=keep delimiting char; 1=remove also delimiting char.
/// RetCod:
///   -1      no deli in s1
/// \endcode

    char    *p1;


  // printf("UTX_endDelWord |%s| %c %d\n",s1,deli,mode);


    // find last word delimited by 'deli'
    p1 = strrchr(s1, deli);

    if(p1 == NULL) return -1;


    if(!mode) ++p1;    

    *p1 = '\0';    // cut word
    
  // printf("ex UTX_endDelWord |%s| %d\n",s1,mode);

 
  return 0;

}


//================================================================
  int UTX_endAddChar (char* txtbuf, char chr) {
//================================================================
/// UTX_endAddChar           if last char of string != chr: add it.

  int   i1;

  i1 = strlen(txtbuf);
  if (txtbuf[i1-1] != chr) {
    txtbuf[i1] = chr;
    txtbuf[i1+1] = '\0';
  }

  // printf("UTX_endAddChar |%s| %c\n", txtbuf,chr);

  return 1;

}


//==============================================================
  char* UTX_pos_1n (char string[]) {
//==============================================================
/// \code
/// UTX_pos_1n             skip blanks (returns Position of first non-blank)
/// ersetzt durch  UTX_pos_skipLeadBlk
/// \endcode


  char *tpos;

  tpos = string;

  while (*tpos  == ' ') ++tpos;

  /* printf("UTX_pos_1n |%s|\n",tpos); */

  return tpos;

}


//==============================================================
  char UTX_pos_del (char** txtIn, char** w_next) {
//==============================================================
/// \code
/// UTX_pos_del            find next delimiter (blank or "'()*+,-/;<=>)
/// 
/// Input:
///   txtIn:  startpos
/// Output:
///   txtIn:  leading blanks skipped
///   w_next: the pos of the deli
///   RetCod: the delimiter as single character
/// 
/// Delimiterzeichen sind:
///  " ""'()*+,-/;<=>"
/// 
/// Beispiel:
///   char *cp1, *cp2, deli;
///   deli=UTX_pos_del(&cp1, &cp2);
/// 
/// Nachfolgend kann man das Wort abtrennen durch
///   *cp2 = '\0';
/// \endcode


/*
Das folgende ist NICHT aktiv:
  Wird ein Blank als Delimiter gefunden, wir noch das naechste Zeichen,
  das != Blank ist, gesucht. Ist diesen ebenfals ein Delimiterzeichen,
  so wird dieses als Delimiter akzeptiert, sonst bleibts beim Blank.
  Es wurde ein Blank als Delimiter gefunden.
  Nun wird noch das naechste Zeichen, das != Blank ist, gesucht.
  Ist diesen ebenfalls ein Delimiterzeichen,
  so wird dieses als Delimiter akzeptiert, sonst bleibts beim Blank.   

*/

  /* int  deltyp; */
  char *tpos, deli;

  /* ------------------------------------------------------------ */

  tpos = *txtIn;



  /* zuerst leading blanks skippen */
  for(;;) {
    if(*tpos != ' ') {
      break;
    }
    ++tpos;
  }
  *txtIn = tpos;

  

  while (*tpos != '\0') {
    /* printf("UTX_pos_del |%c|\n",*tpos); */
    if(strchr(" \"'()*+,-/;<=>", *tpos) != NULL) goto Found;
    ++tpos;
  }




  /* Stringende ist \0 */
  deli = '\0';
  *w_next = tpos;
  goto Fertig;



  /*-----------------------------------------------------------------------*/
  Found:
  /* deltyp = 1; */
  /* skip this delimiter */
  deli = *tpos;
  /* ++tpos; */
  *w_next = tpos;



  /*-----------------------------------------------------------------------*/
  Fertig:
  /* printf("UTX_pos_del |%c|%s|\n",deli,tpos); */

  /* return deltyp; */
  return deli;

}


//==============================================================
  char UTX_pos_delP (char **delPos, char* startPos) {
//==============================================================
/// \code
/// UTX_pos_delP            find next program-delimiter (',' or ')')
/// 
/// Input:
///   startPos:  startpos
/// Output:
///   delPos:    the pos of the deli
/// RetCod:      the delimiter as single character
/// 
/// \endcode


  *delPos = strpbrk(startPos, ",)");

  if(*delPos) return **delPos;
  else        return 0;

}


//=====================================================================
  long UTX_pos_del_prev (char *cBuf, long cPos) {
//=====================================================================
/// \code
/// find previous delimiter (blank vor cPos)
/// Return pos of first char of word before cPos
/// Klammerausdruecke skippen !
/// ".. MOD (2)" ??
/// \endcode

  int   cNr;
  long  aPos;
  char  c1;


  // printf("UTX_pos_del_prev |%s| %ld\n",cBuf,cPos);

  aPos = cPos-1;

  // skip following blanks
  L_start:
  if(aPos <= 0) goto L_fertig;
  c1 = cBuf[aPos];
  if(c1 == ' ')  { --aPos; goto L_start; }

  // --aPos; // skip last char of word



  // aPos ist nun der erste Char != ' '; vor bis next ' '
  L_next:
  if(aPos <= 0) goto L_fertig;
  c1 = cBuf[aPos];
  // printf(" 1 %d |%c|\n",aPos,c1);

  // eine Klammer skippen
  if(c1 == ')')  {
     cNr = 1;
     L_skip:
     --aPos;
     if(aPos <= 0) { TX_Error("UTX_pos_del_prev E001"); goto L_fertig; }
     c1 = cBuf[aPos];
     // printf(" 2 %d |%c| %d\n",aPos,c1,cNr);
     if(c1 == ')') ++cNr;
     if(c1 != '(') goto L_skip;
     --cNr;
     if(cNr > 0) goto L_skip;
  }

  if(c1 != ' ')  { --aPos; goto L_next; }


  // aPos ist nun ein Blank;
  ++aPos;

  // aPos ist derBeginn des Wortes vor cPos
  L_fertig:

  // printf("ex UTX_pos_del_prev %ld |%s|\n",aPos,&cBuf[aPos]);

  return aPos;

}


//=====================================================================
  long UTX_pos_del_next (char *cBuf, long cPos) {
//=====================================================================
/// \code
/// find next delimiter (blank nach cPos)
/// Return pos of first char of word after cPos
/// Input:
///   cBuf     string
///   cPos     startpos as long
/// Output:
///   RetCod:  position of blank in cBuf as long
///
/// Klammerausdruecke skippen !
/// \endcode


  int   cNr;
  long  aPos, end;
  char  c1;


  printf("UTX_pos_del_next |%s| %ld\n",cBuf,cPos);

  end = strlen(cBuf) + 1;


  aPos = cPos;

  // skip following blanks
  L_start:
  if(aPos >= end) goto L_fertig;
  c1 = cBuf[aPos];
  if(c1 == ' ')  { ++aPos; goto L_start; }



  // aPos ist nun der erste Char != ' '; vor bis next ' '
  L_next:
  if(aPos >= end) goto L_fertig;
  c1 = cBuf[aPos];

  // eine Klammer skippen
  if(c1 == '(')  {
     cNr = 1;
     L_skip:
     ++aPos;
     if(aPos >= end) { TX_Error("UTX_pos_del_next E001"); goto L_fertig; }
     c1 = cBuf[aPos];
     if(c1 == '(') ++cNr;
     if(c1 != ')') goto L_skip;
     --cNr;
     if(cNr > 0) goto L_skip;
  }

  if(c1 != ' ')  { ++aPos; goto L_next; }



  // aPos ist nun ein Blank;
  L_finish:
  if(aPos >= end) goto L_fertig;
  c1 = cBuf[aPos];
  if(c1 == ' ')  { ++aPos; goto L_finish; }



  // aPos ist derBeginn des Wortes vor cPos
  L_fertig:

  printf("ex UTX_pos_del_prev %ld |%s|\n",aPos,&cBuf[aPos]);

  return aPos;

}


//==============================================================
  int UTX_add_i_u (char strBuf[], int zahl) {
//==============================================================
/// UTX_add_i_u            add integer to string


  sprintf (&strBuf[strlen(strBuf)],"%d", zahl);

  return 0;

}



//================================================================
  int UTX_add_db (char outBuf[], double *db1) {
//================================================================
/// add double unformatted; Trennzeichen ist blank.

  if(outBuf[strlen(outBuf)-1] != ' ')
    strcat (outBuf, " ");

  UTX_add_fl_u (outBuf, *db1);

  return 0;

}


//================================================================
  int UTX_add_pt (char outBuf[], void *pt1) {
//================================================================
/// add Pointcoords unformatted; Trennzeichen ist blank.

  if(outBuf[strlen(outBuf)-1] != ' ')
    strcat (outBuf, " ");

  UTX_add_fl_u (outBuf, ((Point*)pt1)->x);
  strcat (outBuf, " ");
  UTX_add_fl_u (outBuf, ((Point*)pt1)->y);
  strcat (outBuf, " ");
  UTX_add_fl_u (outBuf, ((Point*)pt1)->z);

  return 0;

}


//================================================================
  int UTX_del_chr (char *cbuf, char cx) {
//================================================================
/// UTX_del_chr      delete all characters <cx> out of string cbuf

  char   *p1, *p2;

  // printf("UTX_del_chr |%s|%c|\n",cbuf,cx);

  p1 = cbuf;
  p2 = cbuf;

  while (*p1 != '\0') {       // p1=ReadPos; p2=WritePos.
    // printf(" |%c|\n",*p1);
    if(*p1 != cx) {
      if(p1 != p2) *p2 = *p1;
      ++p2;
    }
    ++p1;
  }

  if(p1 != p2) *p2 = '\0';

  // printf("ex UTX_del_chr |%s|%c|\n",cbuf,cx);

  return 0;

}


//================================================================
  int UTX_del_follBl (char *cbuf) {
//================================================================
/// \code
/// UTX_del_follBl             delete following blanks, return strLen
/// see also UTX_CleanCR
/// \endcode

  char *cpos;


  cpos = strchr (cbuf, '\0');
  goto L_weiter;


  L_next_char:
  if(*cpos == ' ') *cpos = '\0';
  else goto L_exit;


  L_weiter:
  --cpos;
  if(cpos >= cbuf) goto L_next_char;

  L_exit:
    printf(" UTX_del_follBl |%s|\n",cbuf);

  return (cpos - cbuf + 1);

}


//==============================================================
  int UTX_del_foll0 (char strBuf[]) {
//==============================================================
/// \code
///   Delete following 0's and following ".".
/// \endcode

//   Darf aber die letzte Null nicht wegloeschen !


  char *tpos;

  // printf("UTX_del_foll0 |%s|\n",strBuf);


  // search for '.'; if none: nothing to do ..
  tpos = strchr (strBuf, '.');
  if(!tpos) return 0;

  // delete following 0's
  tpos = &strBuf[strlen (strBuf)];
  --tpos;
  while (*tpos == '0') --tpos;

  if(*tpos == '.') goto L_done;               // delete following .

  // if(isdigit(*tpos)) {++tpos; goto L_done;}   // keep this dig; eg "0.5"
  // // now we have the char before '0'; must keep this 0 !
  // ++tpos;
  ++tpos;

  L_done:
  *tpos = '\0';

  // printf("ex UTX_del_foll0 |%s|\n",strBuf);

  return 1;
}


//================================================================
  int UTX_del_FilTx (char *dtxt, char *filNam) {
//================================================================
/// \code
/// delete all lines containing <dtxt>
/// uses file <filNam>.tmp
/// \endcode


  FILE  *fpi, *fpo;
  char  cBuf[256], tempFilNam[260], *p1;


  // printf("UTX_del_lNr |%s|%s| %d\n",dtxt,filNam);
 


  if((fpi=fopen(filNam,"r")) == NULL) {
    sprintf(cBuf, "*** OPEN ERROR FILE %s ",filNam);
    TX_Write (cBuf);
    return -1;
  }


  // fix tempFilNam
  strcpy(tempFilNam, filNam);
  strcat(tempFilNam, ".tmp");



  if((fpo=fopen(tempFilNam,"w")) == NULL) {
    sprintf(cBuf, "*** OPEN ERROR FILE %s ",tempFilNam);
    TX_Write (cBuf);
    return -1;
  }



  // offnen, zeilenweise lesen und sofort wieder raus - skip Line lNr
  while (!feof (fpi)) {

    if (fgets (cBuf, 250, fpi) == NULL) break;

    p1 = strstr(cBuf, dtxt);
    if(p1) continue;             // skip Line

    fprintf(fpo, "%s",cBuf);     // write all other lines
  }

  fclose(fpi);
  fclose(fpo);


  // rename
  OS_file_delete (filNam);
  rename (tempFilNam, filNam);

  return 0;

}


//==============================================================
  int UTX_add_fl_u (char strBuf[], double zahl) {
//==============================================================
/// \code
/// UTX_add_fl_u            add double unformatted (del foll. 0's and ".")
/// 
///   Delete following 0's and following ".".
/// see UTX_add_fl_f UTX_db10__
/// \endcode


  double u1;

  // Die Zahl -0.0 auf 0.0 korrigieren
  u1 = fabs(zahl);
  if(u1 < FLT_32_MIN1) { strcat (strBuf, "0"); return 0; }
  if(u1 > FLT_32_MAX)  {
      if(zahl < 0.) { strcat (strBuf, "-9999999999999.999"); return -1; }
    else            { strcat (strBuf, "9999999999999.999"); return 1; }
                                  //   123456789012345678
  }


  sprintf (&strBuf[strlen(strBuf)],"%lf", zahl);

  // delete following 0's
  UTX_del_foll0 (strBuf);


  // printf("UTX_add_fl_u |%s|\n",strBuf);

  return 0;
}



//==============================================================
  int UTX_add_fl_u2 (char outBuf[], double z1, double z2) {
//==============================================================
/// \code
/// UTX_add_fl_u2          add double,double unformatted (del foll. 0's and ".")
/// 2 Doubles (Komma dazwischen)
/// \endcode


  UTX_add_fl_u (outBuf, z1);
  strcat (outBuf,",");
  UTX_add_fl_u (outBuf, z2);

  return 1;

}


//==============================================================
  int UTX_add_fl_u3 (char outBuf[],
                     double z1, double z2, double z3, char chDel) {
//==============================================================
/// \code
/// UTX_add_fl_u3          add 3 doubles unformatted (del foll. 0's and ".")
///   chDel    delimiting character; eg blank ..
/// 3 Doubles (Komma dazwischen)
/// \endcode

  int    i1;
  char   se[]=" ";


  se[0] = chDel;

  UTX_add_fl_u (outBuf, z1);
  // strcat (outBuf,",");
  strcat (outBuf, se);

  UTX_add_fl_u (outBuf, z2);
  // strcat (outBuf,",");
  strcat (outBuf, se);

  UTX_add_fl_u (outBuf, z3);

  return 1;

}


//====================================================================
  int UTX_add_fl_ua (char outBuf[], double *da, int dNr, char chDel) {
//====================================================================
/// \code
/// UTX_add_fl_ua          add n doubles unformatted (del foll. 0's and ".")
///   chDel    delimiting character; eg blank ..
/// \endcode

  int    i1;
  char   se[]=" ";


  se[0] = chDel;

  UTX_add_fl_u (outBuf, da[0]);

  for(i1=1; i1<dNr; ++i1) {
    strcat (outBuf, se);
    UTX_add_fl_u (outBuf, da[i1]);
  }

  return 1;

}


//================================================================
  char* UTX_add_fl_1uf (char *outBuf, double d1) {
//================================================================
/// \code
/// UTX_add_fl_1uf         add double with max.1 digit after dec.point
/// unformatted (remove following ".0")
/// retCod = position of closing '\0'
/// see also UTX_add_fl_f
/// \endcode
    

  long   l1;
  char   auxBuf[40], *p1;
  double u1;
    
    
  // Die Zahl -0.0 auf 0.0 korrigieren
  u1 = fabs(d1);

  p1 = &outBuf[strlen(outBuf)];  // pointer at \0
    
  if(u1 < FLT_32_MIN1) {
    strcpy (p1, "0");
    ++p1;
    return p1;

  } else if(u1 > FLT_32_MAX)  {
      if(d1 < 0.) { strcpy (p1, "-9999999999999.999"); p1 += 18; return p1; }
    else          { strcpy (p1, "9999999999999.999"); p1 += 17; return p1; }
                            //   1234567890123456789
  }

  sprintf (p1, "%.1f", d1);       //   printf(" _fl_1uf-|%s|\n",outBuf);


  // remove ".0" if last char = "0"
  l1 = strlen(p1);
  if(p1[l1 - 1] == '0') { l1 -= 2; p1[l1] = '\0'; }

  
  return &p1[l1];

}


//==============================================================
  int UTX_add_fl_f (char strBuf[], double zahl, int nkAnz) {
//==============================================================
/// \code
/// UTX_add_fl_f            add double with <nkAnz> digits after dec.point
///   to string. The nr of digits before dec.point is floating.
///
/// IGES verwendet %.10f !
/// 
/// see UTX_del_foll0
/// \endcode


  char   auxBuf[40], fmtBuf[16];
  double u1;


  // Die Zahl -0.0 auf 0.0 korrigieren
  u1 = fabs(zahl);
  if(u1 < FLT_32_MIN1) { strcat (strBuf, "0"); return 0; }
  if(u1 > FLT_32_MAX)  {
      if(zahl < 0.) { strcat (strBuf, "-9999999999999.999"); return -1; }
    else            { strcat (strBuf, "9999999999999.999"); return 1; }
                                  //   123456789012345678
  }


  // zuerst einmal den Formatstring generieren
  // sprintf (fmtBuf, "%%20.%df", nkAnz);
  // sprintf (fmtBuf, "%%.%df", nkAnz);
    // printf("fmtBuf = %s\n", fmtBuf);
  // sprintf (auxBuf, fmtBuf, zahl);

  sprintf (auxBuf, "%.*f", nkAnz, zahl);


  // strcat (strBuf, UTX_pos_1n(auxBuf));
  strcat (strBuf, auxBuf);

  // printf("UTX_add_fl_f |%s|\n",strBuf);

  return 0;
}


//================================================================
  char* UTX_db10__ (char s1[], double d1) {
//================================================================
/// \code
/// UTX_db10__             double -> string; precision 10 digits
/// remove following blanks and ending '.'
/// returns position of closing 0
/// size of s1 > 32
/// \endcode

// Double hat 15-17 signifikante Stellen.


  char    *tpos;
  double  u1;


  // printf("UTX_db10__ ||%f\n",s1,d1);

  u1 = fabs(d1);

  if(u1 < FLT_32_MIN1)              {
    strcpy (s1, "0");
    tpos = &s1[1];
    goto L_exit;
  } else if(u1 < 1.)                       {
    sprintf (s1, "%.10lf", d1);
  } else if(u1 < 10.)               {
    sprintf (s1, "%.9lf", d1);
  } else if(u1 < 100.)              {
    sprintf (s1, "%.8lf", d1);
  } else if(u1 < 1000.)             {
    sprintf (s1, "%.7lf", d1);
  } else if(u1 < 10000.)            {
    sprintf (s1, "%.6lf", d1);
  } else if(u1 < 100000.)           {
    sprintf (s1, "%.5lf", d1);
  } else if(u1 < 1000000.)          {
    sprintf (s1, "%.4lf", d1);
  } else if(u1 < 10000000.)         {
    sprintf (s1, "%.3lf", d1);
  } else if(u1 < 100000000.)        {
    sprintf (s1, "%.2lf", d1);
  } else if(u1 < 1000000000.)       {
    sprintf (s1, "%.1lf", d1);
  } else {
    if(u1 > FLT_32_MAX) {
      // TX_Error("UTX_add_fl_10 E001");
      if(d1 < 0.) {strcpy (s1, "-9999999999999.999"); tpos = &s1[18];}
      else        {strcpy (s1, "9999999999999.999");  tpos = &s1[17];}
                           //   123456789012345678
      goto L_exit;
    }
    sprintf (s1, "%.0lf", d1);
  }

  //----------------------------------------------------------------
  // remove following zeros and '.' as last char. See UTX_del_foll0
  tpos = strchr (s1, '.');
  if(!tpos) goto L_exit;
  tpos = &s1[strlen (s1)];
  --tpos;
  while (*tpos == '0') --tpos;
  if(*tpos == '.') --tpos;
  ++tpos;
  *tpos = '\0';
    // printf("  fl_15-nd=%d |%s|\n",nd,s1);


  //----------------------------------------------------------------
  L_exit:

    // printf("ex UTX_db10__ |%s| %lf\n",s1,d1);

  return tpos;

}


//================================================================
  char* UTX_db15__ (char s1[], double d1) {
//================================================================
/// \code
/// UTX_db15__             double -> string; precision 15 digits
/// remove following blanks and ending '.'
/// size of s1 > 40
/// returns position of closing 0
/// \endcode

// Double hat 15-17 signifikante Stellen.


  int     nd;
  char    *tpos;
  double  u1;


  // printf("UTX_db15__ |%f|\n",d1);

  // get nd = nr of digits right of comma
  u1 = fabs(d1);

  if(u1 < FLT_32_MIN1) { strcpy (s1, "0"); return &s1[1]; }

  if(u1 >= 1.) {
    // nd = 15 - log10(fabs(d1));
    nd = log10(u1);
    if(nd > 37) {
      if(d1 < 0.)
        {strcpy(s1, "-99999999999999999999999999999999999999");  tpos = &s1[39];}
      else
        {strcpy(s1, "99999999999999999999999999999999999999");   tpos = &s1[38];}
                //   1234567890123456789012345678901234567890
                //            1         2         3         4
      goto L_exit;
    }
    nd = 15 - nd;
    if(nd < 0) nd = 0;
  } else nd = 15;

  // encode (dynamic-format)
  sprintf (s1, "%.*lf",nd,d1);
    // printf("  fl_15-nd=%d %s\n",nd,s1);


  //----------------------------------------------------------------
  // remove following zeros and '.' as last char. See UTX_del_foll0
  tpos = strchr (s1, '.');
  if(!tpos) goto L_exit;
  tpos = &s1[strlen (s1)];
  --tpos;
  while (*tpos == '0') --tpos;
  if(*tpos == '.') --tpos;
  ++tpos;
  *tpos = '\0';
    // printf("  fl_15-nd=%d |%s|\n",nd,s1);

  //----------------------------------------------------------------
  L_exit:

  // printf("ex UTX_db15__ %3d |%s|\n",nd,s1);

  return tpos;

}


//================================================================
  int UTX_add_fl_10 (char strBuf[], double d1) {
//================================================================
/// \code
/// UTX_add_fl_10          add double with 10 signif digits
/// remove following blanks and ending '.'
/// free size of strBuf > 32
/// \endcode

// Double hat 15-17 signifikante Stellen.


  char    s1[80];   // *tpos;
  // double  u1;


  // printf("UTX_add_fl_10 ||%f\n",strBuf,d1);


  UTX_db10__ (s1, d1);
  strcat (strBuf, s1);

  // printf("ex UTX_add_fl_10 |||%f\n",strBuf,cBuf);

  return 0;

}

/*
//================================================================
  int UTX_add_fl_15 (char strBuf[], double d1) {
//================================================================
/// UTX_add_fl_15          add double with 15 signif digits

// Double hat 15-17 signifikante Stellen.


  char  cBuf[32], *tpos;


  // printf("-|%f|\n",d1);


  if(d1 < 10.)                       {
    sprintf (cBuf, "%.15f", d1);
  } else if(d1 < 100.)               {
    sprintf (cBuf, "%.14f", d1);
  } else if(d1 < 1000.)              {
    sprintf (cBuf, "%.13f", d1);
  } else if(d1 < 10000.)             {
    sprintf (cBuf, "%.12f", d1);
  } else if(d1 < 100000.)            {
    sprintf (cBuf, "%.11f", d1);
  } else if(d1 < 1000000.)           {
    sprintf (cBuf, "%.10f", d1);
  } else if(d1 < 10000000.)         {
    sprintf (cBuf, "%.9f", d1);
  } else if(d1 < 100000000.)         {
    sprintf (cBuf, "%.8f", d1);
  } else if(d1 < 1000000000.)       {
    sprintf (cBuf, "%.7f", d1);
  } else if(d1 < 10000000000.)       {
    sprintf (cBuf, "%.6f", d1);
  } else if(d1 < 100000000000.)     {
    sprintf (cBuf, "%.5f", d1);
  } else if(d1 < 1000000000000.)   {
    sprintf (cBuf, "%.4f", d1);
  } else if(d1 < 10000000000000.)   {
    sprintf (cBuf, "%.3f", d1);
  } else if(d1 < 100000000000000.)   {
    sprintf (cBuf, "%.2f", d1);
  } else if(d1 < 1000000000000000.) {
    sprintf (cBuf, "%.1f", d1);
  } else {
    sprintf (cBuf, "%f",   d1);
    tpos = &cBuf[15];
    while (*tpos  != '.') ++tpos;
    goto L_done;
  }

  tpos = &cBuf[16];

  tpos = &cBuf[16];

  // delete following 0's
  while (*tpos  == '0') --tpos;

  // delete following .
  if(*tpos == '.') --tpos;

  ++tpos;

  L_done:
  *tpos = '\0';

  strcat (strBuf, cBuf);

  // printf("ex UTX_add_fl_15 |%s|\n",cBuf);

  return 0;

}
*/


//================================================================
  int UTX_add_fl_15 (char strBuf[], double d1) {
//================================================================
/// \code
/// UTX_add_fl_15          add double with 15 signif digits
/// free size of strBuf > 40
/// remove following blanks and ending '.'
/// \endcode



  int   nd;
  char  s1[80], *tpos;


  // printf("UTX_add_fl_15 |%f|\n",d1);

  UTX_db15__ (s1, d1);
  strcat (strBuf, s1);

  // printf("ex UTX_add_fl_15 |%s|\n",strBuf);

  return 0;

}


//================================================================
  int UTX_add_fnam_del (char *cbuf) {
//================================================================
/// \code
/// UTX_add_fnam_del       add closing "/" or "\\" to string (filename-delimiter)
/// see also UTX_endAddChar
/// \endcode


  char *ps;

  // wenn cbuf kein closing "/" hat, eins zufuegen.
  ps = &cbuf[strlen(cbuf)-1];

#ifdef _MSC_VER
  // MS: CR-LF !
  if(*ps == '\r') --ps;
#endif


  if(*ps != fnam_del) strcat(cbuf, fnam_del_s);


  return 0;
  
}

  
//================================================================
  int UTX_add_slash (char *cbuf) {
//================================================================
/// \code
/// UTX_add_slash          add closing "/" to string (for dir's)
/// see also UTX_endAddChar UTX_add_fnam_del
/// \endcode

  char cl;

  // wenn cbuf kein closing "/" hat, eins zufuegen.
  cl = cbuf[strlen(cbuf)-1];

  if((cl != '/')&&(cl != '\\')) {
    strcat(cbuf, "/");
  }

  return 0;

}


//================================================================
  int UTX_ins_add (char* s1, int sSiz, char* s2, char *s3) {
//================================================================
/// UTX_ins_add            insert string left and add string right (s1=s2+s1+s3)
/// see also UTX_ins_s2

  char *tmpS1;
  int  i1, i2, i3, i12, i123;


  // printf("UTX_ins_add |%s|%s|%s| %d\n",s1,s2,s3,sSiz);

  i1 = strlen (s1);
  i2 = strlen (s2);
  i3 = strlen (s3);
    // printf(" %d %d %d\n",i1,i2,i3);

  i12 = i1 + i2;
  i123 = i12 + i3;
  if(i123 >= sSiz) {TX_Error("UTX_ins_add E1"); return -1;}

  tmpS1 = MEM_alloc_tmp (i1);
  memcpy (tmpS1, s1, i1);

  memcpy (s1, s2, i2);
  memcpy (&s1[i2], tmpS1, i1);
  memcpy (&s1[i12], s3, i3);
  s1[i123] = '\0';

    // printf("ex UTX_ins_add |%s|\n",s1);

  return 0;

}


//==============================================================
  void UTX_ins_s2 (char* s1, char* s2, int maxlen) {
//==============================================================
/// \code
/// UTX_ins_s2             insert string left (s1=s2+s1)
/// UTX_ins_s2       insert string links (s1 = s2//s1)
/// DO NOT USE;     use MEM_ins_str
/// maxlen ist die max. Länge des S1!
/// s2             MAX 256 chars !!!!!!!!!!!!!
/// see also UTX_ins_add
/// \endcode


  int      i1, i2;
  char     auxBuf[256];

  /* printf("UTX_ins_s2 |%s|%s| %d\n",s1,s2,maxlen); */


  if(maxlen > 256) maxlen = 256;


  i2 = strlen(s2);
  if(i2 > maxlen) {
    strncpy(s1,s2,maxlen);
    return;
  } else {
    strcpy(auxBuf,s2);
  }


  i1 = strlen(s1);
  if(i1+i2 > maxlen) {
    maxlen = maxlen-i2;
    strncat(auxBuf,s1,maxlen);
  } else {
    strcat(auxBuf,s1);
  }


  strcpy(s1,auxBuf);

  /* printf("exit UTX_ins_s2 |%s| %d %d %d\n",s1,i1,i2,maxlen); */

}


//================================================================
  int UTX_f_lifo_add (char *fNam, int maxLnr, char *newLn) {
//================================================================
/// \code
/// UTX_f_lifo_add             add line as first line into file with maxLnr lines
/// add line as first line into file;
/// all lines must uniq;
/// if file has more then maxNr Lines, delete surplus lines
/// \endcode

#define LN_MAX_SIZ 128

  int    i1, lln;
  char   fnTmp[256], sln[LN_MAX_SIZ], *p1;
  FILE   *fpi, *fpo;


  // printf("UTX_f_lifo_add |%s|%s|\n",fNam,newLn);

  // create tempfilnam
  sprintf(fnTmp,"%s.tmp",fNam);

  // write to tempfile
  if((fpo = fopen (fnTmp, "w")) == NULL) {
    MSG_STD_ERR (ERR_file_open, "'%s'", fnTmp);
    return -1;
  }
  fprintf(fpo, "%s\n", newLn);
  lln = strlen(newLn);

  // add existing; read write
  if((fpi = fopen (fNam, "r")) == NULL) goto L_exit1;

  i1 = 1;
  while (i1<maxLnr) {
    if(feof (fpi)) break;
    // read
    p1 = fgets (sln, LN_MAX_SIZ, fpi);
    // skip if not uniq, else write
    if(p1) {
      if(strncmp(sln,newLn,lln)) {
        fprintf(fpo, "%s", sln);
        ++i1;
      }
    }
  }

  fclose (fpi);

  L_exit1:
  fclose (fpo);

  // move fnTmp fNam
  OS_file_rename (fnTmp, fNam); // old,new

  return 0;

}


//=======================================================================
  int UTX_fsavLine (char *cbuf, char *filNam, int lnMaxSiz, int lNr) {
//=======================================================================
/// \code
/// UTX_fsavLine           change line Nr. <lNr> in file <filNam>
///
/// Input:
///   cbuf       line to add/modify
///   filNam
///   lnMaxSiz   max. size of lines of file <filNam>
///   lNr   -1   add line; else modify existing line; 
///              First line is lNr=1.
///              if file has less lines, empty lines are added.
/// RetCod:  1   File did not exist; created
///          2   Line did not exist; added.
///          0   OK
//          -1   Error
/// \endcode

// read filNam, write tempFil; copy filNam = tempFil. 
// if(lNr < 0)  if(filNam exists): append cbuf; 
//                else create filNam, write cbuf.


  int       i1;
  char      *p1, *sln, fNamTmp[256];
  FILE      *fpi, *fpo;


  // printf("UTX_fsavLine |%s|%s| %d\n",cbuf,filNam,lNr);

  // create tempfilnam
  sprintf(fNamTmp,"%s.tmp",filNam);

    // write to tempfile
  if(lNr >= 0) {
    if((fpo = fopen (fNamTmp, "w")) == NULL) {
      return -1;
    }
  } else {
    // append to existing file
    // copy infile -> tempfile
    if(OS_checkFilExist(filNam, 1))  {
      OS_file_copy (filNam, fNamTmp); // old,new
      // write to tempfile
      fpo = fopen (fNamTmp, "a");
    } else {
      // append to tempfile
      fpo = fopen (fNamTmp, "w");
    }
    fprintf(fpo, "%s\n", cbuf);
    goto L_exit2;
  }


  // existing file = inputfile
  if((fpi = fopen (filNam, "r")) == NULL) {
    // file does not exist; add new line.
    if(lNr > 1) {
      for(i1=1; i1<lNr; ++i1) fprintf(fpo, "\n");
    }
    fprintf(fpo, "%s\n", cbuf);
    goto L_exit2;
  }

  // in- and out-file open; read, write ..

  // get spc for line
  sln = (char*) MEM_alloc_tmp (lnMaxSiz);
  lnMaxSiz -= 2; // \n\0

  i1 = 0;
  p1 = sln;

  L_nxt:
    ++i1; //                                  printf(" nxt %d\n",i1);
    if(p1 != NULL) {
      p1 = fgets (sln, lnMaxSiz, fpi); //     printf(" l %d |%s|\n",i1,sln);
      if(p1 == NULL) sprintf(sln, "\n");
    }
    if(feof (fpi)) goto L_exit1;

    if(i1 != lNr) fprintf(fpo, "%s", sln);
    else          fprintf(fpo, "%s\n", cbuf);

    goto L_nxt;


  //----------------------------------------------------------------
  L_exit1:
    fclose (fpi);

  L_exit2:
    // copy filNam = tempFil.
    fclose (fpo);
    OS_file_rename (fNamTmp, filNam); // old,new


  return 0;

}


//=======================================================================
  int UTX_fRevL (char *fno, char *fni) {
//=======================================================================
/// \code
/// UTX_fRevL        File revert lines; eine Datei zeilenweise umdrehen;
/// letze Zeile wird zur ersten, vorletze Zeile wird zur zweiten usw.
/// \endcode

  int    irc=0;
  char   *memSpc, *pEnd;
  long   fSiz;
  FILE   *fpi, *fpo=NULL;


  printf("UTX_fRevL |%s|%s|\n",fno,fni);
//getchar();

  // get filesize
  fSiz = OS_FilSiz (fni);
    // printf(" fSiz=%d\n",fSiz);
  if (fSiz < 1) {
    return TX_Error ("UTX_fRevL  E000");
  }


  if ((fpi = fopen (fni, "rb")) == NULL) {
    return TX_Error ("UTX_fRevL  E001");
  }


  // allocat memspc fuer gesamte Datei
  memSpc = malloc (fSiz + 64);
  if(memSpc == NULL) {
    return TX_Error ("UTX_fRevL - out of memory");
  }


  // gesamtes file als block lesen ..
  irc = fread (memSpc, fSiz, 1, fpi);
  fclose (fpi);
  if (irc < 1) {
    TX_Error ("UTX_fRevL  E003");
    goto L_exit;
  }

  // terminieren - MS-Win Problem !
  pEnd = &memSpc[fSiz];
  *pEnd = '\0';
    // printf(" |%s|\n",memSpc);


  // open outFile
  if ((fpo = fopen (fno, "w")) == NULL) {
    fclose (fpo);
    return TX_Error ("UTX_fRevL  E002");
  }


L_prev_char:
  --pEnd;


L_next_line:
  if(pEnd > memSpc) {
    if (*pEnd != '\n') goto L_prev_char;
    ++pEnd;     // skip the \n; erste Zeile hat kein \n
  }
  // pEnd ist nun ein \n oder der 1. char
    // printf(" pEnd=%d |%s|\n",pEnd-memSpc,pEnd);
  if(strlen(pEnd))               // damit werden Leerzeilen geskippt (EOF!)
  fprintf(fpo, "%s\n", pEnd);


  if(pEnd > memSpc){
    --pEnd;               // back to \n
    *pEnd = '\0';         // new FileEnd
    --pEnd;               // back
    if(*pEnd == '\r') {   // is it a CR ?
      *pEnd = '\0';       // remove the CR
      --pEnd;             // back
    }
    goto L_next_line;
  }



L_exit:
  free (memSpc);
  if(fpo) fclose (fpo);

//getchar();

  return irc;

}


//=======================================================================
  int UTX_fgetLine (char *cbuf, int sizBuf, char *filNam, int lNr) {
//=======================================================================
/// \code
/// UTX_fgetLine           read line nr. <lNr> out of file <filNam>
///             first line has lineNr 1
/// sizBuf      size of cbuf
/// rc -1   File does not exist
/// rc -2   Line does not exist
/// rc  0   OK
/// \endcode


  int    i1;
  FILE   *fpi;

  // printf("UTX_fgetLine |%s| %d\n",filNam,lNr);


  if ((fpi = fopen (filNam, "r")) == NULL) {
    return -1;
  }

  i1=0;
  while (!feof (fpi)) {
    ++i1;
    if (fgets (cbuf, sizBuf, fpi) == NULL) goto Fertig;
    // printf(" ..--in |%s|\n",cbuf);
    if(i1 != lNr) continue;
    fclose(fpi);
    UTX_CleanCR (cbuf);
    // printf("ex UTX_fgetLine %d |%s|\n",lNr,cbuf);
    return 0;
  }
  Fertig:
  fclose(fpi);
  return -2;
}


//=======================================================================
  char *UTX_memFind (char *buf, char *str) {
//=======================================================================
/// str in buf suchen


  return UTX_memFind1 (buf,strlen(buf),str,strlen(str));

}


//=======================================================================
  char *UTX_memFind1 (char *buf, long bufSiz, char *str, long strSiz) {
//=======================================================================
/// \code
/// UTX_memFind1           Daten mit bestimmter Laenge suchen
/// 
/// Datablock str (siz = strSiz) in buf bis max. buf[strSiz] suchen.
/// \endcode

/*
  strcpy(cbuf, "asnfasjkhfajshfaklsjdhfkasdjf");
  pv1 = UTX_memFind1 (cbuf, strlen(cbuf), "ak", 2L);
  printf("  %c%c\n",cbuf[pv1-cbuf],cbuf[pv1-cbuf+1]);

*/

  int      i1;
  char     *pa, *p1, *p2, *pEnd;


  // printf("UTX_memFind1 %ld %ld\n",bufSiz,strSiz);


  pa = buf;
  pEnd = buf + bufSiz - strSiz;

  if(bufSiz < strSiz) goto L_err;



  L_next:
  if(pa > pEnd) goto L_err;
  // printf("       ... %ld\n",pa-buf);

  p1 = (void*) memchr((void*)pa, str[0], bufSiz);
  if(p1 == NULL) goto L_err;

  p2 = p1;


  for(i1=1; i1<strSiz; ++i1) {
    ++p1;
    if(*p1 != str[i1]) {
      ++pa;
      goto L_next;
    }
  }


  // printf("ex UTX_memFind1 found %ld\n",p2-buf);

  return p2;


  //=============================
  L_err:
  printf("ex UTX_memFind1 NIX found\n");

  return NULL;


}


//================================================================
  int UTX_memcmp (char *s1, char *s2) {
//================================================================
/// returns nr of equal characters

  int   ii = 0;

  while (*s1 == *s2) {
    ++s1;
    ++s2;
    ++ii;
  }

  // printf("ex UTX_memcmp %d |%s|%s|\n",ii,s1,s2);

  return ii;
}



//===================================================================
  char *UTX_memstr (char* von, char* bis, char* sstr) {
//===================================================================
/// \code
/// den String sstr im Bereich von - bis suchen.
/// Returns:
///   position of first char if found
///   else NULL
/// \endcode


  unsigned long msiz;
  int  i1, slen;
  char *pa, *p1, *p2;

  pa = von;
  slen = strlen(sstr);

  L_next:
  msiz = bis - pa;
  if(msiz < slen) return NULL;

  p1 = (void*) memchr((void*)pa, sstr[0], msiz);
  if(p1 == NULL) return NULL;

  p2 = p1;


  for(i1=1; i1<slen; ++i1) {
    ++p1;
    if(*p1 != sstr[i1]) {
      ++pa;
      goto L_next;
    }
  }


  return p2;

}


//================================================================
  int UTX_chrNr (char *s1) {
//================================================================
// get nr of chars to end of line (excluding EOL)

  char *p1 = strchr(s1,'\n');
   if(p1) return p1 - s1;
   else   return strlen (s1);

}


//====================================================================
  char* UTX_fgets (char* line, int len, FILE* fp) {
//====================================================================
/// \code
/// UTX_fgets              Zeile aus Datei lesen und CR',s LF's am Ende deleten.
/// 
///   Lesen einer Datenzeile aus einer Datei.
///   Im gelesenen String werden von hinten nach vorne ueberfluessige
///   Blanks ' ', Carriage Returns '\r' und Linefeeds '\n' durch
///   Nullzeichen '\0' ersetzt (bis zum ersten Vorkommen eines
///   anderen Zeichens).
/// \endcode

//   while(fgets(..))) {
//   }



  char* lp;

  /* ---------------------------------------------------------- */

  lp = fgets (line, len, fp);
  if (lp == NULL) return(lp);

  UTX_CleanCR (line);

  return (lp);
}


//=================================================================
  void UTX_CleanAN (char* s1) {
//=================================================================
/// \code
/// change all chars not alpha or numeric into '_'
/// keep [0-9],[A-Z],[a-z]; change others into '_'
///
/// see also UTX_ck_uml_s UTX_safeName
/// \endcode

  char *t1;


  // printf("UTX_CleanAN |%s|\n",s1);


  // keep [0-9],[A-Z],[a-z]; change others into '_'
  while (*s1) {
      // printf(" %c\n",*s1);
    if(*s1 < '0') goto L_change;
    if(*s1 > 'z') goto L_change;
    if(*s1 >= 'a') goto L_keep;
    if(*s1 <= '9') goto L_keep;
    if(*s1 < 'A') goto L_change;
    if(*s1 > 'Z') goto L_change;
    goto L_keep;

    L_change:
      *s1 = '_';

    L_keep:
      ++s1;
  }

  return;

}


//=================================================================
  void UTX_CleanSC (char* string) {
//=================================================================
/// \code
/// change all special-chars into '?'
/// special-char: all (char < ' ') or (char > '}')
///
/// see also UTX_ck_uml_s UTX_safeName
/// \endcode

  int i1, ilen, ic;
  


  ilen = strlen (string);


  for(i1=0; i1 < ilen; ++i1) {
    ic = string[i1];
    // printf(" %d %d\n",i1,ic);
    if((string[i1] < ' ')||(string[i1] > '}')) string[i1] = '?';
  }
   
  return;

}


//===========================================================
  char* UTX_CleanCR (char* string) {
//===========================================================
/// \code
/// UTX_CleanCR              Delete Blanks, CR's and LF's at end of string
/// returns positon of stringterminator \0
/// 
/// see also UTX_del_follBl UTX_CleanSC
/// \endcode


  int  ilen;
  char *tpos;

  ilen = strlen (string);

  tpos = &string[ilen];

  if(ilen < 1) goto L_exit;

  --tpos;


  while ((*tpos  == ' ')  ||
         (*tpos  == '\t') ||          /* tab */
         (*tpos  == '\n') ||          /* newline */
         (*tpos  == '\r'))   {        /* CR */

    *tpos    = '\0';
    if(tpos <= string) goto L_exit;
    --tpos;
  }

  ++tpos;


  L_exit:
  // printf("ex UTX_CleanCR |%s|\n", string);

  return tpos;
}


//================================================================
  int UTX_CleanLF (char *sBuf) {
//================================================================
/// \code
/// UTX_CleanLF           change lineEnd (Cr|CrLf ..) to single LF.
/// Input:
///   sBuf     line, eg from file (ex MS ..)
/// Output:
///   sBuf      end of line is '\n'
///   RetCod    nr of characters of sBuf
/// \endcode


  int    sLen;
  char   *cp1, *cp2;


  sLen = strlen(sBuf);


  // letzter char muss \n sein
  if(sLen < 2) {
    // only one char: \r > \n
    if(*sBuf == '\r') *sBuf = '\n';
    // add \n if none ..
    if(*sBuf == '\n') goto L_done;
    goto L_add;
  }


  // text .. cp1 cp2 \0
  cp1 = &sBuf[sLen-2];
  cp2 = &sBuf[sLen-1];
  if((*cp2 == '\r')||(*cp2 == '\n'))    {
    if((*cp1 == '\r')||(*cp1 == '\n'))    {
      // \n\r - remove last char ..
      *cp1 = '\n';
      *cp2 = '\0';
      --sLen;
      goto L_done;
    }
    *cp2 = '\n';
    goto L_done;
  }


  // add \n
  L_add:
    cp1 = &sBuf[sLen];
    *cp1 = '\n';
    ++cp1;
    *cp1 = '\0';
    ++sLen;


  L_done:
  return sLen;
}


//================================================================
  int UTX_CleanCommC (char *cBuf) {
//================================================================
/// Clean C-Comments (remove // comment and following blanks)

  char    *p1;

  p1 = strstr (cBuf, " //");
  if(p1) *p1 = '\0';

  UTX_CleanCR (cBuf);

  return 0;

}


//================================================================
  int UTX_Clean_db (char *cbuf) {
//================================================================
/// \code
/// clean doubles (change ',' into '.') in gesamter cbuf
/// MS-Win schreibt "1,0"
/// See also UTX_chg_chr1.
/// \endcode

  char   *p1, *p2;

  // UTX_CleanCR (cbuf);  // TEST

  p2 = cbuf;

  L_nxt:
  p1 = strchr(p2, ',');

  if(p1) {
    // printf(" Clean_db |%s|\n",p1);
    p2 = p1 + 1;
    if(isdigit(*p2)) *p1 = '.';
    goto L_nxt;
  }

  return 0;

}


//===========================================================================
  char* UTX_pos_skipStr (char *cbuf) {
//===========================================================================
/// \code
/// UTX_pos_skipStr      find corresponding \" (skip string)
/// find end of string ..
/// Input:
///   cbuf: '\"' must be the first character !
/// Output:
///   retCod  the position of the '\")' corresponding to cbuf[0]
///
///   cp2 = UTX_pos_skipStr (cp1);
///   if(*cp2) - handle error ..
/// \endcode


  char *tpos;


  // printf("UTX_pos_skipStr |%s|\n",cbuf);

  tpos = cbuf;


  L_weiter:
  ++tpos;
  if(*tpos  == '\0') goto L_fertig;
  if(*tpos  != '\"') goto L_weiter;



  L_fertig:
  // printf("ex UTX_pos_skipBrack |%s|\n",tpos);
  return tpos;

}


//================================================================
  int UTX_pos_skipBrackX (char **ps, char *pe, char ce) {
//================================================================
// UTX_pos_skipBrackX     find corresponding bracket
// can be used for {([ ..
// Input:
//   ps     character where to start; { or ( or [
//   pe     end of string
//   ce     closing bracket; '}' or ')' or ']' 
// Output:
//   ps     points to the closing bracket (char ce)
// RetCod:
//    0     OK, ps = closing bracket.
//   -1     closing bracket not found 
 
// see also UTX_pos_skipBrack UTX_pos_skipBrack1 wrl1_r_skipBlock


  int  iLev;
  char cs, *p1;


  p1   = *ps;
  cs   = *p1;
  iLev = 1;

    // printf("wrl1_r_skipBlock %d |",level);
    // UTX_dump_s__ (p1, 50);
    // printf("|\n");

  L_next:
  ++p1;

  if(p1 >= pe) {TX_Print("UTX_pos_skipBrackX E001"); return -1;}

  if(*p1 == cs)  {++iLev; goto L_next;}
  if(*p1 == ce)  {--iLev; if(iLev < 1) goto L_found;}
  goto L_next;


  //----------------------------------------------------------------
  L_found:
  // printf("  skipBlock len=%d\n",p1-*ps);
  *ps = p1;

    // printf("ex UTX_pos_skipBrackX |");
    // UTX_dump_s__ (p1, 30);
    // printf("|\n");

  return 0;

}


//===========================================================================
  char* UTX_pos_skipBrack (char *cbuf) {
//===========================================================================
/// \code
/// UTX_pos_skipBrack      find corresponding ')' (returns Position of ')')
/// find end of bracket ..
/// Input:
///   cbuf: '(' must be the first character !
/// Output:
///   retCod  the position of the ')' corresponding to cbuf[0]
/// \endcode


  int  ianz;
  char *tpos;


  // printf("UTX_pos_skipBrack |%s|\n",cbuf);

  tpos = cbuf;
  ianz = 1;


  L_weiter:
  ++tpos;


  L_start:
  if(*tpos  == '(')  {
    ++ianz;
    goto L_weiter;
  }
  if(*tpos  == ')')  {
    --ianz;
    if(ianz == 0) {
      // ++tpos;
      goto L_fertig;
    }
    goto L_weiter;
  }
  if(*tpos  != '\0') goto L_weiter;



  L_fertig:
  // printf("ex UTX_pos_skipBrack |%s|\n",tpos);
  return tpos;

}

//===========================================================================
  char* UTX_pos_skipBrack1 (char *cbuf) {
//===========================================================================
/// \code
/// UTX_pos_skipBrack1     find corresponding ')' (returns Position of ')')
/// find end of bracket ..
/// Input:
///  cbuf:    !!! first '(' is not included in *cbuf !!!
/// Output:
///   retCod  the position of the ')' corresponding to the missing ')'
///  Examples:
///   strcpy(s1, "(3*3*sin(9)) + (4*4))");
///   UTX_pos_skipBrack1(&s1[1]);      returns ") + (4*4))"
/// \endcode


  int  ianz;
  char *tpos;


  // printf("UTX_pos_skipBrack |%s|\n",cbuf);

  tpos = cbuf;
  ianz = 1;
  goto L_start;


  L_weiter:
  ++tpos;


  L_start:
  if(*tpos  == '(')  {
    ++ianz;
    goto L_weiter;
  }
  if(*tpos  == ')')  {
    --ianz;
    if(ianz == 0) {
      // ++tpos;
      goto L_fertig;
    }
    goto L_weiter;
  }
  if(*tpos  != '\0') goto L_weiter;



  L_fertig:
  // printf("ex UTX_pos_skipBrack |%s|\n",tpos);
  return tpos;

}


//================================================================
  int UTX_ck_caseChr (char *s1, char *s2) {
//================================================================
/// \code
/// compare 2 characters - ignore case
/// s2 must be uppercase !
/// Cannot handle german specialChars (ae, ue, ..).
/// Retcod:
///     0    both chars are equal
///     1    chars are NOT equal
/// 
/// see also strcasecmp strncasecmp (nur Linux !)
/// \endcode


  // printf("UTX_ck_caseChr %c %c\n",*s1,*s2);


  if((*s1 < 97)||(*s1 > 122)) {
    if(*s1 == *s2) return 0;
    return 1;
  }

  // change s1 > uppercase
  if((*s1-32) == *s2) return 0;
  return 1;

}


//================================================================
  int UTX_ck_casenChr (char *s1, char *s2, int cNr) {
//================================================================
/// \code
/// compare n chars of 2 strings - ignore case
/// s2 must be uppercase !
/// Cannot handle german specialChars (ae, ue, ..).
/// Retcod:
///     0    both Strings are equal
///     1    Strings are NOT equal
/// 
/// see also strcasecmp strncasecmp (nur Linux !)
/// \endcode

  int    i1;

  for(i1=0; i1<cNr; ++i1) {
      // printf(" caseChr=%d %c %c\n",UTX_ck_caseChr(s1,s2),*s1,*s2);
    if(UTX_ck_caseChr (s1, s2)) return 1;
    ++s1;
    ++s2;
  }

  return 0;

}


//================================================================
  int UTX_ck_caseStr (char *s1, char *s2) {
//================================================================
/// \code
/// compare 2 strings - ignore case
/// s2 must be uppercase !
/// Cannot handle german specialChars (ae, ue, ..).
/// Retcod:
///     0    both Strings are equal
///     1    Strings are NOT equal
/// 
/// see also strcasecmp strncasecmp (nur Linux !)
/// \endcode


  while(*s1) {
      // printf(" caseChr=%d %c %c\n",UTX_ck_caseChr(s1,s2),*s1,*s2);
    if(UTX_ck_caseChr (s1, s2)) return 1;
    ++s1;
    ++s2;
  }

  return 0;

}


//================================================================
  int UTX_ckc_Del1 (char c1) {
//================================================================
/// \code
/// UTX_ckc_Del1           check if char is a delimiter
/// RC 0 = yes, char is a delimiter
/// RC 1 = no;
/// \endcode


  if((c1 == ' ')   ||
     (c1 == '=')   ||
     (c1 == ',')   ||
     (c1 == '(')   ||
     (c1 == ')')   ||
     (c1 == '"')   ||
     (c1 == '\n')  ||
     (c1 == '\t')  ||
     (c1 == '\r')  ||
     (c1 == '{')   ||
     (c1 == '}')   ||
     (c1 == '[')   ||
     (c1 == ']')   ||
     (c1 == '\0'))    return 0;

  return 1;

}


//================================================================
  int UTX_ckc_Del2 (char c1) {
//================================================================
/// \code
/// UTX_ckc_Del2           check if char is a delimiter
/// RC 0 = yes, char is a delimiter
/// RC 1 = no;
/// \endcode


  if((c1 == ' ')   ||
     (c1 == '\t'))    return 0;

  return 1;

}


//================================================================
  int UTX_ck_specChars (char *s1) {
//================================================================
/// \code
/// test if string contains specialCharacters
/// RetCod: 0=OK, keine SpecChar im string s1 enthalten.
/// RetCod: -1=ERROR; es ist SpecChar im string s1 enthalten.
/// \endcode


  int   irc;
  char  *pc;


  pc = s1;

  while(*pc) {
      // printf("|%c|\n",*pc);

    if((*pc < 48)||(*pc > 122)) {                    // 48-122
      if((*pc == 45)&&(pc > s1)) goto L_nxt;         // minus ab second char
      return -1;
    }
    if(*pc == 44) return -1;                          // 44  (",")
    if((*pc > 57)&&(*pc < 65)) return -1;             // 58-64
    if(*pc == 95) goto L_nxt;                         // 95 = underscore
    if((*pc > 90)&&(*pc < 97)) return -1;             // 91-96

    L_nxt:
    ++pc;
  }


  return 0;

}


//================================================================
  char* UTX_find_Del1 (char *p1) {
//================================================================
/// find next delimiter ' ' '=' ',' '\n' '\t' '{' ...

  char   *p2;

  p2 = p1;
 
  while((*p2 != ' ')   &&
        (*p2 != '=')   &&
        (*p2 != ',')   &&
        (*p2 != '(')   &&
        (*p2 != ')')   &&
        (*p2 != '"')   &&
        (*p2 != '\n')  &&
        (*p2 != '\t')  &&
        (*p2 != '\r')  &&
        (*p2 != '{')   &&
        (*p2 != '}')   &&
        (*p2 != '[')   &&
        (*p2 != ']')   &&
        (*p2 != '\0'))    ++p2;

  return p2;

}


//================================================================
  char* UTX_pos_skipDeli1 (char *p1) {
//================================================================
/// UTX_pos_skipDeli1      skip delimiter ' ' ',' '\n' '\r' '\t'


  while ((*p1 == ' ')  ||
         (*p1 == ',')  ||
         (*p1 == '\n') ||
         (*p1 == '\t') ||
         (*p1 == '\r'))
    ++p1;


  return p1;

}


//===================================================================
  char* UTX_2db_tx (double *d1, double *d2, char *cbuf) {
//===================================================================
/// see also UT3D_pt_txt

  char *p1;


  *d1 = strtod (cbuf, &p1); ++p1;
  *d2 = strtod (p1,&p1);


  // printf("ex UTX_2db_tx %f %f |%s|\n",*d1,*d2,cbuf);

  return p1;

}


//===================================================================
  double UTX_db_tx (char **po, char *pi) {
//===================================================================
/// \code
/// read float from ascii-string
/// po == pi erlaubt ..
/// ACHTUNG: pi muss veraenderbar sein !! (D wird in E umgewandelt ...)
/// \endcode

  double    d1;
  char      *p1;
  char      c1;


  // strtod kann kein "2D1"; das D muss in E umgewandelt werden !!
  p1 = pi;
  UTX_pos_skip_num  (&p1);


#ifdef _MSC_VER

  // ACHTUNG; MS-BUG; MS-Bug MS-bug :
  // string muss mit 0 terminiert sein, sonst dauerts 1000-fach laenger !!!!!
  c1  = *p1;
  *p1 = '\0';
  d1 = atof (pi);
  *p1 = c1;
  *po = p1;

#else

  d1 = strtod (pi, po);

/*
  // Exponentialdarstelleung decodieren
  // printf("Del. = %-1.1s\n",ip2);
  // 110,9.3D0, 2D1,-2.7D2  -1D-14 9.527182240151D-1,0D0,
  // ,-1.04285E-16,2.0E+05,
  // if(!strncmp(ip2, "D0", 2)) ip2 += 2;
  if(*ip2 == 'D') {   // find next ',' or ';'
    ++ip2;
    i1 = atoi(ip2);
    ra[parInd] = ra[parInd] * pow(10., i1);
    // printf(" D%d %f\n",i1,ra[parInd]);
    for(;;) {
      if((*ip2 == ',')||(*ip2 == ';')) break;
      ++ip2;
    }
  }
*/

#endif



  // printf("ex UTX_db_tx %f |%s|\n",d1,*ps);

  return d1;

}


//===================================================================
  int UTX_pos_skip_num (char **ps) {
//===================================================================
/// skip number


  char  *tpos;


  // printf("UTX_pos_skip_num |%s|\n",cbuf);

  tpos = *ps;

  // zuerst leading blanks weg
  while (*tpos  == ' ')  { ++tpos; }

  // nun darf ein Vor. sein
  if((*tpos == '-')||(*tpos == '+')) ++tpos;

  // Vorkommastellen
  while(isdigit(*tpos)) { ++tpos; }

  // nun darf ein Punkt sein
  if(*tpos == '.') ++tpos;

  // Nachkommastellen
  while(isdigit(*tpos)) { ++tpos; }

  // E oder D; change D --> E
  if((*tpos == 'E')||(*tpos == 'D')||
     (*tpos == 'e')||(*tpos == 'd'))  {
    *tpos = 'E';
    ++tpos;
    if((*tpos == '-')||(*tpos == '+')) ++tpos;
    while(isdigit(*tpos)) { ++tpos; }
  }

  *ps = tpos;

  // printf("ex UTX_pos_skip_num   %d|\n",iNr);
  // if(iNr > 0) return 0;

  return -1;

}


//===================================================================
  int UTX_pos_skip_int (char **ps) {
//===================================================================
/// skip int-number in string


  char  *tpos;


  // printf("UTX_pos_skip_int |%s|\n",*ps);

  tpos = *ps;

  // zuerst leading blanks weg
  while (*tpos  == ' ')  { ++tpos; }

  // nun darf ein Vor. sein
  if((*tpos == '-')||(*tpos == '+')) ++tpos;

  // Ziffern
  while(isdigit(*tpos)) { ++tpos; }

  *ps = tpos;

  // printf("ex UTX_pos_skip_int |%s|\n",tpos);

  return 0;

}


//================================================================
  int UTX_pos_skipTermWord (char **wNxt, char *cbuf) {
//================================================================
// terminate & skip next word in string.
// seach for end of 1. word in cbuf (',' '\n' '\t' ' ' '\0')
// terminate this word ('\0')
// return pos of next word (++pos)
// RetCod:
//    0      OK; wNxt is next word in string
//   -1      cbuf is last word in string 
//   -2      cbuf is empty; wNxt and cbuf = '\0'

// see also UTX_find_Del1 UTX_pos_skipWord UTX_pos_skipDeli1


  char     *p1;

  if(*cbuf == '\0') return -2;

  *wNxt = UTX_pos_skipWord (cbuf);
    // printf(" wNxt = |%s|\n",*wNxt);


  if(**wNxt == '\0') return -1;

  **wNxt = '\0';

  L_skip_1:
    *wNxt += 1;
    while (**wNxt  == ' ')  goto L_skip_1;

  return 0;

}


//===========================================================================
  char* UTX_pos_skipWord (char *cbuf) {
//===========================================================================
/// \code
/// UTX_pos_skipWord       skip Word
/// returns Pos. of first char after word (the delimiter !)
/// find next '\0' oder ','; skip "(...)".
/// DOES NOT (YET) SKIP STRINGS ("...").
/// following blanks werden geskippt
/// \endcode


  char *tpos;


  // printf("UTX_pos_skipWord |%s|\n",cbuf);
  // printf("UTX_pos_skipWords |");UTX_dump_s__(cbuf);printf("|\n");
  

  // tpos = cbuf;
  // // skip leading blanks
  // L_start:
  // if(*tpos  == ' ')  { ++tpos; goto L_start; }
  tpos = UTX_pos_skipDeli1 (cbuf);  // skip 1 delimiter (blank,comman,\n \t \r)
    // printf("   n.skipDeli1 |");UTX_dump_s__(tpos);printf("|\n");
  if(*tpos == '\0') goto L_exit;
  goto L_test;


  L_weiter:
  ++tpos;     // skip char of word.


  L_test:
  if(*tpos  == '(')  {
    tpos = UTX_pos_skipBrack (tpos);
    goto L_weiter;
  }
  if(*tpos  == '\"')  {
    tpos = UTX_pos_skipStr (tpos);
    goto L_weiter;
  }
  if(*tpos == ' ')   goto L_fertig;
  if(*tpos == ',')   goto L_exit;
  if(*tpos == '\n')  goto L_exit;
  if(*tpos == '\t')  goto L_exit;
  if(*tpos != '\0')  goto L_weiter;


  // skip following blanks
  L_fertig:
  // if(*tpos == ' ')  { ++tpos; goto L_fertig; }




  L_exit:
    // printf("ex UTX_pos_skipWord |%s|\n",tpos);
    // printf("ex UTX_pos_skipWords |");UTX_dump_s__(tpos);printf("|\n");


  return tpos;

}


//================================================================
  char* UTX_pos_skipWords (int wNr, char *p1) {
//================================================================
/// UTX_pos_skipWords      skip wNr Words (ret. Pos. of first char after word)

  int   i1;


  for(i1=0; i1<wNr; ++i1) {
    // printf(" skip %d. word|",i1);UTX_dump_s__(p1);printf("|\n");
    p1 = UTX_pos_skipWord (p1);
  }

  // printf(" ex UTX_pos_skipWords |");UTX_dump_s__(p1);printf("|\n");
  return p1;

}


//===========================================================================
  int UTX_wordnr (char *cbuf) {
//===========================================================================
/// \code
/// UTX_wordnr             returns nr of words
/// returns Anzahl von Worten in cbuf
/// als delimiter gelten nur blank und oder ","
/// Ausdruck wird korrekt behandelt ("D(0 0 1)" ist nur 1 Wort !!)
/// \endcode


  int   iNr;
  char  *p1, *p2;


  // printf("UTX_wordnr |%s|\n",cbuf);

  iNr = 0;
  p2 = cbuf;

  while(*p2  == ' ')  ++p2;
  if(*p2 == '\0') goto L_fertig;
  
  L_next:
  ++iNr;
  p2 = UTX_pos_skipWord (p2);
  if(*p2 != '\0') goto L_next;


  // if last char was one or more blanks:
  --p2;
  if(*p2 == ' ') --iNr;
  

  L_fertig:
  // printf("ex UTX_wordnr %d |%s|\n",iNr,cbuf);
  return iNr;

}


//================================================================
  int UTX_cnr_chr (char *txt, char c1) {
//================================================================
/// returns nr of char c1 in string txt


  int   iNr;
  char  *p1;

  iNr = 0;

  p1 = txt;

  while (*p1) {
    if(*p1 == c1) ++iNr;
    ++p1;
  }

  return iNr;

}

//================================================================
  int UTX_sget_nrRange (double *d1, double *d2, char **expr) {
//================================================================
// UTX_sget_nrRange             get nr or range out of textstring
// sacn a line of single-numbers or ranges
// RetCod:
//     0       end of string
//     1       single number (d1)
//     2       range (d1 - d2)
//    -1       inputerror

// example:
//  int     irc;
//  double  d1, d2;
//  char    txt[80]="1 2,13-45,16 17", *w;
//  w = txt;
//  L_nxt:
//  irc = UTX_sget_nrRange (&d1, &d2, &w);
//    printf(" _nrRange %d %f %f\n",irc,d1,d2);
//  if(irc > 0) goto L_nxt;


  int    wtyp, iStat;
  char   deli, txtOut[32], *w_next, *w;


  // w_next = txt;
  iStat  = 0;


  Next:
  w = *expr;
  wtyp = UTX_get_word (&w, expr, txtOut, d2, &deli);
  // printf("txtOut=|%s| typ=%d  deli=|%c| w_next=|%s| val=%f\n",
          // txtOut,wtyp,deli,w_next,d1);


  if(wtyp > 0) {
    if(wtyp != 3) {
      TX_Print("***** inputerror: cannot decode number..");
      return -1;
    }

    if(iStat == 1) {
      // printf(" range: %f - %f\n",*d2,*d1);
      return 2;
    }

    *d1 = *d2;

    if((deli == ' ')||(deli == ',')) {
      // printf(" single: %f\n",*d1);
      return 1;
    }
    if(deli == '-') {
      iStat = 1;
      goto Next;
    }

  }


  return 0;

}


//=========================================================================
  int UTX_get_word (char** txtIn, char** w_next, char* txtOut,
                    double *val, char *deli) {
//=========================================================================
/// \code
/// UTX_get_word           get next word and its delimiter and typ of word
/// 
/// Input:  
///   txtIn  startpos
/// 
/// Output:
///   txtIn  leading blanks skipped
///   w_next Startpos des nach dem 1. Wort folgenden Textes;
///          excluding Delimiter, but not operators, Blanks skipped.
///   txtOut Kopie des 1. Wortes im Buffer txtIn; max. 32 char;
///          (nur bei Typ 1 und 2);  without leading & following blanks
///   val    value (des 1. Wortes, atof)
///   deli   der Delimiter (ein single char; der erste char NACH dem Wort)
///          
/// Returncodes:
/// TXT_Typ_EOF    0 = end of txtIn (empty);
/// TXT_Typ_spec   1 = spec.char (out in txtOut)  + - / * ^ . ; ( ) $ ,
/// TXT_Typ_text   2 = alfatext, (out in txtOut) delimiters are blank , + - / *
///                    . _ does not terminate text.
/// TXT_Typ_num    3 = num. value (double, out in val);
/// TXT_Typ_string 4 = string (durch " beidseitig begrenzt) String is copied
///                    to txtOut (max 32 chars) without ", terminated with \0
/// 
/// 
/// Beispiele:
///            RC   val  deli  w_next  txtOut
/// "+3"       num    3  ' '   ""      -
/// "-3+2"     num   -3  '+'   "2"     -
/// "-3 -2"    num   -3  ' '   "-2"    -             <<<<<<<<<
/// "- 3"      spec      '-'   "3"     "-"
/// "*3"       spec      '*'   "3"     "*"
/// "(3+1)*2"  spec      ' '   "3"     "("
/// "v1=5"     text   -  '='   "5"     "v1"
/// "v1 = 5"   text   -  '='   "5"     "v1"
/// "v1-5"     text   -  '-'   "5"     "v1"          <<<<<<<<<
/// "v1 -5"    text   -  ' '   "-5"    "v1"          <<<<<<<<<
/// "v1 p2"    text   -  ' '   "p"     "v1"
/// ""ab ""    string    ' '           "ab "
/// "abc "a""  text      '"'   "a""    "abc"
/// \endcode

/*
--------------------------------
Example (scan line):
  int ctyp;
  double d1;
  char deli, txtOut[32];
  char txt[80]=" abc 123.5,xx/ + -12;\"", *w_next, *w;

  w_next = txt;
  Next:
  w = w_next;
  ctyp = UTX_get_word (&w, &w_next, txtOut, &d1, &deli);
  printf("%d |%c|%s|%s| %f\n",ctyp,deli,txtOut,w_next,d1);
  if (ctyp > 0) goto Next;


--------------------------------------------------------*/

  int   i1, ilen, ipos, ctyp, itxt;
  char  *txt, *p1, *p2, *p3;


  // printf("============================================== \n");
  // printf("UTX_get_word |%s|\n",*txtIn);


  txt = *txtIn;

  ilen = strlen(txt);
  ipos = 0;
  ctyp = TXT_Typ_EOF;
  itxt = 0;
  *val = 0.0;
  txtOut[0] = '\0';
  if(ilen < 1) goto L_Exit;




  L_next_char:
    // printf("  _get_word %d |%c|\n",ipos,txt[ipos]);

  // skip leading blanks
  if (txt[ipos] == ' ') {
    ++txt;
   *txtIn = &txt[ipos];
    goto L_next_char;
  }



  // ----------- handle Textstrings mit "" ----
  // copy Textstring (ohne ") bis zum closing "
  if(txt[ipos] != '"') goto L_ck_Spec;
      // printf("found Textstring\n");
    ctyp = TXT_Typ_string;

    L_isString:
    ++ipos;   // skip leading "
    if (txt[ipos] == '\0') {--ipos; goto L_String;}

    txtOut[itxt] = txt[ipos];  // copy char
    if(itxt < 32) ++itxt;
    if (txt[ipos] != '"') goto L_isString;

    L_String:
    --itxt;   // skip following "
    txtOut[itxt] = '\0';
    p1 = &txt[ipos+1];
    goto L_skip_deli;  // 2012-12-20
    // *deli = *p1;
    // goto AllesFertig;



  /* --------------------------------------------------- */
  /* L_isspecial: */
  L_ck_Spec:
  if(!ispunct(txt[ipos]))  goto L_ck_txt; // FALSE: blank 0-9 a-z A-Z else TRUE
    // hier TRUE: + - . , ^ * / ; " ( ) [ ] { } @
    // printf(" ispunct %d |%c|\n",ipos,txt[ipos]); 
    // plus or minus or .: if afterwards comes a digit: goto number 
    // test for numeric ..
    if (txt[ipos] == '.') goto L_isnum;
    if ((txt[ipos] == '+') ||
        (txt[ipos] == '-'))    {      // test for "+5"(num)  or "+V5"(spec)
      i1 = ipos + 1;
      if (txt[i1] == '.') goto L_isnum;
      if (isdigit(txt[i1])) goto L_isnum;
    }

    /* L_ispunct: */
  L_isSpec:
    txtOut[0] = txt[ipos];
    txtOut[1] = '\0';
    ctyp = TXT_Typ_spec;
    // w_next does not contain the specChar
    p1 = &txt[ipos];
    if(*p1) ++p1;             // w_next = without delimiter
    while(*p1  == ' ') ++p1;  // skip blanks until real delim.
    // *deli = ' ';
    *deli = *p1;
    while(*p1  == ' ') ++p1;  // skip blanks behind real delim.
    goto AllesFertig;



  /* ----- handle texte (ohne "") -------------------------- */
  L_ck_txt:
  // printf("L2 %d\n",ipos);
  if (!isalpha(txt[ipos]))  goto L_isnum;
    // printf(" isalpha %d\n",ipos);
    L_isalpha:
      // normalen Text nach txtOut kopieren; max 32 chars.
      txtOut[itxt] = txt[ipos];
      if (itxt < 32) ++itxt;
      ++ipos;
      if (isalnum(txt[ipos])) goto L_isalpha; // accept a-z and 0-9
      // Abbruch bei delimiter;
      // Punkt, Underscore gelten NICHT als delimiter
      if(txt[ipos] == '_') goto L_isalpha;
      if(txt[ipos] == '.') goto L_isalpha;
        // printf(" end isalpha %d |%c| %d\n",ipos,txt[ipos],itxt);
      ctyp = TXT_Typ_text;
      txtOut[itxt] = '\0';
      p1 = &txt[ipos];
        // printf(" _text nxt=|%s|\n",p1);
      goto L_skip_deli;



  /* ----- handle numbers ------------------------------ */
  L_isnum:
  i1 = UTX_ck_num_f (&p1, &txt[ipos]);
  if(i1 == 0) {
      // printf(" Zahl; p1=|%s|\n",p1);
    ctyp = TXT_Typ_num;
    *val = atof (txt);
    goto L_skip_deli;
  }
  // ist keine zahl, beginnt aber mit + od - od .
  goto L_isSpec;



  //----------------------------------------------------------------
  // p1 points to the delimiter; set deli.
  // if deli is operator or '='
  L_skip_deli:
    *deli = *p1;
    if(*p1 == ' ') {
      // der primaere delimiter war ein blank.
      p2 = p1;                   // test next char
      while(*p2  == ' ') ++p2;   // skip following blanks
        // printf(" 1p2=|%s|\n",p2);
      if(ispunct(*p2)) {        // is operator or =
        // hier TRUE = + - . , ^ * / ; " ( ) [ ] { } @
          // printf(" ispunkt1-Yes\n");
        // do not accept .  (+ - werden automat als  + operation gesetzt)
        if(*p2 == '"') goto AllesFertig;
        if((*p2 == '-')||(*p2 == '+')) {
          // nach einem Blank folgt ein '-';  Beispiel:    "12 -13"
          // wenn nachfolgendes Zeichen numer. ist: skip this new Deli.
          p3 = p2 + 1;
          if((isdigit(*p3))||(*p2 =='.')) goto AllesFertig;
        }
        if(*p2 == '.') {
          p3 = p2 + 1;
          if(isdigit(*p3)) goto AllesFertig;
        }
        // ist zB =
        ++p1;
        *deli = *p1;
        if(*p1) ++p1;             // w_next = without delimiter
        while(*p1  == ' ') ++p1;  // skip blanks behind real delim.
        goto AllesFertig;
      }
    }
    L_skip_del1:
      if(ispunct(*p1)) {        // is operator or =
        // hier TRUE = + - . , ^ * / ; " ( ) [ ] { } @
          // printf(" ispunkt2-Yes\n");
        // do not accept . + -
        // do not accept .  (+ - werden automat als  + operation gesetzt)
        if(*p1 == '"') goto AllesFertig;
        if(*p1 == '.') goto L_skip_del2;
        // ist zB =
        if(*p1) ++p1;             // w_next = without delimiter
        while(*p1  == ' ') ++p1;  // skip blanks behind real delim.
        goto AllesFertig;
      }
    L_skip_del2:
    // goto AllesFertig;





  //----------------------------------------------------------------
  AllesFertig:
  // skip foll. blanks
  *w_next = UTX_pos_1n (p1);


  // EndOfLine: deli = blank;
  if(*deli == '\0') *deli = ' ';


  L_Exit:

    // printf("ex UTX_get_word ctyp=%d deli=%c nxt=|%s| val=|%f| txOut=|%s|\n",
                            // ctyp,   *deli, *w_next,  *val,     txtOut);

  return ctyp;
}


//================================================================
  int UTX_cut_word (char *sBuf, char *s1) {
//================================================================
// remove word s1 (and its following delimiter) out of sBuf
// RetCod:
//   0=Ok, word removed;  -1=s1 not in sBuf.

  long    iDel;
  char    *p1, *p2;


  // printf("UTX_cut_word |%s|%s|\n",sBuf,s1);


  p1 = strstr(sBuf, s1);
  if(!p1) return -1;

  iDel = strlen (s1);
  p2 = p1 + iDel;


  // check if a delimiter follows s1 in sBuf.
  while(!UTX_ckc_Del2(*p2)) { ++iDel; ++p2; }

  // cut ii characters out of sBuf starting at ?
  UTX_cut2 (p1, iDel);
  

  return 0;

}


//=====================================================================
  int UTX_cut1 (char *cbuf, long *bufSiz, long cPos, long txLen) {
//=====================================================================
/// \code
/// UTX_cut1               cut Textstring out of Textbuffer
/// Aus cbuf von Pos. cPos weg txLen characters rausloeschen.
/// \endcode

  long   l1;


  // printf("UTX_cut1|%s| %d %d %d\n",cbuf,*bufSiz,cPos,txLen);



  l1 = cPos + txLen;

  memmove(&cbuf[cPos], &cbuf[l1], *bufSiz - l1);

  *bufSiz -= txLen;

  cbuf[*bufSiz] = '\0';

  // printf("ex UTX_cut1|%s| %d\n",cbuf,*bufSiz);


  return 0;

}


//=====================================================================
  int UTX_cut2 (char *s1, long lDel) {
//=====================================================================
/// \code
/// UTX_cut2               cut lDel chars out of string s1
/// starting from position s1 remove <lDel> characters.
/// \endcode

  long   lTot, lMov;
  char   *p1;




  lTot = strlen (s1);
  lMov = lTot - lDel;

  // printf("UTX_cut2 |%s| %ld tot=%ld mov=%ld\n",s1,lDel,lTot,lMov);

  if(lDel > lTot) {
    TX_Error ("UTX_cut2 E001 |%s|",s1);
    return -1;
  }

  memmove (s1, &s1[lDel], lMov);

  s1[lMov] = '\0';


  // remove following delimiters
  p1 = &s1[lMov] - 1;
  while(!UTX_ckc_Del2(*p1)) {
    *p1 = '\0';
    --p1;
  }



  // printf("ex UTX_cut2 |%s| %ld %ld\n",s1,lTot,lMov);


  return 0;

}


//=====================================================================
  int UTX_insert1 (char *cbuf, long *bufSiz, char *auxBuf, long cPos) {
//=====================================================================
/// \code
/// UTX_insert1            insert Textstring into Textbuffer
/// den textstring auxBuf in den Buffer cbuf an Position cPos einfuegen.
/// Die momentane Size von cbuf ist bufSiz.
/// 
/// long i1;
/// i1 = strlen(cbuf);
/// UTX_insert1(cBuf, &i1, "FSUB ", 0);
/// \endcode


  long actSiz, auxSiz, l1;


  // printf("UTX_insert1 |%s| %ld |%s| %ld\n",cbuf,*bufSiz,auxBuf,cPos);

  actSiz = *bufSiz;
  auxSiz = strlen(auxBuf);

  l1 = actSiz - cPos;



  if((cPos < 0)||(cPos >= *bufSiz)) {

    strcat(cbuf, auxBuf);


  } else {
    // auseinander; +1 mit dem bin = am Ende. +1 am 2004-06-21.
    memmove(&cbuf[cPos+auxSiz], &cbuf[cPos], l1+1);
    // hinein
    memcpy(&cbuf[cPos], auxBuf, auxSiz);
  }



  *bufSiz += auxSiz;


  return 0;

}


//================================================================
  int UTX_dump_cnl (char *cPos, int maxLen) {
//================================================================
/// \code
/// print next <maxLen> characters, but stop at \n.
///    printf("|");UTX_dump_cnl(p1,40);printf("|\n");
/// DOES NOT OUTPUT the closing \n
/// \endcode

// see also UTX_dump__

// "dynamic" format "%<len>.<len>s"


  int  ll;
  char *p1;

  // printf("UTX_dump_cnl %d |%s|\n",maxLen,cPos);

  if(!cPos) return -1;

  p1 = strchr(cPos, '\n');

  if(p1) {
    ll = p1 - cPos;
    // printf(" ll=%d\n",ll);
    if(ll > maxLen) ll = maxLen;
    UTX_dump_c__ (cPos, ll);

  } else {
    ll = strlen(cPos);
    if(ll > maxLen) ll = maxLen;
    UTX_dump_c__ (cPos, ll);
  }

  return 0;

}


//================================================================
  int UTX_dump_c__ (char *cPos, int cLen) {
//================================================================
/// \code
/// print next <cLen> characters;  \n will be printed ..
///    printf(" Line %d |",i1);
///    UTX_dump_c__ (cpos, 30);
///    printf("|\n");
/// \endcode

// "dynamic" format "%<cLen>.<cLen>s"


  char  fmt[256];



  // sprintf(fmt, "%s%%%d.%ds%s",tx1,ll,ll,tx2);
  sprintf(fmt, "%%%d.%ds",cLen,cLen);
  printf(fmt, cPos);

  return 0;

}


//================================================================
  int UTX_dump_word (char *cPos, char *sDel) {
//================================================================
// print word delimited by one of sDel-chars
// sDel   delimiters;  eg " {["
 
  int   cLen;
  char  *p1, fmt[64];


  cLen = strcspn (cPos, sDel); // find 1. pos of one of the sDel-chars

  // if(p1 == NULL) cLen = strlen (cPos);
  // else cLen = p1 - cPos;

  sprintf(fmt, "%%%d.%ds",cLen,cLen);
  printf(fmt, cPos);



  return 0;

}


//================================================================
  int UTX_dump_s__ (char *cPos, int cNr) {
//================================================================
/// \code
/// UTX_dump_s__    dump String (replace \n\r\t\0)  max cNr chars;
/// Es wird auch '\0' und die folgenden chars ausgegeben !!
/// \endcode

  int  i1;
  char cbuf[80], cw[4];

  cbuf[0] = '\0';
  cw[1] = '\0';

  for(i1=0; i1<cNr; ++i1) {
    if(*cPos == '\n') strcat(cbuf, "\\n");
    else if(*cPos == '\r') strcat(cbuf, "\\r");
    else if(*cPos == '\t') strcat(cbuf, "\\t");
    else if(*cPos == '\0') strcat(cbuf, "\\0");
    else {cw[0] = *cPos; strcat(cbuf, cw);}
    ++cPos;
  }

  printf("%s",cbuf);

  return 0;

}
 

//================================================================
  int UTX_dump_wTab (char *wTab[]) {
//================================================================
/// \code
/// UTX_dump_wTab          dumpt table of words
/// \endcode

  int      i1 = 0;
  char     **pa;

  pa = wTab; 

  while (*pa) {
    printf(" wTab[%d] = |%s|\n",i1,*pa);
    ++pa;
    ++i1;
  }


  return 0;

}


//================================================================
  int UTX_dump_p1p2 (char *p1, char *p2) {
//================================================================
/// \code
/// UTX_dump_p1p2          dump string from ps to pe
/// Usage:
/// printf(" p1-p2 = |n");UTX_dump_p1p2(p1,p2);printf("|\n");
/// \endcode

  int    ii;

  ii = p2 - p1;

  UTX_dump_s__  (p1, ii);

  return 0;

}


//================================================================
  int UTX_dump__ (char *cPos, int maxLen) {
//================================================================
/// \code
/// UTX_dump__   dump String (replace \n\r\t)
/// Es wird bei '\0' gestoppt.
/// \endcode

  int  i1;
  char cbuf[256], cw[4];

  cbuf[0] = '\0';
  cw[1] = '\0';

  if(maxLen > 254) maxLen=254;

  for(i1=0; i1<maxLen; ++i1) {
    if(*cPos == '\n') strcat(cbuf, "\\n");
    else if(*cPos == '\r') strcat(cbuf, "\\r");
    else if(*cPos == '\t') strcat(cbuf, "\\t");
    else if(*cPos == '\0') {*cPos = '\0'; break;}
    else {cw[0] = *cPos; strcat(cbuf, cw);}
    ++cPos;
  }

  printf("%s",cbuf);

  return 0;

}

//================================================================
  int UTX_cp_print (char *txo, char *txi, int maxLen) {
//================================================================
/// \code
/// add printable characters from txi --> txo; but max. maxLen Chars.
/// size of txo must be (maxLen + 1) !
/// Change nonprintable characters to '?'
/// \endcode

  int           ii, io;
  unsigned char c1;


  ii = 0;
  io = 0;

  L_nxt:
    c1 = txi[ii];
    if(c1 > 126) {c1 = '?'; goto L_add_1;}
    if(c1 == '\0') goto L_exit;

    if(c1 < 32) {
      if(c1 == '\n')      {c1 = 'n'; goto L_add_2;}
      else if(c1 == '\r') {c1 = 'r'; goto L_add_2;}
      else if(c1 == '\t') {c1 = 't'; goto L_add_2;}
      else                {c1 = '?'; goto L_add_1;}
    }


    L_add_1:
      txo[io] = c1;
      ++ii;
      ++io;
      if(io < maxLen) goto L_nxt;
      goto L_exit;


    L_add_2:
      txo[io] = '\\';
      ++io;
      if(io >= maxLen) goto L_exit;
      txo[io] = c1;
      ++ii;
      ++io;
      if(io < maxLen) goto L_nxt;



  L_exit:
  txo[io] = '\0';

  printf(" |%s|\n",txo);

  return io;

}


//================================================================
  int UTX_cp_left (char *sOut, char *sIn, int cNr) {
//================================================================
/// \code
/// copy string, max. cNr chars from left side of string sIn -> sOut;
///   ATTENTION: size of sOut must be (cNr + 1) !
///   - terminates always with '\0'
///   sOut may not overlap sIn.
///
/// RetCod: nr of characters copied (excluding '\0')
///
/// UTX_cp_left (s2, s1, sizeof(s2) - 1);
///
/// \endcode

  int    l1;

  l1 = strlen(sIn);

  if(l1 < cNr) cNr = l1;

  memcpy (sOut, sIn, cNr);

  sOut[cNr] = '\0';   // terminate

    // printf("UTX_cp_left |%s|%s|\n",sOut,sIn);


  return l1;

}


//================================================================
  int UTX_cp_right (char *sOut, char *sIn, int cNr) {
//================================================================
/// \code
/// UTX_cp_right           copy n chars from right side of string
///   ATTENTION: size of sOut must be (cNr + 1) !
///   - terminates always with '\0'
///   sOut may not overlap sIn.
/// 
/// RetCode: 0=equal; else=notEqual.
///
/// UTX_cp_right (s2, s1, sizeof(s2) - 1);
///
/// \endcode

  int   l1, l2;

  l1 = strlen(sIn);

  if(l1 < cNr) cNr = l1;

  l2 = l1 - cNr;

  // strcpy(sOut, &sIn[l2]);

  memcpy (sOut, &sIn[l2], cNr);

  sOut[cNr] = '\0';   // terminate

    // printf("ex UTX_cp_right |%s|%s| %d\n",sOut,sIn,cNr);

  return cNr;

}


//===================================================================
  int UTX_cpLn_2cp (char *cbuf, char *pos1, char *pos2, int bufSiz) {
//===================================================================
/// \code
/// UTX_cpLn_2cp      copy line between 2 pointers; del ending blanks, lf.
/// Input:
///   pos1        start of line to be copied
///   pos2        start of next line
/// \endcode


  int   i1;
  char  *tpos;


  i1 = pos2 - pos1;
  if(i1 >= bufSiz) i1 = bufSiz - 1;
  if(i1 < 1) i1 = 0;

  strncpy(cbuf, pos1, i1);
  cbuf[i1] = '\0';

  L_nxtPos:
    --i1;
    if(i1 > 1) {
      tpos = &cbuf[i1];
      if((*tpos  == ' ')  ||
         (*tpos  == '\t') ||
         (*tpos  == '\n') ||
         (*tpos  == '\r'))   {
        *tpos    = '\0';
        goto L_nxtPos;
      }
    }

    // printf("ex UTX_cpLn_2cp |%s|\n",cbuf);

  return 0;

}


//================================================================
  int UTX_cp_Line (char *cTo, char **cNxt, char *cFrom, int maxLen) {
//================================================================
/// \code
/// UTX_cp_Line            copy a Line (terminated by '\n')
/// Input:
///   cFrom        string in, 
///   maxLen       size of cTo, including "\0" (minimum 2)
/// Output:
///   cTo          string out; \n is not included;
///                  terminated with "\0"; length=maxlen or less 
///   cNxt         position of 1 char of next Line
///   retCod       length of cTo
/// \endcode

  int  ll;
  char *p1;

  // printf("UTX_cp_Line %d |%s|\n",maxLen,cFrom);

  --maxLen;  // must include \0


  p1 = strchr(cFrom, '\n');

  if(p1) {
    ll = p1 - cFrom;
    *cNxt = p1 + 1;

  } else {
    // no \n in string;
    ll = strlen(cFrom);
    *cNxt = NULL;
  }

  if(ll > maxLen) ll = maxLen;
  strncpy (cTo, cFrom, ll);
  cTo[ll] = '\0';

  printf("UTX_cp_Line |%s| %d %d\n",cTo,maxLen,ll);

  return ll;

}


//================================================================
  int UTX_setup_set (char *fn, char *par, char *val) {
//================================================================
/// \code
/// UTX_setup_set          add/replace/delete parameter in file
/// delete parameter if val = NULL
/// max. Linelength = 1020
/// Line must stat with <par>; par-val-Delimiter must be a blank !
/// # starts a comment-line.
/// RetCod:  -2  Error (open, write tempFile error)
///          -3  Error (tempFile already exists; other process hanging ?)
///           0  parameter replaced
///           1  parameter added
///           2  new file created
///
/// Using temporary file <fn>_temp; on init delete all *_temp ?
/// \endcode

// read all lines and write immediate (into tempfile);
// if parameter comes in: replace;
// parameter not in file: add
// replace file

  int    i1, lp, irc;
  char   s1[1024];
  FILE   *fpo, *fpi;


  printf("UTX_setup_set |%s|%s|%s|\n",fn,par,val);


  // create tempFilename 
  sprintf(s1, "%s_temp", fn);
    // printf(" temp=|%s|\n",s1);


  // test if the tempfilename of fn already exists;
  if(OS_checkFilExist(s1,1)) {
    // yes: wait until its gone ..
    i1 = OS_file_waitExist (s1, 25);
    if(i1 < 0) {
      TX_Error ("UTX_setup_set E-3");
      return -3;
    }
  }


  L_start1:
  irc = 1;
  lp = strlen(par);
    // printf(" lp=%d\n",lp);



  // open infile
  if ((fpi = fopen (fn, "r")) == NULL) {
    irc = 2;
  }

  // open tfn
  if ((fpo = fopen (s1, "w")) == NULL) {
    fclose (fpi);
    return -2;
  }


  if(!fpi) goto L_add;       // create new file


  while (!feof (fpi)) {
    if (fgets (s1, 1020, fpi) == NULL) break;

    // skip commentlines starting with #
    if(s1[0] == '#') goto L_wr9;

    // copy lines not starting with par
    if(strncmp(s1, par, lp)) goto L_wr9;
    // next char must be a delimiter
    if(s1[lp] != ' ') goto L_wr9;

    // found parameter;
    irc = 0;

    // delete if val == NULL ..
    if(!val) continue;

    // replace ..
      // printf(" replace |%s|%s|\n",s1,val);
    sprintf (s1, "%s %s\n", par, val);


    // copy line ..
    L_wr9:
      fprintf (fpo, "%s", s1);
  }


  // add parameter
  L_add:
    printf(" irc=%d\n",irc);
  if(irc) {
    if(val) fprintf (fpo, "%s %s\n", par, val);  // 2014-01-14 if(val)
  }

  if(fpi) fclose (fpi);
  fclose (fpo);


  // replace file
  sprintf(s1, "%s_temp", fn);
  OS_file_rename (s1, fn);  // (old, new)


  return irc;

}

//=======================================================================
  int UTX_setup_get__ (char *cval, char *ctyp, char *fnam) {
//=======================================================================
/// \code
/// UTX_setup_get__        get parameters (typ, value) from setup-file
///             skips lines starting with # in setup-file
/// Input:
///   ctyp      keyWord (1.word) in setupFile.
/// Output:
///   cval      copy of the value (the 2.word) in the file; size up to 250 chars.
///             unmodified if keyWord not found
/// RetCod:     0=OK; -1=ctyp not found.
/// \endcode


  int    i1;
  char   cbuf[256], *p1;
  FILE  *fpi;


  // printf("UTX_setup_get__ |%s|\n",ctyp);


  // datei neu oeffnen
  // printf("UTX_setup_get fopen|%s|\n",fnam);
  if((fpi=fopen(fnam,"r")) == NULL) {
    TX_Print("UTX_setup_get__ E001 %s",fnam);
    goto L_errEx;
  }

  i1 = strlen(ctyp);

  L_read:
  for(;;) {
    if(fgets(cbuf, 250, fpi) == NULL) goto L_eof;

    if(*cbuf == '#') continue;
    if(cbuf[i1] != ' ') continue;
    if(strncmp(cbuf, ctyp, i1)) continue;
    break;
  }

  // line contains ctyp
  ++i1;
  p1 = &cbuf[i1];
  while (*p1  == ' ') ++p1;
  strcpy(cval, p1);
  UTX_CleanCR (cval);

    // printf("ex UTX_setup_get__ |%s|%s|\n",ctyp,cval);

  fclose (fpi);
  return 0;


  L_eof:
    fclose (fpi);

  L_errEx:
    // *cval = '\0';
      // printf("ex UTX_setup_get EOF |%s|%s|\n",ctyp,cval);
    return -1;

}


//=======================================================================
  int UTX_setup_get (char *cval, char *ctyp, char *fnam, int mode) {
//=======================================================================
/// \code
/// UTX_setup_get          get parameters (typ, value) from setup-file
///             skips lines starting with # in setup-file
/// Input:
///   ctyp      keyWord (1.word) in setupFile.
///   mode      0=quiet, 1=display errormessage
/// Output:
///   cval      copy of the value (the 2.word) in the file; size up to 80 chars.
///             unmodified if keyWord not found
/// RetCod:     0=OK; -1=ctyp not found.
/// \endcode

// einlesen der Defaultwerte; zB Reset Modelsize / Reset Textsize ...
// ACHTUNG: Lesen bis EOF (irc < 0 oder *ctyp == NULL)
// Datei enthaelt TYP wert
// es werden alle Werte geliefert, deren Typ das gesuchte ctyps enthaelt.
// Es wird nur ein Wort (das erste) geliefert !

// Beispiel: ctyps "TEXTSIZ" returns "TEXTSIZ", dann "DIMTEXTSIZ",
//   dann leer (EOF).
// # sind Kommentarzeilen
// ctyp = "": kein weiterer Wert mehr gefunden. (test: if (*ctyp) ..)
// 
//  char   cbuf1[128], cbuf2[128], cbuf3[128];
//  sprintf(cbuf3,"%sxa%cgCAD3D.rc",OS_get_bas_dir(),fnam_del);
//  L_read:
//    UTX_setup_get (cbuf1, cbuf2, "ZBUFSIZ", cbuf3);
//    if(*cbuf2) {
//      //if(!strcmp(cbuf2,"NTEXTSIZ")) {
//      i1 = atoi(cbuf1);
//      goto L_read;
//    }




  char   cbuf[128], *p1, *p2;
  FILE  *fpi;


  // printf("UTX_setup_get |%s|\n",ctyp);


  // datei neu oeffnen
  // printf("UTX_setup_get fopen|%s|\n",fnam);
  if((fpi=fopen(fnam,"r")) == NULL) {
    if(mode > 0) TX_Print("UTX_setup_get E001 %s",fnam);
    goto L_errEx;
  }


  L_read:
  for(;;) {
    if(fgets(cbuf, 126, fpi) == NULL) goto L_eof;
 
    if(*cbuf == '#') continue;
    if(!strstr(cbuf, ctyp)) continue;
    break;
  }

  // line contains ctyp
  UTX_CleanCR (cbuf);
    // printf("   setup_get: |%s|\n",cbuf);
  p1 = cbuf;
  while (*p1  == ' ')  { ++p1; }
  p2 = strchr(p1, ' ');
  if(!p2) { 
    if(mode > 0) TX_Print("UTX_setup_get E002");
    goto L_eof;
  }

  *p2 = '\0';                           // delimit parameter - value
  ++p2;
  while (*p2  == ' ')  { ++p2; }
  // p1 = strpbrk(p1, ' \0');
  p1 = strchr(p2, ' ');
  if(p1) *p1 = '\0';
  strcpy(cval, p2);

    // printf("ex UTX_setup_get |%s|%s|\n",ctyp,cval);

  fclose (fpi);
  return 0;


  L_eof:
    fclose (fpi);

  L_errEx:
    // *cval = '\0';
      // printf("ex UTX_setup_get EOF |%s|%s|\n",ctyp,cval);
    return -1;

}


//================================================================
  int UTX_setup_modw (char *fn, char *par, char *wNew, int wNr) {
//================================================================
// UTX_setup_modw         add/replace word of value in param-value-file
// max. Linelength = 1020; delimiters are  one blank.
// Line (starting with par) must already exist.
// # starts a comment-line.
// RetCod:  -1  Error; parameter did not yet exist
//          -2  Error (open, write tempFile error)
//          -3  Error (tempFile already exists; other process hanging ?)
//           0  parameter replaced
//
// Using temporary file <fn>_temp; on init delete all *_temp ?

// read all lines and write immediate (into tempfile);
// if parameter comes in: replace;
// parameter not in file: add
// replace file

  int    i1, lp, irc;
  char   s1[1024];
  FILE   *fpo, *fpi;


  // printf("UTX_setup_modw |%s|%s|%s| %d\n",fn,par,wNew,wNr);


  // create tempFilename 
  sprintf(s1, "%s_temp", fn);
    // printf(" temp=|%s|\n",s1);



  // test if the tempfilename of fn already exists;
  if(OS_checkFilExist(s1,1)) {
    // yes: wait until its gone ..
    i1 = OS_file_waitExist (s1, 25);
    if(i1 < 0) {
      TX_Error ("UTX_setup_modw E-3");
      return -3;
    }
  }


  L_start1:
  lp = strlen(par);
    // printf(" lp=%d\n",lp);


  // open infile
  irc = -1;
  if ((fpi = fopen (fn, "r")) == NULL) {
    return irc;
  }

  // open tfn
  if ((fpo = fopen (s1, "w")) == NULL) {
    fclose (fpi);
    return -2;
  }

  while (!feof (fpi)) {
    if (fgets (s1, 1020, fpi) == NULL) break;

    // skip commentlines starting with #
    if(s1[0] == '#') goto L_wr9;

    // copy lines not starting with par
    if(strncmp(s1, par, lp)) goto L_wr9;
    // next char must be a delimiter
    if(s1[lp] != ' ') goto L_wr9;

    // found parameter; replace ..
      // printf(" replace |%s|%s| %d\n",s1,wNew,wNr);
    UTX_chg_wordn (s1, wNew, wNr, sizeof(s1));
    irc = 0;


    // copy line ..
    L_wr9:
      fprintf (fpo, "%s", s1);
  }

  fclose (fpi);
  fclose (fpo);

  if(irc) return irc;    // parameter not found

  // replace file
  sprintf(s1, "%s_temp", fn);
  OS_file_rename (s1, fn);  // (old, new)

  return irc;

}


//================================================================
  int UTX_setup_decs (char *s1, char **pv) {
//================================================================
/// \code
/// decode setup-string; separates parameter, returns valPos.
/// Input:
///   s1      parameter blank value
/// Output:
///   s1      parameter (s1 modified !) separated with '\0'
///   pv      startposition of value in s1
/// RetCod    0=ok; -1=no delimiter (blank) found
/// \endcode

  char    *p1;

  // printf(" setup_dec |%s|\n",s1);

  p1 = strchr(s1, ' ');
  if(!p1) return -1;

  *p1 = '\0';
  ++p1;

  *pv = p1;

  // printf(" setup_dec |%s|%s|\n",s1,*pv);

  return 0;

}


//================================================================
  int UTX_htm_fop (FILE **fp, char *fnam) {
//================================================================
/// UTX_htm_fop            open html-textfile


  printf("UTX_htm_fop |%s|\n",fnam);

  if((*fp = fopen (fnam, "w")) == NULL) { return -1; }
  fprintf(*fp, "<HTML>\n<HEAD>\n</HEAD>\n<BODY>\n<PRE>\n");
  
  // if((OS_stdout__ (0, fnam))  < 0) { return -1; }
  // printf("<HTML>\n<HEAD>\n</HEAD>\n<BODY>\n<PRE>\n");

  return 0;

}


//================================================================
  int UTX_htm_fcl (FILE **fp) {
//================================================================
/// UTX_htm_fcl            close html-textfile

  printf("UTX_htm_fcl\n");
  fprintf(*fp, "</PRE>\n</HTML>\n");
  fclose(*fp);

  // printf("</PRE>\n</HTML>\n");
  // OS_stdout__ (1, NULL);

  printf("ex UTX_htm_fcl\n");

  return 0;

}


/*====================== EOF ==============================*/
