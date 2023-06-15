// include with deactivated extern in "../xa/xa.c"
// some variables must be initilized in 
// needs #include "../ut/ut_geo.h"    (Point)


#include "../xa/ap_stat.h"                 // AP_STAT ap_stat - application-status
#include "../xa/mdl__.h"                   // SIZMFNam  


#define  WinSizMinX 800                   // minimum X-size of grafic window
#define  WinSizMinY 240                   // minimum Y-size of grafic window
#define  WinSizMin__ "-800,-240"


// applicationObjectNames - displayed in selectionList - used by plugins.
// see PLU_appNamTab_set
typedef struct {char* oNam; int oTyp;}                         APP_OBJ_NAM;



//================================================================
// included and defined in ../xa/xa.c (with extern invalidated)
extern char AP_mod_dir[SIZMFTot];  // abs.directory of active model, with '/' at end
extern char AP_mod_fnam[SIZMFNam]; // active mainModelname without filetyp
extern char AP_mod_ftyp[SIZMFTyp]; // filetyp of active mainModel
extern char AP_mod_sym[SIZMFSym];  // symbolic directory for OPEN (no '/' at end)
  // AP_mod_iftyp: integer-filetyp of active model; eg Mtyp_DXF
extern char AP_mod_iftyp;          // integer-filetyp - eg Mtyp_Gcad|Mtyp_DXF ..

extern char AP_modact_nam[SIZMFNam]; // name of the active model (symbolic)
                                   // empty if mainmodel is active;
extern int  AP_modact_ibm;         // indicates if model is active or being created
//  -1: the active-model is the primary-Model. The primaryModel can be subModel.
// 0-n: a SubModel is being created (its the baseModelNr). See INF_basModels__
extern int  AP_modact_inm;         // mdl_names-index of primary model; 0=mainModel
extern int  AP_modact_tmp;         // 1=temp-model-exists; 0=not


extern Point AP_box_pm1, AP_box_pm2;  // box around mainmodel

// extern char AP_dir_save[128];     // directory for SAVE
// extern char AP_sym_save[64];      // symbolic directory for SAVE

extern char AP_dir_prg[SIZMFTot]; // directory for programs
extern char AP_sym_prg[64];       // symbolic directory for programs
extern char AP_symDir_fnam[128];  // filename active SymbolDirFile

extern char AP_errText[128];      // errortext for minor errors
extern char AP_printer[80];       // Printer
extern char AP_editor[80];        // TextEditor
extern char AP_lang[4];           // en or de
extern char AP_winSiz[64];        // size of application-window "-1000,-690"

extern int  APP_act_typ;          // type of APP_act_nam: 0=none;
                                  // 1=applic. 2=process 3=plugin 4=remote;
                                  // APP       PRC       PLU      RPC
extern char APP_act_nam[128];     // name of last script- or plugin-program
extern char APP_act_proc[128];    // name of processor (dll)
extern long APP_dli_start;        // first free dli (last dli of core-obj's)
extern double AP_mod_defSiz;      // DefaultModelsize

extern char*  AP_fVwr;            // fileViewer - must set to NULL
extern FILE*  AP_deb_fp;          // debug-file-pointer

extern AP_STAT AP_stat;


// ../xa/mdlmp.c
extern int   AP_modact_inm;           // mdl_names-index of primary model



//================================================================
  int AP_defaults_write ();
 long AP_cre_defHdr (int typ, int istart);
  int AP_Init0 ();
  int AP_Init1 ();
  int AP_search_init ();
  // int AP_search_obj (char *bufIn);
  // int AP_search_def (char *cbuf);
  int AP_Print0 (int mode,char* cmd,int irot,char* off,char* scl);
  int AP_PrintWrHex (FILE *fpo,unsigned char *dat,int ilen,int gray);
  int AP_defaults_read ();

  char AP_typChar_typ (int typ);
  int APED_oid_dbo__ (char *buf, int typ, long ind);

  // int AP_appr (char *obj, char *ref, double tol, double lmin, int mode);

  int AP_obj_del0    ();
  // int AP_obj_del1    ();
  // int AP_obj_del2    (int lNr, char *tx1, int mode);
  // int AP_obj_analyze (int typ, long ind);

  int AP_get_lNr  ();
  int AP_set_lNr  (int lNr);

  // long AP_lNr_get ();
  // int  AP_lNr_set (long lNr);

  // int APcol_defCol__ (ColRGB *cSel);  // ColRGB!

  char* AP_Get_ConstPl_Z (int typ);
  // int AP_GetObj    (ObjG* o1, long dli);
  int AP_APT_clean ();

  int AP_errStat_set (int stat);
  int AP_errStat_get ();



