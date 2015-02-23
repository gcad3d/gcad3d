//   xa_appDat.c                  2011-02-01                   RF
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
2011-02-01 Erstmalig erstellt. RF.

-----------------------------------------------------
*/
/*!
\file  ../xa/xa_appDat.c
\brief  application data
\code
=====================================================
List_functions_start:

MSH_aload_appDat        copy section "APPDAT" into file <temp>/<appNam>.appdat

List_functions_end:
=====================================================

\endcode *//*----------------------------------------

*/


#ifdef _MSC_VER
#include "MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>                    // isdigit

#include "../ut/ut_os.h"               // OS_get_bas_dir ..




//================================================================
   int MSH_aload_appDat (char *lBuf, int bufSiz, FILE *fpi) {
//================================================================
/// \code
/// MSH_aload_appDat        copy section "APPDAT" into file <temp>/<appNam>.appdat
/// lbuf = buffer with line  "SECTION APPDAT appNam"
/// \endcode


  char    fNam[256];
  FILE    *fpo;


  sprintf(fNam, "%s%s.appdat",OS_get_tmp_dir(),&lBuf[15]);

  printf("MSH_aload_appDat |%s|%s|\n",fNam,lBuf);

  if((fpo=fopen(fNam,"w")) == NULL) {
    TX_Error("Mod_aload_sm E001 %s",fNam);
    return -1;
  }


  // i1 = 0;
  while (!feof (fpi)) {
    if (fgets (lBuf, bufSiz, fpi) == NULL) break;
    UTX_CleanCR (lBuf);
    if(!strcmp(lBuf, "SECTIONEND")) break;
    fprintf(fpo,"%s\n",lBuf);
    // ++i1;
  }
  // printf(" i1=%d\n",i1);

  fclose (fpo);

  return 0;

}


// EOF
