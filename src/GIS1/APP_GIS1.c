/***************************************************************************
 *   Copyright (C) 2008 by Franz Reiter   *
 *   franz.reiter@cadcam.co.at   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
/*

APP_GIS1.c                                2008-01-24    Franz Reiter.

-----------------------------------------------------
Modifications:
2012-08-20 V3.10 Port -> C-KIT-GUI. RF.
2011-01-03 V2.00 New: Breaklines, InnerBoundaries. RF.
2008-01-24 V1.00 Projektstart. RF


Linkjob: ../xa/APP_GIS1.mak
make -f APP_GIS1.mak "OS=Linux32"
or dynamic (Options/compile-ON)


=====================================================
Liste_Funktionen_Beginn:

gCad_main            CAD-Core starts application here
gCad_fini            close application
gis_init_pl          get surf-pTab-List from DB
gis_init_UI          Create UI-panel (Toolbar) Main-Window
gis_msg__
gis_changed_Func
gis_changed_visi     lock prjMsh if no mesh exists
gis_changed_mshMod   ActFtabModFlag changes  (mesh needs update)
gis_changed_actElNr  actEl.rNr changes
gis_ui_ButPrj        block/unblock PrjMsh-Function
gis_block            block/unblock

gis_mdl_upd          create update / Modelcodes
gis_mdl_del_msh      delete mesh-codeline in model
gis_mdl_del_pTab     delete pTab from model (del A<surInd> - Line)

gis_lst_sup          create list of sufaces with pTabs; display List.
gis_surTab_add       add new surf
gis_surInd_surNr     get index into surTab from DB-index of pointSurface
gis_sur_on           activate surface surTab[surInd]
gis_sur_off          disactivate surface surTab[ActSurInd]
gis_disp_actObj       display activeSurfNr & activePointNr

gis_CB1              action-dispatcher
gis_del__            delete active gis-obj
gis_del_ptRange_CB   delete gisPoints; single or range

gis_key_CB           keypress-callback
gis_sel_CB           selection-callback CAD-Core reports selection
gis_SurLst_CB        userselection of surfaces-listEntry
gis_EdgTyp_CB        userselection of edge-type
gis_UI_getPt         get coords from entryfields

gis_mesh             create/diplay mesh
gis_msh_disp
gis_msh_del          remove mesh

gis_DrawAllEL
gis_DrawEL           display EdgeLine
gis_actEl_disp
gis_EL_disp          display polygon - us3d for display of Edgline actEl
gis_HideAllEL
gis_HideEL
gis_disp_pTab        update display (boundary) of pTab.
gis_DrawPoints
gis_DrawPoint
gis_HidePoints
gis_disp_coords      display coords of active gis-point

gis_Edges_add        add polygon iTab[iNr] to ActEtab/DatEtab
gis_Edges_upd        init/add/update/delete ActEdges
gisEdg_addPtab       add polygon iTab to active EdgeLine actEl
gisEdg_addGisPt      add index to point ActPtNr to active EdgeLine;
gis_Edges_del        delete EdgeLine elInd
gis_Edges_all_del    delete actEl & alle EdgeLines
gisEdg_ck_pt__       test if gispoint lies on edge ip1-ip2
gis_actEl_del        clear actEl, remove display
gis_EL_cleanI           init stat of all EdgeLines
gis_EL_cleanE           delete edgelines with less than 2 points;

gis_ptab_init        if no surface exists, init one
gis_ptab_load        user has pointfile selected; load it to pTab
gis_ptab_move        move all points
gis_addPtab          create GIS-points
gis_ptab_del         remove all Points (from surface ActSurPtb)
gis_freePos_pTab     get next free Position in pTab
gis_ptab_save        Export points into Ascii-file
gis_PtMod            modify Point Nr ActPtNr from surface ActSurPtb
gis_PtDelAct         delete Point Nr ActPtNr from surface ActSurPtb
gis_PtDel__          delete Point Nr ActPtNr from surface ActSurPtb
gis_PtAdd            add Point to surface ActSurPtb
gisPt_ck_EL__        test if point lies on a edgeline
// gis_obd_cre          Create OuterBoundary from all points

ptab_file_load1      read Punkte aus einer Ascii-Datei nach pTab
ptab_cad_load1
ptab_ptAdd           check if pt1 is already in pTab

gis_dump_surTab
gis_dump_pTab
gis_dump_actEl

Liste_Funktionen_Ende:
=================================================================

GIS-Punkte in der Displaylist: 
  typ=Typ_APPOBJ (20); Ind=pointNr; att=0

GIS-Edgelines in der Displaylist: 
  typ=Typ_APPOBJ (20); Ind=EdgeIndexNr; att=1

GIS-Mesh in der Displaylist:
  typ=Typ_SUR (150); att=1 (symbolic)

GIS-Ptab in der Displaylist (= boundaryCurve):
  typ=Typ_SUR (150); uv=1 (unvisible)



---------------------------------------------------------
Funktionen:
gis_CB1,    (void*)"crMsh"
  gis_mesh ();
    MSH_msh1
      MSH_EL_get_tab
      MSH_ibnd_pt2n    
      gis_Edges_add   
      MSH_EL_get_tab
      MSH_itess_pt2n

GR_DrawSur
  GR_DrawSurPtab
    MSH_ibnd_ptn
  TSU_DrawSurMsh
    MSH_bload_pTab


*/
#ifdef globTag
 void gis(){}
#endif


#include "../GIS1/APP_GIS1.h"

#include <GL/gl.h>
#include <GL/glu.h>


#ifdef _MSC_VER
// die folgenden 2 Funktionen exportieren (werden vom Main gerufen):
__declspec(dllexport) int gCad_main ();
__declspec(dllexport) int gCad_fini ();
// nachfolgende externals werden aus dem Main-Exe imported:
#define extern __declspec(dllimport)
#endif




// #include "../ut/ut_umem.h"             // UME_alloc_tmp
#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_msh.h"              // Fac3 ..
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_tin.h"              // TYP_EDGLN_BL
#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_TX.h"               // TX_Print

#include "../gui/gui__.h"

#include "../gr/ut_UI.h"               // SYM_TRI_S
#include "../gr/ut_GL.h"               // GL_GetViewPos
#include "../gr/ut_DL.h"               // DB_GetDLatt

#include "../db/ut_DB.h"               // DB_GetGTxt

#include "../xa/xa.h"                  // APP_OBJ_NAM WC_modnam
#include "../xa/xa_mem.h"              // memspc51
#include "../xa/xa_sele.h"             // Typ_goGeom


#define   TRUE 1
#define   FALSE 0



//----------------------------------------------------------------
//---------------- external vars: ---------------------

// ex xa_ui.c:
extern MemObj    UIw_Box_TB;    // toolbarBox
extern int       KeyStatCtrl;


// // ex ../ut/ut_txfil.c
// extern char *UTF_FilBuf0;
// extern long UTF_FilBuf0Len;  // die aktuelle Filesize


// aus ../gr/ut_DL.c:
extern DL_Att  *GR_ObjTab;  // das ist die DL




//----------------------------------------------------------------
typedef_MemTab(int);
typedef_MemTab(Point);
typedef_MemTab(Fac3);
typedef_MemTab(EdgeLine);




//---------------- local vars: ---------------------
// a list of active surfaces from PTAB's
static  typ_sur_ptab surTab[MAX_SURPTAB];      // Liste der aktiven pTab-surfs
static  int surTabNr;                          // nr of active pTab-surfs
static  int surIndLast;

static  int       ActSurInd;    // index of active surTab-record
        int       ActSurPtb,     // DB-Index (surTab.indp) of active point-surf.
                  ActSurMsh,    // DB-Index (surTab.indm) of active mesh-surface
                  ActPtNr,
                  ActSelTyp,
                  ActSelInd;
static  long      ActPtDli;     
static  int       gisFunc;


static  MemTab(Point) ActPtab = MemTab_empty;        // table of Point's
static  int       ActPtabModFlag; 
static  int       ActPtabBndModFlag; 

static  MemTab(Fac3) ActFtab = MemTab_empty;         // table of Fac3's
static  int       ActFtabModFlag;              // ActFtabNr
// static  Fac3      *ActFtab = NULL;

// static  Edges     ActEdges;
// static  int       ActEtabNr, ActEtabSiz, ActEtabModFlag;
static  MemTab(EdgeLine) ActEtab = MemTab_empty;    // table of EdgeLine's

static  MemTab(int) DatEtab = MemTab_empty;         // dataSpace for all EdgeLines

// static  EdgeLine  actEl;                      // active EdgeLine
static  MemTab(int) actEl = MemTab_empty;           // dataSpace for active EdgeLine

#define col_actEl 7                  // colour


static  MemObj tbGis1=GUI_OBJ_NEW;    // GIS-Toolbar
static  MemObj wlbas, wlbap;
static  MemObj wenvx, wenvy, wenvz, wBoxEd, wcboz;
static  MemObj wls, wledg;
static  MemObj wButDel, wButMsh, wButEx, wButEdi, wButPrj, wMenBar;

static  GLfloat GL_col_tab [][4] = {
  0.7,  0.1,  0.1, 1.0,     //  0 col. points
  0.1,  0.1,  0.7, 1.0};    //  1 col. info
// red  green blue intens




//----------------------------------------------------------------
//---------------- prototypes: ---------------------
  int gis_init_UI (MemObj *parent);
  int gis_sel_CB (int src, long dl_ind);
  int gis_key_CB (int key);
  int gis_CB1 (MemObj *mo, void **data);
  int gis_SurLst_CB (MemObj *mo, void **data);
  int gis_EdgTyp_CB (MemObj *mo, void **data);
  int gis_del_pTab (MemObj *mo, void **data);
  int gis_ptab_load (char *fnam, char *dirNam);
  int gis_ptab_save (char *fnam, char *dirNam);
  int gis_surInd_surNr (long surNr);
  int gis_DrawEL (EdgeLine *el1, int ind);
  int gis_EL_disp (long dli, int *ib, int iNr, Point *pa, int att, int mode);
  int gis_del_ptRange_CB ();
  int gis_movPts_CB ();








//=========================================================
  int gCad_main () {
//=========================================================
// user has selected this plugin; starting ...


  static APP_OBJ_NAM appObjNamTab[]={
    {"GisPt",  APPTYP_GISPT},   // 0
    {"Edge",   APPTYP_EDGLN},   // 1
    {"",-1}
  };



  int     *iba, iNr;
  // Mesh    ms1;

  // write to Main-Infowindow ..
  TX_Print("%s",VERSION);


  UI_block__ (1, 0, 0);

  // init all structs
  MemTab_ini (&ActPtab, sizeof(Point), Typ_PT, INC_Ptab);
  MemTab_ini (&ActFtab, sizeof(Fac3), Typ_Fac3, INC_Ftab);

  MemTab_ini (&actEl,   sizeof(int), Typ_Int4, INC_actEl);
  MemTab_ini (&ActEtab, sizeof(EdgeLine), Typ_EdgeLine, INC_Etab);
  MemTab_ini (&DatEtab, sizeof(int), Typ_Int4, INC_datEtab);


  // create List of active pTabs
  // get list of surfaces with subTyp PTAB
  gis_init_pl ();   // get pTab-List from DB 


  // get memSpc for Edges ..
  gis_Edges_upd (TYP_FuncInit);


  // surNr = find last used surface-nr (A)
  surIndLast = DB_dbo_get_free (Typ_SUR);
    // printf(" LastUsed A-nr = %d\n",surIndLast);


  // create & activate toolbar
  gis_init_UI (&UIw_Box_TB);

  ActSurInd  = -1;
  ActSurPtb  = -1;
  ActSurMsh = -1;
  // if(surTabNr < 1) goto L_99;


  // if no surface exists, init one
  if(surTabNr < 1) gis_ptab_init ();


  // update surface-list
  gis_lst_sup ();


  // activate last mesh-surf
  gis_sur_on (surTabNr-1);


  //----------------------------------------------------------------
  // attach grafic selections
  L_99:
  AP_UserKeyIn_get (gis_key_CB);
  AP_UserSelection_get (gis_sel_CB);
  AP_UserAppObjNamTab (appObjNamTab);
  sele_set__ (Typ_goGeom);       // enable selection of all types
  sele_setNoConstrPln ();        // enable selection of point on ConstrPln

  ActPtDli = -1;

  // after a new toolbar 2 redraws necessary (both planes ?)
  // GUI_update__ ();
  // UI_GR_Redraw ();
  // GUI_update__ ();

  TX_Print("%d point-surfaces loaded",surTabNr);

  AP_stru_2_txt (NULL, 0, NULL, 0);  // init the objNr's before UTO_sav_ost


  return 0;

}


//================================================================
  int gis_init_UI (MemObj *parent) {
//================================================================

  MemObj box0, box1, box2;
  MemObj mb1, mb1f, mb2s;
  char *optLst[]={"save EdgeLine",
                  "save InnerBoundary",
                  "save OuterBoundary",
                  NULL};


  printf("gis_init_UI \n");


  if(GUI_OBJ_IS_VALID(&tbGis1)) {
    GUI_set_show (&tbGis1, 1);
    return 0;
  }



  // tbGis1 = GUI_toolbar__ (parent);
  tbGis1 = GUI_toolbox__ (parent);        // 2014-11-05

  box0 = GUI_box_v (&tbGis1, "");
  box1 = GUI_box_h (&box0, "");
  box2 = GUI_box_h (&box0, "");

  GUI_label__ (&box1, " GIS1 ", "l");

  wMenBar = GUI_menu__ (&box1, NULL, 0);


  //----------------------------------------------------------------
  mb1f = GUI_menu__ (&wMenBar, " File ", 0);
  GUI_menu_entry (&mb1f, "Import Points from File",
                        gis_CB1, (void*)"opePtab");
  GUI_menu_entry (&mb1f, "Export Points",
                        gis_CB1, (void*)"savPtab");



  //----------------------------------------------------------------
  mb1f = GUI_menu__ (&wMenBar, " Tools ", 0);
  GUI_menu_entry (&mb1f,"create new surface",
                        gis_CB1, (void*)"CrSurf");
  GUI_menu_entry (&mb1f,"delete active mesh and all points",
                        gis_CB1, (void*)"delAll");
  GUI_menu_entry (&mb1f,"delete range of points",
                        gis_CB1, (void*)"delPtR");
  GUI_menu_entry (&mb1f,"move all points",
                        gis_CB1, (void*)"movPts");
  GUI_menu_entry (&mb1f,"dump all edgelines",
                        gis_CB1, (void*)"dumpEL");
  GUI_menu_entry (&mb1f,"dump all points",
                        gis_CB1, (void*)"dumpPt");
  GUI_menu_entry (&mb1f,"print",
                        gis_CB1, (void*)"print");
  GUI_menu_entry (&mb1f, "---",     NULL,       NULL);
  wcboz = GUI_menu_checkbox__ (&mb1f, "remove outer zone", 1, NULL, NULL);
    GUI_Tip  (" remove outer triangles (check sidelenght) ON or OFF"); 



  //----------------------------------------------------------------
  wButDel = GUI_button__ (&box1, " Delete ",      gis_CB1, (void*)"bt_del", "");
  GUI_Tip  (" deletes active surface and ALL its points ");
  wButMsh = GUI_button__ (&box1, " create Mesh ", gis_CB1, (void*)"crMsh",  "");


  wButEdi =
  GUI_radiobutt__ (&box1, "edit  ",        0, gis_CB1, (void*)"edit",   "");
    GUI_Tip  (" create/modify/delete Gis-points, delete Edgelines"); 
  wButPrj =
  GUI_radiobutt__ (&box1, "prj -> Mesh  ", 1, gis_CB1, (void*)"prjMsh", "");
    GUI_Tip  (" project Cad-object onto Mesh, create new Cad-objects"); 
  GUI_radiobutt__ (&box1, "create Edge  ", 1, gis_CB1, (void*)"crEdge", "");
    GUI_Tip  (" create Gis-edges; finish with save Edgeline .."); 
  // GUI_RadButtSet 

  // list for edgeTypes
  wledg = GUI_optmen__ (&box1, optLst[0], optLst, NULL, gis_EdgTyp_CB, "");
  GUI_set_enable (&wledg, 0);  // not active
  // GUI_OptMenSet (&wledg, 3, NULL);
    printf(" ***** disactivate optMen ...\n");

  gisFunc = GISFUNC_Edit;



  //----------------------------------------------------------------
  wBoxEd = GUI_box_h (&box2, "");

  // create (empty) list for the mesh-surfaces
  wls = GUI_optmen__ (&wBoxEd, "?", NULL, NULL, gis_SurLst_CB, "");
  GUI_Tip  (" change active surface ");

  wlbap = GUI_label__ (&wBoxEd, "- ", "l16");

  wenvx = GUI_entry__ (&wBoxEd, " X: ", "", NULL, NULL, "9");
  wenvy = GUI_entry__ (&wBoxEd, " Y: ", "", NULL, NULL, "9");
  wenvz = GUI_entry__ (&wBoxEd, " Z: ", "", NULL, NULL, "9");

  GUI_button__ (&wBoxEd, "Mod.Pt.", gis_CB1,    (void*)"PtMod", "");
  GUI_button__ (&wBoxEd, "Add Pt.", gis_CB1,    (void*)"PtAdd", "");



  // GUI_Butt (box2, "Test", gis_CB1,    (void*)"fTest", 0);
  GUI_button__ (&box2, "Help", gis_CB1,    (void*)"Help", "");
  wButEx = GUI_button__ (&box2, "Exit", gis_CB1,    (void*)"Exit", "");

  GUI_set_show (&tbGis1, 1);    // show

  return 0;

}


