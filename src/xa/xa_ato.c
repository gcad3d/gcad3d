// xa_ato.c                        2011-12-13          RF
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
ATO_ato_eval_geom: add P L C D R S U     (P D already done)
rename ATO_srcTxt -> ATO_eval_ConstVal

ObjCodTab aufteilen into codes fuer Curves, Surfaces, subModels, allgemein.
 Es soll nur die Liste fuer den gesuchten Objekttyp untersucht werden ..

-----------------------------------------------------
Modifications:
2011-12-13 Created. RF.

-----------------------------------------------------
*/
#ifdef globTag
 void ATO(){}
#endif
/*!
\file  ../xa/xa_ato.c
\brief functions handling atomicObjects.
\code
=====================================================
List_functions_start:

ATO_ato_srcLn__         get atomicObjects from sourceLine; full evaluated.
ATO_ato_txo__           get atomic-objects from source-objects
ATO_ato_expr_add        add ato to ato-table
ATO_swap                swap 2 records
ATO_del_rec             delete record
ATO_clear__             delete all records
ATO_clear_block         delete block of records
ATO_clean__             make clean atomicObjects from ato (use ATO_clean_1)
ATO_clean_1             close all gaps (typ == Typ_NULL)
ATO_cpy_rec             copy ATO-record iTo = iFrom, delete iFrom
ATO_sort1               sort types ascending
ATO_pNr__               get nr of parameters following record

ATO_parents__           get all parents of obj (index into ato)

// ATO_ato_srcLn_exp    DO NOT USE  get/add ato as typ/val from modelCode
// ATO_ato_atoTab__     DO NOT USE  get single dbo from ato
// ATO_ato_expr__       DO NOT USE  get struct from modelCode (text).

ATO_ato_typTab          get atomicObjects from aus_typ/aus_tab (old version)
ATO_ato_eval__          evaluate atomic-objects (compute); reduce records.
ATO_ato_eval_geom       evaluate geometrical functions
ATO_ato_obj_pt          get parametric position of point on obj
ATO_srcTxt              get atomicObj from Typ_Txt (after AP_typ_srcExpr)

ATO_malloc              get memspace for atomicObjects
ATO_free                free ObjAto after ATO_malloc
ATO_tmpSpc_get__        get memspace on stack or heap for active func only
ATO_tmpSpc_get_s        get memspace for atomicObjects (string)
ATO_getSpc_tmp__        get temp - memspace for atomicObjects
ATO_getSpc_tmp1         aux.Func for ATO_getSpc_tmp__
ATO_getSpc__            get memSpc for atomicObjects (memspc53/54/55)
ATO_getSpc_siz       compute necessary space for type/value-table
ATO_dump__              dump ato-table

List_functions_end:
=====================================================
for txo-funcs see APED
see also:
APT_obj_ato             create struct from atomicObjs

obsolete:
// ATO_getSpc1             get memSpc for atomicObjects - simple

\endcode *//*----------------------------------------




Struct ObjAto has n records of (int-typ + double-val + short-ilev) records,
  and one pointer for a textstring.

ATO-functions decode a sourcetext into atomic-objects.

ATO_ato_srcLn__:
- get space
- get tso from srcTxt (APED_txo_srcLn__)
- get ato from tso (ATO_ato_txo__)
- eval ato (ATO_ato_eval__)


ObjTXTSRC tso   has level and text and val=offsetposition in text
ObjAto    ato   has level, typ and val = atomic-object; no text



ato.ilev (char *ilev) = array of characters, used as ints (-100 - +100)
/// ilev       level; -1=primary level. NULL=unused


Testcodes:
V20=4
V21=VAL(SQRT(10+VAL(V20+2)))

V20=10
P20=P(-V20 -V20 0)

# test case
v20=val(rad_90)
p20=10 v20 0
p21=p(100 v20 0)


----------------------------------------
ATO_ato_srcLn__           get ato from modelCode (text).
  store values, funcs in ato.val;
  store text as ? in ato.val;
  P(..) L(..) C(..) S(..) are stored as dynamic objects (dbi < 0)

APT_store_obj     store ato in DB;
  Input:  ato, dbTyp,dbi,
    using all APT_decode_..
    using all DB_Store..


IE_inpTxtOut           create/add modelCode from modelCode (eg add "D(*)"
    using ATO_ato_srcLn_exp
TODO: for (inptyp == Typ_Val) ATO_ato_srcLn_exp unused !
TODO: replace ATO_ato_srcLn_exp


----------------------------------------
DO NOT USE:

ATO_ato_srcLn_exp          get/add ato as typ/val from modelCode
  Input:  ModelCode, typ, val
  Output: ato  ?
    used only by IE_inpTxtOut [ATO_ato_expr__ RECU]
    using ATO_ato_atoTab__ ATO_ato_expr__


ATO_ato_atoTab__     get single dbo from ato
  creates / returns dynamic obj
  Input:  typ, ato
  Output: db-typ,dbi.
    used only by ATO_ato_srcLn_exp
    using APT_obj_ato           // get binaryObject from ato
    using DB_store_obj(dbi=-1) // save struct in DB


ATO_ato_expr__            get struct from modelCode (text).
  ModelCode must result in a single DB-obj (resolved by ATO_ato_srcLn__)
  copies struct -> output-data
  Input:  typ, ModelCode
  Output: binObj, struct is <typ>
    used only by ATO_ato_srcLn_exp
    using [ATO_ato_srcLn_exp RECURSE]
TODO: kopieren unnutz, da alle structs=DB-obj's; change output-data into pointer


APT_obj_ato                create struct from atomicObjs
  Input:  typ, ato
  Output: binObj, struct is <typ>
    used only by ATO_ato_atoTab__
    using all APT_decode_..





----------------------------------------
*/

#ifdef _MSC_VER
#include "MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // va_list
#include <ctype.h>                       // isdigit toupper

#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_gtypes.h"           // AP_src_typ__
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_txTab.h"            // TxtTab
#include "../ut/ut_memTab.h"           // MemTab
#include "../ut/ut_BitTab.h"           // BitTab

#include "../db/ut_DB.h"               // DB_VCX_IND DB_PLX_IND

#include "../xa/xa_ed_mem.h"           // APED OSRC
#include "../xa/xa_mem.h"              // memspc..
#include "../xa/xa_ato.h"              // ATO_getSpc_tmp__
#include "../xa/xa_sele.h"             // Typ_go*

#include "../gui/gui__.h"              // GUI_* TYP_EventEnter




//================================================================
// EXTERN SYMBOLS:

// ex ../ut/ut_gtypes.c
extern char   *ObjCodTab[];
extern double NcoValTab[];




//================================================================
// PROTOTYPES:
  int ATO_ato_srcLn_exp (ObjAto *ato, int *typ, double *val, char *srcLn);
  int ATO_ato_expr_add (ObjAto *ato, int typ, double val, int lev);
  int ATO_dump__ (ObjAto *ato, char *txt);




//================================================================
  int ATO_malloc (ObjAto *ato, int sizTab) {
//================================================================
// ATO_malloc              get memspace for atomicObjects
// get space for the ato-tables typ,val,ilev, but not for the obj itself.

  int   ib;


  ato->nr = 0;
  if(ato->siz >= sizTab) return 0;

  ib = ATO_getSpc_siz (sizTab);

  ato->typ = (int*) realloc (ato->typ, ib);

  if(!ato->typ) {TX_Print("ATO_malloc E1"); return -1;}

  ato->val = (double*)((char*)ato->typ + sizTab * sizeof(int));

  ato->ilev = (short*)((char*)ato->val + sizTab * sizeof(double));

  ato->siz = sizTab;
  ato->txt = NULL;

  return 0;

}


//================================================================
  int ATO_free (ObjAto *ato) {
//================================================================
// ATO_free                  free ObjAto after ATO_malloc

  if(ato->typ) {
    free (ato->typ);
    ato->typ  = NULL;
    ato->val  = NULL;
    ato->ilev = NULL;
    ato->siz  = 0;
  }

  return 0;

}
 

//================================================================
  int ATO_del_rec (ObjAto *ato, int iDel) {
//================================================================
// ATO_del_rec             delete record

  int   i1;

  MEM_del_IndRec (&ato->nr, ato->typ, iDel);
  MEM_del_DbRec (&i1, ato->val, iDel);
  MEM_del_ShRec (&i1, ato->ilev, iDel);

  return 0;

}
 

