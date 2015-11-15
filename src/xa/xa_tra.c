//         Hilfsfunktion f. Modify Modelposition.             RF 2007-04-29
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
\file  ../xa/xa_tra.c
\brief GUI Modify_Modelposition UI_Tra_ 
\code
=====================================================
List_functions_start:

UI_WinTra__       main-Window
UI_Tra_nb_cb      callback of switch-notebook-page
UI_Tra__          main-callback (selection)
UI_WinTra_CB      click or key into inputField
UI_Tra_Mod        .

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



----------ProgramFlow--------:
UI_menCB |ModPos|
  UI_GR_Sel_Filter(16);
UI_GR_Select_work1
  UI_Tra__



===========================================================================
Translation   Point to Point; in plane oder along axis oder distance
              along axis oder innerhalb einer Ebene
              zu einem Zentrierungspunkt
                (aus 2 Punkten, aus 3 Punkten, aus Kreis ..)

Rotation      around axis - Richtung  od 2 Punkte
              Angle  (Keyin od Slider)

Surface-Surface         origin-kante pt


Snaps         Surface to Surface  (+ Reverse)
              bei varicad sehr umfangreich  Endpoint Midpoint Circle ..



 cuts all dependencies !
 die neue Posi wird direkt in die aktive Memory-Zeile eingetragen
 (der alte Code wird ersetzt !)

\endcode *//*




...................................
Translation:
  from-point  w10
  to-point    w11
  distance    w12
  axis        w13
  plane       w14

               


................................... 
  Objektzeile suchen 
  Objektzeile decodieren
  Objektzeile neu generieren
  Objektzeile ueberschreiben
    





===========================================================================
UI_menCB |ModPos|
  UI_GR_Sel_Filter(16);
UI_GR_Select_work1
  UI_Tra__



===========================================================================*/

#ifdef _MSC_VER
#include "MS_Def0.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#include "../ut/ut_geo.h"           // Typ_Model
#include "../ut/ut_cast.h"             // INT_PTR

#include "../gui/gui__.h"              // Gtk3

#include "../db/ut_DB.h"            // DB_get_ModBas
#include "../gr/ut_UI.h"            // UI_FuncSet
#include "../xa/xa_uid.h"           // AP_SRC_EDI
#include "../xa/xa_mem.h"           // memspc51, mem_cbuf1


#define   TRUE 1
#define   FALSE 0


//============ Extern Var: =====================
// extern int       AP_src;                // AP_SRC_MEM od AP_SRC_EDI



//============ Lokale Var: =====================
static struct {MemObj wa0[5];
               MemObj wa1[4];
               MemObj wa2[7];
               MemObj lb_m;    // label for ModelID
               ModelRef  mr0;
               ModelRef  mr1;
               long      dli, lNr;
               int       typ, ind;
               char      fnc, inr, stat, mode, modi;} otra;
/// \code
/// wa0     EntryFelder TRANSL
/// wa1     EntryFelder ROT
/// wa2     EntryFelder Surf
/// fnc     active function; 0=Transl, 1=Rot, 2=Surf
/// mode    0=Init; 1=Preview; 2=Reset; 3=save; 4=Exit
/// stat    1=panelLeer;
/// modi    0=unmodified; 1=modfied
/// inr     active inputField
/// \endcode

static int otra_typ0[]={
   Typ_PT, Typ_PT, Typ_LN, Typ_Val, Typ_Val };

static int otra_typ1[]={
   Typ_LN, Typ_PT, Typ_Val, Typ_Val };

static int otra_typ2[]={
   Typ_PT, Typ_PT, Typ_PT, Typ_PT, Typ_PT, Typ_PT, Typ_PT };



//========= Prototypen: ========================







