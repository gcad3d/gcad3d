//     xa_main.c          AP_search_init
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
2012-03-16 test if <gcad_dir_bas>icons/ exist  added. RF.

-----------------------------------------------------
*/
/*!
\file  ../xa/xa_main.c
\brief main () des Interaktiven gCAD 
\code
=====================================================
List_functions_start:

main              main for interactive gcad
AP_lang_init      get language - first start only
AP_lngTab_get     get list of installed languages
AP_lngTab_set     set list of installed languages

List_functions_end:
=====================================================

\endcode *//*----------------------------------------

Batch-main ist in xa_batch.c.

*/

#if defined _MSC_VER || __MINGW64__
#define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>              // islower ..


// Achtung: braucht mit w32 __pctype u ___mb_cur_max / unresolvbar !
//#include <ctype.h>

#include "../ut/ut_geo.h"              // OFF, ON ..
#include "../ut/ut_os.h"               // AP_get_bas_dir
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_txTab.h"            // TxtTab
#include "../ut/ut_memTab.h"           // MemTab
#include "../ut/ut_deb.h"              // DEB_exit DEB_stop DEB_STAT


#include "../gui/gui__.h"
// #include "../ut/func_types.h"          // UI_FuncInit

#include "../xa/xa_ui.h"               // UI_win_main
#include "../xa/xa_uid.h"              // UID_Main_title
#include "../xa/xa_mem.h"
#include "../xa/xa_sele.h"             // Typ_goGeom
#include "../xa/xa.h"                  // AP_mod_fnam
#include "../xa/xa_msg.h"              // MSG_get_str
#include "../xa/gcad_version.h"        // INIT_TXT



//_____________________________________
// EXTERNALS:
// aus ../xa/xa.c:
extern int       AP_argNr;
extern char      **AP_argv;
extern TxtTab    AP_TxTab1;

// ../ci/NC_Main.c
extern double  APT_ModSiz;



//_____________________________________
       char *AP_bin_dir = NULL;         // dir binaries
static char *AP_bas_dir = NULL;         // gcad_dir_bas
static char *AP_loc_dir = NULL;         // local
static char *AP_cfg_dir = NULL;
static char *AP_tmp_dir = NULL;
static char *AP_doc_dir = NULL;
       char *AP_ico_dir = NULL;


#define LNG_MAX_NR 16
typedef  char lngCode[4];
static lngCode lngTab[LNG_MAX_NR];
static int     lngNr;






//_____________________________________
// prototypes:
// int UI_GR_RECONFIG (void *data1, void *data2);



//================================================================
  int gdb_halt () {
//================================================================
// used for halting in debugger.
// break gdb_halt      // in gCAD3D.gdb

  printf(" gdb_halt ..\n");

  return 0;

}


