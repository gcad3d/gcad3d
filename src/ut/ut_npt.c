// ../ut/ut_npt.c                  RF                     2014-02-16
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
2014-02-16 Extracted from ../ut/ut_ccv.c. RF.

-----------------------------------------------------
*/
/*!
\file  ../ut/ut_npt.c
\brief polygons from curves, surfaces ... 
\code
=====================================================
List_functions_start:

UT3D_ptNr_obj              get nr of points for polygon from obj (cv)

UT3D_pta_ox_lim            polygon and segNr from complex-obj  (eg CCV)
UT3D_npt_ox__              polygon from complex-obj  (eg CCV)
UT3D_npt_obj               Polygon from bin-obj (typ,struct)
UT3D_npt_trmCv             polygon from trimmed-curve
UT3D_pta_plg               Polygon from PolygonCurve
UT3D_pta_bsp               Polygon from bSpline
UT3D_npt_fac               closed polygon (4 points) from indexed-triangle (Fac3)
UT3D_npt_tria              closed polygon (4 points) from Triangle

UT3D_pta_sus               SurStd-Boundary -> Polygon umwandeln

UT3D_grd_pta               add gridbox-points to MemTab(Point)
UT3D_grd_ptya              add gridbox-points with y-valTab to MemTab(Point)

List_functions_end:
=====================================================
see also:
UT3D_npt_clot             polygon from planar curve (clotoid)
UT3D_pta_plg               polygon from CurvPoly (polygon)
UT3D_pta_bez UT3D_pta_rbez UT3D_cv_rbsp
bspl_pol_bsp
UT3D_cv_sru_               INTERNAL
UT3D_cv_ci                 polygon from circle
UT3D_npt_ci                circular polygon

\endcode *//*----------------------------------------



==============================================================
*/



#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




#include "../ut/ut_geo.h"
#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_itmsh.h"            // MSHIG_EDGLN_.. typedef_MemTab.. Fac3
#include "../ut/ut_ox_base.h"          // OGX_SET_INDEX
#include "../ut/func_types.h"          // UI_Func... SYM_..

#include "../db/ut_DB.h"               // DB_GetCurv

#include "../xa/xa_mem.h"              // memspc55





//================================================================
  int UT3D_npt_tria (Point *pa, Triangle *tria1) {
//================================================================
// make closed polygon (4 points) from Triangle

  pa[0] = *tria1->pa[0];
  pa[1] = *tria1->pa[1];
  pa[2] = *tria1->pa[2];
  pa[3] = *tria1->pa[0];

  return 0;

}


//================================================================
  int UT3D_npt_fac (Point *pa, Fac3 *fac1, Point *pTab) {
//================================================================
// make closed polygon (4 points) from indexed-triangle (Fac3)

  pa[0] = pTab[fac1->i1];
  pa[1] = pTab[fac1->i2];
  pa[2] = pTab[fac1->i3];
  pa[3] = pTab[fac1->i1];

  return 0;

}


//================================================================
  int UT3D_npt_trmCv (int *ptNr, Point* pTab, int ptSiz,
                      CurvCCV* cvt, double tol, int mode) {
//================================================================
/// \code
/// UT3D_npt_trmCv                polygon from trimmed-curve
///   mode   0=perm, fix PRCV; 1=temp, do not use PRCV; 2=unknown
/// RetCod:  0=OK; -1=Error; -2=pTab_overflow;
/// \endcode

 
  int       irc, ptn, otyp;
  Point     *pa;
  char      cv1[OBJ_SIZ_MAX];


  // UT3D_stru_dump (Typ_CVTRM, cvt, " UT3D_npt_trmCv");


  // test if PRCV exists
  if((!mode) && (cvt->dbi != 0)) {
    // get polygon of trimmedCurve
    irc = PRCV_npt_trmCv (&pa, &ptn, cvt);
    if(irc < 0) return -1;
    // copy points -> pTab
    if(ptn + *ptNr >= ptSiz) return -2;
    memcpy (&pTab[*ptNr], pa, sizeof(Point) * ptn);
    *ptNr += ptn;
    goto L_exit;
  }

  // no PRCV exists; change trimmed curve into standard curve
  irc = UTO_cv_cvtrm (&otyp, cv1, NULL, cvt);
  if(irc < 0) return -1;
  irc = UT3D_npt_obj (ptNr, pTab, ptSiz, otyp, cv1, 1, tol, mode);


/*
    ptn = ptSiz - *ptNr;  // get remaining space in pTab
    irc = UT3D_pta_ox_lim (&ptn, &pTab[*ptNr], cvt, 0, NULL, tol);
    if(irc < 0) return -1;
    *ptNr += ptn;
*/

  L_exit:

    // TESTBLOCK
    // printf("ex UT3D_npt_trmCv irc=%d ptNr=%d\n",irc,*ptNr);
    // {int i1; for(i1=0;i1<*ptNr;++i1)
     // printf(" _npt_ %d = %f %f %f\n",i1,pTab[i1].x,pTab[i1].y,pTab[i1].z); }
    // END TESTBLOCK


  return irc;

}


//================================================================
  int UT3D_ptNr_obj (int typ, void *data, double tol) {
//================================================================
/// \code
/// UT3D_ptNr_obj              get nr of points for polygon from obj (cv)
///  RetCode:      >0  nr of points;
/// \endcode

  int      pNr;


  // printf("UT3D_ptNr_obj %d %lf\n",typ,tol);

  if(typ == Typ_PT) return 1;

  if(typ == Typ_LN) return 2;


  switch(typ) {

    case Typ_CI:      // Circ  UT2D_angr_ciSec GR_DrawCirc
      pNr = UT2D_ptNr_ci (fabs(((Circ*)data)->rad),
                          fabs(((Circ*)data)->ango), tol);
      break;

    case Typ_CVELL:   // CurvElli 
      pNr = UT3D_ptNr_ell ((CurvElli*)data, tol);
      break;

    case Typ_CVCLOT:   // CurvClot
      // only estimation.
      UT3D_ptNr_clot (&pNr, data, tol);
      break;

    case Typ_CVBSP:   // CurvBSpl
      // only estimation.
      // UT3D_ptNr_bsplTol (&pNr, data, tol); // use bspl_cv_bsp
      UT3D_ptNr_bsp (&pNr, data, tol); // use bspl_cv_bsp
      break;

    case Typ_CVRBSP:   // rat-B-Spline
      UT3D_ptNr_rbsp (&pNr, data, tol);
      break;

    case Typ_CVPOL:   // CurvPoly
      pNr = ((CurvPoly*)data)->ptNr;
      break;

    case Typ_SURSWP:
      pNr = Tess_ptNr_SURSWP (data, tol);
      break;

    case Typ_Model:
    case Typ_PLN:
      pNr = 1;
      break;

    default:
      TX_Error("UT3D_ptNr_obj not supp. %d",typ);
      return -1;

  }

    // printf("ex UT3D_ptNr_obj %d\n",pNr);

  return pNr;

}


