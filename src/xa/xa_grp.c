//  Group-functions.        Franz Reiter.  10.jan.2006
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
2006-01-10 Erstmalig erstellt. RF.

-----------------------------------------------------
*/
#ifdef globTag
 void Grp(){}
#endif
/*!
\file  ../xa/xa_grp.c
\brief  group functions (Grp_)
\code
=====================================================
List_functions_start:

Grp_ck_def       check if obj is in group

Grp_init         init obj-list
Grp_Start        start filling group
Grp_Clear        clear (reset) group
Grp_Inv          invert group
Grp_add__        add obj to obj-list
Grp_add_all1     add all objs to group (without hidden)
Grp_del          remove obj from group
Grp_typ_del      remove all objs of type from group
Grp_ck_def       check if obj is in group
Grp_exp          export (native) alle objects of obj-list --> file
Grp_res          resolv list and add all parents to list
Grp_cbuf         return list ob objs of group as text
Grp_hide         hide all objs of grp
Grp_upd          Redraw &| update GrpNr-label

Grp_get__        returns GrpTab & GrpNr
Grp_get_nr       return nr of objs in group
Grp_dump

Grp_alloc
Grp_realloc

List_functions_end:
=====================================================
// Grp_dlAdd        add all objs of group to DL-grp1

see also
DL_grp1_copy      copy all DL-obj with groupBit ON --> GroupList
UI_grp__
UI_grpAdd


\endcode *//*
----------------------------------------


Funktionsweise:
Mit Add to Group wird nur in der DL das Groupbit gesetzt;
damit (allein) werden GroupObjs gehilitet dargestellt.
Alle GroupObjs werden zusätzlich in GrpTab geführt.



Save_Group_to_File:
  DL_grp1_copy       Grp_init; then copy all objs with grp=ON via Grp_add__
  Grp_res           add all parents of all objs --> Liste
  Grp_exp           write alle Definitionlines if its obj is in grp.

man kann sortiert/unsortiert (betreff Selektionsreihenfolge!) ausgeben:
  DL_grp1_copy macht eine unsortierte Gruppe;
  mit Grp_add__ wird dzt auch (sortierte)) Gruppe GrpTab mitgeschrieben.


*/

#ifdef _MSC_VER
#include "MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                         // for ...

#include <ctype.h>                          // f isdigit ..


#include "../gui/gui_types.h"           // UI_Func..

#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_txt.h"              // fnam_del
// #include "../ut/ut_os.h"               // OS_get_bas_dir ..
// #include "../ut/ut_obj.h"              // UTO_stru_2_obj
#include "../ut/ut_txfil.h"            // UTF_GetPosLnr
#include "../ut/ut_os.h"               // OS_ ..
#include "../ut/ut_cast.h"             // PTR_INT
#include "../ut/ut_TX.h"               // TX_Print
// #include "../ut/func_types.h"          // FUNC_..

#include "../gr/ut_DL.h"               // DL_GetAtt
#include "../gr/ut_GL.h"               // GL_GetCen
// #include "../db/ut_DB.h"               // DB_GetGTxt

#include "xa.h"                        // aus_SIZ
#include "../xa/xa_mem.h"              // memspc51, mem_cbuf1
#include "../xa/xa_uid.h"              // UID_ouf_grpNr




//============ Extern Var: =====================


//============ Local Var: =====================
static ObjDB    *GrpTab = NULL;
static int      GrpMax = 0;
static int      GrpNr  = 0;



//================================================================
  int Grp_get__ (ObjDB **oTab) {
//================================================================
/// \code
/// Grp_get__     returns GrpTab & GrpNr
/// RetCod = nr of objects in group
/// \endcode

  *oTab = GrpTab;
  return GrpNr;

}
 

//================================================================
  int Grp_Start () {
//================================================================
// Grp_Start        start filling group


  // printf("Grp_Start \n");


  DL_grp1_set (-1L, OFF);           // clear all groupBits

  // if(UI_GR_Sel_Filter(-1) != 5)     // nochmal ware Reset !
    // UI_GR_Sel_Filter (5);           // activate add to group

  DL_hili_off (-1L);               // unhilite alle Objekte
  // DL_Redraw ();

  return 0;

}


