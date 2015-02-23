// gCAD3D - APPLICATION-INTERFACE                    2008-01-21   Franz Reiter
/*
 *
 * Copyright (C) 2015 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
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

APP_start
APP_free               kernel-free; see APP_realloc
APP_realloc            kernel-realloc
APP_Close              ReEnable Ctrl Y
APP_Unl                unload plugin
APP_Help               simple help
APP_Open               select file from list, callback.
APP_Save               simple save
APP_browse
APP_edit
APP_htm_fop            open a temp-file for a html-dumpFile
APP_htm_print          write into open htm-file

List_functions_end:
=====================================================

\endcode *//*----------------------------------------

*/


#ifdef _MSC_VER
#include "MS_Def1.h"
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
#include "../ut/ut_os.h"               // OS_get_bas_dir ..

#include "../gr/ut_UI.h"               // UI_FuncUCB8

#include "../xa/xa_mem.h"              // memspc51, mem_cbuf1
#include "../xa/xa_uid.h"              // UID_ckb_comp
#include "../xa/xa.h"                  // AP_STAT



//===============================================================
// Externe Variablen:


//===============================================================
// Interne Variablen:



//==== prototypes: ===============================================

  // void  (*xa_test)();

 
/*
//================================================================
  int APP_dll_load (int mode) {
//================================================================
// mode == 0:  connect dynLib xa_test.so
// mode == 1:  unload, recompile, connect dynLib xa_test.so

// Leider kein Weg zum interpose (override active symbol) gefunden.
// see xa_test.c

  static void  *dl1 = NULL;

  int   irc;
  char  cBuf[256];


#ifdef _MSC_VER
  return 0;
#endif


  printf("APP_dll_load %d\n",mode);
 

  //----------------------------------------------------------------
  if(mode != 1) goto L_load;

  // unload dll
  if(dl1) {
    irc = dlclose(dl1);
    if(irc) {
      TX_Error("APP_dll_load: cannot close dyn. Lib.");
      return -1;
    }
  }

  // recompile
  sprintf(cBuf, "cd xa;make -f %sxa/xa_test.mak",OS_get_bas_dir());
  system (cBuf);
  // OS_spawn_wait (cBuf, 5);


  //----------------------------------------------------------------
  L_load:
  sprintf(cBuf, "%sbin/xa_test.so",OS_get_bas_dir());
    printf("   _dll_load |%s|\n",cBuf);


  // exit if dll does not exist;  else load DLL
  if(OS_checkFilExist(cBuf, 1) == 0) return 0;


  // load DLL
  // dl1 = NULL;
  // dl1 = dlopen(cBuf, RTLD_LAZY);   // linkt nicht !
  // dl1 = dlopen(cBuf, RTLD_NOW);
  dl1 = dlopen(cBuf, RTLD_LAZY|RTLD_GLOBAL);
  // dl1 = dlopen(cBuf, RTLD_NOW|RTLD_DEEPBIND|RTLD_GLOBAL);
  if(dl1 == NULL) {
    TX_Error("APP_dll_load: cannot open dyn. Lib. |%s|",cBuf);
    return -1;
  }


  // // Adresse von Func. holen
  // xa_test=dlsym(dl1,"xa_test");
  // if(xa_test == NULL) {
    // TX_Error("cannot open gCad_main");
    // return -1;
  // }



  printf("ex xa_test\n");

  return 0;

}
*/


//================================================================
  int APP_browse (char *filnam) {
//================================================================
/// \code
/// html-browse <filnam>
/// do not wait for end of process.
/// filnam = NULL    using default <temp>/temp.htm
///
/// see OS_browse_
/// \endcode

// "firefox file:///mnt/serv1/Devel/dev/gCAD3D/doc/CAD_AC_de.htm#F6"


  char  cbuf[280], s1[256], *fNam;


  if(filnam == NULL) {
    sprintf(s1, "%stmp.htm",OS_get_tmp_dir());
    fNam = s1;
  } else {
    fNam = filnam;
  }



#ifdef _MSC_VER
  sprintf(cbuf, "start %s %s",AP_browser,fNam);
#else
  sprintf(cbuf, "%s file://%s 2>/dev/null &",AP_browser,fNam);
#endif

  printf("APP_browse |%s|\n",cbuf);
  OS_system(cbuf);

  TX_Print ("- display help-file ..");

  return 0;

}


//================================================================
  int APP_edit (char *filnam) {
//================================================================
/// \code
/// <edit> <filnam>
/// waits for end of process.
///
/// see OS_edit_ ED_sysed__
/// \endcode

  char  cbuf[256];


#ifdef _MSC_VER
  sprintf(cbuf, "%s \"%s\"",AP_editor,filnam);      // 2013-10-05
#else
  sprintf(cbuf, "%s %s",AP_editor,filnam);      // 2013-10-05
#endif

  printf("APP_edit |%s|\n",cbuf);
  TX_Print("%s",cbuf);

  OS_system(cbuf);

  return 0;

}


