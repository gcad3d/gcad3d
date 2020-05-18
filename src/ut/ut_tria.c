//    ut_tria.c                                   RF 205-08-18
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
#ifdef globTag
void UTRI(){}
#endif
/*!
\file  ../ut/ut_tria.c
\brief triangles (using struct Point - UTRI_) 
\code
=====================================================
List_functions_start:

UT3D_tria_pta              create triangle from 3 points
UTRI_tria_3pt              create triangle from 3 points
UT3D_tria_fac              create triangle from Fac3                   INLINE
UTRI_ntria_patch           get triangles from opengl-patch (pTab, pNr, pTyp)

UTRI_triaNr_patch          get nr of triangles for [indexed-]opengl-patch
UT2D_orient_3pt            orientation of triangle (CW or CCW)
UT3D_ck_triaOrientV        get orientation from indices   INLINE
UT3D_ck_triaOrientI
UT2D_2pt_tngLnCiY          Beruehrung Kreis (pc auf horiz. Gerade) - LineSeg
UT3D_vc_perpTria           vector = perpendic. to Triangle (crossprod)
UTRI_radi_3pt              inkreisradius
UTRI_rado_3pt              radius circumCircle        umkreisradius
UTRI_ptCC_3pt              center of circumCircle     UmkreisMittelpunkt
UTRI_ar_3pt                area of triangle         see UT2D_ar_3pt

UT3D_indTria2ccw           3EckIndices CCW orientieren
UT3D_ind3Tria_2ind         give index of 3.point of triangle (0/1/2)  INLINE

UT3D_lenq_PtTria           minimum (quadr) distance Point-Triangle
UT3D_lenq_PlnTria          minimum (quadr) distance Plane-Triangle
UT3D_ck_pt_in_tria         check if point is inside Triangle
UT3D_ck_pt_prj_tria        check if projection of point is inside Triangle
UT2D_pt_gcp_3pt            gravity-centerPoint of triangle
UT3D_pt_gcp_3pt            gravity-centerPoint of triangle
UT3D_pt_gcp_tria           gravity-centerPoint of triangle
UT3D_tria_inv              Umlaufsinn umdrehen

UT3D_pt_projPtTria         project point onto triangle
UTRI_pt_prjZpt3pt          project point to face along Z-vec
UT3D_pt_intTriaLnu         Point from intersect Triangle - LineUnlimited
UTRI_ln_int_LnFac          intersect Line - Face
UT3D_ln_intTriaPln         Line = intersect Triangle (Triangle) - Plane
UT3D_ln_int_tria_pln       Line = intersect Triangle (3 points) - Plane
UT3D_ln_intTriaTria        Line = intersect Triangle - Triangle
UT3D_2ln_tngTriPlSph       Kugel in Ebene an 3Eck abrollen

UT3D_pl1_tria              make Plane from Triangle

List_functions_end:
=====================================================
- see also:
TSU_ntria_bMsh__             triangles from binary-mesh
statt UT3D_vc_perptria     use UT3D_vc_perp3pt      Normalvektor auf 3Eck
GR_Disp_triv GR_Disp_tria
TSU_ntria_bMsh__ TSU_ntria_bMsh_p UTRI_tria_3pt
../ut/ut_face.c   UFA   Funktionen fuer Faces (Fac3)
../ut/ut_msh.c   MSH   Funktionen fuer Meshes

- test functions:
UT3D_tria_tst_load         load testTriangles

\endcode *//*----------------------------------------


*/

#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/gl.h>            // GL_TRIANGLE_STRIP ..


#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_itmsh.h"            // MSHIG_EDGLN_.. typedef_MemTab.. Fac3
#include "../xa/xa_mem.h"              // memspc51
#include "../ut/gr_types.h"            // SYM_STAR_S


//================================================================
  int UTRI_tria_3pt (Triangle *triTab, int *triNr, int triSiz,
                        Point *p1, Point *p2, Point *p3) {
//================================================================
// add 1 triangle       als 3 Pointer --> triTab[*triNr];
// der Pointer auf den 1. Punkt zB ist        triTab[i1].pa[0]
// see also tess_ntri_tfac_add

  int    it;


  it = *triNr;

  // printf("UTRI_tria_3pt %d\n",it);
  // GR_Disp_pt (p1, SYM_STAR_S, 2);
  // GR_Disp_pt (p2, SYM_STAR_S, 2);
  // GR_Disp_pt (p3, SYM_STAR_S, 2);


  if(it >= triSiz) {
    TX_Error("UTRI_tria_3pt EOM");
    return -1;
  }


  triTab[it].pa[0] = p1;
  triTab[it].pa[1] = p2;
  triTab[it].pa[2] = p3;

  ++it;
  *triNr = it;


  return 0;

}


//=======================================================================
  int UTRI_ntria_patch (Triangle *tTab, int *tSiz,
                        Point *pTab, int pNr, int pTyp) {
//=======================================================================
// get triangles from opengl-patch (pTab, pNr, pTyp)
// Input:
//   tSiz   size of tTab
//   pTyp   patchTyp GL_TRIANGLE_STRIP|GL_TRIANGLE_FAN|GL_QUAD_STRIP
// Output:
//   tSiz   nr of Triangles created
// 
// find min size of tTab: UTRI_triaNr_patch()
// see TSU_nfac_ipatch__ TSU_nfac_ipatch__ tess_ntri_tfac__
//
// TODO: use UTRI_ntria_patch in tess_ntri_tfac__

  int      i1, ii, tNr;


  ii = 0;

  switch (pTyp) {

    //================================================================
    case GL_TRIANGLES:  // 4

      tNr = pNr / 3;           // total nr of triangles
      if(tNr > *tSiz) goto L_errEOM;

      L_T0:
        tTab->pa[0] = pTab;  ++pTab;
        tTab->pa[1] = pTab;  ++pTab;
        tTab->pa[2] = pTab;  ++pTab;
        ++tTab;
        ++ii;
        if(ii < tNr) goto L_T0;

      break;


    //================================================================
    case GL_FAC_PLANAR:      // 16:   Achtung: additinal for indexed 
    case GL_TRIANGLE_FAN:    // 6
      //    1------2           ptAnz = 4
      //    |    /  |
      //    |   /   |
      //    |  /    |
      //    | /     |
      //    0.------3
      //      \     /
      //        \  /
      //         4
      //
      // sollte so zerlegt werden:
      // 0 1 2
      // 0 2 3
      // 0 3 4

      tNr = pNr - 2;
      if(tNr > *tSiz) goto L_errEOM;
      i1 = 1;

      L_T6:
        tTab->pa[0] = &pTab[0];
        tTab->pa[1] = &pTab[i1]; ++i1;
        tTab->pa[2] = &pTab[i1];
        ++tTab;
        ++ii;
        if(ii < tNr) goto L_T6;

      break;


    //================================================================
    case GL_TRIANGLE_STRIP:  // 5
    case GL_QUAD_STRIP:      // 8
      //
      //    0--2--4--6
      //    | /| /| /|
      //    |/ |/ |/ |
      //    1--3--5--7
      //
      // sollte so zerlegt werden:
      // 0 1 2
      // 2 1 3
      // 2 3 4
      // 4 3 5 ..

      tNr = pNr - 2;
      if(tNr > *tSiz) goto L_errEOM;
      i1 = 0;

      L_T8:
        tTab->pa[0] = &pTab[i1]; ++i1;
        tTab->pa[1] = &pTab[i1]; ++i1;
        tTab->pa[2] = &pTab[i1];
        ++tTab;
        ++ii;
        if(ii >= tNr) break;

        tTab->pa[0] = &pTab[i1];
        tTab->pa[1] = &pTab[i1-1];
        tTab->pa[2] = &pTab[i1+1];
        ++tTab;
        ++ii;
        if(ii < tNr) goto L_T8;

      break;


    //================================================================
    default:
      TX_Error("UTRI_ntria_patch E001 %d", pTyp);
      return -1;

  }

  *tSiz = tNr;

  return 0;


  L_errEOM:
    TX_Error("UTRI_ntria_patch EOM");
    return -1;



  return 0;

}


//================================================================
  int UTRI_triaNr_patch (int ptNr, int patchTyp) {
//================================================================
/// \code
/// UTRI_triaNr_patch     get nr of triangles for [indexed-]opengl-patch
/// Input:
///   ptNr     nr of points
///   patchTyp GL_TRIANGLE_STRIP|GL_TRIANGLE_FAN|GL_QUAD_STRIP
///
/// see TSU_facNr_ipatch
/// see tess_triaNr_patch
/// \endcode

  int   tNr;


    switch (patchTyp) {

      case GL_TRIANGLES:
        tNr = ptNr / 3;
        break;

      case GL_TRIANGLE_STRIP:
      case GL_FAC_PLANAR:
      case GL_TRIANGLE_FAN:
      case GL_QUAD_STRIP:
        tNr = ptNr - 2;
        break;

      default:
        TX_Error("UTRI_triaNr_patch NYI typ=%d",patchTyp);
        return -1;
    }


    // printf("ex UTRI_triaNr_patch %d\n",tNr);


  return tNr;

}


/*=======================================================================*/
  int UT2D_orient_3pt (Point2 *p1, Point2 *p2, Point2 *p3) {
/*==================
UT2D_orient_3pt        orientation of triangle (CW or CCW)

UT2D_orient_3pt        Author: Thomas Backmeister       27.8.2003

Compute orientation of a triangle.

IN:
  Point2 *p1      ... 1. point of triangle
  Point2 *p2      ... 2. point of triangle
  Point2 *p3      ... 3. point of triangle
OUT:
Returncodes:
  1 = CW  = clockwise
  0 = CCW = counter clockwise
 -1 = no orientation (collinear points - 3 points in line)
      maximale Genauigkeit; kein UT_TOL_X - test !
*/

  double scp;
  Vector2 v1, vn;

  // printf("UT2D_orient_3pt: orientation of triangle\n");

  // vector p1->p2
  v1.dx = p2->x - p1->x;
  v1.dy = p2->y - p1->y;

  // normal vector to p1->p3
  vn.dx = p1->y - p3->y;
  vn.dy = p3->x - p1->x;

  // scalar product <v1,vn>
  // scp = UT2D_acos_2vc (&v1, &vn);
  scp = v1.dx * vn.dx + v1.dy * vn.dy;


  if (scp > 0.0) return CW;     // -1
  if (scp < 0.0) return CCW;    //  1

  return 0;
}


