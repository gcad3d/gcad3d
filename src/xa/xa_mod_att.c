//  xa_mod_att.c                                       2010-04-25   RF
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
2010-04-25 new - extracted from ../xa/xa_ui.c. RF

-----------------------------------------------------
*/
/*!
\file  ../xa/xa_mod_att.c
\brief  GUI for modify surfStyle, surfTransp, surfCol
\code
=====================================================
List_functions_start:

UI_WinSurfCol
UI_colSel              see UI_WinDefTx TSU_exp_wrlCol

UI_WinSurfStyl         see GUI_GetText

UI_WinSurfTra          Transparenz

SurMod_Ini             Init & Exit
SurMod_sel_CB
SurMod_key_CB
SurMod_EdKeyPress

List_functions_end:
=====================================================

\endcode *//*----------------------------------------

ex ../xa/xa_ui.c


-------------------------------------------------------
Compile:
cc -c `gtk-config --cflags` xa_sur_mod.c
cl -c /I ..\include xa_sur_mod.c

*/


#ifdef _MSC_VER
#include "MS_Def1.h"
#else
#include <dlfcn.h>           // Unix: dlopen
#endif




#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>           // toupper

#include <stdarg.h>          // for ../xa/xa_msg.h


#include "../ut/ut_geo.h"         // OFF, ON ..
#include "../ut/ut_TX.h"
#include "../ut/ut_os.h"
#include "../ut/ut_txt.h"         // fnam_del
#include "../ut/ut_txfil.h"       // UTF_FilBuf0Siz
#include "../ut/ut_cast.h"        // INT_PTR

#include "../gui/gui__.h"

#include "../xa/xa.h"
#include "../xa/xa_msg.h"         // MSG_open
#include "../xa/xa_ui.h"          // APF_TB_CAD,
#include "../xa/xa_sele.h"        // Typ_go*


#define   TRUE 1
#define   FALSE 0



// local:------------------------------------------
// static int       actFilt;
static int       ACTMOD;        // 1=Col,2=Tra,3=Styl


// extern:-----------------------------------------
// aus xa.c:
extern ColRGB     AP_actcol;
extern ColRGB     AP_defcol;






//=========================================================
  int SurMod_sel_CB (int src, long dli) {
//=========================================================
// userselection callback
// user has selected surface


  int    irc, i1, styp, typ;
  long   dbi;
  char   snam[64];


  printf("SurMod_sel_CB src=%d ind=%ld ACTMOD=%d\n",src,dli,ACTMOD);


  // save &exit m. rechter maustaste (see also PED_key_CB)
  if(src == GUI_MouseR) {
    // right-mousebutton
    if(ACTMOD == 1) {                  // exit color
      // UI_WinSurfCol (NULL, (void*)UI_FuncUCB3);
      UI_WinSurfCol (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB3));

    } else if(ACTMOD == 2) {           // exit transparency
      UI_WinSurfTra (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB4));

    } else if(ACTMOD == 3) {           // exit SurfStyle
      UI_WinSurfStyl (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB2));

    } else if(ACTMOD == 4) {           // exit linetype
      UI_WinLtypMod (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB2));
    }
    return 0;
  }


  UI_GR_get_selNam (&styp, &dbi, &snam);
    printf(" f-GR_get_selNam typ=%d dbi=%ld\n",styp,dbi);



  //----------------------------------------------------------------
  // left-mousebutton
  if(ACTMOD == 1) {
    // apply active colour.
    // query stat-Col; 0=selectNewCol; 1=applyColor; 2=removeColor.
    i1 = UI_WinSurfCol(NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB6));
      printf(" col-actStat=%d\n",i1);
    // i1=2: set color of (styp, dbi) as new AP_actcol
    GA_Col__ (dli, i1, styp, dbi);
    if(i1 == 2) {
      // 2:  select_as_new_color
      TX_Print("new active color is #%02x%02x%02x",  // %d,%d,%d ..",
        AP_actcol.cr,AP_actcol.cg,AP_actcol.cb);
      // report new col in panel
      UI_WinSurfCol (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB5));

    // } else {
        // printf("**** TODO: SurMod_sel_CB-%d\n",i1);
    }


  //----------------------------------------------------------------
  } else if(ACTMOD == 2) {
    // apply transparency
    // 0=reset,1=50%,2=100%
    // query tra
    i1 = UI_WinSurfTra(NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB5));
    GA_Tra__ (dli, i1, styp, dbi);


  //----------------------------------------------------------------
  } else if(ACTMOD == 3) {
    // apply SurfStyle
    i1 = UI_WinSurfStyl(NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB1));
    // query sStyl
    GA_sStyl__ (dli, i1, styp, dbi);


  //----------------------------------------------------------------
  } else if(ACTMOD == 4) {
    // apply linetype
    // query ltyp
    i1 = UI_WinLtypMod(NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB1));
    GA_lTyp__ (dli, i1, styp, dbi, 0);
  }


  return 0;

}

