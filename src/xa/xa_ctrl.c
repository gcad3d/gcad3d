// ../xa/xa_ctrl.c           communication-control    
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
  // TODO: CTRL_read__: skip \n inside "textStrings"
  Checkbox RemoteControl (statt Plugin);
  einen StartupParameter für RemoteControl=ON|OFF;
  remote control nur in VWR, nicht MAN, CAD;
  Anzeige wenn RemoteControl aktiv ist.

-----------------------------------------------------
Modifications:
2012-12-09 First version. RF.

-----------------------------------------------------
*/
#ifdef globTag
 void CTRL(){}
#endif
/*!
\file  ../xa/xa_ctrl.c
\brief communication-control 
\code
=====================================================
List_functions_start:

CTRL_CB__      mainentry (idle-callback)

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


Messageformat:
 '\0' terminates the command.

Multiline-message must be written into files,
 use command "do_script(filename)\n"



Test pipe:
echo 'new' > ~/gCAD3D/tmp/CTRLpin
// echo adds always \n at end-of-string !
echo 'load("/mnt/serv1/Devel/dev/gCAD3D/dat/sample_area1.gcad")' ..
echo 'load("Data/sample_area1.gcad")' ..

cat  ~/gCAD3D/tmp/CTRLpout     // in a new console-window


----------------------------------------
CONCEPT:
The mainTimer calls CTRL_CB__ periodically;
  CTRL_CB__ tests for input in inputpipe "CTRLpin".
    CTRL_read__ get input from pipe, test if input is complete
    CTRL_CB_do__ executes commands.



=========================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../ut/ut_geo.h"                // UT_INT_MAX
// #include "../ut/ut_umem.h"               // MEM_alloc_tmp
#include "../ut/ut_txTab.h"              // TxtTab
#include "../ut/ut_txt.h"                // UTX_pos_skipLeadBlk
#include "../ut/ut_os.h"                 // OS_get_bas_dir
#include "../ut/ctrl_os.h"               // OS_CTL_read__

#include "../gui/gui_types.h"            // GUI_KeyEsc

#include "../xa/xa_mem.h"                // mem_cbuf1_SIZ memspc201
#include "../xa/xa.h"                    // AP_sym_open ..


//_____________________________________
// EXTERNALS:
// aus ../xa/xa.c:
extern TxtTab    AP_TxTab1;
extern int       WC_modact_ind;        // the Nr of the active submodel; -1 = main.


// ../xa/xa_ui.c
// extern char       UI_stat_hide;


// ../ci/NC_Main.
extern double WC_ask_ModSiz ();




//_____________________________________
// INTERNALS:
// static char *pnin = NULL;       // pipenames
// static char *pnout = NULL;       // pipenames

static int   iUserCB = 0;

static char sBufOut[280];



//_____________________________________
// PROTOTYPES:
  int CTRL_sel_CB (int src, long dl_ind);




//================================================================
  int CTRL_CB__ () {
//================================================================
// callback from mainTimer; test for input in inputpipe "CTRLpin".
 
static int ctrlStat = 0;

  int     irc, ii;
  char    *cmdLn;
  // struct stat att1;


  // printf("CTRL_CB__ %d\n",iUserCB);


  // return if user-callback is pending 
  if(iUserCB) return 1;         // 1: keep timer ..

  // GUI_update__ (); // makes freeze complete in 2.24

  if(!ctrlStat) {

    //===== init pipes ===============================================
      printf(" init pipes ..\n");


    // open input-pipe
    sprintf(sBufOut, "%sCTRLpin",OS_get_tmp_dir());
    irc = OS_CTL_read_init (sBufOut);
    if(irc < 0) {
      sprintf(sBufOut,"OS_CTL_read_init: open error %d\n",irc);
      goto L_err_startup;
    }

    // open output-pipe
    sprintf(sBufOut, "%sCTRLpout",OS_get_tmp_dir());
    irc = OS_CTL_write_init (sBufOut);
    if(irc < 0) {
      sprintf(sBufOut,"OS_CTL_write_init: error %d\n",irc);
      goto L_err_startup;
    }

    ctrlStat = 1;
  }


  //===== read, work, write ========================================
  L_nxt:
    // printf("CTRL_CB__ \n");


  // get input from pipe, test if input is complete 
  cmdLn = OS_CTL_read__ (&ii);
  if(!cmdLn) return 1;
    printf(" new cmd = |%s|\n",cmdLn);



  // remote-command exists, execute ..
  irc = CTRL_CB_do__ (cmdLn);
    printf(" after CTRL_CB_do__ %d\n",irc);



  if(irc == 1) return 1;    // wait for user-selection


  goto L_nxt;


  //----------------------------------------------------------------
  L_err_startup:
    TX_Print(sBufOut);
    TX_Print("***** cannot init remoteControl ..");
    ctrlStat = -1;
    return 0;

}


//================================================================
  int CTRL_CB_do__ (char *parCmd) {
//================================================================
// executes commands.

 
  int    i1;
  long   l1, dli;
  double d1;
  char   cmd[64], s1[64], s2[128], s3[128], 
         *wPos1, *wPos2, *wPos3, *p1, cd;
  char   *cmdTab[] = {
         "PRINT", "VIEW", "SHOW",    "ATTL", "ATTS",
         "DUMP",  "MODE", "ZOOMALL", NULL};
         


  // printf("CTRL_CB_do__  |%s|\n", parCmd);

/*
  // DO NOT USE mem_cbuf1; WC_Work__ is using it !
  parCmd = IE_buf;      // size mem_cbuf1_SIZ!


  // check size of file
  if(finSiz >= mem_cbuf1_SIZ) {
    TX_Print("***** RemoteControl: buffer overflow ..");
    return -1;
  }


  // copy file > buffer <parCmd>
  fread (parCmd, finSiz, 1, finLun);
  parCmd[finSiz] = '\0';
    // printf("CTRL_CB_do__|%s|\n",parCmd);
*/

  // skip commands starting with # (comments)
  if(parCmd[0] == '#') return 0;


  // find 1. '('
  wPos1 = strchr (parCmd, '(');
  if(wPos1) {
    *wPos1 = '\0';
    ++wPos1;  // startpos parameters
    // skip wPos1-blanks
    UTX_pos_skipLeadBlk (wPos1);

    // remove closing ')';
    p1 = strrchr (wPos1, ')');
    if(p1) *p1 = '\0';
  }
 

  // copy > uppercase
  if(strlen(parCmd) > 62) {
    TX_Print("***** RemoteControl: command unknown ..");
    return -1;
  }
  UTX_cp_word_2_upper (cmd, parCmd);




    // TESTDISPLAY
    // printf(" cmd=|%s|\n",cmd);
    // if(wPos1) printf(" wPos1=|%s|\n",wPos1);




  //----------------------------------------------------------------
  UI_block__ (1,1,1);         // block funcs, input, cursor



  // clear error
  if(AP_errStat_get ()) {
    AP_errStat_reset (1);     // reset error
  }



  sBufOut[0] = '\0'; // empty answer



  //----------------------------------------------------------------
  // test APT_work_PrgCmd - functions
  // do command (HIDE VIEW MODSIZ DEFTX EXECM .. (AppCodTab))
  i1 = APT_work_AppCodTab (cmd, &wPos1);
  if(i1 >= 0) goto L_done;
  if(i1 == -1) return i1;
  // -2 == not_in_AppCodTab

