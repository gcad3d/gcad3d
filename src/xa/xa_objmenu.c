// objspecific mouse-menus                           2009-10-26    RF.
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
  - click into empty part of browser: activate menu MSG_CreSubModel (version=2)

-----------------------------------------------------
Modifications:
2009-10-26 Created. RF.

-----------------------------------------------------
*/
#ifdef globTag
void OMN(){}
#endif
/*!
\file  ../xa/xa_objmenu.c
\brief objspecific mouse-menus 
\code
=====================================================
List_functions_start:

OMN_init
OMN_popup_Brw   create & display popup-menu
OMN_CB_popup    callback of popup-menu

OMN_popup_dli   UNUSED activate objMenu from DispListIndex

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


add new commands:
  - add the new commandword into xa_msg.h
  - add the new commandwordtext into msg_const_de.txt & msg_const_en.txt
  - add commandword to menu in OMN_popup_Brw
  - add callback-of-selection-handler in OMN_CB_popup


*/

  
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>                    // va_list
#include <string.h>

#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_txTab.h"            // TxtTab
#include "../ut/ut_gtypes.h"           // AP_src_typ__
#include "../ut/ut_cast.h"             // INT__PTR
#include "../ut/ut_memTab.h"           // MemTab
#include "../ut/os_dll.h"              // DLL_*

#include "../gui/gui__.h"              // Gtk3

#include "../gr/ut_DL.h"               // DL_get_dbi
#include "../db/ut_DB.h"               // 

#include "../xa/xa_mem.h"
#include "../xa/xa_brw.h"              // ICO_Model,
#include "../xa/xa_uid.h"              // UI_MODE_VWR,
#include "../xa/xa_msg.h"              // MSG_activate ..
#include "../xa/xa.h"                  // AP_STAT
#include "../xa/xa_ico.h"              // ICO_PT,
#include "../xa/xa_cad_ui.h"           // IE_FUNC_IS_ACTIVE




//================= local vars: =====================
static int MenNr;                      // nr of the active menu.

// Das active_object wurde vom user mit M1 od M3 im graficWindow
// oder im browserWindow selektiert:
static ObjDB actObj;

// table of all possible strings
       UtxTab_NEW (MenTab);

static int  MenInd[100];  // indices into MenTxt[]
static char *omnTab[100];

static void *OMN_menu1;






//============ Extern Var: =====================
// ../xa/xa.c

// ex ../xa/xa_ui.c:
extern int       UI_InpMode;
extern MemObj    winED;

// ex ../xa/xa_ui.c
extern char      UI_stat_view, UI_stat_hide;

// ex ../xa/xa_cad_ui.c
extern int       IE_modify;



//========= Prototypen: =============================================
  int OMN_CB_popup (MemObj *mo, void** data);
  int OMN_CB_MAN_M3_empty (MemObj *mo, void** data);




//================================================================
  int OMN_init () {
//================================================================
// load all constant-messages -> MenTab
 
  printf("OMN_init \n");
  // DEB_mcheck__();
  // mcheck_pedantic ();
  // mcheck_check_all ();



  UtxTab_init__ (&MenTab);            // init (malloc ..)

  // MSG_get_wTab (&MenTab, MenIa, MenSiz);
  MSG_get_wTab (&MenTab, MSG_SIZE);

  
    // // TESTAUSG:
    // { int i1;
      // for(i1=0;i1<MSG_SIZE;++i1) printf(" %d |%s|\n",i1,UtxTab__(i1,&MenTab));
    // }

  return 0;

}


