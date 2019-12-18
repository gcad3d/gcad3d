// ../ut/ut_prcv_db.c                 RF                      2019-05-03
/*
 *
 * Copyright (C) 2015 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
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
2019-05-03  new; replacement for slow PRCV_wrf__ PRCV_rdf__ (files). RF.

-----------------------------------------------------
*/
#ifdef globTag
void PRCV_DB(){}
#endif
/*!
\file  ../ut/ut_prcv_db.c
\brief PRCV database in memspc
\code
=====================================================
List_functions_start:

PRCV_DB_init        get memspc for 200 curves
PRCV_DB_save        add/replace PRCV
PRCV_DB_load        read PRCV
PRCV_DB_spc_add     add space for <ptNr> points
PRCV_DB_free        free one CurvPrcv
PRCV_DB_all_free    free|clear PRCV_GRP, delete all point-groups
PRCV_DB_fwrite      write PRCV-DB to file
PRCV_DB_fread       read PRCV-DB from file

PRCV_DB_ck_exist    check if PRCV already exists
PRCV_DB_dump

List_functions_end:
=====================================================

\endcode *//*----------------------------------------

*/



#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                         // for ...


#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_geo_const.h"        // UT3D_CCV_NUL
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_prcv__.h"           // 
#include "../ut/ut_os.h"               // OS_get_bas_dir ..
#include "../xa/xa_msg.h"              // MSG_* ERR_*
#include "../xa/xa_mem.h"              // memspc51, mem_cbuf1



//===========================================================================
// EXTERNALS:



//===========================================================================
typedef_MemTab(CurvPrcv);

static MemTab(CurvPrcv) PRCV_GRP = _MEMTAB_NUL;



//================================================================
  int PRCV_DB_init () {
//================================================================
// PRCV_DB_init        get memspc for 200 curves 
#define CV_NR 500

  // printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXX PRCV_DB_init \n");


  if(PRCV_GRP.spcTyp == MEMTYP_NONE) {
    // get memSpc for PRCV_GRP
    MemTab_ini__ (&PRCV_GRP, sizeof(CurvPrcv), Typ_PRCV, CV_NR);

  } else {
    // free PRCV_GRP, delete all point-groups
    PRCV_DB_all_free (0);
    // reset PRCV0
    PRCV0_free__ ();
  }


    // TESTBLOCK
    // DEB_dump_obj__ (Typ_MemTab, &PRCV_GRP, "ex-PRCV_DB_init");
    // END TESTBLOCK
 

  return 0;

}


//================================================================
  int PRCV_DB_spc_add (CurvPrcv *prc, int ptNr) {
//================================================================
/// \code
/// PRCV_DB_spc_add     get space for <ptNr> points
/// not enough memspc. malloc-realloc; heap-Error; empty-malloc.
/// Input:
///   ptNr      total size in nr of points
/// Retcodes:     0=OK, -1=Error-heap; -2=EOM
/// \endcode

  long     l1, l2, l3;
  CurvPrcv newPrc;


  // printf("PRCV_DB_spc_add-ptNr=%d prc.ptNr=%d siz=%d npt=%p\n",ptNr,
         // prc->ptNr,prc->siz,prc->npt);
  // printf("PRCV_DB_spc_add %d\n",ptNr);
  // DEB_dump_obj__ (Typ_PRCV, prc, "  prc ");
  // PRCV_dump__ (2, prc, "  prc-in");

  if(ptNr < 1 ) return 0;

  // if((prc->spcTyp > 0)&&(prc->spcTyp < 5)) {
  if(MEM_CANNOT_ALLOC(prc->spcTyp)) {
    TX_Error ("PRCV_DB_spc_add EOM-1");
    return -1;
  }

  // copy
  newPrc = *prc;

  // get space for the points + aux.data
  l1 = sizeof(Point) * ptNr;
  l2 = sizeof(double) * ptNr;
  l3 = sizeof(long) * ptNr;
  // prc->npt = realloc ((void*)prc->npt, l1+l2+l3);
  newPrc.npt = malloc (l1+l2+l3);
  if(!newPrc.npt) {
    TX_Error("PRCV_DB_spc_add E1");
    return -2;
  }

  newPrc.npar = (void*)(newPrc.npt + ptNr);
  newPrc.nipt = (void*)(newPrc.npar + ptNr);
  newPrc.siz  = ptNr;


  if(prc->siz > 0) {
    // copy existing points + aux.data
    l1 = sizeof(Point) * prc->ptNr;
    l2 = sizeof(double) * prc->ptNr;
    l3 = sizeof(long) * prc->ptNr;
    memcpy (newPrc.npt, prc->npt, l1);
    memcpy (newPrc.npar, prc->npar, l2);
    memcpy (newPrc.nipt, prc->nipt, l3);

    // free old prcv
    free (prc->npt);
  }


  // copy back
  *prc = newPrc;


    // PRCV_dump__ (2, prc, "ex-PRCV_DB_spc_add");
    // printf("ex-PRCV_DB_spc_add npt=%p\n",newPrc.npt);


  return 0;

}