//================================================================
  int UT3D_pt_projPtTria (Point *pto,                               // out
                          Point *pti, Triangle *trii) {             // in
//================================================================
// project point onto triangle / Normalabstand Triangle-Point
//  Retcod, ob der Punkt auf dem 3Eck liegt oder nicht.
//  Wenn ja: NormalAbstand und ProjektionsPunkt am Dreieck.
// RetCod:  0  OK;
//         -1  3Eck degeneriert ...

// normal projection of P onto triangle plane (if necessary)
// P = OP - (AP . (AB x AC)) / ((AB x AC) . (AB x AC));

// see UT3D_ln_intTriaPln UT3D_ln_intTriaTria UT3D_pt_intptvcpl_ UT3D_pt_projptpl



  double  t1, t2;
  Vector  vc_abXac, vab, vac, vap, vpi;


  // printf("UT3D_pt_projPtTria \n");
    // GR_Disp_pt (pti, SYM_STAR_S, 2);
    // GR_Disp_pt (trii->pa[0], SYM_STAR_S, 3);


  // Vektoren laden
  UT3D_vc_2pt (&vab, trii->pa[0], trii->pa[1]);     // AB=0,1
  UT3D_vc_2pt (&vac, trii->pa[0], trii->pa[2]);     // AC=0,2
  UT3D_vc_2pt (&vap, trii->pa[0], pti);             // AP=0,pti


  // Normalvekttor auf das 3Eck
  UT3D_vc_perp2vc (&vc_abXac, &vab, &vac);
    // GR_tDyn_vc (&vc_abXac, trii->pa[0], 9, 0);


  t1 = UT3D_skp_2vc (&vc_abXac, &vc_abXac);
  if(t1 == 0.) return -1;


  t2 = UT3D_skp_2vc (&vap, &vc_abXac) / t1;


  pto->x = pti->x - (vc_abXac.dx * t2);
  pto->y = pti->y - (vc_abXac.dy * t2);
  pto->z = pti->z - (vc_abXac.dz * t2);


    // GR_Disp_pt (pto, SYM_STAR_S, 3);

  return 0;

}


//================================================================
  int UTRI_pt_prjZpt3pt (Point *pto, Point *pti,
                         Point *p1, Point *p2, Point *p3) {
//================================================================
// project point to face along Z-vec
// see also MSH_pt_prjptmsh1

  int     irc;
  double  d1;
  Vector  *vp;
  Plane   pl1;


  vp = (Vector*)&UT3D_VECTOR_Z;  // projectionVector


  // get Z-coord of point on 2D-Triangle.
  // see UT3D_pl1_tria
  UT3D_vc_perp3pt (&pl1.vz, p1, p2, p3); // normalVector of triangle
  UT3D_vc_setLength (&pl1.vz, &pl1.vz, 1.);
    // DEB_dump_obj__ (Typ_VC, &pl1.vz, "pl1.vz:");

  // create pl1.p from po & vz
  UT3D_pl_ptpl (&pl1, p1);
    // DEB_dump_obj__ (Typ_PLN, &pl1, "pl1:");

  // int. pti-vp with plane
  irc = UT3D_ptDi_intptvcpln (pto, &d1, &pl1, pti, vp);

  if(irc == 0) {   // plane parallel; Z=zVal of gravityCenterpoint)
    *pto = *pti;
    pto->z = (p1->z + p2->z + p3->z) / 3.;
  }


  return 0;

}


//================================================================
  int UT3D_lenq_PtTria (double *lenq, Point *p,
                        Point *p1, Point *p2, Point *p3) {
//================================================================
// UT3D_lenq_PtTria        minimum (quadr) distance Point-Triangle

  int      irc=0;
  double   A, B, C, D, E, det, t1, t2, x, y, z, d1, d2;
  Vector   p1p2, p1p3, pp1;





  // printf("UT3D_lenq_PtTria\n");
  // DEB_dump_obj__ (Typ_PT, p, "  pd:");
  // DEB_dump_obj__ (Typ_PT, p1, "  pa:");
  // DEB_dump_obj__ (Typ_PT, p2, "  pb:");
  // DEB_dump_obj__ (Typ_PT, p3, "  pc:");


  UT3D_vc_2pt (&p1p2, p1, p2);
  UT3D_vc_2pt (&p1p3, p1, p3);
  UT3D_vc_2pt (&pp1, p, p1);


  B = UT3D_skp_2vc (&p1p3, &p1p2);
  E = UT3D_skp_2vc (&p1p2, &p1p2);
  C = UT3D_skp_2vc (&p1p3, &p1p3);

  det = B*B - E*C;
    // printf(" det=%f B=%f E=%f C=%f\n",det,B,E,C);


  if (det == 0.) {         // 3Eck degeneriert ..
    irc = -1;
    goto L_exit;
  }


  A = UT3D_skp_2vc (&p1p3, &pp1);
  D = UT3D_skp_2vc (&p1p2, &pp1);

  t1 = (D*C - A*B) / det;
  t2 = (A*E - D*B) / det;
    // printf(" t1=%f t2=%f A=%f D=%f\n",t1,t2,A,D);


  if (t1 < 0.) {
    UT3D_lenq_PtLn (lenq, p, p3, p1);
    goto L_exit;
  }

  if (t2 < 0.) {
    UT3D_lenq_PtLn (lenq, p, p1, p2);
    goto L_exit;
  }

  if (t1 + t2 > 1.) {
    UT3D_lenq_PtLn (lenq, p, p2, p3);
    goto L_exit;
  }


  x = pp1.dx + t1 * p1p2.dx + t2 * p1p3.dx;
  y = pp1.dy + t1 * p1p2.dy + t2 * p1p3.dy;
  z = pp1.dz + t1 * p1p2.dz + t2 * p1p3.dz;
    // printf(" x=%f y=%f z=%f\n",x,y,z);

  *lenq = x * x + y * y + z * z;


  L_exit:

  // printf("ex UT3D_lenq_PtTria %d %f %f\n",irc,*lenq,sqrt(*lenq));

  return irc;

}

//================================================================
  int UT3D_lenq_PlnTria (double *lenq, Point *plnO, Vector *plnZ,
                         Point *p1, Point *p2, Point *p3) {
//================================================================
// UT3D_lenq_PlnTria          minimum (quadr) distance Plane-Triangle

// see also

  int      irc, imin, imax, smin, smax;
  double   p, dTab[3];
  Vector   vco1;


  // printf("UT3D_lenq_PlnTria\n");
  // DEB_dump_obj__ (Typ_PT, plnO, "  plnO:");
  // DEB_dump_obj__ (Typ_VC, plnZ, "  plnZ:");
  // DEB_dump_obj__ (Typ_PT, p1,   "  pa:");
  // DEB_dump_obj__ (Typ_PT, p2,   "  pb:");
  // DEB_dump_obj__ (Typ_PT, p3,   "  p3:");


  // die slenq von allen 3 Punkten auf die Plane holen;
  UT3D_vc_2pt (&vco1, plnO, p1);
  dTab[0] = UT3D_slenq_projvcvc (plnZ, &vco1);

  UT3D_vc_2pt (&vco1, plnO, p2);
  dTab[1] = UT3D_slenq_projvcvc (plnZ, &vco1);

  UT3D_vc_2pt (&vco1, plnO, p3);
  dTab[2] = UT3D_slenq_projvcvc (plnZ, &vco1);
    // printf(" dTab = %f %f %f\n",dTab[0],dTab[1],dTab[2]);



  // min/max-Abstand der 3 punkte bestimmen
  imin = UTP_min (3, dTab);   // ind minVal
  imax = UTP_max (3, dTab);   // ind maxVal
    // printf(" i/dmin,i/dmax=%d %f %d %f\n",imin,dTab[imin],imax,dTab[imax]);



  // wenn min/max-Vorzeichen verschieden: minDist ist 0.;
  smin = DSIGN(dTab[imin]);
  smax = DSIGN(dTab[imax]);
  if(smin != smax) {
    *lenq = 0.;

  } else {
    // beide Werte fabs; der kleinere Wert ist der minDist.
    *lenq = DMIN(fabs(dTab[imin]), fabs(dTab[imax]));
  }

  // printf("ex UT3D_lenq_PlnTria %f %f\n",*lenq,sqrt(*lenq));


  return 0;

}


/*
//=====================================================================
  int UT3D_ind3Tria_2ind (int *i1, int *i2) {
//=====================================================================
// UT3D_ind3Tria_2ind               give index of 3.point of triangle (0/1/2)

    // printf("ex UT3D_ind3Tria_2ind %d\n",IABS(i1 + i2 - 3));
  return IABS(i1 + i2 - 3);

}
*/


//=====================================================================
  int UT3D_indTria2ccw (int *i1, int *i2) {
//=====================================================================
// 3EckIndices CCW orientieren
// out:  0-1 1-2 2-0

  int ii;

  ii = *i1 + *i2;

  if(ii == 1) {*i1 = 0; *i2 = 1;}
  else if(ii == 2) {*i1 = 2; *i2 = 0;}
  else {*i1 = 1; *i2 = 2;}

    // printf("ex UT3D_indTria2ccw %d %d\n",*i1,*i2);

  return 0;

}