//================================================================
  int OMN_popup_Br_modVals (int ii, int typ, long dbi) {
//================================================================
// modify values
// user for popup-menu of browser and OpernGl-window.
// see UI_mcl_CB_modVals SRCU_analyz__
      

  int          itsMax, itsAct, iLast, ll, i1, i2;
  long         lNr;
  ObjTXTSRC    *tso;
  char         *s1, *p1, *p2, oid[32];

  // static   char *sa1[]={"X","Y","Z"};


  // printf("OMN_popup_Br_modVals %d typ=%d dbi=%ld\n",ii,typ,dbi);
  // printf(" IE_modify=%d\n",IE_modify);


  if(UI_InpMode == UI_MODE_VWR) goto L_exit;
  if(IE_modify != 0) goto L_exit;

/*
  if(typ == Typ_VAR) {
    MenInd[ii] = 100;    ++ii;    // modify=100; 1=index into tso
    goto L_exit;
  }
*/

  // get definition-line for db-obj typ,dbi;
  APED_oid_dbo__ (oid, typ, dbi);
  iLast = APED_search_defLn (&p1, &lNr, &ll, oid, -1L, 0);
  if(iLast < 0) goto L_exit;
    // printf(" lNr=%ld ll=%d |",lNr,ll);
    // UTX_dump_cnl (p1, 60); printf("|\n");

  // skip def.header
  p2 = strchr (p1, '=');
  if(!p2) {TX_Print("UI_mcl_CB_modVals E001"); goto L_exit;}
  ++p2;  // skip '='
  // terminate code
  p1[ll] = '\0';


  itsMax = SRCU_tsMax (p2);
    // printf(" itsMax=%d siz=%d\n",itsMax,itsMax * sizeof(ObjTXTSRC));
  tso = MEM_alloc_tmp ((int)(itsMax * sizeof(ObjTXTSRC)));

  itsAct = APED_txo_srcLn__ (tso, itsMax, p2);
    // printf(" _txo_srcLn__ %d\n",itsAct);
    // APED_txo_dump (tso, p2);

  // restore code
  if(iLast == 0) p1[ll] = '\n';

  // get list of all value-objects
  SRCU_list_vals__ (tso, p2);

  // make list
  s1 = memspc011;
  p1 = s1 + sizeof(memspc011) - 32; // eos

  i1 = 0;
  for(;;) {
    if(tso[i1].typ == TYP_FuncEnd) break;
      // printf(" val-typ=%d i1=%d\n",tso[i1].typ,i1);
    SRCU_txt_popCoord (s1, tso[i1].typ, tso[i1].ipar, i1);
    MenInd[ii] = 100 + i1;
    omnTab[ii] = s1;
      // printf(" s1-len=%d\n",strlen(s1));
    s1 += strlen(s1) + 1;  // next free spc in s1 ..
    ++ii;
    ++i1;
    if(s1 >= p1 ) break;
    if(ii >= 100) break;
  }


  L_exit:
    // printf("ex OMN_popup_Br_modVals %d\n",ii);

  return ii;
    
}   
    

//================================================================
  int OMN_popup_Br_ed_del (int ii) {
//================================================================
 
  if(UI_InpMode != UI_MODE_VWR) {
    if(IE_modify == 0) {    // 0=Add 1=Modify 2=Insert
      MenInd[ii] = MSG_edit;      ++ii;
      MenInd[ii] = MSG_delete;    ++ii;
    }
  }

  return ii;

}


