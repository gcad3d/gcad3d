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

GIS1.c                                2008-01-24    Franz Reiter.
- interactive GIS

-----------------------------------------------------
TODO:
HOW TO ADD gis-points:
- save all OB,IB-points
- add gis-point(s)
- restore OB,IB-points (add after last gis-point)
- restore OB,IB-edgeline (update pointers)




-----------------------------------------------------
Modifications:
2012-08-20 V3.10 Port -> C-KIT-GUI. RF.
2011-01-03 V2.00 New: Breaklines, InnerBoundaries. RF.
2008-01-24 V1.00 Projektstart. RF


Linkjob: ../xa/GIS1.mak
make -f GIS1.mak
or dynamic (Options/compile-ON)


=====================================================
Liste_Funktionen_Beginn:

gCad_main            CAD-Core starts application here
gCad_fini            close application
gis_init_pl          get surf-pTab-List from DB
gis_init_UI          Create UI-panel (Toolbar) Main-Window

gis_CB1              action-dispatcher
gis_key_CB           keypress-callback
gis_sel_CB           selection-callback CAD-Core reports selection
gis_ntb_CB
gis_SurLst_CB        userselection of surfaces-listEntry
gis_EdgTyp_CB        userselection of edge-type
gis_UI_getPt         get coords from entryfields
gis_del_ptRange_CB   delete gisPoints; single or range
gis_movPts_CB        move range of points
gis_del__            delete active gis-obj

gis_msg__

gis_block            block/unblock
gis_UI_disp_actObj   display activeSurfNr & activePointNr
gis_UI_edg_mod       ActEdg.rNr changes
gis_changed_Func
// gis_changed_visi     lock prjMsh if no mesh exists
gis_changed_mshMod   ActFtabModFlag changes  (mesh needs update)
// gis_ui_ButPrj     block/unblock PrjMsh-Function
gis_UI_sur_lst       create list of sufaces with pTabs; display List.

gis_mdl_upd          create update / Modelcodes
gis_mdl_msh_del      delete mesh-modelcode
gis_mdl_del_pTab     delete pTab from model (del A<surInd> - Line)
gis_mdl_add_msh      add A1=MSH A<pointSurfNr>   to model
gis_mdl_add_ptab     add A1=PTAB   to model

gis_surTab_add       add new surf
// gis_surInd_surNr     get index into surTab from pointSet-dbi
gis_sur_on           activate surface surTab[surInd]
gis_sur_off          disactivate surface surTab[ActSurInd]

gis_edg_bnd_disp_off hide/redisplay boundary of pointSet

// // gis_pts_add__        add point to pointSet and display point
// // gis_pts_bnd_disp__   create/update display of boundary of pointSet (mesh-bound)
// // gis_pts_bnd_disp_del delete display of boundary of pointSet
// // gis_pts_msh_del1     delete pointSet and all meshes with this pointSet
// gis_pts_msh_del2  UU
// gis_pts_del__        delete Point Nr ActPtNr from surface ActSurPtb
// gis_pts_del_all      remove all Points
gis_pts_disp_all     display all gis-points
gis_pts_disp__       display single gis-point
gis_pts_disp_1       display point
gis_pts_disp_off     remove display of all gis-points
gis_pt_disp__        extra display gis-point
gis_pt_disp_tmp      display gis-point
gis_disp_coords      display coords of active gis-point
// // gis_ptab_init        if no surface exists, init one
// gis_ptab_load  UU    user has pointfile selected; load it to pTab
// gis_ptab_fload
// gis_ptab_move        move all points
// gis_ptab_save  UU    Export points into Ascii-file
gis_PtMod            modify Point Nr ActPtNr from surface ActSurPtb
gis_PtDelAct         delete Point Nr ActPtNr from surface ActSurPtb
gis_PtAdd            add Point to surface ActSurPtb
gis_cre_cadPt

gis_fac_disp         extra display face

gis_msh_cr           create new mesh with active pointset
gis_mesh             create/diplay mesh
gis_msh_disp         display or redisplay mesh; shaded or symbolic
gis_msh_del          remove mesh
gis_msh_disp_del     delete mesh-display

gisEdg_init          start definition of edgeline
gis_edg_ini_stat     init stat of all IntTabs
gis_edg_add__        add polygon iTab[iNr] to ActEtab/DatEtab

gis_edg_act_disp     display ActEdg as temp. polygon
gis_edg_act_disp_del remove display

gis_edg_disp_all     (re)display all edgeLines (not mesh-boundary)
gis_edg_disp_upd     update display of all edgeLines
gis_edg_disp__       display IntTab
gis_edg_disp_cv      display edgeline polygon
gis_edg_disp_off     remove display of edgeLines
gis_edg_disp_del__   delete display of IntTab
gis_edg_disp_del_all remove display of all edgeLines
gis_edg_load__
gis_edg_fload


gis_edg_del_all      delete ActEdg & alle IntTabs
gis_edg_mod          init/add/update/delete ActEdges

// gis_etab_save    UU  Export edgelines into Ascii-file
// gis_etab_load    UU  import edgelinefile

// gis_obd_cre          Create OuterBoundary from all points

ptab_cad_load1        get all points and all lines from DB      UNUSED

// gis_dump_surTab
// gis_dump_pntTab      dump pntTab
// gis_dump_ActEdg
// gis_dumpHtm_EL       dump ActEtab

Liste_Funktionen_Ende:
=================================================================


PrgFlow:
gis_CB1     (void*)"crMsh"
  gis_mesh ();
    MSH_msh0__ 
      MSH_EL_get_tab
      MSH_ibnd_pt2n    
      gis_edg_add__   
      MSH_EL_get_tab
      MSH_itess_pt2n

GR_DrawSur
  GR_DrawSurPtab
    MSH_ibnd_ptn
  TSU_DrawSurPMsh




//----------------------------------------------------------------
Modelfile:
  SECTION PTAB _A1
  SECTION PTAB _A3
  SECTION MESH _A2
  SECTION MESH _A4
  A1=PTAB
  A2=MSH A1

typ_sur_ptab* surTab[surTabNr]
  dbi_p   dbi of pointSet;
  dbi_m   dbi of mesh; 
  exi_p  0 = pointSet created this run; gis_mdl_upd creates "A# = PTAB"
         1 = pointSet did exist when entering prog
  exi_m  0 = mesh created this run; gis_mdl_upd creates "A# = MSH A#"
         1 = mesh did exist when entering prog
  vis_p  0 = the boundary is not yet visible (new defined, no points yet)
         1 = the boundary is already visible (first create done)
  vis_m  0 = the mesh is not yet visible (new defined, no points yet)
         1 = the mesh is already visible (first create done)

typ_ptab* pntTab[pntTabNr]
  dbi_p    DB-Index of PTAB-surf

ActSurInd   pointer into surTab
ActSurPtb   dbi of active pointSet
ActSurMsh   dbi of active mesh

MemTab(Point)    ActPtab    the active pointSet
MemTab(Fac3)     ActFtab    the active faces
MemTab(IntTab) ActEtab    the active edgeLines
MemTab(int)      DatEtab    indexes to points for edgeLines

IntTab* ActEtab
  ia     Indexes into pointTable DatEtab
  iNr    nr of objects in ia
  typ    type of edgeLine; BL IB OB AB
  stat   char;

GIS-Punkte in der Displaylist: 
  typ=Typ_APPOBJ
  Ind=pointNr;
  att=0=APPTYP_GISPT     gis_pts_disp__

GIS-Edgelines in der Displaylist: 
  typ=Typ_APPOBJ
  Ind=EdgeIndexNr;
  att=1=APPMSHIG_EDGLN     gis_edg_disp__

GIS-Mesh in der Displaylist:
  typ=Typ_SUR
  att=1 (symbolic)

GIS-Ptab in der Displaylist (= boundaryCurve):
  typ=Typ_SUR (150);
  uv=1 (unvisible)


---------------------------------------------------------
appStart:  add all existing meshes into surTab
           make last mesh active
appExit:   make active mesh inactive

make mesh active:    display mesh symbolic, hide boundary, create edgeLines

recompute mesh:      redisplay face symbolic and outer edgeLine

make mesh inactive:  display mesh shaded; recreate baoundary, delete edgeLines







---------------------------------------------------------
../ut/ut_itmsh.h
(IntTab*)    ActEtab
  typ  2=IntTab 3=InnerBound 5=OuterBound



---------------------------------------------------------
    MSH_bload_pTab


*/
#ifdef globTag
 void gis(){}
#endif


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


#include <GL/gl.h>
#include <GL/glu.h>


#ifdef _MSC_VER
// die folgenden 2 Funktionen exportieren (werden vom Main gerufen):
__declspec(dllexport) int gCad_main ();
__declspec(dllexport) int gCad_fini ();
// nachfolgende externals werden aus dem Main-Exe imported:
#define extern __declspec(dllimport)
#endif




// #include "../ut/ut_umem.h"             // MEM_alloc_tmp
#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_itmsh.h"            // MSHIG_EDGLN_.. typedef_MemTab.. Fac3
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_face.h"              // MSH_EDGLN_BL
#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_TX.h"               // TX_Print
#include "../ut/ut_col.h"              // COL_DL_ATT

#include "../gui/gui__.h"

#include "../ut/func_types.h"               // SYM_TRI_S
#include "../gr/ut_GL.h"               // GL_get_curPos_CP__
#include "../gr/ut_DL.h"               // DB_GetDLatt

#include "../db/ut_DB.h"               // DB_GetGTxt

#include "../xa/xa.h"                  // APP_OBJ_NAM AP_mod_fnam
#include "../xa/xa_mem.h"              // memspc51
#include "../xa/xa_sele.h"             // Typ_goGeom

#include "../myGIS1/GIS1.h"


#define   TRUE 1
#define   FALSE 0

#define TMP_DL_ci -3L    // small circ for 1. point




//---------------- external vars: ---------------------

// ../xa/xa.c
extern char APP_act_nam[128];    // name of last script- or plugin-program


// ex xa_ui.c:
extern MemObj    UIw_Box_TB;    // toolbarBox
extern int       KeyStatCtrl;


// // ex ../ut/ut_txfil.c
// extern char *UTF_FilBuf0;
// extern long UTF_FilBuf0Len;  // die aktuelle Filesize


// aus ../gr/ut_DL.c:
extern DL_Att  *GR_ObjTab;  // das ist die DL



//----------------------------------------------------------------
// ../myGIS1/Gis_u1.c 
extern MemTab(Point) ActPtab;        // table of gis-Point's
// extern MemTab(Point) ActBtab;        // table of boundary-Point's

extern MemTab(Fac3) ActFtab;         // table of Fac3's

extern MemTab(IntTab) ActEtab;    // table of IntTab's

// indexes to points for ActEtab
extern MemTab(int) DatEtab;

// static  IntTab  ActEdg;                      // active IntTab
extern MemTab(int) ActEdg;        // dataSpace for active IntTab

// extern int       gis_PTNR;      // nr of user-defined gis-points
extern Point2    gis_origin;
extern double    gis_TOL_PT;

extern int       ActFtabModFlag;   // 1=modified; need update.  0=reset
extern int       ActPtNr;
extern int       ActSelTyp;


//---------------- local vars: ---------------------
// a list of active surfaces from PTAB's
static  typ_sur_ptab surTab[MAX_SURPTAB];      // Liste der aktiven pTab-surfs
static  int surTabNr;                          // nr of active pTab-surfs
static  int surIndLast;

static  typ_ptab     pntTab[MAX_SURPTAB];      // Liste der aktiven pointSets
static  int pntTabNr;

static  int       ActSurInd;    // index of active surTab-record
        int       ActSurPtb,    // DB-Index (surTab.dbi_p) of active point-surf.
                  ActSurMsh,    // DB-Index (surTab.dbi_m) of active mesh-surface
                  ActSelInd;
static  long      ActPtDli;     
static  int       gisFunc;

static  char *s_wenpn="edPt";



#define col_ActEdg 7                  // colour


static  MemObj tbGis1=GUI_OBJ_NEW;    // GIS-Toolbar
static  MemObj wlbas, wenpn, wEL_nr, wEL_box1, wEL_box2;
static  MemObj wenvx, wenvy, wenvz, wcboz;
static  MemObj wlsPtb, wlsMsh, womMsh;
static  MemObj wButMsh, wButEx;

static  GLfloat GL_col_tab [][4] = {
  0.7,  0.1,  0.1, 1.0,     //  0 col. points
  0.1,  0.1,  0.7, 1.0};    //  1 col. info
// red  green blue intens



  static APP_OBJ_NAM appObjNamTab[]={
    {"GisPt",  APPTYP_GISPT},   // 0
    {"Edge",   APPMSHIG_EDGLN},   // 1
    {"Mesh",   APPTYP_MESH},    // 2
    {"Line",   APPTYP_LINE},    // 3
    {"",-1}
  };



//----------------------------------------------------------------
//---------------- prototypes: ---------------------
  int gis_init_UI (MemObj *parent);
  int gis_sel_CB (int src, long dl_ind);
  int gis_key_CB (int key);
  int gis_CB1 (MemObj *mo, void **data);
  int gis_ntb_CB (MemObj *mo, void **data);
  int gis_SurLst_CB (MemObj *mo, void **data);
  int gis_EdgTyp_CB (MemObj *mo, void **data);
  // int gis_pts_msh_del2 (MemObj *mo, void **data);
  int gis_ptab_load (char *fnam, char *dirNam);
  // int gis_etab_load (char *fnam, char *dirNam);
  // int gis_etab_save (char *fnam, char *dirNam);
  int gis_ptab_save (char *fnam, char *dirNam);
  // int gis_surInd_surNr (long surNr);
  int gis_edg_disp__ (IntTab *el1, int ind);
  int gis_edg_disp_cv (long dli, int *ib, int iNr, Point *pa, int att, int mode);
  int gis_del_ptRange_CB ();
  int gis_movPts_CB ();
  int gis_edg_fload (char *filNam, char *dirNam);








//=========================================================
  int gCad_main () {
//=========================================================
// user has selected this plugin; starting ...



  int     *iba, iNr;
  // Mesh    ms1;

  // AP_debug__ ("GIS1");

  // write to Main-Infowindow ..
  TX_Print("%s",VERSION);               //  ../myGIS1/GIS1.h
  printf(" %s\n",APP_act_nam);

  LOG_A_init ("APP_GIS1");

  // save active selectionfilter
  sele_save ();

  // block funcs
  UI_block__ (1, 0, 0);   

  // init all structs
  gis_mem_init (0);

  // fill surTab-List from ModelCodes
  gis_init_pl ();

  // reset Edges ..
  gis_edg_mod (TYP_FuncInit);

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
  gis_UI_sur_lst ();


  // activate last mesh-surf
  gis_sur_on (surTabNr-1);


  //----------------------------------------------------------------
  // attach grafic selections
  L_99:
  AP_UserKeyIn_get (gis_key_CB);
  AP_UserSelection_get (gis_sel_CB);
  PLU_appNamTab_set (appObjNamTab);

  ActPtDli = -1;

  AP_stru_2_txt (NULL, 0, NULL, 0);  // init the objNr's before UTO_sav_ost

  TX_Print("%d GIS-surfaces loaded",surTabNr);

  GR_Disp_message (0); // off

    printf(" ex _main 1\n");


  return 0;

}


