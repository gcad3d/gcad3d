//     ut_txfil.c           Reiter 17.9.2000
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
2001-11-09 UTF_GetLen0 neu zu. RF.
2001-11-01 UTF_GetPosLnr korr. RF.
2001-06-23 UTF_GetPosLnr neu. RF.
2001-06-22 FilBuf1-Functions zu. RF.
2001-06-15 UTF_GetLinNr LinendChar f. Unix -> LF. RF.
2000-10-20 UTF_wri_file neu zu. RF.

-----------------------------------------------------
*/
#ifdef globTag
 void UTF(){}
#endif
/*!
\file  ../ut/ut_txfil.c
\brief textfile in memory
\code
=====================================================
List_functions_start:

UTF_clear_          Memory loeschen
UTF_offset_         eine Charpos (ev ex UTF_GetLinNr) in Offset umrechnen
UTF_add_line        eine Zeile hinten anfuegen
UTF_add_nl          ein '\n' hintanfuegen, wenn nicht vorh.
UTF_add_file        Buffer1 in den Hauptbuffer umkopieren.
UTF_add_fil__       Datei hintanhaengen
UTF_wri_file        Memory -> Datei rausschreiben
UTF_wri_f__         Memory -> Datei rausschreiben
UTF_file_Buf1__     Buffer1 in eine Datei rausschreiben

UTF_txt_chg         1-n chars aendern
UTF_chg_line        modify line in mem
UTF_chg_lnPos       change Line im mem starting at cPos                 UNUSED
UTF_comment_chg     change (insert/remove char cs (eg '_')) first char  UNUSED
UTF_comment_line    modify 1. (last) char of line
UTF_wrf_line        eine Zeile in ein File rausschreiben

UTF_EMPTY           test if model is empty                                  INLINE
UTF_GetPos2         returns Startpos & Endpos of Buf0.
UTF_GetLen0         liefert die Laenge des UTF_FilBuf0 UTF_FilBuf0Len.
UTF_GetLinNr        Liefert Zeile mit Zeilennummer lNr u. ihre Laenge
UTF_GetLineTx       Zeile, die mit sText beginnnt, zurueckliefern
UTF_GetLinLen       length of line excluding LF
UTF_GetPosLnr       Startposition und Zeilenlaenge von zeile lNr liefern.
UTF_GetLnrPos       ZeilenNr einer Position liefern
UTF_GetnextLnPos    Position der nachfolgenden Zeile liefern
UTF_GetPrevLnPos    Position der vorherigen zeile liefern

UTF_find_tx1        Zeilennummer der Zeile, die tx1 als erste enthaelt, suchen

UTF_add2_line       eine Zeile zwischenspeichern; wenn laenger 80 char teilen.
UTF_get1_LnPos      find startPos of line from LineNr
UTF_get1_LnLen      length of line starting at lStart in UTF_FilBuf1
UTF_get1_nxtLnPos   naechste Zeile aus UTF_FilBuf1 lesen
UTF_chg1_Ln_cpCs    change Line in Buf1
UTF_insert1         Zwischenspeicher im Memory an der aktPos einfuegen
UTF_insertTxt       insert text

UTF_Buf1_GetPos2    returns Startpos & Endpos of Buf1.
UTF_clear1          den Zwischenspeicher UTF_FilBuf1 loeschen
UTF_add1_file       Eine Datei into Buffer UTF_FilBuf1.
UTF_add1_line       eine Zeile in UTF_FilBuf1 hintanfuegen
UTF_add1_last_add   add txt to last line in UTF_FilBuf1  (add objName)
UTF_add1_strLF      add a line, which is terminated with LF, to Buffer1
UTF_file_Buf1__     Buffer1 in eine Datei rausschreiben
UTF_get_Buf1        copy Buffer1 -> memspc

UTF_del_line1       Eine Zeile im Memory loeschen
UTF_del_start       bis "text" Memory loeschen
UTF_del_rest        ab "text" Memory loeschen
UTF_del_text        delete from cPos to cPos

UTF_dump__          display memInhalt
UTF_dump1__         display memInhalt of Buffer1

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



  Fortsetzungszeilen werden von UTF_GetPosLnr UTF_GetLnrPos UTF_GetLinNr
    wahrscheinlich falsch behandelt !!

  man koennte UTF_add_file durch UTF_add1_file ersetzen.


Eine Textdatei im Memory bearbeiten.ipt
Es gibt 2 Textbuffer; den Hauptbuffer Buf0 und Buf1.
Der Memorybuffer dazu hier lokal.
  Buf0 = UTF_FilBuf0    (UTF_alloc1_)
  Buf1 = UTF_FilBuf1

Linendcharacters sind bei Unix/Windows unterschiedlich
  (Unix: LF; Win: CRLF !)


ZeilenNummer letzte Zeile holen:
  ED_lnr_act = UTF_GetLnrPos (UTF_GetLen0());



Compile:
cc -c ut_txfil.c


Offline testen: siehe OFFLINE

testlink:
cc  ut_txfil.c -Wall ../gCAT/TX.o ../gCAT/ut_txt.o ../gCAT/ut_geo.o\
 -lm && a.out

*/


#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_txt.h"      // term_buf
#include "../ut/ut_TX.h"       // TX_Print
#include "../ut/ut_txfil.h"

#include "../xa/xa_mem.h"               // mem_cbuf1


#define UTF_BUF0_INC          2000000
#define UTF_BUF1_INC          1000000


char       *UTF_FilBuf0 = NULL;
long       UTF_FilBuf0Siz  = 0;  // die allocierte DatenbereichSize
long       UTF_FilBuf0Len  = 0;  // die belegte DatenbereichSize



char       *UTF_FilBuf1 = NULL;
long       UTF_FilBuf1Siz = 0;               // die aktuelle Filesize
long       UTF_FilBuf1Len = 0;               // die aktuelle Filesize



// static char       UTF_buf1[256];



//################################################################
//################################################################
#ifdef OFFLINE

// nachfolgend ein testprogramm nur fuer UTF-Tests.
// cd ../ut
// cc ut_txfil.c ut_TX.o ut_txt.o ut_mem.o ut_os_aix.o -DOFFLINE&&./a.out

// xa_mem.h:
char mem_cbuf1[mem_cbuf1_SIZ];


int main() {


  char* cpos;
  int  lNr, ll, i1;
  char lbuf[256];

  printf("+++++++++++++Start tst_txfil\n");


  // erstmal eine Datei einlesen
  // system("cat /p2/fwork/dev/xa/00.apt");
  UTF_clear_ ();
  UTF_add_file ("../tmp/Model");

  // lNr = UTF_GetLnrPos (14);

  UTF_ut_dispDat (); // display alle Lines

  // change Line 35
  UTF_chg_line (35, "Neue Zeile 35");  UTF_ut_dispDat ();

  // cpos = UTF_GetPosLnr (&ll, 2);

  // suchen des naechsten Vorkommens von txt
  // cpos = UTF_search_ ("p1");

  // Zeile 3 holen
  // cpos = UTF_GetLinNr (lbuf, &ll, 3);
  // i1 = UTF_offset_ (cpos);

  // Memory -> Datei rausschreiben
  // UTF_clear1   ();
  // UTF_add1_line ("Zeile 1");
  // UTF_insert1 (8);


  return 0;

}

int UTF_ut_dispDat () {

  char* cpos;
  long  i1, ll;

  for(i1=1; i1<9999; ++i1) {
    cpos = UTF_GetPosLnr (&ll, i1);
    if(cpos == NULL) break;
    printf(" Line %d |",i1);
    UTX_dump_cnl (cpos);
    printf("|\n");
  }
}


