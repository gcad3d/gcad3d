// ../gr/tess_vr_ut.c                             2010-02-26    RF
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
\file ../gr/tess_vr_ut.c
\brief general purpose export-import-functions 
\code
=====================================================
List_functions_start:

VR_pt_cad2vr         change CoordinateSystem CAD -> VR
VR_vc_cad2vr         change CoordinateSystem CAD -> VR

VR_pt_vr2cad         change CoordinateSystem VR -> CAD
VR_vc_vr2cad         change CoordinateSystem VR -> CAD

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_geo.h"              // Point ...



//================================================================
  int VR_pt_cad2vr (Point *pt1) {
//================================================================
// change CoordinateSystem CAD -> VR

  double   vry;

  vry = pt1->y;

  pt1->y = pt1->z;
  pt1->z = -vry;

  return 0;

}


//================================================================
  int VR_vc_cad2vr (Vector *vc1) {
//================================================================
// change CoordinateSystem CAD -> VR

  double   vry;


  // UT3D_stru_dump (Typ_VC, vc1, "VR_vc_cad2vr:");

  vry = vc1->dy;

  vc1->dy = vc1->dz;
  vc1->dz =  -vry;

  // UT3D_stru_dump (Typ_VC, vc1, "ex VR_vc_cad2vr:");

  return 0;

}


//================================================================
  int VR_pt_vr2cad (Point *pt1) {
//================================================================
// change CoordinateSystem VR -> CAD

  double   vry;

  vry = pt1->y;

  pt1->y = -(pt1->z);
  pt1->z =  vry;

  return 0;

}


//================================================================
  int VR_vc_vr2cad (Vector *vc1) {
//================================================================
// change CoordinateSystem VR -> CAD

  double   vry;

  vry = vc1->dy;

  vc1->dy = -(vc1->dz);
  vc1->dz =  vry;

  return 0;

}


// EOF
