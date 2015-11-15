//
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
2001-06-15 Startup korr. RF.

-----------------------------------------------------
*/
/*!
\file  ../xa/xa_ui_gr.c
\brief GUI graphicwindow 
\code
=====================================================
List_functions_start:

UI_CB_hide
UI_GR_ScalAuto       "butRes"/ScaleFix   & "butRes"/ScaleAll
UI_GR_view
UI_GR_Init
UI_GR_Redraw         Main RedrawFunction
UI_GR_WinInit
UI_GR_GLInit
UI_GR_Draw           When widget is exposed
UI_GR_Reshape        When glarea widget size changes
UI_GR_MotionNotify   callBack mouse-movement
UI_GR_CB_Sel1        CB of Select aus Popup-Men.
UI_GR_CB_Sel2        CB of MouseOverPopup-Eintrag
UI_GR_Select1        get objs from GL
UI_GR_Select2        Popup-Eintrag selektiert
UI_GR_Select3        hilite obj ..
UI_GR_Sel_Filter
UI_GR_ButtonPress    Mousebuttons im GrafWin (selection, indicate)
UI_GR_ButtonRelease
UI_GR_Destroy
UI_CursorWait        0=change to wait-cursor, 1=reset cursor
UI_ChangeCursor
UI_ResetCursor
UI_GR_view_set_func  unused
UI_GR_view_set_Z     ..
UI_GR_view_set_Z1
UI_GR_setKeyFocus    set focus to glarea widget
UI_GR_Indicate
UI_GR_Select_work_vc selection (only for vector)
UI_GR_Select_work1   selection
UI_GR_Select_work2   selection
UI_GR_selMen_init    create popup-menu for mousebutton-middle
UI_GR_selMen_cbSel   callback of popup-menu

UI_CK_HIDE_VIEW      test if hide or view is active                INLINE

GR_set_dispTra       inhibit display-transformations; 0=yes, 1=no

UI_GR_get_actPos_    return the active cursorPosition as string "<x> <y> <z>"
UI_GR_GetdlInd       get zuletzt selektierte DLIndex#
UI_GR_get_actPosA    get current curPos in userCoords on constructionPlane
UI_GR_SelVert        get vertex nearest to cursor
AP_Mousemove2dx      get single value from relative mousemove

AP_UserSelection_get
AP_UserSelection_reset
AP_UserMousemove_get
AP_UserMousemove_reset
AP_UserSelFunc_get   test if plugin is active

UI_GR_FROM
UI_GR_RAPID
UI_KeyFieldWri       unused

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


__________________________________________________________________
Compile:
cc -c `gtk-config --cflags` xa_ui_gr.c
cl -c /I ..\include xa_ui_gr.c

*/

#ifdef _MSC_VER
#include "MS_Def1.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include <GL/gl.h>



#include "../ut/ut_geo.h"
#include "../ut/ut_txt.h"
#include "../ut/ut_TX.h"
#include "../ut/ut_os.h"             // OS_get_bas_dir
#include "../ut/ut_cast.h"           // INT_PTR
#include "../ut/ut_err.h"            // ERR_SET1
#include "../ut/ut_gtypes.h"         // AP_src_typ__

#include "../gui/gui__.h"            // Gtk3

#include "../gr/ut_gr.h"
#include "../gr/ut_DL.h"

#include "../gr/ut_UI.h"             // FUNC_DispRend ..
#include "../gr/ut_GL.h"
#include "../gr/ut_DL.h"             // DL_GetAtt

#include "../db/ut_DB.h"

// #include "../wc/ut_nc.h"             // UI_wcg_sel1
// #include "../wc/wc_g.h"              // IE_wcTec_sel1
// #include "../wc/wc_t.h"              // IE_wcTec_sel1

#include "../xa/xa.h"
#include "../xa/xa_ui.h"               // UID_ckb_search
#include "../xa/xa_edi__.h"
#include "../xa/xa_ui_gr.h"
#include "../xa/xa_ui_cad.h"
#include "../xa/xa_mem.h"              // memspc101
#include "../xa/xa_sele.h"             // Typ_goGeom
#include "../xa/xa_app.h"              // PRC_IS_ACTIVE


#define   TRUE 1
#define   FALSE 0





//============ Externe Var: =======================================
// aus xa.c:
extern AP_STAT   AP_stat;                    // sysStat,errStat..

// extern void *UI_MainWin;
extern MemObj    UI_curPos;
extern int       WC_sur_ind;            // Index auf die ActiveConstrPlane
extern Mat_4x3   WC_sur_mat;            // TrMat of ActiveConstrPlane
extern Mat_4x3   WC_sur_imat;           // inverse TrMat of ActiveConstrPlane


// aus xa_ui.c:
extern MemObj    winMain;
extern MemObj    winGR;
extern MemObj    winED;
extern MemObj    ckb_such;
extern MemObj    ckb_meas;
extern MemObj    ckb_hide, ckb_view;
extern int       KeyStatShift;
extern int       KeyStatCtrl;
extern int       KeyStatAlt;
extern int       KeyStatEscape;
extern int       UI_Focus;
extern int       UI_InpMode;
// extern int       UI_InpSM;               // S/M/DEL; ON od OFF
extern int       UI_InpVWZ;              // ViewZ aktiv; ON od OFF
extern int       UI_EditMode;            // Add/Modify/Delete
extern char      UI_stat_view;
extern char      UI_stat_hide;
extern char      UI_stat_3D;
extern int       UI_RelAbs;                 // 0=Rel, 1=Abs
extern int       (*UI_UserKeyFunc) (); 
extern int       xa_fl_TxMem;


// aus ../ci/NC_Main.c:
extern int     APT_dispPT, APT_dispPL, APT_dispSOL;


// aus ../gr/ut_DL.c:
extern DL_Att  *GR_ObjTab;  // das ist die DL


// ex ../xa/xa_sele.c
extern int    GR_selTyp;
extern long   GR_selDbi;
extern long   GR_selDli;
extern char   GR_selNam[128];    // objname of selected object


// ex ../gr/ut_GL.c
extern int  GL_rub_stat;   // state of rubberbox; GL_RubberBox_drw__();



//============ Lokale Var: =======================================


#define GR_Func_Pan 1
#define GR_Func_Rot 2
#define GR_Func_Scl 3


/* Das GL-Window */
void *GL_widget;

// void *w_CMD_Line;         /* die textzeile im Mainwin */
// char      CMD_Line[80];        /* die textzeile im Mainwin */


int    (*UI_UserSelFunc) ();       // ob Selection an eine UserFunction geht
int    (*UI_UserMousemoveFunc) (); // ob MouseMove an eine UserFunction geht



int    KeyM1 = OFF;            /* Status der Mausbuttons */
int    KeyM2 = OFF;
int    KeyM3 = OFF;


static Point  GR_CurUk;        ///< curPos in worldCoords on constructionPlane


int    GR_Func_Act=0;             // GR_Func_Pan GR_Func_Rot GR_Func_Scl
int    GR_Event_Act;              // der GTK-Event; GDK_2BUTTON_PRESS=Doppelklick

static int    GR_do_dispTra = 0;  // do display-transformations; 0=yes, 1=no


static int    GR_Sel_Filter = 0;
#define SEL_Posi2P 1
#define SEL_Vert2P 2
#define SEL_Obj2P  3
#define SEL_dump   4


static char   GR_actPos[60];      // cursorPos as "P(x y z)"
static int    GR_optMenNr;
static long   GR_dli_hili;


// Prototypes-------------------------------------------------
int  UI_GR_selMen_cbSel (MemObj *mo, void **data);
// gint UI_GR_Scroll (void *widget, GdkEventScroll *ev);
  // int CTRL_CB__ (void *data);











//================================================================
  int UI_GL_draw__ (MemObj *mo, void **data) {
//================================================================
/// \code
/// Redraw necessary. Redraw whole scene.
///   GUI_DATA_EVENT =*(int*)data[0]=TYP_EventConfig|TYP_EventDraw|TYP_EventEnter
///   GUI_DATA_DEVICE=*(int*)data[1]=TYP_DeviceScreen0
///   GUI_DATA_I1    =*(int*)data[2]=window-width (for event=TYP_EventConfig)
///   GUI_DATA_I2    =*(int*)data[3]=window-height (for event=TYP_EventConfig)
///   GUI_DATA_I1    =*(int*)data[2]=modifierKeyState (for event=TYP_EventEnter)
/// \endcode

// was UI_GR_Draw UI_GR_Reshape
// CAN BE RECURSIVE !  (AP_init__)


  int     i1;


  // printf("UI_GL_draw__ event=%d %d %d\n",
         // GUI_DATA_EVENT, GUI_DATA_I1, GUI_DATA_I2);
  // printf("  AP_stat.sysStat=%d\n",AP_stat.sysStat);





  //----------------------------------------------------------------
  if(GUI_DATA_EVENT != TYP_EventEnter) goto L_Config;       // 300
      // printf(" _EventEnter: keyState=%d\n",GUI_DATA_I1);

    // set KeyStatShift,KeyStatCtrl,KeyStatAlt
    UI_key_mod_decode (GUI_DATA_I1);

    UI_key_view__ (GUI_DATA_EVENT, 0);

    return 0;



  //----------------------------------------------------------------
  L_Config:
  if(GUI_DATA_EVENT != TYP_EventConfig) goto L_draw;       // 306
  if(AP_stat.sysStat < 2) {

    i1 = ERR_SET1 ();
    if(i1) {
      printf("Fehler beim Aktivieren von OpenGL\n");
      // GUI_MsgBox ("Fehler beim Aktivieren von OpenGL\n");
      GUI_MsgBox (" Error activating OpenGL ");
      exit(1);
    }

    GUI_gl_set_active (1, mo);
      // config
      GL_Init__ (0, GUI_DATA_I1, GUI_DATA_I2); // width, height);
    GUI_gl_set_active (0, mo);

    // GUI_update__ ();
    ERR_RESET ();

    AP_stat.sysStat = 2;   

    return 0; // mit diesem event kein enter_notify_event möglich !

  }


  GUI_gl_set_active (1, mo);
    GL_Resize (GUI_DATA_I1, GUI_DATA_I2); // width, height);
  GUI_gl_set_active (0, mo);

  return 0;



  //----------------------------------------------------------------
  L_draw:
  if(AP_stat.sysStat < 2) return 0;



  //----------------------------------------------------------------
  if(AP_stat.sysStat < 3) {
    AP_src_new (0);   // 2013-06-19
    AP_init__ ();     // work startup-parameters, load defaultmodel
    AP_tmr_init ();
    AP_stat.sysStat = 3;
  }

  
  //----------------------------------------------------------------
  // DL_Redraw ();
  GUI_gl_set_active (1, mo);
    GL_Redraw ();
  GUI_gl_set_active (0, mo);


  if(UI_InpMode == UI_MODE_MAN) GUI_edi_Focus (&winED);

  return 0;     // TRUE=1   FALSE=0

}



//================================================================
  int UI_GL_move__ (MemObj *mo, void **data) {
//================================================================
/// \code
///   GUI_DATA_EVENT=*(int*)data[0]=TYP_EventMove
///   GUI_DATA_I1   =*(int*)data[2]=x-val mousepos in screencoords
///   GUI_DATA_I2   =*(int*)data[3]=y-val mousepos in screencoords
/// \endcode

// was UI_GR_MotionNotify

  int    actPosX, actPosY, dx, dy, ikey;
  char   buf1[128];
  Point  pt1;



  // printf(" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
  // printf("UI_GL_move__ %d %d\n", GUI_DATA_I1, GUI_DATA_I2);
  // printf(" _move__ KeyM1=%d GR_do_dispTra=%d GR_Func_Act=%d\n",
         // KeyM1,GR_do_dispTra,GR_Func_Act);
  // if(KeyM1 == ON) printf(">>>>>> UI_GL_move__ M1 is pressed ! \n");
  // printf(" _keys_mod=%d\n",GUI_get_keys_mod ());




  actPosX = GUI_DATA_I1;
  actPosY = GUI_DATA_I2;


  //----------------------------------------------------------------
  // get state of control and shift-key; the key-press-event often blocked.
  ikey = GUI_get_keys_mod ();
  if(ikey || GR_Func_Act) {
      // printf(" _keys_mod=%d\n",ikey);
    // update state of KeyStatShift, KeyStatCtrl, KeyStatAlt.
    UI_key_mod_decode (ikey);
  }



  //----------------------------------------------------------------
  // test if mousebutton-1 is down
  if(KeyM1 == ON) {
      // printf(" move-M1: test trap ..\n");
    // set GL_mouse_x_act, GL_mouse_y_act
    GL_Do_Idle (&dx, &dy, actPosX, actPosY);
    // do Rubberbox
    GL_RubberBox_do ();
    // goto L_fertig;
    goto L_exit;

  }



  if(GR_do_dispTra == 1) goto L_disp_no;  // do not modify display; report only ..

  if(GR_Func_Act != 0) goto L_disp_yes;
  // if((KeyStatShift == ON)||(KeyStatCtrl == ON)) goto L_disp_yes;




  //================================================================
  // do NO display-transformations (pan/rotate/zoom) -
  // report movement to userFunctions.
  L_disp_no:

    // set GL_mouse_x_act, GL_mouse_y_act, return relative mousemovement
    // in screencoords
    GL_Do_Idle (&dx, &dy, actPosX, actPosY);

    //  call userfunction
    if(UI_UserMousemoveFunc != NULL) {
      UI_UserMousemoveFunc (dx, dy);
    }

    goto L_fertig;


  //================================================================
  // do display-transformations (pan/rotate/zoom)
  L_disp_yes:
    GUI_gl_block (&winMain, 1); // block mousemoves, keystrokes, selections


  //------------------------------------------------------------
  // scale
  if(GR_Func_Act == GR_Func_Scl) {

    UI_GR_DrawInit ();
    GL_Do_Scale__ (actPosX, actPosY);
    UI_GR_DrawExit ();

    // Ausgabefeld Scale setzen
    UI_AP (UI_FuncSet, UID_ouf_scl, NULL);


  //----------------------------------------------------------------
  // MOVE
  } else if(GR_Func_Act == GR_Func_Pan) {

    UI_GR_DrawInit ();
    GL_Do_Pan__ (actPosX, actPosY);
    GL_Redraw ();                                    //2009-12-31
    UI_GR_DrawExit ();


  //------------------------------------------------------------
  // rotate
  } else if(GR_Func_Act == GR_Func_Rot) {

    // Checkbox 3D == ON: skip rotate.
    if(UI_stat_3D == 1) {
      UI_GR_DrawInit ();
      GL_Do_Rot__ (actPosX, actPosY);
      GL_Redraw ();                                    //2009-12-31
      UI_GR_DrawExit ();
    }
  }

  
  // unblock
  GUI_gl_block (&winMain, 0);   // unblock keystrokes & grafic_selections
  goto L_fertig;



  //----------------------------------------------------------------
  L_weiter:
  // save -> GL_mouse_x_act
  GL_Do_Idle (&dx, &dy, actPosX, actPosY);



  //----------------------------------------------------------------
  L_fertig: // L_weiter:

  //  die Cursorpos auf der ConstrPlane in uk's errechnen und anzeigen
  GR_set_constPlnPos ();  // compute GR_CurUk in worldCoords

  UI_GR_get_actPosA (&pt1);   // get GR_CurUk


  sprintf(buf1, "%+10.3f %+10.3f %+10.3f",pt1.x,pt1.y,pt1.z);
    // printf("  UI_curPos formatted %s\n",buf1);
  GUI_label_mod (&UI_curPos, buf1);


  L_exit:

  return 0;

}


//================================================================
  int UI_GL_mouse__ (MemObj *mo, void **data) {
//================================================================
/// \code
/// callback mouseButton press | release | scroll
///   GUI_DATA_EVENT=*(int*)data[0]=TYP_EventPress|TYP_EventRelease
///   GUI_DATA_I2   =*(int*)data[1]=button; GUI_MouseL|M|R|2L|ScUp|ScDown
/// \endcode

// was UI_GR_ButtonPress UI_GR_ButtonRelease UI_GR_Scroll


  int     iButt, irc, mode, typ, i1, i2;
  long    dbi, dli, objInd=-1L;
  double  d1;
  char    s1[16];
  Point   pt1;


  iButt = GUI_DATA_I2;


  // printf("UI_GL_mouse__ event=%d %d\n", GUI_DATA_EVENT, iButt);
  // printf("  GR_Sel_Filter=%d\n",GR_Sel_Filter);
  // printf(" TUT_stat=%d\n",AP_stat.TUT_stat);



  //================================================================
  // TYP_EventRelease .. 303
  if(GUI_DATA_EVENT != TYP_EventRelease) goto L_ev_press;


  if(iButt == GUI_MouseL) {
    KeyM1 = OFF;
      // printf(" KKKKKKKKKKKKKKKKKKKKKKKKKKK release M1\n");
    // is rubberbox active: close !
    if(GL_rub_stat) GL_RubberBox_stop ();


  } else if(iButt == GUI_MouseM) {
    // printf(" reset KeyM2\n");
    KeyM2 = OFF;


  } else if(iButt == GUI_MouseR) {
    // printf(" reset KeyM3\n");
    KeyM3 = OFF;
  }

  return 0;



  //================================================================
  // TYP_EventPress .. 302
  L_ev_press:


  // skip sel if locked ..
  if(iButt == GUI_MouseL)  {
    if(GR_Sel_Filter == 20) return FALSE;
  }


  // ScreenCast ON: draw mouse ..
  if(AP_stat.TUT_stat) {
     GUI_TUT_m__ (iButt);
  }


  GUI_gl_block (&winMain, 1); // block mousemoves, keystrokes & grafic_selections
  UI_CursorWait (0);    // wait-cursor



  //----------------------------------------------------------------
  // Ctrl + M1 = inhibit objSelection (for group-selection)
  // selection: set mouseposition
  if((KeyStatCtrl == OFF)&&(KeyStatShift == OFF)) {    // nur ohne Shift/Ctrl-key

    // DL_hili_off (-1L);// reset hili: ohne dieser zeile doppelte Obj in popup ?
    // Punkt auf Flaeche geht dzt nur damit (no hilited surfaces) ...
    GL_Redra__ (0);       // find selected point
    GL_MousePos (&pt1);
    sele_set_pos (&pt1);

    // get userCoords on viewPlane & constructionPlane 2009-05-25
    GR_set_constPlnPos();    // set point GR_CurUk on constructionPlane
    GL_set_viewPlnPos ();    // set point GL_actUsrPos on viewPlane
  }



  //================================================================
  // handle scroll-events
  if((iButt == GUI_MouseScUp)||(iButt == GUI_MouseScDown)) {

    GUI_gl_block (&winMain, 1); // block mousemoves, keystrokes, selections
    UI_GR_DrawInit ();
    if(iButt == GUI_MouseScUp) i1 = -1;
    else                       i1 = 1;
    GL_Do_Scale__ (UT_INT_MAX, i1);
    UI_GR_DrawExit ();
    GUI_gl_block (&winMain, 0);   // unblock keystrokes & grafic_selections
    UI_AP (UI_FuncSet, UID_ouf_scl, NULL); // reset scale

    // GR_set_constPlnPos ();  // compute GR_CurUk in worldCoords
    // UI_GR_get_actPosA (&pt1);   // get GR_CurUk

    goto Fertig;

  }




  //================================================================
  //========== M1|M3==================
  if(iButt == GUI_MouseR)  {
    // M3
    if(KeyStatCtrl == ON)  {
      // Ctrl-M3
      // applic active ?
      if(GR_Sel_Filter == 17) { PRG_key_CB ("CMBR"); goto Fertig; }
      if(UI_InpMode == UI_MODE_CAD) {
        if(IE_get_Func() < 0) {
          UI_EdKeyCtrlM3 (); // disp grp-menu
          goto Fertig;
        }
      } else if((UI_InpMode == UI_MODE_VWR) ||
                (UI_InpMode == UI_MODE_MAN))  {
        UI_EdKeyCtrlM3 (); // disp grp-menu
        goto Fertig;
      }
    } else {
      // M3 without Ctrl
      if(GR_Sel_Filter == 17) { PRG_key_CB ("MBR"); goto Fertig; }
    }
  }

  if(iButt != GUI_MouseM)  {

    // M1,M3
    if(iButt == GUI_MouseL) {
      // M1
        // printf("KKKKKKKKKKKKKKKKKKKKKKKKK KeyM1 = ON \n");
      GR_Event_Act = GUI_MouseL;
      KeyM1 = ON;

    } else {        // M3
      KeyM3 = ON;
      GR_Event_Act = GUI_MouseR;
    }


    // check for double-click
    if(iButt == GUI_Mouse2L) {
      GR_Event_Act = GUI_Mouse2L;
        printf("Doppelklick\n");
        // geht bei select nicht, weil man manchmals aus Liste waehlen muss...
        DL_Redraw ();  // CAD: after selection ConstPlane is unvis
        goto Fertig;
    }

    // Check ob ViewZ aktiv ist
    // UI_AP (UI_FuncGet, UID_ckb_vwz, (void*)&ckb_vwz_stat);
    // if(ckb_vwz_stat == 0) mode = 1;          // ViewZ YES

    if(UI_InpVWZ == ON) {
      mode = -1;


    } else {
      // M1 | M3
      mode = 0;

      if(GUI_ckbutt_get (&ckb_meas) == 1) goto L_ck_sel;       //2013-05-06

      if(UI_InpMode == UI_MODE_CAD) {        
        if(GR_Event_Act == GUI_MouseR) {
          // M3 in CAD
          // M3: if CAD-OK is active: no menu, give OK=Enter.  2010-04-29
            printf(" Func=%d\n",IE_get_Func());
            printf(" TypAct=%d\n",IE_get_inp_TypAct());

          if(IE_get_Func() >= 0) {
            // Ctrl-M3 = OK
            if(KeyStatCtrl == ON)  {
              UI_EdKeyCR (2);                  // 2013-04-09
            } else {
              IE_inp_selection_clear ();       // clear selection
              // IE_cad_InpIn_CR ();              // do CR
              UI_GR_selMen_init (2);            // 2013-04-08
            }
            goto Fertig;
          }
        }
/*
        // wenn CAD / seek Vertex aktiv: sofort retour.
        IE_cad_get_func (&i1, &i2);
        if((i1 == 15) && (i2 == 4)) {   // IE_cad_z 4
          objInd = -1L;
          goto L_11;
          // UI_GR_Select_work1 (0L);
          // return 0;
        }
*/

      //----------------------------------------------------------------
      } else if(UI_InpMode == UI_MODE_MAN) {
        // M1|M3 in MAN
        if(UI_UserSelFunc != NULL) goto L_ck_sel;  // 2015-08-11
        i1 = ED_query_CmdMode (); // analyze active line; -1=empty, 0=DefLn..
          // printf(" MAN; ev=%d i1=%d\n",GR_Event_Act,i1);

        if(GR_Event_Act == GUI_MouseL) {
          // M1 in MAN
          if((UI_stat_hide)&&(UI_stat_view)) {
            // hide,view not active;
            if(i1 < 0) {
              // empty line, create def-menu (P L C ..)
              UI_GR_selMen_init (0);                  // MAN,M1,empty
              goto Fertig;
            }
          }

        } else if(GR_Event_Act == GUI_MouseR) {
          // M3 in MAN
          // test if active line = definitionLine; if yes: activate menu
          // if process is active: do process-subMenu
          if(PRC_IS_ACTIVE) {
            sprintf(s1, "MBR_%d", i1);
              // printf(" subMen for process |%s|\n",s1);
            PRC__ (-1, s1);  // report M3 to process
            goto Fertig;
          }
          // M3: wait for selection of objects; goes -> OMN_selMen_MAN_M3_empty
        }
      }
    }


    //--------------------------------------------------
    // ist SelectionFilter definiert ?
    L_ck_sel:
    if(GR_Sel_Filter != 0) {
      if(GR_Sel_Filter < 3) {  // vertex
        i1 = GR_Sel_Filter;
        UI_GR_Sel_Filter (0);  // reset   2013-04-08
        if(i1 == SEL_Posi2P) UI_GR_Indicate();
        else if(i1 == SEL_Vert2P) UI_GR_SelVert(NULL);
        goto Fertig;
      }
    }

    // remove last selection (avoid that, if noPickable obj selected, the
    // previous selected obj is stored
    UI_GR_set_sel__ (-1L);


    // no selection if shift or ctrl is on        2010-09-28
    if((KeyStatCtrl != OFF)||(KeyStatShift != OFF)) {
      // printf(" skip selection if shift or ctrl is on\n");
      goto Fertig;
    }

    // selection (Obj analysieren)
    irc = UI_GR_Select1 (mode, &objInd);
    // irc=0: more than 1 obj's found !
      // printf(" nach UI_GR_Select1 %d %d %ld\n",irc,mode,objInd);
      // printf(" GR_Sel_Filter=%d GR_selTyp=%d\n",GR_Sel_Filter,GR_selTyp);

    // GL_Select (mode, &objInd);
    // GR_SelObj = objInd;              // der DLindex des zuletzt sel. obj
    // hier kein swapuffers!
    //GRView_DrawExecute ();



    // if(ckb_vwz_stat == 0) {        // ViewZ YES               do RotCen
    if(UI_InpVWZ == ON) {
      UI_GR_view_set_Z (objInd);
      UI_InpVWZ = OFF;
      goto Fertig;
    }

    L_11:
      // printf(" L_11: objInd=%ld\n",objInd);


    // user-plugin-callback ?
    if((UI_UserSelFunc != NULL)&&(irc))  {                // 2011-06-26
      // selection or indicate (-1); use UI_GR_get_selNam
      UI_UserSelFunc (GR_Event_Act, objInd);
      goto Fertig;
    }


    //------- M3-Indicate ----------------------
    if((GR_Event_Act == GUI_MouseR) &&           // M3
       (irc == 1) &&
       (GR_selTyp == Typ_TmpPT)) {                 // indicate
        // printf(" M3-ind.\n");
      DL_Redraw ();           // else grid is missing ..
      TX_Print (" .. group is empty, nothing selected - ?");
    }



    //------- M1-Indicate ----------------------
    if((GR_Event_Act == GUI_MouseL) &&           // M1
       (irc == 1) &&                               // nr of objs
       (GR_selTyp == Typ_TmpPT)) {                 // indicate

        // printf(" indicate ..\n");

      // indicate in group-mode useless ..
      if(GR_Sel_Filter == 5) {
        TX_Print("***  groupmode; select objects ..");
        goto Fertig;
      }

      // if((ckb_search_stat == 0) ||    // suchen YES
      // if((UI_InpSM == ON)       ||    // suchen YES (ON=0)
      // if "View" or Hide" is active:
      if(UI_CK_HIDE_VIEW) {
        // TX_Error ("- nix gfundn");
        TX_Write (" - ? ");
        DL_hili_off (-1L);
        DL_Redraw ();
        goto Fertig;
      }

      // SMD:
      if(UI_InpMode != UI_MODE_VWR) {
        if(KeyStatCtrl == ON) {
          TX_Print("select object to modify/delete ...");
          goto Fertig;
        }
      }

      // UI_GR_Indicate ();  // or call UI_UserSelFunc
      // goto Fertig;
    }



    // one obj selected
    if(irc == 1) {
      GL_sel_get (&typ, &dbi, &dli, 0);          // get GR_selTab[0]
      UI_GR_Select2 (typ, dbi, dli);             // work obj selected
    }

    // more than 1 obj in selectionSet
    goto Fertig;
  }




  //========== M2 ====================================================
  // mousebutt middle
  // if (state & GDK_BUTTON2_MASK) {
  if (iButt == GUI_MouseM) {
    KeyM2 = ON;
    GR_Event_Act = GUI_MouseM;
    /* printf("B2\n"); */
    // CI_Mouse (2,KeyStatShift,KeyStatCtrl,KeyStatAlt,x,y);
    if(UI_UserSelFunc != NULL) {
      UI_UserSelFunc (GUI_MouseM, objInd);
      goto Fertig;
    }

    // ist CAD aktiv ?
    if(UI_InpMode == UI_MODE_CAD) {
      IE_cad_selM2 (0);
    }

    // if(UI_InpMode == UI_MODE_MAN) {
      // UI_GR_selMen_init ();
    // }

  }




  //================================================================
  Fertig:
  // printf("ex UI_GR_ButtonPress %d %d %d\n",x, y, state);
  UI_CursorWait (1);            // reset cursor from wait
  GUI_gl_block (&winMain, 0);   // unblock keystrokes & grafic_selections
  return FALSE;



}


