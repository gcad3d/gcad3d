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

ATO_ato_srcLn_exp       split sourceLine, add all expressions to ato
ATO_ato_atoTab__        decode all atomicObjects in a single ato
ATO_dump__              dump ato-table
ATO_ato_expr__          resolv expression and add it to ato-table
ATO_ato_expr_add        add ato to ato-table
ATO_swap                swap 2 records
ATO_clear__             delete all records
ATO_clean__             make clean atomicObjects from ato (remove FncNam, NULL )
ATO_clean_1             close all gaps (typ == Typ_NULL)
ATO_cpy_rec             copy ATO-record iTo = iFrom, delete iFrom

ATO_ato_srcLn__         get atomicObjects from sourceLine; full evaluated.
ATO_ato_txo__           get atomic-objects from source-objects
ATO_ato_typTab          get atomicObjects from aus_typ/aus_tab (old version)
ATO_ato_eval__          evaluate atomic-objects (compute); reduce records.
ATO_ato_eval_geom       evaluate geometrical functions
ATO_ato_obj_pt          get parametric position of point on obj
ATO_srcTxt              get atomicObj from Typ_Txt (after AP_typ_srcExpr)

ATO_getSpc_tmp__        get memspace for atomicObjects
ATO_getSpc__     get memSpc for atomicObjects - struct
ATO_getSpc1      get memSpc for atomicObjects - simple

List_functions_end:
=====================================================

\endcode *//*----------------------------------------




ato.ilev (char *ilev) = array of characters, used as ints (-100 - +100)
/// ilev       level; -1=primary level. NULL=unused



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

#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_gtypes.h"           // AP_src_typ__
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_txTab.h"            // TxtTab
#include "../ut/ut_memTab.h"           // MemTab
#include "../ut/ut_BitTab.h"           // BitTab

#include "../db/ut_DB.h"               // DB_VCX_IND DB_PLX_IND

#include "../xa/xa_mem.h"              // memspc..
#include "../xa/xa_ato.h"              // ATO_getSpc_tmp__
#include "../xa/xa_sele.h"             // Typ_go*

#include "../gui/gui__.h"              // GUI_* TYP_EventEnter



//================================================================
// EXTERN SYMBOLS:

// ex ../ut/ut_gtypes.c
extern char *ObjCodTab[];





//================================================================
// PROTOTYPES:
  int ATO_ato_srcLn_exp (ObjAto *ato, int *typ, double *val, char *srcLn);
  int ATO_ato_expr_add (ObjAto *ato, int typ, double val, int lev);
  int ATO_dump__ (ObjAto *ato, char *txt);



//================================================================
  int ATO_getSpc_tmpSiz (int sizTab) {
//================================================================
/// compute necessary space for type/value-table

  // printf("ATO_getSpc_tmpSiz siz=%d\n",sizTab);

  return (sizeof(int) * sizTab) + (sizeof(double) * sizTab);
}


