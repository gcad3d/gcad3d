/*  ../ut/ut_col.c
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

-----------------------------------------------------
Modifications:
2015-10-07 new; imported from xa.c. RF.

-----------------------------------------------------


*/
#ifdef globTag
 void Col(){}
#endif
/*!
\file  ../ut/ut_col.c
\brief main application functions; not depending on GUI.
\code
=====================================================
List_functions_start:

Col_set_3db         colour from 3 doubles
Col_set__           create colour from red,green,blue
Col_dump            dump ColRGB
Col_DL_Att          get pointer -> Color out of DL_Att     INLINE
Col_set_vsym        set style, symbolic|shaded

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#include "../ut/ut_geo.h"         // OFF, ON ..




//================================================================
  int Col_set__ (ColRGB *col1, int cr, int cg, int cb) {
//================================================================
/// \code
/// Col_set__       create surface-colour from red,green,blue
/// Set vtra,vsym,vtex = 0; color = 1.
/// Input:
///   cr         red part of colour; 0-255
///   cg         green part of colour; 0-255
///   cb         blue part of colour; 0-255
/// Output:
///   col1
///
/// Examples 1:
///   int   i1;                    // or ColRGB i1
///   Col_set__ (&i1, 12,12,122);  // set colour 
///   GR_Disp_sur (ox1, i1);       // apply colour i1
///
/// Examples 2:
///   AP_SetCol3i (12,112,12, 1);  // set defaultcolor
///   GR_Disp_sur (ox1, 0);        // use defaultcolor
///
/// see also AP_SetCol3i AP_SetCol__
/// \endcode

  static int  i0 = 0;


  // printf("Col_set__ %d %d %d\n",cr,cg,cb);


  *col1 = *((ColRGB*)&i0);
  // col1->vtra  = 0;
  // col1->vsym  = 0;
  // col1->vtex  = 0;

  col1->color = 1;

  col1->cr =  cr;
  col1->cg =  cg;
  col1->cb =  cb;

  return 0;

}


//================================================================
  int Col_set_3db (ColRGB *col1, double d1, double d2, double d3) {
//================================================================
/// Col_set_3db    colour from 3 doubles (values 0.0 - 1.0)

// see  wrl1_r_dec_Col_ Col_set__

  // printf("Col_set_3db d1=%f d2=%f d3=%f\n",d1,d2,d3);

  col1->cr = d1 * 255;
  col1->cg = d2 * 255;
  col1->cb = d3 * 255;

  // 0 als Col ist Default - geht ned !!
  if(col1->cr < 1) col1->cr = 1;
  if(col1->cg < 1) col1->cg = 1;
  if(col1->cb < 1) col1->cb = 1;

  // col1->typ = 0;   // diffuseColor
  col1->color = 1;
  col1->vtra  = 0;
  col1->vsym  = 0;
  col1->vtex  = 0;

    // printf(" newCol=%d %d %d\n",col1->cr,col1->cg,col1->cb);

  return 0;

}


//================================================================
  int Col_dump (ColRGB *col) {
//================================================================
// Col_dump                 dump ColRGB

  char   cAtt[80];


  if(col->vtex == 0) {
    sprintf(cAtt, "Color RGB %02x%02x%02x, transp=%d, sym=%d color=%d",
            col->cr,col->cg,col->cb,col->vtra,col->vsym,col->color);
  } else {
    sprintf(cAtt, "texture %d",((stru_c3c1*)col)->b123);
  }

  printf("ColRGB = %s\n",cAtt);


  return 0;

}


// EOF
