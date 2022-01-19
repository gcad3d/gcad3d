// ut_os_aix.c 
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
2001-11-06 OSD_date: Jahr von 101 _> 2001. RF.
2001-09-19 OS_checkDirExist - RC = 2 zu. RF.
2001-04-24 OS_check_isDir neu zu.

-----------------------------------------------------
*/
#ifdef globTag
void OS(){}
#endif
/*!
\file  ../ut/ut_os_aix.c
\brief opsys functions for unix (AIX, Linux)
\code
Needs:
#include "../ut/ut_os.h"               // OS_ ..
-ldl (unix)
=====================================================
List_functions_start:

OS_Init_ (argv[0]);      save basedir

OS_get_os_bits                  returns "Linux" or "MS"
OS_get_bits               returns 32 or 64
// OS_id                    Read System - ID (uname -m) (HW-ID!)

OS_date                  Datum (als iyear, imon, iday) holen
OS_date1                 Format: 2002/09/04-13:30:33
OS_time                  Zeit (ihour, imin, isec) holen
OS_timA_now              get timestamp-string "now"; |2017-10-14 10:32:07|
OS_timA_diff_s           get difftime in seconds of two timestrings
OS_tim_timA              get timestamp from time-string
OS_rTime                 returns time in sec's, (res = mycrosec's)
OS_prc_time_start        returns processor-time
OS_prc_time_diff         returns processor-time
OS_TimeStamp             get TimeStamp (sec; res = 0.01; ~1 year)
OS_date_cmp              compare 2 dates
OS_Wait                  in millisecs

OS_beep
OS_system                Perform OS-Command; wait for completion (system)
OS_sys1                  get systemCommand (popen); skip if starting with "##"
OS_exec                  Perform OS-Command; do not wait for completion.
OS_spawn_wait            execute command and wait explicit
OS_edit_                 edit File (OS_get_edi, OS_system)
OS_browse_htm               display file in HTML-browser
OS_err1                  print errno

OS_check_isDir           is this dir (or file ..)
OS_checkFilExist         check if File or Directory exists
OS_checkFileWritable     check if file is writeable;
OS_checkDirExist         und try to create dir
OS_CkPrcIsActive         check if process with name prcNam is active
OS_ck_SW_is_installed    Test if software <ssw> is installed

OS_prc__                 get full filename of active process
OS_get_scrRes            get total screensize
OS_get_curDir            get current directory
OS_get_lang              getenv("LANG")
OS_get_edi               liefert bei Linux "gedit"
OS_get_printer           get 1. word of /etc/printcap
OS_get_browse_htm           liefert konqueror/mozilla/netscape
OS_get_term              liefert bei Linux "xterm "
OS_get_imgConv1          returns jpg2bmp-converter; eg /usr/bin/djpeg
OS_get_imgConv2          returns bmp2jpg-converter-program; eg /usr/bin/cjpeg
OS_jpg_bmp               convert BMP -> JPG

OS_get_bas_dir
OS_get_tmp_dir
OS_get_loc_dir
OS_get_cfg_dir
OS_get_ico_dir
OS_get_bin_dir
OS_get_doc_dir
OS_get_dir_pwd           get current process-working-directory "PWD"

OS_ck_DirAbs             check if string is absoluter or relativer Filname
OS_dirAbs_fNam           get full (absolut) path from filename
OS_dir_scan_             scan (list) directory (see UTX_dir_listf)
OS_dir_root              Liste der untersten Rootverzeichnisse anlegen
OS_dir_ck1               check ob rootDir beschreibbar ist

OS_stdout__              direct console-output into file

OS_osVar_eval            expand shell variables in string
OS_filnam_eval    DO-NOT-USE  - replaced by OS_osVar_eval
OS_file_copy             copy file
OS_file_rename           rename File; keine Wildcards !
OS_file_delete           delete File; keine Wildcards !
OS_file_delGrp           Delete files/Wildcard; Single File "remove (fn);"
                  Rename files via "rename (oldNam, newNam);"
OS_file_waitExist        wait max <maxTim> secords for existence of file <fn>
OS_file_readm1           read complete file into memory
OS_file_zip              compress/uncompress file
OS_file_concat           concatenate 2 files  (fno = fn1 + fn2)
OS_file_compare_A        compare 2 files ascii
OS_file_date_m           get last modification-date of file
OS_FilSiz                query filesize
OS_filterff              sort & filter file

OS_dll_do                load dll, start function, unload dll
OS_dll__                 load dll| start function| unload dll
OS_dll_build             (re)build dll
OS_dll_close             close dll
// OS_dll_close_fn          close dll from libraryName  
OS_dll_unload_idle       unload a dll (idle-callback)

List_functions_end:
=====================================================
see also:
../ut/os_uix.c       basic OS-functions
../ut/ut_os__.c      OS-independant functions

UNUSED:

\endcode *//*----------------------------------------

// OS_get_dialog          replaced by OS_get_GUI



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



Alte Muster:
 cdapt; vi wincut_ut_os.cpp

cc -c ut_os_aix.c -ansi -pedantic -Wall


_______________________________________________________________________________
*/



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <dlfcn.h>           // Unix: dlopen

#include <stdarg.h>          // f ...
#include <dirent.h>        // f. DIR ..
#include <sys/utsname.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>          ///f. access, R_OK .. 
#include <sys/types.h>       // f. S_ISDIR
#include <sys/stat.h>        // f. mkdir , stat
#include <wordexp.h>         // OS_filnam_eval

/* #include <fcntl.h> */



#include "../ut/ut_txt.h"         // fnam_del_s

#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/func_types.h"             // FUNC_Pan FUNC_Rot FUNC_LOAD ..


#define PTRSIZ             sizeof(void*)   // 4 || 8



#define LOCAL_DEF_VAR          // damit wird "extern" im Includefile geloescht !
#include "../ut/ut_os.h" 
#undef LOCAL_DEF_VAR           // reset extern ..





//_____________________________________
// exported VARS:


//_____________________________________
// static VARS:
// wird von div Funktionen retourniert !
static char txbuf[256];


//_____________________________________
// EXTERNALS:

// errno.h:
extern int errno;


// aus xa.c:




//================================================================
  int OS_stdout__ (int mode, char *fn) {
//================================================================
/// OS_stdout__              direct console-output into file
/// mode     0 open file for console-output
///          1 reconnect stdout

  static FILE    *fp;


  // printf("OS_stdout__ %d |%s|\n",mode,fn);


  if(mode == 0) {
    fp = freopen (fn, "w", stdout);
    if(!fp) return -1;


  } else {
    fflush (fp);
    stdout = fdopen (2, "w");

      printf("ex OS_stdout__-1\n");


  }

  return 0;

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
    // ii = GetFileAttributes (fn); // -1=file not found; 16=file, 32=directory
    ii = access (fn, R_OK);  // 0=exists
    if(!ii) return 1;
    // Sleep (100);                 // 0.1 sec
    usleep (100000);                 // 0.1 sec
    ++i1;
    if(i1 < maxTim) goto L_nxt;

  return -1;

}


//================================================================
  int OS_config () {
//================================================================
// Linux only
// set VER_LIBC_MIN to the version actual existing on the linking computer !

#ifdef _LP64
  // Linux64
  // 2011-07-13 2/11;
# define VER_LIBC_MAJ 2
# define VER_LIBC_MIN 11

  // 2011-07-13 2/16;
# define VER_GTK_MAJ 2
# define VER_GTK_MIN 16


#else
  // Linux32
  // 2011-07-13 2/9;
# define VER_LIBC_MAJ 2
# define VER_LIBC_MIN 9
  
  // 2011-07-13 2/16;
# define VER_GTK_MAJ 2
# define VER_GTK_MIN 16

#endif


  char    s1[256];


  printf("%s\n",OS_get_os_bits());  // 32/64-bit


  // Linux-Version: /proc/version or "uname -a"
  system("uname -a");
  if(OS_checkFilExist("/proc/version", 1)) {
    system("cat /proc/version");
  }


  // libc-Version:
  // Linux32: 2011-07-13 2/9;
  if(OS_ck_libcVersion (VER_LIBC_MAJ, VER_LIBC_MIN) < 0) {
    sprintf(s1,"OS_config ERROR libcVersion; libc %d.%d or newer necessary.",
            VER_LIBC_MAJ, VER_LIBC_MIN);
    printf("%s\n",s1);
    GUI_MsgBox (s1);
  }


  // gtk-Version
  // Linux32: 2011-07-13  2/16.
  if(GUI_ck_version (VER_GTK_MAJ, VER_GTK_MIN) < 0) {
    sprintf(s1,"OS_config ERROR gtkVersion; GTK %d.%d or newer necessary.",
            VER_GTK_MAJ, VER_GTK_MIN);
    printf("%s\n",s1);
    GUI_MsgBox (s1);
  }



  return 0;

}


