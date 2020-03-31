// ../ut/ut_cntf.c
/*
 *
 * Copyright (C) 2017 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
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
replacing ../xa/xa_cont.c
  ..

-----------------------------------------------------
*/
#ifdef globTag
void CNTF(){}
#endif
/*!
\file  ../ut/ut_cntf.c
\brief create-countour-functions CNTF_
\code
=====================================================
List_functions_start:

CNTF_init__
CNTF_add__
CNTF_exit__

CNTF_cvco_0_pt
CNTF_cvco_0_lfig
CNTF_cvco_pt_pt
CNTF_cvco_pt_lfig
CNTF_cvco_lfig_pt
CNTF_cvco_lfig_lfig

CNTF_add_ln
CNTF_out_old
CNTF_out_cvtrm

CNTF_rev__
CNTF_ck_nearPts
CNTF_ck_limPt
CNTF_normalPt
CNTF_int__

CNTF_dump
CNTF_test__
CNTF_test_1

List_functions_end:
=====================================================

\endcode *//*----------------------------------------

see also CVTRM__otb__         create trimmed-curve from object-table


CNTF creates contour (trimmed-curves) from user-selections.

*/



#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                         // for ...


// #include <errno.h>
// #include <dlfcn.h>           // Unix: dlopen


#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif



#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_geo_const.h"        // UT3D_CCV_NUL
#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/func_types.h"               // SYM_..
#include "../ut/ut_ox_base.h"          // OGX_SET_INDEX
#include "../ut/ut_math.h"             // IS_NAN
#include "../ut/ut_memTab.h"           // MemTab

#include "../xa/xa_msg.h"              // MSG_*
#include "../xa/xa_mem.h"              // memspc51, mem_cbuf1




// contour-finder-object
// pend       0=obj is not pending, already out (use only pte). Else pending.
typedef struct {double v0, v1; long dbi, ip0, ip1; Point pts, pte;
                int typ, pend; char obj[OBJ_SIZ_MAX], dir, clo, cer;}      cfo;



// local, unexported:
  static int      ccaSiz, ccNr;
  static CurvCCV  *cca;
  static cfo      old, new;
  static double   tol_cv, tol_pt;





//================================================================
#ifdef OFFLINE
//================================================================


//================================================================
  int CNTF_test__ () {
//================================================================
// testPrg for CNTF_cvcomp

#define SIZ_CCA  100
// #define SIZ_PTNR 1000

  int      irc, i1, cvNr, ptNr;
  long     dli;
  Point    *pta;
  ObjGX    oxi;
  CurvCCV  cva[SIZ_CCA];


  printf(" CNTF_test__\n");


  // oxi.typ = TYP_FuncInit;
  // cvNr = SIZ_CCA;
  // CNTF_add__ (&cvNr, cva, &oxi, 0, 0);
  CNTF_init__ (cva, SIZ_CCA, UT_TOL_cv, UT_TOL_pt);


  // CNTF_test_1 (Typ_LN, 22, 0, 0);
  // CNTF_test_1 (Typ_LN, 21, 0, 0);
  // CNTF_test_1 (Typ_LN, 20, 1, 0);
  CNTF_test_1 (Typ_CV, 20, 0, 0);
  CNTF_test_1 (Typ_PT, 21, 0, 0);
  // CNTF_test_1 (Typ_PT, 20, 0, 0);
  // CNTF_test_1 (Typ_PT, 22, 0, 0);
  // CNTF_test_1 (Typ_PT, 23, 0, 0);
  // CNTF_test_1 (Typ_PT, 23, 0, 0);


  // oxi.typ = TYP_FuncExit;  // finish pending obj's
  // CNTF_add__ (&cvNr, cva, &oxi, 0, 0);
  CNTF_exit__ (&cvNr);


  //================================================================
  // dump ccv's
  printf("======================== cvNr=%d ==============\n",cvNr);

  for(i1=0; i1<cvNr; ++i1) 
    DEB_dump_obj__ (Typ_CVTRM, &cva[i1], "cca[%d]",i1);

  printf("================ end CNTF =====================\n");



  //================================================================
  // store ccv
  DB_StoreCvCCV (30L, cva, cvNr);


  //================================================================
  // display ccv's using PRCV
  irc = _PRCV_npt_dbo__ (&pta, &ptNr, Typ_CVTRM, 30L, -1);
    printf(" ptNr=%d irc=%d\n",ptNr,irc);
  if(irc < 0) return -1;

  for(i1=0; i1<ptNr; ++i1) 
    DEB_dump_obj__ (Typ_PT, &pta[i1], "pta[%d]",i1);

  dli = -1L;
  GL_DrawPoly (&dli, Typ_Att_hili, ptNr, pta);



  //================================================================
  // display ccv's (without using PRCV)
  // for(i1=0; i1<cvNr; ++i1) 
    // GR_Disp_obj (Typ_CVTRM, &cva[i1], 8, 0);


  
  return 0;

}