//================================================================
  int UT3D_tria_inv  (Triangle *tr) {
//================================================================
// Umlaufsinn umdrehen; Index 0 und 1 vertauschen.

  // printf("UT3D_tria_inv \n");

  MEM_swap_2vp ((void**)&tr->pa[0], (void**)&tr->pa[1]);

  return 0;

}


//================================================================
  int UT3D_tria_pta (Triangle *tr, Point *pa) {
//================================================================
// create triangle from 3 points

  tr->pa[0] = &pa[0];
  tr->pa[1] = &pa[1];
  tr->pa[2] = &pa[2];

  return 0;

}


//==================================================================
  int UT3D_pt_gcp_tria (Point *ps, Triangle *tri) {
//==================================================================
// UT3D_pt_gcp_tria           gravity-centerPoint of triangle


  return UT3D_pt_gcp_3pt (ps, tri->pa[0],tri->pa[1],tri->pa[2]);

}


//==================================================================
  int UT2D_pt_gcp_3pt (Point2 *ps, Point2 *p1, Point2 *p2, Point2 *p3) {
//==================================================================
// UT2D_pt_gcp_3pt           gravity-centerPoint of triangle

  ps->x = (p1->x + p2->x + p3->x) / 3.;
  ps->y = (p1->y + p2->y + p3->y) / 3.;

  // *ps = *p1;

  return 0;

}


//=====================================================================
  int UTRI_radi_3pt (double *rdi, Point2 *p1, Point2 *p2, Point2 *p3) {
//=====================================================================
// inkreisradius
// ri = A / (a+b+c) * 2;

  double    da, s;


  da = fabs(UT2D_ar_3pt(p1, p2, p3));

  s = (UT2D_len_2pt(p1, p2) + UT2D_len_2pt(p2, p3) + UT2D_len_2pt(p3, p1)) / 2.;

  *rdi = da  / s;


  return 0;

}


//=====================================================================
  int UTRI_rado_3pt (double *rdo, Point2 *p1, Point2 *p2, Point2 *p3) {
//=====================================================================
// UT2D_rado_3pt                      radius circumCircle        umkreisradius

// ru = (a*b*c)  / (A * 4)

  double    da, s;


  da = fabs(UT2D_ar_3pt(p1, p2, p3));

  s = (UT2D_len_2pt(p1, p2) * UT2D_len_2pt(p2, p3) * UT2D_len_2pt(p3, p1));

  *rdo = s / (da * 4.);


  return 0;

}


//=====================================================================
  int UTRI_ptCC_3pt (Point2 *pum, Point2 *p1, Point2 *p2, Point2 *p3) {
//=====================================================================
// UT2D_ptCC_3pt   center of circumCircle
// Umkreismittelpunkt von 3 Punkten bestimmen            Karl Sauer 2008-05-09
//
//  INPUT:
//  ------
//    Point2 p1[2]   ... X/Y Koordinaten Punkt 1
//    Point2 p2[2]   ... X/Y Koordinaten Punkt 2
//    Point2 p3[2]   ... X/Y Koordinaten Punkt 3

//
//  OUTPUT:
//  -------
//    Point2 pum[2]  ... Koordinaten Umkreismittelpunkt
//    long   rc      ... Funktionswert: 1 = OK
//                                      0 = Dreieck ist degeneriert
//


  Point2  pm1, pm2;
  Vector2 v1, v2;


// Variante 1
  pm1.x = (p1->x + p2->x) / 2.0;
  pm1.y = (p1->y + p2->y) / 2.0;

  pm2.x = (p1->x + p3->x) / 2.0;
  pm2.y = (p1->y + p3->y) / 2.0;

  v1.dx = -p2->y + p1->y;
  v1.dy =  p2->x - p1->x;

  v2.dx = -p3->y + p1->y;
  v2.dy =  p3->x - p1->x;

  return UT2D_pt_int2pt2vc (pum, &pm1, &v1, &pm2, &v2);

// Variante 2: unter VC6 (speed optimized) um 10% langsamer
//  als Variante 1, obwohl 2 Divisionen weniger!
/*
  pm1->x = (p1->x + p2->x);
  pm1->y = (p1->y + p2->y);

  pm2->x = (p1->x + p3->x);
  pm2->y = (p1->y + p3->y);

  v1[0] = (-p2->y + p1->y);
  v1[1] = ( p2->x - p1->x);

  v2[0] = (-p3->y + p1->y);
  v2[1] = ( p3->x - p1->x);

  rc = UT2D_pt_int2pt2vc (pum, pm1, v1, pm2, v2);
  pum->x /= 2.0;
  pum->y /= 2.0;

  return rc;
*/

}


//==================================================================
  int UT3D_pt_gcp_3pt (Point *ps, Point *p1, Point *p2, Point *p3) {
//==================================================================
// UT3D_pt_gcp_3pt           gravity-centerPoint of triangle

  ps->x = (p1->x + p2->x + p3->x) / 3.;
  ps->y = (p1->y + p2->y + p3->y) / 3.;
  ps->z = (p1->z + p2->z + p3->z) / 3.;

  // *ps = *p1;

  return 0;

}


//==============================================================================
  int UT3D_pt_intTriaLnu (Point *ptx, Triangle *tri, Point *ptl, Vector *vcl) {
//==============================================================================
// intersect Triangle - LineUnlimited
// Retcod:
//   1 = NO  = point outside triangle
//   0 = YES = point inside triangle
//  -1 = ~~  = point is on a sideline


// Intersect Line-Plane, check if Point inside Triangle ?
// UT3D_pt_intptvcpln
// UT3D_vc_perpTria

  Vector  vn;


  // normalVektor auf 3Eck UT3D_vc_perpTria
  UT3D_vc_perp3pt (&vn, tri->pa[0], tri->pa[1], tri->pa[2]);
    // GR_tDyn_vc (&vn, tri->pa[0], 3, 0);


  // Intersect Line-Plane
  UT3D_pt_intptvcpln (ptx, ptl, vcl, tri->pa[0], &vn);
    // GR_Disp_pt (ptx, SYM_TRI_S, 2);

  // Intersect Line-Plane auch: UT3D_pt_intlnpl1 + UT3D_slen_ptpl


  // check if Point inside Triangle
  return UT3D_ck_pt_in_tria (tri->pa[0], tri->pa[1], tri->pa[2], ptx);

}


//================================================================
  int UTRI_ln_int_LnFac (Point *ip,
                         Point *pf1, Point *pf2, Point *pf3,
                         Point *pl1, Point *pl2) {
//================================================================
// intersect Line - Face.
// Punkt pl1 muss im Face fp1-3 liegen (MSH_if_find_ptmsh).
// Input:
//   fp1-3    facePoints
//   lp1-2    LinePoints
// Output:
//   retCode  0  lp2 is also inside Face fp1-3.
//            1  Line intersect with edge 1
//            2  Line intersect with edge 2
//            3  Line intersect with edge 3
//            4  Line ends on edge 1
//            5  Line ends on edge 2
//            6  Line ends on edge 3
//            7  Line goes tru pf1
//            8  Line goes tru pf2
//            9  Line goes tru pf3
//           10  Line ends on pf1
//           11  Line ends on pf2
//           12  Line ends on pf3


  int     i1, irc;
  int     s1, s2, s3;
  double  d1, d2, dd, dx, dp, dz, dv;
  Point   *pe1, *pe2;
  Point2  ip2;
  Point2  *p1, *p2, *p3, *e1, *e2;
  Vector2 v1, v2, v3, vl;

  // DEB_dump_obj__ (Typ_PT, pl2, "UTRI_ln_int_LnFac - lp2: ");
  // DEB_dump_obj__ (Typ_PT, pl1, "  pl1: ");
  // DEB_dump_obj__ (Typ_PT, pf1, "  pf1: ");
  // DEB_dump_obj__ (Typ_PT, pf2, "  pf2: ");
  // DEB_dump_obj__ (Typ_PT, pf3, "  pf3: ");


  p1 = (Point2*)pf1;
  p2 = (Point2*)pf2;
  p3 = (Point2*)pf3;
  e1 = (Point2*)pl1;
  e2 = (Point2*)pl2;

  UT2D_vc_2pt (&v1, p1, p2);
  UT2D_vc_2pt (&v2, p2, p3);
  UT2D_vc_2pt (&v3, p3, p1);

  UT2D_vc_2pt (&vl, e1, e2);


  // check if lp2 lies also inside Face fp1-3.
  irc = UT2D_ck_pt_in_tria__ (p1, p2, p3, e2);

  if(irc == 1) goto L_out;   // 1=outside Face


  if(irc == 0) {             // 0=inside Face; project point to face.
     *ip = *pl2;
     // get ip= pl2 projected to Face
     UTRI_pt_prjZpt3pt (ip, pl2, pf1, pf2, pf3);
     goto L_exit;
  }

  if(irc == -4) {
    irc = 10;
    *ip = *pf1;
    goto L_exit;

  } else if (irc == -5) {
    irc = 11;
    *ip = *pf2;
    goto L_exit;

  } else if (irc == -6) {
    irc = 12;
    *ip = *pf3;
    goto L_exit;



  // point is on a edge.
  } else if(irc == -1) {
    irc = 4;
    pe1 = pf1;
    pe2 = pf2;

  } else if (irc == -2) {
    irc = 5;
    pe1 = pf2;
    pe2 = pf3;

  } else if (irc == -3) {
    irc = 6;
    pe1 = pf3;
    pe2 = pf1;
  }

  // interpolate PointPosition
  UT3D_pt_ipl_2ptpt2 (ip, pe1, pe2, (Point2*)pl2);
  goto L_exit;




  //----------------------------------------------------------------
  L_out:
  // lp2 does not lie inside face.
  // test which edge is crossing Line lp1-2.
  // set side; s1, s2, s3.
  s1 = UT2D_sid_ptvc__ (e2, p1, &v1);
  s2 = UT2D_sid_ptvc__ (e2, p2, &v2);
  s3 = UT2D_sid_ptvc__ (e2, p3, &v3);
    // printf(" s1=%d s2=%d s3=%d\n",s1,s2,s3);



  // fix irc = side to intersect.
  // man muss beide Edges, wo e2 rechts liegt, testen.
  if(s1 > 0) {     //  test 2 & 3 (p3)
    i1 = UT2D_sid_ptvc__ (p3, e1, &vl);
    if(i1 < 0) irc = 3;   // p3=rechts
    if(i1 > 0) irc = 2;   // p3=links
    if(i1 == 0) {irc = 9; *ip = *pf3; goto L_exit;}  // Line goes tru pf3
    goto L_int;
  }


  if(s2 > 0) {     //  test 1 & 3 (p1)
    i1 = UT2D_sid_ptvc__ (p1, e1, &vl);
    if(i1 < 0) irc = 1;   // p1=rechts
    if(i1 > 0) irc = 3;   // p1=links
    if(i1 == 0) {irc = 7; *ip = *pf1; goto L_exit;}  // Line goes tru pf1
    goto L_int;
  }


  if(s3 > 0) {     //  test 1 & 2 (p2)
    i1 = UT2D_sid_ptvc__ (p2, e1, &vl);
    if(i1 < 0) irc = 2;   // p2=rechts
    if(i1 > 0) irc = 1;   // p2=links
    if(i1 == 0) {irc = 8; *ip = *pf2; goto L_exit;}  // Line goes tru pf2
    // goto L_int;
  }


  // intersect line with edge irc.
  // & interpolate PointPosition
  L_int:
    printf(" irc=%d\n",irc);
  if(irc == 1) {
    i1 = UT2D_pt_int2pt2vc (&ip2, e1, &vl, p1, &v1);
    UT3D_pt_ipl_2ptpt2 (ip, pf1, pf2, &ip2);
  }
  if(irc == 2) {
    i1 = UT2D_pt_int2pt2vc (&ip2, e1, &vl, p2, &v2);
    UT3D_pt_ipl_2ptpt2 (ip, pf2, pf3, &ip2);
  }
  if(irc == 3) {
    i1 = UT2D_pt_int2pt2vc (&ip2, e1, &vl, p3, &v3);
    UT3D_pt_ipl_2ptpt2 (ip, pf3, pf1, &ip2);
  }
    // DEB_dump_obj__ (Typ_PT2, &ip2, "  ip2: ");

    
  //----------------------------------------------------------------
  L_exit:
    // DEB_dump_obj__ (Typ_PT, ip, "ex UTRI_ln_int_LnFac  %d ",irc);
  return irc;

}


