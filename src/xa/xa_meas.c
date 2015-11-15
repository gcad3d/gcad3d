// xa_meas.c                                      2010-04-27  RF
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
2010-04-10 neu erstellt. RF.

-----------------------------------------------------
*/
#ifdef globTag
 void Meas(){}
#endif
/*!
\file  ../xa/xa_meas.c
\brief measure 
\code
=====================================================
List_functions_start:

Meas_init      main-entrypoint
Meas_exit__
Meas_win__     create Toolbar
Meas_key_CB
Meas_sel_CB
Meas_ent_CB
Meas_work_pp

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


*/

#ifdef _MSC_VER
#include "MS_Def0.h"
#endif


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>             // va_list
#include <string.h>

#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_cast.h"             // INT_PTR

#include "../ci/NC_apt.h"              // T_PRJ

#include "../gui/gui__.h"              // Gtk3

#include "../gr/ut_UI.h"               // UI_FuncInit
#include "../gr/ut_GL.h"               // GL_get_Scale

#include "../xa/xa_msg.h"              // MSG_..
#include "../xa/xa_ui.h"               // APF_TB_CAD,
#include "../xa/xa_sele.h"             // Typ_goGeo1,
#include "../xa/xa_mem.h"              // memspc51-55


#define   TRUE 1
#define   FALSE 0



//================================================================
// ex ../xa/xa_ui.c
extern MemObj  UIw_Box_TB;    // toolbarBox

// from ../xa/xa.c:
extern Plane     WC_sur_act;            // die aktive Plane


//================================================================
// LOCAL:
static MemObj Meas_tb=GUI_OBJ_NEW;   // Toolbar
static MemObj Meas_e1, Meas_e2;
static int    Meas_func=0;           // 0=PT/PT: 1=PT/OBJ; 2=OBJ/OBJ
static int    Meas_ie=0;             // 0=e1 is active; 1=e2 is active.
static int    Meas__obj_stat;        // old APT_obj_stat (before, after)


//================================================================
// PROTOS:
  int Meas_sel_CB (int src, long dli);
  int Meas_key_CB (int key);
  int Meas_ent_CB (MemObj *mo, void **data);
  int Meas_opt_CB (MemObj *mo, void **data);
  int Meas_win__ (MemObj *parent, void **data);






//================================================================
  int Meas_init () {
//================================================================

  // lock some application-functions...
  UI_func_stat_set__ (-APF_TB_CAD,
                      -APF_MEN_FIL,
                      -APF_MEN_SM,
                      -APF_MEN_MDF,
                      -APF_MEN_APP,
                      -APF_WIN_B_E,
                      -APF_HIDE,
                      -APF_VWCADMAN,
                      -APF_BUT_GSE,
                      -APF_PLUGINS,
                      -APF_UNDO,
                      0);

  AP_UserKeyIn_get     (Meas_key_CB);         // attach keystrokes
  AP_UserSelection_get (Meas_sel_CB);         // attach grafic selections

  sele_save ();  // 
  sele_set__ (Typ_PT);                          // get points from selections
  // sele_reset_type (Typ_Model);


  // init toolbar
  if(GUI_OBJ_IS_VALID(&Meas_tb)) {
    GUI_set_show (&Meas_tb, 1);
    // clear inputfields
    GUI_entry_set (&Meas_e1, "");
    GUI_entry_set (&Meas_e2, "");

  } else {
    Meas_win__ (&UIw_Box_TB, GUI_SETDAT_EI(TYP_EventPress,UI_FuncInit));
  }


  Meas__obj_stat = WC_get_obj_stat ();
  WC_set_obj_stat (1);


  // die erste Selektion geht sonst ned (das Bild wird kleiner, weil unten
  //   der Toolbar erzeugt wird !))
  // DL_Redraw();
  Grp_Clear (1);

  return 0;

}


//================================================================
  int Meas_exit__ () {
//================================================================

  printf("Meas_exit__ \n");


  DL_hili_off (-1L);             //  -1 = unhilite all


  // GUI_obj_hide (Meas_tb);           // remove Toolbar
  GUI_set_show (&Meas_tb, 0);           // remove Toolbar


  // restore some application-functions...
  UI_func_stat_set__ (APF_TB_CAD,
                      APF_MEN_FIL,
                      APF_MEN_SM,
                      APF_MEN_MDF,
                      APF_MEN_APP,
                      APF_WIN_B_E,
                      APF_HIDE,
                      APF_VWCADMAN,
                      APF_BUT_GSE,
                      APF_PLUGINS,
                      APF_UNDO,
                      0);

  AP_User_reset ();

  GL_temp_delete (); // alle temp. obj loeschen ..

  // uncheck checkbox "Measure"
  UI_ckb_meas (FALSE);

  AP_errStat_reset (0);
  WC_set_obj_stat (Meas__obj_stat);

  sele_restore ();

  return 0;

}


