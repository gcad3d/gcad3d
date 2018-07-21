//  xa_ed.c                        Reiter 2001-06-21
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
  rework ED_add_Line; remove ED_work_ENTER (use normal ED_work_CurSet ..)
  rework ED_work_STEP; use normal ED_work_CurSet;
  make toolbar for MAN; buttons STEP, Help (Help for active inputTyp(P/L/S/...)
  xa_ed: ED_add_objSel u ED_add_objTxt durch ED_add_Text ersetzen.
    ED_query_CmdMode noch aus wa/MainFrm.cpp holen.

-----------------------------------------------------
Modifications:
2013-06-25 ED_Run: skip applic-data- SECTION APPDAT
2001-06-26 ED_GetActLin ersetzt dr. GUI_Ed_RdLnAct.
2001-06-21 neu.RF.

-----------------------------------------------------
*/
/*!
\file  ../xa/xa_ed.c
\brief Neutral EditorFunctions (not Gtk-specific). 
\code
=====================================================
List_functions_start:

-------------- functions for memory only EDM_* --------------
ED_del__            delete all lines including <lnr> in mem
ED_del_block        delete Block; In: first line, last line.   UNUSED
ED_work_file        CALL-File-Funktion. PrgMod_skip_until_file
ED_cont_file        continue with file ..

-------------- functions for memory and editor ED_* --------------
ED_Read_Line        Read aus File|Mem
ED_GetNxtLin        get next line
ED_work_CurSet      new active lineNr; update 
ED_Get_LineNr       get active lineNr ED_lnr_act
ED_Reset            reset to srcLn 0; but no init.
ED_Init             reset in srcLn 0

-------------- functions for editor only EDE_* --------------
ED_update           Filesize changed - update Mem (Edi -> Memory)
ED_query_CmdMode    aktuelle Zeile into ED_buf1 einlesen, ihren Typ feststellen
ED_ck_lnStart       test if cursor is at first position in line
ED_ck_lnEnd         test if cursor is at last position in line

EDE_reload          load ?
ED_new_File         Ein Datei ins Hauptprog laden
ED_add_File         Insert Datei filnam in den Hauptbuffer an Position cpos
ED_add_Def          einen DefText (zB "P123=" in die Datei einfuegen;
ED_srcLn_add        save line in memory, Undo-List, work line.
ED_add_Line         eine komplette Line zufuegen und abarbeiten
ED_add_Text         entscheiden ob kompl. Zeile und nur zufuegen oder auch abarb
ED_Run              Wird vom GO/STEP/END-Button gerufen.
ED_newPos           cursor in new line; 

ED_work_ENTER       only for ED_add_Line; do not use ..
ED_work_STEP
ED_work_CAD
ED_work_GO
ED_work_END         das "RUN"
ED_work_exit        exit modify, work from curPos to end of model ..

ED_set_lnr_act      set ED_lnr_act
ED_get_lnr_act      get ED_lnr_act; see also ED_Get_LineNr
ED_set_cpos_lnr_act set ED_cpos to start of line ED_lnr_act

ED_lnr_bis__        ED_lnr_bis setzen
ED_query_mode       get ED_mode (ED_mode_step|ED_mode_go|ED_mode_enter)
ED_skip_end         reset ED_mode
ED_skip_start       keep ED_mode; set ED_mode = ED_mode_go;
ED_enter            set ED_mode=ED_mode_enter;
ED_step             set ED_mode=ED_mode_step;
ED_go               set ED_mode=ED_mode_go;
ED_lnr_reset        set ED_lnr_von=0; ED_lnr_bis=max;
ED_work_dyn         work block :DYNAMIC_DATA
ED_get_mac_fil      get APT_mac_fil

ED_addRange         decode ObjRanges and add them to MemTab(ObjRange)
ED_test__

see also:
../xa/xa_edi__.c       
../xa/xa_ed_mem.c      APED_*
../xa/xa_src.c         SRC_
AP_SRC_*         AP_SRC_mem_edi AP_SRC_edi_mem
AP_src*          AP_src_typ__ AP_src_new AP_src_mod_ed AP_src_sel_fmt 
AP_lNr_*
AP_APT_*         AP_APT_clean

UNUSED:
ED_work_PP
ED_PP_run
ED_work_Update      work from startingLineNr to end
ED_get_curPos       get cursorPosition, LineStartPosition, LineEndPosition.
ED_get_actPos       get position of active Line (ED_lnr_act) in mem
ED_del_on
ED_del_off
ED_parent_disp      display a selected ParentObject
ED_Init_All

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


 See also xa_ed_mem.c
 GTk-specific Funktions: ../gui_gtk/..



Funktion generell:

In AP_src ist definiert, wo Source liegt: AP_SRC_MEM od AP_SRC_EDI.
Wenn MAN aktiv ist (if(UI_InpMode == UI_MODE_MAN)) ist src = AP_SRC_EDI;
  vor memoryFunktionen (Suchen, ...) sollte man:


               VWR           CAD          MAN
 AP_src  =  AP_SRC_MEM    AP_SRC_MEM   AP_SRC_EDI




  // Editor -> memory
  AP_SRC_mem_edi ();

  ..

  // die Hauptdatei raus ins Editfenster (mem->Window)
  AP_SRC_edi_mem ();



UI_AP (UI_FuncSet,UID_Edit_Line)     // modify Line + set CurPos
UI_AP (UI_FuncSet,UID_Edit_Cpos)     // Cursor auf Line setzen
UI_AP (UI_FuncInsert,UID_Edit_Line)  // insert new line after current line
UI_AP (UI_FuncSet,UID_Edit_LnNxt)    // Cursor auf next Line setzen



===============================================
Compile:
cc -c xa_ed.c `gtk-config --cflags`
cl -c /I ..\include xa_ed.c

*/



#ifdef _MSC_VER
#include "MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>                       // isdigit

#include "../gui/gui_types.h"      // UI_Func..
#include "../ut/ut_geo.h"  
#include "../ut/ut_txt.h"      // term_buf
#include "../ut/ut_TX.h"       // TX_Print
#include "../ut/ut_os.h"       // OS_Wait
#include "../ut/ut_txfil.h"    // UTF_add_file
#include "../ut/ut_memTab.h"          // MemTab_..
#include "../ut/ut_cast.h"             // INT_PTR

#include "../ut/func_types.h"
#include "../gr/ut_DL.h"
#include "../gr/ut_gr.h"

#include "../ci/NC_Main.h"     // WC_Work__
// #include "../ci/NC_up.h"       // NC_up_Init_

#include "../db/ut_DB.h"


#include "xa_mem.h"               // mem_cbuf1
// #include "xa_ui.h"                // UID_ckb_nam u. ex func_types.h: UI_FuncGet
#include "../xa/xa_uid.h"         //  DLI_TMP UID_WinMain ...
#include "../xa/xa_ed.h"
#include "../xa/xa_undo.h"
#include "../xa/xa_app.h"         // PRC_IS_ACTIVE
#include "../xa/xa.h"                  // AP_modact_nam



  typedef_MemTab(ObjRange);



//===========================================================================
// Externals:
//
// aus ../xa/xa.c:
extern AP_STAT    AP_stat;               // sysStat,errStat..
extern int        WC_stat_bound;
extern int        AP_src;                // AP_SRC_MEM od AP_SRC_EDI
extern int        WC_modact_ind;         // -1=primary Model is active;
extern ColRGB     AP_defcol;
// extern int        AP_indCol;
extern int        WC_sur_ind;            // Index auf die ActiveConstrPlane
extern char      AP_ED_oNam[128];        // objectName of active Line


// aus xa_ui.c:
// extern GIO_WinEd winED;    // Editorfenster
extern void*     WIN_ED();    // EditorWindow
extern MemObj    winED;
extern int       UI_InpMode;
extern int       UI_EditMode;
extern int       xa_fl_TxMem;
// extern int       UI_InpSM;


// aus ut_txfil.c:
extern char       *UTF_FilBuf0;
extern long       UTF_FilBuf0Siz;
extern long       UTF_FilBuf0Len;  // die aktuelle Filesize


// aus ../ci/NC_Main.c
extern int       APT_stat_act;    // 0 = normal; 2 = search for jump-Label;
extern int       APT_line_act;    // die momentane APT-LineNr
extern int       UP_level;
extern int       APT_dispPL;


//-------------------------------------------------------
//  local Functions extern used von xa_ui_gr:
// extern void UI_GR_DrawInit ();
// extern void UI_GR_DrawExit ();




//===========================================================================

static char      AP_filnam[256];


static int       ED_mode;          // see ED_query_mode
static int       ED_mode_old;

static int       ED_delay = 0;




static FILE      *maclun = NULL;

       int       APT_mac_fil = OFF;



       int       ED_lnr_act;       // DIE AKTUELLE LINE-NR !!
static int       ED_lnr_von;
static int       ED_lnr_bis;
static int       ED_lnr_max;
       char      *ED_cpos;          // mem-position of last ED_Read_Line


long   UI_Ed_fsiz;      // Textsize



#define    ED_delIndmax   10 
// static int        ED_delVon[ED_delIndmax], ED_delBis[ED_delIndmax];
// static int        ED_delInd, ED_delStat;





//================================================================
  int ED_update (long ipos) {
//================================================================
/// Filesize changed - update Mem (Edi -> Memory) SOFORT !
// Die Aenderung kann nur hinter der curPos passiert sein.
// Daher hinter curPos alles weg.

// USE GUI_edi_getModif          <<<<<<<<<<<<<<<<<<<<

  int     lNr, im;
  long    l1, l2;


  im = GUI_edi_getModif (&winED);  //no=0;
  l1 = GUI_edi_getEof (&winED);

    // printf("ED_update ipos=%ld newFsiz=%ld fsiz=%ld\n",ipos,l1,UI_Ed_fsiz);

  // Filesize changed ? = INSERT !!!
  // l1 = gtk_text_get_length ((GtkText*)widget);
  if((l1 != UI_Ed_fsiz) ||
     (im)               ||
     (xa_fl_TxMem == 1))       {

    // Edi -> Memory
      // printf("  _update %d %d %d\n",l1,l2,lNr);
    // printf("ED_update reload\n");
    xa_fl_TxMem = 1;  // needs update
    ED_unload__ ();   // editor -> memory

    UI_Ed_fsiz = l1;
  }

  return 0;

}
 

//================================================================
  int ED_newPos () {
//================================================================
/// ED_newPos           cursor in new line;
/// use in UI_EdButtonRelease (click into Line)
/// use in UI_EdKeyRelease  (CurUp/Dwn)

  int     lNr;
  long    ipos, fSiz, l1;


  // printf("NNNNNNNNNNNN ED_newPos\n");


  // S/M: nix tun ...
  // if(UI_InpSM == ON) {
    // TX_Print("select object | key objID");
    // return 0;
  // }


  // CurPos holen
  UI_AP (UI_FuncGet, UID_Edit_Cpos, (void*)&ipos);
    // printf("ED_newPos curp=%d\n",ipos);


  // Filesize changed - update Mem (Edi -> Memory)
  ED_update (ipos);


  // aktuelle Zeilennummer holen
  // lNr = UTF_GetLnrPos (ipos);
// ACHTUNG: use always GUI_Ed_getLnr (not UTF_GetLnrPos);
//  die Filesize und cPos ist unterschiedlich, weil gtk Sonderzeichen nur
//  als 1 char behandelt !!!
// Man darf daher die cPos nicht zum Positionieren im Mem benutzen !!!!
  lNr = GUI_edi_getLnr (&winED);
  // printf("lNr=%d  ipos=%d\n",lNr,ipos);

  TX_Print("lineNr = %d; (F3=edit)",lNr);

  // get FilSiz
  fSiz = UTF_GetLen0();
  // printf("fSiz=%d\n",fSiz);


  // LineNr ins Menu
  l1 = lNr;
  UI_AP (UI_FuncSet, UID_ouf_lNr, (void*)l1);


/*

    //.............MAN + SM ............................
    if(UI_InpSM == ON) {

      // click ans EOF: S/M ausschalten ..
      if(fSiz <= ipos) {
        // GL_temp_del_all ();  DL_Redraw ();  // alle temp-Obj. loeschen
        DL_hili_off (-1L); // // unhilite alle Objekte
        // sind am EOF: SM ausschalten, Mode Add.
        UI_EditMode = UI_EdMode_Add;
        // crash ..
        // UI_AP (UI_FuncSet, UID_ckb_search, (void*)FALSE);
        gtk_toggle_button_set_state (GTK_TOGGLE_BUTTON (ckb_mdel), FALSE);
        goto Fertig;
      }
    }
*/



  // von der zuletzt bearbeiteten Zeile bis zu lNr anzeigen/abarbeiten
  ED_work_CurSet (lNr);


  //----------------------------------------------------------------
  Fertig:
  // lNr merken
  ED_set_lnr_act ((long)lNr);

  return 0;

}


//================================================================
  int ED_lnr_bis__ (int lNr) {
//================================================================
/// lNr >= 0      - setzen
/// lNr = -1      - lesen

  
  if(lNr >= 0) {
    ED_lnr_bis = lNr;
  } 

  return ED_lnr_bis;

}


//===========================================================================
  int ED_query_CmdMode () {
//===========================================================================
/// \code
/// aktuelle Zeile into ED_buf1 einlesen, ihren Typ feststellen
/// Retcod:
///         -3   deleted Line (1.char '_')
///         -2   commentline (1.char '#')
///         -1   empty line
///          0   Defline (eg "P20=..")
///          1   Direktcommand
///          2   IndirektCommand
/// \endcode

  int   irc;
  long  ll, lNr;
  char  s1[1024], *p1;


  // printf("ED_query_CmdMode \n");


  // act lineNr
  lNr = GUI_edi_getLnr (&winED);
    // printf(" actLnr=%ld\n",lNr);


  // get a copy of the active line
  GUI_edi_RdLn (s1, 1020, lNr, &winED);
    // printf(" actLn=|%s|\n",s1);


  ll = strlen(s1);
  if(s1[ll - 1] == '\n') --ll;
   

  if(ll <= 0) {
    irc = -1;
    goto Fertig;
  }

  // get first char
  p1 = s1;
  while(*p1 == ' ') ++p1;

  if(*p1 == '#') return -2;
  if(*p1 == '_') return -3;

  // feststellen, ob eine Definitionszeile vorliegt. Einfach nach "=" suchen.
  if(strchr(p1, '=')) {
    //TX_Print(" ist DefLine.");
    irc = 0;
    goto Fertig;
  }


  // Dann kanns nur Direktcommand sein.
  irc = 1;



  Fertig:
    // printf("ex ED_query_CmdMode rc=%d len=%d |%s|\n",irc,ED_buf1Len,ED_buf1);

  return irc;

}




/*
//======================================================================
  void ED_line_work (char* buf) {
//======================================================================
// die hereinkommende Zeile exekutieren.
// used nur when entering CR


  int  lNr, i1;

  printf("ED_line_work |%s|\n",buf);



  // Aktivieren des DC.
  UI_GR_DrawInit ();

  // die Zeilennunmmer der aktuellen Curpos holen
  lNr =  ED_Get_LineNr ();

  // die Zeile exekutieren
  i1 = WC_Work__ (lNr, buf);

  // Das Positionskreuz plazieren
  WC_setPosKreuz();                  // macht ein GL_Redraw

  UI_GR_DrawExit ();

  return;

}
*/


//================================================================
  int EDE_reload () {
//================================================================
// reload cleaned source into editor or memory.
// not important; to be done ..

  // printf("EDE_reload \n");

// exit(0);

  return 0;

}

 
//======================================================================
  int ED_new_File (char *filnam) {
//======================================================================
/// Ein Datei ins Hauptprog laden

  int    irc;

  printf("ED_new_File |%s|\n",filnam);



  // Datei in den Hauptbuffer (Mem) laden.
  UTF_clear_ ();
  irc = UTF_add_file (filnam);
  if(irc < 0) return irc;
    // UI_menCB (NULL, "new");

/*
  // Memory>Editfenster
  if(AP_src == AP_SRC_EDI)
    ED_load__ ();
  else
*/
    ED_Reset ();  // ED_lnr_act = 0;


  // clear Undo-List
  UNDO_clear ();

  DL_Init ();

  // clear Hide-List
  GA_hide__ (-1, 0L, 0);

  ED_Init ();

  IE_cad_ResetInputs (); // alle Inputfelder loeschen & goto Inputfeld 0

  return 0;

}



//======================================================================
  int ED_add_File (char *filnam, int cpos) {
//======================================================================
/// Insert Datei filnam in den Hauptbuffer an Position cpos.
/// cpos < 0: hintanhaengen !

// see also EDI_ins_file


  // vom Window ins Memory kopieren
  ED_unload__ ();

  // Datei in den Buffer 1 einlesen
  UTF_clear1 ();
  UTF_add1_file (filnam);

  // curpos holen

  // Buffer1 in die Hauptdatei einfuegen
  UTF_insert1 (cpos);

  // Memory > Editfenster
  ED_load__ ();
  UI_AP (UI_FuncWork, UID_WinEdit, NULL); // xa_fl_TxMem = 1;


  return 0;

}



//======================================================================
  int ED_add_Def (char *buf) {
//======================================================================
/// einen DefText (zB "P123=" in die Datei einfuegen;
/// muss am Beginn einer zeile liegen.

  int ityp;


  // printf("ED_add_Def: |%s|\n",buf);


  // aktuelle Zeile into ED_buf1 einlesen, ihren Typ feststellen
  // check ob active Line leer ist
  ityp = ED_query_CmdMode ();
  if(ityp != -1) {
    TX_Print(" Line must be empty ...");
    return -1;
  }

  // an der aktuellen curpos einfuegen
  UI_AP (UI_FuncSet, UID_Edit_Line, (void*)buf);

  return 0;

}


//================================================================
  int ED_srcLn_grp_add (char *txt, int *grpNr) {
//================================================================
/// \code
/// add sourcline to last created sourceline as undo-group
/// Input:
///   grpNr  grouplineNr; first = 1, next 2
///          (first object with grpNr = 0 created with ED_srcLn_add)
///          set to 1 at first call, increments automatic.
/// see also ED_srcLn_add (IE_outTxt, 1);
/// \endcode
 
  long     lNr;


  lNr = ED_get_lnr_act();

  // ED_work_CAD (lNr, txt);
  WC_Work__ (lNr, txt);
  ED_set_lnr_act (lNr + 1);       // set ED_lnr_act

  UNDO_grp_add (lNr, *grpNr);

  UTF_add_line (txt);

  ED_inc_lnr ();                  // set ED_lnr_act

  *grpNr += 1;

  return 0;

}


//================================================================
  int ED_srcLn_add (char *srcLn, int mode) {
//================================================================
/// \code
/// ED_srcLn_add             save line in memory, Undo-List, work line.
/// see also ED_add_Line IE_cad_OK
/// mode 0=display&save;
///      1=save;already displayed
/// \endcode

  long     lNr;


  // printf("ED_srcLn_add |%s| mode=%d\n",srcLn,mode);
  // printf(" UI_InpMode=%d\n",UI_InpMode);
  // printf(" lNr=%d APT_obj_stat=%d\n",ED_get_lnr_act(),WC_get_obj_stat());



  //================================================================
  // MAN
  if(UI_InpMode != UI_MODE_MAN) goto L_notMan;


    // add Entry in Undo-List
    lNr = GUI_edi_getLnr (&winED);
    UNDO_grp_add (lNr, 0); // AP_ED_lNr
    UNDO_upd__ ();
  
    // insert line
    UI_AP (UI_FuncSet, UID_Edit_Line, (void*)srcLn);
    UI_AP (UI_FuncSet, UID_Edit_Line, (void*)term_buf);

    return 0;



  //================================================================
  // VWR || CAD
  L_notMan:

  // get last-line-nr (empty line)
  lNr = ED_get_lnr_act();


  // create/display obj
  if(mode == 0) WC_Work__ (lNr, srcLn);


  // add Entry in Undo-List
  UNDO_grp_add (lNr, 0); // AP_ED_lNr
  UNDO_upd__ ();


  // add Line im Memory
  UTF_add_line (srcLn);


  // incr IE_ed_lNr (sonst wird Displayobj ueberschrieben !!
  ED_inc_lnr ();


  return 0;

}


//======================================================================
  int ED_add_Line (char *buf) {
//======================================================================
/// eine komplette Line zufuegen und abarbeiten
/// see also ED_srcLn_add


  int  cNr, lNr;
  long ipos;
  char *p1, *p2, *pEnd;


  // printf("EEEEEEEEEEEEEEEEEE ED_add_Line: |%s| %d\n",buf,strlen(buf));
  // printf(" AP_src=%d\n",AP_src);


 /*
  if(strlen(buf) < 80) {

    strcpy(ED_buf1, buf);
    strcat(ED_buf1, term_buf);   // add EOL; aus ../ut/ut_txfil.

    // an der aktuellen curpos einfuegen
    UI_AP (UI_FuncSet, UID_Edit_Line, (void*)ED_buf1);
    goto L_fertig;
  }




  // Zeilen laenger als 80 Char; zerteilen
  pEnd = buf + strlen(buf);
  p1 = buf;
  lNr = 0;




  L_next:
  ++lNr;
  p2 = p1 + 76;         // soll-LineLen = 76
  if(p2 >= pEnd) goto L_copy_line;



  while((*p2 != ',')&&(*p2 != ' ')) {
    --p2;
    if(p2 < p1) {
      TX_Error("ED_add_Line E001");
      return -1;
    }
  }
  ++p2; // mit ","




  L_copy_line:
  cNr = p2 - p1;
  if(lNr > 1) {
    strcpy(ED_buf1, "& ");
    strncat(ED_buf1, p1, cNr);
    ED_buf1[cNr+2] = '\0';

  } else {
    strncpy(ED_buf1, p1, cNr);
    ED_buf1[cNr] = '\0';
  }

  // printf("   |%s|\n",UTF_buf1);
  if(p2 < pEnd) {
    strcat(ED_buf1, "\\");
  }

  strcat(ED_buf1, term_buf);   // add EOL; aus ../ut/ut_txfil.
  UI_AP (UI_FuncSet, UID_Edit_Line, (void*)ED_buf1);



  p1 = p2;
  if(p1 < pEnd) goto L_next;
*/



  // add text to TextWindow
  if(AP_src == AP_SRC_EDI) {  // 1=AP_SRC_EDI    - MAN
    UI_AP (UI_FuncSet, UID_Edit_Line, (void*)buf);
    UI_AP (UI_FuncSet, UID_Edit_Line, (void*)term_buf);
    ED_lnr_act = GUI_edi_getLnr (&winED);

  } else {                    // 0=AP_SRC_MEM    - VWR | CAD
    UTF_add_line (buf);
    ED_lnr_act = UTF_GetLnrPos (UTF_GetLen0());
    // printf(" ED_lnr_act now %d\n",ED_lnr_act);
  }




  L_fertig:
  // execute last Line
  ED_work_ENTER ();

  return 0;
}


//======================================================================
  int ED_add_Text (int typ, long ind, char *buf) {
//======================================================================
// add text <buf> to active line in editor; cursorposition = insertposition.

// soll entscheiden ob kompl. Zeile und nur zufuegen oder auch abarb.
// soll ED_add_Line rufen (von Select und Indic gerufen werden)
// soll Ersatz f. ED_add_objSel u ED_add_objTxt sein.

  int  ityp;
  long l1, l2;
  char c1;
  char *cmdBuf;


  // printf("ED_add_Text typ=%d ind=%ld buf=|%s|\n",typ,ind,buf);


  // aktuelle Zeile into ED_buf1 einlesen, ihren Typ feststellen
  ityp = ED_query_CmdMode ();
  // -1=LeereZeile, 0=Defline, 1=Direktcmd, 2=Indirektcmd
    // printf(" ityp ln = %d\n",ityp);



  // wenn die zeile komplett leer ist, nix tun ..
  if(ityp < 0) {
    TX_Print ("%s",buf);
    goto Fertig;
  }


  // cmdBuf[0] = '\0';


  // // Wenn der Letzte Char der Zeile "=" oder "|" ist, kein "," zufuegen
  // c1 = ED_buf1[ED_buf1Len-1];
  // if(c1 == '=') goto AddNix;
  // if(c1 == '|') goto AddNix;

  // get char before cursor
  c1 = GUI_edi_RdChr (&winED, -1);
    // printf(" last char=|%c|\n",c1);


  l1 = strlen(buf);
  cmdBuf = (char*) MEM_alloc_tmp	((int)(l1 + 8));


  // add ' ' if last char is not '=' or '('
  if((c1 == '=')  ||
     (c1 == '('))    {
    cmdBuf[0] = '\0';

  } else {
    strcpy(cmdBuf, " ");
  }


  AddNix:
  strcat(cmdBuf, buf);


  WC_set_obj_stat (1);  // 1=workmode



  // wenn DefLine (ityp=0) nur zufuegen.
  // if(ityp == 0) {
    // an der aktuellen curpos einfuegen
    UI_AP (UI_FuncSet, UID_Edit_Line, (void*)cmdBuf);
    goto Fertig;
  // }


  Fertig:
  return 0;
}

/*
//======================================================================
  int ED_add_objSel (int typ, long ind, char *buf) {
//======================================================================

  Normal:
  ED_add_objTxt (buf);       // nein, normale Verarb.
  return 1;

}


//======================================================================
  int ED_add_objTxt (char *buf) {
//======================================================================

// hier kommt Text - zB "P##".
// Wenn letzter Char der akt. Zeile "=" oder "|", dann den Beistrich weg (++buf). 


  int   i1, lnLen, lNr, exFlag;
  long  ind;
  char  *cpos;


  printf("ED_add_objTxt: |%s|\n",buf);


  exFlag = OFF;


  // wenn buf "\r\n" ist, nur raus damit.
  // if(!strcmp(buf, "\r\n")) goto AddNix;
  if(!strcmp(buf, term_buf)) goto AddNix;



  // Aktuelle Zeile aus Edi lesen;
  UI_AP (UI_FuncGet, UID_Edit_Line, (void*)ED_buf1);

  UTX_CleanCR(ED_buf1); // CR und Blanks am Lineend weg
  lnLen = strlen(ED_buf1);

*/


/*
  // Aktuelle Zeile leer: exekutieren, dann CR dran und Zeile speichern.
  if(lnLen < 1) {

    // Skip 1. Char - wenn er ein ',' ist
    if((buf[0] == ',')||(buf[0] == ' ')) ++buf;
    //goto AddNix;

    // die Zeile exekutieren
    ED_line_work (buf);
    goto AddCR;
  }
*/

/*
  //============== Zeile ist NICHT leer ===============================
  // feststellen, ob eine Definitionszeile vorliegt.
  if(strchr(ED_buf1, '=')) {
    //TX_Print(" ist DefLine.");

    // Wenn der Letzte Char der Zeile ein "=", keinen "," zufÃ¼gen
    if(ED_buf1[lnLen-1] == '=') goto AddNix;

    goto AddKomma;
  }



  //if((ED_buf1[lnLen-1] == '=')||(ED_buf1[lnLen-1] == '|')) ++buf;
  //TX_Print("ED_add_objTxt lnr%d len %d |%s|%c|",lNr,lnLen,ED_buf1,ED_buf1[lnLe n-1]);
  
  // musste fÃ¼r "rp P23" zugefÃ¼gt werden, wo rp vom Keybd und P23 aus einer
  // Selektion kommt. Sonst kommt das CR ohne ein execute.
  strcpy(cmdBuf, buf);
  goto L_Raus;




  AddCR:
  printf("AddCR\n");
  strcpy(cmdBuf, buf);
  strcat(cmdBuf, term_buf);   // add EOL; aus ../ut/ut_txfil.
  goto L_Raus;


  AddKomma:
  // Aber nur, wenn Zeile nicht leer ist
  if(lnLen < 1) goto AddNix;
  if(ED_buf1[lnLen-1] == '|') goto AddNix;
  if(!strcmp(buf,"|")) goto AddNix;
  strcpy(cmdBuf, ",");
  goto AddText;


  AddNix:
  strcpy(cmdBuf, "");


  AddText:
  strcat(cmdBuf, buf);


  L_Raus:
  printf("--ausgeben:|%s|\n",cmdBuf);
  // an der aktuellen curpos einfuegen
  UI_AP (UI_FuncSet, UID_Edit_Line, (void*)cmdBuf);


  Fertig:
  return 0;
}
*/



/* UNUSED
//======================================================================
  int ED_Init_All () {
//======================================================================


  // printf ("AAAAAAAAAAAAAAAAAAA ED_Init_All AAAAAAAAAAAAAAAAAAAA\n");


  // den Eingabebuffer rücksetzen
  ED_delInd  = 0;
  ED_delStat = ON;

  // curposY = 0;

  // ED_enter ();

  return 0;

}
*/

//================================================================
  int ED_get_delay () {
//================================================================

  return ED_delay;

}
 

//======================================================================
  int ED_Init () {
//======================================================================
/*
Kein ED_Reset (); weil ED_Init immer in Zeile 1 gerufen wird -> Loop !
*/


  // printf ("ED_Init   [[[[[[[[[[[[[[[[[[\n");

  ED_lnr_max = 0;

  APT_mac_fil = OFF;
  ED_delay    = 0;


  if(maclun != NULL) {
    fclose (maclun);
    maclun = NULL;
  }

  return 0;
}


//================================================================
  int ED_set_eof () {
//================================================================
 
  ED_lnr_act = GUI_edi_getLnMax (&winED);

  return 0;

}


//==========================================================================
  int ED_Reset () {
//==========================================================================
// An den Beginn der Datei ruecksetzen; aber kein WC_Init (fuer JUMP's usw)



  // printf ("ED_Reset {{{{{{{{{{{{{{{{{{{{\n");

  /* mF->ED_Reset (); */
  // ED_FilPosAct  = ED_FilBuf;

  ED_lnr_act = 0;

  APT_mac_fil = OFF;

  return 0;
}



/* UNUSED
//===========================================================================
  void ED_del_on () {
//===========================================================================

  //TX_Write("ED_del_deakt");

  ED_delStat = ON;

}



//===========================================================================
  void ED_del_off () {
//===========================================================================

  //TX_Write("ED_del_deakt");

  ED_delStat = OFF;

}
*/


//================================================================
  int ED_del__ (int lnr) {
//================================================================
/// \code
/// ED_del__            delete line <lnr> and all following lines
/// Example:
/// lNr = 16; 
/// ED_del__ (lNr);  // delete code & displist for all lines starting with lNr
/// ED_work_CurSet (lNr);  // update;  or use APED_update__
/// \endcode

  char *cpos;
  long ll;


  printf("ED_del__ %d\n",lnr);

  cpos = UTF_GetPosLnr (&ll, lnr);
  if(!cpos) return -1;

  *cpos = '\0';
  UTF_FilBuf0Len = cpos - UTF_FilBuf0;

  ED_lnr_act = lnr; 

  // AP_mdl_modified_set ();

  return 0;

}


//================================================================
  int ED_del_block (int mode, char *fromLn, char *toLn) {
//================================================================
/// ED_del_block delete Block; In: first line, last line.
/// Input:
///   mode = 0   do not delete toLn
///          1   also delete toLn


  long     i1;
  char     *cFrom, *cTo;


  // printf("ED_del_block |%s|%s|\n",fromLn,toLn);

  // get cPos of line starting <fromLn>
  cFrom = UTF_find_tx1 (&i1, fromLn);
  if(cFrom == NULL) return -1;

  // get cPos of line starting <toLn>
  cTo = UTF_find_tx1 (&i1, toLn);
  if(cTo == NULL) return -1;

  if(mode == 1) cTo = UTF_GetnextLnPos (cTo);

  // delete block
  UTF_del_text (cFrom, cTo);

  return 0;

}


//==========================================================================
  int ED_query_mode () {
//==========================================================================
/// \code
///   value                setFunc
/// 0=ED_mode_step         ED_step()
/// 1=ED_mode_go    = Run  ED_go()
/// 3=ED_mode_enter = Edit ED_enter()
/// \endcode


// 0=ED_mode_step
//   ED_step();     used by ED_work_ENTER
//
// 1=ED_mode_go:     kein Redraw ! (GL_Redraw skips it)  
//   ED_go()     ;  used by "RUN" (ED_work_CurSet) !
//
// 3=ED_mode_enter
//   ED_enter();    used by CAD.








  // printf("||||||||||||<===========================================\n");
  // printf("ED_query_mode %d\n",ED_mode);


  return ED_mode;

}


//================================================================
  int ED_set_mode (int newMode) {
//================================================================

  ED_mode = newMode;

  return 0;

}


//==========================================================================
  int ED_skip_end () {
//==========================================================================


  // printf("||||||||||||<===========================================\n");
  // printf("ED_skip_end ED_mode=%d -> ED_mode_old=%d\n",ED_mode,ED_mode_old);


  ED_mode = ED_mode_old;

  return 0;
}



//==========================================================================
  int ED_skip_start () {
//==========================================================================
/// used by ED_cont_file, ED_work_file <jump, MAC, CALL, return>


  // printf("||||||||||||<===========================================\n");
  // printf("ED_skip_start ED_mode=%d\n",ED_mode);


  // if(ED_mode != ED_mode_go)  
  ED_mode_old = ED_mode;
  ED_mode = ED_mode_go;

  return 0;
}




//==========================================================================
  int ED_enter () {
//==========================================================================
/// CR by Keyboard; es daf kein CRLF zugefuegt werden ! (sonst doppelt)
/// used by: IE_activate, IE_cad_init1, ED_work_ENTER
/// see ED_query_mode



  // printf ("ED_enter\n");

  ED_mode = ED_mode_enter;

  return 0;
}



//==========================================================================
  int ED_step () {
//==========================================================================
/// used after ED_work_ENTER, ED_work_CurSet(!)


  // printf ("ED_step\n");

  ED_mode = ED_mode_step;

  return 0;
}


//==========================================================================
  int ED_go () {
//==========================================================================
/// used by ED_work_CurSet, ED_PP_run, ED_work_PP.


  // printf("||||||||||||<===========================================\n");
  // printf("ED_go\n");

  ED_mode = ED_mode_go;

  return 0;
}



/* DO NOT USE; replaced by ED_get_lnr_act
//================================================================
  int ED_Get_LineNr ()  {
//================================================================
///  die Zeilennunmmer der aktuellen Curpos (ED_lnr_act) holen.
/// Im Batchmode ein Dummy.


  long ipos;


  // leider muss man Edit->Memory, dort dann LF's zaehlen.
  // Koennte beim Kontursuchen zu langsam sein (hier werden viele
  // Zeilen automat. erzeugt).

  if(AP_src == AP_SRC_EDI) {     // MAN

    // vom Window alles ins Memory kopieren (nur wenn was veraendert wurde)
    ED_unload__ ();

    ED_lnr_act = GUI_edi_getLnr (&winED);


  } else {         // VWR & CAD

    // CurPos holen
    UI_AP (UI_FuncGet, UID_Edit_Cpos, (void*)&ipos);
    // printf("ipos=%ld\n",ipos);

    // aktuelle Zeilennummer holen
    ED_lnr_act = UTF_GetLnrPos (ipos);

  }

  // nur im Batchmode:
  // ++ED_lnr_act;



  // printf("EEEEEEEEEEEEEEEEEE ex ED_Get_LineNr %d\n",ED_lnr_act);

  return ED_lnr_act;

}
*/


//==========================================================================
  int ED_GetNxtLin (int *lnr, char *lbuf) {
//==========================================================================
/// Liefert naechste Zeile und ZeilenNr.
/// Als Retcod = ihre Länge
/// Dzt nur used for read Fortsetzungszeilen  von APT_decode_cv


  // printf("ED_GetNxtLin %d\n",*lnr);

  *lnr = ED_Read_Line (lbuf);

  // printf(" ex ED_GetNxtLin |%s| %d\n",lbuf,*lnr);

  return strlen(lbuf);
}


//================================================================
  char* ED_Read_cPos () {
//================================================================
// returns position of active line (from ED_Read_Line())

  return ED_cpos;

}


//================================================================
  int ED_Read_Line (char* buf)  {
//================================================================
/// \code
/// Get a copy of the next codeline.
/// - normal:               get codeline ED_lnr_act from memory
/// - if(APT_mac_fil==1):   get next line from file
/// The size of buf must be mem_cbuf1_SIZ !
/// Retcode:
///    0-n  LineNr; first Line=0, second line=1 ....
///   -1    EOF
///   -2    ??  irgendwas mit MODEL ???
/// get mepos with ED_Read_cPos
/// \endcode


  int    i1;
  long   iLen;


  // printf("EEEEEEEEEEEEEEEEE ED_Read_Line APT_mac_fil=%d\n",APT_mac_fil);
  // printf(" APT_stat_act=%d UP_level=%d\n",APT_stat_act,UP_level);
  //("ED_Read_Line ED_lnr_act=%d ED_FilPosAct=%d",ED_lnr_act,ED_FilPosAct);



  //============== Read aus File ==========================================
  if(APT_mac_fil == 0) {      // 0=InputFromFile; 1=InputFromMemory.

    L_f_next:
    if(maclun == NULL) goto L_file_exit;

    if (fgets (buf, mem_cbuf1_SIZ, maclun) == NULL) goto L_file_exit;

      if(buf[0] == '#') goto L_f_next;    // skip #-Lines from File
      UTX_CleanCR (buf);
      if(buf[0] == '\0') goto L_f_next;    // skip empty Lines from File
      // printf("ED_Read_Line aus File |%s|\n",buf);

/*
      if(!strcmp(buf, ":DYNAMIC_AREA")) {
        fclose (maclun);
        maclun = NULL;
        APT_mac_fil = OFF;
        ED_skip_end ();
        // mit dem Trick wird im Stepmode nach dem CALL gestoppt.
        strcpy (buf, "EXIT");
        // buf[0] = '#';

      } else if(!strncmp(buf, "MODEL ", 6)) {
*/
      // if(!strcmp(buf, "END")) {
        // goto L_file_exit;


                     // 12345678901234
      if(!strncmp(buf, "SECTION MODEL ", 14)) {
        // starting new (internal) submodel
        // printf(" new (internal) submodel |%s|\n",buf);
        // save submodel > tmp/Model_<name>
        i1 = Mod_file2model (buf, maclun);
        if(i1 < 0) { strcpy (buf, "EXIT"); return 2; }
        goto L_f_next;
      }

    return 2;                    // bei 1 reset ..
    // goto L_fertig;




    //----------------------------------------------------------------
    L_file_exit:
      // fclose (maclun);
      // maclun = NULL;
      // APT_mac_fil = OFF;
      ED_file__ (2, NULL);  // close active file ..
      ED_skip_end ();
      if(UP_level >= 0) {
        // mit diesem END wird ein CALL from File gestoppt
        // ist fuer CALL beenden erforderlich !
        strcpy (buf, "END");

// /* 2013-09-20
      } else {
        // mit dem Trick wird im Stepmode nach dem CALL gestoppt.
        // ist fuer subModel beenden erforderlich !
        // - wird in ED_Run benutzt.
        strcpy (buf, "EXIT");
// */
      }

      // EOF - wieder auf die normale Eingabe rücksetzen
      return 2;

  }




  //========== process active: read in editor ============================
  if(PRC_IS_ACTIVE) {
    ++ED_lnr_act;
    iLen = GUI_edi_RdLn (buf, mem_cbuf1_SIZ, ED_lnr_act, &winED);
    if(iLen < 0) {
      return -1;
    }
    // remove \n                         // 2013-04-11
    L_remEol:
    if(buf[iLen - 1] == '\n') {
      --iLen;
      buf[iLen] = '\0';
      goto L_remEol;
    }
    if(buf[iLen - 1] == '\r') {
      --iLen;
      buf[iLen] = '\0';
      goto L_remEol;
    }

    goto L_fertig;
  }




  //========== Read aus dem Edit-Buffer (Mem) ============================
  ++ED_lnr_act;
  ED_cpos = UTF_GetLinNr (buf, &iLen, ED_lnr_act);

 
  if(iLen < 0) {
    return -1;
  }


  L_fertig:
  // printf("ex ED_Read_Line Zeile %d: |%s| len=%d\n",ED_lnr_act,buf,iLen);

  return ED_lnr_act;


}


//=============================================================
  int ED_lnr_mac (int mode) {
//=============================================================
/// 0 = save ED_lnr_bis  (beim CALL) und set to 999999
/// 1 = restor ED_lnr_bis (nach dem END MAC)

static int lnr1, lnr2;

  if(mode == 0) {
    lnr1 = ED_lnr_von;
    lnr2 = ED_lnr_bis;
    ED_lnr_reset ();

  } else {
    ED_lnr_von = lnr1;
    ED_lnr_bis = lnr2;
  }

  return 0;

}


//==========================================================================
  int ED_lnr_reset () {
//==========================================================================


  ED_lnr_von = 0;
  ED_lnr_bis = 999999;

  return 0;

}


/*
//=======================================================================
  void ED_work_PP () {
//=======================================================================

  printf("ED_work_PP\n");

  // Init Postprocessing
  WC_Init_all (1);

  ED_lnr_von = 0;
  ED_lnr_bis = 999999;

  // work
  ED_go ();
  // ED_Reset ();
  APT_Reset ();
  ED_Run ();

  // Close und Reset
  WC_Init_all (0);

  ED_Reset ();      // ED_lnr_act = 0;
  ED_work_END (1);  // mit restore hidden

}
*/


//================================================================
  int ED_work_dyn () {
//================================================================
// ganz am Beginn der bearbeitung dynBlock im memory suchen;
// wenn einer vorhanden: abarbeiten, dann aus dem mem rausloeschen !!
// dynBlock beginnt am Programmanfang und geht bis :DYNAMIC_DATA

// Vorherige Loesung war :DYNAMIC_AREA am Programmende.
// Warum geht DYNAMIC_AREA (am ProgEnde) nicht:
 // weil "work_detail_for_Ditto" via ED_Run direkt aus der Datei gelesen wird.
 // Da braucht man auch nix wegloeschen ...
 // Und zuerst hinten lesen geht ned ..


  int  irc, iStat;
  long lNr;
  char *cbuf, *c1buf, *cPos, *cp1;


  // printf("ED_work_dyn ---------\n");


  APT_reset_view_stat ();

  
  // Position & Zeilenummer der "DYNAMIC_AREA" suchen
  cPos = UTF_find_tx1 (&lNr, ":DYNAMIC_DATA");
  // cPos = UTF_find_tx1 (&lNr, ":DYNAMIC_AREA");
  if(cPos == NULL) return 0;    // keine daten ..


  // erster Start nach load from File ...
  GA_hide__ (-1, 0L, 0);   // clear existing HIDE-Table




  cbuf = NULL; // Startpos.
  // cbuf = cPos;


  iStat = 0;

  // get pos of nxt line
  for(;;) {
    c1buf = UTF_GetnextLnPos (cbuf);
    if(c1buf == NULL) break;

    // fetch line out of buffer
#ifdef _MSC_VER
    cp1 = _memccpy (mem_cbuf1, c1buf, '\n', mem_cbuf1_SIZ);
#else
    cp1 = memccpy (mem_cbuf1, c1buf, '\n', mem_cbuf1_SIZ);
#endif


    if(cp1) { --cp1; *cp1 = '\0'; }  // replace \n durch \0
      // printf(" dynDat |%s|\n",mem_cbuf1);


    // printf("vor WC_Work__ |%s|\n",mem_cbuf1);
    if(!strcmp(mem_cbuf1,":DYNAMIC_DATA")) break;



    //----------------------------------------------------------------
    // word GATAB bis ENDGATAB                  see also ED_Run
    if(!strcmp(mem_cbuf1,"GATAB")) {
      GA_load__ (NULL);
      iStat = 1;
      goto L_nxt_ln;
    }

    if(iStat == 1) {
      if(!strcmp(mem_cbuf1,"ENDGATAB")) {
        iStat = 0;
        goto L_nxt_ln;
      }

      GA_load__ (mem_cbuf1);
      goto L_nxt_ln;
    }



    //----------------------------------------------------------------
    // Zeile abarbeiten (sofort WC_Work1, da keine SubModelCalls)
    WC_Work1 (-1, mem_cbuf1);


    L_nxt_ln:
    cbuf = c1buf;
  }


  // printf("delete DYNAMIC_AREA\n");
  // UTF_del_rest(":DYNAMIC_AREA");


  // delete gesamten DYNAMIC_DATA-Block
  cPos += 14;
    // printf("delete DYNAMIC_DATA off=%d\n",UTF_offset_(cPos));
  // printf("cPos=|%s|=end_of-cPos\n",cPos);
  UTF_del_start (cPos);

  // printf("ex ED_work_dyn\n");
  return 0;

}


//================================================================
  int ED_work_exit () {
//================================================================
// ED_work_exit        exit modify, work from curPos to end of model ..

  TX_Print("- modify => add");

  UI_EditMode = UI_EdMode_Add;

  // l1 = ED_get_lnr_act();
  // ED_work_CurSet (l1 - 1);

  // display alle elemente, cursor auf letzte zeile:
  ED_work_CurSet (999999);

  // goto eof
  ED_goto__ (-1L);


  return 0;

}


//================================================================
  int ED_work_END (int mode) {
//================================================================
/// das "RUN"
/// mode unused !


  int    irc, lNr, bNr, wrkStat, mTyp;
  double d1;
  char   *cbuf, *c1buf, *cp1;


  // printf("EEEEEEEEEEEEEEEEEEE ED_work_END %d EEEEEEEEEEEEEEEEE\n",mode);
  // printf(" AP_modact_nam=|%s| APP_stat=%d\n",AP_modact_nam,AP_stat.APP_stat);

  // printf(" TSU_mode=%d\n",TSU_get_mode());
  // DB_dump_ModNod ();


  UI_CursorWait (0);     // wait-cursor

  AP_errStat_reset (1);  // clear Error     2011-08-31

  ED_Reset ();           // ED_lnr_act = 0; 2004-02

  // clear DB & DL, but not in addOn-progs
  // delete all basic-models (mdb_dyn) all and model-names (mdb_nam)
  if(AP_stat.APP_stat == 0) {    // 0=no plugin active
    // clear DB & DL
    AP_Init2 (0);
    // primary model always has modnr=-1
    WC_modact_ind = -1;
  }

  // PRC_init ("cut1");  // (re)init active process

/*
  // reset SubmodelLevel
  // Mod_load_bas (NULL,NULL);


  // // Init Levelcounter in WC_Work__
  // WC_Work__ (0, NULL);

  // init ModelNodes
  DB_StoreModNod (0,0,-1);


  // // Reset DB-dyn-objects
  // DB_Init (2);
  // DB_Init(1);
  DB_Init(0);

  // // clear TexRefTab
  // Tex_Init_Ref ();


  APT_Init(); // nur f. APT_line_act=0 f. Typ_Color-Record
*/

  // get active state
  wrkStat = WC_get_obj_stat(); // 0=perm, 1=workmode
  WC_set_obj_stat (0);  // set to perm
  
/*
  // reset DL
  GR_Init1 ();


  // jetzt die DefaultColor nochmal setzen ! (macht eine DL-Eintrag !)
  // DEFCOL-Abarbeitung wurde mit GR_Init1 geloescht !
  // if(AP_indCol != 5)
  AP_indCol = GL_DefColSet (&AP_defcol);
*/

  // check if code exists
  if(UTF_GetLen0() < 3) goto L_run1;


  // // save Hidelist --> File
  // GA_hide_fil_tmp (1);


  // // alle SubmodelCalls als mdb_dyn-Records speichern
  // Mod_get_namFil (fNam, 0);


  // // die TexBas-DL-Records neu erzeugen
  // Tex_loadBas ();




  // ========= load subModels ==========================
  // nur fuer export VRML muessen alle subModels zuerst geladen werden
  // der folgende Block ist nur im Run_for_Tesselation-Mode aktiv !
  // not for normal viewing mode

  if(TSU_get_mode() != 1) goto L_run1;


  // scan gesamten APT-Buffer nach Modelnames. Not recursiv.
  Mod_get_names ();


  // get nr of basic models
  bNr = DB_get_ModBasNr();
    printf(" _ModBasNr=%d\n",bNr);
  if(bNr < 1) goto L_run1;
  

  // save dynam. data
  // DL_sav_dynDat ();
  DB_save__ ("");
if(WC_modact_ind >= 0) TX_Error("**** TODO: DB_save__ only saves primary Model");


  // scan rekursiv die SourceFiles aller basicModels;
  // load Submodels as basicModels (mdb_dyn-Record's - DB_StoreModBas)
  irc = Mod_get_namAll();
    // printf(" n._namAll %d %d\n",irc,ED_lnr_act);

  // if(irc < 0) return irc;
  if(irc < 0) {
    // dzt nicht bekannt, in welcher APT-Zeile der Fehler ist.
    // entweder speichern, (sollte aber eigentlich im DL-Record sein);
    // daher hier einfacher im Text suchen ...
    TX_Error("subModel not found ..");
    ED_lnr_act = -1;
    DL_Redraw ();
    goto L_done;
  }


  // Load Models of mdb_dyn-Records; Load Models; Reihenfolge=seqNr.
  Mod_load_allsm ();

  // reload Hidelist from File
  // GA_hide_fil_tmp (2);

  // reload dynam. data
  // DL_load_dynDat ();
  DB_load__ ("");




  // ========= subModel loaded ==========================



  //----------------------------------------------------------------
  L_run1:
    // printf("============ start normaler progRun =========\n");
    // DB_dump_ModNod ();
    // DL_DumpObjTab ();
    // DB_dump_ModBas ();
    // DB_dump_ModRef ();


  // RUN (abarbeiten); ACHTUNG: loescht die DL !
  irc = ED_work_CurSet (999999);
    // printf(" n._CurSet %d %d\n",irc,ED_lnr_act);
  if(irc < 0) return irc;
  // printf(" END nach ED_work_CurSet \n");



  // wenn kein VIEW gesetzt war, ein Autoscale machen .. ("butRes")
  if(APT_get_view_stat() < 1) UI_GR_ScalAuto (0);


  // if(UI_InpMode == UI_MODE_CAD) return ED_lnr_act;
  if(AP_src == AP_SRC_MEM) {
    // printf("ex ED_work_END %d\n",ED_lnr_act);
    goto L_done;
  }


  //----------------------------------------------------------------
  // MAN is active:

  // // nach Form-input nicht ans Ende..
  // if(UI_GR_Sel_Filter(-1) != 0) goto L_exit;


/* 2012-09-11
  // die Hauptdatei raus ins Editfenster (mem->Window)
  // (erst nach remove DYNAMIC_AREA)
  // ACHTUNG - setzt Cursor in letzte Zeile !!
  lNr = ED_lnr_act;
  ED_load__ ();            // mem -> editor
  ED_lnr_act = lNr;// reset ED_lnr_act (wird von UI_AP geloescht!)
 
  // CAD benutzt AP_ED_lNr !
  ED_set_lnr_act ((long)ED_lnr_act);
*/


/*
  ED_lnr_von = 0; 
  ED_lnr_bis = 30000; 

  // wenn Cursor am EOF und last Char kein LF: ein LF eingeben
  UI_AP (UI_FuncSet, UID_Edit_CEOF, NULL);

  APT_Reset ();

  // gesamtes Display loeschen
  GL_Delete (0L);
  GR_Init1 ();

  ED_go ();  // Workmode setzen

  ED_Run (); // Work ..
*/


  // // den Cursor in die letzte Zeile + scroll
  // UI_AP (UI_FuncSet, UID_Edit_Cpos, NULL);


  // // wenn CAD-Modify aktiv: in den Modus Add gehen
  // UI_CAD_activate ();


  //----------------------------------------------------------------
  L_done:

  UI_CursorWait (1);    // reset cursor
  // TX_Write (".. rework finished ..");  // schlecht, kommt 2 x pro run ..

  WC_set_obj_stat (wrkStat);  // reset


  // If VWR & CAD: unhilite all (last obj)
  if(AP_src != AP_SRC_EDI) {
    DL_hili_off (-1L);   // unhilite alle Objekte

  } else {   // MAN
    // proceed/scroll to EOF; nicht im Inputmode.
    // if(UI_GR_Sel_Filter(-1) == 0) {
      ED_goto__ (-1L);  // goto eof
    // }
  }

  // VWR-only: hide Planes if wanted
  if(UI_InpMode == UI_MODE_VWR) {
    if(APT_dispPL == OFF) {  
      DL_disp_PL (1);
    }
  }


  // update unhilite, hide planes ..
  DL_Redraw ();


  L_exit:
    // printf("ex ED_work_END %d\n",ED_lnr_act);
  return ED_lnr_act;

}
  

//==========================================================================
  void ED_work_ENTER () {
//==========================================================================
/// es wurde CR am Keyb eingegeben

  
  // printf("=================================\nED_work_ENTER\n");
  

  ED_enter ();
  
  ED_Run ();
  
  ED_step ();

  // den Focus wieder ans Edit-Window
  // UI_AP (UI_FuncFocus, UID_WinEdit, NULL);

  return;

}   
  


//==========================================================================
  int ED_work_STEP () {
//==========================================================================

// see also ED_newPos


  int     i1, lNr;
  long    ipos;


  printf("ED_work_STEP\n");

/* 2008-06-15
  ED_step ();    // ED_mode = ED_mode_step;

  ED_Run ();
*/

  // S/M: nix tun ...
  // if(UI_InpSM == ON) {
    // TX_Print("select object | key objID");
    // return 0;
  // }


  // CurPos holen
  UI_AP (UI_FuncGet, UID_Edit_Cpos, (void*)&ipos);
    // printf("ED_newPos curp=%d\n",ipos);


  // Filesize changed - update Mem (Edi -> Memory)
  ED_update (ipos);



  // aktuelle Zeilennummer holen
  ED_lnr_act = GUI_edi_getLnr (&winED);
    // printf(" ED_lnr_act=%d\n",ED_lnr_act);


  lNr = ED_lnr_act + 1;

  // von der zuletzt bearbeiteten Zeile bis zu lNr anzeigen/abarbeiten
  ED_work_CurSet (lNr);

  // Den Cursor an den Anfang der nxt Line
  // UI_AP (UI_FuncSet, UID_Edit_LnNxt, NULL);
  // GUI_Ed_setCnxtL (&winED);
  EDI_set_lnr (lNr);


  return ED_lnr_act;

}   
  

//===========================================================================
  int ED_work_CAD (int lNr, char *cbuf) {
//===========================================================================
/// 1) bei jedem Tab wenn alle Inputs fertig
/// 2) mit OK (CR) wenn alle Inputs fertig
/// RetCod:
///   -3     obj not yet complete


  int      irc, tmpStat;
  long     l1, dl1, dl2;


  tmpStat = WC_get_obj_stat();

  // printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
  // printf("ED_work_CAD %d |%s| tmpStat=%d\n",lNr,cbuf,tmpStat);



  // LineNr ins Menu
  l1 = lNr;
  UI_AP (UI_FuncSet, UID_ouf_lNr, (void*)l1);


  // delete alles von der aktiven zeile weg -
  DL_disp_reset (lNr);


  AP_errStat_set (0);        // reset errStat


  dl1 = GL_GetActInd();

  // printf("->WC_Work__ lNr=%d txt=|%s|\n",IE_ed_lNr, cbuf);
  irc = WC_Work__ (lNr, cbuf);
    // printf(" nach WC_Work__ irc=%d cbuf=|%s|\n",irc,cbuf);

  dl2 = GL_GetActInd();
    // printf(" dl1=%ld dl2=%ld\n",dl1,dl2);



  if(WC_get_obj_stat() != 0) {           // temp od permanent -
      // printf(" temp..\n");

    // save temporary:
    // printf(" dl1=%d dl2=%d\n",dl1,dl2);
    // DL_hili_on (-1L); // loeschen der Hili-Tabelle
    // DL_hili_on (-2L); // das zuletzt bearb. Elem. hiliten
    if(dl2 > dl1) {    // subModels only
      DL_hili_on (-2L);       // das zuletzt bearb. Elem. hiliten
       DL_pick_set (dl2, ON);
    }


  } else {
      // printf(" perm..\n");

    // save permanent:
    // nur wenn ueberhaupt ein Obj erzeugt wurde ..
    if(dl2 > dl1) {
      // save
      DL_hili_on (-2L);       // das zuletzt bearb. Elem. hiliten
      // if it is a temporyr element: set to not pickable
      if(DL_get_dbi(dl2) <= 0) DL_pick_set (dl2, ON);  // set to nopick

      // // set box invalid
      // UT3D_pt_setFree (&AP_box_pm1);
    }

  }


  ED_enter();                 // 2011-12-18
    // after CTLG not korrect !

  // // Update Refsys-display
  // UI_upd_Refs ();


  return irc;

}


//===========================================================================
  int ED_work_CurSet (int new_lnr_act) {
//===========================================================================
/// \code
/// Cursor wurde in Editor-Line new_lnr_act gesetzt.
/// Alle Zeilen bis einschliesslich Zeile new_lnr_act anzeigen (ausfuehren).
/// Obj der Zeile new_lnr_act hiliten
/// 
/// von der zuletzt bearbeiteten Zeile bis zu lNr new_lnr_act abarbeiten
///   new_lnr_act   -1           query active lineNr (ED_lnr_act)
///                 UT_INT_MAX   work from ED_lnr_act to end
/// \endcode

// Example:
// ED_work_CurSet (lNr);          // go back to line lNr
// ED_work_CurSet (UT_INT_MAX);   // work until end of model
//
// ED_lnr_act     ist die CurPos im Editor
// new_lnr_act    ist die gewuenschte neue CurPos.
// mode: 0=down, 1=up.

  int    irc, i1, mode, typ;
  long   l1, dbi, dli;
  char   s1[32];


  // printf("ED_work_CurSet %d\n",new_lnr_act);


  // new_lnr_act = -1: nur aktuelle Line abfragen
  if(new_lnr_act == -1) return ED_lnr_act;


  // printf("sssssssss ED_work_CurSet new=%d act=%d\n",new_lnr_act,ED_lnr_act);
  // printf(" TSU_mode=%d\n",TSU_get_mode());


  if(ED_lnr_act < 1)
    WC_Work__ (0, NULL);       // init level


  // reset hilite of last obj
  if(UI_InpMode == UI_MODE_MAN)             // 2012-10-29
    DL_disp_hili_last (OFF);


  // wurde der Cursor zurueckgesetzt (nur was wegloewschen)
  // oder nach vorn (bis einschliesslich neue zeile abarbeiten) ?
  if(new_lnr_act > ED_lnr_act) goto L_work;



  //=========================================================
  // Cursor zurueckgesetzt (nur wegloeschen)
  //=========================================================
  // printf("ED_work_CurSet - nur wegloeschen\n");
  mode = 1;  // up


  // clear Error, if errorline is later ...       2013-03-09
  if(AP_stat.errStat != 0) {
    if(new_lnr_act <= AP_stat.errLn) AP_errStat_reset (1);
  }


  // Grafik: alles nach Line "new_lnr_act" loeschen
  // (Objekte von Line new_lnr_act bleibt)
  if(PRC_IS_ACTIVE) {                            // 2013-03-28
    // reset process
    sprintf(s1,"RESET %d",new_lnr_act);
    PRC__ (-1, s1);
  } 

  // delete all display-objectsts dli>new_lnr_act
  DL_disp_reset (new_lnr_act);

  goto L_fertig;



  //=========================================================
  // abarbeiten von ED_lnr_act (incl)  bis new_lnr_act; 
  //=========================================================
  // gleiche Zeile: nur diese eine diese Zeile abarbeiten !
  L_work:
  mode = 0;


  // ED_lnr_von = von;
  ED_lnr_von = ED_lnr_act-1;  // ++ED_lnr_act in ED_Read_Line !
  ED_lnr_bis = new_lnr_act-2;   // -2: do not work active Line ..
    // printf("ED_work_CurSet ED_lnr_von=%d ED_lnr_bis=%d\n",
            // ED_lnr_von,ED_lnr_bis);



  if(TSU_get_mode() == 0) {       // 0=Normal 1=Store
    UI_CursorWait (0);            // display watch
    if(PRC_IS_ACTIVE) {
      // reset process
      sprintf(s1,"RESET %d",ED_lnr_act);
      PRC__ (-1, s1);
    } else  {      
      DL_disp_reset (ED_lnr_act);   // alles nach incl.Line "new_lnr_act" loeschen
    }

  } else {
    i1 = TSU_exp_Open ("");            // Start export mainModel
    if(i1 < 0) return i1;
    // printf(" END mit TSU_mode=Store\n");
  }


  ED_go ();            // modus ED_mode = ED_mode_go;
  irc = ED_Run ();     // work all lines
  ED_step ();          // reset modus ED_mode = ED_mode_step;

    // printf(" ->->-> nach ED_Run - new_lnr_act=%d\n",new_lnr_act);



  //=========================================================
  L_fertig:


  // check Error
  irc = AP_errStat_get();
  if(irc) goto L_done_9;



  if(new_lnr_act < 999999) ED_lnr_act = new_lnr_act;



  // hilite Obj of line ED_lnr_act (previous line)
  // but only if we not in an active edit.process in MAN (if line is not empty)
  // Not if Search/Delete is active !
  // if(AP_src == AP_SRC_EDI) { 
  // printf(" _CurSet - APT_obj_stat=%d\n",WC_get_obj_stat());
  if(UI_InpMode == UI_MODE_MAN) {           // 2011-04-10

    if(WC_get_obj_stat() == 0) GL_temp_del_1 (DLI_TMP);   // remove tempObj
    DL_disp_hili_last (ON);             // hilite last obj of DL

  } else if(UI_InpMode == UI_MODE_CAD) {    // 2011-08-25
    if(IE_get_Func() < 0) {
      // no CAD-func active ..
      GL_temp_del_1 (DLI_TMP);             // remove tempObj
    }

  // } else if(UI_InpMode == UI_MODE_VWR) {    // 2011-08-25
    // GL_temp_del_1 (DLI_TMP);               // remove tempObj


  }
  // TX_Print("End [0 - %d]",ED_lnr_act);



  // RefSys testen / korrigieren
    // printf(" mode=%d ED_lnr_act=%d\n",mode,ED_lnr_act);
  if(mode == 1) {           // nur bei GoUp
    // DL_get_iLast (&dli, &iTyp, ED_lnr_act-1);
    // l1 = ED_lnr_act-1;
    l1 = ED_lnr_act;        // 2011-07-20
    irc = DL_Get_dli_lNr (&dli, &l1);
    if(irc >= 0) {           // nur wenn lastObj = geometr.Obj 
      DL_setRefSys (dli);   // RefSys entprechend DL-Record setzen
    } else {
      if(WC_sur_ind != 0) NC_setRefsys (0);
    }
  }



  //----------------------------------------------------------------
  L_done_9:
  // AP_ED_lNr setzen; used bei CAD-functions !
  ED_set_lnr_act ((long)ED_lnr_act);

  // disp LineNr
  UI_AP (UI_FuncSet, UID_ouf_lNr, PTR_INT(ED_lnr_act));

  // display active Constr.Plane and en/dis-able the rel|abs checkboxes
  UI_Set_ConstPl_Z ();

  UI_CursorWait (1);    // reset cursor

  DL_Redraw ();

    // printf("ex ED_work_CurSet ED_lnr_act=%d\n",ED_lnr_act);

  return 0;

}

/*
//===========================================================================
  int ED_work_aux (int bis) {
//===========================================================================
// von der zuletzt bearbeiteten Zeile bis zu lNr bis abarbeiten

  int    ckb_view_stat;


  static int bis_old=0;


  printf("xxxxxxxxxxx  ED_work_aux von %d bis %d\n",bis_old,bis);


  // bis = -1: nur aktuelle Line abfragen
  if(bis == -1) return bis_old;



  // ED_lnr_act;

  // if(bis_old == bis) return 0;


  // von "von" weg alles loeschen
  // WC_actPos_reset (bis);


  if(bis <= bis_old) {
    goto L_fertig;
    // bis_old = bis;
    // return 0;
  }

  // reset all hilite
  

  // ED_lnr_von = von;
  ED_lnr_von = bis_old;
  ED_lnr_bis = bis-1;

  ED_go ();
  APT_Reset ();
  ED_Run ();
  ED_step ();

  printf(" ->->-> nach ED_Run - bis=%d\n",bis);

  L_fertig:
  bis_old = bis;

  // hilite Obj of line bis; delete objects of all following lines
  WC_actPos_reset (bis);


  // // View aktiv: toggle
  // UI_AP (UI_FuncGet, UID_ckb_view, (void*)&ckb_view_stat); // 0=aktiv
  // printf("??????????? view=%d\n",ckb_view_stat);
  // if(ckb_view_stat != 0) {
    // DL_hide_chg ();
    // DL_Redraw ();
  // }


  // bei Goto EOF: Cursor auf diese Position setzen
  if(bis >= 99999) UI_AP (UI_FuncSet, UID_Edit_Cpos, NULL); // ans EOF


    // cp1 = UTF_GetLinNr (txbuf, &llen, bis);
    // startPos = UTF_offset_ (cp1);
    // UI_AP (UI_FuncSet, UID_Edit_Cpos, (void*)startPos);



  return 0;

}
*/


//==========================================================================
  int ED_work_GO () {
//==========================================================================


  // int  ipos;
  // char c1;


  // printf("ED_work_GO\n");


  // reset
  // ED_work_aux (0);


  // RUN
  // ED_work_CurSet (ED_Get_LineNr());
  ED_work_CurSet (ED_get_lnr_act());



/*
  ED_lnr_von = 0; 

  // aktuelle Zeilennummer holen
  ED_lnr_bis = ED_Get_LineNr ();

  --ED_lnr_bis;
  --ED_lnr_bis;


  //TX_Print("GO von %d bis Zeile %d",ED_lnr_von,ED_lnr_bis);

  // wenn Cursor am EOF und last Char kein LF: ein LF eingeben
  // UI_AP (UI_FuncSet, UID_Edit_CEOF, NULL);

  ED_go ();
  APT_Reset ();
  ED_Run ();
  // GL_Redraw ();
*/

  return ED_lnr_act;

}




//=========================================================================
  int ED_cont_file () {
//=========================================================================

  printf("ED_cont_file\n");


  if(maclun == NULL) {
    TX_Error (" ED_cont_file - file not open");
    return -1;
  }


  APT_mac_fil = ON;
  ED_skip_start ();  // bei work_file gibts keinen STEP-mode


  return 0;

}


//================================================================
  int ED_file__ (int mode, char *fNam) {
//================================================================
///  0 = clear/init;
///  1 = openNext
///  2 = closeActive
///  3 = rewindActive (zB fuer JUMP)

static FILE *fTab[16];
static int  actLev=0;

  int  i1, fStat;



  // printf("ED_file__ %d lev=%d |%s|\n",mode,actLev,fNam);



  //----------------------------------------------------------------
  if(mode != 0) goto L_1;
  for(i1=0; i1<actLev; ++i1) {
    if(fTab[i1]) {
      fclose(fTab[i1]);
      fTab[i1] = NULL;
    }
  }
  if(maclun) fclose (maclun);
  maclun = NULL;
  actLev = 0;
  APT_mac_fil = OFF;
  return 0;




  //----------------------------------------------------------------
  L_1:                            // open new
  if(mode != 1) goto L_2;
  if(actLev >= 16) {
    TX_Error("ED_file__ E001");
    return -1;
  }

  fTab[actLev] = maclun;
  ++actLev;

  // Das neue Eingabefile oeffnen
  if ((maclun = fopen (fNam, "r")) == NULL) {
    TX_Error (" open file %s\n",fNam);
    return -1;
  }




  APT_mac_fil = ON;
  return 0;


  //----------------------------------------------------------------
  L_2:                          // close active
  if(mode != 2) goto L_3;
  if(actLev < 2) {
    if(maclun) fclose (maclun);
    maclun = NULL;
    APT_mac_fil = OFF;
    actLev = 0;

  } else {
    if(maclun) fclose (maclun);
    --actLev;
    maclun = fTab[actLev];
    fTab[actLev] = NULL;
  }
  return 0;



  //----------------------------------------------------------------
  L_3:                            // rewindActiveFile
  if(maclun) rewind(maclun);
  ED_lnr_act = 0;
  return 0;

}
 

//=========================================================================
  int ED_work_file (char* filnam) {
//=========================================================================
/// nächster call an ED_Read_Line soll nicht die nächste aktuelle Zeile liefern
///  sondern zuerst das File filnam abarbeiten
///  CloseFile macht auch ED_Read_Line.
/// 
/// Bei subModel UP_level=-1


  int   irc;

  // printf("ED_work_file |%s| UP_level=%d\n",filnam,UP_level);

  strcpy(AP_filnam, filnam);

  // open file <filnam> as maclun, set APT_mac_fil = ON
  irc = ED_file__ (1, filnam);
  if(irc < 0) return irc;


  // // Das Eingabefile oeffnen
  // if(maclun == NULL) {
    // //TX_Print(" open file");
    // if ((maclun = fopen (filnam, "r")) == NULL) {
      // TX_Error (" beim Oeffnen Datei %s\n",filnam);
      // return -1;
    // }
  // } else {
    // rewind (maclun);
  // }


  ED_skip_start ();  // bei work_file gibts keinen STEP-mode


  // Fertig:

  return 0;
}


//=========================================================================
  int ED_set_lnr_von (int lNr) {
//=========================================================================
/// see also ED_set_lnr_act

  // printf("ED_set_lnr_von %d\n",lNr);

  ED_lnr_von = lNr;

  return 0;

}


//=========================================================================
  int ED_set_lnr_act (long lNr) { 
//=========================================================================
/// see also ED_set_lnr_act EDI_goto_lnr


  // printf("EEEEEEEEEEEEEEEEEEEEEEEEE ED_set_lnr_act %d\n",lnr_act);

  // ACHTUNG: Abfrage nur mit CAD verwenden (in MAN via GUI_edi_getLnr)
  if(lNr < 0) lNr = UTF_GetLnrPos (lNr);

  // ED_lnr_act = lnr_act;
  ED_lnr_act = lNr;

  return 0;

}


//================================================================
  int ED_set_cpos_lnr_act () {
//================================================================
/// ED_set_cpos_lnr_act set ED_cpos to start of line ED_lnr_act

  long   il;

  ED_cpos = UTF_GetPosLnr (&il, ED_lnr_act);

  return 0;

}


//=========================================================================
  int ED_get_lnr_act () {
//=========================================================================
/// see also AP_lNr_get APT_line_act

  // printf("ED_get_lnr_act %d\n",ED_lnr_act);

  return ED_lnr_act;

}


//=================================================================
  int ED_inc_lnr () {
//=================================================================
/// was AP_lNr_inc

  ++ED_lnr_act;

  return 0;

}


/* UNUSED ?
//================================================================
  char* ED_get_actPos () {
//================================================================
/// get position of active Line (ED_lnr_act) in mem

  long    ll, lNr;


  // get active LineNr ED_lnr_act
  lNr = ED_get_lnr_act();

  // get active Position
  return UTF_GetPosLnr (&ll, lNr);

}


//================================================================
   int ED_get_curPos (int *actPos, int *lnStart, int *lnEnd) {
//================================================================
// ED_get_curPos       get cursorPosition, LineStartPosition, LineEndPosition.

  int    cpos, cpos1, cpos2;

  cpos = GUI_edi_getCpos (&winED);

  GUI_Ed_getLpos  (lnStart, lnEnd, actPos, &winED);

  return 0;

}
*/


//================================================================
  int ED_ck_lnEnd () {
//================================================================
// ED_ck_lnEnd       test if cursor is at last position in line
//   following blanks are ignored !
// RetCode    0    YES, cursor is at last position in line
//            1    NO; cursor is inside line

// see also ED_ck_lnStart ED_get_curPos

  int    iOff;
  char   c1;

  iOff = 0;

  L_noAmoi:
    c1 = GUI_edi_RdChr (&winED, iOff);

    if(c1 == ' ') { ++iOff; goto L_noAmoi; }

    if((c1 == 10)||(c1 == 0)) return 0;

    return 1;

}


//================================================================
  int ED_ck_lnStart () {
//================================================================
// ED_ck_lnStart       test if cursor is at first position in line
// RetCode    0    YES, cursor is at first position in line
//            1    NO; cursor is inside line

// see also ED_ck_lnEnd ED_get_curPos

  char   c1;

  c1 = GUI_edi_RdChr (&winED, -1);

  if((c1 == 10)||(c1 == 0)) return 0;

  return 1;

}


//=========================================================================
  int ED_get_mac_fil () {
//=========================================================================

  // printf("ED_get_mac_fil %d\n",APT_mac_fil);

  return APT_mac_fil;

}


/* unused
//================================================================
   int ED_work_Update (int lNr) {
//================================================================
// ED_work_Update      work from startingLinenr to end

// Input:
//   lNr       startingLineNr (excluding this Line !)

// see also ED_work_CurSet


  printf("ED_work_Update %d\n",lNr);

  DL_disp_reset (lNr);          // clear objects with higher lineNrs 

  // ED_set_lnr_act (lNr);      // set ED_lnr_act
  ED_set_lnr_von (lNr);         // set ED_lnr_von

  ED_lnr_bis__ (999999);        // set ED_lnr_bis

  ED_go ();                     // ED_mode = ED_mode_go;
  ED_Run ();
  ED_step ();                   // ED_mode = ED_mode_step;

  DL_Redraw();                  // update the display

  return 0;

}
*/


//================================================================
  int ED_Run ()  {
//================================================================
// work lines from ED_lnr_von until ED_lnr_bis
// ED_lnr_von can be -1
// ED_lnr_bis can be 999997 (until eof)


  int      rc, i1, lNr, llen, istat, gaStat, imode;
  long     l1, ipos;
  short    is1;
  char     *cbuf, *cpos, *p1;
  FILE     *fpo=NULL;


  // printf("\nRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR \n");
  // printf("RRRRRRRRRRRRRRRR ED_Run ED_mode=%d\n",ED_mode);


  cbuf = mem_cbuf1;

  // set to perm
  WC_set_obj_stat (0);       // 2013-04-13 else problem with insert in MAN

  // den boundary-Schalter abfragen
  UI_AP (UI_FuncGet, UID_ckb_bound, (void*)&WC_stat_bound);
  // printf("WC_stat_bound=%d\n",WC_stat_bound);


  //----------------------------------------------------------------
  // next ln ..
  L_start_RUN:
    // printf("L_start_RUN: ED_lnr_max=%d\n",ED_lnr_max);
    // printf("  ED_lnr_act=%d ED_lnr_von=%d ED_lnr_bis=%d\n",
              // ED_lnr_act,ED_lnr_von,ED_lnr_bis);


  // ED_lnr_act = ED_lnr_von !!
  ED_lnr_act = IMAX(ED_lnr_von,0);   // am Anfang kommt -1! 
     // printf("  ED_lnr_act=%d ED_lnr_von=%d\n",ED_lnr_act,ED_lnr_von);


  gaStat = 0;
  lNr = 0;


  //----------------------------------------------------------------
  NoAmoi:
  if(lNr < 0) goto L_EOF;

  // naechste Zeile holen (get a copy !)  returns ++ED_lnr_act
  lNr = ED_Read_Line (cbuf);
    // printf("nach ED_Read_Line %d |%s|\n",lNr,cbuf);
    // printf(" AP_src=%d UI_InpMode=%d ED_mode=%d\n",AP_src,UI_InpMode,ED_mode);



  L_check1:


  // handle dialog-windows
  // Nur MAN,edit
  // if(ED_mode != ED_mode_go) {    // der ED_mode stimmt leider nicht ...
  if(AP_src == AP_SRC_EDI) {        // MEM=0, EDI=1
    // skip the Form-records "DLG "...
    if(!strncmp(cbuf, "DLG ", 4)) {              // normal weiter ...
      cbuf[0] = '#';   // ignore Line ..
      goto L_decode;
    }

/*
    // FORMEND in mode MAN,ED_mode_step: step back and write block into File.
    if(!strcmp(cbuf, "FORMEND")) {
      // FORM_write ...
      rc = DLG_sav1 (lNr, cbuf);
      cbuf[0] = '#';   // now done, ignore Line ..
      goto L_decode;
    }
*/
  }




  if(!strcmp(cbuf, ":DYNAMIC_DATA")) {            // goto L_EOF;
    // printf(" :DYNAMIC_DATA\n");
    if(TSU_get_mode() != 0) {       // 0=Normal 1=Store
      // nun sind die Defaults geladen; zB AD_defCol
      TSU_exp_Defaults ();
    }


 
  } else if(!strncmp(cbuf, ":ATTRIB:", 8)) {
    GA_decode__ (&cbuf[8]);
    // comment this line out; only in primaryModel.
    if(WC_modact_ind < 0) {
      cpos = ED_Read_cPos ();
      *cpos = '_'; 
    }
    goto NoAmoi;   // normal weiter ...


/*
  } else if(!strncmp(cbuf, "FORM", 4)) {
    // FORM_write ... (if not yet exists)
    rc = DLG_sav2 (&lNr, cbuf);
    if(rc == 0) goto NoAmoi;  // normal weiter ...
    goto L_exit;
*/


  } else if(!strcmp(cbuf, "ENDWRITE")) {
    // skip ENDWRITE (kommt nur beim interaktiven Testen ...
    // if(AP_src == AP_SRC_EDI) {                // MAN
      // // Den Cursor an den Anfang der nxt Line
      // UI_AP (UI_FuncSet, UID_Edit_LnNxt, NULL);
    // }
    goto NoAmoi;   // normal weiter ...




  } else if(!strncmp(cbuf, "SECTION ", 8)) {
    // skip application-data SECTION when load as external subModel. 2013-06-25
    L_sec_nxt:
      // skip all lines until "SECTIONEND"
      lNr = ED_Read_Line (cbuf);
      if(strncmp(cbuf, "SECTIONEND", 10)) goto L_sec_nxt;
      goto NoAmoi;   // continue ..



  } else if(!strncmp(cbuf, "WRITE ", 6)) {
    if(APT_stat_act != 0) goto L_decode; // nicht bei skip_until_line ..
    // printf(" WRITE fn=|%s| APT_stat_act=%d\n",&cbuf[6],APT_stat_act);
    // filename >> ED_buf1

    i1 = IO_wri_ope (cbuf);   // open outputfile
    if(i1 < 0) goto L_exit; // 
/*
    i1 = APT_str2fnam (ED_buf1, &cbuf[6]);
    // open exportfile
    if(i1 == 1) {   // relativ --> tmp/
      UTX_safeName (ED_buf1, 0);  // change '. ', do not change '/'
      strcat(ED_buf1, ".write");
    }
      // printf(" exportfile=|%s|\n",ED_buf1);
    if((fpo=fopen(ED_buf1,"w")) == NULL) {
      TX_Error("Error open file |%s|",ED_buf1);
      ED_lnr_act = -1;
      return -1;
    }
*/
    L_nxt_export:
      lNr = ED_Read_Line (cbuf);
        // printf(" export %d |%s|\n",lNr,cbuf);
      if(lNr < 1) {
        TX_Error("ENDWRITE missing ...");
        goto L_exit;
      }   

      i1 = IO_wri_wri (cbuf);    // 0=done (ENDWRITE received)
      if(i1 == 0) goto NoAmoi;   // normal weiter ..
/*
      if(!strcmp(cbuf, "ENDWRITE")) {
        // close exportfile
        if(fpo) {fclose(fpo); fpo = NULL;}
        // MAN: WRITE soll interaktiv sofort weiterlaufen bis ENDWRITE ....
        if(AP_src == AP_SRC_EDI) {                // MAN
          // den Cursor setzen.
          cpos = UTF_GetPosLnr (&ipos, lNr+1);
            // printf(" set cursor %d %c\n",lNr,*cpos);
          ipos = UTF_offset_(cpos);
          UI_AP (UI_FuncSet, UID_Edit_Cpos, (void*)ipos);
        }
        goto NoAmoi;   // normal weiter ...
      }
      // write line >> exportfile
      fprintf(fpo, "%s\n",cbuf);
*/
      goto L_nxt_export;


  } else if(!strcmp(cbuf, "EXIT")) {
    strcpy(cbuf, "#EXIT");
    goto L_exit; // nach EOF aus ReadFile.
  }



  //----------------------------------------------------------------
  if(gaStat == 1) {                      // see also ED_work_dyn
    if(!strcmp(cbuf,"ENDGATAB")) {
      gaStat = 0;
      goto NoAmoi;
    }
    GA_load__ (cbuf);
    goto NoAmoi;


  } else {           // decode GATAB bis ENDGATAB
    if(!strcmp(cbuf,"GATAB")) {
      GA_load__ (NULL);             // init
      gaStat = 1;
      goto NoAmoi;
    }
  }



  //----------------------------------------------------------------
  L_decode:
  if(lNr > 0) {

    if(lNr > ED_lnr_max) ED_lnr_max = lNr;
      // printf(" ED_lnr_max=%d\n",ED_lnr_max);


    // extract objectName -> AP_ED_oNam
    APED_onam_cut (cbuf, AP_ED_oNam);

    // work ..
    rc = WC_Work__ (lNr, cbuf);
      // printf(" nach WC_Work__ rc=%d\n",rc);
    if(rc == -2) goto L_exit;  // ? nix tun ..


    istat = AP_errStat_get();

    if(istat == 2) {
      AP_errStat_reset (1);  // reset Error   2011-08-16
      AP_Init2 (1);           // clear DB & DL
      // DB_Init (0);        // clear gesamten DB_CDAT    2009-06-20
      goto L_start_RUN;      // realloc-alles no amoi
    }


    if((rc != 0)||(istat != 0)) {
      // stop - error occured ..
        // printf(" rc=%d istat=%d\n",rc,istat);
        // printf(" rc von WC_Work__=%d |%s|\n",rc,AP_mod_fnam);
        // printf(" ErrLn=%d |%s|\n",AP_stat.errLn,cbuf);
      if(APT_mac_fil == ON) {
        TX_Print("*** Error in Line %d - Submodel %s",lNr,AP_filnam);
      } else {
        TX_Print("*** Error in Line %d",lNr);
      }
      // imode = UI_ask_mode();
      imode = UI_InpMode; // UI_MODE_VWR|UI_MODE_CAD|UI_MODE_MAN
      if(imode == UI_MODE_VWR) // nur in Viewer
        TX_Print("select CAD or MAN to modify or key F3 or Delete");
      else // CAD, MAN:
        TX_Print("Edit (F3=Sysedit) or key Delete");
      // goto Fertig;
      return -1;        // 2011-09-04
    }


    // printf("ED_mode=%d,lNr=%d,ED_lnr_bis=%d\n",ED_mode,lNr,ED_lnr_bis);


    if(ED_mode == ED_mode_go) {                              // "RUN"

      // check if last line is active
        // printf(" lNr=%d ED_lnr_bis=%d\n",lNr,ED_lnr_bis);
      if(lNr > ED_lnr_bis) goto Fertig;


      // check for Esc-key ....
      // es sollte eine Gelegenheit zum Unterbrechen einer Loop gegeben werden
      //is1 = GetAsyncKeyState (VK_ESCAPE);         // 0=normal <= = pressed.
      is1 = UI_askEscape();
      // printf(" askEscape1 %d\n",is1);
/*
      // test if inputMode entered ...
      if(UI_GR_Sel_Filter(-1) > 0) {
        // stop for input ("INPUT form")
          printf(" XXXXXXX stop for input XXXXXXX lNr=%d\n",lNr);
        // set lNr as active line
        ++lNr; // skip line INPUT
        cpos = UTF_GetPosLnr (&l1, lNr);
        l1 = UTF_offset_ (cpos);
          printf(" offset=%d\n",l1);
        UI_AP (UI_FuncSet, UID_Edit_Cpos, (void*)l1);
        ED_set_lnr_act (lNr);
        ED_set_lnr_act (lNr);      // set AP_ED_lNr
        // UI_AP (UI_FuncSet, UID_ouf_lNr, (void*)&lNr);
        goto L_exit; //Fertig;
      }
*/
      if (is1 >= 0) {
        // no escape ..
        if(ED_delay != 0) {
          OS_Wait (ED_delay);
        }
        goto NoAmoi;      // <<<<<<<<<<< hier gehts weiter ...


      } else {
        // TX_Print(" hat Escape gedrückt; nochmal probieren");
        MSG_pri_0 ("XAesc");
        // printf(" Escape gedrueckt; nochmal probieren\n");
        // er hat Escape gedrückt; 500 ms warten, wenn dann Esc noch immer
        // gedrückt ist: Stop
        OS_Wait (50);  // 500
        //is1 = GetAsyncKeyState (VK_ESCAPE);         // 0=normal <= = pressed.
        is1 = UI_askEscape();
        // printf(" askEscape2 %d\n",is1);
        if(is1 >= 0) {
          goto NoAmoi;
        }
      }
    }
  }

  L_EOF:
  WC_EOF ();




  //==================================================================
  Fertig:
  // in nicht aktiven subModels: done.
  if(WC_modact_ind >= 0) return 0;

  // Das Positionskreuz plazieren (nicht in VWR)
  // if(UI_ask_mode() != UI_MODE_VWR) WC_setPosKreuz();
  // if(UI_ask_mode() != UI_MODE_VWR) WC_actPos_disp();
  if(UI_InpMode != UI_MODE_VWR) WC_actPos_disp();

  //GL_Redraw ();
  // UI_GR_DrawExit ();

  // // Update Refsys-display
  // UI_upd_Refs();


  if(ED_mode == ED_mode_step) {

    // hilite last - active - LineObject
    // DL_disp_hili (ED_lnr_act);

    // Den Cursor an den Anfang der nxt Line
    // UI_AP (UI_FuncSet, UID_Edit_LnNxt, NULL);
    // GUI_Ed_setCnxtL (&winED);
    ++ED_lnr_act;
    EDI_set_lnr (ED_lnr_act);
    // ++ED_lnr_act;

  }

  // ??
  ED_enter ();



  L_exit:
    // printf("exit ED_Run\n");

  return 0;

}


//==========================================================================
  void ED_set_delay (int delaytime) {
//==========================================================================
/// An den Beginn der Datei rücksetzen; aber kein WC_Init (für JUMP's usw)


  //TX_Print ("ED_set_delay %d",delaytime);

  ED_delay = delaytime;

}


/*
//================================================================
  void ED_PP_run (char* ppnam) {
//================================================================

  int    i1;
  char   fNam[80], *w;
  char   docTit[80];
  char   cmdBuf[256];

  //-------------------------------------------------------
  //TX_Print("CMainFrame::ED_PP_run |%s|",ppnam);

  TX_Print("nur mit Lizenz verfuegbar");
  return;

  if(!strcmp(ppnam, "ISO")) goto L_Start;

  // nach dem PP-File suchen
  strcpy (cmdBuf, OS_get_bas_dir ());
  strcat (cmdBuf, "\\pp\\");
  strcat (cmdBuf, ppnam);
  strcat (cmdBuf, ".exe ");

  if((OS_checkFilExist (cmdBuf, 1)) == NO) {
    TX_Error("Postprozessor %s nicht verfuegbar",ppnam);
    return;
  }

  // Init Postprocessing
  L_Start:
  WC_Init_all (1);

  ED_lnr_von = 1;
  ED_lnr_bis = 30000;

  // work
  ED_Reset ();
  ED_go ();
  ED_Run ();

  // Close und Reset
  WC_Init_all (0);

  // den Programm-Filename lesen
  //CFrameWnd* pF = (CFrameWnd*) AfxGetMainWnd();  // Get MainWindow
  //ASSERT (pF);
  //CDocument* cDoc = pF->GetActiveDocument(); // Get Document
  //CString docTit = cDoc->GetTitle();
  strcpy(docTit,"test"); // DUMMY !!!!
  TX_Print("Title=%s",docTit);

  // den Filetyp apt weg, stattdessen ncd.
  // Wenn kein Filetyp: nix tun.
  strcpy (fNam, docTit);
  w = fNam;
  for (i1=0; i1<strlen(fNam); ++i1) {
    ++w;
    //printf("%d |%c|\n",i1,*w);
    if(*w == '.') {
      strcpy (w, ".ncd");
      goto L12;
    }
  }
  strcat (fNam, ".ncd");

  // PP-Aufruf zus.bauen
  L12:
  //strcpy (cmdBuf, "call ");
  strcpy (cmdBuf, "start ");
  strcat (cmdBuf, OS_get_bas_dir ());
  strcat (cmdBuf, "\\pp\\");
  strcat (cmdBuf, ppnam);
  strcat (cmdBuf, ".exe ");
  strcat (cmdBuf, fNam);

  if(!strcmp(ppnam, "ISO")) {

    // Display Ergebnis (nur Test) (statt pp)
    //strcpy (cmdBuf, "notepad pp\\pp.dat");
    strcpy (cmdBuf, "notepad ");
    strcat (cmdBuf, OS_get_bas_dir ());
    strcat (cmdBuf, "\\pp\\pp.dat");

  }

  // Den Befehl starten
  TX_Print("%s",cmdBuf);
  system(cmdBuf);

}
*/


//================================================================
  int ED_addRange (MemTab(ObjRange) *rTab, char *lBuf) {
//================================================================
// decode ObjRanges and add them to MemTab(ObjRange)
// Input:
//   lBuf      eg "P10-20 P50 V1-20 V33 L1"

  int    iTyp;
  long   lInd, lNr;
  char   *cp1, cDel, *cEnd;


  // printf("ED_addRange |%s|\n",lBuf);

  cp1 = lBuf;
  cEnd = strchr(cp1, '\0');

  L_nxtExpr:
  UTX_pos_skipLeadBlk (cp1);
  if(cp1 >= cEnd) return 0;


  iTyp = AP_typ_typChar (toupper (*cp1));
    // printf(" typ=%d\n",iTyp);
  ++cp1;

  lInd = atoi(cp1);
    // printf(" ind=%d\n",lInd);

  // skip int
  while(isdigit(*cp1)) ++cp1;

  // skip blanks
  UTX_pos_skipLeadBlk (cp1);

  // test for '-'
  if(*cp1 == '-') {
    ++cp1;
    lNr = atoi(cp1);
    lNr -= lInd;
      // printf(" lNr=%d\n",lNr);

    // skip int
    while(isdigit(*cp1)) ++cp1;

  } else {
    lNr = 1;
  }


  // add range to PRG_internTab
  UTO_addRange (rTab, iTyp, lInd, lNr);

  // proceed with next word ..
  goto L_nxtExpr;


  return 0;

}


//================================================================
  int ED_test__ () {
//================================================================
 
  printf("ED_test__ ============================= \n");
  UTF_dump__ ();
  return 0;
}


/*=========================== EOF ==========================================*/
