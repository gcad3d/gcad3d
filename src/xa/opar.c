// Object-parent-table.                    RF 2017-03-26.
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
2017-03-26 recycled from GA_parent_*. RF.

-----------------------------------------------------
*/
#ifdef globTag
 void OPAR(){}
#endif
/*!
\file  ../xa/xa_ga.c
\brief Object-parent-table. 
\code
=====================================================
List_functions_start:

OPAR_init       init or reset
OPAR_set        add a new record
OPAR_get        get parent of Child
OPAR_del        delete record
OPAR_file       write|read ParentTable
OPAR_dump

List_functions_end:
=====================================================
see also:
DL_parent_hide
DL_parent_ck_c
DL_parent_ck_p
APT_work_parent_hide
AP_parent_get   get all parents of a DB-obj

\endcode *//*----------------------------------------


parents have:
- objs from "TRA","PRJ","MIR" have parents; disp-list.sChd = 1.
  ParTab-record stores dbo parent and child
  find parent with OPAR_get
  

- trimmed-curves from "CUT" have parents; disp-list.sChd = 1.
  no ParTab-record; parents in bin. struct.
  find parent with CVTRM_basCv_trmCv




---------------------------------------------------
*/


#ifdef _MSC_VER
#include "MS_Def0.h"
#endif

#include <string.h>                       /* strcmp ..   */
#include <math.h>                         /* fabs  ...   */
#include <stdio.h>                        /*  FILE .. */
#include <stdlib.h>                       // realloc

#include "../ut/ut_memTab.h"              // MemTab_..



// cldDbi      dbi of child
// parDbi      dbi of parent
// cldTyp      typ of child
// parTyp      typ of parent
typedef struct {long cldDbi, parDbi; short cldTyp, parTyp;}         Parent;
// typedef struct {long cInd, pInd; int typ;}                    Parent;


// Parent-table ParTab:
// Am DL-Record des parent ist das .sPar-Flag gesetzt;
// am DL-Record des child ist das .sChd-Flag gesetzt.
typedef_MemTab(Parent);
static MemTab(Parent) ParTab = MemTab_empty;    // see ../xa/xa_ga.h




//================================================================
  int OPAR_init () {
//================================================================
// OPAR_init            init or reset

  MemTab_ini (&ParTab, sizeof(Parent), 0, 100);


  return 0;
}


//======================================================================
  int OPAR_set (int parTyp, long parDbi, int cldTyp, long cldDbi) {
//======================================================================
// add a new record to parent-table (Parent*)ParTab

  long    i1, ld;
  Parent  pr1, *pTab;


  // printf("OPAR_set p: %d %ld c: %d %ld\n",parTyp,parDbi,cldTyp,cldDbi);


  pr1.parTyp = parTyp;
  pr1.parDbi = parDbi;

  pr1.cldTyp = cldTyp;
  pr1.cldDbi = cldDbi;



  // loop tru ParTab - check if record already exists
  pTab = ParTab.data;
  for(i1=0; i1 < ParTab.rNr; ++i1) {
    // exit if record already exists
    if(memcmp(&pTab[i1], &pr1, sizeof(Parent)) == 0) return 1;
  }

  // printf("  _parent_set save typ=%d cInd=%d pInd=%d\n",typ,cInd,pInd);


  return MemTab_sav (&ParTab, &ld, &pr1, 1);              // add 1 record

}


//========================================================================
  int OPAR_get (int *parTyp, long *parDbi, int cldTyp, long cldDbi) {
//========================================================================
// get parent of Child; 
// RetCod: parentIndex; -1=not found.

  int      typ;
  long     i1;
  Parent   *pTab;


  // printf("OPAR_get %d %ld\n",cldTyp,cldDbi);
  // OPAR_dump ();


  cldTyp = AP_typ_2_bastyp (cldTyp);

  pTab = ParTab.data;
  for(i1=0; i1 < ParTab.rNr; ++i1) {
    if(pTab[i1].cldTyp != cldTyp) continue;
    if(pTab[i1].cldDbi != cldDbi) continue;
    *parTyp = pTab[i1].parTyp;
    *parDbi = pTab[i1].parDbi;
      // printf(" ex OPAR_get %d %d %d\n",*dbi,typ,cInd);
    return i1;
  }

  return -1;
}


//================================================================
  int OPAR_dump () {
//================================================================

  int      i1;
  Parent   *pTab;

  printf("======= OPAR_dump %d =========== \n",ParTab.rNr);

  pTab = ParTab.data;

  for(i1=0; i1 < ParTab.rNr; ++i1) {
    printf(" %3d cldTyp %2d cldDbi %4ld    parTyp %2d parDbi %4ld\n",i1,
           pTab[i1].cldTyp, pTab[i1].cldDbi,
           pTab[i1].parTyp, pTab[i1].parDbi);
  }


  return 0;

}


//================================================================
  int OPAR_del (long cldDbi, int cldTyp) {
//================================================================
// delete record.
// UNUSED

  long     i1;
  Parent   *pTab;

  pTab = ParTab.data;
  for(i1=0; i1 < ParTab.rNr; ++i1) {
    if(pTab[i1].cldDbi != cldDbi) continue;
    if(pTab[i1].cldTyp != cldTyp ) continue;
    MemTab_del (&ParTab, i1, 1);
    break;
  }

  return 0;
}


//================================================================
  int OPAR_file (int mode, FILE *fp1) {
//================================================================
// write|read ParentTable
 
  if(mode == 1) {          // write
    MemTab_write (fp1, &ParTab);

  } else if(mode == 2) {   // read
    MemTab_read (fp1, &ParTab);
    // if(ParTab.rNr < 1) MemTab_clear (&ParTab);

  }


  return 0;
}


//========================= EOF =================================
