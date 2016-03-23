//   functions for joints.                              2011-05-28     RF
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
2011-05-26 Erstmalig erstellt. RF.

-----------------------------------------------------
*/
#ifdef globTag
 void JNT(){}
#endif
/*!
\file  ../xa/xa_joint.c
\brief functions for joints 
\code
=====================================================
List_functions_start:

JNT_exp__             export joints into file "<tmp>/joints"
JNT_imp__             import joint from ObjGX into DBFile "joints"
JNT_imp_all           get a list of all joints for refModel <imdr>
JNT_imp_upd           update new object (note: get new text ..)
JNT_oTyp              get type of output-object for this joint-record
JNT_parent_hide       hide jointParentObject in subModel

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


The joints of all models are in one file: <temp>/joints.



*/




#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_geo.h"           // OFF, ON ..
#include "../ut/ut_os.h"            // OS_get_bas_dir
#include "xa.h"                     // AP_STAT



//================================================================
// EXTERNALS:

// aus xa.h:
extern AP_STAT   AP_stat;

// aus xa.c:
extern int       WC_modact_ind;        // the Nr of the active submodel; -1 = main.



//================================================================
  int JNT_parent_hide (int typ, long dbi, int imod) {
//================================================================
// JNT_parent_hide              hide jointParentObject in subModel

  // DL_unvis_set (8L, 1);
  // DL_hide__ (8L, OFF);  geht ned ..
 
  long    dli;
  // ObjGX   *jnt;


  // printf("JNT_parent_hide typ=%d dbi=%ld imod=%d\n",typ,dbi,imod);


  // get type & dbi of jointObj
  dli = DL_find_smObj (typ, dbi, -1L, imod);
  if(dli >= 0) {
      // printf(" _parent_hide dli=%ld\n",dli);
    GL_Del0 (dli);
  }

  return 0;
}


//================================================================
  int JNT_imp__ (Memspc *spcObj, int *ibmNr, long irmNr, int iJnt) {
//================================================================
// import joint from DBFile "<tmp>/joints"
// Input:
//   irmNr   RefModelNr
//   iJnt    jointNr
// Output:
//   spcObj  the first ObjGX in spcObj ist the resulting outputObj
//   ibmNr   basiModelNr of irmNr


  int     irc, imb, lv;
  char    key[240];
  void    *vp1;
  ObjGX   *jnt;


  // printf("JNT_imp__ rmNr=%ld ij=%d\n",irmNr,iJnt);
  // DBF_dump ();


  // get BaseModelNr of M<iMdl>
  *ibmNr = DB_get_iModBas_iModRef (irmNr);
  if(*ibmNr < 0) {
    TX_Error("JNT_imp__ E001 %d",irmNr);
    return -1;
  }
    // printf(" ibmNr=%d\n",*ibmNr);


  // create key; eg "M123J20"
  sprintf(key, "M%dJ%d", *ibmNr, iJnt);


  // find key
  irc = DBF_find__ (&lv, key, strlen(key));
  if(irc < 0) {
    TX_Print("ERROR JNT_imp__ key=|%s|",key);
    return -1;
  }

  vp1 = UME_reserve (spcObj, lv);      // reserve space in Memspc
  if(!vp1) { TX_Print("ERROR JNT_imp__ EOM "); return -1; }


  // load value
  DBF_getVal (vp1, &lv);

  // relocate
  OGX_reloc__ (vp1, 0);

  // get primary object
  // jnt = UME_get_start (spcObj);
  jnt = vp1;
    // UTO_dump__ (jnt, "JNT_imp__");
    // UTO_dump_s_ (jnt, "JNT_imp__");


  //----------------------------------------------------------------


  return 0;

}


