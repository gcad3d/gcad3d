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

UT3D_npt_obj               Polygon from obj (typ,struct)
UT3D_npt_ox                Polygon from complexObj
UT3D_pta_plg               Polygon from PolygonCurve
UT3D_pta_bsp               Polygon from bSpline
UT3D_pta_ccv               Polygon from CCV

UT3D_pta_sus               SurStd-Boundary -> Polygon umwandeln

UT3D_grd_pta               add gridbox-points to MemTab(Point)
UT3D_grd_ptya              add gridbox-points with y-valTab to MemTab(Point)

List_functions_end:
=====================================================
see also:
UT3D_npt_curvp             polygon from planar curve (clotoid)
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
#include "../ut/ut_cast.h"                // INT_PTR
#include "../ut/ut_memTab.h"           // MemTab
#include "../ut/ut_ox_base.h"             // OGX_SET_INDEX

#include "../gr/ut_UI.h"               // UI_Func... SYM_..

#include "../db/ut_DB.h"               // DB_GetCurv

#include "../xa/xa_mem.h"              // memspc55


typedef_MemTab(Point);





//================================================================
  int UT3D_ptNr_obj (int typ, void *data, double tol) {
//================================================================
/// UT3D_ptNr_obj              get nr of points for polygon from obj (cv)

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

    case Typ_CVBSP:   // CurvBSpl
      // only estimation.
      UT3D_ptNr_bsplTol (&pNr, data, tol); // use bspl_cv_bsp
      break;

    case Typ_CVPOL:   // CurvPoly
      pNr = ((CurvPoly*)data)->ptNr;
      break;

    case Typ_SURSWP:
      pNr = Tess_ptNr_SURSWP (data, tol);
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


    } else if(ox1.typ == Typ_CVCCV) {
      // CCV -> 3D-Polygon umwandeln
      i1 = UT3D_pta_ccv (ptNr, pTab, &ox1, 0, NULL, tol);
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
  int UT3D_npt_ox (int *ptNr, Point* pTab, ObjGX *oxi, double tol) {
//====================================================================
// ObjGX-objekt -> Polygon umwandeln
// Input:
//   ptNr         max Nr of points !
//   oxi          curve
// Output:
//   pTab[ptNr]   polygon
// RetCod:  0=OK; -1=Error; -2=pTab_overflow;


  int       typ, form;
  long      dbi;
  ObjGX     *ox1, *ox2;


  // printf("UT3D_npt_ox typ=%d form=%d siz=%d ptNr=%d tol=%lf\n",
          // oxi->typ,oxi->form,oxi->siz,*ptNr,tol);

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
      return UT3D_npt_ox (ptNr, pTab, ox1, tol);        // recursion
    }

  //----------------------------------------------------------------
  } else if(typ == Typ_CVCCV) {
    return UT3D_pta_ccv (ptNr, pTab, oxi, 0, NULL, UT_DISP_cv);


  //----------------------------------------------------------------
  } else if(typ == Typ_SUR) {

    // oTab[1] sollte Aussenkontur sein
    ox1 = (ObjGX*)oxi->data;
    ox2 = &ox1[1];
    // printf("         SUR typ=%d form=%d siz=%d\n",
          // ox2->typ,ox2->form,ox2->siz);
    return UT3D_npt_ox (ptNr, pTab, ox2, tol);          // recursion


  //----------------------------------------------------------------
  } else if((typ == Typ_SURBSP)  ||
            (typ == Typ_SURRBSP) ||
            (typ == Typ_SURRV)   ||
            (typ == Typ_SURRU))      {

    // Stuetzflaeche; nix tun
    return 1;



  //----------------------------------------------------------------
  }


  return UT3D_npt_obj (ptNr, pTab, form, oxi->data, oxi->siz, tol);

}