//================================================================
  int UI_Tra_Mod () {
//================================================================
/// mode    0=Init; 1=Preview; 2=Reset; 3=save; 4=Exit


  int    i1, irc, lLen, lNr;
  long   l1;
  char   oNam[32], *cPos, *cp1, *cp2, *cp3, *cp4, *cp5;
  double   d1;
  ModelRef *mr;
  ModelBas *mb;
  ObjGX    obj1;
  Point    pt1, pt2;
  Line     ln1;
  Vector   vc1, vc2;
  Plane    pl1;
  TraRot   tr1;
  Mat_4x3  ma, ima;
   


  printf("UI_Tra_Mod %d\n",otra.mode);



  //============= 0=Init ============================================
  // das Model (otra.typ, otra.ind) suchen und in mr0 und mr1 speichern.
  if(otra.mode != 0) goto L_Preview;         // 0=Init

  // lNr suchen
  // UI_Tra__ hat otra.typ & ind gesetzt; Objektzeile suchen.
  APED_oid_dbo__ (oNam, otra.typ, otra.ind);
  irc = APED_search_defLn (&cPos, &otra.lNr, &lLen, oNam, -1, 0);
  if(irc < 0) {TX_Print("cannot find definitionLine .."); return 0;}
  

/*
  // copy Zeile -> mem_cbuf1
  UTX_cp_Line (mem_cbuf1, cPos, mem_cbuf1_SIZ);
    printf(" zeile=|%s|\n",mem_cbuf1);

  // skip the DefinitionHeader
  cPos = strchr(mem_cbuf1, '=');
  ++cPos;

  // decode Zeile -> obj1
  irc = APT_obj_expr (&obj1, otra.typ, cPos);
    UTO_dump__ (&obj1, "old obj:");


  // gesamte ModelRef merken
  mr = obj1.data;
*/

  // get ModelRef from DB
  mr = DB_get_ModRef (otra.ind);


  // copy ModelRef; new ModelRef = Kopie
  otra.mr0 = *mr;
  otra.mr1 = otra.mr0;

  // init
  // otra.fnc  = 0;
  otra.inr  = 0;
  otra.stat = 1;
  otra.modi = 0;

  return 0;




  L_Preview:
  if(otra.mode == 1) goto L_TRA;     // 1=Preview
  if(otra.mode == 3) goto L_Save;    // 3=save
  if(otra.mode != 2) goto L_Exit;    // 4=Exit



  //=================== 2=Reset ======================================
  // origin gleich: clear all EntryFields;
  // else copy newModelPos = oldModelPos; Redraw.
  if(UT3D_comp2pt(&otra.mr1.po, &otra.mr0.po, UT_TOL_pt) != 0) {

      printf(" clear step 1\n");

    // clear all EntryFields & reset Status
    UI_WinTra__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncClear));

    return 0;
  }

    printf(" clear step 2\n");

  GL_temp_delete (); // alle temp. obj loeschen ..
  DL_disp_reset (ED_get_lnr_act());
  DL_Redraw ();


  // new ModelRef = Kopie
  otra.mr1 = otra.mr0;


  // display in primary position
  otra.mode = 1;     // 1=Preview
  goto L_modify;




  //=================== 3=Save ======================================
  L_Save:
    if(otra.modi == 0) goto L_Exit;
    goto L_modify;



  //=================== Exit ======================================
  L_Exit:   // 4=Exit

    WC_set_obj_stat (0);        // perm
    UI_GR_Sel_Filter (0);       // reset function

    GL_temp_delete ();          // alle temp. obj loeschen ..
    DL_disp_reset (ED_get_lnr_act());
    DL_Redraw ();



  return 0;







  //============ Modify/Preview ====================================
  // TRANSLATE
  L_TRA:
  otra.modi = 1;
  if(otra.fnc != 0) goto L_ROT;

  cp1 = GUI_entry_get (&(otra.wa0[0]));      // p1
  cp2 = GUI_entry_get (&(otra.wa0[1]));      // p2
  cp3 = GUI_entry_get (&(otra.wa0[2]));      // axis
  cp4 = GUI_entry_get (&(otra.wa0[3]));      // dist
  i1  = GUI_ckbutt_get (&(otra.wa0[4]));     // invert
  // cp5 = GUI_Entry_get (otra.wa0[4]);   // plane
    // printf(" 01=|%s|\n",cp1);
    // printf(" 02=|%s|\n",cp2);


  // wenn 2 Punkte definiert sind: Vektor bilden
  if((strlen(cp1) > 0)&&(strlen(cp2) > 0)) {
    // string aus 3 zahlen -> Point pt1
    APT_obj_expr (&pt1, Typ_PT, cp1);
    APT_obj_expr (&pt2, Typ_PT, cp2);
    UT3D_vc_2pt (&vc1, &pt1, &pt2);
      // UT3D_stru_dump(Typ_VC, &vc1, "p-p");


  // distance & Axis: Vektor bilden
  } else if(strlen(cp3) > 0) {
    APT_obj_expr (&vc1, Typ_VC, cp3);
      // UT3D_stru_dump(Typ_VC, &vc1, "vc");


  } else {
    TX_Print(" define 2 points or vector");
    return 0;
  }



  // distance: Vektor modfizieren
  if(strlen(cp4) > 0) {
    APT_obj_expr (&d1, Typ_Val, cp4);
    UT3D_vc_setLength (&vc1, &vc1, d1);
      // UT3D_stru_dump(Typ_VC, &vc1, "l");
  }


  // checkbox invert
  if(i1 != 0)  UT3D_vc_invert (&vc1, &vc1);
  


  // den ModelOrigin um vc1 verschieben
  UT3D_pt_traptvc (&otra.mr1.po, &otra.mr1.po, &vc1);
    // UT3D_stru_dump (Typ_Model, &otra.mr1, "");

  goto L_modify;




  //================================================================
  // ROTATE 
  L_ROT:
  if(otra.fnc != 1) goto L_SUR;

  cp1 = GUI_entry_get (&(otra.wa1[0]));      // axis; VC od Line
  cp2 = GUI_entry_get (&(otra.wa1[1]));      // pt (rot.Center)
  cp3 = GUI_entry_get (&(otra.wa1[2]));      // ang (rotAng)
  i1  = GUI_ckbutt_get (&(otra.wa1[3]));     // invert


    // APT_obj_expr (&vc1, Typ_VC, cp1);
    APT_obj_expr (&ln1, Typ_LN, cp1);
    pt1 = ln1.p1;
    UT3D_vc_ln (&vc1, &ln1);
    APT_obj_expr (&pt1, Typ_PT, cp2);
    APT_obj_expr (&d1, Typ_Val, cp3);

  if(i1 != 0) d1 = -d1;

  // das ModelRef mr1 (po,vx,vz) transformieren ...
  UTRA_inirot_pt_vc_angr (&tr1, &pt1, &vc1, UT_RADIANS(d1));
  UTRA_def__ (1, Typ_TraRot, &tr1);
  // UTRA_app__ (objo, oSiz, otyp, 1, obji, NULL);
  UTRA_app_pt (&otra.mr1.po, &otra.mr1.po);
  UTRA_app_vc (&otra.mr1.vx, &otra.mr1.vx);
  UTRA_app_vc (&otra.mr1.vz, &otra.mr1.vz);

  goto L_modify;



  //================================================================
  // SURF-SURF
  L_SUR:
  // 1) in das HauptRefSys transportieren
  //      UT3D_pl_pt2vc
  //      UT3D_m3_loadpl
  //      UT3D_m3_invm3
  // 2) vom HauptRefSys in das neue RefSys transportieren
  //      UT3D_pl_pt2vc
  //      UT3D_m3_loadpl

  // 1) in das HauptRefSys transportieren
  cp1 = GUI_entry_get (&(otra.wa2[0]));      // po
  cp2 = GUI_entry_get (&(otra.wa2[2]));      // px
  cp3 = GUI_entry_get (&(otra.wa2[4]));      // py
  APT_obj_expr (&pt1, Typ_PT, cp1);
  APT_obj_expr (&pt2, Typ_PT, cp2);
  UT3D_vc_2pt (&vc1, &pt1, &pt2);
  APT_obj_expr (&pt2, Typ_PT, cp3);
  UT3D_vc_2pt (&vc2, &pt1, &pt2);
  UT3D_pl_pto_vcx_vcy (&pl1, &pt1, &vc1, &vc2);
  UT3D_m3_loadpl (ma, &pl1);
  UT3D_m3_invm3 (ima, ma);


  // das ModelRef mr1 (po,vx,vz) transformieren ...
  UTRA_def__ (1, Typ_TraMat, ima);
  // UTRA_app__ (objo, oSiz, otyp, 1, obji, NULL);
  UTRA_app_pt (&otra.mr1.po, &otra.mr1.po);
  UTRA_app_vc (&otra.mr1.vx, &otra.mr1.vx);
  UTRA_app_vc (&otra.mr1.vz, &otra.mr1.vz);


  // 2) vom HauptRefSys in das neue RefSys transportieren
  cp1 = GUI_entry_get (&(otra.wa2[1]));      // po
  cp2 = GUI_entry_get (&(otra.wa2[3]));      // px
  cp3 = GUI_entry_get (&(otra.wa2[5]));      // py
  APT_obj_expr (&pt1, Typ_PT, cp1);
  APT_obj_expr (&pt2, Typ_PT, cp2);
  UT3D_vc_2pt (&vc1, &pt1, &pt2);
  APT_obj_expr (&pt2, Typ_PT, cp3);
  UT3D_vc_2pt (&vc2, &pt1, &pt2);

  i1  = GUI_ckbutt_get (&(otra.wa2[6]));     // invert
  if(i1 != 0) UT3D_vc_invert (&vc2, &vc2);

  UT3D_pl_pto_vcx_vcy (&pl1, &pt1, &vc1, &vc2);
  UT3D_m3_loadpl (ma, &pl1);

  // das ModelRef mr1 (po,vx,vz) transformieren ...
  UTRA_def__ (1, Typ_TraMat, ma);
  // UTRA_app__ (objo, oSiz, otyp, 1, obji, NULL);
  UTRA_app_pt (&otra.mr1.po, &otra.mr1.po);
  UTRA_app_vc (&otra.mr1.vx, &otra.mr1.vx);
  UTRA_app_vc (&otra.mr1.vz, &otra.mr1.vz);




  //================================================================
  // modify Obj
  L_modify:
  // mr = otra.obj.data;


  // create die Daten fuer die neue Modelposition;
  // Erforderlich: ein ObjGX Ty=Typ_Model, data= ModelRef-struct mr;
  // ModelRef-struct mr: Origin, vx, vz, mnam=Modelname, 
  obj1.typ  = Typ_Model;
  obj1.form = Typ_Model;
  obj1.siz  = 1;
  obj1.data = &otra.mr1;


  // den Modelname ueber die basicModelNr holen und dann in
  // ModelRef.mnam speichern (fuer AP_stru_2_txt)
  mb = DB_get_ModBas (otra.mr0.modNr);
  otra.mr1.mnam = mb->mnam;
    printf(" mnam=|%s|\n",otra.mr1.mnam);



  // Objektzeile neu generieren
  // fuer Preview wird DB-Index des Obj auf 0 gesetzt
  if(otra.mode == 1) {       // 1=Preview; 2=Reset; 3=save;
    l1 = 0;
  } else {
    l1 = otra.ind;
  }
    printf(" txnew1=|%s|\n",mem_cbuf1);
  AP_stru_2_txt (mem_cbuf1, mem_cbuf1_SIZ, &obj1, l1);
    printf(" txnew2=|%s|\n",mem_cbuf1);



  // Preview: Objekt anzeigen ...
  if(otra.mode == 1) {
    lNr = ED_get_lnr_act();
      // printf(" act.lNr=%d\n",lNr);
    ED_work_CAD (lNr, mem_cbuf1);
    DL_Redraw ();
    return 0;
}



  //=================== Save ======================================
  UTF_chg_line (otra.lNr, mem_cbuf1);        // replace line im mem ..
  ED_load__ ();     // if MAN: copy Memory -> Editor
  DB_StoreModel (otra.ind, &obj1);           // replace obj in DB
  DL_SetInd (otra.dli);             // modify DL-Rec (do not create new DL-Rec)
  APT_DrawModel (otra.mr1.modNr, otra.ind, &otra.mr1);


  return 0;

}

 
//================================================================
  int UI_Tra_nb_cb (int pgNr) {
//================================================================
/// callback of switch-notebook-page

  printf("UI_Tra_nb_cb %d\n",pgNr);

  otra.fnc = pgNr;

  // if(otra.fnc == 0) TX_Print("define 2 points or vector ..");

  // clear all EntryFields & reset Status
  UI_WinTra__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncClear));

  return 0;

}


