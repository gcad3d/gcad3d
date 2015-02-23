//**************************************************************************
/*
 *
 * Copyright (C) 2015 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
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
2004-06-11 neu aus ut_geo.c RF.

-----------------------------------------------------
*/
/*!
\file  ../ut/ut_box.c
\brief  2D-boox, 3D-box
\code

Box = axis-parallel box from 2 points:
 p1 = low x,y,z,
 p2 = high x,y,z.

=====================================================
List_functions_start:

---------------- 2D ------------------------
UT2D_ckBoxinBox1          check if 2 boxes overlap
UT2D_ckBoxinBox2          check if Box2 is complete in Box1
UT2D_pt_ck_inBox          check if point is inside box
UT2D_pt_ck_inBoxTol       check if point p is inside box p1-p2

UT2D_rect_pts             bounding rectangle <-- 2D-points
UT2D_rect_pta3            bounding rectangle <-- 3D-points
UT2D_box_ini0             init box
UT2D_box_extend           Box mit point vergroessern


---------------- 3D ------------------------
UT3D_ckBoxinBox1          check if 2 boxes overlap
UT3D_pt_ck_inBoxTol       check if point p is inside box p1-p2
UT3D_ck_ptInBox           check if point px is inside box p1-p2
UT3D_box_ck_intLn         check if line intersects with axis-parallel box
UT3D_box_ck_intpl         check intersect. Plane / Box (estimate!)

UT3D_box_ix_npt           get bounding-box for n points and find extreme points
UT3D_box_ini0             init box
UT3D_box_pts              bounding box <-- points
UT3D_box_2pt              make box of 2 points
UT3D_box_2pttol           make box of 2 points + tol
UT3D_box_tria             make box from triangle
UT3D_box_extend           Box mit point vergroessern
see also UTO_box_obj
UT3D_box_addTol           add tolerance to box

UT3D_ptvc_intbox          intersect point/vector with box
UT3D_ch_lnbox             check line-Interection with axis-parallel box (u3d.c)
UT3D_ln_intbox            relimit line inside box

UT3D_cv_boxxy             load rect.points from xmin/xmax ymin/ymax in z=0

Liste_Funktionen_Ende:
=====================================================
- see also:
UTO_box_obj                Box mit obj vergroessern
GR_Disp_box                disp boundingBox

\endcode *//*----------------------------------------



=====================================================

*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_geo.h"
#include "../db/ut_DB.h"               //





//====================================================================
  int UT2D_box_ini0 (Point2 *pt1, Point2 *pt2) {
//====================================================================
/// UT3D_box_ini0         init box


  pt1->x =  UT_VAL_MAX;
  pt2->x = -UT_VAL_MAX;

  pt1->y =  UT_VAL_MAX;
  pt2->y = -UT_VAL_MAX;

  return 0;

}


//====================================================================
  int UT2D_box_extend (Point2 *pb1, Point2 *pb2, Point2 *pt1) {
//====================================================================
/// \code
/// Box mit point vergroessern.
/// init Box zB mit UT3D_box_ini0
/// \endcode


    // UT3D_stru_dump(Typ_PT2, pt1, "UT2D_box_extend");
    // UT3D_stru_dump(Typ_PT2, pb1, " _ext pb1i=");
    // UT3D_stru_dump(Typ_PT2, pb2, " _ext pb2i=");


  if(pt1->x < pb1->x) pb1->x = pt1->x;
  if(pt1->x > pb2->x) pb2->x = pt1->x;

  if(pt1->y < pb1->y) pb1->y = pt1->y;
  if(pt1->y > pb2->y) pb2->y = pt1->y;

    // UT3D_stru_dump(Typ_PT2, pb1, " _ext pb1o=");
    // UT3D_stru_dump(Typ_PT2, pb2, " _ext pb2o=");

  return 0;

}


//====================================================================
  int UT3D_box_ini0 (Point *pt1, Point *pt2) {
//====================================================================
/// UT3D_box_ini0         init box


  pt1->x =  UT_VAL_MAX;
  pt2->x = -UT_VAL_MAX;

  pt1->y =  UT_VAL_MAX;
  pt2->y = -UT_VAL_MAX;

  pt1->z =  UT_VAL_MAX;
  pt2->z = -UT_VAL_MAX;


  return 0;

}




//=======================================================================
  int UT2D_ckBoxinBox1 (Point2 *p1, Point2 *p2, Point2 *p3, Point2 *p4) {
//=======================================================================
/// \code
/// check if 2 boxes overlap
/// p1-p2 sind Box1, p3-p4 Box2.
/// ACHTUNG: p1 muss links unter p2 liegen; p3 links unter p4.
/// RC -1:   NO, boxes do not overlap
/// RC  1:   yes boxes overlap.
/// \endcode


  // printf(" UT2D_ckBoxinBox1\n");
  // printf("    b1=%f,%f  - %f,%f\n",p1->x,p1->y,p2->x,p2->y);
  // printf("    b2=%f,%f  - %f,%f\n",p3->x,p3->y,p4->x,p4->y);



  if(p3->x > p2->x) return -1;  // b2 ganz rechts
  if(p4->x < p1->x) return -1;  // b2 ganz links

  if(p3->y > p2->y) return -1;  // b2 ueber b1
  if(p4->y < p1->y) return -1;  // b2 unter b1

  return 1;  // irgendwo overlap

}


//=======================================================================
  int UT2D_ckBoxinBox2 (Point2 *p1, Point2 *p2,
                        Point2 *p3, Point2 *p4, double tol) {
//=======================================================================
/// \code
/// check if Box2 is complete in Box1
/// p1-p2 sind Box1, p3-p4 Box2.
/// ACHTUNG: p1 muss links unter p2 liegen; p3 links unter p4.
/// RC -1:   NO, boxes overlap
/// RC  1:   yes, Box2 is complete inside Box1
/// \endcode


  // printf(" UT2D_ckBoxinBox2\n");
  // printf("    b1=%f,%f  - %f,%f\n",p1->x,p1->y,p2->x,p2->y);
  // printf("    b2=%f,%f  - %f,%f\n",p3->x,p3->y,p4->x,p4->y);


  if(p3->x < p1->x + tol) return -1;   // overlap links
  if(p4->x > p2->x - tol) return -1;   // overlap rechts

  if(p4->y > p2->y - tol) return -1;   // overlap oben
  if(p3->y < p1->y + tol) return -1;   // overlap unten

  return 1;   // komplett innerhalb

}



//=======================================================================
  int UT2D_pt_ck_inBox (Point2 *p1, Point2 *p2, Point2 *p) {
//=======================================================================
/// \code
/// UT2D_pt_ck_inBox             check if point p is inside box p1-p2
/// RC = 0:   YES, p is inside box p1 / p2
/// RC = 1:   NO,  p is outside box p1 / p2
/// \endcode


  if(UTP_db_ck_in2db (p->x, p1->x, p2->x) != 0) return 1;

  if(UTP_db_ck_in2db (p->y, p1->y, p2->y) != 0) return 1;

  return 0;

}


//======================================================================
  int UT2D_rect_pts (Point2 *pmin, Point2 *pmax, int nump, Point2 *ptab) {
//======================================================================
// UT2D_rect_pts        Author: Thomas Backmeister       3.6.2003
// 
/// \code
/// UT2D_rect_pts        bounding rectangle <-- 2D-points
/// 
/// Computation of a minimal axis-parallel rectangle which contains all
/// given 2D-points.
/// The rectangle is given by the endpoints of its diagonal.
/// If all points ly on a line parallel x-, y- or z- axis, the rectangle
/// will be a line segment.
/// 
/// IN:
///   int nump     ... number of 2D-points
///   Point2 *ptab ... 2D-points
/// OUT:
///   Point2 *pmin ... endpoint 1 of rectangle diagonal (lower left)
///   Point2 *pmax ... endpoint 2 of rectangle diagonal (upper right)
/// Returncodes:
///   0 = OK
///  -1 = input error
/// \endcode


  int i1;


  // printf("UT2D_rect_pts ptNr=%d\n",nump);


  if (nump < 1) {
    printf("UT2D_rect_pts: input error\n");
    return -1;
  }

  *pmin = ptab[0];
  *pmax = ptab[0];

  for (i1=1; i1<nump; ++i1) {
    // printf(" pt[%d]=%f,%f\n",i1,ptab[i1].x,ptab[i1].y);
    if (ptab[i1].x < pmin->x) pmin->x = ptab[i1].x;
    if (ptab[i1].y < pmin->y) pmin->y = ptab[i1].y;
    if (ptab[i1].x > pmax->x) pmax->x = ptab[i1].x;
    if (ptab[i1].y > pmax->y) pmax->y = ptab[i1].y;
  }


  // printf("ex UT2D_rect_pts: %9.3f,%9.3f - %9.3f,%9.3f\n",
         // pmin->x,pmin->y,pmax->x,pmax->y);

  return 0;
}


//=================================================================
  int UT2D_rect_pta3 (Point2 *p1, Point2 *p2, Point *pa, int pNr) {
//=================================================================
/// UT2D_rect_pta3           bounding rectangle <-- 3D-points

  int    i1;


  *p1 = UT2D_pt_pt3 (&pa[0]);
  *p2 = UT2D_pt_pt3 (&pa[1]);

  for(i1=2; i1 < pNr; ++i1) {
    UT2D_box_extend (p1, p2, (Point2*)&pa[i1]);
  }

  // printf("ex UT2D_rect_pta3 %f %f / %f %f\n",p1->x,p1->y,p2->x,p2->y);

  return 0;

}


//=======================================================================
  int UT3D_ckBoxinBox1 (Point *p1, Point *p2, Point *p3, Point *p4) {
//=======================================================================
/// \code
/// check if 2 boxes overlap
/// p1-p2 sind Box1, p3-p4 Box2.
/// ACHTUNG: p1 muss links unter p2 liegen; p3 links unter p4.
/// Toleranzen: schon zur Box dazuaddieren.
/// RC -1:   NO, boxes do not overlap
/// RC  1:   yes boxes overlap.
/// \endcode


  // printf(" UT2D_ckBoxinBox1\n");
  // printf("    b1=%f,%f  - %f,%f\n",p1->x,p1->y,p2->x,p2->y);
  // printf("    b2=%f,%f  - %f,%f\n",p3->x,p3->y,p4->x,p4->y);



  if(p3->x > p2->x) return -1;  // b2 ganz rechts
  if(p4->x < p1->x) return -1;  // b2 ganz links

  if(p3->y > p2->y) return -1;  // b2 ueber b1
  if(p4->y < p1->y) return -1;  // b2 unter b1

  if(p3->z > p2->z) return -1;  // b2 ueber b1
  if(p4->z < p1->z) return -1;  // b2 unter b1

  return 1;  // irgendwo overlap

}


//=======================================================================
  int UT2D_pt_ck_inBoxTol (Point2 *p1, Point2 *p2, Point2 *p, double tol) {
//=======================================================================
/// \code
/// UT2D_pt_ck_inBoxTol          check if point p is inside box p1-p2
/// RC = 0:   YES, p is inside box p1 / p2
/// RC = 1:   NO,  p is outside box p1 / p2
/// \endcode


  if(UTP_db_ck_in2dbTol (p->x, p1->x, p2->x, tol) != 0) return 1;

  if(UTP_db_ck_in2dbTol (p->y, p1->y, p2->y, tol) != 0) return 1;

  return 0;

}


//======================================================================
  int UT3D_box_ix_npt (Point *pmin, Point *pmax, int *ia,
                       int nump, Point *ptab) {
//======================================================================
/// UT3D_box_ix_npt     get bounding-box for n points and find extreme points
/// IN:
///   int nump    ... number of points
///   Point *ptab ... points
/// OUT:
///   Point *pmin ... endpoint 1 of box diagonal
///   Point *pmax ... endpoint 2 of box diagonal
///   int   ia[6]     indices of xmin,ymin,zmin,xmax,ymax,zmax-points
///   Returncodes:
///     0 = OK
///    -1 = input error
///
/// UT3D_box_ini0 not necessary
/// \endcode
  
  int   i1;
  
  
  if (nump < 1) {
    printf("UT3D_box_ix_npt: input error\n");
    return -1;
  }
    
  *pmin = ptab[0];
  *pmax = ptab[0];

  for (i1=0; i1<6; ++i1) ia[i1] = 0;

  for (i1=1; i1<nump; ++i1) {
    if (ptab[i1].x < pmin->x) {pmin->x = ptab[i1].x; ia[0] = i1;}
    if (ptab[i1].y < pmin->y) {pmin->y = ptab[i1].y; ia[1] = i1;}
    if (ptab[i1].z < pmin->z) {pmin->z = ptab[i1].z; ia[2] = i1;}
    if (ptab[i1].x > pmax->x) {pmax->x = ptab[i1].x; ia[3] = i1;}
    if (ptab[i1].y > pmax->y) {pmax->y = ptab[i1].y; ia[4] = i1;}
    if (ptab[i1].z > pmax->z) {pmax->z = ptab[i1].z; ia[5] = i1;}
  }

    // printf("ex UT3D_box_ix_npt min-x:%d y:%d z:%d\n",ia[0],ia[1],ia[2]);
    // printf("   UT3D_box_ix_npt max-x:%d y:%d z:%d\n",ia[3],ia[4],ia[5]);

  return 0;

}


//======================================================================
  int UT3D_box_pts (Point *pmin, Point *pmax, int nump, Point *ptab) {
//======================================================================
// UT3D_box_pts         Author: Thomas Backmeister       23.5.2003
// 
/// \code
/// UT3D_box_pts         bounding box <-- points
/// 
/// Computation of a minimal axis-parallel box which contains all given points.
/// The box is given by the endpoints of its diagonal.
/// If all points ly in a plane parallel x,y-plane, the box will be a rectangle.
/// 
/// IN:
///   int nump    ... number of points
///   Point *ptab ... points
/// OUT:
///   Point *pmin ... endpoint 1 of box diagonal
///   Point *pmax ... endpoint 2 of box diagonal
/// Returncodes:
///   0 = OK
///  -1 = input error
/// \endcode


  int i1;

  //printf("UT3D_box_pts: bounding box <-- points\n");

  if (nump < 1) {
    printf("UT3D_box_pts: input error\n");
    return -1;
  }

  *pmin = ptab[0];
  *pmax = ptab[0];
  for (i1=1; i1<nump; ++i1) {
    if (ptab[i1].x < pmin->x) pmin->x = ptab[i1].x;
    if (ptab[i1].y < pmin->y) pmin->y = ptab[i1].y;
    if (ptab[i1].z < pmin->z) pmin->z = ptab[i1].z;
    if (ptab[i1].x > pmax->x) pmax->x = ptab[i1].x;
    if (ptab[i1].y > pmax->y) pmax->y = ptab[i1].y;
    if (ptab[i1].z > pmax->z) pmax->z = ptab[i1].z;
  }

  return 0;
}


//===========================================================================
  int UT3D_box_2pt (Point *pb1, Point *pb2, Point *pt1, Point *pt2) {
//===========================================================================
/// UT3D_box_2pt         make box of 2 points


  // UT3D_stru_dump (Typ_PT, pt1, "UT3D_box_2pt\n");
  // UT3D_stru_dump (Typ_PT, pt2, "  ");


  // if((fabs(pt1->x) > UT_VAL_MAX) ||
     // (fabs(pt1->y) > UT_VAL_MAX) ||
     // (fabs(pt1->z) > UT_VAL_MAX)) {
  // if(isnan(pt1->x)) {
    // printf("***** UT3D_box_2pt I001\n");
    // return 0;
  // }


  if(pt1->x < pt2->x) {
    pb1->x = pt1->x;
    pb2->x = pt2->x;
  } else {
    pb1->x = pt2->x;
    pb2->x = pt1->x;
  }

  if(pt1->y < pt2->y) {
    pb1->y = pt1->y;
    pb2->y = pt2->y;
  } else {
    pb1->y = pt2->y;
    pb2->y = pt1->y;
  }

  if(pt1->z < pt2->z) {
    pb1->z = pt1->z;
    pb2->z = pt2->z;
  } else {
    pb1->z = pt2->z;
    pb2->z = pt1->z;
  }

  // GR_Disp_box (pb1, pb2, 2);

  return 0;

}


//======================================================================
  int UT3D_box_tria (Point *pb1, Point *pb2, Triangle *tr, double tol) {
//======================================================================
/// UT3D_box_tria             make box from triangle


  UT3D_box_2pt (pb1, pb2, tr->pa[0], tr->pa[1]);  // make box of p1,p2
  UT3D_box_extend (pb1, pb2, tr->pa[2]);          // Box mit p3 vergroessern.
  if(tol > 0.)
  UT3D_box_addTol (pb1, pb2, tol);

    // GR_Disp_box (pb1, pb2, 8);

  return 0;

}


//===========================================================================
  int UT3D_box_2pttol (Point *pb1, Point *pb2, Point *pt1, Point *pt2,
                       double tol) {
//===========================================================================
/// UT3D_box_2pttol         make box of 2 points + tol


  // UT3D_stru_dump (Typ_PT, pt1, "UT3D_box_2pt\n");
  // UT3D_stru_dump (Typ_PT, pt2, "");


  if(pt1->x < pt2->x) {
    pb1->x = pt1->x - tol;
    pb2->x = pt2->x + tol;
  } else {
    pb1->x = pt2->x - tol;
    pb2->x = pt1->x + tol;
  }

  if(pt1->y < pt2->y) {
    pb1->y = pt1->y - tol;
    pb2->y = pt2->y + tol;
  } else {
    pb1->y = pt2->y - tol;
    pb2->y = pt1->y + tol;
  }

  if(pt1->z < pt2->z) {
    pb1->z = pt1->z - tol;
    pb2->z = pt2->z + tol;
  } else {
    pb1->z = pt2->z - tol;
    pb2->z = pt1->z + tol;
  }

  // GR_Disp_box (pb1, pb2, 2);

  return 0;

}


//====================================================================
  int UT3D_box_extend (Point* pb1, Point* pb2, Point* pt1) {
//====================================================================
/// \code
/// Box mit point vergroessern.
/// init Box zB mit UT3D_box_ini0
/// \endcode


    // UT3D_stru_dump(Typ_PT, pt1, "UT3D_box_extend");
    // UT3D_stru_dump(Typ_PT, pb1, " _ext pb1i=");
    // UT3D_stru_dump(Typ_PT, pb2, " _ext pb2i=");


  if(pt1->x < pb1->x) pb1->x = pt1->x;
  if(pt1->x > pb2->x) pb2->x = pt1->x;

  if(pt1->y < pb1->y) pb1->y = pt1->y;
  if(pt1->y > pb2->y) pb2->y = pt1->y;

  if(pt1->z < pb1->z) pb1->z = pt1->z;
  if(pt1->z > pb2->z) pb2->z = pt1->z;


    // UT3D_stru_dump(Typ_PT, pb1, " _ext pb1o=");
    // UT3D_stru_dump(Typ_PT, pb2, " _ext pb2o=");

  return 0;

}


//================================================================
  int UT3D_box_addTol (Point *pb1, Point *pb2, double tol) {
//================================================================
/// \code
/// UT3D_box_addTol           add tolerance to box
/// \endcode
// ganz am Ende aufaddieren ist schneller

  pb1->x -= tol;
  pb2->x += tol;

  pb1->y -= tol;
  pb2->y += tol;

  pb1->z -= tol;
  pb2->z += tol;


  return 0;

}


//================================================================
  int UT3D_ck_ptInBox (Point *p1, Point *p2, Point *px) {
//================================================================
/// \code
/// UT3D_ck_ptInBox          check if point px is inside box p1-p2
/// box should have tolerance added; use UT3D_box_addTol
/// RC = 0:   YES, p is inside box p1 / p2
/// RC = 1:   NO,  p is outside box p1 / p2
/// \endcode

  if(px->x < p1->x) return 1;
  if(px->x > p2->x) return 1;

  if(px->y < p1->y) return 1;
  if(px->y > p2->y) return 1;

  if(px->z < p1->z) return 1;
  if(px->z > p2->z) return 1;

  return 0;

}


//=======================================================================
  int UT3D_pt_ck_inBoxTol (Point *p1, Point *p2, Point *p, double tol) {
//=======================================================================
/// \code
/// UT3D_pt_ck_inBoxTol          check if point p is inside box p1-p2
/// RC = 0:   YES, p is inside box p1 / p2
/// RC = 1:   NO,  p is outside box p1 / p2
/// create boxpoints p1,p2 with UT3D_box_2pt()
/// \endcode


  // UT3D_stru_dump (Typ_PT, p, "UT3D_pt_ck_inBoxTol %f pt\n",tol);
  // UT3D_stru_dump (Typ_PT, p1, "  ",tol);
  // UT3D_stru_dump (Typ_PT, p2, "  ",tol);


  if(UTP_db_ck_in2dbTol (p->x, p1->x, p2->x, tol) != 0) return 1;

  if(UTP_db_ck_in2dbTol (p->y, p1->y, p2->y, tol) != 0) return 1;

  if(UTP_db_ck_in2dbTol (p->z, p1->z, p2->z, tol) != 0) return 1;

  return 0;

}



/*
//====================================================================
  int UT3D_box_obj (Point* pb1, Point* pb2, ObjGX *oxi) {
//====================================================================
// Box mit obj vergroessern; Box muss auf Maximum initialisiert sein !
// siehe DL_ReScale_pt_ini
// uses UT3D_box_extend

  int       i1;
  Circ      *cip1;
  SurStd    *sus;
  ObjGX     *ox1, *ox2, *oTab;
  ObjX      xu;


  // printf("UT3D_box_obj typ=%d form=%d siz=%d\n",oxi->typ,oxi->form,oxi->siz);


  if(oxi->typ == Typ_LN) {
    UTO_get_LN (&xu, oxi);
    UT3D_box_extend (pb1, pb2, &xu.ln->p2);



  // man sollte hier SUR-Obj via UTO_objx_get holen (damit DB_Get-Calls weg!)
  } else if(oxi->typ == Typ_SUR) {

    if(oxi->form == Typ_SUR) {
      sus = oxi->data;
      // printf("    in SurStd: typ=%d ind=%d\n",sus->cvTyp,sus->cvID);

      if((sus->cvTyp == Typ_AC)||(sus->cvTyp == Typ_CI)) {
        cip1 = DB_get_CI (sus->cvID);
        UT3D_box_extend (pb1, pb2, &cip1->pc);

      } else if(sus->cvTyp == Typ_CV) {
        ox1 = DB_GetCurv (sus->cvID);
        UT3D_box_obj (pb1, pb2, ox1);  // recu!
      }
    }




  } else if(oxi->typ == Typ_CVCCV) {
    oTab  = oxi->data;
    for(i1=0; i1<oxi->siz; ++i1) {
      ox1 = &oTab[i1];
      UT3D_box_obj (pb1, pb2, ox1);  // recu!
    }

  // } else if(oxi->typ == Typ_CVBSP) {

  }


  return 0;

}
*/