//=====================================================================
  int gis_CB1 (MemObj *mo, void **data) {
//=====================================================================


  long l1;
  char *cp1, cbuf1[256];

  // static void *oldMainFunc;


  // cp1 = (char*)data;
  cp1 = GUI_DATA_S1;

  printf("gis_CB1  /%s/\n",cp1);



  //-------------------------------------------------
  if(!strcmp(cp1, "Help")) {
    APP_Help (APP_NAME, "");



  //-------------------------------------------------
  } else if(!strcmp(cp1, "Exit")) {
    gCad_fini ();


  //-------------------------------------------------
  } else if(!strcmp(cp1, "print")) {
    UI_WinPrint1 (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncInit));


  //-------------------------------------------------
  } else if(!strcmp(cp1, "opePtab")) {
    APP_Open ("Open Pointfile", "*", (void*)gis_ptab_load);


  //-------------------------------------------------
  } else if(!strcmp(cp1, "savPtab")) {
    APP_Save ("Save Pointfile", "points.dat", (void*)gis_ptab_save);


  //-------------------------------------------------
  } else if(!strcmp(cp1, "bt_del")) {
    gis_del__ ();

  //-------------------------------------------------
  } else if(!strcmp(cp1, "delAll")) {  // delete active mesh & all points
    sprintf(cbuf1,"delete Net A%d and all of its Points",surTab[ActSurInd].indm);
    GUI_DialogYN (cbuf1, gis_del_pTab);

  //-------------------------------------------------
  } else if(!strcmp(cp1, "delPtR")) {  // delete (range of) points
    gis_del_ptRange_CB ();
    // GUI_GetText("Delete points: pointnumbers / range ", "12-34", -300,
                // gis_del_ptRange_CB);

  //-------------------------------------------------
  } else if(!strcmp(cp1, "movPts")) {  // move all points
    gis_movPts_CB ();
    // GUI_GetText("move all points - offset:", "100 0 0", -300, gis_movPts_CB);

  //-------------------------------------------------
  } else if(!strcmp(cp1, "CrSurf")) {
    gis_sur_off (0);  // disactivate surface surTab[ActSurInd]
    gis_ptab_init ();
    gis_sur_on (surTabNr-1);  // last in list




  //-------------------------------------------------
  } else if(!strcmp(cp1, "crMsh")) {
    if(actEl.rNr > 0) {
      TX_Print("***** WARNING: active Polygon not saved; select type of edgeline *****");
      return 0;
    }
    gis_mesh ();



  //-------------------------------------------------
  } else if(!strcmp(cp1, "edit")) {      // GisPt's, del GisEdgelines
    if(GUI_DATA_EVENT == TYP_EventRelease) return 0;   // skip disactivation
    GUI_set_enable (&wMenBar, TRUE);
    GUI_set_enable (&wBoxEd, TRUE);
    GUI_set_enable (&wButDel, TRUE);
    GUI_set_enable (&wledg, FALSE);  // edgetypes OFF
    gisFunc = GISFUNC_Edit;
    gis_changed_Func ();
    gis_msg__ (0);
    DL_hili_off (-1L); DL_Redraw ();


  //-------------------------------------------------
  } else if(!strcmp(cp1, "prjMsh")) {     // project Cad-object onto Mesh
    if(GUI_DATA_EVENT == TYP_EventRelease) return 0;   // skip disactivation
    GUI_set_enable (&wMenBar, FALSE);
    GUI_set_enable (&wBoxEd, FALSE);
    GUI_set_enable (&wButDel, FALSE);
    GUI_set_enable (&wledg, FALSE);  // edgetypes OFF
    gisFunc = GISFUNC_prjMsh;
    gis_changed_Func ();
    gis_msg__ (0);
    DL_hili_off (-1L); DL_Redraw ();
  

  //-------------------------------------------------
  } else if(!strcmp(cp1, "crEdge")) {     // create edgeline
    if(GUI_DATA_EVENT == TYP_EventRelease) return 0;   // skip disactivation
    GUI_set_enable (&wMenBar, FALSE);
    GUI_set_enable (&wBoxEd, FALSE);
    GUI_set_enable (&wButDel, FALSE);
    GUI_set_enable (&wledg, FALSE);  // edgetypes OFF
    gisFunc = GISFUNC_crEdge;
    gisEdg_init ();
    gis_changed_Func ();
    gis_msg__ (0);
    GL_temp_delete ();
    DL_hili_off (-1L); DL_Redraw ();


  //-------------------------------------------------
  } else if(!strcmp(cp1, "PtMod")) {
    gis_PtMod();


  //-------------------------------------------------
  } else if(!strcmp(cp1, "PtAdd")) {
    gis_PtAdd();


  //-------------------------------------------------
  } else if(!strcmp(cp1, "dumpPt")) {
    gis_dumpHtm_pTab ();


  //-------------------------------------------------
  } else if(!strcmp(cp1, "dumpEL")) {
    gis_dumpHtm_EL ();


  //-------------------------------------------------
  } else {
    TX_Error("gis_CB1 E001 %s",cp1);
  }


  L_exit:
    return 0;

  L_EOM:
    TX_Error("gis_CB1 EOM");
    return -1;


}


//================================================================
  int gis_changed_pTab () {
//================================================================

  ActPtabModFlag = 1;
  ActPtabBndModFlag = 1;
  gis_changed_mshMod (1); // modified; need update.

  return 0;

}


//================================================================
  int gis_changed_Func () {
//================================================================
// call whenever gisFunc changes

  printf("gis_changed_Func \n");


  return 0;

}


//================================================================
  int gis_changed_visi () {
//================================================================
// lock prjMsh if no mesh exists
// call whenever .visi changes


  // printf("gis_changed_visi %d\n",surTab[ActSurInd].visi);


  // if no mesh exists lock prjMsh
  if(surTab[ActSurInd].visi == 0) {
    GUI_set_enable (&wButPrj, FALSE);
  } else {
    GUI_set_enable (&wButPrj, TRUE);
  }

  return 0;

}


//================================================================
  int gis_changed_mshMod (int mshMod) {
//================================================================
// ActFtabModFlag changes  (mesh needs update)

  // printf("gis_changed_mshMod %d %d\n",mshMod,surTab[ActSurInd].visi);

  ActFtabModFlag = mshMod;



  // Exit-Button:
  //   if mesh exists: if mesh is modified, lock Exit-Button.
  // CreateMesh-Button:
  //   if mesh exists: if mesh is modified, change CreateMesh-Button color red
  if(surTab[ActSurInd].visi != 0) {
    if(ActFtabModFlag != 0) {
      GUI_set_enable (&wButEx, FALSE);
      GUI_button_styl (&wButMsh, 1);  // 1=red
    } else {
      GUI_set_enable (&wButEx, TRUE);
      GUI_button_styl (&wButMsh, 0);  // reset
    }

  } else {
    GUI_set_enable (&wButEx, TRUE);
    GUI_button_styl (&wButMsh, 0);  // reset
  }


  gis_ui_ButPrj ();   // block/unblock PrjMsh-Function

  return 0;

}


//================================================================
  int gis_changed_actElNr () {
//================================================================
// actEl.rNr changes


  // printf("gis_changed_actElNr %d\n",actEl.rNr);


  if(gisFunc == GISFUNC_crEdge) {    // create edge
    if(actEl.rNr > 0) {
      // if temp.edge exists: lock CreateMesh-Button.
      GUI_set_enable (&wButMsh, FALSE);
      // if temp.edge exists: lock Edit-Func.
      GUI_set_enable (&wButEdi, FALSE);


    } else {
      GUI_set_enable (&wButMsh, TRUE);
      GUI_set_enable (&wButEdi, TRUE);
    }

  }

  gis_ui_ButPrj ();   // block/unblock PrjMsh-Function

  return 0;

}


//================================================================
  int gis_ui_ButPrj () {
//================================================================
// block/unblock PrjMsh-Function


  // printf("gis_ui_ButPrj %d %d %d\n",actEl.rNr,
          // surTab[ActSurInd].visi,ActFtabModFlag);


  // if temp.edge exists: lock PrjMsh-Func.
  if(gisFunc == GISFUNC_crEdge) {    // create edge
    if(actEl.rNr > 0) goto L_OFF;
  }


  // if no mesh exists: block PrjMsh-Function.
  if(surTab[ActSurInd].visi == 0) goto L_OFF;


  // mesh exists; if mesh is modified: block PrjMsh-Function.
  if(ActFtabModFlag != 0) goto L_OFF;



  L_ON:
    GUI_set_enable (&wButPrj, TRUE);
    return 0;


  L_OFF:
    GUI_set_enable (&wButPrj, FALSE);
    return 0;

}
 

//================================================================
  int gis_msg__ (int mode) {
//================================================================

  if(mode == 1)
  TX_Print("cannot use object -");

  if(gisFunc == GISFUNC_Edit)
  TX_Print("select Gis-Point or Gis-Edgeline ..");

  else if(gisFunc == GISFUNC_prjMsh)
  TX_Print("select Point or Cad-Curve to project onto Mesh ..");

  else if(gisFunc == GISFUNC_crEdge)
  TX_Print("select Gis-Point or Cad-Point or Cad-Curve to add to EdgeLine ..");

  return 0;

}

 
/*
//================================================================
  int gis_obd_cre () {
//================================================================
// Create OuterBoundary from all points
// es fehlern noch die 2D-Punkte ..

  int       *ibp, ibNr;


  // get memSpc for the boundary ((p2Nr + 1) indices ..)
  i1 = p2Nr + 1;  // closing Pt
  ibp = (int*)UME_alloc_tmp(i1 * sizeof(int));
  // ibp = (int*)_alloca(p2Nr * sizeof(int));
  if(ibp == NULL) {TX_Error("gis_obd_cre EOF"); return -1;}


  //----------------------------------------------------------------
  // create outside-boundary into ibp
  MSH_ibnd_pt2n (ibp, &ibNr, p2a, p2Nr, NULL);
    printf(" ibNr=%d\n",ibNr);
    // for(i2=0;i2<i1;++i2) printf("ibp[%d]=%d\n",i2,ibp[i2]);

  // add closing point
  ibp[ibNr] = ibp[0];
  ++ibNr;

  // add OuterBound ibp to eTab
  gis_Edges_add (ibp, ibNr, 4);



  return 0;

}
*/


//================================================================
  int gis_mesh () {
//================================================================
// (re)create the mesh
// OFFEN:
// write "A# = MSH A<#_pTab>" into mainProg

  int     irc, att, i1, i2, ii, p2Nr, *ibp, ibNr;
  // int     *iba, ibNr;
  long    dli, dbm, dbp, l1;
  Point2  *p2a;
  // Mesh    ms1;
  // Fac3    *fTab = NULL;


  // printf("gis_mesh %d\n",ActSurPtb);

  gis_block (1);  // block input


  if(ActSurPtb < 0) goto L_exit;


  // fix mesh-surf-Nr
  if(surTab[ActSurInd].indm > -1) {           // use existing
    dbm = surTab[ActSurInd].indm;

  } else {                             // create new mesh-surf
    ++surIndLast;
    dbm = surIndLast;
      // printf(" next A-nr = %ld\n",dbm);
  }
    // printf(" dbm=%ld\n",dbm);


  // init edgeline-stats
  gis_EL_cleanI (&ActEtab, &DatEtab);


  // prepare space for 2D-points
  p2a = (Point2*)memspc501;
  i1 = sizeof(memspc501) / sizeof(Point2);  // 16 byte
  if(i1 < ActPtab.rNr) {TX_Error("gis_mesh E001"); goto L_exit;}


  // transport all points > 2D (pa -> p2a)
  // for XY-Plane copy points ..
  // and fill the 2D->-3D-IndexArray ipa
  // MSH_ptn2_ptn3 (p2a, &p2Nr, ipa, pa, pNr);
  MSH_ptn2_ptn3 (p2a, &p2Nr, ActPtab.data, ActPtab.rNr);


  // get memSpc for the boundary
  ibp = (int*)UME_alloc_tmp(p2Nr * sizeof(int));
  // ibp = (int*)_alloca(p2Nr * sizeof(int));
  if(ibp == NULL) {TX_Error("gis_mesh E002"); goto L_exit;}

  // create outer boundary
  irc = MSH_msh_OB (ibp, &ibNr, &ActEtab, p2a, p2Nr);


  // remove automatic created outer boundary
  i1 = MSH_eTab_findRecTyp (&ActEtab, TYP_EDGLN_AB);
  if(i1 >= 0) {
    MSH_eTab_delRec (&ActEtab, &DatEtab, i1);
    for(i2=i1; i2<ActEtab.rNr; ++i2) ActEtab.data[i2].stat = 1;
  }


  // add/update OuterBound ibp to ActEtab/DatEtab
  gis_Edges_add (ibp, ibNr, TYP_EDGLN_AB, 1);


  // remove outerZone - 1=yes.
  i1 = GUI_menu_checkbox_get (&wcboz);  // 0=not checked; 1=checked
    // printf(" outerZone = %d\n",i1);



  // create Mesh-data. Destroys ibp !
  irc = MSH_msh1 (&ActFtab, &ActEtab, &DatEtab,
                  ActPtab.data, ActPtab.rNr,
                  p2a, p2Nr, ibp, ibNr, i1);
  if(irc < 0) goto L_exit;
    // printf("gis_mesh 2 fNr=%d\n",ActFtab.rNr);
    // MSH_dump_eTab (&ActEtab, &DatEtab);
    // MSH_dump_fTab (ActFtab.data, ActFtab.rNr);

/*
  { double dsl;
    // skip this if user-defined outerBound exists ..
    // compute size of whole area (from outer boundary)
    MSH_msh_siz (&dsl, &ActEtab, ActPtab.data);  // TEST
    // dsl = max triangleSideLength; default is (areaDiameter / 2)
    dsl /= 2.;
    // test faces; set sap[]=1 for faces to remove
    MSH_msh_len_ck (dsl, &ActFtab, &ActEtab);
  }
*/

  // save Mesh into binFile
  // MSH_bsav_fTab (&ActFtab, &ActEtab, &DatEtab, -1, dbm);
  MSH_bsav_fTab (&ActFtab, &ActEtab, &DatEtab, WC_modact, dbm);


  // ActFtabModFlag = 0; // unmodified.
  gis_changed_mshMod (0); // reset, unmodified.
  // load faces



  // // update display of breaklines (after deleting and recreating the
  // // autoOuterBoundary.
  // gis_EL_cleanE (&ActEtab, &DatEtab);


  // update the mesh-display (copies dbm > surTab[ActSurInd].indm)
  gis_msh_disp (1, ActSurInd, dbm);

  ActSurMsh = dbm;

  // printf("gis_mesh 4\n");


  L_exit:

  DL_Redraw ();

  gis_block (0);  // unblock input

  return 0;

}

