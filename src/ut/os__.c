// ../ut/os__.c              common OPSYS-functions Unix, MSYS
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
2023-02-28 extracted from ../ut/ut_os_aix.c
-----------------------------------------------------
*/
#ifdef globTag
void OS__(){}
#endif
// 
/*
=====================================================
List_functions_start:
 


List_functions_end:
=====================================================
see also:





_______________________________________________________________________________
*/



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>          // f ...

#include <errno.h>

// #include <dirent.h>        // f. DIR ..

#include <time.h>
#include <signal.h>

#include <sys/stat.h>        // f. mkdir , stat


#ifdef _MSC_VER
#include <io.h>        // f. _access
#include <direct.h>    // f. _mkdir
#include <time.h>      // f. time
#else
// Linux 
#include <unistd.h>          ///f. access, R_OK ..
#include <sys/time.h>
#include <sys/types.h>       // f. S_ISDIR
#endif

// #include <sys/utsname.h>
// #include <fcntl.h>



// #include "../ut/ut_txt.h"         // fnam_del_s
// #include "../ut/ut_cast.h"             // INT__PTR
// #include "../ut/func_types.h"             // FUNC_Pan FUNC_Rot FUNC_LOAD ..


//----------------------------------------------------------------
// ex ../xa/gCAD3D.c
extern char *AP_bin_dir;         // dir binaries




//================================================================
  int OS_bin_dir_set (char** argv) {
//================================================================
// OS_bin_dir_set         set binary directory (from exe-argv[0])
// - to set: OS_bin_dir_set (&argv);
// - to get: char *binDir = OS_bin_dir_get ();


  int      sl;
  char     *p1;


  // set - copy -> _bin_dir
  // find length of dir
#if defined _MSC_VER || __MINGW64__
  // MS
  p1 = strrchr (argv[0], '\\');
#else
  // Linux
  p1 = strrchr (argv[0], '/');
#endif

  if(p1) {
    sl = p1 - argv[0];
      // printf(" OS_bin_dir__-sl=%d\n",sl);
    // get memspc
    AP_bin_dir = (char*) malloc (sl + 8);
    strncpy (AP_bin_dir, argv[0], sl + 1);
    AP_bin_dir[sl + 1] = '\0';
  }

    printf(" ex-OS_bin_dir_set |%s|\n",AP_bin_dir);

  return 0;

}


//================================================================
 char* OS_bin_dir_get () {
//================================================================
// OS_bin_dir_get         get binary directory (from exe-argv[0])
// - to set: OS_bin_dir_set (&argv);
// - to get: char *binDir = OS_bin_dir_get ();
//    - or:  AP_bin_dir
// #define OS_bin_dir_get() AP_bin_dir



    // printf(" ex-OS_bin_dir_get |%s|\n",AP_bin_dir);

  return AP_bin_dir;

}


//================================================================
  int OS_tim_timA (struct tm *tm, char *stA) {
//================================================================
/// \code
/// OS_tim_timA            get timestamp from time-string
/// timestring format: |2017-10-14 10:32:07|
/// \endcode


  int    iy, im, id;

  // printf("OS_tim_timA |%s|\n",stA);


  sscanf (stA, "%4d-%2d-%2d %2d:%2d:%2d",
          &iy, &im, &id,
          &tm->tm_hour, &tm->tm_min, &tm->tm_sec);


  tm->tm_year = iy - 1900;
  tm->tm_mon  = im - 1;
  tm->tm_mday  = id;

  return 0;

}


//================================================================
  int OS_timA_diff_s (double *dd, char *sts1, char *sts2) {
//================================================================
/// \code
/// get difftime in seconds of two timestrings
/// timestring format: |2017-10-14 10:32:07|
/// see OS_timA_now
/// \endcode

  time_t tim1, tim2;         // timestamps
  struct tm ts1, ts2;        // time-structure

  // get time-structure ts1 from timestamp-string sts1
  memset(&ts1, 0, sizeof(struct tm));
  // strptime(sts1, "%Y-%m-%d %H:%M:%S", &ts1);
  OS_tim_timA (&ts1, sts1);

  // make unix-timestamp tim1 from time-structure ts1
  tim1 = mktime (&ts1);


  // get time-structure ts2 from timestamp-string sts2
  memset(&ts2, 0, sizeof(struct tm));
  // strptime(sts2, "%Y-%m-%d %H:%M:%S", &ts2);
  OS_tim_timA (&ts2, sts2);

  // make unix-timestamp tim2 from time-structure ts1
  tim2 = mktime (&ts2);


  // get d1 = Diff.Time in Sec (nur auf Sec. genau) als double !
  *dd = difftime (tim1, tim2);
    printf ("ex-OS_timA_diff_s %f secs\n",*dd);

  return 0;

}


//=============================================================
  long OS_FilSiz (char* filnam) {
//=============================================================
/// OS_FilSiz               returns filesize in bytes

  int         rc;
  struct stat fileStatus;



  rc = stat (filnam, &fileStatus);

  // printf("rc=%d siz=%d\n",rc,fileStatus.st_size);

  if(rc < 0) {  // File nicht vorh.
    // printf("ex OS_FilSiz -1 |%s|\n",filnam);
    return -1;
  }

  // printf("ex OS_FilSiz %d |%s|\n", fileStatus.st_size,filnam);

  return fileStatus.st_size;

}


//========================================================================
  int OS_file_rename (char *fnOld, char *fnNew) {
//========================================================================
// rename File; NO Wildcards !
// MS u Unix gleich.

  // printf("OS_file_rename |%s| -> |%s|\n",fnOld, fnNew);

  remove (fnNew);    // delete File (sonst get das rename ned ..)
                     // ACHTUNG: keine Wildcards mit remove !
  rename (fnOld, fnNew);

  return 0;

}


//========================================================================
  int OS_file_delete (char *fNam) {
//========================================================================
// delete File; NO Wildcards !
// MS u Unix gleich.
// retCod: 0=OK; -1=Error.

  // printf("OS_file_delete |%s|\n",fNam);

  return remove (fNam);    // ACHTUNG: keine Wildcards mit remove !

}



// EOF
