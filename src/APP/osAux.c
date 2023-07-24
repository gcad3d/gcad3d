/* osAux
 - do auxiliary work - eg copy files ...
 - build:        cc ../APP/osAux.c -o osAux
 - run           ./osAux.sh

[DIROUT]
[DIRSRC]
[COPYFILE]

*/

#if defined _MSC_VER || __MINGW64__
#include <windows.h>
#include <processenv.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ut/ut_TX.h"        // TX_Error TX_Print
#include "../xa/xa_msg.h"       // MSG_get_1 MSG_err_1 MSG_pri_1


char    s0[512], s1[1024], s2[1024];
char    sx[2600];


  int DIROUT ();
  int DIRSRC ();
  int COPYFILE ();

  char* UTX_CleanCR (char* string);
  int UTX_fnam_ck_abs (char *fn);

  int OS_osVar_eval_1 (char *sio, int sSiz);
  int OS_osVar_eval__ (char *so, char *si, int soSiz);
  int OS_checkFilExist (char* filnam, int mode);





//================================================================
  int main (int argc, char *argv[]) {
//================================================================
// Input: fileName of commandfile

  FILE    *fpi;
  int     (*fnc)();


  printf(" osAux - |%s|\n",argv[1]);


  if((fpi = fopen ( argv[1], "r")) == NULL) {
    printf ("osAux - ERROR open file %s\n", argv[1]);
    return -1;
  }

  //----------------------------------------------------------------
  while (!feof (fpi)) {
    if (fgets (s0, 512, fpi) == NULL) break;
    UTX_CleanCR (s0);
    if(s0[0] == '#') continue;             // skip comments
    if(s0[0] == '\0') continue;            // skip empty line
      // printf(" -------------- |%s|\n",s0);


    //----------------------------------------------------------------
    if(s0[0] == '[') {
        // printf(" L_do_command |%s|\n",s0);

      //----------------------------------------------------------------
      if(!strcmp (s0,"[DIROUT]")) {
	fnc = DIROUT;
  
      //----------------------------------------------------------------
      } else if(!strcmp (s0,"[DIRSRC]")) {
	fnc = DIRSRC;
  
      //----------------------------------------------------------------
      } else if(!strcmp (s0,"[COPYFILE]")) {
        fnc = COPYFILE;

      //----------------------------------------------------------------
      } else {
        printf ("osAux - ERROR unknown command %s\n", s0);
        return -2;
      }

      continue;
    }

    //----------------------------------------------------------------
    (*fnc)();   // execute ..

  }


}


//================================================================
  int DIROUT () {
//================================================================

  int    irc;
  char   *p1;

    // printf(" DIROUT |%s|\n",s0);
  strcpy(s2, s0);

  // expand shell variables
  p1 = strchr (s2, '%');
  if(p1) OS_osVar_eval_1 (s2, sizeof(s2));

  // test if directory exists
  if(OS_checkFilExist(s2, 1) == 0) {
    // dir s2 does not exist; create
    sprintf (sx, "install -d \"%s\"", s2);
      printf("%s\n",sx);
    irc = system (sx);
  }

  return irc;
}


//================================================================
  int DIRSRC () {
//================================================================

  char   *p1;

    printf(" DIRSRC |%s|\n",s0);
  strcpy(s1, s0);

  // expand shell variables
  p1 = strchr (s1, '%');
  if(p1) OS_osVar_eval_1 (s1, sizeof(s2));

  return 0;
}


//================================================================
  int COPYFILE () {
//================================================================

  int     irc;

  // check for absolute;
  if(UTX_fnam_ck_abs(s0)) {
    // absolut; cp <fn> <OUT>.
    sprintf (sx, "cp -fP \"%s\" \"%s.\"", s0, s2);
      printf("%s\n",sx);


  } else {
    // default; cp <SRC><fn> <OUT>.
    sprintf (sx, "cp -fP \"%s%s\" \"%s.\"", s1, s0, s2);
  }


    printf("do: %s\n",sx);
  irc = system (sx);

  return irc;

}



//===========================================================
  char* UTX_CleanCR (char* string) {
//===========================================================
// UTX_CleanCR              Delete Blanks, CR's and LF's at end of string
// returns positon of stringterminator \0
//
// see also UTX_del_follBl UTX_CleanSC


  int  ilen;
  char *tpos;

  ilen = strlen (string);

  tpos = &string[ilen];

  if(ilen < 1) goto L_exit;

  --tpos;


  while ((*tpos  == ' ')  ||
         (*tpos  == '\t') ||          /* tab */
         (*tpos  == '\n') ||          /* newline */
         (*tpos  == '\r'))   {        /* CR */

    *tpos    = '\0';
    if(tpos <= string) goto L_exit;
    --tpos;
  }

  ++tpos;


  L_exit:
  // printf("ex UTX_CleanCR |%s|\n", string);

  return tpos;
}


//================================================================
  int UTX_fnam_ck_abs (char *fn) {
//================================================================
// UTX_fnam_ck_abs                      check if filename has absolute path
// retCode   1=path-is-absolute; 0=path-is-relative

  int    irc;


#if defined _MSC_VER || __MINGW64__
  // - MS: if second char == ':'   - absPath
  if((fn[0] == '\\')  ||       // UNC-filenames
     (fn[1] == ':'))      {irc = 1; goto L_exit;}
#endif
  // MS & UIX: if first char is '/'
  if(fn[0] == '/')  {irc = 1; goto L_exit;}

  irc = 0; // rel.

  L_exit:
    return irc;

}


/*
//================================================================
  int OS_osVar_eval_1 (char *sio, int sSiz) {
//================================================================
// OS_osVar_eval_1        expand shell variables in string
// retCode:  0=OK; -1=error, -2=string-too-log
// Input:
//   si     string to expand; eg "${DIR_DEV}cadfiles/gcad/"
//   soSiz  size of so
// output:
//   so     expanded text; eg "/mnt/serv2/devel/cadfiles/gcad/"


  int    irc;
  char   *s1;

  s1 = _alloca (sSiz + 32);
  strcpy(s1, sio);

  irc = OS_osVar_eval__ (sio, s1, sSiz);

    // printf(" ex-OS_osVar_eval_1 %d |%s|%s|\n",irc,s1,sio);

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

    // printf(" ex-OS_osVar_eval__ %d |%s|%s|\n",ii,so,si);

  return ii;

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
//   char    fn[512];

  // printf("OS_checkFilExist |%s| %d\n", filnam, mode);

  if(strlen(filnam) > 510) {
    printf ("OS_checkFilExist E1"); exit (-1);
  }

//   strcpy(fn, filnam);
//   UTX_fnam_ms_uix (fn);
  // sprintf(fn, "\"%s\a",filnam);

  i1 = GetFileAttributes (filnam);
  // returns -1=file not found; 16=file, 32=directory ?
    // printf(" GetFileAttributes %d %s\n",i1,filnam);
    // printf("GetFileAttributes %d |%s|\n",i1+1,filnam);
  i1 += 1;
  if(i1 > 1) i1 = 1;

    // if(i1 < 1) printf("ex OS_checkFilExist %d |%s|\n",i1,filnam);

  return i1;

}
*/

// EOF