//================================================================
  void ATO_tmpSpc_get1 (ObjAto *ato1, int bNr) {
//================================================================
/// ATO_tmpSpc_get          get memspace on stack or heap for active func only
/// if(!ato1->typ)   ERROR
/// if(ato1->spcTyp < 3)  must free at end of calling func - ATO_tmpSpc_free

  int    rNr;


  // test if memspc already got on stack
  if(bNr < SPC_MAX_STK) {
    ato1->spcTyp = MEMTYP_STACK__;   // stack; cannot free
  } else {
    ato1->typ = (int*) realloc (ato1->typ, bNr);
    ato1->spcTyp = MEMTYP_ALLOC__;   // heap - malloc-type; must be freed
  }

  if(!ato1->typ) {TX_Print("ATO_tmpSpc_get1 E1"); return;} 

  rNr = bNr / (sizeof(int) + sizeof(double) + sizeof(short));

  ato1->siz = rNr;

  ato1->val = (double*)((char*)ato1->typ + (rNr * sizeof(int)));
    
  ato1->ilev = (short*)((char*)ato1->val + (rNr * sizeof(double)));

  ato1->txt = NULL;

  ato1->nr  = 0;
}


//================================================================
  int ATO_getSpc_siz (int sizTab) {
//================================================================
/// compute necessary space for ato.typ/val/ilev

  // printf("ATO_getSpc_siz siz=%d\n",sizTab);

  return (sizeof(int) * sizTab) + 
         (sizeof(double) * sizTab) +
         (sizeof(short) * sizTab);
}


//================================================================
  void ATO_getSpc_tmp1 (ObjAto *ato, int sizTab) {
//================================================================
/// \code
/// set ato.typ,val,ilev at memPos from ato.typ
/// aux.Func for ATO_getSpc_tmp__
/// \endcode

  char  *p1;

  // printf("ATO_getSpc_tmp1 %d\n",sizTab);

  p1 = (char*)ato->typ;
  p1 += sizeof(int) * sizTab;

  ato->val = (double*)p1;
  p1 += sizeof(double) * sizTab;

  ato->siz   = sizTab;
  ato->nr    = 0;

  ato->ilev  = (short*)p1;
  ato->txt   = NULL;
  ato->txsiz = 0;
}



//===================================================================
  int ATO_ato_typTab (ObjAto *ato,
                      int aus_anz, int aus_typ[], double aus_tab[]) {
//===================================================================

  int      i1;


  // printf("ATO_ato_typTab %d\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1)
    // printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);


  ato->nr    = aus_anz;
  ato->siz   = aus_anz;
  ato->typ   = aus_typ;
  ato->val   = aus_tab;
  ato->ilev  = NULL;
  ato->txt   = NULL;

    // ATO_dump__ (ato, "ex ATO_ato_typTab");

  return 0;

}

 
//================================================================
  int ATO_ato_atoTab__ (int *aTyp, double *aVal, ObjAto *ato) {
//================================================================
// ATO_ato_atoTab__             get a single ato from 1-n ato's
// decode all atomicObjects in ato, return single atomicObject (aTyp,aVal).
// using APT_obj_ato, DB_store_obj(dbi=-1)
// Output:
//   aTyp    db-typ
//   aVal    db-index as double (NEGATIV = dynamic obj !)
// returns type & value

  int    irc;
  long   dbi;
  char   o1[OBJ_SIZ_MAX];
  void   *vp1;


  // printf("ATO_ato_atoTab__ typ=%d val=%lf\n",*aTyp,*aVal);
  // ATO_dump__ (ato, "_atoTab__-in");


  //----------------------------------------------------------------
  // check / evaluate math.expressions; eg "RAD_180 * 2"
  // VAL() MOD()
  if((*aTyp == Typ_Val)     ||
     (*aTyp == Typ_PTS)     ||
     (*aTyp == Typ_PTI)     ||
     (*aTyp == Typ_SEG)     ||
     (*aTyp == Typ_modif))       {
    irc = APT_decode_Opm (aVal, ato);
    if(irc < 0) {TX_Error("ATO_ato_atoTab__ Opm1"); return -1;}
    goto L_exit;
  }



  //----------------------------------------------------------------
  // check / execute math.Functions
  if(TYP_IS_FCM(*aTyp)) {
    // printf("  math.Func %lf\n",ato->val[0]);

    // execute math.function
    irc = APT_decode_Fmc (aVal, aTyp, ato);
    if(irc < 0) {TX_Error("ATO_ato_atoTab__ E00M"); return -1;}

    *aTyp = Typ_Val;
    *aVal = cos(ato->val[0]);
    goto L_exit;
  }



  //----------------------------------------------------------------
  // get binaryObject o1 from ato
  irc = APT_obj_ato ((void*)o1, *aTyp, ato);
  if(irc < 0) {TX_Error("ATO_ato_atoTab__ E001"); return -1;}
  vp1 = o1;
  if(*aTyp == Typ_CV) {
    // Typ_ObjGX will be returned !!
    *aTyp = ((ObjGX*)o1)->form;
    vp1 = ((ObjGX*)o1)->data;
  }
    // DEB_dump_obj__ (*aTyp, vp1, "  obj_ato-vp1:");



  // store binaryObject temporary
  // see DB_StorePoint < APT_store_obj  APT_decode_func APT_store_obj
  dbi = -1L;
  irc = DB_store_obj (&vp1, *aTyp, *aTyp, vp1, 1, &dbi);
  if(irc < 0) {TX_Error("ATO_ato_atoTab__ E002"); return -1;}
  *aVal = dbi;


  L_exit:
    // printf("ex-ATO_ato_atoTab__ %d %lf\n",*aTyp,*aVal);
    // ATO_dump__ (ato, "_atoTab__-out");

  return 0;

}
 

//================================================================
  int ATO_ato_expr__ (ObjAto *ato, char *exp, int sLen) {
//================================================================
// resolv expression and add it to ato.
// strings are already done ..
// Input:
//   exp     expression-source
//   sLen    length of exp in chars



  int      irc, eTyp, i1;
  double   eVal;
  char     *cpos, *s1;


  // printf("ATO_ato_expr__ |");
  // UTX_dump_c__(exp,sLen);
  // printf("| sLen=%d\n",sLen);
  // ATO_dump__ (ato, " ato_expr__-in");


  // check for math.operators
  // must be control-character (< digit '0')
  if((sLen == 1)&&(*exp < '0')) {
    eVal = 0.;
    switch (*exp) {
      case '+':
        eTyp = TYP_OpmPlus; goto L_add;
      case '-':
        eTyp = TYP_OpmMinus; goto L_add;
      case '*':
        eTyp = TYP_OpmMult; goto L_add;
      case '/':
        eTyp = TYP_OpmDiv; goto L_add;
      case '^':
        eTyp = TYP_OpmPow; goto L_add;
      default:
        TX_Error("ATO_ato_expr__ E002");
        return -1;
    }
  }


  // check type of expression; value, string or function.
  // see also AP_typ_srcExpr
  cpos = exp;


  // check for numeric: digit, '+' or '-' or '.'
  if(isdigit(*cpos)) goto L_num__;
  if((*cpos != '-')&&(*cpos != '+')&&(*cpos != '.')) goto L_strg__;


  //----------------------------------------------------------------
  // expr is a number.
  L_num__:
    eTyp = Typ_Val;
    eVal = atof (cpos);
    goto L_add;


  //----------------------------------------------------------------
  // String, not number.
  // word or object;  words:  PERP|TAN ..  objects: P20
  L_strg__:

    // test for object
    ++cpos;
    if(!isdigit(*cpos)) goto L_code__;

    // decode object eg "P123"
    eTyp = AP_typ_typChar (*exp);
    eVal = atoi (cpos);
    goto L_add;


  //----------------------------------------------------------------
  // String, not number, not object
  L_code__:

    // copy string
    s1 = MEM_alloc_tmp (sLen + 1);
    if(!s1) {TX_Error("ATO_ato_expr__ E001"); return -1;}
    strncpy (s1, exp, sLen);
    s1[sLen] = '\0';

    // test for function
    cpos = strchr (s1, '(');
    if(!cpos) goto L_word__;
      // printf(" **** is func |%s|%s|\n",s1,cpos);

    // resolv function eg "P(1 2 3)" or "COS(.5)"
    // terminate funNam and skip '('
    // *cpos = '\0';
    ++cpos;  // skip '('
    s1[sLen - 1] = '\0';   // remove ')'

    // analyze type of function
    i1 = (cpos - s1) - 1;
    eTyp = AP_typ_FncNam (s1, i1);
    if(eTyp < 0) {TX_Error("ATO_ato_expr__ E003 |%s|",s1); return -1;}
    eVal = -1L;

    // split sourceLine into expressions, add all expressions to ato1.
    // RECURSE !
    irc = ATO_ato_srcLn_exp (ato, &eTyp, &eVal, cpos);
    if(irc < 0) {TX_Print("ATO_ato_expr__ E004"); return -1;}
    goto L_exit;



  //----------------------------------------------------------------
  // objectCodes|constantValue|constantObject
  //     objectCodes    (CUT, REV, ...) ObjCodTab Typ_Cmd1
  //     constantValue  (PI, RAD_1 RAD_90 RAD_180 RAD_360 SR_2 SR_3)
  //     constantObject (DX DY DZ DIX DIY DIZ RX RY RZ)
  L_word__:
      // printf("  L_word__: |%s|\n",s1);
    // get atomicObj from objectCodes/constantValue/constantObject
    ATO_srcTxt (&eTyp, &eVal, s1);


  //----------------------------------------------------------------
  L_add:
    irc = ATO_ato_expr_add (ato, eTyp, eVal, 0);
    if(irc < 0) {TX_Print("ATO_ato_expr__ E005"); return -1;}



  //----------------------------------------------------------------
  L_exit:
    // ATO_dump__ (ato, " ato_expr__-out");
  return 0;

}


