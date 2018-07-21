// gCAD3D NC-Processor
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
2013-03-10 Rewritten from ../wc/wcut.c. RF.

-----------------------------------------------------
*/
/*!
\file  ../prc/prc_cut1__.c
\brief process cut (Numeric Control)
\code
=====================================================
List_functions_start:

PRCE__              main entry
PRCE_cmd_work__     WORK L C S
PRCE_func__
PRCE_init_dat

PRCE_tb_init
PRCE_tb_win
PRCE_tb_exit
PRCE_lst_postprocs

PRCE_disp__         display cut-path; lin or circ.
PRCE_disp_ln
PRCE_disp_ci
PRCE_disp_txt

PRCE_Out_ck_plg     test polygon normal/revers
PRCE_Out_ck_C       test circle normal/revers
PRCE_Out__          "G1 "|"G2 "|"G3 "
PRCE_Out_clr_up     tool up
PRCE_Out_write_from
PRCE_Out_write_ln
PRCE_Out_write_ci
PRCE_Out_write_txt

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



*/



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                   // va_list

#ifdef _MSC_VER
#define _CRT_SECURE_NO_DEPRECATE
__declspec(dllexport) int PRCE__ (int iFnc, char*);
#define extern __declspec(dllimport)
#endif


// #include "../ut/ut_umem.h"         // UME_reserve
#include "../ut/ut_geo.h"
#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_txt.h"
#include "../ut/ut_TX.h"
#include "../ut/ut_txTab.h"            // UtxTab
#include "../ut/ut_os.h"               // OS_
#include "../ut/ut_ox_base.h"          // OGX_SET_INDEX

#include "../xa/xa_mem.h"              // memspc101
#include "../xa/xa_ui.h"               // APF_*
#include "../xa/xa_msg.h"              // MSG_*
#include "../xa/xa.h"                  // APP_act_*

#include "../ut/func_types.h"               // SYM_TRIANG
#include "../ut/gr_types.h"               // SYM_* ATT_* LTYP_*

#include "../prc/prc_cut1__.h"         // NCCmdAnz, NCCmdTab

#include "../gui/gui__.h"              // GUI_*



//================================================================
// EXTERN
// ex ../ci/NC_Main.c
extern double    APT_ModSiz;


// ex ../xa/xa.c
// extern int     SRC_ato_anz, SRC_ato_SIZ, *SRC_ato_typ;
extern double  *SRC_ato_tab;

// ex ../xa/xa_ui.c:
// extern MemObj ckb_man, ckb_vwr;
extern MemObj UIw_Box_TB;    // toolbarBox



//================================================================
// LOCAL
typedef struct {double rad, len;} Tool;
#define TLTABSIZ 100

Point   oldPos;
Point   actPos;
Circ    actCir;               // current circ (G2|G3)

double  actZsur1;
double  actZclr1;

int     rapid = -1;           // ON=0; OFF=1;
int     tlActNr = -1;
// int     tlActSid = 0;   // (RI LE OFF)   def = off

char    outBuf[256];
Tool    tlTab[TLTABSIZ];

#define cmd_SIZ 100
int     cmd_anz;
int     *cmd_typ;
double  *cmd_tab;


#define iAtt_rp      20     // rapid; without tool
#define iAtt_cut     21     // cut  (with tool)
#define iAtt_tlrp    22     // rapid with tool


static MemObj  PRCE_tb__ = GUI_OBJ_NEW;   // Toolbar

int     PRCE_mode=0;        // 0 = Edit, > 1 = postProc spprc is active
FILE    *PRCE_fpo=NULL;     // nc-outfile
char    spprc[128];         // postproc


// LOCAL PROTOTYPES:
  char** PRCE_lst_postprocs (char *sproc);
  int PRCE_selMen_cb (MemObj *mo, void **data);