//================================================================
  int UT3D_ln_intTriaPln (Line *lni, Triangle *tria, Plane *pln) {
//================================================================
// Line = intersect Triangle - Plane
// ret   -2  Dreieck liegt ganz ausserhalb Plane
// ret   -1  Dreieck und plane sind coplanar
// ret    0  OK; Intersectionline out in lni.
// ret    1  Kante liegt genau in der Ebene
// ret    2  ein Punkt liegt genau in der Ebene (lni.p1 & p2)

  // int   iEdg;

  return UT3D_ln_int_tria_pln (lni, pln,
                               tria->pa[0], tria->pa[1], tria->pa[2]);

  return 0;

}


//================================================================
  int UT3D_ln_int_tria_pln (Line *lni, Plane *pln,
                            Point *p0, Point *p1, Point *p2) {
//================================================================
// Line = intersect Triangle - Plane
// Output:
//   retCod   -3  face outside plane
//            -2  face and plane coplanar
//            -1  point on plane, no intersection
//             0  Intersection tru 2 edges, line out in lni.
//             1  point on plane, intersection on 1 edge, line out
//             2  edge on plane, line out
//   lni      the intersection-line -
//   lni.typ  2 digits- first digit: edge of startPt of line, 
//            - second digit: edge of endPoint of line.
//            retCod=0: digit-1: edge of startpoint; 1=p0-p1, 2=p1-p2, 3=p2-p0
//            - digit-2: edge of endPoint of line; 1=p0-p1, 2=p1-p2, 3=p2-p0
//            - eg  20 = startpt on edge p1-p2; no intersect for endpoint
//            retCod=1: digit-1: startpoint; 1=p0, 2=p1, 3=p2;
//            - digit-2: edge of endPoint of line; 1=p0-p1, 2=p1-p2, 3=p2-p0
//            - eg  31 = startpt is p2; endPoint on edge p0-p1



  int       i1, i2, i3;
  int       is0, is1, is2;
  double    dn0, dn1, dn2;
  double    d1;
  Point     *pp1, *pp2, pt1, pt2, pt3;
  // Vector    E1, E2, N1, D;


  // printf("UT3D_ln_intTriaPln \n");
    // GR_Disp_triv (tria, 9, -1, 1);
    // GR_tDyn_vc (&pln->vz, &pln->po, 8, 0);


  // die Normalabstaende der Punkte des Dreiecks tria von der Ebene
  // pln errechnen.
  dn0 = UT3D_slen_ptpl (p0, pln);
  dn1 = UT3D_slen_ptpl (p1, pln);
  dn2 = UT3D_slen_ptpl (p2, pln);
    // printf(" dn1/2/3=%f %f %f\n",dn0,dn1,dn2);

  // Vorzeichen der Normalabstaende bestimmen
  is0 = DSIGTOL (dn0, UT_TOL_min1);
  is1 = DSIGTOL (dn1, UT_TOL_min1);
  is2 = DSIGTOL (dn2, UT_TOL_min1);
    // printf(" is1/2/3=%d %d %d\n",is0,is1,is2);


  //----------------------------------------------------------------
  // test all points outside plane
  // wenn alle Punkte auf der gleichen Seite der Ebene liegen (alle
  // Vorzeichen gleich) dann liegt Dreieck ganz ausserhalb Plane.
  if       ((is0 > 0) && (is1 > 0) && (is2 > 0)) {
     return -3;

  } else if((is0 < 0) && (is1 < 0) && (is2 < 0)) {
     return -3;
  }


  //----------------------------------------------------------------
  // test one point in plane
  // Sonderfall: alle Punktabstaende sind 0 (idente Traegerebenen (coplanar)
  // Sonderfall: 2 Punktabstaende sind 0 (eine Kante liegt in der Ebene)
  // } else if((is0 == 0) && (is1 == 0) && (is2 == 0)) {
  if(is0 == 0) {
    if(is1 == 0) {
      if(is2 == 0) {    // alle Punktabstaende sind 0
        return -2;      // coplanar
      }
      // Kante 0-1 liegt in der Ebene
      pp1 = p0;  pp2 = p1;  lni->typ = 11;  goto L_edge;
    } else if(is2 == 0) {
      // Kante 0-2 liegt in der Ebene
      pp1 = p2;  pp2 = p0;  lni->typ = 33;  goto L_edge;
    }
    if(is1 == is2) return -1; // p0 in plane, p1-p2 outside
    // startPt=p0; inters. p1-p2
    lni->p1 = *p0;
    UT3D_pt_intlnpl1 (&lni->p2, &d1, p1, p2, dn1, dn2);
    lni->typ = 12;
    return 1;

  } else if(is1 == 0) {
    if(is2 == 0) {
      // Kante 1-2 liegt in der Ebene
      pp1 = p1;  pp2 = p2;  lni->typ = 22;  goto L_edge;
    }
    if(is2 == is0) return -1; // p1 in plane, p2-p0 outside
    // startPt=p1; inters. p2-p0
    lni->p1 = *p1;
    UT3D_pt_intlnpl1 (&lni->p2, &d1, p2, p0, dn2, dn0);
    lni->typ = 23;
    return 1;

  } else if(is2 == 0) {
    if(is0 == is1) return -1; // p2 in plane, p0-p1 outside
    // startPt=p2; inters. p0-p1
    lni->p1 = *p2;
    UT3D_pt_intlnpl1 (&lni->p2, &d1, p0, p1, dn0, dn1);
    lni->typ = 31;
    return 1;

  }


  //----------------------------------------------------------------
  // intersect 2 edges -

  i1 = 0;
  i2 = 0;
  i3 = 0;

  if(is0 != is1) {               // Kante p0 - p1
    // inters. edge p0-p1
    UT3D_pt_intlnpl1 (&pt1, &d1, p0, p1, dn0, dn1);
    ++i1;  // pt1 occupied
  }


  if(is0 != is2) {        // Kante p0 - p2
    // inters. edge p0-p2
    UT3D_pt_intlnpl1 (&pt3, &d1, p0, p2, dn0, dn2);
    ++i3;   // pt3 occupied 
  }


  if(is1 != is2) {        // Kante p1 - p2
    // inters. edge p1-p2
    UT3D_pt_intlnpl1 (&pt2, &d1, p1, p2, dn1, dn2);
    ++i2; // pt2 occupied
  }

  if(i1) {
    if(i2) {
      // ps=pt1, pe=pt2; iEdg=12
      lni->p1 = pt1;
      lni->p2 = pt2;
      lni->typ = 12;
      goto L_done;
    }
    // ps=pt3, pe=pt1;   iEdg=31
    lni->p1 = pt3;
    lni->p2 = pt1;
    lni->typ = 31;
    goto L_done;
  }

  // ps=pt2, pe=pt3; iEdg=23
  lni->p1 = pt2;
  lni->p2 = pt3;
  lni->typ = 23;


  L_done:
    // printf("ex UT3D_ln_intTriaPln 0\n");
    // GR_Disp_ln (lni, 11);

  return 0;

  //----------------------------------------------------------------
  L_edge:
    // both points of edge pp1-pp2 are in the plane
    lni->p1 = *pp1;
    lni->p2 = *pp2;
    return 2;

}


