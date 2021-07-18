//    ../ut/ut_types.c
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
  remame this file ../ut/ut_otypes.c/h

-----------------------------------------------------
Modifications:
2009-05-28 Created. RF.

-----------------------------------------------------
*/
/*!
\file  ../ut/ut_gtypes.c
\brief get|set|check object typ, index, ID
\code
=====================================================
List_functions_start:

APED_dbo_oid        give typ, index from text "P12"
APED_oid_dbo__      make name from typ and DB-Index  (visible object types)
APED_oid_dbo_sm     object-id from typ and DB-Index and subModelname
APED_oid_dbo_all    make name from typ and DB-index  (all types)
APED_oid_vc         get oid for Vector; (DX or DIX or D#)
APED_dbi_src_std_vc_pl get dbi for std-vector or -planes from GcoTxtTab-index

AP_typ__ck_ato__    check if ato has components of requested type
AP_typ__ck_ato_b    check if ato has components of requested basicType
AP_typLst_grpTyp    get a list of basicTypes for a selectionGroupType
UTO_ck_dbsTyp       check object-typ (struct or object)
UTO_ck_typTyp       check if typ=curve (Typ_lFig) or surface (Typ_SUR)
UTO_ck_symTyp       check if typ is symbolic (no grafic repres.)
UTO_ck_curvForm     check if objTyp is ObjGX or other typ

AP_typ_srcExpr      decode expression
AP_typ_FncNam       get type of function

AP_typ_typChar      make typ from typChar  ("P" -> Typ_PT)
AP_typChar_typ      make typChar from typ  (Typ_PT -> 'P')
AP_typDB_typ        Typ_CVBSP -> Typ_CV   oder Typ_SURRU -> Typ_SUR
AP_cmp_typ          check for identical types

AP_src_typ__        make text from (int)typ; Typ_PT --> "PT" ..
DB_Typ_Char         give typ(int) from text "PT" --> Typ_PT;

List_functions_end:
=====================================================

\endcode *//*----------------------------------------




*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>                     // toupper

#include "../ut/ut_geo.h"
#include "../ut/ut_txt.h"                // UTX_pos_skipLeadBlk
#include "../db/ut_DB.h"                 // DB_VCX_IND DB_PLX_IND

#include "../gui/gui_types.h"

#include "../ut/ut_gtypes.h"

#include "../xa/xa_sele.h"               // Typ_FncPrv
#include "../xa/xa.h"                    // MDL_IS_SUB




//============ Extern Var: =====================
// ex ../xa/xa.c



//============ Locale Var: =======================================


//----------------------------------------------------------------
  static char *TypTxtTab0[]={
//0         1         2         3         4
//5         6         7         8         9
  "Error",  "Var",    "Vec",    "PT",     "LN",        // 0-
  "CI",     "-",      "-",      "-",      "-",
  "VC2",    "PT2",    "LN2",    "CI2",    "CI2C",      // 10-
  "CVLN3",  "------", "------", "------", "------"};

  static char *TypTxtTab20[]={
  "CV",     "CvPlg",  "CvPsp3", "CvBsp",  "CvRBsp",    // 20-
  "CvEll",  "CvBez",  "CvRBez", "Clot",   "CvLnTab",
  "CV2",    "CvPlg2", "CvPsp2", "CvBsp2", "CvRBsp2",   // 30-
  "CvEll2", "CvEll2C","CvBez2", "CvCcv",  "-"};

  static char *TypTxtTab40[]={                         // 40-
  "PLN"};

  static char *TypTxtTab50[]={
  "SUR",    "SurCon", "SurTor", "SurRU",  "SurRV",     // 50-
  "SurSwp", "SurBsp", "SurRBsp","SurPln", "SurTP",
  "SurHat", "SurCir", "SurStrip","SurBnd","SurMsh",    // 60-
  "SurPtab","SurFac3","SurFacQ","SurTri", "SurSup",
  "SurGL_", "SurGLpp","SurGLpo","SurGLis","SurEdg"};   // 70-

  static char *TypTxtTab80[]={
  "SOL",    "Sphere", "Conus",  "Torus",  "Prism",     // 80-
  "BREP"};

  static char *TypTxtTab90[]={
  "Note",   "TxtA",   "TxtG",   "Dimen",  "Dim3",      // 90-
  "Tag",    "Joint",  "------", "------", "------"};

  static char *TypTxtTab100[]={
  "SymBmp", "SymVec", "SymRef", "SymRef2","------",    // 100-
  "Texture","TEXB",   "TEXR",   "------", "------"};

  static char *TypTxtTab110[]={
  "BBox",   "BBox2",  "GrdBox",  "EdgLn", "------",    // 110-
  "TmpPT",  "Vertex", "EyePT",   "TmpGrp","------"}; 

  static char *TypTxtTab120[]={
  "Part",   "Grp",    "ModBas", "Model",  "Mockup",    // 120-
  "Dit",    "------", "CtlgPart","GEOB_2D","GEOB_3D"};

  static char *TypTxtTab130[]={   /// geom. parameters
  "Val",    "X-coord","Y-coord","Z-coord","Angle",     // 130- TYP_IS_GEOMPAR
  "Rad",    "Dist",   "------", "------", "------",
  "Par1",   "UVal",   "VVal",   "------", "------",    // 140-
  "------", "------", "Pt-Std", "Pt-Ind", "Seg-Nr"};

  static char *TypTxtTab150[]={   /// attributes
  "Typ",    "Subtyp", "cmdSub", "Address","Size",      // 150-
  "Index",  "Name",   "Color",  "LnTyp",  "Thick",
  "G_Att",  "Activ",  "Layer",  "AppObj", "AppDat",    // 160-
  "dynSym", "PRCV",   "constPln","Proc",  "------"};

  static char *TypTxtTab170[]={   /// dataFormats
  "Data",   "Int1",   "Int2",   "Int4",   "Int8",      // 170-
  "Float4", "Float8", "-",      "-",      "-",
  "poly--", "poly1-", "poly3-", "polyD3", "-",         // 180-
  "Mat3x2", "Mat3x3", "Mat4x3", "Mat4x4", "-"};        // 185-
  static char *TypTxtTab190[]={   /// text
  "Text",   "String", "StrDel", "StrDel1","FncNam",    // 190-
  "FilNam", "NumString","ConstVal","ConstOG","------"};
  static char *TypTxtTab200[]={   /// containers
  "Memspc", "MemTab", "IndTab", "IgaTab", "ObjSRC",    // 200-
  "ObjGX",  "ObjTab", "ObjBin", "ObjDB",  "ObjAto"};   // 205-
  static char *TypTxtTab210[]={   /// transformations
  "Tra",    "TraTra", "TraRot", "TraMat", "------"};   // 210-
  static char *TypTxtTab220[]={   /// operators
  "Ope__",  "OpeEQ",  "OpeNE",  "OpeLT",  "OpeGT",     // 220-
  "OpeGE",  "OpeLE",  "OpeAND", "OpeOR",  "Mod"};
  static char *TypTxtTab230[]={                        /// modifiers  TYP_IS_MOD
  "ModRep", "Mod1--", "Mod2--", "ModCWCCW","ModREV",   // 230-
  "ModCX",  "ModCTRL","ModPERP","ModPARL","ModHIX",
  "Mod---", "Mod---", "Mod---", "Mod---", "Mod---",    // 240-
  "ModCCW", "ModCW",  "ModIN",  "ModOUT", "ModAux",
  "ModUNL", "------", "------", "------", "------",    // 250-
  "------", "------", "------", "------", "Undef" };
  // static char *TypTxtTab250[]={   /// events
  // "MsBtL",  "MsBrM",  "MsBtR",  "MsBt2L", "------",    // 250-
  // "------", "------", "------", "------", "------"};
  static char *TypTxtTab260[]={   /// functions
  "FuInit", "FuInit1","FuMod",  "FuAdd",  "FuDel",     // 260-
  "FuEnd",  "FuExit", "Fu----", "Fu----", "Fu----"};
  static char *TypTxtTab270[]={   /// function parameters
  "ALL",    "Done",   "NULL",   "Last",   "SIZ",       // 270-
  "------", "------", "------", "------", "------",
  "OpmPlus","OpmMinus","OpmMult","OpmDiv","------",    // 280-   TYP_IS_OPM
  "------", "------", "------", "------", "------",
  "FcmSQRT","FcmSIN", "FcmCOS", "FcmTAN", "FcmASIN",   // 290-   TYP_IS_FCM
  "FcmACOS","FcmATAN","FcmABS", "FcmFIX", "FcmRND"};
  static char *TypTxtTab1000[]={   /// selectionGroups
  "goGeom", "goPrim", "goGeo1", "goGeo2", "goGeo3",    // 1000-
  "goGeo4", "goGeo5", "goSUSU", "goGeo6", "goGeo7",
  "goGeo8", "goAxis", "goLR",   "go----", "go----",    // 1010-
  "go-lf1", "go-lf2", "go----", "go----", "go----"};
  static char *TypTxtTab1020[]={   /// selectionModifiers
  "FncVAR1","FncVAR2","FncVC1", "FncVC2", "FncNxt",    // 1020-
  "FncPrv", "FncDirX","FncOnO", "FncOnC", "Fnc---"};
  static char *TypTxtTab1100[]={   /// aux
  "ValSymTyp","StrDir1","Obj0", "------",  "------"};  // 1100-
//  0         1         2         3         4
//  5         6         7         8         9



//----------------------------------------------------------------
  static char  *Fc1TxtTab[] = {  // geom functions
// 00             1              2              3              4
  "VAL",         "ANG",         "RAD",         "MOD",         "NEW",         
  "PTS",         "PTI",         "SEG",         ""};

// the corresponding types for Fc1TxtTab
  static int Fc1TypTab[]={
  Typ_Val,       Typ_Angle,     Typ_Rad,       Typ_modif,     TYP_FuncInit,
  Typ_PTS,       Typ_PTI,       Typ_SEG};


//----------------------------------------------------------------
  static char  *FcmTxtTab[] = {    // math functions
// 00             1              2              3              4
  "SQRT",        "SIN",         "COS",         "TAN",         "ASIN",
  "ACOS",        "ATAN",        "ABS",         "FIX",         "RND",
  ""};


char  *NcoTxtTab[] = {  // num. constants
// 00             1              2              3              4
  "PI",          "RAD_1",       "RAD_90",      "RAD_180",     "RAD_360",
  "SR_2",        "SR_3",
  ""};

double NcoValTab[] = {  // values for NcoTxtTab; see ATO_srcTxt
  RAD_180,       RAD_1,        RAD_90,        RAD_180,       RAD_360,
  SR_2,          SR_3};


char  *GcoTxtTab[] = {  // geom. constants see APED_dbi_src_std_vc_pl
// 00             1              2              3              4
  "DX",          "DY",          "DZ",          "DIX",         "DIY",
  "DIZ",         "RX",          "RY",          "RZ",          "RIX",
  "RIY",         "RIZ",         ""};

int    GcoDbiTab[] = {  // dbi-tab for GcoTxtTab
  DB_VCX_IND,    DB_VCY_IND,    DB_VCZ_IND,    DB_VCIX_IND,   DB_VCIY_IND,
  DB_VCIZ_IND,   DB_PLX_IND,    DB_PLY_IND,    DB_PLZ_IND,    DB_PLIX_IND,
  DB_PLIY_IND,   DB_PLIZ_IND};


char  *CopTxtTab[] = {  // controlOperators (EQ NE LT GT G_E L_E)  Typ_ope_eq ..
// 00             1              2              3              4   TypTxtTab220
  "EQ",          "NE",          "LT",          "GT",          "G_E",
  "L_E",         "AND",         "OR",
  ""};




char  MOpTxtStr[] = "+-*/^";  // math.operators



