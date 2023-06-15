//  Simple-Gtk-Demo  (minimum)
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
  ..

-----------------------------------------------------
*/
/*!
\file  ../APP/Demo_gsrc.c
\brief  Geometric-Source-Relocatable-Code to Geometric-Source-Code demo
\code
=====================================================
List_functions_start:
..
List_functions_end:
=====================================================

Demo Geometric-Source-Relocatable-Code

for info see in file ../ut/ut_gsrc.c   section  INF_GSRC

\endcode *//*----------------------------------------


*/


// definition "export"
#include "../xa/export.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_txTab.h"            // TxtTab
#include "../gui/gui_types.h"          // GUI_Key*


//----------------------------------------------------------------
// EXPORTS to main-module
export int gCad_main ();
export int gCad_fini ();


//----------------------------------------------------------------
// prototypes:




//=========================================================
  int gCad_main () {
//=========================================================
// user has selected this plugin; starting ...


  TX_Print("gCad_main Demo_gsrc");

//   // attach KeyIn
//   AP_UserKeyIn_get (gcad_key_CB);

  demo_gsrc ();

  return 0;

}


//=========================================================
  int gCad_fini () {
//=========================================================
// dll being unloaded - reset Input, kill all open windows !!!

  TX_Print("...... gCad_fini Demo_gsrc");

  AP_User_reset ();                      // close application

  return 0;

}


//=========================================================
  int demo_gsrc () {
//=========================================================

#define SIZ_LSC  16000


  int        irc, i1;
  long       ld, lnAct;
  char       *s1;     

  UtxTab_NEW (gsrc);
  UtxTab_NEW (gsc);



  printf("demo_gsrc\n");


  // get memSpc for GSRC
  UtxTab_init__ (&gsrc);
 
  // get memSpc for GSC
  UtxTab_init__ (&gsc);

  // UNDO_clear ();
  lnAct = ED_get_lnr_act ();
  
  // add GSRC into memSpc
  // create 3 new points
  UtxTab_add (&gsrc, "P<$>=P(10,10 0)");
  UtxTab_add (&gsrc, "P<$+1>=P(20,10 0)");
  UtxTab_add (&gsrc, "P<$+2>=P(20,20 0)");
  // create curve from points
  UtxTab_add (&gsrc, "S<$>=POL P<$> P<$+1> P<$+2> P<$>");
  // create surf from curve
  UtxTab_add (&gsrc, "A<$>=S<$>");

    // TESTBLOCK
    UtxTab_dump (&gsrc, "demo_gsrc-L2");
    // END TESTBLOCK
 

  //----------------------------------------------------------------
  // translate GSRC into GSC

  s1 = MEM_alloc_tmp (SIZ_LSC);

  GSRC_gsc_init ();

  // translate all gsrc-codelines in gsrc; copy new code into gsc
  for(i1=0; i1<gsrc.iNr; ++i1) {
      // printf(" _gsrc-nxt: |%s|\n",UtxTab__ (i1, &gsrc));

    // s1 = next GSRC-line translated to GSC
    irc = GSRC_gsc_gsrc_ln (s1, SIZ_LSC, UtxTab__ (i1, &gsrc));
    if(irc < 0) goto L_free;

    // add s1 to gsc
    UtxTab_add (&gsc, s1);
  }

    // TESTBLOCK
    UtxTab_dump (&gsc, "demo_gsrc-L3");
    // END TESTBLOCK
 

  //----------------------------------------------------------------
  // add GSC to active model
  for(i1=0; i1<gsc.iNr; ++i1) {
    UTF_add_line (UtxTab__ (i1, &gsc));
  }


  // update active model
  ED_work_CurSet (99999999);


  L_free:
    UtxTab_free (&gsrc);
    UtxTab_free (&gsc);

    if(irc < 0) goto L_exit;

    TX_Print(".... select UNDO - button to remove ....");

    // create undo-record
    UNDO_grp_range_add (lnAct, ED_get_lnr_act());


  L_exit:
    gCad_fini ();        // close app
    return 0;

}


//================  EOF  ==================================
