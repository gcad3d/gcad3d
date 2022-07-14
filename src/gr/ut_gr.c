//     ut_gr.c                                 CADCAM-Services Franz Reiter
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
TSU_tess_sTab
  TSU_tessObj
    APT_DrawSur
    APT_DrawSol


-----------------------------------------------------
Modifications:
2020-05-06 new concept GR_perm_/GR_tDyn_/GR_temp_*. RF.
2002-02-17 GR_CrePoly und GR_CreDitto2 zu. RF.
2001-06-06 GR_Init1 zu.
2001-04-22 GL_DrawRSur: 2 Par. zu.
           GR_CreQFac (Vierecke) raus ; Ersatz GR_CrePrism.
2001-04-21 erstmals erfasst, GR_DrawCyl,GL_DrawRSur korr.

-----------------------------------------------------
*/
#ifdef globTag
 void GR(){}
#endif
/*!
\file  ../gr/ut_gr.c
       ../gr/ut_gr.h
\brief create graphic objects 

Prerequisites:
#include "../ut/ut_memTab.h"           // MemTab_..

-----------------------------------------------------
See INF_GR_RECORDS

GR_perm_*     create permanent objects   see INF_GR_RECORDS

GR_temp_*     create temporary-objects   see INF_GR_RECORDS

GR_tDyn_*     create Temporary-Dynamic-objects   see INF_GR_RECORDS

GR_set_*      used by GR_perm_ and GR_temp_ and GR_tDyn


----- to be replaced:
GR_Draw..     create DL-record and display obj 
              Do not create DB-record

GR_Cre..      create dynamic-DB-obj, DL-record and display obj
              (get dynamic DB-index: dbi = DB_Store.. ((long)-1, obj, ..);

GR_Disp_..    display obj (using dynamic DL-record)
              Do not create DB-record
              (get dynamic DL-record:  dli = DL_StoreObj (oTyp, -1L, att);)


=====================================================
List_functions_start:

GR_Disp_message       switch for textoutput to console (DispMode)

GR_set_obj            display obj     - temp/tDyn/perm
GR_set_osu            disp surface from bin.surf.obj
GR_set_sur            disp surface from dbi
GR_set_ocv            disp all kinds of curves from binary-obj
GR_set_ccv            display n composite-curves

GR_set_pcv            display polygon - temp/tDyn/perm
GR_set_txt__          disp notes with 2D-text
GR_set_txtA           disp 2D-text at position
GR_set_txtG           grafic text (rotated, scaled)
GR_set_dimen          hor/vert/parall. dimension
GR_set_dim3           3D-dimension
GR_set_symV3          vector-symbol, oriented (3D, rotate + tilt)
GR_set_mdr            disp subModel from modelreference

GR_set_strip_v        disp strip vertical

-------------- display permanent objects
GR_perm_pt            perm.disp Point
GR_perm_ln            perm.disp line
GR_perm_pln           plane
// GR_perm_pcv           perm.disp polygon
GR_perm_ocv           perm.disp Curve-obj (C, S)
GR_perm_sur           perm.disp surface
GR_perm_txt__         perm.disp notes with 2D-text
GR_perm_txtG          perm.disp grafic text (rotated, scaled)
GR_perm_mdr           perm.disp subModel from modelreference
GR_perm_Dimen          hor/vert/parall. dimension

-------------- display temporary-dynamic objects
GR_tDyn_ox            temp.Dyn complex-obj ObjGX
GR_tDyn_dbo           temp.Dyn DB-obj
GR_tDyn_ocv           temp.Dyn display of all kinds of curves from binary-obj
GR_tDyn_osu           temp.Dyn display surf from binary-obj
GR_tDyn_obj           tempDisp obj from typ+struct
GR_tDyn_nobj          tempDisp objects from typ+struct, nr

GR_tDyn_pt            temp.Dyn point
GR_tDyn_npt__         disp. points
GR_tDyn_npti          display ptNr points and point-numbers
GR_tDyn_ln            temp.Dyn line
GR_tDyn_ln_2pt        disp line from 2 points
GR_tDyn_pcv           temp.Dyn polygon
GR_tDyn_box__         disp 3D-boundingBox (lines)

GR_tDyn_vc__          3D-vector at 3D-point; length true or normalized.
GR_tDyn_ell           disp. ellipse
GR_tDyn_bez           display  Bezier-Curve
GR_tDyn_rbez          Testdisplay  rational-bezier-curve via rational-B-spline
GR_tDyn_psp3          disp polynom. Spline Typ_CVPSP3 
GR_tDyn_ccv
GR_tDyn_mdr           subModel from modelreference
GR_tDyn_pln           plane
GR_tDyn_sur           surface

GR_tDyn_tx2A          disp text at 2D-position
GR_tDyn_npt_2         disp. 2D-points
GR_tDyn_ln2_2pt       disp line between 2 2D-points
GR_tDyn_vc2__         temp.Dyn 2D-Vector; length true or normalized
GR_tDyn_ci2           display 2D-circle
GR_tDyn_el2c          disp. 2D-ellipse-centerPos
GR_tDyn_bsp2          disp. 2D-bSplineCurve
GR_tDyn_pcv2          disp 2D-polygon
GR_tDyn_box2__        disp 2D-boundingBox from 2 points
GR_tDyn_box2_4db      disp 2D-Box from 4 doubles
GR_tDyn_pcv3p2        Display boundary of triangle from 3 2D-points
GR_tDyn_ccv2

GR_tDyn_symB2__       temp.dyn disp 2D-BITMAP-Symbol
GR_tDyn_symB__        temp.dyn disp BITMAP-Symbol (SYM_TRI_S SYM_STAR_S ..)
GR_tDyn_nsymB         temp.dyn disp BITMAP-Symbols (SYM_TRI_S SYM_STAR_S ..)

GR_tDyn_symV2        disp vector-symbol, rotated (in XY-plane, 2D)
GR_tDyn_symV3         disp vector-symbol, oriented (3D, rotate + tilt)

GR_tDyn_txtG          disp text at 3D-point grafic text (rotated, scaled)
GR_tDyn_txtA          disp text at 3D-point
GR_tDyn_txiA          display number at position

GR_tDyn_npcv_itb_npt  disp group of polygons from IndTab to points
GR_tDyn_npcv_itb_nipt disp group of polygons from IndTab to indexed points
GR_tDyn_ipcv          display indexed point-array as curve
GR_tDyn_ip2cv         display indexed 2D-point-array as curve
GR_tDyn_nifac         display MshFac
GR_tDyn_mtb           disp MemTab; alias MemTab_disp_tdyn

-------------- display temporary objects
GR_temp_message       Display messages with GR_temp_xxx or not.

GR_temp_src           tempDisp obj from sourcecode
GR_temp_dbo           tempDisp DB-obj
GR_temp_nobj          tempDisp binary objects from typ+struct, nr
GR_temp_ocv           tempDisp binary Curve-obj (C, S)
GR_temp_osu           tempDisp binary surface (A)
GR_temp_sur           tempDisp surface (symbolic-hilited)
GR_temp_otb           tempDisp binary-object-table

GR_temp_pt            tempDisp PointGR_draw_mesh
GR_temp_ln            tempDisp line
GR_temp_vc            tempDisp Vector; length true or normalized
GR_temp_pcv__         tempDisp polygon
GR_temp_ci            tempDisp circle
GR_temp_ang           tempDisp angle
GR_temp_traRot        tempDisp transformation-rotation
GR_temp_pln           tempDisp plane
GR_temp_symB          tempDisp BITMAP-Symbol (SYM_TRI_S SYM_STAR_S ..)
GR_temp_symV3         tempDisp oriented vector-symbols (3D, rotate + tilt)
GR_temp_mdr           disp subModel from modelreference

GR_temp_rect1         temp.disp rectangle from point, 2 vectors

GR_temp_txtG          disp text at 3D-point grafic text (rotated, scaled)
GR_temp_txtA          tempDisp text
GR_temp_tx2A          disp text at 2D-position
GR_temp_txiA          disp number at position
GR_temp_txi2          disp number at 2D-position

GR_temp_dimen          hor/vert/parall. dimension

GR_temp_att__         disp direction/objID for all objs in group
GR_temp_att_1         disp direction/objID for all objs in group


-------------- display into open GL-list
GR_disp_cv_pMesh      boundary of surface from points


-------------- TO BE REPLACED WITH GR_temp_* GR_perm_*
GR_Disp_ox            temp. display of ObjGX-structs

GR_Disp_spu           display nicht gelochte planare Flaeche

GR_Disp_tria          display boundary of triangle as lines
GR_Disp_triv          display normalVector and number of triangle
GR_Disp_tris          display surface of triangle
GR_Disp_su3p2         display surface of 3 2D-points
GR_Disp_su3pt         display surface of 3 points
GR_Disp_fan           display triangleStrip
GR_Disp_patch         display Opengl-patch (type & n-points)
GR_Disp_nfac          display (not indexed) Fac3-faces
GR_Disp_iSur          display tesselated faces (using fmt pmt imt)


-------------- OBSOLETE permanent ----------------------------
GR_Draw_spu
GR_DrawSup            draw planare surface (for bodies ..)

GR_create_dummy       create a dummy DL-record; in DL and in GL.
GR_Draw_subCurv       draw segment of polygon or CCV.

GR_DrawDitto2
GR_DrawModel
GR_draw_mesh
GR_DrawSur
GR_DrawCyl               Cyl und Cone
GR_DrawTorSeg
GR_DrawPrism 
GR_DrawDisk
GR_DrawFan
GR_DrawTriaFan 
GR_DrawStrip
GR_DrawTriaStrip
GR_Draw_nipatch          display tesselated faces (using fmt pmt imt)
GR_draw_mesh          disp mesh
GR_DrawSurPtab        display PTAB (its boundary)


//--- OBSOLETE - replaced by functions AP_add_* ------------------
GR_CreDitto2

GR_CreSol__
GR_CreCyl                In: 2 Punkte 2 Radien        
GR_CrePrism              In: zwei Punkteketten
GR_CreDisk               In: innerer Rad u ausserer rad fuer Kreisring       
GR_CreTorSeg             In: 2 Circs         
GR_CreSolSph
GR_CreSolCon
GR_CreSolTor
GR_CreSolBrep            TODO ..

GR_CreFan
GR_CreTriaFan            In: punkte rund um eine Flaeche
GR_CreTriaStrip

//----------------------------------------------------------------
GR_mdMock_imp            import VRML|OBJ|STL-file - save as tmp/Data_<moldel>.tess
GR_pt_par_sel_npt        get selectionpoint and parameter on polygon
GR_pt3_pt2               get 3D-point from 2D-point (with GL2D_Z);

GR_Init0                 primary init
GR_Init1

List_functions_end:
=====================================================

\endcode *//*----------------------------------------
See ../xa/xa_ui_gr.c

UNUSED
// GR_Draw_oid_dir_npt      disp. ObjID, direction
// GR_Disp_ipatch        display a set of triangles from indexed points
// GR_Draw_ipatch        display a set of triangles from indexed points
// GR_Disp_spu1          display planare Contour
// GR_DrawCurv           display curve from struct ObjGX
// GR_Disp_vc2           display 2D-Vector; length true or normalized
// GR_Disp_sur           display surface from OGX
// GR_Disp_sru           Display ungetrimmte/ungelochte Ruled Flaeche
// GR_Disp_sbsp          display  BSP-Surf; nur Kontrollpunkte


Compile:
cl -c ut_gr.c

cc -c -g3 -Wall ut_gr.c


*/


// erforderlich fuer #include <GL/gl.h>
//#ifdef _MSC_VER
//#include <windows.h>
//#endif

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//#include "../gtk/func_types.h"            // Typ_Att_blue

// #include "../ut/ut_umem.h"             // MEM_alloc_tmp
#include "../ut/ut_geo.h"
#include "../ut/ut_cast.h"                // INT_PTR
#include "../ut/ut_geo_const.h"        // UT3D_RSYS_2D
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_itmsh.h"            // MSHIG_EDGLN_.. typedef_MemTab.. Fac3
#include "../ut/ut_ox_base.h"             // OGX_SET_INDEX
#include "../ut/ut_txt.h"                 // fnam_del
#include "../ut/ut_os.h"                  // OS_..
#include "../ut/ut_col.h"              // COL_INT32

#include "../db/ut_DB.h"                  // DB_GetPoint DB_mdlNam_iBas


//#include <GL/gl.h>
//#include <GL/glu.h>

#include "../gr/ut_GL.h"                  // GL_pickSiz
#include "../gr/ut_DL.h"

#define extern          // damit wird "extern" im Includefile geloescht !
#include "../gr/ut_gr.h"
#undef extern           // reset extern ..

#include "../ut/func_types.h"                  // SYM_SQUARE

#include "../xa/xa.h"                     // mem_cbuf1_SIZ
#include "../xa/xa_mem.h"                 // memspc55
#include "../xa/xa_msg.h"                 // MSG_*


// typedef_MemTab(int);
// typedef_MemTab(IndTab);
// typedef_MemTab(Point);




//=============== extern glob. vars ======================
// aus xa.c:
extern AP_STAT   AP_stat;                 // sysStat,errStat..
extern ColRGB    AP_defcol;
extern int       WC_sur_ind;            // Index ActiveConstrPlane
extern Plane     WC_sur_act;            // ActiveConstrPlane

// aus ../ci/NC_Main.c:
extern int     APT_3d_mode;
extern int     APT_dispSOL;           // 0=ON=shade; 1=OFF=symbolic
extern int     APT_obj_stat;          // 0=permanent, 1=temporary (workmode)
extern long    AP_dli_act;            // index dispList
// extern int     APT_dispNam;           // display ObjID
// extern int     APT_dispDir;           // display ObjDirection; 1=yes, 0=not

//  DISP_AC - Toleranz (Sehnenfehler bei der Darstellung von Arcs.
// extern double    APT_TOL_ac;

// extern Point     GL_ptArr30[GL_ptArr30Siz]; // auxBuffer aus ut_GL.c
// extern Point     GL_ptArr31[140];   // auxBuffer aus ut_GL.c Siz 140
// extern Point     GL_ptArr32[140];   // auxBuffer aus ut_GL.c Siz 140

// die folgenden werden zB bei DrawCirc temp. benutzt
// Muessen in der Applikation definiert werden !
// extern Point2    GR_ptArr1[500];    // ex NC_Main.c  Siz 500
// extern Point2    GR_ptArr2[500];    // ex NC_Main.c  Siz 500



// aus ../gr/ut_DL.c:
extern long DL_perm_ind;
extern long DL_temp_ind;        // if(>0) fixed temp-index to use; 0: get next free
extern DL_Att  *GR_ObjTab;



// aus ../gr/ut_GLU.c:
extern Point     *GLT_pta;
extern long      GLT_pta_SIZ;




// aus ../gr/tess_su.c:
extern int TSU_mode;   // 0=normal darstellen; 1=speichern


// aus ../gr/ut_GL.c:
extern ColRGB  GL_actCol;
extern int     GL_actTex;
extern double  GL2D_Z;          // Z-value for 2D-drawing-functions
extern double  GL_SclNorm;           // fix different modelsize
extern double  GL_Scale;             // 



/*=============== local glob. vars =======================*/

int GR_actView = FUNC_ViewIso;

static int   GR_typ = 0;
static long  GR_dbi = 0;
static int   DispMode=1;  ///< 0=Aus, 1=Ein.
static int tmpDspMode = 0;

const MshFac GR_MshFac_NUL = _MSHFAC_NUL;




//================================================================
//================================================================


//==============================================================================
  int GR_set_txtA (int opers, Point *pt1, char *txtA, int att) {
//==============================================================================
// GR_set_txtA           disp 2D-text at position
// Input:
//   opers    object-persistence; TDYN|TEMP|NONE;  see INF_OPERS
//   pt1      position
//   txtA     textstring; null-terminated; maxLen 256
//   att      see INF_COL_SYMB
//

  Point  ptc;

  // printf("GR_set_txtA opers=%d |%s| att=%d\n",opers,txtA,att);
  // DEB_dump_obj__ (Typ_PT, pt1, " set_pcv");


  if(pt1) ptc = *pt1;
  else    ptc = GL_GetCen();   // ScreenCenter in UserCoords
    // DEB_dump_obj__ (Typ_PT, &ptc, " temp_vc-ptc");

  // tdyn
  if(opers & OPERS_TDYN) {
    DL_tDyn_init (att);
    GL_att_sym (att);   // see INF_COL_SYMB

  // temp
  } else if(opers & OPERS_TEMP) {
    DL_temp_init ();
    GL_att_sym (att);   // see INF_COL_SYMB
  }


  GL_set_txtA (&ptc, txtA);

  if(opers & OPERS_CLOSE) GL_list_close (); // close GL-record

  return 0;

}



//================================================================
   int GR_set_sur (int opers, long dbi, int att) {
//================================================================
// GR_set_sur           disp surface from dbi
//   att    for perm: struct ColRGB
//          for temp: GR_TMP_HILI|GR_TMP_DIM|GR_TMP_DEF;
// was APT_DrawSur GR_DrawSur

  int       irc = 0, i1, ssTyp;
  long      l1;
  char      cBuf[256];
  ObjGX     *oxi;


  // printf("GR_set_sur opers=%d A%ld att=%d\n",opers,dbi,att);
  // printf(" DL_perm_ind=%ld\n",DL_perm_ind);
  // UTcol_dump ((ColRGB*)&att, "GR_set_sur");


  // get bin.obj from dbi
  oxi = DB_GetSur (dbi, 1);
    // DEB_dump_ox_s_ (oxi, "GR_set_sur\n");

  return GR_set_osu (opers, oxi, dbi, att);

}


//=====================================================================
   int GR_set_osu (int opers, ObjGX *oxi, long dbi, int att) {
//=====================================================================
// GR_set_osu 	         disp surface from bin.surf.obj
//   att    for perm: struct ColRGB
//          for temp: Typ_Att_hili1       if(ColRGB)att.hili is on;
//                    Typ_Att_dim         if(ColRGB)att.dim is on;
//                    Typ_Att_dash_long   default
// was APT_DrawSur GR_DrawSur

  int       irc = 0, i1, ssTyp;
  long      l1;
  char      cBuf[256];


  // printf("GR_set_osu opers=%d A%ld att=%d\n",opers,dbi,att);
  // printf(" DL_perm_ind=%ld\n",DL_perm_ind);
  // UTcol_dump ((ColRGB*)&att, "GR_set_sur");




  //----------------------------------------------------------------
  // perm
  if(opers & OPERS_PERM) {
    // create perm. DL-record an open GL-list
    DL_perm_init (Typ_SUR, dbi, att);

    // perm: att is struct ColRGB (color with type symbolic|shaded|textured|..)
    GL_actCol = *(COL_INT32(&att));
      // used later in GL_set_* with GL_ColSet

    if (GL_actCol.vtex != 0) {
      // prepare texture (set GL_actTex)
      // find textureNr of surface <apt_ind> from (typ,dbi)
      l1 = GA_find__ (Typ_SUR, dbi);
        // printf(" GA-nr=%d\n",l1);
      if(l1 < 0) {
        printf("GR_set_sur I001 %ld\n",dbi);
        GL_actTex = -1;
      } else {
        // get TexRef of GA-Record l1
        GL_actTex = GA_tex_ga2tr (l1);
      }
      
    } else {
      // no texture ..
      GL_actTex = -1;
    }
        // printf(" _set_sur typ=%d dbi=%ld GL_actTex=%d\n",oxi->typ,dbi,GL_actTex);


  //----------------------------------------------------------------
  // tdyn
  } else if(opers & OPERS_TDYN) {
    DL_tDyn_init (att);


  //----------------------------------------------------------------
  // temp
  } else if(opers & OPERS_TEMP) {
    DL_temp_init ();   //  get GL-index, GL_list_open
    if(((ColRGB*)&att)->vsym) {
      // symbolic - set curve-attribute; 
      if(((ColRGB*)&att)->hili) GL_att_cv (Typ_Att_hili1);
      // else if(((ColRGB*)&att)->dim) GL_att_cv (Typ_Att_dim);
      else                      GL_att_cv (Typ_Att_dash_long);
    } else {
      // set color ..
      GL_att_sur (att);
    }

  }


  //----------------------------------------------------------------
  // tesselate
  irc = TSU_SUR__ (oxi, att, dbi);


  goto L_exit;  // no message degenerate ?

  if(irc < 0) {
    sprintf(cBuf,"degenerate element A%ld",dbi);
    if(MDL_IS_SUB) {
      strcat(cBuf, " in Submodel ");
      strcat(cBuf, DB_mdlNam_iBas(AP_modact_ibm));
    }
    TX_Print(cBuf);
  }
//     AP_dli_act = TSU_dli_get ();
 


  //================================================================
  L_exit:

  if(GL_actTex >= 0) GL_Tex_End ();         // reset texture & GL_actTex
  if(opers & OPERS_CLOSE) GL_list_close (); // close GL-record

  return irc;

}



//==============================================================================
  int GR_set_pcv (int opers, Point *pta, int ptNr, int att) {
//==============================================================================
// GR_set_pcv       display polygon 
// Input:
//   opers    object-persistence; TDYN|TEMP|NONE;  see INF_OPERS
//   ptNr     nr of points
//   att      see INF_COL_CV
//

  // printf("GR_set_pcv opers=%d ptNr=%d att=%d\n",opers,ptNr,att);
  // DEB_dump_nobj__ (Typ_PT, ptNr, pta, " set_pcv");


  // tdyn
  if(opers & OPERS_TDYN) {
    DL_tDyn_init (att);
    GL_att_cv (att);

  // temp
  } else if(opers & OPERS_TEMP) {
    DL_temp_init ();
    GL_att_cv (att);
  }

  GL_set_pcv (ptNr, pta, 0);

  if(opers & OPERS_CLOSE) GL_list_close (); // close GL-record

  return 0;

}


//==============================================================================
  int GR_set_obj (int opers, long dbi, int form, void *obj, int att, int mode) {
//==============================================================================
// GR_temp_nobj    display temp.obj
// Input:
//   opers    object-persistence; eg OPERS_TEMP+OPERS_CLOSE  see INF_OPERS
//   ind      object temorary|temp-dynamic|permanent; see INF_GR_RECORDS
//            OPERS_TEMP: dbi; get next free of records 1-DL_base_font1 (30);
//                        fixed index: set with DL_temp_ind
//            OPERS_TDYN: dbi; dli will be next free perm-dyn DL-index (neg.)
//            OPERS_PERM: dbi; dli will be next free perm-dyn DL-index (neg.)
//                        overwrite DL-record with DL_SetInd/DL_perm_ind
//   dbi      DB-index if known (used for getting PRCV)
//   form,obj obj to display;
//   oNr      nr of objects of type <typ> in obj
//   att      Typ_PT: ATT_PT_BLACK .. see INF_COL_PT
//              ATT_PT_BLACK|ATT_PT_HILI|ATT_PT_DIMMED|ATT_PT_GREEN|ATT_PT_YELLOW
//            Typ_CI|Typ_LN|Typ_CV*|Typ_PLN:   see INF_COL_CV
//            Typ_VC|Typ_SymB:   colorIndex; eg ATT_COL_RED; see INF_COL_SYMB
//              ATT_COL_BLACK|ATT_COL_RED|ATT_COL_GREEN|ATT_COL_BLUE|
//              ATT_COL_YELLOW|ATT_COL_MAGENTA|ATT_COL_CYAN|ATT_COL_WHITE
//              ATT_COL_HILI|ATT_COL_DIMMED
//   mode     Typ_PT|Typ_LN|Typ_CI|Typ_CV*: unused;
//            Typ_VC|Typ_VC2: 0=length-normalized; 1=true-length
//            Typ_SymB: SYM_TRI_S|SYM_STAR_S|SYM_CIR_S|SYM_SQU_B
//            Typ_PLN:  1=Plane; 2=Axis; 4=Axis+Chars; 5=Plane+Axis;
// 
//
// Example - create temporary point:
// GR_set_obj (OPERS_TEMP+OPERS_CLOSE, 0, 0L, Typ_PT, &pt1, ATT_PT_HILI, 0);
//
// see GR_Disp_ox GR_tDyn_obj GR_tDyn_dbo GR_Draw_dbo
//     APT_DrawCurv GR_DrawCurv


  int       irc, oTyp, i1;
  void      *p1;
  void      *o1;
  Point     pt1, pta[2];
  DL_Att    *att1;
  ObjGX     *ox2, ogx;
  // Memspc    tSpc1 = UME_NEW;


  // printf("GR_set_obj opers=%d dbi=%ld form=%d att=%d mode=%d\n",
         // opers,dbi,form,att,mode);
  // printf("GR_set_obj DL_perm_ind=%ld\n",DL_perm_ind);
  // DEB_dump_obj__ (form, obj, "  obj");


  // perm
  if(opers & OPERS_PERM) {
    DL_perm_init (form, dbi, att);

  // tdyn
  } else if(opers & OPERS_TDYN) {
    DL_tDyn_init (att);

  // temp
  } else if(opers & OPERS_TEMP) {
    DL_temp_init ();
  }



  switch (form) {
    //================================================================
    case Typ_PT:
      GL_att_pt (att);   // set color
      GL_set_npt ((Point*)obj, 1);
      break;

    //================================================================
    case Typ_VC:
      // GL_att_sym (att);   // set color
      // pt1 = GL_GetCen();   // ScreenCenter in UserCoords
      GL_att_sym (att);   // set color
      if(mode) {
        // 1 = display exact size (length)
        GR_set_symV3 (OPERS_NONE, SYM_VEC, att, NULL, (Vector*)obj, 
                       UT3D_len_vc((Vector*)obj));
      } else {
        // 0 = display normalized length
        GR_set_symV3 (OPERS_NONE, SYM_ARROW, att, NULL, (Vector*)obj, 1.);
      }
      break;

    //================================================================
    case Typ_LN:
      pta[0] = ((Line*)obj)->p1;
      pta[1] = ((Line*)obj)->p2;
      GL_att_cv (att);
      GL_set_pcv (2, pta, 1); // GL_LINE_STRIP
      if(APT_obj_stat) GL_set_arrh3D (&pta[1], &pta[0]);
      break;

    //================================================================
    case Typ_CI:
    case Typ_CVPOL:     // CurvPoly
    case Typ_CVELL:
    case Typ_CVBSP:
    case Typ_CVRBSP:
    case Typ_CVTRM:
      GL_att_cv (att);
      GR_set_ocv (OPERS_NONE, dbi, form, obj, att);
      break;
/*
    //================================================================
    case Typ_PT2:

    //================================================================
    case Typ_CI2:
      // dbi = DL_get_dbi (*dli);
      GR_tDyn_ci2 (ind, dbi, att, (Circ2*)obj);
      break;

    //================================================================
    case Typ_CVELL2:
      GR_DrawCvEll2 (ind, dbi, att, obj);  // see GR_tDyn_ell GR_DrawCvEll
      break;

    //================================================================
    case Typ_CVPOL2:
      GR_Draw_pol2 (ind, att, obj); // GR_Disp_pol GR_DrawCvPol
      break;

    //================================================================
    case Typ_CVBSP2:
      // dbi = DL_get_dbi (*dli);
      GR_tDyn_bsp2 (ind, dbi, att, obj); // CurvBSpl
      break;


//     //================================================================
//     case Typ_CVTRM:
//       GR_set_ccv (ind, dbi, att, obj, oNr);
// //
// //    OGX_SET_OBJ (&ogx, Typ_CVTRM, Typ_CVTRM, oNr, obj);
// //    // dbi = DL_get_dbi (*dli);
// //      // printf(" CVTRM-dbi=%ld dli=%ld\n",dbi,*dli);
// //    // UME_init (&tSpc1, memspc201, sizeof(memspc201));
// //    // GR_set_ccv (dli, att, obj, &tSpc1);
// //    GR_set_ccv (dli, dbi, att, &ogx, &tSpc1);
// //
//       break;
*/

    //================================================================
    case Typ_SymB:
      GL_att_sym (att);   // set color
      GL_set_symB (mode, obj);
      break;

    //================================================================
    case Typ_PLN:
      GL_att_cv (att);
      GL_set_symVX ((Plane*)obj, mode, 1.);
      // GL_DrawSymV3 (ind, SYM_SQUARE, att, &pl1->po, &pl1->vz, 1.);
      break;
/*
    //================================================================
    case Typ_SURRV:
      // TSU_DrawSurTRV sollte noch in Disp & Draw geteilt werden !
      TSU_DrawSurTRV (ind, att, -1L);
      break;

    //================================================================
    case Typ_SURBSP:
      // nur Kontrollpunkte:
      // sbsp = (SurBSpl*)ox1->data;
      // GR_Disp_sbsp (sbsp->ptUNr, sbsp->ptVNr, sbsp->cpTab, att); // SurBSpl
      // TSU_DrawSurTBSP sollte noch in Disp & Draw geteilt werden !
      TSU_DrawSurTBSP ((SurBSpl*)obj, att, -1L);
      break;

    //================================================================
    case Typ_ObjTab:     // binary-objects-table
      GR_temp_otb (ind, att, obj); // ObjTab
      break;


    //================================================================
    // case Typ_Index:
      // dbi = OGX_GET_INT (ox1);
      // o2 = DB_GetObjGX (ox1->typ, dbi);
      // GR_Draw_ox (dli, &o2, att, mode);
      // break;


    //================================================================
    case Typ_ObjGX:
      return GR_Draw_ox (ind, dbi, obj, att, mode);
//
//    // typ = AP_typDB_typ (typ);
//  case Typ_CV:
//    ox2 = (ObjGX*)obj;
//
//    //----------------------------------------------------------------
//    if(typ == Typ_SUR) {
//      // SUR_Disp_Sur (ox1, wrkSpc, att, p1);
//      TX_Print("**** GR_temp_nobj I1");
//
//    //----------------------------------------------------------------
//    } else {
//      // Typ_CV
//      for(i1=0; i1 < ox2->siz; ++i1) // Recursion !
//        // GR_Draw_ox (ind, &ox2[i1], att, mode);
//        // recurse
//        GR_temp_nobj (ind, ox2[i1].form,  ox2[i1].data, 1, att, mode);
//    }
//    break;
*/

    //================================================================
    default:
      goto L_E001;
  }

  if(opers & OPERS_CLOSE) GL_list_close (); // close GL-record


  return 0;


  L_E001:
      TX_Error("GR_set_obj not yet impl: %d",form);
      return -1;

  L_E002:
      TX_Error("GR_set_obj E002"); // par fehlt ..
      return -1;

  // L_E003:
      // TX_Error("GR_Draw_ox E003"); // wrkSpc fehlt ..
      // return -1;

}


//====================================================================
  void GR_Init0 () {
//====================================================================
// GR_Init0                 primary init

  int     ix, iy;

  LN_WIDTH_ADJUST = 1.f;

  OS_get_scrRes (&ix, &iy);  // get total screensize

  // adjust linewidth for UHD-screens
  if(ix > 3000) LN_WIDTH_ADJUST = 2.f;

  LN_WIDTH_DEF = LN_WIDTH_ADJUST;
  LN_WIDTH_FAT = LN_WIDTH_ADJUST * 4.f;

  MODE_DISP_ONAM_NT = 1;   // disp objNames of notes

  // adjust GL_pickSiz
  GL_pickSiz = 8;
  if(ix > 3000) GL_pickSiz = 16;
  

  return;

}


//====================================================================
  void GR_Init1 (int mode) {
//====================================================================
// is calle from ED_work_END
//   mode    0=clear also basic-models
//           1=do not clear basic-models



  // printf("GGGGGGGGGGGG GR_Init1\n");

  // delete ALL objects in temp-area of DispList
  GL_temp_del_all ();


  // delete displist
  GL_Init1 ();


  AP_mdlbox_invalid_set();   // set AP_stat.mdl_box_valid = void

  // OPAR_init ();

}


/*
//================================================================
  int GR_DrawBMP (long ind, Point *pt1, char *fnam) {
//================================================================

  int   irc;
  long  dlInd;
  char  cBuf[256];



  dlInd = DL_StoreObj (Typ_Tag, ind, 0);

  // den symbol. Pfad aufloesen.
  Mod_get_path (cBuf, fnam);

  irc = GL_Draw_BMP (&dlInd, pt1, cBuf);


  return irc;

}
*/


