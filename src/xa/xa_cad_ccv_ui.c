//    ../xa/xa_cad_ccv_ui.c                     CAD-CCV-Editor, CAD-GroupEditor
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
Group-Edit - Window

TODO:
  ..

-----------------------------------------------------
Modifications:
2014-05-27  Complete reworked. IE_ed1 -> IE_ccv. RF.
2014-04-10  Extracted from xa_cad_ui.c. RF.

-----------------------------------------------------
*/
/*!
\file  ../xa/xa_cad_ccv_ui.c
\brief GUI simple CurveEditor
\code
=====================================================
List_functions_start:

IE_ccv__             create Window; simple CurveEditor
IE_ccv_mod           GroupEditor ?
// IE_ccv_CMB           Mouseklick into GroupEditWindow
IE_ccv_CB
IE_ccv_rem           remove word out of IE_buf
IE_ccv_cw            modify IE_buf add|change|delete word

List_functions_end:
=====================================================

\endcode *//*----------------------------------------




Offline testen:
cc xa_cad_ed.c ../ut/ut_TX.o ../ut/ut_txt.o -DOFFLINE&&a.out

==============================================================

*/

#ifdef _MSC_VER
#include "MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_geo.h"
#include "../ut/ut_txt.h"

#include "../xa/xa_sele.h"        // Typ_go*
#include "../xa/xa_uid.h"               // DLI_DIR_TMP

#include "../gui/gui__.h"         // Gtk


#define   TRUE 1
#define   FALSE 0


static int winTyp;   // 0=std (with REV & MOD)
                     // 1=grp (without REV & MOD)

// PROTOS:
  int IE_ccv__ (MemObj *mo, void **data);
  // int IE_ccv_keyb (MemObj *mo, void **data);



//========================================================
//========================================================
#ifdef OFFLINE

// NUR BEI TESTS !!
#define IE_bufMax  1024
char IE_buf[IE_bufMax];
long IE_bufSiz;

  int main () {


  long cPos;


  strcpy(IE_buf, "");
  IE_bufSiz=strlen(IE_buf);
  IE_ccv_cw (&cPos, "P20");        // Start
  // IE_ccv_cw (&cPos, "P(0 0)");        // Start
  IE_ccv_cw (&cPos, "DEL");        // remove C20
  // IE_ccv_cw (&cPos, "C20");        // add
  exit(0);



  // test add
  strcpy(IE_buf, "P20 C20");
  IE_bufSiz=strlen(IE_buf);
  cPos=strlen(IE_buf);



  // add od change word at Pos cPos.
  // IE_ccv_cw (&cPos, "DEL");        // remove C20
  // IE_ccv_cw (&cPos, "CW");
  // IE_ccv_cw (&cPos, "CCW");       // change vorheriges CW
  // IE_ccv_cw (&cPos, "MOD+");      // add MOD(1)
  // IE_ccv_cw (&cPos, "MOD+");      // change -> MOD(2)
  // IE_ccv_cw (&cPos, "MOD-");



  // test insert CW
  strcpy(IE_buf, "P20 C20 P21");
  IE_bufSiz=strlen(IE_buf);
  cPos=IE_bufSiz-4;             // nach CW muss mit -3 u -4 gehen

  // IE_ccv_cw (&cPos, "CW");       // insert CW
  // IE_ccv_cw (&cPos, "DEL");     // remove CW
  // IE_ccv_cw (&cPos, "CCW");     // change CW -> CCW
  // IE_ccv_cw (&cPos, "CW");



  // test insert MOD
  strcpy(IE_buf, "P20 C20 P21");
  IE_bufSiz=strlen(IE_buf);
  cPos=IE_bufSiz-3;             // nach CW muss mit -3 u -4 gehen

  // IE_ccv_cw (&cPos, "DEL");        // remove MOD(1)
  IE_ccv_cw (&cPos, "MOD+");     // ins o change MOD(1) -> MOD(2)
  // IE_ccv_cw (&cPos, "MOD-");



}