int DL_wri_dynDat (FILE *fpo) { fprintf(fpo, "DUMMYFUNKTION !!\n"); }

#endif
//################################################################
//################################################################





//================================================================
  char* UTF_get1_LnPos (long lNr) {
//================================================================
/// \code
/// UTF_get1_LnPos        find startPos of line from LineNr
/// lNr of first line = 1
/// RetCod:  NULL for EOF or (lNr<1); else first char of line.
/// \endcode


  int     ii;
  char    *lp1, *lp2;

  if(lNr < 1) return NULL;

  // get startPos of buffer
  ii = 1;
  lp2 = UTF_get1_nxtLnPos (NULL);

  L_nxt_ln:
    if(ii == lNr) goto L_done;
    lp1 = lp2;
    if(lp2 == NULL) goto L_done;
    lp2 = UTF_get1_nxtLnPos (lp1);
    ++ii;
    goto L_nxt_ln;


  L_done:
    return lp2;

}


//================================================================
  char* UTF_get1_nxtLnPos (char *actPos) {
//================================================================
/// \code
/// UTF_get1_nxtLnPos            naechste Zeile aus UTF_FilBuf1 lesen
/// Input:
///   actPos                 active Line;
///   if(actPos == NULL)     get first Line;erste zeile liefern.
/// Output:
///   retCod                 position of next line foooollowing actPos
///   if retCod == NULL:     no more line ..
/// 
/// see UTF_GetnextLnPos
/// \endcode


  char   *nxtPos;

  // printf("UTF_get1_nxtLnPos |%s|\n",cpos);

  if(actPos == NULL) return UTF_FilBuf1;

  nxtPos = strchr(actPos, '\n');
  if(nxtPos == NULL) return NULL;

  ++nxtPos;

  if(nxtPos >= (UTF_FilBuf1 + UTF_FilBuf1Len)) return NULL;

  // printf("ex UTF_get1_nxtLnPos |");UTX_dump_cnl(nxtPos,50);printf("|\n");

  return nxtPos;

}


//================================================================
  int UTF_wrf_line (long lNr, char *fnam) {
//================================================================
/// UTF_wrf_line        eine Zeile in ein File rausschreiben

// see also UTF_wrf_line


  long     lLen;
  char     *p1;
  FILE     *fp1;


  // printf("UTF_wrf_line %ld |%s|\n",lNr,fnam);


  // get charpos p1 and lenLength lLen from LineNr lNr
  p1 = UTF_GetPosLnr (&lLen, lNr);
    // printf("UTF_wrf_line lNr=%ld ll=%d |",lNr,lLen);
    // UTX_dump_c__ (p1, lLen); printf("|\n");

  // write Line -> File
  if((fp1=fopen(fnam,"wb")) == NULL) {
    TX_Print("UTF_wrf_line E001 |%s|",fnam);
    return -1;
  }
  fwrite(p1, lLen, 1, fp1);
  fclose(fp1);

  return 0;

}


//================================================================
  int UTF_get_Buf1 (char *cbuf, int maxSiz) {
//================================================================
/// UTF_get_Buf1        copy Buffer1 -> memspc

  printf("UTF_get_Buf1\n");

  if(UTF_FilBuf1Len < maxSiz) {
    memcpy ((void*)cbuf, (void*)UTF_FilBuf1, UTF_FilBuf1Len+1);

  } else {
    TX_Error("UTF_get_Buf1 EOM");
    return -1;
  }

  return 0;

}


//================================================================
  int UTF_file_Buf1__ (char *fnam) {
//================================================================
/// UTF_file_Buf1__       Buffer1 in eine Datei rausschreiben

  FILE     *fp1;

  // printf("UTF_file_Buf1__ |%s|\n",fnam);

  // write Line -> File
  if((fp1=fopen(fnam,"wb")) == NULL) {
    TX_Print("UTF_wrf_line E001 |%s|",fnam);
    return -1;
  }

  UTF_file_Buf1_w (fp1);

  fclose(fp1);

  return 0;

}


//================================================================
  int UTF_file_Buf1_w (FILE *fp1) {
//================================================================
/// UTF_file_Buf1__       Buffer1 in eine Datei rausschreiben


  // printf("UTF_file_Buf1_w %ld\n",UTF_FilBuf1Len);
  // printf("%s\n",UTF_FilBuf1);


  if(fwrite(UTF_FilBuf1, UTF_FilBuf1Len, 1, fp1) < 1) {
    TX_Error("UTF_file_Buf1_w E001");
    return -1;
  }

  return 0;

}


/* UNUSED
//================================================================
  int UTF_comment_chg (long lNr, char cs) {
//================================================================
// change (insert/remove char cs (eg '_')) first char
//   char cs (eg '_'))
// see also APED_undo_do

  int    mode;
  long   ll;
  char   *lPos;


  lPos = UTF_GetPosLnr (&ll, lNr);

    printf("UTF_comment_chg %ld %c |%c|\n",lNr,cs,*lPos);


  if(*lPos == cs) mode = 0;
  else            mode = 1;

  // mode: 1=del, 0=restore
  UTF_comment_line (lPos, mode, &cs);


  return 0;

}
*/

//================================================================
  int UTF_comment_line (char *lPos, int mode, char *cs) {
//================================================================
/// \code
/// UTF_comment_line    modify 1. (last) char of line
/// mode: 0=Restore     restore line, remove first char of line
///       1=Delete      comment line, set first char of line = cs
/// cs    commentString, zB "_" - used only with mode 1
/// \endcode


  long    ii, ll;
  char    *le;


  // get offset of LineStart & len of line
  ii = UTF_offset_ (lPos);
  ll = UTF_GetLinLen (&le, lPos);


  // printf("UTF_comment_line mode=%d ll=%ld ii=%ld\n",mode,ll,ii);
  // UTX_dump_cnl (lPos, ll); printf("\n");


  if(mode == 0) goto L_restore;


  //================================================================
  // 1 = comment line; remove line: set first char of line = cs
  // check if last char of line is blank; if yes: modify line, else insert.
    // printf(" comment-out %c\n",UTF_FilBuf0[ii]);
  if(UTF_FilBuf0[ii] == *cs) goto L_exit;  // already commented ..
  if(UTF_FilBuf0[ii+ll] != ' ') {
    // insert cs as first character ...
      // printf(" insert\n");
    UTF_insertTxt (lPos, 1, cs);
    goto L_exit;
  }


  // copy line 1 char nach rechts, 
    // printf(" move\n");
  memmove(&UTF_FilBuf0[ii+1], &UTF_FilBuf0[ii], ll);

  UTF_FilBuf0[ii] = *cs;

  goto L_exit;




  //================================================================
  // restore: remove first char of line
  L_restore:
    // printf(" restore %c\n",UTF_FilBuf0[ii]);
  if(UTF_FilBuf0[ii] != *cs) goto L_exit;  // already uncommented ..
  // restore: remove 1. char
  // Zeile 1 char nach links; insert blank am LineEnd (to,from,len)
  memmove(&UTF_FilBuf0[ii], &UTF_FilBuf0[ii+1], ll);
  UTF_FilBuf0[ii+ll-1] = ' ';


  L_exit:

    // AP_mdl_modified_set ();

    // printf("ex UTF_comment_line:\n");
    // UTX_dump_cnl (&UTF_FilBuf0[ii], ll); printf("\n");
    // printf("%s\n",lPos);


  return 0;

}


