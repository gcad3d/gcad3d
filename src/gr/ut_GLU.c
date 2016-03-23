//               Interface GLU - Functions        2004-04-05      RF
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
  ..

-----------------------------------------------------
*/
/*!
\file  ../gr/ut_GLU.c
\brief aux. functions tesselate using GLU 
\code
=====================================================
List_functions_start:

GLU_tess__     tesselate outerBoundary with holes above grid-quads.

GLT_spu_       eine nicht gelochte planare Flaeche tesselieren ..
GLT_spp__      tesselate planaren Patch mit Aussenkontur/Loechern
GLT_diff_work  difference of 2 contours -> ContTab
GLT_comm_work  common of 2 contours -> ContTab

- internal:
GLU_alloc
GLT_init
GLT_comb_reset
GLT_exit
GLT_comm_CB    add vertex
GLT_comm_end
GLT_comb_CB    add vertex 
GLT_CB_Err
GLT_CB_Begin
GLT_CB_End
GLT_start__
GLT_spp_beg    start new active contour
GLT_spp_pp     beginn next patch for active surf
GLT_spp_vert   add vertex
GLT_spp_end    add contour to active patch

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



GLU:
 Input: 1 Aussenkontur und 0-n Innenkonturen (als 3D-Punkte).
   Wird via GLU tesseliert;
   via Callbacks werden die Eckpunkte ausgegeben.

     Die Hauptstruktur (Contours) gehen --> GLT_cta
     Die Patches gehen --> GLT_ppa
     Die Punkte gehen nach GLT_pta


gluTessVertex ACHTUNG:
 - braucht 3D-Daten bis nach gluTessEndPolygon !!
 - kopiert nur den Index; Daten muessen bis nach gluTessEndPolygon bestehen !!


Funktionsweise siehe ../gr/tess_su.c



/p2/opt/glut/glut-3.6/progs/mesademos/tess_demo.c; geht "need combine callback"
/p2/opt/glut/glut-3.6/progs/redbook/tess.c  "XFree86-DRI" missing
/p2/opt/glut/glut-3.6/progs/redbook/tesswind.c   geht
/mnt/win_d/dev/math/OpenGeometry2.0/c/tess.c


To obtain the post-tessellation vertex coordinates, tessellate the polygons while
 in feedback mode.

If you are using the 1.0 or 1.1 version of GLU, no COMBINE callback.
 Alte Interfaces:
 gluBeginPolygon -> gluTessBeginPolygon,
 gluNextContour -> gluTessBeginContour

gluTessNormal() can be used to determine the winding direction of the tessellated
 polygons. (auch anstatt gluTessProperty !!)
 gluTessNormal(tessobj, 0., 0., 1.);

The default normal is (0, 0, 0), and its effect is not immediately obvious. In this case, it is expected that the input data lies approximately in a plane, and a plane is fitted to the vertices, no matter how they are truly connected. The sign of the normal is chosen so that the sum of the signed areas of all input contours is nonnegative (where a counterclockwise contour has a positive area). Note that if the input data does not lie approximately in a plane, then projection perpendicular to the computed normal may substantially change the geometry.



GLU_TESS_EDGE_FLAG                                                                                                                                                The edge flag callback is similar to glEdgeFlag. The function takes a single boolean flag that indicates which edges lie on the polygon boundary. If the flag is GL_TRUE, then each vertex that follows begins an edge that lies on the polygon boundary, that is, an edge that separates an interior region from an exterior one. If the flag is GL_FALSE, then each vertex that follows begins an edge that lies in the polygon interior. The edge flag callback (if defined) is invoked before the first vertex callback.                                                           Since triangle fans and triangle strips do not support edge flags, the begin callback is not called with GL_TRIANGLE_FAN or GL_TRIANGLE_STRIP if a non-NULL edge flag callback is provided. (If the callback is initialized to NULL, there is no impact on performance). Instead, the fans and strips are converted to independent triangles. The function prototype for this callback is: void edgeFlag ( GLboolean flag );



GLU_TESS_TOLERANCE                                                                                                                                                gluTessProperty(ot1, GLU_TESS_TOLERANCE, 0.001);                                                                                                                  Specifies a tolerance for merging features to reduce the size of the output. For example, two vertices that are very close to each other might be replaced by a single vertex. The tolerance is multiplied by the largest coordinate magnitude of any input vertex; this specifies the maximum distance that any feature can move as the result of a single merge operation. If a single feature takes part in several merge operations, the total distance moved could be larger.




GLU_TESS_WINDING_RULE

outerBound=CCW;
innerBound=CW.
CW=-1, CCW=1.

For a single contour C, the winding number of a point x is simply the signed number of revolutions we make around x as we travel once around C (where CCW is positive). When there are several contours, the individual winding numbers are summed. This procedure associates a signed integer value with each point x in the plane. Note that the winding number is the same for all points in a single region.

Mehrere (getrennte) Konturen: addition der windingNr.

The winding rule classifies a region as "inside" if its winding number belongs to the chosen category (odd, nonzero, positive, negative, or absolute value of at least two). The previous GLU tessellator (prior to GLU 1.2) used the "odd" rule. The "nonzero" rule is another common way to define the interior. The other three rules are useful for polygon CSG operations.




CSG Uses for Winding Rules
v /mnt/win_d/doc/gluTess2.htm


The winding rules are also designed for computational solid geometry (CSG) operat
ions. Thy make it easy to find the union, difference, or intersection (Boolean op
erations) of several contours.


First, assume that each contour is defined so that the winding number is zero for
 each exterior region and one for each interior region. (Each contour must not in
tersect itself.) Under this model, counterclockwise contours define the outer bou
ndary of the polygon, and clockwise contours define holes. Contours may be nested
, but a nested contour must be oriented oppositely from the contour that contains
 it.


If the original polygons do not satisfy this description, they can be converted t
o this form by first running the tessellator with the GLU_TESS_BOUNDARY_ONLY prop
erty turned on. This returns a list of contours satisfying the restriction just d
escribed. By creating two tessellator objects, the callbacks from one tessellator
 can be fed directly as input to the other.


Given two or more polygons of the preceding form, CSG operations can be implement
ed as follows.

UNION - To calculate the union of several contours, draw all input contours as a
single polygon. The winding number of each resulting region is the number of orig
inal polygons that cover it. The union can be extracted by using the GLU_TESS_WIN
DING_NONZERO or GLU_TESS_WINDING_POSITIVE winding rules. Note that with the nonze
ro winding rule, we would get the same result if all contour orientations were re
versed.

INTERSECTION - This only works for two contours at a time. Draw a single polygon
using two contours. Extract the result using GLU_TESS_WINDING_ABS_GEQ_TWO.

DIFFERENCE - Suppose you want to compute A diff (B union C union D). Draw a singl
e polygon consisting of the unmodified contours from A, followed by the contours
of B, C, and D, with their vertex order reversed. To extract the result, use the
GLU_TESS_WINDING_POSITIVE winding rule. (If B, C, and D are the result of a GLU_T
ESS_BOUNDARY_ONLY operation, an alternative to reversing the vertex order is to u
se gluTessNormal() to reverse the sign of the supplied normal.





                                                                                 ===================================================================

*/

#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif

#ifdef _MSC_VER
#define GLU_CB static void CALLBACK
#else
#define GLU_CB static void
#endif


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_msh.h"              // Fac3 ..
#include "../ut/ut_cast.h"             // PTR_LONG
#include "../ut/ut_memTab.h"           // MemTab
#include "../ut/ut_tin.h"              // TYP_EDGLN_BL

#include "../db/ut_DB.h"               //
#include "../xa/xa_mem.h"              // memspc51, mem_cbuf1
#include "../gr/ut_UI.h"               // SYM_..



// aus ../gr/ut_GL.c:
extern ColRGB  GL_actCol;



//=============== Lokale Variablen: ==========================

// combine-Buffer
static Point *GLU_CombTab=NULL;
static int   GLU_CombSiz=0;
static int   GLU_CombNr=0;
static int   GLU_Err=0;
static int   GLU_pTyp;


// vars for func "common area":
static Point  *GLU_comm_tab;
static int    GLU_comm_Max;
static int    GLU_comm_pNr;

static ContTab *GLU_cTab;
static int     GLU_cMax;
static int     GLU_cNr;


static GLUtesselator *ot1=NULL;


#define GLT_pta_INC 25000
       Point *GLT_pta = NULL;
       long  GLT_pta_SIZ = 0;
static long  GLT_pta_ind, GLT_pta_start;


// Err -2
#define GLT_cta_INC 10000             // 10000
       ObjGX *GLT_cta;
       long  GLT_cta_SIZ;
static long  GLT_cta_ind;


// Err -3
#define GLT_ppa_INC 6000             // 6000
static ObjGX *GLT_ppa;
static long  GLT_ppa_SIZ;
static long  GLT_ppa_ind;



static ObjGX GLT_sur;





//------------- Prototypes old version ------------------------------
GLU_CB GLT_CB_Begin (GLenum);
GLU_CB GLT_CB_End ();
GLU_CB GLT_CB_Err (GLenum);

GLU_CB GLT_comb_CB (GLdouble*, GLdouble**, GLfloat*, GLdouble**);
GLU_CB GLT_comm_CB (void *coords);
GLU_CB GLT_comm_end ();

GLU_CB GLT_spp_beg  (GLenum);
GLU_CB GLT_spp_vert (GLvoid*);
GLU_CB GLT_spp_end  ();



//------------- Prototypes new version ------------------------------
GLU_CB GLU_CB_tess_Err (GLenum);
GLU_CB GLU_CB_tess_newPt (GLdouble*, GLdouble**, GLfloat*, GLdouble**);
GLU_CB GLU_CB_tess_beg (GLenum);
GLU_CB GLU_CB_tess_pt  (double*);
GLU_CB GLU_CB_tess_edg (GLboolean flag);
GLU_CB GLU_CB_tess_end ();

// typedef_MemTab(int);
// typedef_MemTab(char);
// typedef_MemTab(Point);
typedef_MemTab(IndTab);


//------------- statics new version ------------------------------
static MemTab(char)     *GLU_pst;    // status for points GLU_pmt
static MemTab(int)      *GLU_imt;
static MemTab(IndTab)   *GLU_fmt;
static MemTab(Point)    *GLU_pmt;

static int              GLU_typ;
static int              GLU_pNr;
static int              GLU_flag;