/*
  i1 = UTX_wTab_ck (cmdTab, cmd);
  if(i1 >= 0) {
    // APT_work_PrgCmd (cmd, &wPos1);
    APT_work_PrgCmd (cmdTab[i1], &wPos1);    // 2016-03-16
    goto L_done;
  }
*/


  //----------------------------------------------------------------
  // new           see also UI_menCB "new"
  if(!strcmp(cmd, "NEW")) {
    AP_src_new(1);


  //----------------------------------------------------------------
  // GETSYMDIR
  } else if(!strcmp(cmd, "GETSYMDIR")) {
    wPos1 = UTX_CleanBracks (wPos1, '\"', '\"');    // remove "
    strcat (wPos1, "/");   // symdir must be terminated with '/'
    Mod_sym_get1 (sBufOut, wPos1, 0);


  //----------------------------------------------------------------
  // SETSYMDIR
  } else if(!strcmp(cmd, "SETSYMDIR")) {
    i1 = UTX_get_word (&wPos1, &wPos2, s1, &d1, &cd);
      // printf(" _words1 %d |%s|%s|\n",i1,s1,wPos2);
    if(i1 != 4) goto L_err_par;
    i1 = UTX_get_word (&wPos2, &wPos3, s2, &d1, &cd);
    if(i1 != 4) goto L_err_par;
      // printf(" _words |%s|%s|\n",s1,s2);
    Mod_sym_add (s1, s2);
    // check file /home/fwork/gCAD3D/cfg/dir.lst


  //----------------------------------------------------------------
  // DELSYMDIR
  } else if(!strcmp(cmd, "DELSYMDIR")) {
    wPos1 = UTX_CleanBracks (wPos1, '\"', '\"');    // remove "
    Mod_sym_del (wPos1);
    goto L_done;



  //----------------------------------------------------------------
  // LOAD
  } else if(!strcmp(cmd, "LOAD")) {
    wPos1 = UTX_CleanBracks (wPos1, '\"', '\"');    // remove "
    i1 = Mod_sym_get__ (AP_sym_open, AP_dir_open, WC_modnam, wPos1);
    AP_Mod_load (0);                   // load
  

  
  //----------------------------------------------------------------
  // LOADSM
  } else if(!strcmp(cmd, "LOADSM")) {
    wPos1 = UTX_CleanBracks (wPos1, '\"', '\"');    // remove "
                   //   symDir  absDir  filNam
    i1 = Mod_sym_get__ (s1,     s2,     s3,      wPos1);
    Mod_LoadSubmodel (s3, s2);                   // load
  


  //----------------------------------------------------------------
  // LOADTEX
  } else if(!strcmp(cmd, "LOADTEX")) {
    wPos1 = UTX_CleanBracks (wPos1, '\"', '\"');    // remove "
    i1 = Tex_addBas1 (wPos1, 1);                    // load texture



  //----------------------------------------------------------------
  // ADD
  // create objects ....
  } else if(!strcmp(cmd, "ADD")) {
    ED_srcLn_add (wPos1, 0);
    DL_Redraw ();           // update display



  //----------------------------------------------------------------
  // MOD
  // modify lien, update ..
  } else if(!strcmp(cmd, "MOD")) {
    CTRL_mod__ (wPos1);



  //----------------------------------------------------------------
  // ADDFILE(fn)
  // create objects ....
  } else if(!strcmp(cmd, "ADDFILE")) {
    wPos1 = UTX_CleanBracks (wPos1, '\"', '\"');    // remove "
      // printf(" wPos1=|%s|\n",wPos1);
    UTF_add_file (wPos1);           // add file to mem
    ED_work_CurSet (UT_INT_MAX);    // work until end of model  


  //----------------------------------------------------------------
  // ERRCLR
  } else if(!strcmp(cmd, "ERRCLR")) {
    i1 = AP_errStat_get ();
      // printf(" errStat=%d\n",i1);
    sprintf(sBufOut, "%d",i1);
    if(i1) {
      AP_errStat_reset (1);
      DL_Redraw ();           // update display
    }


  //----------------------------------------------------------------
  // get current lineNr    ED_lnr_act
  } else if(!strcmp(cmd, "GETLNR")) {
    sprintf(sBufOut, "%d",ED_get_lnr_act());


  //----------------------------------------------------------------
  // GETOBJINDNXT
  // get next free db-index ..
  } else if(!strcmp(cmd, "GETOBJINDNXT")) {
    i1 = AP_typ_typChar (wPos1[0]);
    sprintf(sBufOut, "%d",DB_QueryNxtFree(i1, 20));


  //----------------------------------------------------------------
  // MODSIZ(newsize)  // change modelsize
  } else if(!strcmp(cmd, "MODSIZ")) {
    d1 = atof (wPos1);
    WC_Init_Modsiz (d1);


  //----------------------------------------------------------------
  // GETMODSIZ        // get current modelsize
  // get next free db-index ..
  } else if(!strcmp(cmd, "GETMODSIZ")) {
    // sprintf(sBufOut, "%lf",WC_ask_ModSiz());
    UTX_add_fl_u (sBufOut, WC_ask_ModSiz());


  //----------------------------------------------------------------
  // END            // reprocess whole model
  } else if(!strcmp(cmd, "END")) {
    UI_but_END ();


  //----------------------------------------------------------------
  // DELFOLL(lnr)     // delete all lines following lineNr'
  } else if(!strcmp(cmd, "DELFOLL")) {
    i1 = atoi (wPos1);
    DL_disp_reset (i1);     // delete dispList
    ED_del__ (i1);          // delete in memory
    DL_Redraw ();           // update display
    


  //----------------------------------------------------------------
  // SAVE
  // SAVE (new_modelname)
  } else if(!strcmp(cmd, "SAVE")) {
    if(wPos1) {
      wPos1 = UTX_CleanBracks (wPos1, '\"', '\"');    // remove "
      i1 = Mod_sym_get__ (AP_sym_save, AP_dir_save, WC_modnam, wPos1);
    } else {
      strcat(WC_modnam, ".gcad");
    }
      // printf(" _save: |%s|%s|%s|\n",AP_sym_save,AP_dir_save,WC_modnam);
    UI_save__ (1);
    UTX_ftyp_cut (WC_modnam);
    goto L_done;
    


  //----------------------------------------------------------------
  // USER
  } else if(!strcmp(cmd, "USER")) {
    iUserCB = 1;                           // block RemCmd-input
    UI_block__ (-1, 0, 0);                 // reset input & cursor
    AP_UserSelection_get (CTRL_sel_CB);
    return 1;



  //----------------------------------------------------------------
  // get data commands ..


  //----------------------------------------------------------------
  } else {
    TX_Print("***** RemoteControl: unknown command \"%s\"",cmd);

  }


  // write answer sBufOut -> pipe poutLun;
  L_done:
  CTRL_write__ ();
    printf(" after CTRL_write__ ..\n");



  //----------------------------------------------------------------
  L_exit:
  UI_block__ (0,0,0);                      // unblock ..

  return 0;


  //----------------------------------------------------------------
  L_err_par:
    TX_Print("***** RemoteControl: parameter-error \"%s\"",wPos1);
    goto L_exit;

}


