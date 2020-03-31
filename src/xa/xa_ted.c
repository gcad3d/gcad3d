// xa_ted.c                                      2010-04-10  RF
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
 void TED(){}
#endif
/*!
\file  ../xa/xa_ted.c
\brief TextureEditor 
\code
=====================================================
List_functions_start:

TED_init       main-entrypoint
TED_exit__
TED_win__      create Toolbar
TED_CB1        callback-dispatch
TED_key_CB
TED_sel_CB
TED_mousemove_CB
TED_frm__      display|remove frame
TED_name       get texturename
TED_Tex_Load   User has textureFile from FileList selected ..
TED_upd__      modify-functions

List_functions_end:
=====================================================
see also xa_ped.c
see also UI_WinTra__       alter modelPositionEditor
\endcode *//*----------------------------------------


UI_GR_Sel_Filt_set - UI_GR_Select_work1
UI_GR_Sel_Filt_set 13  remove texture
UI_GR_Sel_Filt_set 14  apply texture
UI_GR_Sel_Filt_set 15  load texture



*/

#ifdef _MSC_VER
#include "MS_Def0.h"
#endif


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>                    // va_list
#include <string.h>

#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_cast.h"             // INT_PTR

#include "../gui/gui__.h"              // Gtk3

#include "../ut/func_types.h"               // UI_FuncInit
#include "../gr/ut_GL.h"               // GL_get_Scale

#include "../xa/xa.h"                  // AP_STAT
#include "../xa/xa_msg.h"              // MSG_..
#include "../xa/xa_ui.h"               // APF_TB_CAD,


#define   TRUE 1
#define   FALSE 0


//================================================================
// ex ../xa/xa_ui.c
extern MemObj     UIw_Box_TB;    // toolbarBox


// aus xa.c:
extern AP_STAT    AP_stat;
extern char       AP_mod_dir[128], AP_mod_sym[64];



//================================================================
// LOCAL:
static MemObj TEDtb=GUI_OBJ_NEW;    // Toolbar

static int    TED_mode=-1;       // 0=apply, 1=modify, 2=?

static int    actTyp;            // typ of curve to modify
static long   actDbi;            // DB-index
static long   actDli;            // dispListIndex
static void   *actObj;           // data-struct of obj to modify

static TexRef actTr;             // active TexRef (for Restore)



//================================================================
// PROTOS:
  int TED_win__ (MemObj *parent, void *data);
  int TED_CB1 (MemObj *mo, void **data);
  int TED_mousemove_CB (int dx, int dy);
  int TED_key_CB (int key);
  int TED_sel_CB (int src, long dl_ind);
  char* TED_name ();
  int TED_Tex_Load (char *fnam, char *dirNam);







//================================================================
  int TED_init (int typ, long dbi, long dli) {
//================================================================
// main-entrypoint

  int   irc, i1, form, lNr;
  long  l1;
  char  cbuf[160];


  printf("TED_init %d %ld %ld\n",typ,dbi,dli);


  if(AP_stat.texture == 0) {
    GUI_MsgBox ("***** opengl-binding cannot handle textures on this hardware *****");
    return -1;
  }

  // exit if TED is already active
  if(TED_mode >= 0) {
    MSG_pri_0 ("PEDe1"); // ERROR: PED is already active ..");
    return 0;
  }



  // get actObj from DB -------------------------------------
  form = typ;
  irc = UTO_obj_dbo (&actObj, &i1, &form, dbi);
  if(irc < 0) return irc;
    // printf(" form=%d\n",form);
    // DEB_dump_obj__ (form, actObj, "actObj: ");


  actDli = dli;
  actDbi = dbi;
  actTyp = form;



  // lock some application-functions...
  UI_func_stat_set__ (-APF_TB_CAD,
                      -APF_MEN_FIL,
                      -APF_MEN_SM,
                      -APF_MEN_MDF,
                      -APF_MEN_APP,
                      -APF_HIDE,
                      -APF_VWCADMAN,
                      -APF_BUT_GSE,
                      -APF_PLUGINS,
                      -APF_UNDO,
                      0);
  
  AP_UserKeyIn_get     (TED_key_CB);         // attach keystrokes
  AP_UserSelection_get (TED_sel_CB);         // attach grafic selections
  AP_UserMousemove_get (TED_mousemove_CB);   // attach mousemovements


  DL_pick__ (0);               // make all objects unpickable
  DL_pick_typ (Typ_SUR, 1);    // make all surfaces pickable
  GR_set_dispTra (1);          // inhibit display-transformations


  // set surfacDbNr & dliNr
  Tex_setSurfInd (actDbi, actDli);


  // load texture ../xa/xa_tex.c TexBasAct
  irc = GA_Tex__ (actDli, 2, typ, actDbi);
    printf("ex GA_Tex__ %d\n",irc);

  // display red frame (roter Rahmen)
  if(irc >= 0) {
    TED_frm__ (ON);
    TED_mode = 1;

  } else {
    TED_mode = 0;
  }

  TX_Print(
    "- modify texture-position activated - use shift and/or ctrl-key and move mouse"
    );


  // init TED-toolbar
  if(GUI_OBJ_IS_VALID(&TEDtb)) {
    GUI_set_show (&TEDtb, 1);
    // TED_win__ (NULL, (void*)UI_FuncUCB1);
  } else {
    TED_win__ (&UIw_Box_TB, (void*)UI_FuncInit);
  }



  return 0;

}