//================================================================
  int OMN_popup_Brw (int typ, long dbi, long dli, int version) {
//================================================================
// create & display popup-menu.
// Input:
//   version  0=normal;
//            1/Typ_SubModel: change model;
//            2/Typ_SubModel: create new SubModel;
//            3=undo

// static int iTest=0;
  int         i1, i2, ii, iPlg;
  char        s1[32];


  printf("OMN_popup_Brw typ=%d dbi=%ld dli=%ld version=%d\n",typ,dbi,dli,version);
  // printf("  APP_stat=%d\n",AP_stat.APP_stat);
  // ++iTest; if(iTest == 2) AP_debug__ ("OMN_popup_Brw");


  // load MenInd with indices into MenTxt; this is the new menu.
  // Terminate with 0 (index 0).


  ii = 0;

  iPlg = AP_stat.APP_stat;


  // if(UI_UserSelFunc) iPlg = 1;
  // else               iPlg = 0;   // no plugin active

  if(version == 3) {  // deleted obj; undo
    MenInd[ii] = MSG_undo;        ++ii; // help
    goto L_men9;
  }


  //----------------------------------------------------------------
  // build menu ..
  switch (typ) {

    //----------------------------------------------------------------
    case 0:
      // ??
      goto L_exit;


    //----------------------------------------------------------------
    case Typ_TmpPT:   // indicate ..
      // test if group exists;
      if(Grp_get_nr() < 1) {
        // group does not exist: nothing to do; unhilite, exit ..
        return 0;
      }
      // group exists: add "gesamte Gruppe löschen, ausblenden"
      if(iPlg) goto L_Grp0;   // if plugin active: only menu-funcs
        MenInd[ii] = MSG_GrpMove;  ++ii;   // move groupObjs
        MenInd[ii] = MSG_GrpHide;  ++ii;   // gesamte Gruppe ausblenden
        MenInd[ii] = MSG_GrpDelA;  ++ii;   // gesamte Gruppe löschen
        MenInd[ii] = MSG_GrpClear; ++ii;   // gesamte Gruppe auflösen
        MenInd[ii] = MSG_GrpInv;   ++ii;   // Gruppe invertieren
      goto L_work; // skip analyze
      // break;


    //----------------------------------------------------------------
    case Typ_SubModel:   // ind=0=main; ind=1=subModel.
      // subModel main: menu Activate.
      // subModels (nicht main) menu Activate und Delete.
//       if(version == 2) {     // create new sm - UNUSED
//         MenInd[ii] = MSG_CreSubModel; ++ii; // create new sm
//         MenInd[ii] = MSG_help;        ++ii; // help
//       } else {
        MenInd[ii] = MSG_activate;    ++ii; // activate
        if(UI_InpMode == UI_MODE_CAD) {
          if(dbi > 0) {
            MenInd[ii] = MSG_rename;    ++ii; // ren
            MenInd[ii] = MSG_remove;    ++ii; // del
          }
        }
//       }
      break;


    //----------------------------------------------------------------
    case Typ_Model:    // 99; ModelReference selected ..
    case Typ_Mock:     // 95; ModelReference selected ..
      if(iPlg) goto L_Grp0;   // if plugin active: only menu-funcs
      // version=1: activate only.
      // get modeltype from ModelRefIndex - GEHT NED BEI PASSIVE MODELS !
      // only models with typ = MBTYP_INTERN can be activated
      // DB_mdlTyp_iRef (&i1, dbi);
      if(version == 0) { 
        // MenInd[ii] = MSG_move;     ++ii; // move         TODO 2021-02-10
        if(UI_InpMode != UI_MODE_VWR) {
          MenInd[ii] = MSG_edit;     ++ii; // ed
          MenInd[ii] = MSG_remove;   ++ii; // del
        }
      }
      // i1 = Brw_getAux ();
        // printf(" mbTyp=%d\n",i1);
      // if((i1 == ICO_natM)||(i1 == ICO_refM)) {        2018-03-31
      // if(i1 == ICO_natML) {
      // CAD only if no func is active
      if((UI_InpMode == UI_MODE_VWR)  ||
         ((UI_InpMode == UI_MODE_CAD)&&(!IE_FUNC_IS_ACTIVE))) {
        MenInd[ii] = MSG_activate; ++ii; // activate  
      }
      goto L_Grp1;
      // break;


    //----------------------------------------------------------------
    case Typ_Process:
      MenInd[ii] = MSG_activate;    ++ii;
      MenInd[ii] = MSG_delete;    ++ii;
      goto L_work;


    //----------------------------------------------------------------
    case Typ_VAR:
      ii = OMN_popup_Br_ed_del (ii);  // edit, delete
      break;


    //----------------------------------------------------------------
    case Typ_VC:
      ii = OMN_popup_Br_ed_del (ii);  // edit, delete
      break;


    //----------------------------------------------------------------
    case Typ_PT:
    case Typ_LN:
    case Typ_CI:

    case Typ_CV:               // 2012-03-29
    case Typ_CVLNA:            // 2011-09-07
    case Typ_CVPOL:            // 2011-08-05 was Typ_CV
    case Typ_CVPOL2:
    case Typ_CVPSP3:
    case Typ_CVELL:
    case Typ_CVBSP:
    case Typ_CVRBSP:
    case Typ_CVCLOT:
    case Typ_CVTRM:
    // case Typ_CVTRM2:

    case Typ_PLN:
    case Typ_SUR:
    case Typ_SOL:

    case Typ_Note:                       // GroupCode
    case Typ_Tag:                        // Image
    case Typ_ATXT: 
    case Typ_GTXT:                       // Textnote
    case Typ_Dimen:
    case Typ_Dim3:

      if(iPlg) goto L_Grp0;           // if plugin active: only menu-funcs

      ii = OMN_popup_Br_ed_del (ii);  // edit, delete

      if(UI_InpMode != UI_MODE_VWR) {
        if(IE_modify == 0) {             // 0=Add 1=Modify 2=Insert
          if(TYP_IS_CV(typ))  {
            MenInd[ii] = MSG_cvEd; ++ii;        // edit curve
          } else if((typ != Typ_PLN)  &&          // no "move points" for PLN,SUR
             (typ != Typ_SUR))        {
            MenInd[ii] = MSG_movPoints; ++ii;
          }
        }
      }

      // bodies & surfaces: style ..
      if((typ == Typ_SUR)||(typ == Typ_SOL)) {
        MenInd[ii] = MSG_shade; ++ii; // style
      }
      // surfaces: add texture
      if(typ == Typ_SUR) {
        MenInd[ii] = MSG_texture; ++ii;
      }

           
      // goto L_Grp1;
      goto L_men8;


    case Typ_APPOBJ:   // nur Gruppenfunktionen, sonst nix ..
      goto L_Grp0;



    case Typ_Activ:
    case Typ_Joint:
      ii = OMN_popup_Br_ed_del (ii);  // edit, delete
      break;


    case Typ_Vertex:
      return UI_disp_vert ();  // print active vertex-pos

    case Typ_SymB:
      // not supported (PED-symbol)
      return -1;

    case Typ_dynSym:
      // not supported (Temporary-Dynamic-object)
      return -1;



    default:
      TX_Error("OMN_popup_Brw E001 %d",typ);
      return -1;
  }

  goto L_men8;


  //----------------------------------------------------------------
  L_Grp0:                            // grp, no analyze.
     // does group exist ?
      if(Grp_get_nr() > 0) {         //  YES, group exists ..
        // yes group exists; check if selected obj belongs to group
        if(DL_grp1_get(dli) == ON) {
          // yes, obj is in group; remove obj from group
          MenInd[ii] = MSG_GrpRem1;   ++ii;
          MenInd[ii] = MSG_GrpRemTyp; ++ii;
          MenInd[ii] = MSG_GrpRemOTs; ++ii;
        } else {
          // add obj to group
          MenInd[ii] = MSG_GrpAdd1;   ++ii;   // add (obj) to group
        }

      } else {           // no group exists yet
        MenInd[ii] = MSG_GrpAdd1;  ++ii;     // add (obj) to group
      }
      goto L_men9;


  //----------------------------------------------------------------
  L_Grp1:
      // does group exist ?
      if(Grp_get_nr() > 0) {         //  YES, group exists ..
        // yes group exists; check if selected obj belongs to group
        if(DL_grp1_get(dli) == ON) {
          // yes, obj is in group; remove obj from group
          MenInd[ii] = MSG_GrpRem1;   ++ii;
          MenInd[ii] = MSG_GrpRemTyp; ++ii;
          MenInd[ii] = MSG_GrpRemOTs; ++ii;
          MenInd[ii] = MSG_GrpMove;   ++ii;   // move groupObjs
          MenInd[ii] = MSG_GrpHide;   ++ii;   // hide groupObjs
          MenInd[ii] = MSG_GrpDelA;   ++ii;   // delete groupObjs
        } else {
          // add obj to group
          MenInd[ii] = MSG_GrpAdd1;   ++ii;   // add (obj) to group
          MenInd[ii] = MSG_hide;      ++ii;   // hide obj
        }
  
      } else {           // no group exists yet
        MenInd[ii] = MSG_GrpAdd1;  ++ii;     // add (obj) to group
        // no group exists; add hide(8) or show(9)
        if(UI_stat_view != 0) {
          MenInd[ii] = MSG_hide; ++ii; // hide
        } else {
          MenInd[ii] = MSG_show; ++ii; // show
        }
      }



  L_men8:
  if((typ != Typ_SubModel)  &&
     (typ != Typ_APPOBJ))        {
    MenInd[ii] = MSG_inspect; ++ii; // inspect
  }


  // add modify-values
  ii = OMN_popup_Br_modVals (ii, typ, dbi);




  L_men9:
  // copy to model
  if(UI_InpMode == UI_MODE_CAD) {
    if(IE_get_Func() >= 0) {
      MenInd[ii] = MSG_cpyMdl;  ++ii;
    }
  }


  if(typ != Typ_SubModel) {
    MenInd[ii] = MSG_getPar;  ++ii;     // find parent-objects
    MenInd[ii] = MSG_getChd;  ++ii;     // find child-objects
  }


  // delimit
  L_work:
  MenInd[ii] = -1;


  //----------------------------------------------------------------
  // TEST
  // for(i1=0;i1<MenTab.iNr; ++i1)
     // printf(" MenTab[%d]=|%s| MenIa=%d\n",i1,UtxTab__(i1,&MenTab),MenIa[i1]);
  // for(i1=0;i1<ii; ++i1)
     // printf(" MenInd[%d]=%d\n",i1,MenInd[i1]);


  //----------------------------------------------------------------
  // create the list of menutextpointers
  for(i1=0; i1<ii; ++i1) {
    // get index of i2 in MenTab
    // i2 = UTI_ind_iTab_i (MenInd[i1], MenIa, MenSiz);
    i2 = MenInd[i1];
    if(i2 >= 100) {
        // printf(" i1=%d i2=%d\n",i1,i2);
      // strcpy (s1, "modify-value");
      // omnTab[i1] = s1;
      continue;
    }
    if(i2 < 0) continue;

    omnTab[i1] = UtxTab__ (i2, &MenTab);
      // printf("  omnTab[%d] %d |%s|\n",i1,i2,omnTab[i1]);
  }
  omnTab[ii] = NULL;  // terminate List


  // save modelname as active-object
  actObj.typ = typ;
  actObj.dbInd = dbi;
  actObj.dlInd = dli;


  // create the menu
    // UTX_wTab_dump (omnTab);
  OMN_menu1 = GUI_popup__ (omnTab, NULL, 0, OMN_CB_popup, NULL);


  L_exit:
    // printf("ex-OMN_popup_Brw\n");
  return 0;

}


