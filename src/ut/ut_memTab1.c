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
MemTab_int_test       test MemTab_int_add_range

------------------- IndTab
MemTab_IndTab_add     add IndTab = table of int's
MemTab_IndTab_iFind   find index of indtab-record with typi
MemTab_IndTab_ck      check if typ,ind is in MemTab(IndTab)
MemTab_IndTab_test

List_functions_end:
=====================================================
------------------- Point
UT3D_mtpt_obj
UT3D_mtpt_trmCv

see also ../ut/ut_memTab.c
- necessary includes:



IntTab - a list of integers for organizing a MemTab(int)
IndTab - organize groups of integers in IntTab (2 types of typ char)



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

#include "../xa/xa_msg.h"              // MSG_* ERR_*



//================================================================
  int MemTab_int_add_range (MemTab_int *ipa, int irs, int iNr) {
//================================================================
// MemTab_int_add_range   add iNr records of incrementing integers starting with irs
//   ipa must be MemTab(int);
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


//==================================================================
  IndTab* MemTab_IndTab_iFind (void* iTab, int iTyp) {
//==================================================================
// MemTab_IndTab_iFind   find index of indtab-record with typi
// retCode  NULL  record not found;
//          else  indtab-record with typi == <iTyp>

  int     ii, rNr;
  IndTab  *inda;

  rNr = ((MemTab*)(iTab))->rNr;
  inda = (IndTab*)((MemTab*)iTab)->data;

  for(ii=0; ii<rNr; ++ii) if(inda[ii].typi == iTyp) return &inda[ii];

  return NULL;

}


//================================================================
  int MemTab_IndTab_ck (MemTab(IndTab) *iTab, int typ, long ind) {
//================================================================
// MemTab_IndTab_ck          check if typ,ind is in MemTab(IndTab)
// RetCod:
//   0      yes typ,ind is inside a range-obj;
//  -1      no.


  int    irc, i1;


  // printf("MemTab_IndTab_ck %d %d\n",typ,ind);

  irc = -1;

  for(i1=0; i1<iTab->rNr; ++i1) {
    if(iTab->data[i1].typi != typ) continue;
    if(iTab->data[i1].ibeg > ind) continue;
    if(iTab->data[i1].ibeg + iTab->data[i1].iNr < ind) continue;
    irc = 0;
    break;
  }


  // printf("ex MemTab_IndTab_ck irc=%d typi=%d ind=%d\n",irc,typi,ind);

  return irc;

}


//================================================================
  int MemTab_IndTab_all (int *ia, IndTab *ora, int orNr) {
//================================================================
// MemTab_IndTab_all                get list of integers of integer-ranges
// - get a list of all numbers of all lists
//  
// Input:
//   ia     must have size >= size reported if (ia=NULL)
//   ora    orNr IgaTab-records to search
//   orNr   nr of IgaTab-records
// Output:
//   ia    list of ints of orNr IgaTab-records  (get size if ia=NULL)
//   retCod  necessary size of ia (only if ia=NULL)
//
// was UTI_ni_ObjRange

  int   io, ir, i1, i2, ii;

  io = 0;

  if(!ia) {
    // ia = NULL: reported size of ia
    for(ir=0; ir<orNr; ++ir) if(ora[ir].iNr) io += ora[ir].iNr;
      // printf("ex-ni_IgaTab siz=%d \n",io);
    return io;
  }

  for(ir=0; ir < orNr; ++ir) {
    if(ora[0].iNr) {
      i1 = ora[ir].ibeg;
      i2 = i1 + ora[ir].iNr;
      for(ii=i1; ii<i2; ++ii) {
        ia[io] = ii;
        ++io;
      }
    }
  }

    // TESTBLOCK
    // printf("ex-MemTab_IndTab_all ia %d\n",io);
    // for(i1=0; i1<io; ++i1) printf(" ni[%d] = %d\n",i1,ia[i1]);
    // END TESTBLOCK

  return 0;

}


//================================================================
// TESTFUNCTIONS
//================================================================

#ifdef DEB



//================================================================
  int MemTab_int_test () {
//================================================================
// MemTab_int_test              test MemTab_int_add_range

  int          irc;
  MemTab(int)  ipa = _MEMTAB_NUL;


  printf("\nMemTab_int_test \n");


  MemTab_ini__ (&ipa, sizeof(int), Typ_Int4, 100);

  irc = MemTab_int_add_range (&ipa, ipa.rNr, 10);
  if(irc < 0) return MSG_ERROR (irc, "E1");

  irc = MemTab_int_add_range (&ipa, ipa.rNr, 15);
  if(irc < 0) return MSG_ERROR (irc, "E2");

    MemTab_dump (&ipa, "ex-MemTab_int_test");


  return 0;

}


//=========================================================================
  int MemTab_IndTab_test () {
//=========================================================================
// test MemTab_IndTab


  int   irc, ii;
  MemTab(IndTab)  intb1 = _MEMTAB_NUL;


  irc = MemTab_ini__ (&intb1, sizeof(IndTab), Typ_IndTab, 16);
  if(irc < 0) return MSG_ERROR (ERR_EOM, "");

  for(ii=0; ii<10; ii+=3) 
    MemTab_IndTab_add (&intb1, ii, 3, 12);

    MemTab_dump (&intb1, "SMSH_split__-L_DONE:");


  MemTab_free (&intb1);

  return 0;

}


//=========================================================================
  int MemTab_IndTab_dump (MemTab *mtb, char *inf) {
//=========================================================================
// test MemTab_IndTab
// Input:
//   mtb      *MemTab_IndTab


  int     irc, i1, i2, i3, *ia, iNr;
  IndTab  *ita, *it1;


  MemTab_dump (mtb, inf);

  ita = mtb->data;

  for(i1=0; i1<mtb->rNr; ++i1) {
    i2 = ita[i1].ibeg;
    i3 = i2 + ita[i1].iNr - 1;
    printf(" tab[%d] typi=%d  ipFirst=%d .. ipLast=%d\n",i1,ita[i1].typi,i2,i3);
  }


  return 0;

}


#endif

// EOF
