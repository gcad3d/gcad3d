// ut_prcv.c                   RF                     2003-07-01
/*
 *
 * Copyright (C) 2016 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
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
- get points only of PRCV (PRCV_npt_dbo__):
  - open;  keep fp source-global and open; (omits re-open)
  - read nr-of-points; return; keep position 1
  - read Points; keep position 2
  - read parameterd & DB-indexTab; keep position 3


- am Beginn jeder Datei einen Record mit der Punkteanzahl und der Box.
Eigene struct dafür ?
- Eine PRCV_rdf_init - Funktion, die macht open und read 1. record;
- mit dieser Punkteanzahl kann man dann den memSpacebedarf rechnen
- wenn keine PRCV-Datei existiert: mit UTO_ptNr_obj
  (dzt: PRCV_get_dbo_add_tc & PRCV_set_obj_dbi via siz_mspc* MEM_alloc_tmp)


- welche Func gives polygon from binaryObj ?
  UT3D_pta_dbo UT3D_npt_obj UT3D_npt_ox__ UT3D_pta_ox_lim
  UT3D_pta_sus > ?

- nachbauen - als UT3D_npt_prc_dbo

- welche Func makes display-dbo ?
   APT_Draw__ APT_DrawCirc / APT_DrawCurv / APT_DrawSur / APT_DrawSol
APT_DrawCirc GR_DrawCirc UT2D_npt_ci
APT_DrawCurv
  GR_DrawCvBSp UT3D_pta_bsp
  GR_DrawCvRBSp UT3D_cv_rbsp
  GR_DrawCvPol UT3D_pta_plg
  GR_DrawCvEll UT3D_npt_ell
  GR_DrawCvClot UT3D_npt_clot
  GR_DrawCvCCV UT3D_pta_ox_lim
           


-----------------------------------------------------
Modifications:
2016-10-19 Neu.  RF.

-----------------------------------------------------
*/
#ifdef globTag
void PRCV(){}
#endif
/*!
\file  ../ut/ut_prcv.c
\brief polygonal_representation_of_curve  (struct CurvPrcv)
\code

=====================================================
List_functions_start:

PRCV_set_dbo__      create PRCV from bin.obj
PRCV_set_obj_dbi    create PRCV for bin.obj (must have DB-index)
PRCV_set_basCv          create PRCV for basicCurve
PRCV_set_seg        add curve from vs to ve to PRCV
PRCV_set_find       get previous-parameter and next-parameter

PRCV_npt_dbo__      get polygon for DB-obj
PRCV_npt_trmCv      get polygon of trimmedCurve
PRCV_get_dbo__      get PRCV for DB-obj (typ/dbi)
PRCV_get_dbo_add_tc add trimmed-curve ccv1 to prc1
PRCV_get_tc_add_pa  add point to prc1
PRCV_get_tc_add_prc add <ptNr> points from prc2 starting at point <ips> to prc1
PRCV_get_tc_find    find index in from parameter or dbi in PRCV
PRCV_memspc_ini     init memspc for CurvPrcv
PRCV_memspc_add     add space
PRCV_free__         free PRCV0 (new-model)

PRCV_wrf__          write cvp into file "[M<subModelNr>]S<curve#>.odat"
PRCV_rdf__          read PRCV from file "[M<subModelNr>]S<curve#>.odat"
PRCV_rdf_free       free CurvPrcv
PRCV_fsiz__         get filesize
PRCV_fnam__         get filename

PRCV_dump__
PRCV_dump_1
PRCV_dump_dbo
PRCV_test__
PRCV_test_set
PRCV_test_get       test get PRCV for dbo
PRCV_test_disp

  // PRCV_insert_par
  // PRCV_insert__       Create PRC, add 1 or 2 intermediate points
  // PRCV_ccv__        get PRCV from trimmed-curve

List_functions_end:
=====================================================

  
\endcode *//*----------------------------------------


see also: ../ut/ut_ccv.c   trimmed-curve functions


----------------------------------------
Every trimmed-curve (CurvCCV) refers to a basic-curve (typ,dbi).
  More than one trimmed-curves can point to the same basic-curve.


Every basic-curve has a PRCV (polygonal_representation_of_curve)
  The PRCV (struct CurvPrcv) is in a file;
    filename is tmp_dir/[M<subModelNr>]S<surf#>.odat

  Struct CurvPrcv is a polygonal_representation_of_curve -
    - a list of points (npt),
    - a list of the paramter-values of this points (npar), 
    - a list of the DB-indices of this points (nipt).
  

----------------------------------------
Memspc PRCV0 is used for getting the primary PRCV; realloc if too small;
  free at new-model.
PRCV0 is used for:
  PRCV_npt_dbo__     get polygon for DB-obj
  PRCV_set_obj_dbi   create PRCV for DB-obj

*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




#include "../ut/ut_geo.h"
#include "../ut/ut_geo_const.h"           // UT3D_CCV_NUL UT3D_PRCV_NUL
#include "../ut/ut_memTab.h"              // MemTab
#include "../ut/ut_os.h"                  // OS_ ..
#include "../db/ut_DB.h"                  // DB_GetObjDat

#include "../xa/xa_mem.h"                 // memspc*


//===========================================================================
// EXTERNALS:
// from ../xa/xa.c:
extern int       WC_modact_ind;         // -1=primary Model is active;
                                        // else subModel is being created

//===========================================================================
// LOCALS:
static CurvPrcv PRCV0 = _PRCV_NUL;

#define PRCV0_INC_PTNR  256



//================================================================
  int PRCV_free__ () {
//================================================================
/// \code
/// free PRCV0
/// used only when new model is activated
/// \endcode


  UT3D_stru_dump (Typ_PRCV, &PRCV0, " PRCV_free__ ");


  if(!PRCV0.fTmp) {

    if(PRCV0.npt)   free (PRCV0.npt);
    if(PRCV0.npar)  free (PRCV0.npar);
    if(PRCV0.nipt)  free (PRCV0.nipt);
    PRCV0.siz  = 0;
    PRCV0.ptNr = 0;
    PRCV0.fTmp = 2;
  }

  return 0;

}
 

//===========================================================================
  int PRCV_npt_dbo__ (Point **pta, int *ptNr, int typ, long dbi, int mdli) {
//===========================================================================
/// \code
/// PRCV_npt_dbo__      get polygon for DB-obj
/// Output:
///   pta        returns a pointer to <ptNr> points; do not free.
/// \endcode

  int    irc;


  // printf("PRCV_npt_dbo__ typ=%d dbi=%ld\n",typ,dbi);


  // get PRCV from DB-obj (typ/dbi)
  PRCV0.typ  = AP_typ_2_bastyp (typ);
  PRCV0.dbi  = dbi;
  PRCV0.mdli = mdli;
  PRCV0.ptNr = 0;

  irc = PRCV_get_dbo__ (&PRCV0);
  if(irc < 0) return -1;

    // TESTBLOCK
    // PRCV_test_disp (&PRCV0);
    // PRCV_dump__ (2, &PRCV0, "PRCV_npt_dbo__");
    // END TESTBLOCK

  // PRCV_rdf_free (&prc1);

  *pta = PRCV0.npt;
  *ptNr = PRCV0.ptNr;

  return 0;

}


//================================================================
  int PRCV_get_dbo__ (CurvPrcv *prc1) {
//================================================================
/// \code
/// get PRCV for DB-obj (typ/dbi)
///   Memspc for point, parameters must have been provided with PRCV_memspc_ini
///   MUST free prc1 with PRCV_rdf_free !
/// Input:
///   prc1     typ,dbi,mdli must be set; prc1 = PRCV0
/// Output:
///   prc1     ptNr,npt,npar,nipt
/// \endcode




  int      irc, i1, form, oNr;
  void     *obj;
  Point    pta[2];
  Line     ln1;
  CurvCCV  *ccv1;


  // printf("\n========================= PRCV_get_dbo__ typ=%d dbi=%ld siz=%d\n",
         // prc1->typ,prc1->dbi,prc1->siz);
  // UT3D_stru_dump (Typ_PRCV, prc1, " PRCV_get_dbo__-prc1 ");


  // test for CCV (must resolv)
  if(prc1->typ != Typ_CV) goto L_get_basCv;

  form = DB_GetObjDat (&obj, &oNr, prc1->typ, prc1->dbi);
    // printf(" form=%d oNr=%d\n",form,oNr);
  if(form != Typ_CVTRM) goto L_get_basCv;



  //================================================================
  // resolv (group of) trimmed-curve(s)
  for(i1=0; i1<oNr; ++i1) {

    // ccv1 = next trimmedCurve
    ccv1 = &((CurvCCV*)obj)[i1];
       // printf("--------------------- CCV [%d] \n",i1);
       // UT3D_stru_dump (Typ_CVTRM, ccv1, " ccv[%d]: ",i1);

    //----------------------------------------------------------------
    // add line
    if(ccv1->typ != Typ_LN) goto L_add_trCv;
    // get line from CCV and add it to output
    PRCV_get_dbo_add_ln (prc1, ccv1);
    goto L_cont;

    //----------------------------------------------------------------
    L_add_trCv:    // add trimmed curve
      // get PRCV of ccv1 from par ps to par pe to prc1
      irc = PRCV_get_dbo_add_tc (prc1, ccv1);
      if(irc < 0) return -1;

    L_cont:
      continue;
      // TESTBLOCK
      // PRCV_dump__ (2, prc1, "nach ccv");
      // END TESTBLOCK
  }

  goto L_exit;



  //================================================================
  // resolv basecurve - not yet trimmed; Circ, polygon, Ellipse, ..
  L_get_basCv:
    // get PRCV of basecurve;
    irc = PRCV_rdf__ (prc1);
    if(irc < 0) {
      TX_Error ("PRCV_get_dbo__ E1-%d-%ld",prc1->typ,prc1->dbi);
      return -1;
    }


  //----------------------------------------------------------------
  L_exit:

    // TESTBLOCK
    // PRCV_dump__ (2, prc1, "ex PRCV_get_dbo__");
    // END TESTBLOCK

  return 0;

}


//================================================================
  int PRCV_get_tc_add_pa (CurvPrcv *prc1, Point *pta, int ptNr) {
//================================================================
// add points to prc1


  int   ips, irc, siz;


  // printf("PRCV_get_tc_add_pa %d\n",ptNr);
  // for(irc=0;irc<ptNr;++irc)UT3D_stru_dump(Typ_PT, &pta[irc], "pt[%d]:",irc);
  // printf(" tol=%f\n",UT_TOL_cv);



  // test space
  siz = prc1->ptNr + ptNr + 10;
  if(siz > prc1->siz) {
    // get space for <ptNr> more points
    irc = PRCV_memspc_add (prc1, siz + PRCV0_INC_PTNR);
    if(irc < 0) return irc;
  }



  ips = 0;
  if(prc1->ptNr > 0) {
    if(UT3D_comp2pt(&prc1->npt[prc1->ptNr - 1], &pta[ips], UT_TOL_cv)) {
      // identical; skip first point
      --ptNr;
      ++ips;
    } else printf("**** PRCV_get_tc_add_pa I002\n");
  }


  // copy
  memcpy (&prc1->npt[prc1->ptNr], &pta[ips], sizeof(Point) * ptNr);


  prc1->ptNr += ptNr;

  // prc1->npar[prc1->ptNr] = -1.;
  // prc1->nipt[prc1->ptNr] = ip1;


  return 0;

}


//================================================================
  int PRCV_get_dbo_add_ln (CurvPrcv *prc1, CurvCCV *cvi) {
//================================================================
// get line from CCV and add it to output

  int      otyp;
  Point    pta[2];
  Line     ln1;


  // get line from CCV and add it to output
  otyp = Typ_LN;
  UTO_cv_cvtrm (&otyp, &ln1, NULL, cvi);
    // UT3D_stru_dump (Typ_LN, &ln1, "PRCV_get_dbo_add_ln");

  pta[0] = ln1.p1;
  pta[1] = ln1.p2;

  return PRCV_get_tc_add_pa (prc1, pta, 2);

}


//================================================================
  int PRCV_get_dbo_add_tc (CurvPrcv *prc1, CurvCCV *cvi) {
//================================================================
// add trimmed-curve cvi to prc1
//
// read basecurve of ccv1 into prc2;
// find limited segment and add to prc1.

      
  int      irc, ips, ipe, ptNr, irev, siz_mspc2;
  double   v0x, v1x;
  void     *cvBas, *mspc2;
  CurvCCV  ccv1;
  CurvPrcv prc2;

  // printf("----------------PRCV_get_dbo_add_tc ------------------ \n");
  // UT3D_stru_dump (Typ_CVTRM, cvi, " PRCV_get_dbo_add_tc-cvi ");
  // UT3D_stru_dump (Typ_PRCV, prc1, " PRCV_get_dbo_add_tc-prc1 ");
  // PRCV_dump__ (2, prc1, " prc1");


  // if basic-curve of ccv1 is also trimmedCurve: modify in ccv1:
  // - get type and dbi of basicCurve (ccv1.typ and ccv1.dbi)
  // - get v0 and v1 of trimmedCurve on basicCurve (ccv1.v0 and ccv1.v1)
  irc = CVTRM__basCv__ (&ccv1, &cvBas, cvi);
  if(irc < 0) return -1;
    // UT3D_stru_dump (Typ_CVTRM, &ccv1, "     _get_dbo_add_tc-ccv1 ");
    

  // if basic-curve == line: get line and add to prc1.
  if(ccv1.typ == Typ_LN) {
    // add to prc1, no prc2 necessary
    PRCV_get_dbo_add_ln (prc1, &ccv1);
    goto L_done;
  }


  // get prc2 = PRCV for basecurve of ccv1
  prc2.typ  = ccv1.typ;
  prc2.dbi  = ccv1.dbi;
  prc2.mdli = WC_modact_ind;


  // get size of file ccv1.typ,dbi
  siz_mspc2 = PRCV_fsiz__ (&prc2);     // printf("ex-PRCV_fsiz__ %d\n",ii);


  // get memspc for prc2.
  mspc2 = (void*) MEM_alloc_tmp (siz_mspc2);
  if(!mspc2) {TX_Error("PRCV_get_dbo_add_tc EOM"); return -1;}
  PRCV_memspc_ini (&prc2, -1, mspc2, siz_mspc2);


  // load prc2 = basecurve
  irc = PRCV_rdf__ (&prc2);
  if(irc < 0) {
    // not enough memspc for PRCV_rdf__ .. ??
    TX_Error ("PRCV_get_dbo_add_tc E1-%d-%ld",ccv1.typ,ccv1.dbi);
    return -1;
  }
    // PRCV_dump__ (2, &prc2);


  // find startIndex ips in prc2
  ips = PRCV_get_tc_find (ccv1.v0, ccv1.ip0, &prc2);
  if(ips < 0) return -1;

  
  // find endIndex in prc2
  ipe = PRCV_get_tc_find (ccv1.v1, ccv1.ip1, &prc2);
  if(ipe < 0) return -1;


  // fix Bspl-curves, not closed: if v1 < v0 then dir=1
  // if not closed: if ips > ipe then revers ...
  if(ccv1.clo) {
    if((ccv1.typ == Typ_CVBSP)   ||
       (ccv1.typ == Typ_CVRBSP)  ||
       (ccv1.typ == Typ_CVELL))     {
      if(ips > ipe) ccv1.dir = 1;
    }
  }


    // TESTBLOCK
    // printf(" PRCV_get_dbo_add_tc ips=%d ipe=%d dir=%d clo=%d\n",
            // ips,ipe,ccv1.dir,ccv1.clo);
    // PRCV_dump__ (2, prc1, "prc1 nach PRCV_rdf__");
    // END TESTBLOCK



  //================================================================
  // add prc2 from ips to ipe to prc1
  if(ccv1.dir) goto L_revers;

  //----------------------------------------------------------------
  // forward = normal
    // printf(" fwd\n");
  if(ipe > ips) {
    ptNr = ipe - ips + 1;
    // add <ptNr> points from prc2 starting at point <ips> to prc1
    irc = PRCV_get_tc_add_prc (prc1, &prc2, ptNr, ips, ccv1.dir);
    if(irc < 0) return -1;
    goto L_done;
  }


  // not closed: add from ips to ipe
  if(ccv1.clo) {
    // not closed; add from ips to ipe
    ptNr = ipe - ips + 1;
    irc = PRCV_get_tc_add_prc (prc1, &prc2, ptNr, ips, ccv1.dir);
    if(irc < 0) return -1;

  
  } else {
    // closed curve - forward - crossing ipEnd,ipStart:
    // add from ips to ipEnd
    ptNr = prc2.ptNr - ips;
    irc = PRCV_get_tc_add_prc (prc1, &prc2, ptNr, ips, ccv1.dir);
    if(irc < 0) return -1;

    // add from ipStart to ipe.
    ptNr = ipe + 1;
    irc = PRCV_get_tc_add_prc (prc1, &prc2, ptNr, 0, ccv1.dir);
    if(irc < 0) return -1;
  }


  goto L_done;




  //----------------------------------------------------------------
  L_revers:
    // printf(" L_revers:\n");
  // forward = normal
  if(ipe < ips) {
    // revers; add points from ipe -> ips
    ptNr = ips - ipe + 1;
    // add <ptNr> points from prc2 starting at point <ips> to prc1
    irc = PRCV_get_tc_add_prc (prc1, &prc2, ptNr, ipe, ccv1.dir);
    if(irc < 0) return -1;
    goto L_done;
  }


  // not closed: add from ips to ipe
  if(ccv1.clo) {
    // not closed; add from ips to ipe
    // revers; add points from ipe -> ips
    ptNr = ips - ipe + 1;
    // add <ptNr> points from prc2 starting at point <ips> to prc1
    irc = PRCV_get_tc_add_prc (prc1, &prc2, ptNr, ipe, ccv1.dir);
    if(irc < 0) return -1;


  } else {
    // closed curve - forward - crossing ipEnd,ipStart:
    // add from 0 to ips
    ptNr = ips + 1;
    irc = PRCV_get_tc_add_prc (prc1, &prc2, ptNr, 0, ccv1.dir);
    if(irc < 0) return -1;

    // add from ipe to ipEnd.
    ptNr = prc2.ptNr - ipe;
    irc = PRCV_get_tc_add_prc (prc1, &prc2, ptNr, ipe, ccv1.dir);
    if(irc < 0) return -1;
  }





  //----------------------------------------------------------------
  L_done:
  // free prc2
  // PRCV_rdf_free (&prc2);
    // printf("ex PRCV_get_dbo_add_tc\n");
    // if(cvi->dbi == 21L)
    // UT3D_stru_dump (Typ_PRCV, prc1, " ex-PRCV_get_dbo_add_tc-prc1 ");
    // PRCV_dump__ (2, prc1, " prc1");


  return 0;

}


//=========================================================================
  int PRCV_get_tc_add_prc (CurvPrcv *prc1, CurvPrcv *prc2,
                           int ptNr, int ips, int idir) {
//=========================================================================
// add <ptNr> points from prc2 starting at point <ips> to prc1
// first point may be double, test/skip if necessary.
// Input:
//   ptNr    nr of points to copy
//   ips     index of first point to copy from prc2
//   idir    0=forward, 1=reverse;
//           reverse: start with prc2->npt[ips + ptNr - 1], then --ips ...

  int     irc, ii, i1, i2, siz;


  // printf("PRCV_get_tc_add_prc ptNr=%d ips=%d idir=%d\n",ptNr,ips,idir);
  // UT3D_stru_dump (Typ_PRCV, prc1, "tc_add_prc-prc1");
  // UT3D_stru_dump (Typ_PRCV, prc2, "tc_add_prc-prc2");
  // PRCV_dump__ (2, prc2, " prc2");


  // if cv is closed - cv fwd, from 0.5 to 0.0 makes 0.5-1.; then 0.-0.
  if(ptNr < 2) return 0;


  // test space
  siz = prc1->ptNr + ptNr + 10;
  if(siz > prc1->siz) {
    // get space for <ptNr> more points
    irc = PRCV_memspc_add (prc1, siz + PRCV0_INC_PTNR);
    if(irc < 0) {   // return irc;
      printf("**** PRCV_get_tc_add_prc EOM-p1-%d\n",irc);
      return -1;
    }
  }


  //----------------------------------------------------------------
  if(idir) goto L_bwd;
    // fwd; test if last existing point is identical
    i1 = prc1->ptNr - 1;
    if(i1 >= 0) {
      if(UT3D_comp2pt(&prc1->npt[i1], &prc2->npt[ips], UT_TOL_cv)) {
        // identical; skip first point
        --ptNr;
        ++ips;
      } else {
        printf("**** PRCV_get_tc_add_prc I002\n");
        UT3D_stru_dump (Typ_PT, &prc1->npt[i1], "prc1[%d]",i1);
        UT3D_stru_dump (Typ_PT, &prc2->npt[ips], "prc2[%d]",ips);
        // return -1;
      }
    }
    // copy
    memcpy (&prc1->npt[prc1->ptNr], &prc2->npt[ips], sizeof(Point) * ptNr);
    goto L_exit;


  //----------------------------------------------------------------
  L_bwd:
    // revers
    i1 = prc1->ptNr;       // first output-index (in prc1)
    i2 = ips + ptNr - 1;   // first input-index (in prc2)
      // printf(" _add_prc-bwd-1 i1=%d i2=%d ptNr=%d\n",i1,i2,ptNr);

    // test first
    if(i1 > 0) {
        // UT3D_stru_dump (Typ_PT, &prc1->npt[i1 - 1], "prc1");
        // UT3D_stru_dump (Typ_PT, &prc2->npt[i2], "prc2");
      if(UT3D_comp2pt(&prc1->npt[i1 - 1], &prc2->npt[i2], UT_TOL_cv)) {
        // identical; skip first point
        --ptNr;
        --i2;
      }
    }
    // copy
      // printf(" _add_prc-bwd-2 i1=%d i2=%d ptNr=%d\n",i1,i2,ptNr);

    for(ii=0; ii<ptNr; ++ii) {
      memcpy (&prc1->npt[i1], &prc2->npt[i2], sizeof(Point));
      ++i1;
      --i2;
    }


  //----------------------------------------------------------------
  L_exit:

  prc1->ptNr += ptNr;


    // PRCV_dump__ (2, prc1, "nach PRCV_get_tc_add_prc");


  return 0;

}


//=====================================================================
  int PRCV_get_tc_find (double vx, long ipdb, CurvPrcv *prc) {
//=====================================================================
/// \code
/// PRCV_get_tc_find    find index from parameter or dbi in PRCV
/// Input:
///   vx      parameter
///   ipdb    dbi
/// Output:
///   retCod  index of point with parameter=vx or dbi=ipdb in prc
///           -1   not-found
/// \endcode

  int      ii, i1;
  double   vx1, vx2, tol = UT_TOL_min0;



  // printf("PRCV_get_tc_find vx=%lf ipdb=%ld\n",vx,ipdb);

  ii = -1;


  //----------------------------------------------------------------
  if(vx == UT_VAL_MAX) goto L_ck_ipt;

  // find parameter
  vx1 = vx - tol;
  vx2 = vx + tol;

  for(i1 = 0; i1 < prc->ptNr; ++i1) {

    // skip -undef- values
    if(prc->npar[i1] == UT_VAL_MAX) continue;

    if(prc->npar[i1] < vx1) continue;

    if(prc->npar[i1] < vx2) {
      ii = i1; 
      goto L_exit;  // ii found
    }
    goto L_exit;  // not found
  } 


  //----------------------------------------------------------------
  L_ck_ipt:
  if(ipdb) {
    // find pointIndex
    for(i1 = 0; i1 < prc->ptNr; ++i1) {
      if(ipdb != prc->nipt[i1]) continue;
      ii = i1; goto L_exit;
    }
  }


  //----------------------------------------------------------------
  L_exit:
    if(ii < 0) TX_Print("***** PRCV_get_tc_find E1 vx=%lf ipdb=%ld",vx,ipdb);

    // printf("ex PRCV_get_tc_find %d vx=%lf ipdb=%ld\n",ii,vx,ipdb);

  return ii;

}
 


//================================================================
  int PRCV_set_dbo__ (int typ, long dbi) {
//================================================================
// create PRCV for DB-obj (typ/dbi)
// Input:
//   typ,dbi  dbo baseCurve

  int      form, oNr;
  void     *obj;


  // printf("PRCV_set_dbo__ %d %ld\n",typ,dbi);


  form = DB_GetObjDat (&obj, &oNr, typ, dbi);
    // printf(" form=%d\n",form);


  return PRCV_set_obj_dbi (form, obj, oNr, typ, dbi);

}


//========================================================================
  int PRCV_set_obj_dbi (int form, void *obj, int oNr, int typ, long dbi) {
//========================================================================
/// \code
/// PRCV_set_obj_dbi     create PRCV for bin.obj (must have DB-index)
/// lines do not have PRCV-file
///   using memspc201
/// Input:
///   typ,dbi       dbo baseCurve
///   form,obj,oNr  bin.obj (from DB_GetObjDat())
/// \endcode

// does not write CCV. CCV is composed at runtime

 
  int      irc, ptNr, i1, i2, oTyp, siz, typBas, x0x, x1x, siz_mspc1;
  long     ip0s, ip0e, ip1s, ip1e, ipe, dbiBas;
  long     ipdb0s, ipdb0x,ipdb0e, ipdb1s, ipdb1x, ipdb1e;
  double   v0x, v1x, v0s, v0e, v1s, v1e;
  Point    *pa3, *ptx;
  char     cvCut[OBJ_SIZ_MAX];
  void     *cvBas, *mspc1;
  CurvCCV  *ccv1, ccv2;
  CurvPrcv prc1;


  // printf("======================= PRCV_set_obj_dbi typ=%d dbi=%ld\n",typ,dbi);


  if(typ == Typ_LN) return 1;

  if(!dbi) {
    TX_Error("PRCV_set_obj_dbi no-cvi;no-dbi.");
    return -1;
  }


  // form = DB_GetObjDat (&obj, &oNr, typ, dbi);
    // printf(" form=%d\n",form);


  //----------------------------------------------------------------
  if(form != Typ_CVTRM) goto L_basCv;

  // resolv CCV; 
  // CCV does not have own PRCV; decode its component-curves and add all
  // necessary trim-points into its basic-curves.
  for(i1=0; i1<oNr; ++i1) {

    // ccv1 = copy next trimmedCurve
    ccv1 = &((CurvCCV*)obj)[i1];
       // printf("--------------------- CCV [%d] \n",i1);
       // UT3D_stru_dump (Typ_CVTRM, ccv1, " ccv[%d]: ",i1);

    // Lines: direct (no PRCV-file)
    if(ccv1->typ == Typ_LN) continue;

    // if basic-curve of ccv1 is also trimmedCurve: modify in ccv1:
    // - get type and dbi of basicCurve (ccv1.typ and ccv1.dbi)
    // - get v0 and v1 of trimmedCurve on basicCurve (ccv1.v0 and ccv1.v1)
    CVTRM__basCv__ (&ccv2, &cvBas, ccv1);
    typBas = ccv2.typ;
    dbiBas = ccv2.dbi;
      // UT3D_stru_dump (typBas, cvBas, " cvBas");

    // Lines: direct (no PRCV-file)
    if(ccv2.typ == Typ_LN) continue;


    // get limiting parameters for curve
    v0x = ccv2.v0;
    v1x = ccv2.v1;
    ipdb0x = ccv2.ip0;
    ipdb1x = ccv2.ip1;
      // printf(" v0x=%lf v1x=%lf dir=%d\n",v0x,v1x,ccv2.dir);
      // printf(" ipdb0x=%ld ipdb1x=%ld\n",ipdb0x,ipdb1x);


    // get v0-parameter for trimmedCurve
    if(v0x == UT_VAL_MAX) {
      if(ipdb0x != 0) {
        // get point ptx from ipx;
        ptx = DB_get_PT (ipdb0x);
        // get parameter v0x from point
        UTO_par1_pt_pt_obj (&v0x, ptx, typBas, cvBas);
          // irc = UTO_parLim_get_cv (&v0, &v1, Typ_CVTRM, ccv1);
          // if(irc < 0) return -1;
      } else {
        TX_Error("PRCV_set_obj_dbi E1-%d %ld\n",typ,dbi);
        return -1;
      }
    }

    // get v1-parameter for trimmedCurve
    if(v1x == UT_VAL_MAX) {
      if(ipdb1x != 0) {
        // get point from ipx
        ptx = DB_get_PT (ipdb1x);
        // get parameter from point
        UTO_par1_pt_pt_obj (&v1x, ptx, typBas, cvBas);
          // irc = UTO_parLim_get_cv (&v0, &v1, Typ_CVTRM, ccv1);
          // if(irc < 0) return -1;
      } else {
        TX_Error("PRCV_set_obj_dbi E2-%d %ld\n",typ,dbi);
        return -1;
      }
    }
      // printf(" v0x=%lf v1x=%lf\n",v0x,v1x);
      // printf(" ipdb0x=%ld ipdb1x=%ld\n",ipdb0x,ipdb1x);


    // make (v0x < v1x)
    if(v0x > v1x) {
      MEM_swap_2db (&v0x, &v1x);
      MEM_swap_2lg (&ipdb0x, &ipdb1x);
        // printf(" mod1: v0x=%lf v1x=%lf\n",v0x,v1x);
    }


    // get PRCV of basecurve
    prc1.typ  = typBas;
    prc1.dbi  = dbiBas;
    prc1.mdli = WC_modact_ind;

    // get memSpace for basecurve
    // PRCV_memspc_ini (&prc1, -1, memspc201, sizeof(memspc201));
    siz_mspc1 = PRCV_fsiz__ (&prc1);
    mspc1 = (void*) MEM_alloc_tmp (siz_mspc1);
    if(!mspc1) {TX_Error("PRCV_set_obj_dbi EOM"); return -1;}
    PRCV_memspc_ini (&prc1, -1, mspc1, siz_mspc1);


    irc = PRCV_rdf__ (&prc1);
    if(irc < 0) {
      TX_Error ("PRCV_set_obj_dbi E1-%d-%ld",prc1.typ,prc1.dbi); 
      return -1;
    }
      // PRCV_dump__ (&prc1);

    // index last point
    ipe = prc1.ptNr - 1;


    // find previous-parameter v0s and next-parameter v0e for v0x
    x0x = PRCV_set_find (&ipdb0s, &ip0s, &v0s,
                         &ipdb0e, &ip0e, &v0e, &prc1, &v0x);
      // printf(" x0x=%d ip0s=%ld ip0e=%ld v0s=%lf v0e=%lf\n",x0x,ip0s,ip0e,v0s,v0e);



    // find previous-parameter v1s and next-parameter v1e for v1x
    x1x = PRCV_set_find (&ipdb1s, &ip1s, &v1s,
                         &ipdb1e, &ip1e, &v1e, &prc1, &v1x);
      // printf(" x1x=%d ip1s=%ld ip1e=%ld v1s=%lf v1e=%lf\n",x1x,ip1s,ip1e,v1s,v1e);


    // both values already exist: done.
    if(x0x && x1x) return 0;


  
    //----------------------------------------------------------------
    // init PRCV0
    PRCV0.typ  = prc1.typ;
    PRCV0.dbi  = prc1.dbi;
    PRCV0.mdli = prc1.mdli;
    PRCV0.ptNr = 0;
      // UT3D_stru_dump (Typ_PRCV, &PRCV0, "_set_dbo_-PRCV0");

    
    // check / prepare space; at least for additional (prc1.ptNr + 10) points
    siz = PRCV0.ptNr + prc1.ptNr + 10;
    if(PRCV0.siz < siz) {
      // get space
      irc = PRCV_memspc_add (PRCV0, siz + PRCV0_INC_PTNR);
      if(irc < 0) return irc;
    }


    //----------------------------------------------------------------
    // if one value already exists:
    if(x0x || x1x) goto L_single;



    // test if (v0e >= v1x); if yes:
    if(v0e >= v1x) {
      v0e = v1x;
      v1s = v1x;
      ip1s = ip0e;
    }

      

      // TESTBLOCK
      // END TESTBLOCK



    //================================================================
    // v0x and v1x do not exist.
    //
    // 0.  v0s    v0x    v0e    v1s    v1x    v1e    1.
    //      |      v0     |      |      v1     |
    //    ipdb0s ipdb0x ipdb0e ipdb1s ipdb1x ipdb1e        dbi's of points
    // 0  ip0s           ip0e   ip1s          ip1e  ipe    index of points in prc1
    //
    // ipdb = DB-index of point

    //----------------------------------------------------------------
    // copy 0.0 bis v0s (0 - ip0s)
      // printf("------------------ copy_0-ip0s=%ld\n",ip0s);
    if(ip0s > 0) {
      PRCV_set_copy (&PRCV0, &prc1, 0, ip0s);
    }

    //----------------------------------------------------------------
    // add v0s bis v0x
      // printf("------------------ add v0s=%lf - v0x=%lf\n",v0s,v0x);
    if(v0s < v0x) {
      PRCV_set_seg (&PRCV0, typBas, cvBas, v0s, v0x, ipdb0s, ipdb0x);
    }

    //----------------------------------------------------------------
    // add v0x bis v0e
      // printf("------------------ add v0x=%lf - v0e=%lf\n",v0x,v0e);
    if(v0x < v0e) {
      if(v0e == v1x) ipdb0e = ipdb1x;
      PRCV_set_seg (&PRCV0, typBas, cvBas, v0x, v0e, ipdb0x, ipdb0e);
    }

    //----------------------------------------------------------------
    // copy v0e bis v1s (ip0e - ip1s)
      // printf("------------------ copy ip0e=%ld - ip1s=%ld\n",ip0e,ip1s);
    if(ip0e < ip1s) {
      PRCV_set_copy (&PRCV0, &prc1, ip0e, ip1s);
    }

    //----------------------------------------------------------------
    // add v1s bis v1x
      // printf("------------------ add v1s=%lf - v1x=%lf\n",v1s,v1x);
    if(v1s < v1x) {
      PRCV_set_seg (&PRCV0, typBas, cvBas, v1s, v1x, ipdb1s, ipdb1x);
    }


    //----------------------------------------------------------------
    // add v1x bis v1e
      // printf("------------------ add v1x=%lf - v1e=%lf\n",v1x,v1e);
    if(v1x < v1e) {
      PRCV_set_seg (&PRCV0, typBas, cvBas, v1x, v1e, ipdb1x, ipdb1e);
    }

    //----------------------------------------------------------------
    // copy v1e bis 1.0 (ip1e - end)
      // printf("------------------ copy ip1e=%ld - end=%ld\n",ip1e,ipe);
    if(ip1e < ipe) {
      PRCV_set_copy (&PRCV0, &prc1, ip1e, ipe);
    }

    goto L_xxe;


    //================================================================
    // v0x or v1x already exists.
    // 
    // 0.  v0s    v0x    v0e     1.
    //    ipdb0s ipdb0x ipdb0e                             dbi's of points
    // 0   ip0s          ip0e    ipe                       index of points in prc1

    L_single:

    if(x0x) {
      // v0x already exists; use v1x
      v0s = v1s;
      v0x = v1x;
      v0e = v1e;
      ip0s = ip1s;
      ip0e = ip1e;
      ipdb0s = ipdb1s;
      ipdb0e = ipdb1e;
    }

      // printf(" L_single-v0s=%lf v0x=%lf v0e=%lf\n",v0s,v0x,v0e);
      // printf(" L_single-ipdb0s=%ld ipdb0x=%ld ipdb0e=%ld\n",ipdb0s,ipdb0x,ipdb0e);


    //----------------------------------------------------------------
    // copy from 0. to v0s
      // printf("------------------ copy_0-ip0s=%ld\n",ip0s);
    if(ip0s > 0) {
      PRCV_set_copy (&PRCV0, &prc1, 0, ip0s);
    }

    //----------------------------------------------------------------
    // add v0s to v0x
      // printf("------------------ add v0s=%lf - v0x=%lf\n",v0s,v0x);
    if(v0s < v0x) {
      PRCV_set_seg (&PRCV0, typBas, cvBas, v0s, v0x, ipdb0s, ipdb0x);
    }

    //----------------------------------------------------------------
    // add v0x to v0e
      // printf("------------------ add v0x=%lf - v0e=%lf\n",v0x,v0e);
    if(v0x < v0e) {
      if(v0e == v1x) ipdb0e = ipdb1x;
      PRCV_set_seg (&PRCV0, typBas, cvBas, v0x, v0e, ipdb0x, ipdb0e);
    }

    //----------------------------------------------------------------
    // copy from v0e to 1.
      // printf("------------------ copy ip0e=%ld - end=%ld\n",ip0e,ipe);
    if(ip0e < ipe) {
      PRCV_set_copy (&PRCV0, &prc1, ip0e, ipe);
    }



    //----------------------------------------------------------------
    L_xxe:
      // TESTBLOCK
      // PRCV_dump__ (1, &PRCV0);
      // END TESTBLOCK

    // store curve
    PRCV_wrf__ (&PRCV0);

    // PRCV_rdf_free (&prc1);
    // prc1 = memspc201

  }
  
  goto L_exit;



  //----------------------------------------------------------------
  L_basCv:
    // get PRCV of basecurve
    PRCV0.typ  = typ;
    PRCV0.dbi  = dbi;
    PRCV0.mdli = WC_modact_ind;
    PRCV0.ptNr = 0;

    // irc = PRCV_rdf__ (&PRCV0);
    // if(irc < 0)
      return PRCV_set_basCv (form, obj, dbi);
    

  //----------------------------------------------------------------
  L_exit:
      // printf("exit PRCV_set_obj_dbi =====================\n");

    return 0;



  //----------------------------------------------------------------
  L_Err1_exit:
    PRCV_rdf_free (&prc1);
    return irc;
}


//======================================================================
  int PRCV_set_copy (CurvPrcv *prc2, CurvPrcv *prc1, int ips, int ipe) {
//======================================================================
// add points from prc1 from index ips to index ipe into prc prc2
// including ipe

  int     i1, ipos, ptNr;
  Point   *pTab;


  // printf("PRCV_set_copy %d - %d\n", ips, ipe);
  // printf(" prc2->ptNr=%d\n",prc2->ptNr);
  // PRCV_dump__ (2, prc2, "  prc2");
  // PRCV_dump_1 (-1, prc1);
  // for(i1=ips; i1<=ipe; ++i1) PRCV_dump_1 (i1, prc1);



  //----------------------------------------------------------------
  ipos = prc2->ptNr;          // startindex output

  ptNr = ipe - ips + 1;       // remaining nr of points


  // skip 1. point in prc1 if identical with last point in prc2
  if(ipos > 0) {
    if(UT3D_comp2pt(&prc1->npt[ips], &prc2->npt[ipos - 1], UT_TOL_cv)) {
        // printf(" ...skip first\n");
      // identical; skip first point of prc1
      --ptNr;
      ++ips;
    }
  }

    // printf(" ipos=%d ips=%d ptNr=%d\n",ipos,ips,ptNr);

  // memcpy (&prc1->npt[ipos], &prc2->npt[ips], sizeof(Point) * ptNr);
  // memcpy (&prc1->npar[ipos], &prc2->npar[ips], sizeof(double) * ptNr);
  // memcpy (&prc1->nipt[ipos], &prc2->nipt[ips], sizeof(long) * ptNr);
  memcpy (&prc2->npt[ipos], &prc1->npt[ips], sizeof(Point) * ptNr);
  memcpy (&prc2->npar[ipos], &prc1->npar[ips], sizeof(double) * ptNr);
  memcpy (&prc2->nipt[ipos], &prc1->nipt[ips], sizeof(long) * ptNr);

  prc2->ptNr += ptNr;

    // PRCV_dump__ (2, prc2, " ex PRCV_set_copy");

  return 0;

}

 
//================================================================
  int PRCV_set_seg (CurvPrcv *prc, int form, void *cvBas,
                       double vs, double ve, long ipdbs, long ipdbe) { 
//================================================================
// add curve from vs to ve to prc

  int     irc, i1, iis, iie, ptNr, tabSiz;
  char    cvMod[OBJ_SIZ_MAX];
  Point   *pTab;


  // printf("PRCV_set_seg form=%d vs=%lf ve=%lf ipdbs=%ld ipdbe=%ld\n",form,
         // vs,ve,ipdbs,ipdbe);
    // UT3D_stru_dump (form, cvBas, " PRCV_set_seg - cvBas ");


  // create a copy of cvBas
  memcpy (cvMod, cvBas, OBJ_SIZ_MAX);
    // UT3D_stru_dump (form, cvMod, " PRCV_set_seg - cvMod1 ");

  // set limitpoints / modify v0, v1
  UTO_set_ptlim (form, cvMod, NULL, &vs, NULL, &ve);
    // UT3D_stru_dump (form, cvMod, " PRCV_set_seg - cvMod2 ");


  //----------------------------------------------------------------
  iis = prc->ptNr;                   // startindex
  if(iis > 0) --iis;                 // overwrite last point.

  pTab = &prc->npt[iis];             // add new points from here ..
  tabSiz = prc->siz - iis;           // remaining nr of points
    // printf(" iis=%d tabSiz=%d\n",iis,tabSiz);


  //----------------------------------------------------------------
  ptNr = 0;
  irc = UT3D_npt_obj (&ptNr, pTab, tabSiz, form, cvMod, 1, UT_DISP_cv, 0);
  if(irc < 0) {
    printf("**** PRCV_set_seg UT3D_npt_obj %d\n",irc);
    return -1;
  }
  prc->ptNr = iis + ptNr;

    // TESTBLOCK
    // printf(" iis-neu=%d ptNr=%d\n",iis,ptNr);
    // for(i1=0;i1<ptNr;++i1) UT3D_stru_dump(Typ_PT,&pTab[i1],"pTab[%d]=",i1);
    // END TESTBLOCK


  //----------------------------------------------------------------
  // init all parameters = invalid
  for(i1=0; i1<ptNr; ++i1) {
    prc->npar[iis + i1] = UT_VAL_MAX;
    prc->nipt[iis + i1] = 0;
  }

  prc->npar[iis] = vs;
  prc->nipt[iis] = ipdbs;
    // printf(" npar[%d]=%lf\n",iis,prc->npar[iis]);


  iie = iis + ptNr - 1;
  prc->npar[iie] = ve;
  prc->nipt[iie] = ipdbe;
    // printf(" npar[%d]=%lf\n",iie,prc->npar[iie]);


    // TESTBLOCK
    // printf("ex PRCV_set_seg prc.ptNr=%d iis=%d ptNr=%d\n",prc->ptNr,iis,ptNr);
    // PRCV_dump__ (2, prc);
    // END TESTBLOCK




  return 0;

}

/*
//================================================================
  int PRCV_insert_par (CurvPrcv *prc2, CurvPrcv *prc1,
                       int form, void *cvBas, double vx, long ipx) {
//================================================================
 
  int     irc;
  long    ips, ipe;
  Point   *ptx;

  
  printf("PRCV_insert_par form=%d ipx=%ld\n",form,ipx);


  // get parameter vx from ipx
  if(vx != UT_VAL_MAX) goto L_1;

  // get point from ipx
  ptx = DB_get_PT (ipx);

  // get parameters from point
  UTO_par1_pt_pt_obj (&vx, ptx, form, cvBas);




  //----------------------------------------------------------------
  L_1:
    printf("PRCV_insert_par vx=%lf ipx=%ld form=%d\n",vx,ipx,form);

    // find previous-parameter v0 and next-parameter v1
    irc = PRCV_set_find (&ips, &ipe, prc1, &vx);
    if(irc < 0) return irc;

    // copy curve start-parameter to previous-parameter

    // create and add curve previous-parameter to new-parameter
    // irc = PRCV_insert_cvTrm (&prc1, form, obj, v0, ipx);
    // if(irc < 0) return -1;

    // create and add curve new-parameter to next-parameter
    // irc = PRCV_insert_cvTrm (&prc1, form, obj, v1, ipx);
    // if(irc < 0) return -1;

    // copy curve next-parameter to end-parameter



  // get points -> prc.npt
  // irc = UT3D_npt_obj (&prc->ptNr, prc->npt, tabSiz, form, cvBas, 1, UT_DISP_cv);

  // fix index of start/endpoints in PRCV

  return 0;

}
*/