//======================================================================
  int UT3D_box_ck_intLn (Line *ln, Point *pmin, Point *pmax, double tol) {
//======================================================================
// UT3D_box_ck_intLn        Author: Thomas Backmeister       27.5.2003
// 
/// \code
/// UT3D_box_ck_intLn        check line-X with axis-parallel box
/// 
/// Check if a line intersects an axis-parallel box.
/// 
/// IN:
///   Line *ln    ... line
///   Point *pmin ... endpoint 1 of box diagonal
///   Point *pmax ... endpoint 2 of box diagonal
///   double tol  ... tolerance for line intersecting box (>0)
/// OUT:
/// Returncodes:
///   0 = the line does not intersect the box
///   1 = the line intersects the box
/// \endcode



  int rc, i1;
  double a, b, c, d, h, a1, b1, vh;
  Vector vl, vp1, vp2;


  // printf("UT3D_box_ck_intLn: ============================== tol=%f\n",tol);
  // UT3D_stru_dump (Typ_LN, ln, "ln:");
  // UT3D_stru_dump (Typ_PT, pmin, "pmin:");
  // UT3D_stru_dump (Typ_PT, pmax, "pmax:");

  // init
  rc = 1;

  // line vector
  UT3D_vc_ln (&vl, ln);

  // 2 vectors
  UT3D_vc_2pt (&vp1, &(ln->p1), pmin);
  UT3D_vc_2pt (&vp2, &(ln->p1), pmax);

  // check X of line and box
  // - - - - - - - - - - - -
  a = UT_VAL_MIN;
  b = UT_VAL_MAX;
  a1 = vp1.dx;
  b1 = vp2.dx;
  vh = vl.dx;
  for (i1=0; i1<3; ++i1) {
    if (i1 == 1) {
      a1 = vp1.dy;
      b1 = vp2.dy;
      vh = vl.dy;
    }
    else if (i1 == 2) {
      a1 = vp1.dz;
      b1 = vp2.dz;
      vh = vl.dz;
    }
    if (UTP_compdb0 (vh, UT_TOL_min2)) {
      // 0 not in interval [a1,b1] -> no line-box X
      if (b1 < -tol || a1 > tol) {
        rc = 0;
        break;
      }
    }
    else {
      // intersection of [a,b] with [c,d] empty -> no line-box X
      c = a1 / vh;
      d = b1 / vh;
      if (d < c) {
        h = c;
        c = d;
        d = h;
      }
      if (c > a) a = c;
      if (d < b) b = d;
      if (a > (b + tol)) {
        rc = 0;
        break;
      }
    }
  }

  // printf("ex UT3D_box_ck_intLn rc=%d (0=no intersect, 1=yes)\n",rc);


  //================================================================
  // TEST ONLY: test if p1=-1008.122, -142.314,    0.000 is inside box
  // { Point pt1={-1008.122, -142.314,    0.000};
    // if(!UT3D_pt_ck_inBoxTol(pmin,pmax,&pt1,tol)) {
      // printf("-------------- rc=%d\n",rc);
      // UT3D_stru_dump (Typ_PT, pmin, " pmin");
      // UT3D_stru_dump (Typ_PT, pmax, " pmax");
      // GR_Disp_box (pmin, pmax, 0);
    // }
  // }
  //================================================================


  return rc;
}