//================================================================
  char* OS_prc__ () {
//================================================================
/// OS_prc__                 get full filename of active process

  char   s1[256];

  // readlink ("/proc/self/exe", os_bas_dir, 250);
  sprintf(s1, "/proc/%d/exe", getpid());    // LINUX ONLY !
    // printf(" s2,1=|%s|\n",s2);
  readlink (s1, txbuf, 256);  // expand processName -> filename

    printf(" OS_prc__=|%s|\n",txbuf);

  return txbuf;

}

 
//================================================================
  int OS_ck_libcVersion (int vMaj, int vMin) {
//================================================================
/// \code
/// test if new software with Lib-version <versionLinked> can be used.
/// <versionLinked> must be equal or less then the active Lib-version.
/// View the active GNU-Lib-version in a shell with "/lib/libc.so.6"
/// Mandriva 2009 reports "glibc 2.8"
/// Input:
///   versionLinked  of the software at the time of binding
/// RetCod:  0       OK; active-libc-version is >= <versionLinked>
///         -1       Error; active-libc-version is < <versionLinked>
/// \endcode

  // static float versionLinked = 2.8;

  char   cBuf[256], *p1;
  size_t i1;
  int    iv1, iv2;


  // printf("OS_ck_libcVersion %d %d\n", vMaj, vMin);


  //  GNU C library only: get GNU C library version
  i1 = confstr (_CS_GNU_LIBC_VERSION, NULL, 0);
  if(i1 > 256) i1 = 256;
  confstr (_CS_GNU_LIBC_VERSION, cBuf, i1);
    printf("GNU_LIBC_VERSION = |%s|\n", cBuf);
  p1 = strchr (cBuf, ' ');
  if(!p1) return -1;
  iv1 = atoi (p1);
  p1 = strchr (cBuf, '.');
  if(!p1) return -1;
  ++p1;
  iv2 = atoi (p1);
    // printf(" _libcVersion %d %d\n", iv1, iv2);


  if(iv1 < vMaj) return -1;

  if(iv1 == vMaj) {
    if(iv2 < vMin) {
      return -1;
    }
  }

  return 0;

}


//================================================================
  int OS_ck_SW_is_installed (char *ssw) {
//================================================================
// Test if software <ssw> is installed
// returns 0 = yes, is installed,   elso no, not installed.
// Test with "which <ssw>"

  char    s1[256];

  sprintf(s1, "which %s  1>/dev/null 2>/dev/null", ssw);
    printf("OS_ck_SW_is_installed |%s|\n",s1);

  return system (s1);

}


//================================================================
  int OS_CkPrcIsActive (char *prcNam) {
//================================================================
/// \code
/// OS_CkPrcIsActive         check if process with name prcNam is active
/// Retcode:
///    0     process is active
///    1     process is not active
/// \endcode

// ps -C mpg123 --no-heading -o pid


  FILE *fPip1;
  char cBuf1[256];


  sprintf(cBuf1, "ps -C %s --no-heading -o pid", prcNam);
    printf(" OS_ckActive__ |%s|\n",cBuf1);

// #ifdef HPUX
  // fp = popen("ps -ef|awk '{print $2,$8}'", "r");
  // printf("OS_ck_procNr (HP) %s\n",PrcNam);
// #else
  // fp = popen("ps -a -o pid,args", "r");
  // printf("OS_ck_procNr (nicht-HP) %s\n",PrcNam);
// #endif


  // Pipe via stdout oeffnen und in Datei umleiten
  fPip1 = popen (cBuf1, "r");

  if (fPip1 == NULL) {
    printf("***** jb_check_active E001\n");
    return 0;
  }

  cBuf1[0] = '\0';

  while (1 == 1) {
    if (fgets (cBuf1, 256, fPip1) == NULL) break;
    // printf("  jb_check_active |%s|\n",cBuf1);
  }

  pclose (fPip1);

  if(strlen(cBuf1) > 2) return 0;

  // buffer empty: not active.
  return 1;

}


//=================================================================
  int OS_Init_ (char *cmd) {
//=================================================================
// new Version 2010-03-27
// OS_Init_ (argv[0]);
// Output: os_bas_dir    basic directory    with closing "/"

// Das Basisdir (fuer os- Routinen) isolieren. HAT "/" am Ende !!
// Input argv[0] steht ca. C:\win_apt\pp\fanuc01.exe
// or "xy.exe"
// or "./xy.exe"
// or "../xy.exe"
// or "tmp/xy.exe"

  int     irc;
  char    *p1, s1[512], s2[64];


/*
#ifdef _LP64
    strcpy (os_bin_dir, "binLinux64");
#else
    strcpy (os_bin_dir, "binLinux32");
#endif
*/

  printf("OS_Init_ |%s|\n",cmd);



  //----------------------------------------------------------------
  // get dialog-software (check if zenity is installed)
  // OS_get_dialog ();
  // p1 = OS_get_zenity ();
  // if(!p1) {
  irc = OS_get_GUI ();
  if(irc) {
    // abort; GUI_dlg1 - exe not found / installed !
    sprintf(s1, "******* ERROR: GUI_dlg1 not installed *****");
    fprintf(stdout, "%s\n",s1);
    fprintf(stderr, "%s\n",s1);
    exit(2);
  }


/*
  //----------------------------------------------------------------
  // os_bin_dir
  p1 = getenv("gcad_dir_bin");
  if(p1) {
    if(strlen(p1) > 255) goto L_err2;
    strcpy(os_bin_dir, p1);
    UTX_add_fnam_del (os_bin_dir);

  } else {
    // get exename with full path.
    // readlink ("/proc/self/exe", os_bas_dir, 250);
    sprintf(s2, "/proc/%d/exe", getpid());    // LINUX ONLY !
      // printf(" s2,1=|%s|\n",s2);
    readlink (s2, os_bin_dir, 250);  // expand processName -> filename

    // remove exename.  find last "/"
    p1 = strrchr(os_bin_dir, '/');
    if(p1 == NULL) {
      strcpy(txbuf, "Directory \"gcad_dir_bin\" not found; Installationproblem.\n"
             " Fix gcad_dir_bin in startup-script.");
      GUI_MsgBox (txbuf);
      exit(0);
    }
    ++p1;          // keep '/'
    *p1 = '\0';    // overwrite exename.
  }


  //----------------------------------------------------------------
  // os_ico_dir
  p1 = getenv("gcad_dir_ico");
  if(p1) {
    if(strlen(p1) > 255) goto L_err2;
    strcpy(os_ico_dir, p1);
    UTX_add_fnam_del (os_ico_dir);

  } else {
#ifdef _MSC_VER
    strcpy(os_ico_dir, os_bin_dir);
    UTX_endDelChar (os_ico_dir, fnam_del);  // remove last char ('/')
    UTX_endDelWord (os_ico_dir, fnam_del, 0); // remove last word (keep '/')
    strcat(os_ico_dir, "icons/");
#else
    strcpy(os_ico_dir, "/usr/share/gcad3d/icons/");
#endif
  }

  if(!OS_checkFilExist(os_ico_dir, 1)) {
    strcpy(txbuf, "Directory \"os_ico_dir\" not found; Installationproblem.\n"
           " Fix gcad_dir_ico in startup-script.");
    GUI_MsgBox (txbuf);
  }



  //----------------------------------------------------------------
  // get os_bas_dir  (examples.gz,icons/)
  // get "/usr/share/gcad3d/"
  p1 = getenv("gcad_dir_bas");
  if(p1) {
    if(strlen(p1) > 255) goto L_err2;
    strcpy(os_bas_dir, p1);
    UTX_add_fnam_del (os_bas_dir);

  } else {
    strcpy(os_bas_dir, "/usr/share/gcad3d/");
  }

  if(!OS_checkFilExist(os_bas_dir, 1)) {
    strcpy(txbuf, "Directory \"gcad_dir_bas\" not found; Installationproblem.\n"
           " Fix gcad_dir_bas in startup-script.");
    GUI_MsgBox (txbuf);
    exit(0);
  }



  //----------------------------------------------------------------
  // get os_doc_dir  (html, msg)
  p1 = getenv("gcad_dir_doc");
  if(p1) {
    if(strlen(p1) > 255) goto L_err2;
    strcpy(os_doc_dir, p1);
    UTX_add_fnam_del (os_doc_dir);

  } else {
    strcpy(os_doc_dir, "/usr/share/doc/gcad3d/");
  }

  if(!OS_checkFilExist(os_doc_dir, 1)) {
    strcpy(txbuf, "Directory \"gcad_dir_doc\" not found; Installationproblem.\n"
           " Fix gcad_dir_doc in startup-script.");
    GUI_MsgBox (txbuf);
  }


  //----------------------------------------------------------------
  // get os_loc_dir from get gcad_dir_local or $HOME   ("/home/fwork/gCAD3D/")
  // get os_tmp_dir from os_loc_dir
  p1 = getenv("gcad_dir_local");
  if(!p1) {
    p1 = getenv("HOME");
    if(!p1) {
      strcpy(txbuf, "Cannot locate $HOME for a temporary directory.\n"
             " Fix gcad_dir_local in startup-script.");
      GUI_MsgBox (txbuf);
      exit(0);
    }
  }
  if(strlen(p1) > 255) goto L_err2;


  strcpy(os_loc_dir, p1);
  UTX_add_fnam_del (os_loc_dir);   // add closing "/"
  strcat(os_loc_dir, "gCAD3D");
  UTX_add_fnam_del (os_loc_dir);   // add closing "/"

  sprintf(os_tmp_dir, "%stmp%c",OS_get_loc_dir(),fnam_del);





  //----------------------------------------------------------------
  // get os_bas_dir

  p1 = getenv("gcad_dir_bas");
  if(p1) {
    if(strlen(p1) > 255) goto L_err2;
    strcpy(os_bas_dir, p1);

  } else {
    // get exename with full path.
    // readlink ("/proc/self/exe", os_bas_dir, 250);
    sprintf(s2, "/proc/%d/exe", getpid());
    readlink (s2, os_bas_dir, 250);

    // remove exename.  find last "/"
    p1 = strrchr(os_bas_dir, '/');
    if(p1 == NULL) goto L_err1;
    *p1 = '\0';    // overwrite last '/'

    // remove "binLinux32/"  find last "/".          2010-05-23
    p1 = strrchr(os_bas_dir, '/');
    if(p1 == NULL) goto L_err1;
    ++p1;
    *p1 = '\0';

    // // unused
    // getcwd(os_bas_dir, 250);
  }
  UTX_add_fnam_del (os_bas_dir);

  //----------------------------------------------------------------
  // get os_bin_dir
  p1 = getenv("gcad_dir_bin");
  if(p1) {
    if(strlen(p1) > 255) goto L_err2;
    strcpy(os_bin_dir, p1);
    UTX_add_fnam_del (os_bin_dir);
  } else {
#ifdef _LP64
    sprintf (os_bin_dir, "%sbinLinux64%c",os_bas_dir,fnam_del);
#else
    sprintf (os_bin_dir, "%sbinLinux32%c",os_bas_dir,fnam_del);
#endif
  }

  //----------------------------------------------------------------
  // get os_doc_dir
  p1 = getenv("gcad_dir_doc");
  if(p1) {
    if(strlen(p1) > 255) goto L_err2;
    strcpy(os_doc_dir, p1);
    UTX_add_fnam_del (os_doc_dir);
  } else {
    sprintf (os_doc_dir, "%sdoc%c",os_bas_dir,fnam_del);
  }

  //----------------------------------------------------------------
  Fertig:
    // fix temp-dir
    // if environment-variable gcad_dir_tmp exists -
    p1 = getenv("gcad_dir_tmp");
    if(p1) {
      if(strlen(p1) > 255) goto L_err2;
      strcpy(os_tmp_dir, p1);
      UTX_add_fnam_del (os_tmp_dir);
    } else {
      sprintf(os_tmp_dir,"%stmp%c",os_bas_dir,fnam_del);
    }

*/

  //----------------------------------------------------------------
    // done ..
      printf(" bindir = |%s|\n",OS_get_bin_dir());
      printf(" basdir = |%s|\n",OS_get_bas_dir());
      printf(" docdir = |%s|\n",OS_get_doc_dir());
      printf(" locdir = |%s|\n",OS_get_loc_dir());
      printf(" tmpdir = |%s|\n",OS_get_tmp_dir());
      printf(" cfgdir = |%s|\n",OS_get_cfg_dir());
      printf(" icodir = |%s|\n",OS_get_ico_dir());

      // exit(0);  // TEST ONLY

    return 0;

/*
  L_err1:
      printf("Error decode basedirecory; try current working directory (cwd)\n");
    getcwd(os_bas_dir, 250);
    strcat(os_bas_dir, "/");
    goto Fertig;
*/

  L_err2:
      strcpy(txbuf, "Error: directoryname too long.\n");
      printf("%s\n",txbuf);
      GUI_MsgBox (txbuf);
      exit(0);

}




