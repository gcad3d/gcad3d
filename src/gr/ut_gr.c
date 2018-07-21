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
 iAnz fuer Kreis, Cyl, Disk .. muss angepasst werden ..
 Man muesste Strip ebenfalls trennen in planarenStrip und nonPlanarStrip !!!!!!
   (gibts bei Fan schon. Bei non-planar muessen viele Normalvektoren geliefert
   werden, bei planar nur einer.)

  GR_CreDiskSeg (&id1, Typ_Att_Fac1, &ci1, 10.0);
  GR_CreCylSeg  (&id1, Typ_Att_Fac1, &ci1, &ci2);

-----------------------------------------------------
Modifications:
2002-02-17 GR_CrePoly und GR_CreDitto2 zu. RF.
2001-06-06 GR_Init1 zu.
2001-04-22 GL_DrawRSur: 2 Par. zu.
           GR_CreQFac (Vierecke) raus; Ersatz GR_CrePrism.
2001-04-21 erstmals erfasst, GR_DrawCyl,GL_DrawRSur korr.

-----------------------------------------------------
*/
#ifdef globTag
 void GR(){}
#endif
/*!
\file  ../gr/ut_gr.c
\brief create graphic objects 
\code



GR_Cre..      create dynamic-DB-obj, DL-record and display obj
              (get dynamic DB-index: dbi = DB_Store.. ((long)-1, obj, ..);

GR_Draw..     create DL-record and display obj 
              Do not create DB-record

GR_Disp_..    display obj (using dynamic DL-record)
              Do not create DB-record
              (get dynamic DL-record:  dli = DL_StoreObj (oTyp, -1L, att);)


=====================================================
List_functions_start:

GR_Disp_message       switch for textoutput to console (DispMode)

GR_Disp_obj           tempDisp obj from typ+struct
GR_Disp_dbo           tempDisp obj from typ+dbInd
GR_Disp_ox            temp. display of ObjGX-structs

GR_Disp_txi           display integer at position
GR_Disp_txi2          Testdisplay integer at 2D-point
GR_Disp_tx2           text at 2D-point
GR_Disp_tx            text at 3D-point

GR_Disp_pt            disp 3D-point
GR_Disp_pt2           disp 2D-point
GR_Disp_4db           disp 2D-Box from 4 doubles
GR_Disp_pTab          disp 3D-points
GR_Disp_npti          display ptNr points and point-numbers
GR_Disp_p2Tab         disp 2D-points

GR_Disp_vc2           display 2D-Vector; length true or normalized
GR_Disp_vc            disp 3D-vector at 3D-point; length true or normalized.

GR_Disp_ln            disp 3D-line
GR_tmpDisp_ln
GR_Disp_ln1           disp line between 2 points
GR_Disp_ln2           disp line between 2 2D-points
GR_Disp_rect1         disp. rectangle
GR_Disp_cv            disp. polygon between points
GR_Disp_cv2
GR_Disp_ac
GR_Disp_ell           disp. ellipse
GR_Disp_bez           display  Bezier-Curve
GR_Disp_rbez          display rational-bezier-curve
GR_Disp_CvBSp           Testdisplay  BSP-Curve
GR_Disp_rbspl         testdisplay rational-bezier-curve
GR_Disp_pol           disp. polygon = GR_Disp_plg
GR_Disp_ccv           display ConcatenatedCurVe

GR_Disp_spu           display nicht gelochte planare Flaeche
GR_Disp_spu1          display planare Contour
GR_Disp_sur           display surface from OGX
GR_Disp_sru           Display ungetrimmte/ungelochte Ruled Flaeche
GR_Disp_sbsp          display  BSP-Surf; nur Kontrollpunkte

GR_Disp_pln           Plane
GR_Disp_axis          display axisSystem with x,y,z-characters
GR_Disp_box           disp 3D-boundingBox (lines)

GR_Disp_cv3p2         Display boundary of triangle from 3 2D-points
GR_Disp_tria          display boundary of triangle as lines
GR_Disp_triv          display normalVector and number of triangle
GR_Disp_tris          display surface of triangle
GR_Disp_su3p2         display surface of 3 2D-points
GR_Disp_su3pt         display surface of 3 points
GR_Disp_fan           display triangleStrip
GR_Disp_patch         display Opengl-patch (type & n-points)
GR_Disp_nfac          display (not indexed) Fac3-faces
GR_Disp_ipatch        display a set of triangles from indexed points
GR_Disp_iSur          display tesselated faces (using fmt pmt imt)

GR_Draw_obj           display obj from typ+struct
GR_Draw_ox            display of ObjGX-structs
GR_Draw_dbo           display DB-obj (typ+dbInd)
GR_Draw_vc            display 3D-Vector; length true or normalized
GR_Draw_spu
GR_DrawSup

GR_create_dummy       create a dummy DL-record; in DL and in GL.
GR_CrePoint
GR_DrawPoint
GR_CreLine
GR_DrawLine
GR_CreCirc
GR_DrawCirc
GR_CrePoly
GR_DrawPoly
GR_DrawCurv
GR_DrawCvPol
GR_DrawCvEll           draw ellipse
GR_DrawCvPpsp3              
GR_DrawCvBSp           draw bSplineCurve
GR_DrawCvRBSp
GR_DrawCvCCV
GR_DrawCvCCV2
GR_Draw_subCurv        draw segment of polygon or CCV.

GR_CreTxtA
GR_DrawTxtA
GR_CreTxtG
GR_DrawTxtG
GR_DrawDimen

GR_CreDitto2
GR_DrawDitto2
GR_DrawModel

GR_DrawPlane
GR_DrawSur

GR_DrawCyl               Cyl und Cone
GR_DrawTorSeg
GR_DrawPrism 

GR_DrawFan
GR_DrawTriaFan 
GR_DrawStrip
GR_DrawTriaStrip
GR_Draw_ipatch        display a set of triangles from indexed points
GR_Draw_iSur          display tesselated faces (using fmt pmt imt)

GR_DrawDisk

GR_CreCyl                In: 2 Punkte 2 Radien        
GR_CrePrism              In: zwei Punkteketten
GR_CreDisk               In: innerer Rad u ausserer rad fuer Kreisring       
GR_CreTorSeg             In: 2 Circs         
GR_CreSolSph
GR_CreSolCon
GR_CreSolTor
GR_CreSol__

GR_CreFan
GR_CreTriaFan            In: punkte rund um eine Flaeche
GR_CreTriaStrip

GR_pt_par_sel_npt        get selectionpoint and parameter on polygon

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



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


//#include "../gtk/func_types.h"            // Typ_Att_def

// #include "../ut/ut_umem.h"             // MEM_alloc_tmp
#include "../ut/ut_geo.h"
#include "../ut/ut_cast.h"                // INT_PTR
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

#include "../gr/ut_gr.h"
#include "../ut/func_types.h"                  // SYM_SQUARE

#include "../xa/xa.h"                     // mem_cbuf1_SIZ
#include "../xa/xa_mem.h"                 // memspc55



// typedef_MemTab(int);
// typedef_MemTab(IndTab);
// typedef_MemTab(Point);




//=============== extern glob. vars ======================
// aus xa.c:
extern int       WC_modact_ind;           // -1=primary Model is active;
extern AP_STAT   AP_stat;                 // sysStat,errStat..
extern ColRGB    AP_defcol;

// aus ../ci/NC_Main.c:
extern int     APT_3d_mode;
extern int     APT_dispSOL;           // 0=ON=shade; 1=OFF=symbolic
extern int     APT_dispDir;
extern int     APT_obj_stat;          // 0=permanent, 1=temporary (workmode)
extern long    AP_dli_act;            // index dispList

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
extern long   DL_ind_act;


// aus ../gr/ut_GLU.c:
extern Point     *GLT_pta;
extern long      GLT_pta_SIZ;



// aus ../gr/tess_su.c:
extern int TSU_mode;   // 0=normal darstellen; 1=speichern


// aus ../gr/ut_GL.c:
extern ColRGB  GL_actCol;
extern int     GL_actTex;



/*=============== local glob. vars =======================*/
long  objInd = 0;

static int DispMode=1;  ///< 0=Aus, 1=Ein.









//====================================================================
  void GR_Init1 () {
//====================================================================
// is calle from ED_work_END


  // printf("GGGGGGGGGGGG GR_Init1\n");

  GL_temp_del_all ();       // delete ALL objects in temp-area of DispList
  GL_Init1 ();              // delete displist
  AP_mdlbox_invalid_set();  // set AP_stat.mdl_box_valid = void

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


//====================================================================
  void GR_CreTxtA (long *ind, int attInd, Point *pt1, char *txt) {
//====================================================================
/// \code
/// att am besten Typ_Att_Symb.
/// Position links unten.
/// \endcode


  long   dlInd;
  GText  gtx1;


  if(*ind == 0) {

    gtx1.pt   = *pt1;
    gtx1.size = -1.0;
    gtx1.dir  = 0.0;
    gtx1.txt  = txt;

    *ind = DB_StoreGTxt (-1L, &gtx1);

  // } else if(*ind < dyn_tx_max) {
    // *ind += dyn_tx_max;
  }



  dlInd = DL_StoreObj (Typ_ATXT, *ind, attInd);


  GL_DrawTxtA (&dlInd, attInd, pt1, txt);

}







//====================================================================
  void GR_DrawTxtA (long *ind, int attInd, Point *pt1, char *txt) {
//====================================================================
/// \code
///   attInd  colorIndex; eg ATT_COL_RED; see INF_COL_SYMB
///   pt1     Position bottom left.
/// \endcode


  Point   ptTr;

  // if(APT_2d_to_3d_mode == OFF) {
    GL_DrawTxtA (ind, attInd, pt1, txt);

  // } else {
    // UT3D_pt_traptm3 (&ptTr, APT_2d_to_3d_Mat, pt1);
    // GL_DrawTxtA (ind, attInd, &ptTr, txt);
  // }

  return;
}


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


  if(*ind == 0) {

    gtx1.pt   = *pt1;
    gtx1.size = size;
    gtx1.dir  = ang;
    gtx1.txt  = txt;

    *ind = DB_StoreGTxt (-1L, &gtx1);

  // } else if(*ind < dyn_tx_max) {
    // *ind += dyn_tx_max;
  }


  dlInd = DL_StoreObj (Typ_ATXT, *ind, attInd);
  /*  IN:  Objectindex objInd  und ObjTyp Typ_xx */
  /*  OUT: DispListIndex dlInd */



  // GL_DrawTxtG (&dlInd, attInd, pt1, size, ang, txt);
  GL_DrawTxtG (&dlInd, attInd, &gtx1);

}


//=======================================================================
  void GR_DrawDimen (long *ind, int attInd, Dimen *dim1) {
//=======================================================================

  // printf("GR_DrawDimen dtyp=%d\n",dim1->dtyp);


  if(dim1->dtyp == 3)      GL_DrawDima   (ind, attInd, dim1);
  else if(dim1->dtyp > 20) GL_DrawLdr    (ind, attInd, dim1);    // 21
  else if(dim1->dtyp > 1)  GL_DrawDimrad (ind, attInd, dim1);    // 2
  else if(dim1->dtyp > 0)  GL_DrawDimdia (ind, attInd, dim1);    // 1
  else                     GL_DrawDimen  (ind, attInd, dim1);    // 0

  return;

}


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
    // UT3D_pt_traptm3 (&ptTr, APT_2d_to_3d_Mat, pt1);
    // GL_DrawTxtG (ind, attInd, &ptTr, size, ang, txt);
  // }

  return;
}




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


  /* onjInd erhaelt man beim abspeichern in der DB */
  if(*ind == 0) {
    *ind = DB_StorePoint (-1L, pt1);  // store dynamic obj in DB.
  }

  dlInd = DL_StoreObj (Typ_PT, *ind, attInd);
  /*  IN:  Objectindex objInd  und ObjTyp Typ_xx */
  /*  OUT: DispListIndex dlInd */



  GR_DrawPoint (&dlInd, attInd, pt1);

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
    // UT3D_pt_traptm3 (&ptTr, APT_2d_to_3d_Mat, pt1);
    // GL_DrawPoint (ind, attInd, &ptTr);
  // }

  return;
}





//====================================================================
  void GR_CrePoly (long *ind, int attInd, int ianz, Point* ptarr) {
//====================================================================


  long     dlInd;
  ObjGX    ox1;
  Memspc   memSeg1;
  CurvPoly plg1;


  // printf("GR_CrePoly %d %d\n",*ind,ianz);


  if(*ind == 0) {

    // fuer die lvtab wird Platz benoetigt
    UME_init (&memSeg1, memspc55, sizeof(memspc55));
    UT3D_plg_pta (&plg1, ptarr, ianz, &memSeg1);

    ox1.typ   = Typ_CVPOL;
    ox1.form  = Typ_CVPOL; //Typ_PT;
    ox1.siz   = ianz;
    ox1.data  = &plg1;
    *ind = DB_StoreCurv ((long)-1, &ox1, 0);

  }



  dlInd = DL_StoreObj (Typ_CVPOL, *ind, attInd);

  GR_DrawPoly (&dlInd, attInd, ianz, ptarr);

}