//----------------------------------------------------------------

char  *ObjCodTab[] = {
// 00
  "ON",          "OFF",         "IN",          "OUT",         "ARC",
  "HI",          "LO",          "MA",          "TAG",         "PERP",
// 10
  "CCW",         "CW",          "ELL",         "IMG",         "CCV2",
  "POL2",        "POL",         "PSP3",        "BSP",         "CCV",
// 20
  "RCIR",        "RSTRIP",      "SRU",         "HAT",         "SRV",
  "PRISM",       "SPH",         "CON",         "TOR",         "CYL",
// 30
  "CUT",         "REV",         "SWP",         "LE",          "RI",
  "DIM",         "DIMA",        "LDR",         "UNUSED",      "FSUB",
// 40
  "PRJ",         "TRA",         "ROT",         "MIR",         "TXA",
  "DIMD",        "DIMR",        "DIM3",        "REC",         "INT",
// 50
  "RSYS",        "REPL",        "RBSP",        "ARC1",         "CTRL",
  "LDRP",        "LDRC",        "LDRS",        "CTLG",         "ISO",
// 60
  "FW",          "CX",          "PTAB",        "MSH",          "CLOT",
  "TNG",         "IMP",         "BSP1",        "PARL",         "BLEND",
// 70
  "UNL",         "UNL1",        "UNL2",        "DISP_PT",      "DISP_PL",
// 80
  "ANG",         "DRX",         "DRY",         "DRZ",          ""};

// last word must be "" !

// die zugehoerige enum ist Typ_Cmd1 in ../ci/NC_apt.h
// unused: RI LE HI LO SKWC
// nur f WCUT: MA RADI RADC T_MA T_RISO T_RCON SKWC




//----------------------------------------------------------------
// text for 2D-buttons ..
                 // Typ_modCWCCW
  static char *GL2Dtxt1[]={" CW","REV"," CX","CTRL","PERP","PARL","UNL"};


                        // 1020   1     2     3     4     5     6      7
  static char *GL2Dtxt2[]={" V+"," V-","VC+","VC-","TR+","TR-","NXT", "PRV",
                        //  8     9         10
                           "?",  "PTonObj","PTonCP"};


  static char *GL2Dtxt3[]={"LIST"};



//----------------------------------------------------------------






//================================================================
  int APED_oid_dbo_all (char *oid, int typ, long dbi, long dli) {
//================================================================
/// \code
/// APED_oid_dbo_all          make name from typ and DB-index  (all types)
/// Input:
///   dli       -1 for unknown; used only for applicationObjects
///
/// see also APED_oid_dbo__ AP_typDB_typ AP_cre_defHdr
/// \endcode

  int   irc;

  // printf("APED_oid_dbo_all %d %ld %ld\n",typ,dbi,dli);



  if(typ == Typ_APPOBJ) {
    PLU_oid_appNam (oid,dli);

  } else if(typ == Typ_TmpPT) {
    strcpy(oid, "ConstrPlane");


  } else if(typ == TYP_FilNam) {
    strcpy(oid, "ext.Files");

  } else if(typ == Typ_SubModel) {
    strcpy(oid, "int.Files");

  } else if(typ == Typ_CtlgPart) {
    strcpy(oid, "ctlgFiles");


  } else if(TYP_IS_MOD(typ)) {
    strcpy(oid, AP_src_typMod (typ));


  } else {

    irc = APED_oid_dbo__ (oid, typ, dbi);
  }


    // printf("ex-APED_oid_dbo_all |%s| %d %ld %ld\n",oid, typ, dbi, dli);


  return irc;

}


//================================================================
  int APED_dbi_src_std_vc_pl (int *typ, long *dbi, int gti) {
//================================================================
/// \code
/// get dbi for std-vector or planes from oid-index in GcoTxtTab
/// see APED_oid_dbo__ APED_dbo_oid
/// 
/// Input:
///   gti    index GcoTxtTab
/// Output:
///   typ    Typ_VC | Typ_PLN
///   dbi
/// \endcode


  // printf("APED_dbi_src_std_vc_pl |%s|\n",GcoTxtTab[gti]);

  if(gti >= 6) *typ = Typ_PLN;
  else         *typ = Typ_VC;

  *dbi = GcoDbiTab[gti];

  return 0;

}


