// ../ut/cv_parl_pln.c                  RF                     2018-08-16
/*
 *
 * Copyright (C) 2018 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
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
2018-08-16 New. RF.

-----------------------------------------------------
*/
/*!
\file  ../ut/cv_parl_pln.c
\brief parallel polygon to polygon; offset normal to vector.
\code
=====================================================
List_functions_start:

UT3D_npt_parl_pln         parallel polygon to polygon; offset normal to vector.

List_functions_end:
=====================================================
see also:
../ut/ut_npt.c

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




/*
//=========================================================================
  int UT3D_npt_parl_pln (Point *pTabo, int *ptNo, Point *pTabi, int ptNi,
                         Vector *vz, double dist, int iClo) {
//=========================================================================
// UT3D_npt_parl_pln          polygon parallel polygon on plane, dist
// Input:
//   pTabi,ptNi  contour; pTabo must have size >= ptNi
//   iClo        closed; 0=yes, 1=not_closed;
// Output:
//   ptNo        nr of points out; < ptNi
//   pTabo       contour parallel to pTabi;


  int    i1, ii1, ii2, ii3, iiAct, io, iiOld;
  double d1, d2;
  Vector vci1, vci2, vco1, vco2, vcy;
  Vector vcOld, vcAct;
  Point  *ptOld, *ptNxt;
  unsigned short *iTab;
 
  printf("UT3D_npt_parl_pln ptNi=%d iClo=%d\n",ptNi,iClo);


  // loop tru pTab and offset all points
  ii1 = ptNi - 1;
  ii2 = ptNi - 2;


  // get tempSpc for iTab=index of parallel inputSeg for outSeg
  if(ptNi > INT_16_MAX) {TX_Error("UT3D_npt_parl_pln E1"); return -1;}
  iTab = (short*) MEM_alloc_tmp (ptNi * sizeof (short));

  // set out-segment-startpoint normal to first in-segment
  // vc1 = vector of first input-segment
  UT3D_vc_2pt (&vcOld, &pTabi[0], &pTabi[1]);

  // vcy = normal to first input-segment
  UT3D_vc_perp2vc  (&vcy, vz, &vcOld);
  UT3D_vc_setLength (&vcy, &vcy, 1.);
  UT3D_vc_multvc (&vcy, &vcy, dist);

  // set out-segment-startpoint normal to in-segment-startpoint
  pTabo[0] = pTabi[0];
  UT3D_pt_add_vc__ (&pTabo[0], &vcy);
    DEB_dump_obj__ (Typ_PT, &pTabo[0], "  pto-0");



  //----------------------------------------------------------------
  // get intersectionpoints. iiAct=index into pTabi; io=index into pTabo.
  iiAct = 1;
  io = 0;

  L_int_nxt0:
      printf("------------- L_int_nxt0: iiAct=%d io=%d\n",iiAct,io);
    // last stored out-point is pTabo[io],
    // vector for next outSeg = vcOld (vec of last inSeg)
    // find intersection with in-seg[iiAct]
      DEB_dump_obj__ (Typ_VC, &vcOld, "  vcOld");

    // keep index of parallel inSeg
    iTab[io] = iiAct;

    // vcAct = vector of next input-segment
    UT3D_vc_2pt (&vcAct, &pTabi[iiAct], &pTabi[iiAct+1]);
      DEB_dump_obj__ (Typ_VC, &vcAct, "  vcAct");

    // vcy = normal to next input-segment
    UT3D_vc_perp2vc  (&vcy, vz, &vcAct);
    UT3D_vc_setLength (&vcy, &vcy, 1.);
    UT3D_vc_multvc (&vcy, &vcy, dist);

    // set outSegEndPt normal to in-segment-startpoint
    pTabo[io+1] = pTabi[iiAct];
    UT3D_pt_add_vc__ (&pTabo[io+1], &vcy);

    // intersect (outSegStartPt, vcOld) X (outSegEndPt,vcAct)
    // get parameters of intersectionpoint
    UT3D_2par_int2pt2vc (&d1, &d2, &pTabo[io], &vcOld, &pTabo[io+1], &vcAct);
      printf(" iiAct=%d io=%d  d1=%f d2=%f\n",iiAct,io,d1,d2);

    // get intersectionpoint from vcAct * d2
    UT3D_vc_multvc (&vco2, &vcAct, d2);
    UT3D_pt_add_vc__ (&pTabo[io+1], &vco2);
      DEB_dump_obj__ (Typ_VC, &vco2, "  vco2");

    // if segmentLength < UT_TOL_cv skip seg
    if(UT3D_comp2pt (&pTabo[io], &pTabo[io+1], UT_TOL_cv)) goto L_int_nxt9;


    // test if new seg vco1 is parallel or antiparalell to vcOld (previous inSeg)
    UT3D_vc_2pt (&vco1, &pTabo[io], &pTabo[io+1]);
      DEB_dump_obj__ (Typ_VC, &vco1, "  vco1");
    i1 =  UT3D_comp2vc_p (&vcOld, &vco1, UT_TOL_min1); // 1=parl, else not
      printf(" dir-of-iiAct=%d is %d\n",iiAct,i1);
    // if out-segment goes reverse: overwrite pTabo[io]
    if(!i1) {
      // new seg is antiparallel (goes reverse); skip previous inSeg;
      // intersect last stored outSeg with active inSeg vcAct
      // overwrite pTabo[io]; do not creat new outSeg.
      // get vcOld = vc of last stored seg
      UT3D_vc_2pt (&vcOld, &pTabo[io-1], &pTabo[io]);
      // reset old outPt to normal to in-segment-startpoint
      pTabo[io] = pTabi[iiAct];
      UT3D_pt_add_vc__ (&pTabo[io], &vcy);
      // intersect
      UT3D_2par_int2pt2vc (&d1, &d2, &pTabo[io-1], &vcOld, &pTabo[io], &vcAct);
      // set old outPt to intersectionPt
      UT3D_vc_multvc (&vco2, &vcAct, d2);
      UT3D_pt_add_vc__ (&pTabo[io], &vco2);
        DEB_dump_obj__ (Typ_PT, &pTabo[io], "  overwrite: pto[%d]",io);
        // DEB_dump_obj__ (Typ_VC, &vco2, "  vco2");
      // set vcOld to 
      --iiOld;
      goto L_int_nxt9;
    }


    // keep seg ..
    ++io;
    vcOld = vcAct;
    iiOld = iiAct;
      DEB_dump_obj__ (Typ_PT, &pTabo[io], "  pto-%d",io);



    L_int_nxt9:
    ++iiAct;
    if(iiAct >= ii1) goto L_int_end;

    goto L_int_nxt0;


  //----------------------------------------------------------------
  // offset last point
  L_int_end:
      printf("---------------------- _npt_parl_pln-L_int_end:\n");
    ++io;
    UT3D_vc_perpvc2pt  (&vcy,  vz, &pTabi[ii2], &pTabi[ii1]);
    UT3D_vc_setLength (&vcy, &vcy, 1.);
    UT3D_vc_multvc (&vcy, &vcy, dist);
    pTabo[io] = pTabi[ii1];
    UT3D_pt_add_vc__ (&pTabo[io], &vcy);
      DEB_dump_obj__ (Typ_PT, &pTabo[io], "  last=%d",io);

    *ptNo = io + 1;  // points = segments + 1


  //----------------------------------------------------------------
  // fix "closed-contour"
  if(iClo) goto L_exit;

    // intersect first and last segmet; set as start/endpoint.
    UT3D_vc_2pt (&vco1, &pTabi[0], &pTabi[1]);
    UT3D_vc_2pt (&vco2, &pTabi[ii2], &pTabi[ii1]);
    UT3D_2par_int2pt2vc (&d1, &d2, &pTabo[0], &vco1, &pTabo[ii2], &vco2);
      printf(" closed-d1=%f d2=%f\n",d1,d2);
    UT3D_vc_multvc (&vco1, &vco1, d1);
    UT3D_pt_add_vc__ (&pTabo[0], &vco1);
    pTabo[ii1] = pTabo[0];



  //----------------------------------------------------------------
  L_exit:
    printf("ex-UT3D_npt_parl_pln %d\n",*ptNo);
    GR_Disp_npti (*ptNo, pTabo, SYM_TRI_S, ATT_COL_RED, ATT_COL_YELLOW);
    for(i1=0; i1< *ptNo; ++i1) {
      DEB_dump_obj__ (Typ_PT,&pTabo[i1], "%d",i1);
    }
 
  return 0;

}
*/


