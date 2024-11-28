//     xa.c          AP_search_init
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
- AP_mod_defSiz should be modifyable; extra menu in Standards/Tolerances ?
- Die memspc-Felder erst beim ProgStart mallocen
- statt UI_ckb_Names UI_AP; gleich im ci oder erst hier ??

-----------------------------------------------------
Modifications:
2005-06-27 xa_main.c abtrennen (wegen batch). RF.
2002-11-17 Neuer name gCAD3D
2002-08-01 AP_obj_2_apt -> xa_obj_txt.c. RF.
2001-06-06 Trennen in xa_app und xa_ui.
2000-05-01 Neu ex gcad; erster Entwurf vom 1.8.98. RF.

-----------------------------------------------------
*/
#ifdef globTag
 void AP(){}
#endif
/*!
\file  ../xa/xa.c
\brief main application functions; not depending on GUI.
\code
=====================================================
List_functions_start:

AP_init_xa_h          init global variables of ../xa/xa.c
AP_MemTab_init        connect static memspace eg memspc251 - MemTab_spc251
AP_MemTab_get         occupy static - MemTab
AP_MemTab_rel         release static - MemTab

AP_fnam_get_user_2          open file, with directorySelect, filter, waiting.
AP_fnam_get_user_1           open file, with directorySelect, filter, waiting.

AP_get_modnam         returns AP_mod_fnam
AP_get_dir_open       returns AP_mod_dir
AP_get_modact_ind     returns AP_modact_ibm
AP_set_dir_open       set AP_mod_dir and AP_mod_sym from fnam
AP_set_modsiz

AP_stat_file          save AP_box_pm1,2 AP_stat.. 
AP_errStat_set        set AP_stat.errStat
AP_errStat_get        get AP_stat.errStat
AP_tutStat_get        get AP_stat.TUT_stat
AP_debug__            stop in debug
AP_test__             "Ctl shift T"
AP_dump_statPg        dump active subModel, active lineNr

// AP_mdl_modified_ck      check if model is modified
// AP_mdl_modified_set     set model is modified
// AP_mdl_modified_reset   set model is unmodified
AP_mdlbox_invalid_ck  check if modelbox is valid
AP_mdlbox_invalid_set set modelbox not valid
AP_mdlbox_invalid_reset set modelbox = valid

AP_Mod_load_init      load Model from file <fn> and create <tmp>/Mod_.mod_in
AP_Mod_load_fn        load Model from file <fn>
AP_mod_sym            get get symbol-name for new directory from user
AP_iftyp_ftyp         integer-filetyp from string-filetyp
AP_ftyp_iftyp         string-filetyp from integer-filetyp
AP_delActMdl          delete active modelfile
AP_Mod_lstAdd         add active model to list <tmpDir>MdlLst.txt
AP_Mod_lst_fn         add filename to list <tmpDir>MdlLst.txt
AP_mod_sav_ok         rename Mod_.mod_out -> Mod_.mod_in

AP_ck_build           test if reBuild wanted, test if compiler exists 
AP_plu_start          get plugin-name from user; start plugin ..
AP_plu_restart        restart plugin - DLL
AP_plu_exec           start plugin - DLL
AP_kex_exec           start gcad-kernel-extension

AP_APT_sysed          modify line ED_lnr_act with system-editor
AP_SRC_mem_edi        copy Editor --> memory (if necessary)
AP_SRC_edi_mem        copy memory --> Editor

AP_src_init         write empty file tmp/Model_
AP_mdl_init          clear src-Memory, reset Undo, Hide, View-Plane, ConstrPlane
AP_src_mod_ed       modify line in memory with system-editor

AP_APT_clean        remove deleted lines
AP_obj_delCk1       check if line has depending lines

AP_cre_defHdr       create new free ObjHeader
AP_cut_defHdr       extract header

AP_lNr_inc          increment AP_ED_lNr
AP_lNr_get          returns AP_ED_lNr
AP_lNr_set

AP_typDB_typ        give basictyp from typ (SURRU -> Typ_SUR; Typ_CVBSP -> Typ_CV)
AP_cmp_typ          check if types identical;
AP_typ_typChar      make typ from typChar  ("P" -> Typ_PT)
AP_typChar_typ      make typChar from typ  (Typ_PT -> 'P')

AP_vec_txt          give vector from Textvec
AP_get_nxtVec       returns DB-index of next|previous Vector

AP_Set_ConstPl_Z    set WC_sur_txt = constrPln (eg "RZ"), disp
AP_Get_ConstPl_Z    returns ConstrPln or its Z-vec
AP_Get_ConstPl_vz   returns WC_sur_act.vz
AP_Get_scale        AP_scale
AP_Set_scale        AP_scale
AP_impTyp_get       get impTyp UID_ckb_impTyp (CAD-Model or mockup)

AP_PT2EyeBp
AP_PT2EyePln

ACT_ck_act          check and activate activity.

//--- replacing functions GR_cre* --------------------------------
AP_add_pt             add point permanent into DB, add obj into DL, display
AP_add_ln             add line permanent into DB, add obj into DL, display

AP_SetCol__         set default color
APcol_actColTra     set new active colour and transparency
APcol_actCol__      set new active colour
APcol_defCol_3i     set color
APcol_defCol__      select color
COL_INT32           get *ColRGB from *int                                    INLINE
COL_DL_ATT          get *ColRGB from *DL_Att                                 INLINE
COL_ISTYL           set style, symbolic|shaded                               INLINE
UTcol__3db          colour from 3 doubles
UTcol__3i           create colour from red,green,blue
UTcol_dump          dump ColRGB

AP_sel_oid__        get objID(s) from text, hilite, add to grp
AP_DllLst_write     write list of plugins

AP_sav_as           File / save as ".gcad"
AP_sav_qs           quicksav (Ctrl-s)
AP_save__           save model group or subModel

AP_work__           work startparameters
AP_defLoad          load DefaultModel
AP_Init1
AP_Init_planes
AP_exit__           exit app
AP_defaults_write   defaults -> tmp/xa.rc
AP_defaults_dir     defaultdirs -> xa/dir.lst
AP_defaults_read    read defaults aus <base>/tmp/xa.rc
AP_Get_Setup        read line out of gCAD3D.rc
AP_symDirSel        select directory-path-group (file cfg_<os>/dir*.lst)

CONSTRPLN_IS_ON            test if 2D is ON (if ConstructionPlane is active)
CONSTRPLN_IS_OFF            test if 3D is ON (no user-defined ConstructionPlane is active)

MDL_IS_MAIN         check if the active model is the mainmodel               INLINE

List_functions_end:
=====================================================
UU:
AP_Mod_load__       load Model <AP_mod_dir><AP_mod_fnam REPLACED by MDL_imp__
AP_save_ex          save model at exit model            REPLACED GUI_file_over
AP_save_del_smuu    ask for / delete unused subModels   REPLACED by MDL_file_smuu

\endcode *//*----------------------------------------

main() is in xa_main.c


--------------------------------------------------------
Modus Viewer oder CAD oder MAN
  // ex xa_ui.c:
  extern int  UI_InpMode;      // UI_MODE_VWR=1, UI_MODE_CAD=3, UI_MODE_MAN=2
  (was UI_ask_mode ();)


Sourcedaten entweder im memory oder im Editor je nach
 if(AP_src == AP_SRC_MEM/AP_SRC_EDI)
 // Modus Viewer und CAD sind AP_SRC_MEM, Modus MAN ist AP_SRC_EDI.



Editor-Funktionen sind in den Files:
../xa/xa_edi__.c
../xa/xa_ed.c
../xa/xa_cad_ed.c
../xa/xa_ed_mem.c


Memory-Funktione sind in den Files:
  ut_txfil.c       UTF_*                           allg.Mem.Src.funcs
  xa.c             AP_src_* AP_obj_*


---------------------------------------------------------------------
UCS UserCoordinatesystem Refsys ConstrPlane:
---------------------------------------------------------------------
see also ../dox/ProgramFlow.dox \section ConstructionPlane

Das aktive RefSys kann nur in MAN oder CAD gesetzt werden;
  es wird durch ein rotes Achsenkreuz dargestellt.
  Das absolute Achsensystem wird durch ein graues Achsenkreuz dargestellt.
  Das aktuelle RefSys wird in der Liste links-oben als ConstrPln angezeigt.
  WC_sur_act     is the active construction-plane
  WC_sur_mat     is its transformation-matrix

Coordinates: 
  in modelSource not transformed;
  in DB transformed.

Wann und wo wird transformiert ?
  in den APT_decode_functions; zB via UT3D_pt_tra_pt_m3 (., WC_sur_mat, ..
  APT_decode_ausdr() provides WCS-coords; transf. is done in APT_decode_pt().
    (from eg P20=P(1 2 3))

In der DisplayListe wird der RefsysIndex for alle Objekte
  im DL_Att[].irs gespeichert.
Aktivation of a Refsys makes a DL-Record typ=Typ_apDat=93.


Plane     WC_sur_act;    the active ConstructionPlane; in xa.c
int       WC_sur_ind;    DB-Index of the ActiveConstrPlane; 0=none.
double    WC_sur_Z;      the active Z-Offset to the ConstructionPlane
char WC_sur_txt[16];   displayed name_of_Constr.Plane; is "DZ" or "R20"

Mat_4x3   WC_sur_mat;            // TrMat of ActiveConstrPlane
Mat_4x3   WC_sur_imat;           // inverse TrMat of ActiveConstrPlane


UNUSED, aber noch ncht entsorgt: APT_2d_to_3d_Ind
APT_2d_to_3d_mode = ON/OFF;                               now WC_sur_ind
APT_2d_to_3d_Mat                                          now WC_sur_mat
APT_2d_to_3d_Pln                                          now WC_sur_act


Functions ConstrPlane:
DL_setRefSys
  NC_setRefsys
DL_GetTrInd


=========================================================
*/


// #if defined _MSC_VER || __MINGW64__
// #include <windows.h>
// #endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>              // islower ..
#include <stdarg.h>             // va_list


// Achtung: braucht mit w32 __pctype u ___mb_cur_max / unresolvbar !
//#include <ctype.h>


//#include <GL/gl.h>
//#include <GL/glu.h>


#include "../ut/ut_cast.h"        // PTR_INT
#include "../ut/ut_geo.h"         // OFF, ON ..
#include "../ut/ut_txt.h"         // fnam_del
#include "../ut/ut_txfil.h"       // UTF_add1_file
#include "../ut/ut_err.h"         // ERR_SET1
#include "../ut/ut_os.h"          // AP_get_bas_dir
#include "../ut/ut_memTab.h"           // MemTab
#include "../ut/ut_itmsh.h"            // MSHIG_EDGLN_.. typedef_MemTab.. Fac3
#include "../ut/gr_types.h"            // SYM_* ATT_* LTYP_*
#include "../ut/func_types.h"          // SYM_CROSS
#include "../ut/ut_txTab.h"            // TxtTab
#include "../ut/ut_deb.h"              // DEB_*
#include "../ut/os_dll.h"      // DLL_LOAD_only DLL_CONNECT DLL_EXEC ..

#include "../gui/gui__.h"              // GUI_SETDAT_EI ..
#include "../gui/gui_types.h"          // UI_Func..
#include "../gr/ut_gr.h"               // GR_TMP_I0
#include "../db/ut_DB.h"               // DB_PLX_IND

#include "../xa/xa_uid.h"              // UI_MODE_CAD
#include "../xa/xa_ui.h"               // UI_men__
#include "../xa/xa_sele.h"             // Typ_go*
#include "../xa/xa_msg.h"              // MSG_cancel,

#define extern              // damit wird "extern" im Includefile geloescht !
#include "../xa/xa.h"       // AP_STAT AP_mod_fnam ..
#include "../xa/xa_mem.h"   // memspc..
#undef extern               // reset extern ..



#define SIZFNam    400         // max size full filename 
// also in ../xa/mdl__.h




//============ Extern Var: =====================
// ex ../xa/xa_ui.c:
extern int       UI_InpMode;
extern int       UI_block_inp;              // 0=open, 1=blocked
// extern int       UI_InpSM;
// extern void *ckb_cad, *ckb_man;

// ex xa_ed.c:
extern int ED_lnr_act;

// ex ../ci/NC_Main.c:
// extern int       APT_2d_to_3d_Ind; // Achsensystem; 0=Hauptachsensystem
// extern Point2    GR_ptArr1[500];
// extern Point2    GR_ptArr2[500];
extern int        APT_lNr;
extern int        UP_level;
extern int        APT_obj_stat;
extern double  APT_ModSiz;

// ex xa_ui.a
extern char UI_fnamFilt[80];

// ex ../xa/xa.c:
extern AP_STAT   AP_stat;




//============ Locale Var: =======================================

// Vars fuer spezielle Verwendung:
  // int       aus_anz;
  int       SRC_ato_anz;

/// \code
/// used for objTypes in APT_decode_ausdr
/// in WC_Work1: GLT_cta;
/// in IE:       memspc55   
/// \endcode
  // int       *aus_typ=NULL;
  int       *SRC_ato_typ=NULL;

/// \code
/// used for objData in APT_decode_ausdr
/// in WC_Work1: GLT_pta;
/// in IE:       memspc54
/// \endcode
  // double    *aus_tab=NULL;
  double    *SRC_ato_tab=NULL;

/// size of aus_typ & aus_tab
  // int       aus_SIZ;
  int       SRC_ato_SIZ;


char      AP_filnam[256];


int       AP_mode__;             // see AP_mode_step
int       AP_mode_old;

int       AP_src = AP_SRC_EDI;   ///< AP_SRC_MEM or AP_SRC_EDI


// Vars fuer die Applikation:
// char      AP_mod_fnam[SIZMFNam];     ///< active Modelname - without path
// char      AP_modact_nam[128];     ///< name of the active submodel; def=""(main)

Point     WC_mod_ori;            ///< der Model-Origin
Plane     WC_sur_act;            ///< the active construction-plane
int       WC_sur_ind = 0;        ///< Der Index auf die ActiveConstrPlane
Mat_4x3   WC_sur_mat;            ///< TrMat of ActiveConstrPlane
                                 ///<   only if (WC_sur_ind > 0)
Mat_4x3   WC_sur_imat;           ///< inverse TrMat of ActiveConstrPlane
char      WC_sur_txt[16];      ///< oid ConstrPl (DZ, ...R20 ...) displayed

double    AP_txsiz    = 3.5;     ///< Notes-Defaultsize
double    AP_txdimsiz = 2.5;     ///< Dimensions-Text-Defaultsize
int       AP_txNkNr   = 2;       ///< Nachkommastellen
ColRGB    AP_actcol;             ///< the active color ..
ColRGB    AP_defcol;             ///< die DefaultColor des aktiven (sub)Model
// int       AP_indCol;             ///< der DL-Index der aktuellen defCol

int       WC_stat_bound = OFF;   ///<  ON OFF; Draw boundary of Fl�chen J/N
/*

// double    APT_min_dist, WC_ModSiz=300.;


// UNUSED !? see also APT_2d_to_3d_Ind
// int       APT_3d_mode = ON;                //  Inputmode 3D oder 2D
// int       APT_2d_to_3d_mode = OFF;         //  2D -> 3D (PLANE ...)
// Mat_4x3   APT_2d_to_3d_Mat;
// Plane     APT_2d_to_3d_Pln;


//  "NOPICK" setzt GR_pick = ON und alle nun generierten Elem. auf NOPICK.
// int       GR_pick = OFF;


// int       GR_Att_act = 0;
// int       GR_lay_act = 0;

//  DISP_AC - Toleranz (Sehnenfehler bei der Darstellung von Arcs.
// double    APT_TOL_ac    = 0.1;
// UNUSED; gesetzt durch MODE DISP_AC
*/


UtxTab_NEW (AP_TxTab1);                // stringtable


int    AP_argNr;
char **AP_argv;

double AP_scale;

// long      AP_ED_lNr=0;      ///< die aktuelle Line# im Edi.
int       AP_ED_cPos=0;     ///< die aktuelle CharPos im Edi.
char      AP_ED_oNam[128];   ///< objectName of active Line


// MemTab MemTab_spc251 = _MEMTAB_NUL;


// active kernel-extension-dll
void *kex_dll = NULL;



//========= Prototypen: =============================================
  int AP_typ_typChar (char typChar);
  int AP_tmr_CB__ (void *data);



//================================================================
//================================================================

//================================================================
  int AP_init_xa_h () {
//================================================================
// AP_init_xa_h          init global variables of ../xa/xa.c

  // AP_fVwr = NULL;

  return 0;

}



/* replaced by OS_exe_file__
//================================================================
  int AP_GUI__ (char *guiOut, int outSiz, char *exenam, ...) {
//================================================================
// AP_GUI__                        start GUI_exe with parameters
//
// gui-exe    par1
// GUI_file - open      get filenam from user for open (see GUI_file_open__
// GUI_file - save      get filenam from user for save (see GUI_file_save__I
// GUI_dlg1 - info      infoText + close-button, not waiting (see also MsgBox)
// GUI_dlg1 - list1     get selection from list  (see GUI_listf1__)
// GUI_dlg1 - dlgbe     infoText, 1-10 buttons, optional entryField
//                      see also GUI_dlg_2b GUI_dlg_e2b
//
// Input:
//   outSiz    size of output-string guiOut
//   exenam    filename of exe, without gtk-version; eg "GUI_file" or "GUI_dlg1"
//   ...       parameters, must end with a NULL; 
//             Parameters with blanks must be limited with \"
// 
//   parameters for exenam = GUI_file - open or save
//           1 open | save
//           2 outDir/outfilename
//           3 filename of symbolic-directories
//           4 filterText  (eg "*" or "*.jpg" - only one filetyp)
//           5 window-title
//           6 window-size x and y; eg "\"x40,y30\""
//
//   parameters for exenam = GUI_dlg1 - info:
//             infoText  (\n does NOT work with MS-Win)
//
//   parameters for exenam = GUI_dlg1 - list1:
//             list1
//             filename of file with all childs of list
//             window-title            
//
//   parameters for exenam = GUI_dlg1 - dlgbe:
//             dlgbe
//             infotext  (\n does NOT work with MS-Win)
//             1-10 buttons (text on button)
//               optional: entryfield
//             --ent \"entPreset\" 16    // text in entry, size of entry in chars
//
// Output:
//   guiOut    output-string
//             guiOut of GUI_file - open / save
//               full filename; empty for Cancel;
//               guiOut of GUI_dlg1 - list1
//               full text of selected line; empty for Cancel;
//             guiOut of GUI_dlg1 - dlgbe
//               empty = Cancel
//               first char is index of selected button; 0=first ..
//               string of a given entryFieled starts at second character
//   retCode   -1   exe with name <exenam> not found
//             -2   error in GUI_<exe>; see /tmp/debug.dat
//
// Examples: see AP_GUI_test1() ../APP/GUI_file.sh ../APP/GUI_dlg1.sh
//
// see INF_GUI_exe
// see also GUI_MsgBox

#define siz_sPar 1600

//   int      irc, ii;
//   char     sPar[siz_sPar], sEnam[SIZFNam], sCmd[siz_sPar+siz_sPar],
//            fnTmp[400], s1[256], *sx;
//   va_list  va;

  int   irc;
  char  fnTmp[SIZFNam], s2[2048], *p1;
  FILE  *fpo;
  va_list va;


  printf("AP_GUI__ |%s|%s| %d\n",guiOut,exenam,outSiz);
  printf("AP_GUI__ |%s| %d\n",exenam,outSiz);


  //----------------------------------------------------------------
  // create filename outputFile
  GUI_file_in (fnTmp);


  //----------------------------------------------------------------
  // write parameters into outFile fnOut
  if((fpo=fopen(fnTmp,"w")) == NULL) {
    printf("***** Error OS_exe__ Open %s\n",fnTmp);
    exit(1);
  }

  va_start (va, exenam);

  L_nxt_arg:
     p1 = (void*)va_arg (va, char*);
     if(p1) {
       fprintf(fpo, "%s\n",p1);
       goto L_nxt_arg;
     }
  va_end (va);

  fclose(fpo);


  //----------------------------------------------------------------
  // call "exe <inputFileName>"
  sprintf(s2, "%s %s", exenam, fnTmp);   // Linux
  // sprintf(s2,  "CMD /C \"\"%s\" \"%s\"\"",fnExe,fnTmp);
    printf(" AP_GUI__-s2 |%s|\n",s2);

  irc = OS_system (s2);
  if(irc) {printf("***** AP_GUI__ - Error OS_system %d\n",irc); return -1;}

  // remove inputFile of exe
  OS_file_delete (fnTmp);

  // create outFilnam of exe
  GUI_file_out_in (fnTmp, fnTmp);

  // read output of exe (in outFilnam)
  irc = UTX_fgetLine (guiOut, outSiz, fnTmp, 1);
    printf("AP_GUI__-out %d |%s|\n",irc,guiOut);

  // remove outFile of GUI_dlg1
  OS_file_delete (fnTmp);

    printf(" ex-P_GUI__ %d |%s|\n",irc,guiOut);

  return irc;


/
//   // get full filename for GUI_executable
//   irc = AP_GUI_get (sEnam, exenam);
// 
//   // test if exe exists
//   if(!OS_checkFilExist(sEnam,1)) {
//     TX_Print("**** GUI-exefile %s does not exist ..", sEnam);
//     return -1;
//   }
// 
//   // execute
// /

/
//   sPar[0] = '\0';
//   // strcpy (sPar, "\"");
// 
//   va_start (va, exenam);
// 
//   // add all parameters -> sPar
//   ii = 0;
//   L_s_nxt:
//     sx = va_arg(va, char *);
//     if(sx) {
//         printf(" AP_GUI__-nxt |%s|\n",sx);
//       // MSYS: "*" is expande in all versions; do NOT skip ""
//       if(strlen(sx) == 0) {strcat(sPar, " \"\" "); goto L_s_nxt;}
//       // enclose between quotes if containes a blank and add a blank
//       UTX_cp_str_quotes (s1, sx, " ");
//       ii += strlen(s1);
//       if(ii > siz_sPar) {TX_Print("**** AP_GUI__ E-parSiz"); return -2;}
//       strcat(sPar, s1);
//       // strcat(sPar, " ");     // delimit with a blank
//       goto L_s_nxt;
//     }
// 
//   va_end (va);
// 
//   // filename outputFile
//   sprintf (fnTmp, "%stemp_%d",AP_get_tmp_dir(),rand());
// 
//     printf(" AP_GUI__-sEnam |%s|\n",sEnam);   // exenam
//     printf(" AP_GUI__-sPar  |%s|\n",sPar);    // parameters
//     printf(" AP_GUI__-fnTmp |%s|\n",fnTmp);   // outputFile
// 
// 
// 
//   // set s1 = command (exefilnam parameters)
//   // parameters: save outDir symDirFilnam filter title
// #if defined _MSC_VER || __MINGW64__
//   sprintf(sCmd,"CMD /C \"\"%s\" %s \"%s\"", sEnam, sPar, fnTmp);
// #else
//   sprintf(sCmd,"%s %s %s", sEnam, sPar, fnTmp);
// #endif
//     printf(" AP_GUI__ do |%s|\n",sCmd);
// 
//   // execute
//   irc = OS_system (sCmd);
//   if(irc) {printf("***** AP_GUI__ - E1\n"); return -2;}
// 
//   // info does not have outFile
//   if(!strncmp (sPar, "info", 4)) goto L_exit;
// 
//   // read file <tmp>/tmp_<iRnd>, delete file, return filecontent:
//   irc = UTX_fgetLine (guiOut, outSiz, fnTmp, 1);
//   if(irc) {printf("***** AP_GUI__ - E2-%d\n",irc); return -3;}
//   OS_file_delete (fnTmp);
// 
//   // UTX_CleanCR (guiOut);
/

  L_exit:
    printf("ex-AP_GUI__ |%s|\n",guiOut);
  return 0;

}
*/