//================================================================
  int UI_Tra__ (long dli, int typ, long dbi) {
//================================================================
// 1.Call: UI_Tra__  (-1L, 0, 0L);        // init window
// 2.Call: UI_Tra__  (actObj.dlInd, actObj.typ, actObj.dbInd);
// DL-Index, DB-Typ und DB-Index of seleceted Obj


  int       irc, i1, rTyp;
  long      dl1;
  double    d1;
  char      *cp1, *cp2;
  Point     pt1, *pp1;
  MemObj    *wTab, wtmp;


  printf("UI_Tra__ stat=%d typ=%d dbi=%ld dli=%ld ind=%d\n",
          otra.stat,typ,dbi,dli,otra.ind);

  irc = 0;


  if((dli >= 0)          ||
     (typ == Typ_TmpPT))     goto  L_1;
  // if(dli < 0) {otra.stat = 0; return 0;}



  //================ Init ==================================
  // Init; nix aktiv ...
  if(otra.stat > 0) {
    // gtk_widget_grab_focus (otra.lb_m);
    return 0;
  }

  // das Panel erzeugen ...
  UI_WinTra__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncInit));

  otra.stat = 1;  // panel aktiv aber leer ..
  otra.inr  = -1;
  // for(i1=0; i1<5; ++i1) otra_tab[i1].typ = 0;
  return 0;



  //=============== Model selected ===========================
  L_1: 
  if(otra.ind >= 0) goto L_2;


  // test if Model was selected ..
  if((typ != Typ_Model) &&
     (typ != Typ_SubModel) &&
     (typ != Typ_Mock)) {
    // goto L_2;
    TX_Print("****** please select a subModel / Mockup-Model");
             // " or key ESC to exit function");
    return 0;
  }


  // save ModelID
  otra.typ  = typ;
  otra.ind  = dbi;
  otra.dli  = dli;

  otra.mode = 0;  // Init
  UI_Tra_Mod ();  // Init
  // otra.stat = 1;  // panel aktiv aber leer ..
  // otra.inr  = 0;

  // cp1 = Mod_mdlNam_iBas (otra.mr0.modNr);
  // printf(" set modelnam |%s|\n",cp1);
  wtmp = otra.lb_m;
  sprintf(mem_cbuf1, "M%ld",dbi);

  // proceed nxt entry
  otra.inr = -1;   // ++ makes field[0]
  goto L_nxt_E;




  //=============== obj. selected ===========================
  L_2:  // convert selection and store in entryfields
    printf(" fnc=%d\n",otra.fnc); // 0=Transl, 1=Rot, 2=Surf
    printf(" inr=%d\n",otra.inr); // inpFieldNr

