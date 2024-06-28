// ../ut/tstf__.c          TSTF    test-tool-file-based
/*
 *
 * Copyright (C) 2020 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
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
see test_mshi_disp_1
SHOW(A20,off) ???
DISP(A20, perm|dyn|temp, attribs) ?

:: test testfuncnam parameters ..

-----------------------------------------------------
Modifications:
2020-09-07 First version. RF.

-----------------------------------------------------
*/
#ifdef globTag
 void TSTF(){}
#endif
/*!
\file  ../xa/xa_ctrl.c
\brief communication-control 
\code
=====================================================
List_functions_start:

TSTF_load                   work file <fNam>; store obj's in DB
TSTF_do__                   work line - store obj in DB
TSTF_do_tstCmd              call locally implemented testfunctions

List_functions_end:
=====================================================

\endcode *//*----------------------------------------




//================================================================
*/ void INF_tstf__ () {} /*
//================================================================
- Testttool to execute commands in file;
- create file with testcommands with editor;
- start a plugin using function:
  irc = TSTF_load ("<file-with-testcommands>", "<Label>"); // CirSeg-Tria

- Example see ../APP/Demo_tstf_1.c ../APP/Demo_tstf_1.dat ../UIX/Demo_tstf_1.mak
- see ../../doc/html/TSTF_en.htm
- Source is ../ut/tstf__.c

- execute remoteCommand - ":: <remoteCommand>"; see ../../doc/html/RemoteControl_en.htm
- copy modelCodes into CAD-madel and process


Format commandfile:
# #             as first char defines commentline - is skipped
# ##            as first char defines commentline - is displayed
# :funcx:       startlabel, start of block; blockname ist "funcx";
#               end of block is start of next block - eg ":Block_2:"
#                 
# ::            starts remote-command; eg ":: WAIT_ESC" or ":: CLEAR"
# ::: fnc par   starts callBack with parameters;
# ::: :funcx:   starts subProgram, returns to calling program;



//----------------------------------------------------------------
// for a callback the plugin must provide function TSTF_cb like this:


//================================================================
  int TSTF_cb (char * cmd) {
//================================================================
// callback from TSTF-module (see TSTF_load INF_tstf__)
// execute private testfunctions - must start with "::: " in <cmdfile>
// MS-Win: function must be exported.


  char    *p2, s1[80];


  printf("TSTF_cb |%s|\n",cmd);
  // cmd is a string with functionname, parameters;
  // for example see ../APP/Demo_tstf_1.c - TSTF_cb and myFunc1


  return 0;

}



=========================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ut/ut_geo.h"              // UT_INT_MAX
#include "../ut/ut_txt.h"              // UTX_pos_skipLeadBlk


#include "../xa/xa_msg.h"              // MSG_* ERR_*



//_____________________________________
// EXTERNALS:


//_____________________________________
// INTERNALS:


//_____________________________________
// PROTOTYPES:
// int TSTF_cb (char *cmd);




//================================================================
  int TSTF_load (char *fNam, char *lbs) {
//================================================================
// TSTF_load                   work file <fNam>; store obj's in DB
// - calls TSTF_cb


#define mem_cbuf1_SIZ 200000
#define SIZ_PSTACK        12

  int    actInd,                  // lineNr of active file
         prgInd,                  // index in prgStack of calling prog
         prgStack[SIZ_PSTACK];    // returnIndex (lineNr) of calling prog

  int    irc, i1;
  FILE   *fpi;
  char   *p1, *p2;
  char   *txs;                    // read lines of file into txs
  char   prgNam[128];


  printf("\nTSTF_load |%s|%s|\n",fNam,lbs);

  prgInd = 0;  // next free index prgStack = nr of calling (stacked) progs
  actInd = 0;  // lineNr

  if(strlen(lbs) > 128) {TX_Error ("TSTF_load - label too long"); return -1;}
  strcpy(prgNam, lbs);


  if((fpi = fopen (fNam, "r")) == NULL) {
    TX_Error ("TSTF_load - open file %s\n", fNam);
    return -1;
  }

  // get memSpace for inputline
  txs = (char*) malloc (mem_cbuf1_SIZ);
  if(!txs) {TX_Print("*** TSTF_load - EOM"); return -1;}


  //----------------------------------------------------------------
  L_prgFind:           // find startlabel <prgNam> (eg ":L1:")
    printf(" L_prgFind: |%s|\n",prgNam);
  while (!feof (fpi)) {
    if(fgets(txs, mem_cbuf1_SIZ, fpi) == NULL) goto L_err_not_found;
    ++actInd;
    UTX_CleanCR (txs);
    if(txs[0] != ':') continue;
    // first word starts with ':'; test second char 
    if(txs[1] == ':') continue; // skip remote-command outside block
    // find closing ':'
    p1 = strchr(&txs[1], ':');
       // printf(" p1=|%s|\n",p1);
    if(!p1) continue;
    // found label;
    // cut string at secd ':'
    p1[1] = '\0';
    if(!strcmp(txs, prgNam)) goto L_start;
  }


  //----------------------------------------------------------------
  L_prg_exit:     // exit or continue with calling program
  --prgInd;
    // printf(" L_prg_exit: prgInd=%d stack=%d\n",prgInd,prgStack[prgInd]);
  if(prgInd < 0) goto L_done;  // no more stacked progs
  // rewind, continue with line <prgStack[prgInd]>
  rewind (fpi);
  actInd = 0;
  while (!feof (fpi)) {
    if(fgets (txs, mem_cbuf1_SIZ, fpi) == NULL) goto L_err_intern;
    ++actInd;
    if(actInd < prgStack[prgInd]) continue;
    goto L_start;  // continue calling prog
  }


  //================================================================
  L_start:   // found startlabel; process proglines until next startlabel;
      // printf(" TSTF_load-L_start |%s|\n",txs);
  while (!feof (fpi)) {
    if(fgets(txs, mem_cbuf1_SIZ, fpi) == NULL) goto L_prg_exit;  // eof = end of prog
    ++actInd;
    UTX_CleanCR (txs);
    if(!strlen(txs)) continue;        // skip empty line
      // printf("TSTF_load-in |%s|\n",txs);
 
    if(txs[0] == ':') {
      if(txs[1] == ':') {
        if(txs[2] == ':') goto L_tstCmd;
        goto L_remCmd;
      }
      goto L_prg_exit;  // found start of next block = end of prog
    }
      // printf(" in2 |%s|\n",txs);
    if(txs[0] == '#') {
      if(txs[1] != '#') continue; // display line "## xxx"; skip line "# xxx"
    }
    // work line
    irc = ED_srcLn_add (txs, 0);
    if(irc < 0) goto L_exit;
    continue;


    //----------------------------------------------------------------
    L_remCmd:
    // found remote-command (":: cmd")
    p1 = &txs[2];
    UTX_pos_skipLeadBlk (p1);   // skip leading blanks
      // printf(" rem.cmd |%s|\n",p1);

    if(!strcmp (p1, "WAIT_ESC")) {
      DL_Redraw ();
      UI_wait_Esc__ ();

    } else {
      CTRL_CB_do__ (p1);
    }
    continue;


    //----------------------------------------------------------------
    L_tstCmd:
    // found test-command (("::: cmd") must be defined as user-supplied function)
    p1 = &txs[3];
    UTX_pos_skipLeadBlk (p1);   // skip leading blanks
    // test if it is a startlabel (subprog)
    if(p1[0] == ':') {
      p2 = strchr(&p1[1], ':');  // find closing ':'
      if(!p2) goto L_err_syntax;
      p2[1] = '\0';               // cut
      strcpy(prgNam, p1);
      goto L_subProg;
    }
    // user-command; call function TSTF_cb with parameters p1
      // printf(" TSTF-userCmd |%s|\n",p1);
    irc = DLL_plu_exec ("TSTF_cb", (void*)p1);
    if(irc < 0) goto L_exit;
    continue;


    //----------------------------------------------------------------
    L_subProg:
      // printf("L_subProg: goto |%s| at lNr=%d\n",p1,actInd);
    // add active linNr to prgStack; start new scriptProg
    prgStack[prgInd] = actInd;
      // printf(" set prgStack[%d]=%d\n",prgInd,prgStack[prgInd]);
    ++prgInd;
    if(prgInd >= SIZ_PSTACK) {TX_Error("TSTF_load stack E1"); irc=-1; goto L_exit;} 
    // rewind and find new prog
    rewind (fpi);
    actInd = 0;
    goto L_prgFind;
  }
  //================================================================



  //----------------------------------------------------------------
  L_err_syntax:
  TX_Print("*** TSTF_load - error syntax line %d",actInd);
  irc = -1;
  goto L_exit;


  L_err_not_found:
  TX_Print("*** TSTF_load - startLabel %s not found",prgNam);
  irc = -1;
  goto L_exit;


  L_err_intern:
  TX_Print("*** TSTF_load - internal error");
  irc = -1;
  goto L_exit;


  //----------------------------------------------------------------
  L_done:
  irc = 0;


  //----------------------------------------------------------------
  L_exit:
  fclose(fpi);
  if(txs) free (txs);

  DL_Redraw ();
  GUI_update__ ();

    // TESTBLOCK
    // return MSG_ERR__ (ERR_TEST, "ex-TSTF_load");
    // END TESTBLOCK


  return irc;

}


// EOF
