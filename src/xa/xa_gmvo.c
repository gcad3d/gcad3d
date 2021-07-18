// xa_gmvo.c                                      2018-02-14  RF
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
2018-02-14 neu erstellt. RF.

-----------------------------------------------------
*/
#ifdef globTag
 void GMVO(){}
#endif
/*!
\file  ../xa/xa_gmvo.c
\brief Group-MoVe-Objects
\code
=====================================================
List_functions_start:

GMVO__

List_functions_end:
=====================================================
see also
PED_init

\endcode *//*----------------------------------------



*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // va_list
#include <ctype.h>                     // isdigit


#include "../ut/ut_geo.h"              // Point ...

#define   TRUE 1
#define   FALSE 0






//================================================================
  int GMVO__ (int mode) {
//================================================================
// move all objs of group

  printf("GMVO__ %d\n",mode);

  // exit if group empty

  return 0;

}

// EOF