//================================================================
  int GR_create_dummy (int typ, long dbi) {
//================================================================
// create a dummy DL-record; in DL and in GL.

  long    dli;


  dli = DL_StoreObj (typ, dbi, 0);

  DL_unvis_set (dli, 1);

  // NECCESSARY ! Else objIDs of DL and GL different;
  // makes problems in subModels with eg Vectors.
  GL_create_dummy (&dli);

  return 0;

}

/* replaced by GR_temp_txtG GR_perm_txtG
//====================================================================
  void GR_CreTxtG (long *ind, int attInd,
                   Point *pt1, double size, double ang, char *txt) {
//====================================================================
/// \code
/// att am besten 0.
/// Position links unten.
/// \endcode


  long   dlInd;
  GText  gtx1;


  printf("GR_CreTxtG att=%d siz=%f ang=%f txt=|%s|\n",
          attInd,size,ang,txt);

  gtx1.pt   = *pt1;
  gtx1.size = size;
  gtx1.dir  = ang;
  gtx1.txt  = txt;

  if(*ind == 0) {
    *ind = DB_StoreGTxt (-1L, &gtx1);
  // } else if(*ind < dyn_tx_max) {
    // *ind += dyn_tx_max;
  }

  dlInd = DL_StoreObj (Typ_ATXT, *ind, attInd);
//   /  IN:  Objectindex objInd  und ObjTyp Typ_xx 
//   /  OUT: DispListIndex dlInd

  // GL_DrawTxtG (&dlInd, attInd, pt1, size, ang, txt);
  GL_DrawTxtG (&dlInd, attInd, &gtx1);

}


// replaced by GR_temp_txtG GR_perm_txtG
//====================================================================
  void GR_DrawTxtG (long *dli, int attInd, GText *tx1, long dbi) {
//====================================================================
/// \code
/// attInd dzt 0
/// Position links unten.
/// \endcode


  // Point   ptTr;



  // if(APT_2d_to_3d_mode == OFF) {
    // GL_DrawTxtG (ind, attInd, pt1, size, ang, txt);
    GL_DrawTxtG (dli, attInd, tx1);

  // } else {
    // UT3D_pt_tra_pt_m3 (&ptTr, APT_2d_to_3d_Mat, pt1);
    // GL_DrawTxtG (ind, attInd, &ptTr, size, ang, txt);
  // }

  return;
}



// replaced by AP_add_pt
//====================================================================
  void GR_CrePoint (long *ind, int attInd, Point *pt1) {
//====================================================================
/// \code
/// Input:
///   ind > 0: store obj in DB
///   ind = 0: store dynamic obj in DB.
///   ind < 0: do not store obj in DB.
///   attInd: 0=normal, black;  1=red (hilited);
/// \endcode


  long   dlInd;


  // onjInd erhaelt man beim abspeichern in der DB
  if(*ind == 0) {
    *ind = DB_StorePoint (-1L, pt1);  // store dynamic obj in DB.
  }

  dlInd = DL_StoreObj (Typ_PT, *ind, attInd);
  //  IN:  Objectindex objInd  und ObjTyp Typ_xx 
  //  OUT: DispListIndex dlInd



  GR_DrawPoint (&dlInd, attInd, pt1);

}


//====================================================================
  void GR_Draw_pt2 (long *ind, int attInd, Point2 *pt1) {
//====================================================================
/// \code
///   Create 2D-Point
/// attInd: 0=normal, black;  1=red (hilited);
/// \endcode


  Point   pt2;


  // DEB_dump_obj__(Typ_PT2, pt1, "GR_Draw_pt2 ind=%ld attInd=%d",*ind,attInd);
  // printf(" APT_2d_to_3d_mode=%d\n",APT_2d_to_3d_mode);

  pt2 = UT3D_pt_pt2 (pt1);


  // if(APT_2d_to_3d_mode == OFF) {                // OFF=1
    // GL_DrawPoint (ind, attInd, &pt2);
    GR_tDyn_pt (&pt2, attInd);

}


//====================================================================
  void GR_DrawPoint (long *ind, int attInd, Point *pt1) {
//====================================================================
/// \code
///   Create 3D-Point
/// attInd: 0=normal, black;  1=red (hilited);
/// \endcode


  Point   ptTr;


  // printf("GR_DrawPoint ind=%ld attInd=%d\n",*ind,attInd);
  // printf("GR_DrawPoint %f,%f,%f\n",pt1->x,pt1->y,pt1->z);
  // printf(" APT_2d_to_3d_mode=%d\n",APT_2d_to_3d_mode);


  // if(APT_2d_to_3d_mode == OFF) {                // OFF=1
    GL_DrawPoint (ind, attInd, pt1);

  // } else {
    // UT3D_pt_tra_pt_m3 (&ptTr, APT_2d_to_3d_Mat, pt1);
    // GL_DrawPoint (ind, attInd, &ptTr);
  // }

  return;
}
*/


/* UU
//====================================================================
  void GR_CreDitto2 (long *ind, Point *pt, double az, char mir, Ditto *di) {
//====================================================================

  long   dlInd;

  // printf("GR_CreDitto2 %d %d\n",di->ind,di->siz);

  dlInd = DL_StoreObj (Typ_Ditto, *ind, 0);
  GR_DrawDitto2 (&dlInd, pt, az, mir, di);

}


//====================================================================
  void GR_DrawDitto2 (long *ind, Point *pt, double az, char mir, Ditto *di) {
//====================================================================
///   Erzeugung 3D-Ditto

  Point pt1;

  // if(APT_2d_to_3d_mode != OFF) {
    // // Achtung: hier fehlt di->vc1 transformieren !!
    // UT3D_pt_tra_pt_m3 (&di->po, APT_2d_to_3d_Mat, &di->po);
    // UT3D_pt_tra_pt_m3 (&pt1, APT_2d_to_3d_Mat, pt);
  // } else {
    pt1 = *pt;
  // }

  GL_DrawDitto2 (ind, &pt1, az, mir, di);

}

// repl by AP_add_ln
//====================================================================
  void GR_CreLine (long *ind, int attInd, Line *ln1) {
//====================================================================
/// \code
/// create DB-record, DL-record and display line
/// Input:
///                0    create new dynamic DB-obj;
///                < 0  do not store obj in DB
///   attInd       see GR_tDyn_ln2_2pt  (see ~/gCAD3D/cfg/ltyp.rc)
///
/// Output:
///   ind          dbi  (negative for dynamic obj)
/// \endcode
 


  long   dlInd;


  if(*ind == 0) {
    *ind = DB_StoreLine ((long)-1, ln1);
  }


  dlInd = DL_StoreObj (Typ_LN, *ind, attInd);

  GR_DrawLine (&dlInd, *ind, attInd, ln1);

}
*/


//====================================================================
  void GR_CreTriaFan (long *ind, int attInd, Point *pc, int anz, Point *pa1) {
//====================================================================
/// GR_CreTriaFan     ein od mehrere Dreiecke mit gemeinsamem Pkt.

  
  long   dlInd;


  /* onjInd erhaelt man beim abspeichern in der DB */
  // ++objInd;
  
  dlInd = DL_StoreObj (Typ_GL_Sur, *ind, attInd);
  /*  IN:  Objectindex objInd  und ObjTyp Typ_xx */
  /*  OUT: DispListIndex dlInd */



  GR_DrawTriaFan (&dlInd, attInd, pc, anz, pa1);
  
  *ind = dlInd;

}


//====================================================================
  void GR_DrawTriaFan (long *ind, int attInd, Point *pc, int anz, Point *pa1) {
//====================================================================
/// Attr f faces sind 4,5 od 6

//   GL_Surf_Ini (ind, attInd);
  // GL_Draw_Ini (ind, attInd);
  // GL_Draw_Ini (ind, 6);  // 4 OK, 10duenn,blau; 2=rot, 10=rot,

  GL_ColSet (&GL_actCol);

  GL_set_fan (pc, anz, pa1, 1, 1);

//   GL_EndList ();

}


//====================================================================
  void GR_CreTriaStrip (long *ind, int attInd,int ptUNr,int ptVNr,Point *pa) {
//====================================================================

  
  long   dlInd;


  /* onjInd erhaelt man beim abspeichern in der DB */
  // ++objInd;
  
  dlInd = DL_StoreObj (Typ_GL_Sur, *ind, attInd);
  /*  IN:  Objectindex objInd  und ObjTyp Typ_xx */
  /*  OUT: DispListIndex dlInd */



  GR_DrawTriaStrip (&dlInd, attInd, ptUNr,ptVNr,pa, Typ_SUR);
  
  *ind = dlInd;

}


//======================================================================
  int GR_DrawTriaStrip(long *ind, int attInd,
                       int ptUNr, int ptVNr, Point *pa, int newS) {
//======================================================================
/// \code
/// used fuer unbegrenzte B-SplineSurface
/// TSU_mode   0=normal darstellen; 1=speichern
/// Attr f faces sind 4,5 od 6   
///   p1       size = ptUNr * ptVNr
///   newS     Typ_SURBSP
/// \endcode

  int   irc, i1;
  long  l1;


  // printf("GR_DrawTriaStrip att=%d %d %d\n",attInd,ptUNr,ptVNr);



  //================================================================
  // normal interaktiv darstellen
  if(TSU_mode == 0)  {      // 0=normal darstellen; 1=speichern

    // GL_Draw_Ini (ind, attInd);
//     GL_Surf_Ini (ind, attInd);

    GL_ColSet (&GL_actCol);

    if(newS) GLT_stor_rec (6, NULL, NULL, newS);

    for(i1=0; i1<ptVNr-1; ++i1) {
      irc = GL_set_strip_v (pa, &pa[ptUNr], ptUNr, 0);
      if(irc < 0) return irc;
      pa += ptUNr;              // advance 1 row
    }

//     GL_EndList ();

    return 0;
  }



  //================================================================
  // zwischenspeichern

  // test size of GLT-buffer for ptUNr*ptVNr points
  // l1 = (ptUNr + 1) * (ptVNr + 1);
  l1 = ptUNr * ptVNr * 2;
  GLT_alloc_pta (l1);


  GLT_stor_rec (0, NULL, NULL, 0);

  if(newS) GLT_stor_rec (6, NULL, NULL, newS);

  for(i1=0; i1<ptVNr-1; ++i1) {
    irc = GL_set_strip_v (pa, &pa[ptUNr], ptUNr, 0);
    if(irc < 0) return irc;
    pa += ptUNr;              // advance 1 row
  }

  // close & store ..
  GLT_stor_rec (1, NULL, NULL, 0);

  return 0;




}


//====================================================================
  void GR_CrePrism (long *ind, int attInd, int anz, Point *pa1, Point *pa2) {
//====================================================================


  long   dlInd;


  /* onjInd erhaelt man beim abspeichern in der DB */
  // ++objInd;

  dlInd = DL_StoreObj (Typ_GL_Sur, *ind, attInd);
  /*  IN:  Objectindex objInd  und ObjTyp Typ_xx */
  /*  OUT: DispListIndex dlInd */



  GR_DrawPrism (&dlInd, attInd, anz, pa1, pa2);

  *ind = dlInd;

}



//====================================================================
  void GR_DrawPrism (long *ind, int attInd, int anz, Point *pa1, Point *pa2) {
//====================================================================


  if(TSU_mode == 0)  {      // 0 = draw OpenGL
    GL_Draw_Ini (ind, attInd);
    GL_set_strip_v (pa1, pa2, anz, 0);
    GL_EndList ();

  } else {
    GLT_stor_rec (0, NULL, NULL, 0);
    GL_set_strip_v (pa1, pa2, anz, 0);
    GLT_stor_rec (1, NULL, NULL, 0);
  }

}

//=======================================================================
  int GR_CreSolSph (long *ind, int att, Sphere *sp1) {
//=======================================================================
/// att == ColRGB !



  int     sStyl;
  ColRGB  *col;

  // printf("GR_CreSolSph \n");


  // fix style; shaded/symbolic
  sStyl = 0;
  col = COL_INT32(&att);    // col = (ColRGB*)&att;
  if(TSU_mode == 0)  {      // 0 = draw OpenGL

    if((APT_dispSOL == OFF) ||
       (col->vsym   == 1))      sStyl = 1;   // 0=ON=shade; 1=OFF=symbolic
  }



  // GLB_DrawInit ();

  if(TSU_mode == 0)  {      // 0 = draw OpenGL
    if(sStyl == 0) {            // 0=sol
      GL_Surf_Ini (ind, col);
      GL_ColSet (&GL_actCol);
    } else {
      GL_Draw_Ini (ind, Typ_Att_dash_long);
    }

    GL_disp_sph (&sp1->pc, sp1->rad, sStyl);
    GL_EndList ();  // damit kein include GL erforderlich ist


  } else {                                // tesselate --> memory ..
    if(sStyl == 0) {
      GLT_stor_rec (0, NULL, NULL, 0);
      if((GL_actCol.color != 0) || (GL_actCol.vtra != 0))
        GLT_stor_rec (5, NULL, NULL, 0);
      GL_disp_sph (&sp1->pc, sp1->rad, sStyl);
      GLT_stor_rec (1, NULL, NULL, 0);
    }
  }



  return 0;

}


//=======================================================================
  int GR_CreSolCon (long *ind, int att, Conus *co1) {
//=======================================================================


  int     sStyl;
  ColRGB  *col;


  // fix style; shaded/symbolic
  sStyl = 0;
  col = COL_INT32(&att);    // col = (ColRGB*)&att;
  if(TSU_mode == 0)  {      // 0 = draw OpenGL

    if((APT_dispSOL == OFF) ||
       (col->vsym   == 1))      sStyl = 1;   // 0=ON=shade; 1=OFF=symbolic
  }


  if(TSU_mode == 0)  {      // 0 = draw OpenGL
    if(sStyl == 0) {
      GL_Surf_Ini (ind, col);
      GL_ColSet (&GL_actCol);
    } else {
      GL_Draw_Ini (ind, Typ_Att_dash_long);
    }

    GL_disp_cone (co1, 7, sStyl);
    GL_EndList ();  // damit kein include GL erforderlich ist

  } else {                         // tesselate --> memory ..
    if(sStyl == 0) {
      GLT_stor_rec (0, NULL, NULL, 0);
      if((GL_actCol.color != 0) || (GL_actCol.vtra != 0))
        GLT_stor_rec (5, NULL, NULL, 0);
      GL_disp_cone (co1, 7, sStyl);
      GLT_stor_rec (1, NULL, NULL, 0);
    }
  }

  return 0;

/*
  int       i1, ptNr, ptMax;
  Point     *pta, ptc;
  Circ      ci1;


  // printf("GR_CreSolCon r1=%f r2=%f h=%f\n",co1->r1,co1->r2,co1->h);

  // nur symbolisch zwei Kreise mit Verbindungslinie darstellen

  pta   = (Point*)memspc55;
  ptMax = sizeof(memspc55) / sizeof(Point);
  ptNr  = 0;

  ptc = co1->pl.po;

  if(co1->r1 > UT_TOL_pt) {
    UT3D_ci_ptvcr (&ci1, &ptc, &co1->pl.vz, co1->r1);
    i1 = UT3D_ptNr_ci (&ci1, UT_DISP_cv);
    if((ptNr + i1) >= ptMax) goto L_EOM;
    UT3D_cv_ci (&pta[ptNr], &i1, &ci1, i1, -1.);
    ptNr += i1;

  } else {
    pta[0] = ptc;
    ptNr = 1;
  }

  UT3D_pt_traptvclen (&ptc, &ptc, &co1->pl.vz, co1->h);

  if(co1->r2 > UT_TOL_pt) {
    UT3D_ci_ptvcr (&ci1, &ptc, &co1->pl.vz, co1->r2);
    i1 = UT3D_ptNr_ci (&ci1, UT_DISP_cv);
    if((ptNr + i1) >= ptMax) goto L_EOM;
    UT3D_cv_ci (&pta[ptNr], &i1, &ci1, i1, -1.);
    ptNr += i1;

  } else {
    pta[ptNr] = ptc;
    ptNr += 1;
  }

  GR_tDyn_pcv (ind, att, ptNr, pta);

  // GL_DrawLn_Ini (ind, att);
  // DL_draw_polygon ...
  // glEnable (GL_LIGHTING);
  // GL_EndList ();



  return 0;


  L_EOM:
  TX_Error("GR_CreSolCon EM001");
  return -1;
*/
}


//================================================================
  int GR_CreSolBrep (long *ind, int att, int sbNr, ObjDB *sba) {
//================================================================
// Edges erstellen; Mesh erstellen;
// alles in ein File raus .. (see ../ut/ut_msh.c)
// Mesh anzeigen. ??
// Meshformat ? (see Landxml-Meshes; ../ut/ut_msh.c)


  printf("GR_CreSolBrep sbNr=%d\n",sbNr);

  return 0;

}

 
//=======================================================================
  int GR_CreSolTor (long *ind, int att, Torus *to1) {
//=======================================================================

  int     i1, sStyl;
  Vector  vcn;
  ContTab ct1;
  ColRGB  *col;
  ObjGX   *oxi;


  // fix style; shaded/symbolic
  sStyl = 0;
  col = COL_INT32(&att);    // col = (ColRGB*)&att;
  if(TSU_mode == 0)  {      // 0 = draw OpenGL

    if((APT_dispSOL == OFF) ||
       (col->vsym   == 1))      sStyl = 1;   // 0=ON=shade; 1=OFF=symbolic
  }



  if(TSU_mode == 0)  {      // 0 = draw OpenGL
    if(sStyl == 0) {
      GL_Surf_Ini (ind, col);
      GL_ColSet (&GL_actCol);
    } else {
      GL_Draw_Ini (ind, Typ_Att_dash_long);
    }

    GL_disp_tor (to1, sStyl);
    GL_EndList ();  // damit kein include GL erforderlich ist


  } else {                              // tesselate --> memory ..
    if(sStyl == 0) {
      L_draw:
      // new Surf; statt GLT_start__
      GLT_stor_rec (0, NULL, NULL, 0);
      if((GL_actCol.color != 0) || (GL_actCol.vtra != 0))
        GLT_stor_rec (5, NULL, NULL, 0);
      // tessel,store via GL_set_strip_v/GLT_stor_rec(2)
      i1 = GL_disp_tor (to1, sStyl);
      if(i1 == -10) goto L_draw;              // realloc
      // close & copy record --> TSU_vM
      GLT_stor_rec (1, NULL, NULL, 0);
    }
  }

  return 0;


/*
  int       i1, ptNr, ptMax;
  Point     *pta, ptc;
  Circ      ci1;

  // printf("GR_CreSolTor r1=%f r2=%f\n",to1->r1,to1->r2);

  // nur symbolisch zwei Kreise mit Verbindungslinie darstellen

  pta   = (Point*)memspc55;
  ptMax = sizeof(memspc55) / sizeof(Point);
  ptNr  = 0;

  ptc = to1->pl.po;
  // UT3D_ci_ptvcr (&ci1, &ptc, &to1->pl.vz, to1->r1+to1->r2);
  UT3D_ci_ptvcr (&ci1, &ptc, &to1->pl.vz, to1->r1);  // ausserster Kreis

  i1 = UT3D_ptNr_ci (&ci1, UT_DISP_cv);
  if((ptNr + i1) >= ptMax) goto L_EOM;
  UT3D_cv_ci (&pta[ptNr], &i1, &ci1, i1, -1.);
  ptNr += i1;

  UT3D_pt_traptptlen (&ci1.pc, &ci1.p1, &ptc, to1->r2);
  ci1.vz  = to1->pl.vy;
  ci1.rad = to1->r2;

  i1 = UT3D_ptNr_ci (&ci1, UT_DISP_cv);
  if((ptNr + i1) >= ptMax) goto L_EOM;
  UT3D_cv_ci (&pta[ptNr], &i1, &ci1, i1, -1.);
  ptNr += i1;

  GR_tDyn_pcv (ind, att, ptNr, pta);

  return 0;

  L_EOM:
  TX_Error("GR_CreSolCon EM001");
  return -1;
*/

}



//=======================================================================
  int GR_CreSol__ (long *ind, int attInd, ObjGX *db1) {
//=======================================================================

  int       i1, irc, pt1Nr, pt2Nr, iAnz, pSiz, sStyl;
  double    d1, *pd;
  ColRGB    *col;
  Vector    vc1, *pvc;
  Point     *pa1, *pa2, *pTab, *pp;
  Circ      ci1, ci2, *pci;
  ObjGX     *oTab;



  // printf("GR_CreSol__ ind=%ld att=%d\n",*ind,attInd);
  // printf(" typ=%d form=%d\n",db1->typ,db1->form);
  // DEB_dump_ox_0 (db1, "GR_CreSol__");
  // DEB_dump_ox_s_ (db1, "GR_CreSol__");



  // die Color / das Attribut
  GL_actCol =  *(COL_INT32(&attInd)); // GL_actCol = *((ColRGB*)&attInd);



  if(db1->typ == Typ_SPH) return GR_CreSolSph (ind, attInd, db1->data); 
  if(db1->typ == Typ_CON) return GR_CreSolCon (ind, attInd, db1->data); 
  if(db1->typ == Typ_TOR) return GR_CreSolTor (ind, attInd, db1->data); 
  if(db1->typ == Typ_BREP)return GR_CreSolBrep(ind, attInd, db1->siz, db1->data); 

  // db1->typ = Typ_SOL

// wie sieht PRISM aus ?


  //============= PRISM ============================================
  // somit ist es eine PRISMA; bestehend aus 2 OGX; Typ=Typ_SOL.
/*
 Ein Prisma besteht aus 2 Konturen;
 Wenn die Konturen unterschiedliche ObjektAnzahlen haben:
 1) zerlegen in Tabelle aus Object+ObjLaenge.
 2) die Laengen prozentuell angleichen und die Objekte entsprechen teilen.
    Ergibt SRU's (ruled Surfaces).

 Bereits existierende Funktionen dazu:
 TSU_DrawSurTRU
  UT3D_cv_sru_
   UT3D_cv_sruCv
 wcg_cpx_break wcg_cpx_teilen wcg_cpx_len_ges wcg_cpx_len_obj usw ...
 GR_Disp_sru
*/



  // see also GR_CrePrism GR_DrawPrism - dzt unused !
  oTab = db1->data;

  // printf(" O1: typ=%d form=%d\n",oTab[0].typ,oTab[0].form);
  // printf(" O2: typ=%d form=%d\n",oTab[1].typ,oTab[1].form);


  // fix style; shaded/symbolic
  col = COL_INT32(&attInd); // col = (ColRGB*)&attInd;

  if((APT_dispSOL == OFF) ||
     (col->vsym   == 1))      sStyl = 1;   // 0=ON=shade; 1=OFF=symbolic
  else                        sStyl = 0;



  // get memSpc
  pTab = (Point*)memspc201;
  pSiz = sizeof(memspc201) / sizeof(Point);



  iAnz = 1024;

  i1 = 2 * iAnz;
  if(pSiz < i1) goto L_err_eom;


  pa1 = &pTab[0];
  pa2 = &pTab[iAnz];


  // man sollte eigentl die optimale Anzahl Segmente ermitteln, wenn
  // upper object auch eine Kontur ist.
  // see TSU_DrawSurTRU UT3D_cv_sru_ UT3D_cv_sruCv


  // ----------------- obj1 -> pa1
  switch(oTab[0].typ) {

    case Typ_CI:
      // ci1 = DB_GetCirc ((long)oTab[0].data);
      // UT3D_cv_ci (pa1, &pt1Nr, &ci1, iAnz, UT_DISP_cv);
      UTO_objDat_ox (&pci, &i1, &oTab[0]);
        // DEB_dump_obj__(Typ_CI, pci, " o1: ");
      UT3D_cv_ci (pa1, &pt1Nr, pci, iAnz, UT_DISP_cv);
      break;


    case Typ_CV:          // !!!! sollte eigentl f alle objekte gehen !!!
    case Typ_CVPOL:
      pt1Nr = iAnz;
      UT3D_npt_ox__ (&pt1Nr, pa1, &oTab[0], UT_DISP_cv);
      break;


    default:;
      TX_Error("GR_CreSol__ E001 %d",oTab[0].typ);
      return -1;
  }


  // ----------------- obj2 -> pa2
  switch(oTab[1].typ) {

    case Typ_PT:
      // pa2[0] = DB_GetPoint ((long)oTab[1].data);
      irc = UTO_objDat_ox (&pp, &i1, &oTab[1]);
      if(irc < 0) return irc;
      pa2[0] = *pp;   // copy the pt
      // printf("o2p=%f,%f,%f\n",pa2[0].x,pa2[0].y,pa2[0].z);
      pt2Nr = 1;
      break;


    case Typ_CI:
      // ci2 = DB_GetCirc ((long)oTab[1].data);
      // UT3D_cv_ci (pa2, &pt2Nr, &ci2, pt1Nr, -0.1);
      UTO_objDat_ox (&pci, &i1, &oTab[1]);
      UT3D_cv_ci (pa2, &pt2Nr, pci, pt1Nr, -0.1);
      break;


    case Typ_VC:
      irc = UTO_objDat_ox (&pvc, &i1, &oTab[1]);
      if(irc < 0) return irc;
      vc1 = *pvc;   // copy the vec
      goto L_translate;


    case Typ_VAR:
      // printf("VAR # %d\n",oTab[1].data);
      // printf("ci1=%f,%f,%f\n",ci1.vz.dx,ci1.vz.dy,ci1.vz.dz);
      // val d1 aus DB -> d1 holen
      // d1 = DB_GetVar ((long)oTab[1].data); // get Dicke
      irc = UTO_objDat_ox (&pd, &i1, &oTab[1]);
      if(irc < 0) return irc;
      UT3D_vc_perpcv (&vc1, pt1Nr, pa1);  // Normalvektor an Kontur1
      UT3D_vc_setLength (&vc1, &vc1, *pd);
      // printf("vc1=%f,%f,%f\n",vc1.dx,vc1.dy,vc1.dz);
      goto L_translate;


    default:;
      TX_Error("GR_CreSol__ E002 %d",oTab[1].typ);
      return -1;
  }

  goto L_draw;



  // gesamtes Arr. pa1 um vc1 --> pa2 transformieren
  L_translate:
      for(i1=0; i1<pt1Nr; ++i1) {
         UT3D_pt_traptvc (&pa2[i1], &pa1[i1], &vc1);
      }
      pt2Nr = pt1Nr;




  //================================================================
  L_draw:

  if(TSU_mode == 0)  {       // 0 = draw OpenGL
    if(sStyl == 0)           // disp shaded
      GL_Surf_Ini (ind, col);
    else
      GL_Draw_Ini (ind, Typ_Att_dash_long);

    GL_disp_prism (pt1Nr, pa1, pt2Nr, pa2, sStyl);
    GL_EndList ();

  } else {                   // tesselate --> memory ..
    if(sStyl == 0)           // disp shaded
      GL_disp_prism (pt1Nr, pa1, pt2Nr, pa2, sStyl);
  }



  return 0;


  L_err_eom:
    TX_Error("GR_CreSol__ EOM");
    return -1;

}


//====================================================================
  void GR_CreCyl (long *ind,int attInd,Point *pt1, Point *pt2,
                                       double rd1, double rd2) {
//====================================================================


  long   dlInd;


  /* onjInd erhaelt man beim abspeichern in der DB */
  // ++objInd;

  dlInd = DL_StoreObj (Typ_CON, *ind, attInd);
  /*  IN:  Objectindex objInd  und ObjTyp Typ_xx */
  /*  OUT: DispListIndex dlInd */




  GR_DrawCyl (&dlInd, attInd, pt1, pt2, rd1, rd2);

  *ind = dlInd;

}


/*
//================================================================
  void GR_CreTor (long *ind, int attInd, Circ *ci1, double rd1) {
//================================================================

  long   dlInd;



  dlInd = DL_StoreObj (Typ_TOR, *ind, attInd);

  GR_DrawTor (&dlInd, attInd, ci1, rd1);

  *ind = dlInd;

}
*/



//====================================================================
  void GR_CreTorSeg (long *ind, int attInd, Circ *ci1, Circ *ci2) {
//====================================================================


  long   dlInd;


  /* onjInd erhaelt man beim abspeichern in der DB */
  // ++objInd;

  dlInd = DL_StoreObj (Typ_TOR, *ind, attInd);

  GR_DrawTorSeg (&dlInd, attInd, ci1, ci2);

  *ind = dlInd;

}



//====================================================================
  void GR_tDyn_ci2 (long *dli, long dbi, int attInd, Circ2 *cii) {
//====================================================================
/// \code
///   GR_tDyn_ci2          draw 2D-circle
/// Input:
///   ind          dli
///   attInd       see GR_tDyn_ln2_2pt  (see ~/gCAD3D/cfg/ltyp.rc)
/// \endcode

  int     ptNr;
  double  z1=0.;
  Point2  *pa2;


  // DEB_dump_obj__ (Typ_CI2, cii, "GR_tDyn_ci2 dli=%ld, dbi=%ld att=%d",
                  // *dli, dbi, attInd);
    

  // fix nr of points according to modelsize
  ptNr = UT2D_ptNr_ci (fabs(cii->rad), fabs(cii->ango), UT_DISP_cv);
     // printf(" ptNr=%d\n",ptNr);

  // get memory      dzt GLT_pta; better use MEM_alloc_tmp (alloca)
  pa2 = (Point2*)MEM_alloc_tmp((int)(sizeof(Point2)*ptNr));


  // circ2 -> polygon
  UT2D_npt_ci (pa2, ptNr, cii);


// TODO: backplane,z-val ??
  // display polygon
  // z1 = ci1->pc.z;
  // GR_tDyn_pcv2D (dli, attInd, ptNr, pa2, z1);
  // GL2D_Z = z1
  GR_tDyn_pcv2 (pa2, ptNr, attInd);


//   // disp direction
//   if(APT_dispDir) {
//     int   ipe;
//     Point pt31, pt32;
//     ipe = ptNr - 1;
//     // make 3D-points
//     UT3D_pt_pt2_z (&pt31, &pa2[ipe], z1);
//     UT3D_pt_pt2_z (&pt32, &pa2[ipe - 1], z1);
//     APT_disp_dir (&pt31, &pt32);
//   }

}