/* UNUSED
//================================================================
  gint SurMod_EdKeyPress (void *parent, void *event) {
//================================================================
// callback from keys in SurfacColorWindow
 
  printf("SurMod_EdKeyPress %d\n",((GdkEventKey*)event)->keyval);


  SurMod_key_CB (((GdkEventKey*)event)->keyval);

  return TRUE;  // prevent the default handler from being run

}
*/

//================================================================
  int SurMod_key_CB (int key) {
//================================================================
// used by SurMod_EdKeyPress & Main-keystroke-dispatcher


  printf("SurMod_key_CB +++++++++++++++++++++ key=%d ACTMOD=%d\n",key,ACTMOD);


  switch (key) {


    case GUI_KeyReturn:
    case 'q':
    case 's':
    case GUI_KeyEsc:
      // Reset & Exit.
      if(ACTMOD == 1) {
        // exit
        UI_WinSurfCol (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB3));

      } else if(ACTMOD == 2) {
        // exit
        UI_WinSurfTra (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB4));

      } else if(ACTMOD == 3) {
        // exit
        UI_WinSurfStyl (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB2));

      } else if(ACTMOD == 4) {
        // exit
        UI_WinLtypMod (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB2));
      }
      return TRUE;  // do no defaultOperations

    default:
      break;
  }

  return FALSE;  // do defaultOperations

}



//================================================================
  int SurMod_Ini (int mode) {
//================================================================
// mode 0=init, 1=exit
// used by: UI_WinSurfTra  UI_WinSurfCol UI_WinSurfStyl

  printf("SurMod_Ini %d %d\n",mode,ACTMOD);


  if(mode == 0) {
  // lock some application-functions...
  UI_func_stat_set__ (-APF_TB_CAD,
                      -APF_MEN0,
                      -APF_HIDE,
                      -APF_VWCADMAN,
                      -APF_BUT_GSE,
                      -APF_PLUGINS,
                      -APF_UNDO,
                      0);


  if(ACTMOD != 4)  // nicht fuer Linetypes
  AP_UserKeyIn_get     (SurMod_key_CB);         // attach keystrokes
  AP_UserSelection_get (SurMod_sel_CB);         // attach grafic selections

  // DL_pick__ (0);               // make all objects unpickable
  // DL_pick_typ (Typ_SUR, 1);    // make all surfaces pickable


    // actFilt = UI_GR_Sel_Filt_set (-1);  // query only !
    // UI_GR_Sel_Filt_set (8);



  } else {  // restore -----------------------------------------------
  // restore some application-functions...
  UI_func_stat_set__ (APF_TB_CAD,
                      APF_MEN0,
                      APF_HIDE,
                      APF_VWCADMAN,
                      APF_BUT_GSE,
                      APF_PLUGINS,
                      APF_UNDO,
                      0);

  AP_User_reset ();               // reset keyBd & selections ...

  // DL_pick__ (1);                  // reset selectionfilter
  // UI_GR_Sel_Filt_set (actFilt);  // reset

  }


  return 0;

}
 

