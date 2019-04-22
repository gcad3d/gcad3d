//**************************************************************************
/*
 *
 * Copyright (C) 2015 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
 * Copyright (C) 2007 Thomas Backmeister (t.backmeister@gmx.at)
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
2007-08-25 UT_XP_MAX wieder raus; RF.
2007-08-17 NEU: USBS_IntsectLnBspSur, #define UT_XP_MAX 32.
2007-08-14 NEU: USBS_NxtPtBspSur.
2007-08-14 NEU: USBS_NxtPtBdryBspSur.
2007-08-11 NEU: #define NUM_IT 10000.
2007-08-09 Neue Konstante UT_TOL_0Cos.
2007-08-08 NEU: USBS_DerivBspSur.
2007-08-06 Änderung von Funktionsnamen:
           UT3D_pt_evparSurBSpl in USBS_EvPtBspSur;
           UT3D_testsurf        in USBS_TestPatsBspSur.
2007-08-04 In USBS_SkinSurBspCrvs:
	   UCBS_IntpolBspCrv ersetzt durch UCBS_CtrlPtsIntpol.           
2007-07-24 Änderung von Funktionsnamen:
           alle ubs_xxx, die bspline Flächen bearbeiten, in USBS_xxx.
2007-04-28 Änderung von Funktionsnamen:
           UT3D_bspl_sursubdiv   in ubs_DirIndSdBspSur
           UT3D_bspl_surnvpt     in ubs_NvPtBspSur
           UT3D_bspl_sursubdivuv in ubs_SubDivBspSur
           UT3D_bspl_tgisocrv    in ubs_TgVecIsoBspSur
           UT3D_bspl_surparpt    in ubs_UVPtBspSur
           UT3D_bspl_sur4div     in ubs_4QuBspSur

-----------------------------------------------------
*/
/*!
\file  ../ut/ubssur.c
\brief bspline surface functions
\code
=====================================================
List_functions_start:

USBS_pt_projptsbsp        project point --> B-SplSurf

USBS_CpyBspSur            Copy <-- bspline surface 
USBS_DegElevBspSur        Degree elevate <-- bspline surface 
USBS_DerivBspSur          Derivation <-- bspline surface
USBS_DifKntsKntVecBspSur  Difference knots <-- knotvector, bspline surface 
USBS_DirIndSdBspSur       Dir. indep. subdiv. <-- bspline surface
USBS_EvPtBspSur           Evaluate point <-- bspline surface 
USBS_GordSurBspCrvNet     Gordon bspline surface <-- bspline curve net 
USBS_IntpolBspSur         Interpolating bspline surface <-- points
USBS_IntsectLnBspSur      Intersection points <-- bspline surface & line 
USBS_LinCombBspSurs       Linear combination <-- bspline surfaces 
USBS_MkeCmpBspSurs        Compatible bspline surfaces <-- bspline surfaces 
USBS_MrgKntVecBspSurs     Merge U/V-knotvectors <-- bspline surfaces 
USBS_NxtPtBdryBspSur      Next point on boundary <-- bspline surface & point
USBS_NxtPtBspSur          Next point <-- bspline surface & point
USBS_NvPtBspSur           Normal vector <-- point on bspline surface
USBS_RefKntVecBspSur      Refine knotvector <-- bspline surface 
USBS_SkinSurBspCrvs       Skinned bspline surface <-- bspline curves
USBS_SubDivBspSur         Subdivision <-- bspline surface
USBS_SwUVBspSur           Switch UV <-- bspline surface 
USBS_TestPatsBspSur       Test patches <-- bspline surface
USBS_TgVecIsoBspSur       Tangent vector <-- isopar. crv. bspline surface
USBS_UVPtBspSur           UV-parameter <-- point on bspline surface
USBS_4QuBspSur            4 quarters <-- bspline surface

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



Returncodes:
 0 OK
>0 OK with additional info
<0 Error with additional info


compile with gcc:
cc -c -O0 -g3 -Wall ubssur.c

Windows:
cl -c ubssur.c

ctags -f ubssur.tag ubssur.c

*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

// #include "../ut/ut_umem.h"
#include "../ut/ut_geo.h"
#include "../ut/ut_math.h"
#include "../ut/ut_TX.h"
#include "../ut/ubs.h"
#include "../ut/ubssur.h"


#define NUM_IT     10000           // number of iterations


//================================================================
  int USBS_pt_projptsbsp (int *nxp, Point *ptab, double *pu, double *pv,
                          SurBSpl *sur1, Point *pt1,
                          Memspc *memSeg) {
//================================================================
// Wozu ist nu,nv genau ? mit 2 gehts zB nicht !

  int    irc, clu, clv, nu, nv;


  clu = 0;  // surface open in U-direction
  clv = 0;  // surface open in V-direction
  nu = 20;  // number of U-values for 1. solution
  nv = 20;  // number of V-values for 1. solution


  irc = USBS_NxtPtBspSur (ptab, pu, pv, sur1, clu, clv, pt1,
                   nu, nv, memSeg);
  if (irc < 0) { *nxp = 0; return -1;}

  *nxp = 1;

  return 0;

}




/*=======================================================================*/
  int USBS_IntsectLnBspSur (int *nxp, Point *xptab, 
                            int xpSiz, Line *ln, SurBSpl *sur,
                            void *memStart, int mi, Memspc *memSegT[4],
			                      Memspc *workSeg) {
/*========================
USBS_IntsectLnBspSur     Intersection points <-- bspline surface & line 

USBS_IntsectLnBspSur      Author: Thomas Backmeister       17.8.2007

Intersection of a line with a bspline surface.
The function works recursively!
First call with nxp = 0, memStart = NULL, mi = -1.

IN:
  int *nxp           ... size of xptab
  Point *xptab       ... intersection points (*nxp)
  Line *ln           ... line
  SurBSpl *sur       ... bspline surface
  void *memStart     ... start of surface space (or NULL)
  int mi             ... = 0,1,2,3: surface space = memSegT[mi];
                         = -1: <=> memStart = NULL
  Memspc *memSegT[4] ... space for subsurfaces 1-4 (and temp. workspace)
  Memspc *workSeg    ... temporary workspace
OUT:
  int *nxp           ... number of intersection points
  Point *xptab       ... intersection points
Returncodes:
  0 = OK
 -1 = internal error 
 -2 = X-point array too small
*/

  int rc, cpNr, i1, plex, piT[3];
  double u, v;
  void *memStartT[4];
  Point pmin, pmax, xp, gc;
  Plane pls;
  SurBSpl surT[4], *psurT[4];



  // printf("USBS_IntsectLnBspSur: siz=%d\n",xpSiz);

  // pointers of quarter surfaces
  for (i1=0; i1<4; ++i1) psurT[i1] = &surT[i1];

  // number of controlpoints of surface
  cpNr = sur->ptUNr * sur->ptVNr;

  // U/V-parameters for surface subdivision
  u = (sur->kvTabU[sur->ptUNr] + sur->kvTabU[sur->degU]) / 2.0;
  v = (sur->kvTabV[sur->ptVNr] + sur->kvTabV[sur->degV]) / 2.0;

  // bounding box of bspline surface
  rc = UT3D_box_pts (&pmin, &pmax, cpNr, sur->cpTab);
     // GR_Disp_box (&pmin, &pmax, 0);
  if (rc < 0) return -1;

  // check if line intersects bounding box
  if (!UT3D_ch_lnbox (ln, &pmin, &pmax, 0.)) goto L_End;
  // if (!UT3D_ch_lnbox (ln, &pmin, &pmax, UT_TOL_min2)) goto L_End;
  // if (!UT3D_ch_lnbox (ln, &pmin, &pmax, UT_TOL_pt)) goto L_End;  // LOOPT !

  // check planarity of bspline surface
  plex = UT3D_ChkPlanPts (piT, cpNr, sur->cpTab);
    // printf(" plex=%d\n",plex);


  //----------------------------------------------------------------
  if (plex == 0) {  // no plane

    // save start of work spaces
    for (i1=0; i1<4; ++i1) memStartT[i1] = memSegT[i1]->next;

    // subdivide bspline surface into 4 quarters
    rc = USBS_4QuBspSur (psurT, memSegT, sur, u, v, workSeg);
    if (rc < 0) return -1;

    // repeat this function with all 4 subsurfaces
    rc = USBS_IntsectLnBspSur (nxp, xptab, xpSiz, ln, psurT[0], memStartT[0], 0,
                               memSegT, workSeg);
    if (rc < 0) return -1;

    rc = USBS_IntsectLnBspSur (nxp, xptab, xpSiz, ln, psurT[1], memStartT[1], 1,
                               memSegT, workSeg);
    if (rc < 0) return -1;

    rc = USBS_IntsectLnBspSur (nxp, xptab, xpSiz, ln, psurT[2], memStartT[2], 2,
                               memSegT, workSeg);
    if (rc < 0) return -1;

    rc = USBS_IntsectLnBspSur (nxp, xptab, xpSiz, ln, psurT[3], memStartT[3], 3,
                               memSegT, workSeg);
    if (rc < 0) return -1;
    goto L_End;


  //----------------------------------------------------------------
  } else if (plex == 1) {  // limited plane

    // create plane
    rc = UT3D_pl_3pt (&pls, &(sur->cpTab[piT[0]]), &(sur->cpTab[piT[1]]),
		            &(sur->cpTab[piT[2]]));
    if (rc < 0) return -1;

    // intersect line with plane
    rc = UT3D_pt_intlnpl (&xp, &pls, ln);
    if (rc != 1) goto L_End; // line and plane parallel

    // check if X-point is close enough to all surface controlpoints
    UT3D_GravCentPts (&gc, cpNr, sur->cpTab);
    // if (UT3D_len_2pt (&xp, &gc) >= UT_TOL_pt) goto L_End; //
       // UT_TOL_pt: problem with very low Modelsize ! 
    if (UT3D_len_2pt (&xp, &gc) >= UT_TOL_cv) goto L_End; 


  //----------------------------------------------------------------
  } else if (plex == 2) {  // collinear controlpoints
    printf("USBS_IntsectLnBspSur: collinearity should not happen!\n");	  
    goto L_End;


  //----------------------------------------------------------------
  } else {  // identical controlpoints
    xp = sur->cpTab[0];
  }


  //----------------------------------------------------------------
  // check size of X-point array
  if (*nxp >= xpSiz) {
    TX_Error("USBS_IntsectLnBspSur: X-point array too small");
    return -2;
  }

  // skip point already found  RF
  for (i1=0; i1<*nxp; ++i1) {
    if(UT3D_comp2pt(&xp, &xptab[i1], UT_TOL_pt) == 1) goto L_End;
  }
    // DEB_dump_obj__ (Typ_PT, &xp, "  xp[%d]=",*nxp);

  // save intersection point
  xptab[*nxp] = xp;
  ++(*nxp);


  //----------------------------------------------------------------
  L_End:

  if (memStart != NULL && mi >= 0) {
    // release space of sur
    memSegT[mi]->next = memStart;
  }
  
  return 0;
}


/*=======================================================================*/
  int USBS_NxtPtBdryBspSur (Point *Pn, double *tn, SurBSpl *sur, int bd,
			    Point *P, Memspc *workSeg) {
/*========================
USBS_NxtPtBdryBspSur    Next point on boundary <-- bspline surface & point

USBS_NxtPtBdryBspSur     Author: Thomas Backmeister       14.8.2007

Computation of the next point to the given point on one of the
surface's boundaries.
If the bspline surface is parameterized by [a,b] x [c,d], the four
boundaries are isoparametric bspline curves at constant parameter
a, b, c, d resp.

If the degree of the boundary curve is 1, the result might be wrong!
(see UCBS_NxtPtBspCrv)

IN:
  SurBSpl *sur    ... bspline surface
  int bd          ... 0/1/2/3 <=> boundary with const. param. a/b/c/d
  Point *P        ... a point
  Memspc *workSeg ... workspace
OUT:
  Point *Pn       ... next point to P on boundary curve bd
  double *tn      ... parameter of Pn on boundary curve bd
Returncodes:
  0 = OK
 -1 = internal error
*/

  int rc, uv;	
  double tc; 
  CurvBSpl bdcv; 
  void *workStart;

  //printf("\nUSBS_NxtPtBdryBspSur: Next pt on bndry <-- bspline surf & pt\n");

  // isoparameter and isodirection
  if (bd == 0) {tc = sur->kvTabU[0]; uv = 1;}
  else if (bd == 1) {tc = sur->kvTabU[sur->ptUNr + sur->degU]; uv = 1;}
  else if (bd == 2) {tc = sur->kvTabV[0]; uv = 2;}
  else {tc = sur->kvTabV[sur->ptVNr + sur->degV]; uv = 2;}

  // save start of workspace
  workStart = workSeg->next;

  // boundary curve
  rc = UCBS_IsoBspCrvBspSur (&bdcv, workSeg, sur, tc, uv);	    
  if (rc < 0) return -1;

  // next point to P on boundary curve
  rc = UCBS_NxtPtBspCrv (Pn, tn, &bdcv, P, workSeg);
  if (rc < 0) return -1;

  // release workspace
  workSeg->next = workStart;

  return 0;
}


