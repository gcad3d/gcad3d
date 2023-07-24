// ../ut/deb_prt.c
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
DO NOT USE; replaced by DEB_std_file + printf
DEB_std_file         redirect stdout -> file or back to console
see also

\endcode *//*----------------------------------------


// Prerequ.:
#include <stdarg.h>                 // for ...
#include "../ut/deb_prt.h"          // printd

Use with
  DEB_prt_init
  printd (..);

printd with -DDEB    calls DEB_prt_print
  else is skipped




=========================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>              // for ...
#include "../ut/deb_prt.h"          // printd


int  DEB_prt_stat = 0;           // 0=closed; 1=active
char *debDir;



/*
DO NOT USE - does not work with OS_sys1
//================================================================
  int DEB_std_file (int mode) {
//================================================================
// DEB_std_file                    redirect stdout -> file or back to console
// see INF_debug
// Input:
//   mode      1 = direct stdout into "/tmp/debug.dat" (MS: %TEMP%\debug.dat")
//             0 = redirect stdout to console




#ifdef _MSC_VER
  char fNam[400], *p1;
  p1 = getenv ("TEMP");           // %TEMP%
  sprintf(fn, "%s\\debug.dat",p1);

#else
  static char fNam[] = "/tmp/debug.dat";
#endif


  if(mode) {
    // redirect stdout -> file
    // fflush (stdout);
    // freopen (fNam, "w", stdout);
    OS_stdout__ (0, fNam);
    OS_stdout__ (1, NULL);

  } else {
    // redirect stdout -> console
    // fflush (stdout);
    // freopen ("/dev/tty", "w", stdout);
    OS_stdout__ (2, NULL);
  }

  return 0;

}
*/


//================================================================
  void DEB_prt_print (char* format, ...) {
//================================================================
/// print-formatted in debug-mode

  va_list va;
  char    s1[1024], s2[1200], *p;

  printf("## DEB_prt_print |%s|\n",format);

  va_start(va, format);


  if(DEB_prt_stat) {
    vsnprintf(s1,sizeof(s1),format,va);
    UTX_CleanCR (s1);

#if defined _MSC_VER || __MINGW64__
    sprintf(s2, "CMD /C \"echo \"%s\" >> \"%sdebug.dat\"\"", s1, OS_get_tmp_dir());
    OS_system(s2);
#else
    sprintf(s2, "echo \"%s\" >> /tmp/debug.dat",s1);
    system (s2);
#endif

      // TESTBLOCK
      // printf("## - DEB_prt_print |%s|\n",s2);
      // END TESTBLOCK


//   } else {      // this 
//     vprintf (format, va);
  }


  va_end(va);

  return;

}


//================================================================
  void DEB_prt_init (int mode) {
//================================================================
// mode  1   start debug; open debugfile;
//       0   end debug;   close debugfile
// print into debugfile with "printd"
// debugfile = <tmpdir>debug.dat = /tmp/debug.dat
//
// if debugfile is not enabled, printd is replaced by "if(0) printf"

  char  s1[256];


  printf("## DEB_prt_init %d\n",mode);


    // TESTBLOCK
    if(mode==1) {
#if defined _MSC_VER || __MINGW64__
      sprintf(s1, "%s\\debug.dat", OS_get_tmp_dir());
      remove(s1);
#else
      remove("/tmp/debug.dat");
#endif
      DEB_prt_stat=1;
    }

    if(mode==0) {fflush(stdout); DEB_prt_stat=0;}

    return;
    // END TESTBLOCK
 

/*
#ifdef DEB
  //----------------------------------------------------------------
  if(mode) goto L_start;
    // 1= disable debug-print; close the file
    if(DEB_prt_stat > 0) --DEB_prt_stat;
    fflush(stdout);
    fclose (stdout);
    return;


  //----------------------------------------------------------------
  L_start:
    // 0 = enable debug-print
    if(!DEB_prt_stat) {
      // init debugfile

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
    ++DEB_prt_stat;
#endif
*/

  // for OS_sys1 (popen) start debug-text with "##"
  printf("## ex-DEB_prt_init %d\n",DEB_prt_stat);


}


// EOF