//===========================================================================
  int PRCE__ (int iFnc, char* data)  {
//===========================================================================
/// export nc

// the actual position is oldPos.

/// Input:
///   iFnc     >=0 index into NCCmdTab
///            -1  function
///   data     additional text for nc-func NCCmdTab[iFnc]


  int       i1;
  Point     pt1;
  ObjAto    ato1;


  // printf("------------- cut1 ------------------ \n");
  // if(iFnc>=0)printf("PRCE__ |%s|%s| %d\n",NCCmdTab[iFnc],data,iFnc);
  // else printf("PRCE__ %d |%s|\n",iFnc,data);
  // printf("  PRCE_mode=%d\n",PRCE_mode);
  // printf("  rapid=%d tlActNr=%d\n",rapid,tlActNr);


  // function ?
  if(iFnc == -1) return PRCE_func__ (data);




  // commands without parameters

  // decode input
  // cmd_anz = APT_decode_ausdr (cmd_typ, cmd_tab, cmd_SIZ, &data);
  ATO_getSpc_tmp__ (&ato1, cmd_SIZ);
  ATO_ato_srcLn__ (&ato1, data);
  cmd_anz = ato1.nr;
  cmd_typ = ato1.typ;
  cmd_tab = ato1.val;

    // for(i1=0; i1<cmd_anz; ++i1)
    // printf(" %d typ=%d tab=%f\n",i1,cmd_typ[i1],cmd_tab[i1]);



  switch (iFnc) {

    //================================================================
    case 0:   // FROM                          was APT_work_NCCmd
      i1 = APT_decode_pt1 (&actPos, 0, cmd_typ, cmd_tab);
      if(i1 < 0) goto L_err_par;

      // fix actPos-Z
      actPos.z = actZsur1;

      if(PRCE_mode) { // NC-out
        // "FROM pos"
        PRCE_Out_write_ln ("FROM ");

      } else {  // disp
        APT_disp_SymV1 (SYM_TRIANG, 0, &actPos, 0.5);     // Dreieck klein
      }
      oldPos = actPos;
      goto L_hist;


    //================================================================
    case 2:    // GO
        if(tlActNr < 0) {
          tlActNr = 0;
          // DEfault-tool-radius:
          if(tlTab[0].rad == -1.) tlTab[0].rad = APT_ModSiz / 500.;
          if(PRCE_mode) { // NC-out
            PRCE_Out_write_txt ("TL 1");
          }
        }
      if(rapid == ON) {          // 0=ON; 1=OFF;
        rapid = OFF;
        // go down to working-Z ("G1 <oldPos> <newZ>
          // printf(" tool down ..\n"); PRCE_Out_write_txt (" tool down ..");
        actPos = oldPos;
        // actPos.z -= actZclr1;
        actPos.z = actZsur1;
        PRCE_Out__ ("G1 ");
          // PRCE_Out_write_txt (" tool is down ..");
      } else if (rapid == -1) {
        rapid = OFF;
      }
      PRCE_cmd_work__ (data);
      goto L_hist;



    //================================================================
    case 3:    // RP           was APT_wcut_line
    case 4:    // RPR
      // last operation work: go up to clearance-surface
      if(rapid == OFF) PRCE_Out_clr_up ();

      i1 = APT_decode_pt1 (&pt1, 0, cmd_typ, cmd_tab);
      if(i1 < 0) goto L_err_par;
        // UT3D_stru_dump (Typ_PT, &pt1, " pt-rel ");

      if(iFnc == 3) { // RP
        actPos = pt1;
      } else {        // RPR
        actPos.x += pt1.x;
        actPos.y += pt1.y;
        actPos.z += pt1.z;
      }

      // fix actPos-Z
      actPos.z = actZsur1 + actZclr1;
        // printf(" .. fixed z = %f\n",actPos.z);


      if(PRCE_mode) { // NC-out
        // write "RP" out
        if(rapid != ON) PRCE_Out_write_txt ("RP");  // sofort "RP" raus
        // write path out ("G1 pos")
        PRCE_Out_write_ln ("G1 ");

      } else {  // disp ln rapid
        if(tlActNr >= 0) i1 = iAtt_tlrp;
        else             i1 = iAtt_rp;
        PRCE_disp_ln (i1);
      }
      oldPos = actPos;
      rapid = ON;
      goto L_hist;


    //================================================================
    case 20:    // TL  see ../nc/nc_wcut1_work.c :450
      if(cmd_typ[0] == Typ_Val) {
        i1 = cmd_tab[0] - 1;
        if(i1 >= TLTABSIZ) {TX_Error("PRCE__ overflow toolTable"); return -1;}

        // if(rapid == OFF): go ut to clearSurf
        if(rapid == OFF) PRCE_Out_clr_up ();

        if(tlTab[i1].rad < UT_TOL_cv) tlTab[i1].rad = APT_ModSiz / 100.;
        tlActNr = i1;

        sprintf(outBuf, "TL %d",tlActNr+1);
        if(PRCE_mode) { // NC-out
          PRCE_Out_write_txt (outBuf);
        } else {
          PRCE_disp_txt (outBuf);
        }
      }
      goto L_hist;


    //================================================================
    case 10:  // FED
    case 11:  // COOL
    case 16:  // INS
    case 32:  // STOP
      // unmodified -> NC-prog.
      sprintf(outBuf, "%s %s",NCCmdTab[iFnc],data);
      if(PRCE_mode) { // NC-out
        PRCE_Out_write_txt (outBuf);
      } else {
        PRCE_disp_txt (outBuf);
      }
      goto L_hist;


    //================================================================
    case 35:    // TEC
      i1 = cmd_tab[0] - 1;
      if(i1 >= TLTABSIZ) {TX_Error("PRCE__ overflow toolTable"); return -1;}
      if(cmd_anz >= 2) tlTab[i1].rad = cmd_tab[1];
      if(cmd_anz >= 3) tlTab[i1].len = cmd_tab[2];
        // printf(" TL[%d] = %f %f\n",i1,tlTab[i1].rad,tlTab[i1].len);
      // unmodified -> NC-prog.
      if(PRCE_mode) { // NC-out
        sprintf(outBuf, "TEC %s",data);
        PRCE_Out_write_txt (outBuf);
      }
      goto L_hist;


    //================================================================
    case 23:    // CLEAR
      actZclr1 = cmd_tab[0];
      goto L_exit;


    //================================================================
    case 40:    // ZSUR1
      actZsur1 = cmd_tab[0];
      goto L_exit;


    //================================================================
    default: 
      TX_Error("PRCE__ E001");
      return -1;

  }


  L_hist:
    PRCE_hist_save();


  L_exit:
    // printf("exit PRCE__ ..\n\n");

  return 0;







  L_err_par:
      TX_Error("PRCE__ E002 cannot use parameters ..");
      return -1;
    

}


