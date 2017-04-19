//      ut_GL.c                               RF.
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
MaxZ-Val (dzt 75000.) should be ????  GL_RubberBox_drw__()  GL_Init_Siz()
See "2D-Ableitung = Plotten" below.
See "Reframe unfertig" below.

-----------------------------------------------------
Modifications:
2005-03-15 GL_Draw_Tag u GL_Draw_BMP zu. RF.
2004-04-25 Redraw - via GL_eyeX u GL_eyeZ u GL_cen. RF.
2003-12-07 Neue Const.Plane zu. RF.
2003-03-16 GL_DrawVec neu zu. RF.
2002-04-08 Boundary fuer Faces auch an Oberkontur. RF.
2002-02-12 GL_DrawDitto2, GL_GetActInd zu. RF.
2001-11-18 GL_View_set und Get. RF.
2001-11-17 GL_DrawSymB - Colour.
2001-11-13 Selektionsreihenfolge korr. RF.
2001-09-27 GL_DrawTxtG neu zu. RF.
2001-08-03 DL_hili_on (ActObj) zu. RF.
2001-04-22 GL_DrawFan neu, GL_DrawRSur: 2 Par. zu.
2001-04-21 GL_DrawRSur korr, GL_Draw_Ini neu.
2000-10-17 Vereinheitlichung mit d. Unix-Variante.

-----------------------------------------------------
*/
#ifdef globTag
 void GL(){}
#endif
/*!
\file  ../gr/ut_GL.c
\brief the OpenGL functions 
\code

Function-groups:

GR_Cre..      create dynamic-DB-obj, DL-record and display obj
GR_Disp_..    create DL-record, display obj. Do not create DB-record.
GR_Draw..     display obj. Do not create DB-record, DL-record.

=====================================================
List_functions_start:

GL_DrawPoint
GL_Draw_ray
GL_DrawLine
GL_DrawLtab           display group of lines
GL_DrawPoly2D         2D-Polygon on plane Z=zval
GL_DrawPolySc         Scaled Polygon
GL_DrawCvIpTab        draw polygon from indexTable; open or closed
GL_DrawPoly           polygon
GL_Draw_cvp_dir       polygon and direction-arrow
GL_DrawDimen          Hor, Vert Dimension
GL_DrawDima           Angular Dimension
GL_DrawDimdia         Diameter-Dimension
GL_DrawDimrad         Radius-Dimension
GL_DrawLdr
GL_DrawTxtA           Text; .. ??
GL_Draw_Tag           Text + Farbiges Rechteck ?
GL_DrawSymV3          oriented vector-symbols;  SYM_ARROW SYM_SQUARE
GL_DrawVec            Vector true length; SYM_VEC
GL_DrawSymB           display bitmap symbols SYM_TRI_S SYM_STAR_S ..
GL_Draw_tra           display symbolic TraRot (rotation)
GL_DrawAngA           draw angle with arrowhead
GL_DrawSymVX          display plane / axisSystem [with x,y,z-characters
GL_DrawSymVTL         display vector-symbol with true length at 2D-ScreenCenter

GL_Draw_obj           Draw 1-n Polygons as GL_LINE_STRIPs from ObjGX
GL_Draw_rbez          draw rational bezier curve

-------------- draw into open open GL-List -----------------------------
GL_Draw_Ini                Init Flaechen 
GL_Draw_Ini1               Init Lines,Curves|Flaechen
GL_DrawLn_Ini              Init Lines,Curves
GL_Surf_Ini
GL_EndList,GL_EndList1,GL_EndList2                      close GL-Ausgaben
GL_Disp_ray
GL_Disp_ln                                              Ausgabe GL_LINES
GL_Disp_ln2                                             Ausgabe GL_LINES
GL_Disp_ci2                                             Ausgabe 2D-Circ
GL_Disp_cv                                              Ausgabe Polygon
GL_Disp_cv2                                             Ausgabe Polygon
GL_Disp_rbez          draw rational bezier curve
GL_DrawCirSc                                            Ausgabe Circ ..
GL_Disp_arrh          display arrowhead
GL_Disp_vSym          add vectorsymbols (true length) into open displist
GL_Disp_vc            add normalized vector (SYM_ARROW) into open displist
GL_Disp_symV          add vectorsymbols eg SYM_ARROH SYM_VEC into open displist
GL_Disp_symB          add bitmapsymbols into open displist
GL_Disp_2D_box1                                not-zooming 2D-box
GL_Disp_2D_box2                    unused      zoomed 2D-fields (DrawPixels)
GL_Disp_sq1           Disp. quadrat. Flaeche 
GL_Disp_sur           Draw 1-n Planar Patches from ObjGX
GL_Disp_face          display face from points
GL_Disp_iface         display face from indexed-points
GL_DispTag1           display coloured rectangle
GL_DispTag2           display coloured rectangle
GL_icons_Pos          get position for 2D-icons
GL_icons_dispVcPln    draw VectorSelector and StdPlaneselector
GL_icons_dispTags     draw 2D-tag-icons

GL_GetCen             get center of grWin in userCoords
GL_get_Scale          get GL_Scale;
GL_GetScrSiz          get size of graficWindow in screenCoords
GL_GetViewSizU        get size of graficWindow in userCoords
GL_MousePos           get Mousepos in userCoords
GL_GetEyeLn           Sichtstrahl (Point & Vector) retournieren
GL_GetEyeX            Sichtstrahl (Vector) retournieren
GL_GetActSelPos       get screenCoords & userCoords of selectionPoint
GL_set_viewPlnPos       save screenCoords & userCoords of selectionPoint
GL_GetViewPos         Curpos in userkoord. retournieren
GL_GetConstrPos       Curpos in userkoord. retournieren
GL_GetColorBits       returns Nr of ColorBits (usually 24)
GL_ck_sel_PT          test if point selected
GL_selPt              2D-test: find actCurPos in pTab
GL_selSubPt           2D-test: find point in pTab
GL_Query_siz          Estimate size of all OpenGL-objects for FeedbackBufferSize.
GL_query_ViewZ        return GL_cen.z;
GL_pos_move_2D        move 3D-point with 2D-offset
GL2D_pos_move         relative move of the screenPos in screencoords      INLINE

GL_Uk2Sk              userCoord -> screenCoord
GL_Sk2Uk              screenCoord -> userCoord

GL_sel_rect           returns the ID's of all objs inside rectangle
GL_sel_sel            return objs from selection
GL_sel_sort
GL_sel_add_DB         add DB-obj into selectionBuffer GR_selTab
GL_sel_add_DL         add DL-obj into selectionBuffer GR_selTab
GL_sel_get            get GR_selTab-record

GL_set_bgCol          set background-color; 0=OK, else Error.
GL_ColSet             activate Color,transparency for surface from ColRGB
GL_DefColSet
GL_DefColGet

------------------ InitFunctions:
GL_Redraw

GL_Init0
GL_Init1
GL_Init_View
GL_InitModelSize
GL_InitNewAtt         create / change line-attribute in DL
GL_InitPtAtt
GL_InitAFont
GL_InitGFont
GL_InitGF2
GL_InitSymb
GL_Init_Siz           init clipping planes
GL_DefineView
GL_DefineDisp

GL_GetActInd          return dispList-index of last created obj
GL_Get_DLind          return nr of used dispList obj's (index of next free)

GL_Del_om             del multiple objects from DL
GL_Del0               ein Obj der DL loeschen od gesamte DL loeschen
GL_Del1               ein Obj der Gl-DispList loeschen
GL_Delete             DispList zuruecksetzen;
GL_last_del           delete last obj of DL, if its DB-index is 0
GL_Regen1             reset GR_TAB_IND (remove unused recs at end)

GL_temp_Delete        eine bestimmte loeschen ..
GL_temp_delete        alle temp. obj loeschen ..
GL_temp_GetInd        get index of last created temp-obj
GL_GetInd_temp        returns next free temp-obj-ind
GL_GetInd_last_temp   return last temp-obj-index.

GL_Tex_Ini
GL_Tex_Load
GL_Tex_Del            delete OpenGL-texture
GL_Tex_End
GL_Tex_ckSiz          test if texture could be loaded by OpenGL

DL_hili_on            hilite obj from dispListIndex
DL_hili_off          remove hilite from obj from dispListIndex

GL_config_test

------------------ test functions:
GL_tst_pta             testen der GL_Disp_pta

List_functions_end:
=====================================================

\endcode *//*----------------------------------------




cursorPosition in userCoords on constructionPlane & on viewPlane ist noch
  ziemlich durcheinander. Betrifft:
  UI_GR_MotionNotify   setzt GR_CurUk mit cursorPosition on constructionPlane
  GL_GetConstrPos GL_GetActSelPos DB_get_PT(0L); GL_GetViewPos


Transparenz ex subModel geht ned !!!
 Transparenz geht nur wenn die transparenten Flaechen als letzte gezeichnet
 werden. Das geht bei subModels aber nicht, da sie durch den Ditto-Aufruf
 gerufen werden.
 Damit transpar. Flaechen in subModels gehen, duerfen sie im
 submodel nicht gezeichnet werden, da hinterher wieder volle Flaechen
 kommen (mit dem naechsten submodel).
 Nach dem letzen subModel nochmal Loop ueber alle subModels und nur die
 transpar. Flaechen ausgeben.
 Oder ein eigenes subModel anlegen, in dem alle transparenten Flaechen sind ?
 Dzt: Transparenz ohne DepthTest ausgeben.

 Hardwaresupport mit glBlendFunc(GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA); geht ned

...............................
Ditto: Inhaltelemente brauchen kein DL-Index !
GL_DrawQFac noch notwendig ?? (Ersatz GL_DrawStripe)


 APT_disp       without dli, store obj as temp-DB-obj,
                  using (dli = -1), calls APT_Draw

 APT_Draw       In: DB-index, dli = DL_StoreObj(DB-index), calls GR_Draw

 UI_disp        using dli=DLI_TMP; display preview

 GR_Disp        without dli in the interface (for temp. display);
                  using (dli = -1), calls GR_Draw

 GR_Draw        In: dli, calls GL_Draw; (APT_2d_to_3d_Mat unused)

 GL_Draw        In: dli; GL-ini, GL_Disp, GL-end

 GL_Disp        draw into open disp-list: without dli, without GL-ini(glNewList),
                  without GL-end(glEndList).

===============================================================================
TODO:


2D-Ableitung = Plotten 
ev Zeichnungsableitung = 3D -> 2D-View = in ein Submodel einspeisen ..
 mit stencil (GL_DefineDisp (FUNC_DispHid1, 0)) kann man hidden ausblenden;
 das FEEDBACK liefert aber alle ungefiltert ... (auch hidden lines) ... ??
  UI_GR_WinInit (GDK_GL_STENCIL_SIZE, 32,) zu
  AP_print__
   GL_Feedback
 OpenGL-Demoprogramme mit HiddenLineRemoval > /mnt/win_d/dev/OpenGL
  ohidden.c !!!
  hiddenline.c !!


...............................
Reframe unfertig.
Vorläufig durch Vergrössern des glOrtho-Z-Bereiches Clipping-Problem entschärft.
Mit GDK_GL_DEPTH_SIZE, 32,    gehts viel besser.
Zusätzlich ist aber noch eine DefineViewcenter-Funktion notwendig,
  die zusätzlich den gesamten Koordinatenbereich verschiebt. Könnte man wahrscheinlich
  auch mit dem Verschieben des glOrtho-Mittelpunktes erreichen. NEIN !
  Drehmittelpunkt ?
  Variante 1: alle Vertices verschieben (GL_Transl_Vert).??
  Variante 2: zuerst verschieben - dann drehen - irgendwas in der Art. 
  ACHTUNG: Im gcad gibts eine GC_GR_Init_CLIPBOX die scheinbar die virtuelle ClipBox zeichnet !!


Reset sollte eigentlich alle Punkte durcharbeiten; in Screenkoord umwandeln und
  davon max /min bestimmen.


  Alfa-Texte nur am 0-Punkt selektierbar.
    Siehe Example /p2/opt/Mesa/Mesa-2.5/book/font.c
    GrafTexte wie /p2/opt/Mesa/Mesa-2.5/book/font.c;
    Alfatexte ev. einen Strich in die DL.




------------------------------------------------------------------------------
Setup GL: UI_GR_WinInit.


===========================================================================
Funktionsweise View-Transformationen:

Es gibt ein View-Plane (gelb) und eine ConstructionPlane (gruen).
Die View-Plane ist immer parallel zur X-Y-Plane.
 Rotate erfolgt durch drehen (immer um die Z-Achse) und kippen (die 
   Kippachse liegt immer in der View-Plane und geht durch den
   Bildschirmmittelpunkt.

 GL_view_pln (Plane) View-Plane; gelbes Rechteck.
 GL_constr_pln (Plane) ConstPlane; gruen.

 GL_cen (Point) Der Bildschirmmittelpunkt in Userkoords (kleines Achsenkreuz)
 GL_eyeX (Vector) Die Sichtlinie; zeigt vom Mittelpkt zum Auge (verkehrt);
 GL_eyeZ (Vector) ist ein zugehoeriger Vektor (der OpenGL-UpVektor);
         zeigt parallel zum FensterRand nach oben (2D-up-vector).
         Get 2D-horizontal-vector by UT3D_vc_perp2vc (&vcy, &GL_eyeZ, &GL_eyeX);


GL_Do_Pan__
GL_Do_Rot__
GL_Print_Redraw            print with white background
GL_Redra__                 redraw without highlites surfaces
GL_Redraw



===========================================================================
Wie funktioniert Linetyp in GL ?
--------------------------------
  In der GL ist auf DL_base_PtAtt (298) der Record fuer die Point-Attribute.
  In der GL ist ab DL_base_LnAtt (150-199) die Curve-Attribut-Table.
  Beim GL-Init wird ein GL-Record fuer jeden attInd geladen (GL_InitNewAtt).
  Im 'Draw_curve' erfolgt ein 'glCallList (DL_base_LnAtt + attInd);'
  Surfaces ?


Wie funktioniert Hilite in GL ?
--------------------------------
  GL_Redraw; in der 1. DL-Loop keine hilite obj's;
  in der 2. Loop: activate extraLight;
    Tags, Images: draw hilited 2D-Box aussen herum
    surfs: clear dephtBuffer (damit wirds ganz oben gezeichnet)
    curves: change the GL-record of the Attribute to hilited, then restor
      the GL-record.
      GL_InitNewAtt (1, attInd); // hilite
      GL_InitNewAtt (0, attInd); // reset hilite



Wie funktioniert Hide in GL ?
--------------------------------
  GL_Redraw; in der 1. DL-Loop kein draw hidden obj's;
  im Modeus 'draw only hidden' wird hideFlag nur fuer diese Objekte gesetzt.


===========================================================================
Funktionsweise DL:
------------------
GR_ObjTab = displayList
0              - DL_base_font1           temp. objects
DL_base_font1  - DL_Ind_ScBack           Fontcharacters (32-128, polygons)
DL_base_LnAtt                            attributes
DL_base_mod    -  ..                     subModels .. ?
DL_base__      -  ..                     normal objects

In der DispListe liegen von 0-DL_base_font1 die temp. Objekte,
  dann bis DL_Ind_ScBack (129) Fontcharacters (32-128, als Poylgone!)
  dann ab DL_base_LnAtt (150) Attribute,
  ab DL_base_mod (300) beginnen die normalen Objekte.

DL_StoreObj vergibt den naechsten DL-Index; beginnend mit 0.
GL_fix_DL_ind adddiert zum DL-Index den DL_base_mod; ergibt echten DLIndex.
GL_Redraw malt ganz allein;
  for(l1=0; l1<GR_TAB_IND; ++l1) {
    glCallList (l1 + DL_base__);
  }


DL-Index:
---------
Jedes Objekt (Point, Line, ...) hat als Displistindex den Index der DB
  (der DL-Index wird also von aussen vorgegeben).  Soll mit 1 beginnen !?
  Wird ein neg. Index gewuenscht: temporäres Objekt, ein dlInd in einem
   fixen Bereich in der DL (von .. bis ..).



Graf.Attribute:
---------------
In der DL existiert ein Bereich, in dem die graf. Attribute eingetragen werden
  (von DL_base_LnAtt bis += Ln_Attrib_siz).
Jedes Objekt macht einen glCallList auf den Attributeintrag. Dieser besteht aus
 Farbe+Linientyp+Liniendicke. Nicht fuer Punkte und Text.
Mit GL_InitAttTab werden die Defaultattribute angelegt;
mit GL_InitNewAtt kann ein neues Attr angelegt werden.


Hiliten, Dimmen:
----------------
Es gibt zu jedem Objekt ein Statuswort, in dem ein Bit fuer Hilite und eins fuer
  Dimmen reserviert ist.
Am Ende des Redrawzyklus werden (in eigenen Schleifen) alle gehiliteten und dann
  (ebenfalls in eigener Schleife) die gedimmten Objekte hingemalt.
  Dazu muss die gesamte Attributtabelle auf die jeweils erforderliche
  Konfiguration umdefiniert werden (also alle Attribute umschreiben;
  es wird also im GL die der Call geaendert).
  Nun das Obje (zB Line) in GL aufrufen; diese ruft intern das GL-Attribut.


------------------------------------------------------------------------------
Compile:
Linux:
  cc -c -g3 -Wall ut_GL.c




------------------------------------------------------------------------------
Beispiele:

Draw Temporaere DL-Objekte: see ../gr/ut_DL.c


Farbe/Dicke:
  Siehe ut_DL.ca ganz vorn


--------- Symbol anzeigen ------------
  UI_GR_DrawInit ();
  GL_temp_delete ();  // alle temp-Obj. loeschen
  GL_DrawSymB (&dli, 0, SYM_TRI_B, &pt1);
  GL_Redraw ();
  UI_GR_DrawExit ();



==========================================================================
cl -c ut_GL.c


==========================================================================
*/


#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif

#ifdef _MSC_VER
// wird von GL_VERSION_1_1 nicht geliefert; nur von V2 !
#define GL_BGR          0x80E0
#endif


// Display-ScaleGrenzen
#define SCALE_MAX 1000000.0
#define SCALE_MIN 0.0000001


// _HUGE HUGE_VAL macht bei w32 unresolved __HUGE
#define VAL_HUGE  9999999999.9


#include <math.h>                        /* fabs  ...   */
#include <stdio.h>                       // FILE ..
#include <stdlib.h>
#include <string.h>                      // strlen
#include <ctype.h>                       // isdigit



#include <GL/gl.h>                       // /usr/X11R6/include/GL/gl.h
#include <GL/glu.h>



#include "../ut/ut_geo.h"                // Point-def
#include "../ut/ut_msh.h"                // Fac3 ..
#include "../ut/ut_txt.h"                // 
#include "../ut/ut_TX.h"                 // TX_Error
#include "../ut/ut_cast.h"               // INT_PTR
#include "../ut/ut_os.h"                 // OS_get_bas_dir
#include "../ut/func_types.h"                 // ATT_LN_RAY

#include "../ut/func_types.h"                 // Typ_Att_PT, SYM_TRI_S, ..
#include "../db/ut_DB.h"                 // DB_get_ModBas


#include "../gr/ut_gr.h"
#include "../gr/ut_DL.h"
#define extern              // damit wird "extern" im Includefile geloescht !
#include "../gr/ut_GL.h"
#undef extern               // reset extern ..
#include "../gr/ut_GL_bitmaps.h"
#include "../gr/vf.h"                   /* der vektorfont */

#include "../xa/xa_msg.h"               // MSG_STD_ERR
#include "../xa/xa_uid.h"               // UID_ouf_coz
// #include "../xa/xa_edi__.h"             // ED_mode_enter
#include "../xa/xa_ed.h"                // ED_mode_*
#include "../xa/xa.h"                   // AP_Get_ConstPl_Z
#include "../xa/xa_ga.h"                // GA_getTexRefInd
#include "../xa/xa_mem.h"               // memspc..




 
/*=============== Externe Variablen: =======================================*/
// aus ut_DL.c:
extern DL_Att     *GR_ObjTab;                               // DB-DispList
extern long       GR_TAB_IND;

extern Att_ln     GR_AttLnTab[GR_ATT_TAB_SIZ];                //AttributTable
// extern GR_Att     GR_AttTab[GR_ATT_TAB_SIZ];                //AttributTable



// aus xa_ga.c:
extern ObjAtt  *GA_ObjTab;        // die PermanentAttributeTable
extern int     GA_recNr;          // die aktuelle Anzahl von Records

// ex xa_ui.c:
extern int       UI_InpMode;
extern char      UI_stat_view, UI_stat_hide;


// aus xa.c
extern AP_STAT   AP_stat;
extern  int       WC_modact_ind;    // -1=primary Model is active
extern Plane      WC_sur_act;     // die Konstruktionsebene
extern double     WC_sur_Z;       // Konstruktionsebene in Z verschieben !
extern int       WC_sur_ind;            // Index auf die ActiveConstrPlane
extern Mat_4x3   WC_sur_mat;            // TrMat of ActiveConstrPlane
extern Mat_4x3   WC_sur_imat;           // inverse TrMat of ActiveConstrPlane

extern int        WC_stat_bound;  // ON/OFF; Draw boundary of Flächen od nicht
extern double     AP_txsiz;       // Notes-Defaultsize
extern double     AP_txdimsiz;    // Dimensions-Text-size
// extern int        AP_txNkNr;            // Nachkommastellen


// Parameter zum Grafic-Font ex ut_gtx.c:
// extern double GR_tx_scale;            // Scalefaktor Text (was txSiz)
extern double GR_tx_ldExt;            // Ueberlaenge der Leaderlines
extern double GR_tx_ardx;             // Size Arrowhead in X-Dir
extern double GR_tx_ardy;             // Size Arrowhead in Y-Dir
// extern double GR_tx_ardp;             // Size Arrowhead in Y-Dir
extern double GR_tx_chw;              // Characterwidth (incl Abstand)
extern double GR_tx_chh;              // Characterheight (incl Abstand)
extern int    GR_tx_nkNr;             // Anzahl Nachkommastellen beim Text



// aus ../gr/ut_GLU.c:
extern Point     *GLT_pta;
extern long      GLT_pta_SIZ;



// aus ../gr/tess_su.c:
extern int TSU_mode;   // 0=normal darstellen; 1=speichern



// aus ../ci/NC_Main.c:
extern int       APT_dispSOL;           // 0=ON=shade; 1=OFF=symbolic
extern double    APT_ModSiz;


// ex ../xa/xa_ui_cad.c
extern int       IE_modify;






/*=============== Lokale Variablen: =======================================*/
// Point2     GL_ptArr20[70];
// Point2     GL_ptArr21[70];

// Point      GL_ptArr30[GL_ptArr30Siz]; // fuer Kreisboegen, Curves, ..
// Point      GL_ptArr31[GL_ptArr31Siz]; // fuer Kreisbögen, auch bei Cones ....
// Point      GL_ptArr32[GL_ptArr32Siz]; // fuer Kreisbögen, auch bei Cones ....






// static double     GL_sur_act;       // fuer den Drehmittelpunkt


// static unsigned long *GL_IndTab;  // Tabelle mit 1 2 3 4
static unsigned int *GL_IndTab;      // Tabelle mit 1 2 3 4
long GL_TAB_SIZ = 0;
long GL_TAB_INC = 10000;             // wie GR_TAB_INC

int  GL_modified = 0;
int  GL_rub_stat = 0;             // 0=uninitilized; 1=initilized; 2=box exists


static ObjDB   *GR_selTab = NULL; // table of selected dli's
static long    GR_Siz_selTab = 0;
static int     GR_nr_selTab = 0;





/* ======================================================================= */
/* Lighting - related |||||||||||||||||||||||||||||||||||||||||||||||||||| */
/* ======================================================================= */

//int GL_mode_lighting = OFF;                       /* ON: darker backside */


// Definition Lichtquellen
static GLfloat GL_light_2si[4] = {0.0f, 0.0f, 0.0f, 1.0f};

// ambientes Licht == helligkeit der Flaechen im Schatten
// amb. geht gleichmaessig auf alle Flaechen
// diffuses Licht = bestimmt max. Glanz ..
static GLfloat GL_mod_amb1[]   = {0.1f, 0.1f, 0.1f, 1.0f}; // normales Licht
static GLfloat GL_mat_spc1[]   = {0.5f, 0.5f, 0.5f, 1.0f}; // normal 
static GLfloat GL_light_amb1[] = {0.1f, 0.1f, 0.1f, 1.0f};  //0.5 0.5 0.5
static GLfloat GL_light_dif1[] = {0.4f, 0.4f, 0.4f, 1.0f};  //0.8 0.8 0.8
static GLfloat GL_light_spc1[] = {0.5f, 0.5f, 0.5f, 1.0f};


static GLfloat GL_mod_amb2[]   = {0.2f, 0.2f, 0.2f, 1.0f}; // Extralight
static GLfloat GL_mat_spc2[]   = {0.8f, 0.8f, 0.8f, 1.0f}; // Extralight ..
static GLfloat GL_light_amb2[] = {0.4f, 0.4f, 0.4f, 1.0f};  //0.5 0.5 0.5
static GLfloat GL_light_dif2[] = {0.8f, 0.8f, 0.8f, 1.0f};  //0.8 0.8 0.8
static GLfloat GL_light_spc2[] = {0.6f, 0.6f, 0.6f, 1.0f};


static GLfloat GL_mat_hili[]   = {0.8f, 0.8f, 0.8f, 1.0f}; // HiliLight ..
// static GLfloat GL_mat_spec[4]  = {0.5f, 0.5f, 0.5f, 1.0f}; // Extralight ..

static GLfloat GL_local_view[] = { 0.0f};

// Lightpos -20000.0f,  20000.0f, 1000.0f, nicht sichtbar !
//  0.3f,  0.3f,  0.3f, 0.0f}; // links vorn
// -0.3f,  0.3f,  0.3f, 0.0f}; // rechts vorn
// static GLfloat GL_light1_pos[4] = { 20000.0f,  20000.0f,    0.0f, 0.0f};
// static GLfloat GL_light0_pos[4] = {20.0f,  20.0f,    20.0f,  1.0f};
// static GLfloat GL_light1_pos[4] = { 0.6f,  0.6f,  0.6f, 0.0f};
// position: wenn Wert # 4 != 0, dann Position; else Vektor.
// Mit Vektor aber kein Glanz moeglich !
// static GLfloat GL_light0_pos[4] = { 3.3f, -3.3f,  1.3f, 0.5f};
static GLfloat GL_light0_pos[4] = { 0.0f, 400.0f, 400.0f, 1.0f};
// 0,0,0 = ganz hinten; -1,0,0 von rechts, 0,1,0 von halblinks: am besten.
// -1,-1,-1 od 1,1,1 lt Buch geht auch.
// static GLfloat GL_light0_pos[4] = {0.0f, 0.0f, -1.0f, 0.0f};
// static GLfloat GL_light0_pos[4] = {4000.1f, -1000.1f, 10.1f, 1.0f};
// static GLfloat GL_light0_pos[4] = {0.0, 0.0, 1.0, 1.0};
// static GLfloat GL_light1_pos[4] = { 0.3f,  0.3f,  0.3f, 0.5f};

// vector for spotlight
static GLfloat GL_light0_spo[4] = { 0.0f,  0.0f,  1.0f, 0.0f};

// static GLfloat GL_light0_col[4]= { 1.0f, 1.0f, 1.0f, 1.0f };  // white light
// static GLfloat GL_light1_col[4]= { .7f, .7f, 1.0f,1.0f };  // cold blue light

// nur Test:
static GLfloat GLcol_t1[] = {1.0, 1.0, 0.0, 1.0};
static GLfloat GLcol_black[] = {0.0, 0.0, 0.0, 0.0};
// static GLfloat GLcol_white[] = {255.0, 255.0, 255.0, 1.0};

// static GLfloat GL_diff1[] = {1.0, 1.0, 1.0, 0.6};
// static GLfloat GL_diff1[] = {0.6, 0.6, 0.6, 1.0};

// static GLfloat GL_amb1[] = {0.4, 0.4, 0.4, 1.0};
// static GLfloat GL_amb1[] = {0.8, 0.8, 0.8, 1.0};






/* ======================================================================= */
/* Line-Attribute - related |||||||||||||||||||||||||||||||||||||||||||||| */
/* ======================================================================= */
/* Die Ln_AttribTab enthaelt die Line - Attribut - statuse;
  Color, Lintyp, Thickness. Dieser index + dem DL_base_LnAtt
  sind der index des DL-entries f³r das Setzen der Attribute.
  JEDES objekt (line, Text..) hat einen glCall auf diesen DL-entry,
  der Farbe, Lintyp und Thickness setzt.
*/



//Ln_Attrib   Ln_AttribTab[Ln_Attrib_siz], def_stat;
// static long        Ln_Attrib_nxt = 0;


//                                 voll    - . -   - - -   --- ---
static GLint       LtypTab1[] = {     1,      3,       1,       2};
static GLushort    LtypTab2[] = {0xFFFF,  16377,  0x0FFF,  0x0FFF};
  // 16377 = --xxxxxxxxx--x


/* thickness - table; for Lines and for points LTHICK_1 - LTHICK_4 */
// static float LThickTab[] = {1.5, 3.0, 4.0, 6.0};
// static float PThickTab[] = {3.0, 4.0, 6.0, 8.0};



#define COL_ANZ 15

static GLfloat GL_col_tab [COL_ANZ][4] = {
  0.0,  0.0,  0.0, 1.0,   //  0 ATT_COL_BLACK = Default
  0.1,  0.1,  0.6, 1.0,   //  1 ATT_COL_        Blue ...
  1.0,  0.0,  0.0, 1.0,   //  2 ATT_COL_RED
  0.0,  1.0,  0.0, 1.0,   //  3 ATT_COL_GREEN
  0.0,  0.0,  1.0, 1.0,   //  4 ATT_COL_BLUE
  1.0,  1.0,  0.0, 1.0,   //  5 ATT_COL_YELLOW
  1.0,  0.0,  1.0, 1.0,   //  6 ATT_COL_MAGENTA
  0.0,  1.0,  1.0, 1.0,   //  7 ATT_COL_CYAN
  1.0,  1.0,  1.0, 1.0,   //  8 ATT_COL_WHITE
  0.8,  0.2,  0.2, 1.0,   //  9 ATT_COL_HILI     Red 0.8,  0.2,  0.2, 1.0,
  // 0.4,  0.9,  0.9, 1.0,     //  9 hili (rot) 0.8,  0.2,  0.2, 1.0,      XOR
  // 0.6,  0.7,  0.7, 1.0,     // 10 COL_DIMMED (0.6 0.7 0.7) auch 0.7,0.2,0.7
  0.4,  0.4,  0.7, 1.0,   // 10 ATT_COL_DIMMED (0.6 0.7 0.7) auch 0.7,0.2,0.7
  // 0.5,  0.5,  0.65,1.0,     // 11 blaugrau f. faces (0.4,  0.5,  0.5)
  0.75, 0.55, 0.55,1.0,   // 11 ATT_kupferrot f. faces war 0.5,0.4,0.4
  // 0.95, 0.95, 0.0, 1.0,     // 12 gelb f. faces
  0.58, 0.58, 0.68,1.0,   // 12 ATT_ stahlblau f. faces                 DEFCOL
  0.55, 0.55, 0.55,1.0,   // 13 ATT_ grau f. faces (0.45, 0.45, 0.45)
  0.55, 0.15, 0.15,1.0};  // 14 ATT_ ???  (0. 0.25 0.25)


// Hintergrundfarbe  Backgroundcolour
// static GLfloat GL_col_bg[4] = {0.0, 0.25, 0.25, 0.0};      // Oliv
// static GLfloat GL_col_bg[4] = {0.9f, 0.9f, 0.9f, 1.0f};    // hellgrau
// static GLfloat GL_col_bg[4] = {0.75, 0.75, 0.75, 1.0f};    // hellgrau
// static GLfloat GL_col_bg[4] = {0.4, 0.4, 0.4, 1.0};    // mittelgrau
// static GLfloat GL_col_bg[4] = {0.5, 0.5, 0.5, 1.0};    // mittelgrau
// static GLfloat GL_col_bg[4] = {0.75, 0.75, 0.75, 1.0};    // ?
// static GLfloat GL_col_bg[4] = {0.5, 0.5, 0.5, 1.0};    // mittelgrau
// static GLfloat GL_col_bg[4] = {1.f, 1.f, 1.f, 0.0f};  // weiss
// .3f, .4f, .6f, 1.0f hellblau
// static GLfloat GL_col_bg[4] = {.8f, .9f, 1.0f, 1.0f};  // hellblau
// static GLfloat GL_col_bm[4] = {0.55, 0.62, 0.62, 1.0};    // ?? 

// Error-Hintergrundfarbe  Backgroundcolour
static GLfloat GL_col_bg[4] = {0.6, 0.6, 0.6, 1.0};       // normal, gray
static GLfloat GL_col_be[4] = {0.6,  0.4,  0.4,  1.0};    // error,  red


// static ColRGB  actCol, newCol;
       ColRGB  GL_defCol;   // AP_defcol
       ColRGB  GL_actCol;   // NICHT AP_actcol sondern col of active sur or sol
       int     GL_actTex;   // TexRef-index

// static unsigned char GL_transpTab[4] = { 0, 64, 16, 2};
static unsigned char GL_transpTab[4] = { 254, 250, 128, 64};
// static unsigned char GL_transpTab[4] = { 254, 254, 128, 128};
// static unsigned char GL_transpTab[4] = { 254, 254, 32, 32};
// index 1 und 2 used !  1=50%, 2=80%;    0=unsichtbar 255=solid
static int  GL_stat_blend = 0;                      // transparent = OFF








/* ======================================================================= */
/* Viewport related |||||||||||||||||||||||||||||||||||||||||||||||||||||| */
/* ======================================================================= */


// static double GL_Aug_X = 0., GL_Aug_Y = 0., GL_Aug_Z = 50.; 


// GL_Scr_Siz_X/Y: Size of the grafic-window (=Viewport) in ScreenCoords;
// top left is 0,0. Als doubles !
static double GL_Scr_Siz_X, GL_Scr_Siz_Y;

static double GL_Svfakt;     // Screen-Verzerrungsfaktor;


// GL_Scale depends on GL_ModSiz. See GL_Set_Scale() AP_scale
static double GL_Scale = 1.0;
static double GL_Scale_back = 1.0;
static double GL2D_Scale = 1.0;
static double GL_SclNorm;



// ConstrPlane; Kopie von WC_sur_act ? In z verschoben um WC_sur_Z.
static Plane  GL_constr_pln;


/// Viewplane (active displayplane with the GL-RotationCenterpoint GL_cen)
///   GL-RotationCenterpoint GL_cen is always in this plane.
///   Always parall. to X-Y-Plane. Origin.z = z-value of RotationCenterpoint.
///   Changes with definition of RotationCenterpoint
static Plane  GL_view_pln;


/// GL_cen: centerpoint of the Viewport (in UserCoords).
/// Is always in GL_view_pln
static Point  GL_cen = {0., 0., 0.};
// GL_cen.z ist immer Z-ViewPlane  (GL_query_ViewZ, GL_Set_Cen, GL_GetCen)

static Point GL_icons_Ori;  // position of 2D-icons in userCoords


/// \brief screenOrientation
/// \code
/// vectors have userCoordinates; the length is fixed to 1.
/// GL_eyeX goes from the screenCenter to the eye
/// GL_eyeZ goes up - always vertical, parallel to the window outline
/// GL_eyeY always is horizontal, parallel to the window outline
/// \endcode
static Vector GL_eyeX={0.,0.,1.};
static Vector GL_eyeZ={0.,1.,0.};
static Vector GL_eyeY={1.,0.,0.};

// alte Version:
// static double GL_cen_X, GL_cen_Y, GL_cen_Z;
// static double GL_ang_X, GL_ang_Y, GL_ang_Z;
// Vector            GL_view_vz;



// ????????????: ViewportSize (der sichtbare Bereich), in Userkoord's.

// GL_Siz_ is Modelsize/2; used for the OpenGL-Viewing-volume
static double GL_Siz_X, GL_Siz_Y, GL_Siz_Z;


static GLdouble GL_ModSiz = 500., GL_ModScale;


static double GL_box_X1,GL_box_X2,GL_box_Y1,GL_box_Y2;



static int GL_mouse_x_act = 0, GL_mouse_y_act = 0;
//int GL_mouse_x_old = 0, GL_mouse_y_old = 0;
//int GL_mouse_x_box = 0, GL_mouse_y_box = 0;

static Point    GL_actScrPos; ///< curPos in screenCoords
static Point    GL_actUsrPos; ///< curPos in userCoords on viewPlane


/// FUNC_Idle|FUNC_Pan|FUNC_RotCen|FUNC_Rot; aus base
static int      GL_Func_Act;


// die zum Umrechnen Screen- > UserCoord erforderl. Tabellen
static GLint    GL_Viewp[4];     // x-left, y-low, width, heigth
static GLdouble GL_MatMod[16], GL_MatProj[16];


static int GL_actView = FUNC_ViewIso;

static int GL_initMode = 0;


// ../gr/ut_GL.h:
// static int GL_pickSiz  = 8;        // SelectionDistance in Pixel (ScreenCoords)





//================================================================
// Application related ....
//================================================================
 

static int GL_mode_draw_select = GR_MODE_DRAW;    // 0
                              // GR_MODE_SELECT      1
                              // GR_MODE_FEEDBACK    2
static int GL_mode_feed = 0;                          // 0=Print, 1=ReScale
static int GL_mode_wire_shade =  GR_STATE_WIRE;

static int GL_vcSelStat = 0;  // 0=vectorSelect not active; else=yes,
                              // index of vec to display; -1=none
static int GL_plnSelStat = 0; // 0=planeSelect not active; else=yes.
 
       int GL_ViewModTimStamp = 0; // timeStamp of last view-modification;
                              // is incremented with pan/rotate/scale/resize view




/* ======================================================================= */
/* DispList related |||||||||||||||||||||||||||||||||||||||||||||||||||||| */
/* ======================================================================= */
/*
DL_base__ ist der offset, von wo weg die normalen DB-Objekte in der DL liegen;
        zB 200. Von 0 - 199 sind dann die Fonts und die Symbole.
DL_siz ist die Anzahl DL-Plaetze fuer DB-Objekte (ohne DL_base__)

GR_TAB_IND ist der naechste freie DispList-Platz (ohne DL_base__; anfangs also 0)
  Used fuer end of redraw.
// GL_ActInd ist ident mit GR_TAB_IND (auch der naechste freie DispList-Platz)

Der echte DL-Index ist natuerlich ind + DL_base__ !



Die GL_Draw - Routinen retournieren den DL-Index, der dann in der DB
 gespeichert werden muss; zum Loeschen bzw aendern erforderlich.


DL-Symbol-Liste (von 1-DL_base__):
--------------------------------
001     -
002-031 Temporäre Objekte.
032-128 Bitmapobjekte des Alfatext-Font
129     Der ScaleBackBefehl.
130-138 Bitmap-Symbole
139     der Vektor
140-149 Vektor-Symbole (skaliert)
150-199 Ln_AttribTab (DL_base_LnAtt + Ln_Attrib_siz)
200-296 GrafFont (200=Blank=Ascii 32)
297     DL_Ind_Scale2D
298     Point-Typ-Call DL_base_PtAtt
299     DL_Img_ScBack  (scale images)
300     DL_base__

*/

static GLuint
  // DL_Ind_nxt    =   0,         // der naechste freie Displistplatz
  DL_Ind_tmp    =   1,         // der naechste frei Platz fur tempObj
  DL_base_font1 =  32,         // Ende des tempObj-Bereichs
  DL_Ind_ScBack = 129,  // Befehl zum Zurueckskalieren f. glPopMNatrix
  DL_base_LnAtt = 150,  // LineAttributes until DL_base_PtAtt
  DL_base_PtAtt = 195,  // PointAttributes until DL_base_font2
  DL_base_font2 = 200,  // Ende des tempObj-Bereichs
  DL_Ind_Scl2D  = 297,  // DL_Set_Scl2D
  DL_Ind_Cen2D  = 298,  // 2D-screenCenter  DL_Set_Cen2D
  DL_Img_ScBack = 299,  // scale  images
  DL_base_mod   = 300,  // hier beginnt die DL fuer die Overlay-Models.
  DL_base__;              // ab hier die normalen Objekte.

//char    DL_sho_Tab[DL_siz];

static  int  DL_FontOff;       //=DL_base_font2-32; 





/* #define DL_Ind_max     50 */
/* #define DL_Ind_offset   4 */

/* GLuint DispList[50], DL_offset, DL_base__; */
/* long   DispList_ind, DispList_anz; */





// local prototypes:
// void   GL_Select1  (int mode, long* obj_sel, GLuint hits, GLuint *selectBuf);
GLuint GL_fix_DL_ind  (long*);






//=====================================================================
  int GL_Test () {
//=====================================================================
// Typ_Att_hili1

  Vector     vcy;

  printf("GL_Test \n");

  UT3D_vc_perp2vc (&vcy, &GL_eyeZ, &GL_eyeX);

  UT3D_stru_dump (Typ_VC, &GL_eyeX, "GL_eyeX: ");
  UT3D_stru_dump (Typ_VC, &vcy,     "    vcy: ");
  UT3D_stru_dump (Typ_VC, &GL_eyeZ, "GL_eyeZ: ");

  // GL_cen = ScreenCenter in usercoords
  GL_actUsrPos = GL_GetViewPos ();

  // CurPos in usercoords:
  UT3D_stru_dump (Typ_PT, &GL_actUsrPos, "ViewPos: ");

  // Sichtlinie (zeigt vom Mittelpkt zum Auge)
  GR_Disp_vc (&GL_eyeX, &GL_actUsrPos, 9, 0);
  GR_Disp_vc (&vcy, &GL_actUsrPos, 8, 0);
  GR_Disp_vc (&GL_eyeZ, &GL_actUsrPos, 7, 0);

  return 0;

}

/* UNUSED
//================================================================
  int GL_selSubPt (int *iSeg, int *iPt,
                   int iNr, Point *pTab, Point *ptx) {
//================================================================
// test if one point of polygon pTab is equal tp point ptx and which
//   segment was selected
// Input:
//   pTab[iNr]   polygon
// Output:
//   retCod >= 0 ptx == pTab[retCod];
//          -1   no point slected ..


// see also GL_selPt
  int     i1, irc;
  double  dx, dy, d1;
  Point2  p21, p22;
  Point   actScrPos, actUsrPos;


  // printf("GL_selSubPt %d\n",iNr);
  // UT3D_stru_dump (Typ_PT, ptx, "ptx: ");
  // for(i1=0;i1<iNr;++i1) UT3D_stru_dump (Typ_PT, &pTab[i1], "p[%d]: ",i1);


  // get GL_actScrPos & GL_actUsrPos
  GL_GetActSelPos (&actUsrPos, &actScrPos);


  irc = -1;

  for(i1=0; i1<iNr; ++i1) {

    // Vertex --> 2D
    GL_Uk2Sk (&p22.x, &p22.y, &d1, pTab[i1].x, pTab[i1].y, pTab[i1].z);
      // UT3D_stru_dump (Typ_PT2, &p22, " pTab[%d]",i1);

    dx = fabs(actScrPos.x - p22.x);
    dy = fabs(actScrPos.y - p22.y);
    if((dx < GL_pickSiz)&&(dy < GL_pickSiz)) {
      irc = i1;
      break;
    }
  }

  return irc;

}
*/
 

//================================================================
  int GL_ck_sel_PT (Point *pt1) {
//================================================================
/// \code
/// compare pt1 with last mouseposition
/// Input:
/// Output:
///   Retcod:   0  point is inside selection-area
///             1  outside; no selection ..
/// \endcode

// change pt1 into 2D-coordinates; test 2D-distance from cursorPosition.
// see also GL_selPt


  double  dx, dy;
  Point   p21;


  // change point into screenCoords
  GL_Uk2Sk (&p21.x, &p21.y, &p21.z, pt1->x, pt1->y, pt1->z);
    // UT3D_stru_dump (Typ_PT, &p21, " p21");


  // mit startpos vergleichen (immer)
  dx = fabs(GL_actScrPos.x - p21.x);
  dy = fabs(GL_actScrPos.y - p21.y);
    // printf(" dSc=%f %f / %d\n",dx,dy,GL_pickSiz);
  if((dx < GL_pickSiz)&&(dy < GL_pickSiz)) return 0;

  return 1;

}


//================================================================
  int GL_selPt (int *is, int *pSel,  Point *spt,
                int ptNr, Point *pTab,
                int iNr,  int *iTab) {
//================================================================
// den selektieren Punkt am Polygon feststellen und segmentNr liefern
// Each segment is defined by a record in iTab.
// Es werden nur Vertices getestet, die in iTab definiert sind.
// Testen, ob dieser Vertex im Fangkreis liegt;
// In iTab sind alle Punkte definiert, die (als Limitpoints) getestet werden
//   sollen.
// Test: ist Curpos == Limitpoint:
// wenn ja:    pSel=pointNr (index into pTab; 0=first).
// wenn nicht: pSel=-1

// Input:
//   iNr         size of list iTab
//               -1  test all points; do not use iTab;
//               -2 test only 1. and last point; do not use iTab
//   iTab        ein Liste von Indices into pTab, terminated with -1
//               gives pointIndex of first point of next segment.
//               eg iTab[0]=5    // 1.segment: pTab[0-4]
//               eg iTab[1]=9    // 2.segment: pTab[5-8]
//   ptNr, pTab  ein 3D-Polygon der Kurve
// Output:
//   is          segmentNr of selected object; 1=first.
//               selection between points: pSel=0; is=segmentNr (1=first);
//   pSel        if point of pTab selected: pSel=pointNr (0=first);
//               selection between points: pSel=-1
//   spt         the selected point in userCoords
//   retCod      0 = ok;
//              -1 = cannot find selectPosition on this polygon ..

// see also GL_selSubPt GL_ck_sel_PT


  int     iStdTab[2];
  int     irc, i1, ii, sOld, sAct, iSegLastPt;
  double  d1, dx, dy, dMax, ps;
  Point2  p21, p22;
  Point   actScrPos, actUsrPos;


  if(iNr == -2) {
      // printf(" set iStdTab\n");
    iStdTab[0] = ptNr - 1;
    iStdTab[1] = -1;
    iNr = 1;
    iTab = iStdTab;
  }

  // printf("GL_selPt ptNr=%d iNr=%d\n",ptNr,iNr);
  // for(i1=0;i1<ptNr;++i1)UT3D_stru_dump(Typ_PT2,&pTab[i1],"pTab[%d]",i1);
  // if(iNr > 0) {i1=0;
  // while(iTab[i1] >= 0) {printf(" iTab[%d]=%d\n",i1,iTab[i1]);++i1;}}
  // printf(" GL_pickSiz=%d\n",GL_pickSiz);


  // get GL_actScrPos & GL_actUsrPos
  GL_GetActSelPos (&actUsrPos, &actScrPos);
    // UT3D_stru_dump (Typ_PT, &actScrPos, " actScrPos");
    // UT3D_stru_dump (Typ_PT, &actUsrPos, " actUsrPos");

  // change 1. point into screenCoords
  GL_Uk2Sk (&p21.x, &p21.y, &d1, pTab[0].x, pTab[0].y, pTab[0].z);
    // UT3D_stru_dump (Typ_PT2, &p21, " pTab[0]");

  // mit startpos vergleichen (immer)
  dx = fabs(actScrPos.x - p21.x);
  dy = fabs(actScrPos.y - p21.y);
  // printf(" dSc=%f %f / %d\n",dx,dy,GL_pickSiz);
  if((dx < GL_pickSiz)&&(dy < GL_pickSiz)) {
    *pSel = 0;
    *is = 1;
    goto L_done;
  }

  dMax = UT_VAL_MAX;
  ii   = 0;
  sOld = 1;



  //----------------------------------------------------------------
  for(i1=1; i1<ptNr; ++i1) {
    // soll der Punkt getestet werden ? Nur wenn er explizit in der iTab
    // definiert ist.

    //-------------------------------------
    // fix ii=segmentNr and
    //     iSegLastPt=index of last point of active segment
    if(iNr >= 0) {
      L_ck_itab:
      if(iTab[ii] < i1) {
        if(ii < iNr) {++ii; goto L_ck_itab;}
      }
      iSegLastPt = iTab[ii];

    } else {
      ii = i1 - 1;
      iSegLastPt = i1;
    }
      // printf(" i1=%d ii=%d iSegLastPt=%d\n",i1,ii,iSegLastPt);


    //-------------------------------------
    // Vertex --> 2D (screenCoords)
    GL_Uk2Sk (&p22.x, &p22.y, &d1, pTab[i1].x, pTab[i1].y, pTab[i1].z);
      // UT3D_stru_dump (Typ_PT2, &p22, " pTab[%d]",i1);

    if(iSegLastPt == i1)   {      // test ob genau am Punkt ...
      // wenn der Screenabstand (in Pixeln) kleiner als die Toleranz,
      // sofort diesen Vertex selektieren
      dx = fabs(actScrPos.x - p22.x);
      dy = fabs(actScrPos.y - p22.y);
      // printf(" dSc=%f %f / %d\n",dx,dy,GL_pickSiz);
      if((dx < GL_pickSiz)&&(dy < GL_pickSiz)) {
        *pSel = i1;
        *spt = pTab[i1];    // 2012-01-11
        *is = ii + 1;
          // printf(" found1 %d\n",i1);
        goto L_done;
      }
    }


    // test ob actScrPos am Teilsegment p21-p22 liegt
    sAct = UT2D_pt_projpt2pt ((Point2*)spt,NULL,(Point2*)&actScrPos,&p21,&p22);
    if(sAct != 0) goto L_cont1;

      // printf(" found2 %d\n",i1);
    // get intersectionPoint spt
    UT2D_pt_projpt2pt ((Point2*)spt,NULL,(Point2*)&actScrPos,&p21,&p22);
    // check distance spt-actScrPos
    dx = fabs(actScrPos.x - spt->x);
    if(dx > GL_pickSiz) goto L_cont1;
    dy = fabs(actScrPos.y - spt->y);
    if(dy > GL_pickSiz) goto L_cont1;
    // get parameter of spt along p21-p22
    // UT2D_parLn_pt2pt (&ps, &p21, &p22, (Point2*)spt);
    UT2D_par_3pt (&ps, &p21, &p22, (Point2*)spt);

      // printf(" ps=%f\n",ps);
    // compute spt from parameter
    UT3D_pt_evpar2pt (spt, ps, &pTab[i1-1], &pTab[i1]);
    *pSel = -1;
    *is = ii + 1;
    goto L_done;



    L_cont1:
    sOld = sAct;
    p21 = p22;
  }

    printf("ex GL_selPt Err -1\n");
  return -1;


  // L_found:

  

  //----------------------------------------------------------------
  L_done:

    // printf("ex GL_selPt pSel=%d is=%d\n",*pSel,*is);
    // UT3D_stru_dump (Typ_PT, spt, "  spt:");
    // exit(0);

  return 0;

}


//================================================================
  int GL_Tex_End () {
//================================================================
// Exit Texture


  // printf("GL_Tex_End .......... \n");


    glDisable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);


    glMatrixMode (GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);  // reset ..

  glEndList();

  return 0;

}

 //================================================================
  int GL_Tex_ckSiz (int ix, int iy) {
//================================================================
/// \code
/// test if texture could be loaded by OpenGL
/// RetCod:
///   0          OK; can load texture,    -1 NO; texture too big ..
/// \endcode


  static int texSize = 0;
  GLint width;


  // printf("GL_Tex_ckSiz %d %d siz=%d\n",ix,iy,texSize);


  if(!texSize)  {
    glGetIntegerv (GL_MAX_TEXTURE_SIZE, &texSize);
      printf(" GL_MAX_TEXTURE_SIZE = %d\n",texSize);
  }


  if((ix < texSize) && (iy < texSize)) return 0;

  glTexImage2D (GL_PROXY_TEXTURE_2D,   // | GL_TEXTURE_2D
               0,                      // level,   
               GL_RGB,                 // internalFormat; GL_RGB | 3
               ix, iy,                 // width, height,
               0,                      // border,  
               3, //                 // format,GL_BGR|GL_BGR_EXT|GL_LUMINANCE
               GL_UNSIGNED_BYTE,       // data-type,
               NULL);



  glGetTexLevelParameteriv (GL_PROXY_TEXTURE_2D,
                            0,
                            GL_TEXTURE_WIDTH,
                            &width);
    // printf(" textureWidth = %d\n",width);

  if(!width) return -1;  // error if (width == 0)

  return 0;

}


//=====================================================================
  int GL_EndList () {
//=====================================================================
// damit kein include GL erforderlich ist

  // printf("GL_EndList %d\n",GL_stat_blend);


  if(GL_stat_blend != 0) {
    glDisable (GL_BLEND);
    glEnable (GL_DEPTH_TEST);
    GL_stat_blend = 0;
  }


  glEndList ();
  return 0;
}


//=====================================================================
  int GL_EndList1 (int mode) {
//=====================================================================
// mode = 0 fuer Lines, Curves .. (ohne glDisable (GL_LIGHTING);)
// mode = 1 fuer Flaechen (schattieren .. = Default);

  if(mode < 1) glEnable (GL_LIGHTING);

  glEndList ();
  return 0;
}



//=====================================================================
  int GL_EndList2 () {
//=====================================================================

  // glMatrixMode(GL_MODELVIEW);
  glPopMatrix ();

  glEnable (GL_LIGHTING);
  glEndList ();

  return 0;

}


//=====================================================================
  void GL_Reframe () {
//=====================================================================

  // printf("GL_Reframe siz=%f,%f,%f\n",GL_Siz_X,GL_Siz_Y,GL_Siz_Z);


    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();



    GL_box_X1 = GL_cen.x - GL_Siz_X;
    GL_box_X2 = GL_cen.x + GL_Siz_X;
    
    GL_box_Y1 = GL_cen.y - GL_Siz_Y;
    GL_box_Y2 = GL_cen.y + GL_Siz_Y;

/*
    GL_box_Z1 = GL_cen_Z - GL_Siz_Z;
    GL_box_Z2 = GL_cen_Z + GL_Siz_Z;
*/


    GL_Reframe1 ();


}


//=====================================================================
  void GL_Reframe2 () {
//=====================================================================
// TEST perspektivisch

    // hier ist eine Funktion STATT dem GL_Reframe1 fuer perspektivische
    // Projektion mit glFrustum erforderlich ...
    // gluPerspective (16., 1.5, -GL_Siz_Z,  GL_Siz_Z);
     glFrustum (-1000.f, 1000.f, -1000.f, 1000.f, 1000.1f, 20000.f);
     // glFrustum (-100.f, 100.f, -100.f, 100.f, 100.f, 200.f);
     // glFrustum (-10.f, 10.f, -10.f, 10.f, 0.1f, 200.f);
     // glFrustum (-.1f, .1f, -.1f, .1f, 0.1f, 200.f);
     // glFrustum (-GL_ModSiz, GL_ModSiz,
                // -GL_ModSiz, GL_ModSiz,
                // GL_Siz_Z/2,  GL_Siz_Z);


    // gluPerspective (3.f, 1.5f, 100.f,  -100.f);
    // gluPerspective (5., 1.5, 1., 100);


  return;


}

//=====================================================================
  void GL_Reframe1 () {
//=====================================================================
// Parallelprojektion


  //TX_Print("GL_Reframe1 siz=%f,%f,%f",GL_Siz_X,GL_Siz_Y,GL_Siz_Z);



/*
    TX_Print("GL_Reframe1 x=%f,%f y=%f,%f z=%f,%f",
                   GL_box_X1,GL_box_X2,
                   GL_box_Y1,GL_box_Y2,
                   GL_box_Z1,GL_box_Z2);
*/


/*
    glOrtho (GL_box_X1, GL_box_X2, 
             GL_box_Y1, GL_box_Y2,
             GL_box_Z1, GL_box_Z2);
*/





/*
  if (GL_Scr_Siz_X < GL_Scr_Siz_Y) {
    //glOrtho (-GL_ModSiz, GL_ModSiz, -GL_ModSiz*h/w, GL_ModSiz*h/w, GL_ModSiz_Z_LO, GL_ModSiz_Z_HI);
    glOrtho (GL_box_X1, GL_box_X2, 
             GL_box_Y1*GL_Scr_Siz_Y/GL_Scr_Siz_X, GL_box_Y2*GL_Scr_Siz_Y/GL_Scr_Siz_X,
             GL_box_Z1, GL_box_Z2);

  } else {
    //glOrtho (-GL_ModSiz*w/h, GL_ModSiz*w/h, -GL_ModSiz, GL_ModSiz, GL_ModSiz_Z_LO, GL_ModSiz_Z_HI);
    glOrtho (GL_box_X1*GL_Scr_Siz_X/GL_Scr_Siz_Y, GL_box_X2*GL_Scr_Siz_X/GL_Scr_Siz_Y, 
             GL_box_Y1, GL_box_Y2,
             GL_box_Z1, GL_box_Z2);

  }
*/


  if (GL_Scr_Siz_X <= GL_Scr_Siz_Y) {
    glOrtho (-GL_ModSiz, GL_ModSiz,
             -GL_ModSiz*GL_Svfakt, GL_ModSiz*GL_Svfakt,
             -GL_Siz_Z,  GL_Siz_Z);
  } else {
    glOrtho (-GL_ModSiz*GL_Svfakt, GL_ModSiz*GL_Svfakt,
             -GL_ModSiz, GL_ModSiz,
             -GL_Siz_Z,  GL_Siz_Z);
  }



/*
Das folgende bringt nix
  // den Mittelpunkt genau in die Mitte der beiden Z-Ebenen legen !
  // dazu die aktuelle Z-Höhe holen
  if (GL_Scr_Siz_X <= GL_Scr_Siz_Y) {
    glOrtho (
             -GL_ModSiz-GL_cen_X, GL_ModSiz-GL_cen_X,
             -GL_ModSiz*GL_Svfakt-GL_cen_Y, GL_ModSiz*GL_Svfakt-GL_cen_Y,
             -d2,  d2);
  } else {
    glOrtho (
             -GL_ModSiz*GL_Svfakt-GL_cen_X, GL_ModSiz*GL_Svfakt-GL_cen_X,
             -GL_ModSiz-GL_cen_Y, GL_ModSiz-GL_cen_Y,
             -d2,  d2);
}
*/



/*
  // Diese Version ist mit glScaled; geht gut aber mit den Clipping-Problemen.
  //glOrtho (-GL_ModSiz, GL_ModSiz, -GL_ModSiz, GL_ModSiz, -GL_ModSiz, GL_ModSiz);
  // Establish clipping volume (left, right, bottom, top, near, far)
  if (GL_Scr_Siz_X <= GL_Scr_Siz_Y) {
    //glOrtho (-GL_ModSiz, GL_ModSiz, -GL_ModSiz*h/w, GL_ModSiz*h/w, GL_ModSiz_Z_LO, GL_ModSiz_Z_HI);
    glOrtho (-GL_ModSiz, GL_ModSiz,
             -GL_ModSiz*GL_Scr_Siz_Y/GL_Scr_Siz_X, GL_ModSiz*GL_Scr_Siz_Y/GL_Scr_Siz_X,
             -GL_ModSiz*1000., GL_ModSiz*1000.);

  } else {
    //glOrtho (-GL_ModSiz*w/h, GL_ModSiz*w/h, -GL_ModSiz, GL_ModSiz, GL_ModSiz_Z_LO, GL_ModSiz_Z_HI);
    glOrtho (-GL_ModSiz*GL_Scr_Siz_X/GL_Scr_Siz_Y, GL_ModSiz*GL_Scr_Siz_X/GL_Scr_Siz_Y,
             -GL_ModSiz, GL_ModSiz,
             -GL_ModSiz*1000., GL_ModSiz*1000.);

  }
*/


}







//=====================================================================
  void GL_Translate () {
//=====================================================================

  /* GLdouble augX, augY, augZ,   cenX, cenY, cenZ,   upX, upY, upZ; */


  //TX_Print("GL_Translate %f",GL_Scale);

  /* TX_Print("   %f %f %f",GL_cen_X,GL_cen_Y,GL_cen_Z); */
  /* TX_Print("   %f %f %f",GL_ang_X,GL_ang_Y,GL_ang_Z); */

  DL_Set_Cen2D ();   // set new 2D-screenCenter-position
  DL_Set_Scl2D ();   // set horizontal-vector

  return;

  //glMatrixMode (GL_MODELVIEW);
  //glLoadIdentity ();


  // fuer die neue Methode hier glOrtho zu !
  GL_Reframe ();


  GL_Translate1 ();


  /* alle spaeter fuer Proj erforderlichen Matrizen lesen */
  GL_GetGLMat0 ();

  /* glPopMatrix (); */

  /* Die Normalebene auf die Displayplane errechnen. */
  // ersetzt durch GL_eyeX
  // GL_GetNormal0 ();

}




//=====================================================================
  void GL_Translate1 () {
//=====================================================================
//  Scale, Rotate, Translate
// Die Projektionsmatrix ist immer auf null. Verändern bringt nix.


  //TX_Print("GL_Translate1 Scale=%f, cen=%f,%f,%f",GL_Scale,GL_cen_X,GL_cen_Y,GL_cen_Z);


  //glMatrixMode (GL_MODELVIEW);
  //glLoadIdentity ();

/*
  GL_Aug_X =  0.;
  GL_Aug_Y =  0.;
  GL_Aug_Z =  5.;

  TX_Print("GL_Translate1 Aug=%f,%f,%f",GL_Aug_X, GL_Aug_Y, GL_Aug_Z);


  gluLookAt(
            GL_Aug_X, GL_Aug_Y, GL_Aug_Z, 
            0., 0., 0.,
            0., 1., 0.);

*/




  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();


  glScaled (GL_Scale, GL_Scale, GL_Scale);


  // glRotated (GL_ang_X, 1.0, 0.0, 0.0);
  // glRotated (GL_ang_Y, 0.0, 1.0, 0.0);
  // glRotated (GL_ang_Z, 0.0, 0.0, 1.0);

  //d1 = WC_ask_actZsur () / 2.0;
 

  glTranslated (-GL_cen.z, -GL_cen.z, -GL_cen.z);





}


//=====================================================================
  void GL_Transl_VertP (Point *pt1) {
//=====================================================================
// einen Vertex skalieren und translieren und raus. Unused.

/*
  double  pto[3];
  Point   GL_Ori = {50., 0., 50.};

  //TX_Print("GL_Translate1 Scale=%f, cen=%f,%f,%f",GL_Scale,GL_cen_X,GL_cen_Y,GL_cen_Z);

  pto[0] = pt1->x / 3. - GL_Ori.x;
  pto[1] = pt1->y / 3. - GL_Ori.y;
  pto[2] = pt1->z / 3. - GL_Ori.z;

  glVertex3dv (pto);
*/

  // glVertex3d (pt1->x, pt1->y, pt1->z);
  glVertex3dv ((double*)pt1);


}


/*
//=====================================================================
  int GL_Set_Light () {
//=====================================================================
// nicht aktiv.
// keine Lightpos: immer von vorn beleuchtet.



  float   f1;
  GLfloat light_amb[4];

  printf("GL_Set_Light Scale=%f\n",GL_Scale);
  return 0;

  f1 = GL_Scale / 20.f;
  if(1 > 1.f) f1 = 1.f;
  light_amb[0] = f1;
  light_amb[1] = f1;
  light_amb[2] = f1;
  light_amb[3] = 1.f;


  // 1) Lightpos mitdrehen;
  // 2) Lightpos nah ran ans Objekt !!
      // glLightModelf (GL_LIGHT_MODEL_TWO_SIDE, 0); == Default!

  // glEnable  (GL_LIGHT1);
  // GL_light0_pos[0] /= 1.1;
  // GL_light0_pos[1] /= 1.1;
  // GL_light0_pos[2] /= 1.1;
  // printf(" lPos=%9.1f %9.1f %9.1f\n",GL_light0_pos[0],GL_light0_pos[1],
        // GL_light0_pos[2]);
      glLightfv(GL_LIGHT0, GL_AMBIENT,  light_amb);  // hellt stark auf !
      // glLightfv(GL_LIGHT0, GL_DIFFUSE,  GL_light_dif);
      glLightfv(GL_LIGHT0, GL_SPECULAR, GL_light_spc); // zieml unnutz
      glLightfv(GL_LIGHT0, GL_POSITION, GL_light0_pos);
      // glLightf  (GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.2f);
  glEnable  (GL_LIGHT0);

      glLightfv(GL_LIGHT1, GL_AMBIENT,  light_amb);  // hellt stark auf !
      // glLightfv(GL_LIGHT1, GL_DIFFUSE,  GL_light_dif);
      glLightfv(GL_LIGHT1, GL_SPECULAR, GL_light_spc); // zieml unnutz
      glLightfv(GL_LIGHT1, GL_POSITION, GL_light0_pos);
      // glLightf (GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.8f);
  glEnable  (GL_LIGHT1);


      // glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
      // glColorMaterial (GL_BACK,  GL_AMBIENT_AND_DIFFUSE);
      // glColorMaterial (GL_BACK,  GL_DIFFUSE);
      // glColorMaterial (GL_FRONT,  GL_DIFFUSE);
      glColorMaterial (GL_FRONT,  GL_AMBIENT); // machts gut glaenzend !!
      glColorMaterial (GL_FRONT,  GL_SPECULAR);
      // glColorMaterial (GL_BACK,  GL_AMBIENT);
      // glColorMaterial (GL_BACK,  GL_SPECULAR);
      // glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
  glEnable (GL_COLOR_MATERIAL);

      // glMaterialfv (GL_FRONT, GL_SPECULAR, GL_mat_spec);
      // glMaterialfv (GL_BACK, GL_SPECULAR, GL_mat_spec);
      // // Reflexion - 0 bis 128 macht grobe Fehler !
      // glMateriali  (GL_FRONT, GL_SHININESS, 128);
      // glMateriali  (GL_BACK, GL_SHININESS, 128);

      // Spot effects; tut fast nix
      // glLightfv (GL_LIGHT0, GL_SPOT_DIRECTION, GL_light0_spo);
      // glLightf  (GL_LIGHT0, GL_SPOT_CUTOFF, 45.0f);
      // glLightf  (GL_LIGHT0, GL_SPOT_EXPONENT, 100.0f);

  return 0;
}
*/


//================================================================
  int DL_Set_Cen2D () {
//================================================================
// move into 2D-screenCenter-position
// usage:
// glCallList (DL_Ind_Cen2D);   // move into 2D-screenCenter-position

// see also GL_Translate1 GL_pos_move_2D


  // printf("DL_Set_Cen2D %lf %lf %lf\n",GL_cen.x, GL_cen.y, GL_cen.z);


  glNewList (DL_Ind_Cen2D, GL_COMPILE);
    glTranslated (GL_cen.x, GL_cen.y, GL_cen.z);
  glEndList ();

  return 0;

}


//================================================================
  int DL_Set_Scl2D () {
//================================================================
// rotate into 2D-horizontal position
// usage:
// glCallList (DL_Ind_Scale2D);   // Scale2D
// ..
// glPopMatrix ();  // vom ScaleBack
  
// this routine must be called at everey change of the view

// see also GL_pos_move_2D
  
  double  ay, az, scl;


  UT3D_2angr_vc (&az, &ay, &GL_eyeY);
  az = UT_DEGREES(az); //  ???
  // ay = UT_DEGREES(ay);

  // printf("DL_Set_Scl2D %lf %lf \n",az,ay);

  
  // die Ruecktransformation in DL_Ind_ScBack speichern
  glNewList (DL_Ind_Scl2D, GL_COMPILE);
    glRotated (az, 0.0, 0.0, 1.0);  // um Z drehen
    // glRotated (ay, 0.0, -1.0, 0.0);  // um Y drehen
    // glScaled (sc, sc, sc);
  glEndList (); 
  
  
  return 0;

}


//================================================================
  int GL_ScalBack (double sc) {
//================================================================
// apply scale (now always same size; not true length)
// usage:
// glCallList (DL_Ind_ScBack);   // ScaleBack
// ..
// glPopMatrix ();  // vom ScaleBack

// this routine is called at every change of the view-scale !



  // die Ruecktransformation in DL_Ind_ScBack speichern
  glNewList (DL_Ind_ScBack, GL_COMPILE);
    // glPushMatrix ();
    glScaled (sc, sc, sc);
  glEndList ();


  return 0;

}
		
 
//=====================================================================
  int GL_Feedback (GLint *size, GLfloat *feedBuffer, GLint idim) {
//=====================================================================
/// \code
/// Input:
///   idim         GL_2D | GL_3D
/// Output:
///   size         nr of floats in feedBuffer
///   feedBuffer   
/// \endcode


  printf("GL_Feedback %d\n",*size);
  // GLdouble px, py;
  // GLint  i1;


/*
  // prepare for display of surface-borders
  glEnable(GL_CULL_FACE);
  glEnable(GL_NORMALIZE);    // scale normalVectors to  unit  length
  glEnable(GL_AUTO_NORMAL);       // bei Linux u Windows nutzlos
  glPolygonMode (GL_BACK, GL_LINE); // FRONT=Flaeche; BACK=Line.
*/

  // glFeedbackBuffer (*size, GL_3D_COLOR, feedBuffer); // 7 float je vertex

  UI_GR_DrawInit ();

  // die Ruecktransformation in DL_Ind_ScBack speichern
  // (in subModels kann man Planes usw nicht mehr abfangen)
  GL_ScalBack (1.);


  // init feedbackBuffer
  //   idim     GL_2D | GL_3D | GL_3D_COLOR
    printf(" glFeedbackBuffer 1\n");
  glFeedbackBuffer (*size, idim, feedBuffer);
    printf(" glFeedbackBuffer 2\n");

  // prepare GL for feedback
  glRenderMode (GL_FEEDBACK);

  // prepare Redraw
  GL_mode_draw_select = GR_MODE_FEEDBACK;


  // NUR ZUM TEST:
  // 4x4-Pixel Viewport rund um die SelectPosition einrichten.
  // px = GL_mouse_x_act;
  // py = GL_Viewp[3] - GL_mouse_y_act;
  // gluPickMatrix (px, py, GL_pickSiz, GL_pickSiz, GL_Viewp);
  // scheinbar nur fuer GL_SELECT


  // glOrtho
  // GL_Reframe1 (); // ganz wichtig !

  // draw all
    printf(" _Redraw 1\n");
  GL_Redraw ();
    printf(" _Redraw 2\n");

  // Abschliessen, Buffer fuellen. out of Mem: size is negative !
  *size = glRenderMode (GL_RENDER);
  // printf(" glRenderMode-size = %d\n",*size);

  // reset
  GL_mode_draw_select = GR_MODE_DRAW;


  // reset Ruecktransformation
  GL_ScalBack (GL_Scale_back);

  UI_GR_DrawExit ();

    printf("ex GL_Feedback %d\n",*size);

  return 0;

}


//================================================================
  void GL_Redra_mode (int mode) {
//================================================================

  GL_mode_draw_select = mode;

}


//================================================================
  int GL_backgrnd_1 (int mode) {
//================================================================
// permanent white background 0=ON, 1=OFF


  if(mode) GL_mode_draw_select = GR_MODE_DRAW;      // OFF   
  else     GL_mode_draw_select = GR_MODE_PRINT2;    // ON

  return 0;
}


//================================================================
  void GL_Print_Redraw () {
//================================================================
// print with white background
 
  printf("GL_Print_Redraw \n");




  GL_mode_draw_select = GR_MODE_PRINT1;

  UI_GR_DrawInit ();
  GL_Redraw ();
  UI_GR_DrawExit ();

}


//================================================================
  void GL_Redra__ (int mode) {
//================================================================
// 0    redraw complete DL; but without highlites surfaces !

  // printf("GL_Redra__ %d\n",mode);


  // redraw complete DL; but without highlites surfaces !
  GL_mode_draw_select = GR_MODE_NORMAL;

  UI_GR_DrawInit ();
  GL_Redraw ();
  UI_GR_DrawExit ();

  GL_mode_draw_select = GR_MODE_DRAW;

}


/*
//================================================================
  int GL_Redr_1 (long dli) {
//================================================================

  // UI_GR_DrawInit ();

  glCallList (dli);

  // UI_GR_DrawExit ();

  return 0;

}
*/

//================================================================
  void GL_Redraw () {
//================================================================
// muss man ev fuer calls von GL_Feedback und GL_Select noch was ausblenden ???
// must be embedded in UI_GR_DrawInit/UI_GR_DrawExit; use DL_Redraw

// nur allein temp.Objects neuzeichnen geht nicht ..

static int errOld = 123;

  int     irc, i1, i2, sx, sy, dx, dy, imode, trlNr, iXor=0, *ia, errAct;
  int     attInd, attAct;
  long    l1, gaNr;
  int     att_def[GR_ATT_TAB_SIZ];
  double  ay, az;
  float   fa[4];
  char    *ta, *p1, *p2;
  // float   *colb;
  Point   eye;
  ModelBas *mb;
  Mat_4x4 m1;


  // printf("--------------------------------------------\n");
  // printf("GL_Redraw %d %ld\n",DL_Ind_tmp,GR_TAB_IND);
  // printf("GL_Redraw mode=%d feed=%d\n",GL_mode_draw_select,GL_mode_feed);
  // printf("  AP_stat.sysStat=%d\n",AP_stat.sysStat);
  // printf("GL_Redraw GL_cen_Z=%f\n",GL_cen_Z);


  if(AP_stat.sysStat < 2) return;

/*
  // update DL (HideList) - nicht bei SELECT, FEEDBACK)
  if(GL_mode_draw_select == GR_MODE_DRAW) {
    GA_hide__ (1, 0, GL_modified);
    GL_modified = 0; 
  }
*/
  // if(GL_mode_draw_select != GR_MODE_SELECT) {


  // glShadeModel(GL_FLAT);
  // glEnable(GL_LIGHT0);
  // glEnable(GL_LIGHTING);

  // glEnable(GL_NORMALIZE);
  // glEnable(GL_AUTO_NORMAL);       // bei Linux u Windows nutzlos

  // if(GL_test_error ()) return;

  // if(ED_query_mode () == 1) return; // ? removed 2013-10-22 for modify-value.


  //----------------------------------------------------------------
  // fix background-colour
  if(GL_mode_draw_select != GR_MODE_DRAW) {
    if(GL_mode_draw_select == GR_MODE_PRINT2) {
      goto L_backgrnd_w;

    } else if(GL_mode_draw_select == GR_MODE_PRINT1) {
      GL_mode_draw_select = GR_MODE_DRAW;   // reset

      L_backgrnd_w:
        printf(" _Redr white\n");
        glClearColor (255.f, 255.f, 255.f, 1.f);
        errOld = 4; // white bg from PRINT active
        goto L_clear;
    }
  }

/*
  if(GL_mode_draw_select == GR_MODE_PRINT2) {
    GL_mode_draw_select = GR_MODE_DRAW;
    errOld = 99;   // reset bgCol
  }
*/

  errAct = AP_errStat_get();  // get AP_stat.errStat; 0=OK; 1=Err; 2=rerun
  // if((errAct == 0)&&(IE_modify == 1)) errAct = -1;  // ??
    // printf(" redraw %d %d\n",errAct,errOld);


  if(errAct != errOld) {
      // printf(" _Redr col errAct=%d\n",errAct);
    if(errAct == 0) {
        // 0=OK..   gray
        glClearColor (GL_col_bg[0], GL_col_bg[1], GL_col_bg[2], GL_col_bg[3]);

    // } else if (errAct < 0) {
        // glClearColor (GL_col_bm[0], GL_col_bm[1], GL_col_bm[2], GL_col_bm[3]);

    } else {
      // -1=Error   red
      glClearColor (GL_col_be[0], GL_col_be[1], GL_col_be[2], GL_col_be[3]);
    }
    errOld = errAct;
  }



  L_clear:
  glDisable (GL_LINE_STIPPLE);              // Disable Strich-Punkt
  // clear Fensterhintergrund
  // if(GL_mode_wire_shade < GR_STATE_HID2)
      // printf(" _Redr clear\n");  // dauert tw sehr lang ..
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



  // nicht f SELECT (PROJ-Mat. schon fertig geladen)
  // ist fuer FEEDBACK aber auch erforderlich !
  if(GL_mode_draw_select != GR_MODE_SELECT) {
  // if(GL_mode_draw_select == GR_MODE_DRAW) {

    // fix PROJ-Mat.
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();  // init PROJ-Mat.

    // Parallelprojektion
      // printf(" _Redr refram1\n");
    GL_Reframe1 ();     // PROJ-Mat. laden.   glOrtho

    // oder perspektivisch - TEST
    // GL_Reframe2 (); 

  }


  // reset Viewmatrix
    // printf(" _Redr view\n");
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // apply scale (bei Parallelprojektion erforderl!)
  glScaled (GL_Scale, GL_Scale, GL_Scale);



  // eye-Punkt errechnen; GL_eyeX zeigt von GL_cen -> eye
  // GL_eyeX == umgekehrte Sichtlinie !!
  UT3D_pt_traptvc (&eye, &GL_cen, &GL_eyeX);


  // Eye-Position und Eye-Vektor setzen
  gluLookAt (eye.x,      eye.y,      eye.z,
             GL_cen.x,   GL_cen.y,   GL_cen.z,
             GL_eyeZ.dx, GL_eyeZ.dy, GL_eyeZ.dz);



  // fa[0] = eye.x;
  // fa[1] = eye.y;
  // fa[2] = eye.z;
  // fa[3] = 0.f;   // 0=posi, 1=vector.
  // glLightfv(GL_LIGHT0, GL_POSITION, fa);

  // die Clipping-Z-Planes setzen; wenn Z-Werte zu gross, dann sind die
  // Raender der Flaeche total haesslich ..
  // sollte erste nach glLoadIdentity ... erfolgen !!
  // Hier aber gehts gar nicht !?
  // GL_Siz_Z = 1000;
  // GL_Reframe1();


  // alle spaeter fuer Proj erforderlichen Matrizen lesen
  // nicht im SEL-mode; proj-mat falsch !!
  if(GL_mode_draw_select == GR_MODE_DRAW) {
    GL_GetGLMat0 ();
  }

  // imode = UI_ask_mode();
  imode = UI_InpMode;
    // printf(" imode=%d\n",imode);




  //=========================================================================
  // ab hier Objekte generieren .....

/*
    // right-upper-corner: absolute Refsys.
    glPushMatrix ();
    glTranslated (100., 100., 0.);          // das RotCen
    glDisable (GL_LIGHTING);
    glColor3fv   (GL_col_tab[13]);   
    glLineWidth   (1.0);                       // 1 ist am duennsten !
    glDepthFunc (GL_NOTEQUAL); // geht so leider ned ..
    glCallList ((GLuint)SYM_AXIS1);
    glDepthFunc (GL_LEQUAL);      // reset = Standard
    glEnable (GL_LIGHTING);
    glPopMatrix ();
*/


  // zuerst ein Achsenkreuz in den Drehmittelpunkt (nur wenn Rotate aktiv ist)
  // wird fuers ViewZ nun immer gebraucht !!
  // Nicht: GR_MODE_FEEDBACK
  if(GL_mode_draw_select == GR_MODE_DRAW) {
      // printf(" draw Achsenkreuz ..\n");


    // das absolute Achsenkreuz setzen. Immer genau in Bildschirmmitte.
    glPushMatrix ();
    glTranslated (GL_cen.x, GL_cen.y, GL_cen.z);          // das RotCen
    glDisable (GL_LIGHTING);
    glColor3fv   (GL_col_tab[8]);             // was 12 grauweiss
    glLineWidth   (1.0);                       // 1 ist am duennsten !
    glDepthFunc (GL_NOTEQUAL); // geht so leider ned ..
    // glCallList ((GLuint)SYM_AXIS1); // ohne x-y-z
    glCallList ((GLuint)SYM_AXIS);     // mit x-y-z-Characters
    glDepthFunc (GL_LEQUAL);      // reset = Standard
    glEnable (GL_LIGHTING);
    glPopMatrix ();


    GL_Func_Act = FUNC_Idle;


    // Viewer: skip Achsenkreuz und Planes
    if(imode == UI_MODE_VWR)  goto L_L1;

    GL_grid__ (&GL_constr_pln, WC_sur_mat, WC_sur_imat);

    // im aktuellen Origin ein Achsenkreuz (mit X-Y-Z-characters)
    // glCallList (DL_base_LnAtt + Typ_Att_Symb);
    // glCallList ((GLuint)SYM_AXIS);

/*
    // die aktive ViewPlane darstellen
    UT3D_2angr_vc (&az, &ay, &GL_view_pln.vz);
    // az = UT_DEGREES(az);
    // ay = UT_DEGREES(ay) - 90.;
    glPushMatrix ();
    glTranslated (GL_view_pln.po.x, GL_view_pln.po.y, GL_view_pln.po.z);
    // glRotated (az, 0.0, 0.0, 1.0);   // um Z drehen
    // glRotated (ay, 0.0, -1.0, 0.0);  // um Y drehen
    glColor3f (1.f, 1.f, 0.f);       // gelb
    glCallList ((GLuint)SYM_PLANE);
    glPopMatrix ();
*/


    // die aktive ConstrPlane darstellen
    // UT3D_2angr_vc (&az, &ay, &GL_constr_pln.vz);
    // az = UT_DEGREES(az);
    // ay = UT_DEGREES(ay) - 90.;
    glPushMatrix ();
    glDisable (GL_LIGHTING);
    // glTranslated (GL_constr_pln.po.x, GL_constr_pln.po.y, GL_constr_pln.po.z);
    // glRotated (az, 0.0, 0.0, 1.0);   // um Z drehen
    // glRotated (ay, 0.0, -1.0, 0.0);  // um Y drehen

    UT3D_m4_loadpl (m1, &GL_constr_pln);
      // UT3D_stru_dump (Typ_M4x4, m1, "new m1:");
    glMultMatrixd ((double*)m1);

    // glColor3f (0.f, 1.f, 0.f);       // gruen
    glColor3f (1.f, 0.f, 0.f);       // rot
    // glCallList ((GLuint)SYM_PLANE);
    // glScaled (1.0, 1.0, 1.0);
    glCallList ((GLuint)SYM_AXIS);
    glEnable (GL_LIGHTING);
    glPopMatrix ();

  }


  L_L1:
  // nur bei select:
  if(GL_mode_draw_select == GR_MODE_SELECT) {

    glInitNames();               // Init NameStack.
    glPushName((GLuint)0);       // Muss sein. Damit ein Name am Stack ist,
                                 // der im folgenden mit glLoadName
                                 // verändert wird.


  } else {                       // DRAW & FEED & PRINT.

    glColor3fv   (GL_col_tab[0]);     
    // glColor3f (0.f, 1.f, 0.f);       // gelb
    // glLineWidth  (0.0f);
  }



  // Default; sonst kein Glanz wenn Flaeche als erstes;
  // printf(" vor mainRedr:%d\n",GL_mode_wire_shade);
  if(GL_mode_draw_select == GR_MODE_DRAW) {
    glEnable  (GL_LIGHTING);       // Licht EIN

  } else {
    glDisable (GL_LIGHTING);       // SEL & FEED 2009-07-16
  }

/*
  // if enabled: draw Vector Selector.
  if((GL_vcSelStat) || (GL_plnSelStat))
    GL_icons_dispVcPln ();
  // glCallList ((GLuint)297);
*/

  // HIDDEN: die Farbe durch Hintergrundfarbe ersetzen.
  // if(GL_mode_wire_shade == GR_STATE_HID1)
     // GL_InitNewAtt (3, 5);


  if((long)GR_TAB_IND <= 0) goto L_temp;

  attAct = -1;

  // printf(" DL_base_font1=%d DL_base__=%d\n",DL_base_font1,DL_base__);
  // printf(" GL_mode_draw_select=%d GL_mode_feed=%d\n",GL_mode_draw_select,
             // GL_mode_feed);
  // printf(" _Redr main\n",GR_TAB_IND);


  // glColor3ubv ((unsigned char*)&GL_defCol);




  //============== DRAW ============================================
  // Die Hauptschleife.
  //   1) hilited obj's
  //   2) transparent obj's
  // Man kann Hili nicht in der 1. Schleife machen,
  //   weil nachfolgende Objekte das Original-Attr. brauchen !
    // printf(" _Redr DRAW %d\n",GL_mode_draw_select);

  trlNr = 0;
  for(l1=0; l1<GR_TAB_IND; ++l1) {

    // printf(" _Redraw%ld typ=%d disp%d hili%d\n",l1,GR_ObjTab[l1].typ,
             // GR_ObjTab[l1].disp, GR_ObjTab[l1].hili);
    // if(l1 == 16) continue; // TEST ONLY

    // skip all objs outside active model                       2016-06-12
    if((INT_16)GR_ObjTab[l1].modInd != -1) continue;

    // skip unvisible (subModel-objects !)
    if(GR_ObjTab[l1].unvis != 0) continue;

    // skip hidden objs
    if(DL_OBJ_IS_HIDDEN(GR_ObjTab[l1])) continue;        // skip hidden obj's
    // if((GR_ObjTab[l1].disp == 1) && (GR_ObjTab[l1].hili == 1)) continue;

    if(GL_mode_draw_select == GR_MODE_DRAW) {        //2010-06-17
      // GL_Mouse1Pos needs this after GL_Select


      // skip nodisp- (und hili,dim)-obj; disp spaeter.
      // ausser beim Rescale !!
      // if((GR_ObjTab[l1].disp != 0)&&(GL_mode_feed != 1)) continue;


      // 2006-01-15 - skip objects in group (disp spaeter)
      if(GR_ObjTab[l1].grp_1 == 0) continue;


      // skip transparent objects
      if(GR_ObjTab[l1].typ == Typ_SUR) {    // report geaenderten Lintyp ..
        if(((ColRGB*)&GR_ObjTab[l1].iatt)->vtra != 0) {trlNr = 1; continue;}
      }
/*
    } else if(GL_mode_draw_select == GR_MODE_NORMAL) {
      // for get SelObjPos surfaces may not be hilited
      // skip hidden objs
      if((GR_ObjTab[l1].disp == 1) && (GR_ObjTab[l1].hili == 1)) continue;
*/
    }

    // skip Texture (nur subCalls)
// TODO: make Typ_TEXB unvis ?
    if(GR_ObjTab[l1].typ == Typ_TEXB) continue;


    if(GL_mode_draw_select == GR_MODE_SELECT) {
      // den (nur fuer die Selektion erforderlichen) Namen zuweisen
      glLoadName((GLuint)l1);
        // printf(" glLoadName %d\n",l1);



    } else if(GL_mode_draw_select == GR_MODE_FEEDBACK) {

      // Linetyp an plottertreiber
      if(GR_ObjTab[l1].typ != Typ_SUR) {    // report geaenderten Lintyp ..
        // if(GR_ObjTab[l1].typ == Typ_APPOBJ) break; // skip Applic.object
        if(GR_ObjTab[l1].typ == Typ_APPOBJ) goto L_main_9; // f. rescale !
        if(GR_ObjTab[l1].iatt != attAct) {  // report geaenderten Lintyp ..
          attAct = GR_ObjTab[l1].iatt;
          glPassThrough (attAct);    // via float ...
        }
      }

      // Textnote = symbol; wird im FeedbackBuffer als je eine
      // GL_BITMAP_TOKEN pro char ausgegeben
      if(GR_ObjTab[l1].typ == Typ_ATXT) {         // report Textnote ..
        // printf(" typ=%d ind=%d\n",GR_ObjTab[l1].typ,GR_ObjTab[l1].ind);
        glPassThrough (-GR_ObjTab[l1].ind);       // via float ...


      } else if(GR_ObjTab[l1].typ == Typ_PLN) {  // ReScale: skip scalables
        if(GL_mode_feed == 1) continue;
      }

    }


    // OK; redraw this obj ....
    // printf(" redr%d typ=%d ind%d\n",l1,GR_ObjTab[l1].typ,GR_ObjTab[l1].ind);



/*
    // Dittos: DefCol setzen.
    if(GR_ObjTab[l1].typ == Typ_Model) {
      mb = DB_get_ModBas (GR_ObjTab[l1].attInd);
      GL_DefColSet (&mb->defCol);
    }
*/


/*    dzt unused
    if(GL_mode_wire_shade >= GR_STATE_HID1) {

      // Outline
      glEnable(GL_CULL_FACE);
      glCullFace (GL_FRONT_AND_BACK);
      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
      glCallList ((GLuint)l1 + DL_base__);                      // execute

      // als Flaeche drueber
      glStencilFunc(GL_EQUAL, 0, 1);
      glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

      // als Flaeche in Hintergrundfarbe ..
      GL_InitNewAtt (3, 5);
      glDisable(GL_CULL_FACE);
      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
      glCallList ((GLuint)l1 + DL_base__);
      GL_InitNewAtt (3, 0);

      // nochmal Outline drueber ..
      glStencilFunc(GL_ALWAYS, 0, 1);
      glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);

      glEnable(GL_CULL_FACE);
      glCullFace (GL_FRONT_AND_BACK);
      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
      glCallList ((GLuint)l1 + DL_base__);

    } else {
*/

      L_main_9:
      // very slow on MS-Win if RenderMode = GL_SELECT !
      glCallList ((GLuint)l1 + DL_base__);                      // execute
        // printf(" reDraw %ld %d\n",l1,DL_base__); 
    // }


  }





  //================= TRANSPARENT =======================================
    // printf(" _Redr transp\n");
  if(GL_mode_draw_select == GR_MODE_NORMAL) goto L_fertig;  //2010-06-17
  // Zuerst die undurchsichtigen Objekte ausgeben,
  //   dann erst die transparenten, sonst gibts Mist !!!!!
    // printf(" _Redr Transp\n");
  if(trlNr < 1) goto L_hili_0;

  // see GL_Surf_Ini
  glEnable (GL_COLOR_MATERIAL);
  glEnable (GL_LIGHTING);
  glEnable (GL_BLEND);
  // glDisable (GL_DEPTH_TEST); // nur damit immer (in SM) durchsichtig !
  // glBlendFunc (GL_SRC_ALPHA, GL_ONE);   // geht



  for(l1=0; l1<GR_TAB_IND; ++l1) {

    if(GR_ObjTab[l1].typ != Typ_SUR) continue;
    if(((ColRGB*)&GR_ObjTab[l1].iatt)->vtra == 0) continue;
      // printf(" transp: %d\n",l1);

    if(GL_mode_draw_select == GR_MODE_SELECT)
      glLoadName((GLuint)l1);

    // OK; make this obj transp. ....
    // printf(" transp%d typ=%d ind%d\n",l1,GR_ObjTab[l1].typ,GR_ObjTab[l1].ind);

    glCallList ((GLuint)l1 + DL_base__);                      // execute
  }

  glDisable (GL_BLEND);




  //================= HILITE =======================================
  // if(GL_mode_wire_shade >= GR_STATE_HID1) return;
  // if(GL_mode_draw_select == GR_MODE_FEEDBACK) return;
  // Draw hilited auch im Feedbackmode (sonst geht GL_SelVert__ nicht mehr !)
  L_hili_0:
    // printf(" _Redr L_hili_0\n");

  // alle folgenden overwrite ..
  // glEnable (GL_LINE_SMOOTH);  // macht Lines dick; TEST war aktiv 2006-01-03
  // glDepthFunc (GL_NOTEQUAL); // ueberschreibt immer (auch Flächen !)
  // glDepthFunc (GL_GREATER);  // sehr komisch ..



  // init eine tabelle, wo gespeichert wird, ob ein Attribut veraendert wurde
  // (und somit zurueckgesetzt werden muss.
  for(i1=0; i1<GR_ATT_TAB_SIZ; ++i1) {
    att_def[i1] = OFF;
  }


  // alle zu hilitenden Objekte ausgeben (jedes Obj hat in der DL attInd
  //   gespeichert; das Original ueberschrieben mit hili-Col Width(4); dann
  //   draw obj, dann restore attrib.
  // alle folgenden obj ganz nach oben ..
  // glDepthFunc (GL_NOTEQUAL);        // TEST OFF; war aktiv 2006-01-03
  // Flächen nur outlines!
  // glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

  // extrahilite
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, GL_mat_hili);

  // PointAttributes --> hilite
  GL_InitPtAtt (-2);

  // damit ueberdeckt sie auch Flächen !
  // glDisable (GL_DEPTH_TEST);
  glDepthFunc (GL_ALWAYS);          // alles ueberschreiben
  // glDepthFunc (GL_GREATER);          // geht ned ..
  // glDepthFunc (GL_NOTEQUAL);          // wie GL_ALWAYS
  // glDepthRange (0., 0.);              // unnutz


  // Texturen kann man nur so hiliten ...
  // (bei Texturen wirkt kein disable LIGHT od make ExtraLights)
  // glBlendFunc (GL_ONE, GL_ONE_MINUS_CONSTANT_ALPHA);
  // glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_COLOR);
  // glEnable (GL_BLEND);


  // Lines & Curves koennte man mit XOR hiliten; bei surfaces geht das nicht.
  // - hilite fuer surfaces & bodies & subModels in extra loop !
  // oder Liste aller DL-rec#s schreiben und dann abarbeiten ..
  // glLogicOp(GL_XOR);
  // glEnable(GL_COLOR_LOGIC_OP);


  // fuer subModels mit lines jetzt schon
  GL_InitNewAtt (1, 0);
  att_def[0] = ON;


  for(l1=0; l1<GR_TAB_IND; ++l1) {

    // skip all objs outside active model                       2016-06-12
    if((INT_16)GR_ObjTab[l1].modInd != -1) continue;

    // mode select: skip all hilited objects ..  // 2010-10-24
    if(GL_mode_draw_select == GR_MODE_SELECT) {
      if(GR_ObjTab[l1].grp_1 == 0) continue;
      // skip hilited obj
      if((GR_ObjTab[l1].disp == 1)&&(GR_ObjTab[l1].hili == 0)) continue;
    }


    // hilite all group-objs
    if(GR_ObjTab[l1].grp_1 == 0) goto L_hili_1;       // ON=0, OFF=1

    // skip objs not hilited 
    if(GR_ObjTab[l1].disp  != 1) continue;
    if(GR_ObjTab[l1].hili  != 0)  continue;

    L_hili_1:
    if(GR_ObjTab[l1].unvis != 0)   continue; // skip unvisible


    // OK; hilite this obj ....
    // printf("hili %ld typ=%d ind=%ld\n",l1,GR_ObjTab[l1].typ,GR_ObjTab[l1].ind);


    if(GL_mode_draw_select == GR_MODE_SELECT) {
      // den (nur fuer die Selektion erforderlichen) Namen zuweisen
      // mit glLoadName kommt keine brauchbare ID!
      //glPopName();
      //glPushName ((GLuint)l1);
      glLoadName((GLuint)l1);
      //TX_Print(" load nam %d",l1);
    }



    if(GR_ObjTab[l1].typ == Typ_PT) goto L_hili__;
       // (GR_ObjTab[l1].typ == Typ_SymB)  ||
       // (GR_ObjTab[l1].typ == Typ_SymV))     goto L_hili__;


    // Images, Tags: eine Border hinmalen ...
    if(GR_ObjTab[l1].typ == Typ_Tag) {

      // draw; wurde in erster Loop nicht gezeichnet.
      glCallList ((GLuint)l1 + DL_base__);
        // printf(" _hili_tag %ld\n",l1);

      
      // prepare for GL_Disp_2D_box1
      // glNewList (298, GL_COMPILE);   // Open DispList
      glDisable (GL_LIGHTING);
      glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
      glColor3f (1.f, 0.f, 0.f);        // rot
      // glDepthFunc (GL_ALWAYS);          // alles ueberschreiben
    
      // irc = DL_txtgetInfo (&i1, &eye, &sx, &sy, &dx, &dy, l1);
      irc = GR_img_get_dbi (&i1, &eye, &sx, &sy, &dx, &dy, GR_ObjTab[l1].ind);
      if(irc < 0) continue; // SymbolTag
      GL_Disp_2D_box1 (&eye, sx, sy, dx, dy);

      glEnable (GL_LIGHTING);
      // glEndList ();                        // Close DispList
      continue;
    }


    // Flaechen: extraLight + ganz oben zeichnen;
    if((GR_ObjTab[l1].typ == Typ_SUR)    ||
       (GR_ObjTab[l1].typ == Typ_SOL))        {

      // check if symbolic
      if((APT_dispSOL == OFF) ||
         ((ColRGB*)&(GR_ObjTab[l1].iatt))->vsym == 1) {
        // yes, symbolic - uses att Typ_Att_Fac1
          // printf(" disp symbolic ..\n");
        attInd = Typ_Att_Fac1;
        goto L_hili_curv;
      }

      // // check if textured
      // if(((ColRGB*)&(GR_ObjTab[l1].iatt))->vtex == 1) {
        // printf(" hili tex\n");
      // }

      glClear (GL_DEPTH_BUFFER_BIT);  // damit wirds ganz oben gezeichnet ..
      // glEnable (GL_LINE_SMOOTH);  // macht Lines dick
      glLineWidth   (4.0);
      glEnable (GL_BLEND);                                      // 2011-04-12
      glCallList ((GLuint)l1 + DL_base__);                      // execute
      glDisable (GL_BLEND);
      continue;
    }


    // subModels mit Lines only: thick 4.
    // if((GR_ObjTab[l1].typ == Typ_Model) ||
       // (GR_ObjTab[l1].typ == Typ_Mock))     {
    if(GR_ObjTab[l1].typ == Typ_Model) {
      // glColor3fv   (GL_col_tab[GR_AttTab[Typ_Att_hili1].col]); geht ned ..
      // glLineWidth   (4.0);     // fuer Lines im Ditto .. geht ned ..
      glClear (GL_DEPTH_BUFFER_BIT);  // damit wirds ganz oben gezeichnet ..
      // glDepthFunc (GL_NOTEQUAL);
      glEnable (GL_LINE_SMOOTH);  // macht Lines dicker; uebergeht glLineWidth
      attInd = Typ_Att_Fac1;   // fuer Surfs im Ditto ..
      goto L_hili_curv;
    }


    if(GR_ObjTab[l1].typ == Typ_APPOBJ) goto L_hili__;
/*
    // appObjs have a modified iatt!
    // if(GR_ObjTab[l1].typ == Typ_APPOBJ) {
      glLogicOp(GL_XOR);
      glEnable(GL_COLOR_LOGIC_OP);
      iXor = 1;
      goto L_hili__;
      // attInd = 0;
      // goto L_hili_curv;
    // }
*/

    // nur fuer Lines - nicht fuer Surf's
    // wurde Attribut schon umdefiniert -
    // // gaNr = GR_ObjTab[l1].attInd;
    // // attInd = GA_ObjTab[gaNr].gu.iatt;
    attInd = GR_ObjTab[l1].iatt;
    // attInd = ((Ind_Att_ln*)&GR_ObjTab[l1].iatt)->indAtt; // IndAttLn_get_ltyp
      // printf(" attInd=%d\n",attInd);
    if(attInd < 0) goto L_hili__;
    if(attInd >= GR_ATT_TAB_SIZ) goto L_hili__;



    L_hili_curv:
      // printf(" L_hili_curv: attInd=%d l1=%ld att_def=%d\n",
              // attInd,l1,att_def[attInd]);
    if(att_def[attInd] != ON) {
        // printf(" ... hili change %ld > hili\n",attInd);
      att_def[attInd] = ON;
      GL_InitNewAtt (1, attInd);
      // glEnable(GL_CULL_FACE);
      // glDepthFunc (GL_NOTEQUAL);
    }

    L_hili__:
      // printf(" _Redraw L_hili__: %ld\n",l1);
      // printf(" gl-2 %ld",l1);
    glCallList ((GLuint)l1 + DL_base__);           // execute (draw)

    // if(iXor) { glDisable(GL_COLOR_LOGIC_OP); iXor = 0; }
  }


  // zuruecksetzen veraenderter attribute.
  for(l1=0; l1<GR_ATT_TAB_SIZ; ++l1) {
    if(att_def[l1] == ON) {
      // printf(" reset att %d\n",l1);
      att_def[l1] = OFF;
      GL_InitNewAtt (0, l1);
    }
  }

  // glDisable(GL_COLOR_LOGIC_OP);

  glDisable (GL_LINE_SMOOTH);
  // glEnable (GL_DEPTH_TEST);


  // glDisable (GL_BLEND);

  // reset ExtroHiliteLight
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, GL_mod_amb2);

  // PointAttributes Reset
  GL_InitPtAtt (-1);

  // glLineWidth   (0.0);


/*
  //--------------------------------------
  // alle zu dimmenden Objekte ausgeben
  for(l1=0; l1<GR_TAB_IND; ++l1) {
    if(GR_ObjTab[l1].disp  != OFF) continue;
    if(GR_ObjTab[l1].dim   != ON)  continue;
    if(GR_ObjTab[l1].unvis != 0)   continue; // skip unvisible
    //TX_Print("hili %d %d",i1,GL_Hili_Tab[i1]);
    //glCallList (GL_Hili_Tab[i1] + DL_base__ + 1);


    // wurde Attribut schon umdefiniert -
    attInd = GR_ObjTab[l1].iatt;
    if(att_def[attInd] != ON) {
      //TX_Print(" change %d > hili",attInd);
      att_def[attInd] = ON;
      GL_InitNewAtt (2, attInd);
    }


    if(GL_mode_draw_select == GR_MODE_SELECT) {
      // den (nur fuer die Selektion erforderlichen) Namen zuweisen
      // mit glLoadName kommt keine brauchbare ID!
      //glPopName();
      //glPushName ((GLuint)l1);
      glLoadName((GLuint)l1);
      //TX_Print(" load nam %d",l1);
    }

    glCallList ((GLuint)l1 + DL_base__);                      // execute
  }


  // zuruecksetzen veraenderter attribute.
  for(l1=0; l1<GR_ATT_TAB_SIZ; ++l1) {
    if(att_def[l1] == ON) {
      //TX_Print(" reset %d ",l1);
      GL_InitNewAtt (0, l1);
    }
  }
*/


  //======= TEMP OBJ'S ===================================
  L_temp:
    // printf(" _Redraw L_temp\n");
  if(GL_mode_draw_select == GR_MODE_SELECT) goto L_fertig;
  // die temp.Obj's malen (nicht im SelectMode -> unselektierbar !)
  // zuletzt machen, damit sie vorherige ueberdecken.
  // printf(" DL_Ind_tmp=%d\n",DL_Ind_tmp);
  // Schleife ueber die temp. Objects.
  for(l1=1; l1<DL_Ind_tmp; ++l1) {
      // printf(" _Redraw call temp.o %ld\n",l1);
    glCallList ((GLuint)l1);                      // execute
  }

  // endgl und swapbuffer
  // UI_GR_DrawExit ();  // swap_buffers



  //----------------------------------------------------------------
  L_fertig:
    // printf(" L_fertig:\n");
  // only for CAD
  if(UI_InpMode != UI_MODE_CAD) goto L_exit;
  if((GL_mode_draw_select != GR_MODE_DRAW)    && 
     (GL_mode_draw_select != GR_MODE_SELECT))     goto L_exit;
  // if enabled: draw Vector Selector.
  if((GL_vcSelStat) || (GL_plnSelStat))
    GL_icons_dispVcPln ();

  // 2D-buttons
  sele_get_icons (&i1, &ia, &ta, &p2);
  if(i1 > 0) {
    p1 = ta;
    for(i2=0; i2<i1; ++i2) {
      GL_icons_dispTags (i2, p1, p2[i2]);
      p1 += 32;  // next string
    }
  }


  // reset overwrite ..
  glDepthFunc (GL_LEQUAL);    // reset Default
  // glDepthRange (0., 1.);
  // if(GL_mode_wire_shade == GR_STATE_SHADE)
    // glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

  glDisable (GL_LINE_SMOOTH);  // macht Lines dick ..



  //----------------------------------------------------------------
  L_exit:
    // printf("ex GL_Redraw\n");
  return;

}


/*
//================================================================
  int GL_tmp_Redraw () {
//================================================================
GEHT NED ..
  long l1;
 
  // GL_temp_delete ();

  for(l1=1; l1<DL_Ind_tmp; ++l1) {
    // printf(" call temp.o %ld\n",l1);
    glCallList ((GLuint)l1);                      // execute
  }

  return 0;

}
*/

//=====================================================================
  void GL_GetGLMat0 (void) {
//=====================================================================
// alle spaeter fuer Proj erforderlichen Matrizen lesen

  // printf("------------>>>>>>>>> GL_GetGLMat0 .....\n");


  glGetIntegerv (GL_VIEWPORT, GL_Viewp);            // get Viewport-Matrix */

  glGetDoublev (GL_PROJECTION_MATRIX, GL_MatProj);  // get Proj.-Matrix

  glGetDoublev (GL_MODELVIEW_MATRIX, GL_MatMod);    // get Model-Matrix

  // GL_dump_Mat0 ();
}


//================================================================
  int GL_dump_Mat0 () {
//================================================================

  int   i1;

  // Viewport-Matrix */
  printf("...GL_VIEWPORT\n");
  printf(" %d %d %d %d\n",GL_Viewp[0],GL_Viewp[1],GL_Viewp[2],GL_Viewp[3]);

  // Proj.-Matrix
  printf("...GL_PROJECTION_MATRIX\n");
  for(i1=0;i1<16;++i1) printf("[%d]%f ",i1,GL_MatProj[i1]);


  // Model-Matrix
  printf("\n...GL_MODELVIEW_MATRIX\n");
  for(i1=0;i1<16;++i1) printf("[%d]%f ",i1,GL_MatMod[i1]);

  printf("\n");

  return 0;

}


/*
//=====================================================================
  void GL_GetNormal0 () {
//=====================================================================
// Die Normalebene auf die Displayplane errechnen.
// (genau normal zum Beobachter - genau in Blickrichtung) 

  double     ux1,uy1,uz1,ux2,uy2,uz2;



  GL_Sk2Uk (&ux1, &uy1, &uz1,  0.,0.,0.);
  GL_Sk2Uk (&ux2, &uy2, &uz2,  0.,0.,-1.);

  GL_view_vz.dx = ux2 - ux1;
  GL_view_vz.dy = uy2 - uy1;
  GL_view_vz.dz = uz2 - uz1;

  UT3D_vc_setLength (&GL_view_vz, &GL_view_vz, 1.);

  // printf("ex GL_GetNormal0 %f,%f,%f\n",GL_view_vz.dx,GL_view_vz.dy,GL_view_vz.dz);

}
*/


//================================================================
  int GL_pt2_pt3 (Point *pt2, Point *pt3) {
//================================================================
/// GL_pt2_pt3             Userkoords > Screenkoords.

  gluProject (pt3->x, pt3->y, pt3->z,
              GL_MatMod, GL_MatProj, GL_Viewp,
              &pt2->x, &pt2->y, &pt2->z);

  return 0;

}

 
/*=====================================================================*/
  void GL_Uk2Sk (double *sx, double *sy, double *sz,
                 double  ux, double  uy, double  uz) {
/*===============
Userkoords > Screenkoords.
*/


  // printf("GL_Uk2Sk %f %f %f\n",ux, uy, uz);

  // GL_GetGLMat0 (); // hier nur test

  gluProject (ux, uy, uz, GL_MatMod, GL_MatProj, GL_Viewp, sx, sy, sz);

  // printf(" ex GL_Uk2Sk  %f,%f,%f\n", *sx, *sy, *sz);


}


/*=====================================================================*/
  void GL_Sk2Uk (double *ux, double *uy, double *uz,
                 double  sx, double  sy, double  sz) {
/*===============
Screenkoords > Userkoords.
*/


  // printf("GL_Sk2Uk %f %f %f\n",sx, sy, sz);

  // GL_GetGLMat0 (); // hier nur test

  gluUnProject (sx, sy, sz, GL_MatMod, GL_MatProj, GL_Viewp, ux, uy, uz);

  // printf(" ex GL_Sk2Uk %f,%f,%f\n", *ux, *uy, *uz);


}


//================================================================
  int GL_Query_siz (int *siz) {
//================================================================
// Estimate the size of all OpenGL-objects for FeedbackBufferSize.

  int      i1;
  long     l1, ind;
  char     mnam[256], fnam[256];
  ModelRef *mr;
  ModelBas *mb;

  *siz = 0;

  // printf("GL_Query_siz %ld\n",GR_TAB_IND);

  *siz += GR_TAB_IND * 10000;


  // add 1 K for each basic-model
  *siz += DB_get_ModBasNr() * 100000;


 
  for(l1=0; l1<GR_TAB_IND; ++l1) {


    // add size for Mockup-Models
    if(GR_ObjTab[l1].typ == Typ_Mock) {
      ind = GR_ObjTab[l1].ind;
        // printf("GL_Query_siz M%ld\n",ind);
      mr = DB_get_ModRef (ind);           // get ModRef
      mb = DB_get_ModBas (mr->modNr);     // get baseModel
        // printf(" typ=|%d|\n",mb->typ);
        // printf(" mnam=|%s|\n",mb->mnam);

      // get tess-filename; see GR_DrawModel()
      strcpy(mnam, mb->mnam);
      UTX_ftyp_cut (mnam);            // remove fTyp
      UTX_safeName (mnam, 1);         // change all '. ' to '_', not '/'
      i1 = tess_fnam (fnam, mnam);
        // printf(" %d fnam=|%s|\n",i1,fnam);
      if(i1 > 0) {
        // TessFileSize / 7 = size fuer FeedbackBuffer
        *siz = *siz + (OS_FilSiz (fnam) / 1);  // 7
      }
    }
  }


  // noch die DB-siz aufaddieren
  DB_Query_siz (&l1);
    // printf(" DB-siz=%ld\n",l1);

  l1 *= 10;
  *siz = *siz + l1;


  // printf("ex GL_Query_siz %d\n",*siz);

  return 0;

}


//=====================================================================
  void GL_Init0 () {
//=====================================================================
// Die DL-Basis neu anlegen.


  int i1;


  printf(" GL_Init0\n");


  // Tabelle fuer Dittos
  GL_alloc__ (0L);


  /* ------------------------------------- */
  // geht noch nicht ! TX_Print("GL_Init0");


  GL_Func_Act         = FUNC_Idle;


  // Zuerst die displist fuer fonts u. symbols holen
  // i1 = glGenLists (DL_base__ + 10);
  // glGenLists unnutz - wird scheinbar automat. alloziert !
  // if(glIsList(100) == GL_FALSE) glGenLists (DL_base__);
  //}


  /* nun fonts u. symbols dorthinein generieren */
  //GL_Init_AFont0 ();
  //GL_InitSymb0 ();
  /* GL_Init_CLIPBOX (); */


  /* nun displist fuer die DB-objekte holen */
  //DL_base__ = glGenLists (DL_siz);


  // primary init of DL (deletes Overlay-models)
  DL_base__ = DL_base_mod;


  GL_Init1();



  // die DefCol setzen ..
  GL_InitCol (10);

}



//=====================================================================
  void GL_Init1 () {
//=====================================================================
// Die DL zuruecksetzen


  // printf("GL_Init1\n");

  // reset selectBuffer
  if(GR_Siz_selTab > 11000) {
    GR_nr_selTab  = 0;
    GR_Siz_selTab = 0;
    GL_selTab_realloc ();
  }


  // DL loeschen
  GL_Del0 (Typ_ALL_OBJS);

  GR_TAB_IND  = 0;

  GL_modified = 1;

  GL_InitPtAtt (-1);   // Default

  // reset ucs
  UT3D_pl_XYZ (&GL_constr_pln);

  // damit in W32 nicht sooft Crash in GLU32 nach GLT_spp_
  GLT_exit ();
  GLT_init ();

}


/*
//================================================================
  int GL_Init2 () {
//================================================================
 
    GL_initMode = 0;

    GL_Init_View ();

  return 0;

}
*/


//***********************************************************************
  void GL_Init_View () {
//***********************************************************************
// alle viewParameter zuruecksetzen
// UI_GR_DrawInit/UI_GR_DrawExit muss aussen sein !


  // printf("GL_Init_View %d\n",GL_initMode);


  //----------------------------------------------------------------
  if(GL_initMode == 0) {

    GL_Set_Scale (1.0);
    GL_cen.x = 0.; GL_cen.y = 0.; GL_cen.z = 0.;

  }

    // UT3D_stru_dump (Typ_PT, &GL_cen, " GL_cen:");


  /* GL_Scale = 1.0; */
  /* GL_Scale_back = 1.0; */

  // GL_ang_X = 0.0;
  // GL_ang_Y = 0.0;
  // GL_ang_Z = 0.0;




  //GL_cen_Z = WC_ask_actZsur ();
  // GL_sur_act = GL_cen_Z;
  // GL_SetConstrPln ();


  UT3D_pl_XYZ (&GL_view_pln); // first load
  GL_SetViewPln ();


/*
  //GL_fix_space0 ();
  GL_Siz_X = (GL_Scr_Siz_X / GL_Scale) / 2.;
  GL_Siz_Y = (GL_Scr_Siz_Y / GL_Scale) / 2.;
  //GL_Siz_Z = GL_Scr_Siz_??? / GL_Scale;
  GL_Siz_Z = GL_Siz_X;
*/


  GL_Init_Siz ();       // init clipping planes


  // printf("GL_Init_View: siz=%f,%f,%f\n",GL_Siz_X,GL_Siz_Y,GL_Siz_Z);
  // UT3D_stru_dump (Typ_PLN,&GL_view_pln," GL_view_pln:");

}





//================================================================
  void GL_DefineView (int mode) {
//================================================================
/// \code
/// Change Views Function. Set GL_eyeX, GL_eyeZ.
///   mode  FUNC_ViewTop|FUNC_ViewFront|FUNC_ViewSide|FUNC_ViewIso
///         FUNC_Init
/// \endcode

  // Vector vcn;


  // printf("GL_DefineView %d\n",mode);

  if(mode == FUNC_Init) mode = GL_actView;

  GL_actView = mode;
  UI_VW_upd (GL_actView);  // enable/disable active view-button
  

  switch (mode) {


    /* von oben, XY */
    case FUNC_ViewTop:
              GL_eyeX.dx = 0.; GL_eyeX.dy = 0.; GL_eyeX.dz = 1.;
              GL_eyeZ.dx = 0.; GL_eyeZ.dy = 1.; GL_eyeZ.dz = 0.;
              GL_eyeY.dx = 1.; GL_eyeY.dy = 0.; GL_eyeY.dz = 0.;
              GL_Translate ();
      break;


    /* von vorn, YZ */
    case FUNC_ViewFront:
              GL_eyeX.dx = 0.; GL_eyeX.dy = -1.; GL_eyeX.dz = 0.;
              GL_eyeZ.dx = 0.; GL_eyeZ.dy = 0.; GL_eyeZ.dz = 1.;
              GL_eyeY.dx = 1.; GL_eyeY.dy = 0.; GL_eyeY.dz = 0.;
              GL_Translate ();
      break;


    /* von der Seite, XZ */
    case FUNC_ViewSide:
              GL_eyeX.dx = 1.; GL_eyeX.dy = 0.; GL_eyeX.dz = 0.;
              GL_eyeZ.dx = 0.; GL_eyeZ.dy = 0.; GL_eyeZ.dz = 1.;
              GL_eyeY.dx = 0.; GL_eyeY.dy = 1.; GL_eyeY.dz = 0.;
              GL_Translate ();
      break;


    case FUNC_ViewIso:
              GL_eyeX.dx = 1.; GL_eyeX.dy = -1.; GL_eyeX.dz = 1.;
              UT3D_vc_setLength (&GL_eyeX, &GL_eyeX, 1.);

              UT3D_vc_3db (&GL_eyeY, 1., 1., 0.);
              UT3D_vc_setLength (&GL_eyeY, &GL_eyeY, 1.);

              UT3D_vc_perp2vc (&GL_eyeZ, &GL_eyeX, &GL_eyeY);
                // UT3D_stru_dump (Typ_VC, &GL_eyeX, "GL_eyeX: ");
                // UT3D_stru_dump (Typ_VC, &GL_eyeZ, "GL_eyeZ: ");
              GL_Translate ();
      break;


    case FUNC_ViewReframe:
      GL_Reframe ();
      break;


    case FUNC_ViewReset:
              GL_initMode = 0;      // change scale !
              GL_Init_View ();
              // GL_Translate ();
      break;


    default:
      TX_Error ("verflixt ..");
      break;
  }

  ++GL_ViewModTimStamp;      // view modified ..

  GL_Redraw ();


    // TESTBLOCK
    // printf("ex GL_DefineView:\n");
    // UT3D_stru_dump (Typ_VC, &GL_eyeX, "GL_eyeX");
    // UT3D_stru_dump (Typ_VC, &GL_eyeY, "GL_eyeY");
    // UT3D_stru_dump (Typ_VC, &GL_eyeZ, "GL_eyeZ");
    // END TESTBLOCK

}



//================================================================
  void GL_DefineDisp (int mode, int mode1) {
//================================================================
// braucht hinterher kompletten Neuaufbau der Daten und redraw !
// mode1: ExtraLight
// MS braucht UI_GR_DrawInit vorher; viele calls unnutz !


  // printf("GL_DefineDisp %d %d\n", mode,mode1);


  switch (mode) {



    //====================================================================
    case FUNC_DispWire:
      //TX_Print("disp WireFrame");

      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE); // Flächen nur outlines!


      // wenn bessere darstelleung von Flaechen fertig, alle Flaechen so
      // ausblenden !!!
      // so wird gar keine Flaeche generiert !!
      // glEnable(GL_CULL_FACE);
      // glCullFace (GL_FRONT_AND_BACK);


      glDisable (GL_COLOR_MATERIAL);
      glDisable (GL_LIGHTING);


      //glShadeModel(GL_FLAT);
      // gluQuadricDrawStyle (quadObj, GLU_LINE);

      GL_mode_wire_shade = GR_STATE_WIRE;
      break;





    //====================================================================
    case FUNC_DispRend:
      // TX_Print("disp shaded");
      // geht nur mit korrektem Normalvektor!
      // normalize normalVectors; sehr wichtig; sonst alles ziemlich dunkel !!!!
      glEnable(GL_NORMALIZE);


      //  Hidden surface removal (sonst sieht man Punkte durch Flaechen durch !!
      glEnable(GL_DEPTH_TEST);
      glDepthFunc (GL_LEQUAL);              // eine Spur besser als LESS

      // glEnable(GL_AUTO_NORMAL);       // bei Linux u Windows nutzlos

      // statt GL_COLOR_MATERIAL und Light:  geht aber nicht !!
      // glDisable (GL_LIGHTING);
      // glColor3f (0.2f,0.2f,0.0f);

      //  Enable:  Do not calculate inside of solid obj; bleibt ganz weiss
      glDisable(GL_CULL_FACE);

      // // alle Dreiecke umdrehen GL_CW; Def=GL_CCW.
      // glFrontFace (GL_CW);    


      // // Set Material properties to follow glColor values
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);  // beidseitig
      // glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);  // nur FRONT

      // glColorMaterial (GL_BACK,  GL_DIFFUSE);
      // glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT);
      // glColorMaterial (GL_FRONT,  GL_AMBIENT);
      // glColorMaterial (GL_FRONT,  GL_SPECULAR);
      // glColorMaterial (GL_BACK,  GL_AMBIENT);
      // // glColorMaterial (GL_BACK,  GL_AMBIENT_AND_DIFFUSE);


      // Reflexion - 0 bis 128 macht grobe Fehler !
      // glMateriali  (GL_FRONT, GL_SHININESS, 2);
      // glMateriali  (GL_BACK, GL_SHININESS, 2);


      // Enable color tracking (erst nach glColorMaterial)
      glEnable (GL_COLOR_MATERIAL);


      // // So werden beide Seiten gleich (nur diffus, kein Diffus!
      // // man kann damit angeblich (mit Wert 0) auch auf nur aussen schalten
      // // od GL_TRUE od GL_FALSE
      glLightModelf (GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // MS-Win: Super!
      // glLightModelf (GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE); // tut nix ..
      // glLightModelfv (GL_LIGHT_MODEL_TWO_SIDE, GL_light_2si);
      // // gut einstellbar mit GL_mod_amb
      // glLightModelfv(GL_LIGHT_MODEL_AMBIENT, GL_mod_amb);


      // define light0
      // glLightfv(GL_LIGHT0, GL_POSITION, GL_light0_pos);

      // glLightfv(GL_LIGHT1, GL_AMBIENT,  GL_light_amb);  // hellt stark auf !
      // glLightfv(GL_LIGHT1, GL_DIFFUSE,  GL_light_dif);

      // // tut auch nix
      // // glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_local_view);


      // glLightfv(GL_LIGHT0, GL_SPECULAR, GL_light_spc); // zieml unnutz

      // Reflexion - 0 bis 128 macht grobe Fehler !
      glMateriali  (GL_FRONT, GL_SHININESS, 4);
      glMateriali  (GL_BACK, GL_SHININESS, 4);

      // // Spot effects; tut fast nix  machts ganz dunkel...
      // glLightfv (GL_LIGHT0, GL_SPOT_DIRECTION, GL_light0_spo);
      // glLightf  (GL_LIGHT0, GL_SPOT_CUTOFF, 45.0f);
      // glLightf  (GL_LIGHT0, GL_SPOT_EXPONENT, 100.0f);


      // // Daempfung 
      // // glLightf  (GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.8f);
      // glLightfv(GL_LIGHT0, GL_POSITION, GL_light0_pos);


      if(mode1 > 0) {
        // glEnable  (GL_LIGHT1);
          printf(" extraLight\n");
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, GL_mod_amb2);
        glMaterialfv (GL_FRONT, GL_SPECULAR, GL_mat_spc2);
        glMaterialfv (GL_BACK, GL_SPECULAR, GL_mat_spc2);

        glLightfv(GL_LIGHT0, GL_AMBIENT,  GL_light_amb2); // hellt stark auf
        glLightfv(GL_LIGHT0, GL_DIFFUSE,  GL_light_dif2);
        glLightfv(GL_LIGHT0, GL_SPECULAR, GL_light_spc2); // zieml unnutz

      } else {
        // glDisable  (GL_LIGHT1);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, GL_mod_amb1);
        glMaterialfv (GL_FRONT, GL_SPECULAR, GL_mat_spc1);
        glMaterialfv (GL_BACK, GL_SPECULAR, GL_mat_spc1);

        glLightfv(GL_LIGHT0, GL_AMBIENT,  GL_light_amb1); // hellt stark auf
        glLightfv(GL_LIGHT0, GL_DIFFUSE,  GL_light_dif1);
        glLightfv(GL_LIGHT0, GL_SPECULAR, GL_light_spc1); // zieml unnutz

      }

      glEnable  (GL_LIGHT0);
      glEnable  (GL_LIGHTING);       // Licht EIN



      // aussen od aussen u innen
      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // beide shaden; nur tw
      // glPolygonMode (GL_BACK, GL_LINE); // FRONT=Flaeche; BACK=Line.
      // glPolygonMode (GL_FRONT, GL_FILL); // FRONT=Flaeche; BACK=nix.

      // glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, GLcol_t1);
      // glMaterialfv(GL_FRONT, GL_EMISSION, GLcol_black);
      // glMaterialfv(GL_FRONT, GL_DIFFUSE, GL_diff1);
      // glMaterialfv(GL_FRONT, GL_AMBIENT, GL_amb1);


      // Default GL_FLAT wuerde Polygone nicht so schoen verlaufend
      // schattieren wie GL_SMOOTH
      // glShadeModel (GL_FLAT);
      // glShadeModel(GL_SMOOTH);   // Def !


      // glEnable  (GL_AUTO_NORMAL);       // bei Linux u Windows nutzlos


      // glEnable(GL_DITHER);
      // glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
      // glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);

      // glFrontFace(GL_CW);

      GL_mode_wire_shade = GR_STATE_SHADE;
      break;






    //====================================================================
    case FUNC_DispHid1:
      // nur mit dem Z-Buffer werden auch die Flaechen gezeichnet; 
      // dadurch werden die Polygone zerfleddert.
      // glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); 

      TX_Print("disp hidden");

      glEnable(GL_STENCIL_TEST);
      glClear(GL_STENCIL_BUFFER_BIT);
      glStencilMask(1);
      glStencilFunc(GL_ALWAYS, 0, 1);
      glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);

      // zuerst mal als outline
      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

      GL_mode_wire_shade = GR_STATE_HID1;

      break;



    //====================================================================
    case FUNC_DispHid2:

      // als Flaeche drueber
      glStencilFunc(GL_EQUAL, 0, 1);
      glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

      // als Flaeche in Hintergrundfarbe ..
      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

      GL_mode_wire_shade = GR_STATE_HID2;

      break;




    //====================================================================
    case FUNC_DispHid3:

      // nochmal Outline drueber ..
      glStencilFunc(GL_ALWAYS, 0, 1);
      glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);

      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

      GL_mode_wire_shade = GR_STATE_HID3;

      // das disable Stencil fehlt; das macht haessliche punkterln ...

      break;




    //====================================================================
    default:
      TX_Error ("verflixt ..");
      break;
  }


  //GR_Recreate ();
  if(AP_stat.sysStat > 1) GL_Redraw ();        // 2010-08-02

}


//================================================================
  int GL_Regen1 () {
//================================================================
// GL_Regen1             reset GR_TAB_IND (remove unused recs at end)

  long    l1;


  // printf("GL_Regen1 %ld\n",GR_TAB_IND);


  for(l1=GR_TAB_IND-1; l1>=0; --l1) {
      // printf(" _Regen1 %d %d\n",l1,GR_ObjTab[l1].typ);
    if(GR_ObjTab[l1].typ != 0) {
      GR_TAB_IND = l1 + 1;
      goto L_exit;
    }
  }
  GR_TAB_IND = 0;

  L_exit:
    // printf("ex GL_Regen1 %ld\n",GR_TAB_IND);
  return 0;

}


//================================================================
  int GL_Del_om (int typ, long ind, int iatt) {
//================================================================
// GL_Del_om             del multiple objects from DL
// delete all DL-Obj's of typ ind iatt
// use GL_Regen1 to Clean DL
// typ    
// ind     -1L = ignore, else ind must match.
// iatt    -1  = ignore, else iatt must match.

  long    l1;


    // printf("GL_Del_om %d %ld %d\n",typ,ind,iatt);
    // printf("  GR_TAB_IND=%ld\n",GR_TAB_IND);


  for(l1=0; l1<GR_TAB_IND; ++l1) {
    if(GR_ObjTab[l1].typ != typ) continue;
    if(ind >= 0) {
      if(GR_ObjTab[l1].ind != ind) continue;
    }
    if(iatt >= 0) {
      if(GR_ObjTab[l1].iatt != iatt) continue;
    }
    // delete this DL-record
      // printf(" _Del_om-del %d\n",l1);
    GR_ObjTab[l1].typ = 0;
    GL_Del1 (l1);

  }

  return 0;

}


//=====================================================================
  void GL_Del0 (long ind) {
//=====================================================================
//ein Obj der DL loeschen - oder (-1) gesamte DL loeschen

// see also DL_unvis_set

  GLuint DL_ind, ianz;


  //------------------------------------- 
  // printf("GL_Del0 %ld %ld\n",ind, GR_TAB_IND);



  if (ind == Typ_ALL_OBJS) {         // -1
    // TX_Print("GL_Del0 Typ_ALL_OBJS %d",GR_TAB_IND);
    // glDeleteLists (DL_base__, -DL_base__);

    ianz = GR_TAB_IND;
    // glDeleteLists (DL_base__, ianz);
    // 2006-03-16 raus; Mesa crasht wenn man wegloescht !!!!
    GR_TAB_IND = 0;  // 2006-03-16 zu

/*
    for(l1=0; l1<GR_TAB_IND; ++l1) {
      if (glIsList(l1+DL_base__)) {
        glDeleteLists (l1+DL_base__, 1);
      }
    }
*/


  } else {
    GR_ObjTab[ind].typ = 0;        // DispListEintrag unguelig
    GL_Del1 (ind);                 // delete one obj of GL-dispList
  }


}


//================================================================
  int GL_Del1 (long ind) {
//================================================================
// delete one obj of GL-dispList
// eigentlich sollte man auch den DL-Record loaschen;
// vorher testen, ob es DL-Records mit Links into DL-Table gibt !!!!


  GLuint   DL_ind, ianz;


  DL_ind = (GLuint)ind + DL_base__;


  // printf("GL_Del1 %ld %d\n",ind,DL_ind);


  if (glIsList(DL_ind)) {
    glDeleteLists (DL_ind, 1);
  }

  return 0;
}
 

//=====================================================================
  void GL_Delete (long ind) {
//=====================================================================
/// \code
/// reset dispList.
/// remove DL-record[ind] and all following records.
/// \endcode

// die DispList zuruecksetzen; beim nächsten redraw nicht mehr alle
// Obj darstellen.
// DL-Record ind und alle folgenden werden geloescht!

// see also GL_Del1  delete one obj of GL-dispList


  GLuint  l1, ianz;


  // ------------------------------------- 
  // printf("GGGGGGGGGGGGGG  GL_Delete von %ld bis %ld\n",ind, GR_TAB_IND);


  if(GR_TAB_IND <= 0) return;

  if(ind >= GR_TAB_IND) return;

  if(ind < 0) {GR_TAB_IND = 0; return;}


/*
    ianz = GR_TAB_IND - ind;
    glDeleteLists (l1+DL_base__, ianz+1);

    for(l1=ind; l1<GR_TAB_IND; ++l1) {
      if (glIsList(l1+DL_base__)) {
        glDeleteLists (l1+DL_base__, 1);
      }
    }
*/

  GR_TAB_IND = ind;


  // Das GL_Delete loescht auch BaseModels; wenn innerhalb dieses Bereiches ein
  // Basisdetail liegt, muss es auch geloescht werden !!!!
  DB_del_ModBas (ind);

  // printf("ex GL_Delete IND=%ld\n",GR_TAB_IND);


}


//================================================================
  int DL_set__ (long dli) {
//================================================================
/// delete all records following dli

// see GL_last_del
// see DL_disp_reset does also del refModels & activate parents of deletetd childs

  long    ii;


  ii = GR_TAB_IND - dli;

  glDeleteLists (ii, 1);

  GR_TAB_IND =  dli;

  return 0;

}

 
//=====================================================================
  void GL_last_del () {
//=====================================================================
// delete last obj of DL, if its DB-index is 0
// (CAD creates temporary preview-elements)

  long ii;


  ii = GR_TAB_IND - 1;

  // printf("GL_last_del %ld\n",ii);
  // return; // TEST ONLY

  if(ii < 0) return;

  if(GR_ObjTab[ii].ind != 0) return;
  
  glDeleteLists (ii, 1);

  GR_TAB_IND = ii;

}


//=====================================================================
  void GL_temp_Delete (long ind) {
//=====================================================================
/// \code
/// delete object in temp-area of DispList
/// see also GL_temp_delete
/// \endcode


  // printf("GL_temp_Delete %ld\n",ind);

  // DL_Ind_tmp    =   ind;


  if(ind < 0) {
    // delete object in temp-area of DispList (negative dli)
    ind *= -1L;
  }

  glDeleteLists (ind, 1);

}



//=====================================================================
  void GL_temp_delete () {
//=====================================================================
/// delete ALL objects in temp-area of DispList

  // problem, wen man mit ind=-2 ein temp obj erzeugt, wird obj 1
  // wieder sichtbar; eigentlich sollte man beim create index ein Loch mit
  // glDeleteLists schliessen !


  // printf("GL_temp_delete \n");


  // glDeleteLists (1, 1);
  glDeleteLists (1, DL_Ind_tmp);   //2008-04-10

  DL_Ind_tmp    =   1;

}


//================================================================
  long GL_temp_GetInd () {
//================================================================
/// GL_temp_GetInd        get index of last created temp-obj

  return DL_Ind_tmp - 1L;

}


//================================================================
  long GL_GetInd_last_temp () {
//================================================================
/// return last temp-obj-index.

  return -(DL_base_font1 - 1);

}

 
//=====================================================================
  long GL_GetInd_temp () {
//=====================================================================
/// return next free temp-obj-index.


  return -DL_Ind_tmp;

}


//================================================================
  int GL_RubberBox_do () {
//================================================================

  // printf("GL_RubberBox_do\n");


  GL_RubberBox_drw__ (0);

  return 0;

}


//================================================================
  int GL_RubberBox_stop () {
//================================================================

  // printf("GL_RubberBox_stop\n");

  GL_RubberBox_drw__ (1);   // remove box


  // if HIDE is active: transfer immediate ..
  if(UI_stat_hide == 0) {
    Grp_hide (0);    // hide group
  }



  // if VIEW is active: transfer immediate ..
  if(UI_stat_view == 0) {
    Grp_hide (1);    // view group
  }



  return 0;

}


//================================================================
  int GL_RubberBox_sel (Point *ptOri, int dx, int dy) {
//================================================================
// select objs in 2D-box
// Input:
//   dx, dy   width,height of box in screencoords


  int    i1, iNr, cx, cy, typ;
  long   dbi, dli;
  double sx, sy, sz;
  ObjDB  *dlTab;


  // printf("GL_RubberBox_sel\n");


  // dx=0 or dy=0 sellects all !
  if(abs(dx) == 0) return 0;
  if(abs(dy) == 0) return 0;


  // ptOri -> screenCoords
  GL_Uk2Sk (&sx, &sy, &sz, ptOri->x, ptOri->y, ptOri->z);
    // printf(" sx,sy = %f %f\n",sx,sy);

  // cx,cy = centerpoint of rectangle in screencoords
  cx = sx + (dx / 2);
  cy = sy + (dy / 2);
    // printf(" cx,cy = %d %d dx,dy = %d %d\n",cx, cy, dx, dy);


  // get objs in box
  iNr = GL_sel_rect (&dlTab, cx, cy, abs(dx), abs(dy));


  // add selected objs into group
  for(i1=0; i1<iNr; ++i1) {
    // printf("o[%d] %d %d %d\n",i1,dlTab[i1].typ,dlTab[i1].dbInd,dlTab[i1].dlInd);

    // put selected objs into group
    typ = dlTab[i1].typ;
    dbi = dlTab[i1].dbInd;
    dli = dlTab[i1].dlInd;
    Grp_add__ (typ, dbi, dli, 1);
    GR_ObjTab[dli].grp_1 = ON;
    // // activate mode "groupSelect"
    // if(UI_GR_Sel_Filter(-1) != 5) UI_GR_Sel_Filter (5);
  }

  Grp_upd (0);  // Redraw & update GrpNr-label

  return 0;

}


 
//================================================================
  int GL_RubberBox_drw__ (int mode) {
//================================================================
// mode=0=start, 1=stop.

static int rb_x, rb_y;
static int rb_dx, rb_dy;

static Point ptOri;

  double  d1;
  Vector  vc1;


  // printf("GL_RubberBox_drw__ mode=%d stat=%d\n",mode,GL_rub_stat);


  // test if groupSelect is allowed ..
  if(UI_block_group (-1) > 0) return 0;



  // stop ?
  if(mode != 0) {
    if(GL_rub_stat < 1) return 0;
  }


  // if uninitilized: save active position, noting else ..
  if(GL_rub_stat == 0) {
    rb_x = GL_mouse_x_act;
    rb_y = GL_mouse_y_act;
    GL_MousePos (&ptOri);
      // printf("GL_RubberBox_drw__ ptOri %f %f %f\n",ptOri.x,ptOri.y,ptOri.z);
    // entlang der Sichtlinie GL_eyeX verschieben; sonst Plane genau am
    // selektierten Punkt !
    vc1 = GL_GetEyeX ();
      // printf("  vc1 %f %f %f\n",vc1.dx,vc1.dy,vc1.dz);
    // hier sollte der Z-Wert des höchsten Punktes des Modelles benutzt werden ..
    // wenn zu hoch (>50000): keine Ausgabe sichtbar.  Default: GL_ModSiz * 4
    // what is maximum Z ? is 1 !
    d1 = GL_ModSiz * 4;
    if(d1 > 30000.) d1 = 30000.;
    UT3D_pt_traptvclen (&ptOri, &ptOri, &vc1, d1);
      // printf("  ptOri %f %f %f\n",ptOri.x,ptOri.y,ptOri.z);
    GL_rub_stat = 1;
    DL_Redraw ();  // nach einem select erforderl ?
    return 0;
  }


  // if box already exists: delete existing box
  if(GL_rub_stat == 2) {
    GL_RubberBox_draw (&ptOri, rb_dx, rb_dy, 1);
    GL_rub_stat = 1;
  }


  // stop ?
  if(mode != 0) {
    GL_rub_stat = 0;
    return GL_RubberBox_sel (&ptOri, rb_dx, rb_dy);
  }


  // fix new size of box
  rb_dx = GL_mouse_x_act - rb_x;
  rb_dy = rb_y - GL_mouse_y_act;
    // printf(" dx,dy = %d %d\n",rb_dx,rb_dy);

  // draw box
  GL_RubberBox_draw (&ptOri, rb_dx, rb_dy, 0);
  GL_rub_stat = 2;

  return 0;

}


//======================================================================
  int GL_RubberBox_draw (Point *ptOri, int rb_dx, int rb_dy, int mode) {
//======================================================================
/// \code
/// GL_RubberBox_draw
/// 1.call: create box; 2.call deletes box.
/// mode    0=createBox    1=removeBox
///
/// 2015-08-31 Does not work for Driver X.Org-X-Server for AMD/ATI
/// \endcode

// glRecti would also draw a filled rectangle (in user-coords)

// use single buffer (FRONT) !

// Z-height-problem: can glDepthFunc (GL_ALWAYS); help ?


  int     i1, i2;
  GLfloat *fBuf, fx, fy;

  static GLfloat fCol=0.5f;


  // printf("GL_RubberBox_draw %f %f %f %d %d %d\n",ptOri->x,ptOri->y,ptOri->z,
          // rb_dx,rb_dy,mode);



    // TEST ONLY
    // ptOri->x = 0.;
    // ptOri->y = 0.;
    // ptOri->z = 0.;
    // rb_dx = 300;
    // rb_dy = 300;
    // if(mode != 0) return 0;
    // TEST ONLY



  glDrawBuffer (GL_FRONT);


  // glDepthFunc (GL_NOTEQUAL); // ueberschreibt immer (auch Flächen !)

  // activate XOR
  glLogicOp(GL_XOR);
  glEnable(GL_COLOR_LOGIC_OP);


  // set cursorposition; usercoords.
  glRasterPos3dv ((double*)ptOri);
  // i1 = ptOri->x;
  // i2 = ptOri->y;
  // glRasterPos2i (i1, i2);

  // compute buffersize
  i1 = abs(rb_dx * rb_dy);

  // is size 0 (rb_dx or rb_dx is 0)
  if(i1 < 4) goto L_exit;

  // get buffer (alloca)
  fBuf = MEM_alloc_tmp (i1 * sizeof(GLfloat));
  if(fBuf == NULL) {TX_Error("GL_RubberBox_draw EOF"); goto L_exit;}


  // preset BitBuffer
  MEM_set_1recn (fBuf, &fCol, i1, sizeof(GLfloat));


  // move cursorposition (in screencoords)
  if((rb_dx < 0)||(rb_dy < 0)) {
    fx = 0.f;
    fy = 0.f;
    if(rb_dx < 0) { fx = rb_dx; rb_dx = abs(rb_dx); }
    if(rb_dy < 0) { fy = rb_dy; rb_dy = abs(rb_dy); }
    glBitmap (0,0, 0.f,0.f, fx,fy, NULL);
  }


  // draw bitmap
  glPixelZoom (1.f, 1.f);
  glDrawPixels (rb_dx, rb_dy,  // width, height
      // GL_BLUE,  // yellow
      GL_GREEN,    // violet
      // GL_RED,   // cyan
      GL_FLOAT,
      fBuf);                   // data


  L_exit:
  glDisable(GL_COLOR_LOGIC_OP);
  // glDepthFunc (GL_LEQUAL);      // reset = Standard

  // glFlush(); // must flush here
  glFlush();
  glDrawBuffer (GL_BACK);

  return 0;

} 


/*
//================================================================
  void  GL_Start_RubberBox () {
//================================================================

  Point  pt1;
  double d1;

  //TX_Print("GL_Start_RubberBox %d,%d",GL_mouse_x_act,GL_mouse_y_act);

  if ((GL_StatRubberBand == ON) || (GL_StatRubberBox == ON)) {
    GL_Stop_RubberX (&d1, &pt1);
  }

  // die aktuelle Posi in Userkoords
  pt1 = GL_GetViewPos ();

  // der fixe Eckpunkt der Rubberbox
  GL_Rubber_Ori = pt1;

  //TX_Print("Rubber_Ori = %f,%f,%f",GL_Rubber_Ori.x,GL_Rubber_Ori.y,GL_Rubber_Ori.z);

  GL_StatRubberBox = ON;

}


//=====================================================================
  void  GL_Stop_RubberX (double *siz, Point *ptC) {
//=====================================================================
// das RubberBand ist das DL-Obj 0.
// Mittelpunkt aus GL_Rubber_Ori und dem Aktuellen ausrechnen

  Point pt1;


  if ((GL_StatRubberBand == ON) || (GL_StatRubberBox == ON)) {

    //UI_GR_DrawInit ();

    GL_StatRubberBand = OFF;
    GL_StatRubberBox  = OFF;

    // weg damit
    GL_Redraw ();

    pt1 = GL_GetViewPos ();

    // das Center der neuen Ansicht
    pt1.z  = 0.0;
    ptC->z = 0.0;
    UT3D_pt_mid2pt (ptC, &pt1, &GL_Rubber_Ori);

    *siz = UT3D_len_2pt (ptC, &GL_Rubber_Ori) / 2.0;

    //UI_GR_DrawExit ();
  }

}


//=====================================================================
  void GL_Do_RubberBox (){
//=====================================================================
// eine Box hinmalen

  Point    slo, sru, plu, pro, pru;

  //TX_Print("GL_Do_RubberBox");


  // die aktuelle Curpos (Rechts Unten)
  pru = GL_GetViewPos ();


  // Origin Retour into Screenkoord's (slo = Screen links Oben)
  GL_Uk2Sk (&slo.x, &slo.y, &slo.z,
               GL_Rubber_Ori.x, GL_Rubber_Ori.y, GL_Rubber_Ori.z);

  GL_Uk2Sk (&sru.x, &sru.y, &sru.z,
               pru.x, pru.y, pru.z);

  GL_Sk2Uk (&plu.x, &plu.y, &plu.z, slo.x, sru.y, sru.z);

  GL_Sk2Uk (&pro.x, &pro.y, &pro.z, sru.x, slo.y, sru.z);


  // redraw all (die vorherige Box löschen)
  GL_Redraw ();

  // call DefaultLineAttributes
  //glCallList ((GLuint)DL_base_LnAtt);

  glDepthFunc (GL_ALWAYS);            // ueberschreibt immer (auch Flächen !)

  glDisable (GL_LIGHTING);            // damit Polygon nicht schattiert wird

  glDisable (GL_LINE_STIPPLE);	           	// Disable Strich-Punkt
  //glLineStipple (1, (GLushort)0xFFFF);       // solid
  glColor3f (0.f, 0.f, 0.f);                 // schwarz


  glBegin (GL_LINE_LOOP);
    glVertex3d (GL_Rubber_Ori.x, GL_Rubber_Ori.y, GL_Rubber_Ori.z);
    glVertex3d (       plu.x,        plu.y,        plu.z);
    glVertex3d (       pru.x,        pru.y,        pru.z);
    glVertex3d (       pro.x,        pro.y,        pro.z);
  glEnd ();

  glEnable (GL_LIGHTING);
  // glDepthFunc (GL_LEQUAL);     // wieder normal; eine Spur besser als LESS
  //UI_GR_DrawExit ();

}
*/


//=====================================================================
  void*  GL_Print1   (int *iw, int *ih) {
//=====================================================================
// get bitmap (format BGR) of active OpenGL-window;
// returns bitmap; must be freed.
// Output:
//   iw, ih        width, height
//   retCod        bitmap




  int  bSiz;
  static void* buf;


  printf("GL_Print1\n");


  glGetIntegerv (GL_VIEWPORT, GL_Viewp);  // get Viewport-Matrix

  *iw = GL_Viewp[2];  // width
  *ih = GL_Viewp[3];  // height

  // compute memsiz
  // bSiz = (*iw * *ih) + 32;            // for grayscale
  bSiz = (*iw * *ih * 3) + 32;           // BGR
    printf(" iw=%d ih=%d bSiz=%d\n",*iw,*ih,bSiz);

  buf = malloc (bSiz);
  if(buf == NULL) return NULL;



  // alignement in bytes
  glPixelStorei(GL_PACK_ALIGNMENT, 1);

  // nr of rows to skip from bottom
  glPixelStorei(GL_PACK_SKIP_ROWS, 0);

  // nr of pixels to skip on left side of image
  glPixelStorei(GL_PACK_SKIP_PIXELS, 0);

  // glPixelStorei(GL_PACK_ROW_LENGTH, 0);
  // glReadBuffer(GL_BACK);

  // den gesamten Buffer ins memory
  glReadPixels(0, 0,                               // offestpos lower left
      (GLsizei)GL_Viewp[2], (GLsizei)GL_Viewp[3],  // width height
      // GL_RED|GL_BLUE|GL_GREEN,                     // Absturz
      // GL_LUMINANCE,                                // grayscale
      GL_BGR_EXT,
      // GL_ALPHA,                                    // geht ned
      GL_UNSIGNED_BYTE,
      (GLvoid*)buf);


  // free(buf);
  return buf;

}



//================================================================
  int GL_sel_nr_set (int iNr) {
//================================================================

  GR_nr_selTab = iNr;

  return 0;

}


//================================================================
  int GL_sel_nr_get () {
//================================================================

  return GR_nr_selTab;

}



//================================================================
  int GL_sel_get (int *typ, long *dbi, long *dli, int iNr) {
//================================================================
// get GR_selTab-record

  // printf("GL_sel_get %d\n",iNr);


  if(GR_nr_selTab < 1) {
    *typ = Typ_TmpPT;
    *dbi = -1L;
    *dli = -1L;
    return -1;
  }

  *typ = GR_selTab[iNr].typ;
  *dbi = GR_selTab[iNr].dbInd;
  *dli = GR_selTab[iNr].dlInd;

  return 0;

}



//================================================================
  int GL_sel_sel (ObjDB **dla) {
//================================================================
// return objs from selection

  int   px, py;


  px = GL_mouse_x_act;
  py = GL_Viewp[3] - GL_mouse_y_act;

  return GL_sel_rect (dla, px,py,GL_pickSiz,GL_pickSiz);

}


//================================================================
  int GL_sel_sort () {
//================================================================
// sort lowest DB-types first ..

  int    i1, i2, i3, imax;


  for(i1=0; i1<GR_nr_selTab; ++i1) {
    imax = 9999;

    // get i3 = lowest_typ between i1-selNr
    for(i2=i1; i2<GR_nr_selTab; ++i2) {
      if(GR_selTab[i2].typ < imax) { imax = GR_selTab[i2].typ; i3 = i2;}
    }

    // printf("  swap %d %d\n",i1,i3);
    if(i1 != i3) {
      MEM_swap__ (&GR_selTab[i3], &GR_selTab[i1], sizeof(ObjDB));
    }
  }



  return 0;

}


//================================================================
  int GL_sel_rect (ObjDB **dla,
                   int bx, int by, int bdx, int bdy) {
//================================================================
// returns the ID's of all objs in rectangle
// rectangle from centerpoint & size, in screencoords;
// objs partially or full in rectangle
//
// Input:  
//   bx,by     centerpoint of rectangle in screencoords
//   bdx,bdy   width,height in screencoords
// Input,Output:
//   dlaSiz    size of dla; reallocate if necessary
// Output:
//   dla       DispListindex of objs in rectangle
// RetCod      nr of objs in rectangle (in dla)
//
// see also GL_Select
  


  GLint    GL_Viewp[4];

//...................................
// #define BUF_SIZ 40000 
//   GLuint   selectBuf[BUF_SIZ];
  GLuint   *selectBuf, BUF_SIZ;
//...................................
  int      ihit, i1, i2, i3, imax,typ, att, namAnz, pick, selNr, *ip;
  long     l1, ind, oNam, apt_ind;
  int      hits;  // ATTENTION: GLuint hits makes Error with "if(hits < 1)" !!
  Point    pt1;
  GLfloat  f1;
//...................................



  // printf("GL_sel_rect siz=%ld %d %d %d %d\n",GR_Siz_selTab,bx,by,bdx,bdy);


  // fix GL-bufferspace ..
  selectBuf = (int*)memspc501;
  BUF_SIZ = sizeof(memspc501) / sizeof(GLuint);
  // BUF_SIZ = 1000; // TEST ONLY !
    // printf(" GL_sel_rect-BUF_SIZ=%d\n",BUF_SIZ);


  glGetIntegerv (GL_VIEWPORT, GL_Viewp);            // get Viewport-Matrix


//----------------------------------------------------------------
// GL_Select

  // switch to ProjectionMode
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();  // load Defaultmat.

  // Init Sel. Buffer
  glSelectBuffer (BUF_SIZ, selectBuf);


  glRenderMode (GL_SELECT);
  //glRenderMode (GL_FEEDBACK);
  // GL_mode_draw_select = GR_MODE_SELECT;    // statt DispList,Farben Names.
  GL_Redra_mode (GR_MODE_SELECT);


  // Viewport rund um die SelectPosition einrichten.
  // gluPickMatrix (px, py, GL_pickSiz, GL_pickSiz, GL_Viewp);
  // (xCen,yCen, width,heigt) als 4 doubles in screenCoords
  gluPickMatrix (bx, by, bdx, bdy, GL_Viewp);
    // printf("   pickport = %f,%f",px,py);


  // glOrtho
    // printf(" gl_sel 1\n");
  GL_Reframe1 (); // ganz wichtig !
  // redraw all

    // printf(" gl_sel 2\n");
  DL_Redraw ();

  // Abschliessen, Buffer fuellen
  hits = glRenderMode (GL_RENDER);
    // printf(" hits=%d\n",hits);


  // restore 
  // GL_mode_draw_select = GR_MODE_DRAW;
  GL_Redra_mode (GR_MODE_DRAW);


//-----------------------------------------------------------------
// GL_Select2

  ihit  = 0;
  ind   = -1;
  GL_sel_nr_set (0);
  selNr = 0;       // fuer no select !


  if(hits < 0) {
    TX_Print("ERROR: selectbuffer overflow ...");
    goto Fertig;
  }


  CheckNext:

    ++ihit;
    if(ihit > hits) goto Fertig;

      // printf(" next %d %d %d\n",selectBuf[ind],selectBuf[ind+1],
                // selectBuf[ind+4]);

    ++ind;
    namAnz = selectBuf[ind];

    ++ind;   // z1 = selectBuf[ind];
    ++ind;   // z2 = selectBuf[ind];


    // manchmal kommen leere Records ..
    if(namAnz < 1) {
        // printf(" skip empty rec\n");
      goto CheckNext;
    }


    for(i1=0; i1<namAnz; ++i1) {
      ++ind;
      oNam = selectBuf[ind];   // get the DL-index
        // printf("_Select2: Sel: #=%d oNam=%ld\n",i1,oNam);
        // DL_DumpObj__ (oNam);
    }


    // skip obj, if it is not pickable
    if(oNam < GR_TAB_IND) {
      if ((GR_ObjTab[oNam].pick) == ON) {
          // printf(" skip unpickable %ld\n",oNam);
        goto CheckNext;
      }
    }


    // typ = DL_GetObj (oNam, &apt_ind, &att);
    // typ     = DL_GetTyp (oNam);
    // apt_ind = DL_get_dbi (oNam);
    // printf(" .... save[%d] dli=%d typ=%d ind=%ld pick=%d\n",selNr,
                  // oNam,typ,apt_ind,pick);


    // save
    selNr = GL_sel_add_DL (dla, oNam);

    goto CheckNext;
  //----------------------------------------------------------------


  Fertig:

  *dla = GR_selTab;

  return selNr;

}


//================================================================
  int GL_selTab_realloc () {
//================================================================

  long  newSiz;

  GR_Siz_selTab += 2000;

  newSiz = sizeof(ObjDB) * (GR_Siz_selTab + 2);

  GR_selTab = (ObjDB*)realloc(GR_selTab, newSiz);

  if(GR_selTab) return 0;

  return -1;

}


//================================================================
  int GL_sel_add_DB (ObjDB **dla, long dbi, int typ) {
//================================================================
// add DB-obj into selectionBuffer GR_selTab

  // printf("GL_sel_add_DB %d %d %ld\n",GR_nr_selTab,typ,dbi);


  // realloc space in dla if necessary
  if(GR_nr_selTab >= GR_Siz_selTab) {
    if(GL_selTab_realloc() < 0) {
      TX_Error("GL_sel_add_DB E001");
      return -1;
    }
    *dla = GR_selTab;
  }

  GR_selTab[GR_nr_selTab].dlInd = -1L;
  GR_selTab[GR_nr_selTab].dbInd = dbi;
  GR_selTab[GR_nr_selTab].typ   = typ;
  ++GR_nr_selTab;

  return GR_nr_selTab;

}


//================================================================
  int GL_sel_add_DL (ObjDB **dla, long dli) {
//================================================================
// add DL-obj into selectionBuffer GR_selTab


  int    ii, i1, typ, *ia;
  long   l1;
  char   *ta, *stat;


  // printf("GL_sel_add_DL %ld\n",dli);


  // realloc space in dla if necessary
  if(GR_nr_selTab >= GR_Siz_selTab) {
    if(GL_selTab_realloc() < 0) {
      TX_Error("GL_sel_add_DL E001");
      return -1;
    }
    *dla = GR_selTab;
  }


  // test VectorSelect, 2D-button-selection
  if(dli >= GR_TAB_IND) {
    l1 = GR_TAB_IND - dli - 1;
      printf(" GL_sel_add_DL l1=%ld\n",l1);

    if(l1 <= -11L) {             // 2D-button selected
      ii = -l1 - 11;
      // get typTable & textTable for iconTags
      sele_get_icons (&i1, &ia, &ta, &stat);
      typ = ia[ii];       // typ = Typ_modREV;
      l1 = 0;
      GR_nr_selTab = 0;   // highest priority; overwrite underlying objs


    } else if(l1 < -7L) {        // StdPlane  8-10
      typ = Typ_PLN;
      l1 += 7;

		} else {                    // stdVec selected  1-7
      typ = Typ_VC;
      if(l1 == -7L) l1 = GL_vcSelStat;
      // // test if already (underlying) objs selected; skip these
      // for(i1=GR_nr_selTab-1; i1>=0; --i1) {
        // if(GR_selTab[i1].dlInd != 0) GR_nr_selTab 
      // }
    }
      // printf(" selTyp=%d selDli=%ld\n",typ,l1);

    GR_selTab[GR_nr_selTab].dlInd = -1L;      // do not display
    GR_selTab[GR_nr_selTab].dbInd = l1;       // DB_VCX_IND
    GR_selTab[GR_nr_selTab].typ   = typ;


  } else {
    // normal obj selected ..
    GR_selTab[GR_nr_selTab].dlInd = dli;
    GR_selTab[GR_nr_selTab].dbInd = DL_get_dbi (dli);
    GR_selTab[GR_nr_selTab].typ   = DL_GetTyp (dli);
  }

  ++GR_nr_selTab;

    // printf("ex GL_sel_add_DL %d\n",GR_nr_selTab);

  return GR_nr_selTab;

}


//================================================================
  int GL_sel_del_rec (ObjDB **dla, int iDel) {
//================================================================
// delete record <iDel>

  int   i1, i2;

  
  // printf("GL_sel_del_rec %d von %d\n",iDel,GR_nr_selTab);


  for(i1 = iDel + 1; i1 < GR_nr_selTab; ++i1) {
    i2 = i1 - 1;
      // printf(" cp %d -> %d\n",i1,i2);
    GR_selTab[i2] = GR_selTab[i1];
  }
  --GR_nr_selTab;

  return GR_nr_selTab;

}

 
//================================================================
  int GL_sel_dump () {
//================================================================
// see sele_dump2
 
  int   l1;

  printf("GL_sel_dump %d:\n",GR_nr_selTab);

  for(l1=0; l1<GR_nr_selTab; ++l1) {
    printf(" dl[%d] typ=%d dbi=%ld dli=%ld\n", l1,
      GR_selTab[l1].typ, GR_selTab[l1].dbInd,GR_selTab[l1].dlInd);
  }

  return 0;

}


/*
//=====================================================================
  int GL_Select (int mode, int *typTab, long *indTab, long *dlTab) {
//=====================================================================
// mode=-1:    change ViewPlane
// returns nr of selected objects

#define BUF_SIZ 2048

// static GLdouble pickSiz = 8.0;

  int      hits;  // ATTENTION: GLuint hits makes Errorr with "if(hits < 1)" !!
  GLuint   selectBuf[BUF_SIZ];
  GLdouble px, py;

  int             i1;
  Point           pt1;

  // printf ("GL_Select..................\n");


  // save the curPos --> GL_actUsrPos
  // DL_Redraw ();
  // i1 = GL_MousePos (&pt1);

  // GL_actUsrPos = GL_GetViewPos ();
  // GL_set_viewPlnPos ();  // sets GL_actScrPos & GL_actUsrPos



  // UI_GR_DrawInit ();

  // switch to ProjectionMode
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();  // load Defaultmat.


  // Init Sel. Buffer
  glSelectBuffer ((GLsizei)BUF_SIZ, selectBuf);


  glRenderMode (GL_SELECT);
  //glRenderMode (GL_FEEDBACK);
  GL_mode_draw_select = GR_MODE_SELECT;    // statt DispList,Farben Names.


  // 4x4-Pixel Viewport rund um die SelectPosition einrichten.
  px = GL_mouse_x_act;
  py = GL_Viewp[3] - GL_mouse_y_act;
    // printf("   pickport = %f,%f",px,py);
  gluPickMatrix (px, py, GL_pickSiz, GL_pickSiz, GL_Viewp);

  // glOrtho
    // printf(" gl_sel 1\n");
  GL_Reframe1 (); // ganz wichtig !

  // redraw all
    // printf(" gl_sel 2\n");
  GL_Redraw ();

  // Abschliessen, Buffer fuellen
    // printf(" gl_sel 3\n");
  hits = glRenderMode (GL_RENDER);
    // printf(" hits=%d\n",hits);

  // UI_GR_DrawExit ();

  // restore 
  GL_mode_draw_select = GR_MODE_DRAW;


  // in der Hitliste den besten raussuchen
  // unused seit 2003-09-06
  // GL_Select1 (mode, obj_sel, hits, selectBuf);

  return GL_Select2 (mode, typTab, indTab, dlTab, hits, selectBuf);


  //  if (Func_Act == FUNC_ViewZoomBox) { 
    //  TX_Print ("do FUNC_ViewZoomBox"); 
    //  TX_Print ("noch nicht implementiert ............."); 
    //  GL_Stop_RubberX ();
  //  } 


  // if (*obj_sel < -9998) {
    // //TX_Print("GL_Select ex: nix getroffen %d",*obj_sel);
    // return;
  // }



  /// die Axisselektionen > Texteingabe
  /// if (*obj_sel < 0) { 
    /// if (*obj_sel == -3) {
      /// strcpy (buf, "x");
    /// } else if (*obj_sel == -2) {
      /// strcpy (buf, "y");
    /// } else if (*obj_sel == -1) {
      /// strcpy (buf, "z");
    /// }
    /// CI_AddTxt (buf);
    /// return;
  /// }



  // return;


}


  UNUSED ..
//===========================================================================
  int GL_Select2 (int mode, int *typTab, long *indTab, long *dlTab,
                  int hits, GLuint *selectBuf) {
//===========================================================================
// Alle objecte decodieren und retournieren
// mode=-1:    change ViewPlane
// returns nr of selected objects
//   0 = nix selektiert
// Inhalt Select-Buffer: 
// 1) Nr of names on stack
// 2) min.z-val (0-1)
// 3) max.z-val (0-1)
// stack;unterstes zuerst.

  //Object_Status   stat;
  //GLint           hits;

  int             ihit, i1, i2, i3, imax,typ, att, namAnz, pick, selNr, *ip;
  long            l1, ind, oNam, apt_ind;
  // double          z_val, z_last, z1, z2;

  Point           pt1;
  GLfloat         f1;


// Prototypen:
  double   UI_vwz__();



  //--------------------------------------------------
  // printf("222222222222222222 GL_Select2 mode=%d hits=%d\n",mode,hits);

  if(hits < 1) return -1;

  // if(hits > 100) hits = 100;

  // DL_Redraw ();



  // // get & save the selected position ..
  // i1 = GL_MousePos (&pt1);
  // if(i1 < 0) {
    // printf("GL_Select2 SELECT-PROBLEM\n");
    // return 0;
  // }
  // sele_set_pos (&pt1);




  //================================================================
  ihit  = 0;
  selNr = 0;
  ind   = -1;



  //================================================================
  // DisplayOnly
  // ip = selectBuf;
  // for(i1=0; i1<hits; ++i1) {
      // i2 = *ip;
      // printf(" number of names for hit %d = %d\n",i1,i2);   ++ip;
      // printf(" zmin is %g\n", (float) *ip/0x7fffffff);      ++ip;
      // printf(" zmax is %g\n", (float) *ip/0x7fffffff);      ++ip;
      // for(i3=0; i3<i2; ++i3) {  // for each name
         // printf("      name %d = %d\n",i3,*ip);             ++ip;
      // }
   // }





  //================================================================
  CheckNext:

  ++ihit;
  if(ihit > hits) goto Fertig;


  // printf(" next %d %d %d\n",selectBuf[ind],selectBuf[ind+1],
                // selectBuf[ind+4]);


  ++ind;
  namAnz = selectBuf[ind];

  ++ind;   // z1 = selectBuf[ind];
  ++ind;   // z2 = selectBuf[ind];


  // manchmal kommen leere Records ..
  if(namAnz < 1) {
      // printf(" skip empty rec\n");
    goto CheckNext;
  }


  for(i1=0; i1<namAnz; ++i1) {
    ++ind;
    oNam = selectBuf[ind];
      // printf("_Select2: Sel: #=%d oNam=%ld\n",i1,oNam);
    // DL_DumpObj__ (oNam);
  }



  // skip obj, if it is not pickable
  if ((GR_ObjTab[oNam].pick) == ON) {
      // printf(" skip unpickable %ld\n",oNam);
    goto CheckNext;
  }



  // den Typ anfragen
  // typ = DL_GetObj (oNam, &apt_ind, &att);
  typ     = DL_GetTyp (oNam);
  apt_ind = DL_get_dbi (oNam);
    // pick    = DL_GetPick(oNam);
    // printf(" oNam=%d typ=%d ind=%d pick=%d\n",oNam,typ,apt_ind,pick);

  // if(DL_GetPick(oNam) == ON)  goto CheckNext;


  // wenn ViewZ aktiv ist (mode = 1) nur Typ_SymB akzeptieren
  if(mode == -1) {
      if(typ != Typ_SymB) goto CheckNext;
      // die VP-syms (Sternderln) kommen nach den normalen DL-obj's,
      // l1 ist Anz. normales DL-obj's.
      l1 = UI_vwz__ (3, 0L);
      // printf(" z-sym %d %d\n",apt_ind,l1);
      // apt_ind der VP-syms ist ihr neg. DL-Ind.
      if(abs(apt_ind) < l1) goto CheckNext;  // skip normal symbols 
  }




  // save
  // printf(" .... save[%d] dli=%d typ=%d ind=%ld pick=%d\n",selNr,
                  // oNam,typ,apt_ind,pick);

  dlTab[selNr]  = oNam;
  typTab[selNr] = typ; 
  indTab[selNr] = apt_ind; 


  if(selNr < 50) {
    ++selNr;
    if(mode == -1) return 1;   // sel ViewPlane
    goto CheckNext;
  }


  TX_Print("selectbuffer overflow");




  //=======================================================
  Fertig:

  // sort !
  for(i1=0; i1<selNr; ++i1) {
    imax = 9999;
    for(i2=i1; i2<selNr; ++i2) {
      if(typTab[i2] < imax) { imax = typTab[i2]; i3 = i2;}
    }
    // printf("  swap %d %d\n",i1,i3);
    if(i1 != i3) {
      MEM_swap__ (&typTab[i3], &typTab[i1], sizeof(int));
      MEM_swap__ (&indTab[i3], &indTab[i1], sizeof(long));
      MEM_swap__ (&dlTab[i3], &dlTab[i1], sizeof(long));
    }
  }




    // printf("ex GL_Select2: found %d aus %d\n", selNr, hits);

  return selNr;

}


   unused ...
//===========================================================================
  void GL_Select1 (int mode, long* obj_sel, GLuint hits, GLuint *selectBuf) {
//===========================================================================
// Bei Z-Hohe ist die niederere Zahl naeher beim Betrachter.
//  Aber unsigned long!
// -1 = nix selektiert


  //Object_Status   stat;
  //GLint           hits;

  int             ihit, i1, typ, typOld, att, namAnz, pick;
  long            ind, oNam, apt_ind, apt_indOld;
  double          z_val, z_last, z1, z2;

  // -----------------------------------------

  *obj_sel = -99999;


  //glGetIntegerv (GL_NAME_STACK_DEPTH, &hits);


  // printf("GL_Select1 hits=%d mode=%d\n", hits,mode);


  //return;


  if (hits < 1) { goto Fertig; }

  if(hits > 9) hits = 9;


  ihit = 0;
  ind = -1;
  z_last = VAL_HUGE;
  typOld = 9999;
  apt_indOld = 99999999;


  //--------------------------------------------------
  CheckNext:

  ++ihit;
  if(ihit > hits) goto Fertig;


  // printf(" next %d %d %d %d\n",selectBuf[ind+1], selectBuf[ind+2],
                               // selectBuf[ind+3], selectBuf[ind+4]);


  ++ind;
  namAnz = selectBuf[ind];

  ++ind;
  z1 = selectBuf[ind];
  ++ind;
  z2 = selectBuf[ind];


  // manchmal kommen leere Records ..
  if(namAnz < 1) {
    //++ind;
    goto CheckNext;
  }


  for(i1=0; i1<namAnz; ++i1) {
    ++ind;
    oNam = selectBuf[ind];
    // printf("Sel: #=%d oNam=%ld\n",i1,oNam);
  }



  // if(namAnz > 1) {
    // TX_Error("GL_Select1 E001");
    // ind = ind + namAnz -1;
  // }


  //TX_Print(" selection # %d = ID %d",ihit,oNam); 



    // i2 = i1*4;
    // oi = selectBuf[i2+3];
    // TX_Print(" next %d %d %d ID=%d %lu %lu", i1, i2, oi,
      // selectBuf[i2], selectBuf[i2+1], selectBuf[i2+2]);


    // if (oi >= 0) {
      // stat = DB_Get_Stat (oi);

    // die sysAxis hat DL -3,-2,-1
    // } else {
      // stat.pick = ON;
      // stat.otyp = Typ_LN;
    // }
    // TX_Print("      %d %d %d", oi, stat.otyp, stat.pick);


    // skip obj, if it is not pickable
    if ((GR_ObjTab[oNam].pick) == ON) {
      // printf("skip unpickable %ld\n",oNam);
      goto CheckNext;
    }




    // z_val = (fabs((double)(selectBuf[i1+1])) +
             // fabs((double)(selectBuf[i1+2]))) / 2.0;
    // printf(" sel3 %f %f %f\n",selectBuf[i1+1],selectBuf[i1+2],selectBuf[i1+3]);



    // den Typ anfragen
    // typ = DL_GetObj (oNam, &apt_ind, &att);
    typ     = DL_GetTyp (oNam);
    apt_ind = DL_get_dbi (oNam);
    pick    = DL_GetPick(oNam);

    if(DL_GetPick(oNam) == ON)  goto CheckNext;
    // printf(" .... next typ=%d ind=%ld pick=%d\n",typ,apt_ind,pick);



    // wenn ViewZ aktiv ist (mode = 1) nur Typ_SymB akzeptieren
    if(mode == 1) {
      if(typ == Typ_SymB) {
        *obj_sel = oNam;
        goto Fertig;
      }
    }


    if(typ == Typ_Error) goto CheckNext;


    // printf("Sel: #=%d oNam=%ld typ=%d apt_ind=%ld\n",i1,oNam,typ,apt_ind);

    // selektieren, wenn sein Typ höhere Priorität hat.
    // sel obj, if its Typ < typFoundBefore
    if(typ < typOld) {
      // printf(" take typ=%d typOld=%d\n",typ,typOld);
      typOld = typ;
      apt_indOld = apt_ind;
      goto Take_It;
    }


    if (typ > typOld) goto CheckNext;



    // typen sind nun gleich; den mit dem niedrigeren aptIndex waehlen
    if(apt_ind < apt_indOld) {
      // printf(" take typ=%d typOld=%d\n",typ,typOld);
      apt_indOld = apt_ind;
      goto Take_It;
    }



    // // skip obj lower than objfoundBefore
    // if (z_val > z_last) {
      // continue;
    // }



  goto CheckNext;





  Take_It:
    // printf(" Take typ=%d oNam=%ld\n",typ,oNam);

    //z_last = z_val;
    *obj_sel = oNam;
    //TX_Print("take that");


  goto CheckNext;




  Fertig:
  //TX_Print("ex GL_Select1: found %d aus %d", *obj_sel, hits);
  return;

}


  UNUSED ..
//================================================================
  int GL_tstFuncs (int mode) {
//================================================================
// mode=0   store ctive DispListIndex
// mode=1   delete DispList from stored Position


static long dli;


  if(mode == 0) {
    // dli = GR_TAB_IND-1;          // GL_GetActInd
    dli = GR_TAB_IND;               // GL_GetActInd
    printf("GL_tstFuncs save %ld\n",dli);


  } else {
    printf("GL_tstFuncs restore %ld\n",dli);
    GL_Delete (dli);
    DL_Redraw ();
  }


  return 0;

}
*/
 
//=====================================================================
  long GL_GetActInd () {
//=====================================================================
// liefert Index des letzen obj (GR_TAB_IND zeigt auf das naechste freie)

  // printf("ex GL_GetActInd %d\n",GR_TAB_IND-1);

  return GR_TAB_IND-1;

}


//=====================================================================
  long GL_Get_DLind () {
//=====================================================================
// GL_Get_DLind      return DL-Anzahl

  return GR_TAB_IND;

}


//=====================================================================
  int GL_GetViewPln (Plane *pln) {
//=====================================================================

  *pln = GL_view_pln;

  return 0;

}


//=====================================================================
  int GL_SetViewPln () {
//=====================================================================
// GL_view_pln setzen aus GL_view_pln, GL_cen.z; (hier ist Z-Wert aufgerechnet !)

static double old_view_Z = 0.;

  double dz;
  Point  pt1;


  // printf("GL_SetViewPln VVVVVVVVVVVVVVVVVVVV %f\n",GL_cen.z);

  if(fabs(GL_cen.z-old_view_Z) > UT_TOL_pt) {

    dz = GL_cen.z - old_view_Z;
    if(fabs(dz) > UT_TOL_pt) {
      // UT3D_pt_traptvclen (&pt1, &GL_view_pln.po, &GL_view_pln.vz, dz);
      GL_view_pln.po.z = GL_cen.z;
      // UT3D_stru_dump (Typ_PT, &t1, "&new po:");
      UT3D_pl_ptpl (&GL_view_pln, &GL_view_pln.po); // nur move Nullpunkt
      old_view_Z = GL_cen.z;
      // UT3D_stru_dump (Typ_PLN, &GL_view_pln,"&new GL_view_pln:");
    }
  }

      // UT3D_stru_dump (Typ_PLN, &GL_view_pln,"&new GL_view_pln:");

  return 0;

}


//=====================================================================
  int GL_SetConstrPln () {
//=====================================================================
// GL_constr_pln setzen aus WC_sur_act+WC_sur_Z;
// (hier ist Z-Wert aufgerechnet !)
// see UCS_Reset
// see AP_Init_planes

// static double old_Z = 0.;

  double  dz;
  Point   pt1;


  // printf("GL_SetConstrPln %f\n",WC_sur_Z);
  // printf("   ED_mode=%d\n",ED_query_mode());
  // UT3D_stru_dump (Typ_PLN, &WC_sur_act, " new GL_constr_pln:");


  GL_constr_pln = WC_sur_act;

/*
  // WC_sur_act setzen
  UI_AP (UI_FuncSet, UID_ouf_coz, (void*)&WC_sur_Z);


  if(fabs(WC_sur_Z) > UT_TOL_pt) {

    UT3D_pt_traptvclen (&pt1, &WC_sur_act.po,  &WC_sur_act.vz, WC_sur_Z);
    // UT3D_stru_dump (Typ_PT, &pt1, "&new po:");
    UT3D_pl_ptpl (&GL_constr_pln, &pt1);

  }
*/


  if(ED_query_mode() != ED_mode_go) DL_Redraw ();

  return 0;

}



//================================================================
  int GL_DefColGet (ColRGB *nxtCol) {
//================================================================
// preset color for surfaces.
// For 'get DefaultColor' use AP_defcol !!

  int i1 = 12;

  // nxtCol->cr = GL_col_tab[i1][0] * 255;
  // nxtCol->cg = GL_col_tab[i1][1] * 255;
  // nxtCol->cb = GL_col_tab[i1][2] * 255;

  *nxtCol = GL_defCol;

  // UT3D_stru_dump (Typ_Color, nxtCol, "ex GL_DefColGet");
  
  return 0;

}


//================================================================
  int GL_DefColSet (ColRGB *nxtCol) {
//================================================================
// Redefine DefaultColor.
// OFFEN: im DL-Record kann man DB-index mit einer ColorNumber besetzen !
//    Eigentlich sollte man alle Colors als ColRGB anlegen ..
// preset color for surfaces.
// Alle SurfaceDrawFunktionen (APT_DrawSur) rufen mit Attribut AP_indCol.
// AP_indCol ist ein DL-Index auf das glColor3ubv.
// Der Defaultwert von AP_indCol ist 5.
// Surfaces werden mit att=5 gerufen; att 5 benutzt color 12.


  int    i1;
  // float  f1;
  long   ind;
  GLuint dlInd;


  // UT3D_stru_dump (Typ_Color, nxtCol, "GL_DefColSet");

  // if(!strncmp((void*)&actCol,(void*)nxtCol,3)) return 0;

  GL_defCol = *nxtCol;

  return 12;

/* dzt nicht mehr notwendig (zumindest f VRML2)
  ind = DL_StoreObj (Typ_Color, 0L, 0);

  DL_unvis_set (ind, 1);                 // is unvisible
  DL_pick_set  (ind, ON);                // set to nopick

  dlInd = GL_fix_DL_ind (&ind);


  glNewList (dlInd, GL_COMPILE);
  glEnable (GL_LIGHTING);
  glColor3ubv ((unsigned char*)&GL_defCol);
  glEndList ();
*/

/*
  // overwrite Surface-defaultColor # 12
  i1 = 12;
  f1 = nxtCol->cr / 255.;
  GL_col_tab[i1][0] = f1;
  f1 = nxtCol->cg / 255.;
  GL_col_tab[i1][1] = f1;
  f1 = nxtCol->cb / 255.;
  GL_col_tab[i1][2] = f1;


  // overwrite Surface-defaultattributes
  // GL_InitNewAtt (0, 5);

  // change List # 5
  glNewList ((GLuint)DL_base_LnAtt+5, GL_COMPILE);
  glColor3fv    (GL_col_tab[12]);
  glEndList ();
*/

  // printf("ex GL_DefColSet %d\n",dlInd-DL_base_LnAtt);
  return dlInd-DL_base_LnAtt;

}

/*
//================================================================
  int GL_SetCol () {
//================================================================
// activate color for surfaces.

  UT3D_stru_dump (Typ_Color, &newCol, "GL_SetCol");

  if(!strncmp((void*)&actCol,(void*)&newCol,3)) return 0;

  actCol = newCol;

  UT3D_stru_dump (Typ_Color, &newCol, "  activate color: ");
  glColor3ub (newCol.cr, newCol.cg, newCol.cb); // glColor3ubv

  return 0;

}
*/

//=====================================================================
  Point GL_GetViewPt (Point *pti) {
//=====================================================================
// den Punkt pti entlang der Sichtlinie (GL_view_vz) auf die aktive ViewPlane
// projizieren und diesen Punkt retournieren.

  Point pto;

  // printf("GL_GetViewPt: %f,%f,%f\n",pti->x,pti->y,pti->z);
  // UT3D_stru_dump (Typ_PLN, &WC_sur_act, "&WC_sur_act:");
  // UT3D_stru_dump (Typ_PLN, &GL_view_pln, "&GL_view_pln:");


  // Linie pt2 / GL_view_vz mit der Userebene schneiden
  // UT3D_pt_intptvcpl_ (&pto, &GL_view_pln, pti, &GL_view_vz);
  UT3D_pt_intptvcpl_ (&pto, &GL_view_pln, pti, &GL_eyeX);

  // printf("ex GL_GetViewPt: %f,%f,%f\n",pto.x,pto.y,pto.z);

  return pto;

}


//=====================================================================
  int GL_GetCurPosSc (int *xpos, int *ypos) {
//=====================================================================
// curpos in screenkoords retournieren

  *xpos = GL_mouse_x_act;
  // *ypos = GL_Scr_Siz_Y - GL_mouse_y_act;
  *ypos = GL_Viewp[3] - GL_mouse_y_act;

  // printf("GL_GetCurPosSc %d %d / %f %f \n",*xpos,*ypos,
                         // GL_Scr_Siz_X,GL_Scr_Siz_Y);

  return 0;

}


//================================================================
  int GL_GetEyeLn (Point *curPos, Vector *eyeVec) {
//================================================================
// GL_GetEyeLn           Sichtstrahl (Point & Vector) retournieren

  *curPos = GL_actUsrPos;  // curPos on viewPlane

  *eyeVec = GL_eyeX;

    // UT3D_stru_dump (Typ_PT, curPos, "  curPos: ");
    // UT3D_stru_dump (Typ_VC, eyeVec, "  eyeVec: ");

  return 0;

}


//================================================================
  Vector GL_GetEyeX () {
//================================================================
// GL_GetEyeX            Sichtstrahl retournieren
// GL_eyeX zeigt zum Auge.

  return GL_eyeX;

}


//================================================================
  int GL_set_viewPlnPos () {
//================================================================
/// \code
/// GL_set_viewPlnPos     save screenCoords & userCoords of selectionPoint
///   on viewPlane
/// \endcode

// in UI_GR_MotionNotify wird GR_CurUk via GL_GetConstrPos gesetzt; 

  // Screenkoords des indizierten Punktes
  GL_actScrPos.x = GL_mouse_x_act;
  GL_actScrPos.y = GL_Scr_Siz_Y - GL_mouse_y_act;
  GL_actScrPos.z = 0.0;

  GL_actUsrPos = GL_GetViewPos ();

  return 0;

}

//================================================================
  int GL_GetActSelPos (Point *posUsr, Point *posScr) {
//================================================================
/// \code
/// GL_GetActSelPos     Coords of last selectionPoint on viewPlane
///   posUsr     userCoords
///   posScr     screenCoords
/// see also sele_get_pos GL_MousePos
/// \endcode

  // printf("GL_GetActSelPos:\n");
  // UT3D_stru_dump(Typ_PT, &GL_actUsrPos, "posUsr:");
  // UT3D_stru_dump(Typ_PT, &GL_actScrPos, "posScr:");

  *posUsr = GL_actUsrPos;
  *posScr = GL_actScrPos;

  return 0;

}


//================================================================
  Point GL_GetCurPos () {
//================================================================
/// compute cursorPosition in userCoords


  GLdouble   mx, my ,mz;
  Point      pt1;

  // printf("GL_GetCurPos %d %d\n",GL_mouse_x_act,GL_mouse_y_act);


  // Screenkoords des indizierten Punktes
  mx = GL_mouse_x_act;
  my = GL_Scr_Siz_Y - GL_mouse_y_act;
  mz = 0.0;


  // Userkoords
  GL_Sk2Uk (&pt1.x, &pt1.y, &pt1.z, mx, my, mz);
    // printf ("  Uk=%f %f %f\n",pt1.x,pt1.y,pt1.z);


  return pt1;

}


//================================================================
  Point GL_GetViewPos () {
//================================================================
/// \code
/// compute cursorPosition in userCoords on GL_view_pln.
/// GL_view_pln is the displayPlane; always parallel to X-Y.
/// \endcode

// Curpos in userkoord. retournieren
// Input: GL_mouse_x_act, GL_mouse_y_act.
// es wird eine Line durch die aktuelle Mauspsition,
// normal zur Sichtebene GL_view_pln, (= Vektor GL_eyeX) 
// geschnitten mit der Sichtebene GL_view_pln.

// GL_view_pln
// GL_GetConstrPos tut das gleiche fuer GL_constr_pln
// see also UI_GR_Indicate - hat UT3D_pt_traptm3 (.. WC_sur_imat ..) ??
// in UI_GR_MotionNotify wird GR_CurUk via GL_GetConstrPos gesetzt; 

  int      irc;
  GLdouble mx, my ,mz;
  Point    pt1, pt2;
  // Vector   vc1;


  // printf("GL_GetViewPos\n");
  // UT3D_stru_dump (Typ_PLN, &GL_view_pln, " GL_view_pln:");


  //TX_Print ("GL_GetViewPos %d %d",GL_mouse_x_act,GL_mouse_y_act);
    // UT3D_stru_dump (Typ_PT, &GL_actUsrPos, "ViewPos: ");
    // return GL_actUsrPos;


  // get Userkoords of mousepos
  pt1 = GL_GetCurPos ();
    // printf(" _GetViewPos-pt1 = %lf %lf %lf\n",pt1.x,pt1.y,pt1.z);

/*
  // Screenkoords des indizierten Punktes
  mx = GL_mouse_x_act;
  my = GL_Scr_Siz_Y - GL_mouse_y_act;
  mz = 0.0;
  // printf("    GL_GetViewPos scrPos %f %f %f\n",mx,my,mz);


  // Userkoords
  GL_Sk2Uk (&pt1.x, &pt1.y, &pt1.z, mx, my, mz);
    // printf("    GL_GetViewPos pt1 %f %f %f\n",pt1.x, pt1.y, pt1.z);
*/


// see GL_GetConstrPos ?
  if(GL_actView == FUNC_ViewFront)  {
    pt1.y = 0;
    UT3D_pt_projptpl (&pt2, &GL_view_pln, &pt1);
    pt2.y = pt1.z;


  } else if(GL_actView == FUNC_ViewSide) {
    pt1.x = 0;
    UT3D_pt_projptpl (&pt2, &GL_view_pln, &pt1);
    pt2.x = pt1.z;   // 2017-04-12


  } else {
    // FUNC_ViewTop FUNC_ViewIso
    // Linie pt2 / GL_eyeX mit der Userebene schneiden
    // irc = UT3D_pt_intptvcpl_ (&pt2, &GL_constr_pln, &pt1, &GL_eyeX);
    irc = UT3D_pt_intptvcpl_ (&pt2, &GL_view_pln, &pt1, &GL_eyeX);
    if(irc == 0) UT3D_pt_projptpl (&pt2, &GL_view_pln, &pt1);
  }


    // printf(" ex GL_GetViewPos %f %f %f\n",pt2.x, pt2.y, pt2.z);

  return pt2;

}


//=====================================================================
  Point GL_GetConstrPos (Point *pt1) {
//=====================================================================
/// \code
/// compute intersectionpoint of pt1 along eyeVector on constructionPlane
///   eyeVector = GL_eyeX; constructionPlane = GL_constr_pln
/// returns:  point in worldCoords on constructionPlane 
/// \endcode

// PROBLEM:
// if(GL_actView == FUNC_ViewFront) OR if(GL_actView == FUNC_ViewSide) then
//   intersectionpoint is too far;
//   cannot be displayed in label cursor-position (UI_curPos)
// TODO: cannot change constructionPlane to normal-plane; in CAD some points
//       are relativ to constructionPlane. .. ??
//
//   from GL_mouse_x_act,GL_mouse_y_act
// es wird eine Line durch die aktuelle Mausposition,
// normal zur Konstr.ebene GL_constr_pln, (= Vektor GL_eyeX)
// geschnitten mit der Konstr.ebene GL_constr_pln.

// see also GL_GetViewPos (for GL_view_pln)
//   GR_actPos


  int      irc;
  // GLdouble mx, my ,mz;
  // Point    pt1;
  Point    pt2;


  // printf("GL_GetConstrPos %f %f %f\n",pt1->x,pt1->y,pt1->z);
  // UT3D_stru_dump (Typ_PLN, &GL_constr_pln, "&GL_constr_pln:");
  // UT3D_stru_dump (Typ_PLN, &GL_view_pln, "&GL_view_pln:");


  // printf ("GL_GetConstrPos %d %d\n",GL_mouse_x_act,GL_mouse_y_act);
  // UT3D_stru_dump (Typ_VC, &GL_eyeX, "GL_eyeX");


  if(GL_actView == FUNC_ViewFront)  {
    pt1->y = 0;
    UT3D_pt_projptpl (&pt2, &GL_constr_pln, pt1);


  } else if(GL_actView == FUNC_ViewSide) {
    pt1->x = 0;
    UT3D_pt_projptpl (&pt2, &GL_constr_pln, pt1);


  } else {
    // Linie pt2 / GL_eyeX mit der Userebene schneiden
    irc = UT3D_pt_intptvcpl_ (&pt2, &GL_constr_pln, pt1, &GL_eyeX);
    if(irc == 0) UT3D_pt_projptpl (&pt2, &GL_constr_pln, pt1);
  }


  // UT3D_stru_dump (Typ_PT,  &pt1, "PT:");
  // UT3D_stru_dump (Typ_VC,  &GL_eyeX, "VC:");
  // UT3D_stru_dump (Typ_PLN, &GL_constr_pln, "GL_constr_pln:");

  // printf(" ex GL_GetConstrPos %f %f %f\n",pt2.x, pt2.y, pt2.z);

  return pt2;

}


//================================================================
  void GL_Do_Idle (int *dx, int *dy, int x, int y) {
//================================================================
// returns dx,dy = relative movment of mouse
//   dx   positive values go to right side
//   dy   positive values go up
// 0,0 ist links oben.
// also used by UI_GR_MotionNotify  


  // printf("GL_Do_Idle %d %d | %f %f\n",x,y,GL_Scr_Siz_X,GL_Scr_Siz_Y);
  // printf("           %d %d\n",x,(int)GL_Scr_Siz_Y-y);

  *dx = x - GL_mouse_x_act;   // nach links wirds kleiner.
  *dy = GL_mouse_y_act - y;


  GL_mouse_x_act = x;
  GL_mouse_y_act = y;


}


//=====================================================================
  Point GL_GetCen () {
//=====================================================================
/// screencenter in userCoords (returns GL_cen)
/// see also GL_GetViewSizU

  Point pt1;

  pt1.x = GL_cen.x;
  pt1.y = GL_cen.y;
  pt1.z = GL_cen.z;

  // printf(" ex GL_Get_Cen %f %f %f\n",pt1.x, pt1.y, pt1.z);
  return pt1;

}


//================================================================
  void GL_GetViewSizU (double *sx,  double *sy) {
//================================================================
/// return size of graficWindow in userCoords
/// see also GL_GetCen
 
  double   dy;

  // printf(" uSiz %f %f\n",GL_Siz_X,GL_Siz_Y);
  // printf(" sSiz %f %f\n",GL_Scr_Siz_X,GL_Scr_Siz_Y);
  // printf(" scal %f\n",GL_Scale);

  // LO & RU
    // GL_Sk2Uk (&dx1, &dy1, &d3, 0., 0., 0.);  // left lower corner !
    // GL_Sk2Uk (&dx1, &dy1, &d3, GL_Scr_Siz_X, GL_Scr_Siz_Y, 0.); // right upper


  AP_Get_scale (&dy); // get 
  *sy = dy * 2.;
  *sx = *sy * GL_Svfakt;

    // printf(" GL_GetViewSizU %f %f\n",*sx,*sy);


}


//=====================================================================
  void GL_Do_CenRot (double zval) {
//=====================================================================
// Redefinition der GL_cen.z !!



  int    mx,my,mz;
  Point  pt1, pt2;


  // printf("GL_Do_CenRot %f\n",zval);


  // der Screenmittelpunkt in alten Userkoords


  //GL_sur_act = WC_ask_Zsur2 ();  
  GL_cen.z = zval;  



  GL_SetViewPln ();      // nun die ViewPlane korrigieren


/*
  // der Screenmittelpunkt in creen koords
  mx = GL_Scr_Siz_X / 2.;
  my = GL_Scr_Siz_Y / 2.;
  mz = 0.0;
  // der Screenmittelpunkt in neuen UserKoords
  GL_Sk2Uk (&pt1.x, &pt1.y, &pt1.z, mx, my, mz);
*/

  pt1.x = GL_cen.x;
  pt1.y = GL_cen.y;
  pt1.z = GL_cen.z;

  // mit der Userebene schneiden
  UT3D_pt_intptvcpl_ (&pt2, &GL_view_pln, &pt1, &GL_eyeX);



  // = neuer Mittelpunkt
  GL_cen.x = pt2.x;
  GL_cen.y = pt2.y;
  // GL_cen.z = pt2.z;    // MUSS gleich sein !!
  // printf("  new cen=%f,%f,%f\n",GL_cen.x, GL_cen.y, GL_cen.z);
  TX_Print("ViewCenter %9.3f,%9.3f,%9.3f\n",GL_cen.x, GL_cen.y, GL_cen.z);

  DL_Set_Cen2D ();  // set ew 2D-screenCenter-position

  // GL_Translate ();
  GL_Redraw ();

  // Redraw plus Darstellung des RotAchsenSys
  // GL_Do_Rot1 ();

  

}


//======================================================================
  void GL_Do_Pan__ (int x, int y) {
//======================================================================

  double   xAbst, yAbst;
  Point    pt1, pt2;


  // printf("GL_Do_Pan__ %d %d actV=%d\n",x,y,GL_actView);



/*
  if(GL_actView == FUNC_ViewFront) {
    yAbst = (y - GL_mouse_y_act) / GL_Scale / GL_Svfakt;  - verzerrt !
    xAbst = (x - GL_mouse_x_act) / GL_Scale / GL_Svfakt;
    printf(" dy=%f GL_cen.z=%f GL_Scale=%f\n",yAbst,GL_cen.z,GL_Scale);
    // um das Display zu verschieben muss man die Viewplane veraendern !
    // GL_cen.z += yAbst;
    // UI_GR_view_set_Z1 (GL_cen.z);
    GL_mouse_x_act = x;
    GL_mouse_y_act = y;
    GL_cen.x -= xAbst;
    GL_cen.z += yAbst;
    goto L_fertig;
  }
*/

  // pt1 = Userkoords of cursor
  pt1 = GL_GetViewPos ();
    // printf(" _pan_-pt1 = %lf %lf %lf\n",pt1.x,pt1.y,pt1.z);


  // neu setzen
  if(x >= 0) {
    GL_mouse_x_act = x;
    GL_mouse_y_act = y;
  }

  // neue Curpos in Userkoords
  pt2 = GL_GetViewPos ();
    // printf(" _pan_-pt2 = %lf %lf %lf\n",pt2.x,pt2.y,pt2.z);

  xAbst = pt2.x - pt1.x;
  yAbst = pt2.y - pt1.y;
    // printf("   _Pan__-xAbst=%f yAbst=%f\n",xAbst,yAbst);


  // move cen
  if(GL_actView == FUNC_ViewFront) {
    GL_cen.x -= xAbst;
    GL_cen.z -= yAbst;
  
  } else if(GL_actView == FUNC_ViewSide) {
    GL_cen.y -= yAbst;    // horiz;
    GL_cen.z -= xAbst;
  
  } else {
    GL_cen.x -= xAbst;
    GL_cen.y -= yAbst;
  }
    // printf("  GL_cen = %lf,%lf,%lf\n",GL_cen.x, GL_cen.y, GL_cen.z);


  L_fertig:
  DL_Set_Cen2D ();  // set new 2D-screenCenter-position

  ++GL_ViewModTimStamp;    // view modified ..

  // GL_Translate1 ();
  // GL_GetGLMat0 ();

  // L_fertig:
    // printf("    GL_Do_Pan__ %f %f %f\n",GL_cen.x,GL_cen.y,GL_cen.z);
  // GL_Redraw ();

  return;

}

/*
//================================================================
  void GL_Do_Pan (int x, int y) {
//================================================================
// Callback Mousedrag.
//   Wird bei einer Mausbewegung - aber nur mit einem gedruecktem GL_mousebutton
//   gerufen.
// Input is the mouse-position in 2D-screen-koords.


  // int      dx, dy, dz;
  // double   view_cen_SX, view_cen_SY, view_cen_SZ, xAbst, yAbst; 
  double   xAbst, yAbst;
  Point    pt1, pt2;

  // static double xPos=9999., yPos=9999.;


  //----------------------------------------------- 
  // printf("GL_Do_Pan %d %d\n",x,y);


//
  // dx = x - GL_mouse_x_act;
  // dy = GL_mouse_y_act - y;
  // dz = 0;
//

  pt1 = GL_GetViewPos ();
  //TX_Print(" curpos1=%f,%f,%f",pt1.x,pt1.y,pt1.z);


  GL_mouse_x_act = x;
  GL_mouse_y_act = y;



  //TX_Print ("GL_Do_Pan abs=%d,%d rel=%d,%d",x,y,dx,dy);



  // ------------------------------------------------
  // // Pan. Compute new transl.Center.
    // // Transl.Cen. Uk > Sk
    // GL_Uk2Sk (&view_cen_SX, &view_cen_SY, &view_cen_SZ,
              // GL_cen_X,   GL_cen_Y,   GL_cen_Z);
// 
    // // add the sk-Offset
    // view_cen_SX = view_cen_SX - dx;
    // view_cen_SY = view_cen_SY - dy;
    // view_cen_SZ = view_cen_SZ - dz;
// 
    // // Transl.Cen. Sk > Uk
    // GL_Sk2Uk (&GL_cen_X, &GL_cen_Y, &GL_cen_Z,
              // view_cen_SX, view_cen_SY, view_cen_SZ);







  pt2 = GL_GetViewPos ();
  //TX_Print(" curpos2=%f,%f,%f",pt2.x,pt2.y,pt2.z);


  // xAbst = pt2.x - xPos;
  // yAbst = pt2.y - yPos;


  xAbst = pt2.x - pt1.x;
  yAbst = pt2.y - pt1.y;

  //TX_Print("   xAbst, yAbst=%f,%f",xAbst,yAbst);


  //if((fabs(xAbst) < 10.)&&(fabs(yAbst) < 10.)) {

    GL_cen.x -= xAbst;
    GL_cen.y -= yAbst;
    //GL_cen_Z = pt1.z;
  //}

  // xPos = pt2.x;
  // yPos = pt2.y;



  // printf("GL_Do_Pan cen=%f,%f,%f\n",GL_cen_X, GL_cen_Y, GL_cen_Z);




  //GL_Reframe ();

  GL_Translate ();

  //GR_Recreate ();

  GL_Redraw ();

  //UI_GR_DrawExit ();

}


//================================================================
  void GL_Do_Rot (int x, int y) {
//================================================================
// Callback Mousedrag.
  // Wird bei einer Mausbewegung - aber nur mit einem gedr³cktem GL_mousebutton
  // gerufen.
// Input is the transl in screen-koords.


  int    dx, dy;
  double d1, d2;


  dx = x - GL_mouse_x_act;
  dy = GL_mouse_y_act - y;

  GL_mouse_x_act = x;
  GL_mouse_y_act = y;


  //TX_Print ("GL_Do_Rot %d %d / %d %d",x,y,dx,dy);




  // Rotate
  d1 = dy / 5.0;
  d2 = dx / 5.0;


    // GL_ang_X = GL_ang_X - d1;
    // GL_ang_Y = GL_ang_Y + d2;
    // GL_ang_Z = GL_ang_Z + d2;
 
    // if(GL_ang_X < 0.)   {GL_ang_X = GL_ang_X + 360.;}
    // if(GL_ang_X > 360.) {GL_ang_X = GL_ang_X - 360.;}
    // if(GL_ang_Y < 0.)   {GL_ang_Y = GL_ang_Y + 360.;}
    // if(GL_ang_Y > 360.) {GL_ang_Y = GL_ang_Y - 360.;}
    // if(GL_ang_Z < 0.)   {GL_ang_Z = GL_ang_Z + 360.;}
    // if(GL_ang_Z > 360.) {GL_ang_Z = GL_ang_Z - 360.;}

  // Redraw plus Darstellung des RotAchsenSys
  GL_Do_Rot1 ();

}
*/


//======================================================================
  void GL_Do_Rot__ (int x, int y) {
//======================================================================
// rotate view.
// Input:
//   x,y     the mouseposition


  int    dx, dy, zFlag;
  double d1, d2, da;
  Vector vc1;


  // printf("GL_Do_Rot__ %d %d\n",x,y);
  // UT3D_stru_dump (Typ_VC, &GL_eyeX, "GL_eyeX: ");


  // get relative movement of mouse from last stored position
  GL_Do_Idle (&dx, &dy, x, y);


  /* ------------------------------------------------ */
  /* Rotate */
  d1 = dy / 5.0;
  d2 = dx / 5.0;

/*
    GL_ang_X = GL_ang_X - d1;
    //GL_ang_Y = GL_ang_Y + d2;
    GL_ang_Z = GL_ang_Z + d2;

    if(GL_ang_X < 0.)   {GL_ang_X = GL_ang_X + 360.;}
    if(GL_ang_X > 360.) {GL_ang_X = GL_ang_X - 360.;}

    if(GL_ang_Y < 0.)   {GL_ang_Y = GL_ang_Y + 360.;}
    if(GL_ang_Y > 360.) {GL_ang_Y = GL_ang_Y - 360.;}

    if(GL_ang_Z < 0.)   {GL_ang_Z = GL_ang_Z + 360.;}
    if(GL_ang_Z > 360.) {GL_ang_Z = GL_ang_Z - 360.;}
*/

  // printf("GL_Do_Rot__ %f %f %f\n",GL_ang_X,GL_ang_Y,GL_ang_Z);
  // printf("GL_Do_Rot__ %f %f\n",d2,d1);

  // d2 = 0.;


  //================================================================
  // rotate the view-refSys GL_eyeX-GL_eyeY-GL_eyeZ
  //
  // wenn der Sichtvektor GL_eyeX ident mit Z-Vektor:
  // den GL_eyeZ um den GL_eyeX drehen
  zFlag = UT3D_comp2vc_d(&GL_eyeX, (Vector*)&UT3D_VECTOR_Z, UT_TOL_Ang2);
  if(zFlag == 1) {
    // only for top-view: keep GL_eyeX; rotate GL_eyeZ.
    UT3D_vc_rotvcangr (&GL_eyeZ, UT_RADIANS(d2), &GL_eyeZ, &GL_eyeX);
    goto L_kippen;
  }


  // arbitrary view: rotate GL_eyeX & GL_eyeZ around UT3D_VECTOR_Z
  // UT3D_stru_dump (Typ_VC,  &GL_eyeX, " old eyeX:");
  UT3D_vc_rotvcangr (&GL_eyeX, UT_RADIANS(d2), &GL_eyeX, &UT3D_VECTOR_Z);
  UT3D_vc_rotvcangr (&GL_eyeZ, UT_RADIANS(d2), &GL_eyeZ, &UT3D_VECTOR_Z);



  //--------------------------------------------------------------
  // nun um d1 kippen;
  // einen Normalvektor auf GL_eyeX = vcn errechen;
  // um diesen Normalvektor vcn kippen;
  // GL_eyeZ wieder als Normalvektor von GL_eyeX und vcn errechnen
  L_kippen:

  // get vcn = normalvector (negativ horizontal)
  UT3D_vc_perp2vc (&GL_eyeY, &GL_eyeX, &GL_eyeZ);
  UT3D_vc_setLength (&GL_eyeY, &GL_eyeY, 1.);
  // UT3D_stru_dump (Typ_VC,  &GL_eyeX, " old eyeX:");
  // UT3D_stru_dump (Typ_VC,  &GL_eyeZ, " old eyeZ:");


  // arbitrary view:
  // rotate GL_eyeX & GL_eyeZ around GL_eyeY
  UT3D_vc_rotvcangr (&GL_eyeX, UT_RADIANS(d1), &GL_eyeX, &GL_eyeY);
  UT3D_vc_rotvcangr (&GL_eyeZ, UT_RADIANS(d1), &GL_eyeZ, &GL_eyeY);


  // GL_eyeZ normal auf GL_eyeX machen (makes more precise)
  UT3D_vc_setLength (&GL_eyeX, &GL_eyeX, 1.);
    // UT3D_stru_dump (Typ_VC,  &GL_eyeX, " eyeX:");
  // recreate y from x and z
  UT3D_vc_perp2vc (&GL_eyeY, &GL_eyeZ, &GL_eyeX);
  // recreate z from x and y
  UT3D_vc_perp2vc (&GL_eyeZ, &GL_eyeX, &GL_eyeY);


  
  //================================================================
  // check if View is top, front, side or isoview. Set GL_actView.
  // top
  if(UT3D_comp2vc_d(&GL_eyeX, (Vector*)&UT3D_VECTOR_Z, UT_TOL_min1) == 1) {
    GL_actView = FUNC_ViewTop;
    UI_VW_upd (GL_actView);

  // front
  } else if(UT3D_comp2vc_d(&GL_eyeX, (Vector*)&UT3D_VECTOR_Y, UT_TOL_min1) == 1) {
    GL_actView = FUNC_ViewFront;
    UI_VW_upd (GL_actView);

  // side
  } else if(UT3D_comp2vc_d(&GL_eyeX, (Vector*)&UT3D_VECTOR_X, UT_TOL_min1) == 1) {
    GL_actView = FUNC_ViewSide;
    UI_VW_upd (GL_actView);

  } else {
    GL_actView = FUNC_ViewIso;
    UI_VW_upd (GL_actView);
  }


  ++GL_ViewModTimStamp;    // view modified ..

  DL_Set_Scl2D ();   // set horizontal-vector
  // GL_Redraw ();

  return;

}

/*
//=====================================================================
  void GL_Do_Rot1 () {
//=====================================================================
// Redraw plus Darstellung des RotAchsenSys


  GL_Translate ();

  GL_Func_Act = FUNC_Rot; // damit RotCen gezeichnet wird

  GL_Redraw ();


}
*/

//======================================================================
  void GL_Do_Scale__ (int x, int y) {
//======================================================================
/// modify scale according to mousemovement

  int    dx, dy;
  double zoomfakt;

  // printf("GL_Do_Scale__ %d %d\n",x,y);


  //----------------------------------------------------------------


  // autoscale (from mouse-scroll)
  if(x == UT_INT_MAX) {
    if(y > 0) zoomfakt = 1.2;
    else      zoomfakt = 0.8;
    goto L_work1;

  } else {  // 2015-07-05
    // scale from mouse-movement    
    GL_Do_Idle (&dx, &dy, x, y);  // get relative movment of mouse
    dx = -dx;

  }



  //----------------------------------------------------------------
  zoomfakt = (dx + dy) / 250.0;

  if(zoomfakt > 0.5) zoomfakt = 0.5;
  if(zoomfakt < -0.5) zoomfakt = -0.5;

  zoomfakt = (1.0 - zoomfakt);
  // printf("GL_Do_Scale__ %f\n",zoomfakt);


  // den Sichtvektor um zoomfakt verkuerzen/verlaengern
// removed 2011-10-23
  // GL_eyeX.dx *= zoomfakt;
  // GL_eyeX.dy *= zoomfakt;
  // GL_eyeX.dz *= zoomfakt;
  // UT3D_stru_dump (Typ_VC, &GL_eyeX, " new eyeX:");




  //----------------------------------------------------------------
  L_work1:

  ++GL_ViewModTimStamp;    // view modified ..

  GL_Set_Scale (GL_Scale * zoomfakt);

  return;

}


/*
//================================================================
  void GL_Do_Scale (int x, int y) {
//================================================================
// 3,4 = Vergrossern, verkleinern.

  int    dx, dy;
  double zoomfakt, Scale;


  //dx = x - GL_mouse_x_act; // nach links wirds grösser
  dx = GL_mouse_x_act - x;   // nach links wirds kleiner.
  dy = GL_mouse_y_act - y;


  GL_mouse_x_act = x;
  GL_mouse_y_act = y;



  zoomfakt = (dx + dy) / 250.0;
    
  if(zoomfakt > 0.5) zoomfakt = 0.5;
  if(zoomfakt < -0.5) zoomfakt = -0.5;

  Scale  = GL_Scale * (1.0 - zoomfakt);


  GL_Set_Scale (Scale);


}
*/

//=================================================================
  int GL_View_get         (double view[]) {
//=================================================================
/// \code
/// get viewport (10 doubles)
///    0,1,2         GL_eyeX
///    3,4,5         GL_cen
///    6             GL_Scale
///    7,8,9         GL_eyeZ
///
/// GL_eyeX-Vektor: zeigt zum Auge des Betrachters;
///                 ist im Display also immer nur ein Punkt
/// GL_eyeZ-Vektor: zeigt im Display ganz genau nach oben (vertikal);
///                 ist normal auf eyeX-Vektor.
///
/// see GL_View_set GL_View_dump
/// \endcode


// UT3D_3angr_2vc

  // double a1, a2, a3;


  view[0] = GL_eyeX.dx;
  view[1] = GL_eyeX.dy;
  view[2] = GL_eyeX.dz;

  // UT3D_3angr_2vc (&a1, &a2, &a3, &GL_eyeX, &GL_eyeZ);
  // view[0] = UT_DEGREES(a1);
  // view[1] = UT_DEGREES(a2);
  // view[2] = UT_DEGREES(a3);
  // printf("GL_View_get %f %f %f\n",UT_DEGREES(a1),UT_DEGREES(a2),UT_DEGREES(a3));

  view[3] = GL_cen.x;
  view[4] = GL_cen.y;
  view[5] = GL_cen.z;

  view[6] = GL_Scale;

  view[7] = GL_eyeZ.dx;
  view[8] = GL_eyeZ.dy;
  view[9] = GL_eyeZ.dz;

  return 0;

}


//================================================================
  int GL_View_dump () {
//================================================================
// setzen: GL_View_set
// aktivieren: GL_Redraw (glScaled, gluLookAt)
// see also GL_View_get GL_View_set


  printf("GL_View_dump \n");

  UT3D_stru_dump (Typ_PT, &GL_cen, "_cen: "); 
  UT3D_stru_dump (Typ_PT, &GL_eyeX, "eyeX: "); 
  UT3D_stru_dump (Typ_PT, &GL_eyeZ, "eyeZ: "); 

  printf("_scl: %f\n",GL_Scale);



  return 0;

}
 

//=================================================================
  int GL_View_set         (double *view) {
//=================================================================
// braucht nachfolgend GL_Redraw od GR_Redraw
// Input:
//   view          view from 10 numbers;
//        0/1/2    VecX
//        3/4/5    Cen
//        6        Scale
//        7/8/9    VecZ


// see GL_View_get GL_View_dump

  int    i1;
  double a1, a2, a3;


  // printf("GL_View_set \n");
  // printf("   ED_mode=%d\n",ED_query_mode());
  // for(i1=0; i1<10; ++i1) printf(" %d=%f\n",i1,view[i1]);


  GL_eyeX.dx = view[0];
  GL_eyeX.dy = view[1];
  GL_eyeX.dz = view[2];
  if(UT3D_compvc0(&GL_eyeX, 0.1) == 1) GL_eyeX = UT3D_VECTOR_X;
  else UT3D_vc_setLength (&GL_eyeX, &GL_eyeX, 1.);
  

  GL_cen.x = view[3];
  GL_cen.y = view[4];
  GL_cen.z = view[5];

  GL_eyeZ.dx = view[7];
  GL_eyeZ.dy = view[8];
  GL_eyeZ.dz = view[9];
  if(UT3D_compvc0(&GL_eyeX, 0.1) == 1) GL_eyeZ = UT3D_VECTOR_Z;
  else UT3D_vc_setLength (&GL_eyeZ, &GL_eyeZ, 1.);

    // UT3D_stru_dump (Typ_VC, &GL_eyeZ, "  GL_eyeZ/2:");


  // recreate GL_eyeY
  UT3D_vc_perp2vc (&GL_eyeY, &GL_eyeZ, &GL_eyeX);

  GL_SetViewPln ();

  UI_GR_DrawInit ();
  GL_Set_Scale (view[6]);     // MAKES A REDRAW !
  UI_GR_DrawExit ();

  APT_set_view_stat ();       // do not autoScale ..

  return 0;

}


//=================================================================
  void GL_Set_Scale (double Scale) {
//=================================================================
// Set GL_Scale GL_Scale_back GL2D_Scale AP_scale

// GL_Scale depends on GL_ModSiz;
// GL2D_Scale not !

// User-settings: GL_ModSiz, GL_Scale.
// GL_Scale_back = 1. / GL_Scale;
// AP_scale = GL_ModSiz / GL_Scale;
// GL_ModScale = 0.2 * GL_ModSiz;
// GL2D_Scale = GL_ModSiz / (GL_Scale * 500.);
// GL2D_Scale = GL_ModScale / (GL_Scale * 100.);

// values for GL_ModSiz, GL_ModScale:
// GL_ModSiz       500 100   5   1
// GL_ModScale     100  20   1   0.2

// see also GL_SclNorm


  float f_scl;


  // printf("SSSSSSSSSS GL_Set_Scale %f\n",Scale);


  GL_Scale = DLIM2 (Scale, SCALE_MIN, SCALE_MAX);  // check min - max
    // printf(" GL_Scale=%lf\n",GL_Scale);

  GL_Scale_back = 1. / GL_Scale;          // soll immer gleich grosz sein
    // printf(" GL_Scale_back=%lf\n",GL_Scale_back);

  // GL2D_Scale = GL_ModScale / (GL_Scale * 100.);
  GL2D_Scale = GL_ModSiz / (GL_Scale * 500.);
    // printf(" GL2D_Scale=%lf\n",GL2D_Scale);

  AP_Set_scale (GL_ModSiz / GL_Scale);  // = usiz=hor siz in userCoords



  // die Ruecktransformation in DL_Ind_ScBack speichern
  GL_ScalBack (GL_Scale_back);


  // die Ruecktransformation fuer Images
  f_scl = GL_Scale;
  glNewList (DL_Img_ScBack, GL_COMPILE);
    // glPushMatrix ();
    glPixelZoom ((GLfloat)f_scl, (GLfloat)f_scl);
  glEndList ();


  GL_Translate ();


  // if(ED_query_mode() != ED_mode_go)
  GL_Redraw ();



}



//***********************************************************************
  void GL_Reshape (int width, int height) {
//***********************************************************************
//
// gcad-Name:GL_Do_Reshape.
// Wird bei einem (manuell initierten) Rescale des Window gerufen.
//

  // printf("GL_Reshape %d %d\n",width,height);


  if(width > 0) {
    GL_Scr_Siz_X = width;
    GL_Scr_Siz_Y = height;
  }

  GL_Resize (GL_Scr_Siz_X, GL_Scr_Siz_Y);

  GL_Translate ();  // dzt unused

}



//=======================================================================
  int GL_Get_Cen (Point *cen) {
//=======================================================================
/// screencenter in userCoords

  *cen = GL_cen;

  return 0;

}


//=======================================================================
 int GL_Set_Cen (Point *Ucen) {
//=======================================================================

  // UT3D_stru_dump (Typ_PT, Ucen, "GL_Set_Cen:");
  // UT3D_stru_dump (Typ_PT, &GL_cen, "   old cen:");


  GL_cen.x = Ucen->x;
  GL_cen.y = Ucen->y;
  GL_cen.z = Ucen->z;

  // GL_sur_act = GL_cen_Z;


  GL_Translate ();

  DL_Redraw ();

  return 0;
}


//================================================================
  int GL_Rescal1 (Point *pb1, Point *pb2, float *fa) {
//================================================================

  double  sx,sy,sz;
  Point   pt1;


  // if(fabs(fa[2]) > 1.) printf(" >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
  // printf("GL_Rescal1 %f %f %f\n",fa[0],fa[1],fa[2]);


  // sx = fa[0];
  // sy = fa[1];
  // sz = fa[2];
  // GL_Sk2Uk (&pt1.x,&pt1.y,&pt1.z,  sx,sy,sz);

  pt1.x = fa[0];
  pt1.y = fa[1];
  pt1.z = fa[2];
    // UT3D_stru_dump (Typ_PT, &pt1, "GL_Rescal1:");
    // if(fabs(pt1.z) > 1.) printf(" >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");


  UT3D_box_extend (pb1, pb2, &pt1);


  // if(fabs(pb1->z) > 1000.) exit(0);
  // if(fabs(pb2->z) > 1000.) exit(0);




  return 0;

}


//================================================================
  int GL_Rescal0 (Point *pb1, Point *pb2) {
//================================================================
// pb1,pb2 = eine Box um alle Objekte in Userkoords errechnen
// beim Rescale duerfen Obj, die skaliert werden, NICHT gezeichnet werden:
//  Planes, Normierte Vektoren ..
// RetCodes:  0=OK; -1=no useful obj; -2=bufferEmpty.
//    



  int      fSiz, i1, count, token, nvertices, vNr;
  float    *fBuf;
  double   sx,sy,sz;
  Point    pt1, pt2;


  printf("GL_Rescal0 0000000000000000000000000\n");
  UT3D_stru_dump (Typ_PT, pb1, "pb1");
  UT3D_stru_dump (Typ_PT, pb2, "pb2");


  count = 0;
  vNr   = 0;


  // get FeedbackBuffer
  if(GL_FeedGet (&fSiz, &fBuf, GL_3D) < 0) goto L_free;
  // if(GL_FeedGet (&fSiz, &fBuf, GL_3D_COLOR) < 0) goto L_free;
  if(fSiz < 6) {
    free(fBuf);
    return -2;
  }


  printf(" GL-fSiz=%d\n",fSiz);
    // 2 Punkte haben ein fSiz von 10 !



  NextRec:
  token = fBuf[count];
  // printf("........... next: %d [%d]\n",token,count),
  ++count;

      //===================================================================
      if (token == GL_PASS_THROUGH_TOKEN) {
         // printf ("%d GL_PASS_THROUGH_TOKEN %f\n",count,buffer[count]);
         ++count;


      //===================================================================
      } else if (token == GL_POINT_TOKEN) {
         // printf ("%d GL_POINT_TOKEN\n",count);
         // fprintf(fp1, "PT %f %f %f\n",
                 // buffer[count],buffer[count+1],
                 // (0.5 -  buffer[count+2]) * GL_Z_FAKT);

         GL_Rescal1 (pb1, pb2, &fBuf[count]);
         ++vNr;

         count += 3;


      //===================================================================
      } else if (token == GL_LINE_TOKEN) {
         // printf ("%d GL_LINE_TOKEN\n",count);
         // AP_print_vertex (fpo, &count, &buffer[count]);
         // fprintf(fp1, "LN %f %f %f %f %f %f\n",
                 // buffer[count],buffer[count+1],
                 // (0.5 -  buffer[count+2]) * GL_Z_FAKT,
                 // buffer[count+3],buffer[count+4],
                 // (0.5 -  buffer[count+5]) * GL_Z_FAKT);

         GL_Rescal1 (pb1, pb2, &fBuf[count]);
         GL_Rescal1 (pb1, pb2, &fBuf[count+3]);
         vNr += 2;
         count += 6;


      //===================================================================
      } else if (token == GL_POLYGON_TOKEN) {
         nvertices = fBuf[count];
         // printf ("%d GL_POLYGON_TOKEN: (%d)\n",count,nvertices);
         // if(nvertices != 3) {printf("GL_SelVert_scan E003 %d\n",nvertices);}
         ++count;
         // fprintf(fp2, "PO %f %f %f",
                 // buffer[count],buffer[count+1],
                 // (0.5 -  buffer[count+2]) * GL_Z_FAKT);
         // count += 7;
         count += 3;
         for (i1=1; i1<nvertices; ++i1) {
           // AP_print_vertex(stdout, &count, &fBuf[count]);
           // fprintf(fp2, " %f %f %f",
                   // fBuf[count],fBuf[count+1],
                   // (0.5 -  fBuf[count+2]) * GL_Z_FAKT);
           GL_Rescal1 (pb1, pb2, &fBuf[count]);
           count += 3;
         }
         vNr += nvertices-1;
         // fprintf(fp2, "\n");


      //===================================================================
      } else if (token == GL_LINE_RESET_TOKEN) {
         // printf ("%d GL_LINE_RESET_TOKEN\n",count);
         // fprintf(fp1, "LN %f %f %f %f %f %f\n",
                 // buffer[count],buffer[count+1],
                 // (0.5 -  buffer[count+2]) * GL_Z_FAKT,
                 // buffer[count+3],buffer[count+4],
                 // (0.5 -  buffer[count+5]) * GL_Z_FAKT);
         GL_Rescal1 (pb1, pb2, &fBuf[count]);
         GL_Rescal1 (pb1, pb2, &fBuf[count+3]);
         vNr += 2;
         count += 6;


      //===================================================================
      } else if (token == GL_BITMAP_TOKEN) {
         // printf ("%d GL_BITMAP_TOKEN\n",count);
         count += 3;

      //===================================================================
      } else if (token == GL_DRAW_PIXEL_TOKEN) {
         // printf ("%d GL_DRAW_PIXEL_TOKEN\n",count);
         count += 3;

      //===================================================================
      } else if (token == GL_COPY_PIXEL_TOKEN) {
         // printf ("%d GL_COPY_PIXEL_TOKEN\n",count);
         count += 3;

      //===================================================================
      } else printf("%d **** unknown GL-TOKEN %d ****\n",fSiz-count,token);


  if(count < fSiz) goto NextRec;


  // give Memory back ..
  L_free:
  free(fBuf);

  // UT3D_stru_dump (Typ_PT, pb1, " sc-pb1:");
  // UT3D_stru_dump (Typ_PT, pb2, " sc-pb2:");
  printf(" vNr=%d\n",vNr);

  if(vNr < 1) return -1;

  // GL_dump_Mat0 ();


  sx = pb1->x;
  sy = pb1->y;
  sz = pb1->z;
  GL_Sk2Uk (&pt1.x,&pt1.y,&pt1.z,  sx,sy,sz);

  sx = pb2->x;
  sy = pb2->y;
  sz = pb2->z;
  GL_Sk2Uk (&pt2.x,&pt2.y,&pt2.z,  sx,sy,sz);
  // UT3D_stru_dump (Typ_PT, &pt1, " sc-pt1:");
  // UT3D_stru_dump (Typ_PT, &pt2, " sc-pt2:");


  // make box of 2 points    - die 2 pt's liegen nun ganz verkehrt ..
  UT3D_box_2pt (pb1, pb2, &pt1, &pt2);


  // printf("ex GL_Rescal0\n"); // Usercoords !
  // UT3D_stru_dump (Typ_PT, pb1, " pb1:");
  // UT3D_stru_dump (Typ_PT, pb2, " pb2:");


  return 0;

}


//================================================================
  int GL_FeedGet (int *fsiz, float **feedBuffer, int idim) {
//================================================================
/// \code
/// give back FeedbackBuffer
/// YOU MUST FREE feedBuffer AFTER DECODING !
/// Input:
///   idim         GL_2D | GL_3D | GL_3D_COLOR
/// Output:
///   size         nr of floats in feedBuffer
///   feedBuffer   
/// \endcode


// used by Printmodule


#define DR_BUF_SIZ   1000000

  int      incSiz;
  long     msiz;
  float    f1;

  printf("GL_FeedGet %x\n",idim);


  // glGetIntegerv (GL_FEEDBACK_BUFFER_SIZE, &msiz);
    // printf(" msiz=%d\n",msiz);
  // geht nicht; liefert immer die zuletzt in glFeedbackBuffer uebergebene Size 


  // incSiz = (long)DB_cPos();
  // DB_Query_siz (&incSiz);
  GL_Query_siz (&incSiz);


  // printf(" DB_cPos=%d\n",incSiz);
  // incSiz *= 10;
  if(incSiz < DR_BUF_SIZ) incSiz = DR_BUF_SIZ;


  // Memory holen ..
  msiz = incSiz;
  *feedBuffer = malloc (msiz);
  *fsiz = msiz / sizeof(float);



  L_start:
  // get Feedback from OpenGL
  // GL_mode_feed = 1; // ReScale
  GL_Feedback (fsiz, *feedBuffer, idim);
  // GL_mode_feed = 0;


  if(*fsiz < 0) {

    // msiz += DR_BUF_SIZ;
    msiz += msiz;
      printf("GL_FeedGet REALLOC %ld\n",msiz);

    *feedBuffer = realloc (*feedBuffer, msiz);
    *fsiz = msiz / sizeof(float);

    if(*feedBuffer == NULL) {
      TX_Error ("GL_FeedGet: OUT OF MEMORY");
      return -1;
    }
    goto L_start;
  }


    printf("ex GL_FeedGet %d\n",*fsiz);

  return 0;

}


//================================================================
  void GL_Rescale (double Usiz, Point *Ucen) {
//================================================================
/// \code
/// Input:
///   Usiz      size of window (horiz) in usercoords
///   Ucen      position of center of window in usercoords
/// \endcode


  double   d1;


  printf("GL_Rescale Usiz=%f Ucen=%f,%f,%f\n",Usiz,Ucen->x,Ucen->y,Ucen->z);
  printf("   GL_ModSiz=%f\n",GL_ModSiz);


  // if    (Usiz < 0.000000001) Usiz = 0.000000001;
  d1 = APT_ModSiz / 2000.;  // UT_DISP_cv;    2017-04-10
  if    (Usiz < d1) Usiz = d1;
  else if(Usiz > 100000000.) Usiz =  100000000.;


  // set center of screen in usercoords
  GL_cen = *Ucen;

  //GL_cen_Z = WC_ask_actZsur ();
  // GL_sur_act = GL_cen_Z;




/*
  // Ucen soll der Darstellungsmittelpunkt sein. Weil das dispCen dzt aber
  // doppelt verarbeitet wird, muss manipuliert werden!

  // zuesrt mal errechnen, wo Ucen auf der Screen zu liegen käme; dann die
  // ScreenKoordDifferenz zum aktiven ScreenCen dazuzählen u. retour in UKoords. 

  // das IST-Cen in S-Koord
  GL_Uk2Sk (&s1x,     &s1y,     &s1z,
            GL_cen_X, GL_cen_Y, GL_cen_Z);
  TX_Print(" s1=%f,%f,%f",s1x,s1y,s1z);


  // das Soll-Cen in S-Koord
  GL_Uk2Sk (&s2x,     &s2y,     &s2z,
            Ucen->x, Ucen->y, Ucen->z);
  TX_Print(" s2=%f,%f,%f",s2x,s2y,s2z);
*/






/*
  GL_cen_X = Ucen->x / 2.;
  GL_cen_Y = Ucen->y / 2.;
  GL_cen_Z = Ucen->z / 2.;
  Usiz = Usiz * 2.;
*/




  //TX_Print("GL_Rescale cen=%f,%f,%f siz=%f",GL_cen_X,GL_cen_Y,GL_cen_Z);


  UI_GR_DrawInit ();

  // 0.8 passt ganz genau ins Fenster; damit beim drehen nix zu goss ist ..
  GL_Set_Scale (GL_ModSiz / Usiz * 1.25);
  // GL_Translate ();
  // GL_Redraw ();

  UI_GR_DrawExit ();

    // printf("ex GL_Rescale scal=%f cen=%f,%f,%f\n",Usiz,Ucen->x,Ucen->y,Ucen->z);

}


//=======================================================================
  int GL_fix_DL_base (int mode) {
//=======================================================================

  // printf("GL_fix_DL_base mode=%d %d\n",mode,GR_TAB_IND);


  // DL-Offset ganz an den Beginn (deletes Overlay-models)
  if(mode == 0) {

    DL_base__ = DL_base_mod;
    GL_Init1 ();
    GL_Delete (0L);     // setzt GR_TAB_IND = 0;


  // den momentanen DL-Offset als Startindex fuer die normale Geometrie setzen
  } else {              
    DL_base__ = DL_base_mod + GR_TAB_IND;
  }

  return 0;

}


//================================================================
  GLuint GL_fix_DL_ind (long *ind) {
//================================================================
/// \code
/// GL_fix_DL_ind        make OpenGL-dispListIndex from dli (add DL_base__)
///
/// Input:
///   ind >= 0     normal obj; add DL_base__
///   ind == -1    korrigierten DL-index retournieren (next temp-Nr vergeben)
///   ind <= -2    fixer temp-Index (max -DL_base_font1)
///
/// Output:
///   ind     in >= 0   DL_base__ added ..
///           in == -1  (returns next free (2 - <DL_base_font1 - 1>)
///           in <= -2  *= -1   (returns 2 - <DL_base_font1 - 1>)
///   RetCod:      OpenGL-dispListIndex
///              
///
/// \endcode



  int      i1;
  GLuint   DL_ind;


  // printf("GL_fix_DL_ind %ld\n",*ind);
  // printf(" GL_TAB_SIZ=%d GR_TAB_IND=%d\n",GL_TAB_SIZ,GR_TAB_IND);




  //----------------------------------------------------------------
  // wenn ind < 0: temporaeres Objekt anlegen (DL-Platz 1-31)
  if (*ind < 0) {

/* 
    if(DL_Ind_tmp >= DL_base_font1) {  // temp-space von 1-DL_base_font1
      TX_Print("GL_fix_DL_ind I001 - TempList Overflow ***");
      return -1;
    }
*/
    // -1: naechsten freien index geben
    if(*ind == -1) {
      DL_ind = DL_Ind_tmp;
      // printf(" tmp.ind=%d\n",DL_ind);
      ++DL_Ind_tmp;
      goto Fertig;
    }

    // < -1: fixer index
    DL_ind = -(*ind);
    if(DL_ind >= DL_base_font1) {  // temp-space von 1-DL_base_font1
      TX_Print("GL_fix_DL_ind I002 - TempList Overflow ***");
      return -1;
    }
    // Achtung: nix loeschen !
    i1 = DL_ind + 1;
    if(DL_Ind_tmp < i1) DL_Ind_tmp = i1;
    goto Fertig;
  }




  //----------------------------------------------------------------
  // den Ind merken; fuer Hili zB
  // GL_ActInd = *ind - 1;  // wieso -1 ?
  // GL_ActInd = *ind;      // 2002-02-12
  // wozu war das gut ?
  // printf("GGGGGGGGGGGGGGGG  GL_ActInd = %d\n",*ind);
  // printf(" SET GL_ActInd = %d\n",GL_ActInd);


  // realloc GL_IndTab
  if(*ind > GL_TAB_SIZ) {
    if(GL_alloc__ (*ind) < 0) return -1;
  }

  // diesen nächsten Index vergeben oder merken
  if(*ind > GR_TAB_IND) {
    // DL_Ind_nxt = *ind;
    // wozu war das gut ?
    TX_Print("GL_fix_DL_ind  E001 %d\n",*ind);
    *ind = GR_TAB_IND - 1;
  }

  // DL-record already exists ..
  DL_ind = (GLuint)*ind + DL_base__;

  /* den show-status speichern */
  //DL_sho_Tab[*ind] = show | (pick * 2);



  //================================================================
  Fertig:

  // if(DL_ind == 728) AP_debug__  ("GL_fix_DL_ind");   // stop in debugger
  // printf("ex GL_fix_DL_ind dli=%d ind=%ld %ld\n",DL_ind,*ind,GR_TAB_IND);

  return DL_ind;

}




//=======================================================================
  int GL_DrawLn_Ini (long *ind, int attInd) {
//=======================================================================
// Init Linear-Figure
// see also GL_Draw_Ini


  GLuint   dlInd;


  // printf("GL_DrawLn_Ini %d %d\n",*ind,attInd);

  // attInd=0;   // NUR TEST


  /*  den DL-Index (+ Offset) holen)   */
  dlInd = GL_fix_DL_ind (ind);

  glNewList (dlInd, GL_COMPILE);

  glDisable (GL_LIGHTING);

  glCallList (DL_base_LnAtt + attInd);

  return 0;
}


//================================================================
  int GL_Surf_Ini (long *ind, ColRGB *col) {
//================================================================
// exit with GL_EndList
// Init Color nun in GL_ColSet ex GR_DrawFan/GR_DrawStrip
// col UNUSED !


// ACHTUNG: in GL_Disp_sur ebenfalls  2 x glColor3ubv !!!!
 
  GLuint   dlInd;


  // printf("GL_Surf_Ini %ld\n",*ind);
  // if(col) printf("  tra=%d col=%d\n",col->vtra,col->color);
  // else printf("  defCol\n");


  //  den DL-Index (+ Offset) holen)
  dlInd = GL_fix_DL_ind (ind);
    // printf(" dli=%d\n",dlInd);

  glNewList (dlInd, GL_COMPILE);

  glEnable (GL_LIGHTING);

/*
  if(col->vtra > 0) {
    glEnable (GL_LIGHTING);
    glEnable (GL_BLEND);
    GL_stat_blend = 1;
  }

wird im GL_Disp_sur gemacht - vom Color-Record bei den tesselated Records ..
  if(col->vtra > 0) {
    if(col->color == 0) {
      GL_actCol = GL_defCol;
    } else {
      GL_actCol = *col;
    }
    ((char*)&GL_actCol)[3] = GL_transpTab[col->vtra];
    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);
    // glEnable (GL_ALPHA_TEST);
    glEnable (GL_BLEND); // ist hier fuer subModels notwendig !!
    // glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4ubv ((unsigned char*)&GL_actCol);
    // glColor4f (0.7f, 0.7f, 1.0f, 0.25f);  // GlasAehnlich ...
    GL_stat_blend = 1;
    return 0;
  }
*/


/*            nun in GL_ColSet
  // normal farbig, nicht transparent; kein Color-Record bei den tesselated
  // Record!
  if(col) {
    if(col->color == 0) {
      // UT3D_stru_dump (Typ_Color, &GL_defCol, "GL_Surf_Ini-GL_defCol:");
      glColor3ubv ((unsigned char*)&GL_defCol);
    } else {
      glColor3ubv ((unsigned char*)col);
    }

  // } else {
    // glColor3ubv ((unsigned char*)&GL_defCol);
  }
*/


  return 0;
}


//================================================================
  int GL_Tex_Ini (long *ind, TexBas *tb, TexRef *tr) {
//================================================================
// init textured surface
// Die Textur wird am (absoluten) Nullpunkt geladen;
// wenn der erste Punkt der Textur am // Nullpunkt liegt,
// ist kein Offset notwendig; else muss der Offset in der
// Plane bis zum ersten Punkt definiert werden (von 0-1; in X und Y !).
// Der Offset vom (absoluten) Nullpunkt muss selbst errechnet werden !

// udx/udy ist eine Verschiebefaktor; 0.5 = das Bild um Breite*0.5 verschieben.
//   (ist nicht vom Scale abhaengig).

// Die Werte fuer glTranslate sind nicht UserUnits sondern Faktoren;
//   X-Wert=1.5  bedeutet die Textur um 1.5-fache Flaechenbreite verschieben !
//   ist aber vom Scale abhaengig; 

// das bild wird im abs 0,0,0 geladen
// Die Texturebene wird durch 2 Vektoren definiert: vs und vt.
//   diese definieren auch die Groesse des Image (scale).
//   halbe Laenge von vs/vx: 4 Bilder auf der Fläche !
// verschieben; (glTranslatef; im code aber nach glRotatef)
//   ist ein Faktor der Imagesize; negativ eingeben;
//   Faktor =  Abstand des Nullpunkts in Anzahl Imagesizes
//   (Imagesize via vs = skaliert !)
// drehen (glRotatef). Dreht nun um den neuen Nullpunkt.
//   gedreht wird um den durch die Verschiebung definierten neuen nullpunkt.

// glTexGendv (GL_S,    apply horizontal vector vs 
// glTexGendv (GL_T,    apply vertical vector vt
// glRotatef            apply rotation
// glScalef             apply scale
// glTranslatef         apply offset ...



  GLuint  dlInd, texInd;
  double  vs[4], vt[4];
  float   sclx, scly, fx, fy;
  float   f1, f2, f3;
  double  d1, d2, dx, dy;
  // Vector  vx;


  // printf("GL_Tex_Ini TexNr=%d ind=%ld\n",tb->texNr,*ind);
    // UT3D_stru_dump (Typ_VC, &tr->vx, "  vx:");
    // UT3D_stru_dump (Typ_VC, &tr->vy, "  vy:");


  dlInd = GL_fix_DL_ind (ind);

  // get OpenGL-TextureNr
  texInd = tb->texNr;



  //------- Variante 2: ------------------
  dx = 1. / tr->ssx;
  dy = 1. / tr->ssy;
  dx = 1.f / (tr->uscx * tr->ssx);
  dy = 1.f / (tr->uscy * tr->ssy);
  vs[0] = tr->vx.dx * dx;
  vs[1] = tr->vx.dy * dx;
  vs[2] = tr->vx.dz * dx;
  vs[3] = 0.;
  vt[0] = tr->vy.dx * dy;
  vt[1] = tr->vy.dy * dy;
  vt[2] = tr->vy.dz * dy;
  vt[3] = 0.;
  sclx = 1. / tr->uscx; scly = 1. / tr->uscy;



  // udx/y ist scaleUnabhängig; 0.5 = halbe Bildbreite !!!
  // der OpenGL-offset muss scaliert sein;
  // bei scale=2 verschiebt ein offsetwert von 0.25 um halbe bildbreite !
/* bis 2010-04-22
  d1 = tr->uscx * tr->ssx;
  dx = (tr->fx / d1) - tr->udx;
  d2 = modf (dx, &d1);
  fx = 1.f - d2;

  d1 = tr->uscy * tr->ssy;
  dy = (tr->fy / d1) - tr->udy;
  d2 = modf (dy, &d1);
  fy = 1.f - d2;
*/

  d1 = tr->uscx * tr->ssx;
  dx = (tr->fx / d1) - tr->udx;
  // dx = (tr->fx / d1) + (tr->udx * tr->uscx);
  // if(dx < 0) dx += 1.;
  // if(dx > 0.5) dx -= 1.;
  // if(dx < -0.5) dx += 1.;
  // d2 = modf (dx, &d1);  // nicht kappen wegen DrehPunkt !
  fx = dx;

  d1 = tr->uscy * tr->ssy;
  dy = (tr->fy / d1) - tr->udy;
  // dy = (tr->fy / d1) + (tr->udy * tr->uscy);
  // if(dy < 0) dy += 1.;
  // if(dy > 0.5) dy -= 1.;
  // if(dy < -0.5) dy += 1.;
  // d2 = modf (dy, &d1);
  fy = dy;


    // printf(" ssx/ssy   = %f %f\n",tr->ssx,tr->ssy);
    // printf(" fx=%f fy=%f ang-uar=%f\n",tr->fx,tr->fy,tr->uar);
    // printf(" udx / udy = %f %f\n",tr->udx,tr->udy);
    // printf(" vs=%f %f %f %f\n",vs[0],vs[1],vs[2],vs[3]);
    // printf(" vt=%f %f %f %f\n",vt[0],vt[1],vt[2],vt[3]);
    // printf(" uscx, uscy = %f %f\n",tr->uscx,tr->uscy);
    // printf(" px / py   = %f %f\n",tr->px,tr->py);
    // printf(" scaled offset = fx=%f fy=%f\n",fx,fy);



  //----------------------------------------------------------------
  glNewList(dlInd, GL_COMPILE);

    // connect texture ..
    glBindTexture (GL_TEXTURE_2D, texInd);
    // glCallList(dlTex); ersetzt durch glBindTexture

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);


    // scale, rotate, move  texture
    glMatrixMode (GL_TEXTURE);
    glPushMatrix();



    // Die vektoren vt & vt bestimmen auch Vergroesserung !
    // vor glTexGendv ein glTranslate bringt nix;
    glTexGendv(GL_S, GL_OBJECT_PLANE, vs);
    glTexGendv(GL_T, GL_OBJECT_PLANE, vt);
      // glTexGendv(GL_S, GL_EYE_PLANE, vs);
      // glTexGendv(GL_T, GL_EYE_PLANE, vt);


    // Scale. Scale = (1 / BoxSiz) * UserScale
    // 0-1 = Vergroesserung
    // glScalef (sclx, scly, 0.f);




    // Userdefined Rotation; Angle(Deg) & Axis.
    // zuerst drehen, dann verschieben (= um den neuen Punkt drehen).
    if(FDABS(tr->uar) > 0.01) 
    glRotatef (tr->uar, 0.f, 0.f, -1.f);


    // Um den Flaechenorigin zurueckverschieben
    // Origin direkt, negativ..
    // glTranslatef (tr->udx, tr->udy, 0.f);
    glTranslatef (-fx, -fy, 0.f);



    glMatrixMode(GL_MODELVIEW);   // reset ..

    // glMatrixMode (GL_TEXTURE);
    // glPopMatrix();

  return 0;

}


//***********************************************************************
  void GL_Draw_Ini (long *ind, int attInd) {
//***********************************************************************
// Init Flaechen

  GLuint   dlInd;



  /*  den DL-Index (+ Offset) holen)   */
  dlInd = GL_fix_DL_ind (ind);

  glNewList (dlInd, GL_COMPILE);

  glCallList (DL_base_LnAtt + attInd);
  // glEnable (GL_LIGHTING);
  // glDisable (GL_LIGHTING);

  return;
}


//================================================================
  int GL_Draw_Ini1 (long *ind, int attInd, int mode) {
//================================================================
// mode = 0 fuer Lines, Curves .. (ohne glDisable (GL_LIGHTING);)
// mode = 1 fuer Flaechen (schattieren .. = Default);

  GLuint   dlInd;



  /*  den DL-Index (+ Offset) holen)   */
  dlInd = GL_fix_DL_ind (ind);

  glNewList (dlInd, GL_COMPILE);

  if(mode < 1) glDisable (GL_LIGHTING);

  glCallList (DL_base_LnAtt + attInd);

  return 0;
}

 




//================================================================
  void GL_DrawPoint (long *ind, int attInd, Point *pt1 ) {
//================================================================
/// attInd: 0=default; 1=hili; 2=dimmed; 3=green; 4=yellow.

// *ind = Input; bei -1 wird ein temp. Objekt angelegt (aber max 31 !)
// *ind muss DL-Index ohne Offset (DL_base__) sein !



  // double kx,ky,kz;
  GLuint dlInd;


  // printf("GL_DrawPoint ind=%ld att=%d %f,%f,%f\n",*ind,attInd,
          // pt1->x,pt1->y,pt1->z);

// Points have different attributeSets; see GL_InitPtAtt
  if(attInd > 4) attInd = 4;      // 0=normal; 1=hili.  2014-02-09


  // kx=pt1->x;
  // ky=pt1->y;
  // kz=pt1->z;


  // den DL-Index (+ Offset) holen)  
  dlInd = GL_fix_DL_ind (ind);
    // printf("  _DrawPoint-dlInd=%d\n",dlInd);


  glNewList (dlInd, GL_COMPILE);

  glDisable (GL_LIGHTING);

  // glCallList (DL_base_LnAtt + Typ_Att_PT);
  // glCallList (DL_base_LnAtt + attInd);

  glCallList (DL_base_PtAtt + attInd);
  // needs glPointSize !



  glBegin (GL_POINTS);
    // glVertex3d (kx,ky,kz);
    glVertex3dv ((double*)pt1);
  glEnd ();

  glEnable (GL_LIGHTING);

  glEndList ();



}


/*
//================================================================
  void GL_Draw_ray (long *ind, int iAtt, Ray *lnr) {
//================================================================
// iAtt    Typ_Att_dim

  int      attInd;

  attInd = ((Ind_Att_ln*)&iAtt)->indAtt;   // index into GR_AttLnTab

  GL_DrawLn_Ini (ind, attInd);

  GL_Disp_ray (lnr);

  glEndList ();          // GL_EndList ();

}


//================================================================
  void GL_Disp_ray (Ray *lnr) {
//================================================================
// typ: 0 = both sides unlimited; else point = startpoint
// vector must have length = 1


  Point    p1, p2;

  UT3D_stru_dump(Typ_Ray, lnr, "GL_Disp_ray");

  UT3D_pt_traptmultvc (&p2, &lnr->p, &lnr->v, GL_ModSiz);

  if(lnr->typ) {
    p1 = lnr->p;
  } else {
    UT3D_pt_traptmultvc (&p1, &lnr->p, &lnr->v, -GL_ModSiz);
  }

  GL_Disp_ln (&p1, &p2);

}
*/


//================================================================
  void GL_DrawLine (long *ind, int iAtt, Line *ln1) {
//================================================================
/// \code
/// draw line
/// Input:
///   ind      dli
///   iAtt     see GR_Disp_ln2  (see ~/gCAD3D/cfg/ltyp.rc)
/// \endcode


  int         lTyp, attInd, lim;
  double      d1;
  Vector      vc1;
  Point       p1, p2;
  GLuint      dlInd;



  attInd = ((Ind_Att_ln*)&iAtt)->indAtt;   // index into GR_AttLnTab
  // lim    = ((Ind_Att_ln*)&iAtt)->lim;      // 0=limited, 4=unlimited
  // lTyp = GR_AttLnTab[attInd].dash;


    // printf("GL_DrawLine ind=%ld att=%d\n",*ind,iAtt);
    // UT3D_stru_dump(Typ_LN, ln1, "GL_DrawLine %d",attInd);
    // printf(" DL_base_LnAtt=%d\n",DL_base_LnAtt);



  //----------------------------------------------------------------
/*
  if(ln1->typ) {    // display rays

    GL_DrawLn_Ini (ind, attInd);

    UT3D_vc_2pt (&vc1, &ln1->p1, &ln1->p2);
    UT3D_pt_traptmultvc (&p2, &ln1->p1, &vc1, GL_ModSiz);

    if(ln1->typ) {
      p1 = ln1->p1;
    } else {
      UT3D_pt_traptmultvc (&p1, &ln1->p1, &vc1, -GL_ModSiz);
    }

    GL_Disp_ln (&p1, &p2);
    glEndList ();          // GL_EndList ();
    return;
  }
*/
/*
  // if length==1 then modify both points: extend * modelsize; set dimmed
  if(lim == ATT_LN_RAY0) {
    UT3D_vc_ln (&vc1, ln1);
    d1 = UT3D_lenq_vc (&vc1);
      // printf(" GL_DrawLine d1=%lf\n",d1);
    // if(UTP_comp2db (d1, 1.0, UT_TOL_PAR)) {  // 0.00001
      // attInd = Typ_Att_dim;
      // GR_ObjTab[*ind].iatt = attInd; // modify (is not active line-attribute)
      d1 = GL_ModSiz;
      UT3D_pt_traptvclen (&ln2.p1, &ln1->p1, &vc1, -d1);
      UT3D_pt_traptvclen (&ln2.p2, &ln1->p1, &vc1, d1);
      ln1 = &ln2;
  }
*/

  //----------------------------------------------------------------
  // den DL-Index (+ Offset) holen)  
  dlInd = GL_fix_DL_ind (ind);
    // printf(" LN-dlInd=%d attInd=%d\n",dlInd,attInd);


  glNewList (dlInd, GL_COMPILE);

    glDisable (GL_LIGHTING);
    //glDisable (GL_DEPTH_TEST);           // damit ueberdeckt sie auch Flächen !

    glCallList (DL_base_LnAtt + attInd);

    GL_Disp_ln (&ln1->p1, &ln1->p2);

    //glEnable (GL_DEPTH_TEST);           // prinzipiell
    // glEnable (GL_LIGHTING);

  glEndList ();

}


//================================================================
  void GL_DrawLtab (long *ind, int iAtt, Line *lna, int lNr) {
//================================================================
/// GL_DrawLtab     display group of lines

  int    i1, attInd;
  GLuint dlInd;


  // printf("GL_DrawLtab %d\n",lNr);
  // UT3D_stru_dump(Typ_LN, ln1, "GL_DrawLine %d",attInd);
  // printf(" DL_base_LnAtt=%d\n",DL_base_LnAtt);

  attInd = ((Ind_Att_ln*)&iAtt)->indAtt;   // index into GR_AttLnTab

  // den DL-Index (+ Offset) holen)  
  dlInd = GL_fix_DL_ind (ind);

  glNewList (dlInd, GL_COMPILE);

    glDisable (GL_LIGHTING);
    //glDisable (GL_DEPTH_TEST);           // damit ueberdeckt sie auch Flächen !

    glCallList (DL_base_LnAtt + attInd);

    glBegin (GL_LINES);

    for(i1=0; i1 < lNr; ++i1) {
      glVertex3dv ((double*)&lna[i1].p1);
      glVertex3dv ((double*)&lna[i1].p2);
        // UT3D_stru_dump(Typ_LN, &lna[i1], "  _DrawLine %d",i1);
    }

    glEnd ();

    //glEnable (GL_DEPTH_TEST);           // prinzipiell
    // glEnable (GL_LIGHTING);

  glEndList ();

}


//================================================================
  void GL_Disp_ln (Point *p1, Point *p2) {
//================================================================

  glBegin (GL_LINES);
    glVertex3dv ((double*)p1);
    glVertex3dv ((double*)p2);
  glEnd ();

}


//================================================================
  void GL_Disp_ln2 (Point2 *p1, Point2 *p2) {
//================================================================

  glBegin (GL_LINES);
    glVertex3d (p1->x, p1->y, 0.);
    glVertex3d (p2->x, p2->y, 0.);
  glEnd ();

}


//==============================================================================
  void GL_DrawPoly2D (long *ind,int iAtt, int ianz, Point2 pta[], double zval) {
//==============================================================================
// 2D-Polygon on plane Z=zval


  int    attInd;
  long   i1;
  GLuint dlInd;
  // Ind_Att_ln  *lnAtt=(Ind_Att_ln*)&iAtt;


  // printf("GL_DrawPoly2D: ind=%ld ptNr=%d Z=%lf\n",*ind,ianz,zval);
  // UT3D_stru_dump (Typ_Ltyp, &iAtt, " Ind_Att_ln:");


  // for(i1=0;i1<ianz; ++i1) {
    // printf("pt2 %d = %f,%f\n",i1,pta[i1].x,pta[i1].y);
  // }

  // attInd = lnAtt->indAtt;   // index into GR_AttLnTab
  attInd = ((Ind_Att_ln*)&iAtt)->indAtt;   // index into GR_AttLnTab

  // den DL-Index (+ Offset) holen)  
  dlInd = GL_fix_DL_ind (ind);

  glNewList (dlInd, GL_COMPILE);

  glDisable (GL_LIGHTING);

  glCallList (DL_base_LnAtt + attInd);


  glBegin (GL_LINE_STRIP);
    for (i1 = 0; i1 < ianz; i1++) {
      //TX_Print(" plvert %d = %f,%f,%f",i1,pta[i1].x, pta[i1].y, pta[i1].z);
      glVertex3d (pta[i1].x, pta[i1].y, zval);
    }
  glEnd();

  glEnable (GL_LIGHTING);

  glEndList ();

}


//================================================================
  void GL_create_dummy (long *ind) {
//================================================================
// create a empty GL-record.
// Used eg for Vector

  GLuint dlInd;

  dlInd = GL_fix_DL_ind (ind);
  glNewList (dlInd, GL_COMPILE);
  glEndList ();

}

 
//================================================================
  int GL_DrawPolySc (long *ind, int attInd, int ianz, Point *pTab) {
//================================================================
// Scaled Polygon
// geht so nicht; Verschiebung muss skaliert werden !


  long   i1;
  double d1;
  GLuint dlInd;


  // printf("GL_DrawPoly %ld %d %d\n",*ind,attInd,ianz);


  // den DL-Index (+ Offset) holen)
  dlInd = GL_fix_DL_ind (ind);

  glNewList (dlInd, GL_COMPILE);

  glPushMatrix ();
  glTranslated (pTab[0].x/GL_Scale, pTab[0].y/GL_Scale, pTab[0].z/GL_Scale);

  glCallList (DL_Ind_ScBack);           // ScaleBack

  glDisable (GL_LIGHTING);

  glCallList (DL_base_LnAtt + attInd);


  glBegin (GL_LINE_STRIP);
    for (i1 = 0; i1 < ianz; i1++) {
      // printf(" plvert %d = %f,%f,%f\n",i1,pTab[i1].x,pTab[i1].y,pTab[i1].z);
      // glVertex3d (pTab[i1].x, pTab[i1].y, pTab[i1].z);
      glVertex3dv ((double*)&pTab[i1]);
    }
  glEnd();

  // glPopMatrix ();  // vom ScaleBack
  glPopMatrix ();

  glEnable (GL_LIGHTING);

  glEndList ();

  return 0;
}

 
//================================================================
  int GL_DrawCvIpTab (long *ind, int att,
                      int *iba, int ibNr, Point *pTab, int mode) {
//================================================================
// draw polygon from indexTable; open or closed.
//  mode=0   open
//  mode=1   closed; add point pTab[iba[0]];

  long   i1;
  GLuint dlInd;


  // printf("GL_DrawPoly %ld %d %d\n",*ind,attInd,ianz);


  // den DL-Index (+ Offset) holen)
  dlInd = GL_fix_DL_ind (ind);

  glNewList (dlInd, GL_COMPILE);

  glDisable (GL_LIGHTING);

  glCallList (DL_base_LnAtt + att);


  glBegin (GL_LINE_STRIP);

  for (i1 = 0; i1 < ibNr; i1++) {
    // UT3D_stru_dump (Typ_PT, &pTab[iba[i1]], "%d P[%d]",i1,iba[i1]);
    glVertex3dv ((double*)&pTab[iba[i1]]);
  }

  if(mode == 1) glVertex3dv ((double*)&pTab[iba[0]]);  // closed

  glEnd();

  glEnable (GL_LIGHTING);

  glEndList ();

  return 0;

}
 

//================================================================
  int GL_Draw_cvp_dir (long *ind, int iAtt, int ianz, Point *pTab) {
//================================================================
/// \code
/// GL_Draw_cvp_dir          display polygon and direction-arrow
/// Input:
///   ind        nr of dispListRecord; see DL_StoreObj or DL_SetObj
///   iAtt       see GR_Disp_ln2  (see ~/gCAD3D/cfg/ltyp.rc)
///
/// see also GL_Draw_cvp_dir
/// \endcode


  int    attInd, ipe;
  long   i1;
  double ay, az, scl;
  GLuint dlInd;
  Vector vc1;


  printf("GL_Draw_cvp_dir ind=%ld iAtt=%d ptNr=%d\n",*ind,iAtt,ianz);
  // for(i1=0;i1<ianz;++i1)
    // printf(" p[%ld] %lf %lf %lf\n",i1,pTab[i1].x,pTab[i1].y,pTab[i1].z);


  ipe = ianz - 1;
  UT3D_vc_2pt (&vc1, &pTab[ipe], &pTab[ipe - 1]);
    UT3D_stru_dump (Typ_VC, &vc1, "vc1");

  UT3D_2angr_vc (&az, &ay, &vc1);
    // printf(" az=%lf ay=%lf\n",az,ay);


  // rad -> deg
  az = UT_DEGREES(az);
  ay = UT_DEGREES(ay);
    // printf(" az=%lf ay=%lf\n",az,ay);

  scl = GL_SclNorm;


  attInd = ((Ind_Att_ln*)&iAtt)->indAtt;   // index into GR_AttLnTab
  // den DL-Index (+ Offset) holen)  
  dlInd = GL_fix_DL_ind (ind);

  glNewList (dlInd, GL_COMPILE);

  glDisable (GL_LIGHTING);

  glCallList (DL_base_LnAtt + attInd);

    //----------------------------------------------------------------
    glBegin (GL_LINE_STRIP);
    for (i1 = 0; i1 < ianz; i1++) glVertex3dv ((double*)&pTab[i1]);
    glEnd();

    //----------------------------------------------------------------
    glPushMatrix ();

    // Transl und Scale: Reihenfolge wichtig !
    glTranslated (pTab[ipe].x, pTab[ipe].y, pTab[ipe].z);
    glRotated (az, 0.0, 0.0, 1.0);  // um Z drehen
    glRotated (ay, 0.0, -1.0, 0.0);  // um Y drehen
    glScaled (scl, scl, scl);
    glCallList ((GLuint)SYM_ARRO3H);
    glPopMatrix ();


  glEnable (GL_LIGHTING);

  glEndList ();

  return 0;

}


//================================================================
  void GL_DrawPoly (long *ind, int iAtt, int ianz, Point *pTab) {
//================================================================
/// \code
/// display Polygon. Im Array GL_ptArr30. Kreise / Kreisbögen.
/// Input:
///   ind        nr of dispListRecord; see DL_StoreObj or DL_SetObj
///   iAtt       see GR_Disp_ln2  (see ~/gCAD3D/cfg/ltyp.rc)
///
/// see also GL_Draw_cvp_dir
/// \endcode


  int    attInd;
  long   i1;
  GLuint dlInd;


  // printf("GL_DrawPoly ind=%ld att=%d ptNr=%d\n",*ind,attInd,ianz);
  // for(i1=0;i1<ianz;++i1)
    // printf(" p[%ld] %lf %lf %lf\n",i1,pTab[i1].x,pTab[i1].y,pTab[i1].z);

  attInd = ((Ind_Att_ln*)&iAtt)->indAtt;   // index into GR_AttLnTab


  // den DL-Index (+ Offset) holen)  
  dlInd = GL_fix_DL_ind (ind);

  glNewList (dlInd, GL_COMPILE);

  glDisable (GL_LIGHTING);

  glCallList (DL_base_LnAtt + attInd);


  // if(ianz == 2)
    // glBegin (GL_LINES);
  // else
    glBegin (GL_LINE_STRIP);

    for (i1 = 0; i1 < ianz; i1++) {
      // printf(" plvert %d = %f,%f,%f\n",i1,pTab[i1].x,pTab[i1].y,pTab[i1].z);
      // glVertex3d (pTab[i1].x, pTab[i1].y, pTab[i1].z);
      glVertex3dv ((double*)&pTab[i1]);
    }

  glEnd();

  glEnable (GL_LIGHTING);

  glEndList ();

}


/*
//================================================================
  void GL_DrawQFac (long *ind,int attInd,ObjG2* o1,double z1,ObjG2* o2,double z2) {
//================================================================
// Quadrilateral Face. (Viereck).

  GLuint   dlInd;
  Vector   GL_norm;


  printf("GL_DrawQFac %d %f,%f  %f,%f\n",*ind,o1->p1.x,o1->p1.y,
                                              o1->p2.x,o1->p2.y);
  // printf("GL_DrawQFac att=%d\n",attInd);



  // den DL-Index (+ Offset) holen)  
  dlInd = GL_fix_DL_ind (ind);

  glNewList (dlInd, GL_COMPILE);

  //glCallList (DL_base_LnAtt + Typ_Att_Fac2);
  glCallList (DL_base_LnAtt + attInd);
  //glCallList (DL_base_LnAtt + attInd);
  //glColor3f (0.5f, 0.5f, 0.5f);



  glBegin (GL_QUADS);

    GRU_calc_normal2 (&GL_norm, &o1->p1, &o1->p2, &o2->p2, z1, z2);
    glNormal3d (GL_norm.dx, GL_norm.dy, GL_norm.dz);

    glVertex3d (o1->p1.x, o1->p1.y, z1);
    glVertex3d (o1->p2.x, o1->p2.y, z1);
    glVertex3d (o2->p2.x, o2->p2.y, z2);
    glVertex3d (o2->p1.x, o2->p1.y, z2);

  glEnd ();



  // //Variante in Dreicke zerlegen und Normalvektor
  // glBegin (GL_TRIANGLES);
// 
    // GRU_calc_normal2 (&GL_norm, &o1->p1, &o1->p2, &o2->p2, z1, z2);
    // // Verkehrt geht gar net !!
    // //GRU_calc_normal2 (&GL_norm, &o1->p2, &o1->p1, &o2->p1, z1, z2);
    // glNormal3d (GL_norm.dx, GL_norm.dy, GL_norm.dz);
// 
    // glVertex3d (o1->p1.x, o1->p1.y, z1);
    // glVertex3d (o1->p2.x, o1->p2.y, z1);
    // glVertex3d (o2->p1.x, o2->p1.y, z2);
// 
// 
    // GRU_calc_normal2 (&GL_norm, &o2->p2, &o2->p1, &o1->p2, z2, z1);
    // //GRU_calc_normal2 (&GL_norm, &o2->p1, &o2->p2, &o1->p2, z2, z1);
    // glNormal3d (GL_norm.dx, GL_norm.dy, GL_norm.dz);
// 
    // glVertex3d (o1->p2.x, o1->p2.y, z1);
    // glVertex3d (o2->p2.x, o2->p2.y, z2);
    // glVertex3d (o2->p1.x, o2->p1.y, z2);
// 
  // glEnd ();



  if(WC_stat_bound == ON) {
    glDisable (GL_LIGHTING);

    glCallList (DL_base_LnAtt + Typ_Att_Symb);

    // glBegin (GL_LINES);       // unten - oben
    glBegin (GL_LINE_STRIP);       // unten - oben
      glVertex3d (o1->p2.x, o1->p2.y, z1);
      glVertex3d (o2->p2.x, o2->p2.y, z2);
      glVertex3d (o2->p1.x, o2->p1.y, z2); // oberer Linie?
    glEnd ();

    glEnable (GL_LIGHTING);
  }


  glEndList ();


}


//================================================================
  void GL_DrawRCone (long *ind, int attInd, ObjG2* o1, double z1,
                                            ObjG2* o2, double z2) {
//================================================================
// Punkte unten > GL_ptArr20, Punkte oben > GL_ptArr21.

  int      ptAnzU, ptAnzO, i1, i2;
  GLuint   dlInd;
  Vector   GL_norm;


  // Punkt + Line: DrawTri.

  printf("GL_DrawRCone %d u=%d zu=%f o=%d zo=%f\n",*ind,o1->typ,z1,o2->typ,z2);


   ptAnzU = 0;
   ptAnzO = 0;
   
    

  // -------  unten ------------------------
  if(o1->typ == Typ_PT2) {                   // Punkt

    ptAnzU = 1;             
    GL_ptArr20[0] = o1->p1;


  } else if(o1->typ == Typ_LN2) {            // Line

    ptAnzU = 2;            
    GL_ptArr20[0] = o1->p1;
    GL_ptArr20[1] = o1->p2;


  } else if(o1->typ == Typ_CI2) {

    GRU_teileArc (GL_ptArr20, &ptAnzU, o1);   // AC; liefert ptAnz!

  }




  // ------- oben ----------------------------------------------------
  if(o2->typ == Typ_PT2) {                    // Punkt

    ptAnzO = 1;
    GL_ptArr21[0] = o2->p1;


  } else if(o2->typ == Typ_LN2) {             // Line

    ptAnzO = 2;
    GL_ptArr21[0] = o2->p1;
    GL_ptArr21[1] = o2->p2;


  } else if(o2->typ == Typ_CI2) {

    GRU_teileArc (GL_ptArr21, &ptAnzO, o2);   // AC; liefert ptAnz!

  }



  //AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
  // ein Problemfall.
  if((ptAnzU == 0)||(ptAnzO == 0)) {
    TX_Error("GL_DrawRCone nicht unterstuetzter Typ %d %d",o1->typ,o2->typ);
    return;
  }



  //------------------------------------------------------------------------


  //L_Weiter1:
  TX_Print(" ptAnzU=%d z1=%f ptAnzO=%d z2=%f",ptAnzU,z1,ptAnzO,z2);
  for(i1=0; i1<ptAnzU; ++i1) {
    TX_Print(" u %d %f,%f",i1,GL_ptArr20[i1].x,GL_ptArr20[i1].y);
  }
  for(i1=0; i1<ptAnzO; ++i1) {
    TX_Print(" o %d %f,%f",i1,GL_ptArr21[i1].x,GL_ptArr21[i1].y);
  }




  //==========================================================================
  // den DL-Index (+ Offset) holen)  
  dlInd = GL_fix_DL_ind (ind);

  glNewList (dlInd, GL_COMPILE);

  glCallList (DL_base_LnAtt + attInd);
  // glCallList (DL_base_LnAtt + Typ_Att_Fac1);


  // Man darf innen / aussen nicht vertauschen (Farbe beim schattieren).
  // Vert1 ist der gemeinsame Punkt, dann kommt das Polygon.
  glBegin (GL_TRIANGLE_FAN);
    if(ptAnzU < 2) {
      glVertex3d (GL_ptArr20[0].x,GL_ptArr20[0].y,z1);

      for(i1=0; i1<ptAnzO; ++i1) {
        if(i1 > 0) {
          GRU_calc_normal2(&GL_norm,
             GL_ptArr20,&GL_ptArr21[i1-1],&GL_ptArr21[i1],z1,z2);
          UT3D_vc_invert (&GL_norm, &GL_norm);
          // UT3D_stru_dump (Typ_VC, &GL_norm, "vz");
          glNormal3dv ((double*)&GL_norm);
        }
        glVertex3d (GL_ptArr21[i1].x,GL_ptArr21[i1].y,z2);
      }


    } else {
      glVertex3d (GL_ptArr21[0].x,GL_ptArr21[0].y,z2);

      //for(i1=0; i1<ptAnzU; ++i1) {
      for(i1=ptAnzU-1; i1>=0; --i1) {
        if(i1 < ptAnzU-1) {
          GRU_calc_normal2(&GL_norm,
             GL_ptArr21,&GL_ptArr20[i1],&GL_ptArr20[i1+1],z1,z2);
          UT3D_vc_invert (&GL_norm, &GL_norm);
          // UT3D_stru_dump (Typ_VC, &GL_norm, "vz");
          glNormal3dv ((double*)&GL_norm);
        }
        glVertex3d (GL_ptArr20[i1].x,GL_ptArr20[i1].y,z1);
      }
    }
  glEnd ();


  if(WC_stat_bound == ON) {
    glDisable (GL_LIGHTING);

    glCallList (DL_base_LnAtt + Typ_Att_Symb);

    glBegin (GL_LINES);
      i1=ptAnzU-1;
      i2=ptAnzO-1;
      glVertex3d (GL_ptArr20[i1].x,GL_ptArr20[i1].y,z1);
      glVertex3d (GL_ptArr21[i2].x,GL_ptArr21[i2].y,z2);
      // for(i1=0; i1<ptAnzO; ++i1) {
        // glVertex3d (GL_ptArr21[i1].x,GL_ptArr21[i1].y,z2);
      // }


    glEnd ();

    glEnable (GL_LIGHTING);
  }



  glEndList ();


}


//================================================================
  void GL_DrawRFac (long *ind,int attInd,ObjG2* o1,double z1,ObjG2* o2,double z2) {
//================================================================
// Ruft bei Sonderfall LN + LN QFac.
// Punkte unten > GL_ptArr20, Punkte oben > GL_ptArr21.


  int     ptAnz, i1;
  // Point2  pt20, pt21;
  GLuint  dlInd;
  Vector  GL_norm;


  printf("GL_DrawRFac %d u=%d zu=%f o=%d zo=%f\n",*ind,o1->typ,z1,o2->typ,z2);


  // wenn ein Punkt dabei: Cone rufen.
  if((o1->typ == Typ_PT2)||(o2->typ == Typ_PT2)) {
    GL_DrawRCone (ind, attInd, o1, z1, o2, z2);
    return;
  }



  // 2 Lines: normaler GL_QUADS.
  if((o1->typ == Typ_LN2) && (o2->typ == Typ_LN2)) {
    GL_DrawQFac (ind, attInd, o1, z1, o2, z2);
    return;
  }



  // 2 Arcs: den unteren mit GRU_teileArc zerlegen (geht schneller), den
  // oberen mit GRU_teileArc1 (gezielt n Punkte).
  if((o1->typ == Typ_CI2) && (o2->typ == Typ_CI2)) {
    GRU_teileArc  (GL_ptArr20, &ptAnz, o1);            // unten AC; liefert ptAnz!
    GRU_teileArc1 (GL_ptArr21,  ptAnz, o2);            // oben AC
    goto L_Weiter1;
  }


  // Arc unten Line oben.
  // Zuerst den Arc zerlegen (liefert Anz. Punkte)
  if((o1->typ == Typ_CI2) && (o2->typ == Typ_LN2)) {
    GRU_teileArc (GL_ptArr20, &ptAnz, o1);             // unten AC; liefert ptAnz!
    GRU_teileLin (GL_ptArr21,  ptAnz, &o2->p1, &o2->p2);         // oben LN
    goto L_Weiter1;
  }


  // Line unten Arc oben.
  if((o1->typ == Typ_LN2) && (o2->typ == Typ_CI2)) {
    GRU_teileArc (GL_ptArr21, &ptAnz, o2);             // oben AC; liefert ptAnz!
    GRU_teileLin (GL_ptArr20,  ptAnz, &o1->p1, &o1->p2);         // unten LN
    goto L_Weiter1;
  }




  //AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
  // ein Problemfall.
  TX_Error("GL_DrawRFac nicht unterstuetzter Typ %d %d",o1->typ,o2->typ);
  return;




  //------------------------------------------------------------------------
  L_Weiter1:
  printf("GL_DrawRFac - L_Weiter1\n");

  for(i1=0; i1<ptAnz; ++i1) {
    TX_Print(" u %d %f,%f",i1,GL_ptArr20[i1].x,GL_ptArr20[i1].y);
    TX_Print(" o %d %f,%f",i1,GL_ptArr21[i1].x,GL_ptArr21[i1].y);
  }


  //TX_Print(" ptAnz=%d z1=%f z2=%f",ptAnz,z1,z2);





  //==========================================================================
  // den DL-Index (+ Offset) holen)  
  dlInd = GL_fix_DL_ind (ind);

  glNewList (dlInd, GL_COMPILE);

  glCallList (DL_base_LnAtt + attInd);
  //glCallList (DL_base_LnAtt + Typ_Att_Fac2);


  // 0=erster unten, 1=erster oben, 2=zweiter unten, 3= zweiter oben ...
  glBegin (GL_QUAD_STRIP);
    for(i1=0; i1<ptAnz; ++i1) {

      if(i1 > 0) {
        GRU_calc_normal2 (&GL_norm, &GL_ptArr20[i1-1], &GL_ptArr20[i1],
                &GL_ptArr21[i1],  z1, z2);
        glNormal3dv ((double*)&GL_norm);
      }

      glVertex3d (GL_ptArr21[i1].x,GL_ptArr21[i1].y,z2);
      glVertex3d (GL_ptArr20[i1].x,GL_ptArr20[i1].y,z1);
    }
  glEnd ();




  if(WC_stat_bound == ON) {
    glDisable (GL_LIGHTING);

    glCallList (DL_base_LnAtt + Typ_Att_Symb);

    //glDepthFunc (GL_ALWAYS);       // ueberschreibt immer (auch Flaechen !)
    // glBegin (GL_LINES);
    glBegin (GL_LINE_STRIP);
      // glVertex3d (GL_ptArr21[i1].x,GL_ptArr21[i1].y,z2);
      for(i1=0; i1<ptAnz; ++i1) {
        glVertex3d (GL_ptArr21[i1].x,GL_ptArr21[i1].y,z2);
      }
      i1=ptAnz-1;
      glVertex3d (GL_ptArr20[i1].x,GL_ptArr20[i1].y,z1);
    glEnd ();
    //glDepthFunc (GL_LEQUAL);              // eine Spur besser als LESS

    glEnable (GL_LIGHTING);
  }



  glEndList ();

}
*/


/************************************************************************ */
  void GL_DrawRSur (long *ind,int attInd,int ptAnz,Point *pa1,Point *pa2) {
/************************************************************************ */
/* RuledSurf schon fertig aufgeloest.
Die ruled Surf in GL_ptArr30 und GL_ptArr31 hinmalen.
*/


  // printf("GL_DrawRSur %d",attInd);



  /*  den DL-Index (+ Offset) holen)   */
  GL_Draw_Ini (ind, attInd);


  // GL_DrawStripe (ptAnz, pa1, pa2);
  GL_DrawStrip2 (pa1, pa2, ptAnz, 0);


  glEndList ();


}


//================================================================
  int GL_orient__ (int dir) {
//================================================================

  if(dir > 0)  glFrontFace (GL_CCW);
  else         glFrontFace (GL_CW);

  return 0;

}


//================================================================
  int GL_DrawFace (int ptNr, Point *pta, int ptiNr, short *pti, int side) {
//================================================================
// Ausgabe von mehreren Dreiecken
// ACHTUNG: es muss ein OpenGL-Objekt offen sein (GL_Draw_Ini)
// Input: Punkttabelle pta[ptNr]
//        short  pti[ptiNr]; je drei Indexe definieren ein Dreieck.
// Side definiert Aussenseite
// side = 0: Richtung 1 (Conus)
// side = 1: Richtung 2 (Torus)


  int    i1;


  // printf("GL_DrawFace %d %d\n",ptNr,ptiNr);


  // TEST:
  // for(i1=0; i1<ptNr; ++i1)  UT3D_stru_dump (Typ_PT, &pta[i1], "P[%d]:",i1);


  // alle Dreiecke umdrehen GL_CW; Def=GL_CCW ?
  // glFrontFace (GL_CW);



  for(i1=0; i1<ptiNr; i1 += 3) {
    // printf(" Fan %d %d %d\n",pti[i1],pti[i1+1],pti[i1+2]);

    // Richtung 1
    // if(side != 0) {    // Test !=
      GL_DrawStrip1 (&pta[pti[i1]], &pta[pti[i1+2]], &pta[pti[i1+1]], 1);

    // } else {
      // Richtung 2
      // GL_DrawStrip1 (&pta[pti[i1]], &pta[pti[i1+1]], &pta[pti[i1+2]], 1);
    // }
  }


  return 0;


}


//======================================================================
  int GL_DrawLStrip (Point *pTab, int ptAnz) {
//======================================================================

  int   i1;


  printf("GL_DrawLStrip %d\n",ptAnz);


  glBegin (GL_LINE_STRIP);
    for (i1 = 0; i1<ptAnz; i1++) {
      // printf(" plvert %d = %f,%f,%f\n",i1,pTab[i1].x,pTab[i1].y,pTab[i1].z);
      // glVertex3d (pTab[i1].x, pTab[i1].y, pTab[i1].z);
      glVertex3dv ((double*)&pTab[i1]);
    }
  glEnd();

  return 0;

}


//======================================================================
  int GL_DrawStrip1 (Point *ps, Point *pa1, Point *pa2, int ptAnz) {
//======================================================================
// Displ. Streifen, durch Startpunkt 2 Polygone beschrieben (ruled Surf)
// Die Polygone haben natuerlich gleich viele Punkte.

// Erster Punkt gleich (p1 = ps)

//        a2[2]
//     a2[1]
//  a2[0]
// ps       6
//        //|
//       4 /|
//     //| /|
//    2 /|/ |
//  / |/ |/ |
// 1--3--5--7
//
// ps 
//   a1[0]
//      a1[1]
//        a1[2]


  int      i1;
  Vector   GL_norm;


  // printf("GL_DrawStrip1 %d\n",ptAnz);

  // man braucht einen call auf eine Liste mit den Flaechenattributen !!!
  // glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, GLcol_t1);
  // glMaterialfv(GL_FRONT, GL_EMISSION, GLcol_black);
  // glMaterialfv(GL_FRONT, GL_DIFFUSE, GL_diff1);
  // glMaterialfv(GL_FRONT, GL_AMBIENT, GL_amb1);


  // glEnable(GL_NORMALIZE);
  // glEnable(GL_AUTO_NORMAL);       // bei Linux u Windows nutzlos



  // glBegin (GL_TRIANGLES);
  glBegin (GL_TRIANGLE_STRIP);

  // Normalvektor des ersten Dreiecks
  GRU_calc_normal(&GL_norm, ps, &pa1[0], &pa2[0]);
  // GRU_calc_normal(&GL_norm, ps, &pa2[0], &pa1[0]);
  // UT3D_stru_dump (Typ_VC, &GL_norm, "norm:");
  glNormal3dv ((double*)&GL_norm);


  // die ersten 3 Punkte raus ..
  glVertex3dv ((double*)ps);
  glVertex3dv ((double*)&pa1[0]);
  glVertex3dv ((double*)&pa2[0]);



  for(i1=1; i1<ptAnz; ++i1) {

    if(i1 > 1) GRU_calc_normal(&GL_norm, &pa2[i1-1], &pa1[i1-1], &pa1[i1]);
    // if(i1 > 1) GRU_calc_normal(&GL_norm, &pa1[i1-1], &pa2[i1-1], &pa1[i1]);
    glNormal3dv ((double*)&GL_norm);

    glVertex3dv ((double*)&pa1[i1]);


    GRU_calc_normal(&GL_norm, &pa1[i1], &pa2[i1], &pa2[i1-1]);
    // GRU_calc_normal(&GL_norm, &pa2[i1], &pa1[i1], &pa2[i1-1]);
    glNormal3dv ((double*)&GL_norm);

    glVertex3dv ((double*)&pa2[i1]);
  }


  glEnd ();




  return 0;

}


//======================================================================
  int GL_DrawStrip2 (Point *pa, Point *pb, int ptNr, int newS) {
//======================================================================
// 1 Streifen, durch 2 Polygone beschrieben (ruled Surf)
// Die Polygone haben natuerlich gleich viele Punkte.
// 2 Reihen von <ptNr> Punkten; 
// pa(0-(ptNr-1)) ist 1.Reihe, pb(ptNr-(ptNr+ptNr-1)) ist 2.Reihe.

// newS:  0 = do not start new surface
// newS:  1 = start new surface

//        a
//    0--1--2--3      
//    | /| /| /|
//    |/ |/ |/ |
//    0--1--2--3
//        b
//
//        a
//       1--2--3\
//      /| /| /| \e
//    0/ |/ |/ | /
//      \1--2--3/
//        b
//
// OUTPUT:
//  TriangleStrip-GL:
//   Output first 3 points = first triangle.
//   Each following point makes a new triangle (with the last 2 points of
//     the preceding triangle).
//    
//  Clean stripe:
//   a0,b0, a1,b1, a2, b2 ..
//  Stripe with acute start and/or end:
//   First: (0,b1,a1), quads: (a2,b2,a3,b3), last triangle: (e).
//

// TODO:
// Es werden die Dreiecke miteinender verrundet. Bei zB Quader schlecht !
// es sollte also ein Input sein: Flächen abrunden  oder Flächen kantig (planar).


  int      i0, i1, i2, a1, a2, mod1, mod2, iSta;
  Vector   GL_norm1, GL_norm2;

  // Nur Test:
  long  dli;


  // printf("----------------------------------------------- \n");
  // printf("GL_DrawStrip2 %d %d\n",ptNr,TSU_mode);
  // for(i1=0;i1<ptNr;++i1)UT3D_stru_dump(Typ_PT,&pa[i1]," pa[%d]:",i1);
  // for(i1=0;i1<ptNr;++i1)UT3D_stru_dump(Typ_PT,&pb[i1]," pb[%d]:",i1);
  // GR_Disp_pTab (ptNr, pa, SYM_TRI_S, 2); // ACHTUNG: keine Flaeche !!
  // GR_Disp_pTab (ptNr, pb, SYM_TRI_S, 2);
  // return 0;
  



  if(TSU_mode != 0) {  // 0 = draw OpenGL
    if(newS) GLT_stor_rec (6, NULL, NULL, Typ_SUR);
    return GLT_stor_rec (2, pa, pb, ptNr);
  }



  // test if pa1 is a point (strip from revolved surfaces) can have 
  // same point on pa1, pa2.
  // iSta=2: 1.point of pa1 and pa2 is identical; else iSta=1.
  if(UT3D_comp2pt (&pa[0], &pb[0], UT_DISP_cv) == 1)  { i0 = 1;  i1 = 2; }
  else                                                { i0 = 0;  i1 = 1; }
    // printf(" i0=%d i1=%d\n",i0,i1);

  // i1 = ptNr - 1;
  // if(UT3D_comp2pt (&pa1[i1], &pa2[i1], UT_DISP_cv) == 1)    mod2 = 1;
  // else                                                      mod2 = 0;
    // printf(" mod2=%d\n",mod2);



  // MUST draw 1. triangle separate !

  if(i1 == 2) {          // first strip is only a triangle ..
      // printf(" a0,b1,a1\n");
    glBegin (GL_TRIANGLE_STRIP);
    GRU_calc_normal(&GL_norm1, &pa[0], &pb[1], &pa[1]);
    glNormal3dv ((double*)&GL_norm1);

    glVertex3dv ((double*)&pa[0]);
    glVertex3dv ((double*)&pb[1]);
    glVertex3dv ((double*)&pa[1]);
    glEnd ();
  }


  glBegin (GL_TRIANGLE_STRIP);

  GRU_calc_normal(&GL_norm1, &pa[i0], &pb[i0], &pa[i1]);
  glNormal3dv ((double*)&GL_norm1);

    // printf(" a%d b%d a%d / i0=%d i1=%d\n",i0,i0,i1,i0,i1);
  glVertex3dv ((double*)&pa[i0]);
  glVertex3dv ((double*)&pb[i0]);
  glVertex3dv ((double*)&pa[i1]);

  GRU_calc_normal(&GL_norm2, &pa[i1], &pb[i0], &pb[i1]);
  glNormal3dv ((double*)&GL_norm2);

      // printf(" b%d\n",i1);
  glVertex3dv ((double*)&pb[i1]);



  L_drw_nxt:
    ++i0;
    ++i1;
    if(i1 >= ptNr) goto L_drw_end;

    GRU_calc_normal(&GL_norm1, &pa[i0], &pb[i0], &pa[i1]);
    glNormal3dv ((double*)&GL_norm1);
      // UT3D_stru_dump (Typ_VC, &GL_norm1, " nvc1[%d]:",i1);
    // TestDisp Vektor:
    // dli = -1; GL_DrawSymV3 (&dli, SYM_ARROW, 2, &pa[i1], &GL_norm1, 10.);

      // printf(" a%d\n",i1);
    glVertex3dv ((double*)&pa[i1]);


    GRU_calc_normal(&GL_norm2, &pa[i1], &pb[i0], &pb[i1]);
    glNormal3dv ((double*)&GL_norm2);
      // UT3D_stru_dump (Typ_VC, &GL_norm2, " nvc2[%d]:",i1);
    // TestDisp Vektor:
    // dli = -1; GL_DrawSymV3 (&dli, SYM_ARROW, 2, &pb[i1], &GL_norm2, 10.);

      // printf(" b%d\n",i1);
    glVertex3dv ((double*)&pb[i1]);

    goto L_drw_nxt;


  L_drw_end:

    glEnd ();

/*
  glBegin (GL_TRIANGLE_STRIP);

    GRU_calc_normal(&GL_norm, &pa1[0], &pa1[1], &pa1[2]);
    glNormal3d (GL_norm.dx, GL_norm.dy, GL_norm.dz);

    for(i1=0; i1<ptNr; ++i1) {

      if(i1 > 2) {
        // GRU_calc_normal(&GL_norm, &pa1[i1], &pa1[i1-1], &pa1[i1-2]);
        glNormal3d (GL_norm.dx, GL_norm.dy, GL_norm.dz);
        glNormal3dv (&GL_norm);
      }

      // glVertex3d (pa1[i1].x,   pa1[i1].y,   pa1[i1].z);
      glVertex3dv (&pa1[i1]);

    }

  glEnd ();
*/

/*
  // Variante, wenn die Punkte abwechselnd liegen.
  // Variante in Dreicke zerlegen und Normalvektor 
  glBegin (GL_TRIANGLES);

    a1 = 1; a2 = 2;
    for(i1=2; i1<ptNr; ++i1) {

      GRU_calc_normal(&GL_norm, &pa1[i1-a2], &pa1[i1-a1], &pa1[i1]);
      glNormal3d (-GL_norm.dx, -GL_norm.dy, -GL_norm.dz);
      // glNormal3dv (&GL_norm);

      glVertex3dv (&pa1[i1-a2]);
      glVertex3dv (&pa1[i1-a1]);
      glVertex3dv (&pa1[i1]);

      // beim naechsten Drehrichtung umdrehen !
      if(a1 == 1) {
        a1 = 2; a2 = 1;
      } else {
        a1 = 1; a2 = 2;
      }
    }
  glEnd ();
*/


  /* zusaetzl. noch Boundarygrenzen hinmalen */
  if(WC_stat_bound == ON) {
    glDisable (GL_LIGHTING);

    glCallList (DL_base_LnAtt + Typ_Att_Symb);

    /* glDepthFunc (GL_ALWAYS);   // ueberschreibt immer (auch Flaechen !) */
    glBegin (GL_LINES);
    i1=ptNr-1;
      // glVertex3d (pa1[i1].x,pa1[i1].y,pa1[i1].z);
      glVertex3dv ((double*)&pa[i1]);
      // glVertex3d (pa2[i1].x,pa2[i1].y,pa2[i1].z);
      glVertex3dv ((double*)&pb[i1]);
    glEnd ();
    /* glDepthFunc (GL_LEQUAL);              // eine Spur besser als LESS */

    glEnable (GL_LIGHTING);
  }


  return 0;
}



//========================================================================
  void GL_DrawFan (Point *pt1, int ptNr, Point *pa1, int typ, int newS) {
//========================================================================
// Den TriangleFan von Punkt pt1 um pa1 anzeigen.
// ptNr  = Anzahl Punkte in pa1; 
// typ: 0 = planarer Fan (nur ein Normalvec)
// typ: 1 = nicht planarer Fan (Normalvec f jedes Dreieck)
// newS:  0 = do not start new surface
// newS:  1 = start new surface
// USAGE: zB fuer Kreisflaeche pa mit ptNr=6 (erster == letzter Punkt):
//      GL_DrawFan (pa, ptNr-2, &pa[1], 0);

//
//   pa0----pa1          ptNr = 4
//    |    /  |
//    |   /   |
//    |  /    |
//    | /     |
//   pt1------pa2
//      \     /
//        \  /
//         pa3


  int      i1;
  Vector   GL_norm;


  // printf("GL_DrawFan %d\n",ptNr);
  // GR_Disp_pTab (1, pt1, SYM_TRI_S, 3);
  // GR_Disp_pTab (ptNr, pa1, SYM_TRI_S, 3);
  // return;


  if(TSU_mode != 0) {  // 0 = draw OpenGL
    if(typ == 0) {
      i1 = 3;  // planarer fan
      if(newS) GLT_stor_rec (6, NULL, NULL, Typ_SURPLN);
    } else {
      i1 = 4;  // nichtPlanarer Fan
      if(newS) GLT_stor_rec (6, NULL, NULL, Typ_SUR);
    }
    GLT_stor_rec (i1, pt1, pa1, ptNr);
    return;
  }


/*
  // Linux: Normals gehen nicht !
  glBegin (GL_TRIANGLE_FAN);

    GRU_calc_normal(&GL_norm, &pt1, &pa1[0], &pa1[1]);
    glNormal3dv (&GL_norm);

    glVertex3dv (pt1);
    glVertex3dv (&pa1[0]);
    glVertex3dv (&pa1[1]);

    for(i1=2; i1<ptNr; ++i1) {

      // GRU_calc_normal(&GL_norm, &pt1, &pa1[i1-1], &pa1[i1]);
      // glNormal3dv (&GL_norm);

      glVertex3dv (&pa1[i1]);

    }

  glEnd ();
*/




  // Variante in Dreicke zerlegen und Normalvektor
  glBegin (GL_TRIANGLES);

    if(typ < 1) {
      GRU_calc_normal(&GL_norm, pt1, &pa1[0], &pa1[1]);
      // GRU_calc_normal(&GL_norm, pt1, &pa1[1], &pa1[0]);
    }


    for(i1=0; i1<ptNr-1; ++i1) {
    // for (i1=hNr-2; i1>=0; --i1) {

      // Richtung 1
      if(typ > 0) {
        GRU_calc_normal(&GL_norm, pt1, &pa1[i1], &pa1[i1+1]);
        // GRU_calc_normal(&GL_norm, pt1, &pa1[i1+1], &pa1[i1]);
      }

      glNormal3dv ((double*)&GL_norm);

      glVertex3dv ((double*)pt1);
      glVertex3dv ((double*)&pa1[i1]);
      glVertex3dv ((double*)&pa1[i1+1]);


    }

  glEnd ();







  //-----------------------------------------------------------------
  // zusaetzl. noch Boundarygrenzen hinmalen
  if(WC_stat_bound == ON) {
    glDisable (GL_LIGHTING);

    glCallList (DL_base_LnAtt + Typ_Att_Symb);

    // glDepthFunc (GL_ALWAYS);   // ueberschreibt immer (auch Flaechen !)

    glBegin (GL_LINES);


    glVertex3dv ((double*)pt1);
    for(i1=0; i1<ptNr-1; ++i1) {
      glVertex3dv ((double*)&pa1[i1]);
    }
    glVertex3dv ((double*)pt1);

    glEnd ();



    // glDepthFunc (GL_LEQUAL);              // reset
    glEnable (GL_LIGHTING);
  }

}


//==================================================================
  void GL_DrawFtab (Point *pTab, Fac3 *fTab, int fNr, ColRGB *col) {
//==================================================================
// Input:
//   fNr      nr of faces in fTab
//   col      .vsym: 0=ON=shade; 1=OFF=symbolic
// 
// TODO: texture; see TSU_DrawSurMsh (make function for both)

 
  int      ii, i1, i2, i3;
  char     glCol[4];
  Vector   GL_norm;

  // printf("GL_DrawFtab %d\n",fNr);
  // printf(" TSU_mode=%d styl=%d\n",TSU_mode,styl);


  // see GL_DrawFan & GL_DrawStrip2
  if(TSU_mode != 0) {  // 0 = draw OpenGL
    // store for export
    // GLT_stor_rec (6, NULL, NULL, Typ_SURMSH);
    GLT_stor_rec (6, NULL, NULL, Typ_SUR);
    GLT_stor_rec (7, pTab, fTab, fNr);
    return;
  }


  // normal; display via OpenGL
  glBegin (GL_TRIANGLES);


    for(ii=0; ii<fNr; ++ii) {
      i1 = fTab[ii].i1;
      i2 = fTab[ii].i2;
      i3 = fTab[ii].i3;

      if(col->vsym == 0) {
        // Richtung 1
        GRU_calc_normal(&GL_norm, &pTab[i1], &pTab[i2], &pTab[i3]);
        // GRU_calc_normal(&GL_norm, pt1, &pa1[i1+1], &pa1[i1]);
        glNormal3dv ((double*)&GL_norm);
      }

      glVertex3dv ((double*)&pTab[i1]);
      glVertex3dv ((double*)&pTab[i2]);
      glVertex3dv ((double*)&pTab[i3]);

    }

  glEnd ();


  if(col->vsym == 1) {  // 1=OFF=symbolic - reset
    // glEnable (GL_LIGHTING);
    // glEnable (GL_COLOR_MATERIAL);
    // glEnable (GL_BLEND);
    // glEnable (GL_LINE_STIPPLE);
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    // glPopAttrib ();
  }




  //-----------------------------------------------------------------
  // zusaetzl. noch Boundarygrenzen hinmalen
/*
  if(WC_stat_bound == ON) {
    ..
  }
*/


  return;

}


//================================================================
  int GL_DrawSur (long *ind, int att, ObjGX *bMsh) {
//================================================================
// display surface
// Input:
//   ind        dbi
//   att        ColRGB*   color|texture
//   bMsh       binMsh (tesselated surf)
//
// see also GRU_tex_pos2 < TSU_DrawSurTRU
// see also TSU_DrawSurTess


  int    irc, iTex;
  TexBas *btex;
  TexRef *rtex;
  int    iTexBas, iTexRef;


  // printf("GL_DrawSur %ld vtex=%d\n",*ind,((ColRGB*)&att)->vtex);
  // UT3D_stru_dump (Typ_Color, &att, "  att:");

  

  // init GL-obj Surf; mit od ohne texture ..
  // iTex = IF_TEXURE(att);
    // // printf(" iTex=%d\n",iTex);

  // if(iTex != 0) {
  if(((ColRGB*)&att)->vtex != 0) {
    // extract TexRef-RecordNr from iAtt
    // iTexRef = ((stru_c3c1*)&att)->b123;
    iTexRef = GA_getTexRefInd (att);
    // get TexRef-Record
    Tex_getRef (&rtex, iTexRef);
    // get TexBas-Record
    iTexBas = rtex->ibas;
    Tex_getBas__ (&btex, iTexBas);
    // nur wenn vx=NULL (= uninitialized): vx,vy errechnen ..
    if(UT3D_compvcNull(&rtex->vx) == 1) {
      // Vektoren vx/vy, Scales scx/scy und  Offsets dx/dy setzen.
      GRU_tex_pos1 (rtex, bMsh);
    }
    // init texture
    GL_Tex_Ini (ind, btex, rtex);
    irc = GL_Disp_sur (bMsh);
    GL_Tex_End ();

  } else {
    GL_Surf_Ini (ind, (void*)&att);
    irc = GL_Disp_sur (bMsh);
    GL_EndList ();
  }

/*
  irc = GL_Disp_sur (os);

  if(iTex != 0) GL_Tex_End ();
  else          GL_EndList ();
*/

  return irc;

}


//================================================================
  int GL_Disp_sur (ObjGX *bMsh) {
//================================================================
// Draw 1-n Planar Patches; each Patch with 1 vektor and 1-n closed Contours.
// Input: 
//   bMsh     binMsh (tesselated surf)
//
// ContourTypes: 4=GL_TRIANGLES 5=GL_TRIANGLE_STRIP 6=GL_TRIANGLE_FAN
// bMsh->typ must be Typ_GL_Sur.
//
// bMsh (a binary mesh) can have a single patch or 1-n surfaces.
//   Each surface can have a color, a vector, 1-n patches.
//     Each patch has 3-n points and a triangle-type.
//      The points are triangles or triangle-fans or -stripes acoording to type.
//
//          typ        form       aux
// bMsh:    Typ_GL_Sur Typ_ObjGX
// surface: Typ_GL_PP  Typ_ObjGX
// patch:   Typ_PT     Typ_PT     GL_TRIANGLES|GL_TRIANGLE_STRIP|GL_TRIANGLE_FAN
//
// Examples:
//
//  bin.Mesh
//         surfaces
//                   objects  vector/color/faces
//         
//  bMsh + bSur[0] + bObj[0]  vector
//       |         + bObj[1]  patch (pp1)
//       |         + bObj[2]  patch (pp2)
//       |
//       + bSur[1] + bObj[3]  color
//                 + bObj[4]  vector
//                 + bObj[5]  patch (pp3)



//
// Es ist folgende Struktur erforderlich:
//
//      typ     siz 
//          form  data
//      210 143 2 a1     OX      Sur of 2 patches
// a1   211 143 3 a2     OX        patch 1 with 3 contours
//      211 143 4 a3     OX        patch 2 with 4 contours
// a2     5 5 1 a4                   Contour 1 (of Patch 1 of Sur) - vec
//        2 2 5 a5                   Contour 2 (of Patch 1 of Sur) - 5 points
//        2 2 5 a6                   ...
// a3     5 5 1 a7
//        2 2 5 a8
//        2 2 5 a9
//        2 2 5 a10
// a4 (strut VC)
// a5 (strut PT * 5)

// 210 = Typ_GL_Sur      (Surf)
// 211 = Typ_GL_PP       (Patch)
// 143 = Typ_ObjGX
//   5   Typ_AC
//   2   Typ_PT

// Es muss (wenn nicht in den Daten enthalten) ein Normalvektor erzeugt
//   und ausgegeben werden !

// Resolv: see also GRU_tex_pos1 UTO_decPt_GLsur







  int     i1, i2, pNr, pAct, cNr, cAct, ptNr, gTyp, iCol, iTex;
  Point   *pa;
  Vector  vc1, vc2, vc3;
  ObjGX   *actPP, *actCont;
  GLuint  dlInd;
  // ColRGB  *pCol, xCol;

  unsigned char    cr, cg, cb;


  // pCol = NULL;
  iCol = -1;


  // nr of surfaces
  pNr =  bMsh->siz;

  // printf("SSSSSSSSSSSSSSSSSSSSSSS GL_Disp_sur %d patches\n",pNr);
  // UTO_dump__ (bMsh, "GL_DrawSur");
  // UTO_dump_s_ (bMsh, "GL_DrawSur");
/*
  // for(pAct=0; pAct<pNr; ++pAct) {
    // actPP = &((ObjGX*)bMsh->data)[pAct];
    // cNr = actPP->siz;
    // for(cAct=0; cAct<cNr; ++cAct) {
      // printf("%d ",((ObjGX*)actPP->data)[cAct].siz);
    // }
  // }
  // printf("\n");
  // return 0;
*/
  
  // nur ein Col-Rec; bei tess wird defCol gleich als erster Rec ausgegeben ..
  if(bMsh->typ == Typ_Color) {
      // UT3D_stru_dump (Typ_Color, &bMsh->data, "_DrawSurC1:");
    // glColor3ubv ((unsigned char*)&bMsh->data);
    GL_ColSet ((ColRGB*)&bMsh->data);
    iCol = 1;
    if(bMsh->siz != 1) printf("GL_Disp_sur E001\n");
    return 0;
  }



  //---------------------------------------------------
  // loop tru surfaces
  for(pAct=0; pAct<pNr; ++pAct) {

    // get current-surface
    actPP = &((ObjGX*)bMsh->data)[pAct];


    // get nr of patches in current-surface
    cNr = actPP->siz; // Anzahl contours of current Patch
      // printf(" patch %d: typ=%d form=%d siz=%d aux=%d\n",pAct,
             // actPP->typ,actPP->form,cNr,actPP->aux);


    //---------------------------------------------------
    if(actPP->form == Typ_PT) {
      pa = actPP->data;
      if(iCol < 1) GL_ColSet (&GL_defCol);
      if(UT3D_vc_perppta(&vc1, cNr, pa) < 0) continue;
      glNormal3dv ((double*)&vc1);
        // GL_Disp_vc (&vc1, pa, 11);
        // UT3D_vc_setLength (&vc1, &vc1, 1.);
        // UT3D_stru_dump (Typ_VC, &vc1, "_DrawSur0: vc");
      GL_Disp_face (actPP->aux, cNr, pa);
      continue;


    //---------------------------------------------------
    } else if(actPP->form == Typ_SPH) {
      // UT3D_stru_dump (Typ_SPH, actPP->data, "");
      GL_disp_sph (&((Sphere*)actPP->data)->pc,
                    ((Sphere*)actPP->data)->rad, APT_dispSOL);   // sStyl
      continue;


    //---------------------------------------------------
    } else if(actPP->form == Typ_CON) {
      // UT3D_stru_dump (Typ_SPH, actPP->data, "");
      GL_disp_cone ((Conus*)actPP->data, 7, APT_dispSOL);
      continue;


    //---------------------------------------------------
    } else if(actPP->form == Typ_Int4) {
      if(actPP->typ == Typ_Texture) {
        iTex = INT_PTR (actPP->data);
        // yet unused ..
        continue;
      } else if(actPP->typ == Typ_Color) {
        GL_ColSet ((ColRGB*)&actPP->data);
        iCol = 1;
        continue;
      } else if(actPP->typ == Typ_Typ) {
        // markiert den Beginn einer neuen Surf - dzt nur f Intersect used
        continue;
      }

    }


    //---------------------------------------------------
    if(actPP->form != Typ_ObjGX) {
      TX_Error("GL_Disp_sur E001 %d %d",actPP->typ,actPP->form);
      // UTO_dump__ (bMsh, "GL_DrawSur");
      return -1;
    }
      // printf(" _Disp_sur typ=%d\n",actPP->typ);



    //---------------------------------------------------
    // loop tru patches in current-surface (typ=Typ_GL_PP, form=Typ_ObjGX)
    for(cAct=0; cAct<cNr; ++cAct) {

      actCont = &((ObjGX*)actPP->data)[cAct];
      // printf(" pAct=%d cAct=%d typ=%d form=%d siz=%d\n",pAct,cAct,
               // actCont->typ, actCont->form, actCont->siz);

      //---------------------------------------------------
      if(actCont->form == Typ_VC) {
        // Normalvektor
          // UT3D_stru_dump (Typ_VC, actCont->data, "_DrawSur2: c%d",cAct);
          // vc1 = *((Vector*)actCont->data);  // NUR TEST f GR_Disp_vc
          // UT3D_stru_dump (Typ_VC, &vc1, "");
        glNormal3dv ((double*)actCont->data);
        continue;

      //---------------------------------------------------
      } else if(actCont->typ == Typ_Color) {
          // UT3D_stru_dump (Typ_Color, &actCont->data, "_DrawSurC3: c%d",cAct);
        GL_ColSet ((ColRGB*)&actCont->data);
        iCol = 1;
        continue;

      //---------------------------------------------------
      } else if(actCont->typ == Typ_Typ) {
        // markiert den Beginn einer neuen Surf - dzt nur f Intersect used
        continue;

      //---------------------------------------------------
      } else if(actCont->typ == Typ_PT) {
        // draw next contour in current patch
        // typ & form is Typ_PT; 
            // UT3D_vc_setLength (&vc1, &vc1, 1.);
            // GL_Disp_vc (&vc1, actCont->data, 9);
        if(iCol < 1) GL_ColSet (&GL_defCol);
        GL_Disp_face (actCont->aux, actCont->siz, actCont->data);
        continue;


      //---------------------------------------------------
      } else if(actCont->typ == Typ_Texture) {
        // ?  Haus1
        continue;

      //---------------------------------------------------
      } else {
        printf("***** GL_Disp_sur unused typ=%d form=%d\n",
               actCont->typ,actCont->form);
      }

    }   // end contour
    //---------------------------------------------------

  }   // end patch
  //---------------------------------------------------


  // if(pCol) {
    // if(pCol->vtra != 0) {
      // glDisable (GL_BLEND);
    // }
  // }

  return 0;
}


//================================================================
  int GL_ColSet (ColRGB *col) {
//================================================================
// GL_ColSet                 activate Color,transparency for surface from ColRGB

  int               iTra, iSym; 
  unsigned char     glCol[4];



  // printf("GL_ColSet WC_modact_ind=%d\n",WC_modact_ind);
  // UT3D_stru_dump (Typ_Color, pCol, "GL_ColSet ");
  // printf("GL_ColSet vtra=%d WC_modact_ind=%d\n",pCol->vtra,WC_modact_ind);

  memcpy(glCol, col, 3); 

  iSym = col->vsym;
  iTra = col->vtra;
    // printf("GL_ColSet iTra=%d iSym=%d\n",iTra,iSym);



  //------------------SURF-SYMBOLIC---------------------------------
  if(iSym) {
    // 1 = symbolic
    glDisable (GL_LIGHTING);
    glDisable (GL_BLEND);
    glCallList (DL_base_LnAtt); // needed for ability to hilite symbolic surf
    // glColor3ubv ((unsigned char*)&GL_defCol);
    // glLineWidth   (1.0);
    // glColor3fv   (GL_col_tab[0]);  // black
    // glColor3ubv  (glCol); 
    glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);   // draw lines, not faces
    return 0;
  }


  //---------------SURF-SHADED_NOT_TRANSPARENT----------------------
  if(!iTra) {
   // 0 = no transparency

    glDisable (GL_BLEND); // to overwrite transparency 2013-08-20

    if(col->color == 0) {
        // UT3D_stru_dump (Typ_Color, &GL_defCol, "GL_Surf_Ini-GL_defCol:");
      glColor3ubv ((unsigned char*)&GL_defCol);
    } else {
      glColor3ubv (glCol);
    }

    return 0;
  }


  //---------------SURF-SHADED_AND_TRANSPARENT----------------------
    // 1 = surf is transparent
    // printf(" glEnable_GL_BLEND\n");
    // glEnable (GL_LIGHTING);
    // glEnable (GL_COLOR_MATERIAL);
    // glEnable (GL_DEPTH_TEST);
    // glDepthFunc (GL_LEQUAL);   loescht den Z-buffer ?
    // glDepthFunc (GL_NOTEQUAL); GL_ALWAYS immer Hintergrund !
    glEnable (GL_BLEND);
    // glBlendFunc (GL_SRC_ALPHA, GL_ONE);    // geht auch
    // glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);    // geht auch
    // glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);   // geht 

    // glBlendFunc (GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);   // no ..
    // glBlendFunc (GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);   // no ..

    if(WC_modact_ind >= 0)         // nur bei 'make subModel'
      glDisable (GL_DEPTH_TEST); // nur damit immer (in SM) durchsichtig !
    // glBlendFunc (GL_SRC_ALPHA, GL_ONE);   // geht 
    // glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);   // geht 
    // glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);    // geht; aber zu hell !

    glCol[3] = GL_transpTab[col->vtra];   // 0=glass; 255=rigid;
    glColor4ubv (glCol);
    GL_stat_blend = 1;


  return 0;

}


//================================================================
  int GL_Disp_face (int gTyp, int ptNr, Point *pa) {
//================================================================
// gTyp: 4=GL_TRIANGLES 5=GL_TRIANGLE_STRIP 6=GL_TRIANGLE_FAN
// unused: 0=GL_POINTS; 1=GL_LINES; 2=GL_LINE_LOOP ?
//   3=GL_LINE_STRIP ? 0=GL_POLYGON
// prepare with glNormal3dv
// default sense-of-rotation for OpenGL-frontside is CCW, CW for rear.
// see also GL_Disp_iface

  int  i1;

  // printf("GL_Disp_face pNr=%d gTyp=%d\n",ptNr,gTyp);
  // for(i1=0;i1<ptNr;++i1) UT3D_stru_dump (Typ_PT, &pa[i1], "P[%d]",i1);
  // GR_Disp_pTab (ptNr, pa, SYM_STAR_S, 2);
  //// gTyp=6=GL_TRIANGLE_FAN


  if(ptNr < 3) {
    // printf(" *************** GL_Disp_face E001 *******\n");
    return 0;
  }


  glBegin (gTyp);
  for(i1=0; i1<ptNr; ++i1) {
    glVertex3dv ((double*)&pa[i1]);
  }

  glEnd ();

  return 0;
}


//====================================================================
  int GL_Disp_iface (int gTyp, int iNr, int *ia, Point *pa) {
//====================================================================
// display face from  indexed-points
// without glNewList
// Input:
//   gTyp: type of triangles in ia; eg GL_TRIANGLES
//         bitVal-16 set: face is planar; else not.
//   ia[iNr]  indices into pa
//   pa       all points
// OpenGL cannot create normals automatic.
// prepare with glNormal3dv
//
// gTyp:
// 6=GL_TRIANGLE_FAN
//    1---2
//    |  / \
//    | /   \
//    0-----3
//     \   /
//      \ /
//       4
//
// see also GL_Disp_face
// see also TSU_exp_stlFac TSU_exp_objFac TSU_exp_wrl1Fac TSU_exp_wrl2Fac
//     TSU_exp_dxf1Fac


  int      i1, i2, i3, ii, ii1, ii2, ii3, iPln;
  Vector   vcn;

  // printf("GL_Disp_iface gTyp=%d iNr=%d\n",gTyp,iNr);
  // for(i1=0;i1<iNr;++i1) { i2 = ia[i1];
   // printf(" %d P[%d]=%lf,%lf,%lf\n",i1,i2,pa[i2].x,pa[i2].y,pa[i2].z); }
  //// gTyp=6=GL_TRIANGLE_FAN


  // get bit planar (val=16)
  iPln = BIT_GET (gTyp, 16);   // 0=face not planar; 16=planar
  gTyp = BIT_GET (gTyp, 15);   // 1+2+4+8
    // printf(" gTyp=%d iPln=%d\n",gTyp,iPln);




  if(iNr < 3) {
    // printf(" *************** GL_Disp_face E001 *******\n");
    return 0;
  }

  // glEnable(GL_CULL_FACE);    // makes backside invisible !
  // glEnable(GL_NORMALIZE);    // scale normalVectors to  unit  length


  glBegin (gTyp);


  //----------------------------------------------------------------
// 4=GL_TRIANGLES
//    0--2  3--5      
//    | /   | / 
//    |/    |/  
//    1     4   
//
  if(gTyp == GL_TRIANGLES) { // 4
    if(iPln) {
      UT3D_vc_perp3pt (&vcn, &pa[ia[0]], &pa[ia[1]], &pa[ia[2]]);
      glNormal3dv ((double*)&vcn);    // normalVec
    }
    ii = 0;

    L_4_nxt:
      i1 = ia[ii++];
      i2 = ia[ii++];
      i3 = ia[ii++];
      
      if(!iPln) {
        UT3D_vc_perp3pt (&vcn, &pa[i1], &pa[i2], &pa[i3]);
        glNormal3dv ((double*)&vcn);    // normalVec
      }
      glVertex3dv ((double*)&pa[i1]);
      glVertex3dv ((double*)&pa[i2]);
      glVertex3dv ((double*)&pa[i3]);
      if(ii < iNr) goto L_4_nxt;

    goto L_end;
  }


  //----------------------------------------------------------------
// 5=GL_TRIANGLE_STRIP
//    0--2--4--6      
//    | /| /| /|
//    |/ |/ |/ |
//    1--3--5--7
//// sollte so zerlegt werden:
// 0 1 2                       0 2 1
// 2 1 3                       2 3 1
// 2 3 4                       2 4 3
// 4 3 5
// 4 5 6   usw ...

  if(gTyp == GL_TRIANGLE_STRIP) { // 5
    ii1 = ia[0];
    ii2 = ia[1];
    ii3 = ia[2];
      // printf(" ii1=%d ii2=%d ii3=%d\n",ii1,ii2,ii3);
    UT3D_vc_perp3pt (&vcn, &pa[ii1], &pa[ii2], &pa[ii3]);
    glNormal3dv ((double*)&vcn);    // normalVec
    glVertex3dv ((double*)&pa[ii1]);
    glVertex3dv ((double*)&pa[ii2]);
    glVertex3dv ((double*)&pa[ii3]);

    if(iPln) {  // planar
      for(i1=3; i1<iNr; ++i1) {
        ii3 = ia[i1];
        glVertex3dv ((double*)&pa[ii3]);
      }
      goto L_end;
    }

    // not planar
    i3 = 2;
    L_5_nxt:
      ii1 = ii3;
      // ii2 unmodif.
      ++i3;
      if(i3 >= iNr) goto L_5_end;
      ii3 = ia[i3];
        printf(" ii1=%d ii2=%d ii3=%d\n",ii1,ii2,ii3);
      // UT3D_vc_perp3pt (&vcn, &pa[ii1], &pa[ii2], &pa[ii3]);
      // glNormal3dv ((double*)&vcn);    // normalVec
      glVertex3dv ((double*)&pa[ii3]);

      // ii1 unmodif.
      ii2 = ii3;
      ++i3;           // 3 2 4
      if(i3 >= iNr) goto L_5_end;
      ii3 = ia[i3];
        // printf(" ii1=%d ii2=%d ii3=%d\n",ii1,ii2,ii3);
      // UT3D_vc_perp3pt (&vcn, &pa[ii1], &pa[ii2], &pa[ii3]);
      // glNormal3dv ((double*)&vcn);    // normalVec
      glVertex3dv ((double*)&pa[ii3]);
      goto L_5_nxt;

    L_5_end:
    goto L_end;
  }



  //----------------------------------------------------------------
  for(i1=0; i1<iNr; ++i1) {
    ii = ia[i1];
    glVertex3dv ((double*)&pa[ii]);
      // UT3D_stru_dump (Typ_PT, &pa[ii], NULL);
  }


  //----------------------------------------------------------------
  L_end:
  glEnd ();

  return 0;
}


//================================================================
  int GL_Draw_obj (long *dli, int att, ObjGX *oxi) {
//================================================================
// Draw 1-n Polygons as GL_LINE_STRIP's.
// Does not create the DL-Record.
// Since pta is a surface,
// att = Linetyp; for surfs Typ_Att_Fac1
// Typ_SUR muss vsym gesetzt haben, sonst kein Hilite !
// Typ_CV geht so auch ..
//
// MMM (MemoryManagment):
// Memseg & OgxTab im Stack, Daten im memspc.
// see also GL_Disp_sur GL_tst_pta
//
// Example 2 polygons; first polygon has 3 points; second has 5 points.
//    212 143 2 a1
// a1     2   2  3 a2
//        2   2  5 a3
// a2     (data 3 points)
// a3     (data 5 points)
//
//
//   2 = Typ_PT
// 143 = Typ_ObjGX
// 212 = Typ_GL_CV   (GL-Polygon)

  int   i1;
  ObjGX *ox1;
  // Point *pta;



  // printf("GL_Draw_obj \n");
  // UTO_dump__ (oxi, "GL_Draw_obj");
  

  GL_Draw_Ini (dli, att);  // init Linetype

  glDisable (GL_LIGHTING);

  for(i1=0; i1<oxi->siz; ++i1) {
    ox1 = &((ObjGX*)oxi->data)[i1];
    // pta = ox1->data;
    // ox1->form muss hier Typ_PT sein ..
    GL_Disp_cv (ox1->siz, ox1->data);
  }

  glEndList ();

  return 0;

}


//=====================================================================
  int GL_Draw_BMP (long *dli, AText *tx1, long dbi) {
//=====================================================================
// Bitmap wird direkt ins OpenGL kopiert (mit glDrawPixels)
// Es wird keine Textur benutzt.
// Input:
//   p1       links unten; hier wird positioniert
//   p2       endpkt Leader
//   ltyp     -1=keineLeaderline; else typ of Leaderline
//   scl      0: skaliert mit Zoom; else immer gleiche Size


  int    irc, ix, iy, pixSiz, ltyp;
  GLuint dlInd;
  float  x1, y1, f_scl;
  double scl;
  char   *symNam, bNam[256], sSym[64], sDir[128];
  void   *mSpc;
  Point  *p1, *p2;


  symNam = tx1->txt;  // ImageFilename
  scl    = tx1->scl;
  ltyp   = tx1->ltyp; // Linetyp Leaderline; -1=no Leaderline
  p1     = &tx1->p1;
  p2     = &tx1->p2;

  printf("GL_Draw_BMP |%s| dli=%ld scl=%f dbi=%ld\n",symNam,*dli,scl,dbi);


  // provide BitmapFile for <symNam> as file <bNam> in <tmpDir>
  irc = Tex_getBitmap (bNam, symNam, 0);
  if(irc < 0) {
    TX_Error("ERROR GL_Draw_BMP %d %s",irc,symNam);
    pixSiz = -1;
    // message symbolic path <symDir> is <path>
    Mod_sym_get__ (sSym, sDir, bNam, symNam);
    TX_Print (" - symbolic path \"%s\" is \"%s\"", sSym, sDir);
    goto L_err_img;
  }


  // Bitmapdaten --> mSpc
  pixSiz = bmp_load (&mSpc, &ix, &iy, bNam);
  // if(pixSiz < 0) return -1;


  L_err_img:
  if(pixSiz < 0) {
    sprintf(bNam, "%sNO_IMAGE32x32_24.bmp",OS_get_ico_dir());
    pixSiz = bmp_load (&mSpc, &ix, &iy, bNam);
    if(pixSiz < 0) return -1;
    scl = 1.;
  }



  // check for max textureSize
  irc = GL_Tex_ckSiz (ix, iy);
    // printf(" n GL_Tex_ckSiz %d\n",irc);
  if(irc) {
    TX_Error("cannot load texure %s - too big; reduce size or resolution",bNam);
    free (mSpc);
    return -1;
  }


  dlInd = GL_fix_DL_ind (dli);
  // printf(" dli=%d\n",dlInd);


  // tags and Bitmaps: store size
  // DL_setTagSiz (*dli, ix, iy);
  tx1->xSiz = ix;
  tx1->ySiz = iy;


  glNewList (dlInd, GL_COMPILE);

  // damit man mit Lines die Bitmap ueberschreiben kann ...
  glDepthFunc (GL_LESS);

  // glPixelStorei (GL_UNPACK_ALIGNMENT, 4);
  // glPixelStorei (GL_PACK_SKIP_ROWS, 0);
  // glPixelStorei (GL_PACK_SKIP_PIXELS, 0);

  glRasterPos3d (p1->x, p1->y, p1->z);
  // glRasterPos2f (-50.f,   25.f);
  // glRasterPos2i (150, 50);

  glPushMatrix ();

  if(fabs(scl) > UT_TOL_min1) {
    f_scl = scl;
    glPixelZoom (f_scl, f_scl);
  } else {
    glCallList (DL_Img_ScBack);  // scale;
  }


  // // verschieben auf links mitte
  // // geht ned, weil ma den Scalefaktor ned dynamisch veraendern kann.
  // x1 = 0.;
  // y1 = -(iy*GL_Scale) / 2;
  // // y1 = -iy / 2;
  // glBitmap (0,0, 0.f,0.f, x1,y1, NULL);



  // draw bitmap

  if(pixSiz == 3) {
    // for 24-bit-colour:
    glDrawPixels (ix, iy,           // width, height
                 GL_BGR, //GL_BGR_EXT,    // format; GL_RGB ist farblich falsch !
                 GL_UNSIGNED_BYTE,
                 mSpc);             // data

  } else {
    // for 8-bit-colour:
    glDrawPixels (ix, iy,           // width, height
                 GL_LUMINANCE,      //GL_GREEN,
                 GL_UNSIGNED_BYTE,
                 mSpc);             // data
  }




  //----------------------------------------------------------------
  // Leader -1=ohne Leaderline;
  if(ltyp < 0) goto L_99;
  if(ltyp < 1) ltyp = 1;
  glCallList (DL_base_LnAtt + ltyp);
  glBegin (GL_LINES);
    glVertex3dv ((double*)p1);
    glVertex3dv ((double*)p2);
  glEnd ();



  //----------------------------------------------------------------
  L_99:
  // glEnable (GL_LIGHTING);

  glDepthFunc (GL_LEQUAL); // reset ...

  glPopMatrix ();  // vom ScaleBack

  glEndList ();

  free (mSpc);

  // printf("ex GL_Draw_BMP\n");


  return 0;

}


//==================================================================
  void GL_DrawSymB (long *ind, int attInd, int symTyp, Point *pt1){
//==================================================================
/// \code
/// display BITMAP-Symbol (SYM_TRI_S SYM_STAR_S ..)
/// Input:
///   ind        nr of dispListRecord; see DL_StoreObj or DL_SetObj
///   attInd     Color ATT_COL_BLACK ATT_COL_RED ATT_COL_GREEN .. (../ut/func_types.h)
///   symTyp     type of symbol (../ut/func_types.h)
///              SYM_TRI_S   Dreieck
///              SYM_STAR_S  Sternderl
///              SYM_CIR_S   Kreis klein
///              SYM_TRI_B   Viereck
/// \endcode



  GLuint dlInd;


  // printf("GL_DrawSymB ind=%ld att=%d Typ=%d %f,%f,%f\n",
          // *ind,attInd,symTyp,pt1->x,pt1->y,pt1->z);


  // den DL-Index (+ Offset) holen)  
  dlInd = GL_fix_DL_ind (ind);

  glNewList (dlInd, GL_COMPILE);

  glDisable (GL_LIGHTING);

  // glCallList (DL_base_LnAtt + Typ_Att_Symb);
  glColor3fv    (GL_col_tab[attInd]);

  glRasterPos3d (pt1->x, pt1->y, pt1->z);
  glCallList ((GLuint)symTyp);

  glEnable (GL_LIGHTING);

  glEndList ();


}



//***********************************************************************
  void GL_DrawSymV (long *ind, int symTyp, int att, Point *pt1, double scale){
//***********************************************************************
/// \code
/// GL_DrawSymV   display symbols in x-y-plane;
///   eg SYM_TRIANG, SYM_AXIS, SYM_SQUARE, SYM_CROSS
/// \endcode


  GLuint dlInd;


  // printf("GL_DrawSymV %ld %d\n",*ind,symTyp);
  // printf("  pos=%f,%f,%f sc=%f\n",pt1->x,pt1->y,pt1->z,scale);



  // den DL-Index (+ Offset) holen)  
  dlInd = GL_fix_DL_ind (ind);



  glNewList (dlInd, GL_COMPILE);

    glDisable (GL_LIGHTING);

    // glCallList (DL_base_LnAtt + Typ_Att_Symb);
    glCallList (DL_base_LnAtt + att);

    glPushMatrix ();

    // Transl und Scale: Reihenfolge wichtig !
    glTranslated (pt1->x, pt1->y, pt1->z);
    glScaled (scale, scale, scale);

    glCallList ((GLuint)symTyp);
    glPopMatrix ();

    glEnable (GL_LIGHTING);

  glEndList ();


  //if(*ind < 0)  glCallList (dlInd);

}




//***********************************************************************
  void GL_DrawSymV2 (long *ind, int symTyp, int att,
                     Point *pt1, Point *pt2, double scale){
//***********************************************************************
/// \code
/// GL_DrawSymV2    display oriented symbols in x-y-plane; eg SYM_ARROH
/// for 3D use ?
/// \endcode


  double  d1;
  Point2  p20, p21;
  GLuint  dlInd;


  // printf("GL_DrawSymV2 %ld Typ=%d %f,%f  %f,%f\n",*ind,symTyp,
         // pt1->x,pt1->y,pt2->x,pt2->y);

  scale = 1.;

  p20 = UT2D_pt_pt3 (pt1);
  p21 = UT2D_pt_pt3 (pt2);
  d1  = UT_DEGREES(UT2D_angr_ptpt(&p20, &p21));
  //TX_Print(" ang=%f",d1);




  // den DL-Index (+ Offset) holen)  
  dlInd = GL_fix_DL_ind (ind);

  glNewList (dlInd, GL_COMPILE);

    glDisable (GL_LIGHTING);

    // glCallList (DL_base_LnAtt + Typ_Att_Symb);
    glCallList (DL_base_LnAtt + att);


    glPushMatrix ();

    // Transl und Scale: Reihenfolge wichtig !
    glTranslated (pt1->x, pt1->y, pt1->z);
    glRotated (d1, 0.0, 0.0, 1.0);
    glScaled (scale, scale, scale);

    glCallList ((GLuint)symTyp);
    glPopMatrix ();

    glEnable (GL_LIGHTING);

  glEndList ();


  //if(*ind < 0)  glCallList (dlInd);

}


/* replaced by GL_DrawAngA
//=============================================================================
  int GL_DrawCirSc (long *ind, int att, Point *ptc, Vector *vc1, double ang1) {
//=============================================================================
// draw scaled circsegment
// vc1 muss normiert sein !!
// Input:
//   ang1   rad

  int    i1, ptNr, ianz;
  double d1, d2, az, ay;
  Vector vcn;
  Point  pt1;

  UT3D_stru_dump(Typ_VC, vc1, "GL_DrawCirSc a=%f vc=",ang1);
  // GR_Disp_vc (vc1, ptc, 2, 0);
  // GR_Disp_pt (ptc, SYM_STAR_S, 2);

  ianz = 1024; // max

  if(GLT_pta_SIZ < ianz) {
    i1 = GLT_alloc_pta (ianz);
    if(i1 == -1) return -1;
  }

  // wir brauchen nun einen Normalvektor auf vc1;
  UT3D_vc_perp1vc (&vcn, vc1);

  GLT_pta[0] = *ptc;

  // Startpt cir
  UT3D_pt_traptvclen (&pt1, ptc, &vcn, 200./GL_Scale*GL_SclNorm); // GL_Scale
    printf(" lenVec=%f %f %f\n",200./GL_Scale*GL_SclNorm,GL_Scale,GL_SclNorm);

  GLT_pta[1] = pt1;
  // GR_Disp_pt (&GLT_pta[0], SYM_STAR_S, 2);

  ptNr = fabs(ang1) / (RAD_1 * 10.);
  d1 = ang1 / ptNr;
  d2 = d1;
  for(i1=0; i1<ptNr; ++i1) {
    UT3D_pt_rotptptvcangr (&GLT_pta[i1+2], &pt1, ptc, vc1, d2);
    // GR_Disp_pt (&GLT_pta[i1], SYM_STAR_S, 2);
    d2 += d1;
  }

  GLT_pta[ptNr+2] = *ptc;
  ptNr += 3;

  // das Poly GLT_pta darstellen 
  GL_DrawPoly (ind, att, ptNr, GLT_pta);

  GL_DrawLn_Ini (ind, att);
  GL_Disp_cv (ptNr, GLT_pta); // GL_LINE_STRIP

  // Pfeilspitze darstellen
  UT3D_vc_2pt (&vcn, &GLT_pta[ptNr-2], &GLT_pta[ptNr-3]);
  UT3D_2angr_vc (&az, &ay, &vcn);
  az = UT_DEGREES(az);
  ay = UT_DEGREES(ay);
  glLineStipple (1, (GLushort)0xFFFF);  // solid
  GL_Disp_symV (SYM_ARROH, &GLT_pta[ptNr-2], az, ay, 1.);

  glEndList ();

  return 0;

}


//=============================================================================
  int GL_DrawVc1 (long *ind, int att, Point *pti, Vector *vc1) {
//=============================================================================
// normierter vc: att=7, Laenge=10; else att=att.
// att see GR_Disp_vc
// pt1: NULL = screencenter !

  double vl;
  Point  pt1;


  // printf("GL_DrawVc1 %f,%f,%f\n",vc1->dx,vc1->dy,vc1->dz);


  if(pti == NULL) pt1 = GL_GetCen();         // Mittelpunkt Bildschirm
  else            pt1 = *pti;
  

  vl = UT3D_len_vc (vc1);

  if(UTP_comp2db(vl, 1., UT_TOL_pt)) {
    // att 7 = sw; Laenge 1
    // APT_disp_SymV3 (SYM_ARROW, 7, &pt1, &vc1, 10.);
    // APT_disp_Vec (7, (long)vi, &pt1, &vc1);
    // GR_Disp_vc (&vc1, &pt1, 7, 0);
    GL_DrawSymV3 (ind, SYM_ARROW, 12, &pt1, vc1, 20.);

  } else {
    // APT_disp_Vec (2, (long)vi, &pt1, &vc1);
    // GR_Disp_vc (&vc1, &pt1, 2, 1);
    GL_DrawVec (ind, att, &pt1, vc1);
  }


  return 0;

}
*/


//=============================================================================
  void GL_DrawVec (long *ind,int att,Point *pt1,Vector *vc1){
//=============================================================================
/// \code
/// draw vector with ist correct length;
/// for unified length use GL_DrawSymV3 (.. SYM_ARROW)
/// \endcode

// TODO:
// arrowhead zoom-back (always same size) ?

// Draw Vektor; immer mit der richtigen Laenge; for normiert use SYM_ARROW
//   (see UI_disp_vec1)
// GERICHTETE VEKTOR-Symbole.
// Vektorymbole: SYM_ARROW, ..
// Scale 0 = Standardlaenge.
// see GL_Disp_vc


  double  ay, az, scale;
  GLuint  dlInd;



  // printf("GL_DrawVec ind=%d att=%d p=%f,%f,%f  v=%f,%f,%f\n",*ind,att,
               // pt1->x,pt1->y,pt1->z, vc1->dx,vc1->dy,vc1->dz);


  scale = UT3D_len_vc (vc1) / 10.;  // der ARROW hat Laenge 10
    // printf(" scale=%f\n",scale);


  UT3D_2angr_vc (&az, &ay, vc1);

  // rad -> deg
  az = UT_DEGREES(az);
  ay = UT_DEGREES(ay);
  // printf("   az=%f ay=%f\n",az,ay);


  // den DL-Index (+ Offset) holen)
  dlInd = GL_fix_DL_ind (ind);

  glNewList (dlInd, GL_COMPILE);

    glDisable (GL_LIGHTING);

    glCallList (DL_base_LnAtt + att);

    glPushMatrix ();

    // Transl und Scale: Reihenfolge wichtig !
    glTranslated (pt1->x, pt1->y, pt1->z);
    glRotated (az, 0.0, 0.0, 1.0);  // um Z drehen
    glRotated (ay, 0.0, -1.0, 0.0);  // um Y drehen
    glScaled (scale, scale, scale);

    glCallList ((GLuint)SYM_VEC);
    glPopMatrix ();

    glEnable (GL_LIGHTING);

  glEndList ();

}


//================================================================
  int GL_Draw_tra (long *dli, int att, TraRot *tra) {
//================================================================
/// \code
/// GL_Draw_tra           display symbolic TraRot (rotation)
/// see also UI_disp_tra
/// \endcode

  double    a1;
  Point     pt1;
  Vector    vx, vz;


  printf("GL_Draw_tra %ld %d\n",*dli,att);
  UT3D_stru_dump (Typ_TraRot, tra, "TraRot:");


  // a1 = angle
  // Typ_TraRot  ((TraRot*)tra->data)
  a1 = tra->angr * -1.;        // umdrehen ! ?


  // get pt2 = origin of matrix
  UT3D_pt_m3 (&pt1, tra->ma);


  // get axis
  vz = tra->vz;
  // UT3D_vc_setLength (&vc1, &vc1, 1.);
    // UT3D_stru_dump (Typ_VC, &vc1, "vc1");


  // get vx from vz
  UT3D_vc_perp1vc (&vx, &vz);


  // draw
  GL_DrawAngA (dli, att, &pt1, &vx, &vz, &a1);


  return 0;

}

 
//========================================================================
   int GL_DrawAngA (long *ind, int att,
                     Point *ptc, Vector *vx, Vector *vz, double *ang1) {
//========================================================================
/// \code
/// GL_DrawAngA           draw angle with arrowhead
/// display angle between x-vec and y-vec, with direction.
///
/// Point     pt={0., 0., 0.};
/// Vector    vcx={1., 0., 0.};
/// Vector    vcy={0., 1., 0.};
/// double    d1;
/// long      dli = -2L;
/// d1 = RAD_90;
/// GL_DrawAngA (&dli, 12, &pt, &vcx, &vcy, &d1);
/// \endcode



  int    i1, ptNr;
  double d1, d2, rd1;
  Vector vcn, vcx, vcz;
  Point  pt1, *pta;



  // printf("GL_DrawAngA %f\n",*ang1);
  // UT3D_stru_dump (Typ_PT, ptc, "  ptc ");
  // UT3D_stru_dump (Typ_VC, vx, "  vcx ");
  // UT3D_stru_dump (Typ_VC, vz, "  vcz ");
  // GR_Disp_pt (ptc, SYM_STAR_S, 2);
  // GR_Disp_vc (vcx, ptc, 2, 0);
  // GR_Disp_vc (vcz, ptc, 5, 0);

  UT3D_vc_setLength (&vcx, vx, 1.);
  UT3D_vc_setLength (&vcz, vz, 1.);

  // Startpt cir
  // rd1 = GL_get_SclNorm (200.);
  rd1 = 260 * GL2D_Scale;
  UT3D_pt_traptvclen (&pt1, ptc, &vcx, rd1);
    // UT3D_stru_dump (Typ_PT, &pt1, "  pt1 ");
    // GR_Disp_pt (&pt1, SYM_STAR_S, 2);


  // compute nr of points for circSeg
  ptNr = fabs(*ang1) / (RAD_1 * 10.);
  if(ptNr < 1) ptNr = 1;
    // printf(" ptNr=%d\n",ptNr);


  // get mem for circSeg
  pta = MEM_alloc_tmp ((ptNr + 4) * sizeof(Point));   // 2012-01-17 - was 2


  // compute circSegPoints - rotate pt1
  d1 = *ang1 / ptNr;
  d2 = d1;
  for(i1=0; i1<ptNr; ++i1) {
    UT3D_pt_rotptptvcangr (&pta[i1+2], &pt1, ptc, &vcz, d2);
    // GR_Disp_pt (&pta[i1], SYM_STAR_S, 2);
    d2 += d1;
  }


  // copy first / last point
  pta[0] = *ptc;
  pta[1] = pt1;
  pta[ptNr+2] = *ptc;
  ptNr += 3;


  // display pta (startline, circSeg, endLine)
  GL_DrawLn_Ini (ind, att);
  GL_Disp_cv (ptNr, pta); // GL_LINE_STRIP


  // Pfeilspitze darstellen
  GL_Disp_arrh (&pta[ptNr-2], &pta[ptNr-3], &vcn);

  glEndList ();

  return 0;

}


//================================================================
  int GL_Disp_vc (Vector *vc1, Point *pt1, int att) {
//================================================================
// add normalized vector (SYM_ARROW) into open displist


// see GL_DrawVec
// Vector in wahrer Laenge: GR_Draw_vSym GL_DrawSymV3
// normiert oder in wahrerLaenge: UI_disp_vec1

  double  ay, az, scl;



  // printf("GL_Disp_vc %d\n",att);
  // printf(" GL2D_Scale=%lf\n",GL2D_Scale);
  // printf(" GL_SclNorm=%lf\n",GL_SclNorm);
  // UT3D_stru_dump (Typ_PT, pt1, "  pt1");
  // UT3D_stru_dump (Typ_VC, vc1, "  vc1");


  scl = 10. * GL_SclNorm;

  UT3D_2angr_vc (&az, &ay, vc1);

  // rad -> deg
  az = UT_DEGREES(az);
  ay = UT_DEGREES(ay);
    // printf("   az=%f ay=%f\n",az,ay);

    glPushAttrib (GL_CURRENT_BIT);  // save colourSettings

    glDisable (GL_LIGHTING);
    glCallList (DL_base_LnAtt + att);

    glPushMatrix (); // must be done before glTranslated !

      // Transl und Scale: Reihenfolge wichtig !
      glTranslated (pt1->x, pt1->y, pt1->z);
      glRotated (az, 0.0, 0.0, 1.0);  // um Z drehen
      glRotated (ay, 0.0, -1.0, 0.0);  // um Y drehen
      glScaled (scl, scl, scl);
      glCallList ((GLuint)SYM_ARROW);    // draw arrow

    glPopMatrix ();


    glEnable (GL_LIGHTING);
    glPopAttrib ();

   return 0;

}


//================================================================
   int GL_Disp_arrh (Point *p1, Point *p2, Vector *vcn) {
//================================================================
// GL_Disp_arrh          display arrowhead

  double    az, ay;


  // Pfeilspitze darstellen
  UT3D_vc_2pt (vcn, p1, p2);
  UT3D_2angr_vc (&az, &ay, vcn);
  az = UT_DEGREES(az);
  ay = UT_DEGREES(ay);
  glLineStipple (1, (GLushort)0xFFFF);  // solid
  GL_Disp_symV (SYM_ARROH, p1, az, ay, 1.);

  return 0;

}


//===========================================================================
  int GL_Disp_vSym (int symTyp,Point *pt1,Vector *vc1,double scale,int att) {
//===========================================================================
/// GL_Disp_vSym    add vectorsymbols (true length) into open displist
// SYM_VEC: Vektorsymbol (mit wahrer Laenge) in eine offene Displiste zufuegen
//            (vec.normiert: see GL_Disp_vc/GL_DrawVec)
// zB SYM_SQUARE SYM_PLANE
// see also GR_DrawPlane

  double  ay, az, sclNorm;


  // UT3D_stru_dump (Typ_PT, pt1, "GL_Disp_vSym pt=");
  // UT3D_stru_dump (Typ_PT, vc1, "GL_Disp_vSym vc=");


  // sclNorm = 5. * GL_SclNorm;
  // sclNorm = scale * GL_SclNorm;
  sclNorm = scale / 10.;                // f SYM_VEC notwendig !

  UT3D_2angr_vc (&az, &ay, vc1);

  // rad -> deg
  az = UT_DEGREES(az);
  ay = UT_DEGREES(ay);
  // printf("   az=%f ay=%f\n",az,ay);

  glPushAttrib (GL_CURRENT_BIT);  // save colourSettings

  glCallList (DL_base_LnAtt + att);

  GL_Disp_symV (symTyp, pt1, az, ay, sclNorm);

  glPopAttrib ();

  return 0;

}


//===========================================================================
  int GL_Disp_symV (int symTyp,Point *pt1,double az,double ay,double scale) {
//===========================================================================
// GL_Disp_symV     add vectorsymbols eg SYM_ARROH SYM_VEC into open displist


  // printf("GL_Disp_symV %d %f %f %f\n",symTyp,az,ay,scale);

  glPushMatrix ();

  // Transl und Scale: Reihenfolge wichtig !
  glTranslated (pt1->x, pt1->y, pt1->z);
  glRotated (az, 0.0, 0.0, 1.0);  // um Z drehen
  glRotated (ay, 0.0, -1.0, 0.0);  // um Y drehen
  glScaled (scale, scale, scale);

  glCallList ((GLuint)symTyp);
  glPopMatrix ();

  return 0;

}

 
//================================================================
  int GL_Disp_symB (int symTyp, Point *pt1) {
//================================================================
/// GL_Disp_symB          add bitmapsymbols into open displist


  // UT3D_stru_dump (Typ_PT, pt1, "GL_Disp_symB: ");


  // den DL-Index (+ Offset) holen)
  // dlInd = GL_fix_DL_ind (ind);

  // glNewList (dlInd, GL_COMPILE);

  glDisable (GL_LIGHTING);

  // glCallList (DL_base_LnAtt + Typ_Att_Symb);
  // glColor3fv    (GL_col_tab[attInd]);

  glRasterPos3d (pt1->x, pt1->y, pt1->z);
  glCallList ((GLuint)symTyp);

  glEnable (GL_LIGHTING);

  // glEndList ();


   return 0;

}


//================================================================
  int GL_Disp_2D_box1 (Point *p1, int sx, int sy, int dx, int dy) {
//================================================================
// glList must be open !!
// hilite not-zooming 2D-maps.
//   p1     Position of Field in Usercoords
//   sx, sy size of 2D-Field in screencoords
//   dx, dy offset from p1 to lower left 2D-corner in screencoords



  int    irc, i1, i2, ix,iy, st, sizBuf;
  float  fx, fy;
  unsigned char  *cBuf;


  // printf("GL_Disp_2D_box1 %d %d %d %d\n",sx,sy,dx,dy);


  st = 4;      // Linienbreite


  if(GLT_pta_SIZ < 1024) {
    i1 = GLT_alloc_pta (1024);
    if(i1 == -1) return -1;
  }

  cBuf = (void*)GLT_pta;
  sizBuf = GLT_pta_SIZ * sizeof(Point);




  // jedes Pixel braucht ein byte; (dieses definiert den Rot-Wert).
  if(sx > sy) i2 = sx * st;
  else        i2 = sy * st;
  i2 /= 4;
  if(i2 >= sizBuf) {  // passiert nur wenn Box groesser als Window (unvisible)
    // TX_Error("GL_Disp_2D_box1 E001 %d",i2);
    i2 = sizBuf;
  }
  for(i1=0; i1<i2; ++i1) cBuf[i1] = 255;

  glRasterPos3d (p1->x, p1->y, p1->z);


  // move offset
  if((dx != 0)||(dy != 0)) {
    fx = dx; fy = dy; glBitmap (0,0, 0.f,0.f, fx,fy, NULL);
  }

  // ein quadrat sx X sy hinmalen ..
  ix = sx;
  iy = st;
  fx = ix - st;
  fy = iy;
  glBitmap (ix,iy, 0.f,0.f, fx,fy, cBuf);
  ix = st;
  iy = sy;
  fx = ix;
  fy = iy;
  glBitmap (ix,iy, 0.f,0.f, fx,fy, cBuf);

  fx = -sx;
  fy = -sy;
  glBitmap (0,0, 0.f,0.f, fx,fy, NULL);

  ix = st;
  iy = sy;
  fx = ix;
  fy = iy - st;
  glBitmap (ix,iy, 0.f,0.f, fx,fy, cBuf);
  ix = sx - st;
  iy = st;
  fx = ix;
  fy = iy;
  glBitmap (ix,iy, 0.f,0.f, fx,fy, cBuf);

  return 0;

}

/*
//================================================================
  int GL_Disp_2D_box2 () {
//================================================================
// hilite zoomed 2D-maps; nur unten und links.

  int    irc, i1, i2, ix,iy, sx,sy, st;
  float  fx, fy;
  long   ind;
  GLuint DL_ind;
  Point  pt1, pt2;
  unsigned char   cBuf[10000];

  // UT3D_pt_3db (&pt1, 100, 0, 0);
  UT3D_pt_3db (&pt2, -400, 0, 0);

  sx= 400; sy = 100; st = 4;

  // jedes Pixel braucht ein byte; (dieses definiert den Rot-Wert).
  if(sx > sy) i2 = sx * st;
  else        i2 = sy * st;
  for(i1=0; i1<i2; ++i1) cBuf[i1] = 255;

  ind = -1;
  DL_ind = GL_fix_DL_ind (&ind);
  printf(" ind=%d DL_ind=%d\n",ind,DL_ind);
  // DL_ind = 298;

  glNewList (DL_ind, GL_COMPILE);   // Open DispList

  glRasterPos3d (100., 100., 0.);

  glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

  glCallList (DL_Img_ScBack);  // scale; glPixelZoom

  glDrawPixels (sx, st, GL_RED, GL_UNSIGNED_BYTE, cBuf);
  glDrawPixels (st, sy, GL_RED, GL_UNSIGNED_BYTE, cBuf);

  // man kann nur mit glRasterPos3d wieder zustellen;
  // mit glBitmap kann man nur um einen fixen Betrag moven (geht nicht
  // glPixelZoom mit !

  glDepthFunc (GL_LEQUAL); // reset ...
  glPopMatrix ();  // vom ScaleBack
  glEndList ();                        // Close DispList

  return 0;

}
*/


//================================================================
  int GL_Disp_ci2 (Point2 *pc, Point2 *p1, double ao) {
//================================================================
// draw 2D-Circ (CCW) starting at p1 around pc

#define  PTNR_MAX 1024

  int    i1, ptNr, ianz;
  double d1, d2, rdc;
  Vector vcz;
  Point  ptc, pt1, *pta;


  // printf("GL_Disp_ci2 ao=%f\n",ao);
  // UT3D_stru_dump (Typ_PT2, pc, "  pc:");
  // UT3D_stru_dump (Typ_PT2, p1, "  p1:");


  vcz = UT3D_VECTOR_Z;  // od Z-vec der ConstrPln ?
  ptc = UT3D_pt_pt2 (pc);
  pt1 = UT3D_pt_pt2 (p1);


  // ptNr = fabs(ao) / (RAD_1 * 10.);
  rdc = UT2D_len_2pt (pc, p1);   // Radius
  ptNr = UT2D_ptNr_ci (rdc, fabs(ao), UT_DISP_cv);


  // limit the nr of points
  if(ptNr > PTNR_MAX) {
    printf("***** GL_Disp_ci2 I001\n");
    ptNr = PTNR_MAX;
  }

  d1 = ao / ptNr;
  d2 = d1;

  // printf("GL_Disp_ci2 ao=%f ptNr=%d\n",ao,ptNr);


  // get memSpc for points
  pta = MEM_alloc_tmp  (ptNr * sizeof(Point));

  pta[0] = pt1;

  ++ptNr;

  for(i1=1; i1<ptNr; ++i1) {
    UT3D_pt_rotptptvcangr (&pta[i1], &pt1, &ptc, &vcz, d2);
    // GR_Disp_pt (&pta[i1], SYM_STAR_S, 2);
    d2 += d1;
  }

  GL_Disp_cv (ptNr, pta);

  return 0;

}


//================================================================
  int GL_Draw_rbez (long *ind, int iAtt, CurvRBez *rb1) {
//================================================================
/// GL_Draw_rbez          draw rational bezier curve

  int         attInd;


  attInd = ((Ind_Att_ln*)&iAtt)->indAtt;

  GL_DrawLn_Ini (ind, attInd);

  GL_Disp_rbez (rb1);

  glEndList ();

  return 0;

}


//================================================================
  int GL_Disp_rbez (CurvRBez *rb1) {
//================================================================
/// \code
/// GL_Disp_rbez          draw rational bezier curve
/// TEST ONLY; TODO: set nr of subdivisions
/// \endcode

  int         i1, ptNr, ptMax;
  double      *pw;
  wPoint      *pwa;



  UT3D_stru_dump (Typ_CVRBEZ, rb1, "GL_Disp_rbez");

  ptMax = 50;  // nr of subdivisions of polygon

  ptNr = rb1->ptNr;


  // glDisable (GL_LIGHTING);
  // glCallList (DL_base_LnAtt + iAtt); // Farbe, Strichstaerke ..


  //----------------------------------------------------------------
  // get space for wPoint-array
  pwa = MEM_alloc_tmp (sizeof(wPoint) * ptNr);

  // transfer rbsp-curve from format point[3] + weight into format point[4]
  UT3D_wPTn_ptn_wn (pwa, rb1->ptNr, rb1->cptab, rb1->wtab);


  //----------------------------------------------------------------
  glEnable (GL_MAP1_VERTEX_4);

  // setup curve
  // 90-deg-circ: 0-45° = 0., 2.415
  // glMap1f (GL_MAP1_VERTEX_3,
  glMap1d (GL_MAP1_VERTEX_4,
           0.,          // startparameter (< 0.)
           1.,          // endparameter ( > 1.)
           4,           // distance between points in nr of words
           ptNr,        // nr of control-points
           (void*)pwa); // points

  // evaluate all points
  glMapGrid1d (ptMax,   // nr of subdivisions
                0.,     // startparameter
                1.);    // endparameter

  // display points
  glEvalMesh1 (GL_LINE,
                0,       // first point in grid
               ptMax);   // last point in grid

  glDisable (GL_MAP1_VERTEX_4);


  return 0;

}


//================================================================
  int GL_Disp_cv2 (int pnr, Point2 *pta) {
//================================================================
// 2D-Polygon in eine offene Displiste zufuegen

  int i1;

  // printf("GL_Disp_cv2 %d\n",pnr);

  glBegin (GL_LINE_STRIP);
    for (i1 = 0; i1 < pnr; i1++) {
      // printf(" plvert %d = %f,%f,%f\n",i1,pTab[i1].x,pTab[i1].y,pTab[i1].z);
      glVertex3d (pta[i1].x, pta[i1].y, 0.);
    }
  glEnd();


  return 0;

}


//================================================================
  int GL_Disp_cv (int pnr, Point *pta) {
//================================================================
// Polygon in eine offene Displiste zufuegen

  int i1;


  // printf("=========== GL_Disp_cv %d\n",pnr);


  glBegin (GL_LINE_STRIP);
    for (i1 = 0; i1 < pnr; i1++) {
        // printf(" vert %d = %f,%f,%f\n",i1,pta[i1].x,pta[i1].y,pta[i1].z);
      glVertex3dv ((double*)&pta[i1]);
    }
  glEnd();


  return 0;

}


//================================================================
  int GL_DrawSymVX (long *ind, int att, Plane *pln1, int typ,
                    double scale){
//================================================================
/// \code
/// display plane / axisSystem [with x,y,z-characters]
///   att   see GR_Disp_ln2  (see ~/gCAD3D/cfg/ltyp.rc)
///   typ   1=Plane; 2=Axis; 4=Axis+Chars; 5=Plane+Axis+Chars;
///   scale unused.
/// \endcode
 
  Mat_4x4   m1;
  GLuint    dlInd;


  // printf("GL_DrawSymVX att=%d typ=%d\n",att,typ);
  // UT3D_stru_dump (Typ_PLN, pln1, " pln1-");


  // den DL-Index (+ Offset) holen)
  dlInd = GL_fix_DL_ind (ind);

  glNewList (dlInd, GL_COMPILE);

    glDisable (GL_LIGHTING);

    glCallList (DL_base_LnAtt + att);

    glPushMatrix ();

    UT3D_m4_loadpl (m1, pln1);

    glMultMatrixd ((double*)m1);

    if((typ & 1) > 0)                   // bit 1
    glCallList ((GLuint)SYM_SQUARE);

    if((typ & 2) > 0)                   // bit 2
    glCallList ((GLuint)SYM_AXIS1);

    if((typ & 4) > 0)                   // bit 3
    glCallList ((GLuint)SYM_AXIS);


    glPopMatrix ();

    glEnable (GL_LIGHTING);

  glEndList ();


  return 0;

}


//=============================================================================
  int GL_DrawSymVTL (long *ind, int symTyp, int att, Point *pt1, double *uScal) {
//=============================================================================
// display vector-symbol with true length at 2D-ScreenCenter ..

  GLuint  dlInd;
  double  ay, az, scl;


  // printf("GL_DrawSymVTL \n");
  // UT3D_stru_dump (Typ_VC, &GL_eyeX, "  eyeVec: ");

  // scl = *uScal * GL2D_Scale;
  // scl = *uScal * GL_SclNorm / 10.;
  scl = *uScal;


  // den DL-Index (+ Offset) holen)
  dlInd = GL_fix_DL_ind (ind);

  glNewList (dlInd, GL_COMPILE);

    glDisable (GL_LIGHTING);

    glCallList (DL_base_LnAtt + att);
    // glCallList (DL_base_LnAtt + Typ_Att_Symb);

    glPushMatrix ();

    glCallList ((GLuint)DL_Ind_Cen2D); // set to 2D-ScreenCenter

    glCallList ((GLuint)DL_Ind_Scl2D); // rotate,

    glScaled (scl, scl, scl);

    glCallList ((GLuint)symTyp); // draw ..

    glPopMatrix ();

    glEnable (GL_LIGHTING);

  glEndList ();


  return 0;

}


//=============================================================================
  void GL_DrawSymV3 (long *ind, int symTyp, int att,
                     Point *pt1, Vector *vc1, double scale){
//=============================================================================
/// \code
/// GL_DrawSymV3          draw oriented vector-symbols;
/// symTyp:
///   SYM_ARROW     draw line with arrowhead with normalized (always same) length
///   SYM_SQUARE    draw rectangle normal to vc1
///   SYM_PLANE     draw filled rectangle normal to vc1
///   SYM_AXIS1     draw x,y,z.axis normal to vc1
///   SYM_AXIS      draw x,y,z.axis with x,y-characters normal to vc1
///   SYM_CROSS     draw cross (X) normal to vc1
///   SYM_CROSS1    draw symbolic scissor normal to vc1
///   SYM_TRIANG    draw triangle normal to vc1
///   SYM_ARROH     draw 2D-arrowhead-only in the x-y-plane
///   SYM_ARRO3H    draw 3D-arrowhead-only along vc1
///   SYM_LENGTH    draw line with lenght from scale; not normalized.
///
/// dli = -2L; GL_DrawSymV3 (&dli, SYM_ARROW, 12, &pt1, &vcz, 20.);
/// dli = -3L; GL_DrawSymV3 (&dli, SYM_SQUARE, 12, &pt1, &vcz, 1.);
/// \endcode

// TODO:
// SYM_ARROW: Arrowhead wird auch gezoomt! Daher Länge nicht einstellbar ..


  double  ay, az, scl;
  GLuint  dlInd;


  // printf("GL_DrawSymV3 Typ=%d p=%f,%f,%f  v=%f,%f,%f sc=%f\n",symTyp,
               // pt1->x,pt1->y,pt1->z,vc1->dx,vc1->dy,vc1->dz,scale);
  // UT3D_stru_dump (Typ_VC, vc1, "vc1");
  // printf("  GL2D_Scale=%lf\n",GL2D_Scale);


  UT3D_2angr_vc (&az, &ay, vc1);
    // printf(" az=%lf ay=%lf\n",az,ay);


  // rad -> deg
  az = UT_DEGREES(az);
  ay = UT_DEGREES(ay);
  // printf("   az=%f ay=%f scl=%f\n",az,ay,scl);


  // display 3D-plane, vc1 is Normalvec !
  // ../ut/func_types.h: SYM_SQUARE=142 SYM_PLANE=145
  if((symTyp == SYM_SQUARE)||(symTyp == SYM_PLANE)) {
    ay -= 90.;
    // printf("  korr. az=%f ay=%f\n",az,ay);
    scl = scale;

  } else {
    // nur bei den Vektoren !
    scl = scale * GL_SclNorm;
  }


  // den DL-Index (+ Offset) holen)
  dlInd = GL_fix_DL_ind (ind);

  glNewList (dlInd, GL_COMPILE);

    glDisable (GL_LIGHTING);

    glCallList (DL_base_LnAtt + att);

    // glCallList (DL_base_LnAtt + Typ_Att_Symb);

    glPushMatrix ();

    // Transl und Scale: Reihenfolge wichtig !
    glTranslated (pt1->x, pt1->y, pt1->z);
    glRotated (az, 0.0, 0.0, 1.0);  // um Z drehen
    glRotated (ay, 0.0, -1.0, 0.0);  // um Y drehen
    glScaled (scl, scl, scl);

    glCallList ((GLuint)symTyp);
    glPopMatrix ();

    glEnable (GL_LIGHTING);

  glEndList ();


  //if(*ind < 0)  glCallList (dlInd);

}


//================================================================
  int GL_Tex_Load (TexBas *tbAct, char *fNam) {
//================================================================
// loads bitmage-imagefile <fNam>;
//   zuerst ins Memory, dann m. glTexImage2D -> OpenGL als Textur Nr texNr
//   Texture wird in Texturspeicher <texInd> gespeichert.
//   Later use with glBindTexture(<texInd>).
// Input:
//   tbAct->texNr   OpenGL-TextureNr (index of TexBasTab + 1)
//   fNam
// Output:
//   -

// see GL_LoadTex (alte Version)

  int    irc, ix, iy, pixSiz;
  void   *mSpc;
  long   dlTex;
  GLuint dlInd, texInd;
  float  scx, scy, ar, dx, dy;
  Vector vx, vy;
  double vs[4], vt[4], da[4];
  char   bNam[256];


  // printf("GL_Tex_Load |%s| %d\n",fNam,tbAct->texNr);


  // Textur ins memory laden; memory mit malloc neu holen.
  pixSiz = bmp_load (&mSpc, &ix, &iy, fNam);
  // if(pixSiz < 0) return -1;
    // printf(" BMP-siz=%d %d\n",ix,iy);

  if(pixSiz < 0) {
    sprintf(bNam, "%sNO_IMAGE32x32_24.bmp",OS_get_ico_dir());
    pixSiz = bmp_load (&mSpc, &ix, &iy, bNam);
    if(pixSiz < 0) return -1;
  }


  // check for max textureSize
  irc = GL_Tex_ckSiz (ix, iy);
    // printf(" ex GL_Tex_ckSiz %d\n",irc);
  if(irc) {
    TX_Error("cannot load texure %s - too big; reduce size or resolution",fNam);
    free (mSpc);
    return -1;
  }


  // save Imagesize (pixels)
  tbAct->xSiz = ix;
  tbAct->ySiz = iy;


  // ix /= 2;
  // iy /= 2;


/*
  // DL-Nr dlTex f. Textur holen ..
  // save the TextureDiplistNr in the TexBasRecord.
  APT_line_set (-1); // damit wirds nicht geloescht
  dli = DL_StoreObj (Typ_TEXB, 1L, 0);
  tbAct->dli = GL_fix_DL_ind (&dli);
  dlInd = tbAct->dli;
    printf(" dlInd=%d\n",tbAct->dli);
*/


  //----------------------------------------------------------------
  // init Textures
  texInd = tbAct->texNr;
  glDeleteTextures (1, &texInd),   // delete Texture # 1

  // eine neue ID fuer eine Textur generieren
  // glGenTextures (1, &texNr );
      // printf(" GL_Tex_Load Nr=%d\n",texInd);


  //----------------------------------------------------------------
  // define Textur
  // glNewList(dlInd, GL_COMPILE);


    // connect texture ..
    glBindTexture (GL_TEXTURE_2D, texInd);


    // Verkleinern/Vergroessern; GL_LINEAR=Ein, GL_NEAREST=aus
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // GL_REPEAT(Default)|GL_CLAMP.
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // GL_MODULATE GL_DECAL GL_BLEND GL_REPLACE  (nur DEC|REPL gehen)
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    // fuer automat. 2D-coord-Generiereung m glTexGen
    // GL_OBJECT_LINEAR: wird in Richtung Vektor projiziert ..
    glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

    // fuer automat. 2D-coord-Generiereung m glTexGen
    // glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    // glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
      // glTexGendv(GL_S, GL_EYE_PLANE, vs);
      // glTexGendv(GL_T, GL_EYE_PLANE, vt);

    // fuer automat. 2D-coord-Generiereung m glTexGen  MIST
    // glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    // glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);


  if(pixSiz == 3) {
    // specify texture-parameters  24-bit-colours:
    glTexImage2D (GL_TEXTURE_2D,
                  0,                    // Level    0
                  3, //GL_RGB,          // internalFormat/nr of components
                  ix, iy,               // imageSize in pixels
                  0,                    // border
                  GL_BGR,               // Format; GL_BGR_EXT,
                  GL_UNSIGNED_BYTE,     // GL_BITMAP=1bit/pixel
                  mSpc);

  } else {
    // specify texture-parameters  8-bit-colours:
    glTexImage2D (GL_TEXTURE_2D,
                  0,                    // Level    0
                  3, //GL_RGB,          // internalFormat/nr of components
                  ix, iy,               // imageSize in pixels
                  0,                    // border
                  GL_LUMINANCE,               // Format; GL_BGR_EXT,
                  GL_UNSIGNED_BYTE,     // GL_BITMAP=1bit/pixel
                  mSpc);
  }


  // glEndList();

  free (mSpc);

  return 0;

}
 
/* ersetzt durch  GL_Tex_Load
//================================================================
  int GL_LoadTex (long *ind, TexBas *tex) {
//================================================================
// load texture
// save TexBas-DL-RecordNr in tex->dli.
// RetCod:  0 = OK; -1 = Symbol_not_found

  int       irc, ix, iy;
  GLuint    DL_ind, texNr;
  void      *mSpc;
  char      cBuf[256];


  // printf("GL_LoadTex %d %d\n",*ind,tex->texNr);
  // UT3D_stru_dump (Typ_TEXB, tex, "GL_LoadTex: ");


  irc = Mod_get_path (cBuf, tex->fnam);
  if(irc < 0) return -1;
    // printf(" fnam=|%s|\n",cBuf);


  // Textur ins memory laden; memory mit malloc neu holen.
  irc = bmp_load (&mSpc, &ix, &iy, cBuf);
  if(irc < 0) return -1;
    // printf(" BMP-siz=%d %d\n",ix,iy);

  // ix /= 2;
  // iy /= 2;

  // save bitmap-size
  tex->xSiz = ix;
  tex->ySiz = iy;


  // DL-Nr DL_ind f. Textur holen ..
  DL_ind = GL_fix_DL_ind (ind);
    // printf(" DL_ind=%d ind=%d\n",DL_ind,*ind);
  tex->dli = DL_ind;


  //----------------------------------------------------------------
  // init Textures
  // texNr = 1;
  texNr = tex->texNr+1;
  glDeleteTextures (1, &texNr),   // delete Texture
  // tex->texNr = texNr;


  //----------------------------------------------------------------
  // define Textur
  glNewList(DL_ind, GL_COMPILE);

    // Verkleinern/Vergroessern; GL_LINEAR=Ein, GL_NEAREST=aus
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // GL_REPEAT(Default)|GL_CLAMP.
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // GL_MODULATE GL_DECAL GL_BLEND GL_REPLACE  (nur DEC|REPL gehen)
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    // fuer automat. 2D-coord-Generiereung m glTexGen
    // GL_OBJECT_LINEAR: wird in Richtung Vektor projiziert ..
    glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

    // fuer automat. 2D-coord-Generiereung m glTexGen
    // glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    // glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
      // glTexGendv(GL_S, GL_EYE_PLANE, vs);
      // glTexGendv(GL_T, GL_EYE_PLANE, vt);

    // fuer automat. 2D-coord-Generiereung m glTexGen  MIST
    // glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    // glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

    // eine neue ID fuer eine Textur generieren
    // glGenTextures (1, &texNr );
      // printf(" texNr=%d\n",texNr);


    // connect texture ..
    glBindTexture (GL_TEXTURE_2D, texNr);

    // specify texture-parameters
    glTexImage2D (GL_TEXTURE_2D,        // od GL_PROXY_TEXTURE_2D
                  0,                    // mipmap-level ?
                  GL_RGB,            // Format: GL_RGBA
                  ix, iy,            // imageSize in pixels
                  0,                 // Border; 0 od 1
                  GL_BGR_EXT,        // data format GL_BGR gibts bei MS nicht !
                  GL_UNSIGNED_BYTE,  // data type
                  mSpc);


  glEndList();

  free (mSpc);


  return 0;

}
*/

//==============================================================================
  int GL_DrawTxtsym (long *ind, int typ, Point *pts, Vector *vc1,
                     int att, double scl) {
//==============================================================================
// att = color
// typ
//   0  SYM_STAR_S
//   1  SYM_TRI_S
//   2  SYM_CIR_S
//   3  SYM_TRI_B
//   4  vector normalized
//   5  vector with length from scale; not normalized
//   6  arrowhead
// used by APT_DrawTxtA

  GLuint DL_ind;


  // printf("GL_DrawTxtsym ind=%ld typ=%d att%d scl=%f\n",*ind,typ,att,scl);
  // UT3D_stru_dump (Typ_PT, pts, " pts:");
  // UT3D_stru_dump (Typ_VC, vc1, " vec:");


  if(typ > 4) {

    if(typ == 6) { // Arrowhead 2D
      Point pte;
      UT3D_pt_traptvc (&pte, pts, vc1);
      GL_DrawSymV2 (ind, SYM_ARROH, att, pts, &pte, scl);
      return 0;

    } else if(typ == 5) { // vector with real length
      GL_DrawVec (ind, att, pts, vc1);
      return 0;
    } else {
      TX_Error("GL_DrawTxtsym E002 %d",typ);
    }
  }


  DL_ind = GL_fix_DL_ind (ind);

  if(att < 0) att = 0;


  glNewList (DL_ind, GL_COMPILE);   // Open DispList
  glDisable (GL_LIGHTING);
  glDepthFunc (GL_ALWAYS);          // alles ueberschreiben



  //----------------------------------------------------------------
  if(typ == 4) {        // vec normiert
    GL_Disp_vc (vc1, pts, att);
    goto L_99;
  }

  //----------------------------------------------------------------
  glColor3fv (GL_col_tab[att]);

  glRasterPos3d (pts->x, pts->y, pts->z);

  if(typ == 0) {
    glCallList ((GLuint)SYM_STAR_S);

  } else if(typ == 1) {
    glCallList ((GLuint)SYM_TRI_S);

  } else if(typ == 2) {
    glCallList ((GLuint)SYM_CIR_S);

  } else if(typ == 3) {
    glCallList ((GLuint)SYM_TRI_B);
  }
  // werden via rectangle angezeigt ...
  // DL_setTagSiz (*ind, 0, 0); // ??

  goto L_99;



  //----------------------------------------------------------------
  L_99:
  glDepthFunc (GL_LEQUAL); // reset ...
  glEnable (GL_LIGHTING);
  glEndList ();                        // Close DispList


  return 0;

}


//================================================================
  int GL_Draw_Tag (long *dli, AText *tx1, long dbi) {
//================================================================
// atta: fuellfarbe des Block. -1: kein Block.
// attl: Leaderlineattribut; -1: keine Leaderline (pTxt-pLdr).


  int    symTyp, atta, attl, i1, i2, i3, ic, il, ix,iy, dx,dy, sizBuf;
  GLuint DL_ind;
  float  x1, y1;
  unsigned char   *cBuf;
  char   *txt, *p1, *p2;
  Point  *pTxt, *pLdr;


  pTxt   = &tx1->p1;
  pLdr   = &tx1->p2;
  txt    = tx1->txt;
  symTyp = tx1->aTyp;
  atta   = tx1->col;
  attl   = tx1->ltyp;   // Linetyp Leaderline; -1=no Leaderline


  // printf("GL_Draw_Tag |%s|symTyp=%d atta=%d attl=%d\n",txt,symTyp,atta,attl);


  if(GLT_pta_SIZ < 1024) {
    i1 = GLT_alloc_pta (1024);
    if(i1 == -1) return -1;
  }


  cBuf = (void*)GLT_pta;
  sizBuf = GLT_pta_SIZ * sizeof(Point);


  DL_ind = GL_fix_DL_ind (dli);


  // printf("GL_Draw_Tag sizBuf=%d dli=%d DL_ind=%d\n",sizBuf,*dli,DL_ind);
  // UT3D_stru_dump (Typ_PT, pTxt, " pos:");
  // printf(" symTyp=%d atta=%d attl=%d\n",symTyp,atta,attl);
  // printf(" txt=|%s|\n",txt);


  glNewList (DL_ind, GL_COMPILE);   // Open DispList
  glPixelStorei (GL_UNPACK_ALIGNMENT, 1);  // fuer alle folgenden PixelOpers.
  glDisable (GL_LIGHTING);

  // alles ueberschreiben !
  glDepthFunc (GL_ALWAYS);

  // Charspce dx X dy
  dx = 10;    //  8 + 2  = CharBreite + CharAbst
  dy = 17;    // 13 + 4  = CharHoehe + ZeilenAbst

  // ic = max Anzahl chars (longest line)
  // il = Anzahl Lines
  if(txt == NULL) goto L_LDR;                       // 2013-10-05
  il = UTX_find_wordNr (&ic, "[n", txt);
  il += 1;


/*
  //----------------------------------------------------------------
  // symTyp > 4 sind Symbols; 5=SYM_STAR_S 6=SYM_TRI_S 7=SYM_TRI_B
  // SymbolColor = atta.
  if(symTyp < 5) goto L_draw_Tag;
  symTyp -= 5;

  if(symTyp > 3) {           // Vector ..
    if(symTyp == 4) {        // vec normiert
      GL_Disp_vc ((Vector*)pLdr, pTxt, atta);
    } else if(symTyp == 5) { // vec in wahrer Laenge
      GL_Disp_vSym (SYM_VEC, pTxt, (Vector*)pLdr, 1., atta);
    } else {
      TX_Error("GL_Draw_Tag E002 %d",symTyp);
    }
    goto L_99;
  }


  if(atta > 0) glColor3fv (GL_col_tab[atta]);

  glRasterPos3d (pTxt->x, pTxt->y, pTxt->z);

  if(symTyp == 0) {
    glCallList ((GLuint)SYM_STAR_S);

  } else if(symTyp == 1) {
    glCallList ((GLuint)SYM_TRI_S);

  } else if(symTyp == 2) {
    glCallList ((GLuint)SYM_CIR_S);

  } else if(symTyp == 3) {
    glCallList ((GLuint)SYM_TRI_B);
  }
  // werden via rectangle angezeigt ...
  DL_setTagSiz (*ind, 0, 0); // ??
  goto L_99;
*/
  

  //----------------------------------------------------------------
  L_draw_Tag:
  // eine gefuellte Flaeche hinmalen
  // Fuellfeld steht -6,-4 vom TextNullpunkt.
  // Breite/Hoehe des FuellFeldes ist ix/iy
  // BitfieldSize ix X iy
  ix = (ic + 1) * dx;  // add 0.5 char links und rechts
  iy = (dy * il) + 2;   // add unten u oben

  // bei tags und Bitmaps die size im DL-record speichern
  // DL_setTagSiz (*dli, ix, iy);
  tx1->xSiz = ix;
  tx1->ySiz = iy;


  // check Size of bitBuffer
  i1 = (ix / 8) + 1;  // jedes Byte hat nur 8 Bit.
  i1 *= iy;
  // printf(" ix=%d iy=%d i1=%d\n",ix,iy,i1);
  if(i1 >= sizBuf) {TX_Error("GL_Draw_Tag E001 %d",i1); return -1;}
  // preset BitBuffer
  for(i2=0; i2<i1; ++i2) cBuf[i2] = 255;

  // FuellFarbe fuer Block; Default ist gelb.
  if(atta < 0) goto L_TXT;
  if(atta < 2) atta = 5;
  glColor3fv (GL_col_tab[atta]);  // rot

  glRasterPos3d (pTxt->x, pTxt->y, pTxt->z);

  x1 = 0.;
  y1 = -iy / 2;
  glBitmap (0,0, 0.f,0.f, x1,y1, NULL);

  glBitmap (ix, iy,              // Breite, Hoehe
            0.f, 0.f,            // origin innerhalb map;
            0.f, 0.f,            // add to posi after drawing
            cBuf);



  //----------------------------------------------------------------
  // Text schreiben; ZeilenAnzahl=il
  L_TXT:
  if(txt == NULL) goto L_LDR;
  glColor3f (0.f, 0.f, 0.f);      // Textfarbe schwarz
  glRasterPos3d (pTxt->x, pTxt->y, pTxt->z);


/*
// BEGIN TEST:
  glWindowPos3dv (&pTxt);
    printf(" il=%d ic=%d\n",il,ic);
  p1 = txt;
  for(i1=0; i1<ic; ++i1) {
    glCallLists (1, GL_UNSIGNED_BYTE, (GLubyte*)p1);
    ++p1;
  }
// END TEST
*/
  // move rasterpos
  x1 = 6;
  y1 = ((il-1) * dy) + 4 - (iy / 2);
  glBitmap (0,0, 0.f,0.f, x1,y1, NULL);
  p1 = txt;

  for(i1=0; i1<il; ++i1) {
    p2 = strstr(p1, "[n");
    if(p2 == NULL) p2 = txt + strlen(txt);
    i2 = p2 - p1;
    // draw i2 characters
    glCallLists (i2, GL_UNSIGNED_BYTE, (GLubyte*)p1);
    // printf(" %d |%s|\n",i2,p1);
    p1 = p2 + 2;
    x1 = -i2 * 10;
    y1 = -dy;
    // goto startpoint of next line
    glBitmap (0,0, 0.f,0.f, x1,y1, NULL);
  }



  //----------------------------------------------------------------
  // Leader
  L_LDR:
  if(attl < 0) goto L_99;
  if(attl < 1) attl = 1;
  glCallList (DL_base_LnAtt + attl);
  glBegin (GL_LINES);
    glVertex3dv ((double*)pTxt);
    glVertex3dv ((double*)pLdr);
  glEnd ();



  //----------------------------------------------------------------
  L_99:
  glDepthFunc (GL_LEQUAL); // reset ...
  glEnable (GL_LIGHTING);
  glEndList ();                        // Close DispList


  return 0;

}


//=========================================================================
  void GL_DrawTxtA (long *ind, int att, Point *P1, char *txt){
//=========================================================================
// Den Text als Listeneintrag einer Liste speichern.
// Jeder char ist DL-Objekt mit glListBase 0.
// Der gesamte String ist auch ein Objekt (aber anderer offset !)

// Alfatext ist dzt nur am 0-Punkt (links unten) selektierbar !
// see also GL_txt__


  GLuint dlInd;


  // printf("GL_DrawTxtA %d %f %f %f |%s|\n",*ind,P1->x, P1->y, P1->z,txt);


  // den DL-Index (+ Offset) holen)  
  dlInd = GL_fix_DL_ind (ind);

  glNewList (dlInd, GL_COMPILE);

  glDisable (GL_LIGHTING);

  glCallList (DL_base_LnAtt + Typ_Att_Symb);

  if(att) glColor3fv (GL_col_tab[att]);

  glRasterPos3d (P1->x, P1->y, P1->z);

  // glPushAttrib (GL_LIST_BIT);
  glCallLists (strlen(txt), GL_UNSIGNED_BYTE, (GLubyte *) txt);
  // glPushAttrib (GL_LIST_BIT);

  glEnable (GL_LIGHTING);

  glEndList ();


}


//=========================================================================
  int GL_DrawDimrad (long *ind, int att, Dimen *dim1) {
//=========================================================================

/*
  int      i1, i2, ih1;
  double   d1, txAng;
  char     dimText[32], outText[128];
  Point    pt3;
  Point2   p3a, p3e;
  Vector2  vc1;

  // printf("GL_DrawDimrad %f\n",dim1->a1);

  // Pfeil: (Typ=dim1->hd, Pos=dim1->p2), Richtg=vc1
  ih1 = dim1->hd;
  UT2D_vc_2pt (&vc1, &dim1->p2, &dim1->p3);

  // den Text generieren; dmr f oder R
  strcpy(dimText, "R");  // Dmr-zeichen
  d1 = dim1->a2;
  UTX_add_fl_f (dimText, d1, GR_tx_nkNr);
  UTX_del_foll0 (dimText);

  GR_gxt_subst1 (outText, dim1->txt, dimText);

  // Maszlinie: p3a-p3e
  p3a = dim1->p2;

  // innen od aussen ?
  d1 = UT2D_len_2pt (&dim1->p1, &dim1->p3);

  // innen: p3e = Center;
  if(d1 < dim1->a2) {
    p3e = dim1->p1;

  // aussen: p3e um halbe Blockbreite nach aussen schieben
  } else {
    d1 = GR_gtx_ckBlockWidth (outText, AP_txdimsiz) / 2.;
    UT2D_pt_tra2ptlen (&p3e, &dim1->p3, &dim1->p1, -d1);
  }

  txAng = dim1->a1;

  // fix txAng; Masztext von -45-135 Grad aussen, else Innen.
  // if((txAng > RAD_135)&&(txAng < (RAD_360-RAD_45))) {
  if((txAng > 135.)&&(txAng < 315.)) {
    // printf(" >>>>>> Masztext auf die andere Seite <<<<<<<<<<<<\n");
    // txAng -= RAD_180;
    txAng -= 180.;
  }

  //================================================================
  // Init GL-Ausgaben
  GL_DrawLn_Ini (ind, att);

  glPushMatrix ();

  // draw Maszpfeil
  GL_txt_ar2 (ih1, &dim1->p2, &vc1);

  // MaszLinie
  GL_Disp_ln2 (&p3a, &p3e);

  // a1 ist der fertige textBlockWinkel
  glPassThrough (1.0);  // Typ_Att_def = dick
  pt3 = UT3D_pt_pt2 (&dim1->p3);
  GL_txt__ (0, 0, &pt3, txAng, 0., 0., AP_txdimsiz, outText);
*/

  double  atx, dVal;
  Line2   lnd;
  Vector2 vct;
  SymRef2 hd1;
  Point   pt3;
  char    outText[256];


  // get dimensionline, symbolpositions, symboldirection, value, text
  GR_dim_rad__ (&lnd, &hd1, &vct, &atx, &dVal, outText,
                dim1);


  //================================================================
  // Init GL-Ausgaben
  GL_DrawLn_Ini (ind, att);

  glPushMatrix ();

  // draw Maszpfeil
  GL_txt_ar2 (hd1.typ, &hd1.pt, &vct);

  // MaszLinie
  GL_Disp_ln2 (&lnd.p1, &lnd.p2);

  // a1 ist der fertige textBlockWinkel
  glPassThrough (1.0);  // Typ_Att_def = dick
  pt3 = UT3D_pt_pt2 (&dim1->p3);
  GL_txt__ (0, 0, &pt3, atx, 0., 0., AP_txdimsiz, outText);

  // close GL-Ausgaben
  GL_EndList2 ();

  return 0;

}


//=========================================================================
  int GL_DrawDimdia (long *ind, int att, Dimen *dim1) {
//=========================================================================
// angle dim1->a1 in Degree !


/*
  int      i1, i2, ih1, ih2;
  double   d1, txAng;
  char     dimText[32], outText[128];
  Point    pt3;
  Point2   p3a, p3e, p2o;
  Vector2  vc1;

  // printf("GL_DrawDimdia %f\n",dim1->a1);

  // den Text generieren;
  strcpy(dimText, "[d");  // Dmr-zeichen
  d1 = 2. * dim1->a2;
  UTX_add_fl_f (dimText, d1, GR_tx_nkNr);
  UTX_del_foll0 (dimText);

  GR_gxt_subst1 (outText, dim1->txt, dimText);

  // Pfeil: (Typ=dim1->hd, Pos=dim1->p2 und p2o), Richtg=vc1
  ih2 = dim1->hd % 10;
  ih1 = (dim1->hd - ih2) / 10;

  // p2o = opposit pt am circ
  UT2D_pt_opp2pt (&p2o, &dim1->p1, &dim1->p2);
  UT2D_vc_2pt (&vc1, &p2o, &dim1->p2);

  // Maszlinie: p3a-p3e
  // innen od aussen ?
  d1 = UT2D_len_2pt (&dim1->p1, &dim1->p3);

  // innen:
  if(d1 < dim1->a2) {
    p3a = p2o;
    p3e = dim1->p2;
    MEM_swap_int (&ih1, &ih2);

  // aussen: 
  } else {
    p3a = p2o;
    p3e = dim1->p3;
    // p3e um halbe Blockbreite nach aussen schieben
    d1 = GR_gtx_ckBlockWidth (outText, AP_txdimsiz) / 2.;
    UT2D_pt_tra2ptlen (&p3e, &dim1->p3, &dim1->p1, -d1);
  }

  txAng = dim1->a1;

  // fix txAng; Masztext von -45-135 Grad aussen, else Innen.
  // if((txAng > RAD_135)&&(txAng < (RAD_360-RAD_45))) {
  if((txAng > 135.)&&(txAng < 315.)) {
    // printf(" >>>>>> Masztext auf die andere Seite <<<<<<<<<<<<\n");
    // txAng -= RAD_180;
    txAng -= 180.;
  }

  GL_DrawLn_Ini (ind, att);

  glPushMatrix ();

  // draw Maszpfeile
  GL_txt_ar2 (ih1, &dim1->p2,  &vc1);
  GL_txt_ar2 (ih2, &p2o, &vc1);


  // MaszLinie
  GL_Disp_ln2 (&p3a, &p3e);

  // a1 ist der fertige textBlockWinkel
  glPassThrough (1.0);  // Typ_Att_def = dick
  pt3 = UT3D_pt_pt2 (&dim1->p3);
  GL_txt__ (0, 0, &pt3, txAng, 0., 0., AP_txdimsiz, outText);

  // close GL-Ausgaben
  GL_EndList2 ();
*/


  double  atx, dVal;
  Line2   lnd;
  Vector2 vct;
  SymRef2 hd1, hd2;
  Point   pt3;
  char    outText[256];


  // get dimensionline, symbolpositions, symboldirection, value, text
  GR_dim_dia__ (&lnd, &hd1, &hd2, &vct, &atx, &dVal, outText,
                dim1);



  //================================================================
  // Init GL-Ausgaben
  GL_DrawLn_Ini (ind, att);

  glPushMatrix ();

  // draw Maszpfeile
  GL_txt_ar2 (hd1.typ, &hd1.pt, &vct);
  GL_txt_ar2 (hd2.typ, &hd2.pt, &vct);


  // MaszLinie
  GL_Disp_ln2 (&lnd.p1, &lnd.p2);

  // a1 ist der fertige textBlockWinkel
  glPassThrough (1.0);  // Typ_Att_def = dick
  pt3 = UT3D_pt_pt2 (&dim1->p3);
  GL_txt__ (0, 0, &pt3, atx, 0., 0., AP_txdimsiz, outText);

  // close GL-Ausgaben
  GL_EndList2 ();

  return 0;

}


//=========================================================================
  int GL_DrawDima (long *ind, int att, Dimen *dim1) {
//=========================================================================

  int     i1, i2;
  double  atx, dVal;
  char    outText[256];
  Vector2 vct, vc1, vc2;
  Line2   ll1, ll2;
  Circ2   acd;
  SymRef2 hd1, hd2;
  Point   pt3;


  // get leaderlines, dimensionline, symbolpositions, symboldirection,value,text
  GR_dim_ang__ (&ll1, &ll2, &acd, &hd1, &hd2,
                &vct, &atx,
                &dVal, outText,
                dim1);

  //================================================================
  // Init GL-Ausgaben
  GL_DrawLn_Ini (ind, att);

  glPushMatrix ();

  // draw Hilfslinien
  i2 = dim1->ld % 10;
  i1 = dim1->ld / 10;
    // printf("Leadertypes = %d,%d\n",i1,i2);

  // Leaderline 1
  if(i1 == 1) GL_Disp_ln2 (&ll1.p1, &ll1.p2);

  // Leaderline 2
  if(i2 == 1) GL_Disp_ln2 (&ll2.p1, &ll2.p2);


  // draw Maszlinie (Circ! CCW)
  GL_Disp_ci2 (&acd.pc, &acd.p1, acd.ango);


  // draw Maszpfeile
  UT2D_vc_angr (&vc1, hd1.ang);
  UT2D_vc_angr (&vc2, hd2.ang);
  GL_txt_ar2 (hd1.typ, &hd1.pt, &vc1);
  GL_txt_ar2 (hd2.typ, &hd2.pt, &vc2);


  // draw Text
  glPassThrough (1.0);  // Typ_Att_def = dick
  pt3 = UT3D_pt_pt2 (&dim1->p3);
  GL_txt__ (1, 0, &pt3, atx, 0., 0., AP_txdimsiz, outText);

  // close GL-Ausgaben
  GL_EndList2 ();


/*
  int      i1, i2, ia1, ia2, iout;
  double   d0, d1, d2, d3, txAng, a1, a2, a3, aa, am;
  Point    pt3;
  Point2   ps, pt1e, pt2e, pt3a, pt3e, ptm;
  Vector2  vc1, vc2;
  char     dimText[32], outText[128];

  // printf("GL_DrawDima \n");
  // UT3D_stru_dump (Typ_Dimen, dim1, "GL_DrawDima");

  // ps = den Schnittpunkt der beiden Hilfslinien errechnen
  // a1 = UT2D_angr_ptpt (&ps, &dim1->p1);
  // a2 = UT2D_angr_ptpt (&ps, &dim1->p2);
  a1 = dim1->a1;
  a2 = dim1->a2;
  UT2D_vc_angr (&vc1, a1);
  UT2D_vc_angr (&vc2, a2);
  UT2D_pt_int2pt2vc (&ps, &dim1->p1, &vc1, &dim1->p2, &vc2);
    // GR_Disp_pt2 (&ps, SYM_STAR_S, 2);

  // der Abstand des p3 vom Schnittpunkt ps ergibt Laenge der Hilfslinien
  d1 = UT2D_len_2pt (&ps, &dim1->p1);
  d2 = UT2D_len_2pt (&ps, &dim1->p2);
  d3 = UT2D_len_2pt (&ps, &dim1->p3);

  UT2D_pt_traptvclen (&pt3a, &ps, &vc1, d3);
  UT2D_pt_traptvclen (&pt3e, &ps, &vc2, d3);

  // Maszpfeiltypen
  ia2 = dim1->hd % 10;
  ia1 = (dim1->hd - ia2) / 10;

  // Leaderlines um GR_tx_ldExt verlaengern.
  if(d1 < d3) d0 = GR_tx_ldExt;
  else        d0 = -GR_tx_ldExt;
  // von ps -> p1 Dist=d3
  UT2D_pt_tra2ptlen (&pt1e, &pt3a, &ps, -d0);
  if(d2 < d3) d0 = GR_tx_ldExt;
  else        d0 = -GR_tx_ldExt;
  UT2D_pt_tra2ptlen (&pt2e, &pt3e, &ps, -d0);

  // MaszLinienOeffnungswinkel aa
  // a3 = angle ps (Maszlinienschnittpunkt) - p3 (Textpunkt)
  a3 = UT2D_angr_ptpt (&ps, &dim1->p3);
  aa = UT2D_angr_2angr (a1, a2, 1);
    // printf(" aa=%f a1=%f a2=%f a3=%f\n",aa,a1,a2,a3);

  // die Normale auf ps-p3 ergibt den TextWinkel txAng
  txAng = UT2D_angr_perpangr (&a3);     // txAng = a3 + 90 deg
    // printf(" txAng=%f a3=%f\n",txAng,a3);

  // fix txAng; Masztext von 45-225 Grad aussen, else Innen.
  if((a3 > RAD_45)&&(a3 < RAD_225)) {
    // printf(" >>>>>> Masztext auf die andere Seite <<<<<<<<<<<<\n");
    txAng -= RAD_180;
  }

  // Circ. Maszlinie zwischen den Hilfslinien oder rausziehen -
  // liegt aa zwischen a1 / a2 ?
  d1 = UT2D_angr_2angr (a1, a3, 1);
    // printf(" d1=%f\n",d1);

  // liegt Textblock zwischen den Hilfslinien oder ausserhalb
  if(d1 < aa) {   // dazwischen ..
    ptm = pt3a;
    am = aa;

  } else {
    MEM_swap_int (&ia1, &ia2);  // swap Maszpfeiltypen
    d1 = RAD_360 - d1; // nun CW
    d2 = UT2D_angr_2angr (a2, a3, 1);
      // printf(" d1=%f d2=%f\n",d1,d2);
    if(d2 < d1) {      // p3 bei p2
      ptm = pt3a;
      am = aa + d2;
    } else {           // p3 bei p1
      ptm = pt3e;
      am = -(aa + d1);
    }
  }
    // printf(" am=%f d1=%f\n",am,d1);

  iout = 1;

  // den Text generieren
  GR_gxt_prep1 (dimText, UT_DEGREES(aa)); // dimText = aa
  strcat(dimText, "[g");

  // substitute [% by dimText
  GR_gxt_subst1 (outText, dim1->txt, dimText);


  //================================================================
  // Init GL-Ausgaben
  GL_DrawLn_Ini (ind, att);

  glPushMatrix ();


  // draw Hilfslinien
  i2 = dim1->ld % 10;
  i1 = (dim1->ld - i2) / 10;
    // printf("Leadertypes = %d,%d\n",i1,i2);
  if(i1 == 1) GL_Disp_ln2 (&dim1->p1, &pt1e);
  if(i2 == 1) GL_Disp_ln2 (&dim1->p2, &pt2e);

  // draw Maszlinie (Circ! CCW)
  GL_Disp_ci2 (&ps, &ptm, am);

  // draw Maszpfeile
  UT2D_vc_perpvc (&vc1, &vc1);
  UT2D_vc_perpvc (&vc2, &vc2);
  GL_txt_ar2 (ia1, &pt3a, &vc1);
  GL_txt_ar2 (ia2, &pt3e, &vc2);

  // draw Text
  glPassThrough (1.0);  // Typ_Att_def = dick
  pt3 = UT3D_pt_pt2 (&dim1->p3);
  GL_txt__ (iout, 0, &pt3, UT_DEGREES(txAng), 0., 0., AP_txdimsiz, outText);

  // close GL-Ausgaben
  GL_EndList2 ();
*/

  return 0;

}


//=========================================================================
  int GL_DrawLdr (long *ind, int att, Dimen *dim1) {
//=========================================================================

  int     i1;
  double  atx;
  Line2   ll1, ll2;
  Point2  ptx;
  Vector2 vct, vcs;
  SymRef2 hd1;
  Point   p31; 
  char    outText[256];


  // UT3D_stru_dump (Typ_Dimen, dim1, "GL_DrawLdr");


 // get leaderlines, dimensionline, symbolpositions, symboldirection,value,text
  GR_dim_ldr__ (&ll1, &ll2, &hd1, &ptx,
                &vct, &atx,
                outText,
                dim1);


  //================================================================
  // Init GL-Ausgaben
  GL_DrawLn_Ini (ind, att);

  glPushMatrix ();


  // draw Maszpfeil
  i1 = dim1->hd;
  UT2D_vc_angr (&vcs, hd1.ang);
  GL_txt_ar2 (i1, &hd1.pt, &vcs);


  // Linien
  GL_Disp_ln2 (&ll1.p1, &ll1.p2);
  if(dim1->p3.x != UT_DB_LEER)
    GL_Disp_ln2 (&ll2.p1, &ll2.p2);


  // a1 ist der fertige textBlockWinkel
  glPassThrough (1.0);  // Typ_Att_def = dick
  p31 = UT3D_pt_pt2 (&ptx);
  GL_txt__ (2, 0, &p31, atx, 0., 0., AP_txdimsiz, outText);


  // close GL-Ausgaben
  GL_EndList2 ();




/*
  int      i1, i2;
  double   txAng;
  char     outText[128];
  Point2   p3;
  Point    pt3;
  Vector2  vc1;

  // printf("GL_DrawLdr ang=%f hd=%d |%s|\n",dim1->a1,dim1->hd,dim1->txt);

  // MaszpfeilRichtg vc1 = p1 -> p2
  UT2D_vc_2pt (&vc1, &dim1->p1, &dim1->p2);

  // den Text generieren
  GR_gxt_subst1 (outText, dim1->txt, NULL);
  // dimText[0] = '\0';
  // if(dim1->txt != NULL) {  // add ZusatzText
    // strcat(dimText, dim1->txt);
  // }
    // printf(" outText |%s|\n",outText);

  // Linie p2-p3
  if(dim1->p3.x != UT_DB_LEER) {  // es gibt einen p3
    // liegt p2 links oder rechts von p3;
    // wenn rechts: den p3 um TextBlockBreite nach rechts schieben
    p3 = dim1->p3;
    if(dim1->p2.x > dim1->p3.x) {
      p3.x += GR_gtx_ckBlockWidth (outText, AP_txdimsiz);
    }
  }

  txAng = dim1->a1;

  // fix txAng; Masztext von -45-135 Grad aussen, else Innen.
  // if((txAng > RAD_135)&&(txAng < (RAD_360-RAD_45))) {
  if((txAng > 135.)&&(txAng < 315.)) {
    // printf(" >>>>>> Masztext auf die andere Seite <<<<<<<<<<<<\n");
    // txAng -= RAD_180;
    txAng -= 180.;
  }

  //================================================================
  // Init GL-Ausgaben
  GL_DrawLn_Ini (ind, att);

  glPushMatrix ();

  // draw Maszpfeile
  i1 = dim1->hd;
  GL_txt_ar2 (i1, &dim1->p1, &vc1);

  // Linien
  GL_Disp_ln2 (&dim1->p1, &dim1->p2);
  if(dim1->p3.x != UT_DB_LEER)
    GL_Disp_ln2 (&dim1->p2, &p3);

  // a1 ist der fertige textBlockWinkel
  glPassThrough (1.0);  // Typ_Att_def = dick
  if(dim1->p3.x != UT_DB_LEER) pt3 = UT3D_pt_pt2 (&dim1->p3);
  else                         pt3 = UT3D_pt_pt2 (&dim1->p2);

  GL_txt__ (2, 0, &pt3, txAng, 0., 0., AP_txdimsiz, outText);

  // close GL-Ausgaben
  GL_EndList2 ();
*/

  return 0;

}


//=========================================================================
  int GL_DrawDim3 (long *ind, int att, Dim3 *dim3) {
//=========================================================================
// Doku see APT_decode_dim3
// bp = Textrichtung

// att: 0=black, normal;   9=hilited

  int     posTyp, bMod, nkNr;
  double  cw, blw, ay, az1, az2, dl, d1;
  char    dimTyp;
  char    dimText[64], outText[128];
  Vector  vcx, vcy, vcz;
  Point   *pp1, *pp2, *pp3, pm1, pm2, pa1, pa2, ptx;


  // UT3D_stru_dump (Typ_Dim3, dim3, "GL_DrawDim3");
  // printf(" att=%d\n",att);


  pp1 = DB_get_PT (dim3->ip1);
  pp2 = DB_get_PT (dim3->ip2);
  pp3 = DB_get_PT (dim3->ipt);

  dimTyp = dim3->dtyp;   // 'X';


  cw  = GTX_chw_ (1.);


  // vcx zeigt in Richtung MaszText (nach rechts)
  // vcy zeigt in Richtung MaszText nach oben
  // vcz zeigt in Richtung MaszText zum Betrachter



  //----- Variante DimPlane X:-----------------------------------
  if(dimTyp != 'X') goto L_Y;
  strcpy(dimText, "dx=");
  // auf X-Y oder X-Z-Ebene ?
  // Ist der Z-part oder der Y-part von vcs groesser -
  // Hoehenwert der Backplane bestimmen: den niedrigeren Wert von p1/p2 nehmen
  if(dim3->bp == 2)               { // BCKPLN_XY
    vcx = UT3D_VECTOR_X;
    vcy = UT3D_VECTOR_Y;
    vcz = UT3D_VECTOR_Z;
    dl = fabs(pp1->x - pp2->x);
    // az1 = 90.;
  } else {                           // BCKPLN_XZ
    vcx = UT3D_VECTOR_X;
    vcy = UT3D_VECTOR_Z;
    vcz = UT3D_VECTOR_IY;
    dl = fabs(pp1->x - pp2->x);
    // az1 = 90.;
  }
  goto L_draw1;




  //----- Variante DimPlane Y:-----------------------------------
  L_Y:
  if(dimTyp != 'Y') goto L_Z;
  strcpy(dimText, "dy=");
  // auf X-Y oder Y-Z-Ebene ?
  // Ist der Z-part oder der X-part von vcs groesser -
  // Hoehenwert der Backplane bestimmen: den niedrigeren Wert von p1/p2 nehmen
  if(dim3->bp == 2)               {  // BCKPLN_XY
    vcx = UT3D_VECTOR_Y;
    vcy = UT3D_VECTOR_IX;
    vcz = UT3D_VECTOR_Z;
    dl = fabs(pp1->y - pp2->y);
    // az1 = 90.;
  } else {                           // BCKPLN_YZ
    vcx = UT3D_VECTOR_Y;   // fix
    vcy = UT3D_VECTOR_Z;
    vcz = UT3D_VECTOR_X;
    dl = fabs(pp1->y - pp2->y);
    // az1 = 90.;
  }
  goto L_draw1;




  //----- Variante DimPlane Z:-----------------------------------
  L_Z:
  if(dimTyp != 'Z') goto L_parl;
  strcpy(dimText, "dz=");
  // auf X-Z oder Y-Z-Ebene ?
  // Ist der X-part oder der Y-part von vcs groesser -
  // Hoehenwert der Backplane bestimmen: den niedrigeren Wert von p1/p2 nehmen
  if(dim3->bp == 1)               {    // draw to XZ
    vcx = UT3D_VECTOR_Z;
    vcy = UT3D_VECTOR_IX;
    vcz = UT3D_VECTOR_IY;
    dl = fabs(pp1->z - pp2->z);
    // az1 = 90.;
  } else {                            // draw to YZ
    vcx = UT3D_VECTOR_Z;   // fix
    vcy = UT3D_VECTOR_IY;
    vcz = UT3D_VECTOR_X;
    dl = fabs(pp1->z - pp2->z);
    // az1 = 90.;
  }
  goto L_draw1;





  //----- Variante keine DimPlane:-----------------------------------
  L_parl:
  // vcx = p1 - p2
  if(dimTyp != '-') {
    UT3D_vc_2pt (&vcx, pp1, pp2);
  } else {
    UT3D_vc_2pt (&vcx, pp2, pp1);
  }

  // vcz = vcx -   pp3-pp1
  UT3D_vc_perpvc2pt (&vcz, &vcx, pp3, pp1);
    // GR_Disp_vc (&vcz, pp1, 9, 0);

  if(dim3->bp == 4) {   // verkehrt
    UT3D_vc_invert (&vcz, &vcz);
  }

  // 2.Vec in der DimPlane
  UT3D_vc_perp2vc (&vcy, &vcz, &vcx);

  dl = UT3D_len_vc (&vcx);
  dimText[0] = '\0';

  goto L_draw1;





  //----------------------------------------------------------------
  L_draw1:

  // extract GR_tx_nkNr
  nkNr = GR_gxt_nkNr (dim3->txt);
    // printf(" dl=%f nkNr=%d\n",dl,nkNr);

  // den outText erzeugen (add Zahlenwert dl)
  UTX_add_fl_f (dimText, dl, nkNr);
    // printf(" dimText1=|%s|\n",dimText);
  if(nkNr > 0) UTX_del_foll0 (dimText);
    // printf(" dimText2=|%s|\n",dimText);

  // use GR_gxt_subst1 here ...
  bMod = GR_gxt_subst1 (outText, dim3->txt, dimText);
  bMod = 0;  // skip Colour !!


  // pa1 pa2 (sind die Punkte pp1 pp2 auf der Maszlinie) erzeugen
  //   (pp1-pa1 pp2-pa2 sind die MaszHilfslinien)
  // pp3 verschieben in Richtg vcx Dist +dl und -dl;
  UT3D_2pt_oppptvclen (&pa2, &pa1, pp3, &vcx, dl/2.);
    // GR_Disp_pt (&pa1, SYM_TRI_S, 3);
    // GR_Disp_pt (&pa2, SYM_TRI_S, 3);





  //----------------------------------------------------------------
  L_draw2:
  // ab hier werden benoetigt:
  // pp1 pp2 pp3 (pp3 = der Usertextpunkt;
  // vcx zeigt in Richtung MaszText (nach rechts)
  // vcy zeigt in Richtung MaszText nach oben
  // vcz zeigt in Richtung MaszText zum Betrachter
  // pa1 pa2 - die Punkte pp1 pp2 auf der Maszlinie
  //   (pp1-pa1 pp2-pa2 sind die MaszHilfslinien)
  // outText   der fertig decodierte Text
  // cw




  // aus 2 Vektoren die 3 RotWinkel fuer GL errechnen
  GRU_angr3_vxvz (&az1, &ay, &az2, &vcx, &vcz);
    // printf(" az1=%f ay=%f az2=%f\n",az1,ay,az2);


  // compute BlockBreite;
  blw = GR_gtx_ckBlockWidth (outText, 1.);
    // printf(" blw=%f\n",blw);

  // (BlockBreite + 2 chars) > MaszAbstand: Masztext aussen
  if((blw + (2*cw)) < dl) posTyp = 0;  // Masztext innen
  else posTyp = 5;                     // Masztext aussen


  // MassLinie pm1-pm2 ex pa1-pa2 erzeugen
  if(posTyp == 0) {
    // ptx = mittelpkt pa1-pa2
    UT3D_pt_mid2pt (&ptx, &pa1, &pa2);
    pm1 = pa1;
    pm2 = pa2;

  } else {
    // pm1 = 2 chars nach links
    d1 = cw * 2.;
      // printf(" d1=%f\n",d1);
    UT3D_pt_traptvclen (&pm1, &pa1, &vcx, -d1);

    // pm2 um halbe Blockbreite nach aussen schieben
    // d1 = GR_gtx_ckBlockWidth (outText, AP_txdimsiz) / 2.;
    d1 = blw + (cw * 2.);
      // printf(" d1=%f\n",d1);
    UT3D_pt_traptvclen (&pm2, &pa2, &vcx, d1);

    ptx = pa2;

  }


  //  GL-Ausgabe
  // att = Typ_Att_Symb; Typ_GTXT od Typ_Dimen
  // ind = DL_StoreObj (Typ_Dimen, 99L, att);  // schon in APT_DrawDim3


  // Init GL-Ausgaben
  GL_DrawLn_Ini (ind, att);

  // draw Maszpfeile
  if(posTyp == 0) {
    GL_txt_ar3 (1, &pa1, &vcx, &vcy);
    GL_txt_ar3 (2, &pa2, &vcx, &vcy);
  } else {
    GL_txt_ar3 (2, &pa1, &vcx, &vcy);
    GL_txt_ar3 (1, &pa2, &vcx, &vcy);
  }

  // make the lines
  GL_Disp_ln (&pm1, &pm2);

  if(dimTyp != '-') {
    GL_Disp_ln (pp1, &pa1);
    GL_Disp_ln (pp2, &pa2);
  } else {                // pp1/pp2 verkehrt
    GL_Disp_ln (pp2, &pa1);
    GL_Disp_ln (pp1, &pa2);
  }

  

  glPushMatrix ();

  // GL-Ausgabe Text 0 od 5
  // GL_txt__ (posTyp, 0, &ptx, az1, ay, az2, 1., outText);
  GL_txt__ (posTyp, bMod, &ptx, az1, ay, az2, AP_txdimsiz, outText);

  // close GL-Ausgaben
  GL_EndList2 ();

  // printf("ex GL_DrawDim3\n");

  return 0;

}


//=========================================================================
  void GL_DrawDimen (long *ind, int att, Dimen *dim1) {
//=========================================================================
// att = Typ_Att_Symb


  int     i1, i2, bMod;
  double  a1, d1, dVal;
  Line2   ll1, ll2, lnd;
  Point2  ptx;
  Vector2 vct;
  SymRef2 hd1, hd2;
  Point   p31;
  char    outText[256];


  // UT3D_stru_dump (Typ_Dimen, dim1, "GL_DrawDimen");
  // printf(" att=%d\n",att);

  // att = 1;  dann schwarz; aber kein Hilite !!


  // create objects for linear dimension
  bMod = GR_dim_lin__ (&ll1, &ll2, &lnd, &hd1, &hd2, &vct, &dVal, outText,
                       dim1);



  //------------- Init GL-Ausgaben ----------------------
  GL_DrawLn_Ini (ind, att);

  glPushMatrix ();



  //------------- leaderlines ----------------------
  i2 = dim1->ld % 10;
  // i1 = (dim1->ld - i2) / 10;
  i1 = dim1->ld / 10;
    // printf("Leadertypes = %d,%d\n",i1,i2);

  // glCallList (DL_base_LnAtt + Typ_Att_PT);

  // Leaderline 1
  if(i1 == 1) GL_Disp_ln2 (&ll1.p1, &ll1.p2);

  // Leaderline 2
  if(i2 == 1) GL_Disp_ln2 (&ll2.p1, &ll2.p2);



  // die MaszLinie
  GL_Disp_ln2 (&lnd.p1, &lnd.p2);



  //------------- Arrowheads ----------------------
  GL_txt_ar2 (hd1.typ, &hd1.pt, &vct);
  GL_txt_ar2 (hd2.typ, &hd2.pt, &vct);



  //------------- den text raus----------------------
  glPassThrough (1.0);  // Typ_Att_def = dick
  a1 = dim1->a1;
  p31 = UT3D_pt_pt2 (&dim1->p3);
  GL_txt__ (0, bMod, &p31, a1, 0., 0., AP_txdimsiz, outText);




  //---------------------------------------------------
  Fertig:
  // close GL-Ausgaben
  GL_EndList2 ();


  return;

}


//=========================================================================
  int GL_txt__ (int dMod, int bMod,
                Point *ptx, double txAng, double ay, double az,
                double scale, char *txt) {
//=========================================================================
/// \code
/// draw text
/// dMod   0=linear X-mittig Y-unten
///        1=Circ.masz
///        2=Circ.masz
///        3=linear X-links Y-Mitte
///        4=Balloon
///        5=linear X-links(+2 chars) Y-unten
/// bMod   0=nix, 1=farbige Hintergrungflaeche
///        bMod wird auch aus txt via '[#' extrahiert !
/// ptx    der Textmittelpunkt unten
/// txAng  in Degrees
/// scale: AP_txdimsiz oder AP_txsiz oder explizite NoteSize.
///
/// see also GL_DrawTxtG
/// \endcode

  
  int     i1, i2, ilen, ssStat, ssNr;
  double  scl, d1, dx, dy, bw, bh, dx1, dy1, cw;
  Point   p1, p2;
  Point2  p21, p2c;



  // printf("GL_txt__ |%s| bMod=%d ang=%f scl=%f chw=%f\n",txt,bMod,txAng,
          // scale,GR_tx_chw);
  // printf(" dMod=%d pos=%f %f %f\n",dMod,ptx->x, ptx->y, ptx->z);


  if(txt == NULL) {printf("***** GL_txt__ E001 \n"); return -1;}



  // lineWidth of grafText
  glLineWidth (1.);    // GLfloat


  // get Textblockbreite in Usercoords
  bw = GR_gtx_ckBlockWidth (txt, scale);

  // get character height
  bh = GTX_chh_ (scale);
    // printf(" bw=%lf bh=%lf\n",bw,bh);

  cw = GTX_chw_ (scale);
    // printf(" cw=%lf\n",cw);


  // Balloon
  if(dMod == 4) {
    p21 = UT2D_pt_pt3 (ptx);
    p2c.x = p21.x + (bw/2.) + (cw/2.);
    p2c.y = p21.y;
    GL_Disp_ci2 (&p2c, &p21, RAD_360);  // Balloon
  }



  // auf Textstartpos gehen
  // Transl und Scale: Reihenfolge wichtig !
  glTranslated (ptx->x, ptx->y, ptx->z);


  // rotate
  // if(fabs(txAng) > 0.1) {
  if(txAng != 0.) {
    glRotated (txAng, 0.0, 0.0, 1.0);
  }


  if(ay != 0.) {
    glRotated (ay, 0.0, 1.0, 0.0);  // um Y drehen
    if(az != 0.)
      glRotated (az, 0.0, 0.0, 1.0);  // um Z drehen
  }




  // // Drehung um X-Achse: in die X-Z-Ebene
  // glRotated (90., 1.0, 0.0, 0.0);

  // // Drehung um Y-Achse: in die Y-Z-Ebene
  // glRotated (-90., 0.0, 1.0, 0.0);



  // lineare Text zentrieren 
  if(dMod == 0) {
    dx = -bw / 2.;
    dy = 0; //dy = -dx / 12.;
    // printf(" dx=%f dy=%f\n",dx,dy);
    glTranslated (dx, dy, 0.);


  // Angular: den textblock zentrieren; also um halben textBlock nach links
  // und ein wenig heben
  } else if(dMod == 1) {
    dx = -bw / 2.;
    dy = -dx / 12.;
    // printf(" dx=%f dy=%f\n",dx,dy);
    glTranslated (dx, dy, 0.);


  // Angular: den textblock nicht zentrieren; nur ein wenig heben
  } else if(dMod == 2) {
    dx = 0.;
    dy = -dx / 12.;
    // printf(" dx=%f dy=%f\n",dx,dy);
    glTranslated (dx, dy, 0.);


  // Linear links mittig
  } else if(dMod == 3) {
    dx = 0.;
    dy = -bh  / 2.;
    // printf(" dx=%f dy=%f\n",dx,dy);
    glTranslated (dx, dy, 0.);


  // Balloon
  } else if(dMod == 4) {
    dx = cw / 2.; //dx = 0.;
    dy = -bh  / 2.;
    // printf(" dx=%f dy=%f\n",dx,dy);
    glTranslated (dx, dy, 0.);


  // 5=linear X-links(+2 chars) Y-unten
  } else if(dMod == 5) {
    dx = cw * 2.;
    // dy = bh  / 2.;
    dy = 0.;
    // printf(" dx=%f dy=%f\n",dx,dy);
    glTranslated (dx, dy, 0.);

  }



  // scale
  scl = GTX_scale (scale);
  glScaled (scl, scl, scl);



  i1     = 0;
  ssStat = 0;             // status superscript; 0=off, 1=on
  ssNr   = 0;             // nr of chars
  ilen=strlen(txt);

  // glDisable (GL_DEPTH_TEST);
  // glDepthFunc (GL_ALWAYS); // alles ueberschreiben !
  // glDisable (GL_LIGHTING);
  // glLineWidth   (3.0f);

  dx1 = 0.;
  dy1 = 0.;


  NextChar:
  i2 = txt[i1];
    // printf(" next char %d %d\n",i1,i2);

  if(i2 == '[') {                 // SONDERZEICHEN
    ++i1;
    if(i1 >= ilen) goto L_parErr;
    i2 = txt[i1];
    if(i2 == 'd') i2 = 126;       // Diameter  is char 126
    else if(i2 == 'g') i2 = 127;  // Degree    is char 127
    else if(i2 == '+') i2 = 128;  // plusMinus is char 128
    else if(i2 == '\'') i2 = 34;  // " is char 34


    else if(i2 == 'n') {          // newline
      ++i1;
      if(i1 >= ilen) goto L_parErr;
      i2 = txt[i1];
      dx = -GR_tx_chw * ssNr;
      dy = -GR_tx_chw * 1.6;
      // printf(" newline %f %f %f\n",dx,dy);
      glTranslated (dx, dy, 0.);
      dx1 += dx;
      dy1 += dy;
      ssNr = 0;    // nr of chars
      goto NextChar;


    } else if(i2 == '/') {          // higher
      ++i1;
      if(i1 >= ilen) goto L_parErr;
      i2 = txt[i1];
      if(ssStat == 0) ssStat = GL_txtss ();
      dy = GR_tx_chw * 1.7;
      glTranslated (0., dy, 0.);
      dy1 += dy;
      ssNr = 0;    // nr of chars


    } else if(i2 == '\\') {        // lower
      ++i1;
      if(i1 >= ilen) goto L_parErr;
      i2 = txt[i1];
      if(ssStat == 0) ssStat = GL_txtss ();
      // zurueck und nach oben
      if(ssNr > 0) {
        dx = -GR_tx_chw * ssNr;
        dy = -GR_tx_chw * 1.6;
        glTranslated (dx, dy, 0.);
        dx1 += dx;
        dy1 += dy;
        ssNr = 0;    // nr of chars
      }

    } else if((i2 > '0')&&(i2 < '9')) { // nr of digits after comma ?
      ++i1;
      bMod = i2 - '0';

    }
  }

  // draw character; initialized with GL_InitGFont
  glCallList ((GLuint)i2+DL_FontOff);


  ++i1;
  ++ssNr;
  if(i1 < ilen) {
    glTranslated (GR_tx_chw, 0.0, 0.0);  // einen char nach rechts
    dx1 += GR_tx_chw;

    // if(ssStat > 0) ++ssNr;
    goto NextChar;
  }


  // glEnable(GL_DEPTH_TEST);
  glDepthFunc (GL_LEQUAL); // reset ...
  glEnable (GL_LIGHTING);



  //----------------------------------------------------------------
  // farbige Flaeche hinterlegen ...
  if(bMod < 1) goto L_fertig;
  // printf(" offs=%f %f bw=%f scl=%f\n",dx1,dy1,bw,scl);
  // printf(" ilen=%d\n",ilen);
  // printf(" GR_tx_chh=%f GR_tx_scale=%f AP_txdimsiz=%f\n",
           // GR_tx_chh,GR_tx_scale,AP_txdimsiz);
  p2.x = ilen * GTX_chw_(scale);
  p2.y = GTX_chhl2 (scale);
  p2.z = -p2.y / 12.;

  p1 = UT3D_PT_NUL;
  p1.z = p2.z;

  glTranslated (-dx1, -dy1, 0.);
  glColor3fv (GL_col_tab[bMod]);  // 5=gelb
  GL_Disp_sq1 (&p1, &p2);




  //----------------------------------------------------------------
  L_fertig:
  return 0;


  L_parErr:
  TX_Error("Error Sonderzeichen");
  return -1;

}


//=========================================================================
  int GL_Disp_sq1 (Point *p1, Point *p2) {
//=========================================================================

  Point pt1;

  // UT3D_stru_dump (Typ_PT, p1, "GL_Disp_sq1\n ");
  // UT3D_stru_dump (Typ_PT, p2, " ");
  // GL_Disp_ln (p1, p2);  // TEST



  glBegin (GL_POLYGON);

    pt1 = *p1;
    glVertex3dv ((double*)&pt1);

    pt1.x = p2->x;
    glVertex3dv ((double*)&pt1);

    pt1.y = p2->y;
    glVertex3dv ((double*)&pt1);

    pt1.x = p1->x;
    glVertex3dv ((double*)&pt1);

  glEnd ();


  return 0;

}


//=========================================================================
  int GL_txtss () {
//=========================================================================

  double scale = 0.4;

  glScaled (scale, scale, scale);

  return 1;

}


//================================================================
  int GL_txt_ar2 (int hd, Point2 *ptx, Vector2 *vc) {
//================================================================
// draw Maszpfeil
//   0=keiner, 1=<, 2=>, 3=/, 4=><;
// vc: ?

  int       pNr;
  Point2    pa[4];


  // printf("GL_txt_ar2 %d\n",hd);
  // UT3D_stru_dump (Typ_PT2, ptx, "pt:");
  // UT3D_stru_dump (Typ_VC2, vc, "vc:");


  // Arrowhead1: pa1 - pt3a - pa2.

  if(hd == 0) {  // 0=keiner
    return 0;

  } else if(hd == 1) {  // 1=<
    UT2D_pt_traptvc2len (&pa[0], ptx, vc, GR_tx_ardx, GR_tx_ardy);
    pa[1] = *ptx;
    UT2D_pt_traptvc2len (&pa[2], ptx, vc, GR_tx_ardx, -GR_tx_ardy);
    pNr = 3;

  } else if (hd == 2) {  // 2=>
    UT2D_pt_traptvc2len (&pa[0], ptx, vc, -GR_tx_ardx, GR_tx_ardy);
    pa[1] = *ptx;
    UT2D_pt_traptvc2len (&pa[2], ptx, vc, -GR_tx_ardx, -GR_tx_ardy);
    pNr = 3;


  } else if (hd == 3) {  // 3=/
    UT2D_pt_traptvc2len (&pa[0], ptx, vc,  GR_tx_ardx, GR_tx_ardy);
    UT2D_pt_traptvc2len (&pa[1], ptx, vc, -GR_tx_ardx, -GR_tx_ardy);
    GL_Disp_ln2 (&pa[0], &pa[1]);
    return 0;



  } else if (hd == 4) {  // 4=pt
    UT2D_pt_traptvc2len (&pa[0], ptx, vc,  GR_tx_ardx, GR_tx_ardy);
    UT2D_pt_traptvc2len (&pa[1], ptx, vc, -GR_tx_ardx, -GR_tx_ardy);
    GL_Disp_ln2 (&pa[0], &pa[1]);

    UT2D_pt_traptvc2len (&pa[0], ptx, vc,  GR_tx_ardx, -GR_tx_ardy);
    UT2D_pt_traptvc2len (&pa[1], ptx, vc, -GR_tx_ardx,  GR_tx_ardy);
    GL_Disp_ln2 (&pa[0], &pa[1]);
    return 0;



  } else {
    TX_Error("GL_txt_ar2 E001");
    return -1;

  }


  GL_Disp_cv2 (pNr, pa);


  return 0;

}


//================================================================
  int GL_txt_ar3 (int hd, Point *ptx, Vector *vcx, Vector *vcy) {
//================================================================
// draw Maszpfeil
//   0=keiner, 1=<, 2=>, 3=/, 4=o;
// vc: ?

  int       pNr;
  Point     pa[4];


  // printf("GL_txt_ar %d\n",hd);
  // UT3D_stru_dump (Typ_PT2, ptx, "pt:");
  // UT3D_stru_dump (Typ_VC2, vc, "vc:");


  // Arrowhead1: pa1 - pt3a - pa2.

  if(hd == 0) {  // 0=keiner
    return 0;


  } else if(hd == 1) {  // 1=<
    UT3D_pt_trapt2vc2len (&pa[0], ptx, vcx, GR_tx_ardx, vcy, GR_tx_ardy);
    pa[1] = *ptx;
    UT3D_pt_trapt2vc2len (&pa[2], ptx, vcx, GR_tx_ardx, vcy, -GR_tx_ardy);
    pNr = 3;

  } else if (hd == 2) {  // 2=>
    UT3D_pt_trapt2vc2len (&pa[0], ptx, vcx, -GR_tx_ardx, vcy, GR_tx_ardy);
    pa[1] = *ptx;
    UT3D_pt_trapt2vc2len (&pa[2], ptx, vcx, -GR_tx_ardx, vcy, -GR_tx_ardy);
    pNr = 3;


  } else if (hd == 3) {  // 3=/
    UT3D_pt_trapt2vc2len (&pa[0], ptx, vcx,  GR_tx_ardx, vcy, GR_tx_ardy);
    UT3D_pt_trapt2vc2len (&pa[1], ptx, vcx, -GR_tx_ardx, vcy, -GR_tx_ardy);
    GL_Disp_ln (&pa[0], &pa[1]);
    return 0;



  } else if (hd == 4) {  // 4=pt
    UT3D_pt_trapt2vc2len (&pa[0], ptx, vcx,  GR_tx_ardx, vcy, GR_tx_ardy);
    UT3D_pt_trapt2vc2len (&pa[1], ptx, vcx, -GR_tx_ardx, vcy, -GR_tx_ardy);
    GL_Disp_ln (&pa[0], &pa[1]);

    UT3D_pt_trapt2vc2len (&pa[0], ptx, vcx,  GR_tx_ardx, vcy, -GR_tx_ardy);
    UT3D_pt_trapt2vc2len (&pa[1], ptx, vcx, -GR_tx_ardx, vcy,  GR_tx_ardy);
    GL_Disp_ln (&pa[0], &pa[1]);
    return 0;



  } else {
    TX_Error("GL_txt_ar E001");
    return -1;

  }


  GL_Disp_cv (pNr, pa);


  return 0;

}


//=========================================================================
  int GL_DrawTxtLBG (long *dli, AText *tx1, long dbi) {
//=========================================================================
// LeaderLine + Balloon + 3D-Text
// Defaulttext sollte die N-Nummer (dbi) sein und ein Kreis rundherum.
// Keine UserTextangabe: nur N-Nummer;
// else KEIN DefaultText - nur UserText.


  int    i1, attl;         // lineAttribut ..
  GLuint DL_ind;
  double ang, size;
  Point  *pTxt, *pLdr;
  char   *txt, ptTxt[128], outTxt[256];


  pTxt    = &tx1->p1;
  pLdr    = &tx1->p2;
  txt     = tx1->txt;


  printf("GL_DrawTxtLBG dli=%ld dbi=%ld\n",*dli,dbi);
  UT3D_stru_dump (Typ_ATXT, tx1, " tx1:");


  //----------------------------------------------------------------
  // prepare Text
  if(txt == NULL) sprintf(ptTxt, "%ld", dbi);
  else ptTxt[0] = '\0';
    // printf(" ptTxt=|%s|\n",ptTxt);


  // substitute [% by dimText
  GR_gxt_subst1 (outTxt, txt, ptTxt);
    // printf(" outTxt=|%s|\n",outTxt);


  // store nr of chars -> xSiz
  i1 = GR_gxt_strLen (outTxt);
  tx1->xSiz = i1;
  tx1->ySiz = 0;
 
  

  //----------------------------------------------------------------
  attl = 0;

  GL_DrawLn_Ini (dli, attl);
  // DL_ind = GL_fix_DL_ind (dli);
  // glNewList (DL_ind, GL_COMPILE);   // Open DispList
  // glCallList (DL_base_LnAtt + attl);

  glPushMatrix ();

  ang  = 0.;
  size = 1.;


  //----------------------------------------------------------------
  // Leader
  // see also GL_Draw_Tag ..
  if(tx1->ltyp >= 0) {
    glBegin (GL_LINES);
      glVertex3dv ((double*)pTxt);
      glVertex3dv ((double*)pLdr);
    glEnd ();
  }


  // if(outTxt)
  GL_txt__ (4, 0, pTxt, ang, 0., 0., size, outTxt);  // doubles !




  //----------------------------------------------------------------
  L_99:
  // glDepthFunc (GL_LEQUAL); // reset ...
  // glEnable (GL_LIGHTING);
  // glEndList ();                        // Close DispList

  // close GL-Ausgaben, popMat
  GL_EndList2 ();

  return 0;

}


//=============================================================================
  int GL_DrawTxtLG (long *dli, AText *tx1, long dbi) {
//=============================================================================
// draw LeaderLine + 3D-Text

  int    i1, nkz, attl;         // lineAttribut ..
  GLuint DL_ind;
  double ang;
  char   *txt, ptTxt[128], outTxt[256], *p1;
  Point  *pTxt, *pLdr;


  pTxt    = &tx1->p1;
  pLdr    = &tx1->p2; 
  txt     = tx1->txt; 
  

  // printf("GL_DrawTxtLG dli=%d\n",*dli);
  // UT3D_stru_dump (Typ_PT, pTxt, " pTxt:");
  // UT3D_stru_dump (Typ_PT, pLdr, " pLdr:");
  // printf(" txt=|%s|\n",txt);



  //----------------------------------------------------------------
  // prepare Text
  // get nktNr from txt
  nkz = -1;
  p1 = txt;
  if(txt) {
    if((txt[0] == '[')&&(isdigit(txt[1]))) {
      nkz = ICHAR(txt[1]);
      // printf(" ICHAR %d |%c| %d\n",nkz,txt[1],txt[1]);
      p1 = &txt[2];
    }
  }

  // print formatted Point pLdr --> ptTxt
  GR_gxt_prep_Pt (ptTxt, pLdr, nkz);
    // printf(" ptTxt=|%s|\n",ptTxt);


  // substitute [% by dimText
  GR_gxt_subst1 (outTxt, p1, ptTxt);
    // printf(" outTxt=|%s|\n",outTxt);


  // store nr of chars -> xSiz
  i1 = GR_gxt_strLen (outTxt);
  tx1->xSiz = i1;
  tx1->ySiz = 0;


  // ltyp unused; set to 0
  tx1->ltyp = 0;


  //----------------------------------------------------------------
  attl = 0;
  GL_DrawLn_Ini (dli, attl);
  // DL_ind = GL_fix_DL_ind (dli);
  // glNewList (DL_ind, GL_COMPILE);   // Open DispList
  // glCallList (DL_base_LnAtt + iatt);

  glPushMatrix ();

  ang  = 0.;
  // size = 1.;


  //----------------------------------------------------------------
  // Leader
  // see also GL_Draw_Tag ..
  glBegin (GL_LINES);
    glVertex3dv ((double*)pTxt);
    glVertex3dv ((double*)pLdr);
  glEnd ();


  // if(outTxt)
  GL_txt__ (3, 0, pTxt, ang, 0., 0., AP_txdimsiz, outTxt);  // doubles !




  //----------------------------------------------------------------
  L_99:
  // glDepthFunc (GL_LEQUAL); // reset ...
  // glEnable (GL_LIGHTING);
  // glEndList ();                        // Close DispList

  // close GL-Ausgaben, popMat
  GL_EndList2 ();

  return 0;

}



//=========================================================================
  void GL_DrawTxtG (long *ind, int att, GText *tx1) {
//=========================================================================
// Den Text als Listeneintrag einer Liste speichern.
// Jeder char ist DL-Objekt mit glListBase ??.
// Der gesamte String ist auch ein Objekt (aber anderer offset !)

// text ist dzt nur am 0-Punkt (links unten) selektierbar !

  int   chrNr, lNr;
  char  *txt;
  float size, ang;


  // printf("GL_DrawTxtG %ld att=%d pos=%f,%f |%s|\n",*ind,att,pt1->x,pt1->y,txt);
  // printf("    siz=%f  ang=%f|\n",size,ang);


  txt  = tx1->txt;
  size = tx1->size;
  ang  = tx1->dir;

  // Defaultsize einsetzen
  if(size < 0.001) size = AP_txsiz;


  // // get chrNr=max_nr_of_chars and lNr=nr_of_lines
  GR_gtx_BlockWidth__ (&chrNr, &lNr, txt);


  // store in GText
  tx1->xSiz = chrNr;
  tx1->ySiz = lNr;
    // UT3D_stru_dump (Typ_GTXT, tx1, "GL_DrawTxtG");


  // Init GL-Ausgaben
  GL_DrawLn_Ini (ind, att);


  // glMatrixMode(GL_MODELVIEW);
  glPushMatrix ();


  // draw text
  GL_txt__ (2, 0, &tx1->pt, ang, 0., 0., size, txt);  // doubles !


  // close GL-Ausgaben
  // GL_EndList2 ();


  // glMatrixMode(GL_MODELVIEW);
  glPopMatrix ();


  // glEnable (GL_LIGHTING);
  glEndList ();


}


//============================================================================
  int GL_DrawDitto (long *ind, Point *p1, Point *po,long dlNr, long dlInd) {
//============================================================================


  GLuint   DL_ind;


  // printf("GL_DrawDitto %ld %ld\n",dlNr,dlInd);
  // UT3D_stru_dump (Typ_PT, p1, "pt-new");
  // UT3D_stru_dump (Typ_PT, po, "pt-old");
  // return 0;


  // den DL-Index (+ Offset) holen)
  DL_ind = GL_fix_DL_ind (ind);

  glNewList (DL_ind, GL_COMPILE);

    glPushMatrix();

    // auf den neuen Nullpunkt
    glTranslated (p1->x, p1->y, p1->z);

    // um den alten Nullpunkt zurueck
    glTranslated (-po->x, -po->y, -po->z);

    // hier statt GL_IndTab auch nur eine Zahl, da dlNr 1 sein sollte ...
    // glCallLists(dlNr, GL_UNSIGNED_INT, &GL_IndTab[dlInd]);
    glCallList (dlInd + DL_base__);
    // glCallList (2 + DL_base__);
    // printf(" ii=%d %d\n",dlInd,DL_base__);

    glPopMatrix();

  glEndList();

  return 0;

}


//============================================================================
  int GL_DrawDitto1 (long *ind,
                     Point *p1, Point *po, double az1, double ay, double az2,
                     double scl, long dlNr, long dlInd) {
//============================================================================

  GLuint   DL_ind;


  // printf("GL_DrawDitto1 dlNr=%ld dlInd=%ld\n",dlNr,dlInd);
  // printf("  az1=%f ay=%f az2=%f scl=%f\n",az1,ay,az2,scl);
  // UT3D_stru_dump (Typ_PT, p1, "pt-new");
  // UT3D_stru_dump (Typ_PT, po, "pt-old");
  // DL_DumpObjTab ();
  // return 0;


  // den DL-Index (+ Offset) holen)
  DL_ind = GL_fix_DL_ind (ind);
    // printf(" DL_ind=%d\n",DL_ind);


  glNewList (DL_ind, GL_COMPILE);

    glPushMatrix();

    // auf den neuen Nullpunkt
    glTranslated (p1->x, p1->y, p1->z);

    glRotated (az1, 0.0,  0.0, 1.0);   // um Z drehen
    glRotated (ay,  0.0,  1.0, 0.0);   // um Y drehen
    glRotated (az2, 0.0,  0.0, 1.0);   // um Z drehen

    if(fabs(scl - 1.) > UT_TOL_min1) {
      glScaled (scl, scl, scl);     // Scale
    }

    // um den alten Nullpunkt zurueck
    glTranslated (-po->x, -po->y, -po->z);

    // hier statt GL_IndTab auch nur eine Zahl, da dlNr 1 sein sollte ...
    // glCallLists(dlNr, GL_UNSIGNED_INT, &GL_IndTab[dlInd]);
    glCallList (dlInd + DL_base__);
    // glCallList (2 + DL_base__);
    // printf(" ii=%d %d\n",dlInd,DL_base__);

    glPopMatrix();

  glEndList();

  return 0;

}


//============================================================================
  void GL_DrawDitto2 (long *ind, Point *p1, double az, char mir, Ditto *di) {
//============================================================================
// ind    = DL-Ind des Ditto;
// p1     = Position (der neue Nullpunkt)
// az     = Verdrehwinkel um die Z-Achse in Grad.
// mir    = Spiegeln; ' '=keine, 'X'=um X-Z-Ebene spiegeln, 'Y'=um Y-Z-Ebene.

// di.po  = Origin (Verdreh-Nullpunkt, der Original-Nullpunkt des Basisdetails)
// di.ind = der Startindex der ObjListe
// di.siz = Anzahl Objekte


// Beisp:
// di1.ind = GL_GetActInd();       // akt.Index+1 fuer naechstes Obj.
// GR_Cre...                       // nun alle Obj generieren
// di1.siz = GL_GetActInd() - di1.ind;    // Anzahl Obj
// di1.po  = <ditto_Origin>;
// GL_DrawDitto2 (&dli, &p1, 0., ' ', &di1);





  GLuint   DL_ind;


  // printf("GL_DrawDitto2 pt=%f,%f,%f\n",p1->x,p1->y,p1->z);
  // printf("             po=%f,%f,%f\n",di->po.x,di->po.y,di->po.z);
  // printf("             ind=%d, siz=%d\n",di->ind,di->siz);



  // den DL-Index (+ Offset) holen)  
  DL_ind = GL_fix_DL_ind (ind);

  glNewList (DL_ind, GL_COMPILE);

    glPushMatrix();

    glTranslated (p1->x, p1->y, p1->z);

    // Drehung um die Z-Achse
    glRotated (az, 0.0, 0.0, 1.0);

    if(mir != ' ') {
      if(mir == 'X') {
        glScaled (1., -1., 1.);     // X-Z - Spiegelung
      } else if(mir == 'Y') {
        glScaled (-1., 1., 1.);     // Y-Z - Spiegelung
      } else if(mir == 'Z') {
        glScaled (1., 1., -1.);     // X-Y - Spiegelung
      }
    }

    // um den Nullpunkt zurueck
    glTranslated (-di->po.x, -di->po.y, -di->po.z);

    // printf(" Ditto-DL von %d bis %d\n",di->ind,di->siz);


    // DL_start = DL_base_mod + mdb->DLind;
    // glCallLists(mdb->DLsiz, GL_UNSIGNED_INT, &GL_IndTab[DL_start]);

    // if(di->siz > GL_TAB_SIZ) {
      // if(GL_alloc__ (di->siz) < 0) return -1;
    // }

    // glListBase(di->ind);
    // glCallLists(di->siz, GL_INT, GL_IndTab);
    // glListBase(0);

    glCallLists(di->siz, GL_UNSIGNED_INT, &GL_IndTab[di->ind]);

    glPopMatrix();

  glEndList();

}


/*  Version mit glMultMatrixd (m1);  


//============================================================================
  void GL_DrawModel (long *ind, int att, ModelRef *mdr, ModelBas *mdb) {
//============================================================================
// ind    = DL-Ind des Ditto;
// p1     = Position (der neue Nullpunkt)
// az     = Verdrehwinkel um die Z-Achse in Grad.
// mir    = Spiegeln; ' '=keine, 'X'=um X-Z-Ebene spiegeln, 'Y'=um Y-Z-Ebene.

// di.po  = Origin (Verdreh-Nullpunkt, der Original-Nullpunkt des Basisdetails)
// di.ind = der Startindex der ObjListe
// di.siz = Anzahl Objekte


// Beisp:
// di1.ind = GL_GetActInd();       // akt.Index+1 fuer naechstes Obj.
// GR_Cre...                       // nun alle Obj generieren
// di1.siz = GL_GetActInd() - di1.ind;    // Anzahl Obj
// di1.po  = <ditto_Origin>;
// GL_DrawDitto2 (&dli, &p1, 0., ' ', &di1);




  int        irot, iscl;
  double     az1, ay, az2, scl;
  Point      pmb, pmr;
  Vector     vcx, vcz;
  GLuint     DL_ind;
  Mat_4x4    m1;


  printf("GL_DrawModel %d %s\n",mdr->modNr,mdb->mnam);
  UT3D_stru_dump (Typ_Model, mdr, "mdr:\n");
  UT3D_stru_dump (Typ_SubModel, mdb, "mdb:\n");
  UT3D_stru_dump(Typ_PT, &mdr->po, "  po=");
  // UT3D_stru_dump(Typ_VC, &mdr->vx, "  vx=");
  // UT3D_stru_dump(Typ_VC, &mdr->vz, "  vz=");
  // printf("  mdb %d po=%f,%f,%f\n",mdr->modNr,mdb->po.x,mdb->po.y,mdb->po.z);
  // printf("  mdb ind %d siz %d\n",mdb->DLind,mdb->DLsiz);


  // den DL-Index (+ Offset) holen)
  DL_ind = GL_fix_DL_ind (ind);
  // printf(" GL_DrawModel DL_ind=%d\n",DL_ind);


  pmb = mdb->po;
  pmr = mdr->po;

  vcx = mdr->vx;
  vcz = mdr->vz;


  // die relative Verschiebung von pmr in eine absolute umwandeln
  if(WC_sur_ind != 0) {
    UT3D_vc_travcm3 ((Vector*)&pmr, WC_sur_mat, (Vector*)&pmr);
    // UT3D_vc_travcm3 (&vcx, WC_sur_imat, &vcx);
    // UT3D_vc_travcm3 (&vcz, WC_sur_imat, &vcz);
    UT3D_stru_dump(Typ_PT, &pmr, "  transl.pmr=");
    UT3D_stru_dump(Typ_VC, &vcx, "  transl.vcx=");
    UT3D_stru_dump(Typ_VC, &vcz, "  transl.vcz=");
  }


  // wenn vz=Nullvektor, keine Transformation
  if(GRU_angr3_vxvz (&az1, &ay, &az2, &vcx, &vcz) >= 0) {
    irot = 1;

  } else {
    irot = 0;
  }

  // if(UTP_comp2db(mdr->scl, 1., UT_TOL_min1) == 0) {
  if(mdr->scl != 1.) {
    scl = mdr->scl;
    iscl = 1;
  } else {
    iscl = 0;
  }



  UT3D_m4_loadpl (m1, &GL_constr_pln);
    // UT3D_stru_dump (Typ_M4x4, m1, "new m1:");
  glMultMatrixd (m1);





  glNewList (DL_ind, GL_COMPILE);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();


    // // Verschieben - passiert NACH Drehung
    // // glTranslated (mdr->po.x, mdr->po.y, mdr->po.z);
    // glTranslated (pmr.x, pmr.y, pmr.z);


    // // Verschiebung in den aktuellen Nullpunkt (NACH Drehung)
    // if(WC_sur_ind != 0) {
      // glTranslated (WC_sur_act.po.x, WC_sur_act.po.y, WC_sur_act.po.z);
    // }


    glMultMatrixd (m1);


    // // Verschiebung in den absoluten Nullpunkt
    // if(WC_sur_ind != 0) {
      // glTranslated (-WC_sur_act.po.x, -WC_sur_act.po.y, -WC_sur_act.po.z);
    // }

    // // Verschiebung in den Nullpunkt - passiert VOR Drehung !
    // // um den Nullpunkt zurueck
    // glTranslated (-mdb->po.x, -mdb->po.y, -mdb->po.z);

    // printf(" GL_DrawModel-DL von %d siz %d\n",mdb->DLind,mdb->DLsiz);


    glCallLists(mdb->DLsiz, GL_UNSIGNED_INT, &GL_IndTab[mdb->DLind]);
    // // nur Test: skip 1 obj ..
    // glCallLists((mdb->DLsiz)-3, GL_UNSIGNED_INT, (&GL_IndTab[mdb->DLind])+3);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

  glEndList();

}
*/


//============================================================================
  int GL_DrawModel (long *ind, int att, ModelRef *mdr, ModelBas *mdb) {
//============================================================================
// ind    = DL-Ind des Ditto;
// p1     = Position (der neue Nullpunkt)
// az     = Verdrehwinkel um die Z-Achse in Grad.
// mir    = Spiegeln; ' '=keine, 'X'=um X-Z-Ebene spiegeln, 'Y'=um Y-Z-Ebene.

// di.po  = Origin (Verdreh-Nullpunkt, der Original-Nullpunkt des Basisdetails)
// di.ind = der Startindex der ObjListe
// di.siz = Anzahl Objekte


// Beisp:
// di1.ind = GL_GetActInd();       // akt.Index+1 fuer naechstes Obj.
// GR_Cre...                       // nun alle Obj generieren
// di1.siz = GL_GetActInd() - di1.ind;    // Anzahl Obj
// di1.po  = <ditto_Origin>;
// GL_DrawDitto2 (&dli, &p1, 0., ' ', &di1);


// Man muss das Model zuerst in den ABSOLUTEN Nullpunkt schieben;
// dann drehen (die Oriientiereung ist mdr.vx-vz);
// dann wieder zurueck und auf die Position.



// -------------
// um die RefPos verschieben (in den Nullpunkt); dann drehen, dann auf
// Refpos setzen .. ???
// Wurde da mdb.po mit mdr.po verwechselt ?
// Also neu: um mdb.po verschieben (in den Nullpunkt); dann drehen,
//   dann auf mdr.po verschieben ?
// Oder, wenns zuerst ohne Verschiebung und hidden hingezeichnet wird:
//   gleich drehen und um mdr.po verschieben ?
// Wozu ist der mdb.po ?




  int        irot, iscl;
  long       lstSiz;
  double     az1, ay, az2, scl;
  Point      pmb, pmr;
  Vector     vcx, vcz;
  GLuint     DL_ind;
  void       *lstPos;


  // printf("\nGL_DrawModel %d |%s|\n",mdr->modNr,mdb->mnam);
  // UT3D_stru_dump (Typ_Model, mdr, "mdr:\n");
  // UT3D_stru_dump (Typ_SubModel, mdb, "mdb:\n");
  // UT3D_stru_dump(Typ_PT, &mdr->po, "  po=");
  // UT3D_stru_dump(Typ_VC, &mdr->vx, "  vx=");
  // UT3D_stru_dump(Typ_VC, &mdr->vz, "  vz=");
  // printf("  mdb %d po=%f,%f,%f\n",mdr->modNr,mdb->po.x,mdb->po.y,mdb->po.z);
  // printf("  mdb ind %ld siz %ld\n",mdb->DLind,mdb->DLsiz);
  // printf("  GL_IndTab[%ld]=%ld\n",mdb->DLind,GL_IndTab[mdb->DLind]);
  // printf("WC_sur_ind=%d\n",WC_sur_ind);



  
  lstPos = &GL_IndTab[mdb->DLind];
  lstSiz = mdb->DLsiz;


  if(lstSiz < 0) {
    return MSG_STD_ERR (ERR_subModel_undefined, " sm '%s'", mdb->mnam);
  }


  // den DL-Index (+ Offset) holen)
  DL_ind = GL_fix_DL_ind (ind);
  // printf(" GL_DrawModel DL_ind=%d\n",DL_ind);


  pmb = mdb->po;
  pmr = mdr->po;

  vcx = mdr->vx;
  vcz = mdr->vz;


  if(WC_sur_ind != 0) {
    // die relative Verschiebung von pmr in eine absolute umwandeln
    UT3D_vc_travcm3 ((Vector*)&pmr, WC_sur_mat, (Vector*)&pmr);
    // die relative Orientiereung des Model in eine absolute umwandeln
    UT3D_vc_travcm3 (&vcx, WC_sur_mat, &vcx);
    UT3D_vc_travcm3 (&vcz, WC_sur_mat, &vcz);
    // UT3D_stru_dump(Typ_PT, &pmr, "  transl.pmr=");
    // UT3D_stru_dump(Typ_VC, &vcx, "  transl.vcx=");
    // UT3D_stru_dump(Typ_VC, &vcz, "  transl.vcz=");
  }


  // wenn vz=Nullvektor, keine Transformation
  if(GRU_angr3_vxvz (&az1, &ay, &az2, &vcx, &vcz) >= 0) {
    irot = 1;

  } else {
    irot = 0;
  }

  // if(UTP_comp2db(mdr->scl, 1., UT_TOL_min1) == 0) {
  if(mdr->scl != 1.) {
    scl = mdr->scl;
    iscl = 1;
  } else {
    iscl = 0;
  }


  glNewList (DL_ind, GL_COMPILE);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();


    // Verschieben auf die ModelReferencePosition (zuletzt)
    // glTranslated (mdr->po.x, mdr->po.y, mdr->po.z);
    glTranslated (pmr.x, pmr.y, pmr.z);


    // Verschiebung in den aktuellen Nullpunkt (NACH Drehung)
    if(WC_sur_ind != 0) {
      glTranslated (WC_sur_act.po.x, WC_sur_act.po.y, WC_sur_act.po.z);
    }


    // das model wurde in den Nullpunkt verschoben; nun die richtige
    // Orientierung hestellen (ex RefSys der Modelreference)
    if(irot != 0) {
        // printf(" gl-mod-rot %f %f %f\n",az1,ay,az2);
      glRotated (az1, 0.0,  0.0, 1.0);   // um Z drehen
      glRotated (ay,  0.0,  1.0, 0.0);   // um Y drehen
      glRotated (az2, 0.0,  0.0, 1.0);   // um Z drehen
    }


    // if(mir != ' ') {
      // if(mir == 'X') {
        // glScaled (1., -1., 1.);     // X-Z - Spiegelung
      // } else if(mir == 'Y') {
        // glScaled (-1., 1., 1.);     // Y-Z - Spiegelung
      // } else if(mir == 'Z') {
        // glScaled (1., 1., -1.);     // X-Y - Spiegelung
      // }
    // }


    // Verschiebung in den aktuellen Nullpunkt (UCS) - passiert VOR Drehung !
    // um den Nullpunkt zurueck
    glTranslated (-mdb->po.x, -mdb->po.y, -mdb->po.z);

    // printf(" GL_DrawModel-DL von %d siz %d\n",mdb->DLind,mdb->DLsiz);

    if(iscl != 0) {
      scl = mdr->scl;
      glScaled (scl, scl, scl);     // Scale 
      // printf("Scale: %f %f\n",mdr->scl,scl);
    }

    // DL_start = DL_base_mod + mdb->DLind;
    // glCallLists(mdb->DLsiz, GL_UNSIGNED_INT, &GL_IndTab[DL_start]);

    // if(mdb->DLsiz > GL_TAB_SIZ) {
      // if(GL_alloc__ (mdb->DLsiz) < 0) return -1;
    // }

    // glListBase(mdb->DLind);
    // glCallLists(mdb->DLsiz, GL_UNSIGNED_INT, GL_IndTab);
    // glListBase(0);

    glCallLists(lstSiz, GL_UNSIGNED_INT, lstPos);
    // // nur Test: skip 1 obj ..
    // glCallLists((mdb->DLsiz)-3, GL_UNSIGNED_INT, (&GL_IndTab[mdb->DLind])+3);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

  glEndList();

  return 0;

}


//================================================================
  int GL_set_bgCol (int mode) {
//================================================================
// set background-color; 0=OK, else Error.


  printf("GL_set_bgCol %d\n",mode);


  if(mode)
    // mode = 1 = error
    glClearColor (GL_col_be[0], GL_col_be[1], GL_col_be[2], GL_col_be[3]);
  else
    // mode = 0 = OK
    glClearColor (GL_col_bg[0], GL_col_bg[1], GL_col_bg[2], GL_col_bg[3]);

  return 0;

}


//****************************************************************************
  void GL_Clear () {
//****************************************************************************

  // printf("GGGGGGGGGGGGGGG GL_Clear\n");


  // Nicht im "store-tesselated-Vertices"-Modus
  if(TSU_get_mode() != 0) return;


  // die Displist löschen
  GL_Del0 (Typ_ALL_OBJS);

  GL_temp_delete ();

  GL_Init1 ();


  //glEnable (GL_DEPTH_TEST);

  /* Hintergrundfarbe */
  //glClearColor (GL_col_bg[0], GL_col_bg[1], GL_col_bg[2], GL_col_bg[3]);
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  
    glColor3fv    (GL_col_tab[13]);            // die def.-Colour
    // actCol.cr = 0;
    // actCol.cg = 0;
    // actCol.cb = 0;
    // newCol.cr = 0;
    // newCol.cg = 0;
    // newCol.cb = 0;

    glLineWidth   (1.0f);                       // 1 ist am duennsten !
    glLineStipple (1, (GLushort)0xFFFF);       // solid

/*
  // Flush drawing commands
  glFlush();
*/
 
}


//================================================================
  int GL_Init_Siz () {
//================================================================
// GL_Init_Siz           init clipping planes


  // printf("GL_Init_Siz %f\n",GL_ModSiz);


  // Prevent a divide by zero
  if(GL_Scr_Siz_Y == 0) GL_Scr_Siz_Y = 1;


  if (GL_Scr_Siz_X < GL_Scr_Siz_Y) {
    GL_Siz_X = GL_ModSiz / 2.;
    GL_Siz_Y = GL_ModSiz * GL_Scr_Siz_Y / GL_Scr_Siz_X / 2.;

  } else {
    GL_Siz_X = GL_ModSiz * GL_Scr_Siz_X / GL_Scr_Siz_Y / 2.;
    GL_Siz_Y = GL_ModSiz / 2.;

  }

  // Wert zB 1: CutPlane die Obj schneidet.
  GL_Siz_Z = GL_ModSiz * 1000;          // normal 2010-12-05
    // printf(" GL_Siz_Z = %f\n",GL_Siz_Z);
  // GL_Siz_Z = GL_ModSiz * 10;      // TEST 2008-03-02
  // GL_Siz_Z = GL_ModSiz / 10;      // TEST 2008-03-02
  // je kleiner GL_Siz_Z, desto schaerfer sind die Flaechenraender !



  return 0;

}


//================================================================
  void GL_Resize (int w, int h) {
//================================================================
// Called by Windows when it receives the WM_SIZE message.
// Put any code needed here to recalc the viewing volume and
// viewport info


  // printf("GL_Resize %d %d\n",w,h);


  if(w > 0) {
    GL_Scr_Siz_X = w;
    GL_Scr_Siz_Y = h;
  }



  GL_Init_Siz ();       // init clipping planes



  // ScreenVerzerrungsfaktur (fuer glortho)
  if (GL_Scr_Siz_X <= GL_Scr_Siz_Y) {
    GL_Svfakt = GL_Scr_Siz_Y/GL_Scr_Siz_X;
  } else {
    GL_Svfakt = GL_Scr_Siz_X/GL_Scr_Siz_Y;
  }



  // Set Viewport to window dimensions
  glViewport(0, 0, (float)GL_Scr_Siz_X, (float)GL_Scr_Siz_Y);

  glGetIntegerv (GL_VIEWPORT, GL_Viewp);            // get Viewport-Matrix
  /* TX_Print("GL_Viewp %d %d %d %d",GL_Viewp[0], */
                                  /* GL_Viewp[1],GL_Viewp[2],GL_Viewp[3]); */



  // Reset coordinate system
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  GL_Reframe ();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();


  ++GL_ViewModTimStamp;    // view modified ..


}


//=====================================================================
  void GL_InitModelSize (double NewModSiz, int mode) {
//=====================================================================
// mode 0=init, 1=change.

  double d1;

  // printf("GL_InitModelSize %f %d %f\n",NewModSiz,mode,GL_ModSiz);

  // Abarbeiten nicht aktiver subModels: nix tun ?
  // if(WC_modact_ind >= 0) return;


  GL_initMode = mode; // change scale or not


  if(mode == 1) {
    if(UTP_comp2db(GL_ModSiz, NewModSiz, 0.1)) return;
  }



  // ModSiz holen. Wenn sie sich geändert hat, ReInit erforderlich !
    UI_GR_DrawInit ();
    if(mode == 1) {
      d1 = GL_ModSiz / NewModSiz;
      GL_Set_Scale (GL_Scale / d1);
    }


    GL_ModSiz = NewModSiz;

    GL_SclNorm = GL_ModSiz / 500.;   // fuer immer gleich grosse Vektoren ..


    // if(WC_modact_ind < 0) {  // Achtung: DL loeschen loescht SubModels !!!!
      GL_Init__ (1, (int)GL_Scr_Siz_X, (int)GL_Scr_Siz_Y);
      // GL_Redraw ();
      // TX_Print("Modelsize %f",NewModSiz);
    // }

    UI_GR_DrawExit ();

    // display modelsize in gtk-label
    UI_disp_modsiz ();

}


//================================================================
  int GL_InitCol (int ic) {
//================================================================
// muss innerhalb glNewList gerufen werden ..


  // printf("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC \n");
  // printf("GL_InitCol %d\n",ic);




  // die DefCol GL_defCol setzen ..
/*
  AP_SetCol3i ((int)((double)0.88*255.),
               (int)((double)0.88*255.),
               (int)((double)0.95*255.), 1);  // 0.88,0.88,0.95
*/
  AP_SetCol3i ((int)(GL_col_tab[12][0]*255.f),
               (int)(GL_col_tab[12][1]*255.f),
               (int)(GL_col_tab[12][2]*255.f), 1);  // 0.88,0.88,0.95

  GL_actCol = GL_defCol;

  glDisable (GL_BLEND);          // fuer nach einem transparenten Model ..
  glEnable (GL_DEPTH_TEST);
  glEnable (GL_LIGHTING);
  GL_stat_blend = 0;

  GL_Col__ (ic);

  return 0;

}


//================================================================
  int GL_Col__ (int ic) {
//================================================================

  glColor3fv    (GL_col_tab[ic]);

  return 0;

}


//================================================================
  void GL_InitPtAtt (int ithick) {
//================================================================
/// create DL-records DL_base_PtAtt until DL_base_font2

// den DispListRecord DL_base_PtAtt korrigieren;
// enthaelt PunktSize; wird von jedem GL_DrawPoint gerufen.
// Sonderfunktionen:
//   ithick = -3 = initial create of all points
//   ithick = -2 = hilite
//   ithick = -1 = reset nach hilite
//   ithick =  0 = reset nach hilite


static GLfloat  hiliThick = 6.f, stdThick = 5.f, iniThick = 5.f;

  int      iCol;
  GLfloat  actThick;
  GLuint   dli;
  

  // printf("GL_InitPtAtt %d std=%f\n",ithick,stdThick);


  // glDisable (GL_BLEND);          // fuer nach einem transparenten Model ..
  // GL_stat_blend = 0;


  dli = DL_base_PtAtt;  // iatt = 0;

  //----------------------------------------------------------------
  // init all point-types  0=default; 1=hili; 2=dimmed; 3=green; 4=yellow.
  if(ithick == -3) {

    // DL_base_PtAtt; iatt=0
    glNewList (dli, GL_COMPILE);
    glColor3fv  (GL_col_tab[0]);
    glPointSize (stdThick);
    glEndList ();

    ++dli;
    // DL_base_PtAtt + 1; iatt=1;
    glNewList (dli, GL_COMPILE);
    glColor3fv  (GL_col_tab[ATT_COL_HILI]);
    glPointSize (hiliThick);
    glEndList ();

    ++dli;
    // DL_base_PtAtt + 2; iatt=2;
    glNewList (dli, GL_COMPILE);
    glColor3fv  (GL_col_tab[ATT_COL_DIMMED]);
    glPointSize (hiliThick);
    glEndList ();

    ++dli;
    // DL_base_PtAtt + 3; iatt=3;
    glNewList (dli, GL_COMPILE);
    glColor3fv  (GL_col_tab[ATT_COL_GREEN]);
    glPointSize (hiliThick);
    glEndList ();

    ++dli;
    // DL_base_PtAtt + 4; iatt=4;
    glNewList (dli, GL_COMPILE);
    glColor3fv  (GL_col_tab[ATT_COL_YELLOW]);
    glPointSize (hiliThick);
    glEndList ();

    return;


  //----------------------------------------------------------------
  } else if(ithick == -2) {
    iCol = 9;
    actThick = hiliThick;
    // return;


  //----------------------------------------------------------------
  } else if(ithick == -1) {
    iCol = 0;
    actThick = stdThick;


  //----------------------------------------------------------------
  } else if(ithick == 0) {
    iCol = 0;
    actThick = iniThick;
    stdThick = actThick;   // 2011-07-14


  //----------------------------------------------------------------
  } else {
    if(UI_InpMode != UI_MODE_VWR) return;
    iCol = 0;
    actThick = ithick;
    stdThick = actThick;   // 2011-07-14
  }

  // actThick = 1;  // TEST ONLY

  // printf("PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP\n");
  // printf("GL_InitPtAtt %d actThick=%f\n",ithick,actThick);


  glNewList (dli, GL_COMPILE);
  glColor3fv  (GL_col_tab[iCol]);
  glPointSize (actThick);
  glEndList ();

}


//================================================================
  int GL_InitNewAtt (int mode, int Ind) {
//================================================================
/// \code
/// create / change line-attribute in DL.
/// Input:
///   mode    0 = create / restore attribute-record
///           1 = change this attribute to hilited
///           2 = als dim anlegen.                 UNUSED
///           3 = als Hintergrundfarbe anlegen.    UNUSED
///   Ind  is die AttributNummer;
///      ein index into GR_AttTab
///
/// all attributes have a record in the displist (glNewList);
/// att.0 has dli=DL_base_LnAtt; last index is (DL_base_PtAtt - 1).
/// To activate an attribute call record (DL_base_LnAtt + attrib-nr)
/// \endcode

// Problematisch: Flaechen und Lines/Curves mit gleichem Attributindex.
//  Flaechen mit glLineWidth - sind oft komisch; 
// zB werden Flaechen nach einer CCV immer mit dicken Lines gezeichnet ...

  int      col, typ;
  GLfloat  thick, fCol[4];

  GLuint   dli;

  
  // printf("GL_InitNewAtt %d %d\n",mode,Ind);
  // printf("GL_InitNewAtt %d %ld col=%d ltyp=%d thi=%d\n",mode,Ind,
          // GR_AttLnTab[Ind].col,GR_AttLnTab[Ind].ltyp,GR_AttLnTab[Ind].lthick);


  //check if defined
  // if(GR_AttLnTab[Ind].lim > 2) {
    // TX_Print("GL_InitNewAtt I001 %ld",Ind);
    // return -1;
  // }


  // check for max Ind
  dli = DL_base_LnAtt + Ind;
  if(dli >= DL_base_PtAtt) {
    TX_Error("GL_InitNewAtt E001 %d",Ind);
    return -1;
  }

  glNewList (dli, GL_COMPILE);
    // printf(" _InitNewAtt dli=%d\n",dli);

  // fix Linetyp ....
  // typ   = GR_AttTab[Ind].ltyp;
  typ = GR_AttLnTab[Ind].dash;

  if(typ > 0) {
    glEnable (GL_LINE_STIPPLE);             // Enable Strich-Punkt
    glLineStipple (LtypTab1[typ], LtypTab2[typ]);
      // printf("  typ=%d stipple %d %d\n",typ,LtypTab1[typ],LtypTab2[typ]);

  } else {
    glDisable (GL_LINE_STIPPLE);              // Disable Strich-Punkt
  }




    
  //================================================================
  // normal  -  create / restore attribute-record
    if(mode == 0) {
      // activate line-color
      fCol[0] = (float)GR_AttLnTab[Ind].cr / 9;
      fCol[1] = (float)GR_AttLnTab[Ind].cg / 9;
      fCol[2] = (float)GR_AttLnTab[Ind].cb / 9;
      fCol[3] = 1.f;
      glColor3fv (fCol);
      // activate line-thickness
      thick = GR_AttLnTab[Ind].thick;
      glLineWidth (thick);        // 1 ist am duennsten !
        // printf(" glLineWidth %lf\n",thick);



  //================================================================
  //           1 = change this attribute to hilited
    } else if (mode == 1) {
      glColor3fv   (GL_col_tab[Typ_Att_hili1]);  // hili-Farbe
      glLineWidth   (4.0);                         // 1 ist am duennsten !



  //================================================================
  // DIMMED
    } else if (mode == 2) {
      glColor3fv   (GL_col_tab[Typ_Att_dim]);  // dim-Farbe
      thick = GR_AttLnTab[Ind].thick;
      // glLineWidth   (thick);        // 1 ist am duennsten !
      glLineWidth   (1.0);                         // 1 ist am duennsten !


/*   UNUSED
  //================================================================
    } else if (mode == 3) {
      glDisable (GL_LIGHTING);
      glColor3fv   (GL_col_bg);
      // glLineWidth   (1.0);                         // 1 ist am duennsten !
*/



  //================================================================
  // DIMMED
    } else { TX_Error("GL_InitNewAtt E002 %d",mode); return -1; }


  glEndList ();

  return 0;

}


//================================================================
  int GL_AttTab_dump__ () {
//================================================================
 
  int     i1, i2, i3, ii;


  printf("GL_AttTab_dump__ \n");

  for(ii=0; ii<GR_ATT_TAB_SIZ; ++ii) {
    // if(GR_AttTab[ii].used != ON) continue;
    AttLn_Get_ind (&i1, &i2, &i3, ii);
    // if(GR_AttLnTab[i1].uu == ON) continue;
    if(i1 == 0) continue;
    printf(" AT[%2d] col=%2d ltyp=%d thick=%d\n",ii,i1,i2,i3);
  }


  return 0;

}


//=======================================================================
  void  GL_InitAFont () {
//=======================================================================
// Auf die DL-Plaetze 1-126 die Characterbitmaps laden
// 0-31 bleiben aber frei !


  GLuint i1;

  //TX_Print("GL_InitAFont");

  glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

  /* je einen char in je eine DL-Pos laden; 0-31 bleiben aber frei ! */
  for (i1 = 32; i1 < 127; i1++) {
    /* TX_Print("  loadchar %d\n",i1); */
    glNewList (i1, GL_COMPILE);
      /* breite,h÷he,x_o, y_o, x_incr, y_incr,  adress */
    glBitmap (8, 13, 0.f, 2.f, 10.f, 0.f, bitmap_font1[i1-32]);
    glEndList ();
  }


}


//=======================================================================
  void  GL_InitGFont () {
//=======================================================================
// die GrafCharacters auf DL-Pos 200-296 beginnend mit dem blank laden.



  int     i1, ianz, ilen;
  char    *p1, *p2, mode[50];
  double  cx[50], cy[50];




  DL_FontOff = DL_base_font2-32;   // 200-32




  // printf("GGGGGGGGGGGGGGGGG GL_InitGFont\n");




  // fuer alle Syms call GL_InitGF2
    
  for(i1=0; i1<128; ++i1) {

    p1=GL_vfont1[i1];
    if(p1 == NULL) break;
    
    ilen=strlen(p1);

    // printf(" %d |%s| len=%d\n",i1,p1,ilen);

    ianz = 0;
    
    L_Next:
    if(*p1 == ' ') {
      ++p1;
      goto L_Next;
    } 

    if((p1 - GL_vfont1[i1]) >= ilen) goto L_Work0;

    // Zeile zerlegen; Format immer: M od L X-Wert Y-Wert
    mode[ianz] = *p1;
    ++p1;

    cx[ianz] = strtod (p1,&p2); p1 = p2; ++p1;
    cy[ianz] = strtod (p1,&p2); p1 = p2;
    if(ianz >= 50) printf("GL_InitGFont E001 |%s|\n",p1);
    else ++ianz;
    goto L_Next;


    //---------------------------------------
    L_Work0:
    // printf(" L_Work %d\n",ianz);
    GL_InitGF2 (i1, ianz, mode, cx, cy);

  }

  return;

}


//=================================================================
  int GL_InitGF2 (int ind, int ianz, char mode[], double cx[], double cy[]) {
//=================================================================
// in: table of m od l plus cx,cy
  
  int      i2, ianf, iend;
  GLuint   DL_ind;


  DL_ind = (GLuint) DL_FontOff + 32 + ind;


  // printf("GL_InitGF2 ------------- %d\n",DL_ind);
  


  glNewList (DL_ind, GL_COMPILE);




  //----------------------------------------
  if(ianz < 2) goto Fertig;


  ianf=0;
  iend=1;

  
  // den naechsten 'm' suchen
  L_Work1:
  ++iend; 
  if(iend >= ianz) goto L_Work2;
  if(mode[iend] == 'l') goto L_Work1;




  L_Work2:     // VON ianf - (iend-1) ausgeben
  
  glBegin (GL_LINE_STRIP);
  // printf(" ----\n");

  for(i2=ianf; i2<iend; ++i2) {
    // printf(" l %d = %f %f\n",i2,cx[i2],cy[i2]);
    glVertex3d (cx[i2], cy[i2], 0.0);
  }

  glEnd ();


  if(iend < ianz) {
    ianf=iend;
    goto L_Work1;
  }




  //----------------------------------------
  Fertig:

  glEndList ();


  return 0;



}


//***********************************************************************
  void GL_InitSymb () {
//***********************************************************************
// 1) Alle bitmapbasierenden Symbole generieren (als DL-objekt speichern)
// 2) Alle vektorbasierenden Symbole generieren (als DL-objekt speichern)


  double d1, d2, d3, d4;
  GLuint DL_ind;
  Point  p1, p2, p3, p4, po = {0.0, 0.0, 0.0};


  // printf("GL_InitSymb \n");




  /* ----------------  SYM_TRI_S ------------------------------------ */
  DL_ind = (GLuint)SYM_TRI_S;
  glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

  glNewList (DL_ind, GL_COMPILE);
    /* breite,hoehe,x_o, y_o, x_incr, y_incr,  adress */
  glBitmap (8, 8, 4.0f, 4.0f, 0.0f, 0.0f, bitmap_symb_tri_s);
  glEndList ();




  /* --------------  SYM_STAR_S -------------------------------------- */
  DL_ind = (GLuint)SYM_STAR_S;
  glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

  glNewList (DL_ind, GL_COMPILE);
    /* breite,hoehe,x_o, y_o, x_incr, y_incr,  adress */
  glBitmap (8, 8, 4.0f, 4.0f, 0.0f, 0.0f, bitmap_symb_star_s);
  glEndList ();



  // --------------  SYM_CIR_S --------------------------------------
  DL_ind = (GLuint)SYM_CIR_S;
    // printf(" SYM_CIR_S=%d\n",DL_ind);

  glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

  glNewList (DL_ind, GL_COMPILE);
    /* breite,hoehe,x_o, y_o, x_incr, y_incr,  adress */
  glBitmap (8, 8, 4.0f, 4.0f, 0.0f, 0.0f, bitmap_symb_circ_s);
  glEndList ();





  /* --------------  SYM_TRI_B -------------------------------------- */
  DL_ind = (GLuint)SYM_TRI_B;
  glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

  glNewList (DL_ind, GL_COMPILE);
    /* breite,hoehe,x_o, y_o, x_incr, y_incr,  adress */
  // glBitmap (16, 16, 8.0f, 8.0f, 0.0f, 0.0f, bitmap_symb_tri_b);
  glBitmap (10, 10, 5.0f, 5.0f, 0.0f, 0.0f, bitmap_symb_tri_b);
  glEndList ();




  //----------------------------------------------------------------
  // SYM_ARRO3H     3D-arrowHead
  DL_ind = (GLuint)SYM_ARRO3H;
  // d1 = GL_ModScale / 3.;
  d1 = 50.;
  d2 = d1 / 4.;
  d3 = d2 / 2.;
  d4 = d3 * SR_3;
    // printf(" %f %f %f %f\n",d1,d2,d3,d4);

  glNewList (DL_ind, GL_COMPILE);

    glPushMatrix ();
    glCallList (DL_Ind_ScBack);           // ScaleBack

    // glPushAttrib(GL_POLYGON_BIT);              // Polygonmode usw merken
    // glDisable (GL_LIGHTING);                   // das folgende nicht rendern

    p1.x=d1;  p1.y=0.;  p1.z=0.;
    p2.x=d1;  p2.y=0.;  p2.z=d2;
    p3.x=d1;  p3.y=d4;  p3.z=-d3;
    p4.x=d1;  p4.y=-d4; p4.z=-d3;
      // UT3D_stru_dump (Typ_PT, &p1, " P1: ");
      // UT3D_stru_dump (Typ_PT, &p2, " P2: ");
      // UT3D_stru_dump (Typ_PT, &p3, " P3: ");
      // UT3D_stru_dump (Typ_PT, &p4, " P4: ");

    glBegin (GL_LINE_STRIP);
      glVertex3dv ((double*)&po);
      glVertex3dv ((double*)&p3);
      glVertex3dv ((double*)&p1);
      glVertex3dv ((double*)&p2);
      glVertex3dv ((double*)&po);
      glVertex3dv ((double*)&p4);
      glVertex3dv ((double*)&p1);
    glEnd ();

    // glPopAttrib();

    glPopMatrix ();  // vom ScaleBack

  glEndList ();








  // SYM_VEC - Pfeil ==================================
  DL_ind = (GLuint)SYM_VEC;
  //TX_PrintF(" SYM_VEC siz=%f %f",d1,d2);

  glNewList (DL_ind, GL_COMPILE);

    p1.x=10.;  p1.y= 0.0;  p1.z= 0.0;
    p2.x= 9.;  p2.y= 0.2;  p2.z= 0.2;
    p3.x= 9.;  p3.y=-0.2;  p3.z= 0.0;
    p4.x= 9.;  p4.y= 0.0;  p4.z=-0.2;

    glBegin (GL_LINE_STRIP);
      glVertex3dv ((double*)&po);
      glVertex3dv ((double*)&p1);
      glVertex3dv ((double*)&p2);
      glVertex3dv ((double*)&p3);
      glVertex3dv ((double*)&p1);
      glVertex3dv ((double*)&p4);
    glEnd ();

  glEndList ();








  /* ---------- Achsen-Symbol, # SYM_AXIS ------------------ */
  // Axis mit X,Y,Z-Char's
  DL_ind = (GLuint)SYM_AXIS;
  d1 = GL_ModScale / 1.5;      // / 2.;

  glNewList (DL_ind, GL_COMPILE);

    glPushMatrix ();
    glCallList (DL_Ind_ScBack);           // ScaleBack

    p1.x=d1; p1.y=0.0; p1.z=0.0;
    glBegin (GL_LINES);
      glVertex3d (po.x, po.y, po.z);
      glVertex3d (p1.x, p1.y, p1.z);
    glEnd ();

    glRasterPos3d (p1.x, p1.y, p1.z);
    glCallList ((GLuint)120);           /* "x" */

    p1.x=0.0; p1.y=d1; p1.z=0.0;
    glBegin (GL_LINES);
      glVertex3d (po.x, po.y, po.z);
      glVertex3d (p1.x, p1.y, p1.z);
    glEnd ();

    // glRasterPos3d (p1.x, p1.y, p1.z);
    // glCallList ((GLuint)121);           /* "y" */

    p1.x=0.0; p1.y=0.0; p1.z=d1;
    glBegin (GL_LINES);
      glVertex3d (po.x, po.y, po.z);
      glVertex3d (p1.x, p1.y, p1.z);
    glEnd ();

    glRasterPos3d (p1.x, p1.y, p1.z);
    glCallList ((GLuint)122);           /* "z" */

    glPopMatrix ();  // vom ScaleBack

  glEndList ();




  /* ---------- Achsen-Symbol, # SYM_AXIS1 ------------------ */
  // Axis ohne Buchstaben (Sym.Rotcen)
  DL_ind = (GLuint)SYM_AXIS1;
  d1 = GL_ModScale / 2.;

  glNewList (DL_ind, GL_COMPILE);

    glPushMatrix ();
    glCallList (DL_Ind_ScBack);           // ScaleBack

    p1.x=d1; p1.y=0.0; p1.z=0.0;
    glBegin (GL_LINES);
      glVertex3d (po.x, po.y, po.z);
      glVertex3d (p1.x, p1.y, p1.z);
    glEnd ();

    p1.x=0.0; p1.y=d1; p1.z=0.0;
    glBegin (GL_LINES);
      glVertex3d (po.x, po.y, po.z);
      glVertex3d (p1.x, p1.y, p1.z);
    glEnd ();

    p1.x=0.0; p1.y=0.0; p1.z=d1;
    glBegin (GL_LINES);
      glVertex3d (po.x, po.y, po.z);
      glVertex3d (p1.x, p1.y, p1.z);
    glEnd ();

    glPopMatrix ();  // vom ScaleBack

  glEndList ();





  /* ---------- SYM_SQUARE ------------------ */
  DL_ind = (GLuint)SYM_SQUARE;
  d1 = GL_ModScale / 3.;

  glNewList (DL_ind, GL_COMPILE);

    glPushMatrix ();
    glCallList (DL_Ind_ScBack);           // ScaleBack

    glBegin (GL_LINE_LOOP);


      p1.x=d1; p1.y=d1; p1.z=0.0;
      glVertex3d (p1.x, p1.y, p1.z);

      p1.x=-d1; p1.y=d1;
      glVertex3d (p1.x, p1.y, p1.z);

      p1.x=-d1; p1.y=-d1;
      glVertex3d (p1.x, p1.y, p1.z);

      p1.x=d1; p1.y=-d1;
      glVertex3d (p1.x, p1.y, p1.z);

      //p1.x=d1; p1.y=d1;
      //glVertex3d (p1.x, p1.y, p1.z);

    glEnd ();

    glPopMatrix ();  // vom ScaleBack

  glEndList ();





  /* ---------- SYM_PLANE ----------------------------- */
  DL_ind = (GLuint)SYM_PLANE;
  d1 = GL_ModScale / 3.5;

  glNewList (DL_ind, GL_COMPILE);

    glPushMatrix ();
    glCallList (DL_Ind_ScBack);           // ScaleBack

    glPushAttrib(GL_POLYGON_BIT);              // Polygonmode usw merken
    glDisable (GL_LIGHTING);                   // das folgende nicht rendern
    // glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    // glColor3f (1.f, 1.f, 0.f);                 // gelb
    glBegin (GL_POLYGON);

      p1.x=d1; p1.y=d1; p1.z=0.0;
      glVertex3d (p1.x, p1.y, p1.z);

      p1.x=-d1; p1.y=d1;
      glVertex3d (p1.x, p1.y, p1.z);

      p1.x=-d1; p1.y=-d1;
      glVertex3d (p1.x, p1.y, p1.z);

      p1.x=d1; p1.y=-d1;
      glVertex3d (p1.x, p1.y, p1.z);

      //p1.x=d1; p1.y=d1;
      //glVertex3d (p1.x, p1.y, p1.z);

    glEnd ();
    glPopAttrib();

    glPopMatrix ();  // vom ScaleBack

  glEndList ();






  // SYM_CROSS = das Positionskreuz ==================================
  DL_ind = (GLuint)SYM_CROSS;
  d1 = GL_ModScale / 5.;

  glNewList (DL_ind, GL_COMPILE);

    glPushMatrix ();
    glCallList (DL_Ind_ScBack);           // ScaleBack

    p1.x=d1;  p1.y=d1;  p1.z=0.0;
    p2.x=-d1; p2.y=-d1; p2.z=0.0;
    glBegin (GL_LINES);
      glVertex3d (p1.x, p1.y, p1.z);
      glVertex3d (p2.x, p2.y, p2.z);
    glEnd ();

    p1.x=-d1; p1.y=d1;  p1.z=0.0;
    p2.x=d1;  p2.y=-d1; p2.z=0.0;
    glBegin (GL_LINES);
      glVertex3d (p1.x, p1.y, p1.z);
      glVertex3d (p2.x, p2.y, p2.z);
    glEnd ();

    glPopMatrix ();  // vom ScaleBack

  glEndList ();



  // SYM_TRIANG - ein Dreieck ==================================
  DL_ind = (GLuint)SYM_TRIANG;
  d1 = GL_ModScale / 3.;

  glNewList (DL_ind, GL_COMPILE);

    glPushMatrix ();
    glCallList (DL_Ind_ScBack);           // ScaleBack

    p1.x=0.0; p1.y=d1;  p1.z=0.0;
    p2.x=d1;  p2.y=-d1; p2.z=0.0;
    p3.x=-d1; p3.y=-d1; p3.z=0.0;

    glBegin (GL_LINE_LOOP);
      glVertex3d (p1.x, p1.y, p1.z);
      glVertex3d (p2.x, p2.y, p2.z);
      glVertex3d (p3.x, p3.y, p3.z);
    glEnd ();

/*
    glBegin (GL_LINES);
      glVertex3d (p1.x, p1.y, p1.z);
      glVertex3d (p2.x, p2.y, p2.z);
    glEnd ();

    glBegin (GL_LINES);
      glVertex3d (p2.x, p2.y, p2.z);
      glVertex3d (p3.x, p3.y, p3.z);
    glEnd ();

    glBegin (GL_LINES);
      glVertex3d (p3.x, p3.y, p3.z);
      glVertex3d (p1.x, p1.y, p1.z);
    glEnd ();
*/

    glPopMatrix ();  // vom ScaleBack

  glEndList ();



  // SYM_CROSS1 - "Schere" ==================================
  DL_ind = (GLuint)SYM_CROSS1;
  d1 = GL_ModScale / 4.;
  d2 = d1 / 0.5;

  glNewList (DL_ind, GL_COMPILE);

    glPushMatrix ();
    glCallList (DL_Ind_ScBack);           // ScaleBack

    p1.x=d1;  p1.y=d2;  p1.z=0.0;
    p2.x=-d1; p2.y=-d2; p2.z=0.0;
    p3.x=d1;  p3.y=-d2; p3.z=0.0;
    p4.x=-d1; p4.y=d2;  p4.z=0.0;

    glBegin (GL_LINE_STRIP);
      glVertex3d (p1.x, p1.y, p1.z);
      glVertex3d (p2.x, p2.y, p2.z);
      glVertex3d (p3.x, p3.y, p3.z);
      glVertex3d (p4.x, p4.y, p4.z);
    glEnd ();
/*
    glBegin (GL_LINES);
      glVertex3d (p1.x, p1.y, p1.z);
      glVertex3d (p2.x, p2.y, p2.z);
    glEnd ();

    glBegin (GL_LINES);
      glVertex3d (p2.x, p2.y, p2.z);
      glVertex3d (p3.x, p3.y, p3.z);
    glEnd ();

    glBegin (GL_LINES);
      glVertex3d (p3.x, p3.y, p3.z);
      glVertex3d (p4.x, p4.y, p4.z);
    glEnd ();
*/

    glPopMatrix ();  // vom ScaleBack

  glEndList ();




  // SYM_ARROH - Pfeilspitzen ==================================
  DL_ind = (GLuint)SYM_ARROH;
  d1 = GL_ModScale / 3.;
  d2 = d1 / 3.;
  //TX_PrintF(" SYM_ARROH siz=%f %f",d1,d2);

  glNewList (DL_ind, GL_COMPILE);

    glPushMatrix ();
    glCallList (DL_Ind_ScBack);           // ScaleBack

    p1.x=0.;  p1.y=0.;  p1.z=0.0;
    p2.x=d1;  p2.y=d2;  p2.z=0.0;
    p3.x=d1;  p3.y=-d2; p3.z=0.0;

    glBegin (GL_LINE_STRIP);
      glVertex3d (p3.x, p3.y, p3.z);
      glVertex3d (p1.x, p1.y, p1.z);
      glVertex3d (p2.x, p2.y, p2.z);
    glEnd ();

    glPopMatrix ();  // vom ScaleBack

  glEndList ();




  // SYM_ARROW - Pfeil ==================================
  DL_ind = (GLuint)SYM_ARROW;
  //TX_PrintF(" SYM_ARROW siz=%f %f",d1,d2);

  glNewList (DL_ind, GL_COMPILE);

    glPushMatrix ();
    glCallList (DL_Ind_ScBack);           // ScaleBack

    p1.x=10.;  p1.y= 0.0;  p1.z= 0.0;
    p2.x= 9.;  p2.y= 0.2;  p2.z= 0.2;
    p3.x= 9.;  p3.y=-0.2;  p3.z= 0.0;
    p4.x= 9.;  p4.y= 0.0;  p4.z=-0.2;

    glBegin (GL_LINE_STRIP);
      glVertex3dv ((double*)&po);
      glVertex3dv ((double*)&p1);
      glVertex3dv ((double*)&p2);
      glVertex3dv ((double*)&p3);
      glVertex3dv ((double*)&p1);
      glVertex3dv ((double*)&p4);
    glEnd ();

    glPopMatrix ();  // vom ScaleBack

  glEndList ();




  // SYM_LENGTH (true length, no scaleBack) =========================
  DL_ind = (GLuint)SYM_LENGTH;
  
  glNewList (DL_ind, GL_COMPILE);
    
    // glCallList (DL_Ind_Scl2D);           // ScaleBack

    p1.x= 0.;  p1.y= 0.0;  p1.z= 0.0;
    p2.x= 1.;  p2.y= 0.0;  p2.z= 0.0;
    // p3.x= 1.;  p3.y= 0.2;  p3.z= 0.0;
    // p4.x= 9.;  p4.y= 0.0;  p4.z=-0.2;

    glBegin (GL_LINE_STRIP);
      glVertex3dv ((double*)&p1);
      glVertex3dv ((double*)&p2);
    glEnd ();

  glEndList ();







  return;

}



//=====================================================================
  int GL_Exit__ () {
//=====================================================================
// gibt leider mem. im X-Server nicht mehr frei; glDeleteLists ist unnutz.
// Gibts free von gtk_gl_area_new ?
// Siehe auch GLU_tess_exit.
// Wer braucht das 

  long   l1;

  printf("GL_Exit__ %ld\n",GL_TAB_SIZ);

/*
 alles unnutz ..
  // glDeleteLists (1, GL_TAB_SIZ); // von anzahl

  for(l1=0; l1<GR_TAB_IND; ++l1) {
    if (glIsList(l1)) {
      glDeleteLists (l1, 1);
    }
  }
*/
  return 0;

}


//=====================================================================
  int GL_InitInfo () {
//=====================================================================

  printf("GL_InitInfo\n");


  printf("GL_RENDERER:    %s\n",glGetString(GL_RENDERER));
  printf("GL_VERSION:     %s\n",glGetString(GL_VERSION));
  // printf("GL_EXTENSIONS:  %s\n",glGetString(GL_EXTENSIONS));

  printf("GLU_VERSION:    %s\n",gluGetString(GLU_VERSION));
  printf("GLU_EXTENSIONS: %s\n",gluGetString(GLU_EXTENSIONS));

  return 0;
}



//=====================================================================
  void GL_Init__ (int mode, int width, int height) {
//=====================================================================
// mode=0: primaer,
// mode=1: change scale .. (do not kill DL)

  int    i1;
  char   cbuf1[256];


  // printf("GL_Init__ %d %d %d\n",mode,width,height);

  GL_pickSiz = 8;


  // UI_GR_DrawInit ();  is done outside ..  is done outside ..


  //----------------------------------------------------------------
  // startup: check for texture
  if(mode == 0) {

    // bei MS-Win Info erst mit angebundener Lib verfuegbar !
    GL_InitInfo (); // display GL-VENDOR .. VERSION .. EXTENSIONS
    // was gdk_gl_get_info ..

    // display GL-hardware-config,
    i1 = GL_config_test ();
      printf(" AP_stat.texture=%d\n",i1);

    AP_stat.texture = i1;
    if(i1 == 0) {
      // -) in bmp_load koennen dzt nut 24-bit-images geladen werden;
      // -) mit djpeg kann ein 24-bit-Image nicht in ein 16-bit-Image
      //    umgewandelt werden. Wie geht das ? Es sollte ev auch direkt (lesen)
      //    moeglich sein .. ?
      TX_Print
      ("***** opengl-binding cannot handle textures on this hardware *****");

    } else {
      AP_Get_Setup (cbuf1, "TEXTURE_ON");     // on or off
      i1 = atoi(cbuf1);
      if(i1 < 1) {
        printf ("***** textures disabled in gCAD3D.rc *****\n");
        AP_stat.texture = 0;
      }
    }

  }


  //----------------------------------------------------------------
  //GL_ModScale = 100000. / GL_ModSiz;
  GL_ModScale = 0.2 * GL_ModSiz;


  // // Define Background Colour
  // glClearColor (GL_col_bg[0], GL_col_bg[1], GL_col_bg[2], GL_col_bg[3]);

  // GL_Do_Reshape (width, height);


  // glClear (GL_DEPTH_BUFFER_BIT);      // gesamten Depth-Buffer löschen


  // glClearDepth (1.);               // 0. od 1.; Suerbible S.481; 
  glEnable (GL_DEPTH_TEST);           // fuer hidden Lines ..


  // { int ia[1];  ia[1234567] = 123;}   // TESTCRASH ONLY


  // bis 4.10.2003: GL_LEQUAL. Damit werden gehilitete Objekte sichtbar !
  glDepthFunc (GL_LEQUAL);              // eine Spur besser als LESS


  // glDepthFunc (GL_LESS);      // default; alte Linien sind stärker als neue!
  // glDepthFunc (GL_GREATER);           // damit sieht man NIX!

  // glDepthFunc (GL_ALWAYS);            // ueberschreibt immer (auch Flächen !)
  //glDepthFunc (GL_NEVER);             // damit sieht man NIX!
  //glDepthFunc (GL_GEQUAL);            // damit sieht man NIX!
  //glDepthFunc (GL_EQUAL);             // damit sieht man NIX!
  //glDepthFunc (GL_NOTEQUAL);          // ueberschreibt immer (auch Flächen !)



  // mit 1,-1 dreht er die Sichtbarkeit um - recht strange
  // glDepthRange ((GLclampd) - 1.0, (GLclampd) 1.0);

  // damit sind Lines over einer Bitmap sichtbar; Lines muessen aber trotzdem 
  // Z>0 haben !!
  // glDepthRange (1., 0.);


  // Antialiasing von Lines; macht sie aber sehr dick !
  // glEnable (GL_LINE_SMOOTH);


  // Antialiasing von Polygons; ohne werden die Boundary-lines aussen schoener.
  // glEnable (GL_POLYGON_SMOOTH);


  // Punkte nicht als Rechtecke sondern als Kreise
  // koennen sonst ganz feine Punkte sein !!
  // mit SMOOTH Punkte bei Tisler nicht sichtbar; gehts ohne ? 8.Apr.2002
  // glEnable (GL_POINT_SMOOTH);


  glEnable (GL_LINE_STIPPLE);   // Enable Strich-Punkt

  // Enable transparency
  // glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE);   // geht

  // Culling: nur Vorder oder Rueckseite einer Flaeche shaden;
  // spaeter, bei Render oder DispEdges ..
  // glDisable (GL_CULL_FACE);


  // alle Dreiecke umdrehen GL_CW; Def=GL_CCW ?
  // glFrontFace (GL_CCW);    



  if(GL_mode_wire_shade == GR_STATE_WIRE) {
    GL_DefineDisp (FUNC_DispWire, 0);    // Default = Wireframe
  } else {
    GL_DefineDisp (FUNC_DispRend, 0);    // Default = Wireframe
  }

  // Die DL-Basis neu anlegen.
  if(mode == 0) GL_Init0 ();

  GL_InitAFont ();

  DL_InitAttTab ();            // load file ltyp.rc int GR_AttTab

  GL_InitPtAtt (-3);           // create point-attributes

  GL_InitSymb ();              // die Symbole generieren > DL

  GL_InitGFont ();             // den Graf-Char-Font initialisieren

  GR_InitGFPar (AP_txdimsiz);  // Graf.FontParameter setzen

  GL_Init_View ();

  GL_Reshape (width, height);


  // UI_GR_DrawExit ();


  // GL_GetActPlane ();  // die aktive Plane errechnen

  // Load DL usw
  //GL_Redraw0 ();


}


//=====================================================================
  int GL_alloc__ (long Ind) {
//=====================================================================
/// \code
/// realloc space fuer BasModelnames; wird nicht gesichert !
/// Ind = 0      Reset DL
/// Ind = 1      increase DL (add GR_TAB_INC elements)
/// Ind > 1      set size
/// \endcode

  long   i1, newSiz;


  // printf("GL_alloc__ %ld %ld\n",Ind,GL_TAB_SIZ);


  // Reset mdb_nam
  if((Ind == 0)&&(GR_ObjTab != NULL)) {
    printf("GL_alloc__ reset\n");
    GL_Init1 ();         // GR_TAB_IND = 0;
    return 0;
  }

  newSiz = GL_TAB_SIZ;
  while (Ind >= newSiz) newSiz += GL_TAB_INC;

  printf("::::GL_alloc__ %ld\n",newSiz);

  GL_IndTab = (unsigned int*)realloc ((unsigned int*)GL_IndTab,
                                       newSiz*sizeof(int));

  if(GL_IndTab == NULL) {
    TX_Error ("******** out of memory - GL_alloc__ *********");
    return -1;
  }

  // load tab
  for(i1=GL_TAB_SIZ; i1<newSiz; ++i1) {
    // GL_IndTab[i1] = i1+DL_base_mod+1;
    GL_IndTab[i1] = i1+DL_base_mod;
  }

  GL_TAB_SIZ = newSiz;


  return 0;

}


//================================================================
  int GL_MousePos (Point *pt1) {
//================================================================
/// \code
/// compute new Mousepos and return it in userCoords
/// RetCod -1:  no obj under cursor ..
/// get pos of last selection with sele_get_pos
/// see also sele_get_pos GL_GetActSelPos
/// \endcode

  GLdouble sx, sy;


  sx = GL_mouse_x_act;
  sy = GL_Viewp[3] - GL_mouse_y_act;


  return GL_Mouse2Pos (pt1, sx, sy);

}


//================================================================
  int GL_Mouse2Pos (Point *pt1, GLdouble mx, GLdouble my) {
//================================================================
// see also GL_SelVert__
 
  int      irc, ii, ia[]={1,-1,2,-2,3,-3,4,-4};
  // GLint    sx, sy;
  int      sx, sy, sy0;
  GLfloat  fa[9];
  GLdouble mz;



  sx = mx;
  sy = my;
  sy0 = sy;


  // printf("GL_Mouse2Pos %d %d\n",sx,sy);

  sx -= 3;
  if(sx < 0) sx = 0;
  ii = -1;
  irc = 0;

  // bei mehreren Werten wird NICHT der naehere geliefert !
  // GL_LESS GL_GREATER GL_NOTEQUAL GL_LEQUAL
  glDepthFunc (GL_ALWAYS);  // geht bei mehreren Treffern am besten



  L_noAmoi:
  glReadPixels (sx, sy, 7, 1, GL_DEPTH_COMPONENT, GL_FLOAT, fa);
  // GL_FLOAT ist 32-bit-float. mit GL_DOUBLE gehts ned !
    // printf(" %d %f,%f,%f,%f,%f,%f,%f\n",sy,
           // fa[0],fa[1],fa[2],fa[3],fa[4],fa[5],fa[6]);


  if(fa[3] < 0.999999999999999999) { mz = fa[3]; goto L_go; }
  if(fa[2] < 0.999999999999999999) { mz = fa[2]; goto L_go; }
  if(fa[4] < 0.999999999999999999) { mz = fa[4]; goto L_go; }
  if(fa[1] < 0.999999999999999999) { mz = fa[1]; goto L_go; }
  if(fa[5] < 0.999999999999999999) { mz = fa[5]; goto L_go; }
  if(fa[0] < 0.999999999999999999) { mz = fa[0]; goto L_go; }
  if(fa[6] < 0.999999999999999999) { mz = fa[6]; goto L_go; }


  ++ii;

  if(ii < 7) {
    sy = sy0 + ia[ii];
    if(sy < 0) sy = 0;
    goto L_noAmoi;
  }


  mz = 0.5;
  irc = -1;


  L_go:
  GL_Sk2Uk (&pt1->x, &pt1->y, &pt1->z, mx, my, mz);

  glDepthFunc (GL_LEQUAL);    // reset Default

    // printf("ex GL_Mouse2Pos %d %f %f %f\n",irc, pt1->x, pt1->y, pt1->z);

  L_exit:
  // UI_GR_DrawExit ();
  return irc;

}


/*
//================================================================
  int GL_Mouse1Pos (Point *pt1, GLdouble mx, GLdouble my) {
//================================================================
// get Mousepos in userCoords
// RetCod -1:  no obj under cursor ..

  int      i1, i2, d1, d2;
  GLdouble sx, sy ,sz;
  GLfloat   f1;
  // GLdouble  f1;
  // GLenum   i1;



  // damit werden Hitlite-Flächen auch normal ausgegeben;
  // ohne liest glReadPixels immer 1, wenn hilite-Flächen existieren ...
  // GL_mode_draw_select = GR_MODE_SELECT;
  // GL_Redraw ();
  // GL_mode_draw_select = GR_MODE_DRAW;



  sx = mx;
  sy = my;



  printf("GL_Mouse1Pos %f %f\n",sx,sy);

  i1 = 0;
  d1 = 1;
  d2 = 1;

  // UI_GR_DrawInit ();

  // get Z-Val for screenPos sx,sy.
  // bei select background gibts Z=1
  L_try:
  // bei mehreren Werten wird NICHT der naehere geliefert !
  // GL_LESS GL_GREATER GL_NOTEQUAL GL_LEQUAL
  glDepthFunc (GL_ALWAYS);  // geht bei mehreren Treffern am besten

  glReadPixels ((GLint)sx,(GLint)sy, 1,1, GL_DEPTH_COMPONENT, GL_FLOAT, &f1);
  // GL_FLOAT ist 32-bit-float.
  // glReadPixels ((GLint)sx,(GLint)sy, 1,1, GL_DEPTH_COMPONENT, GL_DOUBLE, &f1);

  // glReadPixels(sx, sy, 1, 1, GL_DEPTH_COMPONENT, GL_DOUBLE, &sz);
  // mit GL_DOUBLE gehts ned !

    printf(" sx sy f1 %f %f %f\n",sx,sy,f1);


  i2 = glGetError();
  if(i2) {
      printf(" glErr %d |%s| %d\n",i2,gluErrorString(i2));
    // DL_Redraw ();
    // goto L_try;
  }


  // modify pixelPosition until we get a value ..
  // if(f1 == 1.) {
  if((f1 >= 1.)||(f1 <= 0.00000000001)) {
    ++i1;
    sx += d1;
    sy += d2;
    if(i1 == 10) {
      sx = mx;
      sy = my;
      d1 = -1;
      d2 = -1;
    }
    if(i1 == 20) {
      sx = mx;
      sy = my;
      d1 = 1;
      d2 = -1;
    }
    if(i1 == 30) {
      sx = mx;
      sy = my;
      d1 = -1;
      d2 = 1;
    }
    // if(i1 > 80) return -1;
    if(i1 > 40) {
      // ++i2;
      // i1 = 0;
      // DL_Redraw();
      sx=mx; sy=my;
      // if(i2 < 20) goto L_try;
      i2 = -1; sz = 0.5; goto L_go;
    }
    goto L_try;
  }

  sz = f1;

    // printf(" sx/y/z= %f %f %f\n",sx,sy,sz);

  // i1 = glGetError();
    // printf(" glErr %d |%s| %d\n",i1,gluErrorString(i1));
  i2 = 0;


  L_go:
  GL_Sk2Uk (&pt1->x, &pt1->y, &pt1->z, sx, sy, sz);
    printf("ex GL_Mouse1Pos %f %f %f\n",pt1->x, pt1->y, pt1->z);

  L_exit:
  // UI_GR_DrawExit ();
  return i2;

}
*/
 
//================================================================
  int GL_SelVert__ (Point *pts) {
//================================================================
// give point nearest to cursor in usercoords ..

#define DR_BUF_SIZ   1000000


  int      irc=0, fsiz;
  // long     msiz;
  float    *feedBuffer;


  printf("GL_SelVert__\n");


  
  // GL_mode_feed = 1; // ReScale
  if(GL_FeedGet(&fsiz, &feedBuffer, GL_3D) < 0) return -1;
  if(fsiz < 6) {TX_Print("- nothing selected"); return -1;}
  // GL_mode_feed = 0;


/*
  // Memory holen ..
  msiz = DR_BUF_SIZ;
  feedBuffer = malloc (msiz);
  fsiz = msiz / sizeof(float);


  // get Feedback from OpenGL
  L_start:
  GL_Feedback (&fsiz, feedBuffer);

  if(fsiz < 7) {

    msiz += DR_BUF_SIZ;
    printf("GL_SelVert__ REALLOC %d\n",msiz);

    feedBuffer = realloc (feedBuffer, msiz);
    fsiz = msiz / sizeof(float);

    if(feedBuffer == NULL) {
      TX_Error ("GL_SelVert__: OUT OF MEMORY");
      irc = -1;
      goto L_fertig;
    }
    goto L_start;
  }
*/

  // init buffer
  GL_SelVert_aux (1, feedBuffer);

  // scan - analyze Feedbackbuffer
  GL_SelVert_scan (fsiz, feedBuffer);

  // get found vertex ..
  GL_SelVert_aux (3, feedBuffer);

  pts->x = feedBuffer[0];  // float -> double !
  pts->y = feedBuffer[1];
  pts->z = feedBuffer[2];


  //===========================================
  L_fertig:

  // give Memory back ..
  free(feedBuffer);

  UT3D_stru_dump(Typ_PT, pts, "ex GL_SelVert__ %d:",irc);

  return irc;

}


//=========================================================
  int GL_SelVert_scan (int size, float *buffer) {
//=========================================================
// In:  Feedbackbuffer;
// Out: Objekte ->  Hilfsdatei tmp/print.tmp
// Normaler Record (Point, Line, Polygon) sieht so aus:
// X-Coord Y-Coord Z-Coord R G B A



  int       i1, count, token, nvertices;

  // printf("GL_SelVert_scan size=%d\n",size);


  count = 0;



  NextRec:
  token = buffer[count];
  // printf("........... next: %d [%d]\n",token,count),
  ++count;

      //===================================================================
      if (token == GL_PASS_THROUGH_TOKEN) {
         // printf ("%d GL_PASS_THROUGH_TOKEN %f\n",count,buffer[count]);
         ++count;


      //===================================================================
      } else if (token == GL_POINT_TOKEN) {

         // printf ("%d GL_POINT_TOKEN\n",count);
         // fprintf(fp1, "PT %f %f %f\n",
                 // buffer[count],buffer[count+1],
                 // (0.5 -  buffer[count+2]) * GL_Z_FAKT);

         GL_SelVert_aux (2, &buffer[count]);

         count += 3;


      //===================================================================
      } else if (token == GL_LINE_TOKEN) {
         // printf ("%d GL_LINE_TOKEN\n",count);
         // AP_print_vertex (fpo, &count, &buffer[count]);
         // fprintf(fp1, "LN %f %f %f %f %f %f\n",
                 // buffer[count],buffer[count+1],
                 // (0.5 -  buffer[count+2]) * GL_Z_FAKT,
                 // buffer[count+3],buffer[count+4],
                 // (0.5 -  buffer[count+5]) * GL_Z_FAKT);

         GL_SelVert_aux (2, &buffer[count]);
         GL_SelVert_aux (2, &buffer[count+3]);
         count += 6;


      //===================================================================
      } else if (token == GL_POLYGON_TOKEN) {
         nvertices = buffer[count];
         // printf ("%d GL_POLYGON_TOKEN: (%d)\n",count,nvertices);
         // if(nvertices != 3) {printf("GL_SelVert_scan E003 %d\n",nvertices);}
         ++count;
         // fprintf(fp2, "PO %f %f %f",
                 // buffer[count],buffer[count+1],
                 // (0.5 -  buffer[count+2]) * GL_Z_FAKT);
         // count += 7;
         count += 3;
         for (i1=1; i1<nvertices; ++i1) {
           // AP_print_vertex(fpo, &count, &buffer[count]);
           // fprintf(fp2, " %f %f %f",
                   // buffer[count],buffer[count+1],
                   // (0.5 -  buffer[count+2]) * GL_Z_FAKT);
           GL_SelVert_aux (2, &buffer[count]);
           count += 3;
         }
         // fprintf(fp2, "\n");



      //===================================================================
      } else if (token == GL_BITMAP_TOKEN) {
         // printf ("%d GL_BITMAP_TOKEN\n",count);
         count += 3;


      //===================================================================
      } else if (token == GL_DRAW_PIXEL_TOKEN) {
         // printf ("%d GL_DRAW_PIXEL_TOKEN\n",count);
         count += 3;


      //===================================================================
      } else if (token == GL_COPY_PIXEL_TOKEN) {
         // printf ("%d GL_COPY_PIXEL_TOKEN\n",count);
         count += 3;



      //===================================================================
      } else if (token == GL_LINE_RESET_TOKEN) {
         // printf ("%d GL_LINE_RESET_TOKEN\n",count);
         // fprintf(fp1, "LN %f %f %f %f %f %f\n",
                 // buffer[count],buffer[count+1],
                 // (0.5 -  buffer[count+2]) * GL_Z_FAKT,
                 // buffer[count+3],buffer[count+4],
                 // (0.5 -  buffer[count+5]) * GL_Z_FAKT);
         GL_SelVert_aux (2, &buffer[count]);
         GL_SelVert_aux (2, &buffer[count+3]);
         count += 6;




      //===================================================================
      } else printf("%d **** unknown GL-TOKEN %d ****\n",size-count,token);


  if(count < size) goto NextRec;

  return 0;

}


//================================================================
  int GL_SelVert_aux (int mode, float *ptf) {
//================================================================
// (mode == 1) Init (minDist)
// (mode == 2) den jeweils naechsten Punkt merken
// (mode == 3) convert & return the found (nearest) point
//               muss noch um Scale korrigiert werden !

static double sx,sy,sz, dist;
static float  xpos, ypos;

  int       i1, i2;
  float     dx, dy;
  double    d1, ux,uy,uz;



  //================================================================
  // Init
  if(mode == 1) {

    dist = 99999999.;
    // die aktuelle Curpos in screenkoords holen
    // links unten ist 0,0
    GL_GetCurPosSc (&i1, &i2);
    xpos = i1;
    ypos = i2;
    // printf("GL_SelVert_aux init %f %f\n",xpos,ypos);

      // NUR TEST:
      // GL_Sk2Uk (&ux,&uy,&uz,  xpos,ypos,0);
      // printf(" - _SelVert_aux %f %f %f\n",ux,uy,uz);

    return 0;
  }



  //================================================================
  // test ob Punkt (in Screenkoords) naeher ist als der vorherige
  if(mode == 2) {

    // printf("GL_SelVert_aux test %f %f %f\n",ptf[0],ptf[1],ptf[2]);

    dx = ptf[0] - xpos; //if(dx < 0.) dx = -dx;
    dy = ptf[1] - ypos; //if(dy < 0.) dy = -dy;


    d1 = dx*dx + dy*dy;
    if(d1 > dist) return 0;
    dist = d1;

    // printf("GL_SelVert_aux select..... %f\n",dist);

    sx = ptf[0];
    sy = ptf[1];
    sz = ptf[2];


      // NUR TEST:
      // GL_Sk2Uk (&ux,&uy,&uz,  sx,sy,sz);
      // printf(" - _SelVert_aux %f %f %f\n",ux,uy,uz);

    return 0;
  }



  //================================================================
  // (mode == 3)  work


    printf("  GL_SelVert_aux-3 %f %f %f\n",sx,sy,sz);
  GL_Sk2Uk (&ux,&uy,&uz,  sx,sy,sz);
  // printf("ex GL_SelVert_aux %f %f %f\n",ux,uy,uz);


  // als floats nach draussen kopieren ..
  ptf[0] = ux;
  ptf[1] = uy;
  ptf[2] = uz;

  return 0;

}


//=====================================================================
  int GL_GetColorBits () {
//=====================================================================
/// GL_GetColorBits       returns Nr of ColorBits (usually 24)

  int  ii;
  glGetIntegerv (GL_DEPTH_BITS, &ii);
  return ii;

}


//=====================================================================
  double GL_query_ViewZ () {
//=====================================================================

  // return GL_cen_Z;
  return GL_cen.z;

}


//=====================================================================
  double GL_get_Scale () {
//=====================================================================
// ACHTUNG: GL_Scale ist GL_ModSiz / Usiz * 1.25 !
// use AP_Get_scale


  //TX_Print("GL_get_Scale %f",GL_Scale);

  return GL_Scale;
}


//================================================================
  int GL_GetScrSiz (int *ix, int *iy) {
//================================================================
/// GL_GetScrSiz          get size of graficWindow in screenCoords

    // printf(" siz %f %f\n",GL_Scr_Siz_X,GL_Scr_Siz_Y);

    // WAS IST IN GL_Siz_X ?
    // printf(" siz %f %f\n",GL_Siz_X,GL_Siz_Y);

  *ix = GL_Scr_Siz_X;
  *iy = GL_Scr_Siz_Y;


  return 0;

}


//================================================================
  int GL_Tex_Del (int ii) {
//================================================================
/// GL_Tex_Del       delete OpenGL-texture

  int     i1;
  GLuint  itx;

  itx = ii + 1;
  glDeleteTextures (1, &itx);

  return 0;

}


//================================================================
  int GL_test_error () {
//================================================================

  GLenum   i1;

  i1 = glGetError ();

  if(i1) {
    printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX \n");
    printf(" glErr %d |%s|\n",i1,gluErrorString(i1));
    printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX \n");
  }

  return i1;

}


//================================================================
  int GL_config_test () {
//================================================================
/// see also GL_GetColorBits
/// Retcod: 0=no, hardware does not accept textures; 1=Yes, Ok.

  int  i1, i2, i3, i4;


  glGetIntegerv (GL_DEPTH_BITS, &i1);
  printf(" GL_DEPTH_BITS = %d\n",i1);


  glGetIntegerv (GL_STENCIL_BITS, &i1);
  printf(" GL_STENCIL_BITS = %d\n",i1);


  glGetIntegerv (GL_ACCUM_RED_BITS,   &i1);
  glGetIntegerv (GL_ACCUM_GREEN_BITS, &i2);
  glGetIntegerv (GL_ACCUM_BLUE_BITS,  &i3);
  glGetIntegerv (GL_ACCUM_ALPHA_BITS, &i4);
  printf(" GL_ACCUM_RED/GREEN/BLUE/ALPHA_BITS = %d %d %d %d\n",i1,i2,i3,i4);

  glGetIntegerv (GL_RED_BITS,   &i1);
  glGetIntegerv (GL_GREEN_BITS, &i2);
  glGetIntegerv (GL_BLUE_BITS,  &i3);
  glGetIntegerv (GL_ALPHA_BITS, &i4);
  printf(" GL_RED/GREEN/BLUE/ALPHA_BITS = %d %d %d %d\n",i1,i2,i3,i4);

  // i1=RED_BITS, i2=GREEN_BITS, i3=BLUE_BITS
  // nvidia GT230: 5 6 5 0 !
  if((i1 > 3)&&(i2 > 3)&&(i3 > 3))
    return 1;
  else
    return 0;

}


//================================================================
  int GL_grid__ (Plane *gPln, Mat_4x3 gMat, Mat_4x3 gImat) {
//================================================================
// dx, dy und iNr von scale abhängig ..

  static GLfloat GL_col_grid[4] = { 0.7,  0.7,  0.7, 0.1 };

  int     i1, iNr, irc;
  double  d1, dx, dy, scl;
  Point   p1 = {0., 0., 0.};
  Point   p2 = {0., 0., 0.};
  Point   ptc, pto, p3, p4;
  Vector  vc1, vc2; 
  Mat_4x3 m31;
  Mat_4x4 m41;


  AP_Get_scale (&scl);

  // printf("GL_grid__ scl=%f WC_sur_ind=%d\n",scl,WC_sur_ind);


  iNr = 12;                // nr of lines
  dy = 0.5 * scl;          // distance between lines
  
  d1 = (dy / 2.)  * iNr;   // half width
    // printf(" dy=%f d1=%f\n",dy,d1);



  // get origin of active constrPlane as pt1
  // UT3D_m3_get (&pto, 3, WC_sur_mat);
    // UT3D_stru_dump (Typ_M4x3, WC_sur_mat, "WC_sur_mat:");


  // project GL_cen auf die constPlane
  UT3D_pt_projptpl (&ptc, gPln, &GL_cen);
  // irc = UT3D_pt_intptvcpl_ (&pto, &GL_constr_pln, &ptc, &GL_eyeX);
    // UT3D_stru_dump (Typ_PT, &ptc, "ptc:");
    // GL_Disp_symB (SYM_STAR_S, &ptc);


  // get screencenter on constrPlane
  // ptc = GL_GetConstrPos (&GL_cen);
    // UT3D_stru_dump (Typ_PT, &GL_cen, " GL_cen:");
  // geht ned, da sofort viel zu weit draussen ..


  // get screencenter on refsys constrPlane (inv tr)
  UT3D_pt_traptm3 (&pto, gImat, &ptc);
    // UT3D_stru_dump (Typ_PT, &pto, "pto:");


  // vector origin -> centerpoint
  // UT3D_vc_2pt (&vc1, &pto, &ptc);

  // UT3D_pt_traptvc (&pto, &pto, &vc1);


  // UT3D_m3_loadpl (m31, &GL_constr_pln);
  // UT3D_m3_copy (m31, WC_sur_mat);
  // UT3D_m3_load_o (m31, &pto);
    // UT3D_stru_dump (Typ_M4x3, m31, "WC_sur_mat:");

  // UT3D_m4_loadpl (m41, &GL_constr_pln);
  // UT3D_m4_load_o (m41, &UT3D_PT_NUL);
  // UT3D_m4_load_o (m1, &ptc);
    // UT3D_stru_dump (Typ_M4x4, m41, "bas m1:");

  // UT3D_vc_multvc (&vc2, (Vector*)&ptc, 1. / GL_Scale);
    // UT3D_stru_dump (Typ_VC, &vc2, "vc2:");

    // UT3D_pt_traptm3 (&p4, WC_sur_mat, &pto);
    // GL_Disp_symB (SYM_STAR_S, &p4);


  // hor lines
  p1 = pto;
  p2 = p1;
  p1.y += d1;
  p2.y = p1.y;
  p1.x -= d1;
  p2.x += d1;

    // UT3D_stru_dump (Typ_PT, &p1, "new p1:");
    // UT3D_stru_dump (Typ_PT, &p2, "new p2:");

  UT3D_pt_traptm3 (&p1, gMat, &p1);
  UT3D_pt_traptm3 (&p2, gMat, &p2);

  vc1.dx = 0.;
  vc1.dy = -dy;
  vc1.dz = 0.;
  UT3D_vc_travcm3 (&vc1, WC_sur_mat, &vc1);


  // vert lines
  p3 = pto;
  p4 = p3;
  p3.x += d1;
  p4.x = p3.x;
  p3.y -= d1;
  p4.y += d1;

  UT3D_pt_traptm3 (&p3, WC_sur_mat, &p3);
  UT3D_pt_traptm3 (&p4, WC_sur_mat, &p4);

  vc2.dx = -dy;
  vc2.dy = 0.;
  vc2.dz = 0.;
  UT3D_vc_travcm3 (&vc2, WC_sur_mat, &vc2);

 
  //----------------------------------------------------------------
  glPushMatrix ();
  glDisable (GL_LIGHTING);

  glColor3fv   (GL_col_grid);

  // glLoadIdentity ();  // kills the gluLookAt (zoom/pan) - settings
  // glMultMatrixd ((double*)m41);


  glBegin (GL_LINES);

    for(i1=0; i1<iNr; ++i1) {
      // transform point
      // UT3D_pt_traptm3 (&p3, m31, &p1);
      // UT3D_pt_traptm3 (&p4, m31, &p2);

      glVertex3dv ((double*)&p1);
      glVertex3dv ((double*)&p2);

      glVertex3dv ((double*)&p3);
      glVertex3dv ((double*)&p4);

      UT3D_pt_traptvc (&p1, &p1, &vc1);
      UT3D_pt_traptvc (&p2, &p2, &vc1);

      UT3D_pt_traptvc (&p3, &p3, &vc2);
      UT3D_pt_traptvc (&p4, &p4, &vc2);
    }

  glEnd ();

  glEnable (GL_LIGHTING);
  glPopMatrix ();


  return 0;

}


//================================================================
  int GL_plnSel_init (int ii, int mode) {
//================================================================
// Input:
//   ii   0=planeSelect not active; else=yes.
//   mode 0=do not redraw;
//        1=DL_Redraw;
// RetCod:
//   GL_plnSelStat before modification
    
  int    iDo, oldStat;
    

  // printf("GL_plnSel_init %d %d\n",ii,GL_vcSelStat);
    

  oldStat = GL_plnSelStat;
    
  // if(mode == 2) goto L_exit;
    
  // if(GL_plnSelStat != ii) iDo = 1;
  // else                    iDo = 0;


  GL_plnSelStat = ii;

  // if(iDo) {
    // if(mode) DL_Redraw ();
  // }

  L_exit:
  return oldStat;

}


//================================================================
  int GL_vcSel_init (int ii, int mode) {
//================================================================
// Input:
//   ii   0=vectorSelect not active; else=yes (index of vec to display; -1=none)
//        -1 = display defaultVectors, but no DB-vector.
//   mode 0=do not redraw;
//        1=DL_Redraw;
//        2=query only (RetCod = GL_vcSelStat)
// RetCod:
//   GL_vcSelStat before modification
 
  int    iDo, oldStat;


  // printf("GL_vcSel_init %d %d mode=%d\n",ii,GL_vcSelStat,mode);


  oldStat = GL_vcSelStat;

  if(mode == 2) goto L_exit;

  // if(GL_vcSelStat != ii) iDo = 1;
  // else                   iDo = 0;

 
  GL_vcSelStat = ii;

  // if(iDo) {
    // if(mode) DL_Redraw ();
  // }

  L_exit:
  return oldStat;

}


//================================================================
  int GL_icons_dispTags (int iNr, char *txt, char stat) {
//================================================================
// display tag # iNr with tagText txt
// Input:
//   stat    0=normal, 1=dimmed

  static int ixPos;

  int      iLen, ii, ix, i1, i2, iColTag;
  Point    pt1;
  GLuint   gli;
  // char     s1[32];
  float    fx, fy;


  // printf("GL_icons_dispTags iNr=%d |%s|%c|\n",iNr,txt,stat);

  if(iNr < 1) ixPos = 0;

  GL_icons_Pos ();   // get GL_icons_Ori

  iLen = strlen(txt);
  ixPos += iLen + 2;
    // printf(" ixPos=%d\n",ixPos);


  glDisable (GL_LIGHTING);
  glDepthFunc (GL_ALWAYS);

  // glPushMatrix ();


  gli = GR_TAB_IND;


    if(GL_mode_draw_select == GR_MODE_SELECT)
      glLoadName (gli + 10 + iNr);


    // move from GL_icons_Ori to lower left tag-pos
    // posX
    // i1 = -80 + (-160 * (iNr + 1));  // 200 132
    i1 = -80 + (-30 * ixPos);  // 200 132
    // posY
    i2 = -82;
      // printf(" 2D-icons move i1=%d i2=%d\n",i1,i2);
    GL_pos_move_2D (&pt1, &GL_icons_Ori, i1, i2);

    // adapt size to textlength
    if(stat == '0') iColTag = 5;    // 5=Gelb, COL_GELB
    else            iColTag = 10;   // 10=COL_DIMMED
    // sizX
    ix = ((iLen + 1) * 30) + 15;  // 22
    GL_DispTag2 (&pt1, ix, 52, iColTag);


    glColor3fv   (GL_col_tab[0]);
    // glRasterPos3dv ((double*)&GL_icons_Ori);
    glRasterPos3dv ((double*)&pt1);

    // 2D-move of position  (active positon is startpoint of 1. 
    fx = 8;
    fy = 4;
    GL2D_pos_move (fx, fy);


    // glRasterPos3d (ptt0.x, ptt0.y, ptt0.z);
    // strcpy(s1, "REV");
    glCallLists (iLen, GL_UNSIGNED_BYTE, (GLubyte*)txt);


  //----------------------------------------------------------------
  // done -
  // glPopMatrix ();

  glEnable (GL_LIGHTING);

 
  return 0;

}


//================================================================
  int GL_icons_Pos () {
//================================================================
// used by GL_icons_dispVcPln GL_icons_dispTags
// returns GL_icons_Ori

  int          i1, i2;
  double       d1, d2, d3;
  Point        pt1;
  // Vector       vc1;
  // Plane        pln1;

static int     vcSelTimStamp = -1;

  if(vcSelTimStamp == GL_ViewModTimStamp) return 0;

  vcSelTimStamp = GL_ViewModTimStamp;


  // compute position pt0 


  // get d1,d2,d3 = point - lower right position in screencoords
  GL_GetScrSiz (&i1, &i2);
    // printf(" GL_Scr_Siz_X=%d GL_Scr_Siz_Y=%d\n",i1,i2);
  d1 = (double)i1 * 0.9;
  d2 = (double)i2 * 0.1;
  d3 = 0.;
    // printf(" posS = %lf %lf %lf\n",d1, d2, d3);


  // pt1 -> UserCoords
  GL_Sk2Uk (&pt1.x, &pt1.y, &pt1.z, d1, d2, d3);
    // UT3D_stru_dump (Typ_PT, &pt1, "posU1: ");


  // mit der Userebene schneiden
  // vc1 = GL_GetEyeX ();
  // GL_GetViewPln (&pln1);
  UT3D_pt_intptvcpl_ (&GL_icons_Ori, &GL_view_pln, &pt1, &GL_eyeX);
    // UT3D_stru_dump (Typ_PT, &GL_icons_Ori, "posU2: ");

    // test position
    // { long dl1 = -2L;
      // GL_DrawSymB (&dl1, 2, SYM_CIR_S, &GL_icons_Ori); // red circ
    // }


  return 0;

}


//================================================================
  int GL_icons_dispVcPln () {
//================================================================
// draw VectorSelector and StdPlaneselector; at end of gl-list.

#define SIZ_vcSel    120      // length of vectors
#define SCL_plnSel   1.5

  int     i1, i2, selId;
  long    dl1, dli;
  GLuint  gli;
  double  lv;
  char    s1[32];
  Point   p1 = {0., 0., 0.}, p2 = {100., 0., 0.};
  Point   pt1, pt2, pt3, pt4, pt5, pt6, pt7;
  Vector  vc1;

static GLfloat glColx[] = {0.0, 0.0, 1.0, 0.0};
static GLfloat glColy[] = {0.0, 1.0, 0.0, 0.0};
static GLfloat glColz[] = {1.0, 1.0, 1.0, 0.0};
static GLfloat glColv[] = {1.0, 0.0, 0.0, 0.0};



  // printf("GL_icons_dispVcPln\n");
  // printf("GL_icons_dispVcPln %ld %ld\n",vcSelTimStamp,GL_ViewModTimStamp);
  // printf("  GR_TAB_IND=%ld\n",GR_TAB_IND);


  if(GL_mode_draw_select == GR_MODE_SELECT) selId = 1;
  else                                      selId = 0;

  GL_icons_Pos ();   // get GL_icons_Ori


  // fit size
  lv = SIZ_vcSel * GL2D_Scale;    // 2011-10-23
    // printf(" sc=%lf lv=%lf\n",sc,lv);

 

  //================================================================
  // compute points, then draw
  L_draw:
  //
  // X = 1,2
  pt1 = GL_icons_Ori;
  pt1.x += lv;
  pt2 = GL_icons_Ori;
  pt2.x -= lv;

  // Y = 3,4
  pt3 = GL_icons_Ori;
  pt3.y += lv;
  pt4 = GL_icons_Ori;
  pt4.y -= lv;

  // Z = 5,6
  pt5 = GL_icons_Ori;
  pt5.z += lv;
  pt6 = GL_icons_Ori;
  pt6.z -= lv;


  //----------------------------------------------------------------
  // pt7 = active DB-vector
  if(GL_vcSelStat > 0) {
    vc1 = DB_GetVector ((long)GL_vcSelStat);
    UT3D_vc_setLength (&vc1, &vc1, lv);
    UT3D_pt_traptvc (&pt7, &GL_icons_Ori, &vc1);
  }



  //----------------------------------------------------------------
  // UI_GR_DrawInit ();

  // dl1 = -2L;
  // gli = GL_fix_DL_ind (&dl1);
  gli = GR_TAB_IND;
    // printf(" vcSel gli=%d\n",gli);

  // gli = 1;
  // glNewList (gli, GL_COMPILE);
  // if(GL_mode_draw_select == GR_MODE_SELECT)
  // glPushMatrix ();

  // glCallList (DL_Ind_ScBack);           // ScaleBack

  glDisable (GL_LIGHTING);
  glDepthFunc (GL_ALWAYS);


  //----------------------------------------------------------------
  glCallList (DL_base_LnAtt + 0);     // reset last hili !


  //----- display StandardVectors ----------------------------------
  if(GL_vcSelStat) {

    // X
    glColor3fv   (glColx);
    if(selId) glLoadName(gli);
    glBegin (GL_LINES);
      glVertex3dv ((double*)&GL_icons_Ori);
      glVertex3dv ((double*)&pt1);
    glEnd ();

    if(selId) glLoadName(gli + 3);
    glBegin (GL_LINES);
      glVertex3dv ((double*)&GL_icons_Ori);
      glVertex3dv ((double*)&pt2);
    glEnd ();


    // Y
    glColor3fv   (glColy);
    if(selId) glLoadName(gli + 1);
    glBegin (GL_LINES);
      glVertex3dv ((double*)&GL_icons_Ori);
      glVertex3dv ((double*)&pt3);
    glEnd ();

    if(selId) glLoadName(gli + 4);
    glBegin (GL_LINES);
      glVertex3dv ((double*)&GL_icons_Ori);
      glVertex3dv ((double*)&pt4);
    glEnd ();


    // Z
    glColor3fv   (glColz);
    if(selId) glLoadName(gli + 2);
    glBegin (GL_LINES);
      glVertex3dv ((double*)&GL_icons_Ori);
      glVertex3dv ((double*)&pt5);
    glEnd ();

    if(selId) glLoadName(gli + 5);
    glBegin (GL_LINES);
      glVertex3dv ((double*)&GL_icons_Ori);
      glVertex3dv ((double*)&pt6);
    glEnd ();



    //----- display defined DB-Vector dbi=GL_vcSelStat ----------
    if(GL_vcSelStat > 0) {
      glColor3fv   (glColv);
      if(selId) glLoadName(gli + 6);
      glBegin (GL_LINES);
        glVertex3dv ((double*)&GL_icons_Ori);
        glVertex3dv ((double*)&pt7);
      glEnd ();
    }



    //----- display characters x y for StandardVectors ----------
    // display x y   see APT_disp_TxtA
    glColor3fv   (glColx);
    glRasterPos3dv ((double*)&pt1);
    glCallList ((GLuint)120);           // "x"
  
    glColor3fv   (glColy);
    glRasterPos3dv ((double*)&pt3);
    glCallList ((GLuint)121);           // "y"



    //----- display ID of defined DB-Vector dbi=GL_vcSelStat ----------
    if(GL_vcSelStat > 0) {
      glColor3fv   (glColv);
      glRasterPos3dv ((double*)&pt7);
      // strcpy(s1, "D21");
      sprintf(s1, "D%d",GL_vcSelStat);
      i1 = strlen(s1);
      glCallLists (i1, GL_UNSIGNED_BYTE, (GLubyte*)s1);
    }
  }


  //----------------------------------------------------------------
  //----- display StandardPlanes -----------------------------------
  if(GL_plnSelStat) {
    glPushMatrix ();

      glColor3fv   (glColz);
      if(selId) glLoadName(gli + 9);
      // glRasterPos3d (GL_icons_Ori.x, GL_icons_Ori.y, GL_icons_Ori.z);
      glTranslated (GL_icons_Ori.x, GL_icons_Ori.y, GL_icons_Ori.z);
      glScaled (SCL_plnSel, SCL_plnSel, SCL_plnSel);
      glCallList ((GLuint)SYM_SQUARE);

      glColor3fv   (glColy);
      if(selId) glLoadName(gli + 8);
      glRotated (90., 1., 0., 0.);
      glCallList ((GLuint)SYM_SQUARE);

      glColor3fv   (glColx);
      if(selId) glLoadName(gli + 7);
      glRotated (90., 0., 1., 0.);
      glCallList ((GLuint)SYM_SQUARE);

    glPopMatrix ();
  }



  //----------------------------------------------------------------
  // done -
  glEnable (GL_LIGHTING);
  // glPopMatrix ();
  // glEndList ();

  // glCallList ((GLuint)gli);
  // UI_GR_DrawExit ();


  return 0;

}


//================================================================
  int GL_DispTag2 (Point *oPos, int iSizX, int iSizY, int iCol) {
//================================================================
// display coloured rectangle
// using GL_QUADS (selectable !)

  Point    p1, p2, p3;
  Vector   vcy, vcz, vch, vcv;


  // printf("GL_DispTag2 %f %f sx=%d sy=%d col=%d\n",oPos->x,oPos->y,
          // iSizX,iSizY,iCol);


  UT3D_vc_multvc (&vcy, &GL_eyeY, GL2D_Scale);       // scale Y-vec
  UT3D_vc_multvc (&vcz, &GL_eyeZ, GL2D_Scale);       // scale Z-vec


  UT3D_vc_multvc (&vch, &vcy, (double)iSizX); 
  UT3D_vc_multvc (&vcv, &vcz, (double)iSizY); 


  UT3D_pt_traptvc (&p1, oPos, &vch);
  UT3D_pt_traptvc (&p2, &p1, &vcv);
  UT3D_pt_traptvc (&p3, oPos, &vcv);

  glColor3fv    (GL_col_tab[iCol]);

  glBegin (GL_QUADS);

    // glNormal3dv ((double*)&GL_GetEyeX);    // normalVec

    glVertex3dv ((double*)oPos);
    glVertex3dv ((double*)&p1);
    glVertex3dv ((double*)&p2);
    glVertex3dv ((double*)&p3);

  glEnd ();

  return 0;

}


//================================================================
  int GL_DispTag1 (int iSizX, int iSizY, int iCol) {
//================================================================
// display coloured rectangle
// using glBitmap, not selctable !
// position of lower left corner must have been set.

  int            i1, mSiz;
  unsigned char  *cBuf;


  GL_Test ();


  // compute memSiz
  mSiz = (iSizX / 8) + 1;        // jedes Byte hat nur 8 Bit.
  mSiz *= iSizY;


  // get mem for BitBuffer
  cBuf = MEM_alloc_tmp (mSiz);
  if(!cBuf) {
    TX_Error("GL_DispTag1 E001 %d",mSiz);
    return -1;
  }


  // preset BitBuffer (all planes)
  // for(i1=0; i1<mSiz; ++i1) cBuf[i1] = 255;
  memset ((void*)cBuf, 255, mSiz);


  glColor3fv (GL_col_tab[iCol]); // colour of tag-surface

  glBitmap (iSizX, iSizY,        // Breite, Hoehe
            0.f, 0.f,            // origin innerhalb map;
            0.f, 0.f,            // add to posi after drawing
            cBuf);

  return 0;

}


//================================================================
  int GL_pos_move_2D (Point *po, Point *pi, int idx, int idy) {
//================================================================
// move 3D-point with 2D-offset
// Input:
//   pi        point to move in userCoords
//   idx,idy   offset in screenCoords
// Output:
//   po        point in userCoords

// see also DL_Set_Cen2D

  Vector     vcz, vcy, vch, vcv;


  // printf("GL_pos_move_2D %d %d\n",idx,idy);
  // printf(" GL_ModScale=%lf\n",GL_ModScale);


  UT3D_vc_multvc (&vcy, &GL_eyeY, GL2D_Scale);       // scale Y-vec
  UT3D_vc_multvc (&vcz, &GL_eyeZ, GL2D_Scale);       // scale Z-vec

  UT3D_vc_multvc (&vch, &vcy, (double)idx);
  UT3D_vc_multvc (&vcv, &vcz, (double)idy);


  UT3D_pt_trapt2vc (po, pi, &vch, &vcv);

  return 0;

}


//================================================================
//================================================================
//    TESTFUNKTIONEN:
//================================================================
//================================================================

//================================================================
  int GL_tst1 () {
//================================================================
// get current rasterposition .. ??

  GLfloat       fa1[4];
  GLboolean     b1;

  printf("GL_tst1\n");

  glGetFloatv (GL_CURRENT_RASTER_POSITION, fa1);
    printf(" rasterPos %f %f %f %f\n",fa1[0],fa1[1],fa1[2],fa1[3]);

  glGetBooleanv(GL_CURRENT_RASTER_POSITION_VALID, &b1);
    printf(" valid %d\n",b1);

  return 0;

}


#ifdef DEB

/*
//================================================================
  int GL_tst_pta () {
//================================================================

  long   dli;
  ObjGX  o1, cvTab[4];
  Point  pta[]={{0,0,0},{10,0,0},{15,5,0},{10,10,0},{0,10,0},{0,0,0}},
         pa2[]={{0,0,0},{10,0,0},{10,0,10},{0,0,10},{0,0,0}};


  cvTab[0].typ  = Typ_PT;
  cvTab[0].form = Typ_PT;
  cvTab[0].siz  = 6;
  cvTab[0].data = pta;


  cvTab[1].typ  = Typ_PT;
  cvTab[1].form = Typ_PT;
  cvTab[1].siz  = 5;
  cvTab[1].data = pa2;


  o1.typ  = Typ_GL_CV;
  o1.form = Typ_ObjGX;
  o1.siz  = 2;
  o1.data = cvTab;


  dli = DL_StoreObj (Typ_CV, 0L, Typ_Att_Fac1);
     // Typ_SUR muss vsym=1 haben (Hili!)

  GL_Draw_obj (&dli, Typ_Att_Fac1, &o1);

  return 0;

}


*/
#endif



//=============================== EOF ==============================