//=========================================================================
  int UT3D_npt_parl_pln (Point *pTabo, int *ptNo, Point *pTabi, int ptNi,
                         Vector *vz, double dist, int iClo) {
//=========================================================================
// UT3D_npt_parl_pln          polygon parallel polygon on plane, dist
//   polgon-out is in same plane as polygon-in;
// Input:
//   pTabi,ptNi  contour; pTabo must have size >= ptNi
//   iClo        closed; 0=yes, 1=not_closed;
//   dist        offset
// Output:
//   ptNo        nr of points out; < ptNi
//   pTabo       contour parallel to pTabi;
//   retCod      0 = OK;
//              -1 = degenerated curve; 1 point only


  int    i1, ii1, ii2, ii3, iin;
  double d1, d2;
  Vector vc1, vc2;

 
  printf("UT3D_npt_parl_pln ptNi=%d iClo=%d\n",ptNi,iClo);


  ii1 = ptNi - 1;
  ii2 = ptNi - 2;
  ii3 = ptNi - 3;


  // loop tru pTab and offset all points
  for(i1=0; i1<ii1; ++i1) {
    iin = i1 + 1;
    // get vecY from vecX=Line and vecZ=WC_sur_act.vz
    UT3D_vc_perpvc2pt  (&vc2, vz, &pTabi[i1], &pTabi[iin]);
    UT3D_vc_setLength (&vc2, &vc2, 1.);
    UT3D_vc_multvc (&vc1, &vc2, dist);
    pTabo[i1] = pTabi[i1];
    UT3D_pt_add_vc__ (&pTabo[i1], &vc1);
  }


  // offset last point
  UT3D_vc_perpvc2pt  (&vc2,  vz, &pTabi[ii2], &pTabi[ii1]);
  UT3D_vc_setLength (&vc2, &vc2, 1.);
  UT3D_vc_multvc (&vc1, &vc2, dist);
  pTabo[ii1] = pTabi[ii1];
  UT3D_pt_add_vc__ (&pTabo[ii1], &vc1);
    DEB_dump_obj__ (Typ_PT, &pTabo[ii1], "  last-%d",ii1);



  // get intersectionpoints
  for(i1=0; i1<ii2; ++i1) {
    iin = i1 + 1;
    UT3D_vc_2pt (&vc1, &pTabi[i1], &pTabi[iin]);
    UT3D_vc_2pt (&vc2, &pTabi[iin], &pTabi[iin+1]);
    // get parameters of intersectionpoint
    UT3D_2par_int2pt2vc (&d1, &d2, &pTabo[i1], &vc1, &pTabo[iin], &vc2);
      printf(" %d-%d  d1=%f d2=%f\n",i1,iin,d1,d2);

    // get intersectionpoint from vc1 * d1
    UT3D_vc_multvc (&vc1, &vc1, d1);
    pTabo[iin] = pTabo[i1];
    UT3D_pt_add_vc__ (&pTabo[iin], &vc1);
      DEB_dump_obj__ (Typ_PT, &pTabo[iin], "  %d",iin);
  }

  *ptNo = ptNi;


  //================================================================
  // fix "closed-contour"
  if(iClo) goto L_ck_rev;

  // intersect first and last segmet; set as start/endpoint.
  UT3D_vc_2pt (&vc1, &pTabi[0], &pTabi[1]);
  UT3D_vc_2pt (&vc2, &pTabi[ii2], &pTabi[ii1]);
  UT3D_2par_int2pt2vc (&d1, &d2, &pTabo[0], &vc1, &pTabo[ii2], &vc2);
    printf(" closed-d1=%f d2=%f\n",d1,d2);
  UT3D_vc_multvc (&vc1, &vc1, d1);
  UT3D_pt_add_vc__ (&pTabo[0], &vc1);
  pTabo[ii1] = pTabo[0];


  //================================================================
  // find intersectionpoints, remove islands
  L_ck_rev:
    // TESTBLOCK
    GR_Disp_cv (pTabo, ptNi, Typ_Att_dash__);
    // END TESTBLOCK

  // find intersectionpoints, remove islands
  // i1 = UT3D_npt_parl_pl__ (pTabo, ptNo, pTabi, ptNi, iClo);


  //================================================================
  L_exit:

  return 0;

}