//================================================================
  int UI_key_mod_set (int event, int ikey, int keyMod) {
//================================================================
/// update modifier-keys (KeyStatShift, KeyStatAlt, KeyStatCtrl);
///   keyMod       &1=shift; &4=ctrl; &8=alt; 0=normal.
///   KeyStatShift,KeyStatAlt,KeyStatCtrl: 1=off, 0=on.

// see also UI_key_view__

  int   mode;


  // printf("UI_key_mod_set %d %d %d\n",event,ikey,keyMod);


  //----------------------------------------------------------------
  // update from keyMod               2014-12-18
  KeyStatShift = keyMod & 1 ? 0 : 1;
  KeyStatCtrl  = keyMod & 4 ? 0 : 1;
  KeyStatAlt   = keyMod & 8 ? 0 : 1;

 
  //----------------------------------------------------------------
  // update from event/ikey
  if     (event == TYP_EventPress)   mode = ON;
  else if(event == TYP_EventRelease) mode = OFF;
  else return 0;

  switch (ikey) {
    
    case GUI_KeyShift_L:
    case GUI_KeyShift_R: 
        // printf("Shift chg\n");
      KeyStatShift = mode;
      break;


    case GUI_KeyAlt_L:
    case GUI_KeyAlt_R:
    case GUI_KeyMeta_L:                  // 2013-03-12
        // printf("Alt chg\n");
      KeyStatAlt = mode;
      break;
      // goto L_disp1;


    case GUI_KeyControl_L:
    case GUI_KeyControl_R:
        // printf("ctrl chg\n");
      KeyStatCtrl = mode;
      break;


    default:
      return 0;
  }


  // printf("ex UI_key_mod_set KeyStatShift=%d KeyStatCtrl=%d KeyStatAlt=%d\n",
          // KeyStatShift,KeyStatCtrl,KeyStatAlt);

  return 0;

}

 
//================================================================
  int UI_key_mod_decode (imod) {
//================================================================
// decode state of modier-keys; set state of shift|Ctrl|Alt-key.



  // set state of shift|Ctrl|Alt-key.
  if(imod & GUI_Modif_shift) KeyStatShift = ON;
  else                       KeyStatShift = OFF;

  if(imod & GUI_Modif_ctrl)  KeyStatCtrl = ON;
  else                       KeyStatCtrl = OFF;

  if(imod & GUI_Modif_alt)   KeyStatAlt = ON;
  else                       KeyStatAlt = OFF;



  // update GR_Func_Act & cursorType
  UI_key_view_decode ();

  return 0;

}


//================================================================
  int UI_key_view_decode () {
//================================================================
// update GR_Func_Act & cursorType


  if((KeyStatShift == ON)&&(KeyStatCtrl == ON)) {
    if(GR_Func_Act != GR_Func_Scl) {
      GR_Func_Act = GR_Func_Scl;
      UI_ChangeCursor (4);  // GDK_SIZING);   // f zoom
    }
    return 1;


  } else if(KeyStatShift == ON) {
    if(GR_Func_Act != GR_Func_Pan) { 
      GR_Func_Act = GR_Func_Pan;
      UI_ChangeCursor (3);  // GDK_HAND1);    // f move
    }
    return 1;

    
  } else if(KeyStatCtrl == ON) {
    if(GR_Func_Act != GR_Func_Rot) {
      GR_Func_Act = GR_Func_Rot;
      UI_ChangeCursor (2);   // GDK_EXCHANGE); rotate
    }
    return 1;

  }


  if(GR_Func_Act != 0) {
    GR_Func_Act = 0;
    UI_ResetCursor ();
  }

  return 0;

}

 
//================================================================
  int UI_key_view__ (int imod, int ikey) {
//================================================================
/// \code
/// TODO: use UI_key_mod_set
/// do grafic-viewing (check for shift|Ctrl|Alt-key)
/// Input:
///   imod    eventTyp; TYP_EventEnter|TYP_EventPress|TYP_EventRelease
///   ikey    keyCode
/// RetCod:   0 = not used (no shift|Ctrl|Alt-key)
///           1 = found shift|Ctrl|Alt-key
/// see also UI_GL_keys__
/// \endcode


  // printf("UI_key_view__ %d %d\n",imod,ikey);
  // printf("  KeyStatCtrl=%d\n",KeyStatCtrl);


  // KeyStatShift or KeyStatCtrl or KeyStatAlt is modified:
  if(imod == TYP_EventEnter) goto L_disp1;



  //================================================================
  if(imod == TYP_EventPress) {

  switch (ikey) {

  case GUI_KeyShift_L:
  case GUI_KeyShift_R:
      // printf("Shift ON\n");
    KeyStatShift = ON;
    if(KeyStatAlt == ON) return 1;    // special-func   2013-03-12
    goto L_disp1;


  case GUI_KeyAlt_L:
  case GUI_KeyAlt_R:
  case GUI_KeyMeta_L:                  // 2013-03-12
      // printf("Alt ON\n");
    KeyStatAlt = ON;
    return 1;                          // special-func   2013-03-12
    // goto L_disp1;


  case GUI_KeyControl_L:
  case GUI_KeyControl_R:
      // printf("ctrl ON\n");
    KeyStatCtrl = ON;
    goto L_disp1;

  }


  //================================================================
  } else if(imod == TYP_EventRelease) {

  switch (ikey) {


  case GUI_KeyShift_L:
  case GUI_KeyShift_R:
      // printf("Shift OFF\n");
    KeyStatShift = OFF;
    goto L_disp1;


  case GUI_KeyAlt_L:
  case GUI_KeyAlt_R:
      // printf("Alt OFF\n");
    KeyStatAlt = OFF;
    goto L_disp1;


  case GUI_KeyControl_L:
  case GUI_KeyControl_R:
      // printf("ctrl OFF\n");
    KeyStatCtrl = OFF;
    goto L_disp1;

  }

  }

  return 0;


  //================================================================
  L_disp1:

  // update GR_Func_Act & cursorType    
  if(GR_do_dispTra == 1) return 0;        // do not modify display; report only ..

  // update GR_Func_Act & cursorType
  return UI_key_view_decode ();

}


//================================================================
  int UI_TUT_key (int ikey) {
//================================================================
/// ScreenCast-mode: display key
// see aLso ../ut/ut_gtypes.c

  char    s1[80], s2[32], s3[32];


  // printf("UI_TUT_key %d\n",ikey);

  s3[0] = '\0';

  if(KeyStatCtrl == ON) strcat (s3, "Ctrl-");
  if(KeyStatShift == ON) strcat (s3, "Shift-");
  if(KeyStatAlt == ON) strcat (s3, "Alt-");


  switch (ikey) {

    case GUI_KeyEsc:
      strcpy (s2, "Esc");
      break;

    case GUI_KeyTab:
      strcpy (s2, "Tab");
      break;

    case GUI_KeyF3:
      strcpy (s2, "F3");
      break;

    case GUI_KeyF4:
      strcpy (s2, "F4");
      break;

    case GUI_KeyF5:
      strcpy (s2, "F5");
      break;

    case GUI_KeyDel:                  // "Entf" - Taste
    case GUI_KeyNumDel:
      strcpy (s2, "Del");
      break;

    case GUI_KeyPageUp:
      strcpy (s2, "PageUp");
      break;

    case GUI_KeyPageDown:
      strcpy (s2, "PageDown");
      break;

    case GUI_KeyBackSpace:
      strcpy (s2, "BackSpace");
      break;

    case GUI_KeyReturn:
      strcpy (s2, "Return");
      break;

    default:
      // int -> char
      // strcpy (s2, "?");
      sprintf(s2, "%c", ikey);

  }

  sprintf(s1, "<b> key: %s%s </b>", s3, s2);
  GUI_TUT_t__ (s1);

  return 0;

}
 

//================================================================
  int UI_GL_keys__ (MemObj *mo, void **data) {
//================================================================
/// \code
///   GUI_DATA_EVENT=*(int*)data[0]=TYP_EventPress|TYP_EventRelease
///   GUI_DATA_EVENT=*(int*)data[0]=TYP_EventPress|TYP_EventRelease
///   GUI_DATA_I1   =*(int*)data[1]=TYP_DeviceKeyb
///   GUI_DATA_I2   =*(int*)data[2]=keyvalue; eg 'a'
///   GUI_DATA_I3   =*(int*)data[3]=state of modifierkeys;
///                                  see ../gui/gui_types.h  GUI_Modif_*
///                                   &1=shift; &4=ctrl; &8=alt.
/// Gtk3/Ubu-14 does not provide Ctrl-Press !
/// \endcode

// was UI_EdKeyPress   (UI_GR_KeyPress)
// unused: keyIn in VWR mode;
//       } else if((iKey == GDK_BackSpace) ||        // RemoveLeftChar-Taste  <x|
//                 (iKey == GDK_Delete))       {     // Entf
//         i1 = strlen(xbuf);
//         // printf(" backSp %d |%s|\n",i1,xbuf);
//         if(i1 > 0) {
//           xbuf[i1-1] = '\0';
//           GUI_Tx_rmLast ();
//           TX_Print("...... sel. obj %s",xbuf);
//         }
// 
//       } else {
//         // check for alfanum-char ...
//         if((isalnum(iKey)) && (iKey < 125)) {
//         // printf(" iKey=%d\n",iKey);
//         // add character to buffer xbuf
//         i1 = strlen(xbuf);
//         // printf(" add key %d %d |%s|\n",iKey,i1,xbuf);
//         xbuf[i1] = iKey;
//         xbuf[i1+1] = '\0';
//         GUI_Tx_rmLast ();
//         TX_Print("...... sel. obj %s",xbuf);
//         }

 
  static int iCur = 0, selFi, oldEvent;
  int    iKey, uKey, irc;


  // printf("UI_GL_keys__ event=%d  key=%d  modif=%d\n",
           // GUI_DATA_EVENT,    // event:    TYP_EventPress|TYP_EventRelease
           // GUI_DATA_I2,       // key:      ascii-value of key
           // GUI_DATA_I3);      // modifier: &1=shift; &4=ctrl; &8=alt.


  iKey = GUI_DATA_I2;

  selFi = UI_GR_Sel_Filter(-1);
    // printf(" selFi=%d\n",selFi);


  // update modifier-keys 
  UI_key_mod_set (GUI_DATA_EVENT, GUI_DATA_I2, GUI_DATA_I3);
  // UI_key_mod_decode (GUI_DATA_I3);
    // printf("KeyStatShift=%d KeyStatCtrl=%d KeyStatAlt=%d\n",
      // KeyStatShift,KeyStatCtrl,KeyStatAlt);



  // check for shift|Ctrl|Alt-key  - do grafic-viewing
  if(!isascii(GUI_DATA_I1)) {
    irc = UI_key_view__ (GUI_DATA_EVENT, iKey);
    if(irc) return 1;
  }



  //================================================================
  // TYP_EventPress
  if(GUI_DATA_EVENT == TYP_EventPress) {
    // printf(" press key %d %x\n",iKey,iKey);

  // ScreenCast ON: display keypress
  if(AP_stat.TUT_stat) {
     UI_TUT_key (iKey);
  }

  switch (iKey) {


  case GUI_KeyEsc:
    KeyStatEscape = ON;
    if(oldEvent != TYP_EventPress) UI_key_escape ();
    oldEvent = GUI_DATA_EVENT;   // 2014-04-10
    goto L_exit;


  case GUI_KeyCurLeft:
    if(KeyStatCtrl == ON) { vwpt__ (2); return 1; }   // last viewport
    if((selFi == 14)||(selFi == 15)) return UI_Tex_Key (iKey);
    if((UI_InpMode == UI_MODE_CAD) && (selFi == 0)) {
      // goto end of upper cad-inputField
      IE_cad_InpIn_left();
    }
    // goto L_exit;
    return 1; // skip defaultoperation


  case GUI_KeyCurRight:
    if(KeyStatCtrl == ON) { vwpt__ (1); return 1; }   // nxt viewport
    if((selFi == 14)||(selFi == 15)) return UI_Tex_Key (iKey);
    if(UI_InpMode == UI_MODE_CAD) {
      IE_inp_chg (-1);  // next inputfield
    }
    // goto L_exit;
    return 1; // skip defaultoperation


  case GUI_KeyCurUp:
    if((selFi == 14)||(selFi == 15)) return UI_Tex_Key (iKey);
    if(UI_InpMode == UI_MODE_CAD) {
      IE_inp_chg (-2);  // previous inputfield
    }
    return 1; // skip defaultoperation


  case GUI_KeyCurDown:
    if((selFi == 14)||(selFi == 15)) return UI_Tex_Key (iKey);
    if(UI_InpMode == UI_MODE_CAD) {
      IE_inp_chg (-1);  // next inputfield
    }
    return 1; // skip defaultoperation


  case GUI_KeyTab:
    if(UI_InpMode == UI_MODE_CAD) {
      IE_inp_chg (-1);  // next inputfield
      return 1; // skip defaultoperation
    }
    // ACHTUNG: Tab deaktiviert das Grafic-window (Gtk-Func !)
    return 1; // skip defaultoperation
  }


  // ctrl-keycodes not sent to user:
  if(KeyStatCtrl == ON) {
    if(iKey < 125) {
      uKey = toupper (iKey);

      if(uKey == 'V') { vwpt__ (0); goto L_exit; }    // save VIEWPORT

    }
  }


  // all other keys -> userFunc
  if(UI_UserKeyFunc != NULL) {
    (*UI_UserKeyFunc) (iKey);
    goto L_exit;
  }


  // // handle active selectionFilter
  // if(selFi != 0) return UI_key_sel_fi (iKey);  ?? 2012-08-21


  //================================================================
  switch (iKey) {

  //----------------------------------------------------------------
  case GUI_KeyF3:
    UI_men__ ("Edit");
    // mode VWR: imply "RUN"
    if(UI_InpMode == UI_MODE_VWR) {
      ED_work_END (0);
    }
    goto L_exit;

  //----------------------------------------------------------------
  case GUI_KeyF4:
    AP_APT_clean ();
    goto L_exit;

  //----------------------------------------------------------------
  case GUI_KeyF5:
    if(UI_InpMode == UI_MODE_CAD) {
      // display list - modelfiles|catalogParts|subModels
      IE_cad_selM2 (0);
    }
    goto L_exit;


  //----------------------------------------------------------------
  case GUI_KeyDel:                  // "Entf" - Taste
  case GUI_KeyNumDel:
     // printf(" delete .. %d %d\n",UI_InpMode,IE_get_Func());
    if(UI_InpMode == UI_MODE_CAD) {     // 3
      // skip in CAD if func is active                        // 2013-03-18
      if(IE_get_Func() < 0) {          // no func active ..
        UI_key_delete ();
      } else {
        TX_Print("*** exit CAD-function (Esc Esc ..) for delete-operation ..");
      }
    } else {
      TX_Print("*** delete objects only in CAD or MAN ..");    // 2013-09-04
    }
    goto L_exit;


  //----------------------------------------------------------------
  case GUI_KeyPageUp:
  case GUI_KeyPageDown:
    if(UI_InpMode == UI_MODE_CAD) {
      IE_gr_key_CB (data);
    }
    goto L_exit;



  //----------------------------------------------------------------
  case GUI_KeyBackSpace:
    if(UI_InpMode == UI_MODE_CAD) {
      // del last char of active inputfield
      IE_inp_mod__ (1, NULL);
    }
    goto L_exit; 


  //----------------------------------------------------------------
  case GUI_KeyReturn:
    UI_key_return ();
    goto L_exit;



  //----------------------------------------------------------------
  }



  if(KeyStatCtrl == ON) {
    // do special-function with ctrl + ?
    UI_key_spcCtrl (iKey);
    goto L_exit; 
  }

    // printf(" shi=%d alt=%d\n",KeyStatShift,KeyStatAlt);
  if((KeyStatShift == ON)&&(KeyStatAlt == ON)) {
    // do special-function with shift + alt + ?
    UI_key_spcShAlt (iKey);
    goto L_exit; 
  }


  if(KeyStatAlt == ON) {
    // do special-function with alt + ?
    UI_key_spcAlt (iKey);
    goto L_exit; 
  }


  // keypress - no special action
    // printf("  keypress - no special action\n");
  if(UI_InpMode == UI_MODE_CAD) {
    // test if a menu is active
    if((IE_get_Func() >= 0)) {
        // printf(" iKey=%d %p\n",iKey,&iKey);
      // add iKey to active inputfield
      if(iKey < 256) {                         // 2013-05-01
        IE_inp_mod__ (0, &iKey);
      }
    }
  }


  } // else {} // skip key-release


  L_exit:  // do defaultoperations for this keypress..
    oldEvent = GUI_DATA_EVENT;
  return 0;

}


//================================================================
  int UI_key_spcAlt (int iKey) {
//================================================================
/// work keys with Alt

  // printf("UI_key_spcAlt %d\n",iKey);
    

  switch (iKey) {

    case 'x':
    case 'X':
      // if(UI_InpMode == UI_MODE_VWR) break;
      // if(UI_InpMode == UI_MODE_MAN) {
      AP_work__ ("crashEx", NULL);
      goto AllDone;
      break;


  }


  AllDone:
    return 0;

}


//================================================================
  int UI_key_spcCtrl (int iKey) {
//================================================================
/// work keys with Ctrl


  char   cbuf[256];


  // printf("UI_key_spcCtrl %d\n",iKey);


  switch (iKey) {

  //----------------------------------------------------------------
  case 'a':
  case 'A':
    // add all objs to group ..
      // printf(" UI_stat_view=%d\n",UI_stat_view);
    DL_grp1__ (0, NULL, 2, 0);
    if(UI_stat_view == 0) {
      Grp_hide (1);    // view group
    }
    goto AllDone;


  case 'e':
  case 'E':
    if(APP_act_typ == 1)  PRG_Ed ();      // applic.
    goto Reset_Ctrl;  // 2013-09-01    else rotate is on !



  case 'f':
  case 'F':
    // if(UI_InpMode == UI_MODE_VWR) goto L_cad_man;
    // UI_AP (UI_FuncSet, UID_ckb_search, (void*)TRUE);  // SMD - On setzen
    // UI_mcl__ (NULL, PTR_INT(UI_FuncInit));   // activate Search/Name
    GUI_ckbutt_set (&ckb_such, TRUE);
    goto Reset_Ctrl;  // 2013-09-01    else rotate is on !


  case 'p':
  case 'P':
      // printf("APP_act_typ = %d APP_act_nam = |%s|\n",APP_act_typ,APP_act_nam);
    if(APP_act_typ == 1)  PRG_start ();      // applic.
    if(APP_act_typ == 2)  PRC_restart ();    // process
    if(APP_act_typ == 3)  PLU_restart ();    // plugin
    if(APP_act_typ == 4)  RPC_restart ();    // remote
    // reset the Ctrl-key
    goto Reset_Ctrl;



  case 's':
  case 'S':
    // sichern ..
    strcat(WC_modnam, ".gcad"); // add filetyp
    UI_save__ (1);              // save-overwrite
    UTX_ftyp_cut (WC_modnam);   // remove Filetyp !!!
    // reset the Ctrl-key
    goto Reset_Ctrl;


  case 'T':                     // Ctrl Shift t
    AP_test__ ();
    // KeyStatShift = OFF;
    // goto Reset_Ctrl;
    goto AllDone;


  case 'y':
  case 'Y':
    // if(UI_InpMode == UI_MODE_VWR) break;
    // printf("Ctrl-Y  UI_InpMode=%d\n",UI_InpMode);
    if(UI_InpMode == UI_MODE_MAN) {
      // return FALSE;
      sprintf(cbuf,"%sselection.txt",OS_get_tmp_dir());
      GUI_edi_InsFile (&winED, cbuf);
      goto AllDone;
    }
    // Redo last ElementCreate
    UNDO_grp_redo ();
    goto AllDone;


  case 'z':
  case 'Z':
    // if(UI_InpMode == UI_MODE_VWR) break;
      // printf("Ctrl-Z  UI_InpMode=%d\n",UI_InpMode);
    if(UI_InpMode == UI_MODE_MAN) {
      // return FALSE;
      // gtk_signal_emit_by_name (GTK_OBJECT(winED.win),"activate");
      sprintf(cbuf,"%sselection.txt",OS_get_tmp_dir());
      GUI_edi_InsFile (&winED, cbuf);
      goto AllDone;
    }
    UNDO_grp_undo ();  // CAD & VWR
    goto AllDone;


  }


  AllDone:
    return 0;


  Reset_Ctrl:
    UI_key_view__ (TYP_EventRelease, GUI_KeyControl_L);
    goto AllDone;

}

 
//================================================================
  int UI_key_spcShAlt (int iKey) {
//================================================================
// Alt shift ..

  int    i1;
  char   cbuf[256];


  // printf("UI_key_spcShAlt %d\n",iKey);


  // if(KeyStatAlt == ON) {           // Alt Shift .
    i1 = toupper(iKey);  // im MS-Win kommt immer uppercase !

    if(i1 == 'V') {
      // printf(" add ViePortDef\n");
      UI_men__ ("ViewP");
      goto AllDone;
    }


    // if(i1 == 'G') {GL_tst1 ();       goto AllDone;} // Alt shift z

      // printf(" auxFun %d\n",iKey);
    if(i1 == 'A') {GA_dump__ (NULL); goto AllDone;} // Alt shift g
    if(i1 == 'D') {DL_DumpObjTab (); goto AllDone;} // Alt shift d
    if(i1 == 'G') {Grp_dump ();      goto AllDone;} // Alt shift g
    if(i1 == 'M') {DB_dump_ModBas ();goto AllDone;} // Alt shift m
    // if(i1 == 'N') {WC_actPos_dump ();goto AllDone;} // Alt shift n
    if(i1 == 'O') {DB_dump_stat ();  goto AllDone;} // Alt shift O
    if(i1 == 'R') {DB_dump_ModRef ();goto AllDone;} // Alt shift r
    if(i1 == 'S') {UTF_dump__ ();    goto AllDone;} // Alt shift S
    if(i1 == 'T') {Tex_dump__(NULL); goto AllDone;} // Alt shift T
    if(i1 == 'U') {UNDO_dump ();     goto AllDone;} // Alt shift U     Undo-Tab
    if(i1 == 'X') {AP_debug__ ("key X");    goto AllDone;} // Alt shift x
    // eine temporaer benutzte Testfunktion: Alt shift T
    // if(i1 == 'T') {PP_up_list(NULL,NULL,-2);goto AllDone;}

  AllDone:

  return 0;

}