//================================================================
  int Grp_HiliClear () {
//================================================================
/// unhilite all group-objects

  int    i1;


  for(i1=0; i1<GrpNr; ++i1) {
    DL_hili_off (GrpTab[i1].dlInd);
  }

  return 0;

}


//================================================================
  int Grp_SM () {
//================================================================
// move group -> subModel
// see Mod_m2s__ - (main -> subModel)
// see Grp_Mdl   - (save Group -> File)

  int    irc;
  char   s1[256], smNam[128];


  printf("Grp_SM \n");

  // exit if no greoup exists
  if(GrpNr < 1) {TX_Print(" .. no group exists .."); return 0;}


  //----------------------------------------------------------------
  // get smNam = new subModelname
  irc = Mod_smNam_get (smNam);
  if(irc < 0) return -1;
    printf(" smNam=|%s|\n",smNam);



  //----------------------------------------------------------------
  // - write group into file
  // prepare APTsource: MAN-Mode: copy Edi --> memory.
  AP_SRC_mem_edi ();

  // copy all DL-obj with groupBit ON --> GroupList.
  DL_grp1_copy ();

  // obj-list Recursiv nach seinen Eltern absuchen und diese zufuegen
  irc = Grp_res ();
  if(irc <= 0) {TX_Print("no obj selected ..."); return -1;}

  // export (native) alle objects of obj-list --> file tmpDir/selection1.gcad
  strcpy(s1,"selection1");
  Grp_exp (s1, OS_get_tmp_dir());


  //----------------------------------------------------------------
  // create subModel smNam from file tmpDir/selection1.gcad
  sprintf(s1, "%sselection1.gcad", OS_get_tmp_dir());
  irc = Mod_SM_add_file (smNam, s1);
  if(irc < 0) return -1;


  //----------------------------------------------------------------
  // delete group
  Del_grp__ ();


  return 0;

}

 
//================================================================
  int Grp_Mdl () {
//================================================================
// save Group -> modelFile

  int   irc;

  // prepare APTsource: MAN-Mode: copy Edi --> memory.
  AP_SRC_mem_edi ();

  // copy all DL-obj with groupBit ON --> GroupList.
  DL_grp1_copy ();

  // obj-list Recursiv nach seinen Eltern absuchen und diese zufuegen
  irc = Grp_res ();
  if(irc <= 0) {TX_Print("no obj selected ..."); return -1;}

  // export (native) alle objects of obj-list --> file
  AP_save__ (1, "gcad");

  return 0;

}

 
//================================================================
  int Grp_Inv (int mode) {
//================================================================
/// \code
/// Grp_Inv          invert group
/// Input:
///   mode    2 unhili&redraw
///           1 Redraw
///           0 do not Redraw
/// \endcode


  printf("Grp_Inv %d\n",GrpNr);

  if(GrpNr < 1) return 0;

  // if(mode == 2) Grp_HiliClear ();  // unhilite all group-objects

  // clear GroupBits
  DL_grp1_set (-1L, -1);           // invert all groupBits

  // get GrpNr
  // GrpNr = DL_grp1_nr_get ();


  // DL_hili_off (-1L);   // unhilite alle Objekte  raus 2011-10-03


  if(mode) {
    DL_Redraw ();     // redraw
    // display nr of objs in group
    UI_AP (UI_FuncSet, UID_ouf_grpNr, PTR_INT(GrpNr));
  }

  return 0;

}


//================================================================
   int Grp_Clear (int mode) {
//================================================================
/// \code
/// Grp_Clear        clear (reset) group
/// Input:
///   mode    2 unhili&redraw
///           1 Redraw
///           0 do not Redraw
/// \endcode

  int    i1;


  // printf("Grp_Clear %d\n",GrpNr);

  if(GrpNr < 1) return 0;

  if(mode == 2) Grp_HiliClear ();  // unhilite all group-objects

  // clear GroupBits
  // DL_grp1_set (-1L, OFF);           // clear all groupBits
  for(i1=0; i1<GrpNr; ++i1) {
    DL_grp1_set (GrpTab[i1].dlInd, OFF);
  }

  Grp_init ();          // reset group
  // UI_GR_Sel_Filter (0);             // reset add to group
  // DL_hili_off (-1L);   // unhilite alle Objekte  raus 2011-10-03


  if(mode) DL_Redraw ();     // redraw

  return 0;

}


