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
2017-11-22 New.
-----------------------------------------------------
*/
#ifdef globTag
void MemTab1(){}
#endif
/*!
\file  ../ut/ut_memTab1.c
\brief fixed-length-records in memory: add,insert,delete, realloc. .. MemTab_ 
\code
=====================================================
List_functions_start:

------------------- int's
MemTabI_add_s       add n int's to int-table (add consecutive values)

------------------- IndTab
MemTabIT_add__      add IndTab = table of int's

List_functions_end:
=====================================================
- necessary includes:


\endcode *//*----------------------------------------

Testprog: ../ut/tst_memTab.c


*/


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_geo_const.h"        // _INDTAB_NUL
#include "../ut/ut_memTab.h"
#include "../ut/ut_itmsh.h"            // MSHIG_EDGLN_.. typedef_MemTab.. Fac3





//======================================================================
  int MemTabIT_add__ (MemTab(IndTab) *tab, int iis, int iNr, int iTyp) {
//======================================================================
// MemTabIT_add__      add IndTab = table of int's


  int       irc;
  long      ld;
  IndTab    it1 = _INDTAB_NUL;


  // add boundary-record -> ts1->tab
  it1.ibeg = iis;      // startindex
  it1.iNr  = iNr;      // nr of ints
  it1.typi = iTyp;     // MSH_EDGLN_OB|MSH_EDGLN_IB

  irc = MemTab_add (tab, &ld, &it1, 1, 0);
  // if(irc < 0) return irc;

  return irc;

}


//================================================================
  int MemTabI_add_s (MemTab(int) *ipa, int iNr, int iis) {
//================================================================
// MemTabI_add_s     add n int's to int-table (add consecutive values)
// Input:
//   iNr      nr of int's to add
//   iis      starting value of ints, next is iis+1
// Output:
//   ipa      int-arry added
//   retCod   startindex in ipa
//
//was MSH2D_init_ipa_add

  int      irc, i1, *ia;
  long     l1;


  // printf("MemTabI_add_s \n");

  // get memspc
  ia = (int*) MEM_alloc_tmp ((int)(sizeof(int)*iNr));

  // create consecutive numbers starting with ips
  for(i1=0; i1<iNr; ++i1) {
    ia[i1] = iis;
    ++iis;
  }

  // provide iNr reserved ints in ipa
  iis = MEMTAB_IND (ipa);
  irc = MemTab_add (ipa, &l1, ia, iNr, 0);
  if(irc < 0) iis = irc;

    // MemTab_dump (ipa, "ex-MemTabI_add_s");

  return iis;

}

// EOF
