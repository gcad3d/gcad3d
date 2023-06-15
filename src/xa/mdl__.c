//          Model-managment-functions.        Franz Reiter.  10.Dez.2003
/*
 *
 * Copyright (C) 2021 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
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

use MBTYP_EXTERN/MBTYP_INTERN/MBTYP_CATALOG 0/-1/-2
Mirrorflag -> ModelRef
Ditto-GL: Inhaltelemente brauchen kein DL-Index !

-----------------------------------------------------
Modifications:
2021-03-31 New version. RF.
2003-12-10 Erstmalig erstellt. RF.

-----------------------------------------------------
*/
#ifdef globTag
 void MDL(){}
#endif
/*!
\file  ../xa/mdl__.c     ../xa/mdl__.h
\brief model-managment-functions without GUI
see INF_Models__ INF_workflow_models
\code
=====================================================
List_functions_start:

MDL__IS_MAIN            check if main is active                              INLINE

MDL_load_main           load as mainModel
MDL_load_1              import and scan all models in mdl_tab
MDL_load_2              create loadmap for model imn, load all models
MDL_load_dyn            add new model, temp or perm, add later
MDL_load_mdl__          add new model, temp or perm.
MDL_load_mdl_new        create new empty subModel (usk user for name)
MDL_load_mdl_main       create subModel from active model (usk user for name)
MDL_load_mdl_f          create subModel from load file
MDL_load_mdl_grp        create subModel from group

MDL_load_new__          clear all, init Model_unknown
MDL_load_mNam__         activate main- or internal model
MDL_load_mNam_tmp       activate temp (only mockup; no display)
MDL_clean_f             delete all tmp/Model_* Mod_*.lst *.tess ..
MDL_reset               reset loadmap and mdl_tab
MDL_mdl_tab_set_main    reset mdl_tab to main (for shutdown)

MDL_load_file_split     split file into files tmp/Model_<mNam>
MDL_load_file_cp_mdl    split file - copy subModel into file tmp/Model_<mNam>
MDL_load_file_prc       split file - copy PROCESS
MDL_load_scan__         create list of subModels (tmp/Mod_<safMnam>.lst)
MDL_load_scan_prim      create list of subModels of primary-model
MDL_load_import__       model import (write file tmp/Model_<safMnam>)
MDL_load_import_ext     import as file tmp/Model_ gcad,dxf,stp,igs ..
MDL_load_import_mock    import mockup
MDL_load_import_ctlg    import catalogPart as file tmp/Model_<ctlgPart>
MDL_load_import_attr    write attributes and model (UTF-buffer1)
MDL_load_load           create baseModels, load into DL
MDL_load_unl            unload  dll_imp dll_exp
MDL_load_lst            let user select internal model from list, activate
MDL_load_flst           userSelect new primary-model from list

MDL_init_MDL            clear mdNames mdl_ldm mdl_nam_tmp
MDL_init_noCad          init new mainModel with add. text;

MDL_sav__               save model
MDL_sav_gcad            save mainmodel native as file tmp/Mod_.mod_out
MDL_sav_tmp             save the active sub- or mainmodel AP_modact_nam
MDL_sav_ck_ck           check if model is modified
MDL_sav_ck_new          test for new model (if Mod_.mod_in exists)
MDL_sav_ck_prim         test if primary model is empty
MDL_sav_ck_sm__         test if sm imn belongs to intern or extern parent-model
MDL_sav_ck_sm_1         test if model iCk is using model iSm
MDL_sav_cmp__           compare initial model with modified model
MDL_sav_cmp_i
MDL_exp__               save/export model

MDL_mdb_init__          create all new baseModels
MDL_mdb_load__          load all new baseModels - in reverse order
MDL_mdb_load_1          load baseModel
MDL_mdb_set             create baseModel
MDL_load_sbm            load subModel
MDL_mdb_clear__         clear all baseModels
MDL_mdb_clear_1         clear baseModel

MDL_mdr_clear           clear refModel

MDL_prev_stat_set       set startpositions (for remove following objs later)
MDL_prev_stat_del       delete temp models
MDL_prev_del            delete temp models
MDL_prev_brw_add        add modelRow to browser (temp. model -> perm)

MDL_ren__               rename model
MDL_ren_1               rename model
MDL_ren_fmdl            modify modelfiles - rename or delete submodel
MDL_ren_flst            modify listfiles - rename or delete submodel
MDL_ren_sav             update modelname in browser after save-as

MDL_rem__               remove submodel and its childs
MDL_rem_ck_used         check if model is used
MDL_rem_del             remove subModel
MDL_rem_mNam_f          delete files of subModel

MDL_mod_MODSIZ          modify MODSIZ in all subModels

MDL_lst_sm_prim         make list of all used Submodels of primary-model
MDL_lst_sm_upd          update tmp/Mod_<mPrim>.lst
MDL_lst_sm_add          add subModel uniq to file tmp/Mod_<mPrim>.lst
MDL_lst__               get list of existing subModels
MDL_lst_all             get list of all existing subModels
MDL_lst_stat            get list of models - filter = stat
MDL_lst_ptab            make list of all used PTAB-Surfs
MDL_lst_ptab            make list of all used MSH-Surfs

MDL_file_lst__          make list of all used Submodels -> tmp/Mod.lst
MDL_file_skip_DYNDAT    skip until :DYNAMIC_DATA
MDL_file_ck__           check filetyp and if file exists

MDL_mdl_add             add new name uniq to mdl_names, mdl_tab
MDL_mdl_del             set mdl_tab-record deleted

MDL_brw_upd             update mUsed and Brw-Icons of models
MDL_smuu_upd            set Brw-modelnode from unused -> used
MDL_file_smuu           save / delete unused subModels - ask user

MDL_ldm_f               create loadmap and write into file tmp/Mod.lst
MDL_ldm__               create loadmap for model imn
MDL_ldm_add__           add nameIndex to loadMap mdl_ldm
MDL_ldm_add_flst        add all models in tmp/Mod_<symNam>.lst to loadmap
MDL_LDM_CLEAR           clear loadmap mdl_ldm

MDL_nam__srcLn          get modelTyp and modelName from definition subModel
MDL_nam_src1            check if mNam is modelID or modelname (symb. or intern)

MDL_safNam_prim         get safe modelname of primary model
MDL_safNam_mnam         get safe-name from symbolic-name
MDL_mNam_safe           make safeName from symbolic-name, test length
MDL_safename            make safeName from symbolic-name, test length
MDL_mNam_usr            get new subModelname from user (safe)

MDL_mNam_main           get modelname of mainModel
MDL_mNam_prim           get name of active model (main or subModel)
MDL_mNam_imb            get modelName from index baseModel
MDL_mNam_imr            get modelName from index refModel
MDL_mNam_iNam           get modelname from name-index
MDL_MNAM_INAM           get modelname from name-index                        INLINE
MDL_mNam_cmp_sNam       compare symbolic-Mdlnam - safe-Mdlnam

MDL_imn_mNam__sNam      get symbolic-name and name-index from safe-name
MDL_fnModLst_iNam       get filename Mod_<modelname>.lst from mdl_tab-index
MDL_fnModLst_mNam       get filename Mod_<safe_modelname>.lst                INLINE
MDL_fnModTess_mNam      get filename Mod_<safe_modelname>.tess (mockup)      INLINE
MDL_fnModel_mNam        get filename Model_<primaryModel>
MDL_fnModel_main        get filename Model_<mainModel>

MDL_MTYP_IS_MOCK        check if model is mockup (tess obj, stl, wrl)        INLINE
MDL_MTYP_INAM           get modelType from name-index                        INLINE
MDL_mTyp_mNam           get modeltyp from symbolic-modelname
MDL_mTyp_sNam           get modeltyp from safe-modelname
MDL_MTYP_SET_INAM       set modelType for model from name-index              INLINE
MDL_mTyp_set_sNam       set modeltyp from safe-modelname

MDL_IMB_INAM            get index-baseModel from name-index                  INLINE
MDL_imb_mNam            get basicModelIndex from basicModelName 
MDL_imb_imr             get basicModelIndex from refModelIndex

MDL_INAM_MNAM           get index-modelName from modelName                   INLINE
MDL_iNam_mNam           get index-modelName from modelName (for extern modules)
MDL_iNam_sNam           get index-modelName from safe-modelName

MDL_stat_mNam           get stat from model-name
MDL_STAT_INAM           get stat for model from name-index                   INLINE
MDL_STAT_SET_INAM       set stat for model from name-index                   INLINE

MDL_used_set__          get mUsed for all models (active|passive|unused)
MDL_USED_INAM           get mUsed for model from name-index                  INLINE
MDL_USED_SET_INAM       set mUsed for model from name-index                  INLINE

MDL_msg_no_sm           "submodel must be active"
MDL_typInf              infoText from modeltype
MDL_dump__              dump loadmap mdl_ldm
MDL_test__
MDL_test_init           init MDL with "unknown"

List_functions_end:

../xa/xa.h
MDL_IS_MAIN             check if active model is the mainmodel
MDL_IS_SUB              check if active model is active-model or sm-being-created
=====================================================

UNUSED:
MDL_mNamf_prim          get modelfilename of active model (main or subModel)
MDL_load_file_gcad      get mainModel into tmp/Mod_.tmp_in
MDL_smuu_lst            get list of unused subModels REPLACED by MDL_lst_stat
MDL_smuu__              create list of unused-subModels, display in Brw
MDL_stat_set            set stat for model from model-name in mdl_tab
MDL_stat_upd            update mdl_tab[].stat
MDL_prev_stat__         set preview-status perm | temp
MDL_load_add            model import, add to modMap
MDL_mdl_smuu            add unused-subModels to mdl_names, mdl_tab
MDL_prev__              preview model (in temporary-mode)
MDL_imp__
was ../xa/xa_mod.c ../xa//mdlmp.c


=====================================================
Extern:
char AP_mod_sym[SIZMFSym];       // symbolic directory for OPEN (no '/' at end)
char AP_mod_fnam[SIZMFNam];      // active mainModelname without filetyp
                           // safe, as displayed in Brw-mainRow
char AP_mod_ftyp[SIZMFTyp];      // filetyp of active mainModel

char AP_modact_nam[128];   // name of the active submodel; def="" (main)
int  AP_modact_inm         // index name (mdl_names) of primaryModel
int  AP_modact_ibm         // index into subModelList
  //  -1: primary-Model is active. The primaryModel can be subModel.
  // >=0: subModel is being created.
  // get it with AP_get_modact_ind()
int  AP_modact_tmp;         // 1=temp-model-exists; 0=not

 DL_Att.modInd       -1=mainmodel, else basicModelNr (index mdb_dyn)
 mdb_dyn              List of basicModels           see INF_basModels_DB
 mdr_tab              List of modelReferences       see INF_refModels_DB




=====================================================
Testmodels:
  Data_U/test_mod_0.gcad   
  Data_U/test_mod_1.gcad      // intern1
  Data_U/test_mod_2.gcad      // Profile_4x4_60.ctlg
  Data_U/test_mod_3.gcad      // OBJ_U/myHome3.obj
  Data_U/test_mod_4.gcad      // Data_U/Niet1.gcad
  Data_U/test_mod_5.gcad      // images   
  Data_U/test_mod_6.gcad      // alle

./do comp ELE1/box1.gcad
./do comp ELE1/board1.gcad
./do comp Data_U/t1.gcad
./do Data_U/t5.gcad                      // testfile save
./do Data_U/test1.gcaz

./do Data_U/samp_anilin.wrl
./do OBJ_U/myHome3.obj
./do Data_U/sample_gis1.gcad                                         <<<< TODO
./do Data_U/Haus1.gcad

./do Data_U/sample_dxf1.dxf                           OK
./do Data_U/sample_Iges1.igs           // iges single surf
./do IGS_U/submod3.igs                 // iges mit subModels
./do Data_U/sample_step1.stp                          OK
./do Data_U/shuttle1.3ds                              <<<<
./do LWO_U/penguin.lwo                                <<<<
./do STL_U/Zettler1.stl                               <<<<
XML-NIX.
./do Data_U/test1.svg

./do Data_U/samp_wrl1.wrl
./do Data_U/samp_anilin.wrl
./do OBJ_U/blender1.obj
./do OBJ_U/myHome3.obj
./do TESS_U/t1.tess

./do Data_U/DachWS4.jpg
./do Data_U/sample_Ziegel1.bmp



===================================================== */
#ifdef globTag
 void INF_MDL__(){}
#endif 
/*
=====================================================

static tables:
mdl_names    list of all modelnames (TxtTab)
mdl_tab      typ, stat .. for all modelnames - parallel to mdl_names, 
             (struct stru_tab)

mdl_ldm      loadmap - list of indexes into mdl_names

int  AP_modact_inm             // loadMap-index of primary model; 0=main
char mdl_nam_tmp[]         // temp. modelname

see INF_basModels__

=====================================================
*/




#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>                         /* fabs  ...   */
#include <stdio.h>                        // FILE ..
#include <stdlib.h>
#include <string.h>                       /* strcmp ..   */

#include "../gui/gui_types.h"              // UI_Func..

#include "../ut/ut_geo.h"                // ModelNode
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_itmsh.h"            // MSHIG_EDGLN_.. typedef_MemTab.. Fac3
#include "../ut/ut_txTab.h"              // TxtTab
#include "../ut/AP_types.h"              // Mtyp_*
#include "../ut/ut_os.h"                 // AP_get_tmp_dir
#include "../ut/ut_txt.h"              // UTX_*
#include "../ut/ut_txfil.h"            // UTF_GetPosLnr
#include "../ut/ut_deb.h"              // DEB_*
#include "../ut/os_dll.h"              // DLL_*

#include "../xa/xa_msg.h"              // MSG_* ERR_*
#include "../xa/xa.h"                  // AP_modact_nam AP_modact_tmp
#include "../xa/xa_uid.h"              //  UI_MODE_MAN
#include "../xa/xa_mem.h"              // mem_cbuf1
#include "../xa/xa_ico.h"              // ICO_natM0


#include "../gr/ut_gr.h"               // GR_TMP_I0

#include "../db/ut_DB.h"                 // DB_get_ModBas


#define  APED_SRCLN_BUFSIZ mem_cbuf1_SIZ   // 200000 ../xa/xa_mem.h
#define  APED_SRCLN_BUF__  mem_cbuf1


#define mdlStat_primary  2   // primary model            ICO_natML
#define mdlStat_active   1   // used in active model     ICO_natML
#define mdlStat_passive  0   // not used in active model ICO_natMU
#define mdlStat_unused  -1   // unused                   ICO_natM0
#define mdlStat_deleted -2   // deleted



//================================================================

// mbi       index baseModel
// mTyp      modeltype, eg MBTYP_EXTERN
// mStat     0  name added in mdl_names, mdl_tab 
//           1  import,scan; create Model_<mNam>, Mode_<mn>.lst, empty basMdl
//           2  loaded in dispList and basMdl - can be used for refMdl
//          -2  deleted-model
// mMain     0=unused-by-main 1=used-by_main
// mUsed     0  mdlStat_passive       not used in active model
//           1  mdlStat_active        used in active model
//           2  mdlStat_primary       primary model
//          -1  mdlStat_unused        unused
typedef struct {int mbi; char mTyp, mStat, mMain, mUsed, u5,u6,u7,u8;} stru_tab;

//                    mbi    
stru_tab _TAB_NUL = {(int)-1,
//                    mTyp               mStat    mMain    mUsed
                     (char)MBTYP_UNDEF, (char)0, (char)0, (char)mdlStat_unused,
//                    u5 ..
                     0, 0, 0, 0};


// prototypes:
typedef_MemTab(stru_tab);


// obj-stat;  active only if AP_modact_tmp == 1
//   used by MDL_prev_stat_del
// stat     UU
// iLdm     UU index mdl_ldm
// iTab     index mdl_tab & mdl_names (remove all following)
// iMdr     index refModels (remove all following)
// iMdb     index baseModels (remove all following)
// dlTmp    index temp-dispList (remove all following)
// dlPerm   index perm-dispList (remove all following)
static struct {int stat, iLdm, iTab, iMdr, iMdb, dlTmp; long dlPerm;} mdl_stat;


// see ../xa/xa_ico.h
static int icoTab[]={ICO_natM0, ICO_natMU, ICO_natML, ICO_natMA};


//============ Extern Vars =======================================
// ../xa/xa.c
extern int       AP_src;
extern char*     APT_defTxt;          // die Textzeile ohne N#=


// ../xa/xa_ui.c
extern int       UI_InpMode;

// ../ci/NC_Main.c
extern int       APT_obj_stat;         // 0=permanent, 1=temporary (workmode)
extern int       APT_lNr;              // active lineNr of SRC code

// ../ut/ut_txfil.c
extern long      UTF_FilBuf0Len;       // memspc for the active modeldata (src)

// ../gr/ut_DL.c
extern long      GR_TAB_IND;           // nr of dispList-records

// // aus xa_ga.c:
extern int       GA_recNr;             // die aktuelle Anzahl von Records



//============ Locale Vars =======================================
// List of all (symbolic) modelnames; first name is name of mainModel
// permanent list
       TxtTab     mdl_names = _UTXTAB_NUL;

// static loadmap - list of indexes into mdl_names
       MemTab(int)        mdl_ldm = _MEMTAB_NUL;

// static list of aux-infos parallel to mdl_names
       MemTab(stru_tab)   mdl_tab = _MEMTAB_NUL;

// static char       mdl_nam_tmp[SIZMFNam];   // temp. modelname

static char       sSecEnd[]="SECTIONEND";


static void *dll_imp = NULL;
static void *dll_exp = NULL;


//================================================================

// MDL__IS_MAIN            check if main is active
#define MDL__IS_MAIN (!AP_modact_inm)

// MDL_MBI_INAM            get baseModelIndexe from name-index
#define MDL_MBI_INAM(inm) (mdl_tab.data[inm].mbi)

// MDL_MBI_SET_INAM        set baseModelIndexe from name-index
#define MDL_MBI_SET_INAM(inm, indMb) (mdl_tab.data[inm].mbi = indMb)

// // MDL_MNAM_MAIN           get modelname of mainModel
// char* MDL_MNAM_MAIN ();
// #define MDL_MNAM_MAIN() (UtxTab__(0, &mdl_names))

// MDL_MNAM_INAM           get modelname from name-index
// see also MDL_mNam_iNam
char* MDL_MNAM_INAM (int);
#define MDL_MNAM_INAM(inm) (UtxTab__(inm, &mdl_names))


// MDL_STAT_INAM           get stat for model from name-index
#define MDL_STAT_INAM(inm) (mdl_tab.data[inm].mStat)

// MDL_STAT_SET_INAM       set stat for model from name-index
#define MDL_STAT_SET_INAM(inm,stat) (mdl_tab.data[inm].mStat = stat)


// MDL_USED_INAM           get mUsed for model from name-index
#define MDL_USED_INAM(inm) (mdl_tab.data[inm].mUsed)

// MDL_USED_SET_INAM       set mUsed for model from name-index
#define MDL_USED_SET_INAM(inm,used) (mdl_tab.data[inm].mUsed = used)


// MDL_MTYP_SET_INAM       set modelType for model from name-index
#define MDL_MTYP_SET_INAM(inm,typ) (mdl_tab.data[inm].mTyp = typ)

// MDL_MTYP_INAM           get modelType from name-index
#define MDL_MTYP_INAM(inm) (mdl_tab.data[inm].mTyp)

// MDL_mTyp_iNam           get modelType from name-index
int MDL_mTyp_iNam (int inm) { return mdl_tab.data[inm].mTyp; }

// MDL_IMB_INAM            get index-baseModel from name-index
#define MDL_IMB_INAM(inm) (mdl_tab.data[inm].mbi)


// MDL_INAM_MNAM           get index-modelName from (symbolic) modelName
//   returns -1 for name-not-found
#define MDL_INAM_MNAM(mNam) (UtxTab_find (mNam, &mdl_names))


// MDL_LDM_CLEAR           clear loadmap mdl_ldm
#define MDL_LDM_CLEAR() (mdl_ldm.rNr = 0)


// MDL_fnModLst_mNam       get filename Mod_<mNam>.lst
// Input:  mNam - safe name
// Output: fNam - size 320, 
#define MDL_fnModLst_mNam(fNam,mNam)\
 (sprintf(fNam,"%sMod_%s.lst",AP_get_tmp_dir(),mNam))




//================================================================
//================================================================
//================================================================


//================================================================
  int MDL_fnModLst_iNam (char *lNam, int iNam) {
//================================================================
// MDL_fnModLst_iNam            get filename Mod_<modelname>.lst from mdl_tab-index
// see also MDL_fnModLst_mNam


  char    mNam[SIZFNam];

  strcpy(mNam, UtxTab__(iNam, &mdl_names));
  UTX_safeName (mNam, 1);
  sprintf(lNam, "%sMod_%s.lst", AP_get_tmp_dir(), mNam);

    // printf(" ex-MDL_fnModLst_iNam %d |%s|\n",iNam,lNam);

  return 0;

}


//================================================================
  int MDL_fnModel_main (char *fn) {
//================================================================
// MDL_fnModel_main        get filename Model_<mainModel>

  char      *mm;

  mm = UtxTab__(0, &mdl_names);

  // get new name "Model_<AP_mod_sym>_<AP_mod_fnam>_<AP_mod_ftyp>"
// AP_mod_fnam AP_mod_ftyp AP_modact_nam AP_modact_inm
//   // make mnam safe
  // strcpy (safNam, mmNam);
  // UTX_safeName (safNam, 1);
  sprintf(fn, "%sModel_%s_%s_%s", AP_get_tmp_dir(),AP_mod_sym,AP_mod_fnam,AP_mod_ftyp);

    printf("ex-MDL_fnModel_main |%s|\n",fn);

  return 0;

}


//================================================================
  int MDL_fnModel_mNam (char *fn, int  imn) {
//================================================================
// MDL_fnModel_mNam        get filename Model_<safe_modelname>
// Input:
//   imn        index name (mdl_names) of primaryModel
//   fn         size must be >= 320
// Output
//   fn         full filename of modelfile - eg "/base/tmp/Model_unknown_gcad"
//   retCode    0=OK, -1=ERR

  int     irc;
  char    safNam[SIZMFTot], *mm;


  // printf("MDL_fnModel_mNam %d\n",imn);

  mm = UtxTab__(imn, &mdl_names);
  if(!mm) {TX_Print("***** Err MDL_fnModel_mNam %s",imn); irc = -1; goto L_exit;}

  strcpy(safNam, mm);
  UTX_safeName (safNam, 1);
  sprintf(fn, "%sModel_%s",AP_get_tmp_dir(), safNam);

  irc = 0;

  //----------------------------------------------------------------
  L_exit:
      // printf(" ex-MDL_fnModel_mNam %d |%s| %d\n",irc,fn,imn);

    return irc;

}


//================================================================
  int MDL_iNam_mNam (char *syFn) {
//================================================================
// MDL_iNam_mNam           get modelname from name-index (for extern modules

  return MDL_INAM_MNAM (syFn);

}


//================================================================
  int MDL_iNam_sNam (char *sNam) {
//================================================================
// MDL_iNam_sNam           get index-modelName from safe-modelName


  int     i1, imn, *itxa;
  char    safNam[SIZMFTot];


  itxa = mdl_names.ind;

  // check if string <symNam> already exists;
  for(i1=0; i1<mdl_names.iNr; ++i1) {
    strcpy(safNam, &mdl_names.tab[itxa[i1]]);
    UTX_safeName (safNam, 1);
    if(!strcmp(safNam, sNam)) {
      imn = i1;
      goto L_exit;    // alread exists 
    }
  }

  imn = -1;

  //----------------------------------------------------------------
  L_exit:
    // MDL_dump__ ("ex-MDL_mdl_add");
    // printf("ex-MDL_iNam_sNam %d |%s|\n",imn,sNam);
  return imn;

}


//================================================================
  int MDL_mNam_imb (char *mNam, int *sSiz, int *typ, int imdb) {
//================================================================
// MDL_mNam_imb            get modelName from index baseModel
// sSiz unused

  ModelBas  *mb1;

  // printf("MDL_mNam_imb imdb=%ld siz=%d\n",imdb,*sSiz);

  mb1 = DB_get_ModBas (imdb);
  if(!mb1) {TX_Error("MDL_mNam_imb E1-%d",imdb); return -1;}
    // printf(" mnam=|%s| typ=%d\n",mb1->mnam,mb1->typ);

  strcpy (mNam, mb1->mnam);
  *typ = mb1->typ;

  return 0;

}


//================================================================
  char* MDL_mNam_imr (int imr) {
//================================================================
// MDL_mNam_imr            get modelName from index refModel
// returns   modelName, NULL = error

  int      imb;
  char     *mnam;
  ModelRef *mr1;
  ModelBas *mb1;


  // get baseModelNr
  mr1 = DB_get_ModRef (imr);
  imb = mr1->modNr;

  // get baseModel
  mb1 = DB_get_ModBas (imb);
  if(!mb1) { mnam = NULL; goto L_exit;}

  mnam = mb1->mnam;


  L_exit:
      // printf(" ex-MDL_iNam_imr |%s| imr=%d\n",mnam,imr);
    return mnam;

}

//================================================================
  int MDL_mNam_cmp_sNam (char *mNam, char *sNam) {
//================================================================
// MDL_mNam_cmp_sNam       compare symbolic-Mdlnam - safe-Mdlnam
// Input:
//   mNam    symbolic (or safe)
//   sNam    safe-name
// returns   0     different names
//           else  ident names

  char    safNam[SIZMFTot];


  strcpy(safNam, mNam);
  UTX_safeName (safNam, 1);

  return (!strcmp (safNam, sNam));

}


//================================================================
  int MDL_mdr_clear (long dbi) {
//================================================================
// MDL_mdr_clear            clear refModel
// Input:
//   dbi     DB-index refModel to clear; -1 = clear all refModels

  // int     irc;

  // printf("MDL_mdr_clear %ld\n",dbi);

  // clear refModel in DB (free)
  DB_del_MR__ (dbi);

//   // check if its baseModel is unused; if yes:
//   // - deactivate all objs of basMdl in DL (see mdl_tab-DLind,DLsiz)
//   // - basMdl in DB: reset DLind, DLsiz
//   // - model in mdl_tab: reset mStat to 1 (imported, not loaded)

  // L_exit:
    // printf("ex-MDL_mdr_clear\n");
  return 0;

}


//================================================================
  int MDL_mdb_clear__ () {
//================================================================
// MDL_mdb_clear__         clear all baseModels
// - set   baseModel[].DLind and DLsiz -1  (not loaded)
// - reset mdl_tab-mStat to 1 (imported, not loaded)


  int        i1, mdbNr;
  ModelBas   *mdba;

  // printf("MDL_mdb_clear__ \n");

  DB_get_mdb (&mdbNr, &mdba);

  for(i1=0; i1<mdbNr; ++i1) MDL_mdb_clear_1 (i1);

  return 0;

}


//================================================================
  int MDL_mdb_clear_1 (int imb) {
//================================================================
// MDL_mdb_clear_1            clear baseModel
// - set   baseModel[].DLind and DLsiz -1  (not loaded)
// - reset mdl_tab-mStat to 1 (imported, not loaded)
// Input:
//   imb     index baseModel


  int        mbNr, imn;
  char       *mNam;
  ModelBas   *mba;
  stru_tab   *tab;



  // printf("MDL_mdb_clear_1 %d\n",imb);

  // get baseModels
  DB_get_mdb (&mbNr, &mba);


  if((imb <0)||(imb >= mbNr)) {
    TX_Print("***** ERR MDL_mdb_clear_1 %d",imb);
    goto L_exit;
  }


  // get name-index from basic-model-index
  mNam = mba[imb].mnam;
  imn = MDL_INAM_MNAM(mNam);


  // ignore unused and deleted models
  tab = MEMTAB_DAT (&mdl_tab);
  if(tab[imn].mStat < 2) goto L_exit;


  // basMdl in DB: reset DLind, DLsiz
  mba[imb].DLind = -1;
  mba[imb].DLsiz = -1;


  // model in mdl_tab: reset mStat to 1 (imported, not loaded)
  tab[imn].mStat = 1;


  L_exit:
    // printf("ex-MDL_mdb_clear_1 %d\n",irc);
  return 0;

}

/* UU
//================================================================
  int MDL_prev_stat__ (int mdlStat) {
//================================================================
// MDL_prev_stat__           set perm | temp
// Input:
//   mdlStat         OBJSTAT_temp (1) | OBJSTAT_perm (0)
// was ..

  static int modified = 0;  // 0=no-temp-model-ative; 1=temp-mode-is-active;

  void     *v1;


  printf("MDL_prev_stat__ new=%d act=%d\n",mdlStat,mdl_stat.stat);
  MDL_dump__ ("MDL_prev_stat__");



  if(mdlStat == OBJSTAT_perm) {
    // set to perm

    //================================================================
    // perm following perm: do nothing
    if(mdl_stat.stat == OBJSTAT_perm) goto L_exit;


    //================================================================
    // perm following temp:   if modif delete
    mdl_stat.stat = OBJSTAT_perm;
    MDL_prev_stat_del ();
    goto L_exit;


  //----------------------------------------------------------------
  } else {
    // set to temp
 
    //================================================================
    // temp following perm:   set
    if(mdl_stat.stat == OBJSTAT_perm) {
      mdl_stat.stat = OBJSTAT_temp;
      goto L_set__;
    }


    //================================================================
    // temp following temp:   if modif delete; set.
    if(mdl_stat.stat == OBJSTAT_temp) 
    MDL_prev_stat_del ();
    goto L_set__;
  }


  //================================================================
  L_set__:
    // set startpositions (for remove following objs later)
    mdl_stat.iTab = mdl_tab.rNr;
    mdl_stat.iLdm = mdl_ldm.rNr;
    DB_get_mdr (&mdl_stat.iMdr, &v1);
    DB_get_mdb (&mdl_stat.iMdb, &v1);
    mdl_stat.dlTmp = GL_temp_iNxt ();
    mdl_stat.dlPerm  = GL_Get_DLind ();
      printf(" _prev_stat__ mdl_stat.iTab=%d .iLdm=%d\n",
             mdl_stat.iTab,mdl_stat.iLdm);
      printf(" _prev_stat__ mdl_stat.iMdr=%d .iMdb=%d\n",
             mdl_stat.iMdr, mdl_stat.iMdb);
      printf(" _prev_stat__ mdl_stat.dlTmp=%d .dlPerm=%ld\n",
             mdl_stat.dlTmp, mdl_stat.dlPerm);


  //================================================================
  L_exit:
    // WC_set_obj_stat (mdl_stat.stat);

    return 0;

}
*/

//================================================================
  int MDL_prev_stat_set () {
//================================================================
// MDL_prev_stat_set                   set startpositions (for remove following objs later)

  void     *v1;


  // set startpositions (for remove following objs later)
  mdl_stat.iTab = mdl_tab.rNr;
//   mdl_stat.iLdm = mdl_ldm.rNr;
  DB_get_mdr (&mdl_stat.iMdr, &v1);
  DB_get_mdb (&mdl_stat.iMdb, &v1);
  mdl_stat.dlTmp = GL_temp_iNxt ();
  mdl_stat.dlPerm  = GL_Get_DLind ();

  AP_modact_tmp = 1;

    // TESTBLOCK
    // printf(" _prev_stat__ mdl_stat.iTab=%d\n",mdl_stat.iTab);
    // printf(" _prev_stat__ mdl_stat.iMdr=%d .iMdb=%d\n",
           // mdl_stat.iMdr, mdl_stat.iMdb);
    // printf(" _prev_stat__ mdl_stat.dlTmp=%d .dlPerm=%ld\n",
           // mdl_stat.dlTmp, mdl_stat.dlPerm);
    // END TESTBLOCK

  return 0;

}


//================================================================
  int MDL_prev_del () {
//================================================================
// MDL_prev_del            delete temp models

  if(AP_modact_tmp) MDL_prev_stat_del ();

  return 0;

}


//================================================================
  int MDL_prev_brw_add () {
//================================================================
// MDL_prev_brw_add        add modelRows to browser (temp. model -> perm)
// - create modelRow for all mdl_tab-records >= mdl_stat.iTab

  int   i1, mUsed;
  char  safNam[SIZMFTot];


  // printf("MDL_prev_brw_add\n");


  for(i1=mdl_stat.iTab; i1<mdl_tab.rNr; ++i1) {
    // if(!mdl_tab.data[i1].mLife) continue;

    strcpy(safNam, MDL_MNAM_INAM(i1));
    UTX_safeName (safNam, 1);

    mUsed = mdl_tab.data[i1].mUsed;
    Brw_Mod_add (safNam, icoTab[mUsed + 1]);
    // mdl_tab.data[i1].mLife = 0;
  }

  return 0;

}


