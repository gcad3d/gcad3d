/*
 *
 * Copyright (C) 2023 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
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
../ut/os_exe_file.c    functions for auxiliary-executables (GUI_* ..); not core
-----------------------------------------------------
TODO:

-----------------------------------------------------
Modifications:
2023-03-16 Created from AP_gui__, OS_exe__. RF.


=====================================================
List_functions_start:

OS_exe__           do exe, return its output as string
OS_exe_file_fn     get full filename for GUI_executable
GUI_file_in        get filename for Input; eg <tmpDir>temp_<rand>_in
GUI_file_out_in    get fnOut from fnIn (argv[1])

OS_exe_file_test

List_functions_end:
=====================================================


Using:  see INF_GUI_dlg1   (../inf/../inf/GUI.c)


=============================================================================
// ReBuild:
. ../options.sh && make -f GUI_dlg1.mak


// ReBuild and test:
./GUI_test_dlg1.sh


cat /tmp/debug.dat

*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>              // for ...

// #include <gtk/gtk.h>

#include "../ut/ut_umem.h"          // Memspc
#include "../ut/ut_txTab.h"         // UtxTab_NEW
#include "../xa/mdl__.h"            // SIZFNam
// #include "../ut/deb_prt.h"          // printd


// activate printd
// #define printd printf
// disable printd
// #define printd if(0) printf


// /// FilenamedelimiterChar
// #if defined _MSC_VER || __MINGW64__
// #define  fnam_del '\\'
// #define  fnam_del_s "\\"
// #else
// #define  fnam_del '/'
// #define  fnam_del_s "/"
// #endif


//----------------------------------------------------------------
// prototypes
  char*  OS_get_tmp_dir();
  char*  OS_bin_dir_get();



//================================================================
  int OS_exe_file__ (char *sOut, int sSiz, char *fnExe, ...) {
//================================================================
// OS_exe__           do exe, return its output as string
//   exe must read input via file, put output into file (GUI_*.exe)
//   - if inFilNam is eg "temp_123_in" then outFilNam must be "temp_123_out"
// Input:
//   fnExe       name of executable; eg "GUI_dlg1"
//   ...         all parameters (strings); go into file for input for exe
// Output:
//   sOut        text - output of fnExe
//   sSiz        size of sOut

  int   irc;
  char  fnTmp[400], fnfExe[SIZFNam], s2[2048], *p1;
  FILE  *fpo;
  va_list va;


  // printf("OS_exe_file__ |%s|\n",fnExe);

  // get full exename
  irc = OS_exe_file_fn (fnfExe, fnExe);

  // create filename outputFile
  GUI_file_in (fnTmp);

  //----------------------------------------------------------------
  // write parameters into outFile fnOut
  if((fpo=fopen(fnTmp,"w")) == NULL) {
    printf("***** Error OS_exe_file__ Open %s\n",fnTmp);
    exit(1);
  }

  va_start (va, fnExe);

  L_nxt_arg:
     p1 = (void*)va_arg (va, char*);
     if(p1) {
         // printf(" exe_file__-nxt |%s|\n",p1);
       fprintf(fpo, "%s\n",p1);
       goto L_nxt_arg;
     }
  va_end (va);

  fclose(fpo);


  //----------------------------------------------------------------
  // call "exe <inputFileName>"

#if defined _MSC_VER || __MINGW64__
  sprintf(s2,  "CMD /C \"\"%s\" \"%s\"\"",fnfExe,fnTmp);
#else
  sprintf(s2, "%s %s",fnfExe,fnTmp);   // Linux
#endif
    // printf(" OS_exe_file__-s2 |%s|\n",s2);

  irc = OS_system (s2);
  if(irc) {printf("***** OS_exe_file__ - Error OS_system %d\n",irc); return -1;}


  //----------------------------------------------------------------
  // remove inputFile of exe
  OS_file_delete (fnTmp);

  // create outFilnam of exe
  GUI_file_out_in (fnTmp, fnTmp);

  // read output of exe (in outFilnam)
  irc = UTX_fgetLine (sOut, sSiz, fnTmp, 1);

  // remove outFile of GUI_dlg1
  OS_file_delete (fnTmp);

    // printf(" ex-OS_exe_file__ %d |%s|\n",irc,sOut);

  return irc;

}


//================================================================
  int OS_exe_file_fn (char* sEnam, char *sNam) {
//================================================================
// AP_GUI_get             get full filename for GUI_executable
// Input:
//   sNam           eg GUI_dlg1 or GUI_open
// Output:
//   sEnam          get full filename; size must be SIZFNam
//                  eg /home/fwork/devel/bin/gcad3d/Linux_x86_64/GUI_file_gtk2

  int      irc, vGtk;
  char     sGui[32];


  // printf("OS_exe_file_fn |%s|\n",sNam);


  // get gtk-major-version
  GUI_get_version (sGui, &vGtk, &irc);


  // sEnam = exeFilename
#ifdef _MSC_VER
  sprintf(sEnam,"%s%s_%s%d_MS.exe", OS_bin_dir_get(), sNam, sGui, vGtk);
#elif __MINGW64__
  if(!strncmp (sNam, "GUI_", 4))  {
    // GUI_*.exe - add Gtk<version>
    sprintf(sEnam,"%s%s_%s%d.exe", OS_bin_dir_get(), sNam, sGui, vGtk);
  } else {
    sprintf(sEnam,"%s%s.exe", OS_bin_dir_get(), sNam);
  }
#else
  sprintf(sEnam,"%s%s_%s%d", OS_bin_dir_get(), sNam, sGui, vGtk);
#endif

    printf(" ex-OS_exe_file_fn |%s|\n",sEnam);

  return 0;

}


//================================================================
  int OS_exe_file_test () {
//================================================================


  int    irc;
  char   filNam[SIZFNam], s1[SIZFNam];


  printf(" OS_exe_file_test\n");


//----------------------------------------------------------------
// test open
  strcpy(filNam, "/mnt/serv2/devel/cadfiles/gcad/");

  // call GUI_file/save
  irc = OS_exe_file__ (filNam,                // directory/filename in and out
                  SIZFNam,                  // size if filNam
                  "GUI_file",            // exenam without directory;GUI_file|GUI_dlg1
                  "open",                // for GUI_file: open | save
                  filNam,                // outDir/outfilename
                  "/mnt/serv2/devel/gcad3d/gCAD3D/cfg_Linux/dir.lst",  // filnam symDir
                  "",                    // MSYS: expands "*""\"*\"",  // filterText
                  "sTit",                // window-title
                  "\"x40,y30\"",         // window-size
                  NULL);




//----------------------------------------------------------------
// test save
//   // call GUI_file-save
//   OS_exe_file__ (s1, sizeof(s1), "GUI_file", "save",
//              "/mnt/serv2/devel/cadfiles/gcad/unknown.gcad",
//              "/mnt/serv2/devel/gcad3d/gCAD3D/cfg_Linux/dir.lst",
//              "*",
//              "Speichern",
//              NULL);

  return 0;

}


//================================================================
  int GUI_file_in (char *fnOut) {
//================================================================
// get filename for Input; eg <tmpDir>temp_<rand>_in
// size must be 400


  // filename outputFile
  sprintf (fnOut, "%stemp_%d_in",OS_get_tmp_dir(),(int)rand());

    printf(" ex-GUI_file_in |%s|\n",fnOut);

  return 0;

}


//================================================================
  int GUI_file_out_in (char *fnOut, char *fnIn) {
//================================================================
// get fnOut from fnIn (argv[1])
// // size in and out 400; can be ident;

  int   i1;

  if(fnOut != fnIn) strcpy (fnOut, fnIn);

  // change last 2 chars "in" -> "out"
  i1 = strlen(fnOut) - 2;
  strcpy (&fnOut[i1], "out");
    printf(" ex-GUI_file_out_in |%s|\n",fnOut);

  return 0;

}


// eof

