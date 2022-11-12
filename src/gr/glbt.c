// ../xa/tst_glbt.c                              2018-04-04  RF
/*
 *
 * Copyright (C) 2018 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
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
  change placing the GL2D-objects; use glLoadIdentity instead backScaling.
  GLBT_vcSelStat / GLBT_plnSelStat cannot use negative DBi's
    ..

-----------------------------------------------------
Modifications:
2018-04-04 new - extracted from ut_GL.c. RF.

-----------------------------------------------------
*/
#ifdef globTag
 void GLBT(){}
#endif
/*!
\file  ../xa/glbt.c
\brief 2D - Buttons in OpenGL.           INF_GL2D__ INF_GL2D_buttons__
\code
=====================================================
List_functions_start:

GLBT_Redraw        disp 2D-OpenGL-buttons, Vector/PlaneSelector (GL_Redraw)
GLBT_view__        set GLBT_ori = pos. of GL2D-objects
GLBT_view_set      set GL - transformations according view-plane
GLBT_disp_vc       display VectorSelector
GLBT_disp_Pln      display StdPlaneselector
GLBT_set_stat      set status of 2D-icons; 0=enabled, X=disabled

GLBT_but_ini       init
GLBT_but_add       add GL-2D-button
GLBT_but_disp      display all defined 2D-buttons

GLBT_sel__         analyze selection of 2D-button or obj of Vector/PlaneSelector
GLBT_sel_sel       start GL-selection only for selectors

GLBT_vcSel_set     activate / disactivate
GLBT_plnSel_set    activate / disactivate

GLBT_test__

List_functions_end:
=====================================================
see also
GL_eye_upd         set GLBT_ori - lower right point of window in userCoords



TODO:
- make a struct with all 2D-OpenGL-buttons info; 

I2D_iNr
I2D_typTab
I2D_txtTab
I2D_stat


// Possible icons:
// REV          Typ_modREV
// CCW          Typ_modCWCCW  Typ_modCW Typ_modCCW
// CX           Typ_modCX
// CTRL         Typ_modCTRL
// "1"|"2" ...  Typ_mod1  "1"|"2" ... (Side, Version, solutionNr ..)
// NXT PRV      for PgUp/PgDwn-Buttons for DB-Vectors, Typ_mod1,
// LIST         for Typ_CtlgPart

// Typ_modAux "[Spline/Polygon] POL"  IE_cad_Inp1_Aux
// man könnte Typ_modCX mit Typ_modAux ersetzen.
// man könnte Typ_modCTRL mit Typ_modAux ersetzen.
// man könnte Typ_modREV mit Typ_modAux ersetzen.


// Add new modifier:
// sele_get_icons
// sele_set__
// UI_GR_Select1
// IE_inpCk



-------------------------------------------------------------------
- why 2D-OpenGL-buttons not via glCallList ?   dzt zu aufwendig ..
  - muesste am ori starten; als erstes ein "glCallList (goto-ori)"
    - dann "glCallList (goto-buttonPos)"
    - dann disp button if active


*/

#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // va_list
#include <ctype.h>                     // isdigit

#include <GL/gl.h>                       // /usr/X11R6/include/GL/gl.h
#include <GL/glu.h>                      // gluPickMatrix


#include "../ut/ut_geo.h"              // Point-def
#include "../ut/ut_memTab.h"           // MemTab
#include "../ut/ut_itmsh.h"            // MSHIG_EDGLN_.. typedef_MemTab.. Fac3

#include "../db/ut_DB.h"               // DB_GetVector
#include "../gr/ut_GL.h"               // GR_MODE_SELECT
#include "../gr/ut_gr.h"               // LN_WIDTH_..

#include "../xa/xa_sele.h"             // Typ_FncVAR1
#include "../xa/xa_mem.h"              // memspc..




//============ Extern Var: =====================
// ex ../gr/ut_GL.c
// GLBT_ori = lower right point on GL_eye_pln in userCoords
extern int GL_mode_draw_select;
extern Point GL_cen;
// extern Point GLBT_ori;
extern double GL_angZ, GL_angX;
extern double GL_Scr_Siz_X, GL_Scr_Siz_Y;
extern double GL_Scale;
extern double GL2D_Scale;
// extern Vector *GL_eyeX;
extern Vector *GL_eyeY;
extern Vector *GL_eyeZ;
extern GLdouble GL_ModSiz;
extern GLint    GL_Viewp[4];



