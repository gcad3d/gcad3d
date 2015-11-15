//  Quaternion-utilities                          2006-07-13      RF / TB
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
\file  ../ut/ut_quat.c
\brief Quaternion-utilities 
\code
=====================================================
List_functions_start:

UQT_qt_vxvz      VX-Vec + VZ-Vec --> quat
UQT_qt_3vc       3 vecs --> quat
UQT_qt_vcar      Vec + Angle --> quat

UQT_3vc_qt       quat --> 3 vecs
UQT_vcar_qt      quat --> Vec + Angle

UQT_dump         dump quat

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



A Quaternion is defined using four floating point values |e0, e1, e2, e3;|.

Offline testen:
cc ut_quat.c ut_mainut.c -DOFFLINE -lm&&./a.out
==============================================================
*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



 
//========================================================
//========================================================
#ifdef OFFLINE


typedef struct {double dx, dy, dz;}                                 Vector;
typedef struct {double e0, e1, e2, e3;}                             Quat;

#define UT3D_len_vc(vc)\
 (sqrt((vc)->dx*(vc)->dx + (vc)->dy*(vc)->dy + (vc)->dz*(vc)->dz))



//============================
  int main (int paranz, char *params[]) {
//============================

  double   q[4], ar;
  // double   m[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
  // double   m[16] = { 0,1,0,0, -1,0,0,0, 0,0,1,0, 0,0,0,1};
  double   m[16] = { 0,0,1,0, -1,0,0,0, 0,-1,0,0, 0,0,0,1};
  Vector   vx={0,0,1}, vy={-1,0,0}, vz={0,-1,0}, vr;


  // UQT_qt_3vc (q, &vx, &vy, &vz);
  UQT_qt_vxvz (q, &vx, &vz);

  // UQT_dump (q);
  UQT_vcar_qt (&vr, &ar, q);  // quat --> Vec + Angle

  UQT_qt_vcar (q, ar, &vr);        // quat from angle+vec
  UQT_3vc_qt (&vx, &vy, &vz, q);  // 3 Vecs from quat

  return 0;
}


// NUR ZUM TESTEN !!!

void UT3D_vc_setLength (Vector *vo, Vector *vi, double dl) {
  double len;
  len = dl / UT3D_len_vc (vi);
  vo->dx = vi->dx*len;
  vo->dy = vi->dy*len;
  vo->dz = vi->dz*len;
}

void UT3D_vc_perp2vc   (Vector *vp, Vector *v1, Vector *v2) {
  vp->dx = v1->dy * v2->dz - v1->dz * v2->dy;
  vp->dy = v1->dz * v2->dx - v1->dx * v2->dz;
  vp->dz = v1->dx * v2->dy - v1->dy * v2->dx;
}



#else
//========================================================
//========================================================



#include "../ut/ut_geo.h"          // Vector, Quat ...

#endif


/*=======================================================================*/
  int UQT_qt_vcar (Quat *qu, double angle, Vector *axis) {
/*=====================
UQT_qt_vcar     quaternion <-- angle and axis

UQT_qt_vcar     Author: Thomas Backmeister       20.7.2006

Computation of a normalized quaternion from an angle and a rotation axis.

IN:
  double angle      ... angle of rotation (in [-pi, pi])
  Vector *axis      ... axis of rotation
OUT:
  Quat *qu          ... quaternion of length 1
Returncode:
  0 = OK
*/

  double k, e;

  printf("UQT_qt_vcar a=%f v=%f,%f,%f\n",angle,axis->dx,axis->dy,axis->dz);

  k = cos(angle/2.0);
  e = sqrt((1.0-k*k)/(axis->dx*axis->dx+axis->dy*axis->dy+axis->dz*axis->dz));
  if (angle < 0) e = -e;

  qu->e3 = k;
  qu->e0 = e*axis->dx;
  qu->e1 = e*axis->dy;
  qu->e2 = e*axis->dz;

    UQT_dump (qu);

  return 0;
}



//================================================================
  int UQT_qt_vxvz (double *q, Vector *vx, Vector *vz) {
//================================================================
// UQT_qt_vxvz      VX-Vec + VZ-Vec --> quat

  Vector   vcx,vcy,vcz;
  
  // den Y-Vektor errechnen (normal auf vx, vz)
  UT3D_vc_perp2vc (&vcy, vz, vx);


  UT3D_vc_setLength (&vcx, vx,   1.);
  UT3D_vc_setLength (&vcy, &vcy, 1.);
  UT3D_vc_setLength (&vcz, vz,   1.);

  UQT_qt_3vc (q, &vcx, &vcy, &vcz);


  return 0;

}


//================================================================
  int UQT_vcar_qt (Vector *vc, double *ar, double *q) {
//================================================================
// quat --> Vec + Angle
// quat: 0=x,1=y,2=z,3=w
// quat --> vector + angle

  double  val, sin_val, r[4];


  val = acos(q[3]);
  if (val == 0.) {
      vc->dx = 0.0;
      vc->dy = 0.0;
      vc->dz = 1.0;
      *ar  = 0.0;

  } else {
      sin_val = sin(val);
      vc->dx = q[0] / sin_val,
      vc->dy = q[1] / sin_val,
      vc->dz = q[2] / sin_val;
      // normalize r(0/1/2);
      *ar  = 2 * val;
  }

  // printf("ex UQT_vcar_qt  vc=%f %f %f ar=%f\n",vc->dx,vc->dy,vc->dz,*ar);

  return 0;
}