//=======================================================================
  int UT3D_npt_parl_pl__ (Point *pTabo, int *pno, Point *pTabi, int ptNi,
                          int iClo) {
//=======================================================================
// remove intersections (islands)
// Input:
//   pTabi      contour, ptNi points
//   pTabo      contour parallel to pTabi, has also ptNi points
// Output:
//   retCod      0 = OK;
//              -1 = degenerated curve; 1 point only


  int    irc, i1, i2, ii1, ii2, iBwd, iFwd, ptNr, id1, iAct, iPrv, iNxt, iNnx;
  char   *sStat;
  double d1, d2;
  Vector vci, vco, vcPrv, vcNxt;
  Point  ptx;


  printf("UT3D_npt_parl_pl__ XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX \n");
  printf(" ptNi=%d iClo=%d\n",ptNi,iClo);
  for(i1=0; i1<ptNi; ++i1)
    printf("%4d %12.2f %12.2f %12.2f\n",i1,
           pTabo[i1].x,pTabo[i1].y,pTabo[i1].z);



  ii1 = ptNi - 1;


  // get space for segment-status
  sStat = MEM_alloc_tmp (ptNi);
  for(i1=0; i1<ptNi; ++i1) sStat[i1] = 10;  // init sStat = 10


  //----------------------------------------------------------------
  // loop tru contours and find parallel|antiparallel
  for(i1=0; i1<ii1; ++i1) {
    irc = UT3D_npt_parl_pl_stat (sStat, i1, pTabi, pTabo, ptNi, iClo);
    if(irc < 0) return -1;
  }


  //----------------------------------------------------------------
  // if a segment goes reverse (antiparallel), eliminate this segment, but
  // intersect the previous and the following segment and
  // update the sStat for the these segments.


  iAct = 0;
  
  L_int_nxt:
    // segStartPt = pt-index;
    // segEndPt   = next-VALID-point

    // skip deleted seg / too short seg
    if(sStat[iAct] < -10) goto L_int_end;

    // skip normal seg
    if(sStat[iAct] == 1) goto L_int_end;

    // intersect
    irc = UT3D_npt_parl_pl_int (iAct, pTabi, pTabo, sStat, ptNi, iClo);
    if(irc >= 0) {
      iAct = irc;
      goto L_int_nxt;
    }
    return irc;   // error

    L_int_end:
      ++iAct;
      if(iAct < ii1) goto L_int_nxt;



  //----------------------------------------------------------------
    // TESTBLOCK
    for(i1=0; i1<ptNi; ++i1)
      printf("%4d s=%3d %12.2f %12.2f %12.2f\n",i1,sStat[i1],
             pTabo[i1].x,pTabo[i1].y,pTabo[i1].z);
    // END TESTBLOCK


  //----------------------------------------------------------------
  // remove points marked -11
  // find ii1 = first point to remove
  ii1 = ptNi;
  for(i1=0; i1<ptNi; ++i1) {
    if(sStat[i1] > -10) continue;  // skip normal points
    ii1 = i1;
    break;
  }
  
  if(ii1 < ptNi) {
    for(i1=ii1 + 1; i1<ptNi; ++i1) {
      if(sStat[i1] < -10) continue; // skip deleted points
      // copy point i1 -> ii1
      pTabo[ii1] = pTabo[i1];
      ++ii1;
    }
  }

  *pno = ii1;


  //----------------------------------------------------------------
    // TESTBLOCK
    printf("------------------- ex-npt_parl_pl_i %d\n",*pno);
    for(i1=0; i1 < *pno; ++i1)
      printf("%4d %12.2f %12.2f %12.2f\n",i1,
             pTabo[i1].x,pTabo[i1].y,pTabo[i1].z);
    // END TESTBLOCK

  if(*pno < 2) return -1;

  return 0;

}