//===========================================================================
  int PRCE_disp__ (char *wTyp)  {
//===========================================================================
// display cut-path; lin or circ.

#define TAB_SIZ 1000

static  Line   ln1, ln2;
static  Circ   ci1, ci2;
static  ObjGX  ox1, oxTab[2];

  int    i1, ii;
  double d1;
  Vector vcx, vcy, vcix, vciy;
static  Point  pta[TAB_SIZ], p1, p2;


  // printf("PRCE_disp__ |%s|\n",wTyp);
  // printf(" TL[%d] = %f %f\n",tlActNr,tlTab[tlActNr].rad,tlTab[tlActNr].len);


  if((tlActNr < 0)||(tlActNr >= TLTABSIZ)) return -1;


  //----------------------------------------------------------------
  // circular path ..
  if((wTyp[1] != '2')&&(wTyp[1] != '3')) goto L_path_lin;


  // circ-side-surface - only if tool-length defined: height
  // ruled surf from 2 circles
  d1 = tlTab[tlActNr].len;
  if(d1 > UT_TOL_cv) {

    ci1 = actCir;
    ci2 = ci1;

    ci2.p1.z = ci1.p1.z + d1;
    ci2.p2.z = ci1.p2.z + d1;
    ci2.pc.z = ci1.pc.z + d1;

    oxTab[0].typ  = Typ_CI;
    oxTab[0].form = Typ_CI;
    oxTab[0].siz  = 1;
    oxTab[0].data = &ci1;

    oxTab[1].typ  = Typ_CI;
    oxTab[1].form = Typ_CI;
    oxTab[1].siz  = 1;
    oxTab[1].data = &ci2;

    ox1.typ   = Typ_SURRU;
    ox1.form  = Typ_ObjGX;
    ox1.siz   = 2;
    ox1.data  = oxTab;

      // UT3D_stru_dump (Typ_ObjGX, &ox1, " lin.sur1 ");
      // UTO_dump__ (&ox1, " lin.surf ");

    GR_DrawSur (&ox1, 5, -1L);
  }



    // circ-bottom-surface
    ci1 = actCir;
      // UT3D_stru_dump (Typ_CI, &ci1, "ci1");
    d1 = tlTab[tlActNr].rad;

    // circ (not surf) if rad=0
    if(d1 < UT_TOL_cv) goto L_circ;

    // + pt normal to pc-p1
    UT3D_vc_tng_ci_pt (&vcx, &ci1.p1, &ci1);
    UT3D_vc_setLength (&vcx, &vcx, -d1);
      // UT3D_stru_dump (Typ_VC, &vcx, "vcx:");
    UT3D_pt_traptvc (&pta[0], &ci1.p1, &vcx);
    ii = 1;

    // outer circ from p1 -> p2
    // move startPt
    UT3D_pt_traptptlen (&ci1.p1, &ci1.p1, &ci1.pc, -d1);
    // move endPt
    UT3D_pt_traptptlen (&ci1.p2, &ci1.p2, &ci1.pc, -d1);
    // fix radius
    if(ci1.rad > 0.) ci1.rad += d1;
    else             ci1.rad -= d1;

    UT3D_cv_ci (&pta[ii], &i1, &ci1, TAB_SIZ-ii, UT_DISP_cv);
    ii += i1;

    // + pt normal to pc-p1
    ci1 = actCir;
    UT3D_vc_tng_ci_pt (&vcx, &ci1.p2, &ci1);
    UT3D_vc_setLength (&vcx, &vcx, d1);
      // UT3D_stru_dump (Typ_VC, &vcx, "vcx:");
    UT3D_pt_traptvc (&pta[ii], &ci1.p2, &vcx);
    ii += 1;


    // inner circ from p2 -> p1
    // move startPt
    UT3D_pt_traptptlen (&ci1.p1, &ci1.p1, &ci1.pc, d1);
    // move endPt
    UT3D_pt_traptptlen (&ci1.p2, &ci1.p2, &ci1.pc, d1);
    // revert this circ
    UT3D_ci_inv1 (&ci1);
    // fix radius
    if(ci1.rad > 0.) ci1.rad -= d1;
    else             ci1.rad += d1;

    UT3D_cv_ci (&pta[ii], &i1, &ci1, TAB_SIZ-ii, UT_DISP_cv);
    ii += i1;

    oxTab[0].typ  = Typ_Typ;   // OGX_SET_INT
    oxTab[0].form = Typ_Int4;
    oxTab[0].siz  = 1;
    oxTab[0].data = PTR_INT(Typ_SURPLN);

    oxTab[1].typ  = Typ_PT;
    oxTab[1].form = Typ_PT;
    oxTab[1].siz  = ii;
    oxTab[1].data = pta;

    ox1.typ   = Typ_SUR;
    ox1.form  = Typ_ObjGX;
    ox1.siz   = 2;
    ox1.data  = oxTab;

      // UT3D_stru_dump (Typ_ObjGX, &ox1, " lin.sur1 ");
      // UTO_dump__ (&ox1, " lin.surf ");

    GR_DrawSur (&ox1, 5, -1L);

    return 0;


  //----------------------------------------------------------------
  // linear path (surf) ..
  L_path_lin:
  // lin-side-surface - only if tool-length defined: height
  d1 = tlTab[tlActNr].len;
  if(d1 > UT_TOL_cv) {
    ln1.p1 = oldPos;
    ln1.p2 = actPos;

    ln2.p1 = oldPos;
    ln2.p2 = actPos;
    ln2.p1.z = ln1.p1.z + d1;
    ln2.p2.z = ln1.p2.z + d1;

    oxTab[0].typ  = Typ_LN;
    oxTab[0].form = Typ_LN;
    oxTab[0].siz  = 1;
    oxTab[0].data = &ln1;

    oxTab[1].typ  = Typ_LN;
    oxTab[1].form = Typ_LN;
    oxTab[1].siz  = 1;
    oxTab[1].data = &ln2;

    ox1.typ   = Typ_SURRU;
    ox1.form  = Typ_ObjGX;
    ox1.siz   = 2;
    ox1.data  = oxTab;

      // UT3D_stru_dump (Typ_ObjGX, &ox1, " lin.sur1 ");
      // UTO_dump__ (&ox1, " lin.surf ");

    GR_DrawSur (&ox1, 5, -1L);
  }


  //----------------------------------------------------------------
  // lin-bottom-surface - path - width = radius * 2
  d1 = tlTab[tlActNr].rad;

  // line (not surf) if only z-value of position is different
  if((UTP_comp2db(oldPos.x, actPos.x, UT_TOL_cv)) &&
     (UTP_comp2db(oldPos.y, actPos.y, UT_TOL_cv))) goto L_line;

  // line (not surf) if rad=0
  if(d1 < UT_TOL_cv) goto L_line;

  // get vcx = vector of length=radius direction = oldPos -> actPos
  UT3D_vc_2ptlen (&vcx, &oldPos, &actPos, d1);
    // UT3D_stru_dump (Typ_VC, &vcx, "vcx:");

  // get vcy = vector of length=radius normal to vcx
  UT3D_vc_perp2vc (&vcy, (Vector*)&UT3D_VECTOR_Z, &vcx);
    // UT3D_stru_dump (Typ_VC, &vcy, "vcy:");

  UT3D_vc_invert (&vcix, &vcx);
    // UT3D_stru_dump (Typ_VC, &vcix, "vcix:");
  UT3D_vc_invert (&vciy, &vcy);
    // UT3D_stru_dump (Typ_VC, &vciy, "vciy:");


  UT3D_pt_traptvc (&pta[0], &actPos, &vcx);
  UT3D_pt_traptvc (&pta[1], &actPos, &vcy);
  UT3D_pt_traptvc (&pta[2], &oldPos, &vcy);
  UT3D_pt_traptvc (&pta[3], &oldPos, &vcix);
  UT3D_pt_traptvc (&pta[4], &oldPos, &vciy);
  UT3D_pt_traptvc (&pta[5], &actPos, &vciy);
  pta[6] = pta[0];

    oxTab[0].typ  = Typ_Typ;   // OGX_SET_INT
    oxTab[0].form = Typ_Int4;
    oxTab[0].siz  = 1;
    oxTab[0].data = PTR_INT(Typ_SURPLN);

    oxTab[1].typ  = Typ_PT;
    oxTab[1].form = Typ_PT;
    oxTab[1].siz  = 7;
    oxTab[1].data = pta;

    ox1.typ   = Typ_SUR;
    ox1.form  = Typ_ObjGX;
    ox1.siz   = 2;
    ox1.data  = oxTab;

      // UT3D_stru_dump (Typ_ObjGX, &ox1, " lin.sur2 ");
      // UTO_dump__ (&ox1, " lin.surf ");

    GR_DrawSur (&ox1, 5, -1L);

  return 0;


  //----------------------------------------------------------------
  L_line:
    PRCE_disp_ln (Typ_Att_go);
    return 0;


  //----------------------------------------------------------------
  L_circ:
    PRCE_disp_ci ();
    return 0;




}