//=========================================================================
  int OMN_CB_MAN_M3_empty (MemObj *mo, void** data) {
//=========================================================================
// line isel in popup-menu (ObjectMenu) selected.

  static int isel=-1;
  char   s1[128];

  int     irc, ii, i1, iEv;
  long    lNr;
  char    *cp1, *cp2;


  // printf("OMN_CB_MAN_M3_empty \n");

  if(mo == NULL) {isel = -2; return 0;} // init

  iEv = GUI_DATA_EVENT;
  if(iEv != TYP_EventPress) return 0;

  isel = GUI_DATA_I1;
  if(isel < 0) return 0;  // skip deSelection
    // printf(" iEv=%d isel=%d\n",iEv,isel);

  // isel: 0=edit; 1=hide; 2=inspect.

  switch (isel) {

    case 0: // edit
        APED_oid_dbo__ (s1, actObj.typ, actObj.dbInd);
        TX_Print(" edit obj  %s   manually ..",s1);
        DL_Get_lNr_dli (&lNr, actObj.dlInd);
          // printf(" act lNr = %ld\n",lNr);
        // work -> line lNr
        ED_work_CurSet (lNr);
        // // set cursor into corresponding sourceline in editor
        // GUI_edi_setLnr (WIN_ED(), lNr);
        GUI_edi_sel_ln (&winED, lNr);
      break;


    case 1:  // hide
      GA_view__ (actObj.dlInd, 1, 0, 0L);     // hide obj
      // GA_view__ (actObj.dlInd, 0, 0, 0L);     // view obj
      break;


    case 2:   // inspect
      OVR_add__ (actObj.typ, actObj.dbInd);
      break;
  }



  return 0;

}


