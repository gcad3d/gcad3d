// ../ut/ut_prcv__.c                   RF                     2003-07-01
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
PRCV = polygonal_representation_of_curve     see INF_PRCV
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
  GR_set_ccv UT3D_pta_ox_lim
           


-----------------------------------------------------
Modifications:
2016-10-19 Neu.  RF.

-----------------------------------------------------
*/
#ifdef globTag
void PRCV(){}
#endif
/*!
\file  ../ut/ut_prcv__.c
\brief polygonal_representation_of_curve  (struct CurvPrcv)
\code

See INF_PRCV

=====================================================
List_functions_start:

PRCV_init__         init PRCV0
PRCV0_OCC           occupy PRCV0
PRCV0_REL           release PRCV0

PRCV_set_dbo__      create PRCV for DB-obj (typ/dbi)
PRCV_set_obj_dbi    create PRCV for bin.obj (must have DB-index)
PRCV_set_basCv      create PRCV for basicCurve
PRCV_set_seg        add curve from vs to ve to PRCV
PRCV_set_find       get previous-parameter and next-parameter

PRCV_npt_dbo__      get polygon for DB-obj
PRCV_npt_trmCv      get polygon of trimmedCurve
PRCV_get_dbo__      get PRCV for DB-obj (typ/dbi)
PRCV_get_dbo_add_tc add trimmed-curve ccv1 to prc1
PRCV_get_tc_add_pa  add point to prc1
PRCV_get_tc_add_prc add <ptNr> points from prc2 starting at point <ips> to prc1
PRCV_get_tc_find    find index in from parameter or dbi in PRCV

PRCV0_free__        free PRCV0 (new-model)

PRCV_dump__         dump a CurvPrcv
PRCV_dump_1         dump single record (ind,par,coords of point)
PRCV_dump_dbo       dump PRCV of DB-obj (curve)
PRCV_test__         call PRCV_test_set, PRCV_test_get, PRCV_test_disp
PRCV_test_set       create PRCV for DB-obj (typ/dbi)
PRCV_test_get       test get PRCV for dbo
PRCV_test_disp      display CurvPrcv

  // PRCV_insert_par
  // PRCV_insert__       Create PRC, add 1 or 2 intermediate points
  // PRCV_ccv__        get PRCV from trimmed-curve

List_functions_end:
=====================================================
see ../ut/ut_prcv_f.c ../ut/ut_prcv_db.c

  
\endcode *//*----------------------------------------


see also: ../ut/ut_ccv.c   trimmed-curve functions


//----------------------------------------------------------------
PRCV = Polygonic Represetation of CurVe.

Every basic-curve (except Line) has a PRCV (polygonal_representation_of_curve)
  The PRCV (struct CurvPrcv) is in a file;
    filename is tmp_dir/[M<subModelNr>]<DB-typ><DB-ind>.odat

Struct CurvPrcv is a polygonal_representation_of_curve -
  - a list of points (npt),
  - a list of the paramter-values of this points (npar), 
  - a list of the DB-indices of this points (nipt).

Every trimmed-curve (CurvCCV) refers to a basic-curve (typ,dbi).
  More than one trimmed-curves can point to the same basic-curve.



----------------------------------------

PRCV_set_obj_dbi():
  Create new Curve (eg circle, bspl, ..):
    Compute a 3D-polygon from parameters, write it into PRCV-file

  Create CCV:
    CCV is 1-n trimmed-curves.
    The trimmed curve has a basic-curve end 2 trim-points.
    The trim-points must be added into the PRCV of the basic-curve.




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
#include "../ut/ut_prcv__.h"              // 
#include "../ut/ut_os.h"                  // OS_ ..
#include "../db/ut_DB.h"                  // DB_GetObjDat
#include "../gr/ut_gr.h"               // GR_tDyn_pcv
#include "../xa/xa_mem.h"                 // memspc*


typedef_MemTab(Point);



//===========================================================================
// EXTERNALS:
// from ../xa/xa.c:
extern int       AP_modact_ind;         // -1=primary Model is active;
                                        // else subModel is being created

//===========================================================================
// LOCALS:
static CurvPrcv PRCV0 = _PRCV_NUL;


int      PRCV_REC_SIZ =  sizeof(Point) + sizeof(double) + sizeof(long);



//================================================================
  int PRCV0_OCC () {
//================================================================
// occupy PRCV0

  // printf("PRCV0_OCC  <<<<<<<<<<<<<<<<<<<<<<< \n");

  if(PRCV0.stat) { TX_Error("PRCV0_mtpa_get E1"); return -1;}

  PRCV0.stat = 1;    // 0='not-in-use'       occupy PRCV0
  PRCV0.ptNr = 0;

  return 0;

}

//================================================================
  int PRCV0_REL () {
//================================================================
// release PRCV0

  // printf("PRCV0_REL  >>>>>>>>>>>>>>>>>>>>>>> \n");

  PRCV0.stat = 0;    // 0='not-in-use'       release PRCV0
  PRCV0.ptNr = 0;

  return 0;

}


/* UNUSED
//================================================================
  int PRCV0_mtpa_get (MemTab(Point) *mtpa)  {
//================================================================
// PRCV0_mtpa_get                 get memspc PRCVO as MemTab(Point)
 

  printf(" PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP PRCV0_mtpa_get\n");
  // DEB_dump_obj__ (Typ_PRCV, &PRCV0, "PRCV0_mtpa_get");
  

  if(PRCV0_OCC()) { TX_Error("PRCV0_mtpa_get E1"); return -1;}

  // give whole space as points to mtpa
  mtpa->rSiz   = sizeof(Point);
  mtpa->rMax = PRCV0.siz * PRCV_REC_SIZ / sizeof(Point);
  mtpa->rNr = 0;
  mtpa->data = PRCV0.npt;
  mtpa->spcTyp = MEMTYP_ALLOC_PROT;

  return 0;

}


//================================================================
  int PRCV0_mtpa_free (MemTab(Point) *mtpa)  {
//================================================================
// PRCV0_mtpa_free               set PRCVO 'not-in-use'


  printf(" PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP PRCV0_mtpa_free\n");
  
  PRCV0.npt  = mtpa->data;
  PRCV0.siz  = mtpa->rMax * sizeof(Point) / PRCV_REC_SIZ;
  PRCV0_REL ();    // release PRCV0

  mtpa->data   = NULL;
  mtpa->spcTyp = MEMTYP_NONE;

    // DEB_dump_obj__ (Typ_PRCV, &PRCV0, "ex-PRCV0_mtpa_free");

  return 0;

}
*/


