//        Interaction menu Texture      
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
\file  ../xa/ui_tex.c
\brief userInterface Textures UI_Tex_ 
\code
=====================================================
List_functions_start:

UI_Tex__           window Texture
UI_Tex_Load        textureFile selected ..
UI_Tex_CB1         callback from keyin in texture-InputFileds ..
UI_Tex_Key         modify Scale/Offset/Ang-entries from cursor-keys
UI_Tex_EntUpd      update the entry-fields
UI_Tex_EntIni      init texure-userparameters

List_functions_end:
=====================================================

GA_Tex__              remove texture

Texture-Utility-Functions:    see ../xa/xa_tex.c
Tex_dump__ ..

\endcode *//*----------------------------------------



-------------------------------------------------------
Select-Surface-Callbacks: see GA_Tex__ < UI_GR_Select_work1 < UI_GR_Sel_Filt_set

--------------------------------------------
"load Texture from File":      UI_FuncUCB3
  UI_Tex_Load < GUI_List2
    Tex_addBas__               save & load Texture; set TexBasAct
    UI_Tex__ UI_FuncUCB9       display new textureInfo

--------------------------------------------
select surface - apply active Texture
  GA_Tex__(mode=1) < UI_GR_Select_work1 < UI_GR_Sel_Filt_set(14)


--------------------------------------------
select surface - load Texture
  GA_Tex__(mode=2) < UI_GR_Select_work1 < UI_GR_Sel_Filt_set(15)


--------------------------------------------
select surface - remove Texture
  GA_Tex__(mode=0) < UI_GR_Select_work1 < UI_GR_Sel_Filt_set(13)
    UI_Tex__ UI_FuncUCB9       display new textureInfo
    UI_Tex__ UI_FuncUCB10      display new SurfInfo

--------------------------------------------
Update: update TexRef-UserData; apply Texture.
  UI_Tex_CB1
    UI_Tex__ UI_FuncUCB8    save windowParameters -> TexRef;
    Tex_act_upd

*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../ut/ut_geo.h"         // TexRef
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_txt.h"         // fnam_del
#include "../ut/ut_os.h"               // OS_ ..
#include "../ut/ut_cast.h"             // INT_PTR

#include "../gui/gui__.h"              // Gtk3

#include "../gr/ut_gr.h"               // GR_tDyn_pcv
#include "../xa/xa.h"                  // AP_STAT



// aus xa.c:
extern AP_STAT   AP_stat;
extern char      AP_mod_dir[SIZMFTot], AP_mod_sym[SIZMFSym]; // das Verzeichnis fuer OPEN



// local vars:
static MemObj Tex_ent_sc, Tex_ent_off, Tex_ent_ang;
static int    Tex_actEnt=0;     // actine InputWidget 0=Scale; 1=Offset; 2=RotAng.

// Prototypen





//================================================================
  int UI_Tex_Key (int iKey) {
//================================================================
// Tex_actEnt: 0=Scale; 1=Offset; 2=RotAng

  double    valScl = 0.1;
  long      itr;
  TexRef    *tr;

  // printf("UI_Tex_Key %d\n",iKey);

  itr = Tex_getRefInd ();
  Tex_getRef (&tr, itr);

  // incr offset
  switch (iKey) {

    case GUI_KeyCurLeft:
      if(Tex_actEnt == 0) tr->uscx -= valScl;      // scale
      if(Tex_actEnt == 1) tr->udx += 0.01;         // offset
      if(Tex_actEnt == 2) tr->uar += 1.0;          // rotate
      break;

    case GUI_KeyCurRight:
      if(Tex_actEnt == 0) tr->uscx += valScl;      // scale
      if(Tex_actEnt == 1) tr->udx -= 0.01;         // offset
      if(Tex_actEnt == 2) tr->uar -= 1.0;          // rotate
      break;

    case GUI_KeyCurUp:
      if(Tex_actEnt == 0) tr->uscy -= valScl;      // scale
      if(Tex_actEnt == 1) tr->udy -= 0.01;         // offset
      break;

    case GUI_KeyCurDown:
      if(Tex_actEnt == 0) tr->uscy += valScl;      // scale
      if(Tex_actEnt == 1) tr->udy += 0.01;         // offset
      break;

  }

  // update parameter-display
  UI_Tex_EntUpd (Tex_actEnt, tr);

  // update the active surf
  Tex_act_upd ();

  return 0;

}
 