//================================================================

//=========================================================
  GLU_CB GLU_CB_tess_Err (GLenum gl_err) {
//=========================================================

  const GLubyte   *estring;


  // printf("GLT_CB_Err %d\n",gl_err);

  estring = gluErrorString (gl_err);

  printf("***** GLT_CB_Err |%s|\n", estring);

  // TX_Print("GLT_CB_Err %s",estring);

  if(!GLU_Err) GLU_Err = -1;

  return;
}


//=========================================================
  GLU_CB GLU_CB_tess_newPt (GLdouble coords[3],
                            GLdouble** dataVert,
                            GLfloat weight[4],
                            GLdouble **dataOut) {
                            // void **dataOut) {
//=========================================================
// gluTessCallback(ot1, GLU_TESS_COMBINE_DATA, (void*)&GLU_CB_tess_newPt);
// add new vertex to mesh.
// dataOut ist ein kompletter VERTEX; x,y,z,r,g,b,a;
// rgba sind floats!
// es muss nur static memspc fuer die daten zur Verfuegung gestellt werden
// dieser Platz wird benoetigt bis nach gluTessEndPolygon.
// INPUT:
//   coords        point
// OUTPUT:
//   dataOut       pointer to stored point coords

  int      i1, irc;
  long     l1;
static char c1=0;
  // Point    *p1;


  // printf(" GLU_CB_tess_newPt %lf %lf %lf\n",coords[0],coords[1],coords[2]);

  if(GLU_Err) return;

  // add point to GLU_pmt
  i1 = MEMTAB_IND (GLU_pmt);
    // printf(" GLU_CB_tess_newPt-i1=%d\n",i1);

  irc = MemTab_add (GLU_pmt, &l1, coords, 1, 0);
  if(irc) {
    // realloc or EOM
      printf("************ ERROR REALLOC POINTS %d\n",i1);
    // GLU_CB_tess_Err (1);
    // GLU_exit ();
    GLU_Err = 1;
    return;
  }
  // give pointer to point stored
  *dataOut = (GLdouble*)MEMTAB__ (GLU_pmt, i1);
    // printf(" datPos = %p\n",*dataOut);


  // add stat=0 to GLU_pst
  if(GLU_pst) MemTab_add (GLU_pst, &l1, &c1, 1, 0);


  return;
}


//=========================================================
  GLU_CB GLU_CB_tess_beg (GLenum pTyp) { //, void *data) {
//=========================================================
// start patch
// gluTessCallback(ot1, GLU_TESS_BEGIN,   (void*)&GLU_CB_tess_beg);

  // printf("<<<<<<<<<<< GLU_CB_tess_beg %d\n",pTyp);

  GLU_typ = pTyp;
  GLU_pNr = 0;

  return;
}


//=========================================================
  GLU_CB GLU_CB_tess_pt (double *pt1) {
//=========================================================
// add vertex to patch
// gluTessCallback(ot1, GLU_TESS_VERTEX_DATA, (void*)&GLU_CB_tess_pt);

  int      idr;
  char     *c1;
  long     l1;


  // printf(" GLU_CB_tess_pt %9.2lf %9.2lf %9.2lf\n",
        // ((Point*)pt1)->x,((Point*)pt1)->y,((Point*)pt1)->z);


  // find point-index
  // problem if GLU_pmt did realloc !
  idr = MEMTAB_IND_POS (GLU_pmt, pt1);
    // printf(" idr=%d\n",idr);

  // add point-index to indexList GLU_ia
  MemTab_add (GLU_imt, &l1, &idr, 1, 0);

  ++GLU_pNr; // nr of points for actual face


  // mark point as used for output
  if(GLU_pst) {
    // get status for point idr to set bit-0
    c1 = MEMTAB__ (GLU_pst, idr);
    *c1 |= 8;   // set bit-0 
      // GR_Disp_pt ((Point*)pt1, SYM_TRI_S, ATT_COL_YELLOW);
    // if this point is on boundary: its flag is GL_TRUE 
    if(GLU_flag) *c1 |= 1;   // set bit-0 
      // printf(" pst[%d]=%d\n",idr,*c1);

  }

/*
  // if this point is on boundary: its flag is GL_TRUE 
  if(GLU_flag) {
    if(GLU_pst) {
      // get status for point idr to set bit-0
      c1 = MEMTAB__ (GLU_pst, idr);
      *c1 |= 1;   // set bit-0 
        // GR_Disp_pt ((Point*)pt1, SYM_TRI_S, ATT_COL_YELLOW);
    }
  }
*/


  return;
}


//=========================================================
  GLU_CB GLU_CB_tess_edg (GLboolean flag) {
//=========================================================
// flag TRUE: folling vertex is on boundary; else not.

  // printf("GLU_CB_tess_edg %d\n",flag);

  GLU_flag = flag;

  return;

}


//=========================================================
  GLU_CB GLU_CB_tess_end () {
//=========================================================
//  gluTessCallback(ot1, GLU_TESS_END,     (void*)&GLU_CB_tess_end);
// save patch

  int       ii;
  long      l1;
  IndTab   el1;


  // printf("XXXXXXXXXXXXXXXXXXX  GLU_CB_tess_end \n");

  // create record in GLU_fmt
  el1.typi = TYP_EDGLN_FAC;
  el1.iNr  = GLU_pNr;
  el1.aux  = GLU_typ;
  ii = MEMTAB_IND (GLU_imt) - GLU_pNr;
  // el1.ia = MEMTAB__ (GLU_imt, ii);
  el1.ibeg = ii;
  MemTab_add (GLU_fmt, &l1, &el1, 1, 0);

    // GR_Disp_iface (GLU_typ, 4, GLU_pNr, MEMTAB__ (GLU_imt, el1.ibeg),
                   // MEMTAB_DAT (GLU_pmt));
    // tess_test_disp_face (&el1, GLU_pmt, GLU_imt);

  return;
}