//============================================================================
  int UT3D_npt_parl_pl_int (int iAct, Point *pTabi, Point *pTabo, char *sStat,
                            int ptNi, int iClo) {
//============================================================================
// Input:
//   iAct      next seg to intersect (first with sStat=0 (reverse to inp-contour)
//   iClo        closed; 0=yes, 1=not_closed;
// Output:
//   retCod    0-n  OK; next seg to test
//             -1   Error


  int    irc, i1, iPrv, iNxt, nSeg;
  double d1, d2;
  Vector vcPrv, vcNxt, vc1;


  nSeg = ptNi - 1;

  printf("------------------- UT3D_npt_parl_pl_int iAct=%d\n",iAct);
  for(i1=0; i1<nSeg; ++i1)  printf("%4d sStat=%d\n",i1,sStat[i1]);

  // segStartPt = pt-index;
  // segEndPt   = next-VALID-point

  // get iPrv = startPt of prev-seg
  iPrv = UT3D_npt_parl_pl_oPrv (iAct, pTabo, sStat, ptNi, iClo);
  if(iPrv < 0) {TX_Error("UT3D_npt_parl_pl__ EP1"); return -1;}

  // get iNxt = startPt of next-seg = endPt of  prev-seg
  iNxt = UT3D_npt_parl_pl_oNxt (iAct, pTabo, sStat, ptNi, iClo);
  if(iNxt < 0) {TX_Error("UT3D_npt_parl_pl__ EN1"); return -1;}

  // mark seg (iPrv+1) - (iNxt-1) deleted
  if(iNxt > iPrv) {
    for(i1=iPrv+1; i1<iNxt; ++i1) sStat[i1] = -11;
  } else {
    // closed contour
    for(i1=iPrv+1; i1<ptNi-1; ++i1) sStat[i1] = -11;
    for(i1=0; i1<iNxt; ++i1) sStat[i1] = -11;
  }
    for(i1=0; i1<nSeg; ++i1)  printf("%4d sStat=%d\n",i1,sStat[i1]);

  // intersect (outSegStartPt, vcPrv) X (outSegEndPt,vcNxt)
  UT3D_vc_2pt (&vcPrv, &pTabo[iPrv], &pTabo[iPrv+1]);
  UT3D_vc_2pt (&vcNxt, &pTabo[iNxt], &pTabo[iNxt+1]);
  // get parameters of intersectionpoint
  irc = UT3D_2par_int2pt2vc (&d1,&d2, &pTabo[iPrv], &vcPrv, &pTabo[iNxt], &vcNxt);
    printf(" iPrv=%d iNxt=%d irc=%d d1=%f d2=%f\n",iPrv,iNxt,irc,d1,d2);
  if(irc < 0) {
    // parallel; set outermost points -> pTabo[iNxt]
    pTabo[iNxt] = pTabo[iNxt+1];

  } else {
    // get pt-iNxt = intersectionpoint from vc1 * d1
    UT3D_vc_multvc (&vc1, &vcPrv, d1);
    pTabo[iNxt] = pTabo[iPrv];
    UT3D_pt_add_vc__ (&pTabo[iNxt], &vc1);
  }

    DEB_dump_obj__ (Typ_PT, &pTabo[iNxt], "  %d",iNxt);

  // if closed & first point: set also last point.
  if(!iClo) {
    if(iNxt == 0) pTabo[ptNi-1] = pTabo[iNxt];
  }

  // update sStat for iPrv
  irc = UT3D_npt_parl_pl_stat (sStat, iPrv, pTabi, pTabo, ptNi, iClo);
  if(irc < 0) return -1;


  // update sStat for iNxt
  irc = UT3D_npt_parl_pl_stat (sStat, iNxt, pTabi, pTabo, ptNi, iClo);
  if(irc < 0) return -1;

  // must reWork iPrv
    printf("ex-UT3D_npt_parl_pl_int %d\n",iPrv);

  return iPrv;

}