//================================================================
  int MDL_prev_stat_del () {
//================================================================
// MDL_prev_stat_del       delete temp models
// Input:
//   mdl_stat     startindexes; delete all following objs
// Output:
//   retCode      0=OK, -1=err.

// remove what:
// - loadmap-records (mdl_ldm)
// - name and name-auxInfo (mdl_name & mdl_tab)
// - refModels
// - basicModels
// - permanent dispListrecords
// - temporary dispListrecords
// DOES NOT YET delete tmp/<mnam>.bmp and .tess


  int     i1, mTyp;
  char    mn[SIZMFTot], s1[320];

  // printf("MDL_prev_stat_del \n");
  // MDL_dump__ ("MDL_prev_stat_del-in");


  //----------------------------------------------------------------
  // delete tmp/Model_* and tmp/Mod_*.list and tmp/<catalog>.dat
  // of all deleted models
//   ia = mdl_ldm.data;
//   for(i1=mdl_stat.iLdm; i1<mdl_ldm.rNr; ++i1) {

  for(i1=mdl_stat.iTab; i1<mdl_tab.rNr; ++i1) {

    // make safename
    strcpy(mn, UtxTab__(i1, &mdl_names));
      // printf(" _stat_del-L1 |%s|\n",mn);
    UTX_safeName (mn, 1);

    // del. tmp/Model_<mnam>
    sprintf(s1, "%sModel_%s",AP_get_tmp_dir(), mn);
    OS_file_delete (s1);

    // del. tmp/DB__<mnam>.dat
    sprintf(s1, "%sDB__%s.dat",AP_get_tmp_dir(), mn);
    OS_file_delete (s1);

    // del. tmp/Mod_<mnam>.lst
    sprintf(s1, "%sMod_%s.lst",AP_get_tmp_dir(), mn);
    OS_file_delete (s1);

    // catalog-file: del tmp/<partName>.dat
    if(MDL_MTYP_INAM(i1) == MBTYP_CATALOG) {
      sprintf(s1, "%s%s.dat",AP_get_tmp_dir(), mn);
      OS_file_delete (s1);

    }
  }



//   //----------------------------------------------------------------
//   // remove loadmap-records > mdl_stat.iLdm
//   mdl_ldm.rNr = mdl_stat.iLdm;

  // remove aux-infos > mdl_stat.iTab
  mdl_tab.rNr = mdl_stat.iTab;

  // remove names > mdl_stat.iTab
  mdl_names.iNr = mdl_stat.iTab;

    // MDL_dump__ ("MDL_prev_stat_del-L1");


  //----------------------------------------------------------------
  // remove all temp. refModels
  DB_del_ModRef (mdl_stat.iMdr);

  // remove refModel M0
  DB_del_MR__ (0L);

  // remove baseModel 0
  DB_clear_ModBas (0L);

  // remove perm-displist
  GL_Delete (mdl_stat.dlPerm);

  // remove temp-displist
  GL_temp_del__ (mdl_stat.dlTmp);


  //----------------------------------------------------------------
  AP_modact_tmp = 0;

    // MDL_dump__ ("ex-MDL_prev_stat_del");
    // printf("ex-MDL_prev_stat_del\n");

  return 0;

}


//================================================================
  int MDL_file_ck__ (int rTyp, char *syFn) {
//================================================================
// MDL_file_ck__           check filetyp and if file exists
// Input:
//   rTyp     requested filetyp; check if syFn has this filetyp; test -
//            Typ_SubModel (internal), TYP_FilNam (external), Typ_CtlgPart (catalog)
// Output:
//   retCode  0=OK, -1=wrong filetype, -2=file-not-exists;


  int        irc, mTyp;
  char       fn[SIZFNam];
  stru_FN    ofn;


  // printf("MDL_file_ck__ rTyp=%d |%s|\n",rTyp,syFn);


  // get fn = full filename, type ..
  irc = MDLFN_oFn_fNam (&ofn, syFn);
  if(irc < 0) goto L_exit;

  mTyp = ofn.iTyp;


  //----------------------------------------------------------------
  if(rTyp == Typ_SubModel) {             // "M intern"
    if(mTyp != MBTYP_INTERN) {
      TX_Print ("***** internal models may not have filetyp ..");
      goto L_err1;
    }
    sprintf(fn, "%s%s", ofn.fDir, ofn.fNam);
    goto L_ck_exist;


  //----------------------------------------------------------------
  } else if(rTyp == TYP_FilNam) {       // "M extern"
    if(mTyp == MBTYP_CATALOG) {
      TX_Print ("***** use \"M CatalogPart\"  ..");
      goto L_err1;

    } else if(mTyp < Mtyp_Gcad) {
      TX_Print ("***** wrong filetyp - must be .stp or .dxf  ..");
      goto L_err1;

//     } else if(mTyp >= Mtyp_BMP) {
// makes bug with N#=IMG P# "xx.bmp" (CAD, " Image, ..", filename, LIST, sel. x.jpg)
//       // cannot use images
//       TX_Print ("***** use images with \"Text / N-image\"");
//       goto L_err1;
    }
    sprintf(fn, "%s%s.%s", ofn.fDir, ofn.fNam, ofn.fTyp);
    goto L_ck_exist;


  //----------------------------------------------------------------
  } else if(rTyp == Typ_CtlgPart) {     // "M CatalogPart"
    if(mTyp != MBTYP_CATALOG) {
      TX_Print ("***** wrong filetyp - must be .ctlg ..");
      goto L_err1;
    }
    // get filename catalogfile
    sprintf(fn, "%s%s.%s", ofn.fDir, ofn.symDir, ofn.fTyp);
    goto L_ck_exist;




  //----------------------------------------------------------------
  } else {
    printf("***** MDL_file_ck__ E1");
    goto L_exit;
  }


  //----------------------------------------------------------------
  L_ck_exist:
    // test if file exists
    irc = OS_checkFilExist (fn, 1);
    if(!irc) {
      if(rTyp == Typ_SubModel) TX_Print ("***** submodel not loaded ..");
      if(rTyp == TYP_FilNam) TX_Print ("***** file %s not found ..",fn);
      if(rTyp == Typ_CtlgPart) TX_Print ("***** catalog %s not found ..",fn);
    }

  //----------------------------------------------------------------
  L_exit:
      // printf(" ex-MDL_file_ck__ %d\n",irc);
  return irc;


  //----------------------------------------------------------------
  L_err1:
    irc = -1;
    goto L_exit;

}


//================================================================
  int MDL_file_lst__ (int mode) {
//================================================================
// MDL_file_lst__              make list of all models in mdl_tab -> tmp/Mod.lst
//   used by active primary-model (list of loadMap)
//   creates file tmp/Mod.lst; writes all models in mdl_tab out
// Input:
//     mode     0 symbolic-name-out; eg "Data/Niet.gcad"
//              1 safe-names-out;    eg "Data_Niet_gcad"
// Output:
//   retCode      0=OK, -1=err.
//
// was Mod_flst_all


  int   ii;
  char  safNam[SIZMFTot], *symNam, fnam[320];
  FILE  *fpo;

  // printf("MDL_file_lst__ \n");
  // MDL_dump__ ("MDL_file_lst__");

  sprintf(fnam,"%sMod.lst",AP_get_tmp_dir());
    // printf(" listfile=|%s|\n",fnam);

  // open outfile
  if((fpo=fopen(fnam,"w")) == NULL) {
    TX_Print("***** MDL_file_lst__ E001 %s",fnam);
    return -1;
  }

  // loop tru mdl_tab (skip main) make safNam
  for(ii=1; ii<mdl_tab.rNr; ++ii) {
    symNam = UtxTab__(ii, &mdl_names);
    if(mode) {
      MDL_safename (safNam, symNam);
      fprintf(fpo,"%s\n",safNam);
    } else {
      fprintf(fpo,"%s\n",symNam);
    }
  }

  fclose(fpo);

  L_exit:
    // printf("ex-MDL_file_lst__ \n");
  return 0;

}


//================================================================
  int MDL_load_flst () {
//================================================================
// MDL_load_flst           userSelect new primary-model from list
// Input:
// Output:
//   retCode      0=OK, -1=err.
// was ..

  int     irc;
  char    fn[320], s1[320];


  // printf("MDL_load_flst \n");


  // make file tmp/Mod.lst
  irc = MDL_file_lst__ (0);

  // let user select from list of Submodelnames
  sprintf(fn,"%sMod.lst",AP_get_tmp_dir());
  irc = GUI_listf1__ (s1, sizeof(s1), fn, "- select modelxxi -", "x40,y40");
  if(irc < 0) goto L_exit;

  // activate selected model
  MDL_load_mNam__ (s1, 0);


  L_exit:
    // printf("ex-MDL_load_flst %d\n",irc);
  return irc;

}


//================================================================
  int MDL_load_mdl_new () {
//================================================================
// MDL_load_mdl_new         user define new subModel
// - create new empty model, add to browser; do not activate.
// Input:
// Output:
//   retCode      0=OK, -1=err.
//
// was Mod_cre__


  int     irc, inm;
  char    mNam[SIZMFTot], s1[320];

  // printf("MDL_load_mdl_new \n");


  // get smNam = new subModelname from user
  irc = MDL_mNam_usr (mNam);
  if(irc < 0) return -1;
    // printf(" Mod_cre__-mNam=|%s|\n",mNam);


//   // save the active Submodel AP_modact_nam -> TempFile
//   Mod_sav_tmp ();

  //----------------------------------------------------------------
  // add new model to mdl_names, mdl_tab
  inm = MDL_mdl_add (MBTYP_INTERN, mNam);
  if(inm < 0) {TX_Print("***** MDL_load_mdl_new E1 %s",mNam); irc = -1; goto L_exit;}

  // set model unused
  MDL_USED_SET_INAM (inm, mdlStat_unused);

  // add new model to loadmap
  irc = MDL_ldm_add__ (inm);
  if(irc < 0) {TX_Print("***** MDL_load_mdl_new E2 %s",mNam); goto L_exit;}

  // init new mainModel with add. line s1
  sprintf(s1, "# Submodel %s\n", mNam);
  MDL_init_noCad (mNam, s1);

  //----------------------------------------------------------------
  // add new model to browser
  Brw_Mod_add (mNam, ICO_natM0);

  TX_Print("**** empty subModel %s created ..",mNam);

  irc = 0;



  L_exit:
    // printf("ex-MDL_load_mdl_new %d\n",irc);
  return irc;

}


//================================================================
  int MDL_load_mdl_main () {
//================================================================
// MDL_load_mdl_main       create subModel from active model (usk user for name)
// was Mod_m2s__

  int     irc, imn;
  char    mNam[SIZMFNam], fn1[SIZFNam], fn2[SIZFNam],
          *mnAct, safMnAct[SIZMFTot], s1[320];


  // printf(" MDL_load_mdl_main\n");
  // MDL_dump__ ("MDL_load_mdl_main");



  // get mNam = new-modelname from user
  irc = MDL_mNam_usr (mNam);
  if(irc < 0) goto L_exit;
  // make safename  (internal!)
  UTX_safeName (mNam, 3);


  // get modelname of active-model
  mnAct = MDL_MNAM_INAM (AP_modact_inm);
  // make safe
  strcpy (safMnAct, mnAct);
  UTX_safeName (safMnAct, 1);


  // add new-modelname to mdl_names and mdl_tab
  imn = MDL_mdl_add (MBTYP_INTERN, mNam);
  if(imn < 0) {TX_Print("***** MDL_load_mdl_main E1 %s",mNam); irc = -1; goto L_exit;}

  // set mUsed to unused
  MDL_USED_SET_INAM (imn, mdlStat_unused);

  
  //----------------------------------------------------------------
  // unload editor, clear memspc
  MDL_sav_tmp ();   // this is the new subModel

  //----------------------------------------------------------------
  // rename active-model -> new-model
  sprintf(fn1,"%sModel_%s",AP_get_tmp_dir(),safMnAct);
  sprintf(fn2,"%sModel_%s",AP_get_tmp_dir(),mNam);
  rename (fn1, fn2);

  // rename active-modelList -> new-modelList
  sprintf(fn1,"%sMod_%s.lst",AP_get_tmp_dir(),safMnAct);
  sprintf(fn2,"%sMod_%s.lst",AP_get_tmp_dir(),mNam);
  rename (fn1, fn2);

  // rename DB__
  sprintf(fn1,"%sDB__%s.lst",AP_get_tmp_dir(),safMnAct);
  sprintf(fn2,"%sDB__%s.lst",AP_get_tmp_dir(),mNam);
  rename (fn1, fn2);


  // create new empty model for active-model
  sprintf(s1, "# model %s\n", safMnAct);
  MDL_init_noCad (safMnAct, s1);

  // add new-model to browser
  Brw_Mod_add (mNam, ICO_natM0);

  // must load new file into memspc (for following MDL_sav_tmp ())
  // sprintf(fn1,"%sModel_%s",AP_get_tmp_dir(),safMnAct);
  // UTF_add_fil_0 (fn1);
  UTF_clear_ ();
  sprintf(s1, "# %s",OS_date1());
  UTF_add_line (s1);
  sprintf(s1, "# model %s", safMnAct);
  UTF_add_line (s1);

  // re-activate modified active-model
  irc = MDL_load_mNam__ (safMnAct, 1);


  L_exit:
      // MDL_dump__ ("MDL_load_mdl_main");
      // printf(" ex-MDL_load_mdl_main %d\n",irc);
    return 0;

}


//================================================================
  int MDL_load_mdl_f (char *fnam) {
//================================================================
// MDL_load_mdl_f          create subModel from load file
// Input:
//   fnam      full name of model; NULL = get name from user
// was Mod_LoadFile__ Mod_LoadSubmodel

  int       irc, imn;
  char      s1[320], mNam[SIZMFTot], *pmn;
  stru_FN   ofn;


  // printf("MDL_load_mdl_f |%s|\n",fnam);


  if(!fnam) {
    // get new filename (and directory) from user
    irc = UI_file_open__ (s1, sizeof(s1));
    if(irc < 0) return 0;  // error or cancel

  } else {
    strcpy(s1, fnam);
  }


  // get filenameobj from filename
  irc = MDLFN_oFn_fNam (&ofn, s1);
  if(irc < 0) goto L_exit;

  // add new name uniq to mdl_names, mdl_tab;
  MDLFN_syFn_oFn (mNam, &ofn);
  imn = MDL_mdl_add (ofn.iTyp, mNam);

  // import model
  irc = MDL_load_1 ();

  // make modelname internal (loaded from external file)
  pmn = UtxTab__(imn, &mdl_names);
  UTX_safeName (pmn, 1);
  MDL_MTYP_SET_INAM (imn, MBTYP_INTERN);

  // update mUsed in mdl_tab
  MDL_used_set__ ();

  // update mUsed and Brw-Icons of models
  MDL_brw_upd ();


  L_exit:
    // printf(" ex-MDL_load_mdl_f %d\n",irc);

  return irc;

}


//================================================================
  int MDL_load_mdl_grp (char *mNam, char *fNam) {
//================================================================
// MDL_load_mdl_grp         create subModel from group
// Input:
// Output:
//   retCode      0=OK, -1=err.
// was Mod_SM_add_file
// see also Grp_Mdl MDL_load_mdl_new


  int     irc, inm;
  char    s1[320];

  // printf("MDL_load_mdl_grp |%s|%s|\n",mNam,fNam);


  // rename /<tmpDir>/selection1.gcad /<tmpDir>/"Model_<subModelname>"
  sprintf(s1, "%sModel_%s", AP_get_tmp_dir(), mNam);
    // printf(" Mod_SM_add_file |%s|%s|\n",fNam,s1);
  OS_file_copy (fNam, s1);


  //----------------------------------------------------------------
  // add new model to mdl_names, mdl_tab
  inm = MDL_mdl_add (MBTYP_INTERN, mNam);
  if(inm < 0) {TX_Print("***** MDL_load_mdl_grp E1 %s",mNam); irc = -1; goto L_exit;}

  // set mUsed to unused
  MDL_USED_SET_INAM (inm, mdlStat_unused);


  //----------------------------------------------------------------
  // add new model to browser
  Brw_Mod_add (mNam, ICO_natM0);

  TX_Print("**** subModel %s created ..",mNam);

  irc = 0;


  L_exit:
    // printf("ex-MDL_load_mdl_grp %d\n",irc);
  return irc;

}


//================================================================
  int MDL_load_mNam_tmp (char *symNam, int inm) {
//================================================================
// MDL_load_mNam_tmp           activate temp (only mockup; no display)
// if mockup is to delete, activate this mockup temp to avoid this:
// - after remove-process reactivate main -
// - MDL_sav_tmp overwrites modelcode that has been modified by remove-process.


  int      ibm;


  // printf("MDL_load_mNam_tmp %s\n",symNam);

  // unload active model,
  MDL_sav_tmp ();

  // update list of submodels tmp/Mod_<mNam>.lst (scan active primary-model)
  MDL_load_scan_prim ();

  // index-baseModel
  ibm = MDL_IMB_INAM (inm);

  //----------------------------------------------------------------
  // activate main or subModel -
  AP_modact_inm = inm;             // main = primary model
  AP_modact_ibm = ibm;             // index baseModel of active primary model
  AP_modact_tmp = 0;

  if(!inm) {
    // main
    AP_modact_nam[0] = '\0';

  } else {
    strcpy(AP_modact_nam, symNam);
  }
    // printf(" _mNam_tmp-set-AP_modact_nam |%s|\n",AP_modact_nam);


  //----------------------------------------------------------------
  L_exit:
    // MDL_dump__ ("ex-MDL_load_mNam_tmp");
    // printf("ex-MDL_load_mNam_tmp %d\n",0);

  return 0;

}


//================================================================
  int MDL_load_mNam__ (char *mNam, int typMnm) {
//================================================================
// MDL_load_mNam__           activate main- or internal model
// Input:
//   symNam     symbolic-modelname, main- or internal model
//     typMnm   0 mNam is symbolic-name; eg "Data/Niet.gcad"
//              1 mNam is safe-name -    eg "Data_Niet_gcad"
// Output:
//   retCode    0=OK, -1=err.
//
// was Mod_chg_CB MDLMP_activ


  int       irc, i1, inm, mTyp;
  char      symNam[SIZMFTot], safNam[SIZMFTot], s1[256], fn1[SIZFNam], *p1;
  stru_tab  *tab;


  // printf("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM \n");
  // printf("MDL_load_mNam__ |%s| %d\n",mNam,typMnm);
  // printf("  AP_modact_nam=|%s|\n",AP_modact_nam);
  // MDL_dump__ ("MDL_load_mNam__");
  // UtxTab_dump (&mdl_names, "MDL_load_mNam__");


  //================================================================
  // get mNam (symbolic) and safNam for new primary-model and
  // inm (index mdl_tab) and mTyp for new primary-model

  // test main ("");
  if(!mNam[0]) {
    inm = 0;
    mNam = MDL_MNAM_INAM (0);
    typMnm = 0;
    mTyp = MBTYP_INTERN;
    strcpy(symNam, mNam);
    strcpy(safNam, mNam);
    UTX_safeName (safNam, 1);
    goto L_saf_sym_ok;
  }

  if(typMnm) {
    // mNam = safe-name
    // get symbolic-name and name-index from safe-name  
    inm = MDL_imn_mNam__sNam (symNam, mNam);
    if(inm < 0) {TX_Error("MDL_load_mNam__ E1 %s",mNam); irc = -1; goto L_exit;}
    strcpy(safNam, mNam);

  } else {
    // mNam = symbolic-name
    strcpy(symNam, mNam);
    strcpy(safNam, mNam);
    UTX_safeName (safNam, 1);
    // get inm
    inm = MDL_INAM_MNAM (symNam);
  }

  // get mTyp
  mTyp = MDL_MTYP_INAM(inm);


  //----------------------------------------------------------------
  L_saf_sym_ok:
    // symNam, safNam, inm, mTyp OK.
      // printf(" _load_mNam sym|%s| saf|%s| inm=%d mTyp=%d\n",
             // symNam,safNam,inm,mTyp);

/*
    // do not edit extern-models -
    if(inm) {
      // its not main ..
      if(mTyp != MBTYP_INTERN) {
        // extern model - ask user: edit (load) or rename (make intern model)
        sprintf(s1, " %s is extern model -","abc");
        i1 = GUI_dlg_bt23 (s1,
                           "edit (load extern model)",
                           "rename (change into intern model)",
                           NULL);
        if(i1 < 0) { irc = 0; goto L_exit;}     // cancel by user
        if(i1 < 1) {
          // 0 = load extern model
          // save
          MDL_sav_ck__ (0);
          // get full mdlNam
          irc = MDLFN_ffNam_fNam (fn1, symNam);
          // load
          irc = MDL_load_main (fn1);
          goto L_exit;
        }
        // 1 = rename (change extern-model into intern-model)
return MSG_ERR__ (ERR_TEST, "MDL_load_mNam__ TODO2");

      }
    }
*/


  //================================================================
  // unload active model,
  MDL_sav_tmp ();
 
  // update list of submodels tmp/Mod_<mNam>.lst (scan active primary-model)
  MDL_load_scan_prim ();



  //================================================================
  // have now symNam safNam inm mTyp for new primary-model.

  // skip mockup-models (obj, wrl, stl)
  if(MDL_MTYP_IS_MOCK(mTyp)) {
    TX_Print("***** %s is mockup-model; cannot modify ..",symNam);
    irc = 0;
    goto L_exit;
  }

//   // if(inm == AP_modact_inm) {TX_Print("***** model already active ..."); return 0;}

  //----------------------------------------------------------------
  // test unused-model
  if(MDL_STAT_INAM(inm) == -1) {
    // unused-model -
    // set mStat = 0;  - no baseMdl exists ..
    MDL_STAT_SET_INAM (inm, 0);
    // scan and create baseModel
    MDL_load_1 ();
  }


  //----------------------------------------------------------------
  // activate main or subModel -
  AP_modact_inm = inm;             // main = primary model
  AP_modact_ibm = MDL_BMI_ACT;     // index baseModel of active primary model
  AP_modact_tmp = 0;

  if(!inm) {
    // main
    AP_modact_nam[0] = '\0';

  } else {
    strcpy(AP_modact_nam, symNam);
  }
    // printf(" _activ-set-AP_modact_nam |%s|\n",AP_modact_nam);


  //----------------------------------------------------------------
  // clear loadmap mdl_ldm
  MDL_LDM_CLEAR ();

  // clear dispList and reset mdl_tab and baseModels
  GL_Init1 ();
  MDL_mdr_clear (-1L);   // clear all refModels
  MDL_mdb_clear__ ();    // reset mdl_tab and all baseModels
  // DB_del_ModBas (-1L);    // reset mdl_tab and all baseModels

  // clear PRCV_GRP, delete all PRCV-points
  PRCV_DB_all_free(1);

  // clear Undo-List
  UNDO_clear ();

  // clear Hide-List
  GA_hide__ (-1, 0L, 0);

  ED_Init ();
  ED_Reset ();  // ED_lnr_act = 0;

  // CAD: clear inputfields
  IE_cad_ResetInputs ();


   
  //----------------------------------------------------------------
  // init and work modelfile (was ED_new_File)

  // get file mainModel into memory (find, work, remove DYNAMIC_DATA-block)
  UTF_clear_ ();
  sprintf(fn1, "%sModel_%s", AP_get_tmp_dir(), safNam);
  irc = UTF_add_fil_init (fn1);

  // RUN (abarbeiten)
  ED_work_END (0);

  // im CAD und MAN-Modus:
  if(UI_InpMode != UI_MODE_VWR) ED_enter ();
  

  //----------------------------------------------------------------
  // update mUsed in mdl_tab
  MDL_used_set__ ();

  // update mUsed and Brw-Icons of models
  MDL_brw_upd ();

  // disp submodelname im Titlebar
  UI_title_set ();


  // message "----- <mdlTyp> <mdlNam> is active .."
  if(AP_modact_inm) {
    p1 = MDL_typInf(mTyp);
  } else {
    p1 = "main-model";
  }
  TX_Print("----- %s %s is active ..",p1,MDL_mNam_prim());


//   // activate model in browser
//   if(!inm) {
//     // activate main
//     Brw_Mdl_activ (NULL);
//   } else {
//     Brw_Mdl_activ (safNam);
//   }


  //----------------------------------------------------------------
  L_exit:
    // MDL_dump__ ("ex-MDL_load_mNam__");
    // printf("ex-MDL_load_mNam__ %d\n",irc);
  return irc;

}


//==================================================================
  int MDL_load_dyn (int mTyp, char *mNam, int typMnm, int objStat) {
//==================================================================
// MDL_load_dyn            add new model, temp or perm, add later
// Input:
//   mTyp       MBTYP_INTERN|MBTYP_EXTERN|MBTYP_CATALOG ..
//   mNam       symbolic- or safe-modelname, main- or internal model
//   typMnm     0 mNam is symbolic-name; eg "Data/Niet.gcad"
//              1 mNam is safe-name -    eg "Data_Niet_gcad"
//   objStat    OBJSTAT_perm (0) | OBJSTAT_temp (1)
// Output:
//   retCode    0=OK, -1=err.
//
// Only primary model can activate temp-models


  int       irc, ibm, inm, os, lNr;
  char      *mnPrim, safPrim[SIZMFTot];


  // printf("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD \n");
  // printf("MDL_load_dyn mTyp=%d |%s| %d objStat=%d\n",mTyp,mNam,typMnm,objStat);
  // printf("  AP_modact_nam=|%s|\n",AP_modact_nam);
  // MDL_dump__ ("MDL_load_dyn");
  // UtxTab_dump (&mdl_names, "MDL_load_dyn");


  if(UI_InpMode == UI_MODE_CAD) {
    // remove existing tempModel (different modelName)
    if(AP_modact_tmp) MDL_prev_stat_del ();
    // activate temp-mode in MDL (for later removal of temp-model)
    if(objStat == OBJSTAT_temp) MDL_prev_stat_set ();
  }


  // clear loadmap
  MDL_LDM_CLEAR();

  // get mNam of primary model
  mnPrim = MDL_mNam_prim ();
  strcpy(safPrim, mnPrim);
  UTX_safeName (safPrim, 1);

  // store active baseModelIndex
  ibm = AP_modact_ibm;
  inm = AP_modact_inm;
  os  = APT_obj_stat;
  lNr = APT_lNr;

  // save primary model
  DB_save__  (safPrim);

  // clear UTF_FilBuf0 (memspc)
  UTF_clear_ ();

  // load new model into DL from file
  irc = MDL_load_mdl__ (mTyp, mNam, typMnm, objStat);

  // restore primary model
  DB_load__ (safPrim);

  // reset AP_modact_ibm
  AP_modact_ibm = ibm;
  AP_modact_inm = inm;
  APT_obj_stat  = os;
  APT_lNr       = lNr;


  // set AP_modact_tmp for resetting to perm in APT_work_def
  if(UI_InpMode == UI_MODE_CAD) {
    if(objStat == OBJSTAT_temp) AP_modact_tmp = 1;
  }

      // MDL_dump__ ("ex-MDL_load_dyn");

  return irc;

}


//================================================================
  int MDL_reset (int mode) {
//================================================================
// MDL_reset               reset loadmap and mdl_tab
// Input:
//   mode          0  clear mdl_tab, mdlnames, mdl_ldm
//                 1  reset all baseModel from status loaded to status imported


  int      i1, mbNr;
  ModelBas *mba, *mb1;
  stru_tab *tab;


  // printf("MDL_reset %d\n",mode);


  if(mode == 0) {
    UtxTab_init__ (&mdl_names);
    mdl_tab.rNr = 0;
    MDL_LDM_CLEAR ();   // clear loadmap
    return 0;
  }


  // reset status mdl_tab from 2 > 1
  tab = MEMTAB_DAT (&mdl_tab);
  DB_get_mdb (&mbNr, &mba);
  for(i1=0; i1<mdl_tab.rNr; ++i1) {
    if(tab[i1].mStat == 2) {
      tab[i1].mStat = 1;
      // set baseModel.DLsiz -1
      mb1 = &mba[tab[i1].mbi];
      mb1->DLind = -1L;
      mb1->DLsiz = -1;
    }
  }


  // clear loadmap
  MDL_LDM_CLEAR ();


  return 0;

}

//================================================================
  int MDL_mdl_tab_set_main () {
//================================================================
// MDL_mdl_tab_set_main    reset mdl_tab to main (for shutdown)

  int     ii;

  // printf("MDL_mdl_tab_set_main \n");

  AP_modact_inm = 0;
  for(ii=1; ii<mdl_tab.rNr; ++ii) {
    if(mdl_tab.data[ii].mStat == 1) mdl_tab.data[ii].mStat = 2;
  }

  return 0;

}


//====================================================================
  int MDL_clean_f () {
//====================================================================
/// - delete all tmp/Model_*
/// - delete all tmp/*.tess
/// must NOT kill MdlLst.txt (list of models last used)
/// see also OS_file_delete OS_file_delGrp

  char  cbuf[256];


  // printf("MDL_clean_f \n");

  // sprintf(cbuf,"%sMod.lst",AP_get_tmp_dir());
  // OS_file_delete (cbuf);

  sprintf(cbuf,"%sMod_*",AP_get_tmp_dir());
  OS_file_delGrp (cbuf);

  sprintf(cbuf, "%sModel_*",AP_get_tmp_dir());
  OS_file_delGrp (cbuf);

//   sprintf(cbuf,"%sDB__*.dat",AP_get_tmp_dir());
  // DB__*.dat  and catalog-data *.dat
  sprintf(cbuf,"%s*.dat",AP_get_tmp_dir());
  OS_file_delGrp (cbuf);

  sprintf(cbuf,"%sjoints",AP_get_tmp_dir());
  OS_file_delete (cbuf);

  // del process_* and processes.lst
  sprintf(cbuf, "%sprocess*",AP_get_tmp_dir());
  OS_file_delGrp (cbuf);

  sprintf(cbuf, "%s*.tess",AP_get_tmp_dir());
  OS_file_delGrp (cbuf);

  // catalog-parameterfiles
  CTLG_dat_del ();

  sprintf(cbuf, "%s*.ptab",AP_get_tmp_dir());
  OS_file_delGrp (cbuf);

  sprintf(cbuf, "%s*.msh",AP_get_tmp_dir());
  OS_file_delGrp (cbuf);

  sprintf(cbuf, "%s*.dlg",AP_get_tmp_dir());
  OS_file_delGrp (cbuf);

  sprintf(cbuf, "%s*.appdat",AP_get_tmp_dir());
  OS_file_delGrp (cbuf);

  sprintf(cbuf, "%s*.bmp",AP_get_tmp_dir());
  OS_file_delGrp (cbuf);

  Tex_DelAll ();  // delete all OpenGL-textures

    // TESTBLOCK
    // DEB_exit();
    // END TESTBLOCK

  return 0;

}


//==================================================================
  int MDL_load_mdl__ (int mTyp, char *mNam, int typMnm, int objStat) {
//==================================================================
// MDL_load_mdl__           add new model, temp or perm.
// - import, scan (MDL_load_1) and create loadmap, load into DL (MDL_load_2)
// Input:
//   mTyp       MBTYP_INTERN|MBTYP_EXTERN|MBTYP_CATALOG ..
//   mNam       symbolic- or safe-modelname, main- or internal model
//   typMnm     0 mNam is symbolic-name; eg "Data/Niet.gcad"
//              1 mNam is safe-name -    eg "Data_Niet_gcad"
//   objStat    OBJSTAT_perm (0) | OBJSTAT_temp (1)
// Output:
//   retCode    0=OK, -1=err.
//
// was Mod_chg_CB MDLMP_activ


  int       irc, i1, imn;
  char      symNam[SIZMFTot], safNam[SIZMFTot], fn1[320], *p1;
  // stru_tab  *tab;


  // printf("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM \n");
  // printf("MDL_load_mdl__ mTyp=%d |%s| %d objStat=%d\n",mTyp,mNam,typMnm,objStat);
  // printf("  AP_modact_nam=|%s|\n",AP_modact_nam);
  // MDL_dump__ ("MDL_load_mdl__");


  //----------------------------------------------------------------
  // fix symNam, safNam,
  if(!typMnm) {
    // mNam = symbolic-name
    strcpy(symNam, mNam);
    MDL_safename (safNam, symNam);

  } else {
    // mNam = safe-name
    MDL_imn_mNam__sNam (symNam, mNam);
    strcpy(safNam, mNam);
  }
    // printf(" _load_mdl-L1 |%s|%s|\n",symNam,safNam);


  //----------------------------------------------------------------
  // find symNam in mdl_names; add if not yet defined.
  imn = MDL_INAM_MNAM (symNam);
    // printf(" _load_mdl imn=%d\n",imn);

  // set index name (mdl_names)
  AP_modact_inm = imn;

  if(imn >= 0) goto L_load;

  // add new name uniq to mdl_names, mdl_tab;
  imn = MDL_mdl_add (mTyp, symNam);

    // TESTBLOCK
    // MDL_dump__ ("MDL_load_mdl__-L2");
    // END TESTBLOCK


  //----------------------------------------------------------------
  // import, create tmp/Mod_<sm>.lst, add to loadmap (recu)
  L_load:

//     // add model to loadMap
//     irc = MDL_ldm_add__ (imn);
//     if(irc < 0) {TX_Print("***** MDL_load_mdl__ E2 %s",mNam); goto L_exit;}

    // import, scan
    irc = MDL_load_1 ();
    if(irc < 0) goto L_exit;

    // create loadmap, load
    irc = MDL_load_2 (imn);


  //----------------------------------------------------------------
  L_exit:
    // MDL_dump__ ("ex-MDL_load_mdl__");
    // printf("ex-MDL_load_mdl__ %d\n",irc);
  return irc;

}


