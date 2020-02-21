//  gru.c                                      CADCAM-Services Franz Reiter
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
2005-04-02 Aus ut_GL.c ausgelagert. RF.

-----------------------------------------------------
*/
/*!
\file  ../gr/gru.c
\brief  auxil. graphic-functions not using OpenGL
\code
=====================================================
List_functions_start:

GRU_angr3_vxvz       wie UT3D_3angr_2vc, jedoch degrees out
GRU_calc_normal
GRU_calc_normal2

GRU_tex_pos1
GRU_tex_pos2
GRU_tex_pos3        for Z-meshes only
GRU_tex_pos9
GRU_tex_rec

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>                         /* fabs  ...   */
#include <stdio.h>                        // FILE ..
#include <stdlib.h>
#include <string.h>                       // strlen



#include "../ut/ut_geo.h"                // Point-def
#include "../ut/ut_memTab.h"           // MemTab

#include "../xa/xa_mem.h"                // memspc5*


 
/*=============== Externe Variablen: =======================================*/




//==============================================================================
   int GRU_tex_pos2 (TexRef *rtex, Point *p1a, Point *p2a, int ptNr, int sTyp) {
//==============================================================================
// Vektoren vx/vy, Scales scx/scy und  Offsets dx/dy setzen.
// called by TSU_DrawSurTRU
// Daten ex UT3D_cv_sru_
// sTyp  0  beide p1Tab befuellt mit mind. 2 Punkten
//       1  p1Tab hat nur 1 Punkt
//       2  p2Tab hat nur 1 Punkt
// USING memspc53

  int     ii, i1, i2, i3;
  Point   **ppa;
  Vector  vz;


  // printf("GGGGGGGGGGGGGGGGGGGGGGGG GRU_tex_pos2 GGGGGGGGGGGGGGGGGGGGGGGG\n");


  // fill ppa with all points of the surface
  ppa = (Point**)memspc53;
  ii = sizeof(memspc53) / sizeof(void*);
    // printf(" ii=%d\n",ii);

  if(sTyp == 0) {
    GRU_calc_normal(&vz, &p1a[1], &p2a[0], &p1a[0]);
    i1 = ptNr;
    i2 = ptNr;

  } else if(sTyp == 1) {
    GRU_calc_normal(&vz, &p1a[0], &p2a[0], &p2a[1]);
    i1 = 1;
    i2 = ptNr;

  } else if(sTyp == 2) {
    GRU_calc_normal(&vz, &p1a[1], &p2a[0], &p1a[0]);
    i1 = ptNr;
    i2 = 1;
  }

  if(i1 + i2 > ii) {
    printf("***** GRU_tex_pos2 E001 - EOM *****\n");
    return -1;
  }

  for(i3=0; i3<i1; ++i3) ppa[i3] = &p1a[i3];
  for(i3=0; i3<i2; ++i3) ppa[i3+i1] = &p2a[i3];
  ii = i1 + i2;

    // for(i1=0;i1<ii;++i1) DEB_dump_obj__(Typ_PT,ppa[i1],"ppa[%d]=",i1);


  //----------------------------------------------------------------
  UT3D_vc_setLength (&vz, &vz, 1.);
 
  // get vectors, origin and size
  GRU_tex_rec (rtex, &vz, ppa, ii);


  // umwandeln fuer OpenGL-Textur
  GRU_tex_pos9 (rtex);


  return 0;

}


//================================================================
   int GRU_tex_pos1 (TexRef *rtex, ObjGX *os) {
//================================================================
// Vektoren vx/vy, Scales scx/scy und  Offsets dx/dy setzen.
// os hat tesselierte Daten ex GL_DrawSur

  int     ii;
  Vector  vz;
  Point   **ppa;
  // Plane   pl1;


  // printf("GGGGGGGGGGGGGGGGGGGGGGGG GRU_tex_pos1 GGGGGGGGGGGGGGGGGGGGGGGG\n");
    // DEB_dump_ox_0 (os, "GL_DrawSur");


  // get space for a table of pointers
  ppa = (Point**)memspc55;
  ii = sizeof(memspc55) / sizeof(void*);
    // printf(" ii=%d\n",ii);


  // get pointers of all points in GL-Surf os
  UTO_npt_Tes (&ii, ppa, os);


  // get normalvektor to surf
  TSU_get_vec (&vz, os);
    // DEB_dump_obj__ (Typ_VC, &vz, " vz");


  // get vectors, origin and size
  GRU_tex_rec (rtex, &vz, ppa, ii);


  // umwandeln fuer OpenGL-Textur
  GRU_tex_pos9 (rtex);

  return 0;

}