//================================================================
  int UI_key_return () {
//================================================================

  int   i1;


    if(UI_Focus == 2) {
      UI_suract_keyIn (1);  // keyIn -> ViewZ

    }  else if(UI_Focus == 3) {
      UI_suract_keyIn (2);  // keyIn -> ViewZ

    } else {
      // if(UI_InpSM == ON) {   // wenn Search ON: activate Modify.
        // // UI_WinSM (NULL, (void*)UI_FuncModify);
        // UI_WinSM (NULL, (void*)UI_FuncUCB5);

      // } else {
        if(UI_InpMode == UI_MODE_CAD) {
          IE_inp_selection_clear ();    // clear selection
          // test if OK is active
          if(IE_cad_get_OK()) {
            IE_cad_OK (NULL, NULL);  // OK-Button druecken
            GUI_obj_focus (&winGR);  // 2014-02-25
          } else {
            // activate next inputfield
            IE_cad_InpIn_CB (0, GUI_KeyReturn);
          }

        } else if(UI_InpMode == UI_MODE_MAN){
          if(KeyStatCtrl == ON) {
            UI_EdKeyCtrlM3 ();   // work all ..
          } else {
            i1 = UI_EdKeyCR (1);     // Return pressed 
            if(i1 < 0) goto Changed; // Standardbehandlung, wenn Cursor im File
          }
        }
      // }
    }


  Changed:
  return 0;

}


//================================================================
  int UI_key_delete () {
//================================================================

  int    i1;
  long   l2;
  char   cbuf[256];


  //----------------------------------------------------------------
  // handle active error
    // check if Error is active
    if(AP_stat.errStat != 0) {
      printf(" ERR=%d lNr=%d\n",AP_stat.errStat,AP_stat.errLn);
      // delete Line <AP_stat.errLn>, continue RUN
      UNDO_grp_add (AP_stat.errLn, 0);     // add line to delelete-stack
      UNDO_grp_del ();                     // delete & continue ..
      // mode VWR: imply "RUN"
      if(UI_InpMode == UI_MODE_VWR) {
        ED_work_END (0);
      } 
      goto AllDone;
    }


  //----------------------------------------------------------------
  // no error; handle active group
    if(Grp_get_nr() > 0) {
      // test if MAN/Editor is active ..
      if(UI_InpMode == UI_MODE_VWR) {
        Del_grp__ ();
        goto AllDone;
      }

      if(UI_InpMode == UI_MODE_CAD) {
        if(IE_get_Func() < 0) {
          Del_grp__ ();
          goto AllDone;
        }
      }
      goto Finish;
    }


    //----------------------------------------------------------------
    if(UI_InpMode == UI_MODE_CAD) {
      // do only default operation 
      // IE_cad_ClearInpAct ();
      // goto AllDone;
      goto Finish;
    }


    //----------------------------------------------------------------
    // Delete/UnDelete in MAN
    if(UI_InpMode != UI_MODE_MAN) goto AllDone;


    // printf("UI_EdKeyPress GDK_Delete\n");
    xa_fl_TxMem = 1;   // src modified merken

    // fix filename for selected text
    sprintf(cbuf,"%sselection.txt",OS_get_tmp_dir());
      // printf(" fnam:|%s|\n",cbuf);
    if(KeyStatCtrl == ON) {
      GUI_edi_InsFile (&winED, cbuf);
    }  else {
      i1 = GUI_edi_sel_wrf (&winED, cbuf);   // write selected text -> file
      if(i1 < 1) goto L_del1;
      GUI_edi_sel_del (&winED);         // delete selected text in Editor
      TX_Print("Text deleted; undelete: Ctrl-delete.");
    }

    // l1 = GUI_Ed_getCpos (&winED);  // get cursorposi
    // l2 = UTF_GetLnrPos (l1);          // get APT-LineNr from cursorposi
    l2 = GUI_edi_getLnr (&winED);
    // printf(" l1=%d l2=%d\n",l1,l2);
    ED_work_CurSet (l2);              // work bis l2

    // Edi -> Memory (nach dem Loeschen)
    ED_unload__ ();

    // goto Changed;
      // printf(" dddddddddddddelete done\n");
    goto AllDone;


    L_del1:
    // goto Changed;  // Standardbehandlung



  //----------------------------------------------------------------
  AllDone:
  Finish:

  return 0;

}


//================================================================
  int UI_unHili () {
//================================================================
// clear group, unhilite all objs

  // clear group
  if(Grp_get_nr() > 0) Grp_Clear (1);

  // remove tempObj
  GL_temp_Delete (DLI_TMP);  // 2015-01-04

  // unhilite all hilited objects
  DL_hili_off (-1L);

  Brw_unselect_all ();

  DL_Redraw ();

  return 0;

}
 

//================================================================
  int UI_key_escape () {
//================================================================
 
  int         i1;


  // printf("UI_key_escape \n");


  // Esc: stop user-callback
  if(UI_UserKeyFunc != NULL) {
    UI_UserKeyFunc (GUI_KeyEsc);
    goto AllDone;

  } else if(UI_UserSelFunc != NULL) {
    UI_UserSelFunc (GUI_KeyEsc, -1L);
    goto AllDone;
  }


/*
    // Esc: clear group 2010-12-31
      if(Grp_get_nr() > 0) {
        Grp_Clear (1);
        goto AllDone;
      }
*/


  // // if Search/Name-Window is active: hide it.
  // if(UI_cb_search (4)) {
    // GUI_ckbutt_set (&ckb_such, FALSE);
    // goto AllDone;
  // }


  // if Measure-toolbar is active: hide it
  if(GUI_ckbutt_get (&ckb_meas)) {
    Meas_exit__ ();
    goto AllDone;
  }
    


  // if GR_Sel_Filter==18 is active: reset
  i1 = UI_GR_Sel_Filter (-1);
  if((i1 == 1)||(i1 == 18)||(i1 == 19)) {
    UI_GR_Sel_Filter (0);
    goto AllDone;
  }


      if(UI_InpMode == UI_MODE_VWR) {
        // VWR: unhilite selected obj's
        UI_unHili ();  // clear group, unhilite all objs


      } else if(UI_InpMode == UI_MODE_CAD) {
        // UI_undo_work (0, 0);  // change last entry
        // test if CAD-funtion ist active;
        if(IE_get_Func() < 0) {
          // no CAD-func active ..
          UI_unHili ();  // clear group, unhilite all objs

        } else {
          // CAD-func is active ..
          IE_cad_Inp_undo ();      // delete last CAD-input or exit func
        }


      } else if(UI_InpMode == UI_MODE_MAN) {
          // printf("  _EdKeyPress Esc MAN\n");
        if(Grp_get_nr() > 0) {
          Grp_Clear (2);     // clear group
          goto AllDone;
        }
        DL_hili_off (-1L);
        // war zuletzt ein "create neue zeile" oder ein "delete Text"
        // i1 = UI_undo_get_ustat();
        // if(i1 == 1) {
          // UI_undo_work (0, 0);  // undo last entry .. changed to Ctrl-Z

        // } else if(i1 == 2) {  // last was "delete Text"; undelete it.
          // sprintf(cbuf,"%stmp%cselection.txt",OS_get_bas_dir(),fnam_del);
          // GUI_edi_InsFile (&winED, cbuf);
        // }
        xa_fl_TxMem = 1;         // src modified merken
        // AP_obj_del1 ();
        // AP_obj_del0 (); // geht no ned
        ED_work_exit (); // exit modify, work from curPos to end of model
        goto AllDone;
      }

    // erst hier; stoppt sonst das Redraw vom AP_obj_del1
    // problem wenn nun das Release schon passiert ist ...
    // KeyStatEscape = ON;
    KeyStatEscape -= 1;
    // printf("UI_EdKeyPress Esc\n");



  AllDone:

  return 0;

}


//================================================================
  int UI_key_sel_fi (int iKey) {
//================================================================
// handle active selectionFilter

// UI_EdKeyPress

  printf("UI_key_sel_fi %d\n",iKey);

// 2012-07-30
  TX_Error ("UI_key_sel_fi");

/*
    // printf(" filter=%d\n",selFi);
  if(selFi != 0) { 
    if(KeyStatCtrl == ON) goto L_no_group;   // skip control-funcs
    if(iKey == GDK_Escape) {
      if(selFi == 16) {    // ModifyModelPos: clear previous Entryfield
        UI_WinTra__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUndo));
        goto AllDone;
      }
      if(selFi == 17) {    // Form active
        PRG_key_CB ("Esc");
        goto AllDone;
      }
      
      xbuf[0] = '\0';
      UI_GR_Sel_Filter (0);      // reset selectionFilter
      goto AllDone;
    
    } else if(iKey == GDK_Tab) {
      if(selFi == 16) {    // ModifyModelPos: jump into next Entryfield
        UI_WinTra__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncNext));
        goto AllDone;
      }
      if(selFi == 17) {    // Form active
        PRG_key_CB ("Tab");
        goto AllDone;
      }
    
    } else if(iKey == GDK_Page_Up) {
      if(selFi == 17) {    // Form active
        PRG_key_CB ("PgUp");
        goto AllDone;
      }

    } else if(iKey == GDK_Page_Down) {
      if(selFi == 17) {    // Form active
        PRG_key_CB ("PgDwn");
        goto AllDone;
      }

    }
    if((selFi == 4)||(selFi == 5)) {       // 4=dump_obj | 5=add_to_group
      // if(iKey == GDK_Tab)  // im CAD fuer GotoNextInputfield erforderlich !
        // goto L_no_group;

      if(iKey == GDK_Tab) {
        if(UI_InpMode == UI_MODE_CAD) { // CAD-add_to_group (INTersect)
          goto Finish;  // use default-handler = acticate next Inp.field
        }

      } else if(iKey == GDK_Return) {
        GUI_Tx_rmLast ();
        APED_dbo_oid (&i1, &l1, xbuf);
        if(selFi == 4) {     // dump obj
          UI_dump_obj (i1, l1);
          UI_GR_Sel_Filter (0);      // reset dump
        } else {         // 5 = add to group
          l2 = DL_find_obj (i1, l1, -1L);
          if(l2 < 0) {
            TX_Print("******  Obj. %s nicht gefunden",xbuf);
          } else {
            // add/remove to group
            DL_grp1__(l2, xbuf, 0, 0);  // DL_grp1_set
          }
        }
        xbuf[0] = '\0';
      } else if((iKey == GDK_BackSpace) ||        // RemoveLeftChar-Taste  <x|
                (iKey == GDK_Delete))       {     // Entf
        i1 = strlen(xbuf);
        // printf(" backSp %d |%s|\n",i1,xbuf);
        if(i1 > 0) {
          xbuf[i1-1] = '\0';
          GUI_Tx_rmLast ();
          TX_Print("...... sel. obj %s",xbuf);
        }
      } else {
        // check for alfanum-char ...
        if((isalnum(iKey)) && (iKey < 125)) {
        // printf(" iKey=%d\n",iKey);
        // add character to buffer xbuf
        i1 = strlen(xbuf);
        // printf(" add key %d %d |%s|\n",iKey,i1,xbuf);
        xbuf[i1] = iKey;
        xbuf[i1+1] = '\0';
        GUI_Tx_rmLast ();
        TX_Print("...... sel. obj %s",xbuf);
        }
      }
      goto AllDone;
    } else {
      xbuf[0] = '\0';
    }
  }
*/
  return 0;

}


//================================================================
  int GR_set_dispTra (int mode) {
//================================================================
// do display-transformations; 0=yes, 1=no
 
  GR_do_dispTra = mode;

  return 0;

}


//===================================================
  int UI_CB_hide  (MemObj *mo, void **data) {
//===================================================
// hide objects
// see UI_CB_view


  printf("UI_CB_hide %d\n",GUI_DATA_EVENT);
  // printf("  UI_stat_hide=%d UI_stat_view=%d\n",
           // UI_stat_hide,UI_stat_view);  // 0=ON; 1=OFF


  //----------------------------------------------------------------
  // disactivate hide
  if(GUI_DATA_EVENT == TYP_EventPress) goto L_activate;
    printf("disactivate hide\n");

  UI_stat_hide = 1;    // 1=inactive


  // reactivate functions ..
  UI_func_stat_set__ (APF_TB_CAD,
                      APF_MEN_FIL,
                      APF_MEN_SM,
                      APF_MEN_MDF,
                      APF_MEN_APP,
                      // APF_HIDE,
                      APF_WIN_B_E,
                      APF_VWCADMAN,
                      APF_MEASEA,
                      APF_BUT_GSE,
                      APF_PLUGINS,
                      APF_UNDO,
                      0);



  // UI_mBars_off ();     // Menuebar reset
  sele_restore ();
  goto L_fertig;



  //----------------------------------------------------------------
  // activate hide
  L_activate:
    printf("activate hide\n");

  // wenn view active: disactivate view.
  if (UI_stat_view == 0) {
     UI_objView__ (0);  // disactivate view
  }

  UI_stat_hide = 0;    // 0=active


  // block some functions ..
  // UI_mBars_off ();     // Menuebar abblenden
  UI_func_stat_set__ (-APF_TB_CAD,
                      -APF_MEN_FIL,
                      -APF_MEN_SM,
                      -APF_MEN_MDF,
                      -APF_MEN_APP,
                      // -APF_HIDE,
                      -APF_WIN_B_E,
                      -APF_VWCADMAN,
                      -APF_MEASEA,
                      -APF_BUT_GSE,
                      -APF_PLUGINS,
                      -APF_UNDO,
                      0);


  // unhilite alle Objekte (last obj zB Kontur sonst ganz rot)
  DL_hili_off (-1L); DL_Redraw ();

  MSG_pri_0 ("HideOn");
  // TX_Print("select objects to hide");

  sele_save ();
  sele_set__ (Typ_goGeom);       // enable selection of all types
  sele_setNoConstrPln ();        // disable selection of point on ConstrPln


  L_fertig:
    // printf(" ex UI_CB_hide UI_stat_hide=%d UI_stat_view=%d\n",
           // UI_stat_hide,UI_stat_view);  // 0=ON; 1=OFF

  return 0;

}


//================================================================
  int UI_GR_ScalAuto (int mode) {
//================================================================
//   0   "butRes"  ScaleAll
//   1   "butAll"  ScaleFix

  int    iFunc, iInp, iCur;
  double d1;


  printf("UI_GR_ScalAuto %d\n",mode);
  
  // query state
  UI_block_get (&iFunc, &iInp, &iCur);

  UI_block__ (1, 1, 1);   // block cursor & input
  // GUI_update__ ();

  // printf("UI_GR_ScalAuto L1 \n");

  if(mode == 0) {
    UI_view__ ("ResView"); // ScaleAll   UI_viewCB()

  } else {
    UI_view__ ("AllView"); // ScaleFix   UI_viewCB()
  }

  // printf("UI_GR_ScalAuto L2 \n");


    // Ausgabefeld Scale setzen (calls AP_Get_scale() - gets AP_scale);)
    UI_AP (UI_FuncSet, UID_ouf_scl, NULL);
    // Ausgabefeld ViewZ setzen
    d1 = GL_query_ViewZ();
    UI_AP (UI_FuncSet, UID_ouf_vwz, (void*)&d1);

    APT_set_view_stat ();

  UI_block__ (iFunc, iInp, iCur);   // reset block


  return 0;

}


//===============================================
  int UI_viewCB (MemObj *mo, void **data) {
//===============================================


  int    iCur;
  char   *cp1, cbuf1[256], txbuf[256];
  double view[10];
  FILE   *fp1;



  // --------------------------------------------------
  // printf("UI_GR_view\n");
  // cp1 = (char*)data;
  cp1 = GUI_DATA_S1;

    // printf("UI_viewCB |%s|\n",cp1);


  iCur = UI_CursorWait (0);    // display watch
    // printf(" iCur=%d\n",iCur);



  


  if(!strcmp(cp1, "shad")) {
    printf("shade\n");
    // UI_GR_DrawInit ();
    // GL_DefineDisp (FUNC_DispRend,i2);
    // UI_GR_DrawExit ();
    UI_wireCB(NULL, NULL);


  } else if(!strcmp(cp1, "wire")) {
    // UI_GR_DrawInit ();
    // GL_DefineDisp (FUNC_DispWire,i2);
    // UI_GR_DrawExit ();
    UI_wireCB(NULL, NULL);


  } else if(!strcmp(cp1, "TopView")) {
    UI_GR_DrawInit ();
    GL_DefineView (FUNC_ViewTop);
    UI_GR_DrawExit ();


  } else if(!strcmp(cp1, "FrontView")) {
    UI_GR_DrawInit ();
    GL_DefineView (FUNC_ViewFront);
    UI_GR_DrawExit ();


  } else if(!strcmp(cp1, "SideView")) {
    UI_GR_DrawInit ();
    GL_DefineView (FUNC_ViewSide);
    UI_GR_DrawExit ();


  } else if(!strcmp(cp1, "IsoView")) {
    UI_GR_DrawInit ();
    GL_DefineView (FUNC_ViewIso);
    UI_GR_DrawExit ();


  } else if(!strcmp(cp1, "ResView")) {     // Scale All
    // UI_GR_DrawInit ();
    // printf(" ResView L1\n");
    DL_ReScale__ ();
    // printf(" ResView L2\n");
    // GL_DefineView (FUNC_ViewReset);
    // UI_GR_DrawExit ();


  } else if(!strcmp(cp1, "AllView")) {
    UI_GR_DrawInit ();
    GL_DefineView (FUNC_ViewReset);
    UI_GR_DrawExit ();
    //GR_Redraw();


  } else if(!strcmp(cp1, "ClrView")) {
    UI_GR_DrawInit ();
    GL_Clear ();
    GL_Redraw();
    UI_GR_DrawExit ();



  } else if(!strcmp(cp1, "PT_OFF")) {          // GUI_menu_checkbox__
    // if(((GtkCheckMenuItem*)parent)->active == 1) {
    // is stored in "MODE DISP_PT OFF"
    if(GUI_DATA_EVENT == TYP_EventPress) {
      APT_dispPT = OFF;  // hide pts
      if(UI_InpMode != UI_MODE_VWR)
        TX_Print("... hide points only in mode VWR ..");
      GL_InitPtAtt (1);
    } else {
      APT_dispPT = ON;  // view pts (std)
      GL_InitPtAtt (0);
    }
    // UI_butCB (NULL, (void*)"butEND");     // Ausfuehren END-Button
    DL_Redraw ();



  } else if(!strcmp(cp1, "PL_OFF")) {          // GUI_menu_checkbox__
    // if(((GtkCheckMenuItem*)parent)->active == 1) {
    // is stored in "MODE DISP_PL OFF"
    if(GUI_DATA_EVENT == TYP_EventPress) {
      APT_dispPL = OFF;         // hide
      if(UI_InpMode != UI_MODE_VWR) {
        TX_Print("... hide planes only in mode VWR ..");
      } else {
        DL_disp_PL (1);
      }
    } else {
      APT_dispPL = ON;          // view (std)
      DL_disp_PL (0);
    }
    // UI_butCB (NULL, (void*)"butEND");     // Ausfuehren END-Button
    DL_Redraw ();



  } else if(!strcmp(cp1, "SOL_OFF")) {          // GUI_menu_checkbox__
    // if(((GtkCheckMenuItem*)parent)->active == 1) {
    if(GUI_DATA_EVENT == TYP_EventPress) {
      APT_dispSOL = OFF;
      printf(" SOL -> OFF\n");
    } else {
      APT_dispSOL = ON;
    }
    UI_but_END ();



  } else if(!strcmp(cp1, "Names")) {


  } else if(!strcmp(cp1, "Text")) {



  }


  /* Reset focus to glarea widget */
  // UI_GR_setKeyFocus ();


    // printf("ex UI_viewCB\n");

  if(iCur == 0) UI_CursorWait (1);    // reset cursor


  return 0;
}