//====================================================================
  int ATO_ato_expr_add (ObjAto *ato, int typ, double dVal, int lev) {
//====================================================================
/// \code
/// add ato to ato-table
/// Example: ATO_ato_expr_add (&ato, Typ_PT, 20., 0);
/// \endcode

  // printf("ATO_ato_expr_add typ=%d val=%lf lev=%d\n",typ,dVal,lev);
  // ATO_dump__ (ato, "ATO_ato_expr_add");


  if(ato->nr >= ato->siz) goto L_E1;
  if(lev > INT_16_MAX) goto L_E2;

  ato->typ[ato->nr] = typ;
  ato->val[ato->nr] = dVal;

  if(ato->ilev) {
    ato->ilev[ato->nr] = lev;   // ilev is short*
  }

  // ++(ato->nr);
  ato->nr += 1;

    // ATO_dump__ (ato, "ex-ATO_ato_expr_add");

  return 0;

  //----------------------------------------------------------------
  L_E1:
    TX_Error("ATO_ato_expr_add E001"); return -1;
 
  L_E2:
    TX_Error("ATO_ato_expr_add E002"); return -1;

}


//=======================================================================
  int ATO_ato_srcLn_exp (ObjAto *ato, int *typ, double *val, char *srcLn) {
//=======================================================================
// split sourceLine, add all expressions to ato.

// Input:
//   typ        type of code in srcLn; 0=do not resolv subObjects
//   val        data for typ
//   srcLn      srcTxt to decode; must be \0-terminated.
// Output:
//   ato


  int       irc, i1, aNr, iTyp, ii;
  char      *spa[100];
  int       sSiz[100];
  char      *cp1;
  double    dVal;
  ObjAto    ato1;


  // printf("ATO_ato_srcLn_exp typ=%d val=%lf\n",*typ,*val);
  // printf(" _srcLn__|%s|\n",srcLn);
  // ATO_dump__ (ato, "_srcLn_exp-1");


  // cut srcLn into expressions (get textPos & textLen)
  aNr = UTX_wTab_srcLn (spa, sSiz, 100, srcLn);

    // testDisp splitted line
    // for(i1=0;i1<aNr;++i1)
    // printf(" exp[%d] = %d |%s|\n",i1,sSiz[i1],spa[i1]);


  // create ati1 = child-ato following existing ato's
  ato1 = *ato;  // copy
  if(*typ) {
    ii = ato->nr;
    ato1.typ = &ato->typ[ii];
    ato1.val = &ato->val[ii];
    ato1.nr = 0;
    ato1.siz = ato->siz - ii;
    ato1.txt = srcLn;
  }
    

  // loop tru expressions, add into ato1
  for(i1=0; i1<aNr; ++i1) {

    cp1 = spa[i1];

    // add string to ato
    if(*cp1 == '"') {
      iTyp = Typ_String;
      dVal = cp1 - srcLn;
      irc = ATO_ato_expr_add (&ato1, iTyp, dVal, 0);
      if(irc < 0) {TX_Error("ATO_ato_srcLn_exp E001"); goto L_exit;}

    // resolv expr and add it to ato.
    } else {
      irc = ATO_ato_expr__ (&ato1, cp1, sSiz[i1]);
      if(irc < 0) return -1;
    }

  }
    // printf(" dump child-ato:\n");
    // ATO_dump__ (&ato1, "_srcLn_exp-1");


  // evaluate object (return one single ato from subObjects;i
  //   eg return PT-1 from P(1 2 3)
  if(*typ) {

    // get ato from func (eg create dynObj ..)
    ATO_ato_atoTab__ (typ, val, &ato1);

    // add resulting obj to input-ato
    // create the parent-ato; kill all child-atos
    ATO_ato_expr_add (ato, *typ, *val, 0);

  } else {  // return all atos unmodified
    *ato = ato1;
  }

  irc = 0;

  //----------------------------------------------------------------
  L_exit:
    // ATO_dump__ (&ato1, "ex-ATO_ato_srcLn_exp");
  return irc;

}


//================================================================
  int ATO_dump__ (ObjAto *ato, char *txt) {
//================================================================
//  dump ato-table


  int   i1;
  char  s1[8];


  printf("-------- ATO_dump__ nr=%d siz=%d %s\n",ato->nr,ato->siz,txt);


  for(i1=0; i1 < ato->nr; ++i1) {
    if(ato->ilev) {
      sprintf(s1, "%4d",  ato->ilev[i1]);
    } else {
      strcpy(s1, "----");
    }
    printf(" ato[%d] = %3d %12.3lf  %s  %-16s\n",i1,
      ato->typ[i1],
      ato->val[i1],
      s1,
      AP_src_typ__(ato->typ[i1]));
  }

  return 0;

}


//================================================================
  int ATO_getSpc__ (ObjAto *ato1) {
//================================================================
/// \code
/// ATO_getSpc       get memSpc for atomicObjects
/// uses memspc54 memspc55 memspc53 !
/// \endcode

  int    i1, i2;


  ato1->typ  = (int*) memspc55;
  ato1->val  = (double*) memspc54;
  ato1->ilev = (short*) memspc53;

  i1 = sizeof(memspc55) / sizeof(int);
  i2 = sizeof(memspc54) / sizeof(double);

  ato1->siz = IMIN (i1, i2);
  ato1->nr = 0;

  return 0;

}


/*
//================================================================
  int ATO_getSpc1 (int *tabSiz, int **aus_typ, double **aus_tab) {
//================================================================
/// \code
/// DO NOT USE - use ATO_getSpc__
/// ATO_getSpc1      get memSpc for atomicObjects - simple
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
*/


//================================================================
  int ATO_sort1 (int oNr, int *oTyp, double *oVal) {
//================================================================
/// sort geometric types ascending
///  but do not sort types >= Typ_SymB
// see ATO_sort1 AP_ato_sort1 GL_sel_sort

  int    i1;


  // printf("ATO_sort1 %d\n",oNr);
    // for(i1=0; i1<oNr; ++i1)
    // printf(" %d typ=%d tab=%f\n",i1,oTyp[i1],oVal[i1]);


  L_sort0:

    for(i1=1; i1<oNr; ++i1) {
      if(oTyp[i1] >= Typ_SymB) continue;
      if(oTyp[i1 - 1] >= Typ_SymB) continue;
      if(oTyp[i1] < oTyp[i1 - 1]) {
        MEM_swap_int (&oTyp[i1], &oTyp[i1 - 1]);
        MEM_swap_2db (&oVal[i1], &oVal[i1 - 1]);
        goto L_sort0;
      }
    }



    // for(i1=0; i1<oNr; ++i1)
    // printf("  ATO_sort1 %d typ=%d tab=%f\n",i1,oTyp[i1],oVal[i1]);


  return 0;

}