/*
//=================================================================
  int OS_Init_ (char *cmd) {
//=================================================================
// Zuerst mal das Basisdir (fuer os- Routinen) isolieren. HAT "/" am Ende !!
// Im argv[0] steht ca. C:\win_apt\pp\fanuc01.exe
// oder nur xy.exe (wenn per Hand gestartet).
// Beispiel also - in main():
// OS_Init_ (argv[0]);

// cmd in: 1) "gCAD3D" od 2) "gCAD3D/gCAD3D" od 3) "/..../gCAD3D"
// 1): os_bas_dir = getcwd/
// 2): os_bas_dir = getcwd/gCAD3D/
// 3): os_bas_dir = /..../        - dazu muss mit einem script gestartet werden,
//     in dem explizit der komplette Pfad drinsteht !!


  char *p1;

  printf("OS_Init_ |%s|\n",cmd);

  // UtxTab_clear (&os_txTab);             // init (malloc ..)



  if(sizeof(long) < 5) {       // 32-bit
    strcpy (os_bin_dir, "binLinux32");

  } else {                     // 64-bit
    strcpy (os_bin_dir, "binLinux64");

  }


  // strcpy(os_bas_dir, cmd);
// 
  // // check ob ein \ im Text ist
  // p1 = strrchr(os_bas_dir, '\\');
  // if(p1 != NULL) {
    // // Filename wegschneiden ..
    // *p1 = '\0';
    // goto Fertig;
  // }
// 


  strcpy(os_bas_dir, cmd);


  // find last "/"
  p1 = strrchr(os_bas_dir, '/');
  if((p1 == NULL) ||                       // nur der exenam  zB "gCAD3D"
     (!strncmp(os_bas_dir, "./", 2)))   {  // nur der exenam  zB "./gCAD3D"
    getcwd(os_bas_dir, 127);
    strcat(os_bas_dir, "/");
    goto Fertig;
  }


  if(os_bas_dir[0] == '/') {      // absoluter pfad: 
    // remove exenam
    ++p1;
    *p1 = '\0';
    goto Fertig;
  }


  getcwd(os_bas_dir, 127);
  strcat(os_bas_dir, "/");
  strcat(os_bas_dir, cmd);
  p1 = strrchr(os_bas_dir, '/'); // find last "/"
  ++p1;
  *p1 = '\0';


  Fertig:
  sprintf(os_tmp_dir,"%stmp%c",os_bas_dir,fnam_del);

  printf(" basedir = |%s|\n",os_bas_dir);
  printf(" tmpdir = |%s|\n",os_tmp_dir);

  return 0;

}
*/


//================================================================
  void OS_get_scrRes (int *xRes, int *yRes) {
//================================================================
// OS_get_scrRes                    get total screensize
// HD  = 2K = 1920x1080 = 1080p
// UHD = 4K = 3840x2160 = 2160p
//       8K = 7680x4320 = 4320p

  int  irc;
  char s1[80];


  irc = OS_sys1 (s1, 80, "xdpyinfo | awk '/dimensions/ {print $2}'");

  if(irc < 0) {
    TX_Write("***** ERROR OS_get_scrRes - reset to 1920 X 1080");
    *xRes = 1920;
    *yRes = 1080;

  } else {
    sscanf(s1, "%dx%d", xRes, yRes);
  }

    printf("OS_get_scrRes %d %d\n",*xRes,*yRes);

}


//================================================================
  void OS_get_curDir (char *sd, int sSiz) {
//================================================================
/// OS_get_curDir            get current directory

  getcwd (sd, sSiz);

}


//================================================================
  int OS_get_lang  (char *sLang) {
//================================================================
/// returns system-language; eg "de" or "en"
/// retCod: 0=OK; -1= defaulted to "en"

  int  i1=0;
  char *p1;


  p1 = getenv("LANG");     printf("OS_get_lang |%s|\n",p1);


  if(p1) {
    // get first 2 chars + terminating '\0'
    snprintf(sLang, 3, "%s",p1);
      // printf(" txbuf=|%s|\n",txbuf);

  } else {
    printf("**** ERROR OS_get_lang *****************\n");
    strcpy(sLang, "en");
    i1 = -1;
  }


    // printf("ex OS_get_lang |%s|\n",sLang);
// 
  return i1;

}


//================================================================
  char* OS_get_browse_htm  () {
//================================================================
/// returns html-browser; eg "mozilla"
// "1>null 2>null" zugefuegt; sonst geht der OS-output ins ouFile !



  if(OS_browser[0]) return OS_browser;


  // most important last !
  if(system("which konqueror 1>/dev/null 2>/dev/null") == 0)
    strcpy(OS_browser, "konqueror");

  else if(system("which mozilla 1>/dev/null 2>/dev/null") == 0)
    strcpy(OS_browser, "mozilla");

  else if(system("which netscape 1>/dev/null 2>/dev/null") == 0)
    strcpy(OS_browser, "netscape");

  else if(system("which firefox 1>/dev/null 2>/dev/null") == 0)
    strcpy(OS_browser, "firefox");

  else if(system("which xdg-open 1>/dev/null 2>/dev/null") == 0)
    strcpy(OS_browser, "xdg-open");


  if(strlen(OS_browser) < 2) {
    printf(" **** no HTML-Browser found\n");
    return "";
  }

    printf(" OS_get_browse_htm |%s|\n",OS_browser);

  return OS_browser;
}