/* UU
//====================================================================
  void GR_Draw_ln2 (long *ind, long dbi, int iAtt, Line2 *ln1) {
//====================================================================
/// \code
///   GR_Draw_ln2          draw 2D-line
/// Input:
///   ind          dli
///   attInd       see GR_tDyn_ln2_2pt  (see ~/gCAD3D/cfg/ltyp.rc)
/// \endcode
// see GR_DrawLine GR_tDyn_pcv

  // DEB_dump_obj__(Typ_LN2, ln1, "GR_Draw_ln2 %d", iAtt);

  GR_typ = Typ_LN2;
  GR_dbi = dbi;


  GL_Draw_ln2 (ind, iAtt, ln1);

  // disp direction
  if(APT_dispDir) {
    APT_disp_dir (&ln1->p2, &ln1->p1);
  }

}


// UU
//================================================================
  int GR_Draw_oid_dir_npt (long *dli, Point *pta, int ptNr) {
//================================================================
// GR_Draw_oid_dir_npt             disp. ObjID, direction
// using GR_typ, GR_dbi

  char  oid[32];
  Point pt1;


  // printf("GR_Draw_oid_dir_npt ptNr=%d\n",ptNr);


  // in subModel - no ID, dir
  if(MDL_IS_SUB) return 0;
  if(DL_OBJ_IS_HIDDEN(*dli)) return 0;


  //----------------------------------------------------------------
  if(!APT_dispNam) goto L_disp_dir;
  // display ObjID; see APT_disp_nam
    // printf(" Draw_oid_dir-disp-objID\n");
  // make oid = objID to display
  oid[0] = '_';
  APED_oid_dbo__ (&oid[1], GR_typ, GR_dbi);
  // get pt1 = centerpoint odf polygon
  UT3D_pt_mid_pta (&pt1, pta, ptNr, 1);
    // DEB_dump_obj__(Typ_PT, &pt1, " pt1: ");
  // disp ID
  // GR_tDyn_txtA (0, &pt1, oid);
  GR_tDyn_txtA (&pt1, oid, 0);


  //----------------------------------------------------------------
  L_disp_dir:
//     // display direction
//     if(APT_dispDir) {
//       // disp-dir or statu-nascendi is on
//       APT_disp_dir (&pta[ptNr - 1], &pta[ptNr - 2]);
//     }

  return 0;

}


// UNUSED
//================================================================
  int GR_DrawCurv (long *dli, int att, ObjGX *cv1, double zval) {
//================================================================
/// \code
/// att:                 see ~/gCAD3D/cfg/ltyp.rc
/// see APT_DrawCurv GR_Draw_dbo UTO_obj_Draw__
/// \endcode

  int       form;
  long      dbi = 0L;
  DL_Att    *att1;
  Memspc    tSpc1;


  form = cv1->form;

  // printf("GR_DrawCurv dli=%ld att=%d\n",*dli,att);
  // DEB_dump_obj__ (Typ_ObjGX, cv1, "  cv1:");


  if(form == Typ_CVPSP3) {
    GR_tDyn_psp3 (dli, att, cv1, zval);

  } else if(form == Typ_CVBSP) {
    if(*dli > 0) { DL_dlRec__dli (&att1, *dli); dbi = att1->ind; }
    else dbi = 0L;
    GR_DrawCvBSp (dli, dbi, att, cv1->data);

  } else if(form == Typ_CVRBSP) {
    GR_DrawCvRBSp (dli, att, cv1->data);

  } else if(form == Typ_CVPOL) {
    GR_DrawCvPol (dli, att, cv1->data);

  } else if(form == Typ_CVPOL2) {
    GR_tDyn_pcv2D (dli, att, cv1->siz, cv1->data, zval);

  } else if(form == Typ_CVELL) {
    GR_DrawCvEll (dli, att, cv1->data);

  } else if(form == Typ_CVTRM) {
    // get dbi; get dl-record for dbi from dli
    if(*dli > 0) { DL_dlRec__dli (&att1, *dli); dbi = att1->ind; }
    else dbi = 0L;
    // GR_set_ccv (dli, att, cv1->data);  korr 2007-08-10
    UME_init (&tSpc1, memspc201, sizeof(memspc201));
    GR_set_ccv (dli, dbi, att, cv1, &tSpc1);

// TODO: test for 2D-Curve
  // } else if(form == Typ_CVTRM2) {
    // GR_tDyn_ccv2 (ind, att, cv1);

  } else {
    TX_Error("GR_DrawCurv E001-%d-%d",cv1->typ,cv1->form);
    return -1;
  }

  return 0;

}
*/


//====================================================================
  void GR_CreDisk (long *ind, int attInd, Point *pc, Vector *vz,
                                          double rd1, double rd2) {
//====================================================================
///   Erzeugung einer Disk; rd1 = innerer Rad, rd2 = ausserer rad.


  long   dlInd;


  // ++objInd;

  dlInd = DL_StoreObj (Typ_TOR, *ind, attInd);

  GR_DrawDisk (&dlInd, attInd, pc, vz, rd1, rd2);

  *ind = dlInd;


}



//====================================================================
  void GR_DrawDisk (long *ind, int attInd, Point *pc, Vector *vz,
                                          double rd1, double rd2) {
//====================================================================
///   Erzeugung einer Disk; rd1 = innerer Rad, rd2 = ausserer rad.



  int    i1, p1Anz, iAnz, FlagLoch;
  Vector vx;
  Circ   cii, cia;
  Point  *pa1, *pa2;


  // printf("GR_DrawDisk %.2f,%.2f\n",rd1,rd2);


  /* optimale Punkteanzahl */
  iAnz = 1024;

  if(GLT_pta_SIZ < iAnz * 2) {
    i1 = GLT_alloc_pta (iAnz);
    if(i1 == -1) return;
  }

  pa1 = &GLT_pta[0];
  pa2 = &GLT_pta[1024];


  /* Fan od Stripe; 0=Stripe, 1=Fan(volle Scheibe) */
  FlagLoch = UTP_comp_0(rd1);

  /* den Aussenkreis */
  cia.pc   = *pc;
  cia.rad  = rd2;
  cia.ango = RAD_360;

  /* create the X-vector = Normalvektor auf vc, der in der X-Y-Plane liegt. */
  cia.vz = *vz;
  UT3D_vc_perpvcplXY (&vx, &cia.vz);


  /* einen Startpunkt aussen */
  UT3D_pt_traptvclen (&cia.p1, &cia.pc, &vx, cia.rad);
  cia.p2 = cia.p1;


  /* den ci1 in eine Polygon teilen */
  UT3D_cv_ci (pa1, &p1Anz, &cia, iAnz, -1.0);

  /* den ersten Punkt nochmals hinten dran */
  /* GL_ptArr30[p1Anz] = GL_ptArr30[0]; */
  /* ++p1Anz; */


  /* den Innenkreis */
  if(FlagLoch == 0) {

    cii.pc   = *pc;
    cii.rad  = rd1;
    cii.vz   = *vz;
    cii.ango = RAD_360;

    /* einen Startpunkt aussen */
    UT3D_pt_traptvclen (&cii.p1, &cii.pc, &vx, cii.rad);
    cii.p2 = cii.p1;


    /* den ci1 in eine Polygon teilen */
    UT3D_cv_ci (pa2, &p1Anz, &cii, iAnz, -0.1);

  }






  GL_Draw_Ini (ind, attInd);


  if(FlagLoch == 0) {
    // GL_set_strip_v (p1Anz, GL_ptArr31, GL_ptArr30);
    GL_set_strip_v (pa2, pa1, p1Anz, 1);
  } else {
    GL_set_fan (pc, p1Anz, pa1, 0, 1);
  }


  GL_EndList ();



}




//====================================================================
  void GR_DrawCyl (long *ind, int attInd, Point *pt1, Point *pt2,
                                          double rd1, double rd2) {
//====================================================================
/// \code
///   Erzeugung eines Zylinders / Konus   (Cyl / Cone)
/// See also GL_draw_cone GL_set_patch GL_set_fan
///          GL_set_strip1 GL_set_strip_v GL_DrawRCone
/// \endcode



  Conus   con;
  Vector  vz;

  // int    ptAnz, iAnz;
  // Circ   ciu, cio;
  // Vector vx;

/*
  TX_Print("GR_DrawCyl %f,%f,%f\n",pt1->x,pt1->y,pt1->z);
  TX_Print("         - %f,%f,%f\n",pt2->x,pt2->y,pt2->z);
  TX_Print("         r %f,%f\n",rd1,rd2);
*/


  UT3D_vc_2pt (&vz, pt1, pt2);
  UT3D_pl_ptvc (&con.pl, pt1, &vz);

  con.r1 = rd1;
  con.r2 = rd2;
  con.h  = UT3D_len_2pt (pt1, pt2);



  GR_CreSolCon (ind, attInd, &con);


/*
  ciu.pc = *pt1;
  cio.pc = *pt2;

  ciu.rad  = rd1;
  ciu.ango = RAD_360;
  cio.rad  = rd2;
  cio.ango = RAD_360;


  // zwei Kreise sind notwendig; zuerst die Z-Vektoren
  UT3D_vc_2pt (&ciu.vz, pt1, pt2);
  cio.vz = ciu.vz;

  // create the X-vector = Normalvektor auf vc, der in der X-Y-Plane liegt.
  UT3D_vc_perpvcplXY (&vx, &ciu.vz);


  // einen Startpunkt unten u oben erzeugen
  UT3D_pt_traptvclen (&ciu.p1, pt1, &vx, rd1);
  UT3D_pt_traptvclen (&cio.p1, pt2, &vx, rd2);

  ciu.p2 = ciu.p1;
  cio.p2 = cio.p1;

  iAnz = 64;

  UT3D_cv_ci (GL_ptArr30, &ptAnz, &ciu, iAnz, -0.1);
  UT3D_cv_ci (GL_ptArr31, &ptAnz, &cio, iAnz, -0.1);


  GL_DrawRSur (ind, attInd, ptAnz, GL_ptArr30, GL_ptArr31);
*/
  return;

}





/*
//================================================================
  void GR_DrawTor (long *ind, int attInd, Circ *ci1, double rd1) {
//================================================================

  int    p1Anz, p2Anz, iAnz, ii1;
  // Point  pt1, pt2;
  Circ   ct1, ct2;
  Vector vc2;



  printf("GR_DrawTor %.2f\n",rd1);


  // den ci1 in eine Polygon teilen
  UT3D_cv_ci (GL_ptArr32, &p1Anz, ci1, 64, ci1->rad/100.0);



  ct1.rad = rd1;
  ct2.rad = rd1;


  ii1 = 0;

  GL_Draw_Ini (ind, attInd);


  // =============================
  NextSeg:


  // ------------------------
  ct1.pc  = GL_ptArr32[ii1];
  
  // Vektor zur Mitte
  UT3D_vc_2pt (&vc2, &ct1.pc, &ci1->pc);
  // in Richtung ci1-Center Anfangspkt abschlagen
  UT3D_pt_traptvclen (&ct1.p1, &ct1.pc, &vc2, rd1);
  ct1.p2 = ct1.p1;
  // Startvektor = Kreuzprod. aus vc1 und ci1.vz
  UT3D_vc_perp2vc (&ct1.vz, &vc2, &ci1->vz);
  // wenn der Rad. neg, umdrehen
  if(ci1->rad < 0.0) UT3D_vc_invert (&ct1.vz, &ct1.vz);


  // nur Test:
  // GR_CreCirc (ind, attInd, &ct1);



  // -------------------
  ct2.pc  = GL_ptArr32[ii1+1];


  // Vektor zur Mitte
  UT3D_vc_2pt (&vc2, &ct2.pc, &ci1->pc);
  // in Richtung ci1-Center Anfangspkt abschlagen
  UT3D_pt_traptvclen (&ct2.p1, &ct2.pc, &vc2, rd1);
  ct2.p2 = ct2.p1;
  // Startvektor = Kreuzprod. aus vc1 und ci1.vz
  UT3D_vc_perp2vc (&ct2.vz, &vc2, &ci1->vz);
  // wenn der Rad. neg, umdrehen
  if(ci1->rad < 0.0) UT3D_vc_invert (&ct2.vz, &ct2.vz);

  // nur Test:
  // GR_CreCirc (ind, attInd, &ct2);



  // -------------------
  // ZylSur zwischen ct1 und ct2
  iAnz = 64;

  UT3D_cv_ci (GL_ptArr30, &p2Anz, &ct1, iAnz, -0.1);
  UT3D_cv_ci (GL_ptArr31, &p2Anz, &ct2, iAnz, -0.1);


  // GL_DrawRSur (ind, attInd, p2Anz);
  // GL_set_strip_v (p2Anz, GL_ptArr30, GL_ptArr31);
  GL_set_strip_v (GL_ptArr30, GL_ptArr31, p2Anz);


  ++ii1;
  if(ii1 < (p1Anz-1)) { 
    // ++objInd;
    // *ind = DL_StoreObj (Typ_TOR, objInd, attInd);
    goto NextSeg;
  }


  GL_EndList ();


}


//================================================================
  int GR_DrawSurPtab (long dbi, int att) {
//================================================================
// display PTAB (its boundary)
// load pointSet from bin-file, compute boundary, display.
//
// Input:
//   dli       -1    create new DL-record,
//             >= 0  else use existing DL-record
//   dbi
//   att       ? 5 ?
//   file      binary-data of pointSet, see MSH_bload_pTab/MSH_bsav_pTab
// Output:
//   dli


  int     *iba, iNr, PtabNr, i1;
  long    dli;
  MemTab(Point) pTab = _MEMTAB_NUL;


  printf("GR_DrawSurPtab att=%d dbi=%ld\n",att,dbi);

  MemTab_ini__ (&pTab, sizeof(Point), Typ_PT, 10000);


  // override att:  set to 5=th2BlueDashed
  att = 5;

// see also gis_bload_pTab

  // load pTab from bin.file; mallocs Ptab !
  i1 = MSH_bload_pTab (&pTab, AP_modact_nam, dbi);
  if(i1 < 0) return -1;
  PtabNr = pTab.rNr;
    printf(" PtabNr=%d\n",PtabNr);

  // get space for boundary
  iba = (int*)memspc101;
  iNr = sizeof(memspc101) / sizeof(int);    // 4 byte
  // if(iNr < PtabNr) {TX_Error("GR_DrawSurPtab E001"); return -1;}

  // get boundary (iTab) for n points (pTab)
  i1 = MSH_ibnd_ptn (iba, &iNr, pTab.data, pTab.rNr);
  if(i1 < 0) goto L_done;

  // // Testdisplay of boundary
  // MSH_test_disp2 (iba, iNr, Ptab);

//   dli = DL_StoreObj (Typ_SUR, dbi, att);
  dli = AP_dli_act;
  GL_att_cv
  GL_set_ipcv (&dli, att, iba, iNr, pTab.data, 1);
//   AP_dli_act = dli;

//   // get spce for curve
//   cv = alloca(sizeof(Point)*(iNr+1));
// 
// 
//   for(i1=0; i1<iNr; ++i1) {
//     // printf(" P[%d] ib=%d\n",i1,ib[i1]);
//     // printf("       ia=%d\n",ia[ib[i1]]);
//     cv[i1] = Ptab[iba[i1]];
//   }
//   cv[iNr] = cv[0];
// 
//   // GR_tDyn_pcv (cv, iNr+1, 5);
// 
// 
//   GR_tDyn_pcv (&dli, att, iNr+1, cv);


    // printf(" <<<<<<<<<<<<<<<<<<<GR_DrawSurPtab free pTab %p\n",Ptab);
  L_done:
  MemTab_free (&pTab);

  return 0;

}
*/


//=============================================================================
  int GR_DrawSur (ObjGX *oxi, int att, long apt_ind) {
//=============================================================================
/// \code
/// see also APT_DrawSur
/// Input:
///   oxi            surface; see list below
///   att            color|texture
///   apt_ind        0 = do-NOT-create-new-DL-record (add into already open DL-rec)
///                  >0 create new DL-record
///                  overwrite existing DL-record: see DL_SetInd
/// Output:
///   AP_dli_act     dispListIndex of surf created
///
/// ------------------------------------------------------
/// GR_DrawSur
///   GR_DrawSurPtab                   -
///   TSU_DrawSurT_
///     TSU_DrawSurTRU                 -
///     TSU_DrawSurTRV                 -
///     TSU_DrawSurTS                  -
///     TSU_DrawSurTP1                  -
///     TSU_DrawSurTC                  -
///   GR_CreFan                        -
///   GR_DrawStrip                     -
///   TSU_DrawSurPMsh                   -
///   TSU_DrawSurBsp                   -
///   TSU_DrawSurRBsp                  -
/// GR_CreSol__
///   GR_CreSolSph                     -
///   GR_CreSolCon                     -
///   GR_CreSolTor                     -
/// \endcode



  int       i1, i2, ptNr, ptMax, irow1, irow2, cNr;
  long      ind, dli, l1, l2;
  double    d1, d2, d3;
  ObjGX     *oxp1, *oxp2, *oxa, ox1, ox2;
  // ObjGX     *suc1;
  SurStripe *sus1;
  SurBSpl   *sub1;
  SurRBSpl  *rsub1;
  SurStd    *sus;
  SurHat    *suh;
  SurRev    *surv;
  Point     pt1, *pTab, *pa1, *pa2;
  Plane     pl1;
  Vector    vc1, vc2;
  Line      ln1, ln2, *lTab;
  Circ      ci1, ci2, *cip1;
  ContTab   *cTab;
  // CurvElli  *elp1;
  Memspc    workSeg;

// static char memspc502[2500000];
  char      cBuf[256];



  // printf("GR_DrawSur ind=%ld typ=%d form=%d siz=%d\n",apt_ind,
          // oxi->typ,oxi->form,oxi->siz);
  // printf(" TSU_mode=%d\n",TSU_get_mode());
  // DEB_dump_ox_s_ (oxi, "GR_DrawSur");
  // if(oxi->typ==Typ_SUR) printf(" S:vtex=%d\n",((ColRGB*)&att)->vtex);


  // skip dummies (A=PTAB ..)
  // if(oxi->typ == Typ_ObjGX) return 0;
  // if(oxi->typ == Typ_Typ) return 0;
  // if(oxi->siz < 1) return 0;      // Typ_SURPTAB skip A=PTAB
  // if(oxi->typ == Typ_SURPTAB) return GR_DrawSurPtab (apt_ind, att);
  if(oxi->typ == Typ_SURPTAB) return GR_disp_cv_pMesh (apt_ind, att);



  // do not export hidden objets for Mockups ..
  if(TSU_mode == 1) {  // 0=display_normal; 1=export,intersect ...
    i1 = GA_hide__ (8, apt_ind, Typ_SUR); // ask state; 1=hidden
    // printf(" hide=%d\n",i1);
    if(i1 == 1) return 0;
  }


  // Init Speicherbereich fuer Pointtabelle memspc501 = 500K
  pTab = (void*) memspc501;
  ptMax = sizeof(memspc501) / sizeof(Point);
  // ACHTUNG: GLT_pta unbrauchbar, da es beim ExportMockup benutzt wird !!!
  // pTab = GLT_pta;
  // ptNr = GLT_pta_SIZ;
  // printf(" ptNr=%d\n",ptNr);


  // die Color / das Attribut
  GL_actCol = *(COL_INT32(&att)); // GL_actCol = *((ColRGB*)&att);


  // Texure
  if (GL_actCol.vtex != 0) {
    // find textureNr of surface <apt_ind> from (typ,dbi)
    l1 = GA_find__ (Typ_SUR, apt_ind);
      // printf(" GA-nr=%d\n",l1);
    if(l1 < 0) {
      printf("GR_DrawSur I001 %ld\n",apt_ind);
      GL_actTex = -1;
    } else {
      // get TexRef of GA-Record l1
      GL_actTex = GA_tex_ga2tr (l1);
    }
  } else {
    GL_actTex = -1;
  }
        // printf(" _DrawSur typ=%d ind=%ld GL_actTex=%d\n",oxi->typ,apt_ind,
               // GL_actTex);




  //--------------------------------------------
  if(oxi->typ == Typ_SUR)      goto L_SurCplx;
  if(oxi->typ == Typ_SURRU)    goto L_SurCplx;
  if(oxi->typ == Typ_SURRV)    goto L_SurCplx;
  if(oxi->typ == Typ_SURSWP)   goto L_SurSwp;
  if(oxi->typ == Typ_SURCIR)   goto L_SurCir;
  if(oxi->typ == Typ_SURSTRIP) goto L_SurStripe;
  if(oxi->typ == Typ_SURBSP)   goto L_SurBSpl;
  if(oxi->typ == Typ_SURRBSP)  goto L_SurRBSpl;
  if(oxi->typ == Typ_SURPMSH)   goto L_SurMesh;
  if(oxi->typ == Typ_SURHAT)   goto L_SurHat;

    TX_Error("GR_DrawSur E001 %d",oxi->typ);
    return -1;




  //=======================================================================
  //---------------------- draw StdSur  ----------------------------
/*
  L_SurStd:


  // form=Typ_ObjGX ist eine FSUB-Flaeche;
  if(oxi->form == Typ_ObjGX) {             // 143

    for(i1=0; i1<oxi->siz; ++i1)
      printf("typ[%d]=%d\n",i1,((ObjGX*)oxi->data)[i1].typ);


    // wenn das Objekt Nr.1 eine Flaeche ist, ... noch nix tun..
    if(((ObjGX*)oxi->data)[0].typ == Typ_SUR)  return 0;




    // Planare Flaeche - getrimmt, gelocht:
    // Typ_SUR/Typ_ObjGX; Obj1 ist keine Flaeche;


    // prepare space for contours
    cTab = (ContTab*)memspc101;
    cMax = sizeof(memspc101) / sizeof(ContTab);
    cNr  = 0;

    // Aussen- und Innenkonturen holen
    SUP_load_c (cTab, &cNr, cMax, pTab, ptNr, oxi);

    // Z-Vektor errechnen
    // SUP_tr_set_ ();
    UT3D_pl_pta (&pl1, cTab[0].iNr, pTab); // anzahl Elemente 1. Aussenkontur
    vc1 = pl1.vz;

    // Orientieren der Konturen;
    // Aussenkontur muss CCW, alle Innenkonturen muessen CW sein.
    SUP_orient (&vc1, cTab, cNr);

    dli = DL_StoreObj (Typ_SUR, apt_ind, att);
    GL_Draw_Ini(&dli, att);   // glNewList
    GLT_spp__ (cTab, cNr, &vc1);
    GL_EndList();
    // GLT_pln__(att, apt_ind, cTab, cNr, &vc1, 1);

    return 0;
  }

  //----------------------------------------------------------------------
  sus = oxi->data;
  printf("StdSur typ=%d baseID=%d cvTyp=%d cvID=%d\n",sus->typ,sus->baseID,
                 sus->cvTyp,sus->cvID);


  // SurStd-Boundary -> Polygon umwandeln
  i1 = UT3D_pta_sus (&ptNr, pTab, sus, UT_DISP_cv);
  if(i1 < 0) return i1;



  // Stuetzflaeche decodieren
  if(sus->typ == Typ_SUR) {          // Sur-RU od Sur-BSP
    oxp1 = DB_GetSur (sus->baseID, 1);
    // printf("  stdTyp=%d\n",oxp1->typ);


  } else if(sus->typ == Typ_SOL) {   // 34 = Conus od Torus
    oxp1 = DB_GetSol (sus->baseID);
    // printf("  stdTyp=%d\n",oxp1->typ);


  } else if(sus->typ == Typ_PLN) {
    oxp1       = &ox1;
    oxp1->typ  = Typ_PLN;
    oxp1->data = NULL;


  } else {     // muss sein Body (Conus od Torus)
    TX_Error("GR_DrawSur E006 %d",sus->typ);
    return -1;
  }

  dli = DL_StoreObj (Typ_SUR, apt_ind, att);
  GL_Draw_Ini (&dli, att);


  //            Kontur TypFlaeche Flaeche
  SUS_tess_std (ptNr, pTab, oxp1->typ, oxp1->data);

  GL_EndList ();

  return 0;
*/


  //=======================================================================
  L_SurSwp:        // Sweep; 2014-01-25
    i1 = Tess_sur__ (oxi, att, apt_ind);
    if(i1 < 0) {
      sprintf(cBuf,"degenerate element A%ld",apt_ind);
      if(MDL_IS_SUB) {
        strcat(cBuf, " in Submodel ");
        strcat(cBuf, DB_mdlNam_iBas(AP_modact_ibm));
      }
      TX_Print(cBuf);
    }
    return i1;





  //=======================================================================
  L_SurCplx:
    // Tesselate / Display  trimmed/punched surf
    i1 = TSU_DrawSurT_ (oxi, att, apt_ind);
    if(i1 < 0) {
      sprintf(cBuf,"degenerate element A%ld",apt_ind);
      if(MDL_IS_SUB) {
        strcat(cBuf, " in Submodel ");
        strcat(cBuf, DB_mdlNam_iBas(AP_modact_ibm));
      }
      TX_Print(cBuf);
    }
    AP_dli_act = TSU_dli_get ();
    return i1;






  //=======================================================================
  //---------------------- draw hatch  ----------------------------
  L_SurHat:

  suh = oxi->data;
    // printf("Hatch typ=%d dbi=%d %f %f\n",suh->cvTyp,suh->cvID,
            // suh->off,suh->dir);

  // ox1  = DB_GetObjGX (Typ_CVTRM, suh->cvID);

  lTab = (Line*)memspc55;
  i2   = sizeof(memspc55) / sizeof(Line);

  i1 = UT_hatch__ (&i2, lTab, (int)suh->cvTyp, (long)suh->cvID,
                   (double)suh->off, (double)suh->dir);
  if(i1 < 0) return i1;


  // i1 = UT_hatch_ccv (&i2, lTab, &ox1, suh->off, suh->dir);
  // if(i1 < 0) return i1;


  // TESTAUSG:
  // for(i1=0; i1<i2; ++i1) {
    // DEB_dump_obj__ (Typ_LN,&lTab[i1],"L[%d]\n",i1);
    // dli = -1; GR_CreLine (&dli, 1, &lTab[i1]);
  // }


  dli = DL_StoreObj (Typ_SUR, apt_ind, 1);
  GL_DrawLn_Ini (&dli, 1);

  for(i1=0; i1<i2; ++i1)  GL_set_ln_2pt (&lTab[i1].p1, &lTab[i1].p2);
  // GL_set_pcv (i2, lTab);

  GL_EndList ();

  AP_dli_act = dli;

  return 0;





  //=======================================================================
  //=======================================================================
  L_SurCir:

    i1 = TSU_DrawRCIR (oxi, att, apt_ind);
    return 0;




  //======================================================
  L_SurStripe:

  if(TSU_mode == 0) {
    dli = DL_StoreObj (Typ_SUR, apt_ind, att);
    GL_Surf_Ini (&dli, att);
  }

/*
  oxa  = oxi->data;
  oxp1 = &oxa[0];
  irow1 = oxi->siz;
  irow2 = oxp1->siz;
    // printf(" STRIP %d %d\n",irow1,irow2);
  for(i1=1; i1<irow1; ++i1) {
    oxp2 = &oxa[i1];
    // GL_set_strip_v ((Point*)oxp1->data, (Point*)oxp2->data, irow2);
    GR_DrawStrip ((Point*)oxp1->data, (Point*)oxp2->data, irow2, Typ_SUR);
    oxp1 = oxp2;              // advance 1 row
  }
*/

  sus1 = oxi->data;
  irow1 = sus1->ptUNr;
  irow2 = sus1->ptVNr;
    // printf(" SurStripe %d %d\n",sus1->ptUNr,sus1->ptVNr);

  // get all points -> pTab
  i2 = irow1 * irow2;
  if(i2 > ptMax) { TX_Error("GR_DrawSur EOP SurStripe"); return -1;}
  oxa = sus1->pTab;

  // get points from dbo-tab
  // i1 = UT3D_npt_obj (&ptNr, pTab, Typ_ObjGX, oxa, i2, UT_DISP_cv);
  ptNr = 0;
  i1 = UT3D_npt_obj (&ptNr, pTab, ptMax, Typ_ObjGX, oxa, i2, UT_DISP_cv,
                     APT_obj_stat);
  if(i1 < 0) return -1;

  // display
  // GR_DrawTriaStrip (&dli, att, sus1->ptUNr, sus1->ptVNr, pTab, Typ_SURSTRIP);
  pa1 = &pTab[0];
  for(i1=1; i1<irow2; ++i1) {
    pa2 = &pa1[irow1];
    GR_DrawStrip (pa1, pa2, irow1, Typ_SUR);
    pa1 = &pa2[1];
  }


  if(TSU_mode == 0)
    GL_EndList ();

  AP_dli_act = dli;

  return 0;




  //=======================================================================
  L_SurMesh:            // untrimmed unperforated Mesh
    return TSU_DrawSurPMsh (oxi, att, apt_ind);


  //======================================================
  //  B-SplineSurface
  L_SurBSpl:
    return TSU_DrawSurBsp (oxi, att, apt_ind);


  //======================================================
  //  Rat.B-SplineSurface
  L_SurRBSpl:
    return TSU_DrawSurRBsp (oxi, att, apt_ind);


  //----------------------------------------------------------------
  L_EOM:
  TX_Error("GR_DrawSur EOM (increase Modelsize (Standards/Tolerances))");
  return 0;

  L_not_supp:
  TX_Error("GR_DrawSur E003 %d",oxp1->typ);
  return -1;

}