//========================================================
//========================================================
#else

// Externals:
// from ../xa/xa_cad_ui.c
extern MemObj    IE_edWin1;
extern long      IE_bufMax;
extern long      IE_bufSiz;
extern int       IE_modify;          // 0=Add 1=Modify 2=Insert

// ex xa.c / ../xa/xa_mem.h
extern char IE_buf[];

// aus ../ci/NC_Main.c:
extern int     APT_dispDir;



#endif




//================================================================
  void IE_ccv_set_winTyp (int typ) {
//================================================================
// 0=std (with REV & MOD)
// 1=grp (without REV & MOD)

  winTyp = typ;

}


//=====================================================================
  int IE_ccv_mod (MemObj *mo, void **data) {
//=====================================================================
// Hilfsfunktionen GruppenWindow.
// Functions ((char*)data):
// DEL      delete word links vom Cursor.
// MOD+ MOD-
// PREV-    Cursor ein Wort nach links bewegen und redisplay bis Cursor
// PREV+    Cursor ein Wort nach rechts bewegen und redisplay bis Cursor
// PREVIEW  click into window; curpos setzen und redisplay bis Cursor
// Exit
// else     add (Char*)data into window.

// Der primary Code befindet sich im EditWindow.
// er wird nach IE_buf kopiert. dort wird er modifiziert; (InsertWord,
//   DeleteWord.
// Weiters wird die Curpos im EditWindow und IE_buf gesetzt.
// Vom IE_buf wird der Rest rechts von Curpos abgetrennt,
//   dann wird IE_buf angezeigt.





// static long oldPos;

  int      i1;
  long     cPos, dPos;
  char     *txt;


  // printf("IE_ccv_mod \n");

  if(!GUI_OBJ_IS_VALID(&IE_edWin1))  return -1;


  txt = GUI_DATA_S1;
    // printf("IE_ccv_mod  /%s/\n",txt);

  // IE_ccv__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB1));   // raise win


  // if(!strcmp(txt, "Exit")) {
    // IE_cad_INS ("Exit");  // exit mode Modify;
    // return 0;
  // }


  // gesamten Text vom EditWindow -> IE_buf kopieren
  IE_bufSiz = IE_bufMax;
  // i1 = GUI_Ed_Read (&IE_ccv_wText, IE_buf, &IE_bufMax);
  i1 = GUI_edi_Read (IE_buf, &IE_bufSiz, 0L, -1L, &IE_edWin1);
  if(i1 < 0) {TX_Print("IE_ccv_mod E001"); return 0;}
    // printf("IE_buf=|%s| Siz=%ld\n",IE_buf,IE_bufSiz);


  //----------------------------------------------------------------
  if(!strcmp(txt, "OK")) {
    IE_cad_OK ();  // OK-Button druecken
    // write IE_buf -> active CAD-inputfield
    // IE_inp_set (IE_buf);
    // GUI_update__ ();
    // IE_inp_chg (-1);  // next inputfield
    return 0;
  }


  // Curpos holen
  // cPos = GUI_Ed_getCpos (&IE_ccv_wText);
  cPos = GUI_edi_getCpos (&IE_edWin1);
    // printf("cPos=%ld\n",cPos);
  // wenn (cPos >= IE_bufSiz) dann ist Cursor am Ende


  //----------------------------------------------------------------
  // PREV- ein Wort nach links
  if(!strcmp(txt, "PREV-")) {
    cPos = UTX_pos_del_prev (IE_buf, cPos);
    goto L_view;
  }

  //----------------------------------------------------------------
  // nach rechts ..
  if(!strcmp(txt, "PREV+")) {
    cPos = UTX_pos_del_next (IE_buf, cPos);
    goto L_view;
  }


  //----------------------------------------------------------------
  if(!strcmp(txt, "REV")) {
    // if last word in IE_buf == "CW"  then remove it
    i1 = IE_bufSiz - 4;
      printf(" IE_buf-last |%s|\n",&IE_buf[i1]);
    if(!strcmp (&IE_buf[i1], " REV")) {                  // 2014-04-10
       IE_buf[i1] = '\0';
      IE_bufSiz -= 4;
      // IE_ccv_rem (&dPos, txLen);
    } else {
      strcpy (&IE_buf[IE_bufSiz], " REV");
      IE_bufSiz += 4;
    }
    GUI_edi_Write (&IE_edWin1, IE_buf, &IE_bufSiz, IE_bufMax);
    GUI_edi_setCpos (&IE_edWin1, IE_bufSiz);
    goto L_test;
  }


  //----------------------------------------------------------------
  if(strcmp(txt, "PREVIEW")) {
    L_prev_3:
    // modify Buffer; add od change word
    // CW, CCW, DEL(Esc),  ..
    IE_ccv_cw (&cPos, txt);

    // IE_buf ins GroupeditWindow zurueck
    // GUI_Ed_Write (&IE_ccv_wText, IE_buf, &IE_bufSiz, IE_bufMax);
    GUI_edi_Write (&IE_edWin1, IE_buf, &IE_bufSiz, IE_bufMax);
    goto L_view;
  }


  //----------------------------------------------------------------
  // if(strcmp(txt, "Exit")) {
  // }

  //-----------------------------------------------------
  // wenn nicht PREVIEW (MOD+, MOD-, REV): add od change word
  // Text rechts von CurPos selected setzen
  L_view:
  if(cPos > IE_bufSiz) cPos = IE_bufSiz;
    printf(" IE_bufSiz=%ld cPos=%ld\n",IE_bufSiz,cPos);

/*
    // rest selected setzen: damit wird CurPos gesetzt !!!
    // if(cPos < IE_bufSiz) GUI_Ed_sel(&IE_ccv_wText, cPos, IE_bufSiz);
    GUI_Ed_sel__ (&IE_ccv_wText, IE_bufSiz, cPos);
*/

  // Cursor im GroupeditWindow setzen
  // GUI_Ed_setCpos (&IE_ccv_wText, cPos);
  GUI_edi_setCpos (&IE_edWin1, cPos);


/*
  // PREVIEW: Rest hinter dem Cursor abschneiden
  if(!strncmp(txt, "PREV", 4)) {
    if(oldPos != cPos) {
*/
      IE_buf[cPos] = '\0';
      IE_bufSiz = strlen(IE_buf);
      // printf(" << IE_buf=|%s| Siz=%ld\n",IE_buf,IE_bufSiz);
/*
    }
    oldPos = cPos;
  }
*/

  // nun sind alle Modifikation in IE_buf erledigt;

 

  L_test:
  // testen
  if(IE_bufSiz > 2) {
    // update CW|CCW
    // if..
    IE_cad_test__ ();
    // update lmnr
    IE_ccv__ (NULL, GUI_SETDAT_EI(TYP_EventPress, UI_FuncOK));

  } else {
    GL_temp_del_1 (DLI_TMP);      // delete temporary curve
    GL_temp_del_1 (DLI_DIR_TMP);  // delete temporary direction-arrow
  }



  return 0;
}


