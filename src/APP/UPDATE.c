/* files new|modified in V2_60_06;
- update / add in all following versions ..
*/


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // va_list va_arg


#include "../ut/ut_geo.h"              // Point
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_txfil.h"
#include "../ut/ut_txt.h"              // UTX_pos_skipLeadBlk

#include "../xa/xa_mem.h"              // memspc*
#include "../xa/xa_msg.h"              // MSG_* ERR_*



#include "../APP/UPDATE.h"


// ../ci/NC_Main.c
extern int APT_dispPT;
extern int APT_dispPL;





//================================================================
// ../ut/ut_geo3d.h
//================================================================

// // replace all UT3D_acos_vc2pt with UT3D_cos_2vcn
// // REPLACEMENT FOR UT3D_acos_2vc :

// UT3D_cos_2vcn       cos of opening angle of 2 vectors;
// - returns factor of lengt of V1 / length of rc -
//   - true length of rc if V1-length = 1;
// - test if 2 vectors normal: if(fabs(UT3D_cos_2vcn(&v1,&v2)) < UT_TOL_min1) yes
// - test if 2 vectors parallel: if(UT3D_cos_2vcn(&v1,&v2)) > 1. - UT_TOL_min1) yes
// - test if 2 vectors normal: if(fabs(UT3D_cos_2vcn(&v1,&v2)) < UT_TOL_min1) yes
// get opening-angle: vectors must be normalized -
// - angr = ACOS(UT3D_cos_2vcn (&v1, &v2));
// 
//            /|
//        V2/  |
//        /    |
//      /      |
//    x--------+------>  V1
//    <---rc-->
// 
#define UT3D_cos_2vcn UT3D_skp_2vc








 
//================================================================
// ../ut/ut_geo3d.c
//================================================================
 
//================================================================
  int UT3D_2vc_ck_parl_pa (Vector *v1, Vector *v2, double tol) {
//================================================================
// UT3D_2vc_ck_parl_pa              compare 2 vectors for parallel and antiparallel
// - vectors must not be normalized;
// Input:
//   tol     radians = Pi / degree; eg 1 deg = 0.017 (RAD_1); Pi = RAD_180;
// retCode   1   vectors are parallel
//           -1  vectors are antiparallel
//           0   not (anti)parallel


  double    ang;

  // printf("==================== UT3D_2vc_ck_parl_pa %f\n",tol);
  // DEB_dump_obj__ (Typ_VC, v1, " V1:");
  // DEB_dump_obj__ (Typ_VC, v2, " V2:");

  ang = UT3D_angr_2vc__ (v1, v2);

  if(fabs(ang) < tol) return 1;
  if(fabs(ang - RAD_180) < tol) return -1;

  return 0;

}






//================================================================
// ../ut/ubscrv.c
//================================================================


//================================================================
  int UCBS_disp__ (CurvBSpl *cvi, char *opts, char *inf) {
//================================================================
// UCBS_disp__                   test-display BSpline-Curve
// - 'p' - display controlPoints as tDyn-symbols;
// retCode:  index DL of first tDyn-obj;

  int     i1;
  long    dli = 0;

  // printf("UCBS_disp__ %s\n",inf);


  if(strchr(opts, 'p')) {
    dli = GL_Get_DLind ();          // get DL-index (for later delete)
    // display points also in mode "VWR"
    APT_dispPT = 0;
    // disp Knotpoints
    for(i1=0; i1<cvi->ptNr; ++i1)
      GR_tDyn_symB__ (&cvi->cpTab[i1], SYM_SQU_S, ATT_COL_RED);
  }



    // printf(" ex-UCBS_disp__ %ld\n",dli);

  return (int)dli;

}

//================================================================
// ../ut/ut_bspl.c
//================================================================
// INF_Typ_CVBSP


//=========================================================================
  int UT3D_cbsp_ln (CurvBSpl **bspo, Memspc *memSeg, Line *ln1, int deg) {
//=========================================================================
// UT3D_cbsp_ln                    BSP-Curv from Line; deg 1 or 2;
// deg = 1

  int      irc, ptNr, kvNr;
  CurvBSpl *bsp1;


  // DEB_dump_obj__ (Typ_LN, ln1, "UT3D_cbsp_ln-in");

  bsp1 = memSeg->next;
  irc = UME_add (memSeg, sizeof(CurvBSpl));
  if(irc < 0) return -1;


  *bsp1 = UT3D_CVBSP_NUL;   // init
  // bsp1->deg  = 1;
  // bsp1->v0   = 0.;
  // bsp1->v1   = 1.;


  ptNr = deg + 1;

  bsp1->ptNr = ptNr;
  bsp1->deg = deg;
  bsp1->ptNr = ptNr;

  // create cpTab
  bsp1->cpTab = memSeg->next;
  irc = UME_add (memSeg, sizeof(Point) * ptNr);
  if(irc < 0) return -1;

  bsp1->cpTab[0] = ln1->p1;

  if(deg == 1) {
    bsp1->cpTab[1] = ln1->p2;

  } else if(deg == 2) {
    UT3D_pt_mid2pt (&bsp1->cpTab[1], &ln1->p1, &ln1->p2);
    bsp1->cpTab[2] = ln1->p2;

  } else {TX_Error("UT3D_cbsp_ln TODO: deg > 2"); return -1;}



  // create kvTab
  bsp1->kvTab = memSeg->next;
  kvNr = ptNr + deg + 1;
  irc = UME_add (memSeg, sizeof(double) * kvNr);
  if(irc < 0) return -1;

  if(irc < 0) return -1;
  bsp1->kvTab[0] = 0.;
  bsp1->kvTab[1] = 0.;

  if(deg == 1) {
    bsp1->kvTab[2] = 1.;
    bsp1->kvTab[3] = 1.;

  } else if(deg == 2) {
    bsp1->kvTab[2] = 0.;
    bsp1->kvTab[3] = 1.;
    bsp1->kvTab[4] = 1.;
    bsp1->kvTab[5] = 1.;
  }

  *bspo = bsp1;

    // DEB_dump_obj__ (Typ_CVBSP, *bspo, "ex-UT3D_cbsp_ln");
    // GR_Disp_CvBSp (bsp, 9, memSeg);

  return 0;

}