/*=======================================================================*/
  int USBS_NxtPtBspSur (Point *PP, double *ppu, double *ppv, SurBSpl *sur,
		        int clu, int clv, Point *P, int nu, int nv,
		        Memspc *workSeg) {
/*====================
USBS_NxtPtBspSur     Next point <-- bspline surface & point

USBS_NxtPtBspSur      Author: Thomas Backmeister       8.8.2007

Computation of the next point to the given point on a bspline surface.
The search works inside the surface and on all of its boundaries.
The surface may have any U- or V-degree >= 1.
The limiting parameters of the surface are not considered.

ATTENTION: the function may give wrong results in case of a surface
with degree 1 in U- or V-direction which is NOT SMOOTH (not differenciable).
This special case can be solved in a future work (if necessary).
If however the surface has degree 1 in any direction, but is SMOOTH,
the algorithm works correctly.

IN:
  SurBSpl *sur    ... bspline surface
  int clu         ... 1/0 <=> surface closed in U-direction Y/N
  int clv         ... 1/0 <=> surface closed in V-direction Y/N
  Point *P        ... a point
  int nu,nv       ... number of U- resp. V-values for computation
                      of start parameters (>=3)
  Memspc *workSeg ... workspace
OUT:
  Point *PP       ... next point on the surface
  double *ppu     ... U-parameter of PP
  double *ppv     ... V-parameter of PP
Returncodes:
  0 = OK
 -1 = input error
 -2 = internal error
*/

  int rc, mU, mV, i1, i2, itex, bdua, bdub, bdvc, bdvd;
  double du, dv, u, v, u0, v0, uold, vold, lr, lrmin;
  double J[4], d[2], k[2], dPP, tua, tub, tvc, tvd;
  void *workStart1, *workStart2;
  Point S, Pu, Pv, Puu, Puv, Pvu, Pvv, Pua, Pub, Pvc, Pvd;
  Vector r, Su, Sun, Sv, Svn, Suu, Suv, Svu, Svv, Su2, Sv2, Ss;
  Vector Suold, Svold;
  SurBSpl sur1, sur2, suru, surv, suruu, suruv, survu, survv;

  // printf("\nUSBS_NxtPtBspSur: Next point <-- bspline surface & point\n");

  // check input
  if (nu < 3 || nv < 3) return -1;

  // save start 1 of workspace
  workStart1 = workSeg->next;

  // degree elevate surface if necessary
  if (sur->degU == 1) {
    rc = USBS_DegElevBspSur (&sur1, workSeg, sur, 1, 0);
    if (rc < 0) return -2;
    sur = &sur1;
  }	  
  if (sur->degV == 1) {
    rc = USBS_DegElevBspSur (&sur2, workSeg, sur, 1, 1);
    if (rc < 0) return -2;
    sur = &sur2;
  }

  // save start 2 of workspace
  workStart2 = workSeg->next;

  // length of U/V-knotvectors
  mU = sur->ptUNr + sur->degU + 1;
  mV = sur->ptVNr + sur->degV + 1;

  // some derivated surfaces
  rc = USBS_DerivBspSur (&suru, workSeg, sur, 0);
  if (rc < 0) return -2; 
  rc = USBS_DerivBspSur (&surv, workSeg, sur, 1);
  if (rc < 0) return -2; 
  rc = USBS_DerivBspSur (&suruu, workSeg, &suru, 0);
  if (rc < 0) return -2; 
  rc = USBS_DerivBspSur (&suruv, workSeg, &suru, 1);
  if (rc < 0) return -2; 
  rc = USBS_DerivBspSur (&survu, workSeg, &surv, 0);
  if (rc < 0) return -2; 
  rc = USBS_DerivBspSur (&survv, workSeg, &surv, 1);
  if (rc < 0) return -2; 

  // start parameters u,v from closest point on surface 
  du = (sur->kvTabU[mU-1] - sur->kvTabU[0]) / (double)(nu - 1);
  dv = (sur->kvTabV[mV-1] - sur->kvTabV[0]) / (double)(nv - 1);
  lrmin = UT_VAL_MAX;
  u0 = sur->kvTabU[0];
  v0 = sur->kvTabV[0];
  u = sur->kvTabU[0];
  for (i1=0; i1<nu; ++i1) {
    v = sur->kvTabV[0];	  
    for (i2=0; i2<nv; ++i2) {
      rc = USBS_EvPtBspSur (&S, sur, u, v, workSeg);
      if (rc < 0) return -2; 
      lr = UT3D_len_2pt (P, &S);
      if (lr < lrmin) {
        u0 = u;
	v0 = v;
	*PP = S;
	lrmin = lr;
      }	      
      v += dv;
    }
    u += du;
  }    
  u = u0;
  v = v0;

  // first solution
  *ppu = u;
  *ppv = v;
  //dPP = UT3D_len_2pt (P, PP);
  //printf("1. solution: (u,v)= %f %f, distance P->PP: %f\n", u, v, dPP);


  // ----------------
  // Newton iteration
  // ----------------
  itex = 0;
  for (i1=0; i1<NUM_IT; ++i1) {

    // point S and vectors r,Su,Sv,Suu,Suv,Svu,Svv
    rc = USBS_EvPtBspSur (&S, sur, u, v, workSeg);
    if (rc < 0) return -2; 
    UT3D_vc_2pt (&r, P, &S);
    UT3D_vc_normalize (&r, &r);
    rc = USBS_EvPtBspSur (&Pu, &suru, u, v, workSeg);
    if (rc < 0) return -2; 
    UT3D_vc_pt (&Su, &Pu);
    rc = USBS_EvPtBspSur (&Pv, &surv, u, v, workSeg);
    if (rc < 0) return -2; 
    UT3D_vc_pt (&Sv, &Pv);
    rc = USBS_EvPtBspSur (&Puu, &suruu, u, v, workSeg);
    if (rc < 0) return -2; 
    UT3D_vc_pt (&Suu, &Puu);
    rc = USBS_EvPtBspSur (&Puv, &suruv, u, v, workSeg);
    if (rc < 0) return -2; 
    UT3D_vc_pt (&Suv, &Puv);
    rc = USBS_EvPtBspSur (&Pvu, &survu, u, v, workSeg);
    if (rc < 0) return -2; 
    UT3D_vc_pt (&Svu, &Pvu);
    rc = USBS_EvPtBspSur (&Pvv, &survv, u, v, workSeg);
    if (rc < 0) return -2; 
    UT3D_vc_pt (&Svv, &Pvv);

    // check if solution found
    if (UT3D_comp2pt (P, &S, UT_TOL_pt)) {
      *PP = S;
      *ppu = u;
      *ppv = v;
      itex = 1;
      //printf("USBS_NxtPtBspSur: iteration exit 1\n");
      goto L_CheckBoundaries;
    }	  
    UT3D_vc_normalize (&Sun, &Su);
    UT3D_vc_normalize (&Svn, &Sv);
    if (fabs(UT3D_acos_2vc (&r, &Sun)) < UT_TOL_0Cos) {
      if (fabs(UT3D_acos_2vc (&r, &Svn)) < UT_TOL_0Cos) {
        *PP = S;
        *ppu = u;
        *ppv = v;
	itex = 2;
        //printf("USBS_NxtPtBspSur: iteration exit 2\n");
        goto L_CheckBoundaries;
      }
    }      

    // linear equations
    J[0] = UT3D_acos_2vc (&Su, &Su) + UT3D_acos_2vc (&r, &Suu);
    J[1] = UT3D_acos_2vc (&Su, &Sv) + UT3D_acos_2vc (&r, &Suv);
    J[2] = UT3D_acos_2vc (&Su, &Sv) + UT3D_acos_2vc (&r, &Svu);
    J[3] = UT3D_acos_2vc (&Sv, &Sv) + UT3D_acos_2vc (&r, &Svv);
    k[0] = -UT3D_acos_2vc (&r, &Su);
    k[1] = -UT3D_acos_2vc (&r, &Sv);

    // solve linear equations	  
    rc = UTM_SolLinEquSys (d, 2, J, k);
    if (rc < 0) return -2;

    // save u,v and Su,Sv
    uold = u;
    vold = v;
    Suold = Su;
    Svold = Sv;

    // new parameters u,v
    u += d[0];
    v += d[1];

    // u,v must stay in range
    if (clu == 0) {
      if (u < sur->kvTabU[0]) u = sur->kvTabU[0];
      if (u > sur->kvTabU[mU-1]) u = sur->kvTabU[mU-1]; 
    }
    else {
      if (u < sur->kvTabU[0]) u = sur->kvTabU[mU-1] - sur->kvTabU[0] + u;
      if (u > sur->kvTabU[mU-1]) u = sur->kvTabU[0] - sur->kvTabU[mU-1] + u; 
    }	    
    if (clv == 0) {
      if (v < sur->kvTabV[0]) v = sur->kvTabV[0];
      if (v > sur->kvTabV[mV-1]) v = sur->kvTabV[mV-1]; 
    }
    else {
      if (v < sur->kvTabV[0]) v = sur->kvTabV[mV-1] - sur->kvTabV[0] + v;
      if (v > sur->kvTabV[mV-1]) v = sur->kvTabV[0] - sur->kvTabV[mV-1] + v; 
    }	    

    // halt if u,v dont change significantly
    UT3D_vc_multvc (&Su2, &Suold, (u-uold));
    UT3D_vc_multvc (&Sv2, &Svold, (v-vold));
    UT3D_vc_add2vc (&Ss, &Su2, &Sv2);
    if (UT3D_len_vc (&Ss) < UT_TOL_pt) {
      *PP = S;
      *ppu = u;
      *ppv = v;
      itex = 3;
      //printf("USBS_NxtPtBspSur: iteration exit 3\n");
      goto L_CheckBoundaries;
    }	    
  } // End of Newton iteration
  // -------------------------

L_CheckBoundaries:  

  /*
  if (itex == 0) printf("USBS_NxtPtBspSur: only 1. solution found\n");
  else {
    dPP = UT3D_len_2pt (P, PP);
    printf("Newton iter. sol.: (u,v)= %f %f, distance P->PP: %f\n",
           *ppu, *ppv, dPP);
  } 
  */  

  // release workspace 2
  workSeg->next = workStart2;

  if (itex == 1 || itex == 2) {
    // no better solution possible
    goto L_End;
  }	  

  // improve solution on surface boundary
  // ------------------------------------
  //printf("UT_TOL_min1= %1.10f\n", UT_TOL_min1);
  bdua = 0;
  bdub = 0;
  bdvc = 0;
  bdvd = 0;
  if (UTP_comp2db (*ppu, sur->kvTabU[0], UT_TOL_min1) && clv == 0) {
    // next point to P on Ua-boundary
    bdua = 1;	  
    rc = USBS_NxtPtBdryBspSur (&Pua, &tua, sur, 0, P, workSeg);
    if (rc < 0) return -2;
  }
  if (UTP_comp2db (*ppu, sur->kvTabU[mU-1], UT_TOL_min1) && clv == 0) {
    // next point to P on Ub-boundary
    bdub = 1;	  
    rc = USBS_NxtPtBdryBspSur (&Pub, &tub, sur, 1, P, workSeg);
    if (rc < 0) return -2;
  }
  if (UTP_comp2db (*ppv, sur->kvTabV[0], UT_TOL_min1) && clu == 0) {
    // next point to P on Vc-boundary
    bdvc = 1;	  
    rc = USBS_NxtPtBdryBspSur (&Pvc, &tvc, sur, 2, P, workSeg);
    if (rc < 0) return -2;
  }
  if (UTP_comp2db (*ppv, sur->kvTabV[mV-1], UT_TOL_min1) && clu == 0) {
    // next point to P on Vd-boundary
    bdvd = 1;	  
    rc = USBS_NxtPtBdryBspSur (&Pvd, &tvd, sur, 3, P, workSeg);
    if (rc < 0) return -2;
  }

  // best point on surface boundary
  if (bdua == 1) {
    if (bdvc == 1) {
      if (UT3D_len_2pt (P, &Pua) < UT3D_len_2pt (P, &Pvc)) {
        *PP = Pua;
	*ppv = tua;
      }
      else {
        *PP = Pvc;
	*ppu = tvc;
      }	      
    }
    else if (bdvd == 1) {
      if (UT3D_len_2pt (P, &Pua) < UT3D_len_2pt (P, &Pvd)) {
        *PP = Pua;
	*ppv = tua;
      }
      else {
        *PP = Pvd;
	*ppu = tvd;
      }	      
    }	   
    else {
      *PP = Pua;
      *ppv = tua;      
    }	    
  }
  else if (bdub == 1) {
    if (bdvc == 1) {
      if (UT3D_len_2pt (P, &Pub) < UT3D_len_2pt (P, &Pvc)) {
        *PP = Pub;
	*ppv = tub;
      }
      else {
        *PP = Pvc;
	*ppu = tvc;
      }	      
    }
    else if (bdvd == 1) {
      if (UT3D_len_2pt (P, &Pub) < UT3D_len_2pt (P, &Pvd)) {
        *PP = Pub;
	*ppv = tub;
      }
      else {
        *PP = Pvd;
	*ppu = tvd;
      }	      
    }	   
    else {
      *PP = Pub;
      *ppv = tub;
    }	    
  }
  else {
    if (bdvc == 1) {
      *PP = Pvc;
      *ppu = tvc;
    }
    else if (bdvd == 1) {
      *PP = Pvd;
      *ppu = tvd;
    }	    
  }	  
  /*
  if (bdua == 1 || bdub == 1 || bdvc == 1 || bdvd == 1) {
    dPP = UT3D_len_2pt (P, PP);
    printf("bdry optim. sol.: (u,v)= %f %f, distance P->PP: %f\n",
	    *ppu, *ppv, dPP);
  }
  */

L_End:  

  // release workspace 1
  workSeg->next = workStart1;

  return 0;
}


/*=======================================================================*/
  int USBS_DerivBspSur (SurBSpl *dsur, Memspc *memSeg, SurBSpl *sur, int uv) {
/*====================
USBS_DerivBspSur     Derivation <-- bspline surface

USBS_DerivBspSur      Author: Thomas Backmeister       7.8.2007

U- resp. V-Derivation bspline surface of a bspline surface.
A point with parameters u,v of this surface is the U- resp. V-tangent
vector of the given surface at the same parameter values.

IN:
  SurBSpl *sur    ... bspline surface
  int uv          ... 0/1 <=> derivation in U/V-direction
OUT:
  SurBSpl *dsur   ... derivated bspline surface
  Memspc *memSeg  ... data space of dsur (and workspace)
Returncodes:
  0 = OK
 -1 = out of space
*/

  int rc, mU, mV, i1, i2;
  double *dT, d;  
  void *memStart;

  //printf("USBS_DerivBspSur: Derivation <-- bspline surface\n");

  // init dsur
  if (uv == 0) {	  
    dsur->ptUNr = sur->ptUNr - 1;
    dsur->ptVNr = sur->ptVNr;
    dsur->degU = sur->degU - 1;
    dsur->degV = sur->degV;
  }
  else {
    dsur->ptUNr = sur->ptUNr;
    dsur->ptVNr = sur->ptVNr - 1;
    dsur->degU = sur->degU;
    dsur->degV = sur->degV - 1;
  }	  
  dsur->v0U = sur->v0U;
  dsur->v1U = sur->v1U;
  dsur->v0V = sur->v0V;
  dsur->v1V = sur->v1V;

  // space for U/V-knotvectors
  mU = dsur->ptUNr + dsur->degU + 1;
  dsur->kvTabU = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * mU);
  if (rc < 0) goto L_outOfSpace;
  mV = dsur->ptVNr + dsur->degV + 1;
  dsur->kvTabV = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * mV);
  if (rc < 0) goto L_outOfSpace;

  // space for controlpoints
  dsur->cpTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(Point) * dsur->ptUNr * dsur->ptVNr);
  if (rc < 0) goto L_outOfSpace;

  // U/V-knotvectors
  if (uv == 0) {
    for (i1=0; i1<mU; ++i1) dsur->kvTabU[i1] = sur->kvTabU[i1+1];
    for (i1=0; i1<mV; ++i1) dsur->kvTabV[i1] = sur->kvTabV[i1];
  }
  else {
    for (i1=0; i1<mU; ++i1) dsur->kvTabU[i1] = sur->kvTabU[i1];
    for (i1=0; i1<mV; ++i1) dsur->kvTabV[i1] = sur->kvTabV[i1+1];
  }	  

  // get workspace
  memStart = memSeg->next;
  dT = memSeg->next;
  if (uv == 0) {
    rc = UME_add (memSeg, sizeof(double) * (sur->ptUNr-1));
    if (rc < 0) goto L_outOfSpace;
  }
  else {
    rc = UME_add (memSeg, sizeof(double) * (sur->ptVNr-1));
    if (rc < 0) goto L_outOfSpace;
  }	  
  
  // controlpoints
  if (uv == 0) {
    for (i2=0; i2<sur->ptUNr-1; ++i2)
      dT[i2] = (double)sur->degU /
               (sur->kvTabU[i2+sur->degU+1] - sur->kvTabU[i2+1]);
    for (i1=0; i1<sur->ptVNr; ++i1) {
      for (i2=0; i2<sur->ptUNr-1; ++i2) {	    
        UT3D_pt_LinComb2Pts (&(dsur->cpTab[i1*dsur->ptUNr+i2]),
	                  1.0, &(sur->cpTab[i1*sur->ptUNr+i2+1]),
		         -1.0, &(sur->cpTab[i1*sur->ptUNr+i2]));
        UT3D_pt_multpt (&(dsur->cpTab[i1*dsur->ptUNr+i2]), 
       	                &(dsur->cpTab[i1*dsur->ptUNr+i2]), dT[i2]);
      }
    }      
  }
  else {
    for (i2=0; i2<sur->ptVNr-1; ++i2)
      dT[i2] = (double)sur->degV /
               (sur->kvTabV[i2+sur->degV+1] - sur->kvTabV[i2+1]);
    for (i1=0; i1<sur->ptUNr; ++i1) {
      for (i2=0; i2<sur->ptVNr-1; ++i2) {	    
        UT3D_pt_LinComb2Pts (&(dsur->cpTab[i2*dsur->ptUNr+i1]),
		          1.0, &(sur->cpTab[(i2+1)*sur->ptUNr+i1]),
		         -1.0, &(sur->cpTab[i2*sur->ptUNr+i1]));
        UT3D_pt_multpt (&(dsur->cpTab[i2*dsur->ptUNr+i1]), 
			&(dsur->cpTab[i2*dsur->ptUNr+i1]), dT[i2]);
      }
    }      
  }	  

  // release workspace
  memSeg->next = memStart;

  return 0;