/*
//================================================================
  void* UI_GR_Init (int width, int height) {
//================================================================

  void *glarea;


  // --------------------------------------------------
  printf("UI_GR_Init\n");


  // Check if OpenGL is supported.
  if(UI_GR_DrawQuery() < 0) exit (0);


  // Ein Popup-Window fuer's GL-Window 
  // UI_GR_PopUp1Init (parent);


  // Create new OpenGL widget.
  // glarea = UI_GR_WinInit (parent, "gtkGL", 400, 400, 5);
  glarea = UI_GR_WinInit (width, height, 5);

  // KeyStatShift = OFF;
  // KeyStatCtrl  = OFF;
  // KeyStatAlt   = OFF;


  // GL_InitInfo (); // display GL-Version /in MS-Win noch nicht verfuegbar !
  // was gdk_gl_get_info ..

  // Load Cursors
  // cur_wait = gdk_cursor_new (GDK_WATCH);
  // cur_def  = gdk_cursor_new (GDK_LEFT_PTR);


  return glarea;
}
*/
/*
//=====================================================================
  void UI_GR_Redraw () {
//=====================================================================
// UNUSED
// reInit the grafic-window after a new Toolbar has been added or removed
// wann wird das gerufen ?? von "enter_notify_event" !

  printf("IIIIIIIIII UI_GR_Redraw  IIIIIIIII\n");

    UI_GR_DrawInit ();
    GL_Redraw ();
    UI_GR_DrawExit ();
    GUI_update__ ();


    UI_GR_DrawInit ();
    GL_Resize (0,0);
    // GL_Reshape (0,0);
    // GL_Do_Pan__ (-1, -1);
    UI_GR_DrawExit ();
    GUI_update__ ();


    UI_GR_DrawInit ();
    GL_Redraw ();
    UI_GR_DrawExit ();

    // UI_GR_DrawInit ();
    // // GL_Redraw ();
    // UI_GR_DrawExit ();

     // GUI_gl_block (&winMain, (0);

}


//================================================================
  int UI_GR_block (int mode) {
//================================================================
// inputs blockieren

  int   i1;
  GtkObject *go;

  printf("UI_GR_block %d\n",mode);

  go = GTK_OBJECT(GL_widget);
  i1 = gtk_signal_lookup ("button_press_event", GTK_OBJECT_TYPE(go));
  printf(" button_press_event=%d\n",i1);
  i1 = 437;

  // sleep(1);

  // if(mode == 0) gtk_signal_handler_block (go, i1);
  // else          gtk_signal_handler_unblock (go, i1);

  if(mode == 0) gtk_signal_emit_stop_by_name (go, "button_press_event");

  return 0;

}
*/
/*
//================================================================
  void* UI_GR_WinInit (int width, int height, int border) {
//================================================================
// GraphicWindow
// siehe glXChooseVisual u /usr/include/gtkgl/gdkgl.h
// glXGetConfig glXCreateContext


  int    i1;
  // char   *p1, cbuf1[128], cbuf2[128], cbuf3[128];


  printf("UI_GR_WinInit\n");


  // create GL-widget GL_widget
  UI_GR_DrawCreate ();


  GTK_WIDGET_SET_FLAGS  (GL_widget, GTK_CAN_FOCUS);
  // see UI_GR_setKeyFocus


  // Events for widget must be set before X Window is created
  // vi /usr/include/gdk/gdktypes.h
  gtk_widget_set_events (GTK_WIDGET(GL_widget),
                        // GDK_EXPOSURE_MASK|
                        GDK_VISIBILITY_NOTIFY_MASK|
                        GDK_ENTER_NOTIFY_MASK|
                        GDK_BUTTON_PRESS_MASK|
                        GDK_BUTTON_RELEASE_MASK|
                        GDK_KEY_PRESS_MASK|
                        GDK_KEY_RELEASE_MASK|
                        GDK_SCROLL_MASK|
                        GDK_POINTER_MOTION_MASK|
                        GDK_POINTER_MOTION_HINT_MASK);
                        // GDK_LEAVE_NOTIFY_MASK|

  // GDK_BUTTON1_MOTION_MASK ?
  // GDK_BUTTON2_MOTION_MASK ?
  // KeyPress u. Release muessen hier aktiv sein, damit es das EditWin,
  // das den Focus hat, bekommen kann.



  // spricht nur mit dem kompletten Fenster an (nicht NUR beim GL-Fenster)
                        // GDK_FOCUS_CHANGE|
  // gtk_signal_connect (GTK_OBJECT(GL_widget), "focus_in_event",
                      // GTK_SIGNAL_FUNC(UI_GR_Enter), NULL);

  // Redraw image when exposed.
  // gtk_signal_connect (GTK_OBJECT(GL_widget), "realize",
                     // GTK_SIGNAL_FUNC(UI_GR_GLInit), NULL);

  // Linux only (nicht MS-Win):
  // g_signal_connect_after (GTK_OBJECT(GL_widget), "visibility_notify_event",
  // Linux and MS-Win:
  g_signal_connect_after (GTK_OBJECT(GL_widget), "expose_event",
                      GTK_SIGNAL_FUNC(UI_GR_Draw), NULL);

  // gtk_signal_connect (GTK_OBJECT(GL_widget), "configure_event",
  g_signal_connect_after (GTK_OBJECT(GL_widget), "configure_event",
                      GTK_SIGNAL_FUNC(UI_GR_Reshape), NULL);

  // gtk_signal_connect (GTK_OBJECT(GL_widget), "motion_notify_event",
                      // GTK_SIGNAL_FUNC(UI_GR_MotionNotify), NULL);
  g_signal_connect_after (G_OBJECT(GL_widget), "motion_notify_event",
                    G_CALLBACK(UI_GR_MotionNotify), NULL);

  // enter  u leave
  g_signal_connect (GTK_OBJECT(GL_widget), "enter_notify_event",
                      GTK_SIGNAL_FUNC(UI_GR_Enter), NULL);

  // gtk_signal_connect (GTK_OBJECT(GL_widget), "leave_notify_event",
                      // GTK_SIGNAL_FUNC(UI_GR_Leave), NULL);

  g_signal_connect (GTK_OBJECT(GL_widget), "scroll_event",
                      GTK_SIGNAL_FUNC(UI_GR_Scroll), NULL);


  // die Mausbuttons 
  g_signal_connect (GTK_OBJECT(GL_widget), "button_press_event",
                      GTK_SIGNAL_FUNC(UI_GR_ButtonPress), NULL);

  g_signal_connect (GTK_OBJECT(GL_widget), "button_release_event",
                      GTK_SIGNAL_FUNC(UI_GR_ButtonRelease), NULL);

//
  // // 2009-02-22 raus (Gtk-CRITICAL beim startup)
  // gtk_signal_connect (GTK_OBJECT(GL_widget), "draw",
                      // GTK_SIGNAL_FUNC(UI_GR_Redraw), NULL);
  // printf(" draw\n");
//

//
  // // die Keys; gehen nur, wenn GL-Win Focus hat - derzeit aber gar nicht ..
  // gtk_signal_connect (GTK_OBJECT(GL_widget), "key_press_event",
                      // GTK_SIGNAL_FUNC(UI_GR_KeyPress), NULL);

  // gtk_signal_connect (GTK_OBJECT(GL_widget), "key_release_event",
                      // GTK_SIGNAL_FUNC(UI_GR_KeyRelease), NULL);
//

//
  // gtk_signal_connect (GTK_OBJECT(GL_widget), "destroy",
                      // GTK_SIGNAL_FUNC (UI_GR_Destroy), NULL);




  // set minimum size
  // gtk_widget_set_usize (GTK_WIDGET(GL_widget), width, height);

  // put glarea into window and show it all
  // gtk_container_add (GTK_CONTAINER(frm1),GTK_WIDGET(GL_widget));
  // gtk_widget_show (GTK_WIDGET(GL_widget));
  // gtk_widget_show (frm1);

  // set focus to glarea widget (nur f. KeyEvents erforderl)
  // UI_GR_setKeyFocus ();


  printf("ex UI_GR_WinInit\n");

  return GL_widget;

}
*/

/*
//================================================================
  gint UI_GR_GLInit (void *widget) {
//================================================================
// OpenGL functions can be called only if begingl returns true


  int    i1;
  // char buf1[32];

  printf("UI_GR_GLInit\n");

  // reconfig GL-win
  UI_GR_DrawConfig (widget);     // unused


  // if (gtk_gl_area_begingl (GTK_GL_AREA(widget))) {
  // if (gtk_gl_area_make_current (GTK_GL_AREA(widget))) {
  // UI_GR_DrawInit ();  // 2011-04-07 raus; makes Crash if model is empty !

  GL_Init__ (0, ((GtkWidget*)widget)->allocation.width,
                ((GtkWidget*)widget)->allocation.height);



  // Init Display of Scalefaktor: geht nicht vor show mainWin !
  // sprintf(buf1, "%+10.3f",GL_Scale);
  // printf(" set Scale /%s/\n",buf1);
  // gtk_label_set_text ((GtkLabel*) (UI_Scale), buf1);



  return TRUE;
}
*/

//================================================================
  int UI_GR_expose () {
//================================================================
 
  printf("UI_GR_expose \n");

  // DL_Redraw ();

  // UI_GR_Enter (GL_widget, NULL);

  // UI_GR_MotionNotify

  // UI_GR_DrawInit ();
  // GL_Set_Scale (GL_get_Scale());
  // UI_GR_DrawExit ();


/* 2012-01-24
  UI_GR_RECONFIG (NULL, NULL);   // UI_GR_idle_CB
*/

  return 0;

}

/*
//===============================================================
  gint UI_GR_Enter (void *widget, void *event) {
//===============================================================
// enter_notify_event

  int             x, y, dx, dy;
  GdkModifierType state;
  // GdkEventAny     ev1;
  GdkEventExpose  ev1;
  gint            ig1;



  // printf("UI_GR_Enter \n");
  // printf("  UI_InpMode=%d AP_stat.sysStat=%d\n",UI_InpMode,AP_stat.sysStat);


  if(AP_stat.sysStat < 2) return 0;

  // if (event->count > 0) return TRUE;


  g_signal_handlers_block_by_func (GTK_OBJECT(GL_widget),
                                     GTK_SIGNAL_FUNC(UI_GR_Enter), NULL);


  // set mouseposition, to prevent a jump in relative-movement-coords)
  // get actual cursor-position
  // gdk_window_get_pointer (event->window, &x, &y, &state);
  gdk_window_get_pointer (((GtkWidget*)GL_widget)->window, &x, &y, &state);
  // gdk_window_get_pointer (NULL, &x, &y, &state);

  // printf("UI_GR_Enter %d %d %d\n",x, y, state);

  GL_Do_Idle (&dx, &dy, x, y);

  // if (state & GDK_BUTTON2_MASK) {
    // KeyM2 = ON;
    // UI_GR_view_set_func ();
  // }
  // if (state & GDK_BUTTON3_MASK) {
    // KeyM3 = ON;
    // UI_GR_view_set_func ();
  // }

  // GUI_update__ ();

  // DL_Redraw ();  // 2011-01-24
  // g_signal_emit_by_name (GTK_OBJECT(GL_widget),"visibility-notify-event",
                         // &ev1, &ig1);
                         // event, &ig1);
  UI_GR_idle_GL3_CB (NULL);
  // UI_GR_Draw


  // GUI_update__ ();
  g_signal_handlers_unblock_by_func (GTK_OBJECT(GL_widget),
                                     GTK_SIGNAL_FUNC(UI_GR_Enter), NULL);


  return TRUE;

}
*/

/*
//===============================================================
  gint UI_GR_Leave (void *widget, GdkEventCrossing *event) {
//===============================================================

  // printf("UI_GR_Leave\n");

  GR_Func_Act = 0;
  UI_ChangeCursor (GDK_LEFT_PTR); // default

  return TRUE;

}
*/

/*
//================================================================
  gint UI_GR_Draw (void *widget, void *event) {
//================================================================
// When widget is exposed it's contents are redrawn.

// static int iStart = 0;

  // printf("UI_GR_Draw %d %d\n",event->count,UI_Focus);
  // printf("  UI_InpMode=%d AP_stat.sysStat=%d\n",UI_InpMode,AP_stat.sysStat);


  // Draw only last expose.
  // if (event->count > 0) return TRUE;
    // if(GR_Func_Act >= 0) return TRUE;
  // }


  // gtk_signal_emit_by_name(GTK_OBJECT(widget),"enter_notify_event",&ev1,&ig1);
  if(AP_stat.sysStat < 2) return 0;


  g_signal_handlers_block_by_func (GTK_OBJECT(GL_widget),
                                     GTK_SIGNAL_FUNC(UI_GR_Draw), NULL);

  DL_Redraw ();

  // if(UI_Focus == 1)
    // UI_AP (UI_FuncFocus, UID_WinEdit, NULL);  // GUI_Ed_Focus
  // else
    // geht nicht; sonst kein Focus auf die CAD-Entryfelder moeglich
    // UI_GR_setKeyFocus();   // set Focus to OpenGL-window
    
  // GUI_update__ ();
  g_signal_handlers_unblock_by_func (GTK_OBJECT(GL_widget),
                                     GTK_SIGNAL_FUNC(UI_GR_Draw), NULL);

  return TRUE;    // stop
}
*/
/*
//================================================================
  int UI_GR_idle_GL1_CB (void *data) {
//================================================================
// configure_event
 
  GdkEventAny  ev1;
  gint         ig1;


  // printf("UI_GR_idle_GL1_CB \n");

  // GUI_update__ ();

  // g_signal_emit_by_name (GTK_OBJECT(UI_MainWin),"visibility_notify_event",&ev1,&ig1);



  g_signal_emit_by_name (GTK_OBJECT(GL_widget),"configure_event",&ev1,&ig1);
  // UI_GR_Reshape

  // g_signal_emit_by_name (GTK_OBJECT(GL_widget),"expose_event",&ev1,&ig1);


  // // enter_notify_event
  // GUI_Idle (UI_GR_idle_GL2_CB, NULL);   // 2012-01-24

  GUI_Idle (UI_GR_idle_GL3_CB, NULL);   // 2012-01-24


  return 0;  // removes idle-Call!

}
*/
/*
//================================================================
  int UI_GR_idle_GL2_CB (void *data) {
//================================================================
// enter_notify_event

  GdkEventAny  ev1;
  gint         ig1;


  // printf("UI_GR_idle_GL2_CB \n");

  // GUI_update__ ();

  // g_signal_emit_by_name (GTK_OBJECT(GL_widget),"expose_event",&ev1,&ig1);
  g_signal_emit_by_name (GTK_OBJECT(GL_widget),"enter_notify_event",&ev1,&ig1);
  // UI_GR_Reshape


  // GUI_Idle (UI_GR_idle_GL3_CB, NULL);
  // g_signal_emit_by_name (GTK_OBJECT(GL_widget),"expose_event",&ev1,&ig1);


  return 0;  // removes idle-Call!

}
*/
/*
//================================================================
  int UI_GR_idle_GL3_CB (void *data) {
//================================================================
// expose_event

  GdkEventAny  ev1;
  gint         ig1;


  // printf("UI_GR_idle_GL3_CB \n");

  // GUI_update__ ();

  // g_signal_emit_by_name (GTK_OBJECT(GL_widget),"draw",&ev1,&ig1);
  g_signal_emit_by_name (GTK_OBJECT(GL_widget),"expose_event",&ev1,&ig1);
  // UI_GR_Reshape


  return 0;  // removes idle-Call!

}
*/
/*
//================================================================
  int UI_GR_RECONFIG (void *data1, void *data2) {
//================================================================
// UNUSED

  GdkEventAny  ev1;
  gint         ig1;


  // printf("UI_GR_RECONFIG \n");
// return 0;   //TEST 2012-01-24

  // configure_event
  GUI_Idle (UI_GR_idle_GL1_CB, NULL);

  // // expose_event
  // GUI_Idle (UI_GR_idle_GL2_CB, NULL);   // 2012-01-24

  // GUI_Idle (UI_GR_idle_GL3_CB, NULL);   // 2012-01-24

  return 0;

}
*/
/*
//================================================================
  gint UI_GR_Reshape (void *widget, void *event) {
//================================================================
// When glarea widget size changes, viewport size is set to match the new size
// from configure_event

// Reihenfolge muss sein: UI_GR_Reshape UI_GR_Enter UI_GR_Draw


  int          i1;
  GdkEventAny  ev1;
  gint         ig1;


  // printf("UI_GR_Reshape UI_InpMode=%d AP_stat.sysStat=%d\n",
         // UI_InpMode,AP_stat.sysStat);




  if(AP_stat.sysStat < 2) {

    i1 = ERR_SET1 ();
    if(i1) {
      printf("Fehler beim Aktivieren von OpenGL\n");
      // GUI_MsgBox ("Fehler beim Aktivieren von OpenGL\n");
      GUI_MsgBox (" Error activating OpenGL ");
      gtk_exit(1);
    }

    // UI_GR_GLInit (widget);
    //if(UI_GR_STAT != 1) {
    UI_GR_DrawInit ();
    UI_GR_GLInit (GL_widget);
    UI_GR_DrawExit ();             // 2010-08-24

    // GUI_update__ ();
    ERR_RESET ();

    AP_stat.sysStat = 2;

    return 0; // mit diesem event kein enter_notify_event möglich !

  }


  // if (event->count > 0) return TRUE;

  g_signal_handlers_block_by_func (GTK_OBJECT(GL_widget),
                                     GTK_SIGNAL_FUNC(UI_GR_Reshape), NULL);


  // GL_Reshape (widget->allocation.width,widget->allocation.height);
    // printf(" glwinSiz = %d %d\n",widget->allocation.width,
                                 // widget->allocation.height);

  UI_GR_DrawInit ();
  GL_Resize (((GtkWidget*)widget)->allocation.width,
             ((GtkWidget*)widget)->allocation.height);
  UI_GR_DrawExit ();


  // // exit if startup not complete; sonst crash m ubuntu/suse.
    // if(UI_InpMode == UI_MODE_START) return FALSE;


  DL_Redraw ();  // ONLY FOR Win7/32 necessary ! 2011-05-22

  // DL_Redraw hilft hier nix; muss iregndwann spaeter erfolgen !!!
  // Danach UI_GR_Draw; auch noch zu frueh.
  // GR_set_constPlnPos ();
  // verdreht immer !!!


  if(AP_stat.sysStat > 1) {   // 2011-01-28
    GR_Func_Act = -1;
    // g_signal_emit_by_name (GTK_OBJECT(GL_widget),  "enter-notify-event",
                           // event, &ig1);
    // GUI_update__ ();
    GUI_Idle (UI_GR_idle_GL2_CB, NULL);
  }



  g_signal_handlers_unblock_by_func (GTK_OBJECT(GL_widget),
                                     GTK_SIGNAL_FUNC(UI_GR_Reshape), NULL);


  return TRUE;    // stop
  // return FALSE;      // continue
}
*/
/* UNUSED
//================================================================
  // gint UI_GR_MotionNotify (void *widget, GdkEventMotion *event) {
  gint UI_GR_MotionNotify (void *widget, void *event) {
//================================================================
/// \code
/// callBack mouse-movement
/// compute GR_CurUk = cursorPosition in userCoords on constructionPlane
/// \endcode


  // static char buf1[128];

  // int    doDisp;
  int    actPosX, actPosY, stat3D, dx, dy;
  char   buf1[128];
  Point  pt1;

  GdkModifierType state;

  // int    i1;
  // GtkObject *go;


  // printf("UI_GR_MotionNotify GR_Func_Act=%d\n",GR_Func_Act);
  // printf(" move-KeyM1 %d\n",KeyM1);
  // printf("  UI_InpMode=%d AP_stat.sysStat=%d\n",UI_InpMode,AP_stat.sysStat);
  // printf("  x=%f y=%f x_root=%f y_root=%f\n",
          // event->x,event->y,event->x_root, event->y_root);
  // if(UI_UserMousemoveFunc) printf("  UI_UserMousemoveFunc is active ..\n");


  // block signal
  // GUI_gl_block (&winMain, 1);   // macht MS-Win-Crash beim Startup !
  g_signal_handlers_block_by_func (
      G_OBJECT (GL_widget),
      G_CALLBACK (UI_GR_MotionNotify), NULL);



/
  // // Mauskoord. holen; links oben ist der Nullpunkt
  // if (event->is_hint) {
    // gdk_window_get_pointer (event->window, &actPosX, &actPosY, &state);
  // } else {
    // actPosX = event->x;
    // actPosY = event->y;
  // }
/
  gtk_widget_get_pointer (GL_widget, &actPosX, &actPosY);
    // printf(" actPosX=%d, actPosY=%d\n",actPosX,actPosY);



  KeyStatShift = OFF;
  KeyStatCtrl  = OFF;
  state = ((GdkEventMotion*)event)->state;
  if(state & GDK_SHIFT_MASK)   KeyStatShift = ON;
  if(state & GDK_CONTROL_MASK) KeyStatCtrl  = ON;



  // test if mousebutton-1 is down
  if(KeyM1 == ON) {
      // printf(" move-M1: test trap ..\n");
    // set GL_mouse_x_act, GL_mouse_y_act
    GL_Do_Idle (&dx, &dy, actPosX, actPosY);
    // do Rubberbox
    GL_RubberBox_do ();
    // goto L_fertig;
    goto L_exit;

  }



  // Fehler GTK1: CtrlOff kommt im PopupWin nicht !!!

  if(GR_do_dispTra == 1) goto L_disp_no;  // do not modify display; report only ..

  if((KeyStatShift == ON)||(KeyStatCtrl == ON)) goto L_disp_yes;


  //================================================================
  // do NO display-transformations (pan/rotate/zoom) -
  // report movement to userFunctions.
  L_disp_no:

    if(GR_Func_Act != 0) {
      UI_ResetCursor ();
      GR_Func_Act = 0;
    }


    // set GL_mouse_x_act, GL_mouse_y_act, return relative mousemovement
    // in screencoords
    GL_Do_Idle (&dx, &dy, actPosX, actPosY);


    //  call userfunction
    if(UI_UserMousemoveFunc != NULL) {
      UI_UserMousemoveFunc (dx, dy);
    }

    goto L_fertig;




  //================================================================
  // do display-transformations (pan/rotate/zoom)
  L_disp_yes:
  // printf("     _MotionNotify %d %d\n",actPosX,actPosY);
  // printf("GDK_SHIFT_MASK = %d %d\n",state & GDK_SHIFT_MASK, KeyStatShift);
  // printf("GDK_CONTROL_MASK = %d\n",state & GDK_CONTROL_MASK);


  stat3D = GUI_ckbutt_get (&ckb_3D);  // Checkbox 3D

/
  // gtk_main_quit ();  // stop collecting events ...
  // // g_main_loop_quit (NULL);
/


  // if user requests Mousemovements: do not perform zoom/pan
  // doDisp = 0;


  //------------------------------------------------------------
  // scale
  // if(((KeyStatShift == ON)&&(KeyStatCtrl == ON)) ||
     // ((KeyStatCtrl == ON)&&(stat3D == 0)))          {
  if(GR_Func_Act == GR_Func_Scl) {

    UI_GR_DrawInit ();
    GL_Do_Scale__ (actPosX, actPosY);
    UI_GR_DrawExit ();

    // Ausgabefeld Scale setzen
    UI_AP (UI_FuncSet, UID_ouf_scl, NULL);




  //------------------------------------------------------------
  // move
  // if(KeyM3 == ON) {
  // } else if(KeyStatShift == ON) {
  } else if(GR_Func_Act == GR_Func_Pan) {

    UI_GR_DrawInit ();
    GL_Do_Pan__ (actPosX, actPosY);
    UI_GR_DrawExit ();

    // printf(" n. pan GR_Func_Act=%d\n",GR_Func_Act);



  //------------------------------------------------------------
  // rotate
  // } else if(KeyStatCtrl == ON) {
  } else if(GR_Func_Act == GR_Func_Rot) {

    // Checkbox 3D == ON: skip rotate.
    if(stat3D == 0) goto L_weiter;
    
    UI_GR_DrawInit ();
    GL_Do_Rot__ (actPosX, actPosY);
    UI_GR_DrawExit ();



  }

  L_weiter:


//#ifdef _MSC_VER
//  GUI_del_events ();  // kill pending events ...
//#endif
/
  // // g_main_loop_run (NULL);
  // gtk_main ();        // resume colecting events ...
/



  //  die Cursorpos auf der ConstrPlane in uk's errechnen und anzeigen
  L_fertig:
  GR_set_constPlnPos ();  // compute GR_CurUk in worldCoords




  // die PosKoord. GR_CurUk in ein Ausgabefenster geschr. werden.
  // sprintf(buf1, "%+10.3f  %+10.3f %+8.1f",GR_CurUk.x,GR_CurUk.y,GR_CurUk.z);
/
  // if((WC_sur_ind != 0)&&(UI_RelAbs == 0)) {
    // UT3D_pt_traptm3 (&pt1, WC_sur_imat, &GR_CurUk);
  // } else {
    // pt1 = GR_CurUk;
  // }
/

  UI_GR_get_actPosA (&pt1);   // get GR_CurUk


  sprintf(buf1, "%+10.3f %+10.3f %+10.3f",pt1.x,pt1.y,pt1.z);
  // printf("%s\n",buf1);
  // gtk_label_set_text ((GtkLabel*) (UI_curPos), buf1);
  GUI_label_mod (&UI_curPos, buf1);




  // Fertig:
  L_exit:
  // unblock signal
  // GUI_gl_block (&winMain, 0);
  // GUI_update__ ();
  g_signal_handlers_unblock_by_func (
    G_OBJECT (GL_widget),
    G_CALLBACK (UI_GR_MotionNotify), NULL);


  // go = GTK_OBJECT(GL_widget);
  // i1 = gtk_signal_lookup ("motion_notify_event", GTK_OBJECT_TYPE(go));
  // printf("ex UI_GR_MotionNotify=%d\n",i1);
    // return TRUE; // prevent the default handler from being run


  return TRUE; // prevent the default handler from being run
  // return FALSE;
}
*/

//===============================================================
  int UI_GR_CB_Sel2 (MemObj *mo, void **data) {
//===============================================================
// CB of MouseOverPopup-Eintrag (noch NICHT selektiert)
// return FALSE;   bei TRUE kein hilite des entry under cursor!
// preview
// unmap = -1
//  7 = GDK_BUTTON_RELEASE
// 10 = GDK_ENTER_NOTIFY
// 11 = GDK_LEAVE_NOTIFY
// 15 = GDK_UNMAP

  int  iEv;
  long isel = -1;


  iEv = GUI_DATA_EVENT;
  isel = GUI_DATA_I1;

  // printf("UI_GR_CB_Sel2 ev=%d isel=%d\n",iEv,isel);


  // printf("UI_GR_CB_Sel2 type=%d %d\n",event->type,INT_PTR(data));
  // printf("UI_GR_CB_Sel2 mode=%d stat=%d data=%d\n",
            // event->mode,event->state,(int)data);


  // Unix-GTK2:
  //   GDK_BUTTON_RELEASE  GDK_ENTER_NOTIFY GDK_UNMAP bei select 
  //   GDK_BUTTON_RELEASE  GDK_UNMAP                  bei cancel
  //   Bei MS-Win kommt kein UNMAP !!!
  // if(event->type == GDK_BUTTON_RELEASE) {
  if(iEv == TYP_EventPress) {
    // report release M1 = selection
    UI_GR_Select1 (100, &isel);


  // } else if (event->type == GDK_ENTER_NOTIFY) {
  } else if(iEv == TYP_EventEnter) {
    // preview
    UI_GR_Select1 (101, &isel);


  // } else if (event->type == GDK_LEAVE_NOTIFY) {
  } else if(iEv == TYP_EventLeave) {
    // end preview
    UI_GR_Select1 (102, &isel);

/*
  // } else if (event->type == GDK_UNMAP) {
  } else if(iEv == TYP_EventUnmap) {
    // cancel selection / end preview
    // UI_GR_Select1 (102, &isel);
*/
  } 


  return FALSE;  // GTK2 ! was TRUE

}