//================================================================
  int AP_symDirSel () {
//================================================================
// AP_symDirSel        select directory-path-file (file cfg/dir*.lst)
// - select <cfgDir>/dir*.lst and copy -> <cfgDir>/dir.lst
// - set AP_symDir_fnam = new active directory-path-file


  int    irc, fNr, ii;
  char   fn1[400], fn2[400], s1[256];
  TxtTab fnLst = _UTXTAB_NUL;


  // printf("AP_symDirSel \n");


  // - list all dir_*.lst but exclude dir.lst
  UtxTab_init__ (&fnLst);
  fNr = UTX_dir_list__ (&fnLst, AP_get_cfg_dir(), "dir", ".lst", 0);

    // TESTBLOCK
    // UtxTab_dump (&fnLst, " _lst_mshi-fnLst1");
    // END TESTBLOCK

//   if(fNr > 0) {
//     // remove dir.lst
//     ii = UtxTab_find ("dir.lst", &fnLst);
//     if(ii >= 0) UtxTab_del (ii, &fnLst);
//   }

  if(fNr < 1) {
    TX_Print("***** no file with symbolic directories exist.");
    sprintf(fn1, "%sdir_test.lst",AP_get_cfg_dir());
    TX_Print("***** you can create eg %s ..",fn1);
    goto L_exit;
  }

    // TESTBLOCK
    // UtxTab_dump (&fnLst, " _lst_mshi-fnLst2");
    // END TESTBLOCK


  // write list -> file
  sprintf(fn1, "%stemp.lst",AP_get_tmp_dir());
  UTX_wrf_lst (fn1, &fnLst);


  // - provide this list for selection
  irc = GUI_listf1__ (s1, sizeof(s1), fn1, "- select file -", "x40,y10");
  if(irc < 0) goto L_exit;
    // printf(" _lst_mshi-sel |%s|\n",s1);

//   // - copy file -> cfg/dir.lst
//   sprintf(fn1, "%s%s",AP_get_cfg_dir(),s1);
//   sprintf(fn2, "%sdir.lst",AP_get_cfg_dir());
//   OS_file_copy (fn1, fn2);

  // set AP_symDir_fnam = filename active SymbolDirFile
  if(strlen(s1) < 128) {
    strcpy(AP_symDir_fnam, s1);
    TX_Print("***** SymbolDirFile |%s| is active ..",AP_symDir_fnam);
  } else {
    TX_Print("***** Error AP_symDirSel filename too long ..");
  }


  L_exit:
    UtxTab_free (&fnLst);
      // printf(" ex-AP_symDirSel |%s|\n",AP_symDir_fnam);
    return 0;

}


//================================================================
  int AP_add_pt (long *dbi, int att, Point *pt1) {
//================================================================
// was GR_CrePoint
// Input:
//   dbi      DB-index; > 0 for permanent, -1 for permanent-dynamic
// Output:
//   dbi      DB-index for permanent-dynamic
//   retCod   0=OK, -1=ERR


  *dbi = DB_StorePoint (*dbi, pt1);  // store dynamic obj in DB.
  if(! *dbi) return -1;

  GR_perm_pt (*dbi, pt1, att);

  return 0;

}


//================================================================
  int AP_add_ln (long *dbi, int att, Line *ln1) {
//================================================================
// was GR_CreLine
// Input:
//   dbi      DB-index; > 0 for permanent, -1 for permanent-dynamic
// Output:
//   dbi      DB-index for permanent-dynamic
//   retCod   0=OK, -1=ERR


  *dbi = DB_StoreLine (*dbi, ln1);  // store dynamic obj in DB.
  if(! *dbi) return -1;

  GR_perm_ln (*dbi, ln1, att);

  return 0;

}


//================================================================
  int AP_mdlbox_invalid_ck () {
//================================================================
/// \code
/// AP_mdlbox_invalid_ck           check if modelbox is valid
///   if not - reCalculate model-box
/// 0 = modelbox is valid
/// 1 = modelbox is not valid
/// \endcode

  
  // printf("AP_mdlbox_invalid_ck %d\n",AP_stat.mdl_box_valid);

  return (AP_stat.mdl_box_valid);

}


//================================================================
  int AP_mdlbox_invalid_set () {
//================================================================
/// AP_mdlbox_invalid_set          set modelbox not valid (not initialized)

  

  AP_stat.mdl_box_valid = 1;

  // printf("AP_mdlbox_invalid_set \n");
  // AP_debug__ ("AP_mdlbox_invalid_set");

  return 0;

}

//================================================================
  int AP_mdlbox_invalid_reset () {
//================================================================
/// \code
/// AP_mdlbox_invalid_reset          set modelbox = 0=valid (1=not-valid)
/// see AP_mdlbox_invalid_ck
/// \endcode


  AP_stat.mdl_box_valid = 0;

  // printf("AP_mdlbox_invalid_set \n");
  // AP_debug__ ("AP_mdlbox_invalid_reset");

  return 0;

}


//================================================================
  int AP_get_modact_ind () {
//================================================================

  return AP_modact_ibm;

}


//================================================================
   int AP_dump_statPg (char *txt) {
//================================================================
// AP_dump_statPg      dump active subModel, active lineNr

// AP_modact_ibm     // the Nr of the active submodel; -1 = main.
// AP_modact_ibm  // -1=primary Model is active; else subModel is being created
// AP_modact_nam    // name of the active submodel; def="" (main)



  printf("%s AP_modact_ibm=%d AP_modact_nam=|%s|\n",txt,
          AP_modact_ibm, AP_modact_nam);

  printf(" APT_lNr=%d UP_level=%d\n",APT_lNr,UP_level);


  return 0;

}

/*
//=================================================================
  int AP_lNr_inc () {
//=================================================================

  ++ AP_ED_lNr;

  return 0;

}


//=================================================================
  long AP_lNr_get () {
//=================================================================
/// see also ED_get_lnr_act APT_lNr

  // printf("ex AP_lNr_get %d\n",AP_ED_lNr);

  return AP_ED_lNr;
}


//=================================================================
  int AP_lNr_set (long lNr) {
//=================================================================
/// \code
/// Achtung: der Editor benutzt zumeist ED_lnr_act
/// see also ED_set_lnr_act
/// \endcode


  // printf("AP_lNr_set %d\n",lNr);

  // ACHTUNG: Abfrage nur mit CAD verwenden (in MAN via GUI_edi_getLnr)
  if(lNr < 0) lNr = UTF_GetLnrPos (lNr);

  AP_ED_lNr = lNr;

  return 0;
}
*/

//================================================================
  int AP_errText_set (char *txt) {
//================================================================
 
  strcpy(AP_errText, txt);

  return 0;

}


//=================================================================
  int AP_errStat_reset (int mode) {
//=================================================================
// reset errors
// mode    0=reset only temporary errors (from CAD in temp-mode)
// mode    1=reset 


  // printf("AP_errStat_reset mode=%d errStat=%d errLn=%d APT_obj_stat=%d\n",
         // mode,AP_stat.errStat,AP_stat.errLn,APT_obj_stat);
  // printf("AP_errStat_reset sysStat=%d\n",AP_stat.sysStat);


  if(mode == 1) {
    // for ERR_DB_CSEG_EOM at primary startup set infor for UI_GL_draw__ 2024-06-19
    if(AP_stat.errStat == ERR_DB_CSEG_EOM) AP_stat.db_cseg_eom =1;
    AP_stat.errStat = 0;
    return 0;
  }


  // APT_obj_stat; 0=permanent, 1=temporary.
  if(APT_obj_stat == 1) AP_stat.errStat = 0;         // 2013-05-06
  // if(AP_stat.errLn < 0) AP_stat.errStat = 0;

  return 0;

}


//================================================================
  int AP_stat_file (int mode, FILE *fp1) {
//================================================================
// AP_stat_file                file write|read AP_box_pm1,pm2 AP_stat.mdl_box_valid
// AP_stat.mdl_modified NOT USED !


  char   s1[64];


  //----------------------------------------------------------------
  if(mode == 1) {          // write
    fwrite(&AP_box_pm1, sizeof(Point), 1, fp1);
    fwrite(&AP_box_pm2, sizeof(Point), 1, fp1);

    // save the AP_stat-bits mdl_modified and mdl_box_valid
    s1[0] = AP_stat.mdl_modified;
    s1[1] = AP_stat.mdl_box_valid;
    fwrite(s1, 2, 1, fp1);

  //----------------------------------------------------------------
  } else if(mode == 2) {   // read
    fread(&AP_box_pm1, sizeof(Point), 1, fp1);
    fread(&AP_box_pm2, sizeof(Point), 1, fp1);

    // read the AP_stat-bits mdl_modified and mdl_box_valid
    fread(s1, 2, 1, fp1);
    AP_stat.mdl_modified = s1[0];
    AP_stat.mdl_box_valid = s1[1];

  }

  return 0;

}

/*
//================================================================
  void DEB_prt_init (int mode) {
//================================================================
/// \code
/// mode  1   start debug; open debugfile;
///       0   end debug;   close debugfile
/// print into debugfile with "printd"
/// debugfile = <tmpdir>debug.dat = ~/gCAD3D/tmp/debug.dat
/// \endcode

  char  s1[128];


  AP_stat.debStat = mode;

#ifdef DEB
  //----------------------------------------------------------------
  if(mode != 0) goto L_start;
    // end debug; close debugfile
    if(AP_deb_fp) {
      fprintf(AP_deb_fp, "================ EOF ==============\n");
      fclose (AP_deb_fp);
      AP_deb_fp = NULL;
      printf("++++++ file %sdebug.dat closed +++++\n",AP_get_tmp_dir());
    }
    return;


  //----------------------------------------------------------------
  L_start:
    // start debug; open debugfile
    sprintf(s1, "%sdebug.dat",AP_get_tmp_dir());
    AP_deb_fp = fopen(s1, "w");
    printf("++++++ file %s opened +++++\n",s1);
    fprintf(AP_deb_fp, "++++++ file %s opened +++++\n",s1);
#endif

}


//================================================================
  void AP_deb_print (char* format, ...) {
//================================================================
/// print-formatted in debug-mode

  va_list va;

  // printf("AP_deb_print |%s|\n",format);

  va_start(va, format);


  if(AP_deb_fp) {
    vfprintf (AP_deb_fp, format, va);

  } else {      // this 
    vprintf (format, va);
  }


  va_end(va);

  return;

}
*/


//================================================================
  int AP_err_hide_set (int stat) {
//================================================================
/// err_hide   0=normal-display errors;   1=hide-errors

  AP_stat.err_hide = stat;

  return 0;

}


//================================================================
  int AP_errStat_set (int stat) {
//================================================================
// raise error
// stat:  0 = default = OK;
//        1 = Error                   - raise error; set AP_stat.errStat=1;
//        2 = DB_allocCDAT; rerun (in DB_allocCDAT !)
//
// see AP_errStat_reset AP_errText_set
// store lineNr of active line. if mode=temporary, store lineNr -1.


  printf("AP_errStat_set %d errStat=%d APT_obj_stat=%d APT_lNr=%d\n",
         stat, AP_stat.errStat, APT_obj_stat, APT_lNr);
  // printf("  AP_stat.errStat=%d AP_stat.errLn=%d\n",
         // AP_stat.errStat,AP_stat.errLn);


  // do not overwrite a serious error with a temporary error
  if((stat)&&(AP_stat.errStat))  {
    if(APT_obj_stat) return 1;  // APT_obj_stat: 0=permanent, 1=temporary (workmode)
    if(AP_stat.errStat > stat) return 1;  // do not overwrite; 2024-06-19
  }


  // do not reset an error in a previous line
  if(!stat) { 
    // try to reset;
    if(APT_lNr >= AP_stat.errLn) return 2;
  }


  AP_stat.errStat = stat;


  if(APT_obj_stat) {  // 0=permanent, 1=temporary (workmode)
    // temp-mode
    AP_stat.errLn = -1;

  } else {
    // AP_stat.errLn = ED_get_lnr_act ();
    AP_stat.errLn = APT_lNr;             // 2011-08-11
  }

    // printf("  set AP_stat.errStat=%d AP_stat.errLn=%d\n",
           // AP_stat.errStat,AP_stat.errLn);


  return 0;

}


//=================================================================
  int AP_errStat_get () {
//=================================================================
/// \code
/// returns AP_stat.errStat
/// see AP_errStat_set
/// \endcode


  // printf("AP_errStat_get = %d\n",AP_errStat);

  return AP_stat.errStat;
}


//=================================================================
  int AP_tutStat_get () {
//=================================================================

  // printf("AP_errStat_get = %d\n",AP_errStat);

  return AP_stat.TUT_stat;
}


//=================================================================
  int AP_sysStat_get () {
//=================================================================

  // printf("AP_errStat_get = %d\n",AP_errStat);

  return AP_stat.sysStat;
}


/*
//=================================================================
  int AP_obj_analyze (int typ, long ind) {
//=================================================================
// alt; replace by DEB_dump_obj__ (see IE_analyz__)
// siehe DemoPlugin_Resolv.c

  ObjG     el;
  CurvBSpl *bs1;
  ModelRef *mr1;
  ModelBas *mb1;
  Point    pt3;


  printf("AP_obj_analyze %d %d\n",typ,ind);


  DB_Get_GR_Obj (&el, typ, ind, APT_2d_to_3d_Ind);


  if(el.typ == Typ_PT) {
    TX_Print("PT %9.3f,%9.3f,%9.3f\n",el.p1.x,el.p1.y,el.p1.z);


  } else if(el.typ == Typ_LN) {
    TX_Print("LN %9.3f,%9.3f,%9.3f\n",el.p1.x,el.p1.y,el.p1.z);
    TX_Print("   %9.3f,%9.3f,%9.3f\n",el.p2.x,el.p2.y,el.p2.z);


  } else if((el.typ == Typ_AC)||(el.typ == Typ_CI)) {
    TX_Print("CI pa=%9.3f,%9.3f,%9.3f\n",el.p1.x,el.p1.y,el.p1.z);
    TX_Print("   pe=%9.3f,%9.3f,%9.3f\n",el.p2.x,el.p2.y,el.p2.z);
    TX_Print("   pc=%9.3f,%9.3f,%9.3f\n",el.pc.x,el.pc.y,el.pc.z);
    TX_Print("   vz=%9.3f,%9.3f,%9.3f\n",el.vz.dx,el.vz.dy,el.vz.dz);
    TX_Print("   rd=%9.3f\n",el.rad);


  } else if(el.typ == Typ_CVBSP) {
    bs1 = el.data;
    TX_Print("B-Spline %ld: ptNr=%d deg=%d v0=%.3f,v1=%.3f\n",ind,
                bs1->ptNr, bs1->deg, bs1->v0,bs1->v1);
    UT3D_pt_evalparCv(&pt3, bs1, bs1->v0);
    TX_Print("  Startpt. %12.3f,%12.3f,%12.3f\n",pt3.x,pt3.y,pt3.z);
    UT3D_pt_evalparCv(&pt3, bs1, bs1->v1);
    TX_Print("  Endpt.   %12.3f,%12.3f,%12.3f\n",pt3.x,pt3.y,pt3.z);
//
//      printf("  B-Spline %ld: ptNr=%d deg=%d v0=%.3f,v1=%.3f\n",apt_ind,
//              bs1->ptNr, bs1->deg, bs1->v0,bs1->v1);
//      UT3D_pt_evalparCv(&pt3, bs1, bs1->v0);
//      printf("  Startpt. = %f,%f,%f\n",pt3.x,pt3.y,pt3.z);
//      UT3D_pt_evalparCv(&pt3, bs1, bs1->v1);
//      printf("  Endpt. = %f,%f,%f\n",pt3.x,pt3.y,pt3.z);
//



  } else if(el.typ == Typ_Model) {
    mr1 = el.data;

    // zugehoerigen BasisModelRecord holen
    mb1 = DB_get_ModBas (mr1->modNr);

    TX_Print("Model %ld: %s Nr=%d\n",ind,mb1->mnam,mr1->modNr);
    TX_Print("  Origin:  %12.3f,%12.3f,%12.3f\n",mr1->po.x,mr1->po.y,mr1->po.z);
    TX_Print("  X-Vec.:  %12.3f,%12.3f,%12.3f\n",
                         mr1->vx.dx,mr1->vx.dy,mr1->vx.dz);
    TX_Print("  Z-Vec.:  %12.3f,%12.3f,%12.3f\n",
                         mr1->vz.dx,mr1->vz.dy,mr1->vz.dz);

  }


  return 0;

}


//================================================================
  int AP_fnam_get_user_2 (stru_FN *mns1, char *wTit, char *filter) {
//================================================================
// AP_fnam_get_user_2       open file, with directorySelect, filter, waiting.
//   does not modify globals AP_mod_dir ..
// Input:
//   fNam       startfile, can be empty
//   dNam       startdirectory (can be with filename)
//   title      of window
//   sf         filtertext
// Output:
//   retCode    0=OK, -1=Cancel, 1= no-symbolic-path, -2=fSiz/dSiz too small
// see AP_fnam_get_user_1 GUI_file_open__


#define FNSIZ 256

  int   irc;
  char  filNam[FNSIZ], dirLst[FNSIZ];


  printf("AP_fnam_get_user_2 |%s|%s|\n",wTit,filter);
  // printf(" |%s|\n",AP_mod_dir);

  // get filename of dir.lst (<cfg>/dir.lst)
  MDLFN_syFn_f_name (dirLst);

  // set startDirectory = active model-directory
  strcpy(filNam, AP_mod_dir);

  // call GUI_file/save
  irc = AP_GUI__ (filNam, FNSIZ, "GUI_file", "open",
                  filNam,    // outDir/outfilename
                  dirLst,    // filename of symbolic-directories
                  filter,    // filterText  (eg "*")
                  wTit,      // window-title
                  NULL);
  if(irc < 0) goto L_exit;

  // split selected filename and fill mns1
  irc = MDLFN_oFn_fNam (mns1, filNam);
  if(irc == -1) irc = 1;  // no-symbolic-path


  //----------------------------------------------------------------
  L_exit:
    // printf("ex-AP_fnam_get_user_2__  %d |%s|\n",irc,filNam);

    MDLFN_dump_ofn (mns1, "ex-AP_fnam_get_user_2");

  return irc;

}
*/

//===========================================================================
  int AP_fnam_get_user_1 (int mode, char *fNam, char *dNam, char *title, char *sf) {
//===========================================================================
/// \code
/// AP_fnam_get_user_1       open file, with directorySelect, filter, waiting.
/// Input:
///   mode       0 set AP_mod_dir, AP_mod_sym, AP_mod_fnam
///              1 set AP_mod_dir, AP_mod_sym; do not set AP_mod_fnam
///              2 do not modify AP_mod_dir, AP_mod_sym, AP_mod_fnam
///   fNam       startfile, can be empty
///   dNam       startdirectory (can be with filename)
///   title      of window
///   sf         filtertext
/// Output:
///   dNam       directory, can be symbolic
///   fNam       filename (with filetyp)
///   retCode    0=OK, -1=Cancel, -2=fSiz/dSiz too small
/// \endcode


  int      irc;
  char     s1[128], s2[128], s3[128], s4[40],
           fndl[256],  so[256];
  stru_FN  ofn;


  MDLFN_syFn_f_name (fndl);   // get filename of cfg/dir.lst
    // printf(" Mod_open-fndl |%s|\n",fndl);


    
  printf("AP_fnam_get_user_1 mode=%d title=|%s| dNam=|%s| sf=|%s|\n",mode,title,dNam,sf);
  // printf(" fNam=|%s|\n",fNam);


  // set startdirectory+file
  sprintf(so, "%s%s", dNam,fNam);


  // get filename from user
  //  (dirIn/filnamOut sSiz symDir filter title)
  irc = GUI_file_open__ (so, 256, fndl, sf, title);
    // printf(" AP_fnam_get_user_1-L1 irc=%d so=|%s| len=%ld\n",irc,so,strlen(so));
  if(irc < 0) return -1;
  if(strlen(so) < 1) return 0;
    // printf(" f-file_open__ |%s|\n",so);


  // get filenNameObj from filename
  irc = MDLFN_oFn_fNam (&ofn, so);
//   irc = MDLFN_fNam_resolv (s1, s2, s3, s4, so, AP_get_bas_dir());


  if(mode < 2) {
    // mode, 0, 1:
    if(irc > 0) strcpy(AP_mod_sym, ofn.symDir);  // s1);
    else AP_mod_sym[0] = '\0';
    strcpy(AP_mod_dir, ofn.fDir);                // s2);  // abs.path
    if(mode < 1) {
      strcpy(AP_mod_fnam, ofn.fNam);             // s3);  Modelname without filetyp
      strcpy(AP_mod_ftyp, ofn.fTyp);             // s4);  // filetyp
    }
  }


  // get dNam
  if(ofn.symDir[0]) strcpy(dNam, ofn.symDir);
  else              strcpy(dNam, ofn.fDir);

  // get fNam
  strcpy(fNam, ofn.fNam);     // s3);
  if(strlen(ofn.fTyp)) {strcat(fNam,"."); strcat(fNam,ofn.fTyp);}

    printf("ex-AP_fnam_get_user_1-Mod_open |%s|%s|\n",dNam,fNam);

  return 0;

}

 
//================================================================
  int AP_open__ (char *fNam, char *dNam, char *title, int mode) {
//================================================================
// mode   1=delete
//        2=rename
//        3=copy
 

  int    irc;
  char   s2[256], s3[512], s4[256];


  sprintf(s2, "%s%s",dNam,fNam);


  //================================================================
  L_open_delete:
  if(mode != 1) goto L_open_rename;
    sprintf(s3, "  %s %s  ",title, s2);
    irc = GUI_dlg_2b (s3,
                      MSG_const__(MSG_ok),
                      MSG_const__(MSG_no));
      printf(" irc=%d |%s|\n",irc,s2);
    if(irc == 0) OS_file_delete (s2);
    return 0;


  //================================================================
  L_open_rename:
  if(mode != 2) goto L_open_copy;
    sprintf(s3, " %s %s", title, s2);
    strcpy(s4, s2);
    irc = GUI_dlg_e2b (s4, 240, s3,
                          MSG_const__(MSG_ok),
                          MSG_const__(MSG_no));
      printf(" irc=%d |%s|\n",irc,s4);
    if(irc == 0) rename (s2, s4);   // (old, new)
    return 0;


  //================================================================
  L_open_copy:
  // if(mode != 3) goto L_open_rename;
    sprintf(s3, " %s %s", title, s2);
    strcpy(s4, s2);
    irc = GUI_dlg_e2b (s4, 240, s3,
                          MSG_const__(MSG_ok),
                          MSG_const__(MSG_no));
      printf(" irc=%d |%s|\n",irc,s4);
    if(irc != 0) return 0;

    if(OS_checkFilExist(s4, 1) == 1) {
      // sprintf(cbuf," overwrite file %s.%s ?  ",fnam,ftyp);
      MSG_get_1 (s3, 256, "OVER_fil", "%s", s4);
      irc = GUI_dlg_2b (s3,
                        MSG_const__(MSG_ok),
                        MSG_const__(MSG_no));
      if(irc != 0) return 0;
    }

    OS_file_copy (s2, s4);   // (old, new)
    return 0;


}


/* UU REPLACED BY GUI_file_over
//================================================================
  int AP_save_ex (int mode) {
//================================================================
// AP_save_ex       ask user for save/overwrite active model
//   - model is modified.
// mode:     0=save for exit (no cancel, mainwindow has exited)
//           1=load-new-model;  add option cancel.
// retCod:   0=0k, -1=cancel(do not save)
 
  int  irc;
  char s1[128], s2[256];

  printf("AP_save_ex %d |%s|\n",mode,AP_mod_fnam);


  // do not save model "unknown" if exiting
  // if(mode == 0) {
    // // invalidate modelBox ?
    // if(!strcmp(AP_mod_fnam,"unknown")) return 0;
  // }


  sprintf(s1, "\"  Model %s is modified; save (overwrite) ? \"",AP_mod_fnam);


  //----------------------------------------------------------------
  if(mode == 1) {  // 1=load-new-model
    // call GUI_dlg1/dlgbe
    irc = AP_GUI__ (s2, sizeof(s2), "GUI_dlg1", "dlgbe", s1,
                    MSG_const__(MSG_ok),      // "YES");
                    MSG_const__(MSG_cancel),
                    MSG_const__(MSG_no),
                    NULL);
  
  //----------------------------------------------------------------
  } else {   // 0=save for exit 
    irc = AP_GUI__ (s2, sizeof(s2), "GUI_dlg1", "dlgbe", s1,
                    MSG_const__(MSG_ok),      // "YES");
                    MSG_const__(MSG_no),
                    NULL);
  }
    // printf(" f-AP_GUI__ |%s|\n",s2);


  //----------------------------------------------------------------
  if(irc > 0) return -1;            // error
  if(UTX_IS_EMPTY(s2)) return -1;   // cancel

  if(s2[0] != '0') return 0;  // no = done

  // get outFilename, do not ask for overwrite, save
  AP_save__ (0, 1, 0, "gcad");

  return 0;

}
*/


//================================================================
  int AP_sav_as () {
//================================================================
// AP_sav_as           File / save as ".gcad"

  int    irc, stat;

  stat = AP_stat.mdl_stat;
  AP_stat.mdl_stat = MDLSTAT_save_as;
  irc = AP_save__ (0, 0, 2, "gcad");
  if(irc < 0) {AP_stat.mdl_stat = stat; return irc;}

  MDL_ren_sav ();  // update new modelname in browser

  AP_stat.mdl_stat = MDLSTAT_loaded;

    // printf(" ex-AP_sav_as\n");

  return 0;

}


//================================================================
  int AP_sav_qs () {
//================================================================
// AP_sav_qs           quicksav (Ctrl-s)

//   // do NOT save if AP_mod_iftyp != 0 (gcad)
//   // else quicksave overwrites import-model !
//   if(AP_mod_iftyp) {
//     TX_Print("**** quicksave after import not possible - use save as ****");
//     return 0;
//   }

  AP_stat.mdl_stat = MDLSTAT_save_quick;

  // check if sm or main active
  if(MDL_IS_MAIN) {
    // save main
    // save, 1=overwrite.  (Save_quick, Ctrl-s)
    AP_save__ (0, 2, 0, "gcad");



  } else {
    // sm is active;
    AP_save__ (0, 2, 0, "gcad");
  }



  AP_stat.mdl_stat = MDLSTAT_loaded;

    printf(" ex-AP_sav_qs\n");

  return 0;

}


