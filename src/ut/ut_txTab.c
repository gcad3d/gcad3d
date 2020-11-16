//**************************************************************************
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
2009-06-13 StringList must be a TxtTab; was *void. RF.
           UtxTab_clear -> UtxTab_init__
2009-03-10 UtxTab_nr  new. RF.
2009-03-10 UtxTab_getNr deleted (replace by UtxTab__). RF.

-----------------------------------------------------
*/
#ifdef globTag
void UtxTab(){}
#endif
/*!
\file  ../ut/ut_txTab.c
\brief list of strings (0-terminated); add, find, ..
\code
NEEDS:
../ut/ut_umem.h       ../ut/ut_umem.c                     UME_..
=====================================================
List_functions_start:

UtxTab_NEW         setup of a new StringList
UtxTab_init__      init / clear all words; automatic alloc/realloc
UtxTab_init_Memspc init / clear all words; fixed space in Memspc; no reallocate
UtxTab_init_spc    init / clear all words; fixed space (void*); no reallocate
UtxTab_add         add a new string
UtxTab_add_uniq__  add string, check if it already exists
UtxTab_add_file    make table of strings from file
UtxTab_rem         remove last string
UtxTab_query       get text of last string; retCod = index of next free
UtxTab_find        find string - ask nr
UtxTab_free

UtxTab__           get text of record Nr iNr;
UtxTab_nr          returns nr of strings
UtxTab_bytes_free  returns nr of free bytes in TxtTab
UtxTab_pos_free    returns next free position in memspace of TxtTab

UtxTab_reall_C
UtxTab_reall_I
UtxTab_dump

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


Class for stacking Textstrings: add, find ..; reallocate automatic
All strings are terminated with '\0'.



Usage example - automatic reallocate (malloc):
  #include "../ut/ut_txTab.h"              // TxtTab
  UtxTab_NEW (txTab1);                // stringtable
  char *p1;

  UtxTab_init__ (&txTab1);            // init (malloc ..)
  UtxTab_add (&txTab1, "Word1");      // add a new word
  i1 = UtxTab_nr (&txTab1);           // returns nr of strings
  p1 = UtxTab__ (i1-1, &txTab1);      // get word from index
  UtxTab_query (&p1, &txTab1);        // get last word
  UtxTab_rem (&txTab1);               // remove last word
  UtxTab_init__ (&txTab1);            // reset
  UtxTab_free (&txTab1);              // free mem


Usage example - fixed space in charArray:
  #include "../ut/ut_txTab.h"              // TxtTab
  UtxTab_NEW (txTab1);                // stringtable
  char       myMem[500];
  UtxTab_init_spc (&txTab1, myMem, sizeof(myMem));
  ..


Usage example - fixed space in Memspc:
  #include "../ut/ut_txTab.h"              // TxtTab
  #include "../ut/ut_umem.h"               // Memspc, UME_init
  char       myMem[500];
  Memspc     mySpc;
  UtxTab_NEW (txTab1);

  UME_init (&mySpc, myMem, sizeof(myMem));
  UtxTab_init_Memspc (&txTab1, &mySpc);
  ..


Loop tru StringList:
  for(i1=0; i1 < UtxTab_nr(&txTab1); ++i1) 
    printf(" |%s|\n",UtxTab__(i1,&txTab1));


\endcode *//*


Other TextstringLibs:
ustr             libustr
vstr
bstring



=====================================================

Offline testen:
cd ../ut
cc -ggdb -pg ut_txTab.c ut_umem.c -DOFFLINE&&./a.out
cl ut_txTab.c -DOFFLINE&&ut_txTab.exe
c
==============================================================
*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ut/ut_umem.h"            // Memspc, UME_init
#include "../ut/ut_txTab.h"           // UtxTab




//========================================================
//========================================================
#ifdef OFFLINE





#include <stdarg.h>

void TX_Print (char* txt, ...);
void TX_Error (char* txt, ...);


//============================
  int main (int paranz, char *params[]) {
//============================

  int        i1, i2, irc;
  char       *p1, cBuf[64], myMem[500];
  Memspc     mySpc;
  UtxTab_NEW (txTab1);         // init textTable


/*
  //==== test fixed-size; no reallocate =================================
  UME_init (&mySpc, myMem, sizeof(myMem));
  // fill mySpc with StringList
  UtxTab_init_Memspc (&txTab1, &mySpc);
*/

   UtxTab_init_spc (&txTab1, myMem, sizeof(myMem));


  //==== test automatic reallocate =================================
  // init (malloc ..)
  // UtxTab_init__ (&txTab1);

  // add a new word
  UtxTab_add (&txTab1, "Word1");

  // ask nr of defined words
  i1 = UtxTab_query (&p1, &txTab1);

  // add a new word
  for(i1=123; i1<333; ++i1) {
    sprintf(cBuf, "das ist String Nr %d",i1);
    irc = UtxTab_add (&txTab1, cBuf);
    if(irc < 0) break;
  }


  UtxTab_add (&txTab1, "letzte Zeile");
  i2 = UtxTab_add (&txTab1, "Word4");
    printf(" i2=%d\n",i2);
  UtxTab_add (&txTab1, "Word5");

  printf(" w4 %d |%s|\n",i2,UtxTab__(i2, &txTab1));

  // UtxTab_dump (&txTab1);
  i1 = UtxTab_query (&p1, &txTab1);        // get last word
  UtxTab_rem (&txTab1);                    // remove last word
  i1 = UtxTab_query (&p1, &txTab1);        // get last word
    printf(" %d |%s|\n",i1,p1);
  UtxTab_init__ (&txTab1);

  i1 = UtxTab_query (&p1, &txTab1);        // get last word

  UtxTab_free (&txTab1);


  return 0;
}

 int UTI_round_i2b (int ii) { 
  int   ib = -1;
  if(ii < 2) return 0;
  ii *= 2;
  --ii;
  while(ii) {ii /= 2; ++ib;}
  return ib;
}

  int UTI_round_b2i (int i1) {
  int ii=1;
  while(i1) {--i1; ii *=2;}
  return ii;

}

