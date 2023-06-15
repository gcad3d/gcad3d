//     DLL-UtilityProgs                      Reiter Franz 2005-10-20
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
Noch hierher kopieren:
 DLL_run2
 DLL_build

-----------------------------------------------------
Modifications:
  ..

-----------------------------------------------------
*/
/*!
\file  ../xa/xa_dll.c
\brief DLL-UtilityProgs  OS-indep.
\code
=====================================================
List_functions_start:

DLL_plu_unl      unload user-plugin
DLL_plu__        (re)start user-plugin
DLL_plu_exec     start function in active plugin

List_functions_end:
=====================================================

See also:
OS_dll__
DLL_build      Compile, Link.
OS_debug_dll_

\endcode *//*----------------------------------------


*/




#if defined _MSC_VER || __MINGW64__
#include <windows.h>
#else
// Linux:
// fuer gl.h erforderl !!
#include <dlfcn.h>           // Unix: dlopen
#endif


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/func_types.h"             // FUNC_Pan FUNC_Rot FUNC_LOAD ..
#include "../ut/ut_os.h"               // AP_get_bas_dir ..
#include "../ut/os_dll.h"      // DLL_LOAD_only DLL_CONNECT DLL_EXEC ..

#include "../xa/xa.h"                  // AP_STAT


void *ptr_PLU = NULL;


//===============================================================
// Externe Variablen:


//================================================================
  int DLL_plu_fini () {
//================================================================
// DLL_plu_unl        unload user-plugin

  int     irc;

  printf("DLL_plu_fini \n");

  if(ptr_PLU) {
    // close plugin
    irc = DLL_dyn__ (&ptr_PLU, DLL_CONNECT, "gCad_fini");
    if(irc < 0) return -1;
    irc = DLL_dyn__ (&ptr_PLU, DLL_EXEC, NULL);
    if(irc < 0) return -1;
  }

  return 0;

}


//================================================================
  int DLL_plu_unl () {
//================================================================
// DLL_plu_unl        unload user-plugin

  int     irc;

  printf("DLL_plu_unl \n");

  if(ptr_PLU) {
    // close plugin
    irc = DLL_plu_fini ();
    if(irc < 0) return -1;
    // unload active dll
    irc = DLL_dyn__ (&ptr_PLU, DLL_UNLOAD, NULL);
    if(irc < 0) return -1;
    AP_stat.APP_stat = 0;
  }

  return 0;

}


//================================================================
  int DLL_plu_exec (char *func, void *data) {
//================================================================
// DLL_plu_exec         start function in active plugin
 
  int    irc;

  printf("DLL_plu_exec |%s|\n",func);


  irc =  DLL_dyn__ (&ptr_PLU, DLL_CONNECT, func);
  if(irc < 0) return -1;

  return DLL_dyn__ (&ptr_PLU, DLL_EXEC, data);

}


//================================================================
  int DLL_plu__ (char *dllNam, void *data) {
//================================================================
// DLL_plu__        (re)start user-plugin
// Input: dllNam              without directory, filetyp
// was DLL_run2 OS_dll__

// APP_act_typ=3 (plugin)
// APP_act_nam=dllNam

  int    irc, iNew, iComp, (*up1)();
  char   s1[256];

static char actNam[128] = "abc";


  printf("DLL_plu__ |%s|%s|\n",dllNam,actNam);


  if(strlen(dllNam) >= 128) {
    TX_Error("DLL_plu__ - plugin-name too long");
    goto L_exit;
  }


  // test if reBuild wanted
  iComp = AP_ck_build ();  // 1=yes, else no
 

  // compare if function already active; 0=ident; 1=diff.
  iNew = strcmp(dllNam, actNam);
    printf(" DLL_plu__ iComp=%d iNew=%d\n",iComp,iNew);


  // close dll already in use  if reBuilt or other plugin
  if(ptr_PLU) {
    if((iComp > 0) || iNew || AP_stat.APP_stat) {
      // unload user-plugin (exec "gCad_fini" and unload)
      irc = DLL_plu_unl ();
      if(irc < 0) goto L_exit;
    }
  }


  // rebuild
  if(iComp > 0) {
    irc = DLL_build (dllNam);
    if(irc < 0) goto L_exit;
  }


  // load dll if rebuilt or new
  if(ptr_PLU == NULL) {
    sprintf(s1, "plugins%c%s", fnam_os_del, dllNam);  // fnam_os_del='/' or '\'
    irc = DLL_dyn__ (&ptr_PLU, DLL_LOAD_only, s1);
    if(irc < 0) return -1;
    strcpy(actNam,dllNam);
    AP_stat.APP_stat = 1;
    UI_Set_actPrg (NULL, 2);    // set color=activated after rebuild
  }


  // connect function gCad_main() exec
  irc = DLL_dyn__ (&ptr_PLU, DLL_CONNECT, "gCad_main");
  if(irc < 0) goto L_exit;
  irc = DLL_dyn__ (&ptr_PLU, DLL_EXEC, data);



  L_exit:
      printf(" ex-DLL_plu__ %d\n",irc);

    return irc;

}


/*
//================================================================
  int DLL_run1 (int mode, void *fdat) {
//================================================================
// DLL_run1         cad-kernel extension; connect,run gCad_main(), unload.
// 
// Input:
//   mode    0 Load DLL;  connect dll-Function "gCad_main"
//   mode    2 work; start Function "gCad_main" with (ObjGX*)fdat
//             ObjGX[0] = Typ_ObjGX
//             ObjGX[1] = (int)mode = 1
//             ObjGX[2] = (char*)filename of input (mockup-model)
//             ObjGX[3] = (Typ_Memspc)Typ_Memspc (output)
//   mode    3 unLoad / free    fdat=Memspc
//   fdat    mode = 0: dll-name (xa_wrl_r xa_vr2_r xa_stl_r xa_obj_r)
//           mode = 2: function



  int    irc, mtyp;
  char   ftyp[32], cBuf[SIZFNam];

  static void  *dll1 = NULL; // pointer to loaded dll


  printf("DLL_run1 %d\n",mode);
  // if(mode == 0) printf(" DLL_run1 0 |%s|\n",(char*)fdat);



  //----------------------------------------------------------------
  if(mode != 0) goto L_2;
  // mode = 0         OPEN DLL, connect Function "gCad_main"
  // Input: fdat is the filename of mockup-modelfile

  // connect DLL..
  if(&dll1) {
    irc = OS_dll__ (&dll1, FUNC_LOAD_only, fdat);
    if(irc < 0) return irc;
  }


  // connect function
  irc = OS_dll__ (&dll1, FUNC_CONNECT, (void*)"gCad_main");
  if(irc < 0) return irc;
  return 0;


  //----------------------------------------------------------------
  L_2:      // (mode == FUNC_EXEC)      EXECUTE
  if(mode != 2) goto L_3;
  // mode = 2  - start connected Function with (ObjGX*)fdat

  // execute active function
  irc = OS_dll__ (&dll1, FUNC_EXEC, fdat);
    // printf(" after OS_dll__ %d\n",irc);
  if(irc < 0) return irc;

  return 0;


  //----------------------------------------------------------------
  L_3:     // (mode == FUNC_UNLOAD)       UNLOAD
  // mode = 3  - unload DLL
  irc = OS_dll__ (&dll1, FUNC_UNLOAD, NULL);
  if(irc < 0) return irc;

  return 0;

}
*/

//====================== EOF =============================
