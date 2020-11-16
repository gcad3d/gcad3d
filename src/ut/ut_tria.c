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
2020-07-07 ../ut/tri_orient.c integrated. RF.
2007-03-09 ReImplementiert aus UT3D_tria_orient

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

====================== 2D =====================================
UTRI_tria2_3pt2            create 2D-triangle from 3 2D-points

====================== 3D =====================================
UT3D_tria_pta              create triangle from 3 points
UTRI_tria_3pt              create triangle from 3 points
UT3D_tria_fac              create triangle from Fac3                   INLINE
UT3D_tria_box1             get all neighbour-triangels
UTRI_ntria_patch           get triangles from opengl-patch (pTab, pNr, pTyp)

UT3D_tria_inv              Umlaufsinn umdrehen

UTRI_triaNr_patch          get nr of triangles for [indexed-]opengl-patch

UT3D_indTria2ccw           3EckIndices CCW orientieren
UT3D_tria_orient           alle 3Ecke orientieren (Normalvektoren ausrichten)
UT3D_tria_ori_comm         check for Kanten direkt verbinden
UT3D_tria_ori_parl         naechste Kante mit Spalt suchen
UT3D_tria_ori_par_ck       check ob 2 Linien einen gemeinsamen Bereich haben
UT2D_orient_3pt            orientation of triangle (CW or CCW)
UT3D_ck_triaOrientV        get orientation from indices   INLINE
UT3D_ck_triaOrientI        get orientation from indices

UTRI_radi_3pt              inkreisradius
UTRI_rado_3pt              radius circumCircle        umkreisradius
UTRI_ptCC_3pt              center of circumCircle     UmkreisMittelpunkt
UTRI_ar_3pt                area of triangle         see UT2D_ar_3pt
UT3D_lenq_PtTria           minimum (quadr) distance Point-Triang
UT3D_lenq_PlnTria          minimum (quadr) distance Plane-Triang

UT2D_2pt_tngLnCiY          Beruehrung Kreis (pc auf horiz. Gerade) - LineSeg
UT3D_ck_pt_in_tria         check if point is inside Triang
UT3D_ck_pt_prj_tria        check if projection of point is inside Triang
UT2D_pt_gcp_3pt            gravity-centerPoint of triangle
UT3D_pt_gcp_3pt            gravity-centerPoint of triangle
UT3D_pt_gcp_tria           gravity-centerPoint of triangle
UT3D_pt_projPtTria         project point onto triangle
UTRI_pt_prjZpt3pt          project point to face along Z-vec
UT3D_pt_intTriaLnu         Point from intersect Triang - LineUnlimited

UT3D_vc_perpTria           vector = perpendic. to Triang (crossprod)

UTRI_ln_int_LnFac          intersect Line - Face
UT3D_ln_intTriaPln         Line = intersect Triang (Triang) - Plane
UT3D_ln_int_tria_pln       Line = intersect Triang (3 points) - Plane
UT3D_ln_intTriaTria        Line = intersect Triang - Triang
UT3D_2ln_tngTriPlSph       Kugel in Ebene an 3Eck abrollen

UT3D_pl1_tria              make Plane from Triang

TRI_edg_2i                 die kantennummer feststellen
TRI_edg_3i                 die kantennummer feststellen
UT3D_ind3Tria_2ind         give index of 3.point of triangle (0/1/2)  INLINE

UTRI_disp_tria_tS

UTRI_dump_tria_tS

List_functions_end:
=====================================================
- see also:

TSU_ntria_bMsh__             triangles from binary-mesh
statt UT3D_vc_perptria     use UT3D_vc_perp3pt      Normalvektor auf 3Eck
GR_Disp_triv GR_Disp_tria
TSU_ntria_bMsh__ TSU_ntria_bMsh_p UTRI_tria_3pt

../ut/ut_face.c   UFA   Funktionen fuer Faces (Fac3)
../ut/ut_msh.c    MSH   Funktionen fuer Meshes

- test functions:
UT3D_tria_tst_load         load testTriangs



//================================================================
Triang      {Point *pa[3];}           3D-triangle

Fac3          {int i1, i2, i3, st;}         indexed-triangle; 2D or 3D



//================================================================
UT3D_tria_orient  ist zu langsam;
  kann man die SurfaceNumbers als Info benutzen;
  beim UT3D_tria_ori_comm nur alle zur gleichen Surface gehoerigen 3Ecke testen


weave (join surfaces)
- die beiden Boundaries suchen (see UT3D_tria_ori_comm u. UT3D_tria_ori_parl)
- in UT3D_tria_ori_comm sollte auch der Index des connected Triang
  gespeichert werden; dazu braucht man pro 3Eck 3 Int's !
- ausgehend von den Boundaries die beiden dazu parallelen 3Ecks-Konturlinien
  suchen und verweben; erforderliche Mittelpunkte auf eine Konturlinie
  zwischen den beiden Boundaries.



---------------------------------------------------------------------
tStat:
 bit 1,2,4: ob die kante 0,1 od. 2 verbunden ist;
 bit 8:     ob alle 3 kanten verbunden sind (& 7)
 bit 16     der aktuelle Satz an 3Ecken
 bit 32     der komplette Satz noch offeren 3Ecke






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

#include "../gr/ut_gr.h"               // GR_tDyn_pcv