//============================
  void TX_Print (char* txt, ...) {
//============================
/// \code
/// formatted write into messagewindow.
/// Example:
/// TX_Print("Value d1 = %f string =|%s|",d1,cBuf);
/// \endcode


  va_list va;
  char    TX_buf1[1024];



  va_start(va,txt);
  vsprintf(TX_buf1,txt,va);
  va_end(va);

  if(strlen(TX_buf1) > 255) {
    TX_buf1[256] ='\0';
  }

  // TX_Write (TX_buf1);
  printf("%s\n",TX_buf1);

}

//============================
  void TX_Error (char* txt, ...) { TX_Print (txt); }
//============================

#endif
//========================================================
//========================================================





//================================================================
  int UtxTab_free (TxtTab *tab) {
//================================================================
/// do not free TxtTab from UtxTab_init_Memspc

  // printf("UtxTab_free %d\n",tab->stat);

  if(tab->stat == 1) {
    if(tab->tab) {
      if(tab->tab)free (tab->tab);
      if(tab->ind)free (tab->ind);
      tab->tab = NULL;
      tab->ind = NULL;
    }
  }

  return 0;

}


//================================================================
  int UtxTab_reall_C (char **tab, int newSiz) {
//================================================================


  // printf("::::::::::::: UtxTab_reall_C %d\n",newSiz);


  *tab = realloc(*tab, newSiz);

  if(*tab == NULL) {
    printf("***** UtxTab_reall_C EOM newSiz %d\n",newSiz);
    TX_Error ("******** out of memory - UtxTab_reall_C *********");
    return -1;
  }

  return 0;

}


//================================================================
  int UtxTab_reall_I (long **ind, int newSiz) {
//================================================================


  // printf("::::::::::::: UtxTab_reall_I %d\n",newSiz);


  *ind = realloc(*ind, newSiz * sizeof(long));

  if(*ind == NULL) {
    TX_Error ("******** out of memory - UtxTab_reall_I *********");
    return -1;
  }

  return 0;

}


//=========================================================================
  int UtxTab_add_file (TxtTab *sTab, char *fnam) {
//=========================================================================
/// make table of strings from file.

  int     i1;
  char    s1[256];
  FILE    *fpi;


  // printf("UtxTab_add_file |%s|\n",fnam);


  if ((fpi = fopen (fnam, "r")) == NULL) {
    printf ("***** Error UtxTab_add_file E1|%s|\n",fnam);
    return -1;
  }

  while (!feof (fpi)) {
    if (fgets (s1, 250, fpi) == NULL) break;
    // remove lf
    i1 = strlen(s1) - 1;
    if(s1[i1] == '\n') {s1[i1] = '\0'; --i1;}
    if(s1[i1] == '\r') {s1[i1] = '\0';}
    UtxTab_add (sTab, s1);
  }

  fclose(fpi);

    // UtxTab_dump (sTab);

  return sTab->iNr;

}