//================================================================
  long UTF_get1_LnLen (char **lEnd, char *lStart) {
//================================================================
/// \code
/// UTF_get1_LnLen         length of line starting at lStart in UTF_FilBuf1
/// Output:
///   lEnd            position of next '\n' or '\0'
///   Retcod          (nr of characters ) - 1
/// \endcode
 
  long   ll;
    
    
  // printf("UTF_get1_LnLen %ld |",UTF_FilBuf1Len);
  // UTX_dump_cnl (lStart, 80);
  // printf("|\n");

    
  if(lStart >= &UTF_FilBuf1[UTF_FilBuf1Len]) {
    *lEnd = &UTF_FilBuf1[UTF_FilBuf1Len];
    return 0L;
  }


  *lEnd = strchr(lStart, '\n');  // das naechste LF suchen
  if((lEnd == NULL) ||
     (*lEnd > &UTF_FilBuf1[UTF_FilBuf1Len])) {   // 2012-11-18  was ((lEnd > &..
    *lEnd = &UTF_FilBuf1[UTF_FilBuf1Len];
  }

  ll = *lEnd - lStart;

    // printf("ex UTF_get1_LnLen %ld\n",ll);

  return ll;

}


//================================================================
  long UTF_GetLinLen (char **lEnd, char *lStart) {
//================================================================
/// \code
/// UTF_GetLinLen          length of line starting at lStart in UTF_FilBuf0
/// Output:
///   lEnd            position of next '\n' or '\0'
///   Retcod          nr of characters
/// \endcode
 
  long   ll;


  // printf("UTF_GetLinLen %ld |",UTF_FilBuf0Len);
  // UTX_dump_cnl (lStart, 80);
  // printf("|\n");


  if(lStart >= &UTF_FilBuf0[UTF_FilBuf0Len]) {
    *lEnd = &UTF_FilBuf0[UTF_FilBuf0Len];
    return 0L;
  }


  *lEnd = strchr(lStart, '\n');  // das naechste LF suchen
  if((lEnd == NULL) ||
     (*lEnd > &UTF_FilBuf0[UTF_FilBuf0Len])) {   // 2012-11-18  was ((lEnd > &..
    *lEnd = &UTF_FilBuf0[UTF_FilBuf0Len];
  }

  ll = *lEnd - lStart;

    // printf("ex UTF_GetLinLen %ld\n",ll);

  return ll;

}


//================================================================
  int UTF_chg_lnPos (char *cPos, char *newRec) {
//================================================================
/// \code
/// change Line im mem starting at cPos
/// see also UTF_chg_line
/// \endcode

  long   l1, llold, llnew;
  char   *cp1;


  printf("UTF_chg_lnPos new:|%s|\n",newRec);
  printf("    act=|");UTX_dump_cnl(cPos,50);printf("|\n");


  if(cPos) {
    cp1 = strchr(cPos, '\n');
    if(cp1) {
      llold = cp1 - cPos + 1;
    } else {
      l1 = cp1 - UTF_FilBuf0;
      llold = UTF_FilBuf0Len - l1 + 1;
    }
    llnew = strlen(newRec);
      printf(" llold=%ld llnew=%ld\n",llold,llnew);


    MEM_chg_rec (UTF_FilBuf0, &UTF_FilBuf0Len,
                 newRec, llnew,
                 cPos, llold);

    // nun stimmt cPos von UTF_GetPosLnr nicht mehr !!
    // AP_mdl_modified_set();

  } else {
    printf("*** UTF_chg_line I001\n");
    UTF_add_line (newRec);
  }


  return 0;

}


//================================================================
  int UTF_chg_line (long lNr, char *newRec) {
//================================================================
/// \code
/// change Line im mem (lNr)
/// see also UTF_chg_lnPos
/// \endcode

  long   llold, llnew;
  char   *cPos;


  // printf("UTF_chg_line %d |%s|\n",lNr,newRec);

  cPos = UTF_GetPosLnr (&llold, lNr);

  if(cPos) {
    llnew = strlen(newRec);

    MEM_chg_rec (UTF_FilBuf0, &UTF_FilBuf0Len,
                 newRec, llnew, cPos, llold);

    // nun stimmt cPos von UTF_GetPosLnr nicht mehr !!
    // AP_mdl_modified_set();

  } else {
    printf("*** UTF_chg_line I001\n");
    UTF_add_line (newRec);
  }


  return 0;

}

//================================================================
  int UTF_chg1_Ln_cpCs (char *lPos, char *newLn) {
//================================================================
/// \code
/// UTF_chg1_Ln_cpCs         change Line starting at lPos in UTF_FilBuf1
/// Input:
///   lPos         position of line to be changed
///   newLn        0-terminated
/// 
/// see UTF_chg_line
/// see UTF_chg_lnPos UTF_insert1 UTF_del_line1
/// \endcode


  long   llold, llnew;
  char   *lEnd;


  // printf("UTF_chg1_Ln_cpCs |%s|",newLn);
  // UTX_dump_cnl (lPos, 80);
  // printf("|\n");



  if(lPos) {
    llnew = strlen(newLn);
    // llold = UTF_GetLinLen (&lEnd, lPos);
    llold = UTF_get1_LnLen (&lEnd, lPos);
      // printf(" llold=%ld llnew=%ld\n",llold,llnew);

    MEM_chg_rec (UTF_FilBuf1, &UTF_FilBuf1Len,
                 newLn, llnew, lPos, llold);

    // ++UTF_FilBuf1Stat;

  } else {
    printf("*** UTF_chg1_Ln_cpCs I001\n");
    UTF_add1_line (newLn);
  }


  return 0;

}


//============================================================================
  int UTF_alloc__ (long newSiz) {
//============================================================================
// realloc more memSpace for UTF_FilBuf0


  printf("::::UTF_alloc__ %ld %ld %ld\n",newSiz,UTF_FilBuf0Siz,UTF_FilBuf0Len);

  if(newSiz < UTF_FilBuf0Siz) return 0;


  UTF_FilBuf0Siz += 4;

  while (newSiz >= UTF_FilBuf0Siz) UTF_FilBuf0Siz += UTF_BUF0_INC;

  printf(" reall > %ld\n",UTF_FilBuf0Siz);

  UTF_FilBuf0 = (char*)realloc(UTF_FilBuf0, UTF_FilBuf0Siz);

  if(UTF_FilBuf0 == NULL) {
    TX_Error ("******** out of memory - UTF_alloc__ *********");
    return -1;
  }

  UTF_FilBuf0Siz -= 4;

  if(UTF_FilBuf0Len == 0) UTF_FilBuf0[0] = '\0';

  return 0;

}


//============================================================================
  int UTF_alloc1_ (long newSiz) {
//============================================================================

  if(newSiz < UTF_FilBuf1Siz) return 0;


  // printf("::::UTF_alloc1_ %d %d %d\n",newSiz,UTF_FilBuf1Siz,UTF_FilBuf1Len);
  // printf(" UTF_BUF1_INC=%d\n",UTF_BUF1_INC);


  UTF_FilBuf1Siz += 4;

  while (newSiz >= UTF_FilBuf1Siz) UTF_FilBuf1Siz += UTF_BUF1_INC;

  // printf(" reall > %d\n",UTF_FilBuf1Siz);

  UTF_FilBuf1 = (char*)realloc(UTF_FilBuf1, UTF_FilBuf1Siz);

  if(UTF_FilBuf1 == NULL) {
    TX_Error ("******** out of memory - UTF_alloc1_ *********");
    return -1;
  }

  UTF_FilBuf1Siz -= 4;

  if(UTF_FilBuf1Len == 0) UTF_FilBuf1[0] = '\0';

  return 0;

}