//================================================================
  int CNTF_test_1 (int typ,  long dbi, int isr, int imod) {
//================================================================
// isr   revers; 0=not; else yes
 
  int      cvNr;
  ObjGX    oxi;

  printf(" CNTF_test_1 %d %ld\n",typ,dbi);

  OGX_SET_INDEX (&oxi, typ, dbi);
  CNTF_add__ (&oxi, isr, imod);

  return 0;

}


//================================================================
#endif
//================================================================
 



//============================================================================
  int CNTF_init__ (CurvCCV *ccva, int siz_cca, double cv_tol, double pt_tol) {
//============================================================================


  // printf("CNTF_init__ siz=%d %lf %lf\n",siz_cca,cv_tol,pt_tol);


  old.typ  = TYP_FuncInit;
  old.pend = 0;

  ccaSiz = siz_cca;
  cca = ccva;
  ccNr = 0;

  tol_cv = cv_tol;
  tol_pt = pt_tol;

  return 0;

}

 
//================================================================
  int CNTF_add__ (ObjGX *oxi, int isr, int imod) {
//================================================================
// CNTF_add__    compute connection old-obj - new-obj; old-obj out.
//
/// \code
/// process next obj (add obj's to output)
/// Input:
///   oxi        next DB-obj to process;
///   isr        revers oxi; 0=not; else yes
///   imod       solution-nr (index of intersection-point if != 1)
/// Output:
///   ccvNr      nr of finished records in cca
///   ccva[]     trimmed-curves
///   retCod     0 = OK
///             -2 = degenerated (lt UT_TOL_cv; gt UT_TOL_pt)
///             -3 = degenerated (lt UT_TOL_pt)
///
/// \endcode
 
//----------------------------------------------------------------
// PT > lFig:
// - test endpoints: fit = connect
// - test normal; yes: limit lFig, insert connectionLine
// - no normal: connect to nearest point (start|endPoint)
// lFig > lFig:
// - test endpoints: fit = connect
// - test intersection: yes: limit both
// - no intersection: test normal; yes: limit both, insert connectionLine
// - no normal: connect to nearest point (start|endPoint)
// lFig > PT:
// - test endpoints: fit = connect
// - test normal; yes: limit lFig, insert connectionLine
// - no normal: connect to nearest point (start|endPoint)


  int           irc, oNr;
  char          oid[256];
  void          *vp1;


  // TESTBLOCK
  // printf("============================= CNTF_add__ isr=%d imdo=%d\n",isr,imod);
  // DEB_dump_obj__ (Typ_ObjGX, oxi, "CNTF_add__");
  // printf(" isr=%d old.typ=%d newTyp=%d\n",isr,old.typ,new.typ);
  // CNTF_dump (&old, "CNTF_add__-old");
  // END TESTBLOCK


  // init new
  new.cer = 0;

  // get new from input-obj

  // APT_prim_seg = imod
  APT_set_primSeg (imod);

  // get db-typ & index out of oxi
  OGX_GET_INDEX (&new.typ, &new.dbi, oxi);

  // get DB-obj
  new.typ = DB_GetObjDat (&vp1, &oNr, new.typ, new.dbi);
    // printf(" newTyp=%d\n",new.typ);
  if(new.typ <= Typ_Error) {TX_Print("- error; object must be circle .."); return -1;}

  // make a copy of newObj
  memcpy (new.obj, vp1, OBJ_SIZ_MAX);


  // get start- endpoint
  if(new.typ == Typ_PT) {
    new.pts = *(Point*)new.obj;
    new.pte = *(Point*)new.obj;
    new.ip0 = new.dbi;
    new.ip1 = new.dbi;
    new.clo = 1; // no
    goto L_sta_2;

  } else {
    // get startPt and endtPt of newObj
    irc = UT3D_ptvcpar_std_obj (&new.pts,NULL,&new.v0, Ptyp_start,new.typ,new.obj);
    irc = UT3D_ptvcpar_std_obj (&new.pte,NULL,&new.v1, Ptyp_end, new.typ, new.obj);
    new.ip0 = 0;
    new.ip1 = 0;
  }
    // printf(" _add__-new.v0=%lf new.v1=%lf\n",new.v0,new.v1);

  


  // get direction and closed;
  if(new.typ == Typ_LN) {
    // check for degenerated (length < UT_TOL_cv)
    new.clo = UT3D_ln_ck_degen (new.obj);
    new.dir = 0; // fwd

  } else {
    // check if closed or degenerated; clo: 0=yes, 1=not, -2=degen
    // new.clo = UTO_cv_ck_clo (new.typ, new.obj);  // 0=YES,1=NO
    UTO_cv_ck_dir_clo (&new.dir, &new.clo, new.typ, new.obj);
  }


  // test degenerated
  if(new.clo < -1) {
    // degenerated lfig;
      // printf(" degen:%d\n",newClo);
    APED_oid_dbo_sm (oid, sizeof(oid), new.typ, new.dbi);
    // TX_Print("skip degenerated object %s %s",oid,AP_modact_nam);
    TX_Print("**** skip degenerated object %s",oid);
    // return new.clo;
    return 0;
  }


  // fix reverse
  if(isr) CNTF_rev__ (&new); 


  L_sta_2:

    // TESTBLOCK
    // DEB_dump_obj__ (Typ_PT, &new.pts, " new.pts");
    // DEB_dump_obj__ (Typ_PT, &new.pte, " new.pte");
    // printf(" ccNr=%d v0=%lf v1=%lf dir=%d clo=%d\n",
            // ccNr,new.v0,new.v1,new.dir,new.clo);
    // END TESTBLOCK



  //================================================================
  // process new-obj
  if(old.typ == TYP_FuncInit) {
    if(new.typ == Typ_PT) CNTF_cvco_0_pt ();
    else                  CNTF_cvco_0_lfig ();

  } else if(old.typ == Typ_PT) {
    if(new.typ == Typ_PT) CNTF_cvco_pt_pt ();
    else                  CNTF_cvco_pt_lfig ();

  } else {   // old = lfig
    if(new.typ == Typ_PT) CNTF_cvco_lfig_pt ();
    else                  CNTF_cvco_lfig_lfig (imod);
  }




  //================================================================
  L_exit:

    // TESTBLOCK
    // if(old.pend) CNTF_dump (&old, "ex-CNTF_add__-old");
    // printf("ex-CNTF_add__ \n");
    // END TESTBLOCK

  return 0;

}