//================================================================
  int MDL_load_lst () {
//================================================================
// MDL_load_lst            let user select internal model from list, activate
// Input:
// Output:
//   retCode      0=OK, -1=err.
//
// was Mod_chg__ Mod_flst_all Mod_chg_CB


  int     irc;
  char    s1[256], fNam[256];

  // printf("MDL_load_lst \n");

    MDL_file_lst__ (0); // make list of all Submodels -> <baseDir>/tmp/Mod.lst

    // let user select from list of Submodelnames
    sprintf(s1,"%sMod.lst",AP_get_tmp_dir());
    irc = GUI_listf1__ (fNam,sizeof(fNam),s1,"- select model -","x40,y40");
    if(irc < 0) return -1;

//   // save the active Submodel AP_modact_nam -> TempFile
//   if(mode == 0) Mod_sav_tmp ();

//   // change subModel
//   Mod_chg_CB (s1);


  L_exit:
    // printf("ex-MDL_load_lst \n");
  return irc;

}


//================================================================
  int MDL_load_new__ () {
//================================================================
// MDL_load_new__             clear all, init Model_unknown
// was Mod_init__ Mod_kill__ 
 

  int     irc;
  char    s1[SIZFNam], sfn[320];

  // printf("MDL_load_new__ \n");


  DB_del_Mod__ ();     // delete all basic- and refModels

  strcpy(AP_modact_nam, "unknown");

  // keep AP_mod_sym
  strcpy(AP_mod_fnam, "unknown");
  strcpy(AP_mod_ftyp, "gcad");
  AP_mod_iftyp = MBTYP_EXTERN;

  // get sfn = symbolic modelname "<symDir>/unknown.gcad"
  MDLFN_syFn_AP (sfn);

  // create tmp/Model_unknown
  // init new mainModel with add. line s1
  sprintf(s1, "# model %s\n", AP_modact_nam);
  MDL_init_noCad (sfn, s1);

  AP_modact_inm = 0;  // main = primary
  AP_modact_ibm = MDL_BMI_ACT;

  // init mdl_names
  // add <symDir>/unknown.gcad as mainName into mdl_names[0]
  MDL_init_MDL (sfn, MBTYP_EXTERN);

  // set mStat to 1 = imported (else later imports a stored "unknown.gcad")
  MDL_STAT_SET_INAM (0, 1);


  // load mainModel into memSpc (UTF_FilBuf0)
  UTX_safeName (sfn, 1);
  sprintf(s1, "%sModel_%s",AP_get_tmp_dir(), sfn);
  irc = UTF_add_fil_init (s1);


  L_exit:
    // MDL_dump__ ("ex-MDL_load_new__");
    // printf("ex-MDL_load_new__ %d\n",irc);
  return irc;

}


//================================================================
  int MDL_init_MDL (char *namMain, int mTyp) {
//================================================================
// MDL_init_MDL             clear mdNames mdl_ldm mdl_nam_tmp
// Input:
// Output:
//   retCode      0=OK, -1=err.
// was ..


  int     irc;
  long    l1;

  // printf("MDL_init_MDL |%s| %d\n",namMain,mTyp);


  if(UtxTab_IS_NEW(&mdl_names))  {
    UtxTab_free (&mdl_names);
    MemTab_free (&mdl_tab); 
  }

  UtxTab_init__ (&mdl_names);

  // name[0] = namMain
  UtxTab_add (&mdl_names, namMain);

  // reset mdl_tab
  MemTab_ini__ (&mdl_tab, sizeof(stru_tab), 0, 500);

  // mdl_tab[0] = main
  MemTab_add (&mdl_tab, &l1, &_TAB_NUL, 1, 0);
  mdl_tab.data[0].mTyp = mTyp;


  // reset loadmap
  MemTab_ini__ (&mdl_ldm, sizeof(int), 0, 500);

//   // clear temp. modelname
//   mdl_nam_tmp[0] = '\0';


  L_exit:
    // printf("ex-MDL_init_MDL \n");
  return 0;

}


//================================================================
  int MDL_init_noCad (char *mmNam, char *src) {
//================================================================
// MDL_init_noCad             init new mainModel with add. text;
//   - create file tmp/Model_<mNam>"
//   - make a copy -> Mod_.mod_in
//   write only "# date/time\n" and src ..
//   used for image or mockup only (no :DYNAMIC_DATA)
// Input:
//   mmNam      name mainmodel
//   src        additional src; can be NULL
//
// was AP_src_init AP_mdl_init


  char fn[320], s1[320], safNam[SIZMFTot];
  FILE *fpo;

  // printf("MDL_init_noCad |%s|%s|\n",mmNam,src);

  // make mnam safe
  strcpy (safNam, mmNam);
  UTX_safeName (safNam, 1);

  // sprintf(s1, "%sMod_Empty.dat", AP_get_tmp_dir());
  sprintf(fn, "%sModel_%s", AP_get_tmp_dir(),safNam);

  if(!src) return -1;

  // // add src to file
  // UTX_wrf_app_str (fn, src);


  if((fpo=fopen(fn,"w")) == NULL) {
    TX_Print("MDL_init_noCad E001 |%s|",fn);
    return -1;
  }

  // MODSIZ .. 
  DL_wri_dynDat1 (fpo, 1);

  fprintf(fpo,":DYNAMIC_DATA\n");
  fprintf(fpo,"# %s\n",OS_date1());
  fprintf(fpo, "%s", src);

  fclose(fpo);

  // copy new model -> tmp/Mod_.mod_in
  sprintf(s1, "%sMod_.mod_in", AP_get_tmp_dir());
  OS_file_copy (fn, s1);

  return 0;

}


//================================================================
  int MDL_sav__ (char *fnOut, int mode, int uuSav) {
//================================================================
// MDL_sav__               save model native
// Input:
//   fnOut        outfilename absolute
//   mode         0  save|export main; set AP_mod_*;
//                2  save|export active model;
//   uuSav        0  = save also unused-models
//                1  = do NOT save unused-models
//   AP_stat.zip  global, compress (zip) or not
// Output:
//   retCode      0=OK, -1=err.
//
// was Mod_sav__


  int     irc;
  char    fnTmp[320], fn1[320];


  // printf("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS \n");
  printf("MDL_sav__ |%s| %d AP_stat.zip=%d\n",fnOut,uuSav,AP_stat.zip);


  // save Model+Submodels into tempDir as tmp/Mod_.mod_out native
  irc = MDL_sav_gcad (mode, uuSav);
  if(irc < 0) return irc;


  TX_Print("Save File %s",fnOut);


  //----------------------------------------------------------------
  // copy out

  sprintf(fnTmp, "%sMod_.mod_out", AP_get_tmp_dir());

  // compress ?
  if(AP_stat.zip) {
    // compress, native.
    // change filetyp "gcad" -> "gcaz"
    strcpy(fn1, fnOut);
    fn1[strlen(fn1) - 1] = 'z';
    OS_file_zip (0, fnTmp, fn1);


  } else {
    // copy tempFile -> outfile
    OS_file_copy (fnTmp, fnOut);
  }


  L_exit:
    // printf("ex-MDL_sav__ %d\n",irc);
  return irc;

}


//================================================================
  int MDL_sav_gcad (int mode, int uuSav) {
//================================================================
// MDL_sav_gcad           save primary model native as file tmp/Mod_.mod_out
// - save model <AP_mod_fnam> (mainmodel) + internal-submodels -
// - into file tmp/Mod_.mod_out
// Input:
//   mode   0  save|export primary-model; set AP_mod_*;
//          2  save|export active model;                    UNUSED
//   uuSav  0  = save also unused-models
//          1  = do NOT save unused-models
//
// was Mod_sav__ Mod_sav_i


  int    irc, i1, ii, inm, mtyp, mstat;
  long   l1;
  char   cbuf[320], fnam0[SIZFNam], fnam1[SIZFNam], s1[320], symNam[SIZMFTot],
         safPrim[SIZMFTot], *p1;
  FILE   *fp1, *fpo;
  // Memspc mSpc1;
  // void  *vp1;

  UtxTab_NEW (sTab1);                 // processes

  MemTab(Point) pTab = _MEMTAB_NUL;
  MemTab(Fac3) fTab = _MEMTAB_NUL;
  MemTab(IntTab) eTab = _MEMTAB_NUL;
  MemTab(int) eDat = _MEMTAB_NUL;


  // printf("__________________________________________________________ \n");
  // printf("MDL_sav_gcad %d %d\n",mode,uuSav);
  // if(mode) printf("  AP_modact_nam=|%s|\n",AP_modact_nam);    // sm
  // else     printf("  AP_mod_fnam=|%s|\n",AP_mod_fnam);        // main
  // MDL_dump__ ("MDL_sav_gcad");


  // update list of used submodels         2022-06-01
  MDL_lst_sm_upd ();


  irc = -1;

//   UI_block__ (1, 1, 1);  // block all


  // if primary is not main it must be a intern-model
  if(AP_modact_inm) {
    // must be intern-model ..
    if(MDL_MTYP_INAM(AP_modact_inm) != MBTYP_INTERN) {
      TX_Print("***** Error MDL_sav_gcad - cannot save model if not main");
      irc = -1;
      goto L_exit;
    }
  }

//   // fix box if necessary
//   if(mode >= 0) {
//     // model not empty
//     if(AP_mdlbox_invalid_ck()) {
//       // get box of active model
//       UT3D_box_mdl__ (&AP_box_pm1, &AP_box_pm2, -1, 0);
//       AP_mdlbox_invalid_reset ();  // set box valid
//     }
//   } else {
//     // model is empty;
//     AP_mdlbox_invalid_set (); // set box invaild
//   }
// 
// 
//   // save the active sub- or mainmodel AP_modact_nam -> TempFile
//   // if(!mode) Mod_sav_tmp ();
//   if(mode <= 0) MDL_sav_tmp ();
// 
// 
//   // get lists of all subModels, PTAB-Surfs and MSH-Surfs of subModelfiles
//   fnam0[0] = '\0';    // main = ""
//   irc = Mod_fget_names__ (&mdlTab, &surPtab, &surMsh, fnam0);
//   if(irc < 0) {
//       // printf("ex Mod_sav__ -1\n");
// //     UI_block__ (0, 0, 0);  // unblock all
//     irc = -1;
//     goto L_err1;
//   }


  // set temp.outfilename <tmp>/Mod_.mod_out
  sprintf(fnam0,"%sMod_.mod_out",AP_get_tmp_dir());


  // try to open outfile
  if((fpo=fopen(fnam0,"w")) == NULL) {
    TX_Print("***** MDL_sav_gcad E001 %s",fnam0);
    goto L_err1;
  }

  // get safe modelname of primary model
  MDL_safNam_prim (safPrim);


  //================================================================
  // add all used Submodels to file fnam0

  // get mdlTab = list of all existing subModels (unused models already checked)
  UtxTab_init__ (&sTab1);
  irc = MDL_lst_all (&sTab1);
  if(irc < 0) goto L_err1;
  if(sTab1.iNr < 1) goto L_PTAB;

  for(ii = 0; ii < sTab1.iNr; ++ii) {
    p1 = UtxTab__ (ii, &sTab1);      // get word from index
      // printf(" _sav_gcad-sm1-%d |%s| .....................\n",ii,p1);

    // get mtyp and stat
    inm = MDL_imn_mNam__sNam (symNam, &p1[6]);  // skip "Model_"
    if(inm < 0) {TX_Print("***** MDL_sav_gcad E-symNam %s",p1); continue;}
    mtyp = MDL_MTYP_INAM (inm);
    if(mtyp != MBTYP_INTERN) continue;
    mstat = MDL_STAT_INAM (inm);
      // printf(" _sav_gcad-sm2-%d inm=%d %s mtyp=%d mstat=%d\n",
             // ii, inm, p1,mtyp,mstat);

    // save unused submodels
    if(!uuSav) {
      // save also unused-models
      if(mstat < 1) goto L_sm_out;   // stat=0: unused model
    }

    // test if model = primary model (skip it - write out later as main)
    if(inm == AP_modact_inm) continue;

    // ignore unused subModel
    if(mstat != 2) continue;

    // check if submodel belongs to intern or extern parent-model
    irc = MDL_sav_ck_sm__ (inm);
    if(irc < 0) continue;


    // check if modelfile exists
    L_sm_out:
      sprintf(fnam1,"%s%s",AP_get_tmp_dir(),p1);
      if(OS_checkFilExist(fnam1, 1) == 0) {
        TX_Print("***** MDL_sav_gcad submodel %s not found ..",p1);
        continue;
      }

      // cat file tmp/Model_<mdlnam>
      fprintf(fpo, "SECTION MODEL %s\n",&p1[6]);
      if(UTX_cat_file (fpo, fnam1) < 0) {
        TX_Print("***** MDL_sav_gcad E002 %s",p1);
        goto L_err1;
      }
      fprintf(fpo,"%s\n",sSecEnd);  // SECTIONEND

  }

  //================================================================
  // add PTAB's
  L_PTAB:
// TODO: skip objects not belonging to model if mode==2

  UtxTab_init__ (&sTab1);
  irc = MDL_lst_ptab (&sTab1);
  if(irc < 0) goto L_err1;


  if(sTab1.iNr < 1) goto L_MSH;

  MemTab_ini__ (&pTab, sizeof(Point), Typ_PT, 10000);

  for(ii = 0; ii < sTab1.iNr; ++ii) {
    p1 = UtxTab__ (ii, &sTab1);      // get word from index

    sprintf(fnam1,"%s%s",AP_get_tmp_dir(),p1);
      // printf(" PTAB[%d] |%s|%s|\n",ii,p1,fnam1);

    MSH_bload_pTabf (&pTab, fnam1);

    // save ptab ascii
    UTX_cp_left (s1, p1, strlen(p1) - 5);   // cut off ".ptab"
    MSH_asav_pTabf (pTab.data, pTab.rNr, s1, fpo);
  }
  MemTab_free (&pTab);


  //================================================================
  // add MSH's
  L_MSH:
// TODO: skip objects not belonging to model if mode==2

  UtxTab_init__ (&sTab1);
  irc = MDL_lst_msh (&sTab1);
  if(irc < 0) goto L_err1;

  if(sTab1.iNr < 1) goto L_procs;

  MemTab_ini__ (&fTab, sizeof(Fac3), Typ_Fac3, 10000);
  MemTab_ini__ (&eTab, sizeof(IntTab), Typ_IntTab, 10);
  MemTab_ini__ (&eDat, sizeof(int), Typ_Int4, 50);

  for(ii = 0; ii < sTab1.iNr; ++ii) {
    p1 = UtxTab__ (ii, &sTab1);      // get word from index

    sprintf(fnam1,"%s%s",AP_get_tmp_dir(),p1);
      // printf(" MSH[%d] |%s|%s|\n",ii,p1,fnam1);
    MSH_bload_fTabf (&fTab, &eTab, &eDat, fnam1);

    // save mesh ascii
    UTX_cp_left (s1, p1, strlen(p1) - 4);   // cut off ".msh"
    MSH_asav_fTabf (&fTab, &eTab, &eDat, s1, fpo);

  }

  MemTab_free (&eDat);
  MemTab_free (&eTab);
  MemTab_free (&fTab);



  //================================================================
  // add processes
  L_procs:
  i1 = PRC_lst_processes ();
  if(i1 < 1) goto L_appDat;
// TODO: skip objects not belonging to model if mode==2

  // get list of all processes
  UtxTab_init__ (&sTab1);

  // add file to stringTab
  sprintf(fnam1, "%sprocesses.lst",AP_get_tmp_dir());
  i1 = UtxTab_add_file (&sTab1, fnam1);
  if(i1 < 0) goto L_appDat;
    // UtxTab_dump (&sTab1);

  // loop tru sTab1; cat file tmp/Model_<mdlnam>
  for(i1=0; i1 < UtxTab_nr(&sTab1); ++i1) {
    fprintf(fpo, "SECTION PROCESS %s\n",UtxTab__(i1, &sTab1));
    sprintf(fnam1, "%s%s",AP_get_tmp_dir(),UtxTab__(i1, &sTab1));
    if(UTX_cat_file (fpo, fnam1) < 0) {
      TX_Print("***** MDL_sav_gcad E-prc-1 %s",fnam1);
    }
    fprintf(fpo, "%s\n",sSecEnd);
  }


  //================================================================
  // add ApplicationData
  L_appDat:
// TODO: skip objects not belonging to model if mode==2

  // get a list of files "<temp>/*.appdat" -> sTab
  sprintf(cbuf, "%slst.dat", AP_get_tmp_dir());
  ii = UTX_dir_listf (cbuf, AP_get_tmp_dir(), NULL, ".appdat", 0);
  if(ii < 1) goto L_main;
    // printf("add -appDat-\n");

  if((fp1=fopen(cbuf,"r")) == NULL) {
    TX_Print("***** MDL_sav_gcad E008 %s",cbuf);
    goto L_main;
  }

  while (!feof (fp1)) {
    if (fgets (cbuf, 250, fp1) == NULL) break;
    UTX_CleanCR (cbuf);
    UTX_ftyp_cut (cbuf);      // remove filetype

    // skip file if size = 0
    sprintf(fnam1,"%s%s.appdat",AP_get_tmp_dir(),cbuf);
    l1 = OS_FilSiz (fnam1);
    if(l1 < 3) continue;

    // cat file tmp/<appnam>.appdat
    fprintf(fpo, "SECTION APPDAT %s %ld\n",cbuf,l1);
    if(UTX_cat_file (fpo, fnam1) < 0) {
      TX_Print("***** MDL_sav_gcad E007 %s",fnam1);
      irc = -1;
      goto L_err1;
    }

    // fprintf(fpo,"%s\n",sSecEnd);
    fprintf(fpo,"\n%s\n",sSecEnd);
  }


  //================================================================
  // add primary model
  L_main:
      // printf(" sav_gcad-L_main:\n");
    MDL_safNam_prim (cbuf);
    sprintf(fnam1,"%sModel_%s",AP_get_tmp_dir(),cbuf);
    if(UTX_cat_file (fpo, fnam1) < 0) {
      TX_Print("***** MDL_sav_gcad E006 %s",fnam1);
      goto L_err1;
    }

    irc = 0;
    UtxTab_free (&sTab1);               // free mem
    if(fpo) {fclose(fpo); fpo = NULL;}


  //================================================================
  L_exit:
//     UI_block__ (0, 0, 0);  // unblock all

      // TESTBLOCK
      // printf(" ex-MDL_sav_gcad %d\n",irc);
      // exit(0);
      // END TESTBLOCK

    return irc;

  //----------------------------------------------------------------
  L_err1:
    UtxTab_free (&sTab1);               // free mem
    if(fpo) fclose(fpo);
    irc = -1;
    goto L_exit;

}


//====================================================================
  int MDL_sav_tmp () {
//====================================================================
// MDL_sav_tmp          save the active model into tempFile
// save the active sub- or mainmodel AP_modact_nam -> TempFile
// AP_modact_nam = "" - mainmodel; else submodel
// - write DYNAMIC_DATA-block from memory / GA_ObjTab > file
// - write sourve             from UTF_FilBuf0        > file
//   retCode     0=OK, -1=Error, nothing to save, previous load-error

// TODO: test if modified before writing;
//  - if yes: recompute box with UT3D_box_mdl__ 
//    - and set AP_stat.mdl_box_valid / AP_stat.mdl_modified



  extern int DL_wri_dynDat0 (FILE*);

  int   irc;
  char  fnam[SIZFNam];


  // printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX \n");
  // printf("MDL_sav_tmp %d\n",AP_modact_inm);

  if(!mdl_tab.rNr) return -1;

  // if active model is image or mockup (see MDL_load_mNam_tmp) skip
  if(MDL_MTYP_IS_IMG_MOCK(MDL_MTYP_INAM(AP_modact_inm))) return 0;


  // vom Editor ins Memory kopieren
  if(AP_src == AP_SRC_EDI) ED_unload__ ();

//   // make list of all subModels of active model of model AP_modact_nam
//   Mod_flst_sm ();

  // get tempFilename of primary model
  irc = MDL_fnModel_mNam (fnam, AP_modact_inm);
  if(irc < 0) return -1;
    // printf(" _sav_tmp-fnam=|%s|\n",fnam);

//   // DYNAMIC_AREA (im Memory) zufuegen
//   DL_wri_dynDat0

  // Memory in Datei rausschreiben
  // UTF_wri_file (fnam, 1);
  UTF_wri_file (fnam, DL_wri_dynDat0);

  // // DYNAMIC_AREA (im Memory) wieder wegloeschen
  // UTF_del_rest(":DYNAMIC_AREA");

    // printf(" ex-MDL_sav_tmp |%s| fsiz=%ld\n",fnam,OS_FilSiz(fnam));

  return 0;

}


//================================================================
  int MDL_sav_ck__ (int mode) {
//================================================================
// MDL_sav_ck_ck              check if model is modified; if yes: save
// - used only for save main, not for export, save group, save subModel
// - save also unused-models
// Input:
//   mode         0   normal (during active app)
//                1   at shutdown (cannot restore for error-correction)
//   retCode      0=model-unmodified, 1=model-is-modified
//               -1 = error

  int     irc;


  // printf("MDL_sav_ck__ %d\n",mode);

  
  UI_block__ (1, 1, 1);  // block all


  // save the active model into tempFile
  irc = MDL_sav_tmp ();
  if(irc < 0) {irc = -1; goto L_exit;}


  // test if primary model is empty;
  if(MDL_sav_ck_prim ()) goto L_exit;


  // at shutdown (mode==1) must reset to primary = main !
  if(AP_modact_inm) MDL_mdl_tab_set_main ();


  // test if Mod_.mod_in exists; if not: imported model ask save, save;
  irc = MDL_sav_ck_new ();
  if(irc) goto L_exit;


  // test if model is modified; if yes: save
  irc = MDL_sav_ck_ck ();   // 0=model-unmodified, 1=model-is-modified
  if(irc < 0) {
    if(!mode) goto L_exit;
    // problem; cannot stop closing app; message error at asve model
    GUI_MsgBox ("**** error save model");
  }

  // save
  if(irc > 0) {
    TX_Print("**** model is modified ..");
    AP_save__ (0, 0, 0, "gcad");
//     AP_save__ (0, 1, 0, "gcad");
  }

  L_exit:

    UI_block__ (0, 0, 0);  // unblock all

      // printf(" ex-MDL_sav_ck__ %d\n",irc);

    return irc;

}


//================================================================
  int MDL_sav_ck_sm_1 (int iCk, int iSm) {
//================================================================
// MDL_sav_ck_sm_1          test if model iCk is using model iSm
// - check if model iSm is used by model iCk
// Input:
//   iCk         index mdl_names of parent-model
//   iSm         index mdl_names of child-model
// Output:
//   retCode     -1     model iCk does not use model iSm
//                >= 0  model iCk  is using model iSm

  int    irc, imn, ii;
  char   fnlCk[SIZFNam], *mnSm, fnSm;


  // printf("MDL_sav_ck_sm_1 iCk=%d iSm=%d\n",iCk,iSm);

  // get name from index iNam

  // skip own record
  if(iCk == iSm) return 0;

//   // ignore if not intern
//   if(mdl_tab.data[iSm].mTyp != MBTYP_INTERN) continue;


  // test if submodel iSm is used by submodel iCk

  // get mnSm = safe modelname of iSm
  mnSm = UtxTab__(iSm, &mdl_names);
    // printf(" sav_ck_sm_1-mnSm |%s|\n",mnSm);


  // get fnlCk = filename modelList for iCk
  MDL_fnModLst_iNam (fnlCk, iCk);
    // printf(" sav_ck_sm_1-fnlCk |%s|\n",fnlCk);

  // find word (line) mnSm in file fnlCk (a list of all used subModels)
  irc = UTX_fget_lnTxt (mnSm, fnlCk);

    // printf(" ex-MDL_sav_ck_sm_1 irc=%d iCk=%d iSm=%d\n",irc,iCk,iSm);

  return irc;

}


//================================================================
  int MDL_sav_ck_sm__ (int iSm) {
//================================================================
// MDL_sav_ck_sm__          test if sm imn belongs to intern or extern parent-model
// Input:
//   mNam         modelnam internal subModel, find its parent ..
// Output:
//   retCode     -1 = Error;
//                0 = parent-is-extern - do_not_save;
//                1 = parent-is-intern - save;

  int    irc, imn;
  char   fnl[SIZFNam], fnSm;


  // printf("MDL_sav_ck_sm__ %d\n",iSm);

  // check primary model (has mStat=1, not 2)
  irc = MDL_sav_ck_sm_1 (AP_modact_inm, iSm);
  if(irc >= 0) {
    goto L_exit;
  }


  // - loop tru active internal subModels (all models with mStat=2) -
  for(imn=0; imn < mdl_names.iNr; ++imn) {
    // ignore if not active
    if(mdl_tab.data[imn].mStat != 2) continue;
    // check if imn is internal; if not - ignore
    if(mdl_tab.data[imn].mStat != MBTYP_INTERN) continue;

    // check if model imn is using model iSm
    irc = MDL_sav_ck_sm_1 (imn, iSm);
    if(irc >= 0) goto L_exit;
  }

  irc = -1;


  L_exit:
      // printf(" ex-MDL_sav_ck_sm__ %d\n",irc);
    return irc;

}


//================================================================
  int MDL_sav_ck_new () {
//================================================================
// MDL_sav_ck_new               test for new model (if Mod_.mod_in exists)
// - if not: imported model; check if already exists; 
//   - save - yes or no
//   retCode     0  tmp/Mod_.mod_in exists
//               1

  int      i1, iOver, uuSav;
  char     fn[320];


  // printf("MDL_sav_ck_new \n");


  // test if Mod_.mod_in exists
  sprintf(fn, "%sMod_.mod_in", AP_get_tmp_dir());
  i1 = OS_checkFilExist (fn, 1);  // 0=exists-not; else yes
  if(i1) return 0;


  //----------------------------------------------------------------
  // Mod_.mod_in does not yet exist = eg unstored imported model
  // get full outfilename
  MDLFN_ffNam_AP (fn);
    // printf(" _sav_ck_new-fn |%s|\n",fn);

  iOver = 0;  // ask for filename; ask for overwrite yes|no|cancel
  uuSav = 0;  // save also unused-models

  AP_save__ (0, iOver, uuSav, AP_mod_ftyp);

  return 1;

}


//================================================================
  int MDL_sav_ck_prim () {
//================================================================
// MDL_sav_ck_prim                test if primary model is empty;
//   eg import empty file ..
// retCode   0   model has data
//           1   model is empty
//          -1   Error
// see ED_Read_Line

  int      irc, sizS1, i1;
  FILE     *fp1;
  char     *s1, fnam[SIZFNam];


  // printf("MDL_sav_ck_prim \n");

  s1 = mem_cbuf1;
  sizS1 = mem_cbuf1_SIZ;


  // get tempFilename of primary model
  irc = MDL_fnModel_mNam (fnam, AP_modact_inm);
  if(irc < 0) goto L_exit;
    // printf(" _ck_prim-fnam |%s|\n",fnam);

  
  if((fp1=fopen(fnam,"r")) == NULL) {
    TX_Print("***** MDL_sav_ck_prim E001 %s",fnam);
    irc = -1;
    goto L_exit;
  }
  
  i1 = 1;
  while (!feof (fp1)) {
    if(fgets (s1, sizS1, fp1) == NULL) break;
      // printf("%1d  %-50s\n",i1,s1);

    // skip until ":DYNAMIC_DATA"
    if(i1) {
      if(!strncmp(s1, ":DYNAMIC_DATA", 13)) i1 = 0;
      continue;
    }
    // skip comments / removed lines
    UTX_pos_skipLeadBlk (s1);
    if((*s1 != '#')&&(*s1 != '_')) {
      irc = 0;
      goto L_close;
    }
  }

  irc = 1; // model is empty
  
  L_close:
    fclose (fp1);


  L_exit:
    // printf(" ex-MDL_sav_ck_prim %d\n",irc);
  return irc;
  
}


//================================================================
  int MDL_sav_ck_ck () {
//================================================================
// MDL_sav_ck_ck              check if model is modified
// - used only for save main, not for export, save group, save subModel
// - save also unused-models
// - create complete modelfile tmp/Mod_.mod_out
// - compare tmp/Mod_.mod_in - tmp/Mod_.mod_out
//   retCode      0=model-unmodified, 1=model-is-modified
//               -1 = error

  int     irc;


  // printf("MDL_sav_ck_ck \n");


  // create complete modelfile tmp/Mod_.mod_out
  irc = MDL_sav_gcad (0,0);
  if(irc < 0) return irc;


  // compare tmp/Mod_.mod_in - tmp/Mod_.mod_out
  irc = MDL_sav_cmp__ ();


    // printf(" ex-MDL_sav_ck_ck %d\n",irc);

  return irc;

}


//================================================================
  int MDL_sav_cmp__ () {
//================================================================
// MDL_sav_cmp             compare initial model with modified model
// - compare file Mod_.mod_in with file Mod_.mod_out
//   retCode      0=files_not_different, 1=different_files
// was Mod_sav_cmp__



  int     irc = 0, ieof=0, llM1, llM2;
  long    l1;
  char    fnM1[320], fnM2[320];
  char    *lnM1, *lnM2;  // linebuffers
  FILE    *fpM1 = NULL, *fpM2 = NULL;


  // printf("MDL_sav_cmp__\n");

  sprintf(fnM1, "%sMod_.mod_in",  AP_get_tmp_dir());
  sprintf(fnM2, "%sMod_.mod_out", AP_get_tmp_dir());


  //----------------------------------------------------------------
  // if Mod_.mod_in is empty: no file has been loaded ..
  if(OS_FilSiz(fnM1) < 8) return 0;


  //----------------------------------------------------------------
  // open model1
  if ((fpM1 = fopen (fnM1, "r")) == NULL) {
    printf ("ERROR open %s\n",fnM1);
    goto L_errEx;
  }


  //----------------------------------------------------------------
  // open model2
  if ((fpM2 = fopen (fnM2, "r")) == NULL) {
    printf ("ERROR open %s\n",fnM2);
    goto L_errEx;
  }


  //----------------------------------------------------------------
  // get space for the next 2 lines
  lnM1 = APED_SRCLN_BUF__;
  // lnM2 = (char*) MEM_alloc_tmp (APED_SRCLN_BUFSIZ);
  lnM2 = memspc201;



  //----------------------------------------------------------------
//   // test if Mod_.mod_in has :DYNAMIC_DATA-section;
//   // if not: skip :DYNAMIC_DATA-section in Mod_.mod_out
//   // (import files (dxf ..) do not have :DYNAMIC_DATA-section)
//   L_nxtck0:
//     if(fgets (lnM1, APED_SRCLN_BUFSIZ, fpM1) == NULL) goto L_nxtck2;   // not found
// 
//     if(!strncmp (lnM1, ":DYNAMIC_DATA", 13)) {
//       // :DYNAMIC_DATA-section is in Mod_.mod_in; rewind Mod_.mod_in.
//       rewind (fpM1);
//       goto L_nxtM1;
//     }
//     goto L_nxtck0;
// 
// 
//   L_nxtck2: // NO :DYNAMIC_DATA-section in Mod_.mod_in;
//     // rewind Mod_.mod_in; find ":DYNAMIC_DATA" in Mod_.mod_out
//     rewind (fpM1);
//     L_nxtck3:
//       if(fgets (lnM2, APED_SRCLN_BUFSIZ, fpM2) == NULL) {
//         printf("***** MDL_sav_cmp__ E1\n");
//         return 1;
//       }
//       if(strncmp (lnM2, ":DYNAMIC_DATA", 13)) goto L_nxtck3;


  //----------------------------------------------------------------
  // get next line of model1
  L_nxtM1:
  if(fgets (lnM1, APED_SRCLN_BUFSIZ, fpM1) == NULL) {  // printf(" EOF M1\n");
    ++ieof; goto L_nxtM2; }
  llM1 = strlen(lnM1);
  if(llM1 <= 1) goto L_nxtM1;
  UTX_endDel_crlf (lnM1, &llM1);   // remove closing cr,lf 
  if(llM1 <= 1) goto L_nxtM1;
    // printf(" M1 %d |%s|",llM1,lnM1);
  // test if line can be ignored
  if(MDL_sav_cmp_i(lnM1)) goto L_nxtM1;


  //----------------------------------------------------------------
  // get next line of model2
  L_nxtM2:
  if (fgets (lnM2, APED_SRCLN_BUFSIZ, fpM2) == NULL) {  // printf(" EOF M2\n");
    ++ieof; goto L_nxtCmp; }
  llM2 = strlen(lnM2);
  if(llM2 <= 1) goto L_nxtM2;
  UTX_endDel_crlf (lnM2, &llM2);   // remove closing cr,lf 
  if(llM2 <= 1) goto L_nxtM2;
    // printf(" M2 %d |%s|",llM2,lnM2);
  // test if line can be ignored
  if(MDL_sav_cmp_i (lnM2)) goto L_nxtM2;

  //----------------------------------------------------------------
  // 2 lines to compare ?
  L_nxtCmp:
  if(ieof) {
    if(ieof == 1) goto L_errEx;             // ieof=1: premature end of 1 file
    goto L_exit;                            // ieof=2: finished, OK.
  }


  // compare
  if(llM1 != llM2) goto L_errEx;
  if(memcmp(lnM1, lnM2, llM1)) goto L_errEx;  // objDat already exists

  // ok, both lines equal;
  goto L_nxtM1;


  //----------------------------------------------------------------
  L_errEx:
  irc = 1;
    // printf("  _sav_cmp__-1-|%s| %d\n",lnM1,llM1);
    // printf("  _sav_cmp__-2-|%s| %d\n",lnM2,llM2);


  //----------------------------------------------------------------
  L_exit:
  if(fpM1) fclose (fpM1);
  if(fpM2) fclose (fpM2);

    // printf(" ex-MDL_sav_cmp__ %d\n",irc);

  return irc;

}