//====================================================================
  int UT3D_npt_obj (int *ptNr, Point* pTab,
                    int typ, void *data, int siz, double tol) {
//====================================================================
// ObjGX-objekt -> Polygon umwandeln
// Input:
//   ptNr         max Nr of points !
//   typ          type of struct in data
//   data         curve
//   siz          nr of objects of type 'typ' in 'data'
// Output:
//   ptNr         nr of output-points in pTab
//   pTab[ptNr]   polygon
// RetCod:  0=OK; -1=Error; -2=pTab_overflow;


  int       i1, irc, ptMax, ptn, cvTyp;
  void      *xu;
  void      *vp1;
  ObjGX     *oa;



  // printf("UT3D_npt_obj typ=%d siz=%d ptNr=%d tol=%lf\n",typ,siz,*ptNr,tol);


  irc = 0;
  if(*ptNr < 2) goto L_EOM;




  //----------------------------------------------------------------
  if(typ == Typ_PT)                        {         // 2013-03-19
    // copy all points -> pTab
    if(siz > *ptNr) goto L_EOM; // outTab zu klein
    memcpy(pTab, data, siz * sizeof(Point));
    *ptNr = siz;
    return 0;


  //----------------------------------------------------------------
  } else if(typ == Typ_CVCCV) {
    // CCV -> 3D-Polygon umwandeln (aus GR_DrawSur)
    i1 = UT3D_pta_ccv (ptNr, pTab, data, 0, NULL, tol);
    // printf("  ccv hat %d pt\n",ptNr);


  //----------------------------------------------------------------
  } else if(typ == Typ_LN) {
    // Line -> Polygon
    pTab[0] = ((Line*)data)->p1;
    pTab[1] = ((Line*)data)->p2;
    *ptNr = 2;


  //----------------------------------------------------------------
  } else if(typ == Typ_CI) {
    // CIRC -> Polygon
    UT3D_cv_ci (pTab, ptNr, data, *ptNr, tol);


  //----------------------------------------------------------------
  } else if(typ == Typ_CVPOL) {
    // polygon kopieren
      // UT3D_stru_dump(Typ_CVPOL, oxi->data, "_pta_obj CVPOL:");
    irc = UT3D_pta_plg (ptNr, pTab, data);             // 2012-01-18


  //----------------------------------------------------------------
  } else if(typ == Typ_CVELL) {
    // irc = UT3D_cv_ell (pTab, ptNr, data, *ptNr, tol);
    i1 = *ptNr;  // max ptNr
    *ptNr = UT3D_ptNr_ell (data, tol);
    if(*ptNr > i1) return -1;
    UT3D_npt_ell (*ptNr, pTab, data);


  //----------------------------------------------------------------
  } else if(typ == Typ_CVCLOT) {
    irc = UT3D_npt_curvp  (pTab, ptNr, Typ_CVCLOT, data, tol);


  //----------------------------------------------------------------
  } else if(typ == Typ_CVBSP) {
    // printf(" CurvBSpl v0=%f v1=%f\n",((CurvBSpl*)data)->v0,
                                     // ((CurvBSpl*)data)->v1);
    // UT3D_ptNr_bsplTol ? bspl_cv_bsp ?
    // irc = bspl_pol_bsp (ptNr, pTab, data, *ptNr, tol);
    irc = UT3D_pta_bsp (ptNr, pTab, data, *ptNr, tol);
    

  //----------------------------------------------------------------
  } else if(typ == Typ_CVRBSP) {
    // printf(" CurvRBSpl v0=%f v1=%f\n",((CurvBSpl*)data)->v0,
                                     // ((CurvBSpl*)data)->v1);
    irc = UT3D_cv_rbsp (ptNr, pTab, NULL, data, *ptNr, tol);


  //----------------------------------------------------------------
  } else if(typ == Typ_ObjGX) {
    // recurse
    ptMax = *ptNr;
    *ptNr = 0;
    oa = data;
    for(i1=0; i1<siz; ++i1) {
      ptn = ptMax;
      irc = UT3D_npt_ox (&ptn, &pTab[*ptNr], &oa[i1], tol);
      if(irc < 0) return -1;
      ptMax -= ptn;
      *ptNr += ptn;
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

  int      irc, over, pNr, ii, ptMax;
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
  ii = plg->ptNr - 1;
  vTot = plg->lvTab[ii];
    // printf(" ii=%d vTot=%lf\n",ii,vTot);

  // check if closed
  if(UT3D_comp2pt(&cv1.cpTab[0], &cv1.cpTab[ii], UT_TOL_pt)) { // UT3D_ck_plgclo
      // printf(" yes, closed ..\n");

    if(UTP_comp2db(cv1.v0, cv1.v1, UT_TOL_min1)) {
        // printf(" yes, equal ..\n");
      // yes, v0 == v1
      if(UTP_comp_0(cv1.v0)) {
        cv1.v1 = vTot;
        goto L_do;  // v0 == 0.; NO pass_tru
      }
      // else YES
    } else {
        // printf(" not equal ..\n");
      if(cv1.v0 < cv1.v1) goto L_do;
    }
    d1 = cv1.v1;
    cv1.v1 = vTot;
    over = 1;
  }


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
    // for(ii=0;ii<*ptNr;++ii)
    // printf(" p[%d] %lf %lf %lf\n",ii,pTab[ii].x,pTab[ii].y,pTab[ii].z);
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


  if(cv1.dir) {
    // CW: swap v0,v1, later swap all points
    MEM_swap_2db (&cv1.v0, &cv1.v1);
  }

  //----------------------------------------------------------------
  // CCW
  over = 0;
  ipe = cvi->ptNr - 1;
  vTot = cvi->kvTab[cvi->ptNr + cvi->deg];
    // printf(" ii=%d vTot=%lf\n",ii,vTot);

  // check if closed
  if(UT3D_comp2pt(&cv1.cpTab[0], &cv1.cpTab[ipe], UT_TOL_pt)) { // UT3D_ck_plgclo
      // printf(" yes, closed ..\n");

    if(UTP_comp2db(cv1.v0, cv1.v1, UT_TOL_min1)) {
        // printf(" yes, equal ..\n");
      // yes, v0 == v1
      if(UTP_comp_0(cv1.v0)) {
        cv1.v1 = vTot;
        goto L_do;  // v0 == 0.; NO pass_tru
      }
      // else YES
    } else {
        // printf(" not equal ..\n");
      if(cv1.v0 < cv1.v1) goto L_do;
    }
    d1 = cv1.v1;
    cv1.v1 = vTot;
    over = 1;
  }



  L_do:
  irc = bspl_pol_bsp (&pNr, &pTab[*ptNr], &cv1, ptMax, tol);
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
  if(cv1.dir) {
    UT3D_cv_inv (*ptNr, pTab);
  }


  L_exit:
    // for(ii=0;ii<*ptNr;++ii)
      // printf(" p[%d] %lf %lf %lf\n",ii,pTab[ii].x,pTab[ii].y,pTab[ii].z);

  return irc;

}

/* old version 2014-06-19
//===========================================================================
  int UT3D_pta_ccv (int *ptn, Point* pta, ObjGX *ccv,
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

// see UT3D_pta_ccv
  int       irc, ii, i1, i2, ptNr, ptMax, iMax;
  ObjGX     *oTab, *ox2, *ox3;
  // ObjX      xu;
  // Memspc    wrkSpc;
  // char      tmpSpc[50000];
  void      *vp1;



  printf("=======UT3D_pta_ccv: ================================ \n");
  // printf("UT3D_pta_ccv: iLim=%d ptMax=%d\n",iLim,*ptn);
  // // UT3D_stru_dump (Typ_ObjGX, ccv, "UT3D_pta_ccv:\n");
  UTO_dump__ (ccv, "UT3D_pta_ccv:\n");  //GEHT NICHT MEHR BEI CCV'S UT3D_dump__
  // UTO_dump_s_ (ccv, "UT3D_pta_ccv:\n");


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
      irc = UT3D_npt_curvp  (&pta[ptNr], &i2,
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
      TX_Print("**** UT3D_pta_ccv E001_%d",ox2->typ);
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
    // printf("ex UT3D_pta_ccv irc=%d ptNr=%d\n",irc,ptNr);

  return irc;


  L_EOM:
  TX_Error("UT3D_pta_ccv EOM");
  return -1;


  L_LIM:
  TX_Error("UT3D_pta_ccv LIM-EOM");
  return -1;


}
*/

//================================================================
  int UT3D_grd_pta (MemTab(Point) *pmt, GridBoxH *gb) {
//================================================================
/// \code
/// UT3D_grd_pta               add gridbox-points to MemTab(Point)
/// RetCod: total nr of points
/// Output: Points x1y1z1 x2y1z1 x3y1z1 x1y2z1 ..
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

  pb = *(gb->p1);
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
  int UT3D_grd_ptya (MemTab(Point) *pmt, GridBoxH *gb, double *da) {
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
  pb = *(gb->p1);
  p1 = pb;

  // reserve space for i1 points in pmt
  pb = *(gb->p1);
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
  int UT3D_pta_ccv (int *ptn, Point* pta, ObjGX *ccv,
                    int iLim, int *limTab, double tol) {
//===========================================================================
/// \code
/// UT3D_pta_ccv           get polygon from CCV       CCV -> Polygon
/// Input:
///  ptn = max Nr of points !
///  iLim 0  do not fill limTab;
///       >0 fill limTab; iLim == size of limTab.
/// OutPut: 
///  limTab points to startIndices of the segnments into pta; terminated by -1.
///         can be NULL
///         pta[limTab[1]] =  startPoint of 2. segment;
///         pta[limTab[2]] =  startPoint of 3. segment  ..
///         size must be (ccv->segNr + 1)
///  retCod 0=OK; -1=Err.
/// \endcode
    
  int       irc, ii, i1, i2, ptNr, ptMax, iMax, oNr, otyp;
  long      dbi;
  CurvCCV   *oTab, *pcc, *ox3;
  void      *oo;
  char      cv1[OBJ_SIZ_MAX];
  void      *vp1;


  // printf("======= UT3D_pta_ccv ================================ \n");
  // printf("UT3D_pta_ccv: iLim=%d ptMax=%d\n",iLim,*ptn);
  // // UT3D_stru_dump (Typ_ObjGX, ccv, "UT3D_pta_ccv:\n");
  // UTO_dump__ (ccv, "UT3D_pta_ccv:\n");
  // // UTO_dump_s_ (ccv, "UT3D_pta_ccv:\n"); DEFEKT bei CCV'S?


  ptMax = *ptn;
  ptNr  = 0;
  oTab  = ccv->data;    // cvtab;
  irc = 0;


  if(iLim > 0) {
    i2 = iLim - 1;
    if(i2 <= ccv->siz) goto L_LIM;
    limTab[ccv->siz] = -1;
  }


  for(i1=0; i1<ccv->siz; ++i1) {

    pcc = &oTab[i1];
      // printf(">>>>>>>>>>>> _pta_ccv-oTab[%d]-typ=%d dbi=%ld\n",
             // i1,pcc->typ,pcc->dbi);

    iMax = ptMax - ptNr;
    if(iMax < 2) goto L_EOM;

    if(ptNr >= ptMax) goto L_EOM;

    // get obj out of DB
    otyp = pcc->typ;
    dbi  = pcc->dbi;
    if(dbi) {   // lines can have dbi=0
      irc = UTO_get_DB (&oo, &oNr, &otyp, dbi);
      if(irc < 0) return -1;
    } else {
      oo = NULL;
    }
      // printf(" otyp=%d db=%ld oNr=%d \n",otyp,pcc->dbi,oNr);
      // UT3D_stru_dump (otyp, oo, " oo");
      // printf("dispCCV[%d] typ=%d dbi=%ld ptNr=%d\n",i1,otyp,pcc->dbi,ptNr);


    // change trimmed curve into standard curve
    irc = UTO_cv_cvtrm (&otyp, cv1, oo, pcc);
    if(irc < 0) return -1;

    //----------------------------------------------------
/*
    if(pcc->typ == Typ_PT) {
      pta[ptNr] = *((Point*)oo);
        // printf("pt[%d]=%f %f %f\n",ptNr,pta[ptNr].x,pta[ptNr].y,pta[ptNr].z);
      ++ptNr;

    //----------------------------------------------------
    } else 
*/
    if(pcc->typ == Typ_LN) {
      // add Startpunkt
      if(i1 == 0) {   // first segment: add also startpoint
        pta[ptNr] = ((Line*)cv1)->p1;
          // GR_Disp_pt (&pta[ptNr], SYM_STAR_S, ATT_COL_RED);
        ++ptNr;
      }
      // add endpoint
      pta[ptNr] = ((Line*)cv1)->p2;
        // GR_Disp_pt (&pta[ptNr], SYM_STAR_S, ATT_COL_RED);
      ++ptNr;


    //----------------------------------------------------
    } else if(pcc->typ == Typ_CI) {
      // CIR -> Polygon

      i2 = UT3D_ptNr_ci ((Circ*)cv1, tol);
      if(iMax < i2) goto L_EOM;

      if(i1 > 0) --ptNr;   // first point already exists
      // UT3D_cv_ci (&pta[ptNr], &i2, oo, i2, -1.);
      UT3D_npt_ci (&pta[ptNr], i2, (Circ*)cv1);
      ptNr += i2;
        // printf(" CCV + %d pts f. circ\n",i2);


    //----------------------------------------------------
    } else if(otyp == Typ_CVPOL) {
      if(i1 > 0) --ptNr;   // first point already exists
      // plg -> Polygon
      i2 = iMax;
      irc = UT3D_pta_plg (&i2, &pta[ptNr], (CurvPoly*)cv1);
      ptNr += i2;


    //----------------------------------------------------
    } else if(otyp == Typ_CVELL) {
      if(i1 > 0) --ptNr;   // first point already exists
      // ELL -> Polygon; Toleranz ??
      // UT3D_cv_ell (&pta[ptNr], &i2, vp1, iMax, tol);
      i2 = UT3D_ptNr_ell ((CurvElli*)cv1, tol);
      if(i2 > iMax) goto L_EOM;
      UT3D_npt_ell (i2, &pta[ptNr], (CurvElli*)cv1);
      ptNr += i2;

/*
    //----------------------------------------------------------------
    } else if(otyp == Typ_CVCLOT) {
      if(i1 > 0) --ptNr;   // den ersten Punkt gibts schon !!
      i2 = iMax;
      irc = UT3D_npt_curvp  (&pta[ptNr], &i2,
                             Typ_CVCLOT, vp1, tol);
      ptNr += i2;
*/

    //----------------------------------------------------
    } else if(otyp == Typ_CVBSP) {
      // den ersten Punkt gibts schon !!
      if(i1 > 0) --ptNr;
      // BSP -> Polygon
      // irc = bspl_pol_bsp (&i2, &pta[ptNr], ox3->data, iMax, tol);
      irc = UT3D_pta_bsp (&i2, &pta[ptNr], (CurvBSpl*)cv1, iMax, tol);
      ptNr += i2;


    //----------------------------------------------------
    } else if(otyp == Typ_CVRBSP) {
      // den ersten Punkt gibts schon !!
      if(i1 > 0) --ptNr;
      // BSP -> Polygon
      // irc = UT3D_cv_rbsp (&i2, &pta[ptNr], ox3->data, iMax, tol);
      irc = UT3D_cv_rbsp (&i2, &pta[ptNr], NULL, (CurvRBSpl*)cv1, iMax, tol);
      ptNr += i2;


      // //----------------------------------------------------
      // } else { TX_Error("UT3D_pta_ccv ESTYP_%d",otyp); return -1; }


    //----------------------------------------------------
    } else if(pcc->typ == Typ_Data) {    // noch nix tun ..
      // printf(" ccv-trim \n");


    //----------------------------------------------------
    } else {
      TX_Print("**** UT3D_pta_ccv E001_%d",pcc->typ);
      // exit (0); // TEST
    }

    if(iLim > 0) {
      limTab[i1] = ptNr-1;
        // printf(" limTab[%d] = %d\n",i1,limTab[i1]);
    }

  }


  *ptn = ptNr;

    // GR_Disp_pTab (ptNr, pta, SYM_STAR_S, 2);
    // for(i1=0;i1<ptNr;++i1) UT3D_stru_dump(Typ_PT,&pta[i1],"pta[%d]",i1);
    // printf("ex UT3D_pta_ccv irc=%d ptNr=%d\n",irc,ptNr);

  return irc;


  L_EOM:
  TX_Error("UT3D_pta_ccv EOM");
  return -1;


  L_LIM:
  TX_Error("UT3D_pta_ccv LIM-EOM");
  return -1;


}



/*
//===========================================================================
  int UT3D_pta_ccv (int *ptn, Point* pta, ObjGX *ccv,
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



  // printf("UT3D_pta_ccv: siz=%d iLim=%d ptMax=%d\n",ccv->siz,iLim,*ptn);
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
      irc = UT3D_npt_curvp  (&pta[ptNr], &i2,
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
      TX_Error("**** UT3D_pta_ccv E001_%d",ox2->typ);
      exit (0); // TEST
    }

    if(iLim > 0) {
      limTab[i1] = ptNr-1;
        // printf(" limTab[%d] = %d\n",i1,limTab[i1]);
    }

  }


  *ptn = ptNr;

    // printf("ex UT3D_pta_ccv:\n");
    // GR_Disp_pTab (ptNr, pta, SYM_STAR_S, 2);
    // for(i1=0;i1<ptNr;++i1) UT3D_stru_dump(Typ_PT,&pta[i1],"pta[%d]",i1);

  return 0;



  L_EOM:
  TX_Error("UT3D_pta_ccv EOM");
  return -1;


  L_LIM:
  TX_Error("UT3D_pta_ccv LIM-EOM");
  return -1;


}
*/


//===================== EOF ===========================