/* replace by OS_get_zenity
//================================================================
  int OS_get_dialog () {
//================================================================
/// check if zenity is installed. 1=yes, -1=no.

// zenity --error --text 'text'
// zenity --entry --text="input" --entry-text=" alter text"

  static int installed = 0;  // 0=undefined; 1=installed; -1 = not installed.

  if(installed) goto L_exit;

  if(system("which zenity 1>/dev/null 2>/dev/null") == 0) {
    installed = 1;
  } else {
    installed = -1;
  }

  L_exit:
  return installed;
}
*/

//================================================================
  char* OS_get_edi  () {
//================================================================
///  returns fileEditorProgram; eg "gedit "  or "kedit "


  txbuf[0] = '\0';

  if(system("which gedit 1>/dev/null 2>/dev/null") == 0)
    strcpy(txbuf, "gedit ");
  if(system("which kwrite 1>/dev/null 2>/dev/null") == 0)
    strcpy(txbuf, "kwrite ");
  if(system("which kedit 1>/dev/null 2>/dev/null") == 0)
    strcpy(txbuf, "kedit ");
  if(system("which kate 1>/dev/null 2>/dev/null") == 0)
    strcpy(txbuf, "kate ");
  if(system("which dtpad 1>/dev/null 2>/dev/null") == 0)
    strcpy(txbuf, "dtpad ");

  if(strlen(txbuf) < 2) { 
    printf(" **** no Editor found\n");
    return "";
  }

  return txbuf;
}


//================================================================
  char* OS_get_term  () {
//================================================================
///  returns console-program; eg "xterm -e "

  txbuf[0] = '\0';

  if(system("which xterm 1>/dev/null 2>/dev/null") == 0)
    strcpy(txbuf, "xterm -e ");
  if(system("which konsole 1>/dev/null 2>/dev/null") == 0)
    strcpy(txbuf, "konsole -e ");
  if(system("which dtterm 1>/dev/null 2>/dev/null") == 0)
    strcpy(txbuf, "dtterm -e ");


  if(strlen(txbuf) < 2) {
    printf(" **** no terminal found\n");
    return "";
  }

  return txbuf;
}


//================================================================
  char* OS_get_vwr_ps  () {
//================================================================
///  returns ps-viewer (gv|evince)

  static char sVwr[16] = {""};

  if(!sVwr[0]) printf("OS_get_vwr_ps empty\n");


  if(system("which xdg-open 1>/dev/null 2>/dev/null") == 0) {
    strcpy(sVwr, "xdg-open");

  } else if(system("which evince 1>/dev/null 2>/dev/null") == 0) {
    strcpy(sVwr, "evince");

  } else if(system("which gv 1>/dev/null 2>/dev/null") == 0) {
    strcpy(sVwr, "gv");

  } else if(strlen(sVwr) < 2) {
    printf(" **** no postcript-viewer found\n");
    return "";
  }

  return sVwr;
}


//================================================================
  char* OS_get_user  () {
//================================================================
///  returns console-program; eg "xterm -e "

  strcpy (txbuf, getenv("USER"));
  return txbuf;
}


//================================================================
  int OS_browse_htm (char *fNam, char *label) {
//================================================================
/// \code
/// html-browse <filnam>[<label>]; do not wait for end of process.
/// Input:
///   fNam        full filename of html-file;  NULL = <temp>/temp.htm
///   label        html-label starting with '#'; eg "#L1";  NULL = none
/// \endcode

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
    sprintf(s1, "%stmp.htm",OS_get_tmp_dir());

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
    sprintf(s2, "%s file:%s%s 2>/dev/null &", p1, s1, label);
  } else {
    sprintf(s2, "%s %s 2>/dev/null &", p1, s1);
  }

  printf("OS_browse_htm |%s|\n",s2);
  OS_system (s2);

  return 0;

}


//================================================================
  int OS_edit_ (char *filnam) {
//================================================================
/// \code
/// <edit> <filnam>
/// waits for end of process.
/// DO NOT USE: use APP_edit
/// \endcode

  char  cbuf[256];

  sprintf(cbuf, "%s %s",OS_get_edi(),filnam);

  printf("OS_edit_ |%s|\n",cbuf);
  TX_Print("%s",cbuf);

  OS_system(cbuf);

  return 0;

}


//================================================================
  char* OS_get_printer  () {
//================================================================
///  get 1. word of /etc/printcap

  char   *p1;
  FILE   *fp1;

  if ((fp1 = fopen ("/etc/printcap", "r")) == NULL) goto L_err;

  // Das erste Wort (mit | oder : begrenzt) lesen
  // "lp|dj|deskjet:\"  oder "T610:"
  while (!feof (fp1)) {
    if (fgets (txbuf, 126, fp1) == NULL) break;
    // printf(" |%s|\n",txbuf);
    if(txbuf[0] == '#') continue;
    txbuf[60] = '\0';
    p1 = strchr(txbuf, '|'); if(p1) *p1 = '\0';
    p1 = strchr(txbuf, ':'); if(p1) *p1 = '\0';
    UTX_CleanCR (txbuf);                            // remove foll CR,LF ..
    // // remove following ":"
    // UTX_endDelChar (txbuf, ':');
    printf(" printer:|%s|\n",txbuf);
    fclose (fp1);
    return txbuf;
  }
  fclose (fp1);


  L_err:
  printf("**** printer (/etc/printcap) not found ****\n");
  return "";
}


//================================================================
  int OS_spawn_wait (char *cmd, int iwait) {
//================================================================
/// \code
/// execute command and wait explicit
/// cmd should be one word without parameters ...
/// iwait = time to wait in secs
/// \endcode


  int    i1, i2;
  char   osCmd[256];

  // printf("OS_spawn |%s|\n",cmd);


  sprintf(osCmd, "%s 1>/dev/null 2>/dev/null",cmd);
    printf("   |%s|\n",osCmd);
  system (osCmd);

  sprintf(osCmd, "ps --no-heading -C %s 1>/dev/null 2>/dev/null",cmd);
    printf("   |%s|\n",osCmd);

  for(i1=0; i1<iwait; ++i1) {
    i2 = system (osCmd);
    // printf(" i2=%d\n",i2);
    if(i2) return 0;
    sleep (1);
  }

  printf("***** ERROR %s .......... \n",cmd);

  return -1;

}


//=============================================================
  void OS_Wait (int millisecs) {
//=============================================================
/// wait - milli-seconds (<millisecs> * 0.001 seconds)

  unsigned int i1;


  /* sleep geht nur in sec ! */
  if (millisecs > 1000) {
    i1 = millisecs / 1000;
    sleep (i1);

  } else {
    i1 = millisecs * 1000;
    usleep (i1);     // microseconds !
  }



  /*  PC-Version: */
  /* Sleep (millisecs); */


}




//=============================================================
  void OS_beep () {
//=============================================================
// xset b lautsaerke frequenz dauer        - alle Werte 0-100
//   lautst. in Prozent, freq. in  Herz, dauer in ms,



  // OS_system ("echo ");
  // OS_system ("xkbbell");


  //  PC-Version: Frequenz, Dauer als doubles
  // Beep (1000., 200.);

}




/*
//=============================================================
  void OS_id (char *id) {
//=============================================================
// OS_wcclu2                      Read System - ID (uname -m)

  int res;
  struct utsname sid;

  res = uname(&sid);

  // printf("SYSNAME=%s\n",sid.sysname);
  // printf("NODENAME=%s\n",sid.nodename);
  // printf("RELEASE=%s\n",sid.release);
  // printf("VERSION=%s\n",sid.version);
  // printf("MACHINE=%s\n",sid.machine);
  // printf("RESULT=%d\n",res);

  strcpy (id,sid.machine);
}
*/


//===========================================================
  char* OS_get_os__ () {
//===========================================================
// returns "Linux" or "MS" as  string

  sprintf(txbuf, "Linux");

  return txbuf;

}


//===========================================================
  char* OS_get_os_bits () {
//===========================================================
// returns "Linux32" or "Linux64" as  string
// used for "OS=<OS_os>" for dynamic linking


  sprintf(txbuf, "Linux%d",(int)(sizeof(long) * 8));

  return txbuf;

}


//================================================================
  int OS_get_bits () {
//================================================================

  return PTRSIZ * 8;   // 32 || 64            2011-03-18

}


//===========================================================
  void OS_date (long *i1, long *i2, long *i3) {
//===========================================================
/// OS_date                  returns (year, mon, day)


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
/// \code
/// Format: 2002/09/04-13:30:33
/// printf ("# %s\n", OS_date1());
/// \endcode


  long        y,mo,d,h,mi,s;
  static char cdat[24];


  // strcpy(cdat, "2002/09/04-13:30:33");
  //               1234567890123456789

  OS_date (&y,&mo,&d);
  OS_time (&h,&mi,&s);
  sprintf(cdat,"%04ld/%02ld/%02ld-%02ld:%02ld:%02ld",y,mo,d,h,mi,s);
  //            2002/    09/  04-   hh    mm    ss

  return cdat;

}


