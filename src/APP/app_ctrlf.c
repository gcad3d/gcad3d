// ../xa/ctrlf.c           controlfile    CTRLF_*
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
 void CTRLF(){}
#endif
/*!
\file  ../xa/xa_ctrl.c
\brief communication-control 
\code
=====================================================
List_functions_start:

CTRLF_load                   work file <fNam>; store obj's in DB
CTRLF_do__                   work line - store obj in DB
CTRLF_do_tstCmd              call locally implemented testfunctions

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


command ::
  do remotecommand (CTRL_CB_do__())      Docu.: ../../doc/html/RemoteControl_en.htm

Example:

:<label>
:: LOAD(ln)
:: WAIT_ESC                # in CTRLF_do__ extra implement.
A20=S29 # CW
::: test_disp__ (A20)      # disp. obj
:: WAIT_ESC                # stop; wait for Esc-key
::: test_hide_last ()      # remove diplay of last created obj



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



//================================================================
  int CTRLF_load (char *fNam, char *lbs) {
//================================================================
// CTRLF_load                   work file <fNam>; store obj's in DB

  int    irc;
  FILE   *fpi;
  char   s1[256], *p1;


  printf("CTRLF_load |%s|%s|\n",fNam,lbs);

  if((fpi = fopen (fNam, "r")) == NULL) {
    printf ("Fehler open Datei %s\n", fNam);
    return -1;
  }

  //----------------------------------------------------------------
  // find startlabel (eg ":L1:")
  while (!feof (fpi)) {
    if (fgets (s1, 256, fpi) == NULL) break;
    UTX_CleanCR (s1);
    if(s1[0] != ':') continue;
    // first word starts with ':'; test second char 
    if(s1[1] == ':') continue; // skip remote-command outside block

    p1 = strchr(&s1[1], ':');
       printf(" p1=|%s|\n",p1);
    if(!p1) continue;

    // found label;
    // cut string at secd ':'
    p1[1] = '\0';
    if(!strcmp(s1, lbs)) goto L_start;
  }

  TX_Print("*** CTRLF_load - startLabel not found ...");
  irc = -1;
  goto L_exit;


  //----------------------------------------------------------------
  L_start:
      printf(" CTRLF_load-startLabel |%s|\n",s1);
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
    irc = CTRLF_do__ (s1);
    if(irc < 0) goto L_exit;
    continue;


    //----------------------------------------------------------------
    L_remCmd:
    // found remote-command
    p1 = &s1[2];
    UTX_pos_skipLeadBlk (p1);
      printf(" rem.cmd |%s|\n",p1);

    if(!strcmp (p1, "WAIT_ESC")) {
      test_disp_Esc () ;
    } else {
      CTRL_CB_do__ (p1);
    }
    continue;


    //----------------------------------------------------------------
    L_tstCmd:
    // found test-command (must be defined as local function)
    p1 = &s1[3];
    UTX_pos_skipLeadBlk (p1);
    CTRLF_do_tstCmd (p1);
  }

  irc = 0;


  //----------------------------------------------------------------
  L_exit:
  fclose(fpi);

    // TESTBLOCK
    // return MSG_ERR__ (ERR_TEST, "ex-CTRLF_load");
    // END TESTBLOCK


  return irc;

}


//================================================================
  int CTRLF_do__ (char *cmdLn) {
//================================================================
// CTRLF_do__          work line - store obj in DB

  int      irc, typ;
  long     dbi;
  char     *p1;

  printf("CTRLF_do__ |%s|\n",cmdLn);
  // return MSG_ERR__ (ERR_TEST, "CTRLF_do__-in");


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
    printf("**** ERROR unsupported-header: |%s|\n",cmdLn);
    return -1;
  }


  return 0;

}


//================================================================
  int CTRLF_do_tstCmd (char* cmd) {
//================================================================
// CTRLF_do_tstCmd             call locally implemented testfunctions

  char    *p2;


  printf("CTRLF_do_tstCmd |%s|\n",cmd);


  // cut off functionname
  p2 = strchr (cmd, ' ');
  if(!p2) {TX_Print("***** ERR CTRLF_do_tstCmd |%s|",cmd); return -1;}
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


// EOF