//================================================================
  int TED_exit__ () {
//================================================================
 
  DL_hili_off (-1L);             //  -1 = unhilite all

  // AP_UserMousemove_reset ();      // reset user-mouseMovementCallback
  AP_User_reset ();               // 

  TED_frm__ (OFF);                // remove frame


  // restore some application-functions...
  // GUI_obj_hide (TEDtb);           // remove Toolbar
  GUI_set_show (&TEDtb, 0);           // remove Toolbar
  UI_func_stat_set__ (APF_TB_CAD,
                      APF_MEN_FIL,
                      APF_MEN_SM,   
                      APF_MEN_MDF,
                      APF_MEN_APP,
                      APF_HIDE,
                      APF_VWCADMAN,
                      APF_BUT_GSE,
                      APF_PLUGINS,
                      APF_UNDO,
                      0);

  DL_pick__ (1);                  // reset selectionfilter
  GR_set_dispTra (0);             // restore display-transformations


  TED_mode = -1;

  return 0;

}


//================================================================
  int TED_win__ (MemObj *parent, void *data) {
//================================================================
// create Toolbar

  static  MemObj lb1 = GUI_OBJ_NEW;
  // static  void *TED_wa[3];

  int        ifunc;
  MemObj  box0, box1, box2;


  ifunc = INT_PTR(data);

  printf("TED_win__ \n");


  switch (ifunc) {

    //---------------------------------------------------------
    case UI_FuncInit:  // 90


  if(GUI_OBJ_IS_VALID(&TEDtb)) {
    GUI_set_show (&TEDtb, 1);
    return 0;
  }


  // TEDtb = GUI_toolbar__ (parent);
  TEDtb = GUI_toolbox__ (parent);

  box0 = GUI_box_h (&TEDtb, "");

  box1 = GUI_box_v (&box0, "");
  GUI_label_htm__ (&box1, "<b> TextureEditor: </b>", "");
  // GUI_HSpace (box1, 10);

  // "no texture active"
  // if(Tex_getRefInd() < 0) {
  if(TED_mode == 0) {
    lb1 = GUI_label__ (&box1, MSG_get_0("TED_no"), "l");
  } else {
    lb1 = GUI_label__ (&box1, TED_name(), "l");
  }

  box1 = GUI_box_v (&box0, "");
  GUI_button__ (&box1, MSG_get_0("TED_ldf"), TED_CB1,    (void*)"LdFile", "");
  MSG_Tip ("TED_ldf");
  GUI_button__ (&box1, MSG_get_0("TED_ldn"), TED_CB1,    (void*)"Reload", "");
  MSG_Tip ("TED_ldn");


  box2 = GUI_box_v (&box0, "");
  GUI_button__ (&box2, MSG_get_0("TED_miy"), TED_CB1, (void*)"MirrY", "");
  GUI_button__ (&box2, MSG_get_0("TED_mix"), TED_CB1, (void*)"MirrX", "");


  box2 = GUI_box_v (&box0, "");
  GUI_button__ (&box2, MSG_get_0("TED_sz1"), TED_CB1, (void*)"Siz1", ""); // reset
  GUI_button__ (&box2, MSG_get_0("TED_sz2"), TED_CB1, (void*)"Siz2", ""); // fit


  box2 = GUI_box_v (&box0, "");
  GUI_button__ (&box2, MSG_get_0("TED_rem"), TED_CB1,    (void*)"Rem", "");
  GUI_button__ (&box2, MSG_const__(MSG_cancel), TED_CB1, (void*)"Can", "");
  MSG_Tip ("TED_can");


  box2 = GUI_box_v (&box0, "");
  GUI_button__ (&box2, MSG_const__(MSG_help), TED_CB1,    (void*)"Help", "");
  GUI_button__ (&box2, MSG_const__(MSG_exit), TED_CB1,    (void*)"Exit", "");
  MSG_Tip ("TED_end");


  L_exit:
  GUI_set_show (&TEDtb, 1);
  TED_mode = 0;
  return (0);


    //---------------------------------------------------------
    // case UI_FuncUCB1:          // reset
      // GUI_CkButt_set (TED_wa[TED_mode], TRUE);
      // break;     

    //---------------------------------------------------------
    case UI_FuncUCB2:          // display new textureInfo
        printf(" new lb1 |%s|\n",TED_name());
      GUI_label_mod (&lb1, TED_name());
      break;     



  }


  return (-1);

}