//===========================================================================
  int PRCE_disp_ln (int iatt)  {
//===========================================================================
// 

  long   dli;
  Line   ln1;

  // printf("PRCE_disp_ln \n");
  // UT3D_stru_dump (Typ_PT, &oldPos, " oldPos ");
  // UT3D_stru_dump (Typ_PT, &actPos, " actPos ");


        dli = -1;
        ln1.p1 = oldPos;
        ln1.p2 = actPos;
        GR_CreLine (&dli, iatt, &ln1);

  return 0;

}


//===========================================================================
  int PRCE_disp_ci ()  {
//===========================================================================
// 

  long   dbi;

        dbi = -1;
        GR_CreCirc (&dbi, iAtt_cut, &actCir);

  return 0;

}


//================================================================
  int PRCE_disp_txt (char *txt) {
//================================================================
 

  long   dli;


  dli = DL_StoreObj (Typ_SymV, -1L, 0);

  // Typ_Att_PT = gelb; Typ_Att_hili1 = rot
  GL_DrawTxtA (&dli, Typ_Att_hili1, &oldPos, txt);

  return 0;

}


//===========================================================================
  int PRCE_func__ (char* data)  {
//===========================================================================
// was APT_Init, Pock_Init__

  long GL_Get_DLind ();

  int   i1;


  // printf("PRCE_func__ |%s|\n",data);


  //----------------------------------------------------------------
  if(!strncmp(data, "INIT__", 6)) {

    // report cmdTab
    PRC_set_CmdTab (NCCmdTab);


    // init tools
    for(i1=0; i1<TLTABSIZ; ++i1) tlTab[i1].rad = -1.;

    PRCE_init_dat ();

                         //  col lty thi
    DL_InitAttRec (iAtt_rp,   5,  1,  3);
    DL_InitAttRec (iAtt_cut,  4,  2,  3);
    DL_InitAttRec (iAtt_tlrp, 2,  1,  3);
    // col: 5=yellow, 2=red, 4=blue

    // activate toolbar
    PRCE_tb_init ();

    if(PRCE_mode) {
      fprintf (PRCE_fpo,"$$-----------------------------------------\n");
      fprintf (PRCE_fpo,"$$ %s\n",OS_date1());
      fprintf (PRCE_fpo,"$$ MODEL %s\n",AP_mod_fnam);
      fprintf (PRCE_fpo,"$$ PROCESSOR %s\n",&APP_act_proc[4]);
      fprintf (PRCE_fpo,"$$ PROCESS %s\n",&APP_act_nam[8]);
      fprintf (PRCE_fpo,"$$-----------------------------------------\n");
      fprintf (PRCE_fpo,"G90\n");
    }

    
  //----------------------------------------------------------------
  } else if(!strncmp(data, "EXIT__", 6)) {
    // VWR needs no CmdTab ..
    PRC_set_CmdTab (NULL);


  //----------------------------------------------------------------
  } else if(!strncmp(data, "RESET ", 6)) {
    // set back in source
    i1 = atoi(&data[6]);
    PRCE_hist_reset (i1);


  //----------------------------------------------------------------
  // MBR_<returncode_of_ED_query_CmdMode>
  // <-1 ignore line
  // -1  empty line; 
  // >=0 partially filled line
  } else if(!strncmp(data, "MBR_", 4)) {
    i1 = atoi (&data[4]);
      // printf(" mbr-i1=%d\n",i1);
    if(i1 == -1) PRCE_m3Menu__ (0);
    else if(i1 >= 0) PRCE_m3Menu__ (1);


  //----------------------------------------------------------------
  } else {
    TX_Error("PRCE_func__ E001"); return -1;
  }


  return 0;

}