//===================================================================
  int GLU_tess__ (MemTab(IndTab) *fmt,
                  MemTab(char)     *pst,
                  MemTab(int)      *imt,
                  MemTab(Point)    *pmt,
                  MemTab(IndTab) *cmt,
                  GridBoxH *gb,
                  Vector *vcz) {
//===================================================================
// tesselate outerBoundary with holes above grid-quads.
// All contours must be CCW; all contours must have lastPoint == firstPoint.
// Input:
//   itNr   nr of contours (outerBoundary + holes) = nr of records in itBnd
//   itBnd  boundaries; all points must be in pmt; all indices must be in imt
//          .ibeg is startindex in imt, .iNr is nr of records in imt;
//   gb     gridBox; all points must be in pmt, starting at gb->p1; no indices
//          or NULL for direct tesselation
//   vcz    for 2D-tesselation NULL; else normalvector
//   imt    point-indices
//   pmt    points
// Output:
//   fmt    tesselated-faces; indexes in imt, points in pmt.
//          .ibeg is startindex in imt, .iNr is nr of records in imt;
//   pst    boundary-flags for pmt-points
//          set bitVal 8 for used points, set bitVal 1 for boundary-points.
//   imt    store point-indices for tesselated-faces (fmt) here
//   pmt    add additional points here
// RetCod:  0=OK

// creates triangles in fmt; adds points and point-links to pmt and imt.
// fmt: tesselated-faces; 3 consecutive links in imt give one triangle.
//   ibeg=startIndex in imt; iNr= nr of links in imt;
//   aux=triangle-typ GL_TRIANGLES;


// Gives the common of gridBox + outerBoundary - hole.
// using GLU_TESS_WINDING_ABS_GEQ_TWO (contours must give 2 or more)
// gridBox(CCW) + outerBoundary(CCW)            = 1+1   = 2 = ON
// gridBox(CCW) + outerBoundary(CCW) - hole(CW) = 1+1-1 = 1 = OFF


// ACHTUNG: Konturen p1 und p2 muessen ziemlich genau in einer Plane liegen !!!
// werden NICHT von GLU in eine durch Normalvec definierte Plane projiziert !!!
//
//

// see also GLT_spp__ GLT_spu_ GLT_diff_work GLT_comm_work 


  int       ii, i1, i2, i3, i4, ip1, ip2, ip3, ip4, p1Nr, p2Nr, itNr;
  int       igSiz, *ia;
  Point     *pa, *p1, *p2, *p3, *p4;
  IndTab   *itAct, *itBnd;


  // printf("================ GLU_tess__ elNr=%d\n",MEMTAB_IND (cmt));
  // UT3D_stru_dump (Typ_VC, vcz, "  vcz:");

  GLU_Err = 0;

  GLU_pmt = pmt;  // add additional points here
  GLU_imt = imt;  // store point-indices for tesselated faces here
  GLU_fmt = fmt;  // store tesselated faces here
  GLU_pst = pst;  // store boundary-flags for pmt-points here

  itNr = MEMTAB_IND (cmt);  // nr of contours
  itBnd = MEMTAB_DAT (cmt); // contours
    // printf(" pmt-ind=%d\n",MEMTAB_IND (GLU_pmt));


  // create tess-obj
  // if(ot1) gluDeleteTess(ot1); ot1 = gluNewTess();
  if(ot1 == NULL) { TX_Error("GLU_tess__ E001"); return -1;}


  // define the callback-functions
  gluTessCallback (ot1, GLU_TESS_ERROR,    (void*)&GLU_CB_tess_Err);
  gluTessCallback (ot1, GLU_TESS_COMBINE,  (void*)&GLU_CB_tess_newPt);
  gluTessCallback (ot1, GLU_TESS_BEGIN,    (void*)&GLU_CB_tess_beg);
  gluTessCallback (ot1, GLU_TESS_VERTEX,   (void*)&GLU_CB_tess_pt);
  gluTessCallback (ot1, GLU_TESS_EDGE_FLAG,(void*)&GLU_CB_tess_edg);
  gluTessCallback (ot1, GLU_TESS_END,      (void*)&GLU_CB_tess_end);



  //================================================================
  if(gb) {
    // gridbox(CCW) + outerbound(CCW) = 2 = ON
    // gridbox(CCW) + outerbound(CCW) - innerbound(CW) = 1 = OFF
    // Fill ABSolute values Greater than EQual to TWO 
    // grd(CCW)+OB(CCW)=2; grd(CCW)+OB(CCW)-IB(CW)=1;
    gluTessProperty (ot1, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ABS_GEQ_TWO);
      // printf(" GLU_TESS_WINDING_ABS_GEQ_TWO\n");

  } else {
    // outerBoundary(CCW)                  = 1 = ON
    // outerBoundary(CCW) - innerbound(CW) = 0 = OFF
    gluTessProperty (ot1, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);
      // printf(" GLU_TESS_WINDING_NONZERO\n");

  }

  // triangles, not boundary-lines
  gluTessProperty(ot1, GLU_TESS_BOUNDARY_ONLY, GL_FALSE);

  // define tol: tut gar nix ..
  // gluTessProperty(ot1, GLU_TESS_TOLERANCE, UT_TOL_cv);

  // normalvector
  if(vcz) {
    gluTessNormal(ot1, vcz->dx, vcz->dy, vcz->dz);
      UT3D_stru_dump(Typ_VC, vcz, " vcz:");
  } else {
    gluTessNormal (ot1, 0., 0.,  -1.);
    // defines inside-outsied; +1=inside..??
  }




  //================================================================
  gluTessBeginPolygon (ot1, NULL);
  // gluTessBeginPolygon (ot1, &iEl);
    // reports iEl in eg GLU_TESS_VERTEX_DATA-callback
    // data (a index) cannot be provided additional to each vertex.

    //----------------------------------------------------------------
    // all gridboxes out
    if(!gb) goto L_OB;


    // create gridboxes
    // loop vertical with i1
    p1Nr = gb->iy - 1;
    p2Nr = gb->ix - 1;
      // printf(" p1Nr=%d p2Nr=%d\n", p1Nr, p2Nr);
    pa = gb->p1;   // first point of gridbox
    for(i1=0; i1<p1Nr; ++i1) {
      // loop horizontal with i2
      ii = i1 * gb->ix;  // ipos of 1. point in row
      for(i2=0; i2<p2Nr; ++i2) {
        // CCW:
        ip1 = ii;
        ip2 = ii + 1;
        ip3 = ii + gb->ix + 1;
        ip4 = ii + gb->ix;
          // printf(" iga = %d %d %d %d\n",ip1,ip2,ip3,ip4);
          // UT3D_stru_dump (Typ_PT, &pa[ip1], "ip1");
          // UT3D_stru_dump (Typ_PT, &pa[ip2], "ip2");
          // UT3D_stru_dump (Typ_PT, &pa[ip3], "ip3");
          // UT3D_stru_dump (Typ_PT, &pa[ip4], "ip4");

        gluTessBeginContour (ot1);
          gluTessVertex (ot1, (double*)&pa[ip1], (double*)&pa[ip1]);
          gluTessVertex (ot1, (double*)&pa[ip2], (double*)&pa[ip2]);
          gluTessVertex (ot1, (double*)&pa[ip3], (double*)&pa[ip3]);
          gluTessVertex (ot1, (double*)&pa[ip4], (double*)&pa[ip4]);
        gluTessEndContour (ot1);

        if(GLU_Err) {gluTessEndPolygon(ot1); goto L_glu_err;}
        ++ii;
      }
    }
      // printf(" nach gridBoxes ..\n");
      // printf(" pmt-ind=%d\n",MEMTAB_IND (GLU_pmt));


    //----------------------------------------------------------------
    L_OB:
      // printf(" L_OB:\n");
    // all boundaries out; last point not necessary.
    if(itNr < 1) goto L_end;
    pa   = MEMTAB_DAT(pmt);
    // outer-boundary has correct direction; revert all inner-bounds.
    itAct = &itBnd[0];
    p2Nr = itAct->iNr;
    --p2Nr;  // remove last point (else (BUG) GLU returns this pt as new pt)
    ia   = MEMTAB__ (imt, itAct->ibeg);

    gluTessBeginContour (ot1);
    for(i2=0; i2 < p2Nr; ++i2) {
      ii = ia[i2];
      gluTessVertex (ot1, (double*)&pa[ii], (double*)&pa[ii]);
        // UT3D_stru_dump (Typ_PT, &pa[ii], " OB p[%d] ii=%d",i2,ii);
    }
    gluTessEndContour (ot1);

    if(GLU_Err) {gluTessEndPolygon(ot1); goto L_glu_err;}
        // printf(" nach kontur OB\n");

    // give inner-bounds
    for(i1=1; i1<itNr; ++i1) {
      itAct = &itBnd[i1];
        // printf(" IB %d\n",i1);
      gluTessBeginContour (ot1);
      p2Nr = itAct->iNr;
      --p2Nr;  // remove last point (else (BUG) GLU returns this pt as new pt)
      --p2Nr;
      ia   = MEMTAB__ (imt, itAct->ibeg);
      for(i2=p2Nr; i2>=0; --i2) {
        ii = ia[i2];
        gluTessVertex (ot1, (double*)&pa[ii], (double*)&pa[ii]);
          // UT3D_stru_dump (Typ_PT, &pa[ii], " IB-C%d p[%d] ii=%d",i1,i2,ii);
      }
      gluTessEndContour (ot1);
      if(GLU_Err) {gluTessEndPolygon(ot1); goto L_glu_err;}
        // printf(" nach kontur IB %d\n",i1);
    }


  L_end:
  gluTessEndPolygon(ot1);
    // printf(" nach gluTessEndPolygon\n");

  // delete tess-obj, free mem.
  // gluDeleteTess(ot1);

  // printf("ex GLT_comm_work %d\n",*p3Nr);
  return GLU_Err;

  L_glu_err:
    printf("GLT_comm_work Error %d\n",GLU_Err);
    return GLU_Err;

}



//================================================================
//================================================================
// old functions:
//================================================================
//================================================================

//================================================================
  long GLT_mSiz () {
//================================================================

  long   mSiz;

  printf("GLT_mSiz: GLT_pta_ind=%ld GLT_cta_ind=%ld\n",GLT_pta_ind,GLT_cta_ind);

  mSiz = GLT_pta_ind * sizeof(Point) + GLT_cta_ind * sizeof(ObjGX);

  printf("ex GLT_mSiz: mSiz=%ld\n",mSiz);

  return mSiz;

}

 
//=========================================================
  int GLT_alloc_pta (long Ind) {
//=========================================================
// realloc GLT_pta


  long i1, newSiz;

  // printf("GLT_alloc_pta %d %d\n",Ind,GLT_pta_SIZ);


  if((Ind == 0)&&(GLT_pta != NULL)) {
    // re-init
    newSiz = GLT_pta_SIZ;
    GLT_pta_SIZ = 0;
    goto L_init;
  }

  Ind += 2;
  if(GLT_pta_SIZ > Ind) return 0;


  newSiz = GLT_pta_SIZ + GLT_pta_INC;
  while (Ind >= newSiz) newSiz += GLT_pta_INC;


  printf("::::GLT_alloc_pta %ld %ld %ld %d\n",Ind,GLT_pta_SIZ,newSiz,GLT_pta_INC);
  // printf("  GLT_pta = %p\n",GLT_pta);


  GLT_pta = (Point*)realloc(GLT_pta, newSiz * sizeof(Point));

  if(GLT_pta == NULL) {
    TX_Error ("******** out of memory - GLT_alloc_pta *********");
    return -1;
  }

  L_init:
  GLT_pta_SIZ = newSiz;

    // printf("ex GLT_alloc_pta %d %d %d\n",Ind,GLT_pta_SIZ,newSiz);
    // printf("  GLT_pta = %p\n",GLT_pta);

  // AP_errStat_set (2);  // restart !!

  // GLT_exit ();
  // GLT_init ();


  return -10;   // gesamte Flaeche nochmal ..

}


//=========================================================
  int GLT_alloc_cta (long Ind) {
//=========================================================
// realloc GLT_cta

  long i1, newSiz;


  // printf("GLT_alloc_cta %ld\n",Ind);


  if((Ind == 0)&&(GLT_cta != NULL)) {
    newSiz = GLT_cta_SIZ;
    GLT_cta_SIZ = 0;
    goto L_init;
  }

  if(GLT_cta_SIZ > Ind) return 0;

  newSiz = GLT_cta_SIZ + GLT_cta_INC;

  printf("::::GLT_alloc_cta %ld %ld %ld %d\n",Ind,GLT_cta_SIZ,newSiz,GLT_cta_INC);

  while (Ind >= newSiz) newSiz += GLT_cta_INC;
  // printf("  %d %d %d\n",Ind,GLT_pta_SIZ,newSiz);

  GLT_cta = (ObjGX*)realloc(GLT_cta, (newSiz+2)*sizeof(ObjGX));

  if(GLT_cta == NULL) {
    TX_Error ("******** out of memory - GLT_alloc_cta *********");
    return -1;
  }

  L_init:
  GLT_cta_SIZ = newSiz;

  return -10;   // gesamte Flaeche nochmal ..

}


//=========================================================
  int GLT_alloc_ppa (long Ind) {
//=========================================================
// realloc GLT_cta

  long i1, newSiz;


  // printf("GLT_alloc_cta %ld\n",Ind);


  if((Ind == 0)&&(GLT_ppa != NULL)) {
    newSiz = GLT_ppa_SIZ;
    GLT_ppa_SIZ = 0;
    goto L_init;
  }

  if(GLT_ppa_SIZ > Ind) return 0;

  newSiz = GLT_ppa_SIZ + GLT_ppa_INC;

  printf("::::GLT_alloc_ppa %ld %ld %ld %d\n",Ind,GLT_ppa_SIZ,newSiz,GLT_ppa_INC);

  while (Ind >= newSiz) newSiz += GLT_ppa_INC;
  // printf("  %d %d %d\n",Ind,GLT_ppa_SIZ,newSiz);

  GLT_ppa = (ObjGX*)realloc(GLT_ppa, (newSiz+2)*sizeof(ObjGX));

  if(GLT_ppa == NULL) {
    TX_Error ("******** out of memory - GLT_alloc_ppa *********");
    return -1;
  }

  L_init:
  GLT_ppa_SIZ = newSiz;

  return -10;   // gesamte Flaeche nochmal ..

}


//=========================================================
  int GLT_exit () {
//=========================================================

  // printf("GLT_exit\n");

  if(ot1) {
    gluDeleteTess(ot1);
    ot1 = NULL;
  }

  return 0;

}