//====================================================================
  int UT3D_pta_sus (int *ptNr, Point* pTab, SurStd *sus, double tol) {
//====================================================================
// SurStd-Boundary -> Polygon umwandeln
// In: ptNr = max Nr of points !

  int   i1;
  Circ  *cip1;
  ObjGX ox1;


  // printf("UT3D_pta_sus typ=%d baseID=%d cvTyp=%d cvID=%d\n",
    // sus->typ,sus->baseID,sus->cvTyp,sus->cvID);



  //------------------------------------ contour = CIRC
  if(sus->cvTyp == Typ_CI) {

    // get CI from DB
    cip1 = DB_get_CI (sus->cvID);
    // CIRC -> Polygon
    UT3D_cv_ci (pTab, ptNr, cip1, *ptNr, tol);


  //------------------------------------ contour = CURVE
  } else if(sus->cvTyp == Typ_CV) {

    // get CCV
    ox1  = DB_GetObjGX (Typ_CV, sus->cvID);
    // printf(" cvtyp=%d form=%d\n",ox1.typ,ox1.form);


    if(ox1.typ == Typ_CVELL) {
      // get Elli from DB
      // elp1 = DB_get_CV (&i1, sus->cvID);
      // Elli -> Polygon
      // UT3D_cv_ell (pTab, ptNr, ox1.data, *ptNr, tol);
      i1 = *ptNr;  // max ptNr
      *ptNr = UT3D_ptNr_ell (ox1.data, tol);
      if(*ptNr > i1) return -1;
      UT3D_npt_ell (*ptNr, pTab, ox1.data);


    } else if(ox1.typ == Typ_CVTRM) {
      // CCV -> 3D-Polygon umwandeln
      i1 = UT3D_pta_ox_lim (ptNr, pTab, &ox1, 0, NULL, tol, 2);
      if(i1 < 0) return i1;


    } else if(ox1.typ == Typ_CVBSP) {
      // BSP -> 3D-Polygon umwandeln
      i1 = bspl_pol_bsp (ptNr, pTab, ox1.data, *ptNr, tol);
      if(i1 < 0) return i1;


    } else {
      TX_Error("UT3D_pta_sus E005 %d",ox1.typ);
      return -1;
    }


  //------------------------------------ contour = unknown
  } else {
    TX_Error("UT3D_pta_sus E004 %d",sus->cvTyp);
    return -1;
  }


  return 0;

}

//====================================================================
  int UT3D_npt_ox__ (int *ptNr, Point* pTab, ObjGX *oxi, double tol) {
//====================================================================
/// \code
/// UT3D_npt_ox__              polygon from complex-obj  (eg CCV)
/// Input:
///   ptNr         max Nr of points !
///   oxi          curve
/// Output:
///   ptNr         Nr of points
///   pTab[ptNr]   polygon
/// RetCod:  0=OK; -1=Error; -2=pTab_overflow;
/// \endcode

// see also UT3D_npt_obj


  int       irc, typ, form, paSiz;
  // long      dbi;
  // ObjGX     *ox1, *ox2;


  // printf("UT3D_npt_ox__ typ=%d form=%d siz=%d ptNr=%d tol=%lf\n",
          // oxi->typ, oxi->form, oxi->siz, *ptNr, tol);


  irc = UT3D_pta_ox_lim (ptNr, pTab, oxi, 0, NULL, tol, 2);

    // TESTBLOCK
    // printf(" ex-UT3D_npt_ox__ irc=%d ptNr=%d\n",irc,*ptNr);
    // UT3D_nstru_dump (Typ_PT, *ptNr, pTab, "CVPOL");
    // GR_Disp_npti (*ptNr, pTab, SYM_TRI_S, ATT_COL_RED, ATT_COL_YELLOW);
    // END TESTBLOCK


  return irc;

/*
  typ = oxi->typ;
  form = oxi->form;


  //----------------------------------------------------------------
  if(typ == Typ_PT) {

    if(form == Typ_Index) {
      OGX_GET_INDEX (&typ, &dbi, oxi);
      pTab[0] = DB_GetPoint (dbi);
      *ptNr = 1;
      return 0;
    }

  //----------------------------------------------------------------
  } else if(typ == Typ_CV) {

    if(form == Typ_Index) {
      // UT3D_stru_dump(Typ_CVPOL, vp1, "CVPOL");
      ox1 = DB_GetCurv ((long)oxi->data);
      // UTO_obj_getp (&vp1, oxi);
      // printf("  typ=%d form=%d siz=%d\n",ox1->typ,ox1->form,ox1->siz);
      return UT3D_npt_ox__ (ptNr, pTab, ox1, tol);        // recursion
    }

  //----------------------------------------------------------------
  } else if(typ == Typ_CVTRM) {
    return UT3D_pta_ox_lim (ptNr, pTab, oxi, 0, NULL, UT_DISP_cv);


  //----------------------------------------------------------------
  } else if(typ == Typ_SUR) {

    // oTab[1] sollte Aussenkontur sein
    ox1 = (ObjGX*)oxi->data;
    ox2 = &ox1[1];
    // printf("         SUR typ=%d form=%d siz=%d\n",
          // ox2->typ,ox2->form,ox2->siz);
    return UT3D_npt_ox__ (ptNr, pTab, ox2, tol);          // recursion


  //----------------------------------------------------------------
  } else if((typ == Typ_SURBSP)  ||
            (typ == Typ_SURRBSP) ||
            (typ == Typ_SURRV)   ||
            (typ == Typ_SURRU))      {

    // Stuetzflaeche; nix tun
    return 1;



  //----------------------------------------------------------------
  }


  paSiz = *ptNr;
  *ptNr = 0;
  return UT3D_npt_obj (ptNr, pTab, paSiz, 
                       form, oxi->data, oxi->siz, tol);

*/
}


