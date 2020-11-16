//             Tesselate Surfaces                 2020-05-28   RF
/*
 *
 * Copyright (C) 2020 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
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
2020-05-28  Neu ex ../gr/tess_su.c.c, RF.

-----------------------------------------------------
*/
#ifdef globTag
void GRTSU(){}
#endif
/*!
\file  ../gr/grtsu.c
\brief Tesselate Surfaces 
\code
=====================================================
List_functions_start:

GRTSU_nifac_sru             tesselate ruled-surface

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



===============================*/



#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ut/ut_geo.h"                 // Point ...
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_itmsh.h"            // MSHIG_EDGLN_.. typedef_MemTab.. Fac3

#include "../gr/tess_su.h"             // TSU_uOff

#include "../xa/xa_mem.h"              // memspc51, mem_cbuf1





//======================================================================
  int GRTSU_nifac_sru (MshFac *nfa, ObjGX *oxi) {
//======================================================================
// TSU_nifac_sru                        tesselate ruled-surface
// Input:
//   sru     ruled surface
// Output:
//   gTyp    
//   ptNr    nr of points in pa
//   va      normal-vectors, one for every triangle; can be NULL
//   oTyp    Typ_SUR
//           Typ_SURPLN - planar - only first vector in va used;
//
// see TSU_DrawSurTRU


  int       irc, ptNr, i1, sTyp, p1Max, p2Max, iTex, oTyp;
  int       iTexBas, iTexRef;
  Point     *p1Tab, *p2Tab;
  ColRGB    *col;
  TexBas    *btex;
  TexRef    *rtex;



  // init, Anzahl von Polygonpunkten fuer beide Seiten bestimmen.
  TSU_tr_init_ (oxi);

  // Randkurven in Polygone zerlegen
  // UT3D_cv_ln UT3D_cv_ci bspl_pol_bsp bspl_pol2_bsp2 ?
  ptNr = 1. / TSU_uOff(); // uOff;
    printf(" ptNr=%d uOff=%f\n",ptNr,TSU_uOff());



  // prepare space for polygons (cannot be local memspace !!)
  p1Tab = (Point*)memspc55;
  p1Max = sizeof(memspc55) / sizeof(Point);

  p2Tab = (Point*)memspc54;
  p2Max = sizeof(memspc54) / sizeof(Point);
    // printf(" p1Max=%d p2Max=%d\n",p1Max,p2Max);

  p1Max = DMIN (p1Max, p2Max);
  // if(ptNr > p1Max) ptNr = p1Max;
    printf(" ptNr=%d p1Max=%d\n",ptNr,p1Max);


  // get 2 curves
  sTyp = UT3D_cv_sru_ (p1Tab, p2Tab, &ptNr, p1Max);
    printf(" sTyp=%d ptNr=%d\n",sTyp,ptNr);

    // TESTBLOCK
    // for(i1=0;i1<ptNr;++i1) GR_tDyn_pt(&p1Tab[i1], ATT_PT_HILI);
    // for(i1=0;i1<ptNr;++i1) GR_tDyn_pt(&p2Tab[i1], ATT_COL_GREEN);
    // END TESTBLOCK


  // TODO: test if faces planar ..


  // get nfac from 2 pointArraysf-tst__
  oTyp = Typ_SURRU;
  UFA_nifac_strip (nfa, ptNr, p1Tab, p2Tab, oTyp);

  return 0;

}

// EOF