//================================================================
  int MDL_sav_cmp_i (char *ln1) {
//================================================================
// test if line can be ignored
// retCode:   1=ignore_line;  0=test_line
// ignore also DEFTX DEFCOL CONST_PL (will be added to subModel when activated)

  if(!strncmp(ln1, "MODSIZ ", 7))         return 1;
  if(!strncmp(ln1, "MODBOX ", 7))         return 1;
  if(!strncmp(ln1, "VIEW ", 5))           return 1;

  if(!strncmp(ln1, "DEFCOL ", 7))         return 1;
  if(!strncmp(ln1, "DEFTX ", 6))          return 1;
  if(!strncmp(ln1, "CONST_PL ", 9))       return 1;
  if(!strncmp(ln1, "MODE ", 5))           return 1;

  if(!strncmp(ln1, ":DYNAMIC_DATA ", 13)) return 1;



  return 0;

}


//=====================================================================
  int MDL_exp__ (stru_FN *ofm, int mode, int uuSav) {
//=====================================================================
// MDL_exp__               save/export model
// Input:
//   ofm        filenameObj
//   mode       0  save|export main; set AP_mod_*;
//              2  save|export active model;
//   uuSav      0=save-unused-models
//              1=do-NOT-save-unused-models
// Output:
//   retCod     o=ok; -1=err;
//
// see also AP_save__


  int   irc, i1, ift;
  char  fno[320], s1[320], s2[256];


  // printf("MDL_exp__ mode=%d uuSav=%d\n",mode,uuSav);
  // MDLFN_dump_ofn (ofm, "MDL_exp__");
  // printf("  |%s|%s|%s|%s| %d %d\n",AP_mod_sym,AP_mod_dir,
         // AP_mod_fnam,ofm->fTyp,ofm->iTyp,AP_stat.subtyp);

  // save the active model
  MDL_sav_tmp ();  // already done in AP_save__ ?

//   // check if outDir AP_mod_dir exists
//   if(OS_checkFilExist(AP_mod_dir,1) ==  0) {
//     // TX_Error("Directory %s does not exist ..",AP_mod_dir);
//     MSG_err_1 ("NOEX_dir", "%s", AP_mod_dir);
//     return 0;
//   }
// 
// 
//   // UI_CursorWait (0); // wait-cursor
// 
//   // clear Undo-List (sonst wilde gtk-Fehlermeldungen beim exit)
//   UNDO_clear ();



  //----------------------------------------------------------------
  // get outfilename fno
  MDLFN_ffNam_oFn (fno, ofm);
    // printf(" fno=|%s|\n",fno);

  if(ofm->iTyp == Mtyp_Gcad) {
    // save native into file tmp/Mod_comp and copy out
    MDL_sav__ (fno, mode, uuSav);
    goto L_fertig;


  } else if((ofm->iTyp == Mtyp_WRL)&&(AP_stat.subtyp == 1)){
    // printf(" Export VRML1 |%s|\n",AP_mod_fnam);
    // OS_dll_do ("xa_vr2_exp", "VR2_exp__", fno, 0);
    AP_kex_exec (&dll_exp, "xa_vr2_exp", "VR2_exp__", fno, DLL_LOAD_EXEC_UNLOAD);
    // cp <tmpDir>export.exp AP_mod_dir.AP_mod_fnam.ofm->fTyp
    sprintf(s1, "%sexport.exp",AP_get_tmp_dir());
    irc = OS_file_copy (s1, fno);
    if(irc) { TX_Error("MDL_exp__ cannot copy to %s",s2); return -1; }
    TX_Print ("%s exported ..",fno);
    goto L_fertig;


  } else if(ofm->iTyp == Mtyp_DXF) {
    // printf(" Export DXF |%s|\n",AP_mod_fnam);
    // OS_dll_do ("xa_dxf_w", "DXFW__", fno, 0);
    AP_kex_exec (&dll_exp, "xa_dxf_w", "DXFW__", fno, DLL_LOAD_EXEC_UNLOAD);
    goto L_fertig;


  } else if(ofm->iTyp == Mtyp_SVG) {
    // printf(" Export SVG |%s|\n",AP_mod_fnam);
    // OS_dll_do ("xa_svg_w", "SVG_w__", fno, 0);
    AP_kex_exec (&dll_exp, "xa_svg_w", "SVG_w__", fno, DLL_LOAD_EXEC_UNLOAD);
    goto L_fertig;


  } else if(ofm->iTyp == Mtyp_Iges) {
    // printf(" Export IGS |%s|\n",AP_mod_fnam);
    AP_ExportIges__ (fno);
    goto L_fertig;


  } else if(ofm->iTyp == Mtyp_Step) {
    // OS_dll_do ("xa_stp_w", "STP_w__", fno, 0);
    AP_kex_exec (&dll_exp, "xa_stp_w", "STP_w__", fno, DLL_LOAD_EXEC_UNLOAD);
    goto L_fertig;


  } else if(ofm->iTyp == Mtyp_JPG) {
    sprintf(s1, "%swin.bmp",AP_get_tmp_dir());
    // create BMP-file of active OpenGL-window
    bmp_save__ (s1);
    // create JPG-file from BMP-file
    OS_jpg_bmp (fno, s1);
    TX_Print ("- %s exported ..",fno);
    goto L_fertig;


  } else if((ofm->iTyp == Mtyp_WRL)  ||
            (ofm->iTyp == Mtyp_STL)  ||
            (ofm->iTyp == Mtyp_OBJ)  ||
            (ofm->iTyp == Mtyp_TESS)) {
    goto L_mock;

  } else {
    TX_Print("**** MDL_exp__ Error filetyp %s %d",ofm->fTyp,ofm->iTyp);
    return -1;
  }



  //================================================================
  // save Mockup         // see UI_saveMockCB
  L_mock:
  // tess. exportieren ..
  // sprintf(memspc011, "%s%c%s",dirNam,fnam_del,fnam);
// TODO:
  // get filetyp-string from filetyp-nr
  i1 = TSU_exp__ (ofm->fTyp, fno);
  if(i1 != 0) DB_del_Mod__ ();  // del all subModels nach error

  ED_Reset ();       // ED_lnr_act resetten
  ED_work_END (0);   // restore display ..
  // TX_Print("save tesselated as %s in %s",AP_mod_fnam,AP_mod_dir);




  //================================================================
  // Defaults rausschreiben
  L_fertig:

//   AP_defaults_write ();
// 
//   // Title oben auf den Mainwinrahmen
//   UI_title_set ();
//   // UI_AP (UI_FuncSet, UID_Main_title, NULL);
// 
//   // MAN: reset focus                     // 2013-04-12
//   // sonst kein Cursor im Editor mehr ..
//   if(UI_InpMode == UI_MODE_MAN) {
//     GUI_edi_Focus (&winED);
//   }
// 
// 
//   // model unmodified ..
//   // AP_mdl_modified_reset ();
// 
//   // make a copy of Model for ck-modified (make tmp/Mod_in)
//   Mod_sav_ck (0);

  return 0;

}


/* DO NOT USE - UNUSED - use MDL_load_import_ext
//================================================================
  int MDL_imp__ (stru_FN *ofn) {
//================================================================
// MDL_imp__          import model
//   create files tmp/Model_<main> and subModels tmp/Model_<smNam>
// Input:
//   ofn          filenameObj of inputmodel
// Output:
//   retCode      0=OK, file tmp/Model_<ofn->fNam> created;
//               -1=err.
// was AP_Mod_load__
// see MDL_load_import__

  extern int DL_wri_dynDat0 (FILE*);

  int     irc, dbResiz, impTyp;
  long    lTab[8];
  char    fni[320], fno[320], s1[320], symNam[SIZMFTot], safNam[SIZMFTot];
  void    *pa[2];


  printf("MDL_imp__ \n");
  MDLFN_dump_ofn (ofn, "MDL_imp__");

/
// #define Mtyp_Gcad       0
// #define Mtyp_DXF        1
// #define Mtyp_Iges       2
// #define Mtyp_Step       3
// #define Mtyp_3DS        4
// #define Mtyp_LWO        5
// #define Mtyp_XML        6
// #define Mtyp_SVG        7
// 
// #define Mtyp_TESS      10    // gcad-tesselated-data
// #define Mtyp_WRL       11    ///< VRML1      10-19  tess-Formate
// #define Mtyp_WRL2      12    ///< VRML2
// #define Mtyp_OBJ       13
// #define Mtyp_STL       14
 //  
// #define Mtyp_BMP       20    ///< .bmp       20-29 PixelImages
// #define Mtyp_JPG       21    ///< .jpg
/


  irc = 0;
  dbResiz = 0;  // yes, increase DB-size


  // get fni = filename-absolute
  MDLFN_ffNam_oFn (fni, ofn);

  sprintf(fno, "%sModel_%s",AP_get_tmp_dir(),ofn->fNam);

  switch(ofn->iTyp) {

    //----------------------------------------------------------------
    case Mtyp_Gcad:
      // get mainModel into tmp as Mod_.tmp_in and split into files tmp/Model_<mNam>
      irc = MDL_load_file_gcad (ofn->fDir, ofn->fNam, ofn->fTyp);
      if(irc < 0) goto L_err1;
      goto L_exit;


    //----------------------------------------------------------------
    case Mtyp_DXF:
      pa[0] = fni;
      // s1[0] = '\0';
      pa[1] = ofn->fNam;    //s1;     // empty = primary main
      irc = OS_dll_do ("xa_dxf_r", "DXF_r__", pa, 0);
      if(irc < 0) goto L_err2;
      goto L_exit;  // mode = 2; goto L_native;


    //----------------------------------------------------------------
    case Mtyp_Iges:
      irc = OS_dll_do ("xa_ige_r", "IGE_r__", fni, 0);
      AP_stru_2_txt (NULL, 0, (void*)lTab, 1L); // ask last index
      DB_size_set (lTab);                       // increase DB-size
      // dbResiz = 1;                              // DB-resize done
      goto L_mem2file;


    //----------------------------------------------------------------
    case Mtyp_Step:
      // export into file <tmpDir>Model
      pa[0] = fni;
      // s1[0] = '\0';
      pa[1] = ofn->fNam;    // s1;     // empty = primary main
      irc = OS_dll_do ("xa_stp_r", "STP_r__", pa, 0);
        // printf(" foll-OS_dll_do %d\n",irc);
      if(irc < 0) goto L_err2;
      AP_stru_2_txt (NULL, 0, (void*)lTab, 1L); // ask last index
      DB_size_set (lTab);                       // increase DB-size
      // dbResiz = 1;                              // DB-resize done
      // mode = 2;
      goto L_exit;  // mode = 2; goto L_native;


    //----------------------------------------------------------------
    case Mtyp_3DS:
      AP_Import3ds (0, "0,0,0", 1., fni);
      goto L_mem2file;


    //----------------------------------------------------------------
    case Mtyp_LWO:
      AP_ImportLwo (0, "0,0,0", 1., fni);
      goto L_mem2file;


    //----------------------------------------------------------------
    case Mtyp_OBJ:
      // get ImportTyp; nativ oder Mockup. (user-checkBox)
      UI_AP (UI_FuncGet, UID_ckb_impTyp, (void*)&impTyp); // 1=Mockup, 0=native
      if(!impTyp) {
        // create native file - not tess-data.
        irc = OS_dll_do ("xa_obj_r", "obj_read__", fni, 0); // 2013-08-15
        if(irc < 0) goto L_err2;
        goto L_mem2file;
      }


    //----------------------------------------------------------------
    case Mtyp_TESS:
    case Mtyp_WRL:
    case Mtyp_STL:
      // get symbolic filename
      MDLFN_syFn_oFn (symNam, ofn);
      // "M20="<symDir>/<filNam>.<filTyp>" P(0 0 0)"
      sprintf(s1, "M20=\"%s\" P(0 0 0)\n#\n",symNam);

      // get safe modelname
      MDL_safename (safNam, symNam);
      // init new mainModel with add. line s1
      MDL_init_noCad (safNam, s1);
//       MDL_init_noCad (ofn->fNam, s1);
      goto L_exit;



    //----------------------------------------------------------------
    case Mtyp_BMP:
    case Mtyp_JPG:
      // UI_loadImg_CB (mnam, AP_mod_dir);
      // create empty model with "N20=IMG P(0 0 0) "<symDir>/<filNam>.<filTyp>"
      // get symbolic filename
      MDLFN_syFn_oFn (symNam, ofn);
      // main-model: must add "N20=IMG P(0 0 0) "<symDir>/<filNam>.<filTyp>"
      sprintf(s1, "N20=IMG P(0 0 0) \"%s\"\n#\n",symNam);

      // get safe modelname
      MDL_safename (safNam, symNam);
      // init new mainModel with add. line s1
      MDL_init_noCad (safNam, s1);
//       MDL_init_noCad (ofn->fNam, s1);
      goto L_exit;


    //----------------------------------------------------------------
    default:
      goto L_err1;
  }


  //----------------------------------------------------------------
  // if(dbResiz == 0) {
    // APED_search_dbLimits (lTab);          // search highest indices in Model
    // DB_size_set (lTab);                   // increase DB-size
  // }



  //----------------------------------------------------------------
  L_mem2file:
    // importcode is in UTF_FilBuf0; write -> tmp/Mod_<ofn->fNam>
      // UTF_dump__ ();
    // UTF_wri_file (fno, DL_wri_dynDat0);
    irc = UTF_wri_file (fno, NULL);
    if(irc < 0) goto L_exit;
    ED_Reset ();
    ED_work_END (0);
    if(AP_errStat_get() != 0) irc = -1;



  //----------------------------------------------------------------
  L_exit:
    // file tmp/Mod_<ofn->fNam> exists.

    // TESTBLOCK
    printf("ex-MDL_imp__ %d\n",irc);
    // exit(0);
    // END TESTBLOCK

  return irc;

  L_err1:
    TX_Print("**** ERROR - import filetyp %s not implemented yet",ofn->fTyp);
    irc = -1;
    goto L_exit;

  L_err2:
    TX_Print("**** ERROR - importing filetyp %s",ofn->fTyp);
    irc = -1;
    goto L_exit;


}
*/

//====================================================================
  int MDL_load_main (char *fNam) {
//====================================================================
// MDL_load_main          load as mainModel
// Input:
//   fNam            symbolic filename
//
// was  AP_mdl_init - AP_Mod_load_fn AP_Mod_load__ Mod_load__

  int       irc, impTyp;
  char      symNam[SIZMFTot], safNam[SIZMFTot], fn1[320];
  stru_FN   ofn;

  // printf("MDL_load_main |%s| !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n",fNam);
  // MDL_dump__ ("MDL_load_main");


  // clear dispList
  DL_Init ();

  // clear PRCV, init new model, kill all files, textures, init new model, color ..
  AP_mdl_init (0);


  //----------------------------------------------------------------
  // get ofn = fileName-obj from (unknown) filename
  irc = MDLFN_oFn_fNam (&ofn, fNam);
  if(irc < 0) goto L_exit;
    // MDLFN_dump_ofn (&ofn, "MDL_load_main-1");

  // symbolic-filename from filenameObject
  MDLFN_syFn_oFn (symNam, &ofn);
    // printf(" load_init-symNam |%s|\n",symNam);

  // get safe name
  irc = MDL_mNam_safe (safNam, symNam);
  if(irc) {irc = -1; goto L_exit;}

  AP_stat.mdl_stat = MDLSTAT_empty;

  //================================================================
  // direct load only native-models and cad-models
  // if model is not native-model then create native-internal-model.
//   if(ofn.iTyp < Mtyp_TESS) {
  if(ofn.iTyp < Mtyp_3DS) {
    goto L_load1;
  }

  // get checkbox "nativ oder Mockup"
  UI_AP (UI_FuncGet, UID_ckb_impTyp, (void*)&impTyp); // 1=Mockup, 0=native
  if((ofn.iTyp == Mtyp_OBJ)&&(!impTyp)) goto L_load1;  // .obj: native|mockup

  // load mockup; create internal-native-model from not-native-modeltyp
  irc = MDL_load_noNat (symNam, safNam, &ofn);
  if(irc < 0) goto L_exit;


  //================================================================
  // direct-load.
  L_load1:

  AP_modact_ibm = -1;
  AP_modact_inm = 0;
  AP_modact_tmp = 0;

  // set global AP_mod_sym and AP_mod_dir
  strcpy(AP_mod_sym, ofn.symDir);
  strcpy(AP_mod_fnam, ofn.fNam);
  strcpy(AP_mod_dir, ofn.fDir);

  // init mdl_names, loadmap (add symNam as mainName into mdl_tab, mdl_names
  MDL_init_MDL (symNam, ofn.iTyp);

    // TESTBLOCK
    // MDL_dump__ ("MDL_load_main-3");
    // UTF_dump__ ("MDL_load_main-3");
    // END TESTBLOCK
 

  // import all submodels (mStat 1)
  irc = MDL_load_1 ();
  if(irc < 0) goto L_exit;


  // load mainModel into memSpc (UTF_FilBuf0)
  sprintf(fn1, "%sModel_%s",AP_get_tmp_dir(), safNam);
  irc = UTF_add_fil_init (fn1);
  if(irc < 0) goto L_exit;


  //================================================================
  L_exit:

    // TESTBLOCK
    // MDL_dump__ ("ex-MDL_load_main");
    // printf("ex-MDL_load_main %d\n\n",irc);
    // DEB_exit();
    // END TESTBLOCK

  return irc;

}


//================================================================
  int MDL_load_1 () {
//================================================================
// MDL_load_1         import and scan all models in mdl_tab
//   loop tru mdl_tab - for all models do:
//   - create all necessary importfiles in dir. tmp, create baseModels
//   - set mdl_tab[].mStat = 1;  // imported, not yet loaded


  int       irc, i1, iTab, iNr, imn, mTyp;
  char      *mNam, safNam[SIZMFTot], *sm, fn[320];
  TxtTab    smNames = _UTXTAB_NUL;
  stru_tab  *mTab;


  // printf("\n11111111111111111111111111111111111111111111111111111111111111\n");
  // printf("MDL_load_1\n");
  // MDL_dump__ ("MDL_load_1");



  iTab = 0;
  mTab = MEMTAB_DAT (&mdl_tab);


  //================================================================
  // import and scan - stat 0 -> 1
  // loop tru mdl_tab, find 
  L_nxt:
      // printf(" ================== _load__-nxt stat iTab=%d  0 -> 1\n", iTab);

    if(mTab[iTab].mStat != 0) goto L_cont;
  
      // printf(" ------------------- _load__-nxt [%d] %s\n",
             // iTab,MDL_MNAM_INAM(iTab));

    // 0 -> 1;  import and scan and create baseModel
    mNam = UtxTab__(iTab, &mdl_names);

    // get safe-name
    strcpy (safNam, mNam);
    UTX_safeName (safNam, 1);

    // test if modelfile already exists (unused, splitted from extern model)
    sprintf(fn, "%sModel_%s", AP_get_tmp_dir(), safNam);
    if(OS_checkFilExist(fn, 1)) goto L_imp_ok; // model exists = done

    // import,  create file tmp/Model_<safMnam>
    irc = MDL_load_import__ (mTab[iTab].mTyp, mNam);
    if(irc < 0) {TX_Error ("MDL_load_1 E00"); irc = -1; goto L_exit;}


    //----------------------------------------------------------------
    // update imported models (now native)
    L_imp_ok:
       // printf(" _load_1-L_imp_ok: %d mTyp=%d |%s|\n",iTab,mTab[iTab].mTyp,mNam);
//     if((mTab[iTab].mTyp > Mtyp_Gcad)   && 
//        (mTab[iTab].mTyp < Mtyp_TESS))          { 
      // main-model only -
      if(iTab == 0) {
        AP_mod_iftyp = Mtyp_Gcad;
        strcpy(AP_mod_ftyp, "gcad");
      }
//     }
      // printf(" _load_1-L2 %d mTyp=%d |%s|%s|\n",iTab,mTab[iTab].mTyp,mNam,safNam);


    // scan modelfile for submodels; add submodels to mdl_tab and loadmap
    // - create file tmp/Mod_<safMnam>.lst
    irc = MDL_load_scan__ (&smNames, mNam, 0);
    if(irc < 0) {TX_Error ("MDL_load_1 E01"); irc = -1; goto L_exit;}


    // create basicModel in DB
    // - do not create basicModel for main-model (inm=0)
    if(iTab > 0) {
      irc = DB_StoreModBas (mTab[iTab].mTyp, mNam);
      if(irc < 0) {TX_Error ("MDL_load_1 E02"); irc = -1; goto L_exit;}
      // set mdl_tab[].mbi
      mTab[iTab].mbi = irc;
    }

    // update mStat
    mTab[iTab].mStat = mdlStat_passive;  // 0;

    // unused
    mTab[iTab].mUsed = mdlStat_unused;

//     // find unused-models, add to mdl_tab as unused;
//     UtxTab_init__ (&smNames);
//     iNr = MDL_smuu_lst (&smNames);
//     if(iNr > 0) {
//       for(i1=0; i1<iNr; ++i1) {
//         sm = UtxTab__(i1, &smNames);
//         mTyp = MDL_mTyp_mNam (sm);
//         if(mTyp <= MBTYP_DUP) {TX_Print("***** MDL_load_1 E3 %s",sm); continue;}
//         imn = MDL_mdl_add (mTyp, sm);
//         MDL_USED_SET_INAM (imn, mdlStat_unused);
//       }
//     }


  //----------------------------------------------------------------
  L_cont:
    ++iTab;
    if(iTab < mdl_tab.rNr) goto L_nxt;

    irc = 0;


  //----------------------------------------------------------------
  L_exit:
    UtxTab_free (&smNames);


//     MDL_used_set__ ();

      // TESTBLOCK
      // MDL_dump__ (" ex-MDL_load_1");
      // printf(" ex-MDL_load_1 irc=%d\n",irc);
      // DEB_exit();
      // END TESTBLOCK

    return irc;

}


//================================================================
  int MDL_load_2 (int imn) {
//================================================================
// MDL_load_2       create loadmap for model imn, load all models
// Input:
//   imn   index modelname of model to load

  int   irc;


  // printf("222222222222222222222222222222222222222222222222222222222222222\n");
  // printf("MDL_load_2 %d\n",imn);

  // create loadmap for model imn
  irc = MDL_ldm__ (imn);
  if(irc < 0) goto L_exit;

  // load into dispList; load all new models - in reverse order
  irc = MDL_mdb_load__ ();

  // delete loadmap
  MDL_LDM_CLEAR ();

  L_exit:
      // printf(" ex-MDL_load_2 %d\n",irc);
    return irc;

}


//================================================================
  int MDL_ldm_f () {
//================================================================
// MDL_ldm_f               create loadmap and write into file tmp/Mod.lst
// - type of outNames is symbolic, not safe
// - reversed - load first model first
//   retCode       -1=err, else nr of models

  int   irc, i1, *ia;
  char  fnam[320];
  FILE  *fpo;


  // printf("MDL_ldm_f %d\n",mdl_ldm.rNr);

  // create loadmap for active model
  irc = MDL_ldm__ (AP_modact_inm);
  if(irc < 0) goto L_exit;

  // open outfile
  sprintf(fnam,"%sMod.lst",AP_get_tmp_dir());
  if((fpo=fopen(fnam,"w")) == NULL) {
    TX_Print("***** MDL_ldm_f E2 %s",fnam);
    irc = -2;
    goto L_exit;
  }

  ia = mdl_ldm.data;
//   for(i1=0; i1<mdl_ldm.rNr; ++i1) {
  for(i1=mdl_ldm.rNr - 1; i1 >= 0; --i1) {
      // printf(" %4d  %3d  %s\n",i1, ia[i1], UtxTab__(ia[i1], &mdl_names));
    fprintf(fpo,"%s\n",UtxTab__(ia[i1], &mdl_names));
  }

  fclose(fpo);

  irc = mdl_ldm.rNr;


  L_exit:
    // printf("ex-MDL_ldm_f %d\n",irc);
  return irc;

}


//================================================================
  int MDL_ldm__ (int imn) {
//================================================================
// MDL_ldm__            create loadmap for model imn
//   clear map, add mnam[imn]; add recursiv all its subModels;
// Input:
//   imn   index modelname of model to load
// Output:
//   retCode    0=OK, -1=err

  int       irc;
  char      symNam[SIZMFTot], *mn;


  // printf("\nMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM \n");
  // printf(" MDL_ldm__ %d\n",imn);

  //   clear map
  MDL_LDM_CLEAR ();

  // add model imn to loadmap (if its not main)
  if(imn) {
    irc = MDL_ldm_add__ (imn);
    if(irc < 0) return irc;
  }

  //----------------------------------------------------------------
  // add all subModels in Mod_<mnam>.lst recursiv to loadmap

  // get symbolic modelname of imn
  mn = UtxTab__(imn, &mdl_names);
  if(!mn) {TX_Error("MDL_ldm__ E1");  return -1;}
  strcpy(symNam, mn);


  // add all models in tmp/Mod_<symNam>.lst to loadmap (recursive)
  return MDL_ldm_add_flst (symNam);

}
 

//================================================================
  int MDL_load_noNat (char *symNam, char *safNam, stru_FN *ofn) {
//================================================================
// MDL_load_noNat             create native-model from not-native-modeltyp
//   create native-internal modelfile tmp/Model_<safNam>
//   only mockups, images - create model with code using the not-native-model
//
// was MDL_imp__


  int    irc=0, typIn;
  char   s1[320], symIn[SIZMFTot];


  // printf("MDL_load_noNat |%s|%s|\n",symNam,safNam);
  // MDLFN_dump_ofn (ofn, "MDL_load_noNat");
  // MDL_dump__ ("MDL_load_noNat");


  // keep original modeltype and symNam
  typIn = ofn->iTyp;
  strcpy(symIn, symNam);

  // make internal
  // must change the main-name, else submodel ignored (name ident) or loop
  // strcpy(ofn->fTyp, "import");
  // strcat(ofn->fTyp, "_imp");
  strcat(ofn->fNam, "_imp");
  // do NOT modify ofn->iTyp
  MDLFN_syFn_oFn (symNam, ofn);
  UTX_safeName (symNam, 1);
  strcpy(safNam, symNam);


  switch(typIn) {

    //----------------------------------------------------------------
    case Mtyp_3DS:
    case Mtyp_LWO:
    case Mtyp_WRL:
    case Mtyp_OBJ:
    case Mtyp_STL:
    case Mtyp_TESS:
      // "M20="<symDir>/<filNam>.<filTyp>" P(0 0 0)"
      sprintf(s1, "M20=\"%s\" P(0 0 0)\n#\n",symIn);
      // init new mainModel with add. line s1
      MDL_init_noCad (safNam, s1);
      break;



    //----------------------------------------------------------------
    case Mtyp_BMP:
    case Mtyp_JPG:
      // main-model: must add "N20=IMG P(0 0 0) "<symDir>/<filNam>.<filTyp>"
      sprintf(s1, "N20=IMG P(0 0 0) \"%s\"\n#\n",symIn);
      // init new mainModel with add. line s1
      MDL_init_noCad (safNam, s1);
      break;

    //----------------------------------------------------------------
    default:
    TX_Print("**** ERROR - import filetyp %s not implemented yet",ofn->fTyp);
    irc = -1;
    goto L_exit;
  }


  L_exit:
    // file tmp/Model_<safNam> exists.

    // TESTBLOCK
    // MDLFN_dump_ofn (ofn, "ex-MDL_load_noNat");
    // printf("ex-MDL_load_noNat %d symNam=|%s|%s|\n",irc,symNam,safNam);
    // exit(0);
    // END TESTBLOCK

  return irc;

}


/*
//================================================================
  int MDL_load_file_gcad (char *fDir, char *fNam, char *fTyp) {
//================================================================
// MDL_load_file_gcad           get mainModel into tmp as Mod_.tmp_in
// Input:
// Output:
//   retCode      0=OK, -1=err.
//
// was Mod_load__


  int     irc;
  char    fno[SIZMFTot], fni[SIZMFTot], s1[SIZMFTyp];


  printf("MDL_load_file_gcad |%s|%s|%s|\n",fDir,fNam,fTyp);


  //----------------------------------------------------------------
  // copy model -> tmp/Mod_tmp_in
  sprintf(fni,"%s%s.%s",fDir,fNam,fTyp);
  sprintf(fno,"%sMod_.tmp_in",AP_get_tmp_dir());
    printf(" load_file_gcad-fni |%s|\n",fni);
    printf(" load_file_gcad-fno |%s|\n",fno);

  // check for compressed Model
  UTX_cp_word_2_upper (s1, fTyp);   // make uppercase
  if(!strcmp(s1, "GCAZ")) {
    // decompress & copy
    OS_file_zip(1, fni, fno);

  } else {
    // copy
    OS_file_copy (fni, fno);
  }


  //----------------------------------------------------------------
  // split file tmp/Mod_.tmp_in into files tmp/Model_<mNam>
  MDL_load_file_split (fno, fNam);

  L_exit:
    printf("ex-MDL_load_file_gcad \n");
  return 0;

}
*/