//=======================================================================
  int UT3D_npt_obj (int *ptNr, Point* pTab, int ptSiz,
                    int typ, void *data, int siz, double tol, int mode) {
//=======================================================================
/// \code
/// UT3D_npt_obj               Polygon from bin-obj (typ,struct)
/// Input:
///   ptNr         nr of output-points already in pTab (index of 1. unused point)
///   ptSiz        size of pTab
///   typ          type of struct in data
///   data         curve
///   siz          nr of objects of type 'typ' in 'data'
///   mode         0=perm, fix PRCV; 1=temp, do not use PRCV; 2=unknown
/// Output:
///   ptNr         nr of output-points in pTab
///   pTab[ptNr]   polygon
/// RetCod:  0=OK; -1=Error; -2=pTab_overflow;
/// \endcode


  int       i1, irc, ptMax, ptn, cvTyp;
  void      *xu;
  void      *vp1;
  Point     *pa;
  ObjGX     *oa;



  // printf("UT3D_npt_obj typ=%d siz=%d ptNr=%d ptSiz=%d tol=%lf\n",
         // typ, siz, *ptNr, ptSiz, tol);
  // UT3D_stru_dump (typ, data, " __npt_obj");


  irc = 0;
  if(ptSiz - *ptNr < 2) goto L_EOM;




  //----------------------------------------------------------------
  if(typ == Typ_PT)                        {         // 2013-03-19
    // copy all points -> pTab
    // if(siz > *ptNr) goto L_EOM; // outTab zu klein
    if((*ptNr + siz) > ptSiz) goto L_EOM; // outTab zu klein
    memcpy(&pTab[*ptNr], data, siz * sizeof(Point));
    *ptNr += siz;
    return 0;


  //----------------------------------------------------------------
  } else if(typ == Typ_CVTRM) {
    // trimmed-curve -> 3D-Polygon 
    irc = UT3D_npt_trmCv (ptNr, pTab, ptSiz, (CurvCCV*)data, tol, mode);


  //----------------------------------------------------------------
  } else if(typ == Typ_LN) {
    // Line -> Polygon
    if((*ptNr + 2) > ptSiz) goto L_EOM; // outTab zu klein
    pTab[*ptNr] = ((Line*)data)->p1;
    *ptNr += 1;
    pTab[*ptNr] = ((Line*)data)->p2;
    *ptNr += 1;


  //----------------------------------------------------------------
  } else if(typ == Typ_CI) {
    // CIRC -> Polygon
    // UT3D_cv_ci (pTab, ptNr, data, *ptNr, tol);
    ptn = ptSiz - *ptNr;
    UT3D_cv_ci (&pTab[*ptNr], &ptn, data, ptn, tol);
    // UT3D_npt_ci (&pta[ptNr], i2, (Circ*)cv1);
    *ptNr += ptn;


  //----------------------------------------------------------------
  } else if(typ == Typ_CVPOL) {
    // polygon kopieren
      // UT3D_stru_dump(Typ_CVPOL, oxi->data, "_pta_obj CVPOL:");
    // irc = UT3D_pta_plg (ptNr, pTab, data);
    ptn = ptSiz - *ptNr;
    irc = UT3D_pta_plg (&ptn, &pTab[*ptNr], data);
    *ptNr += ptn;


  //----------------------------------------------------------------
  } else if(typ == Typ_CVELL) {
    // irc = UT3D_cv_ell (pTab, ptNr, data, *ptNr, tol);
    // i1 = *ptNr;  // max ptNr
    // *ptNr = UT3D_ptNr_ell (data, tol);
    // if(*ptNr > i1) return -1;
    // UT3D_npt_ell (*ptNr, pTab, data);
    ptn = UT3D_ptNr_ell (data, tol);
    if((*ptNr + ptn) > ptSiz) goto L_EOM; // outTab zu klein
    irc = UT3D_npt_ell (ptn, &pTab[*ptNr], data);
    *ptNr += ptn;



  //----------------------------------------------------------------
  } else if(typ == Typ_CVCLOT) {
    // irc = UT3D_npt_clot  (pTab, ptNr, Typ_CVCLOT, data, tol);
    ptn = ptSiz - *ptNr;
    irc = UT3D_npt_clot  (&pTab[*ptNr], &ptn, data, tol);
    *ptNr += ptn;


  //----------------------------------------------------------------
  } else if(typ == Typ_CVBSP) {
    // irc = UT3D_pta_bsp (ptNr, pTab, data, *ptNr, tol);
    ptn = ptSiz - *ptNr;
    irc = UT3D_pta_bsp  (&ptn, &pTab[*ptNr], data, ptn, tol);
    // irc = bspl_pol_bsp (&i2, &pta[ptNr], ox3->data, iMax, tol);
       // printf(" ex _pta_bsp irc=%d ptn=%d\n",irc,ptn);
    *ptNr += ptn;

    

  //----------------------------------------------------------------
  } else if(typ == Typ_CVRBSP) {
    // irc = UT3D_cv_rbsp (ptNr, pTab, NULL, data, *ptNr, tol);
    ptn = ptSiz - *ptNr;
    irc = UT3D_cv_rbsp (&ptn, &pTab[*ptNr], NULL, data, ptn, tol);
    *ptNr += ptn;


  //----------------------------------------------------------------
  } else if(typ == Typ_Model) {
    pTab[*ptNr] = ((ModelRef*)data)->po;
    *ptNr += 1;


  //----------------------------------------------------------------
  } else if(typ == Typ_ObjGX) {
    // recurse
    ptMax = ptSiz - *ptNr;
        // printf(" _npt_ox start ptMax=%d *ptNr=%d\n",ptMax,*ptNr);
    // *ptNr = 0;
    oa = data;
    for(i1=0; i1<siz; ++i1) {
      ptn = ptMax;
      irc = UT3D_pta_ox_lim (&ptn, &pTab[*ptNr], &oa[i1], 0, NULL, tol, 2);
      if(irc < 0) return -1;
      ptMax -= ptn;
      *ptNr += ptn;
        // printf(" _npt_ox i1=%d ptMax=%d *ptNr=%d\n",i1,ptMax,*ptNr);
    }


  //----------------------------------------------------------------
  } else {
    TX_Error("UT3D_npt_obj E001-typ %d unsupp",typ);
    return -1;
  }


   // for(i1=0;i1<*ptNr;++i1) UT3D_stru_dump (Typ_PT, &pTab[i1]," pta[%d]\n",i1);

  return irc;


  //----------------------------------------------------------------
  L_EOM:
    TX_Error("UT3D_npt_obj pTab overflow");
    return -2;

}