//===========================================================================
  int PRCV_set_find (long *ipdbs, long *ips, double *vs,
                     long *ipdbe, long *ipe, double *ve,
                     CurvPrcv *prc1, double *vx) {
//===========================================================================
/// \code
/// get previous-parameter and next-parameter
/// Input:
///   vx      value to test
/// Output:
///   vs      previous value, vs < vx
///   ve      next value, ve > vx
///   retCod  0  previous and next value found
///           1  value already exists
/// \endcode

  int      irc, i1, ie;
  double   vx1, vx2, tol = UT_TOL_min0;


  // printf("PRCV_set_find %lf\n",*vx);

  ie = prc1->ptNr - 1;

  *ips   = 0;
  *vs    = prc1->npar[0];
  *ipdbs = prc1->nipt[0];

  *ipe   = ie;
  *ve    = prc1->npar[ie];
  *ipdbe = prc1->nipt[ie];

  vx1 = *vx - tol;
  vx2 = *vx + tol;

    // TESTBLOCK
    // printf(" ips=%ld ipe=%ld vs=%lf ve=%lf\n",*ips,*ipe,*vs,*ve);
    // printf(" vx1=%lf vx2=%lf ie=%d\n",vx1,vx2,ie);
    // END TESTBLOCK



  for(i1 = 0; i1 < prc1->ptNr; ++i1) {

    // skip -undef- values
    if(prc1->npar[i1] == UT_VAL_MAX) continue;

    if(prc1->npar[i1] < vx1) {
      *ips = i1;
      *vs  = prc1->npar[i1];
      *ipdbs = prc1->nipt[i1];
      continue;
    }


    if(prc1->npar[i1] < vx2) {
      irc = 1;
      // value already exists..

    } else {
      irc = 0;
      // next value
    }

    *ipe = i1;
    *ve  = prc1->npar[i1];
    *ipdbe = prc1->nipt[i1];
    break;
  }

    // printf("ex PRCV_set_find irc=%d ips=%ld ipdbs=%ld vs=%lf\n\
                       // ipe=%ld ipdbe=%ld ve=%lf vx=%lf\n",
           // irc, *ips, *ipdbs,*vs, *ipe,*ipdbe,*ve, *vx);


  return irc;

}

 
//================================================================
  int PRCV_set_basCv (int form, void *cvBas, long dbi) {
//================================================================
// create PRCV for basicCurve

  int       irc, i1, ptNr, tabSiz, siz;
  // long      ips, ipe;
  double    v0, v1;
  // CurvPrcv  prc;


  // printf("PRCV_set_basCv form=%d dbi=%ld\n",form,dbi);
  // UT3D_stru_dump (form, cvBas, " PRCV_set_basCv ");


  //----------------------------------------------------------------
  // get nr of necessary points for bin-cvCut
  ptNr = UT3D_ptNr_obj (form, cvBas, UT_DISP_cv);
  if(ptNr < 2) {TX_Error ("PRCV_set_basCv E2-TODO"); return -1;}

  tabSiz = ptNr + 12;
    // printf(" tabSiz=%d\n",tabSiz);


  //----------------------------------------------------------------
  // init space
  PRCV0.typ  = AP_typ_2_bastyp (form);
  PRCV0.dbi  = dbi;
  PRCV0.mdli = WC_modact_ind;
  PRCV0.ptNr = 0;

  // test space
  if(tabSiz > PRCV0.siz) {
    // get space for <ptNr> more points
    irc = PRCV_memspc_add (&PRCV0, tabSiz + PRCV0_INC_PTNR);
    if(irc < 0) return irc;
  }




  //----------------------------------------------------------------
  // get memSpc for PRCV
  // prc.siz  = tabSiz;
  // prc.ptNr = 0;
  // prc.npt  = (Point*)MEM_alloc_tmp(sizeof(Point) * tabSiz);
  // prc.npar = (double*)MEM_alloc_tmp(sizeof(double) * tabSiz);
  // prc.nipt = (long*)MEM_alloc_tmp(sizeof(long) * tabSiz);
  // prc.fTmp = 1;  // 0=malloced-must-free; 1=heapSpc


  // init all pointIndices = 0
  memset (PRCV0.nipt, '\0', sizeof(long) * tabSiz);

  // init all parameters = invalid
  for(i1=0; i1<tabSiz; ++i1) PRCV0.npar[i1] = UT_VAL_MAX;



  //----------------------------------------------------------------
  // get points -> PRCV0.npt
  ptNr = 0;
  irc = UT3D_npt_obj (&ptNr, PRCV0.npt, tabSiz, form, cvBas, 1, UT_DISP_cv, 0);
  PRCV0.ptNr = ptNr;
  if(irc < 0) return -1;
    // printf(" npt_cvCut irc=%d ptNr=%d\n",irc,ptNr);
  // see also: UT3D_npt_obj UT3D_npt_ci UT3D_cv_ci UT3D_pta_ccv


  // get limiting parameters for curve
  irc = UTO_parLim_get_cv (&v0, &v1, form, cvBas);
  if(irc < 0) return -1;


  // set parameters
  PRCV0.npar[0] = v0;
  i1 = PRCV0.ptNr - 1;
  PRCV0.npar[i1] = v1;


    // TESTBLOCK
    // PRCV_dump__ (1, &PRCV0);
    // END TESTBLOCK


  //----------------------------------------------------------------
  // write file
  PRCV_wrf__ (&PRCV0);


  return 0;

}