//====================================================================
  void GR_DrawPoly (long *ind, int attInd, int ianz, Point* ptarr) {
//====================================================================
///   Erzeugung eines 3D-Polygon


  int  i1;


  // printf("GR_DrawPoly %d\n",ianz);

  if(GLT_pta_SIZ < ianz) {
    i1 = GLT_alloc_pta (ianz);
    if(i1 == -1) return;
  }



  for(i1=0; i1 < ianz; ++i1) {

    // if(APT_2d_to_3d_mode == OFF) {
      GLT_pta[i1] = ptarr[i1];

    // } else {
      // UT3D_pt_traptm3 (&GLT_pta[i1], APT_2d_to_3d_Mat, &ptarr[i1]);
    // }
  }

  GL_DrawPoly (ind, attInd, ianz, GLT_pta);

}


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


  /* GR_Att (attInd); */




  // if(APT_2d_to_3d_mode != OFF) {
    // // Achtung: hier fehlt di->vc1 transformieren !!
    // UT3D_pt_traptm3 (&di->po, APT_2d_to_3d_Mat, &di->po);
    // UT3D_pt_traptm3 (&pt1, APT_2d_to_3d_Mat, pt);
  // } else {
    pt1 = *pt;
  // }

  GL_DrawDitto2 (ind, &pt1, az, mir, di);

}


//====================================================================
  void GR_CreLine (long *ind, int attInd, Line *ln1) {
//====================================================================
/// \code
/// create DB-record, DL-record and display line
/// Input:
///                0    create new dynamic DB-obj;
///                < 0  do not store obj in DB
///   attInd       see GR_Disp_ln2  (see ~/gCAD3D/cfg/ltyp.rc)
///
/// Output:
///   ind          dbi  (negative for dynamic obj)
/// \endcode
 


  long   dlInd;


  if(*ind == 0) {
    *ind = DB_StoreLine ((long)-1, ln1);
  }


  dlInd = DL_StoreObj (Typ_LN, *ind, attInd);
  /*  IN:  Objectindex objInd  und ObjTyp Typ_xx */
  /*  OUT: DispListIndex dlInd */



  GR_DrawLine (&dlInd, attInd, ln1);

}


//====================================================================
  void GR_CreCirc (long *dbi, int attInd, Circ *ci1) {
//====================================================================
// TODO: replace APT_DrawCirc; set retCod=dli

  long   dlInd;


  if(*dbi == 0) {
    *dbi = DB_StoreCirc ((long)-1, ci1);

  // } else if(*ind < dyn_ci_max) {
    // *ind += dyn_ci_max;
  }



  dlInd = DL_StoreObj (Typ_CI, *dbi, attInd);
  /*  IN:  Objectindex objInd  und ObjTyp Typ_xx */
  /*  OUT: DispListIndex dlInd */

  GR_DrawCirc (&dlInd, *dbi, attInd, ci1);

}



//========================================================================
  int GR_DrawPlane (long *ind, int attInd, Plane *pl1) {
//========================================================================
/// SYM_SQUARE= normale Plane; SYM_PLANE= gelb (activ)


  // printf("GR_DrawPlane ind=%ld att=%d\n",*ind,attInd);

  // APT_disp_SymV3 (SYM_SQUARE, attInd, &pl1->po, &pl1->vz, 1.);

  // GL_DrawSymV3 (ind, SYM_SQUARE, attInd, &pl1->po, &pl1->vz, 1.);
  GL_DrawSymVX (ind, attInd, pl1, 1, 1.);

  return 0;
}


//====================================================================
  void GR_CreTriaFan (long *ind, int attInd, Point *pc, int anz, Point *pa1) {
//====================================================================
/// GR_CreTriaFan     ein od mehrere Dreiecke mit gemeinsamem Pkt.

  
  long   dlInd;


  /* onjInd erhaelt man beim abspeichern in der DB */
  // ++objInd;
  
  dlInd = DL_StoreObj (Typ_QFac, *ind, attInd);
  /*  IN:  Objectindex objInd  und ObjTyp Typ_xx */
  /*  OUT: DispListIndex dlInd */



  GR_DrawTriaFan (&dlInd, attInd, pc, anz, pa1);
  
  *ind = dlInd;

}


//====================================================================
  void GR_DrawTriaFan (long *ind, int attInd, Point *pc, int anz, Point *pa1) {
//====================================================================
/// Attr f faces sind 4,5 od 6

  GL_Surf_Ini (ind, attInd);
  // GL_Draw_Ini (ind, attInd);
  // GL_Draw_Ini (ind, 6);  // 4 OK, 10duenn,blau; 2=rot, 10=rot,

  GL_ColSet (&GL_actCol);

  GL_DrawFan (pc, anz, pa1, 1, 1);

  GL_EndList ();

}


//====================================================================
  void GR_CreTriaStrip (long *ind, int attInd,int ptUNr,int ptVNr,Point *pa) {
//====================================================================

  
  long   dlInd;


  /* onjInd erhaelt man beim abspeichern in der DB */
  // ++objInd;
  
  dlInd = DL_StoreObj (Typ_QFac, *ind, attInd);
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
    GL_Surf_Ini (ind, attInd);

    GL_ColSet (&GL_actCol);

    if(newS) GLT_stor_rec (6, NULL, NULL, newS);

    for(i1=0; i1<ptVNr-1; ++i1) {
      irc = GL_DrawStrip2 (pa, &pa[ptUNr], ptUNr, 0);
      if(irc < 0) return irc;
      pa += ptUNr;              // advance 1 row
    }

    GL_EndList ();

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
    irc = GL_DrawStrip2 (pa, &pa[ptUNr], ptUNr, 0);
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

  dlInd = DL_StoreObj (Typ_QFac, *ind, attInd);
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
    GL_DrawStrip2 (pa1, pa2, anz, 0);
    GL_EndList ();

  } else {
    GLT_stor_rec (0, NULL, NULL, 0);
    GL_DrawStrip2 (pa1, pa2, anz, 0);
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



  // UI_GR_DrawInit ();

  if(TSU_mode == 0)  {      // 0 = draw OpenGL
    if(sStyl == 0) {            // 0=sol
      GL_Surf_Ini (ind, col);
      GL_ColSet (&GL_actCol);
    } else {
      GL_Draw_Ini (ind, Typ_Att_Fac1);
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
      GL_Draw_Ini (ind, Typ_Att_Fac1);
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

  GL_DrawPoly (ind, att, ptNr, pta);

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
      GL_Draw_Ini (ind, Typ_Att_Fac1);
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
      // tessel,store via GL_DrawStrip2/GLT_stor_rec(2)
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

  GL_DrawPoly (ind, att, ptNr, pta);

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



  // printf("GR_CreSol__ %ld %d\n",*ind,attInd);
  // printf(" typ=%d form=%d\n",db1->typ,db1->form);
  // UTO_dump__ (db1, "GR_CreSol__");
  // UTO_dump_s_ (db1, "GR_CreSol__");



  // die Color / das Attribut
  GL_actCol =  *(COL_INT32(&attInd)); // GL_actCol = *((ColRGB*)&attInd);



  if(db1->typ == Typ_SPH) return GR_CreSolSph (ind, attInd, db1->data); 
  if(db1->typ == Typ_CON) return GR_CreSolCon (ind, attInd, db1->data); 
  if(db1->typ == Typ_TOR) return GR_CreSolTor (ind, attInd, db1->data); 
  if(db1->typ == Typ_BREP)return GR_CreSolBrep(ind, attInd, db1->siz, db1->data); 


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
      UTO_obj_getp (&pci, &i1, &oTab[0]);
        // UT3D_stru_dump(Typ_CI, pci, " o1: ");
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
      irc = UTO_obj_getp (&pp, &i1, &oTab[1]);
      if(irc < 0) return irc;
      pa2[0] = *pp;   // copy the pt
      // printf("o2p=%f,%f,%f\n",pa2[0].x,pa2[0].y,pa2[0].z);
      pt2Nr = 1;
      break;


    case Typ_CI:
      // ci2 = DB_GetCirc ((long)oTab[1].data);
      // UT3D_cv_ci (pa2, &pt2Nr, &ci2, pt1Nr, -0.1);
      UTO_obj_getp (&pci, &i1, &oTab[1]);
      UT3D_cv_ci (pa2, &pt2Nr, pci, pt1Nr, -0.1);
      break;


    case Typ_VC:
      irc = UTO_obj_getp (&pvc, &i1, &oTab[1]);
      if(irc < 0) return irc;
      vc1 = *pvc;   // copy the vec
      goto L_translate;


    case Typ_VAR:
      // printf("VAR # %d\n",oTab[1].data);
      // printf("ci1=%f,%f,%f\n",ci1.vz.dx,ci1.vz.dy,ci1.vz.dz);
      // val d1 aus DB -> d1 holen
      // d1 = DB_GetVar ((long)oTab[1].data); // get Dicke
      irc = UTO_obj_getp (&pd, &i1, &oTab[1]);
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
      GL_Draw_Ini (ind, Typ_Att_Fac1);

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
  void GR_DrawLine (long *ind, int attInd, Line *ln1) {
//====================================================================
/// \code
///   Erzeugung einer 3D-Line
/// Input:
///   ind          dli
///   attInd       see GR_Disp_ln2  (see ~/gCAD3D/cfg/ltyp.rc)
/// \endcode


  Line lnTr;


  // printf("GR_DrawLine %f,%f,%f\n",ln1->p1.x,ln1->p1.y,ln1->p1.z);
  // printf("          - %f,%f,%f\n",ln1->p2.x,ln1->p2.y,ln1->p2.z);
  // printf(" APT_2d_to_3d_mode=%d\n",APT_2d_to_3d_mode);


  /* GR_Att (attInd); */



  // if(APT_2d_to_3d_mode == OFF) {
    GL_DrawLine (ind, attInd, ln1);

  // } else {
    // // od use UTO_obj_tra_m3 ..
    // UT3D_pt_traptm3 (&lnTr.p1, APT_2d_to_3d_Mat, &ln1->p1);
    // UT3D_pt_traptm3 (&lnTr.p2, APT_2d_to_3d_Mat, &ln1->p2);
    // GL_DrawLine (ind, attInd, &lnTr);
  // }


  // disp direction
  if(APT_dispDir) {
    APT_disp_dir (&ln1->p2, &ln1->p1);
  }

}


//====================================================================
  int GR_DrawCirc (long *dli, long dbi, int attInd, Circ *cii) {
//====================================================================


  int     irc, i1, i2, ptAnz, ptNr, idisp;
  double  d1, z1;
  Point   *pa3;
  // Point2  p20, p21, p22;
  Point2  *pa2;
  Circ2   ci2;
  Circ    ciTr, *ci1;




  // printf("GR_DrawCirc um %f,%f,%f\n",cii->pc.x,cii->pc.y,cii->pc.z);
  // printf ("  von %f,%f,%f\n",cii->p1.x,cii->p1.y,cii->p1.z);
  // printf ("  bis %f,%f,%f\n",cii->p2.x,cii->p2.y,cii->p2.z);
  // printf ("  vz  %f,%f,%f\n",cii->vz.dx,cii->vz.dy,cii->vz.dz);
  // printf ("  rad=%f\n",cii->rad);
  // printf("GR_DrawCirc UT_DISP_cv=%f\n",UT_DISP_cv);


  // if(APT_3d_mode == ON) goto L_3D_Circ;


  // find nr of points necessary
  // fix nr of points according to modelsize
  ptNr = UT2D_ptNr_ci (fabs(cii->rad), fabs(cii->ango), UT_DISP_cv);
     // printf(" ptNr=%d\n",ptNr);




  // ist ein refsys aktiv ?
  // if(APT_2d_to_3d_mode == ON) {
    // UTO_obj_tra_m3 ((void*)&ciTr, Typ_CI, (void*)cii, APT_2d_to_3d_Mat);
    // ci1 = &ciTr;
    // goto L_3D_Circ;
  // } else {
    ci1 = cii;
  // }




  /*======================================================================*/
  /*  test 2D - circle */
  /*======================================================================*/

  if(fabs(ci1->vz.dx) > UT_TOL_min1) goto L_3D_Circ;
  if(fabs(ci1->vz.dy) > UT_TOL_min1) goto L_3D_Circ;
  if((fabs(ci1->vz.dz) - 1.0) > UT_TOL_min1) goto L_3D_Circ;


  // printf(" draw 2D_Circ\n");


  // get memory      dzt GLT_pta; better use MEM_alloc_tmp (alloca)
  pa2 = (Point2*)MEM_alloc_tmp((int)(sizeof(Point2)*ptNr));

  // make 2D-circ from 3D-circ
  UT2D_ci_ci3 (&ci2, ci1);

  // circ2 -> polygon
  UT2D_npt_ci (pa2, ptNr, &ci2);


  // display polygon
  z1 = ci1->pc.z;
  GL_DrawPoly2D (dli, attInd, ptNr, pa2, z1);


  // disp direction
  if(APT_dispDir) {
    int   ipe;
    Point pt31, pt32;
    ipe = ptNr - 1;
    // make 3D-points
    pt31 = UT3D_pt_pt2z (&pa2[ipe], z1);
    pt32 = UT3D_pt_pt2z (&pa2[ipe - 1], z1);
    APT_disp_dir (&pt31, &pt32);
  }


  return 0;



  /*======================================================================*/
  /*  3D-Circle */
  /*======================================================================*/
  L_3D_Circ:
  // printf(" draw 3D_Circ\n");


  // get pa3 = circular polygon
  if(dbi > 0) {
    // get polygon from file
    irc = PRCV_npt_dbo__ (&pa3, &ptNr, Typ_CI, dbi, WC_modact_ind);
    if(irc < 0) return -1;

  } else {
    // get memory      dzt GLT_pta; better use MEM_alloc_tmp (alloca)
    pa3 = (Point*)MEM_alloc_tmp((int)(sizeof(Point)*ptNr));
    // get pa3 = circular polygon (compute)
    UT3D_npt_ci (pa3, ptNr, ci1);
    // UT3D_cv_ci (GLT_pta, &ptNr, ci1, ptAnz, UT_DISP_cv);
  }


  // display polygon
  GL_DrawPoly (dli, attInd, ptNr, pa3);


  // display direction
  if(APT_dispDir) {
    int   ipe;
    ipe = ptNr - 1;
    APT_disp_dir (&pa3[ipe], &pa3[ipe - 1]);
  }


  return 0;

}


//================================================================
  int GR_DrawCurv (long *dli, int att, ObjGX *cv1, double zval) {
//================================================================
/// \code
/// att:                 see ~/gCAD3D/cfg/ltyp.rc
/// see APT_DrawCurv GR_Draw_dbo UTO_obj_Draw__
/// \endcode

  int       form;
  long      dbi;
  DL_Att    *att1;
  Memspc    tSpc1;


  form = cv1->form;

  printf("GR_DrawCurv dli=%ld att=%d\n",*dli,att);
  // UT3D_stru_dump (Typ_ObjGX, cv1, "  cv1:");


  if(form == Typ_CVPSP3) {
    GR_DrawCvPpsp3 (dli, att, cv1, zval);

  } else if(form == Typ_CVBSP) {
    if(*dli > 0) { DL_get_dla (&att1, *dli); dbi = att1->ind; }
    else dbi = 0L;
    GR_DrawCvBSp (dli, dbi, att, cv1->data);

  } else if(form == Typ_CVRBSP) {
    GR_DrawCvRBSp (dli, att, cv1->data);

  } else if(form == Typ_CVPOL) {
    GR_DrawCvPol (dli, att, cv1->data);

  } else if(form == Typ_CVPOL2) {
    GL_DrawPoly2D (dli, att, cv1->siz, cv1->data, zval);

  } else if(form == Typ_CVELL) {
    GR_DrawCvEll (dli, att, cv1->data);

  } else if(form == Typ_CVTRM) {
    // GR_DrawCvCCV (dli, att, cv1->data);  korr 2007-08-10
    UME_init (&tSpc1, memspc201, sizeof(memspc201));
    DL_get_dla (&att1, *dli);
    GR_DrawCvCCV (dli, att1->ind, att, cv1, &tSpc1);

// TODO: test for 2D-Curve
  // } else if(form == Typ_CVTRM2) {
    // GR_DrawCvCCV2 (ind, att, cv1);

  } else {
    TX_Error("GR_DrawCurv E001-%d-%d",cv1->typ,cv1->form);
    return -1;
  }

  return 0;

}
 

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
    // GL_DrawStrip2 (p1Anz, GL_ptArr31, GL_ptArr30);
    GL_DrawStrip2 (pa2, pa1, p1Anz, 1);
  } else {
    GL_DrawFan (pc, p1Anz, pa1, 0, 1);
  }


  GL_EndList ();



}




//====================================================================
  void GR_DrawCyl (long *ind, int attInd, Point *pt1, Point *pt2,
                                          double rd1, double rd2) {
//====================================================================
/// \code
///   Erzeugung eines Zylinders / Konus   (Cyl / Cone)
/// See also GL_draw_cone GL_Disp_patch GL_DrawFan
///          GL_DrawStrip1 GL_DrawStrip2 GL_DrawRCone
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
  // GL_DrawStrip2 (p2Anz, GL_ptArr30, GL_ptArr31);
  GL_DrawStrip2 (GL_ptArr30, GL_ptArr31, p2Anz);


  ++ii1;
  if(ii1 < (p1Anz-1)) { 
    // ++objInd;
    // *ind = DL_StoreObj (Typ_TOR, objInd, attInd);
    goto NextSeg;
  }


  GL_EndList ();


}
*/


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
  // Point   *cv;
  // Point   *Ptab = NULL;
  // MemTab(Point) pTab = MemTab_Init(sizeof(Point), Typ_PT, 10000);
  MemTab(Point) pTab = _MEMTAB_NUL;



  MemTab_ini (&pTab, sizeof(Point), Typ_PT, 10000);


  // printf("GR_DrawSurPtab %d %d\n",att,dbi);


  // override att:  set to 5=th2BlueDashed
  att = 5;

// see also gis_bload_pTab


  // load pTab from bin.file; mallocs Ptab !
  i1 = MSH_bload_pTab (&pTab, AP_modact_nam, dbi);
  if(i1 < 0) return -1;
  PtabNr = pTab.rNr;
    // printf(" PtabNr=%d\n",PtabNr);


  // get space for boundary
  iba = (int*)memspc101;
  iNr = sizeof(memspc101) / sizeof(int);    // 4 byte
  // if(iNr < PtabNr) {TX_Error("GR_DrawSurPtab E001"); return -1;}

  // get boundary (iTab) for n points (pTab)
  i1 = MSH_ibnd_ptn (iba, &iNr, pTab.data, pTab.rNr);
  if(i1 < 0) goto L_done;

  // // Testdisplay of boundary
  // MSH_test_disp2 (iba, iNr, Ptab);


  dli = DL_StoreObj (Typ_SUR, dbi, att);

  GL_DrawCvIpTab (&dli, att, iba, iNr, pTab.data, 1);
  AP_dli_act = dli;

/*
  // get spce for curve
  cv = alloca(sizeof(Point)*(iNr+1));


  for(i1=0; i1<iNr; ++i1) {
    // printf(" P[%d] ib=%d\n",i1,ib[i1]);
    // printf("       ia=%d\n",ia[ib[i1]]);
    cv[i1] = Ptab[iba[i1]];
  }
  cv[iNr] = cv[0];

  // GR_Disp_cv (cv, iNr+1, 5);


  GL_DrawPoly (&dli, att, iNr+1, cv);
*/

    // printf(" <<<<<<<<<<<<<<<<<<<GR_DrawSurPtab free pTab %p\n",Ptab);
  L_done:
  MemTab_free (&pTab);

  return 0;

}
 