//======================================================================
  int AP_save__ (int mode, int iOver, int uuSav, char *fTyp) {
//======================================================================
// AP_save__         save model group or subModel
// Input:
//   mode   0  save|export active model;
//          1  save|export group only;
//   iOver  0  ask for filename; ask for overwrite yes|no|cancel
//          1  do not ask for filename;  ask for overwrite yes|no
//          2  do not ask for filename;  overwrite;  (quicksave)
//   uuSav  0  save also unused-models
//          1  do NOT save unused-models
//          2  ask for save unused-models; yes|no|cancel
//          3  ask for save unused-models; yes|no
//   fTyp   eg "gcad" or "dxf" or ..
// Output:
//   retCod 0  ok,
//          -1 cancel(do not save)
//          -2 error
//
// mockups cannot be active - cannot save.
// imports can be active - can save-as and quick-save.

//
// Examples:
// AP_save__ (0, 2, 0, "gcad")       save_quick                     UI_men__ qSav
// AP_save__ (0, 0, 2, "gcad")       save_as                        UI_men__ exp1nat
// AP_save__ (0, 1, 0, "gcad")       save_exit (no cancel)
// AP_save__ (1, 0, 1, "gcad")       save_group                     UI_men__ grpSav
// AP_save__ (0, 0, 1, "dxf")        save_model (export)


static char     pmLast[SIZMFTot] = ""; 
static stru_FN  fnLast;

  int      irc=0, iExp, mTyp;
  char     fno[SIZMFTot], s1[80], s2[SIZMFTot], sTit[64], *pmNam;
  char     *buttons[3], sbt[2][80];
  stru_FN  ofm;


  printf("AP_save__ %d %d %d |%s|\n",mode,iOver,uuSav,fTyp);

  //----------------------------------------------------------------
  // clear Undo-List (sonst wilde gtk-Fehlermeldungen beim exit)
  UNDO_clear ();

  // save the active sub- or mainmodel
  MDL_sav_tmp ();


  // check for export
  if(strcmp(fTyp, AP_mod_ftyp)) iExp = 1;
  else iExp = 0;
    // printf(" AP_save__-iExp=%d\n",iExp);

  // get pmNam = name of active model (main or subModel)
  pmNam = MDL_mNam_prim ();

  // check if main or sm is active
  if(MDL_IS_MAIN) goto L_sav_main;


  //================================================================
  // subModel is active

  // save only intern model
  mTyp = MDL_mTyp_iNam (AP_modact_inm);
  if(mTyp != MBTYP_INTERN) {
    TX_Print("***** extern model; modify and save as main or change to internal ..");
    goto L_err;
  }


  // saveQuick or saveAs
  if(iOver == 0) {
    // saveAs
    goto L_sav_sm_ask;
  }

  // saveQuick
  // check if subModel was stored before
  if(!strcmp(pmNam, pmLast)) {
    // yes, already stored, use fnLast
    ofm = fnLast;
    goto L_sav_sm_do;
  }


  // ask user for modelname
  L_sav_sm_ask:

    // no - new sm; ask user for modelname
    // get ofm = extern filename for primary-model
    MDLFN_ofn_set (&ofm, AP_mod_sym, pmNam, fTyp);
    // MDLFN_ofn_set (&ofm, AP_mod_sym, pmNam, "gcad");

    // get full filename primary model from ofm
    MDLFN_ffNam_oFn (fno, &ofm);

    // window-titel "Model save <directory>" 
    strcpy(sTit, MSG_const__(MSG_save));

    // get s2 = filename of cfg/dir.lst
    MDLFN_syFn_f_name (s2);

    // get fno = filename from user
                       // (filnamOut sSiz symDir filter title)
    irc = GUI_file_save__ (fno, sizeof(fno), s2, fTyp, sTit, 1);
      // printf("ex-GUI_file_save__ irc=%d fno=|%s|\n",irc,fno);
      // 0=OK; -1 = cancel or no; >0 = iOK (chrlen of fNam);
    if(irc < 1) {
      if(irc == -2) goto L_err;
      goto L_cancel;
    }
    if(strlen(fno) < 1) goto L_cancel;
      // printf(" _save__-fno1 %d |%s|\n",irc,fno);

    // test if fNam has filetype; if not: add fTyp
    irc = UTX_ftyp_s (s1, fno, 0);
    if(irc < 0) {
      strcat (fno, ".");
      strcat (fno, fTyp);
        // printf(" _save__-fno2 %d |%s|\n",irc,fno);
    }

    // split fno into filename-object
    MDLFN_oFn_fNam (&ofm, fno);

    // check if outDir AP_mod_dir exists
    if(OS_checkFilExist(ofm.fDir,1) ==  0) {
      // TX_Error("Directory %s does not exist ..",AP_mod_dir);
      MSG_err_1 ("NOEX_dir", "%s", ofm.fDir); goto L_err;
    }


  L_sav_sm_do:
      // MDLFN_dump_ofn (&ofm, "AP_save__-1");
    TX_Print("----- save intern model %s as extern model %s/%s.%s -",
             ofm.fNam, ofm.symDir,ofm.fNam,ofm.fTyp);
    goto L_sav_3;


  //================================================================
  L_sav_main:  // save main and ist subModels
    // printf(" L_sav_main: iOver=%d\n",iOver);

  if(iOver == 2) {
    // do not ask for filename;  overwrite;  (quicksave)
    // get ofm = AP_mod_* (existing fileDir, name, sym)
    MDLFN_get__ (&ofm);

  //----------------------------------------------------------------
  } else if(iOver == 1) {
    // do not ask for filename;  ask for overwrite yes|no

    // get ofm = AP_mod_* (existing fileDir, name, sym)
    MDLFN_get__ (&ofm);

    // set fno = outfilename
    MDLFN_ffNam_oFn (fno, &ofm);

    // ask for overwrite yes|no;
    iOver = GUI_file_over (fno, 0);
    // iOver now: 0=overwrite; 1=not; -1 = abort
    if(iOver) goto L_cancel;

  //----------------------------------------------------------------
  } else if(iOver == 0) {
    // iOver=0: ask for filename; check for overwrite,

    // get filename from user:

    // get a copy of AP_mod_*
    MDLFN_get__ (&ofm);
    // for mode=1/save_group remove filename
    if(mode == 1) strcpy (ofm.fNam, "group1");
    // for mode=2/save_model set filename = modelname
    if(mode == 2) strcpy (ofm.fNam, AP_modact_nam);
    // if export:
    if(iExp) strcpy (ofm.fTyp, fTyp);
    // get full filename active mainModel from ofm
    MDLFN_ffNam_oFn (fno, &ofm);
    // window-titel "Model save <directory>" 
    strcpy(sTit, MSG_const__(MSG_save));
    // get s2 = filename of cfg/dir.lst
    MDLFN_syFn_f_name (s2);
    // get fno = filename from user
                       // (filnamOut sSiz symDir filter title)
    irc = GUI_file_save__ (fno, sizeof(fno), s2, fTyp, sTit, 1);
      // printf("ex-GUI_file_save__ irc=%d fno=|%s|\n",irc,fno);
      // 0=OK; -1 = cancel or no; >0 = iOK (chrlen of fNam);
    if(irc < 1) {
      if(irc == -2) goto L_err;
      goto L_cancel;
    }
    if(strlen(fno) < 1) goto L_cancel;
      // printf(" _save__-fno1 %d |%s|\n",irc,fno);

    // test if fNam has filetype; if not: add fTyp
    irc = UTX_ftyp_s (s1, fno, 0);
    if(irc < 0) {
      strcat (fno, ".");
      strcat (fno, fTyp);
        // printf(" _save__-fno2 %d |%s|\n",irc,fno);
    }

    // split fno into filename-object
    MDLFN_oFn_fNam (&ofm, fno);

    // check if outDir AP_mod_dir exists
    if(OS_checkFilExist(ofm.fDir,1) ==  0) {
      // TX_Error("Directory %s does not exist ..",AP_mod_dir);
      MSG_err_1 ("NOEX_dir", "%s", ofm.fDir); goto L_err;
    }

    // ask for overwrite yes|no|cancel
    iOver = GUI_file_over (fno, 1);
    // iOver now: 0=overwrite; 1=not; -1 = abort
    if(iOver) goto L_cancel;
  }



  //================================================================
  // ask for save unused subModels;
  L_sav_3:
    printf(" L_sav_3: uuSav=%d\n",uuSav);


  if(uuSav == 2) {
    // ask for save unused-models; yes|no|cancel
    uuSav = MDL_file_smuu (uuSav);         // was AP_save_del_smuu
    // uuSav now: -1=cancel; 0=save-unused-models; 1=do-NOT-save-unused-models
    if(uuSav < 0) goto L_cancel;

  //----------------------------------------------------------------
  } else if(uuSav == 3) {
    // ask for save unused-models; yes|no
    uuSav = MDL_file_smuu (uuSav);         // was AP_save_del_smuu
    // uuSav now: 0=save-unused-models; 1=do-NOT-save-unused-models
  }


  //================================================================
  // save|export
    printf(" L_sav_sav: mode=%d\n",mode);

  if(mode == 0) {

    // save|export main (complete model)
    MDL_exp__ (&ofm, mode, uuSav);
    
    // if native: set new AP_mod_* and keep as new model
    if(ofm.iTyp == Mtyp_Gcad) {

      if(MDL_IS_MAIN) {
        // set AP_mod_*
        MDLFN_set__ (&ofm);

        // update title
        UI_title_set ();

        // rename Mod_.mod_out -> Mod_.mod_in
        AP_mod_sav_ok ();

      } else {
        // sm saved 
        fnLast = ofm;
        strcpy(pmLast, pmNam);
      }
    }

    // add filename to list "last-used"
    AP_Mod_lstAdd ();


  //----------------------------------------------------------------
  } else if(mode == 1) {
    // save|export group only;
    Grp_exp (ofm.fNam, ofm.fDir);

    // add filename to list "last-used"
    // TODO: fix fn = name symbolic filename of model - AP_mod_fnam is active model !!
    // AP_Mod_lst_fn (fn);


  //----------------------------------------------------------------
  } else if(mode == 2) {
    // save|export active model;
    MDLFN_ffNam_oFn (fno, &ofm);
    MDL_sav__ (fno, mode, uuSav);
  }


  //================================================================
  L_exit:

    // MAN: reset focus                     // 2013-04-12
    // sonst kein Cursor im Editor mehr ..
    if(UI_InpMode == UI_MODE_MAN) EDI_focus ();

      // printf(" ex-AP_save__ %d\n",irc);
    return irc;


  L_cancel:
    TX_Print("***** save cancelled by user ...");
    return -1;


  L_err:
    return -2;

}
 

/* UU -replaced by MDL_file_smuu
//================================================================
  int AP_save_del_smuu () {
//================================================================
// AP_save_del_smuu            ask for / delete unused subModels
// RetCode     0=OK; <0=Error;
//
// was Mod_fget_names__
// replacing Mod_fget_names_1
// TODO: processes  see Mod_sav_i
// TODO: PTAB-Surfs and MSH-Surfs ??

  int           irc, i1, ii, *ia, no_for_all=0;
  char          s1[SIZMFTot], s2[256], s3[400], *pa[5], *p1;
  FILE          *fp1;

  UtxTab_NEW (mdlTab);                // stringtable
  UtxTab_NEW (surPtab);               // stringtable
  UtxTab_NEW (surMsh);                // stringtable


  printf("UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU \n");
  printf("AP_save_del_smuu \n");


  //----------------------------------------------------------------
  // create list of all subModels, PTAB-Surfs and MSH-Surfs

//   // get lists of all used subModels, PTAB-Surfs and MSH-Surfs
//   s1[0] = '\0';    // main = ""
//   irc = Mod_fget_names__ (&mdlTab, &surPtab, &surMsh,s1);
//   if(irc < 0) { goto L_exit; }
//     // UtxTab_dump (&mdlTab, " f-Mod_fget_names__-mdlTab1");
//  
//   // mdlTab is symbolic-mdlNam (eg |Data/mdlnam|); must make safe-name.
//   for(ii=0; ii < mdlTab.iNr; ++ii) {
//     p1 = UtxTab__ (ii, &mdlTab);
//     UTX_safeName (p1, 1);
//   }
    // UtxTab_dump (&mdlTab, " f-Mod_fget_names__-mdlTab2");
// TODO: test also PTAB-Surfs and MSH-Surfs (was Mod_fget_names__ Mod_fget_names_0)
 
/
  // get file tmp/Mod.lst = list of models used by active primary-model
  MDL_file_lst__ (1);   // Mod_flst_all (1); 

  // get mdlTab = list of existing subModels
//   UtxTab_init__ (&mdlTab);
//   MDL_lst__ (&mdlTab, MBTYP_INTERN, 0);
  MDL_lst_all (&mdlTab);

    // TESTBLOCK
    printf("------------- _del_smuu- used-models-Mod.lst:\n");
    sprintf(s1,"cat %sMod.lst",AP_get_tmp_dir()); OS_system (s1);
    // goto L_exit;
    // END TESTBLOCK
 


  //----------------------------------------------------------------
  // set int-array stat parallel to mdlTab; 0=unused, 1=used
  ia = (int*) MEM_alloc_tmp (sizeof(int) * mdlTab.iNr);
  for(i1=0; i1<mdlTab.iNr; ++i1) ia[i1] = 0;


  // loop tru list of all used subModelFiles
  sprintf(s1,"%sMod.lst",AP_get_tmp_dir());
  if((fp1=fopen(s1,"r")) == NULL) goto L_uuList;  // no sm is used

  p1 = &s1[6];          // skip "Model_"
  while (!feof (fp1)) {
    // get s1 = next subModelname of existing sm. s1 is safe-name.
    if (fgets (s1, SIZMFTot, fp1) == NULL) break;
    UTX_CleanCR (s1);
      // printf(" - Mod_fget_names_1 test |%s|\n",p1);
    // must ignore main
    if(!strcmp(p1, 

    // check if subModel is used - is in list mdlNam.
    // mdlNam 
    ii = UtxTab_find (s1, &mdlTab);
      // printf(" ex UtxTab_find %d\n",ii);
    if(ii >= 0) ia[ii] = 1;   // set used
   }


  L_uuList:
    // TESTBLOCK
    for(i1=0; i1<mdlTab.iNr; ++i1)
      if(ia[i1]) printf(" _del_smuu-unused %d |%s|\n",i1,UtxTab__(i1,&mdlTab));
    // END TESTBLOCK
/


  


//   // loop tru ../tmp/Mod.lst (existing model)
//   irc = -1;
//   while (!feof (fp1)) {
//     // get s1 = next subModelname of existing sm. s1 is safe-name.
//     if (fgets (s1, 256, fp1) == NULL) break;
//     UTX_CleanCR (s1);   // in s1 ist nun Modename
//       // printf(" - Mod_fget_names_1 test |%s|\n",s1);
// 
//     // check if subModel is used - is in list mdlNam.
//     // mdlNam 
//     ii = UtxTab_find (s1, &mdlTab);
//       // printf(" ex UtxTab_find %d\n",ii);
//     if(ii >= 0) continue;   // continue if used (exists in mdlTab)
// 
//       // TESTBLOCK
//       printf(" _del_smuu-unused |%s|\n",s1);
//       // END TESTBLOCK
//  
// 
// 
//     //----------------------------------------------------------------
//     // subModel <s1> is unused;
//     if(no_for_all) goto L_del_sm;
// 
//     // ask user to save subModel s1; if no: delete subModel.
//     // sprintf(s2, "Save unused submodel %s ?",s1);
//     MSG_get_1 (s2, 256, "SAVusm", "%s", s1);
//       // printf("del_smuu-s2=|%s|\n",s2);
//     // sprintf(s3, "\" %s  \"",s2);  // embed with "
//     UTX_ENC_ApoD_TMP (&pa[4], s2);
//       // printf("del_smuu-pa4=|%s|\n",pa[4]);
// 
//     // enclose button-text between ""
//     UTX_ENC_ApoD_TMP (&pa[0], MSG_const__(MSG_no));
//     UTX_ENC_ApoD_TMP (&pa[1], MSG_const__(MSG_no_for_all));
//     UTX_ENC_ApoD_TMP (&pa[2], MSG_const__(MSG_ok_for_all));
//     UTX_ENC_ApoD_TMP (&pa[3], MSG_const__(MSG_ok));
// 
//     // get sel from user
//     irc = AP_GUI__ (s2, sizeof(s2), "GUI_dlg1", "dlgbe", pa[4],  //s3,
//                   pa[0], pa[1], pa[2], pa[3],
//                   NULL);
//       // printf(" f.AP_GUI__ irc=%d s2=|%s|\n",irc,s2);
// 
//     if(irc < 0) goto L_cancel;
//     if(UTX_IS_EMPTY(s2)) goto L_cancel;
// 
//     if(s2[0] == '2') goto L_done;     // ok_for_all - exit
//     if(s2[0] == '3') continue;        // ok = do not delete
//     if(s2[0] == '1') no_for_all = 1;  // no_for_all
// 
// 
//     //----------------------------------------------------------------
//     // '2' = ok = delete subModel
//     L_del_sm:
//       sprintf(s2, "%sModel_%s",AP_get_tmp_dir(), s1);
//         // printf(" del file |%s|\n",s2);
//       OS_file_delete (s2);
//       continue;
// 
// 
//     L_cancel:
//       fclose(fp1);
//       irc = -1;
//       break;
//   }
// 
// 
//   L_done:
//   fclose(fp1);


  L_exit:

  UtxTab_free (&mdlTab);
  UtxTab_free (&surPtab);
  UtxTab_free (&surMsh);

    printf(" ex-AP_save_del_smuu %d\n",irc);

  return irc;

}
*/

//================================================================
  int AP_src_init (char *fnMain) {
//================================================================
// AP_src_init                 write empty file tmp/Model_
//   used only if file without primary model is loaded
 
  FILE     *fpo;


  if((fpo=fopen(fnMain,"w")) == NULL) {
    TX_Error("****** AP_src_init open error %d\n",fnMain);
    return -1;
  }

  fprintf(fpo,"# %s\n",OS_date1());
  fprintf(fpo,":DYNAMIC_DATA\n");

  fclose(fpo);

  return 0;

}


/*
//================================================================
  int AP_mdl_new () {
//================================================================
// AP_src_new             create new empty model "unknown"
// - tmp/Model_unknown

  char   cbuf[256];

  printf("AP_mdl_new \n");

  // init and create new model unknown
  strcpy(AP_mod_fnam, "unknown");
  strcpy(AP_mod_ftyp, "gcad");
  AP_mod_iftyp = Mtyp_Gcad;

  // clear all, init Model_unknown, load mainModel into memSpc (UTF_FilBuf0)
  MDL_load_new__ ();

//   // MAN: clear Edi  UI_ask_mode  if(AP_src == AP_SRC_MEM/AP_SRC_EDI)
//   if(AP_src == AP_SRC_EDI) {
//     // die Hauptdatei raus ins Editfenster (mem->Window)
//     ED_load__ ();
//     // Cursor ans EOF
//     ED_goto__ (-1L);
//     // Focus aus EditWindow
//     EDI_focus ();
//   }

  //----------------------------------------------------------------
    printf("ex-AP_mdl_new \n");

  return 0;

}
*/


//=================================================================
  int AP_mdl_init (int mode) {
//=================================================================
// clear src-Memory, reset Undo, Hide, View-Plane, ConstrPlane.
// mode  unused



  // printf("AP_mdl_init %d\n",mode);

  AP_errStat_reset (1);    // clear Error

  // reset model-modified (was activated with UTF_add_line)
  // AP_mdl_modified_reset ();

  // reset CAD
  if(UI_InpMode == UI_MODE_CAD) {
    // IE_cad_exit0 ();
    IE_cad_exitFunc ();
    IE_cad_Inp_undo ();
    IE_cad_Inp_undo ();
  }


  // unload unused DLLs
  MDL_load_unl ();
  UI_PRI_unl ();   // unload print/export if not active

  // do NOT unload plugins; remoteCtrl makes new = unload from plugin = crash.
  // DLL_plu_unl ();


  // close files
  if(AP_stat.jntStat) {
    DBF_exit ();     // close joints-file
    AP_stat.jntStat = 0;
  }

  // reset PRCV
  PRCV0_REL ();

  // Clear Memory u. Editfenster
  UTF_clear_ ();                    // Clear Mem
  UTF_clear1 ();                    // Clear Mem

  // kill all tmp-files
  MDL_clean_f ();


  //----------------------------------------------------------------
//   if(mode) {
//     // init and create new model unknown
// 
//     strcpy(AP_mod_fnam, "unknown");
//     strcpy(AP_mod_ftyp, "gcad");
//     AP_mod_iftyp = Mtyp_Gcad;
// 
//     // clear all, init Model_unknown
//     MDL_load_new__ ();
// 
// 
//   //----------------------------------------------------------------
//   } else {
    // init for load extern model
    DB_del_Mod__ ();     // delete all basic- and refModels

//     MDL_prev_stat__ (OBJSTAT_perm);  // init mdl_stat.stat
// 
//     // not yet initialized
//     AP_mod_fnam[0] = '\0';
//   }




  //----------------------------------------------------------------
  // alle texturen loeschen ..
  Tex_Init__ ();

  // clear/init colors
  APcol_defCol_3i (147,147,173);   // init AP_defcol
  APcol_actCol__ (&AP_defcol);

  // clear 2D-mode
  NC_setRefsys (0);

  // clear all viewports
  vwpt__ (-1);

  // kill active Form
  PRG_win__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncKill));

  // clear "Search/Name"-List
  UI_cb_search (3);

  // reset "add to Group"
  // UI_reset ();
  UI_GR_Sel_Filt_reset ();      // reset add to group

  // clear PRCV-spc
  PRCV_init__ ();

  // Title oben auf den Mainwinrahmen
  UI_AP (UI_FuncSet, UID_Main_title, NULL);

  // init DL, ED, WC
  AP_Init1 ();

  // reset Group
  Grp_alloc ();

  // clear Undo-List
  UNDO_clear ();

  // reset HideList
  GA_clear ();
  // GA_hide__ (-1, 0L, 0);

  // init View-Plane and ConstrPlane
  AP_Init_planes ();
  UCS_Reset ();          //  set WC_sur_mat, WC_sur_imat

  // clear modelname
  AP_mod_fnam[0] = '\0';

  GL_Clear ();                   // clear GL; 2022-09-01

  // init GR_selTab, GR_Siz_selTab, GR_nr_selTab
  GL_selTab_init ();

  // only first start
  sele_set__ (Typ_goGeom);       // enable selection of all types

  // clear browserWindow
  Brw_init__ ();

  // PED_CB1 (GUI_ES("Exit")); // kill PED if active
  // EDMPT_CB1 (GUI_ES("Exit"));  // kill EDMPT if active
  // OS_dll_do ("xa_edmpt", NULL, NULL, 2);  // unload dll xa_edmpt


//   //----------------------------------------------------------------
//   if(mode) {
// 
//     // init and create new model unknown
//     strcpy(AP_mod_fnam, "unknown");
//     strcpy(AP_mod_ftyp, "gcad");
//     AP_mod_iftyp = Mtyp_Gcad;
// 
//     // clear all, init Model_unknown
//     MDL_load_new__ ();
// 
//     // only first start
//     sele_set__ (Typ_goGeom);       // enable selection of all types
// 
//     // clear browserWindow
//     Brw_init__ ();
// 
//     // load file tmp/Model_unknown into mem UTF_FilBuf0
//     sprintf (cbuf , "%sModel_unknown",AP_get_tmp_dir());
//     UTF_add_fil_init (cbuf);
// 
//     // MAN: clear Edi  UI_ask_mode  if(AP_src == AP_SRC_MEM/AP_SRC_EDI)
//     if(AP_src == AP_SRC_EDI) {
//       // die Hauptdatei raus ins Editfenster (mem->Window)
//       ED_load__ ();
//       // Cursor ans EOF
//       ED_goto__ (-1L);
//       // Focus aus EditWindow
//       EDI_focus ();
//     }
// 
//   }
// 
  //----------------------------------------------------------------
    // printf("ex-AP_mdl_init \n");
  return 0;

}