//================================================================
  int Meas_win__ (MemObj *parent, void **data) {
//================================================================
// create Toolbar

  int     ifunc;
  MemObj  box0, box1, box2, wh;
  char    *optLst[]={"Point/Point   (distance)",             // 0
                     "Point/Object  (normal distance)",      // 1
                     "Line          (angle, length)",        // 2
                     NULL};



  ifunc = GUI_DATA_I1;


  // printf("Meas_win__ %d\n",ifunc);


  switch (ifunc) {

    //---------------------------------------------------------
    case UI_FuncInit:  // 90
      if(GUI_OBJ_IS_VALID(&Meas_tb)) goto L_exit;

      // Meas_tb = GUI_toolbar__ (parent);
      Meas_tb = GUI_toolbox__ (parent);

        // GUI_obj_dump_mo (&Meas_tb);
      box0 = GUI_box_h (&Meas_tb, "e");
        // GUI_obj_dump_mo (&box0);
      // box1 = GUI_Vbox (box0, 0);
      GUI_label_htm__ (&box0, "<b> Measure: </b>", "");

      GUI_optmen__ (&box0, optLst[0], optLst, NULL, Meas_opt_CB, "");


      Meas_e1 = GUI_entry__ (&box0, "", "", Meas_ent_CB, "e1", "20e");   //"20e"
      Meas_e2 = GUI_entry__ (&box0, "", "", Meas_ent_CB, "e2", "20e");
      GUI_button__ (&box0, " Help ", Meas_ent_CB, NULL, "");
      GUI_button__ (&box0, MSG_const__(MSG_exit),
                         Meas_win__, &GUI_FuncKill, "");
      // GUI_set_enable (&wh, 1);

      // GUI_obj_focus (&Meas_e1);
      // Meas_upd_e ();

      L_exit:
      // GUI_Win_go (&Meas_tb);
      GUI_set_show (&Meas_tb, 1);  // show

      
      MSG_pri_0 ("Meas_in"); // Measure: select 2 points ..
      return (0);


    //---------------------------------------------------------
    case UI_FuncKill:  // 99
      Meas_exit__ ();
      return (-1);
  }

  return (-1);

}


//===================================================================
  int Meas_ent_CB (MemObj *mo, void **data) {
//===================================================================

  printf("Meas_ent_CB GUI_OBJ_TYP=%d\n",GUI_OBJ_TYP(mo));
  printf(" event=%d\n",GUI_DATA_EVENT);


  if(GUI_OBJ_TYP(mo) == TYP_GUI_Button) {
    // Help-Button
    APP_Help ("Measure", "");
    return 0;
  }

    // only for entry-callbacks:
    // printf(" key=%d\n",GUI_DATA_I2);
    // printf(" keyMod=%d\n",GUI_DATA_I2);

  // if(GUI_DATA_EVENT != TYP_EventEnter) {
  // }

  if(GUI_DATA_EVENT == TYP_EventPress) {
    if(GUI_DATA_I2 == GUI_KeyReturn) {
        printf(" .....ENTER ......\n");
      goto L_upd;
    }
  }

  return 0;



  L_upd:
    printf("Meas_ent_CB S1=|%s|\n",GUI_DATA_S1);
  if(!strcmp (GUI_DATA_S1, "e1")) {
    Meas_ie = 0;
  } else {
    Meas_ie = 1;
  }


  Meas_work__ ();
  // Meas_upd_e ();

  // return 0;
  return 1;

}



//===================================================================
  int Meas_opt_CB (MemObj *mo, void **data) {
//===================================================================

  Meas_func = GUI_DATA_I1;

  printf("Meas_opt_CB %d\n",Meas_func);

  Meas_upd_styp ();

  return 0;

}