/*  UU
//=======================================================================
  int GR_DrawModel (long db_ind, int att, ModelRef *mdr) {
//=======================================================================

  int        irc, i1, loadMode;
  long       dli;
  char       ffnam[256], mnam[128], ftyp[32];
  double     az1, ay, az2;
  ModelBas   *mdb;
  Memspc     impSpc;
  ObjGX      oTab[4];
  Point      pb1, pb2, pOri;


  printf("GR_DrawModel ind=%ld att=%d\n",db_ind,att);
  printf(" TSU_mode=%d APT_obj_stat=%d\n",TSU_mode,APT_obj_stat);
  // DEB_dump_obj__ (Typ_Model, mdr, " DrawModel-mdr");
  // return;

  GR_typ = Typ_SubModel;
  GR_dbi = db_ind;



  if(TSU_mode != 0) {       // 0=Normal 1=Store
    printf(" !!!!!!!!!!!!!!! DrawModel mit TSU_mode=Store %d\n",mdr->modNr);
    return TSU_exp_Mod (0, mdr);   // 2008-11-09
  }


  mdb = DB_get_ModBas (mdr->modNr);
  if(mdb == NULL) return -1;
    // DEB_dump_obj__ (Typ_SubModel, mdb, " DrawModel-mdb-1");
    // printf(" DrawModel-typ=%d mnam=|%s|\n",mdb->typ,mdb->mnam);


// // removed - xy.tess will be loaded later 
//   if(mdb->DLsiz < 1) {
//     // TX_Error("GR_DrawModel E001 M%ld\n",db_ind);
//     TX_Print("**** M%ld - Model %s empty .. \n",db_ind, mdb->mnam);
//     return -1;
//   }



  //==== Mockup ====================================================
  // if(mdb->typ != 3) goto L_model;
  if(mdb->typ == Mtyp_WRL2) goto L_mock_0;
  if((mdb->typ < Mtyp_WRL)||(mdb->typ >= Mtyp_BMP)) goto L_model;  // 10-19=Mockup

  // mdr: mnam, modNr, po ...
  // mdb: mnam, typ, po, DLind, DLsiz ...

  // get full filename
  L_mock_0:
  Mod_get_path (ffnam, mdb->mnam);
    // printf("ex-Mod_get_path mnam=|%s| ffnam=|%s| dli=%ld\n",
           // mdb->mnam,ffnam,mdb->DLind);

  // is MockupModel already loaded ?
    // printf(" DLind=%d DLsiz=%d\n",mdb->DLind,mdb->DLsiz);
  if(mdb->DLind >= 0) goto L_mock_ditto;


  // nein, get a new ind
  mdb->DLind = GL_Get_DLind (); // GL_GetActInd();
    printf(" DrawModel-Mock-mdb->DLind=%ld\n",mdb->DLind);


  // extract filetype.
  irc = UTX_ftyp_s (ftyp, ffnam, 1);
  if(irc < 0) {TX_Print("GR_DrawModel FileType not found"); return irc;}
    printf(" ftyp=|%s|\n",ftyp);


  // fix a safe modelname
  strcpy (mnam, mdb->mnam);
  // mnam ist internal ("abc") od external ("dir/fn")
  UTX_ftyp_cut (mnam);            // remove fTyp
  UTX_safeName (mnam, 1);         // change all '. ' to '_', not '/'
    printf(" DrawModel-mnam=|%s|\n",mnam);


//   // check if tesselated data of the Model exists in xa/temp
//   irc = tess_read_ (&impSpc, mnam);
//   if(irc == 0) {loadMode = 1; goto L_mock_draw;}
//   loadMode = 0;


  // import modelfile.
  if(!strcmp(ftyp, "TESS")) {
      printf(" ... do TESS ..\n");

    // import Mockup from file into Mockup-struct
    TSU_imp_tess (&impSpc, ffnam);
    // copy tess-file -> tmp/.
    sprintf (memspc011, "%s%s.tess",AP_get_tmp_dir(),mnam);
      // printf(" DrawModel-ffnam=|%s|\n",memspc011);
    OS_file_copy (ffnam, memspc011);

    // find box of tess-model in memory
    tess_box_get (&pb1, &pb2, &impSpc);
       // DEB_dump_obj__ (Typ_PT, &pb1, "pb1");
       // DEB_dump_obj__ (Typ_PT, &pb2, "pb2");

    // set box in basicModel mdb
    mdb->pb1 = pb1;
    mdb->pb2 = pb2;

  } else {
    // get model as tesselated-data into impSpc, then write into file xa/temp
      printf(" ... do import ..\n");

    // Die Parametertabelle oTab fuer die DLL generieren.
    UTO_rec_set (&oTab[0], Typ_ObjGX,  Typ_ObjGX,  3, &oTab[1]);
    UTO_rec_set (&oTab[1], Typ_Int4,   Typ_Int4,   1, PTR_INT(1));  // mode
    UTO_rec_set (&oTab[2], Typ_Txt,    Typ_Txt,    1, ffnam);
    UTO_rec_set (&oTab[3], Typ_Memspc, Typ_Memspc, 1, &impSpc);

    // Load DLL; filename = xa_<fileType>_r.so
    irc = DLL_run1 (0, ffnam);
    if(irc < 0) return irc;

    // load Model --> impSpc
    irc = DLL_run1 (2, oTab);
    if(irc < 0) return irc;

    // find box of tess-model in memory
    tess_box_get (&pb1, &pb2, &impSpc);
       // DEB_dump_obj__ (Typ_PT, &pb1, "pb1");
       // DEB_dump_obj__ (Typ_PT, &pb2, "pb2");


    // set box in basicModel mdb
    mdb->pb1 = pb1;
    mdb->pb2 = pb2;

    // // get origin from box of tess-model
    // tess_origin_box (&pOri, &pb1, &pb2);

    // // subract origin of tess-model in memory
    // tess_origin_set__ (&impSpc, &pOri);

    // save tesselated data in xa/temp for next run
    tess_write__ (mnam, impSpc.start);

    // cannot unload unless impSpc freed; unload DLL   2009-06-06
    // irc = DLL_run1 (3, NULL);
    // if(irc < 0) return irc;

  }
  // printf(" irc nach DLL_run1 %d\n",irc);

  // draw
  L_mock_draw:
    // DEB_dump_obj__ (Typ_SubModel, mdb, " DrawModel-mdb-2");

  dli = DL_StoreObj (Typ_Ditto, 0L, 0);
  DL_unvis_set (dli, 1);         // make Basemodel unvisible
    // removed 2012-01-18 - crashes with import .tss - files

  // display
  TSU_DrawSurTess (&dli, att, impSpc.start);

    // printf(" loadMode=%d\n",loadMode);
  if((loadMode == 0) || (!strcmp(ftyp, "TESS"))) {     // 2011-08-18
    UME_free (&impSpc);

//   } else {
// 
//     // create record "Int=3"
//     UTO_rec_set (&oTab[1], Typ_Int4,   Typ_Int4,   1, (void*)3);  // mode
//     // free mem & unload Dll
//     irc = DLL_run1 (2, oTab);
//     if(irc < 0) return;
// 
//     // unload DLL
//     irc = DLL_run1 (3, NULL);
//     if(irc < 0) return;

  }


  // mdb->DLsiz = GL_Get_DLind() - mdb->DLind;
  // mod1->DLsiz = GL_GetActInd() - mod1->DLind;
  mdb->DLsiz = 1;

    // printf("  GR_DrawModel DLind=%d DLsiz=%d\n",mdb->DLind,mdb->DLsiz);



  //----------------------------------------------------------------
  // bereits geladenes MockupModel als Ditto (Duplikat) darstellen.
  L_mock_ditto:

  // draw ditto .. see GL_DrawModel GL_DrawDitto2
  dli = DL_StoreObj (Typ_Mock, db_ind, att);
  // DL_unvis_set (dli, 1);  // 2014-09-13

  // check if non-standard-position
  i1 = GRU_angr3_vxvz (&az1, &ay, &az2, &mdr->vx, &mdr->vz);

  if(i1 < 0)
    GL_DrawDitto (&dli, &mdr->po, &mdb->po, mdb->DLsiz, mdb->DLind);
  else
    GL_DrawDitto1 (&dli,&mdr->po,&mdb->po,az1,ay,az2,mdr->scl,
                   mdb->DLsiz,mdb->DLind);

  goto L_dir_npt;


  //================================================================
  // SubModel;
  L_model:
  dli = DL_StoreObj (Typ_Model, db_ind, att);
    // printf(" _StoreObj M%ld dli=%ld\n",db_ind,dli);
  GL_DrawModel (&dli, att, mdr, mdb);
  // DL_unvis_set (dli, 1);  // 2014-09-13


  //----------------------------------------------------------------
  L_dir_npt:
  // disp objId, direction (if enabled)
  GR_Draw_oid_dir_npt (&dli, &mdr->po, 1);

    // printf("ex-GR_DrawModel\n");

  return 0;

}
*/


//====================================================================
  void GR_DrawTorSeg (long *ind, int attInd, Circ *ci1, Circ *ci2) {
//====================================================================
/// \code
/// ci1 ist die Hauptdrehachse, ci2 das Profil.
/// Am Anfangspunkt von ci1 sitzt ci2.
/// Nun am Endpunkt von ci1 auch einen ebensolchen Kreis ci3 generieren.
/// Nun die ersten 2 Punkte auf ci2 und ci3 durch ein Kreispolygon parallel
///   zu ci1 verbinden und als RSur ausgeben.
/// Dann die naechsten 2 Punkte usw.
/// \endcode


  int    i1, p1Anz, p2Anz, iAnz, ii1;
  Point  *pa0, *pa1, *pa2;
  Circ   ct1, ct2;
  /* Vector vc1, vc2; */



  printf("GR_DrawTorSeg\n");

  // max 3 x 1024 Punkte ...
  iAnz = 1024;

  i1 = 3 * iAnz;
  if(GLT_pta_SIZ < i1) {
    i1 = GLT_alloc_pta (i1);
    if(i1 == -1) return;
  }

  pa0 = &GLT_pta[0];
  pa1 = &GLT_pta[1024];
  pa2 = &GLT_pta[2048];



  /* den ci1 in ein Polygon teilen */
  UT3D_cv_ci (pa0, &p1Anz, ci1, iAnz, ci1->rad/100.0);



  ii1 = 0;

  GL_Draw_Ini (ind, attInd);


  /* ============================= */
  NextSeg:

  /* ------------------------ */
  ct1.pc  = pa0[ii1];



  /* nur Test: */
  /* GR_CreCirc (ind, attInd, &ct1); */



  /* ------------------- */
  ct2.pc  = pa0[ii1+1];



  /* nur Test: */
  /* GR_CreCirc (ind, attInd, &ct2); */



  /* ------------------- */
  /* ZylSur zwischen ct1 und ct2 */
  UT3D_cv_ci (pa1, &p2Anz, &ct1, iAnz, -0.1);
  UT3D_cv_ci (pa2, &p2Anz, &ct2, iAnz, -0.1);


  /* GL_DrawRSur (ind, attInd, p2Anz); */
  // GL_set_strip_v (p2Anz, GL_ptArr30, GL_ptArr31);
  GL_set_strip_v (pa1, pa2, p2Anz, 0);


  ++ii1;
  if(ii1 < (p1Anz-1)) {
    /* ++objInd; */
    /* *ind = DL_StoreObj (Typ_TOR, objInd, attInd); */
    goto NextSeg;
  }


  GL_EndList ();


}


//==============================================================================
  int GR_set_ccv (int opers, CurvCCV *cva, int cvNr, long dbi, int att) {
//==============================================================================
// display DB-obj - composite-curve
// Input:
//   cvNr    nr of trimmed-curves to display
//   cva     trimmed-curve(s)
//   dbi     dbi of  composite-curve
//   att     see INF_COL_CV


  int       irc, grMode, ptNr;
  double    tol;
  Point     *pta;
  MemTab(Point) mtpa = _MEMTAB_NUL;



  // printf("DDDDDDDDDDDDDDD  GR_set_ccv op=%d dbi=%ld att=%d\n",opers,dbi,att);
  // printf("GR_set_ccv dbi=%ld cvNr=%d\n",dbi,cvNr);
  // for(irc=0;irc<cvNr;++irc) DEB_dump_obj__ (Typ_CVTRM, &cva[irc], "%d",irc); 
  // printf("  APT_obj_stat=%d\n",APT_obj_stat);
    // dbi = 0;  // TEST - display grMode-1 = without PRCV

  // perm
  if(opers & OPERS_PERM) {
    DL_perm_init (Typ_CVTRM, dbi, att);

  // tdyn
  } else if(opers & OPERS_TDYN) {
    DL_tDyn_init (att);

  // temp
  } else if(opers & OPERS_TEMP) {
    DL_temp_init ();
  }


  //----------------------------------------------------------------
  GR_typ = Typ_CV;
  GR_dbi = dbi;


  // set grMode; 0=polygon-frome-PRCV; 1=polygon-from-analytic-curve
  if((dbi > 0)&&(APT_obj_stat == 0)) grMode = 0;
  else                               grMode = 1;


  // init mtpa and get space for ~ 1200 points
  irc = MemTab_ini_fixed (&mtpa, MEM_alloc_tmp(SPC_MAX_STK), SPC_MAX_STK,
                          sizeof(Point), Typ_PT);
  if(irc < 0) return MSG_ERR__ (ERR_MEMSPC_IN_USE, "-");
    // MemTab_dump (&mtpa, "DrawCvCCV-1");


  tol  = UT_DISP_cv;
  // get polygon from trimmed-curve(s)
  irc = UT3D_mtpt_trmCv (&mtpa, NULL, cva, cvNr, tol, grMode);
  if(irc < 0) {TX_Error("GR_set_ccv E2"); goto L_exit; }

  ptNr = mtpa.rNr;
  pta  = mtpa.data;

    // TESTBLOCK
    // printf(" DrawCvCCV-ptNr=%d\n",ptNr);
    // DEB_dump_nobj__ (Typ_PT, mtpa.rNr, mtpa.data, "_DrawCvCCV-1");
    // GR_tDyn_pcv (mtpa.data, mtpa.rNr, Typ_Att_hili); // display temp.
    // END TESTBLOCK

  // point only: Hilite.
  if(ptNr < 2) {
    GL_att_pt (ATT_PT_HILI);   // set color
    GL_set_npt (pta, 1);
    goto L_exit;
  }

  // display
  // GR_DrawPoly (dli, att, ptNr, pta);
  if(opers > OPERS_CLOSE) GL_att_cv (att);
  GL_set_pcv (ptNr, pta, 1); // GL_LINE_STRIP

  if(APT_obj_stat) GL_set_arrh3D (&pta[ptNr - 1], &pta[ptNr - 2]);


  //----------------------------------------------------------------
  L_exit:

  if(opers & OPERS_CLOSE) GL_list_close (); // close GL-record

  // free
  MemTab_free (&mtpa);


    // TESTBLOCK
    // printf("ex-GR_tDyn_ccv\n");
    // END TESTBLOCK

  return irc;

}


/* old version 2014-06-19
//=====================================================================
  int GR_set_ccv (long *ind, int att, ObjGX *cv1, Memspc *tbuf1) {
//=====================================================================

  int    irc, ptNr;
  Point  *pta;


  printf("GR_set_ccv   %ld      XXXXXXXXXXXXXXXXXXXXXX\n",*ind);


  // pta  = (Point*)memspc101;
  // ptNr = sizeof(memspc101) / sizeof(Point);

  UME_connect (pta, tbuf1);
  ptNr = UME_ck_free (tbuf1) / sizeof(Point);


  // CCV -> Polygon
  irc = UT3D_pta_ox_lim (&ptNr, pta, cv1, 0, NULL, UT_DISP_cv);
  // printf("nach UT3D_pta_ox_lim %d %d\n",irc,ptNr);
  if(irc < 0) {TX_Error("GR_set_ccv EOM"); return -1;}

  // nur Startpunkt: Hiliten.
  if(ptNr < 2) {
    GL_DrawPoint (ind, 0, pta);
    return 0;
  }

  GR_tDyn_pcv (ind, Typ_Att_hili, ptNr, pta);

  // disp direction
  // for CCV in statu nascendi ("S0=CCV .."): display also direction
  if((APT_dispDir)||(*ind < 0)) {
    int   ipe;
    ipe = ptNr - 1;
    APT_disp_dir (&pta[ipe], &pta[ipe - 1]);
  }


  return 0;

}
*/

/* UNUSED
//=====================================================================
  int GR_tDyn_ccv2 (long *ind, int att, ObjGX *cv1) {
//=====================================================================
/// NOT UP TO DATE: may have only LN2 & AC2

  int    irc, i1, ptNr;
  Point  *pta;
  Line2     *ln21;
  ObjG2     o21;
  ObjGX     *oTab, *ox2;


  // printf("GR_tDyn_ccv2\n");

    oTab = cv1->data;

    for(i1=0; i1<cv1->siz; ++i1) {
      ox2 = &oTab[i1];
      // printf("CCV-%d form=%d\n",i1,ox2->form);

      if(ox2->form == Typ_LN2) {
        ln21 = ox2->data;
        APT_disp_ln (&ln21->p1, &ln21->p2, att, 0., 0.);

      } else if(ox2->form == Typ_CI2) {
        o21 = UT2D_obj_ci2 (ox2->data);
        APT_disp_ac (&o21, att, 0.);

      } else {
        TX_Error("**** GR_tDyn_ccv2 form %d",ox2->form);
      }
    }


  return 0;

}
*/
/*
//================================================================
  int GR_Draw_subCurv (long *ind, int att, int typ, void *obj,
                       int segNr1, int segNr2) {
//================================================================
/// \code
/// GR_Draw_subCurv        draw segment of polygon or CCV.
/// Input:
///   typ,obj       must be Typ_CVPOL or Typ_CVTRM
///   obj           data-struct of typ <typ>
///   segNr1,segNr2 segmentNumbers: 1 = first segment.
/// \endcode


  int        irc, sTyp;
  char       sObj[OBJ_SIZ_MAX];



  // get (line-) segment out of polygon
  if(typ == Typ_CVPOL) {
    if((segNr1 < 1)||(segNr1 >= ((CurvPoly*)obj)->ptNr)) return -1;
    GR_DrawPoly (ind, att, 2, &((CurvPoly*)obj)->cpTab[segNr1 - 1]);


  } else if(typ == Typ_CVTRM) {

    // get standard-curve from CCV-subSeg
    irc = UTO_cv_cvtrm (&sTyp, sObj, NULL, &((CurvCCV*)obj)[segNr1 - 1]);
    if(irc < 0) return -1;
      DEB_dump_obj__ (sTyp, sObj, " CCVsubSeg %d",segNr1);

    if(segNr2 < 1) {
      // display curve
      GR_temp_nobj (ind, sTyp, sObj, 1, att, 0);
    } else {
      // recurse for line in polygon in ccv
      GR_Draw_subCurv (ind, att, sTyp, sObj, segNr2, 0);
    }

  } else return -2;


  return 0;

}


//================================================================
  int GR_Draw_pol2 (long *ind, int attInd, CurvPol2 *plg1) {
//================================================================
// GR_Draw_pol2             display curve CurvPol2

   int    irc, ptNr;
  Point2  *pTab;
  // MemTab(Point2) mtPt = _MEMTAB_NUL;


  // printf("GR_DrawCvPol ptNr=%d\n",plg1->ptNr);
  // DEB_dump_obj__ (Typ_CVPOL2, plg1, "plg1");


  ptNr = plg1->ptNr;   // + 4;

  pTab = plg1->pTab;

  GR_tDyn_pcv2D (ind, attInd, ptNr, pTab, 0.);

  // disp direction (last-point, point before)
  if(APT_dispDir) {
    APT_disp_dir (&pTab[ptNr - 1], &pTab[ptNr - 2]);
  }

  return 0;

}


//=====================================================================
  int  GR_DrawCvPol (long *dli, long dbi, int attInd, CurvPoly *plg1) {
//=====================================================================
// GR_DrawCvPol          display curve struct CurvPoly

  int    irc, ptNr;
  Point  *pTab;
  MemTab(Point) mtPt = _MEMTAB_NUL;


  // printf("GR_DrawCvPol ptNr=%d\n",plg1->ptNr);

  GR_typ = Typ_CV;
  GR_dbi = dbi;


  ptNr = plg1->ptNr + 4;

  // get memspc until end-of-func (alloca|malloc)
  MemTab_ini_temp (&mtPt, Typ_PT, ptNr);
  pTab = mtPt.data;


  // PolygonCurve -> relimited Polygon
  irc = UT3D_pta_plg (&ptNr, pTab, plg1);     // 
  if(irc) return -1;


  // erhaelt das Polygon als fertiges Array (mit Screenkoords)
  GR_DrawPoly (dli, attInd, ptNr, pTab);


  // free memspace
  MemTab_free (&mtPt);

  return 0;

}
*/



//=====================================================================
  void GR_tDyn_psp3 (long *ind, long dbi, int attInd, ObjGX *cv1, double zval) {
//=====================================================================
// Typ_CVPSP3
// TODO: add into UT3D_mtpt_obj


  int    irc, ptNr, ptMax;
  Point  *pTab;


  // printf("GR_tDyn_psp3  %d %d\n",*ind,cv1->siz);
  // DEB_dump_ox_0 (cv1, "GR_tDyn_psp3");
  
  GR_typ = Typ_CV;
  GR_dbi = dbi;


  pTab = (void*) memspc55;
  ptMax = sizeof(memspc55) / sizeof(Point);

  // Spline -> Polygon   Toleranz
  irc = CVPSP_pol_oPsp3 (&ptNr, pTab, cv1, ptMax, UT_DISP_cv);
  if(irc < 0) {
    TX_Error("GR_tDyn_psp3 E001");
    return;
  }


  // erhaelt das Polygon als fertiges Array
  // GR_DrawPoly (ind, attInd, ptNr, pTab);
  GR_tDyn_pcv (pTab, ptNr, attInd);

  return;

}


/*
//=====================================================================
  void GR_DrawCurv (long *ind, int attInd, PointTab *cv1, double zval) {
//=====================================================================

  // printf("GR_DrawCurv %d\n",cv1->ptNr);

  //erhaelt das Polygon als fertiges Array (mit Screenkoords)
  GR_tDyn_pcv (ind, attInd, cv1->ptNr, cv1->ptTab);

}
*/


/*
//=====================================================================
  void GR_DrawCurv (long *ind, int attInd, Curv *cv1, double zval) {
//=====================================================================

// Anf/EndPkte:    cv1->p1 / cv1->p2
// Die PolygonZwischenPunkte sind als dynPt's gespeichert worden.
// Die (dynam.) Zwischenpunkte: holen mit DB_GetPoint (cv1->ID1 - cv1->ID2)
// cv1->ind (Indexe auf den Start/Endpunkt im crv###.bin) zeigt nur
// ob Kurve verkehrt liegt oder nicht; ind1<ind2 == verkehrt !



  long     ptInd, ptNr, incr, ptAnz;
  Point    pta, pte;
  Point    pt1;

  //TX_Print("GR_DrawCurv %f,%f\n", cv1->p1.x, cv1->p1.y);
  //TX_Print("          - %f,%f\n", cv1->p2.x, cv1->p2.y);
  //TX_Print("GR_DrawCurv: ind=%d/%d OFF=%d/%d",cv1->ID1,
  //   cv1->ID2,cv1->ind1, cv1->ind2);



  // nur Test:
  // return;

  // GR_Att (attInd);


  // der erste Punkt ist cv1->p1
  if(cv1->ind1 < cv1->ind2) {
    incr  = 1;
    ptAnz = cv1->ind2 - cv1->ind1 + 1;
  } else {
    incr  = -1;
    ptAnz = cv1->ind1 - cv1->ind2 + 1;
  }


  pta   = cv1->p1;
  pte   = cv1->p2;


  GL_ptArr30[0] = pta;
  //TX_Print(" Vert 0=%f,%f,%f",GR_ptArr1[0].x,GR_ptArr1[0].y,GR_ptArr1[0].z);


  ptNr = 1;
  ptInd = cv1->ID1 + cv1->ind1;


  if(cv1->ID1 == -2) goto L_CvEnd;  // check for Line


  NextVert:
    if(ptNr > ptAnz) goto L_CvEnd;

    GL_ptArr30[ptNr] = DB_GetPoint (ptInd);
    //TX_Print(" Vert %d=%f,%f,%f",ptNr,
    //      GR_ptArr1[ptNr].x,GR_ptArr1[ptNr].y,GR_ptArr1[ptNr].z);

    ++ptNr;
    if(ptNr >= 999) {
      TX_Error("E001 GR_DrawCurv");
      return;
    }


    ptInd = ptInd + incr;
    goto NextVert;




  // der letzte Punkt ist cv1->p2
  L_CvEnd:
  GL_ptArr30[ptNr] = pte;
  //TX_Print(" last Vert %d=%f,%f,%f",ptNr,
  //     GR_ptArr1[ptNr].x,GR_ptArr1[ptNr].y,GR_ptArr1[ptNr].z);
  ++ptNr;


  //erhaelt das Polygon als fertiges Array (mit Screenkoords)
  GR_tDyn_pcv (ind, attInd, ptNr, GL_ptArr30);


}
*/





/* ============================================================ */
/* ============================================================ */
/* ============================================================ */
/* ============================================================ */
/* ============================================================ */
/* Ab hier nicht benutzte CATIA-Interfaces */


void CAD_Init (int mode)                               {return;  }
long CAD_sol_add (long ID1, long ID2)                  {return 0;}
long CAD_sol_update (long ID1)                         {return 0;}

int GR_chgSet (char* setName)                          {return 0;}
int GR_getGroup (long *stkID, char buf[])              {return 0;}
int GR_getGroupID (long *elID, long *stkID, long *ind) {return 0;}
int GR_Delete (long ind)                               {return 0;}



//===================================================================
//===================================================================
//===================================================================
//===================================================================
//===================================================================




//===================================================================
  int GR_Disp_message (int mode) {
//===================================================================
/// \code
/// Display messages with GR_Disp_xxx or not.
/// 0=OFF=Aus, 1=ON=Ein.
/// \endcode

  DispMode = mode;

  return 0;

}


/* repl by GR_tDyn_pt GR_tDyn_symB__
//===================================================================
  int GR_tDyn_symB__ (Point *pt1, int typ, int att) {
//===================================================================
/// \code
/// Testdisplay 3D-Point
/// Input:
///   typ:  SYM_STAR_S  star
///         SYM_TRI_S   triangle
///         SYM_SQU_B   square
///   att:  colorIndex; eg ATT_COL_RED; see INF_COL_SYMB
/// \endcode

/// see also GR_Disp_pTab

  // long    l1;

  // if(DispMode) DEB_dump_obj__(Typ_PT, pt1, "GR_Disp_pt:");

  APT_disp_SymB (typ, att, pt1);

  // l1 = DL_StoreObj (Typ_SymB, 0L, 0);
  // GL_DrawSymB (&l1, att, typ, pt1); // att=1=sw, 2=rot, 4=bl,

  return 0;

}


//===================================================================
  int GR_Disp_pt2 (Point2 *pt1, int typ, int att) {
//===================================================================
/// \code
/// Testdisplay 2D-Point als Symbol
///   typ:  SYM_STAR_S  star
///         SYM_TRI_S   triangle
///         SYM_SQU_B   square
///   att:  colorIndex; eg ATT_COL_RED; see INF_COL_SYMB
/// \endcode

  Point p31;

  if(DispMode) DEB_dump_obj__(Typ_PT2, pt1, "GR_Disp_pt2:");

  p31=UT3D_pt_pt2(pt1);

  APT_disp_SymB (typ, att, &p31);

  return 0;

}
*/

//=======================================================================
  int GR_tDyn_box2_4db (double x1, double x2, double y1, double y2, int att) {
//=======================================================================
/// \code
/// GR_tDyn_box2_4db           make 2D-Box from 4 doubles
/// att: GR_tDyn_ln2_2pt
/// \endcode

  Point2     p1, p2;

  // printf("GR_tDyn_box2_4db \n");

  p1.x = x1;
  p1.y = y1;

  p2.x = x2;
  p2.y = y2;

  GR_tDyn_box2__ (&p1, &p2, att);

  return 0;

}
 

//========================================================================
  int GR_tDyn_ip2cv (int iNr, int *ia, Point2 *pa2, int  iClo, int att) {
//========================================================================
// GR_tDyn_ip2cv             display indexed 2D-point-array as curve
// Input:
//   att      see INF_COL_CV
//   iNr      nr of indices to display
//   ia       iNr indices into pa
//   pa       table of points
//   iClo     closed; 0=yes, 1=not_closed;

// see also GR_Draw_i2fac MSH2D_disp_cv_inpt

  int     i1;
  Point  *pta;


  printf("GR_tDyn_ip2cv iNr=%d\n",iNr);
  // for(i1=0; i1<iNr; ++i1) printf(" Draw_i2ptb_ ind[%d] = %d\n",i1,ia[i1]);


  pta = (Point*) MEM_alloc_tmp ((iNr + 1) * (int)sizeof(Point));

  for(i1=0; i1<iNr; ++i1) {
    memcpy(&pta[i1], &pa2[ia[i1]], sizeof(Point2));
    pta[i1].z = GL2D_Z;
    // UT3D_pt_pt2_z 
  }

  // must add startPt if closed
  if(!iClo) {
    UT3D_pt_pt2_z (&pta[iNr], &pa2[ia[0]], GL2D_Z);
    ++iNr;
  }

  // disp polygon
  GR_tDyn_pcv (pta, iNr, att);

  return 0;

}


//================================================================
  int GR_tDyn_npcv_itb_npt (int itNr, IndTab *ita, Point *pta,
                            int attCv, int attPt) {
//================================================================
// GR_tDyn_npcv_itb_npt      disp group of polygons from IndTab to points
//  indexed group of points see MSHI_disp_npcv_bnd3D

  int    i1, iNr, iBeg;

  DL_tDyn_init (attCv);

  for(i1=0; i1 < itNr; ++i1) {
    iNr  = ita[i1].iNr;
    iBeg = ita[i1].ibeg;

    // disp curv
    GL_att_cv (attCv);
    GL_set_pcv (iNr, &pta[iBeg], 1);

    // disp startPt
    GL_att_pt (attPt);
    GL_set_symB (SYM_TRI_S, &pta[iBeg]);

    // disp endPt
    GL_set_symB (SYM_TRI_S, &pta[iBeg + iNr - 1]);
  }

  GL_list_close ();

  return 0;

}


//======================================================================
  int GR_tDyn_npcv_itb_nipt (int itNr, IndTab *ita, int *ia, Point *pta,
                             int attCv, int attPt) {
//======================================================================
// GR_tDyn_npcv_itb_nipt     disp group of polygons from IndTab to indexed points
//  indexed group of points see MSHI_disp_npcv_bnd3D

  int    i1, iNr, iBeg, ips, ipe;

  DL_tDyn_init (attCv);

  GL_att_cv (attCv);

  for(i1=0; i1 < itNr; ++i1) {
    iNr  = ita[i1].iNr;
    iBeg = ita[i1].ibeg;
    // disp curv
    GL_set_ipcv (iNr, &pta[iBeg], ia, 1);
  }

  if(attPt >= 0) {
    GL_att_pt (attPt);
    for(i1=0; i1 < itNr; ++i1) {
      iNr  = ita[i1].iNr;
      iBeg = ita[i1].ibeg;
      // disp startPt, endPt
      ips = ia[iBeg];
      GL_set_symB (SYM_TRI_S, &pta[ips]);
      // disp endPt
      ipe = ia[iBeg + iNr - 1];
      GL_set_symB (SYM_TRI_S, &pta[ipe]);
    }
  }

  GL_list_close ();

  return 0;

}


//========================================================================
  int GR_tDyn_ipcv (int iNr, int *ia, Point *pa, int  iClo, int att) {
//========================================================================
// GR_tDyn_ipcv             display indexed point-array as curve
// Input:
//   att      see INF_COL_CV
//   iNr      nr of indices to display
//   ia       iNr indices into pa
//   pa       table of points
//   iClo     closed; 0=yes, 1=not_closed;

// see also GR_Draw_i2fac MSH2D_disp_cv_inpt

  int     i1;
  Point  *pta;


  printf("GR_tDyn_ipcv iNr=%d\n",iNr);
  // for(i1=0; i1<iNr; ++i1) printf(" Draw_i2ptb_ ind[%d] = %d\n",i1,ia[i1]);


  pta = (Point*) MEM_alloc_tmp ((iNr + 1) * (int)sizeof(Point));

  for(i1=0; i1<iNr; ++i1) memcpy (&pta[i1], &pa[ia[i1]], sizeof(Point));

  // must add startPt if closed
  if(!iClo) {
    memcpy (&pta[iNr], &pa[ia[0]], sizeof(Point));
    ++iNr;
  }

  // disp polygon
  GR_tDyn_pcv (pta, iNr, att); 

  return 0;

}


//================================================================
  int GR_tDyn_nifac (MshFac *nifa, int att) {
//================================================================
// GR_tDyn_nifac         display MshFac
// Input:
//   att          0 is defaulColor, else ColRGB

  ColRGB *col;


  printf("GR_tDyn_nifac nf=%d\n",nifa->fNr);
  if(att) DEB_dump_obj__ (Typ_Color, COL_INT32(&att), " col:");


  DL_tDyn_init (att);

  GL_att_sur (att);

  // GL_set_nifac_V1 (nifa->fNr,nifa->fac,nifa->ipa,nifa->pa3,nifa->vc3,nifa->oTyp);
  GL_set_nifac_V2 (nifa);

  GL_list_close (); // close GL-record

  return 0;

}


//=========================================================================
  int GR_tDyn_npti (int ptNr, Point *pTab, int typ, int ptAtt, int txAtt) {
//=========================================================================
// GR_tDyn_npti           disp n points and point-numbers
//   typ     pointSymbolType;    see INF_BITMAP_SYMBOLS
//   ptAtt   color points      see INF_COL_PT
//   txAtt   color for point-numbers; see INF_COL_SYMB
//
// draw polygon with GR_tDyn_pcv

  int    i1;
  char   cbuf[40];


  DL_tDyn_init (0);

  GL_att_sym (ptAtt);   // set points-color

  for(i1=0; i1<ptNr; ++i1) GL_set_symB (typ, &pTab[i1]);

  GL_att_sym (txAtt);   // set color

  for(i1=0; i1<ptNr; ++i1) {
    snprintf(cbuf, sizeof(cbuf), "%d", i1);
    GL_set_txtA (&pTab[i1], cbuf);
  }

  GL_list_close (); // close GL-record

  return 0;

}


//===================================================================
  int GR_tDyn_nsymB (int ptNr, Point *pTab, int typ, int att) {
//===================================================================
/// \code
/// Testdisplay ptNr points with bitmap-symbols (star, triangle, ..)
/// typ: see GR_temp_nobj/GR_temp_nobj/Typ_SymB
///      SYM_TRI_S|SYM_STAR_S|SYM_CIR_S|SYM_SQU_B
/// att: see INF_COL_SYMB       GR_temp_nobj/Typ_SymB
///      ATT_COL_BLACK|ATT_COL_RED|ATT_COL_GREEN|ATT_COL_BLUE|..
///
/// see also:
///   GR_tDyn_pcv            draw polygon
///   GR_tDyn_npti          display points and integers along table of points
/// \endcode

// #include "../ut/func_types.h"               // SYM_..

  int   i1;

  printf("GR_tDyn_npt__ ptNr=%d typ=%d att=%d\n", ptNr,typ,att);

  for(i1=0; i1<ptNr; ++i1) {
    if(DispMode) DEB_dump_obj__(Typ_PT, &pTab[i1], "  pt[%d]=",i1);
    // APT_disp_SymB (typ, att, &pTab[i1]);
    GR_tDyn_symB__ (&pTab[i1], typ, att);

    // NUR TEST:
    // { char cbuf[32];
      // sprintf(cbuf,"%d",i1);
      // GR_tDyn_txtA (att, &pTab[i1], cbuf);
    // }
  }

  return 0;

}

