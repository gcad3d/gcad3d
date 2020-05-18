// processes, plugins, remoteProcesses.
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
Process      (script+dll)   PRC    ../xa/xa_proc.c
  Common funcs and vars for Application/Process/Plugin: ../xa/xa_app.c


TODO:
  ..

-----------------------------------------------------
Modifications:
2013-03-06 New. RF.

-----------------------------------------------------
*/
#ifdef globTag
void PRC(){}
#endif
/*!
\file  ../xa/xa_proc.c
\brief processes, plugins, remoteProcesses 
\code
=====================================================
List_functions_start:

PRC_init
PRC__
PRC_Loa
PRC_disactivate__
PRC_activate__
PRC_Cre__
PRC_Cre_ui__
PRC_lst_processes
PRC_Cre_cre
PRC_Del__
PRC_del_file
PRC_lst_processors
PRC_Help
PRC_set_CmdTab

RPC_Loa

List_functions_end:
=====================================================
see also UI_Set_actPrg UI_Set_typPrg
see also ../xa/xa_prg.c

\endcode *//*----------------------------------------



FileFormat:
SECTION PROCESS <processname> < processorname>
PROCESS <processname> < processorname>
..
ENDSECTION
    processname:    APP_act_nam
    processorname:  cut1     (PRC_cut1.so; PRC_name) Manuf(NC-Proc.).,Robotic
    parameters:     eg name of postprocessor (PPRC_name)

Files:
  <tempDir>cadprocessors.lst           the list of processors
  <binDir>plugins/PRC_*.so             the processors ..
  <binDir>plugins/<processorname>/     directory postprocessors ..
    processorname without "PRC_"
  <tempDir>process_<processname>       the processes



AP_stat.APP_stat  // 0=core-active, 1=addOn-prog-active
APP_act_typ       // 2=process
APP_act_nam       // processname
APP_act_proc      // name of processor (dll)

    
*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>             // va_list



//================================================================
// EXTERN:

#include "../ut/ut_umb.h"                  // UMB_pos__
#include "../ut/ut_geo.h"                  // Typ_Memspc
#include "../ut/ut_txt.h"                   // fnam_del
#include "../ut/ut_os.h"                    // OS_get_bin_dir
#include "../ut/ut_memTab.h"           // MemTab

#include "../gui/gui__.h"

#include "../xa/xa_ui.h"          // UI_but__
#include "../xa/xa_mem.h"         // memspc51, mem_cbuf1 mem_cbuf1_SIZ
#include "../xa/xa_msg.h"         // MSG_const__
#include "../xa/xa.h"             // APP_act_*

// ex ../xa/xa_ui.c:
extern MemObj    ckb_man, ckb_vwr;
extern int       UI_InpMode;
// extern MemObj UIw_Box_TB;    // toolbarBox


//================================================================
// LOCAL PROTOTYPES:



//================================================================
// LOCAL VARS:
static int NC_procNr = 0;  // 0=processor-dll not connected; 1=loaded.

static MemObj  prc_win  = GUI_OBJ_NEW;


char **process_CmdTab;     // was NCCmdTab







//================================================================
  int PRC_set_CmdTab (char **CmdTab) {
//================================================================
// used by process-dll to provide the commandTable

  // printf("PRC_set_CmdTab\n");
  // if(CmdTab) printf("  CmdTab[0] |%s|\n",CmdTab[0]);

  process_CmdTab = CmdTab;

  return 0;

}


//================================================================
  int PRC_init (char *dllNam) {
//================================================================
// Input:
//   ii     1-n load xa_nc_<prcNam>.dll
//          0   call connected function with parameter fdat
//   retCod  -1= error-build;


// OS_dll__ in ../ut/ut_os_aix.c ../ut/ut_os_w32.c

  int          irc;
  char         s1[256];


  printf("PRC_init |%s|\n",dllNam);
  printf("  procNr = %d\n",NC_procNr);
  printf("  APP_act_proc |%s|\n",APP_act_proc);


  if(NC_procNr) {
    // a ddl is loaded; test if <dllNam> already loaded
    if (!strcmp (dllNam, APP_act_proc)) goto L_start; // same dll; OK
    // unload CmdTab
    PRC__ (-1, "EXIT__");
    // unload APP_act_proc
    OS_prc_dll (-4, NULL);
  }


  //----------------------------------------------
  // recompile dll 
  if(AP_stat.comp) {
    sprintf(s1, "%s.so",dllNam);
    irc = OS_dll_build (s1);
    if(irc) return -1;
  }


  //----------------------------------------------
  // fix DLL-FileName
#ifdef _MSC_VER
  sprintf(s1, "%splugins\\%s.dll",OS_get_bin_dir(), dllNam);
#else
  sprintf(s1, "%splugins/%s.so",OS_get_bin_dir(), dllNam);
#endif
    // printf(" so=|%s|\n",s1);



  // load dll 
  irc = OS_prc_dll (-2, (void*)s1);
  if(irc < 0) return irc;


  // connect to function PRCE__
  irc = OS_prc_dll (-3, (void*)"PRCE__");
  if(irc < 0) return irc;

  NC_procNr = 1;

  strcpy (APP_act_proc, dllNam);

  L_start:
  PRC__ (-1, "INIT__");

  return 0;

}


//================================================================
  int PRC__ (int mode, char* data) {
//================================================================
/// Input:
///   mode     >=0 index into NCCmdTab
///            -1  function
///            -2  open dll; data = dllName
///   data     additional text for nc-func NCCmdTab[mode]


  int          irc;
  char         s1[256];


  printf("PRC__ mode=%d |%s|\n",mode,data);
  // printf("  processor |%s|\n",APP_act_proc);



  // test if processor-dll already connected
  if(NC_procNr == 0) {
    // skip "RESET .." before "INIT__"
    if(!strncmp(data, "RESET", 5)) return 0;
    TX_Error ("PRC__ E001");
    return -1;
  }


  // execute func
  irc = OS_prc_dll (mode, data);

  return 0;
}


//================================================================
  int PRC_Loa () {
//================================================================
/// display list of processes; let user select; start selected process.
 
  int   i1;
  char  s1[256], fnam[256];

  printf("PRC_Loa \n");


  // create/update processes.lst
  PRC_lst_processes ();

  // display list of processes; let user select
  sprintf(fnam, "%sprocesses.lst", OS_get_tmp_dir());
//   i1 = GUI_list1_dlg_w (s1, 256,
//                        NULL, " select process", fnam,
//                        "1", NULL, "60,20");

  i1 = GUI_listf1__ (s1, sizeof(s1), fnam, "\"select process\"", "\"x40,y30\"");
  if(i1 < 0) return -1;
    printf(" selected process: |%s|\n",s1);
 
  PRC_activate__ (s1);

  return 0;

}


//================================================================
  int PRC_restart () {
//================================================================

 
  return PRC_activate__ (APP_act_nam);

}


//================================================================
  int PRC_disactivate__ () {
//================================================================
 
  char    s1[256];


  // printf("PRC_disactivate__ |%s|\n",APP_act_nam);

  if(APP_act_typ != 2) return -1;


  // save active process: copy editor -> file
  sprintf(s1, "%s%s", OS_get_tmp_dir(), APP_act_nam);
  ED_save_file (s1);


  // // disactivate active process in browser
  // Brw_Prcs_upd (APP_act_nam, -1);


  UI_Set_actPrg (APP_act_nam, 0);         // inactive
  // see AP_User_reset
  AP_stat.APP_stat = 0;

  APP_act_proc[0] = '\0';

  // remove nc-objects
  GL_Delete (APP_dli_start);


  // restore some application-functions...
  UI_func_stat_set__ (APF_CAD, APF_MAN,
                      APF_MEN_CAT, APF_MEN_APP, APF_MEN_PRC, APF_PLUGINS,
                      APF_MEN_RPC, APF_UNDO,
                      0);


  return 0;

}


//================================================================
  int PRC_activate__ (char *pNam) {
//================================================================
// pNam       mit "process_"
 
  int       ii;
  char      s1[200];


  // printf("PRC_activate__ |%s|\n",pNam);
  // printf("  APP_stat=%d\n",AP_stat.APP_stat);


  // test if file tmp/<pNam> exists
  sprintf(s1, "%s%s", OS_get_tmp_dir(), pNam);
    // printf(" %s\n",s1);
  if(!OS_checkFilExist (s1, 1)) {
    TX_Print ("***** process does not exist ..");
    return -1;
  }


  // core must be active !
  // test if addOn-prog is active
  if(AP_stat.APP_stat == 0) {
    // core-active
    // save the active Submodel AP_modact_nam -> TempFile
    Mod_sav_tmp ();
    goto L_1;
  }

  TX_Print ("***** disactivate active application / plugin ..");
  return -1;



  //----------------------------------------------------------------
  // activate
  L_1:
  // hilite process in browser
  // Brw_Prcs_upd (pNam, 1);


  APP_act_typ = 2;                        // "PRC "
  UI_Set_typPrg ();
  // pNam APP_act_nam can be identical !
  if(APP_act_nam != pNam) strcpy(APP_act_nam, pNam);
  UI_Set_actPrg (APP_act_nam, 2);
  AP_stat.APP_stat = 1;


  // lock some application-functions...
  UI_func_stat_set__ (-APF_CAD, -APF_MAN,
                      -APF_MEN_CAT, -APF_MEN_APP, -APF_MEN_PRC, -APF_PLUGINS,
                      -APF_MEN_RPC, -APF_UNDO,
                      0);


  // get dispListInd (keep all core-obj's)
  APP_dli_start = GL_Get_DLind ();


  // MAN already active: load processfile into editor;
  if(UI_InpMode == UI_MODE_MAN) {
    UI_src_edi ();

  // else activate editor
  } else {
    // implicite UI_VWR_OFF, UI_MAN_ON
    GUI_radiobutt_set (&ckb_man);
  }

  // test errcode
  if(AP_stat.errStat) return -1;


  // test if processor is defined
    // printf("  APP_act_proc |%s|\n",APP_act_proc);
  if(APP_act_proc[0] == '\0') {
    TX_Error("no Prozessor defined (PROCESS processname prozessorname)");
    return -1;
  }

    // printf("exit PRC_activate__ .. \n");

  return 0;

}


//================================================================
  int PRC_Cre__ () {
//================================================================
 

  printf("PRC_Cre__ \n");

  PRC_Cre_ui__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncInit));

  return 0;

}


//================================================================
  int PRC_Cre_ui__ (MemObj *mo, void **data) {
//================================================================

static MemObj prc1;
static int pr_id=0;
static char sproc[128];

  int      i1, ii;
  MemObj   boxv0, box0, box1, box2, w1;
  char     **optLst, s1[256];


  i1 = GUI_DATA_I1;

  printf("PRC_Cre_ui__ ev=%d dat1=%d\n",GUI_DATA_EVENT,i1);
  if(mo) printf("  gui-widget-nr=%d\n",GUI_OBJ_SRC(mo));


  if(mo) {
    ii = GUI_OBJ_SRC(mo);     // get widget-id

    if(ii == pr_id) {         // selection of processor
      strcpy(sproc, GUI_DATA_S2);
        printf(" proc |%s|\n",sproc);
      return 0;
    }
  }



  switch (i1) {


    //================================================================
    case UI_FuncInit:

      if(GUI_OBJ_IS_VALID(&prc_win)) {           // Win schon vorhanden ?
        return -1;
      }

      prc_win = GUI_Win__ ("Create Process", PRC_Cre_ui__, "");

      boxv0 = GUI_box_v (&prc_win, "");

      box1 = GUI_box_h (&boxv0, "");


      prc1 = GUI_entry__ (&box1, "Processname: ", "proc1", NULL, NULL, "16e");


      box2 = GUI_box_h (&boxv0, "");

      // GUI_label__ (&box1, "Processname: ", "l");
      GUI_label__ (&box2, "Processor:   ", "l");


      // get list-of-processes into optLst; display
      sprintf(s1, "%scadprocessors.lst",OS_get_tmp_dir());
      optLst = UTX_wTab_file (memspc55, sizeof(memspc55), s1);
      if(!optLst) {TX_Print("***** no processors found .."); return -1;}
      w1 = GUI_optmen__ (&box2, optLst[0], optLst, NULL, PRC_Cre_ui__, "10");
      pr_id = GUI_OBJ_SRC(&w1);
      strcpy(sproc, optLst[0]);
        printf("  opt1-widget-nr=%d\n",pr_id);


      // // create list of all postprocessors -> file ?
      // optLst = PRC_lst_postprocs (sproc);
      // w1 = GUI_optmen__ (&box2, optLst[0], optLst, NULL, PRC_Cre_ui__, "10");
      // pp_id = GUI_OBJ_SRC(&w1);
      // strcpy(spprc, optLst[0]);
        // printf("  opt2-widget-nr=%d\n",pp_id);


      GUI_button__ (&boxv0, "OK", PRC_Cre_ui__, &GUI_FuncUCB1, "");

      GUI_Win_up (NULL, &prc_win, 0);
      GUI_Win_go (&prc_win);
      break;

 
    //================================================================
    case UI_FuncUCB1:  // 1001
      // ok, init process ..
      i1 = PRC_Cre_cre (GUI_entry_get(&prc1), sproc);
      if(i1 < 0) return 0;


    //================================================================
    case UI_FuncExit:  // 102
    case UI_FuncKill:  //  99
        // printf("UI_FuncKill\n");

      // EXIT
      GUI_Win_kill (&prc_win);
      prc_win = GUI_OBJ_INVALID();
      break;

  }


  return 0;

}


//================================================================
  int PRC_lst_processes () {
//================================================================
// create file <tmp>processes.lst of all files <tmp>process_*


  int   i1;
  char  s1[256], s2[256];


  // printf("PRC_lst_processes\n");


  // list files  -> file <tmp>processes.lst
  // outfilnam
  sprintf(s2, "%sprocesses.lst",OS_get_tmp_dir());
  // dir to search
  sprintf(s1, "%s",OS_get_tmp_dir());
  i1 = UTX_dir_listf (s2, s1, "process_", NULL);
    // printf(" nrFiles=%d\n",i1);


  return i1;

}

/*
//================================================================
  char** PRC_lst_postprocs (char *sproc) {
//================================================================
// create list of postprocessors of processor <sproc>
//   into file <tmp>postproc.lst
// ls -1 <bindir>plugins/<processor>/ > <fnam>
// processor without leading "PRC_"

  static char *optNone[] = {"none",NULL};
  int   i1;
  char  s1[256], s2[256], **optLst;

  printf("PRC_lst_postprocs |%s|\n",sproc);


  // list files -> postprocessors -> file
  // dir to search
  sprintf(s1, "%splugins%c%s",OS_get_bin_dir(),fnam_del,&sproc[4]);
  // outfilnam
  sprintf(s2, "%spostproc.lst",OS_get_tmp_dir());
  i1 = UTX_dir_listf (s2, s1, NULL, NULL);
    printf(" nrFiles=%d\n",i1);

  if(i1 > 0) {
    optLst = UTX_wTab_file (memspc55, sizeof(memspc55), s2);
      UTX_wTab_dump (optLst);
    // add "none" to optLst
    optLst[i1] = optNone[0];
    ++i1;
    optLst[i1] = NULL;

  } else {
    optLst = optNone;
  }



  return optLst;

}
*/