//=============================================================================
  int GR_DrawSur (ObjGX *oxi, int att, long apt_ind) {
//=============================================================================
/// \code
/// siehe auch APT_DrawSur
/// oxi=Input.
/// Output:
///   AP_dli_act     dispListIndex of surf created
/// 
/// GR_DrawSur
///   GR_DrawSurPtab                   -
///   TSU_DrawSurT_
///     TSU_DrawSurTRU                 -
///     TSU_DrawSurTRV                 -
///     TSU_DrawSurTS                  -
///     TSU_DrawSurTP                  -
///     TSU_DrawSurTC                  -
///   GR_CreFan                        -
///   GR_DrawStrip                     -
///   TSU_DrawSurMsh                   -
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
  // UTO_dump_s_ (oxi, "GR_DrawSur");
  // if(oxi->typ==Typ_SUR) printf(" S:vtex=%d\n",((ColRGB*)&att)->vtex);


  // skip dummies (A=PTAB ..)
  // if(oxi->typ == Typ_ObjGX) return 0;
  // if(oxi->typ == Typ_Typ) return 0;
  // if(oxi->siz < 1) return 0;      // Typ_SURPTAB skip A=PTAB
  if(oxi->typ == Typ_SURPTAB) return GR_DrawSurPtab (apt_ind, att);



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
    // find textureNr of surface <apt_ind>
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
  if(oxi->typ == Typ_SURMSH)   goto L_SurMesh;
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
      if(WC_modact_ind >= 0) {
        strcat(cBuf, " in Submodel ");
        strcat(cBuf, DB_mdlNam_iBas(WC_modact_ind));
      }
      TX_Print(cBuf);
    }
    return i1;





  //=======================================================================
  L_SurCplx:
    i1 = TSU_DrawSurT_ (oxi, att, apt_ind);
    if(i1 < 0) {
      sprintf(cBuf,"degenerate element A%ld",apt_ind);
      if(WC_modact_ind >= 0) {
        strcat(cBuf, " in Submodel ");
        strcat(cBuf, DB_mdlNam_iBas(WC_modact_ind));
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
    // UT3D_stru_dump (Typ_LN,&lTab[i1],"L[%d]\n",i1);
    // dli = -1; GR_CreLine (&dli, 1, &lTab[i1]);
  // }


  dli = DL_StoreObj (Typ_SUR, apt_ind, 1);
  GL_DrawLn_Ini (&dli, 1);

  for(i1=0; i1<i2; ++i1)  GL_Disp_ln (&lTab[i1].p1, &lTab[i1].p2);
  // GL_Disp_cv (i2, lTab);

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
    // GL_DrawStrip2 ((Point*)oxp1->data, (Point*)oxp2->data, irow2);
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
    return TSU_DrawSurMsh (oxi, att, apt_ind);




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


  // printf("GR_DrawModel ind=%ld att=%d\n",db_ind,att);
  // printf(" TSU_mode=%d\n",TSU_mode);
  // UT3D_stru_dump (Typ_Model, mdr, " mdr=");
  // return;


  if(TSU_mode != 0) {       // 0=Normal 1=Store
    // printf(" DrawModel mit TSU_mode=Store %d\n",mdr->modNr);
    return TSU_exp_Mod (0, mdr);   // 2008-11-09
  }


  mdb = DB_get_ModBas (mdr->modNr);
  if(mdb == NULL) return -1;
/*
// removed - xy.tess will be loaded later 
  if(mdb->DLsiz < 1) {
    // TX_Error("GR_DrawModel E001 M%ld\n",db_ind);
    TX_Print("**** M%ld - Model %s empty .. \n",db_ind, mdb->mnam);
    return -1;
  }
*/


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
    // printf(" DLind=%ld\n",mdb->DLind);


  // extract filetype.
  irc = UTX_ftyp_s (ftyp, ffnam, 1);
  if(irc < 0) {TX_Print("GR_DrawModel FileType not found"); return irc;}
    // printf(" ftyp=|%s|\n",ftyp);


  // fix a safe modelname
  strcpy (mnam, mdb->mnam);
  // mnam ist internal ("abc") od external ("dir/fn")
  UTX_ftyp_cut (mnam);            // remove fTyp
  UTX_safeName (mnam, 1);         // change all '. ' to '_', not '/'
    printf(" DrawModel-mnam=|%s|\n",mnam);


  // check if tesselated data of the Model exists in xa/temp
  irc = tess_read_ (&impSpc, mnam);
  if(irc == 0) {loadMode = 1; goto L_mock_draw;}
  loadMode = 0;


  // import modelfile.
  if(!strcmp(ftyp, "TESS")) {
    // import Mockup from file into Mockup-struct
    TSU_imp_tess (&impSpc, ffnam);
    // copy tess-file -> tmp/.
    sprintf (memspc011, "%s%s.tess",OS_get_tmp_dir(),mnam);
      printf(" DrawModel-ffnam=|%s|\n",memspc011);
    OS_file_copy (ffnam, memspc011);

  } else {
    // get model as tesselated-data into impSpc, then write into file xa/temp

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
       // UT3D_stru_dump (Typ_PT, &pb1, "pb1");
       // UT3D_stru_dump (Typ_PT, &pb2, "pb2");


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
    // UT3D_stru_dump (Typ_SubModel, mdb, "  mdb-1:");

  dli = DL_StoreObj (Typ_Ditto, 0L, 0);
  DL_unvis_set (dli, 1);         // make Basemodel unvisible
    // removed 2012-01-18 - crashes with import .tss - files

  // display
  TSU_DrawSurTess (&dli, att, impSpc.start);

    // printf(" loadMode=%d\n",loadMode);
  if((loadMode == 0) || (!strcmp(ftyp, "TESS"))) {     // 2011-08-18
    UME_free (&impSpc);
/*
  } else {

    // create record "Int=3"
    UTO_rec_set (&oTab[1], Typ_Int4,   Typ_Int4,   1, (void*)3);  // mode
    // free mem & unload Dll
    irc = DLL_run1 (2, oTab);
    if(irc < 0) return;

    // unload DLL
    irc = DLL_run1 (3, NULL);
    if(irc < 0) return;
*/
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

  return irc;








  //================================================================
  // SubModel;
  L_model:
  dli = DL_StoreObj (Typ_Model, db_ind, att);
    // printf(" _StoreObj M%ld dli=%ld\n",db_ind,dli);
  GL_DrawModel (&dli, att, mdr, mdb);
  // DL_unvis_set (dli, 1);  // 2014-09-13


  return 0;

}


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
  // GL_DrawStrip2 (p2Anz, GL_ptArr30, GL_ptArr31);
  GL_DrawStrip2 (pa1, pa2, p2Anz, 0);


  ++ii1;
  if(ii1 < (p1Anz-1)) {
    /* ++objInd; */
    /* *ind = DL_StoreObj (Typ_TOR, objInd, attInd); */
    goto NextSeg;
  }


  GL_EndList ();


}

//=====================================================================
  int GR_DrawCvClot (long *ind, int att, CurvClot *cl1) {
//=====================================================================

  int    irc, ptNr;
  Point  *pta;
  // Memspc objSeg;
  // Memspc tmpSeg;


  // printf("GR_DrawCvClot\n");
  // UT3D_stru_dump (Typ_CVCLOT, cl1, "Clot.:");



  pta = (Point*)memspc55;
  ptNr = sizeof(memspc55) / sizeof(Point);

  // UME_init (&tmpSeg, memspc201, sizeof(memspc201));


  // clothoid polygon

  // UME_init (&objSeg, memspc501, sizeof(memspc501));
  // irc = UCV_PlgCltCrv (&ptNr, &pta, &objSeg, cl1, &tmpSeg);

  irc = UT3D_npt_clot (pta, &ptNr, cl1, UT_DISP_cv);
  if (irc < 0) return -1;

  GL_DrawPoly (ind, att, ptNr, pta);

  return 0;

}


//============================================================================
  int GR_DrawCvCCV (long *dli, long dbi, int att, ObjGX *cv1, Memspc *tbuf1) {
//============================================================================
// display DB-obj - trimmed-curve 

  int    irc, ptNr, paSiz;
  int    ipe;
  void   *msPos;
  Point  *pta;


  // printf("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD \n");
  // UTO_dump__ (cv1, "GR_DrawCvCCV dbi=%ld",dbi);
  // printf("  APT_obj_stat=%d\n",APT_obj_stat);


  msPos = UME_get_next (tbuf1);  // keep free pos of tbuf1

/*
  // get space for points in tbuf1
  paSiz = (UME_ck_free (tbuf1) / sizeof(Point)) - 1;
  pta = UME_reserve (tbuf1, paSiz * sizeof(Point));
  ptNr = 0;
  irc = UT3D_npt_trmCv (&ptNr, pta, paSiz, cv1, UT_DISP_cv);
  if(irc < 0) return -1;
*/

  //----------------------------------------------------------------
  // get pta = polygon 
  if((dbi > 0)&&(APT_obj_stat == 0)) {
    // CCV -> Polygon from polygonal representation;
    irc = PRCV_npt_dbo__ (&pta, &ptNr, Typ_CVTRM, dbi, WC_modact_ind);
    if(irc < 0) return -1;

  } else {

    // CCV -> Polygon - dynamic view only.
    // get space for points in tbuf1
    paSiz = (UME_ck_free (tbuf1) / sizeof(Point)) - 1;
    pta = UME_reserve (tbuf1, paSiz * sizeof(Point));
    // ptNr = 0;
    ptNr = paSiz; // 2017-09-26
    // CCV -> Polygon
    irc = UT3D_pta_ox_lim (&ptNr, pta, cv1, 0, NULL, UT_DISP_cv, APT_obj_stat);
      // printf("nach UT3D_pta_cvcomp %d %d\n",irc,ptNr);
    if(irc < 0) return -1;
  }


  //----------------------------------------------------------------
  // point only: Hilite.
  if(ptNr < 2) {
    GL_DrawPoint (dli, 0, pta);
    return 0;
  }


    // TESTBLOCK
    // printf(" _DrawCvCCV-irc=%d ptNr=%d\n",irc,ptNr);
    // UT3D_nstru_dump (Typ_PT, ptNr, pta, "CVPOL");
    // GR_Disp_npti (ptNr, pta, SYM_TRI_S, ATT_COL_RED, ATT_COL_YELLOW);
    // END TESTBLOCK


    // APT_dispDir=1; // TEST !

  if((!APT_dispDir)&&(*dli > 0)) {
    // display normal without direction-arrow
    // for CCV in statu nascendi ("S0=CCV .."): display also direction
    GL_DrawPoly (dli, Typ_Att_hili, ptNr, pta);

  } else {
    // disp curve with direction-arrow
    GL_Draw_cvp_dir (dli, Typ_Att_hili1, ptNr, pta);
  }

  UME_set_next (msPos, tbuf1);   // restore wrkSpc

  return 0;

}


/* old version 2014-06-19
//=====================================================================
  int GR_DrawCvCCV (long *ind, int att, ObjGX *cv1, Memspc *tbuf1) {
//=====================================================================

  int    irc, ptNr;
  Point  *pta;


  printf("GR_DrawCvCCV   %ld      XXXXXXXXXXXXXXXXXXXXXX\n",*ind);


  // pta  = (Point*)memspc101;
  // ptNr = sizeof(memspc101) / sizeof(Point);

  UME_connect (pta, tbuf1);
  ptNr = UME_ck_free (tbuf1) / sizeof(Point);


  // CCV -> Polygon
  irc = UT3D_pta_ox_lim (&ptNr, pta, cv1, 0, NULL, UT_DISP_cv);
  // printf("nach UT3D_pta_ox_lim %d %d\n",irc,ptNr);
  if(irc < 0) {TX_Error("GR_DrawCvCCV EOM"); return -1;}

  // nur Startpunkt: Hiliten.
  if(ptNr < 2) {
    GL_DrawPoint (ind, 0, pta);
    return 0;
  }

  GL_DrawPoly (ind, Typ_Att_hili, ptNr, pta);

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

//=====================================================================
  int GR_DrawCvCCV2 (long *ind, int att, ObjGX *cv1) {
//=====================================================================
/// NOT UP TO DATE: may have only LN2 & AC2

  int    irc, i1, ptNr;
  Point  *pta;
  Line2     *ln21;
  ObjG2     o21;
  ObjGX     *oTab, *ox2;


  // printf("GR_DrawCvCCV2\n");

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
        TX_Error("**** GR_DrawCvCCV2 form %d",ox2->form);
      }
    }


  return 0;

}

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
    if((segNr1 < 1)|(segNr1 >= ((CurvPoly*)obj)->ptNr)) return -1;
    GR_DrawPoly (ind, att, 2, &((CurvPoly*)obj)->cpTab[segNr1 - 1]);


  } else if(typ == Typ_CVTRM) {

    // get standard-curve from CCV-subSeg
    irc = UTO_cv_cvtrm (&sTyp, sObj, NULL, &((CurvCCV*)obj)[segNr1 - 1]);
    if(irc < 0) return -1;
      UT3D_stru_dump (sTyp, sObj, " CCVsubSeg %d",segNr1);

    if(segNr2 < 1) {
      // display curve
      GR_Draw_obj (ind, sTyp, sObj, 1, att, 0);
    } else {
      // recurse for line in polygon in ccv
      GR_Draw_subCurv (ind, att, sTyp, sObj, segNr2, 0);
    }

  } else return -2;


  return 0;

}
*/


//=====================================================================
  int  GR_DrawCvPol (long *ind, int attInd, CurvPoly *plg1) {
//=====================================================================

  int    ptNr;
  Point  *pTab;
  MemTab(Point) mtPt = _MEMTAB_NUL;


  // printf("GR_DrawCvPol ptNr=%d\n",plg1->ptNr);


  ptNr = plg1->ptNr + 4;

  // get memspc until end-of-func (alloca|malloc)
  MEMTAB_tmpSpc_get (&mtPt, ptNr);
  pTab = mtPt.data;


  // PolygonCurve -> relimited Polygon
  UT3D_pta_plg (&ptNr, pTab, plg1);


  //erhaelt das Polygon als fertiges Array (mit Screenkoords)
  GL_DrawPoly (ind, attInd, ptNr, pTab);


  // disp direction (last-point, point before)
  if(APT_dispDir) {
    APT_disp_dir (&pTab[ptNr - 1], &pTab[ptNr - 2]);
  }

  // free memspace
  MEMTAB_tmpSpc_free (&mtPt);

  return 0;

}


//=====================================================================
  int GR_DrawCvRBSp (long *ind, int attInd, CurvRBSpl *cv1) {
//=====================================================================
/// \code
/// GR_DrawCvRBSp               display rational-Bspline-curve
/// att      see ~/gCAD3D/cfg/ltyp.rc
/// \endcode


  int    irc, ptNr, ptMax;
  // double us, ue;
  Point  *pTab;


  // printf("GR_DrawCvRBSp\n");
  // UT3D_stru_dump (Typ_CVRBSP, cv1, "GR_DrawCvRBSp:\n");


  pTab = (void*) memspc55;
  ptMax = sizeof(memspc55) / sizeof(Point);


  // Toleranz:
  irc = UT3D_cv_rbsp (&ptNr, pTab, NULL, cv1, ptMax, UT_DISP_cv);
  if(irc < 0) return -1;


  //erhaelt das Polygon als fertiges Array (mit Screenkoords)
  GL_DrawPoly (ind, attInd, ptNr, pTab);


  // disp direction
  if(APT_dispDir) {
    int   ipe;
    ipe = ptNr - 1;
    APT_disp_dir (&pTab[ipe], &pTab[ipe - 1]);
  }


  return 0;

}


//=====================================================================
  void GR_DrawCvPpsp3 (long *ind, int attInd, ObjGX *cv1, double zval) {
//=====================================================================

  int    irc, ptNr, ptMax;
  Point  *pTab;


  // printf("GR_DrawCvPpsp3  %d %d\n",*ind,cv1->siz);
  // UTO_dump__ (cv1, "GR_DrawCvPpsp3");
  

  pTab = (void*) memspc55;
  ptMax = sizeof(memspc55) / sizeof(Point);

  // Spline -> Polygon   Toleranz
  irc = pspl_pol_psp (&ptNr, pTab, cv1, ptMax, UT_DISP_cv);
  if(irc < 0) {
    TX_Error("GR_DrawCvPpsp3 E001");
    return;
  }


  // erhaelt das Polygon als fertiges Array
  GL_DrawPoly (ind, attInd, ptNr, pTab);

  // exit(0);
  return;

}


/*
//=====================================================================
  void GR_DrawCurv (long *ind, int attInd, PointTab *cv1, double zval) {
//=====================================================================

  // printf("GR_DrawCurv %d\n",cv1->ptNr);

  //erhaelt das Polygon als fertiges Array (mit Screenkoords)
  GL_DrawPoly (ind, attInd, cv1->ptNr, cv1->ptTab);

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
  GL_DrawPoly (ind, attInd, ptNr, GL_ptArr30);


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

// TEMP-DISP-FUNCTIONS:
// see also UI_disp_vec1 UI_disp_vec2



//===================================================================
  int GR_tmpDisp_ln (Line *ln, int att) {
//===================================================================
// att: 1=sw, 2=rot 4=bl

  long   dli;


  printf("GR_tmpDisp_ln %d\n",att);

  // GL_temp_del_all (); // alle temp. obj loeschen ..

  dli = -1;  // als temp. obj anlegen ..

  GL_DrawPoly (&dli, att, 2, (Point*)ln);

  DL_Redraw ();

  return 0;

}










// QUICK-DISPLAY-FUNCTIONS:

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


//===================================================================
  int GR_Disp_txi (Point *pt1, int ii, int att) {
//===================================================================
/// \code
/// GR_Disp_txi       display integer at position
///   att     colorIndex; eg ATT_COL_RED; see INF_COL_SYMB
///
/// see also GR_Disp_message GR_Disp_npti
/// \endcode

  char cbuf[32];

  if(DispMode) UT3D_stru_dump(Typ_PT, pt1, "GR_Disp_txi: |%d|",ii);

  sprintf(cbuf,"%d",ii);

  APT_disp_TxtA (att, pt1, cbuf);

  return 0;

}


//===================================================================
  int GR_Disp_txi2 (Point2 *pt1, int ii, int att) {
//===================================================================
/// \code
/// GR_Disp_txi2       Testdisplay integer at 2D-point
/// att: see GR_Disp_tx
/// \endcode

  char    cbuf[32];
  Point   pt3;

  if(DispMode) UT3D_stru_dump(Typ_PT2, pt1, "GR_Disp_txi2: |%d|",ii);

  sprintf(cbuf,"%d",ii);

  pt3 = UT3D_pt_pt2 (pt1);

  APT_disp_TxtA (att, &pt3, cbuf);

  return 0;

}


//===================================================================
  int GR_Disp_tx2 (Point2 *p2, char *txt, int att) {
//===================================================================
/// \code
/// Testdisplay Text
/// att: see GR_Disp_tx
/// \endcode

  Point   p3;

  p3 = UT3D_pt_pt2(p2);

  APT_disp_TxtA (att, &p3, txt);

  return 0;

}


//===================================================================
  int GR_Disp_tx (Point *pt1, char *txt, int att) {
//===================================================================
/// \code
/// Testdisplay Text
///   att     colorIndex; eg ATT_COL_RED; see INF_COL_SYMB
/// \endcode

  UT3D_stru_dump(Typ_PT, pt1, "GR_Disp_tx: |%s|",txt);

  APT_disp_TxtA (att, pt1, txt);

  return 0;

}


//===================================================================
  int GR_Disp_pt (Point *pt1, int typ, int att) {
//===================================================================
/// \code
/// Testdisplay 3D-Point
/// Input:
///   typ:  SYM_STAR_S (Sternderl)
///         SYM_TRI_S (Dreieck)
///         SYM_TRI_B (Viereck)
///   att:  colorIndex; eg ATT_COL_RED; see INF_COL_SYMB
/// \endcode

/// see also GR_Disp_pTab

  // if(DispMode) UT3D_stru_dump(Typ_PT, pt1, "GR_Disp_pt:");

  APT_disp_SymB (typ, att, pt1);

  return 0;

}


//===================================================================
  int GR_Disp_pt2 (Point2 *pt1, int typ, int att) {
//===================================================================
/// \code
/// Testdisplay 2D-Point als Symbol
/// typ: SYM_STAR_S (Sternderl) SYM_TRI_S (Dreieck) SYM_TRI_B (Viereck)
/// att: 1=sw, 2=rot 4=bl
/// \endcode

  Point p31;

  if(DispMode) UT3D_stru_dump(Typ_PT2, pt1, "GR_Disp_pt2:");

  p31=UT3D_pt_pt2(pt1);

  APT_disp_SymB (typ, att, &p31);

  return 0;

}


//=======================================================================
  int GR_Disp_4db (double x1, double x2, double y1, double y2, int att) {
//=======================================================================
/// \code
/// make 2D-Box from 4 doubles
/// att: GR_Disp_ln2
/// \endcode


  Point2     pa[5];


  printf("GR_Disp_4db \n");

  pa[0].x = x1;
  pa[0].y = y1;

  pa[1].x = x2;
  pa[1].y = y1;

  pa[2].x = x2;
  pa[2].y = y2;

  pa[3].x = x1;
  pa[3].y = y2;

  pa[4] = pa[0];

  GR_Disp_cv2 (pa, 5, att);

  return 0;

}
 

//=====================================================================
  int GR_Disp_npti (int ptNr, Point *pTab, int typ, int ptAtt, int txAtt) {
//=====================================================================
/// \code
/// Testdisplay ptNr points and point-numbers
/// typ: see GR_Draw_obj/GR_Draw_obj/Typ_SymB
///      SYM_TRI_S|SYM_STAR_S|SYM_CIR_S|SYM_TRI_B
/// ptAtt: color for point-symbols; see GR_Draw_obj/Typ_SymB
///      ATT_COL_BLACK|ATT_COL_RED|ATT_COL_GREEN|ATT_COL_BLUE|..
/// txAtt: color for point-numbers; see GR_Draw_obj/Typ_SymB
///
/// draw polygon with GR_Disp_cv
/// \endcode

  int   i1;

  GR_Disp_pTab (ptNr, pTab, typ, ptAtt);
  for(i1 = 0; i1 < ptNr; ++i1)
    GR_Disp_txi (&pTab[i1], i1, txAtt);

  return 0;

}


//===================================================================
  int GR_Disp_pTab (int ptNr, Point *pTab, int typ, int att) {
//===================================================================
/// \code
/// Testdisplay ptNr points with bitmap-symbols (star, triangle, ..)
/// typ: see GR_Draw_obj/GR_Draw_obj/Typ_SymB
///      SYM_TRI_S|SYM_STAR_S|SYM_CIR_S|SYM_TRI_B
/// att: see INF_COL_SYMB       GR_Draw_obj/Typ_SymB
///      ATT_COL_BLACK|ATT_COL_RED|ATT_COL_GREEN|ATT_COL_BLUE|..
///
/// see also:
///   GR_Disp_cv            draw polygon
///   GR_Disp_npti          display points and integers along table of points
/// \endcode

// #include "../ut/func_types.h"               // SYM_..

  int   i1;

  printf("GR_Disp_pTab ptNr=%d typ=%d att=%d\n", ptNr,typ,att);

  for(i1=0; i1<ptNr; ++i1) {
    if(DispMode) UT3D_stru_dump(Typ_PT, &pTab[i1], "  pt[%d]=",i1);
    APT_disp_SymB (typ, att, &pTab[i1]);

    // NUR TEST:
    // { char cbuf[32];
      // sprintf(cbuf,"%d",i1);
      // APT_disp_TxtA (att, &pTab[i1], cbuf);
    // }
  }

  return 0;

}


//===================================================================
  int GR_Disp_p2Tab (int ptNr, Point2 *p2Tab, int typ, int att) {
//===================================================================
/// \code
/// Testdisplay Points als kleines 3-Eck
/// typ: SYM_STAR_S (Sternderl) SYM_TRI_S (Dreieck) SYM_TRI_B (grosses Viereck)
/// att:                 see ~/gCAD3D/cfg/ltyp.rc
/// \endcode

  int   i1;
  Point p31;

  printf("GR_Disp_p2Tab %d\n", ptNr);

  for(i1=0; i1<ptNr; ++i1) {
    // UT3D_stru_dump (Typ_PT2, &p2Tab[i1], "P2[%d]=",i1);
    p31=UT3D_pt_pt2(&p2Tab[i1]);
    APT_disp_SymB (typ, att, &p31);
  }

  return 0;

}


//===================================================================
  int GR_Disp_vc2 (Vector2 *vc1, Point2 *pt1, int att, int mode) {
//===================================================================
/// \code
/// GR_Disp_vc2     display 2D-Vector; length true or normalized
/// att:                 see ~/gCAD3D/cfg/ltyp.rc
/// mode = 0: immer gleich lang ("normiert")
/// mode = 1: genaue Groesse des Vec darstellen.
/// \endcode


  long   dli;
  Vector v31;
  Point  p31;


  if(DispMode) printf("GR_Disp_vc2 %f,%f P=%f,%f\n",vc1->dx,vc1->dy, pt1->x,pt1->y);

  p31 = UT3D_pt_pt2 (pt1);
  UT3D_vc_vc2 (&v31, vc1);

  dli = DL_StoreObj (Typ_VC2, -1L, 2);

  return GR_Draw_vc (&dli, &v31, &p31, att, mode);

}


//===================================================================
  int GR_Disp_vc (Vector *vc1, Point *pt1, int att, int mode) {
//===================================================================
/// \code
/// GR_Disp_vc      display 3D-Vector; length true or normalized
/// Input:
///   pt1      position fo vector; NULL = display at center of screen
///   att       see INF_COL_CV
///   mode      0: vector has always same lenght
///             1: vector has exact size (length)
///
/// see also GR_Disp_vc2
/// \endcode

// see also IE_cad_Inp_disp_vc UI_disp_vec1

  
  long  dli;

  if(!vc1) {printf("**** GR_Disp_vc E001\n"); return -1;}

  if(DispMode) printf("GR_Disp_vc %f %f %f\n",vc1->dx,vc1->dy,vc1->dz);

  dli=DL_StoreObj (Typ_VC, -1L, 2);

  return GR_Draw_vc (&dli, vc1, pt1, att, mode);

}


//================================================================
  int GR_Disp_ln2 (Point2 *p1, Point2 *p2, int att) {
//================================================================
/// \code
/// att: linetype; see INF_COL_CV
///      att is an index to a linetyp in file ~/gCAD3D/cfg/ltyp.rc.
///      linetyp has color, pattern thickness.
/// \endcode

  Point   pa[2];

  pa[0] = UT3D_pt_pt2(p1);
  pa[1] = UT3D_pt_pt2(p2);

  return GR_Disp_cv (pa, 2, att);

}


//================================================================
  int GR_Disp_ln (Line *ln1, int att) {
//================================================================
/// att                 see ~/gCAD3D/cfg/ltyp.rc

  return GR_Disp_cv ((void*)ln1, 2, att);

}


//================================================================
  int GR_Disp_ln1 (Point *p1, Point *p2, int att) {
//================================================================
/// att                 see ~/gCAD3D/cfg/ltyp.rc

  Point   pa[2];

  pa[0] = *p1;
  pa[1] = *p2;

  return GR_Disp_cv (pa, 2, att);

}


//================================================================
  int GR_Disp_ac (Circ *ci1, int att) {
//================================================================
/// att        see ~/gCAD3D/cfg/ltyp.rc


  long dli;
  int  i1, ianz = 1024, ptAnz;


  // if(DispMode) UT3D_stru_dump (Typ_CI, ci1, "GR_Disp_ac:\n");

  // GR_Disp_pt (&ci1->p1, SYM_STAR_S, 0);
  // GR_Disp_pt (&ci1->p2, SYM_STAR_S, 0);
  // GR_Disp_pt (&ci1->pc, SYM_STAR_S, 0);
  // GR_Disp_vc (&ci1->vz, &ci1->pc, 2, 0);



  if(GLT_pta_SIZ < ianz) {
    i1 = GLT_alloc_pta (ianz);
    if(i1 == -1) return i1;
  }


  // circ -> Polygon (see GR_DrawCirc ..)
  UT3D_cv_ci (GLT_pta, &ptAnz, ci1, ianz, UT_DISP_cv);
  // GR_Disp_pTab (ptAnz, GL_ptArr30, SYM_STAR_S, 2);


  dli=DL_StoreObj (Typ_CI, -1L, 2);
  GL_DrawPoly (&dli, att, ptAnz, GLT_pta);


  return 0;

}


//================================================================
  int GR_Disp_ccv (CurvCCV *ccv, int att, Memspc *tbuf1) {
//================================================================
/// \code
/// Testdisplay ConcatenatedCurVe
/// att                 see ~/gCAD3D/cfg/ltyp.rc
/// \endcode

// see GR_DrawCvCCV GR_Disp_pol

  int    irc, ptNr;
  Point  *pta;
  long   dli;
    
    
  // if(DispMode) printf("GR_Disp_ccv %d\n",ccv->siz);
  // printf("GR_DrawCvCCV\n");

  // pta  = (Point*)memspc101;
  // ptNr = sizeof(memspc101) / sizeof(Point);

  UME_connect (pta, tbuf1); 
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
  
  GL_DrawPoly (&dli, att, ptNr, pta);

  return 0;
 
}


//===================================================================
  int GR_Disp_triv (Triangle *tr, int att, int tNr, int ipNr) {
//===================================================================
/// \code
/// display Triangle-Normalvektor & number of triangle
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
    // GR_Disp_pt (&p1, SYM_STAR_S, 4);


  if(tNr >= 0) GR_Disp_txi (&p1, tNr, 0);


  // Normalvektor auf 3Eck
  UT3D_vc_perp3pt (&v1,tr->pa[0],tr->pa[1],tr->pa[2]);
  GR_Disp_vc (&v1, &p1, att, 0);

  if(ipNr >= 0) {
    GR_Disp_txi (tr->pa[0], 0, 0);
    GR_Disp_txi (tr->pa[1], 1, 0);
    GR_Disp_txi (tr->pa[2], 2, 0);
  }


  return 0;

}


//================================================================
  int GR_Disp_cv3p2 (Point2 *p1, Point2 *p2, Point2 *p3, int att) {
//================================================================
/// \code
/// GR_Disp_cv3p2         Display boundary of triangle from 3 2D-points
/// att: GR_Disp_ln2
/// 
/// see also GR_Disp_triv GR_Disp_tris
/// \endcode

  int    i1;
  Point2  pa[4];

  pa[0] = *p1;
  pa[1] = *p2;
  pa[2] = *p3;
  pa[3] = *p1;
    // GR_Disp_pt (tria->pa[0], SYM_STAR_S, 2);
    // GR_Disp_pt (&pa[0], SYM_STAR_S, 2);
    // GR_Disp_pt (&pa[1], SYM_STAR_S, 2);

  // Flaeche immer defaultfarbe; somit nicht sichtbar !
  GL_Col__ (10);
  // GR_DrawFan(tria->pa[0], 3, pa, 0);

  GR_Disp_cv2 (pa, 4, att);

  return 0;

}


//================================================================
  int GR_Disp_tria (Triangle *tria, int att) {
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
    // GR_Disp_pt (tria->pa[0], SYM_STAR_S, 2);
    // GR_Disp_pt (&pa[0], SYM_STAR_S, 2);
    // GR_Disp_pt (&pa[1], SYM_STAR_S, 2);

  // Flaeche immer defaultfarbe; somit nicht sichtbar !
  // GL_Col__ (10);
  // GR_DrawFan(tria->pa[0], 3, pa, 0);

  GR_Disp_cv (pa, 4, att);

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
  int GR_Disp_tris (Triangle *tria, int icol) {
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
  int GR_Draw_rect1 (long *dli,
                     Point *p1, Vector *vx, double dx, Vector *vy, double dy,
                     int att) {
//=============================================================================
// att:                 see ~/gCAD3D/cfg/ltyp.rc

  Point  pta[5];


  UT3D_stru_dump(Typ_PT, p1, "GR_Draw_rect1 p1");
  UT3D_stru_dump(Typ_VC, vx, "GR_Draw_rect1 vx");
  UT3D_stru_dump(Typ_VC, vy, "GR_Draw_rect1 vy");


  pta[0] = *p1;
  UT3D_pt_traptvclen (&pta[1], p1, vx, dx);
  UT3D_pt_traptvclen (&pta[2], &pta[1], vy, dy);
  UT3D_pt_traptvclen (&pta[3], p1, vy, dy);
  pta[4] = *p1;

  GL_DrawPoly (dli, att, 5, pta);


  return 0;

}
  

//=============================================================================
  int GR_Disp_rect1 (Point *p1, Vector *vx, double dx, Vector *vy, double dy,
                     int att) {
//=============================================================================
// att:                 see ~/gCAD3D/cfg/ltyp.rc

  long   dli;


  // UT3D_stru_dump(Typ_PT, p1, "GR_Disp_rect1 p1");
  // UT3D_stru_dump(Typ_VC, vx, "GR_Disp_rect1 vx");
  // UT3D_stru_dump(Typ_VC, vy, "GR_Disp_rect1 vy");


  dli = DL_StoreObj (Typ_CVPOL, -1L, 2);

  GR_Draw_rect1 (&dli, p1, vx, dx, vy, dy, att);


  return 0;

}


//===================================================================
  int GR_Disp_cv (Point *pta, int ptnr, int att) {
//===================================================================
/// \code
/// Testdisplay Polygon
/// att: GR_Disp_ln2     see ~/gCAD3D/cfg/ltyp.rc
/// see also:
///   GR_Disp_pTab          display table of points with bitmap-symbols
///   GR_Disp_npti          display points and integers along table of points
/// \endcode

  int  i1;
  long dli;

/*
  if(DispMode) {
    printf("GR_Disp_cv %d\n",ptnr);
    for(i1=0; i1<ptnr; ++i1)
      UT3D_stru_dump(Typ_PT, &pta[i1], " p[%d]",i1);
  }
*/

  dli=DL_StoreObj (Typ_CVPOL, -1L, 2);
  GL_DrawPoly (&dli, att, ptnr, pta);

  return 0;

}




//===================================================================
  int GR_Disp_cv2 (Point2 *pta, int ptnr, int att) {
//===================================================================
/// \code
/// Testdisplay 2D-Polygon
/// att: GR_Disp_ln2
/// \endcode

  int  i1;
  long dli;

/*
  if(DispMode) {
    printf("GR_Disp_cv2 %d\n",ptnr);
    for(i1=0; i1<ptnr; ++i1)
      UT3D_stru_dump(Typ_PT2, &pta[i1], " p[%d]",i1);
  }
*/

  dli=DL_StoreObj (Typ_CVPOL, -1L, 2);
  GL_DrawPoly2D (&dli, att, ptnr, pta, 0.);

  return 0;

}


//=========================================================
  int GR_Disp_pln (Plane *pl1, int att) {
//=========================================================
/// \code
/// GR_DrawPlane
/// GR_Disp_pln (&pln1, 2);
/// att          see ~/gCAD3D/cfg/ltyp.rc
/// \endcode

  long dli;

  if(DispMode) UT3D_stru_dump (Typ_PLN, pl1, "GR_Disp_pln");
  // GR_Disp_pt (&pl1->po, SYM_TRI_S, 2);
  // GR_Disp_vc (&pl1->vz, &pl1->po, 2, 0);

  dli = DL_StoreObj (Typ_PLN, -1L, 2);

  GL_DrawSymV3 (&dli, SYM_SQUARE, att, &pl1->po, &pl1->vz, 1.);

  return 0;

}


//=========================================================
  int GR_Disp_axis (Plane *pl1, int att, int typ) {
//=========================================================
/// \code
/// display axisSystem with x,y,z-characters
/// typ: 1=Plane; 2=Axis; 4=Plane+Axis+Chars
/// GR_Disp_axis (&pln1, 2);
/// att          see ~/gCAD3D/cfg/ltyp.rc
/// see APT_disp_SymV1 (SYM_AXIS,   1, &pln1.po, 1.);
/// \endcode

  long dli;

  // if(DispMode) UT3D_stru_dump (Typ_PLN, pl1, "GR_Disp_axis");
  // GR_Disp_pt (&pl1->po, SYM_TRI_S, 2);
  // GR_Disp_vc (&pl1->vz, &pl1->po, 2, 0);

  // dli = DL_StoreObj (Typ_PLN, -1L, 2);
  dli = -2;  // index for first CAD-inputfield

  GL_DrawSymVX (&dli, att, pl1, typ, 1.);

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
  // iatt = Typ_Att_Fac1;
  GL_Surf_Ini (&dli, NULL); //(void*)&iatt);  // glNewList (for gTyp=4,5,6,9)


  if(icol >= 0)
    GL_Col__ (icol);  // glColor3fv (GL_col_tab[icol]); 2=red,3=green,5=yellow..


  GL_Disp_patch (gTyp, pNr, pa);

  // close GL-dispList
  GL_EndList ();                  // glEndList ();

  return 0;

}


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

  GL_Disp_ipatch (gTyp, iNr, ia, pa);

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

  GL_Disp_ipatch (gTyp, iNr, ia, pa);

  // close GL-dispList
  GL_EndList ();                  // glEndList ();

  return 0;

}


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


  dli=DL_StoreObj (Typ_QFac, -1L, att);

  GL_Draw_Ini (&dli, att);
  GL_DrawStrip1 (ps, pa1, pa2, 1);
  GL_EndList ();
  
  // Ausgabe der Dreiecke als TriaFans via GL.
  // GL_DrawFace (ptNr, pTab, ptiNr, pti, 1);

  // GR_DrawTriaStrip (&dli, att, ptUNr, ptVNr, pTab);
  // od GR_CreTriaStrip

  return 0;

}


