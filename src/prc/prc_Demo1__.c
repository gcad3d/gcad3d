// gCAD3D Processor Demo1
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
2013-03-10 Rewritten from ../wc/wcut.c. RF.

-----------------------------------------------------
*/
/*!
\file  ../prc/prc_Demo1__.c
\brief  processes main
\code
=====================================================
List_functions_start:

PRCE__              main entry

List_functions_end:
=====================================================

\endcode *//*----------------------------------------

*/



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                   // va_list

#ifdef _MSC_VER
#define _CRT_SECURE_NO_DEPRECATE
__declspec(dllexport) int PRCE__ (int iFnc, char*);
#define extern __declspec(dllimport)
#endif


// #include "../ut/ut_umem.h"         // UME_reserve
#include "../ut/ut_geo.h"
#include "../ut/ut_txt.h"
#include "../ut/ut_TX.h"
#include "../ut/ut_txTab.h"            // UtxTab
#include "../ut/ut_os.h"               // OS_
#include "../ut/ut_ox_base.h"          // OGX_SET_INDEX

#include "../xa/xa_mem.h"              // memspc101
#include "../xa/xa_ui.h"               // APF_*
#include "../xa/xa_msg.h"              // MSG_*
#include "../xa/xa.h"                  // APP_act_*
#include "../xa/xa_ato.h"              // ATO_getSpc_tmp__

#include "../ut/func_types.h"               // SYM_TRIANG

#include "../prc/prc_Demo1__.h"        // NCCmdTab



//================================================================
// EXTERN




//================================================================
// LOCAL

#define cmd_SIZ 100
int     cmd_anz;
int     *cmd_typ;
double  *cmd_tab;


Point actPos;


// LOCAL PROTOTYPES:



//===========================================================================
  int PRCE__ (int iFnc, char* data)  {
//===========================================================================
/// PRCE__              main entry
// the actual position is oldPos.

/// Input:
///   iFnc     >=0 index into NCCmdTab
///            -1  function
///   data     additional text for nc-func NCCmdTab[iFnc]


  int       i1;
  Point     pt1;
  ObjAto    ato1;


  printf("----------- Demo1 ------------------- \n");
  if(iFnc>=0)printf("PRCE__ |%s|%s| %d\n",NCCmdTab[iFnc],data,iFnc);
  else printf("PRCE__ %d |%s|\n",iFnc,data);


  // function ?
  if(iFnc == -1) return PRCE_func__ (data);


  // no, command ..
  // commands without parameters
  // ..

  // decode input
  // cmd_anz = APT_decode_ausdr (cmd_typ, cmd_tab, cmd_SIZ, &data);
  ATO_getSpc_tmp__ (&ato1, cmd_SIZ);
  ATO_ato_srcLn__ (&ato1, data);
  cmd_anz = ato1.nr;
  cmd_typ = ato1.typ;
  cmd_tab = ato1.val;

    for(i1=0; i1<cmd_anz; ++i1)
    printf(" %d typ=%d tab=%f\n",i1,cmd_typ[i1],cmd_tab[i1]);



  switch (iFnc) {

    //================================================================
    case 0:   // FROM                          was APT_work_NCCmd
      i1 = APT_decode_pt1 (&actPos, 0, cmd_typ, cmd_tab);
      if(i1 < 0) goto L_err_par;
      DEB_dump_obj__ (Typ_PT, &actPos, " pt-rel ");
      break;


    //================================================================
    case 2:    // GO
      break;


    //================================================================
    case 3:    // RP           was APT_wcut_line
    case 4:    // RPR
      break;


    //================================================================
    default: 
      TX_Error("PRCE__ E001");
      return -1;

  }



  L_exit:
    printf("exit PRCE__ ..\n\n");

  return 0;



  L_err_par:
      TX_Error("PRCE__ E002 cannot use parameters ..");
      return -1;
    

}


//===========================================================================
  int PRCE_func__ (char* data)  {
//===========================================================================
// was APT_Init, Pock_Init__

  int   i1;


  printf("PRCE_func__ |%s|\n",data);


  //----------------------------------------------------------------
  if(!strncmp(data, "INIT__", 6)) {

    // report cmdTab
    PRC_set_CmdTab (NCCmdTab);

    TX_Print ("**** Demo1: Exit this process with \"VWR\"..");


  //----------------------------------------------------------------
  // MBR_<returncode_of_ED_query_CmdMode>
  // <-1 ignore line
  // -1  empty line; 
  // >=0 partially filled line
  } else if(!strncmp(data, "MBR_", 4)) {
    i1 = atoi (&data[4]);
      // printf(" mbr-i1=%d\n",i1);
    // provide popup-menu ..
    UI_EdKeyCR (2);           // imply OK


  //----------------------------------------------------------------
  } else if(!strncmp(data, "EXIT__", 6)) {
    // reset CmdTab ..
    PRC_set_CmdTab (NULL);


  //----------------------------------------------------------------
  } else if(!strncmp(data, "RESET ", 6)) {


  //----------------------------------------------------------------
  } else {
    TX_Error("PRCE_func__ E001"); return -1;
  }



  return 0;

}

// EOF
