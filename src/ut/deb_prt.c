// ut_dump.c   
/*
 *
 * Copyright (C) 2020 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
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
2020-01-20 extracted from ../ut/xa.c. RF.

-----------------------------------------------------
*/
#ifdef globTag
void DEB_prt(){}
#endif

/*!
\file  ../ut/ut_dump.c
\brief dump structs, objects 
\code
=====================================================
List_functions_start:

DEB_prt_init         start/open | close debugfile;
DEB_prt_print        printd = print-formatted in debug-mode                INTERNAL

List_functions_end:
=====================================================

see also DEB_std_file

\endcode *//*----------------------------------------


// Prerequ.:
#include <stdarg.h>                 // for ...
#include "../ut/deb_prt.h"          // printd

Use with
  DEB_prt_init
  printd (..);



=========================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>              // for ...


int  DEB_prt_stat = 0;
char *debDir;


#ifdef _MSC_VER
#endif




//================================================================
  void DEB_prt_print (char* format, ...) {
//================================================================
/// print-formatted in debug-mode

  va_list va;
  char    s1[1024], s2[1200], *p;

  // printf("DEB_prt_print |%s|\n",format);

  va_start(va, format);


  if(DEB_prt_stat) {
//     vfprintf (DEB_prt_fp, format, va);
//     fflush (DEB_prt_fp);                       // 2020-01-22
   
    vsprintf(s1,format,va);
    UTX_CleanCR (s1);

#ifdef _MSC_VER
    sprintf(s2, "echo \"%s\" >> %s\\debug.dat", s1, debDir);
#else
    sprintf(s2, "echo \"%s\" >> /tmp/debug.dat",s1);
#endif

      // printf(" - DEB_prt_print |%s|\n",s2);
    system(s2);

//   } else {      // this 
//     vprintf (format, va);
  }


  va_end(va);

  return;

}

//================================================================
  void DEB_prt_init (int mode) {
//================================================================
/// \code
/// mode  1   start debug; open debugfile;
///       0   end debug;   close debugfile
/// print into debugfile with "printd"
/// debugfile = <tmpdir>debug.dat = ~/gCAD3D/tmp/debug.dat
/// \endcode

  char  s1[200];



#ifdef DEB
  //----------------------------------------------------------------
  if(mode) goto L_start;
    // end debug; close debugfile
    if(DEB_prt_stat > 1) --DEB_prt_stat;
    
    // if(!DEB_prt_stat) {
//       // if(DEB_prt_fp) {
//         // fprintf(DEB_prt_fp, "================ EOF ==============\n");
//         // fclose (DEB_prt_fp);
//         // DEB_prt_fp = NULL;
//       // }
//       // printf("++++++ file %sdebug.dat closed +++++\n",OS_get_tmp_dir());
    // }
    return;


  //----------------------------------------------------------------
  L_start:
    if(!DEB_prt_stat) {

#ifdef _MSC_VER
      debDir = getenv("TMP");
      strcpy(s1, debDir);
      strcat(s1, "\\debug.dat");
        // printf(" DEB_prt_init-1-1 |%s|\n",s1);
      remove (s1);
#else
      system("rm -rf /tmp/debug.dat");
#endif

    }
    // fprintf(DEB_prt_fp, "++++++ file %s opened +++++ level %d\n",s1,level);
    ++DEB_prt_stat;
    // printf("++++++ file %s opened +++++\n",s1);
#endif

}


// EOF