//============================================================================
  int UTF_dump__ () {
//============================================================================
/// UTF_dump__          display memInhalt


  printf("UTF_dump__ %ld %ld ==================================\n",
         UTF_FilBuf0Len,UTF_FilBuf0Siz);
  printf("%s\n",UTF_FilBuf0);
  printf("UTF_dump__ End ========================================\n");
  return 0;

}


//============================================================================
  int UTF_dump1__ () {
//============================================================================
/// UTF_dump1__          display memInhalt of Buffer1


  printf("UTF_dump1__ %ld %ld ==================================\n",
         UTF_FilBuf1Len,UTF_FilBuf1Siz);
  if(UTF_FilBuf1) printf("%s\n",UTF_FilBuf1);
  printf("UTF_dump1__ End ========================================\n");
  return 0;

}


//============================================================================
  int UTF_wri_file (char* fnam, int iniFunc(FILE*)) {
//============================================================================
/// \code
/// write memory -> file fnam; call iniFunc first.
///   iniFunc      eg DL_wri_dynDat0
/// rc = 0 = Error;
///      1 = OK.
/// \endcode



  FILE    *fpo=NULL;


  // printf ("UTF_wri_file: Filnam =|%s|\n",fnam);



  // Das Outputfile oeffnen
  // if ((fpo = fopen (fnam, "wb")) == NULL) {
  if ((fpo = fopen (fnam, "w")) == NULL) {
#ifdef _MSC_VER
    OS_err1 (__FUNCTION__);
#else
    OS_err1 (__func__);
#endif
    sprintf(mem_cbuf1, "beim Oeffen der Datei %s ****",fnam);
    TX_Error (mem_cbuf1);
    return 0;
  }


  // // write dynamicData (HIDE, .. ) if mode>0) eg DL_wri_dynDat0
  // if(mode > 0) DL_wri_dynDat (fpo);
  // if(mode > 0) DL_wri_dynDat (fpo);
  if(iniFunc != NULL) {
    iniFunc(fpo);
  }


/*
  // als ein Block rausschreiben probieren ..
  ilen = fwrite (UTF_FilBuf0, UTF_FilBuf0Len, 1, fpo);

  if(ilen < 0) {
    sprintf(mem_cbuf1, " Ausgabefehler Datei %s",fnam);
    TX_Error (mem_cbuf1);
    return 0;
  }
*/


  // write mem_cbuf1 -> open file; skip deleted lines ('_')
  UTF_wri_f__ (fpo);

  fclose (fpo);

    // printf ("ex UTF_wri_file: Filnam =|%s|\n",fnam);

  return 1;

}


//================================================================
  int UTF_wri_f__ (FILE *flun) {
//================================================================
/// \code
/// write memory -> open file; skip deleted lines ('_')
/// rc = 0 = Error;
///      1 = OK.
/// \endcode

 

  int     idel=0;
  long    llen, i1;
  char    *cp1, *cp2;
  size_t  ilen;


  // printf("UTF_wri_f__ \n");


  // 2002-05-22: alle zeilen, beginnend mit "_", skippen.
  for(i1=1; i1<999999; ++i1) {

    // naechste Zeile holen ..
    cp1 = UTF_GetLinNr (mem_cbuf1, &llen, i1);
    if(llen < 0) break;

    // cp2 = UTX_pos_1n (mem_cbuf1); // skip leading blanks

    if(*cp1 == '_') {
      idel = 1;  // must skip also following '&'-lines
      continue;

    } else if (*cp1 == '&') {
      if(idel == 1) continue;
    }
    idel = 0;

      // printf(" %d %d\n",mem_cbuf1[llen],mem_cbuf1[llen - 1]);

    L_ck_c:
      --llen;
      if((mem_cbuf1[llen] == '\n') || (mem_cbuf1[llen] == '\n')) {
        mem_cbuf1[llen] = '\0';
        goto L_ck_c;
      }

    // UTX_CleanCR (mem_cbuf1);   // remove LF ..
    // fprintf(flun, "%s\n",mem_cbuf1);

    fprintf(flun, "%s\n",mem_cbuf1);
  }

    // printf("ex UTF_wri_f__\n");

  return 1;

}


//===============================================================
  int UTF_txt_chg (char *cpos, char *txt, long txlen) {
//===============================================================
/// UTF_txt_chg         1-n chars aendern


  memcpy ((void*)cpos, (void*)txt, txlen);


  return 0;

}

//===============================================================
  int UTF_GetLnrPos (long ipos) {
//===============================================================
/// \code
/// die zeilennummer zu einer offsetpos im MemFile liefern
/// = die Anzahl von \n von 0 bis ipos zaehlen.
/// \endcode


  char *p1, *p2;
  int  ianz;


  // printf("UTF_GetLnrPos %d\n",ipos);

  if(ipos < 0) ipos = UTF_FilBuf0Len;
  if(UTF_FilBuf0Len < 3) return 0;


  ianz = 1;

  p1 = UTF_FilBuf0;
  p2 = &UTF_FilBuf0[ipos];



  //===========================================
  NextLine:
  p1 = strchr(p1, '\n');

  if(!p1) goto Fertig;

  if(p1 < p2) {
    ++p1;
    ++ianz;
    goto NextLine;
  }



  Fertig:
  // printf("ex UTF_GetLnrPos %d %ld\n",ianz,ipos);
  return ianz;

}


//================================================================
   int UTF_GetPos2 (char **p1, char **p2) {
//================================================================
/// UTF_GetPos2         returns Startpos & Endpos of Buf0.


  *p1 = UTF_FilBuf0;
  *p2 = UTF_FilBuf0 + UTF_FilBuf0Len;

  return 0;

}

//================================================================
   int UTF_Buf1_GetPos2 (char **p1, char **p2) {
//================================================================
/// UTF_Buf1_GetPos2    returns Startpos & Endpos of Buf1.


  *p1 = UTF_FilBuf1;
  *p2 = UTF_FilBuf1 + UTF_FilBuf1Len;

  return 0;

}



//===============================================================
  long UTF_GetLen0 () {
//===============================================================
/// \code
/// UTF_GetLen0         liefert die Laenge des UTF_FilBuf0 UTF_FilBuf0Len.
/// = Dateisize abfragen (zB zum Check EOF ..)
/// \endcode

  return UTF_FilBuf0Len;

}