//================================================================
  int GRU_tex_pos9 (TexRef *rtex) {
//================================================================
// compute scale & offset for texture for OpenGL.


  double   xSiz, ySiz, d1, d2, x1, y1;
  Point    p1;


  // printf("GRU_tex_pos9 ============\n");


  // p1 = origin der Flaeche
  p1.x = rtex->px;
  p1.y = rtex->py;
  p1.z = rtex->pz;
    // DEB_dump_obj__(Typ_PT, &p1, "GRU_tex_pos9 p1");



  //----------------------------------------------------------------
  // spiegeln um Y: Vector umdrehen und den origin auf der anderen Seite
  if(rtex->uscx < 0.) {
    rtex->uscx = -rtex->uscx;
    // move origin p1 along vx length=ssx
    UT3D_pt_traptvclen (&p1, &p1, &rtex->vx, rtex->ssx);
    // invert vx
    UT3D_vc_invert (&rtex->vx, &rtex->vx);

    rtex->px = p1.x;
    rtex->py = p1.y;
    rtex->pz = p1.z;
  }



  //----------------------------------------------------------------
  // spiegeln um X: Vector umdrehen und den origin auf der anderen Seite
  if(rtex->uscy < 0.) {
    rtex->uscy = -rtex->uscy;
    // move origin p1 along vx length=ssx
    UT3D_pt_traptvclen (&p1, &p1, &rtex->vy, rtex->ssy);
    // invert vx
    UT3D_vc_invert (&rtex->vy, &rtex->vy);

    rtex->px = p1.x;
    rtex->py = p1.y;
    rtex->pz = p1.z;
  }




  // Flaechenorigin; die UserCoords des TexturNullpunkts auf der Flaeche.
  // ACHTUNG: hier ist der Abstand des Absoluten Nullpunktes zum
  //   TexturNullpunkts bezueglich der Ebene vx/vy erforderlich !!!
 
  // x1,y1 = signed distance of surface-origin from 0,0,0 in diretions vx/vy
  x1 = UT3D_slen_2ptvc ((Point*)&UT3D_PT_NUL, &p1, &rtex->vx);
  y1 = UT3D_slen_2ptvc ((Point*)&UT3D_PT_NUL, &p1, &rtex->vy);
  // x1 = rtex->px;
  rtex->fx = x1;
  rtex->fy = y1;
    // printf(" x1=%f y1=%f\n",x1,y1);

/*
  // den Faktor errechnen; nur den nachKommaTeil behalten (offset).
  xSiz = rtex->ssx;
  // xSiz = rtex->vx.dx * rtex->ssx;
    // printf(" xSiz=%f\n",xSiz);
  d1 = 1. - fabs(x1 / xSiz);
  // d1 = fabs(xSiz / x1);
  rtex->fx = modf (d1, &d2);  // keep only nachKommaTeil
  if(x1 < 0.) rtex->fx = 1  - rtex->fx;


  // den Faktor errechnen; nur den nachKommaTeil behalten (offset).
  ySiz = rtex->ssy;
  d1 = 1. - fabs(y1 / ySiz);
  // d1 = fabs(ySiz / y1);
  rtex->fy = modf (d1, &d2);  // keep only nachKommaTeil
  if(y1 < 0.) rtex->fy = 1  - rtex->fy;
*/
    // printf(" texRef.fx=%f fy=%f\n",rtex->fx,rtex->fy);
    // printf(" texRef.uscx / y = %f %f\n",rtex->uscx,rtex->uscy);


  return 0;

}