//===================================================================
  int UT3D_box_ck_intpl (Plane *pln, Point *p1, Point *p2, double tol) {
//===================================================================
/// \code
/// UT3D_box_ck_intpl               check intersect. Plane / Box (estimate!)
/// ACHTUNG: dzt nur rasche Abschaetzung !!!
/// Returncodes:
///   0 = plane does not intersect the box
///   1 = plane intersects the box
/// \endcode

// erweitern: test mit dem Vektor Boxcenter -> Plane, welche Seitenflaeche der
// Box geschnitten wird.
// oder: es muss auf mindestens einer der 3 Hauptachsen (/Y/Z) der vektorTeil
// der Vektor Boxcenter->Plane groesser sein als vom Vektor Boxcenter->BoxCorner.
// 

  int    irc;
  double d1, d2;
  Point  pbc, ppc;
  // Vector vc1, vc2;


  // den Boxmittelpunkt auf die Plane projizieren
  UT3D_pt_mid2pt (&pbc, p1, p2);
  // GR_Disp_pt (&pbc, SYM_TRI_S, 0);

  // UT3D_pt_projptpl (&ppc, pln, &pbc);
  UT3D_pt_intptvcpl_ (&ppc, pln, &pbc, &pln->vz);
  // GR_Disp_pt (&ppc, SYM_TRI_S, 0);

  // UT3D_vc_2pt (&vc1, &pbc, p1);
  // UT3D_vc_2pt (&vc2, &pbc, &ppc);
  // UT3D_stru_dump (Typ_VC, &vc1, "vc1");
  // UT3D_stru_dump (Typ_VC, &vc2, "vc2");

  // BoxSiz = Abst**2 von BoxCenter / BoxEckpunkt
  d1 = UT3D_lenq_PtPt (&pbc, p1) + (tol * tol);
  // Abstand = Abst**2 von BoxCenter / Plane
  d2 = UT3D_lenq_PtPt (&pbc, &ppc);

  if(d1 < d2) irc = 0; // BoxSiz < Abstand = no intersect
  else        irc = 1; // yes

  // printf("ex UT3D_box_ck_intpl %d %f %f\n",irc,d1,d2);

  return irc;

}


