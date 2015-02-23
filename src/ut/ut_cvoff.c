//**************************************************************************
/*
 *
 * Copyright (C) 2015 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
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
2007-04-30 UCVO_cv_OffSurCv statt UT3D_cv_offsurf 

-----------------------------------------------------
*/
/*!
\file  ../ut/ut_cvoff.c
\brief offset curve on surface
\code
=====================================================
List_functions_start:

UCVO_cv_OffSurCv           Offset surface curve <-- curve

List_functions_end:
=====================================================

\endcode *//*----------------------------------------




Returncodes:
 0 OK
>0 OK with additional info
<0 Error with additional info


compile with gcc:
cc -c -O0 -g3 -Wall ucv.c

Windows:
cl -c ucv.c

ctags -f ucv.tag ucv.c

*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../ut/ut_geo.h"
#include "../ut/ut_math.h"
#include "../ut/ut_TX.h"
#include "../ut/ut_cvoff.h"
#include "../ut/ubscrv.h"



/*=======================================================================*/
  int UCVO_cv_OffSurCv (void *offcv, Memspc *memSeg1, void *cv, int cvt,
                     void *sf, int sft, double offd, Memspc *memSegT[4],
                     Memspc *workSeg) {
/*=================
UCVO_cv_OffSurCv    Offset surface curve <-- curve 

UCVO_cv_OffSurCv     Author: Thomas Backmeister       16.1.2007

Given a curve lying on a surface a new curve with the same parameter
range is computed such that for every parameter the difference vector
of the corresponding curve points is normal to the supporting surface.

Type, structure and size of the resulting curve are the same as of
the input curve.

IN:
  void *cv           ... a curve
  int cvt            ... type of curve
  void *sf           ... a surface (support of curve)
  int sft            ... type of surface
  double offd        ... offset distance
  Memspc *memSegT[4] ... temporary workspace
  Memspc *workSeg    ... temporary workspace
OUT:
  void *offcv        ... offset curve
  Memspc *memSeg1    ... data space of offset curve
Returncodes:
  0 = OK
 -1 = internal error
*/

  printf("UCVO_cv_OffSurCv: Offset surface curve <-- curve\n");

  switch (cvt)
  {
    case Typ_CVBSP:
      switch (sft)
      { 
        case Typ_SURBSP:
          UCBS_OffSurBspCrv ((CurvBSpl*)offcv, memSeg1, (CurvBSpl*)cv,
	                     (SurBSpl*)sf, offd, memSegT, workSeg);
          break;

        default:
          printf("UCVO_cv_OffSurCv: unexpected surface\n");
          return -1;
	  break;
      } 
      break;

    default:
      printf("UCVO_cv_OffSurCv: unexpected curve\n");
      return -1;
      break;
  }

  return 0;
}


/*=============================== eof ===================================*/