//===================================================================
  int GR_Disp_rbez (CurvRBez *rbez, int att) {
//===================================================================
/// \code
/// Testdisplay  rational-bezier-curve via rational-B-spline
/// att: see GR_Draw_obj
/// see also GR_Disp_rbspl GL_Disp_rbez
/// \endcode

  // int      irc, ptNr, ptMax;
  // long     dli;
  // Point    *pTab;
  // void     *memstart;
  int       ii;
  CurvRBSpl crbsp;
  Memspc    tmpSpc=UME_NEW, *memSeg1;
    

  // UT3D_stru_dump (Typ_CVRBEZ, rbez, "GR_Disp_rbez ptNr=%d va=%f vb=%f\n",
                  // rbez->ptNr,rbez->va,rbez->vb);

  GR_Disp_pTab (rbez->ptNr, rbez->cptab, SYM_STAR_S, 2);
    


  // get tempspace (gets lost on func-return)
  ii = 32 + rbez->ptNr * sizeof(Point) * 2;
  // UME_alloc_tmp (&tmpSpc, ii);
  UME_malloc (&tmpSpc, ii, ii/2);
  memSeg1 = &tmpSpc;


  // make rbspl from rbez
  UT3D_rbsp_rbez (&crbsp, rbez, memSeg1);
    // UT3D_stru_dump (Typ_CVRBSP, &crbsp, " rbsp");

  // display rBspl
  ii = GR_Disp_rbspl (&crbsp, att);

  UME_free (&tmpSpc);

  return ii;

/*
  // memstart = workSeg->next;
  pTab = memSeg1->next;
  ptMax = UME_ck_free(memSeg1) / sizeof(Point);
    printf("GR_Disp_rbez Pts.free=%d\n",ptMax);

  dli = DL_StoreObj (Typ_CVRBEZ, -1L, 2);

  // rbez -> Polygon
  // irc = UT3D_pta_rbez (&ptNr, pTab, rbez, ptMax, UT_DISP_cv);

  if(irc >= 0) GL_DrawPoly (&dli, att, ptNr, pTab);
*/

  return 0;

}


