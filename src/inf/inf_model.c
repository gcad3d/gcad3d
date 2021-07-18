/* ../inf/inf_model.c

INF_MODEL__         load save baseModel refModel 


================================================================== */
void INF_Models__ (){        /*! \code

INF_MDL__                 functions in source ../xa/mdl__.c
INF_basModels__
INF_basModels_DB
INF_workflow_models
INF_workflow_model_preview
INF_workflow_model_files

INF_imp_int               native files intern
INF_rescale__             rescale active model ("Scale All")



================================================================== \endcode */}
INF_basModels__ (){        /*! \code

INF_import
INF_workflow_models
INF_refModels_DB
INF_basModels_DB
INF_DynamicDataArea



baseModel (basic-model) has its own modelcode (in file tmp/Model_<mdlNam>)
  is stored in DB in mdb_dyn[DYN_MB_IND] (DB_StoreModBas)
  is unvisible (has not position, orientation)
  primary model has no baseModel - only all its subModels
  index baseModel in DL of primaryModel is MDL_BMI_ACT (-1)
  index baseModel in DL of subModels is >MDL_BMI_ACT  (>= 0)
  numbers of baseModels are sequential starting with 0,
  numbers of baseModels have no SRC-code-number as refModels

refModel = reference to basModel.
  is stored in DB in mdr_tab[APT_MR_IND] (DB_StoreModel)
  has position, orientation and name of basModel.
  one baseModel can have n refModels.
  the number of the refModel = the SRC-modelNr (M<refMdlNr>)
  the DB-record can be active or free (DB_setFree_MR)

Example: |M20="Data_U/Niet1.gcad" P(0 0 0)|
    BaseModel = "Data_U/Niet1.gcad"
      symbolic-directory = Data_U
      Modelname = Niet1;     gcad = modelTyp
    RefModel = M20


For modifying the objects of a refModel its baseModel must be active 
  - activating makes it to the primaryModel.

mainModel is the topmost basModel.
primaryModel = the topmost active subModel

The loadSequence for all active models is defined in loadMap mdl_ldm.




// While loading subModels AP_modact_ibm is the index of the basicModel of the
//   subModel being loaded.
//  -1       primary Model is active (can also be a submodel);
//  (>= 0)   subModel is active
// After all subModels have been loaded AP_modact_ibm = -1.
// Diplaylist (DL_Att*) GR_ObjTab[].modInd gives the basicModel-index.


MDL_IS_SUB   DO NOT USE
// test if active model is a subModel being created or the active-model
//   active-model can be a subModel; see MDL_IS_MAIN




If a subModel is active -
- AP_modact_nam is the name of the active submodel;
- AP_modact_ibm is -1;
- the basicModel of the active submodel does not exist (its the primary model)
- no basicModels of the other submodels in the mainmodel exist
- for every basicModel one or more referenceModels exist.


-----------------------------------------------------------------------------
Variables:
char AP_mod_sym[SIZMFSym];       // symbolic directory for OPEN (no '/' at end)
char AP_mod_fnam[SIZMFNam];     // active mainModelname without filetyp
                           // safe, as displayed in Brw-mainRow
char AP_mod_ftyp[SIZMFTyp];      // filetyp of active mainModel
char AP_modact_nam[128];   // name of the active submodel; def="" (main)
int  AP_modact_ibm         // index into subModelList
  //  -1: primary-Model is active. The primaryModel can be subModel.
  // >=0: subModel is being created.
  // get it with AP_get_modact_ind()
int  AP_modact_inm          // index name (mdl_names) of primaryModel
int  AP_modact_tmp;         // 1=temp-model-exists; 0=not

 DL_Att.modInd       -1=mainmodel, else basicModelNr (index mdb_dyn)
 mdb_dyn              List of basicModels           see INF_basModels_DB
 mdr_tab              List of modelReferences       see INF_refModels_DB

see INF_MDL__


-----------------------------------------------------------------------------
Functions:
MDL_IS_MAIN                // test if active model is the mainModel

  AP_get_modact_ind                         get SM-index
  APT_get_line_act                          get lineNr in mainModel
  ED_get_lnr_SM                             get lineNr in subModel

  DB_mdlNam_iBas(AP_modact_ibm)             get the subModelname from  SM-index
  DB_get_ModRef       get Ditto from Index
  DB_get_ModBas       get the basicModel with index <Ind>

  dla = DL_GetAtt(DL Index);  // get DL-record
  model_nr = dla.modInd;      // -1=active Model, 0-n=Submodel

  ModelBas * mdr;
  // the BasicModelRecord of a Submodels get
  mdr = DB_get_ModBas (dla.modInd);
  // mdr >mnam is now the Modelname of the Submodel.



-----------------------------------------------------------------------------
Files in <tempDir>:
- see INF_workflow_model_files



================================================================== \endcode */}
INF_workflow_models (){        /*! \code

INF_workflow_mod_sav

see also INF_basModels__           (variables)



................. Load_startModel:
UI_GL_draw__(init)
  AP_mdl_init           // init all, kill all files in <tmpDir>
  AP_init__             // work startup-parameters
    AP_work__
      MDLFN_set__            // get name of model to load -> AP_mod_*
  AP_Mod_load_fn        // load Model from file <fn>
    // see AP_Mod_load_fn


................. Load_open_model:
UI_men__ "open"
  MDL_sav_ck__ (0);    // check if model is modified; if yes: save
  UI_file_open__       // get new filename (and directory) from user
  AP_mdl_init          // init all
  AP_Mod_load_fn       // get AP_mod_sym,AP_mod_dir,AP_mod_fnam,AP_mod_ftyp ..


................. Load_last_used:
UI_open_last
  AP_mdl_init            // init all
  AP_Mod_load_fn        // load Model from file <fn>
    // see AP_Mod_load_fn


................. Load_open_new
UI_men__"new"      
  MDL_sav_tmp
  MDL_sav_ck__ (0);        // check if model is modified; if yes: save
  AP_mdl_init 
  AP_Mod_load_fn ("", 2);


................. load_mainModel_remote
CTRL_CB_do__ "LOAD"
  AP_Mod_load_fn (wPos1, 0);


//----------------------------------------------------------------
AP_Mod_load_fn        // load Model from file
 /MDL_load_new__        // if no modelname given -
    MDL_init_noCad         // create tmp/Model_unknown
    MDL_init_MDL           // init mdl_names, mdl_tab
    UTF_add_fil_init       // load prim.model into memSpc
 |MDL_load_main         // model import, load into DL, prim.model into memSpc
    MDL_load_noNat        // load mockup; create internal-native-model
    MDL_init_MDL          // init mdl_names, loadmap 
    MDL_load_1            // import all submodels (mStat 1)
      MDL_load_import__     // import
       /MDL_load_import_ctlg  // import catalog
       |MDL_load_import_ext   // import gcad,dxf, stp, igs, ..
      MDL_load_scan__         // scan for submodels
      DB_StoreModBas        // create basicModel
    UTF_add_fil_init      // load prim.model into memSpc
  Brw_init__            // update browser


................. load_submodel_new
UI_men__ |mod_cre|
  MDL_load_mdl_new
    MDL_mdl_add
    MDL_init_noCad


................. load_submodel_from_main
UI_men__ |mod_m2s|
  MDL_load_mdl_main
    MDL_mdl_add
    MDL_init_noCad
    MDL_load_mNam__


................. load_submodel_from_group
Grp_SM
  Grp_res
  Grp_exp                // group-objs -> file tmp/selection1.gcad
  MDL_load_mdl_grp       // create subModel from objs in file
    MDL_mdl_add
  Del_grp__              // delete group


................. load_submodel_from_file
UI_men__ "mod_loa"
  MDL_load_mdl_f
    MDL_mdl_add


................. load_subModel_remote
CTRL_CB_do__ "LOADSM"
  MDL_load_mdl_f (wPos1);


................. CAD-Load-dynamic-model:
// CAD: preview model as dynamic-modeL; only one dynamic model possible
IE_cad_test__                 compose, test, preview src from CAD-inputfields
  ED_work_CAD                   decode, store in DB, (pre)view src-codeLine
    APT_decode_model              decode atomic-objCode from src-codeLine
      // see below


................. CAD-delete-refMdl
OMN_CB_popup   // MSG_delete
  Del_obj__
    Del_tab__
      UNDO_grp_del
        // change def.Line "M20=.." into "_M20=.." ..


................. MAN-Load-model:
// MAN: create perm. model, but hilite;
// - go back: delete model (delete DL-objs, reset its mStat to 1; set DLsiz=-1)


// load model from src-code:
APT_decode_model
  MDL_load_dyn                add new model, temp or perm
    MDL_load_mdl__                add new model
      MDL_load_1                  import and scan all models in mdl_tab
      MDL_load_2                  create loadmap, load all models
        MDL_ldm__                   create loadmap for model 
          MDL_ldm_add_flst            add all subModels from file, recursive
            MDL_ldm_add__               add nameIndex to loadMap mdl_ldm
        MDL_mdb_load__              load all new models into dispList


................. MAN-delete-src-to-end
EDI_CB__                  // line in editor changed ..
  ED_work_CurSet            // set to new line in scr
    DL_disp_reset             // set back in scr (remove lines)
      MDL_mdb_clear_1              // clear baseModel
      MDL_mdr_clear              // clear refMdl
      GL_Delete                 // free dispList




................. activate_model:
OMN_CB_popup
  MSG_activate:
  MDL_load_mNam__         // activate main- or internal model
    MDL_LDM_CLEAR         // clear loadmap mdl_ldm
    GL_Init1              // clear dispList
    MDL_mdr_clear         // clear all refModels
    MDL_mdb_clear__       // reset mdl_tab and baseModels
    MDL_load_mdl__          // add (activate) model
      MDL_load_1            // import and scan all models in mdl_tab
      MDL_load_2            // create loadmap for model imn, load all models
        MDL_ldm__             // create loadmap
        MDL_mdb_load__        // load models



................. rename_model:
OMN_CB_popup
  Brw_sMdl_ren__
    MDL_ren__          // rename model
      MDL_ren_1          // rename model
      Brw_sMdl_ren_CB    // update modelname in brw


................. remove_model
OMN_CB_popup
  MSG_delete:
  Brw_sMdl_del__         // delete subModel and childs and activate main
    MDL_rem__              // delete subModel
      MDL_rem_del            // delete (used) active primary model 
      MDL_rem_mdl            // delete unused childs of active primary model
       MDL_rem_ck_used         // check if model is used
       MDL_rem_del             // delete unused model
    MDL_load_mNam__          // activate main




================================================================== \endcode */}
INF_workflow_mod_sav (){        /*! \code
//================================================================

................. save_exit:
AP_exit__               // exit app
  MDL_sav_tmp             // save the active model into tempFile
  MDL_sav_ck__               // check if model is modified
    MDL_sav_ck_new             // test new (imported model)
    MDL_sav_ck_ck              // check if model is modified
      MDL_sav_gcad (0,0)       // create complete modelfile tmp/Mod_.mod_out
      MDL_sav_cmp__            // compare tmp/Mod_.mod_in - tmp/Mod_.mod_out
    AP_save__ (0, 0, 0, "gcad");
  AP_defaults_write       // save defaults -> cfg/xa.rc


.................  save_as 
UI_men__ |exp1nat|
  set AP_stat.zip   // 0=not checked; 1=checked
  AP_save__ (0, 0, 2, "gcad");


................. save_active_sm_to_file        mod_sav
UI_men__ |mod_sav|
AP_save__ (2, 0, 1, "gcad")


................. save_quick:         Ctrl-s
UI_men__ "qSav"
  AP_save__ (0, 2, 0, "gcad")


................. Group_save:
UI_men__ "grpSav"                  // save Group -> modelFile
  Grp_Mdl                            // save Group -> modelFile
    Grp_res                            // add Recursiv all parents
    AP_save__ (1, 0, 1, "gcad");         // export native
      Grp_exp




.................
AP_save__
  GUI_file_over           // ask for overwrite
  GUI_file_save__         // get filename from user
  MDL_file_smuu           // ask for save unused-models
 /MDL_exp__               // save|export main
    MDL_sav__               // save native
      MDL_sav_gcad            // save native
 AP_mod_sav_ok            // rename Mod_.mod_out -> Mod_.mod_in
 |Grp_exp                 // save|export group only
 |MDL_sav__               // save|export active model only





??????????????????????????????????
- to check:


....................................................................
................. MDL_save_as 
UI_men__ |exp1nat|
  AP_save__ (0,0,2,"gcad|gcaz") // save model group or subModel
    GUI_file_save__             // get filename from user
    GUI_file_over_              // ask for / delete unused subModels
   -MDL_exp__                   // 
     -MDL_sav__                   // save OR <export-models>
        MDL_sav_gcad                // save model native -
          MDL_sav_tmp
     |<export-models>
   |Grp_exp
   |Mod_sav2file_CB
  


<export-models>
  OS_dll_do ("xa_dxf_w"
  OS_dll_do ("xa_stp_w",
  AP_ExportIges__
  OS_jpg_bmp
  TSU_exp__


.................MDL_save_quick:         Ctrl-s
UI_men__ "qSav"
  AP_save__ (0, 2, 0, "gcad")


.................MDL_save_exit:
AP_exit__
  AP_save__ (0, 1, 3, "gcad");


.................Model_save:
UI_men__ "mod_sav"            // "save active Submodel to File"
  AP_save__ (2, 0, 1, "gcad")


//----------------------------------------------------------------
Functions:
//   Mod_sav_(-1)          copy empty model -> tmp/Model
//   Mod_sav_i (0);        save Model+Submodels into tempDir as "Model" nativ
//   Mod_sav_ck (0);       make a copy of Model for ck-modified



================================================================== \endcode */}
INF_workflow_model_files (){        /*! \code


Files in <tempDir>:
  Model_<safNam>        native source of single subModel
  Mod_<safNam>.lst      list of names of used submodels of model <safNam>
  Mod_<safNam>.tess     tesseleted model (.wrl .obj .stl ..)

  Mod_.act_in           the active used model
  Mod_.mod_in           a copy of Model for check if modified
  Mod_.mod_out          the complete model after prg.exit
  Mod_.tmp_utf          store / reload UTF_FilBuf0 UTF_save__ UTF_load__
  Mod.lst               file with list of mdl_tab (for user-selection) MDL_file_lst__
                        or loadmap (MDL_ldm_f)

  processes.lst


  MdlLst.txt            list of models last used

  Exp_<subModelname>    last exported subModel
  M#A#.msh              binary; Mesh-Faces & Edgelines.
  M#A#.ptab             binary; Mesh-points.
  <subModelname>.tess   externes Mockup-subModel (zB wrl, stl)

  Catalog.lst     eine Liste aller existierenden CatalogFiles
  Catalog.act     der Filename des momentan aktiven CatalogFile
  CatParts.lst    die PartList des momentan aktiven CatalogFile



================================================================== \endcode */}
INF_workflow_model_preview (){        /*! \code

preview temporary submodels

Variables:
AP_modact_tmp         1=temp-model-exists; 0=not
mdl_stat              struct with indexes of objects belonging to active prev.obj
mdl_stat.iTab         index mdl_tab & mdl_names (remove all following)


Functions:
MDL_prev_stat_set     fill mdl_stat with indexes (objs of prev.obj start here)
MDL_prev_stat_del     delete all objs & data of active prev.obj

MDL_prev_brw_add      add modelRows to browser (when making prev.obj perm)



================================================================== \endcode */}
INF_rescale__ (){        /*! \code

rescale active model ("Scale All")

- load new model with submodels;
  - load sm into DL (MDL_mdb_load_1):
    - test if baseMdl.pm2 is set; if not: get box (UT3D_box_mdl__ (AP_modact_ibm))




Variables:
AP_stat.mdl_box_valid                // 0=valid, 1=not-valid
AP_box_pm1, AP_box_pm2               // box around mainmodel
(ModelBas)<basicModel>.pb1, .pb2     // box around subModel


Functions:
DL_ReScale__                 // rescale and redraw active model
DL_scale_grp                 // scale group
int DB_isFree_PT (pb2)       // 1=box-is-free=not-set, 0=box-is-set-valid;
AP_mdlbox_invalid_ck         // check if modelbox is valid (AP_stat.mdl_box_valid)
AP_mdlbox_invalid_set               // set  modelbox = 1=not-valid
AP_mdlbox_invalid_reset             // set  modelbox = 0=valid
UT3D_box_mdl__               // get box of active model

APT_work_AppCodTab ("MODBOX")       // set AP_box_pm1/pm2
AP_stat_file                 // file write|read AP_box_pm1,pm2 AP_stat.mdl_box_valid


TODO:
AP_stat.mdl_box_valid   unused

Check in MDL_sav_tmp if model is modified; (see MDL_sav_ck__)
- if yes: recompute box with UT3D_box_mdl__ and set AP_stat.mdl_box_valid or
  AP_stat.mdl_modified




================================================================== \endcode */}
// eof
