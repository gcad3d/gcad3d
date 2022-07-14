//**************************************************************************
/*
 *
 * Copyright (C) 2022 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
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

-----------------------------------------------------
Modifications:
-----------------------------------------------------
*/
#ifdef globTag
void ITAB(){}
#endif
/*!
\file  ../ut/edga.c

=====================================================
List_functions_start:

ITAB_init__            see example ITAB_test__
ITAB__add__            add polygon iTab[iNr] to iTab/iDat
ITAB__del_itb         delete record in IntTabTable
ITAB__ins_itb         insert record(s) in IntTabTable
ITAB_itb_iTyp          get data from type                                      
ITAB_itb_iTyp          get data of IntTab-record with index ii
ITAB_iitb_iTyp         get recordNr of IntTabTable from type
ITAB__upd              recover IntTab-pointers ia
ITAB__ck_iTyp          check if breakline(s) exist
ITAB__dump__           dump IntTabs (IntTab+iTab - eTab+eDat)

ITAB_test__            example-code

List_functions_end:
=====================================================
see also:
MSH_EL_*      ../myGIS1/GIS__.c
gis_edg_*     ../myGIS1/Gis_u1.c

*/
 void INF_IntTab () {} /*--------------------------------------

INF_IntTab      group of integer-tables
- for integer-data a MemTab(int) must be provided.

ITAB consists of an array of IntTab's (MemTab(IntTab))
  and an array of integers (MemTab(int))


struct IntTab:
//  iNr    nr of objects in ia
//  ia     array integers
//  typ    type of data;
//  aux    aux-info, 0=default ..
//  stat   status, 0=default ..
typedef struct {int *ia, iNr; char typ, aux, stat;}                 IntTab;


Example: see ITAB_test__


*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_itmsh.h"            // MSHIG_EDGLN_.. typedef_MemTab.. Fac3
#include "../ut/itab.h"                // ITAB_itb_iTyp ..




//================================================================
  IntTab* ITAB_itb_iTyp (MemTab(IntTab) *eTab, int typ) {
//================================================================
// ITAB_itb_iTyp          get data from type                                      

  int    i1;
  IntTab *ela;

  for(i1=0; i1<eTab->rNr; ++i1) {
    if(eTab->data[i1].typ == typ) {
      ela = &eTab->data[i1];
      goto L_exit;
    }
  }

  ela = NULL;

  L_exit:
      // printf("ex ITAB_iitb_iTyp %d %d\n",ii, elTyp);
    return ela;

}


//================================================================
  IntTab* ITAB_itb_iitb (MemTab(IntTab) *eTab, int ii) {
//================================================================
// ITAB_itb_iTyp          get data of IntTab-record with index ii

  IntTab *ela;

  if(eTab->rNr > ii) {
    ela = &eTab->data[ii];

  } else {
    ela = NULL;
  }

  return ela;

}


//====================================================================
  int ITAB__add__ (MemTab(IntTab) *eTab,  MemTab(int) *eDat,
                     int *iTab, int iNr, int typ, int aux, int stat) {
//====================================================================
// add polygon iTab[iNr] to Etab/Etab
// Input:
//   iTab      iNr indexes into pointSet
//   iNr       nr of points
//   typ       eg MSH_EDGLN_AB
//   dispMode  0=update display, 1=do not update display
// Output:
//   retCod    >=0   index of new created record in eTab
//             < 0   error


  int      irc, ii;
  long     l1;
  IntTab el1;


  // printf("ITAB__add__ iNr=%d typ=%d\n",iNr,typ);
  // for(ii=0;ii<iNr;++ii) printf("ip[%d]=%d\n",ii,iTab[ii]);


  // add one more IntTab in Etab
  ii = eTab->rNr;
  el1.iNr = iNr;
  el1.typ = typ;
  el1.aux = aux;
  el1.stat = stat;
  MemTab_sav (eTab, &l1, &el1, 1);
  if(eTab->data == NULL) goto L_EOM1;


  // add/copy data; copy iTab[iNr] into eTab as <typ>
  MemTab_sav (eDat, &l1, iTab, iNr);
  if(eDat->data == NULL) goto L_EOM2;


  // die pointers Etab[].ia neu aufbauen
  ITAB__upd (eTab, eDat);


  // display IntTab
  // if(dispMode == 0) gis_edg_disp__ (&eTab->data[ii], ind);

  // ActFtabModFlag = 1  (modified; need updated)
  // gis_changed_mshMod (1);


  irc = eTab->rNr - 1;


  //----------------------------------------------------------------
  L_exit:
    // TESTBLOCK
    // ITAB__dump__ (&Etab, &Etab, "ex-ITAB__add__");
    // printf(" ex-ITAB__add__-irc=%d\n",irc);
    // END TESTBLOCK

  return irc;


  //----------------------------------------------------------------
  L_EOM1:
  TX_Print("ITAB__add__ EOM-eTab"); {irc = -1; goto L_exit;}

  L_EOM2:
  TX_Print("ITAB__add__ EOM-eDat"); {irc = -2; goto L_exit;}

}


//================================================================
  int ITAB__del_itb (MemTab(IntTab) *eTab, MemTab(int) *eDat,
                       int elInd) {
//================================================================
// ITAB__del_itb         delete record in IntTabTable

// see MSH_EL_del_pt MSH_EL_get_tab ITAB__upd

  int     i1, iSta, iNr, iLast;


  // printf("ITAB__del_itb %d\n",elInd);
  // ITAB__dump__ (eTab, eDat, "in-ITAB__del_itb");


  // if last record of eTab - remove last
  iLast = eTab->rNr -1;
  if(elInd == iLast) {
    eDat->rNr -= eTab->data[iLast].iNr;
    eTab->rNr -= 1;  // remove last eTab-record
    goto L_exit;
  }


  // iSta = startPos of datavalues in eDat
  // if((elInd < 0)||(elInd > Etab.rNr)) iSta = Etab.rNr;
  iSta = 0;
  for(i1=0; i1<elInd; ++i1) iSta += eTab->data[i1].iNr;


  iNr  = eTab->data[elInd].iNr;
    // printf(" iSta=%d iNr=%d\n",iSta,iNr);


  // delete Etab-record Nr elInd
  MEM_del_nrec (&eTab->rNr, eTab->data, elInd, 1, eTab->rSiz);


  // delete data out of iDat
  if(iNr > 0)
  MEM_del_nrec  (&eDat->rNr, eDat->data, iSta, iNr, eDat->rSiz);


  // die pointers Etab[].ia neu aufbauen
  ITAB__upd (eTab, eDat);


  L_exit:
    // ITAB__dump__ (eTab, NULL, "ex-ITAB__del_itb");
  return 0;

}


//================================================================
  int ITAB__ins_itb (MemTab(IntTab) *eTab, MemTab(int) *eDat,
                      int irs, int iPos, int *ia,  int iNr) {
//================================================================
// ITAB__ins_itb         insert record(s) in IntTabTable
// Input:
//   irs      index dataset to modify in eTab
//   iPos     keep records 0 - iPos-1; insert ia, then add the rest (iPos - iNr)
//   ia       data to insert
//   iNr      nr of records to insert
//
// iPos = 0 inserts ia as first data
// iPos = 1 inserts ia as record [1], keeping record [0]
// to insert at the begin of the table set iPos = 0


  int        i1, ii, ieAB, *ipaNew, ipnNew, iTyp;
  IntTab   *ela;


  // printf("ITAB__ins_itb irs=%d iPos=%d iNr=%d\n",irs,iPos,iNr); 

  // get ieAB = index AB
  ela = &eTab->data[irs];
  iTyp = ela->typ;

  // get space for new bnd
  i1 = ela->iNr + iNr;
  ipaNew = (int*) MEM_alloc_tmp (i1 * sizeof(int));

  ipnNew = 0;

  for(i1=0; i1<iPos; ++i1) {ipaNew[ipnNew] = ela->ia[i1]; ++ipnNew;}

  for(i1=0; i1<iNr; ++i1) {ipaNew[ipnNew] = ia[i1]; ++ipnNew;}

  for(i1=iPos; i1<ela->iNr; ++i1) {ipaNew[ipnNew] = ela->ia[i1]; ++ipnNew;}

    // TESTBLOCK
    // for(i1=0; i1<ipnNew; ++i1) printf(" ipaNew[%d] = %d\n",i1,ipaNew[i1]);
    // END TESTBLOCK


  // delete existing AB
  ITAB__del_itb (eTab, eDat, irs);

  // add new AB
  ITAB__add__ (eTab, eDat, ipaNew, ipnNew, iTyp, 0, 0);

  return 0;

}


//================================================================
  int ITAB_iitb_iTyp (MemTab(IntTab) *eTab, int elTyp) {
//================================================================
// get recordNr of IntTabTable from edgeLineTyp
// RetCod:
//    RecordNr; -1 = notExists.

  int    i1, ii;

  ii = -1;

  for(i1=0; i1<eTab->rNr; ++i1) {

    if(eTab->data[i1].typ == elTyp) {
      ii = i1;
      break;
    }
  }

    // printf("ex ITAB_iitb_iTyp %d %d\n",ii, elTyp);

  return ii;

}


//================================================================
  int ITAB__upd (MemTab(IntTab) *eTab, MemTab(int) *eDat) {
//================================================================
// ITAB__upd      recover IntTab-pointers ia
// (nach relocation) die pointers neu aufbauen

  int    i1, ii;

  // get ii = actual used size of iDat
  ii = 0;
  for(i1=0; i1<eTab->rNr; ++i1) {
    eTab->data[i1].ia = &eDat->data[ii];
      // printf(" _fix eTab[%d] = %d\n",i1,ii);
    ii += eTab->data[i1].iNr;
  }

  return 0;

}


//================================================================
   int ITAB__ck_iTyp (MemTab(IntTab) *eTab, int eTyp) {
//================================================================
// ITAB__ck_iTyp          check if breakline(s) exist; 1=yes, 0=no

  int    i1, ii;

  for(i1=0; i1<eTab->rNr; ++i1) {
    // if(eTab->data[i1].typ == MSH_EDGLN_BL) return 1;
    if(eTab->data[i1].typ == eTyp) return 1;
  }

  return 0;

}


//==========================================================================
  int ITAB__dump__ (MemTab(IntTab) *eTab, MemTab(int) *eDat, char *txt) {
//==========================================================================
/// ITAB__dump__         dump IntTabs
// see also MSH_dump_EL gis_edg_dump

  int        i1, i2, ii, iNr, *ia;
  IntTab   *ela;


  // printf("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE ITAB__dump__\n");
  printf("------------------------------- ITAB__dump__ %s\n",txt);
 (Typ_MemTab, eTab, "----- ITAB__dump__ %s", txt);
  DEB_dump_obj__ (Typ_MemTab, eTab, "");


  // Etab.data is a table of IntTabs
  for(i1=0; i1<eTab->rNr; ++i1) {
    ela = &eTab->data[i1];
    printf("eTab[%d].iNr=%d typ=%d stat=%d\n",i1,ela->iNr,ela->typ,ela->stat);
    for(i2=0; i2<ela->iNr; ++i2) {
      printf("    i[%d]=%d\n",i2,ela->ia[i2]);
    }
  }

  //----------------------------------------------------------------
  if(eDat) {
    DEB_dump_obj__ (Typ_MemTab, eDat, "eDat:");
    ii = 0;
    ia = &eDat->data[0];
    for(i1=0; i1<eTab->rNr; ++i1) {
      ela = &eTab->data[i1];
      printf(" ----------- record %d startIndex %d nr %d\n",i1,ii,ela->iNr);
      for(i2=0; i2<ela->iNr; ++i2) {
        printf("eDat %d [%d]=%d\n",i1,i2,ia[i2]);
      }
      ii += ela->iNr;
      ia = &eDat->data[ii];
    }
  }

  printf("------------------------------- ex-ITAB__dump__\n");

  return 0;

}

//=============================================================================
  int ITAB_init__ (MemTab(IntTab) *iTab, MemTab(int) *iDat, int tNr, int iNr) {
//=============================================================================
// tNr   nr of tables to init/increase
// iNr   nr of data (ints) to init/increase (used for all tables)


  // printf("ITAB_init__ \n");

  // init new iTab
  MEMTAB_SET_NULL (iTab);
  MemTab_ini__ (iTab, sizeof(IntTab), Typ_IntTab, tNr);

  MEMTAB_SET_NULL (iDat);
  MemTab_ini__ (iDat, sizeof(int), Typ_Int4, iNr);

  return 0;

}


//================================================================
  int ITAB_test__ () {
//================================================================
// code-example

  int    irc, ii, iNr, *ia;
  int    ia1[] = { 1, 3, 5, 7};
  int    ia2[] = { 2, 4, 6, 8};
  int    ia3[] = { 12, 24, 36, 48, 50};
  int    ia4[] = { 40, 41, 42};
  IntTab *ela;


  MemTab(IntTab) iTab;
  MemTab(int) iDat;

  printf("ITAB_test__ \n");

  // init new iTab
  irc = ITAB_init__ (&iTab, &iDat, 10, 100);
  if(irc < 0) {printf("***** ERROR-EOM\n"); return -1;}

  // fill iTab
  irc = ITAB__add__ (&iTab, &iDat, ia1, sizeof(ia1)/sizeof(int), 1,0,0);
  irc = ITAB__add__ (&iTab, &iDat, ia2, sizeof(ia2)/sizeof(int), 2,0,0);
  irc = ITAB__add__ (&iTab, &iDat, ia3, sizeof(ia3)/sizeof(int), 3,0,0);
  if(irc < 0) {printf(" EOM\n"); return -1;}
  
  // dump iTab
  ITAB__dump__ (&iTab, NULL, "ITAB_test__-1");

  // get ii = index of table typ=2
  ii = ITAB_iitb_iTyp (&iTab, 2);

  // replace table typ = 2 with ia4
  ITAB__del_itb (&iTab, &iDat, ii);
  irc = ITAB__add__ (&iTab, &iDat, ia4, sizeof(ia4)/sizeof(int), 2,0,0);

  // dump iTab
  ITAB__dump__ (&iTab, NULL, "ITAB_test__-2");


  // get data of IntTab-record typ=3
  ela = ITAB_itb_iTyp (&iTab, 3);
  if(!ela) {printf("***** ERROR-E1\n"); return -1;}
  iNr = ela->iNr;
  ia  = ela->ia;
    for(ii=0; ii<iNr; ++ii) printf(" ia[%d] = %d\n",ii,ia[ii]);


  // get the IntTab-index of record typ=2
  ii = ITAB_iitb_iTyp (&iTab, 2);
    printf(" ii=%d\n",ii);


  // get data of IntTab-record with index ii
  ela = ITAB_itb_iitb (&iTab, 2);


  // free iTab
  MemTab_free (&iTab);
  MemTab_free (&iDat);



  return 0;

}

 
//================  EOF  ==================================