//================================================================
  int MDL_load_file_split (char *filNam, char *mdlNam) {
//================================================================
// MDL_load_file_split             split file filNam into files tmp/Model_<mNam>
// Input:
//   filNam      full filename of model (with absolute path)
//   mdlNam      modelname mainModel
// Output:
//   retCode      0=OK, -1=err.
// was Mod_load_split


  int     irc, lSiz;
  long    lTab[8], lNr, ll, i1;
  char    cbuf[256], *lBuf;
  FILE    *fpi, *fpo=NULL;


  // printf("MDL_load_file_split |%s|%s|\n",filNam,mdlNam);


  lBuf = mem_cbuf1;
  lSiz = mem_cbuf1_SIZ;

  //----------------------------------------------------------
  // split file into tmp/Model_<subModelName>
  if((fpi=fopen(filNam,"r")) == NULL) {
    TX_Error("MDL_load_file_split E001 %s",filNam);
    return -1;
  }


  lNr = 0;
  while (!feof (fpi)) {

    if (fgets (lBuf, lSiz, fpi) == NULL) break;
      // printf(" lBuf-len=%ld\n",strlen(lBuf));

    UTX_CleanCR (lBuf);

    // check first 10 Lines for binary chars
    // check only first 10 chars ..
    if(lNr < 1) {
      // skip checking commentLines (haben viele Sonderzeichen)
      // if(lBuf[0] == '#') goto L_cs;
      ll = strlen(lBuf);
      for(i1=0; i1<ll; ++i1) {
        // skip tab, lf, cr
        if(lBuf[i1] == '\t') continue;  // \n & \r check with UTX_CleanCR
        if((lBuf[i1] < 32)||(lBuf[i1] > 125)) {
          // printf(" lNr=%d ch=|%c| %d\n",lNr,lBuf[i1],lBuf[i1]);
          TX_Error("Model %s is not plain text",filNam);
          return -1;
        }
      }
    }
    ++lNr;


    L_cs:
    if(!strncmp(lBuf, "MODEL ", 6)) {
      // extract old-version internal-model
      MDL_load_file_cp_mdl (lBuf, lSiz, fpi, 0);
//       Mod_aload_sm (0, lBuf, lSiz, fpi);
      continue;
    }


    if(!strncmp(lBuf, "SECTION", 7)) {
      // printf(" SecLine=|%s|\n",lBuf);

      if(!strncmp(&lBuf[7], " PTAB ", 6)) {
        MSH_aload_pTab (lBuf, lSiz, fpi);
        continue;
      }

      if(!strncmp(&lBuf[7], " MESH ", 6)) {
        MSH_aload_fTab (lBuf, lSiz, fpi);
        continue;
      }

      // copy section "APPDAT" into file <temp>/<appNam>.appdat
      //   ascii|binary
      if(!strncmp(&lBuf[7], " APPDAT ", 8)) {
        appDat_aload (lBuf, lSiz, fpi);
        continue;
      }

      // extract process -> <temp>process_<processName>
      if(!strncmp(&lBuf[7], " PROCESS ", 9)) {
        MDL_load_file_prc (lBuf, lSiz, fpi);
        continue;
      }

      // write source of subModel into file <tmpDir>/Model_<mdlNam>
      if(!strncmp(&lBuf[7], " MODEL ", 7)) {
        MDL_load_file_cp_mdl (lBuf, lSiz, fpi, 1);
//         Mod_aload_sm (1, lBuf, lSiz, fpi);
        continue;
      }

      // skip unknown section
      TX_Print ("***** skip unknown section %s",lBuf);
      while (!feof (fpi)) {
         if (fgets (lBuf, lSiz, fpi) == NULL) break;
                         // 1234567890
         if(!strncmp(lBuf, "SECTIONEND", 10)) break;
      }

    }


    // open Mainmodel
    if(fpo == NULL) {
      sprintf(cbuf, "%sModel_%s",AP_get_tmp_dir(),mdlNam);
        // printf(" mainmodel=|%s|\n",cbuf);
      if((fpo=fopen(cbuf,"w")) == NULL) {
        TX_Error("MDL_load_file_split E004 %s",cbuf);
        return -1;
      }
    }

    // Zeile raus ..
      // printf(" out=|%s|\n",lBuf);
    fprintf(fpo,"%s\n",lBuf);

  }

  fclose(fpi);
  if(fpo) fclose(fpo);


  L_exit:
    // printf("ex-MDL_load_file_split \n");
  return 0;

}


//========================================================================
  int MDL_load_file_cp_mdl (char *lBuf, int bufSiz, FILE *fpi, int mode) {
//========================================================================
// MDL_load_file_cp_mdl    split file - copy subModel into file tmp/Model_<mNam>
//   save submodel > tmp/Model_<mNam>
// Input:
//   lBuf    inputline according mode
//   bufSiz  size of lBuf
//   fpi     lBuf has been read ..
//   mode    0 = old-version; "MODEL <mNam>"  ..\n "MODEL END"
//           1 = new Version; "SECTION MODEL <mNam>"  ..\n "SECTIONEND"
//
//   open new file tmp/Model_<mNam>
//   copy all following lines into this file -
//   stop (do not copy) line starting with "SECTIONEND" | "MODEL END"

  static char s0End[]="MODEL END";
  static char s1End[]="SECTIONEND";

  int     namPos;
  char    safNam[SIZMFTot], fNam[320], *txEnd;
  FILE    *fpo;


  // printf("MDL_load_file_cp_mdl |%s| %d\n",lBuf,mode);


  if(mode == 0) {
    // "MODEL <modelname>" -.. "MODEL END"
    //  0123456
    namPos = 6;
    txEnd = s0End;

  } else {
    // "SECTION MODEL modelname" .. "SECTIONEND"
    //  012345678901234
    namPos = 14;
    txEnd = s1End;
  }

  // get safe modelname
  strcpy(safNam, &lBuf[namPos]);
  UTX_safeName (safNam, 1);
  sprintf(fNam, "%sModel_%s", AP_get_tmp_dir(), safNam);
    // printf("Mod_aload_sm |%s|\n",fNam);

  if((fpo=fopen(fNam,"w")) == NULL) {
    TX_Error("MDL_load_file_cp_mdl E001 %s",fNam);
    return -1;
  }


  // copy until "<txEnd>"
  while (!feof (fpi)) {
    if (fgets (lBuf, bufSiz, fpi) == NULL) break;
    UTX_CleanCR (lBuf);
    if(!strcmp(lBuf, txEnd)) break;
    fprintf(fpo,"%s\n",lBuf);
    // ++i1;
  }
    // printf(" i1=%d\n",i1);

  fclose (fpo);

    // printf(" ex-MDL_load_file_cp_mdl |%s|\n",fNam);
    // exit(0);

  return 0;

}


//================================================================
  int MDL_load_file_prc (char *lBuf, int bufSiz, FILE *fpi) {
//================================================================
/// \code
/// read from fpi and write until SECTIONEND into file tmp/Model_<mdlNam>
/// lBuf is line "SECTION PROCESS process_<prcNam>"
///               01234567890123456
/// add proc to file <tmp>processes.lst
/// \endcode

  char    s1[256], s2[256], *p1;
  FILE    *fpo;


  // printf("MDL_load_file_prc |%s|\n",lBuf);

  // extract processname
  strcpy(s1, &lBuf[16]);
  // p1 = strchr(s1, ' ');
  // if(p1) *p1 = '\0';
  // else {TX_Error("MDL_load_file_prc E001\n"); return -1;}

  // sprintf(s2, "%sprocess_%s", AP_get_tmp_dir(), s1);
  sprintf(s2, "%s%s", AP_get_tmp_dir(), s1);
    // printf(" extract proc |%s|\n",s2);



  if((fpo=fopen(s2, "w")) == NULL) {
    TX_Error("MDL_load_file_prc E002 %s", s2);
    return -1;
  }


  // add process to file  processes.lst
  sprintf(s2, "%sprocesses.lst", AP_get_tmp_dir());
  UTX_fsavLine (s1, s2, 80, -1);



  L_nxt:
    if (fgets (lBuf, bufSiz, fpi) == NULL) goto L_exit;
    UTX_CleanCR (lBuf);
    if(!strcmp(lBuf, sSecEnd)) goto L_exit;
    fprintf(fpo,"%s\n",lBuf);
    if(!feof (fpi)) goto L_nxt;


  L_exit:
  fclose (fpo);

  return 0;

}





/* UU
//================================================================
  int MDL_load_add (char *symNam, int lifespan) {
//================================================================
// MDL_load_add        model import, add to modMap
//   load additional submodel - recursive 
//   get via MDL_load_scan__ models not yet loaded into mdl_smLst
//   add mdl_smLst (models not yet loaded) to loadMap mdl_ldLst
// Input:
//   symNam    symbolic-modelname; load this model
//   lifespan  OBJSTAT_perm=0, OBJSTAT_temp=1;
// Output:
//   loadMap mdl_ldLst


//             AP_modact_nam = name of parentModel; for mainModel "";

  int       irc, i1, ilm, iNam;
  char      safNam[SIZMFTot]; // safSm[SIZMFTot], symSm[SIZMFTot], s1[320];
  TxtTab    smNames = _UTXTAB_NUL;
//   FILE      *fp1;


  printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA \n");
  printf("MDL_load_add |%s| %d\n",symNam,lifespan);
  MDL_dump__ ("MDL_add__");             // dump loadMap
  // DB_dump_ModBas ("MDL_load_add"); // dump baseModels
  // DB_dump_ModRef ("\MDL_load_add");


  //----------------------------------------------------------------
  // test if model already loaded
// /
//   // test if model already loaded
//   roi = MDL_roi_symMn (symNam);
//   if(roi >= 0) {
//     // test if temp-model; yes: remove all temp-bits.
//     if(mdRoots[roi].lifespan) MDL_prev_temp ();
//     goto L_exit;
//   }
// /


  // get index last rec in loadMap
  ilm = mdl_ldm.rNr - 1;


  //----------------------------------------------------------------
  L_nxt_1:
      printf(" _load_add-L_nxt_1: %s\n",symNam);

    irc = MDL_safename (safNam, symNam);      // get safNam
    if(irc < 0) goto L_exit;

    // import tmp/Model_<safNam>
    // create list of subModels (write file tmp/Mod_<safNam>.lst)
    // add symNam and its subModel to loadmap mdl_ldm
    // import and scan
    irc = MDL_load_scan__ (&smNames, symNam, safNam);
    if(irc < 0) goto L_exit;


    //----------------------------------------------------------------
    // scan subModels of newly added models (recurs)
    L_nxt_2:
      ++ilm;
        printf(" _load_add-L_nxt_2: ilm=%d mdl_ldm.rNr=%d\n",ilm,mdl_ldm.rNr);
      if(mdl_ldm.rNr <= ilm) goto L_exit;   // no new subModels added

      // get nameIndex of new loadMap-record
      iNam = mdl_ldm.data[ilm];
      if(iNam < 0) {TX_Print("***** MDL_load_add E001"); irc = -1; goto L_exit;}
      // get name of new loadMap-record
      symNam = UtxTab__(iNam, &mdl_names);
        printf(" _load_add-ilm=%d iNam=%d nam |%s|\n",ilm,iNam,symNam);

      // test if model already exists or must be imported and scanned
      if(MDL_STAT_INAM(iNam) < 1) goto L_nxt_1;

      // add all models in tmp/Mod_<symNam>.lst to loadmap
      irc = MDL_ldm_add_flst (symNam);
      if(irc < 0) goto L_exit;

      // continue ..
      goto L_nxt_2;


  //----------------------------------------------------------------
  L_exit:
    UtxTab_free (&smNames);
  

    // TESTBLOCK
    printf("ex-MDL_load_add &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
    MDL_dump__ ("\nex-MDL_add__");             // dump loadMap
    // exit(MSG_ERR__ (ERR_USER_ABORT, "testexit"));
    printf("ex-MDL_load_add &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
    // exit(0);
    // END TESTBLOCK

  return 0;

}
*/

//================================================================
  int MDL_load_scan_prim () {
//================================================================
// MDL_load_scan_prim          create list of subModels of primary-model

  int       irc;
  char      *pmn;
  TxtTab    smNames = _UTXTAB_NUL;


  // get modelname of primary-model
  pmn = MDL_mNam_prim ();

  // printf("MDL_load_scan_prim |%s|\n",pmn);


  // scan modelfile for submodels; add submodels to mdl_tab and loadmap
  // - create file tmp/Mod_<safMnam>.lst
  irc = MDL_load_scan__ (&smNames, pmn, 1);

    UtxTab_free (&smNames);

  return irc;

}


//=================================================================
  int MDL_load_scan__ (TxtTab *smNames, char *symNam, int reScan) {
//=================================================================
// MDL_load_scan__             create list of subModels (tmp/Mod_<safMnam>.lst)
// - add symNam to loadmap
// - import (MDL_load_import__) write file tmp/Model_<safNam>
// - scan file tmp/Model_<safMnam> for subModelnames
// - add subModelnames into file tmp/Model_<safNam>.lst
// - add subModelnames to mdl_ldm  (loadmap)
//
// Input:
//   symNam       symbolic-modelname; scan this model
//   reScan       0 do not scan (again) if mStat > 0
//                1 scan again
// Output:
//   smNames      list of subModels used in model <symNam>
//   retCode      0=OK, -1=err.
//                mdl_smLst returns list of subModelnames:
//
// was MDLMP_add_mnam_scan


  int       irc, ii, lSiz, mTyp, mbTyp, inm;
  long      l1;
  char      subNam[SIZMFTot], safNam[SIZMFTot], s1[320], *lBuf, *p1;
  FILE      *fpi, *fpo = NULL;


  // printf("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS \n");
  // printf("MDL_load_scan__ |%s| %d\n",symNam,reScan);
  // MDL_dump__ ("MDL_load_scan__");

  lBuf = mem_cbuf1;
  lSiz = mem_cbuf1_SIZ;


  // get modeltype from symNam
  mTyp = MDL_mTyp_mNam (symNam);
    // printf(" _load_scan |%s| mTyp=%d\n",symNam,mTyp);

  // skip mockups and images
  if(MDL_MTYP_IS_IMG_MOCK(mTyp)) {irc = 0; goto L_exit;}
//   if(mTyp >= Mtyp_BMP) {irc = 0; goto L_exit;}


  //----------------------------------------------------------------
  // first add myself to loadmap.

  // get nameIndex in mdl_names
  inm = MDL_INAM_MNAM (symNam);
  if(inm < 0) {TX_Print("***** MDL_load_scan__ E000 %s",symNam);irc= 1;goto L_exit;}

//   // add nameIndex to loadMap mdl_ldm
//   if(inm != 0) {   // do not add primary model to loadmap
//     irc = MDL_ldm_add__ (inm);
//     if(irc < 0) goto L_exit;       // ERR
//   }

  // test if model already scanned (mdl_tab[inm].stat >= 1)
  if(!reScan) {
    if(MDL_STAT_INAM(inm) >= 1) goto L_exit;
  }


  //----------------------------------------------------------------
  // get fnSub = filename for new subModel mb->mnam
  irc = MDL_mNam_safe (safNam, symNam);
  if(irc) {irc = -1; goto L_exit;}

  // open infile
  sprintf(lBuf, "%sModel_%s", AP_get_tmp_dir(),safNam);
    // printf(" _load_scan__-fopen1 %s\n",lBuf);
  if(fpi=fopen(lBuf, "r")) goto L_scan;

  // no modelfile exists yet - must import

  // MBTYP_INTERN - exist from split main - 
  // if(mTyp == MBTYP_INTERN) goto L_scan;

  // images and mockups
  if(mTyp >= Mtyp_TESS) goto L_done;
  if(mTyp == Mtyp_3DS) goto L_done;
  if(mTyp == Mtyp_LWO) goto L_done;



  //----------------------------------------------------------------
  // model import
  irc = MDL_load_import__ (mTyp, symNam);
  if(irc < 0) {
    TX_Error("***** MDL_load_scan__ E1 %s",symNam);
    irc = -3;
    goto L_exit;
  }

  sprintf(lBuf, "%sModel_%s", AP_get_tmp_dir(),safNam);
    // printf(" _load_scan__-fopen2 %s\n",lBuf);

  if((fpi=fopen(lBuf, "r")) == NULL) {
    TX_Error("***** MDL_load_scan__ E2 %s",symNam);
    irc = -3;
    goto L_exit;
  }


  //----------------------------------------------------------------
  // modelfile exists
  L_scan:
  UtxTab_init__ (smNames);            // init (malloc ..)


  // skip until :DYNAMIC_DATA
  MDL_file_skip_DYNDAT (fpi);

  // read source of Model_<mNam>; add all its subModels into smNames
  while (!feof (fpi)) {
    if(fgets (lBuf, lSiz, fpi) == NULL) break;
    p1 = lBuf;
       // printf(" ........nxt ln |%s|\n",p1);

    while (*p1  == ' ')  ++p1;
    if(*p1 != 'M') continue;
    ++p1;
    if(!isdigit(*p1)) continue;

    // get mbTyp = modeltype and subNam = modelname (symb.)
    mbTyp = MDL_nam__srcLn (subNam, lBuf);
    if(mbTyp <= MBTYP_DUP) {
      if(mbTyp == MBTYP_DUP) continue;  // ignore duplicate smNam eg "M21="M20").."
      TX_Print("***** MDL_load_scan__ E3 %s",subNam);
      continue;
    }
      // printf(" _load_scan-mbTyp=%d sub|%s| of |%s|\n",mbTyp,subNam,symNam);

    //......................................
    // skip images (dyn. load at disp)
    if(mbTyp >= Mtyp_BMP) continue;

    //......................................
    // mockups have no childs - do not add to loadmap, create baseModel
    if(mbTyp >= Mtyp_TESS) {
      // get or store subNam in mdl_names
      ii = MDL_mdl_add (mbTyp, subNam);
      if(ii < 0) {TX_Print("***** MDL_load_scan__ E8"); irc = -1; goto L_done;}
      // create baseModel
      DB_StoreModBas (mbTyp, UtxTab__(ii, &mdl_names));
      // add  also to list of used submodels
      goto L_add_sm;
    }

    //......................................
    // native and cad-model - 
    // add new name uniq to mdl_names and mdl_tab;
    inm = MDL_mdl_add (mbTyp, subNam);
    if(inm < 0) {TX_Print("***** MDL_load_scan__ E4 %s",subNam); goto L_done;}


//     //......................................
//     // add name to loadMap mdl_ldm
//     // subNam is CTLG|intern|extern;
//     // test if last record of loadMap is <inm>
//     irc = MDL_ldm_add__ (inm);
//     if(irc) {
//       if(irc < 0) goto L_done;  // ERR
//       goto L_add_sm;            // is last entry in loadmap
//     }

    //......................................
    // add new name uniq to smNames = list of subModels (later file Mod_<sm>.lst)
    L_add_sm:
    irc = UtxTab_add_uniq__ (smNames, subNam);
    if(irc < 0) {TX_Print("***** MDL_load_scan__ E6 %s",subNam); goto L_done;}

  }
  fclose(fpi);
    // printf(" _load_scan-mdl_smNames.iNr=%d\n",smNames->iNr);


  //----------------------------------------------------------------
  // write all subModels into file tmp/Mod_<actMdl>.lst
  if(smNames->iNr < 1) goto L_done;
  sprintf(s1, "%sMod_%s.lst", AP_get_tmp_dir(),safNam);
  if((fpo=fopen(s1, "w")) == NULL) {
    TX_Print("***** MDL_load_scan__ E7 %s",s1);
    irc = -4;
    goto L_exit;
  }
  for(ii=0; ii < smNames->iNr; ++ii)
    fprintf (fpo, "%s\n",UtxTab__(ii, smNames));
      // printf(" |%s|\n",UtxTab__(ii,smNames));
  fclose(fpo);


  //----------------------------------------------------------------
  L_done:
//     // set stat of symNam to 1 (imported, tmp/Mod_<safNam>.lst created
//     irc = MDL_stat_set (symNam, 1);


  //----------------------------------------------------------------
  L_exit:
    // UtxTab_dump (smNames, "ex-MDL_load_scan__-smNames");
    // UtxTab_dump (&mdl_names, "ex-MDL_load_scan__-mdl_names");
    // MDL_dump__ ("ex-MDL_load_scan__");
    // printf("ex-MDL_load_scan__ %d\n",irc);
    // DEB_exit();
  return irc;

}


//================================================================
  int MDL_ldm_add__ (int inm) {
//================================================================
// MDL_ldm_add__             add nameIndex to loadMap mdl_ldm
//   subNam is CTLG|intern|extern;
//   test if last record of loadMap is <inm>
// Input:
// Output:
//   retCode      0=OK, -1=err; 1=ident-last-entry


  int     irc, *ia, i1, i2, iNr;
  long    l1;

  // printf("MDL_ldm_add__ %d\n",inm);
  // MDL_dump__ ("MDL_ldm_add__");


  ia = mdl_ldm.data;

  // skip if last entry of loadmap == inm;
  if(mdl_ldm.rNr) {
      // printf(" _ldm_add-inm=%d last=%d\n",inm,mdl_ldm.data[mdl_ldm.rNr-1]);
    // skip second entry
    if(mdl_ldm.data[mdl_ldm.rNr -1] == inm) { irc = 1; goto L_exit;}

  } else goto L_add;

  // check if inm already defined
  for(i1=0; i1<mdl_ldm.rNr; ++i1) {
    if(ia[i1] != inm) continue;

    // remove record i1 from mdl_ldm
    MemTab_del (&mdl_ldm, i1, 1);
    break;
  }

  //----------------------------------------------------------------
  L_add:
    // add at end
    irc = MemTab_add (&mdl_ldm, &l1, &inm, 1, 0);
    if(irc < 0) TX_Print("***** MDL_ldm_add__ EOM");

  //----------------------------------------------------------------
  L_exit:
    // MDL_dump_ldm ("ex-MDL_ldm_add__");
    // printf("ex-MDL_ldm_add__ %d\n",irc);
  return irc;

}


//================================================================
  int MDL_ldm_add_flst (char *symNam) {
//================================================================
// MDL_ldm_add_flst         add all models in tmp/Mod_<symNam>.lst to loadmap
//
// Input:
// Output:
//   retCode      0=OK, -1=err.


  int     irc, iNam;
  char    safNam[SIZMFTot], safSm[SIZMFTot], symSm[SIZMFTot], s1[320];
  FILE    *fp1;


  // printf("MDL_ldm_add_flst |%s|\n",symNam);

  MDL_safename (safNam, symNam);      // get safNam

  // open list of submodels, add all lines to loadmap
  sprintf (s1, "%sMod_%s.lst", AP_get_tmp_dir(),safNam);
  if((fp1=fopen(s1,"r")) == NULL) {
    // model symNam does not have subModels; continue ..
    irc = 0;
    goto L_exit;
  }

  // check model symNam for subModels
  while (!feof (fp1)) {
    if(fgets (symSm, sizeof(symSm), fp1) == NULL) break;
    UTX_CleanCR (symSm);
    MDL_mNam_safe (safSm, symSm);

    // symSm/safSm is a subModel of symNam - get nameIndex iNam
    iNam = UtxTab_find (symSm, &mdl_names);
    if(iNam < 0) {TX_Print("***** MDL_load_add E001"); goto L_err2;}

    // add model iNam to loadmap
    irc = MDL_ldm_add__ (iNam);
    if(irc < 0) {TX_Print("***** MDL_load_add E002"); goto L_err2;}

    // add all childs of model iNam to loadmap (recursion)
    irc = MDL_ldm_add_flst (symSm);
    if(irc < 0) {TX_Print("***** MDL_load_add E003"); goto L_err2;}
  }
  fclose(fp1);


  //----------------------------------------------------------------
  L_exit:
    // printf("ex-MDL_ldm_add_flst %d\n",irc);
  return irc;

  L_err1:
    irc = -1;
    goto L_exit;

  L_err2:
    fclose(fp1);
    goto L_err1;

}


//================================================================
  int MDL_stat_mNam (char *mNam) {
//================================================================
// MDL_stat_mNam           get stat from model-name

// returns     2   model is imported and loaded
//             1   model is imported - ready to load
//             0   modelname stored, no baseModel exists yet
//            -1   model-unused
//            -2   model-deleted
//            -3   modelname unknown 


  int    mStat;
  int    imn;        // index modelname

  // get index-modelName from modelName
  imn = MDL_INAM_MNAM (mNam);
  if(imn < 0) {mStat = -3; goto L_exit;}

  // get stat from name-index
  mStat = MDL_STAT_INAM (imn);

  L_exit:
      // printf(" ex-MDL_stat_mNam %d |%s|\n",mStat,mNam);

  return mStat;

}


/*
//================================================================
  int MDL_stat_set (char *mNam, int stat) {
//================================================================
// MDL_stat_set             set stat for model in mdl_tab
// Input:
// Output:
//   retCode      inm


  int     inm;

  printf("MDL_stat_set \n");

  // get inm = index in mdl_tab
  inm = MDL_INAM_MNAM (mNam);
  if(inm < 0) {TX_Print("***** MDL_stat_set E1 %s",mNam); return -1;}

  // set
  mdl_tab.data[inm].mStat = stat;

  return inm;

}


//================================================================
  int MDL_stat_upd () {
//================================================================
// MDL_stat_upd            update mdl_tab[].stat
// Input:
// Output:
//   retCode      0=OK, -1=err.
// was ..


  int       i1, *ia;
  stru_tab  *tab, *tab1;


  printf("MDL_stat_upd \n");

  tab = MEMTAB_DAT (&mdl_tab);

  // reset all stat in mdl_tab if > 1 to 1
  for(i1=0; i1<mdl_tab.rNr; ++i1) {
    tab1 = &tab[i1];
    if(tab1->stat > 1) tab1->mStat = 1;
  }


  // set all stat of models in loadmap to 2
  ia = mdl_ldm.data;
  for(i1=0; i1<mdl_ldm.rNr; ++i1) {
    tab1 = &tab[ia[i1]];
    tab1->mStat = 2;
  }


  L_exit:
    MDL_dump__ ("ex-MDL_stat_upd");
    printf("ex-MDL_stat_upd\n");
  return 0;

}


// UNUSED
//================================================================
  int MDL_mdl_smuu () {
//================================================================
// MDL_mdl_smuu            add unused-subModels to mdl_names, mdl_tab
//   add all models not yet defined in mdl_tab to mdl_names / mdl_tab
// Input:
// Output:
//   retCode      0=OK, -1=err.
//
// see MDL_smuu_lst

  int        i1, ii, *ia;
  char       *p1, s1[SIZMFTot];
  // stru_tab   *tab, *tab1;
  UtxTab_NEW (mnAll);

  printf("MDL_mdl_smuu \n");

  // get mdlTab = list of existing-subModels
  MDL_lst_all (&mnAll);  // safe-names with "Model_<mnam>"


  //----------------------------------------------------------------
  // set int-array stat parallel to mnAll; 0=unused, 1=used
  ia = (int*) MEM_alloc_tmp (sizeof(int) * mnAll.iNr);
  for(i1=0; i1<mnAll.iNr; ++i1) ia[i1] = 0;


  // loop tru mdl_tab - set model used in ia
  for(i1=0; i1<mdl_tab.rNr; ++i1) {
    p1 = UtxTab__(i1, &mdl_names);
    sprintf(s1,"Model_%s",p1);
    UTX_safeName (s1, 1);
    // find s1 in list existing-subModels; set used
    ii = UtxTab_find (s1, &mnAll);
      printf(" _lst_smuu_find %d |%s|\n",ii,p1);
    if(ii >= 0) ia[ii] = 1;
  }

  // add all unused models in ia to mdl_names / mdl_tab
  for(i1=0; i1<mnAll.iNr; ++i1) {
    if(ia[i1]) continue;
    p1 = UtxTab__(i1, &mnAll);
    MDL_mdl_add (MBTYP_INTERN, &p1[6]);
  }



  L_exit:
    MDL_dump__ ("ex-MDL_mdl_smuu");
    printf("ex-MDL_mdl_smuu\n");
  return 0;

}
*/


//================================================================
  int MDL_mdl_add (int mtyp, char *symNam) {
//================================================================
// MDL_mdl_add             add new name uniq to mdl_names, mdl_tab;
// Input:
// Output:
//   retCode      >=0  index mdl_names for new or existing model
//                -1   Err.
// was ..


  int       irc, i1, inm, *itxa;
  long      l1;
  stru_tab  rTab = _TAB_NUL;


  // printf("MDL_mdl_add %d |%s|\n",mtyp,symNam);

  itxa = mdl_names.ind;


  // check if string <symNam> already exists;
  for(i1=0; i1<mdl_names.iNr; ++i1) {
    if(!strcmp(symNam, &mdl_names.tab[itxa[i1]])) {
      inm = i1;
      // if mStat=mdlStat_deleted  then set mStat=mdlStat_passive
      //   and mUsed=mdlStat_passive
      if(MDL_STAT_INAM(i1) == mdlStat_deleted) {
        MDL_STAT_SET_INAM (i1, mdlStat_passive);
        MDL_USED_SET_INAM (i1, mdlStat_passive);
      }
      goto L_exit;    // alread exists 
    }
  }

  // add new string ..
  inm = UtxTab_add (&mdl_names, symNam);
  if(inm < 0) {TX_Error("MDL_mdl_add EOM mdl_names"); return -1;}
  
  // check space in mdl_tab
  if(mdl_tab.rMax <= inm) MemTab_add (&mdl_tab, &l1, NULL, 200, 1);

  // add mTyp in mdl_tab at pos inm
  rTab.mTyp = mtyp;

  mdl_tab.rNr = inm;
  MemTab_add (&mdl_tab, &l1, &rTab, 1, 0);


  //----------------------------------------------------------------
  L_exit:
    // MDL_dump__ ("ex-MDL_mdl_add");
    // printf("ex-MDL_mdl_add %d\n",inm);
  return inm;

}


//================================================================
  int MDL_mdl_del (char *symNam) {
//================================================================
// MDL_mdl_del             set mdl_tab-record deleted

  int      irc, i1, *itxa;


  // printf("MDL_mdl_del %s\n",symNam);

  itxa = mdl_names.ind;

  // check if string <symNam> already exists;
  for(i1=0; i1<mdl_names.iNr; ++i1) {
    if(!strcmp(symNam, &mdl_names.tab[itxa[i1]])) {
      MDL_STAT_SET_INAM (i1, mdlStat_deleted);
      irc = i1;
      goto L_exit;    // alread exists 
    }
  }

  irc = -1;

  L_exit:
  return irc;

}


//================================================================
  int MDL_load_import__ (int mTyp, char *symNam) {
//================================================================
// MDL_load_import__         model import (write file tmp/Model_<safMnam>)
// Input:
//   typ      MBTYP_EXTERN (native)
//   mnam     modelname, symbolic, eg "Data_U/3dface.dxf"
// Output:
//   retCode      0=OK, -1=err.
//
// was MDLMP_add__ Mod_load_import
// see MDL_imp__


  int     irc;
  char    fn[320];


  // printf("MDL_load_import__ %d |%s|\n",mTyp,symNam);


  //----------------------------------------------------------------
  // intern model
  if(mTyp == MBTYP_INTERN) {
    // check file existence tmp/Model_<mNam>
    sprintf(fn, "%sModel_%s", AP_get_tmp_dir(), symNam);
    if(OS_checkFilExist(fn, 1)) {irc = 0; goto L_exit;}
    irc = -1;
    goto L_exit;
  }


  //----------------------------------------------------------------
  // CTLG-Part
  if(mTyp == MBTYP_CATALOG) {
    irc = MDL_load_import_ctlg (symNam);
    goto L_exit;
  }


  //----------------------------------------------------------------
  // mockups and images
  if(mTyp >= Mtyp_TESS)  {
    // mockup-file (.wrl, .stl, .obj)
    // load at display-time with GR_mdMock_imp
    irc = 0;
    goto L_exit;
  }


  //----------------------------------------------------------------
  // import as file tmp/Model_ (import gcad,dxf, stp, igs, ..)
  irc = MDL_load_import_ext (mTyp, symNam);
  if(irc < 0) goto L_exit;


  //----------------------------------------------------------------
  L_exit:
    // printf("ex-MDL_load_import__ %d\n",irc);
    // DEB_exit();
  return irc;

}


//================================================================
  int MDL_load_import_ctlg (char *symNam) {
//================================================================
// MDL_load_import_ctlg    import catalogPart as file tmp/Model_<ctlgPart>

  int     irc;
  char    safNam[SIZMFTot], fnMdl[320], fnDat[320], fno[320], s1[SIZMFTot];
  stru_FN ofn;


  // printf("MDL_load_import_ctlg |%s|\n",symNam);

  // resolv symNam
  irc = MDLFN_oFn_fNam (&ofn, symNam);
  if(irc < 0) goto L_exit;

  // get safNam = filename for new subModel mb->mnam
  irc = MDL_mNam_safe (safNam, symNam);
  if(irc) goto L_exit;


  //----------------------------------------------------------------
  // catalog-part; create parametric-data-file tmp/<catPart>.ctlg_dat

    // create filename tmp/<catPart>.dat - eg "Profile_6x4_150_ctlg.dat"
    sprintf(fnDat, "%s%s.dat",AP_get_tmp_dir(),safNam);
      // printf(" _import_ctlg-fnDat=|%s|\n",fnDat);

    // check if exists; else create
    if(OS_checkFilExist(fnDat, 1)) goto L_exit;

    // create the catalog-part (the parameter - file)
    sprintf(s1, "\"%s\"",symNam);
    irc = CTLG_Part_Ref1 (fnMdl, fnDat, &ofn);
    if(irc < 0) goto L_exit;
      // printf(" _import_ctlg- |%s|%s|\n",fnMdl,fnDat);

    // create filename tmp/Model_<safNam>
    CTLG_fnMdl_safPartNam (fno, safNam);

    // join fnMdl to "CALL CTLG", fnDat complete, fnMdl to end.
    irc = UTX_fjoin_121 (fno, fnMdl, fnDat, "CALL CTLG");

 
  //----------------------------------------------------------------
  L_exit:
      // TESTBLOCK
      // printf(" ex-MDL_load_import_ctlg %d\n",irc);
      // exit(0);
      // END TESTBLOCK

    return irc;

}



//================================================================
  int MDL_load_import_attr (char *fNam, double mSiz) {
//================================================================
// MDL_load_import_attr    write attributes and model (UTF-buffer1)
// Input:
//   fNam         full filename, eg "<tmp>/Model_unknown"
// Output:
//   retCode      0=OK, -1=err.
//
// was Mod_savSubBuf1


  // int      irc;
  char     s1[256];
  FILE     *fp1;

  // printf("MDL_load_import_attr |%s|\n",fNam);
  // UTF_dump1__ ();

  if((fp1=fopen(fNam,"wb")) == NULL) {
    TX_Print("***** MDL_load_import_attr E001 |%s|",fNam);
    return -1;
  }

  if(mSiz > 0) {
    sprintf(s1, "MODSIZ %f\n",mSiz);
    fputs(s1, fp1);
  }



  // write out the PermanentAttributes - HIDE, G#, SSTYLS ..
  // do not check if att exists (error with STEP-Import)
  GA_fil_wri (fp1, 0, 1, 0);

  fputs(":DYNAMIC_DATA\n", fp1);

  // write code (UTF-buffer1)
  UTF_file_Buf1_w (fp1);

  fclose(fp1);


  L_exit:
    // printf("ex-MDL_load_import_attr\n");
  return 0;

}


