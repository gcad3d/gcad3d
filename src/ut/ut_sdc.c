//    SubdivisonCurve-Functions                 RF 2007-02-25
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
  ..

-----------------------------------------------------
*/
/*!
\file  ../ut/ut_sdc.c
\brief SubdivisonCurve-Functions
\code
=====================================================
List_functions_start:

SDC2_vc_perp2pt            Normalvektor auf einen PolygonPunkt

List_functions_end:
=====================================================

\endcode *//*----------------------------------------

*/



#define _1d12    0.0833333333333333333    //  1 / 12
#define _2d3     0.6666666666666666666    //  2 / 3



#include <math.h>
#include <stdio.h>
#include <stdlib.h>




#include "../ut/ut_geo.h"              // Point ...





//================================================================
  int SDC2_vc_perp2pt (Vector *vc, Point2 *pTab) {
//================================================================
// SDC2_vc_perp2pt            Normalvektor auf einen PolygonPunkt
//                            Thomas Backmeister.


  vc->dx = _1d12 * pTab[-2].x - _2d3  * pTab[-1].x +
           _2d3  * pTab[1].x  - _1d12 * pTab[2].x;

  vc->dy = _1d12 * pTab[-2].y - _2d3  * pTab[-1].y +
           _2d3  * pTab[1].y  - _1d12 * pTab[2].y;

  return 0;

}

//==================== EOF =======================================