//================================================================
  int Grp_init () {
//================================================================
/// init obj-list


  // printf("Grp_init \n");

  // GrpTab = (void*)memspc55;
  // GrpMax = sizeof(memspc55) / sizeof(ObjDB) - 2;

  GrpNr = 0;

  // UI_AP (UI_FuncSet, UID_ouf_grpNr, (void*)GrpNr);
  // display nr of objs in group
  UI_AP (UI_FuncSet, UID_ouf_grpNr, PTR_INT(GrpNr));

  return 0;

}


//================================================================
  int Grp_alloc () {
//================================================================
// verkleinern

  if(GrpMax < 11000) return 0;

  GrpNr  = 0;
  GrpMax = 0;

  return Grp_realloc ();   // reInit
    
}


//================================================================
  int Grp_realloc () {
//================================================================
 
  long  newSiz;

  GrpMax += 5000;

  newSiz = sizeof(ObjDB) * (GrpMax + 2);

  GrpTab = (ObjDB*)realloc(GrpTab, newSiz);

  if(GrpTab) return 0;

  return -1;

}


//================================================================
  int Grp_dump () {
//================================================================

  int   i1;
  char  s1[256], s2[32];

  sprintf(s1," %d objects in group ==============", GrpNr);
  DEB_dump_obj__(Typ_Txt, s1, "");


  for(i1=0; i1<GrpNr; ++i1) {

    APED_oid_dbo__ (s2, GrpTab[i1].typ,GrpTab[i1].dbInd);

    sprintf(s1,"[%4d] %-12s  typ=%d dbi=%ld dli=%ld stat=%d",i1,s2,
      GrpTab[i1].typ,GrpTab[i1].dbInd,GrpTab[i1].dlInd,GrpTab[i1].stat);

    DEB_dump_obj__(Typ_Txt, s1, "");
  }

  return 0;

}


//================================================================
  int Grp_cbuf (char *cBuf, int cSiz) {
//================================================================
// Grp_cbuf         return list ob objs of group as text

  int  i1, maxSiz;
  char nbuf[32];

  cBuf[0] = '\0';
  maxSiz = cSiz - 32;

  // printf("Grp_cbuf %d\n",GrpNr);

  for(i1=0; i1<GrpNr; ++i1) {
      // printf("Grp_cbuf [%d] typ=%d ind=%ld\n",i1,
                 // GrpTab[i1].typ,GrpTab[i1].dbInd);

    APED_oid_dbo__ (nbuf, GrpTab[i1].typ,GrpTab[i1].dbInd);
      // printf(" nbuf=|%s|\n",nbuf);


    if(strlen(cBuf) >= maxSiz) {TX_Error("Grp_cbuf E001"); return -1;}

    if(i1 > 0) strcat(cBuf, " ");
    strcat(cBuf, nbuf);

  }

  return 0;

}


//================================================================
  int Grp_add__ (int typ, long dbi, long dli, int iUpd) {
//================================================================
/// \code
/// add obj to GrpTab
/// Input:
///   dli      -1 no dli (obj not visible)
///            -2 dli unknown (search)
///   iUpd     0 = update display;
///            1 = do not update display (yet)
///            2 = do not update display and do not test if already defined
///
///  see also DL_grp1__ (add obj to DL-group1 (hiliete) and to GrpTab)
/// \endcode

  int    i1;


  // printf("Grp_add__ typ=%d dbi=%ld dli=%ld iUpd=%d %d\n",
                       // typ,dbi,dli,iUpd,GrpNr);
  // printf("  GR_Sel_Filter=%d\n",UI_GR_Sel_Filter(-1));


  // testen, ob obj nicht bereits definiert ..
  if(iUpd != 2) {
    for(i1=0; i1<GrpNr; ++i1) {
      if(GrpTab[i1].typ != typ) continue;
      if(GrpTab[i1].dbInd == dbi) return 0;
    }
  }


  if(GrpNr >= GrpMax) {
    if(Grp_realloc() < 0) {
      TX_Error("Grp_add__ E001");
      return -1;
    }
  }

  // search unknown dli
  if(dli == -2L) {
    dli = DL_find_obj (typ, dbi, -1L);
  }


  GrpTab[GrpNr].typ  = typ;
  GrpTab[GrpNr].dbInd  = dbi;
  GrpTab[GrpNr].dlInd  = dli;
  GrpTab[GrpNr].stat = 0;

  ++GrpNr;

  // display nr of objs in group
  if(iUpd == 0)
  UI_AP (UI_FuncSet, UID_ouf_grpNr, PTR_INT(GrpNr));
  // UI_AP (UI_FuncSet, UID_ouf_grpNr, (void*)GrpNr);

  return 0;

}


