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
IE_ed1_rem           remove word out of IE_buf
IE_ed1_cw            modify IE_buf add|change|delete word
IE_ed1_stat          get status of CCV-Edit-window

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

// aus ../ci/NC_Main.c:
extern int     APT_dispDir;



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





// static long oldPos;

  int      i1;
  long     cPos, dPos;
  char     *txt;


  // printf("IE_ed1_mod \n");

  if(!GUI_OBJ_IS_VALID(&IE_edWin1))  return -1;


  txt = GUI_DATA_S1;
    // printf("IE_ed1_mod  /%s/\n",txt);

  // IE_ed1__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB1));   // raise win


  if(!strcmp(txt, "Exit")) {
    IE_cad_Inp_cancel ();
    return 0;
  }


  // gesamten Text vom EditWindow -> IE_buf kopieren
  IE_bufSiz = IE_bufMax;
  // i1 = GUI_Ed_Read (&IE_ed1_wText, IE_buf, &IE_bufMax);
  i1 = GUI_edi_Read (IE_buf, &IE_bufSiz, 0L, -1L, &IE_edWin1);
  if(i1 < 0) {TX_Print("IE_ed1_mod E001"); return 0;}
    // printf("IE_buf=|%s| Siz=%ld\n",IE_buf,IE_bufSiz);


  //----------------------------------------------------------------
  if(!strcmp(txt, "OK")) {
    IE_cad_OK ();  // OK-Button druecken
    return 0;
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
    goto L_view;
  }

  //----------------------------------------------------------------
  // nach rechts ..
  if(!strcmp(txt, "PREV+")) {
    cPos = UTX_pos_del_next (IE_buf, cPos);
    goto L_view;
  }



  //----------------------------------------------------------------
  // wenn nicht PREVIEW (MOD+, MOD-): add od change word
  if(strcmp(txt, "PREVIEW")) {

    // modify Buffer; add od change word
    // CW, CCW, DEL(Esc),  ..
    IE_ed1_cw (&cPos, txt);

    // IE_buf ins GroupeditWindow zurueck
    // GUI_Ed_Write (&IE_ed1_wText, IE_buf, &IE_bufSiz, IE_bufMax);
    GUI_edi_Write (&IE_edWin1, IE_buf, &IE_bufSiz, IE_bufMax);
    goto L_view;
  }



  //-----------------------------------------------------
  // Text rechts von CurPos selected setzen
  L_view:
  if(cPos > IE_bufSiz) cPos = IE_bufSiz;
    // printf(" IE_bufSiz=%ld cPos=%ld\n",IE_bufSiz,cPos);

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
      IE_buf[cPos] = '\0';
      IE_bufSiz = strlen(IE_buf);
      // printf(" << IE_buf=|%s| Siz=%ld\n",IE_buf,IE_bufSiz);
/*
    }
    oldPos = cPos;
  }
*/

  // nun sind alle Modifikation in IE_buf erledigt;

 
  // if last word in IE_buf == "CW"  then disactivate "CW" and activate "CCW"
  //   else activate "CW" and disactivate "CCW"
  i1 = IE_bufSiz - 3;
    // printf(" IE_buf-last |%s|\n",&IE_buf[i1]);
  if(!strcmp (&IE_buf[i1], " CW")) {                  // 2014-04-10
    IE_ed1__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB2));
  } else {
    IE_ed1__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncUCB1));
  }



  // testen
  if(IE_bufSiz > 2) {
    // update CW|CCW
    // if..
    IE_cad_test__ ();

  } else {
    GL_temp_del_1 (DLI_DIR_TMP);  // delete temporary direction-arrow
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


  // printf("IE_ed1_CB EV=%d\n",GUI_DATA_EVENT);


  if(GUI_DATA_EVENT == TYP_EventEnter) {
    // Mouseklick into GroupEditWindow
    IE_ed1_mod (NULL, GUI_SETDAT_ES(TYP_EventPress,"PREVIEW"));
    return 0;
  }

  // TYP_EventRelease ..
  iKey = GUI_DATA_I1;
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
        IE_ed1__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncKill));
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
// simple CurveEditor
// benutzt - ebenso wie IE_ed2__ - IE_edWin1

