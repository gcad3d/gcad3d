/* ../ut/ut_os_w32.c                                       

    begin                : Fri Aug 25 2000
    copyright            : (C) 2000 by CADCAM-Services Franz Reiter
    email                : franz.reiter@cadcam.co.at
_______________________________________________________________________________
OS-Routines f. Mircosoft                 RF.
Includefile: ../ut/ut_os.h


TODO:
 OS_check_isDir derzeit noch nicht ff !!

-----------------------------------------------------
Modifications:
2002-02-22 aus os_ms.cpp erstellt (nur tw). RF.

-----------------------------------------------------
*//*!
\file ../ut/ut_os_w32.c
\ingroup grp_ut_os
\code

=====================================================
List_functions_start:

OS_hide_win          nur MS-Windows

List_functions_end:
=====================================================
see  ../ut/ut_os_aix.c


// Mircosoft-Test: MS-CompilerVersion; 2010=1600
#ifdef _MSC_VER
  printf(" %d\n",_MSC_VER);
// Mircosoft-Windows-test 32-or-64-Bit:
#ifdef _WIN64
  printf("This is a MS64-OS ! \n");
  printf(" _WIN64 %d\n",_WIN64);
#else
  printf("This is a MS32-OS ! \n");
#endif
#else
// Linux
#ifdef _LP64
  // Linux64
#else
  // Linux32
#endif




\endcode *//*----------------------------------------



Compile:
cl -c ut_os_w32.c

Linken:
Braucht advapi32.lib (f. GetUserName).
*/


#include "../xa/MS_Def1.h"

#include <stdio.h>               //sscanf
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <errno.h>

#include <io.h>        // f. _access
#include <direct.h>    // f. _mkdir
#include <time.h>      // f. time

#include <sys/stat.h>      // f. _stat


#include "../ut/ut_txt.h"              // fnam_del_s
#include "../ut/func_types.h"          // FUNC_..


#define YES                0
#define NO                 1

#define PTRSIZ             sizeof(void*)   // 4 || 8



#define LOCAL_DEF_VAR      // damit wird "extern" im Includefile geloescht !
#include "../ut/ut_os.h"
#undef LOCAL_DEF_VAR        // reset extern ..




//_____________________________________
// exported VARS:


//_____________________________________
// static VARS:
// wird von div Funktionen retourniert !
static char txbuf[256];


//_____________________________________
// EXTERNALS:
// aus xa.c:

// errno.h:
extern int errno;


//----------------------------------------------------------------
// dummy-funcs
  int OS_config () { return 0;}
  int OS_ck_libcVersion (int vMaj, int vMin) { return 0;}




// DUMMY-funcs:
OS_file_date_m (time_t *tim_m, char *filnam) {}
OS_date_cmp (time_t *tim1, time_t *tim2) {}



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

    // printf(" ex-OS_get_scrRes %d %d\n",*xRes,*yRes);

}


//================================================================
  char* OS_get_dir_pwd () {
//================================================================
/// OS_get_dir_pwd           get current process-working-directory "PWD"
// returns NULL on error.

  static char pwd[200];
  _getcwd (pwd, sizeof(pwd));
  return pwd;

}


//================================================================
  int OS_file_compare_A (char *fn1, char *fn2) {
//================================================================
// OS_file_compare_A                   compare 2 files ascii
// RetCode: 0=files_not_different, 1=different_files; -1,-2=ERROR;
// MS-Win: FC

  int    irc;
  char   s1[280];


  if(!OS_checkFilExist (fn1, 1)) return -1;

  if(!OS_checkFilExist (fn2, 1)) return -2;

  sprintf (s1, "CMD /C \"FC /B \"%s\" \"%s\" 1>NUL\"", fn1, fn2);
    printf(" |%s|\n",s1);
  // returns 0 for identical files; 1 for different files

  irc = OS_system (s1);

  if(irc) irc = 1;
  return irc;

}


//================================================================
  int OS_ck_SW_is_installed (char *ssw) {
//================================================================
// Test if software <ssw> is installed
// returns 0 = yes, is installed,   elso no, not installed.
// Test with "which <ssw>"

  // char    s1[256];
  // sprintf(s1, "***** OS_ck_SW_is_installed - %s not avail. in MS-Win.");

  MSG_pri_1 ("NOEX_fil", "ssw");

  return -1;

}


//================================================================
  int OS_prc_dll (int mode, void *fDat) {
//================================================================
/// \code
/// mode >=0  work (mode = nc-function from NCCmdTab)
/// mode -1 = work (programfunction fDat = "FUNC_xx")
/// mode -2 = open (load Lib fNam)
/// mode -3 = connect (connect Func fDat)
/// mode -4 = unload active lib 
/// \endcode


static HINSTANCE    hdl1=NULL;
// static void  (*up1)();
static int   (*up1)();

  int   irc;
  char  *p1, s1[256];


  printf("OS_prc_dll %d\n",mode);


  //----------------------------------------------------------------
  // mode -2 = open (load Lib fNam)
  if(mode == -2) {

    // load DLL
    hdl1 = LoadLibrary ((char*)fDat);
    if (hdl1 == NULL) {
      TX_Error("OS_prc_dll: cannot open dyn. Lib. |%s|",(char*)fDat);
      return -1;
    }

    // damit Debugger stoppt, nachdem DLL geladen wurde
    // p1 = strrchr(fDat, fnam_del);
    // if(p1 == NULL) p1 = fDat;
    // else ++p1;  // skip fnam_del
    // OS_debug_dll_(p1);


  //----------------------------------------------------------------
  // mode -3 = connect (connect Func fDat)
  } else if(mode == -3) {
      printf(" func fDat = |%s|\n",(char*)fDat);


    // Adresse von Func.fncNam holen
    up1 = (void*) GetProcAddress (hdl1, (char*)fDat);
    if(up1 == NULL) {
      TX_Error("OS_prc_dll: cannot open Func. |%s|",(char*)fDat);
      FreeLibrary (hdl1);           // unload DLL
      hdl1 = NULL;
      return -1;
    }


  //----------------------------------------------------------------
  // mode = -1 = FUNC_xx
  // mode >= 0 = work (call active Func with parameter fDat)
  } else if(mode >= -1) {

    // start userprog
    if(up1) {
      irc = (*up1)(mode, fDat);
      return irc;
    } else  TX_Error ("OS_prc_dll E001");



  //----------------------------------------------------------------
  // mode -4 = unload active lib
  } else if(mode == -4) {

    // close DLL
    if(hdl1) FreeLibrary (hdl1);    // unload DLL
    hdl1 = NULL;
    up1 = NULL;
  }

  return 0;

}


//================================================================
  void OS_get_curDir (char *sd, int sSiz) {
//================================================================
/// OS_get_curDir            get current directory

  // _getcwd (sd, sSiz);
  GetWindowsDirectory (sd, sSiz);

}


//================================================================
  int OS_stdout__ (int mode, void *data) {
//================================================================
// OS_stdout__              direct console-output into file
// mode     0   set filename for console-output
// mode     1   (re-)open file for console-output
//          2   close console-output into file; continue with output to console ..

  static char    fn[80];
  static FILE    *fp = NULL;


  // printf("OS_stdout__ %d |%s|\n",mode,fn);


  if(mode == 0) {
    if(strlen((char*)data) >= 80) {printf("***** OS_stdout__ E1 \n"); return -1;}
    strcpy(fn, (char*)data);


  } else if(mode == 1) {
    // oldfp = stdout;
    // stdout = fopen (fn, "w");
    // fgetpos (stdout, &pos);
    // fd = _dup(fileno(stdout));
    if(fp) fflush (fp);
    fp = freopen (fn, "w", stdout);
    if(!fp) {printf("***** OS_stdout__ E2 \n"); return -1;}


  } else {
    if(fp) fflush (fp);
    // fclose (fp);  // (stdout);
    freopen ("CON", "w", stdout);
    // stdout = fopen ("CON", "wb");
    // stdout = oldfp;
    // dup2(fd, fileo(stdout));
    // fsetpos (stdout, &pos);
  }

  return 0;

}