//================================================================
  int MDL_load_unl () {
//================================================================
// MDL_load_unl          unload  dll_imp dll_exp

  DLL_dyn__ (&dll_imp, DLL_UNLOAD, NULL);
  DLL_dyn__ (&dll_exp, DLL_UNLOAD, NULL);

  return 0;

}


//================================================================
  int MDL_load_import_mock (char *dllnam, void *data) {
//================================================================
// MDL_load_import_mock           import mockup

    // load dll, exec "gCad_main"
    return AP_kex_exec (&dll_imp, dllnam, "gCad_main", data, DLL_LOAD_EXEC);

}


//================================================================
  int MDL_load_import_ext (int mTyp, char *mnam) {
//================================================================
// MDL_load_import_ext             import as file tmp/Model_ gcad,dxf,stp,igs ..
// Input:
//   typ      MBTYP_EXTERN (native)
//   mnam     modelname, symbolic, eg "Data_U/3dface.dxf"
// Output:
//   retCode      0=OK, -1=err.
//
// was Mod_load_import AP_Mod_load__
// see also (unused) MDL_imp__


  extern int DL_wri_dynDat0 (FILE*);

  int        irc, dbResiz;
  long       lTab[8];
  char       fnSub[SIZMFTot], fNam[SIZFNam], fno[SIZFNam];
  void       *pa[2];
  stru_FN    ofn;


  printf("MDL_load_import_ext %d |%s|\n",mTyp,mnam);

  // resolv fileName
  irc = MDLFN_oFn_fNam (&ofn, mnam);
  if(irc < 0) goto L_exit;
    // MDLFN_dump_ofn (&ofn, "MDL_load_import_ext-1");

  // get fNam = full filename for mnam
  MDLFN_ffNam_oFn (fNam, &ofn);
    // printf(" MDL_load_import_ext-fNam |%s|\n",fNam);


  // Test if file exists
  irc = OS_checkFilExist (fNam, 2);  // 1=yes, 0=no
    // printf(" f.OS_checkFilExist %d\n",irc);
  if(!irc) {irc = -2; goto L_exit;}

  // get fnSub = filename for new subModel mb->mnam
  irc = MDL_mNam_safe (fnSub, mnam);
  if(irc) goto L_exit;

  sprintf(fno, "%sModel_%s",AP_get_tmp_dir(),fnSub);
    // printf(" _load_import_ext-fno=|%s|\n",fno);



  //----------------------------------------------------------------
  // for load native external model as subModel
  if(mTyp != MBTYP_EXTERN) goto L_Mtyp_DXF;

    // test if mainModel is beeing loaded
    if(!strcmp(mnam, MDL_mNam_iNam(0))) {
      // load mainmodel: copy modelfile into tmp as file tmp/Mod_.mod_in
      sprintf(fno, "%sMod_.mod_in", AP_get_tmp_dir());
      OS_file_copy (fNam, fno);
      strcpy(fNam, fno);
    }

    // get all subModelfiles into tmpDir (split file fNam)
    irc = MDL_load_file_split (fNam, fnSub);
    if(irc < 0) return irc;
    goto L_exit;


  //----------------------------------------------------------------
  L_Mtyp_DXF:
    if(mTyp != Mtyp_DXF) goto L_Mtyp_Iges;

    // create native model <tmpDir>Model_ + all subModels (Model_<smNam>)
    // - no title, no rescale;
    pa[0] = fNam;
    pa[1] = fno;
    // irc = OS_dll_do ("xa_dxf_r", "DXF_r__", pa, 0);
    irc =  AP_kex_exec (&dll_imp, "xa_dxf_r", "DXF_r__", pa, DLL_LOAD_EXEC_UNLOAD);
      // printf(" f.OS_dll_do %d |%s|\n",irc,fNam);
    if(irc < 0) {irc = -3; goto L_exit;}
    goto L_exit;


  //----------------------------------------------------------------
  L_Mtyp_Iges:
    if(mTyp != Mtyp_Iges) goto L_Mtyp_Step;

    // create native model <tmpDir>Model_ + all subModels (Model_<smNam>)
    pa[0] = fNam;
    pa[1] = fno;
    // irc = OS_dll_do ("xa_ige_r", "IGE_r__", pa, 0);
    irc = AP_kex_exec (&dll_imp, "xa_ige_r", "IGE_r__", pa, DLL_LOAD_EXEC_UNLOAD);
      // printf(" f.OS_dll_do %d |%s|\n",irc,fNam);
    if(irc < 0) {irc = -3; goto L_exit;}
    AP_stru_2_txt (NULL, 0, (void*)lTab, 1L); // ask last index
    DB_size_set (lTab);                       // increase DB-size
    dbResiz = 1;                              // DB-resize done
    goto L_exit;


  //----------------------------------------------------------------
  L_Mtyp_Step:
    if(mTyp != Mtyp_Step) goto L_Mtyp_3DS;

    // create native model <tmpDir>Model_ + all subModels (Model_<smNam>)
    // - no title, no rescale;
    pa[0] = fNam;
    pa[1] = fnSub;
    // irc = OS_dll_do ("xa_stp_r", "STP_r__", pa, 0);
    irc = AP_kex_exec (&dll_imp, "xa_stp_r", "STP_r__", pa, DLL_LOAD_EXEC_UNLOAD);
      // printf(" f.OS_dll_do %d |%s|\n",irc,fNam);
    if(irc < 0) {irc = -3; goto L_exit;}
    goto L_exit;


    //----------------------------------------------------------------
  L_Mtyp_3DS:
    if(mTyp != Mtyp_3DS) goto L_Mtyp_LWO;
      AP_Import3ds (0, "0,0,0", 1., fNam);
      goto L_mem2file;


    //----------------------------------------------------------------
  L_Mtyp_LWO:
    if(mTyp != Mtyp_LWO) goto L_Mtyp_LXML;
      AP_ImportLwo (0, "0,0,0", 1., fNam);
      goto L_mem2file;


    //----------------------------------------------------------------
  L_Mtyp_LXML:
    if(mTyp != Mtyp_XML) goto L_unSupp;
    // sprintf(fno, "%s%s.gcad",AP_get_tmp_dir(),fnSub);
    // sprintf(fno, "%simport.gcad",AP_get_tmp_dir());
    // printf(" _load_import_ext-lxml-fno=|%s|\n",fno);
    pa[0] = fNam;
    pa[1] = fno;
    // irc = OS_dll_do ("xa_lxml_r", "LXML_r__", pa, 0);
    irc = AP_kex_exec (&dll_imp, "xa_lxml_r", "LXML_r__", pa, DLL_LOAD_EXEC_UNLOAD);
      // printf(" f.OS_dll_do %d |%s|\n",irc,fNam);
    if(irc < 0) {irc = -3; goto L_exit;}
    // AP_mdl_init (0);
    // AP_stru_2_txt (NULL, 0, (void*)lTab, 1L); // ask last index
    // DB_size_set (lTab);                       // increase DB-size
    // dbResiz = 1;                              // DB-resize done
// set mTyp -> Mtyp_Gcad
    goto L_exit;


/*
    //----------------------------------------------------------------
    case Mtyp_OBJ:
      // get ImportTyp; nativ oder Mockup. (user-checkBox)
      UI_AP (UI_FuncGet, UID_ckb_impTyp, (void*)&impTyp); // 1=Mockup, 0=native
      if(!impTyp) {
        // create native file - not tess-data.
        irc = OS_dll_do ("xa_obj_r", "obj_read__", fni, 0); // 2013-08-15
        if(irc < 0) goto L_err2;
        goto L_mem2file;
      }
*/

  //----------------------------------------------------------------
  L_unSupp:
    TX_Print("***** MDL_load_import_ext cannot load modeltype %d",mTyp);
    return -1;


  //----------------------------------------------------------------
  L_mem2file:
    // importcode is in UTF_FilBuf0; write -> tmp/Mod_<ofn->fNam>
      // UTF_dump__ ();
    irc = UTF_wri_file (fno, DL_wri_dynDat0);
    // irc = UTF_wri_file (fno, NULL);
    if(irc < 0) goto L_exit;
    // ED_Reset ();
    // ED_work_END (0);
    // if(AP_errStat_get() != 0) irc = -1;


  //----------------------------------------------------------------
  L_exit:
    // printf("ex-MDL_load_import_ext %d\n",irc);
  return irc;

}



//================================================================
  int MDL_load_load () {
//================================================================
// MDL_load_load            create baseModels, load into DL
//   - for all models in loadmap mdl_ldm
// Input:
//   mdl_ldLst        loadmap 

  int     irc;

  // printf("MDL_load_load \n");
  // MDL_dump__ ("MDL_load_load");


  //----------------------------------------------------------------
  // create all new baseModels
  irc = MDL_mdb_init__ ();
  if(irc < 0) return -1;

  //----------------------------------------------------------------
  // load all new models - in reverse order
  irc = MDL_mdb_load__ ();
  if(irc < 0) return -1;

  //----------------------------------------------------------------
  L_exit:

    // TESTBLOCK
    // printf("ex-MDL_load_add &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
    // DB_dump_ModBas ("ex-MDL_load_add"); // dump baseModels
    // DB_dump_ModRef ("\nex-MDL_load_add");
    // MDL_dump__ ("\nex-MDL_add__");             // dump loadMap
    // printf("ex-MDL_load_load &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
    // exit(0);
    // END TESTBLOCK

  return 0;

}


//================================================================
  int MDL_mdb_load__ () {
//================================================================
// MDL_mdb_load__             load all new baseModels - in reverse order
// Input:
// Output:
//   retCode      0=OK, -1=err.
// 
// was MDLMP_mdb_load_1 - Mod_load_sm..


  int     irc, ii, imn, *ia, imb;

  // printf("\nBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB \n");
  // printf("MDL_mdb_load__ \n");
  // MDL_dump__ ("MDL_mdb_load__");             // dump loadMap
  

  ia = mdl_ldm.data;

  // loop tru loadmap in reverse order 
  for(ii=mdl_ldm.rNr - 1; ii >= 0; --ii) {

    // get index mdl_tab
    imn = ia[ii];
      // printf(" _mdb_load__-ii=%d imn=%d\n",ii,imn);
    if((imn < 0)||(imn > mdl_tab.rNr))
      {TX_Print("***** MDL_mdb_load__ E1"); irc = -1; goto L_exit;}

    // get imb = baseModelIndex
    imb = mdl_tab.data[imn].mbi;
      // printf(" _mdb_load__-ii=%d imn=%d imb=%d\n",ii,imn,imb);
    if(imb < 0) {TX_Print("***** MDL_mdb_load__ E2"); irc = -1; goto L_exit;}

    // load model into DL
    irc = MDL_mdb_load_1 (imb);
    if(irc < 0) {
      TX_Print("***** MDL_mdb_load__ E1 %s",UtxTab__(imb, &mdl_names));
      irc = -1;
      goto L_exit;
    }
    mdl_tab.data[imn].mStat = 2;
    mdl_tab.data[imn].mUsed = 1;
  }
  irc = 0;

  L_exit:
    // printf("ex-MDL_mdb_load__ irc=%d\n",irc);
  return irc;

}


//================================================================
  int MDL_mdb_load_1 (int mbi) {
//================================================================
// MDL_mdb_load_1             load baseModel
// Input:
// Output:
//   retCode      0=OK, -1=err.
// 
// set AP_modact_ibm (distList.modInd = mbi) - prevents display 
// Save statPos of dispList in baseModel.DLind;
// load all objects of model into dispList;
// save nr of objects of model in baseModel.DLsiz.
// Display later by GL_set_mdr (set refModel in active-model (AP_modact_ibm = -1):
// define position, orientation, then call baseModel.DLind - (nr. baseModel.DLsiz).

  int      irc = 0;
  ModelBas *mb1;


  // printf("MDL_mdb_load_1 mbi=%d\n",mbi);

  // get mb1 = baseModel[mbi]
  mb1 = DB_get_ModBas (mbi);
  if(!mb1) {TX_Print("***** MDL_mdb_load_1 E1"); irc = -1; goto L_exit;}
    // printf(" _mdb_load_1-typ=%d |%s|\n",mb1->typ,mb1->mnam);

  // test if already loaded
  if(mb1->DLsiz > 0) goto L_exit;

  // set startPos in DL
  mb1->DLind = GL_Get_DLind();


  // skip mockups, images (load at display - GR_mdMock_imp)
  if(mb1->typ >= Mtyp_TESS) goto L_exit;

//   // set submodel-index (AP_modact_ibm >= 0 inhibits display of objects)
//   // check if this basemodel is the mainmodel -
//   // if(AP_modact_nam[0] == '\0') {
//   if(!strcmp(mb1->mnam, AP_modact_nam)) {
//     AP_modact_ibm = -1;
//   } else{
//     AP_modact_ibm = mbi;
//   }

  AP_modact_ibm = mbi;    // AP_modact_ibm >= 0 inhibits display of objects

  // load
  irc = MDL_load_sbm (mb1->typ, mb1->mnam);
  if(irc < 0) goto L_exit;

  // set nr of DL-objects for this submodel
  mb1->DLsiz = GL_Get_DLind() - mb1->DLind;

  // set Model-Origin
  // mod1->po = WC_mod_ori;
//   mb1->po = UT3D_PT_NUL;


  //----------------------------------------------------------------
  // set boxpoints for active subModel
  // test if AP_box_pm1 valid
//   if(AP_mdlbox_invalid_ck()) {
  if(DB_isFree_PT(&mb1->pb2)) {
    UT3D_box_mdl__ (&mb1->pb1, &mb1->pb2, AP_modact_ibm, 0);
//     AP_mdlbox_invalid_reset ();
//   } else {
//     mb1->pb1 = AP_box_pm1;
//     mb1->pb2 = AP_box_pm2;
  }


  L_exit:
    // printf("ex-MDL_mdb_load_1 %d\n",irc);
  return irc;

}


//================================================================
  int MDL_load_sbm (int mTyp, char *mnam) {
//================================================================
// MDL_mdb_load_sbm        load subModel
// - load intern|extern native model from file ..
//   Model <mnam> may not have subModels not yet existing.
//   Model <mnam> is processed from file; not loaded into memspc.
// Input:
//   mTyp   MBTYP_EXTERN|MBTYP_INTERN|MBTYP_CATALOG 
//   mnam   symbolic modelname
//
// was Mod_load_sm

  int     irc;
  char    fNam[320], safNam[SIZMFTot];


  // printf("MDL_load_sbm %d |%s|=========================\n",mTyp,mnam);
  // printf(" _load_sbm-TSU_mode=%d\n",TSU_get_mode());


  if(mnam == NULL) return -1;


  // TSU_mode=Store: Open subModel.
  if(TSU_get_mode() != 0) {       // 0=Normal 1=Store
    irc = TSU_exp_Open (mnam);
    if(irc < 0) return irc;
  }


  strcpy(safNam, mnam);
  UTX_safeName (safNam, 1);


  // catalog-Models:
  if(mTyp == MBTYP_CATALOG) {
    // get fNam = full filename of the modelfile
    CTLG_fnMdl_safPartNam (fNam, safNam);

  } else {
    // intern-, extern-subModel
    // irc = Mod_get_path (fNam, mnam);
    sprintf(fNam, "%sModel_%s",AP_get_tmp_dir(),safNam);
  }
    // printf(" _load_sbm-fNam=|%s|\n",fNam);

  // clear Hidelist
  GA_hide__ (-1, 0L, 0);

  // // clear DL; but do not delete basicModels and refModels
  // DB_Init (1);

  AP_mdlbox_invalid_set ();

  ED_Init ();

  // work intern or extern native model from file
  if(ED_work_file (fNam) < 0) goto L_err2;  // define file to work ..

  // must remove all refModels (err with AP_Init2)  2021-05-06
  DB_del_ModRef (0);

  // activate basic refsys
  UCS_Reset ();

  // work also  DYNAMIC_DATA 
  ED_lnr_reset ();
  ED_Run ();

  // save DB                                                2015-11-19
  DB_save__ (mnam);

    // printf("ex MDL_load_sbm |%s|========================\n",mnam);

  return 0;


  //----------------------------------------------------------------
  L_err2:
  // TX_Error("MDL_load_sbm E001: Submodel %s does not exist",mnam);
  MSG_err_1 ("NOEX_mdl", "%s", mnam);
  printf("MDL_load_sbm E001 %s\n",mnam);

  return -1;

}


//================================================================
  int MDL_mdb_init__ () {
//================================================================
// MDL_mdb_init__             create all new baseModels
// - for all loadmap-records
// Input:
//   mdl_ldLst    loadmap
// Output:
//   retCode      0=OK, -1=err.
//
// was MDLMP_mdb_init__


  int       irc=0, ii, inm;
  char      *mNam;

  // printf("MDL_mdb_init__ \n");
  // MDL_dump__ ("MDL_mdb_init__");

  // loop tru loadmap mdl_ldm
  for(ii=0; ii<mdl_ldm.rNr; ++ii) {
    inm = mdl_ldm.data[ii];
    mNam = UtxTab__(inm, &mdl_names);
    // create new baseMdl (tests also if baseMdl already exists)
    mdl_tab.data[inm].mbi = DB_StoreModBas (mdl_tab.data[inm].mTyp, mNam);
    if(mdl_tab.data[inm].mbi < 0) {
      TX_Print("***** MDL_mdb_init__ E1 %s",mNam);
    }
  }


  //----------------------------------------------------------------
  L_exit:
    // TESTBLOCK
    // DB_dump_ModBas ("ex-MDL_mdb_init__"); // dump baseModels
    // printf("ex-MDL_mdb_init__ %d\n",irc);
    // exit(0);
    // END TESTBLOCK
  return irc;

}



//================================================================
  int MDL_mdb_set (int mTyp, char *mNam) {
//================================================================
// MDL_mdb_set             create baseModel
// Input:
//   mTyp         modeltyp eg MBTYP_INTERN
//   mNam         modelname, symbolic
// Output:
//   retCode      basicModelNr, -1=err.
// was ..


  int     irc, inm;
  char    *p1;


  // printf("MDL_mdb_set %d |%s|\n",mTyp,mNam);

  // add modelname in mdl_names
  inm = MDL_mdl_add (mTyp, mNam);

  // get pointer to modelname inm
  p1 = UtxTab__(inm, &mdl_names);
  if(!p1) return -1;

  // create basicModel in DB
  irc = DB_StoreModBas (mTyp, p1);

  L_exit:
    // printf("ex-MDL_mdb_set %d\n",irc);
  return irc;

}


//================================================================
  int MDL_lst_sm_prim (TxtTab *ttb, int mode) {
//================================================================
// MDL_lst_sm_prim         make list of all used Submodels of main -> tmp/Mod.lst
// - get subModels from files tmp/Mod-<sNam>.lst
// - list is symbolic
// Input:
//   mode      0=list-main, else list active primary
// Output:
//   retCode   0=ok; -1=no-main


  int         irc, i1, ii;
  char        mNam[SIZMFTot], safNam[SIZMFTot], fn[320];
  UtxTab_NEW  (smtb);


  // printf("MDL_lst_sm_prim mode=%d AP_modact_inm=%d\n",mode,AP_modact_inm);


  if(!mode) {
    // get main
    strcpy(mNam, MDL_mNam_main ());

  } else {
    // get mNam = name of primary model
    strcpy(mNam, MDL_mNam_prim ());
  }

  // "-" no main
  if(!strcmp (mNam, "-")) {irc = -1; goto L_exit;}


  // init ttb with modelname of primary model
  UtxTab_init__ (ttb);
  UtxTab_add (ttb, mNam);
    // UtxTab_dump (ttb, "  _lst_sm_prim-init");

  ii = 0;

  //----------------------------------------------------------------
  // add recursiv all submodels in ttb to ttb
  L_nxt:
    
    // get fn = filename of list of submodels of model ttb[ii]
    strcpy(safNam, UtxTab__(ii, ttb));
    UTX_safeName (safNam, 1);
    sprintf(fn, "%sMod_%s.lst",AP_get_tmp_dir(), safNam);
      // printf("  _lst_sm_prim-fn=|%s|\n",fn);

    if(!OS_checkFilExist (fn, 1)) goto L_cont;  // may have no subModels

    // get all submodels of model ttb[ii] into smtb
    UtxTab_init__ (&smtb);
    UtxTab_add_file (&smtb, fn);

    // add all submodels of smtb uniq into ttb
    for(i1=0; i1<smtb.iNr; ++i1) UtxTab_add_uniq__ (ttb, UtxTab__(i1, &smtb));

    L_cont:
      ++ii;
      if(ii < ttb->iNr) goto L_nxt;

  irc = 0;


  //----------------------------------------------------------------
  L_exit:
    // UtxTab_dump (ttb, "ex-MDL_lst_sm_prim");
  return irc;

}


//================================================================
  int MDL_lst_sm_upd () {
//================================================================
// MDL_lst_sm_upd          update tmp/Mod_<mPrim>.lst
// - create file tmp/Mod_<mPrim>.lst from memSpc (active primary model)
// see also MDL_load_scan__ UTF_test_scan

  int         irc, mbTyp;
  long        ll, lNr;
  char        *cpl, *cps, subNam[SIZMFTot], fn[SIZFNam];
  UtxTab_NEW  (smNames);

  // printf("MDL_lst_sm_upd \n");

  UtxTab_init__ (&smNames);
  lNr = 0;


  // loop tru all codelines of memSpc (active primary model)
  L_nxt:
    ++lNr;
    // get cpl = next line ..
    cpl = UTF_GetPosLnr (&ll, lNr); // get startPos and lineLength
    if(!cpl) goto L_exit;

    cps = cpl;
       // printf(" ........nxt ln |");UTX_dump_cnl (cps, 100);printf("\n");

    while (*cps  == ' ')  ++cps;
    if(*cps != 'M') goto L_nxt;
    ++cps;
    if(!isdigit(*cps)) goto L_nxt;

    // get mbTyp = modeltype and subNam = modelname (symb.)
    mbTyp = MDL_nam__srcLn (subNam, cpl);
    if(mbTyp <= MBTYP_DUP) {
      if(mbTyp == MBTYP_DUP) goto L_nxt;  // ignore duplicate smNam eg M21="M20"..
      TX_Print("***** MDL_load_scan__ E3 %s",subNam);
      goto L_nxt;
    }
    // skip images (dyn. load at disp)
    if(mbTyp >= Mtyp_BMP) goto L_nxt;

      // printf(" _lst_sm_upd-add %d |%s|\n",mbTyp,subNam);
  
    // native and cad-model -
    irc = UtxTab_add_uniq__ (&smNames, subNam);

    goto L_nxt;



  L_exit:
    if(smNames.iNr > 0) {
      // get safe name of active primary model
      strcpy(subNam, MDL_mNam_prim());
      UTX_safeName (subNam, 1);
      // write smName -> file tmp/Mod_<mPrim>.lst
      sprintf(fn, "%sMod_%s.lst", AP_get_tmp_dir(), subNam);
      // write list
      UTX_wrf_lst (fn, &smNames);
    }

    UtxTab_free (&smNames);

      // printf("ex-MDL_lst_sm_upd %d\n",irc);

  return 0;

}


//================================================================
  int MDL_lst_sm_add (char *syFn) {
//================================================================
// MDL_lst_sm_add             add subModel uniq to file tmp/Mod_<mPrim>.lst

  int      irc;
  char     *mnPrim, mnSaf[SIZMFTot], fn[400];

  // printf("MDL_lst_sm_add %s\n",syFn);

  // get mNam of primary model
  mnPrim = MDL_mNam_prim ();

  // get filename Mod_<mPrim>.lst
  strcpy(mnSaf, mnPrim);
  UTX_safeName (mnSaf, 1);
  irc = MDL_fnModLst_mNam (fn, mnSaf);
  if(irc < 0) return -1;
    // printf(" MDL_lst_sm_add-fn=%s\n",fn);


  // add syFn uniq to file fn
  UTX_f_add_uniq (fn, syFn);

  return 0;

}


//================================================================
  int MDL_lst__ (TxtTab *ttb, int mtyp, int safe) {
//================================================================
// MDL_lst__                get list of existing subModels
// Input:
//   mtyp      filter; MBTYP_UNDEF (all) or MBTYP_INTERN
//   safe      0 symbolic-modelnames,  1 = safe-modelnames
//
// Example:
//   UtxTab_NEW (sTab1);
//   UtxTab_init__ (&sTab1);
//   irc = MDL_lst__ (&sTab1, MBTYP_INTERN, 0);
//   ..
//   UtxTab_free (&sTab1);
//
// was MDL_lst_intern MDLMP_lst__


  int     i1;
  char    s1[SIZMFTot], *pmn;

  // printf("MDL_lst__ ityp=%d safe=%d\n",mtyp,safe);
  // MDL_dump__ ("MDL_lst__");

  // loop tru mdl_tab - add to ttb
  for(i1=1; i1<mdl_tab.rNr; ++i1) {         // skip 0 = main
//     if(mdRoots[i1].par  < 0) continue;   // never add -main-
//     if(mtyp == MBTYP_UNDEF) goto L_add;
//     if(mdRoots[i1].mtyp != mtyp) continue;
    L_add:
    pmn = MDL_MNAM_INAM(i1);
    if(safe) {
      strcpy (s1, pmn);
      UTX_safeName (s1, 1);
      UtxTab_add (ttb, s1);
    } else {
      UtxTab_add (ttb, pmn);
    }
  }

    // UtxTab_dump (ttb, "ex-MDL_lst__");

  return 0;

}


//================================================================
  int MDL_lst_all (TxtTab *ttb) {
//================================================================
// MDL_lst__                get filelist of all existing subModels (including main)
//   list files "Model_*"
//   returns safeNames
// was MDL_lst_intern MDLMP_lst__


  int     fNr; 
  char    s1[SIZMFTot];
  
  // printf("MDL_lst_all\n");


  UtxTab_init__ (ttb);
  strcpy(s1, AP_get_tmp_dir());
  fNr = UTX_dir_list__ (ttb, s1, "Model_", "", 0);
  
    // UtxTab_dump (ttb, "ex-MDL_lst_all");
    // printf(" ex-MDL_lst_all %d\n",fNr);

  return fNr;

}


//================================================================
  int MDL_file_smuu (int mode) {
//================================================================
// MDL_file_smuu          save / delete unused subModels - ask user
// Input:
//   mode         2 yes|no|cancel
//                3 yes|no
// Output:
//   retCode      0 ok    save all unused-subModels
//                1 no    do not save unused-subModels
//               -1 cancel save
// was AP_save_del_smuu


  int          irc = 0, ii, uuSav;
  char         s1[1024], *p1;
  UtxTab_NEW   (ttb);


  // printf("MDL_file_smuu %d\n",mode);

// TODO: create list of all unused PTAB-Surfs and MSH-Surfs (was AP_save_del_smuu)
  // ii = MDL_smuu_lst (&ttb);  

  // create list of all unused subModels
  irc = MDL_lst_stat (&ttb, -1);
  if(irc < 0) {TX_Error("MDL_file_smuu E1"); irc = 0; goto L_exit;}
  if(ttb.iNr < 1) {irc = 0; printf(" - no unused submodels ..\n"); goto L_exit;}

//   // get string s1 = list of all names
//   s1[0] = '\0';
//   for(ii=0; ii < ttb.iNr; ++ii) {
//     p1 = UtxTab__ (ii, &ttb);
//     strcat(s1, &p1[0]);   // skip "Model_"
//     strcat(s1, "\n");
//   }

  // ask user "save unused submodels - .."
  irc = GUI_file_smuu (mode);
  if(irc == 2) irc = -1; // CANCEL

  //----------------------------------------------------------------
//   L_del:    // delete all unused-subModels
//     for(ii=0; ii < ttb.iNr; ++ii) {
//       p1 = UtxTab__ (ii, &ttb);
//       MDL_rem_mNam_f (&p1[6]);
//     }

  //----------------------------------------------------------------
  L_exit:
    UtxTab_free (&ttb);
      // printf("ex-MDL_file_smuu %d\n",irc);
    return irc;

}


//================================================================
  int MDL_lst_stat (TxtTab *ttb, int mStat) {
//================================================================
// MDL_lst_stat            get list of models - filter = stat
//   loop tru mdl_tab, copy names of model with (stat == mStat)
//   out safe-names;
//   do NOT list main. 
// Input:
//   mStat        filter for mdl_tab[].stat; |1|2|
//                -1 for unused-models (mUsed=1)
// Output:
//   retCode      0=OK, -1=err.
// was ..


  int       i1;
  char      s1[SIZMFTot];
  stru_tab  *tab, *tab1;


  // printf("MDL_lst_stat %d\n",mStat);
  // MDL_dump__ ("MDL_lst_stat");


  // loop tru mdl_tab, copy names of model with (stat == mStat)

  UtxTab_init__ (ttb);
  tab = MEMTAB_DAT (&mdl_tab);


  // list unused-models
  if(mStat == -1) {
    for(i1=1; i1<mdl_tab.rNr; ++i1) {
      // skip deleted
      if(tab[i1].mStat < 0) continue;
      // skip used
      if(tab[i1].mUsed == mdlStat_unused)
        UtxTab_add (ttb, UtxTab__(i1, &mdl_names));
    }
    goto L_exit;
  }


  for(i1=1; i1<mdl_tab.rNr; ++i1) {
    if(tab[i1].mStat != mStat) continue;
    // skip all tess- and image-files
    if(tab[i1].mTyp >= Mtyp_TESS) continue;
    strcpy (s1, UtxTab__(i1, &mdl_names));
    UTX_safeName (s1, 1);
    UtxTab_add (ttb, s1);
  }



  L_exit:
    // UtxTab_dump (ttb, "ex-MDL_lst_stat");
    // printf("ex-MDL_lst_stat\n");
  return 0;

}


//================================================================
  int MDL_used_set__ () {
//================================================================
// MDL_used_set__            set mUsed for all models (active|passive|unused)


// was MDL_smuu_lst

  int        irc, i1, ii, imn, mRegNr, mtyp, mstat, pstat, iused;
  char       *smNam, *safNam, syNam[SIZMFTot];
  stru_tab   *mAct;
  UtxTab_NEW (mdlAll);
  UtxTab_NEW (mdlUseM);
  UtxTab_NEW (mdlUseP);



  // printf("MDL_used_set__ \n");

  if(!BRW_STAT) return 0;



  //----------------------------------------------------------------
  // get list of all existing modelfiles ("Model_<safNam>")
  MDL_lst_all (&mdlAll);

  // get (symb.) list of all used Submodels of main
  irc = MDL_lst_sm_prim (&mdlUseM, 0);
  if(irc < 0) goto L_exit;


  // loop tru mdl_tab - set unused;
  for(i1=0; i1<mdl_tab.rNr; ++i1) mdl_tab.data[i1].mUsed = mdlStat_unused;

  //----------------------------------------------------------------
  // if main is active: set to active
  if(MDL__IS_MAIN) {
    // main is active
    // loop tru mdl_tab - if used in main: set mMain = 1, else 0
    for(i1=0; i1<mdl_tab.rNr; ++i1) {
      // find name in list of main - if yes set 
      ii = UtxTab_find (MDL_MNAM_INAM(i1), &mdlUseM);
      if(ii >= 0) mdl_tab.data[i1].mUsed = mdlStat_active;
    }

    goto L_ck_exi;
  }


  //----------------------------------------------------------------
  // submodel is active;
  // get (symb.) list of all used Submodels of primary-model
  MDL_lst_sm_prim (&mdlUseP, 1);

  // loop tru mdl_tab - 
  for(i1=0; i1<mdl_tab.rNr; ++i1) {
    // find name in list of primary - if yes set active
    ii = UtxTab_find (MDL_MNAM_INAM(i1), &mdlUseP);
    if(ii >= 0) { mdl_tab.data[i1].mUsed = mdlStat_active; continue;}
    // find name in list of main - if yes set passive
    ii = UtxTab_find (MDL_MNAM_INAM(i1), &mdlUseM);
    if(ii >= 0) mdl_tab.data[i1].mUsed = mdlStat_passive;
  }


  //----------------------------------------------------------------
  L_ck_exi:
  // loop tru all existing models
  for(i1=0; i1<mdlAll.iNr; ++i1) {

    // get safeName from modelfilename
    safNam = UtxTab__(i1, &mdlAll);
    safNam += 6;                     // skip "Model_"

    // find safNam in mdl_tab
    imn = MDL_iNam_sNam (safNam);
      // printf("\n _used_set__ nxt ------ i1=%d imn=%d %s\n",i1,imn,safNam);
    if(imn >= 0) continue;
   
    // unregistered model: add to mdl_names and mdl_tab
    // get modeltyp from modelname: PROBLEM safNam
    // mtyp = MDL_mTyp_mNam (smNam);
    mtyp = MBTYP_INTERN; // Mtyp_Gcad

    // add to mdl_names and mdl_tab
    imn = MDL_mdl_add (mtyp, safNam);
    if(imn < 0) {TX_Print("***** MDL_used_set__ E2 %s",safNam);}

    // add to Brw unused
    if(BRW_STAT) Brw_Mod_add (safNam, ICO_natM0);
  }


  //----------------------------------------------------------------
  // set primary to mdlStat_primary 2
  if(AP_modact_inm) {
    mdl_tab.data[0].mUsed = mdlStat_passive;
    mdl_tab.data[AP_modact_inm].mUsed = mdlStat_primary;

  } else {
    // main = primary
    mdl_tab.data[0].mUsed = mdlStat_primary;
  }

  irc = 0;


  //----------------------------------------------------------------
  L_exit:
    UtxTab_free (&mdlUseP);
    UtxTab_free (&mdlUseM);
    UtxTab_free (&mdlAll);

      // MDL_dump__ ("ex-MDL_used_set__");

    return irc;

}

 
//================================================================
  int MDL_brw_upd () {
//================================================================
// MDL_brw_upd                   update mUsed and Brw-Icons of models
// - using mdl_tab[].mUsed - set with MDL_used_set__

  int        ii;
  char       safNam[SIZMFTot];
  stru_tab   *mAct;


  // printf("MDL_brw_upd \n");
  // MDL_dump__ ("MDL_brw_upd");

  if(!BRW_STAT) return 0;


  // remove obj-rows of active primary-model-row
  // BUT NOT after remove subModel - else crash !
  if(Brw_mdl_is_active()) Brw_close_typeRows (NULL);


  // update Brw-Icons of models
  for(ii=1; ii<mdl_tab.rNr; ++ii) {
    mAct = &mdl_tab.data[ii];

    // skip deleted models
    if(mAct->mStat < 0) continue;

    strcpy(safNam, UtxTab__(ii, &mdl_names));
    UTX_safeName (safNam, 1);
    Brw_Mdl_ico_mod (safNam, icoTab[mAct->mUsed + 1]);
  }


  // update main = primary-model-row
  if(AP_modact_inm) ii = ICO_natMU; // passive
  else              ii = ICO_natMA; // primay-model
  Brw_Mdl_ico_mod (AP_mod_fnam, ii);


  // create obj-rows for the primary-model
  Brw_Mdl_upd (0);

  return 0;

}