//=========================================================
  int GLU_alloc (int Ind) {
//=========================================================
// realloc GLU_CombTab

#define GLU_CombINC 400

  int   newSiz;


  // printf("GLU_alloc %ld\n",Ind);


  if(Ind == 0) GLU_CombNr = 0;     // reset

  if(GLU_CombSiz > Ind) return 0;


  newSiz = 0;
  while (Ind >= newSiz) newSiz += GLU_CombINC;
  // printf("  %d %d %d\n",Ind,GLU_CombSiz,newSiz);


  printf("::::GLU_alloc %d %d %d %d\n",Ind,GLU_CombSiz,newSiz,GLU_CombINC);

  GLU_CombTab = (Point*)realloc(GLU_CombTab, (newSiz+2)*sizeof(Point));

  if(GLU_CombTab == NULL) {
    TX_Error ("******** out of memory - GLU_alloc *********");
    return -1;
  }

  L_init:
  GLU_CombSiz = newSiz;

  // printf("ex GLU_alloc %d %d %d\n",Ind,GLU_CombSiz,newSiz);

  // GLT_exit ();
  // GLT_init ();

  // AP_errStat_set (2);  // restart !!

  return -10; // -10 = reRun

}






//=========================================================
  int GLT_init () {
//=========================================================
// init userfunction; ganz am Anfang und bei jeden new; sonst nie.


  // printf("GLT_init\n");



#ifndef GLU_VERSION_1_2
  TX_Error("need GLU_VERSION_1_2 - support");
  return -1;
#endif

/*
#ifndef GLU_VERSION_1_3
  TX_Error("need GLU_VERSION_1_3 - support");
  return -1;
#endif
*/

  // printf("%s\n",gluGetString(GLU_VERSION));

  GLT_alloc_pta (1);
  GLT_alloc_cta (1);
  GLT_alloc_ppa (1);

  GLU_alloc (1);

  // create tess-obj
  // GLT_exit ();
  if(ot1 == NULL) ot1 = gluNewTess();
  if(ot1 == NULL) { TX_Error("tess__ E001"); return -1;}


  return 0;

}

/*
//=========================================================
  void tcb_begin (GLenum which) {
//=========================================================

  printf("tcb_begin %d\n",which);
  glBegin(which);
  return;
}


//=========================================================
  void tcb_end () {
//=========================================================

  printf("tcb_end\n");
  glEnd();
  return;
}


//=========================================================
  void tcb_vertex (GLvoid *vertex) {
//=========================================================

  printf("tcb_vertex\n");
  glVertex3dv(vertex);
  return;
}
*/


//=========================================================
  int GLT_comb_reset () {
//=========================================================
// eigentlich nur f tests von extern

  GLU_CombNr = 0;

  return 0;

}

//=========================================================
  int GLT_dump_comb () {
//=========================================================

  int i1;

  for(i1=0; i1<GLU_CombNr; ++i1) 
         UT3D_stru_dump(Typ_PT, &GLU_CombTab[i1], " comb[%d]",i1);

  return 0;

}


//=========================================================
  GLU_CB GLT_comb_CB (GLdouble coords[3], GLdouble *data[4],
                  GLfloat weight[4],  GLdouble **dataOut) {
//=========================================================
// add new vertex to mesh.
// dataOut ist ein kompletter VERTEX; x,y,z,r,g,b,a;
// rgba sind floats!
// es muss nur static memspc fuer die daten zur Verfuegung gestellt werden ?
// dieser Platz wird benoetigt bis nach gluTessEndPolygon.


  // printf(" GLT_comb_CB %d %d\n",GLU_CombNr,GLU_CombSiz);
  // printf(" GLT_comb_CB %d %f,%f,%f\n",GLU_CombNr,
        // coords[0],coords[1],coords[2]);



  if(GLU_CombNr >= GLU_CombSiz) {
    // if(GLU_alloc(GLU_CombNr) < 0) {
    GLU_Err = GLU_alloc (GLU_CombNr);
    if(GLU_Err < 0) {
      // TX_Error("GLT_comb_CB E001");
      // TX_Print("GLT_comb_CB E001");
      printf("GLT_comb_CB I001\n");
      return;
    }
  }


  memcpy (&GLU_CombTab[GLU_CombNr],  coords, sizeof(Point));

  *dataOut = (GLdouble*)&GLU_CombTab[GLU_CombNr];

  ++GLU_CombNr;

  return;
}


//=========================================================
  GLU_CB GLT_CB_Err (GLenum gl_err) {
//=========================================================

  const GLubyte   *estring;


  // printf("GLT_CB_Err %d\n",gl_err);

  estring = gluErrorString (gl_err);

  printf("***** GLT_CB_Err |%s|\n", estring);

  // TX_Print("GLT_CB_Err %s",estring);

  GLU_Err = -1;

  return;
}


//=======================================================================
  int GLT_spu_ (int pNr, Point *pta, Vector *vcz) {
//=======================================================================
// eine nicht gelochte planare Flaeche tesselieren ..
// ACHTUNG: keine Ausgabe --> vMem; use GLT_spp_

  int      iNr, i2, dir = '-';


  // printf("================================= GLT_spu_ %d\n",pNr);
  // UT3D_stru_dump (Typ_VC, vcz, "  vcz:");


  //=============================================================
  if(ot1 == NULL) { TX_Error("GLT_spu_ E001"); return -1;}


  // define the callback-functions
  gluTessCallback(ot1, GLU_TESS_BEGIN,   (void*)&glBegin);
  gluTessCallback(ot1, GLU_TESS_VERTEX,  (void*)&glVertex3dv);
  gluTessCallback(ot1, GLU_TESS_COMBINE, (void*)&GLT_comb_CB);
  gluTessCallback(ot1, GLU_TESS_ERROR,   (void*)&GLT_CB_Err);
  gluTessCallback(ot1, GLU_TESS_END,     (void*)&glEnd);


  // Z-Vektor
  gluTessNormal(ot1, vcz->dx, vcz->dy, vcz->dz);

  // Z-vec direct an GL
  glNormal3dv ((double*)vcz);

  // default = _ODD = doppeltes Loch ist positiv
  gluTessProperty(ot1, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);

  // Flaeche nicht Boundary
  gluTessProperty(ot1, GLU_TESS_BOUNDARY_ONLY, GL_FALSE);




  //---------------------------------------------------------
  gluTessBeginPolygon(ot1, NULL);

      // iNr = pNr - 1;  // Anzahl Punkte der aktiven contour ohne letzten
      iNr = pNr;  // bei mehreren Konturen gibts Problem ohnde dem letzen Pt.

      gluTessBeginContour(ot1);

      // if(dir == '-') {
        // printf(" dir - |%c|\n",dir);
        // for(i2=ipa; i2<ipe; ++i2) {
        for(i2=0; i2<iNr; ++i2) {
          // gluTessVertex(ot1, (double*)&pTab[i2], (double*)&pTab[i2]);
          // UT3D_stru_dump(Typ_PT, &pta[i2], " spu[%d]",i2);
          gluTessVertex(ot1, (double*)&pta[i2], (double*)&pta[i2]);
          // if(i2==5){gluTessEndContour(ot1);gluTessBeginContour(ot1);}
        }
/*
      } else {
        printf(" dir + |%c|\n",dir);
        // for(i2=ipe-1; i2>=ipa; --i2) {
        for(i2=iNr-1; i2>=0; --i2) {
          // gluTessVertex(ot1, (double*)&pTab[i2], (double*)&pTab[i2]);
          gluTessVertex(ot1, (double*)&pta[i2], (double*)&pta[i2]);
        }
      }
*/
      gluTessEndContour(ot1);

  gluTessEndPolygon(ot1);

  return 0;

}


//=======================================================================
  int GLT_spp__ (ContTab *cTab, int cNr, Vector *vcz) {
//=======================================================================
// GLT_spp__      tesselate planaren Patch mit Aussenkontur/Loechern
// alle Flaechen in cTab tesselieren; out via GLT_spp_vert usw


  int           i1, i2, iNr;
  Point         *pta;


  // printf("GLT_spp__  %d\n",cNr);
    // for(i1=0; i1<cNr; ++i1) {
      // iNr = cTab[i1].iNr;  
      // pta = cTab[i1].pa;
      // for(i2=0; i2<iNr; ++i2) {
        // UT3D_stru_dump (Typ_PT, &pta[i2], " c-%d  pt[%d]",i1,i2);
      // }
    // }


  // if((cNr < 1)||(cNr > 2)) {
    // printf("--------------------- GLT_spp__ %d\n",cNr);
    // // UT3D_vc_invert (vcz, vcz);
    // UT3D_stru_dump (Typ_VC, vcz, "   vcz");
  // }


  // reset GLU-combineBuffer
  // GLT_comb_reset ();
  GLU_CombNr  = 0;


  // // init memspc
  // if(GLT_pln_ini() < 0) return -1;

  // NUR TEST: Init
  // GLT_exit (); GLT_init ();


  //=============================================================
  // create tess-obj
  // if(ot1 == NULL) ot1 = gluNewTess();
  if(ot1 == NULL) { printf("GLT_spp__ E001\n"); return -1;}

  // glEnable (GL_LIGHTING);


  // define the callback-functions
  gluTessCallback(ot1, GLU_TESS_BEGIN,   (void*)&GLT_spp_beg);
  gluTessCallback(ot1, GLU_TESS_VERTEX,  (void*)&GLT_spp_vert);
  gluTessCallback(ot1, GLU_TESS_COMBINE, (void*)&GLT_comb_CB);
  gluTessCallback(ot1, GLU_TESS_ERROR,   (void*)&GLT_CB_Err);
  gluTessCallback(ot1, GLU_TESS_END,     (void*)&GLT_spp_end);


  gluTessProperty(ot1, GLU_TESS_TOLERANCE, 0.01);

  // Z-Vektor
  gluTessNormal(ot1, vcz->dx, vcz->dy, vcz->dz);

  // Z-vec direct an GL
  glNormal3dv ((double*)vcz);

// 2005-10-12: GLU_TESS_WINDING_POSITIVE korr. --> GLU_TESS_WINDING_ODD; RF.
  // wenn sich Loecher ueberdecken, bleibts damit negativ
  // (default = _ODD = doppeltes Loch ist positiv !)
  // NEG: subract ist OK, aber Loch muss CW sein !!!
  gluTessProperty(ot1, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);
  // gluTessProperty(ot1, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);
  // gluTessProperty(ot1, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ABS_GEQ_TWO);
// gluTessProperty(ot1, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_POSITIVE);
  // gluTessProperty(ot1, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NEGATIVE);

  // Flaeche nicht Boundary
  gluTessProperty(ot1, GLU_TESS_BOUNDARY_ONLY, GL_FALSE);


  //---------------------------------------------------------
  gluTessBeginPolygon(ot1, NULL);

    // // Z-Vektor
    // gluTessNormal(ot1, vcz->dx, vcz->dy, vcz->dz);

    // // Z-vec direct an GL
    // glNormal3dv ((double*)vcz);

    for(i1=0; i1<cNr; ++i1) {

      // ohne letzen (= ersten) Punkt gibts bei mehreren Aussenkonturen Fehler.
      iNr = cTab[i1].iNr;  // Anzahl Punkte der aktiven contour
      // if((iNr < 4)||(iNr > 50))
      // printf(" cont[%d] iNr=%d dir=%c\n",i1,iNr,cTab[i1].dir);
      pta = cTab[i1].pa;


      // start contours; first=outerBoudary; all following contours are holes.
      gluTessBeginContour(ot1);

      if(cTab[i1].dir == '-') {
        for(i2=0; i2<iNr; ++i2) {
          gluTessVertex(ot1, (double*)&pta[i2], (double*)&pta[i2]);
        }
      } else {
        for(i2=iNr-1; i2>=0; --i2) {
          gluTessVertex(ot1, (double*)&pta[i2], (double*)&pta[i2]);
        }
      }
      gluTessEndContour(ot1);
      if(GLU_Err != 0) {gluTessEndPolygon(ot1); goto L_glu_err;}
    }


  gluTessEndPolygon(ot1); // start tesselation ..
  // the patch-points come with GLT_spp_vert


  return GLU_Err;


  L_glu_err:
    printf("GLT_spp__ Error %d\n",GLU_Err);
    return GLU_Err;

}


