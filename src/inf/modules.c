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
void INF_PRCV(){                   /*! \code
polygonal_representation_of_curve    ../ut/ut_prcv__.c

PRCV keeps points of the polygonal representation of all curves.
If the curve is used as trimmed curve, then the start- and endpoint of the
  trimmed curve is also stored in the PRCV.
That means that all applications of a curve (or parts of a curve) use
  the same set of points; eg 2 surfaces sharing the same baoundarycurve
  use the same points.




programFlow:        --------------------
APT_work_def        Work DefinitionLine 
  APT_store_obj       decode all parameters, create binary obj; store obj in DB
    APT_decode_ccv__    decode all parameters, create binary obj
    DB_StoreCurv        store obj in DB
  PRCV_set_dbo__      create PRCV 
    PRCV_set_obj_dbi   create/update PRCV for dbo
      PRCV_set_basCv       create PRCV for baseCurve

GR_DrawCvCCV        display trimmed-curve
  UT3D_mtpt_trmCv
    PRCV_npt_trmCv
      PRCV_get_dbo_add_tc
        PRCV_get_tc_find     find parameter or DB-poit-index in PRCV


mainFunctions:      --------------------
PRCV_set_dbo__      create PRCV for DB-obj (typ/dbi)
PRCV_npt_dbo__      get polygon for DB-obj



================================================================== \endcode */}
void INF_CNTF(){                   /*! \code
CNTF                  find and create contour (CCV = trimmed-curves)

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


../xa/xa_edmpt.c



================================================================== \endcode */}
void INF_Search(){                   /*! \code

ckb_such    // checkbox "Search/Name"
UI_mcl__
UI_mcl_ctrl        ../xa/xa_objLst.c


================================================================== \endcode */}
// eof