//====================================================================
  int UI_GR_Select1 (int mode, long *dlInd) {
//====================================================================
// Vom GL die Objekte holen; wenn mehrere:
// mode=-1:    change ViewPlane; keine Liste.
// mode=0:     create list of objects under cursor
// mode=0-99:  Liste anbieten, die Auswahl reournieren.
// mode=100:   Select Popup-ListObj
// mode=101:   ENTER Popup-ListObj; preview object
// mode=102:   LEAVE Popup-ListObj;
// RetCod:
//   0  mehrere objects gefunden
//   1  one obj found; unknown obj: dlInd = -1; typ = Typ_Vertex.
//  -1  no object selected; TmpPT on ConstPln 

// UI_GR_ButtonPress
//   UI_GR_Select1



#define SELTABSIZ 100
#define SELTABLEN 60
#define oaSIZ 10

static  ObjDB  *dlTab;                 // = GR_selTab
static  ObjDB  selTab[SELTABSIZ];      // used for 2D-icons only ?
static  int    selNr;
static  char   namTab[SELTABSIZ + 1][SELTABLEN];
static  Point  selPos;

  int     irc, iNr, i1, i2, typ, bTyp, reqTyp, *ip1;
  long    l1, ind, dbi, dli;
  char    *p1, *namPtr[SELTABSIZ + 1];
  char    nam[60];
  ObjDB   parTab[oaSIZ];
  subCurv sca[3];



  // printf("SSSSSSSSSSSSSSS UI_GR_Select1 mode=%d dli=%ld\n",mode,*dlInd);



  if(mode >= 100) goto L_mode100;


  //============== Mode 0-99 ================================0
  // mode = 0: create list of objects under cursor.  

  // get mouseposition in userCoords
  sele_get_pos (&selPos);
    // UT3D_stru_dump (Typ_PT, &selPos, " selPos");
  reqTyp = sele_get_reqTyp ();
    // printf(" reqTyp=%d\n",reqTyp);


  // get nr and type of selected objects (not including images, tags)
  iNr = GL_sel_sel (&dlTab);
    // printf(" _sel_sel %d\n",iNr);


    // DUMP SELECTION_BUFFER dlTab
    // UI_GR_dump_dlTab (dlTab, iNr);


  // if a 2D-icon was selected: done ..
  if(iNr > 0) {
    i1 = dlTab[0].typ;
      // printf(" ip1=%d\n",i1);
    // if((i1 == Typ_modREV)  || (i1 >= Typ_FncVAR1))     {
    if((TYP_IS_MOD(i1)) || (TYP_IS_SELGRP(i1)))     {
      selNr = 1;
      selTab[0].typ   = i1;
      selTab[0].dbInd = 0L;
      selTab[0].dlInd = 0L;
      APED_oid_dbo_all (namTab[0], i1, 0L, 0L);  // get text from typ and dbi
      goto L_done;
    }
  }



  // add selection of TextTags
  // iNr = DL_txtSelect (iNr, typTab, indTab, dlTab);
  iNr = DL_txtSelect (iNr, &dlTab);
    // printf("nach DL_txtSelect %d\n",iNr);

    // DUMP SELECTION_BUFFER dlTab==GR_selTab
    // UI_GR_dump_dlTab (dlTab, iNr);


  // interaction ON: remove all objects without activity.
  if(INTACT_IS_ON) {
    // loop tru all obj's in list
    L_ck_act:
    for(i1=0; i1<iNr; ++i1) {
      // check for interctivity on obj 
      irc = DB_QueryActiv (&p1, dlTab[i1].typ, dlTab[i1].dbInd);
      if(irc >= 0) continue;
      // delete obj from list (no activity)
      iNr = GL_sel_del_rec (&dlTab, i1);
      goto L_ck_act;
    }
  }


    // DUMP SELECTION_BUFFER dlTab
    // UI_GR_dump_dlTab (dlTab, iNr);



  if(iNr < 1) {            // INDICATE 
    // dlTab[0].dlInd = -1;  // no selection - indicate !!!
    iNr = GL_sel_add_DB (&dlTab, -1L, Typ_TmpPT); // add Typ_TmpPT to GR_selTab
    
/*
    // only VWR:
    if(UI_InpMode == UI_MODE_MAN) {
      l1 = -2L;
      UI_GR_DrawInit ();
      GL_DrawSymB (&l1, Typ_Att_hili1, SYM_CIR_S, &selPos);  // rot
        // printf("GL_DrawSymB l1=%ld\n",l1);
      UI_GR_DrawExit ();
      DL_Redraw();
    }
*/

  } else {
    GL_sel_sort ();          // sort
  }

    // disp sorted GR_selTab
    // GL_sel_dump ();



  if(mode < 0) return iNr;  // RotationCenter: exit 



  // test for Vertex-request
  if(GR_Sel_Filter == 3) {
    if(iNr < 1) return -1;
      printf(" give vertex ..\n");
    // GR_Sel_Filter = 0;  // 2013-04-08
    selNr = 1;
    dli = -1L;
    typ = Typ_PT;
    ind = 0;
    nam[0] = '\0';
    AP_obj_add_pt (nam, &selPos);
      // printf(" vertPos |%s|\n",(char*)nam);
    // DB_StorePoint(0L, &selPos);
    goto L_done;
  }


  if(reqTyp == Typ_Vertex) {
    if(iNr < 1) return -1;
    selNr = 1;
    ind = dlTab[0].dbInd;
    typ = dlTab[0].typ;
    APED_oid_dbo__ (nam, typ, ind);
    goto L_done;
  }


/*
  // MAN & CAD: add record for interactivity
  // if(UI_InpMode != UI_MODE_VWR) {
    // get i1 = status of Checkbox "Interaktion"; 1=yes, 0=no
    UI_AP (UI_FuncGet, UID_ckb_Iact, (void*)&i1);  // 0=yes
      // printf(" UID_ckb_Iact = %d\n",i1);

    // not necessary anymore: interactivity has now DL-records
    //   (text, eg "I21") 2011-07-01

    // In Funktion DumpObject: do not execute Interactivities
    if(GR_Sel_Filter == 4) i1 = 0;

    // add Interactivity-Record to typTab, indTab; no dlTab-Record !
    if(i1 != 0) {    // nur wenn Iact == OFF
      iNr = DL_IactSelect (iNr, &dlTab);  // add interactivities into selObjsTab
      // printf(" _IactSelect %d %d\n",iNr,dlTab[0]);
    }
  // }
*/

    // DUMP SELECTION_BUFFER dlTab
    // UI_GR_dump_dlTab (dlTab, iNr);


  // Popup-Liste aufbauen. Loop tru dlTab[], size iNr; create selTab, namTab.
  selNr = 0;
  for(i1=0; i1<iNr; ++i1) {
      // printf(" sel-dlTab[%d] typ=%d dbi=%ld dli=%ld\n",i1,
              // dlTab[i1].typ,dlTab[i1].dbInd,dlTab[i1].dlInd);

    typ = dlTab[i1].typ;
    dbi = dlTab[i1].dbInd;
    dli = dlTab[i1].dlInd;


    // test if objTyp is active in reqObjTab; 0; no; elso yes
    // if(sele_ck_typ (dlTab[i1].typ) == 0)  continue;
    if(sele_ck_typ (typ)) {

      // test ConstrPlane. Temp.points not useful, if ConstrPln not active.
      if(typ == Typ_TmpPT) {                // 2012-01-17
        if(sele_ck_ConstrPln() == 0) continue; 
      }

      // make name from typ and DB-index
      APED_oid_dbo_all (namTab[selNr], typ, dbi, dli);

      // copy typ,dbi,dli.
      selTab[selNr].typ   = typ;
      selTab[selNr].dbInd = dbi;
      selTab[selNr].dlInd = dli;
  
      // if(irc < 0) strcpy(namTab[i1], "-");
        // printf(" selTab-add-[%d] %d %ld %ld |%s|\n",selNr, dlTab[selNr].typ,
               // dlTab[selNr].dbInd, dlTab[selNr].dlInd, namTab[selNr]);
      ++selNr;
      if(selNr >= SELTABSIZ - 2) break;


      // test for parents
      irc = oaSIZ;
      // find all parents for obj (typ, dbi, dli)
      DL_parent_ck__ (&irc, parTab, typ, dbi, dli);
      if(irc > 0) {
        for(i2=0; i2<irc; ++i2) {
          if(selNr >= SELTABSIZ - 2) break;
          // add parent
          selTab[selNr].typ   = parTab[i2].typ;
          selTab[selNr].dbInd = parTab[i2].dbInd;
          selTab[selNr].dlInd = parTab[i2].dlInd;
          APED_oid_dbo__ (namTab[selNr], parTab[i2].typ, parTab[i2].dbInd);
            // printf(" add par [%d] %d %ld %ld |%s|\n",selNr, selTab[selNr].typ,
                   // selTab[selNr].dbInd, selTab[selNr].dlInd, namTab[selNr]);
          ++selNr;
        }
      }
    }


    // if "View" or Hide" is active: do NOT resolve subCurves
      // printf(" hide/view=%d\n",UI_CK_HIDE_VIEW);
    if(UI_CK_HIDE_VIEW) continue;
    

    // skip unresolvable objects
    if((reqTyp == Typ_Angle)   ||
       (reqTyp == Typ_Tra))         goto L_selTab_from_dlTab_nxt;


    bTyp = AP_typ_2_bastyp (typ);
      // printf(" bTyp=%d\n",bTyp);


    // test surfaces, subModels; if Typ_PT requested provide vertex ..
    if((bTyp == Typ_Model)   ||
       (bTyp == Typ_SUR)     ||
       (bTyp == Typ_SOL))       {
      // add the vertex (selPos) as point
        // printf(" test-vertex-1 Typ_PT %d\n",sele_ck_typ (Typ_PT));
// TODO: test if analytical point is possible
      if(sele_ck_typ(Typ_PT)) {    // Typ_TmpPT
        // Typ_TmpPT is requested
        selTab[selNr].typ   = Typ_Vertex;
        selTab[selNr].dbInd = 0L;
        selTab[selNr].dlInd = -1L;
        strcpy(namTab[selNr], "selPos");
        ++selNr;
      }
      goto L_selTab_from_dlTab_nxt;
    }



    // test for component of curve; eg provide also line for polygon
      // printf(" ck-subCurv typ=%d dbi=%ld reqTyp=%d\n",typ,dbi,reqTyp);
      // UT3D_stru_dump (Typ_PT, &selPos, "ex sele_get_pos");
    if((bTyp == Typ_VC)      ||
       (bTyp == Typ_PT)      ||
       (bTyp == Typ_PLN)     ||               // 2015-07-06
       (bTyp == Typ_Note)    ||
       (bTyp == Typ_APPOBJ)  ||               // 2015-06-21
       (bTyp == Typ_TmpPT))       goto L_selTab_from_dlTab_nxt; 
    // resolv obj
    sele_ck_subCurv (sca, typ, dbi, &selPos);
    for(i2=0; i2<3; ++i2) {     //loop tru output (max 3 objects)
       // printf(" sca[%d] typ=%d oid=|%s|\n",i1,sca[i2].typ,sca[i2].oid);
      if(sca[i2].typ == Typ_Error) continue;
      if(selNr >= SELTABSIZ - 2) break;
      // decode src & create temp obj
      DBO_dbo_src__ (&typ, &dbi, sca[i2].oid);
      // add subcurv
      selTab[selNr].typ   = typ;
      selTab[selNr].dbInd = dbi;
      selTab[selNr].dlInd = -1L;
      strcpy(namTab[selNr], sca[i2].oid);
      ++selNr;
    }

    L_selTab_from_dlTab_nxt:
    continue;
  }


  // change nr of sel objs
  GL_sel_nr_set (selNr);
    // printf(" selNr=%d\n",selNr);

    // dump selTab
    // UI_GR_dump_selTab (selTab, namTab, selNr);


  // test if "ConstrPlane" should be added
  // not in mode VWR without userFunction
  if((UI_InpMode == UI_MODE_VWR)&&(UI_UserSelFunc == NULL)) goto L_12;

  // not with mouse-right-button
  if(GR_Event_Act != GUI_MouseL) goto L_12;


    // test if ConstrPlane already in buffer; yes: skip adding it
    for(i1=0; i1<selNr; ++i1) if(dlTab[i1].typ == Typ_TmpPT) goto L_12;
    irc = sele_ck_ConstrPln ();  
      // printf(" %d ex sele_ck_ConstrPln\n",irc);
    if(irc) {   // 0=skip, 1=yes,add.
        // printf(" add ConstrPlane 2 as %d\n",selNr);
      strcpy(namTab[selNr], "ConstrPlane");
      // typTab[selNr] = Typ_TmpPT;
      // dlTab[selNr].typ = Typ_TmpPT;
      // ++selNr;
      // selNr = GL_sel_add_DB (&dlTab, -1L, Typ_TmpPT);
      selTab[selNr].typ   = Typ_TmpPT;
      selTab[selNr].dbInd = -1L;
      selTab[selNr].dlInd = -1L;
      ++selNr;
    }


  L_12:

    // dump selTab
    // UI_GR_dump_selTab (selTab, namTab, selNr);




  // kein object gefunden
  if(selNr < 1 ) {
    // APED_oid_dbo__ (namTab[0], dlTab[0].typ, dlTab[0].dbInd);
      printf(" NO USEFUL OBJ !\n");
    *dlInd = -1;
    DL_Redraw ();
    return -1;
  }


  // genau ein object gefunden
  L_done:
  if(selNr < 2 ) {
      // printf(" 1 obj: %d %ld %ld |%s|\n",selTab[0].typ, selTab[0].dbInd,
                   // selTab[0].dlInd, namTab[0]);
    // UI_GR_set_sel__ (dlTab[0]);  // set GR_selNam, GR_selTyp, GR_selDbi
    // UI_GR_set_selNam (typTab[0],indTab[0],namTab[0]);
    UI_GR_set_selNam (selTab[0].typ, selTab[0].dbInd, namTab[0]);
    // *dlInd = dlTab[0];
    GR_selDli = selTab[0].dlInd;
    *dlInd = GR_selDli;

    // hide or view: done ..   // 2012-10-29
    if(UI_CK_HIDE_VIEW) return 1;


    // only for M3-selections: hilite (single) obj
    // but skip 2D-butts
    if(TYP_IS_MOD(selTab[0].typ))  DL_Redraw (); // finish select-process
    else UI_GR_Select3 (selTab[0].dlInd);

    return 1;          // will call later UI_GR_Select2 (dli);
    // mode = 100;
    // goto L_mode100;
  }


  // more than 1 object found; create pointerList
  // in Liste anbieten; select = Sel1  preview = Sel2
  for(i1=0; i1<selNr; ++i1)  namPtr[i1] = namTab[i1];
  namPtr[selNr] = NULL;
  // GUI_Popup (namPtr, UI_GR_CB_Sel1, UI_GR_CB_Sel2);
  // GUI_Popup (namPtr, NULL, UI_GR_CB_Sel2);
  GUI_popup__ (namPtr, NULL, 0, UI_GR_CB_Sel2, NULL);
  UI_GR_ButtonM1Release ();   // else KeyM1=ON ! 2013-05-01
  // Selektion --> UI_GR_Select1 (100, ..

  return 0;





  //============== Mode 100 ================================0
  // User hat Popup-Eintag # *dlInd selektiert ...
  L_mode100:
  if(*dlInd < 0) return 0;

  if(mode > 100) goto L_mode101;
    // printf(" L_mode100-select: \n");

    // dump selTab
    // printf("UI_GR_Select_work1-100 i=%ld typ=%d dbi=%ld dli=%ld nam=|%s|\n",
      // *dlInd, selTab[*dlInd].typ, selTab[*dlInd].dbInd, selTab[*dlInd].dlInd,
      // namTab[*dlInd]);
    // UI_GR_dump_selTab (selTab, namTab, selNr);


  GR_selDli = selTab[*dlInd].dlInd;
    // printf(" set GR_selDli %ld\n",GR_selDli);



  // test if ConstrPlane selected (namTab ist nicht static)
  if(selTab[*dlInd].typ == Typ_TmpPT) {
    UI_GR_set_selNam (Typ_TmpPT, 0L, NULL);
      // printf(" ConstrPlane sel |%s|\n",GR_selNam);
    goto L_100_w;
  }


  // Interaktion
  if(selTab[*dlInd].typ == Typ_Activ) {
    UI_GR_set_selNam (Typ_Activ, selTab[*dlInd].dbInd, NULL);
    goto L_100_w;
  }



  // haben Obj-Ind; was fuer ein APT-Ind / Typ ist das ?
  // store sel. obj in GR_selNam, GR_selTyp, GR_selDbi
  // vectors do not have a useful dli
  if(selTab[*dlInd].typ == Typ_VC) {
    UI_GR_set_sel_obj (Typ_VC, selTab[*dlInd].dbInd);


  // std-planes do not have a useful dli
  } else if((selTab[*dlInd].typ == Typ_PLN) &&
            (selTab[*dlInd].dbInd < 0)){
      // printf(" std-plane selected ..\n");
    UI_GR_set_sel_obj (Typ_PLN, selTab[*dlInd].dbInd);


  } else {
    // set GR_selNam, GR_selTyp, GR_selDbi
    // UI_GR_set_sel__ (selTab[*dlInd].dlInd);
    UI_GR_set_selNam (selTab[*dlInd].typ, selTab[*dlInd].dbInd, namTab[*dlInd]);
  }

  // override UI_GR_set_sel_obj, UI_GR_set_selNam  2014-12-20
  strcpy (GR_selNam, namTab[*dlInd]);  // for eg "L(S MOD)"


  L_100_w:
  // unhilite last displayed object
  UI_GR_Select3 (-1L);

  // work
  UI_GR_Select2 (selTab[*dlInd].typ,
                 selTab[*dlInd].dbInd,
                 selTab[*dlInd].dlInd);

  return 0;






  //============== Mode 101 ================================0
  // preview obj
  // if(mode >= 100) goto L_mode101;
  L_mode101:
  if(mode > 101) goto L_mode102;
    // GL_sel_dump ();
    // printf(" L_mode101-preview: \n");

  // skip objects previewed later:
  typ = selTab[*dlInd].typ;
  if((typ == Typ_Angle)   ||        // see IE_cad_Inp_disp_ang
     (typ == Typ_Tra)) return 0;

  // indicate|ConstrPln: Kreiserl an der VertexPosition
  if(selTab[*dlInd].typ == Typ_TmpPT) {
    DL_hili_on (-1L); // loeschen der Hili-Tabelle
    l1 = -2L;
    UI_GR_DrawInit ();
    // GL_DrawPoint (&l1, Typ_Att_hili1, &selPos);
    GL_DrawSymB (&l1, Typ_Att_hili1, SYM_CIR_S, &selPos);  // rot
      // printf("GL_DrawSymB l1=%ld\n",l1);
    GL_Redraw();
    UI_GR_DrawExit ();
    return 0;
  }


  // preview vector
  if(selTab[*dlInd].typ == Typ_VC) {
      // printf(" preview vec %ld\n",selTab[*dlInd].dbInd);
    UI_disp_vec1 (Typ_Index, (void*)selTab[*dlInd].dbInd, &selPos);
    GR_dli_hili = DLI_TMP;
    return 0;
  }


  // preview subcurv
  if(namTab[*dlInd][1] == '(') {
    UI_GR_Select3 (-1L);
    UI_GR_disp_oid (namTab[*dlInd]);
    GR_dli_hili = DLI_TMP;
    return 0;
  }


 
  // preview normal obj hilited
  UI_GR_Select3 (selTab[*dlInd].dlInd);    // hilite
  return 0;




  //============== Mode 102 ================================0
  // clear preview obj
  L_mode102:
    // printf(" unHile %d %ld\n",selTab[*dlInd].typ,selTab[*dlInd].dbInd);
  // unhilite last displayed object
  UI_GR_Select3 (-1L);
  // DL_hili_off (selTab[*dlInd].dbInd);
    // DL_hili_on (-1L); // loeschen der Hili-Tabelle


  return 0;

}


//================================================================
  int UI_GR_disp_oid (char *oid) {
//================================================================
// preview subcurv

  int       i1, typ;
  long      dbi, dli;
  ObjAto    ato;


  // printf("UI_GR_disp_oid |%s|\n",oid);


  ATO_getSpc__ (&ato);
  i1 = ATO_ato_srcLn__ (&ato, oid); // 
  if(i1) {
    TX_Error ("Error ATO_ato_srcLn__ %d",i1);
    return -1;
  }
    // ATO_dump__ (&ato, " after _ato_srcLn__");

  typ = ato.typ[0];
  dbi = ato.val[0];

  // preview obj
  dli = DLI_TMP;
  GR_Draw_dbo (&dli, typ, dbi, 9, 0);
  DL_Redraw ();

  return 0;

}


//===========================================================================
   int UI_GR_dump_selTab (ObjDB *selTab, char namTab[][SELTABLEN], int iNr) {
//===========================================================================
// dump selTab

  int    i1;

  printf("UI_GR_dump_selTab %d\n",iNr);

  for(i1=0; i1<iNr; ++i1) {
    printf(" selTab[%d] typ=%d dbi=%ld dli=%ld |%s|\n", i1,
                   selTab[i1].typ, selTab[i1].dbInd,
                   selTab[i1].dlInd, namTab[i1]);


  }
  return 0;

}


//================================================================
   int UI_GR_dump_dlTab (ObjDB *dlTab, int iNr) {
//================================================================
// dump selTab
    
  int    i1;
    

  printf("UI_GR_dump_dlTab %d\n",iNr);
  
  for(i1=0; i1<iNr; ++i1) {
    printf("dlTab[%d] typ=%d dbi=%ld dli=%ld\n",i1,
            dlTab[i1].typ,dlTab[i1].dbInd,dlTab[i1].dlInd);
  } 

  return 0; 
      
}     
      




//====================================================================
  int UI_GR_Select2 (int typ, long dbi, long dli) {
//====================================================================
// Popup-obj sected; work ..


  int     i1;

  // printf("UI_GR_Select2 %d %ld %ld\n",typ,dbi,dli);
  // printf(" UI_stat_hide=%d\n",UI_stat_hide);
  // printf(" UI_stat_view=%d\n",UI_stat_view);



  // VIEW: transfer now .
  if(UI_stat_view == 0) {
    // VIEW is active;
    // GA_view__ (dli, 0, 0, 0L);
    GA_view__ (dli, 2, typ, dbi);    // 2013-01-15
    DL_Redraw ();           // update display
    return 0;
  }


/*
  if(dli >= 0) {
    // das Obj mal hiliten; aber nicht in Mode MAN und CAD
    // do not hilite if parentObj wanted ..
    if(GR_Sel_Filter != 19)
      UI_GR_Select3 (dli);

  }
*/


  // M3: display objMenu
  if(GR_Event_Act == GUI_MouseR) {

    // in MAN: if nothing selected - imply return-key
    if(UI_InpMode == UI_MODE_MAN) {
      i1 = ED_query_CmdMode (); // -1=empty, 0=DefLn..
        // printf("  _Select2 i1=%d\n",i1);
      if(i1 < 0) { 
        // line is empty ..
        OMN_selMen_MAN_M3_empty (typ, dbi, dli);
      } else {
        // line is not empty ..
        UI_GR_selMen_init (1);
      }
      goto L_exit;

      // if(typ == Typ_TmpPT) {
        // UI_EdKeyCR (2);   // implizites CR
      // }


    } else {     // M3-VWR,CAD
      // create & display popup-menu.
      OMN_popup_Brw (typ, dbi, dli, 0);
      return 0;
    }
  }



  // select ..  // work filters,
  UI_GR_Select_work1 (dli);


  L_exit:
  return 0;

}


//====================================================================
  int UI_GR_Select3 (long objInd) {
//====================================================================
// hilite obj and/or unhilite last displayed object
// objInd < 0      unhilite last displayed object

  int   typ;
  long  dbi;


  // printf("UI_GR_Select3 objInd=%ld GR_dli_hili=%ld\n",objInd,GR_dli_hili);

  // if(UI_InpMode == UI_MODE_CAD) return 0;   // 2011-10-19


  // unhilite last displayed object
  if(GR_dli_hili != DLI_TMP) 
    DL_hili_off (GR_dli_hili);
  else
    GL_temp_Delete (GR_dli_hili);    // remove parent-obj


  if(objInd < 0) {
    GR_dli_hili = -1L;
    return -1;
  }



  // unhilite active CAD-obj
  // IE_cad_ClearAct ();  raus 2011-10-03 - löscht tempPt + vec weg ..


  // check if this is a parent-obj
  if(DL_parent_ck_p (objInd)) {
      // printf(" .. is parent ..\n");
    typ = DL_GetTyp (objInd);
    dbi = DL_GetInd (objInd);
    IE_parent_disp (typ, dbi);
    GR_dli_hili = DLI_TMP;


  } else {

    // hilite normal obj

  // das Obj mal hiliten; aber nicht in Mode MAN und CAD
  // if(ckb_search_stat == 0) {

  // nicht wenn HIDE aktiv ist ..
  // if(ckb_hide_stat == 0) return 0;

  // hilite; not if (GR_Sel_Filter == 5) (add to group)
  // if(GR_Sel_Filter != 5) {
    DL_hili_on (objInd);
    GR_dli_hili = objInd;

  }


  // GL_Redraw ();
  DL_Redraw ();


  // hilite row in browserWindow
  if(UI_UserSelFunc == NULL)   // not for userSelections
    Brw_hili_dli (objInd);


  // printf("ex UI_GR_Select3\n");

  return 0;

}