//================================================================
  int gis_init_UI (MemObj *parent) {
//================================================================
// gis_init_UI          Create UI-panel (Toolbar) Main-Window
//
//   ,-----------------------------------------------------------,
//   |,--------------------,,----------------,,-----------------,|
//   || boxh1v             ||   notebook     ||      boxh3v     ||
//   |'--------------------''----------------''-----------------'|
//   '-----------------------------------------------------------'
//


  MemObj boxh, boxh1v, wNtb1, boxh3v, w1, w2, w3;
  MemObj mb1, mb1f, mb2s;


  printf("gis_init_UI \n");


  // if(GUI_OBJ_IS_VALID(&tbGis1)) {
    // return 0;
  // }



  // tbGis1 = GUI_toolbar__ (parent);
  tbGis1 = GUI_toolbox__ (parent);        // 2014-11-05

  boxh = GUI_box_h (&tbGis1, "");

  // 1. box: group of boxes box-below-box
  boxh1v = GUI_box_v (&boxh, "");

  // 2. box: notebook.
  GUI_spc__ (&boxh, 0,  5);     // 5 pixels right
  wNtb1 = GUI_notebook__ (&boxh, gis_ntb_CB);

  // 3. box
  GUI_spc__ (&boxh, 0,  5);     // 5 pixels right
  boxh3v = GUI_box_v (&boxh, "");


  //----------------------------------------------------------------
  // 1. vert. box boxh1v
  GUI_spc__ (&boxh1v, 1,  5);     // 5 pixels down

  GUI_label_htm__ (&boxh1v, "<b> GIS1 </b>", "");

  w2 = GUI_box_h (&boxh1v, "");
  GUI_label__ (&w2, "Pnts ", "");
  wlsPtb = GUI_label__ (&w2, " A.. ", "l5");
  GUI_Tip  (" active point-set ");

  w2 = GUI_box_h (&boxh1v, "");
  GUI_label__ (&w2, "Mesh ", "");
  wlsMsh = GUI_label__ (&w2, " A.. ", "l5");
  GUI_Tip  (" active mesh ");
  


  //----------------------------------------------------------------
  // notebook:
  // create notepook-page-0
  w1 = GUI_notebook_add (&wNtb1, "Admin");
  w2 = GUI_box_h (&w1, "");

  // selection active mesh
  // create (empty) list for the mesh-surfaces
  womMsh = GUI_optmen__ (&w2, "?", NULL, NULL, gis_SurLst_CB, "5");
  GUI_Tip  (" change active surface ");

  GUI_button__ (&w2," create new pointSet", gis_CB1, (void*)"crPts", "");
  GUI_Tip  (" create new pointSet and new Mesh ");

  GUI_button__ (&w2," create new Mesh ",    gis_CB1, (void*)"crMsh", "");
  GUI_Tip  (" create new Mesh with the active pointSet ");

  GUI_button__ (&w2,"delete mesh",          gis_CB1, (void*)"delMsh", "");
  GUI_Tip  ("delete active mesh, delete pointSet if not used");

  wcboz = GUI_ckbutt__ (&w2, "remove outer zone", 0, NULL, NULL, "");
    GUI_Tip  (" remove outer triangles (check sidelenght) ON or OFF");


  //----------------------------------------------------------------
  // create notepook-page-1
  w1 = GUI_notebook_add (&wNtb1, "Points");
  w2 = GUI_box_h (&w1, "");

  GUI_button__ (&w2, "Import Points from File",
                        gis_CB1, (void*)"opePtab", "");

  GUI_button__ (&w2, "Export Points",
                        gis_CB1, (void*)"savPtab", "");

  GUI_button__ (&w2, "move all points",
                        gis_CB1, (void*)"movPts", "");

  GUI_button__ (&w2, "delete range of points",
                        gis_CB1, (void*)"delPtR", "");

  GUI_button__ (&w2, "dump all points",
                        gis_CB1, (void*)"dumpPt", "");



  //----------------------------------------------------------------
  // create notepook-page-1
  w1 = GUI_notebook_add (&wNtb1, "edit Point");
  w2 = GUI_box_h (&w1, "");
  // wlbap = GUI_label__ (&w2, "- ", "l16");
  wenpn = GUI_entry__ (&w2, NULL, "- ", gis_CB1, s_wenpn, "9");

  wenvx = GUI_entry__ (&w2, " X: ", "", NULL, NULL, "12");
  wenvy = GUI_entry__ (&w2, " Y: ", "", NULL, NULL, "12");
  wenvz = GUI_entry__ (&w2, " Z: ", "", NULL, NULL, "9");

  GUI_button__ (&w2, "Mod.Pt.", gis_CB1,    (void*)"PtMod", "");
  GUI_button__ (&w2, "Add Pt.", gis_CB1,    (void*)"PtAdd", "");



  //----------------------------------------------------------------
  // create notepook-page-3
  w1 = GUI_notebook_add (&wNtb1, "Edges");
  w2 = GUI_box_h (&w1, "");

  wEL_nr = GUI_label__ (&w2, "  - ", "3");  // format " %3d "

  wEL_box1 = GUI_box_h (&w2, "");
  GUI_button__ (&wEL_box1, "IntTab",      gis_EdgTyp_CB, (void*)"EL_EL", "");
  GUI_Tip  (" save active line as IntTab ");
  GUI_button__ (&wEL_box1, "InnerBoundary", gis_EdgTyp_CB, (void*)"EL_IB", "");
  GUI_Tip  (" save active line as InnerBoundary ");
  GUI_button__ (&wEL_box1, "OuterBoundary", gis_EdgTyp_CB, (void*)"EL_OB", "");
  GUI_Tip  (" save active line as OuterBoundary ");
  GUI_button__ (&wEL_box1, "cancel",        gis_EdgTyp_CB, (void*)"EL_ex", "");
  GUI_set_enable  (&wEL_box1, 0); // disable

  GUI_sep__ (&w2, 1, 5);
  wEL_box2 = GUI_box_h (&w2, "");
  GUI_button__ (&wEL_box2, "OB-edges", gis_CB1, (void*)"opeEtabOB", "");
  GUI_Tip  (" import edgelines (outer boundary) from file");
  GUI_button__ (&wEL_box2, "IB-edges", gis_CB1,       (void*)"opeEtabIB", "");
  GUI_Tip  (" import edgelines (inner boundary) from file");
  GUI_button__ (&wEL_box2, "BL-edges", gis_CB1,       (void*)"opeEtabBL", "");
  GUI_Tip  (" import edgelines (edges) from file");
  GUI_button__ (&wEL_box2, "Import lines", gis_CB1,    (void*)"opeLna",  "");

  w1 = GUI_frame__ (&wEL_box2, NULL, 0);
  w2 = GUI_box_h (&w1, "");
  mb1 = GUI_menu__ (&w2, NULL, 0);
  mb1f = GUI_menu__ (&mb1, " ... ", 0);
  GUI_menu_entry (&mb1f, "export edges", gis_CB1, (void*)"savEtab");
  GUI_menu_entry (&mb1f, "dump edges",   gis_CB1, (void*)"dumpEL");
  GUI_menu_entry (&mb1f, "clear edges",  gis_CB1, (void*)"clearEL");
  // GUI_menu_entry (&mb1f, "dump fnb > file", gis_CB1, (void*)"dumpFnb");



  //----------------------------------------------------------------
  // create notepook-page-2
  w1 = GUI_notebook_add (&wNtb1, "Project");
  GUI_spc__ (&w1, 1,  5);     // 5 pixels down
  GUI_label__ (&w1, " select CAD-point or CAD-curve ..", "l");


  //----------------------------------------------------------------
  // create notepook-page-4
  w1 = GUI_notebook_add (&wNtb1, "delete");
  GUI_spc__ (&w1, 1,  5);     // 5 pixels down
  GUI_label__ (&w1, " select gis-point or gis-edgeline to delete ..", "l");


  //----------------------------------------------------------------
  // 3. box
  w1 = GUI_box_h (&boxh3v, "e,e");
  wButMsh = GUI_button__ (&w1, " create Mesh ", gis_CB1, (void*)"reMsh",  "");
  w2 = GUI_box_h (&boxh3v, "e,e");
  GUI_button__ (&w2, "Log", gis_CB1,    (void*)"Log", "");
  GUI_Tip  (" view log-file (errors ..)");
  GUI_button__ (&w2, "Help", gis_CB1,    (void*)"Help", "");
  wButEx = GUI_button__ (&w2, "Exit", gis_CB1,    (void*)"Exit", "e,e");


  //----------------------------------------------------------------
  GUI_set_show (&tbGis1, 1);    // show

    printf("ex gis_init_UI\n");


  return 0;

}


//=====================================================================
  int gis_ntb_CB (MemObj *mo, void **data) {
//=====================================================================
// notebook-callbacks

  int      newNtb, i1, i2;
  // char     s1[120];


  printf("gis_ntb_CB \n");

  if(GUI_DATA_EVENT != TYP_EventEnter) return 0;
  newNtb = GUI_DATA_I1;


  printf("================================= \n");
  printf("gis_ntb_CB newNtb=%d gisFunc=%d\n",newNtb,gisFunc);


  // if(gisFunc == ) {
  // }

    gisFunc = newNtb;
    gis_changed_Func ();
    gis_msg__ (0);
    DL_hili_off (-1L);  // reset all hilited ..
    GL_temp_del_1 (TMP_DL_ci);
    DL_Redraw ();


  return 0;

}