//=================================================================
  int AP_src_mod_ed (long lNr) {
//=================================================================
// AP_src_mod_ed                  edit src-line with SystemEditor.
// RetCode    0=OK, -1=userAbort
//
// see also ED_sysed__

static long old_lNr = -2;

  int   i1;
  long  cpos;
  char  *p1, cbuf[256], fn2[256];


  // printf("AP_src_mod_ed %ld %ld\n",lNr,old_lNr);



  if(old_lNr == lNr) {
    sprintf(cbuf,"  OK to modify line %ld  ",lNr);
    i1 = GUI_dlg_2b (cbuf, "OK", "Cancel");
    if(i1 != 0) return -1;
  }


  if(lNr < 0) {   // CB von AP_src_mod_CB
    lNr = old_lNr;
  }


  sprintf(cbuf,"%sselection.txt",AP_get_tmp_dir());


  // Zeile in ein File rausschreiben
  UTF_wrf_line (lNr, cbuf);
  TX_Print("Edit line %d spawned ..",lNr);


  // modify File m. Editor
  sprintf(fn2,"%sselection1.txt",AP_get_tmp_dir());
  OS_file_copy (cbuf, fn2);
  APP_edit (cbuf, 0);    // edit - wait
  // printf(" NNNNNNNNNNNNNNNNNNNach OS_edit_\n");
  // user-abort ?
  if(!OS_file_compare_A (cbuf, fn2)) {
    TX_Print("***** user-abort ..");
    return -1;
  }

  TX_Print("Edit done ..");

  // reset errors
  AP_errStat_reset (1);


  // load file into buf1
  UTF_clear1 ();
  p1 = UTF_add1_file (cbuf);
  UTX_CleanCR (p1);   // remove LF ..

  // change Line im mem by file
  UTF_chg_line (lNr, p1);


  old_lNr = lNr;

  // i1 = UI_ask_mode ();
  i1 = UI_InpMode;


  return 0;


  //----------------------------------------------------------------
  if(i1 == UI_MODE_VWR) {
    return 0; // Wenn Vwr aktiv: nix tun


  //----------------------------------------------------------------
  } else if(i1 == UI_MODE_CAD) {

    if(IE_get_modify() == 0) {         // CAD-Add Error ?
      // CAD-RUN:
      IE_cad_init0 ();  //reRun ..

    } else {                          // CAD-Modify:
      IE_activate ();
    }
    return 0;


  //----------------------------------------------------------------
  } else if(i1 == UI_MODE_MAN) {

    // entweder zeile nur austauschen im Edi oder
    // alles austauschen, END und stop SM.
    // copy Memory -> Editor
    ED_load__ ();

    // if(UI_InpSM == ON) {     // SearchMeasure ON ?
      // UI_WinSM (NULL, (void*)UI_FuncExit);   // exit SM
    // }

    // // reRun
    // UI_but__ ("butEND");

    // // copy --> mem_cbuf1 mem_cbuf1_SIZ
    // UTF_get_Buf1 (mem_cbuf1, mem_cbuf1_SIZ);
    // // execute code in buf1
    // WC_Work1 (lNr, mem_cbuf1);

    // cursor auf Line lNr setzen 
    EDI_set_lNr__ (lNr-1);


    // work Line ...
    ED_work_CurSet (lNr);

  }


  return 0;

}

/*
//================================================================
  int AP_ato_getSpc1 (int *tabSiz, int **aus_typ, double **aus_tab) {
//================================================================
/// \code
/// DO NOT USE - use AP_ato_getSpc__
/// AP_ato_getSpc1      get memSpc for atomicObjects - simple
/// \endcode

  // Im BATCH aber static !!
  // if(AP_stat.batch == 0) {       // 0=interaktiv;

  int    i1, i2;

// aus ../gr/ut_GLU.c:
extern Point     *GLT_pta;
extern long      GLT_pta_SIZ;
extern ObjGX     *GLT_cta;
extern long      GLT_cta_SIZ;


  *aus_typ = (void*)GLT_cta;
  *aus_tab = (void*)GLT_pta;

  i1 = sizeof(ObjGX) * GLT_cta_SIZ / sizeof(int);
  i2 = sizeof(Point) * GLT_pta_SIZ / sizeof(double);

  *tabSiz = IMIN (i1, i2);

  return 0;

}


//================================================================
  int AP_ato_sort1 (int oNr, int *oTyp, double *oVal) {
//================================================================
/// sort types ascending


  int    i1;


  // printf("AP_ato_sort1 %d\n",oNr);
    // for(i1=0; i1<oNr; ++i1)
    // printf(" %d typ=%d tab=%f\n",i1,oTyp[i1],oVal[i1]);


  L_sort0:

    for(i1=1; i1<oNr; ++i1) {
      if(oTyp[i1] < oTyp[i1 - 1]) {
        MEM_swap_int (&oTyp[i1], &oTyp[i1 - 1]);
        MEM_swap_2db (&oVal[i1], &oVal[i1 - 1]);
        goto L_sort0;
      }
    }



    for(i1=0; i1<oNr; ++i1)
    printf(" %d typ=%d tab=%f\n",i1,oTyp[i1],oVal[i1]);


  return 0;

}


//================================================================
  int AP_ato_getSpc__ (ObjAto *ato1) {
//================================================================
// AP_ato_getSpc       get memSpc for atomicObjects

  int    i1, i2;


  ato1->typ = (int*) memspc55;
  ato1->val = (double*) memspc54;
  // ato1->iPar = (double*) memspc011;

  i1 = sizeof(memspc55) / sizeof(int);
  i2 = sizeof(memspc54) / 256;

  ato1->siz = IMIN (i1, i2);
  ato1->nr = 0;

  return 0;

}
*/

//=================================================================
  int AP_APT_sysed () {
//=================================================================
// modify line ED_lnr_act with system-editor

  int  sMode;
  long il1;


  printf("AP_APT_sysed\n");


  // check if Error is active
  if(AP_stat.errStat != 0) {
    AP_src_mod_ed (AP_stat.errLn);
    return 0;
  }


  // sMode = UI_ask_mode ();
  sMode = UI_InpMode;

  

  //================================================================
  if(sMode == UI_MODE_MAN) {

    // // ask LineNr
    // UI_AP (UI_FuncGet, UID_Edit_Cpos, (void*)&il1);
    // // get LineNr from offsetpos
    // il1 = UTF_GetLnrPos (il1);
    il1 = ED_get_lnr_act ();

    printf("AP_APT_sysed %ld\n",il1);

    // // copy Editor -> Memory
    // ED_unload__ ();

    // modify zeile im mem mit dem SystemEditor.
    AP_src_mod_ed (il1);

    // AP_lNr_inc();            // goto next Line

    return 0;



/*
  //================================================================
  } else if(sMode == UI_MODE_CAD) {

    // nur im Modus S/M
    if(UI_InpSM == ON) {
      il1 = AP_lNr_get ();
      // modify zeile im mem mit dem SystemEditor.
      AP_src_mod_ed (il1);
      // redraw
      --il1;
      ED_set_lnr_act (il1);
      ED_work_CurSet (999999); // ED_work_CAD ? ED_work_CurSet
      

    } else {
      TX_Print("for modify start S/M, select obj ..");
    }
*/


  //================================================================
  } else {
    TX_Print("modify needs CAD or MAN ..");
  }


  return 0;

}


//================================================================
  int AP_SRC_mem_edi () {
//================================================================
// copy Editor --> memory (if necessary)

/* should also be useful in
AP_APT_clean
AP_obj_del2
AP_Mod_load__
UI_butSM
Mod_sav_tmp
ED_Get_LineNr
ED_Run
*/

  if(AP_src == AP_SRC_EDI)
    ED_unload__ ();

  return 0;

}



//================================================================
  int AP_SRC_edi_mem () {
//================================================================
// die Hauptdatei raus ins Editfenster (mem->Window) (if necessary)
// und ED_Reset (ED_lnr_act=0)

  if(AP_src == AP_SRC_EDI)
    ED_load__ ();

  return 0;

}


//=================================================================
  int AP_APT_clean () {
//=================================================================
// geloeschte zeilen endgueltig raus ..

  char      txbuf[512];


  printf("AP_APT_clean\n");


  // Editor -> mem
  if(AP_src == AP_SRC_EDI) ED_unload__ ();

  // Mem -> Datei raus (filtern)
  sprintf(txbuf,"%sxa.tmp",AP_get_tmp_dir());
  UTF_wri_file (txbuf, NULL);

  // Datei -> Mem einlesen
  UTF_clear_ ();
  UTF_add_file (txbuf);

  // Mem -> Editor
  if(AP_src == AP_SRC_EDI) ED_load__ ();

  // imply END-Button (Redraw)
  ED_work_END (0);

  // clear Undo-List
  UNDO_clear ();

  return 0;

}


//=================================================================
  int AP_obj_delCk1 (long *lNr) {
//=================================================================
// Zeile AP_ED_lNr soll (mit "_") geloescht werden;
// testen, ob es eine Definitionszeile ist
// testen, ob dieses Obj von einer der Folgezeilen benoetigt wird
// Output:
// lNr      letzte gefundenen zeile
// IRC = 0: OK, loeschen.
// IRC > 0: Zeile wird noch benoetigt; Anzahl wieoft.




  int    irc, ctyp;
  long   lAct, llen;
  char   *cp1, *cp2, *cp3, *cp4, *w_next, txtObj[32],txtOut[32], deli,
         *lBuf;
  double d1;



  // printf("AP_obj_delCk1 %d\n",AP_ED_lNr);

  irc = 0;

  lBuf = memspc101;


  //=============================================================
  // testen, ob es eine Definitionszeile ist
  cp1 = UTF_GetLinNr (lBuf, &llen, ED_lnr_act);
  // printf(" lNr %d |%s| len %d\n",AP_ED_lNr,lBuf,llen);

  if(lBuf[0] == '#') goto L_fertig;
  if(lBuf[0] == '_') goto L_fertig;


  cp2 = lBuf;
  ctyp = UTX_get_word (&cp2, &w_next, txtObj, &d1, &deli);
  // printf(" _word %d |%c|\n",ctyp,deli);

  if(deli != '=') goto L_fertig;


  


  //=============================================================
  // testen, ob dieses Obj von einer der Folgezeilen benoetigt wird
  // printf(" Obj=|%s|\n",txtObj);


  lAct = ED_lnr_act;


  L_nxt_ln:
  ++lAct;

  cp1 = UTF_GetLinNr (lBuf, &llen, lAct);
  if(llen < 0) goto L_fertig;      // nix gefunden

  // printf(" lNr %d |%s| len %d\n",lAct,lBuf,llen);

  if(lBuf[0] == '#') goto L_nxt_ln;
  if(lBuf[0] == '_') goto L_nxt_ln;


  cp2 = lBuf;
  ctyp = UTX_get_word (&cp2, &w_next, txtOut, &d1, &deli);
  // printf(" _word %d |%c|\n",ctyp,deli);

  if(deli != '=') goto L_nxt_ln;


  // printf(" found DefLn: |%s|\n",w_next);

  cp3 = UTX_find_word1 (txtObj, w_next);
  if(cp3 == NULL) goto L_nxt_ln;


  // gefunden!
  L_found:
  ++irc;
  *lNr = lAct;
  goto L_nxt_ln;



  //--------------------------------------------
  L_fertig:
  // printf("ex AP_obj_delCk1 rc=%d |%s| lNr=%d\n",irc,txtObj,*lNr);
  return irc;

}

/*
//=================================================================
  int AP_obj_del0 () {
//=================================================================
// unused ..
// Zeile AP_ED_lNr soll (mit "_") geloescht werden

  int   irc, llen;
  long  lNr, startPos, endPos;
  char  *cp1, *lBuf;



  printf("AP_obj_del0 %d\n", AP_ED_lNr);


  if(AP_ED_lNr <= 0) return 0;

  lBuf = memspc101;

  // ist Obj Definitionszeile die von Folgezeilen benoetigt wird
  irc = AP_obj_delCk1 (&lNr);
  if(irc > 0) {
    TX_Error("*** Obj. wird noch von %d Obj. benutzt",irc);

    // goto Line lNr, hilite Line, hilite Obj.
    AP_ED_lNr = lNr;
    cp1 = UTF_GetLinNr (lBuf, &llen, AP_ED_lNr);
    startPos = UTF_offset_ (cp1);

    // set curPos und scroll ...
    UI_AP (UI_FuncSet, UID_Edit_Cpos, (void*)startPos);

    // Text und Obj in der aktuellen Zeile hiliten
    UI_hili_actLn ();

    return -1;
  }


  AP_obj_del2 (AP_ED_lNr, "_", -1);

  return 0;

}
*/

/*
//=================================================================
  int AP_obj_del1 () {
//=================================================================
// Zeile AP_ED_lNr loeschen (ersten Char -> "_" aendern, Redraw.

  // printf("AP_obj_del1 %d\n", AP_ED_lNr);

  if(AP_ED_lNr <= 0) return 0;


  AP_obj_del2 (AP_ED_lNr, "_", 1);

  return 0;

}


//=================================================================
  int AP_obj_del2 (int lNr, char *tx1, int mode) {
//=================================================================
// mode: 0=restore
// mode: 1=Delete; (< AP_SMD_del)
// mode:-1=Change   (< AP_obj_del0, UNUSED)
// tx1: bei mode=delete: "_" 


  int    irec;
  int   llen;
  // long   llen;
  char   *cp1, *lBuf;

  printf("AP_obj_del2 lnr=%d mode=%d\n",lNr,mode);

  // if(mode == -1) {
    // // geht ned ; UI_undo_ck_lNr soll Nr der sel. Undo-Line liefern
    // // gibts diese Zeile schon in der Undo-Tabelle ?
    // UI_undo_ck_lNr (&irec, lNr);
    // if(irec >= 0) {
      // // change
      // UI_undo_work(irec, 0);
      // return 0;
    // }
    // mode = 1;
  // }

  // cpos of Line AP_ED_lNr abfragen
  lBuf = memspc101;
  cp1 = UTF_GetLinNr (lBuf, &llen, lNr);
  // cp1 = UTF_GetPosLnr (&llen, lNr);
  if(llen < 0) {
    TX_Error("**** AP_obj_del2 E001 ");
    return 0;
  }
  
  // printf("  lnr=%d llen=%d tx=|%s|\n",lNr,llen,lBuf);

  // Delete: eh schon deleted: skip.
  if(mode == 1) {
    if(lBuf[0] == '_') return 0;
  }

  // add Entry in Undo-List
  // if(mode == 1) UI_undo_add1 (cp1, (int)AP_ED_lNr, 'd');
  if(mode == 1) UI_undo_add1 (cp1, lNr, 'd');

  // printf("AP_obj_del2 lnr=%d llen=%d tx=|%s|\n",lNr,llen,lBuf);


  // change this character im Memory -> "_"
  // UTF_txt_chg (cp1, tx1, 1);
  APED_undo_do (lNr, cp1, 1);   // delete line in mem


  // reload gesamte Datei (mem->Window)
  if(AP_src == AP_SRC_EDI)
    ED_load__ ();
  else 
    ED_Reset ();  // ED_lnr_act = 0;
    // ED_set_lnr_act (lNr-1);


  // imply END-Button (Redraw)
  // UI_AP (UI_FuncSet, UID_but_END, NULL);
  ED_work_END (0);



  return 0;

}
*/


//=================================================================
  long AP_cre_defHdr (int typ, int istart) {
//=================================================================
/// \code
/// create ObjHeader zB "P20="
/// wc3-Source: ED_add_objInd
/// \endcode


  int   i1;
  long  ind;
  char  cmd[64];


  // printf("AP_cre_defHdr %d %d\n",typ,istart);


  // den naechsten freien ObjInd holen
  ind = DB_QueryNxtFree (typ, istart);
  //TX_Print(" next free Ind = %d",ind);

  // make name from typ and DB-index
  APED_oid_dbo__(cmd, typ, ind);
  strcat(cmd,"=");
  // printf("cmd=|%s|\n",cmd);


  // add cmd to active Line;
  i1 = ED_add_Def (cmd);
  if(i1 < 0) return i1;

  return ind;

}


//================================================================
  int AP_cut_defHdr (int *typ, long *dbi, char *cbuf) {
//================================================================
/// \code
/// extract header from Definitionline
/// see also APED_dbo_oid
/// Input:
///   cbuf  Definitionline
/// Output:
///   typ, dbi
/// \endcode

  char  *p1, cTyp;

  // printf("AP_cut_defHdr %12.12s\n",cbuf);

  p1 = cbuf;

  // skip leading blanks
  while (*p1 == ' ')  ++p1;

  // copy 1 char and change to uppercase
  if(islower(*p1)) cTyp = toupper(*p1);
  else cTyp = *p1;
  *typ = AP_typ_typChar (cTyp);
  ++p1;


  // find delimiter & copy ind
  // for(i1=0; i1<30; ++i1) {
  // }

  // decode dbi
  *dbi = atol(p1);

    // printf("ex AP_cut_defHdr %d %d %12.12s\n",*typ,*dbi,cbuf);

  return 0;

}


//================================================================
  int AP_startup_params (int iRun) {
//================================================================
// AP_startup_params                       work startup-parameters
// - iRun = 0 - before GL is up;
// - iRun = 1 - after GL is up;
// Out:
//   retCode of iRun=0    mode VWR|CAD|MAN

#define SIZ_PRGSTAT   32
static char parStat[SIZ_PRGSTAT];   // 1=not-yet-processied; 0=already-done
static int mode__;

  int       irc, ii, i1, i2, i3;
  char      *cmd, s1[320];
  stru_FN   mfn;


  // printf("AP_startup_params-in iRun=%d AP_argNr=%d\n",iRun,AP_argNr);

  irc = 0;

  // process startparameters

  if(iRun == 0) {
    for(i1 = 0; i1 < SIZ_PRGSTAT; ++i1) parStat[i1] = 0;
  }
    // TESTBLOCK
    // for(i1 = 0; i1 < SIZ_PRGSTAT; ++i1) printf("parStat[%d]=%d\n",i1,parStat[i1]);
    // END TESTBLOCK



  //----------------------------------------------------------------
  ii = 1;

  L_next_par:
  if(ii >= AP_argNr) goto L_exit;

    if(AP_argNr >= SIZ_PRGSTAT) {printf("***** ERROR AP_startup_params\n"); return -1;}

    cmd = AP_argv[ii];
       // printf(" AP_startup_params1-nxtpar iRun=%d ii=%d |%s|\n",iRun,ii,cmd);
    if(parStat[ii]) goto L_not_found;



    //================================================================
    if(iRun == 0) {

      //------------------------------------------------------------
      if(!strncmp(cmd, "mode_cad", 8)) {
        // iStartMode = 1;
        if(strlen(cmd) > 8) {
          sscanf (&cmd[8],"_%d_%d",&i2, &i3);
            // printf(" cadIni = %d %d\n",i2, i3);
          AP_stat.cadIniM = i2;
          AP_stat.cadIniS = i3;
        }
        mode__ = UI_MODE_CAD;

      //------------------------------------------------------------
      } else if(!strcmp(cmd, "mode_man")) {
        // iStartMode = 2;
        mode__ = UI_MODE_MAN;

      //------------------------------------------------------------
      } else if(!strcmp(cmd, "noTB")) {
        UI_ToolBars (1, 0);
        // return 0;

      } else if(!strcmp(cmd, "noMB")) {
        UI_ToolBars (2, 0);
        // return 0;

      } else if(!strcmp(cmd, "noBrw")) {
        UI_brw__ (-1);           // off
        // return 0;

      //------------------------------------------------------------
      // comp = compile DLL's - Checkbox aktivieren
      } else if(!strcmp(cmd, "comp")) {
        i1 = 1;
        UI_AP (UI_FuncSet, UID_ckb_comp, PTR_INT(i1));   // TRUE=1
        // return 0;

      } else goto L_not_found;


    //================================================================
    } else if(iRun == 1) {

      //------------------------------------------------------------
      // run
      if(!strcmp(cmd, "run")) {
        // UI_but__ ("butEND");     // Ausfuehren END-Button
        i1 = ED_work_END (0);
        // return 0;

      //------------------------------------------------------------
      // debug = debug ON for following DLL
      } else if(!strcmp(cmd, "AP_test__")) {
        AP_test__ ();
        // return 0;

//     //------------------------------------------------------------
//     } else if(!strcmp(cmd, "testdll")) {
//       AP_stat.tstDllStat = 1; // 0=normal (OFF); 1=testdll_ON
//       AP_testdll__ (0);
//       return 0;

      //------------------------------------------------------------
      // exit = shutdown
      } else if(!strcmp(cmd, "exit")) {
        UI_win_main (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncKill));
        // goto L_exit;

      //------------------------------------------------------------
      // Sofortausstieg - ohne sichern..
      } else if(!strcmp(cmd, "crashEx")) {
        AP_exit__ (1);      // shutdown
        exit (0);

      //------------------------------------------------------------
      }
      // test if has filetype; if not: start plugin.
      if(UTX_ftyp_s (s1, cmd, 0) < 0) {
        TX_Print ("*** no filetype - start plugin %s", cmd);
        irc = AP_plu_exec (cmd);
        // set AP_stat.mdl_stat = MDLSTAT_loaded - do not load model after plugin
        if(irc >= 0) AP_stat.mdl_stat = MDLSTAT_loaded;
        goto L_next;
      }

      //------------------------------------------------------------
      // load modelfilename into AP_mod_sym/AP_mod_fnam/AP_mod_ftyp
      i1 = MDLFN_oFn_fNam (&mfn, cmd);
        MDLFN_dump_ofn (&mfn, "AP_startup_params2");
      if(i1 >= 0) {
        if(!strcmp (mfn.fTyp, "cmd")) { // test for commandfilename .cmd
          irc = AP_rcmd (cmd);
          if(!irc) AP_stat.start_rcmd = 1;

        } else { // cad-modelname
          MDLFN_set__ (&mfn);  // copy into AP_mod_sym/AP_mod_fnam/AP_mod_ftyp
          MDLFN_ffNam_AP (s1); // get modelname from AP_mod_dir+AP_mod_fnam+AP_mod_ftyp
          // AP_Mod_load_fn (s1, 0);
          AP_Mod_load_init (s1, 0);
          // i1 = AP_Mod_load_fn (cmd, 0);
          // DL_Redraw ();
        }

      //------------------------------------------------------------
      } else {
        printf("***** ERROR AP_startup_params |%s|\n",cmd);
        goto L_not_found;
      }

    }


    //================================================================
    L_next:
      parStat[ii] = 1;

    L_not_found:
      ++ii;
      goto L_next_par;


  //----------------------------------------------------------------
  L_exit:

    if(iRun == 1) {
        // printf(" mode__=%d\n",mode__);

      // set radiobuttons VWR|CAD|MAN
      UI_main_set__ (mode__);

      // activate VWR|CAD|MAN (imods)
      if(mode__ == UI_MODE_MAN) {
        UI_but__ ("MAN");

      } else if(mode__ == UI_MODE_CAD) {
        // if mode == CAD then check for startFunction
        if(AP_stat.cadIniM >= 0)
        // activate CAD-menu and inputfields
        IE_cad_init__ (AP_stat.cadIniM, AP_stat.cadIniS);
        else UI_but__ ("CAD");

      } else {
        // UI_but__ ("VWR");
        UI_VWR_ON ();  // do not if VWR=on; first of buttonChain = ON !
      }
    }

    // printf("ex-AP_startup_params %d\n",irc);
  return irc;

}


/* replaced by AP_startup_params
//================================================================
  int AP_init__ () {
//================================================================
// work startup-parameters, load defaultmodel
// retCode = mode VWR|CAD|MAN
 
  static char iniNew = 0;     // 0=uninitialized; 1=DefModelLoaded; 2=new;

  int    irc, i1, i2, i3, imods;


  // printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA \n");
  // printf("AP_init__ iniNew=%d AP_argNr=%d\n",iniNew,AP_argNr);
  // for(i1=0; i1<AP_argNr; ++i1) printf(" %d =|%s|\n",i1,AP_argv[i1]);


  if(iniNew > 0) return 0;
  AP_stat.cadIniM = -1;  // do not start with CAD-function
  imods = UI_MODE_VWR;

  // process startparameters
  i1 = 1;

  //----------------------------------------------------------------
  L_next_par:
  if(AP_argNr > i1) {
      // printf(" AP_init__-nxtpar %d |%s|\n",i1,AP_argv[i1]);

    //------------------------------------------------------------
    if(!strncmp(AP_argv[i1], "mode_cad", 8)) {
      // iStartMode = 1;
      if(strlen(AP_argv[i1]) > 8) {
        sscanf (&AP_argv[i1][8],"_%d_%d",&i2, &i3);
          // printf(" cadIni = %d %d\n",i2, i3);
        AP_stat.cadIniM = i2;
        AP_stat.cadIniS = i3;
      }
      imods = UI_MODE_CAD;


    //------------------------------------------------------------
    } else if(!strcmp(AP_argv[i1], "mode_man")) {
      // iStartMode = 2;
      imods = UI_MODE_MAN;


    //------------------------------------------------------------
    } else {
      // work 2 next startparameters
      irc = AP_work__ (AP_argv[i1], AP_argv[i1+1]);
      if(irc > 0) ++i1;
    }


    ++i1;
    goto L_next_par;
  }

  // AP_defLoad (0);             // impliziert ein AP_init__ (rekurs!)


    // printf("ex-AP_init__ %d AP_mod_fnam=|%s|\n",imods,AP_mod_fnam);

  return imods;

}
*/