//==================================================================
  int GRU_tex_rec (TexRef *rtex, Vector *vz, Point **ppa, int pNr) {
//==================================================================
// GRU_tex_rec         find rectangle for texture in modelspace
// get 2 vectors and the origin of a rectangle around the surface.
// and the length of the 2 vectors (size of the rectangle)
// Input:
//   vz              Normalvektor Flaeche
//   ppa             punkte der Flaeche
//   pNr             nr of points in ppa
// Output:
//   rtex->vs,vy     vectors, len=1
//   rtex->px,py,pz  origin
//   rtex->ssx,ssy   length of surface (in usercoords, along vx/vy)

/*
Fuer jede TEXR:
  Input: alle tesselierten Punkte  (pNr, ppa)
  Output: XScale,YScale, X-Offset,Y-Offset.
  Origin, size in X-Richtung, size in Y-Richtung ermitteln,
  daraus den X-Vec, den Y-vec, den Scale und den offset errechnen.
  N-Vec aus allen Punkten ermitteln;
  den X-Vec und den Y-Vec zum N-Vec ermitteln (normiert)
  X-Siz/Y-Siz = MaxDist entlang X-Vec und entlang Y-Vec ermitteln
  wenn ySiz > xSiz  Achsen umdrehen ..
  Origin = der dem absoluten 0-Punkt naehere punkt der 2D-Box ... ???
  XScale = 1 / X-Siz
  YScale = 1 / Y-Siz
  iNr = X-Dist-Origin / X-Siz
  X-Offset =  fmod(X-Dist-Origin, iNr) / X-Siz;
  iNr = Y-Dist-Origin / Y-Siz
  Y-Offset = fmod(Y-Dist-Origin, iNr) / Y-Siz;



Die Textur wird am Nullpunkt geladen; wenn der erste Punkt der Textur am
Nullpunkt liegt, ist kein Offset notwendig; else muss der Offset in der
Plane bis zum ersten Punkt definiert werden (von 0-1; in X und Y !).
Der Offset vom (absoluten) Nullpunkt muss selbst errechnet werden !

Die Werte fuer glTranslate sind nicht UserUnits sondern Faktoren;
  X-Wert=1.5  bedeutet die Textur um 1.5-fache Flaechenbreite verschieben !

pa sind die outermost points entlang X und Y.


*/

  int     i1, ii1, ii2;   // bp;
  double  d1, dmin, dmax, dx, dy;
  Point   *po, p31; // p3o, p3x, p3y;
  Point2  p21, pb1, pb2;
  Vector  vc1;


  // printf("GRU_tex_rec %d\n",pNr);
    // DEB_dump_obj__ (Typ_VC, vz, " vz");
    // for(i1=0;i1<pNr;++i1)GR_Disp_pt (ppa[i1], SYM_TRI_S, ATT_COL_RED);


  //----------------------------------------------------------------
  // fix vectors rtex->vx & rtex->vy

  // if NormalVector = 0,0,1: use VX=1,0,0; VY=0,1,0
  if((fabs(vz->dx) < UT_TOL_min1) &&
      (fabs(vz->dy) < UT_TOL_min1)) {
    rtex->vx = UT3D_VECTOR_X;
    rtex->vy = UT3D_VECTOR_Y;


  } else {
    // den X-Vec und den Y-Vec zum N-Vec ermitteln (normiert)
    // X-vec: den vz auf die xy-ebene projizieren, normieren.
    UT3D_vc_perp1vc (&rtex->vx, vz);
    UT3D_vc_setLength (&rtex->vx, &rtex->vx, 1.);
    // Y-vec: crossprod.
    UT3D_vc_perp2vc (&rtex->vy, &rtex->vx, vz);
    UT3D_vc_setLength (&rtex->vy, &rtex->vy, 1.);
  }
/*
  UT3D_pl_ptvc (&pl1, &pa[0], &vz);
  UT3D_vc_setLength (&rtex->vx, &pl1.vx, 1.);
  UT3D_vc_setLength (&rtex->vy, &pl1.vy, 1.);
  // turn vector up
  UT3D_vc_Zup (&rtex->vy, &rtex->vy);
*/

  // wenn vy nach unten zeigt, wird das Bild umd die X-Achse gespiegelt !
  if(rtex->vy.dz < 0.) {
    UT3D_vc_invert (&rtex->vy, &rtex->vy);
    // UT3D_vc_invert (&rtex->vx, &rtex->vx);
      // printf(" invert y ..\n");
  }

    // DEB_dump_obj__ (Typ_VC, &rtex->vx, " vx");
    // DEB_dump_obj__ (Typ_VC, &rtex->vy, " vy");


/*
  //----------------------------------------------------------------
  // get backplane of surf
  bp = UT3D_bp_vcz (vz);
    // printf(" bp=%d\n",bp);


  // Punkte werden auf die Backplane gelegt; Origin immer links unten.
  // Wenn Z-Vektor verkehrt zur backplane: X-Vektor umdrehen.
  // if(bp == BCKPLN_XZ) {
    // if(rtex->vx.dx > 0.) 
      // UT3D_vc_invert (&rtex->vx, &rtex->vx);
  // }


  // fix origin & size
  po = ppa[0];


  // init 2D-box
  UT2D_box_ini0 (&pb1, &pb2);

  
  // get table of 2D-points from backplane and pointers
  for(i1=0; i1<pNr; ++i1) {
    UT2D_pt_tra_pt3_bp (&p21, ppa[i1], bp);
      // DEB_dump_obj__(Typ_PT2 , &p21, "p21[%d]=",i1);

    // extend 2D-Box
    UT2D_box_extend (&pb1, &pb2, &p21);
  }

    // DEB_dump_obj__(Typ_PT2, &pb1, " _ext pb1o=");
    // DEB_dump_obj__(Typ_PT2, &pb2, " _ext pb2o=");



  // pb1 liegt nun ganz links unten.
  // make 3D-point from 2D-point & backplane
  UT3D_pt_tra_pt2_bp (&p31, &pb1, bp);
    // DEB_dump_obj__(Typ_PT, &p31, " p31=");


  // get vector from Backplane
  UT3D_vc_bp (&vc1, bp);


  // nun den p31 (liegt auf der Backplane) retourprojizieren auf die Ebene.
  // p32 = project p31 along vc1 onto plane po-vz
  UT3D_ptDi_intptvcptvc (&p3o, &d1, &p31, &vc1, po, vz);
    // DEB_dump_obj__(Typ_PT, &p3o, " p3o=");
*/

  //----------------------------------------------------------------
  // get a post to start ..
  po = ppa[0];


  // find size in direction vx
  dmin = UT_VAL_MAX;
  dmax = UT_VAL_MIN;
  for(i1=0; i1<pNr; ++i1) {
    d1 = UT3D_slenq_2ptvc (po, ppa[i1], &rtex->vx);
      // printf(" d1[%d] = %lf\n",i1,d1);
    if(d1 < dmin) { dmin = d1; ii1 = i1; }
    if(d1 > dmax) { dmax = d1; ii2 = i1; }
  }
    // printf(" dx: ii1=%d ii2=%d\n",ii1,ii2);
  // leftmost point in direction vx
  po = ppa[ii1];
  dx  = UT3D_slen_2ptvc (po, ppa[ii2], &rtex->vx);
    // printf(" _tex_rec dx=%f\n",dx);




  // find size in direction vy
  dmin = UT_VAL_MAX;
  dmax = UT_VAL_MIN;
  for(i1=0; i1<pNr; ++i1) {
    d1 = UT3D_slenq_2ptvc (po, ppa[i1], &rtex->vy);
    if(d1 < dmin) { dmin = d1; ii1 = i1; }
    if(d1 > dmax) { dmax = d1; ii2 = i1; }
  }
    // printf(" dy: ii1=%d ii2=%d\n",ii1,ii2);
  // leftmost point in direction vy
  UT3D_pt_projptptvc (&p31, &d1, NULL, ppa[ii1], po, &rtex->vy);
  po = &p31;

  dy  = UT3D_slen_2ptvc (po, ppa[ii2], &rtex->vy);
    // printf(" _tex_rec dy=%f\n",dy);
    // DEB_dump_obj__ (Typ_PT, po, " po");



  //----------------------------------------------------------------
  // den origin p3o -> px,py,pz
  rtex->px = po->x;
  rtex->py = po->y;
  rtex->pz = po->z;

/*
  //----------------------------------------------------------------
  // compute size; output -> ssx,ssy
  p21.x = pb2.x;
  p21.y = pb1.y;
  UT3D_pt_tra_pt2_bp (&p31, &p21, bp);
  UT3D_ptDi_intptvcptvc (&p3x, &d1, &p31, &vc1, po, vz);
    // DEB_dump_obj__(Typ_PT, &p3x, " p3x=");

  p21.x = pb1.x;
  p21.y = pb2.y;
  UT3D_pt_tra_pt2_bp (&p31, &p21, bp);
  UT3D_ptDi_intptvcptvc (&p3y, &d1, &p31, &vc1, po, vz);
    // DEB_dump_obj__(Typ_PT, &p3y, " p3y=");

  // size dx/dy:
  // dx = pb2.x - pb1.x;
  // dy = pb2.y - pb1.y;
  rtex->ssx = UT3D_len_2pt(&p3o, &p3x);
  rtex->ssy = UT3D_len_2pt(&p3o, &p3y);
*/

  rtex->ssx = dx;
  rtex->ssy = dy;
    // printf(" ssx,ssy: %f %f\n",rtex->ssx,rtex->ssy);


  return 0;

}