//================================================================
  int gis_msh_del (int mode) {
//================================================================
// gis_msh_del          remove mesh
// Input:
//   mode    0   delete permanent (delete model in main)    UNUSED ?
//           1   delete display only
// RetCod:
//    -1       no mesh exists for the active sur

// OFFEN:
// wenn man mesh löscht, bleibt der Attribut=symbolic-Record stehen;
// im main ist das mesh nun symbolic !

  long     dbi, dli;


  // printf("gis_msh_del %d\n",mode);


  dbi = surTab[ActSurInd].indm;
  if(dbi < 0) return -1;
    // printf(" dbi-m=%ld\n",dbi);


  // // delete bin.Mesh    sonst geht das UNDO ned ..
  // MSH_bdel_fTab (WC_modnr, dbi);

  if(mode == 1) {
    // delete mesh-display
    dli = DL_find_obj (Typ_SUR, dbi, -1L);
     // printf(" del %d %d\n",dbi,dli);
    GL_Del0 (dli);
    surTab[ActSurInd].visi = 0;

  } else {
    // delete permanent (delete model in main)
    if(surTab[ActSurInd].exi_m != 0) {
      // MSH-surf-Code already exists in Model
      // delete Mesh from model (deletes also its display)
      gis_mdl_del_msh (ActSurInd);
      // // löscht leider auch die points ..
      // gis_DrawPoints (ActPtab.data, ActPtab.rNr, ActSurPtb, 0);
    }
  }

  gis_changed_mshMod (0);  // CrMesh-Butt & Exit-Butt.
  gis_changed_visi ();     // lock prjMsh

  DL_Redraw ();

  return 0;

}


//================================================================
  int gis_msh_disp (int styl, int inds, long dbm) {
//================================================================
// display or redisplay mesh.
// styl: 0=ON=shade; 1=OFF=symbolic
// inds  index to surTab

// used in gis_mesh gis_sur_on gis_sur_off

  int     mode;
  long    dbi;


  // printf("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD \n");
  // printf("gis_msh_disp styl=%d inds=%d dbm=%ld\n",styl,inds,dbm);
// gis_dump_surTab ();



  // first time to create mesh ?
  dbi = dbm;
  if(surTab[inds].exi_m == 0) {
    // no model exists yet
    if(surTab[inds].indm < 0) {
      // first creation
      mode = 0;
      surTab[inds].indm = dbm;
    } else {
      mode = 1;
    }

  } else {
    // model exists; modify
    mode = 1;
  }




  // display mesh
  GR_draw_mesh (mode, dbi, styl, ActPtab.data, ActFtab.data, ActFtab.rNr);

  surTab[ActSurInd].visi = 1;

  gis_changed_visi ();  // unlock prjMsh

  return 0;

}
 

//================================================================
  int gis_disp_actObj () {
//================================================================
// display activeSurfNr & activePointNr

  char      cbuf[128];

/*
  // display new activeSurfNr in Label
  if(ActSurPtb >= 0) {
    sprintf(cbuf, " SurfNr. = A%d ",ActSurPtb);
  } else {
    sprintf(cbuf, " SurfNr. = -- ");
  }
  GUI_Label_mod (wlbas, cbuf);
*/

  // display new activePointNr in Label
  cbuf[0] = '\0';
  // if(ActPtNr >= 0) {
  if(ActSelTyp == APPTYP_GISPT) {
    sprintf(cbuf, " Gis-Point %d ",ActPtNr);
  }

  if(ActSelTyp == APPTYP_EDGLN) {
    sprintf(cbuf, " Edgeline %d", ActSelInd);
  }

  GUI_label_mod (&wlbap, cbuf);

  if(ActSelTyp != APPTYP_GISPT) {
    gis_disp_coords (NULL);
  }

  return 0;

}


//================================================================
  int gis_disp_pTab (int ii) {
//================================================================
// update display (boundary) of pTab.

  long     dli;

  // printf("gis_disp_pTab i=%d dbi=%d exi=%d\n",ii,
         // surTab[ii].indp, surTab[ii].exi_p);
  // printf("  ActPtabBndModFlag=%d\n",ActPtabBndModFlag);


  if(ii < 0) return 0;


  //----------------------------------------------------------------
  if(surTab[ii].exi_p == 1) {          // did exist in model before ..
      if(ActPtabBndModFlag == 0) {
        // ptab is unchanged: make it visible again;
        dli = DL_find_obj (Typ_SUR, surTab[ii].indp, -1L);
        if(dli >= 0) DL_unvis_set (dli, 0);  // set unvis=1

      } else {   // modified, recreate pTab-boundary
          // printf(" pTab %d modified ..\n",ActSurInd);
        dli = DL_find_obj (Typ_SUR, surTab[ii].indp, -1L);
        if(dli >= 0) {
          DL_unvis_set (dli, 0);
          DL_SetInd (dli);           // modify, not new
          APT_DrawSur (5, surTab[ii].indp);
        }   
      }


  //----------------------------------------------------------------
  } else {                             // did not exist in model before ..
    // pTab is new
    // APT_DrawSur (5, surTab[ii].indp);
    GR_DrawSurPtab (5, surTab[ii].indp);

  }




  return 0;

}


//================================================================
  int gis_disp_coords (Point *p1) {
//================================================================

  char      cbuf[60];

  cbuf[0] = '\0';
  if(p1) UTX_add_fl_u (cbuf, p1->x);
  GUI_entry_set (&wenvx, cbuf);
  cbuf[0] = '\0';
  if(p1) UTX_add_fl_u (cbuf, p1->y);
  GUI_entry_set (&wenvy, cbuf);
  cbuf[0] = '\0';
  if(p1) UTX_add_fl_u (cbuf, p1->z);
  GUI_entry_set (&wenvz, cbuf);

  return 0;

}


//================================================================
  int gis_key_CB (int key) {
//================================================================
// callback keypress


  // printf("gis_key_CB %d\n",key);

  switch (key) {

    case 'q':
      gCad_fini ();
      break;

    case GUI_KeyDel:
    case GUI_KeyNumDel:
      gis_del__ ();
      break;

    case 'D':          // Alt-Shift-D Testfunc DispList
      if(KeyStatCtrl != ON) {
        gis_dump_surTab ();
        DL_DumpObjTab ();
      }
      break;

  }

  return 0;

}

 
//================================================================
  int gis_sel_CB (int src, long dl_ind) {
//================================================================
// userselection-callback; CAD-Core reports selection


  int       i1, ipr, icp, ice, ide, idp, p1Nr, p2Nr, typ, *ia;
  long      dli, dbi;
  char      cbuf[128];
  Point     *pa1, *pa2, p1, p2;
  DL_Att    dla;
  ObjGX     ox1;


  printf("gis_sel_CB src=%d dl_ind=%ld\n",src,dl_ind);


  ActSelTyp = -1;
  ActSelInd = -1;


  if(dl_ind < 0) goto L_ind__;        // indicate

  // get DL-record of selected object
  dla = DL_GetAtt(dl_ind);
    // printf(" sel.typ=%d\n",dla.typ);

  if(dla.typ == Typ_APPOBJ) goto L_gis_obj__;
  goto L_cad_obj__;



  //================================================================
  // indicate
  //================================================================
  L_ind__:

  // get userCoords of indicated point
  p1 = GL_GetViewPos ();


  if(gisFunc == GISFUNC_Edit)  {         // 1=edit
    // create gis-point
    goto L_cre_gisPt;
  }


  if(gisFunc == GISFUNC_prjMsh) {        // 2=prjMsh,
    // project pt -> mesh
    i1 = MSH_pt_prjptmsh_ (&p1, &p1, ActSurMsh, ActSurPtb);
    if(i1 != 0) goto L_err_ptOut;
    // create gis-point
    goto L_cre_gisPt;
  }


  if(gisFunc == GISFUNC_crEdge)  {        // 3=crEdge
    // create gis-point
    gis_disp_coords (&p1);
    gis_PtAdd ();
    // add gisPt to edge
    gisEdg_addGisPt (0);
    gis_changed_actElNr ();    // actEl.rNr changed
  }


  return 0;




  //===================================================
  // gis-obj selected
  //===================================================
  L_gis_obj__:
  if(dla.iatt != APPTYP_GISPT) goto L_sel_EL;

  //---------------------------------------------------
  // gis-Point selected
  //---------------------------------------------------
    // printf(" gis-Point selected ..\n");

  // for all gisFunc's: select PisPt as active pt
    ActSelTyp = APPTYP_GISPT;

    // decode Object gis-point
    ActPtDli = dl_ind;
    ActPtNr  = dla.ind;         // PointNr
    ActSelInd = dla.ind;        // PointNr

    // display activeSurfNr & activePointNr
    gis_disp_actObj ();

    // display PointCoords in wenvx/y/z
    gis_disp_coords (&ActPtab.data[ActPtNr]);

    // gis-point selected:
    // if(idp != 0) { gis_del__ (); return 0; }   // idp=1 "delete GIS-Point"
    // if(icp != 0) goto L_cre_cadPt0;   // AddPt-Button


  if(gisFunc == GISFUNC_Edit) return 0;    // 1=edit

  // crEdge: add selected gis-point to active EdgeLine
  if(gisFunc == GISFUNC_crEdge) {
    gisEdg_addGisPt (0);
    gis_changed_actElNr ();    // actEl.rNr changed
    
  }

  // prjMsh: create cad-point from gis-point
  if(gisFunc == GISFUNC_prjMsh) {gis_cre_cadPt (); return 0;}

  // gis_msg__ (1);  // cannot use object ..
  return 0;


  //---------------------------------------------------
  // gis-Edgeline selected
  L_sel_EL:
  if(dla.iatt != APPTYP_EDGLN) {TX_Print("gis_sel_CB I001"); return -1;}
    // printf(" EdgeLine selected ..\n");

  ActSelTyp = APPTYP_EDGLN;
  ActSelInd = dla.ind;        // edgeNr

  if(gisFunc == GISFUNC_Edit) {gis_disp_actObj(); return 0;}

  // cpyCad ??
  // gis_del__ ();
  gis_msg__ (1);  // cannot use object ..
  return 0;



  //===================================================
  // cad-obj selected:
  //===================================================
  L_cad_obj__:
  // test if it is a ptab Typ_SURPTAB 149
  if(dla.typ != Typ_SUR) goto L_CadObj;
    // DL_DumpObj__ (dl_ind);
/*
  ox1 = DB_GetObjGX (dla.typ, dla.ind);
    printf(" typ=%d\n",ox1.typ);
  if(ox1.typ == Typ_Error) {  // SURMSH not yet stored ?
    TX_Print(" dynamic obj ");
    return 0;
  }
    UT3D_stru_dump(Typ_ObjGX, &ox1, "gis sel obj:\n");
*/
  
  //----------------------------------------------------------------
  if(dla.ind == surTab[ActSurInd].indp) {
  // if(ox1.typ == Typ_SURPTAB) {
    // printf(" SURPTAB selected ..\n");
                           


  //----------------------------------------------------------------
  // test if it is a mesh Typ_SURMSH 160
  } else if(dla.ind == surTab[ActSurInd].indm) {
  // } else if(ox1.typ == Typ_SURMSH) {
    ActSelTyp = APPTYP_MESH;
    ActSelInd = dla.ind;        // edgeNr

  }

  gis_msg__ (1);  // cannot use object ..
  return 0;



  //================================================================
  // cad-obj selected:
  L_CadObj:

  //----------------------------------------------------------------
  // cad-point selected:
  if(dla.typ != Typ_PT) goto L_cadObj1;
    // pa1 = DB_get_PT (dla.ind);
    p2 = DB_GetPoint (dla.ind);

    // if(idp > 0) return 0; // DelGis-Pt: skip selection of cad-point

    if(gisFunc == GISFUNC_Edit) {            // 1=edit
      gis_disp_coords (&p2);
      gis_PtAdd ();
      goto L_exit;                // Redraw, exit.
    }

    if(gisFunc == GISFUNC_prjMsh) {            // 2=prjMsh
      i1 = MSH_pt_prjptmsh_ (&p1, &p2, ActSurMsh, ActSurPtb);
      if(i1 != 0) goto L_err_ptOut;
      // pa1 = &p1;
      goto L_cre_cadPt1;
    }
    
    if(gisFunc == GISFUNC_crEdge) {                 // 3=crEdge
      ActPtNr = -1;
      gis_addPtab (&ActPtNr, &p2, 1);  // create GIS-point
      gisEdg_addGisPt (0);             // add Gis-point to active EdgeLine;
      gis_changed_actElNr ();    // actEl.rNr changed
      goto L_exit;                     // Redraw, exit.
    }

    // was tun mit diesem Punkt ?
    gis_msg__ (1);  // cannot use object ..
    return 0;



  //----------------------------------------------------------------
  L_cadObj1:

    // decode; get polygon from DB-object.
    pa1 = (Point*)memspc201;
    p1Nr = sizeof(memspc201) / sizeof(Point);
    i1 = UT3D_pta_dbo (&pa1, &p1Nr, dla.typ, dla.ind, UT_DISP_cv);
    if(i1 < 0) return 0;


    // project Obj
    if(gisFunc == GISFUNC_prjMsh) {                    // 2=prjMsh
      // create polygon from inputcurve
      i1 = MSH_npt_prjcvmsh_ (&pa2, &p2Nr, pa1, p1Nr,
                              &ActFtab, &ActPtab, &ActEtab, &DatEtab);
      if(i1 != 0) {TX_Print("obj. is not inside mesh .."); return 0;}
      pa1 = pa2;
      p1Nr = p2Nr;
      goto L_cre_CadPlg;
    }


    if(gisFunc == GISFUNC_crEdge) {                    // 3=crEdge
      // create GIS-points from polygon
      ia = UME_alloc_tmp (p1Nr * sizeof(int));
      for(i1=0; i1<p1Nr; ++i1) ia[i1] = -1;  // create new points
      gis_addPtab (ia, pa1, p1Nr);
      // create Edgeline from polygon (add cv to actEl)
      gisEdg_addPtab (ia, p1Nr);
      gis_changed_actElNr ();    // actEl.rNr changed
      return 0;
    }


    // was tun mit diesem object ?
    gis_msg__ (1);  // cannot use object ..
    return 0;



  //----------------------------------------------------------------
  L_exit:
    DL_Redraw ();
    return 0;



  //----------------------------------------------------------------
  // create gis-point p1
  L_cre_gisPt:
    gis_disp_coords (&p1);
    gis_PtAdd ();
    return 0;


  //----------------------------------------------------------------
  // create cad-point
  L_cre_cadPt0:
    p1 = ActPtab.data[ActPtNr];


  // create a CAD-Point at p1
  L_cre_cadPt1:
    i1 = DB_dbo_get_free (Typ_PT);
    ++i1;
    sprintf(cbuf, "P%d=",i1);
    AP_obj_add_pt(cbuf, &p1);        // create text
    ED_srcLn_add (cbuf, 0);          // create cad-object
  
    return 0;



  //----------------------------------------------------------------
  // create CAD-Polygon from  pa1[p1Nr]
  L_cre_CadPlg:
  UTO_sav_ost (&i1, &dbi, Typ_CVPOL, Typ_PT, p1Nr, (void*)pa1);

  return 0;



  //----------------------------------------------------------------
  L_err_ptOut:
  TX_Print("***** point is not inside mesh ..");
  return 0;


}