//================================================================
  int AP_testdll__ (int mode) {
//================================================================
// mode == 0:  connect dynLib xa_test.so
// mode == 1:  unload, recompile, connect dynLib xa_test.so

// was APP_dll_load
 

  static void  *dll1 = NULL;
  static char  *dllNam = "testdll";

  int   irc;
  char  s1[128];


  printf("AP_testdll__ %d\n",mode);


//   //----------------------------------------------------------------
//   if(mode != 1) goto L_load;
// 
//   // unload dll
//   irc = OS_dll__ (&dll1, FUNC_UNLOAD, NULL);
//   if(irc < 0) {
//     TX_Error("AP_testdll__: cannot close dyn. Lib.");
//     return -1;
//   }
// 
// 
//   // recompile
//   OS_dll__ (&dll1,  FUNC_RECOMPILE, dllNam);  // rebuild
// 
// 
// 
//   //----------------------------------------------------------------
//   L_load:
//   // sprintf(cBuf, "%s%s",OS_bin_dir_get(),dllNam);
//     // printf("   _dll_load |%s|\n",cBuf);
//   // exit if dll does not exist;  else load DLL
//   // if(OS_checkFilExist(cBuf, 1) == 0) return 0;
// 
// 
//   // load DLL (dlopen); RTLD_GLOBAL: load all funcs in dll.
//   if(&dll1) {
//     irc = OS_dll__ (&dll1, FUNC_LOAD_all, (void*)dllNam);
//     if(irc < 0) return irc;
//   }
// 

  return 0;

}


//================================================================
  int AP_tmr_CB__ (void *data) {
//================================================================
// mainTimerCallback;

static int iRctl=0;


  // printf("AP_tmr_CB__ %d %d\n",iRctl,UI_block_inp);


  if(data) {
    if(!strcmp(data,"RCTL_OFF")) { iRctl = 1; return 1; }

    if(!strcmp(data,"RCTL_ON"))  {
      OS_CTL_read_reset ();  // flush input
      iRctl = 0;
      return 1;
    }
  }

  if(iRctl) return 1;

  if(AP_stat.sysStat < 2) return 1;

  // test if input is blocked; yes: skip processing.
  if(UI_block_inp) return 1; // keep timer

/*
TODO:
remote control nur in VWR, nicht MAN, CAD;
*/


  // test remote-control
  CTRL_CB__ ();

  return 1;   // keep timer

}


//================================================================
  int AP_tmr_init () {
//================================================================
// start timer for remote control
// TODO:
//  during long operations (open/save/workLine) stop/continue remote-control-timer



  printf("AP_tmr_init ............ \n");

  GUI_timer__ (AP_tmr_CB__, 100, NULL);   // 300 ms = 0.3 s

  return 0;

}
 

// //================================================================
//   int AP_defLoad (int mode) {
// //================================================================
// /// load DefaultModel tmp/Model
// /// Input:
// ///   mode   0    load tmp/Model       
// ///   mode   2    load tmp/Model_*     (reload)
// /// see also AP_Mod_load__
//  
//   char   txbuf[512];
// 
//   printf("AP_defLoad %d\n",mode);
// 
//   // init DL, ED, WC   AP_mdl_init
//   AP_Init1 ();
// 
//   Tex_Init__ ();  // init textures
// 
//   sprintf(txbuf,"%sModel",AP_get_tmp_dir());
//   Mod_load__ (mode, txbuf, 0);
//   // AP_Mod_load__ (0); // load
// 
//   // model successful loaded;
//   // AP_mdl_modified_reset ();
// 
// 
// 
//   ED_Reset ();           // ED_lnr_act resetten
//   ED_work_END (0);       // ABARBEITEN
// 
//   Mod_flst_all (0);        // create ../tmp/Mod.lst
//   Brw_init__ ();       // fill browserWin
// 
//   // display scale
//   UI_AP (UI_FuncSet, UID_ouf_scl, NULL);
// 
// 
//   return 0;
// 
// }


//================================================================
  int AP_Init1 () {
//================================================================
// init DL, ED, WC
// used for AP_mdl_init (File/New),
//          Mod_cre__ (create-new-SubModel),
//          Mod_m2s__ move Mainmodel > Submodel

  // printf("AP_Init1\n");


  //WC_Init_all (0); Geht hier noch nicht !!
  // DB_Init();

  // in MS sind alle calls von GL_DefineDisp ohne GLB_DrawInit zwecklos !
  UI_wireCB (NULL, NULL);  // 2010-09-02


  // DL_Init (); called by WC_Init_all WC_Init DL_Init ..

  // ED_Init_All();

  WC_Init_all (0);


  // Redraw (= bei New l�schen !)
  // DL_Redraw ();   2020-10-12

  // printf("ex AP_Init1 |%s|\n",AP_mod_fnam);

  return 0;
}


//================================================================
  int AP_Init2 (int dbMode) {
//================================================================
/// clear DB & DL
//  Input:
///   dbMode   0: init all (new)
///            1: init DB and dyn-objects; not baseModels
///            2: init dyn-objects only


  printf("AP_Init2 %d\n",dbMode);

//   // init ModelNodes
//   DB_StoreModNod (0, 0, -1L);

  // reset DB
  DB_Init (dbMode);

  APT_Init (); // nur f. APT_lNr=0 f. Typ_Color-Record

  // delete DL-temp and AP_mdlbox_invalid_set
  GR_Init1 (dbMode);

  DB_clear__ ();  // clear DB-objs and DYN-objs

  // jetzt die DefaultColor nochmal setzen ! (macht eine DL-Eintrag !)
  // DEFCOL-Abarbeitung wurde mit GR_Init1 geloescht !
  // if(AP_indCol != 5)
  // AP_indCol = GL_DefColSet (&AP_defcol);

  return 0;
}


//================================================================
  int AP_Init_planes () {
//================================================================
/// \code
/// init View-Plane and ConstrPlane
/// see UCS_Reset
/// \endcode


  double d1;

  // printf("AP_Init_planes \n");


//     // reset View-Plane
//     d1 = 0.;
//     UI_AP (UI_FuncSet, UID_ouf_vwz, (void*)&d1);
//     UI_suract_keyIn (1);

/*
    // Reset ConstrPlane z-Val dazu
    // UI_AP (UI_FuncSet, UID_ouf_coz, (void*)&d1);
    UI_suract_keyIn (2);
    UI_sur_act_CB1 (NULL, 0);
*/

    // set ConstPln -> RZ
    UT3D_pl_pto_vcx_vcy (&WC_sur_act, (Point*)&UT3D_PT_NUL,
                   (Vector*)&UT3D_VECTOR_X, (Vector*)&UT3D_VECTOR_Y);
    AP_Set_ConstPl_Z ("RZ");

    // GL_SetConstrPln (1);

  return 0;

}


//====================================================================
  int AP_exit__ (int mode) {
//====================================================================
// Input:
//   mode   0=normal-exit;
//          1=crash-exit; (no save (AP_save_ex), AP_defaults_write)
//
// restore main not possible.


  int   irc;


  printf("AP_exit__ %d\n",mode);

  AP_stat.mdl_stat = MDLSTAT_save_exit;


  //----------------------------------------------------------------
  if(mode == 0) {

    // check if model is modified; if yes: save
    MDL_sav_ck__ (1);

    // save Viewparameters (Scale, Center, ..)
    // UI_view__ ("Save");

    // save defaults -> tmp/xa.rc
    AP_defaults_write();
  }


  //----------------------------------------------------------------
  AP_stat.mdl_stat = MDLSTAT_empty;     // save done

  GLT_exit ();         // glu-memspc retour

  GL_Exit__();         // OpenGL-memspc retour

  // close joints-file
  if(AP_stat.jntStat) {
    DBF_exit ();
    AP_stat.jntStat = 0;
  }

  // close app-messageFile
  APP_MSG_close ();

  // close message-file /doc/msg/msg_<lng>.txt
  MSG_close ();

  // close pipes CTRLfin CTRLpin CTRLpout
  OS_CTL_exit ();

  return 0;

}


//====================================================================
  int AP_defaults_write () {
//====================================================================
/// defaults -> ../../gCAD3D/cfg_<os>/xa.rc

  FILE    *fp1 = NULL;
  char    txbuf[512], *p1;


  printf("AP_defaults_write |%s|%s|\n",AP_mod_dir,AP_mod_fnam);
  // printf("  AP_mod_defSiz=%f\n",AP_mod_defSiz);


  // strcpy(txbuf, AP_get_bas_dir ());
  // strcat(txbuf, "tmp/xa.rc");

// #ifdef _MSC_VER
//   sprintf(txbuf,"\"%sxa.rc\"",AP_get_cfg_dir());
// #else
  sprintf(txbuf,"%sxa.rc",AP_get_cfg_dir());
// #endif
    printf(" defaults_write |%s|\n",txbuf);


  if((fp1=fopen(txbuf,"w")) == NULL) {
    TX_Print("****** OPEN ERROR FILE %s **********\n",txbuf);
    return -1;
  }


  // line 1: Open-baseDir (symbolic or full; without filename)
  strcpy(txbuf, AP_mod_dir);
  UTX_fdir_add_del (txbuf);  // add closing "/"
  fprintf(fp1, "%s\n", txbuf);


  // line 2: Language
  // sprintf(txbuf, "%s  // act. language",OS_get_lang());
  sprintf(txbuf, "%s  // act. language",AP_lang);
  txbuf[2] = '\0';
  fprintf(fp1, "%s\n", txbuf);


  // line 3: Browser f. Help ..
  if(!OS_browser[0]) OS_get_browse_htm ();
  sprintf(txbuf, "%s  // act. html-browser",OS_browser);
  fprintf(fp1, "%s\n", txbuf);


  // line 4: Defaultprinter
  sprintf(txbuf, "%s  // act. printCommand",AP_printer);
  fprintf(fp1, "%s\n", txbuf);


  // line 5: Modelname
  fprintf(fp1, "%s  // act. Modelname\n", AP_mod_fnam);


  // line 6: act. Filetype
  fprintf(fp1, "%s  // act. Filetype\n", AP_mod_ftyp);
  // strcpy(txbuf, AP_mod_dir);
  // if(txbuf[strlen(txbuf)-1] != fnam_del) strcat(txbuf, fnam_del_s);
  // strcat(txbuf, "   // act. Filetype");


  // line 7: program-Dir
  strcpy(txbuf, AP_dir_prg);
  if(txbuf[strlen(txbuf)-1] != fnam_del) strcat(txbuf, fnam_del_s);
  strcat(txbuf, "   // Dir for programs");
  fprintf(fp1, "%s\n", txbuf);


  // line 8: active application
  if(APP_act_typ == 1) {
    sprintf(txbuf, "APP %s   // active application",APP_act_nam);
  } else if(APP_act_typ == 2) { 
    sprintf(txbuf, "PRC %s   // active application",APP_act_nam);
  } else if(APP_act_typ == 3) { 
    sprintf(txbuf, "PLU %s   // active application",APP_act_nam);
  } else { 
    sprintf(txbuf, "RPC %s   // active application",APP_act_nam);
  }
  fprintf(fp1, "%s\n", txbuf);


  // line 9: Editor
  sprintf(txbuf, "%s   // Texteditor",AP_editor);
  fprintf(fp1, "%s\n", txbuf);


  // line 10: size of application-window
  // "-1000,-690"  // size of application-window
  sprintf(txbuf, "%s   // size of application-window",AP_winSiz);
  fprintf(fp1, "%s\n", txbuf);


  // line 11: DefaultModelsize AP_mod_defSiz
  sprintf(txbuf, "%f   // Modelsize",APT_ModSiz);
  fprintf(fp1, "%s\n", txbuf);


  // line 12: DefaultSymbolDirFile AP_symDir_fnam
  sprintf(txbuf, "%s   // SymbolDirFile",AP_symDir_fnam);
  fprintf(fp1, "%s\n", txbuf);


  // line 13: WindowPosition AP_winPos
  // GUI_Winpos_get (&i1, &i2);
  // sprintf(txbuf, "%d,%d    // WindowPosition",i1,i2);
  sprintf(txbuf, "%s    // WindowPosition",AP_winPos);
  fprintf(fp1, "%s\n", txbuf);


  fclose(fp1);

    printf("ex-AP_defaults_write\n");

  return 0;

}


//====================================================================
  int AP_defaults_dir () {
//====================================================================
// AP_defaults_dir             create file cfg_<os>/dir.lst = defaultdirs


  FILE      *fp1 = NULL;
  char      txbuf[512];


  // MDLFN_syFn_f_name (txbuf);   // get filename of dir.lst (<cfg>/dir.lst)
  sprintf(txbuf, "%scfg_%s%cdir.lst",AP_get_loc_dir(),OS_get_os__(),fnam_del);

  printf("AP_defaults_dir %s\n",txbuf);

  // if File exists, do not overwrite.
  if(OS_checkFilExist(txbuf,1) != 0)  goto L_exit;


  if((fp1=fopen(txbuf,"w")) == NULL) {
    TX_Print("AP_defaults_dir E001 %s\n",txbuf);
    goto L_exit;
  }

  fprintf(fp1, "# symbolic_name     path\n");

  sprintf(txbuf, "Data     %sdat%c",AP_get_loc_dir(),fnam_os_del);
  fprintf(fp1, "%s\n", txbuf);

  sprintf(txbuf, "CATALOG  %sctlg%c",AP_get_loc_dir(),fnam_os_del);
  fprintf(fp1, "%s\n", txbuf);

  sprintf(txbuf, "APPLI  %sprg%c",AP_get_loc_dir(),fnam_os_del);
  fprintf(fp1, "%s\n", txbuf);

  // sprintf(txbuf, "PLUGIN  %sprg%c",AP_get_loc_dir(),fnam_del);
  // fprintf(fp1, "%s\n", txbuf);


  fclose(fp1);

  L_exit:
    printf(" ex-AP_defaults_dir\n");
  return 0;

}


//================================================================
  int AP_SetCol__ (ColRGB *cSel) {
//================================================================
/// \code
/// set defaultColour to new colour
///
/// see also UTcol__3i APcol_defCol_3i
/// \endcode


  // printf("AP_SetCol__ %d %d %d\n",cSel->cr,cSel->cg,cSel->cb);

  AP_defcol = *cSel;

  // AP_actcol.vtra  = 0; // solid
  // AP_actcol.vsym  = 0; // shaded
  // AP_actcol.color = 0; // active: no

  return 0;

}


//================================================================
  int APcol_actColTra (ColRGB *newCol) {
//================================================================
/// \code
/// APcol_actColTra set new active colour and transparency
/// see also APcol_actCol__ (clears transparency)
/// \endcode

  AP_actcol = *newCol;

  return 0;

}


//================================================================
  int APcol_actCol__ (ColRGB *cSel) {
//================================================================
/// \code
/// APcol_actCol__                   set new active colour
/// get ColRGB from r,g,b: UTcol__3i
/// see also UI_colSel
/// \endcode


  AP_actcol = *cSel;

  // da in cSel nur 3 Bytes (RGB) definiert, byte 4 setzen ...
  AP_actcol.vtra  = 0; // solid
  AP_actcol.vsym  = 0; // shaded
  AP_actcol.color = 1; // active: yes

  return 0;

}

 
//================================================================
  int APcol_defCol_3i (int cr, int cg, int cb, int mode) {
//================================================================
/// \code
/// Input:
///   cr         red part of colour; 0-255
///   cg         green part of colour; 0-255
///   cb         blue part of colour; 0-255
///   mode    1  set defaultColour to new colour
///
/// see also UTcol__3i AP_SetCol__ APcol_actCol__
/// \endcode


  // printf("APcol_defCol_3i %d %d %d\n",cr,cg,cb);

  AP_defcol.cr = cr;
  AP_defcol.cg = cg;
  AP_defcol.cb = cb;

  // if(mode > 0) AP_indCol = GL_DefColSet (&AP_defcol);

  return 0;

}


//================================================================
  int APcol_defCol__ (ColRGB *cSel) {
//================================================================
/// \code
/// define default-color
/// see UI_WinDefTx
/// see TSU_exp_wrlCol
/// \endcode

  // printf("APcol_defCol__ %d %d %d\n",cSel->cr,cSel->cg,cSel->cb);

  AP_SetCol__ (cSel);            // set AP_defcol = cSel
  GL_init_defCol (cSel);         // set GL-defCol = cSel
  DL_Redraw();                   // redraw with new color

  return 0;

}
 

//====================================================================
  int AP_defaults_read () {
//====================================================================
// read defaults from <base>/cfg_Linux/xa.rc
//  1   AP_mod_dir
//  2   AP_lang
//  3   OS_browser
//  4   AP_printer
//  5   AP_mod_fnam
//  6   AP_mod_ftyp
//  7   AP_dir_prg
//  8   APP_act_typ, APP_act_nam
//  9   AP_editor
// 10   AP_winSiz
// 11   AP_mod_defSiz
// 12   SymbolDirFile AP_symDir_fnam
//
// see also AP_defaults_write


  char    *p1, *txbuf;
  FILE    *fp1 = NULL;
  

  txbuf = memspc011;
  sprintf(txbuf,"%sxa.rc",AP_get_cfg_dir());

    printf("AP_defaults_read |%s|\n",txbuf);


  if((fp1=fopen(txbuf,"r")) == NULL) {
    printf("****** OPEN ERROR FILE %s **********\n",txbuf);
    TX_Print("****** OPEN ERROR FILE %s **********",txbuf);
    return -1;
  }


  // Open-baseDir
  fgets (txbuf, 120, fp1);
  // sscanf(txbuf, "%s",AP_mod_dir); // only 1. word, rest is comment
  UTX_CleanCommC (txbuf);  // ACHTUNG: blanks im Dirnam !
  strcpy (AP_mod_dir, txbuf);
  printf(" AP_mod_dir=|%s|\n",AP_mod_dir);

  // if AP_mod_dir is empty replace with default-dataDir
  // eg after open model without symDir ?
  if(strlen(AP_mod_dir) < 2) sprintf(AP_mod_dir, "%sdat%c",AP_get_loc_dir(),fnam_del);

  // // add closing '/' if necessary
  // UTX_fdir_add_del (sOut);


  // Zeile 2: Language
  fgets (txbuf, 120, fp1);
  strncpy(AP_lang, txbuf, 2);
  AP_lang[2] = '\0';
  printf(" AP_lang=|%s|\n",AP_lang);


  // Zeile 3: Browser f. Help ..
  fgets (txbuf, 120, fp1);
  sscanf(txbuf, "%s",OS_browser); // only 1. word, rest is comment
  printf(" OS_browser=|%s|\n",OS_browser);


  // Zeile 4: Defaultprinter
  fgets (txbuf, 120, fp1);
  p1 = UTX_memFind (txbuf, "//");
  if(p1) *p1 = '\0';
  UTX_CleanCR (txbuf);
  strcpy(AP_printer, txbuf);
  printf(" AP_printer=|%s|\n",AP_printer);


  // Zeile 5: Modelname
  fgets (txbuf, 120, fp1);
// ignore last modelname; set AP_mod_fnam empty
  AP_mod_fnam[0] = '\0';
  // sscanf(txbuf, "%s",AP_mod_fnam); // only 1. word, rest is comment
  // printf(" Modelname=|%s|\n",AP_mod_fnam);


  // Zeile 6: Save-baseDir AP_mod_dir
  fgets (txbuf, 120, fp1);
  // UTX_CleanCommC (txbuf);   // ACHTUNG: blanks im Dirnam !
  // strcpy (AP_mod_dir, txbuf);
  // printf(" AP_mod_dir=|%s|\n",AP_mod_dir);
  sscanf(txbuf, "%s",AP_mod_ftyp); // only 1. word, rest is comment
  // get interger-filetyp AP_mod_iftyp
  AP_mod_iftyp = AP_iftyp_ftyp (AP_mod_ftyp);


  // Zeile 7: program-baseDir AP_dir_prg
  fgets (txbuf, 120, fp1);
  // sscanf(txbuf, "%s",AP_mod_dir); // only 1. word, rest is comment
  UTX_CleanCommC (txbuf);   // ACHTUNG: blanks im Dirnam !
  strcpy (AP_dir_prg, txbuf);
  printf(" AP_dir_prg=|%s|\n",AP_dir_prg);


  // Zeile 8: active application
  if(fgets (txbuf, 120, fp1)) {
    UTX_CleanCommC (txbuf);   // ACHTUNG: blanks im Dirnam !
    strcpy (APP_act_nam, &txbuf[4]);
    if(txbuf[0] == 'A') APP_act_typ = 1;    // "APP "
    if(txbuf[1] == 'R') APP_act_typ = 2;    // "PRC "
    if(txbuf[1] == 'L') APP_act_typ = 3;    // "PLU "
    if(txbuf[0] == 'R') APP_act_typ = 4;    // "RPC "
      printf(" active application=|%s| %d\n",APP_act_nam,APP_act_typ);
  }


  // Zeile 9: Texteditor
  if(fgets (txbuf, 120, fp1)) {
    sscanf(txbuf, "%s",AP_editor); // only 1. word, rest is comment
    printf(" AP_editor=|%s|\n",AP_editor);
  }


  // line 10: size of application-window; def = "-1000,-690" see UI_win_main
  if(fgets (txbuf, 120, fp1)) {
    sscanf(txbuf, "%s",AP_winSiz); // only 1. word, rest is comment
  } else strcpy(AP_winSiz, WinSizMin__); // "-800,-240");
    printf(" AP_winSiz=|%s|\n",AP_winSiz);


  // line 11: DefaultModelsize AP_mod_defSiz; 500.; see also APT_ModSiz
  if(fgets (txbuf, 120, fp1)) {
    sscanf(txbuf, "%lf",&AP_mod_defSiz); // only 1. word, rest is comment
  } else AP_mod_defSiz = 500.;
    printf(" DefaultModelsize=%f\n",AP_mod_defSiz);
  if(AP_mod_defSiz < 0.1) {
    printf(" ****** ERROR AP_defaults_read - DefaultModelsize %f\n",AP_mod_defSiz);
    AP_mod_defSiz = 500.;
  }


  // line 12   SymbolDirFile AP_symDir_fnam
  if(fgets (txbuf, 120, fp1)) {
    p1 = strchr(txbuf, ' ');
    if(p1) {
      if((p1 - txbuf) < 128) {
        sscanf(txbuf, "%s",AP_symDir_fnam); // only 1. word, rest is comment
      } else {
        printf("***** ERR AP_defaults_read line 12");
      }
    }
  }


  // line 13   winPos -> AP_winPos
  strcpy(AP_winPos, "0,0");
  if(fgets (txbuf, 120, fp1)) {
    p1 = strchr(txbuf, ' ');
    if(p1) {
      if((p1 - txbuf) < 128) {
        sscanf(txbuf, "%s",AP_winPos); // only 1. word, rest is comment
          printf("AP_defaults_read-AP_winPos |%s|\n",AP_winPos);
      } else {
        printf("***** ERR AP_defaults_read line 13");
      }
    }
  }
    printf(" AP_winPos=|%s|\n",AP_winPos);

  fclose(fp1);

    // printf("ex-AP_defaults_read \n");
    // DEB_mcheck__ ();

  return 0;

}


//================================================================
  int AP_DllLst_write () {
//================================================================
// alle ../tmp/*.dll bzw *.so in Datei ausgeben; xa_*.dll jedoch ausfiltern.
// see also UTX_dir_listf


  int   i1, i2, iNr, ftyplen;
  char  *p1, cbuf1[256];
  FILE  *fpo;


  printf("AP_DllLst_write \n");


  // try to open outfile
  sprintf(cbuf1, "%splugins.lst",AP_get_tmp_dir());
    // printf(" Dll.lst=|%s|\n",cbuf1);
  if((fpo=fopen(cbuf1,"w")) == NULL) {
    printf("***** AP_DllLst_write E001 %s\n",cbuf1);
    // TX_Print("AP_DllLst_write E001 %s",cbuf1);
    return -1;
  }


  // Searchpath
  sprintf(cbuf1, "%splugins%c",OS_bin_dir_get(),fnam_del);
    // printf(" path-plugins=|%s|\n",cbuf1);


#if defined _MSC_VER || __MINGW64__
      ftyplen = 4; // ".dll"
#else
      ftyplen = 3; // ".so"
#endif


  iNr = 0;
  OS_dir_scan_ (cbuf1, &iNr);   // Init
  // printf(" nach OS_dir_scan_ %d\n",iNr);

  if(iNr > 0)  {

    for (;;) {
      OS_dir_scan_ (cbuf1, &iNr);
      // printf(" n.scan %d |%s|\n",iNr,cbuf1);
      if(iNr < 0) break;

      // filter filetyp
      i1 = strlen(cbuf1);

#if defined _MSC_VER || __MINGW64__
      for(i2=i1-3; i2<i1; ++i2) cbuf1[i2] = tolower (cbuf1[i2]);
      if(strncmp(&cbuf1[i1-4], ".dll", 4)) continue;
#else
      if(strncmp(&cbuf1[i1-3], ".so", 3)) continue;
#endif

      // skip directories
      if(OS_check_isDir(cbuf1) == 0) continue;

      // strip directory - find last '/' or '\'(makes |t1| aus |/mnt/x/t1|)
#if defined _MSC_VER || __MINGW64__
      p1 =  UTX_find_strrchrn (cbuf1, "/\\");
#else
      p1 = strrchr (cbuf1, fnam_os_del);
#endif

      if(p1) ++p1;
      else p1 = cbuf1;

//       // "xa_*" ausfiltern ...
//       if(!strncmp(p1, "xa_", 3)) continue;

      // skip all processes
      if(!strncmp(p1, "PRC_", 4)) continue;

      p1[strlen(p1)-ftyplen] = '\0'; // remove .so /.dll

      fprintf(fpo,"%s\n",p1);
        // printf(" add plugin %d |%s|%s|\n",iNr,cbuf1,p1);
    }
  }

  fclose(fpo);

  return 0;

}