//================================================================
// ../ut/ut_rbspl.c
//================================================================
// INF_Typ_CVRBSP


//===================================================================
  int UT3D_rbsp_ci360 (CurvRBSpl *rb1, Memspc *memSeg1, Circ *ci1) {
//===================================================================
// UT3D_rbsp_ci360              get CurvRBSpl from 360-deg-circle
// TODO: rbsp - circ from 0 to 90 deg;

  double   r2h;
  Vector   vcx, vcy;
  Point    pt0, pt1, pt2, pt3, pt4, pt5;



  // printf("===================================== \n");
  printf("UT3D_rbsp_ci180\n");
  DEB_dump_obj__ (Typ_CI, ci1, "  ci1");
  UME_dump(memSeg1, "      memSeg1:");

//   // test 180 or 360 deg
//   if((!UT3D_ck_ci360(ci)) || (!UT3D_ck_ci180(ci))) {
//     // irc = UT3D_rbsp_ci_180 (nrCv, rba, memSeg1, ci);
// 
//   } else {
//     // create 1-3 curves
//     irc = UT3D_rbsp_ci_0 (nrCv, rba, memSeg1, ci);
//   }


/*
  //================================================================
  // 0-90 deg:     NOT-OK !
  rb1->ptNr = 3;
  rb1->v0   = 0.;
  rb1->v1   = 1.;
  rb1->deg  = 2;
  rb1->dir  = 0;   // fwd
  rb1->clo  = 0;   // yes
  rb1->trm  = 1;   // no
  
  r2h = SR_2 / 2.;


  //----------------------------------------------------------------
  // add cpTab = ptNr Point-s to memSeg1
  rb1->cpTab = UME_reserve (memSeg1, sizeof(Point) * rb1->ptNr);
  if(!rb1->cpTab) goto L_err1;
  
  // pt1 = pt0 + vcy
  UT3D_pt_mid2pt (&pt1, &ci1->p1, &ci1->p2);
  UT3D_pt_traptptlen (&pt1, &ci1->pc,  &pt1, fabs(ci1->rad));

  rb1->cpTab[0] = ci1->p1;
  rb1->cpTab[1] = pt1;
  rb1->cpTab[2] = ci1->p2;
  

  //----------------------------------------------------------------
  // add wTab = ptNr weigths (double) to memSeg1
  rb1->wTab = UME_reserve (memSeg1, sizeof(double) * rb1->ptNr);
  if(!rb1->wTab) goto L_err1;
  
  rb1->wTab[0] = 1.;
  rb1->wTab[1] = 6.59;
  rb1->wTab[2] = 1.;
  

  //----------------------------------------------------------------
  // add kvTab = ptNr+deg+1 knots (double) to memSeg1
  rb1->kvTab = UME_reserve (memSeg1, sizeof(double) * (rb1->ptNr + rb1->deg + 1));
  if(!rb1->kvTab) goto L_err1;
  rb1->kvTab[0] = 0.;
  rb1->kvTab[1] = 0.;
  rb1->kvTab[2] = 0.;
  rb1->kvTab[3] = 1.;
  rb1->kvTab[4] = 1.;
  rb1->kvTab[5] = 1.; 
*/
/*
  //================================================================
  // 90-deg:
  rb1->ptNr = 3;
  rb1->v0   = 0.;
  rb1->v1   = 1.;
  rb1->deg  = 2;
  rb1->dir  = 0;   // fwd
  rb1->clo  = 0;   // yes
  rb1->trm  = 1;   // no
  
  r2h = SR_2 / 2.;

  // vcx = vector pc -> p1
  UT3D_vc_2pt (&vcx, &ci1->pc, &ci1->p1);
    DEB_dump_obj__ (Typ_VC, &vcx, "vcx");

  // vcy = normal to vcx and vz
  UT3D_vc_perp2vc (&vcy, &ci1->vz, &vcx);
    DEB_dump_obj__ (Typ_VC, &vcy, "vcy");
  

  //----------------------------------------------------------------
  // add cpTab = ptNr Point-s to memSeg1
  rb1->cpTab = UME_reserve (memSeg1, sizeof(Point) * rb1->ptNr);
  if(!rb1->cpTab) goto L_err1;

  // pt0 = startPt
  pt0 = ci1->p1;

  // pt1 = pt0 + vcy
  UT3D_pt_traptvc (&pt1, &pt0, &vcy);

  // pt2 = ptc - vcx
  UT3D_pt_traptivc (&pt2, &pt1, &vcx);
    
  rb1->cpTab[0] = pt0;
  rb1->cpTab[1] = pt1;
  rb1->cpTab[2] = pt2; 


  //----------------------------------------------------------------
  // add wTab = ptNr weigths (double) to memSeg1
  rb1->wTab = UME_reserve (memSeg1, sizeof(double) * rb1->ptNr);
  if(!rb1->wTab) goto L_err1;

  rb1->wTab[0] = 1.;
  rb1->wTab[1] = r2h;
  rb1->wTab[2] = 1.;


  //----------------------------------------------------------------
  // add kvTab = ptNr+deg+1 knots (double) to memSeg1
  rb1->kvTab = UME_reserve (memSeg1, sizeof(double) * (rb1->ptNr + rb1->deg + 1));
  if(!rb1->kvTab) goto L_err1;
  rb1->kvTab[0] = 0.;
  rb1->kvTab[1] = 0.;
  rb1->kvTab[2] = 0.;
  rb1->kvTab[3] = 1.;
  rb1->kvTab[4] = 1.;
  rb1->kvTab[5] = 1.;
*/
/*
  //================================================================
  // 180-deg:
  rb1->ptNr = 4;
  rb1->v0   = 0.;
  rb1->v1   = 1.;
  rb1->deg  = 2;
  rb1->dir  = 0;   // fwd
  rb1->clo  = 0;   // yes
  rb1->trm  = 1;   // no

  r2h = SR_2 / 2.;

  // vcx = vector pc -> p1
  UT3D_vc_2pt (&vcx, &ci1->pc, &ci1->p1);
    DEB_dump_obj__ (Typ_VC, &vcx, "vcx");

  // vcy = normal to vcx and vz
  UT3D_vc_perp2vc (&vcy, &ci1->vz, &vcx);
    DEB_dump_obj__ (Typ_VC, &vcy, "vcy");
  

  //----------------------------------------------------------------
  // add cpTab = ptNr Point-s to memSeg1
  rb1->cpTab = UME_reserve (memSeg1, sizeof(Point) * rb1->ptNr);
  if(!rb1->cpTab) goto L_err1;

  // pt0 = startPt
  pt0 = ci1->p1;

  // pt1 = pt0 + vcy
  UT3D_pt_traptvc (&pt1, &pt0, &vcy);

  // pt2 = pt1 + (-2 * vcx)
  UT3D_pt_traptmultvc (&pt2, &pt1, &vcx, -2.);

  // pt3 = ptc - vcx
  UT3D_pt_traptivc (&pt3, &ci1->pc, &vcx);

  rb1->cpTab[0] = pt0;
  rb1->cpTab[1] = pt1;
  rb1->cpTab[2] = pt2;
  rb1->cpTab[3] = pt3;


  //----------------------------------------------------------------
  // add wTab = ptNr weigths (double) to memSeg1
  rb1->wTab = UME_reserve (memSeg1, sizeof(double) * rb1->ptNr);
  if(!rb1->wTab) goto L_err1;

  rb1->wTab[0] = 1.;
  rb1->wTab[1] = 0.5;
  rb1->wTab[2] = 0.5;
  rb1->wTab[3] = 1.;


  //----------------------------------------------------------------
  // add kvTab = ptNr+deg+1 knots (double) to memSeg1
  rb1->kvTab = UME_reserve (memSeg1, sizeof(double) * (rb1->ptNr + rb1->deg + 1));
  if(!rb1->kvTab) goto L_err1;
  rb1->kvTab[0] = 0.;
  rb1->kvTab[1] = 0.;
  rb1->kvTab[2] = 0.;
  rb1->kvTab[3] = 0.5;
  rb1->kvTab[4] = 1.;
  rb1->kvTab[5] = 1.;
  rb1->kvTab[6] = 1.;
*/

  //================================================================
  // 360-deg:
  rb1->ptNr = 7;
  rb1->v0   = 0.;
  rb1->v1   = 1.;
  rb1->deg  = 2;
  rb1->dir  = 0;   // fwd
  rb1->clo  = 0;   // yes
  rb1->trm  = 1;   // no

  // vcx = vector pc -> p1
  UT3D_vc_2pt (&vcx, &ci1->pc, &ci1->p1);
    DEB_dump_obj__ (Typ_VC, &vcx, "vcx");

  // vcy = normal to vcx and vz
  UT3D_vc_perp2vc (&vcy, &ci1->vz, &vcx);
    DEB_dump_obj__ (Typ_VC, &vcy, "vcy");
  

  //----------------------------------------------------------------
  // add cpTab = ptNr Point-s to memSeg1
  rb1->cpTab = UME_reserve (memSeg1, sizeof(Point) * rb1->ptNr);
  if(!rb1->cpTab) goto L_err1;

  // pt0 = startPt
  pt0 = ci1->p1;

  // pt3 = ptc - vcx
  UT3D_pt_traptivc (&pt3, &ci1->pc, &vcx);

  // pt1 = pt0 + vcy
  UT3D_pt_traptvc (&pt1, &pt0, &vcy);

  // pt2 = pt3 + vcy
  UT3D_pt_traptvc (&pt2, &pt3, &vcy);

  // pt4 = pt3 - vcy
  UT3D_pt_traptivc (&pt4, &pt3, &vcy);

  // pt5 = pt0 - vcy
  UT3D_pt_traptivc (&pt5, &pt0, &vcy);

  rb1->cpTab[0] = pt0;
  rb1->cpTab[1] = pt1;
  rb1->cpTab[2] = pt2;
  rb1->cpTab[3] = pt3;
  rb1->cpTab[4] = pt4;
  rb1->cpTab[5] = pt5;
  rb1->cpTab[6] = pt0;


  //----------------------------------------------------------------
  // add wTab = ptNr weigths (double) to memSeg1
  rb1->wTab = UME_reserve (memSeg1, sizeof(double) * rb1->ptNr);
  if(!rb1->wTab) goto L_err1;

  rb1->wTab[0] = 1.;
  rb1->wTab[1] = 0.5;
  rb1->wTab[2] = 0.5;
  rb1->wTab[3] = 1.;
  rb1->wTab[4] = 0.5;
  rb1->wTab[5] = 0.5;
  rb1->wTab[6] = 1.;


  //----------------------------------------------------------------
  // add kvTab = ptNr+deg+1 knots (double) to memSeg1
  rb1->kvTab = UME_reserve (memSeg1, sizeof(double) * (rb1->ptNr + rb1->deg + 1));
  if(!rb1->kvTab) goto L_err1;
  rb1->kvTab[0] = 0.;
  rb1->kvTab[1] = 0.;
  rb1->kvTab[2] = 0.;
  rb1->kvTab[3] = 0.25;
  rb1->kvTab[4] = 0.5;
  rb1->kvTab[5] = 0.5;
  rb1->kvTab[6] = 0.75;
  rb1->kvTab[7] = 1.;
  rb1->kvTab[8] = 1.;
  rb1->kvTab[9] = 1.;

  
  //----------------------------------------------------------------
    DEB_dump_obj__ (Typ_CVRBSP, rb1, "ex-UT3D_rbsp_360");

  return 0;


  L_err1:
    TX_Error("UT3D_rbsp_360: out of memspace");
    return -1;

}