//================================================================
  int PRC_Cre_cre (char *sprnam, char *sproc) {
//================================================================
// sprnam          processname
// sproc           processorname

// vi /home/fwork/gCAD3D/tmp/process_proc1

  int     i1;
  char    s1[256];
  FILE    *fpo;


  printf(" PRC_Cre_cre |%s|%s|\n",sprnam,sproc);


  // create new processfile
  // fnam: <tempDir>process_<processname>
  sprintf(s1, "%sprocess_%s",OS_get_tmp_dir(),sprnam);



  // test if processfile already exist;
  i1 = OS_checkFilExist (s1, 1);
    printf(" exist %d |%s|\n",i1,s1);

  if(i1) {
    sprintf(s1, "ERROR: process %s already exists ..",sprnam);
    GUI_MsgBox (s1);
    return -1;
  }


  //----------------------------------------------------------------
  // write processfile <tempDir>process_<processname>
  if ((fpo = fopen (s1, "w")) == NULL) {
    printf ("Fehler open Datei %s\n", "/tmp/test.dat");
    return (-1);
  }

  // # <processname> < processorname> <parameters> <datum>
  fprintf(fpo, "# %s\n",OS_date1());
  fprintf(fpo, "PROCESS %s %s\n",sprnam,sproc);

  fclose (fpo);


  //----------------------------------------------------------------
  // add process to browser
  Brw_Prcs_add (sprnam);


  return 0;

}