//================================================================
  int CNTF_cvco_0_pt () {
//================================================================
// ../xa/xa_cvcomp.c:690
 
  // printf("CNTF_cvco_0_pt \n");

  // copy
  old = new;

  // nothing pending
  old.pend = 0;

  return 0;

}

//================================================================
  int CNTF_cvco_0_lfig () {
//================================================================

  // printf("CNTF_cvco_0_lfig \n");

  // copy
  old = new;

  // lfig pending
  old.pend = 1;

  return 0;

}

//================================================================
  int CNTF_cvco_pt_pt () {
//================================================================
// ../xa/xa_cvcomp.c|877

  CurvCCV    cc1;


  // printf("CNTF_cvco_pt_pt \n");

  cc1 = UT3D_CCV_NUL;   // empty

  old.pte = new.pts;

  // create line old.pts - old.pte
  CNTF_add_ln (&old.dbi, &new.dbi);

  // set old = new (keep pte)
  old = new;

  // nothing pending
  old.pend = 0;

  return 0;

}

//================================================================
  int CNTF_cvco_pt_lfig () {
//================================================================
// PT > lFig:
// - test endpoints: fit = connect
// - test normal; yes: limit lFig, insert connectionLine
// - no normal: connect to nearest point (start|endPoint)
// was ../xa/xa_cvcomp.c:986


  int    irc;
  double par1;
  Point  pt1;


  // printf("CNTF_cvco_pt_lfig \n");


  // test endpoints (old.pte - new.pts; old.pte - new.pte);
  irc = CNTF_ck_limPt (&old.pte);
  //  0: old.pte == new.pts; old=new; Done.
  //  1: old.pte == new.pte; reverse new; old=new; Done.
  // -1: no connection 
  if(irc == 0) {
    if(new.ip0 == 0) new.ip0 = old.dbi;  // old.pte == new.pts
    old = new;          // skip the point;
    old.pend = 1;       // set lfig = pending
    return 0;
  }
  if(irc == 1) {        // old.pte == new.pte; reverse new; old=new; Done.
    CNTF_rev__ (&new);  // reverse new;
    if(new.ip0 == 0) new.ip0 = old.dbi;  // old.pte == new.pts
    old = new;          // skip the point;
    old.pend = 1;       // set lfig = pending
    return 0;
  }



  // get normal-point (project old.pte onto new.obj)
  irc = CNTF_normalPt (&pt1, &old.pte, &par1, new.typ, new.obj);
    // DEB_dump_obj__ (Typ_PT, &pt1, " _normalPt-pt1-irc=%d",irc);
  // circ & elli: parameter nmust refer to the parent-circle !
  // if(new.cer) par1 = 1. - par1;
  // 0  pt1 == old.pte; point is on new.obj. old=new; Done.
  // 1  pt1 is on new.obj. out LN old.pte-pt1; new.pts=pt1; old=new; Done.
  // -1: no connection 
  if(irc == 0) {        // old.pte == new.pts = on obj.
    new.pts = pt1;
    new.ip0 = old.dbi;
    new.v0 = par1;      // startpoint
    old = new;          // skip the point;
    old.pend = 1;       // set lfig = pending
    return 0;
  }
  if(irc == 1) {        // pt1 is on new.obj. old.pte is not on obj.
    old.pte = pt1;      // endPt line
    CNTF_add_ln (&old.dbi, &new.ip0);  // out LN old.pte-pt1
    new.pts = pt1;      // startpoint on lfig new
    new.v0  = par1;     // startpoint
    old = new;          // skip the point;
    old.pend = 1;       // set lfig = pending
    return 0;
  }



  // connect to nearest point (start|endPoint) with connectionLine
  // test if dist (old.pte - new.pts)||( old.pte - new.pte) is less
  irc = CNTF_ck_nearPts (); 
  // 0 = dist old.pte-new.pts < old.pte-new.pte:
  //     out line old.pte-new.pts; old=new; Done.
  // 1 = dist old.pte-new.pts > old.pte-new.pte:
  //     out line old.pte-new.pte; reverse new; old=new; Done.
  if(irc == 1) {        // old.pte-new.pts > old.pte-new.pte
    CNTF_rev__ (&new);  // reverse new;
  }

  if(new.ip0 == 0) new.ip0 = DB_StorePoint (-1L, &new.pts);
  CNTF_add_ln (&old.dbi, &new.ip0);  // out LN old.pte-new.pts
  old = new;          // skip the point;
  old.pend = 1;       // set lfig = pending




  return 0;

}