//================================================================
  int PRCV_DB_save (CurvPrcv *prc) {
//================================================================
// PRCV_DB_save         add/replace PRCV
// if curve already exists: realloc;
// was PRCV_wrf__

  int     i1, isExist, indExist, ptNr;
  long    lOff, l1, l2, l3, l4;
  CurvPrcv   actCv, *cvp;
  // void    *data, *p1;


  // printf("\n------------------------ \n");
  // printf("PRCV_DB_save typ=%d dbi=%ld ptNr=%d siz=%d\n",
         // prc->typ,prc->dbi,prc->ptNr,prc->siz);
  // PRCV_dump__ (2, prc, "PRCV_DB_save-prc-in");
  // PRCV_DB_dump ("PRCV_DB_save-DB-in");


  l1 = sizeof(Point) * prc->ptNr;
  l2 = sizeof(double) * prc->ptNr;
  l3 = sizeof(long) * prc->ptNr;
  l4 = 0L;


  // check if record already exists
  isExist = PRCV_DB_ck_exist (&indExist, prc);

  // get memspace if already exists
  if(isExist) {
    // curve already exists;
    // get cvp = existing CurvPrcv
    cvp = MEMTAB__ (&PRCV_GRP, indExist);
      // DEB_dump_obj__ (Typ_PRCV, cvp, "PRCV_DB_save-I1");
    // actCv.npt = cvp->npt;
      // printf(" DB_save-exists;ptNr,siz=%d %d\n",cvp->ptNr,cvp->siz);

    // test if enough nr-of-points 
    if(prc->ptNr <= cvp->siz) {
      // enough space; overwrite points
      isExist = 2;
      actCv = *cvp;
      goto L_copy;

    } else {
      // need more space
        // printf(" DB_save-free;%p\n",cvp->npt);
      if(cvp->npt) free (cvp->npt);
      cvp->npt = NULL;
      // actCv        = *prc;
      // actCv.npt    = NULL;
    }

  }  // else {
    // curve not yet exists; add space for 2 points
    actCv     = *prc;
    actCv.npt = NULL;
    // callers may NOT free this memSpc -
    actCv.spcTyp = MEMTYP_ALLOC_PROT;
    // ptNr is also total size
    actCv.siz = prc->ptNr + 2;
    l4 = (sizeof(Point) + sizeof(double) + sizeof(long)) * 2;
  // }
    

  //================================================================
  // L_update:       // update existing record <indExist>
    // printf(" DB_sav-L_update ind=%d\n",indExist);

  // get space for the points + aux.data
  // actCv.npt = (void*)realloc ((void*)actCv.npt, l1+l2+l3);
  // add MSPC2R = space for 2 points
  actCv.npt = (void*)malloc (l1 + l2 + l3 + l4);
    // printf(" DB_save-malloc;%p\n",actCv.npt);
  if(!actCv.npt) {TX_Error("PRCV_DB_save E1"); return -1;}


  //----------------------------------------------------------------
  L_copy:
  // copy points + aux.data into new memChunk
  memcpy (actCv.npt, prc->npt, l1);
  actCv.npar = (void*)(actCv.npt + actCv.siz);
  // actCv.npar = (void*)(actCv.npt + prc->ptNr);
  // actCv.npar = (void*) ((char*)actCv.npt + l1);
  memcpy (actCv.npar, prc->npar, l2);
  actCv.nipt = (void*)(actCv.npar + actCv.siz);
  // actCv.nipt = (void*)(actCv.npar + prc->ptNr);
  // actCv.nipt = (void*)((char*)actCv.npar + l2);
  memcpy (actCv.nipt, prc->nipt, l3);


  // store the curve
  if(isExist == 2) {
    // only points have been overwritten
    // update ptNr
    cvp->ptNr = prc->ptNr;

  } else if(isExist == 1) {
    // free, malloc; replace existing 
    // *cvp = actCv;
    PRCV_GRP.data[indExist] = actCv;

  } else {
    // malloc; add new record ..
    MemTab_add (&PRCV_GRP, &lOff, &actCv, 1, 0);
  }

    // TESTBLOCK
    // DEB_dump_obj__ (Typ_MemTab, &PRCV_GRP, "ex-PRCV_DB_save-GRP");
    // PRCV_DB_dump ("ex-PRCV_DB_save-DB");
    // if(PRCV_GRP.rNr == 2) exit(1);
    // END TESTBLOCK


  return 0;

}