//================================================================
// ../ut/ut_txt.c
//================================================================
  
// #include "../ut/ut_umem.h"                     // Memspc


//================================================================
  int UTX_fwrite (char *txt, FILE *fp1) {
//================================================================
// UTX_fwrite              write string into bin.file (fread needs stringlength)

  int      cNr;

  cNr = strlen(txt) + 1;
    // printf("UTX_fwrite |%s| %d\n",txt,cNr);

  fwrite (&cNr, sizeof(int), 1, fp1);
  fwrite (txt, sizeof(char), cNr, fp1);

  return 0;

}


//================================================================
  int UTX_fread (char *txt, int siz, FILE *fp1) {
//================================================================
// UTX_fread                  read string from bin.file (written by UTX_fwrite)
// Input:
//   siz        size of txt
// Output:
//   txt        characters read;
//   retCode    nr of characters read, including terminator;


  int     ii, cNr;


  ii = fread(&cNr, sizeof(int), 1, fp1);
  if(cNr >= siz) return MSG_ERR__ (ERR_EOM, "");

  ii = fread(txt, sizeof(char), cNr, fp1);

    // printf(" ex-UTX_fread %d |%s|\n",ii,txt);

  return ii;

}


//================================================================
// ../xa/mdl__.c
//================================================================

// ../gr/ut_DL.c
extern long       GR_TAB_IND;