/*
  // display indicate-symbol
  if(typ == Typ_TmpPT) {
    UI_GR_get_actPosA (&pt1);
    dl1 = -1;
    GL_DrawSymB (&dl1, Typ_Att_hili1, SYM_CIR_S, &pt1);  // Circ red
    DL_Redraw ();
  }
*/

  // get wtmp = pointer to active outputfield
  // rTyp = requested Typ for active outputfield
  if(otra.fnc == 0) {                    // TRANSL
    wtmp = otra.wa0[otra.inr];
    rTyp = otra_typ0[otra.inr];


  } else if(otra.fnc == 1) {             // ROT
      printf(" inr=%d\n",otra.inr);
    wtmp = otra.wa1[otra.inr];
    rTyp = otra_typ1[otra.inr];

  } else if(otra.fnc == 2) {             // SURF
    wtmp = otra.wa2[otra.inr];
    rTyp = otra_typ2[otra.inr];
  }

    // printf(" rTyp=%d\n",rTyp);


  // get Entryfield -> mem_cbuf1
  GUI_entry_copy (mem_cbuf1, 512, &wtmp);


  if(typ == Typ_TmpPT) {
    // UI_GR_get_actPos_ (&cp1);
    UI_GR_get_selNam (&i1, &dl1, &cp1);
      // printf(" UI_GR_actPos |%s|\n",cp1);
    strcpy(mem_cbuf1, cp1);

  } else {
    // get sourceCode from new obj typ/dbi
    // irc = SRC_dbo (mem_cbuf1, rTyp, typ, dbi);
    irc = SRC_src_dbo (mem_cbuf1, mem_cbuf1_SIZ, rTyp, typ, dbi);
  }
    // printf(" _cbuf1 |%s| %d\n",mem_cbuf1,irc);




  if(rTyp == Typ_PT) {   // disply red circle for points
    irc = APT_obj_expr (&pt1, rTyp, mem_cbuf1);
      UT3D_stru_dump(Typ_PT, &pt1, "  vtx pt1:");
    if(irc == 0) {
      dl1 = -1;
      GL_DrawSymB (&dl1, Typ_Att_hili1, SYM_CIR_S, &pt1);  // Circ red
      DL_Redraw ();
    }
  }