//================================================================
  int UI_colSel (ColRGB *cSel) {
//================================================================
/// \code
/// user has selected new active color in color-selector
/// see UI_WinDefTx
/// see TSU_exp_wrlCol
/// see UTcol__3i
/// \endcode

  printf("UI_colSel %d %d %d\n",cSel->cr,cSel->cg,cSel->cb);

  // set new active colour
  APcol_actCol__ (cSel);


  TX_Print("new active Colour '%02x%02x%02x' defined ...",
            AP_actcol.cr,AP_actcol.cg,AP_actcol.cb);

  // report new color ..
  // UI_WinSurfCol (NULL, (void*)UI_FuncUCB5);  // up
  UI_WinSurfCol (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB5));  // up

  return 0;

}


//=====================================================================
  int UI_WinSurfCol (MemObj *mo, void **data) {
//=====================================================================
// see GUI_GetText
// - Define new active Color  // Button
// - add Color                // RadioButton
// - Remove Color             // RadioButton


  static MemObj win0=GUI_OBJ_NEW;
  static MemObj lb1;

  // actStat: 0=applyColor; 1=removeColor; 2=selectColFromSurf
  static int actStat;

  MemObj     box0, w1;
  char       s1[80];

  int        i1;
  // char       *p1, cbuf[32];


  // i1 = INT_PTR(data);
  i1 = GUI_DATA_I1;


  printf("UI_WinSurfCol %d actStat=%d ACTMOD=%d\n",i1,actStat,ACTMOD);
  // DEB_dump_obj__ (Typ_Color, &AP_actcol, "AP_actcol: ");


  switch (i1) {


    //---------------------------------------------------------
    case UI_FuncInit:  // 90

      // if(win0 != NULL) {           // Win schon vorhanden ?
      if(GUI_OBJ_IS_VALID(&win0)) {
        // gtk_widget_destroy (win0);
        return -1;
      }

      // win0 = GUI_Window ("change surface-color",
                         // UI_WinSurfCol,(void*)UI_FuncKill,0);
      win0 = GUI_Win__ ("change surface-color", UI_WinSurfCol, "");

      // box0 = GUI_Vbox (win0, 0);
      box0 = GUI_box_v (&win0, "");

      // GUI_Butt (box0, "define Color", UI_WinSurfCol,(void*)UI_FuncUCB4,0);
      GUI_button__    (&box0, "define active Color",
                       UI_WinSurfCol,(void*)&GUI_FuncUCB4,"");
      GUI_Tip("define new active Color");

      GUI_radiobutt__ (&box0,"select color from existing obj",0,
                       UI_WinSurfCol,(void*)&GUI_FuncUCB7,"");

      sprintf(s1," active color is #%02x%02x%02x",
              AP_actcol.cr,AP_actcol.cg,AP_actcol.cb);
      lb1 = GUI_label__ (&box0, s1,  "l");

      GUI_sep__ (&box0, 0, 5);

      w1 = GUI_radiobutt__ (&box0,"select surface to change Color",1,
                            UI_WinSurfCol,(void*)&GUI_FuncUCB1,"");
      GUI_radiobutt__ (&box0,"select surface to remove Color",1,
                       UI_WinSurfCol,(void*)&GUI_FuncUCB2,"");
      GUI_button__    (&box0, "  Exit   ",
                       UI_WinSurfCol,(void*)&GUI_FuncUCB3,"");
      GUI_radiobutt_set (&w1);
/*
      gtk_widget_set_events (win0,
                                GDK_KEY_PRESS_MASK);
      gtk_signal_connect (GTK_OBJECT(win0),"key_press_event",  // GTK2
                           GTK_SIGNAL_FUNC(SurMod_EdKeyPress), NULL);
*/

      GUI_Win_up (NULL, &win0, 0);  // always on top
      // gtk_widget_set_uposition (win0, 0, 0);  // Pos of mainwin
      // gtk_widget_show (win0);
      GUI_Win_go (&win0);

      // init all
      ACTMOD = 1;
      actStat = 0;
      SurMod_Ini (0);
      sele_set__ (Typ_goGeoSUSU);          // sel only Sur|Sol
      // message "define new Color, then select surface .."
      MSG_pri_0("ModColIn");
      return 0;


    //---------------------------------------------------------
    case UI_FuncUCB1:       // apply col to selected surface
      // skip disactivation
      if(GUI_DATA_EVENT == TYP_EventRelease) return 0;
      actStat = 0;
      return 0;


    //---------------------------------------------------------
    case UI_FuncUCB2:         // remove col
      // skip disactivation
      if(GUI_DATA_EVENT == TYP_EventRelease) return 0;
      actStat = 1;
      return 0;


    //---------------------------------------------------------
    case UI_FuncUCB4:   // called from ??   
      // get new colour - call UI_FuncUCB5
      UI_men__ ("colAct");
      return 0;


    //---------------------------------------------------------
    case UI_FuncUCB5:      // get new color - called from UI_colSel/UI_FuncUCB4
      // GUI_WinUp (win0);       // always on top
      sprintf(s1,"active color is #%02x%02x%02x",
              AP_actcol.cr,AP_actcol.cg,AP_actcol.cb);
      GUI_label_mod (&lb1, s1);
      return 0;


    //---------------------------------------------------------
    case UI_FuncUCB6:    // return actStat; used by SurMod_sel_CB
      return actStat;


    //---------------------------------------------------------
    case UI_FuncUCB7:    // set "select color from existing obj"
      actStat = 2;
      return 0;


    //---------------------------------------------------------
    // Exit
    case UI_FuncUCB3:
      // // send destroy-signal
      // gtk_signal_emit_by_name (GTK_OBJECT(win0),"destroy");
      // return 0;


    //---------------------------------------------------------
    // destroy-Signal
    case UI_FuncKill:  // 99

      // EXIT; restore all
      // gtk_widget_destroy (win0);
      // win0 = NULL;
      GUI_Win_kill (&win0);

    case UI_FuncExit:
      win0 = GUI_OBJ_INVALID();

      // exit all
      SurMod_Ini (1);
      return 0;

  }



  return 0;

}