//================================================================
  int CNTF_cvco_lfig_pt () {
//================================================================
// lFig > PT:
// - test endpoint
// - test normal; yes: limit lFig, insert connectionLine
// - no normal: connect to endPoint
// was ../xa/xa_cvcomp.c:896

  int     irc;
  double  d_oe_ns, par1;
  Point   pt1;


  // printf("CNTF_cvco_lfig_pt \n");
  // DEB_dump_obj__ (Typ_PT, &old.pte, "  _lfig_pt-old-pte");
  // DEB_dump_obj__ (Typ_PT, &new.pts, "  _lfig_pt-new-pts");



  // test endpoint (old.pte - new.pts); yes: out old; old=new. Done.
  d_oe_ns = UT3D_lenB_2pt (&old.pte, &new.pts);
  if(d_oe_ns < tol_pt) {
    old.ip1 = new.dbi;
    CNTF_out_old ();  // out old;
    old = new;
    old.pend = 0;     // nothing pending
    return 0;
  }
 



  // get normal-point; (pt1 = project new.pts onto old.obj)
  irc = CNTF_normalPt (&pt1, &new.pts, &par1, old.typ, old.obj);
  // if(old.cer) par1 = 1. - par1;
    // DEB_dump_obj__ (Typ_PT, &pt1, " pt1-irc=%d",irc);
  // 0  pt1 == new.pts; old.pte = pt1. Out old. old=new; Done.
  // 1  old.pte = pt1. Out old. Out LN old.pte-pt1; old=new; Done.
  // -1: no connection
  if(irc == 0) {      // old.pte == new.pts = on obj.
    old.pte = pt1;
    old.ip1 = new.dbi;
    old.v1  = par1;   // endpoint
    CNTF_out_old ();  // out old;
    old = new;        // skip the point;
    old.pend = 0;     // nothing pending
    return 0;
  }
  if(irc == 1) {
    old.pte = pt1;    // modify endPt of old
    old.v1  = par1;
    CNTF_out_old ();  // out old;
    old.pts = old.pte;
    old.pte = new.pts;
    CNTF_add_ln (&old.ip1, &new.dbi);  // out LN = old.pts-old.pte
    old = new;        // skip the point;
    old.pend = 0;     // nothing pending
    return 0;
  }



  // connect to old.pte with connectionLine
  //    out old; out LN old.pte-new.pts; old=new; Done.
  CNTF_out_old ();  // out old;
  CNTF_add_ln (&old.ip1, &new.dbi);  // out LN old.pte-pt1
  old = new;        // skip the point;
  old.pend = 0;     // nothing pending

 
  return 0;

}