//=====================================================================
  int UT3D_ln_intTriaTria (Line *lni, Triangle *tri1, Triangle *tri2) {
//=====================================================================
// Line = intersect Triangle - Triangle
// ret    0  OK; Intersectionline out in lni.
// ret   -1  Dreiecke sind coplanar
// ret   -2  Dreieck beruehren sich nicht

// see also UT3D_ln_intTriaPln


  int       irc, i1, i2, ib;
  int       is0, is1, is2;
  double    dn0, dn1, dn2;
  double    d1, da[4];
  Vector    vc1, vc2;
  Point     pa[4];
  Plane     plt1, plt2;
  Line      lnt1, lnt2;


  // printf("UT3D_ln_intTriaTria \n");
    // GR_Disp_tria (tri1, 9);
    // GR_Disp_tria (tri2, 9);



  // make plane plt1 of tria1
  // vc1=p0-p1, vc2=p0-p2, vcn1=crossProd vc1-vc2
  UT3D_pl1_tria (&plt1, tri1);
    // DEB_dump_obj__(Typ_PLN, &plt1, " plt1");
    // GR_tDyn_vc (&plt1.vz, &plt1.po, 8, 0);


  // make plane plt2 of tria2
  // vc1=p0-p1, vc2=p0-p2, vcn2=crossProd vc1-vc2
  UT3D_pl1_tria (&plt2, tri2);
    // DEB_dump_obj__(Typ_PLN, &plt2, " plt2");
    // GR_tDyn_vc (&plt2.vz, &plt2.po, 9, 0);


  // inters. tri1 mit Plane of tri2
  irc = UT3D_ln_intTriaPln (&lnt1, tri1, &plt2);
  if(irc < 0) return irc;
    // DEB_dump_obj__(Typ_LN, &lnt1, " lnt1");
    // GR_Disp_ln (&lnt1, 8);


  // inters. tri2 mit Plane of tri1
  irc = UT3D_ln_intTriaPln (&lnt2, tri2, &plt1);
  if(irc < 0) return irc;
    // DEB_dump_obj__(Typ_LN, &lnt2, " lnt2");
    // GR_Disp_ln (&lnt2, 11);



  //----------------------------------------------------------------
  // select the overlapping part of the 2 lines lnt1, lnt2

  // vector intersectionline m len=1
  UT3D_vc_perp2vc (&vc1, &plt1.vz, &plt2.vz);

  // longest vectorpart
  ib = UT3D_bplen_vc (&d1, &vc1);
    // printf(" bp=%d\n",ib);

  pa[0] = lnt1.p1;
  pa[1] = lnt1.p2;
  pa[2] = lnt2.p1;
  pa[3] = lnt2.p2;


  // copy the backplane-part of the points
  UT1D_ndb_npt_bp (da, pa, 4, ib);
    // for(i1=0; i1<4; ++i1) printf(" da[%d]=%f\n",i1,da[i1]);


  // select the overlapping part of the 2 lines
  // beide Lines aufsteigend sortieren;
  // wenn die beiden mittleren Punkte (Werte) nicht ueberlappen, dann
  //   beruehren sich die 3Ecke nicht;
  // else: die beiden mittleren Punkte (Werte) sind die gemeinsame Strecke.
  if(da[1] < da[0]) {   // ascending
    MEM_swap_2db (&da[0], &da[1]);
    MEM_swap__ (&pa[0], &pa[1], sizeof(Point));
  }
  if(da[3] < da[2]) {   // ascending
    MEM_swap_2db (&da[2], &da[3]);
    MEM_swap__ (&pa[2], &pa[3], sizeof(Point));
  }
    // for(i1=0; i1<4; ++i1) printf(" da[%d]=%f\n",i1,da[i1]);


  // in der Mitte darf kein Loch sein
  if(da[2] > da[0]) {
    if(da[2] > da[1]) return -2;

  } else {
    if(da[0] > da[3]) return -2;
  }



  // Startpunkt: 0 oder 2
  if(da[0] > da[2]) lni->p1 = pa[0];
  else              lni->p1 = pa[2];
  // Endpunkt: 1 oder 3
  if(da[1] < da[3]) lni->p2 = pa[1];
  else              lni->p2 = pa[3];


    // GR_Disp_ln (lni, 11);


  return 0;
}

//================================================================
  int UT3D_pl1_tria (Plane *plt1, Triangle *tri1) {
//================================================================
// make Plane from Triangle
// Erzeugt nur den Z-Vektor mit Laenge 1. und plane.p
// setzt keinen vx, vy Vektor !
// Als origin wird pa[0] uebernommen.
// ACHTUNG: in UT3D_ln_intTriaTria 2 x einbauen !!!!!!!!!!!!!!!


  Vector    vc1, vc2;

  // printf("UT3D_ln_intTriaPln \n");
    // GR_Disp_tria (tria, 9);



  // make plane plt1 of tria1
  // vc1=p0-p1, vc2=p0-p2, vcn1=crossProd vc1-vc2
  UT3D_vc_2pt (&vc1, tri1->pa[0], tri1->pa[1]);
  UT3D_vc_2pt (&vc2, tri1->pa[0], tri1->pa[2]);
  UT3D_vc_perp2vc (&plt1->vz, &vc1, &vc2);
  UT3D_vc_setLength (&plt1->vz, &plt1->vz, 1.);
    // GR_tDyn_vc (&plt1->vz, tri1->pa[0], 5, 0);

  UT3D_pl_ptpl (plt1, tri1->pa[0]);  // setzt plane.p !!


  return 0;

}


/*
//================================================================
  int UT3D_pt_projPtTria (Point *pto, double *dd,                   // out
                          Triangle *trii, Point *pti) {             // in
//================================================================
// project point onto triangle / Normalabstand Triangle-Point
//  Retcod, ob der Punkt auf dem 3Eck liegt oder nicht.
//  Wenn ja: NormalAbstand und ProjektionsPunkt am Dreieck.
// RetCod:  0  pti is inside Triangle
//          1  pti is NOT inside Triangle

// UT3D_pt_projPtTria
// UT3D_pt_intPtVcTria

// see UT3D_ln_intTriaPln UT3D_ln_intTriaTria UT3D_pt_intptvcpl_ UT3D_pt_projptpl

  int   irc;
  Plane plt;


  // Normalvektor auf Triangle
  UT3D_pl1_tria (&plt, trii);


  // intersect pti-Normalvektor mit Triangle-plane
  UT3D_pt_intptvcpl_ (pto, &plt, pti, &plt.vz);


  // is point inside triangle ?  0=yes, 1=no
  // ACHTUNG ! GEHT DZT NUR BEI X-Y-PLANAREN EBENEN !
  irc = UT2D_ck_pt_in_tria__ (trii->pa[0], trii->pa[1], trii->pa[2], pti);
  if(irc != 0) goto L_exit;


  // Abstand setzen ..
  *dd = UT3D_len_2pt (pti, pto);


  L_exit:
    DEB_dump_obj__ (Typ_PT, pto, "ex UT3D_pt_projPtTria %d %f",irc,*dd);
  return irc;

}
*/


//======================================================================
  int UT2D_2pt_tngLnCiY (Point2 *ps, Point2 *py,
                         Point2 *ls, Point2 *le, double yy, double rc) {
//======================================================================
/*
 Beruehrung Kreis (Mittelpunkt auf horiz. Gerade) - LineSegment
 Input:
   ls-le    ein LineSegment
   yy       Y-Wert der horiz. Geraden, auf der der Kreis-Mittelpunkt liegt
   rc       Kreisradius
 Output:
   ps       der Kreis-Beruehrungspunkt am LineSegment
   py       der Kreis-Mittelpunkt auf der horiz. Geraden
 RetCod:
  -1  Kreis beruehrt LineSegment nicht.
   0  Kreis beruehrt Line; ps und py benutzbar.
   1  Kreis beruehrt Line nicht-tangential an einem Endpunkt; ps/py benutzbar
   2  Kreis beruehrt gesamte Line (coplanar)


see UT2D_pt_intptvcy UT2D_pt_int2vc2pt UT3D_2ln_tngTriPlSph

*/


  int        irc;
  double     k1, d0, d1, d2;
  Vector2    vcl, vnl;
  Point2     p1, p2;


  // printf("UT2D_2pt_tngLnCiY %f %f\n",yy,rc);
    // GR_tDyn_ln2_2pt (ls, le, 9);
    // GR_Disp_message (1);   // 0=Aus, 1=Ein.


  // vector line
  UT2D_vc_2pt (&vcl, ls, le);


  // vector normal to line
  UT2D_vc_rot_90_ccw (&vnl, &vcl);


  // intersection
  k1 = vnl.dy / vnl.dx;
    // printf(" k1=%f\n",k1);


  // Linie horizontal;
  if(fabs(k1) < UT_TOL_min2) {
    if(UTP_comp2db(fabs(ls->y - yy),rc,UT_TOL_pt) == 1) {irc = 2; goto L_done;}
    if(ls->y > (yy + rc)) {irc = -1; goto L_done;}
    if(ls->y < (yy - rc)) {irc = -1; goto L_done;}
    if(le->x > ls->x) *ps = *le;
    else *ps = *ls;
      // printf("UT2D_2pt_tngLnCiY I001\n");  // es fehlt py noch
    irc = 1;
    goto L_done;
  }

  // intersect Normalvektoren auf die Punkte ls,le mit BasisGeraden
  // die Punkte auf der BasisGeraden
  p1.x = ls->x + ((yy - ls->y) / k1);
  p1.y = yy;
    // GR_Disp_pt2 (&p1, SYM_STAR_S, 2);

  p2.x = le->x + ((yy - le->y) / k1);
  p2.y = yy;
    // GR_Disp_pt2 (&p2, SYM_STAR_S, 2);


  // die Abstaende von der BasisGeraden ?
  d1 = UT2D_len_2pt (ls, &p1);
  d2 = UT2D_len_2pt (le, &p2);
  if(ls->y < 0.) d1 = -d1;
  if(le->y < 0.) d2 = -d2;
    // printf(" d1=%f d2=%f\n",d1,d2);


  if(rc < d1) goto L_outside_l;
  if(rc > d2) goto L_outside_r;
  // Tangentenpunkt inside ls-le;

  // get parameter f. rc along d1-d2
  UTP_param_p0p1px (&d0, d1, d2, rc);
    // printf(" d0=%f\n",d0);

  UT2D_pt_tra_pt_pt_par (ps, ls, le, d0);
  UT2D_pt_tra_pt_pt_par (py, &p1, &p2, d0);
  irc = 0;
  goto L_done;


  L_outside_r:
    // printf(" L_outside_r:\n");
  *ps = *le;
  UT2D_solvtriri_a (&d1, le->y - yy,  rc);
  py->x = d1 + le->x;
  py->y = yy;
  irc = 1;
  goto L_done;


  L_outside_l:
    // printf(" L_outside_l:\n");
  if(fabs(ls->y - yy) > rc) {irc = -1; goto L_done;}
  *ps = *ls;
  UT2D_solvtriri_a (&d1, ls->y - yy,  rc);
  py->x = d1 + ls->x;
  py->y = yy;
  irc = 1;


  L_done:
  // printf("ex UT2D_2pt_tngLnCiY %d\n",irc);
    // GR_Disp_pt2 (ps, SYM_STAR_S, 3);
    // GR_Disp_pt2 (py, SYM_STAR_S, 3);
    // printf(" dist=%f\n",UT2D_len_2pt(ps,py));
    // GR_Disp_message (0);   // 0=Aus, 1=Ein.


  return irc;

}