//================================================================
  int UTRI_tria2_3pt2 (Triang2 *triTab, int *triNr, int triSiz,
                       Point2 *p1, Point2 *p2, Point2 *p3) {
//================================================================
// add 1 triangle       als 3 Pointer --> triTab[*triNr];
// der Pointer auf den 1. Punkt zB ist        triTab[i1].pa[0]
// see also tess_ntri_tfac_add

  int    it;


  it = *triNr;

  // printf("UTRI_tria_3pt %d\n",it);
  // GR_tDyn_symB__ (p1, SYM_STAR_S, 2);
  // GR_tDyn_symB__ (p2, SYM_STAR_S, 2);
  // GR_tDyn_symB__ (p3, SYM_STAR_S, 2);


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



//================================================================
  int UTRI_tria_3pt (Triang *triTab, int *triNr, int triSiz,
                        Point *p1, Point *p2, Point *p3) {
//================================================================
// add 1 triangle       als 3 Pointer --> triTab[*triNr];
// der Pointer auf den 1. Punkt zB ist        triTab[i1].pa[0]
// see also tess_ntri_tfac_add

  int    it;


  it = *triNr;

  // printf("UTRI_tria_3pt %d\n",it);
  // GR_tDyn_symB__ (p1, SYM_STAR_S, 2);
  // GR_tDyn_symB__ (p2, SYM_STAR_S, 2);
  // GR_tDyn_symB__ (p3, SYM_STAR_S, 2);


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
  int UTRI_ntria_patch (Triang *tTab, int *tSiz,
                        Point *pTab, int pNr, int pTyp) {
//=======================================================================
// get triangles from opengl-patch (pTab, pNr, pTyp)
// Input:
//   tSiz   size of tTab
//   pTyp   patchTyp GL_TRIANGLE_STRIP|GL_TRIANGLE_FAN|GL_QUAD_STRIP
// Output:
//   tSiz   nr of Triangs created
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
                          Point *pti, Triang *trii) {             // in
//================================================================
// project point onto triangle / Normalabstand Triang-Point
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
    // GR_tDyn_symB__ (pti, SYM_STAR_S, 2);
    // GR_tDyn_symB__ (trii->pa[0], SYM_STAR_S, 3);


  // Vektoren laden
  UT3D_vc_2pt (&vab, trii->pa[0], trii->pa[1]);     // AB=0,1
  UT3D_vc_2pt (&vac, trii->pa[0], trii->pa[2]);     // AC=0,2
  UT3D_vc_2pt (&vap, trii->pa[0], pti);             // AP=0,pti


  // Normalvekttor auf das 3Eck
  UT3D_vc_perp2vc (&vc_abXac, &vab, &vac);
    // GR_tDyn_vc__ (&vc_abXac, trii->pa[0], 9, 0);


  t1 = UT3D_skp_2vc (&vc_abXac, &vc_abXac);
  if(t1 == 0.) return -1;


  t2 = UT3D_skp_2vc (&vap, &vc_abXac) / t1;


  pto->x = pti->x - (vc_abXac.dx * t2);
  pto->y = pti->y - (vc_abXac.dy * t2);
  pto->z = pti->z - (vc_abXac.dz * t2);


    // GR_tDyn_symB__ (pto, SYM_STAR_S, 3);

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


  // get Z-coord of point on 2D-Triang.
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
// UT3D_lenq_PtTria        minimum (quadr) distance Point-Triang

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
// UT3D_lenq_PlnTria          minimum (quadr) distance Plane-Triang

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
  int UT3D_tria_inv  (Triang *tr) {
//================================================================
// Umlaufsinn umdrehen; Index 0 und 1 vertauschen.

  // printf("UT3D_tria_inv \n");

  MEM_swap_2vp ((void**)&tr->pa[0], (void**)&tr->pa[1]);

  return 0;

}


//================================================================
  int UT3D_tria_pta (Triang *tr, Point *pa) {
//================================================================
// create triangle from 3 points

  tr->pa[0] = &pa[0];
  tr->pa[1] = &pa[1];
  tr->pa[2] = &pa[2];

  return 0;

}


//==================================================================
  int UT3D_pt_gcp_tria (Point *ps, Triang *tri) {
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
  int UT3D_pt_intTriaLnu (Point *ptx, Triang *tri, Point *ptl, Vector *vcl) {
//==============================================================================
// intersect Triang - LineUnlimited
// Retcod:
//   1 = NO  = point outside triangle
//   0 = YES = point inside triangle
//  -1 = ~~  = point is on a sideline


// Intersect Line-Plane, check if Point inside Triang ?
// UT3D_pt_intptvcpln
// UT3D_vc_perpTria

  Vector  vn;


  // normalVektor auf 3Eck UT3D_vc_perpTria
  UT3D_vc_perp3pt (&vn, tri->pa[0], tri->pa[1], tri->pa[2]);
    // GR_tDyn_vc__ (&vn, tri->pa[0], 3, 0);


  // Intersect Line-Plane
  UT3D_pt_intptvcpln (ptx, ptl, vcl, tri->pa[0], &vn);
    // GR_tDyn_symB__ (ptx, SYM_TRI_S, 2);

  // Intersect Line-Plane auch: UT3D_pt_intlnpl1 + UT3D_slen_ptpl


  // check if Point inside Triang
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
  int UT3D_ln_intTriaPln (Line *lni, Triang *tria, Plane *pln) {
//================================================================
// Line = intersect Triang - Plane
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
// Line = intersect Triang - Plane
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
    // GR_tDyn_vc__ (&pln->vz, &pln->po, 8, 0);


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
  int UT3D_ln_intTriaTria (Line *lni, Triang *tri1, Triang *tri2) {
//=====================================================================
// Line = intersect Triang - Triang
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
    // GR_tDyn_vc__ (&plt1.vz, &plt1.po, 8, 0);


  // make plane plt2 of tria2
  // vc1=p0-p1, vc2=p0-p2, vcn2=crossProd vc1-vc2
  UT3D_pl1_tria (&plt2, tri2);
    // DEB_dump_obj__(Typ_PLN, &plt2, " plt2");
    // GR_tDyn_vc__ (&plt2.vz, &plt2.po, 9, 0);


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
  int UT3D_pl1_tria (Plane *plt1, Triang *tri1) {
//================================================================
// make Plane from Triang
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
    // GR_tDyn_vc__ (&plt1->vz, tri1->pa[0], 5, 0);

  UT3D_pl_ptpl (plt1, tri1->pa[0]);  // setzt plane.p !!


  return 0;

}


/*
//================================================================
  int UT3D_pt_projPtTria (Point *pto, double *dd,                   // out
                          Triang *trii, Point *pti) {             // in
//================================================================
// project point onto triangle / Normalabstand Triang-Point
//  Retcod, ob der Punkt auf dem 3Eck liegt oder nicht.
//  Wenn ja: NormalAbstand und ProjektionsPunkt am Dreieck.
// RetCod:  0  pti is inside Triang
//          1  pti is NOT inside Triang

// UT3D_pt_projPtTria
// UT3D_pt_intPtVcTria

// see UT3D_ln_intTriaPln UT3D_ln_intTriaTria UT3D_pt_intptvcpl_ UT3D_pt_projptpl

  int   irc;
  Plane plt;


  // Normalvektor auf Triang
  UT3D_pl1_tria (&plt, trii);


  // intersect pti-Normalvektor mit Triang-plane
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
  // UTP_par1_vMin_vMax_vx (&d0, d1, d2, rc);
  d0 = UTP_par1_vMin_vMax_vx (d1, d2, rc);
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
                            Triang *tria, Plane *pls, double rs) {    // in
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
    // GR_tDyn_vc__ (&plt.vz, &plt.po, 9, 0);


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
    // GR_tDyn_symB__ (&pTab[0], SYM_STAR_S, 3);
    // GR_tDyn_symB__ (&pTab[1], SYM_STAR_S, 3);
    // GR_tDyn_symB__ (&pTab[2], SYM_STAR_S, 3);
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
      // GR_tDyn_symB__ (&pTab[i1], SYM_STAR_S, 4);
      // GR_tDyn_symB__ (&pTab[i2], SYM_STAR_S, 4);

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
      // GR_tDyn_vc__ (&vx, &plt.po, 9, 0);
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
  // i1 = UTP_par1_vMin_vMax_vx (&pp1, dTab[imin], dTab[imax], rs);
  pp1 = UTP_par1_vMin_vMax_vx (dTab[imin], dTab[imax], rs);
    // printf(" pp1=%f\n",pp1);  // muss zwischen 0 - 1 sein ..


  // den ersten Punkt auf der strecke imin-imax errechnen;
  // und zwar am 3Eck
  UT3D_pt_evpar2pt (&lnt->p1, pp1, tria->pa[imin], tria->pa[imax]);
    // GR_tDyn_symB__ (&lnt->p1, SYM_TRI_S, 4);

  // und in der Plane
  UT3D_pt_evpar2pt (&lnp->p1, pp1, &pTab[imin], &pTab[imax]);
    // GR_tDyn_symB__ (&lnp->p1, SYM_TRI_S, 5);


  // nun muss die 2. Strecke bestimmt werden;
  // der Startpunkt ist der dritte, fehlende Punkt 
  i2s = 3 - imin - imax;
    // printf(" i2s=%d\n",i2s);


  // endpunkt i2e: rs muss zwischen i2s und i2e liegen;
  // die Abstansparemter von den beiden Punkten rechnen; jenen
  // parameter, der naeher an der Mitte liegt auswaehlen.
  // i1 = UTP_par1_vMin_vMax_vx (&pp1, dTab[i2s], dTab[imin], rs);
  pp1 = UTP_par1_vMin_vMax_vx (dTab[i2s], dTab[imin], rs);
  // i2 = UTP_par1_vMin_vMax_vx (&pp2, dTab[i2s], dTab[imax], rs);
  pp2 = UTP_par1_vMin_vMax_vx (dTab[i2s], dTab[imax], rs);
  // if(i1 < 0) {
  if(pp1 == 0.) {
    goto L_imax;
  // } else if(i2 < 0) {
  } else if(pp2 == 0.) {
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
    // GR_tDyn_symB__ (&lnt->p2, SYM_STAR_S, 4);

  // und in der Plane
  UT3D_pt_evpar2pt (&lnp->p2, ppe, &pTab[i2s], &pTab[i2e]);
    // GR_tDyn_symB__ (&lnp->p2, SYM_STAR_S, 4);


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
// UT2D_ck_pt_in_tria__   check if point is inside Triang
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
// UT3D_ck_pt_in_tria check if point is inside Triang
// all 4 points must be in the same Plane !


  int      bpi;
  Point2   p21, p22, p23, p2x;
  Point    pb1, pb2;
  Vector   vc1;


    // GR_tDyn_symB__ (px, SYM_STAR_S, 4);
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
    // GR_tDyn_vc__ (&vc1, &pb1, 12, 1); 
  bpi = UT3D_bp_perp_vc (NULL, &vc1);               // backplane from vector
    // printf(" bpi=%d\n",bpi);


  // make 2D-points
  UT2D_pt_tra_pt3_bp (&p21, p1, bpi);
  UT2D_pt_tra_pt3_bp (&p22, p2, bpi);
  UT2D_pt_tra_pt3_bp (&p23, p3, bpi);
  UT2D_pt_tra_pt3_bp (&p2x, px, bpi);


  // 2D-check if point is inside Triang
  return UT2D_ck_pt_in_tria__ (&p21, &p22, &p23, &p2x);

}


//=========================================================================
  int UT3D_ck_pt_prj_tria (Point *p1, Point *p2, Point *p3, Point *px) {
//=========================================================================
// UT3D_ck_pt_prj_tria          check if projection of point is inside Triang
// px must not be in triangle-plane; projection is made.
// see UT2D_ck_pt_in_tria__
// Returncodes:
//   1 = NO  = point outside triangle
//   0 = YES = point inside triangle
//  -1 = ~~  = point is on a sideline



  int       i12, i23, i31, irc;
  double    s12, s23, s31;
  Vector    v12, v23, v31, v1x, vnt, vns;



    // GR_tDyn_symB__ (px, SYM_STAR_S, 4);
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
    // GR_tDyn_vc__ (&vnt, p1, 12, 0); 



  // normalvektor auf p1-p2
  vns.dx = v12.dy * vnt.dz - v12.dz * vnt.dy;
  vns.dy = v12.dz * vnt.dx - v12.dx * vnt.dz;
  vns.dz = v12.dx * vnt.dy - v12.dy * vnt.dx;
    // GR_tDyn_vc__ (&vns, p1, 12, 0); 


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
    // GR_tDyn_vc__ (&vns, p2, 12, 0);


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
    // GR_tDyn_vc__ (&vns, p3, 12, 0);


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
  int UT3D_tria_box1 (int *boxTab, int boxSiz,
                      Triang *ta, int triNr, char *tStat,
                      int iRes, int iSta) {
//================================================================
// get all neighbour-triangels 
// alle dem tri0 benachbarten 3Ecke in eine Liste holen
// nicht 3Ecke mit tStat > iRes
//  iRes = 6   nur 3Ecke (fertig resolved)
//  iRes = 0   nur alle nicht aufgeloesten ..


  int      i1, i2, boxNr;
  Point    pb1, pb2;
  Triang *tri1;


  // printf("UT3D_tria_box1 %d %d\n",iRes,iSta);



  // Eine Box um 3Eck it.
  UT3D_box_tria (&pb1, &pb2, &ta[iSta], UT_DISP_cv);
    // GR_tDyn_box__(&pb1, &pb2, 8);




  //----------------------------------------------------------------
  // loop tru all triangs; fill boxTab with trias close to tria0
  boxNr = 0;
  for(i1=0; i1<triNr; ++i1) {
    if((tStat[i1] & 32) == 0) continue;      // 32 muss gesetzt sein ..
    if((tStat[i1] & 7) > iRes) continue;     // 1 + 2 + 4

    tri1 = &ta[i1];

    // check if one of the points of i1 is inside box ...
    i2 = UT3D_ck_ptInBox (&pb1, &pb2, tri1->pa[0]);   // 0=yes,1=no.
    if(i2 == 0) goto LN_1;
    i2 = UT3D_ck_ptInBox (&pb1, &pb2, tri1->pa[1]);   // 0=yes,1=no.
    if(i2 == 0) goto LN_1;
    i2 = UT3D_ck_ptInBox (&pb1, &pb2, tri1->pa[2]);   // 0=yes,1=no.
    if(i2 == 0) goto LN_1;
    continue;

    // is in box; add to boxTab.
    LN_1:
    // printf(" LN_1 %d\n",i1);
    boxTab[boxNr] = i1;
    ++boxNr;
    if(boxNr >= boxSiz) break;
  }


  // for(i1=0; i1<boxNr; ++i1) GR_Disp_tria (&ta[boxTab[i1]], 8);

  // printf("ex UT3D_tria_box1 %d\n",boxNr);

  return boxNr;

}


//================================================================
  int TRI_edg_2i (int ip0, int ip1) {
//================================================================
// die kantennummer feststellen;
// RetCod:
//   1 = Kante 0-1  12 21
//   2 = Kante 1-2  24 42
//   4 = Kante 2-0  14 41


  int    irc;

  irc = 0;

  if(ip0 > ip1) MEM_swap_int (&ip0, &ip1);
  // printf(" nach swap %d %d\n",ip0,ip1);

  if(ip0 == 1) {
    if(ip1 == 2) irc = 1;
    else irc = 4;
  } else irc = 2;


    // printf("ex TRI_edg_2i %d    %d %d\n",irc,ip0,ip1);

  return irc;

}


//================================================================
  int TRI_edg_3i (int ip0, int ip1, int ip2) {
//================================================================
// die kantennummer feststellen;
// RetCod:
//   1 = Kante 0-1
//   2 = Kante 1-2
//   4 = Kante 2-0


  int    irc;



  irc = 0;


  if(ip0<1) goto LBC_12;
  if(ip1<1) goto LBC_02;
  if(ip2<1) goto LBC_01;


  LBC_01:  // CCW
    if((ip0<1)||(ip1<1)) goto L_exit;
    irc = 1;
    goto L_exit;

  LBC_02:   // CW
    if((ip0<1)||(ip2<1)) goto L_exit;
    irc = 4;
    goto L_exit;

  LBC_12:  // CCW
    if((ip1<1)||(ip2<1)) goto  L_exit;
    irc = 2;
    goto L_exit;



  L_exit:

    // printf("ex TRI_edg_3pt %d    %d %d %d\n",irc,ip0,ip1,ip2);

  return irc;

}


//================================================================
  int UTRI_dump_tria_tS (char *tStat, int triNr) {
//================================================================

  int     i1;


  printf("UTRI_dump_tria_tS %d\n",triNr);

  for(i1=0; i1<triNr; ++i1) printf(" %d=%d,",i1,tStat[i1]);
  printf(" \n");

  return 0;

}


//================================================================
  int UTRI_disp_tria_tS (Triang *ta, char *tStat, int triNr) {
//================================================================

  int       i1, i2;
  Triang  *tri0;


  printf("================ UTRI_disp_tria_tS ================\n");

    for(i1=0; i1<triNr; ++i1) if(tStat[i1] > 7) printf(" %d=%d,",i1,tStat[i1]);
    printf(" \n");

    // die Kanten darstellen ..
    for(i1=0; i1<triNr; ++i1) {
      if(tStat[i1] < 8) continue;
      tri0 = &ta[i1];

      // i1 = 40; // TEST-ONLY
      i2 = tStat[i1];

      // kante 0-1
      if((i2 & 1) == 0) GR_tDyn_ln_2pt (tri0->pa[0], tri0->pa[1], 11);
      if((i2 & 2) == 0) GR_tDyn_ln_2pt (tri0->pa[1], tri0->pa[2], 11);
      if((i2 & 4) == 0) GR_tDyn_ln_2pt (tri0->pa[2], tri0->pa[0], 11);
    }

  return 0;

}


//======================================================================
  int UT3D_tria_orient (Triang *ta, int triNr, Point *ptFar) {
//======================================================================
// alle 3Ecke orientieren (alle Normalvektoren nach innen ausrichten)
// ACHTUNG: benutzt fast alle Speicher aus xa_mem.h !

// Input:
//   ta       TriangArray,
//   triNr    Anzahl 3Ecke
//   ptFar    ein Punkt weit aussen

//  Eine StatusListe aller 3Ecke; 0=undefined; 1=fixed.
//  das erste 3Eck suchen und bestimmen;
//  alle daran anschliessenden 3Ecke suchen und bestimmen.
// OFFEN: statt UT3D_tria_oriCkParl  eine Funktion, die das gleiche gleich
//        fuer ein 3Eck macht; damit kann man eine Menge Zeit sparen ....
//        Man kann das ganze auf 2D umwandeln, wenn man es in Richtung
//        des Vektors sieht !


  int       i1, it, ip;
  double    d1, d2;
  char      *tStat;
  Vector    vc1, vc2;
  Triang  *tri0, *tri1;


  // printf("OOOOOOOOOOOOOOOOOO UT3D_tria_orient OOOOOOOOOOOOOOOO \n");
  // DEB_dump_obj__ (Typ_PT, ptFar, "UT3D_tria_orient");




  //----------------------------------------------------------------
  // Eine StatusListe aller 3Ecke; 0=undefined; 1=fixed.
  tStat = memspc51;
  if(sizeof(memspc51) < triNr) {TX_Error("UT3D_tria_orient E001"); return -1;}
  for(i1=0; i1<triNr; ++i1) tStat[i1] = 0;



  //----------------------------------------------------------------
  // das erste 3Eck suchen. Den naechsten Punkt zu ptFar suchen.
  // OFFEN:
  // Wenn das 3Eck jedoch mit ptFar in der gleichen Ebene liegt,
  // muesste man das 3Eck skippen !
  // Eigentlich sollte das 3Eck mit dem geringesten Abstand gesucht werden !!
  d2 = UT_VAL_MAX;
  for(i1=0; i1<triNr; ++i1) {
    d1 = UT3D_lenq_PtPt (ta[i1].pa[0], ptFar);
    if(d1 < d2) {ip = 0; goto L1_take;}
    d1 = UT3D_lenq_PtPt (ta[i1].pa[1], ptFar);
    if(d1 < d2) {ip = 1; goto L1_take;}
    d1 = UT3D_lenq_PtPt (ta[i1].pa[2], ptFar);
    if(d1 > d2) continue;
    // take this ...
    ip = 2;
    L1_take:
    it = i1;
    d2 = d1;
  }
    printf(" Start: it=%d ip=%d\n",it,ip);


  // das gefundene Dreieck so orientieren, dass ptFar aussen liegt.
  tri1 = &ta[it];
  // vc1 = Normalektor aufs 3Eck
  UT3D_vc_perp3pt (&vc1, tri1->pa[0],tri1->pa[1],tri1->pa[2]);
  // vc2 = Vector vom ptFar zum Punkt am 3Eck
  UT3D_vc_2pt (&vc2, ptFar, tri1->pa[ip]);
  // vc1 und vc2 muessen gleiche Richtung haben, else 3Eck umdrehen.
  if(UT3D_sid_2vc(&vc1,&vc2) < 0) UT3D_tria_inv (tri1);


    // TEST-ONLY-BLOCK:
    // GR_Disp_tria (tri1, 9);
    // GR_Disp_triv (tri1, 9, it);
    // return -1;




  //----------------------------------------------------------------
  // Alle Kanten direkt verbinden,
  // dabei speichern, wieviel Kanten verbunden wurden, (max 3 = komplett)
  L_nxt:
  UT3D_tria_ori_comm (ta, tStat, triNr, it);
    // UI_wait_Esc ();



  //----------------------------------------------------------------
  // die naechste zu verbindende Kante suchen (mit Spalt ..)
  // Dazu die laengste Aussenkante suchen, alle benachbarten 3Ecke
  // zu dieser Kante suchen, dann eine Nachbarkante suchen.
  // Achtung: alle 3 Kanten untersuchen; es koennen von einem (sehr
  // schmalen 3Eck) 2 Kanten benachbart sein; es muss die naehere Kante
  // benutzt werden !!!!
  it = UT3D_tria_ori_parl (ta, tStat, triNr);
    // UI_wait_Esc ();




  // dann wieder alle direkten Kanten suchen usw.
  // if(it == 148) return 0; // TEST-ONLY
  if(it >= 0) goto L_nxt;




  return 0;

}


//========================================================================
  int UT3D_tria_ori_comm (Triang *ta, char *tStat, int triNr, int iSta) {
//========================================================================


  int       irc, i1, i2, i3, it, ib, ib0, ibi, boxTab[100], boxNr, iLast;
  int       iBox, iNr, istat1, istat2, ick, itc;
  int       ipb, ipb0, ipb1, ipb2, ipt, ipt0, ipt1, ipt2;
  double    d1, d2;
  Point     pt1, *pp1, *pp2, pb1, pb2;
  Vector    vc1, vc2;
  Triang  *tri0, *tri1;


  // printf("================ UT3D_tria_ori_comm it=%d ================\n",iSta);


  it = iSta;
  // it = 523;   // TEST-ONLY
  itc = -1;

  iNr = 0;
  ick = 0;
  istat2 = 0;


  // tStat aller noch offenen 3Ecke auf +32 setzen
  for(i1=0; i1<triNr; ++i1) if(tStat[i1] == 0) tStat[i1] = 32;





  //----------------------------------------------------------------
  // Vom 3Eck [it] ist die Orientierung richtig und bekannt.
  // Eine an dieses 3Eck anschliessende Kante suchen, dessen 3Eck orientieren
  // und als korrekt orientiert markieren.
  LN_0:
    // printf("LN_0:  it=%d iNr=%d\n",it,iNr);  // Anz bearb.3Ecke
  if(it < 0) goto L_fertig;
  istat1 = 0;
  // zu 3Eck tri0 ein Anschluss3Eck suchen ..
    // GR_Disp_triv (&ta[it], 9, it);




  //----------------------------------------------------------------
  // loop tru all triangs; fill boxTab with trias close to tria0
  boxNr = UT3D_tria_box1 (boxTab, 100, ta, triNr, tStat, 6, it);

  for(i1=0; i1<boxNr; ++i1) {
    i2 = boxTab[i1];
    if(i2 < 0) continue;
    tStat[i2] |= 16;
  }


  if(boxNr < 1) {
    // es kommt vor, dass bereits alle Nachbar3Ecke gefunden sind ..
    // goto LN_weiter;
    // goto L_fertig;
    // tStat[it] |= 8;   // cannot resolv here ..
      // printf(" set|8 tStat[%d] = %d\n",it,tStat[it]);
    goto L_ICK3;
    // TX_Print("UT3D_tria_orient E002");
    // GR_Disp_tria (tri0, 9);
    // return -1;
  }


    // TEST-ONLY:
    // printf("boxTab: %d",boxNr);
    // for(i1=0; i1<boxNr; ++i1) printf(" %d,",boxTab[i1]);
    // printf("\n");






  //----------------------------------------------------------------
  // nun ist boxTab mit zu testenden 3Ecken gfuellt.
  // zuerst nach gemeinsamer Kante suchen;
  // wenns keine gibt: nach  paralleler Linie suchen ...
  LBC__:
  // printf("================ LBC__ it=%d ================\n",it);
  iLast = -1;
  tri0 = &ta[it];
  if(it < 0)  goto L_ICK3;


  for(iBox=0; iBox<boxNr; ++iBox) {
    ib = boxTab[iBox];
    if(ib < 0) continue;
    if(ib == it) continue;

    if((tStat[ib] & 7) > 6) continue;  // alle 3 Kanten gefunden ..

    tri1 = &ta[ib];

    // check if edge is already connected ...

    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    // test for common line
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    // printf("ccccccccc ck %d-%d iBox=%d\n",it,ib,iBox);
    // check if tri0.p1 fits to one of the points of tri1
    LBC_0:
    pp1 = tri0->pa[0];          // check p0 of it
    ipt0 = 1;
    if(UT3D_comp2pt(pp1, tri1->pa[0], UT_TOL_pt) == 1)
      {ipb0 = 1; goto LBC_1;}
    if(UT3D_comp2pt(pp1, tri1->pa[1], UT_TOL_pt) == 1)
      {ipb0 = 2; goto LBC_1;}
    if(UT3D_comp2pt(pp1, tri1->pa[2], UT_TOL_pt) == 1)
      {ipb0 = 4; goto LBC_1;}
    ipt0 = 0;
    ipb0 = 0;

    LBC_1:
    pp1 = tri0->pa[1];          // check p1 of it
    ipt1 = 2;
    if(UT3D_comp2pt(pp1, tri1->pa[0], UT_TOL_pt) == 1)
      {ipb1 = 1; goto LBC_2;}
    if(UT3D_comp2pt(pp1, tri1->pa[1], UT_TOL_pt) == 1)
      {ipb1 = 2; goto LBC_2;}
    if(UT3D_comp2pt(pp1, tri1->pa[2], UT_TOL_pt) == 1)
      {ipb1 = 4; goto LBC_2;}
    ipt1 = 0;
    ipb1 = 0;


    LBC_2:
    // 2 Punkte frei; damit kann es keine common line geben !
    if((ipb0<1)&&(ipb1<1)) continue; //goto LN_Next;

    pp1 = tri0->pa[2];          // check p2 of it
    ipt2 = 4;
    if(UT3D_comp2pt(pp1, tri1->pa[0], UT_TOL_pt) == 1)
      {ipb2 = 1; goto LBC_3;}
    if(UT3D_comp2pt(pp1, tri1->pa[1], UT_TOL_pt) == 1)
      {ipb2 = 2; goto LBC_3;}
    if(UT3D_comp2pt(pp1, tri1->pa[2], UT_TOL_pt) == 1)
      {ipb2 = 4; goto LBC_3;}
    ipt2 = 0;
    ipb2 = 0;

    // 2 von 3 Indices sollten belegt sein;
    LBC_3:
    if(ipb0<1) goto LBC_12;
    if(ipb1<1) goto LBC_02;
    if(ipb2<1) goto LBC_01;

    // die beiden Dreiecke muessen ungleiche Drehrichtung haben;
    // CCW ist 01 12 20;      CW ist 02 21 10
    LBC_01:  // CCW
    if((ipb0<1)||(ipb1<1))  continue; //goto LN_Next;
      ipb = TRI_edg_2i (ipb0, ipb1);
      // printf(" LBC_01: %d %d\n",ipb0,ipb1);
      i2 = UT3D_ck_triaOrientV (ipb0,ipb1);
      if(i2 < 0) goto LB_found_g;
      goto LB_found_v;

    LBC_02:   // CW
    if((ipb0<1)||(ipb2<1))  continue; //goto LN_Next;
      ipb = TRI_edg_2i (ipb0, ipb2);
      // printf(" LBC_02: %d %d\n",ipb0,ipb2);
      i2 = UT3D_ck_triaOrientV (ipb0,ipb2);
      if(i2 > 0) goto LB_found_g;
      goto LB_found_v;

    LBC_12:  // CCW
    if((ipb1<1)||(ipb2<1))  continue; //goto LN_Next;
      ipb = TRI_edg_2i (ipb1, ipb2);
      // printf(" LBC_12: %d %d\n",ipb1,ipb2);
      i2 = UT3D_ck_triaOrientV (ipb1,ipb2);
      if(i2 < 0) goto LB_found_g;
      goto LB_found_v;



    //------------------------------------------------
    // tri1 grenzt an tri0
    LB_found_v:   // gefunden, verkehrt
    //UT3D_tria_inv (tri1);
      // printf(" invert %d\n",ib);
    MEM_swap_2vp ((void**)&tri1->pa[0], (void**)&tri1->pa[1]);
    // auch ipb vertauschen
    i1 = ipb;
    if(i1 == 2) ipb = 4;
    else if(i1 == 4) ipb = 2;
    LB_found_g:   // gefunden, gleich
      // TEST_ONLY
      // GR_Disp_triv (tri1, 9, -1);
      // DL_Redraw (); OS_Wait (200);
    // bei beiden 3Ecken Status korrigieren
    ipt = TRI_edg_3i (ipt0, ipt1, ipt2);
    if((tStat[it] & ipt) != ipt) {
      tStat[it] |= ipt + 8;
      ++istat1;
      ++istat2;
    }
    if((tStat[ib] & ipb) != ipb) {
      tStat[ib] |= ipb + 8;
      ++istat1;
      ++istat2;
        // GR_Disp_triv (&ta[ib], 9, ib);
      iLast = ib;
    }

      // printf("   ipt0=%d ipt1=%d ipt2=%d\n",ipt0,ipt1,ipt2);
      // printf("   ipb0=%d ipb1=%d ipb2=%d\n",ipb0,ipb1,ipb2);
      // printf(" >>>>>>>>>>>>>> conn %d - %d  (%d %d) %d %d\n",it,ib,
             // tStat[it],tStat[ib],ipt,ipb);
  }


  // wenn keine Verbindung mit einer neuen Box, Box nicht weiter testen ..
  // (else Loop)
  if((ick == 3)&&(iLast < 0)) goto L_ICK3;
  // if((ick > 1)&&(iLast < 0)) goto L_ICK3;
  // if(iLast < 0) goto L_ICK2;
  




  //----------------------------------------------------------------
  // zuletzt wurden it-iLast verbunden; wenn iLast noch nicht komplett
  // aufgeloest: nach iLast suchen (noch mit der alten iBox)
  L_ICK1:
/*
  printf("ICK1: ick=%d iLast=%d %d\n",ick,iLast,tStat[iLast]);
  ick = 1;
  i1 = tStat[iLast];
  i2 = (i1 & 1) + ((i1 & 2) ? 1 : 0) + ((i1 & 4) ? 1 : 0);
  if((i2 > 0)&&(i2 < 3)) {
    it = iLast;
    goto LBC__;
  }
*/



  //----------------------------------------------------------------
  // weitere offene in der aktiven iBox suchen ...
  L_ICK2:

  // printf("ICK2 ick=%d ibi=%d\n",ick,ib0);
  // wenn vorher auch ICK2 war, boxTab-Record loeschen
  if(ick != 2) ib0 = -1;
  ++ib0;
  ick = 2;
  if(ib0 >= boxNr) goto L_ICK3;
  for(ibi=ib0; ibi<boxNr; ++ibi) {
    it = boxTab[ibi];
    if(it < 0) continue;
    i1 = tStat[it] & 7;
    if(i1 < 1) continue;     // noch keine Verbindung
    if(i1 > 6) continue;     // fertig aufgeloest
    // i2 = (i1 & 1) + ((i1 & 2) ? 1 : 0) + ((i1 & 4) ? 1 : 0);
    // if(i2 < 3) goto LBC__;
    goto LBC__;
  }
  // if(istat1 > 0) {istat1 = 0; ib0 = -1; goto L_ICK2;}





  //----------------------------------------------------------------
  L_ICK3:
  // printf("ICK3 %d\n",ick);
  // wenn vorher auch ICK3 war, Record loeschen
  // if((ick == 3)&&(iLast < 0)) tStat[it] |= 8;  // cannot resolv with this iBox
  ick = 3;
  L_nxt_itc:
  ++itc;
  if(itc >= triNr) {
    if(istat2 > 0) {
      itc = -1;
      istat2 = -1;
      goto L_nxt_itc;
    } else {
      goto L_fertig;
    }
  }

  // for(it=0; it<triNr; ++it) {
    it = itc;
    i1 = tStat[it];
    if((i1 & 48) != 48) goto L_nxt_itc;   // nur aus dem aktuellen Satz (16 + 32)
    // if(i1 < 17) goto L_nxt_itc;          // noch keine Verbindung
    i2 = i1 & 7;
    if(i2  < 1) goto L_nxt_itc;           // noch keine Verbindung
    if(i2  > 6) goto L_nxt_itc;           // fertig aufgeloest
    // i2 = (i1 & 1) + ((i1 & 2) ? 1 : 0) + ((i1 & 4) ? 1 : 0);
    // if(i2 < 3) goto LN_0;
  // }
  // Loop bis nix mehr aufgeloest wird ..
    goto LN_0;



  //----------------------------------------------------------------
  L_fertig:

  // Reparatur: bei allen tStat mit &7 == 7 das openBoundary-Bit weg
  // das passiert oft ..
  for(i1=0; i1<triNr; ++i1) {
    tStat[i1] &= ~48;                             // 16 u. 32 weg
    if((tStat[i1] & 7) == 7) tStat[i1] &= ~8;
  }


  // UTRI_dump_tria_tS (tStat, triNr);
  // UTRI_disp_tria_tS (ta, tStat, triNr);

  return 0;

}




//========================================================================
  int UT3D_tria_ori_parl (Triang *ta, char *tStat, int triNr) {
//========================================================================
// ein benachbartes Feld mit status 0 suchen ...
// alle Felder mit Boundary haben bit mit val=8 gesetzt.

  int       irc, i1, i2, i3, ipe, boxTab[20], boxNr;
  int       ieNr, iDir;
  // int       ie01, ie12, ie20;
  // int       id01, id12, id20;
  double    d1, d2, d3;
  Point     pt1, pt2;
  Point     *pp1, *pp2;
  Triang  *tri0, *tri1;



  // printf("================ UT3D_tria_ori_parl ================\n");
  // UTRI_disp_tria_tS (ta, tStat, triNr);





  for(i1=0; i1<triNr; ++i1) {
    // next 3Eck mit offener Boundary suchen
    i2 = tStat[i1] & 7;
    if(i2 < 1) continue;
    if(i2 > 6) continue;
      // printf(" ck-i1 %d %d\n",i1,tStat[i1]);


/*
    // alle dem tri0 benachbarten 3Ecke in eine Liste holen
    boxNr = UT3D_tria_box1 (boxTab, 20, ta, triNr, tStat, 0, i1);
    if(boxNr < 1) continue;
      for(i2=0; i2<boxNr; ++i2) GR_Disp_tria (&ta[boxTab[i2]], 8);
*/

    // welche Kante ist vom 3Eck i1 offen ?
    tri0 = &ta[i1];

    L_CKE0:   // Kante 0-1
    ipe = 0; 
    if((tStat[i1] & 1) == 0) {
      pp1 = tri0->pa[0];
      pp2 = tri0->pa[1];
      goto L_CK__;
    }

    L_CKE1:   // Kante 1-2
    ipe = 1;
    if((tStat[i1] & 2) == 0) {
      pp1 = tri0->pa[1];
      pp2 = tri0->pa[2];
      goto L_CK__;
    }

    L_CKE2:   // Kante 2-0
    ipe = 2;
    if((tStat[i1] & 4) == 0) {
      pp1 = tri0->pa[2];
      pp2 = tri0->pa[0];
      goto L_CK__;
    }

    // keine Kante offen;
    continue;





    // kante mit allen ganz-offenen 3Ecken testen ...
    // wenn 2 Punkte nicht in der kantenZone, kein weiterer Test ...


    L_CK__:
    for(i2=0; i2<triNr; ++i2) {
      if(tStat[i2] > 0) continue;   // nur ganz freie 3Ecke testen
        // printf(" ck-i1=%d (%d) i2=%d ipe=%d\n",i1,tStat[i1],i2,ipe);

      // tri1 = &ta[i2];

      irc = UT3D_tria_ori_par_ck (&ieNr, &iDir, pp1, pp2, &ta[i2]);
      if(irc == 0) goto L_found;

    }

    if(ipe == 0) goto L_CKE1;
    if(ipe == 1) goto L_CKE2;

  }
  i2 = -1;
  goto L_done;



  L_found:
  // 3Eck i2 ist direkter Nachbar von 3Eck i1;
  // testen ob mehr als eine Kante benachbart ist
  if(iDir > 0)  UT3D_tria_inv (&ta[i2]);



  L_done:

    // TEST-ONLY
    // printf("ex UT3D_tria_ori_parl %d dreh=%d\n",i2,iDir);
    // if(i2 >= 0) GR_Disp_triv (&ta[i2], 11, i2);

  return i2;

}


//================================================================
  int UT3D_tria_ori_par_ck (int *ieNr, int *iDir,
                            Point *pa, Point *pb, Triang *tr) {
//================================================================
// check ob 2 Linien einen gemeinsamen Bereich haben
//  (verwendet zum Orientieren eines 3EcksNetzes, UT3D_tria_orient)
// Kriterien:
//- die Punkte duerfen nicht weiter als Toleranz von der Basisline entfernt sein
//- die Lines sollen parallel sein mit max 30 Grad Divergenz
//- die Lines sollen sich teilweise ueberdecken
//    (also nicht nur einen Punkt gemeinsam haben ...)
//    es muss also einer der beiden Punkte der Line2 innerhalb der Line1 sein
//- Weiters braucht man auch die Richtung der Linien;
//    (gleiche Richtung: 3Eck umdrehen, else ok). iDir.
//
// Input:   4 Punkte;
// Output:
//   Retcod   0  - Ok; Linien sind parallel & haben eine gemeinsame Strecke
//           -1  - No; Linien sind unterschiedlich
//            1  - Alle 3 Punkte sind in der gemeinsamen Zone; wahrscheinl.Mist
//   iDir     Richtung parallel oder antiparallel

// see also UT3D_tria_oriCkParl UT3D_nlenq_3pt

//                        d          
//                       / .  .
//                      /  .     e
//                     /   . .   .
//                   c/ .  .     .
//                    .    .     .
//                    .    .     .
//                    .    .     .
//          a---------f----g-----h-----------------b
//


  int       ii, ip1, ip2;
  double    distq, dqc, dqd, dqe;
  double    s_ab_ab, s_ab_ac, s_ab_ad, s_ab_ae, pf_ab, pg_ab, ph_ab;
  double    p_p1, p_p2, d1, d2;
  Point     *pc, *pd, *pe;
  Vector    vab, vac, vad, vae, vaf, vag, vah, vfc, vgd, vhe;


  // DEB_dump_obj__ (Typ_PT, pa, " pa=");
  // DEB_dump_obj__ (Typ_PT, pb, " pb=");



  // den Normalabstand der Punkte c, d, und e von der Linie a-b testen;
  // wenn bei 2 Punkten zu gross: exit
  distq = UT_DISP_cv * UT_DISP_cv;

  pc = tr->pa[0];
  pd = tr->pa[1];
    // DEB_dump_obj__ (Typ_PT, pc, " pc=");
    // DEB_dump_obj__ (Typ_PT, pd, " pd=");



  UT3D_vc_2pt (&vab, pa, pb);           // vab = Vector a-b
  UT3D_vc_2pt (&vac, pa, pc);           // vac = Vector a-c
  UT3D_vc_2pt (&vad, pa, pd);           // vad = Vector a-d

  s_ab_ab = UT3D_skp_2vc (&vab, &vab);  // skp mit sich selbst = Laenge^2
  s_ab_ac = UT3D_skp_2vc (&vab, &vac);  // gibt Wert fuer f relativ zu s_ab_ab
  s_ab_ad = UT3D_skp_2vc (&vab, &vad);  // gibt Wert fuer g relativ zu s_ab_ab
    // printf(" s_ab_ab=%f s_ab_ac=%f s_ab_ad=%f\n",s_ab_ab,s_ab_ac,s_ab_ad);

  // Parameterwert von e zwischen a-b
  pf_ab = s_ab_ac / s_ab_ab;
  pg_ab = s_ab_ad / s_ab_ab;
    // printf(" pf_ab=%f pg_ab=%f\n",pf_ab,pg_ab);

  // Multip. des Vektors a-b mit Parameterwert von e ergibt den Vektor a-e
  UT3D_vc_multvc (&vaf, &vab, pf_ab);
  UT3D_vc_multvc (&vag, &vab, pg_ab);

  // Subtraktion (Vec-a-c - Vec-a-e) = Vec-e-c
  UT3D_vc_sub2vc (&vfc, &vac, &vaf);
  UT3D_vc_sub2vc (&vgd, &vad, &vag);

  dqc = UT3D_skp_2vc (&vfc, &vfc);
  dqd = UT3D_skp_2vc (&vgd, &vgd);
    // printf(" distq=%f dqc=%f dqd=%f\n",distq,dqc,dqd);

  if(dqc > distq) {
    if(dqd > distq) return -1;
    // Pt.c is far; Pt.d is near;
    ii = 1;
  } else {
    if(dqd > distq) ii = 1;   // Pt.c is near; Pt.d is far;
    else            ii = 2;   // Pt.c is near; Pt.d is near;
  }


  pe = tr->pa[2];
  UT3D_vc_2pt (&vae, pa, pe);           // vae = Vector a-e
  s_ab_ae = UT3D_skp_2vc (&vab, &vae);  // gibt Wert fuer e relativ zu s_ab_ab
  ph_ab = s_ab_ae / s_ab_ab;
  UT3D_vc_multvc (&vah, &vab, ph_ab);
  UT3D_vc_sub2vc (&vhe, &vae, &vah);
  dqe = UT3D_skp_2vc (&vhe, &vhe);
    // printf(" dqc=%f dqd=%f,dqe=%f\n",dqc,dqd,dqe);

  // mind 2 der 3 Werte muessen < distq sein
  if(ii == 1) {
    if(dqe > distq) return -1;
  }

  if(dqc > distq) goto L_d;

  if((dqd < distq)&&(dqe < distq)) { 
    // alle 3 Punkte sind in der Zone; do not use
    return 1;
  }


  L_c:
    ip1 = 0;
    p_p1 = pf_ab;
    if(dqd < distq) {   // c & d
      ip2 = 1;
      p_p2 = pg_ab;
    } else {            // c & e
      ip2 = 2;
      p_p2 = ph_ab;
    }
    goto L_done;


  L_d: // d & e in der Zone, nicht c
    ip1 = 1;            // d & e
    p_p1 = pg_ab;
    ip2 = 2;
    p_p2 = ph_ab;



  L_done:
    // printf("  ip1=%d ip2=%d p_p1=%f p_p2=%f\n",ip1,ip2,p_p1,p_p2);


  // parallel/antiparallel
  // printf(" s_ab_ab*0.8=%f  *0.2=%f\n",s_ab_ab * 0.8,s_ab_ab * 0.2);
  if(p_p1 < p_p2) {          // vorwaerts = parallel
    *iDir = 1;
  } else {                   // verkehrt = antiparallel
    *iDir = -1;
    // MEM_swap__ (&vac, &vad, sizeof(Vector));
    MEM_swap_2db (&p_p2, &p_p1);
  }
  // wenn ip1-ip2 CW ist, muss man *iDir umdrehen !
  if(UT3D_ck_triaOrientI(ip1,ip2) < 0) *iDir = *iDir * -1;
    // printf(" *iDir=%d\n",*iDir);


  // Ueberdeckung pa-pb / p1-p2
  // check Ueberdeckung
  d1 = DMAX(0., p_p1);
  d2 = DMIN(1., p_p2);
    // printf(" d1=%f d2=%f ueb=%f\n",d1,d2,(d2-d1));
  // check if outside; muss mind 10% ueberdecken ..
  if((d2 - d1) < 0.5) {
    // printf("ex ck_4pt -1 ueberd=%f\n",(d2-d1));
    return -1;
  }



  return 0;

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
  int UT3D_tria_tst_load (Triang *tri1, Triang *tri2) {
//================================================================
// UT3D_tria_tst_load         load testTriangs
// ACHTUNG: Triang besteht nur aus pointers; daher static !
// view 3Eck: GR_Disp_tria (&tri1, 9);
// Passender Punkt dazu: P20=P(17 14 4)

// Beispiel:
// Triang  tri1, tri2;
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