//================================================================
  int gis_cre_cadPt () {
//================================================================
// create a CAD-Point at p1

  int      typ;
  long     dbi;
  Point    *p1;

  // printf("gis_cre_cadPt \n");

  p1 = &ActPtab.data[ActPtNr];

/*
    i1 = DB_dbo_get_free (Typ_PT);
    ++i1;
    sprintf(cbuf, "P%d=",i1);
    AP_obj_add_pt(cbuf, &p1);

    ED_add_Line (cbuf);
    DL_Redraw ();                 // Redraw DispList
*/

  UTO_sav_ost (&typ, &dbi, Typ_PT, Typ_PT, 1, p1);

  TX_Print ("Cad-Point P%ld created ..",dbi);

  return 0;

}


//================================================================
  int gis_movPts_CB () {
//================================================================
// 
     
  int     irc, i1;
  char    s1[80];
  Vector  vc1;

  // strcpy(cbuf1, "100 0 0");
  irc = GUI_Dialog_e2b ("move all points - offset:", s1, 80, "OK", "Cancel");
  if(irc != 0) return -1;


    printf("gis_movPts_CB |%s|\n",s1);
    
  // decode text -> point
  UT3D_vc_txt (&vc1, s1);

  // move all points
  gis_ptab_move (&vc1, ActPtab.data, ActPtab.rNr);

  // redraw all points
  gis_HidePoints (0);
  gis_DrawPoints (ActPtab.data, ActPtab.rNr, ActSurPtb, 0);

  // redraw all edgelines
  // gis_EL_cleanE (&ActEtab, &DatEtab);
  for(i1=0; i1 < ActEtab.rNr; ++i1)
    gis_DrawEL (&ActEtab.data[i1], i1);
  DL_Redraw ();

  gis_changed_pTab ();    // modified; need update.

  return 0;

}


//================================================================
  int gis_del_ptRange_CB () {
//================================================================
// 
 
  int     irc, i1;
  double  d1, d2;
  char    *w, s1[80];


  // strcpy(cbuf1, "12-34");
  irc = GUI_Dialog_e2b ("Delete points: pointnumbers / range ",
                        s1, 80, "OK", "Cancel");
  if(irc != 0) return -1;


      // printf("gis_del_ptRange_CB |%s|\n",(char*)data);

    w = s1;

    L_nxt:
    irc = UTX_sget_nrRange (&d1, &d2, &w);
      // printf(" _nrRange %d %f %f\n",irc,d1,d2);
    if(irc == 1) {  // single ptNr
      gis_PtDel__ ((int)d1);

    } else if(irc == 2) {  // range
      for(i1=d2; i1>=d1; --i1) gis_PtDel__ (i1);
    }

    // do not continue; points become renumbered !!
    // if(irc > 0) goto L_nxt;


  return 0;

}

 
//================================================================
  int gis_del__ () {
//================================================================
// gis_del__            delete active gis-obj

  int      i1, ii, iPt, iLast;
  long     dli;
  ObjDB    *gTab;
  DL_Att    dla;


  // printf("gis_del__ %d\n",ActSelTyp);


  // test if group is active ...
  ii = Grp_get__ (&gTab);
    // printf(" ii=%d\n",ii);
  if(ii < 1) goto L_noGrp;


  // must delete last pointNrs first (following pointNrs become changed)
  // get last pointNr
  gis_block (1);
  TX_Print ("delete %d points ..",ii);
  iLast = UT_INT_MAX;
  L_nxtPt:
  iPt = -1;
  for(i1=0; i1<ii; ++i1) {  // loop tru group-of-selected-objs
    if(gTab[i1].typ != Typ_APPOBJ) continue;
    dla = DL_GetAtt(gTab[i1].dlInd);
    if(dla.iatt != APPTYP_GISPT) continue;
    if((i1 > iPt)&&(i1 < iLast)) { iPt = i1; dli = dla.ind;}
  }
  if(iPt >= 0) {
    iLast = iPt;
    gis_PtDel__ (dli);
    goto L_nxtPt;
  }


  // clear group
  Grp_Clear (1);
    ActPtNr = -1;
  gis_block (0);
  return 0;


  //----------------------------------------------------------------
  L_noGrp:
  if(ActSelTyp == -1) {
    TX_Print("***** select (gis-) object to delete first ..");
    return 0;
  }



  //----------------------------------------------------------------
  if(ActSelTyp == APPTYP_GISPT) {
    gis_PtDelAct ();


  //----------------------------------------------------------------
  } else if(ActSelTyp == APPTYP_EDGLN) {
    gis_Edges_del (ActSelInd);
    ActSelTyp = -1;
    gis_disp_actObj (); // remove display


  //----------------------------------------------------------------
  } else if(ActSelTyp == APPTYP_MESH) {
    gis_msh_del (1);


  //----------------------------------------------------------------
  } else if(ActSelTyp == -2) {        // crEdge added point(s)
    if(actEl.rNr > 0) {
      actEl.rNr -= 1;
      gis_actEl_disp ();
      DL_Redraw ();
    }
    if(actEl.rNr < 1) {
      gis_changed_actElNr ();
    }
    return 0;  // do not change ActSelTyp
  }

  ActSelTyp = -1;


  return 0;

}
 

//================================================================
  int gis_addPtab (int *iTab, Point *pTab, int pNr) {
//================================================================
// create GIS-points; return pointNumbers.
// iTab     must have size pNr; -1 = get new PointNr, >=0 modify this point.
// see also gis_PtAdd
// returns iTab in memspc102 ! (do not free)

  int    irc, i1, i2, iAct, iaSiz, ii, ip1, ip2, ip3, iMod;
  char   *elStat;


  // printf("gis_addPtab %d\n",pNr);


  // // get memspc for iTab
  // ia = (int*)memspc102;
  // iaSiz = sizeof(memspc102) / sizeof(int);
  // if(iaSiz < pNr) {TX_Error("gis_addPtab EOM1"); return -1;}

  iMod = 0;

  elStat = (char*) UME_alloc_tmp (ActEtab.rNr);
  for(i1=0; i1 < ActEtab.rNr; ++i1) elStat[i1] = 0;


  for(iAct=0; iAct<pNr; ++iAct) {

    //----------------------------------------------------------------
    // get ii = index into pTab
    if(iTab[iAct] < 0) {
      // test if point already exists; (2D-Test; tolerance = TOL_GIS_PT)
      ii = MSH_pt_ck_pTab (ActPtab.data, ActPtab.rNr, &pTab[iAct], TOL_GIS_PT);
      // -1=new, >=0=already exists
      if(ii >= 0) {
        TX_Print("point %d modified; check Z-value ..",ii);
      } else {
        ii = gis_freePos_pTab ();    // get a free pos in ActPtab
        if(ii < 0) {TX_Error("gis_addPtab E001"); return -1;}
      }


    } else {
      ii = iTab[iAct];             // modify point !
    }


    //----------------------------------------------------------------
    // modify point used by edgeline: check for crossing edgelines
    if(iTab[iAct] >= 0) {
      // test if point ii is used by existing edgeline;
      i1 = -1;   // init for 1.run
      L_mod_ckNxt:
      // get next occurence of ii in edges
      irc = MSH_EL_ck_pt (&i1, &i2, &ActEtab, &DatEtab, ii);
      if(irc == -2) goto L_ck_lines;
      if(irc == -1) goto L_save;        // already tested; done.
      // get 1 or 2 segments around pt i2 on egde i1
      // &ip2 is to be modified, ip1 & ip3 keep the pos.
      MSH_3ip_EL_iip (&ip1, &ip2, &ip3, &ActEtab, &DatEtab, i1, i2);
      if(ip1 >= 0) {
        // test if seg ip1-ip2 intersects other edgeline or goes tru point
        irc = MSH_EL_ck__ (&pTab[iAct], &ip1, ii,
                           &ActEtab, ActPtab.data, ActPtab.rNr, TOL_GIS_PT);
        if(irc >= 0) continue; // ERROR!
      }
      // test if seg ip2-ip3 intersects other edgeline or goes tru point
      if(ip3 >= 0) {
        irc = MSH_EL_ck__ (&pTab[iAct], &ip3, ii,
                           &ActEtab, ActPtab.data, ActPtab.rNr, TOL_GIS_PT);
        if(irc >= 0) continue; // ERROR!
      }
      // update the edgelinedisplay
      elStat[i1] = 1;  // modified
      goto L_mod_ckNxt;
    }


    //----------------------------------------------------------------
    // test if point lies on a edgeline
    L_ck_lines:
    irc = gisPt_ck_EL__ (&i1, &i2, (Point2*)&pTab[iAct]);
    if(irc >= 0) {
      // pt lies on a breakline. do not store this point
      TX_Print("***** point at %f,%f ignored; lies on breakline %d - %d",
                pTab[iAct].x, pTab[iAct].y, i1, i2);
      continue;
    }


    //----------------------------------------------------------------
    // save point in ii
    L_save:
    ++iMod;
    ActPtab.data[ii] = pTab[iAct];

    // display point
    if(iTab[iAct] >= 0) {
      // modify, not new ..
      DL_SetInd (ActPtDli);
    }
    gis_DrawPoints (&ActPtab.data[ii], 1, ActSurPtb, ii);


    iTab[iAct] = ii;
  }


  //----------------------------------------------------------------
  // fix display of modified breaklines
  for(i1=0; i1 < ActEtab.rNr; ++i1) {
    if(elStat[i1] == 1) gis_DrawEL (&ActEtab.data[i1], i1);
  }


  //----------------------------------------------------------------
  DL_Redraw ();

  gis_changed_pTab (); // modified; need update.

  TX_Print ("%d points modified ..",iMod);

    // for(ii=0;ii<pNr;++ii) printf(" iTab[%d]=%d\n",ii,iTab[ii]);

  return 0;

}


//================================================================
  int gisPt_ck_EL__ (int *ip1, int *ip2, Point2 *ptx) {
//================================================================
// test if point lies on a edgeline
// test if point is equal to existing point already done !
// RetCod: -1=point is not on edge; else pointNr.

// see also MSH_EL_ck_pTab


  int      irc, i1, i2, *ia, iNr;
  Point2   p1, p2;
  Point    *pTab;
  EdgeLine *el1;


  // printf("gisPt_ck_EL__\n");

  if(ActEtab.rNr < 2) return -1;
    
  // pNr  = ActPtab.rNr;
  pTab = ActPtab.data;


  // loop tru existing edges
  for(i1=0; i1<ActEtab.rNr; ++i1) {
    el1 = &ActEtab.data[i1];
    ia  = el1->ia;
    iNr = el1->iNr;
    for(i2=1; i2<iNr; ++i2) {
      // breaklineSeg [i2-1]-[i2]
        // printf(" test %d %d\n",ia[i2-1],ia[i2]);
      *ip1 = ia[i2-1];
      *ip2 = ia[i2];
      // if((i1 == *ip1) || (i1 == *ip2)) continue;
      p1 = UT2D_pt_pt3 (&pTab[*ip1]);
      p2 = UT2D_pt_pt3 (&pTab[*ip2]);
      irc = UT2D_pt_ck_onLine (NULL, &p1, &p2, ptx, TOL_GIS_PT);
      if(irc >= 0) return irc;

    }
    // types TYP_EDGLN_IB and TYP_EDGLN_OB and TYP_EDGLN_AB have closing edges
    if(el1->typ != TYP_EDGLN_BL) {
        // printf(" test %d %d\n",ia[iNr-1],ia[0]);
      *ip1 = ia[iNr-1];
      *ip2 = ia[0];
      // if((i1 == *ip1) || (i1 == *ip2)) continue;
      p1 = UT2D_pt_pt3 (&pTab[*ip1]);
      p2 = UT2D_pt_pt3 (&pTab[*ip2]);
      irc = UT2D_pt_ck_onLine (NULL, &p1, &p2, ptx, TOL_GIS_PT);
      if(irc >= 0) return irc;
    }
  }


  return -1;

}


//=====================================================================
  int gis_lst_sup () {
//=====================================================================
// create list of point-surfaces from surTab

  int    i1, sl;
  char   *pBuf[MAX_SURPTAB], cbuf1[40], *cBuf2;


  cBuf2 = memspc55;


  // Create a List of Pointers for a list of words.
  for(i1=0; i1<surTabNr;  ++i1) {
    sprintf(cbuf1, " A%d ", surTab[i1].indp);
    sl = strlen(cbuf1);
    strcpy(cBuf2, cbuf1);
    cBuf2[sl] = '\0';
    pBuf[i1] = cBuf2;
    cBuf2 += sl+1;
  }

  pBuf[surTabNr] = NULL; // terminat List


  // change surf
  GUI_optmen_chg (&wls, pBuf, NULL);




  return 0;

}


//===================================================================
  int gis_EdgTyp_CB (MemObj *mo, void **data) {
//===================================================================
// userselection of listEntry of surfaces
// 0=save EdgeLine                  elnEL
// 1=save as InnerBoundary          ibdEL
// 2=save OuterBoundary             obdEL
  
  int   ii; 
  
  ii = GUI_DATA_I1;
  
  // printf("gis_EdgTyp_CB %d\n",ii);


  //================================================================
  if(ii == 0) {                     //  save EdgeLine (bReakline)
     // check if closed
      actEl.use = TYP_EDGLN_BL; // EdgeLine
      TX_Print("active Polygon changed to EdgeLine ..");
      gis_Edges_upd (TYP_FuncAdd);
      // GUI_MenuCkBoxSet (wcbcre, 0);     // create_polygon off



  //================================================================
  } else if(ii == 1) {              //  save as InnerBoundary
      // check if closed
      actEl.use = TYP_EDGLN_IB; // InnerBound
      TX_Print("active Polygon changed to InnerBound ..");
      gis_Edges_upd (TYP_FuncAdd);
      // GUI_MenuCkBoxSet (wcbcre, 0);     // create_polygon off



  //================================================================
  } else if(ii == 2) {              //  save as OuterBoundary
      // check if closed
      actEl.use = TYP_EDGLN_OB; // OuterBound
      TX_Print("active Polygon changed to OuterBound ..");
      gis_Edges_upd (TYP_FuncAdd);
      // GUI_MenuCkBoxSet (wcbcre, 0);     // create_polygon off




  //================================================================
  }

  gis_changed_actElNr ();    // actEl.rNr changed

  return 0;

}


//===================================================================
  int gis_SurLst_CB (MemObj *mo, void **data) {
//===================================================================
// userselection of listEntry of surfaces

  int   lNr;

  lNr = GUI_DATA_I1;

  printf("gis_SurLst_CB A%d\n",surTab[lNr].indp);


  /// activate surface surTab[lNr]
  gis_sur_off (0);     // disactivate surface surTab[ActSurInd]
  gis_sur_on (lNr);



  return 0;

}