//===========================================================================
  int UT3D_pta_plg (int *ptNr, Point *pTab, CurvPoly *plg) {
//===========================================================================
/*
UT3D_pta_plg               Punktetabelle (relimited) aus PolygonCurve

Input:
  ptNr          size of pta (should be plg->ptNr)

Returncodes:
  0 = OK
 -1 = out of tempSpace
*/

  int      irc, over, pNr, ipe, ptMax;
  double   d1, vTot;
  CurvPoly cv1;


  // printf("------------------------------------------------------- \n");
  // printf("UT3D_pta_plg ptNr=%d v0=%f v1=%f\n",plg->ptNr,plg->v0,plg->v1);
  // UT3D_stru_dump (Typ_CVPOL, plg, " plg");


  // copy the primary obj
  cv1 = *plg;
  ptMax = *ptNr;
  *ptNr = 0;
    // printf("  ptMax=%d\n",ptMax);

    // // TEST:
    // cv1.v1 = 2000.;
    // cv1.v0 = 800.;
    // cv1.dir = 1;


  if(cv1.dir) {
    // CW: swap v0,v1, later swap all points
    MEM_swap_2db (&cv1.v0, &cv1.v1);
  }

  //----------------------------------------------------------------
  // fwd=CCW
  over = 0;
  // if curve goes tru startpoint set over=1
  ipe = plg->ptNr - 1;
  vTot = plg->lvTab[ipe];
    // printf(" ipe=%d vTot=%lf\n",ipe,vTot);

  // for over: curve must be closed
  if(UT3D_comp2pt(&cv1.cpTab[0], &cv1.cpTab[ipe], UT_TOL_pt)) { // UT3D_ck_plgclo
      // printf(" yes, closed ..\n");

    if(UTP_comp2db(cv1.v0, cv1.v1, UT_TOL_min1)) {
        // printf(" yes, equal ..\n");
      // yes, v0 == v1
      if(UTP_comp_0(cv1.v0)) {
        cv1.v1 = vTot;
        goto L_do;  // v0 == 0.; NO pass_tru
      }
    } else {
        // printf(" not equal ..\n");
      if(cv1.v0 < cv1.v1) goto L_do;
    }
    d1 = cv1.v1;
    cv1.v1 = vTot;
    over = 1;
  }


  //----------------------------------------------------------------
  L_do:
  pNr = ptMax;
  irc = UT3D_pta_ccw_plg (&pNr, &pTab[*ptNr], &cv1);
  if(irc < 0) return irc;
  *ptNr += pNr;

  if(over) {
    cv1.v0 = plg->lvTab[0];
    cv1.v1 = d1;
    ptMax -= *ptNr;
    over = 0;
    *ptNr -= 1;  // skip same point
    goto L_do;
  }


  //----------------------------------------------------------------
  // CW
  // swap all points ...
  if(cv1.dir) {
    UT3D_cv_inv (*ptNr, pTab);
  }

  L_exit:

    // TESTBLOCK
    // for(ipe=0;ipe<*ptNr;++ipe)
    // printf(" p[%d] %lf %lf %lf\n",ipe,pTab[ipe].x,pTab[ipe].y,pTab[ipe].z);
    // printf("-------------------------- ex UT3D_pta_plg\n");


  return irc;

}


//================================================================
  int UT3D_pta_bsp (int *ptNr, Point *pTab,
                    CurvBSpl *cvi, int ptMax, double tol) {
//========================================================================
// UT3D_pta_bsp               Polygon from bSpline

// - parameters are always between v0 - v1.
// - determine pass_tru_endpoint if curve is closed:
//  if SR = forward:
//    YES if v0 > v1
//  if SR = backward:
//    YES if v1 > v0


  int      irc, over, pNr, ii, ipe;
  double   d1, vTot;
  CurvBSpl cv1;


  // printf("................................................... \n");
  // printf("UT3D_pta_bsp ptNr=%d deg=%d tol=%f\n",cvi->ptNr,cvi->deg,tol);
  // printf("  dir=%d v0=%f v1=%f ptMax=%d\n",cvi->dir,cvi->v0,cvi->v1,ptMax);
  // UT3D_stru_dump (Typ_CVBSP, cvi, "CV:\n");


  // copy the primary obj
  cv1 = *cvi;
  *ptNr = 0;

    // // TEST:
    // cv1.v0 = 0.5;
    // cv1.v1 = 0.1;
    // cv1.dir = 1;



  if(cvi->dir) {
    // CW: swap v0,v1, later swap all points
    MEM_swap_2db (&cv1.v0, &cv1.v1);
  }


  //----------------------------------------------------------------
  // CCW
  over = 0;
  ipe = cvi->ptNr - 1;
  vTot = cvi->kvTab[cvi->ptNr + cvi->deg];
    // printf(" ipe=%d vTot=%lf v0=%lf v1=%lf\n",ipe,vTot,cv1.v0,cv1.v1);

  // check if closed
  if(UT3D_comp2pt(&cv1.cpTab[0], &cv1.cpTab[ipe], UT_TOL_pt)) { // UT3D_ck_plgclo
      // printf(" yes, closed ..\n");

    if(UTP_comp2db(cv1.v0, cv1.v1, UT_TOL_min1)) {
      // yes, v0 == v1
        // printf(" yes, equal ..\n");
      if(UTP_comp_0(cv1.v0)) {
        cv1.v1 = vTot;
        goto L_do;  // v0 == 0.; NO pass_tru
      }
    } else {
        // printf(" not equal ..\n");
      if(cv1.v0 < cv1.v1) goto L_do;
    }
    // if(UTP_comp_0(cv1.v1)) {
      // // startpoint == endpoint; from UTO_cv_cvtrm - get parameter from point
      // cv1.v1 = vTot;
      // goto L_do;
    // }
    d1 = cv1.v1;
    cv1.v1 = vTot;
    over = 1;
  }



  L_do:
    // printf(" over=%d ptNr=%d v0=%lf v1=%lf\n",over,*ptNr,cv1.v0,cv1.v1);
  irc = bspl_pol_bsp (&pNr, &pTab[*ptNr], &cv1, ptMax, tol);
    // printf("ex _pol_bsp irc=%d pNr=%d\n",irc,pNr);
  if(irc < 0) return irc;
  *ptNr += pNr;

  if(over) {
    cv1.v0 = cvi->kvTab[0];
    cv1.v1 = d1;
    ptMax -= *ptNr;
    over = 0;
    *ptNr -= 1;  // skip same point
    goto L_do;
  }


  //----------------------------------------------------------------
  // CW
  // swap all points ...
  if(cvi->dir) {
    UT3D_cv_inv (*ptNr, pTab);
  }

  L_exit:
    // TESTBLOCK
    // printf("ex UT3D_pta_bsp ptNr=%d\n",*ptNr);
    // for(ii=0;ii<*ptNr;++ii)
      // printf(" p[%d] %lf %lf %lf\n",ii,pTab[ii].x,pTab[ii].y,pTab[ii].z);
    // END TESTBLOCK

  return irc;

}