//================================================================
  int main (int paranz, char* argv[]) {
//================================================================
// directory-structure:   see ../../doc/html/SW_layout_en.htm
//
//----------------------------------------------------------------
// activate reInstall of cfg_x, examples.gz
//   rm -rf <gcad_dir_local><cfgDir>gCAD3D.rc
//
// exit gcad: write xa.rc 
//----------------------------------------------------------------


  int     i1, i2, irc;
  char    txbuf1[800], txbuf2[512], *p1;
  double  d1;
  MemObj  win0;
  stru_FN ofn;


  strcpy(txbuf1, INIT_TXT);
  printf("+++++++++++++ Start %s %s\n",txbuf1,OS_date1());

  AP_argNr = paranz;
  AP_argv  = argv;
  for(i1=0; i1<paranz; ++i1) printf("arg[%d]|%s|\n",i1,AP_argv[i1]);



  // mtrace ();   // malloc-tests
  printf("size int:%ld long:%ld ptr:%ld\n",sizeof(int),sizeof(long), sizeof(&i1));


#if defined _MSC_VER || __MINGW64__
    // hide the command-window
    OS_hide_win ();
#else
    // display Linux-Version, libc-Version, gtk-Version
    OS_config ();
#endif


  // init variables of ../xa/xa.h
  AP_init_xa_h ();


  // get AP_bin_dir = dir binaries
  OS_bin_dir_set (argv);


  // init primary textBuffer
  UtxTab_init__ (&AP_TxTab1); 


/*
  // set Defaultlanguage
  strcpy(AP_lang, OS_get_lang());
    printf(" AP_lang=|%s|\n",AP_lang);
*/


  // mit --g-fatal-warnings   bricht aber ganz ab !
  // gtk_init (&paranz, &argv);
  // system(". export LC_ALL=C");   // f sort
  memset (&AP_stat, '\0', sizeof(AP_stat));  // init with 0

  APP_act_typ = 0;

  AP_deb_fp = NULL;


  // init size-of-structs-Table
  irc = UTO_siz_stru (TYPE_STRU_NR);
  if(irc < 0) return -1;


  // set application-directories:
  // AP_loc_dir AP_get_tmp_dir AP_get_cfg_dir AP_get_doc_dir AP_get_ico_dir
  AP_get_dir__ ();


  // get dialog-software (check if GUI (file GUI_dlg1..) exists
  // OS_get_dialog ();
  // p1 = OS_get_zenity ();
  // if(!p1) {
  irc = OS_get_GUI ();
  if(irc) {
    // abort; GUI_dlg1 - exe not found / installed !
    sprintf(txbuf2, "******* ERROR: GUI_dlg1 not installed *****");
    fprintf(stdout, "%s\n",txbuf2);
    fprintf(stderr, "%s\n",txbuf2);
    exit(2);
  }



  // fill lngTab = list of loaded languages
  AP_lngTab_set ();


  DEB_STAT = 0;

  // display debug-switch
#ifdef DEB
  strcpy(txbuf2, "ON");
#else
  strcpy(txbuf2, "OFF");
#endif
  printf("Debug-switch DDEB is %s\n",txbuf2);


  //----------------------------------------------------------------
  // create directory <local> - with AP_cfg_dir AP_tmp_dir ..
  if(!OS_checkDirExist (AP_get_loc_dir())) {
    sprintf(txbuf2, "cannot create directory %s.\n"
            " Fix gcad_dir_local in startup-script.", AP_loc_dir);
    GUI_MsgBox(txbuf2);
    exit(0);
  }


  // create directory AP_tmp_dir;
  if(!OS_checkDirExist (AP_get_tmp_dir())) {
    sprintf(txbuf2, "cannot create directory %s.\n"
            " Fix gcad_dir_local in startup-script.", AP_get_tmp_dir());
    GUI_MsgBox(txbuf2);
    exit(0);
  }

  
  //----------------------------------------------------------------
  // test if <AP_cfg_dir>cfg_Linux/gCAD3D.rc exists; if not: unpack examples.gz
  sprintf(txbuf1, "%sgCAD3D.rc", AP_cfg_dir);
    printf(" test configfile |%s|\n",txbuf1);
  if(OS_checkFilExist (txbuf1, 1)) goto L_startup_defaults;

// TODO: write message into logfile; display logfile at startup of messagewindow ...
  printf("****** configfile %s does not exist - init config-directory\n",txbuf1);


  // test if examples.gz exists
  sprintf(txbuf2, "%sexamples.gz",AP_bas_dir);
  if(!OS_checkFilExist (txbuf2, 1)) {
    sprintf(txbuf1, "cannot (re)install examples.gz - installation problem ..");
    GUI_MsgBox (txbuf1);
  }


// restore all cfg-files;
#if defined _MSC_VER || __MINGW64__
  // MS-Win:-------------------------------------------------------
  // POSTINSTALL: MS-tar only works in outDirectory ..
  // copy <AP_bas_dir>examples.gz -> <AP_loc_dir>.
  sprintf(txbuf1, "%sexamples.gz", AP_loc_dir);         // new
  OS_file_copy (txbuf2, txbuf1);                        // (old, new)
  // unpack: cd <AP_loc_dir> && <AP_bin_dir>tar.exe -xzf examples.gz
  sprintf(txbuf1, "CD/D \"%s\" && \"%star.exe\" -xzf examples.gz",
                      AP_loc_dir, AP_bin_dir);
    printf("%s\n",txbuf1);
  system(txbuf1);


#else
  // LINUX:-------------------------------------------------------
  // POSTINSTALL:  extract <AP_bas_dir>examples.gz -> local /cfg/
  sprintf(txbuf1, "cd %s && tar -xzf %s", AP_loc_dir, txbuf2);
    printf("%s\n",txbuf1);
  OS_system(txbuf1);

#endif



  //----------------------------------------------------------------
  L_startup_defaults:
    printf(" L_startup_defaults:\n");

  // test if <cfgdir>dir.lst exists
  sprintf(txbuf1, "%sdir.lst",AP_get_cfg_dir());
  if(!OS_checkFilExist(txbuf1, 1)) goto L_write_defaults;

  // test if <cfgdir>xa.rc exists
  sprintf(txbuf1,"%sxa.rc",AP_get_cfg_dir());
  if(OS_checkFilExist(txbuf1, 1)) goto L_normal_start;



  //----------------------------------------------------------------
  // first Start: AP_defaults_init - create & write defaults
  L_write_defaults:
      printf(" first start ..\n");
    // create Defaults
    strcpy(AP_mod_fnam, "unknown.gcad");
    sprintf(AP_mod_dir, "%sdat%c",AP_loc_dir,fnam_del);
    // sprintf(AP_mod_dir, "%sdat",AP_loc_dir);
    sprintf(AP_dir_prg, "%sprg%c",AP_loc_dir,fnam_del);

    // get printer
#if defined _MSC_VER || __MINGW64__
    sprintf(AP_printer, "print");
#else
    sprintf(AP_printer, "lpr -l -P%s",OS_get_printer());
#endif

    // get AP_lang = language
    AP_lang_init ();

    // // get browser
    // p1 = OS_get_browse_htm();
    // UTX_cp_left (AP_browser, p1, 79);

    // get editor
    p1 = OS_get_edi();
    UTX_cp_left (AP_editor, p1, 79);

    sprintf(AP_winSiz, "%s   // size of application-window", WinSizMin__);
    // strcpy(AP_winSiz, "-600,-400   // total-size; too small for W32

    APT_ModSiz = 500.;
    strcpy(AP_symDir_fnam, "dir.lst");

    // create defaults ~/gCAD3D/cfg_<os>/xa.rc
    AP_defaults_write();

    // create initial directory-path-file  ~/gCAD3D/cfg_<os>/dir.lst
    AP_defaults_dir();


    // exit(0);  // TEST ONLY


  //==========================================================
  L_normal_start:

  // load gui-dll
  // sprintf(txbuf1, "%splugins/xa_gui_gtk_2.so",OS_bin_dir_get());
    // printf(" gui-dll=|%s|\n",txbuf1);
  // irc = OS_dll_global (txbuf1);    // dlopen
    // if(irc < 0) return -1;





  //==========================================================
  // Init und display Windows

  GR_Init0 ();   // set only primary parameters



  // delete pipe CTRLpin (after crash commands can remain ..)
  sprintf(txbuf1, "%sCTRLpin",AP_get_tmp_dir());
  OS_file_delete (txbuf1);

  // read Defaults from <base>/cfg/xa.rc
  AP_defaults_read ();
  // find and set symbol of AP_mod_dir = last full directory open
//   Mod_sym_get2 (AP_mod_sym, AP_mod_dir, 1);


  MDLFN_oFn_fNam (&ofn, AP_mod_dir);   // get fileName-obj from  last dir
  MDLFN_set__ (&ofn);                  // set symDir fDir


  // init MSG-system
  MSG_const_init (AP_lang);
  MSG_Init (AP_lang);
  OMN_init ();

  // Linetypes laden erfolgt erst in DL_InitAttTab

  // create list of *.dll (*.so) in tmp/Dll.lst
  AP_DllLst_write ();       // create <tmp>plugins.lst
  PRC_lst_processors ();    // create <tmp>cadprocessors.lst


  // init DisplList
  DL_Init ();


  //----------------------------------------------------------------
  // setup window
  // init gtk
  GUI_Init__ ("");     // Read Colours und Fonts
  // GUI_Init ("xa/xa.rc");
    // printf(" f-GUI_Init__ fffffffffffffffffffffffffffffffffffff\n");

  // create all icons
  Ico_init ();       

  // init CAD - interactive-editor
  IE_init__ ();

  // // alle Pfade aus Datei path.setup auslesen und speichern
  // Mod_init_path ();

  // Init und display Windows
  // UI_AP (UI_FuncInit, UID_WinMain, NULL);

  // Init und display Windows
  UI_win_main (&win0, GUI_SETDAT_EI(TYP_EventPress,UI_FuncInit));
  // starts Grafic-window GUI_gl__ UI_GL_draw__ ..


  // Info-Window - only MS-Win (else no cursor in Gtk-2-MAN-editor)
// #if defined _MSC_VER || __MINGW64__
  // UI_WinInfo2 (); // cannot be killed -
// #endif


  AP_UserSelection_reset ();
  AP_UserKeyIn_reset ();      // reset  KeyIn


  AP_stat.sysStat = 1;

  AP_stat.build = OS_dev_ck ();             // test if compiler exists; 1=yes, -1=no
  OS_get_imgConv1 ();       // 1=djpeg exists;
  OS_get_imgConv2 ();       // 1=cjpeg exists;


  // test only - display if messagfile is missing
  MSG_const_init (AP_lang);
  MSG_Init (AP_lang);


  // INIT_TXT = gCAD3D Version <Version> <Datum>
  strcpy(txbuf1, INIT_TXT);
  // GUI: GTK<version> - <subversion>
  GUI_get_version (txbuf2, &i1, &i2);
  TX_Print("%s     GUI: %s%d.%d", txbuf1, txbuf2, i1, i2);


  // strcat(txbuf1, " - Host ");
  // os_l_id (memspc50);
  // strcat(txbuf1, memspc50);
  // TX_Write (txbuf1);


  // display init-text
  MSG_pri_0 ("MM0");
  // TX_Write ("display funcions:");
  // TX_Write ("move:   Shift + drag mouse (do not push mousebuttons)");
  // TX_Write ("rotate: Ctrl + drag mouse");
  // TX_Write ("zoom:   Shift + Ctrl + drag mouse");


  // // init tolerances
  // WC_Init_Tol();

  // // init View-Plane and ConstrPlane  AP_mdl_init
  // AP_Init_planes ();

  // APT-Texbuffer fuer ersten Start
  UTF_alloc__ (100);

  // (re)create GLU-tess-obj
  // GLT_init__ ();


/* 2012-07-30
  // set GL-shaden
  UI_wireCB (NULL, NULL);

  // config OpenGL-window
  UI_GR_idle_GL1_CB (NULL);       // emit configure_event (UI_GR_GLInit)
  // GUI_update__ ();
  // AP_stat.sysStat = 2;


  // configure_event
  if(AP_stat.sysStat < 2) UI_GR_Reshape (NULL, NULL);


  // unbedingt erforderl wenn kein RUN - sonst Achsenkreuz ganz schief
  UI_GR_view (NULL, (void*)"IsoView");
*/


  // default ist VWR
  UI_src_mem (0);

  // clear src-Memory, reset Undo, Hide, View-Plane, ConstrPlane.
  // AP_mdl_init ();    // raus 2009-06-20  (kills all Model* for reLoad)


  // enable selection of all types
  UI_set_Ins_Sel_Cat (0);  // 0=FALSE

/*
  // die Startparameter abarbeiten
  i1 = 1;
  i2 = 0;
  // printf(" vor AP_work__ %d\n",paranz);
  L_next_par:
  if(paranz > i1) {
      printf(" nxtpar |%s|\n",argv[i1]);
    if(!strcmp(argv[i1], "new")) i2 = 1;       // new - kein defLoad !
    irc = AP_work__ (argv[i1], argv[i1+1]);
    if(irc > 0) i1 += irc;
    ++i1;
    goto L_next_par;
  }


  // wenn new innerhalb der Inputdaten nix laden
  if(i2 > 0) goto L_start9;
*/


/* 2012-07-30
  // Datei AP_mod_fnam laden (nur wenn noch nix geladen)
  // tmp/Model zerteilen und laden
    // printf(" FilBuf0Len=%d\n",UTF_GetLen0());
  if(UTF_GetLen0() < 24) {
    AP_defLoad (0);
    // sprintf(txbuf1,"%stmp%cModel",AP_get_bas_dir(),fnam_del);
    // Mod_load__ (txbuf1);
    // ED_work_END (0); // ABARBEITEN
  }
*/

  //================================================================
  L_start9:

  // UI_butCB (NULL, "VWR");
  // sele_set__ (Typ_goGeom);    // enable selection of all types (mode "VWR")
  // disactivate menu Insert Select CATALOG,


/* 2012-07-30
  // Title oben auf den Mainwinrahmen
  UI_AP (UI_FuncSet, UID_Main_title, NULL);
*/


  // Startup fertig; in debug: give chance to set watchpoints
  AP_debug__ ("main"); // die func ist ganz leer; ist als stoppoint definiert.



  // mtrace geht leider nicht, da gtk und Mesa zu viele Stoerzeilen machen ..
// #ifdef DEB
  // mtrace(); // starts to write all mallocs/frees --> File <$MALLOC_TRACE>
// #endif



  // UI_idle__ ();


  // // im Modus CAD starten
  // gtk_toggle_button_set_state (GTK_TOGGLE_BUTTON(ckb_cad), TRUE);

/*
  // load last application (ex AP_defaults_read)
  if(strlen(APP_act_nam) > 0) {
    strcpy(txbuf1, APP_act_nam);
#ifdef _MSC_VER
    strcat(txbuf1, ".dll");
#else
    strcat(txbuf1, ".so");
#endif
    DLL_run2 (txbuf1, 0);
  }
*/

  // redraw with first idle ..
  // GUI_Idle (UI_GR_RECONFIG, NULL);    // 2012-07-30
  // GUI_Idle (UI_GR_idle_CB, NULL);

  // Enter gtk-Mainloop; kein return von dieser Funktion.
  // UI_AP (UI_FuncWork, UID_WinMain, NULL);
  GUI_Win_go (&win0);


  return 0;

}