//=====================================================================
  int gis_CB1 (MemObj *mo, void **data) {
//=====================================================================


  long l1;
  char *cp1, cbuf1[256];


  cp1 = GUI_DATA_S1;

  printf("gis_CB1 |%s|\n",cp1);
  LOG_A__ (MSG_ERR_typ_INF, cp1);



  //-------------------------------------------------
  if(!strcmp(cp1, "Help")) {
    APP_Help (APP_act_nam, "");


  //-------------------------------------------------
  } else if(!strcmp(cp1, "Exit")) {
    gCad_fini ();


  //-------------------------------------------------
  } else if(!strcmp(cp1, "print")) {
    // UI_WinPrint1 (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncInit));


  //-------------------------------------------------
  } else if(!strcmp(cp1, "opePtab")) {
    // APP_Open ("Open Pointfile", "*", (void*)gis_ptab_load);
    // TEST:
    // gis_ptab_fload("/mnt/serv1/Devel/dev/gCAD3D/dat/GIS1/bk4_pt.dat");
    gis_ptab_fload("/mnt/serv1/Devel/dev/gCAD3D/dat/GIS1/mad6_pt.dat");


  //-------------------------------------------------
  } else if(!strcmp(cp1, "opeEtabOB")) {
    ActPtDli = -1L;
    ActEdg.use = MSH_EDGLN_OB;
    // APP_Open ("Open file outer boundary", "*", (void*)gis_edg_fload);
    // TEST:
    // gis_edg_load__("/mnt/serv1/Devel/dev/gCAD3D/dat/GIS1/bk4_ob9.dat");
    gis_edg_load__("/mnt/serv1/Devel/dev/gCAD3D/dat/GIS1/mad6_ob.dat");



  } else if(!strcmp(cp1, "opeEtabIB")) {
    ActPtDli = -1L;
    ActEdg.use = MSH_EDGLN_IB;
    // APP_Open ("Open file inner boundary", "*", (void*)gis_edg_fload);
    // TEST:
    // gis_edg_load__("/mnt/serv1/Devel/dev/gCAD3D/dat/GIS1/bk2_ib1.dat");
    // gis_edg_load__("/mnt/serv1/Devel/dev/gCAD3D/dat/GIS1/bk2_ib2.dat");
    // gis_edg_load__("/mnt/serv1/Devel/dev/gCAD3D/dat/GIS1/bk4_ib.dat");
    gis_edg_load__("/mnt/serv1/Devel/dev/gCAD3D/dat/GIS1/mad6_ib.dat");



  } else if(!strcmp(cp1, "opeEtabBL")) {
    gis_edg_del_typ (MSH_EDGLN_BL);   // delete all existing breaklines
    ActPtDli = -1L;
    ActEdg.use = MSH_EDGLN_BL;
    // APP_Open ("Open file edgelines", "*", (void*)gis_edg_fload);
    // TEST:
    // gis_etab_load ("edgelines_bk1.dat", /mnt/serv1/Devel/dev/gCAD3D/dat/GIS1");
    gis_block (1);  // block input
    // gis_edg_load__ ("/mnt/serv1/Devel/dev/gCAD3D/dat/GIS1/bk4_bl.dat");
    gis_edg_load__("/mnt/serv1/Devel/dev/gCAD3D/dat/GIS1/mad6_bl.dat");
    gis_block (0);  // unblock input



  } else if(!strcmp(cp1, "opeLna")) {      // Import lines
    ActPtDli = -1L;
    // ActEdg.use = MSHIG_EDGLN_LN;
    ActEdg.use = MSH_EDGLN_LN;
    // APP_Open ("Open Pointfile", "*", (void*)gis_edg_fload);
    // TEST:
    gis_edg_load__("/mnt/serv1/Devel/dev/gCAD3D/dat/GIS1/bk4_ob.dat");
    

  //-------------------------------------------------
  } else if(!strcmp(cp1, "savPtab")) {
    APP_Save ("Save Pointfile", "points.dat", (void*)gis_ptab_save);


  //-------------------------------------------------
  } else if(!strcmp(cp1, "savEtab")) {
    // gis_etab_save ("edgelines.dat", "/mnt/serv1/Devel/dev/gCAD3D/dat/GIS1");
//     APP_Save ("Save edgelines to file", "edgelines.dat", (void*)gis_etab_save);
TX_Error("TODO: savEtab - gis_etab_save()");


  //-------------------------------------------------
  } else if(!strcmp(cp1, "delMsh")) {
    // delete active mesh, delete pointset if unused
    gis_pts_msh_del1 (ActSurInd);
    // gis_msh_del (ActSurInd); 
    // gis_sur_on (surTabNr-1);
    // gis_UI_sur_lst ();
    // DL_Redraw ();

/*
  //-------------------------------------------------
  } else if(!strcmp(cp1, "delPts")) {
    //  delete activ pointSet and all its meshes
    gis_pts_msh_del1 ((long)surTab[ActSurInd].dbi_p);
*/

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
  } else if(!strcmp(cp1, "crPts")) {
    gis_sur_off ();          // disactivate surface surTab[ActSurInd]
    gis_ptab_init ();         // create new pointSet and new mesh
    gis_sur_on (surTabNr-1);  // last in list
    DL_Redraw ();


  //-------------------------------------------------
  } else if(!strcmp(cp1, "crMsh")) {
    // create new mesh with active pointset
    gis_msh_cr ();


  //-------------------------------------------------
  } else if(!strcmp(cp1, "reMsh")) {
    GL_temp_del_1 (TMP_DL_ci);
    // reCreate new mesh
    gis_mesh ();
    // display IntTabs (indexes of edgelines changed)
    gis_edg_disp_all (&ActEtab);    


  //-------------------------------------------------
  } else if(!strcmp(cp1, "edPt")) {
    printf("edPt \n");
    cp1 = GUI_entry_get (mo);
    ActPtNr = atoi (cp1);
    gis_disp_coords (&ActPtab.data[ActPtNr]);
    

/*
  //-------------------------------------------------
  } else if(!strcmp(cp1, "edit")) {      // GisPt's, del GisEdgelines
    if(GUI_DATA_EVENT == TYP_EventRelease) return 0;   // skip disactivation
    // GUI_set_enable (&wMenBar, TRUE);
    // GUI_set_enable (&wBoxEd, TRUE);
    // GUI_set_enable (&wButDel, TRUE);
    // GUI_optmen_set (&wledg, 2, NULL);  // disactivate menu edge-types
    gisFunc = GISFUNC_Edit;
    gis_changed_Func ();
    gis_msg__ (0);
    DL_hili_off (-1L); DL_Redraw ();


  //-------------------------------------------------
  } else if(!strcmp(cp1, "prjMsh")) {     // project Cad-object onto Mesh
    if(GUI_DATA_EVENT == TYP_EventRelease) return 0;   // skip disactivation
    // GUI_set_enable (&wMenBar, FALSE);
    // GUI_set_enable (&wBoxEd, FALSE);
    // GUI_set_enable (&wButDel, FALSE);
    // GUI_optmen_set (&wledg, 2, NULL);  // disactivate menu edge-types
    gisFunc = GISFUNC_prjMsh;
    gis_changed_Func ();
    gis_msg__ (0);
    DL_hili_off (-1L); DL_Redraw ();
  

  //-------------------------------------------------
  } else if(!strcmp(cp1, "crEdge")) {     // create edgeline
    if(GUI_DATA_EVENT == TYP_EventRelease) return 0;   // skip disactivation
    // GUI_set_enable (&wMenBar, FALSE);
    // GUI_set_enable (&wBoxEd, FALSE);
    // GUI_set_enable (&wButDel, FALSE);
    // GUI_optmen_set (&wledg, 3, NULL);   // activate menu edge-types
    gisFunc = GISFUNC_crEdge;
    gisEdg_init ();
    gis_changed_Func ();
    gis_msg__ (0);
    GL_temp_del_all ();
    DL_hili_off (-1L); DL_Redraw ();


  //-------------------------------------------------
  } else if(!strcmp(cp1, "delGis")) {
    // GUI_optmen_set (&wledg, 2, NULL);  // disactivate menu edge-types
    gisFunc = GISFUNC_delGis;
    gis_changed_Func ();
*/

  //-------------------------------------------------
  } else if(!strcmp(cp1, "PtMod")) {
    gis_PtMod ();


  //-------------------------------------------------
  } else if(!strcmp(cp1, "PtAdd")) {
    gis_PtAdd ();


  //-------------------------------------------------
  } else if(!strcmp(cp1, "dumpPt")) {
    gis_dumpHtm_pTab ();


  //-------------------------------------------------
  } else if(!strcmp(cp1, "clearEL")) {
    gis_edg_del_all ();
    DL_Redraw ();
    gis_changed_mshMod (1);


  //-------------------------------------------------
  } else if(!strcmp(cp1, "dumpEL")) {
    gis_dumpHtm_EL ();

  //-------------------------------------------------
  // } else if(!strcmp(cp1, "dumpFnb")) {
    // UFA_fnb_dump_f (ActFtab.data, fnb, ActFtab.rNr, "gis1","B");


  //-------------------------------------------------
  } else if(!strcmp(cp1, "Log")) {
    // view logfile
    LOG_A_disp ();



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
  int gis_edg_fload (char *filNam, char *dirNam) {
//================================================================
 
  printf("gis_edg_fload |%s|%s|\n",dirNam,filNam);

  sprintf(memspc011, "%s%s", dirNam, filNam);

  gis_edg_load__ (memspc011);

  return 0;

}


/*
//================================================================
  int gis_changed_pTab () {
//================================================================
  printf("gis_changed_pTab \n");
  ActPtabModFlag = 1;
  ActPtabBndModFlag = 1;
  gis_changed_mshMod (1);   // modified; need remesh.
  return 0;
}
*/


//================================================================
  int gis_changed_Func () {
//================================================================
// call whenever gisFunc changes

  printf("gis_changed_Func %d\n",gisFunc);


  if(gisFunc == GISFUNC_Admin) {
    sele_reset ();


  } else if(gisFunc == GISFUNC_Points) {
    sele_reset ();


  } else if(gisFunc == GISFUNC_Edit) {
    sele_reset ();
    sele_set_types (Typ_TmpPT, Typ_PT, Typ_APPOBJ, 0);   // indicate


  } else if(gisFunc == GISFUNC_crEdge) {
    sele_reset ();
    sele_set_types (Typ_APPOBJ, 0);   // GIS-obj's
    sele_set_add (Typ_go_LCS);        // LN/CI/CV


  } else if(gisFunc == GISFUNC_prjMsh) {
    sele_reset ();
    sele_set_types (Typ_PT, 0);       // add PT
    sele_set_add (Typ_go_LCS);        // LN/CI/CV


  } else if(gisFunc == GISFUNC_delGis) {
    sele_reset ();
    sele_set_types (Typ_APPOBJ, 0);   // indicate
  }


  return 0;

}

/*
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
*/

//================================================================
  int gis_changed_mshMod (int mshMod) {
//================================================================
// ActFtabModFlag changes  (mesh needs update)
// Input:  mshMod 1=modified; need update.
//                0=reset

  static int actMod = 999;


  // printf("gis_changed_mshMod %d %d %d\n",mshMod,actMod,surTab[ActSurInd].vis_m);

  if(actMod == mshMod) return 1;

  ActFtabModFlag = mshMod;

  if(mshMod == 1) {
    // need update
    GUI_button_styl (&wButMsh, 1);     // red
    // GUI_set_enable (&wButEx, FALSE);   // block the exit-button

  } else {
    // reset
    GUI_button_styl (&wButMsh, 0);
    GUI_set_enable (&wButEx, TRUE);
  }

  actMod = ActFtabModFlag;





/*
  // Exit-Button:
  //   if mesh exists: if mesh is modified, lock Exit-Button.
  // CreateMesh-Button:
  //   if mesh exists: if mesh is modified, change CreateMesh-Button color red
  if(surTab[ActSurInd].vis_m != 0) {
    // mesh already exists
    if(ActFtabModFlag != 0) {
      GUI_set_enable (&wButEx, FALSE);
      GUI_button_styl (&wButMsh, 1);  // 1=red; need update
    } else {
      GUI_set_enable (&wButEx, TRUE);
      GUI_button_styl (&wButMsh, 0);  // 0=reset
    }

  } else {
    // mesh does not yet exist
    GUI_set_enable (&wButEx, TRUE);
    GUI_button_styl (&wButMsh, 0);  // reset
  }
*/

  // gis_ui_ButPrj ();   // block/unblock PrjMsh-Function

  return 0;

}


//================================================================
  int gis_UI_edg_mod () {
//================================================================
// ActEdg.rNr changes

  char    s1[8];


  // printf("gis_UI_edg_mod %d\n",ActEdg.rNr);


  if(gisFunc == GISFUNC_crEdge) {    // create edge
    // notebookpage Edges active ..
    if(ActEdg.rNr > 0) {
      // display nr of points in ActEdg
      sprintf(s1, " %3d ", ActEdg.rNr);
      GUI_label_mod (&wEL_nr, s1);

      // lock CreateMesh-Button.
      GUI_set_enable (&wButMsh, FALSE);
      // activate box edgeLine-type
      GUI_set_enable (&wEL_box1, TRUE);
      // disactivate box edgeLine-tools
      GUI_set_enable (&wEL_box2, FALSE);

    } else {
      // activate CreateMesh-Button
      GUI_set_enable (&wButMsh, TRUE);
      // disactivate box edgeLine-type
      GUI_set_enable (&wEL_box1, FALSE);
      // activate box edgeLine-tools
      GUI_set_enable (&wEL_box2, TRUE);
    }

  }

  // gis_ui_ButPrj ();   // block/unblock PrjMsh-Function

  return 0;

}

/*
//================================================================
  int gis_ui_ButPrj () {
//================================================================
// block/unblock PrjMsh-Function

  // printf("gis_ui_ButPrj %d %d %d\n",ActEdg.rNr,
          // surTab[ActSurInd].vis_m,ActFtabModFlag);

  // if temp.edge exists: lock PrjMsh-Func.
  if(gisFunc == GISFUNC_crEdge) {    // create edge
    if(ActEdg.rNr > 0) goto L_OFF;
  }

  // if no mesh exists: block PrjMsh-Function.
  if(surTab[ActSurInd].vis_m == 0) goto L_OFF;

  // mesh exists; if mesh is modified: block PrjMsh-Function.
  if(ActFtabModFlag != 0) goto L_OFF;

  L_ON:
    GUI_set_enable (&wButPrj, TRUE);
    return 0;

  L_OFF:
    GUI_set_enable (&wButPrj, FALSE);
    return 0;

}
*/

//================================================================
  int gis_msg__ (int mode) {
//================================================================
// see also LOG_A__

  if(mode == 1)
  TX_Print("cannot use object -");

  if(gisFunc == GISFUNC_Edit)
  TX_Print("select Gis-Point or Gis-Edgeline ..");

  else if(gisFunc == GISFUNC_prjMsh)
  TX_Print("select Point or Cad-Curve to project onto Mesh ..");

  else if(gisFunc == GISFUNC_crEdge)
  TX_Print("select Gis-Point or Cad-Point or Cad-Curve to add to IntTab ..");

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
  ibp = (int*)MEM_alloc_tmp(i1 * sizeof(int));
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
  gis_edg_add__ (ibp, ibNr, 4);



  return 0;

}
*/


//================================================================
  int gis_msh_cr () {
//================================================================
// create new mesh with active pointset
// add a new record to surTab

  int     OldSurInd;
  char    s1[32];


  printf("gis_msh_cr \n");


  // disactivate active mesh
  gis_msh_disp (0, ActSurInd);

  // remove the edgelines gis_edg_disp_cv gis_edg_act_disp
  GL_Del_om (Typ_APPOBJ, -1L, APPMSHIG_EDGLN);


  // reset ActFtab
  ActFtab.rNr = 0;

  // reset ActEtab
  ActEtab.rNr = 0;
  DatEtab.rNr = 0;


  // get index for new mesh
  ++surIndLast;
  ActSurMsh = surIndLast;
    printf(" init ActSurMsh=%d\n",ActSurMsh);


  // activate new mesh
  OldSurInd = ActSurInd;
  ActSurInd = surTabNr;    // index to the new record
  // add a new record to surTab
  gis_surTab_add (ActSurPtb, ActSurMsh);
  // copy dbi dli of active pointSet
  surTab[ActSurInd].dbi_p = surTab[OldSurInd].dbi_p;
  surTab[ActSurInd].dli_p = surTab[OldSurInd].dli_p;
  surTab[ActSurInd].exi_p = 1;                         // pointSet exists


  // add new mesh to List (update list womMsh)
  gis_UI_sur_lst ();

  // update label meshNr
  sprintf (s1, "A%d", ActSurMsh);
  GUI_label_mod (&wlsMsh, s1);

  // activate new mesh in list womMsh
  GUI_optmen_set (&womMsh, 1, s1);

  DL_Redraw ();

  TX_Print("new mesh A%d initialized; add points ..",ActSurMsh);

    // gis_dump_surTab ();

  return 0;

}


//================================================================
  int gis_mesh () {
//================================================================
// (re)create the mesh
// Input:
//   ActPtab     3D-points
//   ActEtab     boundaries, breaklines
// OFFEN:
// write "A# = MSH A<#_pTab>" into mainProg

  int     irc, att, i1, i2, ii, pNr;
  // int     *ibp, ibNr;
  // int     *iba, ibNr;
  long    dli, dbm, dbp, l1;
  Point   *pa;
  Point2  *p2a;
  // Mesh    ms1;
  // Fac3    *fTab = NULL;


  printf("-------- gis_mesh %d\n",ActSurPtb);

  gis_block (1);  // block input


  if(ActSurPtb < 0) goto L_exit;


  //----------------------------------------------------------------
  // fix mesh-surf-Nr
  if(surTab[ActSurInd].dbi_m > -1) {           // use existing
    dbm = surTab[ActSurInd].dbi_m;

  } else {                                     // create new mesh-surf
    ++surIndLast;
    dbm = surIndLast;
    surTab[ActSurInd].dbi_m = dbm;
      // printf(" next A-nr = %ld\n",dbm);
  }
    // printf(" dbm=%ld\n",dbm);




  //----------------------------------------------------------------
  // init edgeline-stats
  // gis_edg_ini_stat (&ActEtab, &DatEtab);
  // gis_PTNR = ActPtab.rNr;  // nr of user-defined gis-points


  // prepare space for 2D-points
  p2a = (Point2*)memspc501;
  i1 = sizeof(memspc501) / sizeof(Point2);  // 16 byte
  if(i1 < ActPtab.rNr) {TX_Error("gis_mesh E001"); goto L_exit;}


  // transport all points > 2D (pa -> p2a)
  // for XY-Plane copy points ..
  pa = ActPtab.data;
  pNr = ActPtab.rNr;
  UT2D_npt_npt3 (p2a, pa, pNr);


/*
  // get memSpc for the boundary
  ibp = (int*)MEM_alloc_tmp(pNr * sizeof(int));
  if(ibp == NULL) {TX_Error("gis_mesh E002"); goto L_exit;}
  // remove automatic created outer boundary AB
  i1 = MSH_eTab_findRecTyp (&ActEtab, MSHIG_EDGLN_AB);
  if(i1 >= 0) {
    MSH_eTab_delRec (&ActEtab, &DatEtab, i1);
    for(i2=i1; i2<ActEtab.rNr; ++i2) ActEtab.data[i2].stat = 1;
  }
  // add/update OuterBound ibp to ActEtab/DatEtab
  gis_edg_add__ (&ActEtab, &DatEtab, ibp, ibNr, MSHIG_EDGLN_AB);
    // MSH_dump_eTab (&ActEtab, &DatEtab);
*/


  // remove outerZone - 1=yes.
  // i1 = GUI_menu_checkbox_get (&wcboz);  // 0=not checked; 1=checked
  i1 = GUI_ckbutt_get (&wcboz);  // 0=not checked; 1=checked
    printf(" outerZone = %d\n",i1);


  // create Mesh-data.

  // -----------VERSION-0------------------
  // Version 2014. Fastest.
  irc = MSH_msh0__ (&ActPtab, &ActFtab, &ActEtab, &DatEtab, p2a, i1);


  // -----------VERSION-1------------------
  // Version 2015-12-01 / find nearest points, test outer-bound; very slow
  // irc = MSH_msh1__ (&ActFtab, &ActEtab, &DatEtab, pa, p2a, pNr, i1);
  // -----------END VERSION------------------



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
  MSH_bsav_fTab (&ActFtab, &ActEtab, &DatEtab, AP_modact_nam, dbm);


  // // update display of breaklines (after deleting and recreating the
  // // autoOuterBoundary.
  // gis_edg_del_ck2 (&ActEtab, &DatEtab);


  // update the mesh-display (copies dbm > surTab[ActSurInd].dbi_m)
  gis_msh_disp (1, ActSurInd);

  ActSurMsh = dbm;

  // ActFtabModFlag = 0; // unmodified.
  gis_changed_mshMod (0); // reset, unmodified.


  // printf("gis_mesh 4\n");


  L_exit:

  DL_Redraw ();

  gis_block (0);  // unblock input

    printf("ex -------- gis_mesh\n");

  return 0;

}


//================================================================
  int gis_msh_del (int isa) {
//================================================================
// gis_msh_del          delete mesh
// - delete mesh-display
// - delete mesh-modelcode
// - delete surTab-record
// Input:
//   isa         index surTab-record
// RetCod:
//    -1       no mesh exists for the active sur

// OFFEN:
// wenn man mesh löscht, bleibt der Attribut=symbolic-Record stehen;
// im main ist das mesh nun symbolic !

  long     idbm, dli;


  printf("gis_msh_del %d\n",isa);



  // // delete bin.Mesh    sonst geht das UNDO ned ..
  // MSH_bdel_fTab (WC_modnr, dbi);


  //----------------------------------------------------------------
  // delete mesh-display of active mesh
  idbm = surTab[isa].dbi_m;
  if(idbm < 0) return -1;

  // delete mesh-display
  gis_msh_disp_del (isa);



  //----------------------------------------------------------------
  // delete mesh-modelcode (eg "A#=MSH A#")
  // delete Mesh from model (deletes also its display)
  if(surTab[isa].exi_m != 0) gis_mdl_msh_del (idbm);


  //----------------------------------------------------------------
  // delete the active surTab-record
  MEM_del_nrec (&surTabNr, surTab, isa, 1, sizeof(typ_sur_ptab));


  //----------------------------------------------------------------
/*
  // activate last surTab-record
  gis_sur_on (surTabNr-1);

  // update list of meshes (womMsh)
  gis_UI_sur_lst ();

  // reset CreateMesh-Button and Exit-Button
  gis_changed_mshMod (0);

  // gis_changed_vis_m ();     // lock prjMsh

  DL_Redraw ();
*/

  return 0;

}


//================================================================
  int gis_msh_disp (int styl, int isa) {
//================================================================
// gis_msh_disp      display or redisplay mesh; shaded or symbolic
// Input:
//   styl    0 = ON  = shade;        1 = OFF = symbolic
//   isa    index to surTab

// used in gis_mesh gis_sur_on gis_sur_off

  int     mode;
  long    dli, dbi;
  ColRGB  col;


  // printf("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD \n");
  printf("gis_msh_disp styl=%d isa=%d\n",styl,isa);
  printf(" ptNr=%d fNr=%d\n",ActPtab.rNr,ActFtab.rNr);
 

  if(ActFtab.rNr < 1) return -1;


// gis_dump_surTab ();



/*
  // first time to create mesh ?
  dbi = dbm;
  if(surTab[isa].exi_m == 0) {
    // no model exists yet
    if(surTab[isa].dbi_m < 0) {
      // first creation
      mode = 0;
      surTab[isa].dbi_m = dbm;
    } else {
      mode = 1;
    }
  } else {
    // model exists; modify
    mode = 1;
  }
*/




  dbi = surTab[isa].dbi_m;
  dli = surTab[isa].dli_m;
  col = surTab[isa].col;
    printf("  ActFtabModFlag=%d dbi=%ld dli=%ld\n",ActFtabModFlag,dbi,dli);



  //----------------------------------------------------------------
  // recreate or change attrib ?
  if(dli < 0) goto L_recreate;
  if(ActFtabModFlag) goto L_recreate;


  //----------------------------------------------------------------
  // change attrib & redraw; 1=shaded, 2=symbolic
/*
  //   styl    0 = ON  = shade;        1 = OFF = symbolic
  if(styl == 0) mode = 1;
  else          mode = 2;
  // change attrib & redraw; 1=shaded, 2=symbolic  
  GA_sStyl__ (dli, mode, Typ_SUR, dbi);
  goto L_exit;
*/
  DL_SetInd (dli);


  //----------------------------------------------------------------
  // recreate and display mesh
  L_recreate:
    // printf("gis_msh_disp create\n");

  // set styl in col ( 0,shaded or symbolic (1,wireframeboundary)
  COL_ISTYL (&col, styl);
    // UTcol_dump (&col);

  GR_draw_mesh (dli, dbi, &col, ActPtab.data, ActFtab.data, ActFtab.rNr);
  if(dli < 0) {
    surTab[isa].dli_m = GL_GetActInd ();
    surTab[isa].vis_m = 1;
  }


  // gis_changed_visi ();  // unlock prjMsh


  L_exit:
    printf("ex gis_msh_disp\n");

  return 0;

}
 

//================================================================
  int gis_UI_disp_actObj () {
//================================================================
// display activeSurfNr & activePointNr

  char      cbuf[128];


  printf("gis_UI_disp_actObj \n");

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
    // sprintf(cbuf, " Gis-Point %d ",ActPtNr);
    sprintf(cbuf, "%d ",ActPtNr);
  }

  if(ActSelTyp == APPMSHIG_EDGLN) {
    sprintf(cbuf, " Edgeline %d", ActSelInd);
  }

  // GUI_label_mod (&wlbap, cbuf);
  GUI_entry_set (&wenpn, cbuf);

  if(ActSelTyp != APPTYP_GISPT) {
    gis_disp_coords (NULL);
  }

  return 0;

}


//================================================================
  int gis_pts_bnd_disp__ (int isa) {
//================================================================
// gis_pts_bnd_disp__        update display (boundary) of pTab.
// load pointSet from bin-file, compute boundary, display.


  long     dli;

  printf("gis_pts_bnd_disp__ isa=%d dbi=%ld dli=%ld vis=%d\n",isa,
         surTab[isa].dbi_p, surTab[isa].dli_p, surTab[isa].vis_p);

  if(isa < 0) return 0;

  dli = surTab[isa].dli_p;

  // create (-1) or modify curv
  if(dli >= 0) DL_SetInd (dli);

  // redraw curve
  GR_disp_cv_pMesh (surTab[isa].dbi_p, 5);

  if(dli < 0) {
    surTab[isa].dli_p = GL_GetActInd ();
    surTab[isa].vis_p = 1;
  }

/*
  //----------------------------------------------------------------
  // if(surTab[isa].exi_p == 1) { 
  if(surTab[isa].vis_p != 0) {
    // 
    dli = DL_find_obj (Typ_SUR, surTab[isa].dbi_p, -1L);
    if(dli >= 0) DL_unvis_set (dli, 0);  // set unvis=1

    // pointSet did exist in model before ..
//    if(ActPtabBndModFlag == 0) {
        // ptab is unchanged: make it visible again;
//    } else {   // modified, recreate pTab-boundary
//        // printf(" pTab %d modified ..\n",ActSurInd);
//      dli = DL_find_obj (Typ_SUR, surTab[isa].dbi_p, -1L);
//      if(dli >= 0) {
//        DL_unvis_set (dli, 0);
//        DL_SetInd (dli);           // modify, not new
//        APT_DrawSur (5, surTab[isa].dbi_p);
//      }   
//    }

  //----------------------------------------------------------------
  } else {                             // did not exist in model before ..
    // surTab[isa].vis_p == 0
    // pointSet is new
    // APT_DrawSur (5, surTab[isa].dbi_p);
    // create boundary with new DL-record
    GR_DrawSurPtab (surTab[isa].dbi_p, 5, surTab[isa].dbi_p);
    surTab[isa].vis_p = 1;
  }
*/

  return 0;

}


//================================================================
  int gis_fac_disp (int ifc) {
//================================================================
// gis_fac_disp         display face


  MSH_test_disp_f1 (ifc, ActFtab.data, ActPtab.data, "gbs");

  DL_Redraw ();

  return 0;

}

 
//================================================================
  int gis_pt_disp_tmp (Point *p1) {
//================================================================
// gis_pt_disp_tmp       display gis-point

  long      l1;

  // diplay with big square
  l1 = TMP_DL_ci;      // -3L
  GR_temp_symB (&l1, ATT_COL_RED, SYM_SQU_B, p1);
  DL_Redraw ();

  return 0;

}



//================================================================
  int gis_pt_disp__ (int ipt) {
//================================================================
// gis_pt_disp__       display gis-point

  long      l1;
  Point     *p1, p2;


  if(ipt >= ActPtab.rNr) {
    TX_Print ("**** point %d not defined ",ipt);
    return -1;
  }


  p1 = (&ActPtab.data[ipt]);


  // print without offset
  p2 = *p1;
  TX_Print ("p%d = %f %f %f",ipt,p2.x,p2.y,p2.z);

  // print with offset
  UT3D_pt_add_pt2 (&p2, &gis_origin);
  TX_Print ("P%d = %f %f %f",ipt,p2.x,p2.y,p2.z);


  // diplay with big square
  return gis_pt_disp_tmp (p1);

}

 
//================================================================
  int gis_disp_coords (Point *p1) {
//================================================================
// edit-points: display active point in entries and with big cross

  long      l1;
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


  if(p1) {
    l1 = TMP_DL_ci;      // -3L
    GR_temp_symB (&l1, ATT_COL_RED, SYM_SQU_B, p1);
    DL_Redraw ();
  }


  return 0;

}


//================================================================
  int gis_key_CB (int key) {
//================================================================
// callback keypress

  int    i1;
  char   s1[128];
  char   *buttons[]={"Cancel","OK",NULL};


  printf("gis_key_CB key=%d ctrl=%d\n",key,KeyStatCtrl);

  switch (key) {

    //----------------------------------------------------------------
    case GUI_KeyEsc:
      // if group exists: delete group
      if(Grp_get_nr() > 0) {
          Grp_Clear (2);     // clear group
          break;
      }
      // delete last point
      if(gisFunc == GISFUNC_Edit) {
        gis_PtDelAct ();
      } else if(gisFunc == GISFUNC_crEdge) {
        gis_edg_act_del ();
      }
      break;

    //----------------------------------------------------------------
    case 'q':
      gCad_fini ();
      break;

    //----------------------------------------------------------------
    case GUI_KeyDel:
    case GUI_KeyNumDel:
      gis_del__ ();
      break;

    //----------------------------------------------------------------
    case 'p':
      strcpy(s1, "0");
      i1 = GUI_Dialog_e2b ("point-nr = ", s1, 32, "Cancel", "OK");
      if(i1 < 1) break;
      i1 = atoi (s1);
      gis_pt_disp__ (i1);
      break;



    //----------------------------------------------------------------
    case 'e':
      strcpy(s1, "3 1");
      i1 = GUI_DialogEntry ("type [number]", s1, sizeof(s1), buttons, 2);
        printf(" %d |%s|\n",i1,s1);
gis_edg_fwrite ("/mnt/serv1/Devel/dev/gCAD3D/dat/GIS1/bk2_ib1.dat",3,1);
gis_edg_fwrite ("/mnt/serv1/Devel/dev/gCAD3D/dat/GIS1/bk2_ib2.dat",3,2);
      break;




    //----------------------------------------------------------------
    case 'f':
      if(!KeyStatCtrl) {          // Ctrl-f dump faces
        MSH_dump_fTab (ActFtab.data, ActFtab.rNr,"ActFtab");
        break;
      }
      strcpy(s1, "0");
      i1 = GUI_Dialog_e2b ("face-nr = ", s1, 32, "Cancel", "OK");
      if(i1 < 1) break;
      i1 = atoi (s1);
      gis_fac_disp (i1);
      break;

    //----------------------------------------------------------------
    case 't':          // Ctrl-t test
      if(!KeyStatCtrl) {          // 1=OFF; 0=ON
        gis_dump_pTab ();
        gis_dump_surTab ();
        gis_dump_ActEdg ();
        // gis_dumpHtm_pTab ();
        // gis_dumpHtm_EL ();
      }
      break;


    //----------------------------------------------------------------
    case 'd':          // Ctrl-d DispList
      // if(!KeyStatCtrl) {
        // DL_DumpObjTab ();
      // }
      break;

  }

  return 0;

}

 
//================================================================
  int gis_sel_CB (int src, long dl_ind) {
//================================================================
// userselection-callback; CAD-Core reports selection


  int       irc, i1, p1Nr, p1Siz, p2Nr, typ;
  int       *ia, iaNr, iaSiz;
  long      dli, dbi;
  char      cbuf[128], *cp1;
  Point     *pa1, *pa2, p1, p2;
  DL_Att    dla;
  ObjGX     ox1;


  printf("gis_sel_CB src=%d dl_ind=%ld\n",src,dl_ind);


  ActSelTyp = -1;
  ActSelInd = -1;


  if(dl_ind < 0) goto L_ind__;        // indicate

  // get DL-record of selected object
  // dla = DL_GetAtt(dl_ind);
  DL_get_dla (&dla, dl_ind);
    printf(" sel.typ=%d\n",dla.typ);


  if(dla.typ == Typ_APPOBJ) goto L_gis_obj__;
  goto L_cad_obj__;



  //================================================================
  // indicate
  //================================================================
  L_ind__:

    // get pointPosition on object
    // i1 = sele_get_selPos (&p1);
    i1 = sele_get_pos_CP (&p1);
      printf(" indicate: %d\n",i1);
      DEB_dump_obj__ (Typ_PT, &p1, " p1");


    // // get userCoords of indicated point
    // p1 = GL_get_curPos_CP__ ();



  if(gisFunc == GISFUNC_Edit)  {         // 1=edit
    // create gis-point
    goto L_cre_gisPt;
  }


  if(gisFunc == GISFUNC_prjMsh) {        // 2=prjMsh,
    // project pt -> mesh
    i1 = MSH_pt_prjptmsh_ (&p1, &p1, ActSurMsh, ActSurPtb, gis_TOL_PT);
    if(i1 < 0) goto L_err_ptOut;
    // create gis-point
    goto L_cre_gisPt;
  }


  if(gisFunc == GISFUNC_crEdge)  {        // 3=crEdge
    if(dl_ind < 0) goto L_err_sel;
    // display selected gis-point
    gis_disp_coords (&p1);
    // get ActPtNr = pointIndex
    gis_PtAdd ();
    // add gisPt to edge
    irc = gis_edg_act_add_ip (&i1);
      printf(" ex _add_ip irc=%d i1=%d\n",irc,i1);
    if(irc < 0) {
      TX_Print ("**** select point %d before ****",i1);
      return -1;
    }
    gis_UI_edg_mod ();    // ActEdg.rNr changed
  }


  return 0;




  //===================================================
  // gis-obj selected
  //===================================================
  L_gis_obj__:

  ActSelTyp = dla.iatt;
  ActSelInd = dla.ind;        // edgeNr


  if(gisFunc == GISFUNC_delGis) {
    // delete active gis-obj (ActSelTyp, ActSelInd)
    gis_del__ ();
    return 0;
  }


  if(ActSelTyp != APPTYP_GISPT) goto L_sel_EL;

  //---------------------------------------------------
  // gis-Point selected
  //---------------------------------------------------
    // printf(" gis-Point selected ..\n");

    // decode Object gis-point
    ActPtDli = dl_ind;
    ActPtNr  = dla.ind;         // PointNr

    // display activeSurfNr & activePointNr
    gis_UI_disp_actObj ();

    // display PointCoords in wenvx/y/z
    gis_disp_coords (&ActPtab.data[ActPtNr]);

    // gis-point selected:
    // if(idp != 0) { gis_del__ (); return 0; }   // idp=1 "delete GIS-Point"
    // if(icp != 0) goto L_cre_cadPt0;   // AddPt-Button


  if(gisFunc == GISFUNC_Edit) return 0;    // 1=edit

  // crEdge: add selected gis-point to active IntTab
  if(gisFunc == GISFUNC_crEdge) {
    irc = gis_edg_act_add_ip (&i1);
      printf(" ex _add_ip irc=%d i1=%d\n",irc,i1);
    if(irc < 0) {
      TX_Print ("**** select point %d before ****",i1);
      DL_Redraw ();
      return -1;
    }
    gis_UI_edg_mod ();    // ActEdg.rNr changed
    
  }

  // prjMsh: create cad-point from gis-point
  if(gisFunc == GISFUNC_prjMsh) {gis_cre_cadPt (); return 0;}

  // gis_msg__ (1);  // cannot use object ..
  return 0;


  //---------------------------------------------------
  // gis-Edgeline selected
  L_sel_EL:
  if(dla.iatt != APPMSHIG_EDGLN) {TX_Print("gis_sel_CB I001"); return -1;}
    // printf(" IntTab selected ..\n");

  ActSelTyp = APPMSHIG_EDGLN;

  if(gisFunc == GISFUNC_Edit) {
    gis_UI_disp_actObj ();
    return 0;
  }


  // cpyCad ??
  // gis_del__ ();
  gis_msg__ (1);  // cannot use object ..
  return 0;



  //===================================================
  // cad-obj selected:
  //===================================================
  L_cad_obj__:
  // test if it is a ptab Typ_SURPTAB 149
  if(dla.typ != Typ_SUR) goto L_CadObj0;
    // DL_DumpObj__ (dl_ind);
/*
  ox1 = DB_GetObjGX (dla.typ, dla.ind);
    printf(" typ=%d\n",ox1.typ);
  if(ox1.typ == Typ_Error) {  // SURMSH not yet stored ?
    TX_Print(" dynamic obj ");
    return 0;
  }
    DEB_dump_obj__(Typ_ObjGX, &ox1, "gis sel obj:\n");
*/
  
  //----------------------------------------------------------------
  if(dla.ind == surTab[ActSurInd].dbi_p) {
  // if(ox1.typ == Typ_SURPTAB) {
    // printf(" SURPTAB selected ..\n");
                           


  //----------------------------------------------------------------
  // test if it is a mesh Typ_SURPMSH 160
  } else if(dla.ind == surTab[ActSurInd].dbi_m) {
  // } else if(ox1.typ == Typ_SURPMSH) {
    ActSelTyp = APPTYP_MESH;
    ActSelInd = dla.ind;        // edgeNr

  }

  gis_msg__ (1);  // cannot use object ..
  return 0;



  //================================================================
  // cad-obj selected:
  L_CadObj0:

  //----------------------------------------------------------------
  // cad-point selected:
  if(dla.typ != Typ_PT) goto L_cadObj1;
    // pa1 = DB_get_PT (dla.ind);
    p2 = DB_GetPoint (dla.ind);
      DEB_dump_obj__ (Typ_PT, &p2, " p2");

    // if(idp > 0) return 0; // DelGis-Pt: skip selection of cad-point

    if(gisFunc == GISFUNC_Edit) {            // 1=edit
      gis_disp_coords (&p2);
      gis_PtAdd ();
      goto L_exit;                // Redraw, exit.
    }

    if(gisFunc == GISFUNC_prjMsh) {            // 2=prjMsh
      i1 = MSH_pt_prjptmsh_ (&p1, &p2, ActSurMsh, ActSurPtb, gis_TOL_PT);
      if(i1 < 0) goto L_err_ptOut;
      // pa1 = &p1;
      goto L_cre_cadPt1;
    }
    
    if(gisFunc == GISFUNC_crEdge) {                 // 3=crEdge
      ActPtNr = -1;
      gis_pts_add_e (&ActPtNr, &p2, 1);  // create GIS-point
      // add Gis-point to active IntTab;
      irc = gis_edg_act_add_ip (&i1);
      if(irc < 0) {
        TX_Print ("**** select point %d before ****",i1);
        return -1;
      }
      gis_UI_edg_mod ();    // ActEdg.rNr changed
      goto L_exit;                     // Redraw, exit.
    }

    // was tun mit diesem Punkt ?
    gis_msg__ (1);  // cannot use object ..
    return 0;



  //----------------------------------------------------------------
  L_cadObj1:
    // curve selected: get polygon from DB-object.
    pa1 = (Point*)memspc201;
    p1Siz = sizeof(memspc201) / sizeof(Point);
    p1Nr = p1Siz;
    i1 = UT3D_pta_dbo (&pa1, &p1Nr, dla.typ, dla.ind, UT_DISP_cv);
    if(i1 < 0) return 0;


    // project Obj
    if(gisFunc == GISFUNC_prjMsh) {                    // 2=prjMsh
      // create polygon from inputcurve
      i1 = MSH_npt_prjcvmsh_ (&pa2, &p2Nr, pa1, p1Nr,
                              &ActFtab, &ActPtab, &ActEtab, &DatEtab,
                              &gis_TOL_PT);
      if(i1 != 0) {TX_Print("obj. is not inside mesh .."); return 0;}
      pa1 = pa2;
      p1Nr = p2Nr;
      goto L_cre_CadPlg;
    }


    if(gisFunc == GISFUNC_crEdge) {                    // 3=crEdge
      // create GIS-points from polygon
      iaSiz = p1Nr + 100;  // space for 100 insertpoints
      ia = MEM_alloc_tmp (iaSiz * sizeof(int));
      for(i1=0; i1<p1Nr; ++i1) ia[i1] = -1;  // create new points
      gis_pts_add_e (ia, pa1, p1Nr);
      // add polygon ia to active IntTab ActEdg
      iaNr = p1Nr;
      gis_edg_act_add_ia (ia, &iaNr, iaSiz);
      gis_UI_edg_mod ();    // ActEdg.rNr changed
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

  L_err_sel:
  TX_Print("**** cannot use selection ..");
  return -1;

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

  strcpy(s1, "100 0 0");
  irc = GUI_Dialog_e2b ("move all points - offset:", s1, 80, "OK", "Cancel");
  if(irc != 0) return -1;


    printf("gis_movPts_CB |%s|\n",s1);
    
  // decode text -> point
  UT3D_vc_txt (&vc1, s1);

  // move all points
  gis_ptab_move (&vc1, ActPtab.data, ActPtab.rNr);

  // remove display of all gis-points
  gis_pts_disp_off ();

  // redraw all points
  gis_pts_disp_all (ActPtab.data, ActPtab.rNr, ActSurPtb, 0);

  // redraw all edgelines
  // gis_edg_del_ck2 (&ActEtab, &DatEtab);
  gis_edg_disp_all (&ActEtab);
/*
  for(i1=0; i1 < ActEtab.rNr; ++i1)
    gis_edg_disp__ (&ActEtab.data[i1], i1);
  DL_Redraw ();
*/
  gis_changed_mshMod (1);    // modified; need update.

  return 0;

}


//================================================================
  int gis_del_ptRange_CB () {
//================================================================
// 
 
  int     irc, i1;
  double  d1, d2;
  char    *w, s1[80];


  strcpy(s1, "12-34");
  irc = GUI_Dialog_e2b ("Delete points: pointnumbers / range ",
                        s1, 80, "OK", "Cancel");
  if(irc != 0) return -1;


      // printf("gis_del_ptRange_CB |%s|\n",(char*)data);

    w = s1;

    L_nxt:
    irc = UTX_sget_nrRange (&d1, &d2, &w);
      // printf(" _nrRange %d %f %f\n",irc,d1,d2);
    if(irc == 1) {  // single ptNr
      gis_pts_del__ ((int)d1);

    } else if(irc == 2) {  // range
      for(i1=d2; i1>=d1; --i1) gis_pts_del__ (i1);
    }

    // do not continue; points become renumbered !!
    // if(irc > 0) goto L_nxt;


  return 0;

}

 
//================================================================
  int gis_del__ () {
//================================================================
// gis_del__            delete active group or active gis-obj
// Input:
//   ActSelTyp
//   ActSelInd

  int      i1, ii, iPt, iLast;
  long     dli;
  ObjDB    *gTab;
  DL_Att    dla;


  printf("gis_del__ typ=%d ind=%d\n",ActSelTyp,ActSelInd);


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
    // dla = DL_GetAtt(gTab[i1].dlInd);
    DL_get_dla (&dla, gTab[i1].dlInd);
    if(dla.iatt != APPTYP_GISPT) continue;
    if((i1 > iPt)&&(i1 < iLast)) { iPt = i1; dli = dla.ind;}
  }
  if(iPt >= 0) {
    iLast = iPt;
    gis_pts_del__ (dli);
    goto L_nxtPt;
  }


  // clear group
  Grp_Clear (2);
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
    // gis_PtDelAct ();
    gis_pts_del__ (ActSelInd);


  //----------------------------------------------------------------
  } else if(ActSelTyp == APPMSHIG_EDGLN) {
    MSH_eTab_delRec (&ActEtab, &DatEtab, ActSelInd);
    gis_edg_disp_upd ();
    ActSelTyp = -1;
    gis_UI_disp_actObj (); // remove display


  //----------------------------------------------------------------
  // } else if(ActSelTyp == APPTYP_MESH) {
    // gis_msh_del (1);


  //----------------------------------------------------------------
  } else if(ActSelTyp == -2) {        // crEdge added point(s)
    if(ActEdg.rNr > 0) {
      ActEdg.rNr -= 1;
      gis_edg_act_disp (); // display ActEdg as temp. polygon
      DL_Redraw ();
    }
    if(ActEdg.rNr < 1) {
      gis_UI_edg_mod ();
    }
    return 0;  // do not change ActSelTyp


  //----------------------------------------------------------------
  } else {
    printf("***** gis_del__ E1-%d\n",ActSelTyp);
  }


  ActSelTyp = -1;


  return 0;

}
 