//================================================================
  int UQT_dump (double *q) {
//================================================================

  printf(" quat (%f %f %f %f)\n",q[0],q[1],q[2],q[3]);

  return 0;
}


//===================================================================
  int UQT_qt_3vc (double *quat, Vector *vx, Vector *vy, Vector *vz) {
//===================================================================
// UQT_qt_3vc       3 vecs --> quat

  double    tr;
  double    s;

  // printf("UQT_qt_3vc:\n");
    // printf(" vx=%f %f %f\n",vx->dx,vx->dy,vx->dz);
    // printf(" vy=%f %f %f\n",vy->dx,vy->dy,vy->dz);
    // printf(" vz=%f %f %f\n",vz->dx,vz->dy,vz->dz);


  tr = vx->dx + vy->dy + vz->dz;
  if( tr > 0.0 )  {
    s = sqrt( tr + 1.0 );
    quat[3] = s * 0.5;
    s = 0.5 / s;
    quat[0] = (vy->dz - vz->dy) * s;
    quat[1] = (vz->dx - vx->dz) * s;
    quat[2] = (vx->dy - vy->dx) * s;
    return 0;
  }

  // Find dominant element of primary diagonal
  if( vy->dy > vx->dx )  {
    if( vz->dz > vy->dy )  {
      s = sqrt( vz->dz - (vx->dx+vy->dy) + 1.0 );
      quat[2] = s * 0.5;
      s = 0.5 / s;
      quat[3] = (vx->dy - vy->dx) * s;
      quat[0] = (vz->dx + vx->dz) * s;
      quat[1] = (vz->dy + vy->dz) * s;
    } else {
      s = sqrt( vy->dy - (vz->dz+vx->dx) + 1.0 );
      quat[1] = s * 0.5;
      s = 0.5 / s;
      quat[3] = (vz->dx - vx->dz) * s;
      quat[2] = (vy->dz + vz->dy) * s;
      quat[0] = (vy->dx + vx->dy) * s;
    }

  } else {
    if( vz->dz > vx->dx )  {
      s = sqrt( vz->dz - (vx->dx+vy->dy) + 1.0 );
      quat[2] = s * 0.5;
      s = 0.5 / s;
      quat[3] = (vx->dy - vy->dx) * s;
      quat[0] = (vz->dx + vx->dz) * s;
      quat[1] = (vz->dy + vy->dz) * s;
    } else {
      s = sqrt( vx->dx - (vy->dy+vz->dz) + 1.0 );
      quat[0] = s * 0.5;
      s = 0.5 / s;
      quat[3] = (vy->dz - vz->dy) * s;
      quat[1] = (vx->dy + vy->dx) * s;
      quat[2] = (vx->dz + vz->dx) * s;
    }
  }

  // printf("ex UQT_qt_3vc (%f %f %f %f)\n",quat[0],quat[1],quat[2],quat[3]);
  return 0;

}


/*=======================================================================*/
  int UQT_3vc_qt (Vector *vx, Vector *vy, Vector *vz, Quat *qu) {
/*======================
UQT_3vc_qt     Vx+Vy+Vz  <-- normalized quaternion

UQT_3vc_qt     Author: Thomas Backmeister       20.7.2006

Computation of Vx+Vy+Vz (rotation matrix) from a normalized quaternion.

IN:
  Quat *qu          ... quaternion of length 1
OUT:
  Vector Vx,Vy,Vz   ... rotation matrix
Returncode:
  0 = OK
*/


  // printf("UQT_3vc_qt (%f %f %f %f)\n",qu->e0,qu->e1,qu->e2,qu->e3);


  vx->dx = qu->e3*qu->e3 + qu->e0*qu->e0 - qu->e1*qu->e1 - qu->e2*qu->e2;
  vx->dy = 2*qu->e3*qu->e2 + 2*qu->e0*qu->e1;
  vx->dz = -2*qu->e3*qu->e1 + 2*qu->e0*qu->e2;

  vy->dx = -2*qu->e3*qu->e2 + 2*qu->e0*qu->e1;
  vy->dy = qu->e3*qu->e3 - qu->e0*qu->e0 + qu->e1*qu->e1 - qu->e2*qu->e2;
  vy->dz = 2*qu->e3*qu->e0 + 2*qu->e1*qu->e2;

  vz->dx = 2*qu->e3*qu->e1 + 2*qu->e0*qu->e2;
  vz->dy = -2*qu->e3*qu->e0 + 2*qu->e1*qu->e2;
  vz->dz = qu->e3*qu->e3 - qu->e0*qu->e0 - qu->e1*qu->e1 + qu->e2*qu->e2;


    // printf(" vx=%f %f %f\n",vx->dx,vx->dy,vx->dz);
    // printf(" vy=%f %f %f\n",vy->dx,vy->dy,vy->dz);
    // printf(" vz=%f %f %f\n",vz->dx,vz->dy,vz->dz);

  return 0;
}


//====================== EOF ===========================