// ex ../gr/ut_DL.c:
extern long       GR_TAB_IND;



//============ Local Var: =====================
#define GLBT_POS_ORI 46      // hor.size of GL2D-button
                             //   and size of PlaneSelector
#define GLBT_VC_LEN  40      // length of vectors

#define I2D_TABSITZ  32


static Point GLBT_ori;          // GL2D-position of PlaneSelector

       int GLBT_vcSelStat = -1; //  0=defaultVectors active;
                                // >0=index of vec to display (+ 0=def.Vectors)
                                // -1=vectorSelect is NOT active; 
                                // index of vec to display; -1=none

       int GLBT_plnSelStat = -1;//  0=planeSelect is active; 
                                // -1=planeSelect is NOT active; 


static GLfloat glColx[] = {0.0, 0.0, 1.0, 0.0};
static GLfloat glColy[] = {0.0, 1.0, 0.0, 0.0};
static GLfloat glColz[] = {1.0, 1.0, 1.0, 0.0};
static GLfloat glColv[] = {1.0, 0.0, 0.0, 0.0};


static int   I2D_iNr = 0;                        // nr of active 2D-buttons
static int   I2D_typTab[I2D_TABSITZ];            // type of obj (VC, PLN ..)
static char  I2D_txtTab[I2D_TABSITZ][32];        // button-text
static char  I2D_stat[I2D_TABSITZ];










//================================================================
  int GLBT_sel__ (int *sTyp, long *sDbi, int *stat,
                  int sx, int sy) {
//================================================================
// GLBT_sel__        analyze selection of 2D-button or obj of Vector/PlaneSelector
// GL cannot select tags (glDrawPixels) and text; do manually.
// Input:
//   sx,sy         pickPos in screenCoords
// Output:
//   sTyp,sDbi     selected obj; 0=nothing
//   stat          0=normal, 1=high-priority; overwrite underlying objs


  int      i1, iLen, tPos, tWidth, ori2;
  char     *bTx;

  // printf("GLBT_sel__ %d %d\n",sx,sy);
  // printf(" GLBT_ori %f %f\n",GLBT_ori.x,GLBT_ori.y);


  *sTyp = 0;
  ori2 = GLBT_POS_ORI * 2;

  if(sy > ori2) goto L_exit;
  if(sx < ori2) goto L_ck_vc;

  if(sy > GLTXA_sizBY + 2) goto L_exit;
  goto L_ck_bt;


  //----------------------------------------------------------------
  // position is inside selectorZone; start GL-selection only for selectors.
  L_ck_vc:
  GLBT_sel_sel (sTyp, sDbi, stat, sx, sy);
  goto L_exit;


  //================================================================
  // position is inside buttonZone; test x-values of buttons
  L_ck_bt:
  if(I2D_iNr < 1) goto L_exit;
    // printf(" _sel__-buttons\n");

  // loop tru buttons
  tPos = ori2;
  for(i1=0; i1<I2D_iNr; ++i1) {
    bTx = I2D_txtTab[i1];
    iLen = strlen(bTx);
    tWidth = (iLen + 2) * GLTXA_sizBX;
      // printf(" b%d pos=%d width=%d\n",i1,tPos,tWidth);
    if((sx > tPos)&&(sx < tPos + tWidth)) {
        // printf(" selected-button-%d\n",i1);
      *sTyp = I2D_typTab[i1];
      *sDbi = 0L;
      *stat = 1; 
      goto L_exit;
    }
    tPos += (tWidth + GLTXA_sizBX);
  }


  //----------------------------------------------------------------
  L_exit:

    // printf("ex-GLBT_sel__ typ=%d dbi=%ld stat=%d\n",*sTyp,*sDbi,*stat);

  return 0;

}