//================================================================
  int CNTF_cvco_lfig_lfig (int imod) {
//================================================================
/// \code
/// lFig > lFig:
/// - test endpoints: fit = connect
/// - test intersection: yes: limit both
/// - no intersection: test normal; yes: limit both, insert connectionLine
/// - no normal: connect to nearest point (start|endPoint)
/// Input:
///   imod       solution-nr (index of intersection-point if != 1)
/// was ../xa/xa_cvcomp.c:1057
/// \endcode


  int       irc, ir1;
  double    par1, par2;
  Point     pt1;


  // printf("CNTF_cvco_lfig_lfig imod=%d ccNr=%d\n",imod,ccNr);
  // DEB_dump_obj__ (Typ_PT, &old.pts, " old.pts ");
  // DEB_dump_obj__ (Typ_PT, &old.pte, " old.pte ");
  // DEB_dump_obj__ (Typ_PT, &new.pts, " new.pts ");
  // DEB_dump_obj__ (Typ_PT, &new.pte, " new.pte ");


  // 0: old.pte == new.pts;  1: old.pte == new.pte;
  // test if points connected; -1=not
  irc = CNTF_ck_limPt (&old.pte);
    // printf(" ck_limPt1-irc=%d\n",irc);

  // only if 1. and 2. obj. present (later do not reverse old):
  if(ccNr) goto L_1;
  
  if(irc == 0) goto L_1;

  // test (old.pts - new.pts / old.pts - new.pte);
  ir1 = CNTF_ck_limPt (&old.pts);
    // printf(" _lfig_lfig-ck_limPt %d\n",ir1);

  if(ir1 < 0) goto L_1; // no connection
  if(ir1 == 0) {        // old.pts=new.pts: reverse old ..
    CNTF_rev__ (&old);  // reverse old;
    CNTF_out_old ();    // out old;
    new.ip0 = old.ip1;  // old.ipe = new.ips
    old = new;
    old.pend = 1;       // set lfig = pending
    goto L_exit;
  }

  if(ir1 == 1) {        // old.pts=new.pte: reverse old & new ..
    CNTF_rev__ (&old);  // reverse old;
    CNTF_out_old ();    // out old;
    CNTF_rev__ (&new);  // reverse old;
    new.ip0 = old.ip1;  // old.ipe = new.ips
    old = new;
    old.pend = 1;       // set lfig = pending
    goto L_exit;
  }



  // test endpoints (old.pte - new.pts; old.pte - new.pte);
  L_1:
  // if(old.pte == new.pts): out old; old=new; Done.
  // if(old.pte == new.pte): reverse new; old=new; Done.
  //  0: old.pte == new.pts; old=new; Done.
  //  1: old.pte == new.pte; reverse new; old=new; Done.
  // -1: no connection 
  if(irc == 0) {        // old.pte == new.pts
    CNTF_out_old ();    // out old;
    new.ip0 = old.ip1;
    old = new; 
    old.pend = 1;       // set lfig = pending
    goto L_exit;
  }
  if(irc == 1) {        // old.pte == new.pte; reverse new; old=new; Done.
    CNTF_out_old ();    // out old;
    CNTF_rev__ (&new);  // reverse new;
    new.ip0 = old.ip1;
    old = new;          // skip the point;
    old.pend = 1;       // set lfig = pending
    goto L_exit;
  }





  // test intersection: yes: limit both
  L_int0:
  // get pt1 = intersectionpoint old-new;
  // get par1 = parameter on old; par2 = parameter on new
  irc = CNTF_int__ (&pt1, &par1, &par2, imod);
    // printf(" f-CNTF_int__-rc=%d pt1=%f,%f,%f par1=%f par2=%f\n",irc,
                                // pt1.x,pt1.y,pt1.z, par1, par2);


    // TESTBLOCK
    // if(isnan(pt1.x)) AP_debug__  ("CNTF_cvco_lfig_lfig-1");
    // END TESTBLOCK



  // if(old.cer) par1 = 1. - par1;
  // if(new.cer) par2 = 1. - par2;
  if(irc == 0) {
    // 0: old.pte=intPt; out old; new.pts=intPt; old=new; Done
    old.pte = pt1;
    old.v1  = par1;
    CNTF_out_old ();
    new.pts = pt1;
    new.ip0 = old.ip1;
    new.v0  = par2;
    old = new; 
    old.pend = 1;       // set lfig = pending
    goto L_exit;
  }


/*
  // if old == first obj: test also (old.pts - new.pts; old.pts - new.pte);
  //   if connection: reverse_old
  if(ccNr > 0) goto L_int0;       // only for 1. obj
  irc = CNTF_ck_limPt (&old.pts); // test (old.pts - new.pts; old.pts - new.pte);
    printf(" ck_limPt2-irc=%d\n",irc);
  if(irc == 0) {
    // 0: old.pts == new.pts; reverse old; out old; ..
    CNTF_rev__ (&old);  // reverse old;
    CNTF_out_old ();
    new.ip0 = old.ip1;
    old = new; 
    old.pend = 1;       // set lfig = pending
    return 0;
  }
  if(irc == 1) {
    // 1: old.pts == new.pte; reverse old; out old; reverse new; ..
    CNTF_rev__ (&old);  // reverse old;
    CNTF_out_old ();    // out old;
    CNTF_rev__ (&new);  // reverse new;
    new.ip0 = old.ip1;
    old = new;          // skip the point;
    old.pend = 1;       // set lfig = pending
    return 0;
  }
*/

// TODO:
  // get normal-point; if normal-point:
  //   old.pte=normalPtOld; out old; out LN normalPtOld-normalPtNew;
  //   new.pts=normalPtNew; old=new; Done.



  // connect to nearest point (start|endPoint) with connectionLine
  // test endpoints (old.pte - new.pts; old.pte - new.pte);
  irc = CNTF_ck_nearPts (); 
  // 0 = dist old.pte-new.pts < old.pte-new.pte:
  //     out line old.pte-new.pts; old=new; Done.
  // 1 = dist old.pte-new.pts > old.pte-new.pte:
  //     out line old.pte-new.pte; reverse new; old=new; Done.
  if(irc == 1) {        // old.pte-new.pts > old.pte-new.pte
    CNTF_rev__ (&new);  // reverse new;
  }

  CNTF_out_old ();      // out old
  if(new.ip0 == 0) new.ip0 = DB_StorePoint (-1L, &new.pts);
  CNTF_add_ln (&old.ip1, &new.ip0);  // out LN old.pte-new.pts;
  old = new;
  old.pend = 1;         // set lfig = pending

  L_exit:

    // TESTBLOCK
    // CNTF_dump (&old, "ex-CNTF_cvco_lfig_lfig old:");
    // printf("ex-CNTF_cvco_lfig_lfig \n");
    // END TESTBLOCK
 

  return 0;

}


