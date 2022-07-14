/*  Demoprog Animation.                              RF     2007-12-28
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
\file  ../APP/DemoAnim1.c
\brief Demoprog Animation
\code
=====================================================
List_functions_start:
..
List_functions_end:
=====================================================

\endcode *//*----------------------------------------

This is the start of a Kinematik/Animation-Module.

The first Mesh/subModel is animated (eg "M1")


Weiter:
- als Input eine Transformation nehmen; Translation oder Rotation
  oder kombiniert oder Kurve oder quat-Kurve ...
  Diese zerlegen (via parameter 0-1) und anwenden.

- Einzelne animierte Pfade auf einen Teilzyklus eingrenzen;
  durch Startzeitpunkt & Dauer ...


*/


#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>       // fuer Variad. Funktionen (...)
#include <time.h>         // f clock

/*
#include <GL/gl.h>
#include <GL/glu.h>
*/



#ifdef _MSC_VER
// die folgenden 2 Funktionen exportieren (werden vom Main gerufen):
__declspec(dllexport) int gCad_main ();
__declspec(dllexport) int gCad_fini ();
// nachfolgende externals werden aus dem Main-Exe imported:
#define extern __declspec(dllimport)
#endif



#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_os.h"               // AP_get_bas_dir ..
#include "../ut/ut_cast.h"             // INT_PTR

#include "../gui/gui__.h"

#include "../ut/func_types.h"               // UI_Func... SYM_..
#include "../gr/ut_DL.h"               // DL_Att

#include "../db/ut_DB.h"               // DB_get_ModRef



static int anim_stat;
  //  0  uninitialized
  // -1  stop is active
  // -2  immediate exit

static double  cycStat;
  // cycStat is from 0-1, 0=Start of animation; 1=End.


static MemObj    win0;



//================================================================
  int Ani_timer (int mode, double *timCycAct) {
//================================================================
// mode = 0:   init (timCycStart & timCycTot setzen)
// mode = 1:   give timCycAct (vergangene Zeit als Wert 0-1)


#define  minDiff  0.001        // min. increment between updates (tot=0-1)

  double d0, d1;

  static double timCycStart, timCycTot;




  if(mode == 0) {   // init timer
    timCycTot = *timCycAct;
      printf("Ani_timer 0 timCycTot=%f \n",timCycTot);
    timCycStart = OS_rTime ();
    // den StartZeitpunkt verschieben entsprechend der bisher vergangenen Zeit
    // timCycStart += d2 * timCycTot;
    d1 = cycStat * fabs(timCycTot);
    timCycStart -= d1;
      printf(" Start=%f d1=%f cycStat=%f\n",timCycStart,d1,cycStat);
    return 0;

  }


  L_1:
  d0 = OS_rTime();
  d1 = (d0 - timCycStart) / fabs(timCycTot);
  if((d1 - cycStat) < minDiff) {
    // printf(" wait %f\n",cycStat);
    OS_Wait (10);
    goto L_1;
  }
    // printf(" d1=%f cycStat=%f\n",d1,cycStat);
    // d1 is now the new cycStat (0-1) and > cycStat.

  // positive
  if(timCycTot > 0.) {          
    *timCycAct = d1;

  // negative
  } else {
    d1 -= cycStat;              // d1=pos.difference
    *timCycAct = cycStat - d1;
    d1 = *timCycAct * -timCycTot;  // cycleTime from cycStat
    timCycStart = d0 - d1;         // reset StartTime from cycleTime
  }

    // printf("ex Ani_timer %f\n",*timCycAct);

  return 0;

}
 

//================================================================
  int Ani_Idle_CB (void *data) {
//================================================================
// Callback-function of Idle-Function


  static double  az1, ay, az2;

  long     irc, ind, dli;
  ModelRef *mr;
  ModelBas *mb;


  // printf("Ani_Idle_CB %d\n",anim_stat);


  // -2 = exit animation
  if(anim_stat == -2) {
    Ani_work__ (2);       // Reset Scene
    return 0;             // finish idle-calls
  }


  // -1 = stop animation
  if(anim_stat == -1) {
    return 0;             // finish idle-calls
  }


  // 1 = continue animation
    irc = 1; // continue

    // get new cycStat
    Ani_timer (1, &cycStat);

    // stop at overflow
    if(cycStat > 1.) {
      cycStat = 1.;
      anim_stat = -1;
      // reactivate speed-entry
      Ani_win__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB2));
      irc = 0;     // stop idling
    }
    if(cycStat < 0.) {
      cycStat = 0.;
      anim_stat = -1;
      // reactivate speed-entry
      Ani_win__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB2));
      irc = 0;     // stop idling
    }

    // update slider-position (calls Ani_CB_2 > Ani_work__())
    Ani_win__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB4));


    return irc;  // continue

}