//===============================================================
  char* UTF_GetPosLnr (long *llen, long lNr) {
//===============================================================
/// \code
/// von zeile # lNr die Startposition und die Zeilenlaenge der zeile liefern.
/// ln_pos ist der erste char der line, llen ist die laenge ohne terminator;
/// der Start der naechsten Zeile ist also ln_pos + llen + term_anz.
/// Die erste Zeile ist lNr = 1.
/// Output:
///   llen      gives the position of the LF
///   RetCode   NULL: EOF found.
///
/// Usage:
///  p1 = UTF_GetPosLnr (&lLen, lNr);
///  UTX_dump_cnl (p1, 60); printf("\n"); // print max 60 chars, but stop at \n
/// \endcode


  // static long bufNr=-1;
  static char *ln_start, *ln_end, *ln_pos;
  static long lNr_old = 999999;
  static long lnLen, fSizOld = -1;

  // int  iLast;
  char   *pos_eof;


  // printf("UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU \n");
  // printf("UTF_GetPosLnr lNr=%ld lNr_old=%ld\n", lNr, lNr_old);
  // printf("  UTF_FilBuf0Len=%ld fSizOld=%ld\n",UTF_FilBuf0Len,fSizOld);



  pos_eof = UTF_FilBuf0 + UTF_FilBuf0Len; // die EOF-Position

  // iLast = 0;

  // wenn buffer neu eingelesen wurde, muss von vorn begonnen werden.
  // if(bufNr != UTF_FilBuf0Stat) goto NewStart;
  if(UTF_FilBuf0Len != fSizOld) goto NewStart;

  // lNr_old ist die letze bearb. Zeilennummer, ln_start die Startpos dazu
  if(lNr_old < lNr) goto NextLine;
  if(lNr_old == lNr) goto Fertig;


  
    // ganz an den Dateianfang
  NewStart:
    // printf("   _GetPosLnr NewStart\n");
  // bufNr = UTF_FilBuf0Stat;
  fSizOld  = UTF_FilBuf0Len;
  ln_start = UTF_FilBuf0;
  lNr_old  = 0;
  



  //===========================================
  NextLine:
   
  // eof ? 
  if(ln_start >= pos_eof) {
    ln_pos = NULL;
    lnLen  = 0;
    lNr_old = lNr;
    goto Fertig;
  }

  
  // Die Startposition der Zeile lNr suchen
  ++lNr_old; // die zeilennnummer der zuletzt bearbeiteten Zeile


  ln_end = strchr(ln_start, 10);  // das naechste LF suchen
    // display next 60 chars
    // UTX_dump_cnl (ln_end+1, 60); printf(" _GetPosLnr %ld\n",lNr_old);


  // wenns am oder hinter dem EOF liegt-
  if((ln_end == NULL)||(ln_end > pos_eof)) {
    ln_pos = NULL;
    // last line ?
    if(lNr_old == lNr) {
      ln_end = pos_eof;
      goto L_found;
    }
    // printf("UTF_GetLinNr: Ueberlauf lNr_old=%d lNr=%d\n",lNr_old,lNr);
    lnLen  = 0;
    goto Fertig;
  }



  if(lNr_old < lNr) {
    ++ln_end;
    ln_start = ln_end;
    goto NextLine;
  }


  L_found:
  ln_pos = ln_start;

#ifdef LNTERM_CRLF
  lnLen  = ln_end - ln_start - 1;
#else
  lnLen  = ln_end - ln_start;
#endif

  ln_start = ln_end;
  ++ln_start;



  Fertig:
  *llen = lnLen;

    // TESTBLOCK
    // printf("ex UTF_GetPosLnr llen=%ld, lNr=%ld |",*llen,lNr);
    // if(ln_pos) UTX_dump_cnl (ln_pos, 40); printf("|\n");
    // ENd TESTBLOCK

  return ln_pos;

}


//===========================================================================
  int UTF_GetLineTx (char* lbuf, int *lNr, char* sText) {
//===========================================================================
/// \code
/// Zeile, die mit sText beginnnt, zurueckliefern
/// RetCodes:
///  0  Ok, gefunden
/// -1  nix gefunden
/// \endcode



  long    i1, ilen, llen;


  // printf("UTF_GetLineTx |%s|\n",sText);

  ilen = strlen(sText);


  for(i1=1; i1<9999; ++i1) {

    // naechste Zeile holen ..
    UTF_GetLinNr(lbuf, &llen, i1);
    if(llen < 0) break;

    if(lbuf[0] == '#') continue;

    // printf(" %d |%s|\n",i1,lbuf);

    // find cnam
    if(!strncmp(lbuf, sText, ilen)) {
      *lNr = i1;
      return 0;
    }

  }


  return -1;

}


//================================================================
  char* UTF_GetLinNr (char* lbuf, long *llen, long lNr) {
//================================================================
/// \code
/// get sourceline from sourcelineNr
/// Liefert Zeile mit Zeilennummer lNr in lbuf (aus dem mem !)
/// makes a copy of line <lNr> from UTF_FilBuf0 -> lbuf
/// 
/// Output:
///   lbuf    copy of line <lNr> from UTF_FilBuf0, '\0' terminated. Or NULL.
///   llen    length of lbuf
///   Retcod  NULL: Line does not exist; llen = -1.
///           else Startpos of line in UTF_FilBuf0.
///    
/// see also:
///   UTF_GetPosLnr    get a pointer to a sourcecodeline
///
/// ATTENTION: lbuf must have size=mem_cbuf1_SIZ !
/// \endcode


  long  il;
  char* cpos;


  // printf("UTF_GetLinNr %d\n",lNr);


  if(UTF_FilBuf0Len  < 1) goto Err;


  // Startpos und Laenge der Zeile suchen
  cpos = UTF_GetPosLnr (&il, lNr);
  if(cpos == NULL) goto Err;


  // entspr. Zeile gefunden; kopieren.
  if(il > mem_cbuf1_SIZ) {
    printf("***** UTF_GetLinNr I001\n");
    il = mem_cbuf1_SIZ-2;
  }
  if(lbuf) {
    // copy line
    UTX_CP__ (lbuf, cpos, il);
  }


  *llen = il;
 
  // nur Testausg
  // if(llen>0)TX_Print("ex UTF_GetLinNr: %d llen=%d |%s|",lNr,llen,lbuf);
  


  return cpos;


  Err:
    *llen = -1;
    // printf("ex UTF_GetLinNr ERROR line %d\n",lNr);
    return NULL;

}


//===========================================================================
  int UTF_add_nl () {
//===========================================================================
/// UTF_add_nl          ein '\n' hintanfuegen, wenn nicht vorh.


  if(UTF_FilBuf0[UTF_FilBuf0Len-1] != '\n') {
    strcat(UTF_FilBuf0, "\n");
    UTF_FilBuf0Len = strlen(UTF_FilBuf0);
  }

  return 0;

}


//===========================================================================
  int UTF_add_line (char* txt) {
//===========================================================================
/// \code
/// UTF_add_line        add line to membuf UTF_FilBuf0
/// txt is \0-terminated, '\n' will be added.
/// \endcode

  int  lSiz;
  long newSiz;


  // printf("UTF_add_line |%s|\n",txt);
  // printf(" UTF_FilBuf0Len=%ld\n",UTF_FilBuf0Len);


  lSiz = strlen (txt);
  newSiz = UTF_FilBuf0Len + lSiz;

  if(newSiz >= UTF_FilBuf0Siz) {
    if(UTF_alloc__ (newSiz) < 0) return -1;
  }

  // strcat (UTF_FilBuf0, txt);
  // strcat (UTF_FilBuf0, term_buf);
  // UTF_FilBuf0Len = strlen (UTF_FilBuf0);

  strncpy (&UTF_FilBuf0[UTF_FilBuf0Len], txt, lSiz);
  UTF_FilBuf0[newSiz] = '\n';
  ++newSiz;
  UTF_FilBuf0[newSiz] = '\0';
  // UTF_FilBuf0Len += newSiz;
  UTF_FilBuf0Len = newSiz;        // 2013-10-28

    // printf(" exit: UTF_FilBuf0Len=%ld\n",UTF_FilBuf0Len);

  // AP_mdl_modified_set ();


    // UTF_dump__ ();

  return 0;

}