//================================================================
  int UT3D_2ln_tngTriPlSph (Line *lnt, Line *lnp, double *aDiff,        // out
                            Triangle *tria, Plane *pls, double rs) {    // in
//================================================================
/*
 Ob eine Kugel mit Radius rs, Mittelpunkt auf der Ebene pls das 3Eck tria
   beruehren wuerde.
 Gesucht sind die Mittelpunktsbahn der Kugel auf der Ebene (lnp)
  und die Beruehrungslinie am 3Eck (lnt).
  (nur auf der Seite der pos. Z-Achse der Ebene).
 Ausgabe ist also je eine Linie am 3Eck und in der Ebene;
   die Verbindungslinien von lnt - lpt stehen normal auf das 3Eck;
   die Laenge der Verbindungslinien von lnt - lpt ist fix (Rad rs).
 RetCod:
  -1  Kugel beruehrt 3Eck nicht.
   0  Kugel beruehrt 3Eck; lnt und lnp benutzbar.
   1  Kugel beruehrt 3Eck an einer Linie; lnt und lnp benutzbar.
   2,3 oder 4: Kugel beruehrt nur den Punkt 0,1,od 2 des 3Eck's.
   5  Kugel beruehrt gesamte Flaeche (coplanar)
   6  3Eck und Plane sind normal aufeinander (koennte man berechnen..)
   7  3Eck u Ebene sind coplanar genau mit Abstand rs (noch nicht implem.)
   8  Kugel beruehrt 3Eck nicht-tangential an Kante, lnt und lnp benutzbar.
      Kante ist parallel zu Plane; 3Eck lieget hoeher als Kante
   9  wie 8; jedoch ist Kante oben.
 aDiff:
   DifferenzWinkel (Fehlerwinkel) = Winkel zwischen dem 3Ecks-NormalVektor
   und der Linie lnt.p1 - lnp.p1. 0.0 bei RetCod=0; benutzbar bei RetCod=8.

see UT3D_ln_intTriaPln UT3D_ln_intTriaTria UT3D_pt_intptvcpl

*/

  int      irc, imin, imax, i2s, i2e, i1, i2, i3;
  double   d1, d2, dTab[3], pp1, pp2, ppe;
  Point    pTab[3];
  Vector   vk, vx;
  Plane    plt;


  // printf("UT3D_2ln_tngTriPlSph rs=%f\n",rs);
    // GR_tDyn_txtA (tria->pa[0], "0", 1);
    // GR_tDyn_txtA (tria->pa[1], "1", 1);
    // GR_tDyn_txtA (tria->pa[2], "2", 1);


  // - einen Normalvector (plt.vz) aufs 3Eck ermitteln.
  // einfacher waere UT3D_vc_perp3pt ..
  UT3D_pl1_tria (&plt, tria);
    // GR_tDyn_vc (&plt.vz, &plt.po, 9, 0);


  // Die vektoren muessen zueinender zeigen ...
  irc = UT3D_sid_2vc (&plt.vz, &pls->vz);       // od UT3D_skp_2vc
  if(irc > 0) {irc = -1; goto L_exit;}  // 3Eck zeigt auf same Seite ..
  if(irc < 0) goto L_1; 
  // planes parallel; ein Problemfall; die Intersectline am 3Eck
  // ist die Intersectline mit der Plane und inder Plane muesste man den
  // Abstand rs abtragen ..
  irc = 6; goto L_exit;



  L_1:    // Richtige seite, nicht parallel ...
  irc = 0;
  // - Abstaende der 3 3EcksPunkte von der Plane entlang Normalvector an 3Eck
  UT3D_ptDi_intptvcpln (&pTab[0], &dTab[0], pls, tria->pa[0], &plt.vz);
  UT3D_ptDi_intptvcpln (&pTab[1], &dTab[1], pls, tria->pa[1], &plt.vz);
  UT3D_ptDi_intptvcpln (&pTab[2], &dTab[2], pls, tria->pa[2], &plt.vz);
    // GR_Disp_pt (&pTab[0], SYM_STAR_S, 3);
    // GR_Disp_pt (&pTab[1], SYM_STAR_S, 3);
    // GR_Disp_pt (&pTab[2], SYM_STAR_S, 3);
    // printf(" d1,d2,d3=%f %f %f\n",dTab[0],dTab[1],dTab[2]);



  // min-max-Abst von der Plane entlang Normalvector
  imin = UTP_min (3, dTab);   // ind minVal
  imax = UTP_max (3, dTab);   // ind maxVal
    // printf(" i/dmin,i/dmax=%d %f %d %f\n",imin,dTab[imin],imax,dTab[imax]);




  // Radius == Abstand: beruehrt ...
  if(UTP_comp2db(rs, dTab[imin], UT_TOL_pt) == 1) irc = imin+2;
  if(UTP_comp2db(rs, dTab[imax], UT_TOL_pt) == 1) irc = imax+2;
  if(irc > 0) {
    // imin oder imax liegt ganz genau auf der Kugel; der andere nicht.
    i1 = irc - 2;
    if(i1 == imin) i3 = imax;
    else           i3 = imin;
    // den Index des 3. Punktes bestimmen ..
    i2 = UT3D_ind3Tria_2ind(i1, i3);    // printf(" i3=%d\n",i3);
    // liegt der 3. Punkt ebenfalls auf der Kugel ?
    if(UTP_comp2db(rs, dTab[i2], UT_TOL_pt) == 1) {
      irc = 1;
      UT3D_indTria2ccw (&i1, &i2);  // 3EckIndices orientieren
      goto L_Kante;
    }
    goto L_exit;
  }



  // wenn Radius innerhalb der Abstaende: normales intersect ..
  if((rs < dTab[imax]) && (rs > dTab[imin])) goto L_2;



  // den Index des 3. Punktes bestimmen ..
  i1 = UT3D_ind3Tria_2ind(imin, imax);    // printf(" i1=%d\n",i1);

  // testen ob eine Kante parallel zur Plane liegt
  i2 =  -1;
  // testen ob der Abstand des 3. punktes gleich mit einem anderen ist
  if(UTP_comp2db(dTab[i1], dTab[imin], UT_TOL_pt) == 1) i2 = imin;
  if(UTP_comp2db(dTab[i1], dTab[imax], UT_TOL_pt) == 1) i2 = imax;
    // printf(" i1=%d i2=%d\n",i1,i2);
  if(i2 < 0) {irc = -1; goto L_exit;}


  // die Kante i1-i2 ist parallel zur Plane.
  // if NormalAbstand des Punktes tria->pa[i1] von Plane > Radius, exit.
  // (die Kante ist oberhalb der Kugel)        (see UT3D_slen_ptpl)
  d1 = UT3D_acos_2vc (&pls->vz, (Vector*)tria->pa[i1]) - pls->p;
    // printf(" d1=%f\n",d1);
  if(d1 > rs) {irc = -1; goto L_exit;}


  // den Index des 3. Punktes bestimmen ..
  i3 = UT3D_ind3Tria_2ind(i1, i2);    // printf(" i3=%d\n",i3);

  // i1-i2 ist nun die Kante; i3 der 3. punkt.
  if(dTab[i1] < dTab[i3]) {
    // Kante liegt unten
    irc = 8;
    // printf(" unten\n");
    // skippen wenn Kante ganz unten liegt
    if(dTab[i1] < rs) {irc = -1; goto L_exit;}
  } else {
    // Kante liegt oben
    irc = 9;
    // printf(" oben\n");
    // skippen wenn Kante ganz oben liegt
    if(dTab[i1] > rs) {irc = -1; goto L_exit;}
  }


  // Kante ausgeben ..
  goto L_Kante;


  irc = -1;
  goto L_exit;



  //----------------------------------------------------------------
  // Kante liegt an der Kugel; imin oder imax und i3.
  // i1-i2 ist nun die Kante; i3 der 3. punkt.
  L_Kante:       // die kante i1-i2 ausgeben ..

    // lnt = Kante
    lnt->p1 = *tria->pa[i1];
    lnt->p2 = *tria->pa[i2];

    // project points -> Plane
    UT3D_ptDi_intptvcpln(&pTab[i1], &d1, pls, tria->pa[i1], &pls->vz);
    UT3D_pt_trapt2pt (&pTab[i2], &pTab[i1], tria->pa[i1], tria->pa[i2]);
      // GR_Disp_pt (&pTab[i1], SYM_STAR_S, 4);
      // GR_Disp_pt (&pTab[i2], SYM_STAR_S, 4);

    // der Normalabstand ist d1; den Verschiebeabstand d2 errechnen
    UT2D_solvtriri_a (&d2, d1, rs);
      // printf(" d2=%f\n",d2);

    // make Vektor vx; len=d2, normal auf Kante und auf pls->vz
    UT3D_vc_2pt (&vk, tria->pa[i1], tria->pa[i2]);
    UT3D_vc_perp2vc (&vx, &vk, &pls->vz);
    UT3D_vc_setLength (&vx, &vx, d2);

    // vx in die gleiche Richtung wie plt.vz orientieren
    if(UT3D_sid_2vc(&vx, &plt.vz) < 0)  { UT3D_vc_invert (&vx, &vx); }

    // lnp = pTab um vx verschieben
    UT3D_pt_traptvc (&lnp->p1, &pTab[i1], &vx);
    UT3D_pt_traptvc (&lnp->p2, &pTab[i2], &vx);

    // Fehlerwinkel aDiff = Winkel zwischen dem 3Ecks-NormalVektor plt.vz
    // und der Linie lnt.p1 - lnp.p1.
    UT3D_vc_2pt (&vx, &lnt->p1, &lnp->p1);
      // GR_tDyn_vc (&vx, &plt.po, 9, 0);
    if(irc == 8) {
      *aDiff = UT3D_angr_2vc__ (&vx, &plt.vz);
    } else {   // irc=1; Kante beruehrt genau.
      *aDiff = 0.;
    }

    goto L_fertig;



  //----------------------------------------------------------------
  // normale intersection.
  // die beiden Punkte am Dreicke und auf der Plane errechnen.
  // der Abstand dieser 2 Punkte von Plane plt ist gleich.
  L_2:
  // Der Abstand soll rs sein; den Abstandsfaktor errechnen.
  i1 = UTP_param_p0p1px (&pp1, dTab[imin], dTab[imax], rs);
    // printf(" pp1=%f\n",pp1);  // muss zwischen 0 - 1 sein ..


  // den ersten Punkt auf der strecke imin-imax errechnen;
  // und zwar am 3Eck
  UT3D_pt_evpar2pt (&lnt->p1, pp1, tria->pa[imin], tria->pa[imax]);
    // GR_Disp_pt (&lnt->p1, SYM_TRI_S, 4);

  // und in der Plane
  UT3D_pt_evpar2pt (&lnp->p1, pp1, &pTab[imin], &pTab[imax]);
    // GR_Disp_pt (&lnp->p1, SYM_TRI_S, 5);


  // nun muss die 2. Strecke bestimmt werden;
  // der Startpunkt ist der dritte, fehlende Punkt 
  i2s = 3 - imin - imax;
    // printf(" i2s=%d\n",i2s);


  // endpunkt i2e: rs muss zwischen i2s und i2e liegen;
  // die Abstansparemter von den beiden Punkten rechnen; jenen
  // parameter, der naeher an der Mitte liegt auswaehlen.
  i1 = UTP_param_p0p1px (&pp1, dTab[i2s], dTab[imin], rs);
  i2 = UTP_param_p0p1px (&pp2, dTab[i2s], dTab[imax], rs);
  if(i1 < 0) {
    goto L_imax;
  } else if(i2 < 0) {
    goto L_imin;
  } else {
    d1 = pp1 - 0.5;
    d2 = pp2 - 0.5;
    if(fabs(d1) > fabs(d2)) goto L_imax;
    else goto L_imin;
  }

  L_imin:
    i2e = imin;
    ppe = pp1;
    goto L_i2e;

  L_imax:
    i2e = imax;
    ppe = pp2;

  L_i2e:
  // Strecke i2s-i2e wie Strecke imin-imax
    // printf(" pp1=%d/%f, pp2=%d/%f\n",i1,pp1,i2,pp2);
    // printf(" i2e,ppe=%d,%f\n",i2e,ppe);

  // den ersten Punkt auf der strecke i2s-i2e errechnen;
  // und zwar am 3Eck
  UT3D_pt_evpar2pt (&lnt->p2, ppe, tria->pa[i2s], tria->pa[i2e]);
    // GR_Disp_pt (&lnt->p2, SYM_STAR_S, 4);

  // und in der Plane
  UT3D_pt_evpar2pt (&lnp->p2, ppe, &pTab[i2s], &pTab[i2e]);
    // GR_Disp_pt (&lnp->p2, SYM_STAR_S, 4);


  *aDiff = 0.;


  // Fertig.
  L_fertig:

    // TEST-ONLY
    // GR_Disp_ln (lnt, 3);
    // GR_Disp_ln (lnp, 3);
    // printf(" dist1=%f\n",UT3D_len_2pt(&lnt->p1, &lnp->p1));
    // printf(" dist2=%f\n",UT3D_len_2pt(&lnt->p2, &lnp->p2));
 



  L_exit:
    // printf("ex UT3D_2ln_tngTriPlSph irc=%d a=%f\n",irc,*aDiff);
  return irc;

}