//================================================================
  int AP_get_dir_open (char *cbuf) {
//================================================================
  strcpy(cbuf, AP_mod_dir);
  return 0;
}


//================================================================
  int AP_get_modnam (char *cbuf) {
//================================================================
  strcpy(cbuf, AP_mod_fnam);
  return 0;
}


/*
//================================================================
  int AP_set_dir_open (char *newDir) {
//================================================================
// sets AP_mod_dir & AP_mod_sym from newDir

  int   irc;


  printf("AP_set_dir_open |%s|\n",newDir);

 
  // if(!strcmp(newDir, AP_mod_dir)) return 0;

  strcpy (AP_mod_dir, newDir);


  // set sym; else sym = leer
  irc = Mod_sym_get2 (AP_mod_sym, newDir, 0);

  if(irc < 0) {
    printf(" ************* ERRFIX SYMBOL ********\n");
    strcpy (AP_mod_sym, ".");
  }

  return irc;

}


//================================================================
  int AP_set_dir_save (char *newDir) {
//================================================================

  // if(!strcmp(newDir, AP_mod_dir)) return 0;

  strcpy(AP_mod_dir, newDir);

  // set sym; else sym = leer
  Mod_sym_get2 (AP_mod_sym, newDir, 0);

  return 0;

}


// UNUSED
//====================================================================
  int AP_set_modsiz (double newssiz) {
//====================================================================

  WC_Init_Modsiz (newssiz);

  GL_InitModelSize (newssiz, 0);

  // imply END-Button (Redraw)
  ED_Reset ();  // ED_lnr_act = 0;
  ED_work_END (0);


  return 0;

}
*/


//====================================================================
  int AP_work__ (char *cmd, char *cmd1) {
//====================================================================
// AP_work__          work startparameters
// Input:
//   cmd, cmd1   2 consecutive startparameters
// Retcode:      0 = OK, cmd done
//               1 = OK; cmd & cmd1 done


  int       irc, i1;
  long      l1;
  char      s1[80];
  stru_FN   mfn;


  // printf("AP_work__ |%s|\n",cmd);

/*
    // TESTBLOCK
    if(!strcmp(cmd, "test")) {
      // UI_wait_Esc__ ();
      return 0;
    }
    // END TESTBLOCK
*/


  //------------------------------------------------------------
  // rcmd = start with remoteCommandFile
  if(!strcmp(cmd, "rcmd")) {
    // copy file <cmd1> -> <tmpDir>rcmd.txt
    irc = AP_rcmd (cmd1);
    if(!irc) AP_stat.start_rcmd = 1;
    return 1;


  //------------------------------------------------------------
  // exit = shutdown
  } else if(!strcmp(cmd, "exit")) {
    UI_win_main (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncKill));
    return 0;


  //------------------------------------------------------------
  } else if(!strcmp(cmd, "testdll")) {
    AP_stat.tstDllStat = 1; // 0=normal (OFF); 1=testdll_ON
    AP_testdll__ (0);
    return 0;


  //------------------------------------------------------------
  // // load = open Filenname
  // } else if(!strcmp(cmd, "load")) {
    // printf(" load |%s|\n",cmd1);
    // UI_open__ (cmd1, AP_mod_dir);
    // return 1;


//   //------------------------------------------------------------
//   // das Default-Modell laden
//   } else if(!strcmp(cmd, "defLoad")) {
//     AP_defLoad (0);
//     return 1;
// 
// 
//   //------------------------------------------------------------
//   // das Default-Modell laden
//   } else if(!strcmp(cmd, "reLoad")) {
//     AP_defLoad (2);
//     return 1;


  //------------------------------------------------------------
  // run
  } else if(!strcmp(cmd, "run")) {
    // UI_but__ ("butEND");     // Ausfuehren END-Button
    i1 = ED_work_END (0);
    return 0;


  //------------------------------------------------------------
  // comp = compile DLL's - Checkbox aktivieren
  } else if(!strcmp(cmd, "comp")) {
    i1 = 1;
    UI_AP (UI_FuncSet, UID_ckb_comp, PTR_INT(i1));   // TRUE=1
    return 0;


  //------------------------------------------------------------
  // debug = debug einscahlten fuer nachfolgende DLL
  } else if(!strcmp(cmd, "AP_test__")) {
    AP_test__ ();
    return 0;


  //------------------------------------------------------------
  // Sofortausstieg - ohne sichern..
  } else if(!strcmp(cmd, "crashEx")) {
    AP_exit__ (1);      // shutdown
    exit (0);


  //------------------------------------------------------------
  } else if(!strcmp(cmd, "noTB")) {
    UI_ToolBars (1, 0);
    return 0;


  } else if(!strcmp(cmd, "noMB")) {
    UI_ToolBars (2, 0);
    return 0;


  } else if(!strcmp(cmd, "noBrw")) {
    UI_brw__ (-1);           // off
    return 0;

  } 



  //================================================================
  // test if has filetype; if not: start plugin.
  if(UTX_ftyp_s (s1, cmd, 0) < 0) {
    TX_Print ("*** no filetype - start plugin %s", cmd);
    return AP_plu_exec (cmd);
  }


  //================================================================
  // load modelfilename into AP_mod_sym/AP_mod_fnam/AP_mod_ftyp
  i1 = MDLFN_oFn_fNam (&mfn, cmd);
  if(i1 < 0) goto L_test_dll;
  MDLFN_set__ (&mfn); // copy into AP_mod_sym/AP_mod_fnam/AP_mod_ftyp
//   i1 = AP_Mod_load_fn (cmd, 0);
//   if(i1 < 0) goto L_test_dll;

  return 0;



  //================================================================
  // try to start DLL (weil kein anderes command gefunden)
  L_test_dll:
    TX_Error ("Startparameter unused - |%s|",cmd);
  return 0;

}


//================================================================
  int AP_rcmd (char *fNam) {
//================================================================
// copy file <cmd1> -> <tmpDir>rcmd.txt

  char    s1[400];

  printf("AP_rcmd |%s|\n",fNam);


  // test if fNam exists
  if(!OS_checkFilExist(fNam, 1)) {
    TX_Error ("start remoteCommandFile - file not exists");
    return -1;
  }


  // copy file <fNam> -> <tmpDir>rcmd.txt
  sprintf(s1, "%srcmd.txt", AP_get_tmp_dir());
    printf("AP_rcmd-to |%s|\n",s1);
  OS_file_copy (fNam, s1);


  return 0;

}


//================================================================
  int AP_ck_build () {
//================================================================
// AP_ck_build     test if reBuild wanted, test if compiler exists 
// retCode:    1=yes,build;  0=no; -1=Error-compmiler


  int    iComp;


  // compile_DLL - test Checkbox UID_ckb_comp 
  iComp = AP_stat.comp;   // 1=yes,build;  0=no;
    printf(" AP_ck_build-iComp=%d\n",iComp);


  // if compile is on: test if compiler & Linker exists
  if(iComp) {
    // yes, do compile ..
    if(AP_stat.build < 1) {
      TX_Print("**** cannot build - cannot locate compiler ...");
      iComp = -1;
    }
  }


    printf(" ex-AP_ck_build %d\n",iComp);

  return iComp;

}


//======================================================================================
  int AP_kex_exec (void **kex_dll, 
                   char *dllNam, char *fncNam, void *fncDat, int mode) {
//======================================================================================
// AP_kex_exec          start gcad-kernel-extension-dll
// Input:
//   mode     DLL_LOAD_EXEC_UNLOAD
//            DLL_LOAD_EXEC            - do not unload
//            DLL_UNLOAD               - unload
// Output:
//   kex_dll  NULL = dll not loaded;
//
// see also 
// was OS_dll_do


  int     irc;


  printf("AP_kex_exec |%s|%s| mode=%d\n",dllNam,fncNam,mode);


  if((mode == DLL_LOAD_EXEC_UNLOAD)  ||
     (mode == DLL_LOAD_EXEC))                {

    // load dll
    if(*kex_dll) {
      // already a dll loaded; unload
      irc = DLL_dyn__ (kex_dll, DLL_UNLOAD, NULL);
      if(irc < 0) return -1;
    }
    irc = DLL_dyn__ (kex_dll, DLL_LOAD_only, dllNam);
    if(irc < 0) return irc;
  }


  if((mode == DLL_LOAD_EXEC_UNLOAD)  ||
     (mode == DLL_LOAD_EXEC))                {
    // connect to function PRCE__
    irc = DLL_dyn__ (kex_dll, DLL_CONNECT, fncNam);
    if(irc < 0) return irc;


    // execute function
    irc = DLL_dyn__ (kex_dll, DLL_EXEC, fncDat);
    if(irc < 0) return -1;
  }


  // unload active dll
  if((mode == DLL_LOAD_EXEC_UNLOAD)  ||
     (mode == DLL_UNLOAD))                {
    irc = DLL_dyn__ (kex_dll, DLL_UNLOAD, NULL);
    if(irc < 0) return -1;
  }


    printf(" ex-AP_kex_exec 0\n");

  return 0;

}


//================================================================
  int AP_plu_start () {
//================================================================
// AP_plu_start          get plugin-name from user; start plugin ..
// was PLU_Loa

  int   i1;
  char  fnam[256], s1[256];


  printf("AP_plu_start \n");


  // no addOn-prog may be active.
  if(AP_stat.APP_stat != 0) {
    // ask user unload active plugin - yes / no
    sprintf(s1, "  unload active plugin - yes / no ? ");
    i1 = GUI_dlg_2b (s1, MSG_const__(MSG_ok), MSG_const__(MSG_no));
    if(i1 == 0) {
      DLL_plu_unl ();
    } else {
      return -1;
    }
  }


  // display list of plugins (see AP_DllLst_write ) let user select
  sprintf(fnam, "%splugins.lst", AP_get_tmp_dir());
  i1 = GUI_listf1__ (s1, sizeof(s1), fnam, "- select program -", "x40,y40");
  if(i1 < 0) return -1;

  UTX_ftyp_cut  (s1);     // remove the filetyp (.so|.dll)

  APP_act_typ = 3;
  UI_Set_typPrg ();
  strcpy(APP_act_nam, s1);

  // PLU_restart ();
  AP_plu_exec (APP_act_nam);

  return 0;

}


//================================================================
  int AP_plu_restart () {
//================================================================
// AP_plu_restart                restart plugin - DLL


  return  AP_plu_exec (APP_act_nam);

}



//================================================================
  int AP_plu_exec (char *dllNam) {
//================================================================
// AP_plu_exec               start plugin - DLL
// Input: name of the dll without directory & filetyp

  int       irc, iComp;
  long      l1;
  char      txbuf[SIZFNam], s2[SIZFNam];


  printf("AP_plu_exec |%s|\n",dllNam);

  // check if plugin is active ..


  //----------------------------------------------------------------
  // set error-returnAdress
  irc = ERR_SET1 ();
  if(irc) {
    MSG_get_1 (s2, 256, "pluginErr1", "%s", dllNam);
    printf("*****  %s *****\n",s2);
    // unload plugin <APP_act_nam>
    DLL_plu_unl ();
    // redraw
    DL_Redraw ();
    TX_Print("*****  %s *****\n",s2);
    return irc;
    // GUI_MsgBox (" Error in protected_routine1");    // if gtk not yet active
    // gtk_exit(1);
  }



  // disp active program-name; col 2=blue
  UI_Set_actPrg (APP_act_nam, 2);
  // AP_stat.APP_stat = 1;

  // DLL starten (rebuild, load, connect gCad_main)
  irc = DLL_plu__ (dllNam, NULL);
  // inactivate is done by AP_User_reset


  ERR_RESET ();

  return irc;

}


//================================================================
  int AP_iftyp_ftyp (char *ftyp) {
//================================================================
/// \code
/// AP_iftyp_ftyp              integer-filetyp from string-filetyp
/// Input:
///   ftyp       filetyp; size max 32
/// Output:
///   retCod     ftyp as int; 0=native=Mtyp_Gcad, 1=Mtyp_DXF, 2=Mtyp_Iges, ..
///              MBTYP_ERROR = unsupported; filetypes see in ../ut/AP_types.h
///              
/// see also AP_ftyp_iftyp ((int) -> (text))
/// \endcode

  int   lw, i1, ift;
  char  s1[SIZMFTyp + 1];


  lw = strlen(ftyp);
  if((!lw)||(lw >= SIZMFTyp)) { ift = MBTYP_ERROR; goto L_exit;}

  memcpy(s1, ftyp, lw + 1);

  for (i1=0; i1<lw; ++i1)
    if(islower (s1[i1])) s1[i1] = toupper (s1[i1]);
      // printf("AP_iftyp_ftyp |%s|%s|\n",s1,ftyp);


  //----------------------------------------------------------------
    if(!strcmp(s1, "DXF")) {
      ift = Mtyp_DXF;

    } else if((!strcmp(s1, "IGS"))||
              (!strcmp(s1, "IGE"))||
              (!strcmp(s1, "IGES"))) {           // "IG2" ???
      ift = Mtyp_Iges;

    } else if((!strcmp(s1, "STEP"))||
              (!strcmp(s1, "STP"))) {
      ift = Mtyp_Step;

    } else if(!strcmp(s1, "3DS")) {
      ift = Mtyp_3DS;

    } else if(!strcmp(s1, "LWO")) {
      ift = Mtyp_LWO;

    } else if(!strcmp(s1, "XML")) {
      ift = Mtyp_XML;

    } else if(!strcmp(s1, "SVG")) {
      ift = Mtyp_SVG;

    } else if(!strcmp(s1, "WRL")) {
      ift = Mtyp_WRL;
      // Import: VRML1; fcheck for VRML2 with AP_ImportWRL_ckTyp()
      // Export ??

    } else if(!strcmp(s1, "OBJ")) {
      ift = Mtyp_OBJ;

    } else if(!strcmp(s1, "STL")) {
      ift = Mtyp_STL;

    } else if(!strcmp(s1, "TESS")) {
      ift = Mtyp_TESS;

    } else if(!strcmp(s1, "BMP")) {
      ift = Mtyp_BMP;

    } else if((!strcmp(s1, "JPG"))||
              (!strcmp(s1, "JPEG"))) {
      ift = Mtyp_JPG;

    } else if((!strcmp(s1, "GCAD"))||
              (!strcmp(s1, "GCAZ"))) {
    ift = Mtyp_Gcad;

    } else if(!strcmp(s1, "CTLG")) {
      ift = MBTYP_CATALOG;

    } else if(!strcmp(s1, "GCAP")) {
      ift = MBTYP_APPLI;

    } else {
      ift = MBTYP_ERROR;    // unsupported filetyp
    }


  L_exit:
    // printf("ex-AP_iftyp_ftyp %d |%s|\n",ift,ftyp);
  return ift;

}


//================================================================
  int AP_ftyp_iftyp (char *ftyp, int ift) {
//================================================================
/// \code
/// AP_ftyp_iftyp       string-filetyp from integer-filetyp
/// returns ftyp as text from int
/// txt-> i m. AP_iftyp_ftyp
/// \endcode


       if(ift == 1)  strcpy(ftyp, "dxf");
  else if(ift == 2)  strcpy(ftyp, "igs");
  else if(ift == 3)  strcpy(ftyp, "stp");
  else if(ift == 4)  strcpy(ftyp, "3ds");
  else if(ift == 5)  strcpy(ftyp, "lwo");
  else if(ift == 10) strcpy(ftyp, "WRL");
  else if(ift == 11) strcpy(ftyp, "OBJ");
  else if(ift == 12) strcpy(ftyp, "TESS");
  else if(ift == 20) strcpy(ftyp, "bmp");
  else               strcpy(ftyp, "dat");

  // printf("ex AP_ftyp_iftyp |%s| %d\n",ftyp,ift);
  return 0;

}


//================================================================
  int AP_Mod_load_init (char *fn) {
//================================================================
// AP_Mod_load_init           load Model from file <fn> and create <tmp>/Mod_.mod_in
//   not used for load subModels
// Input:
//   fn       full-filename <directory|symbol>/<filename>.<filetyp>
//

// Output:
// mode     0 = load neu (do not save active model)
//          1 = insert; do not write Title & do not rescale
//          2 = initial load without model; create empty model unknown.gcad
//   AP_mod_sym, AP_mod_dir, AP_mod_fnam, AP_mod_ftyp, AP_mod_iftyp


  int    irc;


  printf("AP_Mod_load_init |%s|\n",fn);

  // load Model from file <fn> 
  irc = AP_Mod_load_fn (fn, 0);
  if(irc) return irc;

  // create complete model native as <tmp>/Mod_.mod_out
  irc = MDL_sav_gcad (0, 0);
  if(irc) return irc;

  // rename <tmp>/Mod_.mod_out -> <tmp>/Mod_.mod_in
  sprintf(memspc011, "%sMod_.mod_out",AP_get_tmp_dir());
  sprintf(memspc012, "%sMod_.mod_in",AP_get_tmp_dir());
  OS_file_rename (memspc011, memspc012);


  if(AP_stat.db_cseg_eom) {
    // ERR_DB_CSEG_EOM has been, already fixed; but must redraw;   2024-06-19
      // printf(" AP_Mod_load_init-reDraw\n");
    ED_work_END (1);  // error DB_allocCDAT
    AP_stat.db_cseg_eom = 0;
  }


  return 0;

}


//================================================================
  int AP_Mod_load_fn (char *fn, int mode) {
//================================================================
// AP_Mod_load_fn           load Model from file <fn>
//   not used for load subModels
// Input:
//   fn       full-filename <directory|symbol>/<filename>.<filetyp>
//   mode     0 = load neu (do not save active model)
//            1 = insert; do not write Title & do not rescale
//            2 = initial load without model; create empty model unknown.gcad
// Output:
//   AP_mod_sym, AP_mod_dir, AP_mod_fnam, AP_mod_ftyp, AP_mod_iftyp


  int      irc, i1;
  char     ffn[SIZMFTot], syFn[SIZMFTot];
  stru_FN  ofNam;


  // printf("\nAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA \n");
  // printf("AP_Mod_load_fn |%s| %d\n",fn,mode);

  UI_block__ (1, 1, 1);  // block all

  AP_stat.mdl_stat = MDLSTAT_loading;


  if(mode != 1) AP_stat.mdl_load = 0; // loading-new-primary-model


  L_init:
  //----------------------------------------------------------------
  if(mode == 2) {
      // MDLFN_dump__ ("AP_Mod_load_fn-mode-2");
    // create new empty model "unknown", load into editor
    irc = MDL_load_new__ ();
    if(irc < 0) {TX_Print("***** error creating empty model"); goto L_exit;}
    goto L_start;
  }


  //----------------------------------------------------------------
  // get filenameObj ofNam (symDir,fnam,ftyp ..) of new modelName (can be symbolic)
  irc = MDLFN_oFn_fNam (&ofNam, fn);
  if(irc < 0) {TX_Print("***** error symbolic filepath"); goto L_noFile;}

    // TESTBLOCK
    // MDLFN_dump_ofn (&ofNam, "AP_Mod_load_fn-ofNam");
    // END TESTBLOCK
 

  // get absolute-directory
  MDLFN_ffNam_oFn (ffn, &ofNam);
    // printf("AP_Mod_load_fn-ffn |%s|\n",ffn);

  // Test if file exists (modelfile)
  irc = OS_checkFilExist (ffn, 2);  // 1=yes, 0=no
  if(irc < 1) {TX_Print("***** %s - file does not exist",ffn); goto L_noFile;}

  // get symbolic-filename
  MDLFN_syFn_oFn (syFn, &ofNam);

  // load model - import -> native code; load into editor
  MDL_load_main (syFn);

    // TESTBLOCK
    // DEB_exit();
    // END TESTBLOCK

  // add filename to list "last-used" (get filetyp of loaded model, not .gcad)
  // AP_Mod_lstAdd ();
  MDLFN_syFn_oFn (ffn, &ofNam);
    // printf(" _load_fn-ffn-2=|%s|\n",ffn);
  AP_Mod_lst_fn (ffn);


  //----------------------------------------------------------------
  // all subModels are loaded; mainModel is in memSpc.
  // Following (UI_butCB "|VWR|CAD|MAN") will work main
  L_start:
  AP_stat.mdl_stat = MDLSTAT_loaded;

  // set all subModels active for main-model
  Brw_init__ ();        // clear browser - create primary model & all subModels
  // Brw_main_init ();       // ??

    // TESTBLOCK
    // DEB_exit();
    // END TESTBLOCK
 

  // reprocess whole model
  UI_but_END ();


  //----------------------------------------------------------------
  // update mUsed in mdl_tab
  MDL_used_set__ ();

  // update mUsed and Brw-Icons of models
  if(BRW_STAT) MDL_brw_upd ();

  // display modelTitle in topBar
  UI_title_set ();



//   // find, register, disp unused-subModels
//   MDL_smuu__ ();
// 
//   // update model-icon-color
//   Brw_Mdl_stat ();


  irc = 0;


  //----------------------------------------------------------------
  L_exit:

    if(!irc) TX_Print("..... model %s loaded ..",fn);


    AP_stat.mdl_load = 1; //primary-model-is-active

    // unblock all
    UI_block__ (0, 0, 0);

      // TESTBLOCK
      // MDL_dump__ ("ex-AP_Mod_load_fn\n");
      // printf("ex-AP_Mod_load_fn %d\n",irc);
      // DEB_exit();
      // END TESTBLOCK

    return irc;


  //----------------------------------------------------------------
  L_noFile:
    // load unknown-new
    mode = 2;
    goto L_init;

}