//================================================================
  void ATO_getSpc_tmp1 (ObjAto *ato, int sizTab) {
//================================================================
/// aux.Func for ATO_getSpc_tmp__

  char  *p1;

  // printf("ATO_getSpc_tmp1 %d\n",sizTab);

  p1 = (char*)ato->typ;
  p1 += sizeof(int) * sizTab;

  ato->val = (double*)p1;

  ato->siz   = sizTab;
  ato->nr    = 0;

  ato->ilev  = NULL;
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
// get ato from func ..  see also APT_decode_pt
// decode all atomicObjects in ato, return single atomicObject (aTyp,aVal).
// returns type & value

  int    irc;
  long   dbi;
  char   o1[OBJ_SIZ_MAX];
  void   *vp1;


  // printf("ATO_ato_atoTab__ typ=%d val=%lf\n",*aTyp,*aVal);
  // ATO_dump__ (ato);


  //----------------------------------------------------------------
  // check / evaluate math.expressions; eg "RAD_180 * 2"
  // VAL()
  if(*aTyp == Typ_Val) {
    irc = APT_decode_Opm (aVal, ato);
    if(irc < 0) {TX_Error("ATO_ato_atoTab__ Opm1"); return -1;}
    *aTyp = Typ_Val;
    goto L_exit;

  // MOD()
  } else if(*aTyp == Typ_modif) {
    irc = APT_decode_Opm (aVal, ato);
    if(irc < 0) {TX_Error("ATO_ato_atoTab__ Opm2"); return -1;}
    irc = *aVal - 1;
    *aVal = irc;
    *aTyp = Typ_modif;
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
  // execute function - get binaryObject
  irc = APT_obj_ato ((void*)o1, *aTyp, ato);
  if(irc < 0) {TX_Error("ATO_ato_atoTab__ E001"); return -1;}
    // UT3D_stru_dump (*aTyp, o1, "  o1:");



  // store binaryObject temporary
  // see DB_StorePoint < APT_store_obj  APT_decode_func APT_store_obj
  dbi = -1L;
  irc = DB_store_stru (&vp1, *aTyp, *aTyp, o1, 1, &dbi);
  if(irc < 0) {TX_Error("ATO_ato_atoTab__ E002"); return -1;}
  *aVal = dbi;


  L_exit:
    // printf("ex ATO_ato_atoTab__ %d %lf\n",*aTyp,*aVal);

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
      default:
        TX_Error("ATO_ato_expr__ E002"); return -1;
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
    s1 = UME_alloc_tmp (sLen + 1);
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
    ATO_ato_srcLn_exp (ato, &eTyp, &eVal, cpos);
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
    if(irc < 0) {TX_Print("TX_Error E004"); return -1;}



  //----------------------------------------------------------------
  L_exit:
  return 0;

}


//====================================================================
  int ATO_ato_expr_add (ObjAto *ato, int typ, double val, int lev) {
//====================================================================
/// \code
/// add ato to ato-table
/// Example: ATO_ato_expr_add (&ato, Typ_PT, 20., 0);
/// \endcode

  // printf("ATO_ato_expr_add typ=%d val=%lf\n",*typ,*val);
  // ATO_dump__ (ato);


  if(ato->nr >= ato->siz) return -1;

  ato->typ[ato->nr] = typ;
  ato->val[ato->nr] = val;

  if(ato->ilev) {
    if(ato->nr > 1000) {
      TX_Error ("ATO_ato_expr_add EOM ilev");
      return -1;
    }
    ato->ilev[ato->nr] = lev;   // ilev is char*  --- ???????????
  }

  ++(ato->nr);

  return 0;

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
  // ATO_dump__ (ato);


  // cut srcLn into expressions (get textPos & textLen)
  aNr = UTX_wTab_srcLn (spa, sSiz, 100, srcLn);

    // testDisp splitted line
    // for(i1=0;i1<aNr;++i1)
    // printf(" exp[%d] = %d |%s|\n",i1,sSiz[i1],spa[i1]);


  // create child-ato
  ato1 = *ato;
  if(*typ) {
    ii = ato->nr;
    ato1.typ = &ato->typ[ii];
    ato1.val = &ato->val[ii];
    ato1.nr = 0;
    ato1.siz = ato->siz - ii;
    ato1.txt = srcLn;
  }
    

  // loop tru expressions, create ato
  for(i1=0; i1<aNr; ++i1) {

    cp1 = spa[i1];

    // add string to ato
    if(*cp1 == '"') {
      iTyp = Typ_String;
      dVal = cp1 - srcLn;
      irc = ATO_ato_expr_add (&ato1, iTyp, dVal, 0);
      if(irc < 0) {TX_Error("ATO_ato_srcLn_exp E001"); return -1;}

    // resolv expr 
    } else {
      irc = ATO_ato_expr__ (&ato1, cp1, sSiz[i1]);
      if(irc < 0) return -1;
    }

  }

    // printf(" dump child-ato:\n");
    // ATO_dump__ (&ato1);


  // evaluate object (return one single ato from subObjects;i
  //   eg return PT-1 from P(1 2 3)
  if(*typ) {

    // get ato from func (eg create dynObj ..)
    ATO_ato_atoTab__ (typ, val, &ato1);

    // create the parent-ato; kill all child-atos
    ATO_ato_expr_add (ato, *typ, *val, 0);

  } else {  // return all atos unmodified
    *ato = ato1;
  }


  return 0;

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
      sprintf(s1, "%3d",  ato->ilev[i1]);
    } else {
      strcpy(s1, "---");
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
/// uses memspc54 memspc55 memspc012 !
/// \endcode

  int    i1, i2;


  ato1->typ = (int*) memspc55;
  ato1->val = (double*) memspc54;
  ato1->ilev = memspc012;    // NULL;          // 2014-05-13

  i1 = sizeof(memspc55) / sizeof(int);
  i2 = sizeof(memspc54) / 256;

  ato1->siz = IMIN (i1, i2);
  ato1->nr = 0;

  return 0;

}


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


//================================================================
  int ATO_sort1 (int oNr, int *oTyp, double *oVal) {
//================================================================
/// sort types ascending
// see ATO_sort1 AP_ato_sort1 GL_sel_sort

  int    i1;


  // printf("ATO_sort1 %d\n",oNr);
    // for(i1=0; i1<oNr; ++i1)
    // printf(" %d typ=%d tab=%f\n",i1,oTyp[i1],oVal[i1]);


  L_sort0:

    for(i1=1; i1<oNr; ++i1) {
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
/// see also ATO_cpy_rec

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
  int ATO_clean__ (int *is, int *ie, ObjAto *ato) {
//================================================================
/// \code
/// make clean atomicObjects from ato (remove FncNam, NULL ..)
///
/// Input:
///   is     startIndex in ato
///   ie     last index of expression in ato, not included.
/// Output:
///   is     startIndex in ato
///   ie     last index of expression in ato, not included.
/// \endcode


  int   dNr, iie, i1, iv;


  // printf("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC \n");
  // printf("ATO_clean__ is=%d ie=%d\n",*is,*ie);


  dNr = 0;    // nr of deleted records
  iv = *is + 1;   // 
  iie = ato->nr;
  for(i1=iv; i1<iie; ++i1) {
    // if(ato->typ[i1] > Typ_Val) goto L_09;   ????
    if(ato->typ[i1] == Typ_NULL) { ++dNr; continue; }
    if(ato->ilev[i1] == *is) continue;
    L_09:
     // exclude all records including i1
     iie = i1;
     break;
  }
    // printf(" iv=%d dNr=%d\n",iv,dNr);


  *is = iv;
  *ie = iie;
    // printf(" is=%d ie=%d dNr=%d\n",*is,*ie,dNr);



  //----------------------------------------------------------------
  if(dNr > 0) ATO_clean_1 (is, ie, ato);  // remove gaps with typ=Typ_NULL


  //----------------------------------------------------------------
  L_exit:
    // ATO_dump__ (ato, "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC");

  return 0;

}

//================================================================
  int ATO_clean_1 (int *is, int *ie, ObjAto *ato) {
//================================================================
// close all gaps (typ == Typ_NULL); update ie


  int    i1, i2;


  // ATO_dump__ (ato, "ATO_clean_1");

  L_del_nxt:

  for(i1=0; i1 < *ie; ++i1) {
    // find first gap
    if(ato->typ[i1] != Typ_NULL) continue;
    // find next non-gap
    for(i2=i1; i2 < *ie; ++i2) {
      if(ato->typ[i2] == Typ_NULL) continue;
      // copy non-gap-record i2 over gap-record i1
      ATO_cpy_rec (ato, i1, i2);
      // if(irc) {TX_Error("ATO_clean__ E001"); return -1;}
      goto L_del_nxt;
    }
  }


  // set endIndex to last non-gap-record
  for(i1 = (*ie - 1); i1 >= 0; --i1) {
    if(ato->typ[i1] == Typ_NULL) continue; 
    *ie = i1 + 1;
    break;
  }

    // printf(" is=%d ie=%d\n",*is,*ie);
    // ATO_dump__ (ato, "ex ATO_clean_1");

  return 0;

}


//================================================================
  int ATO_ato_eval_geom (ObjAto *ato, int iif) {
//================================================================
/// \code
/// evaluate geometrical functions
/// geom-func: P(2) L,C,D,R,S,U(121) ANG(137) X(134) Y,Z ..
/// Input:
///   iif      startindex in ato
/// Output:
///   ato      ?
///   Retcod:  0 = no geom.func; 1 = function evaluated; -1 = error
/// see APT_decode_func ../ci/NC_apt.c :16974
/// \endcode

  int       i1, i2, typ, iv, iie, vNr, dNr, iNew;
  long      dbi;
  double    d1;
  Point     pt1;
  Line      ln1;
  Circ      ci1;
  // Vector    vc1;
  Plane     pl1;
  // ObjGX     ox1;
  // Memspc    workSeg;
  char      obj1[OBJ_SIZ_MAX];


  typ = ato->val[iif];

  // printf("GGGGGGGGGGG  ATO_ato_eval_geom iif=%d typ=%d\n",iif,typ);

  // skip math.bracket 
  if(ato->typ[iif+1] > Typ_Val) return 0;


  iv = iif;
  iie = ato->nr;
  i1 = ATO_clean__ (&iv, &iie, ato);
    // ATO_dump__ (ato, " after clean");


  //----------------------------------------------------------------
  L_work:
    vNr = iie - iv;  // nr of values
      // printf(" iv=%d iie=%d vNr=%d\n",iv,iie,vNr);


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

    case Typ_Val:
      i1 = APT_decode_var (&d1, vNr, &ato->typ[iv], &ato->val[iv]);
      if(i1 < 0) goto ParErr;
      break;

    case Typ_Angle:
      i1 = APT_decode_ang (&d1, vNr, &ato->typ[iv], &ato->val[iv]);
      if(i1 < 0) goto ParErr;
      break;

    case Typ_ValX:
      typ = Typ_XVal;
      i1 = APT_decode_xyzval (&d1, vNr, &ato->typ[iv], &ato->val[iv], &typ);
      if(i1 < 0) goto ParErr;
      break;

    case Typ_ValY:
      typ = Typ_YVal;
      i1 = APT_decode_xyzval (&d1, vNr, &ato->typ[iv], &ato->val[iv], &typ);
      if(i1 < 0) goto ParErr;
      break;

    case Typ_ValZ:
      typ = Typ_ZVal;
      i1 = APT_decode_xyzval (&d1, vNr, &ato->typ[iv], &ato->val[iv], &typ);
      if(i1 < 0) goto ParErr;
      break;

    case Typ_modif:  // MOD(#)
      typ = Typ_modif;
      d1 = ato->val[iv] - 1;
      break;

    default:
      TX_Error("ATO_ato_eval_geom E001 %d",typ);
      return -1;
  }


  //----------------------------------------------------------------
  // save func as db-obj
  ato->typ[iif] = typ;
  ato->val[iif] = d1;


  // kill all values for this expression
  for(i1=iv; i1<iie; ++i1) {
    ato->typ[i1] = Typ_NULL;
  }


  // done
 
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
  for(ii=0; ii<ie; ++ii) {
    if(ato->typ[ii] != Typ_VAR) continue;
    APT_decode_var (&d1, 1, &ato->typ[ii],  &ato->val[ii]);
    ato->typ[ii] = Typ_Val;
    ato->val[ii] = d1;
  }
    // ATO_dump__ (ato);


  if(ato->nr < 2) goto L_exit;


  //----------------------------------------------------------------
  // evaluate math.operators (+-*/)
  // 3 records Typ_Val/MathOperator/Typ_Val into 1 record Typ_Val.
  L_nxt0:
  imod = 0;
  if(ato->nr < 3) goto L_fnc0;
  // get first record as i2
  i2 = -1;
  for(ii=0; ii < ie; ++ii) {
    if(ato->typ[ii] == Typ_NULL) continue;
    i2 = ii;
    break;
  }
  if(i2 < 0) goto L_fnc0;
    // printf(" start-i2=%d\n",i2);



  // get second record as i3
  i3 = -1;
  for(ii=i2+1; ii < ie; ++ii) {
    if(ato->typ[ii] == Typ_NULL) continue;
    i3 = ii;
    break;
  }
  if(i3 < 0) goto L_fnc0;
    // printf(" start-i3=%d\n",i3);


  L_nxt1:
      // printf(" L_nxt1:\n");
      // ATO_dump__ (ato);
    i1 = i2;
    i2 = i3;
    // get new i3
    i3 = -1;
    for(ii=i2+1; ii < ie; ++ii) {
      if(ato->typ[ii] == Typ_NULL) continue;
      i3 = ii;
      break;
    }
    if(i3 < 0) goto L_fnc0;
      // printf(" i1,i2,i3 %d %d %d\n",i1,i2,i3);

    // find 3 records (value,operator,value) with same level.
    // record i1 must be a value
    if(ato->typ[i1] != Typ_Val) goto L_nxt1;
    if(ato->ilev[i1] != ato->ilev[i2]) goto L_nxt1;
      // printf(" i1-OK\n");

    // record i2 must be a operator
    if(!TYP_IS_OPM(ato->typ[i2])) goto L_nxt1;
    if(ato->ilev[i2] != ato->ilev[i3]) goto L_nxt1;
      // printf(" i2-OK\n");

    // record i3 must be a value
    if(ato->typ[i3] != Typ_Val) goto L_nxt1;
      // printf(" found %d %d %d\n",i1,i2,i3);
    // now records i1,i2,i3 have same parent, types = val,ope,val.

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
    }
      // printf(" comp %lf %lf ope %d\n",ato->val[i1],ato->val[i3],ato->typ[i2]);

    ato->typ[i2] = Typ_NULL;
    ato->typ[i3] = Typ_NULL;
    ++imod;

    goto L_nxt0;





  //----------------------------------------------------------------
  L_fnc0:
  // eval math-functions with only one value  (and geom-func-VAL)
  // 2 Records: Typ_FncNam,Typ_Val;Typ_Val into 1 record Typ_Val.
      // printf(" L_fnc0:\n");
      // ATO_dump__ (ato);
    // get i1 = function-record
    for(i1=0; i1<ie; ++i1) {
      if(ato->typ[i1] != Typ_FncNam) continue;
      // check if its a math-func (skip geom-func)
      // math-func: SQRT(290) SIN...
      // geom-func: P(2) L,C,D,R,S,U(121) ANG(137) X(134) Y,Z
      if(ato->val[i1] < Typ_FcmSQRT) {
        // resolv MOD-funcs
        if(ato->val[i1] == Typ_modif) {
          i2 = ATO_ato_eval_geom (ato, i1);
          if(i2 < 0) break;
          if(i2 > 0) ++imod;
        }
        // resolv VAL-funcs
        if(ato->val[i1] == Typ_Val) {
          i2 = ATO_ato_eval_geom (ato, i1);
          if(i2 < 0) break;
          if(i2 > 0) ++imod;
        }
        continue;
      }
      // now i1 is a function-record
      // get i2 = first value for record i1
      i2 = -1;
      for(ii=i1+1; ii<ie; ++ii) {
        if(ato->ilev[ii] != i1) goto L_fnc9; //continue;
        if(ato->typ[ii] == Typ_NULL) continue;
        if(ato->typ[ii] != Typ_Val) continue;
        i2 = ii;
        break;
      }
      if(i2 < 0) goto L_fnc9;
        // printf(" fnc-i2=%d\n",i2);



      // evaluate func with one numeric value
      // test if more values exist for i1
      for(ii=i2+1; ii<ie; ++ii) {
        if(ato->ilev[ii] != i1) continue;
        if(ato->typ[ii] != Typ_NULL) goto L_fnc9; // skip eg OpmPlus
      }
        // printf(" found fnc %d val %d\n",i1,i2);

      ii = ato->val[i1];   // type of function
// see APT_decode_Fmc
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
          l1 = ato->val[i2];
          d1 = l1;
          break;
        case Typ_FcmRND:
          d1 = 0.5;
          if(ato->val[i2] < 0.0) d1 = -0.5;
          l1 = (int)(ato->val[i2] + d1);
          d1 = l1;
          break;

        default:
          TX_Error("ATO_ato_eval__ E002 %d",ii);
          return -1;
      }



      // change record i1 into value (preserves ival !), delete record i2.
      L_fncSav:
        // printf(" L_fncSav: i1=%d i2=%d\n",i1,i2);
      ato->typ[i1] = Typ_Val;
      ato->val[i1] = d1;
      ato->typ[i2] = Typ_NULL;
      ++imod;

      L_fnc9:
      continue;
    }

    if(imod) goto L_nxt0;    // redo evaluate math.operators




  //----------------------------------------------------------------
  // evaluate geometrical functions
  L_geom_0:
    // printf(" L_geom_0:\n");
  imod = 0;
  for(ii=0; ii<ie; ++ii) {
    if(ato->typ[ii] != Typ_FncNam) continue;
    i1 = ATO_ato_eval_geom (ato, ii);
    if(i1 < 0) goto L_comp_0;
    if(i1 > 0) ++imod;
  }
  if(imod) goto L_geom_0;


  //----------------------------------------------------------------
  // compress ato (remove all Typ_NULL-records)
  L_comp_0:
    // printf(" L_comp_0:\n");

  ii = -1;
  io = 0;   

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
    // printf(" L_exit:\n");
    // ATO_dump__ (ato);
  return 0;

}