//=====================================================================
  int gis_UI_sur_lst () {
//=====================================================================
// create list of meshes from surTab (womMsh)

  int    i1, sl;
  char   *pBuf[MAX_SURPTAB], cbuf1[40], *cBuf2;


  printf("gis_UI_sur_lst %d\n",surTabNr);
  // gis_dump_surTab ();

  cBuf2 = memspc55;


  // Create a List of Pointers for a list of words.
  for(i1=0; i1<surTabNr;  ++i1) {
    sprintf(cbuf1, "A%ld ", surTab[i1].dbi_m);
    sl = strlen(cbuf1);
    strcpy(cBuf2, cbuf1);
    cBuf2[sl] = '\0';
    pBuf[i1] = cBuf2;
    cBuf2 += sl+1;
  }

  pBuf[surTabNr] = NULL; // terminate List

    i1 = 0;
    while(pBuf[i1]) { printf(" pBuf[%d]=|%s|\n",i1,pBuf[i1]); ++i1;}


  // change surf
  GUI_optmen_chg (&womMsh, pBuf, NULL);


  return 0;

}


//===================================================================
  int gis_EdgTyp_CB (MemObj *mo, void **data) {
//===================================================================
// userselection of listEntry of surfaces
// 0=save IntTab                  elnEL
// 1=save as InnerBoundary          ibdEL
// 2=save OuterBoundary             obdEL
// 3=cancel
  
  char  *cp1;
  
  cp1 = GUI_DATA_S1;
  

  printf("gis_EdgTyp_CB %s\n",cp1);



  //================================================================
  if(!strcmp(cp1, "EL_EL")) {        //  save IntTab (breakline)
     DL_hili_off (-1L);         // unhilte selected obj's
     // check if closed
      ActEdg.use = MSH_EDGLN_BL; // IntTab
      TX_Print("active Polygon changed to IntTab ..");
      gis_edg_mod (TYP_FuncAdd);
      // GUI_MenuCkBoxSet (wcbcre, 0);     // create_polygon off



  //================================================================
  } else if(!strcmp(cp1, "EL_IB")) { //  save as InnerBoundary
      // check if closed
      ActEdg.use = MSH_EDGLN_IB; // InnerBound
      TX_Print("active Polygon changed to InnerBound ..");
      gis_edg_mod (TYP_FuncAdd);
      // GUI_MenuCkBoxSet (wcbcre, 0);     // create_polygon off



  //================================================================
  } else if(!strcmp(cp1, "EL_OB")) { //  save as OuterBoundary
      // check if closed
      ActEdg.use = MSH_EDGLN_OB; // OuterBound
      TX_Print("active Polygon changed to OuterBound ..");
      gis_edg_mod (TYP_FuncAdd);
      // GUI_MenuCkBoxSet (wcbcre, 0);     // create_polygon off



  //================================================================
  } else if(!strcmp(cp1, "EL_ex")) { //  cancel
    gis_edg_act_del ();



  //================================================================
  }

  gis_UI_edg_mod ();    // ActEdg.rNr changed

  return 0;

}


