//**************************************************************************
/*
 *
 * Copyright (C) 2022 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
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

-----------------------------------------------------
Modifications:
-----------------------------------------------------
*/
#ifdef globTag
void GSRC(){}
#endif
/*!
\file  ../ut/ut_gsc.c
\brief Geometric-Source-Code from Geometric-Source-Relocatable-Code 
\code
=====================================================
List_functions_start:

GSRC_gsc_init
GSRC_gsc_gsrc_ln       translate GSRC into GSC
GSRC_eval__            process "typ<$[+#]> = .. typ# .."
GSRC_eval_i            replace "<$[+#]>" with db-index for typ

List_functions_end:
=====================================================

\endcode *//*----------------------------------------

*/
void INF_GSRC(){}
/*!
GSRC - Geometric-Source-Relocatable-Code
GSC  - Geometric-Source-Code


Purpose:
- add geometric objects (source) into model with already existing objects
  eg add object-source from extern

Examples:
  Supposing points to P44 exist, lines to L36 exits; curves to S28 exit;

  # create new line from existing points:
  # the following code sets the index of the next free line into symbol '$';
  #   then adds a new line between 2 existing points
  L<$>=P30 P31
  #   the rsulting GSC-code will be "L37=P30 P31"

  # create second line, using again the line-index '$';
  L<$+1>=P32 P33
  #   the rsulting GSC-code will be "L38=P32 P33"


  # create 3 new points
  P<$>=P(10,10 0)
  P<$+1>=P(20,10 0)
  P<$+2>=P(20,20 0)
  #   the resulting GSC-code will be "P45=P(10,10 0)", P46=.. P47=..

  # create polygon from this points:
  S<$>=POL P<$> P<$+1> P<$+2> P<$>
  #  result is "S29=P45 P46 P47"


Code-example see demo-program ../APP/Demo_gsrc.c

//----------------------------------------------------------------
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_geo.h"              // ObjAto
#include "../ut/ut_txt.h"              // UTX_pos_skipDeli1


static int indVAR, indVC, indPT, indLN, indCI, indCV, indSU;


//----------------------------------------------------------------
// prototypes
  char* GSRC_eval_i (char *p1, int typ);




//================================================================
  int GSRC_gsc_init () {
//================================================================
 
  indVAR = 0;
  indVC  = 0;
  indPT  = 0;
  indLN  = 0;
  indCI  = 0;
  indCV  = 0;
  indSU  = 0;

  return 0;

}


//================================================================
  int GSRC_gsc_gsrc_ln (char *sc, int siz_sc, char *src) {
//================================================================
// GSRC_gsc_gsrc_ln          translate GSRC into GSC

  int     irc;


  // printf("GSRC_gsc_gsrc_ln |%s|\n",src);

  sc[0] = '\0';

  // skip empty lines
  if(src[0] == '\0') goto L_exit;


  // skip comment-lines; copy "##"-Lines
  if(src[0] == '#') {
    if(src[1] == '#') {
      ++src;
      strcpy(sc, src);
    }
    goto L_exit;
  }

  if(strlen(src) >= siz_sc) goto L_err1;
  strcpy(sc, src);


  // process "V<VF[+#]>=NEW(P,20)"; change to eg "V2=44"; then add var into DB
  irc = GSRC_eval__ (sc, siz_sc);
  if(irc < 0) goto L_err2;


  L_exit:
      // printf("ex-_gsrc_ln |%s|\n",sc);
    return irc;


  //----------------------------------------------------------------
  L_err1:
    TX_Error("GSRC_gsc_gsrc_ln string too long");
    irc = -1;
    goto L_exit;

  L_err2:
    TX_Error("GSRC_gsc_gsrc_ln E2");
    irc = -2;
    goto L_exit;

  L_err3:
    TX_Error("GSRC_gsc_gsrc_ln E3");
    irc = -3;
    goto L_exit;

}


//================================================================
  int GSRC_eval__ (char *sc, int siz_sc) {
//================================================================
// GSRC_eval__            process "typ<$[+#]> = .. typ# .."
// typ can be V D P L C S A
// $ is the next free DB-index of typ
// replace "<..>" with the DB-index
// retCode:    0  sc succesful modified
//             <0 error

  int   irc = 0, iCod, iTyp, iInd, iOff, vOff;
  long  dbi, ival;
  char  *p1, *p2, cTyp;


  // loop tru string, find all "<$.>" and substitute
  p1 = sc;

  L_nxt:
    // get next p1 = '<$'  
    p1 = strstr (p1, "<$");
    if(!p1) goto L_OK;

    // get typ left of p1
    cTyp = *(p1 - 1);
    iTyp = AP_typ_typChar(cTyp);
    if(!iTyp) goto L_err;
      // printf(" iTyp=%d cTyp=%c\n",iTyp,cTyp);


    // replace "<$[+#]>" with db-index for typ 
    p1 = GSRC_eval_i (p1, iTyp);
    if(irc < 0) goto L_err;

    goto L_nxt;


  //----------------------------------------------------------------
  L_OK:
    irc = 0;

  L_exit:
      // printf(" ex-GSRC_eval_new %d |%s|\n",irc,sc);
    return irc;


  L_err:
    irc = -1;
    TX_Error("GSRC_eval_new - |%s|",sc);
    goto L_exit;

}


//================================================================
  char* GSRC_eval_i (char *p1, int typ) {
//================================================================
// GSRC_eval_i      replace "<$[+#]>" with db-index for typ 
// Input:
//   p1     string eg <$>  or <$+2> - starting with "<$"
//   typ    eg Typ_PT from "P<$>"

#define DBI_OFF  20      // start-db-index for  all types

  int    irc, sl, vOff;
  long   lTot, lNew, lDel;
  char   *p2, si[16];



  // get iOff
  p2 = p1 + 2; // skip "<$"
  while(*p2 == ' ') ++p2;
  if(*p2 == '+') {
    ++p2;
    vOff = atoi(p2);
  } else  vOff = 0;
  p2 = strchr (p2, '>');
  if(!p2) goto L_err;

  lDel = p2 - p1 + 1;
    // printf(" vOff=%d sl=%d |%s|\n",vOff,sl,p1);


  if(typ == Typ_VAR) {
    if(indVAR == 0) indVAR = DB_QueryNxtFree (typ, DBI_OFF);
    sprintf(si, "%d", indVAR + vOff);

  } else if(typ == Typ_VC) {
    if(indVC == 0) indVC = DB_QueryNxtFree (typ, DBI_OFF);
    sprintf(si, "%d", indVC + vOff);

  } else if(typ == Typ_PT) {
    if(indPT == 0) indPT = DB_QueryNxtFree (typ, DBI_OFF);
    sprintf(si, "%d", indPT + vOff);

  } else if(typ == Typ_LN) {
    if(indLN == 0) indLN = DB_QueryNxtFree (typ, DBI_OFF);
    sprintf(si, "%d", indLN + vOff);

  } else if(typ == Typ_CI) {
    if(indCI == 0) indCI = DB_QueryNxtFree (typ, DBI_OFF);
    sprintf(si, "%d", indCI + vOff);

  } else if(typ == Typ_CV) {
    if(indCV == 0) indCV = DB_QueryNxtFree (typ, DBI_OFF);
    sprintf(si, "%d", indCV + vOff);

  } else if(typ == Typ_SUR) {
    if(indSU == 0) indSU = DB_QueryNxtFree (typ, DBI_OFF);
    sprintf(si, "%d", indSU + vOff);

  } else goto L_err;


  // replace (p1 - p1+sl) by si
  lNew = strlen(si);
  lTot = strlen(p1);  // total length
  MEM_chg_rec (p1, &lTot, si, lNew, p1, lDel);


  L_exit:
      // printf(" ex-GSRC_eval_i |%s|\n",p1);
    return p2;
  

  L_err:
    p2 = NULL;
    TX_Error("GSRC_eval_i - |%s|",p1);
    goto L_exit;

}


// EOF