/*
// DO NOT USE - replaced by MDL_imp__
//================================================================
  int AP_Mod_load__ (int mode) {
//================================================================
/// \code
/// load Model from file <AP_mod_dir><AP_mod_fnam><AP_mod_ftyp>
/// Input:
///   mode   0 = initial load
///          1 = insert; do not write Title & do not rescale
///   AP_mod_dir AP_mod_fnam AP_mod_ftyp AP_mod_iftyp
///
/// see also AP_defLoad UI_save_
/// \endcode

// AP_mod_fnam kann noch filtyp haben; den nach dem Laden entfernen !!


  int  irc, i1, i2, ift, impTyp, dbResiz;
  long l1, l2, lTab[8];
  char s1[256], cbuf[256], mnam[160], ftyp[32];
  void       *pa[2];



  // printf("AP_Mod_load__|%s|%s|%s| %d %d\n",
         // AP_mod_dir,AP_mod_fnam,AP_mod_ftyp,AP_mod_iftyp,mode);


  // ift = integer-Filetyp
  ift = AP_mod_iftyp;
  if(ift < 0)  {
    sprintf(cbuf,"**** cannot load - unknown filetype \"%s\"",AP_mod_ftyp);
    TX_Print(cbuf);
      // printf(" ft=|%s|\n",AP_mod_ftyp);
    return -1;
  }


  irc = 0;
  dbResiz = 0;  // yes, increase DB-size


  // UI_CursorWait (0); // wait-cursor
  UI_block__ (1, 1, 1);  // block UI

  // cbuf = filename-absolute
  sprintf(cbuf, "%s%s.%s",AP_mod_dir,AP_mod_fnam,AP_mod_ftyp);
  // mnam = Filename (without dir, with filetyp)
  sprintf(mnam, "%s.%s",AP_mod_fnam,AP_mod_ftyp);


  TX_Print("import Model %s",cbuf);



  // get ImportTyp; nativ oder Mockup. (user-checkBox)
  UI_AP (UI_FuncGet, UID_ckb_impTyp, (void*)&impTyp); // 1=Mockup, 0=native
  // printf(" impTyp=%d ift=%d\n",impTyp,ift);



  if(ift == Mtyp_Gcad) goto L_native;


  if(ift == Mtyp_DXF) {        // DXF
    // UI_WinDxfImp (NULL, (void*)UI_FuncInit3);
    // AP_ImportDxf (0, "0,0,0", cbuf);
    pa[0] = cbuf;
    s1[0] = '\0';
    pa[1] = s1;     // empty = primary main
    irc = OS_dll_do ("xa_dxf_r", "DXF_r__", pa, 0);
    if(irc < 0) return irc;
    mode = 2;
    goto L_native;


  } else if(ift == Mtyp_Iges) {
    irc = OS_dll_do ("xa_ige_r", "IGE_r__", cbuf, 0);
    AP_stru_2_txt (NULL, 0, (void*)lTab, 1L); // ask last index
    DB_size_set (lTab);                       // increase DB-size
    dbResiz = 1;                              // DB-resize done


  } else if(ift == Mtyp_Step) {
    // export into file <tmpDir>Model
    pa[0] = cbuf;
    s1[0] = '\0';
    pa[1] = s1;     // empty = primary main
    irc = OS_dll_do ("xa_stp_r", "STP_r__", pa, 0);
      // printf(" foll-OS_dll_do %d\n",irc);
    if(irc < 0) return irc;
    AP_stru_2_txt (NULL, 0, (void*)lTab, 1L); // ask last index
    DB_size_set (lTab);                       // increase DB-size
    dbResiz = 1;                              // DB-resize done
    mode = 2;
    // // rename Model -> Mdl_import.gcad
    // sprintf(s1, "%sModel", AP_get_tmp_dir());
    // sprintf(cbuf, "%sMdl_import.gcad", AP_get_tmp_dir());
      // printf(" cbuf = |%s|\n",cbuf);
    // OS_file_rename (s1, cbuf);
    // load Mdl_import.gcad
    goto L_native;


  } else if(ift == Mtyp_WRL) {
    ift = AP_ImportWRL_ckTyp (cbuf);
      printf(" AP_Mod_load__-wrl-ift=%d\n",ift);
    if(ift == Mtyp_WRL) {
      // create sourceLine "M# = <fnam> <origin>"
      UI_loadMock_CB (mnam, AP_mod_dir);
    }
    if(ift == Mtyp_WRL2) {
      irc = OS_dll_do ("xa_vr2_r", "VR2_r__", cbuf, 0);
      if(irc < 0) goto L_exit;
      // dbResiz = 1;                             // DB-resize done
    }


  } else if(ift == Mtyp_3DS) {        // 3DS
    AP_Import3ds (0, "0,0,0", 1., cbuf);


  } else if(ift == Mtyp_LWO) {        // LWO
    AP_ImportLwo (0, "0,0,0", 1., cbuf);


  } else if(ift == Mtyp_XML) {        // XML
    AP_ImportXML (cbuf);


  // 10-19  tess-Formate   tess wrl wrl2 obj stl
  } else if((ift >= Mtyp_TESS) && (ift < Mtyp_BMP)) {

    if(impTyp != 0) {
      // load Mockup - add "M# = <fnam> <origin" to model-source
      UI_loadMock_CB (mnam, AP_mod_dir);

    } else {
      // printf(" load native |%s|\n",cbuf);
      if(ift == Mtyp_OBJ) {   // obj
        // irc = OS_dll_do ("xa_OBJ_R", "obj_read__", cbuf, 0);
        irc = OS_dll_do ("xa_obj_r", "obj_read__", cbuf, 0); // 2013-08-15
      } else {
        TX_Print("**** ERROR - import native %s not implemented yet",AP_mod_ftyp);
        irc = -1;
        goto L_exit;
      }
    }


  // 20-29 PixelImages
  } else if((ift >= Mtyp_BMP) && (ift < 30)) {  // JPG BMP
    UI_loadImg_CB (mnam, AP_mod_dir);


  } else {
    TX_Print("**** ERROR - import filetyp %s not implemented yet",AP_mod_ftyp);
  }


  if(irc < 0) goto L_exit;


  if(AP_src == AP_SRC_EDI)
    ED_load__ ();  // Mem -> Editor
  // // new loescht AP_mod_fnam !
  // strcpy(AP_mod_fnam, mnam);
  goto L_fertig;







  //================================================================
  // kein bekannter Modeltyp; try native ..
  L_native:
  ift = -1;

  // Datei AP_mod_fnam laden
  // ED_new_File (AP_mod_fnam); // Datei ins Memory einlesen
  Mod_load__ (mode, cbuf, dbResiz);


  //----------------------------------------------------------------
  L_fertig:

  TX_Print("load Model %s",cbuf);

  // test - report errors
  if(AP_stat.errStat) {
    TX_Print("***** %d errors *****",AP_stat.errStat);
  }

  // RUN (abarbeiten)
  ED_Reset ();    // ED_lnr_act resetten
  l2 = ED_work_END (0);

  if(AP_errStat_get() != 0) {  // Ablauffehler:
    i2 = UI_InpMode;
    if(i2 == UI_MODE_CAD) {
      // l1 = AP_lNr_get();
      l1 = AP_stat.errLn;      // 2011-08-12
      // modify zeile im mem mit dem SystemEditor.
      AP_src_mod_ed (l1);
    } else if(i2 == UI_MODE_MAN) {
      UI_Ed_sel_ln (l2); // select Line, set Curpos to Line.
    }
  }

  // DL_hili_off (-1L); // unhilite alle Objekte


  Mod_flst_all (0);       // create ../tmp/Mod.lst
  Brw_init__ ();       // fill browserWin


  // mode   1 = insert; do not write Title & do not rescale
  irc = 0;
  if(mode == 1) goto L_exit;


  // Title & Pfade oben auf den Mainwinrahmen
    // printf(" AP_mod_fnam=|%s|\n",AP_mod_fnam);
  UI_AP (UI_FuncSet, UID_Main_title, NULL);


  // Rescale (nur fuer Import - nicht fuer load native)
  if(ift != -1) {
    UI_but__ ("butRes");
  } else {
    // display scale
    UI_AP (UI_FuncSet, UID_ouf_scl, NULL);
  }


  if(AP_src == AP_SRC_EDI)
    ED_load__ ();  // Mem -> Editor



  //----------------------------------------------------------------
  L_exit:
    if(!irc) {
      // model successful loaded;
      // AP_mdl_modified_reset ();
      // reset UI
      UI_block__ (0, 0, 0);
#ifdef _MSC_VER
      UI_GR_expose ();  // MS-Windows only
#endif

      // adf filename to list "last-used"
      AP_Mod_lstAdd ();
    }

      // printf("ex-AP_Mod_load__ %d\n",irc);

    return irc;

}
*/

//================================================================
  int AP_delActMdl () {
//================================================================
// AP_delActMdl        delete active modelfile
// - move active modelfile -> tempDir/tmp.model

  char   fnOld[512], fnNew[256];

  // printf("AP_delActMdl \n");
  // printf(" |%s|%s|%s|\n",AP_mod_dir,AP_mod_fnam,AP_mod_ftyp);


  sprintf(fnOld,"%s%s.%s",AP_mod_dir,AP_mod_fnam,AP_mod_ftyp);
    // printf(" fnOld |%s|\n",fnOld);

  sprintf(fnNew, "%stmp.model",AP_get_tmp_dir());
    // printf(" fnNew |%s|\n",fnNew);

  OS_file_rename (fnOld, fnNew);

  TX_Print ("**** %s moved %s",fnOld,fnNew);

  return 0;

}


//================================================================
  int AP_Mod_lstAdd () {
//================================================================
// AP_Mod_lstAdd           add active model to list <tmpDir>MdlLst.txt

  char mfn[SIZMFTot];


  // do not add .svg - files
  if(AP_mod_iftyp == Mtyp_SVG) return 0;

  // get mfn = symbolic filename of active mainModel
  sprintf(mfn, "%s/%s.%s",AP_mod_sym,AP_mod_fnam,AP_mod_ftyp);
    // printf(" _lstAdd-mfn |%s|\n",mfn);


  return AP_Mod_lst_fn (mfn);

}
 

//================================================================
  int AP_Mod_lst_fn (char *mfn) {
//================================================================
// AP_Mod_lst_fn         add filename to list <tmpDir>MdlLst.txt

#define MAX_LNR 24

  char  lfn[SIZMFTot];


  // set lfn = filename of list
  sprintf(lfn, "%sMdlLst.txt", AP_get_tmp_dir());
    // printf("AP_Mod_lst_fn - lfn |%s|\n",lfn);

  // add 
  UTX_f_lifo_add (lfn, MAX_LNR, mfn);

  return 0;

}
 
//================================================================
  int AP_mod_sav_ok () {
//================================================================
// AP_mod_sav_ok         rename Mod_.mod_out -> Mod_.mod_in


  char     fn1[320], fn2[320];


  sprintf(fn1, "%sMod_.mod_out", AP_get_tmp_dir());
  sprintf(fn2, "%sMod_.mod_in", AP_get_tmp_dir());
  return OS_file_rename (fn1, fn2);

}


//====================================================================
  int AP_debug__ (char *module_info) {
//====================================================================
// DO NOT USE: use DEB_stop
//
// stop here in debug-mode; in core; with Alt-X
// in .gdbinit (gCAD3D.gdb); must start with: "gdb gCAD3D.exe -x gCAD3D.gdb"
// break AP_debug__
// Input:
//   module_info    text in which plugin/dll the stop occurs
// Purpose: eg set "watch <varnam>" after startup is done (saves time)


  // printf("*** debugStop in module %s\n",module_info);
  // fflush (stdout);

  return 0;

}


/*  UNUSED
//====================================================================
  int AP_search_def (char *cbuf) {
//====================================================================

  int    i1, llen;
  char   *lBuf;

  // printf("AP_search_def |%s|\n",cbuf);

  lBuf = memspc101;

  for(i1=1; i1<9999; ++i1) {

    UTF_GetLinNr (lBuf, &llen, i1);
    if(llen < 0) return -1;

    if(strstr(lBuf, cbuf) == NULL) continue;

    printf("ex AP_search_def %d\n",i1);
    return i1;

  }


  return -1;

}
*/


//====================================================================
  int AP_search_init () {
//====================================================================

  ED_lnr_act = 0;

  return 0;

}

/*
//====================================================================
  long AP_hili_obj (char *cbuf) {
//====================================================================
/// \code
/// hilite Obj from cbuf = DefLn od Header; eg "L25=..."
/// see also UI_hili_actLn IE_cad_Inp_disp
/// Input:
///   cbuf = Definitionline  (eg "M20=")
/// Output:
///   RetCode:  -2     object does not have a dispListobj (vector, variable, ..)
///             -1     Error; objID not found in DL
///             >= 0   the DL-index of the obj hilited 
/// \endcode

  int   i1;
  long  l2, dbi, dlInd;
  double d1;
  char   *p2;
  ObjGX  *oxp;


  printf("AP_hili_obj |%s|\n",cbuf);

  dlInd = -2;

  DL_hili_on (-1L);        // loeschen der Hili-Tabelle


  // check if DefLine od NC-Line
  i1 = APED_ck_defLn (&p2, cbuf);
  if(i1 == 0) goto L_def;

  // NC-Line ?
  return 0;





  //----------------------------------------------------------------
  L_def:
  // extract header aus Definitionline
  AP_cut_defHdr (&i1, &dbi, cbuf);
    // printf(" ex _defHdr: typ=%d ind=%d\n",i1,dbi);


  if(i1 == Typ_VC) {
    // UI_prev_vc (Typ_VC, &dbi);
    UI_prev_vc (GR_TMP_I0, Typ_Index, PTR_LONG(dbi), NULL, Typ_Att_hili1);
    goto L_done;


  } else if(i1 == Typ_VAR) {
    d1 = DB_GetVar (dbi);  // i2 ist normalerweise 0 !!!!
    TX_Print ("VAR[%d] = %f",dbi,d1);
    return 0;


  } else if(i1 == Typ_Tra) {
    oxp = DB_GetTra (dbi);
    if(oxp) UI_disp_tra (oxp);
    goto L_done;


  } else if(i1 == Typ_Activ) {
    // Activities sind unsichtbar ...
    return 0;
  }


  // find obj in DL
  dlInd = DL_dli__dbo (i1, dbi, -1L);
  if(dlInd < 0) {
    TX_Print("AP_hili_obj: Obj. %12.12s nicht gefunden",cbuf);
    return dlInd;
  } else {
    DL_hili_on (dlInd);      // hilite obj
  }

  
  L_done:
  DL_Redraw  ();
  

  return dlInd;

}
*/

/*  wird nicht mehr verwendet ...    see APED_search_defLn
//====================================================================
  int AP_search_obj (char *bufIn) {
//====================================================================
// Funnam in wc3: GRView_SucheDef
// suchen, positionieren, selektiert darstellen.
  

  static char oldNam[32];

  int         llen, startPos, endPos, ilen, i2, iloop, iscir;
  char        *l_start, *cp1, *nam1, nam2[32], *lBuf;
  

  printf("AP_search_obj |%s|\n",bufIn);

  lBuf = memspc101;

  
  // skip blanks
  nam1 = UTX_pos_1n (bufIn);

  //TX_Print("GRView_SucheDef /%s/",nam1);

  

  // vom Window ins Memory kopieren
  ED_unload__ ();
  // printf(" nach UI_AP 123674\n");





  //======================================================================


  // Fuer einen Kreis kommt immer C;
  // Wenn C99 gesucht wird, muss aber ebenso nach A99 gesucht werden !!
  if(strlen(nam1) > 30) {
    TX_Error("GRView_SucheDef E001");
    return -1;
  }



  // die Startzeile definieren; wenn vorher nach demselben Text gesucht wurde,
  // wird dort weitergesucht.
  if(strcmp(oldNam, nam1)) {
    //TX_Print(" neuer Name ..");
    AP_ED_lNr = 1;
  } else {
    ++AP_ED_lNr;
  }



  ilen = strlen (nam1);
  iloop = 0;



  //======================================================================
  //loop ueber alle Zeilen, suchen.
  NoAmoi:
  if(iloop > 3) goto Nixgefunden;


  l_start = UTF_GetLinNr (lBuf, &llen, AP_ED_lNr);
  // printf("AP_search_obj %d |%s| iloop=%d\n",AP_ED_lNr,lBuf,iloop);

  if(llen < 0) {      // nach EOF von vorn
    AP_ED_lNr = 1;
    ++iloop;
    goto NoAmoi;
  }


  // wenn Zeile mit "#" beginnt: weiter ..
  if(lBuf[0] == '#') goto L_weiter;
  if(lBuf[0] == '_') goto L_weiter;


  // die Zeile in Grossbuchstaben aendern
  //llen=strlen(lBuf);

  for (i2=0; i2<llen; i2++) {
    if (islower (lBuf[i2])) {
      lBuf[i2] = toupper (lBuf[i2]);
    }
  }
  // printf("AP_ED_lNr %d |%s|\n",AP_ED_lNr,lBuf);


  cp1 = UTX_find_word1 (nam1, lBuf);
  if(cp1 != NULL) goto FoundNext;


  // nix gefunden;
  // wenn bei nochmaliger Suche nix mehr da, no amoi von vorn.
  L_weiter:
  ++AP_ED_lNr;
  goto NoAmoi;



  //--------------------------------------------
  Nixgefunden:
  TX_Error("Definition %s nicht Gefunden",nam1);



  //--------------------------------------------
  Fertig:
  strcpy (oldNam,nam1);

  return 1;




  //--------------------------------------------
  FoundNext:

  TX_Print("%s in Zeile %d",nam1,AP_ED_lNr);

  // Den Startpunkt der Zeile im mem ist startPos; endpunkt = endPos
  startPos = UTF_offset_ (l_start);
  endPos = startPos + llen;

  // den text selektiert darstellen
  UI_Ed_sel (startPos, endPos);


  // die ZeilenNr korrigieren
  UI_AP (UI_FuncSet, UID_ouf_lNr, (void*)AP_ED_lNr);


  goto Fertig;

}
*/

//====================================================================
  int AP_vec_txt (Vector *vco, char *p1) {
//====================================================================
/// \code
/// give vector-struc from vector-text (only Standardvectors & planes)
/// see APED_oid_vc
/// \endcode

// see also APT_obj_expr APED_dbo_oid ATO_srcTxt

  int     ii;
  char    s1[4];
  Plane   pl1;
  // Mat_4x3 ma;

  // printf("AP_vec_txt |%s|\n",p1);

  UTX_cp_nchr_2_upper (s1, p1, 2);

    if(s1[0] == 'R') {
      ii = atoi(&p1[1]);
      DB_GetRef (&pl1, ii);
      *vco = pl1.vz;

    } else if (!strcmp (s1, "DX")) {
      *vco = UT3D_VECTOR_X;

    } else if (!strcmp (s1, "DY")) {
      *vco = UT3D_VECTOR_Y;

    } else if (!strcmp (s1, "DZ")) {
      *vco = UT3D_VECTOR_Z;

    } else if (!strcmp (s1, "DIX")) {
      *vco = UT3D_VECTOR_IX;

    } else if (!strcmp (s1, "DIY")) {
      *vco = UT3D_VECTOR_IY;

    } else if (!strcmp (s1, "DIZ")) {
      *vco = UT3D_VECTOR_IZ;

    } else if (!strncmp (s1, "D(", 2)) {
      return APT_obj_expr (vco, Typ_VC, p1);
      

    } else {
      // TX_Error("AP_vec_txt E001 %s",p1);
      return -1;
    }

  // DEB_dump_obj__(Typ_VC, vco, "ex AP_vec_txt");
  return 0;

}


/* unused
//================================================================
  int AP_get_nxtVal (int mode) {
//================================================================
// returns DB-index of next|previous Value.
// Input:
//   mode   1|-1
// RetCod: 0 = Error; no values defined ..

  static int   ii=0;
  int          i1;


  printf("AP_get_nxtVal ii=%d mode=%d\n",ii,mode);


  // skip if selection of Typ_VAR is not active
  if(sele_ck_typ(Typ_VAR) == 0) return -1;


  if(mode > 0) {                       // get prevoius
    L_nxt:
    i1 = DB_QueryNxtUsed (Typ_VAR, ii + 1);
       printf(" nxt: i1=%d ii=%d\n",i1,ii);

    // if(ii < 1)  {ii = 0; goto L_nxt;}      // LOOP if no vc exist
    if(i1 > 0)  ii = i1;


  } else {                                    // get next
    i1 = DB_dbo_get_last (Typ_VAR, ii - 1);
       printf(" Last: i1=%d ii=%d\n",i1,ii);
    // if(ii < 1)  ii = DB_QueryNxtUsed (Typ_VC, 0);
    if(i1 > 0)  ii = i1;
  }

  if(ii) {
    TX_Print("%d",ii);
  } else {
    TX_Print("no variables defined ..",ii);
  }

  printf("ex AP_get_nxtVal ii=%d mode=%d\n",ii,mode);


  return ii;

}
*/


//================================================================
  int AP_get_nxtVec (int mode) {
//================================================================
// returns DB-index of next|previous Vector.
// Input:
//   mode   1|-1
// RetCod: 0 = Error; no vectors defined ..

// startIndex == ii = static intern


  static long  ii=-1;
  int          i1;


  // printf("AP_get_nxtVec ii=%d mode=%d\n",ii,mode);

  if(ii < 0) ii = DB_dbo_get_last (Typ_VC);


  if(mode > 0) {                       // get next
    L_nxt:
    i1 = DB_QueryNxtUsed (Typ_VC, ii + 1);
    if(i1 > 0)  ii = i1;


  } else {                                    // get previous
    i1 = DB_QueryPrvUsed (Typ_VC, ii - 1);
    if(i1 > 0)  ii = i1;
  }

    // printf("ex AP_get_nxtVec ii=%d mode=%d\n",ii,mode);

  return ii;

}

/*
//====================================================================
  int AP_cmp_typ (int typ1, int typ2) {
//====================================================================
/// \code
/// check if types identical;
/// RC = 0 = ident;
/// 
/// see AP_typDB_typ
/// \endcode


  // printf("AP_cmp_typ %d %d\n",typ1,typ2);


  if(typ1 == typ2) return 0;

  if(typ1 == Typ_GTXT) {
    if((typ2 == Typ_Dimen) ||
       (typ2 == Typ_Tag)   ||
       (typ2 == Typ_ATXT))    return 0;


  } else if((typ1 == Typ_AC)||(typ1 == Typ_CI)) {
    if((typ2 == Typ_AC)||(typ2 == Typ_CI)) return 0;


  } else if(typ1 == Typ_CV) {
    if(typ2 == Typ_CVTRM) return 0;
    // case Typ_CVPOL:
    // case Typ_CVBSP:
    // case Typ_CVRBSP:
    // case Typ_CVELL:
    // case Typ_CVCLOT:
    // case Typ_CVTRM:



  }

  return -1;

}


//====================================================================
  int AP_typDB_typ (int typ) {
//====================================================================
/// \code
/// give basictyp from typ;
/// makes Typ_SURRU -> Typ_SUR   or Typ_CVBSP -> Typ_CV.
/// 
/// siehe APED_oid_dbo__ AP_cmp_typ
/// \endcode



  if((typ == Typ_CVPOL)   ||
     (typ == Typ_CVBSP)   ||
     (typ == Typ_CVRBSP)  ||
     (typ == Typ_CVELL)   ||
     (typ == Typ_CVCLOT)  ||
     (typ == Typ_CVTRM))
    return Typ_CV;                        // S


  if((typ == Typ_SURRU)   ||
     (typ == Typ_SURRV)   ||
     (typ == Typ_SURBSP)  ||
     (typ == Typ_SURPTAB) ||
     (typ == Typ_SURPMSH)  ||
     (typ == Typ_SURCIR))
    return Typ_SUR;                       // A


  if((typ == Typ_CON)   ||
     (typ == Typ_TOR))
    return Typ_SOL;                       // B


  if((typ == Typ_CI)     ||
     (typ == Typ_CI2)) 
    return Typ_AC;                        // C


  if(typ == Typ_LN2) return Typ_LN;


  if(typ == Typ_PT2) return Typ_PT;


  if((typ == Typ_Tag)    ||
     (typ == Typ_Dimen)  ||
     (typ == Typ_Dim3)   ||
     (typ == Typ_ATXT)   ||
     (typ == Typ_GTXT))
    return Typ_Note;                      // N


  if(typ == Typ_Mock)
    return Typ_Model;                     // M


  return typ;

}
*/
/*
//====================================================================
  int AP_typ_typChar (char typChar) {
//====================================================================
/// \code
/// AP_typ_typChar     make typ from typChar  ("P" -> Typ_PT)
/// Error: returns Typ_Error
/// \endcode

  int  ityp;


  // printf("AP_typ_typChar |%c|\n",typChar);


  switch(typChar) {

    case 'V':     ityp = Typ_VAR;       break;
    case 'D':     ityp = Typ_VC;        break;
    case 'P':     ityp = Typ_PT;        break;
    case 'L':     ityp = Typ_LN;        break;
    case 'C':     ityp = Typ_AC;        break;
    case 'A':     ityp = Typ_SUR;       break;
    case 'B':     ityp = Typ_SOL;       break;
    case 'R':     ityp = Typ_PLN;       break;
    case 'S':     ityp = Typ_CV;        break;
    case 'G':     ityp = Typ_G_Att;     break;
    case '$':     ityp = Typ_Txt;       break;
    case 'T':     ityp = Typ_Tra;       break;
    case 'M':     ityp = Typ_Model;     break;
    case 'N':     ityp = Typ_Note;      break; // Group=Typ_ATXT,Dimen
    case 'I':     ityp = Typ_Activ;     break; //
    case 'J':     ityp = Typ_Joint;     break; //
    case 'U':     ityp = Typ_goGeom;    break; // undefined
    default:      ityp = Typ_Error;   
  }

    // printf("ex AP_typ_typChar %d |%c|\n",ityp,typChar);

  return ityp;
}


//================================================================
  char AP_typChar_typ (int typ) {
//================================================================
/// \code
/// AP_typChar_typ      make typChar from typ  (Typ_PT -> 'P')
/// Error: returns '-'
/// \endcode


  if(       (typ == Typ_PT)  ||
            (typ == Typ_PT2))        {
    return 'P';


  } else if((typ == Typ_LN)  ||
            (typ == Typ_LN2))        {
    return 'L';


  } else if((typ == Typ_AC)  ||
            (typ == Typ_CI2) ||
            (typ == Typ_CI)  ||
            (typ == Typ_CI2))        {
    return 'C';


  } else if(typ == Typ_PLN) {
    return 'R';


  } else if(typ == Typ_VC) {
    return 'D';


  } else if((typ == Typ_CV)      ||
            (typ == Typ_CVPOL)   ||
            (typ == Typ_CVBSP)   ||
            (typ == Typ_CVRBSP)  ||
            (typ == Typ_CVELL)   ||
            (typ == Typ_CVTRM))      {
    return 'S';


  } else if((typ == Typ_SUR)      ||
            (typ == Typ_SURRU)    ||
            (typ == Typ_SURRV)    ||
            (typ == Typ_SURCIR)   ||
            (typ == Typ_SURSTRIP) ||
            (typ == Typ_SURBSP)   ||
            (typ == Typ_SURRBSP))    {
    return 'A';



  } else if((typ == Typ_SOL)   ||
            (typ == Typ_CON)   ||
            (typ == Typ_TOR))        {
    return 'B';


  } else if((typ == Typ_ATXT)  ||
            (typ == Typ_GTXT)  ||
            (typ == Typ_Dimen) ||
            (typ == Typ_Tag))        {
    return 'N';


  } else if((typ == Typ_Model) ||
            (typ == Typ_Mock))       {
    return 'M';


  // } else if((typ == Typ_VAR)   ||  // hat hoechste Select-Prioritaet in GL
            // (typ == Typ_Val))        {
  } else if(typ == Typ_VAR) {  // Variable
    return 'V';


  } else if(typ == Typ_Tra) {  // Transformation
    return 'T';


  } else if(typ == Typ_Activ) {  // Activity
    return 'I';


  } else if(typ == Typ_goGeom) {  // undefined
    return 'U';


  }

  // printf("APED_oid_dbo__ |%s| %d\n",buf,strlen(buf)); // nur Testausg.

  return '-';

}
*/
 