//================================================================
  int PRCV_DB_ck_exist (int *ii, CurvPrcv *prc) {
//================================================================
/// PRCV_DB_ck_exist      check if PRCV already exists
/// Input:
///   prc.typ,dbi,mdli    get this PRCV
/// Output:
///   ii                  index of existing PRCV into PRCV_GRP
///   retCod              0 = PRCV not yet exists;  1 = PRCV already stored
///
/// see PRCV_rdf__
  
  int        i1, iExist = 0;
  CurvPrcv   *cvp;


  // check if record already exists
  for(i1=0; i1<PRCV_GRP.rNr; ++i1) {
    cvp = MEMTAB__ (&PRCV_GRP, i1);
    if(cvp->typ != prc->typ) continue;
    if(cvp->dbi != prc->dbi) continue;
    if(cvp->mdli != prc->mdli) continue;
    // curve already exists;
    iExist = 1;
    *ii = i1;
    break;
  }

    // TESTBLOCK
    // printf(" ex-PRCV_DB_ck_exist %d ind=%d typ=%d dbi=%ld ptNr=%d\n",iExist,*ii,
            // prc->typ, prc->dbi, prc->ptNr);
    // PRCV_DB_dump ("ex-PRCV_DB_ck_exist");
    // END TESTBLOCK

  return iExist;

}


//================================================================
  int PRCV_DB_load (CurvPrcv *prc) {
//================================================================
/// PRCV_DB_load             read PRCV-DB;
/// Input:
///   prc.typ,dbi,mdli    get this PRCV
/// Output:
///   prc                 updates prc.npt,npar,nipt,ptNr,siz
///   retCode             -1  PRCV not yet stored;
///
/// spcTyp is set to ? (protected !)
/// see PRCV_rdf__

  int        i1, ii;
  CurvPrcv   *cvp;


  // printf("PRCV_DB_load typ=%d dbi=%ld siz=%d\n",prc->typ,prc->dbi,prc->siz);


  // check if record already exists
  i1 = PRCV_DB_ck_exist (&ii, prc);
  if(!i1) return -1;

  cvp = MEMTAB__ (&PRCV_GRP, ii);

  *prc        = *cvp;
  // set spcTyp to protected
  prc->spcTyp = MEMTYP_FIXED_PROT;

    // PRCV_dump__ (2, prc, "ex-PRCV_DB_load");

  return 0;

}


