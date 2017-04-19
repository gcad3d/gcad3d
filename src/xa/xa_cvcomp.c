//  ../xa/xa_cvcomp.c
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

-----------------------------------------------------
Modifications:
2017-02-12 replaced by ../ut/ut_cntf.c. RF.
2014-05-28 new; was xa_cont.c. RF.

-----------------------------------------------------
*/
/*!
\file  ../xa/xa_cvcomp.c
\brief create-countour-functions APT_decode_cvco.. 
\code
=====================================================
List_functions_start:

APT_decode_cvcomp     decode CCV
APT_decode_cvco_nxt   get next obj with MOD and SR
APT_decode_cvco_add   process next obj (add obj's to output)
APT_decode_cvco_int   compute intersectionPoints
// APT_decode_cvco_prj1  project point pti onto curve cvc
// APT_decode_cvco_prj2  project point cpt onto lFig ccv
APT_decode_cvco_invCC revert trimmedCurve
APT_decode_cvco_out   add cc1 to cca

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
#include <stdarg.h>


#include "../ut/ut_geo.h"                 // Point ...
#include "../ut/ut_geo_const.h"        // UT3D_CCV_NUL
#include "../ut/ut_ox_base.h"          // OGX_SET_OBJ
#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_plg.h"              // UT3D_par_par1plg
#include "../ut/func_types.h"                  // SYM_SQUARE ..
#include "../ci/NC_apt.h"              // T_CW
#include "../xa/xa_mem.h"                 // memspc55


// EXTERN SYMBOLS:
// ex ../xa/xa.c
extern Plane WC_sur_act;
extern double    WC_sur_Z;              // der aktive Z-Wert der WC_sur_sur;
extern char  WC_modact_nam[128];   // name of the active submodel; def="" (main)

// aus ../ci/NC_Main.c:
extern int     APT_dispDir;




 
//================================================================
  int APT_decode_cvco_nxt (ObjGX *oxi, int *isr,  int *imod,
           int iIn, int aus_anz, int *aus_typ, double *aus_tab) {
//================================================================
// get next obj with MOD and SR
/// Output:
///   isr       1=CCW; -1=CW; 0=undefined
///   imod      0=undefined, else MOD#
///   retCod    -1=Error; 0=endOfLine; else next unused aus_typ/aus_tab


  int     i1, iOut;
  long    dbi;



  // printf("APT_decode_cvco_nxt %d %d\n",iIn,aus_anz);

  if(iIn >= aus_anz) return 0; // 0=endOfLine;


  *isr = 0;
  *imod = 0;
  iOut = iIn;


  // first obj must be a geom.obj (DB-obj)
  dbi = aus_tab[iIn];
  OGX_SET_INDEX (oxi, aus_typ[iIn], dbi);
    // UT3D_stru_dump (Typ_ObjGX, oxi, "oxi:");
  goto L_nxt_inp;



  //----------------------------------------------------------------
  L_noAmoi:
  // check for CW|CCW
  if(aus_typ[iOut] != Typ_cmdNCsub) goto L_mod;

  if(aus_tab[iOut] == T_REV) {
    *isr = 1;
  } else {
    // ERROR
    i1 = aus_tab[iOut];
    TX_Print ("**** APT_decode_cvco_nxt I_NCsub %d",i1);
    printf("**** APT_decode_cvco_nxt I_NCsub %d\n",i1);

  }
  goto L_nxt_inp;


  L_mod:
  // check for MOD
  if(aus_typ[iOut] != Typ_modif) return iOut;

  *imod = aus_tab[iOut];


  //----------------------------------------------------------------
  L_nxt_inp:
  iOut += 1;
  if(iOut < aus_anz) goto L_noAmoi;

    // printf("ex APT_decode_cvco_nxt sr=%d mod=%d\n",*isr,*imod);

  return iOut;

}


//=============================================================================
  int APT_decode_cvco_out (int *ccNr, CurvCCV *cca, int ccaSiz, CurvCCV *cc1) {
//=============================================================================
// add cc1 to cca

// TODO: fix Bspl-curves, not closed: if v1 < v0 then dir=1
 

  // printf("--------- APT_decode_cvco_out %d\n",cc1->typ);
  UT3D_stru_dump (Typ_CVCCV, cc1, "APT_decode_cvco_out %d",*ccNr);
  // if(*ccNr == 3) AP_debug__ ("APT_decode_cvco_out 3");



  cc1->v0 = UT_VAL_MAX; // undefined; 2016-10-27
  cc1->v1 = UT_VAL_MAX; // undefined; 2016-10-27



  if(*ccNr < ccaSiz) {

    cca[*ccNr] = *cc1;
    *ccNr += 1;

  } else {
    TX_Error ("APT_decode_cvco_out E001");
    return -1;
  }
  
  return 0;

}


//==================================================================
  int APT_decode_cvco_int (Point *pto, double *par1,
                           CurvCCV *cc1, int typ1, void *oc1, int clo1,
                           CurvCCV *cc2, int typ2, void *oc2, int imod) {
//==================================================================
// compute intersectionPoints oldObj-newObj; select point nr imod
// Input:
//   imod     nr (0=first) of point to return
//   cc1      UNUSED
//   clo1     UNUSED
//   cc2      UNUSED
// Output:
//   retCod   0  - OK; pto,par1 has resulting intersectionPoint
//            -1 - no intersectionPoint exists;

  #define TABSIZ 20

  int    i1, irc, iClo, pNr;
  double va[TABSIZ];
  Point  pa[TABSIZ], ptAct;
  ObjGX  ox1, ox2;
  Memspc wrkSeg;


  ptAct = *pto;

  // printf("APT_decode_cvco_int typ1=%d typ2=%d clo1=%d imod=%d\n",
          // typ1,typ2,clo1,imod);
  // UT3D_stru_dump (Typ_PT, &ptAct, " ptAct");
  // UT3D_stru_dump (typ1, oc1, " oc1");
  // UT3D_stru_dump (typ2, oc2, " oc2");
  // UT3D_stru_dump (Typ_CVCCV, cc1, " cc1");
  // UT3D_stru_dump (Typ_CVCCV, cc2, " cc2");


    // TEST
    // pto->x=1500.; pto->y=-600.; pto->z=0.; *par1=0.5; return 0;


  //----------------------------------------------------------------
  // UME_alloc_tmp (&wrkSeg, 2000000);  // 2015-01-04; crash in MS !
  i1 = 2000000; // 2MB
  irc = UME_malloc (&wrkSeg, i1, i1/2);
  if(irc < 0) return -1;
    // UME_dump (&wrkSeg, "wrkSeg");

  // pNr = TABSIZ; // maxNr (size of pa & va)
  // irc = UTO_stru_int (&pNr, pa, va, typ1, oc1, typ2, oc2, &wrkSeg);

  // obj -> ObjGX
  OGX_SET_OBJ (&ox1, typ1, typ1, 1, oc1);
  OGX_SET_OBJ (&ox2, typ2, typ2, 1, oc2);

  irc = UTO_npt_int_2ox (&pNr, pa, va, TABSIZ, &ox1, &ox2, &wrkSeg);
    // printf(" ex npt_int_2ox irc=%d pNr=%d\n",irc,pNr);
  if(irc < 0) goto L_exit;
  if(pNr <= 0) {
      // printf("APT_decode_cvco_int no solution - %d_%d \n",typ1,typ2);
    irc = -1;
    goto L_exit;
  }
    // printf(" _stru_int irc=%d i1=%d\n",irc,i1);

  // iMaxSol = pNr;  // keep nr of solutions ..
  APT_set_modMax (pNr);


/*
  // if not closed and only one cutter:
  // add endpoint as last point into pa/va.
  if(clo1 == 1) {

    // add endpoint as last point into pa/va.
    if((TABSIZ) < 1) goto TabSizErr;
    pa[pNr] = pt2;
    va[pNr] = v2;
    ++pNr;
      printf(" end added ..\n");

    // if a closed curve goes tru startpoint (endPt < startPt):
    //   (only plg,bsp)
    if(v2 < v1) {
      // test all intersection-points; if(iPt < startPt) add totalCurve.
      vTot = UTO_par_tot (o0Typ, obj0);
      for(i1=1; i1<pNr; ++i1) {
        if(va[i1] < v1) va[i1] += vTot;
      }
    }
  }


  // sort ascending parameters, points (else closed curve goes over gaps !)
  UTP_sort_npar_npt (pNr, va, pa);
*/


  // select solNr; <= pNr
  i1 = ILIM0X (imod, pNr-1);
    // printf(" i1=%d imod=%d pNr=%d\n",i1,imod,pNr);



  // if (pa[0] == actPos) then swap pa[0/1]
  if(UT3D_comp2pt(&pa[0], &ptAct, UT_TOL_cv)) {
    // eqal = swap
    MEM_swap__ (&pa[0], &pa[1], sizeof(Point));
  }



  *pto = pa[i1];
  *par1 = va[i1];  // 2016-10-27
  irc = 0;


  L_exit:
  UME_free (&wrkSeg);

  return irc;

}