L_outOfSpace:
  TX_Error("USBS_DerivBspSur: out of space");
  return -1;
}


/*=======================================================================*/
  int USBS_TestPatsBspSur (SurBSpl *sur, Memspc *workSeg) {
/*=======================
USBS_TestPatsBspSur     Test patches <-- bspline surface

USBS_TestPatsBspSur      Author: Thomas Backmeister       13.9.2003

IN:
  SurBSpl *sur    ... bspline surface
  Memspc *workSeg ... work space
OUT:
Returncodes:
  0 = OK
 -1 = internal error
*/

  int rc, p, q;
  long nbsegU, nbsegV;
  double ua, ub, va, vb, s, t, u, v;
  Point pt;

  //printf("USBS_TestPatsBspSur: Test patches <-- bspline surface\n");

  nbsegU = sur->ptUNr - sur->degU;
  nbsegV = sur->ptVNr - sur->degV;
  //printf("number of u-segments= %d\n", nbsegU);
  //printf("number of v-segments= %d\n", nbsegV);

  for (q=0; q<nbsegV; ++q) {

    // v-segment start- and endparameter
    va = sur->kvTabV[sur->degV+q];
    vb = sur->kvTabV[sur->degV+q+1];

    for (p=0; p<nbsegU; ++p) {

      // u-segment start- and endparameter
      ua = sur->kvTabU[sur->degU+p];
      ub = sur->kvTabU[sur->degU+p+1];

      // ---- TEST (p,q)-PATCH ----
      //printf("\n(p,q)-PATCH (%d, %d)\n", p, q);
      for (s=0.0; s<=1.0; s+=0.20) {
        v = (1.0-s) * va + s * vb;
        for (t=0.0; t<=1.0; t+=0.20) {
          u = (1.0-t) * ua + t * ub;
          rc = USBS_EvPtBspSur (&pt, sur, u, v, workSeg);
          if (rc < 0) return -1;
          //printf("(u,v)= (%f,%f)   pt= %f %f %f\n",u,v,pt.x,pt.y,pt.z);
          // cre_obj (Typ_PT, Typ_PT, 1, (void*)&pt);
        }
      }
    }
  }
  return 0;
}


/*=======================================================================*/
  int USBS_EvPtBspSur (Point *pt, SurBSpl *sur, double u, double v,
                       Memspc *workSeg) {
/*===================
USBS_EvPtBspSur     Evaluate point <-- bspline surface 

USBS_EvPtBspSur      Author: Thomas Backmeister       13.9.2003

Evaluate a point on a bspline surface.

IN:
  SurBSpl *sur    ... bspline surface
  double u        ... 1. coordinate parameter value
  double v        ... 2. coordinate parameter value
  Memspc *workSeg ... work space
OUT:
  Point *pt       ... surface point
Returncodes:
  0 = OK
 -1 = out of work space
 -2 = internal error
*/

  int rc, v1;
  void *memstart;
  Point *qTab;
  CurvBSpl bspl;

  //printf("USBS_EvPtBspSur: Evaluate point <-- bspline surface\n");

  // space for v-curve control points
  memstart = workSeg->next;
  qTab = workSeg->next;
  rc = UME_add (workSeg, sizeof(Point) * sur->ptVNr);
  if (rc < 0) goto L_outOfWorkSpace;

  // init bspline u-curve
  bspl.ptNr = sur->ptUNr;
  bspl.deg = sur->degU;
  bspl.kvTab = sur->kvTabU;

  for (v1=0; v1<sur->ptVNr; ++v1) {

    // control points of u-curve
    bspl.cpTab = &(sur->cpTab[v1*sur->ptUNr]);

    // evaluate point on u-curve
    rc = UCBS_EvPtBspCrv (&(qTab[v1]), &bspl, u, workSeg);
    if (rc < 0) return -2;
  }

  // init bspline v-curve
  bspl.ptNr = sur->ptVNr;
  bspl.deg = sur->degV;
  bspl.kvTab = sur->kvTabV;
  bspl.cpTab = qTab;

  // evaluate point on v-curve
  rc = UCBS_EvPtBspCrv (pt, &bspl, v, workSeg);
  if (rc < 0) return -2;

  // release work space
  workSeg->next = memstart;

  return 0;

L_outOfWorkSpace:
  TX_Error("USBS_EvPtBspSur: out of work space");
  return -1;
}


/*=======================================================================*/
  int USBS_GordSurBspCrvNet (SurBSpl *gsur, Memspc *memSeg, int cvNrU,
		             CurvBSpl **cvTU, int cvNrV, CurvBSpl **cvTV,
			     int degU, int degV, Memspc *work1Seg,
			     Memspc *work2Seg) {
/*=========================
USBS_GordSurBspCrvNet     Gordon bspline surface <-- bspline curve net 

USBS_GordSurBspCrvNet      Author: Thomas Backmeister       20.7.2007

Gordon bspline surface from a net of U- and V-bspline curves.
The Gordon surface is L1 + L2 - T, where L1, L2 are skinned surfaces
through the V- resp. U-curves and T is a tensorproduct surface interpolating
the intersection points of the curve net.
The U-degree of the Gordon surface is max(dcU, degU), where dcU is the
maximal degree of all U-bspline curves.
The V-degree of the Gordon surface is max(dcV, degV), where dcV is the
maximal degree of all V-bspline curves.

The bspline curves may have any parameter range, knotvector and degree.
It is assumed that each (U,V)-curve pair has exactly one intersection point.
It is also assumed that the intersection points of any V-curve with all
U-curves have the same U-parameter value and the intersection points
of any U-curve with all V-curves have the same V-parameter value.
Further it is assumed that all U-curves are parameterized in the same
direction with respect to the V-curves starting at the first V-curve
and that all V-curves are parameterized in the same direction with respect
to the U-curves starting at the first U-curve.

IN:
  int cvNrU        ... number of bspline curves in U-direction (>1)
  CurvBSpl **cvTU  ... bspline curves in U-direction
  int cvNrV        ... number of bspline curves in V-direction (>1)
  CurvBSpl **cvTV  ... bspline curves in V-direction
  int degU         ... U-degree of L1 and T (< min{20,cvNrV})
  int degV         ... V-degree of L2 and T (< min{20,cvNrU})
  Memspc *work1Seg ... workspace 1
  Memspc *work2Seg ... workspace 2
OUT:
  SurBSpl *gsur    ... Gordon bspline surface
  Memspc *memSeg   ... data space of gsur (and workspace)
Returncodes:
  0 = OK
 -1 = input error
 -2 = internal error
 -3 = out of space
*/

  int rc, dcU, dcV, dgU, dgV, i1, i2, s1;	
  double *pvTU, *pvTV, aT[3]; 
  void *memStart, *work1Start; 
  CurvBSpl bspT1[TabS1], bspT2[TabS1], isocrv;
  SurBSpl L1, L2, T, *surTi[3], surTo[3];
  Point *XTab;


/*
  printf("======================================================== \n");
  printf("USBS_GordSurBspCrvNet uNr=%d vNr=%d degu=%d degv=%d\n",
         cvNrU,cvNrV,degU,degV);
  if (cvNrU == 2 && cvNrV == 2)
    printf("1. COONS bspline patch\n\n");
  else if ((cvNrU == 2 && cvNrV > 2) || (cvNrU > 2 && cvNrV == 2))
    printf("2. BIRAIL bspline surface\n\n");
  else 
    printf("3. GORDON bspline surface\n\n");	  
  for(i1=0; i1<cvNrU; ++i1) {
    printf("U1[%d] pt=%d\n",i1,cvTU[i1]->ptNr);
    DEB_dump_obj__ (Typ_CVBSP, cvTU[i1], "U1[%d]",i1);
  }
  for(i1=0; i1<cvNrV; ++i1) {
    printf("U2[%d] pt=%d\n",i1,cvTV[i1]->ptNr);
    DEB_dump_obj__ (Typ_CVBSP, cvTV[i1], "U2[%d]",i1);
  }
*/



  // check input
  if (cvNrU < 2) goto L_InErr;
  if (cvNrV < 2) goto L_InErr;
  if (degU < 1 || degU >= IMIN(20,cvNrV)) goto L_InErr;
  if (degV < 1 || degV >= IMIN(20,cvNrU)) goto L_InErr;

  // save start of memspaces
  memStart = memSeg->next;
  work1Start = work1Seg->next;


  // make U-curves compatible
  rc = UCBS_MkeCmpBspCrvs (bspT1, work1Seg, cvNrU, cvTU, 0, work2Seg);
  if (rc < 0) return -2;
  dcU = bspT1[0].deg;
    // printf("dcU = degree of compatible U-curves = %d\n\n", dcU);


  // make V-curves compatible
  rc = UCBS_MkeCmpBspCrvs (bspT2, work1Seg, cvNrV, cvTV, 0, work2Seg);
  if (rc < 0) return -2;
  dcV = bspT2[0].deg;
    // printf("dcV = degree of compatible V-curves = %d\n\n", dcV);
/*
    // TEST ONLY
    for(i1=0; i1<cvNrU; ++i1) {
      DEB_dump_obj__ (Typ_CVBSP, &bspT1[i1], "U1[%d]",i1);
    }
    for(i1=0; i1<cvNrV; ++i1) {
      DEB_dump_obj__ (Typ_CVBSP, &bspT2[i1], "U2[%d]",i1);
    }
    // TEST ONLY
*/



  // intersection points with U/V-parametervectors of compatible U/V-curves
  rc = UCBS_XPtsBspCrvNet (&XTab, &pvTU, &pvTV, memSeg, cvNrU, bspT1,
                           cvNrV, bspT2, work2Seg);
  if (rc < 0) return -2;

/*
    // TEST ONLY
    // test curve-curve intersection points
    for (i1=0; i1<cvNrU; ++i1) {
      for (i2=0; i2<cvNrV; ++i2) {
        DEB_dump_obj__ (Typ_PT, &XTab[i1*cvNrV+i2], "XTab U%d V%d:",i1,i2);
        // cre_obj (Typ_PT, Typ_PT, 1, (void*)&(XTab[i1*cvNrV+i2]));
        }	  
    } 
    // test U/V-parametervectors
    printf("\npvTU: \n");
    for (i2=0; i2<cvNrV; ++i2) printf("%f\n", pvTU[i2]);
    printf("\n\n");
    printf("\npvTV: \n");
    for (i1=0; i1<cvNrU; ++i1) printf("%f\n", pvTV[i1]);
    printf("\n\n");
    // TEST ONLY
*/


  // skinned surface L1 through V-curves
  rc = USBS_SkinSurBspCrvs (&L1, memSeg, cvNrV, bspT2, degU, 1, pvTU, 1,
		            work2Seg);
  if (rc < 0) return -2;
    // printf("skinned surface L1 through V-curves:\n");
    // printf("L1: degU=%d degV=%d ptU=%d ptV=%d\n",degU,dcV,L1.ptUNr,L1.ptVNr);
    // DEB_dump_obj__ (Typ_SURBSP, &L1, "L1:");
    //cre_obj (Typ_SURBSP, Typ_SURBSP, 1, &L1);



  // skinned surface L2 through U-curves
  rc = USBS_SkinSurBspCrvs (&L2, memSeg, cvNrU, bspT1, degV, 0, pvTV, 1,
		            work2Seg);
  if (rc < 0) return -2;
    // printf("skinned surface L2 through U-curves:\n");
    // printf("L2: degU=%d degV=%d ptU=%d ptV=%d\n",dcU,degV,L2.ptUNr,L2.ptVNr);
    // DEB_dump_obj__ (Typ_SURBSP, &L2, "L2");
    //cre_obj (Typ_SURBSP, Typ_SURBSP, 1, &L2);



  // release workspace 1
  work1Seg->next = work1Start;

  // tensorproduct surface T interpolating the intersection points
  // of U/V-curves
  rc = USBS_IntpolBspSur (&T, memSeg, cvNrV, cvNrU, XTab, degU, degV,
                          pvTU, pvTV, 1, work2Seg);
  if (rc < 0) return -2;
    // printf("tensorproduct surface T through X-points of U/V-curves:\n");
    //printf("(degU,degV)= (%d, %d)\n\n", degU, degV);
    //DEB_dump_obj__ (158, &T, "");
    //cre_obj (Typ_SURBSP, Typ_SURBSP, 1, &T);



  // U- and V-degree of Gordon surface
  dgU = IMAX (dcU, degU);
  dgV = IMAX (dcV, degV);
    // printf("dgU=max{dcU,degU}=max{%d,%d}=U-degree=%d\n", dcU,degU,dgU);
    // printf("dgV=max{dcV,degV}=max{%d,%d}=V-degree=%d\n\n", dcV,degV,dgV);



  // make L1, L2 and T compatible
  surTi[0] = &L1;
  surTi[1] = &L2;
  surTi[2] = &T;
  rc = USBS_MkeCmpBspSurs (surTo, work1Seg, 3, surTi, dgU, dgV, work2Seg);
    // printf(" after USBS_MkeCmpBspSurs %d\n",rc);
  if (rc < 0) return -2;

/*
  // TESTAUSGABEN
  for (s1=0; s1<3; ++s1) {
    printf("Sur %d: degU=%d degV=%d  ptU=%d ptV=%d\n", s1,
            surTo[s1].degU, surTo[s1].degV, surTo[s1].ptUNr, surTo[s1].ptVNr);
    // DEB_dump_obj__ (158, &(surTo[s1]), "");
    //cre_obj (Typ_SURBSP, Typ_SURBSP, 1, &(surTo[s1]));
  }	
  // TESTAUSGABEN
*/


  // release workspace
  memSeg->next = memStart;



  // Gordon surface <-- compatible L1,L2,T
  aT[0] = 1.0;
  aT[1] = 1.0;
  aT[2] = -1.0;
  rc = USBS_LinCombBspSurs (gsur, memSeg, 3, surTo, aT);
  if (rc < 0) return -2;


/*
  // U-isocurves of Gordon surface
  for (i1=0; i1<cvNrU; ++i1) {
    rc = UCBS_IsoBspCrvBspSur (&isocrv, memSeg, gsur, pvTV[i1], 2);
    if (rc < 0) return -2;
    cre_obj (Typ_CVBSP, Typ_CVBSP, 1, (void*)&isocrv);
    DEB_dump_obj__ (10, &isocrv, "");
  }
  // V-isocurves of Gordon surface
  for (i1=0; i1<cvNrV; ++i1) {
    rc = UCBS_IsoBspCrvBspSur (&isocrv, memSeg, gsur, pvTU[i1], 1);
    if (rc < 0) return -2;
    cre_obj (Typ_CVBSP, Typ_CVBSP, 1, (void*)&isocrv);
    DEB_dump_obj__ (10, &isocrv, "");
  }
*/

  // release workspace 1
  work1Seg->next = work1Start;

  return 0;

L_InErr:
  TX_Error("USBS_GordSurBspCrvNet: input error");
  return -1;

L_outOfSpace:
  TX_Error("USBS_GordSurBspCrvNet: out of space");
  return -3;
}