//================================================================
  int ATO_srcTxt (int *atoTyp, double *atoVal, char *cmd) {
//================================================================
// ATO_srcTxt       get atomicObj from Typ_Txt (after AP_typ_srcExpr)
// Input:
//   txt      objectCodes/constantValue/constantObject (from AP_typ_srcExpr)
// Output:
//   atoTyp   Typ_cmdNCsub from objectCodes; Val= ?
//            Typ_Val from constantValue; Val=Value.
//            Typ_VC (from ed "DX") or Typ_PLN (from eg "RZ"); Val=DB-ind.

// see also APT_decode_ausdr1
// see also APED_dec_txt

  int    i1;

  // printf("ATO_srcTxt |%s|\n",cmd);


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


  //----------------------------------------------------------------
  // unknown word ..
    // *atoTyp = Typ_Error;
    *atoTyp = Typ_Txt;   // text, but not enclosed in "
    *atoVal = 0.;
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

    // printf("ex ATO_srcTxt typ=%d val%lf\n",*atoTyp,*atoVal);

  return 0;

}


//================================================================
  int ATO_swap (int aus_typ[], double aus_tab[], int i1, int i2) {
//================================================================
/// ATO_swap                swap 2 records
/// see also ATO_sort1 ATO_cpy_rec

    MEM_swap_int (&aus_typ[i2], &aus_typ[i1]);
    MEM_swap_2db (&aus_tab[i2], &aus_tab[i1]);

  return 0;

}


/*
//================================================================
  int ATO_del_nrec (ObjAto *ato, int ii) {
//================================================================
// delete record ii

  int     i2;

  printf("ATO_del_nrec %d\n",ii);

  i2 = ato->nr;
  MEM_del_nrec (&i2, &ato[0].typ,  ii, 1, sizeof(int));

  i2 = ato->nr;
  MEM_del_nrec (&i2, &ato[0].val,  ii, 1, sizeof(double));

  if(ato->ilev) {
    i2 = ato->nr;
    MEM_del_nrec (&i2, &ato[0].ilev,  ii, 1, sizeof(char));
  }

  // ato->nr -= 1;;

    ATO_dump__ (ato, " after _del_nrec");

  return 0;

}
*/


//================================================================
  int ATO_cpy_rec (ObjAto *ato, int iTo, int iFrom) {
//================================================================
// copy ATO-record iTo = iFrom, delete iFrom

  // printf("ATO_cpy_rec %d %d\n",iTo,iFrom);

  ato->typ[iTo] = ato->typ[iFrom];
  ato->val[iTo] = ato->val[iFrom];
  if(ato->ilev) {
    ato->ilev[iTo] = ato->ilev[iFrom];
  }

  ato->typ[iFrom] = Typ_NULL;

    // ATO_dump__ (ato, "ex ATO_cpy_rec");

  return 0;

}


//================================================================
  int ATO_clear_block (ObjAto *ato, int is, int bNr) {
//================================================================
/// \code
/// ATO_clear_block         delete block of records
/// Input:
///   is    index of 1. record to delete
///   bNr   nr of records to delete
/// \endcode


  int   rNr;


  // printf("ATO_clear_block is=%d bNr=%d nr=%d\n", is, bNr, ato->nr);


  // delete ato->typ-block
  rNr = ato->nr;
  MEM_del_nrec (&rNr, ato->typ, is, bNr, sizeof(int));


  // delete ato->val-block
  rNr = ato->nr;
  MEM_del_nrec (&rNr, ato->val, is, bNr, sizeof(double));


  // delete ato->ilev-block
  if(ato->ilev) {
    rNr = ato->nr;
    MEM_del_nrec (&rNr, ato->ilev, is, bNr, sizeof(short));
  } 


  // update ato->nr
  ato->nr -= bNr;

    // printf("ex clear_block nr=%d\n",ato->nr);

  return 0;

}


//================================================================
  int ATO_clean__ (ObjAto *ato) {
//================================================================
// close all gaps (typ == Typ_NULL); update ie


  int    i1, i2, ie;


  // ATO_dump__ (ato, "ATO_clean_1");

  ie = ato->nr;

  L_del_nxt:

  for(i1=0; i1 < ie; ++i1) {
    // find first gap
    if(ato->typ[i1] != Typ_NULL) continue;
    // find next non-gap
    for(i2=i1; i2 < ie; ++i2) {
      if(ato->typ[i2] == Typ_NULL) continue;
      // copy non-gap-record i2 over gap-record i1
      ATO_cpy_rec (ato, i1, i2);
      // if(irc) {TX_Error("ATO_clean__ E001"); return -1;}
      goto L_del_nxt;
    }
  }


  // set endIndex to last non-gap-record
  for(i1 = (ie - 1); i1 >= 0; --i1) {
    if(ato->typ[i1] == Typ_NULL) continue; 
    ie = i1 + 1;
    ato->nr = ie;
    break;
  }

    // printf(" is=%d ie=%d\n",*is,ie);
    // ATO_dump__ (ato, "ex ATO_clean_1");

  return 0;

}


//================================================================
  int ATO_pNr__ (int *iSta, ObjAto *ato) {
//================================================================
//  get vNr = nr of parameters following record iif
//  parameters must have same level

  int   iNr, is, ie, i1, ila;
  // char  ila;


  is = *iSta + 1;
  ie = ato->nr;

  ila = ato->ilev[is];

  // printf("ATO_pNr__ iSta=%d ila=%d\n",*iSta,ila);


  for(i1=is; i1<ie; ++i1) {
      // printf(" ATO_pNr__ i1=%d c(iv)=%d\n",i1,ato->ilev[i1]);
    if(ila == ato->ilev[i1]) continue;
    iNr = i1 - is;
    goto L_exit;
  }

  iNr = ie - is;

  L_exit:
  *iSta = is;
    // printf("ex ATO_pNr__ iNr=%d iSta=%d\n",iNr,*iSta);

  return iNr;

}