//================================================================
  int GLT_diff_work (int *c1Nr, ContTab *c1Tab,
                         int *p1Nr, Point *p1Tab,
                         int pNr,   Point   *pTab,
                         int cNr,   ContTab *cTab, Vector *vcz) {
//================================================================
// difference of 2 contours (p1 = diff of pTab - c1 - c2 cn)
// diff = der Restbereich; pTab ist Aussenkontur, cTab[1-n] sind Lochkonturen.
// Erste Kontur in cTab ist (alte) Aussenkontur (skippen),
//   alle folgenden sind Lochkonturen.

// INPUT: pTab[pNr] = Aussenkontur
//        cTab[cNr] = cTab[1-n] sind Lochkonturen
//
// ACHTUNG: alle Konturen muessen ziemlich genau in einer Plane liegen !!!
// werden NICHT von GLU in eine durch Normalvec definierte Plane projiziert !!!
//
// INPUT:
//   pTab[pNr]   (outer-boundary)
//   cTab[cNr]   holes  (cTab[].p2a)
//   p1Nr        maxSize of p1Tab!
// OUTPUT:
//   p1Tab       Ergebniskontur
//
// v /mnt/serv1/Devel/dev/OpenGL/doc/GLU_V1.3.pdf

  int           i1, i2, iNr;
  Point         *pta;


  // printf("=====================GLT_diff_work comm_Max=%d\n",*p1Nr);
  // UT3D_stru_dump (Typ_VC, vcz, "  vcz:");


  // GLT_exit ();  ot1 = gluNewTess();  // TEST



  // reset Callback-functions
  GLU_comm_tab  = p1Tab;
  GLU_comm_Max  = *p1Nr;
  GLU_comm_pNr  = 0;

  GLU_cTab = c1Tab;
  GLU_cMax = *c1Nr;
  GLU_cNr  = 0;


  // reset GLU-combineBuffer
  GLU_CombNr  = 0;


  // create tess-obj
  if(ot1 == NULL) { TX_Error("GLT_diff_work E001"); return -1;}


  // glRenderMode(GL_FEEDBACK);


  // statt tesselieren nur die Boundary liefern: (m. GL_LINE_LOOP):
  gluTessProperty(ot1, GLU_TESS_BOUNDARY_ONLY, GL_TRUE);


  // union oder a-b oder b-a aber nicht das gemeinsame von a,b
  gluTessProperty(ot1, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_POSITIVE);

  // gluTessProperty(ot1, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);
  // gluTessProperty(ot1, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);
  // gluTessProperty(ot1, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ABS_GEQ_TWO);
  // gluTessProperty(ot1, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NEGATIVE);


  // // define tol: tut gar nix ..
  // gluTessProperty(ot1, GLU_TESS_TOLERANCE, .001);


  // define callbacks
  gluTessCallback(ot1, GLU_TESS_BEGIN,   (void*)&GLT_CB_Begin);
  gluTessCallback(ot1, GLU_TESS_VERTEX,  (void*)&GLT_comm_CB);
  gluTessCallback(ot1, GLU_TESS_COMBINE, (void*)&GLT_comb_CB);
  gluTessCallback(ot1, GLU_TESS_END,     (void*)&GLT_CB_End);
  gluTessCallback(ot1, GLU_TESS_ERROR,   (void*)&GLT_CB_Err);





  //---------------------------------------------------------
  // Z-Vektor
  gluTessNormal(ot1, vcz->dx,vcz->dy,vcz->dz);
  // gluTessNormal(ot1, 0., 0., 1.);
  // gluTessNormal(ot1, 0., 0., -1.);
  // gluTessNormal(ot1, 0., 0., 0.);




  gluTessBeginPolygon(ot1, NULL);


    // give Aussenkontur
    // if(cTab[i1].dir == '-')
    // gluTessNormal(ot1, vcz->dx,vcz->dy,vcz->dz);
    // else
    // gluTessNormal(ot1, -vcz->dx,-vcz->dy,-vcz->dz);
    gluTessBeginContour(ot1);

    for(i1=0; i1<pNr-1; ++i1) {
    // for(i1=0; i1<pNr; ++i1) {
      // for(i1=pNr-1; i1>=0; --i1) {    // wenn CW-Inputkontur !
      // UT3D_stru_dump(Typ_PT, &pTab[i1], "A:%d",i1);
      gluTessVertex(ot1, (double*)&pTab[i1], (double*)&pTab[i1]);
    }
    gluTessEndContour(ot1);




    // give Innenkonturen
    for(i1=1; i1<cNr; ++i1) {  // 0=Orig.Aussenkontur; skippen.

      if(cTab[i1].use != 'A') continue;

      iNr = cTab[i1].iNr - 1;  // Anzahl Pt der aktiven contour ohne letzten
      // iNr = cTab[i1].iNr;  // Anzahl Pt der aktiven contour ohne letzten
      pta = cTab[i1].p2a;
      // printf(" cont[%d] dir=%c pNr=%d\n",i1,cTab[i1].dir,iNr);


      gluTessBeginContour(ot1);

      if(cTab[i1].dir == '-') {
        for(i2=0; i2<iNr; ++i2) {     // skips pt[iNr]
          // UT3D_stru_dump(Typ_PT, &pta[i2], "-I%d %d",i1,i2);
          gluTessVertex(ot1, (double*)&pta[i2], (double*)&pta[i2]);
        }
      } else {
        for(i2=iNr-1; i2>=0; --i2) {     // skips pt[0]
          // UT3D_stru_dump(Typ_PT, &pta[i2], "+I%d %d",i1,i2);
          gluTessVertex(ot1, (double*)&pta[i2], (double*)&pta[i2]);
        }
      }
      gluTessEndContour(ot1);
      // if(GLU_Err != 0) {gluTessEndPolygon(ot1); goto L_glu_err;}
    }
    // gluTessEndContour(ot1);


  gluTessEndPolygon(ot1);



  //======================================


  *p1Nr = GLU_comm_pNr;
  *c1Nr = GLU_cNr;

  // glRenderMode(GL_RENDER);

  // printf("ex GLT_diff_work %d %d\n",*c1Nr,*p1Nr);

  return 0;

}

 
//===================================================================
  int GLT_comm_work (int *cNr, ContTab *cTab,
                         int *p3Nr, Point *p3Tab,
                         int p1Nr, Point *p1Tab,
                         int p2Nr, Point *p2Tab, Vector *vcz) {
//===================================================================
// common of 2 contours (p3 = common of p1 + p2).        INTERSECTION
// common = der gemeinsame Bereich (Bereich ist in p1 UND in p2 enthalten).
// Test GE_EQU und POSITIV im gleichen Lauf geht nicht !
//
// ACHTUNG: Konturen p1 und p2 muessen ziemlich genau in einer Plane liegen !!!
// werden NICHT von GLU in eine durch Normalvec definierte Plane projiziert !!!
//
// Drehsinn der Konturen soll entgegengesetzt sein.
// Aussenkontur CCW
// Innenkontur CW
// Ergebniskontur wird dann ?
//
// INPUT:
//   p2Tab[p3Nr]         1. contour
//   p1Tab[p1Nr]         2. contour
// OUTPUT:
//   p3Tab[p3Nr]         common contour
//   cTab[GLU_cNr]p2a
//   cTab[GLU_cNr].iNr

  int       i1;

  // FILE      *ft;  // nur TEST
  // ft = fopen ("gluerr.dat","wb");



  // printf("================ GLT_comm_work p1=%d p2=%d\n",p1Nr,p2Nr);
  // UT3D_stru_dump (Typ_VC, vcz, "  vcz:");


  GLU_comm_tab    = p3Tab; // output of GLT_comm_CB
  GLU_comm_Max    = *p3Nr;
  GLU_comm_pNr    = 0;

  GLU_cTab = cTab;
  GLU_cMax = *cNr;
  GLU_cNr  = 0;

  // GLT_comb_reset();
  GLU_CombNr = 0;




  // create tess-obj
  // if(ot1) gluDeleteTess(ot1); ot1 = gluNewTess();
  if(ot1 == NULL) { TX_Error("GLT_comm_work E001"); return -1;}

  // gluTessBeginPolygon(ot1, NULL);
  // gluTessEndPolygon(ot1);

  // define the callback-functions
  // GLU_TESS_EDGE_FLAG: damit werden nur 3Ecke geliefert !
  // Else gibts auch FANS's und STRIP's.
  // gluTessCallback(ot1, GLU_TESS_EDGE_FLAG,(void*)&GLT_CB_edge);

  // gluTessCallback(ot1, GLU_TESS_EDGE_FLAG, NULL);
  // gluTessCallback(ot1, GLU_TESS_BEGIN,     NULL);

  gluTessCallback(ot1, GLU_TESS_COMBINE, (void*)&GLT_comb_CB);
  gluTessCallback(ot1, GLU_TESS_VERTEX,  (void*)&GLT_comm_CB);
  gluTessCallback(ot1, GLU_TESS_ERROR,   (void*)&GLT_CB_Err);
  gluTessCallback(ot1, GLU_TESS_END,     (void*)&GLT_comm_end);

  // gluTessCallback(ot1, GLU_TESS_COMBINE, (GLvoid (*) ()) &GLT_comb_CB);
  // gluTessCallback(ot1, GLU_TESS_VERTEX,  (GLvoid (*) ()) &GLT_comm_CB);
  // gluTessCallback(ot1, GLU_TESS_ERROR,   (GLvoid (*) ()) &GLT_CB_Err);
  // gluTessCallback(ot1, GLU_TESS_END,     (GLvoid (*) ()) &GLT_comm_end);


  //================================================================


  // wenn sich Loecher ueberdecken, bleibts damit negativ
  // (default = _ODD = doppeltes Loch ist positiv !)
  // NEG: subract ist OK, aber Loch muss CW sein !!!
  // gluTessProperty(ot1, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);
  // gluTessProperty(ot1, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);

  // gemeinsamer Bereich von a und b;
  gluTessProperty(ot1, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ABS_GEQ_TWO);

  // union oder a-b oder b-a aber nicht das gemeinsame von a,b
  // gluTessProperty(ot1, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_POSITIVE);

  // macht nur union
  // gluTessProperty(ot1, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NEGATIVE);

  // statt tesselieren nur die Boundary liefern: (m. GL_LINE_LOOP):
  gluTessProperty(ot1, GLU_TESS_BOUNDARY_ONLY, GL_TRUE);


  // // define tol: tut gar nix ..
  // gluTessProperty(ot1, GLU_TESS_TOLERANCE, .1);


  //================================================================
  gluTessNormal(ot1, vcz->dx, vcz->dy, vcz->dz);
  // gluTessNormal(ot1, -vcz->dx, -vcz->dy, -vcz->dz);
  // gluTessNormal(ot1, 0., 0., -1.);


  gluTessBeginPolygon(ot1, NULL);

    gluTessBeginContour(ot1);

      // X-Y-Plane
      // gluTessNormal(ot1, vcz->dx, vcz->dy, vcz->dz);
      // gluTessNormal(ot1, -vcz->dx, -vcz->dy, -vcz->dz);
      // gluTessNormal(ot1, 0., 0., 0.);

      for(i1=0; i1<p2Nr-1; ++i1) {
      // for(i1=p2Nr-2; i1>=0; --i1) {
        // UT3D_stru_dump(Typ_PT, &p2Tab[i1], "1:%d",i1);
        // printf(" out[%d]=%f,%f,%f\n",i1,p2Tab[i1].x,p2Tab[i1].y,p2Tab[i1].z);
        // fwrite(&p2Tab[i1], sizeof(Point), 1, ft);
        gluTessVertex(ot1, (double*)&p2Tab[i1], (double*)&p2Tab[i1]);
      }

      // ohne die naechsten 2 zeilen geht das erste Feld einer Flaeche nicht !
      gluTessEndContour(ot1);
      if(GLU_Err != 0) {gluTessEndPolygon(ot1); goto L_glu_err;}

      gluTessBeginContour(ot1);
      // gluTessNormal(ot1, 0., 0., 0.);
      // gluTessNormal(ot1, -vcz->dx, -vcz->dy, -vcz->dz);

      for(i1=0; i1<p1Nr-1; ++i1) {
      // for(i1=p1Nr-2; i1>=0; --i1) {
        // UT3D_stru_dump(Typ_PT, &p1Tab[i1], "2:%d",i1);
        // printf(" inn[%d]=%f,%f,%f\n",i1,p1Tab[i1].x,p1Tab[i1].y,p1Tab[i1].z);
        // fwrite(&p1Tab[i1], sizeof(Point), 1, ft);
        gluTessVertex(ot1, (double*)&p1Tab[i1], (double*)&p1Tab[i1]);
      }

      gluTessEndContour(ot1);
      if(GLU_Err != 0) {gluTessEndPolygon(ot1); goto L_glu_err;}
      // printf(" nach kontur2\n");

  gluTessEndPolygon(ot1);
  // printf(" nach gluTessEndPolygon\n");


  // glEndList();

  // delete tess-obj, free mem.
  // gluDeleteTess(ot1);

  // fclose(ft);


  //======================================
  *p3Nr = GLU_comm_pNr;
  *cNr  = GLU_cNr;



  // GLT_dump_comb ();  // TEST
  // printf("ex GLT_comm_work %d\n",*p3Nr);
  return GLU_Err;



  L_glu_err:
    printf("GLT_comm_work Error %d\n",GLU_Err);
    return GLU_Err;

}