/*=======================================================================*/
  int USBS_LinCombBspSurs (SurBSpl *suro, Memspc *memSeg, int srNr,
                           SurBSpl *surTi, double *aT) {
/*=======================
USBS_LinCombBspSurs     Linear combination <-- bspline surfaces 

USBS_LinCombBspSurs      Author: Thomas Backmeister       28.6.2007

Linear combination of an array of compatible bspline surfaces.
Compatible means equal degrees, parameter domains and knotvectors.
The controlpoints of the surfaces are linearly combinated.

IN:
  int srNr         ... number of bspline surfaces
  SurBSpl *surTi   ... bspline surfaces
  double *aT       ... scalarfactors for linear combination
OUT:
  SurBSpl *suro    ... lin. comb. bspline surface
  Memspc *memSeg   ... data space of suro
Returncodes:
  0 = OK
 -1 = out of space
*/

  int m, rc, i1, s1;

  //printf("USBS_LinCombBspSurs: Linear combination <-- bspline surfaces\n");

  // init
  *suro = surTi[0];

  // U-knotvector
  m = suro->ptUNr + suro->degU + 1;
  suro->kvTabU = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * m);
  if (rc < 0) goto L_outOfSpace;
  for (i1=0; i1<m; ++i1) suro->kvTabU[i1] = surTi[0].kvTabU[i1];

  // V-knotvector
  m = suro->ptVNr + suro->degV + 1;
  suro->kvTabV = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * m);
  if (rc < 0) goto L_outOfSpace;
  for (i1=0; i1<m; ++i1) suro->kvTabV[i1] = surTi[0].kvTabV[i1];

  // controlpoints
  m = suro->ptUNr * suro->ptVNr;
  suro->cpTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(Point) * m);
  if (rc < 0) goto L_outOfSpace;
  if (srNr == 1) {
    for (i1=0; i1<m; ++i1)
      UT3D_pt_LinComb2Pts (&(suro->cpTab[i1]), aT[0], &(surTi[0].cpTab[i1]),
                                              0.0, &(surTi[0].cpTab[i1]));
  }	
  else {
    for (i1=0; i1<m; ++i1)
      UT3D_pt_LinComb2Pts (&(suro->cpTab[i1]), aT[0], &(surTi[0].cpTab[i1]),
                                            aT[1], &(surTi[1].cpTab[i1]));
    for (s1=2; s1<srNr; ++s1) {
      for (i1=0; i1<m; ++i1) {
        UT3D_pt_LinComb2Pts (&(suro->cpTab[i1]), 1.0, &(suro->cpTab[i1]),
                                           aT[s1], &(surTi[s1].cpTab[i1]));
      }
    }
  }  

  return 0;

L_outOfSpace:
  TX_Error("USBS_LinCombBspSurs: out of space");
  return -1;
}


/*=======================================================================*/
  int USBS_SwUVBspSur (SurBSpl *suro, Memspc *memSeg, SurBSpl *suri) {
/*===================
USBS_SwUVBspSur     Switch UV <-- bspline surface 

USBS_SwUVBspSur    Author: Thomas Backmeister       27.6.2007

Switch U- and V-direction of a bspline surface.

IN:
  SurBSpl *suri   ... bspline surface
OUT:
  SurBSpl *suro   ... suri with switched U-,V-directions
  Memspc *memSeg  ... data space of suro
Returncodes:
  0 = OK
 -1 = out of space
*/

  int rc, m, mU, mV, i1, j1, k1;

  //printf("USBS_SwUVBspSur: Switch UV <-- bspline surface\n");

  // init
  suro->ptUNr = suri->ptVNr;
  suro->ptVNr = suri->ptUNr;
  suro->degU = suri->degV;
  suro->degV = suri->degU;
  suro->v0U = suri->v0V;
  suro->v1U = suri->v1V;
  suro->v0V = suri->v0U;
  suro->v1V = suri->v1U;

  // U-knotvector
  mU = suri->ptVNr + suri->degV + 1;
  suro->kvTabU = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * mU);
  if (rc < 0) goto L_outOfSpace;
  for (i1=0; i1<mU; ++i1) suro->kvTabU[i1] = suri->kvTabV[i1];
 
  // V-knotvector
  mV = suri->ptUNr + suri->degU + 1;
  suro->kvTabV = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * mV);
  if (rc < 0) goto L_outOfSpace;
  for (i1=0; i1<mV; ++i1) suro->kvTabV[i1] = suri->kvTabU[i1];

  // controlpoints
  k1 = 0;
  m = suro->ptUNr * suro->ptVNr;
  suro->cpTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(Point) * m);
  if (rc < 0) goto L_outOfSpace;
  for (i1=0; i1<suri->ptUNr; ++i1) {
    for (j1=0; j1<suri->ptVNr; ++j1) {
      suro->cpTab[k1] = suri->cpTab[j1*suri->ptUNr+i1];
      ++k1;
    }      
  }    
  
  return 0;

L_outOfSpace:
  TX_Error("USBS_SwUVBspSur: out of space");
  return -1;
}


/*=======================================================================*/
  int USBS_CpyBspSur (SurBSpl *suro, Memspc *memSeg, SurBSpl *suri) {
/*==================
USBS_CpyBspSur     Copy <-- bspline surface 

USBS_CpyBspSur    Author: Thomas Backmeister       26.6.2007

Copy a bspline surface.

IN:
  SurBSpl *suri   ... bspline surface
OUT:
  SurBSpl *suro   ... copy of suri
  Memspc *memSeg  ... data space of suro
Returncodes:
  0 = OK
 -1 = out of space
*/

  int rc, m, i1;

  //printf("USBS_CpyBspSur: Copy <-- bspline surface\n");

  // init
  *suro = *suri;

  // U-knotvector
  m = suro->ptUNr + suro->degU + 1;
  suro->kvTabU = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * m);
  if (rc < 0) goto L_outOfSpace;
  for (i1=0; i1<m; ++i1) suro->kvTabU[i1] = suri->kvTabU[i1];

  // V-knotvector
  m = suro->ptVNr + suro->degV + 1;
  suro->kvTabV = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * m);
  if (rc < 0) goto L_outOfSpace;
  for (i1=0; i1<m; ++i1) suro->kvTabV[i1] = suri->kvTabV[i1];

  // controlpoints
  m = suro->ptUNr * suro->ptVNr;
  suro->cpTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(Point) * m);
  if (rc < 0) goto L_outOfSpace;
  for (i1=0; i1<m; ++i1) suro->cpTab[i1] = suri->cpTab[i1];
  
  return 0;

L_outOfSpace:
  TX_Error("USBS_CpyBspSur: out of space");
  return -1;
}


/*=======================================================================*/
  int USBS_DifKntsKntVecBspSur (int *nk, double **nkTab, Memspc *memSeg,
		                int mk, double *mkTab, SurBSpl *sur,
			        int dirUV) {
/*============================
USBS_DifKntsKntVecBspSur   Difference knots <-- knotvector, bspline surface 

USBS_DifKntsKntVecBspSur    Author: Thomas Backmeister       26.6.2007

Compute the difference of knotvectors mkTab - sur->kvTabU resp.
mkTab - sur->kvTabV.
It is assumed that each knot in sur->kvTabU resp. sur->kvTabV also occurs
in mkTab, considering multiplicities.
The difference knots are all knots in mkTab which are not in
sur->kvTabU resp. sur->kvTabV, e.g.:
(0,0,0,0,1,1,2,2,2,3,4,4,4,4) - (0,0,0,0,1,2,2,4,4,4,4) = (1,2,3).

IN:
  int mk          ... number of knots
  double *mkTab   ... array of knots
  SurBSpl *sur    ... bspline surface
  int dirUV       ... 0/1 <=> U/V-direction
OUT:
  int nk          ... number of difference knots
  double *nkTab   ... array of difference knots
  Memspc *memSeg  ... data space of nkTab
Returncodes:
  0 = OK
 -1 = out of space
*/
  
  int i1, i2, pm, m1;
  double *kvTab;
  void *rp;

  //printf("USBS_DifKntsKntVecBspSur: Diff. knots <-- knotvec., bspl. sur.\n");

  // init
  *nk = 0;
  *nkTab = memSeg->next;

  if (dirUV == 0) {
    m1 = sur->ptUNr + sur->degU + 1;
    kvTab = sur->kvTabU;
  }
  else {
    m1 = sur->ptVNr + sur->degV + 1;
    kvTab = sur->kvTabV;
  }	  
  pm = 0;
  for (i1=0; i1<m1; ++i1) {
    for (i2=pm; i2<mk; ++i2) {
      if (UTP_comp2db (kvTab[i1], mkTab[i2], UT_TOL_min1)) {
	if (i2-pm > 0) {      
          rp = UME_save (memSeg, &(mkTab[pm]), sizeof(double)*(i2-pm));
          if (rp == NULL) goto L_outOfSpace;
          *nk += (i2-pm);
        }
	pm = i2 + 1;
	break;
      }
    }
  }
  if (mk-pm > 0) { 
    rp = UME_save (memSeg, &(mkTab[pm]), sizeof(double)*(mk-pm));
    if (rp == NULL) goto L_outOfSpace;
    *nk += (mk-pm);
  }

  return 0;

L_outOfSpace:
  TX_Error("USBS_DifKntsKntVecBspSur: out of space");
  return -1;
}
  

/*=======================================================================*/
  int USBS_MrgKntVecBspSurs (int *mk, double **mkTab, Memspc *memSeg, int nsr,
		             SurBSpl *surTab, int dirUV, Memspc *workSeg) {
/*=========================
USBS_MrgKntVecBspSurs      Merge U/V-knotvectors <-- bspline surfaces 

USBS_MrgKntVecBspSurs     Author: Thomas Backmeister       25.6.2007

Merge the U- or V-knotvectors of an array of bspline surfaces.
The surfaces may have any parameter intervals.

IN:
  int nsr           ... number of bspline surfaces
  SurBSpl *surTab   ... array of bspline surfaces
  int dirUV         ... 0/1 <=> U/V-direction
  Memspc *workSeg   ... workspace
OUT:
  int *mk           ... number of knots in mkTab
  double **mkTab    ... U- or V-knotvector from merging
  Memspc *memSeg    ... data space of mkTab
Returncodes:
  0 = OK
 -1 = out of space
 -2 = out of workspace
*/

  int rc, *p, *m, i1, done;
  double umin, **kvTab;
  void *memStart, *rp;

  //printf("USBS_MrgKntVecBspSurs: Merge U/V-knotvectors <-- bspline surfaces\n");

  // init
  *mk = 0;
  *mkTab = memSeg->next;

  // save start of workspace
  memStart = workSeg->next;

  // workspace and init
  m = workSeg->next;  // number of knots of each knotvector
  rc = UME_add (workSeg, sizeof(int) * nsr);
  if (rc < 0) goto L_outOfWorkSpace;
  kvTab = workSeg->next;  // array of knotvectors
  rc = UME_add (workSeg, sizeof(double*) * nsr);
  if (rc < 0) goto L_outOfWorkSpace;
  if (dirUV == 0) { 
    for (i1=0; i1<nsr; ++i1) {
      m[i1] = surTab[i1].ptUNr + surTab[i1].degU + 1;
      kvTab[i1] = surTab[i1].kvTabU;
    }
  }    
  else {
    for (i1=0; i1<nsr; ++i1) {
      m[i1] = surTab[i1].ptVNr + surTab[i1].degV + 1;
      kvTab[i1] = surTab[i1].kvTabV;
    }
  }    
  p = workSeg->next;  // pointer for each knotvector
  rc = UME_add (workSeg, sizeof(int) * nsr);
  if (rc < 0) goto L_outOfWorkSpace;
  for (i1=0; i1<nsr; ++i1) p[i1] = 0;

  // -------------
  // compute mkTab
  // -------------
  while (1) {
    // check if done
    done = 1;
    for (i1=1; i1<nsr; ++i1) if (p[i1] < m[i1]) done = 0;
    if (done == 1) break;

    // minimal value of current knot positions
    umin = UT_VAL_MAX;
    for (i1=0; i1<nsr; ++i1) {
      if (p[i1] >= m[i1]) continue;
      umin = DMIN (umin, (kvTab[i1])[p[i1]]);
    }

    // new knot into mkTab
    rp = UME_save (memSeg, &umin, sizeof(double));
    if (rp == NULL) goto L_outOfSpace;
    ++(*mk);

    // move all pointers with value umin
    for (i1=0; i1<nsr; ++i1) {
      if (p[i1] >= m[i1]) continue;
      if (UTP_comp2db (umin, (kvTab[i1])[p[i1]], UT_TOL_min1))
        ++p[i1];
    }
  }
  // ----------------------------

  // release workspace
  workSeg->next = memStart;

  return 0;

L_outOfSpace:
  TX_Error("USBS_MrgKntVecBspSurs: out of space");
  return -1;

L_outOfWorkSpace:
  TX_Error("USBS_MrgKntVecBspSurs: out of workspace");
  return -2;
}


