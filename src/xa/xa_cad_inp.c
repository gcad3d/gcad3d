// xa_cad_inp.c
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
  ..

-----------------------------------------------------
Modifications:
  ..

-----------------------------------------------------
*/
/*!
\file  ../xa/xa_cad_inp.c
\brief  CAD inputfunctions
\code
=====================================================
List_functions_start:

IE_inpTxtOut       create outputText from inputFieldText
IE_inpCkAdd        add or replace or skip input.  
IE_inpCkTyp        returns basicobjTyp from inputFieldText.

IE_txt2par1        get next parameter from atomicObjects

List_functions_end:
=====================================================

\endcode *//*----------------------------------------




*/



#ifdef _MSC_VER
#include "MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>                          // f isdigit ..


#include "../ut/ut_geo.h"
#include "../ut/ut_txt.h"         // UTX_pos_skipLeadBlk

#include "../xa/xa_sele.h"        // Typ_go*

#include "../xa/xa_cad_ui.h"      // INPRECANZ inpAuxDat ..





extern int  IE_inpTypAct;     // requested type for the active inputField
extern int  IE_inpSrc;        // type of Input; 2=normal, 3=PgUp/Dwn
extern int  IE_inpTypR[INPRECANZ];         // inputTypes (requested type)
extern inpAuxDat IE_inpAuxDat[INPRECANZ];       // data for inputFields