//================================================================
  int GLBT_sel_sel (int *sTyp, long *sDbi, int *stat,
                    int sx, int sy) {
//================================================================
// start GL-selection only for selectors.
// see GL_sel_rect
 
  int      hits;
  long     *ia;
  float   fz;
  GLuint   *selectBuf, BUF_SIZ;



  // printf("GLBT_sel_sel ============================\n");
  if((GLBT_vcSelStat < 0)&&(GLBT_plnSelStat < 0)) return 0;


  // switch to ProjectionMode
  glMatrixMode (GL_PROJECTION);
  // save active matrix
  glPushMatrix ();
  // load Defaultmat.
  glLoadIdentity ();




  //----------------------------------------------------------------
  // Init Sel. Buffer (BEFORE switching to select)
  selectBuf = (int*)memspc501;
  BUF_SIZ = sizeof(memspc501) / sizeof(GLuint);
  glSelectBuffer (BUF_SIZ, selectBuf);

  // start select
  glRenderMode (GL_SELECT);
  GL_mode_draw_select = GR_MODE_2DSELECT;

  // Init NameStack.
  glInitNames(); 
  glPushName((GLuint)0);       // Muss sein. Damit ein Name am Stack ist,

  // define pick-window (rectangle 5x5 pixels)
  // - changes active (PROJECTION)-matrix !
  gluPickMatrix ((double)sx, (double)sy, 5., 5., GL_Viewp);



  //----------------------------------------------------------------
  // redraw all objs to be picked / selectors, buttons

  // set glOrtho - 2D, GLBT_ori
  GLBT_view__ (); // else cannot pick ?
  // fz = GL_Scr_Siz_X * 10.f;
  // glOrtho (0.f, (float)GL_Scr_Siz_X, 0.f, (float)GL_Scr_Siz_Y, -fz, fz);
  // UT3D_pt_3db (&GLBT_ori, GL_Scr_Siz_X - GLBT_POS_ORI, GLBT_POS_ORI, 0.);

  // redraw
  if(GLBT_vcSelStat >= 0) GLBT_disp_vc ();
  if(GLBT_plnSelStat >= 0) GLBT_disp_Pln ();
  if(I2D_iNr > 0) GLBT_but_disp ();

  // close, fill buffer
  hits = glRenderMode (GL_RENDER);
    // printf(" GLBT-hits=%d\n",hits);

  // restore 
  GL_mode_draw_select = GR_MODE_DRAW;

  if(hits < 1) {
    if(hits < 0) TX_Print("ERROR: selectbuffer overflow ...");
    goto Fertig;
  }


  //----------------------------------------------------------------
  // decode hits, add hits into selectionBuffer
  ia = (long*)MEM_alloc_tmp ((int)(hits * sizeof(long)));

  GL_sel_hits (ia, selectBuf, hits);

  // 1-9 are vectors
  if(ia[0] < 10) {
    *sTyp = Typ_VC;
    // test specialVector GLBT_vcSelStat
    if(ia[0] == 7) *sDbi = GLBT_vcSelStat;
    else           *sDbi = -ia[0];   // stdVector negativ (DB_VCX_IND ..)
    goto Fertig;
  }

  // 11,12,13 = stdPlanes
  *sTyp = Typ_PLN;
  *sDbi = 10L - ia[0];


  //----------------------------------------------------------------
  Fertig:

  // replace saved matrix
  glMatrixMode (GL_PROJECTION);
  glPopMatrix ();
  // back to normal rendering
  glMatrixMode (GL_MODELVIEW);


  return 0;

}


//================================================================
  int GLBT_but_ini () {
//================================================================

  I2D_iNr = 0;

  return 0;

}

 
//================================================================
  int GLBT_but_add  (int *ib, int iTyp, char *bTxt) {
//================================================================
// GLBT_but_add       add GL-2D-button
// Input:
//   ib      button-index to use, clearing all following buttons
// Output:
//   ib      ib += 1;
// was sele_set_icon


  // printf("GLBT_but_add typ=%d |%s|\n",iTyp,bTxt);


  I2D_iNr = *ib;

  I2D_typTab[I2D_iNr] = iTyp;

  strcpy (I2D_txtTab[I2D_iNr], bTxt);

  I2D_stat[I2D_iNr] = '0';          // 0=active X=disabled

  I2D_iNr += 1;
  *ib = I2D_iNr;


  return 0;

}
 

