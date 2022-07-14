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

=====================================================
List_functions_start:

OS_system                Perform OS-Command; wait for completion (system)
OS_exec                  Perform OS-Command; do not wait for completion.
OS_stdout__              direct console-output into file
OS_checkFilExist         check if File or Directory exists
OS_sys1                  get systemCommand (popen); skip if starting with "##"
OS_osVar_eval        expand shell variables in string
OS_get_tmp_dir                get directory for temporary files
OS_edit__                 edit file with system-editor
OS_get_edi                get system-editor


List_functions_end:
=====================================================
// OS_filnam_eval    DO-NOT-USE  - replaced by OS_osVar_eval

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

// #include <io.h>        // f. _access
// #include <direct.h>    // f. _mkdir
// #include <time.h>      // f. time
// 
// #include <sys/stat.h>      // f. _stat

// #define LOCAL_DEF_VAR      // damit wird "extern" im Includefile geloescht !
// #include "../ut/ut_os.h"
// #undef LOCAL_DEF_VAR        // reset extern ..


// #define snprintf my_snprintf


//================================================================
// EXTERNALS:
// errno.h:
extern int errno;



//======================================================================
  int OS_file_copy (char *oldNam, char *newNam) {
//======================================================================
// OS_file_copy             copy file (rename, remove)

  // char cbuf[512];

  //printf("OS_file_copy |%s|%s|\n",newNam,oldNam);


  // auf NT gibts die Option /Y nicht !!
  // sprintf(cbuf,"COPY /Y %s %s",oldNam,newNam);
  // printf("OS_file_copy |%s|\n",cbuf);
  // system(cbuf);

/*
  if(OS_checkFilExist(newNam,1) == 1) {
    printf(" overwrite file |%s|\n",newNam);
    remove (newNam);
  }
  rename (oldNam,newNam);
*/


  // sprintf(cBuf, "copy /y %s %s",oldNam, newNam);
  // system(cBuf);


  CopyFile (oldNam, newNam, FALSE); // FALSE=overWriteYes

  // exit(0);

  return 0;

}


//========================================================================
  int OS_file_rename (char *fnOld, char *fnNew) {
//========================================================================
// rename File; keine Wildcards !
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
// delete File; keine Wildcards !
// MS u Unix gleich.


  // printf("OS_file_delete |%s|\n",fNam);

  remove (fNam);    // delete File (sonst get das rename ned ..)
                    // ACHTUNG: keine Wildcards mit remove !
  return 0;

}


//================================================================
  int OS_system (char *syscmd) {
//================================================================
// OS_system                  Perform OS-Command; wait for completion (system)
// - enclose all parameters with pathname with ""
// - enclose complete command also with ""
// os-function "system" CANNOT handle exename with blanks (does not accept
//   exename enclosed with "")

// returns 0=OK; -1=Error


  int     irc, i1;
  // char  sDir[256];
  BOOL    fExit;
  DWORD   dwExitCode, dw;
  HANDLE  hProcess,   hThread;
  PROCESS_INFORMATION    pi;
  STARTUPINFO sui;


  // printf("OS_system |%s|\n",syscmd);


  memset(&sui, 0, sizeof(sui));
  sui.cb = sizeof(sui);
  // sui.dwFlags     = STARTF_FORCEONFEEDBACK | STARTF_USESHOWWINDOW;
  // sui.wShowWindow = SW_SHOWNORMAL;

  if(!CreateProcess(
                NULL,                 // zu startende Appli (mit Pfad)
                syscmd,               // oder Commandline (appli=NULL)
                NULL,                 // Proc.Security
                NULL,                 // Thread security
                TRUE,                 // handle inheritance flag
                0, //HIGH_PRIORITY_CLASS,  // creation flags
                NULL,                 // pointer to new environment block
                NULL,   // sDir       // pointer to current directory name
                &sui,                 // pointer to STARTUPINFO
                &pi))  {              // pointer to PROCESS_INFORMATION

    irc = -1;
    goto L_exit;
  }
   


  // aufs beenden warten
  dw = WaitForSingleObject (pi.hProcess, INFINITE) ;


  irc = 0;

  if(dw != 0xFFFFFFFF) {
    // den Exitcode abfragen
    i1 = GetExitCodeProcess (pi.hProcess, &dwExitCode) ;
      // printf(" GetExitCodeProcess %d %d\n",i1,dwExitCode);
    if(dwExitCode) irc = -1;
  }

  // close the process and thread object handles 
  CloseHandle(pi.hThread);
  CloseHandle(pi.hProcess);


    // printf(" GetLastError = %d\n",GetLastError());

  
  L_exit:

    // printf(" ex-OS_system %d\n",irc);

  return irc;

}



/*
//================================================================
  int OS_system (char *buf) {
//================================================================
// OS_system                  Perform OS-Command; wait for completion (system)
// os-function "system" CANNOT handle exename with blanks (does not accept
//   exename enclosed with "")

  int  i1, ret;


  // ret=0;
  // for(i1=0; i1<strlen(buf); ++i1) {
    // // skip 1.word (hat oft Options mit /; zB dir/b - nicht aendern!)
    // if(ret == 0) {
      // if(buf[i1] == ' ') ret = 1;
    // } else {
      // if(buf[i1] == '/') buf[i1] = '\\';
    // }
  // }

    printf("OS_system |%s|\n",buf);

  ret = system(buf);

  return (ret);
}
*/