/*
//===============================================================================
  int APT_decode_cvco_prj2 (Point *pto, double *dist, double *par1,
                            Point *datPt,
                            CurvCCV *ccv, int typCv, char *datCv, int imod) {
//===============================================================================
// project point cpt onto lFig ccv; select point nr. imod
// Output:
//   dist      approximate distance pto - lFig ccv;
//   par1      UNUSED

  // int       typPt;
  // char      datCv[OBJ_SIZ_MAX];
  // char      datPt[OBJ_SIZ_MAX];


  printf("APT_decode_cvco_prj2 %d %d\n",typCv,imod);
  // UT3D_stru_dump (Typ_CVCCV, cpt, " cpt");
  UT3D_stru_dump (Typ_PT, datPt, " datPt");
  UT3D_stru_dump (Typ_CVCCV, ccv, " ccv");
  UT3D_stru_dump (typCv, datCv, " datCv");


  // TEST
  // pto->x=30.; pto->y=-600.; pto->z=0.; *dist=100.; *par1=0.5; return 0;


  // // get struct for point cpt
  // UTO_cv_cvtrm (&typPt, datPt, NULL, cpt);
    // UT3D_stru_dump (typPt, datPt, " datPt");


  // // get struct for lFig ccv
  // UTO_cv_cvtrm (&typCv, datCv, NULL, ccv);
    // UT3D_stru_dump (typCv, datCv, " datCv");


  // project pti onto cvo
  UPRJ_def__ (typCv, datCv, NULL);


  UPRJ_app_pt (pto, datPt);
    UT3D_stru_dump (Typ_PT, pto, " pto");


  // set dist
  *dist = UT3D_lenB_2pt (pto, (Point*)datPt);
    printf(" sist=%lf\n",*dist);


  return  0;

}
*/
/*
//===========================================================================
  int APT_decode_cvco_prj1 (Point *pto, double *dist, Point *pti,
                            CurvCCV *cvc, int otyp, void *ocv, int imod) {
//===========================================================================
/// \code
/// project point pti onto curve cvc
/// more than 1 result: select point[imod]
/// Output:
///   dist     approximate distance pto - lFig ccv;
///   retCod:  0=pto not on curve, pto is not endpoint of curve.
///            1=pto is identical with the endpoint of the curve
/// \endcode

// see also APT_prj_obj_perp

  int       irc;
  Point     pte;


  printf("APT_decode_cvco_prj1 %d\n",imod);
  UT3D_stru_dump (Typ_PT, pti, " pti");
  UT3D_stru_dump (Typ_CVCCV, cvc, " cvc");
  UT3D_stru_dump (otyp, ocv, " ocv");


  // TEST
  // pto->x=30.; pto->y=-600.; pto->z=0.; return 0;


  // project pti onto cvo
  irc = UPRJ_def__ (otyp, ocv, NULL);
  // if(irc < 0) {
    // TX_Error("APT_decode_cvco_prj1 PRJ_%d",otyp);
    // return irc;
  // }

  UPRJ_app_pt (pto, pti);
    UT3D_stru_dump (Typ_PT, pto, " pto");


  // distance pti-pto > UT_TOL_cv
  *dist = UT3D_lenB_2pt (pto, pti);


  return 0;

}
*/