//=====================================================================
  int TED_CB1 (MemObj *mo, void **data) {
//=====================================================================
/// handle textures

  int  i1, form;
  long l1;
  char *cp1, cbuf1[256], cbuf2[256];

  // cp1 = (char*)data;
  cp1 = GUI_DATA_S1;

  printf("TED_CB1 |%s| %d\n",cp1,TED_mode);

  //-------------------------------------------------
  if(!strcmp(cp1, "Help")) {
    APP_Help ("TextureEditor", "");


  //-------------------------------------------------
  } else if(!strcmp(cp1, "LdFile")) {    // load from file & apply
/*
     AP_get_fnam_symDir (cbuf1);   // get filename of dir.lst
     // sprintf(cbuf1,"%sxa%cdir.lst",OS_get_bas_dir(),fnam_del);
     GUI_List2 ("select Texturefile", // titletext
              AP_mod_dir,             // Pfadname des activeDirectory
              cbuf1,                    // Liste der directories
              (void*)TED_Tex_Load);    // CallBack der Liste
*/

    i1 = AP_Mod_open (1, cbuf1, cbuf2, "select Texturefile", "*.jpg");
    if(i1 < 0) return -1;
    TED_Tex_Load (cbuf1, cbuf2);



  //-------------------------------------------------
  } else if(!strcmp(cp1, "Can")) {    // Cancel
    Tex_TexRef_UpdU (&actTr);
    Tex_act_upd ();
    TED_exit__ ();


  //-------------------------------------------------
  } else if(!strcmp(cp1, "Reset")) {    // reset texture
    TED_upd__ (5, NULL, NULL);


  //-------------------------------------------------
  } else if(!strcmp(cp1, "Reload")) {    // reload from file & apply
    TED_upd__ (7, NULL, NULL);


  //-------------------------------------------------
  } else if(!strcmp(cp1, "MirrY")) {    // mirror vert
    TED_upd__ (0, NULL, NULL);


  //-------------------------------------------------
  } else if(!strcmp(cp1, "MirrX")) {    // mirror hor
    TED_upd__ (1, NULL, NULL);


  //-------------------------------------------------
  } else if(!strcmp(cp1, "Rem")) {    // remove texture
    GA_Tex__ (actDli, 0, actTyp, actDbi);


  //-------------------------------------------------
  } else if(!strcmp(cp1, "Siz1")) {    // fit size
    // on or off?
    i1 = 0;
    TED_upd__ (6, &i1, NULL);


  //-------------------------------------------------
  } else if(!strcmp(cp1, "Siz2")) {    // fit size
    // on or off?
    i1 = 1;
    TED_upd__ (6, &i1, NULL);


  //-------------------------------------------------
  } else if(!strcmp(cp1, "Exit")) {
    TED_exit__ ();


  //-------------------------------------------------
  }
  L_exit:
  return 0;

}