//=============================================================================
  int UT3D_cv_boxxy (Point* pb1,double x1,double x2,double y1,double y2) {
//=============================================================================
/// UT3D_cv_boxxy          load rect.points from xmin/xmax ymin/ymax in z=0

// Quadrat aus xmin/xmax ymin/ymax in z=0 setzen.
// 5 punkte !


  // printf("UT3D_cv_boxxy %f %f %f %f\n",x1,x2,y1,y2);


  pb1[0].x = x1;   pb1[0].y = y1;  pb1[0].z = 0.;
  pb1[1].x = x2;   pb1[1].y = y1;  pb1[1].z = 0.;
  pb1[2].x = x2;   pb1[2].y = y2;  pb1[2].z = 0.;
  pb1[3].x = x1;   pb1[3].y = y2;  pb1[3].z = 0.;

  pb1[4] = pb1[0];

  return 0;

}


//================================================================
  int UT3D_ln_intbox (Line *ln1, Point *bp1, Point *bp2) {
//================================================================
/// \code
/// UT3D_ln_intbox            relimit line inside box
/// Line an Raumbox begrenzen
/// \endcode


  int    i1, i2;
  Vector vl;

  // UT3D_stru_dump (Typ_LN, ln1, "UT3D_ln_intbox\n");
  // UT3D_stru_dump (Typ_PT, bp1, "  bp1");
  // UT3D_stru_dump (Typ_PT, bp2, "  bp2");


  i1 = UT3D_pt_ck_inBoxTol (bp1, bp2, &ln1->p1, UT_TOL_pt);
  // printf(" i1=%d\n",i1);

  if(i1 == 1) {  // p1 is outsize; replace p1
    UT3D_vc_2pt (&vl, &ln1->p1, &ln1->p2);

    // p1 mit dem Schnittpunkt ersetzen
    UT3D_ptvc_intbox (&ln1->p1, &vl, bp1, bp2);
  }


  i2 = UT3D_pt_ck_inBoxTol (bp1, bp2, &ln1->p2, UT_TOL_pt);
  // printf(" i2=%d\n",i2);

  if(i2 == 1) {  // p1 is outsize; replace p1
    UT3D_vc_2pt (&vl, &ln1->p2, &ln1->p1);

    // p2 mit dem Schnittpunkt ersetzen
    UT3D_ptvc_intbox (&ln1->p2, &vl, bp1, bp2);
  }


  // UT3D_stru_dump (Typ_LN, ln1, "ex UT3D_ln_intbox\n");

  return 0;

}