#include "../ut/ut_itmsh.h"            // MSHIG_EDGLN_.. typedef_MemTab.. Fac3
#include "../gr/ut_gr.h"               // GR_TMP_HILI


//================================================================
  int MDL_prev__ (char *mnam, int att) {
//================================================================
// MDL_prev__                                preview native model 
// Input:
//   mnam     filename of model to load
//   att      GR_TMP_DEF|GR_TMP_HILI|GR_TMP_DIM;

  int    irc;


  sprintf(mem_cbuf1, "M0=%s", mnam);

  // set temp (preview)
  MDL_prev_stat_set ();

  irc = SRC_cmd_do (mem_cbuf1);

  // hilite last DL-obj M0
  DL_att_mdr (GR_TAB_IND - 1L, att);

  return irc;

}


//================================================================
// ../xa/xa_ui.c
//================================================================


//================================================================
  int UI_ckb_dispPT (int mode) {
//================================================================
// UI_ckb_dispPT               display points also in mode "VWR"
// Input:
//   mode  ON (0)  | OFF (1)
//
// - perm: "MODE DISP_PT ON"
// TODO: update APT_work_MODE

  APT_dispPT = mode;

  GL_InitPtAtt (0);          // reset ptSiz

  return UI_upd_ptDisp ();   // update checkbox

}


//================================================================
  int UI_ckb_dispPLN (int mode) {
//================================================================
// UI_ckb_dispPLN               display planes ON | OFF
// Input:
//   mode  ON (0)  | OFF (1)
//
// - perm: "MODE DISP_PL ON"
// TODO: update APT_work_MODE

  APT_dispPL = mode;

  DL_disp_PL (mode);

  return UI_upd_plDisp ();   // update checkbox

}


//================================================================
  int UI_freeze (int ms, char *msg) {
//================================================================
// UI_freeze                             wait <ms> millisecs; no display-update
// OS_Wait makes static display - no zoom, pan -


  int       i1;


  DL_Redraw ();    // update display - 

  TX_Print (msg);

  GUI_update__ (); // else TX_Print-text not visible

  OS_Wait (ms);  // millisecs

  return 0;

}



//================================================================
// ../xa/xa_src.c
//================================================================




//================================================================
  int SRC_cmd_do (char *src) {
//================================================================
// SRC_cmd_do                          do apt-command; eg "MODE DISP_PT ON"
// - executes command, but does not store command in source.


  // copy - input to WC_Work1 (must be writeable)
  strcpy (mem_cbuf1, src);

  return WC_Work1 (0, mem_cbuf1);

}


//=====================================================================
  int SRC_add_surTP (char *src, int sSiz, int typSu, ...) {
//=====================================================================
// SRC_add_surTP              trimmed-perforated-surf
// Input:
//   typSu      type of surf; eg Typ_PLN
//   typ-dbi    Link to outer-boundary
//   [typ-dbi]  [Link to [n]inner-boundaries]
//
// see also AP_obj_2_txt


  int        sl, typ1;
  long       dbi1;
  char       *s1, oid[80];
  va_list    va;


  printf("---------- SRC_add_surTP %d\n",typSu);
  if(sSiz < 100) goto L_e1;

  s1 = src;



  dbi1 = DB_QueryNxtFree (Typ_SUR, 20);
  APED_oid_dbo__ (oid, Typ_SUR, dbi1);
  sprintf(s1, "%s=",oid);
  sl = strlen(oid) + 1;
  sSiz -= sl;
  s1 = &s1[sl];
    printf("SRC_add_surTP oid |%s|\n",src);


  va_start (va, typSu);


  //----------------------------------------------------------------
  // get next record (oID, src);
  L_nxt:

  typ1 = va_arg (va, int);
  if(typ1 < 0) goto L_end;

  dbi1 = va_arg (va, long);
    printf("SRC_add_surTP typ1=%d dbi1=%ld\n",typ1,dbi1);


  // add obj to src
  APED_oid_dbo__ (oid, typ1, dbi1);
  sprintf(s1, "%s ",oid);
  sl = strlen(oid) + 1;
  sSiz -= sl;
  if(sSiz <= 0) goto L_e1;
  s1 += sl;

    printf("SRC_add_surTP |%s|\n",src);


  // continue with next record (oID, src)
  goto L_nxt;


  //----------------------------------------------------------------
  L_end:
    va_end (va);

      // TESTBLOCK
      printf("ex-SRC_add_surTP |%s|\n",src);
      // END TESTBLOCK

    // add & execute line
    ED_srcLn_add (src, 0);


    return 0;



  //----------------------------------------------------------------
  L_e1:
    return MSG_ERR__ (ERR_internal, "ERR_EOM-1");

}




//================================================================
// ../xa/xa_ed_mem.c
//================================================================

#include "../xa/xa_uid.h"        //  UID_WinMain ...


// ../xa/xa.c
extern int       AP_src;


//================================================================
  int APED_srcLn_del (char *p1, int lLen) {
//================================================================
// APED_srcLn_del                         delete sourcelineNr <lNr>
// see APED_src_chg

  char    cs;

  printf("\n APED_srcLn_del lLen=%d\n",lLen);
  UTX_dump_cnl (p1, 100);


  // deactivate line
  cs = '_';
  UTF_comment_line (p1, 1, &cs);


  return 0;

}