//================================================================
  int AP_get_dir__ () {
//================================================================
// AP_get_dir__              set application-directories
// AP_get_tmp_dir AP_get_cfg_dir AP_get_doc_dir AP_get_ico_dir

  char    *p1 = NULL, *p2, s1[512];


  printf("AP_get_dir__ \n");
  // system ("echo gcad_dir_bas = ${gcad_dir_bas}");
  // system ("set | grep gcad");
  if(AP_bin_dir) printf("- gcad_dir_bin   = %s\n",AP_bin_dir);



  //----------------------------------------------------------------
  // get AP_bas_dir  (examples.gz,icons/)        ("../")
  // get "/usr/share/gcad3d/"
  p1 = OS_get_env("gcad_dir_bas");             // icons
  if(p1) {
      // printf(" gcad_dir_bas is defined - |%s|\n",p1);
    if(strlen(p1) > 255) goto L_err2;
    strcpy(s1, p1);
    UTX_fdir_add_del (s1);

  } else {
#if defined _MSC_VER || __MINGW64__
    // MS-WIN
    strcpy(s1, AP_bin_dir);
    UTX_endDelChar (s1, fnam_del);    // remove last char ('/')
    UTX_endDelWord (s1, fnam_del, 0); // remove last word (keep '/')
    // strcat(s1, "icons\\");
#else
    // Linux: "/usr/share/gcad3d/"
      // printf(" - is Linux\n");
    strcpy(s1, "/usr/share/gcad3d/");
#endif
  }
    // printf(" pwd = |%s|\n",OS_get_dir_pwd());
    // printf(" gcad_dir_dev = |%s|\n",OS_get_env("gcad_dir_dev"));
    // printf(" gcad_dir_bas = |%s|\n",s1);
    // printf(" AP_bas_dir = |%s|\n",s1);

  if(!OS_checkFilExist(s1, 1)) {
    strcpy(s1, "Directory \"gcad_dir_bas\" not found; Installationproblem.\n"
           " Fix gcad_dir_bas in startup-script.");
    GUI_MsgBox (s1);
    exit(-1);
  }

  UtxTab_add (&AP_TxTab1, s1);
  UtxTab_query (&AP_bas_dir, &AP_TxTab1);
    printf("- AP_bas_dir = %s\n",AP_bas_dir);



  //----------------------------------------------------------------
  // get AP_loc_dir from get gcad_dir_local or $HOME/gCAD3D/
  p1 = OS_get_env("gcad_dir_local");       // tmp, cfg_*, prg, ctlg
  if(p1) {
      // printf(" gcad_dir_local is defined - |%s|\n",p1);
    if(strlen(p1) > 255) goto L_err2;
    strcpy(s1, p1);
    UTX_fdir_add_del (s1);

  } else {
#if defined _MSC_VER || __MINGW64__
    // MS-WIN
    p1 = OS_get_env("APPDATA");
      // printf(" APPDATA = |%s|\n",p1);
    if(!p1) {
      strcpy(s1, "Cannot locate APPDATA for a temporary directory.\n"
             " Fix gcad_dir_local in startup-script.");
      GUI_MsgBox (s1);
      exit(-1);
      // UTX_fdir_add_del (s1);   // add closing "\\"
    }
    sprintf(s1, "%s\\gCAD3D\\",p1);
#else
    // Linux: 
    p1 = OS_get_env("HOME");
      // printf(" HOME = |%s|\n",p1);
    if(!p1) {
      strcpy(s1, "Cannot locate $HOME for a temporary directory.\n"
             " Fix gcad_dir_local in startup-script.");
      GUI_MsgBox (s1);
      exit(-1);
    }
    sprintf(s1, "%s/gCAD3D/",p1);
#endif
  }

  UtxTab_add (&AP_TxTab1, s1);
  UtxTab_query (&AP_loc_dir, &AP_TxTab1);
    printf("- AP_loc_dir = %s\n",AP_loc_dir);


  if(!OS_checkFilExist(AP_loc_dir, 1)) {
    printf(" - Directory \"AP_loc_dir\" not found; create.\n");
    OS_dir_cre (AP_loc_dir);
    if(!OS_checkFilExist(AP_loc_dir, 1)) {
      printf("***** cannot create directory %s; abort.\n",AP_loc_dir);
      exit(-1);
    }
  }


  //----------------------------------------------------------------
  // AP_tmp_dir         ("/home/fwork/gCAD3D/tmp/");
  strcpy(s1, AP_loc_dir);
  strcat(s1, "tmp");
  UTX_fdir_add_del (s1);   // add closing "/" or "\\"

  UtxTab_add (&AP_TxTab1, s1);
  UtxTab_query (&AP_tmp_dir, &AP_TxTab1);
     printf("- AP_tmp_dir = %s\n",AP_tmp_dir);


  //----------------------------------------------------------------
  // AP_cfg_dir         ("/home/fwork/gCAD3D/cfg_<os>/");
  strcpy(s1, AP_loc_dir);
//   strcat(s1, "cfg");
//   UTX_fdir_add_del (s1);   // add closing "/"
  sprintf(s1, "%scfg_%s", AP_loc_dir, OS_get_os__());  // "os="UIX"|"MSYS")
  UTX_fdir_add_del (s1);   // add closing "/"

  UtxTab_add (&AP_TxTab1, s1);
  UtxTab_query (&AP_cfg_dir, &AP_TxTab1);
    printf("- AP_cfg_dir = %s\n",AP_cfg_dir);


  //----------------------------------------------------------------
  // AP_doc_dir         ("<AP_bas_dir>doc/"  html, msg)
  p1 = OS_get_env("gcad_dir_doc");
  if(p1) {
      // printf(" gcad_dir_doc is defined - |%s|\n",p1);
    if(strlen(p1) > 255) goto L_err2;
    strcpy(s1, p1);
    UTX_fdir_add_del (s1);
  
  } else {
    strcpy(s1, AP_bas_dir);
  }

  strcat(s1, "doc");
  UTX_fdir_add_del (s1);   // add closing "/" or "\\"

  if(!OS_checkFilExist(s1, 1)) {
    printf("**** ERROR AP_doc_dir - %s\n",s1);
    strcpy(s1, "Directory \"gcad_dir_doc\" not found; Installationproblem.\n"
           " Fix gcad_dir_doc in startup-script.");
    GUI_MsgBox (s1);
    exit(-1);
  }

  UtxTab_add (&AP_TxTab1, s1);
  UtxTab_query (&AP_doc_dir, &AP_TxTab1);
    printf("- AP_doc_dir = %s\n",AP_doc_dir);


  //----------------------------------------------------------------
  // AP_ico_dir         ("<AP_bas_dir>icons/");
  p1 = OS_get_env("gcad_dir_ico");
  if(p1) {
    if(strlen(p1) > 255) goto L_err2;
    strcpy(s1, p1);
    UTX_fdir_add_del (s1);

  } else {
    strcpy(s1, AP_bas_dir);
  }

  strcat(s1, "icons");
  UTX_fdir_add_del (s1);   // add closing "/" or "\\"

  if(!OS_checkFilExist(s1, 1)) {
    printf("**** ERROR AP_ico_dir - %s\n",s1);
    printf("***** AP_ico_dir: %s\n",s1);
    strcpy(s1, "Directory \"AP_ico_dir\" not found; Installationproblem.\n"
           " Fix gcad_dir_ico in startup-script.");
    GUI_MsgBox (s1);
    exit(-1);
  }

  UtxTab_add (&AP_TxTab1, s1);
  UtxTab_query (&AP_ico_dir, &AP_TxTab1);
    printf("- AP_ico_dir = %s\n",AP_ico_dir);


    // TESTBLOCK
    // exit(MSG_ERR__ (ERR_USER_ABORT, "testexit"));
    // END TESTBLOCK
 

  //----------------------------------------------------------------
  return 0;

  L_err2:
      strcpy(s1, "Error: directoryname too long.\n");
      printf("%s\n",s1);
      GUI_MsgBox (s1);
      exit(0);

}