/* UU
//===================================================================
  int GR_Disp_vc2 (Vector2 *vc1, Point2 *pt1, int att, int mode) {
//===================================================================
/// \code
/// GR_Disp_vc2     display 2D-Vector; length true or normalized
///   att       color,linetyp,thickness; see INF_COL_CV
///   mode      0  display normalized length of vector
///             1  display real length of vector
/// \endcode


  long   dli;


  if(DispMode) printf("GR_Disp_vc2 %f,%f P=%f,%f\n",vc1->dx,vc1->dy, pt1->x,pt1->y);

  dli = DL_StoreObj (Typ_VC2, -1L, 2);

  return GR_Draw_vc2 (&dli, vc1, pt1, att, mode);

}


// replaced by GR_Disp_ci
//================================================================
  int GR_Disp_ac (Circ *ci1, int att) {
//================================================================
/// GR_Disp_ac                display 3D-circle
/// att: linetype; see INF_COL_CV


  long dli;
  int  i1, ianz = 1024, ptAnz;


  // if(DispMode) DEB_dump_obj__ (Typ_CI, ci1, "GR_Disp_ac:\n");

  // GR_tDyn_symB__ (&ci1->p1, SYM_STAR_S, 0);
  // GR_tDyn_symB__ (&ci1->p2, SYM_STAR_S, 0);
  // GR_tDyn_symB__ (&ci1->pc, SYM_STAR_S, 0);
  // GR_tDyn_vc__ (&ci1->vz, &ci1->pc, 2, 0);



  if(GLT_pta_SIZ < ianz) {
    i1 = GLT_alloc_pta (ianz);
    if(i1 == -1) return i1;
  }


  // circ -> Polygon (see GR_DrawCirc ..)
  UT3D_cv_ci (GLT_pta, &ptAnz, ci1, ianz, UT_DISP_cv);
  // GR_tDyn_npt__ (ptAnz, GL_ptArr30, SYM_STAR_S, 2);


  dli=DL_StoreObj (Typ_CI, -1L, 2);
  GR_tDyn_pcv (&dli, att, ptAnz, GLT_pta);


  return 0;

}
*/

/*
//================================================================
  int GR_Disp_ccv (CurvCCV *ccv, int att, Memspc *tbuf1) {
//================================================================
/// \code
/// Testdisplay ConcatenatedCurVe
/// att                 see ~/gCAD3D/cfg/ltyp.rc
/// \endcode

// see GR_set_ccv GR_Disp_pol

  int    irc, ptNr;
  Point  *pta;
  long   dli;
    
    
  // if(DispMode) printf("GR_Disp_ccv %d\n",ccv->siz);
  printf("GR_Disp_ccv\n");

  // pta  = (Point*)memspc101;
  // ptNr = sizeof(memspc101) / sizeof(Point);

  pta = UME_get_next (tbuf1); 
  ptNr = UME_ck_free (tbuf1) / sizeof(Point);
     printf(" _Disp_ccv - %d free points\n",ptNr);


    
  // CCV -> Polygon
  irc = UT3D_pta_ox_lim (&ptNr, pta, ccv, 0, NULL, UT_DISP_cv, APT_obj_stat);
  // printf("nach UT3D_pta_ox_lim %d %d\n",irc,ptNr);
  if(irc < 0) {TX_Error("GR_Disp_ccv EOM"); return -1;}

  dli = DL_StoreObj (Typ_CVTRM, -1L, 2);

  // nur Startpunkt Hiliten.
  if(ptNr < 2) {
    GL_DrawPoint(&dli, 0, pta);
    return 0;
  }
  
  GR_tDyn_pcv (&dli, att, ptNr, pta);

  return 0;
 
}
*/

//===================================================================
  int GR_Disp_triv (Triang *tr, int att, int tNr, int ipNr) {
//===================================================================
/// \code
/// display Triang-Normalvektor & number of triangle
/// att   colorIndex; eg ATT_COL_RED; see INF_COL_SYMB
///       color,linetyp,thickness; see INF_COL_CV
/// tNr   Display Integer in gravity-centerPoint if (tNr >= 0; -1 = none)
/// ipNr  1 = display pointNumbers (0/1/2); -1=not
///
/// see also GR_Disp_tria GR_Disp_tris
/// \endcode


  long      dli;
  Point     p1;
  Vector    v1;

  // if(DispMode) printf("GR_Disp_triv\n");

  // get gravity-centerPoint of triangle
  UT3D_pt_gcp_tria (&p1, tr);
    // GR_tDyn_symB__ (&p1, SYM_STAR_S, 4);


  if(tNr >= 0) GR_tDyn_txiA (&p1, tNr, 0);


  // Normalvektor auf 3Eck
  UT3D_vc_perp3pt (&v1,tr->pa[0],tr->pa[1],tr->pa[2]);
  GR_tDyn_vc__ (&v1, &p1, att, 0);

  if(ipNr >= 0) {
    GR_tDyn_txiA (tr->pa[0], 0, 0);
    GR_tDyn_txiA (tr->pa[1], 1, 0);
    GR_tDyn_txiA (tr->pa[2], 2, 0);
  }


  return 0;

}


//================================================================
  int GR_tDyn_pcv3p2 (Point2 *p1, Point2 *p2, Point2 *p3, int att) {
//================================================================
/// \code
/// GR_tDyn_pcv3p2         Display boundary of triangle from 3 2D-points
/// att: GR_tDyn_ln2_2pt
/// 
/// see also GR_Disp_triv GR_Disp_tris
/// \endcode

  int    i1;
  Point2  pa[4];

  pa[0] = *p1;
  pa[1] = *p2;
  pa[2] = *p3;
  pa[3] = *p1;
    // GR_tDyn_symB__ (tria->pa[0], SYM_STAR_S, 2);
    // GR_tDyn_symB__ (&pa[0], SYM_STAR_S, 2);
    // GR_tDyn_symB__ (&pa[1], SYM_STAR_S, 2);

  // Flaeche immer defaultfarbe; somit nicht sichtbar !
  GL_Col__ (10);
  // GR_DrawFan(tria->pa[0], 3, pa, 0);

  GR_tDyn_pcv2 (pa, 4, att);

  return 0;

}


//================================================================
  int GR_Disp_tria (Triang *tria, int att) {
//================================================================
/// \code
/// GR_Disp_tria           display boundary of triangle as lines
/// att      color,linetyp,thickness; see INF_COL_CV
///
/// see also GR_Disp_triv GR_Disp_tris
/// \endcode

  int    i1;
  Point  pa[4];


  pa[0] = *(tria->pa[1]);
  pa[1] = *(tria->pa[2]);
  pa[2] = *(tria->pa[0]);
  pa[3] = *(tria->pa[1]);
    // GR_tDyn_symB__ (tria->pa[0], SYM_STAR_S, 2);
    // GR_tDyn_symB__ (&pa[0], SYM_STAR_S, 2);
    // GR_tDyn_symB__ (&pa[1], SYM_STAR_S, 2);

  // Flaeche immer defaultfarbe; somit nicht sichtbar !
  // GL_Col__ (10);
  // GR_DrawFan(tria->pa[0], 3, pa, 0);

  GR_tDyn_pcv (pa, 4, att);

  return 0;

}


//===================================================================
  int GR_Disp_su3p2 (Point2 *p21, Point2 *p22, Point2 *p23, int att) {
//===================================================================
/// \code
/// GR_Disp_su3p2         disp. surface of 3 2D-points
/// att: see GR_Disp_spu
/// \endcode

  Point    p1, p2, p3;

  p1 = UT3D_pt_pt2 (p21);
  p2 = UT3D_pt_pt2 (p22);
  p3 = UT3D_pt_pt2 (p23);

  GR_Disp_fan (&p1, &p2, &p3, 1, att);

  return 0;

}

//===================================================================
  int GR_Disp_su3pt (Point *p1, Point *p2, Point *p3, int att) {
//===================================================================
/// \code
/// GR_Disp_su3pt         disp. surface of 3 points
/// att: see GR_Disp_spu
/// \endcode


  GR_Disp_fan (p1, p2, p3, 1, att);

  return 0;

}


//================================================================
  int GR_Disp_tris (Triang *tria, int icol) {
//================================================================
/// \code
/// display Surface of triangle
/// icol     colorIndex; eg ATT_COL_RED; see INF_COL_SYMB
///
/// see also GR_Disp_triv GR_Disp_tria
/// \endcode

  GR_Disp_fan(tria->pa[0], tria->pa[1], tria->pa[2], 1, icol);

  return 0;

}


//=============================================================================
  int GR_temp_rect1 (Point *p1, Vector *vx, double dx, Vector *vy, double dy,
                     int att) {
//=============================================================================
// GR_temp_rect1        temp.disp rectangle from point, 2 vectors
// att:                 see ~/gCAD3D/cfg/ltyp.rc

  Point  pta[5];


  // DEB_dump_obj__(Typ_PT, p1, "GR_temp_rect1 p1");
  // DEB_dump_obj__(Typ_VC, vx, "GR_temp_rect1 vx");
  // DEB_dump_obj__(Typ_VC, vy, "GR_temp_rect1 vy");


  pta[0] = *p1;
  UT3D_pt_traptvclen (&pta[1], p1, vx, dx);
  UT3D_pt_traptvclen (&pta[2], &pta[1], vy, dy);
  UT3D_pt_traptvclen (&pta[3], p1, vy, dy);
  pta[4] = *p1;

  // GR_tDyn_pcv (dli, att, 5, pta);
  GR_temp_pcv__ (pta, 5, att);

  return 0;

}
 

//===================================================================
  int GR_tDyn_npt__ (Point *pta, int ptnr, int att) {
//===================================================================
// GR_tDyn_npt__           disp. points
// att: see INF_COL_PT

  int  i1;
  long dli;

/*
  if(DispMode) {
    printf("GR_tDyn_pcv2 %d\n",ptnr);
    for(i1=0; i1<ptnr; ++i1)
      DEB_dump_obj__(Typ_PT2, &pta[i1], " p[%d]",i1);
  }
*/

  DL_tDyn_init (att);

  GL_att_pt (att);   // set color

  GL_set_npt (pta, ptnr);

  GL_list_close (); // close GL-record

  return 0;

}


//===================================================================
  int GR_tDyn_npt_2 (Point2 *pta, int ptnr, int att) {
//===================================================================
// GR_tDyn_npt_2           disp. 2D-points
// att: see INF_COL_PT

  int  i1;
  long dli;

/*
  if(DispMode) {
    printf("GR_tDyn_pcv2 %d\n",ptnr);
    for(i1=0; i1<ptnr; ++i1)
      DEB_dump_obj__(Typ_PT2, &pta[i1], " p[%d]",i1);
  }
*/

  DL_tDyn_init (att);

  GL_att_pt (att);   // set color

  for(i1=0; i1<ptnr; ++i1) GL_set_pt2 (&pta[i1]); 

  GL_list_close (); // close GL-record
  
  return 0;

} 
  

//===================================================================
  int GR_tDyn_ln_2pt (Point *pt1, Point *pt2, int att) {
//===================================================================
// GR_tDyn_ln_2pt        disp line from 2 points
/// Testdisplay 2D-Polygon
/// att: see INF_COL_CV

  Point pta[2];

  pta[0] = *pt1;
  pta[1] = *pt2;

  return GR_tDyn_pcv (pta, 2, att);

}


//===================================================================
  int GR_tDyn_ln2_2pt (Point2 *pt1, Point2 *pt2, int att) {
//===================================================================
// GR_tDyn_ln2_2pt        disp line from 2 2D-points
/// att: see INF_COL_CV

  Point2 pta[2];

  pta[0] = *pt1;
  pta[1] = *pt2;

  return GR_tDyn_pcv2 (pta, 2, att);

}


//===================================================================
  int GR_tDyn_pcv2 (Point2 *pta, int ptnr, int att) {
//===================================================================
// GR_tDyn_pcv2          disp 2D-polygon
/// Testdisplay 2D-Polygon
/// att: see INF_COL_CV

  int  i1;
  long dli;

/*
  if(DispMode) {
    printf("GR_tDyn_pcv2 %d\n",ptnr);
    for(i1=0; i1<ptnr; ++i1)
      DEB_dump_obj__(Typ_PT2, &pta[i1], " p[%d]",i1);
  }
*/

  // dli=DL_StoreObj (Typ_CVPOL, -1L, 2);
  // GR_tDyn_pcv2D (&dli, att, ptnr, pta, 0.);

  DL_tDyn_init (att);

  GL_att_cv (att);  // set color/linetype

  GL_set_p2cv (ptnr, pta);

  GL_list_close (); // close GL-record

  return 0;

}


//================================================================
  int GR_Disp_patch (int gTyp, int icol, int pNr, Point *pa) {
//================================================================
// GR_Disp_patch       display Opengl-patch (type & n-points)
// Input:
//   gtyp     type of output; points, lines, surface. see below
//   icol     index of color; see GR_Disp_pt
//
// see also GR_Disp_ipatch
// replaces GR_Disp_fan

// gtyp:
// 0=GL_POINTS;
// 1=GL_LINES;
// 2=GL_LINE_LOOP (makes a closing point)
// 3=GL_LINE_STRIP (open polygon)
// 4=GL_TRIANGLES
// 5=GL_TRIANGLE_STRIP
// 6=GL_TRIANGLE_FAN
// 9=GL_POLYGON (surf; makes a closing point; must be concav)

// GL_TRIANGLE_STRIP
//          6
//        //|
//       4 /|
//     //| /|
//    2 /|/ |
//  / |/ |/ |
// 1--3--5--7

// GL_TRIANGLE_FAN
//
//    2--3--4
//     \ | / \
//      \|/   \
//       1-----5

// GL_POLYGON
//      3----4
//     /  \  |
//    2------5
//    |  \  / 
//    1----6


  long      dli;

  // get gcad-dispList-record
  dli = DL_StoreObj (Typ_SUR, -1L, 1);  // typ, dbi, att


  // init GL-dispList
  // iatt = Typ_Att_dash_long;
  GL_Surf_Ini (&dli, NULL); //(void*)&iatt);  // glNewList (for gTyp=4,5,6,9)


  if(icol >= 0)
    GL_Col__ (icol);  // glColor3fv (GL_col_tab[icol]); 2=red,3=green,5=yellow..


  GL_set_patch (gTyp, pNr, pa);

  // close GL-dispList
  GL_EndList ();                  // glEndList ();

  return 0;

}


/* UU
//=======================================================================
  int GR_Disp_ipatch (int gTyp, int icol, int iNr, int *ia, Point *pa) {
//=======================================================================
/// \code
/// temp. display a set of triangles from indexed points
/// 
/// Input:
///   gTyp     type of triangles in ia; eg GL_TRIANGLES; see GR_Disp_patch
///            bitVal-16 set: face is planar; else not.
///   iNr      nr of indices in ia
///   ia[iNr]  indices into pa
///   pa       table of points
///   icol     colorIndex; eg ATT_COL_RED; see INF_COL_SYMB
/// 
/// was GR_Disp_iface
/// \endcode

  long      dli;


  printf("GR_Disp_ipatch %d %d %d\n",gTyp,icol,iNr);


  // get gcad-dispList-record
  dli = DL_StoreObj (Typ_SUR, -1L, 1);  // typ, dbi, att


  // init GL-dispList
  GL_Surf_Ini (&dli, NULL); //(void*)&iatt);  // glNewList (for gTyp=4,5,6,9)

  if(icol >= 0)
    GL_Col__ (icol);  // glColor3fv (GL_col_tab[icol]); 2=red,3=green,5=yellow.

  GL_set_ipatch (gTyp, iNr, ia, pa);

  // close GL-dispList
  GL_EndList ();                  // glEndList ();

  return 0;

}


//================================================================================
  int GR_Draw_ipatch (long *dli, int gTyp, int icol, int iNr, int *ia, Point *pa){
//================================================================================
// display a set of triangles from indexed points

// Input:
//   pa       all points
//   ia[iNr]  indices into pa
//   gTyp     type of triangles in ia; eg GL_TRIANGLES; see GR_Disp_patch
//            bitVal-16 set: face is planar; else not.
//   icol     see GR_Disp_pt


  printf("GR_Draw_ipatch %d %d %d\n",gTyp,icol,iNr);


  // init GL-dispList
  GL_Surf_Ini (dli, NULL); //(void*)&iatt);  // glNewList (for gTyp=4,5,6,9)

  if(icol >= 0)
    GL_Col__ (icol);  // glColor3fv (GL_col_tab[icol]); 2=red,3=green,5=yellow.

  GL_set_ipatch (gTyp, iNr, ia, pa);

  // close GL-dispList
  GL_EndList ();                  // glEndList ();

  return 0;

}
*/

//===================================================================
  int GR_Disp_fan (Point *ps, Point *pa1, Point *pa2, int pNr, int att) {
//===================================================================
/// \code
/// DO NOT USE; replaced with GR_Disp_patch
/// Testdisplay Dreiecksflaechen; 
/// att: see GR_Disp_spu
/// pNr: Anzahl Punkte der Arrays pa1 und pa2.
/// Minimum: 1 Dreicke; pNr=1.
///
/// Beispiel: pNr=3.
///                a23
///              /  |
///           a22  /|
///         / /|  / |
///      a21 / | /  |
///     / |/   |/   |
///   ps--a11--a12--a13
/// \endcode



  long dli;


  dli=DL_StoreObj (Typ_GL_Sur, -1L, att);

  GL_Draw_Ini (&dli, att);
  GL_set_strip1 (ps, pa1, pa2, 1);
  GL_EndList ();
  
  return 0;

}


//===================================================================
  int GR_tDyn_rbez (CurvRBez *rbez, int att) {
//===================================================================
// GR_tDyn_rbez     Testdisplay  rational-bezier-curve via rational-B-spline
// att: see GR_temp_nobj
// see also GR_Disp_rbspl GL_Disp_rbez

  int       irc, ii;
  CurvRBSpl crbsp;
  Memspc    tmpSpc=UME_NEW, *memSeg1;
    

  // DEB_dump_obj__ (Typ_CVRBEZ, rbez, "GR_tDyn_rbez ptNr=%d va=%f vb=%f\n",
                  // rbez->ptNr,rbez->va,rbez->vb);
  GR_tDyn_nsymB (rbez->ptNr, rbez->cptab, SYM_STAR_S, 2);
    


  // get tempspace (gets lost on func-return)
  ii = 32 + rbez->ptNr * sizeof(Point) * 2;
  // UME_alloc_tmp (&tmpSpc, ii);
  UME_malloc (&tmpSpc, ii, ii/2);
  memSeg1 = &tmpSpc;


  // make rbspl from rbez
  UT3D_rbsp_rbez (&crbsp, rbez, memSeg1);
    // DEB_dump_obj__ (Typ_CVRBSP, &crbsp, " rbsp");

  // display rBspl
  // irc = GR_Disp_rbspl (&crbsp, att);
  irc = GR_tDyn_ocv (Typ_CVRBSP, &crbsp, 0L, att);

  UME_free (&tmpSpc);

  return ii;

/*
  // memstart = workSeg->next;
  pTab = memSeg1->next;
  ptMax = UME_ck_free(memSeg1) / sizeof(Point);
    printf("GR_tDyn_rbez Pts.free=%d\n",ptMax);

  dli = DL_StoreObj (Typ_CVRBEZ, -1L, 2);

  // rbez -> Polygon
  // irc = UT3D_pta_rbez (&ptNr, pTab, rbez, ptMax, UT_DISP_cv);

  if(irc >= 0) GR_tDyn_pcv (&dli, att, ptNr, pTab);
*/

  return 0;

}


//===================================================================
  int GR_tDyn_bez (CurvBez *bez, int att, Memspc *workSeg) {
//===================================================================
/// GR_tDyn_bez                   Testdisplay  Bezier-Curve
/// att: 2=rot, voll, dick

  int   irc, ptNr, ptMax;
  // long  dli;
  Point *pTab;
  void  *memstart;


  // DEB_dump_obj__ (Typ_CVBEZ, bez, "GR_tDyn_bez ptNr=%d va=%f vb=%f\n",
                  // bez->ptNr,bez->va,bez->vb);
  // GR_tDyn_npt__ (bez->ptNr, bez->cptab, SYM_STAR_S, 2);



  memstart = workSeg->next;
  pTab = workSeg->next;
  ptMax = UME_ck_free(workSeg) / sizeof(Point);
    // printf("GR_tDyn_bez Pts.free=%d\n",ptMax);

  // dli=DL_StoreObj (Typ_CVBEZ, -1L, 2);

  // bspl -> Polygon
  irc = UT3D_pta_bez (&ptNr, pTab, bez, ptMax, UT_DISP_cv);

  // if(irc >= 0) GR_tDyn_pcv (&dli, att, ptNr, pTab);
  if(irc >= 0) GR_tDyn_pcv (pTab, ptNr, att);

  // release work space
  workSeg->next = memstart;


  return 0;

}


//===================================================================
  int GR_tDyn_box2__ (Point2 *p1, Point2 *p2, int att) {
//===================================================================
// GR_tDyn_box2__        disp 2D-boundingBox from 2 points
//   att          see INF_COL_CV
//
//                 p2
//      1---------0
//      |         |
//      |         |
//      |         |
//      |         |
//      2---------3
//    p1           
//               
//              
//
// see also GR_tDyn_box__


  Point2 pta[5];


  if(DispMode) {
    printf("GR_tDyn_box2__ %d\n",att);
    DEB_dump_obj__ (Typ_PT2, p1, "");
    DEB_dump_obj__ (Typ_PT2, p2, "");
  }

  // s1
  pta[0].x = p2->x;
  pta[0].y = p2->y;

  pta[1].x = p1->x;
  pta[1].y = p2->y;

  pta[2].x = p1->x;
  pta[2].y = p1->y;

  pta[3].x = p2->x;
  pta[3].y = p1->y;

  pta[4] = pta[0];

  GR_tDyn_pcv2 (pta, 5, att);

  return 0;

}


//===================================================================
  int GR_tDyn_box__ (Point *p1, Point *p2, int att) {
//===================================================================
/// \code
/// GR_tDyn_box__        disp 3D-boundingBox from 2 points
/// att          see INF_COL_CV
///
///      s3---7---s2
///     /|        /|
///   11 |       8 |
///   /  10     /  4
///   x--|--9---x  |
///   |  s4---1-|--s1        s1 = Startpt of polygon1 ..
///  12  /      6  /
///   | 2       | 5
///   |/        |/
///   x----3----x
///
/// \endcode
// see also GL_disp_box


  long  dli;
  Point pta[4];


  if(DispMode) {
    printf("GR_tDyn_box__%d\n",att);
    DEB_dump_obj__ (Typ_PT, p1, "");
    DEB_dump_obj__ (Typ_PT, p2, "");
  }


  DL_tDyn_init (att);

  // GR_tDyn_symB__ (p1, SYM_TRI_S, 0);
  // GR_tDyn_symB__ (p2, SYM_TRI_S, 0);

  GL_att_cv (att);

  // s1
  pta[0].x = p2->x;
  pta[0].y = p2->y;
  pta[0].z = p1->z;

  pta[1].x = p1->x;
  pta[1].y = p2->y;
  pta[1].z = p1->z;

  pta[2].x = p1->x;
  pta[2].y = p1->y;
  pta[2].z = p1->z;

  pta[3].x = p2->x;
  pta[3].y = p1->y;
  pta[3].z = p1->z;

  // GL_DrawLStrip (pta, 4);
  GL_set_pcv (4, pta, 1);

  // s2
  pta[0].x = p2->x;
  pta[0].y = p2->y;
  pta[0].z = p2->z;

  pta[1].x = p2->x;
  pta[1].y = p2->y;
  pta[1].z = p1->z;

  pta[2].x = p2->x;
  pta[2].y = p1->y;
  pta[2].z = p1->z;

  pta[3].x = p2->x;
  pta[3].y = p1->y;
  pta[3].z = p2->z;

  // GL_DrawLStrip (pta, 4);
  GL_set_pcv (4, pta, 1);

  // s3
  pta[0].x = p1->x;
  pta[0].y = p2->y;
  pta[0].z = p2->z;

  pta[1].x = p2->x;
  pta[1].y = p2->y;
  pta[1].z = p2->z;

  pta[2].x = p2->x;
  pta[2].y = p1->y;
  pta[2].z = p2->z;

  pta[3].x = p1->x;
  pta[3].y = p1->y;
  pta[3].z = p2->z;

  // GL_DrawLStrip (pta, 4);
  GL_set_pcv (4, pta, 1);

  // s4
  pta[0].x = p1->x;
  pta[0].y = p2->y;
  pta[0].z = p1->z;

  pta[1].x = p1->x;
  pta[1].y = p2->y;
  pta[1].z = p2->z;

  pta[2].x = p1->x;
  pta[2].y = p1->y;
  pta[2].z = p2->z;

  pta[3].x = p1->x;
  pta[3].y = p1->y;
  pta[3].z = p1->z;

  // GL_DrawLStrip (pta, 4);
  GL_set_pcv (4, pta, 1);

  GL_list_close (); // close GL-record

  return 0;

}


//================================================================
  int GR_set_mtb (int opers, MemTab *mtb, int att) {
//================================================================
// GR_set_mtb                     display MemTab;
// see MemTab_dump

  int      irc=0, i1, ityp;
  Point    pta[2];
  Line     *lna;
  Line2    *ln2a;
  CurvCCV  *ccva;


  printf("============ GR_set_mtb ============= att=%d\n",att);
    printf(" rMax=%d rNr=%d\n",mtb->rMax,mtb->rNr);

  if(!mtb)          { printf(".. undefined ..\n");     return -1;}
  if(mtb->rMax < 1) { printf(".. uninitialized ..\n"); return -1;}
  if(mtb->rNr < 1)  { printf(".. empty ..\n"); return -1;}

  ityp = mtb->typ;


  if(opers & OPERS_TDYN) {
    DL_tDyn_init (att);

  } else if(opers & OPERS_TEMP) {
     DL_temp_init ();
  }


  //----------------------------------------------------------------
  if(ityp != Typ_LN) goto L_ln2;

  GL_att_cv (att);

  lna = MEMTAB_DAT (mtb);
  for(i1=0; i1<mtb->rNr; ++i1) {
    pta[0] = lna[i1].p1;
    pta[1] = lna[i1].p2;
    GL_set_pcv (2, pta, 1);
  }

  // or disp all individually:
  // for(i1=0; i1<mtb->rNr; ++i1) GR_tDyn_ln (&lna[i1], att);

  goto L_exit;


  //----------------------------------------------------------------
  L_ln2:
  if(ityp != Typ_LN2) goto L_cvTrm; 

  GL_att_cv (att);

  ln2a = MEMTAB_DAT (mtb);
  for(i1=0; i1<mtb->rNr; ++i1) {
    UT3D_pt_pt2_z (&pta[0], &ln2a[i1].p1, GL2D_Z);
    UT3D_pt_pt2_z (&pta[1], &ln2a[i1].p2, GL2D_Z);
    GL_set_pcv (2, pta, 1);
  }

  // or disp all individually:
  // for(i1=0; i1<mtb->rNr; ++i1) GR_tDyn_ln (&lna[i1], att);

  goto L_exit;

  //----------------------------------------------------------------
  L_cvTrm:
  if(ityp != Typ_CVTRM) goto L_err;

  ccva = MEMTAB_DAT (mtb);

  for(i1=0; i1<mtb->rNr; ++i1) GL_set_ocv (Typ_CVTRM, &ccva[i1], 0L, att);

  // or disp all individually:
  // for(i1=0; i1<mtb->rNr; ++i1) GR_tDyn_ccv (&lna[i1], att);

  goto L_exit;


  //----------------------------------------------------------------
  L_exit:
  if(opers & OPERS_CLOSE) GL_list_close ();
  return 0;


  //----------------------------------------------------------------
  L_err:
  printf("***** objTyp %d not (yet) supported ..\n",ityp);
  irc = -1;
  goto L_exit;

}


//================================================================
  int GR_Disp_spu (int pNr, Point *pTab, int att) {
//================================================================
/// \code
/// display nicht gelochte planare Flaeche
/// att: 5,7=blau, 8=gruen, 9,12=rot
///      6=hellgrau, 10=hellblau
/// \endcode

  long   ind;
  // Vector vcz;


  // printf("GR_Disp_spu %d %d\n",pNr,att);
  // GR_tDyn_npt__ (pNr, pTab, SYM_TRI_S, 2);

  ind = -1;
  GR_Draw_spu (&ind, att, pNr, pTab);

/*
  // Normalvektor
  UT3D_vc_perpcv (&vcz, pNr, pTab);

  dli=DL_StoreObj (Typ_SURPLN, -1L, att);

  GL_Draw_Ini (&dli, att);  // Flaechenfarbe
  GLT_spu_ (pNr, pTab, &vcz);
  GL_EndList ();
*/

  return 0;

}


/* UU
//================================================================
  int GR_Disp_spu1 (int pNr, Point *pTab, int att) {
//================================================================
/// Display planare Contour. Jedoch mit GLT_spp__  statt GLT_spu_


  int     i1;
  long    ind, dli;
  Vector  vcn;
  ContTab ct1;
  ObjGX   *oxi;



  if(DispMode) {
    printf("GR_Disp_spu1  %d %d\n",pNr,att);
    for(i1=0; i1<pNr; ++i1)
      DEB_dump_obj__(Typ_PT, &pTab[i1], "  pt[%d]=",i1);
  }


  ind = -1;

  dli = DL_StoreObj (Typ_SUR, ind, att);

  // Normalvektor
  UT3D_vc_perpcv (&vcn, pNr, pTab);

  // make ContTab from points
  SUP_ContTab_pTab (&ct1, pNr, pTab, &vcn);

  L_draw:
    GLT_start__ (&oxi);                  // new Surf
    i1 = GLT_spp_pp (&vcn);              // new Patch
    if(i1 == -10) goto L_draw;           // realloc
    i1 = GLT_spp__ (&ct1, 1, &vcn);       // tesselate

  GL_Draw_Ini (&dli, att);
  i1 = GL_set_bMsh (oxi);              // draw OpenGL
  GL_EndList ();

  return 0;

}
*/


//================================================================
  int GR_Draw_spu (long *ind, int att, int pNr, Point *pTab) {
//================================================================
/// \code
/// display nicht gelochte planare Flaeche
/// att: see GR_Disp_spu
/// \endcode

  int     i1;
  long    dli;
  Point   pm;
  Vector  vcn, v1, v2;
  ContTab ct1;
  ObjGX   *gSur;



  // printf("GR_Draw_spu %d %d %d\n",*ind,att,pNr);
  // GR_tDyn_npt__ (pNr, pTab, SYM_TRI_S, 2);


  // Normalvektor mit manchmal falschem Umlaufsinn:
  UT3D_vc_perpcv (&vcn, pNr, pTab);

  // make ContTab from points
  SUP_ContTab_pTab (&ct1, pNr, pTab, &vcn);


  L_draw:
    GLT_start__ (&gSur);                  // new Surf
    i1 = GLT_spp_pp (&vcn);               // new Patch
    if(i1 == -10) goto L_draw;            // realloc
    i1 = GLT_spp__ (&ct1, 1, &vcn);       // tesselate



  // Ausgeben
  if(TSU_mode == 0) {
    // display
    GL_set_sur_tess (gSur, att);

  } else {
    // store
    TSU_store (gSur);
  }


  return 0;
}