//================================================================
  int APED_update (long lNr) {
//================================================================
// APED_update                    update editor after modification
// Input:
//   lNr      sourceCodeLinenumber where to start update


  // set back to line lNr
  ED_work_CurSet ((int)lNr);

  // rework all following lines
  ED_work_CurSet (UT_INT_MAX);

  // if MAN is active - update editor
  if(AP_src == AP_SRC_EDI) ED_load__ ();

  return 0;

}


//================================================================
  int APED_do__ (char *srcLn, ...) {
//================================================================
// APED_do__                      work n srcLines (perm)
// Input:
//   - one to n srcLines, terminated with NULL;
//
// - DL_Redraw ();   // may be necessary to visualize
// - see also ED_srcLn_add (single line); APED_do_buf1  (many lines)


  int        lNr;
  char       *s1;
  va_list    va;


  // printf("\nAPED_do__ init\n");

  lNr = ED_work_CurSet (-1);         // get active lineNr;

  s1 = srcLn;

  va_start (va, srcLn);


  L_nxt:
      // printf("APED_do__ |%s|\n",s1);
    ED_srcLn_add (s1);

  s1 = va_arg (va, char*);

  if(s1) goto L_nxt;

  va_end (va);


  // set back to line lNr
  ED_work_CurSet (lNr);

  // rework all following lines
  ED_work_CurSet (UT_INT_MAX);

  // if MAN is active - update editor
  if(AP_src == AP_SRC_EDI) ED_load__ ();

    // printf(" ex-APED_do__\n");

  return 0;

}


//================================================================
  int APED_do_buf1 () {
//================================================================
// APED_do_buf1                    add buffer1 to model, update model;

  int        lNr;


  lNr = ED_work_CurSet (-1);         // get active lineNr;

    printf(" APED_do_buf1 - lNr=%d\n",lNr);


  UTF_insert1 (-1L);  // add buffer to model

  // set back to line lNr
  ED_work_CurSet (lNr);

  // rework all following lines
  ED_work_CurSet (UT_INT_MAX);

  // if MAN is active - update editor
  if(AP_src == AP_SRC_EDI) ED_load__ ();

    // printf(" ex-APED_do_buf1\n");

  return 0;

}

#include "../ut/ut_ox_base.h"          // OGX_..

//================================================================
  int APED_src_ln_chg (int typo, long dbio, int formo, void *objo,
                       int typi, long dbii) {
//================================================================
// APED_src_ln_chg        change sourceline <typ,dbi> to src of obj ox1  
// - removes existing sourceline starting with "_<typ><dbi>"
// - create and insert sourceline from inputs typo,formo,dbio,objo
// Input:
//   typi,dbii    deactivated sourcecodeline of this obj
//   typo,formo,dbio,objo   output his as new sourceObj
// Output:
//   irc    <0=error; else lineNr of deactivated sourcecodeline

  int      irc, lLen;
  long     lNr, dli;
  char     *src, sOid[80], *p1, *p2;
  ObjGX    ox1;


  // printf("\n------ APED_src_ln_chg typ1=%d dbii=%ld typo=%d dbio=%ld formo=%d\n",
          // typi, dbii, typo, dbio, formo);
  // UTF_dump__ ("APED_src_ln_chg-in UTF_FilBuf0");  // UTF_FilBuf0


  // get obj-ID; eg "S20"
  irc = APED_oid_dbo__ (sOid, typi, dbii);
  if(irc < 0) return irc;

  // get sourcelineNr of obj <sOid[1]>
  irc = APED_search_defLn (&p1, &lNr, &lLen, sOid, -1L, 0);
  if(irc < 0) return irc;
    // printf(" f-APED_find_dbo-irc=%d lNr=%ld\n",irc,lNr); fflush(stdout);

  // get src = space for mem_cbuf1_SIZ chars
  src = &mem_cbuf1[0];

  // encode curve cv1 into src
  OGX_SET_OBJ (&ox1, typo, formo, 1, objo);
    // DEB_dump_obj__ (Typ_ObjGX, &ox1, "cv");
  irc = AP_stru_2_txt (src, (long)mem_cbuf1_SIZ, &ox1, dbio);
  if(irc < 0) return irc;
    // printf("src-new |%s|\n",src);

  // deactivate line <lNr> (insert '_'), following this line insert new line <src>;
  irc = APED_src_chg (lNr, src);

  // return lineNr of deactivated sourcecodeline
  if(irc >= 0) irc = lNr;



    // TESTBLOCK
    // UTF_dump__ ("ex-APED_src_ln_chg UTF_FilBuf0");  // UTF_FilBuf0
    // dump editor
    // END TESTBLOCK

  return irc;

}


//================================================================
// ../ut/ut_txfil.c
//================================================================
extern char *UTF_FilBuf0;


//================================================================
  int UTF_cpyLn_lPos (char *sOut, char *lPos) {
//================================================================
// UTF_cpyLn_lPos                   copy line starting at lPos
// Output:
//   sOut           must have size mem_cbuf1_SIZ

  int       l1;
  char      *p1;


  p1 = strchr(lPos, '\n');       //printf(" strlen l1=%d\n",l1);

  if(p1) {
    l1 = p1 - lPos;
    if(l1 >= mem_cbuf1_SIZ) return -1;
    memcpy(sOut, lPos, l1);    
    sOut[l1] = '\0';

  } else {  
    if(strlen(lPos) >= mem_cbuf1_SIZ) return -1;
    strcpy (sOut, lPos);
  }
    // printf("ex-UTF_cpyLn_lPos |%s|\n",sOut);

  return l1;

}

 