//================================================================
  int PRCV_DB_all_free (int mode) {
//================================================================
// PRCV_DB_all_free    free|clear PRCV_GRP, delete all point-groups
// Input:
//   mode    0=delete all; 1=delete points, clear PRCV_GRP;
// see PRCV0_free__

  int       i1;
  CurvPrcv  *cv_act;


  // printf("XXXXXXXXXXXXXXXXXX PRCV_DB_all_free %d\n",PRCV_GRP.rNr);


  // free all points
  for(i1=0; i1<PRCV_GRP.rNr; ++i1) {
    cv_act = MEMTAB__ (&PRCV_GRP, i1);
    if(!cv_act->npt) continue;
      // printf(" DB_free-points %d\n",i1);
    free ((void*)cv_act->npt);
    cv_act->npt = NULL;
  }

  // free all curves
  if(mode) MEMTAB_CLEAR (&PRCV_GRP);
  else     MemTab_free (&PRCV_GRP);

  return 0;

}



//================================================================
  int PRCV_DB_free (CurvPrcv *prc) {
//================================================================
// PRCV_DB_free          free pta-group
// do not free spcTyp= 5 (PRCV0)

  int       i1;
  CurvPrcv  *cv_act;


  // printf("XXXXXXX PRCV_DB_free %d\n",prc->spcTyp);


  // if(prc->spcTyp < 2) {
  if(MEM_MUST_FREE(prc->spcTyp)) {
    if(prc->npt)  free (prc->npt);
    prc->npt    = NULL;
    prc->siz    = 0;
    prc->ptNr   = 0;
  }


  return 0;

}


//================================================================
  int PRCV_DB_dump (char *s1) {
//================================================================
// PRCV_DB_dump

  int       i1, i2;
  CurvPrcv  *cv_act;
  Point     *pta;
  double    *par;
  long      *ipt;



  printf("============================================ PRCV_DB_dump %s\n",s1);

  DEB_dump_obj__ (Typ_MemTab, &PRCV_GRP, "PRCV_GRP");


  // loop tru PRCV_GRP.grp and dump pta-groups
  for(i1=0; i1<PRCV_GRP.rNr; ++i1) {
    printf("-------- PRCV_GRP[%d] npt=%p\n",i1,PRCV_GRP.data[i1].npt);
    cv_act = MEMTAB__ (&PRCV_GRP, i1);
    if(!cv_act->npt) continue;
    printf(" grp[%d] typ=%d dbi=%ld ptNr=%d siz=%d\n", i1,
           cv_act->typ, cv_act->dbi, cv_act->ptNr, cv_act->siz);
    DEB_dump_obj__ (Typ_PRCV, cv_act, "PRCV_DB_dump");

    pta = cv_act->npt;
    par = cv_act->npar;
    ipt = cv_act->nipt;
    for(i2=0; i2<cv_act->ptNr; ++i2) {
      DEB_dump_txt ("%5d    %9.3f,%9.3f,%9.3f      %9.3f      %3ld", i2,
                    pta[i2].x, pta[i2].y, pta[i2].z, par[i2], ipt[i2]);
    }
  }

  printf("========================================= ex-PRCV_DB_dump %s\n",s1);

  return 0;

}

//================================================================
  int PRCV_DB_fwrite (char *mnam) {
//================================================================
// 
// see DB_save__

  int       i1, i2;
  char      fnam[256], s1[256];
  CurvPrcv  *cv_act;
  FILE      *fp1;


  // printf("PRCV_DB_fwrite |%s|\n",mnam);
  // printf(" PRCV_GRP.rNr=%d\n",PRCV_GRP.rNr);
  // PRCV_DB_dump ("PRCV_DB_fwrite");


  strcpy(s1, mnam);
  // change all '/' of mNam into '_' - else no correct filename possible
  UTX_safeName (s1, 2);

  sprintf(fnam, "%sPRCV__%s.dat",OS_get_tmp_dir(),s1);
    // printf("PRCV_DB_fwrite-fnam |%s|\n",fnam);


  if((fp1=fopen(fnam,"wb")) == NULL) {
    TX_Print("DB_save__ E001-%s\n",fnam);
    return -1;
  }

  // write nr of groups
  fwrite(&PRCV_GRP, sizeof(MemTab), 1, fp1);


  //----------------------------------------------------------------
  // loop tru PRCV_GRP.grp and dump pta-groups
  for(i1=0; i1<PRCV_GRP.rNr; ++i1) {
    cv_act = MEMTAB__ (&PRCV_GRP, i1);
    if(!cv_act->npt) continue;

      // printf("--------- PRCV_GRP[%d]\n", i1);
      // DEB_dump_obj__ (Typ_PRCV, cv_act, "DB_fwrite-prcv");

    fwrite(cv_act, sizeof(CurvPrcv), 1, fp1);

    // save pointTable
    fwrite(cv_act->npt,  sizeof(Point),  cv_act->ptNr, fp1);
    fwrite(cv_act->npar, sizeof(double), cv_act->ptNr, fp1);
    fwrite(cv_act->nipt, sizeof(long),   cv_act->ptNr, fp1);
  }


  //----------------------------------------------------------------
  fclose(fp1);

    // printf("ex-PRCV_DB_fwrite\n");

  return 0;

}


