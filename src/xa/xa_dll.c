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
 OS_dll_build

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

DLL_run1         connect oder run oder unload DLL.
DLL_run2         build & connect & run & unload DLL.
DLL_unload       unload a dll (idle-callback)

List_functions_end:
=====================================================

See also:
OS_dll__
OS_dll_build      Compile, Link.
OS_debug_dll_

\endcode *//*----------------------------------------


*/




#ifndef _MSC_VER
// fuer gl.h erforderl !!
#include <dlfcn.h>           // Unix: dlopen
#else
#include "../xa/MS_Def1.h"         // f. HINSTANCE: windows.h !
#endif


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/func_types.h"             // FUNC_Pan FUNC_Rot FUNC_LOAD ..
#include "../ut/ut_os.h"               // OS_get_bas_dir ..
#include "../xa/xa.h"                  // AP_STAT






//===============================================================
// Externe Variablen:


/*
//================================================================
  int DLL_run2 (void **dll, void *fdat, int mode) {
//================================================================
/// Input:
///   mode       0 = load <dllNam>, start <fncNam>,  unload
///              1 = load <dllNam>, start <fncNam>
///              2 = start <fncNam>
///              2 = unload <dllNam>


  //----------------------------------------------------------------
  if 
*/

 
//================================================================
  int DLL_run1 (int mode, void *fdat) {
//================================================================
// DLL_run1         connect | run gCad_main(), gCad_fini() | unload DLL.
// starts ALWAYS function gCad_main in the dll !
// TODO: use DLL_run2
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
  char   ftyp[32], cBuf[256];

  static void  *dll1 = NULL; // pointer to loaded dll


  // printf("DLL_run1 %d\n",mode);
  // if(mode == 0) printf(" DLL_run1 0 |%s|\n",(char*)fdat);



  //----------------------------------------------------------------
  if(mode != 0) goto L_2;
  // mode = 0         OPEN DLL, connect Function "gCad_main"
  // Input: fdat is the filename of mockup-modelfile

  // extract filetype.
//   irc = UTX_ftyp_s (ftyp, (char*)fdat, 1);
//   if(irc < 0) {
//     TX_Print("***** DLL_run1 E1 |%s|FileType not found",ftyp); return -1;
//   }
// 
//   // change ftyp >lowercase
//   UTX_chg_2_lower (ftyp);
// 
// 
//   // fix DLL-FileName
// // #ifdef _MSC_VER
//   // sprintf(cBuf, "%s\\xa_%s_r.dll",OS_get_bin_dir(),ftyp);
//   sprintf(cBuf, "xa_%s_r",ftyp);
// // #else
//   // sprintf(cBuf, "%s/xa_%s_r.so",OS_get_bin_dir(),ftyp);
//   // sprintf(cBuf, "xa_%s_r",ftyp);
// // #endif
//     // printf(" soNam=|%s|\n",cBuf);


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


///===================================================================
  int DLL_run2 (char *soNam, int ccFlg) {
///===================================================================
/// activate & start gcad-plugin (gCad_main, gCad_fini only)
/// TODO: use OS_dll__
/// Input:
///   soNam     plugin; max 62 chars; including ".so" or ".dll"
///   ccFlg = 1 load plugin, do not recompile
///           0 recompile and load plugin
///          -1 unload plugin   (DLL_run2("",-1);



#ifdef _MSC_VER
  typedef int (__stdcall *dllFuncTyp01)();
  static HINSTANCE hdl1=NULL;
  dllFuncTyp01 dll_up1;


#else
  static void  *dl1 = NULL;
  void  (*up1)();
#endif

  char cbuf[256], *p1;



  // printf("DLL_run2 |%s| %d\n",soNam,ccFlg);




  //----------------------------------------------------------------
  // save name of active application -> APP_act_nam
  if(ccFlg >= 0) {
    if(strlen(soNam) > 64) {TX_Error("DLL_run2 E001"); return -1;}
    strcpy(cbuf, soNam);
    p1 = strchr(cbuf, '.');  // remove filetype
    if(p1) *p1 = '\0';
    else {TX_Error("DLL_run2 E002"); return -1;}
    strcpy(APP_act_nam, cbuf);


    // test if DLL-File exists
    sprintf(cbuf, "%splugins%c%s",OS_get_bin_dir(),fnam_del,soNam);
    if(OS_checkFilExist (cbuf, 1) == 0) {
      // does not exist:
      if(ccFlg > 0) {
        TX_Error("AP_exec_dll E001 |%s|",cbuf);
        return -1;
      }
    }


  }



//======= VERSION WINDOWS ========================================
#ifdef _MSC_VER


  // zuerst close DLL already in use ..
  if (hdl1 != NULL) {
    printf("unload ...\n");
    dll_up1 = (dllFuncTyp01) GetProcAddress (hdl1, "gCad_fini");
    dll_up1 ();         // call Func in Dll
    AP_User_reset();    // reset-funcs, die bei MS-Win u Linux gleich sind
    FreeLibrary(hdl1);
    if(ccFlg < 0) {
      TX_Print("plugin %s unloaded ..",APP_act_nam);
      // APP_act_nam[0] = '\0';
    }
    hdl1 = NULL;                                  // 2010-09-02

  } else {
    if(ccFlg < 0) {
      TX_Print("already unloaded ..");
    }
  }



  // -1 = unload plugin done ..
  if(ccFlg < 0) return 0;


  // 0 = compile
  if(ccFlg == 0) {
    // reOpen Messagefiles ..
    MSG_const_init (AP_lang);
    MSG_Init (AP_lang);
    if(OS_dll_build (soNam) != 0) {
       TX_Print("***** Error compile/link %s",soNam);
       return -1;
    }
  }



  // load DLL
  // strcpy(cbuf, soNam);
  // printf("  open |%s|\n",cbuf);
  hdl1 = LoadLibrary(cbuf);
  if (hdl1 == NULL) {
    TX_Error("cannot open dyn. Lib. %s",cbuf);
    return -1;
  }

  // damit Debugger stoppt, nachdem DLL geladen wurde
  p1 = strrchr(soNam, fnam_del);
  if(p1 == NULL) p1 = soNam;
  else ++p1;  // skip fnam_del
  OS_debug_dll_(p1);

  // display program = active
  // display name of plugin in label UIw_prg in red
  APP_act_typ = 3;
  UI_Set_typPrg ();
  UI_Set_actPrg (APP_act_nam, 2);  // display programName red
  AP_stat.APP_stat = 1;            // plugin is active ..

  // start gCad_main
  dll_up1 = (dllFuncTyp01) GetProcAddress (hdl1, "gCad_main");
  dll_up1 ();  // call Func in Dll




//======= VERSION UNIX ========================================
#else


  // zuerst close DLL already in use ..
  if(dl1 != NULL) {
      // printf("unload ...\n");
    if(up1=dlsym(dl1,"gCad_fini")) {   // Adresse von Func. "gCad_fini" holen
      (*up1)();               // gCad_fini must kill all open windows !!
      AP_User_reset();        // reset-funcs, die bei MS-Win u Linux gleich sind
      OS_dll_close (&dl1);    // unload DLL
      dl1 = NULL;
      if(ccFlg < 0) {
        TX_Print("plugin %s unloaded ..",APP_act_nam);
        // APP_act_nam[0] = '\0';
      }
    } else {
      TX_Error("cannot close dyn. Lib.");
      return -1;
    }

  } else {
    if(ccFlg < 0) {
      TX_Print("already unloaded ..");
    }
  }


  // -1 = unload plugin done ..
  if(ccFlg < 0) return 0;


  // 0 = compile
  if(ccFlg == 0) {
    // reOpen Messagefiles ..
    MSG_const_init (AP_lang);
    MSG_Init (AP_lang);
    if(OS_dll_build (soNam) != 0) {
       TX_Print("***** Error compile/link %s",soNam);
       return -1;
    }
  }


  // load DLL
  dl1=dlopen(cbuf,RTLD_LAZY);
  if(dl1 == NULL) {
    TX_Error("cannot open dyn. Lib. %s",cbuf);
    return -1;
  }

  // Adresse von Func."gCad_main" holen
  up1=dlsym(dl1,"gCad_main");
  if(up1 == NULL) {
    TX_Error("cannot open gCad_main");
    return -1;
  }

  // damit Debugger stoppt, nachdem DLL geladen wurde
  p1 = strrchr(soNam, fnam_del);
  if(p1 == NULL) p1 = soNam;
  else ++p1;  // skip fnam_del
  OS_debug_dll_(p1);


  // display program = active
  // display name of plugin in label UIw_prg in red
  APP_act_typ = 3;
  UI_Set_typPrg ();
  UI_Set_actPrg (APP_act_nam, 2);    // displ pluginName in red
  AP_stat.APP_stat = 1;              // plugin is active ..


  // start userprog
    // printf(" before userprog\n");
  (*up1)();
    // printf(" after userprog\n");

#endif
//======= END VERSION UNIX ========================================

  // AP_stat.APP_stat wird in gCad_fini -> AP_User_reset zurueckgesetzt !!
  // if(ccFlg >= 0) AP_stat.APP_stat = 1;     // plugin is active ..
  // else           AP_stat.APP_stat = 0;     // no plugin is active ..



  return 0;

}


/*
///===================================================================
  int OS_dll_build (char *dllNam) {
///===================================================================
/// wenn .mak vorh: compile/link

// dllNam   zB "xa_ige_r.so"   (ohne Pfad, mit Filetyp).

  int  irc;
  char cbuf[256];         // char cbuf[512];



  printf("OS_dll_build |%s|\n",dllNam);


#ifdef _MSC_VER
  //------------------------- MS-Windows ----------------------------------
  // sprintf(cbuf, "%sxa\\%s",OS_get_bas_dir(),dllNam);
  sprintf(cbuf, "%s..\\src\\APP\\%s",OS_get_loc_dir(),dllNam);
  // ".dll" -> ".nmak"
  strcpy(&cbuf[strlen(cbuf)-4], ".nmak");
    // printf(" exist: |%s|\n",cbuf);
  if(OS_checkFilExist (cbuf, 1) == 0) goto L_err_nof;


  TX_Print(".. compile .. link .. %s",dllNam);


  // sprintf(cbuf, "cd %sxa&&nmake -f %s",OS_get_bas_dir(),dllNam);
  sprintf(cbuf, "cd %s..\\src\\APP&&nmake -f %s",OS_get_loc_dir(),dllNam);

  // strcpy(&cbuf[strlen(cbuf)-4], ".nmak OS=");
  // strcpy(&cbuf[strlen(cbuf)-4], ".mak OS=");
  // strcat(cbuf, OS_os());

  strcpy(&cbuf[strlen(cbuf)-4], ".nmak OS=");
  strcat(cbuf, OS_get_os_bits());


#else
  //------------------------- Linux ----------------------------------
  // sprintf(cbuf, "%sxa/%s",OS_get_bas_dir(),dllNam);
  sprintf(cbuf, "%s../src/APP/%s",OS_get_loc_dir(),dllNam);
  // ".so" -> ".mak"
  strcpy(&cbuf[strlen(cbuf)-3], ".mak");
    printf(" exist: |%s|\n",cbuf);
  if(OS_checkFilExist (cbuf, 1) == 0) goto L_err_nof;


  TX_Print(".. compile .. link .. %s",dllNam);


  // sprintf(cbuf, "cd %sxa;make -f %s",OS_get_bas_dir(),dllNam);
  sprintf(cbuf, "cd %s../src/APP;make -f %s",OS_get_loc_dir(),dllNam);
    printf(" OS_dll_build 2 |%s|\n",cbuf);


  // ".so" -> ".mak"
  strcpy(&cbuf[strlen(cbuf)-3], ".mak OS=");
  strcat(cbuf, OS_get_os_bits());
    // printf(" .. cbuf1 2 |%s|\n",cbuf);

#endif


  // "make -f %s.mmak"
  printf("|%s|\n",cbuf);


  irc = system(cbuf);
  if(irc != 0) TX_Error("Error build %s",dllNam);

  return irc;

  L_err_nof:
    TX_Print("***** %s does not exist ..",cbuf);
    printf("***** %s does not exist ..\n",cbuf);

    return 0;

}
*/

//====================== EOF =============================
