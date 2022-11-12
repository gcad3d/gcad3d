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
void MEMTAB1(){}
#endif
/*!
\file  ../ut/ut_memTab1.c
\brief fixed-length-records in memory: add,insert,delete, realloc. .. MemTab_ 

- necessary includes:
#include "../ut/ut_memTab.h"              // MemTab


=====================================================
List_functions_start:

------------------- int
MemTab_int_add_range  add iNr records of incrementing integers starting with iis

------------------- IndTab
MemTab_IndTab_add     add IndTab = table of int's

List_functions_end:
=====================================================
------------------- Point
UT3D_mtpt_obj
UT3D_mtpt_trmCv

see also ../ut/ut_memTab.c
- necessary includes:



IntTab - a list of integers for organizing a MemTab(int)
IndTab - organize groups of integers in IntTab (2 types of typ char)
IgaTab - organize groups of integers in IntTab (2 types: typ char and typ short)




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



//================================================================
  int MemTab_int_add_range (MemTab_int *ipa, int irs, int iNr) {
//================================================================
// MemTab_int_add_range   add iNr records of incrementing integers starting with iis
//   mtb must be MemTab(int);
//   irs is first integer to be saved, followed by irs+1 ..
// Input:
//   iNr      nr of int's to add
//   iis      starting value of ints, next is iis+1
// Output:
//   ipa      int-arry added
//   retCod   startindex in ipa
// was MemTabI_add_s MSHIG_init_ipa_add

  int      irc, i1, *ia, iis;
  long     l1;


  // printf("MemTab_int_add_range \n");

  // get memspc
  ia = (int*) MEM_alloc_tmp ((int)(sizeof(int)*iNr));

  // create consecutive numbers starting with ips
  for(i1=0; i1<iNr; ++i1) {
    ia[i1] = irs;
    ++irs;
  }

  // provide iNr reserved ints in ipa
  iis = MEMTAB_IND (ipa);
  irc = MemTab_add (ipa, &l1, ia, iNr, 0);
  if(irc < 0) iis = irc;

     // MemTab_dump (ipa, "ex-MemTab_int_add_range");

  return iis;

}


//=========================================================================
  int MemTab_IndTab_add (MemTab_IndTab *tab, int iis, int iNr, int iTyp) {
//=========================================================================
// MemTab_IndTab_add      add IndTab = table of int's
// was MemTabIT_add__


  int       irc;
  long      ld;
  IndTab    it1 = _INDTAB_NUL;


  // add boundary-record -> ts1->tab
  it1.ibeg = iis;      // startindex
  it1.iNr  = iNr;      // nr of ints
  it1.typi = iTyp;     // MSH_EDGLN_OB|MSH_EDGLN_IB

  return MemTab_add (tab, &ld, &it1, 1, 0);

}


// EOF
