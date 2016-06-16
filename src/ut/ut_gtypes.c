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

APED_dbo_oid      give typ, index from text "P12"
APED_oid_dbo__      make name from typ and DB-Index  (visible object types)
APED_oid_dbo_all      make name from typ and DB-index  (all types)
APED_oid_vc         get oid for Vector; (DX or DIX or D#)

AP_typ_srcExpr      decode expression
AP_typ_FncNam       get type of function

AP_typ_typChar      make typ from typChar  ("P" -> Typ_PT)
AP_typChar_typ      make typChar from typ  (Typ_PT -> 'P')
AP_typ_2_bastyp     Typ_CVBSP -> Typ_CV   oder Typ_SURRU -> Typ_SUR
AP_cmp_typ          check for identical types

AP_src_typ__        make text from (int)typ; Typ_PT --> "PT" ..
DB_Typ_Char         give typ(int) from text "PT" --> Typ_PT;

UTO_ck_dbsTyp       check object-typ (struct or object)
UTO_ck_typTyp       check if typ=curve (Typ_lFig) or surface (Typ_SUR)
UTO_ck_curvLimTyp   check if CurveLimits are Points or parameters
UTO_ck_curvForm     check if objTyp is ObjGX or other typ
UTO_ck_surfTyp      returns surfSubTyp

List_functions_end:
=====================================================

\endcode *//*----------------------------------------




*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_geo.h"
#include "../ut/ut_txt.h"                // UTX_pos_skipLeadBlk
#include "../db/ut_DB.h"                 // DB_VCX_IND DB_PLX_IND

#include "../gui/gui_types.h"

#include "../ut/ut_gtypes.h"

#include "../xa/xa_sele.h"               // Typ_FncPrv



//----------------------------------------------------------------
  static char *TypTxtTab0[]={
//0         1         2         3         4
//5         6         7         8         9
  "Error",  "Var",    "Vec",    "PT",     "LN",        // 0-
  "CI",     "-",      "-",      "-",      "-",
  "VC2",    "PT2",    "LN2",    "CI2",    "CI2C"};     // 10-

  static char *TypTxtTab20[]={
  "CV",     "CvPlg",  "CvPsp3", "CvBsp",  "CvRBsp",    // 20-
  "CvCcv",  "CvEll",  "CvBez",  "CvRBez", "Clot",
  "CvLnTab","CV2",    "CvPlg2", "CvPsp2", "CvBsp2",    // 30-
  "CvRBsp2","CvCcv2", "CvEll2", "CvEll2C","CvBez2"};

  static char *TypTxtTab40[]={                         // 40-
  "PLN"};

  static char *TypTxtTab50[]={
  "SUR",    "SurCon", "SurTor", "SurRU",  "SurRV",     // 50-
  "SurSwp", "SurBsp", "SurRBsp","SurPln", "SurTps",
  "SurHat", "SurCir", "SurStrip","SurBnd","SurMsh",    // 60-
  "SurPtab","SurFac3","SurFacQ","SurTri", "-",
  "SurGL_", "SurGLpp","SurGLpo","SurGLis","SurEdg"};   // 70-

  static char *TypTxtTab80[]={
  "SOL",    "Sphere", "Conus",  "Torus",  "Prism",     // 80-
  "BREP"};

  static char *TypTxtTab90[]={
  "Note",   "TxtA",   "TxtG",   "Dimen",  "Dim3",      // 90-
  "Tag",    "------", "------", "------", "------"};

  static char *TypTxtTab100[]={
  "SymBmp", "SymVec", "SymRef", "SymRef2","Texture",  // 100-
  "TEXB",   "TEXR",   "Box",    "EdgLn",  "------"};

  static char *TypTxtTab110[]={
  "TmpPT",  "Vertex", "EyePT",  "------", "------"};   // 110-

  static char *TypTxtTab120[]={
  "Part",   "Grp",    "ModBas", "Model",  "Mockup",    // 120-
  "Dit",    "Joint",  "Proc"};

  static char *TypTxtTab130[]={   /// geom. parameters
  "Val",    "X-coord","Y-coord","Z-coord","DistX",     // 130-
  "DistY",  "DistZ",  "Angle",  "Radius", "Dist",
  "Par1",   "UVal",   "VVal",   "------", "------",    // 140-
  "------", "------", "------", "------", "------"};

  static char *TypTxtTab150[]={   /// attributes
  "Typ",    "Subtyp", "cmdSub", "Address","Size",      // 150-
  "Index",  "Name",   "Color",  "LnTyp",  "Thick",
  "G_Att",  "Activ",  "Layer",  "AppObj", "AppDat",    // 160-
  "Tool",   "------", "------", "------", "------"};

  static char *TypTxtTab170[]={   /// dataFormats
  "Data",   "Int1",   "Int2",   "Int4",   "Float4",    // 170-
  "Float8", "Int4Tab","-",      "-",      "-",
  "poly--", "poly1-", "poly3-", "polyD3", "-",         // 180-
  "Mat3x2", "Mat3x3", "Mat4x3", "Mat4x4", "-"};        // 185-
  static char *TypTxtTab190[]={   /// text
  "Text",   "String", "StrDel", "StrDel1","FncNam",    // 190-
  "FilNam", "CtlgPart","NumString","ConstVal","ConstObj"};
  static char *TypTxtTab200[]={   /// containers
  "Memspc", "MemTab", "ObjRange","ObjSRC","ObjGX",     // 200-
  "------", "------", "ObjDB",  "------", "------"};
  static char *TypTxtTab210[]={   /// transformations
  "Tra",    "TraTra", "TraRot", "TraMat", "------"};   // 210-
  static char *TypTxtTab220[]={   /// operators
  "OpeEQ",  "OpeNE",  "OpeLT",  "OpeGT",  "OpeGE",     // 220-
  "OpeLE",  "OpeAND", "OpeOR",  "Ope---", "Ope---"};
  static char *TypTxtTab230[]={   /// modifiers  TYP_IS_MOD
  "Mod",    "Mod1--", "Mod2--", "ModCWCCW","ModREV",   // 230-
  "ModCX",  "ModCTRL","ModPERP","ModPARL","ModHIX",
  "Mod---", "Mod---", "Mod---", "Mod---", "Mod---",    // 240-
  "ModCCW", "ModCW",  "ModIN",  "ModOUT", "ModAux",
  "ModUNL", "------", "------", "------", "------"};   // 250-
  // static char *TypTxtTab250[]={   /// events
  // "MsBtL",  "MsBrM",  "MsBtR",  "MsBt2L", "------",    // 250-
  // "------", "------", "------", "------", "------"};
  static char *TypTxtTab260[]={   /// functions
  "FuInit", "FuInit1","FuMod",  "FuAdd",  "FuDel",     // 260-
  "FuEnd",  "FuExit", "Fu----", "Fu----", "Fu----"};
  static char *TypTxtTab270[]={   /// function parameters
  "ALL",    "Done",   "NULL",   "Last",   "SIZ",       // 270-
  "------", "------", "------", "------", "------",
  "OpmPlus","OpmMinus","OpmMult","OpmDiv","------",    // 280-
  "------", "------", "------", "------", "------",
  "FcmSQRT","FcmSIN", "FcmCOS", "FcmTAN", "FcmASIN",   // 290-
  "FcmACOS","FcmATAN","FcmABS", "FcmFIX", "FcmRND"};
  static char *TypTxtTab1000[]={   /// selectionGroups
  "goGeom", "goPrim", "goGeo1", "goGeo2", "goGeo3",    // 1000-
  "goGeo4", "goGeo5", "goSUSU", "goGeo6", "goGeo7",
  "goGeo8", "goAxis", "goLR",   "go----", "go----"};   // 1010-
  static char *TypTxtTab1020[]={   /// selectionModifiers
  "FncVAR1","FncVAR2","FncVC1", "FncVC2", "FncNxt",    // 1020-
  "FncPrv", "FncDirX","FncOnO", "FncOnC", "Fnc---"};
  static char *TypTxtTab1100[]={   /// aux
  "ValSymTyp","StrDir1","Obj0", "------",  "------"};  // 1100-
//  0         1         2         3         4
//  5         6         7         8         9



//----------------------------------------------------------------
  static char  *Fc1TxtTab[] = {
// 00             1              2              3              4
  "VAL",         "ANG",         "RAD",         "MOD",         "NEW",         
  ""};

  static int Fc1TypTab[]={
  Typ_Val,       Typ_Angle,     Typ_Rad,       Typ_modif,     TYP_FuncInit};

  static char  *FcmTxtTab[] = {
// 00             1              2              3              4
  "SQRT",        "SIN",         "COS",         "TAN",         "ASIN",
  "ACOS",        "ATAN",        "ABS",         "FIX",         "RND",
  ""};

char  *NcoTxtTab[] = {  // num. constants
// 00             1              2              3              4
  "PI",          "RAD_1",       "RAD_90",      "RAD_180",     "RAD_360",
  "SR_2",        "SR_3",
  ""};

char  *GcoTxtTab[] = {  // geom. constants
// 00             1              2              3              4
  "DX",          "DY",          "DZ",          "DIX",         "DIY",
  "DIZ",         "RX",          "RY",          "RZ",       
  ""};
/*
char  *MOpTxtTab[] = {   // math.operators
// 00             1              2              3              4
  "+",           "-",           "/",           "*",
  ""};
*/
char  MOpTxtStr[] = "+-*/";  // math.operators



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
  "UNUSED",      "UNUSED",      "RBSP",        "ARC1",         "CTRL",
  "LDRP",        "LDRC",        "LDRS",        "CTLG",         "ISO",
// 60
  "FW",          "CX",          "PTAB",        "MSH",          "CLOT",
  "TNG",         "IMP",         "BSP1",        "PARL",         "BLEND",
// 70
  "UNL",         "UNL1",        "UNL2",        ""};