/*
Alte Version, arbeitet nicht in die Ausgabebuffer ...
//================================================================
  int GR_Draw_spu (long *ind, int att, int pNr, Point *pTab) {
//================================================================
// display nicht gelochte planare Flaeche
// att: see GR_Disp_spu

  long   dli;
  Point  pm;
  Vector vcz, v1, v2;


  // printf("GR_Draw_spu %d %d %d\n",*ind,att,pNr);
  // GR_tDyn_npt__ (pNr, pTab, SYM_TRI_S, 2);


  // Normalvektor mit mnchmal falschem Umlaufsinn:
  UT3D_vc_perpcv (&vcz, pNr, pTab);

  dli=DL_StoreObj (Typ_SUR, *ind, att);

  GL_Draw_Ini (&dli, att);  // Flaechenfarbe
  GLT_spu_ (pNr, pTab, &vcz);
  GL_EndList ();

  return 0;
}


//===================================================================
  int GR_Disp_sur (ObjGX *oxi, int att) {
//===================================================================
/// \code
/// Testdisplay  Surface
/// att: surface-color; see UTcol__3i or APcol_defCol_3i
/// ATTENTION: is using mem501 & mem201
/// \endcode


  return TSU_DrawSurT_ (oxi, att, -1L);

}



//===================================================================
  int GR_Disp_sru (ObjGX *oxi, int att) {
//===================================================================
/// \code
/// Tesselate / Display ungetrimmte/ungelochte Ruled Flaeche
/// Testdisplay  Ruled-Surf
/// att: 2=rot, voll, dick
/// \endcode


  return TSU_DrawSurTRU (oxi, att, -1L);

}


//===================================================================
  int GR_Disp_sbsp (int ptUNr, int ptVNr,  Point *pTab, int att) {
//===================================================================
/// \code
/// Testdisplay  BSP-Surf; nur Kontrollpunkte - nix dazwischen !!!
/// att: 2=rot, voll, dick
/// \endcode

  long dli;


  dli=DL_StoreObj (Typ_SURBSP, -1L, 2);

  GR_DrawTriaStrip (&dli, att, ptUNr, ptVNr, pTab, Typ_SURBSP);

  return 0;

}
*/

//================================================================
  int GR_temp_src (char *src, int att, int mode) {
//================================================================
// GR_temp_src           tempDisp obj from sourcecode
// Input:
//   src             sourcecode of obj to display
//   att,mode                               see GR_temp_nobj
//
// see IE_cad_Inp_disp__

  int       irc, ind, typo, iatt;
  long      dbi;
  ObjAto    ato1;
  char      objo[OBJ_SIZ_MAX];

  // printf("GR_temp_src |%s|\n",src);

  // make bin.obj from src

  // get memSpc for ato1 (memspc53, 54, 55)
  ATO_getSpc__ (&ato1);

  // get bin.obj from sourceObj
  typo = UTO_obj_cnvt_src (objo, Typ_unknown, src);
  if(typo < 0) return -1;
    // printf(" foll-_cnvt_src-irc=%d\n",typo);

  // dli = -1L;
  dbi = 0L;
  iatt = 0;
  GR_temp_nobj (0, dbi, typo, objo, 1, iatt, 0);


  return 0;

}


//====================================================================
  int GR_tDyn_nobj (int oTyp, void *obj, int oNr, int att, int mode) {
//====================================================================
// \code
// GR_tDyn_nobj     tempDisp objects from typ+struct, nr
// Input:
//   oTyp     type of sObj; eg. Typ_PT      see GR_temp_nobj
//   obj      data, struct; eg. (Point*)
//   oNr      nr of objects of type <typ> in obj
//   att,mode                               see GR_temp_nobj
//
// see also:
// Point    GR_tDyn_symB__ or APT_disp_SymB
// Vectors: GR_Disp_vc
// Text:    GR_tDyn_txtA
// \endcode

// see GR_tDyn_dbo GR_Disp_ox UTO_obj_Disp__ SUR_Disp_Sur


  long    dbi;

  dbi = 0L;   // only display, no DB

  return GR_temp_nobj (0, dbi, oTyp, obj, oNr, att, mode);

}


//================================================================
  int GR_tDyn_obj (int oTyp, void *obj, int att, int mode) {
//================================================================
/// \code
/// GR_tDyn_obj     tempDisp obj from typ+struct
/// Input:
///   oTyp     type of sObj; eg. Typ_PT      see GR_temp_nobj
///   obj      data, struct; eg. (Point*)
///   att,mode                               see GR_temp_nobj
///
/// see also:
/// Point    GR_tDyn_symB__ or APT_disp_SymB
/// Vectors: GR_Disp_vc
/// Text:    GR_tDyn_txtA
/// \endcode

// see GR_tDyn_dbo GR_Disp_ox UTO_obj_Disp__ SUR_Disp_Sur


  long    dbi;
  // ObjGX  ox;


  // OGX_SET_OBJ (&ox, oTyp, oTyp, 1, sObj);
  // GR_Disp_ox (&ox, att, mode);

  // dli = DL_StoreObj (oTyp, -1L, att);

  dbi = 0L;   // only display, no DB

  // printf("GR_tDyn_obj typ=%d dli=%ld att=%d\n",oTyp,dli,att);
  // DEB_dump_obj__ (oTyp, obj, "  GR_tDyn_obj");


  return GR_temp_nobj (0, dbi, oTyp, obj, 1, att, mode);

}


//================================================================
  int GR_DrawSup (int pNr, Point *pa, int iCon) {
//================================================================
/// \code
/// draw planare surface
/// Input:
///   iCon      1=Konvex; -1=Konkav
/// \endcode

  int     i1;
  Vector  vcz;
  ContTab ct1;
  ObjGX   *oxi;


  // printf("GR_DrawSup %d %d\n",pNr,iCon);



  if(iCon > 0) {       // 1=Konvex; -1=Konkav
    // Konvexe Basisflaeche:
    GR_DrawFan (pa, pNr-1, &pa[1], 0, Typ_SURPLN);

  } else {
    // Konkave Basisflaeche:
    UT3D_vc_perpcv (&vcz, pNr, pa);  // Normalvektor

    // make ContTab from points
    SUP_ContTab_pTab (&ct1, pNr, pa, &vcz);

    GLT_start__ (&oxi);                  // new Surf

    i1 = GLT_spp_pp (&vcz);              // new Patch

    if((GL_actCol.color != 0) || (GL_actCol.vtra != 0))
      GLT_spp_Col (&GL_actCol);

    if(TSU_mode != 0)
      GLT_spp_sTyp (Typ_SURPLN);  // store surfTyp for intersect

    i1 = GLT_spp__ (&ct1, 1, &vcz);       // tesselate

    if(TSU_mode == 0) {
      i1 = GL_set_bMsh (oxi);            // draw OpenGL
    } else {
      TSU_store (oxi);
    }

    return 0;
  }

  return 0;
}


//===========================================================================
  int GR_CreFan (long *ind,int att,Point *pt1,int ptNr,Point *pa,int typ) {
//===========================================================================
/// \code
/// typ: 0 = planarer Fan; 1 = nicht planarer Fan
/// do always start a new surface.
/// \endcode

  long   dli;
  int    newS;

  if(TSU_mode == 0) {
    dli = DL_StoreObj (Typ_SUR, *ind, att);

    // GL_Draw_Ini (&dlInd, att);
    GL_Surf_Ini (&dli, att);
  }

  if(typ == 0) newS = Typ_SURPLN;
  else         newS = Typ_SUR;

  GR_DrawFan (pt1, ptNr, pa, typ, newS);

  if(TSU_mode == 0) {
    GL_EndList ();
    AP_dli_act = dli;
  }

  return 0;

}


//=======================================================================
  int GR_DrawFan (Point *pt1, int ptAnz, Point *pa1, int typ, int newS) {
//=======================================================================
/// \code
/// typ: 0 = planarer Fan; 1 = nicht planarer Fan
/// newS:  0=do not start new surface; else start new Surf with this type
/// \endcode

  // printf("GR_DrawFan %d %d\n",ptAnz,TSU_mode);


  if(TSU_mode != 0) {
    // store
    GLT_stor_rec (0, NULL, NULL, 0);  // init

    if((GL_actCol.color != 0) || (GL_actCol.vtra != 0))  {
      GLT_stor_rec (5, NULL, NULL, 0);

    } else if (GL_actCol.vtex != 0) {
      // activate active TexRefNr
      GLT_stor_rec (8, NULL, NULL, GL_actTex);
    }


  }  else {
    // display
    // if((GL_actCol.color != 0) || (GL_actCol.vtra != 0))
    GL_ColSet (&GL_actCol);
  }

  GL_set_fan (pt1, ptAnz, pa1, typ, newS);

  if(TSU_mode != 0)
    GLT_stor_rec (1, NULL, NULL, 0);  // save

  return 0;

}

//================================================================
  int GR_set_strip_v (Point *pa, Point *pb, int ptAnz, int newS) {
//================================================================
// GR_set_strip_v        disp strip vertical
// newS:  0 = do not start new surface
// newS:  1 = start new surface
//
//
//     pa      pb
//      x-------x    [0]
//      |     / |
//      |   /   |
//      | /     |
//      x-------x    [1]
//      |     / |
//      |   /   |
//      | /     |
//      x-------x    [2]
//
//
//

  // printf("GR_set_strip_v ptAnz=%d TSU_mode=%d newS=%d\n",ptAnz,TSU_mode,newS);
  // {int i1;for(i1=0;i1<ptAnz;++i1){
    // printf("Strip[%d] %.2f,%.2f,%.2f - %.2f,%.2f,%.2f\n",
      // i1,pa[i1].x,pa[i1].y,pa[i1].z,pb[i1].x,pb[i1].y,pb[i1].z);
    // GR_set_obj(OPERS_TDYN,0L,Typ_PT,&pa[i1],1,0);
  // }}
  // DEB_dump_obj__ (Typ_Color, &GL_actCol, "  GL_actCol:");
    
  
  if(TSU_mode != 0) {                 // 0=draw; 1=store
    GLT_stor_rec (0, NULL, NULL, 0);  // init

    if((GL_actCol.color != 0) || (GL_actCol.vtra != 0)) {
      GLT_stor_rec (5, NULL, NULL, 0);

    } else if (GL_actCol.vtex != 0) {
      // activate active TexRefNr
      GLT_stor_rec (8, NULL, NULL, GL_actTex);
    }

  }  else {
    // if((GL_actCol.color != 0) || (GL_actCol.vtra != 0))
    GL_ColSet (&GL_actCol);
  }

  // disp
  GL_set_strip_v (pa, pb, ptAnz, newS);

  if(TSU_mode != 0)
    GLT_stor_rec (1, NULL, NULL, 0);  // save

  return 0;

}


//================================================================
  int GR_DrawStrip (Point *pa1, Point *pa2, int ptAnz, int newS) {
//================================================================
// GR_DrawStrip           add GL-STRIPes into open dispList
// REPLACE WITH GR_set_strip_v
// newS:  0 = do not start new surface
// newS:  1 = start new surface


  // printf("GR_DrawStrip ptAnz=%d TSU_mode=%d newS=%d\n",ptAnz,TSU_mode,newS);
  // {int i1;for(i1=0;i1<ptAnz;++i1){
    // printf("Strip[%d] %.2f,%.2f,%.2f - %.2f,%.2f,%.2f\n",
      // i1,pa1[i1].x,pa1[i1].y,pa1[i1].z,pa2[i1].x,pa2[i1].y,pa2[i1].z);
    // GR_set_obj(OPERS_TDYN,0L,Typ_PT,&pa1[i1],1,0);
  // }}
  // DEB_dump_obj__ (Typ_Color, &GL_actCol, "  GL_actCol:");


  if(TSU_mode != 0) {                 // 0=draw; 1=store
    GLT_stor_rec (0, NULL, NULL, 0);  // init

    if((GL_actCol.color != 0) || (GL_actCol.vtra != 0)) {
      GLT_stor_rec (5, NULL, NULL, 0);

    } else if (GL_actCol.vtex != 0) {
      // activate active TexRefNr
      GLT_stor_rec (8, NULL, NULL, GL_actTex);
    }

  }  else {
    // if((GL_actCol.color != 0) || (GL_actCol.vtra != 0))
    GL_ColSet (&GL_actCol);
  }

  // disp
  // GL_set_strip_v (pa1, pa2, ptAnz, newS);
  GL_set_strip_v (pa1, pa2, ptAnz, newS);

  if(TSU_mode != 0)
    GLT_stor_rec (1, NULL, NULL, 0);  // save

  return 0;

}


//================================================================
  int GR_draw_mesh (long dli, long dbi, ColRGB *col,
                    Point *pTab, Fac3 *fTab, int fNr) {
//================================================================
// Input:
//   dli      >= 0    modify existing object
//            -1L     create new object
//            <= -2L  create a temporary object
//   col      styl     0 = ON  = shade;     1 = OFF = symbolic
//
// see GR_Disp_nfac


  int     att;


  // printf("GR_draw_mesh mode=%d dbi=%ld fNr=%d\n",mode,dbi,fNr);


  // temp-obj's
  if(dli <= -2L) goto L_disp;



  // (Re-)Display mesh
  if(dli >= 0L) {     // modify existing object
    // use existing dli - modify
    // dli = DL_dli__dbo (Typ_SUR, dbi, -1L);
      // printf(" dbi=%ld dli=%ld\n",dbi,dli);
    DL_SetInd (dli);  // modify (do not create new DL-Record)

  // } else if(dli == -1L) {  // create new object
  // } else if(dli < 0) {   // create a temporary object
    // dli = -1L;

  }


  att = 0; // 5; // 8; // ? APT_DrawSur GR_DrawSur TSU_DrawSurPMsh

  dli = DL_StoreObj (Typ_SUR, dbi, *(int*)col);



  // MSH_dump_fTab (ActFtab.data, ActFtab.rNr);


  L_disp:
  GL_Surf_Ini (&dli, col);           // att unused ..
  GL_ColSet (col);                   // 2015-09-30
  GL_set_nfac_V1 (pTab, fTab, fNr);
  GL_EndList ();


  return 0;

}


//=========================================================================
  void GR_Disp_nfac (Point *pTab, Fac3 *fTab, int fNr, ColRGB *col) {
//=========================================================================
/// \code
/// GR_Disp_nfac        display (not indexed) Fac3-faces
/// newS:  0 = do not start new surface
/// newS:  1 = start new surface
/// \endcode

// see TSU_DrawSurPMsh



  // printf("GR_Disp_nfac fNr=%d TSU_mode=%d\n",fNr,TSU_mode);
  // DEB_dump_obj__ (Typ_Color, &GL_actCol, "  GL_actCol:");


  if(TSU_mode != 0) {
    // 1=speichern
    GLT_stor_rec (0, NULL, NULL, 0);  // init

    if((GL_actCol.color != 0) || (GL_actCol.vtra != 0)) {
      GLT_stor_rec (5, NULL, NULL, 0);

    } else if (GL_actCol.vtex != 0) {
      // activate active TexRefNr
      GLT_stor_rec (8, NULL, NULL, GL_actTex);
    }

  }  else {
    // 0=normal darstellen
    // if((GL_actCol.color != 0) || (GL_actCol.vtra != 0))
    GL_ColSet (&GL_actCol);
  }

  GL_set_nfac_V1 (pTab, fTab, fNr);

  if(TSU_mode != 0)
    GLT_stor_rec (1, NULL, NULL, 0);  // save

  return;

}


/*
//=======================================================================
  int GR_tDyn_bsp2 (long *dli, long dbi, int att, CurvBSpl2 *bspl) {
//=======================================================================
// ATTENTION: is using memspc102
// see GR_DrawCurv GR_Disp_CvBSp

//
  int   irc, ptNr, ptMax;
  Point *pTab;
  // void  *memstart;


  // printf("GR_DrawCvBSp dbi=%ld ptNr=%d deg=%d dli=%ld\n",
         // dbi, bspl->ptNr, bspl->deg,*dli);
  // DEB_dump_obj__ (Typ_CVBSP, bspl, "CV:\n");
  // GR_tDyn_npt__ (bspl->ptNr, bspl->cpTab, SYM_STAR_S, 2);



  if((dbi > 0)&&(*dli > 0L)&&(APT_obj_stat == 0)) {
    // bspl -> Polygon using PRCV
    irc = PRCV_npt_dbo__ (&pTab, &ptNr, Typ_CVBSP, dbi, AP_modact_ibm);
    if(irc < 0) return -1;


  } else {

    // get memSpc for points
    UT3D_ptNr_bsp (&ptNr, bspl, UT_DISP_cv);
    ptMax = ptNr * 2;
    pTab = (Point*)MEM_alloc_tmp((int)(sizeof(Point)*ptMax));
    // pTab = (void*) memspc102;
    // ptMax = sizeof(memspc102) / sizeof(Point);
    // memstart = workSeg->next;
    // pTab = workSeg->next;
    // ptMax = UME_ck_free(workSeg) / sizeof(Point);

    // bspl -> Polygon
    irc = UT3D_pta_bsp (&ptNr, pTab, bspl, ptMax, UT_DISP_cv);
    if(irc < 0) return -1;
  }


  // display
  GR_tDyn_pcv (dli, att, ptNr, pTab);


  // disp direction
  if(APT_dispDir) {
    int   ipe;
    ipe = ptNr - 1;
    APT_disp_dir (&pTab[ipe], &pTab[ipe - 1]);
  }


  // // release work space
  // workSeg->next = memstart;
//

  return 0;

}
*/

//=======================================================================
  int GR_tDyn_bsp2 (long *dli, long dbi, int att, CurvBSpl2 *bspl) {
//=======================================================================
// GR_tDyn_bsp2                     display 2D-bspline
// see GR_DrawCurv GR_Disp_CvBSp


  int      irc;
  long     l1;
  void     *m1Spc;
  Memspc   mSpc;
  CurvBSpl bsp3;


  // DEB_dump_obj__ (Typ_CVBSP2, bspl, "GR_tDyn_bsp2");
      
  // get memspace
  l1 = (sizeof(Point) + sizeof(double)) * bspl->ptNr + 16;  // 
  m1Spc = MEM_alloc_tmp (l1);
  UME_init (&mSpc, m1Spc, l1);
    
  // 3D-bspl from 2D.bspl
  UT3D_bsp_tra_bsp2_rsys ((CurvBSpl*)&bsp3, (CurvBSpl2*)bspl,
                          (Refsys*)&UT3D_RSYS_2D, &mSpc);
    // DEB_dump_obj__ (Typ_CVBSP, &bsp3, "bsp3");
    
    
  // disp 3D-bspl 
  // irc = GR_DrawCvBSp (dli, dbi, att, &bsp3);
  irc = GR_tDyn_ocv (Typ_CVBSP, &bsp3, dbi, att);

  return irc;
      
}     


/*
//=======================================================================
  int GR_DrawCvBSp (long *dli, long dbi, int att, CurvBSpl *bspl) {
//=======================================================================
// GR_DrawCvBSp                           display bspline
// see GR_DrawCurv GR_Disp_CvBSp

 
  int   irc, ptNr, ptMax;
  Point *pTab;
  // void  *memstart;

  // printf("GR_DrawCvBSp dbi=%ld ptNr=%d deg=%d dli=%ld\n",
         // dbi, bspl->ptNr, bspl->deg,*dli);
  // DEB_dump_obj__ (Typ_CVBSP, bspl, "CV:\n");
  // GR_tDyn_npt__ (bspl->ptNr, bspl->cpTab, SYM_STAR_S, 2);

  GR_typ = Typ_CV;
  GR_dbi = dbi;

  if((dbi > 0)&&(*dli > 0L)&&(APT_obj_stat == 0)) {
    // bspl -> Polygon using PRCV
    irc = PRCV_npt_dbo__ (&pTab, &ptNr, Typ_CVBSP, dbi, AP_modact_ibm);
    if(irc < 0) return -1;

  } else {

    // get memSpc for points
    UT3D_ptNr_bsp (&ptNr, bspl, UT_DISP_cv); 
    ptMax = ptNr * 2;
    pTab = (Point*)MEM_alloc_tmp((int)(sizeof(Point)*ptMax));
    // pTab = (void*) memspc102;
    // ptMax = sizeof(memspc102) / sizeof(Point);
    // memstart = workSeg->next;
    // pTab = workSeg->next;
    // ptMax = UME_ck_free(workSeg) / sizeof(Point);

    // bspl -> Polygon
    irc = UT3D_pta_bsp (&ptNr, pTab, bspl, ptMax, UT_DISP_cv);
    if(irc < 0) return -1;
  }

  // display
  GR_DrawPoly (dli, att, ptNr, pTab);

  // // release work space
  // workSeg->next = memstart;

  return 0;

}
*/

//================================================================
  int GR_tDyn_el2c (CurvEll2C *el2c, int att) {
//================================================================
// GR_tDyn_el2c          disp. 2D-ellipse-centerPos
// Z-value is GL2D_Z

  long        dli;
  CurvElli    el1;


  DEB_dump_obj__ (Typ_CVELL2C, el2c, "GR_tDyn_el2c el");


  // get 3D-ellipse from 2D-ellipse el2c
  el1.p1 = UT3D_pt_pt2 (&el2c->p1);
  el1.p2 = UT3D_pt_pt2 (&el2c->p2);
  el1.pc = UT3D_PT_NUL;

  el1.vz = UT3D_VECTOR_Z;
  UT3D_vc_multvc (&el1.va, &UT3D_VECTOR_X, el2c->a);
  UT3D_vc_multvc (&el1.vb, &UT3D_VECTOR_Y, el2c->b);

  el1.ango = el2c->ango;
  el1.srot = el2c->srot;
  el1.clo  = -1;
  el1.trm  = -1;
    DEB_dump_obj__ (Typ_CVELL, &el1, " Disp_el2c el2c");

  // dli = DL_StoreObj (Typ_CVELL, -1L, 2);
  // return GR_DrawCvEll (&dli, -1L, att, &el1);
  return GR_tDyn_ell (&el1, att);

}


//================================================================
  int GR_tDyn_ell (CurvElli *el1, int att) {
//================================================================
/// \code
/// Testdisplay Ellipse
/// att                 see ~/gCAD3D/cfg/ltyp.rc INF_COL_CV
/// \endcode
               
  long  dli;

  printf("GR_tDyn_ell att=%d\n",att);

  // return GR_DrawCvEll (&dli, -1L, att, el1);
  return GR_tDyn_ocv (Typ_CVELL, el1, 0L, att);

}


/*
//=======================================================================
  int GR_DrawCvEll2 (long *ind, int att, CurvEll2 *el2) {
//=======================================================================
// see GR_tDyn_ell GR_DrawCvEll
// TODO: does not have parameter dbi (for PCV; see eg GR_DrawCirc GR_DrawCvBSp)

  int        ptNr, pnMax;
  Point2     *pa;
  CurvEll2C  elc;


  DEB_dump_obj__ (Typ_CVELL2, el2, "GR_DrawCvEll2");
  // GR_tDyn_symB__ (&el1->pc, SYM_STAR_S, 0);
  // GR_tDyn_symB__ (&el1->p1, SYM_TRI_S, ATT_COL_GREEN);
  // GR_tDyn_symB__ (&el1->p2, SYM_TRI_S, ATT_COL_RED);
  // GR_tDyn_vc__ (&el1->va, &el1->pc, 8, 1);  // green
  // GR_tDyn_vc__ (&el1->vb, &el1->pc, 9, 1);  // red


  pa = MEM_alloc_tmp (SPC_MAX_STK);
  pnMax = SPC_MAX_STK / sizeof(Point2);
    printf(" pnMax=%d\n",pnMax);


  // get elc from el2
  UT2D_elc_el2 (&el2c, el2);

//
  // nr of points for polygon
  ptNr = UT3D_ptNr_ell (el1, UT_DISP_cv);
    // printf(" ptNr-ell=%d\n",ptNr);


  pa = MEM_alloc_tmp ((int)(ptNr * sizeof(Point)));


  // if(ptNr > ptmax) goto L_e1;
  UT3D_npt_ell (ptNr, pa, el1);
    // UT3D_cv_ell (GLT_pta, &ptAnz, el1, ianz, UT_DISP_cv);
    // GR_tDyn_npt__ (ptNr, pa, SYM_STAR_S, 2);


  GR_tDyn_pcv (ind, att, ptNr, pa);


  // disp direction
  if(APT_dispDir) {
    int   ipe;
    ipe = ptNr - 1;
    APT_disp_dir (&pa[ipe], &pa[ipe - 1]);
  }
//

  return 0;

}


//=======================================================================
  int GR_DrawCvEll2 (long *ind, long dbi, int att, CurvEll2 *el2) {
//=======================================================================
// GR_DrawCvEll2                   display 2D-ellipse
//
// see GR_tDyn_ell GR_DrawCvEll
// TODO: does not have parameter dbi (for PCV; see eg GR_DrawCirc GR_DrawCvBSp)

  int        irc, pNr, ptNr;
  long       l1;
  Point2     *pa;
  CurvEll2C  elc;


  // DEB_dump_obj__ (Typ_CVELL2, el2, "GR_DrawCvEll2");
  // GR_tDyn_symB__ (&el1->pc, SYM_STAR_S, 0);
  // GR_tDyn_symB__ (&el1->p1, SYM_TRI_S, ATT_COL_GREEN);
  // GR_tDyn_symB__ (&el1->p2, SYM_TRI_S, ATT_COL_RED);
  // GR_tDyn_vc__ (&el1->va, &el1->pc, 8, 1);  // green
  // GR_tDyn_vc__ (&el1->vb, &el1->pc, 9, 1);  // red

  GR_typ = Typ_CV;
  GR_dbi = dbi;

  // get elc from el2
  UT2D_elc_el2 (&elc, el2);
    // DEB_dump_obj__ (Typ_CVELL2C, &elc, "  elc");

  // get nr of points for polygon
  ptNr = UT2D_ptNr_el2c (&elc, UT_DISP_cv);
    // printf(" ptNr-ell=%d\n",ptNr);

  // get space for 2D-points
  l1 = sizeof(Point2) * ptNr;
  pa = MEM_alloc_tmp (l1);

  // get polygon for elc
  pNr = ptNr;
  irc = UT2D_npt_elc (&pNr, pa, &elc, UT_DISP_cv);
  if(irc < 0) return -1;

  // transform polygon onto el2
  UT2D_npt_tra_el2 (pNr, pa, el2, &elc);

  // display polygon on el2
    // GR_tDyn_pcv2 (pa, ptNr, Typ_Att_top2);

  // display
  GR_tDyn_pcv2D (ind, att, pNr, pa, 0.);

  // // disp direction
  // if(APT_dispDir) {
    // int   ipe;
    // ipe = pNr - 1;
    // APT_disp_dir (&pa[ipe], &pa[ipe - 1]);
  // }

  return 0;

}


//================================================================
  int GR_Disp_ox (ObjGX *ox1, int att, int mode) {
//================================================================
/// \code
/// Input:
///   ox1 
///   att,mode see GR_temp_nobj
/// 
/// see GR_Draw_ox GR_tDyn_obj GR_tDyn_dbo GR_Draw_dbo
/// \endcode

// TODO:   repl with GR_temp_ox | GR_tDyn_ox


  long  dbi;


  // printf("GR_Disp_ox typ=%d form=%d att=%d\n",ox1->typ,ox1->form,att);


  // dli = DL_StoreObj (ox1->typ, -1L, att);

  dbi = 0;   // only display, no DB

  return GR_Draw_ox (0, dbi, ox1, att, mode);

}


//==========================================================================
  int GR_Draw_ox (int ind, long dbi, ObjGX *ox1, int att, int mode) {
//==========================================================================
/// \code
/// Input:
///   ind      DispListIndex; get it from DL_StoreObj
///   att      points: 0=default; 1=hili; 2=dimmed; 3=green; 4=yellow.
///   mode     points: unused
///   ox1 
/// 
/// see GR_Disp_ox GR_tDyn_obj GR_tDyn_dbo GR_Draw_dbo
///     APT_DrawCurv GR_DrawCurv
/// \endcode

// use *ind = -2 for temp.Obj (fixed temp index)


  int       irc, oTyp, i1;
  long      dli;
  ObjGX     *ox2, o2;


  // printf("GR_Draw_ox ind=%ld att=%d typ=%d form=%d siz=%d\n",*ind,att,
          // ox1->typ,ox1->form,ox1->siz);
  // DEB_dump_ox_0 (ox1, "GR_Draw_ox");


  oTyp = ox1->form;


  switch (oTyp) {

    //================================================================
    case Typ_Index:
      dbi = OGX_GET_INT (ox1);
      o2 = DB_GetObjGX (ox1->typ, dbi);
      GR_Draw_ox (ind, dbi, &o2, att, mode);
      break;


    case Typ_ObjGX:
      ox2 = ox1->data;
      for(i1=0; i1 < ox1->siz; ++i1) // Recursion !
        GR_Draw_ox (ind, dbi, &ox2[i1], att, mode);
      break;


    // //================================================================
    default:
      // handle content of the ObjGX
      irc = GR_temp_nobj (ind, dbi, oTyp, ox1->data, ox1->siz, att, mode);
      if(irc < 0) return irc;
  }

  return 0;


  L_E001:
      TX_Error("GR_Draw_ox not yet impl: %d %d",ox1->typ,ox1->form);
      return -1;

  L_E002:
      TX_Error("GR_Draw_ox E002"); // par fehlt ..
      return -1;

  // L_E003:
      // TX_Error("GR_Draw_ox E003"); // wrkSpc fehlt ..
      // return -1;

}
*/