//================================================================
  char* OS_prc__ () {
//================================================================
/// OS_prc__                 get full filename of active process

 
  GetModuleFileName (0, txbuf, 256);

    printf(" OS_prc__=|%s|\n",txbuf);

  return txbuf;

}


//================================================================
 int OS_file_waitExist (char *fn, int maxTim) {
//================================================================
// wait max <maxTim> secords for existence of file <fn>
// Retcod:  1   OK, file exists ..
//         -1   file does not exist yet ..

  int        ii, i1=0;


  maxTim *= 10;     // sec -> 0.1 sec

  L_nxt:
    ii = GetFileAttributes (fn); // -1=file not found; 16=file, 32=directory
    if(ii >= 0) return 1;
    Sleep (100);                 // 0.1 sec
    ++i1;
    if(i1 < maxTim) goto L_nxt;

  return -1;

}


//================================================================
BOOL CALLBACK OS_hide_winCB (HWND hw1, LPARAM lParam) {
//================================================================
// called by EnumWindows ..

  char cbuf[256];


  GetWindowText (hw1, cbuf, 200);
	if(cbuf[0] == 0) return TRUE;
  
  // printf("OS_hide_winCB |%s|\n",cbuf);

  // keep main-win (starts with "gCAD3D-<version>")
                   //1234567
  // if(!strncmp(cbuf, "gCAD3D-",7)) return 0;
  if(!strcmp(cbuf, "gCAD3D")) {

    printf(" minimize |%s|\n",cbuf);

    // minimize this win ..
    ShowWindow (hw1, SW_SHOWMINIMIZED);

  }

  return TRUE;
}


//===========================================================
  char* OS_get_os__ () {
//===========================================================
// returns "Linux" or "MS" as  string

  sprintf(txbuf, "MS");

  return txbuf;

}


//===========================================================
  char* OS_get_os_bits () {
//===========================================================
// returns "MS32" or "MS64" as  string
// used for "OS=<OS_os>" for dynamic linking


  sprintf(txbuf, "MS%ld",  PTRSIZ * 8);

  return txbuf;

}


//================================================================
  int OS_get_bits () {
//================================================================

  return PTRSIZ * 8;   // 32 || 64            2011-03-18

}


//================================================================
  int OS_hide_win () {
//================================================================

  // DWORD tid;

  printf("OS_hide_win\n");

  // tid = GetCurrentThreadId ();
  // EnumThreadWindows (tid, OS_hide_winCB, 0);  geht ned ..

  EnumWindows (OS_hide_winCB, 0);

  return 0;

}


//================================================================
  int OS_TimeStamp (TimeStamp *ts1) {
//================================================================
/// OS_TimeStamp     get TimeStamp (sec; res = 0.01; ~1 year)

/// static TimeStamp TimeStamp = 0;
/// OS_TimeStamp (&Memspc.ts);  // in UME_create
/// if(Memspc.ts != TimeStamp) {update; TimeStamp = Memspc.ts; }


  double d1, d2;


  SYSTEMTIME  tm1;


  GetSystemTime(&tm1);
  d2 = tm1.wMilliseconds;
  d2 /= 1000.;

  *ts1 = (float)((tm1.wMinute * 60) + tm1.wSecond + d2);

  return 0;

}


//================================================================
  double OS_prc_time_diff (double timStart) {
//================================================================
/// \code
/// OS_prc_time_diff         returns processor-time
/// processor-time does not include usleep ..
/// Example:
///  d1 = OS_prc_start_time();
///  .. 
///  printf(" processor-time used (secs) %lf \n", OS_prc_diff_time(d1));
/// \endcode

  return ((clock() - timStart) / CLOCKS_PER_SEC);

}

//================================================================
  double OS_prc_time_start () {
//================================================================
/// \code
/// OS_prc_time_start        returns processor-time
/// \endcode

  return (clock());

}


//================================================================
  double OS_rTime () {
//================================================================
/// OS_rTime     returns time in sec's, (res = mycrosec's)


  double d1, d2;


  SYSTEMTIME  tm1;


  GetSystemTime(&tm1);
  d2 = tm1.wMilliseconds;
  d2 /= 1000.;

  d1 = (tm1.wMinute * 60) + tm1.wSecond + d2;



/*
  DWORD  l1;
  l1 = timeGetTime(); // nur Mingw32 !?
  d1 = l1;
  d1/= 1000.;
*/

/*
  gettimeofday (&ts1, 0); // liefert sec (seit 1972) und mycrosecunden.
  rt = (double)ts1.tv_sec + ((double)ts1.tv_usec / CLOCKS_PER_SEC);
*/

  // printf ("d1: %f\n",d1);

  return d1;

}


//================================================================
  int OS_get_lang  (char *sLang) {
//================================================================
/// returns system-language; eg "de" or "en"
/// retCod: 0=OK; -1= defaulted to "en"


  int  i1;
  char *p1;
  // char *cmd={"reg query \"HKCU\Control Panel\International\" /v LocaleName"};


  // reg query "HKCU\Control Panel\International" /v LocaleName
  // returns eg "... REG_SZ   de_DE"
  // OS_sys1 (txbuf, sizeof(txbuf), cmd);

  // i1 = GetSystemDefaultUILanguage ();
  // i1 = GetUserDefaultLocaleName (txbuf,240);
  // i1 = GetLocaleInfoA (LOCALE_SYSTEM_DEFAULT, LOCALE_SABBREVLANGNAME,txbuf,240);
  i1 = GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_SABBREVLANGNAME,txbuf,240);
  // i1=GetLocaleInfoA(LOCALE_USER_DEFAULT,LOCALE_SENGLANGUAGE,txbuf,240);//German
    // printf("   W32-OS_get_lang-1 |%s| %d\n",txbuf,i1);


  if(i1) {
    sLang[0] = tolower (txbuf[0]);
    sLang[1] = tolower (txbuf[1]);
    sLang[2] = '\0';
    i1 = 0;
  } else {
    printf("**** ERROR OS_get_lang *****************\n");
    strcpy(sLang, "en");
    i1 = -1;
  }


  L_exit:
    // printf("ex-W32-OS_get_lang |%s|\n",sLang);
  return i1;

}


//================================================================
  char* OS_get_term  () {
//================================================================

  // return "start cmd /s ";

  strcpy(txbuf, "start cmd /s ");
  return txbuf;

}


//================================================================
  char* OS_get_vwr_ps  () {
//================================================================
///  returns ps-viewer (gv|evince)

  strcpy(txbuf, "GSview");

  return txbuf;
}


//================================================================
  char* OS_get_user  () {
//================================================================
///  returns console-program; eg "xterm -e "

  strcpy (txbuf, getenv("USERNAME"));
  return txbuf;
}