//===================================================================
  int GR_Disp_bez (CurvBez *bez, int att, Memspc *workSeg) {
//===================================================================
/// \code
/// Testdisplay  Bezier-Curve
/// att: 2=rot, voll, dick
/// \endcode

  int   irc, ptNr, ptMax;
  long  dli;
  Point *pTab;
  void  *memstart;


  // UT3D_stru_dump (Typ_CVBEZ, bez, "GR_Disp_bez ptNr=%d va=%f vb=%f\n",
                  // bez->ptNr,bez->va,bez->vb);
  // GR_Disp_pTab (bez->ptNr, bez->cptab, SYM_STAR_S, 2);



  memstart = workSeg->next;
  pTab = workSeg->next;
  ptMax = UME_ck_free(workSeg) / sizeof(Point);
    // printf("GR_Disp_bez Pts.free=%d\n",ptMax);



  dli=DL_StoreObj (Typ_CVBEZ, -1L, 2);

  // bspl -> Polygon
  irc = UT3D_pta_bez (&ptNr, pTab, bez, ptMax, UT_DISP_cv);

  if(irc >= 0) GL_DrawPoly (&dli, att, ptNr, pTab);


  // release work space
  workSeg->next = memstart;


  return 0;

}


//===================================================================
  int GR_Disp_box (Point *p1, Point *p2, int att) {
//===================================================================
/// \code
/// GR_Disp_box         disp 3D-boundingBox from 2 points
/// att          see ~/gCAD3D/cfg/ltyp.rc
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
    printf("GR_Disp_box %d\n",att);
    UT3D_stru_dump (Typ_PT, p1, "");
    UT3D_stru_dump (Typ_PT, p2, "");
  }

  // GR_Disp_pt (p1, SYM_TRI_S, 0);
  // GR_Disp_pt (p2, SYM_TRI_S, 0);


  dli=DL_StoreObj (Typ_GridBox, -1L, 2);
  GL_Draw_Ini1 (&dli, att, 0);


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
  GL_Disp_cv (4, pta);

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
  GL_Disp_cv (4, pta);

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
  GL_Disp_cv (4, pta);

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
  GL_Disp_cv (4, pta);

  GL_EndList1 (0);


  return 0;

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
  // GR_Disp_pTab (pNr, pTab, SYM_TRI_S, 2);

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
      UT3D_stru_dump(Typ_PT, &pTab[i1], "  pt[%d]=",i1);
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

  // GL_DrawSur (&il, 5, oxi);
  GL_Draw_Ini (&dli, att);
  i1 = GL_Disp_sur (oxi);              // draw OpenGL
  GL_EndList ();

  return 0;

}

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
  // GR_Disp_pTab (pNr, pTab, SYM_TRI_S, 2);


  // Normalvektor mit manchmal falschem Umlaufsinn:
  UT3D_vc_perpcv (&vcn, pNr, pTab);

  // make ContTab from points
  SUP_ContTab_pTab (&ct1, pNr, pTab, &vcn);


  L_draw:
    GLT_start__ (&gSur);                  // new Surf
    i1 = GLT_spp_pp (&vcn);              // new Patch
    if(i1 == -10) goto L_draw;           // realloc
    i1 = GLT_spp__ (&ct1, 1, &vcn);       // tesselate



  // Ausgeben
  if(TSU_mode == 0) {
    dli = DL_StoreObj (Typ_SUR, *ind, att);
    GL_DrawSur (&dli, att, gSur);
    AP_dli_act = dli;

  } else {
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
  // GR_Disp_pTab (pNr, pTab, SYM_TRI_S, 2);


  // Normalvektor mit mnchmal falschem Umlaufsinn:
  UT3D_vc_perpcv (&vcz, pNr, pTab);

  dli=DL_StoreObj (Typ_SUR, *ind, att);

  GL_Draw_Ini (&dli, att);  // Flaechenfarbe
  GLT_spu_ (pNr, pTab, &vcz);
  GL_EndList ();

  return 0;
}
*/