//=========================================================
  GLU_CB GLT_comm_CB (void *vertex) {
//=========================================================

  // printf(" GLT_comm_CB %d %f,%f,%f\n",GLU_comm_pNr,
        // ((Point*)vertex)->x,((Point*)vertex)->y,((Point*)vertex)->z);


  if(GLU_Err < 0) return;

  memcpy (&GLU_comm_tab[GLU_comm_pNr], vertex, sizeof(Point));

  // GR_Disp_pt (&GLU_comm_tab[GLU_comm_pNr], SYM_STAR_S, 2);

  if(GLU_comm_pNr < GLU_comm_Max-1) ++GLU_comm_pNr;
  else TX_Error("GLT_comm_CB EOM");


  return;
}



/*
//=========================================================
  void GLT_CB_edge (GLboolean flag) {
//=========================================================

  // printf("GLT_CB_edge %d\n",flag);

  return 0;

}
*/



//=========================================================
  GLU_CB GLT_comm_end () {
//=========================================================

static int ptOff;

  // printf(" GLT_comm_end pNr=%d\n",GLU_comm_pNr);

  if(GLU_cNr == 0) {
    GLU_cTab[0].p2a = &GLU_comm_tab[0];
    ptOff = 0;
  }

  // eine Kontur fertig;
  if(GLU_comm_pNr > 0) {
    if(GLU_comm_pNr < GLU_comm_Max-1) {
      GLU_comm_tab[GLU_comm_pNr] = GLU_comm_tab[ptOff];
      ++GLU_comm_pNr;
    }
  }

  GLU_cTab[GLU_cNr].iNr = GLU_comm_pNr - ptOff;

  ptOff = GLU_comm_pNr;

  if(GLU_cNr < GLU_cMax-1) ++GLU_cNr;

  GLU_cTab[GLU_cNr].p2a = &GLU_comm_tab[GLU_comm_pNr];

  return;
}


//=========================================================
  int GLT_cta_inc () {
//=========================================================

  ++GLT_cta_ind;

  if(GLT_cta_ind >= GLT_cta_SIZ-10) {
    // printf("GLT_cta_inc %d %d\n",GLT_cta_ind,GLT_cta_SIZ);
    return GLT_alloc_cta (GLT_cta_ind+100);
  }

  return 0;

}


//=========================================================
  int GLT_spp_sTyp (long sTyp) {
//=========================================================
// see also GLT_stor_rec (6,


  GLT_ppa[GLT_ppa_ind].siz  += 1;             // + col


  GLT_cta[GLT_cta_ind].typ  = Typ_Typ;
  GLT_cta[GLT_cta_ind].form = Typ_Int4;
  GLT_cta[GLT_cta_ind].siz  = 1;
  // (long)GLT_cta[GLT_cta_ind].data = sTyp;
  GLT_cta[GLT_cta_ind].data = (void*)sTyp;

  // next Contour
  return GLT_cta_inc ();

}


//================================================================
  GLT_spp_Tex (long texNr) {
//================================================================
 
  // printf("GLT_spp_Tex %d\n",texNr);

  GLT_ppa[GLT_ppa_ind].siz  += 1;             // + col
  GLT_cta[GLT_cta_ind].typ  = Typ_Texture;
  GLT_cta[GLT_cta_ind].form = Typ_Int4;
  GLT_cta[GLT_cta_ind].siz  = 1;
  // (long)GLT_cta[GLT_cta_ind].data = *((long*)sCol);
  // GLT_cta[GLT_cta_ind].data = (void*)(*((long*)sCol));
  GLT_cta[GLT_cta_ind].data = PTR_LONG (texNr);

  // next Contour
  return GLT_cta_inc ();

}


//=========================================================
  int GLT_spp_Col (ColRGB *sCol) {
//=========================================================
// see also GLT_stor_rec (5,


  // UT3D_stru_dump (Typ_Color, sCol, "GLT_spp_Col ");


  GLT_ppa[GLT_ppa_ind].siz  += 1;             // + col


  //----------------------------------------------------------------
  // TEST: add a color
  // sCol->cr = 33;
  // sCol->cg = 66;
  // sCol->cb = 99;

  GLT_cta[GLT_cta_ind].typ  = Typ_Color;
  GLT_cta[GLT_cta_ind].form = Typ_Int4;
  GLT_cta[GLT_cta_ind].siz  = 1;
  // (long)GLT_cta[GLT_cta_ind].data = *((long*)sCol);
  GLT_cta[GLT_cta_ind].data = (void*)(*((long*)sCol));

  // next Contour
  return GLT_cta_inc ();

}


