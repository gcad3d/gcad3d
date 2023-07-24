// gCAD3D - APPLICATION-INTERFACE                    2008-01-21   Franz Reiter
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
  Common funcs and vars for Application/Process/Plugin: ../xa/xa_app.c

Application  (gcad-script)  PRG    ../xa/xa_prg.c
Process      (script+dll)   PRC    ../xa/xa_proc.c
Plugin       (C-dll)        APP    ../xa/xa_plu.c ../xa/xa_appDat.c ../xa/xa_dll.c


TODO:
-) statt GR_Sel_Filter=17 sollte AP_User* verwendendet werden
   (see xa_ped.c)
-) jeder Input soll alle felder testen; wenn einer fehlt: nix tun,
   else update.

-----------------------------------------------------
Modifications:
  ..

-----------------------------------------------------
*/
/*!
\file  ../xa/xa_app.c
\brief applicationInterface for plugins APP_ 
\code
=====================================================
List_functions_start:

APP_Help               simple help
APP_Save               simple save
APP_edit               edit file
APP_htm_fop            open a temp-file for a html-dumpFile
APP_htm_print          write into open htm-file

List_functions_end:
=====================================================
UU:
// APP_Open               select file from list, callback.

\endcode *//*----------------------------------------

*/


#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifndef _MSC_VER
#include <dlfcn.h>           // Unix: dlopen
#endif


#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_os.h"               // AP_get_bas_dir ..

#include "../ut/func_types.h"               // UI_FuncUCB8

#include "../ut/ut_memTab.h"           // MemTab
#include "../xa/xa_mem.h"              // memspc51, mem_cbuf1
#include "../xa/xa_uid.h"              // UID_ckb_comp
#include "../xa/xa.h"                  // AP_STAT



//===============================================================
// Externe Variablen:
// defined in ../xa/xa.c (with extern invalidated)
extern char AP_mod_dir[SIZMFTot];     // directory for SAVE


// ../xa/xa_plu.c
extern APP_OBJ_NAM *UI_User_appNamTab;




//===============================================================
// Interne Variablen:
// table of user-defined names of application-objects




//==== prototypes: ===============================================

  // void  (*xa_test)();