/* old version 2014-06-19
//===========================================================================
  int UT3D_pta_ox_lim (int *ptn, Point* pta, ObjGX *ccv,
                    int iLim, int *limTab, double tol) {
//===========================================================================
// Input:
//  ptn = max Nr of points !
//  iLim 0  do not fill limTab;
//       >0 fill limTab; iLim == size of limTab.
// OutPut: 
//  limTab points to startIndices of the segnments into pta; terminated by -1.
//         pta[limTab[1]] =  startPoint of 2. segment;
//         pta[limTab[2]] =  startPoint of 3. segment  ..
//         size must be (ccv->segNr + 1)

// see UT3D_pta_ox_lim
  int       irc, ii, i1, i2, ptNr, ptMax, iMax;
  ObjGX     *oTab, *ox2, *ox3;
  // ObjX      xu;
  // Memspc    wrkSpc;
  // char      tmpSpc[50000];
  void      *vp1;



  printf("=======UT3D_pta_ox_lim: ================================ \n");
  // printf("UT3D_pta_ox_lim: iLim=%d ptMax=%d\n",iLim,*ptn);
  // // UT3D_stru_dump (Typ_ObjGX, ccv, "UT3D_pta_ox_lim:\n");
  UTO_dump__ (ccv, "UT3D_pta_ox_lim:\n");  //GEHT NICHT MEHR BEI CCV'S UT3D_dump__
  // UTO_dump_s_ (ccv, "UT3D_pta_ox_lim:\n");


  ptMax = *ptn;
  ptNr  = 0;
  oTab  = ccv->data;    // cvtab;
  // printf(" oTab-posi=%p\n",oTab);
  irc = 0;


  if(iLim > 0) {
    i2 = iLim - 1;
    if(i2 <= ccv->siz) goto L_LIM;
    limTab[ccv->siz] = -1;
  }


  for(i1=0; i1<ccv->siz; ++i1) {

    ox2 = &oTab[i1];
      printf("        _pta_ccv-oTab[%d]\n",i1);

    iMax = ptMax - ptNr;
    if(iMax < 2) goto L_EOM;

    // printf("dispCCV[%d] typ=%d form=%d ptNr=%d\n",i1,ox2->typ,ox2->form,ptNr);

    if(ptNr >= ptMax) goto L_EOM;

    //----------------------------------------------------
    if(ox2->typ == Typ_PT) {
      // UTO_get_PT (&xu, ox2);
      // pta[ptNr] = *xu.pt;
      ii = UTO_obj_getp (&vp1, ox2);
      if(ii < 0) return ii;
      pta[ptNr] = *((Point*)vp1);
        // printf("pt[%d]=%f %f %f\n",ptNr,pta[ptNr].x,pta[ptNr].y,pta[ptNr].z);
      ++ptNr;


    //----------------------------------------------------
    } else if(ox2->typ == Typ_LN) {
      // UTO_get_LN (&xu, ox2);
      ii = UTO_obj_getp (&vp1, ox2);
      if(ii < 0) return ii;
      // Startpunkt ??
      if(i1 == 0) {   // first segment: add also startpoint
        // pta[ptNr] = xu.ln->p1;
        pta[ptNr] = ((Line*)vp1)->p1;
        //printf("LN[%d]=%f %f %f\n",ptNr,pta[ptNr].x,pta[ptNr].y,pta[ptNr].z);
        ++ptNr;
      }
      // pta[ptNr] = xu.ln->p2;
      pta[ptNr] = ((Line*)vp1)->p2;
        // printf("LN[%d]=%f %f %f\n",ptNr,pta[ptNr].x,pta[ptNr].y,pta[ptNr].z);
      ++ptNr;


    //----------------------------------------------------
    } else if(ox2->typ == Typ_CI) {
      // ci1 = DB_GetCirc ((long)ox2->data);
      // UTO_get_CI (&xu, ox2);
      ii = UTO_obj_getp (&vp1, ox2);
      if(ii < 0) return ii;
      // CIR -> Polygon
      // d1 = fabs(xu.ci->rad/500.0);
      // i2 = UT3D_ptNr_ci (xu.ci, tol);
      i2 = UT3D_ptNr_ci (vp1, tol);
      // i2 *= 2.;  // TEST
      if(iMax < i2) goto L_EOM;
      if((ptNr + i2) >= ptMax) goto L_EOM;
      // den ersten Punkt gibts schon !!
      if(i1 > 0) --ptNr;
      UT3D_cv_ci (&pta[ptNr], &i2, vp1, i2, -1.);
      ptNr += i2;
        // printf(" CCV + %d pts f. circ\n",i2);


    //----------------------------------------------------
    } else if(ox2->typ == Typ_CVELL) {
      // ox3 = DB_GetCurv ((long)ox2->data);
      // UTO_obj_get (&ox3, ox2);
      ii = UTO_obj_getp (&vp1, ox2);
      if(ii < 0) return ii;
      // den ersten Punkt gibts schon !!
      if(i1 > 0) --ptNr;
      // ELL -> Polygon; Toleranz ??
      // UT3D_cv_ell (&pta[ptNr], &i2, vp1, iMax, tol);
      i2 = UT3D_ptNr_ell (vp1, tol);
      if(i2 > iMax) goto L_EOM;
      UT3D_npt_ell (i2, &pta[ptNr], vp1);
      ptNr += i2;


    //----------------------------------------------------------------
    } else if(ox2->typ == Typ_CVCLOT) {
      // UTO_obj_get (&ox3, ox2);
      ii = UTO_obj_getp (&vp1, ox2);
      if(ii < 0) return ii;
      // UME_init (&wrkSpc, tmpSpc, 50000);
      if(i1 > 0) --ptNr;   // den ersten Punkt gibts schon !!
      i2 = iMax;
      irc = UT3D_npt_clot  (&pta[ptNr], &i2,
                             Typ_CVCLOT, vp1, tol);
                             // Typ_CVCLOT, ox3->data, tol, &wrkSpc);
      ptNr += i2;

    //----------------------------------------------------
    } else if(ox2->typ == Typ_CVBSP) {
      // UTO_obj_get (&ox3, ox2);
      ii = UTO_obj_getp (&vp1, ox2);
      if(ii < 0) return ii;
      // den ersten Punkt gibts schon !!
      if(i1 > 0) --ptNr;
      // BSP -> Polygon
      // printf(" data-posi=%p\n",ox3->data);
      // UT3D_stru_dump (Typ_CVBSP, ox3->data, "bspl_pol_bsp:cv[%d]\n",i1);
      // irc = bspl_pol_bsp (&i2, &pta[ptNr], ox3->data, iMax, tol);
      irc = UT3D_pta_bsp (&i2, &pta[ptNr], vp1, iMax, tol);
      ptNr += i2;



    //----------------------------------------------------
    } else if(ox2->typ == Typ_CVRBSP) {
      // UTO_obj_get (&ox3, ox2);
      ii = UTO_obj_getp (&vp1, ox2);
      if(ii < 0) return ii;
      // den ersten Punkt gibts schon !!
      if(i1 > 0) --ptNr;
      // BSP -> Polygon
      // printf(" data-posi=%p\n",ox3->data);
      // UT3D_stru_dump (Typ_CVRBSP, ox3->data, "  rbspl ab %d\n",i1);
      // irc = UT3D_cv_rbsp (&i2, &pta[ptNr], ox3->data, iMax, tol);
      irc = UT3D_cv_rbsp (&i2, &pta[ptNr], NULL, vp1, iMax, tol);
      ptNr += i2;

    //----------------------------------------------------
    } else if(ox2->typ == Typ_CVPOL) {
      // ox3 = DB_GetCurv ((long)ox2->data);
      // UTO_obj_get (&ox3, ox2);
      ii = UTO_obj_getp (&vp1, ox2);
      if(ii < 0) return ii;
      // den ersten Punkt gibts schon !!
      if(i1 > 0) --ptNr;  // stehen schon am ersten ersten Punkt - skippen
      // plg -> Polygon
      i2 = iMax;
      // irc = UT3D_pta_plg (&i2, &pta[ptNr], ox3->data);
      irc = UT3D_pta_plg (&i2, &pta[ptNr], vp1);
      ptNr += i2;


    //----------------------------------------------------
    } else if(ox2->typ == Typ_Data) {    // noch nix tun ..
      // printf(" ccv-trim \n");



    //----------------------------------------------------
    } else {
      TX_Print("**** UT3D_pta_ox_lim E001_%d",ox2->typ);
      return -1; //exit (0); // TEST
    }

    if(iLim > 0) {
      limTab[i1] = ptNr-1;
        // printf(" limTab[%d] = %d\n",i1,limTab[i1]);
    }

  }


  *ptn = ptNr;

    // GR_Disp_pTab (ptNr, pta, SYM_STAR_S, 2);
    // for(i1=0;i1<ptNr;++i1) UT3D_stru_dump(Typ_PT,&pta[i1],"pta[%d]",i1);
    // printf("ex UT3D_pta_ox_lim irc=%d ptNr=%d\n",irc,ptNr);

  return irc;


  L_EOM:
  TX_Error("UT3D_pta_ox_lim EOM");
  return -1;


  L_LIM:
  TX_Error("UT3D_pta_ox_lim LIM-EOM");
  return -1;


}
*/