//====================================================================
  int APED_oid_dbo__ (char *buf, int typ, long ind) {
//====================================================================
/// \code
/// APED_oid_dbo__      make name from typ and DB-Index  (visible object types)
/// Input:
///   typ,ind    DB-obj
/// Output:
///   buf        name for DB-obj
/// see also APED_dbo_oid APED_oid_dbo_all AP_typDB_typ AP_cre_defHdr
/// \endcode

// FuncNam should be APED_oid_dbo_all

  char   *p1;

  // printf("APED_oid_dbo__ %d %ld\n",typ,ind);

  // if(!typ) AP_debug__ ("APED_oid_dbo__ E0");


  if(typ == Typ_VAR) {  // hat hoechste Select-Prioritaet in GL
    sprintf(buf, "V%ld", ind);


  } else if((typ == Typ_PT)    ||
            (typ == Typ_PT2)   ||
            (typ == Typ_TmpPT))      {
    sprintf(buf, "P%ld", ind);


  } else if((typ == Typ_LN)    ||
            (typ == Typ_LN2))        {
    sprintf(buf, "L%ld", ind);


  } else if((typ == Typ_CI)    ||
            (typ == Typ_CI2))        {
    sprintf(buf, "C%ld", ind);



  } else if(typ == Typ_VC) {
    if((ind >= 0) || (ind < DB_VCIZ_IND)) {
      sprintf(buf, "D%ld", ind);
    } else {
      if     (ind == DB_VCX_IND) sprintf(buf, "DX");
      else if(ind == DB_VCY_IND) sprintf(buf, "DY");
      else if(ind == DB_VCZ_IND) sprintf(buf, "DZ");
      else if(ind == DB_VCIX_IND) sprintf(buf, "DIX");
      else if(ind == DB_VCIY_IND) sprintf(buf, "DIY");
      else if(ind == DB_VCIZ_IND) sprintf(buf, "DIZ");
    }


  } else if((typ >= Typ_CV) && (typ < Typ_PLN)) {
/*
  } else if((typ >= Typ_CV)      ||
            (typ == Typ_CVLNA)   ||
            (typ == Typ_CVPOL)   ||
            (typ == Typ_CVPOL2)  ||
            (typ == Typ_CVBSP)   ||
            (typ == Typ_CVRBSP)  ||
            (typ == Typ_CVELL)   ||
            (typ == Typ_CVCLOT)  ||
            (typ == Typ_CVTRM))      {
*/
    sprintf(buf, "S%ld", ind);


  } else if(typ < Typ_SUR) {           // PLANE    R
//   } else if(typ == Typ_PLN) {
    if((ind >= 0) || (ind < DB_PLIZ_IND)) {
      sprintf(buf, "R%ld", ind);
    } else {
      if     (ind == DB_PLX_IND) sprintf(buf, "RX");
      else if(ind == DB_PLY_IND) sprintf(buf, "RY");
      else if(ind == DB_PLZ_IND) sprintf(buf, "RZ");
      else if(ind == DB_PLIX_IND) sprintf(buf, "RIX");
      else if(ind == DB_PLIY_IND) sprintf(buf, "RIY");
      else if(ind == DB_PLIZ_IND) sprintf(buf, "RIZ");
    }


  } else if(typ < Typ_SOL) {       // SURFACES    A
//   } else if((typ == Typ_SUR)      ||
//             (typ == Typ_SURSUP)   ||
//             (typ == Typ_SUTP)   ||
//             (typ == Typ_SURSWP)   ||
//             (typ == Typ_SURRU)    ||
//             (typ == Typ_SURRV)    ||
//             (typ == Typ_SURCIR)   ||
//             (typ == Typ_SURSTRIP) ||
//             (typ == Typ_SURBSP)   ||
//             (typ == Typ_SURRBSP)  ||
//             (typ == Typ_SURPTAB)  ||
//             (typ == Typ_SURPMSH))    {
    sprintf(buf, "A%ld", ind);


  } else if(typ < Typ_Note)   {  // SOLIDS   B
//   } else if((typ == Typ_SOL)   ||
//   } else if((typ == Typ_SOL)   ||
//             (typ == Typ_CON)   ||
//             (typ == Typ_TOR))        {
    sprintf(buf, "B%ld", ind);



  } else if(typ < Typ_Joint)   {    // NOTES N
//   } else if((typ >= Typ_Note) && (typ < Typ_SymB)) {
//             (typ == Typ_ATXT)  ||
//             (typ == Typ_GTXT)  ||
//             (typ == Typ_Dimen) ||
//             (typ == Typ_Dim3)  ||
//             (typ == Typ_SymV)  ||                           // 2011-08-07
//             (typ == Typ_Tag))         {
    sprintf(buf, "N%ld", ind);


  } else if(typ < Typ_SymB)   {    // JOINT   J
    sprintf(buf, "J%ld", ind);


  } else if((typ >= Typ_SubModel) && (typ <= Typ_CtlgPart)) {
//   } else if((typ == Typ_Model)    ||
//             (typ == Typ_SubModel) ||
//             (typ == Typ_Mock))        {
    sprintf(buf, "M%ld", ind);


  } else if(typ == Typ_Val) {
    //sprintf (buf,"%f,%f",pt1.x,pt1.y);
    //strcpy(buf, ",");
    buf[0] = '\0';
    return -1;
    // UTX_add_fl_u2 (buf, pt1.x,pt1.y);


  } else if(typ == Typ_Tra) {  // Transformation
    sprintf(buf, "T%ld", ind);


  } else if(typ == Typ_Activ) {  // Activity
    sprintf(buf, "I%ld", ind);


  } else if(typ == Typ_Group) {  // undefined  // 2011-06-01 was Typ_goGeom
    sprintf(buf, "U%ld", ind);


  } else if(typ == Typ_SymB) {  // temporary-SymBmp
    sprintf(buf, "Sym%ld", ind);


  } else if(typ == Typ_dynSym) {  // Temporary-Dynamic-object
    sprintf(buf, "TDS%ld", ind);


  } else if(typ == Typ_APPOBJ) {  // ApplicationObj
    sprintf(buf, "App%ld", ind);


  } else {

    strcpy(buf, "-");
    p1 = AP_src_typ__ (typ);
    // TX_Error("APED_oid_dbo__: Objekttyp %d gefunden",typ);
    TX_Print("Err APED_oid_dbo__ obj %s typ=%d dbi=%d",p1,typ,ind);
    return -1;

  }

    // printf("ex-APED_oid_dbo__ |%s| %d %ld\n",buf,typ,ind);

  return 1;

}


/*
//================================================================
  int APED_oid_dbo_sm (char *oid, int sSiz, int dbTyp, long dbi) {
//================================================================
// APED_oid_dbo_sm     object-id from typ and DB-Index and subModelname

  int  i1, ii;

  APED_oid_dbo__ (oid, dbTyp, dbi);

  if(MDL_IS_SUB) {
    strcat (oid, " in ");
    ii = strlen (oid);
    sSiz -= ii;
    Mod_mNam_mdb (&oid[ii], sSiz, &i1, AP_modact_ibm);
  }

    // printf("ex APED_oid_dbo_sm |%s|\n",oid);


  return 0;

}
*/

 
//====================================================================
  int APED_oid_vc (char *cbuf, Vector *vc1) {
//====================================================================
/// \code
/// APED_oid_vc          get oid for Vector; (DX or DIX or D#)
/// check if Vector is a Defaultvektor (DX or DIX or ...)
/// Input:
///   vc1      struct Vector*
/// Output:
///   text     eg "DZ" if RetCod < 0
///   RC =  0: no, allgemeiner vektor;
///   RC != 0: yes, -1=DX, -4=DIX, (DB-index of standard-vector)
/// see AP_vec_txt APED_oid_dbo__
/// \endcode

  int       i1, ii;
  double    dx, dy, dz;
  char      *vcTab[]={NULL,"DX","DY","DZ","DIX","DIY","DIZ"};

  
  // printf("APED_oid_vc %f %f %f\n",vc1->dx,vc1->dy,vc1->dz);

  i1 = UT3D_vc_ck_std (vc1);  // check for defaultVector (DX DY DZ DIX DIY DIZ)
  if(!i1) { cbuf[0] = '\0'; return 0; }

  ii = i1; if(ii < 0) ii = 3 - ii;  // gives 1-6
  strcpy(cbuf, vcTab[ii]);

  return -ii;  // DBindex: -1 - -6

}