//================================================================
  int TED_key_CB (int key) {
//================================================================

  // printf("PED_key_CB %d\n",key);
  // printf("  PED_stat=%d actInd=%d\n",PED_stat,actInd);


  switch (key) {


    case GUI_KeyReturn:
    case 'q':
    case 's':
      // exit, do not save ..
      TED_exit__ ();
      return TRUE;  // do no defaultOperations

    case GUI_KeyEsc:
      // Reset & Exit.
      TED_CB1 (NULL, GUI_SETDAT_ES(TYP_EventPress,"Can"));
      return TRUE;  // do no defaultOperations

    default:
      break;
  }

  return FALSE;  // do defaultOperations

}


//=========================================================
  int TED_sel_CB (int src, long dli) {
//=========================================================
// userselection callback

  int    irc, styp, typ;
  long   dbi;
  // char   *cp1;
  // Point  pt1;

  printf("TED_sel_CB src=%d ind=%ld\n",src,dli);


  // save &exit m. rechter maustaste (see also PED_key_CB)
  if(src == GUI_MouseR) {
    // if(dli >= 0) return 0;        // skip obj sel with right mousebutt.
    TED_exit__ ();                // exit ..
    return 0;
  }

  // query selected DB-obj
  DBO_sel__ (&styp, &dbi, dli);
    printf(" sel styp=%d dbi=%ld\n",styp,dbi);

  // // unhilite selected surface
  // DL_hili_off (actDli);

  // get texture
  irc = GA_Tex__ (dli, 2, styp, dbi);
  if(irc < 0) return 0;

  // apply texture
  GA_Tex__ (actDli, 1, actTyp, actDbi);

  return 0;

}


//=========================================================
  int TED_mousemove_CB (int dx, int dy) {
//=========================================================
// callback mousemovement
// modifys the active point (actPta[actInd])

  // long      dli;
  // Point     pt2;

extern int       KeyStatShift;
extern int       KeyStatCtrl;


  // printf("TED_mousemove_CB %d %d\n",dx,dy);
  // printf(" shift=%d ctrl=%d\n",KeyStatShift,KeyStatCtrl);

  

  // move texture: modify its udx
  if((KeyStatShift == ON)&&(KeyStatCtrl == ON)) {     // zoom
    TED_upd__ (2, &dx, &dy); 

  } else if(KeyStatShift == ON) { // move
    TED_upd__ (3, &dx, &dy);

  } else if(KeyStatCtrl == ON) { // rotate
    TED_upd__ (4, &dx, &dy);
  }


  return 0;

}


//================================================================
  int TED_frm__ (int mode) {
//================================================================
// display|remove frame

  int        itr;
  long       l1;
  double     d1, d2;
  Point      po;
  TexRef     *tr;

  if(mode == OFF) goto L_del;

  itr = Tex_getRefInd ();
  Tex_getRef (&tr, itr);
    DEB_dump_obj__ (Typ_TEXR, tr, "TR[%d]: ",itr);

  // save active TexRef (for Restore)
  actTr = *tr;


      po.x = tr->px;
      po.y = tr->py;
      po.z = tr->pz;
  
      l1 = -2L;  // temporary red frame
      d1 = tr->ssx;
      d2 = tr->ssy;
      GR_Draw_rect1 (&l1, &po, &tr->vx, d1, &tr->vy, d2, 9);
  
     // l1 = Tex_actDli_get();
        // // printf(" actSur=%d actDli=%ld\n",i1,l1);
      // // unhili geht nicht: wird scheinbar vom select-prozess spaeter gehilitet
      DL_hili_off (l1);  // unhilite
      DL_Redraw ();

  return 0;


  //----------------------------------------------------------------
  L_del:
  GL_temp_del_1 (2L);  // delete red frame


  return 0;

}


