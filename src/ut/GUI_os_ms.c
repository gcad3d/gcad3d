/* ../gui/GUI_os.c            os-functions for GUI_*[.exe]
 *
 *
*/




#if defined _MSC_VER || __MINGW64__
#define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>              // for ...
#include <math.h>
#include <stdarg.h>

#include "../ut/deb_prt.h"          // print

// #include <errno.h>


// errno.h:
// extern int errno;


  char* OS_get_edi  ();


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
  int OS_system (char *syscmd) {
//================================================================
// OS_system                  Perform OS-CommOS_systemand; wait for completion (system)
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


  printf("OS_system |%s|\n",syscmd);


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

    printf(" ex-OS_system %d\n",irc);

  return irc;

}


//================================================================
  int OS_osVar_eval__ (char *so, char *si, int soSiz) {
//================================================================
// OS_osVar_eval__        expand shell variables in filenames
// ExpandEnvironmentStrings.

  int    ii;


  ii = ExpandEnvironmentStrings (si, so, soSiz);
  if(!ii) ii = -1;
  else    ii = 0;

    // printf(" ex-OS_osVar_eval_1 %d |%s|%s|\n",ii,s1,fn);

  return ii;

}


//================================================================
  char* OS_get_tmp_dir() {
//================================================================
// OS_get_tmp_dir                get directory for temporary files
// - must end with "/" or (MS-Win) "\"



// #ifdef _MSC_VER
  static  char os_tmp_dir[256]= "\0\0", *p1;


  if(!os_tmp_dir[0]) {
    p1 = getenv ("TEMP");           // %TEMP%
    strcpy(os_tmp_dir, p1);

    // add closing  "\\" to string (filename-delimiter)
    p1 = &os_tmp_dir[strlen(os_tmp_dir)-1];
    if(*p1 != '\\') strcat(os_tmp_dir, "\\");

      // TESTBLOCK
      printd("## OS_get_tmp_dir |%s|\n",os_tmp_dir);
   // END TESTBLOCK
  }

  return os_tmp_dir;

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

  printf("OS_checkFilExist |%s| %d\n", filnam, mode);

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

    printf("ex OS_checkFilExist %d |%s|\n",i1,filnam);

  return i1;

}


// EOF