//================================================================
  int gis_init_pl () {
//================================================================
// fill surTab-List from ModelCodes


/*
  int      i1, grpNr;
  ObjDB   grp[MAX_SURPTAB];
  ObjGX    flt[1];
  flt[0].typ = Typ_SURPTAB;  // filter all surfaces
  grpNr = MAX_SURPTAB;
  i1 = DB_Filter__ (&grp, &grpNr, &flt, 1);
  // fehlerhaft; da Objekte in der DB nicht geloescht werden !
*/


  int   irc, typ, ii, lNr, lLen;
  long  ind1, ind2;
  char  *lSta, *p1, *p2, oid[32];


  // printf("---------- gis_init_pl ----------\n");

  lSta = NULL;

  surTabNr = 0;
  // scan whole modelsource
  for(;;) {
    // get next surface
    typ = Typ_SUR;
    irc = APED_nxt_def_typ (&lSta, &lNr, &lLen, typ);
    if(irc < 0) break;
      // printf(" nxt ln |");UTX_dump_cnl (lSta, 40);printf("|\n");
      
    // irc = APED_oid_dbo__ (&typ, &ind1, lSta);
    // if(irc != 0) continue;
    ind1 = strtol (&lSta[1], &p2, 10);
      // printf(" typ=%d ind1=%d\n",typ,ind1);

    // get pos of word after '='
    p1 = UTX_find_Del1 (lSta);
    while(*p1 == ' ') ++p1;
    if(*p1 != '=') continue;


    // find next delimiter
    ++p1;
    while(*p1 == ' ') ++p1;

    // check for PTAB-SUR;
    if(!strncmp(p1, "PTAB", 4)) {
      surTab[surTabNr].indp   = ind1;           // DB-Index.
      surTab[surTabNr].indm   = -1;
      surTab[surTabNr].visi  = 0;               // not yet visible
      surTab[surTabNr].exi_p = 1;               // 1=exists in Model
      surTab[surTabNr].exi_m = 0;               // 0=does not exists in Model
      ++surTabNr;
    }

    // check for MSH-SUR;
    if(!strncmp(p1, "MSH", 3)) {
      // find db-ind of pointSurface
      p1 += 4;
      while(*p1 == ' ') ++p1;
      irc = APED_dbo_oid (&typ, &ind2, p1);
        // printf(" p1=|%s| %d %d\n",p1,typ,ind2);
      if(irc != 0) continue;
      if(typ != Typ_SUR) {printf("gis_init_pl E001\n"); continue;}
      // find pointSurf with ind ind1; set mesh-ind to ind2
      ii = gis_surInd_surNr (ind2);
      if(ii < 0) {printf("gis_init_pl E002\n"); continue;}
      surTab[ii].indm = ind1;
      surTab[ii].exi_m = 1;
      surTab[ii].visi  = 1;
        // printf(" surTab[%d].indm = %d\n",ii,ind2);
    }

  }


  // printf("ex gis_init_pl %d\n",surTabNr);
  // gis_dump_surTab ();

  return 0;

}


//================================================================
  int gis_UI_getPt (Point *pt1) {
//================================================================

  char    *p1, *p2;


  p1 = GUI_entry_get (&wenvx);
  if(strlen(p1) < 1) goto L_err1;
  pt1->x = strtod (p1, &p2);

  p1 = GUI_entry_get (&wenvy);
  if(strlen(p1) < 1) goto L_err1;
  pt1->y = strtod (p1, &p2);

  p1 = GUI_entry_get (&wenvz);
  if(strlen(p1) < 1) goto L_err1;
  pt1->z = strtod (p1, &p2);

    // UT3D_stru_dump(Typ_PT, pt1, "ex gis_UI_getPt:");

  return 0;

  L_err1:
  TX_Print("***** coordinate - inputfield empty *****");
  return -1;

}


//================================================================
  int gis_surInd_surNr (long surNr) {
//================================================================
// get index into surTab from DB-index of pointSurface

  int surInd;

  for(surInd=0; surInd<surTabNr;  ++surInd) {
    if(surTab[surInd].indp == surNr) return surInd;
  }

  return -1;

}


//================================================================
  int gis_dispDel_pTab (int iSur) {
//================================================================
// delete display of boundary of pTab of surTab[iSur]

  long     dli;


  // printf("gis_dispDel_pTab \n");


  // search in DL typ=Typ_SUR with ind=surTab[iSur].indp
  // if(surTab[iSur].exi_p == 1)
  if(surTab[iSur].indp < 0) return 0;
  dli = DL_find_obj (Typ_SUR, surTab[iSur].indp, -1L);
  if(dli >= 0) GL_Del0 (dli);


  return 0;

}


//================================================================
  int gis_ptab_del () {
//================================================================
// gis_ptab_del         remove all Points (from surface ActSurPtb)

// see also gis_PtDel__ MSH_bdel_pTab gis_sur_off

  // printf("gis_ptab_del \n");

  gis_HidePoints (surTab[ActSurInd].indp); // delete display of all points

  // delete display of boundary of pTab
  gis_dispDel_pTab (ActSurInd);

  // delete "A#" from Model
  gis_mdl_del_pTab (ActSurInd);

  ActPtab.rNr = 0;


  return 0;

}


//================================================================
  int gis_del_pTab (MemObj *mo, void **data) {
//================================================================
// delete complete surTab-Record

  int     surInd;


  if(GUI_DATA_EVENT == UI_FuncCancel) return 0;

  surInd = ActSurInd;

  //----------------------------------------------------------------
  // delete actEl & alle EdgeLines
  gis_Edges_all_del ();


  //----------------------------------------------------------------
  // delete Mesh
  gis_msh_del (1);   // remove mesh-display
  gis_msh_del (0);   // delete mesh-modelcode


  //----------------------------------------------------------------
  // delete pTab
  gis_ptab_del ();


  //----------------------------------------------------------------




  L_9:
  // delete record # ActSurInd from surTab
  MEM_del_nrec (&surTabNr, surTab, surInd, 1, sizeof(typ_sur_ptab));

  // if no mesh exists, init one
  if(surTabNr < 1) gis_ptab_init ();

  // update surf-list
  gis_lst_sup ();

  // activate last surf
  gis_sur_on (surTabNr-1);

  DL_Redraw ();

  return 0;

}

 
//================================================================
  int gis_PtDelAct () {
//================================================================
// delete Point Nr ActPtNr from surface ActSurPtb
// Point.x is set to UT_VAL_MAX; nothing else ..


  // printf("gis_PtDelAct %d %d %ld\n",ActSurPtb,ActPtNr,ActPtDli);


  if(ActPtNr < 0) {
    TX_Print(" select point to delete ..");
    return 0;
  }
  if(ActPtNr >= ActPtab.rNr) { TX_Error("gis_PtDelAct E001"); return -1; }


  // delete point 
  // gis_PtDel__ (ActPtDli, ActPtNr);
  gis_PtDel__ (ActPtNr);


  // update Labels; ActPtNr-display
  ActSelTyp = -1;
  ActPtNr = -1;
  gis_disp_actObj ();  // display labelext wlbap & coords


  return 0;

}

 
//================================================================
  int gis_PtDel__ (int iDelPt) {
//================================================================
// delete Point Nr iDelPt from surface ActSurPtb

  int      i1, iLastPt;
  long     l1;
  char     info[20];
  EdgeLine *el1;


  // printf("gis_PtDel__ %d\n",iDelPt);

  if(iDelPt < 0) return -1;
  if(iDelPt >= ActPtab.rNr) return -1;


  // delete point ActPtNr from Display
  // DL_unvis_set (dli, 1);
  // GL_Del0 (dli);


  // iDelPt = point to be removed.
  // iLastPt = last point


  // find last point - remove display
  iLastPt = ActPtab.rNr - 1;
  l1 = DL_find_APPOBJ (iLastPt, APPTYP_GISPT);  // index = index -1 !
  if(l1 >= 0) GL_Del0 (l1);


  // delete last point in pTab
  ActPtab.rNr -= 1;
  if(iLastPt == iDelPt) goto L_reset;
  // copy iLastPt -> iDelPt;
  ActPtab.data[iDelPt] = ActPtab.data[iLastPt];


  // modify point iDelPt-display
  i1 = iDelPt;
    l1 = DL_find_APPOBJ (i1, APPTYP_GISPT);  // index = index -1 !
    if(l1 >= 0) DL_SetInd (l1);
    l1 = DL_StoreObj (Typ_APPOBJ, i1, APPTYP_GISPT);
    sprintf(info, "%d", i1);
    gis_DrawPoint (&l1, SYM_TRI_S, &ActPtab.data[i1], 0, 1, info);



  // ------------------------------------------
  // reset stat in all EdgeLines
  L_reset:
  gis_EL_cleanI (&ActEtab, &DatEtab);

  // delete point iDelPt in all edgelines
    // MSH_dump_eTab (&ActEtab, &DatEtab);           // TEST ONLY
  MSH_EL_del_pt (&ActEtab, &DatEtab, iDelPt);
    // MSH_dump_eTab (&ActEtab, &DatEtab); //return 0; // TEST ONLY
  // die pointers ActEtab[].ia neu aufbauen
  MSH_eTab_fix (&ActEtab, &DatEtab);



  // change iLastPt into iDelPt in all edgelines
  MSH_EL_chg_pt (&ActEtab, &DatEtab, iDelPt, iLastPt);
    // MSH_dump_eTab (&ActEtab, &DatEtab); //return 0; // TEST ONLY


  // remove edgelines with less than 2 points.
  // update edgelinedisplay.
  gis_EL_cleanE (&ActEtab, &DatEtab);
    // MSH_dump_eTab (&ActEtab, &DatEtab); return 0; // TEST ONLY
  // -------------------------------------------


  DL_Redraw ();

  gis_changed_pTab (); // modified; need update.
 
  return 0;

}


//================================================================
  int gis_PtMod () {
//================================================================
// modify Point Nr ActPtNr from surface ActSurPtb

  int      i1;
  Point    pt1;
  EdgeLine *el1;


  // printf("gis_PtMod %d %d %ld\n",ActSurPtb,ActPtNr,ActPtDli);


  if(ActPtNr < 0) {
    TX_Print(" select point to modify ..");
    return 0;
  }


  // get point from UI
  i1 = gis_UI_getPt (&pt1);
  if(i1 < 0) return -1;


  // test if point already exists; (2D-Test; tolerance = TOL_GIS_PT)
  // ActPtab.data[ActPtNr].x = UT_DB_LEER;  // cannot find this point as equal

  // modify point
  gis_addPtab (&ActPtNr, &pt1, 1);


  // update ActPtNr-display
  ActSelTyp = APPTYP_GISPT;
  gis_disp_actObj ();

  DL_Redraw ();

  // TX_Print("point %d modified ..",ActPtNr);

  return 0;

}


//================================================================
  int gis_ptab_init () {
//================================================================
 
  int  dbiNew;


  // printf(" gis_ptab_init\n");


  // surNr = find next free surface-nr (A)
  ++surIndLast;
  dbiNew = surIndLast;
  // ActSurPtb = surIndLast;
    printf(" init dbiNew=%d\n",dbiNew);

  ActPtab.rNr = 0;
  ActEtab.rNr = 0;
  DatEtab.rNr = 0;
  actEl.rNr = 0;
  ActFtab.rNr = 0;


  // add surf to surTab RecNr surTabNr
  gis_surTab_add (dbiNew);
  // ActSurPtb = surNr;

  // add surf to List
  gis_lst_sup ();

  TX_Print("new surface A%d initialized; add points ..",dbiNew);

  return 0;

}


//================================================================
  int gis_PtAdd () {
//================================================================
// add Point to surface ActSurPtb
// see also gisEdg_addGisPt

  int    i1;
  Point  pt1;


  // printf("gis_PtAdd %d %d\n",ActSurPtb,ActPtab.rNr);

/*
  if(ActSurPtb < 0) {
    TX_Print("***** create new pointTable.");
    gis_ptab_init ();
  }
*/

  // get point from UI
  i1 = gis_UI_getPt (&pt1);
  if(i1 < 0) return -1;


  ActPtNr = -1;    // create new point
  gis_addPtab (&ActPtNr, &pt1, 1);

  // update ActPtNr-display
  ActSelTyp = APPTYP_GISPT;
  gis_disp_actObj ();

  DL_Redraw ();

  TX_Print("point %d added..",ActPtNr);

  return 0;

}


//================================================================
  int gis_freePos_pTab () {
//================================================================
// return next free Position in pTab; update ActPtab.rNr
// creates new (empty) point in ActPtab if necessary.

  int    i1, i2;
  long   ld;


  // printf("FFFFFFFFFFFF gis_freePos_pTab %p %d\n",ActPtab.data,ActPtab.rNr);

  for(i1=0; i1<ActPtab.rNr; ++i1) {
    if(ActPtab.data[i1].x == UT_VAL_MAX) goto L_exit;
  }


  // no free pos; realloc.
  printf("gis_freePos_pTab %d\n",ActPtab.rNr);
  MemTab_check (&ActPtab, &ld, INC_Ptab);

  if(ActPtab.data == NULL) {TX_Error("gis_freePos_pTab EOM"); return -1;}

  // new points are unused
  for(i2=ActPtab.rNr; i2<ActPtab.rMax; ++i2) ActPtab.data[i2].x = UT_VAL_MAX;

  i1 = ActPtab.rNr;
  ActPtab.rNr += 1;


  L_exit:
    // printf("ex gis_freePos_pTab %d\n",i1);
  return i1;

}
 

//================================================================
  int gis_ptab_save (char *fnam, char *dirNam) {
//================================================================
// Export points into Ascii-file


  int   i1;
  char  cbuf[256];
  FILE  *fpo;

  if(fnam == NULL) return 0;


  // printf("gis_ptab_save |%s|%s|\n",dirNam,fnam);


  // // unload active surf
  // gis_bload_pTab (TYP_FuncEnd);
  // // load points > pTab
  // gis_bload_pTab (TYP_FuncInit);


  if(ActPtab.rNr < 1) {
    TX_Print(" pTab not active ..");
    return 0;
  }


  sprintf(cbuf,"%s%s",dirNam,fnam);
  if((fpo=fopen(cbuf,"w")) == NULL) {
    TX_Print("gis_ptab_save E001 %s",fnam);
    return -1;
  }


  for(i1=0; i1<ActPtab.rNr; ++i1) {
    // printf(" %d %f %f %f\n",i1,
            // ActPtab.data[i1].x,ActPtab.data[i1].y,ActPtab.data[i1].z);
    fprintf(fpo,"P%d %f %f %f\n",i1,
            ActPtab.data[i1].x,ActPtab.data[i1].y,ActPtab.data[i1].z);
  }

  fclose(fpo);

  return 0;

}


//================================================================
  int gis_sur_on (int surInd) {
//================================================================
  // activate surface surTab[surInd]

  int        irc;
  long       dli, dbi;
  char       s1[32];


  // printf("gis_sur_on surInd=%d act=%d\n",surInd,ActSurInd);
  // gis_dump_surTab ();

  // if no surf exists
  if(surInd < 0) {
    ActSurInd  = -1;
    ActSurPtb  = -1;
    ActSurMsh  = -1;
    ActPtNr    = -1;
    // gis_disp_actObj ();
    return 0;
  }
  

  // load pTab from bin.file for surf. ActSurPtb > ActPtab
  ActSurInd = surInd;
  ActSurPtb = surTab[surInd].indp;
  ActSurMsh = surTab[surInd].indm;
  // if(surTab[surInd].exi_p == 0) goto L_2;



  // irc = gis_bload_pTab (TYP_FuncInit); // Load ActPtab from file A<ActSurPtb>
  // irc = MSH_bload_pTab (&ActPtab, -1, ActSurPtb);
  irc = MSH_bload_pTab (&ActPtab, WC_modact, ActSurPtb);
  if(irc < 0) goto L_2;


  // display points
  gis_DrawPoints (ActPtab.data, ActPtab.rNr, ActSurPtb, 0);
  // surTab[surInd].visi = 1;


  // remove display of new pTab-boundary ActSurPtb
  if(surTab[surInd].exi_p != 0) {
    dli = DL_find_obj (Typ_SUR, ActSurPtb, -1L);
    if(dli >= 0) DL_unvis_set (dli, 1);  // set unvis=1
  }
  ActPtabBndModFlag = 0;


  //----------------------------------------------------------------
  // change display of new Mesh to symbolic
  dbi = surTab[surInd].indm;
  if(dbi < 1) goto L_2;
    // printf(" m-new-dbi=%ld\n",dbi);
    // printf(" dbi=%ld  dli=%ld\n",dbi,dli);

  // load fTab & eTab
  // MSH_bload_fTab (&ActFtab, &ActEtab, &DatEtab, -1, dbi);
  MSH_bload_fTab (&ActFtab, &ActEtab, &DatEtab, WC_modact, dbi);

  // if mesh already exists, just modify ..
  if(surTab[surInd].exi_m > 0) {
    // find dli of A<dbi>
    dli = DL_find_obj (Typ_SUR, dbi, -1L);
    if(dli < 0) goto L_2;
    GA_sStyl__ (dli, 2, Typ_SUR, dbi);    // change attrib & redraw


  } else {     // modify Attribut of temporary Element dli
    gis_msh_disp (1, surInd, dbi);
  }

  gis_changed_mshMod (0); // reset, unmodified.



  // display EdgeLines
  gis_DrawAllEL (&ActEtab);


  //----------------------------------------------------------------
  L_2:
  // activate actual surface in list
  // sprintf (s1, "A%d", ActSurMsh);
  sprintf (s1, "A%d", ActSurPtb);
  // GUI_OptMenActivate (&wls, surInd, s1);
  GUI_optmen_set (&wls, 1, s1);


  // display new activepointTab in Label
  ActSelTyp = -1;
  ActPtNr = -1;
  gis_disp_actObj ();

  gis_changed_visi ();  // lock prjMsh

  return 0;

}