//================================================================
  int JNT_exp__ (long indJnt, Memspc *spcObj) {
//================================================================
// export joints into DBFile "<tmp>/joints"
// Input:
//   jnt  objects; data are in memspc201


  int     sizKey, sizVal;
  char    key[240];
  ObjGX   *jnt;
 

  jnt = UME_get_start (spcObj);


  // printf("JNT_exp__ indJnt=%d\n",indJnt);
  // UTO_dump__ (jnt, "JNT_exp__");
  // UTO_dump_s_ (jnt, "APT_decode_Joint");
  // printf(" WC_modact_ind=%d\n",WC_modact_ind);



  //----------------------------------------------------------------
  // delocate
  OGX_deloc__ (jnt, UME_get_used (spcObj));


  //----------------------------------------------------------------
  // open / init DBFile "<tmp>/joints"
  if(!AP_stat.jntStat) {
    // open DBFile "<tmp>/joints"
    sprintf(key, "%sjoints", OS_get_tmp_dir());
    if(DBF_init (key) < 0) {
      TX_Error ("JNT_exp__ E000");
      return -1;
    }
    AP_stat.jntStat = 1;
  }



  //----------------------------------------------------------------
  // create key; eg "M123J20"
  sprintf(key, "M%dJ%ld", WC_modact_ind, indJnt);
  sizKey = strlen(key);

  // size of data
  sizVal = UME_get_used (spcObj);
    // printf(" sizKey=%d key=|%s| sizVal=%d\n",sizKey,key,sizVal);

  // save key & data
  DBF_add__ (key, sizKey, jnt, sizVal);


  //----------------------------------------------------------------
  // report last used jointNr 
  DB_StoreJointNr (indJnt);


  return 0;

}


//================================================================
  int JNT_imp_upd (char *sCode, int sSiz, Memspc *spcObj, int ij) {
//================================================================
// JNT_imp_upd     // update new object (note: get new text ..)
//   Input:
//     ij        joint-nr
//   Output:
//     sCode     Note: new text
//   RetCod      0  no additional source
//               else add to sourceline ..

  int       irc, iOut;
  ObjGX     *ox1;
  char      sText[200], s1[64];
  char      *buttons[]={"OK",NULL};


  // printf("JNT_imp_upd %d\n",sSiz);


  ox1 = UME_get_start (spcObj);
  iOut = 0;
 

  //----------------------------------------------------------------
  if(ox1->typ != Typ_Note) goto L_exit;

  // get new text from user ...
  sText[0] = '\0';
  sprintf(s1, "enter new text for joint %d..", ij);
  irc = GUI_DialogEntry (s1, sText, sizeof(sText), buttons, 2);
    // printf(" DialogWait %d |%s| %d\n",irc, sText, sizText);

  // copy text -> sCode
  if(strlen(sText) < sSiz + 3) {
    sprintf(sCode, " \"%s\"", sText);
  } else {
    TX_Print("JNT_imp_upd E001");
    return 0;
  }

  // hide original text in submodel
  // geht hier nicht mehr - subModel ist bereits fertig gespeichert.

  iOut = 1;
    

  //----------------------------------------------------------------
  L_exit:
  return iOut;
    
}


//================================================================
  int JNT_imp_all (int *jTab, int *sizTab, long imdr) {
//================================================================
// JNT_imp_all           get a list of all joints for refModel <imdr>

  int   ii, i1, lsk, lsfk, lv, imax;
  char  s1[240], sk[16], *sfk, *p1;


  // printf("JNT_imp_all %d %ld\n",*sizTab, imdr);
  // DBF_dump ();


  // get basicModelNr of refModel <imdr>
  ii = Mod_mdbi_mdri (imdr);
  sprintf(sk, "M%dJ", ii);
    // printf(" ii=%d sk=|%s|\n",ii,sk);


  // find all records starting with <sk>
  ii = 0;
  imax = *sizTab;
  *sizTab = 0;
  lsk = strlen(sk);

  while (DBF_find_nxt (&sfk, &lsfk, &lv, sk, lsk, &ii) >= 0) {
    i1 = sizeof(s1); 
    DBF_getVal ((void*)s1, &i1);           // get value -> s1    UNNUTZ !
      // printf(" _find_nxt found ii=%d |%s| %d\n", ii, sfk, lsfk);
    // find 'J' in key; the following nr is the joint-nr
    p1 = strchr (sfk, 'J');
    ++p1;
    jTab[*sizTab] = atoi (p1);
    *sizTab += 1;
    if(*sizTab >= imax) return -1;
  }


    // printf("ex JNT_imp_all %d\n",*sizTab);
    // for(ii=0;ii<*sizTab;++ii)printf("%d,",jTab[ii]);
    // printf("\n");


  return 0;

}

 
//================================================================
  int JNT_oTyp (Memspc *spcObj) {
//================================================================
// JNT_oTyp              get type of output-object for this joint-record

// see also APT_IMP__

  ObjGX     *ox1;

  ox1 = UME_get_start (spcObj);

  if(ox1->typ == Typ_PT) {
    return Typ_PT;

  } else if(ox1->typ == Typ_Note) {
    return Typ_Note;
  }


  return Typ_Error;

}


// EOF