//================================================================
  int GLBT_plnSel_set (int ii, int mode) {
//================================================================
// Input:
//   ii   0  planeSelect active;
//        -1 planeSelect is NOT active;
//   mode 0=do not redraw;  UNUSED
//        1=DL_Redraw;
// RetCod:
//   GL_plnSelStat before modification

  int    iDo, oldStat;


  // printf("GLBT_plnSel_set ii=%d mode=%d stat=%d\n",ii,mode,GLBT_plnSelStat);


  oldStat = GLBT_plnSelStat;

  // if(mode == 2) goto L_exit;

  // if(GL_plnSelStat != ii) iDo = 1;
  // else                    iDo = 0;


  GLBT_plnSelStat = ii;

  // if(iDo) {
    // if(mode) DL_Redraw ();
  // }

  L_exit:
  return oldStat;

}


//================================================================
  int GLBT_vcSel_set (int ii, int mode) {
//================================================================
// set dbi of a vector for display in vector-selector
// Input:
//   ii   0  defaultVectors active;
//        >0 index of vec to display (+ def.Vectors)
//        -1 vectorSelect is NOT active;
//   mode 0=do not redraw; UNUSED
//        1=DL_Redraw = activate
//        2=query only (RetCod = GL_vcSelStat)
// RetCod:
//   GL_vcSelStat before modification

  int    iDo, oldStat;


  // printf("GLBT_vcSel_set ii=%d mode=%d stat=%d\n",ii,GLBT_vcSelStat,mode);


  oldStat = GLBT_vcSelStat;

  if(mode == 2) goto L_exit;

  // if(GL_vcSelStat != ii) iDo = 1;
  // else                   iDo = 0;


  GLBT_vcSelStat = ii;

  // if(iDo) {
    // if(mode) DL_Redraw ();
  // }

  L_exit:
  return oldStat;

}


//================================================================
  int GLBT_Redraw () {
//================================================================
/// \code
/// GLBT_Redraw        disp 2D-OpenGL-buttons, Vector/PlaneSelector (GL_Redraw)
/// make 2D-view for buttons, vector-selector and plane-selector;
/// only GR_MODE_DRAW; not GR_MODE_SELECT GR_MODE_FEEDBACK GR_MODE_PRINT1 ..
/// - in 3D-view the selectors are outside clipping-zone !!
/// \endcode
 

  int      i1, i2, *ia, iLen, btNr, tWidth;
  char     *ta, *p1, *p2, cBuf[32];
  double   sx, sy, sz, ixSiz;
  Point    pts, ptu;
  GLuint   gli;


  // printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX GLBT_Redraw\n");
  // printf(" GLBT_vcSelStat=%d GLBT_plnSelStat=%d I2D_iNr=%d\n",
          // GLBT_vcSelStat, GLBT_plnSelStat, I2D_iNr);
  // printf("  GL_mode_draw_select=%d\n",GL_mode_draw_select);

    // TESTBLOCK
    // i1 = 0; GLBT_but_add (&i1, 0, "Button-1"); GLBT_but_add (&i1, 0, "2-Test");
    // GLBT_vcSelStat = 20;
    // GLBT_plnSelStat = -1;
    // GLBT_test__ (); return 0;
    // END TESTBLOCK
  



  if(GL_mode_draw_select == GR_MODE_SELECT) return 0;


  // return if no 2D-obj is active
  if((GLBT_vcSelStat < 0)   &&
     (GLBT_plnSelStat < 0)  && 
     (I2D_iNr <= 0))            return 0;



  //----------------------------------------------------------------
  glDisable (GL_LIGHTING);
  // glDepthFunc (GL_ALWAYS);
  glDisable (GL_DEPTH_TEST);   // 2D only
  glDisable (GL_LINE_STIPPLE);
  glLineWidth (LN_WIDTH_DEF);

  // save active PROJECTION-matrix, then init
  glMatrixMode (GL_PROJECTION);
  // save active matrix
  glPushMatrix ();
  // load Defaultmat.
  glLoadIdentity ();




  //----------------------------------------------------------------
  // display
  // set glOrtho - 2D, GLBT_ori
  GLBT_view__ ();

  if(GLBT_vcSelStat >= 0) GLBT_disp_vc ();    // vector-selector
  if(GLBT_plnSelStat >= 0) GLBT_disp_Pln ();  // std-plane-selector
  if(I2D_iNr > 0) GLBT_but_disp ();    // 2D-buttons


  //----------------------------------------------------------------
  // retore saved PROJECTION-matrix
  glMatrixMode (GL_PROJECTION);
  glPopMatrix ();
  // back to normal rendering
  glMatrixMode (GL_MODELVIEW);


  // reset 3D
  glEnable (GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);   // restore 3D
  // glDepthFunc(GL_LEQUAL);


  return 0;

}