//=========================================================
  int GLT_spp_pp (Vector *vcz) {
//=========================================================
// beginn next patch for active surf
// and give Z-Vector as first record
// MUSS ZWISCHEN GLT_spp_beg u GLT_spp_end erfolgen.


  // printf("-----------GLT_spp_pp ppa=%d cta=%d pta=%d\n",
          // GLT_ppa_ind,GLT_cta_ind,GLT_pta_ind);


  if(GLT_ppa_ind < GLT_ppa_SIZ) {     // next PatchNr
    ++GLT_ppa_ind;
    GLT_sur.siz  += 1;                // add patch to sur

  } else { 
    // printf("GLT_spp_pp ppa %d %d\n",GLT_ppa_ind,GLT_ppa_SIZ);
    return GLT_alloc_ppa (GLT_ppa_ind+100);
    // GLU_Err = -3;
    // goto L_Err;
  }



  // create patch
  GLT_ppa[GLT_ppa_ind].typ  = Typ_GL_PP;
  GLT_ppa[GLT_ppa_ind].form = Typ_ObjGX;
  GLT_ppa[GLT_ppa_ind].siz  = 1;  // first = vector
  GLT_ppa[GLT_ppa_ind].data = &GLT_cta[GLT_cta_ind];


  // add vector aus first record into contour-tab
  if(GLT_pta_ind >= GLT_pta_SIZ) {
    GLU_Err = GLT_alloc_pta (GLT_pta_ind);
    if(GLU_Err < 0) goto L_Err;
  }



  // if(vcz) {                                     // 2013-11-23
    GLT_cta[GLT_cta_ind].typ  = Typ_VC;
    GLT_cta[GLT_cta_ind].form = Typ_VC;
    GLT_cta[GLT_cta_ind].siz  = 1;
    GLT_cta[GLT_cta_ind].data = &GLT_pta[GLT_pta_ind];

    // den Vektor im Punktearray ablegen
    memcpy(&GLT_pta[GLT_pta_ind], vcz, sizeof(Vector));
    ++GLT_pta_ind;

    // GLT_ppa[GLT_ppa_ind].siz  += 1;     // // first = vector
  // }


  // next Contour (first is vec)
  // ++GLT_cta_ind;
  return GLT_cta_inc ();



  return 0;


  //----------------------------------------------------------------
  L_Err:
  printf("GLT_spp_pp ERROR %d ppa=%ld cta=%ld pta=%ld\n",GLU_Err,
          GLT_ppa_ind,GLT_cta_ind,GLT_pta_ind);
  // printf(" GLT_cta_SIZ=%d GLT_ppa_SIZ=%d\n",GLT_cta_SIZ,GLT_ppa_SIZ);
  return GLU_Err;


}


/*
//=========================================================
  GLU_CB GLT_comm_beg (GLenum which) {
//=========================================================
// start new active contour (GLT_cta_ind)
// 2=GL_LINE_LOOP   3=GL_LINE_STRIP
// 4=GL_TRIANGLES   5=GL_TRIANGLE_STRIP   6=GL_TRIANGLE_FAN


  printf(" GLT_comm_beg %d\n",which);

}
*/


//=========================================================
  GLU_CB GLT_spp_beg (GLenum pTyp) {
//=========================================================
// start new patch (GLT_cta_ind)
// pTyp = type-of-patch:
//   2=GL_LINE_LOOP   3=GL_LINE_STRIP
//   4=GL_TRIANGLES   5=GL_TRIANGLE_STRIP   6=GL_TRIANGLE_FAN


  // printf("<<<<<<<<<<< GLT_spp_beg %d\n",pTyp);


  GLU_pTyp = pTyp;

  // skip GL_LINE_LOOP
  if(pTyp < GL_TRIANGLES) {
    // printf("***** GLT_spp_beg %d\n",pTyp);
    return;
  }


  GLT_cta[GLT_cta_ind].typ  = Typ_PT;
  GLT_cta[GLT_cta_ind].form = Typ_PT;
  GLT_cta[GLT_cta_ind].data = &GLT_pta[GLT_pta_ind];
  GLT_cta[GLT_cta_ind].aux  = pTyp;
  // siz wird in GLT_spp_end gesetzt
  
  GLT_pta_start = GLT_pta_ind; // zur Berechnung Anzahl


  GLT_ppa[GLT_ppa_ind].siz  += 1;

  return;
}


//=========================================================
  GLU_CB GLT_spp_vert (GLvoid *vertex) {
//=========================================================
// add vertex to GLT_pta


  // printf(" GLT_spp_vert %d %p\n",GLT_pta_ind,GLT_pta);
  // printf(" GLT_spp_vert %d %f,%f,%f\n",GLT_pta_ind,
        // ((Point*)vertex)->x,((Point*)vertex)->y,((Point*)vertex)->z);

  // printf("    GLT_pta_ind=%d GLT_pta_start=%d\n",GLT_pta_ind,GLT_pta_start);


  // skip line-vertices ?
  if(GLU_pTyp < GL_TRIANGLES) return;    // 2013-11-23



  //----------------------------------------------------------------
  // check for degenerate Triangles
  if(GLT_cta[GLT_cta_ind].aux == GL_TRIANGLES)   {

    // auf den 3. Punkt warten ...
    if(GLT_pta_ind - GLT_pta_start < 2) goto L_store;

    // compare 0-1
    if(UT3D_comp2pt(&GLT_pta[GLT_pta_ind-2],
                    &GLT_pta[GLT_pta_ind-1],UT_TOL_pt) == 0) goto L_store;
    // compare 1-2
    if(UT3D_comp2pt(&GLT_pta[GLT_pta_ind-1],
                    (Point*)vertex,UT_TOL_pt) == 0) goto L_store;
    // compare 2-0
    if(UT3D_comp2pt(&GLT_pta[GLT_pta_ind-2],
                    (Point*)vertex,UT_TOL_pt) == 0) goto L_store;
    // skip last 2 points
    GLT_pta_ind -= 2;    
    return;


  //----------------------------------------------------------------
  } else if(GLT_cta[GLT_cta_ind].aux == GL_TRIANGLE_FAN)   {
    // auf den 2. Punkt warten ...
    if(GLT_pta_ind - GLT_pta_start < 1) goto L_store;
    // remove double points; bei Fans, nicht bei strips
    // der erste Punkt eines patch darf nicht eliminiert werden !!
    if(UT3D_comp2pt(&GLT_pta[GLT_pta_ind-1],(Point*)vertex,UT_TOL_pt) == 1) {
      // skip this point ..
      return;
    }
  }


/*
  if((GLT_pta_ind > GLT_pta_start)  &&
     (GLT_cta[GLT_cta_ind].aux != GL_TRIANGLE_STRIP))   {
    // wenn neuer Punkt innerhalb PunktToleranz: skip this point ..
    if(UT3D_comp2pt(&GLT_pta[GLT_pta_ind-1],(Point*)vertex,UT_TOL_pt) == 1) {
      printf(" skip pt\n");
      return;
    }
  }
*/

  L_store:


  if(GLT_pta_ind >= GLT_pta_SIZ) {
    printf("GLT_spp_vert %ld\n",GLT_pta_SIZ);
    if(GLU_Err < 0) return;
    GLU_Err = GLT_alloc_pta (GLT_pta_ind);
  }


  if(GLU_Err < 0) return;

  memcpy(&GLT_pta[GLT_pta_ind], vertex, sizeof(Point));
  ++GLT_pta_ind;
    // GR_Disp_pt (vertex, SYM_STAR_S, 0);

  return;
}



//=========================================================
  GLU_CB GLT_spp_end () {
//=========================================================
// add patch to active surface

  int   i1;


  // printf(" GLT_spp_end ind=%d  start=%d\n",GLT_pta_ind,GLT_pta_start);
  // printf("      cta_ind=%d\n",GLT_cta_ind);


  GLT_cta[GLT_cta_ind].siz  = GLT_pta_ind - GLT_pta_start;
  
  if(GLT_cta_ind >= GLT_cta_SIZ) {
    GLU_Err = -2;
    printf("GLT_spp_end E001\n");
    return;
  }
  ++GLT_cta_ind;                   // point now to next active contour


  // NUR TESTAUG:
  // for(i1=0;i1<GLT_cta_ind;++i1)
   // {printf(" cta[%d].siz=%d\n",i1,GLT_cta[i1].siz);}


  return;
}


//=========================================================
  GLU_CB GLT_CB_Begin (GLenum which) {
//=========================================================
// 4=GL_TRIANGLES   5=GL_TRIANGLE_STRIP   6=GL_TRIANGLE_FAN


  // printf(" GLT_CB_Begin %d cNr=%d pNr=%d\n",which,GLU_cNr,GLU_comm_pNr);
  // glBegin(which);


  GLU_cTab[GLU_cNr].p2a = &GLU_comm_tab[GLU_comm_pNr];


  return;
}


//=========================================================
  GLU_CB GLT_CB_End () {
//=========================================================

static int ptOff;


  // printf(" GLT_CB_End cNr=%d pNr=%d\n",GLU_cNr,GLU_comm_pNr);
  // glEnd();


  if(GLU_cNr == 0) ptOff = 0;


  // letzter Punkt == erster Punkt;
  if((GLU_comm_pNr < GLU_comm_Max-1)&&(GLU_comm_pNr > 0)) {
    GLU_comm_tab[GLU_comm_pNr] = GLU_comm_tab[ptOff];
    ++GLU_comm_pNr;
  }


  GLU_cTab[GLU_cNr].iNr = GLU_comm_pNr - ptOff;
  ptOff = GLU_comm_pNr;

  if(GLU_cNr < GLU_cMax-1) ++GLU_cNr;

  return;
}


//================================================================
  int GLT_start__ (ObjGX **gSur) {
//================================================================
// prepare for new surf-tesselation; pointer ruecksetzen.
// return pointer -> surf-obj

  GLU_Err     = 0;

  GLT_pta_ind = 0;
  GLT_cta_ind = 0;

  GLT_ppa_ind = -1;


  // create surf
  GLT_sur.typ  = Typ_GL_Sur;
  GLT_sur.form = Typ_ObjGX;
  GLT_sur.siz  = 0;
  GLT_sur.data = GLT_ppa;

  *gSur = &GLT_sur;

  return 0;

}


