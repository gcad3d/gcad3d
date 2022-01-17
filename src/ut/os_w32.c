/*
 *
 * Copyright (C) 2020 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
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
../ut/os_w32.c       basic OS-functions MS-Windows

see also
../ut/ut_os_w32.c    APP-specif. OS-functions
../ut/ut_os__.c      OS-independant functions
-----------------------------------------------------
*/



#include "../xa/MS_Def1.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>              // for ...

#include <errno.h>


// #define LOCAL_DEF_VAR      // damit wird "extern" im Includefile geloescht !
// #include "../ut/ut_os.h"
// #undef LOCAL_DEF_VAR        // reset extern ..


// #define snprintf my_snprintf


//================================================================
// EXTERNALS:
// errno.h:
extern int errno;





//================================================================
  void OS_get_scrRes (int *xRes, int *yRes) {
//================================================================
// OS_get_scrRes                    get total screensize

  // int  irc;
  // char s1[80];


  // TX_Error ("OS_get_scrRes - MS-win - TODO");
  // exit (-1);


  *xRes = GetSystemMetrics (0);
  *yRes = GetSystemMetrics (1);

    printf(" ex-OS_get_scrRes %d %d\n",*xRes,*yRes);

}


//===================================================================
  int snprintf (char *str, size_t size, const char *format, ...) {
//===================================================================

  int        irc;
  va_list    va;

  va_start (va, format);
  irc = vsprintf (str, format, va);
  va_end (va);

  // str[irc] = '\0';

  return 0;

}


//================================================================
  int OS_checkFilExist (char* filnam, int mode) {
//================================================================
// OS_checkFilExist         check if File or Directory exists
// mode = 0: display message sofort;
// mode = 1: just fix returncode, no message
// 
// rc = 0: no, file does NOT exist, error
// rc = 1: yes, file exists, OK.


  int     i1;
  char    fn[512];

  // printf("OS_checkFilExist |%s| %d\n", filnam, mode);

//   if(strlen(filnam) > 510) {
//     TX_Error ("OS_checkFilExist E1"); exit (-1);
//   }
// 
//   sprintf(fn, "\"%s\a",filnam);

  i1 = GetFileAttributes (filnam);
    // printf(" GetFileAttributes %d %s\n",i1,filnam);
  // returns -1=file not found; 16=file, 32=directory ?
    // printf("GetFileAttributes %d |%s|\n",i1+1,filnam);
  i1 += 1;
  if(i1 > 1) i1 = 1;

    // printf("ex OS_checkFilExist %d |%s|\n",i1,filnam);

  return i1;

}


//================================================================
  int OS_sys1 (char *sOut, int sSiz, char *cmd) {
//================================================================
/// \code
/// OS_sys1                  get systemCommand (popen); skip if starting with "##"
/// RetCod:
///     >0      OK, nr of chars returned in sOut
///    -1       cannot open pipe
///    -2       sOut too small
///    -3       execution-error
/// \endcode

  int   irc=0, ii=0, i1;
  FILE  *fPip1;
  char  s1[256], *p1;


  // printf("OS_sys1 |%s| %d\n",cmd,sSiz);

  sOut[0] = '\0';
  fflush(stdout);
  errno = 0;
  // dup2(STDOUT_FILENO, STDERR_FILENO);

  fPip1 = _popen (cmd, "r");
  if (fPip1 == NULL) {irc = -1; goto L_exit;}
    // fprintf(stderr, "_sys1-1 errno = %d\n", errno);

  while (1 == 1) {
    if (fgets (s1, 255, fPip1) == NULL) break;
    if((s1[0] == '#')&&(s1[1] == '#')) continue;
    i1 = strlen(s1);
    ii += i1 + 1;
      // printf(" s1=|%s| i1=%d ii=%d\n",s1,i1,ii);
    if(ii >= sSiz) {irc = -2; goto L_exit;}
    strcat(sOut, s1);
  }

  _pclose (fPip1);
  // UTX_CleanCR (sOut);  // remove LF from last word
  irc = strlen(sOut);
    // fprintf(stderr, " _sys1-out %d |%s|\n",irc,sOut);


  // test errno for execution-error
  if(errno) {
      // fprintf(stderr, "_sys1-2 errno = %d\n", errno);
    strcpy(sOut, "Error: ");
               // 0123456
    // strerror_r (errno, &sOut[7], sSiz - 7);
    sOut = strerror (errno);
    TX_Print (sOut);
    return -3;
  }

  L_exit:
    // printf("ex-OS_sys1 %d |%s|\n",irc,sOut);
  return irc;

}


//================================================================
  int OS_osVar_eval (char *fn, int fnSiz) {
//================================================================
// OS_osVar_eval        expand shell variables in string
// retCode:  0=OK; -1=error, -2=string-too-log
// On Windows, you can use ExpandEnvironmentStrings.
// preReq: <wordexp.h>
//
// Example: in  "${DIR_DEV}cadfiles/gcad/" 
//          out "/mnt/serv2/devel/cadfiles/gcad/"
//
// was OS_filnam_eval

  int    ii;
  char   *s1;

  s1 = _alloca (fnSiz + 32);
  strcpy(s1, fn);

  ii = ExpandEnvironmentStrings (fn, s1, fnSiz);

    printf(" ex-OS_filnam_eval %d |%s|%s|\n",ii,fn,s1);

  return 0;

}


//================================================================
  int OS_filnam_eval (char *fno, char *fni, int fnoSiz) {
//================================================================
// OS_filnam_eval        expand shell variables in filenames
// ExpandEnvironmentStrings.

  int    ii;


  ii = ExpandEnvironmentStrings (fni, fno, fnoSiz);

    // printf(" ex-OS_filnam_eval %d |%s|%s|\n",ii,fno,fni);

  return 0;

}


/* replaced by OS_get_GUI
//================================================================
  char* OS_get_zenity () {
//================================================================
// OS_get_zenity               get zenity-command
// returns "cd <bindir> && zenity"
// retCode:   NULL = zenity not installed; else command

// zenity --error --text 'text'
// zenity --entry --text="input" --entry-text=" alter text"

  static char cmd[256] = " ";

  if(cmd[0] == ' ') {
    sprintf (cmd, "cd \"%s\" && START /B /WAIT \"\" zenity.exe",
             getenv("gcad_dir_bin"));
      // printf("## get_zenity |%s|\n",cmd);  
  }

  return cmd;

}
*/


// EOF