//===================================================================
  int gis_SurLst_CB (MemObj *mo, void **data) {
//===================================================================
// userselection of listEntry of surfaces

  int   lNr;

  lNr = GUI_DATA_I1;

  printf("gis_SurLst_CB A%ld\n",surTab[lNr].dbi_m);


  /// activate surface surTab[lNr]
  gis_sur_off ();     // disactivate surface surTab[ActSurInd]


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


  int    irc, typ, isa, lNr, lLen;
  long   ind1, ind2, dli;
  char   *lSta, *p1, *p2, oid[32];
  DL_Att att1;
  // ColRGB *col1;


  printf("---------- gis_init_pl ----------\n");

  lSta = NULL;
  surTabNr = 0;
  pntTabNr = 0;

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
        printf(" PTAB=|%4.4s| A%ld\n",p1,ind1);
      pntTab[pntTabNr].dbi_p  = ind1;           // DB-Index.
      ++pntTabNr;
      continue;
    }

    // check for MSH-SUR;
    if(!strncmp(p1, "MSH", 3)) {
      // find db-ind of pointSurface
      p1 += 4;
      while(*p1 == ' ') ++p1;
      irc = APED_dbo_oid (&typ, &ind2, p1);
        printf(" MSH=|%3.3s| dbi_m=%ld\n",p1,ind1);
      if(irc != 0) continue;
      if(typ != Typ_SUR) {printf("gis_init_pl E001\n"); continue;}

      // add new surTab-record
      isa = surTabNr;
      gis_surTab_add (ind2, ind1);
      surTab[isa].exi_p = 1;   // 1 = pointSet exists in Model
      surTab[isa].exi_m = 1;   // 1 = mesh exists in Model
      surTab[isa].vis_p = 1;
      surTab[isa].vis_m = 1;
        // printf(" surTab[%d].dbi_m = %d\n",isa,ind2);

      // find the DL-record of the boundary
      surTab[isa].dli_p = DL_find_obj (Typ_SUR, surTab[isa].dbi_p, -1L);

      // find the DL-record of the mesh
      surTab[isa].dli_m = DL_find_obj (Typ_SUR, surTab[isa].dbi_m, -1L);

      // get attributes of mesh
      // att1 = DL_GetAtt (surTab[isa].dli_m);
      DL_get_dla (&att1, surTab[isa].dli_m);
      // get color
      // col1 = (ColRGB*)&att1.iatt;
      surTab[isa].col = *(COL_DL_ATT (&att1));
      // UTcol_dump (&surTab[isa].col);

    }

  }


    // TESTAUSGABEN
    printf("ex gis_init_pl surTabNr=%d pntTabNr=%d\n",surTabNr,pntTabNr);
    // gis_dump_surTab ();
    // gis_dump_pntTab ();
    // exit (0);


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

    // DEB_dump_obj__(Typ_PT, pt1, "ex gis_UI_getPt:");

  return 0;

  L_err1:
  TX_Print("***** coordinate - inputfield empty *****");
  return -1;

}

/*
//================================================================
  int gis_surInd_surNr (long surNr) {
//================================================================
// get index into surTab from DB-index of pointSurface

  int surInd;

  for(surInd=0; surInd<surTabNr;  ++surInd) {
    if(surTab[surInd].dbi_p == surNr) return surInd;
  }

  return -1;

}
*/


//================================================================
  int gis_edg_bnd_disp_off (int isa, int mode) {
//================================================================
// gis_edg_bnd_disp_off hide/redisplay boundary of pointSet
// Input:
//   mode    ON(0) oder OFF(1,hide)


  printf("gis_edg_bnd_disp_off %d %d exi_p=%d\n",isa,mode,surTab[isa].exi_p);


  if(surTab[isa].dli_p >= 0) {
    DL_hide__ (surTab[isa].dli_p, mode);
    // DL_unvis_set (dli, 1);  // set unvis=1
  }

  return 0;

}


//================================================================
  int gis_pts_bnd_disp_del (long dbi, long dli) {
//================================================================
// gis_pts_bnd_disp_del    delete display of boundary of pointSet


  // search in DL typ=Typ_SUR with ind=surTab[iSur].dbi_p
  // if(surTab[iSur].exi_p == 1)

  // if(surTab[iSur].dbi_p < 0) return 0;

  if(dli < 0L) {
    dli = DL_find_obj (Typ_SUR, dbi, -1L);
  }

  printf("gis_pts_bnd_disp_del %ld %ld\n",dbi,dli);

  if(dli >= 0L) GL_Del0 (dli);


  return 0;

}


//================================================================
  int gis_pts_msh_del1 (int isa) {
//================================================================
// gis_pts_msh_del1     delete mesh and pointSet if unused

  int   i1, *iTab,  iNr;
  long  dbm, dbp;
  char  s1[128];


  if(surTabNr < 1) return -1;


/*
  // make list of all meshes to delete
  iNr = 0;
  iTab = (int*) MEM_alloc_tmp (surTabNr * sizeof(int));
  for(i1=0; i1<surTabNr; ++i1) {
    if(surTab[i1].dbi_p != dbp) continue;
    iTab[iNr] = surTab[i1].dbi_m;
    ++iNr;
  }
*/

  // test if pointSet is used in other mesh
  dbp = surTab[isa].dbi_p;
  dbm = surTab[isa].dbi_m;

  // delete active pointSet and all its meshes
  sprintf(s1, "delete mesh A%ld",dbm);

  for(i1=0; i1<surTabNr; ++i1) {
    if(i1 == isa) continue;
    if(surTab[i1].dbi_p == dbp) goto L_go;
  }

  sprintf(&s1[strlen(s1)], " and pointSet A%ld",dbp);


  L_go:
    // GUI_DialogYN (s1, gis_pts_msh_del2);

  // ask user - delete ? yes / no
  i1 = GUI_dlg_2b (s1, MSG_const__(MSG_ok), MSG_const__(MSG_no));
  if(i1 != 0) return -1;            // error or cancel

  gis_pts_msh_del2 ();

  return 0;

}

 
//================================================================
  int gis_pts_del_all (long dbi, long dli, int exi_p) {
//================================================================
// gis_pts_del_all         remove all Points of PTAB
// Input:
//   dbi      dbi of pointSet (A#=PTAB)
//   exi_p    did pointSet exist on prgStart; 0=no, 1=yes

// see also gis_pts_del__ MSH_bdel_pTab gis_sur_off


  printf("gis_pts_del_all %ld\n",dbi);

  // remove display of all gis-points
  gis_pts_disp_off ();

  // delete display of boundary of pTab
  gis_pts_bnd_disp_del (dbi, dli);

  // delete "A#" from Model
  if(exi_p != 0) gis_mdl_del_pTab (dbi);

  ActPtab.rNr = 0;


  return 0;

}


//================================================================
  int gis_pts_msh_del2 () {
//================================================================
// delete active mesh and pointSet if unused

  int     i1, isa, exi_p;
  long    dbp, dlp, dbm;


  printf("gis_pts_msh_del2 %d\n",ActSurPtb);


  // test if pointSet is used in other mesh
  dbp = surTab[ActSurInd].dbi_p;
  dlp = surTab[ActSurInd].dli_p;
  exi_p = surTab[ActSurInd].exi_p;
  dbm = surTab[ActSurInd].dbi_m;


  for(i1=0; i1<surTabNr; ++i1) {
    if(i1 == ActSurInd) continue;  // skip record to delete
    if(surTab[i1].dbi_p == dbp) {
      exi_p = -1;  // do not delete pointSet
      break;
    }
  }



  //----------------------------------------------------------------
  // delete ActEdg & alle IntTabs
  gis_edg_del_all ();


  // delete mesh
  gis_msh_del (ActSurInd);




  //----------------------------------------------------------------
  // delete pTab
  if(exi_p >= 0) gis_pts_del_all (dbp, dlp, exi_p);




  //----------------------------------------------------------------
  L_exit:
  // if no mesh exists, init one
  if(surTabNr < 1) gis_ptab_init ();

  // update surf-list
  gis_UI_sur_lst ();

  // activate last surf
  gis_sur_on (surTabNr-1);

  DL_Redraw ();

  return 0;

}

 
//================================================================
  int gis_msh_disp_del (int isa) {
//================================================================
// gis_msh_disp_del     delete mesh-display

  long   dli;


  dli = surTab[isa].dli_m;

  printf("gis_msh_disp_del %d %ld\n",isa,dli);



  if(dli < 0L)  return -1;

  GL_Del0 (dli);

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
  gis_pts_del__ (ActPtNr);


  // update Labels; ActPtNr-display
  ActSelTyp = -1;
  ActPtNr = -1;
  gis_UI_disp_actObj ();  // display labelext wlbap & coords


  return 0;

}

 
//================================================================
  int gis_pts_del__ (int iDelPt) {
//================================================================
// delete Point Nr iDelPt from surface ActSurPtb

  int      i1, iLastPt;
  long     l1;
  char     info[20];
  IntTab *el1;


  printf("gis_pts_del__ %d\n",iDelPt);

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
    gis_pts_disp__ (&l1, SYM_TRI_S, &ActPtab.data[i1], 0, 1, info);



  // ------------------------------------------
  // reset stat in all IntTabs
  L_reset:
  gis_edg_ini_stat (&ActEtab, &DatEtab);

  // delete point iDelPt in all edgelines
    // MSH_dump_eTab (&ActEtab, &DatEtab);           // TEST ONLY
  MSH_EL_del_pt (&ActEtab, &DatEtab, iDelPt);
    // MSH_dump_eTab (&ActEtab, &DatEtab); //return 0; // TEST ONLY
  // die pointers ActEtab[].ia neu aufbauen
  MSH_eTab_fix (&ActEtab, &DatEtab);



  // change iLastPt into iDelPt in all edgelines
  MSH_EL_chg_pt (&ActEtab, &DatEtab, iDelPt, iLastPt);
    // MSH_dump_eTab (&ActEtab, &DatEtab); //return 0; // TEST ONLY


  // delete edgelines with less than 2 points; update display
  gis_edg_del_ck2 (&ActEtab, &DatEtab);
    // MSH_dump_eTab (&ActEtab, &DatEtab); return 0; // TEST ONLY
  // -------------------------------------------


  // free point
  ActPtab.data[iLastPt].x = UT_VAL_MAX;


  DL_Redraw ();

  // modified; need update.
  gis_changed_mshMod (1);
 
  return 0;

}


