//    ../xa/xa_cad_ed1.c                     CAD-GroupEditor
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
2014-05-27  remove CCV-function; group-function remains
2014-04-10  Extracted from xa_cad_ui.c. RF.

-----------------------------------------------------
*/
/*!
\file  ../xa/xa_cad_ed1.c
\brief  GUI CAD-GroupEditor
\code
=====================================================
List_functions_start:

IE_ed1__             create Window; simple CurveEditor
IE_ed1_mod           GroupEditor ?
IE_ed1_CMB           Mouseklick into GroupEditWindow
IE_ed1_CB
IE_ed1_cw            modify IE_buf add|change|delete word
IE_ed1_stat          get status of GroupEditWindow

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


Window = IE_edWin1;
textbuffer = IE_buf[IE_bufSiz].


Offline testen:
cc xa_cad_ed.c ../ut/ut_TX.o ../ut/ut_txt.o -DOFFLINE&&a.out

==============================================================

*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_geo.h"
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_txt.h"

#include "../xa/xa_sele.h"        // Typ_go*
// #include "../xa/xa_uid.h"               // GR_TMP_IDIR
#include "../gr/ut_gr.h"          // GR_TMP_I*

#include "../gui/gui__.h"         // Gtk



#define   TRUE 1
#define   FALSE 0

static int IE_EditMode = 0;    // 0=EditMode_add; 1=EditMode_insert



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
  IE_ed1_cw (&cPos, "P20");        // Start
  // IE_ed1_cw (&cPos, "P(0 0)");        // Start
  IE_ed1_cw (&cPos, "DEL");        // remove C20
  // IE_ed1_cw (&cPos, "C20");        // add
  exit(0);



  // test add
  strcpy(IE_buf, "P20 C20");
  IE_bufSiz=strlen(IE_buf);
  cPos=strlen(IE_buf);



  // add od change word at Pos cPos.
  // IE_ed1_cw (&cPos, "DEL");        // remove C20
  // IE_ed1_cw (&cPos, "CW");
  // IE_ed1_cw (&cPos, "CCW");       // change vorheriges CW
  // IE_ed1_cw (&cPos, "MOD+");      // add MOD(1)
  // IE_ed1_cw (&cPos, "MOD+");      // change -> MOD(2)
  // IE_ed1_cw (&cPos, "MOD-");



  // test insert CW
  strcpy(IE_buf, "P20 C20 P21");
  IE_bufSiz=strlen(IE_buf);
  cPos=IE_bufSiz-4;             // nach CW muss mit -3 u -4 gehen

  // IE_ed1_cw (&cPos, "CW");       // insert CW
  // IE_ed1_cw (&cPos, "DEL");     // remove CW
  // IE_ed1_cw (&cPos, "CCW");     // change CW -> CCW
  // IE_ed1_cw (&cPos, "CW");



  // test insert MOD
  strcpy(IE_buf, "P20 C20 P21");
  IE_bufSiz=strlen(IE_buf);
  cPos=IE_bufSiz-3;             // nach CW muss mit -3 u -4 gehen

  // IE_ed1_cw (&cPos, "DEL");        // remove MOD(1)
  IE_ed1_cw (&cPos, "MOD+");     // ins o change MOD(1) -> MOD(2)
  // IE_ed1_cw (&cPos, "MOD-");



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


#endif




//================================================================
  int IE_ed1_stat () {
//================================================================
// IE_ed1_stat          get status of CCV-Edit-window
// Retcod:
//  -1      window is not active
//   0      textfield ist empty
//   1      window is active and textfield is not empty
 
  // printf("IE_ed1_stat valid=%d\n",GUI_OBJ_IS_VALID(&IE_edWin1));

  if(!GUI_OBJ_IS_VALID(&IE_edWin1)) return -1;

  // printf("IE_ed1_stat len=%d\n",strlen(IE_buf));
  if(strlen(IE_buf) < 1) return 0;

  return 1;

}


//=====================================================================
  int IE_ed1_mod (MemObj *mo, void **data) {
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


static long oldPos = 0L;

  int      i1;
  long     cPos, dPos;
  char     *txt;


  // printf("IE_ed1_mod \n");

  if(!GUI_OBJ_IS_VALID(&IE_edWin1))  return -1;

  txt = GUI_DATA_S1;


  // printf("============== IE_ed1_mod |%s|\n",txt);
  // printf("  IE_buf=|%s|\n",IE_buf);
  // printf("  IE_bufSiz=%ld oldPos=%ld\n",IE_bufSiz,oldPos);



  // IE_ed1__ (NULL, GUI_SETDAT_EL(TYP_EventPress,UI_FuncUCB1));   // raise win

  if(!strcmp(txt, "Exit")) {
    IE_cad_Inp_cancel ();
    return 0;
  }

  // copy whole window-buffer -> IE_buf
  IE_bufSiz = IE_bufMax;
  i1 = GUI_edi_Read (IE_buf, &IE_bufSiz, 0L, -1L, &IE_edWin1);
  if(i1 < 0) {TX_Print("IE_ed1_mod E001"); return 0;}
    // printf("IE_buf=|%s| Siz=%ld\n",IE_buf,IE_bufSiz);


  //----------------------------------------------------------------
  if(!strcmp(txt, "END")) {
    goto L_view;
  }


  // Curpos holen
  // cPos = GUI_Ed_getCpos (&IE_ed1_wText);
  cPos = GUI_edi_getCpos (&IE_edWin1);
    // printf("cPos=%ld\n",cPos);
  // wenn (cPos >= IE_bufSiz) dann ist Cursor am Ende


  //----------------------------------------------------------------
  // PREV- ein Wort nach links
  if(!strcmp(txt, "PREV-")) {
    cPos = UTX_pos_del_prev (IE_buf, cPos);
    IE_EditMode = EditMode_insert;
    goto L_view;
  }

  //----------------------------------------------------------------
  // nach rechts ..
  if(!strcmp(txt, "PREV+")) {
    cPos = UTX_pos_del_next (IE_buf, cPos);
    if(cPos >= IE_bufSiz) IE_EditMode = EditMode_add;
    goto L_view;
  }



  //----------------------------------------------------------------
  // wenn nicht PREVIEW (MOD+, MOD-): add od change word
  if(strcmp(txt, "PREVIEW")) {

    // modify Buffer; add od change word
    // CW, CCW, DEL(Esc),  ..
    IE_ed1_cw (&cPos, txt);

    // copy IE_buf back into edit-window
    GUI_edi_Write (&IE_edWin1, IE_buf, &IE_bufSiz, IE_bufMax);
    goto L_view;
  }



  //================================================================
  // Text rechts von CurPos selected setzen
  L_view:
  if(cPos > IE_bufSiz) cPos = IE_bufSiz;
    // printf("IE_ed1__-L_view: IE_bufSiz=%ld cPos=%ld IE_EditMode=%d\n",
           // IE_bufSiz,cPos,IE_EditMode);

/*
    // rest selected setzen: damit wird CurPos gesetzt !!!
    // if(cPos < IE_bufSiz) GUI_Ed_sel(&IE_ed1_wText, cPos, IE_bufSiz);
    GUI_Ed_sel__ (&IE_ed1_wText, IE_bufSiz, cPos);
*/

  // Cursor im GroupeditWindow setzen
  // GUI_Ed_setCpos (&IE_ed1_wText, cPos);
  GUI_edi_setCpos (&IE_edWin1, cPos);