//================================================================
  int CTRL_mod__ (char *srcNew) {
//================================================================
// modify line, update ..

  int      i1, ll;
  long     lNr;
  char     *lPos, oid[40];


  // printf("CTRL_mod__ |%s|\n",srcNew);


  // get objID from def.Line
  i1 = APED_objID_defLn (oid, srcNew);
  if(i1 < 0) return -1;


  // find lineNr
  i1 = APED_search_defLn (&lPos, &lNr, &ll, oid, -1, 0);
    // printf(" i1=%d lNr=%ld\n",i1,lNr);
  if(i1 < 0) return -1;


  // modify line
  i1 = APED_src_chg (lNr, srcNew);


  ++lNr;   // APED_src_chg creates a additional line !
  ED_set_lnr_act (lNr);


  // rework from lineNr
  ED_work_CurSet (999999);


  return 0;

}


//================================================================
  int CTRL_write__ () {
//================================================================
// write answer sBufOut -> pipe poutLun;
 
  int   i1;

  i1 = strlen(sBufOut);


  printf("CTRL_write__ |%s| %d\n",sBufOut,i1);


#ifndef _MSC_VER
  // if using pipes (not when using files !) terminate line with '\n'
  // add '\n\0'
  sBufOut[i1] = '\n';
  ++i1;
  sBufOut[i1] = '\0';
#endif


  // write (poutLun, sBufOut, i1);
  OS_CTL_write__ (sBufOut, i1);

  return 0;

}


//================================================================
  int CTRL_sel_CB (int src, long dl_ind) {
//================================================================
// callback user-selection

  int     i1;
  long    l1;
  char    s1[64], *p1;


  // printf("CTRL_sel_CB %d %ld\n",src,dl_ind);


  // check Esc-key
  if(src == GUI_KeyEsc) {
    strcpy(sBufOut, "SEL()");
    goto L_done;
  }


  UI_GR_get_selNam (&i1, &l1, &p1);
    printf(" _sel_CB %d %ld %s\n",i1,l1,p1);
  sprintf(sBufOut, "SEL(%s)",p1);


  //----------------------------------------------------------------
  L_done:
  CTRL_write__ ();
    printf(" after CTRL_write__ ..\n");


  AP_UserSelection_reset ();    // reset  select
  UI_block__ (0,-1,-1);         // reset blocking Functions

  iUserCB = 0;
 
  return 0;

}


// EOF