//================================================================
  int APT_decode_cvco_invCC (CurvCCV *cc1) {
//================================================================
// APT_decode_cvco_invCC       revert trimmedCurve

  // if(cc1->rev) TX_Error("APT_decode_cvco_invCC E1\n");
  UT3D_stru_dump (Typ_CVCCV, cc1, " APT_decode_cvco_invCC ");

  // swap v0/v1, ip0/ip1
  MEM_swap_2db (&cc1->v0, &cc1->v1);
  MEM_swap_2lg (&cc1->ip0, &cc1->ip1);

  // cc1->rev = 1;
  cc1->dir = ICHG01(cc1->dir);

  return 0;

}
 

/* UNUSED
//=============================================================================
  int APT_decode_cvco_ck_pts (Point *pti, long ipt,
                              int cvTyp, void *ocv, CurvCCV *ccv) {
//=============================================================================
// check if point pti is identical with startPoint of Obj ocv
/// Input:
///   cvTyp,ocv    curve to test
///   retCod:      0=pti is not startPoint of curve.
///                1=pti is identical with the endpoint of the curve
  
  
                             
  double    dist;            
  Point     pts;
  

  // check if pto == endpoint
  // get endpoint
  UTO_2pt_limstru (&pts, NULL, NULL, NULL, cvTyp, ocv);

  // check distance pti-pto > UT_TOL_cv
  dist = UT3D_lenB_2pt (pti, &pts);
    printf("APT_decode_cvco_ck_pts dist=%lf cvClo=%d\n",dist,ccv->cvClo);


  if (dist < UT_TOL_cv) {
    // startPoint of newCC/newObj==ptAct;
    return 1;
  }

  return 0;

}
*/


//=============================================================================
  int APT_decode_cvco_ck_pte (Point *pti, long ipt,
                              int cvTyp, void *ocv, CurvCCV *ccv) {
//=============================================================================
// check if point pti is identical with endPoint of Obj ocv
///   retCod:  0=pti is not endpoint of curve.
///            1=pti is identical with the endpoint of the curve



  int       irc;
  double    dist;
  Point     pte;


  // check if pto == endpoint
  // get endpoint
  // UTO_2pt_limstru (NULL, &pte, NULL, NULL, cvTyp, ocv);
  irc = UT3D_ptvcpar_std_obj (&pte, NULL, NULL, Ptyp_1, cvTyp, ocv);
  if(irc< 0) return -1;


  // check distance pti-pto > UT_TOL_cv
  dist = UT3D_lenB_2pt (pti, &pte);
    // printf("APT_decode_cvco_ck_pte dist=%lf clo=%d\n",dist,ccv->clo);


  if (dist < UT_TOL_cv) {
    // endpoint of newCC/newObj==ptAct; reverse newCC/newObj
    // do nothing for closedCurve
    if(ccv->clo) {
      // 1=not closed; reverse newCC/newObj
      APT_decode_cvco_invCC (ccv);
      UTO_stru_inv (cvTyp, ocv);
    }


  } else {
    // projPt is not equal endPt
    // check if closed
    if(ccv->clo) {
      // 1=not closed; set startPoint = ptAct/ipAct
      ccv->ip0 = ipt;  // update CC
      UTO_set_ptlim (cvTyp, ocv, pti, NULL, NULL, NULL);

    } else {
      // newObj=closed; set startPoint & endPoint = ptAct/ipAct
      ccv->ip0 = ipt;  // update CC
      ccv->ip1 = ipt;  // update CC
      UTO_set_ptlim (cvTyp, ocv, pti, NULL, pti, NULL);
    }
  }

  return 0;
  
}
  

//================================================================
  int APT_decode_cvco_exit (CurvCCV *ccv1) {
//================================================================


  UT3D_stru_dump (Typ_CVCCV, ccv1, " APT_decode_cvco_exit");


  return 0;

}