//================================================================
  int gis_PtMod () {
//================================================================
// modify Point Nr ActPtNr from surface ActSurPtb

  int      i1;
  Point    pt1;
  IntTab *el1;


  printf("gis_PtMod %d %d %ld\n",ActSurPtb,ActPtNr,ActPtDli);


  if(ActPtNr < 0) {
    TX_Print(" select point to modify ..");
    return 0;
  }


  // get point pt1 from UI
  i1 = gis_UI_getPt (&pt1);
  if(i1 < 0) return -1;


  // test if point already exists; (2D-Test; tolerance = gis_TOL_PT)
  // ActPtab.data[ActPtNr].x = UT_VAL_MAX;  // cannot find this point as equal

  // modify & view point
  // get ActPtNr = pointIndex
  gis_pts_add_e (&ActPtNr, &pt1, 1);


// TODO: test if point to be modified is removed ...

  // update ActPtNr-display (pt-nr and coordinates)
  ActSelTyp = APPTYP_GISPT;
  gis_UI_disp_actObj ();

  gis_pts_disp_1 (0, ActPtNr);

  DL_Redraw ();


  // TX_Print("point %d modified ..",ActPtNr);

  return 0;

}


//================================================================
  int gis_ptab_init () {
//================================================================
// create new pointSet and new mesh
// see also gis_msh_cr

 
  int  isa;  // index surTab
  int  ips;  // index pointSet


  printf(" gis_ptab_init\n");


  // surNr = find next free surface-nr (A) for new PTAB
  if(pntTabNr < 1) {
    // create new pointSet
    ++surIndLast;
    // dbiNew = surIndLast;
    ActSurPtb = surIndLast;
    ActPtab.rNr = 0;
      printf(" init new ActSurPtb=%d\n",ActSurPtb);
    ips = -1;

  } else {
    // activate last loaded pointSet
    ips = pntTabNr - 1;
    ActSurPtb = pntTab[ips].dbi_p;
  }



  ActEtab.rNr = 0;
  DatEtab.rNr = 0;
  ActEdg.rNr = 0;
  ActFtab.rNr = 0;


  // get index for new mesh MSH
  ++surIndLast;
  ActSurMsh = surIndLast;

  // add surf to surTab RecNr surTabNr
  isa = surTabNr;
  gis_surTab_add (ActSurPtb, ActSurMsh);


  if(ips >= 0) {
    surTab[isa].exi_p = 1;                         // pointSet exists
    // find the DL-record of the boundary
    surTab[isa].dli_p = DL_find_obj (Typ_SUR, surTab[isa].dbi_p, -1L);
  }



  // add surf to List (update womMsh)
  gis_UI_sur_lst ();

  TX_Print("new pointSet A%d initialized and mesh A%d; add points ..",
           ActSurPtb,ActSurMsh);

  return 0;

}


//================================================================
  int gis_PtAdd () {
//================================================================
// add Point to surface ActSurPtb
// see also gis_edg_act_add_ip

  int    i1;
  Point  pt1;


  printf("gis_PtAdd %d %d\n",ActSurPtb,ActPtab.rNr);

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
  // get ActPtNr = pointIndex
  gis_pts_add_e (&ActPtNr, &pt1, 1);

  // update ActPtNr-display
  ActSelTyp = APPTYP_GISPT;
  gis_UI_disp_actObj ();

  DL_Redraw ();

  TX_Print("point %d added..",ActPtNr);

  return 0;

}

/*
//================================================================
  int gis_etab_save (char *fnam, char *dirNam) {
//================================================================
// gis_etab_save        Export edgelines into Ascii-file
// TODO: use gis_edg_fwrite
  
  int        i1, i2, *iea;
  char       cbuf[256];
  FILE       *fpo;
  IntTab   *ela;
  Point      p1, p2;
  
  if(fnam == NULL) return 0;
  

  printf("gis_etab_save |%s|%s|\n",dirNam,fnam);


  strcpy(cbuf, dirNam);
  UTX_add_slash (cbuf);
  strcat(cbuf, fnam);
    printf(" fn=|%s|\n",cbuf);


  if((fpo=fopen(cbuf,"w")) == NULL) {
    TX_Print("gis_etab_save E001 %s",fnam);
    return -1;
  }

  for(i1=0; i1<ActEtab.rNr; ++i1) {
    ela = &ActEtab.data[i1];
    if(ela->typ != MSH_EDGLN_BL) continue;
    iea = ela->ia;
    p1 = ActPtab.data[ela->ia[0]];
    UT3D_pt_add_pt2 (&p1, &gis_origin);
    // fprintf(fpo,"%lf %lf %lf ", pt1->x, pt1->y, pt1->z);
    for(i2=1; i2<ela->iNr; ++i2) {
        printf("    i[%d]=%d\n",i2,ela->ia[i2]);
      p2 = ActPtab.data[ela->ia[i2]];
      UT3D_pt_add_pt2 (&p2, &gis_origin);
      fprintf(fpo,"%lf %lf %lf  %lf %lf %lf\n",
         p1.x, p1.y, p1.z,  p2.x, p2.y, p2.z);

      p1 = p2;
    }
  }

  fclose(fpo);

    
  return 0;

}
*/

//================================================================
  int gis_ptab_save (char *fnam, char *dirNam) {
//================================================================
// Export points into Ascii-file


  int   i1;
  char  cbuf[256];
  FILE  *fpo;

  if(fnam == NULL) return 0;


  printf("gis_ptab_save |%s|%s|\n",dirNam,fnam);


  // // unload active surf
  // gis_bload_pTab (TYP_FuncEnd);
  // // load points > pTab
  // gis_bload_pTab (TYP_FuncInit);


  if(ActPtab.rNr < 1) {
    TX_Print(" pTab not active ..");
    return 0;
  }


  strcpy(cbuf, dirNam);
  UTX_add_slash (cbuf);
  strcat(cbuf, fnam);
    printf(" fn=|%s|\n",cbuf);


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

  TX_Print("%d points exported into file %s",ActPtab.rNr,cbuf);

  return 0;

}


//================================================================
  int gis_sur_on (int surInd) {
//================================================================
// activate surface surTab[surInd]

  int        irc;
  long       dli, dbi;
  char       s1[32];


  printf("gis_sur_on surInd=%d act=%d\n",surInd,ActSurInd);
  // gis_dump_surTab ();

  // if no surf exists
  if(surInd < 0) {
    ActSurInd  = -1;
    ActSurPtb  = -1;
    ActSurMsh  = -1;
    ActPtNr    = -1;
    // gis_UI_disp_actObj ();
    return 0;
  }
  

  // load pTab from bin.file for surf. ActSurPtb > ActPtab
  ActSurInd = surInd;
  ActSurPtb = surTab[surInd].dbi_p;
  ActSurMsh = surTab[surInd].dbi_m;
  // if(surTab[surInd].exi_p == 0) goto L_2;



  // Load pointSet -> ActPtab from file A<ActSurPtb>
  // irc = gis_bload_pTab (TYP_FuncInit);
  // irc = MSH_bload_pTab (&ActPtab, -1, ActSurPtb);
  irc = MSH_bload_pTab (&ActPtab, AP_modact_nam, ActSurPtb);
  if(irc < 0) goto L_2;

  // find & subtract origin
  gis_ori__ (ActPtab.data, ActPtab.rNr, 0);


  // load ActBtab
  // gis_bnd_read ();


  // display points
  gis_pts_disp_all (ActPtab.data, ActPtab.rNr, ActSurPtb, 0);
  // surTab[surInd].visi = 1;


  // remove display of new pTab-boundary ActSurPtb
  gis_edg_bnd_disp_off (surInd, 1);    // 1=unvis
  // ActPtabBndModFlag = 0;



  //----------------------------------------------------------------
  // change display of new Mesh to symbolic
  dbi = surTab[surInd].dbi_m;
  if(dbi < 1) goto L_2;
    // printf(" m-new-dbi=%ld\n",dbi);
    // printf(" dbi=%ld  dli=%ld\n",dbi,dli);

  // load fTab & eTab
  // MSH_bload_fTab (&ActFtab, &ActEtab, &DatEtab, -1, dbi);
  MSH_bload_fTab (&ActFtab, &ActEtab, &DatEtab, AP_modact_nam, dbi);

/*
  // if mesh already exists, just modify ..
  if(surTab[surInd].exi_m > 0) {
    // find dli of A<dbi>
    dli = DL_find_obj (Typ_SUR, dbi, -1L);
    if(dli < 0) goto L_2;
    GA_sStyl__ (dli, 2, Typ_SUR, dbi);    // change attrib & redraw


  } else {
*/
    // disp mesh 1=symbolic
    gis_msh_disp (1, surInd);
  // }


  // reset, unmodified.
  gis_changed_mshMod (0);


  //----------------------------------------------------------------
  // display IntTabs
  gis_edg_disp_all (&ActEtab);


  //----------------------------------------------------------------
  L_2:
  // activate actual surface in list

  sprintf (s1, "A%d", ActSurPtb);
  GUI_label_mod (&wlsPtb, s1);

  sprintf (s1, "A%d", ActSurMsh);
  GUI_label_mod (&wlsMsh, s1);

  GUI_optmen_set (&womMsh, 1, s1);


  // display new activepointTab in Label
  ActSelTyp = -1;
  ActPtNr = -1;
  gis_UI_disp_actObj ();

  // gis_changed_visi ();  // lock prjMsh

  return 0;

}


//================================================================
  int gis_sur_off () {
//================================================================
// disactivate surface surTab[ActSurInd]
// display mesh shaded; delete edgeLines, keep pTab-outline, remove gisp-points.
// Input:
//   ActSurInd


  long       dli, dbi;


  printf("gis_sur_off isa=%d\n",ActSurInd);
  // gis_dump_surTab ();


  if(ActSurInd < 0) return 0;


  // unload mesh (save mesh -> file)
  if((ActFtab.data)    &&
     (ActFtab.rNr > 0))    {
     MSH_bsav_fTab (&ActFtab, &ActEtab, &DatEtab,
                     AP_modact_nam, surTab[ActSurInd].dbi_m);
     // write ActBtab
     // gis_bnd_write ();

  } else {
     // delete binary faceTableFile
     MSH_bdel_fTab (AP_modact_nam, surTab[ActSurInd].dbi_m);
  }


  // unload active surf-points (save pointSet -> file)
  if(ActPtab.data) {
    surTab[ActSurInd].ptNr = ActPtab.rNr;
    // add origin 
    gis_ori__ (ActPtab.data, ActPtab.rNr, 2);
    // save pointSet -> file
    MSH_bsav_pTab (&ActPtab, AP_modact_nam, surTab[ActSurInd].dbi_p);
  }
  

  //----------------------------------------------------------------
  // delete ActEdg & alle IntTabs
  gis_edg_del_all ();

/*
  // remove ActEdg
  GL_temp_del_all ();
  // remove the Display of all IntTabs
  gis_edg_disp_off (&ActEtab);
  // delete Edglines
  ActEtab.rNr = 0;
  DatEtab.rNr = 0;
*/


  //----------------------------------------------------------------
  // remove display of GIS-points
  gis_pts_disp_off ();



  //----------------------------------------------------------------
  // update pTab-boundary;
  // if ptab is unchanged: make it visible again; else recreate it.
  gis_pts_bnd_disp__ (ActSurInd);
  gis_edg_bnd_disp_off (ActSurInd, 0);   // 0=visible


  //----------------------------------------------------------------
  // change display of active mesh to shaded
  if(ActSurInd < 0) goto L_1;


/*
  dbi = surTab[ActSurInd].dbi_m;
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
      gis_msh_disp (0, ActSurInd);
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

*/

  // display or redisplay shaded
  gis_msh_disp (0, ActSurInd);




  //----------------------------------------------------------------
  L_1:
  ActSurInd = -1;
  return 0;

}


/* UNUSED
//================================================================
  int gis_etab_load (char *fnam, char *dirNam) {
//================================================================
// gis_etab_load           import edgelinefile
// see also AP_ImportXML
// Format of file:  6 numbers = startline,
//                  3 numbers = following segment.
//                  commentlines start with '#'

  int       irc, i1, ptNr, oldptNr, lNr, pa1Nr, pa1Siz;
  long      surNr;
  char      cbuf[256], *cp;
  FILE      *fpi;
  Point     pt1, pt2, *pa1;


  if(fnam == NULL) return 0;


  // get pa = memspc for points
  pa1 = (Point*)memspc201;
  pa1Siz = sizeof(memspc201) / sizeof(Point);



  strcpy(cbuf, dirNam);
  UTX_add_slash (cbuf);
  strcat(cbuf, fnam);

    printf("gis_etab_load |%s|\n",cbuf);

  TX_Print("import points from file %s",cbuf);


  if ((fpi = fopen (cbuf, "r")) == NULL) {
    TX_Print("FILE NOT FOUND |%s| in gis_etab_load",cbuf);
    return -1;
  }


  lNr = 0;
  oldptNr = 0;
  
  pa1Nr = 0;
  while (!feof (fpi)) {

    if(fgets (cbuf, 250, fpi) == NULL) break;
    UTX_CleanCR (cbuf);    // cut off CR,LF 
    cp = cbuf;
    while (*cp == ' ') ++cp; // skip leading blanks
    if(strlen(cp) < 6) continue;
    if(*cp == '#') continue;



    // read 3 or 6 numbers
    pa1[pa1Nr].x = strtod (cp, &cp);
    pa1[pa1Nr].y = strtod (cp, &cp);
    pa1[pa1Nr].z = strtod (cp, &cp);
      DEB_dump_obj__ (Typ_PT, &pa1[pa1Nr], "pt1");
    ++pa1Nr;
    ptNr = 1;

    if(strlen(cp) > 5) {
      pa1[pa1Nr].x = strtod (cp, &cp);
      pa1[pa1Nr].y = strtod (cp, &cp);
      pa1[pa1Nr].z = strtod (cp, &cp);
        DEB_dump_obj__ (Typ_PT, &pa1[pa1Nr], "pt2");
      ++pa1Nr;
      ptNr = 2;
      // store point
    }


    // if active segment has 2 points and last segment had only 1 point
    //   and if first point is not identic with old endpoint: create edgeline
    if((ptNr == 2)&&(oldptNr == 1)) {
      // create edgeline from points pa1
      gis_edg_add_pts (pa1, pa1Nr - 2);
      // move new points -> 0,1
      pa1[0] = pa1[pa1Nr - 2];
      pa1[1] = pa1[pa1Nr - 1];
      pa1Nr = 2;
    }

    oldptNr = ptNr;
    ++lNr;
  }

  fclose(fpi);

  if(ptNr >= 2)
    // create edgeline
    gis_edg_add_pts (pa1, pa1Nr);


  TX_Print("%d edgelines imported ..",ActEtab.rNr);

  return 0;

}
*/


//================================================================
  int gis_ptab_fload (char *fnam) {
//================================================================

  int       irc;


   // load points > pTab
  // irc = gis_pts_add_fil1 (pTab, &ptNr, 0, cbuf);
  // find gis_origin
  irc = gis_pts_add_fil1 (&ActPtab, 0, fnam);
  if(irc < 0) return -1;


  // create point ActPtab.rNr from Display
  // hier fehlt noch PunktIndexOffset !
  gis_pts_disp_all (ActPtab.data, ActPtab.rNr, ActSurPtb, 0);
  // surTab[ActSurInd].visi = 1;


  // update ActPtNr-display
  ActSelTyp = -1;
  ActPtNr = -1;
  gis_UI_disp_actObj ();

  // modified ..
  gis_changed_mshMod (1);

  DL_Redraw ();

  return 0;

}


//================================================================
  int gis_ptab_load (char *fnam, char *dirNam) {
//================================================================
// user has pointfile selected;
// see also AP_ImportXML

  int       irc;
  char      cbuf[256];


  if(fnam == NULL) return 0;


  strcpy(cbuf, dirNam);
  UTX_add_slash (cbuf);
  strcat(cbuf, fnam);


    // printf("gis_ptab_load |%s|\n",cbuf);

  TX_Print("importing %s",cbuf);

  // gis_ptab_init ();


  // load points > pTab, display; msh=modified.
  irc = gis_ptab_fload (cbuf);


  TX_Print("%d points imported ..",ActPtab.rNr);

  return irc;

}


//================================================================
  int gis_ptab_move (Vector *vc1, Point *pTab,  int ptNr) {
//================================================================
// gis_ptab_move        move all points

  int   i1;

  DEB_dump_obj__ (Typ_VC, vc1, "gis_ptab_move");

  for(i1=0; i1<ptNr; ++i1) {
    UT3D_pt_traptvc (&pTab[i1], &pTab[i1], vc1);
  }

  return 0;

}