//================================================================
  int IE_inpTxtOut (char **txt, char *tmpBuf, char *actBuf,
                    int iind, int iskip) {
//================================================================
// add text actBuf to outputText.
// create outputText from inputFieldText,
//   (eg add "D(*)" if necessary)
// Input:
//   actBuf  inputFieldText                           siz=256
//   tmpBuf  space for outputText (if to be modified) siz=256
//   iind    index CAD-inputfield
//   iskip   ON|OFF ?
// Output:
//   txt     outputText, (pointer to tmpBuf or actBuf)
//   retCod  -2 = Error
//           -3 = not enough parameters; do nothing

  // static Vector    vc1;  // keep last vcs


  int       irc, i1, i2, inptyp, typ;
  // int       *atyp;
  double    d1;
  ObjAto    ato;
  char      *p1;
  // Point     pt1, pt2, pt3;


  if(iind < 0) return -1;

  // get inptyp = requested input-type
  inptyp = IE_inpTypR[iind];

  // printf("IE_inpTxtOut inptyp=%d |%s|\n",inptyp,actBuf);


  p1 = actBuf;


  // get memSpc for ato
  ATO_getSpc__ (&ato);

  i1 = 0; // get expressions, not result.
  d1 = 0.;
  ATO_ato_srcLn_exp (&ato, &i1, &d1, p1);
    // ATO_dump__ (&ato);


  // get outTyp; eg Typ_VC from requestedTyp - eg Typ_goGeo7
  typ = IE_inpCkTyp (iind, p1, &ato);

/*
  // get atomicObjs from inputFieldText
  inpWdNr = APT_decode__ (&ato, &p1);
  // inpWdNr = IE_decode_Ln (aus_typ, (void*)aus_tab, 10, actBuf);
    printf(" _inpTxtOut inptyp=%d actBuf=|%s| WdNr=%d\n",
            inptyp,actBuf,inpWdNr);
    ATO_dump__ (&ato);
*/

  // APT_obj_ato (., typ);

 
  //----------------------------------------------------------------
    if(inptyp == Typ_Angle) {
      // already starting with "ang(" ?
      if(UTX_ck_casenChr (actBuf, "ANG", 3) == 0) {
          // printf(" ang exists |%s|\n",actBuf);
        strcpy(tmpBuf, actBuf);
        UTX_cp_word_2_upper (tmpBuf, tmpBuf);
        goto L_add_mod;
      }
      // add "ANG()"
      sprintf(tmpBuf, "ANG(%s)",actBuf);
      goto L_add_mod;



  //----------------------------------------------------------------
    } else if(inptyp == Typ_Val)   {
              // ((inptyp >= Typ_Val_symTyp)&&(inptyp < Typ_Str_Dir1)))  {
      sprintf(tmpBuf, "VAL(%s)",actBuf);
      goto L_add_mod;
      // strcat(IE_outTxt, actBuf);

/*
  //----------------------------------------------------------------
    } else if(inptyp == Typ_ValX) {
      goto L_add_txt;
  //----------------------------------------------------------------
    } else if(inptyp == Typ_ValY) {
      goto L_add_txt;
*/

  //----------------------------------------------------------------
    } else if(inptyp == Typ_YVal) {
      sprintf(tmpBuf, "Y(%s)",actBuf);
      goto L_add_mod;


  //----------------------------------------------------------------
    } else if(inptyp == Typ_XVal) {
      sprintf(tmpBuf, "X(%s)",actBuf);
      goto L_add_mod;


  //----------------------------------------------------------------
    } else if(inptyp == Typ_ZVal) {
      sprintf(tmpBuf, "Z(%s)",actBuf);
      goto L_add_mod;


  //----------------------------------------------------------------
    } else if(inptyp == Typ_EyePT) {
      // IE_getEyePt (tmpBuf);
      // goto L_add_mod;
      sprintf(tmpBuf, "\"%s\"",actBuf);
      goto L_add_mod;


  //----------------------------------------------------------------
    } else if(inptyp == Typ_PT) {
      // Pt aus "P20" direkt raus.
      if(ato.nr == 1) {
        if(((actBuf[0] == 'P')||(actBuf[0] =='p')) &&
           ((actBuf[1] == '(')||(UTX_ck_num_f(&p1,&actBuf[1]) == 0))) {
          goto L_add_txt;
        }
      }
      // Pt aus 2 od 3 zahlen als P(...) raus.
      sprintf(tmpBuf, "P(%s)",actBuf);
      goto L_add_mod;


  //----------------------------------------------------------------
    } else if(inptyp == Typ_LN) {
      // printf(" encode LN |%s|\n",actBuf);
      // wenn text mit L beginnt kein nichtnumerisches Zeichen im Text: raus;
      if(ato.nr == 1) {
        if(((actBuf[0] == 'L')||(actBuf[0] =='l')) &&
           ((actBuf[1] == '(')||(UTX_ck_num_f(&p1,&actBuf[1]) == 0))) {
          goto L_add_txt;
        }
      }

      sprintf(tmpBuf, "L(%s)",actBuf);
      goto L_add_mod;


  //----------------------------------------------------------------
    } else if(inptyp == Typ_CI) {
      // printf(" encode AC |%s|\n",actBuf);
      // wenn text mit C beginnt kein nichtnumerisches Zeichen im Text: raus;
      if(ato.nr == 1) {
        if(((actBuf[0] == 'C')||(actBuf[0] =='c')) &&
           ((actBuf[1] == '(')||(UTX_ck_num_f(&p1,&actBuf[1]) == 0))) {
          goto L_add_txt;
        }
      }

      sprintf(tmpBuf, "C(%s)",actBuf);
      goto L_add_mod;



  //----------------------------------------------------------------
    } else if(inptyp == Typ_VC) {
      // "C20" -> D(C20)"
      if(ato.nr == 1) {
        if((actBuf[0] == 'D')||(actBuf[0] =='d')) {
          goto L_add_txt;
        }
      }
      // eg "DX 2.5" -> "D(DX 2.5)"
      sprintf(tmpBuf, "D(%s)",actBuf);
      goto L_add_mod;


  //----------------------------------------------------------------
    } else if(inptyp == Typ_PLN) {

      // "P20" -> R(P20)"
      if(ato.nr == 1) {
        if((actBuf[0] == 'R')||(actBuf[0] =='r')) {
          goto L_add_txt;
        }
      }
      // eg "L20 DX" -> "R(L20 DX)"
      sprintf(tmpBuf, "R(%s)",actBuf);
      goto L_add_mod;


/*
  //----------------------------------------------------------------
    } else if(inptyp == Typ_goGeo4) {
      // 3 Values -> vector                             2010-11-27
      if(ato.nr == 3) {
        sprintf(tmpBuf, "D(%s)",actBuf);
        goto L_add_mod;
      } else goto L_add_txt;


*/
  //----------------------------------------------------------------
    } else if(inptyp == Typ_goGeo7) {        // ANG|PT  ODER Val|P|D ???
      if(ato.nr == 1) {
        // 1 value can be angle
        // if(ato.typ[0] == Typ_Val) goto L_err_vc3vals;
        if(ato.typ[0] == Typ_Val) goto L_add_txt;
        // 1 vector or 1 point direct; else D().
        if((actBuf[0] == 'P')||(actBuf[0] =='p')) goto L_add_txt;
        if((actBuf[0] == 'D')||(actBuf[0] =='d')) goto L_add_txt;

      } else if(ato.nr == 2) {
        // 2 values:
        if((ato.typ[0] == Typ_Val)&&(ato.typ[1] == Typ_Val)) goto L_err_vc3vals;
      }
      // convert to vec & add 
      sprintf(tmpBuf, "D(%s)",actBuf);
      goto L_add_mod;



  //----------------------------------------------------------------
    } else if(inptyp == Typ_goGeo8) {
      goto L_add_txt;


  //----------------------------------------------------------------
    } else if(inptyp == Typ_goAxis) {
      // wenn mehrere words: "L(..)"; else einfach zufuegen.
      if((actBuf[0] == 'P')||(actBuf[0] =='p')) {
        sprintf(tmpBuf, "L(%s)",actBuf);
        goto L_add_mod;
      } else goto L_add_txt;




  //----------------------------------------------------------------
    } else if(inptyp == Typ_Group) {
        if(ato.nr > 1)
          sprintf(tmpBuf, "U(%s)",actBuf);
        else
          sprintf(tmpBuf, "%s",actBuf);
        goto L_add_mod;



  //----------------------------------------------------------------
    } else if(inptyp == Typ_modCTRL)   {
      goto L_add_txt;


    } else if(inptyp == Typ_modCWCCW)   {
      goto L_add_txt;


    } else if((inptyp == Typ_mod1)   ||
              (inptyp == Typ_mod2))      {
      sprintf(tmpBuf, "MOD(%s)",actBuf);
      goto L_add_mod;




  //----------------------------------------------------------------
    } else if((inptyp == Typ_String)    ||
              (inptyp == Typ_CtlgPart))     {
              // ((inptyp >= Typ_Str_Dir1)&&(inptyp < Typ_Obj_0)))  {
        sprintf(tmpBuf, "\"%s\"",actBuf);
        goto L_add_mod;



  //----------------------------------------------------------------
    } else if((inptyp == TYP_FilNam)    ||
              (inptyp == Typ_SubModel))     {
        // if user selects ctlgModel as internal model:
        if(!strncmp (actBuf, "CTLG ", 5))
          sprintf(tmpBuf, "%s",actBuf);
        else
          sprintf(tmpBuf, "\"%s\"",actBuf);
        goto L_add_mod;



  //----------------------------------------------------------------
    } else {
      if(iskip != ON) goto L_add_txt;
      // printf(" iskip=%d outTxt=|%s| actBuf=|%s|\n",iskip,IE_outTxt,actBuf);
    }


  L_add_txt:   // no modification; actBuf = output;
    *txt = actBuf;
    return 0;

  L_add_mod:   // modified; tmpBuf = output;
    *txt = tmpBuf;
    return 1;


  // L_err_morePars:
    // TX_Error("not enough parameters ..");
    // return -3;

  L_err_vc3vals:
    TX_Error("define Vector by 3 values; eg \"0 0 1\" or angle by \"ANG(degree)\" ..");
    return -2;


}