//================================================================
  int Meas_key_CB (int key) {
//================================================================
// GUI_KeyEsc: clear 

  char      *p1;


  printf("Meas_key_CB %d\n",key);


  switch (key) {


    case GUI_KeyReturn:
    case GUI_KeyEsc:
    case 'q':
    case 's':
      AP_errStat_reset (0);       // clear error
      Meas_work_pp (NULL, NULL);  // clear resulting line
      // test if entry2 is filled; if yes: clear.
      p1 = GUI_entry_get (&Meas_e2);
      if(strlen(p1) > 0) {
        GUI_entry_set (&Meas_e2, "");
        GL_temp_Delete (-3L);     // clear circ 2
        DL_Redraw ();
        Meas_ie = 1;
        Meas_upd_styp ();             // update select-typ
        return TRUE;  // do no defaultOperations
      }

      // test if entry1 is filled; if yes: clear.
      p1 = GUI_entry_get (&Meas_e1);
      if(strlen(p1) > 0) {
        GUI_entry_set (&Meas_e1, "");
        GL_temp_Delete (-2L);     // clear circ 1
        DL_Redraw ();
        Meas_ie = 0;
        Meas_upd_styp ();             // update select-typ
        return TRUE;  // do no defaultOperations
      }

      // exit, do not save ..
      Meas_exit__ ();
      return TRUE;  // do no defaultOperations

    default:
      break;
  }

  return FALSE;  // do defaultOperations

}


//================================================================
  int Meas_sel_ln1 (int typ, long dbi, long dli) {
//================================================================
 
  int      rNr;
  double   dr, dt, dl;
  Vector   vc1;
  void     *vp1;


  printf("Meas_sel_ln1 typ=%d dbi=%ld dli=%ld\n",typ,dbi,dli);

  typ = DB_GetObjDat (&vp1, &rNr, typ, dbi);

  if(typ != Typ_LN) {
    TX_Print ("**** cannot analyze typ %d",typ);
    return -1;
  }

  UT3D_vc_ln (&vc1, vp1);

  dr = UT3D_angr_vcpl_z (&WC_sur_act, &vc1);

  dt = UT3D_angr_vcpl_tilt (&WC_sur_act, &vc1);

  dl = UT3D_len_vc (&vc1);

  TX_Print ("=========== analyze Line L%ld    length = %lf",dbi,dl);
  TX_Print ("Rot.angle  (line - XZ-plane) = %lf deg (%lf rad)",UT_DEGREES(dr),dr);
  TX_Print ("Tilt.angle (line - XY-plane) = %lf deg (%lf rad)",UT_DEGREES(dt),dt);
  TX_Print ("Tilt.angle (line - XY-plane) = %lf deg (%lf rad)",UT_DEGREES(dt),dt);

  return 0;

}


//=========================================================
  int Meas_sel_CB (int src, long dli) {
//=========================================================
// userselection callback
// put selNam into first empty field; entry1 or entry2
//   if none is empty: overwrite entry2.
// if both fields not empty: call Meas_work_pp



  static Point  pt1 = UT3D_pt_NEW;
  static Point  pt2 = UT3D_pt_NEW;


  int    irc, typ;
  long   dbi, dl1;
  char   *p1, *p2, *selNam, s1[256];


  // printf("Meas_sel_CB src=%d dli=%ld\n",src,dli);


  if(src == GUI_MouseR) {
    Meas_exit__ ();
    return 0;
  }


  AP_errStat_reset (0);       // clear error


  //----------------------------------------------------------------
  // get sel.obj -> typ,dbi,selNam
  // indicate: selNam is "ConstrPlane" !
  UI_GR_get_selNam (&typ, &dbi, &selNam);
    // printf("Meas sel typ=%d dbi=%ld nam=|%s|\n",typ,dbi,selNam);



  if(Meas_func == 2) {
    GUI_entry_set (&Meas_e1, selNam);
    return Meas_sel_ln1 (typ, dbi, dli);
  }


  // if(!typ) {
    // TX_Print(".. select object (line or curve ...)");
    // return 0;    // indicate but object wanted
  // }


  // unhilite sel. obj
  DL_hili_off (-1L);



  //----------------------------------------------------------------
  Meas_ie = Meas_ck_active();   // find active inputfield


  // update active inputfield
  if(Meas_ie == 0) {
    GUI_entry_set (&Meas_e1, selNam);
  }

  if(Meas_ie == 1) {
    GUI_entry_set (&Meas_e2, selNam);
  }


  Meas_work__ ();


  Meas_ie = Meas_ck_active();   // find next active inputfield
  Meas_upd_styp ();             // update select-typ


  return 0;

}