//=======================================================================
  int UT3D_npt_parl_pl_oPrv (int iAct, Point *pTabo, char *sStat, int ptn,
                             int iClo) {
//=======================================================================
// get index = startPt of previous fwd-seg (sStat=1)

  int  iPrv;


  iPrv = iAct;

  L_nxt:
    --iPrv;

  if(iPrv < 0) {
    TX_Error("UT3D_npt_parl_pl_oPrv TODO-1"); return -1;
  }

  if(sStat[iPrv] < 1) goto L_nxt;


  //----------------------------------------------------------------
    printf("ex-parl_pl_oPrv iPrv=%d for iAct=%d\n",iPrv,iAct);

  return iPrv;

}


//=======================================================================
  int UT3D_npt_parl_pl_oNxt (int iAct, Point *pTabo, char *sStat, int ptn,
                             int iClo) {
//=======================================================================
// get index = startPt of next fwd-seg (sStat=1)
//   iClo        closed; 0=yes, 1=not_closed;

  int  iNxt;


  iNxt = iAct;

  L_nxt:
    ++iNxt;

    if(iClo) {
      // not closed
      if(iNxt >= ptn) {TX_Error("UT3D_npt_parl_pl_oNxt TODO-1"); return -1;}
    } else {
      // closed
      if(iNxt >= ptn-1) iNxt = 0;
    }

    // skip 0 (reverse-seg) and -11 (deleted-seg)
    if(sStat[iNxt] < 1) goto L_nxt;


  //----------------------------------------------------------------
    printf("ex-parl_pl_oNxt iNxt=%d for iAct=%d\n",iNxt,iAct);

  return iNxt;

}