/*=======================================================================*/
  int USBS_MkeCmpBspSurs (SurBSpl *surTo, Memspc *memSeg, int srNr,
                          SurBSpl **surTi, int degU, int degV,
			  Memspc *workSeg) {   
/*======================
USBS_MkeCmpBspSurs     Compatible bspline surfaces <-- bspline surfaces 

USBS_MkeCmpBspSurs      Author: Thomas Backmeister       25.6.2007

Make compatible some bspline surfaces, i.e. equalize degree and
knotvector of the surfaces in U- and V-direction.
It is assumed that all surfaces are parameterized over the same
domain [a,b] x [c,d].

IN:
  int srNr          ... number of bspline surfaces (> 1)
  SurBSpl **surTi   ... bspline surfaces
  int degU          ... desired U-degree for all bspline surfaces
                        (only effectiv if > max{surface U-degrees})
  int degV          ... desired V-degree for all bspline surfaces
                        (only effectiv if > max{surface V-degrees})
  Memspc *workSeg   ... workspace
OUT:
  SurBSpl *surTo    ... compatible bspline surfaces (size >= srNr !)
  Memspc *memSeg    ... data space of surTo (and workspace)
Returncodes:
  0 = OK
 -1 = internal error
 -2 = input error
*/

  int     i1, rc, mdegU, mdegV, mkU, mkV, nk;
  double  *mkUTab, *mkVTab, *nkTab;
  void    *workStart, *workStart2, *memStart;
  SurBSpl surT1[TabS1], surT2[TabS1];


  // printf("USBS_MkeCmpBspSurs srNr=%d degU=%d degV=%d\n",srNr,degU,degV);


  if (TabS1 < srNr) goto L_InputErr;

  // save start of memspaces
  workStart = workSeg->next;
  memStart = memSeg->next;


  // maximum of U-degrees of all surfaces
  mdegU = surTi[0]->degU;
  if (degU > mdegU) mdegU = degU;
  for (i1=1; i1<srNr; ++i1)
    if (surTi[i1]->degU > mdegU) mdegU = surTi[i1]->degU;


  // maximum of V-degrees of all surfaces
  mdegV = surTi[0]->degV;
  if (degV > mdegV) mdegV = degV;
  for (i1=1; i1<srNr; ++i1)
    if (surTi[i1]->degV > mdegV) mdegV = surTi[i1]->degV;


  // U-degree elevation for all surfaces with degU < mdegU
    // printf(" U-degree elevation\n");
  for (i1=0; i1<srNr; ++i1) {
    if (surTi[i1]->degU == mdegU)
      //surT1[i1] = *(surTi[i1]);
      USBS_CpyBspSur (&(surT1[i1]), memSeg, surTi[i1]);
    else {
      rc = USBS_DegElevBspSur (&(surT1[i1]), memSeg, surTi[i1],
		               mdegU-surTi[i1]->degU, 0);
      if (rc < 0) return -1;
    }
  }


  // V-degree elevation for all surfaces with degV < mdegV
    // printf(" V-degree elevation\n");
  for (i1=0; i1<srNr; ++i1) {
    if (surT1[i1].degV == mdegV)
      //surT2[i1] = surT1[i1];
      USBS_CpyBspSur (&(surT2[i1]), workSeg, &(surT1[i1]));
    else {
      rc = USBS_DegElevBspSur (&(surT2[i1]), workSeg, &(surT1[i1]),
		               mdegV-surT1[i1].degV, 1);
      if (rc < 0) return -1;
    }
  }

  // release workspace
  memSeg->next = memStart;

  // merge all U-knotvectors --> mkUTab
    // printf(" merge U\n");
  rc = USBS_MrgKntVecBspSurs (&mkU, &mkUTab, workSeg, srNr, surT2, 0, memSeg);
  if (rc < 0) return -1;

  // merge all V-knotvectors --> mkVTab
    // printf(" merge V\n");
  rc = USBS_MrgKntVecBspSurs (&mkV, &mkVTab, workSeg, srNr, surT2, 1, memSeg);
  if (rc < 0) return -1;

  // save workspace 2
  workStart2 = workSeg->next;


    // printf(" - new kots: srNr=%d\n",srNr);
  for (i1=0; i1<srNr; ++i1) {
    // new U-knots
      // printf(" new U-knots %d\n",i1);
    rc = USBS_DifKntsKntVecBspSur (&nk, &nkTab, workSeg, mkU, mkUTab,
                                   &(surT2[i1]), 0);
      // printf(" after _DifKntsKntVecBspSur %d %d\n",rc,nk);
    if (rc < 0) return -1;
    if (nk == 0) {
      //surT1[i1] = surT2[i1];
      USBS_CpyBspSur (&(surT1[i1]), workSeg, &(surT2[i1]));
        // printf(" after _CpyBspSur %d\n",i1);
    } else {
      // refine U-knotvector to mkUTab
      rc = USBS_RefKntVecBspSur (&(surT1[i1]), workSeg, &(surT2[i1]),
                                 nk, nkTab, 0);
        // printf(" after _RefKntVecBspSur %d\n",rc);
      if (rc < 0) return -1;
    }

    // new V-knots
      // printf(" new V-knots %d\n",i1);
    rc = USBS_DifKntsKntVecBspSur (&nk, &nkTab, workSeg, mkV, mkVTab,
                                   &(surT1[i1]), 1);
      // printf(" after _DifKntsKntVecBspSur %d %d\n",rc,nk);
    if (rc < 0) return -1;
    if (nk == 0) {
      USBS_CpyBspSur (&(surTo[i1]), memSeg, &(surT1[i1]));
        // printf(" after _CpyBspSur %d\n",i1);
    } else {
      // refine V-knotvector to mkVTab
      rc = USBS_RefKntVecBspSur (&(surTo[i1]), memSeg, &(surT1[i1]),
                                 nk, nkTab, 1);
        // printf(" after _RefKntVecBspSur %d\n",rc);
      if (rc < 0) return -1;
    }
    
    // release workspace 2
    workSeg->next = workStart2;
  }

  // release workspace
  workSeg->next = workStart;

  return 0;


  L_InputErr:
    TX_Error("USBS_MkeCmpBspSurs: too many surfaces");
    return -2;
}


/*=======================================================================*/
  int USBS_RefKntVecBspSur (SurBSpl *suro, Memspc *memSeg, SurBSpl *suri,
		            int nk, double *nkTab, int dirUV) {
/*========================
USBS_RefKntVecBspSur      Refine knotvector <-- bspline surface 

USBS_RefKntVecBspSur     Author: Thomas Backmeister       25.6.2007

Refine the knotvector of a bspline surface.
The function works with any parameter interval [a,b] of the knotvectors
of the surface.
The new knots must be in (a,b).
A new knot may already occur in the surface's knotvector.

Controlpoints:
suri->cpTab[k+l*suri->ptUNr], 0 <= k < suri->ptUNr, 0 <= l < suri->ptVNr.
sequence in suri->cpTab: kl = 00,10,...,01,11,...

suro->cpTab[k+l*suro->ptUNr], 0 <= k < suro->ptUNr, 0 <= l < suro->ptVNr.
sequence in suro->cpTab: kl = 00,10,...,01,11,...

IN:
  SurBSpl *suri   ... bspline surface
  int nk          ... number of new knots (>0)
  double *nkTab   ... array of new knots
  int dirUV       ... 0/1 <=> U/V-direction
OUT:
  SurBSpl *suro   ... bspline surface with additional knots
  Memspc *memSeg  ... data space of suro
Returncodes:
  0 = OK
 -1 = out of space
*/

  int rc, m, a, b, i, j, k, l, ind;
  int i1, l1, l2;
  double alfa;


  // printf("USBS_RefKntVecBspSur: dirUV=%d\n",dirUV);


  // init suro
  if (dirUV == 0) {
    suro->ptUNr = suri->ptUNr + nk; 
    suro->ptVNr = suri->ptVNr; 
  }
  else {
    suro->ptUNr = suri->ptUNr; 
    suro->ptVNr = suri->ptVNr + nk; 
  }	

  suro->degU = suri->degU;
  suro->degV = suri->degV;
  suro->v0U = suri->v0U;
  suro->v1U = suri->v1U;
  suro->v0V = suri->v0V;
  suro->v1V = suri->v1V;

    // printf(" suro = %d %d %d %d %f %f %f %f\n",
           // suro->degU,suro->degV,
           // suro->ptUNr,suro->ptVNr,
           // suro->v0U,suro->v1U,suro->v0V,suro->v1V);


  // space for U/V-knotvectors of suro
  suro->kvTabU = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * (suro->ptUNr+suro->degU+1));
  if (rc < 0) goto L_outOfSpace;

  suro->kvTabV = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * (suro->ptVNr+suro->degV+1));
  if (rc < 0) goto L_outOfSpace;

  // space for controlpoints of suro
  suro->cpTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(Point) * suro->ptUNr * suro->ptVNr);
  if (rc < 0) goto L_outOfSpace;

  if (dirUV == 0) {  // refine knotvector U with nkTab
      // printf(" refKVBS L UV0\n");


    for (i=0; i < suri->ptVNr + suri->degV + 1; ++i)
      suro->kvTabV[i] = suri->kvTabV[i]; // copy V-knotvector

    m = suri->ptUNr + suri->degU;
    a = UBS_FdSpnKntVec (suri->kvTabU, suri->degU, suri->ptUNr, nkTab[0]);
    b = UBS_FdSpnKntVec (suri->kvTabU, suri->degU, suri->ptUNr, nkTab[nk-1]);
    b = b + 1;
      // printf(" refKVBS m=%d a=%d b=%d\n",m,a,b);


    for (i1=0; i1<suro->ptVNr; ++i1) {
        // printf(" refKVBS-1 i1=%d\n",i1);
      l1 = i1 * suri->ptUNr;	    
      l2 = i1 * suro->ptUNr;	    
      for (j=0; j<=a-suri->degU; j++) suro->cpTab[j+l2] = suri->cpTab[j+l1];
      for (j=b-1; j<suri->ptUNr; j++) suro->cpTab[j+nk+l2] = suri->cpTab[j+l1];
    }  

    for (j=0; j<=a; j++) suro->kvTabU[j] = suri->kvTabU[j];
    for (j=b+suri->degU; j<=m; j++) suro->kvTabU[j+nk] = suri->kvTabU[j];

    i = b + suri->degU - 1;  k = b + suri->degU + nk - 1;

    for (j=nk-1; j>=0; j--) {
        // printf(" refKVBS-2 j=%d\n",j);

      while (nkTab[j] <= suri->kvTabU[i] && i > a) {
        for (i1=0; i1<suro->ptVNr; ++i1) {      
          suro->cpTab[k-suri->degU-1+i1*suro->ptUNr] =
            suri->cpTab[i-suri->degU-1+i1*suri->ptUNr];
        }  
        suro->kvTabU[k] = suri->kvTabU[i];
        --k;  --i;
      }
      for (i1=0; i1<suro->ptVNr; ++i1) {
        l2 = i1 * suro->ptUNr;	      
        suro->cpTab[k-suri->degU-1+l2] = suro->cpTab[k-suri->degU+l2];
      }	
      for (l=1; l<=suri->degU; l++) {
        ind = k - suri->degU + l;
        alfa = suro->kvTabU[k+l] - nkTab[j];    
        if (UTP_comp_0 (alfa)) {
          for (i1=0; i1<suro->ptVNr; ++i1) {
            l2 = i1 * suro->ptUNr;		  
            suro->cpTab[ind-1+l2] = suro->cpTab[ind+l2];
          }	    

        } else {
          alfa = alfa / (suro->kvTabU[k+l] - suri->kvTabU[i-suri->degU+l]);
          for (i1=0; i1<suro->ptVNr; ++i1) {
              // printf(" refKVBS-3 i1=%d\n",i1);
            l2 = i1 * suro->ptUNr;		  
            UT3D_pt_LinComb2Pts (&(suro->cpTab[ind-1+l2]), alfa,
              &(suro->cpTab[ind-1+l2]), (1.0-alfa),
              &(suro->cpTab[ind+l2]));
          }  
        }
      }
      suro->kvTabU[k] = nkTab[j];
      --k;
    }


  //----------------------------------------------------------------
  } else {  // refine knotvector V with nkTab
    for (i=0; i<suri->ptUNr+suri->degU+1; ++i)
      suro->kvTabU[i] = suri->kvTabU[i]; // copy U-knotvector
    m = suri->ptVNr + suri->degV;
    a = UBS_FdSpnKntVec (suri->kvTabV, suri->degV, suri->ptVNr, nkTab[0]);
    b = UBS_FdSpnKntVec (suri->kvTabV, suri->degV, suri->ptVNr, nkTab[nk-1]);
    b = b + 1;

    for (i1=0; i1<suro->ptUNr; ++i1) {
      for (j=0; j<=a-suri->degV; j++)
        suro->cpTab[j*suro->ptUNr+i1] = suri->cpTab[j*suri->ptUNr+i1];
      for (j=b-1; j<suri->ptVNr; j++)
        suro->cpTab[(j+nk)*suro->ptUNr+i1] = suri->cpTab[j*suri->ptUNr+i1];
    }  

    for (j=0; j<=a; j++) suro->kvTabV[j] = suri->kvTabV[j];
    for (j=b+suri->degV; j<=m; j++) suro->kvTabV[j+nk] = suri->kvTabV[j];

    i = b + suri->degV - 1;  k = b + suri->degV + nk - 1;
    for (j=nk-1; j>=0; j--) {
      while (nkTab[j] <= suri->kvTabV[i] && i > a) {
        for (i1=0; i1<suro->ptUNr; ++i1) {      
          suro->cpTab[(k-suri->degV-1)*suro->ptUNr+i1] =
            suri->cpTab[(i-suri->degV-1)*suri->ptUNr+i1];
        }  
        suro->kvTabV[k] = suri->kvTabV[i];
        --k;  --i;
      }
      for (i1=0; i1<suro->ptUNr; ++i1) {
        suro->cpTab[(k-suri->degV-1)*suro->ptUNr+i1] =
          suro->cpTab[(k-suri->degV)*suro->ptUNr+i1];
      }	
      for (l=1; l<=suri->degV; l++) {
        ind = k - suri->degV + l;
        alfa = suro->kvTabV[k+l] - nkTab[j];    
        if (UTP_comp_0 (alfa)) {
          for (i1=0; i1<suro->ptUNr; ++i1) {
            suro->cpTab[(ind-1)*suro->ptUNr+i1] =
              suro->cpTab[ind*suro->ptUNr+i1];
          }

        } else {
          alfa = alfa / (suro->kvTabV[k+l] - suri->kvTabV[i-suri->degV+l]);
          for (i1=0; i1<suro->ptUNr; ++i1) {
            UT3D_pt_LinComb2Pts (&(suro->cpTab[(ind-1)*suro->ptUNr+i1]), alfa,
			        &(suro->cpTab[(ind-1)*suro->ptUNr+i1]), (1.0-alfa),
			        &(suro->cpTab[ind*suro->ptUNr+i1]));
          }  
        }
      }
      suro->kvTabV[k] = nkTab[j];
      --k;
    }
  }  

  return 0;

L_outOfSpace:
  TX_Error("USBS_RefKntVecBspSur: out of space");
  return -1;
}