//================================================================
  int Grp_add_all1 (int mod) {
//================================================================
/// Grp_add_all1                add all objs to group (without hidden)
/// Input:
///   mod     modelNr to resolv;    -1 = mainModel



  long      l1, dlNr;
  DL_Att    *dla;


  // get dispList
  dlNr = DL_get__ (&dla);



  for(l1=0; l1<dlNr; ++l1) {

    // skip model
    if((INT_16)dla[l1].modInd != mod) continue;

    // skip hidden
    if(DL_OBJ_IS_HIDDEN(dla[l1])) continue;


    if(GrpNr >= GrpMax) {
      if(Grp_realloc() < 0) {
        TX_Error("Grp_add_all1 E001");
        return -1;
      }
    }


    GrpTab[GrpNr].typ    = dla[l1].typ;
    GrpTab[GrpNr].dbInd  = dla[l1].ind;
    GrpTab[GrpNr].dlInd  = l1;
    GrpTab[GrpNr].stat = 0;

    ++GrpNr;
  }



  return 0;

}


//================================================================
  int Grp_del (int typ, long ind, int iUpd) {
//================================================================
/// remove obj from group
///   iUpd     0=update display; 1=do not update display (yet)

  int    i1;


  // printf("Grp_del %d %ld %d\n",typ,ind,GrpNr);


  // remove last entry
  if(typ == Typ_last) {
    if(GrpNr < 1) return -1;
    // unhilite last obj in grp
    DL_grp1_set (GrpTab[GrpNr - 1].dlInd, OFF);
    --GrpNr;
    goto L_disp;
  }


  // testen, ob obj nicht bereits definiert ..
  for(i1=0; i1<GrpNr; ++i1) {
    if(GrpTab[i1].typ != typ) continue;
    if(GrpTab[i1].dbInd == ind) goto L_remove;
  }
  printf("Grp_del E001 %d %ld\n",typ,ind);
  return 0;


  // remove Record # i1 from Group
  L_remove:
  MEM_del_nrec (&GrpNr, GrpTab, i1, 1, sizeof(ObjDB));
    // printf(" GrpNr now %d\n",GrpNr);


  // display nr of objs in group
  L_disp:
  if(iUpd == 0)
  UI_AP (UI_FuncSet, UID_ouf_grpNr, PTR_INT(GrpNr));
  // UI_AP (UI_FuncSet, UID_ouf_grpNr, (void*)GrpNr);

  return 0;

}

//================================================================
   int Grp_typo_del (int typ) {
//================================================================
// Grp_typo_del      remove all other types from group

  int    i0, i1;


  // printf("Grp_typo_del %d\n",typ);

  // loop tru grp
  i0 = 0;
  L_noAmoi:
  for(i1=i0; i1<GrpNr; ++i1) { 
    if(GrpTab[i1].typ == typ) continue;
    i0 = i1;
    // remove this obj ..
    DL_grp1__ (GrpTab[i1].dlInd, NULL, -1, 1);
    goto L_noAmoi;
  }
  Grp_upd (0);


  return 0;

}