//=========================================================
  int Ani_work__ (int mode) {
//=========================================================
// mode=0   init (save initparameters
// mode=1   Transform all animated objects according cycStat
// mode=2   Reset Scene (all animated objects back into originalPosition)

// cycStat is from 0-1, 0=Start of animation; 1=End.

  long     dbi;
  Vector   vcd;
  double   az1, ay, az2;

  static long dli;
  static ModelRef *mr;
  static ModelRef md;
  static ModelBas *mb;

  static Point  ptb;   // original basepoint

  static Vector vctr;  // translation vector for full cycle
  static double az;    // angle for full cycle


  printf("Ani_work__ %d %f\n",mode,cycStat);


  //======== INIT =======================
  if(mode == 0) {
    anim_stat = 0;
    cycStat = 0.;
    


    // find dispListIndex dli and DB-Index dbi of first MeshModel.
    dbi = auxFun1(&dli);
    if(dbi < 0) return -1;
      printf(" first model in dispList dbi=%ldi dli=%ld\n",dbi,dli);

    // get ModRef
    mr = DB_get_ModRef (dbi);
      DEB_dump_obj__ (Typ_Mock, mr, "M%d:",dbi);

    md = *mr;

    // get baseModel
    mb = DB_get_ModBas (mr->modNr);
      DEB_dump_obj__ (Typ_SubModel, mb, "Ditto:");

    // save BasePoints, Angles ..
    ptb = mr->po;

    // save translationVector
    UT3D_vc_3db (&vctr, 100., 0., 0.);
    az = 180.;


    DL_unvis_set (dli, 1); 

    return 0;
  }



  //======== RESET =======================
  if(mode == 2) {
    mr->po = ptb;
    az1 = 0.;
    ay  = 0.;
    az2 = 0.;

    cycStat = 0.;
    goto L_disp;
  }


  //======== WORK =======================
    // fix new origin mr-> and angles according to cycStat
    UT3D_vc_multvc (&vcd, &vctr, cycStat);
    UT3D_pt_traptvc (&mr->po, &ptb, &vcd);
    // mr->po.x += 0.1;
    az1 = az * cycStat;
    ay  = 0.;
    az2 = 0.;
      printf(" az1 = %f\n",az1);

    md.po.x += 0.2;
      // printf(" ptb.x=%f\n",ptb.x);



  

  //======== DISPLAY =======================
  L_disp:

  // GLB_DrawInit ();
  // DL_SetInd (dli);
  // dli = -8L;
  // GL_DrawDitto1 (&dli, &ptb, &mb->po, az1, ay, az2, mr->scl,
                  // mb->DLsiz, mb->DLind);
  // dbi = 0;
  // GR_DrawModel (&dbi, 0, &md);

  // endgl und swapbuffer
  // GLB_DrawExit ();

  DL_Redraw ();


  return 0;

}


//=========================================================
  int Ani_tmr_load () {
//=========================================================
// get cycleTime & init timer

  double    d1;

  // get total cycleTime and direction (sign)
  Ani_win__ (&d1, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB3));

  Ani_timer (0, &d1);       // init timer

  return 0;

}


//=========================================================
  int Ani_CB_1 (MemObj *mo, void **data) {
//=========================================================
// Dialog-Button-clicked-callback

  char      *cp1;


  if(GUI_DATA_EVENT == TYP_EventExit) {    // from GUI_Win__
    goto L_exit;
  }


  // cp1 = (char*)data;
  cp1 = GUI_DATA_S1;

  printf("Ani_CB_1 |%s| %d\n",cp1,anim_stat);


  //-----------------------------------------------------
  if(!strcmp(cp1, "Start")) {   // Start/Stop-Button pushed ..
    if(anim_stat == 0) {
      Ani_tmr_load ();
      anim_stat = 1;
      GUI_idle__ (Ani_Idle_CB, NULL);  // start idle-callbacks
      // activate speed-entry
      Ani_win__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB1));
      return 0;
    }
    if(anim_stat > 0) {    // is active - stop.
      anim_stat = -1;
      Ani_Idle_CB (NULL);  // stop idling
      // disactivate speed-entry
      Ani_win__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB2));
      return 0;
    }
    if(anim_stat < 0) {    // is stopped - restart
      Ani_tmr_load ();
      GUI_idle__ (Ani_Idle_CB, NULL);  // start idle-callbacks
      anim_stat = 1;
      // reactivate speed-entry
      Ani_win__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB1));
      return 0;
    }


  //-----------------------------------------------------
  } else if(!strcmp(cp1, "Exit")) {
    goto L_exit;
  }

  return 0;


  //-----------------------------------------------------
  L_exit:
    if(anim_stat == 1) {
      anim_stat = -2;
      Ani_Idle_CB (NULL); // stop idling
    }
    Ani_work__ (2);       // Reset Scene
    gCad_fini ();         // kill window & exit

  return 0;

}