//================================================================
  int OS_browse_htm (char *fNam, char *label) {
//================================================================
// html-browse <filnam>[<label>]; do not wait for end of process.
// Input:
//   fNam        full filename of html-file;  NULL = <temp>/temp.htm
//   label        html-label starting with '#'; eg "#L1";  NULL = none
// used by eg Help/Docu

  char  s1[256], s2[400], *p1, *p2;

  p1 = OS_get_browse_htm ();

  //----------------------------------------------------------------
  if(p1 == NULL) {
    sprintf(s2,"** ERROR: cannot find a browser to display file  %s ",fNam);
    // GUI_Dialog (NULL, cbuf);
    TX_Print (s2);
    return -1;
  }


  //----------------------------------------------------------------
  // test if file given; else use <temp>/temp.htm
  if(fNam == NULL) {
    sprintf(s1, "%stmp.htm",AP_get_tmp_dir());

  } else {
    strcpy(s1, fNam);
  }


  //----------------------------------------------------------------
  // test if file exists; if not: change language to "en"
  if(OS_checkFilExist (s1, 1)) goto L_disp;
  TX_Print ("%s - file does not exist", s1);

  // file does not exist; change language to "en"
  // extract langCode (2 chars)
  p2 = strrchr (s1, '.');
  if(!p2) return -1;
  p2 -= 2;
  strncpy (p2, "en", 2);

  // test if file exists;
  if(OS_checkFilExist (s1, 1) == 0) {
    TX_Print ("%s - file does not exist", s1);
    return -1;
  }


  //----------------------------------------------------------------
  // display file fNam with AP_browser
  L_disp:


  if(label) {
    // with label:
    // not for hh:
    // sprintf(s2, "start %s file:%s%s", p1, s1, label);
    sprintf(s2, "CMD /C \"%s file:%s%s\"", p1, s1, label);
  } else {
    // without label:
    // sprintf(s2, "start %s %s", p1, s1);
    sprintf(s2, "CMD /C \"%s %s\"", p1, s1);
  }

  printf("OS_browse_htm |%s|\n",s2);
  OS_system (s2);


  // char  cbuf[256];
  // sprintf(cbuf, "start %s %s",OS_get_browse_htm(),filnam);
  // printf("OS_browse_htm |%s|\n",cbuf);
  // OS_system(cbuf);

  return 0;

}


//================================================================
  char* OS_get_browse_htm  () {
//================================================================
// returns html-browser


  if(OS_browser[0]) return OS_browser;


  // strcpy(OS_browser, "firefox");
  strcpy(OS_browser, "explorer");
  // strcpy(OS_browser, "iexplore");
  // strcpy(OS_browser, "hh");


  return OS_browser;


  //printf(" **** kein HTML-Browser gefunden\n");
  //return NULL;
}




//=================================================================
  void OS_time (long *i1, long *i2, long *i3) {
//================      hour      min       sec


  time_t actt;
  struct tm *Tm;

  actt=time(0);
  Tm=localtime(&actt);
  // cp=asctime(Tm);^M

  *i1=Tm->tm_hour;
  *i2=Tm->tm_min;
  *i3=Tm->tm_sec;

}


//=================================================================
  void OS_date (long *i1, long *i2, long *i3) {
//===============      year     mon       day


  time_t actt;
  struct tm *Tm;

  actt=time(0);
  Tm=localtime(&actt);

  /*
  Tm->tm_sec, Tm->tm_min, Tm->tm_hour, Tm->tm_mday, Tm->tm_mon,
  Tm->tm_year, Tm->tm_wday, Tm->tm_yday, Tm->tm_isdst);
  printf("day =%d\n",Tm->tm_mday);
  */

  *i1=Tm->tm_year;
  *i2=Tm->tm_mon+1;
  *i3=Tm->tm_mday;


 /* Linux: das Jahr 2001 kommt als 101 !!! */
  if(*i1 >= 100) *i1 = *i1 + 1900;



  // printf("OS_date %d %d %d\n",*i1, *i2, *i3);

}


//=============================================================
  char* OS_date1 () {
//=============================================================
// Format: 2002/09/04-13:30:33

  long        y,mo,d,h,mi,s;
  static char cdat[24];


  // strcpy(cdat, "2002/09/04-13:30:33");

  OS_date (&y,&mo,&d);
  OS_time (&h,&mi,&s);
  sprintf(cdat,"%04d/%02d/%02d-%02d:%02d:%02d",y,mo,d,h,mi,s);


  return cdat;

}


//=================================================================
  int OS_Init_ (char *cmd) {
//=================================================================
// Zuerst mal das Basisdir (for os- Routinen) isolieren.
// Im argv[0] steht ca. C:\win_apt\pp\fanuc01.exe
// oder nur xy.exe (wenn per Hand gestartet).
// oder x:\Devel\dev\gCAD3D/gcad3d.exe
// Beispiel also - in main():
// OS_Init_ (argv[0]);


  int    i1;
  char   *p1, *p2;


  printf("OS_Init_ |%s|\n",cmd);

/*
  //----------------------------------------------------------------
  // get os_bas_dir
  // strcpy(os_bas_dir, cmd);
  GetCurrentDirectory(255, os_bas_dir);
  UTX_add_fnam_del (os_bas_dir);
  strcpy(os_loc_dir, os_bas_dir);



  //----------------------------------------------------------------
  // get os_bin_dir
  // which version is running: 32bit or 64bit ?
  // is set at compile-time !
  p1 = getenv("gcad_dir_bin");
    // printf(" env bin = |%s|\n",p1);
  if(p1) {
    if(strlen(p1) > 255) goto L_err2;
    strcpy(os_bin_dir, p1);
    UTX_add_fnam_del (os_bas_dir);

  } else {
    // i1 = PTRSIZ * 8;   // 32 || 64            2011-03-18
    // sprintf (os_bin_dir, "%sbinMS%d%c",os_bas_dir,i1,fnam_del);
    GetCurrentDirectory(255, os_bin_dir);
    UTX_add_fnam_del (os_bin_dir);
  }


  if(!OS_checkFilExist(os_bin_dir, 1)) {
    strcpy(txbuf, "Directory \"gcad_dir_bin\" not found; Installationproblem.\n"
           " Fix gcad_dir_bin in startup-script.");
    GUI_MsgBox (txbuf);
    exit(0);
  }



  //----------------------------------------------------------------
  // os_ico_dir
  p1 = getenv("gcad_dir_ico");
  if(p1) {
    if(strlen(p1) > 255) goto L_err2;
    strcpy(os_ico_dir, p1);
    UTX_add_fnam_del (os_ico_dir);

  } else {
    strcpy(os_ico_dir, os_bin_dir);
    UTX_endDelChar (os_ico_dir, fnam_del);  // remove last char ('/')
    UTX_endDelWord (os_ico_dir, fnam_del, 0); // remove last word (keep '/')
    strcat(os_ico_dir, "icons\\");
  }

  if(!OS_checkFilExist(os_ico_dir, 1)) {
    strcpy(txbuf, "Directory \"os_ico_dir\" not found; Installationproblem.\n"
           " Fix gcad_dir_ico in startup-script.");
    GUI_MsgBox (txbuf);
  }



  //----------------------------------------------------------------
  // get os_loc_dir
  // which version is running: 32bit or 64bit ?
  // is set at compile-time !
  p1 = getenv("gcad_dir_local");
    printf(" env loc = |%s|\n",p1);
  if(p1) {
    if(strlen(p1) > 255) goto L_err2;
    strcpy(os_loc_dir, p1);
    UTX_add_fnam_del (os_loc_dir);

  } else {
    p1 = getenv("APPDATA");
    if(!p1) {
      GUI_MsgBox (" Fix gcad_dir_local in startup-script or set APPDATA.");
      exit(0);
    }
    strcpy(os_loc_dir, p1);
    strcat(os_loc_dir, "\\gCAD3D\\");


    // // remove binMS32 from os_bin_dir
    // strcpy(os_loc_dir, os_bin_dir);
    // i1 = strlen(os_loc_dir) - 1;
    // if(os_loc_dir[i1] == '\\') os_loc_dir[i1] = '\0';
    // p1 = strrchr(os_loc_dir, '\\');
    // if(p1) {
      // ++p1;
      // *p1 = '\0';
    // } else {
      // GUI_MsgBox (" Fix gcad_dir_local in startup-script.");
      // exit(0);
    // }
  }

	  printf(" os_loc_dir=|%s|\n",os_loc_dir);

  if(!OS_checkFilExist(os_loc_dir, 1)) {
    strcpy(txbuf, "Directory \"gcad_dir_local\" not found; Installationproblem.\n"
           " Fix gcad_dir_local in startup-script.");
    GUI_MsgBox (txbuf);
    exit(0);
  }


  sprintf(os_tmp_dir, "%stmp%c",os_loc_dir,fnam_del);




  //----------------------------------------------------------------
  // get os_bas_dir
  strcpy(os_bas_dir, os_loc_dir);



  //----------------------------------------------------------------
  // get os_doc_dir
  p1 = getenv("gcad_dir_doc");
    // printf(" env bin = |%s|\n",p1);
  if(p1) {
    if(strlen(p1) > 255) goto L_err2;
    strcpy(os_doc_dir, p1);
    UTX_add_fnam_del (os_doc_dir);

  } else {
    sprintf (os_doc_dir, "%sdoc%c",os_loc_dir,fnam_del);
  }
*/

  //----------------------------------------------------------------
  // done ..
      printf(" bindir = |%s|\n",AP_get_bin_dir());
      printf(" basdir = |%s|\n",AP_get_bas_dir());
      printf(" docdir = |%s|\n",AP_get_doc_dir());
      printf(" locdir = |%s|\n",AP_get_loc_dir());
      printf(" tmpdir = |%s|\n",AP_get_tmp_dir());
      printf(" cfgdir = |%s|\n",AP_get_cfg_dir());
      printf(" icodir = |%s|\n",AP_get_ico_dir());

    // exit(0);  // TEST ONLY

  L_fertig:
  return 0;


  L_err2:
      strcpy(txbuf, "Error: directoryname too long.\n");
      printf("%s\n",txbuf);
      GUI_MsgBox (txbuf);
      exit(0);

}