//================================================================
  int ATO_ato_eval_geom (ObjAto *ato, int iif) {
//================================================================
/// \code
/// evaluate geometrical functions (Typ_FncNam)
/// geom-func: P(2) L,C,D,R,S,U(121) ANG(137) X(134) Y,Z ..
/// Input:
///   iif      startindex in ato
/// Output:
///   ato      ?
///   Retcod:  0 = no geom.func; 1 = function evaluated; -1 = error
/// see APT_decode_func ../ci/NC_apt.c :16974
/// \endcode

  int       irc, i1, i2, typ, iv, iie, vNr, dNr, iNew;
  long      dbi;
  double    d1;
  Point     pt1;
  Line      ln1;
  Circ      ci1;
  // Vector    vc1;
  Plane     pl1;
  // ObjGX     ox1;
  // Memspc    workSeg;
  char      obj1[OBJ_SIZ_MAX], c1;


  typ = ato->val[iif];


  // printf("GGGGGGGGGGG  ATO_ato_eval_geom iif=%d typ=%d\n",iif,typ);
  // ATO_dump__ (ato, " before clean");
  // printf(" ato->typ[iif+1] = %d\n",ato->typ[iif+1]);


/*
  // skip math.bracket;  but not Typ_cmdNCsub
  // if(ato->typ[iif+1] > Typ_Val) return 0;
  // if(ato->typ[iif+1] > Typ_Val) {
  if(ato->typ[iif+1] > Typ_ZVal) {
    if(ato->typ[iif+1] != Typ_cmdNCsub) return 0;
  }
*/

  iv = iif;
  iie = ato->nr;
    // printf(" iv=%d iie=%d\n",iv,iie);



  //----------------------------------------------------------------
  //  get vNr = nr of parameters following record iif
  vNr = ATO_pNr__ (&iv, ato);
  if(vNr < 1) return -1;
    // printf(" vNr=%d\n",vNr);


  //----------------------------------------------------------------
  L_work:
      // printf("L_work: iv=%d iie=%d vNr=%d\n",iv,iie,vNr);


  switch (typ) {

    case Typ_PT:
      i1 = APT_decode_pt (&pt1, vNr, &ato->typ[iv], &ato->val[iv]);
      if(i1 < 0) goto ParErr;
      dbi = DB_StorePoint (-1L, &pt1);
      d1 = dbi;
      break;

    case Typ_VC:
      i1 = APT_decode_vc ((Vector*)obj1, vNr, &ato->typ[iv], &ato->val[iv]);
      if(i1 < 0) goto ParErr;
      dbi = DB_StoreVector (-1L, (Vector*)obj1);
      d1 = dbi;
      break;

    case Typ_LN:
      i1 = APT_decode_ln ((Line*)obj1, vNr, &ato->typ[iv], &ato->val[iv]);
      if(i1 < 0) goto ParErr;
      dbi = DB_StoreLine (-1L, (Line*)obj1);
      d1 = dbi;
      break;

    case Typ_CI:
      i1 = APT_decode_ci ((Circ*)obj1, vNr, &ato->typ[iv], &ato->val[iv]);
      if(i1 < 0) goto ParErr;
      dbi = DB_StoreCirc (-1L, (Circ*)obj1);
      d1 = dbi;
      break;

    case Typ_CV:
      i1 = APT_decode_cv ((ObjGX*)obj1, &iNew, vNr, &ato->typ[iv], &ato->val[iv]);
      if(i1 < 0) goto ParErr;
      dbi = DB_StoreCurv (-1L, (ObjGX*)obj1, iNew);
      d1 = dbi;
      break;

    case Typ_PLN:
      i1 = APT_decode_pln1 ((Plane*)obj1, vNr, &ato->typ[iv], &ato->val[iv]);
      if(i1 < 0) goto ParErr;
      dbi = DB_StoreRef (-1L, (Plane*)obj1);
      d1 = dbi;
      break;

    case Typ_Val:
      i1 = APT_decode_var (&d1, vNr, &ato->typ[iv], &ato->val[iv]);
      if(i1 < 0) goto ParErr;
      break;

    case Typ_Angle:
      i1 = APT_decode_angd__ (&d1, vNr, &ato->typ[iv], &ato->val[iv]);
      if(i1 < 0) goto ParErr;
      break;

    case Typ_XVal:       // 1 value: ATO_eval_fnc1__; here more than 1 value ..
    case Typ_YVal:
    case Typ_ZVal:
      // X(P#) Y(P#) Z(P#)
      i1 = APT_decode_xyzval (&d1, vNr, &ato->typ[iv], &ato->val[iv], &typ);
      if(i1 < 0) goto ParErr;
      break;

    case Typ_Group:  // U(objs)
      // change FncNam/Typ_Group into Typ_Group/objNr
      ato->typ[iif] = Typ_Group;
      ato->val[iif] = vNr;
      goto L_done;    // do not delete groupMembers


    case Typ_modif:  // MOD(#)
      typ = Typ_modif;
      d1 = ato->val[iv];
      break;

    default:
      TX_Error("ATO_ato_eval_geom E001 %d",typ);
      return -1;
  }


  //----------------------------------------------------------------
  // save func as db-obj
  ato->typ[iif] = typ;
  ato->val[iif] = d1;


  // kill all records for this expression
  ATO_clear_block (ato, iv, vNr);

  // done
  L_done:
  
    // ATO_dump__ (ato, " ex-ATO_ato_eval_geom");
 
  return 1;


  //----------------------------------------------------------------
  ParErr:
    TX_Error("Parameter: %s ATO_ato_eval_geom",AP_src_typ__(typ));
    return -1;

}


//================================================================
  int ATO_ato_eval__ (ObjAto *ato) {
//================================================================
/// \code
/// evaluate atomic-objects (compute); reduce records.
/// math-func: SQRT(290) SIN...
/// geom-func: P(2) L,C,D,R,S,U(121) ANG(137) X(134) Y,Z
/// loop tru all levels(highest first), evaluate ..
/// Record inaktiv setzen (Typ=Typ_NULL); ganz am Ende kompaktieren.
/// Input, Output: ato.
/// see APT_decode_comp1
/// \endcode

  int     ii, io, i1, i2, i3, ie, imod;
  long    l1;
  double  d1;


  // ATO_dump__ (ato, " start ATO_ato_eval__");
  // printf(" ilev=%p\n",ato->ilev);


  if(!ato->ilev) {TX_Error("ATO_ato_eval__ E001"); return -1;}

  ie = ato->nr;



  //----------------------------------------------------------------
  // evaluate variables;
  imod = 0;
  for(ii=0; ii<ie; ++ii) {
    if(ato->typ[ii] != Typ_VAR) continue;
    APT_decode_var (&d1, 1, &ato->typ[ii],  &ato->val[ii]);
    ato->typ[ii] = Typ_Val;
    ato->val[ii] = d1;
  }

  if(imod) ATO_clean__ (ato);

  if(ato->nr < 2) goto L_exit;

    // ATO_dump__ (ato, "after eval_vars");


  //----------------------------------------------------------------
  // evaluate math.operators (+-*/)
  // 3 records Typ_Val/MathOperator/Typ_Val into 1 record Typ_Val.
  L_mathOpe:
  i1 = ATO_eval_ope__ (ato);



  //----------------------------------------------------------------
  L_fnc0:
  // eval math-functions with only one value  (and geom-func-VAL)
  i2 = ATO_eval_fnc1__ (ato);
  if(i2 < 0) return i2;         // 2070-01-27
  if((i1 > 0) || (i2 > 0)) {
    // remove all NULL-records
    ATO_clean__ (ato);
    goto L_mathOpe;
  }



  //----------------------------------------------------------------
  // decode OpmMinus followed by Val
  L_ope_2:
    // ATO_dump__ (ato, " L_ope_2:");
/*
    ie = ato->nr - 1;
    for(ii=0; ii<ie; ++ii) {
      if(ato->typ[ii] != TYP_OpmMinus) continue;
      i1 = ii + 1;
      if(ato->typ[i1] != Typ_Val) continue;
        printf(" ato_eval-ope_2 %d %d\n",ii,i1);
      // change i1 -> (val *= -1); delete ii.
      ato->val[i1] *= -1.;
      ato->typ[ii] = Typ_NULL;
      ie = ato->nr;
      i1 = 0;
      ATO_clean_1 (&i1, &ato->nr, ato);  // remove gaps with typ=Typ_NULL
      goto L_ope_2;
    }
*/


  //----------------------------------------------------------------
  // evaluate geometrical functions (Typ_FncNam)
  L_geom_0:
    // ATO_dump__ (ato, " L_geom_0:");
  imod = 0;
  ie = ato->nr - 1;
    // printf(" L_geom_0: ie=%d\n",ie);
    // ATO_dump__ (ato, "before L_geom_0");
  for(ii=ie; ii>=0; --ii) {
    if(ii >= ato->nr) continue;
    if(ato->typ[ii] != Typ_FncNam) continue;
    i1 = ATO_ato_eval_geom (ato, ii);
      // printf(" i1 ex eval_geom = %d\n",i1);
    if(i1 < 0) goto L_comp_0;
    if(i1 > 0) ++imod;
  }
  if(imod) goto L_geom_0;


  //----------------------------------------------------------------
  // compress ato (remove all Typ_NULL-records)
  L_comp_0:
    // ATO_dump__ (ato, " L_comp_0");
    // printf(" L_comp_0:\n");

  ii = -1;
  io = 0;   
  ie = ato->nr;

  L_comp_nxt:
    ++ii;
    if(ii >= ie) {
      ato->nr = io;
      // ato->ilev = NULL;   // 2014-12-18 (error in ATO_ato_eval__)
      goto L_exit;
    }
    if(ato->typ[ii] == Typ_NULL) goto L_comp_nxt;
    if(ii > io) {
      ato->typ[io] = ato->typ[ii];
      ato->val[io] = ato->val[ii];
    }
    ++io;
    goto L_comp_nxt;


  //----------------------------------------------------------------
  L_exit:
    // printf(" ex-ATO_ato_eval__\n");
    // ATO_dump__ (ato, "ex-ATO_ato_eval__");

  // test for unknown types
/* removed for processCommand "TEC val val val .."
  if(ato->nr >= 2) {
    if((ato->typ[0] == Typ_Val) && (ato->typ[1] == Typ_Val))  // 2020-01-04
      TX_Print ("**** Line %d - change val val . to P(val val .) or D(...)",
                ED_get_lnr_act());
// TODO: lineNr wrong if in subModel !
  }
*/

  return 0;

}