//============================================================================
  int UT3D_npt_parl_pl_stat (char *sStat, int iSeg,
                             Point *pTabi, Point *pTabo, int ptn, int iClo) {
//============================================================================
// - set parallel|antiparallel for this segment
// - delete segment if too short (< UT_TOL_cv)
// - delete segment if not closed, antiparallel and first or last seg
// Input:


  int    iNxt,  oNxt;
  Vector vci, vco;


  printf("UT3D_npt_parl_pl_stat %d\n",iSeg);


  // get oNxt = endPt of seg iSeg in pTabo
  oNxt = UT3D_npt_parl_pl_oNxt (iSeg, pTabo, sStat, ptn, iClo);
  if(oNxt < 0) {TX_Error("UT3D_npt_parl_pl_stat E1"); return -1;}

  // get out-seg
  UT3D_vc_2pt (&vco, &pTabo[iSeg], &pTabo[oNxt]);
    DEB_dump_obj__ (Typ_VC, &vco, " vco %d-%d",iSeg,oNxt);

  // skip segment if too short (< UT_TOL_cv)
    printf(" lenB_vc=%f tol=%f\n",UT3D_lenB_vc (&vco),UT_TOL_cv);
  if(UT3D_lenB_vc (&vco) < UT_TOL_cv) {
    sStat[iSeg] = -11;
    goto L_exit;
  }



  // get iNxt = endPt of seg iSeg in pTabi
  // TODO: UT3D_npt_parl_pl_iNxt
  iNxt = iSeg + 1;
  if(iNxt >= ptn) {TX_Error("UT3D_npt_parl_pl_stat TODO-1"); return -1;}


  // get inp-seg
  UT3D_vc_2pt (&vci, &pTabi[iSeg], &pTabi[iNxt]);

  // set parallel|antiparallel for this segment
  sStat[iSeg] = UT3D_comp2vc_p (&vci, &vco, UT_TOL_min1); // 1=parallel, 0=not



  // - delete segment if not closed, antiparallel and first or last seg
  if(iClo) goto L_exit;
  if(sStat[iSeg] != 0) goto L_exit;
  if((iSeg > 0) && (iSeg < (ptn - 1))) goto L_exit;
  sStat[iSeg] = -12;


  //----------------------------------------------------------------
  L_exit:

    printf("ex-UT3D_npt_parl_pl_stat iSeg=%d iNxt=%d oNxt=%d sStat=%d\n",
           iSeg,iNxt,oNxt,sStat[iSeg]);

  return 0;

}