//================================================================
  int IE_inpCkAdd (int *iAct, int *iNxt,  int *iDisp,
                   int typSel, char *actTxt, char *newTxt) {
//================================================================
// add or replace or skip input.
// newTxt is the selected obj; actTxt is the existing inputFieldText.
// IE_inpTypAct = requested inputTyp; eg Typ_goGeo7
// Input:
//   typSel  last selected obj
//   actTxt  last inputFieldtext
//   newTxt  text to be added to inputField
// Output:
//   iAct    0=replace; 1=add; 2=remove; 3=insert as 1.word
//          -1=skip - Error.
//   iNxt   -1=gotoNextField
//           0=keep active field, test;
//           1=keep active field, do not test (not complete);
//   iDisp   0=display; 1=do not reDisplay;

  int    actLen;


  // printf("IE_inpCkAdd typSel=%d |%s|%s|\n",typSel,actTxt,newTxt);
  // printf("  IE_inpTypAct=%d\n",IE_inpTypAct);
  // printf("  IE_inpSrc=%d\n",IE_inpSrc);


  actLen = strlen(actTxt);


  *iNxt = -1;   // default: gotoNextField

  *iDisp = 0;   // redisplay obj;




  //----------------------------------------------------------------
  if(IE_inpTypAct == Typ_Val) {

    if(typSel == Typ_PT) {    // 1 point only = uncomplete
      if(actLen < 2) goto L_err_not_complete; // keep field, do not test.
    }

    goto L_add;



  //----------------------------------------------------------------
  } else if(IE_inpTypAct == Typ_Angle) {
    if(IE_inpSrc == 3) {      // 3=PgUp/Dwn
      *iNxt =  0;             // keep active field;
      goto L_rep;
    }

    if(typSel == Typ_modREV) {
      *iNxt =  0;             // keep active field;
      // do not accept if line is empty ..
      if(actLen < 2) goto L_err_notFirstM;
      // check if newTxt already exists in actTxt
      if(strstr (actTxt, newTxt)) goto L_rem;
    }

    if(typSel == Typ_PT) {    // 1 point only = uncomplete
      if(actLen < 2) goto L_err_not_complete; // keep field, do not test.
    }

    goto L_add;



  //----------------------------------------------------------------
  } else if(IE_inpTypAct == Typ_VC) {

    // db-vec from V*/VC- button:
    if(IE_inpSrc == 3) {      // 3=PgUp/Dwn
      *iAct = 0;   // 0=replace;
      *iNxt = 0;   // 0=keep,test
      *iDisp = 0;   // 0=display
      return 0;
    }

    // only 1 point: not complete
    if((typSel == Typ_PT)||(typSel == Typ_TmpPT)) {
      if(actLen < 2) goto L_err_not_complete; // keep field, do not test.
    }

    if(typSel == Typ_modREV) {
      *iNxt =  0;             // keep active field;
      // do not accept if line is empty ..
      if(actLen < 2) goto L_err_notFirstM;
      // check if newTxt already exists in actTxt
      if(strstr (actTxt, newTxt)) goto L_rem;
    }


    *iAct = 1;    // 1=add
    *iNxt = -1;   //0;    // 0=keep,test   -1=gotoNextField
    *iDisp = 0;   // 0=display
    return 0;



  //----------------------------------------------------------------
  } else if(IE_inpTypAct == Typ_LN) {

    if((typSel == Typ_TmpPT)  ||
       (typSel == Typ_PT))        {    // 1 point only = uncomplete
      if(actLen < 2) goto L_err_not_complete; // keep field, do not test.
    }

    goto L_add;  // Typ_modREV


  //----------------------------------------------------------------
  } else if(IE_inpTypAct == Typ_CI) {

    if((typSel == Typ_TmpPT)  ||
       (typSel == Typ_PT))        {    // 1 point only = uncomplete
      if(actLen < 2) goto L_err_not_complete; // keep field, do not test.
    }

    goto L_add;  // Typ_modREV



  //----------------------------------------------------------------
  } else if(IE_inpTypAct == Typ_PLN) {

    // handle PERP
    if(typSel == Typ_modPERP) {
      *iNxt =  1;   // keep active field;
      if(strncmp(actTxt,"PERP",4)) {
        if(actLen < 2) *iDisp = 1;       // do not disp
        // insert PERP (as 1. word)
        goto L_in1;
      } else {
        if(actLen == 4) *iDisp = 1;       // do not disp
        // remove PERP (1. word)
        goto L_rem;
      }
    }

    // first obj must be a Plane or a point, not a Vector.
    if((actLen < 1)||(!strcmp(actTxt,"PERP")))   {
      if((typSel == Typ_VC)      ||
         (typSel == Typ_Val))        goto L_err_notFirstPP;
      if((typSel == Typ_TmpPT)   ||
         (typSel == Typ_PT))         {
        // display, but not yet complete;
        *iAct = 1;   // 1=add
        *iNxt = 1;   // 0=keep,do not test
        *iDisp = 0;   // 0=display
        return 0;
      }
    }
    goto L_add;



  //----------------------------------------------
  } else if(IE_inpTypAct == Typ_goGeom) {
    // VC
    if(IE_inpSrc == 3) {       // 3=PgUp/Dwn:  keep active field;
      *iNxt =  0;
      goto L_rep;
    }


  //----------------------------------------------
  } else if(IE_inpTypAct == Typ_goGeo7) {

    if((typSel == Typ_PT)     ||
       (typSel == Typ_TmpPT))    {
      goto L_rep;
    }
    if(typSel == Typ_modREV) {
      // do not accept if line is empty ..
      if(actLen < 2) goto L_err_notFirstM;
      // D(PT REV) cannot be resolved !
      if(actTxt[0] == 'P') goto L_err_RevNotWithPoint;
      // check if newTxt already exists in actTxt
      if(strstr (actTxt, newTxt)) goto L_rem;
      goto L_add;
    }
    // typSel = Typ_VC
    if(IE_inpSrc == 3) {      // 3=PgUp/Dwn
      *iNxt =  1;   // keep active field;
      goto L_rep;
    }
    goto L_rep;
    // goto L_add;


  //----------------------------------------------
  } else if(IE_inpTypAct == Typ_modCWCCW) {
    if(actLen > 0) goto L_rem;

  } else if(IE_inpTypAct == Typ_modREV) {
    if(actLen > 0) goto L_rem;

  } else if(IE_inpTypAct == Typ_modCX) {
    if(actLen > 0) goto L_rem;

  } else if(IE_inpTypAct == Typ_modCTRL) {
    if(actLen > 0) goto L_rem;

  } else if(IE_inpTypAct == Typ_modPARL) {
    // if(actLen > 0) {
      // if() goto L_rem;
      goto L_rem;
    // }

  } else if(IE_inpTypAct == Typ_FncDirX) {  // PARL|CX
    if(actLen > 0) {
       if(strstr(actTxt, newTxt)) goto L_rem;
    }
  }




  // 0=replace; 1=add; 2=remove; -1=skip
  L_rep:   *iAct = 0;  return 0;
  L_add:   *iAct = 1;  return 1;
  L_rem:   *iAct = 2;  return 2;
  L_in1:   *iAct = 3;  return 3;
  L_skip:  *iAct = -1; return -1;


  L_err_notFirstM:
    TX_Print("**** modifier cannot be first parameter ..");
    goto L_skip;


  L_err_notFirstPP:
    TX_Print("**** first parameter must be point or plane ..");
    goto L_skip;


  L_err_RevNotWithPoint:
    TX_Print("**** REV cannot be used with point ..");
    goto L_skip;


  L_err_not_complete:
    TX_Print("**** obj not yet complete ..");
    *iNxt = 1;   // keep field, do not test.
    *iDisp = 1;   // do not redisplay obj;
    goto L_add;

}