//================================================================
  int UtxTab_add_uniq__  (TxtTab *tab, char *newtxt) {
//================================================================
/// \code
/// add string, check if it already exists
/// RetCode:
///  -1     EOM - no enough memory error
///  >=0    index of string
/// \endcode


  int  i1, i2;

  // check if string <newtxt> already exists;

  for(i1=0; i1<tab->iNr; ++i1) {
    i2 = tab->ind[i1];
      // printf(" tab[%d] Pos=%d |%s|\n",i1,i2,&tab->tab[i2]);
    if(!strcmp(newtxt, &tab->tab[i2])) return i1;     // alread exists 
  }

  
  // new string ..
  return UtxTab_add (tab, newtxt);

}


//================================================================
  int UtxTab_add (TxtTab *tab, char *newtxt) {
//================================================================
/// \code
/// add a new string
/// RetCode:
///  -1     EOM - no enough memory error
///  >=0    index of string
/// \endcode

  int  i1, iAct, iNr, iLen, iPos, iEnd;


  // printf("UtxTab_add  |%s|\n",newtxt);
  // printf(" stat=%d\n",tab->stat);
  // UtxTab_dump (tab, "UtxTab_add-in");


  if(!tab) goto L_err_ttb;
  if(!tab->tab) goto L_err_tab;
  if(!tab->ind) goto L_err_ind;
  if(!newtxt) goto L_err_inp;


  // check space in textBuffer tab->tab
  iLen = strlen(newtxt);
  iNr  = tab->iNr;           // index of last stored word;
  iPos = tab->ind[iNr];      // startPos of next word
  iEnd = iPos + iLen;
    // printf("   iLen=%d iPos=%d iEnd=%d iNr=%d\n",iLen,iPos,iEnd,iNr);
  if((iEnd + 1) >= tab->tabSiz) {
    if(tab->stat == 0) return -1;
    i1 = tab->tabSiz + TAB_C_SIZ;
    if(UtxTab_reall_C (&(tab->tab), i1) < 0) goto L_err_tab;
    tab->tabSiz = i1;
  }


  // add newtxt to textbuffer
  strncpy(&(tab->tab[iPos]), newtxt, iLen);
  // iPos += iLen;
  tab->tab[iEnd] = '\0';


  // check space in indexBuffer tab->ind
  iAct = iNr;   // active index
  ++iNr;
  if(iNr >= tab->indSiz) {
    if(tab->stat == 0) goto L_err_ind;
    i1 = tab->indSiz + TAB_I_SIZ;
    if(UtxTab_reall_I (&(tab->ind), i1) < 0) goto L_err_ind;
    tab->indSiz = i1;
  }

  tab->ind[iNr] = iEnd + 1;  // set startPos for next string
  tab->iNr = iNr;


    // TESTBLOCK
    // UtxTab_dump (tab, "ex-UtxTab_add");
    // END TESTBLOCK


  return iAct;

  L_err_ttb:
    TX_Error ("******** UtxTab_add - E000-ttb ***");
    return -1;

  L_err_tab:
    TX_Error ("******** UtxTab_add - E001-tab ***");
    return -1;

  L_err_ind:
    TX_Error ("******** UtxTab_add - E002-ind ***");
    return -1;

  L_err_inp:
    TX_Error ("******** UtxTab_add - E003-inp ***");
    return -2;

}


//================================================================
  int UtxTab_rem (TxtTab *tab) {
//================================================================
/// remove last string

  if(tab->iNr > 0) tab->iNr -= 1;

  return 0;

}


//================================================================
  int UtxTab_init_spc (TxtTab *tab, void *spc, int isize) {
//================================================================
/// \code
/// init / clear all words; fixed space (void*); no reallocate.
/// Input:
///   isize    nr of chars
/// Output:
///   RetCod   -
/// \endcode


  int  i1, chrNr, indNr;

  // printf("UtxTab_init_spc %d\n",isize);


  // compute size of a record; 32 char for the string, 4 for the index = 36
  if(tab->tab == NULL) {

    tab->stat   = 0;     // no reallocate

    // get 66% for chars
    i1 = isize / 12;
    chrNr = i1 * 8;
    tab->tabSiz = chrNr;

    indNr = (isize - chrNr) / sizeof(long); 
    tab->indSiz = indNr;

    tab->tab = spc;
    tab->ind = (void*)(((char*)spc) + (indNr * sizeof(long)));
  }


  tab->iNr    = 0;
  tab->ind[0] = 0;
  tab->tab[0] = '\0';  // erstes Wort terminieren

    // UtxTab_dump (tab, "ex-UtxTab_init_spc");

  return 0;

}