/* UU
//================================================================
  int MDL_smuu__ () {
//================================================================
// MDL_smuu__              find, register, disp unused-subModels
// - add unused-subModels into mdl_tab with mStat = -1
// - only if unused-subModels exist; first line is mainModel;
// - disp unused-subModels in Brw with icon ICO_natM0 (red)

  int          irc, iNr, mtyp, imn, i1;
  char         safNam[SIZMFTot], fn[320], *p1;
  UtxTab_NEW   (ttb);
  FILE         *fpo;


  printf(" MDL_smuu__\n");


  // get list of unused-subModels
  iNr = MDL_smuu_lst (&ttb);
  if(iNr < 1) goto L_exit;

  //---------------------------------------------------------------
  // register unused-subModels with (mStat = -1)
  for(i1=0; i1<iNr; ++i1) {
    p1 = UtxTab__(i1, &ttb);
      printf(" _smuu__L1---------- %d %s\n",i1,p1);

    // get modelTyp
    mtyp = MDL_mTyp_mNam (p1);
    if(mtyp <= MBTYP_DUP) {TX_Print("***** MDL_smuu__ E1 %s",p1); continue;}

    // add model to mdl_tab
    imn = MDL_mdl_add (mtyp, p1);
    // set model unused
    MDL_USED_SET_INAM (imn, mdlStat_unused);

    // disp unused-subModels in Brw with icon ICO_natM0 (red)
    strcpy(safNam, p1);
    UTX_safeName (safNam, 1);
    Brw_Mod_add (safNam, ICO_natM0);
  }

  
  //---------------------------------------------------------------
  L_exit:

    MDL_dump__ ("ex-MDL_smuu__");
    printf(" ex-MDL_smuu__ %d\n",irc);

  return iNr;

}


//================================================================
  int MDL_smuu_lst (TxtTab *ttb) {
//================================================================
// MDL_smuu_lst            get list of unused subModels
// - use only with main active
// Output:
//   ttb         list of unused subModels;
//               first entry is the mainModel (always unused)
//   retCod      nr of unused subModels
//
// TODO: make direct -loop tru mdl_tab, do not use MDL_file_lst__, file tmp/Mod.lst
// see also MDL_used_set__ (sets active|passive|unused for all models)


  int        i1, ii, *ia, mNr;
  char       s1[SIZMFTot], s2[SIZMFTot], *p1;
  FILE       *fp1;
  UtxTab_NEW (mdlTab);


  // get file tmp/Mod.lst = list of all models in mdl_tab
  mNr = MDL_file_lst__ (1);

  // get mdlTab = list of existing-subModels (list directory)
  // - including primary
  MDL_lst_all (&mdlTab);
  if(mdlTab.iNr < 1) goto L_exit;

//   // if ((nr-used + primary) == nr-existing) - no unused
//   if((mNr + 1) == mdlTab.iNr) goto L_exit;

    // TESTBLOCK
    sprintf(s1,"%sMod.lst",AP_get_tmp_dir());
    DEB_dump_file (s1, "MDL_smuu_lst-Mod.lst");
    // END TESTBLOCK



  //----------------------------------------------------------------
  // set int-array stat parallel to mdlTab; 0=unused, 1=used
  ia = (int*) MEM_alloc_tmp (sizeof(int) * mdlTab.iNr);
  for(i1=0; i1<mdlTab.iNr; ++i1) ia[i1] = 0;

  if((fp1=fopen(s1,"r")) == NULL) goto L_uuList;  // no sm is used

  // loop tru list of used models; set ia to 1 (used)
  while (!feof (fp1)) {
    // get s1 = next subModelname of existing sm. s1 is safe-name.
    if (fgets (s1, SIZMFTot, fp1) == NULL) break;
    UTX_CleanCR (s1);

    // add "Model_" (list mdlTab (existing-subModels) has "Model_<mdlNam>")
    sprintf(s2, "Model_%s", s1);
    UTX_safeName (s1, 1);          // Model_name is safe

    // find s2 in list existing-subModels; set used
    ii = UtxTab_find (s2, &mdlTab);
      printf(" _lst_smuu_find %d |%s|%s|\n",ii,s2,s1);
    if(ii >= 0) ia[ii] = 1;   // set used
  }

  fclose(fp1);

    // TESTBLOCK
    for(i1=0;i1<mdlTab.iNr; ++i1) printf(" _smuu_lst-ia[%d]=%d\n",i1,ia[i1]);
    // END TESTBLOCK



  //----------------------------------------------------------------
  L_uuList:
  // get s2 = active primary safe
  strcpy(s2, MDL_mNamf_prim());
  UTX_safeName (s2, 1);


    // TESTBLOCK
    for(i1=0; i1<mdlTab.iNr; ++i1)
      if(!ia[i1]) printf(" _lst_smuu-unused %d |%s|\n",i1,UtxTab__(i1,&mdlTab));
    printf("  _lst_smuu-s2 |%s|\n",s2);
    // END TESTBLOCK

  // copy modelNames -> ttb
  UtxTab_init__ (ttb);
  for(i1=0; i1<mdlTab.iNr; ++i1) {
    if(ia[i1]) continue;    // skip used
    p1 = UtxTab__(i1, &mdlTab);
    if(strlen(p1) < 7) {TX_Print("***** MDL_smuu_lst E2"); continue;}
    p1 += 6;  // skip "Model_"
    // must ignore main
    if(!strcmp(p1, s2)) continue;
    UtxTab_add (ttb, p1);
  } 


  //----------------------------------------------------------------
  L_exit:
    // TESTBLOCK
    UtxTab_free (&mdlTab);
    UtxTab_dump (ttb, "ex-MDL_smuu_lst");
    printf(" ex-MDL_smuu_lst %d\n",ttb->iNr);
    // END TESTBLOCK

  return ttb->iNr;

}
*/

//================================================================
  int MDL_smuu_upd (long dbi) {
//================================================================
// MDL_smuu_upd            set Brw-modelnode from unused -> used
// - but only if the primary-model is used
// - permanent refMdl M<dbi> created

  int        imb, mrn, mbn, imn, used;
  char       safNam[SIZMFTot];
  ModelRef   *mra;
  ModelBas   *mba;


  // printf(" MDL_smuu_upd %ld\n",dbi);
  // DB_dump_ModRef ("MDL_smuu_upd");


//   // let unused if the primary-model is unused
//   if(!MDL_USED_INAM(AP_modact_inm)) goto L_exit;


  // get name from refMdl
  DB_get_mdr (&mrn, &mra);

  // get index basMdl
  imb = mra[dbi].modNr;

  // get basMdl
  DB_get_mdb (&mbn, &mba);

  // get imn from name
  imn = MDL_INAM_MNAM (mba[imb].mnam);

  // get used = mdl[imn].mUsed
  used = MDL_USED_INAM(imn);

  // printf(" _smuu_upd-imb=%d imn=%d used=%d |%s|\n",imb,imn,used,mba[imb].mnam);


  // if unused then set used
  if(used == mdlStat_unused) {
    MDL_USED_SET_INAM (imn, mdlStat_active);
    // if Brw: change icon approriate
    if(BRW_STAT) {
      strcpy(safNam, mba[imb].mnam);
      UTX_safeName (safNam, 1);
      Brw_Mdl_ico_mod (safNam, ICO_natML);
    }
  }


  //----------------------------------------------------------------
  L_exit:
    // TESTBLOCK
    // MDL_dump__ ("ex-MDL_smuu_upd");
    // printf(" ------- ex-MDL_smuu_upd\n");
    // END TESTBLOCK

  return 0;

}


//================================================================
 int MDL_lst_ptab (TxtTab *surPtab) {
//================================================================
// MDL_lst_ptab          make list of all used PTAB-Surfs
// was Mod_fget_names_0

  int    fNr;
  char   s1[256];

  UtxTab_init__ (surPtab);
  strcpy(s1, AP_get_tmp_dir());
  fNr = UTX_dir_list__ (surPtab, s1, "_A", ".ptab", 0);

    // UtxTab_dump (surPtab, "ex-MDL_lst_ptab");
    // printf("ex-MDL_lst_ptab %d\n",fNr);

  return fNr;

}


//================================================================
 int MDL_lst_msh (TxtTab *surMsh) {
//================================================================
// MDL_lst_ptab          make list of all used MSH-Surfs
// was Mod_fget_names_0

  int    fNr;
  char   s1[256];

  UtxTab_init__ (surMsh);
  strcpy(s1, AP_get_tmp_dir());
  fNr = UTX_dir_list__ (surMsh, s1, "_A", ".msh", 0);

    // UtxTab_dump (surMsh, "ex-MDL_lst_msh");
    // printf("ex-MDL_lst_msh %d\n",fNr);

  return fNr;

}


//================================================================
  int MDL_rem__ (char *safNam) {
//================================================================
// MDL_rem__               remove submodel and its childs
//   model to be removed (safNam) must be active
// Input:
// Output:
//   retCode      0=OK, -1=err.
//
// was MDLMP_rem__ Mod_del_mdb1


  int        irc, inm, mTyp;
  char       symNam[SIZMFTot], fn[400];
  UtxTab_NEW (delLst);


  // printf("_____________________________________________________________ \n");
  // printf("MDL_rem__ |%s|\n",safNam);
  // MDL_dump__ ("MDL_rem__");


  // only CAD; not VWR,MAN
  if(!UI_CAD_ck()) goto L_err1;


  // get symNam = symbolic-filename from safNam
  inm = MDL_imn_mNam__sNam (symNam, safNam);
  if(inm < 0) {TX_Print("***** MDL_rem__ E1 %s",safNam); goto L_err1;}

//   // check if model-to-delete == main
  if(inm == 0) {TX_Print("***** cannot remove main ..."); goto L_err1;}



  //----------------------------------------------------------------
  // clear loadmap
  MDL_LDM_CLEAR ();

  // clear all refModels
  MDL_mdr_clear (-1L);

  // reset all loaded baseModel to mStat=1
  MDL_mdb_clear__ ();


  mTyp = MDL_MTYP_INAM (inm);
  if(MDL_MTYP_IS_MOCK(mTyp)) goto L_mdl_mock;
  goto L_mdl_nat;


  //================================================================
  // delete mockup-model
  L_mdl_mock:

    // activate temp (only mockup; no display)
    MDL_load_mNam_tmp (symNam, inm);

    // rem brwRow

    // remove <symNam> out of all Mod_*.lst
    // remove <symNam> out of all Model_*
    irc = MDL_rem_del (symNam);
    if(irc < 0) TX_Print("***** MDL_rem_del E1");

    // remove tmp/Mod_<safNam>.tess
    MDL_fnModTess_mNam (fn, safNam);
    // sprintf(fn, "%sMod_%s.tess", AP_get_tmp_dir(), safNam);
    OS_file_delete (fn);
  

    goto L_exit;


  //================================================================
  // delete native model
  L_mdl_nat:
    UtxTab_init__ (&delLst);

    // get filename Mod_<nam-model-to-delete>.lst
    MDL_fnModLst_mNam (fn, safNam);
      // printf(" _rem__ fn|%s|%s|\n",fn,safNam);

    // add childs of active primary model into delLst
    if(OS_checkFilExist (fn, 1)) UtxTab_add_file (&delLst, fn);

    // delete (used) model symNam in all modelfiles Mdl_*.lst and Model_*
    irc = MDL_rem_del (symNam);
    if(irc < 0) {TX_Print("***** MDL_rem_del E2"); goto L_ex_mdl_nat;}

    // delete models of list if unused
    irc = MDL_rem_mdl (&delLst);

    L_ex_mdl_nat:
      UtxTab_free (&delLst);


  //================================================================
  L_exit:

      // TESTBLOCK
      // MDL_dump__ ("ex-MDL_rem__");
      // // UtxTab_dump (&mdNames, "ex-MDL_rem__");
      // printf("ex-MDL_rem__ %d\n",irc);
      // END TESTBLOCK

    return irc;


  L_err1:
    irc = -1;
    goto L_exit;

}


//================================================================
  int MDL_rem_mdl (TxtTab *delLst) {
//================================================================
// MDL_rem_mdl                delete models of list if unused
// Input:
//   delLst          list names of models to delete

  int     irc;
  char    *symNam, safNam[SIZMFTot], fn[320];


  // printf("MDL_rem_mdl\n");


  //----------------------------------------------------------------
  L_nxtNam:
      // TESTBLOCK
      // UtxTab_dump (delLst, "MDL_rem_mdl-L_nxtNam");
      // END TESTBLOCK
 
    // if list-models-to-delete is empty return
    if(delLst->iNr < 1) {irc = 0; goto L_exit;}

    // get nam-model-to-delete = first name of file list-models-to-delete
    symNam = UtxTab__ (0, delLst);

    // make safe
    strcpy (safNam, symNam);
    UTX_safeName (safNam, 1);

    // delete first name of file list-models-to-delete
    irc = UtxTab_del (0, delLst);
    if(irc < 0) {TX_Print("***** MDL_rem_mdl E1"); goto L_exit;}

    // add all names of file Mod_<nam-model-to-delete>.lst to list-models-to-delete
    // get filename Mod_<nam-model-to-delete>.lst
    MDL_fnModLst_mNam (fn, safNam);
      // printf(" _rem_mdl-fn |%s|\n",fn);


    if(OS_checkFilExist (fn, 1)) {
      // add all models
      UtxTab_add_file (delLst, fn);
    }

    // check nr of occurrences of model symNam in all lists tmp/Mdl_*.lst
    irc = MDL_rem_ck_used (symNam);
    if(irc < 0) goto L_exit;
    
    // if symNam is unused delete model symNam
    if(!irc) MDL_rem_del (symNam);

    goto L_nxtNam;


  //----------------------------------------------------------------
  L_exit:
      // printf(" ex-MDL_rem_mdl %d\n",irc);
    return irc;

}



//================================================================
  int MDL_rem_ck_used (char *symNam) {
//================================================================
// MDL_rem_ck_used                check if model is used
//   get list-all-models (all names of tmp/Mdl_*.lst)
//   return iNr = nr of occurrences of nam-model-to-delete in list-all-models
// Input:
//   symNam          symbolic name of model to check
// Output:
//   retCode        0 symNam is unused (0 occurrences)
//                  1 symNam is used (1 or more occurrences)
//                 -1 error


  int        irc, i1, fNr;
  char       *fn;
  UtxTab_NEW (lstFnam);
  UtxTab_NEW (lstMnam);


  // printf("MDL_rem_ck_used |%s|\n",symNam);


  // add all models of all Mod_*.lst into list allMdls

  // get lstFnam = list of all Mod_*.lst
  UtxTab_init__ (&lstFnam);
  fNr = UTX_dir_list__ (&lstFnam, AP_get_tmp_dir(), "Mod_", ".lst", 1);
  if(!fNr) {irc = 0; goto L_exit;}

  // add all modelnames in files lstFnam into list lstMnam
  UtxTab_init__ (&lstMnam);
  for(i1=0; i1<fNr; ++i1) {
    fn = UtxTab__ (i1, &lstFnam);
    irc = UtxTab_add_file (&lstMnam, fn);
    if(irc < 0) {TX_Print("***** MDL_rem_ck_used E1"); irc = -1; goto L_exit;}
  }

  // find symNam in allMdls
  i1 = UtxTab_find (symNam, &lstMnam);
  if(i1 < 0) irc = 0;
  else       irc = 1;

  L_exit:
    UtxTab_free (&lstFnam);
    UtxTab_free (&lstMnam);
      // printf(" ex-MDL_rem_ck_used %d\n",irc);
    return irc;

}


//================================================================
  int MDL_rem_del (char *symNam) {
//================================================================
// MDL_rem_del                remove subModel
// Input:
//   symNam          symbolic name of model to delete


  int     irc, imn;
  char    safNam[SIZMFTot];

  // printf("MDL_rem_del |%s|\n",symNam);


  // get imn = index modelname
  imn = MDL_INAM_MNAM (symNam);

  // set mStat of model-to-delete = -1
  MDL_STAT_SET_INAM (imn, -2);

  // make safe name
  strcpy (safNam, symNam);
  UTX_safeName (safNam, 1);

  // remove modelfile, listfile and DB-file
  MDL_rem_mNam_f (safNam);

  // remove modelcalls out of all modelfiles tmp/Model_*
  irc = MDL_ren_fmdl (symNam, NULL);
  if(irc < 0) goto L_exit;

  // remove modelcalls out of all modelfiles tmp/Mdl_*.lst
  irc = MDL_ren_flst (symNam, NULL);
  // if(irc < 0) goto L_err1;


  // DO NOT // remove record from mdl_names and mdl_tab 
  // - destroys names of submodels (= pointer into mdl_names
  // irc = UtxTab_del (inm, &mdl_names);
  // if(irc < 0) goto L_err1;
  // irc = MemTab_del (&mdl_tab, inm, 1);
  // // remove baseModel
  // DB_del_mdb_syFn (symNam);


  // delete Model-entry in Browser
  if(BRW_STAT) {
    Brw_del_oid (safNam);
    Brw_del_mdlNod ();       
  }

  L_exit:
      // printf("ex-MDL_rem_del %d\n",irc);
    return irc;

}



//================================================================
  int MDL_rem_mNam_f (char *mNam) {
//================================================================
// MDL_rem_mNam_f          delete files of subModel
//   delete files tmp/<Model_<mNam> and tmp/Mod_<mNam>.lst and tmp/DB__<mNam>.dat
// Input:
//   mNam         safe-name of model to delete
// Output:
//   retCode      0=OK, -1=err.
// was ..


  // int     irc;
  char    s1[320];

  // printf("MDL_rem_mNam_f |%s|\n",mNam);

  // del <tmp/Model_<safNam>>
  sprintf(s1,"%sModel_%s",AP_get_tmp_dir(),mNam);
    // printf(" _rem_1-delFil %s\n",s1);
  OS_file_delete (s1);

  // del Mod_<safNam>.lst
  sprintf(s1,"%sMod_%s.lst",AP_get_tmp_dir(),mNam);
    // printf(" _rem_mNam_f-delFil %s\n",s1);
  OS_file_delete (s1);

  // del DB__<safNam>.dat
  sprintf(s1,"%sDB__%s.dat",AP_get_tmp_dir(),mNam);
    // printf(" _rem_mNam_f-delFil %s\n",s1);
  OS_file_delete (s1);


  L_exit:
    // printf("ex-MDL_rem_mNam_f %d\n",0);
  return 0;

}


//================================================================
  int MDL_mod_MODSIZ (double d1, double d2, double d3) {
//================================================================
// MDL_mod_MODSIZ          modify MODSIZ in all subModels
//
// in den tmp/Model_* steht:
// MODSIZ ...
// ersetzen durch 
// MODSIZ <d1> <d2> <d3>

  char  cbuf1[256], fNam[SIZFNam], newLn[256];
  FILE  *fp1;


  // printf("Mod_allmod_MS %f %f %f\n",d1,d2,d3);
  // AP_debug__ ("Mod_allmod_MS");

  sprintf(newLn, "MODSIZ %f %f %f",d1,d2,d3);

  // make list of all Submodels ohne "-main-" --> <baseDir>/tmp/Mod.lst
  MDL_file_lst__ (1);

  // try to open inListe
  sprintf(cbuf1,"%sMod.lst",AP_get_tmp_dir());
  if((fp1=fopen(cbuf1,"r")) == NULL) {
    TX_Print("Mod_allmod_MS E002 %s",cbuf1);
    return -1;
  }

  while (!feof (fp1)) {
    // loop tru subModels
    if (fgets (cbuf1, 256, fp1) == NULL) break;
    UTX_CleanCR (cbuf1);
    sprintf(fNam,"%sModel_%s",AP_get_tmp_dir(),cbuf1);
      // printf(" allmod_MS-nxt sM=|%s|\n",fNam);

    // gets first line of file fNam
    UTX_fgetLine (cbuf1, 256, fNam, 1);

    if(strstr(cbuf1, "MODSIZ ")) {
      // change MODSIZ Line (Ln # 1)
      UTX_fsavLine (newLn, fNam, mem_cbuf1_SIZ, 1);

    } else {
      // first Line != MODSIZ - insert line
      sprintf(cbuf1, "%stempFile1",AP_get_tmp_dir());
      strcat(newLn, term_buf);    // add LF
      UTX_wrf_str (cbuf1, newLn);
      // join file fNam=cbuf1+fNam
      UTX_fjoin__ (fNam, cbuf1, fNam);
    }

  }

  fclose(fp1);

    // AP_debug__ ("ex-Mod_allmod_MS");

  return 0;

}


/* UU
//================================================================
  int MDL_prev__ (char *src) {
//================================================================
// MDL_imp__          preview model (in temporary-mode)
// - set temporary-mode; create baseModel(s) and refModel(s) temporary, preview.
// Input:
//   src          modelcode; eg "M0=\"OBJ_U/myHome3.obj\" RZ"
//                NULL:  remove all temp-models
// Output:
//   retCode      0=OK, file tmp/Model_<ofn->fNam> created;
//               -1=err.
// was MDLMP_prev__


  int       irc, mTyp;
  long      dbi;
  char      *pSrc, mNam[SIZMFTot];
  ObjAto    ato1;
  ObjGX     ox1;
  ModelRef  mr1;


  printf("VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV \n");
  printf("MDL_prev__ |%s|\n",src);
  MDL_dump__ ("MDL_prev__-in");
  // printf(" APT_obj_stat=%d\n",APT_obj_stat);  // 0=permanent, 1=temporary


  //----------------------------------------------------------------
  // src == NULL: remove all temp-models
  if(!src) {irc = MDL_prev_stat__ (OBJSTAT_perm); goto L_exit;}


  // activate temp-mode in MDL
  MDL_prev_stat__ (OBJSTAT_temp);


  //----------------------------------------------------------------
  // add modelname, create baseModel

    // extract modelname (symbolic)
    mTyp =  MDL_nam__srcLn (mNam, src);
      printf(" _prev__-mTyp=%d mnam=|%s|\n",mTyp,mNam);

    // add new model to mdl_names, mdl_tab; create baseModel
    irc = MDL_load_mdl__ (mTyp, mNam, 0, OBJSTAT_temp);


  //----------------------------------------------------------------
  // decode line - APT_decode_model - create refModel

    // get memSpc for ato1 (memspc53, 54, 55)
    ATO_getSpc__ (&ato1);

    // get atomicObjects from sourceLine p1; full evaluated.
    pSrc = strchr (src, '=');
    if(!pSrc) {TX_Print("***** MDL_prev__ E1 |%s|\n",src); irc = -1; goto L_exit;}
    ++pSrc; // skip '='
    irc = ATO_ato_srcLn__ (&ato1, pSrc);
    if(irc < 0) goto L_exit;
      ATO_dump__ (&ato1, "MDL_prev__-ato1");


    // create single struct from atomicObjs, create refMdl 
    APT_defTxt = src;    // APT_decode_model needs src
    irc = APT_obj_ato (&ox1, Typ_Model, &dbi, &ato1);
    if(irc < 0) {TX_Print("***** MDLMP_prev__ E2 |%s|\n",src); goto L_exit;}
      DEB_dump_obj__ (Typ_ObjGX, &ox1, " -MDL_prev__-L21");

    // get ModelRef mr1
    mr1 = *((ModelRef*)ox1.data);
      DEB_dump_obj__ (Typ_Model, &mr1, " -MDL_prev__-L22");


  //----------------------------------------------------------------
  // disp
  irc = UI_prev_obj (Typ_Model, GR_TMP_I0, &mr1, NULL);
  // goto L_exit;


  //----------------------------------------------------------------
  L_exit:
      MDL_dump__ ("ex-MDL_prev__-in");
      printf(" ex-MDL_prev__ %d\n",irc);
    return irc;

}
*/

//================================================================
  int MDL_ren_sav () {
//================================================================
// MDL_ren_sav             update modelname in browser after save-as

  char   fn1[SIZFNam], fn2[SIZFNam], actNam[SIZMFTot], newNam[SIZMFTot];


  printf("MDL_ren_sav |%s|%s|%s|\n",AP_mod_sym,AP_mod_fnam,AP_mod_ftyp);

  sprintf(newNam, "%s/%s.%s",AP_mod_sym,AP_mod_fnam,AP_mod_ftyp);
  AP_Mod_load_fn (newNam, 0);


/*
  PROBLEM: UtxTab_change invalidates all Modelnames of basicModels
   (pointers into mdlnames) - should be updated; also if mdlnames reallocs !)

  MDL_dump__ ("MDL_ren_sav");
  printf(" MDL_mNam_main-1 = |%s|\n",MDL_mNam_main());


  // get actNam = active mainModelfilename; MDL_mNam_main is old mainModelName
  strcpy(actNam, UtxTab__(0, &mdl_names));
  UTX_safeName (actNam, 1);


  //----------------------------------------------------------------
  // update modelname in nameTable
  // new name is AP_mod_fnam; change mdl_names[0]                        <<<<<
  sprintf(newNam, "%s/%s.%s",AP_mod_sym,AP_mod_fnam,AP_mod_ftyp);
  UtxTab_change (&mdl_names, 0, newNam);
    printf(" MDL_mNam_main-2 = |%s|\n",MDL_mNam_main());
  


  //----------------------------------------------------------------
  UTX_safeName (newNam, 1);
// SEE ALSO MDL_ren_1

  // rename <tmp>/Model_<actNam>    -> <tmp>/Model_<newNam>>
  sprintf(fn1,"%sModel_%s", AP_get_tmp_dir(), actNam);
  sprintf(fn2,"%sModel_%s", AP_get_tmp_dir(), newNam);
  OS_file_rename (fn1, fn2);

  // rename <tmp>/Mod_<actNam>.lst  -> <tmp>/Mod_<newNam>.lst
  sprintf(fn1,"%sMod_%s.lst", AP_get_tmp_dir(), actNam);
  if(OS_checkFilExist (fn1, 1)) {
    sprintf(fn2,"%sMod_%s.lst", AP_get_tmp_dir(), newNam);
    OS_file_rename (fn1, fn2);
  }

  // rename <tmp>/DB__<actNam>.dat  -> <tmp>/DB__<newNam>.dat
  sprintf(fn1,"%sDB__%s.dat", AP_get_tmp_dir(), actNam);
  sprintf(fn2,"%sDB__%s.dat", AP_get_tmp_dir(), newNam);
  OS_file_rename (fn1, fn2);


  //----------------------------------------------------------------
  // exit if browser not active
  if(BRW_STAT) {
    // update browser
    Brw_row_main_set (AP_mod_fnam);
  }
*/

    // MDL_dump__ ("ex-MDL_ren_sav");

  return 0;

} 


//================================================================
  int MDL_ren__ (char *actNam) {
//================================================================
// MDL_ren__              rename model
// Input:
// Output:
//   retCode      0=OK, -1=err.
//
// was MDLMP_ren__


  int      irc;
  char     s1[320], newNam[SIZMFTot], newSafe[SIZMFTot], actSafe[SIZMFTot];

  // printf("______________________________________________________ \n");
  // printf("MDL_ren__ |%s|\n",actNam);

  // only CAD; not VWR,MAN
  if(!UI_CAD_ck()) return 0;

  // check if actNam == main
  if(MDL_IS_MAIN) {TX_Print("***** cannot rename main ..."); return -1;}

  // make safeName
  strcpy(actSafe, actNam);
  UTX_safeName (actSafe, 1); // change '. /\\'


  //----------------------------------------------------------------
  // get new name for model <oldNam>
  sprintf(s1, "rename Submodel %s to:", actNam);
  strcpy(newNam, actNam);
  irc = GUI_dlg_e2b (newNam, 128, s1, "OK", "Cancel");
    // printf(" _ren__irc=%d newNam=|%s|\n",irc,newNam);

  if(irc != 0) return -1;             // error
  if(strlen(newNam) < 1) return -1;   // cancel

  // check maxLen; max 62 chars
  if(strlen(newNam) > 127) {
    newNam[127] = '\0';
    TX_Print("***** max length reduced  to 128 *****");
  }

  // make safeName from new subModelname
  // Mod_safeName (newNam, fnam);
  strcpy (newSafe, newNam);
  UTX_safeName (newSafe, 1); // change '. /\\'

    // TEST:
    // strcpy(newNam, "myConn");
    // TEST END

  //----------------------------------------------------------------
  // rename
  irc = MDL_ren_1 (actSafe, newSafe);
  if(irc < 0) goto L_exit;

  // disp msg
  TX_Print("- Model %s renamed to %s",actNam,newSafe);

  // set name of active-model
  strcpy(AP_modact_nam, newSafe);

  // fix title  ("main/sm")
  UI_title_set ();

  // rename active subModel in browser
  Brw_sMdl_ren_CB (newSafe);


  //----------------------------------------------------------------
  L_exit:
    // TESTBLOCK
    // printf("ex-MDL_ren__ irc=%d \n",irc);
    // DB_dump_ModBas ("ex-MDL_ren__"); // dump baseModels
    // MDL_dump__ ("ex-MDL_ren__");             // dump loadMap
    // END TESTBLOCK
  return irc;

}