//===========================================================================
  int UTF_clear_ () {
//===========================================================================
/// UTF_clear          den Hauptspeicher loeschen


  UTF_FilBuf0[0] = '\0';
  UTF_FilBuf0Len = 0;

  // AP_mdl_modified_set();

  return 0;

}



//===========================================================================
  int UTF_clear1 () {
//===========================================================================
/// \code
/// UTF_clear1          den Zwischenspeicher Buf1 loeschen
/// Example see UTF_insert1
/// \endcode


  if(UTF_FilBuf1) {
    // free(UTF_FilBuf1);
    // UTF_FilBuf1 = NULL;
    // UTF_FilBuf1Siz = 0;

    UTF_FilBuf1[0] = '\0';
    UTF_FilBuf1Len = 0;
  }


  // UTF_FilBuf1[0] = '\0';

  return 0;

}


//=====================================================================
  int UTF_add_fil__ (char *cBuf, long *sizAct, long sizTot, char* fn) {
//=====================================================================
/// \code
/// Datei fn an membuf cBuf hintanhaengen; size of cBuf is sizTot.
/// cBuf muss gross genug sein !!
/// RetCod:
///   -1           OpenError
///    0           OK
/// \endcode


  long  cLen, sizRest, lLn;
  char  *cpos;
  FILE  *flun;


  // printf ("UTF_add_fil__ |%s| siz=%d\n",fn,sizTot);


  // Das Inputfile oeffnen
  if ((flun = fopen (fn, "r")) == NULL) {
    TX_Error ("UTF_add_fil__: FileOpenError  %s ****",fn);
    return -1;
  }

  cLen = *sizAct; // strlen(cBuf);
  cpos = &cBuf[cLen];
  sizRest = sizTot - cLen;

  // printf(" start:|%s| len=%d siz=%d\n",cpos,cLen,sizTot);


  //---------------------------------------------
  // Datei zeilenweise einlesen
  while (fgets (cpos, sizRest, flun) != NULL) {

    // printf(" add |%s|\n",cpos);

    // change fileformat dos --> unix
    // das Zeileende muss so aussehen:   \n  \0;
    // das \0 wird dann wieder ueberschrieben !!!
    lLn = UTX_CleanLF (cpos);

    cpos = &cpos[lLn];
    sizRest -= lLn;

    if(sizRest < 72) {
      TX_Error("UTF_add_fil__ E001");
      break;
    }
  }


  //---------------------------------------------
  /* Fertig: */
  fclose (flun);

  *sizAct = sizTot - sizRest;
  cBuf[*sizAct] = '\0';

  return 0;

}


//================================================================
  int UTF_add_file (char* fn) {
//================================================================
/// \code
/// Datei in den Hauptbuffer laden - hintanhaengen
/// rc = -1 = Error;
///       1 = OK.
/// \endcode

  long  fSiz;
  // long  newSiz;

  // printf ("UTF_add_file |%s|\n",fn);


  // get Filesize
  fSiz = OS_FilSiz (fn);
  if(fSiz < 0) {
    TX_Error("UTF_add_file - Error file |%s|",fn);
    return -1;
  }


  // request mem
  if(UTF_alloc__ (fSiz + UTF_FilBuf0Len) < 0) {
    TX_Error("UTF_add_file - EOM");
    return -1;
  }


  UTF_add_fil__ (UTF_FilBuf0, &UTF_FilBuf0Len, UTF_FilBuf0Siz, fn);

  // UTF_FilBuf0Len = strlen(UTF_FilBuf0);

    // printf("ex UTF_add_file |%s| siz=%d\n",UTF_FilBuf0,UTF_FilBuf0Siz);
    // printf("ex UTF_add_file Len=%d Siz=%d\n",UTF_FilBuf0Len,UTF_FilBuf0Siz);

  // new file has been loaded; its yet unmodified ..
  // AP_mdl_modified_reset ();
  // but modelbox = invalid
  AP_mdlbox_invalid_set ();

  // UTF_dump__ (); // TEST

  return 1;

}



//================================================================
  char* UTF_add1_file (char* fn) {
//================================================================
/// \code
/// Eine Datei into Buf1 laden
/// rc = -1 = Error;
///       1 = OK.
/// \endcode

  long     fSiz;
  // FILE  *flun;
  // char  *cbuf;


  // printf ("UTF_add1_file: Filnam = /%s/\n",fn);

  // get Filesize
  fSiz = OS_FilSiz (fn);

  // request mem
  UTF_alloc1_ (fSiz + UTF_FilBuf1Len);

  UTF_add_fil__ (UTF_FilBuf1, &UTF_FilBuf1Len, UTF_FilBuf1Siz, fn);

  // UTF_FilBuf1Len = strlen(UTF_FilBuf1);

  return UTF_FilBuf1;
  
  
}


//===========================================================================
  long UTF_offset_    (char* cpos) {
//===========================================================================
// UTF_offset_          get offset of text in UTF_FilBuf0
//   use offset eg for select text in editor with GUI_edi_sel__

  long off;

  off = cpos - UTF_FilBuf0;

  // printf("ex UTF_offset_ %d\n",off);

  return off;

}


//================================================================
  int UTF_add1_strLF (char *cBuf) {
//================================================================
/// \code
/// UTF_add1_strLF       add a line, which is terminated with LF, to Buffer1.
/// see UTF_add1_line
/// \endcode


  long lLen, newLen;
  char *lfPos;


  if(cBuf == NULL) return -1;

  lfPos = strchr(cBuf, '\n');
  if(lfPos == NULL) {
    lLen = strlen(cBuf);
  } else {
    lLen = lfPos - cBuf;
  }

  newLen = UTF_FilBuf1Len + lLen;


  // printf("UTF_add1_line newLen=%d actLen=%ld\n",newLen,UTF_FilBuf1Len);
  // printf("UTF_add1_strLF |%s| l=%d Len=%ld\n",cBuf,strlen(cBuf),UTF_FilBuf1Len);
  // printf(" %d %d\n",strlen(term_buf),term_anz);


  // die max. Laenge von UTF_FilBuf1 ist UTF_FilBuf1Siz

  if(newLen >= UTF_FilBuf1Siz) {
    if(UTF_alloc1_ (newLen) < 0) return -1;
  }


  strcpy(&UTF_FilBuf1[UTF_FilBuf1Len], cBuf);
  UTF_FilBuf1Len = newLen;

  strcpy(&UTF_FilBuf1[UTF_FilBuf1Len], term_buf);
  UTF_FilBuf1Len += term_anz;

  // printf("ex UTF_add1_strLF l=%d\n",UTF_FilBuf1Len);

  return 0;

}


//===========================================================================
  int UTF_add1_last_add (char* txt) {
//===========================================================================
// add txt to last line in UTF_FilBuf1

  long newLen;



  --UTF_FilBuf1Len;   // remove the newLine-char

  newLen = UTF_FilBuf1Len + strlen(txt);

  strcpy(&UTF_FilBuf1[UTF_FilBuf1Len], txt);

  UTF_FilBuf1Len = newLen;


  // strcpy(&UTF_FilBuf1[UTF_FilBuf1Len], term_buf);
  // UTF_FilBuf1Len += term_anz;
  UTF_FilBuf1[UTF_FilBuf1Len] = '\n';
  ++UTF_FilBuf1Len;


  // printf("ex UTF_add1_line l=%d\n",UTF_FilBuf1Len);

  return 0;

}