//================================================================
   int ATO_ato_txo__ (ObjAto *ato, ObjTXTSRC *tso, char *sl) {
//================================================================
/// get atomic-objects from source-objects

// see APT_decode_Opm

  int     iTyp, oTyp, its=0, sPos, sLen, lev=0;
  long    dbi;
  double  d1;
  char    *p1, *p2, s1[80];


  // printf("ATO_ato_txo__ |%s|\n",sl);
  // APED_txo_dump (tso, sl);


  for(;;) {
    iTyp = tso[its].typ;

    if(iTyp == TYP_FuncEnd) break;

    sPos = tso[its].ioff;
        // printf(" sl[sPos]=|%s|\n",&sl[sPos]);


    //----------------------------------------------------------------
    if(iTyp == Typ_ObjDB) {
      // decode typ
      oTyp = AP_typ_typChar (sl[sPos]);
      // decode dbi
      ++sPos;
      d1 = strtod (&sl[sPos], &p1);
      // eval Vars ?
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
      ATO_srcTxt (&oTyp, &d1, s1);
      ATO_ato_expr_add (ato, oTyp, d1, tso[its].ipar);


    //----------------------------------------------------------------
    } else if(iTyp == Typ_cmdNCsub) {     // 2014-05-13
      oTyp = iTyp;
      d1 = tso[its].form;
      ATO_ato_expr_add (ato, oTyp, d1, tso[its].ipar);


    //----------------------------------------------------------------
    } else if(iTyp == Typ_FncNam) {
      oTyp = iTyp;
      d1 = tso[its].form;
      ATO_ato_expr_add (ato, oTyp, d1, tso[its].ipar);


    //----------------------------------------------------------------
    } else if(iTyp >= TYP_EventEnter) {
      TX_Print("***** ATO_ato_txo__ E002 Record %d", its);
      return -1;

    //----------------------------------------------------------------
    // test for math-operator, math-func; copy record.
    } else if(iTyp >= Typ_FcmSQRT) {
      oTyp = iTyp;
      ATO_ato_expr_add (ato, oTyp, 0., tso[its].ipar);
// see APT_decode_Fmc


    //----------------------------------------------------------------
    // test for values; copy record.
    } else if((iTyp >= Typ_ValX)&&(iTyp < Typ_Typ)) {
      oTyp = iTyp;
      ATO_ato_expr_add (ato, oTyp, 0., tso[its].ipar);



    //----------------------------------------------------------------
    // test for math-operator, math-func; copy record.
    } else if(iTyp >= TYP_OpmPlus) {          // 280-TYP_EventEnter
      oTyp = iTyp + tso[its].form;
      ATO_ato_expr_add (ato, oTyp, 0., tso[its].ipar);
// see APT_decode_Opm


    } else {
      TX_Print("***** ATO_ato_txo__ E001 Record %d", its);
      return -1;
    }

    ++its;
  }


  // ATO_dump__ (ato);

  return 0;

}


