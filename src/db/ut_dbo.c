// xa_dbo.c            2009-03-03           RF
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
2009-05-28 Created. RF.

-----------------------------------------------------
*/
#ifdef globTag
void DBO(){}
#endif
/*!
\file  ../db/ut_dbo.c
\brief database-object-functions (typ, DB-index)        DBO_ 
\code
=====================================================
List_functions_start:

DBO_dbo_src__          decode & create temp obj
DBO_dbo_ato__          create temp obj from atomic-obj
DBO_sel__              change selection into DB-object
DBO_cvStd_cvTrm        change trimmed-curve into standard-curve
DBO_dump__             dump DB-object -> debug-window

List_functions_end:
=====================================================
-see also:
ATO_ato_eval__         create dynam DB-obj from atomic-obj
APT_decode_pt2         create dynam DB-point from atomic-obj

\endcode *//*----------------------------------------




*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_geo.h"              // Point ...
#include "../gr/ut_DL.h"               // DL_GetAtt
#include "../db/ut_DB.h"               // DB_get_CV


//================================================================
  int DBO_cvStd_cvTrm (int *oTyp, long *oDbi) {
//================================================================
// DBO_cvStd_cvTrm        change trimmed-curve into standard-curve
// Input:
//   oDbi       index of DB-curve to test/change
// Output:
//   retCode    0  curve (oTyp,oDbi) is not curve or standard-curve  
//              1  trimmed-curve changed to standard-curve
//             <0  error
//   oTyp,oDbi     trimmed-curve - only if retCode==1

  int      typ1, irc=0;
  CurvCCV  *cvt;
  char     cv1[OBJ_SIZ_MAX];
  ObjGX    ox1;
  void     *vp1;


  // printf("DBO_cvStd_cvTrm %d %ld \n",*oTyp, *oDbi);

  // check if obj is curve
  if(!TYP_IS_CV(*oTyp)) return 0;

  // get typ of curve
  typ1 = DB_get_typ_cv (*oDbi);

  // check if obj is trimmed-curve
  // skip standard-curve
  if(typ1 != Typ_CVTRM) return 0;

  // get cvt = trimmed-curve-data
  cvt = DB_get_CV (&typ1, *oDbi);
    // DEB_dump_obj__ (typ1, cvt, "  cvStd_cvTrm-cvt");

  // change trimmed-curve into standard-curve
  irc = UTO_cv_cvtrm (oTyp, cv1, NULL, cvt);
  if(irc < 0) return irc;
    // DEB_dump_obj__ (*oTyp, cv1, "  cvStd_cvTrm-cv1");

  // store standard-curve dynamic in DB
  *oDbi = -1L; // dynamic
  irc = DB_store_stru (&vp1, *oTyp, *oTyp, cv1, 1, oDbi);
  if(irc < 0) return irc;

    // printf("ex-DBO_cvStd_cvTrm irc=%d typ=%d dbi=%ld \n",irc,*oTyp,*oDbi);

  return 1;

}


//================================================================
  int DBO_dbo_ato__ (long *ptInd, int *ausInd,
                     int aus_anz, int aus_typ[],double aus_tab[]){
//================================================================
/// DBO_dbo_ato__          create temp obj from atomic-obj

  printf("DBO_dbo_ato__ \n");


  return 0;

}

 
//================================================================
  int DBO_dbo_src__ (int *typ, long *dbi, char *src) {
//================================================================
/// DBO_dbo_src__          decode & create temp obj
///   using memspc53/54/55
/// TODO: combine with APED_dbo_oid

  int       i1;
  ObjAto    ato;


  // printf("DBO_dbo_src__ |%s|\n",src);

  // get memspc53/54/55
  ATO_getSpc__ (&ato);
  i1 = ATO_ato_srcLn__ (&ato, src); // 
  if(i1) {
    TX_Error ("Error DBO_dbo_src__ %d",i1);
    return -1;
  } 
    // ATO_dump__ (&ato, " after DBO_dbo_src__");
    
  *typ = ato.typ[0];
  *dbi = ato.val[0];

  return 0;

}


//================================================================
  int DBO_sel__ (int *typ, long *dbi, long dli) {
//================================================================
/// \code
/// DBO_sel__      change selection into DB-object
///   Indicate (no object): typ=Typ_TmpPT; Point[0] is always overwritten !
///
/// plugins & selectionCallbacks use that, to get selections & indicate
/// \endcode


  DL_Att  att1;
  Point   pt1;

  // handle indicate
  if(dli == -1) {
    *typ = Typ_TmpPT;
    *dbi = 0;
    UI_GR_get_actPosA (&pt1);
      // DEB_dump_obj__ (Typ_PT, &pt1, "indicatedPos=");
    DB_StorePoint (0L, &pt1);   // save as point[0]
    return 0;
  }

  // att1 = DL_GetAtt(dli);   // get DL-record
  DL_get_dla (&att1, dli);
  *typ = att1.typ;
  *dbi = att1.ind;

  return -1;

}


//================================================================
  int DBO_dump__ (int typ, long dbi) {
//================================================================
/// \code
/// dump DB-object -> debug-window
/// see also UI_dump_obj
/// \endcode

  int    irc, oTyp, oNr, i1, oSiz;
  void   *vp;


  printf("DBO_dump__ %d %ld\n",typ,dbi);

  oNr = 1;
  oTyp = typ;

  irc = UTO_objDat_dbo (&vp, &oNr, &oTyp, dbi);     // typ wird auf ObjGX gesetzt !
  if(irc < 0) {
    printf(" - temporary object\n");
    return -1;
  }

  // UTO_dump_obj (oTyp, vp, oNr);                  // dump obj ..
  oSiz = UTO_siz_stru (oTyp);
  for(i1=0; i1<oNr; ++i1) {
    DEB_dump_obj__ (oTyp, vp, "%d",i1);
    oTyp += oSiz;
  }

  return 0;

}


// EOF