//================================================================
  int AP_lang_init () {
//================================================================
// AP_lang_init                    get language - first start only

  int      ii;
  char     s1[80];

  // get system-language
  OS_get_lang (s1);
    printf(" system-language |%s|\n",s1);

  // find language in lngTab; if not exists: use english
  for(ii=0; ii<lngNr; ++ii) {
    if(!strcmp(lngTab[ii], s1)) goto L_set;
  }

  strcpy(s1,"en");

  // set AP_lang = language
  L_set:
  strcpy(AP_lang, s1);
    printf("ex-AP_lang_init |%s|\n",AP_lang);

  return 0;

}


//================================================================
  int AP_lngNam_get (char *lngNam, char *lngCode) {
//================================================================
// was MSG_lng_init
// provide list of supported languages and language-names
//   find all msg_<LANG>.txt files  in <docdir>/msg/

  char *pl;

  // printf(" AP_lngNam_get |%s|\n",lngCode);

  // - get value of LANG__ of all existing language-files
  MSG_Init (lngCode);
  pl = MSG_get_str ("LANG__");
  if(!pl) return -1;

  strcpy (lngNam, pl);
    // printf(" lang = |%s|\n",lngNam);

  return 0;

}


//================================================================
  int AP_lngTab_get (char **lngLst) {
//================================================================
// was MSG_lng_init
// provide list of supported languages and language-names
//   find all msg_<LANG>.txt files  in <docdir>/msg/

  *lngLst = &lngTab[0][0];

  return lngNr;

}