//================================================================
  char* UTF_get_LnPos (long lNr) {
//================================================================
/// \code
/// UTF_get1_LnPos        find startPos of line from LineNr
/// lNr of first line = 1
/// RetCod:  NULL for EOF or (lNr<1); else first char of line.
/// \endcode


  int     ii;
  char    *lp1, *lp2;


  // printf("UTF_get_LnPos %ld\n",lNr);


  if(lNr < 1) return NULL;

  // get startPos of buffer
  ii = 1;
  lp2 = UTF_FilBuf0;
  if(lp2 == NULL) goto L_done;



  L_nxt_ln:
    if(ii == (int)lNr) goto L_done;
    lp1 = lp2;

    // get end of line
    lp2 = strchr(lp1, '\n');
    if(lp2 == NULL) goto L_done;
    ++lp2;   // skip \n
    ++ii;
    goto L_nxt_ln;


  L_done:
      // printf("ex-UTF_get_LnPos %d |",ii); UTX_dump_cnl (lp2, 40); printf("|\n");
    return lp2;

}



//================================================================
// ../ut/sur__.c                              NEW
//================================================================
 

//================================================================
  int SUR_ox_surTP (ObjGX **oxo, int typSu, Memspc *mSeg, ...) {
//================================================================
// SUR_ox_surTP                     make ObjGX for trimmed-perforated-surf

  int     ii, typ1;
  long    dbi;
  ObjGX   *oxa;
  va_list va;


  printf("---------- cre_ox_surTP %d\n",typSu);


  // get space for oxo
  oxa = (ObjGX*) UME_reserve (mSeg, 2 * sizeof(ObjGX));

  ii = 1;

  va_start (va, mSeg);


  //----------------------------------------------------------------
  // get next record (oID, src);
  L_nxt:

  typ1 = va_arg (va, int);
  if(typ1 < 0) goto L_end;
  dbi = va_arg (va, long);
    printf("SUR_ox_surTP typ1=%d dbi=%ld\n",typ1,dbi);


  // add redord to oxa
  OGX_SET_INDEX (&oxa[ii], typ1, dbi);
  ++ii;


  //----------------------------------------------------------------
  // continue with next record (oID, src)
  goto L_nxt;



  //----------------------------------------------------------------
  L_end:
    va_end (va);

    // 1.obj: define typ of supporting surface (planar)
    OGX_SET_OBJ (&oxa[0], Typ_SUTP, Typ_ObjGX, ii - 1, &oxa[1]);

    *oxo = oxa;


    // TESTBLOCK
    DEB_dump_ox_s_ (*oxo, "ex-SUR_ox_surTP");
    printf(" ex-SUR_ox_surTP\n");
    // END TESTBLOCK

  return 0;

}


//================================================================
// ../ut/ut_obj.c
//================================================================


//=====================================================================
  int UTO_wrf_ogx (FILE *fp1, ObjGX *ox1) {
//=====================================================================
// UTO__wrf_ogx                    write binary complex-obj into file
//   File must be open wb
// Output:
//   oo      startpos of obj read into memSpc
//   memSpc  copy here
//   retCod  0=OK; -1=out-of-mem

  int   irc=0;


  DEB_dump_obj__ (Typ_ObjGX, ox1, "----- UTO_wrf_ogx-in");

  fwrite (ox1, sizeof(ObjGX), 1, fp1);

  // skip obj with no pointers
  if(ox1->form == Typ_Index) goto L_exit;    // Link
  if(ox1->form == Typ_Int4)  goto L_exit;    // Typ_Color
  if(ox1->form == Typ_Typ)   goto L_exit;    // Typ_Typ


  // resolv obj 
  irc = UTO_wrf_obj (fp1, ox1->form, ox1->data, ox1->siz);

  L_exit:

    return irc;

}


