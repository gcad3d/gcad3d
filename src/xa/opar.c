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

MEMTAB_tmpSpc_get    get tempSpc for <siz> parent-records
OPAR_get_src       get parents of srcObj oPar

// OPAR_init       init or reset
// OPAR_set        add a new record
// OPAR_get        get parent of Child
// OPAR_del        delete record
// OPAR_file       write|read ParentTable
// OPAR_dump

List_functions_end:
=====================================================
see also:
APT_ato_par_srcLn         get atomicObjects and parents
ATO_parents__             get all parents
DL_parent_hide
DL_parent_ck_c
DL_parent_ck_p
APT_work_parent_hide

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

#include "../ut/ut_geo.h"                 // ObjSRC
#include "../ut/ut_memTab.h"              // MemTab_..
#include "../ut/ut_txfil.h"               // UTF_GetPosLnr
#include "../xa/xa_ed_mem.h"              // typedef_MemTab(ObjSRC)
// #include "../xa/opar.h"                   // MEMTAB_tmpSpc_get



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
static MemTab(Parent) ParTab = _MEMTAB_NUL;    // see ../xa/xa_ga.h



//================================================================
  int OPAR_get_src (MemTab(ObjSRC) *mtPar, ObjSRC *oPar) {
//================================================================
// OPAR_get_src                       get parents of srcObj oPar
// see UNDO_ln_del APT_ato_par_srcLn


  int            i1;
  long           ll;
  char           *lPos, *oSrc;


  // printf("OPAR_get_src typ=%d dbi=%ld lNr=%ld\n",
         // oPar->typ,oPar->dbi,oPar->lnr);


  // get sourceline
  // if(lPos != NULL) {
    // sourceline not defined ..
    if(!oPar->lnr) {
      // lineNr not defined; get it ..
      oPar->lnr = DL_find_obj (oPar->typ, oPar->dbi, -1L);
    }

    // get sourceline
    lPos = UTF_GetPosLnr (&ll, oPar->lnr);
    if(!lPos) {TX_Error("OPAR_get_src E1"); return -1;}
  // }
    // printf(" lnr=%ld\n",oPar->lnr);
    // printf("ln=|");UTX_dump_cnl(lPos,40);printf("|\n");


  // skip dest-obj; but can be disactivated (stating with '_')
  if(lPos[0] == '_') ++lPos;
  i1 = APED_ck_defLn (&oSrc, lPos);
  if(i1) {TX_Error("OPAR_get_src E2"); return -1;}

  // get max nr of atomic-objs
  i1 = SRCU_tsMax (oSrc);  // printf(" siz-mtPar = %d\n",i1);

  // get parents-table and atomic-objs
  APT_ato_par_srcLn (mtPar, NULL, oSrc);

    // MemTab_dump (mtPar, "ex-OPAR_get_src");

  return 0;

}

/*
//=================================================================
  int OPAR_get_src (int *tabNr, ObjDB *parTab, int tabSiz,
                     int typ, long dbi) {
//=================================================================
/// \code
/// OPAR_get_src              get all parents of a DB-obj
///   parents can be not uniq
/// Input:
///   tabNr       nr of already used records in parTab
///   typ,dbi     must be trimmedCurve
/// RetCod:       0=OK; -1 ERROR parTab too small
/// \endcode


  int     irc, i1, form, bTyp, cTyp, oNr;
  long    bDbi;
  void    *data, *bObj;
  CurvCCV *ccv1;


  printf("OPAR_get_src typ=%d dbi=%ld\n",typ,dbi);


  // Connection-lines in contours do not have DB-records (dbi=0).
  if(!dbi) return 1;
  
  cTyp = AP_typ_2_bastyp(typ);

  // if(AP_typ_2_bastyp(typ) != Typ_CV) return 0;

  // get curve
  form = DB_GetObjDat (&data, &oNr, cTyp, dbi);
    // printf(" form1=%d oNr=%d\n",form,oNr);
    // UT3D_stru_dump (form, data, " _basCv-1"); 

  if(form == Typ_CVTRM) {

    // loop tru all all obj's
    for(i1=0; i1<oNr; ++i1) {
      ccv1 = &((CurvCCV*)data)[i1];

      // get basic-curve
      irc = CVTRM_basCv_trmCv (&bTyp, &bDbi, &bObj, ccv1);
      if(irc < 0) continue;

      // add curve to list. Check uniq later.
      if(*tabNr >= tabSiz) return -1;
      parTab[*tabNr].typ   = ccv1->typ;
      parTab[*tabNr].dbInd = ccv1->dbi;
      parTab[*tabNr].dlInd = 0;
      *tabNr += 1;
    }

  } else {
    // get parent
    irc = OPAR_get (&bTyp, &bDbi, cTyp, dbi);
    if(irc > 0) {
      if(*tabNr >= tabSiz) return -1;
      parTab[*tabNr].typ   = bTyp;
      parTab[*tabNr].dbInd = bDbi;
      parTab[*tabNr].dlInd = 0;
      *tabNr += 1;
    }
  }

  // TESTBLOCK
  printf("ex OPAR_get_src %d\n",*tabNr);
  for(i1=0; i1 < *tabNr; ++i1)
    printf(" _parTab[%d] %d %ld\n",i1,parTab[i1].typ, parTab[i1].dbInd);
  // END TESTBLOCK

  return 0;

}


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


  printf("OPAR_get %d %ld\n",cldTyp,cldDbi);
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
    MemTab_wrf (fp1, &ParTab);

  } else if(mode == 2) {   // read
    MemTab_rdf (fp1, &ParTab);
    // if(ParTab.rNr < 1) MemTab_clear (&ParTab);

  }


  return 0;
}
*/

//========================= EOF =================================
