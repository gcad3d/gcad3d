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
../ut/os_uix.c         basic OS-functions unix     - this file
../ut/ut_os_aix.c      APP-specif. OS-functions
../ut/os_dll_uix.c
../ut/ctrl_os_aix.c
../ut/ut_os__.c        OS-independant functions

../ut/os_exe_file.c    functions for auxiliary-executables (GUI_* ..); not core

=====================================================
List_functions_start:

OS_system                Perform OS-Command; wait for completion (system)
OS_exec                  Perform OS-Command; do not wait for completion.
OS_stdout__              direct console-output into file
OS_checkFilExist         check if File or Directory exists
OS_sys1                  get systemCommand (popen); skip if starting with "##"
OS_osVar_eval__          expand shell variables in string
OS_osVar_eval_1          expand shell variables in string
OS_get_tmp_dir           get directory for temporary files
OS_edit__                edit file with system-editor
OS_get_edi               get system-editor

OS_FIND_STR_DELI     find first occurence of  directory-delimiter '/' or '\'
OS_FIND_STRR_DELI    find last occurence of  directory-delimiter '/' or '\'

List_functions_end:
=====================================================

USING:
TX_Print



*/




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // for ...

#include <signal.h>
#include <unistd.h>                    // access, R_OK ..
#include <errno.h>

#include "../ut/ut_os.h"               // OS_ ..
#include "../ut/deb_prt.h"             // printd
// #include "../ut/ut_mem.h"              // MEM__alloc_tmp


//================================================================
// EXTERNALS:

// errno.h:
extern int errno;



//==========================================================================
  int OS_file_copy (char *oldNam, char *newNam) {
//==========================================================================
// OS_file_copy             copy file - force overwrite
//   retCode      0=OK; else Error

  char cbuf[512];

  // printf("OS_file_copy |%s|%s|\n",oldNam,newNam);

  sprintf(cbuf,"/bin/cp -f \"%s\" \"%s\"",oldNam,newNam);
    // printf(cBuf, "copy /y %s %s",fnOld, fnNew);  // MS
    // printf("OS_file_copy |%s|\n",cbuf);

  return system (cbuf);

  // return 0;

}


//================================================================
  int OS_system (char *buf) {
//================================================================
// OS_system                  Perform OS-Command; wait for completion (system)
//   do not wait: use OS_exec

// Spezialversion fuer AIX + CATIA.


  void *catch;
  int ret;

#ifndef __MINGW64__
  catch = signal(SIGCLD, SIG_DFL);   // SIGCLD | SIGCHLD
#endif
				    
  ret = system(buf);

#ifndef __MINGW64__
  signal(SIGCLD, catch);
  if (ret) { perror(buf); }
#endif

  return(ret);

}


//================================================================
  int OS_exec (char* txt) {
//================================================================
// OS_exec                  Perform OS-Command; do not wait for completion.
//   do wait: use OS_system

// es geht nur system mit &

// alle exec*: 
//   timer expired ..

// mit fork:
//  The program '<unknown>' received an X Window System error.
//  The error was 'BadAccess (attempt to access private resource denied)'.
//  Xlib: unexpected async reply (sequence 0x209d)!

  char cbuf[256];

  sprintf(cbuf, "%s &", txt);

  OS_system (cbuf);

  return 0;

}


//================================================================
  int OS_edit__ (char *filnam) {
//================================================================
// OS_edit__                 edit file with system-editor
// waits for end of process.
// DO NOT USE: use APP_edit

  char  cbuf[720];

  snprintf(cbuf, sizeof(cbuf), "%s %s",OS_get_edi(),filnam);

  // printf("OS_edit_ |%s|\n",cbuf);
  // TX_Print("%s",cbuf);

  OS_system(cbuf);

  return 0;

}


//================================================================
  char* OS_get_edi () {
//================================================================
///  returns fileEditorProgram; eg "gedit "  or "kedit "

  static char sEd[8];


  sEd[0] = '\0';

  if(system("which gedit 1>/dev/null 2>/dev/null") == 0)
    strcpy(sEd, "gedit ");
  if(system("which pluma 1>/dev/null 2>/dev/null") == 0)
    strcpy(sEd, "pluma ");
  if(system("which kwrite 1>/dev/null 2>/dev/null") == 0)
    strcpy(sEd, "kwrite ");
  if(system("which kedit 1>/dev/null 2>/dev/null") == 0)
    strcpy(sEd, "kedit ");
  if(system("which kate 1>/dev/null 2>/dev/null") == 0)
    strcpy(sEd, "kate ");
  if(system("which dtpad 1>/dev/null 2>/dev/null") == 0)
    strcpy(sEd, "dtpad ");
  if(system("which gvim 1>/dev/null 2>/dev/null") == 0)
    strcpy(sEd, "gvim ");

  if(strlen(sEd) < 2) { 
    printf(" **** no Editor found\n");
  }

  return sEd;
}


//================================================================
  char* OS_get_tmp_dir() {
//================================================================
// OS_get_tmp_dir                get directory for temporary files
// - must end with "/" or (MS-Win) "\"



// #ifdef _MSC_VER
//   static  char os_tmp_dir[256] = "\0\0", *p1;
// 
// 
//   if(!os_tmp_dir[0]) {
//     p1 = getenv ("TEMP");           // %TEMP%
//     strcpy(os_tmp_dir, p1);
//     UTX_add_fnam_del (os_tmp_dir);
// 
//       // TESTBLOCK
//       // printf(" OS_get_tmp_dir |%s|\n",os_tmp_dir);
//       // END TESTBLOCK
//   }
// 
//   return os_tmp_dir;
// 
// #else
  static  char u_tmp_dir[] = "/tmp/";

  return u_tmp_dir;

// #endif

}