//================================================================
  int ATO_ato_srcLn__ (ObjAto *ato, char *srcLn) {
//================================================================
/// \code
/// get atomicObjects from sourceLine; full evaluated.
/// must provide memspc before with ATO_getSpc__
/// Input:
///   srcLn      eg "D(0 0 1)"
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

 
  // printf("ATO_ato_srcLn__ |%s|\n",srcLn);
  // printf(" ilev=%p\n",ato->ilev);



  // get temp-space for tso
  itsMax = SRCU_tsMax (srcLn);   // printf(" itsMax=%d\n",itsMax);
  tso = UME_alloc_tmp (itsMax * sizeof(ObjTXTSRC));
  if(tso == NULL) {TX_Print("ATO_ato_srcLn__ EOM"); return -1;}


  // analyze sourceline; get source-objects -> tso
  irc = APED_txo_srcLn__ (tso, itsMax, srcLn);
    // printf(" _txo_srcLn__ %d\n",irc);
  if(irc < 1) return irc;
    // APED_txo_dump (tso, srcLn);


  // get atomic-objects from source-objects
  irc = ATO_ato_txo__ (ato, tso, srcLn);
  if(irc < 0) return irc;
    // ATO_dump__ (ato, " nach _ato_txo_");


  // evaluate atomic-objects (compute)
  return (ATO_ato_eval__ (ato));

}