//================================================================
  int UTO_wrf_obj (FILE *fp1, int form, void *obj, int oNr) {
//================================================================
// UTO_wrf_obj            write obj into file
//   File must be open wb.
// TODO: surfaces, bodies.
// was UTO__wrf_obj UTO__rdf_obj UTO__wrf_ox UTO__rdf_ox
// see also UTO_wrf_ogx OGX_ox_copy_obj


  int    recSiz, recNr;
  void   *p1;


  printf("------------------ UTO_wrf_obj form=%d oNr=%d\n",form,oNr);
  DEB_dump_obj__(form, obj, "UTO_wrf_obj-in");



  //----------------------------------------------------------------
  switch (form) {


  //----------------------------------------------------------------
  case Typ_VC:
    recSiz = sizeof(Vector);
    goto L_cpy_noPtr;

  //----------------------------------------------------------------
  case Typ_PT:
    recSiz = sizeof(Point);
    goto L_cpy_noPtr;

  //----------------------------------------------------------------
  case Typ_LN:
    recSiz = sizeof(Line);
    goto L_cpy_noPtr;

  //----------------------------------------------------------------
  case Typ_CI:
    recSiz = sizeof(Circ);
    goto L_cpy_noPtr;

  //----------------------------------------------------------------
  case Typ_CVELL:
    recSiz = sizeof(CurvElli);
    goto L_cpy_noPtr;

  //----------------------------------------------------------------
  case Typ_Model:
    recSiz = sizeof(ModelRef);
    goto L_cpy_noPtr;


  //----------------------------------------------------------------
  case Typ_CVPOL:
    if(oNr != 1) goto L_errNr;
    fwrite (obj, sizeof(CurvPoly), 1, fp1);
    // copy cpTab
    fwrite (((CurvPoly*)obj)->cpTab, sizeof(Point), ((CurvPoly*)obj)->ptNr, fp1);
    // copy lvTab
    fwrite (((CurvPoly*)obj)->lvTab, sizeof(double), ((CurvPoly*)obj)->ptNr, fp1);
    break;

  //-------------------------------------------------------
  case Typ_CVBSP:   // see OGX_ox_copy_obj
    if(oNr != 1) goto L_errNr;
    fwrite (obj, sizeof(CurvBSpl), 1, fp1);
    // copy cpTab
    fwrite (((CurvPoly*)obj)->cpTab, sizeof(Point), ((CurvBSpl*)obj)->ptNr, fp1);
    // copy kvTab
    recNr = ((CurvBSpl*)obj)->ptNr + ((CurvBSpl*)obj)->deg + 1;
    fwrite (((CurvBSpl*)obj)->kvTab, sizeof(double), recNr, fp1);
    break;

  //----------------------------------------------------------------
  case Typ_CVRBSP:
    if(oNr != 1) goto L_errNr;
    fwrite (obj, sizeof(CurvRBSpl), 1, fp1);
    // copy cpTab
    fwrite (((CurvRBSpl*)obj)->cpTab, sizeof(Point), ((CurvRBSpl*)obj)->ptNr, fp1);
    // copy kvTab
    recNr = ((CurvRBSpl*)obj)->ptNr + ((CurvRBSpl*)obj)->deg + 1;
    fwrite (((CurvRBSpl*)obj)->kvTab, sizeof(double), recNr, fp1);
    // copy wTab
    fwrite (((CurvRBSpl*)obj)->wTab, sizeof(double), ((CurvRBSpl*)obj)->ptNr, fp1);
    break;

  //----------------------------------------------------------------
  case Typ_CVBEZ:
    if(oNr != 1) goto L_errNr;
    fwrite (obj, sizeof(CurvBez), 1, fp1);
    // copy cpTab
    fwrite (((CurvBez*)obj)->cptab, sizeof(Point), ((CurvBez*)obj)->ptNr, fp1);
    break;


  //----------------------------------------------------------------
  case Typ_CVRBEZ:
    if(oNr != 1) goto L_errNr;
    fwrite (obj, sizeof(CurvRBez), 1, fp1);
    // copy cpTab
    fwrite (((CurvRBez*)obj)->cptab, sizeof(Point), ((CurvRBez*)obj)->ptNr, fp1);
    // copy wTab
    fwrite (((CurvRBez*)obj)->wtab, sizeof(double), ((CurvRBez*)obj)->ptNr, fp1);
    break;

  //----------------------------------------------------------------
  case Typ_Refsys:
    if(oNr != 1) goto L_errNr;
    fwrite (obj, sizeof(Refsys), 1, fp1);
    // copy pln
    fwrite (((Refsys*)obj)->pln, sizeof(Plane), 1, fp1);
    break;

  //-------------------------------------------------------
  case Typ_GTXT:                          // write grafTex
    if(oNr != 1) goto L_errNr;
    fwrite (obj, sizeof(GText), 1, fp1);
    // copy txt
    UTX_fwrite (((GText*)obj)->txt, fp1);
    break;

  //----------------------------------------------------------------
  case Typ_ATXT:
    if(oNr != 1) goto L_errNr;
    fwrite (obj, sizeof(AText), 1, fp1);
    // copy txt
    UTX_fwrite (((AText*)obj)->txt, fp1);
    break;

  //----------------------------------------------------------------
  case Typ_SubModel:                    // write basic model
    if(oNr != 1) goto L_errNr;
    fwrite (obj, sizeof(ModelBas), 1, fp1);
    // copy txt
    UTX_fwrite (((ModelBas*)obj)->mnam, fp1);
    break;

  //----------------------------------------------------------------
  case Typ_ObjGX:
    if(oNr != 1) goto L_errNr;
    UTO_wrf_ogx (fp1, (ObjGX*)obj);
    break;

  //----------------------------------------------------------------
  default:
    TX_Error("UTO_wrf_obj E002 - unsupported struct %d\n",form);
    goto L_err_ex;
  }

  goto L_exit;


  //----------------------------------------------------------------
  L_cpy_noPtr:
    // write struct with no pointers
    fwrite (obj, recSiz, oNr, fp1);


  //================================================================
  L_exit:

    // TESTBLOCK
    // printf("ex-UTO_wrf_obj\n");
    // END TESTBLOCK

  return 0;


  //================================================================
  L_errM:
    TX_Error("UTO_wrf E001 - EOM\n");
    goto L_err_ex;

  L_errNr:
    TX_Error("UTO_wrf E002 - mult.objs not supp.\n");

  L_err_ex:
    return -1;

}


//=====================================================================
  int UTO_rdf_ogx (void **oo, Memspc *memSpc, FILE *fp1) {
//=====================================================================
// UTO__rdf_ogx                    read binary complex-obj from file
//   File must be open rb
// Output:
//   oo      startpos of obj read into memSpc
//   memSpc  copy here
//   retCod  0=OK; -1=out-of-mem


  int     irc, ii;

  // reserve space
  *oo = UME_reserve (memSpc, sizeof(ObjGX));
  if(*oo == NULL) MSG_ERR__ (ERR_EOM, "E1");

  // read primary obj
  ii = fread(*oo, sizeof(ObjGX), 1, fp1);
  if(ii != 1) {TX_Error("UTO_rdf_ogx E1"); return -1;}
    printf(" UTO_rdf_ogx- typ=%d form=%d siz=%d\n",
           ((ObjGX*)*oo)->typ,((ObjGX*)*oo)->form,((ObjGX*)*oo)->siz);


  // skip obj with no pointers
  if(((ObjGX*)*oo)->form == Typ_Index) goto L_exit;
  if(((ObjGX*)*oo)->form == Typ_Int4) goto L_exit;
  if(((ObjGX*)*oo)->form == Typ_Typ) goto L_exit;


  // get obj to data
  irc = UTO_rdf_obj (&((ObjGX*)*oo)->data, memSpc,
                     ((ObjGX*)*oo)->form, ((ObjGX*)*oo)->siz, fp1);


  L_exit:
    DEB_dump_obj__ (((ObjGX*)*oo)->form, ((ObjGX*)*oo)->data, "  rdf_ogx-4");
    DEB_dump_obj__ (Typ_ObjGX, *oo, "  rdf_ogx-5");
    
  return irc;

}