//================================================================
  int CNTF_rev__ (cfo *cf1) {
//================================================================
// change direction of new - obj.

  // printf("CNTF_rev__ \n");
  // CNTF_dump (cf1, "CNTF_rev__-in");


  // change direction
  cf1->dir = ICHG01(cf1->dir);

  // swap v0/v1, ip0/ip1
  MEM_swap_2db (&cf1->v0, &cf1->v1);
  MEM_swap_2lg (&cf1->ip0, &cf1->ip1);

  // swap start/endpoint
  MEM_swap__ (&cf1->pts, &cf1->pte, sizeof(Point));

  // do not change direction of basic-curve
  //   (parameters must be computed on basic-curve)
  // // change direction of cf1
  // UTO_stru_inv (cf1->typ, cf1->obj);

  // circ & elli: parameter must refer to the parent-circle !
  // if((cf1->typ == Typ_CI)||(cf1->typ == Typ_CVELL)) cf1->cer = 1;


    // CNTF_dump (cf1, "ex-CNTF_rev__");

  return 0;

}

 
//================================================================
  int CNTF_dump (cfo *cf1, char *txt) {
//================================================================

  printf("------ CNTF_dump %s\n",txt);

  // DEB_dump_obj__ handles TYP_FuncInit = open file !
  if(TYP_IS_FNC(cf1->typ)) {
    goto L_pri;
  }


  DEB_dump_obj__ (cf1->typ, cf1->obj," obj");


  L_pri:
  printf(" typ=%d dbi=%ld v0=%lf v1=%lf ip0=%ld ip1=%ld pend=%d dir=%d clo=%d\n",
         cf1->typ, cf1->dbi, cf1->v0, cf1->v1,
         cf1->ip0,cf1->ip1,cf1->pend,cf1->dir,cf1->clo);
  DEB_dump_obj__ (Typ_PT, &cf1->pts,"  CNTF_dump-pts");
  DEB_dump_obj__ (Typ_PT, &cf1->pte,"  CNTF_dump-pte");

  return 0;

}

 
//================================================================
  int CNTF_ck_nearPts () {
//================================================================
// test if dist (old.pte - new.pts)||( old.pte - new.pte) is less
// Output:
//   retcod   0 = dist old.pte-new.pts < old.pte-new.pte:
//            1 = dist old.pte-new.pts > old.pte-new.pte:
 

  double  d_oe_ns, d_oe_ne;

  d_oe_ns = UT3D_lenq_PtPt (&old.pte, &new.pts);
  d_oe_ne = UT3D_lenq_PtPt (&old.pte, &new.pte);

  if(d_oe_ns < d_oe_ne) return 0;

  return 1;

}


//================================================================
  int CNTF_ck_limPt (Point *ptx) {
//================================================================
// test endpoints (old.pte - new.pts; old.pte - new.pte);
// Output-retCod:
//  0: old.pte == new.pts; old=new; Done.
//  1: old.pte == new.pte; reverse new; old=new; Done.
// -1: no connection 

  double  d_oe_ns, d_oe_ne;

  // get longest dist
  // d_oe_ns = dist -> new.pts
  d_oe_ns = UT3D_lenB_2pt (ptx, &new.pts);
  // d_oe_ne = dist -> new.pte
  d_oe_ne = UT3D_lenB_2pt (ptx, &new.pte);

  // test smaller dist for equal point
  if(d_oe_ns < tol_pt) return 0;
  if(d_oe_ne < tol_pt) return 1;
  return -1;

}