//================================================================
  int GLBT_but_disp () {
//================================================================
// display all defined 2D-buttons

  int      btNr;
  int      i1, tWidth;
  double   scl, tw, th, sx;
  float    fscl;
  char     *bTx; //cBuf[32];
  Point    ptu;
  GLuint   gli;



  // printf("GLBT_but_disp\n");
  // printf("  GLTXA_sizBX=%d GLTXA_sizBY=%d GL2D_Scale=%f GL_SclNorm=%f\n",
          // GLTXA_sizBX,GLTXA_sizBY,GL2D_Scale,GL_SclNorm);


  fscl = 1. / GL2D_Scale;
  scl = GL2D_Scale;

  // get th = tagHeight in pixels
  th = GLTXA_sizBY;

  btNr = I2D_iNr;


  //----------------------------------------------------------------

  // set color for text (must be set before glRasterPos3dv)
  glColor3f (0.f, 0.f, 0.f);


  // get startPosition for buttons
  // get ptu = LR-windowCorner in userCoords
//   UT3D_pt_3db (&ptu, GL_Scr_Siz_X - (GLBT_POS_ORI * 2), 2, 0.);
  UT3D_pt_3db (&ptu, GLBT_POS_ORI * 2, 2, 0.);
    // DEB_dump_obj__ (Typ_PT, &ptu, " ptu:");

  // set GL-rasterPos = ptu
  glRasterPos3dv ((double*)&ptu);

  // sx = half charWidth in pixels
  sx = GLTXA_sizCX / 2; 


  // disp buttons. Startpos is 
  for(i1=0; i1<btNr; ++i1) {

    // save the current color & rasterPosition
    GL2D_pos_set ();

    // get bTx = text
    bTx = I2D_txtTab[i1];

    // get tWidth = total width of tag in pixels
    tWidth = (strlen(bTx) + 1) * GLTXA_sizBX;

    // disp button
    tw = tWidth; // * scl;
    glPixelZoom (1.f, 1.f);
    GL_set_tag2D ((int)tw, (int)th, GL_GREEN);

    // move GL-rasterPosition 0.5 char right and 4 pix up
    GL2D_pos_move (sx, 4);

    // display text bTx
    glPixelZoom (fscl, fscl);
    GL_set_txt2D (bTx);

    // restore color & rasterPosition
    GL2D_pos_get ();

    // move GL-rasterPosition to left - tagWidth + 1 char gap
    GL2D_pos_move (tWidth + (GLTXA_sizBX * 2), 0);
  }


  return 0;

}


//================================================================
  int GLBT_view__ () {
//================================================================
// GLBT_view__            set GLBT_ori = pos. of GL2D-objects
// set userCoords = screenCoords, topView.
// glOrtho - 2D

  float   fz;
  Point   eye;

  // printf("GLBT_view__ GL_Scr_Siz_X=%f\n",GL_Scr_Siz_X);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity ();  // init PROJ-Mat.

    // glOrtho (-fx1, fx1, -fy1, fy1, -fz1, fz1);
  fz = GL_Scr_Siz_X * 10.f;
  glOrtho (0.f, (float)GL_Scr_Siz_X, 0.f, (float)GL_Scr_Siz_Y, -fz, fz);
    // printf(" GLBT_view__\n");


  // origin of vector-selector - start on low-left
  UT3D_pt_3db (&GLBT_ori, GLBT_POS_ORI, GLBT_POS_ORI, 0.);
    // DEB_dump_obj__ (Typ_PT, &GLBT_ori, "GLBT_view__-GLBT_ori");


    // TESTBLOCK
    // GL_GetGLMat0 ();
    // GL_dump_Mat0 ();
    // END TESTBLOCK

  return 0;

}