/*
  iNr = APT_decode__ (&atyp, &atab, &mem_cbuf1);
    printf("APT_decode__ iNr=%d\n",iNr);
*/


/*
  // Objekt selektiert. Es sollte von der Selektion der Punkt extrahiert
  // werden.Funktionen dazu: IE_cad_sel1 > AP_src_sel_fmt

  // } else if(typ == Typ_PT) {
  // } else {

    // try to get Vertex
    // UI_GR_SelVert (&pt1);
    // save point in dynTab ?
    GL_SelVert__ (&pt1);
      UT3D_stru_dump(Typ_PT, &pt1, "  VertexPos:",i1);


    // // distance pp1 - pt1
    // d1 = UT3D_len_2pt(pp1, &pt1);
      // printf(" dist = %f\n",d1);
    // if(d1 > UT_DISP_cv) {
      // pt1 = *pp1;
    // }


    // Punktkoordinaten als Text nach mem_cbuf1 und ins Entryfeld schreiben
    mem_cbuf1[0] = '\0';
    AP_obj_add_nval (mem_cbuf1, 3, (double*)&pt1);
      // printf(" cb=|%s|\n",mem_cbuf1);

*/


  // write sourceObj into Entryfield
  L_nxt_E:
    GUI_entry_set (&wtmp, mem_cbuf1);

    // proceed to next inputfield (++ .inr)
    if(irc < 1) UI_WinTra__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncNext));

  return 0;

}