//==============================================================================
   int GRU_tex_pos3 (TexRef *rtex, Point *pa, int pNr) {
//==============================================================================
// nur fuer Z-meshes.    Set vx,vy,  px,py,    ssx,ssy
// see also GRU_tex_pos1


  int      i1, ind;
  double   xSiz, ySiz, x1, x2, y1, y2, z1, z2;


  // printf("GGGGGGGGGGGGGGGGGGGGGGGG GRU_tex_pos3 %d GGGGGGGGGGGGGG\n",pNr);
  // for(i1=0; i1<10; ++i1) DEB_dump_obj__ (Typ_PT, &pa[i1], " p1[%d]:",i1);

  rtex->vx = UT3D_VECTOR_X;
  rtex->vy = UT3D_VECTOR_Y;


  // find min. xval
  i1 = UTP_dbRec_min (pNr, &pa[0].x, sizeof(Point));
  x1 = pa[i1].x;
  z1 = pa[i1].z;

  // find max. xval
  i1 = UTP_dbRec_max (pNr, &pa[0].x, sizeof(Point));
  x2 = pa[i1].x;
  z2 = pa[i1].z;

  // find min. yval
  i1 = UTP_dbRec_min (pNr, &pa[0].y, sizeof(Point));
  y1 = pa[i1].y;

  // find max. yval
  i1 = UTP_dbRec_max (pNr, &pa[0].y, sizeof(Point));
  y2 = pa[i1].y;
     // printf(" min/max %f %f %f %f\n",x1,x2,y1,y2);

  xSiz = x2 - x1;
  ySiz = y2 - y1;
    // printf(" texRef.xSiz=%f ySiz=%f\n",xSiz,ySiz);


  // rtex->ssx = 1. / xSiz;
  // rtex->ssy = 1. / ySiz;
  rtex->ssx = xSiz;
  rtex->ssy = ySiz;
    // printf(" texRef.ssx=%f ssy=%f\n",rtex->ssx,rtex->ssy);


  rtex->px =  x1;
  rtex->py =  y1;
  rtex->pz =  (z1 + z2) / 2.;
    // printf(" texRef.px=%f py=%f\n",rtex->px,rtex->py);


  rtex->fx = x1;
  rtex->fy = y1;
    // printf(" fx=%f fy=%f\n",x1,y1);



  return 0;

}