//==============================================================================
  int CNTF_normalPt (Point *pt2, Point *pt1, double *par, int typ, void *obj) {
//==============================================================================
// CNTF_normalPt      get normal-point and its parameter onto curve
//                    pt2 = project pt1 onto obj;
// Input:
//   pt1        project pt1 onto (typ,obj)
//   typ,obj    curve
// Output:
//   pt2
//   par        parameter of pt1 on curve; vTyp=0 (see INF_struct_par)
//   retCod:    0  pt1 == pt2; point is on new.obj
//              1  pt2 is on new.obj;
//             -1  projectionpoint outside new.obj
//             -2  internal ERR

  int    irc;
  double d1;


  // DEB_dump_obj__ (Typ_PT, pt1, "CNTF_normalPt ");
  // DEB_dump_obj__ (typ, obj, "   obj ");


  // irc = APT_decode_cvco_prj1 (&ptAct, &d_oe_ne,
                              // &ptOld, &newCC, newTyp, newObj, imod);
  irc = UPRJ_def__ (typ, obj, 1, NULL);
  // if(irc < 0) return -2;

  // get pt2 = projection of pt1 onto obj
  irc = UPRJ_app_pt (pt2, pt1);
  if(irc < 0) return -1;

  // get par = parameter of pt2 on obj
  irc = UT3D_par_pt__pt_prj_cv (par, NULL, 0, pt2, typ, obj, UT_DISP_cv);
    // DEB_dump_obj__ (Typ_PT, pt2, " _normalPt-pt2");
    // printf(" _normalPt-irc=%d par1=%lf\n",irc,*par);

  // get d1 = estimated length pt1 - pt2
  d1 = UT3D_lenB_2pt (pt2, pt1);
  if(d1 < tol_pt) return 0;

  return 1;

}


//===================================================================
  int CNTF_int__ (Point *ptx, double *par1, double *par2, int imod) {
//===================================================================
// intersection old-new

// compute intersectionPoints oldObj-newObj; select point nr imod
// Input:
//   imod     nr (0=first) of point to return
// Output:
//   ptx      intersectionpoint
//   par1     parameter of ptx on old-obj
//   par2     parameter of ptx on new-obj
//   retCod   0  - OK; ptx,par1 has resulting intersectionPoint
//            -1 - no intersectionPoint exists;

#define TABSIZ 20

  int    i1, irc, iClo, pNr;
  double va[TABSIZ];
  Point  pa[TABSIZ], ptAct;
  ObjGX  oxo, oxn;
  Memspc wrkSeg;


  // printf("CNTF_int__ %d\n",imod);


  //----------------------------------------------------------------
  // UME_alloc_tmp (&wrkSeg, 2000000);  // 2015-01-04; crash in MS !
  i1 = 2000000; // 2MB
  irc = UME_malloc (&wrkSeg, i1, i1/2);
  if(irc < 0) return -1;
    // UME_dump (&wrkSeg, "wrkSeg");

  // obj -> ObjGX
  OGX_SET_OBJ (&oxo, old.typ, old.typ, 1, old.obj);
  OGX_SET_OBJ (&oxn, new.typ, new.typ, 1, new.obj);

  irc = UTO_npt_int_2ox (&pNr, pa, va, TABSIZ, &oxo, &oxn, &wrkSeg);
    // printf(" ex npt_int_2ox irc=%d pNr=%d\n",irc,pNr);
  if(irc < 0) goto L_exit;
  if(pNr <= 0) {
      // printf("APT_decode_cvco_int no solution - %d_%d \n",typ1,typ2);
    irc = -1;
    goto L_exit;
  }

  // iMaxSol = pNr;  // keep nr of solutions ..
  APT_set_modMax (pNr);


  // select index of intersectionpoint
  if(imod) {
    i1 = ILIM0X (imod, pNr-1);
  } else {
    i1 = CNTF_selPt (pNr, pa, va);
  }

  // get intersectionpoint and its parameter;
  *ptx = pa[i1];
  *par1 = va[i1];    // parameter on old

  // get parameter for intersectionpoint (ptx) on new obj
  UT3D_par_pt__pt_prj_cv (par2, NULL, 0, ptx, new.typ, new.obj, UT_DISP_cv);


  irc = 0;


  L_exit:
  UME_free (&wrkSeg);

    // printf("ex CNTF_int__ %d par1=%lf par2=%lf\n",irc,*par1,*par2);
    // DEB_dump_obj__ (Typ_PT, ptx, " ptx ");
    // if(IS_NAN(*par2)) AP_debug__  ("CNTF_int__-1");

  return irc;

}


