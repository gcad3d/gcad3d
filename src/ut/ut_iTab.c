// ../ut/ut_iTab.c   IntegerTables.     2010-04-16   RF
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
2010-04-16 new created. RF.

-----------------------------------------------------
*/
#ifdef globTag
void I4Tab(){}
#endif
/*!
\file  ../ut/ut_iTab.c
\brief list of integers 
\code
=====================================================
List_functions_start:

U4Tab_NEW         setup of a new IntegerList
I4Tab_init_spc    init / clear table; fixed space (void*); no reallocate
// I4Tab_init__      init / clear table; automatic alloc/realloc
// I4Tab_init_Memspc init / clear table; fixed space in Memspc; no reallocate
// I4Tab_init_fu     init / clear table; save into temp-file.
I4Tab_free        reset, free

I4Tab_add         add integer
I4Tab_add_uniq    add integer, check if it already exists

I4Tab__           return int from index
I4Tab_find        find index of int
I4Tab_nr          return nr of ints

I4Tab_dump

List_functions_end:
=====================================================
see also:
UTI
MemTab_ini__
UtxTab_init__

\endcode *//*----------------------------------------

Functions with List of Integers;   I4Tab  IntTab I8Tab I2Tab
For fixed-size memoryspaces; no reallocate or filespace yet.
See also MemTab  ../ut/ut_memTab.h





=====================================================
Usage example - fixed space:
see main () below

  i1 = iTab1.iNr - 1;      // get last index
  i2 = iTab1.tab[i1];      // get last int in tab


=====================================================
Offline testen:
cd ../ut
cc -ggdb -pg ut_iTab.c -DOFFLINE&&./a.out
cl ut_iTab.c -DOFFLINE&&ut_txTab.exe
c

=====================================================
*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../ut/ut_iTab.h"             // I4Tab





//========================================================
//========================================================
#ifdef OFFLINE

#include <stdarg.h>

// GUI-funcs   in ../ut/ut_ui_TX.c
// Text-funcs  in ../ut/ut_TX.c
void TX_Print (char* txt, ...) { printf("%s\n",txt); }
void TX_Error (char* txt, ...) { TX_Print (txt); }



//============================
  int main (int paranz, char *params[]) {
//============================
  int        i1, irc;
  char       myMem[500];
  I4Tab_NEW  (iTab1);         // init integer-table


  I4Tab_init_spc (&iTab1, myMem, sizeof(myMem));

  // add a new int
  for(i1=288; i1<333; ++i1) {
    irc = I4Tab_add (&iTab1, i1);
    if(irc < 0) break;
  }

  I4Tab_add_uniq (&i1, &iTab1, 99);
  I4Tab_add_uniq (&i1, &iTab1, 299);
  I4Tab_add_uniq (&i1, &iTab1, 399);

  I4Tab_dump (&iTab1);

  // i1 = I4Tab_query (&p1, &iTab1);        // get last word
  // I4Tab_rem (&iTab1);                    // remove last word
  // i1 = I4Tab_query (&p1, &iTab1);        // get last word
    // printf(" %d |%s|\n",i1,p1);
  // i1 = I4Tab_query (&p1, &iTab1);        // get last word

  I4Tab_free (&iTab1);

  return 0;
}


#endif
//========================================================
//========================================================



//================================================================
  int I4Tab_init_spc (I4Tab *tab, void *spc, int isize) {
//================================================================
/// \code
/// init / clear all words; fixed space (void*); no reallocate.
/// RetCod:
/// \endcode


  printf("I4Tab_init_spc \n");


  // compute size of a record; 32 char for the string, 4 for the index = 36
  if(tab->tab == NULL) {
    tab->tab    = spc;
    tab->stat   = 0;                      // no reallocate
    tab->tabSiz = isize / sizeof(int);
  }

  tab->iNr    = 0;

  return 0;

}


//================================================================
  int I4Tab_free (I4Tab *tab) {
//================================================================
/// do not free I4Tab from I4Tab_init_Memspc

  // printf("I4Tab_free %d\n",tab->stat);

  if(tab->stat == 1) {
    if(tab->tab) {
      free (tab->tab);
      tab->tab = NULL;
    }
  }

  tab->iNr    = 0;

  return 0;

}


//================================================================
  int I4Tab_add (I4Tab *tab, int iNew) {
//================================================================
/// \code
/// add a new int
/// RetCode:
///  -1     EOM - no enough memory error
///  >=0    index of iNew in tab
/// \endcode

  int  i1, iNr, iLen, iPos, iEnd;


  // printf("I4Tab_add  |%s|\n",newtxt);
  // printf(" stat=%d\n",tab->stat);



  if(tab == NULL) {
    TX_Error ("******** I4Tab_add - E001 ***");
    return -1;
  }


  iNr = tab->iNr;   // active index
  if(iNr >= tab->tabSiz) {
    if(tab->stat == 0) return -1;     // fixed-size: cannot increase ..
    // realloc
/*
    i1 = tab->indSiz + TAB_I_SIZ;
    if(I4Tab_reall_I (&(tab->ind), i1) < 0) return -1;
    tab->indSiz = i1;
    // TX_Error ("******** I4Tab_add - index overflow *********");
    // return -1;
*/
  }

  tab->tab[iNr] = iNew;
  tab->iNr += 1;

  return iNr;

}

//================================================================
  int I4Tab_add_uniq  (int *iNr, I4Tab *tab, int iNew) {
//================================================================
/// \code
/// add string, check if it already exists
/// Output:
///   iNr    index of iNew in tab
/// RetCode:
///   -1     EOM - not enough memory error
///    0     new record added (iNew is uniq)
///    1     no new record added (iNew is not uniq)
/// \endcode


  int  i1, *iTab;

  // check if int <iNew> already exists;

  iTab = tab->tab;
  for(i1=0; i1<tab->iNr; ++i1) {
      // printf(" tab[%d] Pos=%d |%s|\n",i1,i2,&tab->tab[i2]);
    if(iNew == iTab[i1]) {
      *iNr = i1;
      return 1;     // alread exists 
    }
  }


  // add new int ..
  *iNr = I4Tab_add (tab, iNew);
  return 0;

}


//================================================================
  int I4Tab_find (int ii, I4Tab *tab) {
//================================================================
/// I4Tab_find       find int ii - returns nr; -1 = not found

  int  i1, *iTab;

  // printf("I4Tab_find |%s|\n",cs);

  iTab = tab->tab;
  for(i1=0; i1<tab->iNr; ++i1) {
    if(ii == iTab[i1]) return i1;     // alread exists 
  }

  return -1;

}


//================================================================
  int I4Tab__ (int ii, I4Tab *tab) {
//================================================================
/// get text of record Nr iNr  (returns value)

  if(ii < 0) return -1;
  if(ii >= tab->iNr) return -1;

  return tab->tab[ii];

}


//================================================================
  int I4Tab_nr (I4Tab *tab) {
//================================================================
/// returns nr of strings

  return tab->iNr;

}


//================================================================
  int I4Tab_dump (I4Tab *tab) {
//================================================================
/// dump all strings (test)

  int  i1;

  printf("I4Tab_dump %ld tot=%ld\n",tab->iNr,tab->tabSiz);

  for(i1=0; i1<tab->iNr; ++i1) {
    printf(" iTab[%d] =%d\n",i1,tab->tab[i1]);
  }

  return 0;

}


// EOF