/*
//================================================================
  int OS_browse_htm (char *fNam, char *label) {
//================================================================
/// \code
/// html-browse <filnam>#<label>
/// do not wait for end of process.
/// Input:
///   filnam         full filename of html-file;  NULL = <temp>/temp.htm
///   label          html-label starting with '#'; eg "#L1"
///
/// see OS_browse_htm
/// \endcode

// "firefox file:///mnt/serv1/Devel/dev/gCAD3D/doc/CAD_AC_de.htm#F6"

  char s1[256], s2[320], *p1;


  printf("OS_browse_htm |%s|%s|\n",fNam,label);
    

  //----------------------------------------------------------------
  // test if file exists; if not: change language to "en"
  if(OS_checkFilExist (fNam, 1)) goto L_disp;
  TX_Print ("%s - file does not exist", fNam);

  // file does not exist; change language to "en"
  // extract langCode (2 chars)
  p1 = strrchr (fNam, '.');
  if(!p1) return -1;
  p1 -= 2;
  strncpy (p1, "en", 2);

  // test if file exists;
  if(OS_checkFilExist (fNam, 1) == 0) {
    TX_Print ("%s - file does not exist", fNam);
    return -1;
  }


  //----------------------------------------------------------------
  // display file fNam with AP_browser
  L_disp:
    return OS_browse_htm (fNam, label);

  // // add label to filename makes:
  // // browser unix: firefox|xdg-open|?
  // // <browser> file:<fnam>#<label>
  // // <browser> <fnam>
  // if(strlen(label)) {
    // sprintf(s1, "file:%s%s", fNam,label);
  // } else {
    // strcpy(s1, fNam);
  // }
// 
// #ifdef _MSC_VER
  // sprintf(s2, "start %s %s",AP_browser,s1);
// #else
  // // sprintf(cbuf, "%s file://%s 2>/dev/null &",AP_browser,fNam);
  // // "file://" - problems with "../fn"
  // sprintf(s2, "%s %s 2>/dev/null &",AP_browser,s1);
// #endif
// 
  // printf("OS_browse_htm |%s|\n",s2);
  // OS_system(s2);
// 
  // TX_Print ("- display %s", s2);
// 
  // return 0;
// 
// 
 //  
// 
  // strcat(s1, label);
// 
  // return OS_browse_htm (s1);


}


//================================================================
  int OS_browse_htm (char *filnam) {
//================================================================
/// \code
/// html-browse <filnam>
/// do not wait for end of process.
/// filnam = NULL    using default <temp>/temp.htm
///
/// see OS_browse_htm
/// \endcode

// "firefox file:///mnt/serv1/Devel/dev/gCAD3D/doc/CAD_AC_de.htm#F6"


  char  cbuf[280], s1[256], *fNam, *p1;



  printf("OS_browse_htm in |%s|\n",filnam);


  if(filnam == NULL) {
    sprintf(s1, "%stmp.htm",AP_get_tmp_dir());
    fNam = s1;
  } else {
    fNam = filnam;
  }


  //----------------------------------------------------------------
  // test if file exists; if not: change language to "en"
  if(OS_checkFilExist (fNam, 1)) goto L_disp;
  TX_Print ("%s - file does not exist", fNam);

  // file does not exist; change language to "en"
  // extract langCode (2 chars)
  p1 = strrchr (fNam, '.');
  if(!p1) return -1;
  p1 -= 2;
  strncpy (p1, "en", 2);

  // test if 
  // test if file exists; if not: change language to "en"
  if(OS_checkFilExist (fNam, 1) == 0) {
    TX_Print ("%s - file does not exist", fNam);
    return -1;
  }


  //----------------------------------------------------------------
  // display file fNam with AP_browser
  L_disp:
    return OS_browse_htm (fNam, "");

// 
// #ifdef _MSC_VER
  // sprintf(cbuf, "start %s %s",AP_browser,fNam);
// #else
  // // sprintf(cbuf, "%s file://%s 2>/dev/null &",AP_browser,fNam);
  // // "file://" - problems with "../fn"
  // sprintf(cbuf, "%s %s 2>/dev/null &",AP_browser,fNam);
// #endif
// 
  // printf("OS_browse_htm |%s|\n",cbuf);
  // OS_system(cbuf);
// 
  // TX_Print ("- display %s", fNam);
// 
  // return 0;

}
*/


//================================================================
  int APP_edit (char *filnam, int mode) {
//================================================================
// APP_edit                 <edit> <filnam>
// Input:
//   mode   0 - waits for end of process
//          1 - do not wait for end of process
//  
// see OS_edit_ ED_sysed__

  int   irc;
  long  l1;
  char  cbuf[256];



//   // test size of file filnam
//   if(OS_FilSiz(filnam) < 3) {
//     TX_Print("***** File %s is empty ..", filnam);
//     return -1;
//   }

#ifdef _MSC_VER
  sprintf(cbuf, "%s \"%s\"",AP_editor,filnam);
#else
  sprintf(cbuf, "%s %s",AP_editor,filnam);
#endif

    printf("APP_edit |%s|\n",cbuf);
  TX_Print("%s",cbuf);

  if(mode) {
    // do not wait
    OS_exec (cbuf);

  } else {
    // wait until completion.  TODO: use OS_edit__
    irc = OS_system (cbuf);
      // printf(" _edit-irc = %d\n",irc);
    if(irc) { TX_Print("**** editor not available ??"); }
  }

  return 0;

}


//================================================================
   int APP_Help (char *appNam, char *label) {
//================================================================
/// \code
/// call html-browser.
/// Input:
///   appNam    used for html-filename (use global var APP_act_nam)
///   label     is added to filename (html-chapter).
/// Example: 
///   appNam=PED; label="#chapter1"; AP_lang="en"
///   result: browse "<docdir>/html/PED_en.htm#chapter1"
/// see also IE_cad_help
/// \endcode


  char cbuf1[256];

  printf("APP_Help |%s|%s|%s|\n",appNam,label,AP_lang);

  // cbuf1 = "<docdir>/<app>_<lang>.htm"
  sprintf(cbuf1, "%shtml%c%s_%s.htm",
          AP_get_doc_dir(), fnam_del, appNam, AP_lang);


  // test if File exists;
  if(OS_checkFilExist(cbuf1,1) == 0) {
    // file does not exist;
    TX_Print(" - file %s does not exist; please help translating.", cbuf1);


    // check if engl.version exists
    sprintf(cbuf1, "%shtml%c%s_en.htm",
            AP_get_doc_dir(), fnam_del, appNam);

    if(OS_checkFilExist(cbuf1,1) == 0) {
      TX_Print(" - English version %s does not exist.",cbuf1);
      return -1;
    }

    TX_Print(" - using English version.");
  }

  if((label) && strlen(label)) OS_browse_htm (cbuf1, label);
  else OS_browse_htm (cbuf1, NULL);

  return 0;

}