//=====================================================================
  int IE_ccv_CB (MemObj *mo, void **data) {
//=====================================================================
// keyb-callback from GroupWindow IE_ccv__
// BUT Tab-Key goes to UI_GL_keys__ -> IE_inp_chg

// TODO: =TYP_EventEnter

  static int oldEvent;
  int      i1, iKey;
  long     cPos;


  printf("IE_ccv_CB EV=%d\n",GUI_DATA_EVENT);

/*
  if(GUI_DATA_EVENT == TYP_EventEnter) {
    // Mouseklick into GroupEditWindow
    // comes too often ..
// TODO: skip PREVIEW if curPos is unmodified ..
    IE_ccv_mod (NULL, GUI_SETDAT_ES(TYP_EventPress,"PREVIEW"));
    return 0;
  }
*/

  // TYP_EventRelease ..
  iKey = GUI_DATA_I1;
    // printf("IE_ccv_CB iKey=%d\n",iKey);


  switch (iKey) {

    case GUI_KeyEsc:
      if(oldEvent == TYP_EventPress) goto L_skip_key;   // 2014-04-10
      // CREATE or MODIFY
      if(IE_modify == 0) {  // 0=Add 1=Modify 2=Insert
        //  CREATE: delete entries, then window
        IE_cad_Inp_undo ();
      } else {
        //  MODIFY: exit all ..
        IE_cad_Inp_undo ();      // delete last CAD-input or exit func
        // kill GruppenEditorWindow, if activ
        IE_ccv__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncKill));
      }
      goto L_skip_key;


    case GUI_KeyReturn:
      // // gesamten Text -> IE_buf kopieren
      // IE_bufSiz = IE_BUFSIZ;
      // i1 = GUI_Ed_Read (&IE_ccv_wText, IE_buf, &IE_bufSiz);
      // if(i1 < 0) {TX_Print("IE_ccv_CB E001"); return 0;}
      // IE_cad_OK ();
      IE_ccv_mod (NULL, GUI_SETDAT_ES(TYP_EventPress,"PREVIEW"));
      goto L_skip_key;


    case GUI_KeyCurDown:
    case GUI_KeyTab:
      IE_ccv_mod (NULL, GUI_SETDAT_ES(TYP_EventPress,"PREVIEW"));
      return TRUE;