//================================================================
  int IE_inpCkTyp (int iind, char *actBuf, ObjAto *ato) {
//================================================================
// returns outTyp for inputFieldText.
// Example: requestedTyp Typ_goGeo7, inputFieldText="D12", retCod=Typ_VC.
//
// Input:
//   iind       index inputField
//   ato        test of ato[0].typ
// Output:
//   retCod     outTyp, basicobjTyp

// see also IE_txt2par1


  int     irc, i1, actTyp, typ;
  long    dbi;
  double  d1;
  char    *p1, *selNam, subTyp1;


  // requested objTyp   eg Typ_goGeo7
  typ = IE_inpTypR[iind];
  actTyp = typ;



  // printf("IE_inpCkTyp ii=%d |%s| req=%d\n",iind,actBuf,typ);
  // printf(" ato->typ[0]=%d\n",ato->typ[0]);



  // skip (return) modifier 
  if(TYP_IS_MOD(typ))     goto L_found;

  // skip modelID's
  if((typ == TYP_FilNam)    ||
     (typ == Typ_SubModel)  || 
     (typ == Typ_CtlgPart))    goto L_found;


  // empty input:
  if(strlen(actBuf) < 1) goto L_found;


  // Points, Lines, Vectors can have more than 1 word; eg "P22 X(100)" for point;
  // skip this.
  if((typ==Typ_PT)||(typ == Typ_VC)||(typ == Typ_LN)) goto L_typ;


/*
  // // get type of code ..
  // actTyp = AP_typ_srcExpr (actBuf, &p1);   // decode actBuf

  // get memSpc for ato
  ATO_getSpc__ (&ato);

  i1 = 0; // get expressions, not result.
  d1 = 0.;
  ATO_ato_srcLn_exp (&ato, &i1, &d1, actBuf);
    ATO_dump__ (&ato);
*/

  

  if(ato->nr == 1) {

    if((ato->typ[0] == Typ_VAR) ||
       (ato->typ[0] == Typ_Val))   {
      if((typ == Typ_Val)     ||
         (typ == Typ_Angle)   ||
         (typ == Typ_PT)      ||
         (typ == Typ_VC))    goto L_typ;
      if (typ == Typ_goGeo7) goto L_ato0;
      if (typ == Typ_goGeo8) goto L_ato0;
      if (typ == Typ_goPrim) goto L_ato0;         // 2013-03-15
      goto L_found;
    }
    if(ato->typ[0] == Typ_PT) {
      if (typ == Typ_goGeo7) goto L_ato0;
      if (typ == Typ_goGeo8) goto L_ato0;
    }
  }
/*
    else if(actTyp == Typ_ObjDB) {          // test textTyp ..
    // actTyp = AP_typ_typChar (*actBuf);
    APED_dbo_oid (&actTyp, &dbi, actBuf);


  } else if(actTyp == Typ_Txt) {            // test textTyp ..
    // get atomicObj from Typ_Txt
    ATO_srcTxt (&actTyp, &d1, actBuf);


  } else if(actTyp == Typ_FncNam) {  // test functionTyp
    actTyp = AP_typ_FncNam (actBuf, p1 - actBuf);  // get type of function
  }

      printf(" actTyp  = %d\n",actTyp);


  if(actTyp == Typ_CV) {
    // get curveSubtyp
    DB_get_CV (&actTyp, dbi);
      printf(" curveSubtyp %d\n",actTyp);
      goto L_found;
  }
*/


  //================================================================
 
  //----------------------------------------------------------------
  if(typ == Typ_Angle) {
    // ANG(C)
    goto L_typ;


  //----------------------------------------------------------------
  } else if(typ == Typ_TmpPT) {
    actTyp = Typ_PT;
    goto L_found;

/*
  //----------------------------------------------------------------
  } else if(typ == Typ_PT) {
    if((actTyp == Typ_ValX)||(actTyp == Typ_ValX)||(actTyp == Typ_ValX)||
       (actTyp == Typ_Angle)) {
      actTyp = Typ_PT;
      goto L_found;
    }


  //----------------------------------------------------------------
  } else if(typ == Typ_LN) {
    // "X(.....)" provides a Line !
    if((actTyp == Typ_ValX)||(actTyp == Typ_ValX)||(actTyp == Typ_ValX))
      goto L_found;
*/


  //----------------------------------------------------------------
  } else if(typ == Typ_CI) {
    // P P | P Val | P P P
    actTyp = Typ_CI;
    goto L_found;


  //----------------------------------------------------------------
  } else if(typ == Typ_PLN) {
    // P P | P D | ..
    actTyp = Typ_PLN;
    goto L_found;



  //----------------------------------------------------------------
  } else if(typ == Typ_goGeom) {   
    // Typ_goGeom: all but modifiers
    goto L_ato0;


  //----------------------------------------------------------------
  } else if(typ == Typ_goPrim) {   
    // Typ_goPrim: P|L|C|S(Ell,Bsp,Plg,CCV)  - NOT Sur|Sol;
    goto L_ato0;


  //----------------------------------------------------------------
  } else if(typ == Typ_go_RA) {         // plane|surface
    goto L_ato0;


  //----------------------------------------------------------------
  } else if(typ == Typ_go_LCS) {   
    // Typ_go_LCS: L|C|S(Ell,Bsp,Plg,CCV)   NOT P|Sur|Sol;
    goto L_ato0;


  //----------------------------------------------------------------
  } else if(typ == Typ_goGeo1) {   
    // Typ_goGeo1: Typ_go_LCS|Pln|Sur|Sol;   NOT P
    goto L_ato0;


  //----------------------------------------------------------------
  } else if(typ == Typ_goGeo2) {   
    // Typ_goGeo2: C|Ell
    goto L_ato0;


  //----------------------------------------------------------------
  } else if(typ == Typ_goGeo6) {           // P L C
    goto L_ato0;


  //----------------------------------------------------------------
  } else if(typ == Typ_goGeo7) {           // Typ_goGeo7: Val|P|D

    // get 1 char of 3.word of info = subTyp; 
    subTyp1 = IE_inpAuxDat[iind].subTyp[0];
    if(subTyp1 == 'D') {  // need 2.point for vector !
      actTyp = Typ_VC;
    }

    // Typ_goGeo7 (VC from eg "S MOD")    TODO: not checked correct yet ..
    typ = Typ_VC;
    goto L_typ;


  //----------------------------------------------------------------
  } else if(typ == Typ_goGeoSUSU) {      // Sur|Sol
    goto L_ato0;



  //----------------------------------------------------------------
  }

  L_found:

    // printf("ex IE_inpCkTyp ii=%d typ=%d\n",iind,actTyp);

  return actTyp;

  L_typ:
   actTyp = typ;
   goto L_found;

  L_ato0:
   actTyp = ato->typ[0];
   goto L_found;


}