/*
  // PREVIEW: Rest hinter dem Cursor abschneiden
  if(!strncmp(txt, "PREV", 4)) {
    if(oldPos != cPos) {
*/
      // cut off right of curPos
      IE_buf[cPos] = '\0';
      // update size
      IE_bufSiz = strlen(IE_buf);
      // printf(" << IE_buf=|%s| Siz=%ld\n",IE_buf,IE_bufSiz);


  if(IE_bufSiz > 2) {
    // test if input complete and display
    IE_cad_test__ ();
  }



  return 0;
}


/*
//=====================================================================
  gint IE_ed1_CMB (void *widget, void *event, gpointer data) {
//=====================================================================
// Mouseklick into GroupEditWindow


  printf("IE_ed1_CMB\n");


  // prevent the default handler from being run
  // NEIN; sonst wird Curpos nicht gesetzt!
  // gtk_signal_emit_stop_by_name (GTK_OBJECT(widget),"button_press_event");


  IE_ed1_mod (NULL, (void*)"PREVIEW");

  return FALSE;
  // return TRUE;   // bis 2006-02-02
}
*/


//=====================================================================
  int IE_ed1_CB (MemObj *mo, void **data) {
//=====================================================================
// keyb-callback from GroupWindow IE_ed1__

// TODO: =TYP_EventEnter

  static int oldEvent;
  int      i1, iKey;
  long     cPos;


  // printf("=================== IE_ed1_CB EV=%d\n",GUI_DATA_EVENT);


  //----------------------------------------------------------------
  if(GUI_DATA_EVENT == TYP_EventEnter) {
    // obj into IE_buf added OR Mouseklick into GroupEditWindow
    IE_ed1_mod (NULL, GUI_SETDAT_ES(TYP_EventPress,"PREVIEW"));
    return 0;
  }


  //----------------------------------------------------------------
  // TYP_EventRelease ..
  iKey = GUI_DATA_L1;
    // printf("IE_ed1_CB iKey=%d\n",iKey);


  switch (iKey) {

    case GUI_KeyEsc:
      if(oldEvent == TYP_EventPress) goto L_skip_key;   // 2014-04-10
      // CREATE or MODIFY
      if(IE_modify == 0) {  // 0=Add 1=Modify 2=Insert
        //  CREATE: exit all ..
        IE_ed1_mod (NULL, GUI_SETDAT_ES(TYP_EventPress,"Exit"));
        // IE_ed1__ (NULL, (void*)"UI_FuncKill");
      } else {
        //  MODIFY: exit all ..
        IE_cad_Inp_undo ();      // delete last CAD-input or exit func
        // kill GruppenEditorWindow, if activ
        IE_ed1__ (NULL, GUI_SETDAT_EL(TYP_EventPress,UI_FuncKill));
      }
      goto L_skip_key;


    case GUI_KeyReturn:
      // // gesamten Text -> IE_buf kopieren
      // IE_bufSiz = IE_BUFSIZ;
      // i1 = GUI_Ed_Read (&IE_ed1_wText, IE_buf, &IE_bufSiz);
      // if(i1 < 0) {TX_Print("IE_ed1_CB E001"); return 0;}
      // IE_cad_OK ();
      IE_ed1_mod (NULL, GUI_SETDAT_ES(TYP_EventPress,"PREVIEW"));
      goto L_skip_key;


    case GUI_KeyCurDown:
    case GUI_KeyTab:
      IE_ed1_mod (NULL, GUI_SETDAT_ES(TYP_EventPress,"PREVIEW"));
      return TRUE;
/*
      // Curpos holen
      cPos = GUI_Ed_getCpos (&IE_ed1_wText);
      // gesamten Text -> IE_buf kopieren
      IE_bufSiz = IE_BUFSIZ;
      i1 = GUI_Ed_Read (&IE_ed1_wText, IE_buf, &IE_bufSiz);
      if(i1 < 0) {TX_Print("IE_ed1_CB E001"); return 0;}
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


//=====================================================================
  int IE_ed1__ (MemObj *mo, void **data) {
//=====================================================================
// simple editor for polygons
// using IE_edWin1  (like IE_ed1__ IE_ed2__ IE_ccv__)

static  MemObj   win0, bCW, bCCW;
// static  int      winMode;             // 0=Typ_goPrim; with CW,CCW,MOD+,MOD-
//                                       // else points only; no CW,CCW, ..
  int      irc, i1, mode;
  long     cPos, c1Pos, l1;
  char     *sTmp;
  MemObj   box0, box1;


  i1 = GUI_DATA_L1;

  // printf("EEEEEEEEEEEEEEEEE IE_ed1__ %d\n",i1);
  // printf("  IE_buf=|%s|\n",IE_buf);
  // printf("  IE_bufSiz = %ld\n",IE_bufSiz);


  switch (i1) {


    //---------------------------------------------------------
    case UI_FuncInit:

      // printf(" Ind=%d TypR=%d\n",IE_inpInd,IE_inpTypR[IE_inpInd]);
      // printf(" IE_buf=%ld |%s|\n",IE_bufSiz,IE_buf);
      // if(GUI_OBJ_IS_VALID(&IE_edWin1)) printf(" !! IE_ed1__ invalid !!\n");


      // IE_ed1_win.win = GUI_Window ("GroupEdit",IE_ed1__,(void*)UI_FuncKill,0);
      win0 = GUI_Win__ ("GroupEdit",  IE_ed1__, "-500,-100");  // UI_FuncExit

      // box0 = GUI_box_v (IE_ed1_win.win, 0);

      box0 = GUI_box_v (&win0, "");

      // GUI_Ed_Init (&IE_ed1_wText, IE_ed1_CB, 1);
      IE_edWin1 = GUI_edi__ (&box0, IE_ed1_CB, 1, "e,e");
     
/*
      // connect KlickIntoEditor
      // bei press kommt die alte CurPos !!
      // gtk_signal_connect (GTK_OBJECT(IE_ed1_wText), "button_release_event",
      // gtk_signal_connect (GTK_OBJECT(IE_ed1_wText.text),  // bis 2005-10-26
      gtk_signal_connect (GTK_OBJECT(IE_ed1_wText.view),     // ab 2005-10-26
                          // "button_press_event", // damit kommt alte CurPos !!
                          "button_release_event",
                          GTK_SIGNAL_FUNC(IE_ed1_CMB), NULL);
*/

      // H-Box f Buttons unten
      box1 = GUI_box_h (&box0, "a,a");

      GUI_button__(&box1, " <<< ", IE_ed1_mod, (void*)"PREV-", "a,a");
        MSG_Tip ("ED1<<<");

      GUI_button__(&box1, " >>> ", IE_ed1_mod, (void*)"PREV+", "a,a");
        MSG_Tip ("ED1>>>");

      GUI_button__(&box1, "Delete", IE_ed1_mod, (void*)"DEL", "a,a");
        MSG_Tip ("MMundo");

      GUI_button__(&box1, "Exit", IE_ed1_mod, (void*)"Exit", "a,a");
      // GUI_button__(&box1, "Exit", IE_ed1__, &GUI_FuncKill, "a,a");
        MSG_Tip ("CADexit");

      GUI_button__(&box1, "END", IE_ed1_mod, (void*)"END", "a,a");
        MSG_Tip ("CADsave");

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
          printf(" IE_ed1__-Get\n");
        IE_bufSiz = IE_bufMax;
        GUI_edi_Read (IE_buf, &IE_bufSiz, 0L, -1L, &IE_edWin1);
          // printf(" IE_buf now |%s|\n",IE_buf);
      }
      break;


    //---------------------------------------------------------
    case UI_FuncSet:
      c1Pos = -1L;
      if(IE_EditMode == EditMode_insert) {
        // insert: add all expressions right of curPos of IE_edWin1-text
        c1Pos = IE_bufSiz;  // set new curPos
        cPos = GUI_edi_getCpos (&IE_edWin1);  // get curPos of IE_edWin1-text
          // printf(" INSERT - IE_bufSiz=%ld cPos=%ld\n",IE_bufSiz,cPos);
        // get tempspace for IE_edWin1-text
        l1 = GUI_edi_getEof (&IE_edWin1) + 4;
        sTmp = (char*) MEM_alloc_tmp (l1);
        if(!sTmp) { TX_Error("IE_ed1__ EOM-1"); break; }
        // get IE_edWin1-text
        irc = GUI_edi_Read (sTmp, &l1, 0L, -1L, &IE_edWin1);
        if(irc < 0) { TX_Print("IE_ed1_mod E001"); break; }
          // printf(" sTmp=|%s|\n",sTmp);
        // start with blank
        if(cPos) {
          if(sTmp[cPos] != ' ') --cPos;
        } else {
          strcat(IE_buf, " ");
          ++IE_bufSiz;
        }
        if((IE_bufSiz + (l1 - cPos)) >= IE_bufMax)
          { TX_Print("IE_ed1_mod EOM-2"); break; }
        // add IE_buf + IE_edWin1-text right of curPos
        strcat(IE_buf, &sTmp[cPos]);
        IE_bufSiz = strlen(IE_buf);
      }

      // write IE_buf into IE_edWin1
      GUI_edi_Write (&IE_edWin1, IE_buf, &IE_bufSiz, IE_bufMax);
        // printf(" IE_buf now |%s|\n",IE_buf);
      // set Cursor
      GUI_edi_setCpos (&IE_edWin1, c1Pos);
      break;


    //---------------------------------------------------------
    case UI_FuncKill:      // called from extern
        // printf("IE_ed1__ exit\n");
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
  int IE_ed1_cw (long *cPos, char *newTxt) {
//=====================================================================
// modify Buffer; add od change or delete word at Pos cPos.
// CurPos in IE_buf = cPos.
// Buffer = IE_buf[IE_bufSiz]

  int    i1;
  long   dPos, txLen;
  char   cbuf[64];


  // printf("==================================== \n");
  // printf("IE_ed1_cw %ld |%s|\n",*cPos,newTxt);


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
  if(txLen >= 64) { TX_Error("IE_ed1_cw E001"); return -1; }
  strncpy(cbuf, &IE_buf[dPos], txLen);
  cbuf[txLen] = '\0';
  UTX_CleanCR (cbuf);
  txLen = strlen(cbuf);
  // printf("cbuf=|%s| txLen=%ld dPos=%ld\n",cbuf,txLen,dPos);




  //======== handle "DEL" ========================
  if(!strcmp(newTxt, "DEL")) {
    // das Wort links von cPos loeschen;
    IE_buf_rem (&dPos, txLen);
    // und NIX mehr zufuegen
// TODO: remove last curPos (
    GL_temp_del_1 (GR_TMP_IPOS);

    IE_bufSiz = strlen(IE_buf);
    *cPos = dPos;
    goto L_fertig;




  //======== handle "CW" ========================
  } else if(!strcmp(newTxt, "CW")) {
    // skip new CW
    if(!strcmp(cbuf, "CW")) return 0;
    // remove CCW
    if(!strcmp(cbuf, "CCW")) {
      IE_buf_rem (&dPos, txLen);
      *cPos = dPos;
    }
    // newTxt unveraendert raus
    strcpy(cbuf, newTxt);




  //======== handle "CCW" ========================
  } else if(!strcmp(newTxt, "CCW")) {
    // skip new CW
    if(!strcmp(cbuf, "CCW")) return 0;
    // remove CW
    if(!strcmp(cbuf, "CW")) {
      IE_buf_rem (&dPos, txLen);
      *cPos = dPos;
    }
    // newTxt unveraendert raus
    strcpy(cbuf, newTxt);




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
      // IE_buf_rem (&dPos, txLen);
      // *cPos = dPos;

    } else {          // noch kein MOD vorhanden
      strcpy(cbuf, "MOD(1)");
      goto L_add;
    }

    if(newTxt[3] == '+') {
      // test maxSol
        printf(" maxVal=%d\n",APT_get_modMax1());
       if(APT_get_modMax1() > i1) ++i1;
    } else {
      --i1;
      if(i1 < 1) i1 = 1;
    }
    // fix text
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
  // printf("ex IE_ed1_cw cPos=%ld siz=%ld\n",*cPos,IE_bufSiz);
  // printf("     IE_buf=|%s| rest=|%s|\n",IE_buf,&IE_buf[*cPos]);

  return 0;
}



/* ====================== EOF =========================== */