//===================================================================
  int GR_Disp_sur (ObjGX *oxi, int att) {
//===================================================================
/// \code
/// Testdisplay  Surface
/// att: surface-color; see Col_set__ or AP_SetCol3i
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


//================================================================
  int GR_Disp_obj (int oTyp, void *obj, int att, int mode) {
//================================================================
/// \code
/// GR_Disp_obj     tempDisp obj from typ+struct
/// Input:
///   oTyp     type of sObj; eg. Typ_PT      see GR_Draw_obj
///   obj      data, struct; eg. (Point*)
///   att,mode                               see GR_Draw_obj
///
/// see also:
/// Point    GR_Disp_pt or APT_disp_SymB
/// Vectors: GR_Disp_vc
/// Text:    GR_Disp_tx
/// \endcode

// see GR_Disp_dbo GR_Disp_ox UTO_obj_Disp__ SUR_Disp_Sur

// es wird immer ein DL-record angelegt !

  long    dli;
  // ObjGX  ox;



  // OGX_SET_OBJ (&ox, oTyp, oTyp, 1, sObj);
  // GR_Disp_ox (&ox, att, mode);

  dli = DL_StoreObj (oTyp, -1L, att);

  printf("GR_Disp_obj typ=%d dli=%ld att=%d\n",oTyp,dli,att);

  return GR_Draw_obj (&dli, oTyp, obj, 1, att, mode);

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
      i1 = GL_Disp_sur (oxi);            // draw OpenGL
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

  // printf("GR_DrawFan %d\n",ptAnz);


  if(TSU_mode != 0) {
    GLT_stor_rec (0, NULL, NULL, 0);  // init

    if((GL_actCol.color != 0) || (GL_actCol.vtra != 0))  {
      GLT_stor_rec (5, NULL, NULL, 0);

    } else if (GL_actCol.vtex != 0) {
      // activate active TexRefNr
      GLT_stor_rec (8, NULL, NULL, GL_actTex);
    }


  }  else {
    // if((GL_actCol.color != 0) || (GL_actCol.vtra != 0))
    GL_ColSet (&GL_actCol);
  }

  GL_DrawFan (pt1, ptAnz, pa1, typ, newS);

  if(TSU_mode != 0)
    GLT_stor_rec (1, NULL, NULL, 0);  // save

  return 0;

}


//================================================================
  int GR_DrawStrip (Point *pa1, Point *pa2, int ptAnz, int newS) {
//================================================================
/// \code
/// newS:  0 = do not start new surface
/// newS:  1 = start new surface
/// \endcode


  // printf("GR_DrawStrip ptAnz=%d TSU_mode=%d\n",ptAnz,TSU_mode);
  // {int i1;for(i1=0;i1<ptAnz;++i1)
    // {printf("Strip[%d] %.2f,%.2f,%.2f - %.2f,%.2f,%.2f\n",
      // i1,pa1[i1].x,pa1[i1].y,pa1[i1].z,pa2[i1].x,pa2[i1].y,pa2[i1].z);}}
  // UT3D_stru_dump (Typ_Color, &GL_actCol, "  GL_actCol:");


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

  GL_DrawStrip2 (pa1, pa2, ptAnz, newS);

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
    // dli = DL_find_obj (Typ_SUR, dbi, -1L);
      // printf(" dbi=%ld dli=%ld\n",dbi,dli);
    DL_SetInd (dli);  // modify (do not create new DL-Record)

  // } else if(dli == -1L) {  // create new object
  // } else if(dli < 0) {   // create a temporary object
    // dli = -1L;

  }


  att = 0; // 5; // 8; // ? APT_DrawSur GR_DrawSur TSU_DrawSurMsh

  dli = DL_StoreObj (Typ_SUR, dbi, *(int*)col);



  // MSH_dump_fTab (ActFtab.data, ActFtab.rNr);


  L_disp:
  GL_Surf_Ini (&dli, col);           // att unused ..
  GL_ColSet (col);                   // 2015-09-30
  GL_Disp_nfac (pTab, fTab, fNr, col);
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

// see TSU_DrawSurMsh



  printf("GR_Disp_nfac fNr=%d TSU_mode=%d\n",fNr,TSU_mode);
  // UT3D_stru_dump (Typ_Color, &GL_actCol, "  GL_actCol:");


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

  GL_Disp_nfac (pTab, fTab, fNr, col);

  if(TSU_mode != 0)
    GLT_stor_rec (1, NULL, NULL, 0);  // save

  return;

}