//================================================================
  int APED_dbo_oid (int *defTyp, long *defInd, char* txtIn) {
//================================================================
/// \code
/// APED_dbo_oid                get type and dbi from object-ID;
/// eg returns defTyp=Typ_PT and defInd=20 for "P20"
/// Returns also this constantObjects: DX DY DZ DIX DIY DIZ RX RY RZ
/// see also AP_cut_defHdr
/// 
/// Input:
///   txtIn
///
/// Output:
///   defTyp: see wincut_base.h
///           values return 0 !
///   defInd
/// 
/// Retcodes:
///   0 = OK
///  -1 = only defTyp (eg P(..) or D(..))
///  -2 = Error
///
/// TODO: combine with DBO_dbo_src__
/// \endcode


  int   rc, i1, ilen;
  char  *txt, typ, s1[4];   //txt[80];
  // char  *buf;


  // printf("APED_dbo_oid |%s|\n",txtIn);

  *defTyp = Typ_Error;
  *defInd = 0L;


  if(strlen(txtIn) < 2) {return -2;}

  // skip lead. blanks
  // buf = UTX_pos_1n (txtIn);
  txt = txtIn;
  while (*txt  == ' ')  { ++txt; }

  // to Upper
  // UTX_cp_word_2_upper (txt, buf);
  if(islower(txt[0])) {
    typ = toupper(txt[0]);
  } else {
    typ = txt[0];
  }

  // find typ
  // *defTyp = AP_typ_typChar (*txt);
  *defTyp = AP_typ_typChar (typ);
  if(*defTyp == Typ_Error) {
    *defInd = 0;
    rc = -2;
    goto Fertig;
  }


  // check for P(..) oder D(..)
  if(txt[1] == '(') {
    rc = -1;
    goto Fertig;
  }


  // numerisch ?
  // printf(" txt[1]=|%c|\n",txt[1]);
  if((isdigit(txt[1])) || (txt[1] == '-')) {
    *defInd = atoi (&txt[1]);
    rc = 0;
    goto Fertig;
  }


  rc = -2;

  if(*defTyp == Typ_VC) {  // test DX DY DZ DIX DIY DIZ
// TODO: use APED_dbi_src_std_vc_pl
    ++txt;
    i1 = strlen(txt);
    if(i1 > 2) goto Fertig;
    UTX_cp_nchr_2_upper (s1, txt, i1);
    s1[i1] ='\0';

    if (!strcmp (s1, "X")) {
      *defInd = DB_VCX_IND;

    } else if (!strcmp (s1, "Y")) {
      *defInd = DB_VCY_IND;

    } else if (!strcmp (s1, "Z")) {
      *defInd = DB_VCZ_IND;

    } else if (!strcmp (s1, "IX")) {
      *defInd = DB_VCIX_IND;

    } else if (!strcmp (s1, "IY")) {
      *defInd = DB_VCIY_IND;

    } else if (!strcmp (s1, "IZ")) {
      *defInd = DB_VCIZ_IND;

    } else {
      goto Fertig;
    }
    rc = 0;

  } else if(*defTyp == Typ_PLN) {  // test RX RY RZ
// TODO: use APED_dbi_src_std_vc_pl
    ++txt;
    i1 = strlen(txt);
    if(i1 > 1) goto Fertig;
    UTX_cp_nchr_2_upper (s1, txt, i1);
    s1[i1] ='\0';

    if (!strcmp (s1, "X")) {
      *defInd = DB_PLX_IND;

    } else if (!strcmp (s1, "Y")) {
      *defInd = DB_PLY_IND;

    } else if (!strcmp (s1, "Z")) {
      *defInd = DB_PLZ_IND;

    } else {
      goto Fertig;
    }
    rc = 0;
  }



  Fertig:
    // printf("ex APED_dbo_oid: rc=%d,Typ=%d, Ind=%ld\n",rc,*defTyp,*defInd);
    // printf("                   txtIn=|%s|\n",txtIn);
    // printf("ex APED_dbo_oid |%s| %p\n",txt,defInd);

  return rc;

}


//================================================================
  char* AP_src_typ__ (int typ) {
//================================================================
/// \code
/// get objTypTxt from typ
/// returns eq "PT" for Typ_PT
/// see also AP_src_typMod DB_Typ_Char
/// \endcode

// was DB_Char_Typ

  char *p1;
  
  // printf("AP_src_typ__ %d\n",typ);

  if     (typ < 0)        goto L_err1;
  else if(typ < 20)       p1 = TypTxtTab0[typ];
  else if(typ < 40)       p1 = TypTxtTab20[typ - 20];
  else if(typ < 50)       p1 = TypTxtTab40[typ - 40];
  else if(typ < 80)       p1 = TypTxtTab50[typ - 50];
  else if(typ < 90)       p1 = TypTxtTab80[typ - 80];
  else if(typ < 100)      p1 = TypTxtTab90[typ - 90];
  else if(typ < 110)      p1 = TypTxtTab100[typ - 100];
  else if(typ < 120)      p1 = TypTxtTab110[typ - 110];
  else if(typ < 130)      p1 = TypTxtTab120[typ - 120];
  else if(typ < 150)      p1 = TypTxtTab130[typ - 130];
  else if(typ < 170)      p1 = TypTxtTab150[typ - 150];
  else if(typ < 190)      p1 = TypTxtTab170[typ - 170];
  else if(typ < 200)      p1 = TypTxtTab190[typ - 190];
  else if(typ < 210)      p1 = TypTxtTab200[typ - 200];
  else if(typ < 220)      p1 = TypTxtTab210[typ - 210];
  else if(typ < 230)      p1 = TypTxtTab220[typ - 220];
  else if(typ < 260)      p1 = TypTxtTab230[typ - 230];
  else if(typ < 270)      p1 = TypTxtTab260[typ - 260];
  else if(typ < 1000)     p1 = TypTxtTab270[typ - 270];
  else if(typ < 1020)     p1 = TypTxtTab1000[typ - 1000];
  else if(typ < 1100)     p1 = TypTxtTab1020[typ - 1020];
  else                    goto L_err1;

    // printf("ex AP_src_typ__ %d |%s|\n",typ,p1);

  L_exit:
    return p1;


  L_err1:
    TX_Print("AP_src_typ__ E001 %d",typ);
    p1 = TypTxtTab0[0];
    goto L_exit;

}


//================================================================
  char* AP_src_typMod (int typ) {
//================================================================
/// \code
/// get modifier-text from (modifier)typ
/// typ must be modifier (test with TYP_IS_MOD)
/// \endcode

  char *p1;


  // printf("AP_src_typMod %d\n",typ);


  if(typ >= Typ_FncVAR1) {
    if(typ < Typ_EOT) return p1 = GL2Dtxt2[typ - Typ_FncVAR1];
    return NULL;
  }

  // get text for types Typ_modREV - incl. Typ_modPARL
  // if(typ >= Typ_modREV) {
  if(typ >= Typ_modCWCCW) {
    if(typ <= Typ_modUnlim) return p1 = GL2Dtxt1[typ - Typ_modCWCCW];
    return NULL;
  }

  if((typ == TYP_FilNam)    ||
     (typ == Typ_SubModel)  ||
     (typ == Typ_CtlgPart))    return GL2Dtxt3[0];


  return NULL;
}


/* replaced by AP_src_typ__
//=============================================================
  char* DB_Char_Typ (int typ) {
//=============================================================
/// \code
/// make text from (int)typ; Typ_PT --> "PT" ..
/// max 8 chars
/// \endcode


  switch (typ) {

    case Typ_VAR:
      return "Var";

    case Typ_VC:
      return "Vec";

    case Typ_PT:
      return "PT";

    case Typ_PT2:
      return "PT2";

    case Typ_TmpPT:
      return "TmpPT";

    case Typ_LN:
      return "LN";

    case Typ_LN2:
      return "LN2";

    case Typ_CI:
      return "AC";

    case Typ_CI2:
      return "AC2";

    case Typ_CV:
      return "CV";

    case Typ_CVELL:
      return "CvEll";

    case Typ_CVCLOT:
      return "Clot";

    case Typ_CVBSP:
      return "CvBsp";

    case Typ_CVRBSP:
      return "CvRBsp";

    case Typ_CVLNA:
      return "CvLnTab";

    case Typ_CVPOL:
      return "CvPlg";

    case Typ_CVPSP3:
      return "CvPsp3";

    case Typ_CVTRM:
      return "CvCcv";

    case Typ_PLN:
      return "PLN";

    case Typ_SUR:
      return "SUR";

    case Typ_SURPLN:
      return "SurPLN";

    case Typ_SURRU:
      return "SurRU";

    case Typ_SURRV:
      return "SurRV";

    case Typ_SURBSP:
      return "SurBsp";

    case Typ_SURRBSP:
      return "SurRBsp";

    case Typ_SURCIR:
      return "SurCir";

    case Typ_SURSTRIP:
      return "SurStr";

    case Typ_SURHAT:
      return "SurHat";

    case Typ_SURPTAB:
      return "SurPtab";

    case Typ_SPH:
      return "Sphere";

    case Typ_CON:
      return "Conus";

    case Typ_TOR:
      return "Torus";

    case Typ_SOL:
      return "SOL";

    case Typ_ATXT:
      return "TxtA";

    case Typ_GTXT:
      return "TxtG";

    case Typ_APPOBJ:
      return "Appl.Obj.";

    case Typ_Typ:
      return "Typ";

    case Typ_Index:
      return "Ind";

    case Typ_Txt:
      return "Text";

    case Typ_Dimen:
      return "Dimen";

    case Typ_Ptr:
      return "Data";

    case Typ_Int4:
      return "Int4";

    case Typ_Memspc:
      return "Memspc";

    case Typ_Tra:
      return "Tra";

    case Typ_Activ:
      return "Act";

    case Typ_Joint:
      return "Jnt";

    case Typ_Color:
      return "Col";

    case Typ_ObjGX:
      return "OGX";

    case Typ_GL_Sur:
      return "SurTess";

    case Typ_GL_PP:
      return "CrvTess";

    case Typ_Size:
      return "SIZE";

    case Typ_SymB:
      return "SymBmp";

    case Typ_SymV:
      return "SymVec";

    case Typ_Group:
      return "Grp";

    case Typ_Ditto:
      return "Dit";

    case Typ_Model:
      return "Model";

    case Typ_Mock:
      return "Mockup";

    case Typ_QFac:
      return "QFac";

    case Typ_Done:
      return "Done";

  }

  // TX_Error("DB_Char_Typ E001 %d",typ);

  return "";

}
*/