/*=======================================================================*/
  int USBS_DegElevBspSur (SurBSpl *suro, Memspc *memSeg, SurBSpl *suri,
		          int dup, int dirUV) {
/*======================
USBS_DegElevBspSur      Degree elevate <-- bspline surface 

USBS_DegElevBspSur     Author: Thomas Backmeister       20.6.2007

Degree elevation of a bspline surface.
The function works with any parameter interval [a,b] of the knotvectors
of the surface.

Controlpoints:
suri->cpTab[k+l*suri->ptUNr], 0 <= k < suri->ptUNr, 0 <= l < suri->ptVNr.
sequence in suri->cpTab: kl = 00,10,...,01,11,...

suro->cpTab[k+l*suro->ptUNr], 0 <= k < suro->ptUNr, 0 <= l < suro->ptVNr.
sequence in suro->cpTab: kl = 00,10,...,01,11,...

Work-Arrays:
bpts[k+l*(degi+1)], 0 <= k < degi+1, 0 <= l < ptNr.
sequence in bpts: kl = 00,10,...,01,11,...

ebpts[k+l*(dego+1)], 0 <= k < dego+1, 0 <= l < ptNr.
sequence in ebpts: kl = 00,10,...,01,11,...

nextbpts[k+l*(degi-1)], 0 <= k < degi-1, 0 <= l < ptNr.
sequence in nextbpts: kl = 00,10,...,01,11,...

IN:
  SurBSpl *suri   ... bspline surface
  int dup (>= 1)  ... degree elevation deg --> deg + dup
  int dirUV       ... 0/1 <=> U/V-direction
OUT:
  SurBSpl *suro   ... degree elevated bspline surface
  Memspc *memSeg  ... data space of suro (and temporary workspace)
Returncodes:
  0 = OK
 -1 = out of space
*/

  int rc, i, j, k, m, s1, s, ph2, mpi, mh, kind, cind;
  int lbz, rbz, RBZ, r, a, b, mul, oldr, save, first, last;
  int tr, kj, degi, dego, ptNr, l1, l2, l3, i1, j1, m2;
  double ko, *alphas, inv, ua, ub, numer, alf, alfj;
  double *kvTabi, *kvTabo;
  void *memStart;
  Polynom_ bezalfs;
  Point *bpts, *ebpts, *nextbpts;

  //printf("USBS_DegElevBspSur: Degree elevate <-- bspline surface\n");

  if (dirUV == 0) {
    // suri: number of knots - 1	  
    m = suri->ptUNr + suri->degU;  // U
    m2 = suri->ptVNr + suri->degV; // V
    // suri: number of different interior U-knots
    s1 = 0;
    ko = suri->kvTabU[0];
    for (i=suri->degU+1; i<=suri->ptUNr-1; ++i) {
      if (suri->kvTabU[i] > ko) {
        ++s1;
        ko = suri->kvTabU[i];
      }
    }
    // suro: init
    suro->degU = suri->degU + dup;
    suro->ptUNr = suri->ptUNr + (s1 + 1) * dup; 
    suro->degV = suri->degV;
    suro->ptVNr = suri->ptVNr; 
  }
  else {
    // suri: number of knots - 1	  
    m = suri->ptVNr + suri->degV;  // V
    m2 = suri->ptUNr + suri->degU; // U
    // suri: number of different interior V-knots
    s1 = 0;
    ko = suri->kvTabV[0];
    for (i=suri->degV+1; i<=suri->ptVNr-1; ++i) {
      if (suri->kvTabV[i] > ko) {
        ++s1;
        ko = suri->kvTabV[i];
      }
    }
    // suro: init
    suro->degU = suri->degU;
    suro->ptUNr = suri->ptUNr; 
    suro->degV = suri->degV + dup;
    suro->ptVNr = suri->ptVNr + (s1 + 1) * dup; 
  }
  
  // suro: init
  suro->v0U = suri->v0U;
  suro->v1U = suri->v1U;
  suro->v0V = suri->v0V;
  suro->v1V = suri->v1V;

  // space for U/V-knotvectors of suro
  suro->kvTabU = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * (suro->ptUNr+suro->degU+1));
  if (rc < 0) goto L_outOfSpace;
  suro->kvTabV = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * (suro->ptVNr+suro->degV+1));
  if (rc < 0) goto L_outOfSpace;

  if (dirUV == 0) {
    // copy V-knotvector	  
    for (i1=0; i1<m2+1; ++i1) suro->kvTabV[i1] = suri->kvTabV[i1];
    // notations    
    degi = suri->degU;	  
    dego = suro->degU;
    ptNr = suro->ptVNr;
    kvTabi = suri->kvTabU;
    kvTabo = suro->kvTabU;
  }
  else {
    // copy U-knotvector	  
    for (i1=0; i1<m2+1; ++i1) suro->kvTabU[i1] = suri->kvTabU[i1];	  
    // notations    
    degi = suri->degV;	  
    dego = suro->degV;	  
    ptNr = suro->ptUNr;
    kvTabi = suri->kvTabV;
    kvTabo = suro->kvTabV;
  }	  

  // space for controlpoints of suro
  suro->cpTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(Point) * suro->ptUNr * suro->ptVNr);
  if (rc < 0) goto L_outOfSpace;

  // save start of workspace
  memStart = memSeg->next;

  // workspace
  bezalfs.dim1 = dego + 1;
  bezalfs.dim2 = degi + 1;
  bezalfs.pol = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * bezalfs.dim1 * bezalfs.dim2);
  if (rc < 0) goto L_outOfSpace;
  bpts = memSeg->next;
  rc = UME_add (memSeg, sizeof(Point) * ptNr * (degi+1));
  if (rc < 0) goto L_outOfSpace;
  ebpts = memSeg->next;
  rc = UME_add (memSeg, sizeof(Point) * ptNr * (dego+1));
  if (rc < 0) goto L_outOfSpace;
  nextbpts = memSeg->next;
  rc = UME_add (memSeg, sizeof(Point) * ptNr * (degi-1));
  if (rc < 0) goto L_outOfSpace;
  alphas = memSeg->next;
  rc = UME_add (memSeg, sizeof(double) * (degi-1));
  if (rc < 0) goto L_outOfSpace;
   
  // Bezier degree elevation coefficients
  ph2 = dego / 2.0;
  bezalfs.pol[0] = bezalfs.pol[dego*bezalfs.dim2+degi] = 1.0;
  for (i=1; i<=ph2; i++) {
    inv = 1.0 / UTM_db_bincoeff (dego, i);
    mpi = IMIN(degi, i);
    for (j=IMAX(0,i-dup); j<=mpi; j++)
      bezalfs.pol[i*bezalfs.dim2+j] = inv * UTM_db_bincoeff (degi, j) *
                                            UTM_db_bincoeff (dup, i-j);
  }
  for (i=ph2+1; i<=dego-1; i++) {
    mpi = IMIN(degi, i);
    for (j=IMAX(0,i-dup); j<=mpi; j++)
      bezalfs.pol[i*bezalfs.dim2+j] = 
      bezalfs.pol[(dego-i)*bezalfs.dim2+degi-j];
  }
  if (dirUV == 0) {
    ua = suri->kvTabU[0];
    for (i=0; i<=dego; i++) suro->kvTabU[i] = ua;
    for (j1=0; j1<suro->ptVNr; ++j1)
      suro->cpTab[j1*suro->ptUNr] = suri->cpTab[j1*suri->ptUNr];
  }
  else {
    ua = suri->kvTabV[0];
    for (i=0; i<=dego; i++) suro->kvTabV[i] = ua;
    for (i1=0; i1<suro->ptUNr; ++i1)
      suro->cpTab[i1] = suri->cpTab[i1];
  }	  
  kind = dego + 1;
  cind = 1;

  // init first Bezier strip 
  lbz = 1;  r = -1;
  if (dirUV == 0) {
    for (j1=0; j1<suro->ptVNr; ++j1) {
      for (i1=0; i1<=degi; i1++) 
        bpts[i1+j1*(degi+1)] = suri->cpTab[i1+j1*suri->ptUNr];
    }      
  }    
  else {
    for (i1=0; i1<suro->ptUNr; ++i1) {
      for (j1=0; j1<=degi; j1++)
        bpts[j1+i1*(degi+1)] = suri->cpTab[i1+j1*suri->ptUNr];
    }
  }    
  a = degi;  b = degi + 1; 

  // -------------------------------------
  // Big loop thru knotvector
  // -------------------------------------
  while (b < m) { 
    i = b;
    while (b < m && 
           UTP_comp2db (kvTabi[b], kvTabi[b+1], UT_TOL_min1)) ++b;
    mul = b - i + 1; 
    ub = kvTabi[b];
    oldr = r;  

    // insert knot ub r times
    r = degi - mul;
    if (oldr > 0) lbz = (oldr + 2) / 2;
    else lbz = 1;
    if (r > 0) rbz = dego - (r+1) / 2;
    else rbz = dego;
    
    if (r > 0) {
      // insert knot to get Bezier strip
      numer = ub - ua;
      for (k=degi; k>mul; k--) alphas[k-mul-1] = 
	                            numer / (kvTabi[a+k]-ua);
      for (j=1; j<=r; j++) {
        save = r - j;  s = mul + j;
        for (j1=0; j1<ptNr; ++j1) {
          l1 = j1 * (degi + 1);		
          l2 = j1 * (degi - 1);		
          for (k=degi; k>=s; k--) {
            alf = alphas[k-s];
	    UT3D_pt_LinComb2Pts (&bpts[k+l1], alf, &bpts[k+l1], (1.0-alf),
			      &bpts[k-1+l1]);
          }
          nextbpts[save+l2] = bpts[degi+l1];
	}  
      }	
    }

    // degree elevate Bezier
    if (r > 0) RBZ = rbz + 1;
    else RBZ = rbz;
    for (i=lbz; i<= RBZ; i++) {
      // only points lbz,...,RBZ are used below
      for (j1=0; j1<ptNr; ++j1) ebpts[i+j1*(dego+1)] = UT3D_PT_NUL;
      mpi = IMIN(degi, i);
      for (j1=0; j1<ptNr; ++j1) {
        l1 = j1 * (degi + 1);		
        l2 = j1 * (dego + 1);		
        for (j=IMAX(0,i-dup); j<=mpi; j++) {
          UT3D_pt_LinComb2Pts (&ebpts[i+l2], 1.0, &ebpts[i+l2],
	                    bezalfs.pol[i*bezalfs.dim2+j], &bpts[j+l1]);
        } 
      }
    }

    if (oldr > 1) {
      // must remove knot u = kvTabi[a] oldr times
      if (oldr > 2) {
        // alphas on the right do not change
	alfj = (ua - kvTabo[kind-1]) / (ub - kvTabo[kind-1]);
      }
      if (dirUV == 0) {  // U-direction
        first = kind - 2;  last = kind;
        for (tr=1; tr<oldr; tr++) { // knot removal loop
	  i = first;  j = last;
	  kj = j - kind + 1;
	  // loop and compute the new control points for one removal step
          while (j-i > tr) {
            if (i < cind) {
              alf = (ua - kvTabo[i]) / (ub - kvTabo[i]);
              for (j1=0; j1<ptNr; ++j1) {
                l1 = j1 * suro->ptUNr;		
                UT3D_pt_LinComb2Pts (&(suro->cpTab[i+l1]), (1.0/alf),
			          &(suro->cpTab[i+l1]), ((alf-1.0)/alf),
			          &(suro->cpTab[i-1+l1]));
	      }	
            }
            if (kj >= lbz) {
              for (j1=0; j1<ptNr; ++j1) {
                l2 = j1 * (dego + 1);		
	        UT3D_pt_LinComb2Pts (&(ebpts[kj+l2]), 1.0, &(ebpts[kj+l2]),
               	                  (alfj/(alfj-1.0)), &(ebpts[kj+1+l2]));
	      }	
            }
            ++i;  --j;  --kj;
    	  }	
          --first;  ++last;
        } // end of removing knot u = kvTabi[a]
      }
      else {  // V-direction
        first = kind - 2;  last = kind;
        for (tr=1; tr<oldr; tr++) { // knot removal loop
	  i = first;  j = last;
	  kj = j - kind + 1;
	  // loop and compute the new control points for one removal step
          while (j-i > tr) {
            if (i < cind) {
              alf = (ua - kvTabo[i]) / (ub - kvTabo[i]);
              for (i1=0; i1<ptNr; ++i1) {
                UT3D_pt_LinComb2Pts (&(suro->cpTab[i1+i*ptNr]), (1.0/alf),
			          &(suro->cpTab[i1+i*ptNr]), ((alf-1.0)/alf),
			          &(suro->cpTab[i1+(i-1)*ptNr]));
	      }	
            }
            if (kj >= lbz) {
              for (j1=0; j1<ptNr; ++j1) {
                l2 = j1 * (dego + 1);		
	        UT3D_pt_LinComb2Pts (&(ebpts[kj+l2]), 1.0, &(ebpts[kj+l2]),
               	                  (alfj/(alfj-1.0)), &(ebpts[kj+1+l2]));
	      }	
            }
            ++i;  --j;  --kj;
    	  }	
          --first;  ++last;
        } // end of removing knot u = kvTabi[a]
      }	      
    }

    if (a != degi) {
      // load the knot ua
      for (i=0; i<dego-oldr; i++) {
        kvTabo[kind] = ua;
	++kind;
      }
    }

    // load controlpoints into suro->cpTab
    if (dirUV == 0) {
      for (j=lbz; j<=rbz; j++) {
        for (j1=0; j1<ptNr; ++j1)
          suro->cpTab[cind+j1*suro->ptUNr] = ebpts[j+j1*(dego+1)];
        ++cind;
      }	    
    }
    else {
      for (j=lbz; j<=rbz; j++) {
        for (i1=0; i1<ptNr; ++i1)
          suro->cpTab[i1+cind*ptNr] = ebpts[j+i1*(dego+1)];
        ++cind;
      }	    
    }	    

    if (b < m) {
      // set up for next pass thru loop
      lbz = 1;
      if (dirUV == 0) {
        for (j1=0; j1<ptNr; ++j1) {
          l1 = j1 * (degi + 1);		
          l2 = j1 * (degi - 1);		
	  l3 = j1 * suri->ptUNr;
          for (j=0; j<r; j++) bpts[j+l1] = nextbpts[j+l2];
          for (j=r; j<=degi; j++) bpts[j+l1] = suri->cpTab[b-degi+j+l3];
	}  
      }
      else {
        for (j1=0; j1<ptNr; ++j1) {
          l1 = j1 * (degi + 1);		
          l2 = j1 * (degi - 1);		
          for (j=0; j<r; j++) bpts[j+l1] = nextbpts[j+l2];
          for (j=r; j<=degi; j++)
            bpts[j+l1] = suri->cpTab[j1+(b-degi+j)*suri->ptUNr];
	}  
      }	      
      a = b;  ++b;  ua = ub;
    }
    else {
      // end knot
      for (i=0; i<=dego; i++) kvTabo[kind+i] = ub;
    }
  }
  // -------------------------------------
  // End of big loop
  // -------------------------------------

  // release workspace
  memSeg->next = memStart;

  return 0;

L_outOfSpace:
  TX_Error("USBS_DegElevBspSur: out of space");
  return -1;
}