//================================================================
  int PRCE_m3Menu__ (int mode) {
//================================================================

  char  **p1;
  char  *optLst0[]={"GO   (work)",
                    "TL   (toolchange)",
                    "RP   (rapid)",
                    "FROM (startpos)",
                    "OK   (continue)",
                    "\0"};

  // printf("PRCE_m3Menu__ %d\n",mode);


  if(mode == 1) {  // line not empty
    return UI_GR_selMen_init (1);
  }


  //----------------------------------------------------------------
  // line empty
  p1 = optLst0;
  GUI_popup__ (p1, NULL, 0, PRCE_selMen_cb, NULL);
  UI_GR_ButtonM1Release ();   // else KeyM1=ON ! 2013-05-01

  return 0;

}



//================================================================
  int PRCE_selMen_cb (MemObj *mo, void **data) {
//================================================================
// callback of popup-menu UI_GR_selMen_init


  int   iEv, isel, iTyp;
  long  ind;
  char  s1[64], s2[16];


  iEv = GUI_DATA_EVENT;

  if(iEv != TYP_EventPress) return 0;

  isel = GUI_DATA_I1;
    // printf("PRCE_selMen_cb %d\n",isel);

  switch (isel) {
    case 0:
      strcpy(s1, "GO ");
      break;
    case 1:
      strcpy(s1, "TL ");
      break;
    case 2:
      strcpy(s1, "RP ");
      break;
    case 3:
      strcpy(s1, "FROM ");
      break;
    case 4:
      strcpy(s1, "");  // 2015-06-23
      s1[0] = 13;
      s1[1] = '\0';
      break;
  }

  UI_AP (UI_FuncSet, UID_Edit_Line, (void*)s1);

  return 0;

}

 
//================================================================
  int PRCE_init_dat () {
//================================================================

  // printf("PRCE_init_dat \n");

  rapid = -1;
  actPos = UT3D_PT_NUL;
  tlActNr = -1;
  actZsur1 = 0.;
  actZclr1 = APT_ModSiz / 100.;

  return 0;

}


//===========================================================================
  int PRCE_cmd_work__ (char *data) {
//===========================================================================
// WORK L C S                        was ../nc/nc_wcut1_work.c

  int     i1, ii, ityp, ptNr, ptMax, rNr;
  long    dbi;
  double  d1;
  void    *vp1;
  char    s1[32];
  Point   *pta;
  Line    ln1;
  ObjGX   ox1;


  // printf("PRCE_cmd_work__ |%s|\n",data);

  // end rapid
        // if(rapid != ON) ??


  // loop tru obj's
  for(ii=0; ii<cmd_anz; ++ii) {

    ityp = cmd_typ[ii];
    dbi = cmd_tab[ii];
      // printf(" %d typ=%d tab=%f\n",ii,ityp,aus_tab[ii]);
      // printf("...........PRCE_cmd_work__........................... \n");
      // printf(" next work %d typ=%d tab=%ld\n",ii,ityp,dbi);


    oldPos = actPos;


    //----------------------------------------------------------------
    if(ityp == Typ_PT) {          // line - nxt point - absolut
      i1 = DB_GetObjDat (&vp1, &rNr, ityp, dbi);
      if(i1 <= 0) goto L_err_par;
      actPos = *((Point*)vp1);
      // add actZsur1 to point
      actPos.z += actZsur1;
      PRCE_Out__ ("G1 ");
      continue;
    }


    //----------------------------------------------------------------
    if(ityp == Typ_VC) {          // line - nxt point - relative
      i1 = DB_GetObjDat (&vp1, &rNr, ityp, dbi);
      if(i1 <= 0) goto L_err_par;
      UT3D_pt_traptvc (&actPos, &actPos, (Vector*)vp1);
      PRCE_Out__ ("G1 ");
      continue;
    }


    //----------------------------------------------------------------
    if(ityp == Typ_LN) {          // line - nxt point - relative
      i1 = DB_GetObjDat (&vp1, &rNr, ityp, dbi);
      if(i1 <= 0) goto L_err_par;
      pta = (void*)memspc101;
      ptNr = 2;
      pta[0] = ((Line*)vp1)->p1;
      pta[1] = ((Line*)vp1)->p2;
      goto L_do_polygon;
    }


    //----------------------------------------------------------------
    if(ityp == Typ_CI) {          // line - nxt point - relative
      i1 = DB_GetObjDat (&vp1, &rNr, ityp, dbi);
      if(i1 <= 0) goto L_err_par;
      actCir = *((Circ*)vp1);
      // add actZsur1 to circle
      actCir.p1.z += actZsur1;
      actCir.p2.z += actZsur1;
      actCir.pc.z += actZsur1;
      // normal/revers ?
      PRCE_Out_ck_C (s1);
      // output ..
      PRCE_Out__ (s1);
      continue;
    }


    //----------------------------------------------------------------
    if(ityp == Typ_CV) {          // line - nxt point - relative
      // was wcg_OutBSP
      pta = (void*)memspc101;
      ptNr = sizeof(memspc101) / sizeof(Point);
      // get polygon from curve UT3D_npt_ox__ 
      //   bspl_pol_bsp bspl_cv_bsp bspl_cvpol_cvbsp
      //   APT_DrawCurv APT_decode_conv_pol
      OGX_SET_INDEX (&ox1, ityp, dbi);
      i1 = UT3D_npt_ox__ (&ptNr, pta, &ox1, UT_DISP_cv);
      if(i1 < 0) return i1;
      goto L_do_polygon;
    }


    //----------------------------------------------------------------
    TX_Error("PRCE_cmd_work__ E002 obj typ %d unsupported ..",ityp);
    return -1;


    //----------------------------------------------------------------
    L_do_polygon:
      // add actZsur1 to polygon
      for(i1=0; i1<ptNr; ++i1) pta[i1].z += actZsur1;
      // normal/revers ?
      PRCE_Out_ck_plg (s1, ptNr, pta);
      // output polygon
      for(i1=0; i1<ptNr; ++i1) {
        actPos = pta[i1];
        PRCE_Out__ ("G1 ");
      }
      continue;

  }


  //----------------------------------------------------------------
  return 0;


  L_err_par:
      TX_Error("PRCE_cmd_work__ E001 cannot use parameters ..");
      return -1;


}