//================================================================
  int Meas_normal (Point *pto, Point *pt1, char *sObj) {
//================================================================
// compute pt0 = normal point of pti onto object <obj>

// see IE_analyz_dist or APT_obj_expr


  int       irc, dbTyp, oTyp;
  long      dynPti, dynLni, dynCii, dbi;
  void      *o1;
  int       aus_typ[3];
  double    aus_tab[3];
  ObjGX     ox1;
  Memspc    spcTmp;
  Line      ln1;
  Circ      ci1;


  printf("Meas_normal |%s|\n",sObj);
  UT3D_stru_dump (Typ_PT, pt1, "  pt1: ");


  aus_typ[0] = Typ_cmdNCsub;
  aus_tab[0] = T_PRJ;


  dynPti = DB_dyn__ (0, Typ_PT, 0L);    // save state of dyn-points
  dynLni = DB_dyn__ (0, Typ_LN, 0L);    // save state of dyn-lines
  dynCii = DB_dyn__ (0, Typ_CI, 0L);    // save state of dyn-circs
  dbi = DB_StorePoint (-1L, pt1);       // store dynamic obj in DB.


  aus_typ[1] = Typ_PT;
  aus_tab[1] = dbi;


  // irc = APED_dbo_oid (&dbTyp, &dbi, sObj);
  // APT_obj_expr  (pto, Typ_PT, sObj);
    // UT3D_stru_dump (Typ_PT, pto, "  pto: ");
// return 0;


  irc = APED_dbo_oid (&dbTyp, &dbi, sObj);
  if(irc < 0) {
      printf(" typ=%d\n",dbTyp);
    if(dbTyp == Typ_LN) {
      APT_obj_expr  (&ln1, Typ_LN, sObj);
      dbi = DB_StoreLine (-1L, &ln1);
    } else if (dbTyp == Typ_CI) {
      APT_obj_expr  (&ci1, Typ_CI, sObj);
      dbi = DB_StoreCirc (-1L, &ci1);
    } else {
      TX_Print("cannot use obj 2 |%s|",sObj);
      return -1;
    }
  }
    // printf(" dbTyp=%d dbi=%ld\n",dbTyp,dbi);

  aus_typ[2] = dbTyp;
  aus_tab[2] = dbi;

/*
  // get datastructs from typ, DB-index; for curves we need the subtyp
  oTyp = DB_GetObjDat (&o1, dbTyp, dbi);
    printf(" oTyp=%d\n",oTyp);
    UT3D_stru_dump (oTyp, o1, "o1");
*/

  UME_init (&spcTmp, memspc501, sizeof(memspc501));



  irc = APT_prj_obj_perp (&ox1, 3, aus_typ, aus_tab, &spcTmp);
  if(irc != Typ_PT) {
    TX_Print("cannot project .. |%s|",sObj);
    irc = -1;
    goto L_exit;
  }
    // printf(" irc=%d\n",irc);

  *pto = *((Point*)&ox1);
  irc = 0;


  L_exit:
  DB_dyn__ (2, Typ_PT, dynPti);      // reset state of dyn-points
  DB_dyn__ (2, Typ_LN, dynLni);      // reset state of dyn-Lines
  DB_dyn__ (2, Typ_CI, dynCii);      // reset state of dyn-LinesR

  return irc;

}


//================================================================
  int Meas_ck_active () {
//================================================================
// RetCod: 0=Field_1;   1=Field_2

  int    ii;
  char   *p1; // *p2, s1[256];

  p1 = GUI_entry_get (&Meas_e1);

  if(strlen(p1) < 1) return 0;

  
  // p2 = GUI_entry_get (&Meas_e2);

  return 1;

}