//=========================================================================
  int OMN_CB_popup (MemObj *mo, void** data) {
//=========================================================================
// line isel in popup-menu (ObjectMenu) selected.
//
// iEv: TYP_EventEnter = 400 = create menu (enter)
//      TYP_EventLeave = 401 = leave
//      TYP_EventPress = 402 = user-selection (press mouseKey)
//      TYP_EventUnmap = 407 = unmap (widget disappears)
// isel   selected rowNr or -1=Unmap;   -2=select; <-2=disactivate-row

static void *dll = NULL;
static int  isel=-1;

  char   *namMain = "";
  char   s1[128];

  int     irc, ii, i1, iEv;
  long    lNr;
  char    *cp1, *cp2;



  // init
  if(mo == NULL) {isel = -2; return 0;}

  iEv = GUI_DATA_EVENT;
  isel = GUI_DATA_I1;

    // printf("OMN_CB_popup iEv=%d isel=%d\n",iEv,isel);

  // unhilite last hilited bj (IE_cad_inp_prev__())
  // if(iEv == TYP_EventUnmap) UI_prev_remove ();  // done by Ec-key ..

  // if(event->type == GDK_ENTER_NOTIFY) {isel = LONG__PTR(data); return 0;} // 10
  // if(event->type == GDK_LEAVE_NOTIFY) {isel = -1L; return 0;}        // 11
  // if(event->type != GDK_BUTTON_RELEASE) return 0;                   // 7
  if(iEv != TYP_EventPress) return 0; 

  if(isel < 0) {
    // remove selection from browser (release button outside menu)
    // but only if pupup-window is already gone
    if(isel == -1) Brw_unselect_all ();
    return 0;  // skip deSelection
  }


  ii = MenInd[isel]; // see ../xa/xa_msg.h
    // printf("OMN_CB_popup isel=%d ii=%d typ=%d ind=%ld\n",isel,ii,
           // actObj.typ,actObj.dbInd);


  // UI_block__ (-1, -1, 1);     // wait-cursor
  // UI_CursorWait (0);    // display watch

  // must set the wait-cusor for the popup-window !
  // GUI_cursor (widget, GDK_WATCH);
  // GUI_update__ ();

  //================================================================
  // handle modify
  if(ii >= 100) {
    i1 = ii - 100;
    SRCU_win_var (actObj.typ, actObj.dbInd, i1);
    return 0;
  }


  //================================================================
  switch (ii) {    // see MenTxt


    //----------------------------------------------------------------
    case MSG_GrpInv:        // invert group
      Grp_Inv (1);
      break;


    case MSG_GrpClear:        // clear group
      Grp_Clear (1);
      break;


    case MSG_GrpAdd1:         // add obj to group
      DL_grp1__ (actObj.dlInd, NULL, 1, 0);
      break;


    case MSG_GrpRem1:         // remove obj from group
      DL_grp1__ (actObj.dlInd, NULL, -1, 0);
      break;


    case MSG_GrpRemTyp:       // remove all of this type from Group
      Grp_typ_del (actObj.typ);
      break;


    case MSG_GrpRemOTs:       // remove all other types from Group
      Grp_typo_del (actObj.typ);
      break;


    case MSG_GrpMove:         // Move group
      GMVO__ (0);
      break;


    case MSG_GrpHide:         // hide group
      Grp_hide (0);
      break;


    case MSG_GrpDelA:         // delete group
      Del_grp__ ();
      break;


    case MSG_cpyMdl:          // copy into entry
      APED_oid_dbo__ (s1, actObj.typ, actObj.dbInd);
      IE_sel_CB__ (actObj.typ, actObj.dbInd, s1);
      break;


    case MSG_getPar:          // find parent-objects
      APED_oid_dbo__ (s1, actObj.typ, actObj.dbInd);
      UI_mcl_ctrl (1, s1);
      break; 


    case MSG_getChd:          // find child-objects
      APED_oid_dbo__ (s1, actObj.typ, actObj.dbInd);
      UI_mcl_ctrl (2, s1);
      break;
    



    //----------------------------------------------------------------
    case MSG_activate:        // Function = Activate (model)
      if(actObj.typ == Typ_SubModel) {
          // DEB_dump_obj__ (Typ_ObjDB, &actObj, " _CB_popup-mb:\n");
        // get modelname from ModelRefIndex;   0=main!
        if(actObj.dbInd != 0) {
          // subModel
          cp1 = Brw_getTxt ();
            // printf(" CB_popup-_getTxt |%s| typ=%d\n",cp1,actObj.typ);
          if(!strcmp(cp1, AP_modact_nam)) {
            TX_Print("***** already active ..");
            return 0;
          }
        } else {
        // 0 = main
//           // DB_mdlNam_iRef (&cp1, actObj.dbInd);
          // test if already active ..
          if(MDL_IS_MAIN) {
            TX_Print("***** already active ..");
            return 0;
          }
          cp1 = namMain; // "-main-"
        }
          // printf(" activate |%s|\n",cp1);
        MDL_load_mNam__ (cp1, 1);   // activate Submodel in graficWindow & browser

        break;
      }

      if(actObj.typ == Typ_Process) {
        // activate process
        cp1 = Brw_getTxt ();
        sprintf(s1, "process_%s", cp1);
        PRC_activate__ (s1);
        break;
      }

      L_1_ModRef: // Typ_Model = ModelReference
      if(actObj.typ != Typ_Model) goto L_not_yet;
      // subModelID in row of referenceModels (not subModels) selected !
      irc = DB_mdlNam_iRef (&cp1, actObj.dbInd);
      if(irc < 0) return -1;
        // printf(" CB_popup-actvate-mdlNam |%s|\n",cp1);
      MDL_load_mNam__ (cp1, 1);       // activate Submodel in graficWindow & browser
      break;
   

    //----------------------------------------------------------------
    case MSG_move:        // Move
      if((actObj.typ != Typ_Model) &&
         (actObj.typ != Typ_Mock))     goto L_not_yet;
      // modify RefModel-Position <typ><ind>
      UI_Tra__  (-1L, 0, 0L);        // init window
      UI_Tra__  (actObj.dlInd, actObj.typ, actObj.dbInd); // put ID of sel. obj -> window
      UI_GR_Sel_Filt_set(16);        // directs selections into window UI_WinTra__
      break;


    //----------------------------------------------------------------
    case MSG_edit:          // Edit
      if(UI_InpMode == UI_MODE_CAD) {
        IE_edit_dbo (actObj.dlInd, actObj.typ, actObj.dbInd);


      } else if(UI_InpMode == UI_MODE_MAN) {
        APED_oid_dbo__ (s1, actObj.typ, actObj.dbInd);
        TX_Print(" edit obj  %s   manually ..",s1);
        DL_Get_lNr_dli (&lNr, actObj.dlInd);
          // printf(" act lNr = %ld\n",lNr);
        // work -> line lNr
        ED_work_CurSet (lNr);
        // // set cursor into corresponding sourceline in editor
        // GUI_edi_setLnr (WIN_ED(), lNr);
        GUI_edi_sel_ln (&winED, lNr);
      }
      break;


    //----------------------------------------------------------------
    case MSG_movPoints:     // move points (PED)
    case MSG_cvEd:          // edit points, curve (EDMPT)
      // PED_init (actObj.typ, actObj.dbInd, actObj.dlInd);
      // EDMPT_init (actObj.typ, actObj.dbInd, actObj.dlInd);
      {
        int     irc, typ;
        void    *pa[3];
        // reBuild plugin
        if(AP_stat.comp) {
#ifdef _MSC_VER
          irc = DLL_build ("xa_edmpt.dll");
#else
          irc = DLL_build ("xa_edmpt.so");
#endif
            printf(" build=%d\n",irc);
          if(irc != 0) return -1;
        }
        // fill parameterBlock pa
        typ = actObj.typ;
        pa[0] = &typ;
        pa[1] = &actObj.dbInd;
        pa[2] = &actObj.dlInd;
        // load, start, do not unload dll until button exit.
        // OS_dll_do ("xa_edmpt", "EDMPT__", &pa, 1);
        AP_kex_exec (&dll, "xa_edmpt", "EDMPT__", &pa, DLL_LOAD_EXEC);
      }
// TX_Print("********** see ../xa/tst_edmpt.c *** ");
      break;

    //----------------------------------------------------------------
    case MSG_texture:       // modify texture (TED)
      TED_init (actObj.typ, actObj.dbInd, actObj.dlInd);
      break;


    //----------------------------------------------------------------
    case MSG_rename:        // Rename
      if(actObj.typ != Typ_SubModel) goto L_not_yet;
      // remove popup now
      GUI_Obj_kill (OMN_menu1);
      // get modelname from ModelRefIndex;   0=main!
      if(actObj.dbInd != 0) {
        // subModel
        cp1 = Brw_getTxt ();
      } else {
        // 0 = main
        TX_Print("***** cannot rename main ..");
        return 0;
      }
      Brw_sMdl_ren__ (cp1);     // Rename subModel
      break;


    //----------------------------------------------------------------
    case MSG_delete:        // Delete
    case MSG_remove:        // Delete
      // remove popup now
      GUI_Obj_kill (OMN_menu1);
      cp1 = Brw_getTxt ();
        // printf(" CB_popup-_getTxt |%s| typ=%d\n",cp1,actObj.typ);

      if(actObj.typ == Typ_SubModel) {
        // delete subModel and activate main
        irc = Brw_sMdl_del__ (cp1);     
        if(irc < 0) return -1;
        break;
      }

      if(actObj.typ == Typ_Process) {
        // TODO: check if active; yes: do not delete !
        sprintf(s1, "process_%s", cp1);
        PRC_del_file (s1);      // delete processfile
        Brw_del_oid (cp1);      // delete process in browser
        break;
      }

      L_5_ModRef:
/*
      if(UI_InpMode == UI_MODE_VWR) {
        TX_Print(" CAD must be active ..");
        return 0;
      }
*/
      // init delete obj & depending objs
      // ii = Del_init (actObj.typ, actObj.dbInd, actObj.dlInd); // old version
      Del_obj__ (actObj.typ, actObj.dbInd);

      // unload dll
      AP_kex_exec (&dll, "xa_edmpt", NULL, NULL, DLL_UNLOAD);

      break;


    //----------------------------------------------------------------
    case MSG_CreSubModel:        // create subModel
      // Mod_cre__ ();
TX_Error ("Mod_cre__-L1");
      break;


    //----------------------------------------------------------------
    case MSG_help:        // help browser
      APP_Help ("Browser", "");
      break;


    //----------------------------------------------------------------
    case MSG_hide:        // hide obj
      // get dli from dbo
      actObj.dlInd = DL_find_smObj (actObj.typ, actObj.dbInd, -1L, AP_modact_ibm);
      GA_view__ (actObj.dlInd, 1, actObj.typ, actObj.dbInd);     // hide obj
      break;


    //----------------------------------------------------------------
    case MSG_show:        // show obj
      // get dli from dbo
      actObj.dlInd = DL_find_smObj (actObj.typ, actObj.dbInd, -1L, AP_modact_ibm);
      GA_view__ (actObj.dlInd, 0, actObj.typ, actObj.dbInd);     // view obj
      break;


    //----------------------------------------------------------------
    case MSG_shade:        // surf.style
        ii = DL_get_sStyl (actObj.dlInd);       // return 0=shaded; 1=symbolic
        if(ii == 0) {
          i1 = 2;                         // 1=shaded, 2=symbolic, 3=transparent.
        } else {
          i1 = 1;                         // 1=shaded, 2=symbolic, 3=transparent.
        }
        irc = GA_sStyl__ (actObj.dlInd, i1, actObj.typ, actObj.dbInd);
      break;
			

    //----------------------------------------------------------------
    case MSG_inspect:      // inspect
      OVR_add__ (actObj.typ, actObj.dbInd);
      break;


    //----------------------------------------------------------------
    case MSG_undo:      // undo = delete
        // printf(" undo %d %d\n",actObj.typ, actObj.dbInd);
      // UNDO_obj_undo (actObj.typ, actObj.dbInd);
      // get source-lineNumber from DispListIndex actObj.dlInd
      APED_oid_dbo__ (&s1[1], actObj.typ, actObj.dbInd);
      s1[0] ='_';
      irc = APED_search_defLn (&cp1, &lNr, &i1, s1, -1L, 0);
        // printf(" f-search_defLn irc=%d lNr=%ld s1=|%s|\n",irc,lNr,s1);
      if(irc < 0) return irc;
      irc = UNDO_grp_add (lNr, 0);         // add to undoTab
      UNDO_grp_res (irc);                  // undelete
      break;



    //----------------------------------------------------------------
    default:
      printf("OMN_CB_popup E001 %d %d\n",isel,ii);
      return -1;
  }


  return 0;


  L_not_yet:
    TX_Print(" OMN_CB_popup not yet impl ..");
    return 0;

}

