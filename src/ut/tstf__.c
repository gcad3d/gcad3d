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

- ../ut/tstf__.c
- Example see Demo_tstf_1.c Demo_tstf1.dat Demo_tstf_1.mak


- remotecommands start with ":: "; see ../../doc/html/RemoteControl_de.htm

  do remotecommand (CTRL_CB_do__())      Docu.: ../../doc/html/RemoteControl_en.htm


Commands:
# commentline  (first char must be '#')
:<label>:                  # label; processing starts here and ends at next label;
:: LOAD(modelfilename)     # load model
:: WAIT_ESC                # in TSTF_do__ extra implement.
::: test_disp__ (A1 A2)    # call user-function test_disp__ with parameter "A1 A2"
:: WAIT_ESC                # stop; wait for Esc-key
A20=S29 # CW



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

  int    irc;
  FILE   *fpi;
  char   s1[512], s2[512], *p1;


  printf("TSTF_load |%s|%s|\n",fNam,lbs);

  if((fpi = fopen (fNam, "r")) == NULL) {
    printf ("Fehler open Datei %s\n", fNam);
    return -1;
  }

  //----------------------------------------------------------------
  // find startlabel (eg ":L1:")
  while (!feof (fpi)) {
    if (fgets (s1, 512, fpi) == NULL) break;
    UTX_CleanCR (s1);
    if(s1[0] != ':') continue;
    // first word starts with ':'; test second char 
    if(s1[1] == ':') continue; // skip remote-command outside block

    p1 = strchr(&s1[1], ':');
       // printf(" p1=|%s|\n",p1);
    if(!p1) continue;

    // found label;
    // cut string at secd ':'
    p1[1] = '\0';
    if(!strcmp(s1, lbs)) goto L_start;
  }

  TX_Print("*** TSTF_load - startLabel %s not found ...",lbs);
  irc = -1;
  goto L_exit;


  //----------------------------------------------------------------
  L_start:
      printf(" TSTF_load-startLabel |%s|\n",s1);
  while (!feof (fpi)) {
    if (fgets (s1, 256, fpi) == NULL) break;
    UTX_CleanCR (s1);
    if(!strlen(s1)) continue;
      // printf(" in1 |%s|\n",s1);
    if(s1[0] == '#') continue;
    if(s1[0] == ':') {
      if(s1[1] == ':') {
        if(s1[2] == ':') goto L_tstCmd;
        goto L_remCmd;
      }
      goto L_exit;
    }
      // printf(" in2 |%s|\n",s1);
    // work line
    // irc = TSTF_do__ (s1);
    irc = ED_srcLn_add (s1, 0);
    if(irc < 0) goto L_exit;
    continue;


    //----------------------------------------------------------------
    L_remCmd:
    // found remote-command (":: cmd")
    p1 = &s1[2];
    UTX_pos_skipLeadBlk (p1);
      printf(" rem.cmd |%s|\n",p1);

    if(!strcmp (p1, "WAIT_ESC")) {
      UI_wait_Esc__ ();

    } else {
      CTRL_CB_do__ (p1);
    }
    continue;


    //----------------------------------------------------------------
    L_tstCmd:
    // found test-command (("::: cmd") must be defined as user-supplied function)
    p1 = &s1[3];
    UTX_pos_skipLeadBlk (p1);
      printf(" TSTF-userCmd |%s|\n",p1);
    // call function TSTF_cb with parameters p1
    strcpy(s2, "TSTF_cb ");
    strcat(s2, p1);
    DLL_run2 (s2, 2);
  }

  irc = 0;


  //----------------------------------------------------------------
  L_exit:
  fclose(fpi);

  DL_Redraw ();
  GUI_update__ ();

    // TESTBLOCK
    // return MSG_ERR__ (ERR_TEST, "ex-TSTF_load");
    // END TESTBLOCK


  return irc;

}



/*
//================================================================
  int TSTF_cb (char * cmd) {
//================================================================
// callback from TSTF-mudule (see TSTF_load INF_tstf__)
// execute private testfunctions - must start with "::: " in <cmdfile>


  char    *p2, s1[80];


  printf("TSTF_cb-main |%s|\n",cmd);

  return 0;

}


//================================================================
  int TSTF_do__ (char *cmdLn) {
//================================================================
// TSTF_do__          work line - store obj in DB

  int      irc, typ;
  long     dbi;
  char     *p1;

  printf("TSTF_do__ |%s|\n",cmdLn);
  // return MSG_ERR__ (ERR_TEST, "TSTF_do__-in");


  // get definition-header "A20="
  p1 = strchr(cmdLn, '=');
  if(!p1) {
    printf("**** ERROR no definition-src: |%s|\n",cmdLn);
    return -1;
  }

  // *p1 = '\0';
  ++p1;

  // get def.header typ,dbi
  irc = APED_dbo_oid (&typ, &dbi, cmdLn);
    printf(" _mshi_do__ typ=%d dbi=%ld\n",typ,dbi);

  if(irc < 0) {
    printf("**** ERROR definition-header: |%s|\n",cmdLn);
    return -1;
  }
    printf(" typ=%d dbi=%ld\n",typ,dbi);

  //----------------------------------------------------------------
  if(typ == Typ_SUR) {
    // test load surface from SRC and store in DB
    irc = test_mshi_do_sur (&dbi, p1);
    if(irc < 0) return irc;
      // MSHI_disp_sur_dbi_1 (dbi, "sbv"); // disp A<dbi>

  //----------------------------------------------------------------
  } else if(typ == Typ_SOL) {
    // test load solid from SRC and store in DB
    irc = test_mshi_do_sol (&dbi, p1);
    if(irc < 0) return irc;

  //----------------------------------------------------------------
  } else {
    ED_srcLn_add (cmdLn, 0);
  }


  return 0;

}


//================================================================
  int TSTF_do_tstCmd (char* cmd) {
//================================================================
// TSTF_do_tstCmd             call locally implemented testfunctions

  char    *p2;


  printf("TSTF_do_tstCmd |%s|\n",cmd);


  // cut off functionname
  p2 = strchr (cmd, ' ');
  if(!p2) {TX_Print("***** ERR TSTF_do_tstCmd |%s|",cmd); return -1;}
  *p2 = '\0';
  ++p2;

  // remove brackets
  p2 = UTX_CleanBracks (p2, '(', ')');
     printf(" _do_tstCmd |%s|%s|\n",cmd,p2);


  // call
  if(!strcmp (cmd, "test_disp__")) test_disp__ (p2);
  if(!strcmp (cmd, "test_hide_last")) test_hide_last ();


  return 0;

}
*/

// EOF