//================================================================
  int APP_start () {
//================================================================
// start plugin <APP_act_nam>

  AP_exec_dll (APP_act_nam);

  return 0;

}

 
//================================================================
  int APP_free (void **ptr) {
//================================================================
/// see APP_realloc

  free (*ptr);
  *ptr = NULL;

  return 0;

}


//================================================================
  int APP_realloc (void **ptr, int siz) {
//================================================================
/// nur fuer MS-Win:
/// wenn Kernel ein malloc macht, muss auch das free und das realloc
/// ebenfalls im Kernel passieren !

  *ptr = realloc(*ptr, siz);

  return 0;

}


///================================================================
   int APP_Close () {
///================================================================
/// ReEnable Ctrl Y                   unused


  printf("APP_Close\n");

  // AP_stat.APP_act = 0;  // ReEnable Ctrl Y

  return 0;

}



//================================================================
   int APP_Unl () {
//================================================================
/// unload plugin <APP_act_nam>

  printf("APP_Unl \n");

  DLL_run2 ("", -1);  // unload plugin

  return 0;

}




//================================================================
   int APP_Help (char *APP_NAME, char *label) {
//================================================================
/// \code
/// call html-browser.
/// label    is added to filename.
/// Example: 
///   input:  APP_NAME=PED; label="#chapter1"; lang="en"
///   result: "/mnt/serv1/Devel/dev/gCAD3D/doc/PED_en.htm#chapter1"
/// see also IE_cad_help
/// \endcode


  char cbuf1[256];

  printf("APP_Help |%s|%s|%s|\n",APP_NAME,label,AP_lang);


  sprintf(cbuf1, "%shtml%c%s_%s.htm",
          OS_get_doc_dir(), fnam_del, APP_NAME, AP_lang);


  // test if File exists;
  if(OS_checkFilExist(cbuf1,1) == 0) {
    // file does not exist;
    TX_Print(" - file %s does not exist; please help translating.", cbuf1);


    // check if engl.version exists
    sprintf(cbuf1, "%shtml%c%s_en.htm",
            OS_get_doc_dir(), fnam_del, APP_NAME);

    if(OS_checkFilExist(cbuf1,1) == 0) {
      TX_Print(" - English version %s does not exist.",cbuf1);
      return -1;
    }

    TX_Print(" - using English version.");
  }

  strcat(cbuf1, label);

  APP_browse (cbuf1);

  return 0;

}


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

/*
  AP_get_fnam_symDir (cbuf1);   // get filename of dir.lst
  // sprintf(cbuf1,"%sxa%cdir.lst",OS_get_bas_dir(),fnam_del);

  GUI_List2 (wTit,              // titletext
            AP_dir_open,        // Pfadname des activeDirectory
            cbuf1,              // Liste der directories
            funcNam);           // CallBack der Liste
*/

  // Liste mit Dir-Auswahl
  i1 = AP_Mod_open (0, cbuf1, cbuf2, wTit, filter);
  if(i1 < 0) return -1;

  uFunc = funcNam;
  return uFunc (cbuf1, cbuf2);

}


///================================================================
  int APP_Save (char *wTit, char *defNam, void *funcNam) {
///================================================================

  int   irc, (*uFunc)();
  char  fNam[256], dNam[256], dirLst[256];


    AP_get_fnam_symDir (dirLst);   // get filename of dir.lst
    // sprintf(cbuf1,"%sxa%cdir.lst",OS_get_bas_dir(),fnam_del);
/*
    GUI_save__ (wTit,              // titletext
            AP_dir_save,           // path
            cbuf1,                 // directoryList
            defNam,                // defaultModelname
            funcNam);              // CallBack der Liste
*/

  strcpy(fNam, defNam);
  irc = GUI_file_save__ (fNam, 256, dNam, 256, dirLst, wTit, "*");
  if(irc) return 0;
    // does already ask for overwrite !
    printf(" APP_Save %d |%s|%s|\n",irc,fNam,dNam);




  uFunc = funcNam;
  return uFunc (fNam, dNam);

}


//================================================================
  int APP_htm_fop (FILE **fpo) {
//================================================================
// APP_htm_fop            open a temp-file for a html-dumpFile

// close with UTX_htm_fcl(); display with APP_browse (NULL);
// if used in plugin: use APP_htm_print to write into file
//   MS-Win does in a plugin not write into a file, opened from core !!

  char   cbuf1[256];


  // open temp. html-File
  sprintf(cbuf1, "%stmp.htm",OS_get_tmp_dir());
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
