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

// ../ci/NC_Main.c
extern int APT_dispPT;
extern int APT_dispPL;


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


// EOF