/*
//=======================================================================
  int UT3D_npt_parl_pl__ (Point *pTabo, int *pno, Point *pTabi, int ptNi,
                          int iClo) {
//=======================================================================
// remove intersections (islands)
// Input:

  int    irc, i1, i2, ii1, ii2, iBwd, iFwd, ptNr, id1;
  char   *sStat;
  double d1, d2;
  Vector vc1, vc2;
  Point  ptx;


  printf("UT3D_npt_parl_pl__ XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX \n");
  printf(" ptNi=%d iClo=%d\n",ptNi,iClo);


  ptNr = ptNi;
  ii1  = ptNr - 1;

  // get space for segment-status
  sStat = MEM_alloc_tmp (ptNi);

  // loop tru contours and find parallel|antiparallel
  for(i1=0; i1<ii1; ++i1) {
    // set parallel|antiparallel for this segment2yy
    UT3D_vc_2pt (&vc1, &pTabi[i1], &pTabi[i1+1]);
    UT3D_vc_2pt (&vc2, &pTabo[i1], &pTabo[i1+1]);
    sStat[i1] = UT3D_comp2vc_p (&vc1, &vc2, UT_TOL_min1);
      printf(" s%d sStat=%d\n",i1,sStat[i1]);  // 1=parallel, 0=not
  }

// TEST ALSO last/first-segment ?
  // if(iClo) {
  // // set parallel|antiparallel for last segment
  // sStat[ii1] = UT3D_comp2vc_p (&vc1, &vc2, UT_TOL_min1);
  // }



  //----------------------------------------------------------------
  // check segments around reverse-segments for intersection
  L_ck_rev:
  // set iBwd = first reverse-segment
  ii1 = ptNr - 1;
  for(i1=0; i1<ii1; ++i1) {
    if(sStat[i1]) continue;
    iBwd = i1;
    if(iBwd < 0) {
      {TX_Error("UT3D_npt_parl_pl__ TODO-1"); return -1;}
      // if(iClo) ..
    }
    goto L_int_1;
  }
  // no more intersections ..
  goto L_exit;



  // set iFwd = last reverse-segment
  L_int_1:
  iFwd = -1;
  for(i1=iBwd+1; i1<ii1; ++i1) {
    if(!sStat[i1]) continue;
    iFwd = i1 - 1;
    break;
  }
  if(iFwd < 0) {TX_Error("UT3D_npt_parl_pl__ E1"); return -1;}
    printf(" iFwd=%d iBwd=%d\n",iFwd,iBwd);

  i1 = iFwd;
  i2 = iBwd;

  // ++iFwd
  L_fwd:
    ++i1;
    if(i1 >= ptNr) {TX_Error("UT3D_npt_parl_pl__ E2"); return -1;}


  // --iBwd
  L_bwd:
    --i2;
    if(i2 < 0) {
      i2 = iBwd;
      goto L_fwd;
    }


    // intersect iFwd X iBwd
    irc = UT3D_pt_int2pt2pt_lim (&ptx, NULL, NULL,
                                 &pTabo[i1], &pTabo[i1+1],
                                 &pTabo[i2], &pTabo[i2+1], UT_TOL_cv);
      printf(" int- i1=%d X i2=%d  irc=%d\n",i1,i2,irc);
    if(irc) goto L_bwd;
      DEB_dump_obj__ (Typ_PT, &ptx, " ptx");


    //----------------------------------------------------------------
    // intersection on segments i1,i2; intersectionPoint is ptx.

    // delete points and sStat;
    // id1 = first point to delete
    id1 = iBwd;
    // ii1 = nr of points, sStat to delete
    ii1 = iFwd - iBwd + 1;
      printf(" id1=%d ii1=%d\n",id1,ii1);


    ii2 = ptNr;
    MEM_del_nrec (&ii2, pTabo, id1, ii1, sizeof(Point));
    MEM_del_nrec (&ptNr, sStat, id1, ii1, 1);

    // set endPt of iBwd = startPt of iFwd = ptx
    pTabo[iBwd] = ptx;

    // continue
    goto L_ck_rev;


  //================================================================
  L_exit:

    *pno = ptNr;
    printf("ex-UT3D_npt_parl_pl__ pno=%d\n",*pno);


  return 0;

}
*/


//===================== EOF ===========================