//================================================================
   int Grp_typ_del (int typ) {
//================================================================
// Grp_typ_del      remove all objs of type from group

  int    i0, i1;


  // printf("Grp_typ_del %d\n",typ);

  // loop tru grp
  i0 = 0;
  L_noAmoi:
  for(i1=i0; i1<GrpNr; ++i1) {
    if(GrpTab[i1].typ != typ) continue;
    i0 = i1;
    // remove this obj ..
    DL_grp1__ (GrpTab[i1].dlInd, NULL, -1, 1);
    goto L_noAmoi;
  }
  Grp_upd (0);

  return 0;

}


//================================================================
  int Grp_res () {
//================================================================
/// \code
/// obj-list Recursiv nach seinen Eltern absuchen und diese zufuegen
/// RetCod:  nr of objs in group (GrpTab)
/// 
/// GrpTab[].stat = 0: newly added
/// GrpTab[].stat = 1: all parents searched and added ..
///
/// TODO: use APED_find_par__
/// \endcode


  int    irc, gInd, lLen, iEnd;
  long   lNr;
  char   oNam[32], *cPos;


  // printf("Grp_res \n");
  if(!GrpTab) return -1;

  L_nxt_run:
  gInd = 0;

  L_nxt_obj:
  if(GrpTab[gInd].stat > 0) goto L_weiter;
    // printf(" res obj [%d] typ=%d ind=%d stat=%d\n",gInd,
      // GrpTab[gInd].typ,GrpTab[gInd].ind,GrpTab[gInd].stat);


  // find ALL Definitionlines of obj, find all its componentI's, add its
  // components to List.

  // create objName
  APED_oid_dbo__ (oNam, GrpTab[gInd].typ,GrpTab[gInd].dbInd);

  iEnd = -1;

  //=========== Search all DefinitionLines of obj oNam
  L_nxt:
  irc = APED_search_defLn (&cPos, &lNr, &lLen, oNam, iEnd, 0);

  if(irc >= 0) {     // kein (weitere) DefLn gefunden

    // die gefundene Line decodieren, alle darin enthalten Objekte in die
    // Liste zufuegen.
    APED_decAdd (cPos);

    iEnd = lNr;
    goto L_nxt;
  }
  //===========


  GrpTab[gInd].stat = 1;
  

  L_weiter:
  ++gInd;
  if(gInd < GrpNr) goto L_nxt_obj;


  // Testausg:
  // Grp_dump ();


  return GrpNr;

}


/*
//================================================================
  int Grp_dlAdd () {
//================================================================
// Grp_dlAdd        add all objs of group to DL-grp1

  int    i1;

  for(i1=0; i1<GrpNr; ++i1) {
    DL_grp1_set (GrpTab[i1].dlInd, ON);
  }
  DL_Redraw (); // hilite / unhilite
  return 0;

}
*/

//================================================================
  int Grp_hide (int mode) {
//================================================================
// Grp_hide         hide all objs of grp
// Input:
//   mode      0=hide;  1=view
// see also UI_GR_Select_work2


  int   i1;

  // printf("Grp_hide %d\n",GrpNr);


  // HIDE
  if(mode == 0) {
    for(i1=0; i1<GrpNr; ++i1) {
      DL_grp1_set (GrpTab[i1].dlInd, OFF);
      DL_hili_off (GrpTab[i1].dlInd);
      GA_view__ (GrpTab[i1].dlInd, 3, GrpTab[i1].typ, GrpTab[i1].dbInd);
      // GA_hide__ ??
    }


  // VIEW
  } else {
    for(i1=0; i1<GrpNr; ++i1) {
      DL_grp1_set (GrpTab[i1].dlInd, OFF);
      // DL_hili_off (GrpTab[i1].dlInd);
      GA_view__ (GrpTab[i1].dlInd, 2, GrpTab[i1].typ, GrpTab[i1].dbInd);
    }



  }


  Grp_init ();      // reset group

  DL_Redraw ();


  return 0;

}


//================================================================
  int Grp_ck_def (int typ, long dbi) {
//================================================================
/// \code
/// check if obj is in group
/// RetCod >= 0: yes, RetCod = index.
///          -1: no.
/// \endcode

  int   i1, irc=-1;

  for(i1=0; i1<GrpNr; ++i1) {
    if(GrpTab[i1].typ != typ) continue;
    if(GrpTab[i1].dbInd != dbi) continue;
    irc = i1;
    break;
  }

  // printf("ex Grp_ck_def %d %d %d %d\n",irc,typ,dbi,GrpNr);

  return irc;
}
 