//================================================================
  int GLT_stor_rec (int mode, Point *pa1, Point *pa2, int ptNr) {
//================================================================
// einen Datenrecord in GLT_ppa(ogx-Patches) und GLT_pta(Points) speichern
// und mit close nach TSU_vM kopieren.
// GLT_cta wird nicht benutzt (PunktDatenAdresse direkt into Patch).
// Subfunctions:
// mode = 0  init, ptNr ist Anzahl Patches
// mode = 1  close record
// mode = 2  add strip (used by GL_DrawStrip2)
// mode = 3  add planaren fan (used by GL_DrawFan)
// mode = 4  add nichtPlanaren fan (used by GL_DrawFan)
// mode = 5  add Color actCol
// mode = 8  add Texture
// mode = 6  add SurfaceTyp; typ=ptNr
// mode = 7  add Mesh (PointTab + faceTab)

// see also obj_readTess__ wrl1_r_dec_sph wrl1_r_dec_cyl wrl1_r_dec_ifs


  int        ii, i1, i2, i3, irc;
  long       l1;
  Fac3       *fTab;


  // printf("-------------------------------\n");
  // printf("GLT_stor_rec mode=%d ptNr=%d\n",mode,ptNr);



  //===================== CLOSE ====================================
  // close &copy record --> TSU_vM
    // ein ReInit braucht close
    // if((mode == 0)&&(oldSiz >= 0)) goto L_close;

    if(mode != 1) goto L_init;

    // if(oldSiz < 0) return 0;  // kein close erforderl.
      // printf(" GLT_ppa_SIZ=%ld GLT_ppa_ind=%ld\n",GLT_ppa_SIZ,GLT_ppa_ind);
      // printf(" GLT_pta_SIZ=%ld GLT_pta_ind=%ld\n",GLT_pta_SIZ,GLT_pta_ind);



    L_close:
      // printf("GLT_stor_rec close %ld\n",GLT_ppa_ind);
    GLT_sur.typ  = Typ_GL_Sur;
    GLT_sur.form = Typ_ObjGX;
    GLT_sur.siz  = GLT_ppa_ind + 1;
    GLT_sur.data = GLT_ppa;

    // copy record --> TSU_vM
    TSU_store (&GLT_sur);

    if(mode == 1) return 0;  // weiter wenn ein Init requested ...



  //===================== INIT =====================================
  // INIT new tess-record
  L_init:
  if(mode != 0) goto L_strip;

    // printf("GLT_stor_rec init %d\n",ptNr);

    GLU_Err     = 0;

    GLT_pta_ind = 0;
    GLT_cta_ind = 0;
    GLT_ppa_ind = -1;

    return 0;


  //==================== STRIPE ====================================
  L_strip:
  if(mode != 2) goto L_fan;
  // save a stripe (<ptNr> points in pa1 and in pa2) as a new patch
  // printf("GLT_stor_rec strip %d\n",ptNr);
  // GR_Disp_pTab (ptNr, pa1, SYM_TRI_S, 3);
  // GR_Disp_pTab (ptNr, pa2, SYM_TRI_S, 3);


  // start new Patch
  if(GLT_ppa_ind < GLT_ppa_SIZ) {     // next PatchNr
    ++GLT_ppa_ind;
  } else { GLU_Err = -3; goto L_Err; }


  // create patch
  GLT_ppa[GLT_ppa_ind].typ  = Typ_GL_PP;
  GLT_ppa[GLT_ppa_ind].form = Typ_PT;
  GLT_ppa[GLT_ppa_ind].siz  = ptNr * 2;
  GLT_ppa[GLT_ppa_ind].aux  = GL_TRIANGLE_STRIP;
  GLT_ppa[GLT_ppa_ind].data = &GLT_pta[GLT_pta_ind];


  // check for space for points
  i1 = GLT_pta_ind + ptNr*2;
  if(i1 >= GLT_pta_SIZ) {
    GLU_Err = GLT_alloc_pta (GLT_pta_ind);
    if(GLU_Err < 0) goto L_Err;
  }


  // add Point-data-Record
  for(i1=0; i1<ptNr; ++i1) {       // write pointTable
    memcpy(&GLT_pta[GLT_pta_ind], &pa1[i1], sizeof(Point)); ++GLT_pta_ind;
    memcpy(&GLT_pta[GLT_pta_ind], &pa2[i1], sizeof(Point)); ++GLT_pta_ind;
  }

  return 0;




  //================================================================
  L_fan:      // modes 3 & 4
  if(mode > 4) goto L_col;
  // add data
  // printf("GLT_stor_rec fan %d\n",ptNr);
  // GR_Disp_pTab (1, pa1, SYM_TRI_S, 3);
  // GR_Disp_pTab (ptNr, pa2, SYM_TRI_S, 3);


  // start new Patch
  if(GLT_ppa_ind < GLT_ppa_SIZ) {     // next PatchNr
    ++GLT_ppa_ind;
  } else { GLU_Err = -3; goto L_Err; }


  // create patch
  GLT_ppa[GLT_ppa_ind].typ  = Typ_GL_PP;
  GLT_ppa[GLT_ppa_ind].form = Typ_PT;
  GLT_ppa[GLT_ppa_ind].siz  = ptNr + 1;
  if(mode == 3)
    GLT_ppa[GLT_ppa_ind].aux  = GL_TRIANGLE_FAN;
  else 
    GLT_ppa[GLT_ppa_ind].aux  = GL_FAC_PLANAR;
  GLT_ppa[GLT_ppa_ind].data = &GLT_pta[GLT_pta_ind];


  // check for space for points
  i1 = GLT_pta_ind + ptNr + 1;
  if(i1 >= GLT_pta_SIZ) {
    GLU_Err = GLT_alloc_pta (GLT_pta_ind);
    if(GLU_Err < 0) goto L_Err;
  }


  // add Point-data-Record
  memcpy(&GLT_pta[GLT_pta_ind], &pa1[0], sizeof(Point)); ++GLT_pta_ind;
  for(i1=0; i1<ptNr; ++i1) {       // write pointTable
    memcpy(&GLT_pta[GLT_pta_ind], &pa2[i1], sizeof(Point)); ++GLT_pta_ind;
  }

  return 0;





  //================================================================
  L_col:      // mode 5
  // add color; see also GLT_spp_Col
  if(mode > 5) goto L_tex;
    // printf("GLT_stor_rec 5 (Col)\n");

  // start new Patch
  if(GLT_ppa_ind < GLT_ppa_SIZ) {     // next PatchNr
    ++GLT_ppa_ind;
  } else { GLU_Err = -3; goto L_Err; }

  // create color = patch
  GLT_ppa[GLT_ppa_ind].typ  = Typ_Color;
  GLT_ppa[GLT_ppa_ind].form = Typ_Int4;
  GLT_ppa[GLT_ppa_ind].siz  = 1;
  // (long)GLT_ppa[GLT_ppa_ind].data = *((long*)&GL_actCol);
  GLT_ppa[GLT_ppa_ind].data = (void*)(*((long*)&GL_actCol));

  return 0;


  //================================================================
  L_tex:      // mode 8
  // add texture; see also GLT_spp_Tex
  if(mode != 8) goto L_typ;
    // printf("GLT_stor_rec 8 (Tex %d)\n",ptNr);

  // start new Patch
  if(GLT_ppa_ind < GLT_ppa_SIZ) {     // next PatchNr
    ++GLT_ppa_ind;
  } else { GLU_Err = -3; goto L_Err; }

  // create color = patch
  GLT_ppa[GLT_ppa_ind].typ  = Typ_Texture;
  GLT_ppa[GLT_ppa_ind].form = Typ_Int4;
  GLT_ppa[GLT_ppa_ind].siz  = 1;
  // (long)GLT_ppa[GLT_ppa_ind].data = *((long*)&GL_actCol);
  GLT_ppa[GLT_ppa_ind].data = PTR_INT (ptNr);

  return 0;




  //================================================================
  L_typ:      // modes 6
  if(mode > 6) goto L_Msh;
  // save ptNr in a Typ_Typ/Typ_Int4-record
  // dzt nur bei Intersect benutzt (speichern; TSU_mode == 1);
  // nicht bei draw (TSU_mode == 0)
  // if(mode > 6) goto L_typ;
    // printf("GLT_stor_rec 5\n");

  // start new Patch
  if(GLT_ppa_ind < GLT_ppa_SIZ) {     // next PatchNr
    ++GLT_ppa_ind;
  } else { GLU_Err = -3; goto L_Err; }

  // create SurfacTyp = patch
  GLT_ppa[GLT_ppa_ind].typ  = Typ_Typ;
  GLT_ppa[GLT_ppa_ind].form = Typ_Int4;
  GLT_ppa[GLT_ppa_ind].siz  = 1;
  l1 = ptNr;
  // (long)GLT_ppa[GLT_ppa_ind].data = l1;
  GLT_ppa[GLT_ppa_ind].data = (void*)l1;



  return 0;


  //================================================================
  L_Msh:      // modes 7  -   Mesh
  // if(mode > 6) goto L_Msh;
  // pa1 = pTab; pa2 = fTab; ptNr = faceNr.


  // start new Patch
  if(GLT_ppa_ind < GLT_ppa_SIZ) {     // next PatchNr
    ++GLT_ppa_ind;
  } else { GLU_Err = -3; goto L_Err; }


  // ptNr = 10000; // TEST ONLY


  // create patch
  GLT_ppa[GLT_ppa_ind].typ  = Typ_GL_PP;
  GLT_ppa[GLT_ppa_ind].form = Typ_PT;
  GLT_ppa[GLT_ppa_ind].siz  = ptNr * 3;
  GLT_ppa[GLT_ppa_ind].aux  = GL_TRIANGLES;
  GLT_ppa[GLT_ppa_ind].data = &GLT_pta[GLT_pta_ind];


  // check for space for points
  i1 = GLT_pta_ind + ptNr * 3;
  if(i1 >= GLT_pta_SIZ) {
    GLU_Err = GLT_alloc_pta (i1);
    if(GLU_Err < 0) goto L_Err;
  }


  // add Point-data-Record
  fTab = (void*)pa2;
  for(ii=0; ii<ptNr; ++ii) {       // write pointTable
    i1 =fTab[ii].i1;
    i2 =fTab[ii].i2;
    i3 =fTab[ii].i3;
    memcpy(&GLT_pta[GLT_pta_ind], &pa1[i1], sizeof(Point)); ++GLT_pta_ind;
    memcpy(&GLT_pta[GLT_pta_ind], &pa1[i2], sizeof(Point)); ++GLT_pta_ind;
    memcpy(&GLT_pta[GLT_pta_ind], &pa1[i3], sizeof(Point)); ++GLT_pta_ind;
  }

  return 0;



  //================================================================
  L_err_eom:
    TX_Error("GLT_stor_rec EOM");
    // TSU_Init (0, &sizPos);
    return -1;

  L_Err:
    TX_Error("GLT_stor_rec ERR");
    printf("GLT_stor_rec ERROR %d ppa=%ld cta=%ld pta=%ld\n",GLU_Err,
          GLT_ppa_ind,GLT_cta_ind,GLT_pta_ind);
    return GLU_Err;

}


//========================== EOF =============================
