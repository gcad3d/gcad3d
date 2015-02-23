//  ../ut/ut_BitTab.c                      Reiter Franz.    2013-09-27
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
2013-09-27 New. RF.

-----------------------------------------------------
*/
/*!
\file  ../ut/ut_BitTab.c
\brief array of bits
\code
inline functions in ../ut/ut_BitTab.h
=====================================================
List_functions_start:

BitTab
BitTab_ini0
BitTab_ini1
BitTab_set
BitTab_get
BitTab_clr
BitTab_cpy
BitTab_bNr
BitTab_dump

List_functions_end:
=====================================================

\endcode *//*----------------------------------------




*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../ut/ut_geo.h"              // ILIM01
#include "../ut/ut_BitTab.h"           // bit-arrays




//================================================================
  int BitTab_dump (unsigned char *bt, int bNr) {
//================================================================

  int    i1, i2, i3;

  printf("BitTab_dump %d\n",bNr);

  printf(" --- 0 1 2 3 4 5 6 7 8 9\n");

  i1 = 0;
  for(;;) {
    printf(" %3d",i1);
    for(i2=0; i2<10; ++i2) {
      i3 = ILIM01(BitTab_get(bt,i1));
      printf(" %d",i3);
      ++i1;
      if(i1 >= bNr) break;
    }
    printf("\n");
    if(i1 >= bNr) break;
  }
  return 0;

}

// EOF