//================================================================
  int UT3D_grd_pta (MemTab(Point) *pmt, GridBox *gb) {
//================================================================
/// \code
/// UT3D_grd_pta               add gridbox-points to MemTab(Point)
/// Input:
///   gb->p1     must be set to startPoint of grid
///   gb ix,iy   number of rows/columns
///   gb dx,dy   offset rows/columns
/// Output:
///   pmt        all gridPoints are added; x1y1z1 x2y1z1 x3y1z1 x1y2z1 ..
/// RetCod: total nr of points
/// \endcode


  int   irc, i1, i2, i3, iTot;
  long  l1;
  Point pb, p1;


  // printf("GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG \n");
  // printf("UT3D_grd_pta\n");
  // printf(" iXyz %d %d %d\n",gb->ix, gb->iy, gb->iz);
  // printf(" dXyz %lf %lf %lf\n",gb->dx, gb->dy, gb->dz);



  // check for total nr of points
  iTot = gb->ix * gb->iy * gb->iz;
  if(iTot < 1) {
    printf ("***** UT3D_grd_pta E001\n");
    return 0;
  }

  // reserve space for i1 points in pmt

  pb = gb->pMin;
  p1 = pb;

  for(i1=0; i1<gb->iz; ++i1) {
    for(i2=0; i2<gb->iy; ++i2) {
      for(i3=0; i3<gb->ix; ++i3) {
        irc = MemTab_add (pmt, &l1, &p1, 1, 0);
          // UT3D_stru_dump (Typ_PT, &p1, "p %d %d %d",i1,i2,i3);
          // GR_Disp_pt (&p1, SYM_TRI_S, ATT_COL_RED);
        if(irc < 0) return -1;
        p1.x += gb->dx;
      }  // end x
      p1.x = pb.x;
      p1.y += gb->dy;
    }  // end y
    p1.x = pb.y;
    p1.z += gb->dz;
  } // end z


  return iTot;

}


//==================================================================
  int UT3D_grd_ptya (MemTab(Point) *pmt, GridBox *gb, double *da) {
//==================================================================
/// \code
/// UT3D_grd_ptya              add gridbox-points with y-valTab to MemTab(Point)
/// Input:
///   da     y-values
/// RetCod: total nr of points
/// Output: Points x1y1z1 x2y1z1 x3y1z1 x1y2z1 ..
/// \endcode
  

  int   irc, i1, i2, i3, iTot;
  long  l1;
  Point pb, p1;
  
  
  // printf("GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG \n");
  // printf("UT3D_grd_ptya\n");
  // printf(" iXyz %d %d %d\n",gb->ix, gb->iy, gb->iz);
  // printf(" dXyz %lf %lf %lf\n",gb->dx, gb->dy, gb->dz);
  // for(i1=0;i1<gb->iy;++i1) printf(" da[%d]=%lf\n",i1,da[i1]);
  
    
  // check for total nr of points
  iTot = gb->ix * gb->iy * gb->iz;
  if(iTot < 1) {
    printf ("***** UT3D_grd_ptya E001\n");
    return 0;
  }

  // reserve space for i1 points in pmt
  pb = gb->pMin;
  p1 = pb;

  for(i1=0; i1<gb->iz; ++i1) {
    for(i2=0; i2<gb->iy; ++i2) {
      p1.y = da[i2];
      for(i3=0; i3<gb->ix; ++i3) {
        irc = MemTab_add (pmt, &l1, &p1, 1, 0);
          // GR_Disp_pt (&p1, SYM_TRI_S, ATT_COL_RED);
        if(irc < 0) return -1;
        p1.x += gb->dx;
      }  // end x
      p1.x = pb.x;
    }  // end y
    p1.x = pb.y;
    p1.z += gb->dz;
  } // end z


  return iTot;

}