//================================================================
  int PRC_Del__ () {
//================================================================

  int     i1;
  char    s1[256], fnam[256];

  printf("PRC_Del__ \n");

  // create/update processes.lst
  PRC_lst_processes ();


  // display list of processes; let user select
  sprintf(fnam, "%sprocesses.lst", OS_get_tmp_dir());
//   i1 = GUI_list1_dlg_w (s1, 256,
//                        NULL, " delete process", fnam,
//                        "1", NULL, "60,20");

  i1 = GUI_listf1__ (s1, sizeof(s1), fnam, "\"delete process\"", "\"x40,y30\"");
  if(i1 < 0) return -1;
  
  
  // delete processfile
  PRC_del_file (s1);


  // delete process in browser
  Brw_Prcs_del (&s1[8]);

  return 0;

}


//================================================================
  int PRC_del_file (char *pNam) {
//================================================================
// pNam        with process_

  char    s2[256];

  printf("PRC_del_file \n");

  // start selected plugin
  sprintf(s2, "delete process %s",pNam);
  TX_Print(s2);

  sprintf(s2, "%s%s", OS_get_tmp_dir(),pNam);
    printf(" del |%s|\n",s2);
  OS_file_delete (s2);

  return 0;

}


//================================================================
  int PRC_lst_processors () {
//================================================================
/// list all processors into file <tmp>cadprocessors.lst

  int   i1, i2, iNr, ftyplen;
  char  *p1, cbuf1[256];
  FILE  *fpo;


  printf("PRC_lst_processors \n");


  // try to open outfile
  sprintf(cbuf1, "%scadprocessors.lst",OS_get_tmp_dir());
    // printf(" Dll.lst=|%s|\n",cbuf1);
  if((fpo=fopen(cbuf1,"w")) == NULL) {
    printf("***** PRC_lst_write E001 %s\n",cbuf1);
    // TX_Print("AP_DllLst_write E001 %s",cbuf1);
    return -1;
  }


  // Searchpath
  sprintf(cbuf1, "%splugins/",OS_get_bin_dir());
    printf(" path-plugins=|%s|\n",cbuf1);


#ifdef _MSC_VER
      ftyplen = 4; // ".dll"
#else
      ftyplen = 3; // ".so"
#endif


  iNr = 0;
  OS_dir_scan_ (cbuf1, &iNr);   // Init
    // printf(" nach OS_dir_scan_ %d\n",iNr);

  if(iNr > 0)  {

    for (;;) {
      OS_dir_scan_ (cbuf1, &iNr);
      // printf(" n.scan %d |%s|\n",iNr,cbuf1);
      if(iNr < 0) break;

      // filter filetyp
      i1 = strlen(cbuf1);

      // keep only dll's
#ifdef _MSC_VER
      for(i2=i1-3; i2<i1; ++i2) cbuf1[i2] = tolower (cbuf1[i2]);
      if(strncmp(&cbuf1[i1-4], ".dll", 4)) continue;
#else
      if(strncmp(&cbuf1[i1-3], ".so", 3)) continue;
#endif

      // skip directories
      if(OS_check_isDir(cbuf1) == 0) continue;


      // strip directory - find last "/" (makes |t1| aus |/mnt/x/t1|)
      p1 = strrchr (cbuf1, fnam_del);
      if(p1) ++p1;
      else p1 = cbuf1;

      // keep only PRC_*.dll's
      if(strncmp(p1, "PRC_", 4)) continue;

      // remove filetyp .so /.dll
      p1[strlen(p1)-ftyplen] = '\0';

      // write -> file
      fprintf(fpo,"%s\n",p1);
        // printf(" add plugin %d |%s|%s|\n",iNr,cbuf1,p1);
    }
  }

  fclose(fpo);
  return 0;

}