// last word must be "" !

// die zugehoerige enum ist in ../ci/NC_apt.h
// unused: RI LE HI LO SKWC
// nur f WCUT: MA RADI RADC T_MA T_RISO T_RCON SKWC




//----------------------------------------------------------------
// text for 2D-buttons ..
  static char *GL2Dtxt1[]={" CW","REV"," CX","CTRL","PERP","PARL"};
                        // 1020   1         2      3      4      5 
  static char *GL2Dtxt2[]={" V+"," V-",    "VC+", "VC-", "NXT", "PRV",
                        //  6     7         8
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
/// see also APED_oid_dbo__ AP_typ_2_bastyp AP_cre_defHdr
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


    // printf("ex APED_oid_dbo_all |%s| %d %ld %ld\n",oid, typ, dbi, dli);


  return irc;

}


//====================================================================
  int APED_oid_dbo__ (char *buf, int typ, long ind) {
//====================================================================
/// \code
/// APED_oid_dbo__      make name from typ and DB-Index  (visible object types)
/// see also APED_oid_dbo_all AP_typ_2_bastyp AP_cre_defHdr
/// \endcode

// FuncNam should be APED_oid_dbo_all

  char   *p1;

  // printf("APED_oid_dbo__ %d %d\n",typ,ind);


  if(       (typ == Typ_PT)    ||
            (typ == Typ_PT2)   ||
            (typ == Typ_TmpPT))      {
    sprintf(buf, "P%ld", ind);


  } else if((typ == Typ_LN)    ||
            (typ == Typ_LN2))        {
    sprintf(buf, "L%ld", ind);


  } else if((typ == Typ_CI)    ||
            (typ == Typ_CI2))        {
    sprintf(buf, "C%ld", ind);


  } else if(typ == Typ_PLN) {
    if(ind >= 0) sprintf(buf, "R%ld", ind);
    else {
      if     (ind == DB_PLX_IND) sprintf(buf, "RX");
      else if(ind == DB_PLY_IND) sprintf(buf, "RY");
      else if(ind == DB_PLZ_IND) sprintf(buf, "RZ");
    }



  } else if(typ == Typ_VC) {
    if(ind >= 0) sprintf(buf, "D%ld", ind);
    else {
      if     (ind == DB_VCX_IND) sprintf(buf, "DX");
      else if(ind == DB_VCY_IND) sprintf(buf, "DY");
      else if(ind == DB_VCZ_IND) sprintf(buf, "DZ");
      else if(ind == DB_VCIX_IND) sprintf(buf, "DIX");
      else if(ind == DB_VCIY_IND) sprintf(buf, "DIY");
      else if(ind == DB_VCIZ_IND) sprintf(buf, "DIZ");
      else sprintf(buf, "D%ld", ind);
    }


  } else if((typ == Typ_CV)      ||
            (typ == Typ_CVLNA)   ||
            (typ == Typ_CVPOL)   ||
            (typ == Typ_CVPOL2)  ||
            (typ == Typ_CVBSP)   ||
            (typ == Typ_CVRBSP)  ||
            (typ == Typ_CVELL)   ||
            (typ == Typ_CVCLOT)  ||
            (typ == Typ_CVCCV))      {
    sprintf(buf, "S%ld", ind);


  } else if((typ == Typ_SUR)      ||
            (typ == Typ_SURRU)    ||
            (typ == Typ_SURRV)    ||
            (typ == Typ_SURCIR)   ||
            (typ == Typ_SURSTRIP) ||
            (typ == Typ_SURBSP)   ||
            (typ == Typ_SURRBSP)  ||
            (typ == Typ_SURPTAB)  ||
            (typ == Typ_SURMSH))    {
    sprintf(buf, "A%ld", ind);


  } else if((typ == Typ_SOL)   ||
            (typ == Typ_CON)   ||
            (typ == Typ_TOR))        {
    sprintf(buf, "B%ld", ind);


  } else if((typ == Typ_Note)  ||
            (typ == Typ_ATXT)  ||
            (typ == Typ_GTXT)  ||
            (typ == Typ_Dimen) ||
            (typ == Typ_SymV)  ||                           // 2011-08-07
            (typ == Typ_Tag))         {
    sprintf(buf, "N%ld", ind);


  } else if((typ == Typ_Model) ||
            (typ == Typ_Mock))        {
    sprintf(buf, "M%ld", ind);


  } else if(typ == Typ_Val) {
    //sprintf (buf,"%f,%f",pt1.x,pt1.y);
    //strcpy(buf, ",");
    buf[0] = '\0';
    return -1;
    // UTX_add_fl_u2 (buf, pt1.x,pt1.y);


  } else if(typ == Typ_VAR) {  // hat hoechste Select-Prioritaet in GL
    sprintf(buf, "V%ld", ind);


  } else if(typ == Typ_Tra) {  // Transformation
    sprintf(buf, "T%ld", ind);


  } else if(typ == Typ_Activ) {  // Activity
    sprintf(buf, "I%ld", ind);


  } else if(typ == Typ_Joint) {  // Joint
    sprintf(buf, "J%ld", ind);


  } else if(typ == Typ_Group) {  // undefined  // 2011-06-01 was Typ_goGeom
    sprintf(buf, "U%ld", ind);


  } else if(typ == Typ_SymB) {  // temporary-SymBmp
    sprintf(buf, "Sym%ld", ind);


  } else if(typ == Typ_APPOBJ) {  // ApplicationObj
    sprintf(buf, "App%ld", ind);


  } else {

    strcpy(buf, "-");
    p1 = AP_src_typ__ (typ);
    // TX_Error("APED_oid_dbo__: Objekttyp %d gefunden",typ);
    TX_Print("Err APED_oid_dbo__ obj %s %d %d",p1,ind,typ);
    return -1;

  }

  // printf("ex APED_oid_dbo__ |%s| %d\n",buf,strlen(buf)); // nur Testausg.

  return 1;

}


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
/// see AP_vec_txt
/// \endcode

  int       i1, ii;
  double    dx, dy, dz;
  char      *vcTab[]={NULL,"DX","DY","DZ","DIX","DIY","DIZ"};


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
///   OUT:
/// defTyp: see wincut_base.h
///         values return 0 !
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


  // printf("APED_dbo_oid |%s| %p\n",txtIn,defInd);
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
  if (isdigit(txt[1])) {
    *defInd = atoi (&txt[1]);
    rc = 0;
    goto Fertig;
  }


  rc = -2;

  if(*defTyp == Typ_VC) {  // test DX DY DZ DIX DIY DIZ
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
  else if(typ < 110)      p1 = TypTxtTab90[typ - 90];
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


  if(typ >= Typ_FncVAR1) {
    if(typ < Typ_EOT) return p1 = GL2Dtxt2[typ - Typ_FncVAR1];
    return NULL;
  }

  if(typ >= Typ_modCWCCW) {
    if(typ < Typ_modHIX) return p1 = GL2Dtxt1[typ - Typ_modCWCCW];
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

    case Typ_CVCCV:
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

    case Typ_Data:
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
  int    DB_Typ_Char      (char* auxBuf) {
//================================================================
/// \code
/// Text > int-3D-Typ
/// give typ(int) from text "PT" --> Typ_PT
/// see also AP_src_typ__ (objTypTxt from typ)
/// \endcode


  //TX_Print("DB_Typ_Char /%s/",auxBuf);


  if(!strcmp (auxBuf, "PT")) {
    return Typ_PT;

  } else if(!strcmp (auxBuf, "LN")) {
    return Typ_LN;


  } else if(!strcmp (auxBuf, "CI")) {
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
            (typ == Typ_CVCCV))      {
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
  int AP_typ_2_bastyp (int typ) {
//====================================================================
/// \code
/// give basictyp from typ;
/// makes Typ_SURRU -> Typ_SUR   or Typ_CVBSP -> Typ_CV.
/// 
/// siehe APED_oid_dbo__ AP_cmp_typ
/// \endcode


  if(typ >= Typ_CV) {
    if(typ < Typ_PLN)   return Typ_CV;       // S
    if(typ < Typ_SUR)   return Typ_PLN;      // R
    if(typ < Typ_SOL)   return Typ_SUR;      // A
    if(typ < Typ_Note)  return Typ_SOL;      // B
    if(typ < Typ_SymB)  return Typ_Note;     // N

    if(typ == Typ_Mock) return Typ_Model;    // M

  }

/*
  if((typ == Typ_CVLNA)   ||
     (typ == Typ_CVPOL)   ||
     (typ == Typ_CVBSP)   ||
     (typ == Typ_CVRBSP)  ||
     (typ == Typ_CVELL)   ||
     (typ == Typ_CVCLOT)  ||
     (typ == Typ_CVCCV))
    return Typ_CV;                        // S


  if((typ == Typ_SURRU)   ||
     (typ == Typ_SURRV)   ||
     (typ == Typ_SURBSP)  ||
     (typ == Typ_SURPTAB) ||
     (typ == Typ_SURMSH)  ||
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


  if((typ == Typ_CI)     ||
     (typ == Typ_CI2))
    return Typ_CI;                        // C


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


  int   iTyp;
  char  s1[32];


  // printf("AP_typ_FncNam %d |%s|\n",sLen,fncNam);

  // get strLen
  while (fncNam[sLen - 1] == ' ') --sLen;

  if(sLen > 1) goto L_fnc1;

  // test for obj-typ
  iTyp = AP_typ_typChar (*fncNam);
  if(iTyp) goto L_exit;
  if(*fncNam == 'X') { iTyp = Typ_ValX; goto L_exit;}
  if(*fncNam == 'Y') { iTyp = Typ_ValY; goto L_exit;}
  if(*fncNam == 'Z') { iTyp = Typ_ValZ; goto L_exit;}
  goto L_exit;


  //----------------------------------------------------------------
  L_fnc1:
  if(sLen > 30) { iTyp = -2; goto L_exit;}
  strncpy (s1, fncNam, sLen);
  s1[sLen] = '\0';
    // printf(" s1 %d |%s|\n",sLen,s1);

  // VAL ANG RAD NEW
  iTyp = UTX_cmp_word_wordtab (Fc1TxtTab, s1);
  if(iTyp >= 0) {
    iTyp = Fc1TypTab[iTyp];
    goto L_exit;
  }


  // SQRT SIN COS TAN ASIN ACOS ATAN ABS FIX RND
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
 
  printf("AP_typ_srcExpr |%s|\n",expr);


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
      printf(" L_func__:\n");

    // Function: implicitObject|mathFunction

    // test for implicitObject ((Val(), P()-B(), X|Y|Z() ANG() RAD() NEW())
    // (APT_decode_func)

    // test for mathFunction (SQRT SIN COS TAN ASIN ACOS ATAN ABS FIX RND)
    // (APT_decode_func)

    ityp = Typ_FncNam;
    goto L_exit;



  //----------------------------------------------------------------
  L_exit:
    printf("ex AP_typ_srcExpr %d |%s|\n",ityp,*pNxt);

  return ityp;

}

//====================================================================
  int AP_cmp_typ (int typ1, int typ2) {
//====================================================================
/// \code
/// check if types identical;
/// RC = 0 = ident;
/// 
/// see AP_typ_2_bastyp
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
    if(typ2 == Typ_CVCCV) return 0;
    // case Typ_CVPOL:
    // case Typ_CVBSP:
    // case Typ_CVRBSP:
    // case Typ_CVELL:
    // case Typ_CVCLOT:
    // case Typ_CVCCV:



  }

  return -1;

}


//================================================================
  int UTO_ck_dbsTyp (int typ) {
//================================================================
/// \code
/// UTO_ck_dbsTyp           check object-typ (struct or object)
///   typ              must be a basictype; see AP_typ_2_bastyp
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
///   typ    must be basic-type (AP_typ_2_bastyp)
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
    case Typ_CVCCV:

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
*/
 
//================================================================
  int UTO_cv_ck_clo (int otyp, void *obj) {
//================================================================
/// \code
/// UTO_cv_ck_clo        test if curve is closed; ignore trimmed.
/// 360-deg-circ/elli
/// returns   1 NO, not closed
///           0 YES, closed
///          -2 degenerated.
/// TODO: CCV
/// \endcode

  int  iClo;


  iClo = 1;  // 1=NO; for L

  if(otyp == Typ_CI) {
    iClo = UT3D_ck_ci360 ((Circ*)obj);

  } else if(otyp == Typ_CVELL) {
    iClo = UT3D_ck_el360 ((CurvElli*)obj);

  } else if(otyp == Typ_CVPOL) {
    iClo = UT3D_ck_plgclo ((CurvPoly*)obj);
    if(iClo == -1) iClo = 0;    // closed=closed,but trimmed

  } else if(otyp == Typ_CVBSP) {
    iClo = UT3D_bsp_ck_closed_tr ((CurvBSpl*)obj);

  } else if(otyp == Typ_CVRBSP) {
    iClo = UT3D_rbspl_ck_closed ((CurvRBSpl*)obj);

  } else {
    TX_Error("UTO_cv_ck_clo E001_%d",otyp);
    return -1;
  }

    // printf("ex UTO_cv_ck_clo %d\n",iClo); // 0=yes,1=no,-1=degen

  return iClo;

}

 
//================================================================
  int UTO_ck_surfTyp (ObjGX *oxi) {
//================================================================
/// \code
/// UTO_ck_surfTyp          returns surfSubTyp
/// Input:
///  oxi      su_tab-record
/// Output:
///  retCod   Typ_SURTPS|Typ_SURBSP|Typ_SURRU|Typ_SURRV ..
/// 
/// see also SUR_ck_typ (get details for surfaces)
/// see GR_DrawSur TSU_DrawSurT_
/// \endcode


  int   ityp;
  ObjGX *ox1;

  // printf("UTO_ck_surfTyp %d %d\n",oxi->typ,oxi->form);
  // UT3D_stru_dump (Typ_ObjGX, oxi, "");
  // UTO_dump__ (oxi, "");

  // if(oxi->typ == Typ_SURPTAB) return Typ_SURPTAB;


  if(oxi->typ == Typ_SUR) {    // eg Typ_PT  from   A=PTAB  
    ityp = oxi->form;
    if(ityp == Typ_ObjGX) {     // planar od gelocht ..
      // form of 1.subObj: Typ_SURPLN or Index of supportSurface
      ox1 = (ObjGX*)oxi->data;
      ityp = ox1->form;
      if(ityp != Typ_SURPLN) ityp = Typ_SURTPS;
    }
    goto L_exit;

  } else {
    ityp = oxi->typ;
    goto L_exit;
  }



  return -1;


  L_exit:
    // printf("ex UTO_ck_surfTyp %d\n",ityp);
  return ityp;

}

/*
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
