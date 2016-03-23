//  xa_batch.c                                2005-06-25
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
../exp/ige_r.c  geht mit -D_MSC_VER  ned weil es keine Dll ist !!!!
../gr/ut_GLU.c  hat OpenGL-calls drin !!!!!!!

-----------------------------------------------------
Modifications:
  ..

-----------------------------------------------------
*/
/*!
\file  ../xa/xa_batch.c
\brief  dummy-functions for batch-module
\code
=====================================================
List_functions_start:


List_functions_end:
=====================================================

\endcode *//*----------------------------------------


Das main des gCad-Batch.

Ruft Applikation via "gCAD_Batch (paranz, argv);"

Linkjob dazu xa_batch.mak.

Testausgaben:
 Default ist Linken ohne CMD-Window; somit nur MsgBox usw;
 fuer Tests mit printf die zeile "#ifdef _MSC_VER" raus.


*/



/*
#ifdef _MSC_VER
#include <windows.h>
#endif
*/




#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>





/*
#ifdef _MSC_VER
// nachfolgende externals werden aus dem Main-Exe imported:
#define extern __declspec(dllimport)
#endif
*/


#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_os.h"

#include "xa_uid.h"                    // nur f. AP_SRC_MEM
#include "xa.h"                        //  AP_STAT



// aus ../gr/ut_GLU.c


#define GLT_pta_INC 15000
/*
       Point *GLT_pta = NULL;
       long  GLT_pta_SIZ = 0;
*/

#define GLT_cta_INC 8000
       ObjGX *GLT_cta;
       long  GLT_cta_SIZ;


 








// ============ Externe Var: ==================================
// aus xa.c:
extern int       AP_src;                // AP_SRC_MEM od AP_SRC_EDI
extern AP_STAT   AP_stat;               // sysStat,errStat..

extern int       *aus_typ;
extern double    *aus_tab;


// aus ../gr/ut_GLU.c:
extern Point     *GLT_pta;
extern long      GLT_pta_SIZ;
void glBegin () {}
void glEnd () {}
void glVertex3dv () {}
void glNormal3dv () {}



#ifdef WIN_mit_printf               // Test: Linken mit CMD-Window und printf
// #ifdef _MSC_VER                     // Prod.-Version: Linken ohne CMD-Window