//================================================================
  int UtxTab_init_Memspc (TxtTab *tab, Memspc *memSpc) {
//================================================================
/// \code
/// init / clear all words; fixed space in Memspc; no reallocate.
/// RetCod:
///   -1      out of memory error
/// \endcode


  int  i1, i2, i3;
 
  // printf("UtxTab_init_Memspc \n");


  // compute size of a record; 32 char for the string, 4 for the index = 36
  if(tab->tab == NULL) {

    tab->stat   = 0;     // no reallocate

    i1 = UME_ck_free (memSpc) / 36;
    i2 = i1 * 32;  // size tab (chars)
    i3 = i1 * 4;   // size ind (int)
      // printf(" i1=%d i2=%d i3=%d\n",i1,i2,i3);

    // get spc for tab
    tab->tab    = UME_reserve (memSpc, i2);
    tab->tabSiz = i2;

    // get spc for ind
    tab->ind    = UME_reserve (memSpc, i3);
    tab->indSiz = i1;

  }

  tab->iNr    = 0;
  tab->ind[0] = 0;
  tab->tab[0] = '\0';  // erstes Wort terminieren

  return 0;

}


//================================================================
  int UtxTab_init__ (TxtTab *tab) {
//================================================================
/// \code
/// init / clear all words; automatic alloc/realloc.
/// \endcode

  // printf("UtxTab_init__ \n");


  if(tab->tab == NULL) {
    tab->stat   = 1;     // 1=automatic reallocting
      // printf("UtxTab_init__ init \n");
    if(UtxTab_reall_C (&(tab->tab), TAB_C_SIZ) < 0) return -1;
    tab->tabSiz = TAB_C_SIZ;

    // get spc for ind
    tab->ind = NULL;
    if(UtxTab_reall_I (&(tab->ind), TAB_I_SIZ) < 0) return -1;
    tab->indSiz = TAB_I_SIZ;
  }


  tab->iNr    = 0;
  tab->ind[0] = 0;
  tab->tab[0] = '\0';  // erstes Wort terminieren

  return 0;

}


//================================================================
  int UtxTab_find (char *cs, TxtTab *tab) {
//================================================================
/// UtxTab_find       find string cs - returns nr; -1 = not found

  int  i1, iPos;

  // printf("UtxTab_find |%s|\n",cs);

  for(i1=0; i1<tab->iNr; ++i1) {
    iPos = tab->ind[i1];
    if(!(strcmp(&(tab->tab[iPos]), cs))) return i1;
  }

  return -1;

}


//================================================================
  int UtxTab_query (char **cLast, TxtTab *tab) {
//================================================================
/// \code
/// get text of last string;
/// RetCod = index of next free
/// \endcode

  int  iPos;

  // printf("UtxTab_query %d\n",tab->iNr);
  // printf(" stat=%d\n",tab->stat);


  iPos = tab->ind[tab->iNr - 1];
  *cLast = &tab->tab[iPos];

  // printf("ex UtxTab_query |%s| %d %d\n",&tab->tab[iPos],tab->iNr,iPos);

  return tab->iNr;

}


//================================================================
  char* UtxTab__ (int iNr, TxtTab *tab) {
//================================================================
/// get text of record Nr iNr  (returns pointer !)

  if(iNr < 0) return NULL;
  if(iNr >= tab->iNr) return NULL;
  if(!tab->tab) return NULL;

  return &tab->tab[tab->ind[iNr]];

}


//================================================================
  int UtxTab_nr (TxtTab *tab) {
//================================================================
/// returns nr of strings

  return tab->iNr;

}


//================================================================
  int UtxTab_bytes_free (TxtTab *xTab) {
//================================================================
/// returns nr of free bytes in TxtTab

  return (xTab->tabSiz - xTab->ind[xTab->iNr]);

}


//================================================================
  char* UtxTab_pos_free (TxtTab *xTab) {
//================================================================
/// returns next free position in memspace of TxtTab

  // iNr  = txTab1.iNr;     // UtxTab_nr (&txTab1);
  // iPos = txTab1.ind[iNr];
  // optLst = (char**)&(txTab1.tab[iPos]);
    // printf(" iNr=%d iPos=%d\n",iNr,iPos);

  return (&(xTab->tab[xTab->ind[xTab->iNr]]));

}


//================================================================
  int UtxTab_dump (TxtTab *tab, char *inf) {
//================================================================
/// dump all strings (test)

  int  i1, i2;

  printf("----------------- UtxTab_dump %s\n",inf);
  printf("  indSiz=%ld tabSiz=%ld iNr=%d stat=%d\n",
         tab->indSiz, tab->tabSiz, tab->iNr, tab->stat);


  for(i1=0; i1<tab->iNr; ++i1) {
    i2 = tab->ind[i1];
    printf (" tab[%d] Pos=%d |%s|\n",i1,i2,&tab->tab[i2]);
  }

  return 0;

}


//====================== EOF ===========================