//================================================================
  int CNTF_selPt (int iNr, Point *pta, double *va) {
//================================================================
// Input:
//   iNr      nr of Intersectionpoints.
//   *pta     Intersectionpoints.
//   modFlg G user-selected index of Intersectionpoint; -1=undefined.
//   o1Dat  G new_obj o1Typ
//   o2Dat  G old_obj o2Typ
// Output:
//   retCod   index of selected point in pta, va

// was AP_cont_intPt
// find ii = nearest point to actPos; but if point == actPos skip this point.


  int   ii, i1;


  // printf("CNTF_selPt %d\n",iNr);
  // DEB_dump_obj__ (Typ_PT, &old.pts, " old.pts ");
  // for(ii=0;ii<iNr;++ii) DEB_dump_obj__(Typ_PT,&pta[ii]," pta[%d] ",ii);


  // check which intersectionPoint ist nearest to endPt
  L_nxt:
  ii = UT3D_ipt_cknear_npt (&old.pts, pta, iNr);
  if(iNr < 2) goto L_exit;  



  // test if point ii == old.pts; yes: delete point, try again
  if(UT3D_comp2pt(&pta[ii], &old.pts, tol_cv)) {
      // printf(" selPt-int.pt==actPos;\n");
    i1 = iNr;
    MEM_del_nrec (&i1, pta, ii, 1, sizeof(Point));
    MEM_del_nrec (&iNr, va, ii, 1, sizeof(double));
    goto L_nxt;
  }


  L_exit:
    // printf("ex-CNTF_selPt ii=%d\n",ii);

  return ii;

}

 
//================================================================
  int CNTF_exit__ (int *ccvNr) {
//================================================================
// was ../xa/xa_cvcomp.c:658


  // printf("--------------- CNTF_exit__ ccNr=%d\n",ccNr);


  // check for pending object
  if(old.pend != 0) {
    CNTF_out_old ();
  }


  if(ccNr < 1) {
    TX_Error("CNTF_exit__ EMPTY CCV\n");
    return -1;
  }

  *ccvNr = ccNr;

  return 0;

}


//================================================================
  int CNTF_add_ln (long *ip0, long *ip1) {
//================================================================
/// \code
/// create line old.pts - old.pte
/// retCod: -1  points equal, no output.
///          0  OK
/// \endcode

  CurvCCV    cc1 = UT3D_CCV_NUL;   // empty


  // printf("CNTF_add_ln %ld %ld\n",*ip0,*ip1);
  // DEB_dump_obj__ (Typ_PT, &old.pts, "  _add_ln-old-pts");
  // DEB_dump_obj__ (Typ_PT, &old.pte, "  _add_ln-old-pte");


  // test equal point
  if(UT3D_comp2pt(&old.pts, &old.pte, tol_cv)) {
    if(*ip0 > 0) *ip1 = *ip0;
    return -1;
  }



  cc1.typ = Typ_LN;
  cc1.ip0 = *ip0;
  cc1.v0  = 0.;
  cc1.ip1 = *ip1;
  cc1.v1  = 1.;
  cc1.clo = 1;

  CNTF_out_cvtrm (&cc1);

  *ip0 = cc1.ip0;
  *ip1 = cc1.ip1;


  return 0;

}


//================================================================
  int CNTF_out_old () {
//================================================================

  CurvCCV    cc1;


  // printf("CNTF_out_old \n");


  cc1 = UT3D_CCV_NUL;   // empty
/*
  cc1->v0 = UT_VAL_MAX; // undefined; 2016-10-27
  cc1->v1 = UT_VAL_MAX; // undefined; 2016-10-27
*/


  if(old.ip0 == 0) {
    old.ip0 = DB_StorePoint (-1L, &old.pts);
  }
  if(old.ip1 == 0) {
    old.ip1 = DB_StorePoint (-1L, &old.pte);
  }


  cc1.typ = old.typ;
  cc1.dbi = old.dbi;
  cc1.dir = old.dir;
  cc1.clo = old.clo;
  cc1.ip0 = old.ip0;
  cc1.ip1 = old.ip1;
  cc1.v0  = old.v0;
  cc1.v1  = old.v1;

  CNTF_out_cvtrm (&cc1);

    // printf("ex-CNTF_out_old \n");

  return 0;

}


//================================================================
  int CNTF_out_cvtrm (CurvCCV *cc1) {
//================================================================
// was APT_decode_cvco_out
// using old.pts - old.pte for ip0, ip1 !


  // DEB_dump_obj__ (Typ_CVTRM, cc1, " CNTF_out_cvtrm %d",ccNr);
  // if(*ccNr == 3) AP_debug__ ("APT_decode_cvco_out 3");


  // create points for ip0,ip1 if undefined
  if(cc1->ip0 == 0) {
    cc1->ip0 = DB_StorePoint (-1L, &old.pts);
      // printf(" set-ip0=%ld\n",cc1->ip0);

  }
  if(cc1->ip1 == 0) {
    cc1->ip1 = DB_StorePoint (-1L, &old.pte);
      // printf(" set-ip1=%ld\n",cc1->ip1);
  }


  if(ccNr < ccaSiz) {
    cca[ccNr] = *cc1;
    ccNr += 1;

  } else {
    TX_Error ("CNTF_out_cvtrm E001");
    return -1;
  }

    // printf(" ex-CNTF_out_cvtrm\n");

  return 0;

}
 
 
// EOF