//================================================================
  int ATO_eval_ope__ (ObjAto *ato) {
//================================================================
/// \code
/// evaluate math.operators (+-*/)
/// 3 records Typ_Val/MathOperator/Typ_Val into 1 record Typ_Val.
/// Retcode:  nr of evaluations
/// \endcode


  int      imod, ii, i1, i2, i3, is, ie, ila;
  // char     c1;


  // ATO_dump__ (ato, "ATO_eval_ope__ start");

  imod = 0;

  if(ato->nr < 3) goto L_exit;

  ie = ato->nr;
  ii = 0;


  // get first record as i1; must be Typ_Val
  L_nxt:
  is = ii;
  i1 = -1;
  for(ii=is; ii<ie; ++ii) {
    if(ato->typ[ii] != Typ_Val) continue;
    i1 = ii;
    ila = ato->ilev[i1];
    break;
  }
  if(i1 < 0) goto L_exit;
    // printf(" _ope_-i1=%d ila=%d\n",i1,ila);


  // get 2. record as i2; must be operator (TYP_IS_OPM)
  i2 = -1;
  for(ii=i1+1; ii<ie; ++ii) {
    if(ato->typ[ii] == Typ_NULL) continue;
    if(ato->ilev[ii] != ila) goto L_nxt;
    if(!TYP_IS_OPM(ato->typ[ii])) goto L_nxt;
    i2 = ii;
    break;
  }
  if(i2 < 0) goto L_exit;
    // printf(" _ope_-i2=%d\n",i2);



  // get 3. record as i3; must be value
  i3 = -1;
  for(ii=i1+1; ii<ie; ++ii) {
    if(ato->typ[ii] == Typ_NULL) continue;
    if(ato->ilev[ii] != ila) goto L_nxt;
    if(ato->typ[ii] != Typ_Val) continue;
    i3 = ii;
    break;
  }
  if(i3 < 0) goto L_exit;
    // printf(" _ope_-i3=%d\n",i3);


  //----------------------------------------------------------------
  // found val-ope-val; work.
  // change ato->val[i1];
  switch (ato->typ[i2]) {
    case TYP_OpmPlus:
      ato->val[i1] += ato->val[i3];
      break;
    case TYP_OpmMinus:
      ato->val[i1] -= ato->val[i3];
      break;
    case TYP_OpmMult:
      ato->val[i1] *= ato->val[i3];
      break;
    case TYP_OpmDiv:
      ato->val[i1] /= ato->val[i3];
      break;
    case TYP_OpmPow:
      ato->val[i1] = pow(ato->val[i1], ato->val[i3]);
  }
    // printf(" comp %lf %lf ope %d\n",ato->val[i1],ato->val[i3],ato->typ[i2]);

  ato->typ[i2] = Typ_NULL;
  ato->typ[i3] = Typ_NULL;
  ++imod;

  is = i3 + 1;
  if(is < ie) goto L_nxt;



  //----------------------------------------------------------------
  L_exit:
  return imod;

}


//================================================================
  int ATO_eval_fnc1__ (ObjAto *ato) {
//================================================================
// eval math-functions with only one value  (and geom-func-VAL)
// 2 Records: Typ_FncNam,Typ_Val;Typ_Val into 1 record Typ_Val.

  int      imod, ii, i1, i2, i3, is, ie, ix, ila;
  // char     c1;
  double   d1;


  // ATO_dump__ (ato, "ATO_eval_fnc1__ start");

    
  imod = 0;
    
  if(ato->nr < 2) goto L_exit;

  ie = ato->nr;
  ii = 0;


  // get first record as i1; must be Typ_FncNam
  L_nxt:
  is = ii;
  i1 = -1;
  for(ii=is; ii<ie; ++ii) {
    if(ato->typ[ii] != Typ_FncNam) continue;
    i1 = ii;
    // get ila = level of following data
    ila = ato->ilev[i1 + 1];
    break;
  }
  if(i1 < 0) goto L_exit;
    // printf(" _fnc1_-i1=%d ila=%d\n",i1,ila);


  // get 2. record as i2; must be Typ_Val
  i2 = -1;
  for(ii=i1+1; ii<ie; ++ii) {
    if(ato->typ[ii] == Typ_NULL) continue;
    if(ato->ilev[ii] != ila) goto L_nxt;
    if(ato->typ[ii] != Typ_Val) goto L_nxt;
    i2 = ii;
    break;
  }
  if(i2 < 0) goto L_exit;
    // printf(" _fnc1_-i2=%d\n",i2);


  // record following i2 must not be same level ..
  is = i2 + 1;
  for(ii=is; ii<ie; ++ii) {
    if(ato->typ[ii] == Typ_NULL) continue;
    if(ato->ilev[ii] == ila) goto L_nxt;
  }


  //----------------------------------------------------------------
  // found func [i1] with single value [i2]; work:
  // set [i1] = value(<func>([i2]));   set [i2] = NULL

      ii = ato->val[i1];   // type of function; see APT_decode_Fmc
      switch (ii) {

        case Typ_Val:
          d1 = ato->val[i2];
          break;
        case Typ_FcmSQRT:
          d1 = sqrt(ato->val[i2]);
          break;
        case Typ_FcmSIN:
          d1 = sin(ato->val[i2]);
          break;
        case Typ_FcmCOS:
          d1 = cos(ato->val[i2]);
          break;
        case Typ_FcmTAN:
          d1 = tan(ato->val[i2]);
          break;
        case Typ_FcmASIN:
          d1 = asin(ato->val[i2]);
          break;
        case Typ_FcmACOS:
          d1 = acos(ato->val[i2]);
          break;
        case Typ_FcmATAN:
          d1 = atan(ato->val[i2]);
          break;
        case Typ_FcmABS:
          d1 = fabs(ato->val[i2]);
          break;
        case Typ_FcmFIX:
          ix = ato->val[i2];
          d1 = ix;
          break;
        case Typ_FcmRND:
          d1 = 0.5;
          if(ato->val[i2] < 0.0) d1 = -0.5;
          ix = (int)(ato->val[i2] + d1);
          d1 = ix;
          break;

        case Typ_Angle:
          ato->typ[i1] = Typ_Angle;
          d1  = ato->val[i2];
          goto L_sav1;

        case Typ_modif:
          ato->typ[i1] = Typ_modif;
          d1  = ato->val[i2];
          goto L_sav1;

        case Typ_XVal:
          ato->typ[i1] = Typ_XVal;
          d1  = ato->val[i2];
          goto L_sav1;
        case Typ_YVal:
          ato->typ[i1] = Typ_YVal;
          d1  = ato->val[i2];
          goto L_sav1;
        case Typ_ZVal:
          ato->typ[i1] = Typ_ZVal;
          d1  = ato->val[i2];
          goto L_sav1;

        case Typ_PTS:
          ato->typ[i1] = Typ_PTS;
          d1  = ato->val[i2];
          goto L_sav1;
        case Typ_PTI:
          ato->typ[i1] = Typ_PTI;
          d1  = ato->val[i2];
          goto L_sav1;
        case Typ_SEG:
          ato->typ[i1] = Typ_SEG;
          d1  = ato->val[i2];
          goto L_sav1;

        default:
          TX_Error("ATO_eval_fnc1__ E002 %d",ii);
          return -1;
      }


    // change record i1 into value (preserves ival !), delete record i2.
      // printf(" L_fncSav: i1=%d i2=%d\n",i1,i2);
    ato->typ[i1] = Typ_Val;

  L_sav1:
    // set value of active record i1
    ato->val[i1] = d1;
    // delete record i2.
    ato->typ[i2] = Typ_NULL;
    ++imod;

    if(ie > is) goto L_nxt;



  //----------------------------------------------------------------
  L_exit:
  return imod;

}