//================================================================
  int OS_exec (char* syscmd) {
//================================================================
// OS_exec                  Perform OS-Command; do not wait for completion.
// WinExec(txt, SW_NORMAL);  cannot handle exename with blanks, also not with "";
// CreateProcess nur mit CREATE_NEW_PROCESS_GROUP geht ned ..


  int     irc;
  // char  sDir[256];
  BOOL    fSuccess,   fExit;
  DWORD   dwExitCode, dw;
  HANDLE  hProcess,   hThread;
  PROCESS_INFORMATION    pi;
  STARTUPINFO sui;


  // printf("OS_exec |%s|\n",syscmd);


  memset(&sui, 0, sizeof(sui));
  sui.cb = sizeof(sui);
  // sui.dwFlags     = STARTF_FORCEONFEEDBACK | STARTF_USESHOWWINDOW;
  // sui.wShowWindow = SW_SHOWNORMAL;

  fSuccess = CreateProcess(
                NULL,                 // zu startende Appli (mit Pfad)
                syscmd,               // oder Commandline (appli=NULL)
                NULL,                 // Proc.Security
                NULL,                 // Thread security
                FALSE,                // handle inheritance flag
                HIGH_PRIORITY_CLASS|         // creation flags
                CREATE_NEW_PROCESS_GROUP,    // creation flags
                NULL,                 // pointer to new environment block
                NULL,   // sDir       // pointer to current directory name
                &sui,                 // pointer to STARTUPINFO
                &pi);                 // pointer to PROCESS_INFORMATION


  if (fSuccess) {
    // close the process and thread object handles 
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    irc = 0;


  }  else  {
    irc = -1;
  }

    // printf(" ex-OS_exec %d\n",irc);

  return irc;

}


//================================================================
  char* OS_get_edi  () {
//================================================================
//  liefert bei Linux "gedit "
//  strcpy(txbuf, OS_get_edi());


  // static char os_edi[]="gedit ";

  // XP: geht, wartet aber nicht auf Ende !
  // static char os_edi[]="start notepad ";


  static char os_edi[]="notepad ";

  return os_edi;

}


//================================================================
  int OS_edit__ (char *filnam) {
//================================================================
// <edit> <filnam>     - wait for end of process
// aufs Ende warten !

  char  cbuf[512];

  // printf("OS_edit__ |%s|%s|",OS_get_edi(),filnam);

  sprintf(cbuf,"CMD /C \"%s \"%s\"\"",OS_get_edi(),filnam);
  OS_system(cbuf);

  return 0;

}


//================================================================
  char* OS_get_tmp_dir() {
//================================================================
// OS_get_tmp_dir                get directory for temporary files
// - must end with "/" or (MS-Win) "\"



// #ifdef _MSC_VER
  static  char os_tmp_dir[256] = "\0\0", *p1;


  if(!os_tmp_dir[0]) {
    p1 = getenv ("TEMP");           // %TEMP%
    strcpy(os_tmp_dir, p1);

    // add closing  "\\" to string (filename-delimiter)
    p1 = &os_tmp_dir[strlen(os_tmp_dir)-1];
    if(*p1 != '\\') strcat(os_tmp_dir, "\\");

      // TESTBLOCK
      // printf("## OS_get_tmp_dir |%s|\n",os_tmp_dir);
      // END TESTBLOCK
  }

  return os_tmp_dir;

// #else
//   static  char u_tmp_dir[] = "/tmp/";
// #endif
// 
  // return u_tmp_dir;

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
// OS_sys1                  get systemCommand (popen); 
//   skip all lines of output starting with "##"
// Input:
//   cmd       command + parameters;
//             eg "START \"\" /WAIT /B \"cmd\" parameters"
//             command and parameters with ' ' must be enclosed with ""
//             a parameter ending with '\' must have an additional ' ' or '\'
// Output:
//   sOut      output of command
//   RetCod:
//     >0      OK, nr of chars returned in sOut
//    -1       cannot open pipe
//    -2       sOut too small
//    -3       execution-error

  int   irc=0, ii=0, i1;
  FILE  *fPip1;
  char  s1[512], *p1;


  // printf("## OS_sys1 MS |%s| %d\n",cmd,sSiz);

  sOut[0] = '\0';
  fflush(stdout);
  errno = 0;
  // dup2(STDOUT_FILENO, STDERR_FILENO);


  fPip1 = _popen (cmd, "r");
  if (fPip1 == NULL) {irc = -1; goto L_exit;}
    // fprintf(stderr, "_sys1-1 errno = %d\n", errno);

  while (1 == 1) {
    if (fgets (s1, 510, fPip1) == NULL) break;
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

  ii = ExpandEnvironmentStrings (s1, fn, fnSiz);
  if(!ii) ii = -1;
  else    ii = 0;

    // printf(" ex-OS_osVar_eval %d |%s|%s|\n",ii,s1,fn);

  return ii;

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
