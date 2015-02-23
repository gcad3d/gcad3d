// ut_os__.c 
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
2012-01-09 Extracted from ../ut/ut_os_aix.c. RF.

-----------------------------------------------------
*/
#ifdef globTag
void OS(){}
#endif
/*!
\file  ../ut/ut_os__.c
\brief opsys functions - OS-independant.
\code
=====================================================
List_functions_start:

OS_file_sig_cre          create signal-file
OS_file_sig_wait         wait for signalfile & get its content

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



*/



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>

#include "../ut/ut_cast.h"             // INT_PTR

#include "../ut/ut_os.h"





//================================================================
  int OS_file_sig_cre (int mode, void *data) {
//================================================================
/// \code
/// OS_file_sig_cre          create signal-file
/// Input:
///   mode      0  clear file; data = signalfilename; must be done first
///             1  write data = string (null terminated) -> signalfile
///             2  write data = int (4 bytes) -> signalfile
///            99  cancelled by user (following read gives retCod -1)
///            -1  read data = string (null terminated) -> data (size of data !)
///            -2  read data = int (4 bytes) -> data
/// Output:
///   data     text or int (mode);
///   RetCod:   0  OK, data valid
///            -1  cancelled by user
/// \endcode

// after read must be clear befor next write !


  long   ls;
  char   *fmact, *fmw = "w", *fmr = "r";
  static char fn[256];
  static int  lastMode = -1;
  FILE        *fpo;


  // printf("..OS_file_sig_cre %d %d\n",mode,lastMode);

  // skip delete window before init
  if(mode > 0) {
    if(lastMode < 0) return 0;
  }


  // if(fnam) {
  if(mode == 0) {
    // fn = fnam;
    strcpy(fn, data);
      // printf(" sig_cre fn=|%s|\n",fn);

    // delete file
    OS_file_delete (fn);
    lastMode = 0;
    return 0;
  }

  //  write | read
  if(mode < 0) { 
    // read
    ((char*)data)[0] ='\0';
    if(lastMode == 99) {   // user cancelled !
      return -1;
    }
    fmact = fmr;
    ls = OS_FilSiz (fn);       //printf(" ls=%ld\n",ls);

  } else {
    // write
    fmact = fmw;
  }

  lastMode = mode;

  if((fpo=fopen(fn, fmact)) == NULL) {
    TX_Print("OS_file_sig_cre E001 %s",fn);
    return -1;
  }


  // if(data) {    can be int=0 !
    if     (mode ==  1) {
      fprintf(fpo, "%s", TXT_PTR(data));
        // printf(" wr s |%s|\n",TXT_PTR(data));

    } else if(mode ==  2) {
      fprintf(fpo, "%d", INT_PTR(data));
        printf(" wr int %d\n",INT_PTR(data));


    } else if(mode == -1) {
      // fgets(data, 250, fpo);
      fread(data, ls, 1, fpo);
      ((char*)data)[ls] = '\0';
        // printf(" rd s |%s|\n",(char*)data);

    } else if(mode == -2) {
      fscanf(fpo, "%d", (int*)data);
        // printf(" rd int %d\n",*((int*)data));
    }

  // } else {
    // if(mode > 0) fprintf(fpo, "");
  // }


  fclose(fpo);

  // rename after write: necessary if > 1 processes 




  return 0;

}


//================================================================
  int OS_file_sig_wait (int mode, void *data) {
//================================================================
// wait for signalfile & get its content
// mode     1=string, 2=int
///   RetCod:   0  OK, data valid
///            -1  cancelled by user

  int        i1;
  char       fn[240], *p1;

  // printf("OS_file_sig_wait %d\n",mode);


  // clear signal-file
  sprintf(fn, "%sGDW1.sig", OS_get_tmp_dir());
    // printf(" fn=|%s|\n",fn);
  OS_file_sig_cre (0, fn);


  // wait for answer from GUI_DialogEntryCB
  for(i1=0; i1<100; ++i1) {   // 100 sec
    GUI_update__ ();          // GUI_DialogEntry needs that !
    OS_Wait (200);    // millisecs
      // printf(" i1=%d\n",i1);
    if(OS_checkFilExist(fn, 1) == 0) continue;
    goto L_finish;
  }
  for(;;) {
    GUI_update__ ();
    OS_Wait (600);    // millisecs
    if(OS_checkFilExist(fn, 1) == 0) continue;
    goto L_finish;
  }

  // get entry back
  L_finish:
    mode *= -1;   // read

  return OS_file_sig_cre (mode, data);   // get file-content

}


// EOF