//================================================================
  int GLBT_test__ () {
//================================================================
// disp text "Test" at screencoords, gl must be open
// GL_DrawSymB gets clipped if outside

  int          i1, iLen;
  long         gli, dli;
  char         *cBuf;
  float        *fBuf;
  double       lux, luy, sx,sy,sz;
  Point        pts, ptu;
  // GLfloat pos[4];
  GLfloat fCol=0.5f;

  printf("========================================= GLBT_test__\n");

  // glEnable (GL_LIGHTING);
  glDisable (GL_LIGHTING);
  glDepthFunc (GL_ALWAYS);
  // glDepthFunc (GL_NOTEQUAL);


  //----------------------------------------------------------------
/*
  // dli = -2L;
  // gli = GL_fix_DL_ind (&dli);
  // glNewList (gli, GL_COMPILE);
    glColor3f (1.f, 1.f, 0.f);       // gelb
    // get screenPos of GLBT_ori
    // GL_ptSc_ptUc (&pts, &GLBT_ori);
    // pts = GLBT_ori;
    sx = GL_Scr_Siz_X;
    sx -= 100;
    sy = 100;
    UT3D_pt_3db (&pts, sx, sy, 0.);
    GL_ptUc_ptSc (&ptu, &pts);
       DEB_dump_obj__ (Typ_PT, &ptu, "  ptu: ");
    glBegin (GL_QUADS);
    glVertex3dv ((double*)&ptu);
    ptu.x += 10.;
    glVertex3dv ((double*)&ptu);
    ptu.y += 10.;
    glVertex3dv ((double*)&ptu);
    ptu.x -= 10.;
    glVertex3dv ((double*)&ptu);
    glEnd ();
*/

  //----------------------------------------------------------------
  // place buttons from right to left; 
  // posEnd = Y-value of startPosition of last button (screenCoords).



    sx = GL_Scr_Siz_X;
    sx -= 100;
    sy = 100;
    UT3D_pt_3db (&pts, sx, sy, 0.);
    // GL_ptUc_ptSc (&ptu, &pts);
      // DEB_dump_obj__ (Typ_PT, &ptu, " ptu:");
    glRasterPos3dv ((double*)&pts);

  // glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
  // GL_DispTag1 (100, 12, ATT_COL_YELLOW);

  GL_set_tag2D (80, 12, GL_GREEN);

  // disp text
  // glColor4f (0.f, 1.f, 0.f, 1.f);    // USELESS! Text always black ..
  GL_set_txt2D ("Test");


  //----------------------------------------------------------------
/*
  // test disp vector-rectangle
  DEB_dump_obj__ (Typ_PT, &GLBT_ori, "GLBT_ori");
  // GL_DrawSymB (&dl1, ATT_COL_RED, SYM_SQU_B, &GLBT_ori);
  pt1 = GLBT_ori;

  // move the symbol along x and y
  lux = -200. / (GL_Scale * GL_SclNorm);  // get usercoords
  luy =   50. / (GL_Scale * GL_SclNorm);
  UT3D_pt_trapt2vc2len (&pt1, &GLBT_ori, GL_eyeY, lux, GL_eyeZ, luy);
    DEB_dump_obj__ (Typ_PT,  &pt1, " pt1: ");

  GL_DrawSymV (&dl1, SYM_SQUARE, ATT_COL_RED, &pt1, 1.);
*/


  //----------------------------------------------------------------
  // test display plane-selector
  // GLBT_vcSelStat = 0;
  // GLBT_plnSelStat = 1;
  // GLBT_dispVcPln ();

// glEnable (GL_LIGHTING);


  return 0;

}


//================================================================
  int GLBT_view_set () {
//================================================================
// set transformations according view-plane
// see GLBT_view__ (get 
  
  glTranslated (GLBT_ori.x, GLBT_ori.y, GLBT_ori.z);
  glRotated (GL_angX, 1.0, 0.0, 0.0);  // um X drehen
  glRotated (GL_angZ, 0.0, 0.0, 1.0);  // um Z drehen

  return 0;

}