//===================================================================
  int GR_Disp_CvBSp (CurvBSpl *bspl, int att) {
//===================================================================
/// \code
/// Testdisplay  BSP-Curve
/// att: see ~/gCAD3D/cfg/ltyp.rc
/// \endcode

  long  dli;


  printf("GR_Disp_CvBSp att=%d\n",att);


  dli = DL_StoreObj (Typ_CVBSP, -1L, 2);

  return GR_DrawCvBSp (&dli, -1L, att, bspl);

}


//====================================================================
  int GR_Disp_rbspl (CurvRBSpl *cv1, int att) {
//====================================================================
/// \code
/// GR_Disp_rbspl        testdisplay rational-Bspline-curve
/// att      see ~/gCAD3D/cfg/ltyp.rc
/// \endcode


  long  dli;


  // printf("GR_Disp_rbspl att=%d\n",att);

  dli = DL_StoreObj (Typ_CVRBSP, -1L, 2);

  return GR_DrawCvRBSp (&dli, att, cv1);


}


//=======================================================================
  int GR_DrawCvBSp (long *dli, long dbi, int att, CurvBSpl *bspl) {
//=======================================================================
// ATTENTION: is using memspc102
// see GR_DrawCurv GR_Disp_CvBSp

 
  int   irc, ptNr, ptMax;
  Point *pTab;
  // void  *memstart;


  // printf("GR_DrawCvBSp dbi=%ld ptNr=%d deg=%d dli=%ld\n",
         // dbi, bspl->ptNr, bspl->deg,*dli);
  // UT3D_stru_dump (Typ_CVBSP, bspl, "CV:\n");
  // GR_Disp_pTab (bspl->ptNr, bspl->cpTab, SYM_STAR_S, 2);



  if((dbi > 0)&&(*dli > 0L)&&(APT_obj_stat == 0)) {
    // bspl -> Polygon using PRCV
    irc = PRCV_npt_dbo__ (&pTab, &ptNr, Typ_CVBSP, dbi, WC_modact_ind);
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
  GL_DrawPoly (dli, att, ptNr, pTab);


  // disp direction
  if(APT_dispDir) {
    int   ipe;
    ipe = ptNr - 1;
    APT_disp_dir (&pTab[ipe], &pTab[ipe - 1]);
  }


  // // release work space
  // workSeg->next = memstart;


  return 0;

}


//================================================================
  int GR_Disp_pol (CurvPoly *plg1, int att) {
//================================================================
/// \code
/// Testdisplay Polygon
/// att                 see ~/gCAD3D/cfg/ltyp.rc
/// \endcode

  long  dli;

  if(DispMode) printf("GR_Disp_pol %d\n",plg1->ptNr);


  dli = DL_StoreObj (Typ_CVPOL, -1L, 2);

  return GR_DrawCvPol (&dli, att, plg1);

}


//================================================================
  int GR_Disp_ell (CurvElli *el1, int att) {
//================================================================
/// \code
/// Testdisplay Ellipse
/// att                 see ~/gCAD3D/cfg/ltyp.rc
/// \endcode
               
  long  dli;

  printf("GR_Disp_ell att=%d\n",att);


  dli = DL_StoreObj (Typ_CVELL, -1L, 2);

  return GR_DrawCvEll (&dli, att, el1);

}


//=======================================================================
  int GR_DrawCvEll (long *ind, int att, CurvElli *el1) {
//=======================================================================
// see GR_Disp_ell GR_DrawCvEll

  int     ptNr;
  Point   *pa;
    
    
  // UT3D_stru_dump (Typ_CVELL, el1, "GR_DrawCvEll");
  // GR_Disp_pt (&el1->pc, SYM_STAR_S, 0);
  // GR_Disp_pt (&el1->p1, SYM_TRI_S, ATT_COL_GREEN);
  // GR_Disp_pt (&el1->p2, SYM_TRI_S, ATT_COL_RED);
  // GR_Disp_vc (&el1->va, &el1->pc, 8, 1);  // green
  // GR_Disp_vc (&el1->vb, &el1->pc, 9, 1);  // red

    
 /*   
  if(GLT_pta_SIZ < ianz) {
    i1 = GLT_alloc_pta (ianz);
    if(i1 == -1) return i1;
  }
*/

  // nr of points for polygon
  ptNr = UT3D_ptNr_ell (el1, UT_DISP_cv);
    // printf(" ptNr-ell=%d\n",ptNr);


  pa = MEM_alloc_tmp ((int)(ptNr * sizeof(Point)));


  // if(ptNr > ptmax) goto L_e1;
  UT3D_npt_ell (ptNr, pa, el1);
    // UT3D_cv_ell (GLT_pta, &ptAnz, el1, ianz, UT_DISP_cv);
    // GR_Disp_pTab (ptNr, pa, SYM_STAR_S, 2);


  GL_DrawPoly (ind, att, ptNr, pa);


  // disp direction
  if(APT_dispDir) {
    int   ipe;
    ipe = ptNr - 1;
    APT_disp_dir (&pa[ipe], &pa[ipe - 1]);
  }


  return 0;

}


//================================================================
  int GR_Disp_ox (ObjGX *ox1, int att, int mode) {
//================================================================
/// \code
/// Input:
///   ox1 
///   att,mode see GR_Draw_obj
/// 
/// see GR_Draw_ox GR_Disp_obj GR_Disp_dbo GR_Draw_dbo
/// \endcode


  long dli;


  printf("GR_Disp_ox typ=%d form=%d att=%d\n",ox1->typ,ox1->form,att);


  dli = DL_StoreObj (ox1->typ, -1L, att);

  return GR_Draw_ox (&dli, ox1, att, mode);

}


//==========================================================================
  int GR_Draw_ox (long *ind, ObjGX *ox1, int att, int mode) {
//==========================================================================
/// \code
/// Input:
///   ind      DispListIndex; get it from DL_StoreObj
///   att      points: 0=default; 1=hili; 2=dimmed; 3=green; 4=yellow.
///   mode     points: unused
///   ox1 
/// 
/// see GR_Disp_ox GR_Disp_obj GR_Disp_dbo GR_Draw_dbo
///     APT_DrawCurv GR_DrawCurv
/// \endcode

// use *ind = -2 for temp.Obj (fixed temp index)


  int       oTyp, i1;
  long      dbi;
  ObjGX     *ox2, o2;


  printf("GR_Draw_ox ind=%ld att=%d typ=%d form=%d\n",*ind,att,
          ox1->typ,ox1->form);
  // UTO_dump__ (ox1, "GR_Draw_ox");


  oTyp = ox1->form;


  switch (oTyp) {

    //================================================================
    case Typ_Index:
      dbi = OGX_GET_INT (ox1);
      o2 = DB_GetObjGX (ox1->typ, dbi);
      GR_Draw_ox (ind, &o2, att, mode);
      break;


    case Typ_ObjGX:
      ox2 = ox1->data;
      for(i1=0; i1 < ox1->siz; ++i1) // Recursion !
        GR_Draw_ox (ind, &ox2[i1], att, mode);
      break;


    // //================================================================
    // case Typ_PT:
      // return GR_Draw_obj (


    default:
      goto L_E001;
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


//========================================================================
  int GR_Draw_vc (long *ind, Vector *vc1, Point *pt1, int att, int mode) {
//========================================================================
/// \code
/// GR_Draw_vc        display 3D-Vector; length true or normalized
/// Input:
///   pt1      position fo vector; NULL = display at center of screen
///   att       0,1=sw, 2=strichliert, 3=strichPunkt 5=gelb 7=blau 8=grün dick
///             9=rot dick  11=sw dick 12=rot dick strichl.
///   mode      0: vector has always same lenght
///             1: vector has exact size (length)
/// \endcode


  Point ptc;


  if(pt1 == NULL) {
    ptc = GL_GetCen();   // ScreenCenter in UserCoords
    pt1 = &ptc; 
  }


  if(mode == 0) {
    // immer gleich lang ("normiert")
    GL_DrawSymV3 (ind, SYM_ARROW, att, pt1, vc1, 10.);

  } else {
    // genaue Groesse des Vec darstellen
    GL_DrawVec (ind, att, pt1, vc1);
  }


  return 0;

}


//==============================================================================
  int GR_Draw_obj (long *dli, int form, void *obj, int oNr, int att, int mode) {
//==============================================================================
/// \code
/// Input:
///   dli      DispListIndex; get it from DL_StoreObj
///   form,obj obj to display;
///   oNr      nr of objects of type <typ> in obj
///   att      Typ_PT:  see INF_COL_PT
///              ATT_PT_BLACK|ATT_PT_HILI|ATT_PT_DIMMED|ATT_PT_GREEN|ATT_PT_YELLOW
///            Typ_SymB:   colorIndex; eg ATT_COL_RED; see INF_COL_SYMB
///              ATT_COL_BLACK|ATT_COL_RED|ATT_COL_GREEN|ATT_COL_BLUE|
///              ATT_COL_YELLOW|ATT_COL_MAGENTA|ATT_COL_CYAN|ATT_COL_WHITE
///              ATT_COL_HILI|ATT_COL_DIMMED
///            Typ_CI|Typ_CVBSP|Typ_PLN:   see INF_COL_CV
///   mode     Typ_PT|Typ_CI|Typ_CV*: unused;
///            Typ_SymB: SYM_TRI_S|SYM_STAR_S|SYM_CIR_S|SYM_TRI_B
///            Typ_PLN:  1=Plane; 2=Axis; 4=Axis+Chars; 5=Plane+Axis;
/// 
/// Vectors: GR_Disp_vc
/// Text:    GR_Disp_tx 
///
/// see GR_Disp_ox GR_Disp_obj GR_Disp_dbo GR_Draw_dbo
///     APT_DrawCurv GR_DrawCurv
/// \endcode

// use *dli = -2 for temp.Obj (fixed temp index)



  int       irc, oTyp, i1;
  long      dbi;
  void      *p1;
  ObjGX     *ox2, ogx;
  Memspc    tSpc1;
  void      *o1;
  Point     pt1;
  DL_Att    *att1;


  // printf("GR_Draw_obj dli=%ld att=%d form=%d\n",*dli,att,form);
  // UT3D_stru_dump (typ, obj, "  obj");


  // oTyp = typ;


  switch (form) {

    //================================================================
    case Typ_PT2:
      o1 = MEM_alloc_tmp ((int)sizeof(Point));
      *((Point*)o1) = UT3D_pt_pt2 (obj);
      GR_DrawPoint (dli, att, (Point*)o1);
      break;

    //================================================================
    case Typ_PT:
      // see APT_DrawPoint
      GR_DrawPoint (dli, att, (Point*)obj);
      break;

    //================================================================
    case Typ_VC:
      // normalized; see also UI_disp_vec1
      pt1 = GL_GetCen ();
      GL_DrawSymV3 (dli, SYM_ARROW, att, &pt1, (Vector*)obj, 10.);
      break;

    //================================================================
    case Typ_SymB:
      // see APT_disp_SymB GL_DrawSymB
      GL_DrawSymB (dli, att, mode, (Point*)obj);
      break;

    //================================================================
    case Typ_LN:
      GR_DrawLine (dli, att, (Line*)obj);
      break;

    //================================================================
    case Typ_CI:
      dbi = DL_get_dbi (*dli);
      GR_DrawCirc (dli, dbi, att, (Circ*)obj);
      break;


    //================================================================
    case Typ_CVELL:
      GR_DrawCvEll (dli, att, obj);  // see GR_Disp_ell GR_DrawCvEll
      break;


    //================================================================
    case Typ_CVPOL:     // CurvPoly
      GR_DrawCvPol (dli, att, obj); // GR_Disp_pol GR_DrawCvPol
      break;


    //================================================================
    case Typ_CVBSP:
      dbi = DL_get_dbi (*dli);
      GR_DrawCvBSp (dli, dbi, att, obj); // CurvBSpl
      break;

    //================================================================
    case Typ_CVTRM:
      UME_init (&tSpc1, memspc201, sizeof(memspc201));
      // GR_DrawCvCCV (dli, att, obj, &tSpc1);
      OGX_SET_OBJ (&ogx, Typ_CVTRM, Typ_CVTRM, oNr, obj);
      dbi = DL_get_dbi (*dli);
      GR_DrawCvCCV (dli, dbi, att, &ogx, &tSpc1);
      break;

    //================================================================
    case Typ_PLN:
      // GR_DrawPlane
      GL_DrawSymVX (dli, att, obj, mode, 1.);
      break;

    //================================================================
    case Typ_SURRV:
      // TSU_DrawSurTRV sollte noch in Disp & Draw geteilt werden !
      TSU_DrawSurTRV (obj, att, -1L);
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
    // case Typ_Index:
      // dbi = OGX_GET_INT (ox1);
      // o2 = DB_GetObjGX (ox1->typ, dbi);
      // GR_Draw_ox (dli, &o2, att, mode);
      // break;


    //================================================================
    case Typ_ObjGX:
      return GR_Draw_ox (dli, obj, att, mode);
/*
      // typ = AP_typ_2_bastyp (typ);
    case Typ_CV:
      ox2 = (ObjGX*)obj;

      //----------------------------------------------------------------
      if(typ == Typ_SUR) {
        // SUR_Disp_Sur (ox1, wrkSpc, att, p1);
        TX_Print("**** GR_Draw_obj I1");

      //----------------------------------------------------------------
      } else {
        // Typ_CV
        for(i1=0; i1 < ox2->siz; ++i1) // Recursion !
          // GR_Draw_ox (ind, &ox2[i1], att, mode);
          // recurse
          GR_Draw_obj (ind, ox2[i1].form,  ox2[i1].data, 1, att, mode);
      }
      break;
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
      TX_Error("GR_Draw_obj not yet impl: %d",form);
      return -1;

  L_E002:
      TX_Error("GR_Draw_obj E002"); // par fehlt ..
      return -1;

  // L_E003:
      // TX_Error("GR_Draw_ox E003"); // wrkSpc fehlt ..
      // return -1;

}


//================================================================
  int GR_Disp_dbo (int typ, long dbi, int att, int mode) {
//================================================================
// att, mode: see GR_Draw_obj

  long dli;


  // printf("GR_Disp_dbo typ=%d dbi=%ld att=%d\n",typ,dbi,att);


  dli = DL_StoreObj (typ, -1L, att);

  return GR_Draw_dbo (&dli, typ, dbi, att, mode);

}

 
//=========================================================================
  int GR_Draw_dbo (long *dli, int typ, long dbi, int att, int mode) {
//=========================================================================
// att, mode: see GR_Draw_obj
// see also GR_Disp_dbo GR_Disp_obj UTO_obj_Disp__

  // int        ii;
  // char       o1[OBJ_SIZ_MAX];
  // ObjGX     ox1;
  // Memspc    tmpSeg;

  int       form, oNr;
  void      *data;


  // printf("GR_Draw_dbo dli=%ld typ=%d dbi=%ld\n",*dli,typ,dbi);

  // make obj of DB-obj
  // OGX_SET_INDEX (&ox1, typ, dbi);
  // draw obj
  // return GR_Draw_ox (dli, &ox1, att, mode);

  form = DB_GetObjDat (&data, &oNr, typ, dbi);
  if(form < 1) return -1;
    // UT3D_stru_dump (form, data, " _Draw_dbo");

  return GR_Draw_obj (dli, form, data, oNr, att, mode);

}
 

//==============================================================================
  int GR_Disp_iSur (int iAtt,
                    MemTab(IndTab) *fmt, MemTab(int) *imt, MemTab(Point) *pmt) {
//==============================================================================
// GR_Disp_iSur    display tesselated faces (using fmt pmt imt)

  long dli;

  dli = DL_StoreObj (Typ_SUR, -1L, iAtt);

  return GR_Draw_iSur (&dli, iAtt, fmt, imt, pmt);

}


//================================================================
  int GR_Draw_iSur (long *dli, int iAtt,
                    MemTab(IndTab) *fmt,
                    MemTab(int)    *imt,
                    MemTab(Point)  *pmt) {
//================================================================
// GR_Draw_iSur    display tesselated faces (using fmt pmt imt)
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

  // printf("GR_Draw_iSur %d\n",fNr);
  // UT3D_stru_dump (Typ_Color, &iAtt, "sur-iAtt");


  // init GL-dispList
  // GL_Surf_Ini (dli, NULL); //(void*)&iatt);  // glNewList (for gTyp=4,5,6,9)
  GL_Surf_Ini (dli, (void*)&iAtt);  // glNewList (for gTyp=4,5,6,9)


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
    // GR_Disp_ipatch (it->aux, iCol, it->iNr, ia, pa);    <<<<<<<<<<
    GL_Disp_ipatch (it->aux, it->iNr, ia, pa);

  }

  // close GL-dispList
  GL_EndList ();                  // glEndList ();

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
//   ipt          see retCod
//   pa1          parameter of pts on line-segment, see retCod
//   retCod       0=OK, point pta[ipt] selected
//                1=OK, point between pta[ipt] and pta[ipt+1]; offset is pa1
//               -1=ptAct_not_on_polygon
 

// see UT3D_segpar_sel_LN GL_ck_sel_PT GL_selPt

  int        irc, i0, ii, i1;
  double     tol = GL_pickSiz, dx, dy, dp, dMax, p1;
  Point      p2Act, *p2a;
  Point2     pb1, pb2, p2s;


  // printf("----------------- GR_pt_par_sel_npt %d\n",ptNr);
  // for(i1=0; i1<ptNr; ++i1) UT3D_stru_dump (Typ_PT, &pta[i1],"p[%d]",i1);

  tol = GL_pickSiz,

  // get ScreenCoords of ptAct
  GL_ptSc_ptUc (&p2Act, ptAct);
    // UT3D_stru_dump (Typ_PT, &p2Act, " p2Act");

  // get memspc for p2a = ptNr 2D-points
  p2a = (Point*)MEM_alloc_tmp ((int)(sizeof(Point) * ptNr));

  // loop tru points, get 2D-coords pf point, test if 2D-point == ptAct-2D
  dMax = tol;
  ii = -1;
  for(i1=0; i1<ptNr; ++i1) {
    // get ScreenCoords 
    GL_ptSc_ptUc (&p2a[i1], &pta[i1]);
      // UT3D_stru_dump (Typ_PT, &p2a[i1], " p2a[%d]",i1);
      // GR_Disp_pt (&pta[i1], SYM_TRI_S, ATT_COL_YELLOW);

    // get approx distance 
    dp = UT2D_lenB_2pt ((Point2*)&p2a[i1],(Point2*)&p2Act);
    // if(UT2D_comp2pt((Point2*)&p2a[i1],(Point2*)&p2Act,tol)) {
    if(dp < dMax) {
      // yes, found; keep index of nearest point
      ii = i1;
      dMax = dp;
    }
  }

  // select nearest point
  if(ii >= 0) {
    *pts = pta[ii];
    *ipt = ii;
    *pa1 = 0.;
    irc  = 0;
    goto L_exit;
  }


  // loop tru segments
  dMax = tol;
  for(i1=1; i1<ptNr; ++i1) {
    i0 = i1 - 1;
      // printf(" i0=%d i1=%d\n",i0,i1);
    // test if actPos is inside 2D-box; no: continue
    // init box with p2a[i1-1]
    memcpy(&pb1, &p2a[i0], sizeof(Point2));
    memcpy(&pb2, &p2a[i0], sizeof(Point2));
    // extend box with p2a[i1]
    UT2D_box_extend (&pb1, &pb2, (Point2*)&p2a[i1]);
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
    UT2D_pt_traPtPtPar  (&p2s, (Point2*)&p2a[i0], (Point2*)&p2a[i1], *pa1);

    // test dist pts - p2Act
    dp = UT2D_lenB_2pt (&p2s, (Point2*)&p2Act);
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
    // UT3D_stru_dump (Typ_PT, pts, " pts:");
    // if(irc >= 0) GR_Disp_pt (pts, SYM_STAR_S, ATT_COL_RED);

  return irc;

}


/*================== EOF ===========================*/