//================================================================
  int OS_date_cmp (time_t *tim1, time_t *tim2) {
//================================================================
// OS_date_cmp           compare 2 dates
// RetCod: 1   tim2 is younger than tim1
//         0   tim1 and tim2 have same date
//        -1   tim1 is younger than tim2


  double d1;

  // Liefert Diff.Time in Sec (nur auf Sec. genau) als double !
  d1 = difftime (*tim1, *tim2);
    // printf ("time diff:%f\n",d1);
    // does: d1 = tim1 - tim2

  if(d1 > 0.) return -1;  // positiv: tim1 is younger than tim2
  if(d1 < 0.) return  1;  // negativ:  tim2 is younger than tim1
  return 0;

}


//============================================================
  void OS_time (long *i1, long *i2, long *i3) {
//============================================================
/// OS_time                  returns (hour, min, sec)


  time_t actt;
  struct tm *Tm;

  actt=time(0);
  Tm=localtime(&actt);

  *i1=Tm->tm_hour;
  *i2=Tm->tm_min;
  *i3=Tm->tm_sec;

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
/// \code
/// OS_rTime     returns time in sec's, (res = mycrosec's)
/// Example:
/// d1 = OS_rTime (); ... d2 = OS_rTime ();
/// printf(" time used (secs) %lf \n",d2 - d1);
/// \endcode


  double  rt;
  struct timeval ts1;


  gettimeofday (&ts1, 0); // liefert sec (seit 1972) und mycrosecunden.
    // printf ("OS_rTime:%d %d\n",ts1.tv_sec, ts1.tv_usec);
    // printf(" CLOCKS_PER_SEC=%d\n",CLOCKS_PER_SEC);


  rt = (double)ts1.tv_sec + ((double)ts1.tv_usec / CLOCKS_PER_SEC);
    // printf ("rt: %f\n",rt);

  return rt;

}


//================================================================
  int OS_TimeStamp (TimeStamp *ts1) {
//================================================================
/// \code
/// OS_TimeStamp     returns TimeStamp (sec; res = 0.01; ~1 year)
///
/// Example:
///  TimeStamp ts1=0, ts2;
///  OS_TimeStamp (&ts2);
///  if(ts1 != ts2) {update; ts1 = ts2; }
/// \endcode


  struct timeval tr1;


  gettimeofday (&tr1, 0);
  // 20 bit filtern (sonst zu gross f. float)
  *ts1 = (float)(tr1.tv_sec & 0xfffff) + ((float)tr1.tv_usec / CLOCKS_PER_SEC);
    // printf("ex OS_TimeStamp %f\n",*ts1);

  return 0;

}


//======================== Perform OS - Command =========
  int OS_system (char *buf) {
//======================== Perform OS - Command =========
/// \code
/// OS_system                  Perform OS-Command; wait for completion (system)
///   do not wait: use OS_exec
/// \endcode

// Spezialversion fuer AIX + CATIA.


  void *catch;
  int ret;

  catch = signal(SIGCLD, SIG_DFL);
  ret = system(buf);

  signal(SIGCLD, catch);
  if (ret) { perror(buf); }

  return(ret);

}


//================================================================
  int OS_exec (char* txt) {
//================================================================
/// \code
/// OS_exec                  Perform OS-Command; do not wait for completion.
///   do wait: use OS_system
/// \endcode

// es geht nur system mit &

// alle exec*: 
//   timer expired ..

// mit fork:
//  The program '<unknown>' received an X Window System error.
//  The error was 'BadAccess (attempt to access private resource denied)'.
//  Xlib: unexpected async reply (sequence 0x209d)!

  char cbuf[256];

  sprintf(cbuf, "%s&", txt);

  OS_system (cbuf);

  return 0;

}


//================================================================
  int OS_ckFileWritable (char *fnam) {
//================================================================
/// \code
/// OS_checkFileWritable         check if file is writeable;
/// 0=no, 1=yes.
/// \endcode

  char  cbuf[300];

  OS_file_delete (fnam);
  sprintf(cbuf, "touch %s",fnam);
  system(cbuf);

  return OS_checkFilExist (fnam, 0);

}


//=============================================================
  int OS_checkDirExist (char* dirnam) {
//=============================================================
/// \code
/// if dir dirnam does not exist, try to create it.
/// Nur testen ob directory existiert: use OS_checkFilExist.
/// 
/// rc = 0 = Fehler; Dir existiert NICHT.
/// rc = 1 = Dir sucessfully created.
/// rc = 2 = Dir did already exist.
/// \endcode


  int     i1;
  // static char    buf[80];

  // printf("OS_checkDirExist |%s|\n", dirnam);


  if (!OS_checkFilExist (dirnam, 1)) {
    printf (" - Try to create %s\n",dirnam);
    // sprintf (buf, " - Try to create %s\n",dirnam);
    // GUI_Dialog (NULL, buf);

    /* damit dir beschreibbar ist - rw-rw-rw- */
    i1 = umask (0);

    /* mkdir (buf, S_IRWXU|S_IRWXG|S_IRWXO); */
    mkdir (dirnam, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);
      printf(" mkdir |%s|\n",dirnam);


    /* Version PC: */
    /* mkdir (dirnam, 0); */

    return OS_checkFilExist (dirnam, 0);
  }

  return 2;

}


//================================================================
  char* OS_get_dir_pwd () {
//================================================================
// OS_get_dir_pwd           get current process-working-directory "PWD"
// - does not have closing '/'; use UTX_add_fnam_del


  return getenv ("PWD");

}


//================================================================
  int OS_ck_DirAbs (char *fNam) {
//================================================================
/// \code
/// check if string is absolute or relative Filname ..
///   USE this func for MS-win compatibility
/// Returncodes:
///   0  = yes, absolut
///   1  = no, relativ ..
/// 
/// see also OS_dirAbs_fNam
/// 
/// Varianten:
/// /dir/fn
/// ./fn
/// ../dir/fn
/// dir/fn
/// \endcode


  UTX_pos_skipLeadBlk (fNam);

  if(fNam[0] == '/') return 0;

  return 1;

}
 

