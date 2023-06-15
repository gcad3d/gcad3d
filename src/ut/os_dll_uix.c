//  os_dll_uix.c
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


see:
../xa/export.h

../ut/os_uix.c       basic OS-functions
../ut/ut_os__.c      OS-independant functions

*/


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                   // for ...
#include <ctype.h>                    // isdigit

#include <signal.h>
#include <errno.h>

#include <unistd.h>
#include <dlfcn.h>           // Unix: dlopen


#include "../ut/ut_os.h"       // OS_get_env
#include "../ut/os_dll.h"      // DLL_LOAD_only DLL_CONNECT DLL_EXEC ..


static void  *dl1 = NULL;

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

  int    irc;
  char   s1[512];


  // printf("DLL_dyn__ %d\n",mode);


//   if(*dll__ != dllAct) {
//     printf(" *****  disconnect active dll *****\n");
//     irc = DLL_dyn_close (dll__);
//     if(irc < 0) {
//       TX_Error("***** ERROR 1 DLL_dyn__: close dll .. \n");
//       return -1;
//     }
//   }


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
    sprintf(s1, "%s%s.so",OS_bin_dir_get(),(char*)data);

    // load DLL
    if(mode == DLL_LOAD_only) {
      // load funcs with DLL_CONNECT
      *dll__ = dlopen (s1, RTLD_LAZY);
         printf(" DLL_LOAD_only |%s|\n",s1); fflush(stdout);

    } else {
      // load all funcs in dll
      *dll__ = dlopen (s1, RTLD_LAZY|RTLD_GLOBAL);
      // CANNOT close recompile reOpen this dll !!
      // *dll__ = dlopen (s1, RTLD_NOW|RTLD_GLOBAL);
      // *dll__ = dlopen (s1, RTLD_NOW|RTLD_GLOBAL|RTLD_DEEPBIND);
         printf(" DLL_LOAD_all |%s|\n",s1); fflush(stdout);
    }

    if(*dll__ == NULL) {
      TX_Error("***** ERROR 3 DLL_dyn__: cannot open dyn. Lib. |%s|",(char*)data);
      return -1;
    }
    dllAct = *dll__;
    up1 = NULL;
    irc = 0;

    // stop Debugger after DLL has been loaded
    OS_debug_dll_((char*)data);

    goto L_exit;
  }


  //----------------------------------------------------------------
  // connect function
  if(mode == DLL_CONNECT) {
    printf(" DLL_CONNECT  |%s|\n",(char*)data);

    // get adress of Function
    up1 = dlsym (*dll__, (char*)data);
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


  // unload if already loaded
  if(*dl1 != NULL) {
      printf("DLL_dyn_close \n");
    irc = dlclose (*dl1);  // 0=success
    if(!irc) {
      *dl1 = NULL;
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
//   dllNam   plugin: eg "Demosimple"   (without path, without Filetyp).
//            - using xa_ige_r.mak for build

  int  irc;
  char *sdir, s1[512], makNam[256];         // char cbuf[512];



  printf("DLL_build |%s|\n",dllNam);


  // get the directory ${gcad_dir_dev}src/APP
  sdir = OS_get_env ("gcad_dir_dev");
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

    printf(" ex-DLL_build %d\n",irc);

  return irc;

}


// //================================================================
//   int OS_dll_unload_idle (void *data) {
// //================================================================
// // DLL_unload       unload a dll (idle-callback)
// 
// 
//   // printf("OS_dll_unload_idle |%s|\n",(char*)data);
// 
//   // DLL_dyn_close_fn ((char*)data);
//   OS_dll_do ((char*)data, NULL, NULL, 2);
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