//================================================================
   int ATO_ato_txo__ (ObjAto *ato, ObjTXTSRC *tso, char *sl) {
//================================================================
/// \code
/// get atomic-objects from source-objects
/// Input:
///   tso
///   sl        srcTxt
/// Output:
///   ato
/// \endcode

// see APT_decode_Opm

  int     ii, iTyp, oTyp, its, sPos, sLen, lev=0;
  long    dbi;
  double  d1;
  char    *p1, *p2, s1[80];


  // printf("ATO_ato_txo__ |%s|\n",sl);
  // APED_txo_dump (tso, sl, "start ATO_ato_txo__");


  its = 0;

  for(;;) {
    iTyp = tso[its].typ;

    if(iTyp == TYP_FuncEnd) break;

    sPos = tso[its].ioff;
        // printf("ato_txo__-nxt [%d] iTyp=%d |",its,iTyp);
               // UTX_dump_cnl(&sl[sPos],50);printf("|\n");


    //----------------------------------------------------------------
    if(iTyp == Typ_ObjDB) {
      // decode typ
      oTyp = AP_typ_typChar (toupper(sl[sPos]));
      // decode dbi
      ++sPos;
      d1 = strtod (&sl[sPos], &p1);   // d1 = dbi
        // printf(" _txo_ typ=%d dbi=%lf sPos=%d\n",oTyp,d1,sPos);

      // for Typ_VAR only:
      if(oTyp == Typ_VAR) {
        d1 = DB_GetVar ((long)d1);
        oTyp = Typ_Val;
        if(sPos > 2) {
          // if(('-' before 'V') and (deli before '-')) 
            // printf(" before=|%c|%c|\n",sl[sPos-3],sl[sPos-2]);
          if(sl[sPos-2] == '-') {
            if(!UTX_ckc_Del1(sl[sPos-3])) {
                // printf(" si_deli_yes: |%c|\n",sl[sPos-3]);
              // get value *= -1 and ignore '-'
              d1 *= -1.;
              ato->nr -= 1;
            }
          }
        }
      }


      // eval Vars
      ATO_ato_expr_add (ato, oTyp, d1, tso[its].ipar);


    //----------------------------------------------------------------
    } else if(iTyp == Typ_NumString) {
      // d1 = decode value
      d1 = strtod (&sl[sPos], &p1);
      oTyp = Typ_Val;
      ATO_ato_expr_add (ato, oTyp, d1, tso[its].ipar);


    //----------------------------------------------------------------
    } else if(iTyp == Typ_ConstVal) {
      sLen = tso[its].ilen;
      strncpy(s1, &sl[sPos], sLen);
      s1[sLen] = '\0';
       // decode constVal -> d1
      // ATO_srcTxt (&oTyp, &d1, s1);
      oTyp = Typ_Val;
      d1 = NcoValTab[(int)tso[its].form];
      ATO_ato_expr_add (ato, oTyp, d1, tso[its].ipar);


    //----------------------------------------------------------------
    // test for geometric-constant-object; eg DX RZ
    } else if(iTyp == Typ_ConstOG) {
      ii = tso[its].form;
      APED_dbi_src_std_vc_pl (&oTyp, &dbi, ii);
      d1 = dbi;
      ATO_ato_expr_add (ato, oTyp, d1, tso[its].ipar);


    //----------------------------------------------------------------
    // copy ..
    } else if((iTyp == Typ_ope__)    ||
              (iTyp == Typ_cmdNCsub) ||
              (iTyp == Typ_FncNam))      {
      oTyp = iTyp;
      d1 = tso[its].form;
      ATO_ato_expr_add (ato, oTyp, d1, tso[its].ipar);

/*
    //----------------------------------------------------------------
    } else if(iTyp == Typ_FncNam) {
      oTyp = iTyp;
      d1 = tso[its].form;
      ATO_ato_expr_add (ato, oTyp, d1, tso[its].ipar);
*/

    //----------------------------------------------------------------
    // test for strings
    } else if(iTyp == Typ_String) {
      d1 = tso[its].ioff;    // offset (startPos of '"')
      // including '"', 
      ATO_ato_expr_add (ato, iTyp, d1, tso[its].ipar);


    //----------------------------------------------------------------
    // test for CObj-name; eg " # sum"; skip record      2017-04-19
    } else if(iTyp == Typ_Name) {  // 156


    //----------------------------------------------------------------
    } else if(iTyp >= TYP_EventEnter) {  // 400
      TX_Print("***** ATO_ato_txo__ E002 Record %d", its);
      return -1;

    //----------------------------------------------------------------
    // test for math-operator, math-func; copy record.  
    } else if(iTyp >= Typ_FcmSQRT) {     // 290
      oTyp = iTyp;
      ATO_ato_expr_add (ato, oTyp, 0., tso[its].ipar);
// see APT_decode_Fmc


    //----------------------------------------------------------------
    // test for values; copy record.
    } else if((iTyp >= Typ_Val)&&(iTyp < Typ_Typ)) {
      oTyp = iTyp;
      ATO_ato_expr_add (ato, oTyp, 0., tso[its].ipar);


    //----------------------------------------------------------------
    // test for math-operator, math-func; copy record.
    } else if(iTyp >= TYP_OpmPlus) {          // 280-TYP_EventEnter
      oTyp = iTyp + tso[its].form;
      ATO_ato_expr_add (ato, oTyp, 0., tso[its].ipar);
// see APT_decode_Opm


    //----------------------------------------------------------------
    // unknown type iTyp
    } else {
      TX_Print("***** ATO_ato_txo__ E001 Record %d type %d", its, iTyp);
      return -1;
    }

    ++its;
  }


    // TESTBLOCK
    // ATO_dump__ (ato, "------ ex ATO_ato_txo__:");
    // END TESTBLOCK

  return 0;

}


//================================================================
  int ATO_ato_srcLn__ (ObjAto *ato, char *srcLn) {
//================================================================
/// \code
/// get atomicObjects from sourceLine; full evaluated.
///   (provides dynamic-DB-obj's for geometric expressions)
/// must provide memspc before with ATO_getSpc__
/// Input:
///   srcLn      eg "D(0 0 1)"      // may not have objNames !
///   ato        free space for atomic-objs
/// Output:
///   ato        eg ato->nr=1; ato->typ[0]=Typ_VC; ato->val[0]=-2.;
///   RetCod     0=OK, -1=Err
/// Example:
///  ATO_getSpc__ (&ato);
///  i1 = ATO_ato_srcLn__ (&ato, s1);
///    ATO_dump__ (&ato, " after _ato_srcLn__");
/// \endcode


  int       irc, itsMax;
  ObjTXTSRC *tso;

 
  // printf("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS\n");
  // printf("ATO_ato_srcLn__ |%s|\n",srcLn);
  // printf(" ilev=%p\n",ato->ilev);



  // get temp-space for tso
  itsMax = SRCU_tsMax (srcLn);   // printf(" itsMax=%d\n",itsMax);
  tso = MEM_alloc_tmp ((int)(itsMax * sizeof(ObjTXTSRC)));
  if(tso == NULL) {TX_Print("ATO_ato_srcLn__ EOM"); return -1;}


  // analyze sourceline; get source-objects -> tso
  irc = APED_txo_srcLn__ (tso, itsMax, srcLn);
    // printf(" _txo_srcLn__ %d\n",irc);
  if(irc < 1) return irc;
    // APED_txo_dump (tso, srcLn, "nach-txo_srcLn__");


  // get atomic-objects from source-objects
  irc = ATO_ato_txo__ (ato, tso, srcLn);
  if(irc < 0) return irc;
    // ATO_dump__ (ato, " nach _ato_txo_");


  // evaluate atomic-objects (compute)
  return (ATO_ato_eval__ (ato));

}