//=====================================================================
  int AP_test__ () {
//=====================================================================
// starten mit "Ctl shift T"
// oder dem Startparameter AP_test__

  int          irc;
  char         s1[256], filNam[SIZFNam];
  Point        p1;
  UtxTab_NEW   (ttb);

  printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA AP_test__\n");
  TX_Print("AP_test__");

  // PRC__ (-1, "EXIT__");
  // PRC_exit ();
  // MDL_dump__ ("AP_test__");
  // AP_GUI_test1 ();

return -1;




  // MDL_test__ ();
  // MDL_load_mNam__ ("ELE1/board1.gcad");
  //----------------------------------------------------------------
  // MDL_sav_gcad (0GUI_file_smuu ("file1\nfile2\n", 2);
  // GUI_file_over ("/mnt/serv2/devel/cadfiles/gcad/t5.gcad", 0);
  // AP_save__ (2, 0, 1, "gcad");
  // MDL_sav__ ("/mnt/serv2/devel/cadfiles/gcad/t5.gcad", 0, 0);
  // MDL_rem_smuu__ ();
  // MDL_smuu_lst (&ttb);
  // UtxTab_free (&ttb);
  // AP_save_del_smuu ();


  // PRCV_DB_dump ("AP_test__");
  // OS_dll_do ("xa_edmpt", NULL, NULL, 2);
  // OS_dll_unload_idle ("xa_edmpt");

  // ED_test__();

  // TSU_exp__ ("TESS", "/mnt/win_d/dev/gCAD3D/dat/.0.tess"); return 0;
  // TSU_exp__ ("WRL", "/mnt/win_d/dev/gCAD3D/dat/.0.wrl"); return 0;


  //----------------------------------------------
  // HiddenLine

  // GL_DefineDisp (FUNC_DispHid1, 0);   //
  // DL_Redraw ();
  // GL_DefineDisp (FUNC_DispHid2, 0);   //
  // DL_Redraw ();
  // GL_DefineDisp (FUNC_DispHid3, 0);   //
  // DL_Redraw ();


  // TSU_test_sbsp ();   // test BSP-Surface tesselation
  // TSU_test_sru ();   // test Surface tesselation
  // TSU_test_srv ();   // test Surface tesselation
  // DL_Redraw ();


  // UI_menCB (NULL, "new");   // Gesamtes File loeschen

  //----------------------------------------------
  // IMPORT IGES-FILE
  // AP_ImportIg1 ("0,0,0", 1, "dat/0.igs");
  // AP_ImportIg1 ("0,0,0", 1, "igs/f408_2.igs");

  // IMPORT as DXF-FILE
  // AP_ImportDxf (0, "0,0,0", "dat/0.dxf");
  // AP_ImportDxf (0, "0,0,0", "dxf/dim3.dxf");

  // ED_load__ ();  // Mem -> Editor
  // ED_work_END (0);  // ABARBEITEN


  //----------------------------------------------
  // EXPORT as IGES-FILE
  // AP_ExportIges__ ("/mnt/win_d/dev/gCAD3D/dat/0.igs");   // LINUX
  // AP_ExportIges__ ("/dev/dat/0.igs");   // MS-WIN

  // EXPORT as DXF-FILE
  // AP_ExportDxf ("/mnt/win_d/dev/gCAD3D/dat/0.dxf");   // LINUX

  // EXPORT as NC-ISO
  // ED_work_PP ();

  //----------------------------------------------
  // PLOT
  // AP_print__ ();
  //         irot  off    scl  gray
  // AP_print_psv2 (1, "0,0", "1", "2");


  //----------------------------------------------
  // DB_dump_ModBas ();       // Basemodels
  // DB_dump__ ();            // dump DB
  // DL_DumpObjTab ();        // Dump DL
  // APT_dump_itab ();        // dump EditorLine->DL-Hilfsliste
  // GA_dump__ ();            // Dump HideList


  return 0;

}


//================================================================
  int AP_Get_scale (double *scl) {
//================================================================
// AP_Get_scale        AP_scale

  // printf(" AP_Get_scale %f\n",AP_scale);

  *scl = AP_scale;

  return 0;
}


//================================================================
  int AP_Set_scale (double scl) {
//================================================================
// AP_Set_scale        AP_scale

// scl * 2 = vertical size of screen in usercoords
// scl * GL_Svfakt * 2 = horiz size of screen in usercoords 

// see also GL_GetViewSizU

  // printf("AP_Set_scale %lf\n",scl);

  AP_scale = scl;

  return 0;
}


//================================================================
  int AP_impTyp_get () {
//================================================================
// AP_impTyp_get       get impTyp UID_ckb_impTyp (CAD-Model or mockup)
// returns   1=Mockup, 0=native
 
  int     impTyp;

  UI_AP (UI_FuncGet, UID_ckb_impTyp, (void*)&impTyp); // 1=Mockup, 0=native

    printf(" ex-AP_impTyp_get %d\n",impTyp);


  return impTyp;

}


//================================================================
  int AP_Get_Setup (char *cbufo, char *ctyps) {
//================================================================
/// get line from gCAD3D.rc

 
  char      cbuf3[128];

  sprintf(cbuf3,"%sgCAD3D.rc",AP_get_cfg_dir());

  // printf("AP_Get_Setup %s |%s|\n",cbuf3,ctyps);


  return UTX_setup_get (cbufo, ctyps, cbuf3);

}



//=====================================================================
  int AP_Set_ConstPl_Z (char *cbuf) {
//=====================================================================
// AP_Set_ConstPl_Z             disp WC_sur_txt (eg "RZ") above graf.Win

  // printf("AP_Set_ConstPl_Z |%s|\n",cbuf);

  strcpy (WC_sur_txt, cbuf);

  // display 
  UI_Set_ConstPl_Z ();

  return 0;
}

//=====================================================================
  int AP_Get_ConstPl_vz (Vector *vc) {
//=====================================================================

  *vc = WC_sur_act.vz;

  return 0;

}


//=====================================================================
  char* AP_Get_ConstPl_Z (int typ) {
//=====================================================================
/// \code
/// typ = 0       returns the active ConstrPln as Text; zB "RZ" or "R22"
/// typ = Typ_VC  returns the Z-vec of the active ConstrPln as Text
/// \endcode

// Die ConstrPlane == die struct zu WC_sur_txt ist WC_sur_act !


static char cbuf[32];


  // printf("AP_Get_ConstPl_Z %d |%s|\n",typ,WC_sur_txt);


  if(typ == Typ_VC) {

    if     (!strcmp (WC_sur_txt, "RX")) strcpy (cbuf, "DX");
    else if(!strcmp (WC_sur_txt, "RY")) strcpy (cbuf, "DY");
    else if(!strcmp (WC_sur_txt, "RZ")) strcpy (cbuf, "DZ");
    else {   // Z-Vektor der WC_sur_act
      sprintf(cbuf, "D(%s)",WC_sur_txt);
    }

    // printf("ex AP_Get_ConstPl_Z |%s|\n",cbuf);
    return cbuf;
  }

  // printf("ex AP_Get_ConstPl_Z |%s|\n",WC_sur_txt);
  return WC_sur_txt;

}


//==========================================================================
  int AP_PT2EyeBp (char *plnTyp,Point *p1,Point *p2,Point *p3,Vector *vcs) {
//==========================================================================

  int     bp;
  Vector  vcx, vcz;


  //----- Variante DimPlane X:-----------------------------------
  if(plnTyp[0] != 'X') goto L_Y;
  // auf X-Y oder X-Z-Ebene ?
  // Ist der Z-part oder der Y-part von vcs groesser -
  // Hoehenwert der Backplane bestimmen: den niedrigeren Wert von p1/p2 nehmen
  if(fabs(vcs->dz) > fabs(vcs->dy)) {
    bp = 2; // 2 = BCKPLN_XY

  } else {
    bp = 1; // 1 = BCKPLN_XZ

  }
  goto L_draw1;



  //----- Variante DimPlane Y:-----------------------------------
  L_Y:
  if(plnTyp[0] != 'Y') goto L_Z;
  // auf X-Y oder Y-Z-Ebene ?
  // Ist der Z-part oder der X-part von vcs groesser -
  // Hoehenwert der Backplane bestimmen: den niedrigeren Wert von p1/p2 nehmen
  if(fabs(vcs->dz) > fabs(vcs->dx)) {
    bp = 2; // 2 = BCKPLN_XY

  } else {
    bp = 0; // 0 = BCKPLN_YZ

  }
  goto L_draw1;



  //----- Variante DimPlane Z:-----------------------------------
  L_Z:
  if(plnTyp[0] != 'Z') goto L_parl;
  // auf X-Z oder Y-Z-Ebene ?
  // Ist der X-part oder der Y-part von vcs groesser -
  // Hoehenwert der Backplane bestimmen: den niedrigeren Wert von p1/p2 nehmen
  if(fabs(vcs->dy) > fabs(vcs->dx)) {    // draw to XZ
    bp = 1; // 1 = BCKPLN_XZ

  } else {                            // draw to YZ
    bp = 0; // 0 = BCKPLN_YZ

  }
  goto L_draw1;




  //----- Variante keine DimPlane:-----------------------------------
  L_parl:
/*
  // testen ob Schrift auf der rechten Seite (lesbar) ...
  if(p2->y > p1->y) {
    bp = 3; // 3 = parallel
  } else {
    bp = 4; // 4 = parallel / invers
  }
*/

  UT3D_vc_2pt (&vcx, p1, p2);
  UT3D_vc_perpvc2pt (&vcz, &vcx, p3, p1);
  if(UT3D_slen_projvcvc (vcs, &vcz) < 0.) bp = 4;
  else bp = 3;
  // goto L_draw1;




  L_draw1:

    // DEB_dump_obj__ (Typ_PT, pt, "ex AP_PT2EyeBp %d:",bp);

  return bp;

}


//==========================================================================
  int AP_PT2EyePln (Point *pt,
                    char *plnTyp,Point *p1,Point *p2,Point *p3,Vector *vcs) {
//==========================================================================
// Doku see APT_decode_dim3
// Input:
//   plnTyp    X od Y od Z or blank ( Parallelplane)
// Output:
//   RetCode:  backplane 
//   pt        midpoint; see APT_decode_dim3

  
  int     irc;
  double  d1;
  Point   pm, px;
  Vector  vc1;
  Plane   pl1;

  // Point  p1x, p2x;    // nur fuer Tests ..


  // printf("AP_PT2EyePln |%s|\n",plnTyp);
    // GR_tDyn_symB__ (p1, SYM_STAR_S, 2);
    // GR_tDyn_symB__ (p2, SYM_STAR_S, 5);
    // GR_tDyn_symB__ (p3, SYM_STAR_S, 5);
    // GR_tDyn_vc__ (vcs, p3, 1, 0);


  // Mittelpunkt zwischen MP1/MP2
  UT3D_pt_mid2pt (&pm, p1, p2);

  // durch pm eine Ebene normal auf vcs;
  // gesucht:  Durchstoszpunkt von p3+vcs
  if(plnTyp[0] == 'X') {  // 
    irc = UT3D_pt_intptvcplx (&px, p3, vcs, pm.x);
    if(irc < 0) {
      // y u z vom p3, x vom pm
      px.x = pm.x;
      px.y = p3->y;
      px.z = p3->z;
    }

  } else if(plnTyp[0] == 'Y') {
    irc = UT3D_pt_intptvcply (&px, p3, vcs, pm.y);
    if(irc < 0) {
      // x u z vom p3, y vom pm
      px.x = p3->x;
      px.y = pm.y;
      px.z = p3->z;
    }

  } else if(plnTyp[0] == 'Z') {
    irc = UT3D_pt_intptvcplz (&px, p3, vcs, pm.z);
    if(irc < 0) {
      // x u y vom p3, z vom pm
      px.x = p3->x;
      px.y = p3->y;
      px.z = pm.z;
    }

  } else {    // parallel
    UT3D_vc_2pt (&vc1, p1, p2);
    UT3D_pl_ptvc (&pl1, &pm, &vc1);
    irc = UT3D_pt_intptvcpl_ (&px, &pl1, p3, vcs);
    if(irc < 1) {
      UT3D_pt_projpt2pt (&px, &d1, p3, p1, p2);
      // px-p3 von pm weg ergibt neuen p3
      UT3D_vc_2pt (&vc1, &px, p3);
      UT3D_pt_traptvc (&px, &pm, &vc1);
    }
  }


  *p3 = px;

  L_keep_p3:

/*
  //-------------------------------
  // NUR TEST:
  // durch p1 eine Ebene normal auf vcs;
  // gesucht:  Durchstoszpunkt von p3+vcs
  if(plnTyp[0] == 'X') {
    UT3D_pt_intptvcplx (&p1x, p3, &UT3D_VECTOR_X, p1->x);
    UT3D_pt_intptvcplx (&p2x, p3, &UT3D_VECTOR_X, p2->x);

  } else if(plnTyp[0] == 'Y') {
    UT3D_pt_intptvcply (&p1x, p3, &UT3D_VECTOR_Y, p1->y);
    UT3D_pt_intptvcply (&p2x, p3, &UT3D_VECTOR_Y, p2->y);

  } else if(plnTyp[0] == 'Z') {
    UT3D_pt_intptvcplz (&p1x, p3, &UT3D_VECTOR_Z, p1->z);
    UT3D_pt_intptvcplz (&p2x, p3, &UT3D_VECTOR_Z, p2->z);

  } else {    // parallel
    UT3D_vc_2pt (&vc1, p1, p2);
    UT3D_pl_ptvc (&pl1, p1, &vc1);
    UT3D_pt_intptvcpl_ (&p1x, &pl1, p3, &vc1);
    UT3D_pl_ptpl (&pl1, p2);
    UT3D_pt_intptvcpl_ (&p2x, &pl1, p3, &vc1);
  }

    GR_tDyn_symB__ (&p1x, SYM_STAR_S, 7);
    GR_tDyn_symB__ (&p2x, SYM_STAR_S, 7);
*/





  //-------------------------------
    // GR_tDyn_symB__ (p3, SYM_STAR_S, 7);
    // DL_Redraw ();



  return AP_PT2EyeBp (plnTyp, p1, p2, p3, vcs);

}


/*
//==========================================================================
  int AP_PT2EyePln (Point *pt,char *plnTyp,Point *p1,Point *p2,Vector *vcs) {
//==========================================================================
// bringt p3 int eine Plane mit p1 und p2 und Normalvektor vcs.
// p1 und p2 liegen in der Plane mit dem Typ plnTyp.
// Input:
//   plnTyp    X od Y od Z od blank (eine Parallelplane)


  int     bp;
  Point   pth;
  Vector  vcx, vcy, vcz;


  printf("AP_PT2EyePln  |%c|\n",plnTyp[0]);
    DEB_dump_obj__ (Typ_PT, p1, "  p1:");
    DEB_dump_obj__ (Typ_PT, p2, "  p2:");
    DEB_dump_obj__ (Typ_PT, pt, "  pt:");
    DEB_dump_obj__ (Typ_VC, vcs, "  vc:");


  // change first char of plnTyp to uppercase
  plnTyp[0] = toupper (plnTyp[0]);


  //----- Variante DimPlane X:-----------------------------------
  if(plnTyp[0] != 'X') goto L_Y;
  // auf X-Y oder X-Z-Ebene ?
  // Ist der Z-part oder der Y-part von vcs groesser -
  // Hoehenwert der Backplane bestimmen: den niedrigeren Wert von p1/p2 nehmen
  if(fabs(vcs->dz) > fabs(vcs->dy)) {
    bp = 2; // 2 = BCKPLN_XY
    if(p2->y > p1->y) pth = *p2;
    else pth = *p1;
    // vcx = UT3D_VECTOR_X;
    // vcy = UT3D_VECTOR_Y;
    vcz = UT3D_VECTOR_Z;
    // dl = fabs(p1->x - p2->x);
    // az1 = 90.;
  } else {
    bp = 1; // 1 = BCKPLN_XZ
    if(p2->y > p1->y) pth = *p2;
    else pth = *p1;
    // vcx = UT3D_VECTOR_X;
    // vcy = UT3D_VECTOR_Z;
    vcz = UT3D_VECTOR_IY;
    // dl = fabs(p1->x - p2->x);
    // az1 = 90.;
  }
  goto L_draw1;



  //----- Variante DimPlane Y:-----------------------------------
  L_Y:
  if(plnTyp[0] != 'Y') goto L_Z;
  // auf X-Y oder Y-Z-Ebene ?
  // Ist der Z-part oder der X-part von vcs groesser -
  // Hoehenwert der Backplane bestimmen: den niedrigeren Wert von p1/p2 nehmen
  if(fabs(vcs->dz) > fabs(vcs->dx)) {
    bp = 2; // 2 = BCKPLN_XY
    if(p2->y > p1->y) pth = *p2;
    else pth = *p1;
    // vcx = UT3D_VECTOR_Y;
    // vcy = UT3D_VECTOR_IX;
    vcz = UT3D_VECTOR_Z;
    // dl = fabs(p1->y - p2->y);
    // az1 = 90.;
  } else {
    bp = 0; // 0 = BCKPLN_YZ
    if(p2->y > p1->y) pth = *p2;
    else pth = *p1;
    // vcx = UT3D_VECTOR_Y;   // fix
    // vcy = UT3D_VECTOR_Z;
    vcz = UT3D_VECTOR_X;
    // dl = fabs(p1->y - p2->y);
    // az1 = 90.;
  }
  goto L_draw1;



  //----- Variante DimPlane Z:-----------------------------------
  L_Z:
  if(plnTyp[0] != 'Z') goto L_parl;
  // auf X-Z oder Y-Z-Ebene ?
  // Ist der X-part oder der Y-part von vcs groesser -
  // Hoehenwert der Backplane bestimmen: den niedrigeren Wert von p1/p2 nehmen
  if(fabs(vcs->dy) > fabs(vcs->dx)) {    // draw to XZ
    bp = 1; // 1 = BCKPLN_XZ
    if(p2->z > p1->z) pth = *p2;
    else pth = *p1;
    // vcx = UT3D_VECTOR_Z;
    // vcy = UT3D_VECTOR_IX;
    vcz = UT3D_VECTOR_IY;
    // dl = fabs(p1->z - p2->z);
    // az1 = 90.;
  } else {                            // draw to YZ
    bp = 0; // 0 = BCKPLN_YZ
    if(p2->z > p1->z) pth = *p2;
    else pth = *p1;
    // vcx = UT3D_VECTOR_Z;   // fix
    // vcy = UT3D_VECTOR_IY;
    vcz = UT3D_VECTOR_X;
    // dl = fabs(p1->z - p2->z);
    // az1 = 90.;
  }
  goto L_draw1;




  //----- Variante keine DimPlane:-----------------------------------
  L_parl:
  // plnTyp[0] = 'P';
  // vcx = p1 - p2
  UT3D_vc_2pt (&vcx, p1, p2);


  // testen ob Schrift auf der rechten Seite (lesbar) ...
  if(p2->y > p1->y) {
    bp = 3; // 3 = parallel

  } else {
    bp = 4; // 4 = parallel / invers
    UT3D_vc_invert (&vcx, &vcx);
    // plnTyp[0] = '-';  // p1/p2 verkehrt
  }


  // 2.Vec in der DimPlane
  UT3D_vc_perp2vc (&vcy, &vcx, vcs);
  // daraus den Z-Vec der DimPlane
  UT3D_vc_perp2vc (&vcz, &vcy, &vcx);
    // GR_tDyn_vc__ (&vcz, p1, 9, 0);

  pth = *p1;
  // dl = UT3D_len_vc (&vcx);

  goto L_draw1;


  //----------------------------------------------------------------
  L_draw1:

  // pth ist der entferntere Punkt von p1/p2
  // vcz ist nun ein Normalvektor auf eine Ebene durch p1 und p2
  // pth = p3 in die Ebene pth-vcz bringen
  UT3D_pt_intptvcpln (&pth,   pt, vcs,     &pth, &vcz);
    // GR_tDyn_symB__ (&pth, SYM_TRI_S, 2);



  // der Textpunkt ist zu korrigieren !!!
  *pt = pth;



  //----------------------------------------------------------------

    DEB_dump_obj__ (Typ_PT, pt, "ex AP_PT2EyePln %d:",bp);

  return bp;

}
*/


/*
//=====================================================================
  int AP_GetObj    (ObjG* o1, long dli) {
//=====================================================================
// ein transformiertes Objekt liefern
// irc 1 = OK; -1=Fehler


  int     irc, tra_ind, apt_typ;
  long    apt_ind;


    apt_typ = DL_dbTyp__dli (dli);
    apt_ind = DL_get_dbi (dli);
    tra_ind = DL_GetTrInd (dli);

    irc = DB_Get_GR_Obj (o1, apt_typ, apt_ind, tra_ind);

  return irc;

}
*/


//================================================================
  // int AP_DLL__ () {
//================================================================

  
 


//================================================================
  int ACT_ck_act (long dli) {
//================================================================
/// \code
/// check if selected obj is connected to interactivity and activate activity.
/// RecCod:
///   1=obj has activity, but interactivity is not active
///   0=Yes, interactivity performed;
///  -1=No, normal object.
/// \endcode

  int      irc, i1, typ;
  long     ind, dbi;
  Activity *act1;
  char     *p1;


  // printf("ACT_ck_act %ld\n",dli);

  if(dli < 0) return -1;

  typ = DL_dbTyp__dli (dli);
  typ = AP_typDB_typ (typ);
  ind = DL_get_dbi (dli);
  if(ind <= 0) return -1;


  // printf("ACT_ck_act %ld %d %ld\n",dli,typ,ind);


  // query DB, if a interactivity is connected to this obj
  irc = DB_QueryActiv (&act1, typ, ind);
  if(irc < 0) return irc;


  // check if interactivity is active; return if not.
  i1 = AP_stat.iActStat;
  if(!i1) return 1;
  // UI_AP (UI_FuncGet, UID_ckb_Iact, (void*)&i1);  // 0=yes
    // // printf(" ckb_Iact=%d\n",i1);
  // if(i1) return 1;



  //----------------------------------------------------------------
  // yes; Activity present; execute it ..

  // UI_GR_block (0);
  L_resolv:
  p1 = act1->data;

  // decode "I23"
  if((*p1 == 'I')&&(isdigit(*(p1+1)))) {
    APED_dbo_oid (&typ, &dbi, p1);
    act1 = DB_get_Activ (dbi);
    goto L_resolv;
  }


  // printf("Activity |%s| selected\n",act1->data);
  TX_Print("Activity |%s| selected\n",act1->data);

  // execute activity ....
  irc = WC_Work1 (0, p1);

  // unselect obj (dli changes !
  DL_hili_off (-1L); DL_Redraw ();

  // UI_GR_block (1);

  return 0;

}


//================================================================
  int AP_sel_oid__ () {
//================================================================
// AP_sel_oid__                       get objID(s) from text, hilite, add to grp 


  int      irc, typ, ii;
  long     dbi, dli, ll;
  char     s1[256], *p1;


  // get s1 = objID(s)
  s1[0] ='\0'; 
  irc = GUI_dlg_e2b (s1, 256, "objID(s) ", "Cancel", "OK");
  if(irc < 1) return -1;

    // TESTBLOCK:
    // strcpy(s1, "S20 S21, S22 S(XX) P21");
    printf(" |%s|\n",s1);
    // END TESTBLOCK:

  ll = strlen(s1);
  p1 = s1;
  ii = 0;

  //----------------------------------------------------------------
  // get next objID
  L_nxt:
  UTX_pos_skipLeadBlk (p1);

  // test if its oid 
  irc = APED_oid_src1 (&typ, &dbi, &p1, &ll);
  if(irc < 0) goto L_exit;
    printf(" irc=%d typ=%d dbi=%ld ll=%ld\n",irc,typ,dbi,ll);


  // hilite obj
  dli = DL_dli__dbo (typ, dbi, -1L);
  DL_hili_on (dli);        // hilite obj

  // add obj to group
  DL_grp1__ (dli, NULL, 1, 1);

  ++ii;
  goto L_nxt;


  //----------------------------------------------------------------
  L_exit:
  Grp_upd (1);  // update GrpNr-label

  return 0;

}




/* ====================== EOF =========================== */