//=========================================================================
  int UT2D_ck_pt_in_tria__ (Point2 *p1, Point2 *p2, Point2 *p3, Point2 *px) {
//=========================================================================
// UT2D_ck_pt_in_tria__   check if point is inside Triangle
// UT2D_ck_pt_in_tria__                          nach Karl Sauer 2007-01-14
// see UT2D_ck_pt_in_tria__ UT2D_pt_ck_inCv3
// Returncodes:
//   1 = NO  = point outside triangle
//   0 = YES = point inside triangle
//  -1 = ~~  = point is on sideline p1-p2
//  -2 = ~~  = point is on sideline p2-p3
//  -3 = ~~  = point is on sideline p3-p1
//  -4 = ~~  = point is equal to p1
//  -5 = ~~  = point is equal to p2
//  -6 = ~~  = point is equal to p3

// 2008-05-27   UT_TOL_min1 > UT_TOL_min0. RF.



  int       i12, i23, i31, irc;
  double    s12, s23, s31;
  Vector2   nv, v1x;


  // DEB_dump_obj__ (Typ_PT2, px, "UT2D_ck_pt_in_tria__:");
  // DEB_dump_obj__ (Typ_PT2, p1, "   p1:");
  // DEB_dump_obj__ (Typ_PT2, p2, "   p2:");
  // DEB_dump_obj__ (Typ_PT2, p3, "   p3:");
    // GR_Disp_pt2 (px, SYM_STAR_S, 4);
    // GR_tDyn_ln2_2pt (p1, p2, 0);
    // GR_tDyn_ln2_2pt (p1, p3, 0);
    // GR_tDyn_ln2_2pt (p3, p2, 0);
    // GR_tDyn_tx2A (p1, "1", 1);
    // GR_tDyn_tx2A (p2, "2", 1);
    // GR_tDyn_tx2A (p3, "3", 1);


  // normalvektor auf p1-p2
  nv.dx = -(p2->y - p1->y);
  nv.dy = p2->x - p1->x;

  // vektor p1-px
  v1x.dx = px->x - p1->x;
  v1x.dy = px->y - p1->y;

  // skalarprodukt von nv12 x v1x
  s12 = nv.dx * v1x.dx + nv.dy * v1x.dy;
  i12 = DSIGTOL (s12, UT_TOL_min0);
  // (s12 > 0) = punkt ist links von p1-p2
  // (s12 < 0) = punkt ist rechts von p1-p2



  // normalvektor auf p2-p3
  nv.dx = -(p3->y - p2->y);
  nv.dy = p3->x - p2->x;

  // vektor p2-px
  v1x.dx = px->x - p2->x;
  v1x.dy = px->y - p2->y;

  // skalarprodukt von nv12 x v1x
  s23 = nv.dx * v1x.dx + nv.dy * v1x.dy;
  i23 = DSIGTOL (s23, UT_TOL_min0);
  // (s23 > 0) = punkt ist links von p2-p3
  // (s23 < 0) = punkt ist rechts von p2-p3

    // printf(" i12=%d i23=%d\n",i12,i23);
    // printf(" s12=%f s23=%f\n",s12,s23);



  if((i12 != 0) && (i23 != 0)) { // keines der Vorzeichen ist 0 ..
    if(i12 != i23) {
      // echt unterschiedlich - Punkt ist aussen.
      irc = 1;
      goto L_exit;
    }
  }


  // Beide Vorzeichen gleich, oder eine Seite ist 0;
  // 3. Seite ebenfalls testen.
 
  // normalvektor auf p3-p1
  nv.dx = -(p1->y - p3->y);
  nv.dy = p1->x - p3->x;

  // vektor p3-px
  v1x.dx = px->x - p3->x;
  v1x.dy = px->y - p3->y;

  // skalarprodukt von nv12 x v1x
  s31 = nv.dx * v1x.dx + nv.dy * v1x.dy;
  i31 = DSIGTOL (s31, UT_TOL_min0);
  // (s31 > 0) = punkt p2 ist links von p3-p1
  // (s31 < 0) = punkt p2 ist rechts von p3-p1

    // printf(" i31=%d s31=%f\n",i31,s31);


  if(i12 == 0) {
    if(i31 == 0) {irc = -4; goto L_exit;}
    if(i23 == 0) {irc = -5; goto L_exit;}
    // i23 und i31 muessen gleich sein ...
    if(i23 == i31) irc = -1; // genau auf p1-p2
    else irc = 1;


  } else if(i23 == 0) {
    if(i31 == 0) {irc = -6; goto L_exit;}
    // i12 und i31 muessen gleich sein ...
    if(i12 == i31) irc = -2; // genau auf p2-p3
    else irc = 1;


  } else {                   // wenn die beiden Vz gleich waren (1, 1)
    // genau auf p1-p3
    if(i31 == 0)  {irc = -3; goto L_exit;}
    // wenn alle 3 gleich = innen.
    if(i31 == i23) irc = 0;
    else irc = 1;
  }



  //----------------------------------------------------------------
  L_exit:
  // printf("ex UT2D_ck_pt_in_tria__ %d\n",irc);


  return irc;

}


