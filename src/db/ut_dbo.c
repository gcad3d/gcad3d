// xa_dbo.c            2009-03-03           RF
/*
 *
 * Copyright (C) 2015 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
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
/// TODO: combine with APED_dbo_oid

  int       i1;
  ObjAto    ato;


  // printf("DBO_dbo_src__ |%s|\n",src);

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
      // UT3D_stru_dump (Typ_PT, &pt1, "indicatedPos=");
    DB_StorePoint (0L, &pt1);   // save as point[0]
    return 0;
  }

  att1 = DL_GetAtt(dli);   // get DL-record
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

  irc = UTO_get_DB (&vp, &oNr, &oTyp, dbi);     // typ wird auf ObjGX gesetzt !
  if(irc < 0) {
    printf(" - temporary object\n");
    return -1;
  }

  // UTO_dump_obj (oTyp, vp, oNr);                  // dump obj ..
  oSiz = UTO_siz_stru (oTyp);
  for(i1=0; i1<oNr; ++i1) {
    UT3D_stru_dump (oTyp, vp, "%d",i1);
    oTyp += oSiz;
  }

  return 0;

}


// EOF