//================================================================
  int APT_decode_cvco_add (int *ccNr, CurvCCV *cca,
                           ObjGX *oxi, int isr, int imod) {
//================================================================
/// \code
/// process next obj (add obj's to output)
/// was AP_cont_nxt
/// Input:
///   oxi        next DB-obj to process; TYP_FuncInit=init, TYP_FuncExit=exit
///   isr        revers oxi; 0=not; else yes
///   imod       solution-nr (index of intersection-point if != 1)
/// Output:
///   ccNr       nr of finished records in cca
///   cca[]
///   retCod     0 = OK
///             -2 = degenerated (lt UT_TOL_cv; gt UT_TOL_pt)
///             -3 = degenerated (lt UT_TOL_pt)
///
/// \endcode

// incoming obj:
//   newTyp, newObj, newPts, newPte, newClo, (CurvCCV)newCC
// store obj (pending):
//   oldTyp = type of pending-obj; Typ_Error = none pending.
//   oldObj, oldPts, oldPte, oldClo, (CurvCCV)oldCC
// ipAct is the DB-index of the endpoint of the last contourObj.
// ipOld is the DB-index of the startpoint of the last contourObj.
// ptAct is the endPt of the last output-obj (in cca)
//   if oldObj=Error (after new point) ptAct is the active position
// store unprocessed lFig newCC on exit in oldCC
// unstored lfig in oldCC; its endpoint is oldPte.
// newObj is the struct exactly representing newCC.

// TODO: set v0,v1 in cca (UT_VAL_MAX in APT_decode_cvco_out)

// - es wird dzt eine basicCurve newObj UND eine trimmedCurve newCC
//   untersucht, modifiziert, ...  = MIST !
// - VEREINFACHUNG: do not invert newCC; update newRev,ipOld,ipAct; 
//   (APT_decode_cvco_invCC) bzw create newCC erst bei APT_decode_cvco_out
// necessary to create CurvCCV:
//  - baseCurve typ/dbi, dbi of startpoint, endpoint (ipOld,ipAct)
//  - oldClo/newClo
//  -   make new oldRev/newRev (direction of trimmedCurve compared to baseCurv)
//
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


  static Point    ptAct, oldPts, oldPte;
  static CurvCCV oldCC;
  static char     oldObj[OBJ_SIZ_MAX];
  static int      oldClo, oldTyp, ccaSiz;
  static long     ipAct, ipOld;

  CurvCCV  newCC;
  char     newObj[OBJ_SIZ_MAX];
  char     oid[256];
  int      newTyp, newClo;

  int      irc, i1, oNr, iConn, newDBtyp;
  long     dbi;
  double   d_oe_ns, d_oe_ne, d_os_ns, d_os_ne, par1, d1, d2;
  Point    ptOld, *pp1, newPte, newPts;
  CurvCCV *cco, auxCC;
  void     *vp1;


  // check for init-process
  if(oxi->typ == TYP_FuncInit) {
    oldTyp = TYP_FuncInit;
    ccaSiz = *ccNr;
    return 0;
  }



  // TESTBLOCK
  printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA \n");
  UT3D_stru_dump (Typ_PT, &ptAct, " ptAct; ipAct=%d",ipAct);
  UT3D_stru_dump (Typ_PT, &oldPte, " oldPte");
  if(oxi->typ != TYP_FuncExit) {
  UT3D_stru_dump (Typ_ObjGX,oxi,"APT_decode_cvco_add");
  printf(" ccNr=%d isr=%d imod=%d oldTyp=%d\n",*ccNr,isr,imod,oldTyp);
  printf(" UT_TOL_cv=%lf\n",UT_TOL_cv);
  } else printf(" APT_decode_cvco_add TYP_FuncExit\n");
  // END TESTBLOCK



  // check for exit-process
  if(oxi->typ != TYP_FuncExit) goto L_start;


  //----------------------------------------------------------------
  // exit-process;
  //----------------------------------------------------------------
    // printf(" FuncExit-ccNr=%d oldTyp=%d\n",*ccNr,oldTyp);

  // check for pending object
  if((oldTyp != Typ_Error)&&(oldTyp != TYP_FuncInit)) {
    // output oldCC
    if(oldCC.ip1 == 0) {
      oldCC.ip1 = DB_StorePoint (-1L, &oldPte);
    }
    APT_decode_cvco_out (ccNr, cca, ccaSiz, &oldCC);
    ptAct = oldPte;
    // if last obj was point; no pending obj.
  }


  // test if ptAct == oldPte; else add line (cover degen obj)
  d1 = UT3D_lenB_2pt (&ptAct, &oldPte);
  if(d1 > UT_TOL_pt) {
    // obj > UT_TOL_pt: create aux-line
      // printf(" CREATE AUX_LINE\n");
      // printf(" ipAct=%ld ipOld=&ld\n",ipAct,ipOld);
    ipOld = ipAct;
    if(!ipOld) ipOld = DB_StorePoint (-1L, &ptAct);
    ipAct = DB_StorePoint (-1L, &oldPte);
    newCC = UT3D_CCV_NUL;
    newCC.typ = Typ_LN;
    newCC.ip0 = ipOld;
    newCC.ip1 = ipAct;
    APT_decode_cvco_out (ccNr, cca, ccaSiz, &newCC);
  }


  // if last curve has ip1=0: set to end or startpoint.


  if(*ccNr < 1) {
    printf(" EMPTY CCV !!!!!!!!!!!!!\n");
  }


  return 0;
  // goto L_ex_done;
  // goto L_exit;



  //================================================================
  L_start:
  APT_set_primSeg (imod);  // APT_prim_seg = imod

  // get db-typ & index out of oxi
  OGX_GET_INDEX (&newDBtyp, &dbi, oxi);
  
  // get DB-obj
  newTyp = DB_GetObjDat (&vp1, &oNr, newDBtyp, dbi);
    printf(" newTyp=%d newDBtyp=%d\n",newTyp,newDBtyp);

  // make a copy of newObj
  memcpy (newObj, vp1, OBJ_SIZ_MAX);

  if(newTyp != Typ_PT) {
    // get startPt and endtPt of newObj
    irc = UT3D_ptvcpar_std_obj (&newPts, NULL, NULL, Ptyp_0, newTyp, newObj);
    irc = UT3D_ptvcpar_std_obj (&newPte, NULL, NULL, Ptyp_1, newTyp, newObj);
      UT3D_stru_dump (Typ_PT, &newPts, " newPts");
      UT3D_stru_dump (Typ_PT, &newPte, " newPte");

    // get newCC = new trimmed-curve from DB-obj
    UTO_cvtrm_cv (&newCC, newTyp, dbi);
      UT3D_stru_dump (Typ_CVCCV, &newCC, " newCC-A");

  // fix reverse
    if(isr) {
      // change direction of trimmed-curve
      APT_decode_cvco_invCC (&newCC);
      // change direction of newObj
      UTO_stru_inv (newTyp, newObj);
      // swap start/endpoint
      MEM_swap__ (&newPts, &newPte, sizeof(Point));
        UT3D_stru_dump (Typ_CVCCV, &newCC, " newCC-B");
    }

  } else {
    newPts = *(Point*)newObj;
    newPte = *(Point*)newObj;
  }

  // make newTyp/newObj = basic-curve (standad-struct) of trimmedCurve newCC
  // UTO_cv_cvtrm (&newTyp, &newObj, NULL, &newCC);
    // UT3D_stru_dump (newTyp, &newObj, " newObj");
      // if(dbi == 4) exit(0); // TEST ONLY !

  // get startPt and endtPt of newObj
  // UTO_2pt_limstru (&newPts, &newPte, NULL, NULL, newTyp, newObj);
  // irc = UT3D_ptvcpar_std_obj (&newPts, NULL, NULL, Ptyp_0, newTyp, newObj);
  // irc = UT3D_ptvcpar_std_obj (&newPte, NULL, NULL, Ptyp_1, newTyp, newObj);

    // UT3D_stru_dump (Typ_PT, &newPts, " newPts");
    // UT3D_stru_dump (Typ_PT, &newPte, " newPte");

  // not yet initialized: set ptAct to newPts
  if(oldTyp == TYP_FuncInit) {
      // printf(" init ptAct=newPts\n");
    ptAct = newPts;
  }


  // check closed; 0=YES, 1=NO
  if(newTyp != Typ_PT)  {

    if(newTyp == Typ_LN) {
      // check for degenerated (length < UT_TOL_cv)
      newClo = UT3D_ln_ck_degen (newObj);

    } else {
      // check if closed or degenerated
      newClo = UTO_cv_ck_clo (newTyp, newObj);  // 0=YES,1=NO
    }


    // test if lfig is degenerated;
    if(newClo < 0) {
      // degenerated lfig;
        // printf(" degen:%d\n",newClo);
      APED_oid_dbo_sm (oid, sizeof(oid), newTyp, dbi);
      // TX_Print("skip degenerated object %s %s",oid,WC_modact_nam);
      TX_Print("skip degenerated object %s",oid);
/*  makes problem ..
      // last stored pos = ptAct; new end
      // test if newPts or newPte is nearer
      // test if dist. (ptAct - newPte) > UT_TOL_cv; yes: create line
      d1 = UT3D_lenB_2pt (&ptAct, &newPts);
      d2 = UT3D_lenB_2pt (&ptAct, &newPte);
        // printf(" degen-d1=%lf d2=%lf\n",d1,d2);
      if(d1 > d2) {
          // printf(" degen-swap\n");
        MEM_swap__ (&newPts, &newPte, sizeof(Point));
        d2 = d1; // revert new lfig
      }
      if(d2 > UT_TOL_pt) {
        // obj > UT_TOL_pt: create aux-line
          // printf(" CREATE AUX_LINE\n");
          // printf(" ipAct=%ld ipOld=%ld\n",ipAct,ipOld);
        ipOld = ipAct;
        if(!ipOld) ipOld = DB_StorePoint (-1L, &ptAct);
        ipAct = DB_StorePoint (-1L, &newPte);
        newCC.typ = Typ_LN;
        newCC.dbi = 0L;
        newCC.ip0 = ipOld;
        newCC.ip1 = ipAct;
        APT_decode_cvco_out (ccNr, cca, ccaSiz, &newCC); 
        ptAct = newPte;
        oldPte = newPte;
        oldTyp = Typ_Error; // nothing in buffer
      } else {
        oldPte = newPte; // keep for exit
      }
*/
      return newClo; // exit - new obj is degen. lfig
    }

    newCC.clo = newClo;
      printf(" newClo=%d\n",newClo);

  }



  //----------------------------------------------------------------
  // check for init-process
  if(oldTyp != TYP_FuncInit) goto L_ini_OK;

  if(newTyp != Typ_PT) goto L_ini_lFig;
  //================================================================
  // 0 > PT
  //================================================================
    printf(" 0 > PT\n");
  ptAct = *((Point*)newObj);
  ipAct = dbi;
  newCC.ip0 = ipAct;
  goto L_ex_done;



  L_ini_lFig:
  //================================================================
  // 0 > lFig
  //================================================================
    printf(" 0 > lFig\n");
  // get newPts=startPt of newObj and ptAct=endPt of newObj
  ipAct = DB_StorePoint (-1L, &ptAct);
  newCC.ip0 = ipAct;
  ptAct = newPts;
  // first obj; keep unmodified pending
  goto L_ex_pend;



  //----------------------------------------------------------------
  L_ini_OK: // init done
  ptOld = ptAct;
    // UT3D_stru_dump (Typ_PT, &ptOld, " ptOld=ptAct");
    // printf("oldTyp=%d ipOld=%ld newTyp=%d dbi=%ld\n",oldTyp,ipOld,newTyp,dbi);


  if(newTyp != Typ_PT) {
    // new=lFig
    if(oldTyp != Typ_Error) goto L_lFig_lFig;
    goto L_PT_lFig;
  }

  // new=PT;
  if(oldTyp == Typ_Error) goto L_PT_PT;
  goto L_lFig_PT;



  //================================================================
  // PT > PT              oldObj = Error; newObj = PT
  //================================================================
  L_PT_PT:
    // printf(" PT > PT\n");
  // change newCC (point) into line, DBi=0; startPt=ipAct, endPt=dbi.
  newCC = UT3D_CCV_NUL;
  newCC.typ = Typ_LN;
  newCC.ip0 = ipOld;
  newCC.ip1 = dbi;
  // output of newCC -> cca
  APT_decode_cvco_out (ccNr, cca, ccaSiz, &newCC); 
  // set ptAct = Point newCC
  ptAct = *((Point*)newObj);
  ipAct = dbi;
  goto L_ex_done;  // no obj pending



  //================================================================
  // lFig > PT
  //================================================================
  // unstored lfig in oldCC; its endpoint is oldPte.
  // new point = newCC.
  L_lFig_PT:
    printf(" lFig > PT\n");

  // set newPte = new point
  newPte = *(Point*)newObj;

    // UT3D_stru_dump (Typ_PT, &oldPts, " oldPts");
    // UT3D_stru_dump (Typ_PT, &oldPte, " oldPte");
    // UT3D_stru_dump (Typ_PT, &newPte, " newPte");
    // UT3D_stru_dump (Typ_CVCCV, &oldCC, " oldCC");


  // test if oldPte == newPte;
  if(UT3D_comp2pt(&oldPte, &newPte, UT_TOL_cv)) {
      // printf(" oldPte == new point\n");
    // output oldCC unmodified
    APT_decode_cvco_out (ccNr, cca, ccaSiz, &oldCC);
    ptAct = newPte;
    ipAct = dbi;
    goto L_ex_done; // no pending obj
  }


  // if lFig = first obj:
  if(*ccNr < 1) {
    // test if oldPts == newPte; yes: inv oldCC, output.
    if(UT3D_comp2pt(&oldPts, &newPte, UT_TOL_cv)) {
        // printf(" oldPts == new point\n");
      if(oldClo) {
        // 1=not closed; reverse oldCC
        APT_decode_cvco_invCC (&oldCC);
        // UTO_stru_inv (oldTyp, oldObj);
      }
      // output oldCC
      APT_decode_cvco_out (ccNr, cca, ccaSiz, &oldCC);
      ptAct = newPte;
      ipAct = dbi;
      goto L_ex_done; // no pending obj
    }
  }


  // create tangential point on oldObj (project newCC(PT) -> oldCC(lFig))
  // select point (MOD ?)
  // APT_decode_cvco_prj2 (&ptAct, &dist, &par1, &newPts,
                        // &oldCC, oldTyp, oldObj, imod);

  // ptAct = project newPte onto oldObj
  UPRJ_def__ (oldTyp, oldObj, 1, NULL);
  UPRJ_app_pt (&ptAct, &newPte);
    // UT3D_stru_dump (Typ_PT, &ptAct, " ptAct");

  // trim oldObj (set oldCC-endpoint = ptAct)
  // oldCC.v1 = par1;
  d_oe_ne = UT3D_lenB_2pt (&ptAct, &newPte);
    // printf(" d_oe_ne=%lf\n",d_oe_ne);
  if(d_oe_ne > UT_TOL_cv) {
    // d_oe_ne > tol; create connectionLine
    ipAct = DB_StorePoint (-1L, &ptAct);
    oldCC.ip1 = ipAct;
  } else {
    oldCC.ip1 = dbi;
  }

  // output oldObj (lFig);
  APT_decode_cvco_out (ccNr, cca, ccaSiz, &oldCC);

  // check if point ptAct is on oldCC (length of connectionLine == 0.)
  if(d_oe_ne > UT_TOL_cv) {
    // output connectionLine (newObj = point) ip0=ipAct ip1=dbi
    newCC = UT3D_CCV_NUL;
    newCC.typ = Typ_LN;
    newCC.ip0 = ipAct;
    newCC.ip1 = dbi;       
    APT_decode_cvco_out (ccNr, cca, ccaSiz, &newCC);
  }

  // set ptAct = Point newCC
  ptAct = *(Point*)newObj;
  ipAct = dbi;
  goto L_ex_done; // no pending obj




  //================================================================
  // PT > lFig       oldObj = Error; newObj = lFig
  //================================================================
  L_PT_lFig:
    printf(" PT > lFig\n");

  // test if ptAct (old obj) == startPt of new Obj
  if(UT3D_comp2pt(&ptOld, &newPts, UT_TOL_cv)) {
      // printf(" ptAct == newStart\n");
    ptAct = newPts; 
    newCC.ip0 = ipAct;  // 2017-02-06
    goto L_ex_pend;  // skip pt; keep newCC (unmodified)
  }

  // test if ptAct (old obj) == endPt of new Obj; yes: rev new obj
  if(UT3D_comp2pt(&ptOld, &newPte, UT_TOL_cv)) {
      // printf(" ptAct == newEnd\n");
    // ptAct==endpoint of newCC/newObj; if not closed: reverse newCC/newObj
    if(newClo) {
      // 1=not closed; reverse newCC/newObj
      APT_decode_cvco_invCC (&newCC);
      UTO_stru_inv (newTyp, newObj);
      MEM_swap__ (&newPts, &newPte, sizeof(Point));
    }
    ptAct = newPts;
    goto L_ex_pend;  // keep newCC
  }

  // project oldObj PT onto newObj lFig
  // create tangential point on oldObj (project oldObj (PT) -> newObj)
  // select point (MOD ?)
  // irc = APT_decode_cvco_prj1 (&ptAct, &d_oe_ne,
                              // &ptOld, &newCC, newTyp, newObj, imod);
  irc = UPRJ_def__ (newTyp, newObj, 1, NULL);
  UPRJ_app_pt (&ptAct, &ptOld);
  d_oe_ne = UT3D_lenB_2pt (&ptAct, &ptOld);
  // irc=1:ptAct=endPt
    // printf(" prj1-irc=%d d_oe_ne=%lf\n",irc,d_oe_ne);
    // UT3D_stru_dump (Typ_PT, &ptAct, "  ptAct");

  // check if point ptAct is on newObj (length of connectionLine == 0.)
  // if(UT3D_comp2pt(&ptAct, &ptOld, UT_TOL_cv)) {
  if(d_oe_ne > UT_TOL_cv) {
    // d_oe_ne > tol; create connectionLine
    // create point
    ipAct = DB_StorePoint (-1L, &ptAct);
    // create connectionLine (-> ptAct)
    UTO_cvtrm_cv (&auxCC, Typ_PT, ipAct);
    // output connectionLine ipOld-ipAct
    auxCC = UT3D_CCV_NUL;
    auxCC.typ = Typ_LN;
    auxCC.ip0 = ipOld;
    auxCC.ip1 = ipAct;            
    APT_decode_cvco_out (ccNr, cca, ccaSiz, &auxCC); 
  } else {
    ipAct = ipOld;
  }

  // ptAct is endPt of connectionLine;
  // check if point ptAct == endPoint of newobj; yes: reverse newobj
  // fix newCC/Obj
  newCC.clo = newClo;
  APT_decode_cvco_ck_pte (&ptAct, ipAct, newTyp, newObj, &newCC);
// does UTO_stru_inv newObj !!

  // store newCC
  goto L_ex_pend;



  L_lFig_lFig:
  //----------------------------------------------------------------
  // lFig > lFig           oldCC > newCC
  //----------------------------------------------------------------
    printf(" lFig > lFig   ccNr=%d\n",*ccNr);
    // UT3D_stru_dump (Typ_PT, &oldPte, " oldPte");
    // UT3D_stru_dump (Typ_PT, &newPts, " newPts");
    // UT3D_stru_dump (Typ_PT, &newPte, " newPte");


  // d_oe_ns = UT3D_lenB_2pt (&oldPte, &newPts);  
  // d_oe_ne = UT3D_lenB_2pt (&oldPte, &newPte);
  d_oe_ns = UT3D_len_2pt (&oldPte, &newPts);  
  d_oe_ne = UT3D_len_2pt (&oldPte, &newPte);
      // printf(" d_oe_ns=%lf\n",d_oe_ns);
      // printf(" d_oe_ne=%lf\n",d_oe_ne);

  // check connection
  if(*ccNr < 1) {
    d_os_ns = UT3D_lenB_2pt (&oldPts, &newPts);    // dist to new startPt
    d_os_ne = UT3D_lenB_2pt (&oldPts, &newPte);    // dist to new endPt
      // printf(" d_os_ns=%lf\n",d_os_ns);  
      // printf(" d_os_ne=%lf\n",d_os_ne);


    // get shortest of 4 distances; 0|1|2|3
    iConn = UTP_min_4 (&d_oe_ns, &d_os_ns, &d_oe_ne, &d_os_ne);
      printf(" iConn=%d\n",iConn);


    // iConn=0: OK; oe == ns.

    if(iConn == 1) {         // iConn=1: revert oldObj
      // d_os_ns; revert oldObj
      if(oldClo) {
        if(d_os_ns < UT_TOL_cv) {
          APT_decode_cvco_invCC (&oldCC);
          MEM_swap__ (&oldPts, &oldPte, sizeof(Point));
          MEM_swap__ (&d_os_ne, &d_oe_ne, sizeof(double));
          MEM_swap__ (&d_os_ns, &d_oe_ns, sizeof(double));
        }
      }
      
    } else if(iConn == 2) {  // iConn=2: revert newObj
      // d_oe_ne; revert newObj
      if(newClo) {
        if(d_oe_ne < UT_TOL_cv) {
          APT_decode_cvco_invCC (&newCC);
          UTO_stru_inv (newTyp, newObj);
          MEM_swap__ (&newPts, &newPte, sizeof(Point));
          MEM_swap__ (&d_oe_ns, &d_oe_ne, sizeof(double));
          MEM_swap__ (&d_os_ns, &d_os_ne, sizeof(double));
        }
      }

    } else if(iConn == 3) {  // iConn=3: revert oldObj and newObj
      // d_os_ne; revert oldObj and newObj
      if(d_os_ne < UT_TOL_cv) {
        if(oldClo) {
          APT_decode_cvco_invCC (&oldCC);
          MEM_swap__ (&oldPts, &oldPte, sizeof(Point));
          MEM_swap__ (&d_oe_ns, &d_oe_ne, sizeof(double));
        }
      // revert newObj
        if(newClo) {
          APT_decode_cvco_invCC (&newCC);
          UTO_stru_inv (newTyp, newObj);
          MEM_swap__ (&newPts, &newPte, sizeof(Point));
        }
        MEM_swap__ (&d_oe_ne, &d_os_ns, sizeof(double));
        MEM_swap__ (&d_os_ne, &d_oe_ns, sizeof(double));
      }
    }

  } else {
    // oldObj must have correct orientation ..
    // if(d_oe_ns > d_oe_ne) {
    if(d_oe_ne < UT_TOL_cv) {
      // revert newObj
      if(newClo) {
        APT_decode_cvco_invCC (&newCC);
        UTO_stru_inv (newTyp, newObj);
        MEM_swap__ (&newPts, &newPte, sizeof(Point));
        MEM_swap__ (&d_oe_ns, &d_oe_ne, sizeof(double));
      }
    }
  }


  // test 1.case: oldPte == newPts of newObj (OK)
    // printf(" d_os_ns=%lf tol=%lf\n",d_os_ns,UT_TOL_cv);  
    // get newPts = startPt of newObj
  if(d_oe_ns < UT_TOL_cv) {
      // printf(" oldEnd == newStart\n");
    // yes, ptAct == startPt of newObj
    // output oldCC (unmodified)
    // if ip1==0 set oldPte
    if(oldCC.ip0 == 0) {        // 2017-02-06
      ipOld = DB_StorePoint (-1L, &oldPts);
      oldCC.ip0 = ipOld;
    }
    if(oldCC.ip1 == 0) {        // 2017-02-06
      ipAct = DB_StorePoint (-1L, &oldPte);  // oldPte == newPts
      oldCC.ip1 = ipAct;
    }
    APT_decode_cvco_out (ccNr, cca, ccaSiz, &oldCC);
    ptAct = newPts;
    goto L_ex_pend;  // keep newCC (unmodified)
  }


  // no connection found; test intersection ..
  // compute intersectionPoints oldObj-newObj; select point nr imod
  // do intersection of limited objects only !
  irc = APT_decode_cvco_int (&ptAct, &par1,
                             &oldCC, oldTyp, oldObj, oldClo,
                             &newCC, newTyp, newObj, imod);
    printf("ex _cvco_int irc=%d par1=%lf\n",irc,par1);
    UT3D_stru_dump (Typ_PT, &ptAct,  "  ptAct ex _int");

  // if no intersection exists: connect with connection-line
  if(irc < 0) goto L_lf_fl_conn;
    // printf(" found conn OK\n");


  // trim oldObj (set oldObj-endpoint = selectedPoint)
  ipAct = DB_StorePoint (-1L, &ptAct);
  oldCC.ip1 = ipAct;
  // oldCC.v1  = par1;

  // output oldObj;
  APT_decode_cvco_out (ccNr, cca, ccaSiz, &oldCC);

  // set startPoint newObj = selectedPoint
  newCC.ip0 = ipAct;
  // newCC.v0  = par1;
  newPts = ptAct;
  // if closed: change also endPoint
  if(!newClo) {
    newCC.ip1 = ipAct;
    // newCC.v1  = par1;
    newPte = ptAct;
  }

  goto L_ex_pend;  // keep newCC


  //----------------------------------------------------------------
  L_lf_fl_conn:
  // no connection between oldObj - newObj; find nearest endpoints,
  // add connectLine between nearest endpoints.
      // printf(" L_lf_fl_conn: ccNr=%d iConn=%d oldClo=%d newClo=%d\n",
             // *ccNr,iConn,oldClo,newClo);
// TODO: create point as normal from ptAct to newObj; test distance.

  if(*ccNr < 1) {
    if(iConn == 1) { // d_os_ns; revert oldObj
      if(oldClo) {
          APT_decode_cvco_invCC (&oldCC);
          MEM_swap__ (&oldPts, &oldPte, sizeof(Point));
          MEM_swap__ (&d_os_ne, &d_oe_ne, sizeof(double));
          MEM_swap__ (&d_os_ns, &d_oe_ns, sizeof(double));
      }

    } else if(iConn == 2) {  // iConn=2: revert newObj
      // d_oe_ne; revert newObj
      if(newClo) {
          APT_decode_cvco_invCC (&newCC);
          UTO_stru_inv (newTyp, newObj);
          MEM_swap__ (&newPts, &newPte, sizeof(Point));
          MEM_swap__ (&d_oe_ns, &d_oe_ne, sizeof(double));
          MEM_swap__ (&d_os_ns, &d_os_ne, sizeof(double));
      }

    } else if(iConn == 3) {  // iConn=3: revert oldObj and newObj
      // d_os_ne; revert oldObj and newObj
        if(oldClo) {
          APT_decode_cvco_invCC (&oldCC);
          MEM_swap__ (&oldPts, &oldPte, sizeof(Point));
          MEM_swap__ (&d_oe_ns, &d_oe_ne, sizeof(double));
        }
      // revert newObj
        if(newClo) {
          APT_decode_cvco_invCC (&newCC);
          UTO_stru_inv (newTyp, newObj);
          MEM_swap__ (&newPts, &newPte, sizeof(Point));
        }
        MEM_swap__ (&d_oe_ne, &d_os_ns, sizeof(double));
        MEM_swap__ (&d_os_ne, &d_oe_ns, sizeof(double));
    }

  }

    // output oldObj
    APT_decode_cvco_out (ccNr, cca, ccaSiz, &oldCC);
    if(d_oe_ns > d_oe_ne) {
      // revert newObj
      APT_decode_cvco_invCC (&newCC);
      UTO_stru_inv (newTyp, newObj);
      MEM_swap__ (&newPts, &newPte, sizeof(Point));
    }

    // output connect-line oldPte-newPts
    auxCC = UT3D_CCV_NUL;
    auxCC.typ = Typ_LN;
    auxCC.ip0 = DB_StorePoint (-1L, &oldPte);
    auxCC.ip1 = DB_StorePoint (-1L, &newPts);
    APT_decode_cvco_out (ccNr, cca, ccaSiz, &auxCC);
    // keep newCC
    goto L_ex_pend;



  //================================================================
  L_ex_pend:
    // obj is valid and pending
    // copy newObj newCC -> oldCC
    oldCC  = newCC;
    oldTyp = newTyp;
    memcpy (oldObj, newObj, OBJ_SIZ_MAX);
    oldClo = newClo;
    oldCC.clo = newClo;
      UT3D_stru_dump (Typ_CVCCV, &oldCC, " oldCC");
      // UT3D_stru_dump (oldTyp, oldObj, " oldObj");
    goto L_exit;


  //================================================================
  L_ex_done:
    // newCC already processed; no pending obj
    // ptAct and ipAct must be set.
    oldTyp = Typ_Error;

    
  //================================================================
  L_exit:
    ipOld = ipAct;

    oldPts = newPts;
    oldPte = newPte;

      printf("ex APT_decode_cvco_add oldTyp=%d ipOld=%ld ipAct=%ld\n",
             oldTyp,ipOld,ipAct);
      // UT3D_stru_dump (Typ_CVCCV, &oldCC, " oldCC");
      UT3D_stru_dump (Typ_PT, &ptAct,  "  ptAct");
      UT3D_stru_dump (Typ_PT, &oldPts, "  oldPts");
      UT3D_stru_dump (Typ_PT, &oldPte, "  oldPte");
      // printf("AAAAAAAAAAAAAAAAAAAAAAA ex APT_decode_cvco_add \n");


  return 0;

}


