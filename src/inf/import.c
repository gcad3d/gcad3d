/* ../inf/import.c

================================================================== */
void INF_import(){                   /*! \code

Sources:
../gr/tess_ut.c             export/import tesselated faces obj,stl,dxf ..


the native model consists of:
- intern-native-models;                                INF_imp_int
- references to native-files                           INF_imp_ext
- references to CAD-files (dxf, iges, step ..)         INF_imp_cad
- references to mockup-files-native (3ds lwo)          INF_imp_mock1
- references to mockup-files (obj, wrl, stl)           INF_imp_mock2
- references to images (jpg, bmp)                      INF_imp_img
- references to catalog-models (ctlg)                  INF_imp_ctlg



================================================================== \endcode */}
void INF_imp_int(){                   /*! \code
native files intern
MBTYP_INTERN
- are included in the native modelfile; extracted when importing a native file.

- format in native-file is:
SECTION MODEL <mdlnam>
<modelCode>
SECTIONEND

- Flow see INF_imp_ext
- extracted into file tmp/Model_<mdlnam> at import in -
  MDL_imp__ > MDL_load_file_gcad > MDL_load_file_split


OLD_VERSION:
MODEL <mdlnam>
..
MODEL END



================================================================== \endcode */}
void INF_imp_ext(){                   /*! \code
native-files                           INF_imp_ext
MBTYP_EXTERN

Flow:
- import modelfile as tmp/Model_<modelName> (MDL_imp__())
- find and load all subModels, create loadmap (MDL_load_add(), recurs.)
- create baseModels, load into DL  (MDL_load_load())



================================================================== \endcode */}
void INF_imp_cad(){                   /*! \code
CAD-files (dxf, iges, step ..)         INF_imp_cad
Mtyp_DXF Mtyp_Iges Mtyp_Step Mtyp_3DS Mtyp_LWO Mtyp_XML Mtyp_SVG

Import:
- translates cad-modelfile into native-gcad-models and submodels.
- is done in MDL_imp__
  - eg for DXF by:  OS_dll_do ("xa_dxf_r", "DXF_r__", pa, 0);
- Flow see INF_imp_ext



================================================================== \endcode */}
void INF_imp_mock1(){                   /*! \code
- references to mockup-files-native (3ds lwo)          INF_imp_mock1

//----------------------------------------------------------------
Import_LWO:                           creates native code ...
- create tmp/Model_<mnam>_lwo
  - with primary P=..; A=RCIR..;
- add modelname into Mod_<parent>.lst

MDL_load_import_ext
  AP_ImportLwo

//----------------------------------------------------------------
Import_3DS:                           creates native code ...
- create tmp/Model_<mnam>_3ds
  - with primary P=..; A=RCIR..;
- add modelname into Mod_<parent>.lst

MDL_load_import_ext
  AP_Import3ds



================================================================== \endcode */}
void INF_imp_mock2(){                   /*! \code
mockup-files (tess obj, stl, wrl (V1 u V2))        INF_imp_mock2
Mtyp_TESS Mtyp_STL Mtyp_WRL Mtyp_WRL2


M20="Data/samp_anilin.wrl" P(0 0 0)
M25="Data_U/sample_mod_nut1.tess"


Flow:
- create native-internal-model in MDL_load_noNat
- create baseModel in MDL_load_scan__
- create refModel in APT_decode_model
- display Mockup: GR_set_mdr > GR_mdMock_imp ..
  - import = produce tesselated-binary model;
    write -> file tmp/Mod_<safNam>.tess;
    load tesselated-binary indo DL;


GR_set_mdr           // disp subModel from modelreference
  GR_mdMock_imp        // import VRML|OBJ|STL-file - save as tmp/Data_<moldel>.tess
    DLL_run1             // load model into memSpc
   -tess_write__         // write .tess-file into temp-dir
   |TSU_imp_tess         // read tess-file into memSpc
      tess_read_f
    TSU_DrawSurTess      // load all records of memSpc into GL-list


//----------------------------------------------------------------
Import_OBJ:                           creates tesselated model (binary)
- create Mod_<mnam>.tess

GR_mdMock_imp
  DLL_run1 "xa_obj_r"
  tess_write__

//----------------------------------------------------------------
Import_STL                           creates tesselated model (binary)
- create Mod_<mnam>.tess

GR_mdMock_imp
  DLL_run1 "xa_stl_r"
  tess_write__

//----------------------------------------------------------------
Import_VR1:                           creates tesselated model (binary)
- create Mod_<mnam>.tess

GR_mdMock_imp
  DLL_run1 "xa_wrl_r"
  tess_write__

//----------------------------------------------------------------
Import_VR2:                           
- not active - creates PTAB and MESH - SECTIONs; needs new surface concept
  for indexed surface + bodies (= closed shells = solids)

GR_mdMock_imp
  DLL_run1 "xa_vr2_r"





================================================================== \endcode */}
void INF_imp_img(){                   /*! \code
images (jpg, bmp)                      INF_imp_img
Mtyp_BMP
Mtyp_JPG (also translated into bmp-format by ?)

CAD-code:
N20=IMG P(0 0 0) "Data_U/DachWS4.jpg"


- has a baseModel - mnam, typ, dli, dlsiz set; pb2 = size
- no refModels


Flow:
- create baseModel by ?
- disp image: GR_set_txt__ > GL_set_BMP > bmp_load
  - import imagefile as tmp/Data_<safNam>.bmp   (eg "Data_U_DachWS4_jpg.bmp)


------------------------------------------
GUI_img__ // disp Pixmap  (.xpm or .jpg or .png)
OS_get_imgConv*          get Pixmap-converters
OS_jpg_bmp               convert BMP -> JPG
bmp_load    load bmp-file into memory
bmp_save__  save pixmap (open GL-window) as bmp-file
MDL_exp__    


================================================================== \endcode */}
void INF_imp_ctlg(){                   /*! \code
catalog-models (ctlg)                  INF_imp_ctlg
MBTYP_CATALOG


MDL_load_import__
  CTLG_Part_Ref1     Create tmp/<catalog>_<part>.ctlg_dat (parameters only)
                     First line is the full filename of the modelname !
  - does not copy the modelfile into tmpDir;



MDL_load_sbm
  ED_Run
  WC_Work1 "CALL CTLG" does -
    APT_work_TPC_CALL
      if T_CTLG - 
      - CTLG_fnWrite_modelnam     get filename of catalog-datafile 
      - and execute as macro



================================================================== \endcode */}
// eof