//================================================================
  int MDL_ren_1 (char *actNam, char *newNam) {
//================================================================
// MDL_ren_1              rename model
// Input
//   actNam,newNam     rename model <actNam> -> <newNam> (safe-names)
//   both names must be safenames

  int       irc, inm1, inm2, mtyp, mbi;   // ii, mbi, roi, mtyp;
  char      symNam[SIZMFTot], fn1[256], fn2[256];
  ModelBas  *mb1;


  // printf("MDL_ren_1 |%s|%s|\n\n",actNam,newNam);
  // MDL_dump__ ("MDL_ren_1");
  // DB_dump_ModBas ("MDL_ren_1");


  //----------------------------------------------------------------
  // model must be extern|intern; NOT ctlg|mockup|image

  // test if model already exists
  inm1 = MDL_INAM_MNAM (newNam);
  if(inm1 >= 0) {TX_Print("***** model %s already exists ",actNam); goto L_err1;}

  // get symbolic-name and index of actNam
  inm1 = MDL_imn_mNam__sNam (symNam, actNam);
  if(inm1 < 0) {TX_Print("***** MDL_ren_1 E1 %s",actNam); goto L_err1;}

  // get mtyp
  mtyp = MDL_MTYP_INAM (inm1);
    // printf(" _ren_1-inm1=%d mtyp=%d\n",inm1,mtyp);
  if((mtyp > Mtyp_TESS)||(mtyp < MBTYP_INTERN))
    {TX_Print("***** cannot rename - only intern or extern models"); goto L_err1;}


  //----------------------------------------------------------------
  // add new name into mdNames
  inm2 = MDL_mdl_add (mtyp, newNam);
  if(inm2 < 0) goto L_err1;      // error

  // update mdl-tab record (stat, mbi)
  mdl_tab.data[inm2] = mdl_tab.data[inm1];
  mdl_tab.data[inm2].mbi = mdl_tab.data[inm1].mbi;

  // get index baseModel
  mbi = MDL_IMB_INAM (inm1);

  // get baseModel mbi
  mb1 = DB_get_ModBas (mbi);

  // update name in baseModel
  mb1->mnam = MDL_MNAM_INAM (inm2);

  // reset mStat of old mdl-tab
  mdl_tab.data[inm1].mStat = -1;

  // set AP_modact_inm (name-index new primaryModel)
  AP_modact_inm = inm2;


  //----------------------------------------------------------------
  // rename <tmp>/Model_<actNam>    -> <tmp>/Model_<newNam>>
  sprintf(fn1,"%sModel_%s", AP_get_tmp_dir(), actNam);
  sprintf(fn2,"%sModel_%s", AP_get_tmp_dir(), newNam);
  OS_file_rename (fn1, fn2);

  // rename <tmp>/Mod_<actNam>.lst  -> <tmp>/Mod_<newNam>.lst
  sprintf(fn1,"%sMod_%s.lst", AP_get_tmp_dir(), actNam);
  if(OS_checkFilExist (fn1, 1)) {
    sprintf(fn2,"%sMod_%s.lst", AP_get_tmp_dir(), newNam);
    OS_file_rename (fn1, fn2);
  }

  // rename <tmp>/DB__<actNam>.dat  -> <tmp>/DB__<newNam>.dat
  sprintf(fn1,"%sDB__%s.dat", AP_get_tmp_dir(), actNam);
  sprintf(fn2,"%sDB__%s.dat", AP_get_tmp_dir(), newNam);
  OS_file_rename (fn1, fn2);


  //----------------------------------------------------------------
  // - change oldName into newName in all files Model_*
  irc = MDL_ren_fmdl (symNam, newNam);

  // - change oldName into newName in all files Mod_*.lst
  irc = MDL_ren_flst (symNam, newNam);


  //----------------------------------------------------------------
  L_exit:
      // TESTBLOCK
      // MDL_dump__ ("ex-MDL_ren_1");
      // DB_dump_ModBas ("ex-MDL_ren_1");
      // printf("ex-MDL_ren_1 %d\n",irc);
      // END TESTBLOCK
    return irc;


  L_err1:
    irc = -1;
    goto L_exit;

}


//================================================================
  int MDL_ren_fmdl (char *fn1, char *fn2) {
//================================================================
// MDL_ren_fmdl      modify modelfiles - rename or delete submodel
//   change modelname fn1 into fn2 in all subModels of all modelfiles
// Input:
//   fn1     symbolic-filename to be changed
//   fn2     internal filename; safe; if(!fn2) delete codeline
//
// see also Mod_del_mdb1 MDL_add_mnam_scan

#define SRCLNSIZ 15000
  int    irc, ii, ln1, iNr, fNr;
  long   l1, l2, dbi;
  char   s1[SRCLNSIZ], s3[256], str1[256], str2[256],
         *p1, *p2, *fnIn, fnOut[256];
  TxtTab fnLst = _UTXTAB_NUL;
  FILE   *fpMod, *fpOut;


  // printf("MDL_ren_fmdl |%s|%s|\n",fn1,fn2);

  // get fnLst = list of files tmp/Model_*
  UtxTab_init__ (&fnLst);
  strcpy(s1, AP_get_tmp_dir());
  fNr = UTX_dir_list__ (&fnLst, s1, "Model_", "", 1);
  if(fNr < 1) goto L_exit;


  sprintf(fnOut,"%sModel.tmp",AP_get_tmp_dir());

  if(fn2) l2 = strlen(fn2);

  //----------------------------------------------------------------
  // loop tru fileList
  for(ii=0; ii<fNr; ++ii) {
    fnIn = UtxTab__ (ii, &fnLst);
      // printf(" ::::::::::::updFiles-nxt sM=|%s|\n",fnIn);


    if((fpMod=fopen(fnIn,"r")) == NULL) {
      TX_Print("***** MDL_ren_files E003 %s",fnIn);
      goto L_err;
    }

    if((fpOut=fopen(fnOut,"w")) == NULL) {
      TX_Print("***** MDL_ren_files E004 %s",fnOut);
      goto L_err;
    }

    iNr = 0;

    //----------------------------------------------------------------
    // read file - change subModel fn1 -> fn2
    while (!feof (fpMod)) {
      if(fgets (s1, SRCLNSIZ, fpMod) == NULL) break;
        // printf(" ........nxt ln |%s|\n",s1);

      // test if refModel-definition; get dbi 
      irc = SRC_ck_objDefLn (&dbi, &p1, s1, 'M');
      if(irc < 0) goto L_write;
        // printf(" _ren_files-L1 |%s|%s|\n",s1,p1);

      if(*p1 != '"') {
        TX_Print("***** MDL_ren_fmdl |%s| ???",s1);
        goto L_write;
      }
      ++p1; // skip '"'

      // get s3 = next string, size 
      p2 = UTX_cp_word_term (s3, p1, '"');
      if(!p2) {TX_Print("***** MDL_ren_fmdl E005 %s",p1); goto L_write;}
        // printf(" _ren_files-L2 |%s|%s|\n",s1,s3);

      if(strcmp(s3, fn1)) goto L_write;

      ++iNr;

      // found fn1; change or delete
      if(!fn2) continue;

      // change fn1 to fn2
      l1 = strlen(s1);
      MEM_chg_rec (s1, &l1,                   // whole line
                   fn2, l2,                   // new
                   p1,  strlen(s3));          // oldPos, oldLen
        // printf(" ........new ref-4 |%s|\n",s1);

      L_write:
        fprintf(fpOut, "%s", s1);
    }
    //----------------------------------------------------------------

    fclose(fpMod);
    fclose(fpOut);

    // rename ..
    if(iNr) OS_file_rename (fnOut, fnIn);

  }

  irc = 0;


  L_exit:
    UtxTab_free (&fnLst);
      // printf("ex-MDL_ren_fmdl irc=%d iNr=%d\n",irc,iNr);
    return 0;


  L_err:
    irc = -1;
    goto L_exit;

}


//================================================================
  int MDL_ren_flst (char *fn1, char *fn2) {
//================================================================
// MDL_ren_flst      modify listfiles - rename or delete submodel
//   change modelname fn1 into fn2 in all listfiles tmp/Mod_<fn>lst
// Input:
//   fn1     symbolic-filename to be changed
//   fn2     internal filename; safe; if(!fn2) delete line
//
// see also Mod_del_mdb1 MDL_add_mnam_scan

#define SRCLNSIZ 15000
  int    irc, ii, fNr;
  char   *fNam, *p1, s1[320];
  TxtTab fnLst = _UTXTAB_NUL;


  // printf("MDL_ren_flst |%s|%s|\n",fn1,fn2);

  // get fnLst = list of files tmp/Mod_*.lst
  UtxTab_init__ (&fnLst);
  fNr = UTX_dir_list__ (&fnLst, AP_get_tmp_dir(), "Mod_", ".lst", 1);
  if(fNr < 1) goto L_exit;


  //----------------------------------------------------------------
  // loop tru fileList
  for(ii=0; ii<fNr; ++ii) {
    fNam = UtxTab__ (ii, &fnLst);
      // printf(" ::::::::::::_ren_files2-nxt |%s|\n",fNam);
    // change or delete filename
    irc = UTX_fmod_lnTxt (fNam, fn1, fn2, SRCLNSIZ);
    if(irc) {
      p1 = strrchr(fNam, fnam_del);
      strcpy(s1, &p1[5]);              // skip "/Mod_"
      p1 = strchr(s1, '.');
      if(p1) *p1 = '\0';
      if(irc < 0) {
        TX_Print("***** Error MDL_ren_flst %s",s1);
      } else { 
        // model(s) modified; but not main
        if(!MDL_mNam_cmp_sNam(MDL_mNam_main(), s1)) {
          TX_Print("***** change Model %s to internal !",s1);
          MDL_mTyp_set_sNam (s1, MBTYP_INTERN);
        }
      }
    }
  }

  irc = 0;


  L_exit:
    UtxTab_free (&fnLst);
      // printf("ex-MDL_ren_fmdl %d\n",irc);
    return 0;


  L_err:
    irc = -1;
    goto L_exit;

}


//================================================================
  int MDL_file_skip_DYNDAT (FILE *fpi) {
//================================================================
// MDL_file_skip_DYNDAT             skip until :DYNAMIC_DATA
//   retCode      0=OK, 1=:DYNAMIC_DATA not found; rewinded.


  int     irc, lSiz;
  char    *lBuf;

  // printf("MDL_file_skip_DYNDAT \n");

  lBuf = mem_cbuf1;
  lSiz = mem_cbuf1_SIZ;


  //----------------------------------------------------------------
  L_nxtLn:
    if(fgets (lBuf, lSiz, fpi) == NULL) {
      // :DYNAMIC_DATA not exists
      rewind (fpi);
      irc = 1;
      goto L_exit;
    }

    if(lBuf[0] != ':') goto L_nxtLn;
    if(strncmp(&lBuf[1], "DYNAMIC_DATA", 12)) goto L_nxtLn;

    irc = 0;


  //----------------------------------------------------------------
  L_exit:
    // printf("ex-MDL_file_skip_DYNDAT %d\n",irc);
  return irc;

}


/*
//================================================================
  char* MDL_mNamf_prim () {
//================================================================
// MDL_mNamf_prim              get modelfilename of active model (main or subModel)
// Output:
//   retCode      pointer to modelfilename, symbolic, without "Model_"
//
// get file-modelname with mnam = mdl_tab.data[0].

  int   ii;
  char  *mdlNam;

  ii = AP_modact_inm;
  if(ii < 0) ii = 0; 
  mdlNam = UtxTab__(ii, &mdl_names);

  L_exit:
    printf("ex-MDL_mNamf_prim |%s|\n",mdlNam);
  return mdlNam;

}
*/



//================================================================
  char* MDL_mNam_main () {
//================================================================
// MDL_mNam_main           get modelname of mainModel
// Output:
//   retCode      pointer to AP_mod_fnam | AP_modact_nam
//                "-" if mainModel undefined
//

  char  *mdlNam;
static char noNam[] = "-";


  if(mdl_names.iNr >0) mdlNam = UtxTab__(0, &mdl_names);
  else mdlNam = noNam;


  L_exit:
    // printf("ex-MDL_mNam_prim |%s|\n",mdlNam);
  return mdlNam;

}


//================================================================
  char* MDL_mNam_iNam (int inm) {
//================================================================
// MDL_mNam_iNam           get modelname from name-index
// Output:
//   retCode      pointer to modelname
//                "-" if undefined
//

  char  *mdlNam;
static char noNam[] = "-";

  
  if(mdl_names.iNr >0) mdlNam = UtxTab__(inm, &mdl_names);
  else mdlNam = noNam;
    

  L_exit:
    // printf("ex-MDL_mNam_prim |%s|\n",mdlNam);
  return mdlNam;
    
} 


//================================================================
  char* MDL_mNam_prim () {
//================================================================
// MDL_mNam_prim              get name of active model (main or subModel)
// - returns "" if mainModel is active !
// Output:
//   retCode      pointer to AP_mod_fnam | AP_modact_nam
//

  char  *mdlNam;

  if(MDL_IS_MAIN)  {
    mdlNam = MDL_mNam_main ();
  } else {
    mdlNam = &AP_modact_nam[0];
  }

  L_exit:
    // printf("ex-MDL_mNam_prim |%s|\n",mdlNam);
  return mdlNam;

}


//================================================================
   int MDL_nam__srcLn (char *syNam, char *cbuf) {
//================================================================
// MDL_nam__srcLn          get modelTyp and modelName from definition subModel
// give typ from definition Line and extract (complete) Modelname --> syNam
// Input:
//   cbuf     definitionLine; zB   M20="Profile/4x4_60.ctlg"  ..
// OutPut:
//   syNam    symbolic modelname  eg "Data/xx.gcad" (without ")
//   RetCod:
//       Mtyp_Gcad to < Mtyp_BMP  - extern - native | import
//       MBTYP_EXTERN           0 = extern model
//       MBTYP_INTERN          -1 = internal model
//       MBTYP_CATALOG         -2 = catalog-model
//       MBTYP_DUP             -3 duplicate model  (from eg (M25="M20" <newPos>) UU
//       MBTYP_ERROR           -4 Error

// ATTENTION:
// MBTYP_INTERN can return a reference to the real Modelname eg "M20" !

// M#="mdlNam" refsys
// M#=CTLG "ctlgNam" refsys OBSOLETE
// M#=MIR M# mirror


  int   mode, ityp, iuu;
  long  dbi;
  char  *cp1, ftyp[16], *cp2;


  // printf("MDL_nam__srcLn |");UTX_dump_cnl (cbuf, 50);printf("|\n");


  cp1 = strchr(cbuf, '=');
  if(cp1 == NULL) {ityp = MBTYP_ERROR; goto L_exit;}
  ++cp1;

  // TODO: remove - old version catalog-part-definition.
  // check for catalog-model  (M20=CTLG "...")
  UTX_pos_skipLeadBlk (cp1);
  if(!strncmp(cp1, "CTLG", 4)) {
    printf("******** old version CATOLG-PART-DEFINITION .. \n");
    cp1 += 4;
    ityp = MBTYP_CATALOG;
  } else {
    ityp = 0;
  }

  // auf next word nach "M#=" positionieren
  cp1 = strchr(cp1, '"');
  if(cp1 == NULL)  {ityp = MBTYP_ERROR; goto L_exit;}
  ++cp1;

  // syNam = mdlNam (incl.Path, excluding '"')
  UTX_cp_word_term (syNam, cp1, '"');
    // printf(" _nam__srcLn syNam=|%s|\n",syNam);

  // get modelTyp
  // TODO: if-cond. only for old version catalog-part-definition.
  if(ityp != MBTYP_CATALOG)
    ityp = MDL_mTyp_mNam (syNam);


  L_exit:
    // printf("ex-MDL_nam__srcLn syNam=|%s| ityp=%d\n",syNam,ityp);
  return ityp;

}


//================================================================
  int MDL_nam_src1 (char *mnam) {
//================================================================
// MDL_nam_src1              check if mNam is modelID or modelname (symb. or intern)
// retCode:   -1    mnam is symbolic-modelname   eg "Data/Niet1.gcad"
//            >=0   mnam is modelID eg "M21"; retCode = dbi (21)


  int    irc = -1;
  long   l1;
  char   *cpe;

  // printf("MDL_nam_src1 |%s|\n",mnam);

  UTX_pos_skipLeadBlk (mnam);

  if((mnam[0] != 'M')&&(mnam[0] != 'm')) goto L_exit;
  ++mnam;
  l1 = strtol(mnam, &cpe, 10);
  if(*cpe != '\0') goto L_exit;

  irc = l1;


  L_exit:
    // printf(" ex-MDL_nam_src1 %d\n",irc);
  return irc;

}


//================================================================
  int MDL_mTyp_mNam (char *mNam) {
//================================================================
// MDL_mTyp_mNam          get modeltyp from symbolic-modelname
// Input:
//   mNam       symbolic-modelname; eg "Data/test1.gcad" (without quotes)
// Output:
//   retcod     modeltyp eg Mtyp_Gcad 
//
// was MDL_get_typ2
// see also MDL_nam__srcLn

  int    irc, mTyp;
  long   dbi;
  char   ftyp[32], *cp1;


  // printf("MDL_mTyp_mNam |%s|\n",mNam);


  // check MBTYP_DUP; check if name is a refModelID
  if(mNam[0] == 'M') {
    dbi = strtol (&mNam[1], &cp1, 10);
      // printf(" _mTyp_mNam- M %ld\n",dbi);
    // refModels not yet stored ..
    if((cp1 > &mNam[1])&&(*cp1 == '\0')) {
      // dbi = already existing refModel; get its modelname -> syNam
        // printf(" f-MDL_mNam_mdr__-dbi=%ld |%c|\n",dbi,*cp2);
      mTyp = MBTYP_DUP;
      goto L_exit;
    }
  }


  // get ftyp = copy filetyp uppercase
  irc = UTX_ftyp_s (ftyp, mNam, 1);
  if(irc < 0) {
    if(irc == -1) {
      mTyp = MBTYP_INTERN;   // no filetyp - internal
      goto L_exit;
    }
    TX_Print("***** MDL_mTyp_mNam E1"); goto L_exit;
  }
  

  // get mmodeltyp from filetyp-UC
  mTyp = AP_iftyp_ftyp (ftyp);
  if(mTyp >= MBTYP_ERROR) goto L_exit;


  //----------------------------------------------------------------
  L_exit:
    // printf("ex-MDL_mTyp_mNam %d |%s|\n",mTyp,mNam);
  return mTyp;

}


//================================================================
  int MDL_mTyp_sNam (char *safNam) {
//================================================================
// MDL_mTyp_sNam           get modeltyp from safe-modelname
// returns modeltyp or MBTYP_ERROR

  int     inm, mTyp;

  // printf("MDL_mTyp_sNam %s\n",safNam);

  inm = MDL_iNam_sNam (safNam);
  if(inm < 0) {TX_Print("***** Err MDL_mTyp_sNam %s",safNam); return MBTYP_ERROR;}

  mTyp = MDL_MTYP_INAM (inm);

  return mTyp;

}


//================================================================
  int MDL_mTyp_set_sNam (char *sNam, int typ) {
//================================================================
// MDL_mTyp_set_sNam       set modeltyp from safe-modelname


  int     imn;


  // printf("MDL_mTyp_set_sNam |%s| %d\n",sNam,typ);


  // get nameIndex from safe-modelname
  imn = MDL_imn_mNam__sNam (NULL, sNam);
  if(imn < 0) {TX_Print("***** Error MDL_mTyp_set_sNam %s",sNam); return -1;}

  // set 
  MDL_MTYP_SET_INAM (imn, typ);

  return 0;

}


//================================================================
  int MDL_imb_mNam (char *mnam, int mode) {
//================================================================
// MDL_imb_mNam       get basicModelIndex from basicModelName
//   "-main-" returns -1;
//   "M20"    returns mbi of M20.
// Input:
//     mode     0 modNam is symbolic-name; eg "Data/Niet.gcad"
//              1 modNam is safe-name -    eg "Data_Niet_gcad"
//   retCode    -1   main,
//              -2   error;
//              >=0  ok, index
// see 
// see Mod_load_allsm Mod_get_namAll

  int      irc, i1, mdbNr, mdrNr;
  long     dbi;
  char     *p1, mnSaf[256];
  ModelBas *mdba;
  ModelRef *mdra;


  // printf("MDL_imb_mNam |%s| %d\n",mnam,mode);

//   if(!strcmp(mnam,"-main-")) { irc = -1; goto L_exit; }
// 
//   // check if modNam is ModelID ("M20");
//   if(mnam[0] == 'M') {
//     p1 = &mnam[1];
//     while(isdigit(*p1)) ++p1;
//     if(*p1) goto L_std;
//     dbi = strtol(&mnam[1], &p1, 10);
//     if(dbi < 0) goto L_err;
//     DB_get_mdr (&mdrNr, &mdra);
//     if(dbi > mdrNr) goto L_err;
//     irc = mdra[dbi].modNr;
//     goto L_exit;
//   }


  // loop tru all basicModels
  L_std:
  DB_get_mdb (&mdbNr, &mdba);
  for(i1=0; i1<mdbNr; ++i1) {
    if(mdba[i1].mnam == NULL) continue;

    if(!mode) {
      // 0 = symbolic-name
      if(strcmp(mnam, mdba[i1].mnam)) continue;

    } else {
      // 1 = safe-name
      strcpy (mnSaf, mdba[i1].mnam);
      UTX_safeName (mnSaf, 2);
      if(strcmp(mnam, mnSaf)) continue;
    }
    irc = i1;
    goto L_exit;
  }

  L_err:
  irc = -2;    // ERROR not-found

  L_exit:
    // printf(" ex-MDL_imb_mNam %d |%s|\n",irc,mnam);
  return irc;

}


//================================================================
  int MDL_imb_imr (long ir) {
//================================================================
// MDL_imb_imr             get basicModelIndex from refModelIndex


  ModelRef  *mr1;

  mr1 = DB_get_ModRef (ir);

  return mr1->modNr;

}


//================================================================
  int MDL_safename (char *safNam, char *symNam) {
//================================================================
// MDL_safename                  make safeName from symbolic-name, test length
//   retcod     0=OK, -1=name-too-long

  char   s1[SIZMFTot];

  if(strlen(symNam) >= SIZMFTot) {
    TX_Error("Length-modelname %s",symNam);
    return -1;
  }

  strcpy (safNam, symNam);
  UTX_safeName (safNam, 1);

  return 0;

}


//================================================================
  int MDL_imn_mNam__sNam (char *symNam, char *safNam) {
//================================================================
// MDL_imn_mNam__sNam              get symbolic-name and name-index from safe-name
// Input:
//   safNam       safe-name
// Output:
//   symNam       symbolic-name; NULL or size >= SIZMFTot
//   retCode      -1    error - model not exists
//                >=0   index of modelname in mdl_names
//
// was MDLMP_symMn_safMn


  int     irc, i1;
  char    safNm1[SIZMFTot], *symNm1;


  // printf("MDL_imn_mNam__sNam |%s|\n",safNam);

  // loop tru mdl_names, change to safnam, compare ..
  for(i1=0; i1<mdl_names.iNr; ++i1) {
    // get modelname[i1]
    symNm1 = MDL_MNAM_INAM(i1);
    // make safeName
    MDL_safename (safNm1, symNm1);
    // compare
    if(!strcmp(safNam, safNm1)) {
      if(symNam) strcpy(symNam, symNm1);
      irc = i1;
      goto L_exit;
    }
  }
  irc = -1;


  L_exit:
    // printf("ex-MDL_imn_mNam__sNam %d |%s|\n",irc,symNam);
  return irc;

}


//================================================================
  int MDL_mNam_usr (char *smNam) {
//================================================================
// MDL_mNam_usr            get new subModelname from user (safe)
// Output smNam       size must be >= 64
//        retCod      -1=cancel;    0=OK

  int    irc;
  char   s1[256], newNam[256];


  L_name:
  // get new subModelname
  strcpy(s1, "submodel_1");
  irc = GUI_dlg_e2b (s1, 250, "name for new submodel:", "OK", "Cancel");
  if(irc != 0) return -1;


  // make safeName from new subModelname
  UTX_safeName (s1, 1); // change '. /\\'


  // check maxLen; max SIZMFNam chars
  if(strlen(s1) > SIZMFNam) {
    TX_Print("**** max length of subModelname = %d chars - reduced",SIZMFNam);
    s1[SIZMFNam] = '\0';
  }
  strcpy(smNam, s1);


  // test if subModel smNam already exists 
  sprintf(newNam,"%sModel_%s",AP_get_tmp_dir(),smNam);
  if(OS_checkFilExist(newNam,1) == 1) {
    // sprintf(newNam,"  overwrite submodel %s ?  ",mNam);
    MSG_get_1 (newNam, 256, "OVER_mdl", "%s", smNam);
    irc = GUI_dlg_2b (newNam, "OK", "Cancel");
    if(irc != 0) goto L_name;
  }

    printf(" ex-MDL_mNam_usr |%s|\n",smNam);


  return 0;

}


//================================================================
  int MDL_mNam_safe (char *safNam, char *symNam) {
//================================================================
// MDL_mNam_safe           make safeName from symbolic-name, test length

  char   s1[128];

  if(strlen(symNam) >= 128) {
    strncpy (s1, symNam, 120);
    s1[120] = '\0';
    TX_Error("Length-modelname %s",s1);
    return -1;
  }

  strcpy (safNam, symNam);
  UTX_safeName (safNam, 1);

  return 0;

}


//================================================================
  char* MDL_safNam_mnam (char *safNam, char *mnam) {
//================================================================
// MDL_safNam_mnam              get safe-name from symbolic-name
// Input:
//   mnam         symbolic-name (can be safe-name); size must be SIZMFTot
// Output:
//   safNam       safe-name, size must be SIZMFTot
//   retCode      pointer to safNam
      
  // get safe-name of primary model
  strcpy(safNam, mnam);
  UTX_safeName (safNam, 1);

  return safNam;
      
}     

    
//================================================================
  int MDL_safNam_prim (char *safNam) {
//================================================================
// MDL_safNam_prim              get safe modelname of primary model
// Output:
//   safNam       safe-name, size must be SIZMFTot

  char     *mn;


  // get safe-name of primary model
  mn = UtxTab__(AP_modact_inm, &mdl_names);
  strcpy(safNam, mn);
  UTX_safeName (safNam, 1);

    // printf(" ex-MDL_safNam_prim |%s|\n",safNam);

  return 0;

}


//================================================================
  int MDL_msg_no_sm (int irc) {
//================================================================
// MDL_msg_no_sm       "submodel must be active"


  TX_Print ("***** Error: submodel must be active ... *****");

  // GUI_MsgBox ("ERROR: subModel must be active ..");

  return irc;

}



//================================================================
  int MDL_test__ (int mTyp, char *mNam, ModelRef *mr1) {
//================================================================
 
  int    irc;

  printf("MDL_test__ ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; \n");
  // DB_dump_ModRef ("MDL_test__");
  // DB_dump_ModBas ("MDL_test__");
  // UtxTab_dump (&mdl_names, "MDL_test__");
  // MDL_dump__ ("MDL_test__");
  printf("  _test__ mTyp=%d mNam=|%s|\n",mTyp,mNam);
  DEB_dump_obj__ (Typ_Model, mr1, "MDL_test__-in");

//   // activate temp-mode in MDL
//   MDL_prev_stat__ (OBJSTAT_temp);


  // add new model to mdl_names, mdl_tab; create baseModel
  irc = MDL_load_mdl__ (mTyp, mNam, 0, OBJSTAT_temp);
  if(irc < 0) goto L_exit;

  // disp
  irc = UI_prev_obj (Typ_Model, GR_TMP_I0, mr1, NULL);


  L_exit:
    printf(" ex-MDL_test__\n");
  return irc;
}


//================================================================
  int MDL_test_init () {
//================================================================
// MDL_test_init            init MDL with "unknown"

  int       i1;
  long      l1;
  stru_tab  *tab;

  // printf("MDL_init_0 \n");


  // clear dispList and delete all baseModels
  GL_Init1 ();


  UtxTab_init__ (&mdl_names);

  // name[0] = namMain
  UtxTab_add (&mdl_names, "unknown");

  // reset mdl_tab
  MemTab_ini__ (&mdl_tab, sizeof(stru_tab), 0, 500);

  // mdl_tab[0] = main
  MemTab_add (&mdl_tab, &l1, &_TAB_NUL, 1, 0);
  mdl_tab.data[0].mTyp = MBTYP_INTERN;


  // reset loadmap
  MemTab_ini__ (&mdl_ldm, sizeof(int), 0, 500);

//   // clear temp. modelname
//   mdl_nam_tmp[0] = '\0';


  return 0;

}


//================================================================
  int MDL_dump_lst () {
//================================================================
// test MDL_lst_sm_prim    disp list of all used Submodels of main/primary


  int        i1;
  UtxTab_NEW (lst1);


  printf("================ MDL_dump_lst ================ \n");

  // get list of all existing modelfiles ("Model_<safNam>")
  MDL_lst_all (&lst1);
  printf("\n");


  // get (symb.) list of all used Submodels of main
  UtxTab_init__ (&lst1);
  MDL_lst_sm_prim (&lst1, 0);
  printf("\n");

//   for(i1=0; i1<lst1.iNr; ++i1) {
//     printf(" %d   %s\n", i1, UtxTab__(i1, &lst1));
//   }

  UtxTab_free (&lst1);

  return 0;

}


//================================================================
  char* MDL_typInf (int mTyp) {
//================================================================
// MDL_typInf              infoText from modeltype


  static char *typTab[]={
              "application",          // MBTYP_APPLI    -3
              "catalog-part",         // MBTYP_CATALOG
              "intern-model",         // MBTYP_INTERN
              "extern-native-model",  // MBTYP_EXTERN
              "DXF-import-model",     // Mtyp_DXF
              "Iges-import-model",    // Mtyp_Iges
              "Step-import-model",    // Mtyp_Step
              "3DS-import-model",     // Mtyp_3DS
              "LWO-import-model",     // Mtyp_LWO
              "XML-import-model",     // Mtyp_XML
              "SVG-import-model",     // Mtyp_SVG    7
              "UU",                   // UU          8
              "UU",                   // UU          9
              "Tess-mockup-model",    // Mtyp_TESS
              "VRML1-mockup-model",   // Mtyp_WRL
              "VRML2-mockup-model",   // Mtyp_WRL2
              "OBJ-mockup-model",     // Mtyp_OBJ
              "STL-mockup-model",     // Mtyp_STL
              "UU",                   // UU          15
              "UU",                   // UU          16
              "UU",                   // UU          17
              "UU",                   // UU          18
              "UU",                   // UU          19
              "BMP-image",            // Mtyp_BMP
              "JPG-image"};           // Mtyp_JPG

  // printf("MDL_typInf %d\n",mTyp);


  if((mTyp < MBTYP_APPLI)||(mTyp > Mtyp_JPG)) return "unknown-error";

  return typTab[mTyp - MBTYP_APPLI];

}
  

//================================================================
  int MDL_dump__ (char *txt) {
//================================================================
// MDL_dump__                    dump loadmap mdl_ldm

  int       i1, *ia;
  stru_tab  *tab, *tab1;

  printf("\n------------------------------------------------------------ \n");
  printf("MDL_dump__  %s\n",txt);


  //----------------------------------------------------------------
  printf("  --------------- mdl_tab %d                  mdl_names\n",mdl_tab.rNr);
  tab = MEMTAB_DAT (&mdl_tab);
  for(i1=0; i1<mdl_tab.rNr; ++i1) {
    tab1 = &tab[i1];
    printf("%4d mTyp= %2d  mStat %2d mbi %4d  mUsed %2d  mMain %1d  %s\n",
           i1, tab1->mTyp, tab1->mStat, tab1->mbi, tab1->mUsed, tab1->mMain,
           UtxTab__(i1, &mdl_names));
  }


  //----------------------------------------------------------------
  MDL_dump_ldm (txt);

  //----------------------------------------------------------------
  printf("  --------------- mdl_stat\n");
      printf(" _prev_stat__ mdl_stat.iTab=%d\n",mdl_stat.iTab);
    printf(" _prev_stat__ mdl_stat.iMdr=%d .iMdb=%d\n",
           mdl_stat.iMdr, mdl_stat.iMdb);
    printf(" _prev_stat__ mdl_stat.dlTmp=%d .dlPerm=%ld\n",
           mdl_stat.dlTmp, mdl_stat.dlPerm);
    printf(" AP_modact_tmp=%d\n",AP_modact_tmp);



  //----------------------------------------------------------------
  printf("  ---------------\n");
  printf("  AP_mod_fnam   |%s|\n",AP_mod_fnam);
  printf("  AP_mod_ftyp   |%s|\n",AP_mod_ftyp);
  printf("  AP_mod_iftyp  %d\n",AP_mod_iftyp);
  printf("  AP_modact_nam |%s|\n",AP_modact_nam);
  printf("  AP_modact_ibm %d\n",AP_modact_ibm);
  printf("  AP_modact_inm %d\n",AP_modact_inm);
  printf("  UTF_FilBuf0Len=%ld\n",UTF_FilBuf0Len);  // nr of chars of primary-source
  printf("  GR_TAB_IND=%ld\n",GR_TAB_IND);          // nr of dispList-records
  printf("  GA_recNr=%d\n",GA_recNr);               // nr of graf-attributes


  //----------------------------------------------------------------
  printf("  ---------------\n");
  DB_dump_ModBas ("");

  printf("  ---------------\n");
  DB_dump_ModRef ("");

  printf("  --------------- ex-MDL_dump__\n");




  return 0;

}


//================================================================
  int MDL_dump_ldm (char *txt) {
//================================================================
// MDL_dump_ldm             dump loadmap


  int     i1, *ia;

  printf("  --------------- mdl_ldm %d    %s\n",mdl_ldm.rNr,txt);
  ia = mdl_ldm.data;
  for(i1=0; i1<mdl_ldm.rNr; ++i1) {
    printf(" %4d iNam = %3d  %s\n",i1,
           ia[i1], UtxTab__(ia[i1], &mdl_names));
  }

  return 0;

}


// EOF