//================================================================
  int PRCV_init__ () {
//================================================================
// PRCV_init__         init PRCV0
// used by AP_src_new only

  // printf(" PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP PRCV_init__\n");


  if(PRCV0.spcTyp == MEMTYP_NONE) {
    // first use
    // get memSpc for PRCV0
    PRCV0.spcTyp = MEMTYP_ALLOC_PROT;
    PRCV0.siz    = 0;
    PRCV_DB_spc_add (&PRCV0, PRCV0_INC_PTNR * 2);
  }


  PRCV_DB_init ();

  return 0;

}


//================================================================
  int PRCV0_free__ () {
//================================================================
/// \code
/// free PRCV0
/// used only when new model is activated
/// \endcode


  // printf(" PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP PRCV0_free__\n");
  // DEB_dump_obj__ (Typ_PRCV, &PRCV0, " PRCV0_free__ ");


  if(PRCV0.siz) {

    if(PRCV0.npt)   free (PRCV0.npt);
    PRCV0.siz    = 0;
    PRCV0.ptNr   = 0;
    PRCV0.spcTyp = MEMTYP_ALLOC_PROT;
  }

  return 0;

}
 

//===========================================================================
  int PRCV_npt_dbo__ (Point **pta, int *ptNr, int typ, long dbi, int mdli) {
//===========================================================================
/// \code
/// PRCV_npt_dbo__      get polygon for DB-obj
/// Input:
///   typ,dbi,mdli   DB-obj to get
/// Output:
///   pta        returns a pointer to <ptNr> points; do not free.
///   retCod     0=OK, else error
/// \endcode

  int       irc;
  CurvPrcv  prc = _PRCV_NUL;


  // printf("PRCV_npt_dbo__ typ=%d dbi=%ld mdli=%d\n",typ,dbi,mdli);
  // printf("  npt_dbo__-PRCV0.siz=%d\n",PRCV0.siz);

  

  prc.typ  = AP_typDB_typ (typ);
  prc.dbi  = dbi;
  prc.mdli = mdli;

  irc = PRCV_get_dbo__ (&prc);
  if(irc < 0) return -1;

    // TESTBLOCK
    // PRCV_test_disp (&PRCV0);
    // PRCV_dump__ (2, &PRCV0, "PRCV_npt_dbo__");
    // END TESTBLOCK

  // PRCV_DB_free (&prc1);

  *pta = prc.npt;
  *ptNr = prc.ptNr;

  // *pta = PRCV0.npt;
  // *ptNr = PRCV0.ptNr;

  return 0;

}