//=====================================================================
  int UI_WinSurfTra (MemObj *mo, void **data) {
//=====================================================================
// Transparenz
// see GUI_GetText


  static MemObj win0=GUI_OBJ_NEW, ckb1, ckb2, ckb3, box0;
  static int actTra;
  // static int       actFilt;

  int        i1;
  char       *p1, cbuf[32];


  i1 = GUI_DATA_I1;

  printf("UI_WinSurfTra %d\n",i1);


  switch (i1) {


    //---------------------------------------------------------
    case UI_FuncInit:  // 90

      if(GUI_OBJ_IS_VALID(&win0)) {           // Win schon vorhanden ?
        return -1;
      }

      win0 = GUI_Win__ ("make Surf. transparent", UI_WinSurfTra, "");

      box0 = GUI_box_v (&win0, "");

      ckb1 = GUI_radiobutt__ (&box0,"0 - solid",      0, UI_WinSurfTra,
                              (void*)&GUI_FuncUCB1,"");
      ckb2 = GUI_radiobutt__ (&box0,"1 - 50% transp.",1, UI_WinSurfTra,
                              (void*)&GUI_FuncUCB2,"");
      ckb3 = GUI_radiobutt__ (&box0,"2 - 80% transp.",1, UI_WinSurfTra,
                              (void*)&GUI_FuncUCB3,"");

      GUI_button__           (&box0, "  Exit   ", UI_WinSurfTra,
                              (void*)&GUI_FuncUCB4,"");
/*
      gtk_widget_set_events (win0, 
                                GDK_KEY_PRESS_MASK);
      gtk_signal_connect (GTK_OBJECT(win0),"key_press_event",  // GTK2
                                GTK_SIGNAL_FUNC(SurMod_EdKeyPress), NULL);
*/

      GUI_Win_up (NULL, &win0, 0);  // always on top
      GUI_Win_go (&win0);

      // actFilt = UI_GR_Sel_Filt_set (-1);  // query only !
      // UI_GR_Sel_Filt_set (10);

      actTra = 0;
      ACTMOD = 2;
      SurMod_Ini (0);                    // init all
      sele_set__ (Typ_goGeoSUSU);          // sel only Sur|Sol
      MSG_pri_0("ModTraIn");
      return 0;


    //---------------------------------------------------------
    case UI_FuncUCB1:
      // skip disactivation
      if(GUI_DATA_EVENT == TYP_EventRelease) return 0;
      // UI_GR_Sel_Filt_set (10);
      actTra = 0;
      return 0;


    //---------------------------------------------------------
    case UI_FuncUCB2:
      // skip disactivation
      if(GUI_DATA_EVENT == TYP_EventRelease) return 0;
      // UI_GR_Sel_Filt_set (11);
      actTra = 1;
      return 0;



    //---------------------------------------------------------
    case UI_FuncUCB3:
      // skip disactivation
      if(GUI_DATA_EVENT == TYP_EventRelease) return 0;
      // UI_GR_Sel_Filt_set (12);
      actTra = 2;
      return 0;


    //---------------------------------------------------------
    // return active setting
    case UI_FuncUCB5:
      return actTra;


    //---------------------------------------------------------
    // Exit
    case UI_FuncUCB4:
      // send destroy-signal
      SurMod_Ini (1); // exit    20111-02-10
      // gtk_signal_emit_by_name (GTK_OBJECT(win0),"destroy");
      // return 0;


    //---------------------------------------------------------
    // destroy-Signal
    case UI_FuncKill:
      GUI_Win_kill (&win0);

    case UI_FuncExit:   // called from GUI_Win__
      win0 = GUI_OBJ_INVALID();

      ACTMOD = 1;
      // SurMod_Ini (1);   // exit all

      return 0;

  }

  return 0;

}