//================================================================
  int ATO_ato_obj_pt (ObjAto *ato, int oTyp, int iseg,
                         int iTyp, void *iObj, Point *ptx) {
//================================================================
// ATO_ato_obj_pt          get all atomicObjs for selection
//
// Input:
//   ptx       select-position
//   iTyp,iObj    typ/object of selected-obj
//   oTyp    Typ_PT - "P(..)"
//             Typ_VC - "D(..)"
//             Typ_LN - "L(..)"
//             0 (Typ_Error) gives eg "P#"
//             Typ_goGeo1 gives L|C (== typ)
//   iseg      0    do not output segmentNr (basic-curve selected)
//             >0   output iseg as SEG(<iseg>) (segment of CCV selected)
// Output:
//   ato       all atomicObjs for definition of selected obj are added;
//               the atomicObjs form an objID of type <oTyp>
//   retCod:   0     OK, ptx is on iObj; parameters are in ato
//             -1    ptx not on iObj (iTyp,iObj)
//             -2    ptx is on iObj, but no conversion possible
//             -9    internal error;
//
// type of output can be:
//   oTyp:
//   Typ_PT     (parametric position on selected curve); P(L20 0.456)
//   Typ_VC     (direction of selected position)         D(S20 MOD(2))
//   Typ_LN     from C: tangent                          L(C# par) 
//   Typ_LN     from S-POL: segment                      L(S# MOD(segNr)) 
//   Typ_LN     from CCV-L: [segment]                    L(S# [MOD(segNr)]) 
//   Typ_goGeo1 LN/CI/CV                                 S(S20 MOD(1))
//
//  L(S#)
//  D|L|C|S(basecurve segNr)            on polygon|CCV
//  D|L(basecurve segNr segNr)          on polygon in CCV
//
//  D|P(basecurve par)                  on simple-cv
//  D|P(basecurve segNr par)            on CCV
//                       


  int       irc, is, ptNr, ptMax;
  double    pars;
  Point     pts, *pta;


  //----------------------------------------------------------------
  // printf("ATO_ato_obj_pt iseg=%d oTyp=%d iTyp=%d\n",iseg,oTyp,iTyp);
  // ATO_dump__ (ato, " _obj_pt-in");
  // DEB_dump_obj__ (iTyp, iObj, " ato_obj_pt-iObj-in");
  // DEB_dump_obj__ (Typ_PT, ptx, " ato_obj_pt-ptx-in");



  //----------------------------------------------------------------
  // get pta = polygon for obj (iTyp,iObj)

  // get nr of points
  ptMax = UT3D_ptNr_obj (iTyp, iObj, UT_DISP_cv);
  if(ptMax < 1) return -1;
    // printf(" ptMax=%d\n",ptMax);

  ptMax += 16;

  // get tempSpc for points
  pta = (Point*)MEM_alloc_tmp((int)(sizeof(Point) * ptMax));


  // get polygon from obj iTyp,iObj
// TODO: get polygon of PRCV if exists
  ptNr = 0;
  irc = UT3D_npt_obj (&ptNr, pta, ptMax, iTyp, iObj, 1, UT_DISP_cv, 2);
  if(irc< 0) {TX_Error("ATO_ato_obj_pt E001"); goto L_err2;}
    // printf(" _obj_pt-ptNr=%d\n",ptNr);



  //----------------------------------------------------------------
  // test all segments of polygon; return selPt, point# and offsetValue
  // get is = index of selected segment; 0 = first ..
  irc = GR_pt_par_sel_npt (&pts, &is, &pars, pta, ptNr, ptx);
    // printf(" f-par_sel_npt irc=%d is=%d pars=%lf\n",irc,is,pars);
  if(irc < 0) goto L_err1;  // -1 = ptx not on iObj; exit ..
    // ATO_dump__ (ato, " ato_obj_pt-obj-added");


  //----------------------------------------------------------------
  // skip impossible combinations (obj already in ato, must clear)
  // circ only from circ
  if((iTyp != Typ_CI) && (oTyp == Typ_CI)) goto L_noObj;
     // (iTyp == Typ_CVBSP) ||
     // (iTyp == Typ_CVPOL))   {
    // if(oTyp == Typ_CI) goto L_noObj;
  // }

  //----------------------------------------------------------------
  // selected point, segment#, parameter found.

  // add primary segment-nr if more > 1 segments exist
  if(iseg > 0) {
      // printf(" add iseg %d\n",iseg);
    ATO_ato_expr_add (ato, Typ_SEG, (double)iseg, 0);    // 2019-10-10 was MOD
      // ATO_dump__ (ato, " ato_obj_pt-modif-added");
  }


  // Typ_goGeo1 -------------------------------------------------------------
  // add secondary segment-nr for polygon in CCV
  // L|C|S(basecurve                   on polygon|CCV
  if(oTyp == Typ_goGeo1) {   // S|C|L
    if(iTyp == Typ_CVPOL) {   // skip subSegNrs for line & circ in CCV
      is += 1; // is = index -> segNr
      // if endpoint was selected:
      if(is >= ptNr) is = ptNr - 1;
      goto L_add_mod;
    }
    goto L_exit;
  }


  // C -------------------------------------------------------------
  if(oTyp == Typ_CI) {
    // C() wanted; 
    if(iTyp == Typ_CI) goto L_exit; // C(S MOD) - no parameter
  }

  // D -------------------------------------------------------------
  if(oTyp == Typ_VC) {
    // D() wanted
    if(iTyp == Typ_LN) goto L_exit; // D(L) - no segNr
    if(iTyp == Typ_CVPOL) {
      //  D(basecurve segNr segNr)          on polygon in CCV
      is += 1; // is = index -> segNr
      // if endpoint was selected:
      if(is >= ptNr) is = ptNr - 1;
      goto L_add_SEG;   // ?
    }
    // if(iTyp != TypCI) goto L_exit;           // D from C via tangent
  }



  // L -------------------------------------------------------------
  if(oTyp == Typ_LN) {
    if(iTyp == Typ_LN) goto L_exit; // L(L) - no segNr, no parameters
    if(iTyp == Typ_CVPOL) {
      // L(basecurve segNr segNr)          on polygon in CCV
      is += 1; // is = index -> segNr
      // if endpoint was selected:
      if(is >= ptNr) is = ptNr - 1;
      goto L_add_SEG;
    }
    // if(iTyp != TypCI) goto L_exit;       // L(C) - no segNr, no par
  }


  // S -------------------------------------------------------------
  if(oTyp == Typ_CV) {
    goto L_exit;
  }


  // P -------------------------------------------------------------
  // point P wanted:   parameter (value) or pointIndex (MOD(ptNr))
  // get characteristic point (first or last point or point of plg)
  if(irc == 0) {     
    // retCode of GR_pt_par_sel_npt: 0=point selected
    if(iTyp == Typ_CVPOL) {
      if(is == 0) {
        // startpt of polygon selected; out PTS(1)
        is = Ptyp_start;
        goto L_add_PTS;
      } else if(is == ptNr - 1) {
        // yes, endPt selected, out PTS(2)
        is = Ptyp_end;
        goto L_add_PTS;
      }
      ++is;    // is = index of point
      goto L_add_PTI;
    }

    // eg line, circ, bspl ..
    if(is == 0) {
      // startpoint selected
      is = 1;
      goto L_add_PTS;

    } else if(is == (ptNr - 1)) {
      // endpoint selected
      is = 2;
      goto L_add_PTS;
    }
  }


  // point: no characteristic point selected; parameter necessary ...
  // get parameter from selected point; always 0-1 = vTyp=1 (see INF_struct_par)
  irc = UT3D_par_pt__pt_prj_cv (&pars, NULL, 1, 0, &pts, iTyp, iObj, UT_DISP_cv);
  if(irc < 0) goto L_err2; // ?? {TX_Error("ATO_ato_obj_pt E002"); goto L_err2;}

  // add parameter to atomicObjs
  ATO_ato_expr_add (ato, Typ_Val, pars, 0);

  goto L_exit;


  //----------------------------------------------------------------
  L_add_mod:
    ATO_ato_expr_add (ato, Typ_modif, (double)is, 0);
    goto L_exit;

  //----------------------------------------------------------------
  L_add_PTS:    // add StandardPointIndex; see Ptyp_start,Ptyp_end ..
    ATO_ato_expr_add (ato, Typ_PTS, (double)is, 0);
    goto L_exit;

  //----------------------------------------------------------------
  L_add_PTI:     // pointIndex (controlpoint ..)
    ATO_ato_expr_add (ato, Typ_PTI, (double)is, 0);
    goto L_exit;

  //----------------------------------------------------------------
  L_add_SEG:     // segmentNr
    ATO_ato_expr_add (ato, Typ_SEG, (double)is, 0);
    // goto L_exit;



  //----------------------------------------------------------------
  L_exit:
    // ATO_dump__ (ato, "ex-ATO_ato_obj_pt");
  return 0;


  //----------------------------------------------------------------
  L_noObj:
    // clear ato
    ATO_clear__ (ato);
    return -2;

  //----------------------------------------------------------------
  L_err1:
    return -1;

  L_err2:
    return -9;

}


//================================================================
  int ATO_parents__ (MemTab(ObjSRC) *mtPar, ObjAto *ato) {
//================================================================
// ATO_parents__           get all parents of obj (index into ato)

  int      irc, i1, pNr=0;
  long     ld;
  ObjSRC   odb1 = _OSRC_NUL;


  // ATO_dump__ (ato, "ATO_parents__");

  for(i1=0; i1 < ato->nr; ++i1) {
    // skip all non-geometric-objects
    if(ato->typ[i1] >= Typ_Val) continue;
    // skip all dynamic objects
    if(ato->val[i1] <= 0.) continue;

    // have explizit (with values) definend geometric obj
    odb1.typ = ato->typ[i1];
    odb1.dbi = ato->val[i1];

    // get the dispListIndex of parent
    odb1.dli = DL_dli__dbo ((int)odb1.typ, odb1.dbi, -1L);

    // add dbo (parent) to mtPar
    irc = MemTab_add (mtPar, &ld, &odb1, 1, 0);
    if(irc < 0) return irc;
  }


    // TESTBLOCK
    // MemTab_dump (mtPar, " ex-ATO_parents__ ");
    // DL_DumpObjTab (" ex-ATO_parents__ ");
    // END TESTBLOCK
    

  return pNr;

}


// EOF