//================================================================
  int OS_get_dialog () {
//================================================================
/// check if zenity is installed

// zenity exists for MS but is not used.

  return -1;

}


//================================================================
  char* OS_get_printer  () {
//================================================================
//  strcpy(txbuf, "print");
// Retcod: "Error.." = Error

// fPrint.exe returns the printername
// "copy/b <filename> <printername>"
// UI_WinPrint1

  FILE  *fu1;


  sprintf(txbuf, "\"%sfPrint.exe\"",AP_get_bin_dir());
    printf("|%s|\n", txbuf);

  fu1 = _popen (txbuf, "r");
  if(fu1 == NULL) {
    sprintf(txbuf, "Error fPrint.exe ..\n");
    return txbuf;
  }
  while (1 == 1) {
    if (fgets (txbuf, 250, fu1) == NULL) break;
    if((txbuf[0] != '\\')||(txbuf[1] != '\\')) {
      printf("Error read fPrint ..\n");
      break;
    }
    UTX_CleanCR (txbuf);
      printf("Printer = |%s|\n",txbuf);
    break;
  }
  _pclose (fu1);

  return txbuf;

}

/*
//================================================================
  char* AP_get_tmp_dir () {
//================================================================
  return os_tmp_dir;
}
//================================================================
  char* AP_get_loc_dir () {
//================================================================
  return os_loc_dir;
}
//================================================================
  char* AP_get_cfg_dir () {
//================================================================
  sprintf(txbuf, "%scfg\\", os_loc_dir);
  return txbuf;
}
//================================================================
  char* AP_get_ico_dir () {
//================================================================
  return os_ico_dir;
}
//================================================================
  char* AP_get_bin_dir () {
//================================================================
  return os_bin_dir;
}
//================================================================
  char* AP_get_doc_dir () {
//================================================================
  return os_doc_dir;
}
*/


//================================================================
  int OS_spawn_wait (char *cmd, int iwait) {
//================================================================
// MS_WIN: NOCH NICHT IMPLEMENTIERT !!!!!
// execute command and wait explicit
// cmd should be one word without parameters ...
// iwait = time to wait in secs


  int    i1, i2;
  char   osCmd[256];

  // printf("OS_spawn |%s|\n",cmd);


  // sprintf(osCmd, "%s 1>/dev/null 2>/dev/null",cmd);
    // printf("   |%s|\n",osCmd);
  OS_system (osCmd);

  Sleep (iwait * 1000);

  // sprintf(osCmd, "ps --no-heading -C %s 1>/dev/null 2>/dev/null",cmd);
    // printf("   |%s|\n",osCmd);

  // for(i1=0; i1<iwait; ++i1) {
    // i2 = system (osCmd);
    // // printf(" i2=%d\n",i2);
    // if(i2) return 0;
    // sleep (1);
  // }

  // printf("***** ERROR %s .......... \n",cmd);

  TX_Error("*** OS_spawn_wait ***");
  return -1;

}




/*==========================================================================*/
  void OS_Wait (int millisecs) {
/*====================
*/

  // PC-Version:
  Sleep (millisecs);

}




/*==========================================================================*/
  void OS_beep () {
/*====================
*/

  // PC-Version: Frequenz, Dauer als doubles
  Beep (1000., 200.);

}


/*============================================================= */
  int OS_checkDirExist (char* dirnam) {
/*=======================
if dir dirnam does not exist, try to create it.
mode = 0: display message sofort;
mode = 1: just fix returncode, no message

rc = 0 = Fehler; Dir existiert NICHT.
rc = 1 = Dir sucessfully created.
rc = 2 = Dir did already exist.
*/

  int     i1;
  static char    buf[80];
  static int     mode=1;


  for(i1=0; i1<strlen(dirnam); ++i1) {
    if(dirnam[i1] == '/') dirnam[i1] = '\\';
  }

  // printf("OS_checkDirExist |%s|\n", dirnam);



  if ((OS_checkFilExist (dirnam, 1)) == 0) {
    if (mode == 0) {
      sprintf (buf, " - Try to create %s\n",dirnam);
      printf ("%s\n",buf);
    }

    /* GUI_Dialog (NULL, buf); */
    /* i1 = umask (0); */
    /* mkdir (buf, S_IRWXU|S_IRWXG|S_IRWXO); */

    /* Version UNIX: */
    /* mkdir (dirnam, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH); */

    /* Version PC: braucht direct.h ! */
    _mkdir (dirnam);

    return OS_checkFilExist (dirnam, 0);
/*
    if ((OS_checkFilExist (dirnam, 1)) == 0) {
      if (mode == 0) {
        strcpy (buf, "*** Error  - Dir. ");
        strcat (buf, dirnam);
        strcat (buf, " does not exist");
        //GUI_Dialog (NULL, buf);
        printf ("%s\n",buf);
      }
      return NO;
    }
*/
  }

  // Ja, gibts schon.
  return 2;


}



//=============================================================
  long OS_FilSiz (char* filnam) {
//=============================================================

  int         rc;
  struct _stat fileStatus;


  // printf("OS_FilSiz |%s|\n", filnam);

  rc = _stat (filnam, &fileStatus);

  // printf("OS_FilSiz: rc=%d siz=%d\n",rc,fileStatus.st_size);

  if(rc < 0) {  // File nicht vorh.
    return -1;
  }

  return fileStatus.st_size;

}


/*============================================================= */
  int OS_check_isDir (char* dirnam) {
/*=================================
// braucht:  #include <sys/stat.h> und #include <unistd.h> ?
rc = 1 = OFF = Fehler; Dir ist kein Dir.
rc = 0 = ON  = OK; dirnam ist Dir.
*/

/*
  int         rc;
  struct _stat fileStatus;


  printf("OS_check_isDir |%s|\n", dirnam);


  rc = _stat (dirnam, &fileStatus);

  printf("typ=%d\n",fileStatus.st_mode);
  // printf("typ=%d\n",fileStatus.st_mode&S_IFDIR);


  // if(fileStatus.st_mode & S_IFDIR)
  // FileSize: printf("%d\n", fileStatus.st_size);

  // if(fileStatus.st_mode & 0x0040000) {

  if(fileStatus.st_mode == 16877) {
    // OK - ist Dir
    return 0;
  }

  return 1;
*/



  DWORD  dw1;


  dw1 = GetFileAttributes (dirnam);
  // printf(" att = %d\n",dw1);
  // FFF=no media in drive (keine CD ..)


  if( dw1 == 0xFFFFFFFF ) { 
    printf("Err\n");
    return -1;
  }


  if (!(dw1 & FILE_ATTRIBUTE_DIRECTORY)) {
    // ist file
    return 1;
  }

  // printf(" ist dir\n");
  return 0;

}