//================================================================
  int RPC_restart () {
//================================================================
/// reStart remote
/// TODO: check if already active ..

  int   irc;
  char  fn[256];

  if(APP_act_typ != 4) {
    TX_Error("RPC_restart - active prog must be remote-control-prog ..");
    return -1;
  }


// TODO: check execute-permission 
  sprintf(fn, "%s%s.cmd",AP_mod_dir, APP_act_nam);
    printf(" TODO: check execute-permission |%s|\n",fn);


  // execute nonblocking
  // cd <<bindir>/remote&&excute
  // sprintf(memspc011, "cd %sremote&&./%s&",OS_get_bin_dir(), APP_act_nam);
  sprintf(memspc011, "cd %s &&./%s.cmd &",AP_mod_dir, APP_act_nam);
    printf("RPC_restart |%s|\n",memspc011);
  irc = OS_system (memspc011);
    printf(" _restart %d\n",irc);


  return 0;

}

 
//================================================================
  int RPC_Loa () {
//================================================================
/// \code
/// Start remote
/// display list of files in directory <bindir>/remote;
/// execute selected file
/// \endcode

  int    irc;
  char   s1[256], s2[256], fn[256];

  printf("RPC_Loa \n");


  // no addOn-prog may be active.
  if(AP_stat.APP_stat != 0) {
    TX_Print ("***** disactivate active application / plugin ..");
    return -1;
  }


  // get filename of cfg/dir.lst
  MDLFN_symFilNam (s1);

  // list directory <bindir>/remote
  sprintf(s2, "%sprg/", OS_get_loc_dir());
//   irc = GUI_file_open__ (pNam, 128, s1, 200, NULL, NULL, "open", "*");
//   if(irc) return 0;

  // (dirIn/filnamOut sSiz symDir filter title)
  irc = GUI_file_open__ (s2, 256, s1, "\"*.cmd\"", "open remote control-prog");
    printf(" open %d |%s|\n",irc, s2);

  // UTX_add_fnam_del (s1);    // add following "/"

  // cut dirctory/filename
  UTX_fnam__ (AP_mod_dir, AP_mod_fnam, AP_mod_ftyp, s2);

  APP_act_typ = 4;                        // "RPC "
  UI_Set_typPrg ();
  strcpy(APP_act_nam, AP_mod_fnam);
  // display prgNam
  UI_Set_actPrg (APP_act_nam, 0);


  // execute nonblocking
  RPC_restart ();

  return 0;

}


// EOF