//================================================================
  int UI_Tex_EntUpd (int mode, TexRef *tr) {
//================================================================
// update the entry-fields;
//   mode  -1=all_fields; 0=scale; 1=Offset; 2=RotAng

  char  cbuf[256];

  printf("UI_Tex_EntUpd %d\n",mode);


      // save -> ent_sc ent_off ent_ang
  if((mode == 0)||(mode < 0)) {
      cbuf[0] = '\0';
      UTX_add_fl_u2(cbuf, tr->uscx,tr->uscy);
      GUI_entry_set (&Tex_ent_sc, cbuf);
  }

  if((mode == 1)||(mode < 0)) {
      cbuf[0] = '\0';
      UTX_add_fl_u2(cbuf, tr->udx,tr->udy);
      GUI_entry_set (&Tex_ent_off, cbuf);
  }


  if((mode == 2)||(mode < 0)) {
      cbuf[0] = '\0';
      UTX_add_fl_u(cbuf, tr->uar);
      GUI_entry_set (&Tex_ent_ang, cbuf);
  }
  
  return 0;

}


//================================================================
  int UI_Tex_EntIni () {
//================================================================
// init texure-userparameters

  TexRef   tr;


  tr.uscx = 1.;
  tr.uscy = 1.;

  tr.udx  = 0.;
  tr.udy  = 0.;

  tr.uar  = 0.;

  // Reset Userparameters
  UI_Tex_EntUpd (-1, &tr);

  return 0;

}


//================================================================
  int UI_Tex_Load (stru_FN *mns1) {
//================================================================
// User has textureFile from FileList selected ..
// Load Texture
// see also TED_Tex_Load

  int       i1, irc;
  char      symNam[320];
  TexRef    *tr;

  if(AP_stat.texture == 0) return 0;

  // UI_Tex__ (NULL, (void*)UI_FuncUCB7);  // redisplay nach hide only ...
//   if(fnam == NULL) return 0;

  // MDLFN_dump_ofn  (mns1, "UI_Tex_Load");

  // get keepFlag from UI_Tex__
  // i1 = UI_Tex__ (NULL, (void*)UI_FuncUCB6);
  // i1 = 1;

  // load Texture
  sprintf(symNam, "%s/%s.%s",mns1->symDir,mns1->fNam,mns1->fTyp);
  i1 = Tex_addBas1 (symNam, 0);
  if(i1 < 0) return -1;

  // display textureName
  // UI_Tex__ (NULL, (void*)UI_FuncUCB9);
  UI_Tex__ (NULL, GUI_SETDAT_EI (TYP_EventPress,UI_FuncUCB9));

  // switch to "apply Texture"
  // UI_Tex__ (NULL, (void*)UI_FuncUCB11);
  UI_Tex__ (NULL, GUI_SETDAT_EI (TYP_EventPress,UI_FuncUCB11));

  // Reset Userparameters
  UI_Tex_EntIni ();

/*
  // wenn "Apply Tex." aktiv ist: Reset Userparameters & Apply.
  if(UI_GR_Sel_Filt_set(-1) != 14) goto L_done;  // 14 = apply texture


  // apply & update texture
  i1 = Tex_act_upd ();
  if(i1 < 0) goto L_done;

  // display selected, loaded texture ..
  // UI_Tex__ (NULL, (void*)UI_FuncUCB7);
  i1 = Tex_getRefInd ();
  Tex_getRef (&tr, i1);
  UI_Tex_EntUpd (-1, tr);
*/

  L_done:
  TX_Print("key Return in inputFields = update surface");
  // restore display after hide
  UI_Tex__ (NULL, GUI_SETDAT_EI (TYP_EventPress,UI_FuncUCB7));

  return 0;

}


