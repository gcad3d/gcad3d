// xa_ui_cad.c                   Toolbar CAD
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

BUG modify eg S42=BSP1 "L22 L23" or S42=BSP1 "L22 L23 VAL(1.5)"
  cannot decode line; problem is IE_txt2parG cannot return nr-of-groupObjs.


CAD-Insert sollt VOR aktLn einfuegen !
  Den Rest of Model in Datei auslagern; nun weiter gleich wie bei "Add".
  Mit Exit-Insert die Datei hintanfuegen, fertig.


ISOparametric Curve from Surf", "ISO",
  S0=ISO A22 0.5 CX
  1) BUG: s0 !
  2) sollte bei den curves sein ! (kann aber line | circ liefern !)


-----------------------------------------------------
Modifications:
2015-02-18 IE_cad_init_men_1 - do not disact. menu. RF.
2011-06-22 IE_ck1,iOver,"overWrite" removed.
2001-08-10 Neufassung mit gtk.

-----------------------------------------------------
*/
/*!
\file  ../xa/xa_cad_ui.c
\brief GUI CAD toolbar 
\code
=====================================================
List_functions_start:

UI_cad               create CAD-Window (Bar)
IE_sel_CB_1          Selektion eines Objekts gemeldet (core reports selection)
IE_gr_key_CB         keypress in grafic-window
IE_cad_OK            OK-Butt. pressed (store)

IE_cad_get_OK        returns IE_stat_OK
IE_cad_set_OK        set IE_stat_OK    0=OK-locked; 1=OK-active
IE_cad_test__        test ob alle Eingaben vorh; Ausgabezeile generieren
IE_cad_upd_hdr       change header of definition-line (in IE_outTxt)
IE_inpTest           test Inputs fuer diese zeile komplett
IE_undo              Ctrl-Z pressed or Undo-Button pressed ..
IE_cad_Inp_undo      delete last CAD-input | exit function

IE_edit_dbo          edit obj (from typ/dbi)
IE_activate          Zeile auslesen, analysieren, eintragen
IE_txt2parG          test for groupobjects
IE_txt2par1          Decode Werte (Text -> Parameter)
IE_decode_Ln         Zerlegen der Inputzeile
IE_modif__           modify codeline

IE_cad_init0         neue AptIndexe holen
IE_cad_exit0
IE_cad_exitFunc      exit the active cadFunction
IE_cad_p_CB IE_cad_l_CB IE_cad_c_CB IE_cad_r_CB IE_cad_d_CB    Cad-Func. sel

-------------------- menu-functions ---------------------------
IE_cad_init__        activate mainMenu + Submenu
IE_cad_init2         activate mainMenu
IE_cad_init1         activate Submenu und inputFelder
IE_get_Func          returns mainFunctionNr (ck if a menu is active)
IE_ck_menGrpInd      Hauptmenuegruppe bestimmen
IE_inp_set_menu_col  set col of CAD-menu
IE_cad_init_men__    init menu & inputs
IE_cad_init_men_1    disactivate menu & inputs

-------------------- inputField-functions ---------------------------
IE_cad_InpIn_CR      CR aus irgendeinem anderen Win ..
IE_cad_InpIn_CB      Key in in ein Inputfeld
IE_inp_CB__          callback of inputField
IE_cad_InpIn__       activate new inputField
IE_inp_chg           change inputField
IE_inp_set           write into active CAD-inputfield
IE_cad_ClearInp1     clear one inputField
IE_cad_ResetInputs   clear all inputFields & goto inputField 0
IE_cad_ClearInputs   clear all inputFields
UNUSED IE_cad_ClearAct      clear display corresponding to active inputField
IE_get_inp__         read text of inputField
IE_get_inp_t         get type & text of inputField
IE_get_inp_typ       get type & text of inputField
IE_get_inp_TypAct    get IE_inpTypAct = IE_inpTypR[IE_inpInd];
IE_set_inp           write text -> inputField
IE_inp_mod__         modify active inputField
IE_inp_selection_clear    check / clear a active selection in inputfield
IE_inp_ck_empty      check if all inputfelds empty
IE_inp_ck_del        dialog canel unstored input
IE_inp_ck_prev_empty add 0 into prev. field if nxt field has value
IE_cad_InpIn_String  cbuf in das naechste freie inputfeld eintragen ...
IE_cad_InpIn_left    goto end of upper cad-inputField
IE_cad_Inp_edit_field das aktuelle Inputfeld mit editor
IE_get_inp_dli       get dli for active inputField
IE_GET_INP_DLI       get dli for inputField                              INLINE

IE_get_tempPos       returns position for a temporary object
IE_cad_Inp_disp__    display temp. symbols for inpField iind (zB Vec)
IE_cad_Inp_disp_pt   display point for actual inputfield
IE_cad_Inp_disp_val  display value in messageWin
IE_cad_Inp_disp_ang  display angle

-------------------- infoField-functions ---------------------------
IE_cad_ClearMenInf   clear MenuInfos (right of inputboxes)
IE_info_col_set      change color of info-field

-------------------- other-functions ---------------------------
IE_cad_reset_modif   reset to CREATE from MODIFY
IE_cad_INS           cancel-function & insert-function

IE_nxtInd            den neuen ObjIndex holen
IE_set_txtHdr        display name of new outpt-obj in field IE_entHdr
IE_parDef            check for Parameter-defaults to save

IE_cad_Inp1_Info     write infoText into Messagewindow
IE_cad_Inp1_nxtTra   provide next|previous -
IE_cad_Inp1_nxtVal   provide next|previous value (from VAR eg V20);
IE_cad_Inp1_nxtVec   provide next|previous vector (eg D20)
IE_cad_Inp1_nxtREV
IE_cad_Inp1_nxtCW
IE_cad_Inp1_nxtAng
IE_cad_Inp1_nxtpNr   incr od decr PointNr for EndpointSelections
IE_cad_Inp1_nxtMod

IE_analyz__
IE_analyz_dist

IE_cad_selM2         mittlere Maustaste.
IE_cad_selM2_CB      filename aus Liste selected
IE_get_modify

IE_ed2__             simple TextEditor
IE_ed2_mod           "Exit" "Test" "OK"

IE_auxFun1           get pointers to functionDescriptions (.ftxt)
IE_cad_get_func
IE_cad_RotS
IE_cad_Side
IE_cad_Side1

IE_oLst_ini          init ObjLst

IE_dump_rec_stru     dump struct IE_rec_stru*

List_functions_end:
=====================================================

\endcode *//*----------------------------------------

OBSOLETE:
IE_cad_test_typ      get header for CUT INT PRJ ISO REV replaced by IE_cad_upd_hdr


-------------------------------------------------------
Neuen Menuepunkt zufuegen:
 Eintrag in der cad_lst_ und in der IE_cad_ zufuegen.
 In der IE_cad_ sind die Nummern im ersten Feld zu korrigieren (muessen
 fortlaufend sein)
 Das sollte normalerweise alles sein.




-------------------------------------------------------
WORKFLOW:
-------------------------------------------------------
IE_sel_CB__                (CAD-selection-callback)
  IE_inpCkAdd
  IE_inp_chg|IE_cad_Inp_disp__

IE_inp_CB__                (callback of keyPress in inputField)
  IE_cad_InpIn__           (change inputField)
    IE_cad_Inp_disp__      (update display of field to leave)
      IE_inpDecode
        IE_inpCkTyp
        IE_inpCkAct
      IE_cad_test__
        IE_inpDecode       (decode all fields, display result)


----------------------  funcs called from graficWindow:
IE_inp_chg            (GUI_KeyCurDown,GUI_KeyTab,GUI_KeyCurUp;
                         imply IE_inp_CB__)
IE_cad_InpIn_left       (GDK_Left)

IE_cad_InpIn_CB         UI_key_return  (IE_cad_test__ | IE_cad_OK)

IE_cad_Inp_undo         (GDK_Escape)

IE_cad_OK               UI_key_return

IE_cad_selM2            KeyF5 or mouse-middleButton;    display list

IE_inp_mod__            KeyBackSpace or ascii-key;  del key or add key




-------------------------------------------------------
activate existing CodeLine:
-------------------------------------------------------
OMN_CB_popup
  IE_edit_dbo
    IE_activate
      IE_decode_Ln
      IE_txt2par1




-------------------------------------------------------
IE_sel_CB_1     (wenn neues Obj kommt):
  test ob Obj brauchbar
    JA: Obj einspeichern,
        zugehoerigen Text generieren und ins Entryfeld stellen

IE_cad_test__    (wird mit jedem Eingabefeld aufgerufen):
  Test ob alle Eingaben vorhanden;
    JA: Zeile generieren,
        OK-Button freigeben
        Zeile exekutieren


KeyIn in eine Inputfeld:        IE_inp_CB__


OK-Button gedrueckt:   IE_cad_OK
  clear Inputs (nicht Infos)
  Zeile in die Datei abspeichern
    ChangeMode: ?
    InsertMode: ?


in IE_cad_init1: den Header fuer das neue Obj generieren und irgendwo darstellen
 (wie alte "P" Funktion - Ctrl P)



------------------------------------------
Check if CAD is active:
  if(UI_InpMode != UI_MODE_CAD) ..
Die aktive Menuegruppe ist IE_FuncTyp (0=point .... 12=CUT/TRA/PRJ..)
  und auch menGrpInd
Die aktive MenueUntergruppe ist IE_FuncSubTyp (in Gruppe 12: 0=CUT, 1=TRA..)
  und auch menSubGrpInd

 IE_lst_act = (IE_rec_txt*) cad_lst_p {char* ftxt; char* ftyp;}
 zB  "CIR PT-Start PT-End PT-Cen [Z-Axis]",  "ARC",
 IE_lst_act[IE_FuncSubTyp].ftyp ist hier  "ARC"


Der erste Index auf die momentan aktiven Parameter ist IE_first
 der letze IE_last, Anzahl IE_inpAnz.

IE_cad_act = (IE_rec_stru*) IE_cad_p {int ind; int typ; char* info;}
 zB   0, Typ_PT,      "Mittelpunkt",
 IE_cad_act[IE_first].typ    ist der Typ des ersten Paramter ..
 IE_cad_act[IE_first].info   


WC_get_obj_stat()=1: preview object
                 =0: final save ("OK") requested



Der Index auf das momentan aktive inputField ist IE_inpInd (0 - (INPRECANZ-1))
Der zum aktiven inputField gehoerige Eingabetyp ist IE_inpTypR[IE_inpInd]



====================================================================
Anzeige temporärer Objekte:
- für jedes Eingabefeld kann ein temporärObjekt angezeigt werden;
  dli dazu ist fix IE_GET_INP_DLI (-ind - 2).
- IE_cad_test__>ED_work_CAD>WC_Work1 erzeugt ein temporärObjekt mit dli DLI_TMP.
Funcs:
IE_cad_Inp_disp__




====================================================================
MEMORY:

Inputzeile   IE_buf[mem_cbuf1_SIZ]
Outputzeile  IE_outTxt[mem_cbuf1_SIZ]

Man kann mem_cbuf1 wahrscheinl nicht benutzen, da nach UTF_GetLinNr
 ED_work_CurSet (Redraw all bis Curpos) gerufen wird ...



Memoryusage fuer modify Line:
IE_activate
  mit UTF_GetLinNr   eine Zeile --> IE_buf holen;
  mit IE_decode_Ln   decode Line --> aus_typ, aus_tab (memspc54/memspc54)

  Wenn es ein GruppenWindow ist:
  IE_ed1__
  GUI_Ed_Write ..



====================================================================
Global Vars in this Source: 

menGrpInd      the active menuIndex; 0=PT, 1=LN .. 13=joints 14=ConstrPlane
               test with eg: if(menGrpInd == IE_Func_Modify) ..
IE_FuncTyp     the active outptType; corresponding menGrpInd;
               eg Typ_PT  Typ_LN .. Typ_Joint; menGrpInd=14 is undefined;

menSubGrpInd   submenu-index from GUI-selection;
IE_FuncSubTyp  submenu-index; into IE_cad_act

IE_cad_act     the active menu; eg cad_lst_p for Typ_PT;
               .ind (IE_FuncSubTyp?),
               .typ, (eg Typ_goGeo1)
               .info (eg "LN/CI/Curv" or "[Version]")
IE_first       first index into active subMenu ?
IE_inpAnz      nr of records of active subMenu
IE_inpTypR     a copy of IE_inpAnz IE_cad_act.typ starting at IE_first

IE_inpInd      index of active CAD-inputfield
IE_inpTypAct   requested obj-type for the active inputField
IE_grp_typ     '('=union-group; '<'=ccv-group; '['=optional ?

IE_lst_act     IE_lst_act[menSubGrpInd].ftxt

IE_cad_typ     DB-typ of obj to create
IE_objInd      DB-ind of obj to create



IE_rec_stru    .info  see below

....
IE_FncNr       index into IE_fTbab_Modify        USED ONLY for func IE_cad_test_typ




====================================================================
IE_rec_stru.info  consists of 3 parts, delimited by '|':
  1. Block:
  - displayText info (goes -> IE_wCad_info); from start - first '|'
  2.Block:
  - preloadText (if exists: goes -> IE_wCad_obj (inputFields))
      keep it in IE_wCad_preLoad[IE_inpInd]
  3.Block:
  - inputGroupTyp; optional or group; (first char)
      keep it in IE_grp_typ
  - subTyp; info about subtypes; eg: inputType=Typ_Angle; subTyp=TiltAngle ..
      subTypText and infos in IE_inpAuxDat[];
      IE_inpTypR[IE_inpInd] is the (primary-) inputType;
      using funcs:
        IE_cad_Inp_disp_vc()
        IE_cad_Inp_disp_ang()  

Functions:
  IE_cad_init1  writes displayText
  IE_parDef     writes preloadText



====================================================================
inputGroupTyp - Types of input (from first char of .info):
[ = optional
< = Group of objects          only one Group in command possible !
( = Union  (Group with U())   more than one unions possible ..  




====================================================================
Compile:
cc -c `gtk-config --cflags` xa_ui_cad.c
cl -c /I ..\include xa_ui_cad.c




====================================================================
*/



#ifdef _MSC_VER
#include "MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>                      // memcmp memcpy
#include <stdarg.h>               // va_list
#include <ctype.h>                // f isdigit ..


#include "../ut/ut_geo.h"
#include "../ut/ut_txt.h"         // term_buf
#include "../ut/ut_TX.h"
#include "../ut/ut_txfil.h"       // UTF_GetLinNr
#include "../ut/ut_cast.h"        // INT_PTR
#include "../ut/ut_os.h"          // OS_beep
#include "../ut/ut_gtypes.h"      // AP_src_typ__
#include "../ut/ut_ox_base.h"     // OGX_SET_OBJ OGX_NUL
#include "../ut/ut_obj.h"         // UTO_

#include "../gui/gui__.h"         // Gtk3

#include "../ut/func_types.h"          // FUNC_DispWire
#include "../gr/ut_GL.h"          // GL_Redraw
#include "../gr/ut_DL.h"          // DL_Redraw

#include "../db/ut_DB.h"          // DB_QueryNxtFree

#include "../ci/NC_Main.h"        // WC_Work__
#include "../ci/NC_apt.h"         // T_CCW,       T_CW,     ..


#include "../xa/xa.h"             // APED_oid_dbo__
#include "../xa/xa_edi__.h"       // ED_Get_LineNr ED_enter
#include "../xa/xa_ui.h"
#include "../xa/xa_ui_gr.h"       // UI_GR_DrawInit UI_GR_DrawExit
#include "../xa/xa_ui_cad.h"
#include "../xa/xa_cad_ed1.h"
#include "../xa/xa_undo.h"
#include "../xa/xa_mem.h"         // memspc51, mem_cbuf1 mem_cbuf1_SIZ
                                  // IE_buf IE_outTxt
#include "../xa/xa_sele.h"        // Typ_go*
#include "../xa/xa_msg.h"         // MSG_const__
#include "../xa/xa_cad_ui.h"      // INPRECANZ inpAuxDat ..


#define   TRUE 1
#define   FALSE 0




//---------------------------------------------------------------------
// EXTERNALS:
// aus ../xa/xa.c:
extern AP_STAT   AP_stat;               // sysStat,errStat..
extern char      AP_mod_dir[128];      // das Verzeichnis fuer OPEN
extern char      AP_lang[4];            // en od de
extern char      AP_ED_oNam[128];        // objectName of active Line
extern Plane     WC_sur_act;            // die Konstruktionsebene

// aus ../xa/xa_ui.c:
extern int       UI_InpMode;
// extern int       UI_InpSM;
extern int       KeyStatCtrl;
extern MemObj    winGR;

// ex ../gr/ut_GL.c

// ../gui/gui_base.c:
extern int UI_fontsizX, UI_fontsizY;

// ex ../xa/xa_sele.c
extern long   GR_selDli;         // 


// ex ../xa/xa_ed.c
extern int       ED_lnr_act;
extern char      *ED_cpos; 




//--------------------------------------------------------------------
// local:
//--------------------------------------------------------------------
// static void *IE_wCad_men[INTPTYANZ];       // menuFields
static MemObj    IE_wCad_men[INTPTYANZ];       // menuFields

static MemObj    IE_wCad_obj[INPRECANZ];       // inputFields
static MemObj    IE_wCad_info[INPRECANZ];      // infotext for inputFields

       int       IE_inpTypR[INPRECANZ];         // inputTypes (requested type)

static char      IE_wCad_preLoad[INPRECANZ][16];// preloadText (out of .info)

       inpAuxDat IE_inpAuxDat[INPRECANZ];       // data for inputFields


static MemObj    boxCad0, boxCad1, boxCad2, boxCad3;
static MemObj    boxInp2;
static MemObj    IE_but_OK, IE_but_Canc, IE_but_Clear, IE_help_F, IE_help_I;
static MemObj    IE_entHdr;  // Entryfeld f. den APT-Header
static MemObj    IE_entNam;  // Entryfeld f. ObjName-Infotext
static MemObj    IE_txtMode; // Labelfeld f. den Mode (Add/Modify/Insert)


// static GIO_WinEd IE_ed1_wText;             // f IE_ed2__
// static GIO_Win__ IE_ed1_win = {NULL, 0};


       MemObj    IE_edWin1 = GUI_OBJ_NEW;   // window CCV-editor

static int       (*IE_ui_func) ();


// static int       IE_ed_lNr;        // die lNr der aktuellen Zeile
//                                    ersetzt durch ED_get_lnr_act /set/inc


       int       IE_modify=0;      // 0=Add 1=Modify 2=Insert
static int       IE_save=0;        // 0=preview, 1=save perm.
static int       IE_delete=0;      // 0=nein, 1=Ja.
static int       IE_EdFnc=0;
  // 0 = DefaultEditFunctions; via Inputfelder (IE_wCad_obj)
  // 1 = GroupEditor  IE_ed1__  '<'
  // 2 = TextEditor   IE_ed2__



/// active MenueGroup;  0=PT, 1=LN .. ANALYZ=14 ..
static int       IE_FuncTyp = 0;

/// active MenueSubGroup; depends of MenueGroup.
static int       IE_FuncSubTyp = 0;   //

static char      IE_ftyp_act[16];
static char      IE_modifHdr[32];     // der original-inputHeaeder bei modify
static char      IE_grp_typ;          // '('=union-group, '<'=ccv-group, else not.
                                      // is first char of .info of active field
// static void *IE_ent_act;         // the active inputField (entry)

static IE_rec_stru *IE_cad_act = NULL; // zeigt auf IE_cad_p od IE_cad_l ..
static int          IE_cad_typ = -1;   // active outputTyp; eg Typ_PT or Typ_LN
                                       // corresponds with IE_FuncTyp


       int  IE_inpTypAct;     // requested type for the active inputField
static int  IE_inpInd;        // 0-5, zeigt auf das EntryFeld, das den Focus hat
static int  IE_inpAnz;        // Anzahl erforderl. Objekte
       int  IE_inpSrc;        // type of Input; 2=normal, 3=PgUp/Dwn

// static char IE_inpStat[INPRECANZ];   // Status Input; '0'=Leer, '1'=complete.
static int  IE_inpStat = 0;   // status of the active inputField;
            // 0=active inputfield has been changed, not yet initialized
            // 1=inputfield is initialized after change.


// Eingabezeile; muss leider size=mem_cbuf1_SIZ haben !
#define IE_BUFSIZ mem_cbuf1_SIZ
// char   IE_buf[IE_BUFSIZ];  // nun ex xa_mem.h
long   IE_bufSiz;
long   IE_bufMax=IE_BUFSIZ;


// Ausgabezeile; muss Size mem_cbuf1_SIZ haben;
// static char IE_outTxt[mem_cbuf1_SIZ];  // nun ex xa_mem.h

#define IE_MEN_NR 15       // total nr of menuGroups

static int IE_stat__;  // Status Menu,inputFields;
  // 0=active, normal operation
  // 1=startPhase; activate 1 inputField
  // 2=createPhase; create Menu & empty inputFields.

static int IE_stat_OK; // Status OK-Button; 0=inaktiv, 1=active

// static char **IE_lst_act;
static IE_rec_txt *IE_lst_act;

static  int    menGrpInd, menSubGrpInd;



static long      IE_objInd=20;  // der ObjInd fuer das akt.(Ausgabe)Obj.

static int       IE_selTyp;     // der Typ des zuletzt selektierten objekts
static long      IE_selInd;     // der Ind des zuletzt selektierten objekts



//================================================================
// prototypes
  // gint IE_ButtonPress (void *widget, void *event);
  // gint IE_ButtonPress (GtkWidget *widget, GdkEventButton *event);
  char* IE_get_inp__ (int ind);
  int IE_cad_CB_Cancel (MemObj *mo, void **data);
  int IE_cad_CB_Clear (MemObj *mo, void **data);
  int IE_cad_CB_Ok (MemObj *mo, void **data);
  // ex xa_cad_ccv_ui.c
  int IE_ccv__ (MemObj *mo, void **data);
  // ex xa_cad_ed1.c
  int IE_ed1__ (MemObj *mo, void **data);



#define GROUP_IS_ACTIVE (IE_grp_typ == '<')||(IE_grp_typ == '(')

#ifdef globTag
void CAD(){}
#endif



/*
../xa/xa_sele.h            definition of selection-groups

See INF_workflow_select

// sele_decode  UNUSED  converts the selected obj into a requested obj


Typ_VC        D
              D(val,val,val),D(ANG(ang)[,ANG(ang)][,val]),D(D[,val][,REV])
              D(L[,val][,REV]),D(C[,val][,REV])
              D(Plg[,val]),D(CCV[,val])
              NOT YET: D(P,P[,val][,REV])

Typ_PT        P                           Point
              P(Typ_go_LCS)               L,C,S; A, B geht nur teilweise !

Typ_LN        direct: L                   Line
              L(Plg),L(CCV),L(P,P),L(P,D),L(P,L),L(R),L(C)

Typ_AC        C
              C(CCV)

Typ_Val       Val|V                       Value from keyIn|Variable; eg V20
              VAL(C) = Radius;

Typ_Angle     ANG(Val),ANG(VC),ANG(V)     Angle (from keyIn, Vector)

Typ_XVal      direct: -                   Distance DX/Y/Z
Typ_YVal      X(Val),X(V),X(P),X(L) (length)
Typ_ZVal

Typ_PLN       R                           Plane, RefSys;
              R(P [R|P|D|L|angle])
              R([PERP] P P|D|L)
              R(L|C)

........................................ groups:
Typ_goGeom    all but modifiers  (D P L C S A B)

Typ_goPrim    P|L|C|S(Ell,Bsp,Plg,CCV) NOT D|A|B
                                       Konturobjs ruledSurf,
                                       tang.Objs for a Circ;
                                       obj's for a ruled surf;

Typ_go_LCS    L|C|S(Ell,Bsp,Plg,CCV)   NOT P|D|A|B
                                       Konturobj RevSur; Konturobj RuledSur;

Typ_go_lf1    L|C|S(Ell,Bsp,Plg,CCV)   NOT P|D|A|B|contour
              only single trimmed curve; for: standard-points-of-curve

Typ_goGeo1    Typ_go_LCS|Pln|Sur|Sol   alle curves,  NOT P|D;

Typ_goGeo2    C|Ell                    Centerpt Circ,Elli; Tang.|Z-Axis;

Typ_go_PD     PT|VC                    gives Point or Vector

Typ_goGeo4    UNUSED   D|L|Pln            liefert ein ModelRefsys

Typ_goGeo5    C|Ell|Plg|Bsp|CCV         NOT D,P,L,A,B
                                        closed Contour

Typ_goGeo6    P|L|C                     NOT D|S|A|B
              L(Plg),L(CCV),C(CCV)      Line(also from Plg,CCV),Circ.
                                        "CIR Tang Tang ..

Typ_goGeo7    Val|V|P|D                 Direction from ang,vec or pt. RevSur.
              D(LN),D(Plg),D(CCV)
              Val|V  out as value (nicht VAL, nicht ANG)

Typ_goGeo8    Val|V|P                   Distance, Parameter (f.RevSur)|Point
              VAL(C)=Radius;            "CIR Cen Radius (Radius)

Typ_go_RA     plane|surface             eg surface for offset-curve

Typ_goGeoSUSU Sur|Sol                   Supporting Surface CON/TOR/SRU/SRV/SBS

........................................ modifiers:
Typ_mod1      1-n                       versionNumber; 2D-buttons: "NXT¦PRV"
                                        "LN LN Dist. P3

Typ_modCWCCW  "CW" ("CCW")              2D-button: "CW"
                                        "ARC Cen Ang1 Ang2 Radius

Typ_modREV    "REV"                     reverse; 2D-button: "REV"
                                        "VEC cartes  (vc from line)

Typ_modCX     "CX"                       Modifier ACROSS (Default = Along)
                                         2D-button: "CX"
                                         "ISOparametric Curve from Surf"

Typ_modUnlim  "UNL|UNL1|UNL2"


Typ_SubModel  APT_decode_model _FilNam_  "M int.."
TYP_FilNam    subModel oder filename     "M ext.."
Typ_CtlgPart  APT_decode_model _FilNam_  "M CatalogPart


......................................................................
UNUSED:
Typ_Group      alle                 wird fuer Curves genutzt; PROBLEM:
                                    man sollte alle goGeo zusätzlich mit einem
                                    Group-bit setzen können !
Typ_goAxis    UNUSED                - use line
              LN|PT+PT|PT+LN|PT+VC|PLN  RotAxis

Typ_go_LR     UNUSED                    MirrorObj (Line|Plane)
              R -> R


......................................................................
Functions with Typ_go_* see ../xa/xa_sele.h
......................................................................
Axis can be defined by -
 - Typ_VC alone - eg for "ARC PT PT Radius [Z-Axis] [Version]"
 - Typ_VC + reference to point in upper inputfield - eg "[Axis-VEC]||DD0",
 - Typ_LN - eg "A Revolved S. (Axis Contour)"
 - Typ_PLN - eg "A cylindr.Surf(Axis,Rad.)"

......................................................................
Add-on parameters in IE_rec_stru.info; eg "|1" or "||DD0"  (see ?)
  separator "|"   - definition of preLoad-text; see IE_wCad_preLoad
  separator "||"  -
    see IE_inpAuxDat[filedNr].subTyp  (set in IE_cad_init1)
   Ax#  display angle; see IE_cad_Inp_disp_ang
   DD#  display vector; see IE_cad_Inp_disp_vc
        character 3 of auxInf gives the fieldIndex of the positionPoint







*/
//=========== PUNKTE ===================
// menGrpInd = 0
#define IE_Func_Points 0


static IE_rec_txt cad_lst_p[]={
  {"PT cartes, offset",                 ""},   // 0
  {"PT polar",                          ""},   // 1
  {"PT rotate-Z",                       ""},   // 2
  {"PT rotate-axis",                    ""},   // 2
  {"PT translate",                      ""},   // 3
  {"PT endpoints center focus ..",      ""},   // 4
  {"PT control-points",                 ""},   // 5
  {"PT parametric on LN/CIR/CRV/SUR",   ""},   // 7
  {"",""}
};



static IE_rec_stru IE_cad_p[]={
  //"P-kartes",
    0, Typ_PT,      "[BasePoint]",
    0, Typ_XVal,    "[offset X-coord]",
    0, Typ_YVal,    "[offset Y-coord]",
    0, Typ_ZVal,    "[offset Z-coord]",
  //"P-polar",
    1, Typ_PT,      "BasePoint",
    1, Typ_Angle,   "Angle-Rot.||AP0",
    1, Typ_Angle,   "[Angle-Tilt]||AN0",
    1, Typ_Val,     "Dist.",
  //"PT rotate-Z"
    2, Typ_PT,      "CenterPoint",
    2, Typ_go_PD,   "Point/Vector",
    2, Typ_Angle,   "Angle-Rot.||AA013",
    2, Typ_Angle,   "[Angle-Tilt]||AN0",
    2, Typ_Val,     "[Dist.]",
  //"PT rotate-axis"
    3, Typ_PT,      "Point to rotate",
    3, Typ_LN,      "Rot-Axis",
    3, Typ_Angle,   "Angle",
  //"PT translate"
    4, Typ_PT,      "Point",
    4, Typ_VC,      "Direction-VEC||DD0",
    4, Typ_Val,     "[Dist.]",
    4, Typ_Val,     "[Dist.Normal]",
  //"PT endpoints center focus ..",      ""},   // 4
    5, Typ_go_lf1,  "LN/CI/Curv",
    5, Typ_PTS,     "Version|1",
  //"PT control-points",                 ""},   // 5
    6, Typ_go_lf1,  "Curv",
    6, Typ_PTI,     "point-Nr|1",
  //"PT parametric on LN/CIR/CRV/SUR",   ""},
    7, Typ_goGeo1,  "LN/CI/Curv/Surf",
    7, Typ_Val,     "Parameter along",
    7, Typ_Val,     "[Parameter across]",
    -1, -1,          ""
};






//=========== LINIEN ===================
// menGrpInd = 1
#define IE_Func_Lines 1


static IE_rec_txt cad_lst_l[]={
  {"LN PT  PT",                                ""},       // 0
  {"LN parallel",                              ""},       // 1
  {"LN PT Direct.[Length,Rot.Angle,TiltAngle]",""},       // 2
  {"LN LN Dist.          (parall)",            ""},       // 3
  {"LN LN LN [Rot.Angle,TiltAngle] (mid)",     ""},       // 4
  {"LN tangent to 2 objs ..",                  "TNG"},    // 5
  // {"LN CirTang Direct. [Rotate]",      ""},                  // 5
  // {"LN CirTang PT",                    ""},                  // 6
  // {"LN CirTang CirTang",               ""},                  // 7
  {"",""}
};


static IE_rec_stru IE_cad_l[]={
  // "L Pt-Anf - Pt-End",
   0, Typ_PT,      "Startpt",
   0, Typ_PT,      "Endpt",
   0, Typ_modUnlim,"[unlimited]",
  // "LN parall.",                       ""
   1, Typ_VC,      "Direction (Vec,Crv)",
   1, Typ_PT,      "[BasePoint]",
   1, Typ_Val,     "[Length]",
   1, Typ_XVal,    "[X-offset]",
   1, Typ_YVal,    "[Y-offset]",
   1, Typ_ZVal,    "[Z-offset]",
   1, Typ_modCX,   "[perpend.]",
   1, Typ_modUnlim,"[unlimited]",
  // {"LN PT Direct.[Length,Rot.Angle,TiltAngle]",      ""},    //
   2, Typ_PT,      "Point (on Line/Curve)",
   2, Typ_VC,      "Direction (Vec,Crv)||DD0",
   2, Typ_Val,     "[Length]",
   2, Typ_Angle,   "[Angle-Rot.]||AP0",
   2, Typ_Angle,   "[Angle-Tilt]||AN0",
   2, Typ_modUnlim,"[unlimited]",
  // {"LN LN Dist.          (parall)",    ""},                  // 5
   3, Typ_LN,      "parall to Line",
   3, Typ_Val,     "Dist.",
   3, Typ_mod1,    "[Position]",
  // {"LN LN LN [Rot.Angle,TiltAngle] (mid)",     ""},          // 6
   4, Typ_LN,      "Line 1",
   4, Typ_LN,      "Line 2",
   4, Typ_Angle,   "[Angle-Rot.]",
   4, Typ_Angle,   "[Angle-Tilt]||AN",
  // {"LN tangent to 2 objs ..",                  "TNG"},    // 5
   5, Typ_goGeom,  "object 1",
   5, Typ_goGeom,  "object 2",
   5, Typ_mod1,    "[solutionNr]",
   5, Typ_modREV,  "[REVers]",
   5, Typ_modUnlim,"[unlimited]",
/*
  // "LN CirTang Direct. [Rotate]",      ""},                  // 7
   5, Typ_AC,      "Circ tang",
   5, Typ_VC,      "Direct.-VEC||DD0",
   5, Typ_mod1,    "Side|1",
   5, Typ_Angle,   "[Angle-Rot.]||AP0",
  // "LN CIR-Tang  PT",                  ""},     // 8
   6, Typ_PT,      "Point outside",
   6, Typ_AC,      "Circ tang",
   6, Typ_mod1,    "[Side]|1",
  // "L Tangent.2 Circ",
   7, Typ_AC,      "Circ 1",
   7, Typ_AC,      "Circ 2",
   7, Typ_mod1,    "Version|1",      // hi, lo, le or ri
*/
  //===========================
  -1, -1,          ""};








//=========== KREISE ===================
// menGrpInd = 2
#define IE_Func_Circles 2


static IE_rec_txt cad_lst_c[]={
  "CIR Cen Radius [Z-Axis]",     "",           // 0
  "CIR Axis Point",              "",           // 1
  "CIR Cen Tang",                "",           // 2
  "CIR Tang Tang Radius",        "",           // 3
  "CIR Tang Tang Tang",          "",           // 4
  "ARC Cen Ang1 Ang2 Radius",    "ARC",        // 5
  "ARC Cen PT Angle",            "ARC",        // 6
  "ARC PT PT Radius",            "ARC",        // 7
  "ARC PT Tang Radius [Angle]",  "ARC",        // 8
  "ARC PT PT Cen",               "ARC",        // 9
  "ARC PT PT PT",                "ARC1",       // 10 
  "",""};



static IE_rec_stru IE_cad_c[]={
  // "CIR Cen Radius [Z-Axis]",     "",           // 0
   0, Typ_PT,       "Centerpoint",
   0, Typ_goGeo8,   "Radius  (radius/PT/CIR)",
   0, Typ_VC,       "[Z-Axis-Vector]||DD0",
  // "CIR Axis Point",              "",
   1, Typ_LN,       "Axis-centerline",
   1, Typ_PT,       "Point on Circle",
  // "CIR Cen Tang",                "", 
   2, Typ_PT,       "Centerpoint",
   2, Typ_goPrim,   "Tang. (Pt/Ln/Cir/Curv)",
   2, Typ_modCWCCW, "[CW/CCW]",
   2, Typ_mod1,     "[Version]",                      // 2012-09-26
  // "CIR Tang Tang Radius",        "",
   3, Typ_goGeo6,   "Obj1 (PT/LN/CIR)",
   3, Typ_goGeo6,   "Obj2 (PT/LN/CIR)",
   3, Typ_goGeo8,   "Radius",
   3, Typ_modCWCCW, "[CW/CCW]",
   3, Typ_mod1,     "[Version]",
  // "CIR Tang Tang Tang",          "",
   4, Typ_goGeo6,   "Obj1 (PT/LN/CIR)",
   4, Typ_goGeo6,   "Obj2 (PT/LN/CIR)",
   4, Typ_goGeo6,   "Obj2 (PT/LN/CIR)",
   4, Typ_mod1,     "[Version]",
   // 3, Typ_VC,      "[Z-Achs-Vektor]",
  // "CIR PT-PT-PT",                          "",
   // 3, Typ_PT,       "1.point",
   // 3, Typ_PT,       "2.point",
   // 3, Typ_PT,       "3.point",
  // "ARC Cen Ang1 Ang2 Radius",    "ARC",
   5, Typ_PT,       "Centerpoint",
   5, Typ_Angle,    "Angle-Start||AP0",
   5, Typ_Angle,    "Angle-End||AP0",
   5, Typ_goGeo8,   "Radius  (radius/PT/CIR)",
   5, Typ_modCWCCW, "[CW/CCW]",
  // "ARC Cen PT Angle Z-Axis",            "ARC",
   6, Typ_PT,       "Centerpoint",
   6, Typ_PT,       "StartPoint",
   6, Typ_Angle,    "Angle||AA013",
   6, Typ_VC,       "[Z-Axis-Vector]||DD0",
  // "ARC PT PT Radius [Z-Axis] [Version]",     "ARC",
   7, Typ_PT,       "Startpoint",
   7, Typ_PT,       "Endpoint",
   7, Typ_goGeo8,   "Radius  (radius/PT/CIR)",
   7, Typ_VC,       "[Z-Axis-Vector]",
   7, Typ_modCWCCW, "[CW/CCW]",
   7, Typ_mod1,     "[Version]",
  // "ARC PT Tang Radius [Angle]",  "ARC",
   8, Typ_PT,       "Startpoint",
   8, Typ_VC,       "StartDirection||DD0",
   8, Typ_goGeo8,   "Radius",
   8, Typ_Angle,    "[Angle]||AA--4",
   8, Typ_VC,       "[Z-Axis-Vector]",
   8, Typ_modCWCCW, "[CW/CCW]",
  // "ARC PT PT Cen [Z-Axis]",     "ARC",
   9, Typ_PT,       "Startpoint",
   9, Typ_PT,       "Endpoint",
   9, Typ_PT,       "Centerpoint",
   9, Typ_VC,       "[Z-Axis-Vector]||DD2",
   9, Typ_modCWCCW, "[CW/CCW]",
  // "ARC PT PT PT",                "ARC1",
  10, Typ_PT,       "Startpoint",
  10, Typ_PT,       "Midpoint",
  10, Typ_PT,       "Endpoint",
  10, Typ_modCWCCW, "[CW/CCW]",
  //===========================
  -1, -1,           ""};










//=========== PLANE / REFSYS ===================
// menGrpInd = 3
#define IE_Func_Planes 3


static IE_rec_txt cad_lst_r[]={
  "PLN Z-Axis [X-Axis]",            "PERP",   //0
  "PLN X-Axis [Y-Axis]",            "",       //1
  "PLN PT Plane Offset Angle",      "RSYS",   //2
  "",""};


static IE_rec_stru IE_cad_r[]={
  // "PLN [PT] Z-Axis [X-Axis]",        "PERP",
   0, Typ_PT,      "[Origin]",
   0, Typ_VC,      "Z-Axis||DD0",
   0, Typ_VC,      "[X-Axis]||DD0",
   0, Typ_goGeo8,  "[offset-Z-axis]",
  // "PLN [PT] X-Axis [Y-Axis]",        ""
   1, Typ_PT,      "[Origin]",
   1, Typ_VC,      "X-Axis||DD0",
   1, Typ_VC,      "[Y-Axis]||DD0",
   1, Typ_goGeo8,  "[offset-Z-axis]",
  // "PLN PT Plane Offset Angle",      "RSYS"
   2, Typ_PT,      "[Origin]",
   2, Typ_PLN,     "[Refsys]",
   2, Typ_XVal,    "[offset-X-axis]",
   2, Typ_YVal,    "[offset-Y-axis]",
   2, Typ_ZVal,    "[offset-Z-axis]",
   2, Typ_Angle,   "[Angle-around-Z]",
   2, Typ_Angle,   "[Angle-around-Y]",
  //===========================
  -1, -1,          ""};










//=========== VEKTOREN ===================
// menGrpInd = 4
#define IE_Func_Vectors 4


static IE_rec_txt cad_lst_d[]={
  "VEC cartes, polar",                       "",       // 0
  // "VEC polar      (Angles Length)",     "",           //  
  // "VEC PT -> PT   (Length]",            "",           //  
  "VEC from objects",                        "",       // 1 
  // "VEC CIR/ELL",    "",                               // 
  // "VEC PLN",    "",                                   // 
  // "VEC perp. (PT,VC,LN,Circ,Pln)",      "PERP",       // 
  // "VEC tangent. (Circ,Elli,Bsp,Cloth)", "TNG",        // 
  "",""};


static IE_rec_stru IE_cad_d[]={
  // "VEC cartes",                         "",         // 0
    0, Typ_VC,      "BaseVector (Line/dx,dy,dz)",
    0, Typ_Angle,   "[Angle-Rot.]||AP0",
    0, Typ_Angle,   "[Angle-Tilt]||AN0",
    0, Typ_Val,     "[Length]",
    0, Typ_modREV,  "[REVers]",
   // 0, Typ_VC,      "Line/dx,dy,dz",
   // 0, Typ_Val,     "[Length]",
   // 0, Typ_modREV,  "[REVers]",
  // "VEC polar      (Angle-Tilt Length]", "",         // 1
   // 1, Typ_Angle,   "Angle-Rot.",
   // 1, Typ_Angle,   "[Angle-Tilt]||AN",
   // 1, Typ_Val,     "[Length]",
   // 1, Typ_modREV,  "[REVers]",
  // // "D Start-Pt, Direct.-Pt [Length]"
   // 2, Typ_PT,      "StartPoint",
   // 2, Typ_PT,      "Direct.-Pt",
   // 2, Typ_Val,     "[Length]",
   // 2, Typ_modREV,  "[REVers]",
/*
  // "VEC perp. CIR/ELL",    "",                   3
   3, Typ_goGeo2,  "Circ/Ellipse",
   3, Typ_mod1,"[Z-Axis,X-Axis,..]",
   3, Typ_modREV,  "[REVers]",
  // "VEC perp. PLN",    "",                       4
   4, Typ_PLN,     "Plane",
   4, Typ_mod1,"[Z-Axis,X-Axis,..]",
   4, Typ_Val,     "[Length]",
   4, Typ_modREV,  "[REVers]",
*/
  // "VEC from objects",                   "",         // 3
   1, Typ_goGeom,  "Obj.1",
   1, Typ_PT,      "[Point (on obj)]",
   1, Typ_go_PD,   "[PT or VC]",
   1, Typ_Val,     "[Length]",
   1, Typ_modREV,  "[REVers]",
   1, Typ_FncDirX, "[parallel-across]",
   // 2, Typ_modCX,   "[across  (only Surf.)]",
   // 2, Typ_modPERP, "[perpend (only Surf.)]",
  // "VEC tangent. (Circ, Elli, Bsp)"    "",       6
   // 4, Typ_PT,      "StartPoint",
   // 4, Typ_go_LCS,    "obj-tangent",
   // 4, Typ_modREV,  "[REVers]",
  //===========================
  -1, -1,          ""};








//=========== VARIABLEN ==================
// menGrpInd = 5
#define IE_Func_Values 5


static IE_rec_txt cad_lst_v[]={
  "V Variable - Value",         "",
  "V X/Y/Z-part PT|VC",         "",
  "V PT-PT Dist.[Direction]",   "",
  "V LN   Length",              "",
  "V PT - LN Perp.Dist.",       "",
  "V CIR  Radius",              "",
  "V Angle line/vector",        "ANG",
  "V Angle 2 lines/vectors",    "ANG",
  "",""};




static IE_rec_stru IE_cad_v[]={
  // "V Variable - Value",
   0, Typ_Val,     "Value",
  // "V X/Y/Z-part PT",    "",
   1, Typ_XVal,    "[value X-coord]",
   1, Typ_YVal,    "[value Y-coord]",
   1, Typ_ZVal,    "[value Z-coord]",
  // "V Dist. PT - PT",
   2, Typ_PT,      "Point - from",
   2, Typ_PT,      "Point - to",
   2, Typ_VC,      "[Direction]",
  // "V Length  LN",             "",
   3, Typ_LN,      "Line (Length)",
  // "V Normalabstand PT - LN",
   4, Typ_PT,      "Point - from",
   4, Typ_LN,      "Line - ortho.",
  // "V Radius CI",
   5, Typ_CI,      "Circ (Radius)",
  // "V Angle line/vector",        "ANG"
   6, Typ_VC,      "Line/Vector",
   6, Typ_modPERP, "[tilt-angle]",
   6, Typ_modREV,  "[REVers]",
   6, Typ_modCX,   "[complement]",
  // "V Angle 2 lines/vectors",    "ANG",
   7, Typ_LN,      "Line/Vector 1",
   7, Typ_LN,      "[Line/Vector 2]",
   7, Typ_modREV,  "[REVers]",
   7, Typ_modCX,   "[complement]",
  -1, -1,          ""};





//=========== KURVEN ==================
// menGrpInd = 6
#define IE_Func_Curves 6


static IE_rec_txt cad_lst_s[]={
  "S Ellipse Center,AxisEndPoint,Point",     "ELL",
  "S Ellipse Center Axes [EndPoints]",       "ELL",
  "S connection-lines < Points...",          "MSH",   // 2
  // "S Ellipse < Proj. CIR/ELL -> PLN",        "ELL", 
  "S Polygon < Points...",                   "POL",   // 3
  "S Polygon < Rectangle",                   "REC",   // 4
  // "S Polygon < Proj. Polygon -> PLN",        "POL",  
  "S BSpline < Points...",                   "BSP",   // 5
  // "S BSpline < Polygon...",                  "BSP",   
  "S BSpline < convert & join obj's",        "BSP1",  // 6
  "S Clotoid",                               "CLOT",  // 7
  "S Contour(CCV) <- PT/LN/CIR/CRV",         "CCV",   // 8
  // "S Contour  <- Rectangle rounded",  "CCV",
  "",""};


static IE_rec_stru IE_cad_s[]={
  // "S Ellipse Center, AchsendPoint", "ELL",
   0, Typ_PT,      "Center",
   0, Typ_PT,      "EndPoint MajorAxis",
   0, Typ_PT,      "Point on Ell.",
  // "S Ellipse Center, Hauptachsen, EndPoint", "ELL",
   1, Typ_PT,      "Center",
   1, Typ_VC,      "MajorAxis (VEC)||DD0",
   1, Typ_VC,      "Axis (VEC)||DD0",
   1, Typ_PT,      "[StartPoint]",
   1, Typ_PT,      "[EndPoint]",
  // "S connection-lines < Points...",          "MSH",   // 9
   2, Typ_PT,      "(Points)",
   2, Typ_VC,      "[Vector/Line]",
   2, Typ_Val,     "[Distance]",
  // "S Polygon  <- Point...",          "POL",   // 4
   3, Typ_PT,      "<Points...>",
  // "S Polygon  <- Rectangle",          "CCV",
   4, Typ_PT,      "CornerPoint",
   4, Typ_VC,      "DX||DD0",
   4, Typ_VC,      "DY||DD0",
  // // "S Polygon  <- Proj. Polygon -> PLN",        "POL",   // 6
   // 5, Typ_CVPOL,   "Polygon",
   // 5, Typ_PLN,     "Plane",
  // "S BSpline < Points...",                   "BSP",   // 6
   5, Typ_PT,      "<Points...>",
   5, Typ_Val,     "[Degree]",
   5, Typ_modCTRL, "[Controlpoints] CTRL",
  // "S BSpline < convert & join obj's",        "BSP1",
   6, Typ_goGeom,  "<contour>",
   6, Typ_Val,     "[smoothFactor]",
  // "S Clotoid",                               "CLOT",  // 9
   7, Typ_PT,      "StartPoint",
   7, Typ_Angle,   "StartVector||DD0",
   7, Typ_Angle,   "Angle (Diff. Start-End)||AA--5",
   7, Typ_Val,     "StartRadius",
   7, Typ_Val,     "EndRadius",
   7, Typ_VC,      "[Z-Axis-Vector]||DD0",
  // "S Contour  <- Rectangle rounded",  "CCV",
  // "S Contour(CCV) <- PT/LN/CIR/CRV",         "CCV",   // 8
   8, Typ_goPrim,  "<Contour (PT/LN/CIR/CRV)>",
   // 6, Typ_PT,      "Cornerpoint",
   // 6, Typ_Val,     "DX-dist.",
   // 6, Typ_Val,     "DY-dist.",
   // 6, Typ_Val,     "Radius",
  //-------
  -1, -1,          ""};


/*
  // "S Polygon <- Point...",  "POL",
   3, Typ_PT,      "<StuetzPoint...>",
  // "S Polygon  <- B-Spline",      "POL",   // 1
   4, Typ_CV,      "B-Spline",
   4, Typ_Val,     "Toleranz",
  // "S B-Spline <- Polygon",       "BSP",   // 2
   6, Typ_CV,      "Polygon",
   6, Typ_Val,     "Degree",
  // "S Circ   <- 2D-Polygon",    "CCV2",  // 3
   8, Typ_CV,      "2D-Polygon",
   8, Typ_Val,     "Toleranz",
*/



//=========== FLAECHEN ==================
// menGrpInd = 7
#define IE_Func_Surfaces 7


static IE_rec_txt cad_lst_a[]={
  "A Planar Surf (trim/punch)",       "",           // F0
  "A Spheric Surf (Axis,Rad.)",       "SPH",        // F1
  "A Cylinder Surf (Axis,Rad.)",      "CYL",        // F2
  "A Revolved Surf (Axis Contour)",   "SRV",        // F3
  // "A Ruled Surf (Obj1 Obj2)",         "SRU",
  "A Extrusion Surf (Contour Vec)",   "SRU",        // F4
  "A Sweep Surf (Contour Path)",      "SWP",        // F5
  "A Loft Surf (Curves across)",      "BSP",        // F6
  "A Loft Surf (Curves along/across)","BSP",        // F7
  "A supported trimmed punched",      "FSUB",       // F8
  "A Hatch / Schraffur",              "HAT",        // F9
  "",""};




static IE_rec_stru IE_cad_a[]={
  // "A planar Surf.(trim/punch)",     "",
   0, Typ_goGeo5,   "<Contours (CIR/ELL/CCV)>",
  // "A spheric.Surf(Axis,Rad.)",      "SPH",
   1, Typ_PLN,      "Axis (LN..PLN)",
   1, Typ_goGeo8,   "Radius  (radius/PT/CIR)",
   1, Typ_goGeo7,   "[hor-u1 (angle/PT/VC)]|0",
   1, Typ_goGeo7,   "[hor-u2 (angle/PT/VC)]|360",
   1, Typ_goGeo7,   "[vert-v1 (angle/PT/VC)]|0",
   1, Typ_goGeo7,   "[vert-v1 (angle/PT/VC)]|180",
  // "A cylindr.Surf(Axis,Rad.)",      "CYL",
   2, Typ_PLN,      "Axis (LN..PLN)",
   2, Typ_goGeo8,   "Radius  (radius/PT/CIR)",
   2, Typ_goGeo7,   "[hor-u1 (angle/PT/VC)]|0",
   2, Typ_goGeo7,   "[hor-u2 (angle/PT/VC)]|360",
   2, Typ_goGeo8,   "[vert-v1 (dist/PT)]|0",
   2, Typ_goGeo8,   "[vert-v1 (dist/PT)]|1",
  // "A Revolved S. (Axis Contour)",   "SRV",
   3, Typ_LN,       "Axis (LN..PLN)",
   3, Typ_go_LCS,     "Contour (LN/CI/BSP)",
   3, Typ_goGeo7,   "[hor-u1 (angle/PT/VC)]|0",
   3, Typ_goGeo7,   "[hor-u2 (angle/PT/VC)]|360",
   3, Typ_goGeo8,   "[vert-v1 (Param/PT)]|0",
   3, Typ_goGeo8,   "[vert-v1 (Param/PT)]|1",
   3, Typ_modCWCCW, "[CW; Std=CCW]",
  // // "A Ruled Surf.(Obj1 Obj2)",    "SRU",
   // 4, Typ_goPrim,   "Obj 1 (PT/LN/CIR/CRV)",
   // 4, Typ_goPrim,   "Obj 2 (PT/LN/CIR/CRV)",
   // 4, Typ_modCWCCW, "[Direct. Obj 2; CW=verk.]",
  // "A Extrusion Surf. (Contour Vec)",  "SRU"
   4, Typ_go_LCS,     "Contour (LN/CIR/CRV)",
   4, Typ_VC,       "Vector (Line)",
  // "A Sweep Surf (Contour Path)",      "SWP",
   5, Typ_go_LCS,     "Contour (CI)",
   5, Typ_go_LCS,     "Path (LN/CI/BSP)",
  // "A Loft Surf (Curves across)",      "BSP",
   6, Typ_goPrim,  "(Curves across (BSP))",
   6, Typ_Val,     "[Degree across]",
  // "A Loft Surf (Curves along/across)","BSP",
   7, Typ_goPrim,  "(Curves along (BSP))",
   7, Typ_goPrim,  "(Curves across (BSP))",
   7, Typ_Val,     "[Degree along]",
   7, Typ_Val,     "[Degree across]",
  // "A supported trimmed punched",    "FSUB",
   8, Typ_goGeoSUSU,"Supp.Surf (SRU/SRV/SBS)",
   8, Typ_goGeo1,   "<Contours (CIR/ELL/CCV)>",
  // "A Hatch/Schraffur",              "HAT",
   9, Typ_goGeo5,   "Kontur (CCV)",
   9, Typ_Val,      "Dist.",
   9, Typ_Angle,    "Angle",
  -1, -1,           ""};






//=========== SOLIDS ==================
// menGrpInd = 8
#define IE_Func_Solids 8


static IE_rec_txt cad_lst_b[]={
  "B Conus PT/PT/R/R",             "CON",
  "B Conus Circ,height,radius",    "CON",
  "B Torus Axis/R/R",              "TOR",
  "B Torus Axis/CI",               "TOR",
  "B Sphere Center,radius",        "SPH",
  "B Prism Base Thick/Point(Cone)","PRISM",
  "B Prism Base Vec",              "PRISM",
  "",""};




static IE_rec_stru IE_cad_b[]={
  // "B Conus ..........",  "CON",
   0, Typ_PT,       "Center 1",
   0, Typ_PT,       "Center 2",
   0, Typ_Val,      "Radius 1",
   0, Typ_Val,      "Radius 2",
  // "B Conus Circ,height,radius"     "CON",
   1, Typ_CI,       "Circ 1",
   1, Typ_Val,      "Height",
   1, Typ_Val,      "Radius top",
  // "B Torus ..........",  "TOR",
   2, Typ_PT,       "Center",
   2, Typ_VC,       "Axis (VEC)||DD0",
   2, Typ_Val,      "Radius outermost",
   2, Typ_Val,      "Radius Torusring",
  // "B Torus Axis/CI",    "TOR",
   // 3, Typ_PT,       "Center",
   // 3, Typ_VC,       "Hauptachse (VEC)",
   3, Typ_LN,       "Axis (LN..PLN)",
   3, Typ_CI,       "Circ Torusring",
   // 3, Typ_mod1,"[aussen/innen]",        // in or out
  // "B Sphere Center,radius"",SPH",
   4, Typ_PT,       "Center",
   4, Typ_goGeo8, "Radius  (radius/PT/CIR)",
  // "B Prism Base Thick/Point(Cone)","PRISM",
   5, Typ_goGeo5,   "BaseContour (CIR/ELL/CCV)",
   5, Typ_goGeo8,   "Thickness/Point(Cone)",
  // "B Prism Base Vec",              "PRISM",
   6, Typ_goGeo5,   "BaseContour (CIR/ELL/CCV)",
   6, Typ_VC,       "Vector",
  -1, -1,           ""};






//=========== TEXTE / NOTES ==================
// menGrpInd = 9
#define IE_Func_Note 9

static IE_rec_txt cad_lst_n[]={
  "N DIM  hor/vert/parall.",  "DIM",     // 0
  "N DIM3 X/Y/Z/parl",        "DIM3",    // 1
  "N DIM  Angle",             "DIMA",    // 2
  "N DIM  Dmr",               "DIMD",    // 3
  "N DIM  Radius",            "DIMR",    // 4
  "N PointPos,LN",            "LDRP",    // 5               ex TAG 4
  "N Text",                   "",        // 6 Txt
  "N Text,LN",                "LDR",     // 7 LN+Txt        ex LDR
  "N Text,LN,Circle",         "LDRC",    // 8               ex TAG 3
  "N TextTag,LN",             "TAG",     // 9               ex TAG 0,2
  "N Symbol",                 "LDRS",    // 10              ex TAG 5,6,7
  "N Image Pos,Filename",     "IMG",     // 11
  "",""};




static IE_rec_stru IE_cad_n[]={
  // "N DIM  hor/vert/parall.",  "",
   0, Typ_PT,      "DimPoint 1",
   0, Typ_PT,      "DimPoint 2",
   0, Typ_PT,      "TextPosition",
   0, Typ_Angle,   "[Angle dimLine]||DD2",
   0, Typ_Txt,     "[Arrows 0-4 0-4]|-1",
   0, Typ_Txt,     "[Guides 0-1 0-1]",
   0, Typ_String,  "[Add.Text]",
  // "N DIM3 X/Y/Z/parl",        "DIM3",
   1, Typ_PT,      "DimPoint 1",
   1, Typ_PT,      "DimPoint 2",
   1, Typ_PT,      "TextPosition",
   1, Typ_EyePT,   "[X/Y/Z/def=parl]|P",
   1, Typ_String,  "[Add.Text]",
  // "N DIM  Angle",             "DIM",
   2, Typ_PT,      "DimPoint 1",
   2, Typ_VC,      "Direct. 1||DD0",
   2, Typ_PT,      "DimPoint 2",
   2, Typ_VC,      "Direct. 2||DD2",
   2, Typ_PT,      "TextPosition",
   2, Typ_Val,     "[Arrows 0-4 0-4]|-1",
   2, Typ_Txt,     "[Guides 0-1 0-1]",
   2, Typ_String,  "[Add.Text]",
  // "N DIM  Dmr",               "DIMD",
   3, Typ_CI,      "Circle",
   3, Typ_PT,      "TextPosition",
   3, Typ_Val,     "[Arrows 0-4 0-4]",
   3, Typ_String,  "[Add.Text]",
  // "N DIM  Radius",            "DIMR",
   4, Typ_CI,      "Circle",
   4, Typ_PT,      "TextPosition",
   4, Typ_Txt,     "[ArrowTyp 0-4]",
   4, Typ_String,  "[Add.Text]",
  // "N PointPos,LN",            "LDRP",    //                 ex TAG 4
   5, Typ_PT,      "TextPosition",
   5, Typ_PT,      "Point",
   // 5, Typ_Val,     "[Linetyp]",
   5, Typ_String,  "[Add.Text]",
  // "N Text Direct./Groesse",  "",
   6, Typ_PT,      "TextPosition",
   6, Typ_Val,     "[TextSize]",
   6, Typ_Angle,   "[TextAngle(Deg.)]||DD0",
   6, Typ_String,  "Text",
  // "N Text,LN",                "LDR",
   7, Typ_PT,      "Startpoint Line",
   7, Typ_PT,      "Endpoint-1",
   7, Typ_PT,      "[Endpoint-2]",
   7, Typ_Angle,   "[TextAngle(Deg.)]",
   7, Typ_Txt,     "[ArrowTyp 0-4]",
   7, Typ_String,  "[Text]",
  // "N Text,LN,Circle",         "LDRC",
   8, Typ_PT,      "TextPosition",
   8, Typ_PT,      "[StartPoint Line]",
   8, Typ_Txt,     "[Linetyp]",
   8, Typ_String,  "[Text]",
  // "N TextTag,LN",             "TAG",     //                 ex TAG 0,2
   9, Typ_PT,      "TextPosition",
   9, Typ_PT,      "[StartPoint Line]",
   9, Typ_Txt,     "[color (0-7)]",
   9, Typ_String,  "Text",
  // "N Symbol",                 "LDRS",    //                 ex TAG 5,6,7
  10, Typ_PT,      "SymbolPosition",
  10, Typ_Txt,     "[symbolTyp]|0",
  10, Typ_Txt,     "[color (0-7)]",
  10, Typ_go_PD,   "[Endpoint/Vector]",
  // "N Image Pos,Filename",     "IMG",
  11, Typ_PT,      "ImagePosition",
  11, Typ_PT,      "[StartPoint Line]",
  11, Typ_mod1,    "[Linetyp]",
  // 11, Typ_PLN,     "[Orientation]",       // 2015-07-06
  11, TYP_FilNam,  "Filename (JPG,BMP)",
  11, Typ_Val,     "[Scale]",
  -1, -1,          ""};




//=========== Models ==================
// menGrpInd = 10
#define IE_Func_Models 10


static IE_rec_txt cad_lst_m[]={
  "M internal Model",         "",
  "M external Model",         "",
  "M CatalogPart",            "CTLG",
  "",""};
  // "M MockupModel Filename Pos ",             "MOCK",


static IE_rec_stru IE_cad_m[]={
  // "M int.Model Submodel Pos [Vec/Refsys]"
   0, Typ_SubModel,"basic Submodel",
   0, Typ_PLN,     "Position,Orientation",
   0, Typ_Val,     "[Scale]|1",
  // "M ext.Model Filename Pos [Vec/Refsys]"
   1, TYP_FilNam,  "Filename",      // TYP_FilNam
   1, Typ_PLN,     "Position,Orientation",
   1, Typ_Val,     "[Scale]|1",
  // "M CatalogPart",            "CTLG",
   2, Typ_CtlgPart,"CatalogPart",      // TYP_FilNam
   2, Typ_PLN,     "Position,Orientation",
   2, Typ_Val,     "[Scale]|1",
  //
  -1, -1,          ""};
/*
  // "M MockupModel Filename Pos  "MOCK",
   2, TYP_FilNam,  "Filename",      // TYP_FilNam
   2, Typ_PT,      "Position",
   2, Typ_goGeo4,  "[Z-Axis/Refsys]",
   2, Typ_VC,      "[X-Axis]",
*/


//=========== Transformationen  ==================
// menGrpInd = 11
#define IE_Func_Transform 11


static IE_rec_txt cad_lst_t[]={
  "TRafo transl",  "",
  "TRafo rot",     "ROT",
  "",""};


static IE_rec_stru IE_cad_t[]={
  // "TRafo transl",  "",
   0, Typ_VC,      "VC    (Direct.)",
   0, Typ_Val,     "[Length]",
  // "TRafo rot",     "ROT",
   1, Typ_LN,      "Axis-Rot. (Line)",
   1, Typ_Angle,   "Angle-Rot.",
  // "CUT object",    "CUT",
  -1, -1,          ""};





//=========== Modify ==================
// menGrpInd = 12
// see IE_fTbab_Modify
#define IE_Func_Modify 12

static IE_rec_txt cad_lst__[]={
  "CUT (trim) obj",         "CUT",    // 0
  "INT (intersect) obj",    "INT",    // 1
  "TRAnslate obj (VC)",     "TRA",    // 2
  "TRAnslate obj (PLN)",    "TRA",    // 3
  "TRAnsform obj (Tra)",    "TRA",    // 4
  "REV (reverse) curve",    "REV",    // 5
  "Project obj (Prj)",      "PRJ",    // 6
  "Parallel obj (offset)",  "PARL",   // 7
  "Mirror about line",      "MIR",    // 8
  "Mirror about plane",     "MIR",    // 9
  "ISOparametric Curve from Surf", "ISO",  // 10
  "",""};




static IE_rec_stru IE_cad__[]={
  // "CUT object",  "CUT",
   0, Typ_goGeo1,    "L/C/S (obj to cut)",
   0, Typ_goPrim,    "P/L/C/S/val. (cutting obj1)",
   0, Typ_goPrim,    "[P/L/C/S/val (cutting obj2)]",
   0, Typ_mod1,      "[solutionNr]",
   0, Typ_modREV,    "[REVers]",
  // "INT (intersect) obj",  "INT",
   // 1, Typ_Group,   "Cv/Pln/Sur/Sol (obj1)",
   // 1, Typ_Group,   "Cv/Pln/Sur/Sol (obj2)",
   1, Typ_goGeo1,    "Cv/Pln/Sur/Sol (obj1)",
   1, Typ_goGeo1,    "Cv/Pln/Sur/Sol (obj2)",
   1, Typ_mod1,      "[solutionNr]",
   1, Typ_modAux,    "[Spline/Polygon] POL",
  // "TRAnslate obj (VC)",  "TRA",
   2, Typ_goGeom,    "objs to move/copy",
   2, Typ_VC,        "VC    (Translation)",
   2, Typ_Val,       "[Length]",
   2, Typ_modRepl,   "[replace REPL] duplicate",
  // "TRAnslate obj (PLN)",  "TRA",
   3, Typ_goGeom,    "obj to transform",
   3, Typ_PLN,       "Plane",
   3, Typ_modREV,    "[REVers]",
  // "TRAnsform obj (Tra)", "TRA",
   4, Typ_goGeom,    "obj to transform",
   4, Typ_Tra,       "Tra      (Transformation)",
  // "REV (reverse) curve",    "REV",    // 5
   5, Typ_goGeo1,    "L/C/S (obj to reverse)",
  // "Project obj (Prj)",      "PRJ",    // 6
   6, Typ_goPrim,    "P/L/C/S. (obj to project)",
   6, Typ_goGeo1,    "Curv/Surf (where to project)",
   6, Typ_VC,        "[Direction-VEC none=normal]",
   6, Typ_mod1,      "[SolutionNr]",
   6, Typ_modRepl,   "[replace REPL] duplicate",
  // "Parallel obj (offset)",  "PARL",   // 7
   7, Typ_goGeom,    "basic obj",      // L C S A
   7, Typ_Val,       "offset dist",
   7, Typ_go_RA,     "[basic plane/surf]",
   7, Typ_mod1,      "[nr of contour; 0=all]",
   7, Typ_modAux,    "[rounded/edges] POL",
   // 7, Typ_mod2,      "[rounded]",
  // "Mirror obj        ",   "MIR",
   8, Typ_goGeom,    "obj to mirror",
   8, Typ_LN,        "mirrorline",
  // "Mirror about plane",   "MIR",
   9, Typ_goGeom,    "obj to mirror",
   9, Typ_PLN,       "mirrorplane",
  // "ISOparametric Curve from Surf", "ISO",
  10, Typ_goGeom,    "Surface",
  10, Typ_goGeo8,    "Parameter/Point",
  10, Typ_modCX,     "[across]",
  -1, -1,            ""};



//=========== INTERACT  ==================
// menGrpInd = 13
#define IE_Func_Interact 13


static IE_rec_txt cad_lst_i_[]={
  "Interact.connect",    "",
  "Joint-export",        "",
  "Joint-import",        "IMP",
  "",""};




static IE_rec_stru IE_cad_i_[]={
  // "Interact.connect",    "",
   0, Typ_goGeom,  "connect object",
   0, Typ_String,  "command",
  // "Joint-export",        "",
   1, Typ_goGeom,  "joint-object",
  // "Joint-import",        "IMP",
   2, Typ_Model,   "Submodel",
   2, Typ_Joint,   "Joint",
   2, Typ_String,  "[Text]",
  -1, -1,          ""};



/*
//=========== ANALYZE  ==================
#define IE_Func_Analyze 14

static IE_rec_txt cad_lst_z[]={
  "Analyze Point/Posi.", "ANALYZP",
  "Analyze Obj.",        "ANALYZO",
  "Analyze Dist. PT-PT", "ANALYZD1",
  "Analyze Dist. PT-Obj","ANALYZD2",
  "Analyze Vertex",      "ANALYZV",
  "",""};




static IE_rec_stru IE_cad_z[]={
  // "Analyze Point/Posi.", "ANALYZP",
   0, Typ_PT,      "Point",
  // "Analyze Obj.",        "ANALYZO",
   1, Typ_goGeom,  "geom. object",
  // "Analyze Dist. PT-PT", "ANALYZD1",
   2, Typ_PT,      "PT 1",
   2, Typ_PT,      "PT 2",
  // "Analyze Dist. PT-Obj","ANALYZD2",
   3, Typ_PT,      "PT 1",
   3, Typ_goPrim,  "Obj 2 (LN/Circ)",
  // "Analyze Vertex",      "ANALYZV",
   4, Typ_goGeom,  "Vertex",
  -1, -1,          ""};
*/




//=========== ActiveCADEnv  ==================
#define IE_Func_CADEnv 14

static IE_rec_txt cad_lst_Ace[]={
  "ActiveConstrPlane", "",
  "Reset ConstrPlane", "",
  "",""};




static IE_rec_stru IE_cad_Ace[]={
  // "ActiveConstrPlane", "",
   0, Typ_PLN,  "Refsys",
  // "Reset ConstrPlane", "",
   1, Typ_PLN,  "Abs.Refsys|RZ",
  -1, -1,          ""};







//---------------------------------------------------------------

// table of words of menu IE_Func_Modify
// UUU = unused
// TODO: IMP = IE_Func_Interact
                        //  0      1      2      3      4   
static char *IE_fTbab_Modify[]={"CUT", "INT", "TRA", "PRJ", "MIR",
                        //  5      6      7      8      9  
                          "ISO", "REV", "PARL","UUU", "UUU",
                        //  5      6      7      8      9  
                          "IMP", ""};
                        // 10    11
/// index into IE_fTbab_Modify
static int  IE_FncNr = -1;





static int IE_first, IE_last;



// externe Prototypen:
  // gint UI_Enter (void *widget, GdkEventCrossing *event);
  // int UI_GR_RECONFIG (void *data1, void *data2);


// lokale Prototypen:
  int IE_cad_init1 (int ind);
  int IE_cad_init2 (int ind);
  int IE_nxtInd ();
  int IE_cad_test__ ();
  int IE_analyz_dist (char fnc);
  int IE_popup_planes_CB (MemObj *mo, void **data);



//================================================================
  int IE_cad_set_OK (int iStat) {
//================================================================
// set IE_stat_OK    0=OK-locked; 1=OK-active

 
  IE_stat_OK = iStat;    // 0=OK-locked; 1=OK-active

  GUI_set_enable (&IE_but_OK, IE_stat_OK);  //1=activ,0=inaktiv.

  return 0;

}


//================================================================
  int IE_cad_get_OK () {
//================================================================
// IE_cad_get_OK        returns IE_stat_OK     0=inaktiv, 1=active

  return IE_stat_OK;

}


//=====================================================================
  int  IE_txt2parG (char* buf,int typRec,
                    int aus_anz,int *ind,int *aus_typ,char txtTab[][256]) {
//=====================================================================
// test for groupobjects
// Input:
//   ind   index of first free obj in aus_typ
// Output:
//   ind   index of next free obj in aus_typ
// RetCod  the nr of objs out of aus_typ with types corresponding to typRec.


  int    i1, typ, iNr;
  long   i;
  char   *ptx, cbuf[32];


/*
  printf("IE_txt2parG  aus_anz=%d ind=%d typRec=%d\n",aus_anz,*ind,typRec);
  for(i1=*ind; i1<aus_anz; ++i1) {
    printf("  %d aus_typ=%d txtTab=%s\n",i1,aus_typ[i1], txtTab[i1]);
  }
*/


  iNr = 0;

  buf[0] = '\0';


  L_weiter:


  typ = aus_typ[*ind]; // aus EditLine
  ptx = txtTab[*ind];
    // printf("   ..nxt typ=%d txtTab=|%s|\n",typ,ptx);


  //-------------------------------------------------------
  if(typ == Typ_PT) {        // ein PT in der Editline -

    // txt kan sein "P12" oder "10 0"

    if(typRec == Typ_PT) goto L_OK_P;
    if(typRec == Typ_goPrim) goto L_OK_P;



  //-------------------------------------------------------
  } else if(typ == Typ_LN) {
    if(typRec == Typ_goPrim) goto L_OK_2;




  //-------------------------------------------------------
  } else if(typ == Typ_CI) {
   if(typRec == Typ_goPrim) goto L_OK_2;
   if(typRec == Typ_goGeo5) goto L_OK_2;




  //-------------------------------------------------------
  } else if(typ == Typ_CV) {
    if(typRec == Typ_goGeo1) goto L_OK_2;
    if(typRec == Typ_goPrim) goto L_OK_2;
    if(typRec == Typ_goGeo5) goto L_OK_2;




  //-------------------------------------------------------
  } else if(typ == Typ_SUR) {
    if(typRec == Typ_goGeo1) goto L_OK_2;




  //-------------------------------------------------------
  } else if(typ == Typ_modif) {
    if(typRec == Typ_goPrim) {
      strcpy(cbuf, "MOD(");
      strcat(cbuf, ptx);
      strcat(cbuf, ")");
      ptx = cbuf;
      goto L_OK_2;
    }




  //-------------------------------------------------------
  } else if(typ == Typ_modCWCCW) {
    if(typRec == Typ_goPrim) goto L_OK_2;






  //-------------------------------------------------------
  }

  goto Fertig;




  //__________________________________________________
  L_OK_P:
  if(*ptx == 'P') goto L_OK_2;
  if(strlen(buf) > 0) strcat(buf, " ");
  strcat(buf, "P(");
  strcat(buf, ptx);
  strcat(buf, ")");
  if(aus_anz > *ind) {
    *ind = *ind + 1;
    ++iNr;
    goto L_weiter;
  }




  //__________________________________________________
  L_OK_2:
  if(strlen(buf) > 0) strcat(buf, " ");
  strcat(buf, ptx);
  if(aus_anz > *ind) {
    *ind = *ind + 1;
    ++iNr;
    if(aus_anz > *ind) goto L_weiter;
  }




  Fertig:

    // printf("ex IE_txt2parG (aus)ind=%d iNr=%d\n",*ind,iNr);
    // printf("              |%s|\n",buf);

  return iNr;






  //__________________________________________________
  L_Err:
  printf("ex IE_txt2parG  ******* nix gefunden *********\n");
  return -1;

}



//=====================================================================
  int  IE_set_txtHdr () {
//=====================================================================
// display name of new outpt-obj in field IE_entHdr
//
// Input:
//   IE_outTxt    name of new outpt-obj                extern
// Output:
//   IE_modifHdr  copy of name of new outpt-obj        extern
//   IE_entHdr    GUI-field
//
// see also IE_cad_upd_hdr


  // printf("IE_set_txtHdr |%s|\n",IE_outTxt);


  // ins Entryfeld schreiben
  GUI_label_mod (&IE_entHdr, IE_outTxt);  // 2011-06-21

  // save name of new outpt-obj if IE_modify = 1=Modify
  if(IE_modify == 1) strcpy(IE_modifHdr, IE_outTxt);

  return 0;

}


//=====================================================================
  int IE_decode_Ln (int *typTab, char txtTab[][256], int tabSiz, char *lnIn) {
//=====================================================================
// Zerlegen der Inputzeile lnIn und subTyp IE_ftyp_act setzen.
// Es werden Anzahl, Typ und Wert(als String!) ausgegeben.
// Input:
//   IE_lst_act   (global)
// Output:
//   typTab       typ
//   txtTab       string (part of lnIn)
//   IE_ftyp_act  (global) 
// RetCod:
//   nr of records (typTab, txtTab)

// ACHTUNG: braucht IE_lst_act !
//    sollte man rausnehmen; ev via ObjCodTab (ohne ANALYZ*) !

// see also APT_obj_expr APED_dec_txt



  int   i1, ityp, objNr;
  long  iind;
  char  *p1, *p2, *p3, *w_act, *w_nxt, cBuf[256];



  // printf("IE_decode_Ln |%s| %d\n",lnIn,tabSiz);

  w_act = lnIn;
  objNr = 0;



  // das erste wort des Ausdruckes -> cBuf
  w_nxt = APT_cp_ausd (cBuf, w_act, 256);
  UTX_cp_word_2_upper(cBuf, cBuf);
    // printf("  objNr=%d cBuf=|%s|\n",objNr,cBuf);



  //=====================================================
  // get IE_ftyp_act (1. word) none = NULL.
  // das erste Wort koennte ein Subtyp sein.
  // verglichen mit allen Subtypen der akt. Gruppe
  IE_ftyp_act[0] = '\0';
  if(objNr > 0) goto L_kein_subtyp;
  if(!IE_lst_act) goto L_next_ausdr;      // no CAD-func active ..

  for(i1=0; i1<100; ++i1) {
    if(IE_lst_act[i1].ftxt[0] == '\0') break;
      // printf(" %d Subtyp|%s|\n",i1,IE_lst_act[i1].ftyp);
    if(!strcmp(cBuf, IE_lst_act[i1].ftyp)) {
        // printf(" got Subtyp|%s|\n",IE_lst_act[i1].ftyp);
      strcpy(IE_ftyp_act, IE_lst_act[i1].ftyp);
      ityp = Typ_Subtyp;
      w_act = w_nxt;
      goto L_next_ausdr;
    }
  }




  //=====================================================
  L_next_ausdr:
  // outIndex objNr testen...
  if(objNr >= tabSiz) {TX_Error("IE_decode_Ln E001"); return -1;}


  // copy next expr from w_act into cBuf
  w_nxt = APT_cp_ausd (cBuf, w_act, 256);
    // printf("L_next_expr |%s|\n",cBuf);


  // L_test_string:
    p1 = cBuf;
    if(*p1 != '\"') goto L_test_function;
    i1 = strlen(p1) - 1;
    if(p1[i1] != '\"') goto L_test_function;
    p1[i1] = '\0';
    ++p1;
    // printf(" ist string |%s|\n",p1);
    // TYP_FilNam must have directory ('/') and afterwards filetyp ('.')
    p2 = strchr(p1, '/');
    if(p2) {
      p3 = strchr(p2, '.');
      if(p3) {
        ityp = TYP_FilNam;
        goto L_weiter;
      }
    }
    ityp = Typ_String;
    goto L_weiter;





  L_test_function:
  UTX_cp_word_2_upper(cBuf, cBuf);
    // printf(" L_test_function: %d cBuf=|%s|\n",objNr,cBuf);



  // Function (zb ANG(..)) ? Funcinhalt kopieren.
  L_kein_subtyp:

  p1=strchr(cBuf, '(');
  if(p1 == NULL) goto L_test_obj;

  // unveraendert ins Feld uebernehmen; fuer zB B=PRISM S10 D(0 0 10) korr.
  if(!strncmp(cBuf,"P(", 2)) {ityp = Typ_PT; p1=cBuf; goto L_weiter;}
  if(!strncmp(cBuf,"L(", 2)) {ityp = Typ_LN; p1=cBuf; goto L_weiter;}
  if(!strncmp(cBuf,"D(", 2)) {ityp = Typ_VC; p1=cBuf; goto L_weiter;}
  if(!strncmp(cBuf,"C(", 2)) {ityp = Typ_CI; p1=cBuf; goto L_weiter;}
  if(!strncmp(cBuf,"S(", 2)) {ityp = Typ_CV; p1=cBuf; goto L_weiter;}

  // remove the function 
  if(!strncmp(cBuf,"U(", 2)) {ityp = Typ_Group; goto L_rem_func;}


  *p1 = '\0';         // FuncNam abtrennen
  ++p1;
  p2=strrchr(p1, ')'); if(p2 == NULL) goto L_err1;
  *p2 = '\0';         // del ")"


  // Funktionstyp feststellen
  // printf("  test func |%s|\n",cBuf);
       if(!strcmp(cBuf, "R"   )) ityp = Typ_PLN;
  else if(!strcmp(cBuf, "X"   )) ityp = Typ_XVal;
  else if(!strcmp(cBuf, "Y"   )) ityp = Typ_YVal;
  else if(!strcmp(cBuf, "Z"   )) ityp = Typ_ZVal;
  else if(!strcmp(cBuf, "ANG" )) ityp = Typ_Angle;
  else if(!strcmp(cBuf, "VAL" )) ityp = Typ_Val;
  else if(!strcmp(cBuf, "MOD" )) ityp = Typ_modif;
  else if(!strcmp(cBuf, "U" ))   ityp = Typ_Group;
  else  goto L_err1;

  goto L_weiter;




  L_test_obj:   // zB "P12"
    // printf(" L_test_obj:\n");
  p1 = cBuf;
  i1 = APED_dbo_oid (&ityp, &iind, p1);
  if(i1 != 0) goto L_numer;
  goto L_weiter;




  // auf rein numer testen (Typ_Val);
  L_numer:
    // printf(" L_numer:\n");
  i1 = UTX_ck_num_f (&p2, p1);
  if(i1 == 0) {
    ityp = Typ_Val;
    goto L_weiter;
  }



  // kann noch sein CW CCW CX
    // printf(" check-aux-words: |%s|\n",p1);
  if(cBuf[0] == 'C') {
    if((!strcmp(p1,"CW"))||(!strcmp(p1,"CCW"))) {
      ityp = Typ_modCWCCW;
      goto L_weiter;
    } else if (!strcmp (p1, "CTRL")) {
      ityp = Typ_modCTRL;
      goto L_weiter;
    } else if (!strcmp (p1,"CX")) {
      ityp = Typ_modCX;
      goto L_weiter;
    }


  // kann noch sein DX DY DZ DIX DIY DIZ
  } else if(cBuf[0] == 'D') {
    ityp = Typ_VC;
    if      (!strcmp (p1, "DX"))  goto L_weiter;
    else if (!strcmp (p1, "DY"))  goto L_weiter;
    else if (!strcmp (p1, "DZ"))  goto L_weiter;
    else if (!strcmp (p1, "DIX")) goto L_weiter;
    else if (!strcmp (p1, "DIY")) goto L_weiter;
    else if (!strcmp (p1, "DIZ")) goto L_weiter;
    else goto L_err1;


  } else if(cBuf[0] == 'P') {
    if      (!strcmp (p1, "PARL")) { ityp = Typ_modPARL; goto L_weiter; }
    else if (!strcmp (p1, "PERP")) { ityp = Typ_modPERP; goto L_weiter; }




  } else if(cBuf[0] == 'R') {
    ityp = Typ_PLN;
    if      (!strcmp (p1, "RX"))  goto L_weiter;
    else if (!strcmp (p1, "RY"))  goto L_weiter;
    else if (!strcmp (p1, "RZ"))  goto L_weiter;
    else if (!strcmp (p1, "REV"))  { ityp = Typ_modREV;  goto L_weiter; }
    else if (!strcmp (p1, "REPL")) { ityp = Typ_modRepl; goto L_weiter; }



  } else if(cBuf[0] == 'U') {
    if      (!strncmp (p1, "UNL", 3)) {
      // "UNL|UNL1|UNL2"
      ityp = Typ_modUnlim; goto L_weiter;
    }
    else goto L_err1;




  } else {
    goto L_err1;
  }
  goto L_weiter;  // copy out p1



  //----------------------------------------------------------------
  L_rem_func:
  // remove function, copy out ..
    // printf(" remFunc1 %d |%s|\n",ityp,cBuf);
  ++p1; 
  p2=strrchr(p1, ')');
  if(p2 == NULL) goto L_err1;
  *p2 = '\0';         // del ")"
    // printf(" remFunc2 |%s|\n",p1);
  strcpy(txtTab[objNr], p1);

  goto L_contin;



  //----------------------------------------------------------------
  L_weiter:
  // copy out unmodified
    // printf("   typ[%d] = %d txt=|%s|\n",objNr,ityp,p1);
  strcpy(txtTab[objNr], p1);


  L_contin:
  typTab[objNr] = ityp;
  w_act = w_nxt;
  ++objNr;

  if(strlen(w_act) > 0) goto L_next_ausdr;




  //----------------------------------------------------------------
  // TESTAUSGABEN:
  // printf("ex IE_decode_Ln: %d\n",objNr);
  // for(i1=0; i1<objNr; ++i1)
    // printf("  aus-rec %d %d |%s|\n",i1,typTab[i1],txtTab[i1]);
  // printf("  Subtyp=|%s|\n",IE_ftyp_act);
  //----------------------------------------------------------------



  return objNr;



  L_err1:
    printf("IE_decode_Ln: Err dec |%s|\n",cBuf);
  return -1;

}


//=====================================================================
  int IE_edit_dbo (long dli, int typ, long dbi) {
//=====================================================================
/// IE_edit_dbo          edit obj (from typ/dbi)

// dli = DL_dli__dbo (typ, dbi, -1L);


  int      irc, lLen;
  long     lNr;
  char     *cp1, oNam[32];


  // printf("IE_edit_dbo %ld %d %ld\n",dli, typ, dbi);


  if(UI_InpMode != UI_MODE_CAD)
    {TX_Print("ERROR: CAD must be active  .."); return -1;}

  // if(UI_InpSM == ON)
    // {TX_Print("ERROR: Modify/Delete already active .."); return -1;}



  //----------------------------------------------------------------
  // VAR & VC: get sourecLineNr in memory
  if((typ != Typ_VAR)&&(typ != Typ_VC)) goto L_DL;

  APED_oid_dbo__ (oNam, typ, dbi);
    // printf("IE_edit_dbo %d %d oNam=|%s|\n",typ,dbi,oNam);

  // search definition-line 
  irc = APED_search_defLn (&cp1, &lNr, &lLen, oNam, -1, 0);
    // printf(" irc=%d lNr=%d |%s|\n",irc,lNr,oNam);
  if(irc < 0) {TX_Print("IE_edit_dbo E003"); return -1;}

  goto L_exit;



  //----------------------------------------------------------------
  // get sourecLineNr via DL-record
  L_DL:
  // irc = APED_search_defLn (&cp1, &lNr, &lLen, oNam, -1, 0);
  irc = DL_Get_lNr_dli (&lNr, dli);
  if(irc < 0) {TX_Print("IE_edit_dbo E001"); return -1;}


  irc = DL_Get_lNr_dli (&lNr, dli);
  if(irc < 0) {TX_Print("IE_edit_dbo E002"); return -1;}


  //----------------------------------------------------------------
  L_exit:
  ED_set_lnr_act (lNr);

  IE_activate ();

  return 0;

}


//=====================================================================
  int  IE_activate () {
//=====================================================================
/*
Wird von UI_butSM beim deaktivieren von SM gerufen.
= nach Select Line im EditWin: die selektierten zeile aktivieren
die aktuelle Zeile auslesen, analysieren, eintragen.
*/
  int    i1, i2, irc, ilen, iFound, typAct, typIs;
  long   ipos, fSiz, l1, lNr;
  char   *cpos;



  L_start:
  // printf(" aaaaaaaaaaaaaaaaaaaaaaaa IE_activate %d\n",IE_modify);



  // reset add to group
  UI_grp__ (OFF);

  // disactivate SM
  UI_cb_search (2);


  //---------------------------------------------------------
  // feststellen, ob am EOF; JA: Add, else Insert.

  // aktuelle Zeilennummer holen
  lNr = ED_get_lnr_act ();
    // printf(" act.lNr=%ld\n",lNr);


  // disp LineNr
  UI_AP (UI_FuncSet, UID_ouf_lNr, (void*)lNr);
  

  // charpos zu lNr errechnen
  // den Zeilentext nach IE_buf holen ..
  // get a copy of line <lNr> into IE_buf
  IE_buf[0] = '\0';
  cpos = UTF_GetLinNr (IE_buf, (long*)&IE_bufSiz, lNr);
    // printf("IE_activate lNr=%ld, ln=|%s| len=%ld\n",lNr,IE_buf,IE_bufSiz);


  // cut off objName, copy -> AP_ED_oNam
  // <typC>#=<code> # <AP_ED_oNam>
  APED_onam_cut (IE_buf, AP_ED_oNam);


  fSiz = UTF_GetLen0();  // get FilSiz
  // printf(" fSiz=%d\n",fSiz);

  if(cpos) {
    ipos = UTF_offset_ (cpos);
  } else {
    ipos = fSiz;
  }


  // display alles bis zur akt. Line
  // ED_Reset ();   // ED_lnr_act=0; = rework alles!
  // Ohne Reset: zeigt durch CUT veraenderte Basisobjekte nicht mehr, wenn
  // aktuelle Line zwischen Basisobjekt und CUT-Operation liegt.
  // Abhilfe: wenn ein getrimmtes Objet weggeblendet wird, muesste automat
  // das vorherige Basisobjekt wieder sichtbar sein ...
  // GR_Init1 ();   // 2006-02-20
  WC_set_obj_stat (0);  // 0=perm
  ED_work_CurSet (lNr);


  // nach ED_work_CurSet reset erforderl.
  // InputMode; bei CCVS Viewmode (letztes Obj anders behandeln)
  WC_set_obj_stat (1);  // 1=workmode
  ED_enter();



  // if(ipos < fSiz) goto L_modify;
  if(ipos < fSiz) {
    // do MODIFY
    irc = IE_modif__ ();  
    if(irc == -2) goto L_start;
    return irc;
  }



  //---------------------------------------------------------
  // IE_modify: 0=Add 1=Modify 2=Insert
    // printf("IE_modify  ADD %d\n",IE_modify); 

  if(IE_modify != 0) {
    // MODIFY -> CREATE
    IE_cad_reset_modif ();
    // IE_set_modify (OFF);
  }

  // Box Modify/Insert ausschalten
  // gtk_widget_set_sensitive (IE_box_INS, FALSE);

  // // restore S/M/DEL (make selectable)
  // UI_AP (UI_FuncEnable, UID_ckb_search, (void*)TRUE);


  // den naechsten freien Objheader feststellen und anzeigen
  IE_nxtInd ();

  // // Hintergrund Editfenster hilite ON / OFF
  // UI_Ed_hili (OFF);

  // alle Inputfelder loeschen
  IE_cad_ClearInputs (INPRECANZ);


  // feld 1 aktivieren
  IE_inp_chg (0);

  return 0;
  // goto Fertig;

}


//================================================================
  int IE_modif__ () {
//================================================================
// IE_modif__          modify codeline
// Input:
//   ED_lnr_act, ED_cpos     line to modify               EXTERNAL
// Output:
//   retCode:   -2  line empty or comment; continue with next line
//              -1  Error
 

  static int entInd=0;

  int       irc, i1, i2, tabSiz;
  int       aus_anz, aus_ind, *aus_typ;
  long      l1, ipos;
  char      *aus_tab;  // strings a 256 chars
  char      entBuf[INPRECANZ+1][256], *p1, typChar, *cpos;




  aus_typ = (int*) memspc55;
  aus_tab = (char*) memspc54;

  i1 = sizeof(memspc55) / sizeof(int);
  i2 = sizeof(memspc54) / 256;
  tabSiz = IMIN (i1, i2);


  cpos = ED_cpos;
  ipos = UTF_offset_ (cpos);


  //---------------------------------------------------------
  // MODIFY. Inputs: IE_buf=line_to_modify; AP_ED_oNam=primary_obj
  L_modify:
    // printf("mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm \n");
    // printf("IE_modif__ Modify |%s|%s|\n", IE_buf, AP_ED_oNam);


  if(IE_modify == 0) MSG_pri_0 ("CADinit");
    // printf(" APT_obj_stat=%d\n",WC_get_obj_stat());


  // die aktuelle zeile nochmal hinmalen; aber nur mehr im temp-Mode
  ED_work_CAD (ED_get_lnr_act (), IE_buf);
  // hiliten erst ganz am Ende (IE_cad_init1 loescht hilites ..)

  // reset Display
  // WC_actPos_reset (lNr+1);


  // IE_set_modify (ON);
  IE_modify = 1;    // 0=Add 1=Modify 2=Insert

  // disactivate browser
  UI_func_stat_set__ (-APF_WIN_B_E, 0);

  // Ganze Box Insert/Modify einschalten
  // gtk_widget_set_sensitive (IE_box_INS, TRUE);
  GUI_set_enable (&IE_but_Canc, TRUE);
  GUI_set_enable (&IE_but_Clear,TRUE);

  // Hintergrund Editfenster hilite ON / OFF
  // UI_Ed_hili (ON);

  // GUI_mod_styl (IE_txtMode, 1);   // make cancel-button red

  // display in red: "MODIFY"
  // GUI_label_htm_mod (IE_txtMode, "MODIFY  ");
  GUI_label_htm_mod (&IE_txtMode,
          "<span fgcolor=\"#ff0000\" weight=\"bold\">MODIFY  </span>");


  // blank lines skippen
  if(strlen(IE_buf) < 1) {
    cpos += term_anz;
    ipos = UTF_offset_ (cpos);
    // UI_Ed_sel (ipos, ipos);   // nur die CurPos setzen ..
    // goto L_start;
      // printf(" skipln1\n");
    ED_inc_lnr ();            // goto next Line
    // goto L_start;
    return -2;
  }


  // skip Kommentarzeilen
  if((IE_buf[0]=='#')||(IE_buf[0]=='_')) {
    // cpos += IE_bufSiz + term_anz;
    // ipos = UTF_offset_ (cpos);
    // UI_Ed_sel (ipos, ipos);   // nur die CurPos setzen ..
    // goto L_start;
      // printf(" skipln2\n");
    ED_inc_lnr ();            // goto next Line
    // goto L_start;
    return -2;
  }


/*
  if((IE_buf[0]=='A')||(IE_buf[0]=='a')) {
    TX_Error(" Analze A noch nicht implementiert");
    goto L_error;

  } else if((IE_buf[0]=='B')||(IE_buf[0]=='b')) {
    TX_Error(" Analze B noch nicht implementiert");
    goto L_error;
  }
*/



/*
  // add Fortsetzungszeile
  if(IE_buf[IE_bufSiz-1] == '\\') {
    // remove backslash
    UTX_endDelChar(IE_buf, '\\');
    printf("Fortsetzungszeile holen ..\n");
    p1 = UTF_GetLinNr (IE_outTxt, &i1, lNr+1);
    strcat(IE_buf, &IE_outTxt[1]); // remove strting '&'
    IE_bufSiz += i1;
  }
*/


/*
  // den text selektiert darstellen
  ipos = UTF_offset_ (cpos);
  UI_Ed_sel (ipos, ipos+IE_bufSiz);
*/




/*
  // Leerzeile: ADD.
  if(IE_bufSiz < 1) {
    IE_set_modify (OFF);

    // wenn GenerateNewLine: den neuen ObjIndex holen.
    IE_nxtInd ();

    goto Fertig;
  }


  // select the line (setzt aber den Cursor auf die Zeile vorher !!)
  UI_AP (UI_FuncEnable, UID_Edit_Line, NULL);


  // Zeile mit Inhalt; MODIFY.
  IE_set_modify (ON);


  // ist das eine Kommentarzeile - 1.Char # ??
  cpos = UTX_pos_1n(IE_buf);
  if(*cpos == '#') return FALSE;
  if(*cpos == '_') return FALSE;
*/


  // ist eine Definitionline ? das "=" suchen
  // ev hier APED_ck_defLn
  cpos = strchr(IE_buf, '=');
  if(cpos == NULL) {
    TX_Print("no CAD-Definition-line ..");
    // IE_outTxt[0] = '\0';
    // check for ActiveCADEnv
    if((IE_buf[0] == 'R')&&(isdigit(IE_buf[1]))) {
      menGrpInd = 15;
      // IE_FuncTyp = 15; darf erst von IE_cad_init2 gesetzt werden !
      menSubGrpInd = 0;
      IE_FuncSubTyp = 0;
      IE_cad_init2 (menGrpInd);
      strcpy(entBuf[0], IE_buf);
      entInd = 1;
      goto L_GrpFound;
    }
    // unknown; modify with TextEditor
    goto L_err_TextEd;
    // UI_AP (UI_FuncSet, UID_ckb_search, (void*)TRUE);
    // return FALSE;
  }
  ++cpos; // skip '='


  // Typ und Index des Zielobjektes bestimmen
  APED_dbo_oid (&IE_cad_typ, &IE_objInd, IE_buf);
    // printf("  IE_cad_typ=%d, IE_objInd=%ld\n",IE_cad_typ, IE_objInd);


  // create name of new outpt-obj
  APED_oid_dbo__ (IE_outTxt, IE_cad_typ, IE_objInd);
    // printf(" _modif-outTxt1=|%s|\n",IE_outTxt);


  // display name of new outpt-obj in field IE_entHdr
  IE_set_txtHdr ();
    // printf(" _modif-outTxt2=|%s|\n",IE_outTxt);



  //----------------------------------------------------------------
  // get menu-goup-index menGrpInd; -1=error
  i1 = IE_ck_menGrpInd (cpos);
  if(i1 < 0) return i1;
  // if(i1 < 0) return FALSE;
    // printf(" menGrpInd=%d\n",menGrpInd);

  // activate menuGroup
  IE_cad_init2 (menGrpInd);



  //====================================================================
  // Input Zeile decodieren, IE_ftyp_act (first word; zB "LDR") bestimmen.

/* 
  // skip enclosing type-func (if identical with active typ IE_cad_typ)
  i1 = IE_decode_ftyp (IE_cad_typ, cpos);
  if(i1) {
    cpos += 2;
    p1 = strrchr (cpos, ')');
    *p1 = '\0';
  }
*/

  // aus_anz = APT_decode_ausdr (aus_typ, aus_tab, &cpos);
  // decodes to type and text
  aus_anz = IE_decode_Ln (aus_typ, (void*)aus_tab, tabSiz, cpos);
  if(aus_anz < 0) goto L_err_TextEd;


    // TESTBLOCK
    // printf("decode - cpos=|%s|\n",cpos);
    // printf("decode - ftyp=|%s| aus_anz=%d\n",IE_ftyp_act,aus_anz);
    // p1 = aus_tab;
    // for(i1=0; i1<aus_anz; ++i1) {
      // printf(" decode_Ln %d typ=%d |%s|\n",i1,aus_typ[i1],p1);
      // p1 += 256;
    // }
    // IE_dump_rec_stru (IE_cad_act, " _modif-cad_act");
    // END TESTBLOCK





  //====================================================================
  // find SubgroupIndex & menSubGrpInd; fill entBuf.
  i1 = 0;


  // test next record; i1 is index.
  L_NewRec:
    menSubGrpInd = IE_cad_act[i1].ind;
      // printf("============ L_NewRec: i1=%d menSubGrpInd=%d\n",i1,menSubGrpInd);
    IE_first = i1;
    aus_ind=0;
    entInd=0;
    entBuf[entInd][0] = '\0';



    // Testen ob Subtyp IE_ftyp_act stimmt
      // printf(" ftyp=|%s|%s|\n",IE_ftyp_act,IE_lst_act[menSubGrpInd].ftyp);
    if(strcmp(IE_ftyp_act,IE_lst_act[menSubGrpInd].ftyp)) goto L_SkipGrp;
      // printf(" test OK IE_ftyp_act |%s| menSubGrpInd=%d\n",
             // IE_ftyp_act,menSubGrpInd);
      // OK, menSubGrpInd found

  // test all inputFiledTypes; index is entInd.
  L_NxtSubRec:
    typChar = IE_cad_act[i1].info[0];  // first char of info = groupTyp
      // printf("------L_NxtSubRec i1=%d typChar=%c\n",i1,typChar);

    if(IE_cad_act[i1].ind < 0) goto L_err_TextEd; // -1=EndOfData; nix gfundn

    // neue Subgroup ?
    if(IE_cad_act[i1].ind != menSubGrpInd) goto L_NewRec;


      // printf(" getSubGrp-i1=%d ind=%d typ=%d entInd=%d typChar=%c\n",i1,
             // IE_cad_act[i1].ind,IE_cad_act[i1].typ,entInd,typChar);


    // test group
    if(typChar == '<') {           // "< steht fuer 1-n objekte (objGroup)
      // irc = IE_txt2parG(entBuf[entInd], IE_cad_act[i1].typ,
                        // aus_anz, &aus_ind, aus_typ, aus_tab);
      irc = IE_txt2parG (IE_buf, IE_cad_act[i1].typ,
                         aus_anz, &aus_ind, aus_typ, (void*)aus_tab);
        // printf(" after IE_txt2parG irc=%d\n",irc);
      if(irc > 0) aus_ind += irc;                 // 2019-04-16

      IE_bufSiz = strlen(IE_buf);



    } else if(typChar == '(') {    // union-group
        // printf(" test union-group ind=%d typ=%d\n",aus_ind,aus_typ[aus_ind]);
      if(aus_typ[aus_ind] == Typ_Group) {
        p1 = (char*)aus_tab;
        p1 += aus_ind * 256;
        strcpy(entBuf[entInd], p1);
        irc = 0;  // found ..
        ++aus_ind;
      }


    // test parameter, zuordnen, nach entBuf kopieren ..
    } else {
      irc = IE_txt2par1 (entBuf[entInd], IE_cad_act[i1].typ,
                         aus_anz, &aus_ind, aus_typ, (void*)aus_tab);
    }
      printf("  _txt2par1 %d %d |%s|\n",irc,aus_ind,entBuf[entInd]);


    // wenn Parameter mandatory und notFound: goto NextGroup.
    if(irc < 0) { // nix gefunden ..
      if(typChar != '[') goto L_SkipGrp;  // skip optional
    }


    if(entInd < (INPRECANZ - 1)) {
      ++entInd;
      entBuf[entInd][0] = '\0';
    }
    ++i1;
      // printf(" next entInd=%d SubRec=%d\n",entInd,i1);


    // gibts noch parameter im text, die nicht zugeordnet sind ?
    if(aus_ind < aus_anz) goto L_NxtSubRec;







    L_test_RestOfGroup:
    // ist subgroup zu ende oder fehlt noch was
    // printf("..subgrp[%d]=%d %d\n",i1,IE_cad_act[i1].ind,IE_cad_act[i1+1].ind);

    // in i1 nun ein neues Obj; wenn neue SubGrp: Rec. found!
    if(IE_cad_act[i1-1].ind != IE_cad_act[i1].ind) goto L_GrpFound;

    // ist dieses Obj. mandatory: ignore it, check next.
    if(IE_cad_act[i1].info[0] == '[') { ++i1; goto L_test_RestOfGroup; }

    // bei GroupFeld : wenn schon einige Inputs vorhanden: skippen
    if(IE_cad_act[i1].info[0] == '<') {
      if(i1 > 0) {     // nicht fuer erstes Inputfeld
        ++i1;
        goto L_test_RestOfGroup;
      }
    }


    // es gab nicht befriedigtes Eingabefeld; weitersuchen.
    // nicht in dieser Group; skip it.
    L_SkipGrp:
    ++i1;
    if(IE_cad_act[i1].ind < 0) goto L_err_TextEd;
    if(IE_cad_act[i1].ind == menSubGrpInd) goto L_SkipGrp;
    goto L_NewRec;




  //----------------------------------------------------------------
  // es gibt kein CAD-Menu zur activeLine; mit TextEdit anzeigen.
  L_err_TextEd:
  // OS_beep (); // Error; not found
  // TX_Error("cannot analyze line -> modify manually");
  TX_Print("cannot analyze line -> modify manually");
    // ask LineNr
    l1 = ED_get_lnr_act ();

    // modify zeile im mem mit dem SystemEditor.
    // AP_src_mod_ed (l1);

    // Start TextEditor; display active line ..
    // IE_ed2__ (NULL, (void*)UI_FuncInit);
    IE_ed2__ (NULL, GUI_SETDAT_EL(TYP_EventPress,UI_FuncInit));
    // IE_modify = 1;    // reset CAD-editmode   2013-09-03

    goto Fertig;





  //==========================================================================
  // CAD-Func found; activate Edit (DefaultEdit or GroupEditor)
  L_GrpFound:
    // printf(" L_GrpFound: IE_cad_typ=%d\n",IE_cad_typ);

    // printf(" found grp=%d subgrp=%d entInd=%d\n",menGrpInd,menSubGrpInd,
           // entInd);
    // printf("  ftxt=|%s|\n",IE_lst_act[menSubGrpInd].ftxt);


  // Display groupMenu (eg PT or LN ..)
  // see IE_cad_init2 (menGrpInd);
  GUI_optmen_set (&IE_wCad_men[menGrpInd],
                  1,
                  IE_lst_act[menSubGrpInd].ftxt);

  // activate "Help Func" and ""Help Input"
  GUI_set_enable (&IE_help_F, TRUE);
  GUI_set_enable (&IE_help_I, TRUE);


  // Init das gefundene SubMenu (und alle Inputfelder loeschen);
  // if necessary create GroupEditorWindow
  IE_cad_init1 (menSubGrpInd);
    // hier wird IE_inp_CB__ gerufen; noch ohne den richtigen Inhalt !


  // den Header ausgeben
  IE_nxtInd ();


  // activate Edit ...
  IE_EdFnc = 0;



    // TESTBLOCK
    // for(i1=0; i1<=entInd; ++i1) printf(" ent[%d]=|%s|\n",i1,entBuf[i1]);
    // END TESTBLOCK


  //----------------------------------------------------------------
  // copy strings entBuf into inputfields
  for(i1=0; i1<=entInd; ++i1) {
    typChar = IE_cad_act[i1 + IE_first].info[0];

      // TESTBLOCK
      // printf(" preset %d |%s| typChar=|%c| Act=%d Is=%d |%s|\n",i1,
        // entBuf[i2],typChar,typAct,typIs,entBuf[i2]);
      // END TESTBLOCK

    // '<' = group
    if(typChar == '<') continue;
    // copy into inputfield i1
    IE_set_inp (i1, entBuf[i1]);
  }



  //----------------------------------------------------------------
  // display objName AP_ED_oNam in IE_entNam
  GUI_entry_set (&IE_entNam, AP_ED_oNam);



  // start with filled inputFields
  IE_stat__ = 1;   // 1=startPhase; activate 1 inputField
  IE_inp_CB__ (&IE_wCad_obj[0], NULL);
  IE_stat__ = 0;    // 0=active, normal operation.


  // // hilite primaeres Objekt (jetzt erst wegen IE_cad_init1)
  // DL_hili_on (-2L); // das zuletzt bearb. Elem. hiliten
  // DL_Redraw ();



  //---------------------------------------------
  Fertig:

  // // disp LineNr
  // l1 = ED_get_lnr_act ();
  // UI_AP (UI_FuncSet, UID_ouf_lNr, (void*)l1);

    // printf(" ex IE_activate 0\n");

  return 0;



  //---------------------------------------------
  L_error:
  // // wieder in S/M
  // UI_AP (UI_FuncSet, UID_ckb_search, (void*)TRUE);
  return 0;

}


//================================================================
  int IE_decode_ftyp (int typ, char *srctxt) {
//================================================================
/// \code
/// test if eg srctxt = "P(" and typ == Typ_PT ..
/// Retcod 0: typ != geom.func in srctxt
///        1: typ = srctxt-func
/// \endcode

  char  s1[] = "P(";


  // printf("IE_decode_ftyp %d |%s|\n",typ,srctxt);


  // make char <- typ
  s1[0] = AP_typChar_typ (typ);
    printf(" s1=|%s| typ=%d src=|%s| IE_decode_ftyp\n",s1,typ,srctxt);

  if(!strncmp(srctxt, s1, 2)) return 1;


  return 0;

}
 

//=====================================================================
  int IE_get_modify () {
//=====================================================================
// 0=Add 1=Modify 2=Insert

   return IE_modify;

}


//================================================================
   int IE_inp_mod__ (int mode, void *data) {
//================================================================
// IE_inp_mod__         modify active inputField
// mode=0      add iKey to active inputfield
// mode=1      del last char of active inputfield    


  int        i1;
  char       *sNew = mem_cbuf1, *sAct;

  // printf("IE_inp_mod__ %d %p\n",mode,data);


  // get content if inputField <IE_inpInd>
  sAct = IE_get_inp__ (IE_inpInd);


  //----------------------------------------------------------------
  // mode=0      add iKey to active inputfield
  if(mode != 0) goto L_01;

  i1 = *(int*)data;
    // printf(" IE_inp_mod__ ikey=%d\n",i1);
  sprintf(sNew, "%s%c", sAct, i1);
  goto L_exit;



  //----------------------------------------------------------------
  L_01:
  // mode=1      del last char of active inputfield
  i1 = strlen(sAct) - 1;
  if(i1 < 0) return 0;

  UTX_cp_left (sNew, sAct, i1);


  //----------------------------------------------------------------
  L_exit:

  // replace inputField
  IE_set_inp (IE_inpInd, sNew);

  return 0;

}




//================================================================
  int IE_inp_set (char *cBuf) {
//================================================================
// IE_inp_set             write into active CAD-inputfield
// write cBuf -> inputField[ind]


  // printf("SSSSSSSSSSSSSS IE_inp_set %d |%s|\n",ind,cBuf);

  if(IE_inpInd >= 0)
  GUI_entry_set (&IE_wCad_obj[IE_inpInd], cBuf);

  // GUI_update__ ();  // ?

  return 0;

}




//================================================================
  int IE_set_inp (int ind, char *cBuf) {
//================================================================
// IE_set_inp                     write text -> inputField
// write cBuf -> inputField[ind]


  // printf("SSSSSSSSSSSSSS IE_set_inp %d |%s|\n",ind,cBuf);

  if(ind >= 0)
  GUI_entry_set (&IE_wCad_obj[ind], cBuf);

  // GUI_update__ ();  // ?

  return 0;

}


//================================================================
   int IE_get_Func () {
//================================================================
// IE_get_Func          returns mainFunctionNr
// -1      no mainFunction is active

// subFunctionNumber is IE_FuncSubTyp

    // printf("ex IE_get_Func %d\n",IE_cad_typ);

  return IE_cad_typ;

}


//================================================================
  int IE_get_inp_TypAct () {
//================================================================
// IE_get_inp_TypAct           get IE_inpTypAct = IE_inpTypR[IE_inpInd];

  return IE_inpTypAct;   // = IE_inpTypR[IE_inpInd];

}

 
//================================================================
   int IE_get_inp_t (int *typ, char **txt, int ii) {
//================================================================
// IE_get_inp_t         get type & text of inputField
// Input:
//   ii       -1 = previous inputfield (IE_inpInd - )
// Output:
//   typ      0 if requested inputField does not exist


  // printf("IE_get_inp_t %d\n",ii);


  ii = IE_inpInd + ii;

  if(ii < 0) {
    *typ = 0;
    return -1;
  }

  return IE_get_inp_typ (typ, txt, ii);

}


//=====================================================================
  int IE_get_inp_typ (int *typ, char **txt, int ii) {
//=====================================================================
// IE_get_inp_typ       get type & text of inputField


  *typ = IE_inpTypR[ii];
  *txt = IE_get_inp__ (ii);


  // printf("ex IE_get_inp_typ %d |%s| %d\n",*typ,*txt,ii);


  return 0;

}


//=====================================================================
  char* IE_get_inp__ (int ind) {
//=====================================================================
/// den Text der CAD-Panel-Entries auslesen

  static char c0 = '\0';
  char *gtxt, *p1;

  // printf("IE_get_inp__ %d\n",ind);

  if(ind < 0) return &c0;


  gtxt = GUI_entry_get (&IE_wCad_obj[ind]);
    // printf(" gtxt=|%s| %d\n",gtxt,strlen(gtxt));

  // skip lead. blanks;  NOT FOR Strings !
  // p1 = UTX_pos_1n(gtxt);
  p1 = gtxt;

    // printf("ex IE_get_inp__ %d |%s|\n",ind,p1);

  return p1;

}


/* unused
//=====================================================================
  int IE_del_start (void *parent, void *data) {
//=====================================================================
// disact. Entries, Text -> DELETE
// act. ADD-Butt; TX_Pri "select obj to delete"



  if (GTK_TOGGLE_BUTTON (parent)->active) goto L_activate;

  // disactivate
  IE_delete = 0;

  // Menus u Entries disactiv.
  gtk_widget_set_sensitive (IE_box_ME, TRUE);

  // Modus Add: Ins/Add aus
  if(IE_modify == 0) gtk_widget_set_sensitive (IE_box_INS, FALSE);



  return 0;





  //===========================================================
  L_activate:


  IE_delete = 1;
  // gtk_label_set_text ((GtkLabel*) (IE_txtMode), "DELETE");


  // Menus u Entries disactiv.
  gtk_widget_set_sensitive (IE_box_ME, FALSE);

  // Ins/Add ein
  gtk_widget_set_sensitive (IE_box_INS, TRUE);


  TX_Print("select obj to clear -");


  return 0;

}
*/


//=====================================================================
  int IE_inpTest (int typReq, char *txBuf) {
//=====================================================================
// RC =  0: Inputs fuer diese zeile komplett.
// RC = -1: es fehlen noch Inputs


  int       tLen;
  int       aus_typ[10], aus_anz;
  char      aus_tab[10][256];  // 10 strings a 256 chars




  printf("IE_inpTest typ=%d |%s|\n",typReq,txBuf);


  tLen = strlen(txBuf);
  if(tLen < 1) goto L_not_compl;


  aus_anz = IE_decode_Ln (aus_typ, aus_tab, 10, txBuf);
  if(aus_anz < 0) goto L_not_compl;



  //-----------------------------------------------------
  if(typReq == Typ_PT) {
    if((aus_anz == 1)&&(aus_typ[0]==Typ_PT)) goto L_OK;



  //-----------------------------------------------------
  } else if(typReq == Typ_VC) {
    if(aus_anz == 1) {
      if(aus_typ[0]==Typ_VC) goto L_OK;
      if(aus_typ[0]==Typ_LN) goto L_OK;

    } else if(aus_anz == 2) {
      if((aus_typ[0]==Typ_PT)&&(aus_typ[1]==Typ_PT)) goto L_OK;
    }


  //-----------------------------------------------------
  } else if(typReq == Typ_PLN) {
    if(aus_anz == 1) {
      if(aus_typ[0]==Typ_VC) goto L_OK;
      if(aus_typ[0]==Typ_LN) goto L_OK;

    } else if(aus_anz == 2) {
      if((aus_typ[0]==Typ_PT)&&(aus_typ[1]==Typ_PT)) goto L_OK;
    }


/*
  if((typ == Typ_VC)       ||
     (typ == Typ_goAxis))      {
    // wenn 1.Obj == PT, muss mind ein 2. Obj definiert werden ..
    if(cbuf[0] == 'P') {
      // Anzahl ausdruecke muss mind 2 sein ...
      wNr = UTX_wordnr(cbuf);
      if(wNr < 2) irc = 1;
    }
  }
*/


  //-----------------------------------------------------
  }


  L_not_compl:
  return -1;



  L_OK:
  return 0;



}

/*
//=====================================================================
  void IE_cad_selM2_CB (void *widget, gpointer data) {
//=====================================================================
// filename aus Liste selected


  printf("IE_cad_selM2_CB1 %d\n",INT_PTR(data));

  IE_cad_selM2 (INT_PTR(data));

  IE_cad_test__ ();

  return;

}
*/

//=====================================================================
  int IE_cad_selM2_CB (char *fnam, char *dirNam) {
//=====================================================================
// filename aus Liste selected

  int  actTyp;
  char cbuf[256];


  if(fnam == NULL) return 0;

  printf("IE_cad_selM2_CB |%s%s|\n",dirNam,fnam);

  actTyp = IE_inpTypR[IE_inpInd];


  //---------------------------------------------------
  if(actTyp == TYP_FilNam) {  // 238
    strcpy(AP_mod_dir, dirNam);

    // get symbol from path
    if(Mod_sym_get2 (cbuf, dirNam) < 0) return 0;


    // add text to Inputfield
    // strcat(cbuf, fnam_del_s);
    strcat(cbuf, "/");  // auch bei Windows "/"
    strcat(cbuf, fnam);



  //---------------------------------------------------------------
  } else if(actTyp == Typ_CtlgPart) {  // 239
    sprintf(cbuf, "%s/%s",dirNam,fnam);



  //---------------------------------------------------------------
  } else if(actTyp == Typ_SubModel) {  // 97
    strcpy(cbuf, fnam);

  }

  // IE_sel_CB_1 (TYP_FilNam, 0, "xxx");
  // gtk_entry_set_text ((GtkEntry*) (IE_wCad_obj[IE_inpInd]), cbuf);
  IE_set_inp (IE_inpInd, cbuf);


  // proceed to next Inputfield
  IE_inp_chg (-1);


  // // testen
  // IE_cad_test__ ();

  return 0;

}


//=====================================================================
  int IE_cad_selM2 (int mode) {
//=====================================================================
// display list - modelfiles|catalogParts|subModels
// Input:
//   IE_inpTypAct   TYP_FilNam
//                  Typ_CtlgPart
//                  Typ_SubModel
// Mousebuttton-middle or FunctionKey F5
// mode   unused
// 


  int   i1;
  char  cbuf1[256], fNam[256], dNam[256];  //cbuf2[256];

  static char  **ftab;


  printf("IE_cad_selM2 IE_inpTypAct=%d\n",IE_inpTypAct);
  // printf("  FeldNr=%d FeldTyp=%d\n",IE_inpInd,IE_inpTypR[IE_inpInd]);


  //============ -1 = init ==========================================
  // welcher feldtyp ist aktiv ?
  // printf(" feldtyp=%d\n",IE_inpTypAct);


  //---------------------------------------------------------------
  if(IE_inpTypAct == TYP_FilNam) {  // 238

    strcpy(cbuf1, MSG_const__(MSG_open));  // "Model open"
    // i1 = AP_Mod_open (1, fNam, dNam, cbuf1, "*.gcad");
    // 2015-07-06 filetyp removed (N=IMG needs jpg,bmp..)
    i1 = AP_Mod_open (1, fNam, dNam, cbuf1, "*.*");
    UI_GR_ButtonM1Release ();   // else KeyM1=ON ! 2013-05-01
    if(i1 < 0) return -1;
    IE_cad_selM2_CB (fNam, dNam);
    // AP_open__ (cbuf1, 4);


  //---------------------------------------------------------------
  } else if(IE_inpTypAct == Typ_CtlgPart) {          // 239
    // display catalog-list and part-list
    CTLG_Sel__ ();




  //---------------------------------------------------------------
  } else if(IE_inpTypAct == Typ_SubModel) {  // 97

    // Mod_mkList (1); // make list of all Submodels -> <baseDir>/tmp/Mod.lst
    sprintf(cbuf1,"%sMod.lst",OS_get_tmp_dir());
    i1 = GUI_list1_dlg_w (fNam, 256,
                         NULL, "select Submodel", cbuf1,
                         "1", NULL, "60,40");
    UI_GR_ButtonM1Release ();   // else KeyM1=ON ! 2013-05-01
    if(i1 < 0) return -1;
    IE_cad_selM2_CB (fNam, NULL);



  //---------------------------------------------------------------
  } else {

    UI_GR_selMen_init (2);

  }



  return 0;

}


//=====================================================================
  int IE_sel_CB__ (int typSel, long ind, char *buf) {
//=====================================================================

  IE_inpSrc = 1;   // selection in gr-win

  return IE_sel_CB_1 (typSel, ind, buf);

}


//=====================================================================
  int IE_sel_CB_1 (int typSel, long ind, char *buf) {
//=====================================================================
// Vom GR-Fenster wird Selektion eines Objekts gemeldet ..
// Mode manuell: ED_add_Text
// Hier wird festgestellt, ob das selektierte Objekt ins aktive Entryfeld
//   eingetragen wird.

// Hier wird der entryfeldText generiert.
// Ausgabetext generiert wird in IE_cad_test__.
// Inputcommands werden decodiert in IE_txt2par1 IE_decode_Ln ?

// Input:
//   typSel     ist der Typ des sel. Objekts ex DL;
//              Curves kommen als Typ_CVTRM oder Typ_CV (kein CV-subTyp !)
//   ind        DB-index
// Output:
//   IE_outTxt  object as string


/* Interne Hilfsfunktionen:

L_wr_obj:       "obj"   (buf)
GotSubTyp:    "subtypSel(obj MOD(iSeg))"
GotSubSubTyp: "typReq(obj MOD(iSeg) MOD(iSubSeg))"
GotSubPoint:  "P(obj MOD(iSeg) lpar)"
GotSubSeg:    "typReq(obj MOD(iSeg))"

TestSubLine   get ln of CCV/ln or CCV/plga -> GotSubSubTyp
TestObjLine   get ln of Plg -> GotSubSeg
TestObjPoints get pt on LN/AC/Plg/CCV -> AP_pt_segpar ("P(L21 MOD(iSeg)|lpar)")

*/



  int       irc, i1, i2, i3, typReq, fLen, actRec, recFull;
            // subtypSel, ccvtypSel, actRec,
            // iSeg, iSubSeg, sSel, iaSeg[3], iaNr;
  long      cPos, dli;
  double    lpar, d1;
  char      *p1, *p2, tmpBuf[256], auxBuf[32], outFmt[8], typChar;
  Point     pt1;
  Circ      ci1;
  ObjGX     *cv1, oxo;
  ModelRef  *modr1;
  ModelBas  *modb1;


  // printf("SSSSSSSSSS IE_sel_CB_1 typSel=%d ind=%ld |%s|\n",typSel,ind,buf);


  // exit if no function active ...
  if(IE_cad_typ < 0) {
    MSG_pri_0 ("CAD_On");  // select CAD-Function first ..
    return 0;
  }


  IE_selTyp = typSel;
  IE_selInd = ind;


  // ist delete active ??
  if(IE_delete == 0) goto L_cad_work;


  //===============================================================
  // delete inputObj
  strcpy(IE_outTxt, buf);
  strcat(IE_outTxt, "=");

  IE_cad_OK ();


  return 0;




  //===============================================================
  L_cad_work:       // CAD create ..

/*
  test ob Obj brauchbar
    JA: Obj einspeichern,
        zugehoerigen Text generieren und ins Entryfeld stellen
  test ob alle Eingaben vorh;
    JA: Zeile generieren,
        Zeile exekutieren
        OK-Button freigeben
*/



  //================================================================
  // see AP_typDB_typ
  // if(typSel == Typ_CI) typSel = Typ_AC;

  // printf(" IE_inpInd=%d IE_inpAnz=%d IE_ed1_win.stat=%d\n",
           // IE_inpInd,IE_inpAnz,IE_ed1_win.stat);



  //----------------------------------------------------------------
  // test < Group of objects
  // if(IE_grp_typ == '<') {  // normal-group
  if(GROUP_IS_ACTIVE) {

    // temporary display position (with little red circle)
    // for indicate, point; not curve ..
    if((typSel == Typ_TmpPT)||(typSel == Typ_PT))
      UI_disp_Pos (Typ_Txt, buf);
   
    // only one normal-group in a command; the normal-group-text is stored
    // in IE_buf.
    // add new obj to IE_buf.
    // strcpy (tmpBuf, buf);
    IE_buf_add (buf);

    // write into ed1-window and test
    // if(IE_ed1_win.stat != 0) {
    if(GUI_OBJ_IS_VALID(&IE_edWin1)) {
      // add tmpBuf into window
      // IE_ed1_mod (NULL, GUI_SETDAT_ES(TYP_EventPress,tmpBuf));
      IE_ui_func (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncSet));
    }
    goto L_done;
  }



  //----------------------------------------------------------------
  // test (  Union  (Group with U())
  if(IE_grp_typ == '(') {
    // IE_set_inp (IE_inpInd, buf); // display obj in inputField
    // strcat(IE_buf, buf);
      // printf(" union-group\n");
    // test if obj is already in grp
    if(Grp_ck_def (typSel, ind) >= 0) {
      i2 = -1;   // exists already - remove from grp
    } else {
      i2 = 1;    // add to grp
    }
    dli = DL_dli__dbo (typSel, ind, -1L);
      // printf(" grp1__ %d %d\n",dli,i2);

    if(dli >= 0) DL_grp1__ (dli, NULL, i2, 0);         // add/remove obj

    goto L_done;
  }



  //----------------------------------------------------------------
  // normal object.
  // old unused functions:
  //   IE_inpTest - test if input complete  see also IE_decode_Ln

  // reset hilite of selected obj   // 2014-04-17
  DL_hili_off (GR_selDli);


  // get fieldText
  p1 = IE_get_inp__ (IE_inpInd);
    // printf(" IE_inpInd=%d fieldText=|%s|\n",IE_inpInd,p1);


  UTX_pos_skipLeadBlk (buf); // skip leading blanks (" CX")


  // add or replace or remove or ignore input.
  irc = IE_inpCkAdd (&i1, &i2, &i3, typSel, p1, buf);
    // printf(" f-IE_inpCkAdd i1=%d i2=%d i3=%d irc=%d\n",i1,i2,i3,irc);
    // printf(" ex _inpCk = %d %d %d\n",i1,i2,i3);
    // i1: 0=replace; 1=add; 2=remove; 3=insert as 1.word
    // i2: -1=gotoNextField; 0=keep,test; 1=keep,do not test
    // i3: 0=display; 1=do not reDisplay;



  // i1: 0=replace; 1=add; 2=remove; -1=skip
  if(i1 == 0) {  // 0=replace
    // replace complete field by input.
    // write IE_outTxt into inputField [actRec]
    IE_set_inp (IE_inpInd, buf);


  } else if(i1 == 1) {  // 1=add
    // add " <input>"
    if(strlen(p1) > 0) {
      sprintf(IE_outTxt, "%s %s",p1,buf);
      IE_set_inp (IE_inpInd, IE_outTxt);
    } else {                    
      IE_set_inp (IE_inpInd, buf);
    }


  } else if(i1 == 2) {  // 2=remove
    // <input> already exists, remove <input> from field
    // copy <p1> -> IE_outTxt
    strcpy (IE_outTxt, p1);
    // remove leading blanks of input
    UTX_pos_skipLeadBlk (buf);
    // remove <buf> (and its following delimiter) out of IE_outTxt
    if(UTX_cut_word (IE_outTxt, buf) < 0) goto L_done;
    // remove following blanks
    UTX_del_follBl (IE_outTxt);
    IE_set_inp (IE_inpInd, IE_outTxt);


  } else if(i1 == 3) {  // 3=insert as 1.word
    if(strlen(p1) > 0) {
      sprintf(IE_outTxt, "%s %s",buf,p1);
      IE_set_inp (IE_inpInd, IE_outTxt);
    } else {                    
      IE_set_inp (IE_inpInd, buf);
    }


  } else if(i1 == -1) {  // -1=skip, ignore
    goto L_done;

  }


  //----------------------------------------------------------------
  // update display for vectors (cannot select direct)
  if(i3 == 0) {
    // display temporary
    GLBT_set_stat (IE_inpInd); // update 2D-icons
    IE_cad_Inp_disp__ (IE_inpInd, 0);

  } else {
    DL_Redraw ();
  }


  //----------------------------------------------------------------
  // i2: 1=keep active field, do not test (not complete);
  if(i2 == 1) {
    // set cursor to end of inputField <IE_inpInd>
    GUI_entry_pos_set (-1, &IE_wCad_obj[IE_inpInd]);
    return 0;


  //----------------------------------------------------------------
  // activate next inputField
  } else if(i2 < 0) {
    IE_inp_chg (-1);  // activate next inputfield
    return 0;
  }



  goto L_done;

/*
  // Loop ueber alle InputFields; ev kann ein noch leeres Feld den Input brauchen
  for(actRec=0; actRec<IE_inpAnz; ++actRec) {
      // printf(" nxt actRec %d .....\n",actRec);

    // get fieldText
    p1 = IE_get_inp__ (actRec);
      // printf(" fieldText=|%s|\n",p1);

    // skip bereits besetzte Felder;      SEE ALSO IE_inpTest   !!
    // eine Ausnahme ist Typ_PLN/Typ_goAxis aus PT+PT/PT+VC/PT+LN
    // aber nur wenn das Inputfeld aktiv ist.
    // ist actRec das aktive inputField ?
    if(actRec != IE_inpInd) continue; // nein; skip this field

    // printf(" >>>>>> aktives inputField %d |%s|\n",actRec,p1);
    if(strlen(p1) > 0) {       // es ist schon was im Feld ..

    // skip feld, wenn mehr als ein object ..
    if(UTX_wordnr(p1) > 1) continue;

      // check for Point in Typ_PLN-Field
      if(p1[0] == 'P') {       // ist ein Punkt
        if((IE_inpTypR[actRec] == Typ_PLN)       ||
           (IE_inpTypR[actRec] == Typ_VC)        ||
           (IE_inpTypR[actRec] == Typ_goAxis)) goto L_ck_inp;
      }
      // // ein Vector braucht mind 2 Zahlen !
      // if(UTX_ck_num_f(&p2, p1) == 0) {   // es ist eine Zahl im Inputfeld
        // if(IE_inpTypR[actRec] == Typ_VC) goto L_ck_inp;   // unvollstaendig ..
      // }
      continue;                // skip this field ..
    }



    L_ck_inp:

    // get group-typ for this inputField
    typChar = IE_cad_act[actRec+IE_first].info[0];

    typReq = IE_inpTypR[actRec];
      // printf(" typReq=%d typSel=%d typChar=%c\n",typReq,typSel,typChar);



    //------------------------------------------------------
    if(typChar != '<') {     // not for Groupedit

      // den Text aus den Inputfeld holen
      p1 = IE_get_inp__ (actRec);
      strcpy(IE_outTxt, p1);
      fLen = strlen(p1);


      // testen ob dieses Feld schon komplett
      // if(IE_inpStat[actRec] == '1') continue;
      i2 = IE_inpTest (typReq, IE_outTxt);
      if(i2 >= 0) continue;

      if(fLen > 0) strcat(IE_outTxt, " ");

    }


      // printf(" %d start test typSel=%d typReq=%d\n",actRec,typSel,typReq);
      // if(typSel == Typ_CV) printf(" subtypSel=%d\n",subtypSel);


// ACHTUNG:
// - wurde durch sele_decode () ersetzt !
    strcpy(tmpBuf, buf);                  // save input to tmpBuf
    strcat(IE_outTxt, buf);               // add input to IE_outTxt
    goto GotAllInput; // copy out ..

  }



  //========================================================
  // keine Auswertung = Fehler.
  // OS_beep ();
  // TX_Error("cannot use %s",buf);
  TX_Print("cannot use %s",buf);
  DL_Redraw ();
  return 0; // unbrauchbarer Inp.



  //========================================================
  // copy Text <IE_outTxt> ins Entryfeld
  GotAllInput:
    // printf(" _sel1 GotAllInput |%s|\n",IE_outTxt);

  //------------------------------------------------------
  // normal objects:
  // write IE_outTxt into inputField [actRec]
  // gtk_entry_set_text ((GtkEntry*) (IE_wCad_obj[actRec]), IE_outTxt);
  IE_set_inp (actRec, IE_outTxt);


  // // save input in IE_inpTxtG
  // strcpy(IE_inpTxtG[actRec], tmpBuf);


  // Testen ob naechstes Feld aktivieren
  recFull = IE_ckFull (typReq, IE_outTxt);


  // noch das naechste Eingabefeld aktivieren
  if(recFull == 0) {
    ++actRec;
    IE_inp_chg (actRec);

  // oder den cursor ans Feldende
  } else {
    i2 = strlen(IE_outTxt);
    GUI_Entry_curPos_set (i2, IE_wCad_obj[actRec], i2);
  }
*/


  //----------------------------------------------------------------
  // test ob alle Eingaben vorh ..
  L_done:
  IE_cad_test__ ();

  // update group-windows
  if(IE_ui_func) {
    IE_ui_func (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncOK));
  }

  return 0;

}



//=====================================================================
  int IE_cad_reset_modif () {
//=====================================================================
// Exit Modify.
// wenn Modus != Add: exit Modify -> Modus Add
// IE_modify: 0=Add 1=Modify 2=Insert

  if(IE_modify == 0) return 0;


  // printf("IE_cad_reset_modif IE_modify=%d\n",IE_modify);

  // gtk_label_set_text ((GtkLabel*) (IE_txtMode), "CREATE  ");
  // GUI_mod_styl (IE_txtMode, 1);   // make cancel-button red
  GUI_label_htm_mod (&IE_txtMode, "CREATE  ");

  IE_modify = 0;                        // reset to ADD; 2010-01-09
  UI_func_stat_set__ (APF_WIN_B_E, 0);  // reactivate browser  2013-09-03

/*
  // display alle elemente, cursor auf letzte zeile:
  ED_work_CurSet (99999);

  // Hintergrund Editfenster hilite ON / OFF
  UI_Ed_hili (OFF);
*/
  return 0;

}


//================================================================
  int IE_cad_exitFunc () {
//================================================================
// IE_cad_exitFunc      exit the active cadFunction
// see also IE_cad_Inp_undo


  // printf("IE_cad_exitFunc IE_modify=%d\n",IE_modify);

  // exit if CAD not yet initialized
  if(UI_InpMode != UI_MODE_CAD) return 0;


  //----------------------------------------------------------------
  if(IE_modify != 0) goto L_mod__;
    // CREATE-MODE (Add)

    // clear error
    // AP_errStat_set (0);
    AP_errStat_reset (0);

    // remove Window GroupInput
    if(IE_ui_func) IE_ui_func (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncKill));

    // clear all input-fields
    IE_cad_ClearInputs (INPRECANZ);

    // clear the header-field IE_entHdr
    GUI_label_mod (&IE_entHdr, "-");

    // disactivate the active menu
    IE_cad_init2 (-1);

    // make all objs selectable ..
    sele_set__ (Typ_goGeom);
    MSG_pri_0 ("CAD_On");

    goto L_exit;


  //----------------------------------------------------------------
  L_mod__:
  if(IE_modify != 1) {
    TX_Error("IE_cad_exitFunc E001 %d",IE_modify);
    return -1;
  }

    // MODIFY-MODE
    // activate "Exit" 
    IE_cad_INS ("Exit");

    goto L_exit;



  //----------------------------------------------------------------
  L_exit:
  return 0;

}


//=====================================================================
  int IE_cad_exit0 () {
//=====================================================================


  // printf("IE_cad_exit0\n");


  // wenn Insert aktiv: nach Add wechseln.
  if(IE_modify != 0) {

    // display alles bis zur akt. Line
    ED_work_CurSet (999999);

    // Hintergrund Editfenster hilite ON / OFF
    UI_Ed_hili (OFF);
  }

  // exit AddToGroup-Mode, wenn aktiv
  UI_grp__ (OFF);    // reset add to group


  // kill GruppenEditorWindow, if activ
  if(IE_ui_func) IE_ui_func (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncKill));


  WC_set_obj_stat (0);   // perm



  return 0;

}



//=====================================================================
  int IE_cad_init0 () {
//=====================================================================
// neue AptIndexe holen (ReRun)

  int   i1, i2;
  long  lNr;


  // printf("00000000000000000000000000000000000000000000 \n");
  printf("IE_cad_init0\n");

  // aktueller ObjTyp=IE_cad_typ,  akt. ObjInd=IE_objInd
  // printf("IE_cad_init0 typ=%d Ind=%ld\n",IE_cad_typ, IE_objInd);

  // UI_AP (UI_FuncFocus, UID_WinCAD, NULL);


  // display alle elemente, cursor auf letzte zeile:
  L_reRun:
  ED_work_CurSet (999999);


  // Ablauffehler: fix ...
  i1 = AP_errStat_get();
  if(i1 != 0) {

    // ask LineNr
    lNr = ED_get_lnr_act ();

    // modify zeile im mem mit dem SystemEditor.
    i2 = AP_src_mod_ed (lNr);

    // reRun
    ED_work_CurSet (0); // TEST
    if(i2 >= 0) goto L_reRun;
    return 0;
  }


  // IE_cad_init_add ();

  MSG_pri_0 ("CAD_On");
  // TX_Print("select CAD-Function / S/M/DEL / Exit: Ctl right mouseb.");

  IE_modify = 0; // Add

  // disact. Modify-Block
  // gtk_widget_set_sensitive (IE_box_INS, FALSE);


  WC_set_obj_stat (1);   // temp


  // init IE_inpAuxDat
  IE_cad_ClearInputs (INPRECANZ);


  if(IE_cad_typ < 0) {
    // TX_Print("** CAD - Funktion selektieren ...");
    // IE_cad_init2 (-1);
    GUI_set_enable (&boxCad2, FALSE);     // disactivate inputs
    return 0;
  }


  // start with filled inputFields
  IE_stat__ = 1;   // 1=startPhase; activate 1 inputField
  IE_inp_CB__ (&IE_wCad_obj[0], NULL);
  IE_stat__ = 0;    // 0=active, normal oper


  // create ObjHeader
  IE_nxtInd ();


  return 0;

}


//=====================================================================
  int IE_ck_menGrpInd (char *lbuf) {
//=====================================================================
// get menu-goup-index menGrpInd; -1=error
//
// Test 1. word of codeline;
//   CUT/REV/MIR .. defines menGrpInd = 12 = IE_Func_Modify = IE_cad__ cad_lst__
//   IMP            defines 13 (menGrpInd)
// if 1. word not in IE_fTbab_Modify: set menGrpInd to type of outputObj IE_cad_typ

  // int  i1;
  char cBuf[256];

  int         i1, i2, im1, im2;
  IE_rec_stru *men1;
  IE_rec_txt  *lst1;


  // printf("IE_ck_menGrpInd |%s|\n",lbuf);



  //----------------------------------------------------------------
  // get first word of string lbuf (a definition-line) into cBuf
  APT_cp_ausd (cBuf, lbuf, 256);
  UTX_cp_word_2_upper(cBuf, cBuf);
    // printf(" cBuf=|%s|\n",cBuf);



  //----------------------------------------------------------------
  // find code <cBuf> in list of menues
  // loop tru menues
  for(im1=0; im1<IE_MEN_NR; ++im1) {
    // get menu, lst
    IE_cad_init_men__ (&i2, &men1, &lst1, im1);

    // loop tru menu-codes
    for(im2=0; im2<100; ++im2) {
        // printf("menGrpInd %d %d|%s|%s|\n",im1,im2,lst1[im2].ftxt,lst1[im2].ftyp);
      if(lst1[im2].ftxt[0] == '\0') break;
      if(strlen(lst1[im2].ftyp) < 2) continue;
        // printf("menGrpInd %d |%s|\n",im1,lst1[im2].ftyp);
      if(!strcmp(lst1[im2].ftyp, cBuf)) {
        menGrpInd = im1;
        // menSubGrpInd = im2; !!
        goto L_exit;
      }
    }
  }



/*   old version:
  // ist cBuf ident mit einem Wort aus der Liste IE_fTbab_Modify:
  i1 = UTX_cmp_word_wordtab (IE_fTbab_Modify, cBuf);
    printf("ex-UTX_cmp_word_wordtab %d\n",i1);
  if(i1 >= 0) {
    if(i1 >= 9) menGrpInd = 13; // IMP joint cad_lst_i_ IE_cad_i_
    else        menGrpInd = 12; // IE_cad__ cad_lst__ IE_Func_Modify
    goto L_exit;
  }
*/



  //================================================================
  switch(IE_cad_typ) {
    case Typ_PT:    menGrpInd = 0; break;
    case Typ_LN:    menGrpInd = 1; break;
    case Typ_CI:    menGrpInd = 2; break;
    case Typ_PLN:   menGrpInd = 3; break;
    case Typ_VC:    menGrpInd = 4; break;
    case Typ_VAR:   menGrpInd = 5; break;
    case Typ_CV:    menGrpInd = 6; break;
    case Typ_SUR:   menGrpInd = 7; break;
    case Typ_SOL:   menGrpInd = 8; break;
    case Typ_Note:
    case Typ_ATXT:
    case Typ_GTXT:  menGrpInd = 9; break;
    case Typ_Model: menGrpInd = 10; break;
    case Typ_Tra:   menGrpInd = 11; break;
    case Typ_Activ: menGrpInd = 13; break;
    case Typ_Joint: menGrpInd = 13; break;
    default:
      TX_Error("Obj.typ not yet supported; edit manually");
      return -1;
  }


  L_exit:
    // printf("ex IE_ck_menGrpInd IE_cad_typ=%d menGrpInd=%d\n",
           // IE_cad_typ,menGrpInd);

  return 0;

}


//================================================================
  int IE_cad_init_men_1 (int mode) {
//================================================================
// disactivate inputs  1=TRUE=active; 0=FALSE=unpickable
// - disables all inputfields, but keeps inputselectionfilter

// 2015-02-18 do not disactivate menu - cannot be reactivated

  // printf("IE_cad_init_men_1 %d\n",mode);

  // GUI_set_enable (&boxCad1, mode);     // disactivate menu
  GUI_set_enable (&boxCad2, mode);    // disactivate inputs

  return 0;

}


//================================================================
  int IE_cad_init__ (int iMain, int iSub) {
//================================================================
// IE_cad_init__        activate mainMenu + Submenu

  void **data;


  printf("IE_cad_init__ %d %d\n",iMain,iSub);

  data = GUI_SETDAT_EI (0,iSub);

  
  switch (iMain) {

    case 0: IE_cad_p_CB (NULL, data); break;
    case 1: IE_cad_l_CB (NULL, data); break;
    case 2: IE_cad_c_CB (NULL, data); break;
    case 3: IE_cad_r_CB (NULL, data); break;
    case 4: IE_cad_d_CB (NULL, data); break;
    case 5: IE_cad_v_CB (NULL, data); break;
    case 6: IE_cad_s_CB (NULL, data); break;
    case 7: IE_cad_a_CB (NULL, data); break;
    case 8: IE_cad_b_CB (NULL, data); break;
    case 9: IE_cad_n_CB (NULL, data); break;
    case 10: IE_cad_m_CB (NULL, data); break;
    case 11: IE_cad_t_CB (NULL, data); break;
    case 12: IE_cad___CB (NULL, data); break;
    case 13: IE_cad_i_CB (NULL, data); break;
    case 14: IE_cad_Ace_CB (NULL, data); break;

    default:
      TX_Error("*** IE_cad_init__ iMain");
  }


  return 0;

}


//=====================================================================
  int IE_cad_init_men__ (int *typ, IE_rec_stru **men, IE_rec_txt **lst,
                         int func) {
//=====================================================================
// IE_cad_init_men__    init menu & inputs


  // printf("IE_cad_init_men__ %d\n",func);

  switch(func) {

    case -1:    // disactivate the active menu
      // (IE_cad_typ < 0) return 0;
      *typ = -1;
      *men = NULL;   //0;
      *lst = NULL;   //0;
      // IE_cad_ClearMenInf ();                // clear Menutext
      // IE_cad_init_men_1 (FALSE);              // disactivate inputs
      // GUI_set_enable (&IE_help_F, FALSE);   // disactivate "Help Func"
      // GUI_set_enable (&IE_help_I, FALSE);   // disactivate "Help Input"
      // GUI_set_enable (&IE_but_Canc, FALSE);   // disactivate "Cancel"
      // GUI_set_enable (&IE_but_Clear, FALSE);   // disactivate "Clear"
      // DL_hili_off (-1L); DL_Redraw ();      // unhilite
      // IE_ui_func = NULL;
      break;

    case IE_Func_Points:
      *typ = Typ_PT;
      *men = IE_cad_p;
      *lst = cad_lst_p;
      // IE_infInd  = UI_inf_sip;
      break;
    case IE_Func_Lines:
      *typ = Typ_LN;
      *men = IE_cad_l;
      *lst = cad_lst_l;
      break;
    case IE_Func_Circles:
      *typ = Typ_CI;
      *men = IE_cad_c;
      *lst = cad_lst_c;
      break;
    case IE_Func_Planes:
      *typ = Typ_PLN;
      *men = IE_cad_r;
      *lst = cad_lst_r;
      break;
    case IE_Func_Vectors:
      *typ = Typ_VC;
      *men = IE_cad_d;
      *lst = cad_lst_d;
      break;
    case IE_Func_Values:
      *typ = Typ_VAR;
      *men = IE_cad_v;
      *lst = cad_lst_v;
      break;
    case IE_Func_Curves:
      *typ = Typ_CV;
      *men = IE_cad_s;
      *lst = cad_lst_s;
      break;
    case IE_Func_Surfaces:
      *typ = Typ_SUR;
      *men = IE_cad_a;
      *lst = cad_lst_a;
      break;
    case IE_Func_Solids:
      *typ = Typ_SOL;
      *men = IE_cad_b;
      *lst = cad_lst_b;
      break;
    case IE_Func_Note:
      *typ = Typ_Note;          // Typ_GTXT;
      *men = IE_cad_n;
      *lst = cad_lst_n;
      break;
    case IE_Func_Models:
      *typ = Typ_Model;
      *men = IE_cad_m;
      *lst = cad_lst_m;
      Mod_mkList (1); // make list of all Submodels -> <baseDir>/tmp/Mod.lst
      break;
    case IE_Func_Transform:
      *typ = Typ_Tra;
      *men = IE_cad_t;
      *lst = cad_lst_t;
      break;
    case IE_Func_Modify:       // CUT INT TRA Project Mirror
      *typ = Typ_CV;
      *men = IE_cad__;
      *lst = cad_lst__;
      break;
    case IE_Func_Interact:      // Interactiv
      *typ = Typ_Activ;
      *men = IE_cad_i_;
      *lst = cad_lst_i_;
      break;
/*
    case IE_Func_Analyze:      // ANALYZE
      *typ = Typ_Error;
      *men = IE_cad_z;
      *lst = cad_lst_z;
      break;
*/
    case IE_Func_CADEnv:       // ActiveCADEnv
      *typ = Typ_Error;
      *men = IE_cad_Ace;
      *lst = cad_lst_Ace;
      break;

  }

  return 0;

}

 
//=====================================================================
  int IE_cad_init2 (int ind) {
//=====================================================================
// set Indicator ob PT od LN aktiv
// called beim init und von IE_activate
// sets IE_cad_typ, IE_cad_act
// Input:
//   ind   -1   disactivate the active menu
//        >=0   new objtype IE_cad_typ



  // printf("222222222222222222222222222222222222222222222222222\n");
  // printf("IE_cad_init2 %d %d %d\n",ind,IE_cad_typ,IE_FuncTyp);
  // printf("     typ=%d\n",IE_cad_typ);

  // disactivate inputFields
  IE_inp_CB__ (NULL, NULL);

  // if no cad-func active: activate "Help Func" && "Help Info"
  if(IE_cad_typ < 0) {
    if(ind >= 0) {
      GUI_set_enable (&IE_help_F, TRUE);     // activate "Help Func"
      GUI_set_enable (&IE_help_I, TRUE);     // activate "Help Input"
      GUI_set_enable (&IE_but_Canc, TRUE);  // activate "Cancel"
      GUI_set_enable (&IE_but_Clear, TRUE);  // activate "Cancel"
    }
  }

  // remove Color von altem menu
  if(IE_cad_act) {
    IE_inp_set_menu_col (0, IE_FuncTyp, NULL);
  }

  if((!IE_cad_act) && (ind >= 0))
    IE_cad_init_men_1 (TRUE);    // reactivate inputs

  menGrpInd  = ind;


  if((menGrpInd < 0)&&(IE_cad_typ < 0)) return 0;

  // set IE_cad_typ, IE_cad_act, IE_lst_act
  IE_cad_init_men__ (&IE_cad_typ, &IE_cad_act, &IE_lst_act, menGrpInd);

  // disactivate menu
  if(IE_cad_typ < 0) {
      IE_cad_ClearMenInf ();                  // clear Menutext
      IE_cad_init_men_1 (FALSE);              // disactivate inputs
      GUI_set_enable (&IE_help_F, FALSE);     // disactivate "Help Func"
      GUI_set_enable (&IE_help_I, FALSE);     // disactivate "Help Input"
      GUI_set_enable (&IE_but_Canc, FALSE);   // disactivate "Cancel"
      GUI_set_enable (&IE_but_Clear, FALSE);  // disactivate "Clear"
      DL_hili_off (-1L); DL_Redraw ();        // unhilite
      IE_ui_func = NULL;
  }


    // if(IE_cad_typ >=0) printf(" sel.subMenu=|%s|\n",
      // IE_lst_act[menSubGrpInd].ftxt);


  // den neachsten freien Objheader feststellen und anzeigen
  // aber nur wenn ObJTyp geaendert wurde
  // IE_nxtInd ();

/*
  // Checkbox newObj aktiv/inaktiv
  if(IE_lst_act == cad_lst__)
    gtk_widget_set_sensitive (IE_ck1, TRUE);
  else
    gtk_widget_set_sensitive (IE_ck1, FALSE);
*/

  // den aktiven shaden
  // gtk_widget_set_sensitive (IE_MenInd[IE_FuncTyp], FALSE);


  // den neuen hiliten
  // gtk_widget_set_sensitive (IE_MenInd[ind], TRUE);


  GL_temp_del_all (); // alle temp. obj loeschen ..


  // neues menu markieren
  if(IE_cad_typ >= 0) IE_inp_set_menu_col (1, ind, NULL);

  IE_FuncTyp = ind;


  // // alle inputfields loeschen ..
  // IE_cad_ClearInputs ();


  // AP_errStat_set (0);  // clear Error
  AP_errStat_reset (0);  // clear Error


  // block / unblock groupSelections (RubberBox)
  if(IE_cad_typ < 0) UI_block_group (0);    // unblock
  else               UI_block_group (1);    // block


    // printf("ex IE_cad_init2\n");

  return 0;
}


/* UNUSED
//================================================================
  int IE_cad_ClearAct () {
//================================================================
// IE_cad_ClearAct      clear display corresponding to active inputField


  printf("AAAAAAAAAAAAA IE_cad_ClearAct %d\n",IE_inpInd);

  if(UI_InpMode != UI_MODE_CAD) IE_inpInd = 0;

  GL_temp_del_1 (-2 - IE_inpInd);

  return 0;

}
*/
 

//================================================================
  int IE_cad_ClearInp1 (int ii, int iRedraw) {
//================================================================
// IE_cad_ClearInp1     clear one inputField
// iRedraw      0=redraw;     1=do not redraw

  long   dli;


  // printf("CCCCCCCCCCC IE_cad_ClearInp1 %d %d\n",ii,iRedraw);


  if(ii < 0) return 0;
  if(ii >= INPRECANZ) return 0;


  // clear inputField ii
  // IE_inpTxtG[ii][0] = '\0';
  IE_set_inp (ii, "");


  // delete temp obj (-IE_inpInd - 2)
  // temp -2 = field 0; temp -3 = field 2.
  dli = IE_GET_INP_DLI (ii);
  GL_temp_del_1 (dli);


  // init IE_inpAuxDat
  UT3D_vc_setFree (&IE_inpAuxDat[ii].vx);


  if(!iRedraw) DL_Redraw ();


  return 0;

}


//================================================================
  int IE_info_col_set (int mode, int ind) {
//================================================================
/// \code
/// IE_info_col_set      change color of info-field
///   mode = 0: reset; 1=set Color (red).
/// \endcode


  // printf("IE_info_col_set ind=%d mode=%d\n",ind,mode);

  GUI_label_styl (&IE_wCad_info[ind], mode);  // 1=red, 0=std

  return 0;

}


//================================================================
  int IE_cad_ClearMenInf () {
//================================================================
// IE_cad_ClearMenInf   clear MenuInfos (right of inputboxes)

  int     i1, i2;

  // printf("IE_cad_ClearMenInf %d %d\n",IE_first,IE_last);

  i2 = IE_last - IE_first;
  i2 = IMIN(i2, INPRECANZ-1);

  for(i1=0; i1 <= i2; ++i1) 
    GUI_label_mod (&IE_wCad_info[i1], "-");

  return 0;

}


//================================================================
  int IE_cad_ResetInputs () {
//================================================================
// IE_cad_ResetInputs   clear all inputFields & goto inputField 0

  if(UI_InpMode != UI_MODE_CAD) return 0;

  IE_cad_ClearInputs (IE_inpAnz);       // alle Inputfelder loeschen

  IE_inp_chg (0);    // feld 1 aktivieren

  return 0;

}


//================================================================
  int IE_cad_ClearInputs (int iNr) {
//================================================================
// clear all inputFields

  int i1;

  // printf("CCCCCCCCCCCCCC IE_cad_ClearInputs %d\n",iNr);
  // printf("  IE_ed1_win.stat=%d\n",IE_ed1_win.stat);


  // clear all Inputs (IE_wCad_obj)
  // alle Inputfelder loeschen
  // den Caret kriegt man nicht weg ..
  for(i1=0; i1<iNr; ++i1) IE_cad_ClearInp1 (i1, 1);

  // reset active-color of inputFieldText

  // set cursor back into 1. field   2013-09-03
  GUI_obj_focus (&IE_wCad_obj[0]);


  // Reset GruppenWindow
  if(IE_modify != 1) {  // nicht bei Modify !
    IE_bufSiz = 0;
    IE_buf[0] = '\0';

    // if(IE_ed1_win.stat != 0) {
    if(GUI_OBJ_IS_VALID(&IE_edWin1)) {
      // GUI_Ed_Write (&IE_ed1_wText, IE_buf, &IE_bufSiz, IE_bufMax);
      // GUI_Ed_Write (&IE_edWin1, IE_buf, &IE_bufSiz, IE_bufMax);
      GUI_edi_Write (&IE_edWin1, IE_buf, &IE_bufSiz, IE_bufMax);
    }
  }


  // clear objectName IE_entNam
  GUI_entry_set (&IE_entNam, "");


  // OK-Butt. disact.
  IE_cad_set_OK (0);

  // printf("ex IE_cad_ClearInputs\n");

  return 0;

}


//=====================================================================
  int IE_cad_get_func (int *typ, int *subTyp) {
//=====================================================================

  *typ    = IE_FuncTyp;
  *subTyp = IE_FuncSubTyp;

  // printf("ex IE_cad_get_func %d %d\n",*typ,*subTyp);

  return 0;

}


//=====================================================================
  int IE_cad_init1 (int ind) {
//=====================================================================
// activate Submenu
// ind ist der gewahlte Funktionstyp (die ZeilenNr)


  static int nObjOld=INPRECANZ, nObjR;

  int  i1, i2;
  char *p1, *p2, cbuf[256];



  if(ind < 0) ind = IE_FuncSubTyp;   // reactivate


  // printf("11111111111111111111111111111111111111111111111111111111111\n");
  // printf("IE_cad_init1 IE_FuncTyp=%d IE_FuncSubTyp=%d IE_modify=%d\n",
         // IE_FuncTyp,ind,IE_modify);
  // printf(" IE_cad_typ=%d\n",IE_cad_typ);


  IE_stat__ = 2;    // 2=createPhase; create Menu & empty inputFields.

  IE_FuncSubTyp = ind;



  // fix IE_cad_typ for joints
  if((IE_FuncTyp == 13) && (IE_FuncSubTyp >= 1)) {        // joint export
    IE_cad_typ = Typ_Joint;
  }


  // printf(" 1.Rec %d %d %s\n",
        // IE_cad_act[0].ind,IE_cad_act[0].typ,IE_cad_act[0].info);
  // printf(" 2.Rec %d %d %s\n",
        // IE_cad_act[1].ind,IE_cad_act[1].typ,IE_cad_act[1].info);
  // printf(" 3.Rec %d %d %s\n",
        // IE_cad_act[2].ind,IE_cad_act[2].typ,IE_cad_act[2].info);



  // wenn GenerateNewLine: den neuen ObjIndex holen.
  if(IE_modify != 1) IE_nxtInd ();


  Grp_Clear (1); // clear group


  // clear active function
  IE_cad_InpIn__ (-2);


  // zuerst Inputfelder loeschen
  IE_cad_ClearInputs (IE_inpAnz);


  // ein Reset des GruppenWindow erzwingen
  // IE_inp_CB__ (NULL, NULL);


  // reset hilited
  GL_temp_del_all ();     // alle temp. obj loeschen
  DL_hili_on (-1L);      // loeschen der Hili-Tabelle
  DL_Redraw ();


  // InputMode; bei CCVS Viewmode (letztes Obj anders behandeln)
  ED_enter();
  WC_set_obj_stat (1);

  // printf(" IE_cad_init1-L1\n");

  i1=0;



  // find first Index in IE_cad_act
  // printf(" ------ find IE_first\n");
  if(IE_cad_act == NULL) return -1;
  while(IE_cad_act[i1].ind >= 0) {
    // printf(" i1=%d act.ind=%d\n",i1,IE_cad_act[i1].ind);
    if(IE_cad_act[i1].ind == ind) {
      IE_first = i1;
      goto FindLast;
    }
    ++i1;
  }
  return -1;


  // find first-/last Index in IE_cad_act
  FindLast:
  i1 = IE_first;
    // printf(" last ind=%d i1=%d act=%d\n",ind,i1,IE_cad_act[i1].ind);

  while(IE_cad_act[i1].ind == ind) {
    ++i1;
  }
  IE_last = i1-1;
    // printf(" first=%d last=%d\n",IE_first,IE_last);

  // if((IE_last-IE_first) >= INPRECANZ) {
    // TX_Error("IE_cad_init1 E001"); // zu viele Inputparameter
    // return -1;
  // }





  //------------------------------------------------------
  // print info into Menulines and -
  // get preloadText out of .info into IE_wCad_preLoad
  nObjR=0;   // index inputField
  for(i1=IE_first; i1<=IE_last; ++i1) {
      // printf(" inp:iCad=%d ind=%d inf=|%s|\n",i1,nObjR,IE_cad_act[i1].info);

    if(nObjOld <= nObjR)
      GUI_set_enable (&IE_wCad_obj[nObjR], TRUE);

    // if(IE_cad_act[i1].typ == Typ_Subtyp) continue;
    strcpy(cbuf, IE_cad_act[i1].info);
    // split info DefaultText abtrennen
    p1 = strchr(cbuf, '|');
    if(p1) {
      // i2 = p1 - cbuf;  // length of displayText
      *p1 = '\0';
      ++p1;  // pointer to preloadText
      p2 = strchr(p1, '|');
      if(p2) {
        // keep input-subTypes
        *p2 = '\0';
        ++p2;
        // save subtypes
        strcpy(IE_inpAuxDat[nObjR].auxInf, p2);

      } else {
        // no subtypes
        IE_inpAuxDat[nObjR].auxInf[0] = '\0';
      }
      // save preloadText
      strcpy(IE_wCad_preLoad[nObjR], p1);


    } else {
      IE_wCad_preLoad[nObjR][0] = '\0';
      IE_inpAuxDat[nObjR].auxInf[0] = '\0';
    }
    // write whole .info = displayText
    GUI_label_mod (&IE_wCad_info[nObjR], cbuf);
    IE_info_col_set (0, nObjR);  // clear color   // 2014-04-06

      // printf(" ind=%d dispTxt=|%s| preLoad=|%s| auxInf=|%s|\n",nObjR,
              // cbuf,IE_wCad_preLoad[nObjR],IE_inpAuxDat[nObjR].auxInf);

    ++nObjR;
    if(nObjR > INPRECANZ) {
      TX_Error("IE_cad_init1 E001"); // zu viele Inputparameter
      return -1;
    }
  }


  // delete unused Menulines
  // disactivate unused entries & info
  // printf(" nObjOld=%d,nObjR=%d\n",nObjOld,nObjR);
  if(nObjOld > nObjR) {
    for(i1=nObjR; i1<nObjOld; ++i1) {
        // printf(" disactiv. inp[%d]\n",i1);
      GUI_set_enable (&IE_wCad_obj[i1], FALSE);
      GUI_label_mod (&IE_wCad_info[i1], " ");
    }
  }
  nObjOld=nObjR;


/*
  // Funktionsinfo -> Messageline rausschreiben
  // ACHTUNG: Laenge begrenzen !
  // printf(" info=|%s|\n",IE_lst_act[ind].ftxt);
  gtk_label_set_text ((GtkLabel*) (IE_txtHdr), IE_lst_act[ind].ftxt);
*/



  //------------------------------------------------------
  // copy requested types -> IE_inpTypR / IE_inpAnz
  IE_inpAnz = IE_last - IE_first + 1;
  i2 = 0;
  for(i1=IE_first; i1<=IE_last; ++i1) {
    IE_inpTypR[i2] = IE_cad_act[i1].typ;
    ++i2;
  }



  // OK-Butt. deaktivieren
  IE_cad_set_OK (0);


  // IE_ed_lNr feststellen
  // IE_ed_lNr = ED_Get_LineNr ();



  // Reset Panels
  // IE_inp_CB__ (NULL, NULL);


  // // activate first inputField (else inputFields not active !)
  // IE_inp_chg (0);   // 2011-11-23 removed for IE_activate


  // active FuncWord -> IE_FncNr (IE_FncNr 0=CUT, 2=TRA ..)
  p1 = IE_lst_act[IE_FuncSubTyp].ftyp;
  IE_FncNr = UTX_cmp_word_wordtab (IE_fTbab_Modify, p1);


/*
  if(IE_FncNr != 1) {         // 0=CUT; 1=INT
    gtk_widget_set_sensitive (IE_ck1, TRUE);      // view
    // IE_set_ck1 (1);        //   newObj on 0=off 1=on
  } else {
    gtk_widget_set_sensitive (IE_ck1, FALSE);     // INT: hide
    // IE_set_ck1 (0);        // newObj off
    // disactivat newObj (Probleme transl+overWrite)
  }
*/


  IE_stat__ = 0;     // 0=active, normal operation.

  // activate 1. inputField      2011-11-23
  IE_inp_chg (0);


    // printf("ex IE_cad_init1\n");

  return 0;

}


//=====================================================================
  int IE_inp_chg (int iNxt) {
//=====================================================================
// activate other inputfield (set IE_inpInd).
// -1   next
// -2   previous (zB Cursor up)
// -3   do NOT change inputfield at following call
// >=0  index


  int    indOld;
static int _do_not = 0;



  // printf("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF   IE_inp_chg %d\n",iNxt);
  // printf(" IE_inp_chg IE_inpAnz=%d IE_inpInd=%d\n",IE_inpAnz,IE_inpInd);


  if(iNxt == -3) {
    _do_not = 1;
    return 0;
  }


  if(_do_not) {
    _do_not = 0;
    return 0;
  }




  indOld = IE_inpInd;


  // -1   next
  if(iNxt == -1) {
    // if(IE_inpInd >= INPRECANZ) {
    iNxt = IE_inpInd + 1;




  // -2   previous (zB Cursor up)
  } else if (iNxt == -2) {
    iNxt = IE_inpInd - 1;
    // if(IE_inpInd < 0) IE_inpInd = INPRECANZ-1;


  } else if (iNxt < -2) {
    // ERROR
    return -1;


  }



  if(iNxt < 0) iNxt = 0;


  // last entry: only test ..
  if(iNxt >= IE_inpAnz) {
    IE_cad_Inp_disp__ (IE_inpInd, 0);      // display temp. symbol
    IE_cad_test__ ();
    return 0;
  }


/*
  if(iNxt != IE_inpInd) {
    // modify color of inputfield-info-label.
    if(IE_inpInd >= 0) IE_info_col_set (0, IE_inpInd); // reset old
    IE_info_col_set (1, iNxt);                         // set new info=red
  }
*/

    // printf(" next inputfieldInd = %d\n",iNxt);

  // activate inputField iNxt
  IE_cad_InpIn__ (iNxt);   // statt GUI_obj_focus 2012-09-17


/* 2012-09-13
  // activate inputField iNxt
  // UI_AP (UI_FuncFocus, UID_WinCAD, NULL);
  GUI_obj_focus (&IE_wCad_obj[mode]);
  // in Linux impliziert gtk_widget_grab_focus ein IE_inp_CB__,
  // nicht in MS; daher: (else manchmal kein IE_ed1__)
#ifdef _MSC_VER
  if(IE_inpInd < 0) IE_cad_InpIn__ (0);
#endif
*/

  // error: reactivate previous field
  if(AP_errStat_get()) {
      // printf(" ======== reactivate field %d\n",indOld);
    sele_set__ (IE_inpTypR[indOld]);    // reset GR_selTmpStat
    // GUI_obj_focus (&IE_wCad_obj[indOld]);
    if(IE_inpInd >= 0) IE_info_col_set (0, IE_inpInd);  // reset 
    IE_inpInd = indOld;
    if(IE_inpInd >= 0) IE_info_col_set (1, IE_inpInd);  // set info red
    DL_Redraw ();
  }

  // printf("ex IE_inp_chg %d\n",mode);

  return 0;

}


//=====================================================================
  int IE_cad_test__ () {
//=====================================================================
/// \code
/// IE_cad_test__        test if input complete, create sourcline, activate OK-butt
/// RetCode:     0  OK
///             -1  input not yet complete
///             -2  error execute-sourcline
/// \endcode


  static Vector    vc1;  // keep last vcs

  int       aus_typ[10];
  int       irc, i1, i2, i3, i4, ii, iskip, ilen, lNr, sTyp, iActRec;
  long      l1, l2, dbi;
  char      typChar, tmpBuf[256], actBuf[256], *cp1, *p1;
  char      aus_tab[10][256];

  /// IE_lst_act[IE_FuncSubTyp].ftyp; the primary Codeword; "CUT" or "INT" ..
  char      *fncAct;

  char      *ep[INPRECANZ];
  Point     pt1, pt2, pt3;

  // void *w_nam;
  // gchar     *gtxt;



  // printf(">>>>-IE_cad_test__ anz=%d first=%d\n",IE_inpAnz,IE_first);
  // printf("     IE_FuncTyp=%d IE_FuncSubTyp=%d\n",IE_FuncTyp,IE_FuncSubTyp);
  // printf(" IE_buf=|%s|\n",IE_buf);
  // printf(" IE_EdFnc=%d\n",IE_EdFnc);
  // printf(" IE_cad_typ=%d IE_objInd=%ld\n",IE_cad_typ,IE_objInd);


  if(!IE_lst_act) return 0;         // Drama - woher ??

  if(IE_EdFnc == 2) return 0;       // TextEdit


  // es muss grp u subgrp aktiv sein
  if(IE_cad_typ < 0) return -1;


  AP_errStat_reset (0); // reset temp. Errors   2011-08-07


  fncAct = IE_lst_act[IE_FuncSubTyp].ftyp;
    // printf(" fncAct=|%s|\n",fncAct);


  // get pointers to all inputfields into ep;
  IE_inp_ck_prev_empty (ep);


  //================================================================
  // test if input complete
  L_test1_1:
    // printf(" IE_first=%d act.info=|%s|\n",IE_first,IE_cad_act[IE_first].info);
  i3 = 0;  // nr of inputfields-not-empty
  // loop tru inputfields
  for(i1=0; i1<IE_inpAnz; ++i1) {
    // set ii = index into IE_cad_act (active IE_rec_stru, eg IE_cad_a)
    ii = i1 + IE_first;

      // printf("%d |%s|%s|\n",i1,IE_cad_act[ii].info,IE_inpTxtG[i1]);
    typChar = IE_cad_act[ii].info[0];
      // printf(" Entry-%d ii=%d typChar=|%c|\n",i1,ii,typChar);

    if((typChar == '<')||(typChar == '(')) {
      // group-field; if field is active, then input is in IE_buf
      if(i1 == IE_inpInd) p1 = IE_buf;  // Grp_get_nr ();
      else                p1 = ep[i1];
      UTX_pos_skipLeadBlk (p1);
      i2 = strlen(p1);
      // i2 = UTX_wordnr (p1);
        // printf(" Group-txt=|%s| i2=%d\n",IE_buf,i2);
      if(i2 < 1) goto L_not_ok;         // if(i2 < i4) goto L_not_ok;
      // if(strlen(IE_buf) > 3) goto L_start;
      if(strlen(p1) > 1) ++i3;
      continue;
      // goto L_start;
    }

    // normal input ..
    p1 = ep[i1];
    UTX_pos_skipLeadBlk (p1);
    i2 = strlen(p1);
    if(i2 > 0) ++i3;  // Anzahl Eingaben
      // printf(" ent. %d = |%s| strlen=%d\n",i1,p1,i2);


    // input not necessary, if 1. Char of Infotext = '['
    // if all inputfields are optional ('[') at least one input is necessary
    if(typChar == '[') continue;


    // input for this field mandatory; exit with input not complete
    if(i2 < 1) goto L_not_ok;         // if(i2 < i4) goto L_not_ok;

  }

  // at least 1 input ..
  if(i3 < 1) goto L_not_ok;





  //================================================================
  // Fix Header.
  // test ob ObjHeader offen (CUT, TRA ...)
    // printf("  Fix Header. mod=%d func=%d typ=%d ind=%ld\n",IE_modify,IE_FuncTyp,
           // IE_cad_typ,IE_objInd);

  // no objHdr necessary for activate|reset ConstrPlane
  if(IE_FuncTyp == IE_Func_CADEnv) goto L_start;         // ActiveCADEnv


  // no objHdr necessary for Analyze
  if(!strncmp(fncAct, "ANALYZ", 6)) goto L_start;


/* 2019-04-09
  // set IE_FncNr to index in list IE_fTbab_Modify
  // wenn eine Function aus CUT/INT/TRA aktive, Index IE_FncNr setzen.
  IE_FncNr = UTX_cmp_word_wordtab (IE_fTbab_Modify, fncAct);
    // printf("  IE_FncNr=%d fncAct=|%s|\n",IE_FncNr,fncAct);
  if(IE_FncNr < 0) goto L_start;

  // a function CUT|INT|TRA|.. is activ;
*/

  // if mode = modify: keep existing output-type and index !
  if(IE_modify == 1) {
    // mode=Modify: keep original Header
    APED_dbo_oid (&IE_cad_typ, &IE_objInd, IE_modifHdr);
      // printf(" modify; copy header %d %ld\n",IE_cad_typ,IE_objInd);
    strcpy(IE_outTxt, IE_modifHdr);
    // display name of new outpt-obj in field IE_entHdr
    IE_set_txtHdr ();
    goto L_start;
  }

/*
  //----------------------------------------------------------------
  // handle Grp 12 (IE_Func_Modify: INT,CUT,PRJ,MIR,ISO: get resultingObjTyp
  // get type and name of resulting-obj for modify-curve-functions, eg CUT
  // CUT returns CurvCCV (trimmed-curve) for all input-curves
    // printf(" handle Grp 12 FncNr=%d\n",IE_FncNr);
  IE_cad_typ = IE_cad_test_typ (ep);
  if(IE_cad_typ == Typ_Error) goto L_not_ok;
*/
  goto L_start;



  //---------------------------------------------
  // NICHT alle erforderl. Eingaben sind vorhanden ....
  L_not_ok:
      // printf("IE_cad_test__ L_not_ok\n");
    // OK-Button sperren
    IE_cad_set_OK (0);        // 0=OK-locked; 1=OK-active

    // das zuletzt temporaer erzeugte objekt loeschen
    // GL_last_del ();
    GL_temp_del_1 (DLI_TMP);      // delete temporary outputObject of WC_Work1
    GL_temp_del_1 (DLI_DIR_TMP);  // delete temporary direction-arrow
    DL_Redraw ();
    return -1;





  //---------------------------------------------
  // Alle erforderl. Eingaben sind vorhanden ....
  L_start:
    // printf("IE_cad_test__ L_start:\n");
  GL_temp_del_1 (DLI_DIR_TMP);  // delete temporary direction-arrow
  // necessary if WC_Work1 provides no obj


  // das zuletzt temporaer erzeugte objekt loeschen
  // GL_last_del ();   // bleibt sunst in subModels stehen .. raus 2011-10-03


  // TX_Write("Save: butt OK / right mouseb.");
  MSG_pri_0 ("CADinit");


  // ANALYZ-Function: sofort ausfuehren.
  if(!strncmp(fncAct, "ANALYZ", 6)) {
    IE_analyz__ (fncAct);
    return 0;
  }



  // normale CAD-Func.; OK-Button freigeben
  if(IE_stat_OK != 1) IE_cad_set_OK (1);  // 0=OK-locked; 1=OK-active




  //================================================================
  // Create sourceCode for obj -> IE_outTxt

  // Create Zielobjekt (zB "P12="); make name from typ and DB-index
  // fuer Tests nach ObjInd=0 speichern; sonst kann Obj ueberschrieben werden
  // und kein 2. Versuch mehr moeglich (CUT, TRANSL ..)
  // siehe UI_creObjHdr (NULL, (void*)Typ_PT);
  // UI_AP (UI_FuncSet, UID_inf_sip, NULL);

  // printf(" WC_get_obj_stat=%d\n",WC_get_obj_stat());
  // if(WC_get_obj_stat() == 0) l1 = IE_objInd;
  if(IE_save == 1) l1 = IE_objInd;   // 1=save perm.
  else l1 = 0;

    // printf(" IE_cad_test__ IE_cad_typ=%d ind=%ld\n",IE_cad_typ,l1);


  if(IE_FuncTyp == IE_Func_CADEnv) {             // ActiveCADEnv
    // if(IE_FuncSubTyp == 0) {                  // R##    - sonst nix
      IE_outTxt[0] = '\0';
      goto L_out_par;
    // }

  } else {
    APED_oid_dbo__ (IE_outTxt, IE_cad_typ, l1);  // create "L10"
  }

  strcat(IE_outTxt,"=");
    // printf(" CADheader=|%s|\n",IE_outTxt);


  // add Subtyp (eg "POL")
  if(strlen(IE_lst_act[IE_FuncSubTyp].ftyp) > 0) {
    strcat(IE_outTxt,IE_lst_act[IE_FuncSubTyp].ftyp);
      // printf(" outTxt-2 |%s|%s|\n",IE_outTxt,IE_lst_act[IE_FuncSubTyp].ftyp);
  }




  // add Parameters.
  L_out_par:
    // printf("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO \n");
    // printf(" outTxtStart=|%s| IE_first=%d\n",IE_outTxt,IE_first);
    // printf(" IE_inpAnz=%d IE_inpInd=%d\n",IE_inpAnz,IE_inpInd);



  // i1 = index inputField
  for(i1=0; i1<IE_inpAnz; ++i1) {
       // printf(" ----------------nxt-i1 = %d outTxt=|%s|\n",i1,IE_outTxt);

    iActRec = i1 + IE_first;  // index into IE_cad_act


    sTyp = IE_inpTypR[i1];
      // printf(" add %d, sTyp=%d\n",i1,sTyp);
      // printf("         Info %s\n",IE_cad_act[IE_first+i1].info);

    typChar = IE_cad_act[iActRec].info[0];
      // printf(" typChar = |%c|\n",typChar);


    // normal-group; no "U("; get text from IE_buf while IE_ed1__ win is active
    if(typChar == '<') {
      if(IE_outTxt[strlen(IE_outTxt)-1] != '=') strcat(IE_outTxt, " ");
      p1 = ep[i1];
      // if(!strcmp(p1, "<Group>"))     // 2019-04-26
      p1 = IE_buf;
      UTX_pos_skipLeadBlk (p1);   // skip leading blanks
      strcat(IE_outTxt, p1);
        // printf(" outTxt(group)=|%s|%s|\n",IE_outTxt,IE_buf);
      continue;
    }

    // get union-group
    if(typChar == '(') {
        // printf(" get union-group %d act=%d\n",i1,IE_inpInd);
      if(IE_outTxt[strlen(IE_outTxt)-1] != '=') strcat(IE_outTxt, " ");
      // if active field = "<Group>" then get IE_buf; else field
      // if(ep[i1][0] == '<') p1 = IE_buf;
      if(i1 == IE_inpInd) p1 = IE_buf;
      else                p1 = ep[i1];
      UTX_pos_skipLeadBlk (p1);   // skip leading blanks
      i2 = UTX_del_follBl (p1);  // printf(" _follBl-i2 = %d\n",i2);
      if(i2) {
        strcat(IE_outTxt, "U(");
        strcat(IE_outTxt, p1);
        strcat(IE_outTxt, ") ");
      }
      continue;
    }


    // other fields:
    p1 = ep[i1];
      // printf(" cad-inp-ep[%d]=|%s| sTyp=%d\n",i1,p1,sTyp);

    // skip leading blanks - NOT FOR STRINGS!
    if(sTyp != Typ_String) UTX_pos_skipLeadBlk (p1);

    // is input empty ?
    if(*p1 == '\0') {
      // check if following field has same type and is not empty ..
      if(i1+1 >= IE_inpAnz) continue; // no more inputfields
      // if not - continue;
      if(IE_parDef_ck (iActRec)) continue;
      // following field has same type; test if it is also empty ..
        // printf(" -out_add-nxtField=|%s| len=%ld\n",ep[i1+1],strlen(ep[i1+1]));
      if(strlen(ep[i1+1]) < 1) continue;
      // must provide default-value for this (empty) field;
      // get default-value for type
      IE_parDef_get (actBuf, IE_cad_act[iActRec].typ);
      goto L_out_add2;
    }

    // actBuf = copy input
    strcpy(actBuf, p1);
      // printf(" nxt[%d]=|%s|\n",i1,actBuf);


    // // check for Parameter-defaults to save
    // IE_parDef (1, actBuf, i1+IE_first);
    L_out_add2:
    ilen = strlen(actBuf);

    // iskip=ON if inputfield-info='[' ??
    if((IE_cad_act[iActRec].info[0] == '[')&&(ilen < 1)) {
      iskip = ON;
    } else {
      iskip = OFF;
    }
      // printf(" ilen=%d iskip=%d\n",ilen,iskip);
    if(iskip == ON) continue;


    // create outputText from inputFieldText actBuf
      // printf(" outTxt-3 now |%s|\n",IE_outTxt);
    // <tmpBuf> = create text from input <actBuf>
    // get p1 = point to tmpBuf or actBuf
    irc = IE_inpTxtOut (&p1, tmpBuf, actBuf, i1, iskip);
      // printf(" f-npTxtOut-irc = %d |%s|\n",irc,p1);
    if(irc < 0) return irc;

    i2 = strlen (IE_outTxt);
    --i2;
    if(IE_outTxt[i2] != '=') strcat (&IE_outTxt[i2], " ");
    strcat (&IE_outTxt[i2], p1);
      // printf(" outTxt-4 now |%s|%s|\n",IE_outTxt,p1);


    continue;  // next inpField ..

  }



  //-----------------------------------------------------------------
  // Nun ist die Codezeile IE_outTxt fertig:
  // remove "," am Ende (kann vorkommen, wenn letzter parameter optional)
  cp1 = &IE_outTxt[strlen(IE_outTxt)-2];
  if(!strncmp(cp1,", ",2)) *cp1 = '\0';
    // printf("  _cad_test1 IE_outTxt=|%s|\n",IE_outTxt);
  // Ausg.zeile exekutieren; fuer temporaer (Tab) oder permanent (OK)
  // printf("IE_cad_test__: Ausg.zeile exekutieren\n");


/*
  // Insert: next Line!
  if(IE_modify == 2) {        // 0=Add 1=Modify 2=Insert
   ++IE_ed_lNr;
  }
*/


  // wenn die Codezeile ein Model beschreibt, dann Codezeile in eine Datei sichen
  // (durch das Mod_load_sm usw wird IE_outTxt beleidigt !))
  i1 = 0;
  if(IE_outTxt[0] == 'M') {
    i1 = 1;
    sprintf(tmpBuf,"%sIE_inpTxtG.txt",OS_get_tmp_dir());
    if(UTX_wrf_str(tmpBuf, IE_outTxt) < 0) return -1;
  }



  //-----------------------------------------------------------------
  // execute
    // printf(" L execute\n");
  if(IE_save != 0) {      // 0=preview, 1=save perm.
    WC_set_obj_stat (0);    // set perm

  } else {
    // temp:
    WC_set_obj_stat (1);    // set temp
    DB_CSEG__ (0);          // save DB_CSEG-state
  }


  // work (preview or display and store; call WC_Work__ WC_Work1)
  irc = ED_work_CAD (ED_get_lnr_act (), IE_outTxt);
    // printf(" after ED_work_CAD %d\n",irc);
  if(irc < 0) {
     // Error: lock OK-Button
    IE_cad_set_OK (0);     // 0=OK-locked; 1=OK-active
    DL_Redraw ();          // makes error visible ..
    if(irc == -3) return irc;    // -3 = obj not yet complete
    return -2;
  }



  // update browserWindow
  if(IE_save != 0) {                // 1=save perm.
    // Brw_upd_act (IE_outTxt);
    // IE_objInd points altready to the next unused !
    if(IE_cad_typ > 0)                  // reset ConstrPln: IE_cad_typ=0
      Brw_objRow_upd (IE_cad_typ, IE_objInd);

  } else {
    // temp:
    DB_CSEG__ (1);          // reset DB_CSEG
  }


  // IE_outTxt nach ED_work_CAD:
  if(i1 == 1) {   // only if obj=Model: get src from file
    if(UTX_fgetLine(IE_outTxt, mem_cbuf1_SIZ, tmpBuf, 1) < 0) return -1;
    UTX_CleanCR(IE_outTxt);   // remove LF ..
  }

    // printf("  nach ED_work_CAD IE_outTxt=|%s|\n",IE_outTxt);




  DL_Redraw ();


/*
  // fix Typ and Index for 1 == INT
  if(IE_FncNr == 1) {
    if(IE_cad_typ == Typ_goGeom) {
      IE_cad_typ = WC_get_obj_typ (); // get corrected typ
      IE_objInd = DB_QueryNxtFree (IE_cad_typ, 20);
      printf(" typ,ind fixed: %d %d\n",IE_cad_typ,IE_objInd);
      // Header generieren
      APED_oid_dbo__ (tmpBuf, IE_cad_typ, IE_objInd);
      // Header anzeigen
      gtk_entry_set_text ((GtkEntry*) (IE_entHdr), tmpBuf);
      // der gesamte Text ist nun schon in IE_outTxt; ObjHeader replacen.
      p1 = strchr(IE_outTxt, '=');
      if(p1) {
        i1 = p1 - IE_outTxt;
        UTX_chg_str1 (&l1, IE_outTxt, tmpBuf, IE_outTxt, i1);
        printf(" n chg:|%s|\n",IE_outTxt);
      }
    }
  }
*/

    // printf(" ex IE_cad_test__\n");


  return 0;


}


//================================================================
  int IE_cad_upd_hdr (int hdrTyp, long hdrInd) {
//================================================================
// IE_cad_upd_hdr               change header of definition-line (in IE_outTxt)
// was IE_cad_test_typ
// see also IE_nxtInd
 
  int    i1;
  char   newHdr[32];


  // printf("IE_cad_upd_hdr typ=%d ind=%ld\n",hdrTyp,hdrInd);

  // change header of definition-code;
  IE_cad_typ = hdrTyp;
  IE_objInd  = hdrInd;


  // make new obj-header
  APED_oid_dbo__ (newHdr, IE_cad_typ, IE_objInd);
        // printf(" new hdr |%s|\n",newHdr);


  // if temporary: display new obj-header;
  GUI_label_mod (&IE_entHdr, newHdr);   // see also IE_set_txtHdr()


  // if permanent: change obj-header in sourcecode in IE_outTxt
  strcat(newHdr, "=");
  UTX_chg_left_del (IE_outTxt, sizeof(IE_outTxt), newHdr, '=');


    // printf("ex-IE_cad_upd_hdr %d %ld\n",IE_cad_typ,IE_objInd);

  return 0;

}


//================================================================
  int IE_cad_test_typ (char *ep[]) {
//================================================================
// get type and name of resulting-obj for modify-curve-functions, eg CUT
// CUT returns CurvCCV (trimmed-curve) for all input-curves
//
// Input:
//   IE_FncNr      active subFunc                             extern
//   IE_lst_act IE_FuncSubTyp IE_objInd                       extern
//   ep            input-fields
// Output:
//   IE_cad_typ
//   IE_outTxt     header for resulting-obj                   extern
//   retcod        Typ_Error    = error
//                 else type of resulting-obj
//
// IE_fTbab_Modify = list of codes with outputTypes different to active function.


  int    IE_cad_typ, i1, i2, irc;
  long   dbi;
  char   *p1;


  printf("IE_cad_test_typ IE_FncNr=%d\n",IE_FncNr);
  printf("  \n");



  // fix header for 0=CUT (typ of resulting obj unknown !)
  if(IE_FncNr == 0) {   // "CUT"
    IE_cad_typ = Typ_CVTRM;
    // same typ as in 1 inputObj
    // IE_cad_typ = AP_typ_typChar (ep[0][0]);
     // printf(" 1.obj=|%s| i1=%d\n",ep[0],i1);
    goto L_HdrNew;


  // fix header for 1=INT (typ of resulting obj unknown !)
  } else if(IE_FncNr == 1) {   // "INT"
    // IE_cad_typ = Typ_goGeom; // undefined output-obj
    // IE_objInd  = 0;
    // goto L_HdrKeep; // IE_cad_typ wird erst nach Work1 korrigiert

    i1 = APT_int_cktyp (ep);      // return resultingObjTyp
    if(i1 > 0) {IE_cad_typ = i1;  goto L_HdrNew;}
    printf("****** INT_Error_E001 \n");
    return -1;    // goto L_not_ok;
    // return 0;  // noch nicht implementiert ...


  } else if(IE_FncNr == 3) {   // "PRJ"
    i1 = APT_prj_cktyp (ep);   // find resulting objTyp
    if(i1 > 0) {IE_cad_typ = i1;  goto L_HdrNew;}


  } else if(IE_FncNr == 5) {   // "ISO"
    i1 = APT_iso_cktyp (ep);
    if(i1 > 0) {IE_cad_typ = i1;  goto L_HdrNew;}
    printf("****** ISO_Error_E001 \n");
    return Typ_Error;    // goto L_not_ok;
    // return 0;  // noch nicht implementiert ...


  } else if(IE_FncNr == 6) {   // "REV"
    IE_cad_typ = Typ_CVTRM;
    goto L_HdrNew;
  }


  //----------------------------------------------------------------
/*
  // fix header for "CUT","TRA","PRJ","MIR"; bei allen diesen ist der
  // resulting-Typ ident mit dem Typ des ersten objekts !!!

    // iOver = GTK_TOGGLE_BUTTON (IE_ck1)->active; // 0=OFF, 1=ON
      // printf(" iOver=%d\n",iOver);

    // Bei TRA muss immer ein neues Obj generiert werden !!
    // es wird bei modify das Objekt mehrfach transformiert (Trafo aufaddiert)
    // auch mit "L0=TRA L23 D(0 0 50)" wird Transformation ausgefuehrt;
    // es soll aber nur des aktuelle obj gehilitet werden !
    // muesste also das obj vor der Transformation bekommen (od Tra invers ?)
    if(!strcmp(fncAct, "TRA")) {
      if(iOver == 0) {
        TX_Print("TRA interactiv: reset newObj=ON");
        iOver = 1;
        IE_set_ck1 (1);
      }
    }
*/
    // erstes Eingabefeld holen
    p1 = ep[0];
      // printf(" Specialheader-ent1=|%s|\n",p1);
    i2 = strlen(p1);
    if(i2 > 1) {

      // Typ und Index des Zielobjektes bestimmen
      irc = APED_dbo_oid (&IE_cad_typ, &dbi, p1);

      // Project Circ -> Plane: Ausgabeobjekt ist Curve nicht Circ !!
      if(IE_cad_typ == Typ_CI) {
        if(!strcmp(IE_lst_act[IE_FuncSubTyp].ftyp,"PRJ")) {
          // printf(" prj ci => ell\n");
          IE_cad_typ = Typ_CV;
        }
      }

      // APED_dbo_oid / irc = -1: P(..) oder D(..)
      // if(irc == -1) goto L_HdrNew;

      // if(iOver == 0) {IE_objInd = dbi; goto L_HdrKeep;} // 0=OFF, 1=ON

      L_HdrNew:
        // den naechsten freien ObjInd holen
        i1 = AP_typDB_typ (IE_cad_typ);   // get basic-type
        IE_objInd = DB_QueryNxtFree (i1, 20);
          // printf(" nxt free ind = %ld %d\n",IE_objInd,IE_cad_typ);

      // L_HdrKeep:
      // Header generieren
      APED_oid_dbo__ (IE_outTxt, IE_cad_typ, IE_objInd);
        printf(" new hdr |%s|\n",IE_outTxt);

      // display name of new outpt-obj in field IE_entHdr
      IE_set_txtHdr ();
    }


    printf("ex-IE_cad_test_typ IE_cad_typ=%d IE_objInd=%ld\n",IE_cad_typ,IE_objInd);

  return IE_cad_typ;

}

 
//=====================================================================
  int IE_nxtInd () {
//=====================================================================
// wenn GenerateNewLine: den neuen ObjIndex holen.
// den naechsten freien Objheader feststellen und anzeigen
// mode=0: unbedingt
// mode=1: aber nur wenn ObJTyp geaendert wurde

  int       istart;
  // char      cbuf[12];




  // printf("IE_nxtInd mod=%d typ=%d Ind=%ld\n",IE_modify,IE_cad_typ,IE_objInd);


  // wenn kein Typ aktiv: exit.
  if(IE_cad_typ < 1) return 0;

  // if(mode == 1) {
    // if(old_typ =

  if(IE_modify == 1) goto L_raus;           // 0=Add 1=Modify 2=Insert

  // den StartIndex (weiter)setzen
  // UI_AP (UI_FuncSet, UID_inf_sip, (void*)ind);
  // sprintf(cbuf, "%6ld", IE_objInd);    // 999999 rechtsbuendig mit blanks
  // sprintf(cbuf, "%ld", IE_objInd);    // linksbuendig
  // gtk_entry_set_text ((GtkEntry*) (IE_infInd), cbuf);


  // akt. Startindex lesen
  // gtxt = gtk_entry_get_text ((GtkEntry*) (IE_infInd));
  // gtxt = gtk_editable_get_chars ((GtkEntry*) (w_nam), 1, 5);
  // printf("StartInd=|%s|\n",gtxt);
  // istart = atoi(gtxt);
  istart = 20;

  // den naechsten freien ObjInd holen
  IE_objInd = DB_QueryNxtFree (IE_cad_typ, istart);
    // printf("IIIIIIIStart=%d, next=%ld typ=%d\n",istart,IE_objInd,IE_cad_typ);
  if(IE_objInd < 1) return 0;


  // den Header schreiben
  L_raus:
  // create name of new outpt-obj
  APED_oid_dbo__ (IE_outTxt, IE_cad_typ, IE_objInd);
  // gtk_label_set_text ((GtkLabel*) (IE_txtHdr), IE_outTxt);

  // display name of new outpt-obj in field IE_entHdr
  IE_set_txtHdr ();



  return 0;

}


/*
//=====================================================================
  int IE_cad_RotS (void *parent, void *data) {
//=====================================================================

  // printf("IE_cad_RotS  /%s/\n",(char*)data);

  IE_sel_CB_1 (Typ_modCWCCW, 0L, (char*)data);

  return 0;
}
*/


//=====================================================================
  int IE_cad_Side (void *parent, void *data) {
//=====================================================================

  // printf("IE_cad_Side  /%s/\n",(char*)data);

  IE_sel_CB_1 (Typ_mod1, 0L, (char*)data);

  return 0;
}



/*
//=====================================================================
  int IE_cad_Side1 (void *parent, void *data) {
//=====================================================================

  // printf("IE_cad_Side1  /%s/\n",(char*)data);

  IE_sel_CB_1 (Typ_modInOut, 0L, (char*)data);

  return 0;
}
*/


//================================================================
  int IE_cad_CB_Ok (MemObj *mo, void **data) {
//================================================================
// user pressed CAD-Cancel-button

  return IE_cad_OK ();

}


//================================================================
  int IE_cad_CB_Clear (MemObj *mo, void **data) {
//================================================================
// user pressed CAD-Cancel-button

  return IE_cad_Inp_undo ();

}


//================================================================
  int IE_cad_CB_Cancel (MemObj *mo, void **data) {
//================================================================
// user pressed CAD-Cancel-button

  return IE_cad_Inp_cancel ();

}
 

//=====================================================================
  int IE_cad_INS (char *cmd) {
//=====================================================================
// "Exit":  exit MODIFY; activate ADD.

// Cancel-function &
// insert line after current line
//  IE_modify: 0=Add 1=Modify 2=Insert



  long     l1;


  // printf("iiiiiiiiiiiii IE_cad_INS |%s| IE_modify=%d\n",cmd,IE_modify);


/* nur f Checkbutt
  if (GTK_TOGGLE_BUTTON (parent)->active) {    // parent statt w1 geht auch !
    printf("IE_cad_INS activate\n");
    goto L_activate;
  }
*/



  //================================================================
  // Cancel-Button
  // exit MODIFY; activate ADD.
  if(!strcmp (cmd,"Exit")) {


    //----------------------------------------------------------------
    // no menuFunction: do nothing ..
    if(IE_cad_typ < 0) return 0;


    //----------------------------------------------------------------
    // CREATE: like escape:
    if(IE_modify == 0) {
      IE_cad_Inp_undo ();  // clear inputfield or exit menu
      return 0;
    }

    //----------------------------------------------------------------
    // MODIFY
    // auf add umschalten; CurPos ans dateiende, dann activate
    // printf("IE_cad_INS Exit IE_EdFnc=%d\n",IE_EdFnc);

    // kill TextEditWindow if active ..
    if(IE_EdFnc == 2) {
      // kill win
      // if(IE_ed1_win.stat != 0) {
      if(GUI_OBJ_IS_VALID(&IE_edWin1)) {
        // gtk_signal_emit_by_name (GTK_OBJECT(IE_ed1_win.win),"destroy");
        // IE_ed2__ (NULL, PTR_INT(UI_FuncKill));   // 2011-04-14
        IE_ed2__ (NULL, GUI_SETDAT_EL(TYP_EventPress,UI_FuncKill));
      }
      GUI_update__ ();   // warten bis Window weg ist ..
      IE_EdFnc = 0;
    }



    // eine Zeile zurueck (ev aktive Zeile testweise modifiziert ...)
    l1 = ED_get_lnr_act ();
    if(l1 > 0) {
      --l1;
      ED_set_lnr_act (l1);
    }


    WC_set_obj_stat (0);  // perm

    ED_work_CurSet (999999);


    // reset active cad-function
    IE_cad_init2 (-1);
    sele_set__ (Typ_goGeom);   // make all objs selectable ..


    // // restore S/M/DEL (make selectable)
    // UI_AP (UI_FuncEnable, UID_ckb_search, (void*)TRUE);


    // gtk_widget_set_sensitive (IE_box_INS, FALSE);

    // reset to ADD
    IE_cad_reset_modif ();          // 2011-07-19


    // UI_cb_search (1);  // reactivate SM
    // DL_Redraw ();

    IE_activate ();

    return 0;
  }







  //---------------------------------------------
  // Insert


  IE_modify = 2;    // 0=Add 1=Modify 2=Insert

  // gtk_label_set_text ((GtkLabel*) (IE_txtMode), "INSERT  ");
  GUI_label_htm_mod (&IE_txtMode, "INSERT  ");


  // Modify OFF
  // IE_set_modify (OFF);
  // IE_modify = OFF;


  // den Header ausgeben
  IE_nxtInd ();



  // den Focus ans CAD-Window
  // UI_AP (UI_FuncFocus, UID_WinCAD, NULL);
  IE_inp_chg (0);


  // Felder loeschen
  IE_cad_ClearInputs (INPRECANZ);


  // Insert Line
  // UI_AP (UI_FuncInsert, UID_Edit_Line, NULL);


  return 0;

}


/*
//=====================================================================
  int IE_cad_DEL (void *parent, void *data) {
//=====================================================================
// delete selected Line


  // delete selected Line
  UI_AP (UI_FuncKill, UID_Edit_Line, NULL);

  // da der Cursor eine Zeile zu weit oben: 1 Zeile runter
  // UI_AP (UI_FuncSet, UID_Edit_LnNxt, NULL);

  // following line bearb.
  IE_activate ();

  return 0;

}


//================================================================
  int IE_cad_Tab () {
//================================================================
// Tab-key pressed

  // test if last inputfield; jes: stop event (simulate focus-out)

  if(IE_cad_ck_inpLast ()) {
        IE_cad_Inp2_CB (NULL, NULL);


  return 0;

}
*/


//=====================================================================
  int IE_cad_OK () {
//=====================================================================
/* OK-Butt. pressed. If OK is locked: activate next input-field
  clear Inputs (nicht Infos)
  Zeile in die Datei abspeichern
    ChangeMode: ?
    InsertMode: ?
  Inputs:
    IE_stat_OK         0=OK-is-locked; 1=not
*/

  int   irc, i1, ieAct;
  long  lNr, dli;
  char  *p1, *p2;


  // printf("\nOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO \n");
  // printf("IE_cad_OK |%s| mod=%d del=%d\n",IE_outTxt,IE_modify,IE_delete);
  // printf(" cad_OK-IE_inpInd=%d\n",IE_inpInd);


  // If OK is locked: activate next input-field
  if(!IE_stat_OK) return IE_inp_chg (-1);

  GL_temp_del_all (); // alle temp. obj loeschen ..


  WC_set_obj_stat (0);  // perm

  // MODIFY: overwrite existing DL-object
  // if(IE_modify == 1) {  //1=Modify
    // dli = DL_dli__dbo (3, 21L, -1L);
    // DL_SetInd (dli);
    // delete last DL-record

    // dli = DL_get__  (NULL);
    // --dli;
    // GL_Delete (dli);

    // GL_last_del
  // }


  // display obj permanent; create code in IE_outTxt
  IE_save = 1;     // save; 0=preview, 1=save perm.

  ieAct = IE_inpInd;       // save for rest color (modif. in IE_cad_test__)

  irc = IE_cad_test__ ();  // create src in IE_outTxt
    // printf(" _test1-irc=%d\n",irc);


  // clear all temporary inputfields
  GL_temp_del_all ();


  // clear group
  Grp_Clear (1);


  IE_save = 0;     // reset; 0=preview, 1=save perm.
  if (irc < 0) return 0;


  // reset color of text of active inputField
  if(ieAct > 0) IE_info_col_set (0, ieAct); 


  // OK-Button sperren
  IE_cad_set_OK (0);      // 0=OK-locked; 1=OK-active


  // add the objectname IE_entNam -> IE_outTxt
  p1 = GUI_entry_get (&IE_entNam);
  if(strlen(p1) > 0) {
    strcat(IE_outTxt, " #");
    strcat(IE_outTxt, p1);
  }



  //================================================================
  if(IE_modify == 0) {  // 0=Add

    // add sourcecode into data-space (memory)
      // printf(" IE_cad_OK add|%s|\n",IE_outTxt);

    // save line in memory, Undo-List
      // printf(" cad_OK-ED_srcLn_add |%s|\n",IE_outTxt);
    ED_srcLn_add (IE_outTxt, 1);


    // create subModel: create import-joint-objects.
    if(IE_cad_typ == Typ_Model) {
      IE_joints_import ();
    }


    // reset hilite of previous selected objects ..  2011-10-14
    DL_hili_off (-1L);         //  -1 = unhilite all


    // clear all inputfields, goto 1. entry
    IE_cad_ResetInputs ();

    // get next free objHeader for active function and display,
    IE_nxtInd ();
/*
    //   but not for menuGroup Modify
    if(menGrpInd == IE_Func_Modify) {   // CUT INT TRA ..
      IE_cad_typ = Typ_goGeom; // undefined output-obj
      IE_objInd  = 0;
    } else {
      IE_nxtInd ();
    }
*/

    // feld 1 aktivieren
    // IE_inp_chg (0);



  //================================================================
  } else if(IE_modify == 1) {  //1=Modify

    lNr = ED_get_lnr_act ();
    i1 = APED_src_chg (lNr, IE_outTxt);
    if(i1 == 0) {
      ED_inc_lnr  ();  // APED_src_chg creates a additional line !
      // ED_set_lnr_act (ED_get_lnr_act ()+1); // use with IE_activate
    }

    // // IE_outTxt mit aktueller Zeile vertauschen ..
    // UTF_chg_line (ED_get_lnr_act (), IE_out // use with IE_activateTxt);
      // // UTF_dump__();

/*
    // activate next line ...
    ED_inc_lnr  ();
    IE_activate ();
*/

    IE_cad_INS ("Exit"); // exit MODIFY; activate ADD. 2011-07-19



  //================================================================
  } else if(IE_modify == 2) {  // 2=Insert
    TX_Error("IE_cad_OK E002");

  }



  WC_set_obj_stat (1); // alle Displays mit WC_Work__ ab nun nur temporaer ..


  return 0;

}


/*
//=====================================================================
  int IE_cad_OK (void *parent, void *data) {
//=====================================================================
// OK-Butt. pressed
//  clear Inputs (nicht Infos)
//  Zeile in die Datei abspeichern
//    ChangeMode: ?
//    InsertMode: ?


  int   i1, ilen, lNr;
  long  ipos;


  printf("IE_cad_OK |%s| mod=%d del=%d\n",IE_outTxt,IE_modify,IE_delete);


  GL_temp_del_all (); // alle temp. obj loeschen ..



  WC_set_obj_stat (0);


  if(IE_delete == 0) {
    // normaler Anlauf:
    i1 = IE_cad_test__ ();
    if (i1 < 0) return 0;



  } else {
    // delete:
    lNr = ED_Get_LineNr ();
    WC_Work__ (lNr, IE_outTxt);
  }



  ++IE_ed_lNr;

  // wenn GenerateNewLine: den neuen ObjIndex holen.
  // IE_nxtInd ();



  // add CrLf zur Ausgabezeile (ausser bei Modify ?)
  if(IE_modify != 2) strcat(IE_outTxt, term_buf);  // add EOL



  if(IE_modify == 1) {        // 0=Add 1=Modify 2=Insert

    // Modify: remove selected
    // Absturz, wenn nix selektiert !!
    UI_AP (UI_FuncKill, UID_Edit_Line, NULL);
    printf("done...\n");

    // da der Cursor eine zeile zu weit oben: 1 Zeile runter
    // UI_AP (UI_FuncSet, UID_Edit_LnNxt, NULL);

    // ab nun mit ADD weiter
    // IE_set_modify (OFF);



  } else if(IE_modify == 2) {        // 0=Add 1=Modify 2=Insert


    // Insert (blank) Line
    UI_AP (UI_FuncInsert, UID_Edit_Line, NULL);

  }



  // CurPos holen
  UI_AP (UI_FuncGet, UID_Edit_Cpos, (void*)&ipos);

  // Text an der aktuellen curpos einfuegen, cPos -> next Line.
  UI_AP (UI_FuncSet, UID_Edit_Line, (void*)IE_outTxt);

  // Udate (Editfenster(gesamten APT-Text) > Memory kopieren)
  ED_unload__ ();


  ilen = strlen(IE_outTxt) - term_anz;


  // add Entry in Undo-List
  UI_undo_add2 (IE_outTxt);

  // Ja; OK-Button sperren
  IE_stat_OK = 0;
  gtk_widget_set_sensitive(IE_but_OK,(gint)IE_stat_OK);//1=activ,0=inaktiv.

  WC_set_obj_stat (1); // alle Displays mit WC_Work__ nur temporaer ..



  if(IE_modify == 1) {        // 0=Add 1=Modify 2=Insert
    // naechste Zeile im Modify-Mode analysieren
    IE_activate ();

  } else {        // 0=Add 1=Modify 2=Insert

    // alle Inputfelder loeschen
    IE_cad_ClearInputs ();

    // den naechsten freien Objheader feststellen und anzeigen
    IE_nxtInd ();
  }

  if(IE_modify == 2) {        // 0=Add 1=Modify 2=Insert
    // Nach Insert neu erzeugte Zeile selektiert darstellen
    UI_Ed_sel (ipos, ipos+ilen);
  }


  return 0;

}
*/

//=====================================================================
  int IE_cad_InpIn_CR () {
//=====================================================================
/*  CR aus irgendeinem anderen Win ..

*/

  IE_cad_InpIn_CB (IE_inpInd, GUI_KeyReturn);

  return 0;

}


//================================================================
  int IE_get_inp_dli () {
//================================================================
// IE_get_inp_dli       get dli for active inputField
// returns 0 if no CAD-inputfield is active
// else -2 to -7

  // printf("IE_get_inp_dli IE_inpInd=%d\n",IE_inpInd);
  // printf("  APP_stat=%d UI_InpMode=%d\n",AP_stat.APP_stat,UI_InpMode);
  

  if(AP_stat.APP_stat) return 0;    // app/plugin/prc is active;

  if(UI_InpMode != UI_MODE_CAD) return 0;    // cad not active

  return (IE_inpInd - 2);

}

 
//================================================================
  int IE_cad_Inp_edit_field () {
//================================================================
// das aktuelle Inputfeld mit editor

  // char   cbuf[2056];
  char   fnam[256], *p1;


  printf("IE_cad_InpIn_String %d\n",IE_inpInd);

  sprintf(fnam,"%sselection.txt",OS_get_tmp_dir());

  // das aktive Feld --> datei ausgeben
  // printf(" _inpTxtG|%s|\n",IE_inpTxtG[IE_inpInd]);
  // if(UTX_wrf_str(fnam, IE_inpTxtG[IE_inpInd]) < 0) return -1;
  p1 = IE_get_inp__ (IE_inpInd);
  if(UTX_wrf_str(fnam, p1) < 0) return -1;

  // modify File m. Editor
  TX_Print("Edit field %d spawned ..",IE_inpInd);
  APP_edit (fnam, 0);
  // printf(" NNNNNNNNNNNNNNNNNNNach APP_edit\n");
  TX_Print("Edit done ..");


  // load file back -->
  p1 = memspc101;
  if(UTX_fgetLine(p1, sizeof(memspc101), fnam, 1) < 0) return -1;
  UTX_CleanCR(p1);   // remove LF ..


  // ins aktive Feld ausgeben ..
  // IE_cad_ClearInpAct ();
  // IE_sel_CB_1 (Typ_Txt, 0L, p1);
  GUI_entry_set (&IE_wCad_obj[IE_inpInd], p1);


  return 0;

}


//================================================================
  int IE_cad_InpIn_oid (char *cbuf) {
//================================================================
// from keyIn objID

  int   typ;
  long  dbi;


  APED_dbo_oid (&typ, &dbi, cbuf);

  IE_sel_CB_1 (typ, dbi, cbuf);

  return 0;

}

 
//================================================================
  int IE_cad_InpIn_String (char *cbuf) {
//================================================================
// cbuf in das naechste freie inputfeld eintragen ...

  // printf("IE_cad_InpIn_String |%s| %ld\n",cbuf,strlen(cbuf));

  IE_sel_CB_1 (Typ_Txt, 0L, cbuf);

  return 0;

}


//=====================================================================
  int IE_cad_InpIn_CB (int ind, int key) {
//=====================================================================
// Key in in ein Inputfeld
// ind unused



  char *gtxt;
  char   *p1;
  int    i1;


  // printf("IE_cad_InpIn_CB %d %d\n",ind,key);


/*
  // gtxt = gtk_entry_get_text ((GtkEntry*) (IE_wCad_obj[ind]));
  gtxt = GUI_Entry_get (IE_wCad_obj[ind]);
  // printf("IE_cad_InpIn_CB %d |%s| key=%d\n",ind,gtxt,key);

  p1 = UTX_pos_1n(gtxt);  // skip lead. blanks
  // save input
  strcpy(IE_inpTxtG[ind], p1);
*/


  // mit CR: ausgeben.
  if(key == GUI_KeyReturn) {
      // printf(" -ret; _stat_OK=%d\n",IE_stat_OK);
    if(IE_stat_OK != 0) {  // wenn OK aktiv ist, do OK
      IE_cad_OK ();

    } else {
      // not yet OK; do Tab
      // IE_cad_test__ ();
      IE_inp_chg (-1);
/*
      // ein TAB implizieren
      i1 = ind + 1;
        // printf(" IE_cad_InpIn_CB i1=%d IE_inpAnz=%d\n",i1,IE_inpAnz);
      if(i1 >= IE_inpAnz) {
        DL_Redraw ();  // after selection ConstrPlane is unvis ..
        return 0;      // skip M3 in last field ..
      }
      IE_inp_chg (i1);
      IE_cad_InpIn__ (i1);
*/
    }

  } else {  // do Tab
      // printf(" -tab;\n");
    // test ob alle Eingaben vorh ..
    IE_cad_test__ ();
  }




  return 0;

}


//=====================================================================
  int IE_cad_help (MemObj *mo, void **data) {
//=====================================================================

  int    i1;
  char   *p1, sf[40], sl[32];
  char   s_mod[]="Modify",
         // s_analyz[]="Analyz",
         s_base[]="Base",
         s_txg[]="TxtG",
         s_ase[]="Ace";


  printf("IE_cad_help |%s|\n",GUI_DATA_S1);
  // printf("     _cad_typ=%d IE_FuncTyp=%d IE_FuncSubTyp=%d\n",
            // IE_cad_typ,IE_FuncTyp,IE_FuncSubTyp);

  // if(IE_cad_act) printf(" |%s|\n",IE_cad_act[IE_first].info);


  //================================================================
  if(!strcmp (GUI_DATA_S1, "CAD")) {
    sprintf(sf, "CAD_using");   //,IE_FuncSubTyp);
    sl[0] ='\0';
    goto L_disp__;
  }


  //================================================================
  // aktives haupt/submenu
  // ist IE_cad_typ ein ObjektTyp ?
  if(IE_cad_typ < 0) {                 // keine Funktion vorgewaehlt ..
    p1 = s_base;
    goto L_go;
  }



  //================================================================
  // help-Inputfield
  if(strcmp (GUI_DATA_S1, "Input")) goto L_func;

  i1 = IE_inpTypAct;                  // typNr of Input
    sprintf(sf, "CAD_input");
    sprintf(sl, "#_%s_",AP_src_typ__(i1));

    goto L_disp__;



  //================================================================
  // help-Function
  L_func:

  if(IE_cad_typ > 0) {                   // AP_typChar_typ 
    p1 = AP_src_typ__ (IE_cad_typ);
  }

  if(IE_FuncTyp == IE_Func_Modify) {          // CUT, INT ..
    p1 = s_mod;
  } else if(IE_FuncTyp == IE_Func_Note) {     // Notes
    p1 = s_txg;
  // } else if(IE_FuncTyp == IE_Func_Analyze) {  // ANALYZ
    // p1 = s_analyz;
  } else if(IE_FuncTyp == IE_Func_CADEnv) {   // ActiveCADEnv
    p1 = s_ase;
  }


  //----------------------------------------------------------------
  L_go:
  if(IE_cad_typ >= 0) {
    sprintf(sf, "CAD_%s",p1);
    sprintf(sl, "#F%d",IE_FuncSubTyp);

  } else {
    sprintf(sf, "CAD_using");   //,IE_FuncSubTyp);
    sl[0] ='\0';
  }


  //----------------------------------------------------------------
  L_disp__:
  APP_Help (sf, sl);


  return 0;

}


/*
//=====================================================================
  int IE_cad_AptInd_CB (void *parent, GdkEventAny *event) {
//=====================================================================


  // printf("IE_cad_AptInd_CB\n");

  return 0;

}
*/

//=====================================================================
  int IE_cad_Inp1_Info (int iind) {
//=====================================================================

/*
  int   i1, i2;
typedef  struct {int typ; char *txt;} IE_info_rec;
static IE_info_rec IE_info_tab[] = {
 Typ_Val,      "Key Val / PgUp / PgDwn / Sel LN (Len.) / Sel Cir (Radius)",
 Typ_PT,       "Sel PT / Indicate PT",
 Typ_LN,       "Sel LN",
 Typ_CI,       "Sel CIR",
 Typ_VC,       "Vector: Key PgUp/Dwn / D[I]X/Y/Z / dx dy [dz] /"
               " ANG(ax) [ANG(az)]; Sel LN / Cir / PLN / PT+PT; \"REV\"",
 Typ_CV,       "Sel CRV",
 Typ_CVTRM,    "Sel CCV",
 Typ_CVPOL,    "Sel Polygon",
 Typ_XVal,     "Key X-Val; Sel PT",
 Typ_YVal,     "Key Y-Val; Sel PT",
 Typ_ZVal,     "Key Z-Val; Sel PT",
 Typ_Angle,    "Angle: Key Val(Deg.); Sel LN / Vec / PT-PT; Key PgUp/Dwn",
 Typ_PLN,      "Sel PLN; Key PgDwn (ConstrPln)",
 Typ_SOL,      "Sel Conus/Torus",
 Typ_goPrim,   "Sel PT/LN/CIR",
 Typ_go_LCS,    "Sel LN/Cir/CRV",
 Typ_goGeom,   "Sel Object",
 Typ_goGeo1,   "Sel LN/Cir/CRV/PLN",
 Typ_goGeo2,   "Sel Circ or Ellipse",
 Typ_goGeo3,   UNUSED
 Typ_goGeo4,   "Sel VC / LN / PLN",
 Typ_goGeo5,   "Sel CIR/ELL/CCV",
 Typ_goGeo7,   "Key Val(Angle-Degree); Sel PT",
 Typ_goGeo8,   "Key Val; Sel PT",
 Typ_goAxis,   "Sel LN / Cir / PLN / PT+PT / PT+LN / PT+VC (PgUp) / PT+PLN",
 Typ_goRadius, "Key Radius; Sel PT / Cir",
 Typ_goGeoSUSU,"Sel Conus Torus RuledSurf RevolvedSurf B-Spl-Surf",
 Typ_Tra,      "Key PgUp/Dwn (select Transformation)",
 Typ_mod2,     "Key PgUp/Dwn",
 Typ_mod1, "Key PgUp/Dwn (selects alternate versions)",
 Typ_modCWCCW, "ReversButton; PgUp/Dwn; Key \"CW\" (clockwise)",
 Typ_modCTRL,  "Key PgUp / PgDwn  (\"CTRL\" = Controlpoints)",
 // Typ_modInOut, "Sel IN / OUT",
 Typ_modREV,   "Key PgUp / PgDwn  (\"REV\" = reverse)",
 Typ_modCX,    "Key PgUp / PgDwn  (\"CX\" = across)",
 Typ_String,   "Key Text / Modify-Edit",
 TYP_FilNam,   "Key Filename / middle Mousebutt.(F5=FileList) / sel.Model",
 Typ_CtlgPart, "Key  Catalog/Part / middle Mousebutt.(F5=FileList) / sel.CatalogPart",
 Typ_SubModel, "Key Modelname / middle Mousebutt.(F5) / sel.Model",
 Typ_Str_Dir1, "Key X / Y / Z / P / PgUp/Dwn",
 Typ_Val_symTyp, "Key 0 (Star) 1 (Tria.) 2 (Circ) 3 (Rec.)"
                 " 4 (VecNorm) 5 (Vec) / PgUp/Dwn",
 Typ_apDat,    "Key application-specific data",
 Typ_Group,    "Sel obj's to add to group; exit with Tab-key",
 -1,           ""};
*/


  int   typ;


  // printf("IE_cad_Inp1_Info feld %d Typ=%d\n",IE_inpInd,IE_inpTypR[IE_inpInd]);


  if(iind < 0) {
    typ = IE_inpTypR[IE_inpInd];
  } else {
    typ = IE_inpTypR[iind];
  }


  if(typ == 0) return 0;

  IE_cad_Inp2_Info (typ);
/*
  for(i1=0; i1<1000; ++i1) {
    i2 = IE_info_tab[i1].typ;
    if(i2 < 0) {
      TX_Print("IE_cad_Inp1_Info typ %d unbekannt",typ);
      return 0;
    }

    if(i2 == typ) {
      TX_Print(IE_info_tab[i1].txt);
      break;
    }
  }
*/

  return 0;

}


//================================================================
  int IE_cad_Inp2_Info (int typ) {
//================================================================
// see INF_MSG_new
// modify, delete, add keywords/message: see INF_MSG_new
 
  int      i1, i2;
  typedef  struct {int typ; char *txt;} IE_info_rec;
  static   IE_info_rec IE_info_tab[] = {
  Typ_Val,      "Typ_Val",
  Typ_PT,       "Typ_PT",
  Typ_LN,       "Typ_LN",
  Typ_CI,       "Typ_CI",
  Typ_VC,       "Typ_VC",
  Typ_CV,       "Typ_CV",
  Typ_CVTRM,    "Typ_CVTRM",
  Typ_CVPOL,    "Typ_CVPOL",
  Typ_XVal,     "Typ_XVal",
  Typ_YVal,     "Typ_YVal",
  Typ_ZVal,     "Typ_ZVal",
  Typ_Angle,    "Typ_Angle",
  Typ_PLN,      "Typ_PLN",
  Typ_SOL,      "Typ_SOL",
  Typ_Tra,      "Typ_Tra",
  Typ_modRepl,  "Typ_modRepl",
  Typ_mod1,     "Typ_mod1",
  Typ_mod2,     "Typ_mod2",
  Typ_modCWCCW, "Typ_modCWCCW",
  Typ_modREV,   "Typ_modREV",
  Typ_modCX,    "Typ_modCX",
  Typ_modCTRL,  "Typ_modCTRL",
  Typ_modPERP,  "Typ_modPERP",
  Typ_modPARL,  "Typ_modPARL",
  Typ_modUnlim, "Typ_modUnlim",
  Typ_modAux,   "Typ_mod1",                    // Bild-Tasten/ (Version ändern)
  Typ_Txt,      "Typ_Txt",
  Typ_String,   "Typ_String",
  TYP_FilNam,   "TYP_FilNam",
  Typ_CtlgPart, "Typ_CtlgPart",
  Typ_Model,    "Typ_Model",
  Typ_SubModel, "Typ_SubModel",
  // Typ_Str_Dir1, "Typ_Str_Dir1",              // nur CAD
  // Typ_Val_symTyp, "Typ_Val_symTyp",          // nur CAD
  Typ_Joint,    "Typ_Joint",
  // Typ_apDat,    "Typ_apDat",
  Typ_Group,    "Typ_Group",

  Typ_PTS,      "Typ_PTS",
  Typ_PTI,      "Typ_PTI",
  Typ_SEG,      "Typ_SEG",

  Typ_goPrim,   "Typ_goPrim",
  Typ_go_LCS,   "Typ_lFig",
  Typ_goGeom,   "Typ_goGeom",
  Typ_goGeo1,   "Typ_goGeo1",
  Typ_goGeo2,   "Typ_goGeo2",
  Typ_go_PD,    "Typ_go_PD",
  // Typ_goGeo4,   "Typ_goGeo4",
  Typ_goGeo5,   "Typ_goGeo5",
  Typ_goGeo7,   "Typ_goGeo7",
  Typ_goGeo8,   "Typ_goGeo8",
  Typ_goAxis,   "Typ_goAxis",
  Typ_goGeoSUSU,"Typ_goGeoSUSU",
  Typ_go_RA,    "Typ_go_RA",
  Typ_go_lf1,   "Typ_go_lf1",

  -1,           ""};               // end


  // printf("IE_cad_Inp2_Info %d\n",typ);


  for(i1=0; i1<1000; ++i1) {
    i2 = IE_info_tab[i1].typ;
    if(i2 < 0) {  // -1 = end of tab
      TX_Print("IE_cad_Inp1_Info typ %d unbekannt",typ);
      return 0;
    }

    if(i2 == typ) {
      // TX_Print (IE_info_tab[i1].txt);
        // printf(" cad_Inp2_Info |%s|\n",IE_info_tab[i1].txt);
      MSG_pri_0 (IE_info_tab[i1].txt);
      break;
    }
  }


  return 0;

}


//=====================================================================
  int IE_cad_Inp1_nxtTra (int mode) {
//=====================================================================
// soll auch IE_cad_Inp1_nxtVec includieren ...

// mode=-1=Page_Up
// mode=1=Page_Down

  long  ii, dbi;
  char  *p1, *p2, cbuf[32];


  // printf("<<<<<<<<<<<<<<\nIE_cad_Inp1_nxtTra %d %d\n",mode,IE_inpInd);

  // index auslesen
  p1 = IE_get_inp__(IE_inpInd);


  if(p1[0] == 'T') {
    ++p1;
    dbi = strtol (p1, &p2, 10);
    if(p2 != p1) goto L_1;
  }

  dbi = DB_dbo_get_free (Typ_Tra);


  L_1:
    // printf(" dbi=%ld\n",dbi);
  if(mode > 0) {
    ii = DB_QueryNxtUsed (Typ_Tra, dbi + 1);
  } else {
    ii = DB_QueryPrvUsed (Typ_Tra, dbi - 1);
  }
  if(ii > 0)  dbi = ii;


  sprintf(cbuf, "T%ld", dbi);
  IE_set_inp (IE_inpInd, cbuf);

  // // disp; koennte auch in IE_parDef/save erfolgen wie bei IE_cad_Inp1_nxtVec
  // UI_disp_vec2 (ii);

  IE_cad_test__ ();

  return 0;

}




//=====================================================================
  int IE_cad_Inp1_nxtVec (long *dbi, int mode) {
//=====================================================================
// IE_cad_Inp1_nxtVec              get & preview next|previous vector
// Input:
//   mode   1=next; -1=previous
//
// see also IE_cad_Inp1_nxtVal


  // long  ii;
  char  cbuf[32];


  printf("IE_cad_Inp1_nxtVec %d\n",mode);


  *dbi = AP_get_nxtVec (mode);      // get next DB-index
    printf(" _nxtVec %ld\n",*dbi);

  if(!*dbi) {
    // TX_Print("**** no vectors defined ..");
    MSG_pri_0("VC0");
    return -1;
  }


  // // display vec in VectorSelector
  // GLBT_vcSel_set (dbi, 0);


  // add caux into active cad-Inputfield and do not proceed to next field
  // IE_sel_CB_1 (Typ_VC, dbi, caux);


  // for PgUp/Dwn
  sprintf(cbuf, "D%ld",*dbi);
  GUI_entry_set (&IE_wCad_obj[IE_inpInd], cbuf);
  IE_cad_test__ ();



/*
// for GL2D-icon:
  // clear active inputfield
  IE_set_inp (IE_inpInd, "");

  // do NOT proceed to next inputfield
  IE_inp_chg (-3);
*/

  return 0;

}


//================================================================
  int IE_cad_Inp1_nxtSM (int mode) {
//================================================================
// get next internal submodel into input

  static int i1 = 1;
  char   *p1, s1[256], s2[128];


  printf("IE_cad_Inp1_nxtSM %d %d\n",mode,i1);


  p1 = IE_get_inp__(IE_inpInd);
  // printf(" inp=|%s| %d\n",p1,strlen(p1));

  L_noAmoi:
  sprintf(s1,"%sMod.lst",OS_get_tmp_dir());
  if(UTX_fgetLine (s2, 126, s1, i1) < 0) {
    --i1;
    goto L_noAmoi;
  }
  i1 += mode;
  if(i1 < 1) i1 = 1;

  IE_set_inp (IE_inpInd, s2);

  IE_cad_test__ ();

  return 0;

}

 
//=====================================================================
  int IE_cad_Inp1_nxtREV (int mode, char *wGrp) {
//=====================================================================
// abwechselnd blank bzw <txt1> ausgeben.
// see also IE_cad_Inp1_Aux
// mode UNUSED

  int    ii;
  char   *p1, s1[80];


  // printf("IE_cad_Inp1_nxtREV |%s|\n",wGrp);

  // get content of textfield
  p1 = IE_get_inp__(IE_inpInd);
    // printf(" inp=|%s| l=%d\n",p1,strlen(p1));

  // input empty: get 1. word
  if(strlen(p1) < 1) {
    ii = 0;
    goto L_copy;
  }


  // find pos of p1 in wGrp
  ii = UTX_wGrp_find (wGrp, p1);


  // find position of next word
  ii = UTX_wGrp_nxt (wGrp, ii);
    printf(" ii=%d\n",ii);


  // copy word <ii> of txTab -> s1
  L_copy:
  if(ii < 0) {
    s1[0] = '\0';
  } else {
    UTX_cp_word_term (s1, &wGrp[ii], '|');
      // printf(" s1=|%s|\n",s1);
  }

  // gtk_entry_set_text ((GtkEntry*)(IE_wCad_obj[IE_inpInd]), cbuf);
  L_work:
  IE_set_inp (IE_inpInd, s1);

  IE_cad_test__ ();

  return 0;

}


//================================================================
  int IE_cad_Inp1_nxtFDX () {
//================================================================
// "PARL"|"CX"|""

  int  ii;
  char *p1, *ta[]={"PARL","CX", ""};


  // get content of textfield
  p1 = IE_get_inp__(IE_inpInd);
    // printf(" inp=|%s| l=%d\n",p1,strlen(p1));


  ii = strlen(p1);
  if(ii < 1) { ii = 0; goto L_work; }
  if(ii > 3) { ii = 1; goto L_work; }
  ii = 2;


  // gtk_entry_set_text ((GtkEntry*)(IE_wCad_obj[IE_inpInd]), cbuf);
  L_work:
  IE_set_inp (IE_inpInd, ta[ii]);
  IE_cad_test__ ();

  return 0;

}

 
//================================================================
  int IE_cad_Inp1_iNr (int mode, int iMax) {
//================================================================
// eine Zahl zw 0 und  iMax
// mode: -1 = Up; 1 = Dwn.
// see also IE_cad_Inp1_nxtMod

  int    i1;
  char   *p1, cbuf[32];


  printf("IE_cad_Inp1_iNr %d %d\n",mode,iMax);


  p1 = IE_get_inp__(IE_inpInd);
    printf(" inp=|%s| %d\n",p1,(int)strlen(p1));

  if(strlen(p1) < 1) {
    i1 = 0;

  } else {
    i1 = atoi(IE_get_inp__(IE_inpInd));
    i1 += mode;
    if(i1 < 0) {
      i1 = 0;
      OS_beep();
    } else if (i1 > iMax) {
      i1 = iMax;
      OS_beep();
    }
  }


  sprintf(cbuf, "%d",i1);
  // gtk_entry_set_text ((GtkEntry*)(IE_wCad_obj[IE_inpInd]), cbuf);
  IE_set_inp (IE_inpInd, cbuf);

  IE_cad_test__ ();

  return 0;

}


//================================================================
  int IE_cad_Inp1_DirS (int mode) {
//================================================================

  char   *p1, cbuf[64];


  printf("IE_cad_Inp1_DirS %d\n",mode);


  p1 = IE_get_inp__(IE_inpInd);
  printf(" inp=|%s| %d\n",p1,(int)strlen(p1));

  strcpy(cbuf, p1);

  if(cbuf[0] == 'P') cbuf[0] = 'X';
  else if(cbuf[0] == 'X') cbuf[0] = 'Y';
  else if(cbuf[0] == 'Y') cbuf[0] = 'Z';
  else if(cbuf[0] == 'Z') cbuf[0] = 'P';


  // gtk_entry_set_text ((GtkEntry*)(IE_wCad_obj[IE_inpInd]), cbuf);
  IE_set_inp (IE_inpInd, cbuf);

  IE_cad_test__ ();

  return 0;

}


//================================================================
  int IE_cad_Inp1_PLN (int mode) {
//================================================================
// PageUp in a TypPLN_field ..

  // IE_popup_planes__ ();   // 2013-04-03
// removed 2013-04-03 - comes after CurDwn in Typ_PLN after clicked into it

/*
  char   cbuf[64];

  // aktuelle ConstrPln holen ...
  strcpy(cbuf, AP_Get_ConstPl_Z(0));

  gtk_entry_set_text ((GtkEntry*)(IE_wCad_obj[IE_inpInd]), cbuf);

  IE_cad_test__ ();
*/

  return 0;

}


/*
//=====================================================================
  int IE_cad_Inp1_nxtCW (int mode) {
//=====================================================================


  char   *p1, cbuf[32];


  printf("IE_cad_Inp1_nxtCW %d\n",mode);

  p1 = IE_get_inp__(IE_inpInd);
  // printf(" inp=|%s| %d\n",p1,strlen(p1));


  if(strlen(p1) < 1) {
    strcpy(cbuf, "CW");

  } else {
    strcpy(cbuf, "");
  }


  gtk_entry_set_text ((GtkEntry*)(IE_wCad_obj[IE_inpInd]), cbuf);

  IE_cad_test__ ();

  return 0;

}
*/


//=====================================================================
  int IE_cad_Inp1_nxtAng (int mode) {
//=====================================================================
// Zeile ist ca "90" od "L20" od "L20 ANG(90).
// Wenn "L20": add " MOD(1)"; else incr od decr last Nr.


  int    i1, oTyp, oInd, maxVal;
  char   *p1, *p2, cbuf[256];
  double d1, d2, aInc = 90.;

  printf("IE_cad_Inp1_nxtAng %d\n",mode);


  p1 = IE_get_inp__(IE_inpInd);
  UTX_pos_skipLeadBlk (p1);
    printf(" inp=|%s| %d\n",p1,(int)strlen(p1));


  // empty: start with "90"
  if(strlen(p1) < 1) {
    strcpy(cbuf, "90");
    d1 = 90.;
    goto L_fertig;
  }


  // copy field => cbuf
  strcpy(cbuf, p1);


  // check if input starts with "ANG"
  if(UTX_ck_casenChr (cbuf, "ANG", 3) == 0) goto L_3;


  // is input numer ?  ("90" or "L20")
  // if(isdigit(*p1)) goto L_2;   // problem if it is '-'
  if(UTX_ck_num_f (&p2, cbuf) == 0) goto L_2;

  
  // object, eg "L20"
  strcat(cbuf, " ANG(90)");
  goto L_fertig;


  //================================================================
  // decode & incr direct value eg "30"
  L_2:
  d1 = atof(cbuf);
  // modify angle
  d1 += aInc * mode;
  if(fabs(d1) < 361.) {
    cbuf[0] = '\0';
    UTX_add_fl_u (cbuf, d1);
  }
  goto L_fertig;



  //================================================================
  // decode & incr  "ANG(90)"
  L_3:
  p1 += 4;
  d1 = atof(p1);


  // modify angle
  d1 += aInc * mode;
  if(fabs(d1) < 361.) {
    *p1 = '\0';
    UTX_add_fl_u (cbuf, d1);
    strcat(cbuf, ")");
    // goto L_fertig;
  }


  L_fertig:
    printf(" d1=%lf cbuf=|%s|\n",d1,cbuf);
  // gtk_entry_set_text ((GtkEntry*)(IE_wCad_obj[IE_inpInd]), cbuf);
  IE_set_inp (IE_inpInd, cbuf);

  // test ..
  IE_cad_test__ ();

  return 0;

}


//=====================================================================
  int IE_cad_Inp1_nxtpNr (int mode, int typ) {
//=====================================================================
// Zeile ist ca "L20" od "L20 MOD(1)".
// Wenn "L20": add " MOD(1)"; else incr od decr last Nr.


  int    i1, oTyp, maxVal;
  long   oInd;
  char   *p1, *p2, cbuf[256];


  printf("IE_cad_Inp1_nxtpNr %d %d\n",mode,typ);


  p1 = IE_get_inp__(IE_inpInd);
  // printf(" inp=|%s| %d\n",p1,strlen(p1));

  if(strlen(p1) < 2) return 0;

  strcpy(cbuf, p1);

  APED_dbo_oid (&oTyp, &oInd, cbuf);
  if(oTyp == Typ_PT) goto L_fertig;  // is already point .. //2013-04-16


  // find last blank
  p1 = strrchr (cbuf, ' ');


  // kein blank: add " 1"
  if(p1 == NULL) {
    strcat(cbuf, " MOD(2)");
    goto L_fertig;
  }


  // maxVal = 2;
  // if(oTyp == Typ_CV) maxVal = UTO_ptnr_obj (oTyp, oInd);
  // else TX_Print("IE_cad_Inp1_nxtpNr I001");
  // get nr of characteristic points for obj
  maxVal = UTO_ptnr_std_obj (oTyp, oInd);


  // decode last nr
  p2 = p1 + 1;  // blank
  // printf(" p2=|%s|\n",p2);
  if(strncmp(p2,"MOD(",4)){TX_Print("IE_cad_Inp1_nxtpNr I002"); return -1;}
  p2 += 4;
  i1 = atoi(p2);
  // printf(" i1=%d mode=%d\n",i1,mode);
  i1 += mode;
  if(i1 < 1) {
    i1 = 1;
    OS_beep();
  } else if (i1 > maxVal) {
    i1 = maxVal;
    OS_beep();
  }

  sprintf(p1, " MOD(%d)",i1);



  L_fertig:
  // gtk_entry_set_text ((GtkEntry*)(IE_wCad_obj[IE_inpInd]), cbuf);
  IE_set_inp (IE_inpInd, cbuf);

  IE_cad_test__ ();

  return 0;

}

//=====================================================================
  int IE_cad_Inp1_Aux (int mode) {
//=====================================================================
// abwechselnd blank bzw das letzte Infowort ausgeben. Typ_modAux.
// Das letzte Wort der Info wird ausgegeben; Delimiter muss blank sein !
// see also IE_cad_Inp1_nxtREV

  int    i1;
  char   *p1, cbuf[32];


  printf("IE_cad_Inp1_Aux %d ind=%d\n",mode,IE_inpInd);
  printf(" info=|%s|\n",IE_cad_act[IE_first + IE_inpInd].info);


  p1 = IE_get_inp__(IE_inpInd);
  printf(" inp=|%s| %d\n",p1,(int)strlen(p1));


  if(strlen(p1) < 1) {
    // write word after last blank into inputField <IE_inpInd>
    i1 = IE_first + IE_inpInd;
    p1 = strrchr(IE_cad_act[IE_first + IE_inpInd].info, ' ');
    if(p1) ++p1;
    else TX_Error("IE_cad_Inp1_Aux E001");
    strcpy(cbuf, p1);

  } else {
    // clear inputField <IE_inpInd>
    strcpy(cbuf, "");
  }

  IE_set_inp (IE_inpInd, cbuf);

  IE_cad_test__ ();

  return 0;

}


//================================================================
  int IE_cad_Inp1_DirX () {
//================================================================

  char   *p1, cbuf[8];


  p1 = IE_get_inp__(IE_inpInd);


  strcpy(cbuf, "test");
  GUI_entry_set (&IE_wCad_obj[IE_inpInd], cbuf);

  IE_cad_test__ ();



  return 0;

}

//=====================================================================
  int IE_cad_Inp1_nxtTxt (int iMod, int mode) {
//=====================================================================
// iMod    1=Typ_mod1, 2=Typ_mod2
// TODO: PageDwn soll next record aus einer tabelle der possible values holen

  int    i1, maxVal;
  char   *p1, cbuf[32];
  double d1;
    
      
  // printf("IE_cad_Inp1_nxtTxt %d\n",mode);
  
  p1 = IE_get_inp__(IE_inpInd);
    // printf(" inp=|%s| %ld\n",p1,strlen(p1));
  
    
  if(strlen(p1) < 1) {
    i1 = 0;
    
  } else {
    if(iMod == 1) maxVal = APT_get_modMax1();
    else          maxVal = APT_get_modMax2();
    // decode active value -> i1
    i1 = atoi(IE_get_inp__(IE_inpInd));
    i1 += mode;
    if(i1 < 0) {
      i1 = 0;
      OS_beep();
    } else if (i1 > maxVal) {
      i1 = maxVal;
      OS_beep();
    }
  }


  sprintf(cbuf, "%d",i1);
    // printf(" _modCoord=|%s| %d\n",cbuf,i1);

  IE_set_inp (IE_inpInd, cbuf);

  IE_cad_test__ ();

  return 0;

}


//=====================================================================
  int IE_cad_Inp1_nxtMod (int iMod, int mode) {
//=====================================================================
// modify active inputField (PageUp|PageKey pressed).
// Input:
//   iMod    1=Typ_mod1, 2=Typ_mod2
//   mode    1=up, -1=down
// TODO: PageDwn soll next record aus einer tabelle der possible values holen

  int    i1, maxVal;
  char   *p1, cbuf[32];
  double d1;


  printf("IE_cad_Inp1_nxtMod %d %d\n",iMod,mode);

  if(iMod == 1) maxVal = APT_get_modMax1();   // -1=all, -2=none
  else          maxVal = APT_get_modMax2();
    printf(" _nxtMod-imod = %d modMax =%d\n",iMod,maxVal);


  if(maxVal < 1) {
    if(maxVal == -2) {          // -2=none; no MOD-value possible
      TX_Print ("**** no version for this obj ..");
      return 0;
    }

    TX_Print ("**** no other version for this obj ..");
    // return 0;
  }


  // p1 = get inputField
  p1 = IE_get_inp__(IE_inpInd);
    // printf(" inp=|%s| %ld\n",p1,strlen(p1));


  if(strlen(p1) < 1) {
    // empty; set to 1
    i1 = 1;

  } else {
    // decode active value -> i1
    i1 = atoi(p1);
    i1 += mode;
    if(i1 < 1) {
      i1 = 1;
      OS_beep();
    } else if (i1 > maxVal) {
      i1 = maxVal;
      OS_beep();
    }
  }


  sprintf(cbuf, "%d",i1);
    // printf(" _modCoord=|%s| %d\n",cbuf,i1);

  IE_set_inp (IE_inpInd, cbuf);

  IE_cad_test__ ();

  return 0;

}


//=====================================================================
  int IE_cad_Inp1_nxtVal (int mode) {
//=====================================================================
// IE_cad_Inp1_nxtVal            provide next|previous value;
//   used by key PgUp|Page_Down and GL2D-buttons "V+"|"V-"
// mode= -1: PgUp
// mode=  1: Page_Down
// see also AP_get_nxtVal IE_cad_Inp1_nxtVec



// static long vi=20;

  int    i1;
  long   l1 , dbi;
  char   *p1, *p2, cbuf[32];
  double d1;


  // aktuelle FeldNr = IE_inpInd;
  // Feldinhalt holen
  p1 = IE_get_inp__(IE_inpInd);


  // printf("<<<<<<<<<<<<<<<\nIE_cad_Inp1_nxtVal %d\n",mode);
  // printf("  IE_cad_typ=%d IE_FuncSubTyp=%d\n",IE_cad_typ,IE_FuncSubTyp);


  if(IE_cad_typ == Typ_CV) goto L_cv;


  //----------------------------------------------------------------
  // get dbi last used;
  // wenn dzt kein Var aktiv, den letzten definierten holen
  if(p1[0] == 'V') {
    ++p1;
    dbi = strtol (p1, &p2, 10);
    if(p2 != p1) goto L_1;
  }

  dbi = DB_dbo_get_free (Typ_VAR);


  //----------------------------------------------------------------
  // get next or prev. dbi
  L_1:
      // printf(" dbi=%ld\n",dbi);
    if(mode > 0) {
      l1 = DB_QueryNxtUsed (Typ_VAR, dbi + 1);
    } else {
      l1 = DB_QueryPrvUsed (Typ_VAR, dbi - 1);
    }
    if(l1 > 0)  dbi = l1;
      // printf(" ex NxtUsed dbi=%ld\n",dbi);


  if(dbi < 1) {
    MSG_pri_0("VR0");
    return 0;
  }


  //----------------------------------------------------------------
  // get value, display value as text
  d1 = DB_GetVar (dbi);

  // test if textline before last starts with " VAR";
  //   if yes: delete last 2 textlines.
  if(IE_inpStat) {
    UI_winTX_rmLast ();
    UI_winTX_rmLast ();
  }
  IE_inpStat = 1;

  TX_Print ("VAR %ld = %f",dbi,d1);

  sprintf(cbuf, "V%ld",dbi);


  //----------------------------------------------------------------
  // copy objID into inputfield, test inputs
  L_done:
  GUI_entry_set (&IE_wCad_obj[IE_inpInd], cbuf);
  IE_cad_test__ ();

  return 0;



  //----------------------------------------------------------------
  // create curve is active .. ??
  L_cv:
  if(IE_FuncSubTyp != 6) goto L_bsp_smv;
  L_bsp_deg:
    // test max degree of active bspl-curv
  i1 = atoi(p1);
  i1 += mode;
  i1 = ILIM2 (i1, 1, APT_get_modMax1());
  sprintf(cbuf, "%d", i1);
  goto L_done;


  L_bsp_smv:
  if(IE_FuncSubTyp != 8) return -1;
  // smoothVal 0-1
  d1 = atof(p1);
  d1 += mode * 0.2;
  d1 = DLIM2 (d1, 0., 1.);
  sprintf(cbuf, "%.1f", d1);
  goto L_done;


}


//================================================================
  int IE_inp_selection_clear () {
//================================================================
// IE_inp_selection_clear    check / clear a active selection in inputfield

  // printf("IE_inp_selection_clear %d\n",IE_inpInd);

  if(IE_inpInd < 0) return -1;

  // clear selection
  // GUI_selection_clear (IE_wCad_obj[IE_inpInd]);

  return 0;

}


//=====================================================================
  int IE_cad_ck_inpLast () {
//=====================================================================
// returns 1 is last inputfield is active else 0

  int   irc = 0, iLast, ii, i1, i2;


  // printf("IE_cad_ck_inpLast %d\n",IE_inpInd);


  iLast = IE_inpAnz - 1;
  if(IE_inpInd >= iLast)  irc = 1;

    // printf("ex IE_cad_ck_inpLast %d\n",irc);


  return irc;

}

/* unused
//=====================================================================
  int IE_cad_Inp2_CB (void *parent, GdkEventAny *event) {
//=====================================================================
// focus-out-event in inputfield
// IE_inpInd = active_field_ind

  int    i1, iLast;


  // printf("----------------------------------------------- \n");
  printf("OOOOOOOOOOO     IE_cad_Inp2_CB IE_inpInd=%d\n",IE_inpInd);
  // printf("  IE_stat__=%d IE_modify=%d\n",IE_stat__,IE_modify);
  // printf("  IE_inpAnz=%d\n",IE_inpAnz);
  // printf("  event->type=%d\n",event->type); always 12 or NULL




  // // Feld IE_inpInd wird verlassen;
  // // display temp. symbols for old inpField:
  // i1 = IE_cad_Inp_disp__ (IE_inpInd, -1);
  // if(i1 < 0) goto L_exit;


  // // eval fields; if complete: display obj
  // i1 = IE_cad_test__ ();


  // if active field == last field:
  L_exit:
  // iLast = IE_inpAnz - 1;


  // mouseklick: do nothing ..
  // Tab, CursorDown: reactivate this field.
  // CursorUp, BackTab: ?
  

  // if(IE_inpInd >= iLast)  {
    // leave last inputfield: activate last inputfield again
    // MIST: damit kann man in kein anderes Eingabefeld klicken !!
    // GUI_obj_focus (IE_wCad_obj[iLast]);
    // Must continue normal with handler !
  // }



  return FALSE;

}
*/

//================================================================
  int IE_set_inpSrc (int src) {
//================================================================

  IE_inpSrc = 3;  // pgUp || 2d-icon-select; do not proceed to next inpField
 
  return 0;

}
 

//================================================================
  int IE_gr_key_CB (void **data) {
//================================================================

  return IE_inp_CB__ (&IE_wCad_obj[IE_inpInd], data);

}
 

//=====================================================================
  int IE_nam_CB__ (MemObj *mo, void **data) {
//=====================================================================
// callback of the CAD-name-inputfield


  int         iKey;


  // printf("----------------------------------------------- \n");
  // printf("IIIIIIIIIII     IE_nam_CB__ IE_inpInd=%d\n",IE_inpInd);

  if(GUI_DATA_EVENT != TYP_EventPress) return 0;

  iKey = GUI_DATA_I2;
    // printf(" GUI_DATA_EVENT=%d iKey=%d\n",GUI_DATA_EVENT,iKey);

  switch (iKey) {
    case GUI_KeyReturn:
    return IE_cad_OK ();

  }

  return 0;

}


//=====================================================================
  int IE_inp_CB__ (MemObj *mo, void **data) {
//=====================================================================
// callback of the CAD-inputfields (entry).

// foucs-in-event (Focus faellt auf ein Inputfeld) oder
// key-release in einem Inputfeld.

// Input:
//   IE_inpInd   index of inputfield which is active; 0=first


  int         irc, typ, iKey, i1, i2;
  long        dbi;
  char        typChar, *pi;


  // printf("IIIIIIIIIII     IE_inp_CB__ IE_inpInd=%d\n",IE_inpInd);
  // printf("  IE_stat__=%d IE_modify=%d\n",IE_stat__,IE_modify);
  // printf("  IE_inpAnz=%d\n",IE_inpAnz);



  // startup-modify: noch nix tun (Inhalt der Felder noch nicht gesetzt)
  if(IE_stat__ > 1) {
    return 0;
  }


  // es muss grp u subgrp aktiv sein
    // printf("  IE_cad_typ=%d\n",IE_cad_typ);
  if(IE_cad_typ < 0) return -1;


  // skip M3 in last field ..
  if(IE_inpInd > IE_inpAnz) {
    IE_inpInd = IE_inpAnz;
      // printf(" skip - last field ..\n");
    return 0;
  }


  // Reset neues Element (typ_old = -1) duch NULL,NULL.
  // sonst "skip FocusChange"
  // if((parent == NULL)&&(event == NULL)) {
  if((mo == NULL)&&(data == NULL)) {
    return IE_cad_InpIn__ (-2);
  }


  // ein reset ?
  // if(event == NULL) goto L_02;
  if(data == NULL) goto L_02;



  //----------------------------------------------------------------
  // TYP_EventEnter = eg click_into_field !
  if(GUI_DATA_EVENT == TYP_EventEnter) {
    goto L_02;
  }


  iKey = GUI_DATA_I2;
    // printf(" TYP_EventEnter iKey=%d\n",iKey);

/*
  // handle grafic operations
  // check for shift|Ctrl|Alt-key  - do grafic-viewing
  // unused; entering graficWindow set focus -> graficWindow
  if(!isascii(iKey)) {
    irc = UI_key_view__ (GUI_DATA_EVENT, iKey);
    if(irc) return 1;    // TRUE=1=skip defaultOperations
  }
*/


  //----------------------------------------------------------------
  if(GUI_DATA_EVENT == TYP_EventPress) {    // 302
      // printf(" inp_CB__-TYP_EventPress iKey=%d\n",iKey);


    switch (iKey) {
      case GUI_KeyEsc:
        IE_cad_Inp_undo (); // delete last CAD-input or exit menuFunction
        return 1; // do no defaultOperations; skip handling this key

      case GUI_KeyCurDown:
      case GUI_KeyTab:
        // test if this is the last field                    // 2013-03-16
        // if((IE_inpInd+1) >= IE_inpAnz) {    // removed 2013-04-29
           IE_cad_test__ ();   // test input
           // GUI_obj_focus (&IE_wCad_obj[IE_inpInd]); makes last entry selected
           // GUI_obj_focus (&winGR);  makes 1 entry selected .. ?
           return 0;
        // }

      case GUI_KeyPageUp:
        i1 = -1;
        goto L_01;

      case GUI_KeyPageDown:
        i1 = 1;
        goto L_01;

      case GUI_KeyReturn:
          return IE_cad_OK ();

    }
    return 0; // continue with defaultOperations for this key
  }


  //----------------------------------------------------------------
  if(GUI_DATA_EVENT == TYP_EventRelease) {    // 303
      // printf(" inp_CB__-TYP_EventRelease iKey=%d\n",iKey);

      // ev_k = (void*)event;
        // printf("       Inp1_CB keyval=%d %x\n",ev_k->keyval,ev_k->keyval);

    switch (iKey) {

      // case GDK_ISO_Left_Tab:         // shift Tab
      case GUI_KeyCurUp:
      case GUI_KeyCurDown:
      case GUI_KeyTab:
      case GUI_KeyReturn:
        IE_inpSrc = 2;    // 2=keyIn
        return 0; // ignore (do default-GTK-eventhandler - goto next widget)
          // if(IE_inpInd >= IE_inpAnz) {
            // gtk_widget_grab_focus (IE_wCad_obj[IE_inpAnz]);
            // return 1;
          // }
          // goto L_02;

        // case 'z':
        // case 'Z':
          // if(KeyStatCtrl != ON) return 0;
            // printf(" Ctr-Z\n");
          // return 0;

        default:              // normaler Key
          return 0;
      }


/*
      if     (ev_k->keyval == GUI_KeyPageUp)   i1 = -1;
      else if(ev_k->keyval == GUI_KeyPageDown) i1 = 1;
      // bei Cr,Tab,Backtab,CursorUp/Dwn weiter -> L_01
      // else (normaler Key) nix tun
      else goto L_01;
      // else return 0;
*/

      //----------------------------------------------------------------
      // handle PgUp/Dwn-KeyStroke:
      L_01:
      IE_inpSrc = 3;    // 3=PgUp/Dwn
      typ = IE_inpTypR[IE_inpInd];
        // printf(" L_01:-PgUp/Dwn %d typ=%d feld=%d\n",i1,typ,IE_inpInd);

      if      (typ == Typ_Angle)      IE_cad_Inp1_nxtAng (i1);
      else if (typ == Typ_PLN)        IE_cad_Inp1_PLN    (i1);
      else if (typ == Typ_PT)         IE_cad_Inp1_nxtpNr (i1,Typ_PT);
      else if((typ == Typ_VC)     ||
              (typ == Typ_go_PD))    IE_cad_Inp1_nxtVec (&dbi, i1);
      else if (typ == Typ_Tra)        IE_cad_Inp1_nxtTra (i1);
      else if (typ == Typ_Txt)        IE_cad_Inp1_nxtTxt (1, i1);
      else if((typ == Typ_XVal)    ||
              (typ == Typ_YVal)    ||
              (typ == Typ_ZVal))      IE_cad_Inp1_nxtVal (i1);
      else if((typ == Typ_PTI)     ||
              (typ == Typ_PTS)     ||
              (typ == Typ_mod1))      IE_cad_Inp1_nxtMod (1, i1);
      else if (typ == Typ_mod2)       IE_cad_Inp1_nxtMod (2, i1);
      else if (typ == Typ_modAux)     IE_cad_Inp1_Aux    (i1);
      else if (typ == Typ_modREV)     IE_cad_Inp1_nxtREV (i1, "REV");
      else if (typ == Typ_modCX)      IE_cad_Inp1_nxtREV (i1, "CX");
      else if (typ == Typ_modPERP)    IE_cad_Inp1_nxtREV (i1, "PERP");
      else if (typ == Typ_modUnlim)   IE_cad_Inp1_nxtREV (i1, "UNL|UNL1|UNL2");
      else if (typ == Typ_modCTRL)    IE_cad_Inp1_nxtREV (i1, "CTRL");
      else if (typ == Typ_modCWCCW)   IE_cad_Inp1_nxtREV (i1, "CW");
      else if (typ == Typ_modRepl)    IE_cad_Inp1_nxtREV (i1, "REPL");
      else if (typ == Typ_FncDirX)    IE_cad_Inp1_nxtFDX ();
      else if (typ == Typ_SubModel)   IE_cad_Inp1_nxtSM  (i1);
      else if (typ == Typ_EyePT)      IE_cad_Inp1_DirS   (i1);
      else if (typ == TYP_FilNam)     IE_cad_selM2 (-1);


      IE_cad_Inp_disp__ (IE_inpInd, 0);    // display (update) symbols
      return 0;
    }


  //----------------------------------------------------------------
  // startPhase:
  L_02:
    // printf(" inp_CB__-L02-IE_stat__=%d\n",IE_stat__);
  if(IE_stat__ == 1) {
    return IE_cad_InpIn__ (0);
  }



  // feldNr des neuen Feldes (das automat. aktiviert wurde) suchen;
  //   IE_cad_InpIn__ damit aufrufen ..
  for(i1=0; i1<INPRECANZ; ++i1) {
      // printf(" inp_CB__-L02-i1=%d\n",i1);

    // do not change to next field if all necessary fields full; else loop !
    if(i1 > IE_inpAnz) return 0;          // 2011-01-28
    // war >= 2011-02-02 wieder raus; Fehler mit CAD/S=Rectangle..

    if(!GUI_obj_cmp (mo, &IE_wCad_obj[i1])) {
        // printf(" fL02-found %d\n",i1);
/*
      if(i1 > IE_inpAnz) {
        // nach Key Cursor-down; aktuelles feld reaktivieren; 
        gtk_widget_grab_focus (IE_wCad_obj[IE_inpAnz]);
        return 1;
      }
*/
      // activate field i1
      return IE_cad_InpIn__ (i1);
    }
  }



  return 0;

}


//================================================================
  int IE_get_tempPos (Point *pt1) {
//================================================================
// IE_get_tempPos       returns position for a temporary object


  if(IE_inpInd > 0) goto L_sel;

  *pt1 = GL_GetCen();     // get ScreenCenter
  goto L_exit;


  L_sel:
  sele_get_pos__ (pt1);     // get last selection-point


  L_exit:
  // DEB_dump_obj__ (Typ_PT, pt1, "ex IE_get_tempPos:");

  return 0;

}


//================================================================
  int IE_cad_InpIn_left () {
//================================================================
// goto end of upper cad-inputField
// RetCod: 0=not on position 0;   1=OK, done.

  int    ii;
  char   *p1;

  // printf("IE_cad_InpIn_left %d\n",IE_inpInd);

  if(IE_inpInd <= 0) return 0;


  p1 = IE_get_inp__ (IE_inpInd);
    printf(" p1=|%s|\n",p1);
  if(strlen(p1) < 1) {            // only if active inputField is empty !

    // get cursorposition in cad-inputField IE_inpInd
    ii = GUI_entry_pos_get (&IE_wCad_obj[IE_inpInd]);
      // printf(" ii=%d\n",ii);
  
    // leftmost position: goto previous inputfield
    if(ii < 1) {
      IE_inp_chg (-2);  // previous inputfield
        // printf(" IE_inpInd=%d\n",IE_inpInd);
      GUI_entry_sel_del (&IE_wCad_obj[IE_inpInd]);
      return 1;           // terminate event
    }

  }


  return 0;

}


//================================================================
  int IE_inp_ck_empty () {
//================================================================
// IE_inp_ck_empty      check if all inputfelds empty
// retCod: 0=fields not empty;
//         1=all fields empty
// test also "<group>" for eg planar Surf. ?

  int   i1;
  char  *p1;
  
  
  printf("IE_inp_ck_empty IE_inpAnz=%d\n",IE_inpAnz);

  for(i1=0; i1<IE_inpAnz; ++i1)  {
    p1 = GUI_entry_get (&IE_wCad_obj[i1]);
      printf(" ie[%d]=|%s|\n",i1,p1);
    if(strlen(p1) > 0) return 0;
  }

    printf("ex-_ck_empty-irc=1\n");

  return 1;

}


//================================================================
  int IE_inp_ck_prev_empty (char **ep) {
//================================================================
// - get pointers to all inputfields into ep;
// - test if 2 consecutive inputfields have same types;
//     if yes: if second field has a value, add "0" into first field.


  int   i1, ii;
  // char  *p1, *p0;

  // printf("IE_inp_ck_prev_empty \n");


  // get pointers to all fields -> ep
  // set all ep to NULL {
  ii = IE_inpAnz;
  for(i1=0; i1<IE_inpAnz; ++i1) {
    ep[i1] = GUI_entry_get (&IE_wCad_obj[i1]);
    // // find ii = last field with value
    // if(strlen(ep[i1]) > 0) ii = i1;
  }
  ep[IE_inpAnz] = NULL;


  // if(IE_inpAnz < 2) return 0;

  // test 2 fields ..
/*
PROBLEM: do not (eg edit line p-p) change p1 to "0" if p2 is empty
  for(i1=0; i1<ii; ++i1) {
    i0 = i1 - 1;
    if(IE_inpTypR[i0] != IE_inpTypR[i1]) continue;
    if(strlen(ep[i0]) > 0) continue;
    if(strlen(ep[i1]) > 0) continue;
    // change previous filed to"0"
    GUI_entry_set (&IE_wCad_obj[i0], "0");
    ep[i0] = GUI_entry_get (&IE_wCad_obj[i0]);
  }
*/

  // TESTBLOCK
  // for(i1=0; i1<ii; ++i1) printf(" -CAD-ent- %d |%s|\n",i1,ep[i1]);
  // END TESTBLOCK


  return 0;

}

 
//================================================================
  int IE_cad_InpIn__ (int ind) {
//================================================================
// IE_cad_InpIn__                       activate cad-inputField
// disactivate the active inputField, then activate inputField ind
// ind     -2 = clear, reset active menu and display


  int         typ_old = -1, ind_old = -1;
  int         irc, i1, i2;
  char        *pi, grp_typ_old;



  // printf("11111111111 IE_cad_InpIn__ ind=%d IE_inpInd=%d\n",ind,IE_inpInd);
  // printf("  IE_inpAnz=%d\n",IE_inpAnz);


  if(IE_cad_act == NULL) return -1;


  if(ind == -2) {      // Reset
      // printf("IE_cad_InpIn__ reset\n");
    // if(grp_typ_old == '<') {    // disactivate normal-group
    // if(IE_ed1_win.stat != 0) {   // exit normal-group
    if(GUI_OBJ_IS_VALID(&IE_edWin1)) {
      // kill GruppenEditorWindow, if activ
      // IE_ed1__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncKill));
      IE_ui_func (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncKill));
    }
    IE_inpInd    = -1;
    IE_inpTypAct = -1;
    IE_grp_typ   = '-';
    // delete preview
    GL_last_del ();     // delete last obj of DL, if its DB-index is 0
    GL_temp_del_all ();  // delete temp-circles
    DL_Redraw ();
    goto L_exit;
  }


  // check for FocusChange (newField == oldField)
  if(IE_inpInd == ind) {
    // printf(" L_02-skip FocusChange\n");
    // stay in active field ..
    GLBT_set_stat (IE_inpInd);
    DL_Redraw ();      // update 2D-icons
    goto L_exit;
  }

/*
  // init - after modify !
  if(ind == -1) {
    IE_inp_chg (0); // select text of entry 0
    IE_inpInd = -1;   // ind_old
    ind = 0;          // new ind
  }
*/

  // last field: ??
  if(ind >= IE_inpAnz) {
    // ind ist ein Leerfeld;
    // IE_inp_chg (IE_inpAnz); // aufs erste leere Feld 2011-01-28 raus
    // return TRUE;  // ACHTUNG: tut was; Default-Handler !!!
    ind_old = ind;
    goto L_exit;
  }



  //-----------------------------------------
  ind_old      = IE_inpInd;         // index active inputField
  typ_old      = IE_inpTypAct;
  grp_typ_old  = IE_grp_typ;
    // printf(" InpIn__-ind_old=%d\n",ind_old);


  IE_inpInd    = ind;                          // FieldIndex of new field
  IE_inpTypAct = IE_inpTypR[IE_inpInd];
  IE_grp_typ   = IE_cad_act[IE_inpInd+IE_first].info[0];
  IE_inpStat   = 0;  // inputfiled has been changed, not yet initialized
    // printf("  IE_inpInd=%d IE_grp_typ=|%c|\n",IE_inpInd,IE_grp_typ);
    // printf("  IE_first=%d IE_last=%d fieldNr ind=%d\n",IE_first,IE_last,ind);





  //-----------------------------------------
  // disactivate active inpField ind_old:
  //-----------------------------------------
    // printf(" disactiv. ind_old=%d grp_typ_old=|%c| typ_old=%d\n",
                       // ind_old,grp_typ_old,typ_old);
  if(ind_old < 0) goto L_neu__;

  // modify color of inputfield-info-label.
  IE_info_col_set (0, ind_old);   // reset old info


  // if(grp_typ_old == '<') {    // disactivate normal-group
  if((grp_typ_old == '<')||(grp_typ_old == '(')) {  // disactivate group
    // if(IE_ed1_win.stat != 0) {   // exit normal-group
      // printf(" IE_edWin1.ioff=%d\n",IE_edWin1.ioff);

    if(GUI_OBJ_IS_VALID(&IE_edWin1)) {
        // printf(" IE_edWin1 is active ..\n");

      // copy "Group-Edit" - content -> IE_buf
      IE_ui_func (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncGet));
      // copy IE_buf -> field[ind_old]
      GUI_entry_set (&IE_wCad_obj[ind_old], IE_buf);
      // clear Group-Edit - entry
      IE_buf[0] = '\0';  // else content for field IE_inpInd in IE_cad_tet__
      IE_bufSiz = 0;
      // kill GruppenEditorWindow, if activ
      IE_ui_func (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncKill));
    }


  } else {
    // only MODIFY: display/update temp. symbols for old inpField
    irc = IE_cad_Inp_disp__ (ind_old, -1);
      // printf(" f-irc-cad_Inp_disp__ %d\n",irc);
    if(irc < 0) return -1;
  }



  // eval fields; if complete: display obj
  L_old_test:
  irc = IE_cad_test__ ();
    // printf(" f-cad_test irc=%d\n",irc);
  if(irc == -2) {   // -3=obj not complete
    // reactivate this field
    return -1;
  }


  //-----------------------------------------
  // activate new inpField IE_inpInd:
  //-----------------------------------------
  L_neu__:
    // printf("  L_neu__: next IE_inpInd %d\n",IE_inpInd);

  // modify color of inputfield-info-label.
  IE_info_col_set (1, IE_inpInd);  // set new info=red


  // Infotexte fuers neue inpField unten ausgeben
  IE_cad_Inp1_Info (-1);


  // Defaults ausgeben (vorbelegen predefine preSet)
  // define new selectionFilter.
  sele_set__ (IE_inpTypAct);


  // get fieldText -> pi
  pi = IE_get_inp__(IE_inpInd);
  i1 = strlen(pi);
    // printf(" Inputfeld %d len=%d |%s|\n",IE_inpInd,i1,pi);


  // check for group-window
  if(GROUP_IS_ACTIVE) {
      // printf(" Grp: IE_inpTypAct=%d\n",IE_inpTypAct);

    // window already active ?
    if(GUI_OBJ_IS_VALID(&IE_edWin1)) {
      IE_ui_func (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncKill));
    }

    if(IE_inpTypAct == Typ_goPrim) {
      // activate CCV-window
      if(IE_grp_typ == '<') i2 = 0;
      else                  i2 = 1;
      IE_ccv_set_winTyp (i2);
      IE_ui_func = IE_ccv__;
    } else {
      // Start GroupEditor
      IE_ui_func = IE_ed1__;
    }
    // set existing text into Group-Edit-Window
    if(i1) {
      strcpy(IE_buf, pi); 
      IE_bufSiz = i1;
    }
    // activate Group-Edit-Window
    IE_ui_func (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncInit));
    // write "(Group)" -> inpField
    IE_set_inp (IE_inpInd, "<Group>");
    goto L_exit;
  }



  // Ist das neue Feld leer ?
  // leer: vorbelegen
  if(i1 > 0) goto L_0;
  // wenn soeben ein FeldLoeschen (IE_cad_Inp_undo) war: nix vorbelegen ..
  // if(??) goto L_0;


  //-----------------------------------------
  // activate empty inpField IE_inpInd:
  //-----------------------------------------

  // if(IE_grp_typ == '<') {    // init normal-group
    // // Start GroupEditor
    // IE_ed1__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncInit));
    // // write "(Group)" -> inpField
    // IE_set_inp (IE_inpInd, "<Group>");
    // goto L_exit;
  // }

/*
  if(IE_grp_typ == '(') { // init union-group
      // printf(" init union-group for empty inpField %d\n",IE_inpInd);
    Grp_Start ();
    IE_set_inp (IE_inpInd, "(Group)"); // write "(Group)" -> inpField
    goto L_exit;
  }
*/

  // preset normal field
    // printf(" preload %d |%s|\n",IE_inpInd,IE_wCad_preLoad[IE_inpInd]);
  if(strlen(IE_wCad_preLoad[IE_inpInd]) > 0) {
    IE_set_inp (IE_inpInd, IE_wCad_preLoad[IE_inpInd]);

    // test preload
    irc = IE_cad_test__ ();
  }


  goto L_exit;



  //-----------------------------------------
  // activate not-empty inpField IE_inpInd:
  //-----------------------------------------
  L_0:

  // if(IE_grp_typ == '<') {    // init normal-group
    // // Start GroupEditor
    // IE_ed1__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncInit));
    // // write "(Group)" -> inpField
    // IE_set_inp (IE_inpInd, "<Group>");
    // goto L_exit;
  // }

/*
  if(IE_grp_typ == '(') { // init union-group
      // printf(" init union-group for preset inpField %d\n",IE_inpInd);
    // decode pi; activate group ..
    // IE_cad_Inp1_Grp (1, IE_inpInd);
    Grp_Clear (1);
    Grp_Start ();
    APED_dec_txt (pi); // add all objs to group
    Grp_dlAdd (ON);    // hilite all objs of grp
    IE_set_inp (IE_inpInd, "(Group)"); // write "(Group)" -> inpField
    goto L_exit;

  }
*/

  L_exit:
    // printf("ex-IE_cad_InpIn__ IE_inpInd=%d old=%d\n",IE_inpInd,ind_old);
  return 0;

}



//================================================================
  int IE_undo () {
//================================================================
// IE_undo              Ctrl-Z pressed or Undo-Button pressed ..
// RetCod:
//    0 = undo done; do not delete last group-element
//   -1 = no IE-undo possible; do default operation
//
// see also IE_cad_Inp_undo !

// unused: Ctrl-K blocked ..

  int   i1;

  // printf("IE_undo \n");

  // CAD must be active
  if(UI_InpMode != UI_MODE_CAD) return -1;

  // group-window IE_ed1__ active ?
  // if(IE_ed1_win.stat == 0) return -1;
  // if(!GUI_OBJ_IS_VALID(&IE_edWin1)) return -1;
  i1 = IE_ed1_stat();      // printf("_ed1_stat-i1=%d \n",i1);

  if(i1 == -1) goto L_1;   // -1=not-exists;
  if(i1 == 0) return -1;   // 0=exists, but empty; 
  return 0;                // 1=contains data


  //----------------------------------------------------------------
  L_1:
  // CCV-Edit-window is not active;
  // no CAD func active ?
  if(IE_get_Func() < 0) return -1;

  // all entries empty ?
  if(IE_inp_ck_empty()) return -1;

  // delete last word from IE_buf
  // IE_ed1_mod (NULL, GUI_SETDAT_ES(TYP_EventPress,"DEL"));
  // IE_ui_func (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncPrev));
  IE_buf_del ();

  return 0;

}


//=====================================================================
  int IE_cad_Inp_cancel () {
//=====================================================================
// delete last CAD-input or exit menuFunction (only for ADD)
// called by key Escape or cancel-button

  int   li;
  char  *pi;


  // printf("IE_cad_Inp_cancel IE_inpInd=%d IE_grp_typ=|%c|\n",IE_inpInd,IE_grp_typ);

  // exit active cadFunction ..
  IE_cad_exitFunc ();

  if(IE_inpInd < 0) return 0;

  return 0;

}


//=====================================================================
  int IE_cad_Inp_undo () {
//=====================================================================
// delete last CAD-input or exit menuFunction (only for ADD)
// called by key Escape or cancel-button

  int   li;
  char  *pi;


  // printf("IE_cad_Inp_undo IE_inpInd=%d IE_grp_typ=|%c|\n",IE_inpInd,IE_grp_typ);

  // is input active
  if(IE_inpInd < 0) return 0;


  // clear temp-error
  // AP_errStat_reset (0);                   // 2013-05-06


  if(IE_grp_typ == '<') {
    // if(IE_ed1_win.stat == 0) goto L_exit;
    if(!GUI_OBJ_IS_VALID(&IE_edWin1)) goto L_exit;
    // IE_ed1_mod (NULL, GUI_SETDAT_ES(TYP_EventPress,"DEL"));
    li = IE_buf_del ();
    if(li < 0) goto L_exit;     // window empty; exit CAD-func
    // update display IE_ccv__
    IE_cad_test__ ();
    return 0;
  }


  // is active inputfield group ?
  if(IE_grp_typ == '(') {
    li = Grp_del (Typ_last, 0L, 1);
    if(li < 0) goto L_exit;
    IE_cad_test__ ();
    return 0;
  }


  // clear a CAD-group, if one exists
  if(Grp_get_nr() > 0) {
    Grp_Clear (1);     // clear group
    return 0;
  }


  // get content of active inputField
  pi = IE_get_inp__ (IE_inpInd);
  li = strlen (pi);
    // printf(" _Inp_undo-li=%d pi=|%s|\n",li,pi);



  if(li > 0) {
    // clear content of active inputfield ..
    IE_cad_ClearInp1 (IE_inpInd, 1);
    // update depending inputFields
    IE_inp_ck_dep (IE_inpInd);
    IE_cad_test__ ();
    return 0;
  }
 

  // clear upper inputfield and go back into upper inputfield ..
  if(IE_inpInd > 0) {
    IE_cad_ClearInp1 (IE_inpInd - 1, 1);
    IE_inp_chg (-2);
    IE_cad_test__ ();
    return 0;
  }


  L_exit:
  // exit active cadFunction ..
  IE_cad_exitFunc ();

  return 0;

}


//================================================================
  int IE_cad_Inp_disp_pt (Point *pt1, int iind) {
//================================================================
// IE_cad_Inp_disp_pt      display point for actual inputfield

  long   l1;


  // DEB_dump_obj__ (Typ_PT, pt1, "IE_cad_Inp_disp_pt: \n");

  // save point
  IE_inpAuxDat[iind].pos = *pt1;

  l1  = IE_GET_INP_DLI (iind);
  GL_DrawSymB (&l1, 2, SYM_CIR_S, pt1);

  return 0;

}
 

//================================================================
  int IE_cad_Inp_disp_vc (long *dli, Vector *vc1, int iind) {
//================================================================
// IE_cad_Inp_disp_vc      display vector for actual inputfield
// Input:
//   iind    fieldNr
// auxInf Vectors:
//   1.char     'D'      Vectors
//     2.char   'D'      Vectors
//       3.char '#'      index of inputField of position; '-'=none.


// see also UI_disp_vec1 GR_Disp_vc


  double    vl;
  char      *auxInf;
  Point     pt1;
  inpAuxDat *auxDat;


  auxDat = &IE_inpAuxDat[iind];
  auxInf = auxDat->auxInf;

    // DEB_dump_obj__ (Typ_VC, vc1, "IE_cad_Inp_disp_vc: |%s|",auxInf);


  // save vector in IE_inpAuxDat[iind].vx
  IE_inpAuxDat[iind].vx = *vc1;


  // get tempPos 
  // IE_get_tempPos (&pt1);
  IE_get_inpAuxPos (&pt1, auxDat->auxInf, iind);



  // vl = UT3D_len_vc (vc1);
    // printf("  vl=%f\n",vl);


  // display always same length (cannot see short vectors)
  GL_DrawSymV3 (dli, SYM_ARROW, 12, &pt1, vc1, 20.);


/*
  if(UTP_comp2db(vl, 1., 0.1)) {    // check if vl has length 1
    // att 7 = sw; Laenge 1
    // APT_disp_SymV3 (SYM_ARROW, 7, &pt1, &vc1, 10.);
    // APT_disp_Vec (7, (long)vi, &pt1, &vc1);
    // GR_Disp_vc (&vc1, &pt1, 7, 0);
    // GL_DrawSymV3 (&dli, SYM_ARROW, 7, &pt1, &vc1, 10.);
    GL_DrawSymV3 (dli, SYM_ARROW, 12, &pt1, vc1, 20.);

  } else {
    // APT_disp_Vec (2, (long)vi, &pt1, &vc1);
    // GR_Disp_vc (&vc1, &pt1, 2, 1);
    GL_DrawVec (dli, 13, &pt1, vc1);
  }
*/

  // display vec as text (overwrite)
  // remove "Save: OK or right mousebtton or Enter-key .."
  if(IE_stat_OK) UI_winTX_rmLast ();
  UI_winTX_rmLast ();
  TX_Print("....... vector %f %f %f",
           vc1->dx,vc1->dy,vc1->dz);

  DL_Redraw ();


  return 0;

}


//================================================================
  int IE_cad_Inp_disp_pln (Plane* pln1, int iind) {
//================================================================
// temp preview of Plane

  long   dli;


  // printf("IE_cad_Inp_disp_pln %d\n",iind);
  // DEB_dump_obj__ (Typ_PLN, pln1, "  pln1:");

  dli = IE_GET_INP_DLI (iind);
    // printf(" dli=%ld\n",dli);


  GL_DrawSymVX (&dli, 9, pln1, 5, 1.);   // 1=pln, 5=pln + axis


  return 0;

}


//================================================================
  int IE_cad_Inp_disp_ang (long *dli, double *ang1, int  iind) {
//================================================================
// IE_cad_Inp_disp_ang              disp angle
// auxInf Angles:
//   1.char     'A'      Angles  (AP,AN..)
//     2.char   'P'      parallel to construction-plane
//       3.char '#'      index of inputField of position; '-'=none.
//     2.char   'A'      arbitrary-angle
//       3.char '#'      index of inputField of position; '-'=none.
//       4.char '#'      index of inputField of X-vector; '-'=Default.
//       5.char '#'      index of inputField of Z-vector; '-'=Default.
//     2.char   'N'      tilt-angle / normal to construction-plane
//       3.char '#'      index of inputField of position; '-'=none.
//
// example auxInf:
// "AP0"   angle in construction-plane, position from field 0.
// "AA013" angle at pos0, vecX from 1, vecZ from 3.
// "AN0"   tilt-angle at pos0.
//         before "AN" MUST be a "AP" or "AA"-Record !

  int       i1;
  double    a1, aRot;
  char      *auxInf;
  Point     pt1;
  Vector    vx, vz, vRot;
  inpAuxDat *auxDat;


  auxDat = &IE_inpAuxDat[iind];
  auxInf = auxDat->auxInf;

    printf("IE_cad_Inp_disp_ang ang1=%lf auxInf=|%s|\n",*ang1,auxInf);


  a1 = UT_RADIANS(*ang1);





  //================================================================
  if(!strncmp(auxInf, "AN", 2)) { // "AN" tilt-angle (normal to constr.plane)
  //================================================================
    // get position
    IE_get_inpAuxPos (&pt1, auxInf, iind);

    vx = WC_sur_act.vx;
    UT3D_vc_invert (&vz, &WC_sur_act.vy);    // -Y
    // get angle of previous inputfield
    // aRot = UT_RADIANS(90.) * -1.;
    i1 = iind - 1;
    if(i1 >= 0) aRot = IE_inpAuxDat[i1].d1;
    aRot *= -1.;
      // printf(" i1=%d aRot=%lf\n",i1,aRot);

    // rotate vectors
    vRot = WC_sur_act.vz;
    UT3D_vc_rotvcangr (&vx, aRot, &vx, &vRot);
    UT3D_vc_rotvcangr (&vz, aRot, &vz, &vRot);




  //================================================================
  } else if(!strncmp(auxInf, "AA", 2)) {       // arbitrary-angle
  //================================================================
    // get position
    IE_get_inpAuxPos (&pt1, auxInf, iind);
    // get x-vector
    IE_get_inpAuxVec (&vx, auxInf, 3, iind, &pt1);
    // get z-vector
    IE_get_inpAuxVec (&vz, auxInf, 4, iind, &pt1);




  //================================================================
  } else {         // "AP"  Rotation-angle (around Z-axis, parallel to constrPln)
  //================================================================
    // get position
    IE_get_inpAuxPos (&pt1, auxInf, iind);

    vx = WC_sur_act.vx;
    vz = WC_sur_act.vz;
  }

  GL_DrawAngA (dli, 12, &pt1, &vx, &vz, &a1);


  return 0;

}
 

//======================================================================
  int IE_get_inpAuxVec (Vector *vco, char* auxInf, int ipos, int iind,
                        Point  *ptOri) {
//======================================================================
// return vector from character auxInf[ipos];
// auxInf[ipos] gives a inputFieldNr.
// inputField provides vector or point; point: use its position for a vector.

  int    ii, iTyp;
  Point  *p2;


  printf("IE_get_inpAuxVec |%s| ipos=%d iind=%d\n",auxInf,ipos,iind);

  if(strlen(auxInf) < ipos) goto L_def;
  // get inputFieldNr -> ii
  ii = ICHAR(auxInf[ipos]);
  if((ii < 0)||(ii > 9)) goto L_def;

  // get type of inputField ii
  iTyp = IE_inpTypR[ii];
    printf(" inptyp %d = %d\n",ii,iTyp);


  if(iTyp == Typ_PT) {
    // inputField ii is a point; use pto and this point for the vector
    p2 = &IE_inpAuxDat[ii].pos;
    UT3D_vc_2pt (vco, ptOri, p2);
    UT3D_vc_setLength (vco, vco, 1.);


  } else if(iTyp == Typ_VC) {
    // inputField ii is a vector;
    // if vector is empty, use default
      // DEB_dump_obj__ (Typ_VC, &IE_inpAuxDat[ii].vx, " vx:");
    if(UT3D_vc_isFree(&IE_inpAuxDat[ii].vx) == 1) goto L_def;
    // use this vector..
    *vco = IE_inpAuxDat[ii].vx;


  } else {
    printf("**** IE_get_inpAuxVec I002 %d\n",iTyp);

  }


  L_exit:
    // DEB_dump_obj__ (Typ_VC, vco, "ex IE_get_inpAuxVec");

  return 0;



  L_def:
    // get default for undefined vector
    if     (ipos == 3) *vco = WC_sur_act.vx;
    else if(ipos == 4) *vco = WC_sur_act.vz;
    goto L_exit;

}


//================================================================
  int IE_get_inpAuxPos (Point *pt1, char* auxInf, int iind) {
//================================================================
// character 3 of auxInf gives the index of the positionPoint
//   '0' gives IE_inpAuxDat[0].pos;
// see also IE_get_tempPos


  int    ii;

  // printf("IE_get_inpAuxPos |%s| %d\n",auxInf,iind);


  if(!auxInf) goto L_SC;
  if(strlen(auxInf) < 3) goto L_SC;

  // *pt1 = sele_get_pos__ (pt1);

    // printf(" chr-3=%d\n",auxInf[2]);

  ii = ICHAR(auxInf[2]);
  if(ii < 0) goto L_SC;       // eg from '-'
  if(ii >= iind) goto L_SC;
  *pt1 = IE_inpAuxDat[ii].pos;

    // DEB_dump_obj__ (Typ_PT, pt1, "ex IE_get_inpAuxPos");

  return 0;


 
  L_SC:
  *pt1 = GL_GetCen();     // get ScreenCenter
  return 0;


  // L_err1:
    // TX_Print("**** IE_get_inpAuxPos E%d",ii);
    // return -1;

}


//================================================================
  int IE_get_inpInd () {
//================================================================
 
  return IE_inpInd;

}


//================================================================
  int IE_cad_Inp_disp_val (char *sSrc) {
//================================================================
// IE_cad_Inp_disp_val             display value in messageWin
// see also IE_cad_Inp_disp__

  int       irc;
  double    *dp1;
  ObjAto    ato1;


  // printf("IE_cad_Inp_disp_val |%s|\n",sSrc);

  // get memSpc for ato1 (memspc53, 54, 55)
  ATO_getSpc__ (&ato1);

  // get atomicObjects from sourceLine p1; full evaluated.
  irc = ATO_ato_srcLn__ (&ato1, sSrc);
  if(irc < 0) {printf(" IE_cad_Inp_disp_val I1\n"); return 0;}
    // ATO_dump__ (&ato1, "_Inp_disp_val-1");
    // printf(" ato.typ[0] = %d ato.val[0] = %lf\n",ato1.typ[0],ato1.val[0]);

  if(TYP_IS_VAL(ato1.typ[0])) {
    if(IE_inpStat == 2) UI_winTX_rmLast ();
    TX_Print ("  value: %lf ..",ato1.val[0]);
  }

  IE_inpStat = 2;


  return 0;
  
}   
    

//=====================================================================
  int IE_cad_Inp_disp__ (int iind, int mode) {
//=====================================================================
// display temp. symbol for inputField iind

// Input:
//   iind    fieldNr
//   mode   -1 = leave field
//           0 = update field
//           1 = enter field
// Output:
//   retCod  0=OK;
//          -1=input-not-valid 

// DispListIndexes:
//   for CAD-inputFields:  get (fixed) index from IE_GET_INP_DLI ("-iind - 2;"
//   for outputObject (WC_Work1):  DLI_TMP (-8L)

// OFFEN: IE_decode_Ln und danach APT_obj_expr ist Mist; es sollte
//        eine APT_obj_expr-Version die schon aus_typ, aus_tab mitbringt.
// see also DL_disp_hili AP_hili_obj

  int       aus_typ[10], aus_anz, irc, i1, rTyp, oTyp, dbTyp, oNr, form,
            iatt = Typ_Att_top2;    // Typ_Att_hili1;
  long      dbi=0L, dli, dynPti;
  double    d1;
  char      aus_tab[10][256], subTyp1;  // 10 strings a 256 chars
  char      *p1, os1[OBJ_SIZ_MAX];
  void      *op1;
  Point     pt1, *ppt=NULL;
  Vector    vc1;
  ObjAto    ato1;
  ObjGX     ox1; // = OGX_NUL;
  // Memspc    tSpc1;



  // printf("IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII\n");
  // printf("IE_cad_Inp_disp__ iind=%d mode=%d\n",iind,mode);


  if(iind < 0) return 0;


  // requested obj
  rTyp = IE_inpTypR[iind];
    // printf(" rTyp=%d\n",rTyp);


  //----------------------------------------------------------------
  // ignore objects not displayable
  if(rTyp == Typ_Txt)      return 0;  // skip text  2016-10-05
  if(rTyp == Typ_SubModel) return 0;  // skip Submodel  2018-06-24
  if(rTyp == TYP_FilNam)   return 0;
  if(rTyp == Typ_CtlgPart) return 0;
  if(rTyp == Typ_PTS) return 0;
  if(rTyp == Typ_PTI) return 0;
  // if(TYP_IS_VAL(rTyp))     return 0;  // ignore values


  //----------------------------------------------------------------
  // get (fixed) dli for active inputF  -iind - 2; first =-2L, next -3L, ..
  dli = IE_GET_INP_DLI (iind);
    // printf(" dli=%ld\n",dli);


  // get inputFieldText
  p1 = IE_get_inp__(iind);
  if(strlen(p1) < 1) {
    // cad-inputField is empty
    GL_temp_del_1 (dli);            // delete cad-inputFieldObject
    return 0;
  }


  // change to uppercase (not for TYP_FilNam)
  if((rTyp != Typ_String))      {           // eg auxText for Notes
    strcpy (IE_outTxt, p1);
    i1 = UTX_chg_2_upper (IE_outTxt);
      // printf(" _2_upper %d |%s|\n",i1,IE_outTxt);
    if(i1) {
      // update inputfield
      GUI_entry_set (&IE_wCad_obj[iind], IE_outTxt);
      GUI_update__();
    }
    p1 = IE_get_inp__(iind);
  }


    // printf(" _Inp_disp__ iind=%d rTyp=%d |%s|\n",iind,rTyp,p1);



  //----------------------------------------------------------------
  // skip some types ..

  // skip GROUP                         // 2013-03-16
  if(!strcmp(p1, "<GROUP>")) return 0;


  // skip Typ_EyePT
  if(rTyp == Typ_EyePT) {
    IE_getEyePt ();
    return 0;
  }


  //----------------------------------------------------------------
  // get memSpc for ato1 (memspc53, 54, 55)
  ATO_getSpc__ (&ato1);


  // save state of dyn-points
  // DB_dyn__ (0, Typ_PT, 0L);
  dynPti = DB_dyn__ (0, Typ_PT, 0L);       // save state of dyn-points


  // get atomicObjects from sourceLine p1; full evaluated.
  irc = ATO_ato_srcLn__ (&ato1, p1);
  if(irc < 0) goto L_err_dyn;
    // ATO_dump__ (&ato1, "_Inp_disp__-1");


  // get outTyp; eg Typ_VC from requestedTyp - eg Typ_goGeo7
  oTyp = IE_inpCkTyp (iind, p1, &ato1);
    // printf(" _inpCkTyp oTyp=%d\n",oTyp);


  //----------------------------------------------------------------
  // skip not-displayable inputTypes
  // if((oTyp == Typ_XVal)       ||
     // (oTyp == Typ_YVal)       ||
     // (oTyp == Typ_ZVal))          goto L_err_dyn;
  // // geom.parameters - can view Typ_Angle
  // if(TYP_IS_GEOMPAR(typ)) {
    // if(typ != Typ_Angle) return 0;          // skip modifiers
  // }

  // cannot display Typ_mod1, Values ..
  if(TYP_IS_MOD(oTyp)) goto L_err_dyn;

  if((oTyp == Typ_String)     ||
     (oTyp == TYP_FilNam)     ||
     (oTyp == Typ_SubModel)   ||   
     (oTyp == Typ_CtlgPart))      goto L_err_dyn;

  if((oTyp == Typ_Val)   ||
     (oTyp == Typ_VAR)   ||
     (oTyp == Typ_Val)   ||
     (oTyp == Typ_XVal)  ||
     (oTyp == Typ_YVal)  ||
     (oTyp == Typ_ZVal))     {

    TX_Print(".. inputfield %d value is %lf", iind + 1, ato1.val[0]);
    goto L_done;
  }


  //================================================================
  // get requested typ oTyp from given typ ato1.typ[0]
// TODO make ATO-func giving requested typ from given bjects ..

  //----------------------------------------------------------------
  // if(oTyp=Typ_PT and ato1=Typ_Val(eg from "0 0 -5")): change ato1 to Typ_PT
  // (create dynamic point from "0 0 -5")
  if(oTyp == Typ_PT) {
    if(ato1.typ[0] != Typ_PT) {
      // goto L_disp;
      irc = ATO_ato_atoTab__ (&oTyp, &d1, &ato1);
            // ATO_dump__ (&ato1, "_Inp_disp__-2");
      if(irc < 0) goto L_err_dyn;
      ato1.typ[0] = Typ_PT;
      ato1.val[0] = d1;
        // printf(" Inp_disp__-_atoTab__ irc=%d otyp=%d d1=%f\n",irc,oTyp,d1);
    }

  //----------------------------------------------------------------
  // if(oTyp=Typ_VC and ato1=Typ_Val(eg from "0 0 1")): change ato1 to Typ_VC
  // (create dynamic vector from "0 0 1")
  } else if(oTyp == Typ_VC) {
    // for ato1=3-vals, otyp=vec create dynamic-vec
    if(ato1.typ[0] != Typ_VC) {
      irc = ATO_ato_atoTab__ (&oTyp, &d1, &ato1);
      if(irc < 0) goto L_err_dyn;
      ato1.typ[0] = Typ_VC;
      ato1.val[0] = d1;
        // printf(" Inp_disp__-_atoTab__ irc=%d otyp=%d d1=%f\n",irc,oTyp,d1);
    }

  //----------------------------------------------------------------
  // if(oTyp=Typ_LN and ato1=Typ_PT: change ato1 to Typ_VC
  // (create dynamic line)
  } else if(oTyp == Typ_LN) {
    // for ato1=3-vals, otyp=vec create dynamic-vec
    if(ato1.typ[0] != Typ_LN) {
      irc = ATO_ato_atoTab__ (&oTyp, &d1, &ato1);
      if(irc < 0) goto L_err_dyn;
      ato1.typ[0] = Typ_LN;
      ato1.val[0] = d1;
        // printf(" Inp_disp__-_atoTab__ irc=%d otyp=%d d1=%f\n",irc,oTyp,d1);
    }

  //----------------------------------------------------------------
  } else if(oTyp == Typ_PLN) {
    if(ato1.typ[0] != Typ_PLN) {
      irc = ATO_ato_atoTab__ (&oTyp, &d1, &ato1);
      if(irc < 0) goto L_err_dyn;
      ato1.typ[0] = Typ_PLN;
      ato1.val[0] = d1;
        // printf(" Inp_disp__-_atoTab__ irc=%d otyp=%d d1=%f\n",irc,oTyp,d1);
    }
  }

    // ATO_dump__ (&ato1, "_Inp_disp__-3");

    



  //================================================================
  // get op1 = dataStruct from ato (see DBO_dbo_src__)
  if(TYP_IS_DBO(oTyp)) {             // test for DB-obj; not eg Typ_Val
      // printf(" Inp_disp__-is-dbo; yes\n");
    // yes, input is existig DB-obj; get it ..
    dbi = ato1.val[0];
    dbTyp = ato1.typ[0];
    form = DB_GetObjDat (&op1, &oNr, ato1.typ[0], dbi);
    if(form < 0) goto L_err_inp;
      // printf("  _cad_Inp_disp %d oTyp=%d |%s| dli=%ld\n",iind,oTyp,p1,dli);
      // printf(" form=%d oNr=%d\n",form,oNr);
      // DEB_dump_obj__ (form, op1, "op1");

  } else {
    // get space for PLN, Ang
    op1 = os1;
  }

  // display binObj form,op1

  //----------------------------------------------------------------
  L_disp:
      // printf(" _Inp_disp__ oTyp=%d\n",oTyp);


  if(oTyp == Typ_PT) {
    IE_cad_Inp_disp_pt ((Point*)op1, iind);
    goto L_done;


  //----------------------------------------------------------------
  } else if(oTyp == Typ_LN) {
    goto L_draw_ln;


  //----------------------------------------------------------------
  } else if(oTyp == Typ_CI) {
    // save selectionPoint
    sele_get_pos__ (&pt1);
    IE_inpAuxDat[iind].pos = pt1;
    goto L_draw_ac;


  //----------------------------------------------------------------
  } else if(oTyp == Typ_VC) {
    // ox1 = UTO_ox_dbo (form, dbi);
    OGX_SET_OBJ (&ox1, dbTyp, form, 1, op1);
    UT3D_ptvc_ox (&pt1, &vc1, &ox1);
    IE_cad_Inp_disp_vc (&dli, &vc1, iind);
    goto L_done;


  //----------------------------------------------------------------
  } else if(oTyp == Typ_Angle) {
    // save angle
    // IE_inpAuxDat[iind].d1 = UT_RADIANS(*(double*)op1);
    irc = APT_obj_ato ((double*)op1, oTyp, &ato1);
    if(irc < 0) return -1;
    IE_cad_Inp_disp_ang (&dli, (double*)op1, iind);
    goto L_done;


  //----------------------------------------------------------------
  } else if(oTyp == Typ_Tra) {

    if(((ObjGX*)op1)->form == Typ_VC) {
      // display vector
      IE_cad_Inp_disp_vc (&dli, (Vector*)(((ObjGX*)op1)->data), iind);

    } else {
      // display TraRot
      GL_Draw_tra (&dli, 12, (TraRot*)(((ObjGX*)op1)->data));
    }

    goto L_done;


  //----------------------------------------------------------------
  } else if((oTyp == Typ_CV)      ||
            (oTyp == Typ_CVBSP)   ||
            (oTyp == Typ_CVRBSP)  ||
            (oTyp == Typ_CVPOL)   ||
            (oTyp == Typ_CVELL)   ||
            (oTyp == Typ_CVCLOT)  ||
            (oTyp == Typ_CVTRM))      {
    // display curves dashed-red-thick2
    goto L_draw_cv;


/*
  //----------------------------------------------------------------
  } else if((typ == Typ_goPrim)  ||
            (typ == Typ_goGeo4)  ||
            (typ == Typ_goGeo7)  ||
            (typ == Typ_goGeo8))   {

    irc = IE_decode_Ln (aus_typ, (void*)aus_tab, 10, p1);
    if(irc < 0) goto L_err;

    typ = aus_typ[0];
      printf(" typ[0]=%d\n",typ);
    irc = APT_obj_expr ((void*)o1, typ, p1);  // "D24 REV"
    if(irc < 0) goto L_err;

    if(typ == Typ_VC) {
      UT3D_vc_setLength ((Vector*)o1, (Vector*)o1, 1.);
      goto L_draw_vc;

    } else if(typ == Typ_Val) {   // Typ_Angle
      d1 = UT_RADIANS(*((double*)o1));
      UT3D_vc_angr ((Vector*)o1, d1);
      goto L_draw_vc;

    } else if(typ == Typ_LN) {
      goto L_draw_ln;

    } else if(typ == Typ_CI) {
      // save selectionPoint
      sele_get_pos__ (&pt1);
      IE_inpAuxDat[iind].pos = pt1;
      goto L_draw_ac;

    } else if(typ == Typ_CV) {
      goto L_draw_cv;

    } else return 0;

  //----------------------------------------------------------------
  } else if(typ == Typ_goAxis)  {
    // sollte als Vektor auf fixem Punkt angezeigt werden;
    // C() od L() od nix (zB "P20 [L()]"; als Line resolven)

    // irc = IE_decode_Ln (aus_typ, (void*)aus_tab, 10, p1);
    // if(irc < 0) goto L_err;
    // typ = aus_typ[0];

    typ = AP_typ_typChar (p1[0]);
    if(typ == Typ_PT) {
      // kann sein PT+PT|PT+LN|PT+VC; wenn nur der Punkt: nix tun ..
      if(UTX_wordnr(p1) < 2) return 0;
      typ = Typ_LN;
    }

    // irc = APT_obj_expr ((void*)o1, typ, aus_tab[0]);
    irc = APT_obj_expr ((void*)o1, typ, p1);
    if(irc < 0) goto L_err;

    if(typ == Typ_LN) {
      pt1 = ((Line*)o1)->p1;
      UT3D_vc_2pt (&vc1, &((Line*)o1)->p1, &((Line*)o1)->p2);
    } else if(typ == Typ_CI) {
      pt1 = ((Circ*)o1)->pc;
      vc1 = ((Circ*)o1)->vz;
    } else if(typ == Typ_PLN) {
      pt1 = ((Plane*)o1)->po;
      vc1 = ((Plane*)o1)->vz;
    } else return 0;

    GL_DrawVc1 (&dli, 12, &pt1, &vc1);

    goto L_done;
*/



  //----------------------------------------------------------------
  } else if(oTyp == Typ_PLN)  {
    // irc = APT_obj_expr ((void*)o1, Typ_PLN, p1);
    // if(irc < 0) goto L_err_inp;
    irc = APT_obj_ato ((Plane*)op1, oTyp, &ato1);
    if(irc < 0) return -1;
    IE_cad_Inp_disp_pln ((Plane*)op1, iind);
    goto L_done;



  //----------------------------------------------------------------
  } else {
    printf("**** IE_cad_Inp_disp__ %d %d\n",oTyp,form);
    goto L_err_dyn;
  }



  //----------------------------------------------------------------
  L_draw_ln:
      // DEB_dump_obj__ (Typ_LN, op1, " L_draw_ln:temp.line:");
    GL_DrawLine (&dli, iatt, (Line*)op1);
    goto L_done;

  L_draw_ac:
    // DEB_dump_obj__ (Typ_CI, o1, "");
    GR_DrawCirc (&dli, 0L, iatt, op1);
    goto L_done;

  L_draw_cv:
    // if its dbo eg Typ_CVTRM we have data
    if(TYP_IS_DBO(oTyp)) oTyp = form;
    GR_Draw_obj (&dli, dbi, oTyp, op1, oNr, iatt, 0);
    goto L_done;

  L_draw_vc:
    IE_cad_Inp_disp_vc (&dli, (Vector*)op1, iind);
    // GL_DrawVc1 (&dli, 12, ppt, o1);
    // GL_DrawVc1 (&dli, 12, NULL, o1);
    // UI_disp_vec1 (Typ_Txt, p1);




  //----------------------------------------------------------------
  L_done:
  // check for depending input-fields
  IE_inp_ck_dep (iind);


  DL_Redraw ();
  // DB_dyn__ (1, Typ_PT, 0L);    // reset to previous saved
  DB_dyn__ (2, Typ_PT, dynPti);   // reset state of dyn-points



  L_exit:
    // printf(" ex-IE_cad_Inp_disp__\n");

  return 0;


  //----------------------------------------------------------------
  L_err_dyn:
    DB_dyn__ (2, Typ_PT, dynPti);   // reset to previous saved
    return -1;

  L_err:
    // Objekt unvollstaendig; Reset Error.
    // AP_errStat_set (0);
    IE_cad_Inp1_Info(iind);
    // TX_Print(" ****** obj not complete *******");
    return -1;

  
  L_err_inp:
    // parameters error
    TX_Error("**** parameters error in inputfield %d",iind);
    DL_Redraw ();
    return -1;

}


//================================================================
  int IE_inp_ck_dep (int iind) {
//================================================================
// check for depending input-fields
//  check if temporary-displayObjects of other inputFields depend on
//  inputField [iind]; if yes: redraw this temporary-displayObjects.
//  Temporary-displayObjects: eg IE_cad_Inp_disp_ang


  int       i1, i2, i3, ii;
  char      *auxInf;
  inpAuxDat *auxDat;


  // printf("IE_inp_ck_dep %d\n",iind);

  // loop tru all fields; check if field <iind> is used;
  // if yes: reEvaluate with IE_cad_Inp_disp__.

  for(i1=0; i1<IE_inpAnz; ++i1) {
    if(i1 == iind) continue;
    // get auxInf
    auxDat = &IE_inpAuxDat[i1];
    auxInf = auxDat->auxInf;
    i3 = strlen(auxInf) - 2;
    if(i3 < 1) continue;
      // printf(" deps.auxInf[%d] = |%s|\n",i1,&auxInf[2]);

    // loop tru characters 2-n
    for(i2=0; i2<i3; ++i2) {
      ii = ICHAR(auxInf[i2 + 2]);
      if(ii != iind) continue;
      IE_cad_Inp_disp__ (i1, 0);
    }

  }

    // printf("ex-IE_inp_ck_dep %d\n",iind);

  return 0;

}


//=====================================================================
  int IE_cad_p_CB (void *parent, void **data) {
//=====================================================================
// aktivate Point (ein PointMenueEintrag wurde selected)

  // check inputsfields if empty; if not query continue|not
  if(IE_inp_ck_del()) return 0;

  menSubGrpInd = GUI_DATA_I1;

  DL_disp_reset (ED_get_lnr_act ());
  DL_Redraw ();

  IE_cad_init2 (IE_Func_Points); // 0=PT

  IE_cad_init1 (menSubGrpInd);

  return 0;

}

//=====================================================================
  int IE_cad_l_CB (void *parent, void **data) {
//=====================================================================
// aktivate LineFunction (ein LineMenueEintrag wurde selected)

  // check inputsfields if empty; if not query continue|not
  if(IE_inp_ck_del()) return 0;

  menSubGrpInd = GUI_DATA_I1;

  DL_disp_reset (ED_get_lnr_act ());
  DL_Redraw ();

  IE_cad_init2 (IE_Func_Lines);

  IE_cad_init1 (menSubGrpInd);

  return 0;

}

//=====================================================================
  int IE_cad_c_CB (void *parent, void **data) {
//=====================================================================

  // check inputsfields if empty; if not query continue|not
  if(IE_inp_ck_del()) return 0;

  menSubGrpInd = GUI_DATA_I1;

  DL_disp_reset (ED_get_lnr_act ());
  DL_Redraw ();

  IE_cad_init2 (IE_Func_Circles);

  IE_cad_init1 (menSubGrpInd);

  return 0;

}

//=====================================================================
  int IE_cad_r_CB (void *parent, void **data) {
//=====================================================================

  // check inputsfields if empty; if not query continue|not
  if(IE_inp_ck_del()) return 0;

  menSubGrpInd = GUI_DATA_I1;

  DL_disp_reset (ED_get_lnr_act ());
  DL_Redraw ();

  IE_cad_init2 (IE_Func_Planes);

  IE_cad_init1 (menSubGrpInd);

  return 0;

}

//=====================================================================
  int IE_cad_d_CB (void *parent, void **data) {
//=====================================================================

  // check inputsfields if empty; if not query continue|not
  if(IE_inp_ck_del()) return 0;

  menSubGrpInd = GUI_DATA_I1;

  DL_disp_reset (ED_get_lnr_act ());
  DL_Redraw ();

  IE_cad_init2 (IE_Func_Vectors);

  IE_cad_init1 (menSubGrpInd);

  return 0;

}

//=====================================================================
  int IE_cad_v_CB (void *parent, void **data) {
//=====================================================================

  // check inputsfields if empty; if not query continue|not
  if(IE_inp_ck_del()) return 0;

  menSubGrpInd = GUI_DATA_I1;

  DL_disp_reset (ED_get_lnr_act ());
  DL_Redraw ();

  IE_cad_init2 (IE_Func_Values);

  IE_cad_init1 (menSubGrpInd);

  return 0;

}


//================================================================
  int IE_inp_ck_del () {
//================================================================
/// IE_inp_ck_del        dialog canel unstored input
// check inputsfields if empty; if not return
// test also OK-button
// retCod: 0  delete input
//         1  no, keep old input


  char  s1[128];

  // check OK-button-status; inaktiv: return 0
  if(!IE_stat_OK) return 0;

  // check inputsfields if empty; if yes return 0
  // if(IE_inp_ck_empty()) return 0;

  strcpy(s1, "   input not stored, delete input ?  ");

  return GUI_Dialog_2b (s1, "YES", "NO");

}


//=====================================================================
  int IE_cad_s_CB (void *parent, void **data) {
//=====================================================================

  // check inputsfields if empty; if not query continue|not
  if(IE_inp_ck_del()) return 0;

  menSubGrpInd = GUI_DATA_I1;

  DL_disp_reset (ED_get_lnr_act ());
  DL_Redraw ();

  IE_cad_init2 (IE_Func_Curves);

  IE_cad_init1 (menSubGrpInd);

  return 0;

}

//=====================================================================
  int IE_cad_a_CB (void *parent, void **data) {
//=====================================================================

  // check inputsfields if empty; if not query continue|not
  if(IE_inp_ck_del()) return 0;

  menSubGrpInd = GUI_DATA_I1;

  DL_disp_reset (ED_get_lnr_act ());
  DL_Redraw ();

  IE_cad_init2 (IE_Func_Surfaces);

  IE_cad_init1 (menSubGrpInd);

  return 0;

}

//=====================================================================
  int IE_cad_b_CB (void *parent, void **data) {
//=====================================================================

  // check inputsfields if empty; if not query continue|not
  if(IE_inp_ck_del()) return 0;

  menSubGrpInd = GUI_DATA_I1;

  DL_disp_reset (ED_get_lnr_act ());
  DL_Redraw ();

  IE_cad_init2 (IE_Func_Solids);

  IE_cad_init1 (menSubGrpInd);

  return 0;

}

//=====================================================================
  int IE_cad_n_CB (void *parent, void **data) {
//=====================================================================

  // check inputsfields if empty; if not query continue|not
  if(IE_inp_ck_del()) return 0;

  menSubGrpInd = GUI_DATA_I1;

  DL_disp_reset (ED_get_lnr_act ());
  DL_Redraw ();

  IE_cad_init2 (IE_Func_Note); // 9

  IE_cad_init1 (menSubGrpInd);

  return 0;

}

//=====================================================================
  int IE_cad_m_CB (void *parent, void **data) {
//=====================================================================

  // check inputsfields if empty; if not query continue|not
  if(IE_inp_ck_del()) return 0;

  menSubGrpInd = GUI_DATA_I1;

  DL_disp_reset (ED_get_lnr_act ());
  DL_Redraw ();

  IE_cad_init2 (IE_Func_Models);

  IE_cad_init1 (menSubGrpInd);

  return 0;

}

//=====================================================================
  int IE_cad_t_CB (void *parent, void **data) {
//=====================================================================

  // check inputsfields if empty; if not query continue|not
  if(IE_inp_ck_del()) return 0;

  menSubGrpInd = GUI_DATA_I1;

  DL_disp_reset (ED_get_lnr_act ());
  DL_Redraw ();

  IE_cad_init2 (IE_Func_Transform);

  IE_cad_init1 (menSubGrpInd);

  return 0;

}

//=====================================================================
  int IE_cad___CB (void *parent, void **data) {
//=====================================================================

  // check inputsfields if empty; if not query continue|not
  if(IE_inp_ck_del()) return 0;

  menSubGrpInd = GUI_DATA_I1;

  DL_disp_reset (ED_get_lnr_act ());
  DL_Redraw ();

  IE_cad_init2 (IE_Func_Modify); // 12

  IE_cad_init1 (menSubGrpInd);

  return 0;

}

/*
//=====================================================================
  int IE_cad_z_CB (void *parent, void **data) {
//=====================================================================

  menSubGrpInd = GUI_DATA_I1;

  DL_disp_reset (ED_get_lnr_act ());
  DL_Redraw ();

  IE_cad_init2 (IE_Func_Analyze);

  IE_cad_init1 (menSubGrpInd);

  return 0;

}
*/

//=====================================================================
  int IE_cad_i_CB (void *parent, void **data) {
//=====================================================================

  // check inputsfields if empty; if not query continue|not
  if(IE_inp_ck_del()) return 0;

  menSubGrpInd = GUI_DATA_I1;

  DL_disp_reset (ED_get_lnr_act ());
  DL_Redraw ();

  IE_cad_init2 (IE_Func_Interact);

  IE_cad_init1 (menSubGrpInd);

  return 0;

}

//=====================================================================
  int IE_cad_Ace_CB (void *parent, void **data) {
//=====================================================================

  // check inputsfields if empty; if not query continue|not
  if(IE_inp_ck_del()) return 0;

  menSubGrpInd = GUI_DATA_I1;

  DL_disp_reset (ED_get_lnr_act ());
  DL_Redraw ();

  IE_cad_init2 (IE_Func_CADEnv); // 14 ConstrPlane ..

  IE_cad_init1 (menSubGrpInd);

  return 0;

}

//=====================================================================
  int IE_auxFun1 (char **obuf, char **tbuf, IE_rec_txt *ibuf) {
//=====================================================================
// get pointers to functionDescriptions (.ftxt) into obuf
// max. 50 entries,
// optText max 60 bytes,     (obuf >= 5k)
// tipText max 512 bytes,    (tbuf >= 30k)

  int    i1, il;
  char   *p1, *tPos;


  obuf[0] = '\0';
  tPos = (char*)&tbuf[50];


  for(i1=0; i1<50; ++i1) {
    if(ibuf[i1].ftxt[0] == '\0') {
      obuf[i1] = NULL;
      break;
    }
    // printf(" %d |%s|\n",i1,ibuf[i1].ftxt);
    obuf[i1] = ibuf[i1].ftxt;

    // tbuf[i1] = MSG_get_str (obuf[i1]);

    // get next tipText from its key (= menuItemText) and stor -> tbuf.
    tbuf[i1] = tPos;
    p1 = MSG_get_str (obuf[i1]);
    if(p1) {
      il = strlen(p1);
      strncpy (tPos, p1, il);
    } else il = 0;
    tPos[il] = '\0';
    ++il;
    tPos += il;
      // printf(" _auxFun1[%d] |%s|%s|\n",i1,obuf[i1],tbuf[i1]);

  }


  return 0;

}



//=====================================================================
  int IE_inp_set_menu_col (int mode, int ind, char *txt) {
//=====================================================================
// modify colour of menu
// input:
//   mode = 0: reset; 1=set Color.
//   ind    ?

  MemObj *bact;


  // printf("CCCCCCC IE_inp_set_menu_col mode=%d ind=%d\n",mode,ind);


  bact = &IE_wCad_men[ind];

  if(mode == 0) {
    // GUI_mod_styl (bact, 0);
    GUI_optmen_set (bact, 0, txt);

  } else {
    // GUI_mod_styl (bact, 2);
    GUI_optmen_set (bact, 1, txt);
  }


  return 0;

}


//================================================================
  int IE_parDef_ck (int ind) {
//================================================================
// IE_parDef_ck                 check if two consecutive parameters have same type
//   compare types IE_cad_act[ind].typ / IE_cad_act[ind+1].typ
// retCode       1=different-types;  0=identical-types
 
  int   i2;

  i2 = ind + 1;

  // printf("IE_parDef_ck ind=%d typ=%d nxtTyp=%d\n",ind,
         // IE_cad_act[ind].typ,IE_cad_act[i2].typ);


  if(IE_cad_act[ind].typ != IE_cad_act[i2].typ) return 1;  // different

  return 0;   // identical-types;

}


//================================================================
  int IE_parDef_get (char *so, int typ) {
//================================================================
// IE_parDef_get               get default-value for type <typ>
// was IE_parDef

  // printf("IE_parDef_get %d\n",typ);


  if(typ == Typ_Val) {
    strcpy(so, "0");

  } else if(typ == Typ_Angle) {
    strcpy(so, "ANG(0)");

  } else {
    strcpy(so, "??");
    TX_Print("********* IE_parDef_get %d\n",typ);
  }


  return 0;

}

 
/*
//================================================================
  int IE_parDef (int mode, char *cbuf, int ind) {
//================================================================
// check for Parameter-defaults to save
// Input:
//   mode = 0: preset with saved data; 1: save data
//   ind   inputField-nr
// RetCod =  0: preloaded
//          -1: nothing to preload


  static char defRad[16]={"10"};
  // static int  defTra=0;

  int    irc, i1, typ;
  long   il;
  char   *inf, *p1;
  Vector vc1;


  typ = IE_cad_act[ind].typ;
  inf = IE_cad_act[ind].info;

  if(inf[0] == '[') ++inf;  // skip '['


  // printf("IE_parDef mode=%d |%s| ind=%d\n",mode,cbuf,ind);
  // printf(" IE_inpInd=%d\n",IE_inpInd);
  // printf(" preLoad=|%s|\n",IE_wCad_preLoad[IE_inpInd]);
  // printf("  typ=%d\n",typ);
  // printf("  info=|%s|\n",inf);



  // check for preloadtext
  if(mode == 0) {
    if(strlen(cbuf) > 0) goto L_normal;  // preload only if field is empty
    p1 = IE_wCad_preLoad[IE_inpInd];
    if(p1 == NULL) goto L_normal;        // no preloadtext for this field
    strcpy(cbuf, p1);                    // copy preloadtext out
    return 0;
  }




  //----------------------------------------------------------------
  L_normal:
  irc = -1;



  if(typ == Typ_mod1) {
    if(mode < 1) {
      // wenn mod optional, nix tun
      if(IE_cad_act[ind].info[0] != '[') {strcpy(cbuf, "1"); irc = 0;}
    }


  //----------------------------------------------------------------
  } else if (typ == Typ_Val) {

    if (!strcmp(inf, "Radius")) {
      if(mode < 1) {strcpy(cbuf, defRad); irc = 0;}
      else strcpy(defRad, cbuf);


    } else if (!strcmp(inf, "Degree")) {
      if(mode < 1) {strcpy(cbuf, "2"); irc = 0;}


    } else if (!strcmp(inf, "Toleranz")) {
      if(mode < 1) {strcpy(cbuf, "0.005"); irc = 0;}
    }


  // //----------------------------------------------------------------
  // } else if (typ == Typ_VC) {
// 
    // if (!strncmp(inf, "Z-Axis", 6)) {
      // if(mode < 1) {   // preset
        // // aktuellen ConstrPlane-Z-vektor holen
        // strcpy(cbuf, AP_Get_ConstPl_Z(Typ_VC)); irc = 0;
      // }
    // }


    // // display bei save
    // if(mode > 0) {   // save
      // if(strlen(cbuf) > 1) UI_disp_vec1 (Typ_Txt, cbuf);
      // // if(strlen(cbuf) > 1) {
        // // printf(" dispVC |%s|\n",cbuf);
        // // APT_obj_expr (&vc1, Typ_VC, cbuf);
        // // UI_disp_vec1 (Typ_VC, &vc1);
      // // }
    // }



  //----------------------------------------------------------------
  } else if (typ == Typ_Tra) {

    if(mode < 1) {     // preset
      il = DB_dbo_get_free (Typ_Tra, 0);
      if(il > 0) {
        sprintf(cbuf, "T%ld", il); irc = 0;

        // wird dzt auch von WC_Work1 ausgageben; sollte hier und in WC_Work1
        // weg und nur in IE_cad_Inp_disp__
        // UI_disp_vec2(il);
      }
    }


  //----------------------------------------------------------------
  } else if (typ == Typ_PLN) {


    if(mode < 1) {     // preset
      // fuer "Reset ConstrPlane"
      if(!strcmp(inf, "Abs.Refsys")) {
        strcpy(cbuf, "R0");
        return 1;
        // IE_set_inp (0, "R0");    // preset entry0
        // IE_inp_chg (1);        // gleich ins naechste Inputfeld ...
        // irc = 0;
      }

        // // aktuelle ConstrPln holen ...
        // strcpy(cbuf, AP_Get_ConstPl_Z(0)); irc = 0;

    }


  //----------------------------------------------------------------
  }




  return irc;

}
*/


//================================================================
  int IE_joints_import () {
//================================================================
// after creating subModel add objects at all its exported joints.
// create lines like "P<next> = IMP M<IE_objInd> J#"

  int      iUp, i1, grpNr, sizTab, oTyp, *jTab, ibmNr;
  long     lNr, oInd;
  char     oNam[32], s1[240];
  Memspc   oSpc;


  // printf("IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII \n");
  // printf("IE_joints_import IE_objInd=%ld\n",IE_objInd);


  // get list of all joints for subModel <IE_objInd>
  jTab = (int*)memspc50;
  sizTab = sizeof(memspc50) / sizeof(int);
  JNT_imp_all (jTab, &sizTab, IE_objInd);


  grpNr = 1;

  for(i1=0; i1<sizTab; ++i1) {
     // printf(" nxt jnt %d %d\n",i1,jTab[i1]);

    // get next joint-record
    UME_init (&oSpc, memspc201, sizeof(memspc201));
    JNT_imp__ (&oSpc, &ibmNr, IE_objInd, jTab[i1]);

    // get type of output-object for this joint-record
    oTyp = JNT_oTyp (&oSpc);
    if(oTyp == Typ_Error) {
      TX_Print ("IE_joints_import E001 J%d",jTab[i1]);
      continue;
    }

    // update new object oSpc (note: get new text ..)
    iUp = JNT_imp_upd (s1, sizeof(s1), &oSpc, jTab[i1]);

    // get next free index for oTyp
    oInd = DB_QueryNxtFree (oTyp, 20);

    // fix outObjName
    APED_oid_dbo__ (oNam, oTyp, oInd);

    // fix complete sourceline
    sprintf(IE_outTxt, "%s=IMP M%ld J%d", oNam, IE_objInd, jTab[i1]); 

    if(iUp) strcat(IE_outTxt, s1);
      // printf(" code = |%s|\n",IE_outTxt);

    // save objects to DB & display; eg "P19=IMP M25 J3"
    ED_srcLn_grp_add (IE_outTxt, &grpNr);

    // update browserWindow
    Brw_objRow_upd (oTyp, oInd);
  }

  UNDO_upd__ ();



  // display objects


  return 0;

}


/* UNUSED:
//=======================================================================
  int IE_popup_planes__ () {
//=======================================================================
// build List of ConstrPlanes and display as popup-list


#define LstSiz  200   // Anzahl Listenelemente
  char     *LstPtr[LstSiz];
  char     LstBuf[LstSiz][32];
  int      i1, i2;
  long     l1;


  printf("IE_popup_planes__\n");


  strcpy(LstBuf[0], "RZ");
  strcpy(LstBuf[1], "RY");
  strcpy(LstBuf[2], "RX");

  LstPtr[0]=LstBuf[0];
  LstPtr[1]=LstBuf[1];
  LstPtr[2]=LstBuf[2];
  LstPtr[3]=NULL;

  // DB_GetRef
  i1 = 3;
  for(l1=1; l1<1000000; ++l1) {
    i2 = DB_CkRef (l1);
    if(i2 == -2) break;  // out of range
    if(i2 >= 0) {
      sprintf(LstBuf[i1],"R%ld",l1);
      LstPtr[i1]=LstBuf[i1];
        // printf(" +Ref[%d] |%s| %ld\n",i1,LstBuf[i1],l1);
      ++i1;
      if(i1 >= LstSiz) {TX_Print("IE_popup_planes__ E001"); break;}
      LstPtr[i1]=NULL;
    }
  }

  GUI_popup__ (LstPtr, NULL, 0, IE_popup_planes_CB, NULL);

  return 0;

}
*/

//===============================================================
  int IE_popup_planes_CB (MemObj *mo, void **data) {
//===============================================================
// mouse goes over entry in popup
// see also UI_GR_CB_Sel2


  int  iEv;


  iEv = GUI_DATA_EVENT;

  printf("IE_popup_planes_CB iEv=%d\n",iEv);

  if(iEv == TYP_EventPress) {
    // selection
      printf(" menu: select |%s|\n",GUI_DATA_S2);
    // report selection to CAD as text
    IE_cad_InpIn_oid (GUI_DATA_S2);


  } else if(iEv == TYP_EventEnter) {
    // hilite
      printf(" menu: enter |%s|\n",GUI_DATA_S2);
    // hilite obj cSel
    UI_disp_pln_oid (GUI_DATA_S2);


  } else if(iEv == TYP_EventLeave) {
    // unhilite
      printf(" menu: leave |%s|\n",GUI_DATA_S2);
    // clear hilite
    UI_disp_pln_oid (NULL);


  } else if(iEv == TYP_EventUnmap) {
    // cancel selection / end preview
      printf(" menu: unmap\n");
  }


  return FALSE;  // GTK2 ! was TRUE

}


//=====================================================================
  MemObj UI_cad (MemObj *parent, void **data) {
//=====================================================================
/// activate CAD-toolbar

  int       i1, i2;
  char      *p1, opts[16];
  MemObj    tbc, boxMen1, boxMen2, boxMen3, // boxMen4,
            boxBt1, boxBt2, bact,
            wtmp1;


  //------------------------------------------------------------------
  // printf("UI_cad\n");


  // tbc = GUI_toolbar__ (parent);
  tbc = GUI_toolbox__ (parent);
  boxCad0  = GUI_box_h (&tbc, "ea");      // hor - all

  boxCad1  = GUI_box_h (&boxCad0, "");   // all menus
  boxCad2  = GUI_box_v (&boxCad0, "ea");   // inputfields, Labels
  boxCad3  = GUI_box_v (&boxCad0, "");   // all right
  GUI_spc_v (&boxCad0, 4);


  //==================================================================
  // fill boxCad1
  boxMen1    = GUI_box_v (&boxCad1, "");    // P,L,..OptMen's
  boxMen2    = GUI_box_v (&boxCad1, "");    // V,V,..OptMen's
  boxMen3    = GUI_box_v (&boxCad1, "");    // B,N,..OptMen's


  // i1 = -115; //-120; // hsiz text
  // i2 = -UI_fontsizY * 3;
  strcpy(opts, "16,e");


  //......... OptionMenueblock 1 .................................
  IE_auxFun1 ((char**)&memspc101, (char**)&memspc102, cad_lst_p);
  p1 = MSG_get_str ("CADPT");
  IE_wCad_men[0] = GUI_optmen__ (&boxMen1, p1,
                                 (char**)&memspc101, (char**)&memspc102,
                                 IE_cad_p_CB, opts);

  IE_auxFun1 ((char**)&memspc101, (char**)&memspc102, cad_lst_l);
  p1 = MSG_get_str ("CADLN");
  IE_wCad_men[1] = GUI_optmen__ (&boxMen1, p1,
                                 (char**)&memspc101, (char**)&memspc102,
                                 IE_cad_l_CB, opts);

  IE_auxFun1 ((char**)&memspc101, (char**)&memspc102, cad_lst_c);
  p1 = MSG_get_str ("CADCI");
  IE_wCad_men[2] = GUI_optmen__ (&boxMen1, p1,
                                 (char**)&memspc101, (char**)&memspc102,
                                 IE_cad_c_CB, opts);

  IE_auxFun1 ((char**)&memspc101, (char**)&memspc102, cad_lst_r);
  p1 = MSG_get_str ("CADPLN");
  IE_wCad_men[3] = GUI_optmen__ (&boxMen1, p1,
                                 (char**)&memspc101, (char**)&memspc102,
                                 IE_cad_r_CB, opts);

  IE_auxFun1 ((char**)&memspc101, (char**)&memspc102, cad_lst_d);
  p1 = MSG_get_str ("CADVC");
  IE_wCad_men[4] = GUI_optmen__ (&boxMen1, p1,
                                 (char**)&memspc101, (char**)&memspc102,
                                 IE_cad_d_CB, opts);


  //......... OptionMenueblock 2 .................................
  IE_auxFun1 ((char**)&memspc101, (char**)&memspc102, cad_lst_v);
  p1 = MSG_get_str ("CADVAR");
  IE_wCad_men[5] = GUI_optmen__ (&boxMen2, p1,
                                 (char**)&memspc101, (char**)&memspc102,
                                 IE_cad_v_CB, opts);

  IE_auxFun1 ((char**)&memspc101, (char**)&memspc102, cad_lst_s);
  p1 = MSG_get_str ("CADCV");
  IE_wCad_men[6] = GUI_optmen__ (&boxMen2, p1,
                                 (char**)&memspc101, (char**)&memspc102,
                                 IE_cad_s_CB, opts);

  IE_auxFun1 ((char**)&memspc101, (char**)&memspc102, cad_lst_a);
  p1 = MSG_get_str ("CADSUR");
  IE_wCad_men[7] = GUI_optmen__ (&boxMen2, p1,
                                 (char**)&memspc101, (char**)&memspc102,
                                 IE_cad_a_CB, opts);

  IE_auxFun1 ((char**)&memspc101, (char**)&memspc102, cad_lst_b);
  p1 = MSG_get_str ("CADSOL");
  IE_wCad_men[8] = GUI_optmen__ (&boxMen2, p1,
                                 (char**)&memspc101, (char**)&memspc102,
                                 IE_cad_b_CB, opts);

  IE_auxFun1 ((char**)&memspc101, (char**)&memspc102, cad_lst_n);
  p1 = MSG_get_str ("CADDIM");
  IE_wCad_men[9] = GUI_optmen__ (&boxMen2, p1,
                                 (char**)&memspc101, (char**)&memspc102,
                                 IE_cad_n_CB, opts);


  //......... OptionMenueblock 3 ................................
  IE_auxFun1 ((char**)&memspc101, (char**)&memspc102, cad_lst_m);
  p1 = MSG_get_str ("CADMDL");
  IE_wCad_men[10] = GUI_optmen__ (& boxMen3, p1,
                                 (char**)&memspc101, (char**)&memspc102,
                                 IE_cad_m_CB, opts);

  IE_auxFun1 ((char**)&memspc101, (char**)&memspc102, cad_lst_t);
  p1 = MSG_get_str ("CADTR");
  IE_wCad_men[11] = GUI_optmen__ (&boxMen3, p1,
                                 (char**)&memspc101, (char**)&memspc102,
                                 IE_cad_t_CB, opts);


  IE_auxFun1 ((char**)&memspc101, (char**)&memspc102, cad_lst__);
  p1 = MSG_get_str ("CADCUT");
  IE_wCad_men[12] = GUI_optmen__ (&boxMen3, p1,
                                 (char**)&memspc101, (char**)&memspc102,
                                 IE_cad___CB, opts);

  IE_auxFun1 ((char**)&memspc101, (char**)&memspc102, cad_lst_i_);
  p1 = MSG_get_str ("CADINT");
  IE_wCad_men[13] = GUI_optmen__ (&boxMen3, p1,
                                 (char**)&memspc101, (char**)&memspc102,
                                 IE_cad_i_CB, opts);

//   IE_auxFun1 ((char**)&memspc101, (char**)&memspc102, cad_lst_z);
//   p1 = MSG_get_str ("CADANA");
//   IE_wCad_men[14] = GUI_optmen__ (&boxMen3, p1,
//                                  (char**)&memspc101, (char**)&memspc102,
//                                  IE_cad_z_CB, opts);

  IE_auxFun1 ((char**)&memspc101, (char**)&memspc102, cad_lst_Ace);
  p1 = MSG_get_str ("CADCP");
  IE_wCad_men[14] = GUI_optmen__ (&boxMen3, p1,
                                 (char**)&memspc101, (char**)&memspc102,
                                 IE_cad_Ace_CB,opts);







  // gtk_widget_set_sensitive (IE_MenInd[0], FALSE);
  // gtk_widget_set_sensitive (IE_MenInd[1], FALSE);
  // gtk_widget_set_sensitive (IE_MenInd[2], FALSE);
  // gtk_widget_set_sensitive (IE_MenInd[3], FALSE);
  // gtk_widget_set_sensitive (IE_MenInd[4], FALSE);





  //-----------------------------------------------------
  // die InputEntryfelder u die InputInfofelder
  // manuell, damit GesamtHoehe geringer wird.
/*
  wtmp1 = gtk_fixed_new ();
  gtk_container_add (GTK_CONTAINER (boxInpEnt), wtmp1);
  i2=2;
  for(i1=0; i1 < INPRECANZ; ++i1) {
    IE_wCad_obj[i1]=gtk_entry_new ();
    gtk_signal_connect (GTK_OBJECT (IE_wCad_obj[i1]),
                        "key_press_event",
                        (GtkSignalFunc)IE_inp_CB__, NULL);
    gtk_signal_connect (GTK_OBJECT (IE_wCad_obj[i1]),
                        "focus_in_event",
                        (GtkSignalFunc)IE_inp_CB__, NULL);
    gtk_widget_set_usize (IE_wCad_obj[i1], 150, 0);
    gtk_fixed_put (GTK_FIXED (wtmp1), IE_wCad_obj[i1], 0, i2);

    IE_wCad_info[i1]=gtk_label_new ("-");
    gtk_fixed_put (GTK_FIXED (wtmp1), IE_wCad_info[i1], 160, i2+2);

    gtk_widget_show (IE_wCad_obj[i1]);
    gtk_widget_show (IE_wCad_info[i1]);
    // i2 += UI_fontsizY * 2;  // UI_fontsizY Linux: UI_fontsizX/Y=7,9
    i2 += UI_fontsizY;             // UI_fontsizY Windows: groesser !!!
    printf(" fixed-i2=%d\n",i2);
  }
*/


  //====================================================================
  // fill boxCad2
  // create the inputFields and the infoText
  // i2 = UI_fontsizY + UI_fontsizX;  // box-height
/*
#ifdef _MSC_VER
  i2 = UI_fontsizY + (UI_fontsizY / 3);
#else
  i2 = UI_fontsizY + UI_fontsizY - (UI_fontsizY / 4);  // box-height
#endif
    // printf(" UI_fontsizX=%d, UI_fontsizY=%d\n",UI_fontsizX,UI_fontsizY);
  // sprintf(opts, "20,%d", -i2);
*/
  sprintf(opts, "fea");   // frameless extend; gtk3

    // printf(" start-boxCad2\n");

  for(i1=0; i1<INPRECANZ; ++i1) {
    // due to different height of entry/label Gtk3 needs a hbox
    wtmp1 = GUI_box_h (&boxCad2, "ea");
    IE_wCad_obj[i1] = GUI_entry__ (&wtmp1,NULL,"",IE_inp_CB__,NULL,opts);
    IE_wCad_info[i1] = GUI_label__ (&wtmp1, "-", "l15");
    GUI_sep__ (&boxCad2, 0, 0);   // vert. line under entry
  }
    // printf(" done-boxCad2\n");



  //====================================================================
  // fill boxCad3   // right side;
  boxInp2 = GUI_box_v (&boxCad3, "");          // Label CREATE + DB-name
  wtmp1  = GUI_box_h (&boxCad3, "");        // for 2 vert. buttonrows
  boxBt1 = GUI_box_v (&wtmp1, "");          // left:  Buttons Help ..
  boxBt2 = GUI_box_v (&wtmp1, "");          // right: Buttons Cancel, OK



  // Entry AptIndex
  wtmp1   = GUI_box_h (&boxInp2, "");  // DB-index, Info
  // IE_txtMode  = GUI_Label_1(wtmp1, "CREATE  ", 0);
  IE_txtMode  = GUI_label_htm__ (&wtmp1, "CREATE  ", "");
                               // "MODIFY "
  // IE_entHdr   = GUI_Entry(wtmp1, NULL, "", IE_cad_AptInd_CB, -1);
  IE_entHdr   = GUI_label__ (&wtmp1, "", "l");

  // gtk_entry_set_editable (IE_entHdr, FALSE);
  // GUI_Label_1(wtmp1, " ID ", 0); // nur Abstandshalter ..

  // no modification possible ..
  // GUI_set_enable (IE_entHdr, FALSE);   // 201-06-21


  //-----------------------------------------------------
  IE_entNam = GUI_entry__ (&boxInp2, NULL, "", IE_nam_CB__,NULL, "");  // -150
  MSG_Tip ("CADname");
  // GUI_Tip ("define ObjectName, ObjectInfo (you can search that with 'Search/Name')");



  //-----------------------------------------------------
  // wtmp2 = GUI_box_h (boxInp2, 0);
  // IE_ck1 = GUI_CkButt (wtmp2, "hide BaseObj", FALSE, NULL, NULL, 0);
  // IE_ck1 = GUI_CkButt (wtmp2, "new Obj", FALSE, NULL, NULL, 0);
  // GUI_Tip ("Obj. neu erzeugen oder modifizieren / create new obj or modify");
  // IE_ck1 = GUI_CkButt (wtmp2, "overWrite", TRUE, NULL, NULL, 0);
  // MSG_Tip ("CADover");
  // GUI_Tip ("overwrite existing object / vorhandenes Object ueberschreiben");



  // bctl1   = GUI_box_h (boxInp2, 0);    // Butt REV



  // GUI_Label_1(bctl1, "            ", 0);
  // GUI_Butt (bctl1, "Revers", IE_cad_RotS,    (void*)"Rev", 0);
  // MSG_Tip ("CADreve");
    // GUI_Tip  ("change direction / umdrehen");
  // GUI_Butt (bctl1, "CCW", IE_cad_RotS,   (void*)"CCW", 0);
      // GUI_Tip  ("Gegenuhrzeigersinn bei Draufsicht");



/*
  wtmp2 = GUI_box_v (bctl2, 0);
  wtmp3 = GUI_box_h (wtmp2, 0);
  GUI_Butt (wtmp3, "HI", IE_cad_Side,    (void*)"HI", 0);
      GUI_Tip  ("Seite oben");
  GUI_Butt (wtmp3, "LO", IE_cad_Side,    (void*)"LO", 0);
      GUI_Tip  ("Seite unten");


  wtmp3 = GUI_box_h (wtmp2, 0);
  GUI_Butt (wtmp3, "LEft", IE_cad_Side,  (void*)"LE", 0);
      GUI_Tip  ("Seite links");
  GUI_Butt (wtmp3, "RIght", IE_cad_Side, (void*)"RI", 0);
      GUI_Tip  ("Seite rechts");
*/
/*
  wtmp2 = GUI_box_v (bctl2, 0);
  GUI_Butt (wtmp2, "HIX", IE_cad_Side,    (void*)"HIX", 0);
      GUI_Tip  ("rechts bei Draufsicht");
  GUI_Butt (wtmp2, "LOX", IE_cad_Side,    (void*)"LOX", 0);
      GUI_Tip  ("links bei Draufsicht");

  wtmp2 = GUI_box_v (bctl2, 0);
  GUI_Butt (wtmp2, "HIY", IE_cad_Side,    (void*)"HIY", 0);
      GUI_Tip  ("oben bei Draufsicht");
  GUI_Butt (wtmp2, "LOY", IE_cad_Side,    (void*)"LOY", 0);
      GUI_Tip  ("unten bei Draufsicht");

  wtmp2 = GUI_box_v (bctl2, 0);
  GUI_Butt (wtmp2, "HIZ", IE_cad_Side,    (void*)"HIZ", 0);
  GUI_Butt (wtmp2, "LOZ", IE_cad_Side,    (void*)"LOZ", 0);
*/



/*
  GUI_Butt (bctl3, "IN", IE_cad_Side1,   (void*)"IN", 0);
      GUI_Tip  ("Ort innen");
  bctl3_2=GUI_Butt (bctl3, "OUT", IE_cad_Side1,  (void*)"OUT", 0);
      GUI_Tip  ("Ort aussen");
*/

  // disact.
  // gtk_widget_set_sensitive (bctl1, FALSE);
  // gtk_widget_set_sensitive (bctl2, FALSE);
  // gtk_widget_set_sensitive (bctl3, FALSE);







  //-----------------------------------------------------
  GUI_button__ (&boxBt1, "Help CAD", IE_cad_help, (void*)"CAD", "");
  MSG_Tip ("CADhelpCAD");

  IE_help_F = GUI_button__ (&boxBt1, "Help Func", IE_cad_help, (void*)"Func", "");
  MSG_Tip ("CADhelpFunc");

  IE_help_I = GUI_button__ (&boxBt1, "Help Input", IE_cad_help, (void*)"Input", "");
  MSG_Tip ("CADhelpInput");





  //-----------------------------------------------------
  IE_but_Canc=GUI_button__ (&boxBt2, " Cancel ", IE_cad_CB_Cancel, NULL, "");
  MSG_Tip ("CADexit");
  // GUI_Tip  ("exit Modify; does not Save (Ctrl+rightMouseButt)");
  // IE_but_DEL=GUI_Butt (boxCad2C, "Del", IE_cad_DEL, NULL, 2);


  IE_but_Clear=GUI_button__ (&boxBt2, " Clear ", IE_cad_CB_Clear, NULL, "");
  MSG_Tip ("CADclear");
  // GUI_Tip  ("exit Modify; does not Save (Ctrl+rightMouseButt)");
  // IE_but_DEL=GUI_Butt (boxCad2C, "Del", IE_cad_DEL, NULL, 2);



  IE_but_OK =GUI_button__ (&boxBt2, " OK ", IE_cad_CB_Ok, NULL, "");
  MSG_Tip ("CADsave");
  // GUI_Tip  ("aktives Obj. speichern / save active record / right mouseb.");

  // disact.
  GUI_set_enable (&IE_but_OK, FALSE);   // disactivate "OK"
  GUI_set_enable (&IE_help_F, FALSE);   // disactivate "Help Func"
  GUI_set_enable (&IE_help_I, FALSE);   // disactivate "Help Input"
  GUI_set_enable (&IE_but_Canc, FALSE);   // disactivate "Cancel"
  GUI_set_enable (&IE_but_Clear, FALSE);   // disactivate "Clear"


  // IE_bar__ = tbc;

    // printf("exit UI_cad\n");

/* 2012-01-24
  // connect the hide (unmap-event)
  // g_signal_connect (GTK_OBJECT(tbc), "unrealize",
  g_signal_connect (GTK_OBJECT(tbc), "hide",
                    GTK_SIGNAL_FUNC(UI_GR_RECONFIG), NULL);
*/


  // g_signal_connect (GTK_OBJECT(tbc), "button_press_event",
                    // GTK_SIGNAL_FUNC(IE_ButtonPress), NULL);


  return tbc;

}

/* UNUSED
//================================================================
  gint IE_ButtonPress (GtkWidget *widget, GdkEventButton *event) {
//================================================================

  // printf("BBBBBBBBBBBBBB IE_ButtonPress button=%d\n",event->button);

  if(event->button == 2) {
      // printf(" M2\n");
    IE_cad_selM2 (-1);


  } else if(event->button == 3) {
      // printf(" M3\n");
    IE_inp_selection_clear ();    // clear selection
    IE_cad_InpIn_CR ();
  }

  return FALSE;

}
*/



//================================================================
//========= TextEdit ed2 =========================================
//================================================================



//=====================================================================
  int IE_ed2_mod (MemObj *mo, void **data) {
//=====================================================================
// Hilfsfunktionen Window TextEdit


  int      i1, lNr, iFlag;
  char     *txt;


  // txt = data;
  txt = GUI_DATA_S1;


  // printf("============================================= \n");
  // printf("IE_ed2_mod  |%s|\n",txt);


  if(!strcmp(txt, "Exit")) {
    // exit mode Modify; goto EndOfProgram; IE_activate..
    IE_cad_INS ("Exit");
    return 0;
  }


  // ab hier "OK oder "Test"
  if(!strcmp(txt, "Test")) iFlag = 1;  // 1=Test
  else                     iFlag = 2;  // 2=OK


  // gesamten Text vom EditWindow -> IE_buf kopieren
  IE_bufSiz = IE_BUFSIZ;
  // i1 = GUI_Ed_Read (&IE_ed1_wText, IE_buf, &IE_bufSiz);
  i1 = GUI_edi_Read (IE_buf, &IE_bufSiz, 0L, -1L, &IE_edWin1);
  if(i1 < 0) {TX_Print("IE_ed2_mod E001"); return 0;}
    // printf("IE_buf=|%s| Siz=%ld\n",IE_buf,IE_bufSiz);


  lNr = ED_get_lnr_act ();   // get act.lNr

  if(iFlag == 2) WC_set_obj_stat (0);  // perm; nur bei OK

  // IE_cad_test__:
  ED_work_CAD (lNr, IE_buf);
  DL_Redraw ();

  WC_set_obj_stat (1); // ab nun wieder alles temp ..

  if(iFlag == 1) return 0;  // Test done ..


  //----------------------------------------------------------------
  // IE_cad_OK:
  UTF_chg_line (lNr, IE_buf);   // replace line im mem ..

  // kill win
  // IE_ed2__ (NULL, PTR_INT(UI_FuncKill));  // 2011-04-14
  IE_ed2__ (NULL, GUI_SETDAT_EL(TYP_EventPress,UI_FuncKill));
  GUI_update__ ();   // warten bis Window weg ist ..

  // activate next line ...
  ED_inc_lnr  ();
  IE_activate ();

  return 0;
}


//=====================================================================
  int IE_ed2__ (MemObj *mo, void **data) {
//=====================================================================
// IE_ed2__             simple TextEditor
// benutzt - ebenso wie IE_ed1__ - IE_edWin1


  static MemObj win0;

  int    i1, mode;
  long   lNr, lLen;
  char   *p1;
  MemObj box0, box1;


  i1 = GUI_DATA_L1;
  // printf("IE_ed2__ %d\n",i1);



  switch (i1) {


    //---------------------------------------------------------
    case UI_FuncInit:

      // printf(" IE_buf=%d |%s|\n",IE_bufSiz,IE_buf);


      // if(IE_ed1_win.stat != 0) {   // Win schon vorhanden ?
        // if(IE_EdFnc != 2) {
          // gtk_widget_destroy (IE_ed1_win.win);
          // IE_ed1_win.stat = 0;
        // }
      // }

      IE_EdFnc = 2;

      // IE_ed1_win.win = GUI_Window ("TextEdit",IE_ed2__,(void*)UI_FuncKill,0);
      win0 = GUI_Win__ ("TextEdit",IE_ed2__, "-700,-200");

      // box0 = GUI_box_v (IE_ed1_win.win, 0);
      box0 = GUI_box_v (&win0, "a,a");

      // GUI_Ed_Init (&IE_ed1_wText, NULL, 1);
      IE_edWin1 = GUI_edi__ (&box0, IE_ed2__, 1, "e,e");


/*
      // connect KlickIntoEditor
      // bei press kommt die alte CurPos !!
      // gtk_signal_connect (GTK_OBJECT(IE_ed1_wText), "button_release_event",
      // gtk_signal_connect (GTK_OBJECT(IE_ed1_wText.text),  // bis 2005-10-26
      gtk_signal_connect (GTK_OBJECT(IE_ed1_wText.view),     // ab 2005-10-26
                          // "button_press_event", // damit kommt alte CurPos !!
                          "button_release_event",
                          GTK_SIGNAL_FUNC(IE_ed2_CMB), NULL);
*/

      // get act.lNr
      lNr = ED_get_lnr_act ();
      p1 = UTF_GetPosLnr (&lLen, lNr);
        // printf(" lNr=%d lLen=%d\n",lNr,lLen);
      if(lNr >= IE_bufMax) {TX_Error("IE_ed2__ EOM"); return -1;}
      memcpy(IE_buf, p1, lLen);
      IE_buf[lLen] = '\0';
      IE_bufSiz = lLen;

      // Text -> EdWin
      // GUI_Ed_Write (&IE_ed1_wText, p1, lLen);
      // GUI_Ed_Write (&IE_ed1_wText, IE_buf, &IE_bufSiz, IE_bufMax);
      GUI_edi_Write (&IE_edWin1, IE_buf, &IE_bufSiz, IE_bufMax);

      // Cursor -> EOL
      // GUI_Ed_setCpos (&IE_ed1_wText, IE_bufSiz);


      // H-Box f Buttons unten
      // box1 = GUI_box_h (box0, -1);
      box1 = GUI_box_h (&box0, "a,a");

      GUI_button__(&box1, "Exit", IE_ed2_mod, (void*)"Exit", "");
      GUI_button__(&box1, "Test", IE_ed2_mod, (void*)"Test", "");
      GUI_button__(&box1, "OK",   IE_ed2_mod, (void*)"OK",   "");

      GUI_Win_up (NULL, &win0, 0);
      GUI_Win_go (&win0);

      break;



    //---------------------------------------------------------
    case UI_FuncKill:   // Exit-Button
        printf("IE_ed2__ Kill\n");
      GUI_Win_kill (&win0);


    case UI_FuncExit:   // called from GUI_Win__
      IE_edWin1 = GUI_OBJ_INVALID();
        printf("IE_ed2__ Exit\n");
  }



  return 0;

}


//================================================================
  int IE_getEyePt () {
//================================================================
// output:
//   inpField-2   textPoint
//   inpField-3   DimPlane    direction (X|Y|Z|P) & backPlane (0|1|2|3|4)

  static Vector    vc1;  // keep last vcs
  int       i1, i2;
  char      *p1, *p2, *p3, *p4, tmpBuf[128];
  Point     pt1, pt2, pt3;


  // printf("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE \n");
  // printf("IE_getEyePt stat=%d\n",WC_get_obj_stat());
      // aber nur wenn Punkt in der ConstrPlane (pt.Z == ConstrPlane.Z)
      // if(WC_get_obj_stat() == 0) goto L_add_txt;  // 0==save obj

      // den Punkt auf die Plane p1-p2 projizieren.
      // ACHTUNG: Feld[-2] muss p1 sein;
      //          Feld[-1] muss p2 sein;
      //          Feld[+1] muss der PlaneTyp sein
      // printf(" pt1=|%s| pt2=|%s| plnTyp=|%s|\n",ep[i1-2],ep[i1-1],ep[i1+1]);


  // get aktuellen Sichtstrahl holen (zeigt zum Auge)
  // if(WC_get_obj_stat() != 0)
  vc1 = GL_GetEyeX();
    // DEB_dump_obj__ (Typ_VC, &vc1, "  vc1:");

  p1 = IE_get_inp__ (0);
  p2 = IE_get_inp__ (1);


  // get 2 dimension-points
  APT_obj_expr (&pt1, Typ_PT, p1);
  APT_obj_expr (&pt2, Typ_PT, p2);
    // DEB_dump_obj__ (Typ_PT, &pt1, "  pt1:");
    // DEB_dump_obj__ (Typ_PT, &pt2, "  pt2:");


  // get cursor-position on constrPln
  p3 = IE_get_inp__ (2);
  if(strlen(p3) < 2) {
    UI_GR_get_actPosA (&pt3);
  } else {
    APT_obj_expr (&pt3, Typ_PT, p3);
  }
    // DEB_dump_obj__ (Typ_PT, &pt3, "  pt3:");


  // get dimensioning-direction (1-char Z|X|Y|P)
  p4 = IE_get_inp__ (3);
  // get EyePT from 3 points and plane
  i2 = AP_PT2EyePln (&pt3, p4, &pt1, &pt2, &pt3, &vc1);
    // DEB_dump_obj__ (Typ_PT, &pt3, "  EyePln:");


  // add bp to PlaneTyp
  if(strlen(p4) < 1) sprintf(tmpBuf, "P%d",i2);  // Def parallel 'P'
  else {
    sprintf(tmpBuf, "%c%d",*p4,i2);
    tmpBuf[0] = toupper (tmpBuf[0]);
  }
    printf(" tmpBuf=|%s|\n",tmpBuf);



  // write into entry
  // gtk_entry_set_text ((GtkEntry*) (IE_wCad_obj[i1+1]), tmpBuf);
  IE_set_inp (3, tmpBuf);
  // ep[i1+1] = IE_get_inp__ (i1+1);
    // printf(" korr.plnTyp=|%s|\n",ep[i1+1]);


  // fix textPoint
  tmpBuf[0] = '\0';
  AP_obj_add_nval (tmpBuf, 3, (double*)&pt3, NULL);
  IE_set_inp (2, tmpBuf);


  return 0;

}


//================================================================
//========= ANALYZ ===============================================
//================================================================


//================================================================
  int IE_analyz__ (char *fncAct) {
//================================================================

  int    i1, i2, typ1, tabSiz;
  long   l1;
  char   *p1;
  char   o1[OBJ_SIZ_MAX], cbuf[256];
  void   *vp1;
  int       *aus_typ, aus_anz;
  char      *aus_tab;  // strings a 256 chars


  printf("IE_analyz__ |%s|\n",fncAct);


  if(!strncmp(fncAct, "ANALYZD", 7)) {
    IE_analyz_dist (fncAct[7]);
    goto L_fertig;
  }



  //================================================================
  // ANALYZP = analyze Point
  if(strcmp(fncAct, "ANALYZP")) goto L_ANALYZO;

  // p1 = gtk_entry_get_text ((GtkEntry*) (IE_wCad_obj[0]));
  p1 = GUI_entry_get (&IE_wCad_obj[0]);
  printf(" ANALYZP |%s|\n",p1);

  typ1 = Typ_PT;
  i1 = APT_obj_expr ((void*)o1, typ1, p1);
  // DEB_dump_obj__ (typ1, o1, "rc=%d",i1);

  if(i1 == 0) {
    sprintf(cbuf, "=============>>> Analyze point %s:",p1);
    UI_winTX_prt (cbuf);
    vp1 = o1;
    goto L_print;
  }


  goto L_fertig;




  //================================================================
  // ANALYZO = analyze obj
  L_ANALYZO:
  if(strcmp(fncAct, "ANALYZO")) goto L_ANALYZV;


  // p1 = gtk_entry_get_text ((GtkEntry*) (IE_wCad_obj[0]));
  p1 = GUI_entry_get (&IE_wCad_obj[0]);
  printf(" ANALYZO |%s|\n",p1);

  aus_typ = (int*) memspc55;
  aus_tab = (char*) memspc54;

  i1 = sizeof(memspc55) / sizeof(int);
  i2 = sizeof(memspc54) / 256;
  tabSiz = IMIN (i1, i2);



  aus_anz = IE_decode_Ln (aus_typ, (void*)aus_tab, tabSiz, p1);
  typ1 = aus_typ[0];

  sprintf(cbuf, "=============>>> Analyze obj %s:",p1);
  UI_winTX_prt (cbuf);

  // ModRefs ("M20") sofort raus
  if(IE_selTyp == Typ_Model) {
    TX_Print("M%d   Model %s",IE_selInd,p1);
    // goto L_fertig;
    vp1 = DB_get_ModRef (IE_selInd); // ModelReference
    // l1 = ((ModelRef*)mrRec)->modNr;
    // vp1 = DB_get_ModBas (l1);  // den BasisModelRecord holen
    typ1 = IE_selTyp;
    goto L_print;
  }

  i1 = APT_obj_expr ((void*)o1, typ1, p1);
  if(i1 < 0) goto L_fertig;

  // typ1=Typ_GTXT: o1 ist ein ObjGX; weiter decodieren.
  if((typ1 == Typ_Note) ||
     (typ1 == Typ_CV)   ||
     (typ1 == Typ_SUR))    {
      // DEB_dump_ox_0 (o1, "OBJX");
    typ1 = ((ObjGX*)o1)->form;
    vp1  = ((ObjGX*)o1)->data;

  } else {
    vp1 = o1;
  }
  goto L_print;



  //================================================================
  // ANALYZV = analyze Vertex
  L_ANALYZV:

  // p1 = gtk_entry_get_text ((GtkEntry*) (IE_wCad_obj[0]));
  p1 = GUI_entry_get (&IE_wCad_obj[0]);

  typ1 = Typ_PT;
  sprintf(cbuf, "=============>>> Analyze GL-Vertex %s:",p1);
  UI_winTX_prt (cbuf);

  GL_temp_del_all (); // alle temp. obj loeschen
  DL_hili_off (-1L); // // unhilite all


  GL_SelVert__ ((void*)o1); // den dem Cursor naechsten vertex holen

  // unselect all hilited und temp;
  // display vertex
  l1 = -2;
  GL_DrawSymB (&l1, 2, SYM_CIR_S, (void*)o1);
  vp1 = o1;
  goto L_print;



  //================================================================
  L_print:
  sprintf(cbuf, "%sanalyz.dat",OS_get_tmp_dir());
  // DEB_dump_obj__ (TYP_FuncInit, NULL, cbuf);
  // DEB_dump_obj__ (typ1, vp1, "");
  // DEB_dump_obj__ (TYP_FuncEnd, NULL, cbuf);
  UI_winTX_prf (cbuf);




  //================================================================
  L_fertig:
  // alle Inputfelder loeschen
  IE_cad_ClearInputs (IE_inpAnz);

  // feld 1 aktivieren
  IE_inp_chg (0);


  DL_Redraw ();

  return 1;

}


//================================================================
  int IE_ui_func_closed () { 
//================================================================
 

  IE_edWin1 = GUI_OBJ_INVALID();
  IE_ui_func = NULL;

  return 0;

}


//================================================================
  int IE_analyz_dist (char fnc) {
//================================================================
// fnc = '1' (Dist. PT-PT) od '2' (Dist. PT-Obj)


  int    i1, i2, typ1, typ2, tabSiz;
  char   *p1, *p2;
  char   o1[OBJ_SIZ_MAX], o2[OBJ_SIZ_MAX];
  Point  pa[2];
  double d1, dx,dy,dz;

  int       *aus_typ, aus_anz;
  char      *aus_tab;  // strings a 256 chars



  printf("IE_analyz_dist\n");


  aus_typ = (int*) memspc55;
  aus_tab = (char*) memspc54;

  i1 = sizeof(memspc55) / sizeof(int);
  i2 = sizeof(memspc54) / 256;
  tabSiz = IMIN (i1, i2);


  // ANALYZ2:   dist PT - PT
  // p1 = gtk_entry_get_text ((GtkEntry*) (IE_wCad_obj[0]));
  p1 = GUI_entry_get (&IE_wCad_obj[0]);
    // printf(" p1=|%s|\n",p1);


  // p2 = gtk_entry_get_text ((GtkEntry*) (IE_wCad_obj[1]));
  p2 = GUI_entry_get (&IE_wCad_obj[1]);
    // printf(" p2=|%s|\n",p2);


  // aus_anz = IE_decode_Ln (aus_typ, (void*)aus_tab, p1);
  // typ1 = aus_typ[0];
  // if(typ1 == Typ_PT) APT_obj_expr ((void*)o1, typ1, p1);
  // else { TX_Error("IE_analyz__ Obj1 NOT YET SUPPORTED"); return 0;}

  typ1 = Typ_PT;
  i1 = APT_obj_expr ((void*)o1, typ1, p1);
  // DEB_dump_obj__ (typ1, o1, "rc=%d",i1);

  if(fnc == '1') {

    typ2 = Typ_PT;
    i1 = APT_obj_expr ((void*)o2, typ2, p2);
    // DEB_dump_obj__ (typ2, o2, "rc=%d",i1);

  } else {
    aus_anz = IE_decode_Ln (aus_typ, (void*)aus_tab, tabSiz, p2);
    typ2 = aus_typ[0];

    if     (typ2 == Typ_PT) APT_obj_expr ((void*)o2, typ2, p2);
    else if(typ2 == Typ_LN) APT_obj_expr ((void*)o2, typ2, p2);
    else if(typ2 == Typ_CI) APT_obj_expr ((void*)o2, typ2, p2);
    else { TX_Error("IE_analyz__ Obj2 NOT YET SUPPORTED"); return 0;}
  }


  // PT PT
  if((typ1 == Typ_PT)&&(typ2 == Typ_PT)) {
    pa[0] = *((Point*)o1);
    pa[1] = *((Point*)o2);
    d1 = UT3D_len_2pt (&pa[0], &pa[1]);
    TX_Print("Dist = %f / %s - %s",d1,p1,p2);
    dx = pa[1].x - pa[0].x;
    dy = pa[1].y - pa[0].y;
    dz = pa[1].z - pa[0].z;
    TX_Print("    DX = %f   DY = %f   DZ = %f",dx,dy,dz);


  // PT LN
  } else if((typ1 == Typ_PT)&&(typ2 == Typ_LN)) {
    pa[0] = *((Point*)o1);
    // proj PT -> LN
    UT3D_pt_projptln (&pa[1], &d1, NULL, &pa[0], (Line*)o2);
    TX_Print("Dist = %f / %s - %s",d1,p1,p2);


  // PT AC
  } else if((typ1 == Typ_PT)&&(typ2 == Typ_CI)) {
    pa[0] = *((Point*)o1);
    // proj PT -> AC
    UT3D_pt_projptci (&pa[1], NULL, &pa[0], (Circ*)o2);
    d1 = UT3D_len_2pt (&pa[0], &pa[1]);
    TX_Print("Dist = %f / %s - %s",d1,p1,p2);


  } else { TX_Error("IE_analyz__ NOT YET SUPPORTED"); return 0;}



  GR_tmpDisp_ln (pa, 3);



  return 0;

}


//================================================================
  int IE_buf_del () {
//================================================================
// delete last word from IE_buf; update IE_bufSiz
  
  long    l1;


  if(IE_bufSiz < 2) {
    IE_buf[0] = '\0';
    IE_bufSiz = 0;
    return -1;
  }


  // find last delimiter ' '
  l1 = UTX_pos_del_prev (IE_buf, IE_bufSiz);
    // printf("IE_buf_del %ld %ld\n",l1,IE_bufSiz);

  IE_buf[l1] = '\0';
  IE_bufSiz = l1;


  // write text in IE_buf into window
  if(IE_ui_func)
    IE_ui_func (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncSet));

  return 0;

}


//================================================================
  int IE_buf_add (char *buf) {
//================================================================
// add " " and buf to IE_buf; update IE_bufSiz
 

  int     i1;

  // printf("IE_buf_add |%s| %ld\n",buf,IE_bufSiz);


  i1 = strlen(buf) + 1;
  if((IE_bufSiz + i1) >= IE_bufMax) {
    TX_Error("IE_buf_add E001");
    return -1;
  }

  sprintf(&IE_buf[IE_bufSiz], " %s",buf);
  IE_bufSiz += i1;

      // printf(" IE_buf=|%s|\n",IE_buf);

  return 0;

}
   

//================================================================
  int IE_dump_rec_stru (IE_rec_stru *rs, char *txi) {
//================================================================
// IE_dump_rec_stru     dump struct IE_rec_stru*


  printf("IE_dump_rec_stru %s\n", txi);

  printf(" ind=%d typ=%d info=|%s|\n", rs->ind, rs->typ, rs->info);

  return 0;

}


//================================================================
  int IE_test () {
//================================================================
 
  printf("ex IE_ck_menGrpInd menGrpInd=%d typ=%d\n",IE_cad_typ,menGrpInd);

  return 0;

}


/*
//================================================================
int IE_oLst_ini () {
//================================================================
// init ObjLst

  int     iTyp, iInd;
  char    oNam[32], oVal[256], oInfo[128];
  double  *d1;


  printf("IE_oLst_ini\n");
  printf("  ii=%d Typ=%d\n",IE_inpInd,IE_inpTypR[IE_inpInd]);

  if(IE_inpInd < 0) return 0;
  iTyp = IE_inpTypR[IE_inpInd];


  // which type of input is active ?
  if((iTyp == Typ_VAR) ||
     (iTyp == Typ_XVal)) {

    // (re)Init win
    //UI_mcl__ (NULL, (void*)UI_FuncInit);

    // loop tru DB, get all var's
    for(iInd=1; iInd < APT_VR_SIZ; ++iInd) {
      d1 = DB_get_Var (iInd);
    }
    APED_oid_dbo__ (oNam, iTyp, iInd);

    UI_mcl_add (oNam, oVal, oInfo);



  } else if((iTyp == Typ_VC)) {

  }


  return 0;

}
*/

/* ====================== EOF =========================== */