//================================================================
  int PRCE_Out_ck_plg (char *wTyp, int ptNr, Point *pta) {
//================================================================
// test polygon normal/revers
// Input:
//   oldPos
//   wTyp        G1
// Output:
//   actPos

  int     i1;


  // test if p1 = oldPos or p2 = oldPos
  i1 = UT3D_ipt_cknear_3pt (&oldPos, &pta[0], &pta[ptNr-1]);

  if(i1 == 1) {    // endPt nearer ..
    // invert all points
    MEM_inv_rtab (ptNr, pta, sizeof(Point));
  }

  strcpy (wTyp, "G1 ");

  return 0;

}


//================================================================
  int PRCE_Out_ck_C (char *wTyp) {
//================================================================
// test circle normal/revers
// Input:
//   oldPos
//   wTyp        G1|G2|G3  
// Output:
//   actPos

  int    i1;
  double d1;


  // test if p1 = oldPos or p2 = oldPos
  i1 = UT3D_ipt_cknear_3pt (&oldPos, &actCir.p1, &actCir.p2);

  if(i1 == 0) {    // p1=nearer
      actPos = actCir.p2;
      d1 = actCir.rad;

  } else {         // p2=nearer
      actPos = actCir.p1;
      d1 = actCir.rad * -1.;
  }


  if(d1 > 0.) strcpy (wTyp, "G2 ");
  else        strcpy (wTyp, "G3 ");


  return 0;

}


//================================================================
  int PRCE_Out__ (char *wTyp) {
//================================================================
// wTyp "G1 "|"G2 "|"G3 "
// Input:
//   oldPos
//   actPos
//   wTyp        "G1 "|"G2 "|"G3 "

  int   ii;


  // printf("PRCE_Out__ |%s| PRCE_mode=%d\n",wTyp,PRCE_mode);
  // UT3D_stru_dump (Typ_PT, &oldPos, "oldPos ");
  // UT3D_stru_dump (Typ_PT, &actPos, "actPos ");


  // test if newPos == currentPos
  if(UT3D_comp2pt(&oldPos,&actPos,UT_TOL_cv)) {
      // printf("PRCE_Out__ I001\n");
    // 360-deg-circle ends on startpoint !
    if((wTyp[1] == '2')||(wTyp[1] == '3')) goto L_1;
    return 0;
  }


  L_1:
  if(PRCE_mode) { // NC-out
    // write path out ("G1 pos")
    if((wTyp[1] == '2')||(wTyp[1] == '3')) {
      PRCE_Out_write_ci (wTyp);        // G2 G3
    } else {
      PRCE_Out_write_ln (wTyp);       // G1 FROM
    }


  } else {  // disp
    if(rapid == ON) {
      if(tlActNr >= 0) ii = iAtt_tlrp;
      else             ii = iAtt_rp;
      PRCE_disp_ln (ii);

    } else {
      PRCE_disp__ (wTyp);
    }
  }

  oldPos = actPos;

  return 0;

}
 

//================================================================
  int PRCE_Out_clr_up () {
//================================================================

    // printf(" tool up ..\n");// PRCE_Out_write_txt (" tool up ..");
  actPos = oldPos;
  // actPos.z += actZclr1;
  actPos.z = actZsur1 + actZclr1;
  PRCE_Out__ ("G1 ");
    // printf(" tool is up ..\n");// PRCE_Out_write_txt (" tool is up ..");

  return 0;

}
 