//===========================================================================
  int UTF_add1_line (char* txt) {
//===========================================================================
/// \code
/// UTF_add1_line        add string (0-termin.) into UTF_FilBuf1
/// Schreibt --> UTF_FilBuf1  (hintanfuegen)
/// retCod    0    OK
///          -1    out of mem
/// Example see UTF_insert1
/// \endcode

  long newLen;


  newLen = UTF_FilBuf1Len + strlen(txt);


  // printf("UTF_add1_line newLen=%ld actLen=%ld\n",newLen,UTF_FilBuf1Len);
  // printf("UTF_add1_line |%s| l=%d Len=%ld\n",txt,strlen(txt),UTF_FilBuf1Len);
  // printf(" %d %d\n",strlen(term_buf),term_anz);


  // die max. Laenge von UTF_FilBuf1 ist UTF_FilBuf1Siz

  if(newLen >= UTF_FilBuf1Siz) {
    if(UTF_alloc1_ (newLen) < 0) return -1;
  }

  // alte, SEHR langsame Variante.
  // strcat(UTF_FilBuf1, txt);
  // strcat(UTF_FilBuf1, term_buf);
  // UTF_FilBuf1Len = strlen(UTF_FilBuf1);



  strcpy(&UTF_FilBuf1[UTF_FilBuf1Len], txt);
  UTF_FilBuf1Len = newLen;


  // strcpy(&UTF_FilBuf1[UTF_FilBuf1Len], term_buf);
  // UTF_FilBuf1Len += term_anz;
  UTF_FilBuf1[UTF_FilBuf1Len] = '\n';
  ++UTF_FilBuf1Len;


    // printf("ex UTF_add1_line l=%ld\n",UTF_FilBuf1Len);

  return 0;

}


/*
//===========================================================================
  int UTF_add2_line (char* txt) {
//===========================================================================
// UTF_add1_line        eine Zeile zwischenspeichern
//

  int  cNr, lNr;
  char *p1, *p2, *pEnd;

  // printf("UTF_add2_line |%s| l=%d\n",txt,strlen(txt));




  if(strlen(txt) < 80) {
    UTF_add1_line  (txt);    // Zeile raus
    return 0;
  }



  // Zeilen laenger als 80 Char; zerteilen
  pEnd = txt + strlen(txt);
  p1 = txt;
  lNr = 0;



  L_next:
  ++lNr;
  p2 = p1 + 76;         // soll-LineLen = 76
  if(p2 >= pEnd) goto L_copy_line;



  while((*p2 != ',')&&(*p2 != ' ')) {
    --p2;
    if(p2 < p1) {
      TX_Error("UTF_add2_line E001");
      return -1;
    }
  }
  ++p2; // mit ","




  L_copy_line:
  cNr = p2 - p1;
  if(lNr > 1) {
    strcpy(mem_cbuf1, "& ");
    strncat(mem_cbuf1, p1, cNr);
    mem_cbuf1[cNr+2] = '\0';

  } else {
    strncpy(mem_cbuf1, p1, cNr);
    mem_cbuf1[cNr] = '\0';
  }

  // printf("   |%s|\n",mem_cbuf1);
  if(p2 < pEnd) {
    strcat(mem_cbuf1, "\\");
  }
  UTF_add1_line  (mem_cbuf1);    // Zeile raus

  p1 = p2;
  if(p1 < pEnd) goto L_next;


  return 0;

}
*/


//================================================================
  int UTF_insertTxt (char *insPos, long cNr, char *auxBuf) {
//================================================================
/// \code
/// UTF_insertTxt        insert text
/// insert cNr chars of auxBuf into cBuf at pos insPos
/// \endcode

  long  l1, newSiz, ii;


  // printf("UTF_insertTxt %ld |",cNr);UTX_dump_c__(auxBuf,cNr);printf("\n");
  // printf(" insPos1 |%s|\n",insPos);



  // get enough mem ..
  newSiz = UTF_FilBuf0Len + cNr;
  if(newSiz > UTF_FilBuf0Siz) {
    if(UTF_alloc__ (newSiz) < 0) return -1; 
  }

  ii = insPos - UTF_FilBuf0;   // cPos as offset
  l1 = UTF_FilBuf0Len - ii;    // muss verschoben werden
    // printf("mv %ld von %ld nach %ld\n",l1,ii,ii+cNr);
  // unbedingt memmove, sonst wird der hintere Teile zerstoert !
  memmove(&UTF_FilBuf0[ii+cNr], &UTF_FilBuf0[ii], l1);
    // printf("cp %ld nach %ld\n",cNr,ii);
  memcpy(&UTF_FilBuf0[ii], auxBuf, cNr);  // insert


  UTF_FilBuf0Len += cNr;
  UTF_FilBuf0[UTF_FilBuf0Len] = '\0';

  // AP_mdl_modified_set ();

    // printf(" insPos2 |%s|\n",insPos);
    // printf(" UTF_FilBuf0|%s|\n",UTF_FilBuf0);


  return 0;

}


//===========================================================================
  int UTF_insert1 (long cpos) {
//===========================================================================
/// \code
/// UTF_insert1    insert auxBuffer into mainBuffer at pos cpos.
/// auxBuffer: UTF_FilBuf1; mainBuffer: UTF_FilBuf0.
/// cpos < 0: join buffers (mainBuffer += auxBuffer).
/// update display after buffer-update: see APED_update__ ();
/// Example:
///   ObjGX o1; char s1[1000]; int siz_s1=1000; Point p1={1.,0.,0.};
///   UTF_clear1 ();                       // init auxBuf
///   AP_obj_2_txt (NULL, 0L, NULL, 0L);   // init AP_obj_2_txt
///   OGX_SET_OBJ (&o1, Typ_PT, Typ_PT, 1, &p1); // create point-object
///   AP_obj_2_txt (s1, siz_s1, &o1, -1L); // create sourcecode for obj -> auxBuf
///   UTF_add1_line ("# end of xx");       // add text to auxBuf
///   UTF_insert1 (-1L);       // store auxBuf (add to active code)
///   APED_update__ (-1L);     // work new codes
/// \endcode

// UTF_insert1         Zwischenspeicher im Memory an Pos cpos einfuegen
// insert UTF_FilBuf1 into UTF_FilBuf0 nach char # cpos (der erste = 1)
// cpos < 0: hinten anfuegen

  int  irc;
  long l1, newSiz;


  // printf("UTF_insert1 cp=%ld Buf0Len=%ld Buf1Len=%ld siz=%ld\n",cpos,
         // UTF_FilBuf0Len,UTF_FilBuf1Len,UTF_FilBuf0Siz);
  // printf(" Buf0=|%s|\n",UTF_FilBuf0);
  // printf(" Buf1=|%s|\n",UTF_FilBuf1);



  irc = 0;
  if(UTF_FilBuf1Len < 1) return 0;


  newSiz = UTF_FilBuf0Len+UTF_FilBuf1Len;

  if(newSiz > UTF_FilBuf0Siz) {
    if(UTF_alloc__ (newSiz) < 0) return -1;
  }



  if((cpos < 1)||(cpos >= UTF_FilBuf0Len)) {

    strcat(UTF_FilBuf0, UTF_FilBuf1);


  } else {


    l1 = UTF_FilBuf0Len - cpos;  // muss verschoben werden
    // printf("mv %d von %d nach %d\n",l1,cpos+UTF_FilBuf1Len,cpos);
    // unbedingt memmove, sonst wird der hintere Teile zerstoert !
    memmove(&UTF_FilBuf0[cpos+UTF_FilBuf1Len], &UTF_FilBuf0[cpos], l1);
    // printf("mv %d von Buf1 nach %d\n",UTF_FilBuf1Len,cpos);
    memcpy(&UTF_FilBuf0[cpos], UTF_FilBuf1, UTF_FilBuf1Len);  // insert

  }


  UTF_FilBuf0Len += UTF_FilBuf1Len;
  UTF_FilBuf0[UTF_FilBuf0Len] = '\0';

  // AP_mdl_modified_set ();

  // clear Buf1
  UTF_clear1 ();



  // printf("------------------buf0-Anf\n");
  // printf("%s",UTF_FilBuf0);
  // printf("------------------buf0-End\n");

  // printf("ex UTF_insert1 Buf0Len=%ld siz=%ld\n",
         // UTF_FilBuf0Len,UTF_FilBuf0Siz);


  return 0;

}