//================================================================
  int gis_sur_off (int mode) {
//================================================================
// disactivate surface surTab[ActSurInd]
// mode    0=switch;
//         1=delete;
//         2=save   (exit app)

//   remove gisp-points, redraw pTab-outline

  long       dli, dbi;


  // printf("gis_sur_off mode=%d act=%d\n",mode,ActSurInd);
  // gis_dump_surTab ();


  if(ActSurInd < 0) return 0;

  // unload mesh; save if modified.
  // MSH_bsav_fTab (&ActFtab, &ActEtab, &DatEtab, -1, surTab[ActSurInd].indm);
  if((ActFtab.data)    &&
     (ActFtab.rNr > 0))    {
     MSH_bsav_fTab (&ActFtab, &ActEtab, &DatEtab,
                     WC_modact, surTab[ActSurInd].indm);
  } else {
     MSH_bdel_fTab (WC_modact, surTab[ActSurInd].indm);
  }





  // unload active surf-points
  // gis_bload_pTab (TYP_FuncEnd);  // save ActPtab to file A<ActSurPtb>
  if(ActPtab.data) {
    surTab[ActSurInd].ptNr = ActPtab.rNr;
    // MSH_bsav_pTab (&ActPtab, -1, surTab[ActSurInd].indp);
    MSH_bsav_pTab (&ActPtab, WC_modact, surTab[ActSurInd].indp);
    // ActPtab.rNr = 0;
    // ActPtabModFlag = 0;
  }
  

  //----------------------------------------------------------------
  // delete actEl & alle EdgeLines
  gis_Edges_all_del ();

/*
  // remove actEl
  GL_temp_delete ();
  // remove the Display of all EdgeLines
  gis_HideAllEL (&ActEtab);
  // delete Edglines
  ActEtab.rNr = 0;
  DatEtab.rNr = 0;
*/


  //----------------------------------------------------------------
  // delete display of all points
  gis_HidePoints (surTab[ActSurInd].indp);



  //----------------------------------------------------------------
  // update pTab-boundary;
  // if ptab is unchanged: make it visible again; else recreate it.
  gis_disp_pTab (ActSurInd);



  //----------------------------------------------------------------
  // change display of active mesh to shaded
  if(ActSurInd < 0) goto L_1;

  dbi = surTab[ActSurInd].indm;
  if(dbi < 1) goto L_1;
    // printf(" m-act-dbi=%ld\n",dbi);


  // GA_sStyl__ ruft APT_Draw__; geht ned wenn noch kein srcObj existiert !

  dli = DL_find_obj (Typ_SUR, dbi, -1L);
    // printf(" dli surMsh = %d\n",dli);


  // mode    0=switch; 1=delete; 2=save
  // see also gis_sur_on
  if(mode == 0) {            // 0=switch
    if(surTab[ActSurInd].exi_m > 0) { // did exist before
      if(dli >= 0) GA_sStyl__ (dli, 1, Typ_SUR, dbi);    // change attrib & redraw
    } else {
      gis_msh_disp (0, ActSurInd, dbi);
    }

  } else if(mode == 1) {     // 1=delete
    if(dli >= 0) GL_Del0 (dli);


  } else if(mode == 2) {     // 2=save
    if(surTab[ActSurInd].exi_m > 0) { // did exist before
      if(dli >= 0) GA_sStyl__ (dli, 1, Typ_SUR, dbi); // change attrib & redraw

    } else {                           // did not exist before
      if(dli >= 0) GL_Del0 (dli);
    }
  }


  // } else {   // modify Attribut of temporary Element dli
    // gis_msh_disp (0, ActSurInd, dbi);   // load & display mesh




  //----------------------------------------------------------------
  L_1:
  ActSurInd = -1;
  return 0;

}


//================================================================
  int gis_ptab_load (char *fnam, char *dirNam) {
//================================================================
// user has pointfile selected;
// see also AP_ImportXML

  int       irc, i1, ptNr;
  long      surNr;
  char      cbuf[256], attrib[2];
  // Point     *pTab;


  if(fnam == NULL) return 0;

  strcpy(cbuf, dirNam);
  strcat(cbuf, fnam);


    // printf("gis_ptab_load |%s|\n",cbuf);

  TX_Print("importing %s",cbuf);

  // gis_ptab_init ();


  // load points > pTab
  // irc = ptab_file_load1 (pTab, &ptNr, 0, cbuf);
  irc = ptab_file_load1 (&ActPtab, 0, cbuf);
  if(irc < 0) return -1;


  // create point ActPtab.rNr from Display
  // hier fehlt noch PunktIndexOffset !
  gis_DrawPoints (ActPtab.data, ActPtab.rNr, ActSurPtb, 0);
  // surTab[ActSurInd].visi = 1;


  // update ActPtNr-display
  ActSelTyp = -1;
  ActPtNr = -1;
  gis_disp_actObj ();

  gis_changed_pTab ();

  TX_Print("%d points imported ..",ActPtab.rNr);

  return 0;

}


//================================================================
  int gis_ptab_move (Vector *vc1, Point *pTab,  int ptNr) {
//================================================================
// gis_ptab_move        move all points

  int   i1;

  UT3D_stru_dump (Typ_VC, vc1, "gis_ptab_move");

  for(i1=0; i1<ptNr; ++i1) {
    UT3D_pt_traptvc (&pTab[i1], &pTab[i1], vc1);
  }

  return 0;

}


//=======================================================================
  int gis_DrawPoints (Point *pTab,  int ptNr, int surNr, int iOff) {
//=======================================================================
// Input:
//   iOff  the nr of the first point
// DispList:
//   typ   ApplicationObject (Typ_APPOBJ = 20)
//   indp   SurfNr (A#)
//   iatt  pointNr (index in pTab)

// Wie soll Bezug auf die Application eingebracht werden ?
// Gis hat viele ApplicationObjectTypes ..

// see APT_disp_SymB GL_DrawSymB
// see APT_disp_TxtA GR_DrawTxtA GL_DrawTxtA

  int    i1, icol1, icol2;
  long   l1;
  char   info[20];


  // printf("gis_DrawPoints ptNr=%d surNr=%d iOff=%d\n",ptNr,surNr,iOff);

  icol1 = 0;
  icol2 = 1;

  for(i1=0; i1<ptNr; ++i1) {
    sprintf(info, "%d", i1+iOff);   // itoa
    // save objtyp APPTYP_GISPT as att
    // save pointNr as ind
    // l1 = DL_StoreObj (Typ_APPOBJ, surNr, i1+iOff);
    l1 = DL_StoreObj (Typ_APPOBJ, i1+iOff, APPTYP_GISPT);
    gis_DrawPoint (&l1, SYM_TRI_S, &pTab[i1], icol1, icol2, info);
  }
  ActPtDli = l1;

  return 0;

}



//================================================================
  int gis_DrawPoint (long *ind, int symTyp, Point *pt1,
                     int icol1, int icol2, char *info) {
//================================================================

  GLuint dlInd;


  // printf("gis_DrawPoint %d %d\n",*ind,symTyp);


   // den DL-Index (+ Offset) holen)
   dlInd = GL_fix_DL_ind (ind);
     // printf(" dlInd=%d\n",dlInd);
  
   glNewList (dlInd, GL_COMPILE);
  
   glDisable (GL_LIGHTING);
  
   // nur fuer das Rescale mit Feedback erforderlich:
   glBegin (GL_POINTS);
   glVertex3dv ((double*)pt1);
   glEnd ();

   // display symbol symTyp
   glColor3fv    (GL_col_tab[icol1]);
   glRasterPos3d (pt1->x, pt1->y, pt1->z); // notwendig zum activate color!
   glCallList ((GLuint)symTyp);

   // display info
   glColor3fv    (GL_col_tab[icol2]);
   glRasterPos3d (pt1->x, pt1->y, pt1->z);
   glCallLists (strlen(info), GL_UNSIGNED_BYTE, (GLubyte*)info);


   glEnable (GL_LIGHTING);

   glEndList ();

     // printf("ex gis_DrawPoint dli=%d\n",dlInd);
 
  return 0;

}

 
//================================================================
  int gis_HidePoints (int surNr) {
//================================================================
// delete display of GIS-points
// surNr unused; deletes all application-objects APPTYP_GISPT


  // printf("gis_HidePoints sur=%d\n",surNr);

  // GL_Del_om (Typ_APPOBJ, surNr, -1);   // del multiple objects from DL
  GL_Del_om (Typ_APPOBJ, -1L, APPTYP_GISPT);  // del multiple objects from DL
  GL_Regen1 ();                        // reset DL
  

  return 0;

}


//================================================================
  int gis_DrawAllEL (MemTab(EdgeLine) *eTab) {
//================================================================

  int    i1;

  // printf("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE \n");
  // printf("gis_DrawAllEL %d\n",eTab->rNr);

  for(i1=0; i1<eTab->rNr; ++i1) {
    if(eTab->data[i1].typ == TYP_EDGLN_AB) continue; // skip automatic bound
    gis_DrawEL (&eTab->data[i1], i1);
  }
  DL_Redraw ();


  return 0;

}


//================================================================
  int gis_DrawEL (EdgeLine *el1, int ind) {
//================================================================
// display EdgeLine
// see gis_DrawPoint gis_EL_disp

  int    iAtt, iClo;
  long   l1;
  GLuint dlInd;

  static unsigned char atta[] = {9, 9, 5, 8, 9};  // see GR_Disp_ln2
                             //  0  1  2  3  4
                             //        BL IB OB

  // printf("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE \n");
  // printf("gis_DrawEL %d\n",ind);

  
  // iAtt = 9;
  iAtt = atta[el1->typ];


  if(el1->typ > 2) iClo = 1; // 0=open; 1=closed
  else             iClo = 0;


  // test if this breakline already exists ..
  // yes: overwrite; else create ..
  l1 = DL_find_APPOBJ (ind, APPTYP_EDGLN);
  if(l1 >= 0)
  DL_SetInd (l1);       // modify (do not create new DL-Record)
  l1 = DL_StoreObj (Typ_APPOBJ, ind, APPTYP_EDGLN);
    // printf(" l1=%ld\n",l1);

  gis_EL_disp (l1, el1->ia, el1->iNr, ActPtab.data, iAtt, iClo);

  return 0;

}


//================================================================
  int gis_HideAllEL (MemTab(EdgeLine) *eTab) {
//================================================================
// eTab unused;

  int   i1;


  // printf("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE \n");
  // printf("gis_HideAllEL \n");

  gis_HideEL (-1L);

  // for(i1=0; i1<eTab->rNr; ++i1) {
    // gis_HideEL (eTab->data[i1]);
  // }

  return 0;

}


//================================================================
  int gis_HideEL (long  i1) {
//================================================================

  // printf("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE \n");
  // printf("gis_HideEL \n");

  // GL_Del_om (Typ_APPOBJ, surNr, -1);   // del multiple objects from DL
  GL_Del_om (Typ_APPOBJ, i1, APPTYP_EDGLN);  // del multiple objects from DL
  GL_Regen1 ();                        // reset DL

  return 0;

}


//=========================================================
  int gCad_fini () {
//=========================================================
// dll being unloaded - reset Input, kill all open windows

  int    i1, ii;
  long   dbi, dli;


  printf("gCad_fini GIS1\n");
  GUI_obj_dump_mo (&tbGis1);

  // return 0;


  // do nothing, if already unloaded.
  if(!GUI_OBJ_IS_VALID(&tbGis1)) return 0;

  // gis_dump_surTab ();


  // write to Main-Infowindow ..
  TX_Print("...... gCad_fini GIS1");




    //----------------------------------------------------------------
    // disactivate surface surTab[ActSurInd]
    gis_sur_off (2);



    // create update / Modelcodes
    // nur Modelcodes korrigieren; displayKorrektur done by gis_sur_off
    gis_mdl_upd ();



    //----------------------------------------------------------------
    // free memory:
    // free memSpace for active EdgeLine
    MemTab_free (&actEl);

    // free memSpace for Edgelines
    MemTab_free (&ActEtab);

    // free EdgeLine-dataSpace
    MemTab_free (&DatEtab);

    MemTab_free (&ActFtab);
    MemTab_free (&ActPtab);


    // remove the toolbar tbGis1
    // GUI_obj_del (tbGis1);
    GUI_set_show (&tbGis1, 0);    // hide
    // GUI_update__ ();
    // UI_GR_Redraw ();  // erforderlich wenn beim esc cursor nicht im grafWin !
    // GUI_update__ ();



  // reset  select (give back to mainApp)
  AP_User_reset ();    // 2010-11-10
  // APP_Close ();

    // printf("ex gCad_fini \n");


  ED_work_CurSet (UT_INT_MAX);   // work until end of model    2010-11-29

  UI_block__ (0, 0, 0);

  // beim Shutdown gibts hier ein Problem !
  // DL_Redraw ();     // sometimes necessary to remove toolbar
  DL_hili_off (-1L); DL_Redraw ();


  return 0;

}


//================================================================
  int gis_mdl_del_msh (int surInd) {
//================================================================
// delete mesh-codeline in model

  int      irc, ll, surNr;
  long     lNr;
  char     *lPos, cbuf[64];


  // printf("gis_mdl_del_msh %d\n",surInd);

 
  // no mesh to delete - did not exist before
  if(surTab[surInd].exi_m == 0) return 0;

  // no mesh created
  if(surTab[surInd].indm < 0) return 0;

  surNr = surTab[surInd].indm;

    APED_oid_dbo__ (cbuf, Typ_SUR, surNr);
    // search defineition-line
    irc = APED_search_defLn (&lPos, &lNr, &ll, cbuf, -1, 0);
      // printf(" irc=%d lNr=%d |%s|\n",irc,lNr,cbuf);
    if(irc < 0) goto L_cr;

    // delete line in model
    // APED_undo_do (lNr, lPos, 1);
    // APED_update__ (lNr);
    UNDO_grp_add (lNr, 0);        // add to undoTab
    UNDO_grp_del ();                 // delete last active grp & update display


    // delete meshfile    sonst geht das UNDO ned ..
    // MSH_bdel_fTab (WC_modnr, surTab[surInd].indm);

  L_cr:

  return 0;

}


//================================================================
  int gis_mdl_del_pTab (int surInd) {
//================================================================
// delete pTab from model (del A<surInd> - Line)

  int       irc, ll, surNr;
  long      lNr;
  char      *lPos, cbuf[64];

  
  // printf("gis_mdl_del_pTab %d\n",surInd);


  // no mesh to delete - did not exist before
  if(surTab[surInd].exi_p == 0) return 0;

  // points deleted ..
  if(surTab[surInd].indp < 0) return 0;

    surNr = surTab[surInd].indp;

    APED_oid_dbo__ (cbuf, Typ_SUR, surNr);
    // search defineition-line
    irc = APED_search_defLn (&lPos, &lNr, &ll, cbuf, -1, 0);
      // printf(" irc=%d lNr=%d |%s|\n",irc,lNr,cbuf);
    if(irc < 0) goto L_cr;

    // delete line in model
    UNDO_grp_add (lNr, 0);        // add to undoTab
    UNDO_grp_del ();                 // delete last active grp & update display

    // delete bin. pointFile (wird sonst wieder ins Model gespeichert)
    // cannot redo if deleted ..
    // MSH_bdel_pTab (WC_modnr, surTab[surInd].indp);

  L_cr:

  return 0;

}