//===========================================================================
  int PRCE_Out_write_from ()  {
//===========================================================================
// write line into nc-file           "G1 actPos.x, actPos.y"
// Input:
//   actPos
// 

// was APT_PP_Write_Lin APT_PP_Write APT_PP_WriOut wcut_OutObj wcut_Out__


  strcpy (outBuf,"FROM ");
  UTX_add_fl_u2 (outBuf, actPos.x, actPos.y);
        
  PRCE_Out_write_txt (outBuf);
      
  return 0;
      
}     

//===========================================================================
  int PRCE_Out_write_ln (char *wTyp)  {
//===========================================================================
// write G1 into nc-file           "G1 actPos.x, actPos.y"
// Input:
//   actPos        

// was APT_PP_Write_Lin APT_PP_Write APT_PP_WriOut wcut_OutObj wcut_Out__
//     wcut_OutPP

  char    s1[64]="", s2[64]="", s3[64]="";

  // printf("PRCE_Out_write_ln |%s| %f %f %f\n",wTyp,actPos.x,actPos.y,actPos.z);


  // strcpy (outBuf, wTyp);
  // UTX_add_fl_u3 (outBuf, actPos.x, actPos.y, actPos.z, ' ');
  UTX_add_fl_u (s1, actPos.x);
  UTX_add_fl_u (s2, actPos.y);
  UTX_add_fl_u (s3, actPos.z);
  sprintf(outBuf, "%s X%s Y%s Z%s",wTyp,s1,s2,s3);

  PRCE_Out_write_txt (outBuf);

  return 0;

}


//===========================================================================
  int PRCE_Out_write_ci (char *wTyp)  {
//===========================================================================
// write G2|G3 into nc-file           "G2 endPt centerPt"
// Input:
//   acCen, acRad
// 

// was APT_PP_Write_Lin APT_PP_Write APT_PP_WriOut wcut_OutObj wcut_Out__
// wcut_OutPP

  char    s1[64]="", s2[64]="", s3[64]="";
  char    s4[64]="", s5[64]="", s6[64]="";
  Vector  vcc;


  // endpt
  // strcpy (outBuf, wTyp);
  // UTX_add_fl_u3 (outBuf, actPos.x, actPos.y, actPos.z, ' ');
  UTX_add_fl_u (s1, actPos.x);
  UTX_add_fl_u (s2, actPos.y);
  UTX_add_fl_u (s3, actPos.z);


  // centerpt 
  // strcat (outBuf,",");
  // UTX_add_fl_u3 (outBuf, actCir.pc.x, actCir.pc.y, actCir.pc.z, ' ');

  // I,J,K = centerpt
  // UT3D_vc_2pt (&vcc, &actPos, &actCir.pc);
  UTX_add_fl_u (s4, actCir.pc.x);
  UTX_add_fl_u (s5, actCir.pc.y);
  UTX_add_fl_u (s6, actCir.pc.z);


  sprintf(outBuf, "%s X%s Y%s Z%s I%s J%s K%s",wTyp,s1,s2,s3,s4,s5,s6);

  PRCE_Out_write_txt (outBuf);

  return 0;

}


//===========================================================================
  int PRCE_Out_write_txt (char *txt)  {
//===========================================================================
// write <txt> into nc-file 

  if(!PRCE_mode) return 0;

  fprintf (PRCE_fpo,"%s\n",txt);

  return 0;

}


//================================================================
  int PRCE_tb_init () {
//================================================================
// activate toolbar


  // printf("PRCE_tb_init \n");

  // lock some application-functions...
  UI_func_stat_set__ (-APF_VWR,
                      -APF_MEN0,
                      0);

  // init toolbar
  if(GUI_OBJ_IS_VALID(&PRCE_tb__)) {
    // GUI_set_show (&PRCE_tb__, 1);
    return -1;
  } else {
    PRCE_tb_win (&UIw_Box_TB, GUI_SETDAT_EI(TYP_EventPress,UI_FuncInit));
  }

  return 0;

}


//================================================================
  int PRCE_tb_win (MemObj *mo, void **data) {
//================================================================
 
static int pp_id=0;

  int      ifunc, ii;
  char     **optLst;
  MemObj   box0, w1;


  ifunc = GUI_DATA_I1;

  // printf("PRCE_tb_win %d\n",ifunc);


  if(mo) {
    ii = GUI_OBJ_SRC(mo);     // get widget-id

    if(ii == pp_id) {  // selection of postprocessor
      strcpy(spprc, GUI_DATA_S2);
      PRCE_pp__ ();
      return 0;
    }

  }


  switch (ifunc) {

    //---------------------------------------------------------
    case UI_FuncInit:  // 90
      if(GUI_OBJ_IS_VALID(&PRCE_tb__)) goto L_exit;

      // PRCE_tb__ = GUI_toolbar__ (mo);
      PRCE_tb__ = GUI_toolbox__ (mo);

        // GUI_obj_dump_mo (&Meas_tb);
      box0 = GUI_box_h (&PRCE_tb__, "e");
        // GUI_obj_dump_mo (&box0);
      // box1 = GUI_Vbox (box0, 0);
      sprintf(memspc011, "<b> %s: </b>",APP_act_proc);
      GUI_label_htm__ (&box0, memspc011, "");

      GUI_label__(&box0, " Postproc.:", "");
      GUI_Tip  ("select and start postprocessing");

      // create list of all postprocessors -> file ?
      optLst = PRCE_lst_postprocs (APP_act_proc);
      w1 = GUI_optmen__ (&box0, optLst[0], optLst, NULL, PRCE_tb_win, "10");
      pp_id = GUI_OBJ_SRC(&w1);  // save id of postproc-menu
      strcpy(spprc, optLst[0]);
        // printf("  opt2-widget-nr=%d\n",pp_id);


      GUI_button__ (&box0, " Help ", PRCE_tb_win, &GUI_FuncUCB2, "");
      GUI_button__ (&box0, MSG_const__(MSG_exit),
                         PRCE_tb_win, &GUI_FuncKill, "");


      L_exit:
      // GUI_Win_go (&Meas_tb);
      GUI_set_show (&PRCE_tb__, 1);  // show


      // MSG_pri_0 ("Meas_in"); // Measure: select 2 points ..
      return (0);


    //---------------------------------------------------------
    case UI_FuncUCB2:
        // printf(" Help prc\n");
      APP_Help ("PRC_cut1", "");
      return (0);


    //---------------------------------------------------------
    case UI_FuncKill:  // 99
        // printf(" exit prc\n");
      PRCE_tb_exit ();
      UI_main_set__ (UI_MODE_VWR);
      return (-1);
  }

  return (-1);

}