//================================================================
  int PRCV_DB_fread (char *mnam) {
//================================================================
// PRCV_DB_fread                   read PRCV-DB from file
// see DB_load__

  int       i1, grpNr, rNr;
  long      l1;
  char      fnam[256], s1[256];
  CurvPrcv  actCv;
  FILE      *fp1;

  // printf("PRCV_DB_fread |%s|\n",mnam);


  strcpy(s1, mnam);
  // change all '/' of mNam into '_' - else no correct filename possible
  UTX_safeName (s1, 2);

  sprintf(fnam, "%sPRCV__%s.dat",OS_get_tmp_dir(),s1);
    // printf("PRCV_DB_fread-fnam |%s|\n",fnam);


  if((fp1=fopen(fnam,"rb")) == NULL) {
    TX_Print("DB_load__ E001 |%s|\n",fnam);
    return -1;
  }

  fread(&PRCV_GRP, sizeof(MemTab), 1, fp1);
    // DEB_dump_obj__ (Typ_MemTab, &PRCV_GRP, "DB_fread-PRCV_GRP-1");

  // get space for PRCV_GRP
  l1 = sizeof(CurvPrcv) * PRCV_GRP.rMax;
  PRCV_GRP.data = (void*)malloc (l1);
  if(!PRCV_GRP.data) { TX_Error("PRCV_DB_fread EOM1"); return -1; }


  //----------------------------------------------------------------
  // loop tru PRCV_GRP.grp and dump pta-groups
  rNr = PRCV_GRP.rNr;
  PRCV_GRP.rNr = 0;
  for(i1=0; i1<rNr; ++i1) {
      printf("--------- PRCV_GRP[%d]\n", i1);

    fread(&actCv, sizeof(CurvPrcv), 1, fp1);
      // DEB_dump_obj__ (Typ_PRCV, &actCv, "DB_fread-prcv");

    // get space for points+auxdat; see 
    l1 = (sizeof(Point) + sizeof(double) + sizeof(long)) * actCv.siz;
    actCv.npt = (void*)malloc (l1);
    if(!actCv.npt) { TX_Error("PRCV_DB_fread EOM2"); return -1; }

    actCv.npar = (void*)(actCv.npt + actCv.siz);
    actCv.nipt = (void*)(actCv.npar + actCv.siz);

    // read pointTable
    fread(actCv.npt,  sizeof(Point),  actCv.ptNr, fp1);
    fread(actCv.npar, sizeof(double), actCv.ptNr, fp1);
    fread(actCv.nipt, sizeof(long),   actCv.ptNr, fp1);

    // save actCv
    MemTab_add (&PRCV_GRP, &l1, &actCv, 1, 0);
  }


  //----------------------------------------------------------------
  fclose(fp1);

    // TESTBLOCK
    // printf("ex-PRCV_DB_fread\n");
    // PRCV_DB_dump ("ex-PRCV_DB_fread");
    // END TESTBLOCK


  return 0;

}


//================================================================
  int PRCV_DB_test1 (MemTab(CurvPrcv) *mtbPrcv) {
//================================================================
 
  *mtbPrcv = PRCV_GRP;

  return 0;

}


// EOF