//================================================================
  int Grp_exp (char *fnam,char *dirNam) {
//================================================================
/// \code
/// export (native) alle objects of obj-list --> file
///   fnam        <filename>[.<filetyp>]
///   dirNam      absolute
/// \endcode

//----------------------------------------------------------------
// Zuerst nach Buffer1 rausschreiben, dann in Datei ausgeben.
// alle Sourcezeilen der Reihe nach durchgehen;
// wenn es Definitionline ist: check ob Obj in der Gruppe enthalten ist;
// wenn ja: ausgeben.

  int    i1, i2, typ;
  long   lNr, llen, ind;
  char   *cpos, cBuf[256], cNam[32], *p2;
  FILE   *fpo;

  if(fnam == NULL) return 0;   // cancel



  // printf("Grp_exp |%s|%s|\n",dirNam,fnam);

  // remove Filetyp from fnam
  UTX_ftyp_cut (fnam);

  sprintf(cBuf, "%s%s.gcad",dirNam,fnam);

  // printf(" file=|%s|\n",cBuf);
  TX_Print("save to File %s",cBuf);


  //----------------------------------------------------------------
  // clear Buffer1.
  UTF_clear1 ();


  // add all defLines --> Buffer1.
  lNr = 1;

  L_nxt_ln:
  cpos = UTF_GetPosLnr (&llen, lNr);
  if(cpos == NULL) goto L_eof;
    // printf(" nxt ln %ld |",lNr);UTX_dump_cnl(cpos, 50);printf("|\n");

  // check for definitionline, defTyp and defInd
  if(APED_ck_defLn (&p2, cpos) != 0) goto L_weiter;

  // decode definitionObject
  AP_cut_defHdr (&typ, &ind, cpos);
    // printf(" _defHdr %d %ld\n",typ,ind);

  // check if obj is in group
  if(Grp_ck_def (typ, ind) < 0) goto L_weiter;

  // add this line to Buffer1.
  UTF_add1_strLF (cpos);

  L_weiter:
  ++lNr;
  goto L_nxt_ln;



  //----------------------------------------------------------------
  L_eof:
  // UTF_dump1__ ();

  // nun ist der gesamte APT-code in Buffer1; Filename in cBuf.
  // Save Buffer1 --> File "cBuf".
  // Hidelist: Filter with Group.


  // Das Outputfile oeffnen
  if ((fpo = fopen (cBuf, "w")) == NULL) {
    sprintf(mem_cbuf1, "beim Oeffen der Datei %s ****",cBuf);
    TX_Error (mem_cbuf1);
    return 0;
  }


  // write MODSIZ DEFTX DEFCOL VIEW CONST_PL; not MODBOX
  DL_wri_dynDat1 (fpo, 1);

  // add GA-Table (HIDE/G#/..)
  GA_fil_wri (fpo, 1, 0, 1);

  fprintf(fpo, ":DYNAMIC_DATA\n");


  fprintf(fpo, "# %s\n", OS_date1());

  // Buffer1 als eine Block rausschreiben ..
  UTF_file_Buf1_w (fpo);

  fclose (fpo);

  return 0;

}


//================================================================
  int Grp_upd (int mode) {
//================================================================
// Grp_upd          Redraw &| update GrpNr-label
// mode      0=Redraw & update GrpNr-label
//           1=update GrpNr-label


  if(mode == 0) DL_Redraw ();     // redraw


  // update GrpNr-label
  // UI_AP (UI_FuncSet, UID_ouf_grpNr, (void*)GrpNr);
  UI_AP (UI_FuncSet, UID_ouf_grpNr, PTR_INT(GrpNr));

  return 0;

}


//================================================================
  int Grp_get_nr () {
//================================================================
// Grp_get_nr       return nr of objs in group

  // printf("Grp_get_nr %d\n",GrpNr);

  return GrpNr;

}


//================================================================