/*
      // Curpos holen
      cPos = GUI_Ed_getCpos (&IE_ccv_wText);
      // gesamten Text -> IE_buf kopieren
      IE_bufSiz = IE_BUFSIZ;
      i1 = GUI_Ed_Read (&IE_ccv_wText, IE_buf, &IE_bufSiz);
      if(i1 < 0) {TX_Print("IE_ccv_CB E001"); return 0;}
      printf(" cPos=%ld IE_bufSiz=%ld\n",cPos,IE_bufSiz);
      // nur bis Curpos abarbeiten (besseres Finden der akt.Pos in der Kontur)

      IE_bufSiz = cPos;
      IE_buf[IE_bufSiz] = '\0';
      IE_cad_test__ ();
      goto L_skip_key;
*/
  }

  // return FALSE;



  //==================================================================
  L_skip_key:
  // prevent the default handler from being run
  // sonst kommt zB Esc twice
  // gtk_signal_emit_stop_by_name (GTK_OBJECT(parent),"key_press_event");

    oldEvent = GUI_DATA_EVENT;

  // return TRUE;
  return 0;

}


//================================================================
  int IE_ccv_ck_active () {
//================================================================

  // printf("IE_ccv_ck_active \n");

  return IE_ccv__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB3));

}


 /*
//=====================================================================
  int IE_ccv_keyb (MemObj *mo, void **data) {
//=====================================================================

  printf(" key=%d\n",*(int*)data[2]);

  return 0;

}
*/
 