//==========================================================================
  int OS_dir_root (char *cbuf, int csiz) {
//==========================================================================
// alle Rootpfade (Laufwerke) zurueckliefern.
// csiz = size of cbuf in byte.
// out in cbuf= "A:\0C:\0F:\00" (0 hier binaeres Null !)



  DWORD  dw1;
  int    i1, i2;
  char   *p1, *p2, auxBuf[256];


  // die Names der log. Drives holen
  dw1=GetLogicalDriveStrings (256, auxBuf);
  printf("|%s| %d\n",cbuf,dw1);

  i1 = 0;
  i2 = 0;
  p1 = auxBuf;
  p2 = cbuf;

  L_next:
  printf(" %d=|%s|\n",i1,p1);

  i2 += strlen(p1);
  if(i2 >= csiz) return -1;
  strcat(p2,p1);
  p2[strlen(p2)-1] = '\0'; // remove foll. "\"
  p2 = &cbuf[i2];


  i1 += strlen(p1) + 1;
  p1 = &auxBuf[i1];
  if(i1<dw1) goto L_next;



  // NUR TEST:
  // strcpy(cbuf,"C:0F:0G:0");cbuf[2]='\0';cbuf[5]='\0';cbuf[8]='\0';
  // NUR TEST:



  return 0;

}


//==========================================================================
  int OS_dir_ck1 (char *p1) {
//==========================================================================
// check ob rootDir beschreibbar ist (=typ of directory)
// irc=0:CD-Rom, 1=Diskette, 2=normal.


  DWORD  dw1;


  // printf("OS_dir_ck1 |%s|\n",p1);



  // A und B (disketten) verlangen immer "Datentraeger einlegen"; skippen
  if(*p1 == 'A') return 1;
  if(*p1 == 'B') return 1;

  // h1 = CreateFile(p1, 0, FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
  // if (h1 != INVALID_HANDLE_VALUE) { printf("err\n"); }



  dw1 = GetFileAttributes (p1);
  // FFF=no media in drive (keine CD ..)
  if( dw1 == 0xFFFFFFFF ) return -1;


  if(GetDriveType(p1) == DRIVE_CDROM) return 0;

  return 2;

}


//==========================================================================
  int OS_dir_ck2 (char *p1) {
//==========================================================================
// check ob rootDir beschreibbar ist
// irc=0:nicht beschreibbar, 1=ja, ist beschreibbar.

  int  irc = 0;


  // printf("OS_dir_ck1 |%s|\n",p1);


  if ((_access (p1, 6)) == 0) {   // 6=read&write permission; 0=yes,OK.
    // printf ("*** Error OS_checkFilExist: %s does not exist\n",filnam);
    irc = 1;
  }

  return irc;

}


//==========================================================================
  int OS_dir_scan_ (char *cbuf, int *iNr) {
//==========================================================================
// search files - return next fileName
// - not sorted; different for UX / MS !
// iNr = 0: init suche; zu scannender Pfad ist cbuf.
// iNr > 0: cbuf ist next found file; do not change iNr! (Filename ohne Path !)
// iNr < 0; kein weiteres File found; directory closed.


  static HANDLE h1 = NULL;
  static WIN32_FIND_DATA findData;
  static char  dir1[256];

  int          irc, i1;
  char         *p1;
  


  // printf("OS_dir_scan_ |%s| %d\n",cbuf,*iNr);
  // if(*iNr > 100) exit(0);


  //----------------------------------------------------------------
  if(*iNr == 0) {
    // init - open
    if(h1 != NULL) FindClose (h1);

    strcpy(dir1, cbuf);
    i1 = strlen(cbuf);
    if(cbuf[i1 - 1] != '\\') strcat(dir1, "\\");  // add "\\"
    strcat(dir1, "*");

    h1 = FindFirstFile (dir1, &findData); // erster Filename

    if (!h1 || h1 == (HANDLE) -1) {
        // printf("nix gfundn\n");
      *iNr = -1;
      FindClose (h1);
      h1 = NULL;
      irc  = -1;
      goto L_exit;
    }

    // den "*" wieder weg !
    dir1[strlen(dir1)-1] = '\0';
    goto L_fertig;

  }


  //----------------------------------------------------------------
  // get next Filename
  L_dir_weiter:

  i1 = FindNextFile (h1, &findData);

  if(i1 == 0) {
    FindClose (h1);
    h1 = NULL;
    *iNr = -1;
    irc  = -1;
    goto L_exit;
  }



  L_found:
  p1 = findData.cFileName;
    // printf(" next file %d |%s|\n",i1,p1);


  // skip the ".."
  if((p1[0] == '.')&&(p1[1] == '.')) goto L_dir_weiter;

  strcpy(cbuf, dir1);
  strcat(cbuf, p1);


  L_fertig:
    *iNr += 1;
    irc  = 0;


  L_exit:
    // printf("ex OS_dir_scan_ %d |%s| %d\n",irc,cbuf,*iNr);
  return 0;

}


//=======================================================================
  int OS_filterff (char *fnamO, char *fnamI, char* ftext) {
//=======================================================================
// gesamte Liste (Datei) sortieren u. filtern;
// ftext = Filtertext
// Die InputDatei muss aber erhalten bleiben !
// RC = -1: kein Inputfile
//      -2: kein outfile

  char cbuf[512];


  // printf("OS_filterff |%s|%s|%s|\n",fnamO,fnamI,ftext);


  if(OS_checkFilExist(fnamI,1) == 0) return -1;


  if(strlen(ftext) < 1) {
    // sprintf(cbuf,"cat %s|sort > %s",fnamI,fnamO);
    sprintf(cbuf,"CMD /C \"TYPE \"%s\"|SORT > \"%s\"\"",fnamI,fnamO);
      // printf("  sys |%s|\n",cbuf);
    OS_system(cbuf);


  } else {
      // find geht im MSYS nicht !
    sprintf(cbuf,"CMD /C \"TYPE \"%s\"|SORT|FIND/I \"%s\" > \"%s\"\"",fnamI,ftext,fnamO);
    //sprintf(cbuf,"type \"%s\"|sort|find \"%s\" > \"%s\"",fnamI,ftext,fnamO);
    //sprintf(cbuf,"find \"%s\" \"%s\" > \"%s\"",ftext,fnamI,fnamO);
      // printf("  sys |%s|\n",cbuf);
    OS_system(cbuf);
  }

  if(OS_checkFilExist(fnamO,1) == 0) return -2;

  return 0;

}


//==========================================================================
  int OS_file_delGrp (char *fNam) {
//==========================================================================
// OS_file_delGrp           Delete files/Wildcard; Single File "remove (fn);"
// del od. remove

  char cbuf[512];

  // printf("OS_file_delGrp |%s|\n",fNam);


  sprintf(cbuf,"CMD /C \"DEL /F /Q \"%s\" 1>NUL 2>NUL\"",fNam);
  // sprintf(cbuf,"ERASE \"%s\"",fNam);
  OS_system(cbuf);
  

  return 0;

}


//================================================================
  int OS_file_readm1 (char *cbuf, int cSiz, char *fnam) {
//================================================================
// read complete file into memory
// Datei fnam in den Buff cbuf einlesen.
// cSiz = size of cbuf
// Retcode:
//   -1    Error
//   >0    size of file

  long   fSiz;
  FILE   *fpi=NULL;


  fSiz = OS_FilSiz (fnam);
    // printf("OS_file_readm1 %d |%s| %d\n",cSiz,fnam,fSiz);

  if((fSiz+4) > cSiz) return -1;


  if((fpi=fopen(fnam,"rb")) == NULL) {
    printf("***** OS_file_readm1 E001 |%s|\n",fnam);
    return -1;
  }

  // gesamtes file als block lesen ..
  fread (cbuf, fSiz, 1, fpi);

  fclose (fpi);

  return fSiz;

}