//=====================================================================
  int UTO_rdf_obj (void **oo, Memspc *memSpc,
                    int recTyp, int recNr, FILE *fp1) {
//=====================================================================
// UTO__rdf_obj                    read binary obj from file
//   File must be open rb
// Output:
//   oo      startpos of obj read into memSpc
//   memSpc  copy here
//   retCod  >0 = objTyp in oo; 
//           <=0 = Error;


  int    irc, ii, osiz, recSiz;
  long   l1;
  char   tmpSpc[4096];


  printf("UTO__rdf_obj recTyp=%d recNr=%d\n",recTyp,recNr);

  recSiz = UTO_siz_stru (recTyp);  // form;
  if(recSiz <= 0) return -1;
    // printf("  rdf_obj recSiz=%d\n",recSiz);

  osiz = recNr * recSiz;

  // reserve space
  *oo = UME_reserve (memSpc, osiz);
  if(*oo == NULL) MSG_ERR__ (ERR_EOM, "E1");

  // read primary obj
  ii = fread(*oo, recSiz, recNr, fp1);
  if(ii != recNr) {TX_Error("UTO__rdf_obj E2"); return -1;}



  //----------------------------------------------------------------
  switch (recTyp) {

    case Typ_VC:
    case Typ_PT:
    case Typ_LN:
    case Typ_CI:
    case Typ_CVELL:
    case Typ_CVCLOT:
    case Typ_CVTRM:
    case Typ_Val:
    case Typ_Model:
      break;  // no pointers;


  //----------------------------------------------------------------
  case Typ_CVPOL:
    if(recNr != 1) goto L_errNr;
    // get cpTab
    irc = UTO_rdf_obj ((void**)&(((CurvPoly*)*oo)->cpTab), memSpc,
                       Typ_PT, ((CurvPoly*)*oo)->ptNr, fp1);
    if(irc < 0) return irc;
    // copy lvTab
    irc = UTO_rdf_obj ((void**)&(((CurvPoly*)*oo)->lvTab), memSpc,
                       Typ_Val, ((CurvPoly*)*oo)->ptNr, fp1);
    if(irc < 0) return irc;
    break;


  //----------------------------------------------------------------
  case Typ_CVBSP:
    if(recNr != 1) goto L_errNr;
    // get cpTab
    irc = UTO_rdf_obj ((void**)&(((CurvBSpl*)*oo)->cpTab), memSpc,
                       Typ_PT, ((CurvBSpl*)*oo)->ptNr, fp1);
    if(irc < 0) return irc;
    // copy kvTab
    ii = ((CurvBSpl*)*oo)->ptNr + ((CurvBSpl*)*oo)->deg + 1;
    irc = UTO_rdf_obj ((void**)&(((CurvBSpl*)*oo)->kvTab), memSpc,
                       Typ_Val, ii, fp1);
    if(irc < 0) return irc;
    break;


  //----------------------------------------------------------------
  case Typ_CVRBSP:
    if(recNr != 1) goto L_errNr;
    // get cpTab
    irc = UTO_rdf_obj ((void**)&(((CurvRBSpl*)*oo)->cpTab), memSpc,
                       Typ_PT, ((CurvRBSpl*)*oo)->ptNr, fp1);
    if(irc < 0) return irc;
    // copy kvTab
    ii = ((CurvBSpl*)*oo)->ptNr + ((CurvRBSpl*)*oo)->deg + 1;
    irc = UTO_rdf_obj ((void**)&(((CurvRBSpl*)*oo)->kvTab), memSpc,
                       Typ_Val, ii, fp1);
    if(irc < 0) return irc;
    // copy wTab
    irc = UTO_rdf_obj ((void**)&(((CurvRBSpl*)*oo)->wTab), memSpc,
                       Typ_Val, ((CurvRBSpl*)*oo)->ptNr, fp1);
    if(irc < 0) return irc;
    break;


  //----------------------------------------------------------------
  case Typ_CVBEZ:
    if(recNr != 1) goto L_errNr;
    // get cpTab
    irc = UTO_rdf_obj ((void**)&(((CurvBez*)*oo)->cptab), memSpc,
                       Typ_PT, ((CurvBez*)*oo)->ptNr, fp1);
    if(irc < 0) return irc;
    break;


  //----------------------------------------------------------------
  case Typ_CVRBEZ:
    if(recNr != 1) goto L_errNr;
    // get cpTab
    irc = UTO_rdf_obj ((void**)&(((CurvRBez*)*oo)->cptab), memSpc,
                       Typ_PT, ((CurvRBez*)*oo)->ptNr, fp1);
    if(irc < 0) return irc;
    // copy wTab
    irc = UTO_rdf_obj ((void**)&(((CurvRBez*)*oo)->wtab), memSpc,
                       Typ_Val, ((CurvRBez*)*oo)->ptNr, fp1);
    if(irc < 0) return irc;
    break;


  //----------------------------------------------------------------
  case Typ_Refsys:
    if(recNr != 1) goto L_errNr;
    // copy pln
    irc = UTO_rdf_obj ((void**)&(((Refsys*)*oo)->pln), memSpc,
                       Typ_PLN, 1, fp1);
    if(irc < 0) return irc;
    break;


  //----------------------------------------------------------------
  case Typ_GTXT:                     // read grafTex
    if(recNr != 1) goto L_errNr;
      // DEB_dump_obj__ (Typ_GTXT, (GText*)*oo, "tmpSpc-1");
    // copy txt
    ii = UTX_fread (tmpSpc, sizeof(tmpSpc), fp1);
    ((GText*)*oo)->txt = UME__copy (memSpc, NULL, tmpSpc, ii);
    break;


  //----------------------------------------------------------------
  case Typ_ATXT:
    if(recNr != 1) goto L_errNr;
    // copy txt
    ii = UTX_fread (tmpSpc, sizeof(tmpSpc), fp1);
    ((AText*)*oo)->txt = UME__copy (memSpc, &l1, tmpSpc, ii);
    break;


  //----------------------------------------------------------------
  case Typ_SubModel:                    // read basic model
    if(recNr != 1) goto L_errNr;
    // copy txt
    ii = UTX_fread (tmpSpc, sizeof(ModelBas), fp1);
    ((ModelBas*)*oo)->mnam = UME__copy (memSpc, &l1, tmpSpc, ii);
    break;


  //----------------------------------------------------------------
  default:
    TX_Error("UTO_rdf_obj - unsupported struct %d\n",recTyp);
    return -1;
  }


  return recTyp;

 
  //----------------------------------------------------------------
  L_errNr:
    TX_Error("UTO_rdf_obj - mult.objs not supp. for %d\n",recTyp);
    return -2;

}



// EOF