//================================================================
  int GRU_angr3_vxvz (double *az1, double *ay, double *az2,
                     Vector *vx, Vector *vz) {
//================================================================
// wie UT3D_3angr_2vc, jedoch degrees out
// RetCod:  0 - OK, alle 3 Winkel errechnet.
// RetCod: -1 - kein Input = Defaultlage

  int   irc;

  // printf("GRU_angr3_vxvz:\n");
  // DEB_dump_obj__ (Typ_VC, vx, "vx:");
  // DEB_dump_obj__ (Typ_VC, vz, "vz:");

 
  // wenn vz=Nullvektor, keine Transformation
  if(UT3D_compvc0 (vz, UT_TOL_min1) == 0)  {        // kein Nullvektor !

    // 3 opening angels (3D-position) from X and Z-vec.
    UT3D_3angr_2vc (az1, ay, az2, vx, vz);
    *az1 = UT_DEGREES(*az1);
    *ay  = UT_DEGREES(*ay);
    *az2 = UT_DEGREES(*az2);
    // printf("  az1=%f ay=%f az2=%f\n",az1,ay,az2);

    irc = 0;


  } else {
    irc = -1;
  }

  // printf("ex GRU_angr3_vxvz %d %f %f %f\n",irc,*az1,*ay,*az2);

  return irc;

}