//================================================================
  int ATO_ato_obj_pt (ObjAto *ato, int outTyp, int iseg,
                         int typ, void *o1, Point *ptx) {
//================================================================
// ATO_ato_obj_pt          get parametric position of point on obj
// get parametric position of point ptx on obj (typ,o1) as atomicObjs
// eg returns "P(S1 0.5)"
// Input:
//   outTyp    Typ_PT - "P(..)"
//             Typ_VC - "D(..)"
//             0 (Typ_Error) gives eg "P#"
//   typ,o1    typ/object of selected-obj
//   ptx       select-position
//   iseg      nr of segment of selected-obj
//             0 - do not output segmentNr
//             1-n output iseg as MOD(<iseg>) (eg subCurveNr)
// Output:
//   retCod:   0     OK; parameters are in ato
//             -1    ptx not on obj (typ,o1)
//             -2    internal error;
//
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
  // printf("ATO_ato_obj_pt outTyp=%d typ=%d\n",outTyp,typ);



  //----------------------------------------------------------------
  // get nr of points
  ptMax = UT3D_ptNr_obj (typ, o1, UT_DISP_cv);
    // printf(" ptNr=%d\n",ptNr);

  // get tempSpc for points
  pta = (Point*)UME_alloc_tmp(sizeof(Point) * ptMax);



  // get polygon from obj typ,obj
  ptNr = ptMax;
  irc = UT3D_npt_obj (&ptNr, pta, typ, o1, 1, UT_DISP_cv);
  if(irc< 0) {TX_Error("ATO_ato_obj_pt E001"); goto L_err2;}



  // test all segments of polygon; return selPt, point# and offsetValue
  irc = GR_pt_par_sel_npt (&pts, &is, &pars, pta, ptNr, ptx);
  if(irc < 0) goto L_err1;  // -1 = ptx not on obj
    // printf(" _par_sel_npt is=%d pars=%lf\n",is,pars);
    // ATO_dump__ (ato, " nach _sel_npt");


  // add primary segment-nr if more > 1 segments exist
  if(iseg > 0) {
      // printf(" add iseg %d\n",iseg);
    ATO_ato_expr_add (ato, Typ_modif, (double)iseg, 0);
  }


  // add secondary segment-nr for polygon in CCV
  // D|L|C|S(basecurve segNr)            on polygon|CCV
  if(outTyp == Typ_goGeo1) {   // S|C|L
    if(typ == Typ_CVPOL) {   // skip subSegNrs for line & circ in CCV
      is += 1; // segNr
      goto L_add_mod;
    }
    goto L_exit;
  }


  if((outTyp == Typ_LN)     ||
     (outTyp == Typ_VC))       {
    if(typ == Typ_LN) goto L_exit; // D(L) - no segNr
    if(typ == Typ_CVPOL) {
      //  D|L(basecurve segNr segNr)          on polygon in CCV
      is += 1; // segNr
      goto L_add_mod;
    }
  }




  // for P is parameter necessary ...
  // point wanted:
  // get characteristic point (first or last point or point of plg)
  if(irc == 0) {     // retCode of GR_pt_par_sel_npt: 0=point selected
    if(is == 0) {
      // startpoint selected
      is = 1;
      goto L_add_mod;

    } else if(is == (ptNr - 1)) {
      // endpoint selected
      is = 2;
      goto L_add_mod;
    }
  }


  //----------------------------------------------------------------
  // point: no characteristic point selected; parameter necessary ...
  // get parameter from selected point
  irc = UTO_parpt_pt_obj (&pars, &pts, typ, o1);
  if(irc < 0) {TX_Error("ATO_ato_obj_pt E002"); goto L_err2;}

  // add parameter to atomicObjs
  ATO_ato_expr_add (ato, Typ_Val, pars, 0);
  goto L_exit;


  //----------------------------------------------------------------
  L_add_mod:
    ATO_ato_expr_add (ato, Typ_modif, (double)is, 0);


  //----------------------------------------------------------------
  L_exit:
    // ATO_dump__ (ato, "ex ATO_ato_obj_pt");
  return 0;

  L_err1:
    return -1;

  L_err2:
    return -2;

}



// EOF