//================================================================
  int AP_lngTab_set () {
//================================================================
// AP_lngTab_set                  fill lngTab = list of available languages
// was MSG_lng_init
// provide list of supported languages and language-names
//   find all msg_<LANG>.txt files  in <docdir>/msg/

  int    ii, iNr, lNr;
  char   *p1, cbuf1[256], *pl;

// uu
// sssss();

  // printf("MSG_lng_init %d\n",*lngNr);


  //----------------------------------------------------------------
  // - make list of all <docdir>/msg/msg_*.txt

#if defined _MSC_VER || __MINGW64__
  sprintf(cbuf1,"%smsg",AP_get_doc_dir());
#else
  sprintf(cbuf1,"%smsg/",AP_get_doc_dir());
#endif


  ii = strlen (cbuf1);
    // printf(" _scan_- %d |%s|\n",ii,cbuf1);
// 
  iNr = 0;
  OS_dir_scan_ (cbuf1, &iNr);   // Init
    // printf(" _scan_%d |%s|\n",ii,cbuf1);

  lNr = 0;
  for(;;)  {
    OS_dir_scan_ (cbuf1, &iNr);
      // printf(" _scan_%d |%s|\n",iNr,cbuf1);
    if(iNr < 0) break;
    p1 = strstr (&cbuf1[ii], "msg_");
    if(!p1) continue;

    // if(strncmp(&cbuf1[ii], "msg_", 4)) continue;
    p1 += 4;
    if(!strncmp(p1, "const", 4)) continue;


    // extract & copy language-code
    strncpy (lngTab[lNr], p1, 2);
    lngTab[lNr][2] = '\0';
      // printf(" n.scan |%s| %d |%s|\n",lngCode[lNr], lNr, cbuf1);

    if(lNr < LNG_MAX_NR) ++lNr;
  }



  //----------------------------------------------------------------
  lngNr = lNr;


    // TESTBLOCK
    // printf("------ ex AP_lngTab_set %d \n",lNr);
    for(ii=0; ii<lNr; ++ii) printf(" lng[%d] %s\n",ii,lngTab[ii]);
    // END TESTBLOCK


  return 0;

}