//==============================================================================
  int GR_temp_nobj (int ind, long dbi,
                   int form, void *obj, int oNr, int att, int mode) {
//==============================================================================
// GR_temp_nobj    display temp.obj
// Input:
//   ind      fixed index: 1 - (DL_base_font1 - 1)
//            0    get next free index
//   dbi      DB-index if known (used for getting PRCV)
//   form,obj obj to display;
//   oNr      nr of objects of type <typ> in obj
//   att      Typ_PT: ATT_PT_BLACK .. see INF_COL_PT
//              ATT_PT_BLACK|ATT_PT_HILI|ATT_PT_DIMMED|ATT_PT_GREEN|ATT_PT_YELLOW
//            Typ_CI|Typ_LN|Typ_CV*|Typ_PLN:   see INF_COL_CV
//            Typ_SymB:   colorIndex; eg ATT_COL_RED; see INF_COL_SYMB
//              ATT_COL_BLACK|ATT_COL_RED|ATT_COL_GREEN|ATT_COL_BLUE|
//              ATT_COL_YELLOW|ATT_COL_MAGENTA|ATT_COL_CYAN|ATT_COL_WHITE
//              ATT_COL_HILI|ATT_COL_DIMMED
//   mode     Typ_PT|Typ_LN|Typ_CI|Typ_CV*: unused;
//            Typ_VC|Typ_VC2: 0=length-normalized; 1=true-length
//            Typ_SymB: SYM_TRI_S|SYM_STAR_S|SYM_CIR_S|SYM_SQU_B
//            Typ_PLN:  1=Plane; 2=Axis; 4=Axis+Chars; 5=Plane+Axis;
// 
// Vectors: GR_Disp_vc
// Text:    GR_tDyn_txtA 
//
// see GR_Disp_ox GR_tDyn_obj GR_tDyn_dbo GR_Draw_dbo
//     APT_DrawCurv GR_DrawCurv


  int       irc, oTyp, i1;
  void      *p1;
  void      *o1;
  Point     pt1, pt2;
  DL_Att    *att1;
  ObjGX     *ox2, ogx;
  // Memspc    tSpc1 = UME_NEW;


  // printf("GR_temp_nobj ind=%d att=%d form=%d oNr=%d\n",ind,att,form,oNr);
  // DEB_dump_nobj__ (form, oNr, obj, "GR_temp_nobj");


  // oTyp = typ;
  DL_temp_ind = ind;


  switch (form) {
/*
    //================================================================
    case Typ_PT2:
      // o1 = MEM_alloc_tmp ((int)sizeof(Point));
      L_PT2:
        // *((Point*)o1) = UT3D_pt_pt2 (obj);
        GR_Draw_pt2 (ind, att, (Point2*)obj);
        if(oNr > 1) {
          --oNr;
          obj = (char*)obj + sizeof(Point2);
          goto L_PT2;
        }
        break;
*/
    //================================================================
    case Typ_PT:
      // see APT_DrawPoint
      // GR_DrawPoint (dli, att, (Point*)obj);
      GR_temp_pt ((Point*)obj, ATT_PT_HILI);
      break;

    //================================================================
    case Typ_VC:
      // normalized; see also UI_prev_vc
      // pt1 = GL_GetCen ();
      // GL_DrawSymV3 (dli, SYM_ARROW, att, &pt1, (Vector*)obj, 10.);
      // GR_temp_vc (ind, obj, NULL, att, mode);
      GR_temp_vc ((Vector*)obj, NULL, att, mode);
      break;
/*
    //================================================================
    case Typ_VC2:
      GR_Draw_vc2 (ind, obj, NULL, att, mode);
      break;
*/
    //================================================================
    case Typ_SymB:
      // see APT_disp_SymB GL_DrawSymB
      // GL_DrawSymB (ind, att, mode, (Point*)obj);
      GR_temp_symB ((Point*)obj, mode, att);
      break;

    //================================================================
    case Typ_LN:
      // GR_DrawLine (ind, dbi, att, (Line*)obj);
      GR_temp_ln ((Line*)obj, att);
      break;
/*
    //================================================================
    case Typ_LN2:
      GR_Draw_ln2 (ind, dbi, att, (Line2*)obj);
      break;
*/
    //================================================================
    case Typ_CI:
    case Typ_CVPOL:     // CurvPoly
    case Typ_CVELL:
    case Typ_CVBSP:
    case Typ_CVRBSP:
      // dbi = DL_get_dbi (*dli);
      // GR_DrawCirc (dli, dbi, att, (Circ*)obj);
      GR_temp_ocv (form, obj, dbi, att);
      break;
/*
    //================================================================
    case Typ_CI2:
      // dbi = DL_get_dbi (*dli);
      GR_tDyn_ci2 (ind, dbi, att, (Circ2*)obj);
      break;

//     //================================================================
//     case Typ_CVELL:
//       GR_DrawCvEll (ind, dbi, att, obj);  // see GR_tDyn_ell GR_DrawCvEll
//       break;

    //================================================================
    case Typ_CVELL2:
      GR_DrawCvEll2 (ind, dbi, att, obj);  // see GR_tDyn_ell GR_DrawCvEll
      break;

//     //================================================================
//     case Typ_CVPOL:     // CurvPoly
//       GR_DrawCvPol (ind, dbi, att, obj); // GR_Disp_pol GR_DrawCvPol
//       break;

    //================================================================
    case Typ_CVPOL2:
      GR_Draw_pol2 (ind, att, obj); // GR_Disp_pol GR_DrawCvPol
      break;

//     //================================================================
//     case Typ_CVBSP:
//       dbi = DL_get_dbi (*dli);
//       // if(*dli > 0) { DL_dlRec__dli (&att1, *dli); dbi = att1->ind;
//       // else dbi = 0L;
//       GR_DrawCvBSp (ind, dbi, att, obj); // CurvBSpl
//       break;

//     //================================================================
//     case Typ_CVRBSP:
//       // dbi = DL_get_dbi (*dli);
//       GR_DrawCvRBSp (ind, dbi, att, obj); // CurvRBSpl
//       break;

    //================================================================
    case Typ_CVBSP2:
      // dbi = DL_get_dbi (*dli);
      GR_tDyn_bsp2 (ind, dbi, att, obj); // CurvBSpl
      break;
*/


    //================================================================
    case Typ_CVTRM:
      GR_set_ccv (OPERS_TEMP+OPERS_CLOSE, obj, oNr, dbi, att);
      break;

    //================================================================
    case Typ_PLN:
      // GR_DrawPlane
      // GL_DrawSymVX (ind, att, obj, mode, 1.);
      GR_temp_pln ((Plane*)obj, att,  mode);
      break;

    //================================================================
    case Typ_SURRV:
      // TSU_DrawSurTRV sollte noch in Disp & Draw geteilt werden !
      TSU_DrawSurTRV (ind, att, -1L);
      break;

    //================================================================
    case Typ_SURBSP:
      // nur Kontrollpunkte:
      // sbsp = (SurBSpl*)ox1->data;
      // GR_Disp_sbsp (sbsp->ptUNr, sbsp->ptVNr, sbsp->cpTab, att); // SurBSpl
      // TSU_DrawSurTBSP sollte noch in Disp & Draw geteilt werden !
      TSU_DrawSurTBSP ((SurBSpl*)obj, att, -1L);
      break;
/*
    //================================================================
    case Typ_ObjTab:     // binary-objects-table
      GR_temp_otb (ind, att, obj); // ObjTab
      break;


    //================================================================
    // case Typ_Index:
      // dbi = OGX_GET_INT (ox1);
      // o2 = DB_GetObjGX (ox1->typ, dbi);
      // GR_Draw_ox (dli, &o2, att, mode);
      // break;


    //================================================================
    case Typ_ObjGX:
      return GR_Draw_ox (ind, dbi, obj, att, mode);
//
//    // typ = AP_typDB_typ (typ);
//  case Typ_CV:
//    ox2 = (ObjGX*)obj;
//
//    //----------------------------------------------------------------
//    if(typ == Typ_SUR) {
//      // SUR_Disp_Sur (ox1, wrkSpc, att, p1);
//      TX_Print("**** GR_temp_nobj I1");
//
//    //----------------------------------------------------------------
//    } else {
//      // Typ_CV
//      for(i1=0; i1 < ox2->siz; ++i1) // Recursion !
//        // GR_Draw_ox (ind, &ox2[i1], att, mode);
//        // recurse
//        GR_temp_nobj (ind, ox2[i1].form,  ox2[i1].data, 1, att, mode);
//    }
//    break;
*/


    //================================================================
    default:
      goto L_E001;
  }

  // // save wrkSpc
  // if(wrkSpc) memstart = wrkSpc->next;
  // // release work space
  // if(wrkSpc) wrkSpc->next = memstart;

  return 0;


  L_E001:
      TX_Error("GR_temp_nobj not yet impl: %d",form);
      return -1;

  L_E002:
      TX_Error("GR_temp_nobj E002"); // par fehlt ..
      return -1;

  // L_E003:
      // TX_Error("GR_Draw_ox E003"); // wrkSpc fehlt ..
      // return -1;

}


//================================================================
  int GR_temp_otb (long *dli, int att, ObjTab *otb1) {
//================================================================
// GR_temp_otb           disp binary-object-table
// does not support not-connected-objects !

  int    irc, i1, pnr, paSiz;
  double tol;
  Point  *pta;


  // printf("GR_temp_otb att=%d oNr=%d\n",att,otb1->oNr);
  // DEB_dump_obj__ (Typ_ObjTab, otb1, "GR_temp_otb-1");


  // get stack-space for points
  pta = MEM_alloc_tmp (SPC_MAX_STK);
  paSiz = SPC_MAX_STK / sizeof(Point);
  pnr = 0;
  tol = UT_DISP_cv;
    // printf("  paSiz = %d points tol=%f\n",paSiz,tol);


  // make a polygon from otb1 and disp objects
  irc = UT3D_pta_otb (&pnr, pta, otb1, paSiz, tol);
  if(irc < 0) return -1; 

  GR_temp_pcv__ (pta, pnr, Typ_Att_Symb);

//   // open GL mode=0(curves)
//   GL_Draw_Ini1 (dli, att, 0);
//   // glBegin(GL_LINE_STRIP);..glEnd();
//   GL_set_pcv (pnr, pta, 1);
//   // close GL
//   GL_EndList1 (0);

  return 0;

}

 
//==============================================================================
  int GR_Disp_iSur (int iAtt,
                    MemTab(IndTab) *fmt, MemTab(int) *imt, MemTab(Point) *pmt) {
//==============================================================================
// GR_Disp_iSur    display tesselated faces (using fmt pmt imt)

  long dli;

  dli = DL_StoreObj (Typ_SUR, -1L, iAtt);

  return GR_Draw_nipatch (&dli, iAtt, fmt, imt, pmt);

}


//================================================================
  int GR_Draw_nipatch (long *dli, int iAtt,
                    MemTab(IndTab) *fmt,
                    MemTab(int)    *imt,
                    MemTab(Point)  *pmt) {
//================================================================
// GR_Draw_nipatch    display tesselated faces (using fmt pmt imt)
//  Input:
//    iAtt       index into GL_col_tab
//    fmt        tesselated-faces; ibeg=startIndex in imt;
//               .ibeg is startindex in imt, iNr= nr of links in imt;
//               .aux=triangle-typ
//    imt        indextable to points
//    pmt        points

  int       i1, fNr, *ia, i2, ii;
  IndTab    *it;
  Point     *pa;


  fNr = MEMTAB_IND (fmt);

  printf("GR_Draw_nipatch %d\n",fNr);
  // DEB_dump_obj__ (Typ_Color, &iAtt, "sur-iAtt");


  // init GL-dispList
  // GL_Surf_Ini (dli, NULL); //(void*)&iatt);  // glNewList (for gTyp=4,5,6,9)
//   GL_Surf_Ini (dli, (void*)&iAtt);  // glNewList (for gTyp=4,5,6,9)


  GL_ColSet ((ColRGB*)&iAtt);

/*
  // indexed color or true color ?
  if(((ColRGB*)&iAtt)->color == 0) {
    // default-color or indexed color
    // GL_Col__ (iAtt);
    glColor3ubv ((unsigned char*)&AP_defcol);

  } else {
    // real color
    glColor3ubv ((unsigned char*)&iAtt);
  }
*/


  pa = MEMTAB_DAT (pmt);

  for(i1=0; i1<fNr; ++i1) {

    // get next fmt-record
    it = MEMTAB__ (fmt, i1);

    // get imt-list of links
    ia = MEMTAB__ (imt, it->ibeg);

      // for(i2=0; i2<it->iNr; ++i2) { ii = ia[i2];
      // printf(" i[%d]=%d p[%d]=%lf,%lf,%lf\n",i2,ii,ii,
        // pa[ii].x, pa[ii].y, pa[ii].z); }
     
      // for(i2=0; i2<it->iNr; ++i2) printf(" p[%d] %lf %lf %lf\n",i2,
                                  // pa[i2].x, pa[i2].y, pa[i2].z);

    // display triangles; col: ind into GL_col_tab
    GL_set_ipatch (it->aux, it->iNr, ia, pa);

  }

  // close GL-dispList
//   GL_EndList ();                  // glEndList ();

  return 0;

}


//==============================================================================
  int GR_pt_par_sel_npt (Point *pts, int *ipt, double *pa1,
                         Point *pta, int ptNr, Point *ptAct) {
//==============================================================================
// GR_pt_par_sel_npt      get selectionpoint and parameter on polygon
// do test in 2D.
//
// Input:
//   ptAct        testpoint in userCoords
//   pta          polygon (line-segments)
//   ptNr         nr of points in pta
// Output:
//   pts          selectionpoint in usercoords (on line pt1-pt2)
//   ipt          index of selected segment; 0 = first; see retCod
//   pa1          parameter of pts on line-segment, see retCod
//   retCod       0=OK, point pta[ipt] selected
//                1=OK, point between pta[ipt] and pta[ipt+1]; offset is pa1
//               -1=ptAct_not_on_polygon
 

// see UT3D_segpar_sel_LN GL_ck_sel_PT GL_selPt

  int        irc, i0, ii, i1;
  double     tol, dx, dy, dp, dMax, p1;
  Point      p2Act, *p2a;
  Point2     pb1, pb2, p2s;


  // printf("----------------- GR_pt_par_sel_npt %d\n",ptNr);
  // for(i1=0; i1<ptNr; ++i1) DEB_dump_obj__ (Typ_PT, &pta[i1],"pta[%d]",i1);
  // DEB_dump_obj__ (Typ_PT, ptAct, " ptAct");

  tol = GL_pickSiz;
    // printf(" tol=%f\n",tol);


  // get memspc for p2a = ptNr 2D-points
  p2a = (Point*)MEM_alloc_tmp ((int)(sizeof(Point) * ptNr));
  if(!p2a) {printf(" GR_pt_par_sel_npt-EOM\n"); return -99;}


  // get ScreenCoords of ptAct
  GL_ptSc_ptUc (&p2Act, ptAct);
    // DEB_dump_obj__ (Typ_PT, &p2Act, " p2Act");


  //----------------------------------------------------------------
  // test if p2Act == cornerpoint of polygon 
  // loop tru points, get 2D-coords pf point, test if 2D-point == ptAct-2D
  dMax = tol;
  ii = -1;
  for(i1=0; i1<ptNr; ++i1) {
    // get ScreenCoords 
    GL_ptSc_ptUc (&p2a[i1], &pta[i1]);
      // DEB_dump_obj__ (Typ_PT, &p2a[i1], " p2a[%d]",i1);
      // GR_tDyn_symB__ (&pta[i1], SYM_TRI_S, ATT_COL_YELLOW);

    // get approx distance 
    dp = UT2D_lenB_2pt ((Point2*)&p2a[i1],(Point2*)&p2Act);
    if(dp < dMax) {
      // yes, found; keep index of nearest point
      ii = i1;
      dMax = dp;
    }
      // printf(" CK-ON-i1=%d dp=%f ii=%d\n",i1,dp,ii);
  }

  // select nearest point
  if(ii >= 0) {
    *pts = pta[ii];
    *ipt = ii;
    *pa1 = 0.;
    irc  = 0;
    goto L_exit;
  }


  //----------------------------------------------------------------
  // test if p2Act is on segment of polygon
  // loop tru segments
  dMax = tol;
  for(i1=1; i1<ptNr; ++i1) {
    i0 = i1 - 1;
      // printf(" CK-IN-i0=%d i1=%d\n",i0,i1);
    // test if actPos is inside 2D-box; no: continue
    UT2D_box_2pt__ (&pb1, &pb2, &p2a[i0], &p2a[i1]);
    if(UT2D_pt_ck_inBoxTol(&pb1, &pb2, (Point2*)&p2Act, tol)) continue;

    // compute lTot and lAct from longer side (dx or dy)
    // - parameter = ld/lTot
    dx = fabs(pb2.x - pb1.x);
    dy = fabs(pb2.y - pb1.y);
    if(dx > dy) {
      dp = fabs(p2Act.x - p2a[i0].x);
      *pa1 = dp / dx;
        // printf(" ..dx=%lf dp=%lf pa1=%lf i0=%d\n",dx,dp,*pa1,i0);

    } else {
      dp = fabs(p2Act.y - p2a[i0].y);
      *pa1 = dp / dy;
        // printf(" ..dy=%lf dp=%lf pa1=%lf i0=%d\n",dy,dp,*pa1,i0);
    }
    // compute 2D-point from parameter
    UT2D_pt_tra_pt_pt_par  (&p2s, (Point2*)&p2a[i0], (Point2*)&p2a[i1], *pa1);

    // test dist pts - p2Act
    dp = UT2D_lenB_2pt (&p2s, (Point2*)&p2Act);
      // printf(" i1=%d dp=%f\n",i1,dp);

    if(dp < dMax) {
      // yes, found; keep index of nearest point
      ii = i0;
      dMax = dp;
      p1 = *pa1;
    }
  }


  // select nearest point
  L_select:
  if(ii >= 0) {
      // printf(" sel-par=%f ii=%d\n",p1,ii);
    *ipt = ii;
    *pa1 = p1;
    // compute 3D-point from parameter
    UT3D_pt_evpar2pt (pts, *pa1, &pta[ii], &pta[ii + 1]);
    irc  = 1;
    goto L_exit;
  }


  irc = -1;


  //----------------------------------------------------------------
  L_exit:

    // printf("ex _sel_npt irc=%d ipt=%d pa1=%lf\n",irc,*ipt,*pa1);
    // DEB_dump_obj__ (Typ_PT, pts, " pts:");
    // if(irc >= 0) GR_tDyn_symB__ (pts, SYM_STAR_S, ATT_COL_RED);

  return irc;

}


//================================================================
//================================================================
// display Temporary-Objects
//================================================================
//================================================================
 

//===================================================================
  int GR_temp_message (int mode) {
//===================================================================
/// \code
/// Display messages with GR_temp_xxx or not.
/// 0=OFF=Aus, 1=ON=Ein.
/// \endcode

  tmpDspMode = mode;

  return 0;

}


//===================================================================
  int GR_temp_txiA (Point *pt1, int ii, int att) {
//===================================================================
// GR_temp_txiA       display number at position
//  Input:
//   DL_temp_ind - set fixed recordIndex; else get next free; see INF_GR_RECORDS
//   ii       nr to display
//   att      colorIndex; eg ATT_COL_RED; see INF_COL_SYMB
//
// see also GR_tDyn_txiA GR_temp_txi2 GR_temp_npti

  char   cbuf[40];


  snprintf(cbuf, sizeof(cbuf), "%d", ii);

  return GR_temp_txtA (pt1, cbuf, att);

}


//===================================================================
  int GR_tDyn_txiA (Point *pt1, int ii, int att) {
//===================================================================
// GR_tDyn_txiA       display number at position
//  Input:
//   ii       nr to display
//   att      colorIndex; eg ATT_COL_RED; see INF_COL_SYMB
//
// see also GR_tDyn_txiA GR_temp_txi2 GR_temp_npti

  char   cbuf[40];


  snprintf(cbuf, sizeof(cbuf), "%d", ii);

  return GR_tDyn_txtA (pt1, cbuf, att);

}


//===================================================================
  int GR_temp_tx2A (Point *pt1, char *txtA, int att) {
//===================================================================
// GR_temp_tx2A       display text at 2D-position
//  Input:
//   DL_temp_ind - set fixed recordIndex; else get next free; see INF_GR_RECORDS
//   txtA     text to display
//   att      colorIndex; eg ATT_COL_RED; see INF_COL_SYMB
//
// see also GR_tDyn_txiA GR_temp_txi2 GR_temp_npti

  Point  pt3;


  GR_pt3_pt2 (&pt3, pt1);

  return GR_temp_txtA (&pt3, txtA, att);

}


//===================================================================
  int GR_tDyn_tx2A (Point2 *pt1, char *txtA, int att) {
//===================================================================
// GR_tDyn_tx2A       display text at 2D-position
//  Input:
//   txtA     text to display
//   att      colorIndex; eg ATT_COL_RED; see INF_COL_SYMB
//
// see also GR_tDyn_txiA GR_temp_txi2 GR_temp_npti

  Point  pt3;


  GR_pt3_pt2 (&pt3, pt1);

  return GR_tDyn_txtA (&pt3, txtA, att);

}


//===================================================================
  int GR_temp_txi2 (Point2 *pt1, int ii, int att) {
//===================================================================
// GR_temp_txi2       display number at 2D-position
//  Input:
//   DL_temp_ind - set fixed recordIndex; else get next free; see INF_GR_RECORDS
//   ii       nr to display
//   att      colorIndex; eg ATT_COL_RED; see INF_COL_SYMB
//
// see also GR_tDyn_txiA GR_temp_txi2 GR_temp_npti

  char   cbuf[40];
  Point  pt3;

  snprintf(cbuf, sizeof(cbuf), "%d", ii);

  GR_pt3_pt2 (&pt3, pt1);

  return GR_temp_txtA (&pt3, cbuf, att);

}


/*
//===================================================================
  int GR_perm_pt (long dbi, Point *pt1, int att) {
//===================================================================
// GR_tDyn_symB__          temp.dyn disp BITMAP-Symbol (SYM_TRI_S SYM_STAR_S ..)
// Input:
//   ind      fixed index: 1 - (DL_base_font1 - 1)
//            0    get next free index
//   typ:  SYM_STAR_S  star
//         SYM_TRI_S   triangle
//         SYM_CIR_S   circle small
//         SYM_SQU_B   square
//   att:  eg. ATT_PT_GREEN                see INF_COL_PT

/// see also GR_temp_pTab

  printf("GR_perm_pt dbi=%ld att=%d\n",dbi,att);


  // if(tmpDspMode) DEB_dump_obj__(Typ_PT, pt1, "GR_tDyn_symB__:");

  // get gli, create DL-record
  // DL_tDyn_init (att);
  DL_perm_init (Typ_PT, dbi, att);

  GL_att_pt (att);   // set color

  GL_set_npt (pt1, 1);

  GL_list_close (); // close GL-record

  return 0;

}
*/



//============================================================================
  int GR_tDyn_symV2 (int typ, int att, Point *p1, Vector *v1, double scale) {
//============================================================================
// GR_tDyn_symV2        disp vector-symbol, rotated (in XY-plane, 2D)
//   typ       symtyp; SYM_AXIS|SYM_SQUARE|SYM_CROSS ..   see GR_tDyn_symV3
//   att       color of symbols            see INF_COL_SYMB
//   v1        for rotation in XY-plane only; can be NULL

  double    ad;
  Vector2   vcxy;

  DL_tDyn_init (att);

  GL_att_sym (att);   // set color

  //----------------------------------------------------------------
  if(v1) {
    // 2D-Vector aus Draufsicht auf X-Y
    vcxy.dx = v1->dx;
    vcxy.dy = v1->dy;

    // 2D-Vector normieren
    UT2D_vc_setLength (&vcxy, &vcxy, 1.);
    // printf("vcxy=%f,%f\n",vcxy.dx,vcxy.dy);

    ad = ACOS(vcxy.dx); // angle deg
    ad = UT_DEGREES(ad);


  } else ad = 0.;

  //----------------------------------------------------------------
  GL_set_symV_r (typ, p1, ad, scale);

  GL_list_close (); // close GL-record

  return 0;

}


//============================================================================
  int GR_set_symV3 (int opers, int typ, int att,
                     Point *p1, Vector *v1, double scale) {
//============================================================================
// GR_set_symV3        disp vector-symbol, oriented (3D, rotate + tilt)
//   all symbols except SYM_VEC and SYM_LENGTH scale back (have always same length)
// Input:
//   opers     object-persistence; temp|tDyn;   see INF_OPERS
//   typ       symtyp; SYM_TRIANG, SYM_AXIS, SYM_SQUARE, SYM_CROSS ..
//     SYM_ARROW     vector normalized
//     SYM_SQUARE    draw rectangle normal to vc1
//     SYM_PLANE     draw filled rectangle normal to vc1
//     SYM_AXIS1     draw x,y,z.axis normal to vc1
//     SYM_AXIS      draw x,y,z.axis with x,y-characters normal to vc1
//     SYM_CROSS     draw cross (X) normal to vc1
//     SYM_CROSS1    draw symbolic scissor normal to vc1
//     SYM_TRIANG    draw triangle normal to vc1
//     SYM_ARROH     draw 2D-arrowhead-only in the x-y-plane
//     SYM_ARRO3H    draw 3D-arrowhead-only along vc1
//     SYM_VEC       vector true-length
//     SYM_LENGTH    line with true-lenght
//   att       color of symbols            see INF_COL_SYMB
//   p1        position, can be NULL (use screen-center)
//   v1        orientation

  double  ay, az;
  // Point   ptc;


  // printf("GR_set_symV3 opers=%d typ=%d att=%d scale=%f\n",opers,typ,att,scale);
  // printf("  GL_SclNorm=%f GL_Scale=%f\n",GL_SclNorm,GL_Scale);


  //----------------------------------------------------------------
  // tdyn
  if(opers & OPERS_TDYN) {
    DL_tDyn_init (att);
    GL_att_sym (att);   // set color

  // temp
  } else if(opers & OPERS_TEMP) {
    DL_temp_init ();
    GL_att_sym (att);   // set color
  }


  //----------------------------------------------------------------
  GL_set_symV3 (typ, p1, v1, scale);


  //----------------------------------------------------------------
  if(opers & OPERS_CLOSE) GL_list_close (); // close GL-record

  return 0;

}


//===================================================================
  int GR_tDyn_symB2__ (Point2 *pt1, int typ, int att) {
//===================================================================
// GR_tDyn_symB2__          temp.dyn disp 2D-BITMAP-Symbol
// Input:
//   ind      fixed index: 1 - (DL_base_font1 - 1) DL_temp_ind
//            0    get next free index
//   typ:  SYM_STAR_S  star                      see INF_BITMAP_SYMBOLS
//         SYM_TRI_S   triangle
//         SYM_CIR_S   circle small
//         SYM_SQU_B   square
//   att:  colorIndex; eg ATT_COL_RED;           see INF_COL_SYMB

/// see also GR_temp_pTab

  Point  pt3;

  if(tmpDspMode) DEB_dump_obj__(Typ_PT2, pt1, "GR_tDyn_symB2__:");

  GR_pt3_pt2 (&pt3, pt1);

  return GR_tDyn_symB__ (&pt3, typ, att);

}


//===================================================================
  int GR_tDyn_symB__ (Point *pt1, int typ, int att) {
//===================================================================
// GR_tDyn_symB__          temp.dyn disp BITMAP-Symbol (SYM_TRI_S SYM_STAR_S ..)
// Input:
//   ind      fixed index: 1 - (DL_base_font1 - 1) DL_temp_ind
//            0    get next free index
//   typ:  SYM_STAR_S  star                      see INF_BITMAP_SYMBOLS
//         SYM_TRI_S   triangle
//         SYM_CIR_S   circle small
//         SYM_SQU_B   square
//   att:  colorIndex; eg ATT_COL_RED;           see INF_COL_SYMB

/// see also GR_temp_pTab


  if(tmpDspMode) 
  DEB_dump_obj__(Typ_PT, pt1, "GR_tDyn_symB__:");

  // get gli and open new GL-list
  DL_tDyn_init (att);

  GL_att_sym (att);   // set color

  GL_set_symB (typ, pt1);

  GL_list_close (); // close GL-record

    // printf(" ex-GR_tDyn_symB__\n");

  return 0;

}


//===================================================================
  int GR_temp_symB (Point *pt1, int typ, int att) {
//===================================================================
// GR_temp_symB            Temp.display 3D-Point
// alias GR_temp_pt
// Input:
//   ind      fixed index: 1 - (DL_base_font1 - 1) DL_temp_ind
//            0    get next free index
//   typ:  SYM_STAR_S  star                      see INF_BITMAP_SYMBOLS
//         SYM_TRI_S   triangle
//         SYM_CIR_S   circle small
//         SYM_SQU_B   square
//   att:  colorIndex; eg ATT_COL_RED;           see INF_COL_SYMB

/// see also GR_temp_pTab


  if(tmpDspMode) DEB_dump_obj__(Typ_PT, pt1, "GR_temp_symB:");

  // get gli
  DL_temp_init ();

  GL_att_sym (att);   // set color

  GL_set_symB (typ, pt1);

  GL_list_close (); // close GL-record

  return 0;

}


//================================================================
  int GR_set_mdr (int opers, ModelRef *mdr, long dbi, int att) {
//================================================================
// GR_set_mdr             disp subModel from modelreference
// - import mockup-models (obj stl wrl)
// - native models must already be loaded in dispList
// Input:
//   att                  GR_TMP_DEF|GR_TMP_HILI|GR_TMP_DIM;
// Output:
//   retCode    0=OK, -1=Err

  int        irc, mbi;
  long       gli;
  ModelBas   *mdb;


  // printf("GR_set_mdr opers=%d dbi=%ld att=%d ----------\n",opers,dbi,att);
  // DEB_dump_obj__ (Typ_Model, mdr, "GR_set_mdr");

  // get basicModel
  mbi = mdr->modNr;
  mdb = DB_get_ModBas (mbi);
  if(!mdb) {
    TX_Print ("***** GR_set_mdr basMod %d not exists ...",mdr->modNr);
    irc = -1;
    goto L_exit;
  }
    // DEB_dump_obj__ (Typ_SubModel, mdb, "  set_mdb-mdb: ");



  //----------------------------------------------------------------
  // test if its a mock
  if((mdb->typ >= Mtyp_TESS)&&(mdb->typ < Mtyp_BMP)) {
    // is mockp-model
    // test if tess-file already loaded and stored in GL (as Typ_Ditto);
    irc = GR_mdMock_imp (mdb, (long)mbi);
    if(irc < 0) goto L_exit;
  }


  //----------------------------------------------------------------
  // perm
  if(opers & OPERS_PERM) {
    DL_perm_init (Typ_Model, (long)dbi, att); // ModelReference
    DL_att_mdr (AP_dli_act, att);             // sel hili|dim

  // tdyn
  } else if(opers & OPERS_TDYN) {
    DL_tDyn_init (att);
    DL_att_mdr (AP_dli_act, att);     // sel hili|dim


  // temp
  } else if(opers & OPERS_TEMP) {
     gli = DL_temp_init ();
     DL_att_temp (gli, att);
  }

  // display native models and mockups
  GL_set_mdr (mdr, mdb);

  if(opers & OPERS_CLOSE) GL_list_close (); // close GL-record

  irc = 0;


  //----------------------------------------------------------------
  L_exit:

    // MDL_dump__ ("ex-GR_set_mdr");
    // DL_DumpObjTab ("ex-GR_set_mdr");
    // printf(" ex-GR_set_mdr %d\n",irc);

  return irc;

}


//================================================================
 int GR_mdMock_imp (ModelBas *mdb, long dbi) {
//================================================================
// import VRML|OBJ|STL-file - save as tmp/Data_<model>.tess
//   retCode   0=OK
// - import model -> tess-file <tmp>/Data_<moldel>.tess
// - load tess-file into memory-space impSpc
// - load impSpc into dispList DL and GL
//
// was GR_DrawModel

  int        irc, loadMode, ift;
  long       dli;
  char       ffnam[256], tessFn[256], safNm[128], ftyp[32], *p1, dllnam[40];
  ObjGX      oTab[4];
  Memspc     impSpc;
  Point      pb1, pb2;



  // printf("GR_mdMock_imp ------------ dbi=%ld\n",dbi);
  // printf(" AP_stat.mdl_stat=%d\n",AP_stat.mdl_stat);
  // DEB_dump_obj__ (Typ_SubModel, mdb, "  -mdb");


  //================================================================
  // test if tess-file already loaded and stored in GL (as Typ_Ditto);
  if(mdb->DLsiz > 0) goto L_exit;

  // "Redraw all" can have removed/overwritten the DL/GL-record; compare typ,dbi
//   irc = DL_ck_typ_dbi (mdb->DLind, Typ_Ditto, mdb->DBind); // 0=yes, 1=no
//     printf(" _mdMock_imp-DL_ck_typ_dbi-irc = %d\n",irc);
//   if(!irc) goto L_exit;



  //================================================================

  // fix a safe modelname
  strcpy (safNm, mdb->mnam);
  // safNm ist internal ("abc") od external ("dir/fn")
  // UTX_ftyp_cut (mnam);            // remove fTyp
  UTX_safeName (safNm, 1);         // change all '. ' to '_', not '/'
    // printf(" _mdMock_imp safNm=|%s|\n",safNm);

  // set tessFn = filename tess-file (tmp/Model_
  sprintf(tessFn, "%sMod_%s.tess",AP_get_tmp_dir(),safNm);
    // printf(" _mdMock_imp tessFn |%s|\n",tessFn);

  // set ffnam = full filename of input (mockup-model)
  MDLFN_ffNam_syFn (ffnam, mdb->mnam);
    // printf(" _mdMock_imp-ffnam=|%s|\n",ffnam);



  //================================================================
  // set dllnam
  switch(mdb->typ) {
    case Mtyp_TESS:
      goto L_load_tess;

    case Mtyp_WRL:
      // // get VR-Version of file: V1 or V2
      ift = AP_ImportWRL_ckTyp (ffnam);
        // printf(" _mdMock_imp-wrl-ift=%d\n",ift);
      if(ift == Mtyp_WRL) {
        strcpy(dllnam, "xa_wrl_r");
      } else if(ift == Mtyp_WRL2) {
        // strcpy(dllnam, "xa_vr2_r"); break;
        TX_Error("***** VR-Version 2 not supported for this version");
        irc = -1;
        // on startup clear all
        if(!AP_stat.mdl_load) {
          AP_mdl_init (0);
          MDL_load_new__ ();
          goto L_exit;
        }
        // only in MAN already registered in mdl_tab; delete;
        MDL_mdl_del (mdb->mnam);
        goto L_exit;
      }
      break;

    case Mtyp_OBJ:
      strcpy(dllnam, "xa_obj_r");
      break;

    case Mtyp_STL:
      strcpy(dllnam, "xa_stl_r");
      break;

    default:
      TX_Print("***** GR_mdMock_imp E1 typ %d",mdb->typ); irc = -1; goto L_exit;
  }
    // printf(" _mdMock_imp-dllnam=|%s|\n",dllnam);


  //================================================================
  L_imp_tess:
  // get (import) tess-file (write file <tmp>/Data_<moldel>.tess)
  //   (and keep tess-file in impSpc)
    // Die Parametertabelle oTab fuer die DLL generieren.
    UTO_rec_set (&oTab[0], Typ_ObjGX,  Typ_ObjGX,  3, &oTab[1]);
    UTO_rec_set (&oTab[1], Typ_Int4,   Typ_Int4,   1, PTR_INT(1));  // mode
    UTO_rec_set (&oTab[2], Typ_Txt,    Typ_Txt,    1, ffnam);
    UTO_rec_set (&oTab[3], Typ_Memspc, Typ_Memspc, 1, &impSpc);

    // Load DLL; filename = xa_<fileType>_r.so
    irc = DLL_run1 (0, dllnam);
    if(irc < 0) return irc;

    // load Model --> impSpc
    irc = DLL_run1 (2, oTab);
    if(irc < 0) goto L_exit;


    //----------------------------------------------------------------
//     // VR2 (over)writes Model_<mnam>; 
//     if(ift == Mtyp_WRL2) {
//       goto L_exit;
//     }
   

    //----------------------------------------------------------------
    // wrl obj stl makes tesselated data in impSpc

    // find box of tess-model in memory
    tess_box_get (&pb1, &pb2, &impSpc);
       // DEB_dump_obj__ (Typ_PT, &pb1, "pb1");
       // DEB_dump_obj__ (Typ_PT, &pb2, "pb2");

    // set box in basicModel mdb
    mdb->pb1 = pb1;
    mdb->pb2 = pb2;

    // save tesselated data in xa/temp for next run
    tess_write__ (tessFn, impSpc.start);

  // load impSpc into to DL/GL
  goto L_load_DL;



  //================================================================
  L_load_tess:
  // load tess-file into impSpc

  // test if file <tmp>/Data_<model>.tess exist
  irc = OS_checkFilExist (ffnam, 1);  //0=no, 1=yes.
  if(!irc) {TX_Print("***** GR_mdMock_imp E2 %s",ffnam); irc = -1; goto L_exit;}

  // copy file ffnam -> tessFn
  OS_file_copy (ffnam, tessFn);

  // and import model as mockup into impSpc
  irc = TSU_imp_tess (&impSpc, tessFn);
//   irc = tess_read__ (&impSpc, tessFn);   // 0=OK, exists ..
  if(irc < 0) {irc = -2; goto L_exit;}



  //================================================================
  L_load_DL:
  // load tess-data from impSpc into to DL/GL
  // dli = DL_StoreObj (Typ_Ditto, 0L, 0);
  DL_perm_init (Typ_Ditto, dbi, 0);
  dli = AP_dli_act;
    // printf(" mock_imp-dli=%ld\n",dli);
  DL_unvis_set (dli, 1);         // make Basemodel unvisible

  // load all records of impSpc into GL-list
  TSU_DrawSurTess (impSpc.start);

  GL_list_close ();

  UME_free (&impSpc);

  // update basic-model; now complete.
//   mdb->DBind = dbi;
  mdb->DLind = dli;
  mdb->DLsiz = 1;
  
  irc = 0;


  //================================================================
  L_exit:

      // MDL_dump__ (" ex-GR_mdMock_imp");
      // printf(" ex-GR_mdMock_imp %d\n",irc);

    return irc;

}