/*===================================================================== */
  int GRU_calc_normal (Vector* vc, Point* p1,  Point* p2, Point* p3) {
/*===================================================================== */
// NICHT NORMIERT !

  double lv;
  Vector v1, v2, vn;


  /* die 2 Vektoren */
  v1.dx = p2->x - p1->x;
  v1.dy = p2->y - p1->y;
  v1.dz = p2->z - p1->z;

  v2.dx = p3->x - p1->x;
  v2.dy = p3->y - p1->y;
  v2.dz = p3->z - p1->z;

  /* printf("v1=%f,%f,%f\n",v1.dx,v1.dy,v1.dz); */
  /* printf("v2=%f,%f,%f\n",v2.dx,v2.dy,v2.dz); */



  /* das Kreuzprod */
  vn.dx = (v1.dy * v2.dz) - (v1.dz * v2.dy);
  vn.dy = (v1.dz * v2.dx) - (v1.dx * v2.dz);
  vn.dz = (v1.dx * v2.dy) - (v1.dy * v2.dx);

  /* printf("vn=%f,%f,%f\n",vn.dx,vn.dy,vn.dz); */


  *vc = vn;
/*
  // normalize
  lv = sqrt((vn.dx * vn.dx) + (vn.dy * vn.dy) + (vn.dz * vn.dz));
  if(lv == 0.0) lv = 1.0;
  vc->dx = vn.dx / lv;
  vc->dy = vn.dy / lv;
  vc->dz = vn.dz / lv;
*/


  // umdrehen (Test)
  // vc->dx = -vc->dx;
  // vc->dy = -vc->dy;
  // vc->dz = -vc->dz;

  // printf("ex GRU_calc_normal %f,%f,%f\n",vc->dx,vc->dy,vc->dz);

  return 0;

}




/*===================================================================== */
  void GRU_calc_normal2 (Vector* vc, Point2* p1,  Point2* p2, Point2* p3,
                        double z1, double z2) {
/*===================================================================== */
/*
p1 und p2 muessen auf Z-Level z1 sein, p3 auf Z=z2
*/

  double lv;
  Vector v1, v2, vn;


  /* die 2 Vektoren */
  v1.dx = p2->x - p1->x;
  v1.dy = p2->y - p1->y;
  v1.dz = 0.0;

  v2.dx = p3->x - p1->x;
  v2.dy = p3->y - p1->y;
  v2.dz = z2 - z1;

  /* printf("v1=%f,%f,%f\n",v1.dx,v1.dy,v1.dz); */
  /* printf("v2=%f,%f,%f\n",v2.dx,v2.dy,v2.dz); */



  /* das Kreuzprod */
  vn.dx = (v1.dy * v2.dz) - (v1.dz * v2.dy);
  vn.dy = (v1.dz * v2.dx) - (v1.dx * v2.dz);
  vn.dz = (v1.dx * v2.dy) - (v1.dy * v2.dx);

  /* printf("vn=%f,%f,%f\n",vn.dx,vn.dy,vn.dz); */



  /* normalize */
  lv = sqrt((vn.dx * vn.dx) + (vn.dy * vn.dy) + (vn.dz * vn.dz));
  if(lv == 0.0) lv = 1.0;
  vc->dx = vn.dx / lv;
  vc->dy = vn.dy / lv;
  vc->dz = vn.dz / lv;


  // umdrehen (Test)
  vc->dx = -vc->dx;
  vc->dy = -vc->dy;
  vc->dz = -vc->dz;
  /* printf("norm=%f,%f,%f\n",vc->dx,vc->dy,vc->dz); */

}

//=============================== EOF ==============================