//================================================================
  int UI_GR_Sel_Filter (int mode) {
//================================================================
//-1 = query state
// 0 = OFF
// 1 = Posi2P (give temporary-point from cursorposition on constrPlane, no sel.)
// 2 = Vert2P
// 3 = Obj2P (give temporary-point from obj-selection) see also Typ_Vertex
// 4 = dump
// 5 = addGrp
// 6 = modLtyp
// 7 = modSstyl
// 8 = modify surfaceColor
// 9 = reset Col
// 10  reset transpar
// 11  50 % transpar
// 12  100 % transpar
// 13  remove texture
// 14  apply texture
// 15  load texture
// 16  modify Position
// 17  input form
// 18  Obj2PP   (give parametric-point from cursorposition on selected object)
// 19  Obj2LN   (give line from polygon/contour)     
// 20  NO select/indicate inhibited
// 21  object_with_activity_only

// used in UI_GR_Select_work1

// OFFEN: mode=6 sollte zB alle anderen aktiven (zB addToGroup oder
//   modSstyl ausschalten !!!)

  int    actFilt;


  // printf("UI_GR_Sel_Filter %d %d\n",mode,GR_Sel_Filter);


  if(mode == -1) return GR_Sel_Filter;    // query only


  // nochmal selektieren == Reset
  if((mode == 0)              ||
     (mode == GR_Sel_Filter))    {
    GR_Sel_Filter = 0;
    UI_Set_infoSel (0);
    return 0;
  }

  if(mode == 1) {
    UI_Set_infoSel (mode);   // infolabel in upper toolbar
    

    // else if(mode == 1) TX_Print("give position of cursor on constr.plane ..");
  } else if(mode == 2) MSG_pri_0("FILT02");
    else if(mode == 3) MSG_pri_0("FILT03"); //"give position of cursor on selected obj ..");
    else if(mode == 5) MSG_pri_0("FILT05"); //"add following selections/keyIn to group, end group with Tab-Key");
    else if(mode == 6) MSG_pri_0("FILT06"); //"sel Line|Circ|Curve to modify ..
    else if((mode >= 7)&&
            (mode < 13)) MSG_pri_0("FILT07"); //"sel surf to modify
    else if(mode == 13) MSG_pri_0("FILT13"); //"select surf to remove texture"
    else if(mode == 14) MSG_pri_0("FILT14"); //"select surf to apply texture"
    else if(mode == 15) MSG_pri_0("FILT15"); //"select surf to load texture"
    else if(mode == 16) MSG_pri_0("FILT16"); //"modify position: select subModel"

    else if(mode == 18) {                    //"give parametric-point
      MSG_pri_0("FILT18");
      UI_Set_infoSel (mode);   // infolabel in upper toolbar

    } else if(mode == 19) {
      MSG_pri_0("FILT19");
      UI_Set_infoSel (mode);   // infolabel in upper toolbar

  } else if(mode == 20) UI_CursorNo (0);


  actFilt = GR_Sel_Filter;

  if(actFilt == 20) UI_CursorNo (1);

  GR_Sel_Filter = mode;

  // printf("UI_GR_Sel_Filter %d\n",mode);

  // if(GR_Sel_Filter == 0) GR_Sel_Filter = mode;
  // else GR_Sel_Filter = 0;

  return actFilt;

}


//================================================================
  int GR_set_constPlnPos () {
//================================================================
/// \code
/// GR_set_constPlnPos     save of selectionPoint on constructionPlane
/// \endcode

  Point    pt1;


  // get mousePos in userCoords
  pt1 = GL_GetCurPos ();

  // returns worldCoords !
  GR_CurUk = GL_GetConstrPos (&pt1);

  return 0;

}


//================================================================
  int GR_get_constPlnPos (Point *po) {
//================================================================
/// \code
/// GR_get_constPlnPos     ?
/// \endcode


  // returns worldCoords !
  *po = GR_CurUk;

  return 0;

}





/*
//================================================================
  gint UI_GR_Scroll (void *widget, GdkEventScroll *ev) {
//================================================================
// 2010-11-30

  double  d1;


  // printf("UI_GR_Scroll %d\n",ev->direction);


  if(ev->direction == GDK_SCROLL_UP) d1 = 0.8;
  else                               d1 = 1.2;
  UI_GR_DrawInit ();
  GL_Set_Scale (GL_get_Scale() * d1);
  UI_GR_DrawExit ();


  return FALSE;  // do defaultOperations

}
*/
/* UNUSED
//================================================================
  gint UI_GR_ButtonPress (void *widget, void *event) {
//================================================================
/// callback mousebuttonpress and startDrag

// Cursor-Positions:
// ScreenPos         GL_mouse_x_act      GL_Do_Idle()
// ConstrPlanePos    GR_CurUk            GR_set_constPlnPos() GL_GetConstrPos()
// viewPlanePos      GL_actScrPos        GL_set_viewPlnPos()
//                   GL_actUsrPos        GL_GetViewPos()
// SelObjPos         GR_selPos           GL_MousePos() GL_Mouse1Pos()


  int    irc, x, y, mode, typ, i1, i2, dx,dy;
  long   dbi, dli, objInd;
  Point  pt1;

  GdkModifierType state;


  state = ((GdkEventButton*)event)->state;
  typ   = ((GdkEventButton*)event)->type;

  gtk_widget_get_pointer (GL_widget, &x, &y);

  objInd = -1L;


  // printf("BBBBBBBBBBBBBB UI_GR_ButtonPress button=%d\n",event->button);
  // printf(" _GR_ButtonPress x=%d y=%d typ=%d state=%d\n",x,y,typ,state);
  // printf(" _GR_Sel_Filter=%d\n",GR_Sel_Filter);
  // printf(" Ctrl=%d\n",KeyStatCtrl);
  // if(UI_UserSelFunc) printf(" UI_UserSelFunc exists!\n");


  // skip sel if locked ..
  if(((GdkEventButton*)event)->button == 1)  {
    if(GR_Sel_Filter == 20) return FALSE;
  }


  GUI_gl_block (&winMain, 1); // block mousemoves, keystrokes & grafic_selections
  UI_CursorWait (0);    // wait-cursor


  //  save mousepos -> GL_mouse_x_act,GL_mouse_y_act
  GL_Do_Idle (&dx, &dy, x, y);
 

  // selection: set mouseposition
  if((KeyStatCtrl == OFF)&&(KeyStatShift == OFF)) {    // nur ohne Shift/Ctrl-key

    // DL_hili_off (-1L);// reset hili: ohne dieser zeile doppelte Obj in popup ?
    // Punkt auf Flaeche geht dzt nur damit (no hilited surfaces) ...
    GL_Redra__ (0);       // find selected point
    GL_MousePos (&pt1);
    sele_set_pos (&pt1);


    // get userCoords on viewPlane & constructionPlane 2009-05-25
    GR_set_constPlnPos();    // set point GR_CurUk on constructionPlane
    GL_set_viewPlnPos ();    // set point GL_actUsrPos on viewPlane

  }



  //========== M1|M3==================================================
    // printf(" butt L01\n");

  if(((GdkEventButton*)event)->button == 3)  {
    if(KeyStatCtrl == ON)  {
      // Ctrl-M3
      if(UI_InpMode == UI_MODE_CAD) {
        if(IE_get_Func() < 0) {
          UI_EdKeyCtrlM3 (); // disp grp-menu
          goto Fertig;
        }
      } else if((UI_InpMode == UI_MODE_VWR) ||
                (UI_InpMode == UI_MODE_MAN))  {
        UI_EdKeyCtrlM3 (); // disp grp-menu
        goto Fertig;
      }
    }
  }

  if((((GdkEventButton*)event)->button == 1)||
     (((GdkEventButton*)event)->button == 3))  {

    if(((GdkEventButton*)event)->button == 1) { 
      GR_Event_Act = GUI_MouseL;
      KeyM1 = ON;

    } else {        // M3
      KeyM3 = ON;
      GR_Event_Act = GUI_MouseR;
    }


    // check for double-click
    if(typ == GDK_2BUTTON_PRESS) {
      GR_Event_Act = GUI_Mouse2L;
        printf("Doppelklick\n");
        // geht bei select nicht, weil man manchmals aus Liste waehlen muss...
        DL_Redraw ();  // CAD: after selection ConstPlane is unvis
        goto Fertig;
    }



    // CI_Mouse (1,KeyStatShift,KeyStatCtrl,KeyStatAlt,x,y);



  //--------------------------------------------------
  // Ist Zoomen gewuenscht -
/
  if(Stat_Zoom == 1) {

    // an der actPos eine hor & und eine vert-Linie hinmalen
    //pt1 = GR_sk2uk (ix, iy);
    //GRView_tmpSym (1, ix, iy);
    GL_Start_RubberBox ();
    //GRView_DrawExecute ();


    Zoom_x = ix;
    Zoom_y = iy;

    TX_Print("Selektieren Sie die rechte obere Ecke");
    Stat_Zoom = 2;

    SetCursor (LoadCursor (NULL, IDC_SIZENWSE));

    goto Fertig;



  } else if (Stat_Zoom == 2) {

    GL_Stop_RubberX (&d1, &pt1);

    GL_Rescale (d1, &pt1 );
    GRView_DrawExecute ();



    ED_update();  // Focus aufs Edit-Win.


    Stat_Zoom = 0;
    goto Fertig;

  }
/

  //--------------------------------------------------
  // kein Zoomen - normale Selektion


    // // is hide, view aktiv ? (0 = Yes)
    // ckb_hide_stat = 1 - GTK_TOGGLE_BUTTON (ckb_hide)->active;
    // ckb_view_stat = 1 - GTK_TOGGLE_BUTTON (ckb_view)->active;


    // Check ob ViewZ aktiv ist
    // UI_AP (UI_FuncGet, UID_ckb_vwz, (void*)&ckb_vwz_stat);
    // if(ckb_vwz_stat == 0) mode = 1;          // ViewZ YES

    if(UI_InpVWZ == ON) {
      mode = -1;


    } else {
      mode = 0;

      if(UI_InpMode == UI_MODE_CAD) {
        // M3: if CAD-OK is active: no menu, give OK=Enter.  2010-04-29
        if(GR_Event_Act == GUI_MouseR) {     // M3
          if(IE_get_Func() >= 0) {
            IE_inp_selection_clear ();    // clear selection
            IE_cad_InpIn_CR ();
            goto Fertig;
          }
        }
        // wenn CAD / seek Vertex aktiv: sofort retour.
        IE_cad_get_func (&i1, &i2);
        if((i1 == 15) && (i2 == 4)) {   // IE_cad_z 4
          objInd = -1L;
          goto L_11;
          // UI_GR_Select_work1 (0L);
          // return 0;
        }
      }
    }



    //--------------------------------------------------
    // ist SelectionFilter definiert ?
    if(GR_Sel_Filter != 0) {
      if(GR_Sel_Filter < 3) {  // nur indicate u vertex
        i1 = GR_Sel_Filter;
        GR_Sel_Filter = 0;
        if(i1 == SEL_Posi2P) UI_GR_Indicate();
        else if(i1 == SEL_Vert2P) UI_GR_SelVert(NULL);
        goto Fertig;
      }
    }


    // remove last selection (avoid that, if noPickable obj selected, the
    // previous selected obj is stored
    UI_GR_set_sel__ (-1L);


    // no selection if shift or ctrl is on        2010-09-28
    if((KeyStatCtrl != OFF)||(KeyStatShift != OFF)) {
      // printf(" skip selection if shift or ctrl is on\n");
      goto Fertig;
    }

    // selection (Obj analysieren)
    irc = UI_GR_Select1 (mode, &objInd);
    // irc=0: more than 1 obj's found !
      // printf(" nach UI_GR_Select1 %d %d %ld\n",irc,mode,objInd);
      // printf(" GR_Sel_Filter=%d GR_selTyp=%d\n",GR_Sel_Filter,GR_selTyp);

    // GL_Select (mode, &objInd);
    // GR_SelObj = objInd;              // der DLindex des zuletzt sel. obj
    // hier kein swapuffers!
    //GRView_DrawExecute ();



    // if(ckb_vwz_stat == 0) {        // ViewZ YES               do RotCen
    if(UI_InpVWZ == ON) {
      UI_GR_view_set_Z (objInd);
      UI_InpVWZ = OFF;
      goto Fertig;
    }


    L_11:
      // printf(" L_11: objInd=%d, UI_InpSM=%d\n",objInd,UI_InpSM);


    if((UI_UserSelFunc != NULL)&&(irc))  {                // 2011-06-26
      UI_UserSelFunc (GR_Event_Act, objInd);
      goto Fertig;
    }


    //------- M3-Indicate ----------------------
    if((GR_Event_Act == GUI_MouseR) &&           // M3
       (irc == 1) &&
       (GR_selTyp == Typ_TmpPT)) {                 // indicate
        printf(" M3-ind.\n");
      DL_Redraw ();           // else grid is missing ..
      TX_Print (" .. group is empty, nothing selected - ?");
    }


    //------- M1-Indicate ----------------------
    if((GR_Event_Act == GUI_MouseL) &&           // M1
       (irc == 1) &&                               // nr of objs
       (GR_selTyp == Typ_TmpPT)) {                 // indicate

        // printf(" indicate ..\n");

      // indicate in group-mode useless ..
      if(GR_Sel_Filter == 5) {
        TX_Print("***  groupmode; select objects ..");
        goto Fertig;
      }

      // if((ckb_search_stat == 0) ||    // suchen YES
      // if((UI_InpSM == ON)       ||    // suchen YES (ON=0)
      if((UI_stat_hide == 0)   ||    // hide YES
         (UI_stat_view == 0)) {      // view YES
        // TX_Error ("- nix gfundn");
        TX_Write (" - ? ");
        DL_hili_off (-1L);
        DL_Redraw ();
        goto Fertig;
      }

      // SMD:
      if(UI_InpMode != UI_MODE_VWR) {
        if(KeyStatCtrl == ON) {
          TX_Print("select object to modify/delete ...");
          goto Fertig;
        }
      }

      // UI_GR_Indicate ();  // or call UI_UserSelFunc
      // goto Fertig;
    } 

    

    // one obj selected
    if(irc == 1) {
      GL_sel_get (&typ, &dbi, &dli, 0);          // get GR_selTab[0]
      UI_GR_Select2 (typ, dbi, dli);             // work obj selected

/
      // M3: call subMenu
      if(GR_Event_Act == GUI_MouseR) {
        UI_GR_Select3 (dli); // hilite this obj
        OMN_popup_Brw (typ, dbi, dli, 0);        // display objectMenu
        goto Fertig;
      }
      // add obj to grp
      DL_grp1__ (dli, NULL, 0, 0);
      // userfunction ?
      if(UI_UserSelFunc != NULL) UI_UserSelFunc (GUI_MouseL, dli);
      // select ..
      UI_GR_Select_work1 (dli);
/
    }  

    // more than 1 obj in selectionSet

    goto Fertig;
  }




  //========== M2 ====================================================
  // if (state & GDK_BUTTON2_MASK) {
  if (((GdkEventButton*)event)->button == 2) {
    KeyM2 = ON;
    GR_Event_Act = GUI_MouseM;
    // printf("B2\n");
    // CI_Mouse (2,KeyStatShift,KeyStatCtrl,KeyStatAlt,x,y);
    if(UI_UserSelFunc != NULL) {
      UI_UserSelFunc (GUI_MouseM, objInd);
      goto Fertig;
    }

    // ist CAD aktiv ?
    if(UI_InpMode == UI_MODE_CAD) {
      IE_cad_selM2 (-1);
    }

    if(UI_InpMode == UI_MODE_MAN) {
      UI_GR_selMen_init ();
    }

  }




/
  //========== M3 ====================================================
  // if (state & GDK_BUTTON3_MASK) {
  L_M3:
  if(event->button == 3) {
    KeyM3 = ON;
    GR_Event_Act = GUI_MouseR;

    // printf("+B3\n");
    // CI_Mouse (3,KeyStatShift,KeyStatCtrl,KeyStatAlt,x,y);


    // // Userfunc aktiv ?
    // if(UI_UserSelFunc != NULL) {
      // UI_UserSelFunc (GUI_MouseR, objInd);
      // goto Fertig;
    // }
// 

    if(KeyStatCtrl == ON) {  // Ctrl-Key is on
      UI_EdKeyCtrlM3 ();

    } else {
      // implizites CR
      UI_EdKeyCR (2);
    }
  }

  
  // UI_GR_view_set_func ();
/



  Fertig:
  // printf("ex UI_GR_ButtonPress %d %d %d\n",x, y, state);
  UI_CursorWait (1);          // reset cursor from wait
  GUI_gl_block (&winMain, 0); // unblock keystrokes & grafic_selections
  return FALSE;

}
*/

//================================================================
  int UI_GR_ButtonM1Release () {
//================================================================
// solves GTK-Problem: UI_GR_ButtonRelease after exit popup, Dialog.. not fired
// necessary for PED_points

  KeyM1 = OFF;
  
  return 0;

}

/* UNUSED
//================================================================
  gint UI_GR_ButtonRelease (void *widget, void *event) {
//================================================================
/// callback buttonrelease & exit drag


  int x, y;
  GdkModifierType state;


  // printf("BBBBBBBBBBBBBBBBBBBBBBBB UI_GR_ButtonRelease \n");


  UI_block__ (-1, 1, 1);  // block UI

  state = ((GdkEventButton*)event)->state;
  // gdk_window_get_pointer (NULL, &x, &y, &state);

  // printf("UI_GR_ButtonRelease %d %d state=%d\n",x,y,state); 
  // printf("UI_GR_ButtonRelease button=%d\n",event->button); 

  // if ((state & GDK_BUTTON1_MASK) == 0) {
  if(((GdkEventButton*)event)->button == 1) {
    KeyM1 = OFF;
      // printf(" KKKKKKKKKKKKKKKKKKKKKKKKKKK release M1\n");
    // is rubberbox active: close !
    GL_RubberBox_stop ();


  // if ((state & GDK_BUTTON2_MASK) == 0) {
  } else if(((GdkEventButton*)event)->button == 2) {
    // printf(" reset KeyM2\n");
    KeyM2 = OFF;


  // if ((state & GDK_BUTTON3_MASK) == 0) {
  } else if(((GdkEventButton*)event)->button == 3) {
    // printf(" reset KeyM3\n");
    KeyM3 = OFF;
  }


  UI_block__ (-1, 0, 0);  // reset UI


  return FALSE;
}
*/
/*
//===============================================================
  gint UI_GR_KeyPress (void *widget, void *event) {
//====================
// wird derzeit nicht benutzt
// 
// 
// GDK_F1 - GDK_F12
// GDK_Return
// GDK_BackSpace
// GDK_Left
// GDK_Right


  double d1;


  printf("UI_GR_KeyPress %d /%c/\n",event->keyval,event->keyval);


  // F-Keys 1 - 35
  if ((event->keyval >= GDK_F1) &&
      (event->keyval <= GDK_F35)) { goto FuncKey; }



  switch (event->keyval) {


  case GDK_Shift_L:
  case GDK_Shift_R:
    printf("Shift ON\n");
    KeyStatShift = ON;
    goto Finish;


  case GDK_Alt_L:
  case GDK_Alt_R:
    printf("Alt ON\n");
    KeyStatAlt = ON;
    goto Finish;


  case GDK_Control_L:
  case GDK_Control_R:
    printf("ctrl ON\n");
    KeyStatCtrl = ON;
    goto Finish;



  case GDK_Up:
    UI_GR_DrawInit ();
    d1 = GL_get_Scale ();
    d1 *= 1.3;
    GL_Set_Scale (d1);
    UI_GR_DrawExit ();
    goto Finish;

  case GDK_Down:
    UI_GR_DrawInit ();
    d1 = GL_get_Scale ();
    d1 /= 1.3;
    GL_Set_Scale (d1);
    UI_GR_DrawExit ();
    goto Finish;



  case GDK_Left:
  case GDK_Right:
  case GDK_Escape:



    goto FuncKey;
  // case GDK_Return:
  // case GDK_BackSpace:


  default:
  goto StdKey;
  }



  FuncKey:
  CI_FKey (event->keyval,KeyStatShift,KeyStatCtrl,KeyStatAlt);
  goto Finish;


  StdKey:
  CI_Key ((char)event->keyval,KeyStatShift,KeyStatCtrl,KeyStatAlt);
  goto Finish;


  Finish:
  // prevent the default handler from being run
  // gtk_signal_emit_stop_by_name (GTK_OBJECT(widget),"key_press_event");
  return TRUE;


}
*/


/*
//===============================================================
  gint UI_GR_KeyRelease (void *widget, void *event) {
//======================

  // printf("UI_GR_KeyRelease %d\n",event->keyval);

  switch (event->keyval) {

  case GDK_Shift_L:
  case GDK_Shift_R:
    printf("Shift OFF\n");
    KeyStatShift = OFF;
    break;

  case GDK_Alt_L:
  case GDK_Alt_R:
    printf("Alt OFF\n");
    KeyStatAlt = OFF;
    break;


  case GDK_Control_L:
  case GDK_Control_R:
    printf("ctrl OFF\n");
    KeyStatCtrl = OFF;
    UI_ResetCursor ();
    break;

  }

  // prevent the default handler from being run
  gtk_signal_emit_stop_by_name(GTK_OBJECT(widget),"key_release_event");

  return TRUE;
}
*/


/* UNUSED
//================================================================
  gint UI_GR_Destroy (void *widget) {
//================================================================

  printf("UI_GR_Destroy\n");

  // return TRUE;
  return FALSE;
}
*/

//================================================================
  int UI_CursorWait (int mode) {
//================================================================
// mode = 0  change to wait-cursor (GDK_WATCH)
// mode = 1  reset cursor


  int   ii;

  // printf("UI_CursorWait %d\n",mode);


  if(mode == 0) {   // activate watch
    // actCur |= 1;
    ii = UI_ChangeCursor (1);   // GDK_WATCH);
    if(ii == 0) GUI_update__ ();    // wait for cursor-update
    return ii;
  }

  // actCur &= ~1;
  return UI_ResetCursor ();                

}
 

//================================================================
  int UI_CursorNo (int mode) {
//================================================================
// mode = 0  change to Nosel-cursor (GDK_MAN)
// mode = 1  reset cursor


  // printf("UI_CursorNo %d\n",mode);


  if(mode == 0) {   // activate watch
    // actCur |= 2;
    UI_ChangeCursor (5);   // GDK_MAN);


  } else {  // reset
    // actCur &= ~2;
    UI_ResetCursor ();
  }

  return 0;

}


//================================================================
  int UI_ResetCursor () {
//================================================================
// Reset zoom/rot/pan
// activate zoom/rot/pan with UI_ChangeCursor

  // printf("RRRRRRRRRRRRRRR UI_ResetCursor\n");

  // if((actCur & 1) > 0) {  // 1=wait
    // UI_ChangeCursor (GDK_WATCH);
  // } else if((actCur & 2) > 0) {  // 2=noSel
    // UI_ChangeCursor (GDK_MAN);
  // } else {
    UI_ChangeCursor (0);  // GDK_LEFT_PTR);
  // }

  return 0;
}


//================================================================
  int UI_ChangeCursor (int newCur) {
//================================================================
// GTK 1.2: geht nicht richtig; man muesste den Cursor ev. fuer alle
// windows explizit setzen. ....

//tatic int curTab[]={GDK_LEFT_PTR,       // 0 68   normal
//                    GDK_WATCH,           // 1 150
//                    GDK_EXCHANGE,        // 2 50   rotate
//                    GDK_HAND1,           // 3 58   move
//                    GDK_SIZING,          // 4 120
//                    GDK_X_CURSOR};       // 5 0    GDK_MAN,GDK_PIRATE};

// GDK_X_CURSOR            0
// GDK_LEFT_PTR           68   normal
// GDK_EXCHANGE           50   rotate
// GDK_HAND1              58   move
// GDK_DIAMOND_CROSS      36
// GDK_SB_V_DOUBLE_ARROW 116 
// GDK_SIZING            120
// GDK_WATCH             150
// GDK_MAN                80   no selections


  static int    actCur=0;

// vi /usr/include/gtk*/gdk/gdkcursors.h
// Sams S.266


  // printf("CCCCCCCCCCCC UI_ChangeCursor new=%d act=%d\n",newCur,actCur);

  if(actCur == newCur) return 1;

  actCur = newCur;

  // GUI_cursor (GL_widget, newCur);
  // GUI_cursor__ (&winGR, newCur);
  GUI_cursor__ (&winMain, newCur);

  // GUI_update__ ();
  // gdk_window_set_cursor(GL_widget->window,gdk_cursor_new(newCur));
  // while (gtk_events_pending()) {   // update display
    // gtk_main_iteration();
  // }

  return 0;
}



