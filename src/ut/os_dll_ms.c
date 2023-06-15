// os_dll_ms.c
/*
 *
 * Copyright (C) 2023 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
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
Load dynamic library; start function; disconnect; rebuild 

-----------------------------------------------------
TODO:
  ..

-----------------------------------------------------
Modifications:
2023-04-06 extracted from ../ut/ut_os_aix.c. RF.


=====================================================
List_functions_start:

DLL_dyn__        dynamic-dll - load,connect,exec,unload
DLL_dyn_close    dynamic-dll - unload
DLL_build        (re)build dll (compile/link with .mak)
OS_debug_dll_    enable stop for debuuger

List_functions_end:
=====================================================


*/






#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                   // for ...
#include <ctype.h>                    // isdigit

#include <signal.h>
#include <errno.h>

#include "../xa/MS_Def1.h"

#include "../xa/xa_msg.h"           // ERR_*



#include "../ut/os_dll.h"      // DLL_LOAD_only DLL_CONNECT DLL_EXEC ..


static HINSTANCE  dl1 = NULL;

char* OS_bin_dir_get();            // get directory with binaries (*.dll ..)




//================================================================
  int DLL_dyn__ (void **dll__, int mode, void *data) {
//================================================================
// DLL_dyn__        dynamic-dll - load,connect,exec,unload
// Input:
//   mode    DLL_LOAD_only|DLL_LOAD_all  load dll;
//             data - dllName (without directory, filetyp)
//           DLL_CONNECT                     connect function
//             data - name of function
//           DLL_EXEC                        exec function
//             data - parameters of function
//           DLL_UNLOAD                          unload dll
// Output:
//   dll__   pointer to dll; NULL=no-dll-connected
//   retCode >=0 is OK; <0 is error
//
// was OS_dll_do OS_dll__ DLL_run2 DLL_run1

 
static void   *dllAct = NULL;
static int    (*up1)();

  int    irc = 0;
  char   s1[512];


  // printf("DLL_dyn__ %d\n",mode);


  //----------------------------------------------------------------
  // open (load Lib fNam) 
  if((mode == DLL_LOAD_only)  ||
     (mode == DLL_LOAD_all))      {

    // test if a dll is loaded; yes: unload
    if(*dll__) {
      DLL_dyn_close (dll__);
      if(*dll__ != NULL) {     // TX_Error("OS_dll__: close dll .. ");
        TX_Error("***** ERROR 2 DLL_dyn__: close dll .. \n");
        return -1;
      }
    }

    // get full dll-filename
#if defined _MSC_VER || __MINGW64__
    sprintf(s1, "%s%s.dll",OS_bin_dir_get(),(char*)data);
#else
    sprintf(s1, "%s%s.so",OS_bin_dir_get(),(char*)data);
#endif
       // printf(" DLL_LOAD |%s|\n",s1); fflush(stdout);

    // load DLL
    *dll__ = (HINSTANCE) LoadLibrary (s1);
    if(*dll__ == NULL) {
      TX_Error("***** ERROR 3 DLL_dyn__: cannot open dyn. Lib. |%s|",(char*)data);
      return -1;
    }
    dllAct = *dll__;
    up1 = NULL;

    // stop Debugger after DLL has been loaded
    OS_debug_dll_((char*)data);

    goto L_exit;
  }


  //----------------------------------------------------------------
  // connect function
  if(mode == DLL_CONNECT) {
    // printf(" DLL_CONNECT  |%s|\n",(char*)data);

    // get adress of Function
    up1 = (void*) GetProcAddress (*dll__, (char*)data);
    if(up1 == NULL) {
      TX_Error("***** ERROR 4 DLL_dyn__: cannot connect Func. |%s|",(char*)data);
      DLL_dyn_close (*dll__);             // unload DLL
      return -1;
    }
  }


  //----------------------------------------------------------------
  // exec function
  if(mode == DLL_EXEC) {
    // printf(" DLL_EXEC\n");
  
    // start userprog
    if(up1) {
      irc = (*up1)(data);
      if(irc < 0) goto L_exit;
    } else {
      TX_Error("***** ERROR 5 DLL_dyn__: no func defined.");
      DLL_dyn_close (*dll__);             // unload DLL
      return -1;
    }
  }


  //----------------------------------------------------------------
  // unload DLL
  if(mode == DLL_UNLOAD) {
    irc = DLL_dyn_close (dll__);
    dllAct = NULL;
    *dll__ = NULL;
  }


  //----------------------------------------------------------------
  L_exit:
      // printf(" ex-DLL_dyn__ %d\n",irc);
    return irc;

}