//================================================================
  char* AP_get_loc_dir () {
//================================================================
/// returns confDir (with closing '/')  <gcad_dir_local>xa/

  return AP_loc_dir;

}

//================================================================
  char* AP_get_cfg_dir () {
//================================================================
/// returns confDir (with closing '/')  <gcad_dir_local>xa/
      
  return AP_cfg_dir;

}

//================================================================
  char* AP_get_tmp_dir () {
//================================================================
// returns tempDir (with closing '/')  <gcad_dir_local>tmp/
// SEE ALSO OS_get_tmp_dir()

  return AP_tmp_dir;

}

//================================================================
  char* AP_get_doc_dir () {
//================================================================
/// AP_get_doc_dir           returns path of docu
           
  return AP_doc_dir;

} 

//================================================================
  char* AP_get_ico_dir () {
//================================================================
/// returns directory for icons (with closing '/')  <gcad_dir_bin>icons/

  return AP_ico_dir;

}


//================================================================
  char* AP_get_bas_dir () {
//================================================================
/// AP_get_bas_dir           returns path of ?

  return AP_bas_dir;

}


//================================================================
  int TODO (char *fn, ...) {
//================================================================
/// AP_get_bas_dir           returns path of ?

  printf("************ TODO func %s () ***********\n",fn);


  return 0;

}




/* ====================== EOF =========================== */