//================================================================
  int gis_mdl_add_ptab (int iSur) {
//================================================================

  char      cbuf[80];


  // printf("gis_mdl_add_ptab %d\n",iSur);

  sprintf(cbuf, "A%d=PTAB",surTab[iSur].indp);
    // printf("gis_mdl_add_ptab |%s|\n",cbuf);
  ED_srcLn_add (cbuf, 0);

  surTab[iSur].exi_p = 1;

  return 0;

}


//================================================================
  int gis_mdl_upd () {
//================================================================
// create update / Modelcodes

  int       i1;
  long      dli;
  char      cbuf[80];


  // printf("gis_mdl_upd %d\n",surTabNr);
    // gis_dump_surTab ();


  for(i1=0; i1<surTabNr;  ++i1) {


    //----------------------------------------------------------------
    // remove the display of pTab-Surface that did not exist before
    if(surTab[i1].exi_p == 0) {    // did not exist before
      dli = DL_find_obj (Typ_SUR, surTab[i1].indp, -1L);
      if(dli >= 0) GL_Del0 (dli);
    }


    //----------------------------------------------------------------
    // update pTab-Surface
    // add A1=PTAB "pointfilename" to model;
    // pointfilename is unused !
    // sprintf(cbuf, "A%d=PTAB \"%s\"",surNr,fnam);
    if(surTab[i1].exi_p == 0) {    // did not exist before
      if(surTab[i1].indp >= 0) {
        if(surTab[i1].ptNr > 3) {
          gis_mdl_add_ptab (i1);  // create "A#=PTAB"
        }
      }
    }


    //----------------------------------------------------------------
    // remove the display of Mesh-surface that did not exist before
    if(surTab[i1].exi_m == 0) {    // did not exist before
      dli = DL_find_obj (Typ_SUR, surTab[i1].indm, -1L);
      if(dli >= 0) GL_Del0 (dli);
    }


    //----------------------------------------------------------------
    // update Mesh-surface
    // add A1=MSH A<pointSurfNr>   to model;
    if(surTab[i1].exi_p == 0) goto L_exists; // no pTab -no Mesh
    if(surTab[i1].exi_m != 0) goto L_exists;
    // did not exist before

      if(surTab[i1].indm > -1) {
        if(surTab[i1].visi != 0) {
          sprintf(cbuf, "A%d=MSH A%d",surTab[i1].indm,surTab[i1].indp);
            // printf(" add |%s|\n",cbuf);
          ED_srcLn_add (cbuf, 0);
        }
      }
      goto L_ex_msh;


    //----------------------------------------------------------------
    // mesh did exist; check for delete
    L_exists:
      if(surTab[i1].visi == 0) {     // not visible = deleted !
        // delete Mesh from model (deletes also its display)
        gis_mdl_del_msh (i1);
      }

 
    L_ex_msh:
    continue;

  }


  return 0;

}


//====================================================================
  int ptab_cad_load1 (Point *pTab, int *ptNr, Edg3 *lTab, int *lnNr) {
//====================================================================
// get points from DB &
// get breaklines from DB


  int      i1, ip1, ip2, ptMaxNr, lnMaxNr;
  long     dbi, dbMax;
  double   tol;
  Point    *pt1;
  Line     *ln1;

  // printf("ptab_cad_load1 %d %d\n",*ptNr,*lnNr);

  tol = UT_DISP_cv;
  ptMaxNr = *ptNr;
  lnMaxNr = *lnNr;

  // pTab ex DB-points laden.
  dbMax = DB_dbo_get_free (Typ_PT);
    // printf(" PT-dbMax=%ld\n",dbMax);

  // Punkte ex DB holen
  *ptNr = 0;
  for(dbi=1; dbi<=dbMax; ++dbi) {
    if(DB_QueryDef(Typ_PT, dbi) < 0) continue;
    pt1 = DB_get_PT (dbi);
    ip1 = ptab_ptAdd (pt1, pTab, ptNr, ptMaxNr, tol);
    if(ip1 < 0) break;
  }



  // Linien (breaklines) ex Db holen ...
  // neue Punkt an pTab anhaengen ..
  // ptMaxNr = *ptNr;
  dbMax = DB_dbo_get_free (Typ_LN);
    // printf(" LN-dbMax=%ld\n",dbMax);



  // Lines ex DB holen
  i1 = 0;
  for(dbi=1; dbi<=dbMax; ++dbi) {
    if(DB_QueryDef(Typ_LN, dbi) < 0) continue;
    ln1 = DB_get_LN (dbi);
    // in pTab suchen, ob points already defined; ip1, ip2 setzen.
    // not yet defined: add to pTab.
    ip1 = ptab_ptAdd (&ln1->p1, pTab, ptNr, ptMaxNr, tol);
    ip2 = ptab_ptAdd (&ln1->p2, pTab, ptNr, ptMaxNr, tol);
    if(ip2 < 0) break;
    // add breakline
    if(i1 >= lnMaxNr) {TX_Print("ptab_cad_load1 E002"); break;}
    lTab[i1].i1 = ip1;
    lTab[i1].i2 = ip2;
    ++i1;
  }
  *lnNr = i1;


/*
  // Test only ...........
    for(i1=0;i1<*ptNr;++i1) printf("p[%d]=%f %f %f\n",i1,
      pTab[i1].x,pTab[i1].y,pTab[i1].z);
    for(i1=0;i1<*lnNr;++i1) printf("l[%d]=%d %d\n",i1,
      lTab[i1].i1,lTab[i1].i2);
*/

  return 0;

}


//==============================================================================
  int ptab_ptAdd (Point *pt1, Point *pTab, int *ptNr, int ptMaxNr, double tol) {
//==============================================================================
// check if pt1 is already in pTab; return its index,
// if not: add pt1 to pTab, return its index.
// RetCod = pointIndex or -1 (pTab.-Overfolw).
// used by ptab_cad_load1

  int   i1;

  // UT3D_stru_dump(Typ_PT, pt1, "ptab_ptAdd:");
  // printf(" ptNr=%d\n",*ptNr);

  // check if pt1 is already in pTab
  for(i1=0; i1<*ptNr; ++i1) {
    if(UT3D_comp2pt(pt1, &pTab[i1], tol) == 0) continue;
    return i1;   // gefunden ..
  }

  // add pt1 to pTab.
  i1 = *ptNr;
  if(i1 >= ptMaxNr) {TX_Print("ptab_ptAdd E001"); return -1;}
  pTab[i1] = *pt1;
  ++(*ptNr);


    GR_Disp_pt (&pTab[i1], SYM_TRI_S, 0);
    GR_Disp_txi (&pTab[i1], i1, 0);

  
  // printf("ex ptab_ptAdd ptNr=%d\n",*ptNr);
  
  return i1;

}





//====================================================================
  int ptab_file_load1 (MemTab(Point) *pTab, int mode, char *fNam) {
//====================================================================
// Punkte aus einer Ascii-Datei nach pTab einlesen.
// mode beschreibt das Dateiformat.
// Input:
//   ptNr   max nr of points on input !
//   mode   = Filetyp; 0 = N X Y Z C; 1 = X Y Z N C   UNUSED
//   fNam   incl. Filetyp

// Es wird getestet, ob erstes Wort numerisch.

// DefaultFileFormat:
// N=PointName, Text, zB "P001"; X,Y,Z=Coords; C=PointCode, Text, zB "Haus"
 
  int   irc, i1, i2, ix, iy, iz, lNr; //, ptMaxNr;
  long  ld;
  FILE  *fpi;
  char  cBuf[256], *cTab[6], *cp, *p1, cWd[80];
  Point pt1;


  // printf("ptab_file_load1 |%s|\n",fNam);

  // ptMaxNr = *ptNr;


  if ((fpi = fopen (fNam, "r")) == NULL) {
    TX_Print("FILE NOT FOUND |%s| in ptab_file_load1",fNam);
    return -1;
  }


  // fix the correlation-pointers
  // read first line & test first word.
  fgets (cBuf, 250, fpi);
/*
  cp = UTX_pos_skipDeli1 (cBuf); // skip leading blanks, tabs ..
  if(UTX_ck_num_f(&p1, cp) == 0) {    // yes numer.
    ix = 0;
    iy = 1;
    iz = 2;
  } else {
    ix = 1;
    iy = 2;
    iz = 3;
  }
*/
  p1 = cBuf;
  i1 = -1;


  // search 1. word containing "."
  // - this is position of X-coord; Y-Coord and Z-Coord follows.

  // get next word
  L_nxt_wd:
  ++i1;
  cp = UTX_cp_word__ (cWd, p1);
    // printf(" wd[%d]=|%s|%s|\n",i1,cWd,cp);
  if(strchr(cWd, '.') == NULL) {
    p1 = UTX_pos_skipDeli1 (cp);
    if(i1 < 3) goto L_nxt_wd;
    TX_Print("FileFormat-Error %s",fNam);
    return -1;
  }


  // from extern:
  ix = i1;
  iy = ix + 1;
  iz = iy + 1;
    // printf(" positions %d %d %d\n",ix,iy,iz);


  rewind(fpi);


  lNr = 0;
  while (!feof (fpi)) {
    if(fgets (cBuf, 250, fpi) == NULL) break;
    UTX_CleanCR (cBuf);    // cut off CR,LF 
      // printf("%s\n",cBuf);
    cp = cBuf;
    while (*cp == ' ') ++cp; // skip leading blanks
    if(strlen(cp) < 6) continue;
    // separate words ..
    cTab[0] = &cBuf[0];
    i2 = 0;

    // find starting-positions of first 6 words
    for(i1=0; i1<5; ++i1) {
      ++i2;
      // skip leading blanks
      cp = cTab[i1];
      // skip leading blanks
      while (*cp == ' ') ++cp;
      cTab[i2] = UTX_find_Del1 (cp);
      if(cTab[i2]) {*cTab[i2] = '\0'; ++cTab[i2];}   // separate with '\0'
    }
      // printf("|%s|%s|%s|%s|%s|\n",cTab[0],cTab[1],cTab[2],cTab[3],cTab[4]);

    // distribute; load Coords according to ix,iy,iz;
    pt1.x = strtod (cTab[ix], &cp);
    pt1.y = strtod (cTab[iy], &cp);
    pt1.z = strtod (cTab[iz], &cp);

    // test if point already exists; (2D-Test; tolerance = TOL_GIS_PT)
    if(MSH_pt_ck_pTab(ActPtab.data, ActPtab.rNr, &pt1, TOL_GIS_PT) < 0) {
      // 0=newPoint .. save point ..
      irc = MemTab_sav (pTab, &ld, &pt1, 1);
      if(irc < 0) goto L_EOM;
        // UT3D_stru_dump (Typ_PT, &pTab[lNr], "%s:",cTab[0]);
        // GR_Disp_pt (&pTab[lNr], SYM_TRI_S, 0);
        // GR_Disp_txi (&pTab[lNr], lNr, 0);

    }

    // proceed to next line ...
    ++lNr;
    // if(lNr > ptMaxNr) {
      // TX_Error("ptab_file_load1 EOM %d",ptMaxNr);
      // return -1;
    // }
  }

  fclose(fpi);

  // *ptNr = lNr;
    // printf("ex ptab_file_load1 points=%d\n",lNr);
  return 0;


  L_EOM:
    printf("ptab_file_load1 EOM\n");
    return -1;
}


//================================================================
  int gis_Edges_all_del () {
//================================================================
// delete actEl & alle EdgeLines


  // remove display of actEl
  GL_temp_delete ();
  actEl.rNr = 0;
  gis_changed_actElNr ();



  // remove the Display of all EdgeLines
  gis_HideAllEL (&ActEtab);

  // delete Edglines
  ActEtab.rNr = 0;
  DatEtab.rNr = 0;

  return 0;

}

 
//================================================================
  int gis_Edges_del (int elInd) {
//================================================================
// delete EdgeLine elInd

  // int iSta, iNr;


  // printf("gis_Edges_del %d\n",elInd);

  // remove display
  // gis_HideEL (elInd);
  gis_HideAllEL (&ActEtab); // remove the Display of all EdgeLines

  MSH_eTab_delRec (&ActEtab, &DatEtab, elInd);

  // draw all (remaining) Edgelines
  gis_DrawAllEL (&ActEtab);

  // ActFtabModFlag = 1;
  gis_changed_mshMod (1); // modified; need update.

  return 0;

}
 

//================================================================
  int gis_Edges_add (int *iTab, int iNr, int typ, int dispMode) {
//================================================================
// add polygon iTab[iNr] to ActEtab/DatEtab
// dispMode         0=update display, 1=do not update display

  int      ind;
  long     l1;
  EdgeLine el1;


  // printf("gis_Edges_add iNr=%d typ=%d\n",iNr,typ);


  // add one more EdgeLine in ActEtab
  ind = ActEtab.rNr;
  el1.iNr = iNr;
  el1.typ = typ;
  MemTab_sav (&ActEtab, &l1, &el1, 1);
  if(ActEtab.data == NULL) goto L_EOM;


  // add/copy data; actEl.iNr ints in actEl.ia.
  MemTab_sav (&DatEtab, &l1, iTab, iNr);
  if(DatEtab.data == NULL) goto L_EOM;

  // die pointers ActEtab[].ia neu aufbauen
  MSH_eTab_fix (&ActEtab, &DatEtab);

  // display EdgeLine
  if(dispMode == 0) gis_DrawEL (&ActEtab.data[ind], ind);

  // modified
  // ActFtabModFlag = 1;
  gis_changed_mshMod (1); // modified; need update.

    // MSH_dump_eTab (&ActEtab, &DatEtab);

  return 0;

  L_EOM:
  TX_Print("gis_Edges_add EOM"); return -1;

}


//================================================================
  int gis_Edges_upd (int mode) {
//================================================================
// init/add/update/delete ActEdges

  int      irc, i1, i2;
  long     l1;
  double   d1;
  EdgeLine *el1;


  //----------------------------------------------------------------
  // Init: create empty Record.
  if(mode == TYP_FuncInit) {
    // printf(" gis_Edges_upd TYP_FuncInit\n");

    // get memSpace for EdgeLine-records
    ActEtab.rNr = 0;     // reset

    // get memSpace for EdgeLine-dataSpace
    DatEtab.rNr = 0;     // reset


    // ActEdges.eTab = NULL;
    // ActEdges.eNr  = 0;
    // ActEdges.eTyp = NULL;
    return 0;
  }



  //----------------------------------------------------------------
  // Add: add EdgeLine actEl to ActEdges
  if(mode != TYP_FuncAdd) goto L_Del;

  // test if actEl is OK ..
    // gis_dump_actEl ();
  if(actEl.rNr < 2) {
    TX_Print("***** active Poygon is not defined *****");
    return -1;
  }

  // remove closing-point from boundaries
  if((actEl.use == TYP_EDGLN_IB)||(actEl.use == TYP_EDGLN_OB)) {
    i1 = actEl.data[0];
    i2 = actEl.data[actEl.rNr - 1];
    if(i1 == i2) actEl.rNr -= 1;
  }

  // check if actEl is CCW; else invert.
  irc = UT2D_srar_inpt3 (&d1, actEl.rNr, actEl.data, ActPtab.data);
  if(irc < 0) MEM_inv_itab (actEl.rNr, actEl.data);

  // add actEl to ActEtab
  irc = gis_Edges_add (actEl.data, actEl.rNr, actEl.use, 0);

  // clear actEl, remove display; redraw.
  gis_actEl_del ();

  return irc;


  //----------------------------------------------------------------
  // Delete:  remove EdgeLine ? from ActEdges
  L_Del:
  if(mode != TYP_FuncDel) {TX_Print("gis_Edges_upd E001"); return -1;}
  TX_Print("gis_Edges_upd E002");
  return -1;

}
 
