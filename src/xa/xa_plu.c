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
Plugin       (C-dll)        APP    ../xa/xa_plu.c ../xa/xa_appDat.c ../xa/xa_dll.c
  Common funcs and vars for Application/Process/Plugin: ../xa/xa_app.c


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

PLU_start
PLU_free               kernel-free; see PLU_realloc
PLU_realloc            kernel-realloc
PLU_close              ReEnable Ctrl Y
PLU_unl                unload plugin
PLU_Loa                reStart remote
PLU_restart            exec plugin
PLU_appNamTab_set      provide names for application-objects
PLU_appNamTab_get      return name of AppNam[iNam]
PLU_oid_appNam      create name (oid) for application-object

List_functions_end:
=====================================================

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
#include "../ut/ut_os.h"               // OS_get_bas_dir ..
#include "../ut/func_types.h"               // UI_FuncUCB8
#include "../ut/ut_memTab.h"           // MemTab

#include "../xa/xa_mem.h"              // memspc51, mem_cbuf1
#include "../xa/xa_uid.h"              // UID_ckb_comp
#include "../xa/xa.h"                  // AP_STAT



//===============================================================
// Externe Variablen:
// defined in ../xa/xa.c (with extern invalidated)



//===============================================================
// Interne Variablen:
// table of user-defined names of application-objects
APP_OBJ_NAM *UI_User_appNamTab = NULL;     // appObjNamTab



//==== prototypes: ===============================================

  // void  (*xa_test)();

 

//================================================================
  int PLU_start () {
//================================================================
// start plugin <APP_act_nam>

  AP_exec_dll (APP_act_nam);

  return 0;

}

 
//================================================================
  int PLU_free (void **ptr) {
//================================================================
/// see PLU_realloc

  free (*ptr);
  *ptr = NULL;

  return 0;

}


//================================================================
  int PLU_realloc (void **ptr, int siz) {
//================================================================
/// nur fuer MS-Win:
/// wenn Kernel ein malloc macht, muss auch das free und das realloc
/// ebenfalls im Kernel passieren !

  *ptr = realloc(*ptr, siz);

  return 0;

}


///================================================================
   int PLU_close () {
///================================================================
/// ReEnable Ctrl Y                   unused


  printf("PLU_close\n");

  // AP_stat.APP_act = 0;  // ReEnable Ctrl Y

  return 0;

}



//================================================================
   int PLU_unl () {
//================================================================
/// unload plugin <APP_act_nam>

  printf("PLU_unl \n");

  DLL_run2 ("", -1);  // unload plugin

  return 0;

}


//================================================================
  int PLU_restart () {
//================================================================
// exec plugin

  if(APP_act_typ != 3) {
    TX_Error("RPC_restart - active prog must be plugin ..");
    return -1;
  } 
    

  // start selected plugin
  sprintf(memspc011, "Execute Userprog. %s",APP_act_nam);
  TX_Print(memspc011);

  UTX_ftyp_cut  (APP_act_nam);     // remove the filetyp (.so|.dll)

  // UNDO_app__ (0);            // init undo (get act.lNr)
  AP_exec_dll (APP_act_nam); // exec dll
  // UNDO_app__ (1);            // create undo-record; activate undo-button

  return 0;

}


//================================================================
  int PLU_Loa () {
//================================================================
/// \code
/// reStart remote
/// display list of dll-files in directory <bindir>/plugins;
/// execute selected plugin
/// \endcode
// was UI_DllLst_CB 

  int   i1;
  char  fnam[256], s1[256];


  printf("PLU_Loa \n");


  // no addOn-prog may be active.
  if(AP_stat.APP_stat != 0) {
    TX_Print ("***** disactivate active application / plugin ..");
    return -1;
  }




  // display list of plugins (see AP_DllLst_write ) let user select
  sprintf(fnam, "%splugins.lst", OS_get_tmp_dir());
//   i1 = GUI_list1_dlg_w (s1, 256,
//                        NULL, " select program", fnam,
//                        "1", NULL, "60,40");

  i1 = GUI_listf1__ (s1, sizeof(s1), fnam, "\"select program\"", "\"x40,y40\"");
  if(i1 < 0) return -1;

  UTX_ftyp_cut  (s1);     // remove the filetyp (.so|.dll)

  APP_act_typ = 3;
  UI_Set_typPrg ();
  strcpy(APP_act_nam, s1);
  UI_Set_actPrg (APP_act_nam, 2);

  PLU_restart ();

  return 0;

}


//================================================================
  char* PLU_appNamTab_get (int iNam) {
//================================================================
/// \code
/// return name of AppNam[iNam]
/// (see PLU_appNamTab_set)
/// \endcode

  int  ii = 0;

  while (UI_User_appNamTab[ii].oTyp >= 0) {
    if(UI_User_appNamTab[ii].oTyp == iNam) return UI_User_appNamTab[ii].oNam;
    ++ii;
  }
  return NULL;
}


//================================================================
  int PLU_appNamTab_set (APP_OBJ_NAM *appObjNamTab) {
//================================================================
/// \code
/// provide names for application-objects
///   appObjNamTab must be static until exit of userApplication !
/// size of words max 32 chars
/// \endcode


  // printf("PLU_appNamTab_set \n");
  // { int   i1; i1 = 0; for(;;) {
    // if(appObjNamTab[i1].oTyp < 0) break;
      // printf(" appObjNamTab[%d] |%s| %d\n",i1,
             // appObjNamTab[i1].oNam,appObjNamTab[i1].oTyp); ++i1;} }


  UI_User_appNamTab = appObjNamTab;     // appObjNamTab

  return 0;

}

//================================================================
  int PLU_oid_appNam (char *oid, long dli) {
//================================================================
// PLU_oid_appNam       create name (oid) for application-object

  char    *p1;
  DL_Att  dla;


  // get DL-record from dispListIndex dli
  DL_dlRec__dli (&dla, dli);


  if(UI_User_appNamTab) { // display user-defined name of application-object
    p1 = PLU_appNamTab_get (dla.iatt);
    sprintf(oid, "%s %ld",p1,dla.ind);

  } else {               // default-name of application-object
    sprintf(oid, "%ld/%d",dla.ind,
                           dla.iatt);
  }

  return 0;

}




//======================= EOF ====================================