///===================================================================
  int OS_dll_build (char *dllNam) {
///===================================================================
/// wenn .mak vorh: compile/link

// dllNam   zB "xa_ige_r.so"   (ohne Pfad, mit Filetyp).

  int  irc;
  char *sdir, cbuf[256];         // char cbuf[512];



  printf("OS_dll_build |%s|\n",dllNam);


  // get the directory ${gcad_dir_dev}src/APP
  sdir = getenv ("gcad_dir_dev");
  if(!sdir) {TX_Print("***** cannot find direcory gcad_dir_dev ..."); return -1;}


  // sprintf(cbuf, "%sxa\\%s",AP_get_bas_dir(),dllNam);
  // sprintf(cbuf, "%s..\\src\\APP\\%s",AP_get_loc_dir(),dllNam);
  sprintf(cbuf, "%ssrc\\APP\\%s", sdir, dllNam);


  // ".dll" -> ".nmak"
  strcpy(&cbuf[strlen(cbuf)-4], ".nmak");
    // printf(" exist: |%s|\n",cbuf);
  if(OS_checkFilExist (cbuf, 1) == 0) goto L_err_nof;


  TX_Print(".. compile .. link .. %s",dllNam);


  // sprintf(cbuf, "cd %sxa&&nmake -f %s",AP_get_bas_dir(),dllNam);
  // sprintf(cbuf, "cd %s..\\src\\APP&&nmake -f %s",AP_get_loc_dir(),dllNam);
  sprintf(cbuf, "CMD /C \"cd \"%ssrc\\APP\" && nmake -f %s\"", sdir, dllNam);
    // printf(" OS_dll_build 2 |%s|\n",cbuf);


  // ".dll" -> ".nmak"
  strcpy(&cbuf[strlen(cbuf)-5], ".nmak OS=");
  strcat(cbuf, OS_get_os_bits());
  strcat(cbuf, "\"");
    printf("OS_dll_build-do |%s|\n",cbuf);


  irc = OS_system(cbuf);
  if(irc != 0) TX_Error("Error build %s",dllNam);

  return irc;

  L_err_nof:
    TX_Print("***** %s does not exist ..",cbuf);

    return 0;

}


//================================================================
  int OS_dll_close (void **dl1) {
//================================================================
// on successfule returns irc = 0,  dl1 = NULL
// BUG dlclose: returnCode is OK but handle not NULL.

  int  irc = 0;

  printf("OS_dll_close \n");

  // unload if already loaded
  if(*dl1 != NULL) {
    irc = FreeLibrary (*dl1);    // 0=err
      // printf(" close %d\n",irc);
    if(irc) {
      // success
      *dl1 = NULL;
      irc = 0;
    } else {
      irc = -1;
    }
  }

  return irc;

}


//================================================================
  int OS_dll_unload_idle (void *data) {
//================================================================
// DLL_unload       unload a dll (idle-callback)


  printf("OS_dll_unload_idle |%s|\n",(char*)data);

  // OS_dll_close_fn ((char*)data);
  OS_dll_do ((char*)data, NULL, NULL, 2);

  return 0;   // must be 0 - removes idle-Call

}


//================================================================
  int OS_dll__ (void **dl1, int mode, void *fDat) {
//================================================================
/// \code
/// load dll | start dll-function | unload dll
/// Input:
///   mode     FUNC_LOAD    = load DLL. fDat: dll-name without directory, fTyp.
///            FUNC_CONNECT = connect (connect Func fDat)
///            FUNC_EXEC    = work (call active Func with parameter fDat)
///            FUNC_UNLOAD  = unload active lib
///            FUNC_RECOMPILE = recompile DLL
/// Output:
///   dll      (address of) loaded DLL
///   retCod   0=OK; else error
/// \endcode


  // typedef int (__stdcall *dllFuncTyp01)(void*);
  // static dllFuncTyp01 dll_up1;

//  static HINSTANCE    hdl1=NULL;
  static int          (*dll_up1)();

  int   irc;
  char  s1[256], *p1;


  // printf("OS_dll__ %d\n",mode);


  //----------------------------------------------------------------
  // mode 0 = open (load Lib fNam)
  if((mode == FUNC_LOAD_only)  ||
     (mode == FUNC_LOAD_all))      {

    // unload if already loaded
    if(*dl1 != NULL) {
      FreeLibrary (*dl1);
      *dl1 = NULL;
    }

    // load DLL
    sprintf(s1, "%s%s.dll",AP_get_bin_dir(),(char*)fDat);
      printf(" dll=|%s|\n",s1); fflush(stdout);

    *dl1 = LoadLibrary (s1);
    if (*dl1 == NULL) {
      TX_Error("OS_dll__: cannot open dyn. Lib. |%s|",(char*)fDat);
      return -1;
    }
    dll_up1 = NULL;


  // damit Debugger stoppt, nachdem DLL geladen wurde
  OS_debug_dll_((char*)fDat);



  //----------------------------------------------------------------
  // mode 1 = connect (connect Func fDat)
  } else if(mode == FUNC_CONNECT) {

    // Adresse von Func.fncNam holen
    // dll_up1 = (dllFuncTyp01) GetProcAddress (hdl1, (char*)fDat);
    dll_up1 = (void*) GetProcAddress (*dl1, (char*)fDat);
    if(dll_up1 == NULL) {
      TX_Error("OS_dll__: cannot open Func. |%s|",(char*)fDat);
      FreeLibrary (*dl1);           // unload DLL
      *dl1 = NULL;
      return -1;
    }


  //----------------------------------------------------------------
  // mode 2 = work (call active Func with parameter fDat)
  } else if(mode == FUNC_EXEC) {

    // start userprog
    if(dll_up1 != NULL) {
      return (*dll_up1) (fDat);

    } else  {
      TX_Error ("OS_dll__ E001");
      return -1;
    }


  //----------------------------------------------------------------
  // mode 3 = unload active lib
  } else if(mode == FUNC_UNLOAD) {

    // close DLL
    if(*dl1 != NULL) {
      FreeLibrary (*dl1);           // unload DLL
      *dl1 = NULL;
    }


  //----------------------------------------------------------------
  // 4 = recompile dll
  } else if(mode == FUNC_RECOMPILE) {

    if(*dl1 != NULL) {
      FreeLibrary (*dl1);           // unload DLL
      *dl1 = NULL;
    }
    sprintf(s1, "%s.dll",(char*)fDat);
      printf(" dll=|%s|\n",s1); fflush(stdout);
  
    if(OS_dll_build (s1) != 0) {
       TX_Error("OS_dll__: compile/link %s",s1);
       return -1;
    }


  //----------------------------------------------------------------
  }


  return 0;

}


//====================================================================
  int OS_debug_dll_ (char *dllNam) {
//====================================================================
// wenn im Debug-Modus wird hier gestoppt;
// In .gdbinit  muss stehen:
// break OS_debug_dll_
// wenn Debugger steht, eingeben "watch varnam" und "c"
//
// Zweck: Debugger kann DLL nur ansprechen wenn geladen.

  printf("OS_debug_dll_ |%s|\n",dllNam);

  return 0;

}


//=====================================================================
  int OS_dll_do (char *dllNam, char *fncNam, void *fncDat, int mode) {
//=====================================================================
/// load dll; start function fncNam (fncDat); unload dll.
/// see also UI_DllLst_work
// Input:
//   fncnam  main-entry using datablock
//   fncdat  datablock
//   mode    0=load+start+unload
//           1=load+start               - do not unload (OS_dll_close_fn
//           2=unload


  static HINSTANCE hdl1=NULL;

  char  dlNamAct[256];
  int           (*dll_up1)(char*);


  printf("OS_dll_do |%s|%s| %d\n",dllNam,fncNam,mode);
  fflush (stdout);

  if(mode == 2) { mode = 0; goto L_close; }


  if(hdl1 != NULL) {
    TX_Error("**** ERROR OS_dll_run: core-plugin |%s|  open ..",dllNam);
    return -1;
  }


  // fix DLL-FileName
  sprintf(dlNamAct, "%s%s.dll",AP_get_bin_dir(),dllNam);
    printf(" dll_do-fn |%s|\n",dlNamAct);


  // load DLL
  hdl1 = LoadLibrary (dlNamAct);
  if(hdl1 == NULL) {
    TX_Error("OS_dll_run: cannot open dyn. Lib. %s",dllNam);
    return -1;
  }


  // stop debugger after dll is loaded
  OS_debug_dll_(dllNam);


  // get adress of func. <fncNam>
  dll_up1 = (void*) GetProcAddress (hdl1, fncNam);
  if(dll_up1 == NULL) {
    TX_Error("OS_dll_run: cannot connect to func %s",fncNam);
    return -1;
  }


  // start userprog
  (*dll_up1)(fncDat);  // call Func in Dll
    printf(" foll-dll_do |%s|\n",fncNam);


  // close DLL
  L_close:
  if(mode < 1) OS_dll_close (&hdl1);     // unload DLL

    printf("ex-OS_dll_run\n");

  return 0;

}