//=========================================================================
  int UT3D_ck_pt_in_tria (Point *p1, Point *p2, Point *p3, Point *px) {
//=========================================================================
// UT3D_ck_pt_in_tria check if point is inside Triangle
// all 4 points must be in the same Plane !


  int      bpi;
  Point2   p21, p22, p23, p2x;
  Point    pb1, pb2;
  Vector   vc1;


    // GR_Disp_pt (px, SYM_STAR_S, 4);
    // GR_tDyn_pcv (p1, p2, 0);
    // GR_tDyn_pcv (p1, p3, 0);
    // GR_tDyn_pcv (p3, p2, 0);
    // GR_tDyn_txtA (p1, "1", 1);
    // GR_tDyn_txtA (p2, "2", 1);
    // GR_tDyn_txtA (p3, "3", 1);


  // get a backplane
  UT3D_box_2pt__ (&pb1, &pb2, p1, p2);     // make box of p1,p2
  UT3D_box_extend (&pb1, &pb2, p3);      // Box mit p3 vergroessern.

  // hier testen ob px inside box ??


  UT3D_vc_2pt (&vc1, &pb1, &pb2);        // vec from box
    // GR_tDyn_vc (&vc1, &pb1, 12, 1); 
  bpi = UT3D_bp_perp_vc (NULL, &vc1);               // backplane from vector
    // printf(" bpi=%d\n",bpi);


  // make 2D-points
  UT2D_pt_tra_pt3_bp (&p21, p1, bpi);
  UT2D_pt_tra_pt3_bp (&p22, p2, bpi);
  UT2D_pt_tra_pt3_bp (&p23, p3, bpi);
  UT2D_pt_tra_pt3_bp (&p2x, px, bpi);


  // 2D-check if point is inside Triangle
  return UT2D_ck_pt_in_tria__ (&p21, &p22, &p23, &p2x);

}


//=========================================================================
  int UT3D_ck_pt_prj_tria (Point *p1, Point *p2, Point *p3, Point *px) {
//=========================================================================
// UT3D_ck_pt_prj_tria          check if projection of point is inside Triangle
// px must not be in triangle-plane; projection is made.
// see UT2D_ck_pt_in_tria__
// Returncodes:
//   1 = NO  = point outside triangle
//   0 = YES = point inside triangle
//  -1 = ~~  = point is on a sideline



  int       i12, i23, i31, irc;
  double    s12, s23, s31;
  Vector    v12, v23, v31, v1x, vnt, vns;



    // GR_Disp_pt (px, SYM_STAR_S, 4);
    // GR_tDyn_pcv (p1, p2, 0);
    // GR_tDyn_pcv (p1, p3, 0);
    // GR_tDyn_pcv (p3, p2, 0);
    // GR_tDyn_txtA (p1, "1", 1);
    // GR_tDyn_txtA (p2, "2", 1);
    // GR_tDyn_txtA (p3, "3", 1);


  // Normalvektor auf das 3Eck rechnen
  // see UT3D_vc_perp3pt
  UT3D_vc_2pt (&v12, p1, p2);   // vektor p1-p2
  UT3D_vc_2pt (&v23, p2, p3);   // vektor p2-p3
  vnt.dx = v12.dy * v23.dz - v12.dz * v23.dy;
  vnt.dy = v12.dz * v23.dx - v12.dx * v23.dz;
  vnt.dz = v12.dx * v23.dy - v12.dy * v23.dx;
    // GR_tDyn_vc (&vnt, p1, 12, 0); 



  // normalvektor auf p1-p2
  vns.dx = v12.dy * vnt.dz - v12.dz * vnt.dy;
  vns.dy = v12.dz * vnt.dx - v12.dx * vnt.dz;
  vns.dz = v12.dx * vnt.dy - v12.dy * vnt.dx;
    // GR_tDyn_vc (&vns, p1, 12, 0); 


  // vektor p1-px
  UT3D_vc_2pt (&v1x, p1, px);


  // skalarprodukt von nv12 x v1x
  s12 = vns.dx * v1x.dx + vns.dy * v1x.dy + vns.dz * v1x.dz;
  i12 = DSIGTOL (s12, UT_TOL_min0);
  // (s12 > 0) = punkt ist links von p1-p2
  // (s12 < 0) = punkt ist rechts von p1-p2



  
  // normalvektor auf p2-p3
  vns.dx = v23.dy * vnt.dz - v23.dz * vnt.dy;
  vns.dy = v23.dz * vnt.dx - v23.dx * vnt.dz;
  vns.dz = v23.dx * vnt.dy - v23.dy * vnt.dx;
    // GR_tDyn_vc (&vns, p2, 12, 0);


  // vektor p2-px
  UT3D_vc_2pt (&v1x, p2, px);


  // skalarprodukt von nv x v1x
  s23 = vns.dx * v1x.dx + vns.dy * v1x.dy + vns.dz * v1x.dz;
  i23 = DSIGTOL (s23, UT_TOL_min0);
  // (s12 > 0) = punkt ist links von p1-p2
  // (s12 < 0) = punkt ist rechts von p1-p2
    // printf(" s12=%f s23=%f\n",s12,s23);
    // printf(" i12=%d i23=%d\n",i12,i23);


  if((i12 != 0) && (i23 != 0)) { // keines der Vorzeichen ist 0 ..
    if(i12 != i23) {
      // echt unterschiedlich - Punkt ist aussen.
      irc = 1;
      goto L_exit;
    }
  }


  // Beide Vorzeichen gleich, oder eine Seite ist 0;
  // 3. Seite ebenfalls testen.


  // normalvektor auf p3-p1
  UT3D_vc_2pt (&v31, p3, p1);   // vektor p3-p1
  vns.dx = v31.dy * vnt.dz - v31.dz * vnt.dy;
  vns.dy = v31.dz * vnt.dx - v31.dx * vnt.dz;
  vns.dz = v31.dx * vnt.dy - v31.dy * vnt.dx;
    // GR_tDyn_vc (&vns, p3, 12, 0);


  // vektor p3-px
  UT3D_vc_2pt (&v1x, p3, px);


  // skalarprodukt von nv12 x v1x
  s31 = vns.dx * v1x.dx + vns.dy * v1x.dy + vns.dz * v1x.dz;
  i31 = DSIGTOL (s31, UT_TOL_min0);
  // (s31 > 0) = punkt ist links von p3-p1
  // (s31 < 0) = punkt ist rechts von p3-p1
    // printf(" s31=%f\n",s31);


  if(i12 == 0) {
    // i23 und i31 muessen gleich sein ...
    if(i23 == i31) irc = -1;
    else irc = 1;


  } else if(i23 == 0) {
    // i12 und i31 muessen gleich sein ...
    if(i12 == i31) irc = -1;
    else irc = 1;


  } else {                   // wenn die beiden Vz gleich waren ..
    if(i31 == i23) irc = 0;  // wenn alle 3 gleich = innen.
    else irc = 1;
  }


  //----------------------------------------------------------------
  L_exit:
    // printf("ex UT3D_ck_pt_prj_tria %d\n",irc);
  return irc;

}


//================================================================
//================================================================
// Liste_TESTFUNKTIONEN:
//================================================================
//================================================================


// #ifdef DEB

/*

  Point2    pti={ -7.600000,   12.000000};
  Point2    p20={-10.000000,   12.000000};
  Point2    p21={-10.000000,   8.184550};
  Point2    p22={ 10.000000,   12.000000};

  GR_tDyn_ln2_2pt (&p20, &p21, 0);
  GR_tDyn_ln2_2pt (&p21, &p22, 0);
  GR_tDyn_ln2_2pt (&p22, &p20, 0);

  GR_Disp_pt2 (&pti, SYM_STAR_S, 2);

  GR_Disp_pt2 (&p20, SYM_TRI_S, 2);
  GR_Disp_pt2 (&p21, SYM_TRI_S, 2);
  GR_Disp_pt2 (&p22, SYM_TRI_S, 2);

  i1 = UT2D_ck_pt_in_tria__ (&p20, &p21, &p22, &pti);
  printf(" i1=%d\n",i1);
  return 0;
}


//================================================================
  int UT3D_tria_tst_load (Triangle *tri1, Triangle *tri2) {
//================================================================
// UT3D_tria_tst_load         load testTriangles
// ACHTUNG: Triangle besteht nur aus pointers; daher static !
// view 3Eck: GR_Disp_tria (&tri1, 9);
// Passender Punkt dazu: P20=P(17 14 4)

// Beispiel:
// Triangle  tri1, tri2;
// UT3D_tria_tst_load (&tri1,  &tri2);
// GR_Disp_tria (&tri1, 9);


// Variante: via APT - "A#=RCIR P1 p2 P3" - see UT3D_srbsp_tst_load

  static Point  pTab1[]={
    11.236,  -34.097,   -6.500,
     2.378,  -28.875,  -18.500,
     2.946,  -35.780,   -6.500
    // -40.617,    1.844,    1.742,
    // -40.043,    7.054,    1.742,
    // -37.390,    6.587,    3.809
    // 10,10,5,
    // 20,10,5,
    // 16,20,5
  };

  static Point  pTab2[]={
    // -43.751,    1.986,    0.443,
    // -40.043,    7.054,    1.742,
    // -40.617,    1.844,    1.742
    20,15,0,
     9, 9,0,
    15,20,0
  };

  // GR_tDyn_pcv (pTab1, 3, 9);
  // GR_tDyn_pcv (pTab2, 3, 9);

  UT3D_tria_pta (tri1, pTab1);
  UT3D_tria_pta (tri2, pTab2);

    // GR_Disp_tria (tri1, 9);
    // GR_Disp_tria (tri2, 9);

  return 0;

}
*/
// #endif


//======================= EOF ====================================