/*=======================================================================*/
  int USBS_IntpolBspSur (SurBSpl *sur, Memspc *memSeg, int npu, int npv,
		         Point *pTab, int degU, int degV, double *pvTU,
			 double *pvTV, int usep, Memspc *workSeg) {
/*=====================
USBS_IntpolBspSur     Interpolating bspline surface <-- points

USBS_IntpolBspSur      Author: Thomas Backmeister       18.6.2007

Tensor product bspline surface interpolating a rectangular arranged
set of points.

Zu interpolierende Punkte: pTab[i+j*npu], 0 <= i < npu, 0 <= j < npv.

IN:
  int npu           ... number of points in U-direction
  int npv           ... number of points in V-direction
  Point *pTab       ... array of npu x npv points
  int degU          ... degree in U-direction (< min{20,npu})
  int degV          ... degree in V-direction (< min{20,npv})
  double *pvTU      ... parameter vector in U-direction (npu)
  double *pvTV      ... parameter vector in V-direction (npv)
  int usep          ... 1/0 <=> use pvTU and pvTV Y/N
  Memspc *workSeg   ... workspace
OUT:
  SurBSpl *sur      ... interpolating bspline surface  
  Memspc *memSeg    ... data space of sur
Returncodes:
  0 = OK
 -1 = out of space
 -2 = out of workspace
 -3 = input error
 -4 = internal error
*/

  int rc, i1, j1, dirUV;
  double *pvTab;
  void *workStart1, *workStart2;
  Point *qTab, *phTab;

  //printf("\nUSBS_IntpolBspSur: Interpolating bspline surface <-- points\n"); 

  // check input
  if (degU < 1 || degU >= IMIN(20,npu)) goto L_InputError;
  if (degV < 1 || degV >= IMIN(20,npv)) goto L_InputError;

  // init surface
  sur->ptUNr = npu;
  sur->ptVNr = npv;
  sur->degU = degU;
  sur->degV = degV;

  // space for controlpoints of surface
  sur->cpTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(Point) * sur->ptUNr * sur->ptVNr);
  if (rc < 0) goto L_outOfSpace;

  // start 1 of workspace
  workStart1 = workSeg->next;

  // workspace for npu x npv points
  qTab = workSeg->next;
  rc = UME_add (workSeg, sizeof(Point) * npu * npv);
  if (rc < 0) goto L_outOfWorkSpace;

  // start 2 of workspace
  workStart2 = workSeg->next;

  // parametervector in U-direction
  if (usep == 1) pvTab = pvTU;
  else {
    dirUV = 0;
    rc = UBS_ParVecUVPts (&pvTab, workSeg, npu, npv, pTab, dirUV);
    if (rc < 0) return -4;
  }

  // U-knotvector of surface
  rc = UBS_KntVecParVec (&(sur->kvTabU), memSeg, degU, npu, pvTab);
  if (rc < 0) return -4;

  // continue init surface
  sur->v0U = sur->kvTabU[0];
  sur->v1U = sur->kvTabU[sur->ptUNr+sur->degU];

  // npv interpolations of npu points 
  // --------------------------------
  for (j1=0; j1<npv; ++j1) {
    rc = UCBS_CtrlPtsIntpol (&(qTab[j1*npu]), npu, &(pTab[j1*npu]),
		             degU, sur->kvTabU, pvTab, workSeg);
    if (rc < 0) return -4;
  }

  // start 2 <-- release workspace
  workSeg->next = workStart2;

  // parametervector in V-direction
  if (usep == 1) pvTab = pvTV;
  else {
    dirUV = 1;
    rc = UBS_ParVecUVPts (&pvTab, workSeg, npu, npv, pTab, dirUV);
    if (rc < 0) return -4;
  }

  // V-knotvector of surface
  rc = UBS_KntVecParVec (&(sur->kvTabV), memSeg, degV, npv, pvTab);
  if (rc < 0) return -4;

  // continue init surface
  sur->v0V = sur->kvTabV[0];
  sur->v1V = sur->kvTabV[sur->ptVNr+sur->degV];

  // workspace for npv points
  phTab = workSeg->next;
  rc = UME_add (workSeg, sizeof(Point) * npv);
  if (rc < 0) goto L_outOfWorkSpace;

  // npu interpolations of npv points
  // -------------------------------- 
  for (i1=0; i1<npu; ++i1) {
    for (j1=0; j1<npv; ++j1) phTab[j1] = qTab[i1+j1*npu];
    rc = UCBS_CtrlPtsIntpol (phTab, npv, phTab, degV, sur->kvTabV,
		             pvTab, workSeg);
    if (rc < 0) return -4;
    for (j1=0; j1<npv; ++j1) sur->cpTab[i1+j1*npu] = phTab[j1];
  }

  // release workspace
  workSeg->next = workStart1;

  return 0;
 
L_outOfSpace:
  TX_Error("USBS_IntpolBspSur: out of space");
  return -1;
 
L_outOfWorkSpace:
  TX_Error("USBS_IntpolBspSur: out of workspace");
  return -2;

L_InputError:
  TX_Error("USBS_IntpolBspSur: input error - invalid degree!");
  return -3;
}


/*=======================================================================*/
  int USBS_SkinSurBspCrvs (SurBSpl *sks, Memspc *memSeg, int ncv,
		           CurvBSpl *bspTab, int deg, int dirUV, double *parT,
			   int usep, Memspc *workSeg) {
/*=======================
USBS_SkinSurBspCrvs     Skinned bspline surface <-- bspline curves

USBS_SkinSurBspCrvs      Author: Thomas Backmeister       9.5.2007

Skinned bspline surface interpolating a set of cross sectional
bspline curves.
The curves must be compatible, i.e. parameter interval, degree and
knotvector must be equal.
The curves must be clamped, i.e. their knotvectors must look like
[a,...,a,u1,...,uk,b,...,b], where a and b occur degree times.

IN:
  int ncv           ... number of bspline curves
  CurvBSpl *bspTab  ... array of bspline curves
  int deg           ... surface degree across the curves
                        (< min{20,ncv} !)
  int dirUV         ... 1/0 <=> U-direction across/along curves
  double *parT      ... parameter vector across bspline curves
  int usep          ... 1/0 <=> use parT Y/N
  Memspc *workSeg   ... workspace
OUT:
  SurBSpl *sks      ... skinned bspline surface  
  Memspc *memSeg    ... data space for sks
Returncodes:
  0 = OK
 -1 = out of space
 -2 = out of workspace
 -3 = input error
 -4 = internal error
*/

  int rc, m, i1, i2;
  double *pvTab;
  Point *pTab, *cpTab;
  void *memStart;


  // printf("------------------------------------------------------ \n");
  // printf("USBS_SkinSurBspCrvs dir=%d ncv=%d sep=%d\n",dirUV,ncv,usep); 
  // for(i1=0; i1<ncv; ++i1) {
    // DEB_dump_obj__ (Typ_CVBSP, &bspTab[i1], "  bspTab[%d]",i1);
  // }


  // check input
  if (deg < 1 || deg >= IMIN(20,ncv)) goto L_InputError;

  // init surface
  if (dirUV == 0) {
    sks->ptUNr = bspTab[0].ptNr;
    sks->ptVNr = ncv;
    sks->degU = bspTab[0].deg;
    sks->degV = deg;
    sks->v0U = bspTab[0].v0;
    sks->v1U = bspTab[0].v1;
  }
  else {
    sks->ptUNr = ncv;
    sks->ptVNr = bspTab[0].ptNr;
    sks->degU = deg;
    sks->degV = bspTab[0].deg;
    sks->v0V = bspTab[0].v0;
    sks->v1V = bspTab[0].v1;
  }	  

  // U- resp. V-knotvector of surface
  m = bspTab[0].ptNr + bspTab[0].deg + 1;
  if (dirUV == 0) {
    sks->kvTabU = memSeg->next;
    rc = UME_add (memSeg, sizeof(double) * m);
    if (rc < 0) goto L_outOfSpace;
    for (i1=0; i1<m; ++i1) sks->kvTabU[i1] = bspTab[0].kvTab[i1];
  }
  else {
    sks->kvTabV = memSeg->next;
    rc = UME_add (memSeg, sizeof(double) * m);
    if (rc < 0) goto L_outOfSpace;
    for (i1=0; i1<m; ++i1) sks->kvTabV[i1] = bspTab[0].kvTab[i1];
  }	  

  // save start of workspace
  memStart = workSeg->next;


  // parameter vector across bspline curves
  if (usep == 1) pvTab = parT;
  else {
    rc = UCBS_CrssParVecBspCrvs (&pvTab, workSeg, ncv, bspTab);
    if (rc < 0) return -4;
  }
    // TEST ONLY
    // for(i1=0; i1<ncv; ++i1) printf("pvTab[%d]= %f\n", i1, pvTab[i1]);



  // V- resp. U-knotvector of surface
  if (dirUV == 0) {
    rc = UBS_KntVecParVec (&(sks->kvTabV), memSeg, deg, ncv, pvTab);
    if (rc < 0) return -4;
      // TEST ONLY
      // i2 = ncv + deg + 1;
      // for(i1=0; i1<i2; ++i1) printf("kvTabV[%d]= %f\n", i1, sks->kvTabV[i1]);


  } else {
    rc = UBS_KntVecParVec (&(sks->kvTabU), memSeg, deg, ncv, pvTab);
    if (rc < 0) return -4;
      // TEST ONLY
      // i2 = ncv + deg + 1;
      // for(i1=0; i1<i2; ++i1) printf("kvTabU[%d]= %f\n", i1, sks->kvTabU[i1]);
  }



  // continue init surface
  if (dirUV == 0) {
    sks->v0V = sks->kvTabV[0];
    sks->v1V = sks->kvTabV[ncv+deg];
  }
  else {
    sks->v0U = sks->kvTabU[0];
    sks->v1U = sks->kvTabU[ncv+deg];
  }	  


  // space for controlpoints of surface
  sks->cpTab = memSeg->next;
  rc = UME_add (memSeg, sizeof(Point) * sks->ptUNr * sks->ptVNr);
  if (rc < 0) goto L_outOfSpace;

  // workspace for ncv controlpoints to be interpolated
  pTab = workSeg->next;
  rc = UME_add (workSeg, sizeof(Point) * ncv);
  if (rc < 0) goto L_outOfWorkSpace;

  // workspace for 1 row of controlpoints of skinned surface
  cpTab = workSeg->next;
  rc = UME_add (workSeg, sizeof(Point) * ncv);
  if (rc < 0) goto L_outOfWorkSpace;


  // controlpoints of skinned surface
  // --------------------------------
  if (dirUV == 0) {
    for (i1=0; i1<bspTab[0].ptNr; ++i1) {
      for (i2=0; i2<ncv; ++i2) pTab[i2] = bspTab[i2].cpTab[i1];

      // V-row i1 <-- interpolate ncv controlpoints across bspline curves
      rc = UCBS_CtrlPtsIntpol (cpTab, ncv, pTab, deg, sks->kvTabV, pvTab,
		               workSeg);
      if (rc < 0) return -4;

      // copy V-row i1 of controlpoints of surface
      for (i2=0; i2<ncv; ++i2) sks->cpTab[i1+i2*sks->ptUNr] = cpTab[i2];
    }
  }
  else {
    for (i1=0; i1<bspTab[0].ptNr; ++i1) {
      for (i2=0; i2<ncv; ++i2) pTab[i2] = bspTab[i2].cpTab[i1];

      // U-row i1 <-- interpolate ncv controlpoints across bspline curves
      rc = UCBS_CtrlPtsIntpol (cpTab, ncv, pTab, deg, sks->kvTabU, pvTab,
		               workSeg);
      if (rc < 0) return -4;

      // copy U-row i1 of controlpoints of surface
      for (i2=0; i2<ncv; ++i2) sks->cpTab[i1*sks->ptUNr+i2] = cpTab[i2];
    }      
  }	  
  // --------------------------------


  // release workspace
  workSeg->next = memStart;

  return 0;
 
L_outOfSpace:
  TX_Error("USBS_SkinSurBspCrvs: out of space");
  return -1;
 
L_outOfWorkSpace:
  TX_Error("USBS_SkinSurBspCrvs: out of workspace");
  return -2;

L_InputError:
  TX_Error("USBS_SkinSurBspCrvs: input error - invalid degree!");
  return -3;
}


/*=======================================================================*/
  int USBS_DirIndSdBspSur (double **kvTabS1, double **kvTabN1, long *ptNNr1,
                           Point **cpTab1, Memspc *memSeg1,
                           double **kvTabS2, double **kvTabN2, long *ptNNr2,
                           Point **cpTab2, Memspc *memSeg2,
                           int deg, double *kvTabS, double *kvTabN, int ptNrS,
                           int ptNrN, Point *cpTab, int m, double t, int uv) {
/*=======================
USBS_DirIndSdBspSur    Dir. indep. subdiv. <-- bspline surface 

USBS_DirIndSdBspSur     Author: Thomas Backmeister       25.10.2003

Direction-independent subdivision of a bspline surface into
2 bspline surfaces.

IN:
  int deg           ... surface-degree for division
  double *kvTabS    ... undivided knotvector
  double *kvTabN    ... knotvector for division
  int ptNrS         ... undivided number of controlpoints
  int ptNrN         ... number of controlpoints for division
  Point *cpTab      ... controlpoints of surface
  int m             ... number of undivided knots - 1
  double t          ... division parameter corresponding to kvTabN
  int uv            ... 1: t=u-value; 2: t=v-value
OUT:
  double **kvTabS1  ... undivided knotvector of surface 1
  double **kvTabN1  ... divided knotvector of surface 1
  long *ptNNr1      ... divided number of controlpoints of surface 1
  Point **cpTab1    ... controlpoints of surface 1
  Memspc *memSeg1   ... data space of knots and controlpoints of
                        surface 1 (and temporary work space)
  double **kvTabS2  ... undivided knotvector of surface 2
  double **kvTabN2  ... divided knotvector of surface 2
  long *ptNNr2      ... divided number of controlpoints of surface 2
  Point **cpTab2    ... controlpoints of surface 2
  Memspc *memSeg2   ... data space of knots and controlpoints of
                        surface 2
Returncodes:
  0 = OK
 -1 = internal error
 -2 = out of space
*/

  int rc, i1, q, r, ip, ip1, ip2;

  //printf("USBS_DirIndSdBspSur: Dir. indep. subdiv. <-- bspline surface\n");

  // an abbreviation
  q = ptNrN + deg;  // knotvector for division =: (t0, ..., tq)

  // undivided knotvector of surface 1
  *kvTabS1 = memSeg1->next;
  rc = UME_add (memSeg1, sizeof(double) * (m+1));
  if (rc < 0) goto L_outOfSpace;
  for (i1=0; i1<=m; ++i1) (*kvTabS1)[i1] = kvTabS[i1];

  // undivided knotvector of surface 2
  *kvTabS2 = memSeg2->next;
  rc = UME_add (memSeg2, sizeof(double) * (m+1));
  if (rc < 0) goto L_outOfSpace;
  for (i1=0; i1<=m; ++i1) (*kvTabS2)[i1] = kvTabS[i1];

  // divide knotvector
  rc = UBS_DivKnotVec (kvTabN1, memSeg1, kvTabN2, memSeg2, &r,
                       kvTabN, q, deg, t);
  if (rc < 0) return -1;

  // divided number of controlpoints of surface 1 and 2
  *ptNNr1 = r;
  if (t > kvTabN[r]) ++(*ptNNr1);
  *ptNNr2 = q - r;

  // space for controlpoints of surface 1
  *cpTab1 = memSeg1->next;
  rc = UME_add (memSeg1, sizeof(Point) * (*ptNNr1) * ptNrS);
  if (rc < 0) goto L_outOfSpace;

  // space for controlpoints of surface 2
  *cpTab2 = memSeg2->next;
  rc = UME_add (memSeg2, sizeof(Point) * (*ptNNr2) * ptNrS);
  if (rc < 0) goto L_outOfSpace;

  // divide controlpoints
  if (uv == 1) {
    ip = ip1 = ip2 = 0;
    for (i1=0; i1<ptNrS; ++i1) {
      rc =  UBS_DivCtrlPtsBsp (&((*cpTab1)[ip1]), &((*cpTab2)[ip2]), deg,
                               kvTabN, ptNrN, &(cpTab[ip]), t, r, 0, 1,
                               memSeg1);
      if (rc < 0) return -1;
      ip += ptNrN;
      ip1 += (*ptNNr1);
      ip2 += (*ptNNr2);
    }
  } else {
    for (i1=0; i1<ptNrS; ++i1) {
      rc = UBS_DivCtrlPtsBsp (&((*cpTab1)[0]), &((*cpTab2)[0]), deg,
                              kvTabN, ptNrN, &(cpTab[0]), t, r, i1,
                              ptNrS, memSeg1);
      if (rc < 0) return -1;
    }
  }

  return 0;
 
L_outOfSpace:
  TX_Error("USBS_DirIndSdBspSur: out of space");
  return -2;
}