//================================================================
  int DLL_dyn_close (void **dl1) {
//================================================================
// on successfule returns irc = 0,  dl1 = NULL
// BUG dlclose: returnCode is OK but handle not NULL.

  int  irc = 0;

  // printf("DLL_dyn_close \n");

  // unload if already loaded
  if(*dl1 != NULL) {
    irc = FreeLibrary (*dl1);  // 0=Error
    if (irc) {
      *dl1 = NULL;
      irc = 0;
    } else {
      TX_Error("***** ERROR DLL_dyn_close");
      irc = -1;
    }
  }

  return irc;

}


//===================================================================
  int DLL_build (char *dllNam) {
//===================================================================
// DLL_build             (re)build dll (compile/link with .mak)
// Input:
//   dllNam   plugin: eg "Demosimple"    (without path, without Filetyp).
//            - using xa_ige_r.mak for build

  int  irc;
  char *sdir, s1[512], makNam[256];         // char cbuf[512];



  // printf("DLL_build |%s|\n",dllNam);


  // get the directory ${gcad_dir_dev}src/APP
  sdir = getenv ("gcad_dir_dev");
  if(!sdir) {
    TX_Print("***** DLL_build - cannot find direcory gcad_dir_dev ...");
    return -1;
  }

  // makNam = add ".mak"
  strcpy(makNam, dllNam);
  // strcpy(&makNam[strlen(dllNam)-3], ".mak");
  strcat(makNam, ".mak");

  // plugins-only:
  sprintf(s1, "%s%s",sdir,makNam);
    printf(" ck-exist: |%s|\n",s1);


  // check if makefile exists
  if(!OS_checkFilExist (s1, 1)) {
    TX_Print("***** file %s does not exist - cannot rebuild",s1);
    return -1;
  }


  TX_Print(".. compile .. link .. %s",s1);


  // sprintf(cbuf, "cd %sxa;make -f %s",AP_get_bas_dir(),dllNam);
  // sprintf(cbuf, "cd %s../src/APP;make -f %s",AP_get_loc_dir(),dllNam);
  sprintf(s1, "cd %s && make -f %s", sdir, makNam);
    printf(" DLL_build - do |%s|\n",s1);

  // compile, link ..
  irc = system(s1);
  if(irc != 0) {
    TX_Error("Error build %s",makNam);
    irc = -1;
  }

    // printf(" ex-DLL_build %d\n",irc);

  return irc;

}


// //================================================================
//   int OS_dll_unload_idle (void *data) {
// //================================================================
// // DLL_unload       unload a dll (idle-callback)
// 
// 
//   printf("OS_dll_unload_idle |%s|\n",(char*)data);
// 
// return MSG_ERR__ (ERR_TODO_I, "OS_dll_unload_idle");
// 
// //   // DLL_dyn_close_fn ((char*)data);
// //   OS_dll_do ((char*)data, NULL, NULL, 2);
// 
//   return 0;   // must be 0 - removes idle-Call
// 
// }


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


// eof

/*
//================================================================
  int OS_dll__ (void **dl1, int mode, void *fDat) {
//================================================================
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


  // typedef int (__stdcall *dllFuncTyp01)(void*);
  // static dllFuncTyp01 dll_up1;

//  static HINSTANCE    hdl1=NULL;
  static void*        (*dll_up1)();

  int   irc = 0;
  unsigned long  lrc = 123;
  char  s1[256], *p1;


  printf("OS_dll__ %d\n",mode);


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
    // sprintf(s1, "%s%s.dll",AP_get_bin_dir(),(char*)fDat);
    sprintf(s1, "./%s.dll",(char*)fDat);
      printf(" dll=|%s|\n",s1); fflush(stdout);

    *dl1 = LoadLibrary (s1);
    if (*dl1 == NULL) {
      TX_Error("OS_dll__: cannot open dyn. Lib. |%s|",(char*)fDat);
      return -1;
    }
    dll_up1 = NULL;


  //----------------------------------------------------------------
  // mode 1 = connect (connect Func fDat)
  } else if(mode == FUNC_CONNECT) {

    // Adresse von Func.fncNam holen
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
      (*dll_up1) (fDat);

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
      // FreeLibraryAndExitThread (*dl1, lrc);           // unload DLL
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


    printf("ex-OS_dll__ mode=%d irc=%d\n",mode,irc);

  return 0;

}
*/

// eof