//===========================================================================
  int UT3D_pta_ox_lim (int *ptn, Point* pta, ObjGX *oxi,
                    int iLim, int *limTab, double tol, int mode) {
//===========================================================================
/// \code
/// UT3D_pta_ox_lim            get polygon and segNr from complex-obj  (eg CCV)
/// Input:
///   ptn    max Nr of points !
///   iLim    0  do not fill limTab;
///          >0 fill limTab; iLim == size of limTab.
///   mode   0=perm, fix PRCV; 1=temp, do not use PRCV; 2=unknown
/// OutPut: 
///   limTab points to startIndices of the segnments into pta; terminated by -1.
///          can be NULL
///          pta[limTab[1]] =  startPoint of 2. segment;
///          pta[limTab[2]] =  startPoint of 3. segment  ..
///          size must be (oxi->segNr + 1)
///   retCod 0=OK; -1=Err.
///
/// see UT3D_npt_obj
/// \endcode
    
  int       irc, ii, i1, i2, ptNr, ptMax, iMax, oNr, typ, form, otyp, dbTyp;
  long      dbi;
  CurvCCV   *oTab, *pcc, *ox3;
  void      *oo, *oxTab;
  char      cv1[OBJ_SIZ_MAX];
  ObjGX     *ox1, *ox2;


  // printf("======= UT3D_pta_ox_lim ================================ \n");
  // if(oxi->form == 0) exit(-1); // AP_debug__ ("UT3D_pta_ox_lim E001");
  // printf("UT3D_pta_ox_lim iLim=%d ptMax=%d\n",iLim,*ptn);
  // // UT3D_stru_dump (Typ_ObjGX, oxi, "UT3D_pta_ox_lim:\n");
  // UTO_dump__ (oxi, "UT3D_pta_ox_lim:\n");
  // // UTO_dump_s_ (oxi, "UT3D_pta_ox_lim:\n"); DEFEKT bei CCV'S?


  ptMax = *ptn;
  ptNr  = 0;
  typ   = oxi->typ;
  form  = oxi->form;
  oxTab = oxi->data;    // cvtab;
  oNr   = oxi->siz;
  irc = 0;



  //----------------------------------------------------------------
  L_oStart:
  for(i1=0; i1<oNr; ++i1) {

    if(ptNr >= ptMax) goto L_EOM;

    iMax = ptMax - ptNr;
    if(iMax < 2) goto L_EOM;

      // TESTBLOCK
      // printf(" _oStart i1=%d typ=%d form=%d oNr=%d ptNr=%d iMax=%d\n",i1,
             // typ,form,oNr,ptNr,iMax);
      // END TESTBLOCK


    //----------------------------------------------------------------
    if(form == Typ_Index) {
      OGX_GET_INDEX (&dbTyp, &dbi, oxi);
      form = DB_GetObjDat (&oxTab, &oNr, dbTyp, dbi);  // UTO_get_DB
      if(form <= 0) return -1;
      typ = form;
      goto L_oStart;
    }

    //----------------------------------------------------------------
    if(typ == Typ_SUR) {
      // oTab[1] is outer-boundary
      ox1 = (ObjGX*)oxTab;
      ox2 = &ox1[1];
      // printf("         SUR typ=%d form=%d siz=%d\n",
            // ox2->typ,ox2->form,ox2->siz);
      return UT3D_pta_ox_lim (ptn, pta, ox2, 0, NULL, tol, mode);    // recursion


    //----------------------------------------------------------------
    } else if((typ == Typ_SURBSP)  ||
              (typ == Typ_SURRBSP) ||
              (typ == Typ_SURRV)   ||
              (typ == Typ_SURRU))      {

      // support-surf; nothing to do ..
      return 1;


    //----------------------------------------------------------------
    } else if(typ == Typ_CVTRM) {
      // resolv CCV - get standard-obj from trimmed-obj
      if(i1 == 0) {
        if(iLim > 0) {
          i2 = iLim - 1;
          if(i2 <= oNr) goto L_LIM;
          limTab[oNr] = -1;
        }
      }
      pcc = &((CurvCCV*)oxTab)[i1];
        // UT3D_stru_dump (Typ_CVTRM, pcc," pcc[%d]",i1);
      if(pcc->dbi) {
        // db-obj; get its PRCV in UT3D_npt_obj
        oo = (void*)pcc;
        otyp = form;

      } else {
        // change trimmed curve into standard curve
        otyp = pcc->typ;
        irc = UTO_cv_cvtrm (&otyp, cv1, NULL, pcc);
        if(irc < 0) return -1;
          // UT3D_stru_dump (otyp, cv1, " ex cv_cvtrm");
        oo = (void*)cv1;
      }


    //----------------------------------------------------------------
    } else {
      // is standard-object(s) (P,L,C)
      if(oxi->siz > 1) TX_Print("UT3D_pta_ox_lim E002-TODO");
      otyp = form;
      oo   = oxTab;
    }


    if(i1 > 0) { --ptNr; ++iMax; }  // first point already exists

    // polygon from bin.obj
    // ptNr = nr of output-points already in pTab (index of 1. unused point)
    irc = UT3D_npt_obj (&ptNr, pta, ptMax,
                        otyp, oo, 1, tol, mode);
      // TESTBLOCK
      // printf(" ex-npt_obj irc=%d ptNr=%d\n",irc,ptNr);
      // for(i2=0; i2<ptNr; ++i2)
        // printf(" p[%d] %f %f %f\n",i2, pta[i2].x,pta[i2].y,pta[i2].z);
      // END TESTBLOCK

    if(irc < 0) return irc;


    if(iLim > 0) {
      limTab[i1] = ptNr-1;
        // printf(" limTab[%d] = %d\n",i1,limTab[i1]);
    }

  }


  *ptn = ptNr;

    // GR_Disp_pTab (ptNr, pta, SYM_STAR_S, 2);
    // for(i1=0;i1<ptNr;++i1) UT3D_stru_dump(Typ_PT,&pta[i1],"pta[%d]",i1);
    // printf("ex UT3D_pta_ox_lim irc=%d ptNr=%d\n",irc,ptNr);

  return irc;


  L_EOM:
  TX_Error("UT3D_pta_ox_lim EOM");
  return -1;


  L_LIM:
  TX_Error("UT3D_pta_ox_lim LIM-EOM");
  return -1;


}