/*
//==================================================================== 
  void UI_GR_view_set_func () {
//==================================================================== 
// dzt unnutz
// die akt. funktionsbez. und den akt. cursor korr

// KeyM#:  0=ON=pressed, 1=OFF=not

  printf("UI_GR_view_set_func %d %d\n",KeyM2,KeyM3);



  // rot
  if((KeyM2 == ON)&&(KeyM3 == OFF)) {
    GR_Func_Act = GR_Func_Rot;
    UI_ChangeCursor (GDK_EXCHANGE); // f. rot
    goto Fertig;
  }


  // pan
  if((KeyM2 == OFF)&&(KeyM3 == ON)) {
    GR_Func_Act = GR_Func_Pan;
    UI_ChangeCursor (GDK_HAND1);    // f move
    goto Fertig;
  }


  // zoom
  if((KeyM2 == ON)&&(KeyM3 == ON)) {
    GR_Func_Act = GR_Func_Scl;
    UI_ChangeCursor (GDK_SIZING);   // f zoom
    goto Fertig;
  }


  if((KeyM2 == OFF)&&(KeyM3 == OFF)) {
    // printf(" cur reset\n");
    GR_Func_Act = 0;
    // UI_ResetCursor ();
    UI_ChangeCursor (GDK_LEFT_PTR); // default
    goto Fertig;
  }


    // UI_ChangeCursor (GDK_LEFT_PTR);
    // UI_ChangeCursor (GDK_CROSSHAIR);


  Fertig:
  return;

}
*/


//==================================================================== 
  void UI_GR_view_set_Z    (long objInd) {
//==================================================================== 
// wird von Select gerufen

  int     i1;
  // long    ind;
  double  zVal;
  Point   pt1, pt2, pt3;
  // ObjG    o1;



  printf("UI_GR_view_set_Z %ld\n",objInd);



/*
  // wurde ein Bitmap-symbol selektiert (eines der roten Sternderln) ?
  if(objInd >= 0) {
    if(DL_GetTyp(objInd) == Typ_SymB) {
      zVal = UI_vwz__ (1, objInd);
      goto L_fertig;
    }
  }
*/


  UI_vwz__ (2, 0L);  // die Bitmap-symbole (die roten Sternderln) weg


  // get Z-Val, umrechnen in UserCoords
  if(objInd >= 0) {
    sele_get_pos (&pt1);

  } else {
    i1 = GL_MousePos (&pt1);
    if(i1 < 0) {
      TX_Print("no object selected ..");
      goto L_exit;
    }
  }





/*
  // den dem Cursor naechsten vertex holen ...
  i1 = GL_SelVert__ (&pt1);
  if(i1 < 0) goto L_fertig;
  if(i1 < 0) { DL_Redraw (); return;}  // sternderln weg ..
*/

  zVal = pt1.z;
    // GR_Disp_pt (&pt1, SYM_STAR_S, 2);


  // den neuen Mittelpunkt setzen 
  GL_Set_Cen (&pt1);




/*
  AP_GetObj (&o1, objInd);
  // die Z-Hoehe des selektierten Elements-
  switch (o1.typ) {
      case Typ_PT:
        zVal = o1.p1.z;
        break;
      case Typ_LN:
        zVal = (o1.p1.z + o1.p2.z) / 2.0;
        break;
      case Typ_AC:
      case Typ_CI:
        zVal = o1.pc.z;
        break;
    }
*/


  L_fertig:

  // ViewPlane setzen ..
  UI_GR_view_set_Z1 (zVal);

  L_exit:

  // Reset ViewZ- Checkbox
  UI_AP (UI_FuncSet, UID_ckb_vwz, NULL);


}



//==================================================================== 
  void UI_GR_view_set_Z1   (double zVal) {
//==================================================================== 

  printf("UI_GR_view_set_Z1 %f\n",zVal);


  // das hebt die gelbe Plane, UND das Viewcenter !!!
  GL_Do_CenRot (zVal);

  // TX_Print("Darstellungsebene geaendert");


  // die Ausgabe (Entry Curpos Z) korrigieren
  UI_AP (UI_FuncSet, UID_ouf_vwz, (void*)&zVal);

}



/*
//==========================================================================
  int CI_Mouse (int buttNr,int KeyStatShift,int KeyStatCtrl,int KeyStatAlt,
                int x, int y) {
//============
//reports mousebutton-press-infos.
//
//buttNr 1 = Mousebutton 1;
//       2 = Mousebutton 2.
//       3 = Mousebutton 3.
//x,y    the position, in screencoords. 0,0 is top left.


  printf("CI_Mouse %d\n",buttNr);
  // printf("                  %d %d\n",x,y);



  // --------------------------------------------------------
  // ------------- handle Mouse-Button-1 --------------------
  if (buttNr == 1) {
    // printf("B1\n");


  } else if (buttNr == 2) {
    UI_ChangeCursor (GDK_EXCHANGE);


  } else if (buttNr == 3) {
    UI_ChangeCursor (GDK_HAND1);



  }

  return 0;
}
*/


/*
//========================================================================
  int CI_Key (char ltxt, int KeyStatShift,int KeyStatCtrl,int KeyStatAlt) {
//==========
// reports key-presses (each single char)


short       i1;


  // check for aux. functions, triggered by Ctrl-keys
//  if (AuxFun (ltxt,KeyStatShift,KeyStatCtrl,KeyStatAlt)) { return; }
//

  // printf("CI_Key |%c|\n",ltxt,


  i1 = strlen (CMD_Line);


  // BackSpace:
  if (ltxt == '\b') {
    if (i1) {i1--;}
    CMD_Line[i1] = '\0';
    goto Fertig;
  }

  // Return:
  if (ltxt == '\r') {
    printf(" Execute  |%s|\n",CMD_Line);
    CMD_Line[0] = '\0';
  }


  CMD_Line[i1] = ltxt;
  i1++;
  CMD_Line[i1] = '\0';



  Fertig:;
  UI_KeyFieldWri ();
  // printf(" CMD_Line now  |%s|\n",CMD_Line);
  return 0;

}
*/


/*
//=============================================================
  int CI_FKey (int key,
               int KeyStatShift,int KeyStatCtrl,int KeyStatAlt) {
//===========
// Function-Key or special-key pressed


  // -----------------------------------------------
  printf("CI_FKey |%d| %d %d %d\n",key,
                   KeyStatShift,KeyStatCtrl,KeyStatAlt);

  switch (key) {

    case GDK_Escape:
      gtk_exit (1);




    case GDK_Up:                     
      UI_GR_DrawInit ();
      GC_GR_Zoom0 (3);
      GC_GR_Do_Display ();
      UI_GR_DrawExit ();
      break;


    case GDK_Down:                 
      UI_GR_DrawInit ();
      GC_GR_Zoom0 (4);
      GC_GR_Do_Display ();
      UI_GR_DrawExit ();
      break;



    case GDK_F1:

      if (KeyStatShift == ON) {
        UI_GR_DrawInit ();
        GC_GR_FuncView (FUNC_DispRend);
        UI_GR_DrawExit ();
        break;
      }


      UI_GR_DrawInit ();
      GC_GR_FuncView (FUNC_ViewIso);
      UI_GR_DrawExit ();
      break;




    case GDK_F2:
      if (KeyStatShift == ON) {
        UI_GR_DrawInit ();
        GC_GR_FuncView (FUNC_DispWire);
        UI_GR_DrawExit ();
        break;
      }

      UI_GR_DrawInit ();
      GC_GR_FuncView (FUNC_ViewTop);
      UI_GR_DrawExit ();
      break;



    case GDK_F3:
      UI_GR_DrawInit ();
      GC_GR_FuncView (FUNC_ViewFront);
      UI_GR_DrawExit ();
      break;



    case GDK_F4:
      UI_GR_DrawInit ();
      GC_GR_FuncView (FUNC_ViewSide);
      UI_GR_DrawExit ();
      break;



    case GDK_F5:
      UI_GR_DrawInit ();
      GC_GR_FuncView (FUNC_ViewReframe);
      UI_GR_DrawExit ();
      break;



    default:
      printf(" was tun ?\n");
  }

  return 0;
}
*/



/*
//================================================================
  int UI_GR_setKeyFocus () {
//================================================================
//  set focus to glarea widget (nur f. KeyEvents erforderl)
//
// Muss leider nach jeder Interaktion (bzw vor jedem buttonpress)
//   gerufen werden.

  // printf("UI_GR_setKeyFocus\n");


  gtk_widget_grab_focus (GTK_WIDGET(GL_widget));

  UI_Focus = 0;      // wer Focus hat; 0=GL, 1=Edit, 2=ViewZ-Entryfeld

  return 0;

}
*/

/*
//================================================================
  void UI_GR_DrawInit () {
//================================================================

  // printf("UI_GR_DrawInit %d\n");

  gtk_gl_area_make_current (GTK_GL_AREA(GL_widget));

  // gtk_gl_area_begingl (GTK_GL_AREA(GL_widget));


  return;
}



//================================================================
  void UI_GR_DrawExit () {
//================================================================

  // printf("UI_GR_DrawExit %d\n");

  // GC_GR_Do_Display ();

  // gtk_gl_area_endgl (GTK_GL_AREA(GL_widget));
  gtk_gl_area_swap_buffers (GTK_GL_AREA(GL_widget));


  return;
}
*/


//================================================================
  int UI_GR_SelVert (Point *pto) {
//================================================================
/// get vertext nearest to cursor
// den dem Cursor naechsten vertex holen (wenn (pto == NULL));
// an ein aktives Userprog. abgeben mit UI_GR_SelVert (&Point);

static Point   pt1;

  long    dli;
  char    cbuf[256];


  printf("UI_GR_SelVert\n");


  // (pto != NULL):   an ein aktives Userprog. abgeben.
  if(pto) {
    *pto = pt1;
    return 0;
  }


  // GL_temp_delete (); // alle temp. obj loeschen
  // DL_hili_off (-1L); // // unhilite all

  GL_SelVert__ (&pt1); // den dem Cursor naechsten vertex holen

  // in UCS umrechnen ..
  if(WC_sur_ind != 0) {
    UT3D_pt_traptm3 (&pt1, WC_sur_imat, &pt1);
  }


  // Kreiserl an der VertexPosition
  dli = -1;
  GL_DrawSymB (&dli, Typ_Att_hili1, SYM_CIR_S, &pt1);  // rot
  DL_Redraw ();



  // UserFunction active ?? Hier nur indicate; ind dabei 0 !!
  if(UI_UserSelFunc != NULL) {
    UI_UserSelFunc (GR_Event_Act, -2L);
    return 0;
  }



  UTX_Clear (cbuf);
  AP_obj_add_pt_sp (cbuf, &pt1);  // "P(...)
  UI_GR_Select_work2 (Typ_TmpPT, 0L, cbuf, 0L);

  return 0;

}


//================================================================
  int AP_Mousemove2dx (double *dv, int dx, int dy) {
//================================================================
/// AP_Mousemove2dx      get single value from relative mousemove

  int      i1;
  double   dd;


  // get offset-value dd from mouse-offset dy,dy --------------
  // einen einzelnen Wert aus dx, dy errechnen.
  dd = IABS(dx) + IABS(dy);
  i1 = ISIGN(dx + dy);
  dd *= i1;

  // fix value according to scale
  dd *= 50;
  if(dd == 0) return 0;  // no mousemove
  *dv = dd / GL_get_Scale ();
    // printf("AP_Mousemove2dx %f dd=%d d1=%f\n",*dv,dd);

  return 0;

}


//================================================================
  int UI_GR_get_actPosA (Point *curPosAbs) {
//================================================================
/// \code
/// get current curPos in userCoords on constructionPlane
/// in absolute userCoords
/// Input:   GR_CurUk in worldCoords
/// Output:  curPosAbs in userCoords (relative to the active constrPlane)
/// see also GL_GetViewPos
/// \endcode

// retour into worldCoords:
// if(WC_sur_ind != 0) UT3D_pt_traptm3 (&pt1, WC_sur_imat, &pt1);


  // printf("UI_GR_get_actPosA GR_CurUk=%f,%f,%f\n",
          // GR_CurUk.x,GR_CurUk.y,GR_CurUk.z);
  


  // die PosKoord. GR_CurUk in ein Ausgabefenster geschr. werden.
  // sprintf(buf1, "%+10.3f  %+10.3f %+8.1f",GR_CurUk.x,GR_CurUk.y,GR_CurUk.z);
  if((WC_sur_ind != 0)&&(UI_RelAbs == 0)) {
    UT3D_pt_traptm3 (curPosAbs, WC_sur_imat, &GR_CurUk);
  } else {
    *curPosAbs = GR_CurUk;
  }


  // printf(" ex UI_GR_get_actPosA %f %f %f\n",
          // curPosAbs->x, curPosAbs->y, curPosAbs->z);

  return 0;

}

 
//================================================================
  int UI_GR_get_actPos_ (char **p) {
//================================================================
/// return (a pointer to) the active cursorPosition as string "P(<x> <y> <z>)"
/// change "ConstrPlane" -> "P(<x> <y> <z>)"


  // printf("UI_GR_get_actPos_ |%s|\n",GR_actPos);

  if((strlen(GR_actPos) < 2) ||
     (!strcmp(GR_actPos, "ConstrPlane"))) {
    // write actPos -> GR_actPos as "P(...)"
    UI_GR_actPos ();
  }

  *p = GR_actPos;

  return 0;

}

 
//================================================================
  int UI_GR_actPos () {
//================================================================
// write actPos -> GR_actPos as "P(...)"

  int    nkAnz;
  double d1;
  Point  pt2;



  // get Scale
  d1 = GL_get_Scale ();
  // printf("UI_GR_Indicate Scale = %f\n",d1);

  // get curPos
  UI_GR_get_actPosA (&pt2);


  //  entspr. Scale Anzahl d. nachkommastellen bestimmen
  nkAnz = 3;
  // if(d1 < 100.) nkAnz = 2;
  if(d1 < 10.)  nkAnz = 2;
  if(d1 < 1.)   nkAnz = 1;
  if(d1 < 0.1)  nkAnz = 0;


  strcpy(GR_actPos,"P(");
  UTX_add_fl_f  (GR_actPos, pt2.x, nkAnz);
  UTX_del_foll0 (GR_actPos);
  strcat (GR_actPos, " ");
  UTX_add_fl_f  (GR_actPos, pt2.y, nkAnz);
  UTX_del_foll0 (GR_actPos);
  strcat (GR_actPos, " ");
  UTX_add_fl_f  (GR_actPos, pt2.z, nkAnz);
  UTX_del_foll0 (GR_actPos);
  strcat (GR_actPos, ")");

    // printf("ex GR_actPos |%s|\n",GR_actPos);


  return 0;

}

 
//================================================================
  int UI_GR_Indicate () {
//================================================================
/// \code
/// get MousePos on Constr.Plane:
/// - as *Point on Constr.Plane with DB_get_PT(0L)
/// - as String ("P(x y z)") from GR_actPos
/// \endcode
/* wird nur mehr benutzt von Select / pos from cur on constrPln
UI_menCB |Posi2P|
  UI_GR_Sel_Filter (1);
    GR_Sel_Filter = 1;

schreibt ins CAD-Eingabefeld nur wenn diese leer ist !

*/

  int    nkAnz;
  long   l1;
  double d1;
  char   fmt[32];
  // char   GR_actPos[128];
  Point  pt1, pt2;



  printf("UI_GR_Indicate at %f %f %f\n",GR_CurUk.x,GR_CurUk.y,GR_CurUk.z);
  // printf(" \n");


  // write actPos -> GR_actPos as "P(...)"
  UI_GR_actPos ();

  
/*
  // if no point wanted: return            2010-05-06
  if(sele_ck_typ (Typ_PT) == 0) {
    if(UI_UserSelFunc != NULL) UI_UserSelFunc (GR_Event_Act, -1L);
    return 0;
  }
*/

/*
  /  wie steht der Scale ? 
  d1 = GL_get_Scale ();
  // printf("UI_GR_Indicate Scale = %f\n",d1);


  // get curPos
  UI_GR_get_actPosA (&pt2);
/
  UI_GR_get_actPosA 
  pt1.x = GR_CurUk.x;
  pt1.y = GR_CurUk.y;
  pt1.z = GR_CurUk.z;

  // retour ins absolute ..
  if(WC_sur_ind != 0) {
    UT3D_pt_traptm3 (&pt2, WC_sur_imat, &pt1);
      UT3D_stru_dump (Typ_PT, &pt1, "pt-ind: ");
  } else {
    pt2 = pt1;
  }
/

  // den Punkt als APT-Punkt P0 merken
  // DB_StorePoint(0L, &pt2);
  // GL_actUsrPos = pt2;


  // den formatstring erzeugen
  // Z-Wert zu wenn Z != 0
  // if(UTP_comp_0(GR_CurUk.z) != 0) {
    // sprintf(fmt,"P(%%.%df %%.%df)",nkAnz,nkAnz);
    // sprintf(GR_actPos,fmt,GR_CurUk.x,GR_CurUk.y);
  // } else {
    // sprintf(fmt,"P(%%.%df %%.%df %%.%df)",nkAnz,nkAnz,nkAnz);
    // sprintf(GR_actPos,fmt,GR_CurUk.x,GR_CurUk.y,GR_CurUk.z);
  // }
  // printf("Format: /%s/",fmt);


  //  entspr. Scale Anzahl d. nachkommastellen bestimmen
  nkAnz = 3;
  // if(d1 < 100.) nkAnz = 2;
  if(d1 < 10.)  nkAnz = 2;
  if(d1 < 1.)   nkAnz = 1;
  if(d1 < 0.1)  nkAnz = 0;


  strcpy(GR_actPos,"P(");
  UTX_add_fl_f  (GR_actPos, pt2.x, nkAnz);
  UTX_del_foll0 (GR_actPos);
  strcat (GR_actPos, " ");
  UTX_add_fl_f  (GR_actPos, pt2.y, nkAnz);
  UTX_del_foll0 (GR_actPos);
  strcat (GR_actPos, " ");
  UTX_add_fl_f  (GR_actPos, pt2.z, nkAnz);
  UTX_del_foll0 (GR_actPos);
  strcat (GR_actPos, ")");
    // printf(" GR_actPos |%s|\n",GR_actPos);
*/

  
 

  // UserFunction active ?? Hier nur indicate; ind dabei 0 !!
  if(UI_UserSelFunc != NULL) {
    UI_UserSelFunc (GR_Event_Act, -1L);

  } else {

    if(GR_Sel_Filter != 0) {
      if(GR_Sel_Filter == 16)
        return UI_Tra__ (0L, Typ_TmpPT, 0L);
      else if(GR_Sel_Filter == 17)
        return PRG_sel_CB (0L, Typ_TmpPT, 0L, GR_actPos);
    }



    // do hide/view S/M, VWR/CAD/MAN
    UI_GR_Select_work2 (Typ_TmpPT, 0L, GR_actPos, 0L);


    // MAN only: display indicated position
    // ( CAD removes this symbold selective if necessary)
    if(UI_InpMode == UI_MODE_MAN) {
        // UT3D_stru_dump (Typ_PT, &pt2, "MAN-pt2:");

      if(WC_sur_ind != 0) {
        UT3D_pt_traptm3 (&pt2, WC_sur_mat, &pt2);
      }

      // l1 = -2;
      // GL_DrawSymB (&l1, 2, SYM_CIR_S, &pt2);
      l1 = -1;
      // UI_GR_DrawInit ();
      GL_DrawSymB (&l1, Typ_Att_hili1, SYM_CIR_S, &pt2);  // Circ red
      // UI_GR_DrawExit ();
      DL_Redraw ();
    }

  }

  return 0;

}