//=====================================================================
  int UI_WinSurfStyl (MemObj *mo, void **data) {
//=====================================================================
// see GUI_GetText


  static MemObj win0=GUI_OBJ_NEW, ckb1, ckb2, ckb3;
  // static int       actFilt;

  int        i1, i2, i3;
  char       *p1, cbuf[32];
  MemObj     box0;


  i1 = GUI_DATA_I1;

  printf("UI_WinSurfStyl %d\n",i1);


  switch (i1) {


    //---------------------------------------------------------
    case UI_FuncInit:  // 90

      if(GUI_OBJ_IS_VALID(&win0)) {           // Win schon vorhanden ?
        return -1;
      }

      win0 = GUI_Win__ ("change style of surface", UI_WinSurfStyl, "");

      box0 = GUI_box_v (&win0, "");

      ckb1 = GUI_radiobutt__ (&box0, " shaded ",      0, NULL, NULL, "");
      ckb2 = GUI_radiobutt__ (&box0, " symbolic ",    1, NULL, NULL, "");
      GUI_Tip("surfaceboundaries as wireframe");
      ckb3 = GUI_radiobutt__ (&box0, " transparent ", 1, NULL, NULL, "");

      GUI_button__ (&box0, "  Exit   ", UI_WinSurfStyl,
                    (void*)&GUI_FuncUCB2,"");
/*
      gtk_widget_set_events (win0,
                                GDK_KEY_PRESS_MASK);
      gtk_signal_connect (GTK_OBJECT(win0),"key_press_event",  // GTK2
                                GTK_SIGNAL_FUNC(SurMod_EdKeyPress), NULL);
*/

      GUI_Win_go (&win0);
      GUI_Win_up (NULL, &win0, 0);

      // actFilt = UI_GR_Sel_Filt_set (7);

      ACTMOD = 3;
      SurMod_Ini (0); 
      sele_set__ (Typ_goGeoSUSU);          // sel only Sur|Sol
      MSG_pri_0("ModSstIn");
      return 0;


    //---------------------------------------------------------
    // Query act. LineTypNr
    case UI_FuncUCB1:  // 1001

      if(GUI_OBJ_IS_VALID(&win0)) {
        i1 = GUI_radiobutt_get (&ckb1);
        i2 = GUI_radiobutt_get (&ckb2);
        i3 = GUI_radiobutt_get (&ckb3);
        // printf(" %d %d %d\n",i1,i2,i3);
        if(i2 > 0) i1 = 2;
        else if(i3 > 0) i1 = 3;
      }

      return i1;



    //---------------------------------------------------------
    // Exit
    case UI_FuncUCB2:  // 1002
      // send destroy-signal
      // gtk_signal_emit_by_name (GTK_OBJECT(win0),"destroy");
      // return 0;


    //---------------------------------------------------------
    // destroy-Signal
    case UI_FuncKill:  // 99
      // UI_GR_Sel_Filt_set (actFilt);  // reset
      GUI_Win_kill (&win0);

    case UI_FuncExit:   // called from GUI_Win__
      win0 = GUI_OBJ_INVALID();

      // exit all
      SurMod_Ini (1);

      return 0;

  }



  return 0;

}