//======================================================================
  int GR_tDyn_vc2__ (Vector2 *vc1, Point2 *pt1, int att, int mode) {
//======================================================================
// GR_tDyn_vc2__            temp.Dyn 2D-Vector; length true or normalized
// Input:
//   pt1       position fo vector; NULL = display at center of screen
//   att       color,linetyp,thickness; see INF_COL_SYMB
//   mode      0  display normalized length of vector
//             1  display real length of vector
//
// see also GR_temp_vc2
// see also IE_cad_inp_vcPos_i UI_prev_vc

  Point   pt2;
  Vector  vc2;


  UT3D_pt_pt2_z (&pt2, pt1, GL2D_Z);
  UT3D_vc_vc2 (&vc2, vc1);
  GR_tDyn_vc__ (&vc2, &pt2, att, mode);

  return 0;

}


//======================================================================
  int GR_tDyn_vc__ (Vector *vc1, Point *pt1, int att, int mode) {
//======================================================================
// GR_tDyn_vc__            temp.Dyn Vector; length true or normalized
// Input:
//   pt1       position fo vector; NULL = display at center of screen
//   att       color,linetyp,thickness; see INF_COL_SYMB
//   mode      0  display normalized length of vector
//             1  display real length of vector
//
// see also GR_temp_vc2

// see also IE_cad_inp_vcPos_i UI_prev_vc

  if(mode) {
    // disp true length
    GR_tDyn_symV3 (SYM_LENGTH, att, pt1, vc1, UT3D_len_vc(vc1));
    // GR_tDyn_symV3 (SYM_VEC, att, pt1, vc1, UT3D_len_vc(vc1));  // true
   
  } else {
    // normalized
    GR_tDyn_symV3 (SYM_ARROW, att, pt1, vc1, 12.);
    // GR_tDyn_symV3 (SYM_ARROW, att, pt1, vc1, 1.); // norm.
  }

  return 0;

}


//======================================================================
  int GR_temp_vc (Vector *vc1, Point *pt1, int att, int mode) {
//======================================================================
/// \code
/// GR_temp_vc     display 3D-Vector at position; length true or normalized
/// Input:
//   DL_temp_ind - set fixed recordIndex; else get next free; see INF_GR_RECORDS
///   pt1       position fo vector; NULL = display at center of screen
///   att       color,linetyp,thickness; see INF_COL_SYMB
///   mode      0  display normalized length of vector
///             1  display real length of vector
///
/// see also GR_temp_vc2 GR_set_obj GR_tDyn_vc__
/// \endcode

// see also IE_cad_inp_vcPos_i UI_prev_vc


  // printf("GR_temp_vc %d\n",mode);
// TX_Print("**** GR_temp_vc temp OFF .."); return 0;

  if(mode) {
    // disp true length
    GR_temp_symV3 (SYM_LENGTH, att, pt1, vc1, UT3D_len_vc(vc1));
  } else {
    // normalized
    GR_temp_symV3 (SYM_ARROW, att, pt1, vc1, 12.);
  }

  return 0;

}


//================================================================
  int GR_temp_traRot (TraRot *tra,  int att) {
//================================================================

  double    a1;
  Point     pt1;
  Vector    vx, vz;


  printf("GR_temp_traRot %d\n",att);
  DEB_dump_obj__ (Typ_TraRot, tra, "TraRot:");


  DL_temp_init ();

  // a1 = angle
  // Typ_TraRot  ((TraRot*)tra->data)
  a1 = tra->angr * -1.;        // umdrehen ! ?

  // get pt2 = origin of matrix
  UT3D_pt_m3 (&pt1, tra->ma);

  // get axis
  vz = tra->vz;
  // UT3D_vc_setLength (&vc1, &vc1, 1.);
    // DEB_dump_obj__ (Typ_VC, &vc1, "vc1");

  // get vx from vz
  UT3D_vc_perp1vc (&vx, &vz);


  // draw
  GL_att_cv (att);

  GL_set_ang (att, &pt1, &vx, &vz, a1);

  GL_list_close (); // close GL-record


  return 0;

}

 
//====================================================================
  int GR_temp_ang (int att, Point *pt1, Vector *vx, Vector *vz, double ang1) {
//====================================================================
//   att       color,linetyp,thickness; see INF_COL_CV
// Input:
//   ang1     angle in rad; pos=CCW, neg=CW
//   vx       vector from center to startpt of angle - at o deg
//   vz       rotate vx around (center-vz) for endvector - at angle <ang1> deg
// //   vx,vz    def of plane (have 90 deg) vx = 0 deg;


// see GL_DrawAngA

  Point ptc;


  if(tmpDspMode) DEB_dump_obj__ (Typ_PT, pt1, "GR_temp_ang %lf",ang1);

  // printf("GR_temp_ang %lf\n",ang1);
  // GR_temp_pt (&pl1->po, SYM_TRI_S, 2);
  // GR_temp_vc (&pl1->vz, &pl1->po, 2, 0);

  DL_temp_init ();

  if(pt1) ptc = *pt1;
  else    ptc = GL_GetCen();   // ScreenCenter in UserCoords
    // DEB_dump_obj__ (Typ_PT, &ptc, " temp_vc-ptc");


  if(!vz) {
    vx = &WC_sur_act.vx;
    vz = &WC_sur_act.vz;
  }

  GL_att_cv (att);

  GL_set_ang (att, &ptc, vx, vz, ang1);

  GL_list_close (); // close GL-record

  return 0;

}


/*
//====================================================================
  int GR_temp_ln (Line *ln1, int att) {
//====================================================================
// GR_temp_ln            tempDisp line
//   att       color,linetyp,thickness; see INF_COL_CV
// see GR_Disp_ln

  Point   pta[2];

  if(tmpDspMode) DEB_dump_obj__ (Typ_LN, ln1, "GR_temp_ln");

  DL_temp_init ();

  GL_att_cv (att);

  pta[0] = ln1->p1;
  pta[1] = ln1->p2;

  GL_set_pcv (2, pta, 1); // GL_LINE_STRIP

  GL_list_close (); // close GL-record

  return 0;

}
*/

//====================================================================
  int GR_temp_ci (long dbi, Circ *cii, int att) {
//====================================================================
// GR_temp_ci            tempDisp circle
//   att       color,linetyp,thickness; see INF_COL_CV
// see APT_DrawCirc GR_DrawCirc

  return GR_temp_ocv (Typ_CI, cii, dbi, att);

}


//===============================================================================
  int GR_set_txtG (int opers, GText *txG, long dbi, int att) {
//===============================================================================
// GR_set_txtG           grafic text (rotated, scaled)
//   att      see INF_COL_SYMB

  // printf("TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT dbi=%ld att=%d\n",dbi,att);
  // if(tmpDspMode) DEB_dump_obj__ (Typ_GTXT, txG, "GR_set_txtG");

  // perm
  if(opers & OPERS_PERM) {
    DL_perm_init (Typ_GTXT, dbi, att);
    GL_att_cv (att);

  // tdyn
  } else if(opers & OPERS_TDYN) {
    DL_tDyn_init (att);
    GL_att_cv (att);

  // temp
  } else if(opers & OPERS_TEMP) {
    DL_temp_init ();
    GL_att_cv (att);
  }


  GL_set_txtG (txG);

  if(opers & OPERS_CLOSE) GL_list_close (); // close GL-record

  return 0;

}


//===============================================================================
  int GR_set_dimen (int opers, Dimen *dim1, long dbi, int att) {
//===============================================================================
// GR_set_dimen          hor/vert/parall. dimension

  if(tmpDspMode) DEB_dump_obj__ (Typ_Dimen, dim1, "GR_set_dimen");

  // perm
  if(opers & OPERS_PERM) {
    DL_perm_init (Typ_Dimen, dbi, att);
    GL_att_cv (att);

  // temp
  } else if(opers & OPERS_TEMP) {
    DL_temp_init ();
    GL_att_cv (att);
  }



  if(dim1->dtyp == 3)      GL_set_Dima   (att, dim1);
  else if(dim1->dtyp > 20) GL_set_Ldr    (att, dim1);    // 21
  else if(dim1->dtyp > 1)  GL_set_Dimrad (att, dim1);    // 2
  else if(dim1->dtyp > 0)  GL_set_Dimdia (att, dim1);    // 1
  else                     GL_set_Dimen  (att, dim1);    // 0

  if(opers & OPERS_CLOSE) GL_list_close (); // close GL-record

  return 0;

}


//===============================================================================
  int GR_set_dim3 (int opers, Dim3 *dim1, long dbi, int att) {
//===============================================================================
// GR_set_dim3           3D-dimension

  if(tmpDspMode) DEB_dump_obj__ (Typ_Dim3, dim1, "GR_set_Dim3");

  // perm
  if(opers & OPERS_PERM) {
    DL_perm_init (Typ_Dim3, dbi, att);
    GL_att_cv (att);

  // temp
  } else if(opers & OPERS_TEMP) {
    DL_temp_init ();
    GL_att_cv (att);
  }


  GL_set_Dim3 (dim1);

  if(opers & OPERS_CLOSE) GL_list_close (); // close GL-record

  return 0;

}


//===============================================================================
  int GR_set_txt__ (int opers, AText *txa, long dbi, int att) {
//===============================================================================
// GR_set_txt__         disp notes with 2D-text
//   att      see INF_COL_SYMB

  // if(tmpDspMode)
  // DEB_dump_obj__ (Typ_ATXT, txa, "GR_set_txt__ att=%d",att);


  // perm
  if(opers & OPERS_PERM) {
    DL_perm_init (Typ_ATXT, dbi, att);

  // temp
  } else if(opers & OPERS_TEMP) {
    DL_temp_init ();
  }

  if(opers > OPERS_CLOSE) {
    if(txa->col > 0) att = txa->col;
    GL_att_sym (att);
  }


  if(txa->aTyp == 1)      GL_set_BMP    (txa); // Image-BMP
  else if(txa->aTyp == 3) GL_set_TxtLBG (txa); // LeaderLine + Balloon + 3Dtxt LDRC
  else if(txa->aTyp == 4) GL_set_TxtLG  (txa); // LeaderLine + 3D-Text   LDRP
  else if(txa->aTyp > 4)  GL_set_TxtSym (txa); // symTyp > 4 sind Symbols
  else                    GL_set_Tag    (txa); // 2=Block-tag; 0=A-text

  if(opers & OPERS_CLOSE) GL_list_close (); // close GL-record

  return 0;

}


//====================================================================
  int GR_set_ocv (int opers, long dbi, int form, void *obj, int att) {
//====================================================================
// GR_set_ocv            disp all kinds of curves from binary-obj
// Input:
//   opers      object-persistence; eg OPERS_TEMP+OPERS_CLOSE  see INF_OPERS
//   form,obj   bin.obj of curve to display
//   dbi        DB-index (for use of PRCV) else 0
//   att        see INF_COL_CV


  int     irc;

  // printf("GR_set_ocv opers=%d form=%d dbi=%ld\n",opers,form,dbi);

  if(tmpDspMode) DEB_dump_obj__ (form, obj, "GR_set_ocv");

  // perm
  if(opers & OPERS_PERM) {
    DL_perm_init (form, dbi, att);

  // tdyn
  } else if(opers & OPERS_TDYN) {
    DL_tDyn_init (att);

  // temp
  } else if(opers & OPERS_TEMP) {
    DL_temp_init ();
  }

  // disp
  irc = GL_set_ocv (form, obj, dbi, att);

  L_exit:
  if(opers & OPERS_CLOSE) GL_list_close (); // close GL-record

  return irc;

}


//==================================================================
  int GR_temp_dbo (int ind,  int typ, long dbi, int att, int mode) {
//==================================================================
// GR_temp_dbo         tempDisp DB-obj
// Input:
//   ind      GL-index;
//   dbi      DB-index if known (used for getting PRCV)
//   att      Typ_PT:  see INF_COL_PT
//              ATT_PT_BLACK|ATT_PT_HILI|ATT_PT_DIMMED|ATT_PT_GREEN|ATT_PT_YELLOW
//            Typ_CI|Typ_LN|Typ_CV*|Typ_PLN:   see INF_COL_CV
//            Typ_SymB:   colorIndex; eg ATT_COL_RED; see INF_COL_SYMB
//              ATT_COL_BLACK|ATT_COL_RED|ATT_COL_GREEN|ATT_COL_BLUE|
//              ATT_COL_YELLOW|ATT_COL_MAGENTA|ATT_COL_CYAN|ATT_COL_WHITE
//              ATT_COL_HILI|ATT_COL_DIMMED
//   mode     Typ_PT|Typ_LN|Typ_CI|Typ_CV*: unused;
//            Typ_VC|Typ_VC2: 0=length-normalized; 1=true-length
//            Typ_SymB: SYM_TRI_S|SYM_STAR_S|SYM_CIR_S|SYM_SQU_B
//            Typ_PLN:  1=Plane; 2=Axis; 4=Axis+Chars; 5=Plane+Axis;
// 
// GR_Draw_dbo GR_tDyn_dbo

  int       form, oNr;
  void      *data;


  // printf("GR_temp_dbo ind=%d typ=%d dbi=%ld\n",ind,typ,dbi);

  DL_temp_ind = ind;   // overwrite or next ..

  form = UTO__dbo (&data, &oNr, typ, dbi);
  if(form < 1) return -1;
    // DEB_dump_obj__ (form, data, " _temp_dbo-L1");

  // return GR_temp_ocv (form, data, dbi, att);
  // return GR_tDyn_obj (form, data, att, mode);
  return GR_temp_nobj (ind, dbi, form, data, oNr, att, mode);

}


//================================================================
  int GR_tDyn_ox (ObjGX *ox1, int att, int mode) {
//================================================================
// GR_tDyn_ox            temp.Dyn complex-obj ObjGX

  int    irc, i1, iNr, oTyp, oForm;
  long   dbi;
  ObjGX  *ox2, o2;
  void   *oDat;


  // printf("GR_tDyn_ox att=%d mode=%d\n",att,mode);
  // DEB_dump_obj__ (Typ_ObjGX, ox1, "GR_Disp_ox-in");


  oForm = ox1->form;


  switch (oForm) {

    //================================================================
    case Typ_Index:
      dbi = OGX_GET_INT (ox1);
      oTyp = UTO__dbo (&oDat, &iNr, ox1->typ, dbi);
      GR_set_obj (OPERS_TEMP+OPERS_CLOSE, dbi, oTyp, oDat, att, mode);
      break;


    case Typ_ObjGX:
      ox2 = ox1->data;
      for(i1=0; i1 < ox1->siz; ++i1) // Recursion !
        GR_tDyn_ox (&ox2[i1], att, mode);
      break;


    // //================================================================
    default:
      // handle content of the ObjGX
      irc = GR_tDyn_nobj (oForm, ox1->data, ox1->siz, att, mode);
      if(irc < 0) return irc;
  }

  return 0;

}


//==================================================================
  int GR_tDyn_dbo (int typ, long dbi, int att, int mode) {
//==================================================================
// GR_temp_dbo         tempDisp DB-obj
// Input:
//   ind      GL-index;
//   dbi      DB-index if known (used for getting PRCV)
//   att      Typ_PT:  see INF_COL_PT
//              ATT_PT_BLACK|ATT_PT_HILI|ATT_PT_DIMMED|ATT_PT_GREEN|ATT_PT_YELLOW
//            Typ_CI|Typ_LN|Typ_CV*|Typ_PLN:   see INF_COL_CV
//            Typ_SymB:   colorIndex; eg ATT_COL_RED; see INF_COL_SYMB
//              ATT_COL_BLACK|ATT_COL_RED|ATT_COL_GREEN|ATT_COL_BLUE|
//              ATT_COL_YELLOW|ATT_COL_MAGENTA|ATT_COL_CYAN|ATT_COL_WHITE
//              ATT_COL_HILI|ATT_COL_DIMMED
//   mode     Typ_PT|Typ_LN|Typ_CI|Typ_CV*: unused;
//            Typ_VC|Typ_VC2: 0=length-normalized; 1=true-length
//            Typ_SymB: SYM_TRI_S|SYM_STAR_S|SYM_CIR_S|SYM_SQU_B
//            Typ_PLN:  1=Plane; 2=Axis; 4=Axis+Chars; 5=Plane+Axis;
// 
// GR_Draw_dbo GR_tDyn_dbo

  int       form, oNr;
  void      *data;


  // printf("GR_tDyn_dbo typ=%d dbi=%ld\n",typ,dbi);

  form = UTO__dbo (&data, &oNr, typ, dbi);
  if(form < 1) return -1;
    // DEB_dump_obj__ (form, data, " _Draw_dbo");

  if(oNr > 1) {
    return GR_tDyn_nobj (form, data, oNr, att, mode);

  } else {
    // return GR_temp_ocv (form, data, dbi, att);
    return GR_tDyn_obj (form, data, att, mode);
  }

}


/* -> BBX__pMsh_dbi
//================================================================
  int GR_cv_pMesh_box (Point *pb1, Point *pb2, long dbi) {
//================================================================
// GR_cv_pMesh_box          boundingBox for surface from points

  int           i1, typ, ptNr;
  MemTab(Point) pTab = _MEMTAB_NUL;


  // printf("GR_cv_pMesh_box %ld\n",dbi);

  MemTab_ini__ (&pTab, sizeof(Point), Typ_PT, 10000);

  // load pTab from bin.file; mallocs Ptab !
  i1 = MSH_bload_pTab (&pTab, AP_modact_nam, dbi);
  if(i1 < 0) return -1;
  ptNr = pTab.rNr;
    printf(" ptNr=%d\n",ptNr);

  // get bounding box <-- points
  i1 = UT3D_box_pts (pb1, pb2, ptNr, pTab.data);

  MemTab_free (&pTab);

    // printf("::::::::::::::::::::::::: ex GR_cv_pMesh_box: ::::::::::: \n");
    // DEB_dump_obj__ (Typ_PT, &pb1, "pb1");
    // DEB_dump_obj__ (Typ_PT, &pb2, "pb2");

  return 0;

}
*/

//================================================================
  int GR_disp_cv_pMesh (long dbi, int att) {
//================================================================
// GR_disp_cv_pMesh               boundary of surface from points
// was GR_DrawSurPtab
// also TSU_Draw_pMesh

// load pointSet from bin-file, compute boundary, display.
//
// Input:
//   dbi       get dataset "A<dbi> out of modelSource
//   att       ? 5 ?
//   file      binary-data of pointSet, see MSH_bload_pTab/MSH_bsav_pTab
// Output:
//   dli


  int           *iba, ibNr, PtabNr, i1;
  // long          dli;
//   ColRGB        *col;
  MemTab(Point) pTab = _MEMTAB_NUL;



  // printf("GR_disp_cv_pMesh att=%d dbi=%ld\n",att,dbi);

  // dli = AP_dli_act;

//   col = COL_INT32(&att);   // col = (ColRGB*)&att;
//     DEB_dump_obj__ (Typ_Color, col, " col");

  MemTab_ini__ (&pTab, sizeof(Point), Typ_PT, 10000);

//   // override att:  set to 5=th2BlueDashed
//   att = 5;
// see also gis_bload_pTab


  // load pTab from bin.file; mallocs Ptab !
  i1 = MSH_bload_pTab (&pTab, AP_modact_nam, dbi);
  if(i1 < 0) return -1;
  PtabNr = pTab.rNr;
    // printf(" PtabNr=%d\n",PtabNr);


  // get space for boundary
  iba = (int*)memspc101;
  ibNr = sizeof(memspc101) / sizeof(int);    // 4 byte
  // if(ibNr < PtabNr) {TX_Error("GR_DrawSurPtab E001"); return -1;}

  // get boundary (iTab) for n points (pTab)
  i1 = MSH_ibnd_ptn (iba, &ibNr, pTab.data, pTab.rNr);
  if(i1 < 0) goto L_done;

  // // Testdisplay of boundary
  // MSH_test_disp2 (iba, ibNr, Ptab);


//   dli = DL_StoreObj (Typ_SUR, dbi, att);

  // display symbolic
//   if(TSU_sStyl > 0) {
    GL_att_cv (Typ_Att_dash_long);  // attrib for symbolic
    GL_set_ipcv (ibNr, pTab.data, iba, 1);
//     GL_DrawCvIpTab (att, iba, ibNr, pTab.data, 1);
//   }

/*
  // get spce for curve
  cv = alloca(sizeof(Point)*(ibNr+1));
  for(i1=0; i1<ibNr; ++i1) {
    // printf(" P[%d] ib=%d\n",i1,ib[i1]);
    // printf("       ia=%d\n",ia[ib[i1]]);
    cv[i1] = Ptab[iba[i1]];
  }
  cv[ibNr] = cv[0];
  // GR_tDyn_pcv (cv, ibNr+1, 5);
  GR_tDyn_pcv (&dli, att, ibNr+1, cv);
*/

    // printf(" <<<<<<<<<<<<<<<<<<<GR_DrawSurPtab free pTab %p\n",Ptab);

  L_done:
  MemTab_free (&pTab);

  return 0;

}


//================================================================
  int GR_temp_att__ (int iAtt) {
//================================================================
// GR_temp_att__    disp direction/objID for all objs in group
// Input:
//   iAtt     bit-0   disp-objID         (GR_TMP_IDO)
//            bit-1   disp-direction     (GR_TMP_IDIR)
//
// was APT_disp_dir

  static TimeStamp ts1;
  static int       at1 = -1;
  void             *p1;


  // printf("GR_temp_att__ %d\n",iAtt);


  // check if iAtt is modified
  if(iAtt != at1) {
    at1 = iAtt;
    goto L_mod;
  }

  // iAtt unmodified - exit if nothing to display
  if(!iAtt) goto L_exit;


  // check if Group changed 
  Grp_get_ts ((TimeStamp*)&p1);
  if(ts1 == *((TimeStamp*)p1)) {
      // printf(" _dispDir-ts-unmod.\n");
    goto L_exit;
  }
  ts1 = *((TimeStamp*)p1);


  L_mod:
    // get bit-0 = objID
    if(iAtt & 1) {
      GR_temp_att_1 (0);
    } else {
      GL_temp_del_1 (GR_TMP_IDO);
    }

    // get bit-1 = direction
    if(iAtt & 2) {
      GR_temp_att_1 (1);
    } else {
      GL_temp_del_1 (GR_TMP_IDIR);
    }


  L_exit:
  // DL_Redraw ();
  return 0;

}


//================================================================
  int GR_temp_att_1 (int iAtt) {
//================================================================
// GR_temp_att_1    disp direction/objID for all objs in group
// Input:
//   iAtt     0   disp-objID         (GR_TMP_I0)
//            1   disp-direction     (GR_TMP_IDIR)
#define dspNam 0  // use GL-recordNr GR_TMP_I0 for Display/ObjNames-ON
#define dspDir 1  // use GL-recordNr GR_TMP_IDIR for Display/ObjDirection-ON


  int           irc, typ, bTyp, ptNr, grpNr;
  long          dli, dlSiz, dbi, l1;
  char          oid[32];
  Point         *pta, pt1;
  Vector        vc1;
  DL_Att        *dla;
  MemTab(Point) mtp = _MEMTAB_NUL;
  ObjDB         *grp;


  // printf("GR_temp_att_1 %d\n",iAtt);


  // get memSpc for points
  // MemTab_ini_fixed (&mtp, MEM_alloc_tmp (SPC_MAX_STK), SPC_MAX_STK,
                    // sizeof(Point), Typ_PT);
  MemTab_ini__ (&mtp, sizeof(Point), Typ_PT, 0);
  irc = MemTab_add (&mtp, &l1, NULL, 3000, 2);   // get spc
  if(irc < 0) {TX_Error("GR_temp_att_1 EOM-1"); return -1;}

  // get nr of groupMembers
  grpNr = Grp_get__ (&grp);
    // printf(" temp_att_1-grpNr=%d\n",grpNr);
    // Grp_dump ();

  // get DL
  dlSiz = DL_get__ (&dla);
    // printf(" temp_att_1-dlSiz=%ld\n",dlSiz);


  // preset GL-record-index
  if(iAtt == dspNam) DL_temp_ind = GR_TMP_IDO;    // 0=objID
  else               DL_temp_ind = GR_TMP_IDIR;   // 1=direction


  DL_temp_init ();               // open temp. GL-list DL_temp_ind
  GL_att_sym (ATT_COL_YELLOW);   // set color and LineWidth


  for(dli=0; dli<dlSiz; ++dli) {

  // for(ii=0; ii<grpNr; ++ii) {
    // dli = grp[ii].dlInd;

    if(grpNr) {
      // group is active; skip obj not in group.
      if(dla[dli].grp_1) continue;
    }


    // skip subModels
    if(DL_OBJ_IS_ACTMDL(dla[dli])) continue;

    // skip hidden objects
    if(DL_IS_HIDDEN(dla[dli])) continue;

    // skip invis. obj's
    if(dla[dli].unvis != 0) continue;

    typ = dla[dli].typ;

    // skip dynam. objects (see APT_disp_SymB)
    // if(dbi < 0) continue;
    if(typ == Typ_dynSym) continue;

    // skip undisplayable objects
    if(typ < Typ_LN) {
      if(typ < Typ_PT) continue;
      // dir: skip also PT
      if(iAtt == dspDir) continue;
    }

    // get basic typ from typ
    bTyp = AP_typDB_typ (dla[dli].typ);
      // printf(" typ_2_bastyp %d from %d\n",bTyp,typ);

    // skip notes if MODE_DISP_ONAM_NT is 0
    if((bTyp == Typ_Note)&&(!MODE_DISP_ONAM_NT)) continue;


    // skip objects with no direction ..
    if(iAtt == dspDir) {
      if(bTyp >= Typ_PLN) continue;    // TODO: normal-dircetion for surfaces ..
      // if(bTyp >= Typ_Note) continue;
    }


    //----------------------------------------------------------------
    dbi = dla[dli].ind;
      // printf(" _dispDir dli=%ld typ=%d dbi=%ld\n",dli,typ,dbi);


    // handle objects without polygons
    if(bTyp >= Typ_PLN) {
      if(bTyp == Typ_PLN) {
        // get origin
        UT3D_ptvcpar_std_dbo (&pt1, NULL, NULL, Ptyp_start, Typ_PLN, dbi);
        pta  = &pt1;
        ptNr = 1;
        goto L_disp;
      } else if(bTyp == Typ_Note) {
        irc = UT3D_ptvcpar_std_dbo (&pt1, NULL, NULL, Ptyp_start, bTyp, dbi);
        if(irc >= 0) {
          pta  = &pt1;
          ptNr = 1;
          goto L_disp;
        }
      }
// TODO:
//       TX_Print("**** TODO: GR_temp_att_1 typ %d not yet supp.",bTyp);
      continue;
    }



    // get polygon from DB-obj
    MEMTAB_CLEAR (&mtp);
    irc = UT3D_mtpt_dbo (&mtp, NULL, bTyp, dbi, AP_modact_ibm);
    if(irc < 0) {
      TX_Print("******* GR_temp_att_1 E-%d %ld",bTyp,dbi);
      continue;
    }

    ptNr = mtp.rNr;
    pta  = mtp.data;

      // TESTBLOCK
      // DEB_dump_nobj__ (Typ_PT, ptNr, pta, "_dispDir-pta");
      // if(ptNr > 200) DEB_halt();
      // END TESTBLOCK


    //----------------------------------------------------------------
    L_disp:
      if(iAtt == dspNam) {
        // display obj-ID
        oid[0] = '_';
        APED_oid_dbo__ (&oid[1], bTyp, dbi);
  
        // get pt1 = centerpoint odf polygon
        UT3D_pt_mid_pta (&pt1, pta, ptNr, 1);
          // DEB_dump_obj__(Typ_PT, &pt1, " _dispDir-pt1: oid=|%s|",oid);
        // disp obj-ID
        GL_set_txtA (&pt1, oid);
        continue;
  
  
      //----------------------------------------------------------------
      } else {
        // display direction
        GL_set_arrh3D (&pta[ptNr - 1], &pta[ptNr - 2]);
      }

  }

  GL_list_close (); // close GL-record  glEndList

  MemTab_free (&mtp);

  return 0;

}


/*
//=========================================================
  int GR_temp_pln (int ind, Plane *pl1, int att, int typ) {
//=========================================================
// display axisSystem with x,y,z-characters
//   att       linetype; see INF_COL_CV
//   typ       1=Plane; 2=Axis; 4=Axis+Chars; 5=Plane+Axis+Chars;
//
// see APT_disp_SymV1 (SYM_AXIS,   1, &pln1.po, 1.);


  if(tmpDspMode) DEB_dump_obj__ (Typ_PLN, pl1, "GR_temp_pln");
  // GR_temp_pt (&pl1->po, SYM_TRI_S, 2);
  // GR_temp_vc (&pl1->vz, &pl1->po, 2, 0);

  DL_temp_init ();

  // GL_DrawSymV3 (&dli, SYM_SQUARE, att, &pl1->po, &pl1->vz, 1.);
  GL_set_symVX (pl1, typ, 1.);

  GL_list_close (); // close GL-record

  return 0;

}
*/

/*================== EOF ===========================*/