//=====================================================================
  int  IE_txt2par1 (char* buf,int typRec,
                    int aus_anz,int *ind,int *aus_typ,char txtTab[][256]) {
//=====================================================================
/*
IE_txt2par1        get next parameter(s) from atomicObjects
Example: requested type is Typ_VC,
         atomicObjTab provides 3 Typ_Val's,
         outputText keeps this 3 Typ_Val's.
         

used by IE_activate - modify scrObj

In:
     typRec  requested type; eg Typ_VC or Typ_goGeom

     ind   der Zeiger into aus_typ u. txtTab fuer das next Obj.
     aus_typ = die Parametertype die in der Editline erkannt wurden;
     txtTab   der zu aus_typ gehoerige Wert


Out: buf = der zugehoerige Text (fuers Entryfeld)
     ind        pointer to next unused value in aus_typ/txtTab
     RetCod:    1=OK, -1=no useful object for typRec

*/


  int     i1, typ, indNxt1, indNxt2, typNxt1, typNxt2;
  char    *p1;
  // char   val[32], valNxt1[32], valNxt2[32];




  // printf("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW \n");
  printf("IE_txt2par1  aus_anz=%d ind=%d typRec=%d\n",aus_anz,*ind,typRec);
  printf(" nxt typ=%d\n",aus_typ[*ind]);
  for(i1=*ind; i1<aus_anz; ++i1) {
    printf("  %d aus_typ=%d txtTab=%s\n",i1,aus_typ[i1], txtTab[i1]);
  }



  // typ = typ from inputline 
  typ = aus_typ[*ind];
  // printf(" _txt2par1 typRec=%d typ=%d txt=|%s|\n",typRec,typ,txtTab[*ind]);


  // next typ/val
  if(aus_anz >= (*ind+1)) {
    indNxt1 = *ind + 1;
    typNxt1 = aus_typ[indNxt1];
    // valNxt1 = aus_tab[indNxt1];
    /* printf(" typ/valNxt1=%d %f\n",typNxt1,valNxt1); */
  } else {
    typNxt1 = -1;
  }


  // next typ/val
  if(aus_anz >= (*ind+2)) {
    indNxt2 = *ind + 2;
    typNxt2 = aus_typ[indNxt2];
    // valNxt2 = aus_tab[indNxt2];
    /* printf(" typ/valNxt2=%d %f\n",typNxt2,valNxt2); */
  } else {
    typNxt2 = -1;
  }


  if(typ == typRec) goto L_OK_2;




  //-------------------------------------------------------
  // typ ist nun ein objTyp der selektierten APT-Line;
  // typRec ist ein typ im IE_cad_act-Record
    // printf(" _txt2par1  typ=%d typRec=%d\n",typ,typRec);


  //-------------------------------------------------------
  if(typ == Typ_Val) {        // eine Zahl in der Editline -

    if(typRec == Typ_XVal)    goto L_OK_2;
    if(typRec == Typ_YVal)    goto L_OK_2;
    if(typRec == Typ_ZVal)    goto L_OK_2;
    if(typRec == Typ_Rad)     goto L_OK_2;
    if(typRec == Typ_Angle)   goto L_OK_2;
    if(typRec == Typ_goGeo7)  goto L_OK_2;
    if(typRec == Typ_goGeo8)  goto L_OK_2;
    if(typRec == Typ_goGeom)  goto L_OK_2;
    if(typRec == Typ_Txt)     goto L_OK_2;

    if(typRec == Typ_PT) {  // koennte man durch 2 Typ_Val plus Typ_ZVal ersetzen
      if((typNxt1 == Typ_Val)   &&
         (typNxt2 == Typ_ZVal)) {
        *ind = *ind + 2;
        goto L_OK_3;
      }
    }


    if(typRec == Typ_PT) {  // koennte man durch 2 Typ_Val ersetzen
      if(typNxt1 == Typ_Val) {
        *ind = *ind + 1;
        goto L_OK_3;
      }
    }


  //-------------------------------------------------------
  } else if(typ == Typ_VAR) {  // im APT-Text steht zB "V12"

    if(typRec == Typ_Val)    goto L_OK_2;
    if(typRec == Typ_XVal)   goto L_OK_2;
    if(typRec == Typ_YVal)   goto L_OK_2;
    if(typRec == Typ_ZVal)   goto L_OK_2;
    if(typRec == Typ_Rad)    goto L_OK_2;
    if(typRec == Typ_goGeom) goto L_OK_2;
    if(typRec == Typ_goGeo8) goto L_OK_2;  // Radius
    if(typRec == Typ_mod1)   goto L_OK_2;  // only for old N-commands
    // if((typRec >= Typ_Val_symTyp)&&(typRec < Typ_Str_Dir1)) goto L_OK_2;

  //-------------------------------------------------------
  } else if(typ == Typ_Angle) {
    if(typRec == Typ_LN)      goto L_OK_2;


  //-------------------------------------------------------
  } else if(typ == Typ_PT) {
    if(typRec == Typ_EyePT)   goto L_OK_2;
    if(typRec == Typ_goPrim)  goto L_OK_2;
    if(typRec == Typ_goGeom)  goto L_OK_2;
    if(typRec == Typ_goGeo7)  goto L_OK_2;
    if(typRec == Typ_goGeo8)  goto L_OK_2;

  //-------------------------------------------------------
  } else if(typ == Typ_LN) {
    if(typRec == Typ_goPrim)  goto L_OK_2;
    if(typRec == Typ_go_LCS)    goto L_OK_2;
    if(typRec == Typ_goGeom)  goto L_OK_2;
    if(typRec == Typ_goGeo1)  goto L_OK_2;
    if(typRec == Typ_goGeo6)  goto L_OK_2;
    if(typRec == Typ_go_LR)   goto L_OK_2;
    if(typRec == Typ_goAxis)  goto L_OK_2;


  //-------------------------------------------------------
  } else if(typ == Typ_CI) {
    if(typRec == Typ_goPrim)  goto L_OK_2;
    if(typRec == Typ_go_LCS)    goto L_OK_2;
    if(typRec == Typ_goGeom)  goto L_OK_2;
    if(typRec == Typ_goGeo1)  goto L_OK_2;
    if(typRec == Typ_goGeo2)  goto L_OK_2;
    if(typRec == Typ_goGeo5)  goto L_OK_2;
    if(typRec == Typ_goGeo8)  goto L_OK_2;
    if(typRec == Typ_goAxis)  goto L_OK_2;


  //-------------------------------------------------------
  // im Entryfeld steht zB "S22"
  } else if(typ == Typ_CV) {
    if(typRec == Typ_CVTRM)  goto L_OK_2;
    if(typRec == Typ_CVPOL)  goto L_OK_2;
    if(typRec == Typ_go_LCS)   goto L_OK_2;
    if(typRec == Typ_goPrim) goto L_OK_2;
    if(typRec == Typ_goGeom) goto L_OK_2;
    if(typRec == Typ_goGeo1) goto L_OK_2;
    if(typRec == Typ_goGeo2) goto L_OK_2;
    // nur ELL und CCV; testen ?
    if(typRec == Typ_goGeo5) goto L_OK_2;

  //-------------------------------------------------------
  } else if(typ == Typ_VC) {
    if(typRec == Typ_Angle)  goto L_OK_2;
    if(typRec == Typ_goGeom) goto L_OK_2;
    if(typRec == Typ_goGeo7) goto L_OK_2;

  //-------------------------------------------------------
  } else if(typ == Typ_PLN) {
    if((typRec == Typ_goGeom)          ||
       (typRec == Typ_goGeo1)          ||
       (typRec == Typ_go_LR)           ||
       (typRec == Typ_goAxis))  goto L_OK_2;

  //-------------------------------------------------------
  } else if(typ == Typ_SUR) {
    if(typRec == Typ_goGeoSUSU) goto L_OK_2;
    if(typRec == Typ_goGeom)    goto L_OK_2;

  //-------------------------------------------------------
  } else if(typ == Typ_SOL) {
    if(typRec == Typ_goGeo1)    goto L_OK_2;
    if(typRec == Typ_goGeoSUSU) goto L_OK_2;

  //-------------------------------------------------------
  } else if(typ == Typ_modif) {
    if(typRec == Typ_mod1) goto L_OK_2;
    if(typRec == Typ_mod2) goto L_OK_2;
    // if(typRec == Typ_modInOut) goto L_OK_2;

  //-------------------------------------------------------
  } else if((typ == Typ_modCX)      ||
            (typ == Typ_modPARL)) {
    if(typRec == Typ_FncDirX) goto L_OK_2;

  //-------------------------------------------------------
  } else if(typ == Typ_String) {                 // in der Inputzeile;
    if(typRec == Typ_CtlgPart) goto L_OK_2;
    if(typRec == Typ_SubModel) goto L_OK_2;
    if(typRec == Typ_EyePT)    goto L_OK_2;
    // if((typRec >= Typ_Str_Dir1)&&(typRec < Typ_Obj_0)) goto L_OK_2;

  //-------------------------------------------------------
  // } else if(typ == Typ_GTXT) {               // Notes, Tags ...
  } else if(typ == Typ_Note) {               // Notes, Tags ...
    if(typRec == Typ_goGeom) goto L_OK_2;

  //-------------------------------------------------------
  } else if(typ == Typ_Model) {              // M
    if(typRec == Typ_SubModel) goto L_OK_2;
    if(typRec == Typ_goGeom)   goto L_OK_2;

  //-------------------------------------------------------
/*
  } else if(typ == Typ_cmdNCsub) {
    i1 = val;
    printf("Typ_cmdNCsub %d\n",i1);
    if(i1 == T_CW) {
      strcpy(buf, "CW");
      goto Fertig;
    }else if(i1 == T_CCW) {
      strcpy(buf, "CCW");
      goto Fertig;


    }else if(i1 == T_IN) {
      strcpy(buf, "IN");
      goto Fertig;
    }else if(i1 == T_OUT) {
      strcpy(buf, "OUT");
      goto Fertig;


    }else if(i1 == T_HI) {
      strcpy(buf, "HI");
      goto Fertig;
    }else if(i1 == T_LO) {
      strcpy(buf, "LO");
      goto Fertig;
    }else if(i1 == T_RGT) {
      strcpy(buf, "RI");
      goto Fertig;
    }else if(i1 == T_LFT) {
      strcpy(buf, "LE");
      goto Fertig;
    }
*/




  //-------------------------------------------------------
  }

/*
  if(typRec == Typ_apDat) {
    // applicationdata; alle restlichen daten raus.
    L_apDat1:
      // printf(" apDat-nxt: %d %d |%s|\n",*ind,aus_typ[*ind],txtTab[*ind]);
      if(strlen(buf) > 0) strcat(buf, " ");
      if(aus_typ[*ind] == Typ_String) {
        strcat(buf, "\"");
        strcat(buf, txtTab[*ind]);
        strcat(buf, "\"");
      } else {
        strcat(buf, txtTab[*ind]);
      }
      *ind = *ind + 1;
      if(aus_anz > *ind) goto L_apDat1;
      goto Fertig;
  }
*/
  goto L_Err;


  //__________________________________________________
  L_OK_P:  // "P(xx)" -> "xx"
    if(strncmp(txtTab[*ind], "P(", 2)) goto L_OK_2;
    p1 = txtTab[*ind];
    p1 += 2;
    strcat(buf, p1);
    UTX_del_right (buf, 1);      // remove last char ")"
        *ind = *ind + 1;
    goto Fertig;


  //__________________________________________________
  L_OK_2:
  if(strlen(buf) > 0) strcat(buf, " ");
  strcat(buf, txtTab[*ind]);
        *ind = *ind + 1;
  goto Fertig;




  L_OK_3:
  if(strlen(buf) > 0) strcat(buf, " ");
  strcat(buf, txtTab[*ind]);
  // goto Fertig;




  Fertig:
    printf("ex-IE_txt2par1 |%s| (aus)ind=%d\n",buf,*ind);
  return 1;






  //__________________________________________________
  L_Err:
  buf[0] = '\0';
    printf("ex-IE_txt2par1  ******* nix gefunden *********\n");
  return -1;

}

// EOF