//================================================================
  int OS_dirAbs_fNam (char *dirOut, char *fNam) {
//================================================================
/// OS_dirAbs_dir            get full (absolut) path from filename
// 
// Varianten:
// /dir/fn
// ./fn
// ../dir/fn
// dir/fn

  char    *cp, *co;



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
  int OS_file_date_m (time_t *tim_m, char *filnam) {
//================================================================
// return unix-timestamp of last modification-date of file <filnam>.
// needs:
// #include <time.h>
// #include <unistd.h>
// #include <sys/stat.h>
// #include <sys/time.h>
// 
//   time_t        st_atime;    // Letzter Zugriff
//   time_t        st_mtime;    // Letzte Modifikation
//   time_t        st_ctime;    // Letzte Aenderung

  int         rc;
  struct stat fileStatus;


  // printf("OS_file_date_m %s\n",filnam);

  rc = stat (filnam, &fileStatus);

  if(rc < 0) {  // File nicht vorh.
    printf("OS_file_date_m: file %s does not exist\n",filnam);
    return -1;
  }

    // // printf(" st_atime=%s",ctime(&fileStatus.st_atime));
    // printf(" st_mtime=%s",ctime(&fileStatus.st_mtime));
    // // printf(" st_ctime=%s",ctime(&fileStatus.st_ctime));

  *tim_m = fileStatus.st_mtime;

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


//=============================================================
  int OS_check_isDir (char* dirnam) {
//=============================================================
/// \code
/// returns if {dirnam} is directory or file
/// rc = 1  = OFF = NO; dirnam is NOT directory
/// rc = 0  = ON  = OK; dirnam is directory.
/// rc = -1 = Error - dirnam does not exist
/// \endcode

// braucht:  #include <sys/stat.h> und #include <unistd.h> ?

  int         rc;
  struct stat fileStatus;


  // printf("OS_check_isDir |%s|\n", dirnam);


  rc = stat (dirnam, &fileStatus);

  if(rc != 0) return -1;   // Error; dirnam does not exist.

  // printf("typ=%d\n",fileStatus.st_mode);
  // printf("typ=%d\n",fileStatus.st_mode&S_IFDIR);
  // FileSize: printf("%d\n", fileStatus.st_size);


  // if(fileStatus.st_mode & 0x0040000) {
  // if(fileStatus.st_mode == 16877) {
  if(fileStatus.st_mode&S_IFDIR) {
    // OK - ist Dir
    // printf("ex OS_check_isDir 0=|%s|\n", dirnam);
    return 0;
  }

  // printf("ex OS_check_isDir 1=|%s|\n", dirnam);
  return 1;

}


//==========================================================================
  int OS_dir_root (char *roots, int csiz) {
//==========================================================================
/// OS_dir_root          returns a list of root-directories

  int  i1, i2;
  char cbuf[256], *p2;


  // printf("OS_dir_root %d\n",csiz);


  strcpy(cbuf, "/");
  i1 = 0;
  i2 = 0;
  p2 = roots;


  L_next:
  OS_dir_scan_ (cbuf, &i1);
  if(i1 < 0) goto L_fertig;


  // check if it is file or dir
  if(OS_check_isDir(cbuf) == 0) {
    // printf(" >>>>>>>>> |%s| %d\n",cbuf,i1);

    // add to roots
    i2 += strlen(cbuf) + 1;
    if(i2 >= csiz) {
      TX_Error("*** OS_dir_root E001");
      return -1;
    }
    strcat(p2, cbuf);
    p2 = &roots[i2];
    *p2 =  '\0';
  }
  goto L_next;



  L_fertig:
  return 0;

}

//==========================================================================
  int OS_dir_ck1 (char *p1) {
//==========================================================================
/// \code
/// check rootDir
/// irc=0  CD-Rom,
///     1  Diskette,
///     2  normal
/// \endcode

  int    i1, irc=0;
  struct stat fileStatus;
  // struct statfs fsysStat; 
  FILE   *fp1;


  // printf("OS_dir_ck1 |%s|\n",p1);

  // das folgende hilft nix!
  // if(OS_check_isDir(p1) != 0) { irc=-1; goto L_fertig; }
 
  // muss mind lesbar sein !
  i1 = access (p1, R_OK);  // W_OK od W_ACC
  // printf(" access=%d\n",i1);
  if(i1 != 0) goto L_fertig;


  // ist es ueberhaupt gemountet ?
  irc = stat (p1, &fileStatus);
  if(irc != 0) { irc = -1; goto L_fertig; }

  if(fileStatus.st_mode&S_IFDIR) irc = 2;

  // printf("IFREG=%d\n",fileStatus.st_mode&S_IFREG);
  // printf("typ=%d\n",fileStatus.st_mode);
  // printf("lnk=%d\n",fileStatus.st_nlink);
  // printf("dev=%d\n",fileStatus.st_dev);
  // printf("blk=%d\n",fileStatus.st_blocks);
  // printf("siz=%d\n",fileStatus.st_size);


  // try to open ..
  // fp1 = fopen (p1, "r");
  // if(fp1) { fclose(fp1); irc = -2; goto L_fertig; }
   
  // i1 = open(p1, O_RDONLY);
  // printf("open %d\n",i1);
  // if(fp1) { fclose(fp1); irc = -2; goto L_fertig; }

  


  // irc = statfs (p1, &fsysStat);
  // printf("statfs %d %d\n",irc,fsysStat.f_bfree);
 


  L_fertig:
  // printf("ex OS_dir_ck1 %d=|%s|\n",irc,p1);
  return irc;

}


//==========================================================================
  int OS_dir_ck2 (char *dir) {
//==========================================================================
/// \code
/// check if directory is writable
/// irc=0=no, not writable,
///     1=jes, directory is writable
/// \endcode

  int    i1, irc=0;
  // struct stat fileStatus;
  // struct statfs fsysStat; 
  // FILE   *fp1;


  // printf("OS_dir_ck1 |%s|\n",dir);

 
  i1 = access (dir, W_OK);  // W_OK od W_ACC; 0=Ok/Ja
  // printf(" access=%d\n",i1);
  if(i1 != 0) goto L_fertig;
  irc = 1; // ja, ist beschreibar.


  L_fertig:
  // printf("ex OS_dir_ck1 %d=|%s|\n",irc,dir);
  return irc;

}


//==========================================================================
  int OS_dir_scan_ (char *cbuf, int *iNr) {
//==========================================================================
// OS_dir_scan_                     scan directory
// - not sorted; different for UX / MS !
// start scanning with cbuf = path_to_scan and iNr = 0;
// all following calls to OS_dir_scan_ return a filenname in cbuf
//   until (iNr < 0). Do not modify iNr for all following calls.
// Initial call with (iNr = 0) does not yet return a filenname.
// End of files: iNr and retCode returns -1.
//
// opendir, closedir, readdir: include sys/types and dirent.h


  static DIR     *dir1;
  static char    dirAct[256];
  static int     dirStat = 0;

  int            i1, irc;
  char           *p1;
  struct dirent  *fn1;


  // printf("OS_dir_scan_ |%s| %d\n",cbuf,*iNr);
  // if(*iNr > 100) exit(0);


  //----------------------------------------------------------------
  if(*iNr == 0) {
    // init - open
    if(dirStat) closedir (dir1);

    i1 = strlen(cbuf);
    if(i1 >= sizeof(dirAct)) { printf("OS_dir_scan_ E strlen"); return -1; }
    strcpy(dirAct, cbuf);
    if(dirAct[i1-1] != '/') strcat(dirAct, "/");  // add "\\"
      // printf("init OS_dir_scan_ |%s| %d\n",dirAct,*iNr);

    dir1 = opendir (dirAct);

    if(dir1 == NULL) {
      // printf("OS_dir_scan_ E opendir %s\n",dirAct);
      *iNr = -1;
      irc  = -1;
      goto L_exit;
    }
    dirStat = 1;
    goto L_fertig;
  }


  //----------------------------------------------------------------
  NextNam:
  fn1 = readdir (dir1);
  if(fn1 == NULL) {
    closedir (dir1);
    *iNr = -1;
    irc  = -1;
    dirStat = 0;
    goto L_exit;
  }

  p1 = fn1->d_name;
    // printf(" next1 |%s| fType=%d\n",p1,fn1->d_type);

  // skip "." und ".."
  if(!strcmp (p1, ".")) goto NextNam;
  if(!strcmp (p1, "..")) goto NextNam;


  strcpy(cbuf, dirAct);
  strcat(cbuf, p1);
    // printf(" next2 |%s|\n",cbuf);

  L_fertig:
  *iNr += 1;
  irc  = 0;

  L_exit:
    // printf("ex OS_dir_scan_ %d |%s| %d\n",irc,cbuf,*iNr);
  return irc;

}


//==========================================================================
  int OS_filterff (char *fnamO, char *fnamI, char* ftext) {
//==========================================================================
/// \code
/// sort & filter file. Ignore case.
/// gesamte Liste (Datei) sortieren u. filtern;
/// ftext = Filtertext
/// RC = -1: kein Inputfile
/// \endcode

// Die InputDatei muss aber erhalten bleiben !

  char cbuf[512];

  // printf("OS_filterff |%s|\n",ftext);


  if(OS_checkFilExist(fnamI,1) == 0) return -1;


  if(strlen(ftext) < 1) {
    sprintf(cbuf,"export LC_ALL=C&&cat %s|sort > %s",fnamI,fnamO);
    // printf("  sys |%s|\n",cbuf);
    system(cbuf);


  } else {
    sprintf(cbuf,"cat %s|sort|grep -i \"%s\" > %s",fnamI,ftext,fnamO);
    // printf("  sys |%s|\n",cbuf);
    system(cbuf);
  }

  if(OS_checkFilExist(fnamO,1) == 0) return -2;

  return 0;

}


//==========================================================================
  int OS_file_delGrp (char *fNam) {
//==========================================================================
// OS_file_delGrp           Delete files/Wildcard; Single File "remove (fn);"
//   Example:    OS_file_delGrp ("/tmp/exp*.exp");

  char cbuf[512];


  // printf("OS_file_delGrp |%s|\n",fNam);

  sprintf(cbuf,"rm -rf %s",fNam);
    // printf("OS_file_delGrp |%s|\n",cbuf);
  system(cbuf);

  return 0;

}


//==========================================================================
  int OS_file_copy (char *oldNam, char *newNam) {
//==========================================================================
// OS_file_copy             copy file
//   retCode      0=OK; else Error

  char cbuf[512];

  // printf("OS_file_copy |%s|%s|\n",oldNam,newNam);

  sprintf(cbuf,"/bin/cp -f \"%s\" \"%s\"",oldNam,newNam);
    // printf(cBuf, "copy /y %s %s",fnOld, fnNew);  // MS
    // printf("OS_file_copy |%s|\n",cbuf);

  return system (cbuf);

  // return 0;

}


//========================================================================
  int OS_file_rename (char *fnOld, char *fnNew) {
//========================================================================
/// rename File; NO Wildcards !
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
/// \code
/// delete File; NO Wildcards !
/// MS u Unix gleich.
/// retCod: 0=OK; -1=Error.
/// \endcode

  // printf("OS_file_delete |%s|\n",fNam);

  return remove (fNam);    // ACHTUNG: keine Wildcards mit remove !

}


//================================================================
  int OS_file_readm1 (char *cbuf, int cSiz, char *fnam) {
//================================================================
/// \code
/// read complete file into memory
/// Datei fnam in den Buff cbuf einlesen.
/// cSiz = size of cbuf
/// Retcode:
///   -1    Error
///   >0    size of file
/// \endcode

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


/*
//================================================================
  int OS_dll_global (char *dllNam) {
//================================================================
// connect a global ddl
// can call all functions of this dll direct (without dlsym)
// must call this function from a dll !

  void  *dl1;

  extern int gtk_gui_t1 ();

  // load DLL
  dl1 = dlopen (dllNam, RTLD_GLOBAL);
  if(dl1 == NULL) {
    TX_Error("OS_dll_global: cannot open dyn. Lib. |%s|",dllNam);
    return -1;
  }

  return 0;

}
*/


//================================================================
  int OS_prc_dll (int mode, void *fDat) { 
//================================================================
/// \code
/// load dll | start dll-function | unload dll
/// TODO: replace with OS_dll__
/// mode >=0  work (mode = nc-function from NCCmdTab)
/// mode -1 = work (programfunction fDat = "FUNC_xx")
/// mode -2 = open (load Lib fNam)
/// mode -3 = connect (connect Func fDat)
/// mode -4 = unload active lib 
/// \endcode


static void  *dl1 = NULL;
// static void  (*up1)();
static int   (*up1)();
    
  int   irc;
  char  *p1, s1[256];
    

  // printf("OS_prc_dll %d\n",mode);
    

  //----------------------------------------------------------------
  // mode -2 = open (load Lib fNam)
  if(mode == -2) {

    // load DLL
    dl1 = dlopen(fDat, RTLD_LAZY);
    if(dl1 == NULL) {
      TX_Error("OS_prc_dll: cannot open dyn. Lib. |%s|",(char*)fDat);
      return -1;
    }

    // damit Debugger stoppt, nachdem DLL geladen wurde
    p1 = strrchr(fDat, fnam_del);
    if(p1 == NULL) p1 = fDat;
    else ++p1;  // skip fnam_del
    OS_debug_dll_(p1);



  //----------------------------------------------------------------
  // mode -3 = connect (connect Func fDat)
  } else if(mode == -3) {
      // printf(" func fDat = |%s|\n",(char*)fDat);


    // Adresse von Func.fncNam holen
    up1 = dlsym(dl1, fDat);
    if(up1 == NULL) {
      TX_Error("OS_prc_dll: cannot open Func. |%s|",(char*)fDat);
      OS_dll_close (&dl1);   // unload DLL
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
    OS_dll_close (&dl1);              // unload DLL
    up1 = NULL;
  }

  return 0;

}


///===================================================================
  int OS_dll_build (char *dllNam) {
///===================================================================
/// \code
/// OS_dll_build             (re)build dll (compile/link with .mak)
/// Input:
///   dllNam   eg "xa_ige_r.so"   (without path, with Filetyp).
///            - using xa_ige_r.mak for build
/// \endcode

  int  irc;
  char *sdir, cbuf[256];         // char cbuf[512];



  printf("OS_dll_build |%s|\n",dllNam);


  // get the directory ${gcad_dir_dev}src/APP
  sdir = getenv ("gcad_dir_dev");
  if(!sdir) {TX_Print("***** cannot find direcory gcad_dir_dev ..."); return -1;}


  // sprintf(cbuf, "%sxa/%s",OS_get_bas_dir(),dllNam);
  // sprintf(cbuf, "%s../src/APP/%s",OS_get_loc_dir(),dllNam);
  sprintf(cbuf, "%ssrc/APP/%s", sdir, dllNam);


  // ".so" -> ".mak"
  strcpy(&cbuf[strlen(cbuf)-3], ".mak");
    printf(" exist: |%s|\n",cbuf);
  if(OS_checkFilExist (cbuf, 1) == 0) goto L_err_nof;


  TX_Print(".. compile .. link .. %s",dllNam);


  // sprintf(cbuf, "cd %sxa;make -f %s",OS_get_bas_dir(),dllNam);
  // sprintf(cbuf, "cd %s../src/APP;make -f %s",OS_get_loc_dir(),dllNam);
  sprintf(cbuf, "cd %ssrc/APP;make -f %s", sdir, dllNam);
    printf(" OS_dll_build 2 |%s|\n",cbuf);


  // ".so" -> ".mak"
  strcpy(&cbuf[strlen(cbuf)-3], ".mak OS=");
  strcat(cbuf, OS_get_os_bits());
    printf("dll_build-3 |%s|\n",cbuf);


  irc = system(cbuf);
  if(irc != 0) TX_Error("Error build %s",dllNam);

  return irc;

  L_err_nof:
    TX_Print("***** %s does not exist ..",cbuf);

    return 0;

}


/* UNUSED
//================================================================
  int OS_dll_close_fn (char *dllNam) {
//================================================================
// OS_dll_close_fn          close dll from libraryName
// DOES NOT WORK .. ??

  void  *dl1;
  char  s1[256];

  sprintf(s1, "%s%s.so",OS_get_bin_dir(),dllNam);
  // sprintf(s1, "%s.so",dllNam);

    printf("OS_dll_close_fn |%s|\n",s1);


  // load DLL
  dl1 = NULL;
  dl1 = dlopen (dllNam, RTLD_NOLOAD);
  if(dl1 == NULL) {
    //TX_Error("OS_dll_close_fn: cannot open dyn. Lib. |%s|",s1);
    printf("**** Lib. %s not open ..\n",s1);

    return -1;
  }


  // close DLL
  OS_dll_close (&dl1);     // unload DLL

    printf("ex-OS_dll_close_fn\n");


  return 0;

}
*/

//================================================================
  int OS_dll_unload_idle (void *data) {
//================================================================
// DLL_unload       unload a dll (idle-callback)


  // printf("OS_dll_unload_idle |%s|\n",(char*)data);

  // OS_dll_close_fn ((char*)data);
  OS_dll_do ((char*)data, NULL, NULL, 2);

  return 0;   // must be 0 - removes idle-Call

}


//================================================================
  int OS_dll_close (void **dl1) {
//================================================================
// on successfule returns irc = 0,  dl1 = NULL
// BUG dlclose: returnCode is OK but handle not NULL.

  int  irc = 0;

  // printf("OS_dll_close \n");

  // unload if already loaded
  if(*dl1 != NULL) {
    irc = dlclose (*dl1);  // 0=success
      // printf(" close %d\n",irc);
    if(!irc) *dl1 = NULL;
  }

  return irc;

}


//================================================================
  int OS_dll__ (void **dl1, int mode, void *fDat) {
//================================================================
/// \code
/// load dll | start dll-function | unload dll
/// Input:
///   mode
///     FUNC_LOAD_only = load DLL. fDat: dll-name without directory, fTyp.
///     FUNC_LOAD_all  = load DLL. fDat: dll-name without directory, fTyp.
///     FUNC_CONNECT   = connect (connect Func fDat)
///     FUNC_EXEC      = work (call active Func with parameter fDat)
///     FUNC_UNLOAD    = unload active lib; fDat unused
///     FUNC_RECOMPILE = recompile DLL; fDat = dllName without path, without Filetyp
/// Output:
///   dll      (address of) loaded DLL
///   retCod   0=OK; else error
/// \endcode


static int   (*up1)();

  int   irc;
  char  s1[256], *p1;


  // printf("OS_dll__ %d\n",mode);


  //----------------------------------------------------------------
  // mode 0 = open (load Lib fNam) 
  if((mode == FUNC_LOAD_only)  ||
     (mode == FUNC_LOAD_all))      {

    // unload if already loaded
    OS_dll_close (dl1);
    if(*dl1 != NULL) goto L_e_cl;

    // load DLL
    sprintf(s1, "%s%s.so",OS_get_bin_dir(),(char*)fDat);


    if(mode == FUNC_LOAD_only) {
      // load funcs with FUNC_CONNECT
      *dl1 = dlopen (s1, RTLD_LAZY);
         // printf(" LOAD_only |%s|\n",s1); fflush(stdout);

    } else {
      // load all funcs in dll
      *dl1 = dlopen (s1, RTLD_LAZY|RTLD_GLOBAL);
      // CANNOT close recompile reOpen this dll !!
      // *dl1 = dlopen (s1, RTLD_NOW|RTLD_GLOBAL);
      // *dl1 = dlopen (s1, RTLD_NOW|RTLD_GLOBAL|RTLD_DEEPBIND);
         printf(" LOAD_all |%s|\n",s1); fflush(stdout);
    }

    if(*dl1 == NULL) {
      TX_Error("OS_dll__: cannot open dyn. Lib. |%s|",(char*)fDat);
      return -1;
    }
    up1 = NULL;

    // stop Debugger after DLL has been loaded
    OS_debug_dll_((char*)fDat);



  //----------------------------------------------------------------
  // mode 1 = connect (connect Func fDat)
  } else if(mode == FUNC_CONNECT) {
      // printf(" func fDat = |%s|\n",(char*)fDat);

    // get adress of Function
    up1 = dlsym (*dl1, fDat);
    if(up1 == NULL) {
      TX_Error("OS_dll__: cannot open Func. |%s|",(char*)fDat);
      OS_dll_close (dl1);             // unload DLL
      return -1;
    }



  //----------------------------------------------------------------
  // mode 2 = work (call active Func with parameter fDat)
  } else if(mode == FUNC_EXEC) {

    // start userprog
    if(up1 != NULL) {
      return (*up1)(fDat);

    } else {
      TX_Error ("OS_dll__ E001");
      return -1;
    }



  //----------------------------------------------------------------
  // mode 3 = unload active lib
  } else if(mode == FUNC_UNLOAD) {

    // close DLL
    OS_dll_close (dl1);
    if(*dl1 != NULL) goto L_e_cl;


  //----------------------------------------------------------------
  // 4 = recompile dll
  } else if(mode == FUNC_RECOMPILE) {

    OS_dll_close (dl1);
    if(*dl1 != NULL) goto L_e_cl;

    sprintf(s1, "%s.so",(char*)fDat);
      // printf(" dll=|%s|\n",s1); fflush(stdout);
  
    if(OS_dll_build (s1) != 0) {
       TX_Error("OS_dll__: compile/link %s",s1);
       return -1;
    }


  //----------------------------------------------------------------
  }

  return 0;


  L_e_cl:
    // TX_Error("OS_dll__: close dll .. ");
    printf("***** ERROR OS_dll__: close dll .. \n");
    return -1;

}


//====================================================================
  int OS_debug_dll_ (char *dllNam) {
//====================================================================
/// \code
/// wenn im Debug-Modus wird hier gestoppt;
/// In .gdbinit  muss stehen:
/// break OS_debug_dll_
/// wenn Debugger steht, eingeben "watch varnam" und "c"
///
/// Zweck: Debugger kann DLL nur ansprechen wenn geladen.
/// \endcode

  // printf("OS_debug_dll_ |%s|\n",dllNam);

  return 0;

}


/* 
//====================================================================
  int OS_dll_do (char *dllNam, char *fncnam, void *fncdat, int mode) {
//====================================================================
/// load dll; start function fncNam (fncDat); unload dll.
// Input:
//   fncnam  main-entry using datablock
//   fncdat  datablock
//   mode    0=load+start+unload
//           1=load+start               - do not unload (OS_dll_close_fn)


  // // remove dll (damit man Compile-Error bemerkt)
  // OS_file_delete ("xa/xa_stp_r.so");
  // system("cd xa;make -f xa_stp_r.mak");
  // AP_dll_do ("xa_stp_r", "STP_r__", "dat/.0.stp");



  char  cBuf[256];


  printf("OS_dll_do |%s|%s| %d\n",dllNam,fncnam,mode);



  // fix DLL-FileName
// #ifdef _MSC_VER
  // sprintf(cBuf, "%s%s.dll",OS_get_bin_dir(),dllNam);
// #else
  sprintf(cBuf, "%s%s.so",OS_get_bin_dir(),dllNam);
// #endif

  // printf(" so|dll=|%s|\n",cBuf);


  // load dll, start function, unload dll
  return OS_dll_run (cBuf, fncnam, fncdat, mode);

}
*/


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


static void  *dl1=NULL;
static char  dlNamAct[256];
  char  *p1, s1[256];
  void  (*up1)();

 
  printf("OS_dll_do |%s|%s| %d\n",dllNam,fncNam,mode);
  // fflush (stdout);


  if(mode == 2) { mode = 0; goto L_close; }


  if(dl1 != NULL) {
    TX_Error("**** ERROR OS_dll_run: core-plugin |%s|  open ..",dllNam);
    return -1;
  }


  // fix DLL-FileName
  strcpy(dlNamAct, dllNam);
  sprintf(s1, "%s%s.so",OS_get_bin_dir(),dllNam);


  // load DLL
  dl1 = dlopen(s1, RTLD_LAZY);
  if(dl1 == NULL) {
    TX_Error("OS_dll_do: cannot open dyn. Lib. |%s|",dllNam);
    return -1;
  }


  // stop debugger after dll is loaded
  OS_debug_dll_(dllNam);


  // get adress of func. <fncNam>
  up1 = dlsym(dl1, fncNam);
  if(up1 == NULL) {
    OS_dll_close (&dl1);     // unload DLL
    TX_Error("OS_dll_do: cannot open Func. |%s|",fncNam);
    return -1;
  }


  // start userprog
  (*up1)(fncDat);
     printf(" foll-dll_do |%s|\n",fncNam);



  // close DLL
  L_close:
  if(mode < 1) OS_dll_close (&dl1);     // unload DLL

    printf("ex-OS_dll_do\n");


  return 0;

}


//================================================================
  int OS_file_zip (int mode, char *fnFrom, char *fnTo) {
//================================================================
/// \code
/// compress | uncompress file
/// mode = 0: compress:   gzip -c9 t1.gcad > t1.gcaz
/// mode = 1: uncompress: gzip -dc t1.gcaz > t1.gcad
/// \endcode

  char   osCmd[400];

  printf("OS_file_zip %d |%s|%s|\n",mode,fnFrom,fnTo);


  if(system("which zip 1>/dev/null 2>/dev/null") != 0) {
    TX_Print("zip/unzip not installed ........");
    return -1;
  }

  if(mode == 0) {
    sprintf(osCmd, "gzip -qc %s > %s",fnFrom,fnTo);
    // sprintf(osCmd, "zip -9jq %s %s",fnTo,fnFrom);
  } else {
    sprintf(osCmd, "gzip -dqc %s > %s",fnFrom,fnTo);
    // sprintf(osCmd, "unzip -pqq %s > %s",fnFrom,fnTo);
  }

    printf("|%s|\n",osCmd);

  return OS_system (osCmd);

}


//================================================================
  int OS_file_concat (char *fno, char *fn1, char *fn2) {
//================================================================
// concatenate 2 files  (fno = fn1 + fn2)
// fno must be different.

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

  sprintf(s1, "cat %s %s > %s",fn1,fn2,fno);
    // printf(" |%s|\n",s1);
  system (s1);

  return 0;

}


//================================================================
  int OS_file_compare_A (char *fn1, char *fn2) {
//================================================================
// OS_file_compare_A                   compare 2 files ascii
// RetCode: 0=files_not_different, -1=different_files.
// MS-Win: FC

  char   s1[512];


  sprintf (s1, "diff -q %s %s", fn1, fn2);
  // returns 0 for identical files
  // sprintf (s1, "cmp -s %s %s", fn1, fn2);


  return OS_system (s1);

}


//================================================================
  char* OS_get_imgConv1  () {
//================================================================
/// returns jpg2bmp-converter-program; eg /usr/bin/djpeg

  static int  iStat = 0;          // 0=notYetTested; 1=OK; -1=NotOk.
  static char fn1[] = "djpeg";



  if(iStat == 0) {    // init
    iStat = system("which djpeg 1>/dev/null 2>/dev/null");
    if(iStat == 0) iStat =  1;   // OK
    else           iStat = -1;   // not OK
  }

  if(iStat > 0) return &fn1[0];


  // printf(" **** jpg2bmp-converter djpeg does not exist\n");
  MSG_pri_1 ("NOEX_fil", "jpg2bmp-converter djpeg");


  return "";

}


//================================================================
  char* OS_get_imgConv2  () {
//================================================================
/// returns bmp2jpg-converter-program; eg /usr/bin/cjpeg

  static int  iStat = 0;          // 0=notYetTested; 1=OK; -1=NotOk.
  static char fn1[] = "cjpeg";


  if(iStat == 0) {    // init
    iStat = system("which cjpeg 1>/dev/null 2>/dev/null");
    if(iStat == 0) iStat =  1;   // OK
    else           iStat = -1;   // not OK
  }

  if(iStat > 0) return &fn1[0];

  MSG_pri_1 ("NOEX_fil", "bmp2jpg-converter cjpeg");

  return "";

}


//================================================================
  int OS_jpg_bmp (char *fn_jpg, char *fn_bmp) {
//================================================================
// convert BMP -> JPG

  char  s1[400];

  sprintf(s1, "%s \"%s\" > \"%s\"",OS_get_imgConv2(),fn_bmp,fn_jpg);
    printf(" |%s|\n",s1);

  return OS_system(s1);

}


//================================================================
  int OS_dev_ck () {
//================================================================
// test if compiler & Linker exists; 0=yes, -1=no

  int  iStat;


  iStat = system("which cc 1>/dev/null 2>/dev/null");
  if(iStat != 0) goto L_notOK;

  iStat = system("which make 1>/dev/null 2>/dev/null");
  if(iStat != 0) goto L_notOK;



  L_exit:
    printf("OS_dev_ck %d\n",iStat);
  return iStat;


  L_notOK:
  iStat = -1;
  goto L_exit;

}

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
  sprintf(sEnam,"%sGUI_dlg1_%s%d", OS_get_bin_dir(), sGui, vGtk);
    printf(" OS_get_GUI |%s|\n",sEnam);

  // test if exe exists
  if(!OS_checkFilExist(sEnam,1)) {
    TX_Print("**** file %s does not exist ..", sEnam);
    return -1;
  }

  return 0;

}



//========================= EOF ====================================