//=====================================================================
  int IE_ccv__ (MemObj *mo, void **data) {
//=====================================================================
// simple CurveEditor
// benutzt - ebenso wie IE_ed2__ - IE_edWin1

static  MemObj   win0, bREV, lmnr, cb_poc;
  MemObj   box0, box1;
  int      i1, mode;
  char     s1[32];


  i1 = GUI_DATA_I1;


  printf("IE_ccv__ %d\n",i1);


  switch (i1) {


    //---------------------------------------------------------
    case UI_FuncInit:

      // printf(" Ind=%d TypR=%d\n",IE_inpInd,IE_inpTypR[IE_inpInd]);
      // printf(" IE_buf=%ld |%s|\n",IE_bufSiz,IE_buf);
      // if(GUI_OBJ_IS_VALID(&IE_edWin1)) printf(" !! IE_ccv__ invalid !!\n");


      // IE_ccv_win.win = GUI_Window ("GroupEdit",IE_ccv__,(void*)UI_FuncKill,0);
      if(winTyp == 0) strcpy(s1, "CCV-Edit");
      else            strcpy(s1, "Group-Edit");
      win0 = GUI_Win__ (s1,  IE_ccv__, "-500,-100");  // UI_FuncExit

      // box0 = GUI_box_v (IE_ccv_win.win, 0);

      box0 = GUI_box_v (&win0, "");

      // GUI_Ed_Init (&IE_ccv_wText, IE_ccv_CB, 1);
      IE_edWin1 = GUI_edi__ (&box0, IE_ccv_CB, 1, "e,e");
     

      // H-Box f Buttons unten
      box1 = GUI_box_h (&box0, "a,a");

      cb_poc = GUI_ckbutt__(&box1, "point on curve", 0,
                            IE_ccv__, &GUI_FuncUCB4, "a,a");


      if(!winTyp) {
        bREV=GUI_button__(&box1, "REV", IE_ccv_mod, (void*)"REV", "a,a");
          MSG_Tip ("ED1CW");
        // GUI_set_enable (&bCCW, 0); //1=activ, 0=inaktiv
      }


      GUI_sep__ (&box1, 1, 0);

      lmnr = GUI_label__ (&box1, "-", "l");

      if(!winTyp) {
        GUI_button__(&box1, "MOD+", IE_ccv_mod, (void*)"MOD+", "a,a");
          MSG_Tip ("ED1MOD+");
        GUI_button__(&box1, "MOD-", IE_ccv_mod, (void*)"MOD-", "a,a");
          MSG_Tip ("ED1MOD-");
      }

      GUI_button__(&box1, " <<< ", IE_ccv_mod, (void*)"PREV-", "a,a");
        MSG_Tip ("ED1<<<");
      GUI_button__(&box1, " >>> ", IE_ccv_mod, (void*)"PREV+", "a,a");
        MSG_Tip ("ED1>>>");
      GUI_button__(&box1, "Delete", IE_ccv_mod, (void*)"DEL", "a,a");
        MSG_Tip ("MMundo");
      GUI_button__(&box1, "Cancel", IE_ccv__, &GUI_FuncKill, "a,a");
        MSG_Tip ("CADexit");
      GUI_button__(&box1, "OK", IE_ccv_mod, (void*)"OK", "a,a");
        MSG_Tip ("CADsave");


      // GUI_Win_ev_key (&win0, IE_ccv_keyb);
      GUI_Win_up (NULL, &win0, 0);
      GUI_Win_go (&win0);

      // Text -> EdWin
      if(IE_bufSiz > 0) {
        GUI_edi_Write (&IE_edWin1, IE_buf, &IE_bufSiz, IE_bufMax);
        // Cursor -> EOL
        GUI_edi_setCpos (&IE_edWin1, -1L);
      }

      break;


    //---------------------------------------------------------
    case UI_FuncGet:
      if(GUI_OBJ_IS_VALID(&IE_edWin1)) {
        IE_bufSiz = IE_bufMax;
        GUI_edi_Read (IE_buf, &IE_bufSiz, 0L, -1L, &IE_edWin1);
          // printf(" IE_buf now |%s|\n",IE_buf);
      }
      break;

    //---------------------------------------------------------
    case UI_FuncSet:
      // write text in IE_buf into window 
        GUI_edi_Write (&IE_edWin1, IE_buf, &IE_bufSiz, IE_bufMax);
          // printf(" IE_buf now |%s|\n",IE_buf);
        // Cursor -> EOL
        GUI_edi_setCpos (&IE_edWin1, -1L);
      // reset "point on curve"
      if(GUI_ckbutt_get(&cb_poc)) {
        GUI_ckbutt_set (&cb_poc, 0);
        UI_GR_Sel_Filter (0);  // reset
      }
      break;


    //---------------------------------------------------------
    case UI_FuncPrev:
      // delete last word in IE_buf, update IE_bufSiz
        GUI_edi_Write (&IE_edWin1, IE_buf, &IE_bufSiz, IE_bufMax);
          // printf(" IE_buf now |%s|\n",IE_buf);
        // Cursor -> EOL
        GUI_edi_setCpos (&IE_edWin1, -1L);
      break;


    //---------------------------------------------------------
    // update after IE_cad_test__:
    case UI_FuncOK:  // update lmnr
      i1 = APT_get_modMax1 ();
      sprintf(s1, "  %d ", i1);
      GUI_label_mod (&lmnr, s1);
      break;


/*
    //---------------------------------------------------------
    case UI_FuncUCB1:  // activate "CW" and disactivate "CCW" 1=activ, 0=inaktiv
      GUI_set_enable (&bCW, 1);
      GUI_set_enable (&bCCW, 0);
      break;


    //---------------------------------------------------------
    case UI_FuncUCB2:  // disactivate "CW" and activate "CCW" 1=activ, 0=inaktiv
      GUI_set_enable (&bCW, 0);
      GUI_set_enable (&bCCW, 1);
      break;
*/

    //---------------------------------------------------------
    case UI_FuncUCB3:  // check if active
      return GUI_OBJ_IS_VALID(&win0);

    //---------------------------------------------------------
    case UI_FuncUCB4:  // "point on curve" selected ..
      UI_GR_Sel_Filter (18);
      break;



    //---------------------------------------------------------
    case UI_FuncKill:      // called from extern
        // printf("IE_ccv__ exit\n");
      GUI_Win_kill (&win0);


    //---------------------------------------------------------
    case UI_FuncExit:   // 102; called from GUI_Win__

      GL_temp_del_all (); // alle temp. obj loeschen ..

      win0 = GUI_OBJ_INVALID();
      IE_edWin1 = GUI_OBJ_INVALID();
      IE_ui_func_closed (); // reset IE_edWin1 & IE_ui_func
  }



  return 0;

}