//================================================================
  int PRCE_tb_exit () {
//================================================================

  // printf("PRCE_tb_exit \n");


  // reset CmdTab
  PRCE_func__ ("EXIT__");

  // clear DL and history
  PRCE_hist_reset (-1);

  // DL_hili_off (-1L);             //  -1 = unhilite all

  // remove Toolbar
  // GUI_set_show (&PRCE_tb__, 0);
  PRCE_tb__ = GUI_toolbox_del (&PRCE_tb__);

  // restore some application-functions...
  UI_func_stat_set__ (APF_VWR,
                      APF_MEN0,
                      0);

  // AP_User_reset ();

  GL_temp_del_all (); // alle temp. obj loeschen ..

  return 0;

}


//================================================================
  char** PRCE_lst_postprocs (char *sproc) {
//================================================================
// create list of postprocessors of processor <sproc>
//   into file <tmp>postproc.lst
// ls -1 <bindir>plugins/<processor>/* > <fnam>
// processor without leading "PRC_"

  static char *optNone[] = {"none",NULL};
  int   i1;
  char  s1[256], s2[256], **optLst;
    
  // printf("PRCE_lst_postprocs |%s|\n",sproc);
    

  // list files -> postprocessors -> file
  // dir to search
  sprintf(s1, "%splugins%c%s",OS_get_bin_dir(),fnam_del,&sproc[4]);
  // outfilnam
  sprintf(s2, "%spostproc.lst",OS_get_tmp_dir());
  i1 = UTX_dir_listf (s2, s1, NULL, NULL);
    // printf(" nrFiles=%d\n",i1);

  if(i1 > 0) {
    optLst = UTX_wTab_file (memspc55, sizeof(memspc55), s2);
      // UTX_wTab_dump (optLst);
    // add "none" to optLst
    optLst[i1] = optNone[0];
    ++i1;
    optLst[i1] = NULL;

  } else {
    optLst = optNone;
  }



  return optLst;

}


//================================================================
  int PRCE_pp__ () {
//================================================================

  char   fnLog[256];

  // printf("PRCE_pp__ |%s|\n",spprc);

  //----------------------------------------------------------------
    // WC_PP_open (1);
    // APT_Init, Pock_Init__
    // open outfile nc.apt
    if(PRCE_fpo == NULL) { 
      sprintf(outBuf,"%snc.apt",OS_get_tmp_dir());
      if ((PRCE_fpo = fopen (outBuf, "w")) == NULL) {
        TX_Error ("PRCE_pp__ open file %s ****",outBuf);
        return -1;
      } 
    }
    PRCE_mode = 1;


  //----------------------------------------------------------------
  // work complete process
  // ED_work_PP ();
  UI_but_END ();     // reprocess whole model

  //----------------------------------------------------------------
    // close outfile nc.apt
    // // WC_PP_open (0);
    if(PRCE_fpo != NULL) {
      sprintf(outBuf,"%snc.apt",OS_get_tmp_dir());
      TX_Print ("NC-APT exported into file %s",outBuf);

      fprintf(PRCE_fpo,"%s\n","FINI");
      fclose (PRCE_fpo);
      PRCE_fpo = NULL;
    }

    PRCE_mode = 0;



  //----------------------------------------------------------------
  // do postprocessing
  if(!strcmp(spprc, "none")) return 0;


  // delete logfile
#ifdef _MSC_VER
  sprintf(fnLog,"\"%snc.log\"",OS_get_tmp_dir());
#else
  sprintf(fnLog,"%snc.log",OS_get_tmp_dir());
#endif
    printf("delete %s ..\n",fnLog);
  OS_file_delete (fnLog);


  // system "<pp> <infilnam>"
#ifdef _MSC_VER
    sprintf(memspc011, "cd \"%splugins%c%s\" && %s \"%s\"",
      OS_get_bin_dir(),fnam_del,&APP_act_proc[4], spprc, outBuf);
    
#else
    sprintf(memspc011, "%splugins%c%s%c%s %s",
      OS_get_bin_dir(),fnam_del,&APP_act_proc[4],fnam_del,spprc,outBuf);
#endif
      printf("%s\n",memspc011);
    OS_system (memspc011);



  //----------------------------------------------------------------
  // display pp-output <temp>/nc.log
  sprintf(fnLog,"%snc.log",OS_get_tmp_dir());
  TX_file_Print (fnLog);


  return 0;

}


// EOF