//================================================================
  int DB_Typ_Char (char* auxBuf) {
//================================================================
/// \code
/// return type from obj-definition ("PT" or "LN") or obj-ID ("eg "P20")
/// Text > int-3D-Typ
/// give typ(int) from text "PT" --> Typ_PT
/// see also AP_src_typ__ (objTypTxt from typ)
/// \endcode


  //TX_Print("DB_Typ_Char /%s/",auxBuf);


  // if(!strcmp (auxBuf, "PT")) {
  if(auxBuf[0] == 'P') {
    return Typ_PT;

  // } else if(!strcmp (auxBuf, "LN")) {
  } else if(auxBuf[0] == 'L') {
    return Typ_LN;


  // } else if(!strcmp (auxBuf, "CI")) {
  } else if(auxBuf[0] == 'C') {
    return Typ_CI;


  } else {
    return Typ_Error;
  }

}


//================================================================
  int DB_2DTyp_3DTyp (int typIn) {
//================================================================
/// \code
/// DB_2DTyp_3DTyp     give 2D-Typ of 3D-Typ
///   RetCod: 0=Error else 2d-type
/// \endcode

  switch (typIn) {

    case Typ_PT:
    case Typ_PT2:
      return Typ_PT2;

    case Typ_LN:
    case Typ_LN2:
      return Typ_LN2;


    case Typ_CI:
    case Typ_CI2:
      return Typ_CI2;


    case Typ_CV:
      return Typ_CV2;



  }

  TX_Error("DB_2DTyp_3DTyp %d",typIn);
  return Typ_Error;

}


//================================================================
  int AP_typ_ck_sym (int typ) {
//================================================================
// AP_typ_ck_sym      check if typ is symbolic-typ / not basic typ (VC,Tra,VAR,Act
// returns  1 = yes,its a symbolic-typ (VC,Tra,VAR,Act),
//          0 = not

  switch(typ) {

    case Typ_VAR:
    case Typ_VC:
    case Typ_Joint:
    case Typ_Activ:
    case Typ_Tra:
      break;

    default:
      return 0;
  }

  return 1;

}


//====================================================================
  int AP_typ_typChar (char typChar) {
//====================================================================
/// \code
/// AP_typ_typChar     make typ from typChar  ("P" -> Typ_PT)
/// Error: returns Typ_Error
/// AP_typChar_typ      make typChar from typ  (Typ_PT -> 'P')
/// \endcode

  int  ityp;


  // printf("AP_typ_typChar |%c|\n",typChar);


  switch(typChar) {

    case 'V':     ityp = Typ_VAR;       break;
    case 'D':     ityp = Typ_VC;        break;
    case 'P':     ityp = Typ_PT;        break;
    case 'L':     ityp = Typ_LN;        break;
    case 'C':     ityp = Typ_CI;        break;
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
    case 'U':     ityp = Typ_Group;     break; // 2011-06-01 was Typ_goGeom
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


  } else if((typ == Typ_CI)  ||
            (typ == Typ_CI2))        {
    return 'C';


  } else if(typ == Typ_PLN) {
    return 'R';


  } else if(typ == Typ_VC) {
    return 'D';


  } else if((typ == Typ_CV)      ||
            (typ == Typ_CVLNA)   ||
            (typ == Typ_CVPOL)   ||
            (typ == Typ_CVBSP)   ||
            (typ == Typ_CVRBSP)  ||
            (typ == Typ_CVELL)   ||
            (typ == Typ_CVCLOT)  ||
            (typ == Typ_CVTRM))      {
    return 'S';

  } else if((typ == Typ_SUR)      ||
            (typ == Typ_SURRU)    ||
            (typ == Typ_SURRV)    ||
            (typ == Typ_SURCIR)   ||
            (typ == Typ_SURSTRIP) ||
            (typ == Typ_SURBSP)   ||
            (typ == Typ_SURSWP)   ||
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


  } else if(typ == Typ_Group) {  // undefined  // 2011-06-01 was Typ_goGeom
    return 'U';


  }

  // printf("APED_oid_dbo__ |%s| %d\n",buf,strlen(buf)); // nur Testausg.

  return '-';

}

//====================================================================
  int AP_typDB_typ (int typ) {
//====================================================================
/// \code
/// give basictyp from typ;  eg Typ_CV from Typ_CVPOL
/// makes Typ_SURRU -> Typ_SUR   or Typ_CVBSP -> Typ_CV.
/// 
/// siehe APED_oid_dbo__ AP_cmp_typ
/// \endcode


  if(typ >= Typ_CV) {
    if(typ < Typ_PLN)     return Typ_CV;       // S
    if(typ < Typ_SUR)     return Typ_PLN;      // R
    if(typ < Typ_SOL)     return Typ_SUR;      // A
    if(typ < Typ_Note)    return Typ_SOL;      // B
    if(typ < Typ_Texture) return Typ_Note;     // N    2016-10-03

    if(typ == Typ_Mock)   return Typ_Model;    // M

  }

/*
  if((typ == Typ_CVLNA)   ||
     (typ == Typ_CVPOL)   ||
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


  if((typ == Typ_Tag)    ||
     (typ == Typ_Dimen)  ||
     (typ == Typ_Dim3)   ||
     (typ == Typ_ATXT)   ||
     (typ == Typ_GTXT))
    return Typ_Note;                      // N
*/


  if(typ == Typ_CI2) return Typ_CI;       // C


  if(typ == Typ_LN2) return Typ_LN;


  if(typ == Typ_PT2) return Typ_PT;



  return typ;

}


//================================================================
  int AP_typ_FncNam (char *fncNam, int sLen) {
//================================================================
// get type of function

// Input
//   fncNam     must be uppercase
// RetCod:
//   >=0         type of function eg Typ_Angle or Typ_FcmSQRT
//   -1   unknown function
//   -2   functionName too long

// see also APT_decode_func ObjCodTab,


  int   i1, iTyp;
  char  s1[32];


  // printf("AP_typ_FncNam %d |%s|\n",sLen,fncNam);

  // get strLen
  while (fncNam[sLen - 1] == ' ') --sLen;

  if(sLen > 30) { iTyp = -2; goto L_exit;}

  // copy fncNam -> s1, make uppercase     see also UTX_cp_word_2_upper
  // strncpy (s1, fncNam, sLen);
  for(i1=0; i1<sLen; ++i1) s1[i1] = toupper (fncNam[i1]);
  s1[sLen] = '\0';
    // printf(" s1 %d |%s|\n",sLen,s1);


  //----------------------------------------------------------------
  if(sLen > 1) goto L_fnc1;

  // test for obj-typ
  iTyp = AP_typ_typChar (*s1);
  if(iTyp) goto L_exit;

  if(*s1 == 'X') { iTyp = Typ_XVal; goto L_exit;}
  if(*s1 == 'Y') { iTyp = Typ_YVal; goto L_exit;}
  if(*s1 == 'Z') { iTyp = Typ_ZVal; goto L_exit;}
  goto L_exit;


  //----------------------------------------------------------------
  L_fnc1:
  // VAL ANG RAD MOD NEW ..
  iTyp = UTX_cmp_word_wordtab (Fc1TxtTab, s1);
    // printf(" ex-word_wordtab-Fc1TxtTab %d\n",iTyp);

  if(iTyp >= 0) {
    iTyp = Fc1TypTab[iTyp];
    goto L_exit;
  }


  // SQRT SIN COS TAN ASIN
  // ACOS ATAN ABS FIX RND
  iTyp = UTX_cmp_word_wordtab (FcmTxtTab, s1);
  if(iTyp >= 0) iTyp += Typ_FcmSQRT;



  //----------------------------------------------------------------
  L_exit:

  // printf("ex AP_typ_FncNam %d %d |%s|\n",iTyp,sLen,fncNam);

  return iTyp;

}