/*=======================================================================*/
  int USBS_SubDivBspSur (SurBSpl *sur1, Memspc *memSeg1, SurBSpl *sur2,
                         Memspc *memSeg2, SurBSpl *sur, double t, int uv) {
/*=====================
USBS_SubDivBspSur    Subdivision <-- bspline surface 

USBS_SubDivBspSur     Author: Thomas Backmeister       24.10.2003

Subdivide a bspline surface into 2 bspline surfaces.

IN:
  SurBSpl *sur    ... bspline surface
  double t        ... subdivision parameter
  int uv          ... 1: t=u-value; 2: t=v-value
OUT:
  SurBSpl *sur1   ... bspline surface 1
  Memspc *memSeg1 ... data space of sur1 (and temporary workspace)
  SurBSpl *sur2   ... bspline surface 2
  Memspc *memSeg2 ... data space of sur2
Returncodes:
  0 = OK
 -1 = internal error
*/

  int rc, m1, m2;

  //printf("USBS_SubDivBspSur: Subdivision <-- bspline surface\n");

  // some abbreviations
  m1 = sur->ptUNr + sur->degU;  // knot vector =: (U0, ..., Um1)
  m2 = sur->ptVNr + sur->degV;  // knot vector =: (V0, ..., Vm2)

  // init sur1 and sur2
  sur1->degU = sur->degU;
  sur1->degV = sur->degV;
  sur2->degU = sur->degU;
  sur2->degV = sur->degV;

  if (uv == 1) {

    // number of V-controlpoints of sur1 and sur2
    sur1->ptVNr = sur->ptVNr;
    sur2->ptVNr = sur->ptVNr;

    // U-subdivide surface
    rc = USBS_DirIndSdBspSur (&(sur1->kvTabV), &(sur1->kvTabU), &(sur1->ptUNr),
                              &(sur1->cpTab), memSeg1,
                              &(sur2->kvTabV), &(sur2->kvTabU), &(sur2->ptUNr),
                              &(sur2->cpTab), memSeg2,
                              sur->degU, sur->kvTabV, sur->kvTabU, sur->ptVNr,
                              sur->ptUNr, sur->cpTab, m2, t, uv);
    if (rc < 0) return -1;
  }
  else {

    // number of U-controlpoints of sur1 and sur2
    sur1->ptUNr = sur->ptUNr;
    sur2->ptUNr = sur->ptUNr;

    // V-subdivide surface
    rc = USBS_DirIndSdBspSur (&(sur1->kvTabU), &(sur1->kvTabV), &(sur1->ptVNr),
                              &(sur1->cpTab), memSeg1,
                              &(sur2->kvTabU), &(sur2->kvTabV), &(sur2->ptVNr),
                              &(sur2->cpTab), memSeg2,
                              sur->degV, sur->kvTabU, sur->kvTabV, sur->ptUNr,
                              sur->ptVNr, sur->cpTab, m1, t, uv);
    if (rc < 0) return -1;
  }

  return 0;
}



/*=======================================================================*/
  int USBS_4QuBspSur (SurBSpl *surT[4], Memspc *memsurT[4], SurBSpl *sur,
                      double u, double v, Memspc *workSeg) {
/*==================
USBS_4QuBspSur    4 quarters <-- bspline surface 

USBS_4QuBspSur     Author: Thomas Backmeister       30.10.2003

Subdivide a bspline surface into 4 quarter bspline surfaces.

IN:
  SurBSpl *sur       ... bspline surface
  double u           ... subdivision U-parameter
  double v           ... subdivision V-parameter
  Memspc *workSeg    ... temporary workspace
  Memspc *memsurT[4] ... memsurT[0], memsurT[2]: temporary workspace
OUT:
  SurBSpl *surT[4]   ... bspline surfaces 1-4
  Memspc *memsurT[4] ... data space of surfaces 1-4
Returncodes:
  0 = OK
 -1 = internal error
*/

  int rc;
  void *startsur3, *startwork;
  SurBSpl surA, surB;

  //printf("USBS_4QuBspSur: 4 quarters <-- bspline surface\n");

  // save memory start of surA and surB
  startsur3 = memsurT[2]->next;
  startwork = workSeg->next;

  // U-division of sur
  rc = USBS_SubDivBspSur (&surA, memsurT[2], &surB, workSeg, sur, u, 1);
  if (rc < 0) return -1;

  // V-division of surA
  rc = USBS_SubDivBspSur (surT[0], memsurT[0], surT[1], memsurT[1],
                          &surA, v, 2);
  if (rc < 0) return -1;

  // release space of surA
  memsurT[2]->next = startsur3;

  // V-division of surB
  rc = USBS_SubDivBspSur (surT[2], memsurT[2], surT[3], memsurT[3],
                          &surB, v, 2);
  if (rc < 0) return -1;

  // release space of surB
  workSeg->next = startwork;

  return 0;
}



/*=======================================================================*/
  int USBS_UVPtBspSur (double *pu, double *pv, double *useg, double *vseg,
                       SurBSpl *sur, void *memStart, int mi, Point *pt,
                       double tol, Memspc *memSegT[4], Memspc *workSeg) {
/*===================
USBS_UVPtBspSur    UV-parameter <-- point on bspline surface 

USBS_UVPtBspSur     Author: Thomas Backmeister       28.10.2003

Computation of u- and v-parameter of a point on a bspline surface.
The function works recursively!
useg and vseg must be initialized with high values!
1. call of recursion: set memStart = NULL and mi = -1!

IN:
  SurBSpl *sur       ... bspline surface
  void *memStart     ... start of surface space (or NULL)
  int mi             ... = 0,1,2,3: surface space = memSegT[mi];
                         = -1: <=> memStart = NULL
  Point *pt          ... point on the surface
  double tol         ... tolerance for u-,v- segment length
  Memspc *memSegT[4] ... space for subsurfaces 1-4 (and temp. workspace)
  Memspc *workSeg    ... temporary workspace
OUT:
  double *pu         ... u-parameter of point
  double *pv         ... v-parameter of point
  double *useg       ... length of u-segment of point
  double *vseg       ... length of v-segment of point
Returncodes:
  0 = OK; point parameter NOT FOUND (point outside sur-controlpoints)
  1 = OK; point parameter outside tol but APPROXIMATED
  2 = OK; point parameter within tol FOUND
 -1 = internal error
*/

  int rc, i1;
  double du, dv, u, v;
  void *memStartT[4];
  Point pmin, pmax;
  SurBSpl surT[4], *psurT[4];

  //printf("USBS_UVPtBspSur: UV-parameter <-- point on bspline surface\n");

  // pointer of surfaces
  for (i1=0; i1<4; ++i1) psurT[i1] = &surT[i1];

  // some abbreviations
  u = (sur->kvTabU[sur->ptUNr] + sur->kvTabU[sur->degU]) / 2.0;
  v = (sur->kvTabV[sur->ptVNr] + sur->kvTabV[sur->degV]) / 2.0;
  du = fabs (sur->kvTabU[sur->ptUNr] - sur->kvTabU[sur->degU]);
  dv = fabs (sur->kvTabV[sur->ptVNr] - sur->kvTabV[sur->degV]);

  // bounding box of surface controlpoints
  rc = UT3D_box_pts (&pmin, &pmax, sur->ptUNr*sur->ptVNr, sur->cpTab);
  if (rc < 0) return -1;

  // point outside bounding box --> return
  if (pt->x > (pmax.x + UT_TOL_min1) ||
      pt->y > (pmax.y + UT_TOL_min1) ||
      pt->z > (pmax.z + UT_TOL_min1) ||
      pt->x < (pmin.x - UT_TOL_min1) ||
      pt->y < (pmin.y - UT_TOL_min1) ||
      pt->z < (pmin.z - UT_TOL_min1)) {

    if (memStart != NULL && mi >= 0) {
      // release space of sur
      memSegT[mi]->next = memStart;
      return 1;
    }
    else return 0;
  }

  if (du < *useg && dv < *vseg) {
    // save smaller parameter-segment containing the point
    *pu = u;
    *pv = v;
    *useg = du;
    *vseg = dv;
  }
       
  if (du > tol || dv > tol) {

    // save start of work spaces
    for (i1=0; i1<4; ++i1) memStartT[i1] = memSegT[i1]->next;

    // subdivide surface into 4 quarter surfaces
    rc = USBS_4QuBspSur (psurT, memSegT, sur, u, v, workSeg);
    if (rc < 0) return -1;

    // repeat this function with all 4 subsurfaces
    rc = USBS_UVPtBspSur (pu, pv, useg, vseg, psurT[0], memStartT[0], 0, pt,
                          tol, memSegT, workSeg);
    if (rc < 0 || rc == 2) return rc;
    rc = USBS_UVPtBspSur (pu, pv, useg, vseg, psurT[1], memStartT[1], 1, pt,
                          tol, memSegT, workSeg);
    if (rc < 0 || rc == 2) return rc;
    rc = USBS_UVPtBspSur (pu, pv, useg, vseg, psurT[2], memStartT[2], 2, pt,
                          tol, memSegT, workSeg);
    if (rc < 0 || rc == 2) return rc;
    rc = USBS_UVPtBspSur (pu, pv, useg, vseg, psurT[3], memStartT[3], 3, pt,
                          tol, memSegT, workSeg);
    if (rc < 0 || rc == 2) return rc;
  }
  else {
    // u-,v- parameter found
    return 2;
  }

  return 1;
}


/*=======================================================================*/
  int USBS_NvPtBspSur (Vector *nv, SurBSpl *sur, Point *pt, 
		       Memspc *memSegT[4], Memspc *workSeg) {
/*===================
USBS_NvPtBspSur    Normal vector <-- point on bspline surface

USBS_NvPtBspSur     Author: Thomas Backmeister       11.11.2003

Computation of the normal-vector of a point on a bspline surface.
The direction of the normal-vector corresponds to axis UxV of the
parameterization-plane (U,V).

IN:
  SurBSpl *sur       ... bspline surface
  Point *pt          ... point on the surface
  Memspc *memSegT[4] ... temporary workspace (for subdivision of sur)
  Memspc *workSeg    ... temporary workspace
OUT:
  Vector *nv         ... normal-vector in pt (length = 1.0)
Returncodes:
  0 = OK
 -1 = internal error
*/

  int rc, i1;
  double pu, pv, useg, vseg;
  void *memStartT[4], *memStart;
  Vector tgU, tgV;

  //printf("USBS_NvPtBspSur: Normal vector <-- point on bspline surface\n");

  // save start of workspaces
  for (i1=0; i1<4; ++i1) memStartT[i1] = memSegT[i1]->next;

  // u-,v- parameter of point
  useg = UT_VAL_MAX;
  vseg = UT_VAL_MAX;
  pu = -UT_VAL_MAX;
  pv = -UT_VAL_MAX;
  rc = USBS_UVPtBspSur (&pu, &pv, &useg, &vseg, sur, NULL,
                        -1, pt, UT_TOL_min1, memSegT, workSeg);
  if (rc <= 0) return -1;

  // release workspaces
  for (i1=0; i1<4; ++i1) memSegT[i1]->next = memStartT[i1];

  // tangent of U-isocurve at pv
  rc = USBS_TgVecIsoBspSur (&tgU, sur, pu, pv, 1, workSeg);
  if (rc < 0) return -1;

  // tangent of V-isocurve at pu
  rc = USBS_TgVecIsoBspSur (&tgV, sur, pv, pu, 2, workSeg);
  if (rc < 0) return -1;

  // normal vector
  UT3D_vc_perp2vc (nv, &tgV, &tgU);
  UT3D_vc_normalize (nv, nv);

  return 0;
}


/*=======================================================================*/
  int USBS_TgVecIsoBspSur (Vector *tg, SurBSpl *sur, double t1, double t2,
		           int uv, Memspc *workSeg) {
/*=======================
USBS_TgVecIsoBspSur    Tangent vector <-- isopar. crv. bspline surface 

USBS_TgVecIsoBspSur     Author: Thomas Backmeister       11.11.2003

Computation of the tangent-vector of t1-isoparametric curve at t2.

IN:
  SurBSpl *sur       ... bspline surface
  double t1          ... surface parameter 1
  double t2          ... surface parameter 2
  int uv             ... 1: (t1,t2)=(u,v); 2: (t1,t2)=(v,u)
  Memspc *workSeg    ... temporary workspace
OUT:
  Vector *tg         ... tangent-vector (not normalized)
Returncodes:
  0 = OK
 -1 = internal error
*/

  int rc;
  void *memStart;
  CurvBSpl crv;

  //printf("USBS_TgVecIsoBspSur: Tangent vec. <-- isopar. crv. bspline sur.\n");

  // save start of workspace
  memStart = workSeg->next;

  // t1-isoparametric curve
  rc = UCBS_IsoBspCrvBspSur (&crv, workSeg, sur, t1, uv);
  if (rc < 0) return -1;

  // tangent-vector of t1-isocurve at t2
  UT3D_vc_evalparCv (tg, &crv, t2);

  // release workspace
  workSeg->next = memStart;

  return 0;
}


/*=============================== eof ===================================*/