//======================================================================
  int UT3D_ptvc_intbox (Point *pl, Vector *vl, Point *bp1, Point *bp2) {
//======================================================================
/// \code
/// UT3D_ptvc_intbox               intersect point/vector with box
/// 
/// Input:
///   pl ist ausserhalb box. vl zeigt in die box hinein.
/// Output:
///   pl ist der Schnittpunkt mit der am naechsten liegenden Boxplane.
/// \endcode

  int   ix, iy, iz;
  Point px, py, pz;


  // printf("UT3D_ptvc_intbox\n");
  // UT3D_stru_dump (Typ_PT, pl, "  pl");
  // UT3D_stru_dump (Typ_VC, vl, "  vl");


  // mit einer X-Plane, einer Y-Plane und einer Z-Plane schneiden


  // Schnitt mit einer X-Plane
  // welche X-Plane liegt naeher an pl->x
  if(pl->x < bp1->x) {       // darunter
    ix = 1;
    UT3D_pt_intptvcplx (&px, pl, vl, bp1->x);

  } else if(pl->x > bp2->x) {       // darueber
    ix = 1;
    UT3D_pt_intptvcplx (&px, pl, vl, bp2->x);

  } else {                          // dazwischen;
    ix = 0;
  }



  // Schnitt mit einer Y-Plane
  // welche Y-Plane liegt naeher an pl->x
  if(pl->y < bp1->y) {       // darunter
    iy = 1;
    UT3D_pt_intptvcply (&py, pl, vl, bp1->y);

  } else if(pl->y > bp2->y) {       // darueber
    iy = 1;
    UT3D_pt_intptvcply (&py, pl, vl, bp2->y);

  } else {                          // dazwischen;
    iy = 0;
  }


  // Schnitt mit einer Z-Plane
  // welche Z-Plane liegt naeher an pl->z
  if(pl->z < bp1->z) {       // darunter
    iz = 1;
    UT3D_pt_intptvcplz (&pz, pl, vl, bp1->z);

  } else if(pl->z > bp2->z) {       // darueber
    iz = 1;
    UT3D_pt_intptvcplz (&pz, pl, vl, bp2->z);

  } else {                          // dazwischen; kein Z-inters.Pt.
    iz = 0;
  }


  // if(px->x > AP_spc_p2->x) ..


  // nur einer der 3 Schnittpunkte befindet sich innerhalb der Box;
  // diesen wahlen ....
  if(ix > 0) {
    if(UT3D_pt_ck_inBoxTol (bp1, bp2, &px, UT_TOL_pt) == 0) {  // inside
      *pl = px;
      goto L_fertig;
    }
  }

  if(iy > 0) {
    if(UT3D_pt_ck_inBoxTol (bp1, bp2, &py, UT_TOL_pt) == 0) {  // inside
      *pl = py;
      goto L_fertig;
    }
  }

  if(iz > 0) {
    if(UT3D_pt_ck_inBoxTol (bp1, bp2, &pz, UT_TOL_pt) == 0) {  // inside
      *pl = pz;
      goto L_fertig;
    }
  }


  L_fertig:
  // UT3D_stru_dump (Typ_PT, pl, "ex UT3D_ptvc_intbox");

  return 0;

}


//====================== EOF ===========================