//================================================================
  int UTF_del_text (char *cFrom, char *cTo) {
//================================================================
/// UTF_del_text        delete from cPos to cPos

  long    lPos, txLen;


  lPos = UTF_offset_ (cFrom);

  txLen = cTo - cFrom;
    // printf("UTF_del_text lPos=%ld txLen=%ld\n",lPos,txLen);

  // ausschneiden
  UTX_cut1 (UTF_FilBuf0, &UTF_FilBuf0Len, lPos, txLen);

  // printf("ex UTF_del_line1 Buf0Len=%ld\n",UTF_FilBuf0Len);

  return 0;

}


//===========================================================================
  int UTF_del_line1 (long lNr) {
//===========================================================================
/// Eine Zeile im Memory loeschen

  char    *cPos;
  long    lPos, txLen;


  // printf("UTF_del_line1 lNr=%d Buf0Len=%ld\n",lNr,UTF_FilBuf0Len);
  
  cPos = UTF_GetPosLnr (&txLen, lNr);

  txLen += term_anz;

  lPos = UTF_offset_ (cPos);

  // printf("   lPos=%ld txLen=%ld\n",lPos,txLen);

  // ausschneiden
  UTX_cut1 (UTF_FilBuf0, &UTF_FilBuf0Len, lPos, txLen);

  // printf("ex UTF_del_line1 Buf0Len=%ld\n",UTF_FilBuf0Len);

  return 0;


}


//===========================================================================
  char* UTF_GetnextLnPos (char *cpos) {
//===========================================================================
/// \code
/// UTF_GetnextLnPos    Position der nachfolgenden zeile liefern
/// cpos = NULL: Bufferstartpos UTF_FilBuf0 liefern.
/// \endcode

  char *nxtLn;

  // printf("UTF_GetnextLnPos |%s|\n",cpos);


  if(cpos == NULL) return UTF_FilBuf0;



  nxtLn = strchr(cpos, '\n');
  // if(cpos == NULL) return NULL;
  if(nxtLn == NULL) return NULL;

  ++nxtLn;

  if(nxtLn >= (UTF_FilBuf0 + UTF_FilBuf0Len)) return NULL;

  // printf("ex UTF_GetnextLnPos |%s|\n",nxtLn);

  return nxtLn;

}


//================================================================
  char* UTF_GetPrevLnPos (long *lNr, char *cpos) {
//================================================================
/// \code
/// UTF_GetPrevLnPos    Position der vorherigen zeile liefern
/// Input:
///   lNr        NULL: do not provide lineNr of previous Line.
///   cpos       NULL: BufferEndPos UTF_FilBuf0+len liefern.
/// Output:
///   lNr = -1   Start of Buffer reached
/// \endcode

  char *prvLn;
  long iOff;

  // printf("UTF_GetnextLnPos |%s|\n",cpos);


  if(cpos == NULL) return UTF_FilBuf0 + UTF_FilBuf0Len;


  prvLn = cpos - 1;



  // skip all empty lines ..
  L_1:
  if(*prvLn == '\n') { --prvLn; goto L_1; }


  // go back until '\n'
  L_2:
  --prvLn;
  if(prvLn <= UTF_FilBuf0) {
    if(lNr) *lNr = -1;
    return UTF_FilBuf0;
  }
  if(*prvLn != '\n') goto L_2;

  ++prvLn;


  iOff = prvLn - UTF_FilBuf0;

  if(lNr)   *lNr = UTF_GetLnrPos (iOff);


    // printf("ex UTF_GetPrevLnPos  %d |",*lNr);
    // UTX_dump_cnl(prvLn,30);printf("|\n");


  return prvLn;

}


//===========================================================================
  char* UTF_find_tx1 (long *lNr, char *tx1) {
//===========================================================================
// UTF_find_tx1          find lineNumber of first occurance of text
//   search in UTF_FilBuf0 (memSpace source) 
// Input:
//   lNr        lineNumber where to start
// Output:
//   lNr        lineNumber of first occurance of text <tx1>; -1 = not-found
//   RetCod     position of tx1
//              NULL - tx1 not found

/*
Soll tx1 am Zeilenbeginn stehen:
  strcpy(auxBuf, term_buf);
  strcat(auxBuf, suchText);
  UTF_find_tx1 (&i1, auxBuf);
*/


  
  long    ipos, llen, lst;
  char    *cpos, *pst;


  // printf("UTF_find_tx1|%s| %ld\n",tx1,*lNr);

  if(UTF_FilBuf0 == NULL) return NULL;

  lst = *lNr;



  // set pst = startpos
  L_nxt:
    // printf(" find_tx1-lst=%ld\n",lst);
  pst = UTF_GetPosLnr (&llen, lst);
  if(pst) {
    // cpos = strstr(UTF_FilBuf0, tx1);
    cpos = strstr(pst, tx1);
    if(cpos) goto L_found;
  }

  if(lst == 0L) {
    *lNr = -1;
    return NULL;
  }
  lst = 0L;
  goto L_nxt;


  //----------------------------------------------------------------
  L_found:
  // bei Suche nach Neuzeile ist \n first char
  if(*cpos == '\n') ++cpos;


  // zeilennummer holen
  ipos = cpos - UTF_FilBuf0;
  *lNr = UTF_GetLnrPos (ipos);

    // printf("ex UTF_find_tx1 lNr=%ld ipos=%ld |%s|\n",*lNr,ipos,cpos);

  return cpos;

}


//===========================================================================
  int UTF_del_start (char *text) {
//===========================================================================
/// UTF_del_start        bis "text" Memory loeschen

  long isiz;


  // printf("UTF_del_start |%s|\n",text);

  isiz = text - UTF_FilBuf0;

  if(isiz > 0) {
    UTF_FilBuf0Len -= isiz;
    memmove (UTF_FilBuf0, text, UTF_FilBuf0Len+1);
  }

  // printf("new siz %d %d\n",UTF_FilBuf0Len,isiz);

  return 0;

}


/* UNUSED
//===========================================================================
  int UTF_del_rest (char *text) {
//===========================================================================
/// UTF_del_rest        ab "text" Memory loeschen

  char *cpos;


  // printf("UTF_del_rest |%s|\n",text);

  if(UTF_FilBuf0 == NULL) return -1;


  cpos = strstr(UTF_FilBuf0, text);
  if(cpos == NULL) return -1;

  // printf("found %ld\n",UTF_FilBuf0Len);
  *cpos = '\0';

  UTF_FilBuf0Len = cpos - UTF_FilBuf0;
  // printf("now %ld\n",UTF_FilBuf0Len);

  return 0;

}
*/

/*=================== EOF ============================*/