//================================================================
  int Meas_work__ () {
//================================================================
 
  static Point  pt1;
  static Point  pt2;

  long   dl1;
  char   *p1, *p2, s1[256];


  printf("Meas_work__ \n");

  p1 = GUI_entry_get (&Meas_e1);
  p2 = GUI_entry_get (&Meas_e2);


  //----------------------------------------------------------------
  // e1=active: update pt1
  if(Meas_ie != 0) goto L_upd1;

    APT_obj_expr (&pt1, Typ_PT, p1);
    UT3D_stru_dump (Typ_PT, &pt1, "spt1: ");
    dl1 = -2L;
    GL_DrawSymB (&dl1, 2, SYM_CIR_S, &pt1); // red circ
    sprintf(s1, "%s = %f %f %f",p1,pt1.x,pt1.y,pt1.z);
    TX_Print(s1);
    if(strlen(p2) > 0) {
      if(Meas_func == 1) {
        Meas_normal (&pt2, &pt1, p2);
      }
    }
    goto L_disp;


  //----------------------------------------------------------------
  L_upd1:
  // e2=active: update pt2

      if(Meas_func == 0) {
        APT_obj_expr (&pt2, Typ_PT, p2);
      } else {
        Meas_normal (&pt2, &pt1, p2);
      }
        // UT3D_stru_dump (Typ_PT, &pt2, "spt2: ");
      dl1 = -3L;
      GL_DrawSymB (&dl1, 2, SYM_CIR_S, &pt2); // red circ
      sprintf(s1, "%s = %f %f %f",p2,pt2.x,pt2.y,pt2.z);
      TX_Print(s1);



  //----------------------------------------------------------------
  // work if both entries are filled.
  // both entries are filled.
  L_disp:
  if(strlen(p1) < 1) goto L_next;
  if(strlen(p2) < 1) goto L_next;

    // printf(" pt1 = %d\n",UT3D_pt_isFree(&pt1));
    // printf(" pt2 = %d\n",UT3D_pt_isFree(&pt2));

  // if((NOT UT3D_pt_isFree(&pt1))&&(NOT UT3D_pt_isFree(&pt2))) {
    // draw temp. line between pt1-pt2
    Meas_work_pp (&pt1, &pt2);
    DL_Redraw ();
  // }


  //----------------------------------------------------------------
  L_next:
/*
  // activate other inputfield
  if(Meas_ie == 0) {
    Meas_ie = 1;
  } else {
    Meas_ie = 0;
  }
  Meas_upd_e ();
  DL_Redraw ();
*/



  return 0;

}


//================================================================
  int Meas_work_pp (Point *pt1, Point *pt2) {
//================================================================
// both entries are filled; draw temp. line between pt1-pt2
// pt1 NULL: clear line ..

 
  long     dli;
  double   d1, dx, dy, dz;
  Line     ln1;


  // printf("Meas_work_pp\n");

  dli = -4L;

  if(pt1 == NULL) {
    GL_temp_Delete (dli);
    return 0;
  }


  // thick red dashed line 
  ln1.p1 = *pt1;
  ln1.p2 = *pt2;
  // GR_tmpDisp_ln (&ln1, 9);
  GL_DrawPoly (&dli, 9, 2, (Point*)&ln1);


  d1 = UT3D_len_2pt (pt1, pt2);
  // TX_Print("Dist = %f / %s - %s",d1,spt1,spt2);
  TX_Print("    Dist = %f ",d1);
  dx = pt2->x - pt1->x;
  dy = pt2->y - pt1->y;
  dz = pt2->z - pt1->z;
  TX_Print("    DX = %f   DY = %f   DZ = %f",dx,dy,dz);


  return 0;

}


//================================================================
  int Meas_upd_e () {
//================================================================

  // printf("Meas_upd_e %d\n",Meas_ie);


  if(Meas_ie == 0) {        // enable e1
    // GUI_set_enable (&Meas_e2, 0); // make e2 inactive
    // GUI_set_enable (&Meas_e1, 1); // make e1 active
    GUI_entry_styl (&Meas_e1, GUI_STYL_activ);
    GUI_entry_styl (&Meas_e2, GUI_STYL_normal);

  } else {                  // enable e2
    // GUI_set_enable (&Meas_e1, 0); // make e1 inactive
    // GUI_set_enable (&Meas_e2, 1); // make e2 active
    GUI_entry_styl (&Meas_e1, GUI_STYL_normal);
    GUI_entry_styl (&Meas_e2, GUI_STYL_activ);
  }

  Meas_upd_styp ();

  return 0;

}


//================================================================
  int Meas_upd_styp () {
//================================================================
// prepare for next inputfield

  int  styp;

  // printf("Meas_upd_styp ie=%d\n",Meas_ie);

  if(Meas_func == 2) {
    styp = Typ_LN;
    goto L_exit;
  }

  styp = Typ_PT;

  if(Meas_ie == 1) {
    // if(Meas_func != 0) styp = Typ_goGeo1;
    if(Meas_func != 0) styp = Typ_goGeo6;  // P|L|C
    // sele_set__ (Typ_APPOBJ);       // init
    // sele_set_types (Typ_LN, 0);


  }

  L_exit:
  sele_set__ (styp);                          // get points from selections
  // sele_setNoConstrPln ();



  return 0;

}

// EOF