//================================================================
  int UI_Tex_CB1 (MemObj *mo, void **data) {
//================================================================
// DoUpdate; callback from keyin in texture-InputFields ..

  int   ii, iEv;


  iEv = GUI_DATA_EVENT;
  printf("UI_Tex_CB1 %d\n",iEv);


  if(iEv == TYP_EventRelease) goto L_KEY_PRESS;    // 9
  // TYP_EventEnter

  // which widget ? 0=Scale; 1=Offset; 2=RotAng.
  // printf(" %d\n",GUI_obj_cmp(mo, &Tex_ent_sc));
  if     (!GUI_obj_cmp(mo, &Tex_ent_sc))  ii = 0;
  else if(!GUI_obj_cmp(mo, &Tex_ent_off)) ii = 1;
  else if(!GUI_obj_cmp(mo, &Tex_ent_ang)) ii = 2;
  else TX_Print("***** UI_Tex_CB1 E1");
    printf(" activate ii=%d\n",ii);


  if(ii == Tex_actEnt) return 0; //FALSE;  // continue with defaultHandler

  // entering new Entry
  Tex_actEnt = ii;
    // printf(" enter Entry %d\n",Tex_actEnt);

  return 0; //FALSE;



  //----------------------------------------------------------------
  // KEY_PRESS-events:
  L_KEY_PRESS:
  ii = GUI_DATA_I2;
    printf(" key=%d\n",ii);

  switch (ii) {

/*
    case GUI_KeyReturn:
    case GUI_KeyTab:

      // printf(" update texture ..\n");
      // UI_Tex__ soll alle Paramter --> actTexRef liefern
      UI_Tex__ (NULL, (void*)UI_FuncUCB8);

      // update the active surf
      Tex_act_upd ();
      break;
*/

    // nur f. MS32 (bei Linux hat Mainwin den Focus !)
    case GUI_KeyCurLeft:
    case GUI_KeyCurRight:
    case GUI_KeyCurUp:
    case GUI_KeyCurDown:
      UI_Tex_Key (ii);
      return 1;//TRUE;  // prevent the default handler from being run


    default:
      // get parameters > actTexRef
      UI_Tex__ (NULL, GUI_SETDAT_EI (TYP_EventPress,UI_FuncUCB8));
      Tex_act_upd ();                        // update the active surf
      return 1;  // prevent the default handler from being run

  }

  return 0;
}