//================================================================
  int AP_typ_srcExpr (char *expr, char **pNxt) {
//================================================================
// get type of expression

// Input:
// Output:
//   retCod    Typ_Val|Typ_String|Typ_ObjDB|Typ_Txt|Typ_FncNam

// for Typ_Val:      use strtod,strtof, atoi,atol,atof,atoff.
// for Typ_ObjDB:    use AP_typ_typChar, APED_dbo_oid
// for Typ_Txt:      decode expr -> atomicObj; TODO !
// for Typ_FncNam:   use AP_typ_FncNam


// definitionCodes:
//   Typ_Val        numericExpr    (1.5)
//   Typ_String     string         ("string")
//   Typ_ObjDB      DB-Object      (V1, P2, S3, A4, B5 ..)

//   Typ_Txt
//     objectCodes    (CUT, REV, ...) ObjCodTab Typ_Cmd1
//     constantValue  (PI, RAD_1 RAD_90 RAD_180 RAD_360 SR_2 SR_3)
//     constantObject (DX DY DZ DIX DIY DIZ RX RY RZ)

//   Typ_FncNam
//     implicitObject (Val(), P()-B(), X|Y|Z() ANG() RAD() NEW())
//     mathFunction   (SQRT SIN COS TAN ASIN ACOS ATAN ABS FIX RND)

// controlCodes:
//   programCode    (IF JUMP DLG DEBUG INTERN EXIT ..)  SUB ?
//   operator       (EQ NE LT GT G_E L_E)  Typ_ope_eq

// definitionLines:  DB-Object = definitionCode
// controlLines:     controlCodes (programCodes, operators)
// definitionCodes:  all but controlCodes
// controlCodes:     programCodes, operators

// see also
// APED_dec_txt
// APED_dbo_oid      DB-Objects constantObjects
// APT_decode_ausdr1   operators constantObjects constantValues
// APT_decode_func     SQRT SIN COS TAN ASIN ACOS ATAN ABS FIX RND
// UTX_ck_num_f        test ob word numer ist

  int     ityp = 0;
  char    *cpos, *fncPos;
 
  // printf("AP_typ_srcExpr |%s|\n",expr);


  *pNxt = expr;
  cpos = expr;
  UTX_pos_skipLeadBlk(cpos);         // remove leading blanks


  // check for numeric: digit, '+' or '-' or '.'
  if(isdigit(*cpos)) goto L_num__;
  if((*cpos != '-')&&(*cpos != '+')&&(*cpos != '.')) goto L_strg__;


  //----------------------------------------------------------------
  // expr is a number.
  L_num__:
    ityp = Typ_Val;
    goto L_exit;

  

  //----------------------------------------------------------------
  // String, not number.
  L_strg__:

    // check for '"' (string)
    if(*cpos == '"') {
      ityp = Typ_String;
      goto L_exit;
    }

    // check for '(' (function)
    *pNxt = strchr (cpos, '(');
    if(*pNxt) goto L_func__;


    // cpos is a code, not a function.
    // if second char is a digit = DB-Object
    if(isdigit(cpos[1])) {
      ityp = Typ_ObjDB;
      goto L_exit;
    }

    // cpos is a single word;  objectCode|constantValue|constantObject

    // test for constantObjects (DX DY DZ DIX DIY DIZ RX RY RZ)
    // (APT_decode_ausdr1)
   
    // test for constantValue (PI, RAD_1 RAD_90 RAD_180 RAD_360 SR_2 SR_3)
    // (APT_decode_ausdr1)

    // test for objectCode (CUT, REV, ...) ObjCodTab Typ_Cmd1 APT_decode_ausdr1

    ityp = Typ_Txt;
    goto L_exit;



  //----------------------------------------------------------------
  L_func__:
    // cpos is a functionName; the pos of '(' is fncPos.
      // printf(" L_func__:\n");

    // Function: implicitObject|mathFunction

    // test for implicitObject ((Val(), P()-B(), X|Y|Z() ANG() RAD() NEW())
    // (APT_decode_func)

    // test for mathFunction (SQRT SIN COS TAN ASIN ACOS ATAN ABS FIX RND)
    // (APT_decode_func)

    ityp = Typ_FncNam;
    goto L_exit;



  //----------------------------------------------------------------
  L_exit:
    // printf("ex AP_typ_srcExpr %d |%s|\n",ityp,*pNxt);

  return ityp;

}


