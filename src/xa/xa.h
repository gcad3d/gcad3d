// include with deactivated extern in "../xa/xa.c"
// needs #include "../ut/ut_geo.h"    (Point)


#include "../xa/ap_stat.h"                 // AP_STAT ap_stat - application-status



/// \code
/// applicationObjectNames - displayed in selectionList - used by plugins.
/// see PLU_appNamTab_set
/// \endcode
typedef struct {char* oNam; int oTyp;}                         APP_OBJ_NAM;



//================================================================
// included and defined in ../xa/xa.c (with extern invalidated)
extern char AP_mod_dir[128];      // abs.directory of active model, with '/' at end
extern char AP_mod_fnam[128];     // active Modelname without filetyp
extern char AP_mod_ftyp[32];      // filetyp of active model
extern char AP_mod_sym[64];       // symbolic directory for OPEN (no '/' at end)
  // AP_mod_iftyp: integer-filetyp of active model; eg Mtyp_DXF
extern char AP_mod_iftyp;         // integer-filetyp - eg Mtyp_Gcad|Mtyp_DXF ..
extern char AP_modact_nam[128];   // name of the active submodel; def="" (main)
                                  //   not set in runmode.
extern Point AP_box_pm1, AP_box_pm2;  // box around mainmodel

// extern char AP_dir_save[128];     // directory for SAVE
// extern char AP_sym_save[64];      // symbolic directory for SAVE

extern char AP_dir_prg[128];      // directory for programs
extern char AP_sym_prg[64];       // symbolic directory for programs

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

extern FILE *AP_deb_fp;           ///< debug-file-pointer

extern AP_STAT AP_stat;





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



// test if 2D is ON (if ConstructionPlane is active)
//   if yes: SRC-coordinates are relativ;
#define AP_IS_2D (WC_sur_ind > 0)

// test if 3D is ON (if no user-defined ConstructionPlane is active)
#define AP_IS_3D (WC_sur_ind <= 0)



// test if active model is a subModel being created or the active-model
//   active-model can be a subModel; see MDL_IS_MAIN
#define MDL_IS_SUB (AP_modact_ind >= 0)


/// MDL_IS_MAIN      check if the active model is the mainmodel
/// retCode      0   subModel is active
///              1   the main-model is active
// test if AP_modact_nam == ""  (mainmodel = ""; else subModelname)
#define MDL_IS_MAIN ((AP_modact_nam[0] == 0) ? 1 : 0)





//=================== EOF ==============================