//================================================================
  int UI_Tex__ (MemObj *mo, void **data) {
//================================================================
// Texture

  static MemObj win0, ckb1, ckb2, ckb3, cb1, lb1, lb2;
  static int    actFilt, gli_frame;

  MemObj     box0, box1;

  int        i1, ifunc, ibas, itr;
  long       l1, l2;
  char       *p1, cbuf1[256], cbuf2[256];
  double     d1, d2;
  TexRef     texr, *tr;
  TexBas     *pTexBas;
  Point      po, pa[5];
  stru_FN mns1;


  // ifunc = GUI_DATA_EVENT;
  ifunc = GUI_DATA_I1;
  printf("<<<<<<<<<< UI_Tex__ %d\n",ifunc);


  switch (ifunc) {

    //---------------------------------------------------------
    case UI_FuncInit:  // 90

      if(AP_stat.texture == 0) {   // 2008-10-16 fixed; RF.
        TX_Print("***** OpenGL-binding cannot support textures on this hardware ******");
        return -1;
      }


      // if(win0 != NULL) {           // Win schon vorhanden ?
        // gtk_widget_destroy (win0);
      // }

      gli_frame = 0;

      win0 = GUI_Win__ ("Texture", UI_Tex__, "");

      box0 = GUI_box_v (&win0, "");

      lb2 = GUI_label__ (&box0, "Sur: - no Surface active -", "l");
      lb1 = GUI_label__ (&box0, "Tex: - no texture active -", "l");

      GUI_sep_h (&box0, 0);

      GUI_button__ (&box0, "Load Texture from File",
                UI_Tex__,(void*)&GUI_FuncUCB3, "");

      // GUI_Butt (box0, "Load Texture from Surf",
                // UI_Tex__,(void*)UI_FuncUCB4,0);

      ckb1 = GUI_radiobutt__ (&box0,"activate Texture from Surf", 0,
                              UI_Tex__, (void*)&GUI_FuncUCB4,"");

      ckb2 = GUI_radiobutt__ (&box0,"apply active Texture", 1,
                              UI_Tex__, (void*)&GUI_FuncUCB2,"");

      ckb3 = GUI_radiobutt__ (&box0,"remove Texture", 1,
                              UI_Tex__, (void*)&GUI_FuncUCB1,"");


      // hor sepLine
      GUI_sep_h (&box0, 5);


      // ent_sc = GUI_Entry (box0,"Scale X/Y","1,1",NULL,-120);
      Tex_ent_sc = GUI_entry__ (&box0,"Scale X/Y","1,1",
                                UI_Tex_CB1,NULL,"6");
      GUI_Tip("scale;  2 = half size");

      // ent_off = GUI_Entry (box0,"Offset X/Y","0.,0.",NULL,-120);
      Tex_ent_off = GUI_entry__ (&box0,"Offset X/Y","0.,0.",
                                 UI_Tex_CB1,NULL,"6");
      GUI_Tip("move texture; 0.0 - 1.0");

      // ent_ang = GUI_Entry (box0,"RotAng    ","0",NULL,-120);
      Tex_ent_ang = GUI_entry__ (&box0,"RotAng    ","0",
                                 UI_Tex_CB1,NULL,"6");
      GUI_Tip("rotate texture; angle in degree");

      box1 = GUI_box_h (&box0, "");

      GUI_ckbutt__ (&box1, "fit size", 0, UI_Tex__, (void*)&GUI_FuncUCB13,"");
        GUI_Tip("Groesse anpassen");

      GUI_button__ (&box1, " Reload ", UI_Tex__,(void*)&GUI_FuncUCB12,"");
        GUI_Tip("Aktive Textur aus Datei neu laden");

      box1 = GUI_box_h (&box0, "");

      // GUI_Butt (box1, "  Apply  ", UI_Tex__,(void*)UI_FuncUCB4,0);
      GUI_button__ (&box1, " Help ", UI_Tex__,(void*)&GUI_FuncUCB6, "");
      GUI_button__ (&box1, " Exit ", UI_Tex__,(void*)&GUI_FuncUCB5, "");

      GUI_Win_up (NULL, &win0, 0);  // always on top; muss vor dem show !
      GUI_Win_go (&win0);

      actFilt = UI_GR_Sel_Filt_set (-1);  // query only !

      GUI_ckbutt_set (&ckb2, 1);
      // ruft UI_FuncUCB2 == activiert UI_GR_Sel_Filt_set (14);
      // goto L_lb;

      Tex_actBas_set (-1);  // no tex active ..

      TX_Print(" select texture from BMP-File or textured surf ..");

      // DL_pick_typ (Typ_SUR, 1);    // make all surfaces pickable 2011-02-10
      sele_set__ (Typ_SUR);

      return 0;


    //---------------------------------------------------------
    case UI_FuncUCB1:          // remove Texture
      // skip disactivation
      if (GUI_ckbutt_get(mo) == 0) return 0;
      UI_GR_Sel_Filt_set (13);
      return 0;


    //---------------------------------------------------------
    case UI_FuncUCB2:            // "apply active Texture" activated
      // skip disactivation
      if (GUI_ckbutt_get(mo) == 0) return 0;
      UI_GR_Sel_Filt_set (14);
      return 0;


    //---------------------------------------------------------
    case UI_FuncUCB3:            // Load new Texture from File   1003
      // get textureFile from user
      i1 = MDLFN_ofn_user (&mns1, "select Texturefile", "*.jpg");
      if(i1 < 0) return -1;
      // load tetxure
      UI_Tex_Load (&mns1);
      // weiter mit UI_FuncUCB7 ..
      return 0;


    //---------------------------------------------------------
    case UI_FuncUCB4:    // 3ec  // Load Texture from surf
      // skip disactivation ..
      if (GUI_ckbutt_get(mo) == 0) return 0;
      TX_Print("select surface to load/modify Texture ....");
      UI_GR_Sel_Filt_set (15); 
      // weiter mit UI_GR_Select_work1 UI_FuncUCB9 ..
      return 0;


    //---------------------------------------------------------
    // HELP
    case UI_FuncUCB6:
      APP_Help ("Textures", "");
      return 0;  // 0=not sel, 1=selected.


    //---------------------------------------------------------
    // restore window on-top
    case UI_FuncUCB7:
      // gtk_widget_show (win0);
      // GUI_WinUp (win0);  // always on top
      return 0;


    //---------------------------------------------------------
    // save all UserParamters -> active TexRef
    case UI_FuncUCB8:
      ibas = Tex_actBas_get ();
      if(ibas < 0) {
        TX_Print(" no texture active; use Load-Texture ....");
        return -1;
      }
      // p1 = gtk_entry_get_text ((GtkEntry*) (Tex_ent_sc));
      p1 = GUI_entry_get (&Tex_ent_sc);
      UTX_2db_tx (&d1, &d2, p1);
      if(fabs(d2) < UT_TOL_min1) d2 = d1;
      texr.uscx = d1;
      texr.uscy = d2;
      // p1 = gtk_entry_get_text ((GtkEntry*) (Tex_ent_off));
      p1 = GUI_entry_get (&Tex_ent_off);
      UTX_2db_tx (&d1, &d2, p1);
      // if(fabs(d2) < UT_TOL_min1) d2 = d1;
      texr.udx = d1;
      texr.udy = d2;
      // p1 = gtk_entry_get_text ((GtkEntry*) (Tex_ent_ang));
      p1 = GUI_entry_get (&Tex_ent_ang);
      texr.uar = atof(p1);
      // p1 = gtk_entry_get_text ((GtkEntry*) (ent_vx));
      // p1 = gtk_entry_get_text ((GtkEntry*) (ent_vz));

      // texr.ibas = ibas;
      // texr.vx = UT3D_VECTOR_NUL;
      // texr.vy = UT3D_VECTOR_NUL;

      // copy all userparameters -> act.TexRef
      Tex_TexRef_UpdU (&texr);
      return 0;


    //---------------------------------------------------------
    // display new textureInfo
    case UI_FuncUCB9:
      // disp. act. TexFileName
      ibas = Tex_actBas_get ();
      Tex_getBas__ (&pTexBas, ibas);
      GUI_label_mod (&lb1, pTexBas->fnam);
      return 0;



    //---------------------------------------------------------
    // 1010 - nur act.Surf-Display updaten (user has selected surface)
    case UI_FuncUCB10:
      // get DB-Ind of surf
      i1 = Tex_actSur_get ();
      sprintf(cbuf1, "A%d",i1);
      GUI_label_mod (&lb2, cbuf1);


      itr = Tex_getRefInd ();
      Tex_getRef (&tr, itr);

      po.x = tr->px;
      po.y = tr->py;
      po.z = tr->pz;
  
//       l1 = -2L;  // temporary red frame
      d1 = tr->ssx;
      d2 = tr->ssy;
//       GR_temp_rect1 (&l1, &po, &tr->vx, d1, &tr->vy, d2, 9);
//       gli_frame = GL_temp_iNxt ();
      UT3D_pta_parlg_pt_2vc (pa, &po, &tr->vx, d1, &tr->vy, d2);
      GR_temp_pcv__ (pa, 5, Typ_Att_hili1);

      // l1 = Tex_actDli_get();
        // // printf(" actSur=%d actDli=%ld\n",i1,l1);
      // // unhili geht nicht: wird scheinbar vom select-prozess spaeter gehilitet
      // DL_hili_off (l1);  // unhilite
      DL_Redraw ();

      return 0;


    //---------------------------------------------------------
    // 1011 - switch to "apply Texture"
    case UI_FuncUCB11:
      GUI_ckbutt_set (&ckb2, 1);
      // weiter mit UI_FuncUCB2
      return 0;


    //---------------------------------------------------------
    // 1012 - Reload active Texture
    case UI_FuncUCB12:
      // reload from file
      ibas = Tex_actBas_get ();
      if(ibas < 0) goto L_eb1;
      // get DB-Ind of surf (only for Test)
      i1 = Tex_actSur_get ();
      if(i1 < 1) goto L_es1;
      // reload texure from file
      Tex_getBas__ (&pTexBas, ibas);
      Tex_addBas1 (pTexBas->fnam, 1);
      // update
      Tex_act_upd ();
      return 0;


    //---------------------------------------------------------
    // 1013 - "fit size"
    case UI_FuncUCB13:
      ibas = Tex_actBas_get ();
      if(ibas < 0) goto L_eb1;
      // get DB-Ind of surf (only for Test)
      i1 = Tex_actSur_get ();
      if(i1 < 1) goto L_es1;
      // load active TexRef
      itr = Tex_getRefInd ();
        printf(" RefInd=%d\n",itr);
      Tex_getRef (&tr, itr);
      // switch to ON or OFF ?
      i1 = GUI_ckbutt_get (mo);
        printf(" fit size %d\n",i1);
      if(i1 == 1) {
        // get pixel-size of basic-texture
        Tex_getBas__ (&pTexBas, ibas);
          printf(" imgSiz(pixels) = %d %d\n",pTexBas->xSiz,pTexBas->ySiz);
        // get size of surface in userCoords
          printf(" imgSiz(userCo) = %f %f\n",tr->ssx,tr->ssy);
        d1 = UTP_db_rnd2sig (pTexBas->xSiz * tr->ssx);
        d2 = UTP_db_rnd2sig (pTexBas->ySiz * tr->ssy);
        tr->uscx = d1;
        tr->uscy = d2;
      } else {
        tr->uscx = 1.;
        tr->uscy = 1.;
      }
      // update parameter-display
      UI_Tex_EntUpd (Tex_actEnt, tr);
      // update the active surf
      Tex_act_upd ();
      return 0;


    //---------------------------------------------------------
    // 100 = OK-Button pressed, activeTexRef permanent speichern
    case UI_FuncOK:


    //---------------------------------------------------------
    // Cancel/Exit
    case UI_FuncUCB5:
      // send destroy-signal
      // gtk_signal_emit_by_name (GTK_OBJECT(win0),"destroy");  2011-02-10
      // return 0;


    //---------------------------------------------------------
    // destroy-Signal
    case UI_FuncKill:
      UI_GR_Sel_Filt_set (actFilt);  // reset

//       GL_temp_del_1 (2L);  // delete red frame
      if(gli_frame) GL_temp_del_1 (gli_frame);  // delete red frame
      gli_frame = 0;

      // EXIT
      GUI_Win_kill (&win0);
      DL_hili_off (-1L); DL_Redraw (); // unhilite alle Objekte
      return 0;

  }


  // L_cb:
  // gtk_widget_set_sensitive (cb1, i1); // TRUE od FALSE
  return 0;


  //----------------------------------------------------------------
  L_eb1:
  TX_Print("***** Error: no texture is active; *****");
  return 0;

  L_es1:
  TX_Print("***** Error: no surface is active; *****");
  return 0;


}


// EOF