//================================================================
  int OMN_selMen_MAN_M3_empty (int typ, long dbi, long dli) {
//================================================================
// see OMN_popup_Brw UI_GR_selMen_init
// MSG_edit MSG_hide MSG_inspect

  // char  *optLst5[]={"edit", "ausbl", "analy", "\0"};
  char        *mnTab[4];


  // printf("OMN_selMen_MAN_M3_empty %d %ld %ld\n",typ,dbi,dli);

  if(dli < 0L) return 0;  // was indicate ..

  // save modelname as active-object
  actObj.typ = typ;
  actObj.dbInd = dbi;
  actObj.dlInd = dli;


  mnTab[0] = UtxTab__ (MSG_edit, &MenTab);
  mnTab[1] = UtxTab__ (MSG_hide, &MenTab);
  mnTab[2] = UtxTab__ (MSG_inspect, &MenTab);
  mnTab[3] = NULL;

  GUI_popup__ (mnTab, NULL, 0, OMN_CB_MAN_M3_empty, NULL);
  UI_GR_ButtonM1Release ();   // else KeyM1=ON ! 2013-05-01
  

  
  return 0;

}


/*
//================================================================
  int OMN_popup_brw (int  mNr, int typ, long ind, char *mnam) {
//================================================================
// activate objMenu from Browserselection   OLD VERSION
// 
// Input:
//   mNr     menuNumber
//   mnam    modelname (only if typ=Typ-Model)

// sLev - examples: "0" = topNode; "0:0" = first subNode of first topNode;
//                  "0:2" = 3. subNode of first topNode.

  int    i1;
  long   l1;

  char   *optLst1[]={"Activate","\0"};

  char   *optLst2[]={"Activate",
                    "Move",
                    "Edit",
                    "Delete","\0"};


  printf("OMN_popup_brw %d %d %d |%s|\n",mNr,typ,ind,mnam);


  //----------------------------------------------------------------
  // topNode
  if(mNr != 1) goto L_2;
  MenNr = 1;
  GUI_Popup (optLst1, NULL, OMN_CB_popup);
  goto L_99;



  //----------------------------------------------------------------
  L_2:
  // ModelNode
  MenNr = 2;
  GUI_Popup (optLst2, NULL, OMN_CB_popup);



  //----------------------------------------------------------------
  L_99:
  // init CB
  OMN_CB_popup (NULL, NULL, NULL);

  // save modelname as active-object
  actObj.typ = typ;
  actObj.dbInd = ind;
  if(mnam) strcpy(memspc011, mnam);

  return 0;

}
*/

/*
//================================================================
  int OMN_popup_dli (long dli) {
//================================================================
// activate objMenu from DispListIndex

  int    mode, typ;
  long   dbi;
  char   *cp1;


  // printf("OMN_popup_dli %d\n",dli);


  // get DB-typ & DB-index from DispListIndex
  typ = DL_dbTyp__dli (dli);
  dbi = DL_get_dbi (dli);

  //----------------------------------------------------------------
  if(actObj.typ != Typ_Model) goto L_PT;

  // // get modelname from its DB-ind
  // DB_mdlNam_iRef (&cp1, actObj.dbInd);

  mode = 0;
  goto L_menu;


  //----------------------------------------------------------------
  L_PT:
  mode = 0;
  goto L_menu;




  //----------------------------------------------------------------
  L_err:
  TX_Print("OMN_popup_dli I001");
  return 0;


  L_menu:
  OMN_popup_Brw (typ, dbi, dli, mode);
  return 0;

}
*/
 
// EOF