//================================================================
  int gis_pts_disp_1 (int iNew, int pNr) {
//================================================================
// display point ActPtab.data[pNr]
// inew == 0   point is in dispList; use existing DL-record
// See also MSH__disp_pt__

  long   dli;

  // printf("gis_pts_disp_1 inew=%d pNr=%d ActPtDli=%ld\n",iNew,pNr,ActPtDli);

  if(!iNew) {
    // point did exist; modify, not new ..
    if(ActPtDli < 0) dli = DL_find_obj (Typ_APPOBJ, pNr, -1L);
    else  dli = ActPtDli;
    DL_SetInd (dli);
  }
  
  // disp this point
  gis_pts_disp_all (&ActPtab.data[pNr], 1, ActSurPtb, pNr);

  return 0;

}
 

//=======================================================================
  int gis_pts_disp_all (Point *pTab,  int ptNr, int surNr, int iOff) {
//=======================================================================
// gis_pts_disp_all           display all gis-points
// Input:
//   iOff  the nr of the first point
// DispList:
//   typ   ApplicationObject (Typ_APPOBJ = 20)
//   dbi_p   SurfNr (A#)
//   iatt  pointNr (index in pTab)

// Wie soll Bezug auf die Application eingebracht werden ?
// Gis hat viele ApplicationObjectTypes ..


  int    i1, icol1, icol2;
  long   l1;
  char   info[20];


  // printf("gis_pts_disp_all ptNr=%d surNr=%d iOff=%d\n",ptNr,surNr,iOff);

  icol1 = 0;
  icol2 = 1;

  for(i1=0; i1<ptNr; ++i1) {
    sprintf(info, "%d", i1+iOff);   // itoa
    // save objtyp APPTYP_GISPT as att
    // save pointNr as ind
    // l1 = DL_StoreObj (Typ_APPOBJ, surNr, i1+iOff);
    l1 = DL_StoreObj (Typ_APPOBJ, i1+iOff, APPTYP_GISPT);
    gis_pts_disp__ (&l1, SYM_TRI_S, &pTab[i1], icol1, icol2, info);
  }
  ActPtDli = l1;

  return 0;

}



//================================================================
  int gis_pts_disp__ (long *dli, int symTyp, Point *pt1,
                      int icol1, int icol2, char *info) {
//================================================================
// gis_pts_disp__            display single gis-point
// DL-record: typ=Typ_APPOBJ Ind=pointNr; att=0=APPTYP_GISPT

  GLuint dlInd;


  // printf("gis_pts_disp__ %ld %d |%s|\n",*dli,symTyp,info);
  // DEB_dump_obj__ (Typ_PT, pt1, "pt1");



  // den DL-Index (+ Offset) holen)
  dlInd = GL_fix_DL_ind (dli);
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

    // printf("ex gis_pts_disp__ ogl-dli=%d\n",dlInd);
 
  return 0;

}

 
//================================================================
  int gis_pts_disp_off () {
//================================================================
// remove display of GIS-points

  printf("gis_pts_disp_off\n");

  GL_Del_om (Typ_APPOBJ, -1L, APPTYP_GISPT);  // del multiple objects from DL

  // too many points ..
  GL_Regen1 ();                        // reset DL
  
  return 0;

}


//================================================================
  int gis_edg_disp_upd () {
//================================================================

  // remove the Display of all IntTabs
  gis_edg_disp_off (-1L);

  // draw all (remaining) Edgelines
  gis_edg_disp_all (&ActEtab);

  // ActFtabModFlag = 1;
  // mesh modified; need update.
  gis_changed_mshMod (1);

  return 0;

}

 
//================================================================
  int gis_edg_disp_all (MemTab(IntTab) *eTab) {
//================================================================
// gis_edg_disp_all     (re)display all edgeLines (not mesh-boundary)

  int    i1;

  // printf("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE \n");
  // printf("gis_edg_disp_all %d\n",eTab->rNr);

  // delete all edgelines
  GL_Del_om (Typ_APPOBJ, -1L, APPMSHIG_EDGLN);

  // recreate all edgelines
  for(i1=0; i1<eTab->rNr; ++i1) {
    // if(eTab->data[i1].typ == MSHIG_EDGLN_AB) continue; // skip automatic bound
    if(eTab->data[i1].typ == MSH_EDGLN_AB) continue; // skip automatic bound
    gis_edg_disp__ (&eTab->data[i1], i1);
  }
  DL_Redraw ();


  return 0;

}


//================================================================
  int gis_edg_disp__ (IntTab *el1, int ind) {
//================================================================
// gis_edg_disp__       display IntTab
// Input
//   ind    index in eTab; is stored in DL
// see gis_pts_disp__ gis_edg_disp_cv

  int    iAtt, iClo, eTyp;
  long   l1;
  GLuint dlInd;
  Point  *pa;

  static unsigned char atta[] = {9, 5,10, 7, 8, 7, 8};  // color see GR_tDyn_ln2_2pt
                             //  0  1  2  3  4
                             //     BL AB IB OB IC OC


  eTyp = el1->typ;

  // printf("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE \n");
  printf("gis_edg_disp__ ind=%d typ=%d\n",ind,eTyp);
  // MSH_dump_EL (el1, "_disp__");
  

  // iAtt = 9;
  iAtt = atta[eTyp];
  pa = ActPtab.data;


  // BL=open, IB|OB=closed
  // BL=ActPtab, IB|OB=ActBtab
  if(eTyp > MSH_EDGLN_BL) {
    iClo = 1; // 0=open; 1=closed
    // pa = ActBtab.data;
  } else {
    iClo = 0;
    // pa = ActPtab.data;
  }

  if(!pa) {TX_Print("**** gis_edg_disp__ E001 %d",ind); return -1;}

  // test if this breakline already exists ..
  // yes: overwrite; else create ..
  l1 = DL_find_APPOBJ (ind, APPMSHIG_EDGLN);
  if(l1 >= 0)
  DL_SetInd (l1);       // modify (do not create new DL-Record)
  l1 = DL_StoreObj (Typ_APPOBJ, ind, APPMSHIG_EDGLN);
    // printf(" l1=%ld\n",l1);


  gis_edg_disp_cv (l1, el1->ia, el1->iNr, pa, iAtt, iClo);

  return 0;

}


//================================================================
  int gis_edg_disp_del__ (long dbi) {
//================================================================
/// gis_edg_disp_del__   delete display of IntTab

  long   l2;


  l2 = DL_find_APPOBJ (dbi, APPMSHIG_EDGLN);

  // del dlObj
  if(l2 >= 0) GL_Del0 (l2);

  return 0;

}


//================================================================
  int gis_edg_disp_del_all () {
//================================================================
// gis_edg_disp_del_all    remove display of all edgeLines


  // remove display of ActEdg (gis_edg_act_del)
  GL_temp_del_all ();

  // activate "reCreate mesh"-button
  gis_UI_edg_mod ();

  // remove the Display of all IntTabs
  gis_edg_disp_off (-1L);

  // delete all lines
  GL_Del_om (Typ_APPOBJ, -1L, APPTYP_LINE);

  return 0;

}


//================================================================
  int gis_edg_disp_off (long  ind) {
//================================================================
// gis_edg_disp_off     remove display of edgeLines
// Input:
//   ind       index of edgeline to remove;
//             -1    remove all edgelines


  printf("gis_edg_disp_off %ld\n",ind);

  DL_hili_off (-1L);    // unhilite all

  GL_Del_om (Typ_APPOBJ, ind, APPMSHIG_EDGLN);  // del multiple objects from DL
  // GL_Regen1 ();                        // reset DL

  return 0;

}


//=========================================================
  int gCad_fini () {
//=========================================================
// dll being unloaded - reset Input, kill all open windows

  int    i1, ii;
  long   dbi, dli;


  printf("gCad_fini GIS1\n");
  // GUI_obj_dump_mo (&tbGis1);

  // return 0;


  // do nothing, if already unloaded.
  if(!GUI_OBJ_IS_VALID(&tbGis1)) return 0;

  // gis_dump_surTab ();


  // write to Main-Infowindow ..
  TX_Print("...... gCad_fini GIS1");




    //----------------------------------------------------------------
    // disactivate surface surTab[ActSurInd]
    gis_sur_off ();

    // display shaded

    // create update / Modelcodes
    // nur Modelcodes korrigieren; displayKorrektur done by gis_sur_off
    gis_mdl_upd ();



    //----------------------------------------------------------------
    // free memory:
    // free memSpace for active IntTab
    gis_mem_free ();

    // remove the toolbar tbGis1
    tbGis1 = GUI_toolbox_del (&tbGis1);


  // reset  select (give back to mainApp)
  AP_User_reset ();    // 2010-11-10
  // PLU_close ();

    // printf("ex gCad_fini \n");


  ED_work_CurSet (UT_INT_MAX);   // work until end of model    2010-11-29

  UI_block__ (0, 0, 0);

  sele_restore ();         // restore saved selectionFilter

  DL_hili_off (-1L);
  GL_Regen1 ();            // reset DL
  DL_Redraw ();


  return 0;

}


//================================================================
  int gis_mdl_msh_del (long dbi) {
//================================================================
// gis_mdl_msh_del           delete mesh-modelcode and display

  int      irc, ll;
  long     lNr;
  char     *lPos, cbuf[64];


  printf("gis_mdl_msh_del %ld\n",dbi);


  APED_oid_dbo__ (cbuf, Typ_SUR, dbi);


  // search defineition-line
  irc = APED_search_defLn (&lPos, &lNr, &ll, cbuf, -1L, 0);
    // printf(" irc=%d lNr=%d |%s|\n",irc,lNr,cbuf);
  if(irc < 0) return -1;

  // delete line in model
  UTF_del_line1 (lNr);

  // // APED_undo_do (lNr, lPos, 1);
  // // APED_update__ (lNr);
  // UNDO_grp_add (lNr, 0);        // add to undoTab
  // UNDO_grp_del ();              // delete last active grp & update display

  // delete meshfile    sonst geht das UNDO ned ..
  // MSH_bdel_fTab (WC_modnr, surTab[isa].dbi_m);

  return 0;

}
 

//================================================================
  int gis_mdl_del_pTab (long dbi) {
//================================================================
// delete pTab from model (del A<surInd> - Line)

  int       irc, ll;
  long      lNr;
  char      *lPos, cbuf[64];

  
  printf("gis_mdl_del_pTab %ld\n",dbi);

  

  // no mesh to delete - did not exist before
  // if(surTab[surInd].exi_p == 0) return 0;


  // points deleted ..
  // if(surTab[surInd].dbi_p < 0) return 0;
  if(dbi < 0) return 0;

    // surNr = surTab[surInd].dbi_p;

    APED_oid_dbo__ (cbuf, Typ_SUR, dbi);
    // search defineition-line
    irc = APED_search_defLn (&lPos, &lNr, &ll, cbuf, -1, 0);
      // printf(" irc=%d lNr=%d |%s|\n",irc,lNr,cbuf);
    if(irc < 0) return -1;

    // delete line in model
    UTF_del_line1 (lNr);


    // UNDO_grp_add (lNr, 0);        // add to undoTab
    // UNDO_grp_del ();                 // delete last active grp & update display

    // delete bin. pointFile (wird sonst wieder ins Model gespeichert)
    // cannot redo if deleted ..
    // MSH_bdel_pTab (WC_modnr, surTab[surInd].dbi_p);

  return 0;

}


//================================================================
  int gis_mdl_add_msh (int iSur) {
//================================================================
// gis_mdl_add_msh     add A1=MSH A<pointSurfNr>   to model

  char      cbuf[80];

  printf("gis_mdl_add_msh %d\n",iSur);

  sprintf(cbuf, "A%ld=MSH A%ld",surTab[iSur].dbi_m,surTab[iSur].dbi_p);
    printf(" add|%s|\n",cbuf);
  DL_SetInd (surTab[iSur].dli_m);  // use existing DL-record
  ED_srcLn_add (cbuf, 0);

  surTab[iSur].exi_m = 1;

  return 0;

}


//================================================================
  int gis_mdl_add_ptab (int iSur) {
//================================================================
// gis_mdl_add_ptab     add A1=PTAB   to model

  char      cbuf[80];


  // printf("gis_mdl_add_ptab %d\n",iSur);

  sprintf(cbuf, "A%ld=PTAB",surTab[iSur].dbi_p);
    // printf("gis_mdl_add_ptab |%s|\n",cbuf);
  DL_SetInd (surTab[iSur].dli_p);  // use existing DL-record
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


  printf("gis_mdl_upd %d\n",surTabNr);
  // gis_dump_surTab ();
  // DL_DumpObjTab ();


  for(i1=0; i1<surTabNr;  ++i1) {

/*
    //----------------------------------------------------------------
    // remove the display of pTab-Surface that did not exist before
    if(surTab[i1].exi_p == 0) {    // did not exist before
      dli = DL_find_obj (Typ_SUR, surTab[i1].dbi_p, -1L);
      if(dli >= 0) GL_Del0 (dli);
    }
*/

    //----------------------------------------------------------------
    // update pTab-Surface
    // add A1=PTAB "pointfilename" to model;
    // pointfilename is unused !
    // sprintf(cbuf, "A%d=PTAB \"%s\"",surNr,fnam);
    if(surTab[i1].exi_p == 0) {    // did not exist before
      if(surTab[i1].dbi_p >= 0) {
        if(surTab[i1].ptNr > 3) {
          gis_mdl_add_ptab (i1);  // create "A#=PTAB"
        }
      }
    }


/*
    //----------------------------------------------------------------
    // remove the display of Mesh-surface that did not exist before
    if(surTab[i1].exi_m == 0) {    // did not exist before
      dli = DL_find_obj (Typ_SUR, surTab[i1].dbi_m, -1L);
      if(dli >= 0) GL_Del0 (dli);
    }
*/

    //----------------------------------------------------------------
    // update Mesh-surface
    if(surTab[i1].exi_p == 0) goto L_exists; // no pTab -no Mesh
    if(surTab[i1].dli_m < 0) goto L_exists;  // no mesh created yet 
    if(surTab[i1].exi_m == 0) {
      // add A1=MSH A<pointSurfNr>   to model;
      gis_mdl_add_msh (i1);  // create "A#=MSH A#"
    }
    goto L_ex_msh;


    //----------------------------------------------------------------
    // mesh did exist; check for delete
    L_exists:
/*
      if(surTab[i1].visi == 0) {     // not visible = deleted !
        // delete Mesh from model (deletes also its display)
        // gis_mdl_del_msh (i1);
        gis_mdl_msh_del ((long)surTab[i1].dbi_m);
      }
*/

 
    L_ex_msh:
    continue;

  }


  return 0;

}


/*
//====================================================================
  int ptab_cad_load1 (Point *pTab, int *ptNr, Edg3 *lTab, int *lnNr) {
//====================================================================
// get all points and all lines from DB
// Input:
//   DB
// Output:
//   pTab
//   ptNr    nr of points in pTab
//   lTab


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

  // get all points from DB
  *ptNr = 0;
  for(dbi=1; dbi<=dbMax; ++dbi) {
    if(DB_QueryDef(Typ_PT, dbi) < 0) continue;
    pt1 = DB_get_PT (dbi);
    // add pt1 to pTab
    ip1 = gis_pts_add__ (pt1, pTab, ptNr, ptMaxNr, tol);
    if(ip1 < 0) break;
  }



  // Linien (breaklines) ex Db holen ...
  // neue Punkt an pTab anhaengen ..
  // ptMaxNr = *ptNr;

  // get next free LN-index (end of LN-DB)
  dbMax = DB_dbo_get_free (Typ_LN);
    // printf(" LN-dbMax=%ld\n",dbMax);

  // get all Lines from DB
  i1 = 0;
  for(dbi=1; dbi<=dbMax; ++dbi) {
    if(DB_QueryDef(Typ_LN, dbi) < 0) continue;
    ln1 = DB_get_LN (dbi);
    // in pTab suchen, ob points already defined; ip1, ip2 setzen.
    // not yet defined: add p1 to pTab.
    ip1 = gis_pts_add__ (&ln1->p1, pTab, ptNr, ptMaxNr, tol);
    // not yet defined: add p2 to pTab.
    ip2 = gis_pts_add__ (&ln1->p2, pTab, ptNr, ptMaxNr, tol);
    if(ip2 < 0) break;
    // add breakline
    if(i1 >= lnMaxNr) {TX_Print("ptab_cad_load1 E002"); break;}
    // add pointers to lTab
    lTab[i1].i1 = ip1;
    lTab[i1].i2 = ip2;
    ++i1;
  }
  *lnNr = i1;



  // // Test only ...........
    // for(i1=0;i1<*ptNr;++i1) printf("p[%d]=%f %f %f\n",i1,
      // pTab[i1].x,pTab[i1].y,pTab[i1].z);
    // for(i1=0;i1<*lnNr;++i1) printf("l[%d]=%d %d\n",i1,
      // lTab[i1].i1,lTab[i1].i2);


  return 0;

}
*/


