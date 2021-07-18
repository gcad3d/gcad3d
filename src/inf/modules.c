/* infotext.c                 RF                     2016-12-29
 *
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
Infos gcad3d modules.
Contains infotext only, no code.
Use it with "vi -t <code>"



================================================================== */
void INF_Brw__(){                   /*! \code
obj-browser
See INF_Brw_gtk

Mousebuttons on obj in browser-window:

Vector und all not-visible-objs:
- M1 - preview from dbi and add to group
- M3 - menu hide delete
Visible-objs:
- M1 - hilite / add to group
- M3 - menu hide delete
SubModels:
- M1 - hilite all refMdls of this baseMdl
- M3 - Menu activate (rename, remove)


//----------------------------------------------------------------
PROGRAMFLOW:


--------------------PRIMARY_LOAD:
Brw_init__
  Brw_Mdl_init            // create all subModelNodes from Mod.lst
    Brw_Mdl_createRow
  Brw_Prcs_Init ();        // create all processes
  Brw_Mdl_upd              // update and open obj-rows of primary-model


--------------------LOAD_FROM_BRW:
MDL_load_mNam__
  Brw_Mdl_activ      // activate subModel in Browser
    Brw_Mdl_upd        // update and open obj-rows of primary-model
    Brw_Mdl_stat       // set icons of models according stat


--------------------REMOVE_FROM_BRW:
Brw_sMdl_del__
  Brw_del_oid          // delete browser-row and its childs from obj-ID


//----------------------------------------------------------------
FUNCTIONS:
if(BRW_STAT) ..   // check if Browser is active

Brw_CB_sel         callback tree-obj-selection
OMN_popup_Brw      create & display popup-menu
OMN_CB_popup       callback select menu of browser



//----------------------------------------------------------------
FILES:
../xa/xa_brw.c
../xa/xa_brw.h      Brw_getTxt


================================================================== */
void INF_Brw_gtk(){                   /*! \code
obj-browser

Obj-line has 4 columns:
Co1 0 = icon
Co1 1 = text
Co1 2 = state: 1=act, 0=pass
Co1 3 = iconNr

Functions:
GUI_tree1_analyz1

GUI_tree1_iter_string        find row that starts with <txt>

// gtk_tree_model_get_iter_from_string

// get iter: gtk_tree_model_get_iter  

// analyze column: gtk_tree_model_get ( In: iter, colNr);








================================================================== \endcode */}
void INF_PRCV(){                   /*! \code
polygonal_representation_of_curve    ../ut/ut_prcv__.c
                                     ../ut/ut_prcv_db.c

PRCV keeps all points of the polygonal representation of all curves.

If the curve is used as trimmed curve, then the start- and endpoint of the
  trimmed curve is also stored in the PRCV.
  This knotPoints have also stored the parameter and the DB-index.

That means that all applications of a curve (or parts of a curve) use
  the same set of points; eg 2 surfaces sharing the same boundarycurve
  use the same points.




programFlow:        --------------------
APT_work_def        Work DefinitionLine 
  APT_store_obj       decode all parameters, create binary obj; store obj in DB
    APT_decode_ccv__    decode all parameters, create binary obj
    DB_StoreCurv        store obj in DB
  PRCV_set_dbo__      create PRCV 
    PRCV_set_obj_dbi   create/update PRCV for dbo
      PRCV_set_basCv       create PRCV for baseCurve

GR_set_ccv        display composite-curve
  UT3D_mtpt_trmCv
    PRCV_mtpt_trmCv
      PRCV_get_dbo_add_tc    get curve from PRCV 
        PRCV_get_tc_find     find parameter or DB-poit-index in PRCV
        PRCV_get_tc_add_prc


internal:           --------------------
PRCV_GRP is a MemTab(CurvPrcv) = all curves, fixed size = sizeof(CurvPrcv);  

PRCV_DB_save does:
  - add the CurvPrcv to PRCV_GRP;
  - Store data:
   - malloc space for all data (points,parameters,DBis)
   - copy data, store the location in the CurvPrcv-record in PRCV_GRP.
   - the malloc-location (for free) is (CurvPrcv).npt.
   - for every curve one malloc is done.


mainFunctions:      --------------------
PRCV_set_dbo__      create PRCV for DB-obj (typ/dbi)
PRCV_npt_dbo__      get polygon for DB-obj
PRCV_DB_save
PRCV_DB_load



================================================================== \endcode */}
void INF_CNTF(){                   /*! \code
CNTF                  find and create contour (CCV = trimmed-curves)

used for creating composite-curve from points,lines,circles,curves,trimmed-curves.
eg "S20=CCV P31 P41 C21 P43 P42 C20 P40 P30"

--------------------------------------------------------------------
TODO:
- CNTF tries to find normal exit (perpendicular, right-angled) from curves;
  but sometimes -
  - tangential exit is wanted or
  - direct connection to endpoint of curve is wanted
  Add checkBoxes "parallel" and "normal" to the CCV-Menu; 
    compute the appropriate point, add this point into output ..


--------------------------------------------------------------------
CNTF_init__

CNTF_add__
  CNTF_cvco_pt_pt
  CNTF_cvco_pt_lfig
  CNTF_cvco_lfig_pt
  CNTF_cvco_lfig_lfig
    CNTF_out_old

CNTF_exit__
  CNTF_out_old




================================================================== \endcode */}
void INF_EDMPT(){                   /*! \code


OMN_CB_popup
  OS_dll_do ("xa_edmpt", "EDMPT__", &pa, 1);


EDMPT__
  EDMPT_init
    APED_find_dep__          // find all depending objects (childs)
    EDMPT_dep_sav            // write sourcecodes of all depending objects 
                             // -> file <tmpDir>/src_dep.txt


../xa/edmpt.c


================================================================== \endcode */}
void INF_Search(){                   /*! \code

ckb_such    // checkbox "Search/Name"
UI_mcl__
UI_mcl_ctrl        ../xa/xa_objLst.c


================================================================== \endcode */}
// eof