//================================================================
  int UI_WinTra_CB (MemObj *mo, void **data) {
//================================================================
/// click or key into inputField

// - 12 GDK_FOCUS_CHANGE  fuer leave.old- und enter new field
// -  8 GDK_KEY_PRESS     fuer zB CursorDwn ..


  MemObj  wtmp;


  printf("UI_WinTra_CB %d\n",GUI_DATA_EVENT);


  // tab-change activates first field of tab !
  if(otra.ind < 0) {
      printf(" skip it\n");
    wtmp = otra.lb_m;
    GUI_obj_focus (&wtmp);
    return TRUE;  // nix tun
  }

  if(!GUI_obj_cmp(mo, &(otra.lb_m))) {
     printf(" Modelfield\n");
    // otra.inr = -1;
    // wtmp = otra.lb_m;
    return FALSE;  // do defaultOperations (set focus to this field)
    // return 0;
  }


  // test which field
  if(otra.fnc == 0) {
    if(!GUI_obj_cmp(mo,&(otra.wa0[0]))) otra.inr = 0;
    if(!GUI_obj_cmp(mo,&(otra.wa0[1]))) otra.inr = 1;
    if(!GUI_obj_cmp(mo,&(otra.wa0[2]))) otra.inr = 2;
    if(!GUI_obj_cmp(mo,&(otra.wa0[3]))) otra.inr = 3;
    wtmp = otra.wa0[otra.inr];

  } else if(otra.fnc == 1) {
    if(!GUI_obj_cmp(mo,&(otra.wa1[0]))) otra.inr = 0;
    if(!GUI_obj_cmp(mo,&(otra.wa1[1]))) otra.inr = 1;
    if(!GUI_obj_cmp(mo,&(otra.wa1[2]))) otra.inr = 2;
    wtmp = otra.wa1[otra.inr];

  } else if(otra.fnc == 2) {
    if(!GUI_obj_cmp(mo,&(otra.wa2[0]))) otra.inr = 0;
    if(!GUI_obj_cmp(mo,&(otra.wa2[1]))) otra.inr = 1;
    if(!GUI_obj_cmp(mo,&(otra.wa2[2]))) otra.inr = 2;
    if(!GUI_obj_cmp(mo,&(otra.wa2[3]))) otra.inr = 3;
    if(!GUI_obj_cmp(mo,&(otra.wa2[4]))) otra.inr = 4;
    if(!GUI_obj_cmp(mo,&(otra.wa2[5]))) otra.inr = 5;
    wtmp = otra.wa1[otra.inr];

  }
    printf(" field=%d\n",otra.inr);




  if(GUI_DATA_EVENT == TYP_EventEnter) {
      printf("GDK_FOCUS_CHANGE \n");
    // gtk_widget_grab_focus (wtmp);
    return FALSE;  // do defaultOperations (set focus to this field)
    // return TRUE;  // do no defaultOperations
  }
   


  if(GUI_DATA_EVENT != TYP_EventRelease) {
    return FALSE;  // do defaultHandler
  }

    // printf("  keyval=%d %x\n",((void*)event)->keyval,
                              // ((void*)event)->keyval);

  switch (GUI_DATA_I2) {

    case GUI_KeyEsc:
      UI_WinTra__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUndo));
      return TRUE;

    case GUI_KeyReturn:
    case GUI_KeyTab:
      // advance to next field
      UI_WinTra__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncNext));
      return TRUE;
  }
  return FALSE;  // do defaultOperations (use all other keys ..)

}