/*
//=====================================================================
  int PRCV_insert__ (CurvCCV *ccvi, int form, void *cvCut, void *cvBas) {
//=====================================================================
// Create PRC with one or 2 intermediate points
// insert cutPoints ip0 and ip1 into PRC of basicCurve
// Input:
//   ccvi      trimmed-curve
//   form      type of data of cvBas and cvCut
//   cvCut     data standardCurve of ccvi
//   cvBas     data baseCurve (ccvi->dbi) NULL if cvCut == cvBas
// Output:
//   irc       -1  PRCV (file)  for baseCurve does not exist
//
// load basecurve; find previous-point, find next-point;
// make curve previous-point to insert-point
// make curve insert-point to next-point;
// write curve start-point to previous-point;
// write curve previous-point to insert-point;
// write curve insert-point to end-point.

// replacing PRCV_ccv__


  int       irc, ptNr, tabSiz, mode, ips, ipe;
  Point     *ptx;
  CurvCCV   cvx;
  CurvPrcv  prcBas, prcCut;


  printf("--------------------------------- \n");
  UT3D_stru_dump (Typ_CVTRM, ccvi, " PRCV_insert__ ccvi");
 

  // prcBas.dbi  = ccvi->dbi;
  // prcBas.mdli = WC_modact_ind;


  // load basecurve;
  irc = PRCV_rdf__ (&prcBas);
  if(irc < 0) {
    // create basicCurve
    return -1;
  }


  //----------------------------------------------------------------
  // get nr of necessary points for bin-cvCut
  if(prcBas.ptNr < 2) {
    ptNr = UT3D_ptNr_obj (form, cvCut, UT_DISP_cv);
    if(ptNr < 2) {TX_Error ("PRCV_ccv__ E2-TODO"); return -1;}
    prcCut.ptNr = ptNr;
      printf(" ptNr=%d\n",ptNr);
  }


  //----------------------------------------------------------------
  // get memSpc for PRCV
  tabSiz = prcCut.ptNr + 10;
    printf(" tabSiz=%d\n",tabSiz);
  prcCut.npt = (Point*)MEM_alloc_tmp(sizeof(Point) * tabSiz);
  prcCut.npar = (double*)MEM_alloc_tmp(sizeof(double) * tabSiz);
  prcCut.nipt = (long*)MEM_alloc_tmp(sizeof(long) * tabSiz);
  prcCut.fTmp = 1;  // 0=malloced-must-free; 1=heapSpc

  // init
  memset (prcCut.nipt, '\0', sizeof(long) * tabSiz);


  // first ccvi->ip0, then ccvi->ip1
  cvx = *ccvi;

  if(prcBas.ptNr < 2) {
    mode = 2;
    goto L_nxt_pt;
  }

  mode = 0;       // mode: 0=ip0_to_ipx; 1=ipx_to_ip1; 2=ip0_to_ip1
  // get ips = startpoint of basic-curve
  // get ipe = endpoint of basic-curve


  //================================================================
  L_nxt_pt:

  // find previous-point, find next-point;
  if(mode == 0) {
    // ips_to_ip0
    cvx.ip0 = ccvi->ip0;
    cvx.ip1 = ips;

  } else if(mode == 1) {
    // ip1_to_ipe
    cvx.ip0 = ccvi->ip1;
    cvx.ip1 = ipe;

    // else mode = 2; ip0_to_ip1 - basic curve does not yet exist !
  }


  // get points for cvx
  prcCut.ptNr = 0;
  irc = UT3D_npt_obj (&prcCut.ptNr, prcCut.npt, tabSiz, form, &cvx,1,UT_DISP_cv);
  if(irc < 0) return -1;
    printf(" npt_cvx irc=%d ptNr=%d\n",irc,ptNr);
  // UT3D_npt_obj UT3D_npt_ci UT3D_cv_ci UT3D_pta_ccv




//   // insert point ipx;
//   // get parameter vx of point ipx
//   if(ccvi->v0 == UT_VAL_MAX) {
//     UTO_par1_pt_pt_obj (&vx, &ptx, form, cvBas);
//   }



  // insert cutPoint ip0


  // insert cutPoint ip1


// make curve previous-point to insert-point
// make curve insert-point to next-point;
// write curve start-point to previous-point;
// write curve previous-point to insert-point;
// write curve insert-point to end-point.



//   if(mode == 0) {
//     mode = 1;       // mode: 0=ip0_to_ipx; 1=ipx_to_ip1; 2=ip0_to_ip1
//     goto L_nxt_pt;
//   }
//   //================================================================


  PRCV_rdf_free (&prcBas);


  return 0;

}


//=========================================================================
  int PRCV_ccv__ (int mode, CurvCCV *ccvi,
                  int form, void *cvCut, void *cvBas, int iAtt) {
//=========================================================================
// get PRCV from trimmed-curve
// Input:
//   mode      1=write_file; 2=read_file; 4=display
//   ccvi      trimmed-curve
//   cvCut     data standardCurve of ccvi
//   cvBas     data basic-curve (ccvi->dbi) NULL if cvCut == cvBas
// Output:
//   file or display (mode)
 
  int       irc, oNr, ptNr, tabSiz, i1;
  long      l1, dli;
  CurvPrcv  prc;


  printf("=========================================== \n");
  printf("PRCV_ccv__ mode=%d iatt=%d\n",mode,iAtt);
  UT3D_stru_dump (Typ_CVTRM, ccvi, " ccvi");
  UT3D_dump_dbo (Typ_PT, ccvi->ip0, " ip0");
  UT3D_dump_dbo (Typ_PT, ccvi->ip1, " ip1");


  prc.typ  = AP_typ_2_bastyp (form);
  prc.dbi  = ccvi->dbi;
  prc.mdli = WC_modact_ind;


  //----------------------------------------------------------------
  // bit 2: read_file
  if(mode & 2) { 
    PRCV_rdf__ (&prc);
    goto L_100;
  }


  //----------------------------------------------------------------
  // get nr of necessary points for bin-cvCut
  ptNr = UT3D_ptNr_obj (form, cvCut, UT_DISP_cv);
  if(ptNr < 2) {TX_Error ("PRCV_ccv__ E2-TODO"); return -1;}

  prc.ptNr = ptNr;
    printf(" ptNr=%d\n",ptNr);


  //----------------------------------------------------------------
  // get memSpc for PRCV
  tabSiz = ptNr + 10;
  prc.npt = (Point*)MEM_alloc_tmp(sizeof(Point) * tabSiz);
  prc.npar = (double*)MEM_alloc_tmp(sizeof(double) * tabSiz);
  prc.nipt = (long*)MEM_alloc_tmp(sizeof(long) * tabSiz);
  prc.fTmp = 1;  // 0=malloced-must-free; 1=heapSpc

  // init
  memset (prc.nipt, '\0', sizeof(long) * tabSiz);



  //----------------------------------------------------------------
  // get points & parameters
  prc.ptNr = 0;
  irc = UT3D_npt_obj (&prc.ptNr, prc.npt, tabSiz, form, cvCut, 1, UT_DISP_cv);
  if(irc < 0) return -1;
    printf(" npt_cvCut irc=%d ptNr=%d\n",irc,ptNr);
  // UT3D_npt_obj UT3D_npt_ci UT3D_cv_ci UT3D_pta_ccv


  // store DB-ID's in PRCV
  i1 = prc.ptNr - 1;
  prc.nipt[0] = ccvi->ip0;
  prc.nipt[i1] = ccvi->ip1;


  // get & store parameters in PRCV
  if(ccvi->v0 == UT_VAL_MAX) 
    UTO_par1_pt_pt_obj (&prc.npar[0], &prc.npt[0], form, cvBas);

  if(ccvi->v1 == UT_VAL_MAX) 
    UTO_par1_pt_pt_obj (&prc.npar[i1], &prc.npt[i1], form, cvBas);

    printf(" par[%d]=%f\n",0,prc.npar[0]);
    printf(" par[%d]=%f\n",i1, prc.npar[i1]);


  // PRCV OK; use it according mode
    PRCV_dump__ (1, &prc);


  //----------------------------------------------------------------
  L_100:
  //----------------------------------------------------------------
  // bit 1: write_file
  if(mode & 1) { 
    PRCV_wrf__ (&prc);
  }


  //----------------------------------------------------------------
  // bit 2: display
  if(mode & 4) {
    dli = DL_StoreObj (Typ_CI, ccvi->dbi, iAtt);
      printf(" DISP: dli=%ld\n",dli);
      // for(i1=0;i1<prc.ptNr;++i1) UT3D_stru_dump(Typ_PT,&prc.npt[i1],"%d",i1);
    GL_DrawPoly (&dli, iAtt, prc.ptNr, prc.npt);
  }


  //----------------------------------------------------------------
  // free memspc
  if(!prc.fTmp) PRCV_rdf_free (&prc);


  return 0;

}
*/

