/*   ../ut/GUI_os_uix.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // for ...

#include <signal.h>
#include <unistd.h>                    // access, R_OK ..
#include <errno.h>
#include <wordexp.h>         // OS_osVar_eval__



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
  char* OS_get_tmp_dir() {
//================================================================
// OS_get_tmp_dir                get directory for temporary files
// - must end with "/" or (MS-Win) "\"

  static  char u_tmp_dir[] = "/tmp/";

  return u_tmp_dir;

// #endif

}


// eof