//================================================================
  int APT_decode_cvcomp (ObjGX *ocv, Memspc *tbuf1, Memspc *tbuf2,
                         int aus_anz,int aus_typ[],double aus_tab[]) {
//================================================================
// was APT_decode_ccv
// Output:
//   ocv    Typ_CVCCV with n CurvCCV-records in tbuf1


 
  int      irc, i1, isr, imod, ccNr , iIn, ccaSiz;
  long     l1, dbi;
  char     cbuf[32];
  ObjGX    oxi;
  CurvCCV *cca;


  // printf("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC \n");
  // printf("APT_decode_cvcomp %d\n",aus_anz);
  // for(i1=0;i1<aus_anz;++i1)
    // printf(" typ[%d]=%d val=%lf\n",i1,aus_typ[i1],aus_tab[i1]);
  // printf("  UT_TOL_pt=%lf UT_TOL_cv=%lf\n",UT_TOL_pt,UT_TOL_cv);


  // Par.1= "CCV"!
  if(aus_anz < 2) {
    TX_Error(" zuenig Parameter");
    return -1;
  }

  // if((int)aus_tab[0] == T_CCV) return -1;

  // verrundetes Rechteck (aus APT_decode_ccv <<<<<<<<<<<<<<<<<<<<<<<<<<<<


  // Test if all obj's exist ..
  for(i1=1; i1<aus_anz; ++i1) {
    if(aus_typ[i1] == Typ_modif) continue;     // skip MOD()
    if(aus_typ[i1] == Typ_cmdNCsub) continue;  // skip CW ..
    l1 = aus_tab[i1];
    if(DB_QueryDef(aus_typ[i1],l1) < 0) {
      APED_oid_dbo__ (cbuf, aus_typ[i1],l1);
      TX_Error("CCV: %s (Obj %d) not defined",cbuf,i1);
      return -1;
    }
  }


  // only a point: temp-disp.
  if(aus_anz == 2) {
    if(aus_typ[1] == Typ_PT) {
      UI_disp_dbo ((int)aus_typ[1], (long)aus_tab[1]);
      return 1;
    }
  }



  // get memSpc for cca (CurvCCV-records) in tbuf1
  // max memspc needed:
  ccaSiz = UME_ck_free (tbuf1) / sizeof(CurvCCV);
    // printf(" ccaSiz=%d\n",ccaSiz);
  i1 = aus_anz + 4;  // minsiz
  if(ccaSiz > i1) ccaSiz = i1;
  cca = UME_get_start (tbuf1);


  iIn  = 1;
  ccNr = 0;

  // init
  oxi.typ = TYP_FuncInit;
  APT_decode_cvco_add (&ccaSiz, cca, &oxi, 0, 0);


  //----------------------------------------------------------------
  L_nxt_obj:
  // loop tru objects ..
      // printf(" ========== iIn=%d ccNr=%d\n",iIn,ccNr);


    if(ccNr >= ccaSiz) {
      TX_Error ("APT_decode_cvcomp EOM");
      return -1;
    }

    // get next obj with MOD and SR
    iIn = APT_decode_cvco_nxt (&oxi, &isr, &imod, iIn, aus_anz, aus_typ, aus_tab);
    if(iIn < 0) return -1;
    if(!iIn) goto L_close;

    // process next obj (add obj's to output)
    irc = APT_decode_cvco_add (&ccNr, cca, &oxi, isr, imod);
    // if(irc < 0) return -1;

    goto L_nxt_obj;



  //----------------------------------------------------------------
  // after last obj: add pending obj's to output
  L_close:
  oxi.typ = TYP_FuncExit;  // finish pending obj's
  APT_decode_cvco_add (&ccNr, cca, &oxi, isr, imod);

  //------------------------------------------------
  // create CCV (top-obj)
  OGX_SET_OBJ (ocv, Typ_CVCCV, Typ_CVCCV, ccNr, cca);

  // reserve memspce in tbuf1
  if(ccNr > 0) UME_reserve (tbuf1, sizeof(CurvCCV) * ccNr);


    // TESTONLY:
    // UTO_dump__ (ocv, "ex decode_cvcomp:");
    // UT3D_dump_dbo (Typ_PT, -5L);
    // printf("CCCCCCCCCCCCCCCCCCCCCCCCCCCCC exiting decode_cvcomp: \n");

  return 0;

}

// EOF
