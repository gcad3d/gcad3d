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
../ut/os_uix.c       basic OS-functions unix

see also
../ut/ut_os_aix.c    APP-specif. OS-functions
../ut/ut_os__.c      OS-independant functions
-----------------------------------------------------

USING:
TX_Print



*/




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>              // for ...

#include <unistd.h>          ///f. access, R_OK ..
#include <errno.h>
#include <wordexp.h>         // OS_filnam_eval

#include "../ut/ut_os.h"            // OS_ ..
#include "../ut/deb_prt.h"          // printd


//================================================================
// EXTERNALS:

// errno.h:
extern int errno;


//=============================================================
  int OS_checkFilExist (char* filnam, int mode) {
//=============================================================
/// \code
/// OS_checkFilExist         check if File or Directory exists
///   filnam may not have shell-variables; see OS_filnam_eval
/// mode = 0: display message sofort;
/// mode = 1: just fix returncode, no message
/// mode = 2: make Errormessage (TX_Error) if File does not exist
/// 
/// rc = 0 = Fehler; datei existiert NICHT.
/// rc = 1 = OK, Datei existiert.
/// \endcode


  char    buf[256];


  // printf("OS_checkFilExist |%s| %d\n", filnam, mode);

  // remove following "/"
  strcpy (buf, filnam);

  // if last char of string == '/': remove it
  if(buf[strlen(buf)-1] == '/') buf[strlen(buf)-1] = '\0';


  /* Version PC: (braucht IO.h) */
  /* if ((access (buf, 0)) != 0) { */
  /* if ((access (buf, R_OK|W_OK)) != 0) { */

  if ((access (buf, R_OK)) != 0) {
    if (mode == 0) {
      // printf ("*** Error OS_checkFilExist: %s does not exist\n",filnam);
      MSG_get_1 (buf, 256, "NOEX_fil", "%s", filnam);
      printf ("*** Error OS_checkFilExist: %s\n", buf);
      /* UI_Dialog (NULL, buf); */

    } else if (mode == 2) {
      // TX_Error(" - File %s does not exist ..",filnam);
      MSG_err_1 ("NOEX_fil", "%s", filnam);
    }

    return 0;
  }

  // printf("ex OS_checkFilExist YES |%s| %d\n",filnam,mode);
  return 1;

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

  fPip1 = popen (cmd, "r");
  if (fPip1 == NULL) {irc = -1; goto L_exit;}
    // fprintf(stderr, "_sys1-1 errno = %d\n", errno);

  while (1 == 1) {
    if (fgets (s1, 255, fPip1) == NULL) break;
    if((s1[0] == '#')&&(s1[1] == '#')) continue;
    i1 = strlen(s1);
    ii += i1 + 1;
      // printf(" s1=|%s| i1=%d ii=%d\n",s1,i1,ii);
    if(ii >= sSiz) {irc = -2; pclose (fPip1); goto L_exit;}
    strcat(sOut, s1);
  }

  pclose (fPip1);
  // UTX_CleanCR (sOut);  // remove LF from last word
  irc = strlen(sOut);
    // printf(" _sys1-out %d |%s|\n",irc,sOut);


  // test errno for execution-error
  if(errno) {
      // fprintf(stderr, "_sys1-2 errno = %d\n", errno);
    strcpy(sOut, "Error: ");
               // 0123456
    strerror_r (errno, &sOut[7], sSiz - 7);
    TX_Print (sOut);
    return -3;
  }

  L_exit:
    // printd("ex-OS_sys1 %d |%s|\n",irc,sOut);
  return irc;

}


//================================================================
  int OS_filnam_eval (char *fno, char *fni, int fnoSiz) {
//================================================================
// OS_filnam_eval        expand shell variables in filenames
// retCode:  0=OK; -1 = no-filename; internal error ..
// On Windows, you can use ExpandEnvironmentStrings.
// preReq: <wordexp.h>

  wordexp_t p;
  char **w;
  // int i;

  // wordexp provides n results, use only first.
  wordexp(fni, &p, 0);
  if(p.we_wordc < 1) return -1;

  w = p.we_wordv;
  // for (i = 0; i < p.we_wordc; i++)
  // printf("%s\n", w[i]);
  if(strlen(w[0]) >= fnoSiz) return -1;
  strcpy (fno, w[0]);
  wordfree(&p);
  // exit(EXIT_SUCCESS);

    // printf(" ex-OS_filnam_eval |%s|%s|\n",fno,fni);


  return 0;

}


/* replaced by OS_get_GUI
//================================================================
  char* OS_get_zenity () {
//================================================================
// OS_get_zenity               get zenity-command
// retCode:   NULL = zenity not installed; else command

// zenity --error --text 'text'
// zenity --entry --text="input" --entry-text=" alter text"

  static int installed = 0;  // 0=undefined; 1=installed; -1 = not installed.
  static char *cmd = "zenity";

  if(!installed) {
    if(system("which zenity 1>/dev/null 2>/dev/null") == 0) {
      installed = 1;
    } else {
      // NOT installed !
      return NULL;
    }
  }


  return cmd;

}
*/

// EOF