//================================================================
  char* AP_typTxt_typ (int typ) {
//================================================================
// AP_typTxt_typ            get typTxt from typ (eg "Typ_PT" from Typ_PT)

  int    i1, i2;

  typedef  struct {int typ; char *txt;} IE_info_rec;

  static   IE_info_rec IE_info_tab[] = {
  Typ_Val,      "Typ_Val",
  Typ_PT,       "Typ_PT",
  Typ_LN,       "Typ_LN",
  Typ_CI,       "Typ_CI",
  Typ_VC,       "Typ_VC",
  Typ_CV,       "Typ_CV",
  Typ_CVTRM,    "Typ_CVTRM",
  Typ_CVPOL,    "Typ_CVPOL",
  Typ_CVBSP,    "Typ_CVBSP",
  Typ_CVRBSP,   "Typ_CVRBSP",
  Typ_CVELL,    "Typ_CVELL,",
  Typ_XVal,     "Typ_XVal",
  Typ_YVal,     "Typ_YVal",
  Typ_ZVal,     "Typ_ZVal",
  Typ_Angle,    "Typ_Angle",
  Typ_PLN,      "Typ_PLN",
  Typ_SOL,      "Typ_SOL",
  Typ_Tra,      "Typ_Tra",
  Typ_modAux,   "Typ_mod1",                    // Bild-Tasten/ (Version ändern)
  Typ_Txt,      "Typ_Txt",

  Typ_TmpPT,    "Typ_Txt",
  Typ_Vertex,   "Typ_Vertex",

  Typ_SubModel, "Typ_SubModel",
  Typ_Model,    "Typ_Model",
  Typ_CtlgPart, "Typ_CtlgPart",

  // Typ_Str_Dir1, "Typ_Str_Dir1",              // nur CAD
  // Typ_Val_symTyp, "Typ_Val_symTyp",          // nur CAD
  Typ_Joint,    "Typ_Joint",
  // Typ_apDat,    "Typ_apDat",
  Typ_Group,    "Typ_Group",

  Typ_PTS,      "Typ_PTS",
  Typ_PTI,      "Typ_PTI",
  Typ_SEG,      "Typ_SEG",

  Typ_String,   "Typ_String",
  TYP_FilNam,   "TYP_FilNam",

  Typ_modRepl,  "Typ_modRepl",
  Typ_mod1,     "Typ_mod1",
  Typ_mod2,     "Typ_mod2",
  Typ_modCWCCW, "Typ_modCWCCW",
  Typ_modREV,   "Typ_modREV",
  Typ_modCX,    "Typ_modCX",
  Typ_modCTRL,  "Typ_modCTRL",
  Typ_modPERP,  "Typ_modPERP",
  Typ_modPARL,  "Typ_modPARL",
  Typ_modUnlim, "Typ_modUnlim",

  Typ_goPrim,   "Typ_goPrim",
  Typ_go_LCS,   "Typ_lFig",
  Typ_goGeom,   "Typ_goGeom",
  Typ_goGeo1,   "Typ_goGeo1",
  Typ_goGeo2,   "Typ_goGeo2",
  Typ_go_PD,    "Typ_go_PD",
  // Typ_goGeo4,   "Typ_goGeo4",
  Typ_goGeo5,   "Typ_goGeo5",
  Typ_goGeo7,   "Typ_goGeo7",
  Typ_goGeo8,   "Typ_goGeo8",
  Typ_goAxis,   "Typ_goAxis",
  Typ_goGeoSUSU,"Typ_goGeoSUSU",
  Typ_go_RA,    "Typ_go_RA",
  Typ_go_lf1,   "Typ_go_lf1",
  Typ_go_lf2,   "Typ_go_lf2",

  -1,           ""};               // end



  for(i1=0; i1<1000; ++i1) {
    i2 = IE_info_tab[i1].typ;
    if(i2 < 0) goto L_err;  // -1 = end of tab
    if(i2 == typ) goto L_exit;
  }
  goto L_err;


  L_exit:
      // printf("ex-AP_typTxt_typ |%s|\n",IE_info_tab[i1].txt);
    return IE_info_tab[i1].txt;


  L_err:
    TX_Print("******* AP_typTxt_typ typ %d undefined",typ);
    return NULL;

}


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


  } else if(typ1 == Typ_CI) {
    if(typ2 == Typ_CI) return 0;


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


//================================================================
  int UTO_ck_data (int form) {
//================================================================
// UTO_ck_data           check if obj has extern data (or pointer is data)
// RetCod:    0=no-extern-data;  else yes, pointer is valid, not data.
// see also UTO_ck_ptr

  if((form == Typ_Typ)                           ||
     (form == Typ_Index)                         ||
     ((form >= Typ_Ptr) && (form < Typ_Float4))  ||
     (form >= Typ_ope__))   return 0;   // 

  return 1;

}


//================================================================
  int UTO_ck_ptr (int typ) {
//================================================================
// UTO_ck_ptr              check if struct has pointer (is relocatable)
// check if struct is relocatable without modifications
// RetCod: 0=OK; relocatable without modifications (no pointers in struct).

  switch (typ) {
    // structs without pointers
    case Typ_VC:
    case Typ_PT:
    case Typ_LN:
    case Typ_CI:
    case Typ_CVELL:  // CurvElli
    case Typ_CVCLOT: // CurvClot:
    case Typ_CVTRM:  // CurvCCV:
    case Typ_PLN:
    case Typ_SURRV:  // SurRev
    case Typ_SUR:    // SurStd
    case Typ_Color:  // ColRGB:
    case Typ_TEXR:   // TexRef:
    case Typ_Ditto:  // Ditto:
    case Typ_SPH:    // Sphere:
    case Typ_CON:    // Conus:
    case Typ_TOR:    // Torus:
    case Typ_SymRef: // SymRef:
    case Typ_Model:  // ModelRef
      return 0;

    default:
      // struct has pointers, eg. CurvPoly CurvBSpl CurvRBSpl CurvBez2 ..
      break;
  }

  return -1;

}


//================================================================
  int UTO_ck_symTyp (int typ) {
//================================================================
// UTO_ck_symTyp               check if typ is symbolic (no grafic repres.)
// retCod     1 = yes (typ is VAR|VC|Tra|Actic|Joint)
//            0 = normal visible obj (PT|Curv|Surf ..)


  switch (typ) {
    case Typ_VAR:
    case Typ_VC:
    case Typ_Tra:
    case Typ_Activ:
    case Typ_Joint:
      return 1;

    default:
      break;
  }

  return 0;

}


//================================================================
  int UTO_ck_dbsTyp (int typ) {
//================================================================
/// \code
/// UTO_ck_dbsTyp           check object-typ (struct of object)
///   typ              must be a basictype; see AP_typDB_typ
/// Retcode:
///   0   dedicated struct      -  VPDLCRMI
///       V=Value,P=Point,D=Vector,L=Line,C=Circ,R=Plane,M=Model,I=Activity
///   1   complex-object (ObjGX) - SABNT
///       S=Curve,A=Surf,B=Sol,N=Note,T=Trafo
/// \endcode

// 0=VPDLCRMI
// 1=SABNT

  if(typ < Typ_CV) return 0;     // 0=V,D,P,L,C

  if(typ < Typ_PLN) return 1;    // 1=S

  if(typ < Typ_SUR) return 0;    // 0=R

  if(typ < Typ_SymB) return 1;   // 1=N,A,B

  if(typ < Typ_Txt)  return 0;   // 0=M(123-125),I(161)

  return 1;                      // 1=T(210-219)

}


//================================================================
  int UTO_ck_typTyp (int typ) {
//================================================================
/// \code
/// UTO_ck_typTyp     check if typ=curve (Typ_lFig) or surface (Typ_SUR)
/// Input
///   typ    must be basic-type (AP_typDB_typ)
/// Retcod   Typ_go_LCS  from LN|AC|CV     (L/C/S)
///          Typ_SUR     from PLN|SUR|SOL  (R/A/B)
///          Typ_PT      from PT           (P)
///          <typ>       unmodified, from N|M   (Typ_Note|Typ_Model ..)
/// \endcode


  switch(typ) {

    case Typ_LN:
    case Typ_CI:
    case Typ_CV:
      return Typ_go_LCS;


    case Typ_PLN:
    case Typ_SUR:
    case Typ_SOL:
      return Typ_SUR;

    case Typ_PT:
      return Typ_PT;

    default:                  // N|M   Typ_Note|Typ_Model ..
      return typ;

  }

}


//================================================================
  int UTO_ck_curvLimTyp (int typ) {
//================================================================
/// \code
/// UTO_ck_curvLimTyp              check if CurveLimits are Points or parameters
/// RetCode:
///   0   die CurveLimits basieren auf Points  (LN, AC, CVELL usw)
///   1   die CurveLimits basieren auf Parameters (CVPOL, CVBSP)
/// \endcode


  switch(typ) {
    case Typ_CVBSP:
    case Typ_CVPOL:
    case Typ_CVLNA:
      return 1;
  }

  // alle folgenden liefern Points
  return 0;

}


//================================================================
  int UTO_ck_curvForm (int typ) {
//================================================================
/// \code
/// UTO_ck_curvForm           check if objTyp is ObjGX or other typ
/// Output:
///   RetCode:   Typ_ObjGX (for Typ_CVPOL,Typ_CVBSP,Typ_CVELL ..)
///              else typ
/// \endcode


  switch(typ) {
    case Typ_CVLNA:
    case Typ_CVPOL:
    case Typ_CVBSP:
    case Typ_CVRBSP:
    case Typ_CVELL:
    case Typ_CVCLOT:
    case Typ_CVTRM:

      return Typ_ObjGX;


    default:
      // obj without ObjGX:
      // Typ_PT
      // Typ_LN
      // Typ_CI
      // Typ_CI
      return typ;

  }

}


//================================================================
  int* AP_typLst_grpTyp (int rTyp) {
//================================================================
// AP_typLst_grpTyp     get a list of basicTypes for a selectionGroupType
// selection-groupTypes: ../xa/xa_sele.h see INF_groupTypes

  // Typ_goPrim      P|L|C|S(Ell,Bsp,Plg,CCV) NOT D|R|A|B
  static int lPr[] = {Typ_PT, Typ_LN, Typ_CI, Typ_CV, 0};

  // Typ_goGeo1      Typ_go_LCS|Pln|Sur|Sol   all curves,  NOT P|D;
  static int lG1[] = {Typ_LN, Typ_CI, Typ_CV, Typ_PLN, Typ_SUR, Typ_SOL, 0};

  // Typ_go_PD       PT|VC
  static int lPD[] = {Typ_PT, Typ_CV, 0};

  // Typ_goGeo5      C|Ell|Plg|Bsp|CCV         NOT D,P,L,A,B
  static int lG5[] = {Typ_CI, Typ_CV, 0};

  // Typ_goGeoSUSU   Supporting Surface PLN,SUR,CON/TOR/SRU/SRV/SBS
  static int lGS[] = {Typ_PLN, Typ_SUR, Typ_SURCON, Typ_SURTOR, Typ_SURRU,
                      Typ_SURRV, Typ_SURSWP, Typ_SURBSP, Typ_SURRBSP, 0};

  // Typ_goGeo6      P|L|C                     NOT D|S|A|B
  static int lG6[] = {Typ_PT, Typ_LN, Typ_CI, 0};

  // Typ_goGeo7      Val|V|P|D                 NOT R|A|B
  static int lG7[] = {Typ_PT, Typ_VC, Typ_Val, 0};

  // Typ_goGeo8      Val|V|PT  (radius,offset,dist,param)
  static int lG8[] = {Typ_VAR, Typ_PT, Typ_LN, Typ_CI,
                      Typ_Val, Typ_XVal, Typ_YVal, Typ_ZVal, 0}; 

  // Typ_go_LCS      L|C|S(Ell,Bsp,Plg,CCV)   NOT P|D|A|B
  static int lLS[] = {Typ_LN, Typ_CI, Typ_CV, 0};

  // Typ_go_RA       plane|surface
  static int lRA[] = {Typ_PLN, Typ_SUR, 0};

  // Typ_go_lf1      L|C|S(Ell,Bsp,Plg,CCV)   NOT P|D|A|B|contour
  static int lf1[] = {Typ_LN, Typ_CI, Typ_CV, 0};

  // Typ_go_lf2      L|C|S(Ell,Bsp,Plg)       NOT P|D|A|B|CCV
  static int lf2[] = {Typ_LN, Typ_CI, Typ_CV, 0};

  // Typ_go_JNT      P|GText
  static int lJT[] = {Typ_PT, Typ_GTXT, 0};

  // empty,unused,error
  static int l_E[] = {0};

  //                  0          1         2        3        4
  //                  5          6         7        8        9
  static int *pa[] = {l_E,       lPr,      lG1,     l_E,     lPD,     // 0-4
                      l_E,       lG5,      lGS,     lG6,     lG7,     // 5-9
                      lG8,       l_E,      lLS,     l_E,     lRA,     // 10-14
                      lf1,       lf2,      lJT,     l_E,     l_E};    // 15-19

                      // l_E,       l_E,      l_E,     l_E,     l_E,     // 20-24
                      // l_E,       l_E,      l_E,     l_E,     l_E};    // 25-29

  int     i1, *ia;


  // printf("AP_typLst_grpTyp %d\n",rTyp);

  i1 = rTyp - 1000;
  if(i1 > 19) goto L_err;

  ia = pa[i1];

  if(ia == l_E) goto L_err;

    // TESTBLOCK
    // for(i1=0;i1<100;++i1) {
      // if(!ia[i1])break; printf(" Lst_grpTyp[%d]=%d\n",i1,ia[i1]);}
    // printf("ex-AP_typLst_grpTyp \n");
    // END TESTBLOCK


  L_exit:
    // printf("ex-AP_typLst_grpTyp in-%d\n",rTyp);
  return ia;
  

  L_err:
  printf("***************** AP_typLst_grpTyp TODO: %d\n",rTyp);
  TX_Print("***************** AP_typLst_grpTyp TODO: %d",rTyp);
  ia = NULL;
  goto L_exit;

}
 

//================================================================
  int AP_typ__ck_ato__ (ObjAto *ato, int typ) {
//================================================================
// AP_typ__ck_ato__      check if ato has components of requested type
// Input:
//   typ        type to check; basicTyp or groupTyp
// Output:
//   retCod     outTyp, basicTyp
// was IE_inpCkTyp


  int   irc=0, i1, *typLst;



  // printf("AP_typ__ck_ato__ typ=%d\n",typ);
  // ATO_dump__ (ato, "  ck_ato__-ato");

  // skip empty ato
  if(ato->nr < 1) goto L_ok;

  if(typ >= Typ_goGeom) goto L_groupTyp;

  // check basicTyp
  irc = AP_typ__ck_ato_b (ato, typ);
  if(irc > 0) goto L_ok;
  goto L_err;


  //================================================================
  L_groupTyp:

  // selectionModifiers (Typ > Typ_FncVAR1) not yet implemented ..
  if(typ >= 1020) goto L_ok;

  // get list of types for Typ_go_PD
  if(typ == Typ_goGeom) { typ = ato->typ[0]; goto L_ok; }   // all objs

  // get list of ..
  typLst = AP_typLst_grpTyp (typ);
  if(!typLst) goto L_err;


  // loop tru typLst
  i1 = 0;
  while (typLst[i1]) {
    typ = AP_typ__ck_ato_b (ato, typLst[i1]);
    if(typ > 0) goto L_ok;
    ++i1;
  }

  L_err:
  typ = -1;


  //================================================================
  L_ok:

    // printf("ex-AP_typ__ck_ato__ %d\n",typ);
  return typ;

}


//================================================================
  int AP_typ__ck_ato_b (ObjAto *ato, int typ) {
//================================================================
// AP_typ__ck_ato_b      check if ato has components of requested basicType
// Input:
//   typ        type to check; basicTyp
// Output:
//   retCod     outTyp; -1=not useful components
// was IE_inpCkTyp  see IE_inpTxtOut


  int   i1, *typLst;

  // printf("AP_typ__ck_ato_b typ=%d\n",typ);
  // ATO_dump__ (ato, "AP_typ__ck_ato_b");

  // skip modifiers TODO
  if(TYP_IS_MOD(typ))     goto L_ok;
  // skip modelID's  TODO
  if((typ == TYP_FilNam)    ||
     (typ == Typ_SubModel)  ||
     (typ == Typ_CtlgPart))    goto L_ok;


  // test single obj in ato
  if(ato->nr == 1) {
    if(ato->typ[0] == typ) goto L_ok;
  }


  // loop tru components of ato, test



  L_err:
  typ = -1;


  //================================================================
  L_ok:
    // printf("ex-AP_typ__ck_ato_b %d\n",typ);
  return typ;

}
 

/*
//================================================================
  int UTO_cv_ck_degen (int otyp, void *obj) {
//================================================================
/// \code
/// UTO_cv_ck_degen        test if curve is degenerated
/// returns 0=NO, 1=YES
/// TODO: all
/// \endcode

  return 1;

}
 


//================================================================
  int AP_ato_srcTxt (int *atoTyp, double *atoVal, char *cmd) {
//================================================================
// AP_ato_srcTxt       get atomicObj from Typ_Txt (after AP_typ_srcExpr)
// Input:
//   txt      objectCodes/constantValue/constantObject (from AP_typ_srcExpr)
// Output:
//   atoTyp   Typ_cmdNCsub from objectCodes; Val= ?
//            Typ_Val from constantValue; Val=Value.
//            Typ_VC (from ed "DX") or Typ_PLN (from eg "RZ"); Val=DB-ind.

// see also APT_decode_ausdr1
// see also APED_dec_txt

  int    i1;

  printf("AP_ato_srcTxt |%s|\n",cmd);


  //----------------------------------------------------------------
  // check for constantValues (PI, RAD_1 RAD_90 RAD_180 RAD_360 SR_2 SR_3)
  // and constantObjects      (DX DY DZ DIX DIY DIZ RX RY RZ)

  if (cmd[0] == 'D') {

      if (!strcmp (cmd, "DX")) {
        *atoVal = DB_VCX_IND;
        goto L_constantObject_VC;

      } else if (!strcmp (cmd, "DY")) {
        *atoVal = DB_VCY_IND;
        goto L_constantObject_VC;

      } else if (!strcmp (cmd, "DZ")) {
        *atoVal = DB_VCZ_IND;
        goto L_constantObject_VC;

      } else if (!strcmp (cmd, "DIX")) {
        *atoVal = DB_VCIX_IND;
        goto L_constantObject_VC;

      } else if (!strcmp (cmd, "DIY")) {
        *atoVal = DB_VCIY_IND;
        goto L_constantObject_VC;

      } else if (!strcmp (cmd, "DIZ")) {
        *atoVal = DB_VCIZ_IND;
        goto L_constantObject_VC;

      }


    } else if (cmd[0] == 'P') {

      if (!strcmp (cmd, "PI")) {
        *atoVal = RAD_180;
        goto L_constantValue;
      }



    } else if (cmd[0] == 'R') {

      if (!strcmp (cmd, "RX")) {
        *atoVal = DB_PLX_IND;
        goto L_constantObject_PLN;

      } else if (!strcmp (cmd, "RY")) {
        *atoVal = DB_PLY_IND;
        goto L_constantObject_PLN;

      } else if (!strcmp (cmd, "RZ")) {
        *atoVal = DB_PLZ_IND;
        goto L_constantObject_PLN;

      } else if (!strcmp (cmd, "RAD_360")) {
        *atoVal = RAD_360;
        goto L_constantValue;

      } else if (!strcmp (cmd, "RAD_180")) {
        *atoVal = RAD_180;
        goto L_constantValue;

      } else if (!strcmp (cmd, "RAD_90")) {
        *atoVal = RAD_90;
        goto L_constantValue;

      } else if (!strcmp (cmd, "RAD_1")) {
        *atoVal = RAD_1;
        goto L_constantValue;

      }


    } else if (cmd[0] == 'S') {

      if (!strcmp (cmd, "SR_3")) {
        *atoVal = SR_3;
        goto L_constantValue;

      } else if (!strcmp (cmd, "SR_2")) {
        *atoVal = SR_2;
        goto L_constantValue;

      }
    }


  //----------------------------------------------------------------
  // check for objectCodes (CUT, REV, ...) ObjCodTab Typ_Cmd1
   i1 = UTX_cmp_word_wordtab (ObjCodTab, cmd);
    if (i1 >= 0) {
      // printf(" found SubCmd %d\n",i1);
      *atoTyp = Typ_cmdNCsub;
      *atoVal = i1;
      goto L_exit;
    }


    *atoTyp = Typ_Error;
    goto L_exit;


  //----------------------------------------------------------------
  L_constantObject_VC:
    *atoTyp = Typ_VC;
    goto L_exit;
    
  L_constantObject_PLN:
    *atoTyp = Typ_PLN;
    goto L_exit;
    
  L_constantValue:
    *atoTyp = Typ_Val;
    goto L_exit;


  //----------------------------------------------------------------
  L_exit:

    printf("ex AP_ato_srcTxt typ=%d val%lf\n",*atoTyp,*atoVal);



  return 0;

}
*/

// EOF