/*
///================================================================
  int APP_Open (char *wTit, char *filter, void *funcNam) {
///================================================================
/// \code
/// APP_Open     select file from list, callback.
/// filter         eg "*" or "*.gcad"
/// prototype funcNam:
/// int funcNam ((char *filNam, char *dirNam);
/// \endcode

  int   i1;
  char  cbuf1[256], cbuf2[256];
  int  (*uFunc)();


//   MDLFN_syFn_f_name (cbuf1);   // get filename of dir.lst
//   // sprintf(cbuf1,"%sxa%cdir.lst",AP_get_bas_dir(),fnam_del);
// 
//   GUI_List2 (wTit,              // titletext
//             AP_mod_dir,        // Pfadname des activeDirectory
//             cbuf1,              // Liste der directories
//             funcNam);           // CallBack der Liste


  // Liste mit Dir-Auswahl
  i1 = AP_fnam_get_user_1 (0, cbuf1, cbuf2, wTit, filter);
  if(i1 < 0) return -1;

  uFunc = funcNam;
  return uFunc (cbuf1, cbuf2);

}
*/

///================================================================
  int APP_Save (char *wTit, char *defNam, void *funcNam) {
///================================================================
// funcNam           callBack save .. ??

  int   irc, (*uFunc)();
  char  fNam[256], dNam[256], dirLst[256], s1[400];


  printf("APP_Save |%s|%s|\n",wTit,defNam);

  MDLFN_syFn_f_name (dirLst);   // get filename of dir.lst
  // sprintf(cbuf1,"%sxa%cdir.lst",AP_get_bas_dir(),fnam_del);

/*
    GMDL_exp__ (wTit,              // titletext
            AP_mod_dir,           // path
            cbuf1,                 // directoryList
            defNam,                // defaultModelname
            funcNam);              // CallBack der Liste
*/

  strcpy(fNam, defNam);


  strcpy(dNam, AP_mod_dir);

//   irc = GUI_file_save__ (fNam, 256, dNam, 256, dirLst, wTit, "*");
//   if(irc) return 0;


                     // (filnamOut sSiz symDir filter title)
  irc = GUI_file_save__ (s1, sizeof(s1), dirLst, "*", wTit, 0);
    // printf("ex-GUI_file_save__ irc=%d fNam=|%s| dNam=|%s|\n",irc,fNam,dNam);
  if(irc < 2) return -1;
  if(strlen(fNam) < 1) return 0;


  // separate fNam,dNam
  UTX_fnam1__ (fNam, dNam, s1);

    // does already ask for overwrite !
    printf(" APP_Save %d |%s|%s|\n",irc,fNam,dNam);

  uFunc = funcNam;
  return uFunc (fNam, dNam);

}


//================================================================
  int APP_htm_fop (FILE **fpo) {
//================================================================
// APP_htm_fop            open a temp-file for a html-dumpFile

// close with UTX_htm_fcl(); display with OS_browse_htm (NULL);
// if used in plugin: use APP_htm_print to write into file
//   MS-Win does in a plugin not write into a file, opened from core !!

  char   cbuf1[256];


  // open temp. html-File
  sprintf(cbuf1, "%stmp.htm",AP_get_tmp_dir()); // TODO: use OS_get_tmp_dir
  printf("APP_htm_fop |%s|\n",cbuf1);


  return UTX_htm_fop (fpo, cbuf1);    // open

}


//================================================================
  int APP_htm_print (FILE *fpo, char *txt) {
//================================================================
// APP_htm_print          write into open htm-file
// MS-Win does in a plugin not write into a file, opened from core !!

  fprintf(fpo, "%s", txt);
  return 0;

}


//======================= EOF ====================================