//================================================================
  char* TED_name () {
//================================================================
// get texturename

  int    i1;
  TexBas *txb;


  i1 = Tex_actBas_get ();
  if(i1 < 0) return NULL;

  Tex_getBas__ (&txb, i1);
    printf(" iBas=%d texNam=|%s|\n",i1,txb->fnam);
 
  return txb->fnam;

}


//================================================================
  int TED_Tex_Load (char *fnam, char *dirNam) {
//================================================================
// User has textureFile from FileList selected ..
// Load Texture
// RetCod: must be 0 (gtk)

  int       i1, irc;
  TexRef    *tr;

  // UI_Tex__ (NULL, (void*)UI_FuncUCB7);  // redisplay nach hide only ...

  if(fnam == NULL) return 0;


  printf("TED_Tex_Load |%s|%s|\n",fnam,dirNam);

  AP_set_dir_open (dirNam);  // copy dir -> AP_mod_dir; load AP_mod_sym

  // save & load Texture
  irc = Tex_addBas__ (fnam, AP_mod_sym, 0);
  if(irc < 0) return 0;       // MUST return with 0 (gtk-returncode !)


  // display textureName
  TED_win__ (NULL, (void*)UI_FuncUCB2);

  // // Reset Userparameters
  // UI_Tex_EntIni ();

  // apply texture
  GA_Tex__ (actDli, 1, actTyp, actDbi);

  // nur MS:update
  Tex_act_upd ();


  return 0;

}