//================================================================
  int GLBT_disp_vc () {
//================================================================
// display VectorSelector
// GL_mode_draw_select = 


  int     i1;
  GLuint  gli;
  double  lv;
  char    s1[32];
  Point   pt1, pt2, pt3, pt4, pt5, pt6, pt7, ptu;
  Vector  vc1;


  // printf("GLBT_disp_vc\n");

 
  if(GL_mode_draw_select == GR_MODE_2DSELECT) gli = 1;
  else                                        gli = 0;
    // printf(" gli=%d\n",gli);


  // fit size
  lv = GLBT_VC_LEN;


  // X = 1,2
  pt1 = UT3D_PT_NUL;
  pt1.x += lv;
  pt2 = UT3D_PT_NUL;
  pt2.x -= lv;

  // Y = 3,4
  pt3 = UT3D_PT_NUL;
  pt3.y += lv;
  pt4 = UT3D_PT_NUL;
  pt4.y -= lv;

  // Z = 5,6
  pt5 = UT3D_PT_NUL;
  pt5.z += lv;
  pt6 = UT3D_PT_NUL;
  pt6.z -= lv;



  glPushMatrix ();

  // move to GLBT_ori, rotate GL_angZ, GL_angX;
  GLBT_view_set ();


    // X
    if(gli) glLoadName(1);  // DB_VCX_IND   // GL-ID's must be positive
  // glNewList ((GLuint)30, GL_COMPILE_AND_EXECUTE);
    glColor3fv   (glColx);
    glBegin (GL_LINES);
      glVertex3dv ((double*)&UT3D_PT_NUL);
      glVertex3dv ((double*)&pt1);
    glEnd ();
  // glEndList ();

    if(gli) glLoadName(4);   // DB_VCIX_IND
    glBegin (GL_LINES);
      glVertex3dv ((double*)&UT3D_PT_NUL);
      glVertex3dv ((double*)&pt2);
    glEnd ();


    // Y
    glColor3fv   (glColy);
    if(gli) glLoadName(2);          // DB_VCY_IND
    glBegin (GL_LINES);
      glVertex3dv ((double*)&UT3D_PT_NUL);
      glVertex3dv ((double*)&pt3);
    glEnd ();

    if(gli) glLoadName(5);        // DB_VCIY_IND
    glBegin (GL_LINES);
      glVertex3dv ((double*)&UT3D_PT_NUL);
      glVertex3dv ((double*)&pt4);
    glEnd ();


    // Z
    glColor3fv   (glColz);
    if(gli) glLoadName(3);         // DB_VCZ_IND
    glBegin (GL_LINES);
      glVertex3dv ((double*)&UT3D_PT_NUL);
      glVertex3dv ((double*)&pt5);
    glEnd ();

    if(gli) glLoadName(6);         // DB_VCIZ_IND
    glBegin (GL_LINES);
      glVertex3dv ((double*)&UT3D_PT_NUL);
      glVertex3dv ((double*)&pt6);
    glEnd ();


    //----- display characters x y for StandardVectors ----------
    // display x y   see GR_tDyn_txtA
    glColor3fv   (glColx);
    glRasterPos3dv ((double*)&pt1);
    glCallList ((GLuint)120);           // "x"

    glColor3fv   (glColy);
    glRasterPos3dv ((double*)&pt3);
    glCallList ((GLuint)121);           // "y"


    //----- display defined DB-Vector dbi=GLBT_vcSelStat ----------
    if(GLBT_vcSelStat > 0) {

      vc1 = DB_GetVector ((long)GLBT_vcSelStat);
      UT3D_vc_setLength (&vc1, &vc1, lv);
        // DEB_dump_obj__ (Typ_VC, &vc1, " D%d",GLBT_vcSelStat);

      glColor3fv   (glColv);
      if(gli) glLoadName(7);
      glBegin (GL_LINES);
        glVertex3dv ((double*)&UT3D_PT_NUL);
        glVertex3dv ((double*)&vc1);
      glEnd ();


      //----- display ID of defined DB-Vector dbi=GLBT_vcSelStat ----------
      // glColor3fv   (glColv);
      glRasterPos3dv ((double*)&vc1);
      // strcpy(s1, "D21");
      sprintf(s1, "D%d",GLBT_vcSelStat);
      i1 = strlen(s1);
      glCallLists (i1, GL_UNSIGNED_BYTE, (GLubyte*)s1);
    }



    glPopMatrix ();


  return 0;

}