//=================================================
int WINAPI WinMain (HINSTANCE hInstance,
                              HINSTANCE hPrevInstance,
                              LPSTR lpszCmdLine,
                              int nCmdShow) {
//=================================================

  int    i1;
  char   *argv[2], paranz=0, *pcl;
  // char   cbuf[512];



  // den gesamten Commandstring holen. 1.Word is xx.exe.
  // parameter durch Blanks getrennt.
  pcl = GetCommandLine();

  argv[0] = strtok (pcl, " ");
  if(argv[0]) {
    ++paranz;
    // MessageBox (0, argv[0], "Par 0", 0);
    argv[1] = strtok (NULL, " ");
    if(argv[1]) {
      ++paranz;
      // MessageBox (0, argv[1], "Par 1", 0);
    }
  }



#else

//================================================================
  int main (int paranz, char* argv[]) {
//================================================================


  int   i1;
#endif


  printf("gCAD %d\n",paranz);
  // for(i1=0; i1<paranz; ++i1) printf("%d|%s|\n",i1,argv[i1]);



  // progress -status; BATCH = 1
  AP_stat.batch = 1;


  // init Basisdir
  OS_Init_ (argv[0]);

  // // init tolerances
  // WC_Init_Tol();

  // init View-Plane and ConstrPlane
  AP_Init_planes ();

  // APT-Texbuffer fuer ersten Start
  UTF_alloc__ (100);

  // aus_typ und aus_tab werden auf GLT_cta und GLT_pta gesetzt !!!!
  // aus_SIZ =  AUS_SIZ;
  // GLT_pta = (Point*)malloc(GLT_pta_INC*sizeof(Point));
  GLT_alloc_pta (GLT_pta_INC);
  // GLT_cta = (ObjGX*)malloc(GLT_cta_INC*sizeof(ObjGX));
  GLT_alloc_cta (GLT_cta_INC);

  // init DL, ED, WC
  AP_Init1 ();

  // AP_sysStat += 2;   // set Startup-bit

  // kein Editor ..
  // UI_src_mem();
  AP_src = AP_SRC_MEM;


/*
  // die Startparameter abarbeiten
  i1 = 1;
  L_next_par:
  if(paranz > i1) {
    // printf(" nxtpar |%s|\n",argv[i1]);
    irc = AP_work__ (argv[i1], argv[i1+1]);
    if(irc > 0) i1 += irc;
    ++i1;
    goto L_next_par;
  }
*/


  // Startup fertig; in debug: give chance to set watchpoints
  AP_debug__ ("batch"); // die func ist ganz leer; ist als stoppoint definiert.


  // Call Userfunc
  gCAD_Batch (paranz, argv);


  return 0;

}


//================================================================
//================================================================
//      HIER ALLE DUMMY-FUNKTIONEN
//================================================================
//================================================================
// OFF = 1;       ON = 0.
 
// xa_aux.c
  void AP_ImportDxf () {}
  void AP_Import3ds () {}
  void AP_ImportLwo () {}
  void AP_ImportXML () {}
  void AP_ImportWRL_ckTyp () {}

// ../ci/NC_drill.c
  void APT_drill_line () {}

//================================================================
// xa_ui.c
  int    UI_InpMode    = UI_MODE_START;
  // int    UI_InpSM      = 1;
  int xa_fl_TxMem;
  int  UI_win_main () {}
  void UI_ToolBars () {}
  void UI_DllLst_work () {}
  void UI_menCB () {}
  void UI_open__ () {}
  // void UI_reset () {}
  void UI_cb_search () {}
  void UI_AP () {}
  void UI_loadMock_CB () {}
  void UI_loadImg_CB () {}
  void UI_Ed_sel_ln () {}
  void UI_Ed_sel () {}
  void UI_Set_ConstPl_Z () {}
  void UI_GR_DrawInit () {}
  void UI_GR_DrawExit () {}
  void UI_GR_ScalAuto () {}
  // void UI_upd_Refs () {}
  // void UI_ask_mode () {}
  void UI_askEscape () {}
  void UI_sur_act_CB1 () {}
  void UI_suract_keyIn () {}
  // void UI_InpSM () {}      ist ein Int !!!!
  // void UI_WinSM () {}
  void UI_butCB () {}
  void UI_CursorWait () {}
  void UI_disp_vec1 () {}
  void UI_disp_tra () {}
  int AP_User_reset () {return 0;}
  void* WIN_ED () {}
  int UI_RelAbs_act () {}

// xa_brw.c
  void Brw_Mod_add () {}
  void Brw_sMdl_act () {}
  void Brw_Mdl_init () {}

// xa_undo.c
  void UNDO_chg_add () {}
  void UNDO_clear () {}

// ../xa/xa_ped.c
  void PED_CB1 () {}

  // void GUI_Dialog (void* v1, char* txt) {printf("*** GUI_Dialog |%s|\n",txt);}
  void GUI_DialogYN () {}
  // void GUI_GetText () {}
  // void GUI_GetText_CB () {}
  // void GUI_List1 () {}
  // void GUI_List2 () {}
  // void GUI_save__ () {}
  void GUI_Tip () {}

  void GUI_Tx_rmLast () {}

  void GUI_Ed_setCnxtL () {}
  void GUI_Ed_getLnr () {}
  void GUI_Ed_setLnr () {}
  void GUI_Ed_RdLnAct () {}
  void GUI_Ed_getEof () {}

// xa_ui_cad.c
  void IE_get_modify () {}
  void IE_cad_init0 () {}
  void IE_activate () {}
  void IE_cad_ResetInputs () {}

// xa_ga.c
  // void GA_hide__ () {}
  // void GA_hide_fil_tmp () {}
  // void GA_load__ () {}

// xa_prg.c
  void PRG_win__ () {}
  void APT_decode_print () {}
  void IO_wri_ope () {}
  void IO_wri_wri () {}

// xa_sele.c
  void sele_get_pos () {}
  void UI_GR_get_selNam () {}

// ../xa/xa_ui_gr.c
  int UI_GR_get_actPos_ () {}
  int UI_GR_get_actPosA () {}
  int UI_GR_Sel_Filter () {}

// xa_tex.c
  void Tex_Init__ () {}
  void Tex_getRef () {}
  void Tex_getBas__ () {}
  void Tex_get_fn () {}
  void Tex_dump__ () {}
  void Tex_DelAll () {}
  void Tex_setRefInd () {}
  void Tex_setSurfInd () {}
  void Tex_actBas_set () {}
  void Tex_actBas_get () {}
  void Tex_TexRef_InitU () {}
  void Tex_TexRef_InitS () {}
  void Tex_RefTest () {}
  void TexRefNr () {}
  void TexRefTab () {}
  void Tex_itb_itr () {}
  void Tex_tbNr () {}
  void Tex_addRef () {}
  void Tex_addBas1 () {}
  void Tex_savRef () {}

// ../gr/gru.c
  void GRU_tex_pos2 () {}
  void GRU_tex_pos3 () {}

// ../gr/ut_gr.c
  void GR_Disp_pt () {}
  void GR_Disp_pt2 () {}
  void GR_Disp_pTab () {}
  void GR_Disp_vc () {}
  void GR_Disp_vc2 () {}
  void GR_Disp_ln () {}
  void GR_Disp_ln2 () {}
  void GR_Disp_cv () {}
  void GR_Disp_cv2 () {}
  void GR_Disp_ac () {}
  void GR_Disp_ell () {}
  void GR_Disp_pol () {}
  void GR_Disp_bsp () {}
  void GR_Disp_sur () {}
  void GR_Disp_sru () {}
  void GR_Disp_pln () {}
  int  GR_Disp_txi () {return 0;}
  int GR_Disp_ln1 () {return 0;}
  int GR_Disp_tx () {return 0;}
  int GR_Disp_dbo () {return 0;}
  void GR_Disp_txi2 () {}
  void GR_Disp_su3p2 () {}
  void GR_Disp_su3pt () {}
  void GR_Disp_axis () {}
  void GR_DrawPoint () {}
  void GR_DrawLine () {}
  void GR_DrawCirc () {}
  void GR_DrawDimen () {}
  void GR_DrawTxtG () {}
  void GR_DrawTxtA () {}
  void GR_DrawPlane () {}
  void GR_DrawSur () {}
  int  GR_DrawModel () {}
  void GR_DrawCvPpsp3 () {}
  void GR_DrawCvBSp () {}
  void GR_DrawCvRBSp () {}
  void GR_DrawCvPol () {}
  int GR_DrawCvCCV () {}
  int GR_DrawCvEll () {}
  int GR_DrawCvCCV2 () {return 0;}
  int GR_DrawCvClot () {return 0;}
  int GR_Draw_spu () {return 0;}
  int GR_DrawFan () {return 0;}
  int GR_DrawStrip () {return 0;}
  int GR_DrawTriaStrip () {return 0;}
  void GR_DrawFtab () {}
  void GR_CreLine () {}
  void GR_CreDitto2 () {}
  void GR_CreSol__ () {}
  void GR_CrePoint () {}
  void GR_CreCirc () {}
// ../gr/ut_gtx.c
  void GR_Init1     () {}
  void GR_InitGFPar () {}
  void GR_tx_scale () {}

// ../gr/ut_DL.c
  DL_Att     *GR_ObjTab = NULL;            // das ist die DL
  long       GR_TAB_IND = 0;               // naechster freier Index
  void DL_Init () {}
  void DL_alloc__ () {}
  void DL_InitAttTab () {}
  void DL_Redraw () {}
  void DL_find_obj () {}
  void DL_GetTyp () {}
  void DL_GetInd () {}
  void DL_GetTrInd () {}
  void DL_Get_lNr_dli () {}
  void DL_get_dla () {}
  void DL_SetInd () {}
  void DL_Lay_mod () {}
  void DL_Lay_act_g1 () {}
  void DL_Lay_col_g1 () {}
  void DL_Lay_ltyp_g1 () {}
  void DL_Lay_thk_g1 () {}
  void DL_Lay_typ_g1 () {}
  void DL_Lay_add () {}
  void DL_StoreObj () {}
  void DL_StoreAtt () {}
  void DL_unvis_set () {}
  void DL_unvis__ () {}
  void DL_pick_set () {}
  void DL_disp_def () {}
  void DL_hide__ () {}
  // void DL_disp_hili () {}
  void DL_disp_reset () {}
  void DL_sav_dynDat  () {}
  void DL_wri_dynDat  () {}
  void DL_wri_dynDat0 () {}
  void DL_wri_dynDat1 () {}
  void DL_load_dynDat () {}
  void DL_Stat () {}
  void DL_parent_hide () {}
  void DL_GetNrSur () {}
  void DL_GetNrSol () {}
  int DL_setRefSys () {}
  // void DL_get_iLast () {}

// ../gr/tess_su.c
  // int TSU_mode = 0;               // 0=Normal(Draw); 1=Store -> TSU_vMem.
  // void TSU_get_mode () {}
  // void TSU_fix_ax_crx () {}
  // void TSU_DrawSurTRV () {}
  // void TSU_DrawSurTBSP () {}

// ../gr/ut_GLU.c
  // void GLT_exit () {}

// ../gr/ut_GL.c
  void* GL_actCol;

  void GL_alloc__ () {}
  void GL_EndList () {}
  void GL_View_get () {}
  void GL_GetActInd () {}
  void GL_Get_DLind () {}
  Point GL_GetCen () {}
  int  GL_SelVert__ () {return 0;}
  void GL_InitPtAtt () {}
  void GL_Del0 () {}
  void GL_Delete () {}
  void GL_temp_delete () {}
  void DL_hili_off () {}
  void GL_Clear () {}
  void GL_View_set () {}
  void GL_Surf_Ini () {}
  void GL_SetConstrPln () {}
  void GL_fix_DL_base () {}
  void GL_InitModelSize () {}
  void GL_ColSet () {};
  int  GL_DefColSet () {return 0;}
  void GL_DefColGet () {}
  void DL_hili_on () {}
  void GL_LoadBMP () {}
  void GR_Draw_dbo () {}
  void GL_DrawTag1 () {}
  void GL_DrawSymB () {}
  void GL_DrawSymV () {}
  void GL_DrawSymV2 () {}
  void GL_DrawSymV3 () {}
  void GL_DrawVec () {}
  void GL_DrawPoly () {}
  void GL_DrawPoly2D () {}
  void GL_DrawPoint () {}
  void GL_DrawSymVX () {}
  void GL_DrawDim3 () {}
  void GL_DrawTxtLBG () {}
  void GL_DrawTxtLG () {}
  void GL_DrawTxtsym () {}
  void GL_DrawSur () {}
  void GL_Disp_sur () {}
  void GL_Exit__ () {}
  void GL_GetEyeLn () {}
  void GL_selPt () {}
  void GL_Tex_Ini () {}
  void GL_Tex_End () {}
  void GL_actTex () {}
  void GL_sSym_spl () {}
  void GL_sSym_srv () {}
  void GL_sSym_srus () {}
  void GL_sSym_sbsp () {}
  void GL_sSym_srbsp () {}



//====================== EOF ===========================