/* UNUSED
//================================================================
  int OS_dll_run (char *dllNam, char *fncNam, char *fncDat) {
//================================================================
// see also UI_DllLst_work


  // typedef int (__stdcall *dllFuncTyp02)(char*);
  // dllFuncTyp02  dll_up1;

  HINSTANCE     hdl1=NULL;
  int           (*dll_up1)(char*);

  printf("OS_dll_run |%s|%s|%s|\n",dllNam,fncNam,fncDat);


  // load DLL
  // sprintf(txbuf, "%sxa%c%s",AP_get_bas_dir(),fnam_del,soNam);
  // strcpy(txbuf, soNam);
  // printf("  open |%s|\n",txbuf);
  hdl1 = LoadLibrary (dllNam);
  if(hdl1 == NULL) {
    TX_Error("OS_dll_run: cannot open dyn. Lib. %s",dllNam);
    return -1;
  }


  // Adresse von Func.fncNam holen
  dll_up1 = (void*) GetProcAddress (hdl1, fncNam);
  if(dll_up1 == NULL) {
    TX_Error("OS_dll_run: cannot connect to func %s",fncNam);
    return -1;
  }


  // start userprog
  (*dll_up1) (fncDat);  // call Func in Dll


  // close DLL
  FreeLibrary (hdl1);

  return 0;

}
*/


//================================================================
  int OS_dirAbs_fNam (char *dirOut, char *fNam) {
//================================================================
// OS_dirAbs_dir            get full (absolut) path from filename

  char    *cp, *co;

// Varianten:
// /dir/fn
// ./fn
// ../dir/fn
// dir/fn

/*
man braucht einen Ersatz fuer  getenv("PWD" !!!
sonst wahrscheinl nur \\ statt / ...
*/

  TX_Error("*** OS_dirAbs_fNam ******");


  if(fNam[0] != '/') {
    strcpy(dirOut, getenv("PWD"));
  } else {
    dirOut[0] = '\0';
  }


  if(!strncmp(fNam, "./", 2)) {
    strcat(dirOut, &fNam[1]);
    goto L_9;
  }


  cp = fNam;

  L_8:

  if(!strncmp(cp, "../", 3)) {
    cp += 3;   // skip the "../"
    co = strrchr(dirOut, '/');   // cut off the last dir.
    if(co) {++co; *co = '\0';}
    goto L_8;
  }


  UTX_endAddChar (dirOut, '/'); // add closing "/"
  strcat(dirOut, cp);



  // cut off filename
  L_9:
  cp = strrchr(dirOut, '/');
  if(cp) {++cp; *cp = '\0';}



  UTX_endAddChar (dirOut, '/'); // add closing "/"
    // printf("OS_dirAbs_dir |%s|%s\n",dirOut,fNam);


  return 0;

}


//================================================================
  int OS_file_zip (int mode, char *fnFrom, char *fnTo) {
//================================================================
// mode = 0: compress:   zip -9jq t1.gcaz t1.gcad
// mode = 1: uncompress: unzip -pqq t1.gcaz > t1.gcad

  char   osCmd[400];

  printf("OS_file_zip MS %d |%s|%s|\n",mode,fnFrom,fnTo);

/*
  // check if file ..\bin\zip.exe exists ...
    TX_Print("gzip not installed ........");
    return -1;
  }
*/

  if(mode == 0) {
    sprintf(osCmd, "CMD /C \"%sgzip -9qc \"%s\" > \"%s\"\"",AP_get_bin_dir(),fnFrom,fnTo);
    // sprintf(osCmd, "cd \"%s\"&&gzip -9qc \"%s\" > \"%s\"",os_bas_dir,fnFrom,fnTo);
    // sprintf(osCmd, "zip -9jq %s %s",fnTo,fnFrom);
  } else {
    sprintf(osCmd, "CMD /C \"%sgzip -dqc \"%s\" > \"%s\"\"",AP_get_bin_dir(),fnFrom,fnTo);
    // sprintf(osCmd, "cd \"%s\"&&gzip -dqc \"%s\" > \"%s\"",os_bas_dir,fnFrom,fnTo);
    // sprintf(osCmd, "unzip -pqq %s > %s",fnFrom,fnTo);
  }

    printf("|%s|\n",osCmd);

  return OS_system (osCmd);

}

/* replaced by OS_files_join
//================================================================
  int OS_file_concat (char *fno, char *fn1, char *fn2) {
//================================================================
// concatenate 2 files  (fno = fn1 + fn2)
// fno must be different.
// "copy /Y t1 + t2 t3"
// "type t1 t2 > t3"

  char  s1[600];

  printf("OS_file_concat |%s|%s|%s|\n",fno,fn1,fn2);


  // check if fn1, fn2 exists
  if(OS_checkFilExist(fn1, 1) == 0) {
    // TX_Print("OS_file_concat: %s does not exist",fn1);
    MSG_pri_1 ("NOEX_fil", "%s", fn1);
    return -1;
  }
  if(OS_checkFilExist(fn2, 1) == 0) {
    // TX_Print("OS_file_concat: %s does not exist",fn2);
    MSG_pri_1 ("NOEX_fil", "%s", fn2);
    return -1;
  }

  sprintf(s1, "copy /Y %s + %s %s",fn1,fn2,fno);
    // printf(" |%s|\n",s1);
  system (s1);

  return 0;

}
*/

//================================================================
  int OS_files_join (char *fno, char *fn1, char *fn2, ...) {
//================================================================
// OS_files_join  concatenate 2 or more files
// Input:
//   fn1, fn2, ...    2 or more filenames; last parameter must be NULL
// Output:
//   fno              outfile (all files joined);  filename must be different.

  char    fni[1024], s1[1200], s2[400], *p1;
  va_list va;


  // printf("OS_files_join |%s|%s|%s|\n",fno,fn1,fn2);

  sprintf(fni, "\"%s\" + \"%s\"",fn1, fn2);

  // check if fn1, fn2 exists
  if(OS_checkFilExist(fn1, 1) == 0) {
    // TX_Print("OS_file_concat: %s does not exist",fn1);
    MSG_pri_1 ("NOEX_fil", "%s", fn1);
    return -1;
  }

  if(OS_checkFilExist(fn2, 1) == 0) {
    // TX_Print("OS_file_concat: %s does not exist",fn2);
    MSG_pri_1 ("NOEX_fil", "%s", fn2);
    return -1;
  }

  sprintf(fni,"\"%s\" + \"%s\" ",fn1, fn2);
    // printf("  _files_join-1 |%s|\n",fn1);


  // get fni = list of files
  va_start(va, fn2);
  L_nxt_arg:
    p1 = (void*)va_arg (va, char*);
    if(p1) {
      // check if p1 exists
      if(OS_checkFilExist(p1, 1) == 0) {
        MSG_pri_1 ("NOEX_fil", "%s", p1);
        return -1;
      }
      sprintf(s2, "+ \"%s\" ",p1);
      if((strlen(fni) + strlen(s2)) >= sizeof(fni)) goto L_err1;
      strcat(fni, s2);
      goto L_nxt_arg;
    }
    va_end(va);
      // printf(" _files_join-2 |%s|\n",fni);

  // sprintf(s1, "cat %s > %s",fni,fno);
  if((strlen(fni) + strlen(fno) + 16) >= sizeof(s1)) goto L_err1;
  sprintf(s1, "CMD /C \"COPY /Y %s \"%s\" > nul\"",fni,fno);
  //sprintf(s1, "CMD /C \"COPY /Y %s\"%s\"\"",fni,fno);
    // printf(" _files_join-3 |%s|\n",s1);
  OS_system (s1);

  return 0;

  L_err1:
    TX_Error("OS_files_join overflow");
    return -1;

}