/*
//===========================================================================
  int UT3D_pta_ox_lim (int *ptn, Point* pta, ObjGX *ccv,
                    int iLim, int *limTab, double tol) {
//===========================================================================
// Input:
//  ptNr = max Nr of points !
//  iLim 0  do not fill limTab;
//       >0 fill limTab; iLim == size of limTab.
// OutPut: 
//  limTab points to startIndices of the segnments into pta; terminated by -1.
//         pta[limTab[1]] =  startPoint of 2. segment;
//         pta[limTab[2]] =  startPoint of 3. segment  ..



  int       irc, i1, i2, ptNr, ptMax, iMax;
  ObjGX     *oTab, *ox2, *ox3;
  ObjX      xu;
  Memspc    wrkSpc;



  // printf("UT3D_pta_ox_lim: siz=%d iLim=%d ptMax=%d\n",ccv->siz,iLim,*ptn);
  // UTO_dump_s_ (ccv, "vor ccv");
  // UTO_dump__ (ccv, "vor ccv");
  // printf(" pTab-posi=%p\n",pta);

  ptMax = *ptn;
  ptNr  = 0;
  oTab  = ccv->data;
  // printf(" oTab-posi=%p\n",oTab);



  if(iLim > 0) {
    i2 = iLim - 1;
    if(i2 <= ccv->siz) goto L_LIM;
    limTab[ccv->siz] = -1;
  }


  for(i1=0; i1<ccv->siz; ++i1) {

    ox2 = &oTab[i1];
    // printf(" oTab[%d]-posi=%p\n",i1,ox2);

    iMax = ptMax - ptNr;
    if(iMax < 2) goto L_EOM;

    // printf("dispCCV[%d] typ=%d form=%d ptNr=%d\n",i1,ox2->typ,ox2->form,ptNr);

    if(ptNr >= ptMax) goto L_EOM;

    //----------------------------------------------------
    if(ox2->typ == Typ_PT) {
      // sollte nur erstes Objekt sein !
      UTO_get_PT (&xu, ox2);
      pta[ptNr] = *xu.pt;
      // printf("pt[%d]=%f %f %f\n",ptNr,pta[ptNr].x,pta[ptNr].y,pta[ptNr].z);
      ++ptNr;


    //----------------------------------------------------
    } else if(ox2->typ == Typ_LN) {
      UTO_get_LN (&xu, ox2);
      // Startpunkt ??
      if(i1 == 0) {
        pta[ptNr] = xu.ln->p1;
        //printf("LN[%d]=%f %f %f\n",ptNr,pta[ptNr].x,pta[ptNr].y,pta[ptNr].z);
        ++ptNr;
      }
      pta[ptNr] = xu.ln->p2;
      // printf("LN[%d]=%f %f %f\n",ptNr,pta[ptNr].x,pta[ptNr].y,pta[ptNr].z);
      ++ptNr;


    //----------------------------------------------------
    } else if(ox2->typ == Typ_CI) {
      // ci1 = DB_GetCirc ((long)ox2->data);
      UTO_get_CI (&xu, ox2);
      // CIR -> Polygon
      // d1 = fabs(xu.ci->rad/500.0);
      i2 = UT3D_ptNr_ci (xu.ci, tol);
      // i2 *= 2.;  // TEST
      if(iMax < i2) goto L_EOM;
      if((ptNr + i2) >= ptMax) goto L_EOM;
      // den ersten Punkt gibts schon !!
      if(i1 > 0) --ptNr;
      UT3D_cv_ci (&pta[ptNr], &i2, xu.ci, i2, -1.);
      ptNr += i2;
      // printf(" CCV + %d pts f. circ\n",i2);


    //----------------------------------------------------
    } else if(ox2->typ == Typ_CVELL) {
      // ox3 = DB_GetCurv ((long)ox2->data);
      UTO_obj_get (&ox3, ox2);
      // den ersten Punkt gibts schon !!
      if(i1 > 0) --ptNr;
      // ELL -> Polygon; Toleranz ??
      // d1 = (UT3D_len_vc (&((CurvElli*)ox3->data)->va) +
      //       UT3D_len_vc (&((CurvElli*)ox3->data)->vb)) / 1000.;
      UT3D_cv_ell (&pta[ptNr], &i2, ox3->data, iMax, tol);
      // for(i3=ptNr; i3<ptNr+i2; ++i3) {  // nur Testausg.
        // printf("  pt[%d]=%f %f %f\n",i3,pta[i3].x,pta[i3].y,pta[i3].z);
      // }
      ptNr += i2;


    //----------------------------------------------------------------
    } else if(ox2->typ == Typ_CVCLOT) {
      UTO_obj_get (&ox3, ox2);
      if(i1 > 0) --ptNr;   // den ersten Punkt gibts schon !!
      i2 = iMax;
      irc = UT3D_npt_clot  (&pta[ptNr], &i2,
                             Typ_CVCLOT, ox3->data, tol);
      ptNr += i2;


    //----------------------------------------------------
    } else if(ox2->typ == Typ_CVBSP) {
      UTO_obj_get (&ox3, ox2);
      // den ersten Punkt gibts schon !!
      if(i1 > 0) --ptNr;
      // BSP -> Polygon
      // printf(" data-posi=%p\n",ox3->data);
      // UT3D_stru_dump (Typ_CVBSP, ox3->data, "bspl_pol_bsp:cv[%d]\n",i1);
      irc = bspl_pol_bsp (&i2, &pta[ptNr], ox3->data, iMax, tol);
      ptNr += i2;



    //----------------------------------------------------
    } else if(ox2->typ == Typ_CVRBSP) {
      UTO_obj_get (&ox3, ox2);
      // den ersten Punkt gibts schon !!
      if(i1 > 0) --ptNr;
      // BSP -> Polygon
      // printf(" data-posi=%p\n",ox3->data);
      // UT3D_stru_dump (Typ_CVRBSP, ox3->data, "  rbspl ab %d\n",i1);
      irc = UT3D_cv_rbsp (&i2, &pta[ptNr], ox3->data, iMax, tol);
      ptNr += i2;


    //----------------------------------------------------
    } else if(ox2->typ == Typ_CVPOL) {
      // ox3 = DB_GetCurv ((long)ox2->data);
      UTO_obj_get (&ox3, ox2);
      // den ersten Punkt gibts schon !!
      if(i1 > 0) --ptNr;  // stehen schon am ersten ersten Punkt - skippen
      // plg -> Polygon
      i2 = iMax;
      irc = UT3D_pta_plg (&i2, &pta[ptNr], ox3->data);
      ptNr += i2;


    //----------------------------------------------------
    } else {
      TX_Error("**** UT3D_pta_ox_lim E001_%d",ox2->typ);
      exit (0); // TEST
    }

    if(iLim > 0) {
      limTab[i1] = ptNr-1;
        // printf(" limTab[%d] = %d\n",i1,limTab[i1]);
    }

  }


  *ptn = ptNr;

    // printf("ex UT3D_pta_ox_lim:\n");
    // GR_Disp_pTab (ptNr, pta, SYM_STAR_S, 2);
    // for(i1=0;i1<ptNr;++i1) UT3D_stru_dump(Typ_PT,&pta[i1],"pta[%d]",i1);

  return 0;



  L_EOM:
  TX_Error("UT3D_pta_ox_lim EOM");
  return -1;


  L_LIM:
  TX_Error("UT3D_pta_ox_lim LIM-EOM");
  return -1;


}
*/


//===================== EOF ===========================