//================================================================
  int GLBT_disp_Pln () {
//================================================================
// draw VectorSelector and StdPlaneselector; at end of gl-list.

// #define SIZ_vcSel    100      // length of vectors
// #define SCL_plnSel   1.5
#define SIZS_PLN 20

  int      gli;
  double   sx, sy, ax, ay, az;


  // printf("GLBT_disp_Pln \n");

  if(GL_mode_draw_select == GR_MODE_2DSELECT) gli = 1;
  else                                        gli = 0;



  //----- display StandardPlanes -----------------------------------
  glPushMatrix ();

    // move to GLBT_ori, rotate GL_angZ, GL_angX;
    GLBT_view_set ();


    glColor3fv   (glColz);
    if(gli) glLoadName(13);        // DB_PLZ_IND
    GL_Disp_pln__ ((double)SIZS_PLN);

    glColor3fv   (glColy);
    if(gli) glLoadName(12);        // DB_PLY_IND
    glRotated (90., 1., 0., 0.);
    GL_Disp_pln__ ((double)SIZS_PLN);

    glColor3fv   (glColx);
    if(gli) glLoadName(11);        // DB_PLX_IND
    glRotated (90., 0., 1., 0.);
    GL_Disp_pln__ ((double)SIZS_PLN);

  glPopMatrix ();

  return 0;

}


//================================================================
  int GLBT_set_stat (int ii) {
//================================================================
// set status of 2D-icons; 0=enabled, X=disabled
// Input:
//   ii    inputFieldInd

  int   i1, i2, typR;
  char  *p1;

  // Func:
  char *IE_get_inp__ ();


  // printf("GLBT_set_stat %d %d\n",ii,I2D_iNr);

  // typR = IE_inpTypR[ii];   // requested typ
  // p1 = IE_get_inp__ (ii);
  IE_get_inp_typ (&typR, &p1, ii);  // get requested typ & text of inputField


  // typR = IE_inpCkTyp (ii, p1);
    // printf(" typR=%d |%s|\n",typR,p1);


  // loop tru all active buttons:
  for(i1=0; i1<I2D_iNr; ++i1) {

    if(typR == Typ_modREV) {
      I2D_stat[i1] = '0';
    } else {
      if(I2D_typTab[i1] == Typ_modREV) {
        // Input Vector: may not be first word of inputField
        if(strlen(p1) < 1) I2D_stat[i1] = 'X';
        else               I2D_stat[i1] = '0';
          // printf(" REV -> |%c|\n",I2D_stat[i1]);
      }
    }

    // check if vars defined
    if((I2D_typTab[i1] == Typ_FncVAR1)  ||
       (I2D_typTab[i1] == Typ_FncVAR2))     {
      i2 = DB_get_ObjNr (Typ_VAR);
      if(i2 < 1) {
        I2D_stat[i1] = 'X';   // no vars defined
        // MSG_pri_0("VR0");
      }
    }

    // check if vectors defined
    if((I2D_typTab[i1] == Typ_FncVC1)  ||
       (I2D_typTab[i1] == Typ_FncVC2))     {
      i2 = DB_get_ObjNr (Typ_VC);
      if(i2 < 1) {
        I2D_stat[i1] = 'X';   // no vars defined
        // MSG_pri_0("VC0");
      }
    }

    // check if transformation defined
    if((I2D_typTab[i1] == Typ_FncTr1)  ||
       (I2D_typTab[i1] == Typ_FncTr2))     {
      i2 = DB_get_ObjNr (Typ_Tra);
      if(i2 < 1) {
        I2D_stat[i1] = 'X';   // no vars defined
        // MSG_pri_0("VC0");
      }
    }


  }

  return 0;

}


// EOF