static  MemObj   win0, bCW, bCCW;
static  int      winMode;             // 0=Typ_goPrim; with CW,CCW,MOD+,MOD-
                                      // else points only; no CW,CCW, ..
  MemObj   box0, box1;
  int      i1, mode;


  i1 = GUI_DATA_I1;
  // printf("IE_ed1__ %d\n",i1);


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

      winMode = IE_get_inp_TypAct();

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

      // Nur fuer Kontur (nicht nur f. Punkte):
      // if(IE_inpTypR[IE_inpInd] == Typ_goPrim) {
      if(winMode == Typ_goPrim) {
        bCW=GUI_button__(&box1, "CW", IE_ed1_mod, (void*)"CW", "a,a");
          MSG_Tip ("ED1CW");
        bCCW=GUI_button__(&box1, "CCW", IE_ed1_mod, (void*)"CCW", "a,a");
        GUI_set_enable (&bCCW, 0); //1=activ, 0=inaktiv
          MSG_Tip ("ED1CCW");
        GUI_button__(&box1, "MOD+", IE_ed1_mod, (void*)"MOD+", "a,a");
          MSG_Tip ("ED1MOD+");
        GUI_button__(&box1, "MOD-", IE_ed1_mod, (void*)"MOD-", "a,a");
          MSG_Tip ("ED1MOD-");
      }

      GUI_button__(&box1, " <<< ", IE_ed1_mod, (void*)"PREV-", "a,a");
        MSG_Tip ("ED1<<<");
      GUI_button__(&box1, " >>> ", IE_ed1_mod, (void*)"PREV+", "a,a");
        MSG_Tip ("ED1>>>");
      GUI_button__(&box1, "Delete", IE_ed1_mod, (void*)"DEL", "a,a");
        MSG_Tip ("MMundo");
      GUI_button__(&box1, "Exit", IE_ed1_mod, (void*)"Exit", "a,a");
      // GUI_button__(&box1, "Exit", IE_ed1__, &GUI_FuncKill, "a,a");
        MSG_Tip ("CADexit");
      GUI_button__(&box1, "OK", IE_ed1_mod, (void*)"OK", "a,a");
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
        IE_bufSiz = IE_bufMax;
        GUI_edi_Read (IE_buf, &IE_bufSiz, 0L, -1L, &IE_edWin1);
          // printf(" IE_buf now |%s|\n",IE_buf);
      }
      break;


    //---------------------------------------------------------
    case UI_FuncSet:
      // write text in IE_buf into window 
      // if(IE_ed1_win.stat != 0) {   // Win schon vorhanden ?
        GUI_edi_Write (&IE_edWin1, IE_buf, &IE_bufSiz, IE_bufMax);
          // printf(" IE_buf now |%s|\n",IE_buf);
        // Cursor -> EOL
        GUI_edi_setCpos (&IE_edWin1, -1L);
      break;


    //---------------------------------------------------------
    case UI_FuncUCB1:  // activate "CW" and disactivate "CCW" 1=activ, 0=inaktiv
      if(winMode == Typ_goPrim) {
        GUI_set_enable (&bCW, 1);
        GUI_set_enable (&bCCW, 0);
      }
      break;


    //---------------------------------------------------------
    case UI_FuncUCB2:  // disactivate "CW" and activate "CCW" 1=activ, 0=inaktiv
      if(winMode == Typ_goPrim) {
        GUI_set_enable (&bCW, 0);
        GUI_set_enable (&bCCW, 1);
      }
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
  int IE_ed1_rem (long *cPos, long txLen) {
//=====================================================================
// remove word out of IE_buf[IE_bufSiz]

  // long l1;

  // printf("IE_ed1_rem %ld %ld %ld\n",*cPos,txLen,IE_bufSiz);
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

    // printf("ex IE_ed1_rem |%s|\n",IE_buf);

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
    IE_ed1_rem (&dPos, txLen);
    // und NIX mehr zufuegen

    IE_bufSiz = strlen(IE_buf);
    *cPos = dPos;
    goto L_fertig;




  //======== handle "CW" ========================
  } else if(!strcmp(newTxt, "CW")) {
    // skip new CW
    if(!strcmp(cbuf, "CW")) return 0;
    // remove CCW
    if(!strcmp(cbuf, "CCW")) {
      IE_ed1_rem (&dPos, txLen);
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
      IE_ed1_rem (&dPos, txLen);
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
      // IE_ed1_rem (&dPos, txLen);
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