///================================================================
  char* OS_get_imgConv1  () {
///================================================================
/// returns jpg2bmp-converter-program.

  //static char fn1[] = "djpeg";
  //static int iStat = 0;          // 0=notYetTested; 1=OK; -1=NotOk.
  //return fn1;

  // sprintf(txbuf,"cd %s && djpeg.exe",AP_get_bin_dir());
  // sprintf(txbuf,"cd \"%s\"&&djpeg",os_bin_dir);
  //sprintf(txbuf,"%sdjpeg",os_bin_dir);
  // Problem Win7: blank im Pfadnamen !!
  // sprintf(txbuf,"cd/d \"%s \" && djpeg.exe",AP_get_bin_dir());
  sprintf(txbuf,"\"%sdjpeg.exe\"",AP_get_bin_dir());

  return txbuf;

}


///================================================================
  char* OS_get_imgConv2  () {
///================================================================
/// returns bmp2jpg-converter-program; eg /usr/bin/cjpeg

  //static char fn1[] = "djpeg";
  //static int iStat = 0;          // 0=notYetTested; 1=OK; -1=NotOk.
  //return fn1;

  // sprintf(txbuf,"cd \"%s\"&&djpeg",os_bin_dir);
  //sprintf(txbuf,"%sdjpeg",os_bin_dir);
  // Problem Win7: blank im Pfadnamen !!
  // sprintf(txbuf,"cd/d \"%s \" && cjpeg.exe",AP_get_bin_dir());
  sprintf(txbuf,"\"%scjpeg.exe\"",AP_get_bin_dir());

  return txbuf;

}


//================================================================
  int OS_jpg_bmp (char *fn_jpg, char *fn_bmp) {
//================================================================
// convert BMP -> JPG

  char  s1[400];

  printf("OS_jpg_bmp |%s|%s|\n",fn_jpg,fn_bmp);

  // sprintf(s1, "%s \"%s\" > \"%s\"",OS_get_imgConv2(),fn_bmp,fn_jpg);
  // sprintf(s1, "%s \"%s > %s\"",OS_get_imgConv2(),fn_bmp,fn_jpg);
  sprintf(s1, "CMD /C \"%s \"%s\" > \"%s\"\"",OS_get_imgConv2(),fn_bmp,fn_jpg);
    printf(" |%s|\n",s1);

  return OS_system(s1);

}


//================================================================
  int OS_CkPrcIsActive (char *prcNam) {
//================================================================
// OS_CkPrcIsActive         check if process with name prcNam is active
// Retcode:
//    0     process is active
//    1     process is not active

// ps -C mpg123 --no-heading -o pid

  printf("***** ERROR - NOT YET IMPLEMENTED: OS_CkPrcIsActive \n");

  return -1;

}

///================================================================
  int OS_ckFileWritable (char *fnam) {
///================================================================
/// OS_checkFileWritable     check if file is writeable;
/// 0=no, 1=yes.

  printf("***** ERROR - NOT YET IMPLEMENTED: OS_ckFileWritable \n");

  return -1;

}


//================================================================
  int OS_dev_ck () {
//================================================================
// test if compiler & Linker exists; 0=yes, -1=no
// compiler: "cl"     make: "nmake".

  int   iStat;
  FILE  *fu1;


/* liefert keinen Returncode ..
  strcpy(txbuf, "cl");
  fu1 = _popen (txbuf, "r");
  if(fu1 == NULL) goto L_notOK;
  while (1 == 1) {
    if (fgets (txbuf, 250, fu1) == NULL) break;
  }
  _pclose (fu1);
*/


  iStat = OS_system("CMD /C \"cl/?>null\"");
    printf(" cl %d\n",iStat);
  if(iStat != 0) goto L_notOK;

  iStat = OS_system("CMD /C \"nmake/?>null\"");
    printf(" nmake %d\n",iStat);
  if(iStat != 0) goto L_notOK;


  L_exit:
    printf("ex-OS_dev_ck %d\n",iStat);
  return iStat;


  L_notOK:
  iStat = -1;
  GUI_MsgBox (NULL, " Cannot find compiler and make. Start from console-window;\
 use vcvarsall.bat. ");
  goto L_exit;

}


//================================================================
  int OS_timA_now (char *sts1) {
//================================================================
// get timestamp-string "now";  |2017-10-14 10:32:07|
//   size of sts1 >= 32

  long   i1;
  time_t tim1;        // timestamp
  struct tm *ts1;      // time-structure


  // get tim1 = timestamps "now"
  tim1 = time (0);

  // get time-structure ts1 from timestamp tim1
  ts1 = localtime (&tim1);

  // get sts1 = timestamp-string sts1 from time-structure ts1
  strftime (sts1, 32, "%Y-%m-%d %H:%M:%S", ts1);
    printf ("  ex OS_timA_now |%s|\n",sts1);

  return 0;

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


/*
//================================================================
  int OS_file_sig_get (char *fnam) {
//================================================================
/// \code
/// OS_file_sig_get          returns content of file as integer
/// RetCod:        -1 = Error, else c9ontent of first line of file
/// \endcode

  int    i1;
  char   s1[40];
  FILE   *fpi;


  if ((fpi = fopen (fnam, "r")) == NULL) {
    return -1;
  }

  if(fgets (s1, sizeof(s1), fpi) == NULL) {
    fclose(fpi);
    return -1;
  }

  i1 = atoi (s1);
  fclose(fpi);

    // printf("ex OS_file_sig_get %d |%s|\n",i1,fnam);

  return i1;
}


//================================================================
  int OS_file_sig_cre (char *fnam, int iSig) {
//================================================================
/// \code
/// OS_file_sig_cre          create signal-file
/// Input:
///   fnam      if(fnam) then clear (delete) this file; keep fnam static.
///             else write <iSig> into this file
/// \endcode


  static char *fn;
  FILE        *fpo;


  // printf("OS_file_sig_cre |%s| %d\n",fnam,iSig);

  if(fnam) {
    fn = fnam;
    // delete file
    OS_file_delete (fn);
    return 0;
  }

  //  write <iSig> into file fn
  if((fpo=fopen(fn,"w")) == NULL) {
    TX_Print("OS_file_sig_cre E001 %s",fn);
    return -1;
  }

  fprintf(fpo,"%d",iSig);
  fclose(fpo);


  return 0;

}
*/


//================================================================
  int OS_err1 (char *fNam) {
//================================================================
/// \code
/// OS_err1                  print errno
/// Example:
/// OS_err1 (__func__);
/// \endcode

  perror (fNam);

  return 0;

}


//================================================================
  int OS_get_GUI () {
//================================================================
// OS_get_GUI                       check if GUI (file GUI_dlg1..) exists
// retCod        0=OK, -1= Error, no GUI


  int      irc, vGtk;
  char     sEnam[256], sGui[32];

  // get gtk-major-version
  GUI_get_version (sGui, &vGtk, &irc);

  // sEnam = exeFilename
  sprintf(sEnam,"%sGUI_dlg1_%s%d_MS.exe", AP_get_bin_dir(), sGui, vGtk);
    printf(" OS_get_GUI |%s|\n",sEnam);

  // test if exe exists
  if(!OS_checkFilExist(sEnam,1)) {
    TX_Print("**** file %s does not exist ..", sEnam);
    return -1;
  }

  return 0;

}


//========================= EOF ====================================