//================================================================
  int PRCV_get_dbo__ (CurvPrcv *prc1) {
//================================================================
/// \code
/// get PRCV for DB-obj (typ/dbi)
///   Memspc for point, parameters must have been provided with PRCV_DB_spc_ini
///   MUST NOT free prc1
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
  // DEB_dump_obj__ (Typ_PRCV, prc1, " PRCV_get_dbo__-prc1 ");


  // test for CCV (must resolv)
  if(prc1->typ != Typ_CV) goto L_get_basCv;

  form = DB_GetObjDat (&obj, &oNr, prc1->typ, prc1->dbi);
    // printf(" form=%d oNr=%d\n",form,oNr);
  if(form != Typ_CVTRM) goto L_get_basCv;



  //================================================================
  // resolv (group of) trimmed-curve(s); return PRCV0
  // PRCV0.ptNr = 0;
  // *prc1 = PRCV0;
  for(i1=0; i1<oNr; ++i1) {

    // ccv1 = next trimmedCurve
    ccv1 = &((CurvCCV*)obj)[i1];
       // printf("--------------------- _get_dbo__CCV [%d] \n",i1);
       // DEB_dump_obj__ (Typ_CVTRM, ccv1, " ccv[%d]: ",i1);

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
  // load prc2 = basecurve; return the DB-prcv
  irc = PRCV_DB_load (prc1);
  if(irc < 0) {
    TX_Error ("PRCV_get_dbo__ E1-%d-%ld",prc1->typ,prc1->dbi);
    return -1;  // do not free prc1
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
  // for(irc=0;irc<ptNr;++irc)DEB_dump_obj__(Typ_PT, &pta[irc], "pt[%d]:",irc);
  // printf(" tol=%f\n",UT_TOL_cv);



  // test space
  siz = prc1->ptNr + ptNr + 10;
  if(siz > prc1->siz) {
    // get space for <ptNr> more points
    irc = PRCV_DB_spc_add (prc1, siz + PRCV0_INC_PTNR);
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
    // DEB_dump_obj__ (Typ_LN, &ln1, "PRCV_get_dbo_add_ln");

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
  CurvPrcv prc2 = _PRCV_NUL;


  // printf("----------------PRCV_get_dbo_add_tc ------------- \n");
  // DEB_dump_obj__ (Typ_CVTRM, cvi, " PRCV_get_dbo_add_tc-cvi ");
  // DEB_dump_obj__ (Typ_PRCV, prc1, " PRCV_get_dbo_add_tc-prc1 ");
  // PRCV_dump__ (2, prc1, " prc1");


  // if basic-curve of ccv1 is also trimmedCurve: modify in ccv1:
  // - get type and dbi of basicCurve (ccv1.typ and ccv1.dbi)
  // - get v0 and v1 of trimmedCurve on basicCurve (ccv1.v0 and ccv1.v1)
  irc = CVTRM__basCv__ (&ccv1, &cvBas, cvi);
  if(irc < 0) return -1;
    // printf(" _dbo_add_tc-1-typ=%d dbi%ld\n",ccv1.typ,ccv1.dbi);
    // DEB_dump_obj__ (Typ_CVTRM, &ccv1, "     _get_dbo_add_tc-ccv1 ");
    

  // if basic-curve == line: get line and add to prc1.
  if(ccv1.typ == Typ_LN) {
    // add to prc1, no prc2 necessary
    PRCV_get_dbo_add_ln (prc1, &ccv1);
    goto L_exit;
  }


  // get prc2 = PRCV for basecurve of ccv1
  prc2.typ    = AP_typDB_typ (ccv1.typ);
  prc2.dbi    = ccv1.dbi;
  prc2.mdli   = AP_modact_ind;

    // load prc2 = basecurve
  irc = PRCV_DB_load (&prc2);
  if(irc < 0) {
    // not enough memspc for PRCV_rdf__ .. ??
    TX_Error ("PRCV_get_dbo_add_tc E1-%d-%ld",ccv1.typ,ccv1.dbi);
    goto L_err1;   // free prc2
  }
    // PRCV_dump__ (2, &prc2, "  _dbo_add_tc-prc2-1");


  // find startIndex ips in prc2
  ips = PRCV_get_tc_find (ccv1.v0, ccv1.ip0, &prc2);
  if(ips < 0) goto L_err1;   // free prc2

  
  // find endIndex in prc2
  ipe = PRCV_get_tc_find (ccv1.v1, ccv1.ip1, &prc2);
  if(ipe < 0) goto L_err1;   // free prc2


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
    if(irc < 0) goto L_err1;   // free prc2
    goto L_done;
  }


  // not closed: add from ips to ipe
  if(ccv1.clo) {
    // not closed; add from ips to ipe
    ptNr = ipe - ips + 1;
    irc = PRCV_get_tc_add_prc (prc1, &prc2, ptNr, ips, ccv1.dir);
    if(irc < 0) goto L_err1;   // free prc2

  
  } else {
    // closed curve - forward - crossing ipEnd,ipStart:
    // add from ips to ipEnd
    ptNr = prc2.ptNr - ips;
    irc = PRCV_get_tc_add_prc (prc1, &prc2, ptNr, ips, ccv1.dir);
    if(irc < 0) goto L_err1;   // free prc2

    // add from ipStart to ipe.
    ptNr = ipe + 1;
    irc = PRCV_get_tc_add_prc (prc1, &prc2, ptNr, 0, ccv1.dir);
    if(irc < 0) goto L_err1;   // free prc2
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
    if(irc < 0) goto L_err1;   // free prc2
    goto L_done;
  }


  // not closed: add from ips to ipe
  if(ccv1.clo) {
    // not closed; add from ips to ipe
    // revers; add points from ipe -> ips
    ptNr = ips - ipe + 1;
    // add <ptNr> points from prc2 starting at point <ips> to prc1
    irc = PRCV_get_tc_add_prc (prc1, &prc2, ptNr, ipe, ccv1.dir);
    if(irc < 0) goto L_err1;   // free prc2


  } else {
    // closed curve - forward - crossing ipEnd,ipStart:
    // add from 0 to ips
    ptNr = ips + 1;
    irc = PRCV_get_tc_add_prc (prc1, &prc2, ptNr, 0, ccv1.dir);
    if(irc < 0) goto L_err1;   // free prc2

    // add from ipe to ipEnd.
    ptNr = prc2.ptNr - ipe;
    irc = PRCV_get_tc_add_prc (prc1, &prc2, ptNr, ipe, ccv1.dir);
    if(irc < 0) goto L_err1;   // free prc2
  }


  irc = 0;


  //----------------------------------------------------------------
  L_done:
  // free prc2
    PRCV_DB_free (&prc2);

  L_exit:
    // printf("ex PRCV_get_dbo_add_tc\n");
    // if(cvi->dbi == 21L)
    // DEB_dump_obj__ (Typ_PRCV, prc1, " ex-PRCV_get_dbo_add_tc-prc1 ");
    // PRCV_dump__ (2, prc1, " prc1");


  return irc;

  //----------------------------------------------------------------
  L_err1:
    irc = -1;
    goto L_done;

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
  // DEB_dump_obj__ (Typ_PRCV, prc1, "tc_add_prc-prc1");
  // DEB_dump_obj__ (Typ_PRCV, prc2, "tc_add_prc-prc2");
  // PRCV_dump__ (2, prc2, " prc2");


  // if cv is closed - cv fwd, from 0.5 to 0.0 makes 0.5-1.; then 0.-0.
  if(ptNr < 2) return 0;


  // test space
  siz = prc1->ptNr + ptNr + 10;
  if(siz > prc1->siz) {
    // get space for <ptNr> more points
    irc = PRCV_DB_spc_add (prc1, siz + PRCV0_INC_PTNR);
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
        // DEB_dump_obj__ (Typ_PT, &prc1->npt[i1], "prc1[%d]",i1);
        // DEB_dump_obj__ (Typ_PT, &prc2->npt[ips], "prc2[%d]",ips);
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
        // DEB_dump_obj__ (Typ_PT, &prc1->npt[i1 - 1], "prc1");
        // DEB_dump_obj__ (Typ_PT, &prc2->npt[i2], "prc2");
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


    // PRCV_dump__ (2, prc1, "ex-PRCV_get_tc_add_prc");


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
  // DEB_dump_obj__ (Typ_PRCV, prc, " _tc_find");
  // PRCV_dump__ (2, prc, " _tc_find");


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
    if(ii < 0) {
      TX_Print("***** PRCV_get_tc_find E1 vx=%lf ipdb=%ld",vx,ipdb);
      AP_debug__  ("PRCV_get_tc_find E1");
    }

    // TESTBLOCK
    // printf("ex-PRCV_get_tc_find ii=%d vx=%lf ipdb=%ld\n",ii,vx,ipdb);
    // printf("\n\n");
    // END TESTBLOCK

  return ii;

}
 


//================================================================
  int PRCV_set_dbo__ (int typ, long dbi) {
//================================================================
// create PRCV for DB-obj (typ/dbi)
// Input:
//   typ,dbi  dbo baseCurve
// Output:
//   PRCV0
//
// dbo (typ,dbi) can be a circle or polygon or CCV.
// ccv has 1-n trimmed-curves.
// trimmed-curves has a baseCurve which can be a circle .. or a trimmed-curve.
// The baseCurve can have no PRCV yet; test it / create it.


  int      irc, form, oNr, ii, ls;
  void     *obj;
  CurvCCV  *cvt;


  // printf("PPPPPPPPPPPP PRCV_set_dbo__ %d %ld\n",typ,dbi);


  // get data for dbo
  form = DB_GetObjDat (&obj, &oNr, typ, dbi);
    // printf(" _set_dbo__-form=%d oNr=%d\n",form,oNr);



  if(form != Typ_CVTRM) goto L_normal;


  //----------------------------------------------------------------
  // update the basic-curves of trimmed-curves
  cvt = obj;
  // ls = UTO_siz_stru (Typ_CVTRM);

  for(ii=0; ii<oNr; ++ii) {

      // TESTBLOCK
      // printf(" ----------------- PRCV_set_dbo__-nxt %d\n",ii);
      // DEB_dump_obj__ (form, &cvt[ii], " obj[%d]",ii);
      // END TESTBLOCK


    // trimmed-curve: if its baseCurve does not yet have PRCV: create it.
    // check if baseCurve already exists
    if(cvt[ii].typ == Typ_LN) goto L_nxt;  // no PRCV for lines

    if(cvt[ii].dbi < 0) {
      // test if PRCV already exists
      if(cvt[ii].stat == 0) {
        // recurse - create PRCV for baseCurve of this trimmed-curve
        irc = PRCV_set_dbo__ ((int)cvt[ii].typ, cvt[ii].dbi);
        if(irc < 0) {TX_Error("PRCV_set_dbo__ E1"); return irc;}
        // PRCV for basCurve created ..
        cvt[ii].stat = 1;
      }
    }
  
  
    L_nxt:
    continue;
    // ls = UTO_siz_stru (form);
    // obj = (char*)obj + ls;
    // cvt = (char*)cvt + ls;
  }


  //----------------------------------------------------------------
  L_normal:
  // create PRCV
  irc =  PRCV_set_obj_dbi (form, obj, oNr, typ, dbi);
  if(irc < 0) {TX_Error("PRCV_set_dbo__ E2"); return irc;}


    // TESTBLOCK
    // printf("ex-PRCV_set_dbo__\n");
    // if((typ=Typ_CV)&&(dbi == 22L)) {fflush (stdout);exit(-1);}
    // END TESTBLOCK

  return irc;

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
// For trimmed-curves (CCV) find basic-curve and add the trim-points.

 
  int      irc, ptNr, i1, mdli, oTyp, siz, typ_bas, typDB_bas, x0x, x1x, siz_mspc1;
  long     ip0s, ip0e, ip1s, ip1e, ipe, dbiBas;
  long     ipdb0s, ipdb0x,ipdb0e, ipdb1s, ipdb1x, ipdb1e;
  double   v0x, v1x, v0s, v0e, v1s, v1e;
  Point    *pa3, *ptx;
  char     cvCut[OBJ_SIZ_MAX];
  void     *cvBas, *mspc1;
  CurvCCV  *ccv1, ccv2;
  CurvPrcv prc1 = _PRCV_NUL;


  // printf("\n============= PRCV_set_obj_dbi typ=%d form=%d dbi=%ld oNr=%d\n",
         // typ,form,dbi,oNr);
  // DEB_dump_obj__ (form, obj, "PRCV_set_obj_dbi");


  if(typ == Typ_LN) return 1;

  if(!dbi) {
    TX_Error("PRCV_set_obj_dbi no-cvi;no-dbi.");
    return -1;
  }


  PRCV0_OCC ();         // occupy PRCV0

  // index of active model;
  mdli = AP_modact_ind;


  //----------------------------------------------------------------
  if(form != Typ_CVTRM) goto L_basCv;

  // resolv CCV; 
  // CCV does not have own PRCV; decode its component-curves and add all
  // necessary trim-points into its basic-curves.
  for(i1=0; i1<oNr; ++i1) {

    // ccv1 = copy next trimmedCurve
    ccv1 = &((CurvCCV*)obj)[i1];
       // printf("--------------------- CCV [%d] of %d\n",i1,oNr);
       // DEB_dump_obj__ (Typ_CVTRM, ccv1, " ccv[%d]: ",i1);

    // Lines: direct (no PRCV-file)
    if(ccv1->typ == Typ_LN) continue;

    // if basic-curve of ccv1 is also trimmedCurve: modify in ccv1:
    // - get type and dbi of basicCurve (ccv1.typ and ccv1.dbi)
    // - get v0 and v1 of trimmedCurve on basicCurve (ccv1.v0 and ccv1.v1)
    CVTRM__basCv__ (&ccv2, &cvBas, ccv1);
    typ_bas = ccv2.typ;
    typDB_bas = AP_typDB_typ (typ_bas);
    dbiBas = ccv2.dbi;

      // printf(" set_obj_dbi-typ_bas=%d dbiBas=%ld\n",typ_bas,dbiBas);
      // DEB_dump_obj__ (typ_bas, cvBas, " cvBas");

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
        UT3D_par_pt__pt_prj_cv (&v0x, NULL, 0, ptx, typ_bas, cvBas, UT_DISP_cv);
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
        UT3D_par_pt__pt_prj_cv (&v1x, NULL, 0, ptx, typ_bas, cvBas, UT_DISP_cv);
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
    // cannot test ccv1->dir; for closed curve - eg circle - can curve cross startPt
    // if(ccv1->dir) {                           // 2019-12-09
    if(v0x > v1x) {
      // curve is reverse
      MEM_swap_2db (&v0x, &v1x);
      MEM_swap_2lg (&ipdb0x, &ipdb1x);
        // printf(" swap v0/1, ipdb0/1:\n");
        // printf("   v0x=%lf v1x=%lf\n",v0x,v1x);
        // printf("   ipdb0x=%ld ipdb1x=%ld\n",ipdb0x,ipdb1x);
    }


    // get PRCV of basecurve
    prc1.typ    = typDB_bas;
    prc1.dbi    = dbiBas;
    prc1.mdli   = mdli;

    // load prc2 = basecurve
    irc = PRCV_DB_load (&prc1);
    if(irc < 0) {
      // not enough memspc for PRCV_rdf__ .. ??
      TX_Error ("PRCV_set_obj_dbi E1-%d-%ld",prc1.typ,prc1.dbi); 
      goto L_err1;   // free prc1
    }
      // PRCV_dump__ (2, &prc1, "PRCV_set_obj_dbi-bc");


    // index last point
    ipe = prc1.ptNr - 1;

    // find previous-parameter v0s and next-parameter v0e for v0x
    x0x = PRCV_set_find (&ipdb0s, &ip0s, &v0s,
                         &ipdb0e, &ip0e, &v0e, &prc1, &v0x);
      // printf("x0x=%d ip0s=%ld ip0e=%ld v0s=%lf v0e=%lf\n",x0x,ip0s,ip0e,v0s,v0e);

    // find previous-parameter v1s and next-parameter v1e for v1x
    x1x = PRCV_set_find (&ipdb1s, &ip1s, &v1s,
                         &ipdb1e, &ip1e, &v1e, &prc1, &v1x);
      // printf("x1x=%d ip1s=%ld ip1e=%ld v1s=%lf v1e=%lf\n",x1x,ip1s,ip1e,v1s,v1e);

    // both values already exist: done.
    if(x0x && x1x) continue;    // 2018-07-24


  
    //----------------------------------------------------------------
    // init PRCV0
    PRCV0.typ  = prc1.typ;
    PRCV0.dbi  = prc1.dbi;
    PRCV0.mdli = prc1.mdli;
    PRCV0.ptNr = 0;                // clear;
      // DEB_dump_obj__ (Typ_PRCV, &PRCV0, "_set_dbo_-PRCV0");

    
    // check / prepare space; at least for additional (prc1.ptNr + 10) points
    siz = PRCV0.ptNr + prc1.ptNr + 10;
    if(PRCV0.siz < siz) {
      // get space
      irc = PRCV_DB_spc_add (PRCV0, siz + PRCV0_INC_PTNR);
      if(irc < 0) goto L_err1;   // free prc1; TX_Error already called
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
      PRCV_set_seg (&PRCV0, typ_bas, cvBas, v0s, v0x, ipdb0s, ipdb0x);
    }

    //----------------------------------------------------------------
    // add v0x bis v0e
      // printf("------------------ add v0x=%lf - v0e=%lf\n",v0x,v0e);
    if(v0x < v0e) {
      if(v0e == v1x) ipdb0e = ipdb1x;
      PRCV_set_seg (&PRCV0, typ_bas, cvBas, v0x, v0e, ipdb0x, ipdb0e);
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
      PRCV_set_seg (&PRCV0, typ_bas, cvBas, v1s, v1x, ipdb1s, ipdb1x);
    }


    //----------------------------------------------------------------
    // add v1x bis v1e
      // printf("------------------ add v1x=%lf - v1e=%lf\n",v1x,v1e);
    if(v1x < v1e) {
      PRCV_set_seg (&PRCV0, typ_bas, cvBas, v1x, v1e, ipdb1x, ipdb1e);
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
      ipdb0x = ipdb1x;        // 2019-12-09
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
      PRCV_set_seg (&PRCV0, typ_bas, cvBas, v0s, v0x, ipdb0s, ipdb0x);
    }

    //----------------------------------------------------------------
    // add v0x to v0e
      // printf("------------------ add v0x=%lf - v0e=%lf\n",v0x,v0e);
    if(v0x < v0e) {
      if(v0e == v1x) ipdb0e = ipdb1x;
      PRCV_set_seg (&PRCV0, typ_bas, cvBas, v0x, v0e, ipdb0x, ipdb0e);
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
      // PRCV_dump__ (2, &PRCV0, "PRCV_set_obj_dbi-L_xxe:");
      // END TESTBLOCK

    // store curve
    PRCV_DB_save (&PRCV0);

    // PRCV_DB_free (&prc1);
    // prc1 = memspc201

  }
  
  goto L_exit;



  //----------------------------------------------------------------
  L_basCv:
    // set PRCV of basecurve
    irc =  PRCV_set_basCv (form, obj, dbi, mdli);
    if(irc < 0) return irc;
    

  //----------------------------------------------------------------
  L_exit:
    PRCV0_REL ();         // release PRCV0

      // TESTBLOCK
      // activate PRCV_DB_dump at end of PRCV_DB_save
      // printf("exit-PRCV_set_obj_dbi =====================\n");
      // if((typ=Typ_CV)&&(dbi == 22L)) {fflush (stdout);exit(-1);}
      // END TESTBLOCK

    return 0;



  //----------------------------------------------------------------
  L_err1:    // error - free prc1
    PRCV_DB_free (&prc1);
    // if((prc1.fTmp == 0)&&(prc1.npt)) free (prc2.npt);
    return -1;

}


//======================================================================
  int PRCV_set_copy (CurvPrcv *prc2, CurvPrcv *prc1, int ips, int ipe) {
//======================================================================
// add points from prc1 from index ips to index ipe into prc prc2
// including ipe

  int     i1, ipos, ptNr;
  Point   *pTab;


  // printf("\nPRCV_set_copy %d - %d\n", ips, ipe);
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


  // printf("\nPRCV_set_seg form=%d vs=%lf ve=%lf ipdbs=%ld ipdbe=%ld\n",form,
         // vs,ve,ipdbs,ipdbe);
  // DEB_dump_obj__ (form, cvBas, "  _set_seg - cvBas ");
  // PRCV_dump__ (2, prc, "  _set_seg-prc-in");
    


  // create a copy of cvBas
  memcpy (cvMod, cvBas, OBJ_SIZ_MAX);
    // DEB_dump_obj__ (form, cvMod, " PRCV_set_seg - cvMod1 ");

  // set limitpoints / modify v0, v1
  UTO_set_ptlim (form, cvMod, NULL, &vs, NULL, &ve);
    // DEB_dump_obj__ (form, cvMod, " PRCV_set_seg - cvMod2 ");


  //----------------------------------------------------------------
  iis = prc->ptNr;                   // startindex
  if(iis > 0) --iis;                 // overwrite last point.

  pTab = &prc->npt[iis];             // add new points from here ..
  tabSiz = prc->siz - iis;           // remaining nr of points
    // printf(" _set_seg-iis=%d tabSiz=%d\n",iis,tabSiz);

  // 2019-05-07 PRCV0_INC_PTNR
  if(tabSiz < 200) PRCV_DB_spc_add (prc, tabSiz + PRCV0_INC_PTNR);


  //----------------------------------------------------------------
  ptNr = 0;
  // create pTab=polygon of curve form/cvMod
  irc = UT3D_npt_obj (&ptNr, pTab, tabSiz, form, cvMod, 1, UT_DISP_cv, 0);
  if(irc < 0) {
    printf("**** PRCV_set_seg UT3D_npt_obj %d\n",irc);
    return -1;
  }
  prc->ptNr = iis + ptNr;

    // TESTBLOCK
    // printf(" iis-neu=%d ptNr=%d\n",iis,ptNr);
    // for(i1=0;i1<ptNr;++i1) DEB_dump_obj__(Typ_PT,&pTab[i1],"pTab[%d]=",i1);
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
    // PRCV_dump__ (2, prc, "ex-_set_seg");
    // if(ipdbe == 20L) {fflush (stdout);exit(-1);}
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
  UTO_par__pt_obj (&vx, ptx, form, cvBas);




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
  int PRCV_set_basCv (int form, void *cvBas, long dbi, int mdli) {
//================================================================
// create PRCV for basicCurve

  int       irc, i1, ptNr, tabSiz, siz;
  // long      ips, ipe;
  double    v0, v1;
  // CurvPrcv  prc;


  // printf("PRCV_set_basCv form=%d dbi=%ld mdli=%d\n",form,dbi,mdli);
  // DEB_dump_obj__ (form, cvBas, " PRCV_set_basCv ");


  //----------------------------------------------------------------
  // get nr of necessary points for bin-cvCut
  ptNr = UT3D_ptNr_obj (form, cvBas, UT_DISP_cv);
  if(ptNr < 2) {TX_Error ("PRCV_set_basCv E2-TODO"); return -1;}

  if(form == Typ_CVBSP) tabSiz = (ptNr * 3) + 12;
  else                  tabSiz = ptNr + 12;
    // printf(" tabSiz=%d\n",tabSiz);


  //----------------------------------------------------------------
  // init space
  PRCV0.typ  = AP_typDB_typ (form);
  PRCV0.dbi  = dbi;
  PRCV0.mdli = mdli;

  // test space
  if(tabSiz > PRCV0.siz) {
    // get space for <ptNr> more points
    irc = PRCV_DB_spc_add (&PRCV0, tabSiz + PRCV0_INC_PTNR);
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
  PRCV_DB_save (&PRCV0);


    // TESTBLOCK
    // printf("ex-PRCV_set_basCv \n");
    // END TESTBLOCK

  return 0;

}

 
//================================================================
  int PRCV_dump_dbo (int mode, int dbTyp, long dbi) {
//================================================================
// PRCV_dump_dbo         dump PRCV of DB-obj (curve)
// no PRC-files for Line, CurvCCV
// Input:
//   mode     0 print short
//            1 display
//            2 print all


  int       irc;
  CurvPrcv  prc;


  printf("PRCV_dump_dbo %d %d %ld\n",mode,dbTyp,dbi);

  // if((dbTyp == Typ_LN)      ||
     // (dbTyp == Typ_CVTRM)) goto L_e1;   hier nicht form !


  prc.typ    = dbTyp;
  prc.dbi    = dbi;
  prc.mdli   = AP_modact_ind;
  prc.siz    = 0;
  prc.spcTyp = MEMTYP_STACK__;  // no expand, no free

  // irc = PRCV_rdf__ (&prc);
  irc = PRCV_DB_load (&prc);
  if(irc < 0) goto L_exit;

  PRCV_dump__ (mode, &prc, "PRCV_dump_dbo");


  L_exit:
  PRCV_DB_free (&prc);

  return 0;

  // L_e1:
    // printf("**** PRCV_dump_dbo I1 %d %ld\n",dbTyp, dbi);
    // return -1;
}


//================================================================
  int PRCV_dump_1 (int ii, CurvPrcv *prc) {
//================================================================
// PRCV_dump_1        dump single record (ind,par,coords of point)

  char   s1[80], s2[32];


  if(ii < 0) {
    printf("---- ptNr=%d siz=%d spcTyp=%d\n", prc->ptNr, prc->siz, prc->spcTyp);
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
// PRCV_dump__            dump a CurvPrcv
// Input:
//   mode     0 print short
//            1 display triangles at all points
//            2 print all
 

  int    i1, iatt;
  char   s1[80];


  printf("-------------------- PRCV_dump__ %s\n",txt);
  printf("  %d ptNr=%d siz=%d spcTyp=%d\n", mode, prc->ptNr, prc->siz, prc->spcTyp);

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
      // APT_disp_SymB (SYM_TRI_S, iatt, &prc->npt[i1]);
      GR_tDyn_symB (&prc->npt[i1], SYM_TRI_S, iatt);
      if(prc->nipt[i1]) GR_tDyn_txiA (&prc->npt[i1], i1, 0);
    }

  }
  
  return 0;

}


//============================================================================
  int PRCV_mtpt_trmCv (MemTab(Point) *mtpa, CurvCCV *ccv1) {
//============================================================================
// PRCV_npt_trmCv        add polygon of trimmedCurve into MemTab(Point)
// see UT3D_npt_obj GR_set_ccv UTO_cv_cvtrm
// Input:
//   ccv1
// Output:
//   mtpa

// static int iNr=0;
  int      irc, i1, ptn;
  long     l1;
  Point    *pta;


  // printf("----------------- \n");
  // printf("PRCV_npt_trmCv ptNr=%d\n",*ptNr);
  // printf("    iNr=%d\n", ++iNr);
  // DEB_dump_obj__ (Typ_MemTab, mtpa, " PRCV_mtpt_trmCv-in");
  // if(mtpa->rNr < 0) exit (-3); // AP_debug__ ("PRCV_mtpt_trmCv");
  // DEB_dump_obj__ (Typ_CVTRM, ccv1, " PRCV_npt_trmCv");


  PRCV0_OCC ();         // occupy PRCV0


  // get PRCV0 =  trimmed-curve ccv1
  irc = PRCV_get_dbo_add_tc (&PRCV0, ccv1);
  if(irc < 0) goto L_exit;
    // printf(" PRCV_mtpt_trmCv-1 irc= %d ptNr = %d\n",irc,PRCV0.ptNr);
    // PRCV_dump__ (2, &PRCV0, "npt_trmCv-1");


  // get nr of free points in mtpa
  ptn = MEMTAB_RFREE(mtpa);
     // printf(" PRCV_mtpt_trmCv-ptn %d\n",ptn);


  // add PRCV0.ptNr from PRCV0.npt into mtpa; realloc if necessary
  i1 = MemTab_add (mtpa, &l1, PRCV0.npt, PRCV0.ptNr, 0);
  if(i1 < 0) {
    TX_Error("PRCV_mtpt_trmCv EOM");
    return -1;
  }
 

  L_exit:
  PRCV0_REL ();         // release PRCV0


    // TESTBLOCK
    // DEB_dump_obj__ (Typ_MemTab, mtpa, " ex-PRCV_mtpt_trmCv");
    // DEB_dump_nobj__ (Typ_PT, *ptNr, pta, "ex-PRCV_npt_trmCv");
    // GR_tDyn_npt__ (*ptNr, *pta, SYM_STAR_S, 2);
    // END TESTBLOCK

  return irc;

}


//============================================================================
  int PRCV_npt_trmCv (Point *pta, int *ptNr, CurvCCV *ccv1) {
//============================================================================
// PRCV_npt_trmCv        get polygon of trimmedCurve.
// see UT3D_npt_obj GR_set_ccv UTO_cv_cvtrm
// Input:
//   ccv1
// Output:
//   pta,ptNr

// static int iNr=0;
  int      irc;


  // printf("PRCV_npt_trmCv ptNr=%d\n",*ptNr);
  // printf("    iNr=%d\n", ++iNr);
  // DEB_dump_obj__ (Typ_CVTRM, ccv1, " PRCV_npt_trmCv");


  PRCV0_OCC ();         // occupy PRCV0

  // add trimmed-curve ccv1 to PRCV0
  irc = PRCV_get_dbo_add_tc (&PRCV0, ccv1);
    // printf(" _npt_trmCv-irc = %d ptNr = %d\n",irc,PRCV0.ptNr);
    // PRCV_dump__ (2, &PRCV0, "npt_trmCv-1");


  // copy points -> pta
  if(PRCV0.ptNr > *ptNr) { TX_Error("PRCV_npt_trmCv EOM"); return -1; }
  if(!irc) { 
    memcpy (pta, PRCV0.npt, PRCV0.ptNr * sizeof(Point));
    *ptNr = PRCV0.ptNr;
  }

  PRCV0_REL ();         // release PRCV0


      // TESTBLOCK
      // DEB_dump_nobj__ (Typ_PT, *ptNr, pta, "ex-PRCV_npt_trmCv");
      // GR_tDyn_npt__ (*ptNr, *pta, SYM_STAR_S, 2);
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
// PRCV_test_set       create PRCV for DB-obj (typ/dbi)
//   not necessary for Lines
 
  int   iAtt;
  long  dli;



  printf("################################################ \n");
  printf("PRCV_test_set %d %ld\n", typ, dbi);

  // create PRCV for typ/dbi
  // mode: 1=write_file; 2=read_file; 4=display
  PRCV_set_dbo__ (typ, dbi);

    // TESTBLOCK
    PRCV_dump_dbo (2, typ, dbi);
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

  irc = PRCV_npt_dbo__ (&pta, &ptNr, typ, dbi, AP_modact_ind);
  if(irc < 0) return -1;
    printf(" ex-PRCV_npt_dbo__-ptNr = %d\n",ptNr);


    // TESTBLOCK
    // GR_tDyn_pcv (pta, ptNr, 11);
    GR_tDyn_npti (ptNr, pta, SYM_STAR_S, ATT_COL_RED, ATT_COL_YELLOW);
    // END TESTBLOCK

  return 0;

}


//================================================================
  int PRCV_test_disp (CurvPrcv *prc) {
//================================================================
// display polygon in CurvPrcv


  int     att;


  att = 11;  // see ~/gCAD3D/cfg/ltyp.rc

  GR_tDyn_pcv (prc->npt, prc->ptNr, att);

  // GR_tDyn_npti (prc->ptNr, prc->npt, SYM_TRI_S, ATT_COL_RED, ATT_COL_RED);

  return 0;

}


#endif

// EOF