//================================================================
  int OS_stdout__ (int mode, void *data) {
//================================================================
// OS_stdout__              direct console-output into file
// mode     0   set filename for console-output
// mode     1   (re-)open file for console-output
//          2   close console-output into file; continue with output to console ..

  static int     o;
  static char    fn[256];
  static FILE    *fp = NULL;


  // printf("OS_stdout__ %d |%s|\n",mode,fn);


  if(mode == 0) {
    // set filenam only
    if(strlen((char*)data) >= 250) {printf("***** OS_stdout__ E1 \n"); return -1;}
    strcpy(fn, (char*)data);


  } else if(mode == 1) {
    // 1 = direct stdout into file fn
    if(fp) { fflush (fp); fclose (fp);}
    // o = dup(fileno(stdout));
    clearerr ( stdout);
    fp = freopen (fn, "a", stdout);
    if(!fp) {printf("***** OS_stdout__ E2 \n"); return -1;}


  } else {
    // 2 = redirect stdout to console
    // printf(" -------- OS_stdout__ close debugfile ---------------\n");
    if(fp) {
      fflush (fp);
      // stdout = fdopen (2, "w");
      fclose (fp);
      fp = NULL;
      clearerr ( stdout);
      freopen ("/dev/tty", "a", stdout);
      // dup2 (o, fileno(stdout));
      // close(o);
    }
  }

  return 0;

}


//=============================================================
  int OS_checkFilExist (char* filnam, int mode) {
//=============================================================
/// \code
/// OS_checkFilExist         check if File or Directory exists
///   filnam may not have shell-variables; see OS_osVar_eval__
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
// OS_sys1                  get systemCommand (popen); skip if starting with "##"
// DO NOT USE freopen (.. stdout) in <cmd>
// output-lines starting with "##" are skipped (debug-output)
// RetCod:
//     >0      OK, nr of chars returned in sOut
//    -1       cannot open pipe
//    -2       sOut too small
//    -3       execution-error

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
#ifdef __MINGW64__
    sOut = strerror (errno);
#else
    strerror_r (errno, &sOut[7], sSiz - 7);
#endif
    TX_Print (sOut);
    return -3;
  }

  L_exit:
    // printd("ex-OS_sys1 %d |%s|\n",irc,sOut);
  return irc;

}


#ifdef __MINGW64__ 

//================================================================
  int OS_osVar_eval__ (char *so, char *si, int soSiz) {
//================================================================
// OS_osVar_eval__        expand shell variables in string
// retCode:  0=OK; -1=error, -2=string-too-log
// On Windows, you can use ExpandEnvironmentStrings.
// preReq: <wordexp.h>
//
// Input:
//   si     string to expand; eg "${DIR_DEV}cadfiles/gcad/"
//   soSiz  size of so
// output:
//   so     expanded text; eg "/mnt/serv2/devel/cadfiles/gcad/"
//
// was OS_osVar_eval__

  int    ii;

  ii = ExpandEnvironmentStringsA (si, so, soSiz);
  if(!ii) ii = -1;
  else    ii = 0;

    // printf(" ex-OS_osVar_eval_1 %d |%s|%s|\n",ii,si,so);

  return ii;

}

#else

#include <wordexp.h>         // OS_osVar_eval__
			     
//================================================================
  int OS_osVar_eval__ (char *so, char *si, int soSiz) {
//================================================================
// OS_osVar_eval__        expand shell variables in string
// retCode:  0=OK; -1=error, -2=string-too-log
// On Windows, you can use ExpandEnvironmentStrings.
// preReq: <wordexp.h>
//
//
// Input:
//   si     string to expand; eg "${DIR_DEV}cadfiles/gcad/"
//   soSiz  size of so
// output:
//   so     expanded text; eg "/mnt/serv2/devel/cadfiles/gcad/"
//

  int  irc = 0;
  char **w;
  wordexp_t p;


  // printf("OS_osVar_eval_1 |%s|\n",fn);

  // wordexp provides n results, use only first.
  wordexp(si, &p, 0);
  if(p.we_wordc < 1) {irc = -1; goto L_exit;}

  w = p.we_wordv;
  // for (i = 0; i < p.we_wordc; i++)
  // printf("%s\n", w[i]);
  if(strlen(w[0]) >= soSiz) {irc = -2; goto L_exit;}
  strcpy (so, w[0]);
  wordfree(&p);
  // exit(EXIT_SUCCESS);

  L_exit:

    // printf(" ex-OS_osVar_eval_1 |%s|\n",so);

  return 0;

}

#endif


//================================================================
  int OS_osVar_eval_1 (char *sio, int sSiz) {
//================================================================
// OS_osVar_eval_1        expand shell variables in string
// Input:
//   si     string to expand; eg "${DIR_DEV}cadfiles/gcad/"
//   soSiz  size of so
// output:
//   so     expanded text; eg "/mnt/serv2/devel/cadfiles/gcad/"


  int    irc;
  char   *s1;

  // s1 = (char*)MEM_alloc_tmp (sSiz + 32);
  s1 = alloca (sSiz + 32);
  strcpy(s1, sio);

  irc = OS_osVar_eval__ (sio, s1, sSiz);

    // printf(" ex-OS_osVar_eval_1 %d |%s|%s|\n",irc,s1,sio);

  return irc;

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