//=====================================================================
  int UI_WinLtypMod (MemObj *mo, void **data) {
//=====================================================================
// modify lintype 
// see GUI_GetText


  static MemObj  win0=GUI_OBJ_NEW, ent_ltyp;
  static int     act_lnr=0;
  // static int     actFilt;

  int        i1;
  char       *p1, cbuf[32];
  MemObj     box0, w1;


  i1 = GUI_DATA_I1;

  printf("UI_WinLtypMod %d\n",i1);


  switch (i1) {


    //---------------------------------------------------------
    case UI_FuncInit:  // 90

      if(GUI_OBJ_IS_VALID(&win0)) {           // Win schon vorhanden ?
        return -1;
      }

      win0 = GUI_Win__ ("change Linetype of object", UI_WinLtypMod, "");

      box0 = GUI_box_v (&win0, "");

      sprintf(cbuf,"%d",act_lnr);
      ent_ltyp = GUI_entry__ (&box0,"new LineTypNr ",cbuf,NULL,NULL,"5");
      // GUI_Tip("see Standards/Linetyp (2=dash, 3=dash-dot");

      // GUI_label__ (&box0, " 0 ___________",  "l");
      // GUI_label__ (&box0, " 2 _ _ _ _ _ _",  "l");
      // GUI_label__ (&box0, " 3 __ . __ . __", "l");

      GUI_button__ (&box0, "edit linetypes", UI_menCB, (void*)"LtypEd", "");
      MSG_Tip ("MMstLty");
      GUI_button__ (&box0, "Exit", UI_WinLtypMod, (void*)&GUI_FuncUCB3, "");

      // necessary for GUI_WinUp !
      // gtk_signal_connect (GTK_OBJECT(win0),"key_press_event",  // GTK2
                                      // GTK_SIGNAL_FUNC(SurMod_EdKeyPress), NULL);

      GUI_Win_go (&win0);
      GUI_Win_up (NULL, &win0, 0);   // always on top

      // actFilt = UI_GR_Sel_Filt_set (6);
      ACTMOD = 4;
      SurMod_Ini (0);                    // init all
      sele_set__ (Typ_go_LCS);             // |LN|AC|CV
      MSG_pri_0("ModLtyIn");
      return 0;


    //---------------------------------------------------------
    // Query act. LineTypNr
    case UI_FuncUCB1:  // 1001
      if(GUI_OBJ_IS_VALID(&win0)) {
        p1 = GUI_entry_get (&ent_ltyp);
        act_lnr = atoi(p1);
      }
      return act_lnr;

    //---------------------------------------------------------
    // edit
    case UI_FuncUCB2:  // 1002
      // send destroy-signal
      // gtk_signal_emit_by_name (GTK_OBJECT(win0),"destroy");
      // return 0;

    //---------------------------------------------------------
    // Exit
    case UI_FuncUCB3:  // 1003
    case UI_FuncKill:   // destroy-Signal
    case UI_FuncExit:   // called from GUI_Win__

      if(GUI_OBJ_IS_VALID(&win0)) {           // Win schon vorhanden ?
        DL_hili_off (-1L); // // unhilite alle Objekte
        GUI_Win_kill (&win0);
        win0 = GUI_OBJ_INVALID();
        SurMod_Ini (1);   // exit all
      }
      return 0;

  }


  return 0;

}


// EOF