//=====================================================================
  int PRCV_memspc_ini (CurvPrcv *prc, int ptNr, char* mSpc, int mSiz) {
//=====================================================================
// PRCV_memspc_ini             init memspc for CurvPrcv
// Input:
//   ptNr     -1   use complete space
//            >0   get only this size
//   mSpc     memspc to be used
//   mSiz     size of mSpc in chars

// Example using temp. Memspc:
//  Memspc *tbuf1 int siz_mspc1; void *mspc1;
//  siz_mspc1 = UME_ck_free (tbuf1);
//  mspc1 = UME_reserve (tbuf1, siz_mspc1);
//  PRCV_memspc_ini (&prc1, -1, mspc1, siz_mspc1);

// Example using tempSpace:
//  int siz_mspc1; void *mspc1;
//  siz_mspc1 = 200000;  // byte
//  mspc1 = (void*) MEM_alloc_tmp (siz_mspc1);
//  if(!mspc1) {TX_Error("fNam EOM"); return -1;}
//  PRCV_memspc_ini (&prc1, -1, mspc1, siz_mspc1);

// TODO:  fTmp always = 1; 

  int   ii;
  char  *cp1;


  // printf("PRCV_memspc_ini %d\n",ptNr);


  ii = sizeof(Point) + sizeof(double) + sizeof(long);

  if(ptNr < 0) {
    // compute size
    ptNr = mSiz / ii;

  } else {
    // check size
    if((ptNr * ii) > mSiz) {
      printf("**** PRCV_memspc_ini EOM\n");
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
  prc->fTmp = 1;      // 0=malloced-must-free; 1=do not free

  return 0;

}


//================================================================
  int PRCV_memspc_add (CurvPrcv *prc, int ptNr) {
//================================================================
/// \code
/// PRCV_memspc_add     add space for <ptNr> points
/// not enough memspc. malloc-realloc; heap-Error; empty-malloc.
/// Input:
///   ptNr      total size in nr of points
/// Retcodes:     0=OK, -1=Error-heap; -2=EOM
/// \endcode


  // printf("PRCV_memspc_add %d\n",ptNr);
  // UT3D_stru_dump (Typ_PRCV, prc, "  prc ");
  // PRCV_dump__ (2, prc, "  prc-in");

  if(ptNr < 1 ) return 0;


  if(prc->fTmp == 2) {
    // empty; malloc.
    prc->npt  = (Point*)malloc(sizeof(Point) * ptNr);
    prc->npar = (double*)malloc(sizeof(double) * ptNr);
    prc->nipt = (long*)malloc(sizeof(long) * ptNr);
    prc->siz  = ptNr;
    prc->fTmp = 0;  // 0=malloced-must-free; 1=heapSpc
    goto L_load;
  }


  if(prc->fTmp == 1) {
    // heap-Error     See PRCV_memspc_ini.
    TX_Error ("PRCV_rdf__ EOM-1");
    return -1;
  }

  if(prc->fTmp == 0) {
    // 0=malloc-realloc;
    prc->npt  = (Point*)realloc(prc->npt, sizeof(Point) * ptNr);
    prc->npar = (double*)realloc(prc->npar, sizeof(double) * ptNr);
    prc->nipt = (long*)realloc(prc->nipt, sizeof(long) * ptNr);
    prc->siz  = ptNr;
    goto L_load;
  }

  TX_Error ("PRCV_rdf__ EOM-2");
  return -1;


  //----------------------------------------------------------------
  L_load:
  if(!prc->npt) return -2;
  if(!prc->npar) return -2;
  if(!prc->nipt) return -2;

    // PRCV_dump__ (2, prc, "ex-PRCV_memspc_add");

  return 0;

}

 
//================================================================
  int PRCV_wrf__ (CurvPrcv *prc) {
//================================================================
// write cvp into file "[M<subModelNr>]S<curve#>.odat"
// ofid   (objectfile-id)   subModelNr [M<subModelNr>]S<curve#>.odat
//  see APED_oid_dbo__


  int       rNr;
  char      ofid[128];
  FILE      *fp1;



  // printf("PRCV_wrf__ M%d %ld\n",prc->mdli,prc->dbi);


  PRCV_fnam__ (ofid, prc);
    // printf(" PRCV_wrf__-ofid=|%s|\n",ofid);


  if((fp1=fopen(ofid,"wb")) == NULL) {
    TX_Print("PRCV_wrf__ E001 |%s|",ofid);
    return -1;
  }


  rNr = prc->ptNr;

  fwrite(prc, sizeof(CurvPrcv), 1, fp1);

  fwrite(prc->npt, sizeof(Point), rNr, fp1);
  fwrite(prc->npar, sizeof(double), rNr, fp1);
  fwrite(prc->nipt, sizeof(long), rNr, fp1);

  fclose(fp1);


    // printf("ex PRCV_wrf__ ptNr=%d\n",rNr);
    // PRCV_dump__ (2, prc, "ex PRCV_wrf__");


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

  return 0;

}


//================================================================
  int PRCV_fsiz__ (CurvPrcv *prc) {
//================================================================
// PRCV_fsiz__             get filesize

  char   fn1[128];

  PRCV_fnam__ (fn1, prc);

  return (OS_FilSiz (fn1) * 1.5);

}

 
//================================================================
  int PRCV_rdf__ (CurvPrcv *prc) {
//================================================================
/// \code
/// read PRCV from file "[M<subModelNr>]S<curve#>.odat"
/// Memspace for points, params is mallocd, if not provided with PRCV_memspc_ini
///   If Malloced: MUST BE FREED WITH PRCV_rdf_free !
/// Input:
///   prc->typ
///   prc->dbi
///   prc->mdli
/// Output:
///   retCod      0 OK; MUST PRCV_rdf_free()
///              -1 file not found
///
/// ofid   (objectfile-id)   subModelNr [M<subModelNr>]S<curve#>.odat
///  see APED_oid_dbo__
/// \endcode


  int      irc, tabSiz, ptNr, inSiz;
  char     ofid[128], inTmp;
  FILE     *fp1;
  CurvPrcv fPrc;



  // UT3D_stru_dump (Typ_PRCV, prc, "PRCV_rdf__: ");

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
    // UT3D_stru_dump (Typ_PRCV, &fPrc, "_rdf_");

  ptNr = fPrc.ptNr;
  tabSiz = ptNr + 10;

  if(tabSiz < prc->siz) goto L_load;


  //----------------------------------------------------------------
  // not enough memspc. malloc-realloc; heap-Error; empty-malloc.
  irc = PRCV_memspc_add (prc, tabSiz + PRCV0_INC_PTNR);
  if(irc < 0) return -1;


  //----------------------------------------------------------------
  L_load:

  prc->ptNr = ptNr;

  fread(prc->npt, sizeof(Point), ptNr, fp1);
  fread(prc->npar, sizeof(double), ptNr, fp1);
  fread(prc->nipt, sizeof(long), ptNr, fp1);

  fclose(fp1);


    // UT3D_stru_dump (Typ_PRCV, prc, "ex PRCV_rdf__: ");
    // PRCV_dump__ (2, prc, "ex PRCV_rdf__");
    // printf("ex PRCV_rdf__ ptNr=%d\n",prc->ptNr);

  return 0;

}


//================================================================
  int PRCV_rdf_free (CurvPrcv *prc) {
//================================================================
 
  // UT3D_stru_dump (Typ_PRCV, prc, "PRCV_rdf_free");
  // PRCV_dump__ (2, prc, "PRCV_rdf_free");


  if(!prc->fTmp) {

    if(prc->npt)  free (prc->npt);
    if(prc->npar)  free (prc->npar);
    if(prc->nipt)  free (prc->nipt);
    prc->siz  = 0;
    prc->fTmp = 2;
  }

  return 0;

}


//================================================================
  int PRCV_dump_dbo (int mode, int dbTyp, long dbi) {
//================================================================
// no PRC-files for Line, CurvCCV
// Input:
//   mode     0 print short
//            1 display
//            2 print all


  int       irc;
  CurvPrcv  prc;


  // printf("PRCV_dump_dbo %d %d %ld\n",mode,dbTyp,dbi);

  // if((dbTyp == Typ_LN)      ||
     // (dbTyp == Typ_CVTRM)) goto L_e1;   hier nicht form !


  prc.typ  = dbTyp;
  prc.dbi  = dbi;
  prc.mdli = WC_modact_ind;
  prc.siz  = 0;
  prc.fTmp = 2;

  irc = PRCV_rdf__ (&prc);
  if(irc < 0) return -1;

  // PRCV_dump__ (mode, &prc, "PRCV_dump_dbo");

  PRCV_rdf_free (&prc);

  return 0;

  // L_e1:
    // printf("**** PRCV_dump_dbo I1 %d %ld\n",dbTyp, dbi);
    // return -1;
}


//================================================================
  int PRCV_dump_1 (int ii, CurvPrcv *prc) {
//================================================================

  char   s1[80], s2[32];


  if(ii < 0) {
    printf("---- ptNr=%d siz=%d tmp=%d\n", prc->ptNr, prc->siz, prc->fTmp);
    return 0;
  }


  if((prc->npar[ii] < -FLT_32_MAX) || (prc->npar[ii] > FLT_32_MAX))
    strcpy(s1, "    -undef-");
  else sprintf (s1, "%11.3f", prc->npar[ii]);


  if((prc->nipt[ii] < -INT_32_MAX) || (prc->nipt[ii] > INT_32_MAX)) 
    strcpy(s2, "  ---");
  else sprintf (s2, "%5ld", prc->nipt[ii]);


  printf("%5d pt = %11.3f %11.3f %11.3f   par %s   dbi %s\n", ii,
         prc->npt[ii].x, prc->npt[ii].y, prc->npt[ii].z,
         s1,    // prc->npar[ii],
         s2);   // prc->nipt[ii]);


  return 0;

}

 
//================================================================
  int PRCV_dump__ (int mode, CurvPrcv *prc, char *txt) {
//================================================================
// Input:
//   mode     0 print short
//            1 display triangles at all points
//            2 print all
 

  int    i1, iatt;
  char   s1[80];


  printf("-------------------- PRCV_dump__ %s\n",txt);
  printf("  %d ptNr=%d siz=%d tmp=%d\n", mode, prc->ptNr, prc->siz, prc->fTmp);

  for(i1 = 0; i1 < prc->ptNr; ++i1) {

    iatt = ATT_COL_YELLOW;

    if(mode == 2) {
      // print all
      PRCV_dump_1 (i1, prc);


    // print marked-points
    } else {
      // if(prc->npar[i1] != UT_VAL_MAX) {
      if((prc->npar[i1] > -FLT_32_MAX) && (prc->npar[i1] < FLT_32_MAX)) {
        iatt = ATT_COL_RED;
        // printf("prc.npar[%d] = %lf\n",i1,prc->npar[i1]);
        printf("%5d pt = %11.3f %11.3f %11.3f   par %11.3f   dbi %5ld\n", i1,
               prc->npt[i1].x, prc->npt[i1].y, prc->npt[i1].z,
               prc->npar[i1],
             prc->nipt[i1]);
      }
    }


    // display
    if(mode == 1) {
      APT_disp_SymB (SYM_TRI_S, iatt, &prc->npt[i1]);
      if(prc->nipt[i1]) GR_Disp_txi (&prc->npt[i1], i1, 0);
    }

  }
  
  return 0;

}


//============================================================================
  int PRCV_npt_trmCv (Point **pta, int *ptNr, CurvCCV *ccv1) {
//============================================================================
// PRCV_npt_trmCv        get polygon of trimmedCurve.
// see UT3D_npt_obj GR_DrawCvCCV UTO_cv_cvtrm


  int      irc;
  long     dbi;


  // UT3D_stru_dump (Typ_CVTRM, ccv1, " PRCV_npt_trmCv");

  // clear CurvPrcv PRCV0
  PRCV0.ptNr = 0;

  // add trimmed-curve ccv1 to prc1
  irc = PRCV_get_dbo_add_tc (&PRCV0, ccv1);
    printf(" _add_tc-irc=%d\n",irc);


  *pta = PRCV0.npt;
  *ptNr = PRCV0.ptNr;

      // TESTBLOCK
      // GR_Disp_pTab (*ptNr, *pta, SYM_STAR_S, 2);
      // END TESTBLOCK

  return irc;

}


//================================================================
//================================================================
// Liste_TESTFUNKTIONEN:
//================================================================
//================================================================

#ifdef DEB



//================================================================
  int PRCV_test__ () {
//================================================================
 
  int     ptNr;
  Point   *pta;

  printf("PRCV_test__ \n");


  // OS_system ("rm -rf /mnt/serv1/Devel/gcad3d/gCAD3D/tmp/*.odat");

  // create PRCV for typ/dbi; not necessary for Lines
  // PRCV_test_set (Typ_CI, 20L);
  PRCV_test_set (Typ_CV, 20L);
  PRCV_test_set (Typ_CV, 21L);
  PRCV_test_set (Typ_CV, 22L);

    // TESTBLOCK
    // PRCV_dump_dbo (2, Typ_CI, 20L);
    // PRCV_dump_dbo (2, Typ_CV, 20L);
    // END TESTBLOCK

  // get PRCV for dbo
  // PRCV_test_get (Typ_CI, 20L);
  // PRCV_test_get (Typ_CV, 20L);
  PRCV_test_get (Typ_CV, 21L);
  PRCV_test_get (Typ_CV, 22L);


  return 0;

}


//================================================================
  int PRCV_test_set (int typ, long dbi) {
//================================================================
// create PRCV for typ/dbi; not necessary for Lines
 
  int   iAtt;
  long  dli;



  printf("################################################ \n");
  printf("PRCV_test_set %d %ld\n", typ, dbi);

  // create PRCV for typ/dbi
  // mode: 1=write_file; 2=read_file; 4=display
  PRCV_set_dbo__ (typ, dbi);

    // TESTBLOCK
    // PRCV_dump_dbo (2, typ, dbi);
    // END TESTBLOCK

  return 0;

}


//================================================================
  int PRCV_test_get (int typ, long dbi) {
//================================================================
// PRCV_test_get            test get PRCV for dbo
// display

  int       irc, ptNr;
  Point     *pta;

  printf("PRCV_test_get typ=%d dbi=%ld\n",typ,dbi);

  irc = PRCV_npt_dbo__ (&pta, &ptNr, typ, dbi, WC_modact_ind);
  if(irc < 0) return -1;
    printf(" ex-PRCV_npt_dbo__-ptNr = %d\n",ptNr);


    // TESTBLOCK
    // GR_Disp_cv (pta, ptNr, 11);
    GR_Disp_npti (ptNr, pta, SYM_STAR_S, ATT_COL_RED, ATT_COL_YELLOW);
    // END TESTBLOCK

  return 0;

}


//================================================================
  int PRCV_test_disp (CurvPrcv *prc) {
//================================================================
// display polygon in CurvPrcv


  int     att;


  att = 11;  // see ~/gCAD3D/cfg/ltyp.rc

  GR_Disp_cv (prc->npt, prc->ptNr, att);

  return 0;

}


#endif

// EOF