//=====================================================================
  int IE_ccv_rem (long *cPos, long txLen) {
//=====================================================================
// remove word out of IE_buf[IE_bufSiz]

  // long l1;

  printf("IE_ccv_rem %ld %ld %ld\n",*cPos,txLen,IE_bufSiz);
  // printf("  buf=|%s| rest=|%s|\n",IE_buf,&IE_buf[*cPos]);



  // Remove cbuf from End of IE_buf
  if((*cPos+txLen) < IE_bufSiz) goto L_cut;
  // *cPos -= txLen;
  if(*cPos > 0) --(*cPos);
  IE_buf[*cPos] = '\0';
  goto L_fertig;




  //------------------------------------------------------
  L_cut:
  // cut cbuf out of IE_buf
  UTX_cut1 (IE_buf, &IE_bufSiz, *cPos, txLen+1);




  L_fertig:
  IE_bufSiz = strlen(IE_buf);

    // printf("ex IE_ccv_rem |%s|\n",IE_buf);

  return 0;

}



//=====================================================================
  int IE_ccv_cw (long *cPos, char *newTxt) {
//=====================================================================
// modify Buffer; add od change or delete word at Pos cPos.
// Input:
//   cPos         position of cursor in IE_buf
//   newTxt
// Extern:
//   IE_buf[IE_bufSiz]   whole buffer


  int    i1;
  long   dPos, txLen;
  char   cbuf[64];


  // printf("==================================== \n");
  printf("IE_ccv_cw %ld |%s|\n",*cPos,newTxt);


  if(*cPos > IE_bufSiz) *cPos = IE_bufSiz;

  // printf("  buf=|%s| rest=|%s|\n",IE_buf,&IE_buf[*cPos]);




  // cPos weiter bis zum naechsten Wort
  L_start:
  // printf(" actChar=|%c|\n",IE_buf[*cPos]);
  if(*cPos < IE_bufSiz) {
    if(IE_buf[*cPos] == ' ') {
      ++(*cPos);
      goto L_start;
    }
  }




  if(IE_bufSiz < 1) {
    if(!strcmp(newTxt, "DEL")) return 0;
    strcpy(cbuf, newTxt);
    goto L_add;
  }


  // dPos = find previous delimiter (last blank)
  dPos = UTX_pos_del_prev (IE_buf, *cPos);


  // cbuf[txLen] = last word (Work links vom Cursor kopieren)
  txLen = *cPos - dPos;
  if(txLen >= 64) { TX_Error("IE_ccv_cw E001"); return -1; }
  strncpy(cbuf, &IE_buf[dPos], txLen);
  cbuf[txLen] = '\0';
  UTX_CleanCR (cbuf);
  txLen = strlen(cbuf);
    printf("cbuf=|%s| txLen=%ld dPos=%ld\n",cbuf,txLen,dPos);



  //======== handle "DEL" ========================
  if(!strcmp(newTxt, "DEL")) {
    // das Wort links von cPos loeschen;
    IE_ccv_rem (&dPos, txLen);
    // und NIX mehr zufuegen

    IE_bufSiz = strlen(IE_buf);
    *cPos = dPos;
    goto L_fertig;



  //======== handle "MOD" ========================
  // newTxt is MOD+ or MOD-
  // whats cbuf
  // whats dPos
  // whats txLen
  } else if(!strncmp(newTxt, "MOD", 3)) {
    i1 = 0;
    // war vorher auch ein MOD ?
    if(!strncmp(cbuf, "MOD", 3)) {
      // modNr auslesen
      i1 = atoi(&cbuf[4]);
      // remove
      // IE_ccv_rem (&dPos, txLen);
      // *cPos = dPos;

    } else {          // noch kein MOD vorhanden
      strcpy(cbuf, "MOD(2)");
      goto L_add;
    }

    // fix i1=new nr
    if(newTxt[3] == '+') {
      // test maxSol
        printf(" maxVal=%d\n",APT_get_modMax1());
       if(APT_get_modMax1() > i1) ++i1;
    } else {
      --i1;
      if(i1 < 1) i1 = 1;
    }

    // fix text (change nr)
    // sprintf(cbuf, "MOD(%d)", i1);
    dPos += 4;
    // change number in line
    i1 = UTX_chg_nr (&IE_buf[dPos], i1);
    // i1 = IE_bufSiz;
    // IE_bufSiz = strlen(IE_buf);
    // *cPos += IE_bufSiz - i1;      // fix CurPos
    IE_bufSiz += i1;
    *cPos += i1;
    goto L_fertig;




  //======== handle "xxx" ========================
  } else {
    // newTxt unveraendert raus
    strcpy(cbuf, newTxt);

  }




  //-----------------------------------------
  // add oder insert new word at Pos *cPos
  L_add:
  // printf(" add at %ld\n",*cPos);

  // add data to buf
  if(*cPos < IE_bufSiz) goto L_insert;
  if((IE_bufSiz > 0)&&(IE_buf[IE_bufSiz-1] != ' ')) strcat(IE_buf, " ");
  strcat(IE_buf, cbuf);
  IE_bufSiz = strlen(IE_buf);
  *cPos = IE_bufSiz;
  goto L_fertig;


  // insert new word into IE_buf
  L_insert:
  strcat(cbuf, " ");
  UTX_ins_s2 (&IE_buf[*cPos], cbuf, IE_bufMax-dPos);
  IE_bufSiz = strlen(IE_buf);
  *cPos = *cPos + strlen(cbuf);



  //-----------------------------------------
  // *cPos nun die Pos nach der korrigierten Position.
  L_fertig:
  // printf("ex IE_ccv_cw cPos=%ld siz=%ld\n",*cPos,IE_bufSiz);
  // printf("     IE_buf=|%s| rest=|%s|\n",IE_buf,&IE_buf[*cPos]);

  return 0;
}



/* ====================== EOF =========================== */