// //================================================================
// /// debug-print (see DEB_prt_init())
// #ifdef DEB
// #define printd if(DEB_prt_stat) AP_deb_print
// #else
// #define printd if(0) printf
// #endif


// // test if 2D is ON (if ConstructionPlane is active)
// //   if yes: SRC-coordinates are relativ;
// #define AP_IS_2D (WC_sur_ind != 0)

// test if constrPln is active (if 2D is on);    1=yes, 0=no
// DB_PLIZ_IND = -6
// #define CONSTRPLN_IS_ON ((WC_sur_ind > 0)||(WC_sur_ind < -6))
#define CONSTRPLN_IS_ON (WC_sur_ind)


// // test if 3D is ON (if no user-defined ConstructionPlane is active)
// #define AP_IS_3D (WC_sur_ind == 0)

// test if constrPln is active (if 2D is on);    1=yes, 0=no
// DB_PLIZ_IND = -6
// #define CONSTRPLN_IS_OFF ((WC_sur_ind >= -6)&&(WC_sur_ind <= 0))
#define CONSTRPLN_IS_OFF (!WC_sur_ind)



// MDL_IS_MAIN      check if the active model is the mainmodel
// retCode      0   subModel is active
//              1   the main-model is active
#define MDL_IS_MAIN (!AP_modact_inm)


// test if active model is a subModel being created or the active-model
//   active-model can be a subModel; see MDL_IS_MAIN
// retCode      0   the primary-model is active  (AP_modact_ibm = -1)
//              1   a subModel is being created  (AP_modact_ibm >= 0)
#define MDL_IS_SUB (AP_modact_ibm != MDL_BMI_ACT)

// test if active model is the active-model or a subModel being created
//   active-model can be a subModel; see MDL_IS_MAIN
// retCode      0   a subModel is being created  (AP_modact_ibm >= 0)
//              1   the primary-model is active  (AP_modact_ibm = -1)
#define MDL_IS_PRIM (AP_modact_ibm == MDL_BMI_ACT)



// test if browser is active;  1=yes, 0=no
#define BRW_STAT (AP_stat.brw_stat == BRWSTAT_active)



//----------------------------------------------------------------
// UTRA_UCS_WCS_VC                      transfer vector from WCS into UCS
// input is absolute; if constrPlane is active, transfer input into UCS
void UTRA_UCS_WCS_VC (Vector*, Vector*);
#define UTRA_UCS_WCS_VC(vco, vci) {\
  if(CONSTRPLN_IS_ON) UT3D_vc_tra_vc_m3 (vco, WC_sur_imat, vci);\
  else if(vco != vci) *vco = *vci;}


// UTRA_UCS_WCS_PT                      transfer point from WCS into UCS
// input is absolute; if constrPlane is active, transfer input into UCS
void UTRA_UCS_WCS_PT (Point*, Point*);
#define UTRA_UCS_WCS_PT(pto, pti) {\
  if(CONSTRPLN_IS_ON) UT3D_pt_tra_pt_m3 (pto, WC_sur_imat, pti);\
  else if(pto != pti) *pto = *pti;}


//----------------------------------------------------------------
// UTRA_WCS_UCS_VC                      transfer vector from UCS into WCS
// input is absolute; if constrPlane is active, transfer input into WCS
void UTRA_WCS_UCS_VC (Vector*, Vector*);
#define UTRA_WCS_UCS_VC(vco, vci) {\
  if(CONSTRPLN_IS_ON) UT3D_vc_tra_vc_m3 (vco, WC_sur_mat, vci);\
  else if(vco != vci) *vco = *vci;}


// UTRA_WCS_UCS_PT                      transfer point from UCS into WCS
// input is absolute; if constrPlane is active, transfer input into WCS
void UTRA_WCS_UCS_PT (Point*, Point*);
#define UTRA_WCS_UCS_PT(pto, pti) {\
  if(CONSTRPLN_IS_ON) UT3D_pt_tra_pt_m3 (pto, WC_sur_mat, pti);\
  else if(pto != pti) *pto = *pti;}



//=================== EOF ==============================