/*=====================================================================*/
  int UI_GR_Select_work1 (long objInd) {
/*=====================================================================*/
// work filters (GR_Sel_Filter), then work (UI_GR_Select_work2)
// war im wc3 in OGL_GRView.cpp, OnLButtonDown.

  int       irc, i1, att;
  long      l1;
  // char      s1[128];
  Point     pSel, pS2;
  // Memspc    tmpSeg;



  // printf("UI_GR_Select_work1 %ld Filt=%d\n",objInd,GR_Sel_Filter);
  // printf(" GR_selTyp=%d GR_selDbi=%ld\n",GR_selTyp,GR_selDbi);
  // if(UI_UserSelFunc) printf(" UI_UserSelFunc exists!\n");



  //----------------------------------------------------------------
  L_ck_filt:
  if(GR_Sel_Filter == 18) {
      // give parametric-point from cursorposition on selected object

        // printf(" Obj2PP %d %ld\n",GR_selTyp,GR_selDbi);
      // GL_MousePos (&pSel);
      // GL_GetActSelPos (&pSel, &pS2);
        // UT3D_stru_dump (Typ_PT, &pSel, "pSel:");
        // sprintf(GR_selNam, "P(%f %f %f)", pSel.x, pSel.y, pSel.z);
      sele_get_pos (&pSel);

      // temp.display of position - nur im MAN-Mode
      if(UI_InpMode == UI_MODE_MAN) {
        l1 = -2;
        GL_DrawSymB (&l1, 2, SYM_CIR_S, &pSel);
      }

/*
      i1 = Typ_PT;
      // irc = AP_src_selObj (GR_selNam, i1, GR_selTyp, GR_selDbi);
      GR_selNam[0] = '\0';
      irc = SRC_dbo (GR_selNam, i1, GR_selTyp, GR_selDbi);
*/
      // get parametric-point from selected-point and selected obj
      // irc = SRC_parPt_ptDbo (GR_selNam, &pSel, GR_selTyp, GR_selDbi);
      irc = SRC_src_pt_dbo (GR_selNam, sizeof(GR_selNam), Typ_PT,
                            &pSel, GR_selTyp, GR_selDbi);
      if(irc < 0) {
        sprintf(GR_selNam,"***** cannot use selection of %s-%ld",
                AP_src_typ__(GR_selTyp), GR_selDbi);
        TX_Print(GR_selNam);
        return -1;
      }
      GR_selTyp = Typ_PT;         // output - exact.
      UI_GR_Sel_Filter (0);       // reset;
      goto L_done;
  }


  //----------------------------------------------------------------
  if(GR_Sel_Filter == 19) {
      // give line from obj (polygon/contour)
        // printf(" Obj2LN %d %ld\n",GR_selTyp,GR_selDbi);
      // GL_MousePos (&pSel);
      sele_get_pos (&pSel);
        // UT3D_stru_dump (Typ_PT, &pSel, "pSel:");
        // sprintf(GR_selNam, "P(%f %f %f)", pSel.x, pSel.y, pSel.z);

      // temp.display of position - nur im MAN-Mode
      if(UI_InpMode == UI_MODE_MAN) {
        l1 = -2;
        GL_DrawSymB (&l1, 2, SYM_CIR_S, &pSel);
      }
      // irc = SRC_parPt_ptDbo (GR_selNam, &pSel, GR_selTyp, GR_selDbi);
      // irc = SRC_LnAc_ptDbo (1, GR_selNam, &pSel, GR_selTyp, GR_selDbi);
      irc = SRC_src_pt_dbo (GR_selNam, sizeof(GR_selNam), Typ_LN,
                            &pSel, GR_selTyp, GR_selDbi);
      if(irc < 0) {
        sprintf(GR_selNam,"***** cannot use selection of %s-%ld",
                AP_src_typ__(GR_selTyp), GR_selDbi);
        TX_Print(GR_selNam);
        return -1;
      }
      GR_selTyp = Typ_LN;         // output - exact.
      UI_GR_Sel_Filter (0);       // reset;
      // goto L_done;
     goto L_ck_func;  // 2015-05-06
  }


  //----------------------------------------------------------------
  // decode selected object; change selected_object into requested_object
  irc = sele_decode ();
    // printf(" irc sele_decode = %d\n",irc);
    // sele_dump2 (); // print GR_selNam, GR_selTyp, GR_selDbi


  if(GR_selTyp == Typ_NULL) {                    // 2011-09-08
    DL_hili_off (-1L);
    DL_Redraw ();            // unhilite
    return -1;
  }


  // // MAN, CAD: if  Ctrl-Key is on: SMD - On setzen
  // if(UI_InpMode != UI_MODE_VWR) {
    // if(KeyStatCtrl == ON) {  // Ctrl-Key is on
      // UI_AP (UI_FuncSet, UID_ckb_search, (void*)TRUE);  // SMD - On setzen
    // }
  // }


  // sele_decode kann GR_Sel_Filter modifiziern !
  if(GR_Sel_Filter == 18) goto L_ck_filt;
  if(GR_Sel_Filter == 19) goto L_ck_filt;


  if(GR_Sel_Filter != 0) {

    //----------------------------------------------------------------
    if(GR_Sel_Filter == 3) {       // Obj2Point
      // give Text for Point on geometr. Obj near Cursor
        // printf(" Obj2P %d %ld\n",GR_selTyp,GR_selDbi);
      // get coords of sel. Obj
      GL_MousePos (&pSel);
        // UT3D_stru_dump (Typ_PT, &pSel, "pSel:");
      // new objTyp
      GR_selTyp = Typ_TmpPT;
      sprintf(GR_selNam, "P(%f %f %f)", pSel.x, pSel.y, pSel.z);
      // UI_GR_set_selNam (Typ_TmpPT, -1L, s1);
      // temp.display of position - nur im MAN-Mode
      if(UI_InpMode == UI_MODE_MAN) {
        l1 = -2;
        GL_DrawSymB (&l1, 2, SYM_CIR_S, &pSel);
      }


    //----------------------------------------------------------------
    // } else if(GR_Sel_Filter == 18) {
      // // give parametric-point from cursorposition on selected object
        // // printf(" Obj2PP %d %ld\n",GR_selTyp,GR_selDbi);
      // GL_MousePos (&pSel);
        // // UT3D_stru_dump (Typ_PT, &pSel, "pSel:");
        // // sprintf(GR_selNam, "P(%f %f %f)", pSel.x, pSel.y, pSel.z);
      // // temp.display of position - nur im MAN-Mode
      // if(UI_InpMode == UI_MODE_MAN) {
        // l1 = -2;
        // GL_DrawSymB (&l1, 2, SYM_CIR_S, &pSel);
      // }
      // // get parametric-point from typ/ind
      // i1 = Typ_PT;
      // // irc = AP_src_selObj (GR_selNam, i1, GR_selTyp, GR_selDbi);
      // GR_selNam[0] = '\0';
      // irc = SRC_dbo (GR_selNam, i1, GR_selTyp, GR_selDbi);
      // if(irc < 0) {
        // sprintf(GR_selNam,"***** cannot use selection of %s-%ld",
                // AP_src_typ__(i1), GR_selDbi);
        // TX_Print(GR_selNam);
        // return -1;
      // }
      // GR_selTyp = Typ_PT;         // output - exact.


    // //----------------------------------------------------------------
    // } else if(GR_Sel_Filter == 19) {
      // // return parentObj of selected object typ/ind
      // irc = GA_parent_get (&GR_selDbi, GR_selTyp, GR_selDbi);
      // if(irc < 0) {TX_Print("ERROR: parentObj does not exist .."); return -1;}
      // APED_oid_dbo__ (GR_selNam, GR_selTyp, GR_selDbi);
      // // hilite it (skipped it in UI_GR_Select2)
      // if(UI_InpMode == UI_MODE_CAD) {
        // IE_parent_disp (GR_selTyp, GR_selDbi);
      // } else if(UI_InpMode == UI_MODE_MAN) {
        // ED_parent_disp (GR_selTyp, GR_selDbi);
      // }



    //----------------------------------------------------------------
    } else if(GR_Sel_Filter == 4) {       // dump obj
      i1 = GR_Sel_Filter;
      GR_Sel_Filter = 0;           // reset
      UI_dump_obj (GR_selTyp, GR_selDbi);
      return 0;



    //----------------------------------------------------------------
    } else if(GR_Sel_Filter == 5) {       // grpAdd
      // check if groupBit of obj objInd is ON;
      // set groupBit for obj
      // printf(" grp=%d\n",objAtt.grp_1);
      // APED_oid_dbo__(GR_selNam, GR_selTyp, GR_selDbi);
      // return DL_grp1__(objInd, GR_selNam, 0);  // DL_grp1_set
      return DL_grp1__(objInd, NULL, 0, 0);  // DL_grp1_set

      // if(objAtt.grp_1 == ON) {
        // DL_grp1_set (objInd, OFF);
        // DL_hili_off (-1L); DL_Redraw (); // unhilite alle Objekte
        // TX_Print ("remove obj %s from group",GR_selNam);
        // return 0;
      // } else {
        // DL_grp1_set (objInd, ON);
        // TX_Print ("add obj %s to group",GR_selNam);
        // return 0;
      // }


    //----------------------------------------------------------------
    } else if(GR_Sel_Filter == 6) {                    // modLtyp
      // query lTyp
      i1 = UI_WinLtypMod(NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB1));
      return GA_lTyp__ (objInd, i1, GR_selTyp, GR_selDbi, 0);

    //----------------------------------------------------------------
    } else if(GR_Sel_Filter == 7) {                    // modSstyl
      // i1 = UI_WinSurfStyl(NULL, (void*)UI_FuncUCB1);   // query sStyl
      // query sStyl
      i1 = UI_WinSurfStyl(NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB1));
      return GA_sStyl__ (objInd, i1, GR_selTyp, GR_selDbi);

    //----------------------------------------------------------------
    } else if(GR_Sel_Filter == 8) {                    // modify Col
      return GA_Col__ (objInd, 0, GR_selTyp, GR_selDbi);

    //----------------------------------------------------------------
    } else if(GR_Sel_Filter == 9) {                    // reset Col
      return GA_Col__ (objInd, 1, GR_selTyp, GR_selDbi);

    //----------------------------------------------------------------
    } else if(GR_Sel_Filter == 10) {                    // reset transpar.
      return GA_Tra__ (objInd, 0, GR_selTyp, GR_selDbi);

    //----------------------------------------------------------------
    } else if(GR_Sel_Filter == 11) {                    // 50 % transpar.
      return GA_Tra__ (objInd, 1, GR_selTyp, GR_selDbi);

    //----------------------------------------------------------------
    } else if(GR_Sel_Filter == 12) {                    // 100 % transpar.
      return GA_Tra__ (objInd, 2, GR_selTyp, GR_selDbi);

    //----------------------------------------------------------------
    } else if(GR_Sel_Filter == 13) {                    // remove texture
      return GA_Tex__ (objInd, 0, GR_selTyp, GR_selDbi);

    //----------------------------------------------------------------
    } else if(GR_Sel_Filter == 14) {                    // apply texture
      return GA_Tex__ (objInd, 1, GR_selTyp, GR_selDbi);

    //----------------------------------------------------------------
    } else if(GR_Sel_Filter == 15) {                    // load texture
      i1 = GA_Tex__ (objInd, 2, GR_selTyp, GR_selDbi);
      if(i1 < 0) return -1;
      return UI_Tex__ (NULL, GUI_SETDAT_EI (TYP_EventPress,UI_FuncUCB9));
      

    //----------------------------------------------------------------
    } else if(GR_Sel_Filter == 16) {                    // modify position
      return UI_Tra__ (objInd, GR_selTyp, GR_selDbi);

    //----------------------------------------------------------------
    } else if(GR_Sel_Filter == 17) {                    // Form active
      return PRG_sel_CB (objInd, GR_selTyp, GR_selDbi, GR_selNam);

    //----------------------------------------------------------------
    }
  }



  L_ck_func:

  // UserFunction active ??  Work ..
  if(UI_UserSelFunc != NULL) {
    // keine Selections an Userprogs bei hide, view, Wcut.
    if((UI_stat_hide  != 0)&&
       (UI_stat_view  != 0)&&
       (UI_InpMode != UI_MODE_WCGEO)) {   // war (ckb_wcGeo_stat != 0)) {
      UI_UserSelFunc (GR_Event_Act, objInd);
      goto Fertig;
    }
  }




  // check if hide is active; 0=yes, 1=not
  if(UI_stat_hide != 0) {
    // check if selected obj is connected to interactivity
    i1 = ACT_ck_act (objInd);
    if(i1 == 1) TX_Print(
"**** ERROR: Activity not executed; activate with checkbox Interaction.");
    if(i1 == 0) goto Fertig;
  }


/*
  if(GR_selTyp == Typ_Ditto) {
    TX_Error(" Ditto; cannot annalyze");
    return -1;
  }

  if(GR_selDbi < 0) {
    TX_Error(" dynam. Element; cannot annalyze");
    return -1;
  }
*/


  // make name from typ and DB-index
  if((GR_Sel_Filter == 3)   ||        // Obj2P
     (GR_Sel_Filter == 18))    {      // Obj2PP
    GR_Sel_Filter = 0;                // reset Pointfilter
    goto L_done;
  }

/*
  if(GR_Sel_Filter == 19)      {     // parentObj
    GR_Sel_Filter = 0;                // reset Pointfilter
  }
*/

/*
  // create objName
  if(GR_selTyp != Typ_TmpPT) {   // Point on ConstrPlane already in GR_selNam,
    if(APED_oid_dbo__(GR_selNam, GR_selTyp, GR_selDbi) < 0) {
      AP_search_init ();                      //  ???
      goto Fertig;
    }
  }
*/


  L_done:
  UI_GR_Select_work2 (GR_selTyp, GR_selDbi, GR_selNam, objInd);

  Fertig:
  return 0;

}



//=====================================================================
  int AP_UserSelection_reset () {
//=====================================================================

  // printf("AP_UserSelection_reset\n");

  UI_UserSelFunc = NULL;
  // AP_stat.APP_stat = 0;    // no plugin is active ..

  return 0;

}


//=====================================================================
  int AP_UserSelection_get (int uFunc ()) {
//=====================================================================
/// \code
/// request Mousebutton-callback into function uFunc
///
/// prototyp uFunc:
/// int uFunc (int source, long DispListInd);
///   source: GUI_MouseL|GUI_MouseM|GUI_MouseR|GUI_Mouse2L|
///           GUI_MouseScUp|GUI_MouseScDown
///   DispListInd   dli of selected obj
///
/// get type of selected obj from global var GR_selTyp
/// get cursorposition with UI_GR_get_actPosA (&Point)
/// get DispList from DispListInd with DL_GetAtt (DispListInd)
/// reset with AP_User_reset or AP_UserSelection_reset
/// \endcode


  // printf(" AP_UserSelection_get\n");

  UI_UserSelFunc = uFunc;

  return 0;

}


//=====================================================================
  int AP_UserMousemove_get (int uFunc ()) {
//=====================================================================
/// \code
/// request Mousemove-callback into function uFunc
///
/// prototyp uFunc:
/// int uFunc (int dx, int dy);
///
/// reset with AP_UserMousemove_reset
/// \endcode
// see UI_GR_get_actPosA AP_Mousemove2dx


  // printf(" AP_UserSelection_get\n");
  
  UI_UserMousemoveFunc = uFunc;

  return 0;
  
} 


//=====================================================================
  int AP_UserMousemove_reset () {
//=====================================================================
// reset user-mouseMovementCallback

  // printf("AP_UserMousemove_reset\n");

  UI_UserMousemoveFunc = NULL;

  return 0;

}


/* unused
//================================================================
  int UI_GR_Select_work_vc (long dbi) {
//================================================================
// UI_GR_Select_work1 for vectors

  long   dli;


  printf("UI_GR_Select_work_vc %ld\n",dbi);

  if(UI_UserSelFunc != NULL) {

    dli = DL_SetTmpObj (Typ_VC, dbi);   // save a dummy-record

    GR_selTyp = Typ_VC;
    GR_selDbi = dbi;
    APED_oid_dbo__ (GR_selNam, GR_selTyp, GR_selDbi);

    UI_UserSelFunc (GUI_MouseL, dli);

  }
  

  return 0;

}
*/

  
//================================================================
  int UI_GR_Select_work2 (int typ, long dbi, char* buf, long dli) {
//================================================================
// work;
//   CAD: provide input -> IE_sel_CB__



  int      i1, imode;
  char     sf[16], s1[80];
  ModelRef *mrRec;
  ModelBas *mbRec;


  // printf("UI_GR_Select_work2 %d %ld |%s| %ld\n",typ,dbi,buf,dli);


  // hier kommen auch noch Typ_Activ; in UI_GR_Select_work1 nicht mehr !
  if(typ == Typ_Activ) {
    if(GR_Sel_Filter == 4) {       // dump obj
      GR_Sel_Filter = 0;           // reset
      UI_dump_obj (typ, dbi);
      goto Fertig;
    }
  }


  //----------------- hide ---------------------------------------------
  if(UI_stat_hide == 0) {
    TX_Print ("hide obj %s",buf);
    // printf(" hide %d %ld\n",typ,dbi);
    // DL_hili_off (dli);  // sets GR_ObjTab[].disp to ON = view !
    // DL_hide__ (dli, OFF);  // setzt nur das DispListFeld
    // DL_Redraw ();
    // add to Hide-List
    // GA_hide__ (3, dli, 0);
    GA_view__ (dli, 1, 0, 0L);
    goto Fertig;
  }



  //----------------- S/M ---------------------------------------------
  // if(UI_InpSM == ON)       {      // S/M fuer MAN u CAD
    // AP_SMD_init1 (buf);
    // // AP_search_obj (buf);          // suchen im Text und hiliten
    // // AP_obj_analyze (typ, dbi);    // display Koords
    // // IE_activate();                // CAD activate
    // goto Fertig;
  // }



  // // UserFunction active ?? Hier nur indicate; dbi dabei 0 !!
  // } else if(UI_UserSelFunc != NULL) {
    // UI_UserSelFunc (GUI_MouseL, typ, dbi);
    // goto Fertig;



  // printf(" sel. %d %ld |%s| %ld\n",typ,dbi,buf,dli);
  imode = UI_InpMode;


  //----------------- VWR ---------------------------------------------
  if(imode == UI_MODE_VWR) {
    goto L_addGrp;          // add obj to group



  //----------------- CAD ---------------------------------------------
  // normaler Konstruktionsvorgang (kein search) - Meldung ans Userprog. 
  // } else if(ckb_cad_stat == 0) {  // CAD-Menu aktiv=0
  } else if (imode == UI_MODE_CAD) {
      // printf(" cad-typ=%d dbi=%ld\n",typ,dbi);


    // test if CAD-Function is active
    if(IE_get_Func() < 0) {
      // no CAD-Function is active
      if(typ == Typ_TmpPT) {
        // indicate !
        DL_Redraw ();           // else grid is missing ..
        MSG_pri_0 ("CAD_On");   // Message: select CAD-function !
        goto Fertig;
      }
      goto L_addGrp;          // add obj to group
    }

    // provide selection -> cad-input
    // if(GR_Event_Act == GUI_Mouse2L) { // Doppelklick; Modify.
    IE_sel_CB__ (typ, dbi, buf); 
    goto Fertig;





  //----------------- MAN ---------------------------------------------
  } else if (imode == UI_MODE_MAN) {

    // test if cursor is inside line or at startposition in line
    if(ED_ck_lnStart() == 0) {
      // yes, cursor is at first position in line
      goto L_addGrp;          // add obj to group


    } else {       // cursor is inside line

      if(typ == Typ_TmpPT) UI_disp_Pos (Typ_PT, &GR_CurUk);

      // add nur wenn akt. Line zB "L25=" ist ...
      // i1 = ED_query_CmdMode ();
      // if(i1 == 0) {  // 0=Defline
        ED_add_Text (typ, dbi, buf);
        // update Window ..
        // ED_update (0L);   // 2013-04-11 weg
  
        // es koennte ein Insert sein; LineNr ?
      i1 = ED_get_lnr_act ();  // sets also ED_lnr_act
      // disp LineNr
      UI_AP (UI_FuncSet, UID_ouf_lNr, PTR_INT(i1));
    }

    goto Fertig;



/*
  //----------------- NC ----------------------------------------------
  // } else if(ckb_nc_stat == 0) {    // NC-Menu aktiv=0
  } else if (UI_InpMode == UI_MODE_NC) {

    // Check FROM-Checkbox
    if((UI_GR_FROM(buf)) == 1) goto Fertig;

    // Check RAPID-Checkbox
    if((UI_GR_RAPID(buf)) == 1) goto Fertig;

    ED_add_Text (typ, dbi, buf);



  //----------------- WC_GEO ----------------------------------------------
  // } else if(ckb_wcGeo_stat == 0) {    // NC-Menu aktiv=0
  } else if (UI_InpMode == UI_MODE_WCGEO) {
    UI_wcg_sel1 (typ, dbi, buf);



  //----------------- WC_TEC ----------------------------------------------
  // } else if(ckb_wcTec_stat == 0) {    // NC-Menu aktiv=0
  } else if (UI_InpMode == UI_MODE_WCTEC) {
    IE_wcTec_sel1 (typ, dbi, buf);
*/


  //-------------------------------------------------------------------
  } else {
    TX_Print("UI_GR_Select_work2 E001");
    goto Fertig;
  }



  L_addGrp:
    // skip indicates ..
    if(typ == Typ_TmpPT) {
      TX_Print(" .. ?");
      DL_Redraw ();
      goto Fertig;
    }

    // test if obj is already in group
    if(Grp_ck_def(typ, dbi) >= 0) goto L_remGrp;

    // add obj to group ..
    DL_grp1__ (dli, NULL, 1, 0);         // add obj to group
    strcpy(sf, "group-add ");
    goto L_print;


  L_remGrp:
    // remove obj from group ..
    DL_grp1__ (dli, NULL, -1, 0);
    strcpy(sf, "group-remove ");


  L_print:
    // ist es Model: display SubmodelID
    if(typ == Typ_Model) {
      // ModelReference
      mrRec = DB_get_ModRef (dbi); 
      // UT3D_stru_dump (typ, mrRec, "M%d:",ind);
      // den BasisModelRecord holen
      mbRec = DB_get_ModBas (mrRec->modNr);
      sprintf(s1, "%sf %s (Model %s)",sf,buf,mbRec->mnam);

    } else {
      sprintf(s1, "%s obj %s",sf,buf);
    }
    // TX_Print("Unhilite: right mouseb.");
    // TX_Print("Esc = clear group");
    strcat(s1, "; Esc = clear group");
    TX_Print(s1);



  Fertig:
    // printf("ex UI_GR_Select_work2 typ=%d dli=%ld dbi=%ld |%s|\n",
           // typ,dbi,dli,buf);
  return 0;


  Err_1:
   // TX_Error(" ????");
   TX_Print("- objTyp/Index not defined - (key ctrl p)");
   return -1;

}


/*
//================================================================
  int UI_GR_FROM (char* buf) {
//================================================================

  int    ickb;
  char   buf1[80];


  // printf("UI_GR_FROM %s\n",buf);


  // Check FROM-Checkbox
  // ickb= ckb->GetCheck();
  ickb= UI_NC_AP (UI_FuncGet, UID_nc_ckb_from, NULL);
 
  if(ickb== 0) return 0;




  sprintf(buf1, "FROM %s",buf);


  // FROM - Button zuruecksetzen
  // ckb->SetCheck(0);
  UI_NC_AP (UI_FuncSet, UID_nc_ckb_from, NULL);



  ED_add_Text (Typ_String, 0L, buf1);

  return 1;

}


//================================================================
  int UI_GR_RAPID (char* buf) {
//================================================================

  int    ickb;
  char   buf1[80];


  // printf("UI_GR_FROM %s\n",buf);


  // Check FROM-Checkbox
  // ickb = ckb->GetCheck();
  ickb = UI_NC_AP (UI_FuncGet, UID_nc_ckb_rapid, NULL);

  if(ickb == 0) return 0;

  sprintf(buf1, "RP %s",buf);

  // FROM - Button zuruecksetzen
  // ckb->SetCheck(0);
  UI_NC_AP (UI_FuncSet, UID_nc_ckb_rapid, NULL);

  ED_add_Text (Typ_String, 0L, buf1);

  return 1;

}
*/


/*=====================================================================*/
  int UI_KeyFieldWri () {
/*==================
Copy ltxt in die Panel-Ausgabezeile (darstellen)
Jeden einzelnen Char !
*/


  /* printf("CMD_Write /%s/\n", ltxt); */

  /* strcpy (CMD_Line, ltxt); */

  //gtk_label_set_text ((GtkLabel*) (w_CMD_Line), CMD_Line);

  return 0;
}


//================================================================
  int UI_GR_selMen_init (int mode) {
//================================================================
// create popup-menu for mousebutton-middle
// mode: 0=MAN,M1,empty line;   optLst0
//       1=MAN,M3,not empty;    optLst1
//       2=CAD

// see OMN_popup_Brw OMN_selMen_MAN_M3_empty UI_mcl_CBL

  int   inpTyp;
  char  **p1;
  char  *optLst0[]={"P (point)",
                    "L (line)",
                    "C (circle)",
                    "S (polygon)",
                    "S (ellipse)",
                    "S (spline)",
                    "S (contour)",
                    "A (surface)",
                    "D (vector)",
                    "V (value)",
                    "\0"};

  char  *optLst1[]={"OK       (Ctrl-r.Mb.)",
                    "point on object",
                    "point on constr.Plane",
                    "line from polygon/contour",
                    "\0"};

  char  *optLst2[]={"list of internal subModels",     "\0"};
  char  *optLst3[]={"list of modelfiles",             "\0"};
  char  *optLst4[]={"list of catalogparts",           "\0"};
  char  *optLst5[]={"OK",
                    "next",
                    "previous",
                    "\0"};


  inpTyp = IE_get_inp_TypAct();

  // printf("UI_GR_selMen_init %d %d\n",mode,inpTyp);


  if(mode == 0) {  // MAN,empty
    p1 = optLst0;
    GR_optMenNr = 0;

  } else if(mode == 1) {  // MAN, defLn
    p1 = optLst1;
    GR_optMenNr = 1;


  } else if(mode == 2) {  // CAD
    if     (inpTyp == Typ_mod1)     {p1 = optLst5; GR_optMenNr = 3;}
    else if(inpTyp == Typ_SubModel) {p1 = optLst2; GR_optMenNr = 2;}
    else if(inpTyp == TYP_FilNam)   {p1 = optLst3; GR_optMenNr = 2;}
    else if(inpTyp == Typ_CtlgPart) {p1 = optLst4; GR_optMenNr = 2;}
    else                            {p1 = optLst1; GR_optMenNr = 1;}
  }

  GUI_popup__ (p1, NULL, 0, UI_GR_selMen_cbSel, NULL);
  UI_GR_ButtonM1Release ();   // else KeyM1=ON ! 2013-05-01


  return 0;

}


//================================================================
  int  UI_GR_selMen_cbSel (MemObj *mo, void **data) {
//================================================================
// callback of popup-menu UI_GR_selMen_init


  int   iEv, isel, iTyp;
  long  ind;
  char  s1[64], s2[16];


  iEv = GUI_DATA_EVENT;

  if(iEv != TYP_EventPress) return 0;

  isel = GUI_DATA_I1;
    // printf("UI_GR_selMen_cbSel %d\n",isel);



  //----------------------------------------------------------------
  if(GR_optMenNr != 0) goto L_pt;
  strcpy(s2,"=");
  switch (isel) {
    case 0:  // P
      iTyp = Typ_PT;
      break;
    case 1:  // L
      iTyp = Typ_LN;
      break;
    case 2:  // C
      iTyp = Typ_CI;
      break;
    case 3:  // S=POL
      iTyp = Typ_CV;       strcpy(s2,"=POL");
      break;
    case 4:  // S=ELL
      iTyp = Typ_CV;       strcpy(s2,"=ELL");
      break;
    case 5:  // S=BSP
      iTyp = Typ_CV;       strcpy(s2,"=BSP");
      break;
    case 6:  // S=CCV
      iTyp = Typ_CV;       strcpy(s2,"=CCV");
      break;
    case 7:  // A
      iTyp = Typ_SUR;
      break;
    case 8:  // D
      iTyp = Typ_VC;
      break;
    case 9:  // V
      iTyp = Typ_VAR;
      break;
  }

  ind = DB_QueryNxtFree (iTyp, 20);
  APED_oid_dbo__(s1, iTyp, ind);
  strcat(s1,s2);
  UI_AP (UI_FuncSet, UID_Edit_Line, (void*)s1);
  goto L_exit;


  //----------------------------------------------------------------
  L_pt:
  if(GR_optMenNr != 1) goto L_m2;
  switch (isel) {
    case 0:
      UI_EdKeyCR (2);           // OK
      break;
    case 1:
      UI_GR_Sel_Filter (18);    // give parametric-point from curpos on sel obj
      break;
    case 2:
      UI_GR_Sel_Filter (1);     // give point from curpos on constrPlane
      break;
    case 3:
      UI_GR_Sel_Filter (19);     // give line from obj (polygon/contour)
      break;
  }

  goto L_exit;



  //----------------------------------------------------------------
  L_m2:  // list of files
    if(GR_optMenNr != 2) goto L_m3;
    IE_cad_selM2 (0);
    goto L_exit;



  //----------------------------------------------------------------
  L_m3:  // MOD()
    if(GR_optMenNr != 3) goto L_exit;
  switch (isel) {
    case 0:
      UI_EdKeyCR (2);              // OK
      break;
    case 1:
      IE_cad_Inp1_nxtMod (1,  1);  // nxt
      break;
    case 2:
      IE_cad_Inp1_nxtMod (1, -1);  // prev
      break;
  }
    goto L_exit;




  //----------------------------------------------------------------
  L_exit:
// // only MS-Win: no redraw od menu-area
// // does not help ..
// #ifdef _MSC_VER
  // GL_Redraw();
    // printf(" ms-redraw ..\n");
// 
// #endif
// 
    return 0;

}


/* ----------------------------- eof ----------------------------- */