//================================================================
  int TED_upd__ (int mode, void *val1, void *val2) {
//================================================================
// see UI_Tex__
 
  int      i1, itr, itb;
  float    f1, f2, fs;
  double   d1, d2, d3;
  TexRef   *tr;
  TexBas   *tb;


  printf("TED_upd__ %d\n",mode);



  // get a link to the active texure 
  itr = Tex_getRefInd ();     // get TexRefAct
  i1 = Tex_getRef (&tr, itr);      // get TexRefTab[TexRefAct]
  if(i1 < 0) goto L_eb1;

    // Tex_TexRef_UpdU (&tr);     // update active texture TexRefTab[TexRefAct]


  //================================================================
  if(mode != 0) goto L_mode_1;  // 0=flip vert

    tr->uscx *= -1.f;
    GRU_tex_pos9 (tr);          // update origin
    goto L_update;




  //================================================================
  L_mode_1:
  if(mode != 1) goto L_mode_2;  // 1=flip vert
    tr->uscy *= -1.f;
    GRU_tex_pos9 (tr);          // update origin
    goto L_update;



  //================================================================
  L_mode_2:  // ZOOM
  if(mode != 2) goto L_mode_3;  // 2=zoom texture
    // den Offset korrigieren; ZoomCenter ist der Origin; soll aber die
    // Bildmitte sein; also den offset etwas verschieben.
    // --------- X ----------
    d1 = tr->uscx;                       // d1 = scl-act
    f1 = *((int*)val1);
    tr->uscx -= (f1 / 400.f);
    d2 = tr->uscx;                       // d2 = scl-new
    d3 = (0.5 / d2) * (d2 - d1);
    // d2-d1 = differenz;  (1/d2) * dd = parameterwert (0-1) der differenz
    tr->udx += d3;
      printf(" d1=%f d2=%f d3=%f\n",d1,d2,d3);


    // --------- Y ----------
    d1 = tr->uscy;
    f1 = *((int*)val2);
    // f1 *= tr->uscy;
    tr->uscy -= (f1 / 400.f);
    d2 = tr->uscy;
    d3 = (0.5 / d2) * (d2 - d1);
    // d2-d1 = differenz;  (1/d2) * dd = parameterwert (0-1) der differenz
    tr->udy += d3;
      printf(" d1=%f d2=%f d3=%f\n",d1,d2,d3);
      printf(" new scales %f %f\n",tr->uscx,tr->uscy);
    goto L_update;


  //================================================================
  L_mode_3:   // MOVE
  if(mode != 3) goto L_mode_4;  // 3=move texture
    // fs = GL_get_Scale () / 50.;
      // printf(" Scale fs=%f\n",fs);

    f1 = *((int*)val1);
    tr->udx -= (f1 / 700.f);
    // anpassung scale; sonst verschiebt vergroesseung langsamer !
    // tr->udx -= (f1 / tr->uscx / 500.f);
    // tr->udx -= (f1 / tr->uscx / fs);
    // tr->udx -= (f1 / tr->ssx / fs);

    f1 = *((int*)val2);
    tr->udy -= (f1 / 700.f);
    // tr->udy -= (f1 / tr->uscy / 500.f);
    // tr->udy -= (f1 / tr->uscy / fs);
    // tr->udy -= (f1 / tr->ssy / fs);

    // values 0-1 only
    // if(tr->udx > 1.f) tr->udx -= 1.f;
    // if(tr->udx < 0.f) tr->udx += 1.f;
    // if(tr->udy > 1.f) tr->udy -= 1.f;
    // if(tr->udy < 0.f) tr->udy += 1.f;

    goto L_update;


  //================================================================
  L_mode_4:
  if(mode != 4) goto L_mode_5;  // 4=rotate texture
    f1 = *((int*)val1);
    tr->uar += (f1 / 7.f);
    goto L_update;


  //================================================================
  L_mode_5:
  if(mode != 5) goto L_mode_6;  // 5=reset texture
    Tex_TexRef_InitU (itr);
    goto L_update;



  //================================================================
  L_mode_6:
  if(mode != 6) goto L_mode_7;  // 6=fit size   // reset 

  // Scale = Anzahl von Images pro Surface.
  // Pro 1-m-SurfaceSize 100 Pixels des Image = 1 Pixel pro cm.
  // Scale = (SurfaceSize_in_m * 100 / pixelSize)

    i1 = *((int*)val1);

    if(i1 == 1) {   // fixe Groesse
      itb = Tex_actBas_get ();
      if(itb < 0) goto L_eb1;
      Tex_getBas__ (&tb, itb);  // get pixel-size of basic-texture
        // printf(" imgSiz(pixels) = %d %d\n",tb->xSiz,tb->ySiz);
        // printf("   (userCoords)ssx=%f ssy=%f\n",tr->ssx,tr->ssy);
      // d1 = tr->ssx * 100. / tb->xSiz;
      // tr->uscx = 1. / UTP_db_rnd2sig (d1);
      // d1 = tr->ssy * 100. / tb->ySiz;
      // tr->uscy = 1. / UTP_db_rnd2sig (d1);
      d1 = (double)tb->xSiz / (double)tb->ySiz;
      d2 = (double)tr->ssx / (double)tr->ssy;
      if(d2 > d1) {
        tr->uscx = 1. / d2 * d1; tr->uscy = 1.0;
        // tr->uscx = 0.35;      tr->uscy = 1.0;
      } else {
        tr->uscx = 1.; tr->uscy = 1. * d2 / d1;
        // tr->uscx = 1.0;       tr->uscy = 0.35;
      }
        // printf(" d1=%f d2=%f\n",d1,d2);
        // printf(" uscx=%f uscy=%f\n",tr->uscx,tr->uscy);


    } else {    // einpassen
      Tex_TexRef_InitU (itr);
        // tr->uscx = 1.;
        // tr->uscy = 1.;
    }

    
    goto L_update;




  //================================================================
  L_mode_7:
  if(mode != 7) goto L_mode_8;  // 7 = reload from file
    itb = Tex_actBas_get ();
    if(itb < 0) goto L_eb1;
    Tex_getBas__ (&tb, itb);

    // get DB-Ind of surf (only for Test)
    i1 = Tex_actSur_get ();
    if(i1 < 1) goto L_es1;

    // reload texure from file
    Tex_addBas1 (tb->fnam, 1);

    goto L_update;






  //================================================================
  L_mode_8:
  return 0;




  //================================================================
  L_update:
    Tex_act_upd ();             // update the active surf
    return 0;

  L_eb1:
    TX_Print("***** Error: no texture is active; *****");
    return 0;

  L_es1:
    TX_Print("***** Error: no surface is active; *****");
    return 0;

}


// EOF
