// ../ut/ut_prcv_f.c                 RF                      2019-05-03
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
2019-05-03  extracted from file ../ut/ut_prcv.c

-----------------------------------------------------
*/
/*!
\file  ../ut/ut_prcv_f.c
\brief PRCV database in memspc
\code
=====================================================
List_functions_start:

PRCV_DB_init        compat.
PRCV_DB_load        read PRCV from file "[M<subModelNr>]S<curve#>.odat"
PRCV_DB_save        write cvp into file "[M<subModelNr>]S<curve#>.odat"

PRCV_DB_spc_add     add space for <ptNr> points
PRCV_DB_free        free one CurvPrcv
PRCV_DB_all_free    clean all PRCV's of last run

PRCV_rdf__          INTERNAL  read PRCV from file "[M<subModelNr>]S<curve#>.odat"
PRCV_fsiz__         INTERNAL  get filesize
PRCV_DB_spc_ini     INTERNAL  init memspc for CurvPrcv
PRCV_fnam__         INTERNAL  get filename

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
#include "../ut/ut_os.h"               // OS_ ..
#include "../xa/xa_msg.h"              // MSG_* ERR_*
#include "../xa/xa_mem.h"              // memspc51, mem_cbuf1



//===========================================================================
// EXTERNALS:
// from ../xa/xa.c:
extern int       WC_modact_ind;         // -1=primary Model is active;
                                        // else subModel is being created

//===========================================================================
// LOCALS:  
    
#define PRCV0_INC_PTNR  256

//================================================================
int PRCV_DB_init () {}           // nothing to do ..
//================================================================


//================================================================
  int PRCV_DB_spc_add (CurvPrcv *prc, int ptNr) {
//================================================================
/// \code
/// PRCV_DB_spc_add     add space for <ptNr> points
/// not enough memspc. malloc-realloc; heap-Error; empty-malloc.
/// Input:
///   ptNr      total size in nr of points
/// Retcodes:     0=OK, -1=Error-heap; -2=EOM
/// \endcode


  printf("PRCV_DB_spc_add %d siz=%d\n",ptNr,prc->siz);
  // DEB_dump_obj__ (Typ_PRCV, prc, "  prc ");
  // PRCV_dump__ (2, prc, "  prc-in");

  if(ptNr < 1 ) return 0;


  if(prc->spcTyp == 7) {
    // empty; malloc.
    prc->npt    = (Point*)malloc(sizeof(Point) * ptNr);
    prc->npar   = (double*)malloc(sizeof(double) * ptNr);
    prc->nipt   = (long*)malloc(sizeof(long) * ptNr);
    prc->siz    = ptNr;
    prc->spcTyp = 5;  // 0=malloced-must-free; 1=heapSpc
    goto L_load;
  }


  if(prc->spcTyp == 5) {
    // 0=malloc-realloc;
    prc->npt  = (Point*)realloc(prc->npt, sizeof(Point) * ptNr);
    prc->npar = (double*)realloc(prc->npar, sizeof(double) * ptNr);
    prc->nipt = (long*)realloc(prc->nipt, sizeof(long) * ptNr);
    prc->siz  = ptNr;
    goto L_load;
  }

  // if(prc->spcTyp == 1) {
    // // heap-Error     See PRCV_DB_spc_ini.
    // TX_Error ("PRCV_DB_spc_add EOM-1");
    // return -1;
  // }

  TX_Error ("PRCV_rdf__ E1");
  return -1;


  //----------------------------------------------------------------
  L_load:
  if(!prc->npt) goto L_err1;
  if(!prc->npar) goto L_err1;
  if(!prc->nipt) goto L_err1;

    // PRCV_dump__ (2, prc, "ex-PRCV_DB_spc_add");

  return 0;

  L_err1:
    TX_Error("PRCV_DB_spc_add EOM");
    return -1;

}


//================================================================
  int PRCV_DB_load (CurvPrcv *prc) {
//================================================================
 
  int     irc, siz_mspc, ptNr, ii;
  void    *mspc;


  printf("PRCV_DB_load siz=%d spcTyp=%d\n",prc->siz,prc->spcTyp);
  if((prc->spcTyp < 0)||(prc->spcTyp > 7)) AP_debug__  ("PRCV_DB_load-D1");


  // if(prc->siz > 100) goto L_load;

  // get size of file ccv1.typ,dbi
  siz_mspc = PRCV_fsiz__ (prc);     // printf("ex-PRCV_fsiz__ %d\n",ii);

  // compute nr of points necessary
  ii = sizeof(Point) + sizeof(double) + sizeof(long);
  ptNr = siz_mspc / ii;


  // get memspc
  if(prc->spcTyp == 7) {
    // get temp-memspc for prc2.
    // mspc = (void*) MEM_alloc_tmp (siz_mspc);
    mspc = (void*) MEM_alloc_tmp (siz_mspc);
    if(!mspc) {TX_Error("PRCV_DB_load EOM"); return -1;}

    // set for ptNr points
    PRCV_DB_spc_ini (prc, -1, mspc, siz_mspc);

  } else if((prc->spcTyp < 1)||(prc->spcTyp > 4)) {

    if(ptNr > prc->siz) {
      irc = PRCV_DB_spc_add (prc, ptNr);
      if(irc < 0) return -1;
    }

  } else {
    TX_Error("PRCV_DB_load E1");
    return -1;

  }

  // load prc2 = basecurve
  L_load:
  return PRCV_rdf__ (prc);

}


//================================================================
  int PRCV_DB_save (CurvPrcv *prc) {
//================================================================
// write cvp into file "[M<subModelNr>]S<curve#>.odat"
// ofid   (objectfile-id)   subModelNr [M<subModelNr>]S<curve#>.odat
//  see APED_oid_dbo__


  int       rNr;
  char      ofid[128];
  FILE      *fp1;



  // printf("PRCV_DB_save M%d %d %ld\n",prc->mdli,prc->typ,prc->dbi);
  // PRCV_dump__ (2, prc, "  PRCV_DB_save");


  PRCV_fnam__ (ofid, prc);
    // printf(" PRCV_DB_save-ofid=|%s|\n",ofid);


  if((fp1=fopen(ofid,"wb")) == NULL) {
    TX_Print("PRCV_DB_save E001 |%s|",ofid);
    return -1;
  }


  rNr = prc->ptNr;

  fwrite(prc, sizeof(CurvPrcv), 1, fp1);

  fwrite(prc->npt, sizeof(Point), rNr, fp1);
  fwrite(prc->npar, sizeof(double), rNr, fp1);
  fwrite(prc->nipt, sizeof(long), rNr, fp1);

  fclose(fp1);


    // printf("ex PRCV_DB_save ptNr=%d\n",rNr);
    // PRCV_dump__ (2, prc, "ex PRCV_DB_save");


  return 0;

}


//================================================================
  int PRCV_fnam__ (char *fno, CurvPrcv *prc) {
//================================================================
// PRCV_fnam__            get filename
// file "[M<subModelNr>]S<curve#>.odat"

  char   cTyp;

  cTyp = AP_typChar_typ (prc->typ);

  // printf("PRCV_fnam__ cTyp=|%c| dbi=%ld mdli=%d\n",cTyp,prc->dbi,prc->mdli);


  if(prc->mdli >= 0) {           // 0-n = sind in Submodel; -1=main
    sprintf(fno, "%sM%d_%c%ld.odat", OS_get_tmp_dir(),prc->mdli,cTyp,prc->dbi);
  } else {
    sprintf(fno, "%s%c%ld.odat", OS_get_tmp_dir(),cTyp,prc->dbi);
  }

    // printf(" ex-PRCV_fnam__ |%s|\n",fno);

  return 0;

}


//================================================================
  int PRCV_fsiz__ (CurvPrcv *prc) {
//================================================================
// PRCV_fsiz__             get filesize

  char   fn1[128];

  PRCV_fnam__ (fn1, prc);

  return (512 + OS_FilSiz (fn1));

}


//================================================================
  int PRCV_rdf__ (CurvPrcv *prc) {
//================================================================
/// \code
/// read PRCV from file "[M<subModelNr>]S<curve#>.odat"
/// Memspace for points, params is mallocd, if not provided with PRCV_DB_spc_ini
///   If Malloced: MUST BE FREED WITH PRCV_DB_free !
/// Input:
///   prc->typ
///   prc->dbi
///   prc->mdli
/// Output:
///   retCod      0 OK; MUST PRCV_DB_free()
///              -1 file not found
///
/// ofid   (objectfile-id)   subModelNr [M<subModelNr>]S<curve#>.odat
///  see APED_oid_dbo__
/// \endcode


  int      irc, tabSiz, ptNr, inSiz;
  char     ofid[128], inTmp;
  FILE     *fp1;
  CurvPrcv fPrc;



  // DEB_dump_obj__ (Typ_PRCV, prc, "PRCV_rdf__: ");


  // if(!prc->fTmp) { TX_Error("PRCV_rdf__ E001"); return -1; }

    // printf("PRCV_rdf__ M%d %ld\n",prc->mdli,prc->dbi);


  // inSiz = prc->siz;  // keep size
  // inTmp = prc->fTmp;


  PRCV_fnam__ (ofid, prc);
    // printf(" ofid=|%s|\n",ofid);


  if((fp1=fopen(ofid,"rb")) == NULL) {
    printf("**** PRCV_rdf__ I001 |%s|\n",ofid);
    return -1;
  }


  fread(&fPrc, sizeof(CurvPrcv), 1, fp1);
    // DEB_dump_obj__ (Typ_PRCV, &fPrc, "_rdf_");

  ptNr = fPrc.ptNr;
  tabSiz = ptNr + 10;

  if(tabSiz < prc->siz) goto L_load;


  //----------------------------------------------------------------
  // not enough memspc. malloc-realloc; heap-Error; empty-malloc.
  irc = PRCV_DB_spc_add (prc, tabSiz + PRCV0_INC_PTNR);
  if(irc < 0) {
    printf("**** PRCV_rdf__ EOM\n");
    return -1;
  }


  //----------------------------------------------------------------
  L_load:

  prc->ptNr = ptNr;

  fread(prc->npt, sizeof(Point), ptNr, fp1);
  fread(prc->npar, sizeof(double), ptNr, fp1);
  fread(prc->nipt, sizeof(long), ptNr, fp1);

  fclose(fp1);


    // DEB_dump_obj__ (Typ_PRCV, prc, "ex PRCV_rdf__: ");
    // PRCV_dump__ (2, prc, "ex PRCV_rdf__");
    // printf("ex PRCV_rdf__ ptNr=%d\n",prc->ptNr);

  return 0;

}


//=====================================================================
  int PRCV_DB_spc_ini (CurvPrcv *prc, int ptNr, char* mSpc, int mSiz) {
//=====================================================================
// PRCV_DB_spc_ini             init memspc for CurvPrcv
// Input:
//   ptNr     -1   use complete space
//            >0   get only this size
//   mSpc     memspc to be used
//   mSiz     size of mSpc in chars

// Example using temp. Memspc:
//  Memspc *tbuf1 int siz_mspc1; void *mspc1;
//  siz_mspc1 = UME_ck_free (tbuf1);
//  mspc1 = UME_reserve (tbuf1, siz_mspc1);
//  PRCV_DB_spc_ini (&prc1, -1, mspc1, siz_mspc1);

// Example using tempSpace:
//  int siz_mspc1; void *mspc1;
//  siz_mspc1 = 200000;  // byte
//  mspc1 = (void*) MEM_alloc_tmp (siz_mspc1);
//  if(!mspc1) {TX_Error("fNam EOM"); return -1;}
//  PRCV_DB_spc_ini (&prc1, -1, mspc1, siz_mspc1);


  int   ii;
  char  *cp1;


  // printf("PRCV_DB_spc_ini %d\n",ptNr);


  ii = sizeof(Point) + sizeof(double) + sizeof(long);

  if(ptNr < 0) {
    // compute size
    ptNr = mSiz / ii;

  } else {
    // check size
    if((ptNr * ii) > mSiz) {
      printf("**** PRCV_DB_spc_ini EOM\n");
      return -1;
    }
  }

  // intialize prc
  // *prc = UT3D_PRCV_NUL;   // _PRCV_NUL


  cp1 = mSpc;
  prc->npt = (Point*)cp1;

  cp1 += (sizeof(Point) * ptNr);
  prc->npar = (double*)cp1;

  cp1 += (sizeof(double) * ptNr);
  prc->nipt = (long*)cp1;

  prc->siz  = ptNr;   // size
  prc->ptNr = 0;

  // prc->fTmp = 1;      // rem. 2019-05-06

  return 0;

}



//================================================================
  int PRCV_DB_all_free () {
//================================================================
// PRCV_DB_all_free    clean all PRCV's of last run

  char  cbuf[256];

  printf("PRCV_DB_all_free \n");

  // remove all PRCV-files
  sprintf(cbuf, "%s*.odat",OS_get_tmp_dir());
  OS_file_delGrp (cbuf);

  return 0;

}


//================================================================
  int PRCV_DB_free (CurvPrcv *prc) {
//================================================================
// PRCV_DB_free        free one CurvPrcv

  // DEB_dump_obj__ (Typ_PRCV, prc, "PRCV_DB_free");
  // PRCV_dump__ (2, prc, "PRCV_DB_free");


  if(prc->spcTyp < 2) {

    if(prc->npt)  free (prc->npt);
    if(prc->npar)  free (prc->npar);
    if(prc->nipt)  free (prc->nipt);
    prc->siz    = 0;
    prc->ptNr   = 0;
    prc->spcTyp = 5;
  }

  return 0;

}


// EOF