//=====================================================================
  int UI_WinTra__ (MemObj *mo, void **data) {
//=====================================================================
//

static MemObj win0=GUI_OBJ_NEW;
  
  int    ifunc;
  MemObj wb1, wb2, wnb, wch0, wch1, wch2, wtmp;


  ifunc = GUI_DATA_I1;
  printf("UI_WinTra__ %d\n",ifunc);


  switch (ifunc) {


    //---------------------------------------------------------
    case UI_FuncInit:  // 90
      if(GUI_OBJ_IS_VALID(&win0)) {           // Win schon vorhanden ?
        // gtk_widget_destroy (win0);
        return -1;
      }
      win0 = GUI_Win__ ("modify position",UI_WinTra__,"");
      // ..

      wb1 = GUI_box_v (&win0, "");  // Buttons Modify Cancel Exit
      otra.lb_m = GUI_entry__ (&wb1,"Model ","",NULL,NULL, ""); //-160);

      wnb = GUI_notebook__ (&wb1, UI_Tra_nb_cb);

      wch0 = GUI_notebook_add (&wnb, "Translation");
      otra.wa0[0] = GUI_entry__ (&wch0,"from-point ","",
                                 UI_WinTra_CB,NULL, "10");
      otra.wa0[1] = GUI_entry__ (&wch0,"to - point ","",
                                 UI_WinTra_CB,NULL, "10");
      otra.wa0[2] = GUI_entry__ (&wch0,"along axis ","",
                                 UI_WinTra_CB,NULL, "10");
      otra.wa0[3] = GUI_entry__ (&wch0,"distance   ","",
                                 UI_WinTra_CB,NULL, "10");
      otra.wa0[4] = GUI_ckbutt__(&wch0,"invert direction",0,NULL,NULL,"");


      wch1 = GUI_notebook_add (&wnb, "Rotation");
      otra.wa1[0] = GUI_entry__ (&wch1,"axis ","",
                                 UI_WinTra_CB,NULL, "10");
        GUI_Tip  ("key DZ");
      otra.wa1[1] = GUI_entry__ (&wch1,"point ","",
                                 UI_WinTra_CB,NULL, "10");
        GUI_Tip  ("select point for rotation-axis");
      otra.wa1[2] = GUI_entry__ (&wch1,"angle   ","",
                                 UI_WinTra_CB,NULL, "10");
        GUI_Tip  ("key rotation-angle in degree");
      otra.wa1[3] = GUI_ckbutt__(&wch1,"invert direction",0,NULL,NULL,"");


      wch2 = GUI_notebook_add (&wnb, "Surface-Surface");
      otra.wa2[0] = GUI_entry__ (&wch2,"from-origin-point ","",
                                 UI_WinTra_CB,NULL, "10");
      otra.wa2[1] = GUI_entry__ (&wch2,"to-origin-point","",
                                 UI_WinTra_CB,NULL, "10");
      otra.wa2[2] = GUI_entry__ (&wch2,"from-direction-point","",
                                 UI_WinTra_CB,NULL, "10");
      otra.wa2[3] = GUI_entry__ (&wch2,"to-direction-point","",
                                 UI_WinTra_CB,NULL, "10");
      otra.wa2[4] = GUI_entry__ (&wch2,"from-point-in-plane","",
                                 UI_WinTra_CB,NULL, "10");
      otra.wa2[5] = GUI_entry__ (&wch2,"to-a-in-plane","",
                                 UI_WinTra_CB,NULL, "10");
      otra.wa2[6] = GUI_ckbutt__(&wch2,"invert direction",0,NULL,NULL,"");


      wb2 = GUI_box_h (&wb1, "");  // Buttons Modify Cancel Exit
      GUI_button__ (&wb2, "Preview", UI_WinTra__, &GUI_FuncUCB1, "");
      GUI_button__ (&wb2, "Reset",   UI_WinTra__, &GUI_FuncUCB2, "");
      GUI_button__ (&wb2, " Save ",  UI_WinTra__, &GUI_FuncUCB3, "");
        GUI_Tip  ("save, do not exit");
      // GUI_Butt (wb2, " Help ",  UI_WinTra__, (void*)UI_FuncUCB4, 0);
      GUI_button__ (&wb2, " Exit ",  UI_WinTra__, &GUI_FuncUCB4, "");
        GUI_Tip  ("exit, do not save");

      // gtk_window_set_position (win0, GTK_WIN_POS_MOUSE);
      GUI_Win_up (NULL, &win0, 0); // window always on top
      GUI_Win_go (&win0);

      otra.fnc = 0;   // starts with ?
      otra.inr = -1;
      otra.ind = -1;

      return 0;


    //---------------------------------------------------------
    case UI_FuncUCB1:  // Modify-Button; activate new Position     1001
      WC_set_obj_stat (1); // alle Displays ab nun nur temporaer ..
      otra.mode = 1;
      UI_Tra_Mod ();
      return 0;


    //---------------------------------------------------------
    case UI_FuncUCB2:  // Reset-Button; reset old position
      otra.mode = 2;
      UI_Tra_Mod (); // Reset
      return 0;


    //---------------------------------------------------------
    case UI_FuncUCB3:  // Save-Button; save at last new Position and exit
      WC_set_obj_stat (0); // alle Displays ab nun permanent
      otra.mode = 3;
      UI_Tra_Mod ();   // save
      DL_Redraw ();
      // send destroy-signal: impliziert ein UI_FuncKill
      // gtk_signal_emit_by_name (GTK_OBJECT(win0),"destroy");
      return 0;


    //---------------------------------------------------------
    case UI_FuncUndo:  // 113 = clear previous Entryfield
      // wenn kein Window aktiv, Funktion beenden
      if(!GUI_OBJ_IS_VALID(&win0)) {
        UI_GR_Sel_Filter (0);
        return 0;
      }
     
      if(otra.inr > 0) --otra.inr;
      if(otra.fnc == 0) {
        wtmp = otra.wa0[otra.inr];
      } else if(otra.fnc == 1) {
        wtmp = otra.wa1[otra.inr];
      } else if(otra.fnc == 2) {
        wtmp = otra.wa2[otra.inr];
      }
      // clear & focus
      GUI_entry_set (&wtmp, "");
      GUI_obj_focus (&wtmp);
      return 0;


    //---------------------------------------------------------
    case UI_FuncNext:  // 97 = jump into next Entryfield
      // active function = otra.fnc; active inputField =otra.inr;
      if(otra.fnc == 0) {
        if(otra.inr < 3) ++otra.inr;
        wtmp = otra.wa0[otra.inr];
      } else if(otra.fnc == 1) {
        if(otra.inr < 2) ++otra.inr;
        wtmp = otra.wa1[otra.inr];
      } else if(otra.fnc == 2) {
        if(otra.inr < 5) ++otra.inr;
        wtmp = otra.wa2[otra.inr];
      }
      // gtk_signal_emit_stop_by_name (GTK_OBJECT(wtmp),"focus-in-event");
      goto L_focus;
      // return FALSE;  // do defaultOperations



    //---------------------------------------------------------
    case UI_FuncClear:  // 194 = clear all entryFields
      printf(" fnc=%d stat=%d\n",otra.fnc,otra.stat);
      // crash wenn noch nicht komplett up
      if(otra.stat == 0)  return FALSE;  // do defaultOperations
      if(otra.fnc == 0) {
        GUI_entry_set (&(otra.wa0[0]), "");
        GUI_entry_set (&(otra.wa0[1]), "");
        GUI_entry_set (&(otra.wa0[2]), "");
				GUI_entry_set (&(otra.wa0[3]), "");
      } else if(otra.fnc == 1) {
        GUI_entry_set (&(otra.wa1[0]), "");
        GUI_entry_set (&(otra.wa1[1]), "");
        GUI_entry_set (&(otra.wa1[2]), "");
      } else if(otra.fnc == 2) {
        GUI_entry_set (&(otra.wa2[0]), "");
        GUI_entry_set (&(otra.wa2[1]), "");
        GUI_entry_set (&(otra.wa2[2]), "");
        GUI_entry_set (&(otra.wa2[3]), "");
        GUI_entry_set (&(otra.wa2[4]), "");
        GUI_entry_set (&(otra.wa2[5]), "");
      }
      // otra.stat = 1;  // panel aktiv aber leer ..
      otra.inr  = -1;
      return FALSE;  // do defaultOperations
      // goto L_focus;




    //---------------------------------------------------------
    case UI_FuncUCB4:  // 1004 Cancel-Button; exit and do not save
      WC_set_obj_stat (0); // alle Displays ab nun permanent
      // send destroy-signal: impliziert ein UI_FuncKill
      // gtk_signal_emit_by_name (GTK_OBJECT(win0),"destroy");
      // return 0;


    //---------------------------------------------------------
    // destroy-Signal
    case UI_FuncKill:  // 99
      otra.mode = 4;
      UI_Tra_Mod ();   // exit

      GUI_Win_kill (&win0);
      win0 = GUI_OBJ_INVALID();

      otra.stat = 0;
      return 0;

  }


  return 0;


  L_focus:
      printf(" focus %d\n",otra.ind);
    if(otra.ind < 0) wtmp = otra.lb_m;
    GUI_obj_focus (&wtmp);
    return TRUE;  // skip defaultOperations
    // return FALSE;  // do defaultOperations

}


//=============== EOF ============================================