//=========================================================
  int Ani_CB_2 (MemObj *mo, void **data) {
//=========================================================
// user-has-moved-slider-callback

  char      *cp1;
  char      cbuf[512];


  // printf("Ani_CB_2 %f\n",gtk_adjustment_get_value(adj1));

  if(GUI_DATA_EVENT != TYP_EventMove) return 0;  // 2013-09-10

  // cycStat = gtk_adjustment_get_value(adj1);
  cycStat = GUI_DATA_D1;
  Ani_work__ (1);

  return 0;

}



//=========================================================
  int Ani_win__ (void *parent, void **data) {
//=========================================================
// Animation-window; all functions.


  static MemObj    we1, wc1, wsl1;

  MemObj           box0, box1, wact;
  int              i1;
  double           d1;
  char             *cp1;


  i1 = GUI_DATA_I1;
  // i1 = INT_PTR(data);

  printf("Ani_win__ %d\n",i1);



  switch (i1) {

    //---------------------------------------------------------
    case UI_FuncInit:
      printf("Ani_win__ init\n");

      // win0 = GUI_Win__ ("Animation-Demo", Ani_CB_1, "");
      win0 = GUI_Win__ ("Animation-Demo", Ani_win__, "");

      box0 = GUI_box_v (&win0, "a,a");

      wsl1 = GUI_Slider__ (&box0, 0, Ani_CB_2, 0., 1., 0., "10");

      box1 = GUI_box_h (&box0, "a,a");

      we1 = GUI_entry__ (&box1,  " time ","5", NULL, NULL, "a,a");

      GUI_label__ (&box1, "    ", "l"); // space between

      GUI_button__ (&box1, " Start/Stop ",  Ani_CB_1, (void*)"Start", "a,a");

      GUI_label__ (&box1, "    ", "l"); // space between

      wc1 = GUI_ckbutt__ (&box1, "back", 0, NULL, NULL, "a,a");

      GUI_button__ (&box1, " Exit ",   Ani_CB_1, (void*)"Exit", "a,a");

      GUI_Win_go (&win0);

      GUI_Win_up (NULL, &win0, 0); // always on top ..

      break;



    //---------------------------------------------------------
    case UI_FuncUCB1:     // disactivate entry
      printf("win UCB1\n");
      GUI_set_enable (&we1, 0);
      break;


    //---------------------------------------------------------
    case UI_FuncUCB2:      // activate entry
      printf("win UCB2\n");
      GUI_set_enable (&we1, 1);
      break;


    //---------------------------------------------------------
    // get total cycleTime and direction (sign)
    case UI_FuncUCB3:
      printf("win UCB3\n");
      // activate entry
      GUI_set_enable (&we1, 1);
      cp1 = GUI_entry_get (&we1);
      d1 = fabs(atof (cp1));
      i1 = GUI_ckbutt_get (&wc1);
      if(i1) d1 = - d1;
      *((double*)parent) = d1;
      break;


    //---------------------------------------------------------
    // update slider-position (calls Ani_CB_2 > Ani_work__())
    case UI_FuncUCB4:
      // printf("win UCB4\n");
      GUI_Slider_set (&wsl1, cycStat);
      break;


    //---------------------------------------------------------
    case UI_FuncKill:
      printf("win exit\n");
      gCad_fini ();

  }

  return 0;

}


//=========================================================
  int gCad_fini () {
//=========================================================
// dll being unloaded - reset Input, kill all open windows !!!

  TX_Print("...... gCad_fini DemoAnim1");

  // kill window
  GUI_Win_kill (&win0);

  AP_User_reset ();      // close application

  return 0;

}


//=========================================================
  int gCad_main () {
//=========================================================
// init userfunction

  int     i1;
  double  d1;

  TX_Print("--->>>>>> gCad_main aus DemoAnim1 >>>>>>>---\n");

  i1 = Ani_work__ (0);                        // init transformations
  if(i1 < 0) {
    TX_Print("no MeshModel loaded; open samp_wrl1.wrl or other Mesh ..");
    gCad_fini();
    return 0;
  }

  // create window
  Ani_win__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncInit));

  return 0;


}


//================================================================
  int auxFun1 (long *dli) {
//================================================================
// returns DBIndex of first MeshModel else -1

  long   l0, l1;
  DL_Att dlRec;


  l1 = GL_GetActInd();

  for(l0=0; l0<=l1; ++l0) {
     // dlRec = DL_GetAtt(l0);
     DL_dlRec__dli (&dlRec, l0);
     printf(" dl[%ld] = %d %ld\n",l0,dlRec.typ,dlRec.ind);
     if(dlRec.typ == Typ_Mock) {*dli = l0; return dlRec.ind;}
  }



  return -1;

}


//====================== EOF =============================