//================================================================
  int gis_pts_add__ (Point *pt1) {
//================================================================
// gis_pts_add__      add point to pointSet ActPtab


  printf("gis_pts_add__ ***\n");

  // ActPtab.data[ii] = *pt1;


  return 0;

}
 
/* version used for ptab_cad_load1
//==============================================================================
  int gis_pts_add__ (Point *pt1, 
                     Point *pTab, int *ptNr, int ptMaxNr, double tol) {
//==============================================================================
// gis_pts_add__        add point to pointSet and display point
// check if pt1 is already in pTab; return its index,
// if not: add pt1 to pTab, return its index.
//
// Input:
//   pt1      point to add
//   ptMaxNr  siz of pTab
// Output:
//   pTab
//   ptNr     +=1
//   RetCod   pointIndex or -1 (pTab.-Overfolw).
//
// used by ptab_cad_load1

  int   i1;


  // DEB_dump_obj__(Typ_PT, pt1, "gis_pts_add__:");
  // printf(" ptNr=%d\n",*ptNr);

  // check if pt1 is already in pTab
  for(i1=0; i1<*ptNr; ++i1) {
    if(UT3D_comp2pt(pt1, &pTab[i1], tol) == 0) continue;
    return i1;   // gefunden ..
  }

  // add pt1 to pTab.
  i1 = *ptNr;
  if(i1 >= ptMaxNr) {TX_Print("gis_pts_add__ E001"); return -1;}
  pTab[i1] = *pt1;
  ++(*ptNr);


  GR_tDyn_symB__ (&pTab[i1], SYM_TRI_S, 0);
  GR_tDyn_txiA (&pTab[i1], i1, 0);

  
  // printf("ex gis_pts_add__ ptNr=%d\n",*ptNr);
  
  return i1;

}


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
  int gis_surTab_add (long dbi_p, long dbi_m) {
//================================================================
// add new created pointSurf to surTab
//   dbi_m      -1L  does not yet exist

  if(surTabNr >= MAX_SURPTAB) return -1;

  // printf("gis_surTab_add %d\n",dbi_p);

  surTab[surTabNr].dbi_p  = dbi_p;
  surTab[surTabNr].dbi_m  = dbi_m;
  surTab[surTabNr].dli_p  = -1L;
  surTab[surTabNr].dli_m  = -1L;
  surTab[surTabNr].exi_p = 0;        // new created
  surTab[surTabNr].exi_m = 0;        // new created
  surTab[surTabNr].vis_p = 0;        // no mesh yet
  surTab[surTabNr].vis_m = 0;        // no bound yet

  ++surTabNr;

  return 0;

}
 

//================================================================
  int gis_dumpHtm_EL () {
//================================================================

  int      i1, i2, iNr, *ia;
  FILE     *fpo;
  char     s1[240];
  IntTab *el1;


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
  OS_browse_htm (NULL, NULL);


  return 0;

}


//================================================================
  int gis_dump_ActEdg () {
//================================================================

  int  i1;

  printf("gis_dump_ActEdg --------------------------------\n");

  DEB_dump_obj__ (Typ_MemTab, &ActEdg, "ActEdg:");
  for(i1=0; i1<ActEdg.rNr; ++i1) {
    printf("  ActEdg[%d]=%d\n",i1,ActEdg.data[i1]);
  }

  printf("end gis_dump_ActEdg --------------------------------\n");

  return 0;

}

 
//================================================================
  int gis_dump_pntTab () {
//================================================================

  int  i1;

  for(i1=0; i1<pntTabNr;  ++i1) {
    printf("pntTab[%d] = A%ld\n", i1,
        pntTab[i1].dbi_p);
  }

  return 0;

}


//================================================================
  int gis_dump_surTab () {
//================================================================

  int  i1;

  printf("========== gis_dump_surTab %d ===========\n",surTabNr);
  printf(" active PTAB=%d MSH=%d FtabMod=%d\n",
         ActSurPtb, ActSurMsh, ActFtabModFlag);


  for(i1=0; i1<surTabNr;  ++i1) {
    printf(
" surTab[%d].dbi_p=%ld dbi_m=%ld dli_p=%ld dli_m=%ld exip=%d exim=%d visp=%d vism=%d\n",
        i1,
        surTab[i1].dbi_p,
        surTab[i1].dbi_m,
        surTab[i1].dli_p,
        surTab[i1].dli_m,
        surTab[i1].exi_p,
        surTab[i1].exi_m,
        surTab[i1].vis_p,
        surTab[i1].vis_m);

  }


  printf("ActEdg:\n");
  gis_dump_ActEdg ();

  printf("ActPtab:\n");
  DEB_dump_obj__ (Typ_MemTab, &ActPtab, "ActPtab:");

  printf("ActFtab:\n");
  DEB_dump_obj__ (Typ_MemTab, &ActFtab, "ActFtab:");

  printf("ActEtab:\n");
  MSH_dump_eTab (&ActEtab, &DatEtab);


  return 0;

}


//================================================================
  int gisEdg_init () {
//================================================================
// start definition of edgeline


  // printf("gisEdg_init \n");

  // clear existing edgeline
  ActEdg.rNr = 0;

  gis_UI_edg_mod ();


  // // clear display
  // GL_temp_del_all ();
  // DL_Redraw ();


  return 0;

}


//================================================================
  int gis_edg_act_disp () {
//================================================================
// display ActEdg as temp. polygon 

  int    ii;
  long   l1;
  Point  *p1;


  // temporary preview of IntTab
  GL_temp_del_all ();   // remove all temp elems (previous IntTab)


  if(ActEdg.rNr == 1) {
    // disp temp. circ at ActPtNr
    l1 = TMP_DL_ci;      // -3L
    // p1 = &ActPtab.data[ActPtNr];
    ii = ActEdg.data[0];
    if(ii >= 0) {
      p1 = &ActPtab.data[ii];
      GR_temp_symB (&l1, ATT_COL_RED, SYM_CIR_S, p1);
    }


  } else {
    // GR_tDyn_pcv
    gis_edg_disp_cv (TMP_DL_ci,
                     ActEdg.data, ActEdg.rNr, ActPtab.data, col_ActEdg, 0);
    // gis_edg_disp_cv (-1L, ActEdg.data, ActEdg.rNr, ActPtab.data, col_ActEdg, 0);
  }

    // gis_dump_ActEdg ();       // dump ActEdg

  return 0;

}
 

//================================================================
  int gis_edg_act_disp_del () {
//================================================================
// remove display of ActEdg

  // printf("gis_edg_act_del \n");

  GL_temp_del_all ();
  DL_Redraw ();

  // update wEL_nr
  GUI_label_mod (&wEL_nr, " - ");

  return 0;

}
 

//========================================================================
  int gis_edg_disp_cv (long dli,
                   int *ib, int iNr, Point *pa, int att, int mode) {
//========================================================================
/// gis_edg_disp_cv   display edgeline polygon
/// Input:
///   dli       -1 for temporary element; or from DL_StoreObj
///   ib, iNr   boundary; indices into pa
///   pa        pointTable
///   mode 0    open polygon
///        1    closed polygon - closing point is added !

  int    i1, iSiz;
  Point  *cv;


  // printf("gis_edg_disp_cv dli=%ld iNr=%d mode=%d\n",dli,iNr,mode);


  if(iNr < 2) return 0;

  iSiz = iNr + mode;
  i1 = sizeof(Point) * iSiz;
    // printf(" tmpSpcSiz=%d\n",i1);

  cv = (Point*)MEM_alloc_tmp(i1);
  // cv = (Point*)alloca(sizeof(Point)*(iNr + 1));
  if(cv == NULL) {TX_Print("gis_edg_disp_cv EOM"); return -1;}


  for(i1=0; i1<iNr; ++i1) {
      // printf(" P[%d] ib=%d\n",i1,ib[i1]);
    cv[i1] = pa[ib[i1]];
  }

  if(mode > 0) cv[iNr] = cv[0];

  GR_tDyn_pcv (&dli, att, iSiz, cv);

  // dli=DL_temp_nxt-1
    // printf(" dli=%ld\n",GL_temp_iNxt() - 1);

  return 0;

}


//================================================================
  int gis_edg_ini_stat (MemTab(IntTab) *eTab, MemTab(int) *eDat) {
//================================================================
// gis_edg_ini_stat       init stat of all IntTabs

  int      i1;

  // printf("gis_edg_ini_stat \n");

  for(i1=0; i1<eTab->rNr; ++i1) eTab->data[i1].stat = 0;


  return 0;

}


//================================================================
  int gis_block (int mode) {
//================================================================
// mode 1=block input
//      0=unblock input
 

  printf("gis_block %d\n",mode);

  if(mode > 0) {
    GUI_set_enable (&tbGis1, FALSE); // block toolbar
    UI_block__  (-1, 1, 1);         // block input


  } else {
    GUI_set_enable (&tbGis1, TRUE);
    UI_block__  (-1, 0, 0);

  }


  return 0;

}

 
//================================================================
  int gis_edg_mod (int mode) {
//================================================================
// gis_edg_mod         init/add/update/delete ActEdges
// add ActEdg to ActEtab
// Input:
//   mode    TYP_FuncInit TYP_FuncAdd TYP_FuncDel
//   ActEdg
//
// ActEtab DatEtab ActEdg

  int      irc, i1, i2, ind;
  long     l1;
  double   d1;
  IntTab *el1;


  // printf("gis_edg_mod mode=%d ActEdg.rNr=%d\n",mode,ActEdg.rNr);


  //----------------------------------------------------------------
  // Init: create empty Record.
  if(mode == TYP_FuncInit) {
    // printf(" gis_edg_mod TYP_FuncInit\n");

    ActEtab.rNr = 0;     // reset
    DatEtab.rNr = 0;     // reset
    ActEdg.rNr   = 0;     // reset

    return 0;
  }



  //----------------------------------------------------------------
  // Add: add IntTab ActEdg to ActEdges
  if(mode != TYP_FuncAdd) goto L_Del;

  // test if ActEdg is OK ..
    // gis_dump_ActEdg ();
  if(ActEdg.rNr < 2) {
    TX_Print("***** active Poygon is not defined *****");
    printf("**** empty ActEdg\n");
    return -1;
  }

  // tests boundaries (not breaklines)
  if((ActEdg.use == MSH_EDGLN_IB)||(ActEdg.use == MSH_EDGLN_OB)) {
    // remove closing-point from boundaries
    i1 = ActEdg.data[0];
    i2 = ActEdg.data[ActEdg.rNr - 1];
    if(i1 == i2) ActEdg.rNr -= 1;

    // check if ActEdg is CCW; else invert.
    irc = UT2D_srar_inpt3 (&d1, ActEdg.rNr, ActEdg.data, ActPtab.data);
    if(irc < 0) MEM_inv_itab (ActEdg.rNr, ActEdg.data);

  }

  // add ActEdg to ActEtab
  irc = gis_edg_add__ (&ActEtab, &DatEtab, ActEdg.data, ActEdg.rNr, ActEdg.use);
    // MSH_dump_eTab (&ActEtab, &DatEtab);

  // display IntTab
  ind = ActEtab.rNr - 1;
  gis_edg_disp__ (&ActEtab.data[ind], ind);

  // clear ActEdg, remove display.
  gis_edg_act_del ();

  return irc;


  //----------------------------------------------------------------
  // Delete:  remove IntTab ? from ActEdges
  L_Del:
  if(mode != TYP_FuncDel) {TX_Print("gis_edg_mod E001"); return -1;}
  TX_Print("gis_edg_mod E002");
  return -1;

}


//================================================================
  int gis_edg_del_all () {
//================================================================
// delete ActEdg & alle IntTabs


  printf("gis_edg_del_all \n");

  // delete Edglines
  ActEtab.rNr = 0;
  DatEtab.rNr = 0;
  ActEdg.rNr = 0;

  // remove display of all edgeLines
  gis_edg_disp_del_all ();

  return 0;

}



//=============================================================================
  int MSH_msh0__ (MemTab(Point) *pTab, MemTab(Fac3) *fTab,
                  MemTab(IntTab) *eTab, MemTab(int) *eDat,
                  Point2 *p2a, int iOut) {
//=============================================================================
// see ../myGIS1/GIS__.c:4980
//
// MSH_msh0__                create Mesh from Points & boundary
// Input:
//   pTab     the 3D-points-table
//   p2a      the 2D-points-table
//   pNr      nr of points in pa
//   iOut     0=do not remove outer triangles; 1=yes, remove.
//   eTab     edgelines;
//   eDat     ??
// Output:
//   fTab     faces of mesh; indexes -> pa;
//   retCod   

// create Mesh-data.
// - read all points
// - read BL (add tp points)
// - create AB
// - create mesh from AB
// - process BL


// Prerequisites: OB|AB and BL must already be in eTab.
// Cannot create new points; breaklines and boundaries may not intersect;
//   (must be broken with new gis-point at intersection)
// Die Aussenkontur erzeugen = Startkontur.
// Die gesamte AussenKontur vermaschen
// mesh all points inside outer-boundary
//    das Face suchen, in dem der Punkt liegt;
//    das Face durch 3 neue Faces (die durch den Punkt gehen) ersetzen
// mesh all points of inner-boundaries
// Optimize: alle Faces mit 2 benachbarten Kanten testen;
//   wenn Querkante kuerzer als gemeinsame Kante: Kante vertauschen.

// pa    pNr 3D-points
// p2a   pNr 2D-points (identical points without z)
//
// fTab (*MemTab(Fac3))   (int) ->rNr, (*Fac3) ->data
//  i1, i2, i3    index to points in pa; all faces CCW
//  st            0=OK, 16=marked_for_delete
//
// eTab (*MemTab(IntTab))  (int) ->rNr, (*IntTab) ->data
//   ia=ipTab, iNr=size of ia, typ=BL|IB|OB
//   see ../ut/ut_msh.h
//
// sap statusArray points;  char* parallel to pTab
//   -1 = outside;
//    0 = unknown
//    1   bit - BL
//    2   bit - IB
//    4   bit - OB|AB
//   16   bit - to be remeshed
// 
// fnb neighbour-edges;  Fac3* parallel to fTab
//   i1  index face from face-point i1 to face-point i2
//   i2  index face from face-point i2 to face-point i3
//   i3  index face from face-point i3 to face-point i1
//       -1 = this edge is unresolved or outside or inside hole
//       -2 = this edge is a breakline (-MSH_EDGLN_BL)
//       -3 = this edge is a inner breakline (-MSH_EDGLN_IB)
//       -4 = this edge is a outer breakline (-MSH_EDGLN_OB)
//       -16  fnb for this face invalid; call UFA_fnb_init_1
//       -32  temporary-breakline
//   st  0=OK, 16=marked_for_delete



  return -1;

}


//================================================================
  int gis_pts_add_e (int *iTab, Point *p1a, int pNr) {
//================================================================
// see ../myGIS1/GIS__.c:4773
// gis_pts_add_e    add points to pointSet ActPtab for edgeline-use
// returns a table of point-indexes
// Input:
//   iTab[pNr]    -1 on input or index to point in ActPtab
//   p1a[pNr]     new points
// Output:
//   iTab[pNr]    index in ActPtab for all new points
//   retCod       -1 = EOM error
//
// create GIS-points; return pointNumbers.
// iTab     must have size pNr; -1 = get new PointNr, >=0 modify this point.
// see also gis_PtAdd
// returns iTab in memspc102 ! (do not free)

  int    irc, i1, i2, iAct, iaSiz, ii, ip1, ip2, ip3, iMod, iNew;
  long   ld;
  // char   *elStat;
  double dz;
  Point  px;


  printf("gis_pts_add_e %d ActPtab.rNr=%d\n",pNr,ActPtab.rNr);


  return -1;

}


//================================================================
  int gis_edg_load__ (char *fn) {
//================================================================
// gis_edg_load__     UU    load active edgeline from file -> ActEtab
// first load into local space ia1,pa1; then do tests and store -> ActEtab
// Input:
//   ActEdg.use  typ  MSH_EDGLN_BL|MSH_EDGLN_OB|MSH_EDGLN_IB
// Output:
//   retCode     unused
//
// using memspc201 memspc501 

  int       i1, irc, ii, pNr, peNr, pa1Siz, *ia1, typ, lNr;
  // char      *sap;
  FILE      *fpi;
  Point     *pa1, pt1, pt2;
  Point2    *p2a;


  typ = ActEdg.use;


  printf("LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL \n");
  printf("gis_edg_load__ typ=%d |%s|\n",typ,fn);
  printf("  ActPtab.rNr=%d\n",ActPtab.rNr);
  // printf(" tol=%lf\n",gis_TOL_PT);


  return -1;

}


//================  EOF  ==================================
