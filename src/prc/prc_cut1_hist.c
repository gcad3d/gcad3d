//   prc_cut1_hist.c                            2013-03-21    RF
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
  ..

-----------------------------------------------------
*/
/*!
\file  ../prc/prc_cut1_hist.c
\brief store active sate (position, toolnr, sourceLineNr ..)
\code
=====================================================
List_functions_start:

PRCE_hist_save
PRCE_hist_reset
PRCE_hist_dump

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


save current sourceLineNr, current working-position, toolnr and rapid-status.
When user goes back in source, restore corresponding position and status.

*/


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
      

#ifdef _MSC_VER
#define _CRT_SECURE_NO_DEPRECATE
#define extern __declspec(dllimport)
#endif


#include "../ut/ut_geo.h"
// #include "../ut/ut_umem.h"         // UME_reserve



//----------------------------------------------------------------
// EXTERN
// ex ../ci/NC_Main.c
extern int       APT_lNr;            // die momentane APT-LineNr


// ex ../prc/prc_cut1__.c


//----------------------------------------------------------------
// LOCAL

typedef struct {
  Point    pos;
  long     lNr;
  short    tlNr;
  char     rpd;
} typ_hist;



static typ_hist    *nc_hist = NULL;                        // was NC_recTyp


# define   hist_inc_siz  100
static int hist_siz = 0;
static int hist_nr = 0;






//================================================================
  int PRCE_hist_save (Point *oldPos, int *tlActNr, int *rapid) {
//================================================================

  // int    i1;


  // printf("PRCE_hist_save\n");


  //----------------------------------------------------------------
  // fix tabSiz
  if((hist_siz - hist_nr) < 5) {
    hist_siz += hist_inc_siz;
    nc_hist = (typ_hist*)realloc (nc_hist, hist_siz*sizeof(typ_hist));
  }


  //----------------------------------------------------------------
  nc_hist[hist_nr].lNr  = APT_lNr;
  nc_hist[hist_nr].pos  = *oldPos;
  nc_hist[hist_nr].tlNr = *tlActNr;
  nc_hist[hist_nr].rpd  = *rapid;

  ++hist_nr;


    // PRCE_hist_dump();
    // printf("ex PRCE_hist_save ..\n");

  return 0;

}


//========================================================================
  int PRCE_hist_reset (Point *oldPos, int *tlActNr, int *rapid, int lNr) {
//========================================================================
// set back in source

  int    ii;


  // printf("PRCE_hist_reset %d\n",lNr);
  // PRCE_hist_dump();


  if(lNr < 0) {        // exit
    if(hist_siz > 0) {
      free (nc_hist);
      nc_hist = NULL;
      hist_siz = 0;
      hist_nr = 0;
    }
    return 0;
  }


  if(hist_nr > 0) {
    if(nc_hist[0].lNr >= lNr) {
      // clear displist (from startingpoint = keep core-objects)
      // clear history
      hist_nr = 0;
      // reset all
      PRCE_init_dat ();
      return 0;
    }
  } else {
    return 0;
  }



  // find record with nc_hist.lNr=< lNr
  for(ii=hist_nr-1; ii>0; --ii) {
    if(nc_hist[ii].lNr < lNr) goto L_found;
  }
  return -1;


  //----------------------------------------------------------------
  // record found; reset.
  L_found:
    hist_nr = ii + 1;
    *oldPos = nc_hist[ii].pos;
    *tlActNr = nc_hist[ii].tlNr;
    *rapid = nc_hist[ii].rpd;

      // printf(" reset rec=%d %f,%f,%f t=%d r=%d\n",ii,
        // oldPos.x,oldPos.y,oldPos.z,tlActNr,rapid);

  return 0;

}


//================================================================
  int PRCE_hist_dump () {
//================================================================

  int    i1;

  printf("=========== PRCE_hist_dump %d ===========\n",hist_nr);

  for(i1=0; i1<hist_nr; ++i1) {
    // if(actPosU_tab[i1].x == UT_DB_LEER) continue;
    printf(" nc_hist[%d] lNr=%ld pos=%f,%f,%f TL=%d Rpd=%d\n",i1,
           nc_hist[i1].lNr,
           nc_hist[i1].pos.x,nc_hist[i1].pos.y,nc_hist[i1].pos.z,
           nc_hist[i1].tlNr,
           nc_hist[i1].rpd);
  }



  return 0;

}



// EOF