/*
//================================================================
  int gis_DatEtab_sum (int recNr) {
//================================================================
// recNr 0-ActEtab.rNr  returns startPos of datavalues
//                        of ActEtab-Record recNr in DatEtab
// recNr   -1           returns size of DatEtab

  int    i1, ii;

  // get ii = actual used size of DatEtab
  if((recNr < 0)||(recNr > ActEtab.rNr)) recNr = ActEtab.rNr;

  ii = 0;

  for(i1=0; i1<recNr; ++i1) ii += ActEtab.data[i1].iNr;
    printf("ex gis_DatEtab_sum %d\n",ii);

  return ii;

}
*/
  // if(l1 != 0) for(..) ActEtab[i1].ia = DatEtab[i2];
/* MSH_eTab_fix
//================================================================
  int gis_DatEtab_fix () {
//================================================================
// (nach relocation) die pointers neu aufbauen
// ersetzen durch MSH_eTab_fix

  int    i1, ii;

  // get ii = actual used size of DatEtab
  ii = 0;
  for(i1=0; i1<ActEtab.rNr; ++i1) {
    ActEtab.data[i1].ia = &DatEtab.data[ii];
      printf(" _fix ActEtab[%d] = %d\n",i1,ii);
    ii += ActEtab.data[i1].iNr;
  }

  return 0;

}
*/


//================================================================
  int gis_surTab_add (long indp) {
//================================================================
// add new created pointSurf to surTab

  if(surTabNr >= MAX_SURPTAB) return -1;

  // printf("gis_surTab_add %d\n",indp);

  surTab[surTabNr].indp  = indp;
  surTab[surTabNr].indm  = -1,       // does not yet exist
  surTab[surTabNr].visi  = 0;        // no mesh yet
  surTab[surTabNr].exi_p = 0;        // new created
  surTab[surTabNr].exi_m = 0;        // new created

  ++surTabNr;

  return 0;

}
 

//================================================================
  int gis_dump_pTab () {
//================================================================
 
  int  i1;

  printf("========== gis_dump_pTab %d ===========\n",surTabNr);

  for(i1=0; i1<ActPtab.rNr;  ++i1) {
    UT3D_stru_dump (Typ_PT, &ActPtab.data[i1], "P[%d]",i1);
  }

  return 0;

}

//================================================================
  int gis_dumpHtm_pTab () {
//================================================================

  int      i1;
  char     s1[240];
  FILE     *fpo;

  i1 = APP_htm_fop (&fpo);
  if(i1 < 0) return -1;

  for(i1=0; i1<ActPtab.rNr;  ++i1) {
    sprintf(s1, "P[%d] = %f %f %f\n", i1, 
      ActPtab.data[i1].x,
      ActPtab.data[i1].y,
      ActPtab.data[i1].z);
    APP_htm_print (fpo, s1);
  }

  UTX_htm_fcl (&fpo);
  APP_browse (NULL);

  return 0;

}


//================================================================
  int gis_dumpHtm_EL () {
//================================================================

  int      i1, i2, iNr, *ia;
  FILE     *fpo;
  char     s1[240];
  EdgeLine *el1;


  i1 = APP_htm_fop (&fpo);
  if(i1 < 0) return -1;

  for(i1=0; i1<ActEtab.rNr; ++i1) {
    el1 = &ActEtab.data[i1];
    sprintf(s1, "Breakline[%d] Typ=%d\n",i1,el1->typ);
    APP_htm_print (fpo, s1);
    ia  = el1->ia;
    iNr = el1->iNr;
    for(i2=0; i2<iNr; ++i2) {
      sprintf (s1, "  %d",ia[i2]);
      APP_htm_print (fpo, s1);
    }
    APP_htm_print (fpo, "\n");
  }

  UTX_htm_fcl (&fpo);
  APP_browse (NULL);


  return 0;

}


//================================================================
  int gis_dump_actEl () {
//================================================================

  int  i1;

  UT3D_stru_dump (Typ_MemTab, &actEl, "actEl:");
  for(i1=0; i1<actEl.rNr; ++i1) {
    printf("  actEl[%d]=%d\n",i1,actEl.data[i1]);
  }

  return 0;

}

 
//================================================================
  int gis_dump_surTab () {
//================================================================

  int  i1;

  printf("========== gis_dump_surTab %d ===========\n",surTabNr);
  printf(" active PTAB=%d MSH=%d PtabMod=%d FtabMod=%d\n",
         ActSurPtb, ActSurMsh,
         ActPtabModFlag,ActFtabModFlag);


  for(i1=0; i1<surTabNr;  ++i1) {
    printf(" surTab[%d].indp=%d indm=%d vis=%d exp=%d exm=%d\n",i1,
             surTab[i1].indp,
             surTab[i1].indm,
             surTab[i1].visi,
             surTab[i1].exi_p,
             surTab[i1].exi_m);

  }


  printf("actEl:\n");
  gis_dump_actEl ();
  printf("ActPtab:\n");
  UT3D_stru_dump (Typ_MemTab, &ActPtab, "ActPtab:");
  printf("ActFtab:\n");
  UT3D_stru_dump (Typ_MemTab, &ActFtab, "ActFtab:");
  printf("ActEtab:\n");
  MSH_dump_eTab (&ActEtab, &DatEtab);


  return 0;

}


//================================================================
  int gisEdg_addPtab (int *iTab, int pNr) {
//================================================================
// add polygon iTab to active EdgeLine actEl
// see gisEdg_addGisPt

  int   irc, i1, i1s, i1e, i2s, i2e;
  long  l1;


  // printf("gisEdg_addPtab %d\n",pNr);
  // gis_dump_actEl ();       // dump actEl
  // for(i1=0;i1<pNr;++i1)printf(" iTab[%d] = %d\n",i1,iTab[i1]);
  

  if(actEl.rNr < 1) goto L_add;

  // try to connect actEl and iTab; 
  i1s = actEl.data[0];
  i1e = actEl.data[actEl.rNr - 1];

  i2s = iTab[0];
  i2e = iTab[pNr - 1];
    // printf(" i1s=%d i1e=%d i2s=%d i2e\n",i1s,i1e,i2s,i2e);



  // test if last point of actEl == 1. point of iTab;
  // if yes: remove last point of actEl.
  if(i1e == i2s) {
    actEl.rNr -= 1;
    goto L_add;
  }


  // test if last point of actEl == last point of iTab;
  // if yes: remove last point of iTab; invert itab;
  if(i1e == i2e) {
    pNr -= 1;
    MEM_inv_itab (pNr, iTab);
    goto L_add;
  }


  // test if 1 point of actEl == 1. point of iTab;
  // if yes: invert actEl; remove last point of actEl.
  if(i1s == i2s) {
    MEM_inv_itab (actEl.rNr, actEl.data);
    actEl.rNr -= 1;
    goto L_add;
  }


  // test if 1 point of actEl == last point of iTab;
  // if yes: invert actEl; remove last point of actEl; invert itab.
  if(i1s == i2e) {
    MEM_inv_itab (actEl.rNr, actEl.data);
    pNr -= 1;
    MEM_inv_itab (pNr, iTab);
    goto L_add;
  }




  //----------------------------------------------------------------
  // add iTab to actEl
  L_add:
  // check free space (realloc if necessary) & add index
  // UME_alloc_add (&actEl.ia, &l1, &actElSiz, &actEl.iNr,
                 // sizeof(int), &ActPtNr, 1, INC_actEl);
  for(i1=0; i1<pNr; ++i1) {
    ActPtNr = iTab[i1];
    irc = gisEdg_addGisPt (1);
    if(irc < 0) return irc;
  }


  // temporary preview of EdgeLine
  gis_actEl_disp ();
  DL_Redraw ();

    // printf("ex gisEdg_addPtab\n");

  return 0;

}

 
//================================================================
  int gisEdg_init () {
//================================================================
// start definition of edgeline


  // printf("gisEdg_init \n");

  // clear existing edgeline
  actEl.rNr = 0;

  gis_changed_actElNr ();


  // // clear display
  // GL_temp_delete ();
  // DL_Redraw ();


  return 0;

}


//================================================================
  int gisEdg_addGisPt (int mode) {
//================================================================
// add index to point ActPtNr to active EdgeLine;
// mode    0=displayactEl;  1=only update actEl; do not display.
// see also gis_PtAdd gisEdg_addPtab 

  long l1;
  int  irc, i1, *ip1, *ip2, ip3, ip4;


  // printf("gisEdg_addGisPt %d %d\n",ActPtNr,actEl.rNr);


  // test if last edgepoint is same point
  if(actEl.rNr > 0) {
    if(actEl.data[actEl.rNr - 1] == ActPtNr) {
      TX_Print("***** point already stored..");
      return 0;
    }
  }


  // test if other point lies on this edge
  ip1 = &ActPtNr;
  ip2 = &actEl.data[actEl.rNr - 1];
  if(actEl.rNr > 0) {
    irc = gisEdg_ck_pt__ (ip1, ip2);
    if(irc >= 0) {
      TX_Print("***** ERROR: point %d lies on edge %d-%d *****",
               irc, *ip1, *ip2);
      return -1;
    }
  }


  // test if edge intersects other edges
  if(actEl.rNr > 0) {
    irc = MSH_EL_ck_EL__ (&ip3, &ip4, ip1, ip2, &ActEtab, ActPtab.data);
    if(irc >= 0) {
      TX_Print("***** ERROR: edges %d-%d / %d-%d intersect *****",
               *ip1, *ip2, ip3, ip4);
      return -1;
    }
  }


  // test if actEl selfintersects
  if(actEl.rNr > 2) {
    for(i1=1; i1 < actEl.rNr - 1; ++i1) {
      ip3 = actEl.data[i1-1];
      ip4 = actEl.data[i1];
      irc = MSH_EL_ck_EL_4ip (ip1, ip2, &ip3, &ip4, ActPtab.data);
      if(irc >= 0) {
        TX_Print("***** ERROR: edges %d-%d / %d-%d intersect *****",
                 *ip1, *ip2, ip3, ip4);
        return -1;
      }
    }
  }



  // check free space (realloc if necessary) & add index
  // UME_alloc_add (&actEl.ia, &l1, &actElSiz, &actEl.iNr,
                 // sizeof(int), &ActPtNr, 1, INC_actEl);
  MemTab_sav (&actEl, &l1, &ActPtNr, 1);
  if(actEl.data == NULL) goto L_EOM;
  
  ActSelTyp = -2;     // used by delete-operation



  // temporary preview of EdgeLine
  if(mode != 0) return 0;

  gis_actEl_disp ();

  DL_Redraw ();

  return 0;


  //----------------------------------------------------------------
  L_EOM:
    TX_Print("gisEdg_addGisPt EOM");
    return -1;


}

//================================================================
  int gisEdg_ck_pt__ (int *ip1, int *ip2) {
//================================================================
// test if gispoint lies on edge ip1-ip2
// RetCod: -1=no point on edge; else pointNr.

  int      irc, i1, pNr;
  Point2   p1, p2;
  Point    *pTab;

  // printf("gisEdg_ck_pt__ %d %d\n",*ip1,*ip2);

  pNr  = ActPtab.rNr;
  pTab = ActPtab.data;

  p1 = UT2D_pt_pt3 (&pTab[*ip1]);
  p2 = UT2D_pt_pt3 (&pTab[*ip2]);

  for(i1=0; i1<pNr; ++i1) {
    if((i1 == *ip1) || (i1 == *ip2)) continue;
    irc = UT2D_pt_ck_onLine (NULL, &p1, &p2, (Point2*)&pTab[i1], TOL_GIS_PT);
    if(irc >= 0) return i1;
  }


  return -1;

}


//================================================================
  int gis_actEl_disp () {
//================================================================


  // temporary preview of EdgeLine
  GL_temp_delete ();   // remove all temp elems (previous EdgeLine)

  // GR_Disp_cv
  gis_EL_disp (-1L, actEl.data, actEl.rNr, ActPtab.data, col_actEl, 0);

    // gis_dump_actEl ();       // dump actEl

  return 0;

}
 

//================================================================
  int gis_actEl_del () {
//================================================================
// clear actEl, remove display

  // printf("gis_actEl_del \n");

      GL_temp_delete ();
      DL_Redraw ();

      actEl.rNr = 0;
      actEl.use = 2; // EdgeLine

  return 0;

}
 

//========================================================================
  int gis_EL_disp (long dli,
                   int *ib, int iNr, Point *pa, int att, int mode) {
//========================================================================
// display polygon - us3d for display of Edgline actEl
/// Input:
///   dli       -1 for temporary element; or from DL_StoreObj
///   ib, iNr   boundary; indices into pa
///   pa        pointTable
///   mode 0    open polygon
///        1    closed polygon - closing point is added !

  int    i1, iSiz;
  Point  *cv;


  // printf("gis_EL_disp dli=%ld iNr=%d mode=%d\n",dli,iNr,mode);


  if(iNr < 2) return 0;

  iSiz = iNr + mode;
  i1 = sizeof(Point) * iSiz;
    // printf(" tmpSpcSiz=%d\n",i1);

  cv = (Point*)UME_alloc_tmp(i1);
  // cv = (Point*)alloca(sizeof(Point)*(iNr + 1));
  if(cv == NULL) {TX_Print("gis_EL_disp EOM"); return -1;}


  for(i1=0; i1<iNr; ++i1) {
      // printf(" P[%d] ib=%d\n",i1,ib[i1]);
    cv[i1] = pa[ib[i1]];
  }

  if(mode > 0) cv[iNr] = cv[0];

  GL_DrawPoly (&dli, att, iSiz, cv);

  // dli=DL_Ind_tmp-1
    // printf(" dli=%ld\n",GL_GetInd_temp() - 1);

  return 0;

}


//================================================================
  int gis_EL_cleanI (MemTab(EdgeLine) *eTab, MemTab(int) *eDat) {
//================================================================
// gis_EL_cleanI       init stat of all EdgeLines

  int      i1;

  // printf("gis_EL_cleanI \n");

  for(i1=0; i1<eTab->rNr; ++i1) eTab->data[i1].stat = 0;


  return 0;

}


//================================================================
  int gis_EL_cleanE (MemTab(EdgeLine) *eTab, MemTab(int) *eDat) {
//================================================================
// gis_EL_cleanE       delete edgelines with less than 2 points; 

  int      i1, elNr, iFirst;
  long     l1;
  EdgeLine *el1;


  // printf("gis_EL_cleanE %d\n",eTab->rNr);

  elNr = eTab->rNr - 1;
  if(elNr < 0) return 0;

  iFirst = elNr;

  for(i1=0; i1<eTab->rNr; ++i1) {
    el1 = &eTab->data[i1];
    if(el1->iNr < 2) {
      // gis_Edges_del (i1);
      MSH_eTab_delRec (eTab, eDat, i1);
      // redraw all following breaklines!
      if(iFirst > i1) iFirst = i1;
      --i1;  // check record[i1] once more ..
    }
  }


  for(i1=iFirst; i1<eTab->rNr; ++i1) eTab->data[i1].stat = 1;


  // remove display of deleted edgelines ..
  // search dli of appObj
  for(i1 = elNr; i1 >= eTab->rNr; --i1) {
      // printf(" del disp edgLn %d\n",i1);
    l1 = DL_find_APPOBJ (i1, APPTYP_EDGLN);
    // del dlObj
    if(l1 >= 0) GL_Del0 (l1);
  }


  // redraw modified edgelines
  for(i1=0; i1<eTab->rNr; ++i1) {
    if(eTab->data[i1].typ == TYP_EDGLN_AB) continue; // skip automatic bound
    if(eTab->data[i1].stat == 0) continue;
    gis_DrawEL (&eTab->data[i1], i1);
  }


  return 0;

}


//================================================================
  int gis_block (int mode) {
//================================================================
// mode 1=block input
//      0=unblock input
 

  if(mode > 0) {
    GUI_set_enable (&tbGis1, FALSE); // block toolbar
    UI_block__  (-1, 1, 1);         // block input


  } else {
    GUI_set_enable (&tbGis1, TRUE);
    UI_block__  (-1, 0, 0);

  }


  return 0;

}

 
//================  EOF  ==================================
