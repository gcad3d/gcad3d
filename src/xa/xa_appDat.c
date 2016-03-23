//   xa_appDat.c                  2011-02-01                   RF
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
ApplicationData is stored with the model (as ascii-text).
Store ApplicationData:
  open file with appdat_open__(&fp, "w")
  write data (as ascii-text !)

Restore ApplicationData:
  open file with appdat_open__(&fp, "r")
  read data (as ascii-text !)



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

appdat_open__       open applicationData-file for read or write
appDat_aload        copy section "APPDAT" into file <temp>/<appNam>.appdat

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







//----------------------------------------------------------------
//---------------- external vars: ---------------------

// ../xa/xa.c
extern char APP_act_nam[128];    // name of last script- or plugin-program







//================================================================
  int appdat_open__ (FILE **fo, char *mode) {
//================================================================
/// \code
/// appdat_open__       open applicationData-file for read or write
/// Input:
///   mode     "w"=write, "r"=read
/// Output:
///   fo       file, already opened
///   retCod   0=OK,  -1=error
/// 
/// ApplicationData is stored with the model (as ascii-text).
/// Store ApplicationData:
///   open file with appdat_open__(&fp, "w")
///   write data (as ascii-text !)
/// Restore ApplicationData:
///   open file with appdat_open__(&fp, "r")
///   read data (as ascii-text !)
/// 
/// See ../APP/Demo_appDat1.c
/// \endcode

  FILE    *fpo;
  char    fNam[256];


  sprintf(fNam, "%s%s.appdat",OS_get_tmp_dir(),APP_act_nam);
    printf(" appdat_open__ |%s|%s|\n",fNam,mode);


  if((fpo=fopen(fNam,mode)) == NULL) {
    printf("**** appdat-file does not exist; first call ..\n");
    // TX_Print("appdat_open__ E001 %s",fNam);
    return -1;
  }

  *fo = fpo;
  return 0;

}



//================================================================
   int appDat_aload (char *lBuf, int bufSiz, FILE *fpi) {
//================================================================
/// \code
/// appDat_aload        copy section "APPDAT" into file <temp>/<appNam>.appdat
/// Write all data until "SECTIONEND" from file fpi
///   into file <tmpdir>/<appNam>.appdat
/// Data can be ascii|binary
/// Input:
///   lbuf    buffer with line  "SECTION APPDAT appNam nrBytes"
///   bufSiz  size of lbuf
/// \endcode


  int     i1;
  long    bNr;
  char    fNam[256], fncNam[128];
  FILE    *fpo;


  // extract appNam from "SECTION APPDAT appNam nrBytes"
  //                      0123456789012345
  sscanf (&lBuf[15], "%s %ld", fncNam, &bNr);
    printf(" fNam=|%s| bNr=%ld\n",fncNam,bNr);
  sprintf(fNam, "%s%s.appdat",OS_get_tmp_dir(),fncNam);


  printf("appDat_aload |%s|%s| siz=%d\n",fNam,lBuf,bufSiz);

  if(bNr > bufSiz) {
    TX_Error("appDat_aload E001");
    return -1;
  }

  i1 = fread (lBuf, 1, bNr, fpi);
  if(i1 < 1) {
    TX_Error("appDat_aload E002");
    return -1;
  }


  if((fpo = fopen (fNam, "wb")) == NULL) {
    TX_Error("appDat_aload E003"); 
    return -1;
  }
  fwrite(lBuf, 1, bNr, fpo);
  fclose (fpo);


  // read until SECTIONEND
  L_r9:
    fgets (lBuf, bufSiz, fpi);  // get "SECTIONEND"
      printf(" _aload %ld\n",strlen(lBuf));
    if(!strncmp(lBuf, "SECTIONEND", 10)) return 0;
    if(!feof (fpi)) goto L_r9;

    TX_Error("appDat_aload E004"); 
    return -1;

}


// EOF
