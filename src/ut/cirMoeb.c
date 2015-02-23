//**************************************************************************
/*
 *
 * Copyright (C) 2015 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
 * Copyright (C) 2007 Thomas Backmeister (t.backmeister@gmx.at)
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
2007-11-05 Neu: UCV_FilterTouch3MCirs.
2007-10-25 Neu: UCV_Touch3MoebCirs, UCV_Touch3MCirsCase4, UCV_TouchCi2Lns.
2007-10-24 Neu: UCV_Touch2ConcCiMoebCir, UCV_Touch3MCirsCase3, UCV_Touch3Lns.
2007-10-22 Neu: UCV_Touch3MCirsCase2.
2007-10-15 Neu: UCV_TouchLnLnMoebCir, UCV_Touch3MCirsCase1, UCV_DispMoebCir,
                UCV_LnTouchLnMoebCir.
2007-10-09 Neu: UCV_CiInvertMoebCir.
2007-10-04 Neu: Konstante UT2D_PT_INFTY; UCV_IntSectMoebCirs.
2007-10-03 Neu: UCV_CasesApollConfig.

-----------------------------------------------------
*/
/*!
\file  ../ut/cirMoeb.c
\brief curves (bsplines, conic sections, etc.)
\code
=====================================================
List_functions_start:

UCV_CasesApollConfig     Cases <-- Apollonian configuration
UCV_FilterTouch3MCirs    Filter <-- touching Moebius circles
UCV_IntSectMoebCirs      Intersection <-- two Moebius circles
UCV_CiInvertMoebCir      Invert at circle <-- Moebius circle
UCV_LnTouchLnMoebCir     Touching lines <-- 1 line, 1 M-cir
UCV_OffSurCrv            Offset surface curve <-- curve
UCV_TouchCi2Lns          Touching circles <-- 1 circle & 2 unparallel lines
UCV_TouchLnLnMoebCir     Touching M-cirs <-- 2 parallel lines, 1 M-cir
UCV_Touch2ConcCiMoebCir  Touching M-cirs <-- 2 concentric M-cirs, 1 M-cir
UCV_Touch3Lns            Touching circles <-- 3 pairwise unparallel lines
UCV_Touch3MCirsCase1     Touching Moebius circles <-- 3 M-circles - case 1
UCV_Touch3MCirsCase2     Touching Moebius circles <-- 3 M-circles - case 2
UCV_Touch3MCirsCase3     Touching Moebius circles <-- 3 M-circles - case 3
UCV_Touch3MCirsCase4     Touching Moebius circles <-- 3 M-circles - case 4
UCV_Touch3MoebCirs       Touching Moebius circles <-- three Moebius circles

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



Returncodes:
 0 OK
>0 OK with additional info
<0 Error with additional info


compile with gcc:
cc -c -O0 -g3 -Wall ucv.c

Windows:
cl -c ucv.c

ctags -f ucv.tag ucv.c

*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_geo.h"
#include "../ut/ut_ox_base.h"             // OGX_SET_INDEX
#include "../gr/ut_UI.h"                  // Symbols
#include "../ut/ut_math.h"
#include "../ut/ut_TX.h"
// #include "../ubs/ubscrv.h"
// #include "../umt/umt.h"
// #include "../u2d/u2d.h"
// #include "ucv.h"

#include "../ut/utemp.h"

// includes from ../ut/ut_geo_const.h
extern const Point2  UT2D_PT_INFTY;






//======================================================================
  int UCV_Touch3MCirsCase4 (ObjGX *oxtT, Memspc *memSeg, ObjGX *oxT,
		            Point2 *xp, Memspc *workSeg) {
//======================================================================
// UCV_Touch3MCirsCase4     Author: Thomas Backmeister      25.10.2007
// 
/// \code
/// UCV_Touch3MCirsCase4    Touching Moebius circles <-- 3 M-circles - case 4
/// 
/// Compute all 8 Moebius circles except points which touch three given
/// Moebius circles in case 4 - Apollonian configuration:
/// any two elements are intersecting in two different points &
/// all three elements have no common point.
/// A Moebius circle is a circle, line or finite point in
/// (xy-plane & UT2D_PT_INFTY).
/// 
/// IN:
///   ObjGX *oxT       ... 3 Moebius circles (in case 4 - Apoll config)
///                        ([0]...circle if xp == UT2D_PT_INFTY, element
/// 		              not incident with xp if xp == finite)
///   Point2 *xp       ... one of the intersection points
///                        (=UT2D_PT_INFTY if existing) 
///   Memspc *workSeg  ... workspace
/// OUT:
///   ObjGX *oxtT      ... touching Moebius circles (8)
///   Memspc *memSeg   ... space for oxtT[i].data, i=0,1,...
///                        and workspace
/// Returncodes:
///   0 = OK
///  -1 = internal error
/// \endcode


  int rc, rcT[3], i1;	  
  void *memStart, *workStart;
  ObjGX oyT[3], oytT[8];

  //printf("UCV_Touch3MCirsCase4: Touching M-cirs <-- 3 M-cirs - case 4\n");

  if (!UT2D_comp2pt (xp, &UT2D_PT_INFTY, UT_TOL_pt)) {
    // all intersection points are FINITE
    // ..................................

    // save start of memspaces
    memStart = memSeg->next;
    workStart = workSeg->next;
      
    // invert Moebius circles	    
    for (i1=0; i1<3; ++i1) {
      rcT[i1] = UCV_CiInvertMoebCir (&oyT[i1], memSeg, &oxT[i1], xp, 1.0);
      if (rcT[i1] != 0) return -1;
    }	

    // touching circles <-- circle, line, line
    rc = UCV_TouchCi2Lns (oytT, workSeg, oyT);
    if (rc < 0) return -1;

    // release memspace
    memSeg->next = memStart;

    // invert touching Moebius circles
    for (i1=0; i1<8; ++i1) {
      rc = UCV_CiInvertMoebCir (&oxtT[i1], memSeg, &oytT[i1], xp, 1.0);
      if (rc < 0) return -1;
    }	

    // release workspace
    workSeg->next = workStart;
  }
  else {
    // one intersection point is INFINITE
    // ..................................

    // touching circles <-- circle, line, line
    rc = UCV_TouchCi2Lns (oxtT, memSeg, oxT);
    if (rc < 0) return -1;
  }	    

  return 0;
}


//======================================================================
  int UCV_Touch3MCirsCase3 (ObjGX *oxtT, Memspc *memSeg, ObjGX *oxT,
		            Point2 *tp, Memspc *workSeg) {
//======================================================================
// UCV_Touch3MCirsCase3     Author: Thomas Backmeister      24.10.2007
// 
/// \code
/// UCV_Touch3MCirsCase3    Touching Moebius circles <-- 3 M-circles - case 3
/// 
/// Compute all 4 Moebius circles except points which touch three given
/// Moebius circles in case 3 - Apollonian configuration:
/// any two elements are intersecting in two different points &
/// all three elements have 1 common point.
/// A Moebius circle is a circle, line or finite point in
/// (xy-plane & UT2D_PT_INFTY).
/// 
/// IN:
///   ObjGX *oxT       ... 3 Moebius circles (in case 3 - Apoll config)
///   Point2 *tp       ... common point of all 3 elements
///   Memspc *workSeg  ... workspace
/// OUT:
///   ObjGX *oxtT      ... touching Moebius circles (4)
///   Memspc *memSeg   ... space for oxtT[i].data, i=0,1,...
///                        and workspace
/// Returncodes:
///   0 = OK
///  -1 = internal error
/// \endcode


  int rc, rcT[3], i1;	  
  ObjGX oyT[3], oytT[8];
  void *memStart, *workStart;

  //printf("UCV_Touch3MCirsCase3: Touching M-cirs <-- 3 M-cirs - case 3\n");

  if (!UT2D_comp2pt (tp, &UT2D_PT_INFTY, UT_TOL_pt)) {
    // FINITE common point
    // ................... 

    // save start of memspaces
    memStart = memSeg->next;
    workStart = workSeg->next;
      
    // invert Moebius circles	    
    for (i1=0; i1<3; ++i1) {
      rcT[i1] = UCV_CiInvertMoebCir (&oyT[i1], memSeg, &oxT[i1], tp, 1.0);
      if (rcT[i1] != 0) return -1;
    }	

    // touching circles <-- 3 pairwise unparallel lines
    rc = UCV_Touch3Lns (oytT, workSeg, oyT);
    if (rc < 0) return -1;

    // release memspace
    memSeg->next = memStart;

    // invert touching Moebius circles
    for (i1=0; i1<4; ++i1) {
      rc = UCV_CiInvertMoebCir (&oxtT[i1], memSeg, &oytT[i1], tp, 1.0);
      if (rc < 0) return -1;
    }	

    // release workspace
    workSeg->next = workStart;
  }
  else {
    // INFINITE common point
    // .....................

    // touching circles <-- 3 pairwise unparallel lines
    rc = UCV_Touch3Lns (oxtT, memSeg, oxT);
    if (rc < 0) return -1;
  }	    

  return 0;
}


//======================================================================
  int UCV_Touch3MCirsCase2 (int *nrt, ObjGX *oxtT, Memspc *memSeg,
		            ObjGX *oxT, Memspc *workSeg) {
//======================================================================
// UCV_Touch3MCirsCase2     Author: Thomas Backmeister      16.10.2007
// 
/// \code
/// UCV_Touch3MCirsCase2    Touching Moebius circles <-- 3 M-circles - case 2
/// 
/// Compute all Moebius circles except points (at most 8) which touch
/// three given Moebius circles in case 2 - Apollonian configuration:
/// there is no pair of touching elements &
/// there are two elements without common point.
/// A Moebius circle is a circle, line or finite point in
/// (xy-plane & UT2D_PT_INFTY).
/// 
/// IN:
///   ObjGX *oxT       ... 3 Moebius circles (in case 2 - Apoll config)
///                        ([0],[1]...2 elements without common point)
///   Memspc *workSeg  ... workspace
/// OUT:
///   int *nrt         ... number of touching Moebius circles (<=8)
///   ObjGX *oxtT      ... touching Moebius circles (8)
///   Memspc *memSeg   ... space for oxtT[i].data, i=0,1,...
///                        and workspace
/// Returncodes:
///   0 = OK
///  -1 = internal error
///  -2 = input error
///  -3 = out of space
/// \endcode


  int rc, rcT[3], i1, inv;	  
  ObjGX oyT[3], oytT[8];
  Point2 pt, E, F, *p1, *p2, *p3;
  Vector2 vc;
  Circ2 *ci1, *ci2;
  Line2 *ln1, *ln2;
  void *memStart, *workStart;

  //printf("UCV_Touch3MCirsCase2: Touching M-cirs <-- 3 M-cirs - case 2\n");

  // save start of memspaces
  memStart = memSeg->next;
  workStart = workSeg->next;

  // compute center E of inversion which transforms the first
  // two Moebius circles to CONCENTRIC Moebius circles
  F = UT2D_PT_NUL;
  if (oxT[0].typ == Typ_CI2) {
    ci1 = (Circ2*)oxT[0].data;
    if (oxT[1].typ == Typ_CI2) {
      rc = UT2D_FixPtsProd2Invs (&E, &F, &oxT[0], &oxT[1]);
      if (rc < 0) return -1;
    }
    else if (oxT[1].typ == Typ_LN2) {
      rc = UT2D_FixPtsProd2Invs (&E, &F, &oxT[0], &oxT[1]);
      if (rc < 0) return -1;
    }
    else {
      p2 = (Point2*)oxT[1].data;
      if (UT2D_comp2pt (p2, &ci1->pc, UT_TOL_min1)) F = UT2D_PT_INFTY;
      else E = UT2D_CiInvertPoint (p2, &ci1->pc, fabs(ci1->rad));
    }	    
  }
  else if (oxT[0].typ == Typ_LN2) {
    ln1 = (Line2*)oxT[0].data;
    if (oxT[1].typ == Typ_CI2) {
      rc = UT2D_FixPtsProd2Invs (&E, &F, &oxT[1], &oxT[0]);
      if (rc < 0) return -1;
    }
    else if (oxT[1].typ == Typ_LN2) goto L_InErr;
    else {
      p2 = (Point2*)oxT[1].data;
      E = UT2D_LnInvertPoint (p2, ln1);
    }	    
  }
  else {
    p1 = (Point2*)oxT[0].data;
    if (oxT[1].typ == Typ_CI2) {
      ci2 = (Circ2*)oxT[1].data;
      if (UT2D_comp2pt (p1, &ci2->pc, UT_TOL_min1)) F = UT2D_PT_INFTY;
      else E = UT2D_CiInvertPoint (p1, &ci2->pc, fabs(ci2->rad));
    }
    else if (oxT[1].typ == Typ_LN2) {
      ln2 = (Line2*)oxT[1].data;
      E = UT2D_LnInvertPoint (p1, ln2);
    }
    else {
      p2 = (Point2*)oxT[1].data;
      if (oxT[2].typ == Typ_PT2) {
        p3 = (Point2*)oxT[2].data;

	      // check collinearity of p1,p2,p3
        UT2D_vc_2pt (&vc, p1, p2);	      
        rc = UT2D_pt_projptptvc (&pt, p3, p1, &vc);

        if (rc == 1) {
          // solution = line through points		
          ln1 = memSeg->next;
          rc = UME_add (memSeg, sizeof(Line2));
          if (rc < 0) goto L_outOfSpace;
          UT2D_ln_ptvc (ln1, p1, &vc);
          OGX_SET_OBJ (&oxtT[0], Typ_LN2, Typ_LN2, 1, ln1);
        }
        else {	
          // solution = circle through points		
          ci1 = memSeg->next;
          rc = UME_add (memSeg, sizeof(Circ2));
          if (rc < 0) goto L_outOfSpace;
          rc = UT2D_Ci3Pts (ci1, p1, p2, p3);
          if (rc < 0) return -1;
          OGX_SET_OBJ (&oxtT[0], Typ_CI2, Typ_CI2, 1, ci1);
        }
        *nrt = 1;
      }
      else {
        oyT[0] = oxT[2];
        oyT[1] = oxT[0];
        oyT[2] = oxT[1];

        // oyT[0] and oyT[1] have no common point and not both are points!
        // compute touching Moebius circles (at most 8)
        rc = UCV_Touch3MCirsCase2 (nrt, oxtT, memSeg, oyT, workSeg);
        if (rc < 0) return -1;
      }	      
      goto L_Done;
    }	    
  }	  
      
  inv = 0;
  if (UT2D_comp2pt (&F, &UT2D_PT_INFTY, UT_TOL_pt)) {
    // the first two M-circles are CONCENTRIC M-circles	    
    for (i1=0; i1<3; ++i1) {
      oyT[i1] = oxT[i1];  
      rcT[i1] = 0;
    }      
  }
  else {
    // invert Moebius circles;
    // the first two M-circles turn into CONCENTRIC M-circles	    
    inv = 1;	  
    for (i1=0; i1<3; ++i1) {
      rcT[i1] = UCV_CiInvertMoebCir (&oyT[i1], memSeg, &oxT[i1], &E, 1.0);
      if (rcT[i1] < 0) return -1;
    }
  }	  

  if (rcT[2] == 1) {
    // no solution <-- 3. element is infinite point	  
    memSeg->next = memStart;		
    *nrt = 0;
    return 0;
  }
  else {
    // touching M-circles <-- 2 concentric M-circles, 1 M-circle
    if (inv == 0) {	  
      rc = UCV_Touch2ConcCiMoebCir (nrt, oxtT, memSeg, oyT);
      if (rc < 0) return -1;
    }
    else {
      rc = UCV_Touch2ConcCiMoebCir (nrt, oytT, workSeg, oyT);
      if (rc < 0) return -1;
    }	    
  }

  if (inv == 1) {
    // invert touching Moebius circles
    memSeg->next = memStart;		
    for (i1=0; i1<(*nrt); ++i1) {
      rc = UCV_CiInvertMoebCir (&oxtT[i1], memSeg, &oytT[i1], &E, 1.0);
      if (rc < 0) return -1;
    }	
    workSeg->next = workStart;
  }	  

L_Done:  
  return 0;

L_InErr:
  TX_Error("UCV_Touch3MCirsCase2: input error");
  return -2;

L_outOfSpace:
  TX_Error("UCV_Touch3MCirsCase2: out of space");
  return -3;
}


//======================================================================
  int UCV_Touch3MCirsCase1 (int *nrt, ObjGX *oxtT, Memspc *memSeg,
		            ObjGX *oxT, Point2 *tp, Memspc *workSeg) {
//======================================================================
// UCV_Touch3MCirsCase1     Author: Thomas Backmeister      15.10.2007
// 
/// \code
/// UCV_Touch3MCirsCase1    Touching Moebius circles <-- 3 M-circles - case 1
/// 
/// Compute all Moebius circles except points (at most 6) which touch
/// three given Moebius circles in case 1 - Apollonian configuration:
/// there are two elements touching each other.
/// A Moebius circle is a circle, line or finite point in
/// (xy-plane & UT2D_PT_INFTY).
/// 
/// IN:
///   ObjGX *oxT       ... 3 Moebius circles (in case 1 - Apoll config)
///   Point2 *tp       ... touch point of M-cirs [0],[1]
///   Memspc *workSeg  ... workspace
/// OUT:
///   int *nrt         ... number of touching Moebius circles (<=6)
///   ObjGX *oxtT      ... touching Moebius circles (6)
///   Memspc *memSeg   ... space for oxtT[i].data, i=0,1,...
///                        and workspace
/// Returncodes:
///   0 = OK
///  -1 = internal error
///  -2 = out of space
/// \endcode


  int rc, rcT[3], i1;	  
  ObjGX oyT[3], oytT[8];
  void *memStart, *workStart;

  //printf("UCV_Touch3MCirsCase1: Touching M-cirs <-- 3 M-cirs - case 1\n");

  // init
  *nrt = 0;

  if (!UT2D_comp2pt (tp, &UT2D_PT_INFTY, UT_TOL_pt)) {
    // FINITE touchpoint
    // ................. 

    // save start of memspaces
    memStart = memSeg->next;
    workStart = workSeg->next;
      
    // invert Moebius circles	    
    for (i1=0; i1<3; ++i1) {
      rcT[i1] = UCV_CiInvertMoebCir (&oyT[i1], memSeg, &oxT[i1], tp, 1.0);
      if (rcT[i1] < 0) return -1;
    }	
    if (rcT[2] == 1) {
      memSeg->next = memStart;	    
      return 0;
    }  

    if (rcT[0] != 1 && rcT[1] != 1) {
      // touching M-circles <-- 2 lines, 1 M-circle
      rc = UCV_TouchLnLnMoebCir (nrt, oytT, workSeg, oyT);
      if (rc < 0) return -1;
    }
    else {
      // touching lines <-- 1 line, 1 M-circle
      if (rcT[0] == 1) {	      
        rc = UCV_LnTouchLnMoebCir (nrt, oytT, workSeg, &oyT[1], &oyT[2]);
        if (rc < 0) return -1;
      }  
      else {	      
        rc = UCV_LnTouchLnMoebCir (nrt, oytT, workSeg, &oyT[0], &oyT[2]);
        if (rc < 0) return -1;
      }  
    }	      
    memSeg->next = memStart;

    // invert touching Moebius circles
    for (i1=0; i1<(*nrt); ++i1) {
      rc = UCV_CiInvertMoebCir (&oxtT[i1], memSeg, &oytT[i1], tp, 1.0);
      if (rc < 0) return -1;
    }	
    workSeg->next = workStart;
  }
  else {
    // INFINITE touchpoint
    // ...................

    // touching M-circles <-- 2 lines, 1 M-circle
    rc = UCV_TouchLnLnMoebCir (nrt, oxtT, memSeg, oxT);
    if (rc < 0) return -1;
  }	    

  return 0;

L_outOfSpace:
  TX_Error("UCV_Touch3MCirsCase1: out of space");
  return -2;
}


//======================================================================
  int UCV_TouchCi2Lns (ObjGX *oxtT, Memspc *memSeg, ObjGX *oxT) {
//======================================================================
// UCV_TouchCi2Lns    Author: Thomas Backmeister       25.10.2007
// 
/// \code
/// UCV_TouchCi2Lns   Touching circles <-- 1 circle & 2 unparallel lines
/// 
/// Compute all eight circles which touch a circle and two unparallel lines.
/// It is supposed that any two elements are intersecting in two different
/// points (meaning for the lines not to be parallel) and that all three
/// elements have no common point.
/// 
/// IN:
///   ObjGX *oxT      ... circle ([0]) and two lines
/// OUT:
///   ObjGX *oxtT     ... touching circles (8)
///   Memspc *memSeg  ... space for oxtT[i].data, i=0,1,...
/// Returncodes:
///   0 = OK
///  -1 = internal error
///  -2 = input error
///  -3 = out of space
/// \endcode


  int rc, i1, i2, j1, nrt;	  
  double r1;
  Circ2 *ci1, *ci, cT[2];
  Line2 *ln2, *ln3, gT[2], hT[2];
  Vector2 v2, v3;
  Point2 pt;

  //printf("UCV_TouchCi2Lns: Touching circles <-- 1 circle & 2 unpar. lines\n");
   
  // check input & get elements
  if (oxT[0].typ != Typ_CI2) return -2;
  ci1 = (Circ2*)oxT[0].data;
  if (oxT[1].typ != Typ_LN2) return -2;
  ln2 = (Line2*)oxT[1].data;
  if (oxT[2].typ != Typ_LN2) return -2;
  ln3 = (Line2*)oxT[2].data;

  // linevectors
  UT2D_vc_ln (&v2, ln2);
  UT2D_vc_ln (&v3, ln3);

  // parallel lines to ln2 at distance ci1->rad
  UT2D_pt_tranor2ptlen (&pt, &ln2->p1, &ln2->p2, fabs(ci1->rad));
  UT2D_ln_ptvc (&gT[0], &pt, &v2);
  UT2D_pt_tranor2ptlen (&pt, &ln2->p1, &ln2->p2, -fabs(ci1->rad));
  UT2D_ln_ptvc (&gT[1], &pt, &v2);

  // parallel lines to ln3 at distance ci1->rad
  UT2D_pt_tranor2ptlen (&pt, &ln3->p1, &ln3->p2, fabs(ci1->rad));
  UT2D_ln_ptvc (&hT[0], &pt, &v3);
  UT2D_pt_tranor2ptlen (&pt, &ln3->p1, &ln3->p2, -fabs(ci1->rad));
  UT2D_ln_ptvc (&hT[1], &pt, &v3);

  nrt = 0;
  for (i1=0; i1<2; ++i1) {
    for (i2=0; i2<2; ++i2) {
	     
      // 2 circles through ci1->pc & touching lines gT[i1], hT[i2]	    
      rc = UT2D_TouchLnLnPt (&cT[0], &cT[1], &gT[i1], &hT[i2], &(ci1->pc));
      if (rc < 0) return -1;

      // 2 circles touching ci1, ln2, ln3
      for (j1=0; j1<2; ++j1) {
        ci = memSeg->next;
        rc = UME_add (memSeg, sizeof(Circ2));
        if (rc < 0) goto L_outOfSpace;
	r1 = fabs(fabs(ci1->rad) - fabs(cT[j1].rad));
        rc = UT2D_ci_ptrd (ci, &(cT[j1].pc), r1);
        if (rc < 0) return -1;
        OGX_SET_OBJ (&(oxtT[nrt]), Typ_CI2, Typ_CI2, 1, ci);
	++nrt;
      }
    }
  }    

  return 0;

L_outOfSpace:
  TX_Error("UCV_TouchCi2Lns: out of space");
  return -3;
}


//======================================================================
  int UCV_Touch3Lns (ObjGX *oxtT, Memspc *memSeg, ObjGX *oxT) {
//======================================================================
// UCV_Touch3Lns    Author: Thomas Backmeister       24.10.2007
// 
/// \code
/// UCV_Touch3Lns   Touching circles <-- 3 pairwise unparallel lines
/// 
/// Compute all four circles which touch three pairwise unparallel lines.
/// 
/// IN:
///   ObjGX *oxT      ... 3 pairwise unparallel lines
/// OUT:
///   ObjGX *oxtT     ... touching circles (4)
///   Memspc *memSeg  ... space for oxtT[i].data, i=0,1,...
/// Returncodes:
///   0 = OK
///  -1 = internal error
///  -2 = input error
///  -3 = out of space
/// \endcode


  int rc, i1;	  
  double rT[4];	  
  Line2 *ln1, *ln2, *ln3;
  Vector2 v1, v2, v3, v12, v23, v12p, v23p;
  Point2 A, B, IT[4], pp;
  Circ2 *ci;


  // printf("UCV_Touch3Lns: Touching circles <-- 3 pairwise unpar. lines\n");
  // UT3D_stru_dump(Typ_ObjGX, &oxT[0], "o[0]");
  // UT3D_stru_dump(Typ_ObjGX, &oxT[1], "o[1]");
  // UT3D_stru_dump(Typ_ObjGX, &oxT[2], "o[2]");

   
  // check input & get lines
  if (oxT[0].typ != Typ_LN2) goto L_InErr;
  ln1 = (Line2*)oxT[0].data;
  if (oxT[1].typ != Typ_LN2) goto L_InErr;
  ln2 = (Line2*)oxT[1].data;
  if (oxT[2].typ != Typ_LN2) goto L_InErr;
  ln3 = (Line2*)oxT[2].data;

  // linevectors
  UT2D_vc_ln (&v1, ln1);
  UT2D_vc_ln (&v2, ln2);
  UT2D_vc_ln (&v3, ln3);

  // middle vectors of v1,v2 and v2,v3
  UT2D_vc_mid2vc (&v12, &v1, &v2);
  UT2D_vc_mid2vc (&v23, &v2, &v3);

  // perpendicular vectors to v12,v23
  UT2D_vc_perpvc (&v12p, &v12);
  UT2D_vc_perpvc (&v23p, &v23);

  // intersection of lines 1,2 and 2,3
  rc = UT2D_pt_int2ln (&A, ln1, ln2);
  if (rc < 1) goto L_InErr;
  rc = UT2D_pt_int2ln (&B, ln2, ln3);
  if (rc < 1) goto L_InErr;

  // centerpoints of 4 circles which touch all 3 lines
  UT2D_pt_int2pt2vc (&IT[0], &A, &v12, &B, &v23);
  // if (rc != 1) goto L_InErr;
  UT2D_pt_int2pt2vc (&IT[1], &A, &v12p, &B, &v23);
  // if (rc != 1) goto L_InErr;
  UT2D_pt_int2pt2vc (&IT[2], &A, &v12, &B, &v23p);
  // if (rc != 1) goto L_InErr;
  UT2D_pt_int2pt2vc (&IT[3], &A, &v12p, &B, &v23p);
  // if (rc != 1) goto L_InErr;

  // radiuses of 4 circles which touch all 3 lines
  UT2D_pt_projptptvc (&pp, &IT[0], &(ln1->p1), &v1);
  rT[0] = UT2D_len_2pt (&IT[0], &pp);
  UT2D_pt_projptptvc (&pp, &IT[1], &(ln1->p1), &v1);
  rT[1] = UT2D_len_2pt (&IT[1], &pp);
  UT2D_pt_projptptvc (&pp, &IT[2], &(ln1->p1), &v1);
  rT[2] = UT2D_len_2pt (&IT[2], &pp);
  UT2D_pt_projptptvc (&pp, &IT[3], &(ln1->p1), &v1);
  rT[3] = UT2D_len_2pt (&IT[3], &pp);

  // create touching circles
  for (i1=0; i1<4; ++i1) {
    ci = memSeg->next;
    rc = UME_add (memSeg, sizeof(Circ2));
    if (rc < 0) goto L_outOfSpace;
    rc = UT2D_ci_ptrd (ci, &IT[i1], rT[i1]);
    if (rc < 0) return -1;
    OGX_SET_OBJ (&oxtT[i1], Typ_CI2, Typ_CI2, 1, ci);
  }

  return 0;

L_InErr:
  TX_Error("UCV_Touch3Lns: input error");
  return -2;

L_outOfSpace:
  TX_Error("UCV_Touch3Lns: out of space");
  return -3;
}


//======================================================================
  int UCV_Touch2ConcCiMoebCir (int *nrt, ObjGX *oxtT, Memspc *memSeg,
		               ObjGX *oxT) {
//======================================================================
// UCV_Touch2ConcCiMoebCir    Author: Thomas Backmeister       22.10.2007
// 
/// \code
/// UCV_Touch2ConcCiMoebCir   Touching M-cirs <-- 2 concentric M-cirs, 1 M-cir
/// 
/// Compute all Moebius circles except points (at most 8) which touch
/// two concentric and one Moebius circle.
/// A Moebius circle is a circle, line or finite point in
/// (xy-plane & UT2D_PT_INFTY).
/// All three elements are supposed to be different!
/// One of the two concentric Moebius circles might be a point (=circlecenter).
/// 
/// IN:
///   ObjGX *oxT      ... 2 concentric M-cirs ([0],[1]) and 1 M-cir [2]
/// OUT:
///   int *nrt        ... number of touching Moebius circles
///   ObjGX *oxtT     ... touching Moebius circles (8)
///   Memspc *memSeg  ... space for oxtT[i].data, i=0,1,...
/// Returncodes:
///   0 = OK
///  -1 = internal error
///  -2 = input error
///  -3 = out of space
/// \endcode


  int rc, n1, i1, i2;	  
  double rT[2], sT[2], rd, d1, radT[8];
  Circ2 *ci1, *ci2, *ci3, *ci;	  
  Line2 *ln;
  Point2 *pt2, *pt, p1, p2, pT[8], qt;
  Vector2 vl;

  //printf("UCV_Touch2ConcCiMoebCir: Touch M-cirs <-- 2 conc. M-cirs, 1 M-cir\n");
   
  // init
  *nrt = 0;

  // get concentric Moebius circles
  // (ci1,ci2 or ci1,pt2 with radiuses rT[0] > rT[1])
  if (oxT[0].typ == Typ_CI2) {
    ci1 = (Circ2*)oxT[0].data;	  
    rT[0] = fabs(ci1->rad);	  
    if (oxT[1].typ == Typ_CI2) {
      ci2 = (Circ2*)oxT[1].data;	  
      rT[1] = fabs(ci2->rad);	
      if (rT[1] > rT[0]) {
        ci1 = (Circ2*)oxT[1].data;
	rT[0] = fabs(ci1->rad);
        ci2 = (Circ2*)oxT[0].data;
	rT[1] = fabs(ci2->rad);
      }	      
    }
    else if (oxT[1].typ == Typ_PT2) {
      pt2 = (Point2*)oxT[1].data;
      rT[1] = 0.0;
    }
    else goto L_InErr;    
  }
  else if (oxT[0].typ == Typ_PT2) {
    pt2 = (Point2*)oxT[0].data;
    rT[1] = 0.0;
    if (oxT[1].typ == Typ_CI2) {
      ci1 = (Circ2*)oxT[1].data;	  
      rT[0] = fabs(ci1->rad);	  
    }	    
    else goto L_InErr;
  }
  else goto L_InErr;

  // 1 or 2 concentric circles about ci1->pc with radiuses sT[0],sT[1] 
  sT[0] = 0.5 * (rT[0] + rT[1]);
  sT[1] = 0.5 * (rT[0] - rT[1]);
  n1 = 1;
  if (!UTP_comp_0 (rT[1])) n1 = 2;

  // ----------------------------------------------------
  // compute centerpoint & radius of all touching circles
  // ----------------------------------------------------

  if (oxT[2].typ == Typ_CI2) {
    // -------------------	  
    // 3. element = circle
    // -------------------	  

    // get circle
    ci3 = (Circ2*)oxT[2].data;	  

    for (i1=0; i1<n1; ++i1) {

      // intersect circle (ci1->pc,rad=sT[i1]) with
      // circle (ci3->pc,rad=rd)
      rd = fabs(ci3->rad) + sT[(i1+1)%2];	    
      rc = UT2D_2pt_intcici (&p1, &p2, &(ci1->pc), sT[i1], &(ci3->pc), rd);
      if (rc == 0) {
        pT[*nrt] = p1; radT[*nrt] = sT[(i1+1)%2]; ++(*nrt);	    
        pT[*nrt] = p2; radT[*nrt] = sT[(i1+1)%2]; ++(*nrt);	    
      }	    
      else if (rc == 1) {
        pT[*nrt] = p1; radT[*nrt] = sT[(i1+1)%2]; ++(*nrt);	    
      }	    

      rd = fabs(fabs(ci3->rad) - sT[(i1+1)%2]);
      if (rd > UT_TOL_min1) {
        // intersect circle (ci1->pc,rad=sT[i1]) with
	// circle (ci3->pc,rad=rd)
        rc = UT2D_2pt_intcici (&p1, &p2, &(ci1->pc), sT[i1], &(ci3->pc), rd);
        if (rc == 0) {
          pT[*nrt] = p1; radT[*nrt] = sT[(i1+1)%2]; ++(*nrt);	    
          pT[*nrt] = p2; radT[*nrt] = sT[(i1+1)%2]; ++(*nrt);	    
        }	    
        else if (rc == 1) {
          pT[*nrt] = p1; radT[*nrt] = sT[(i1+1)%2]; ++(*nrt);	    
        }	    
      }	    
    }
  }

  else if (oxT[2].typ == Typ_LN2) {
    // -----------------	  
    // 3. element = line
    // -----------------	  

    // get line & linevector	  
    ln = (Line2*)oxT[2].data;	 
    UT2D_vc_ln (&vl, ln); 

    for (i1=0; i1<n1; ++i1) {
      d1 = sT[(i1+1)%2];
      for (i2=0; i2<2; ++i2) {
	if (i2 == 1) d1 = -d1;

        // parallel line to ln at distance d1
        UT2D_pt_tranor2ptlen (&qt, &ln->p1, &ln->p2, d1);

        // intersect circle (ci1->pc,rad=sT[i1]) with parallel line
        rc = UT2D_2pt_intciptvc (&p1, &p2, &(ci1->pc), sT[i1], &qt, &vl);
        if (rc == 0) {
          pT[*nrt] = p1; radT[*nrt] = sT[(i1+1)%2]; ++(*nrt);	    
          pT[*nrt] = p2; radT[*nrt] = sT[(i1+1)%2]; ++(*nrt);	    
        }	      
        else if (rc == 1) {
          pT[*nrt] = p1; radT[*nrt] = sT[(i1+1)%2]; ++(*nrt);	    
        }	      
      }
    }      
  }

  else if (oxT[2].typ == Typ_PT2) {
    // ------------------	  
    // 3. element = point
    // ------------------	  

    // get point	  
    pt = (Point2*)oxT[2].data;	 

    for (i1=0; i1<n1; ++i1) {

      // intersect circle (ci1->pc,rad=sT[i1]) with circle
      // (pt,rad=sT[(i1+1)%2]) 
      rc = UT2D_2pt_intcici (&p1, &p2, &(ci1->pc), sT[i1],
                             pt, sT[(i1+1)%2]);
      if (rc == 0) {
        pT[*nrt] = p1; radT[*nrt] = sT[(i1+1)%2]; ++(*nrt);	    
        pT[*nrt] = p2; radT[*nrt] = sT[(i1+1)%2]; ++(*nrt);	    
      }	    
      else if (rc == 1) {
        pT[*nrt] = p1; radT[*nrt] = sT[(i1+1)%2]; ++(*nrt);	    
      }	    
    }
  }

  else goto L_InErr;    
  // ----------------------------------------------------

L_Done:

  // touching circles (at most 8)
  for (i1=0; i1<(*nrt); ++i1) {
    ci = memSeg->next;
    rc = UME_add (memSeg, sizeof(Circ2));
    if (rc < 0) goto L_outOfSpace;
    rc = UT2D_ci_ptrd (ci, &pT[i1], radT[i1]);
    if (rc < 0) return -1;
    OGX_SET_OBJ (&oxtT[i1], Typ_CI2, Typ_CI2, 1, ci);
  }

  return 0;

L_InErr:
  TX_Error("UCV_Touch2ConcCiMoebCir: input error");
  return -2;

L_outOfSpace:
  TX_Error("UCV_Touch2ConcCiMoebCir: out of space");
  return -3;
}


//======================================================================
  int UCV_LnTouchLnMoebCir (int *nrt, ObjGX *oxtT, Memspc *memSeg,
		            ObjGX *ox1, ObjGX *ox2) {
//======================================================================
// UCV_LnTouchLnMoebCir    Author: Thomas Backmeister       15.10.2007
// 
/// \code
/// UCV_LnTouchLnMoebCir   Touching lines <-- 1 line, 1 M-cir
/// 
/// Compute the lines (at most 2) which touch a line and a Moebius circle.
/// A Moebius circle is a circle, line or finite point in
/// (xy-plane & UT2D_PT_INFTY).
/// 
/// IN:
///   ObjGX *ox1      ... line
///   ObjGX *ox2      ... Moebius circle
/// OUT:
///   int *nrt        ... number of touching lines
///   ObjGX *oxtT     ... touching lines (2)
///   Memspc *memSeg  ... space for oxtT[i].data, i=0,1,...
/// Returncodes:
///   0 = OK
///  -1 = input error
///  -2 = out of space
/// \endcode


  int rc, i1;	  
  Circ2 *ci2;	  
  Line2 *ln1, *ln, lT[2];
  Point2 *pt2, qt;
  Vector2 v1;

  //printf("UCV_LnTouchLnMoebCir: Touching lines <-- 1 line, 1 M-cir\n");
   
  // init
  *nrt = 0;

  // get line & line vector
  if (ox1->typ != Typ_LN2) goto L_InErr;
  ln1 = (Line2*)ox1->data;
  UT2D_vc_ln (&v1, ln1);

  if (ox2->typ == Typ_CI2) {
    // get circle	  
    ci2 = (Circ2*)ox2->data;

    // tangents of circle parallel to line
    UT2D_pt_tranorptvclen (&qt, &(ci2->pc), &v1, fabs(ci2->rad));
    if (UT2D_sid_ptvc (&qt, &(ln1->p1), &v1) != 0) {
      UT2D_ln_ptvc (&lT[*nrt], &qt, &v1);	    
      ++(*nrt);
    }	    
    UT2D_pt_tranorptvclen (&qt, &(ci2->pc), &v1, -fabs(ci2->rad));
    if (UT2D_sid_ptvc (&qt, &(ln1->p1), &v1) != 0) {
      UT2D_ln_ptvc (&lT[*nrt], &qt, &v1);	    
      ++(*nrt);
    }	    
  }
  else if (ox2->typ == Typ_LN2) {
    // no (new) solution  
  }
  else {
    // get point	  
    pt2 = (Point2*)ox2->data;

    // parallel line through point
    if (UT2D_sid_ptvc (pt2, &(ln1->p1), &v1) != 0) {
      UT2D_ln_ptvc (&lT[0], pt2, &v1);
      *nrt = 1;
    }      
  }

  // touching lines (at most 2)
  for (i1=0; i1<(*nrt); ++i1) {
    ln = memSeg->next;
    rc = UME_add (memSeg, sizeof(Line2));
    if (rc < 0) goto L_outOfSpace;
    *ln = lT[i1];
    OGX_SET_OBJ (&oxtT[i1], Typ_LN2, Typ_LN2, 1, ln);
  }

  return 0;

L_InErr:
  TX_Error("UCV_LnTouchLnMoebCir: input error");
  return -1;

L_outOfSpace:
  TX_Error("UCV_LnTouchLnMoebCir: out of space");
  return -2;
}


//======================================================================
  int UCV_TouchLnLnMoebCir (int *nrt, ObjGX *oxtT, Memspc *memSeg,
		            ObjGX *oxT) {
//======================================================================
// UCV_TouchLnLnMoebCir    Author: Thomas Backmeister       10.10.2007
// 
/// \code
/// UCV_TouchLnLnMoebCir   Touching M-cirs <-- 2 parallel lines, 1 M-cir
/// 
/// Compute all Moebius circles except points (at most 6) which touch two
/// parallel lines and one Moebius circle.
/// A Moebius circle is a circle, line or finite point in
/// (xy-plane & UT2D_PT_INFTY).
/// All three elements are supposed to be different!
/// 
/// IN:
///   ObjGX *oxT      ... 2 lines ([0],[1]) and 1 Moebius circle [2]
/// OUT:
///   int *nrt        ... number of touching Moebius circles
///   ObjGX *oxtT     ... touching Moebius circles (6)
///   Memspc *memSeg  ... space for oxtT[i].data, i=0,1,...
/// Returncodes:
///   0 = OK
///  -1 = internal error
///  -2 = input error
///  -3 = out of space
/// \endcode


  int rc, i1, nrq;	  
  double dl, rT[2];	  
  Circ2 *ci3, *ci, c1;	  
  Line2 *ln1, *ln2, *ln3;
  Line2 *lt, ml, l1, l2;
  Point2 *pt3, mp, pp, p1, p2, pT[4], qT[2];
  Vector2 v1, v3;

  //printf("UCV_TouchLnLnMoebCir: Touching M-cirs <-- 2 par. lns, 1 M-cir\n");
   
  // init
  *nrt = 0;
  nrq = 0;

  // get parallel lines
  if (oxT[0].typ != Typ_LN2 || oxT[1].typ != Typ_LN2) goto L_InErr;
  ln1 = (Line2*)oxT[0].data;
  ln2 = (Line2*)oxT[1].data;

  // vector of line 1
  UT2D_vc_ln (&v1, ln1);

  // middle line of parallel lines
  UT2D_pt_mid2pt (&mp, &ln1->p1, &ln2->p1);
  UT2D_ln_ptvc (&ml, &mp, &v1);

  // distance between middle line and line 1
  UT2D_pt_projptptvc (&pp, &mp, &ln1->p1, &v1);
  dl = UT2D_len_2pt (&mp, &pp);

  if (oxT[2].typ == Typ_CI2) {
    // ------------------
    // line, line, circle
    // ------------------

    // get circle	  
    ci3 = (Circ2*)oxT[2].data;

    // intersection points of 1 or 2 concentric circles with middle line
    rT[0] = fabs(ci3->rad) + dl;
    rT[1] = fabs(fabs(ci3->rad) - dl);
    for (i1=0; i1<2; ++i1) {
      if (rT[i1] < UT_TOL_min1) continue;	    
      rc = UT2D_2pt_intciptvc (&p1, &p2, &ci3->pc, rT[i1], &mp, &v1);
      if (rc == 0) {
	pT[*nrt] = p1; ++(*nrt);
	pT[*nrt] = p2; ++(*nrt);
      }	      
      else if (rc == 1) {
        pT[*nrt] = p1;
       	++(*nrt);
      }	      
    }

    // touch points of parallel lines (to lines 1,2) on circle ci3
    UT2D_pt_tranorptvclen (&qT[nrq], &(ci3->pc), &v1, fabs(ci3->rad));
    if (UT2D_sid_ptvc (&qT[nrq], &(ln1->p1), &v1) != 0) {
      if (UT2D_sid_ptvc (&qT[nrq], &(ln2->p1), &v1) != 0) ++nrq;
    }
    UT2D_pt_tranorptvclen (&qT[nrq], &(ci3->pc), &v1, -fabs(ci3->rad));
    if (UT2D_sid_ptvc (&qT[nrq], &(ln1->p1), &v1) != 0) {
      if (UT2D_sid_ptvc (&qT[nrq], &(ln2->p1), &v1) != 0) ++nrq;
    }
  }

  else if (oxT[2].typ == Typ_LN2) {
    // ----------------
    // line, line, line
    // ----------------

    // get line	  
    ln3 = (Line2*)oxT[2].data;

    // vector of line 3	  
    UT2D_vc_ln (&v3, ln3);

    // check if all 3 lines are parallel
    if (UT2D_comp2vc (&v1, &v3, UT_TOL_PAR)) return 0;

    // 2 parallel lines to line 3 at distance dl
    UT2D_pt_tranor2ptlen (&pT[0], &ln3->p1, &ln3->p2, dl);
    UT2D_ln_ptvc (&l1, &pT[0], &v3);
    UT2D_pt_tranor2ptlen (&pT[1], &ln3->p1, &ln3->p2, -dl);
    UT2D_ln_ptvc (&l2, &pT[1], &v3);

    // intersection points ml-l1, ml-l2
    rc = UT2D_pt_int2ln (&pT[0], &ml, &l1);
    if (rc < 1) return -1;
    rc = UT2D_pt_int2ln (&pT[1], &ml, &l2);
    if (rc < 1) return -1;
    *nrt = 2;
  }

  else {
    // -----------------
    // line, line, point
    // -----------------

    // get point	  
    pt3 = (Point2*)oxT[2].data;

    // intersection points of circle about pt3 with middle line
    rc = UT2D_2pt_intciptvc (&p1, &p2, pt3, dl, &mp, &v1);

    if (rc == 0) {
      pT[*nrt] = p1; ++(*nrt);
      pT[*nrt] = p2; ++(*nrt);
    }	      
    else if (rc == 1) {
      pT[*nrt] = p1;
      ++(*nrt);
    }	      

    // point pt3
    if (UT2D_sid_ptvc (pt3, &(ln1->p1), &v1) != 0) {
      if (UT2D_sid_ptvc (pt3, &(ln2->p1), &v1) != 0) {
	qT[0] = *pt3;      
        nrq = 1;
      }	      
    }
  }

  // touching circles (at most 4)
  for (i1=0; i1<(*nrt); ++i1) {
    ci = memSeg->next;
    rc = UME_add (memSeg, sizeof(Circ2));
    if (rc < 0) goto L_outOfSpace;
    rc = UT2D_ci_ptrd (ci, &pT[i1], dl);
    if (rc < 0) return -1;
    OGX_SET_OBJ (&oxtT[i1], Typ_CI2, Typ_CI2, 1, ci);
  }

  // touching lines (at most 2)
  for (i1=0; i1<nrq; ++i1) {
    lt = memSeg->next;
    rc = UME_add (memSeg, sizeof(Line2));
    if (rc < 0) goto L_outOfSpace;
    UT2D_ln_ptvc (lt, &qT[i1], &v1);
    OGX_SET_OBJ (&oxtT[*nrt], Typ_LN2, Typ_LN2, 1, lt);
    ++(*nrt);
  }    

  return 0;

L_InErr:
  TX_Error("UCV_TouchLnLnMoebCir: input error");
  return -2;

L_outOfSpace:
  TX_Error("UCV_TouchLnLnMoebCir: out of space");
  return -3;
}


//======================================================================
  int UCV_Touch3MoebCirs (int *nrt, ObjGX *oxtT, Memspc *memSeg,
		          ObjGX *oxiT, Memspc *workSeg) {
//======================================================================
// UCV_Touch3MoebCirs     Author: Thomas Backmeister       5.10.2007
// 
/// \code
/// UCV_Touch3MoebCirs    Touching Moebius circles <-- three Moebius circles
/// 
/// Compute all Moebius circles except points (at most 8) which touch
/// three different Moebius circles.
/// A Moebius circle is a circle, line or finite point in
/// (xy-plane & UT2D_PT_INFTY).
/// In case of three touching Moebius circles no solution is computed
/// (infinite number of possibilities).
/// There might be solutions touching one of the input elements at infinity!
/// 
/// IN:
///   ObjGX *oxiT      ... 3 Moebius circles
///   Memspc *workSeg  ... workspace
/// OUT:
///   int *nrt         ... number of touching Moebius circles
///   ObjGX *oxtT      ... touching Moebius circles (8)
///   Memspc *memSeg   ... space for oxtT[i].data, i=0,1,...
///                        and workspace
/// Returncodes:
///   0 = OK
///  -1 = internal error
/// \endcode


  int rc, i1;	  
  Point2 xp;
  Point pt;
  Circ2 *ci2;
  Circ ci;
  ObjGX oxT[3];


  // printf("UCV_Touch3MoebCirs: Touching M-circles <-- 3 M-circles\n");

  // init
  *nrt = 0;

  // determine special case of Apollonian configuration
  rc = UCV_CasesApollConfig (oxT, &xp, oxiT);
    // printf(" case=%d\n",rc);
  if (rc < 0) return -1;
  if (rc == 5) return 0; // no solution

  pt = UT3D_pt_pt2 (&xp);


  // -------------------------------------------------------------
  // --------------- treatment of different cases ----------------
  // -------------------------------------------------------------

  if (rc == 1) { // there are two elements touching each other.
    // printf("UCV_Touch3MoebCirs: CASE 1 - two TOUCHING elements\n");

    /*
    // display touchpoint (if finite)
    if (!UT2D_comp2pt (&xp, &UT2D_PT_INFTY, UT_TOL_pt)) 
      APT_disp_SymB (SYM_CIR_S, 7, &pt);
  
    // display touching elements
    for (i1=0; i1<2; ++i1) UTO_obj_Disp__ (&oxT[i1], 9);
    */

    // compute touching Moebius circles (at most 4)
    rc = UCV_Touch3MCirsCase1 (nrt, oxtT, memSeg, oxT, &xp, workSeg);
    if (rc < 0) return -1;
  }    
  // -------------------------------------------------------------

  else if (rc == 2) { // there is no pair of touching elements &
	              // there are two elements without common point.
    // printf("UCV_Touch3MoebCirs: CASE 2 - two NON-INTERSECTING elements\n");

    // display non-intersecting elements
    //for (i1=0; i1<2; ++i1) UTO_obj_Disp__ (&oxT[i1], NULL, 9);

    // compute touching Moebius circles (at most 8)
    rc = UCV_Touch3MCirsCase2 (nrt, oxtT, memSeg, oxT, workSeg);
    if (rc < 0) return -1;
  }	  
  // -------------------------------------------------------------

  else if (rc == 3) { // any two elements are intersecting in two
	              // different points & all three elements have
		      // 1 common point.
    // printf("UCV_Touch3MoebCirs: CASE 3 - pairwise 2-pt-intersection &");
    // printf(" 1 COMMON point\n");

    /*
    // display common point (if finite)
    if (!UT2D_comp2pt (&xp, &UT2D_PT_INFTY, UT_TOL_pt)) 
      APT_disp_SymB (SYM_CIR_S, 7, &pt);
    */      

    // compute touching Moebius circles (4)
    rc = UCV_Touch3MCirsCase3 (oxtT, memSeg, oxT, &xp, workSeg);
    if (rc < 0) return -1;
    *nrt = 4;
  }	  
  // -------------------------------------------------------------

  else { // any two elements are intersecting in two different points &
         // all three elements have 0 common points.
    // printf("UCV_Touch3MoebCirs: CASE 4 - pairwise 2-pt-intersection &");
    // printf(" 0 COMMON points\n");

    /*
    // display intersection point (if finite)
    if (!UT2D_comp2pt (&xp, &UT2D_PT_INFTY, UT_TOL_pt)) 
      APT_disp_SymB (SYM_CIR_S, 7, &pt);
    */      

    // compute touching Moebius circles (8)
    rc = UCV_Touch3MCirsCase4 (oxtT, memSeg, oxT, &xp, workSeg);
    if (rc < 0) return -1;
    *nrt = 8;
  }	  
  // -------------------------------------------------------------
  // ----------END: treatment of different cases -----------------
  // -------------------------------------------------------------

  return 0;
}


//======================================================================
  int UCV_CiInvertMoebCir (ObjGX *oxi, Memspc *memSeg, ObjGX *ox, Point2 *ci,
		           double ri) {
//======================================================================
// UCV_CiInvertMoebCir     Author: Thomas Backmeister       8.10.2007
// 
/// \code
/// UCV_CiInvertMoebCir    Invert at circle <-- Moebius circle
/// 
/// Invert (=mirror) a Moebius circle at a circle.
/// A Moebius circle is a circle, line or finite point in
/// (xy-plane & UT2D_PT_INFTY).
/// The function may be called with same parameter for ox and oxi.
/// 
/// IN:
///   ObjGX *ox       ... Moebius circle
///   Point2 *ci      ... center of inversion (!=UT2D_PT_INFTY)
///   double ri       ... radius of inversion
/// OUT:
///   ObjGX *oxi      ... inverse Moebius circle
///   Memspc *memSeg  ... space for oxi->data
/// Returncodes:
///   1 = oxi is NO Moebius circle but INFINITE point
///   0 = OK
///  -1 = input error
///  -2 = out of space
/// \endcode


  int rc;	  
  Circ2 *cir, *ci1;
  Line2 *ln, *ln1;
  Point2 *pt, *qt, p1, p2;
  Vector2 vn, vc;

  //printf("UCV_CiInvertMoebCir: Invert at circle <-- Moebius circle\n");

  if (ox->typ == Typ_CI2) {
    cir = (Circ2*)ox->data;
    // invert the circle	  
    // -----------------
    if (UT2D_comp2pt (ci, &(cir->pc), UT_TOL_min0)) {
      ci1 = memSeg->next;
      rc = UME_add (memSeg, sizeof(Circ2));
      if (rc < 0) goto L_outOfSpace;
      p1 = UT2D_CiInvertPoint (&(cir->p1), ci, ri);
      UT2D_Ci2PtsCentPer (ci1, &(cir->pc), &p1);
      OGX_SET_OBJ (oxi, Typ_CI2, Typ_CI2, 1, ci1);
      return 0;
    }	    
    UT2D_vc_2pt (&vn, ci, &(cir->pc));
    UT2D_vc_normalize (&vn, &vn);
    UT2D_vc_multvc (&vc, &vn, fabs(cir->rad));
    UT2D_pt_traptvc (&p1, &(cir->pc), &vc);
    UT2D_vc_multvc (&vc, &vn, -fabs(cir->rad));
    UT2D_pt_traptvc (&p2, &(cir->pc), &vc);
    p1 = UT2D_CiInvertPoint (&p1, ci, ri);
    if (UT2D_comp2pt (&p2, ci, UT_TOL_min0)) {
      // inverse circle is a line
      // (not containing center of inversion)
      ln1 = memSeg->next;
      rc = UME_add (memSeg, sizeof(Line2));
      if (rc < 0) goto L_outOfSpace;
      UT2D_vc_perpvc (&vn, &vn);
      UT2D_ln_ptvc (ln1, &p1, &vn);
      OGX_SET_OBJ (oxi, Typ_LN2, Typ_LN2, 1, ln1);
    }
    else {
      // inverse circle is a circle
      // (not containing center of inversion)
      ci1 = memSeg->next;
      rc = UME_add (memSeg, sizeof(Circ2));
      if (rc < 0) goto L_outOfSpace;
      p2 = UT2D_CiInvertPoint (&p2, ci, ri);
      UT2D_Ci2PtsDiam (ci1, &p1, &p2);
      OGX_SET_OBJ (oxi, Typ_CI2, Typ_CI2, 1, ci1);
    }	    
  }

  else if (ox->typ == Typ_LN2) {
    ln = (Line2*)ox->data;
    // invert the line
    // ---------------
    UT2D_vc_ln (&vc, ln);
    rc = UT2D_pt_projptptvc (&p1, ci, &(ln->p1), &vc);
    if (rc == 1) {
      // inverse line is the same line
      // (containing center of inversion)
      ln1 = memSeg->next;
      rc = UME_add (memSeg, sizeof(Line2));
      if (rc < 0) goto L_outOfSpace;
      *ln1 = *ln; 
      OGX_SET_OBJ (oxi, Typ_LN2, Typ_LN2, 1, ln1);
      return 0;
    }	    
    // inverse line is a circle
    // (containing center of inversion)
    ci1 = memSeg->next;
    rc = UME_add (memSeg, sizeof(Circ2));
    if (rc < 0) goto L_outOfSpace;
    p1 = UT2D_CiInvertPoint (&p1, ci, ri);
    UT2D_Ci2PtsDiam (ci1, &p1, ci);
    OGX_SET_OBJ (oxi, Typ_CI2, Typ_CI2, 1, ci1);
  }

  else if (ox->typ == Typ_PT2) {
    pt = (Point2*)ox->data;
    // invert the point
    // ----------------
    qt = memSeg->next;
    rc = UME_add (memSeg, sizeof(Point2));
    if (rc < 0) goto L_outOfSpace;
    if (UT2D_comp2pt (pt, ci, UT_TOL_min0)) {
      *qt = UT2D_PT_INFTY;	    
      OGX_SET_OBJ (oxi, Typ_PT2, Typ_PT2, 1, qt);
      goto L_Ex1;
    }	    
    *qt = UT2D_CiInvertPoint (pt, ci, ri);
    OGX_SET_OBJ (oxi, Typ_PT2, Typ_PT2, 1, qt);
  }

  else goto L_InErr;

  return 0;

L_Ex1:
  //printf("UCV_CiInvertMoebCir: inverse element is INFINITE point\n");
  return 1;

L_InErr:  
  TX_Error("UCV_CiInvertMoebCir: input error");
  return -1;

L_outOfSpace:
  TX_Error("UCV_CiInvertMoebCir: out of space");
  return -2;
}


//======================================================================
  int UCV_IntSectMoebCirs (Point2 *xp1, Point2 *xp2, ObjGX *ox1, ObjGX *ox2) {
//======================================================================
// UCV_IntSectMoebCirs     Author: Thomas Backmeister       3.10.2007
// 
/// \code
/// UCV_IntSectMoebCirs    Intersection <-- two Moebius circles
/// 
/// Intersection of two different Moebius circles.
/// A Moebius circle is a circle, line or finite point in
/// (xy-plane & UT2D_PT_INFTY).
/// An intersection point may be UT2D_PT_INFTY.
/// 
/// IN:
///   ObjGX *ox1   ... Moebius circle 1
///   ObjGX *ox2   ... Moebius circle 2
/// OUT:
///   Point2 *xp1  ... intersection point 1
///   Point2 *xp2  ... intersection point 2
/// Returncodes:
///   0 = 0 intersection points
///   1 = 1 intersection point (*xp1 = *xp2)
///   2 = 2 intersection points (different!)
///  -1 = input error
/// \endcode


  int rc;	  
  double dist;
  Vector2 vc;
  Circ2 *ci1, *ci2;	  
  Line2 *ln1, *ln2;
  Point2 *pt1, *pt2;

  // printf("UCV_IntSectMoebCirs: Intersection <-- 2 Moebius circles\n");

  // check input
  if (ox1->typ != Typ_CI2 && ox1->typ != Typ_LN2 && ox1->typ != Typ_PT2)
    goto L_InErr;	    
  if (ox2->typ != Typ_CI2 && ox2->typ != Typ_LN2 && ox2->typ != Typ_PT2)
    goto L_InErr;	    

  if (ox1->typ == Typ_CI2) {
    ci1 = (Circ2*)(ox1->data);
    if (ox2->typ == Typ_CI2) {
      ci2 = (Circ2*)(ox2->data);
      // check input
      if (UT2D_comp2pt (&(ci1->pc), &(ci2->pc), UT_TOL_min1) &&
	  UTP_comp2db (fabs(ci1->rad), fabs(ci2->rad), UT_TOL_min1))
	goto L_InErr;
      // CIRCLE-CIRCLE intersection
      rc = UT2D_2pt_intcici (xp1, xp2, &(ci1->pc), fabs(ci1->rad),
		                       &(ci2->pc), fabs(ci2->rad));
      if (rc == 0) return 2;
      else if (rc == 1) return 1;
      else return 0;
    }
    else if (ox2->typ == Typ_LN2) {
      ln2 = (Line2*)(ox2->data);
      // CIRCLE-LINE intersection
      UT2D_vc_ln (&vc, ln2);
      rc = UT2D_2pt_intciptvc (xp1, xp2, &(ci1->pc), fabs(ci1->rad),
		                         &(ln2->p1), &vc);
      if (rc == 0) return 2;
      else if (rc == 1) return 1;
      else return 0;
    }
    else {
      pt2 = (Point2*)(ox2->data);
      // CIRCLE-POINT intersection
      dist = UT2D_len_2pt (pt2, &(ci1->pc));
      if (UTP_comp2db (dist, fabs(ci1->rad), UT_TOL_min1)) {
	      *xp1 = *xp2 = *pt2;      
        return 1;
      }
      else return 0;      
    }
  }    
  else if (ox1->typ == Typ_LN2) {
    ln1 = (Line2*)(ox1->data);
    if (ox2->typ == Typ_CI2) {
      ci2 = (Circ2*)(ox2->data);
      // LINE-CIRCLE intersection
      UT2D_vc_ln (&vc, ln1);
      rc = UT2D_2pt_intciptvc (xp1, xp2, &(ci2->pc), fabs(ci2->rad),
		                         &(ln1->p1), &vc);
      if (rc == 0) return 2;
      else if (rc == 1) return 1;
      else return 0;
    }
    else if (ox2->typ == Typ_LN2) {
      ln2 = (Line2*)(ox2->data);
      // check input
      if (UT2D_Comp2Lns (ln1, ln2)) goto L_InErr;
      // LINE-LINE intersection
      *xp2 = UT2D_PT_INFTY;
      rc = UT2D_pt_int2ln (xp1, ln1, ln2);   // Thomas: UT2D_PtInt2Lns
        // printf(" rc-int2ln=%d\n",rc);
	      // UT3D_stru_dump (Typ_PT2, xp1, "xp1:");
      if (rc > 0) return 2;
      else {                         // lines parallel ..
        *xp1 = UT2D_PT_INFTY;	      
        return 1;
      }	
    }
    else {
      pt2 = (Point2*)(ox2->data);
      // LINE-POINT intersection
      UT2D_vc_ln (&vc, ln1);
      if (UT2D_sid_ptvc (pt2, &(ln1->p1), &vc) == 0) {
        *xp1 = *xp2 = *pt2;
	return 1;
      }
      else return 0;      
    }
  }	  
  else {
    pt1 = (Point2*)(ox1->data);
    if (ox2->typ == Typ_CI2) {
      ci2 = (Circ2*)(ox2->data);
      // POINT-CIRCLE intersection
      dist = UT2D_len_2pt (pt1, &(ci2->pc));
      if (UTP_comp2db (dist, fabs(ci2->rad), UT_TOL_min1)) {
	    *xp1 = *xp2 = *pt1;      
        return 1;
      }
      else return 0;      
    }
    else if (ox2->typ == Typ_LN2) {
      ln2 = (Line2*)(ox2->data);
      // POINT-LINE intersection
      UT2D_vc_ln (&vc, ln2);
      if (UT2D_sid_ptvc (pt1, &(ln2->p1), &vc) == 0) {
        *xp1 = *xp2 = *pt1;
	      return 1;
      }	      
      else return 0;
    }
    else {
      pt2 = (Point2*)(ox2->data);
      // check input
      if (UT2D_comp2pt (pt1, pt2, UT_TOL_min1)) goto L_InErr;
      return 0;
    }
  }	  

L_InErr:
  TX_Error("UCV_IntSectMoebCirs: input error");
  return -1;
}


//======================================================================
  int UCV_CasesApollConfig (ObjGX *oxoT, Point2 *xp, ObjGX *oxiT) {
//======================================================================
// UCV_CasesApollConfig     Author: Thomas Backmeister       3.10.2007
// 
/// \code
/// UCV_CasesApollConfig    Cases <-- Apollonian configuration
/// IN:
///   ObjGX *oxiT    ... 3 Moebius circles
/// OUT:
///   ObjGX *oxoT    ... ordered elements of oxiT (3);
///                      case 1: touching elements at indices 0,1;
/// 		     case 2: elements without common point at indices 0,1;
/// 		     case 4: at index 0: circle if xp == UT2D_PT_INFTY,
/// 		             element not incident with xp if xp == finite.
///   Point2 *xp     ... case 1: touching point of elements 0,1;
///                      case 3: common point of all three elements;
/// 		     case 4: one of the intersection points
/// 		             (=UT2D_PT_INFTY if existing);
/// Returncodes:
///   1 = case 1
///   2 = case 2
///   3 = case 3
///   4 = case 4
///   5 = case 5
///  -1 = internal error
/// \endcode

// Es gibt immer 3 Inputelemente; zB 3 Lines (= 3 MoebiusCircles).
// 
// Determine the special case of an Apollonian configuration.
// An Apollonian configuration consists of three DIFFERENT Moebius circles.
// A Moebius circle is a circle, line or finite point in
// (xy-plane & UT2D_PT_INFTY).
// The possible cases are:
// 1) There are two elements touching each other.
//    (2 der 3 Inputelmente beruehren sich).
//    2 parallele Geraden gelten als beruehrend (- im Unendlichen)
// 2) There is no pair of touching elements &
//    there are two elements without common point.
//    zb 3 Kreise, zwischen denen es keine Beruehrung geben darf
//    und mind 2 duerfen sich nicht schneiden ..
// 3) Any two elements are intersecting in two different points &
//    all three elements have 1 common point.
//    zb 3 Kreisen muessen sich schneiden UND einer der beiden Schnittpunkte muss
//      ident sein !
//    und auch 3 Geraden (der commonPoint ist im unendlichen ..)
// 4) Any two elements are intersecting in two different points &
//    all three elements have 0 common points.
//    zB 3 Kreise mit keinem gemeinsamen Punkt aber 3 X 2 intersectPunkten
// 5) Any two elements are intersecting in two different points &
//    all three elements have 2 common points (--> no solution).
// 


  int rcT[3], i1, i2, j1, k1, k2, k3;
  Point2 xpT[6];

  // printf("UCV_CasesApollConfig: Cases <-- Apollonian configuration\n");

  // init oxoT
  for (i1=0; i1<3; ++i1) oxoT[i1] = oxiT[i1];

  // pairwise intersection of Moebius circles
  j1 = 0;
  k1 = 0;
  for (i1=0; i1<2; ++i1) {
    for (i2=i1+1; i2<3; ++i2) {
      rcT[j1] = UCV_IntSectMoebCirs (&xpT[k1], &xpT[k1+1], &oxiT[i1], &oxiT[i2]);
      if (rcT[j1] < 0) return -1;
      ++j1;
      k1 += 2;
    }
  }    

    // for (i1=0; i1<3; ++i1) printf(" rcT[%d]=%d\n",i1,rcT[i1]);

  // check case 1
  for (i1=0; i1<3; ++i1) {
    if (rcT[i1] == 1) {
      if (i1 == 1) {
        oxoT[0] = oxiT[0];
        oxoT[1] = oxiT[2];
        oxoT[2] = oxiT[1];
      }
      else if (i1 == 2) {
        oxoT[0] = oxiT[1];
        oxoT[1] = oxiT[2];
        oxoT[2] = oxiT[0];
      }	      
      *xp = xpT[2*i1];
      return 1;
    }	    
  }	  

  // check case 2
  for (i1=0; i1<3; ++i1) {
    if (rcT[i1] == 0) {
      if (i1 == 1) {
        oxoT[0] = oxiT[0];
        oxoT[1] = oxiT[2];
        oxoT[2] = oxiT[1];
      }
      else if (i1 == 2) {
        oxoT[0] = oxiT[1];
        oxoT[1] = oxiT[2];
        oxoT[2] = oxiT[0];
      }	      
      return 2;
    }  
  }

  // check case 3 or 5
  // (UT_TOL_min0 should be a little bit greater than UT_TOL_min1!)
  for (i1=0; i1<2; ++i1) {
    for (i2=2; i2<4; ++i2) {
      if (UT2D_comp2pt (&xpT[i1], &xpT[i2], UT_TOL_min0)) {
        for (j1=4; j1<6; ++j1) {
          if (UT2D_comp2pt (&xpT[i1], &xpT[j1], UT_TOL_min0)) {
	    k1 = (i1 + 1) % 2;
	    k2 = ((i2 - 1) % 2) + 2;
	    k3 = ((j1 - 3) % 2) + 4;
            if (UT2D_comp2pt (&xpT[k1], &xpT[k2], UT_TOL_min0)) {
              if (UT2D_comp2pt (&xpT[k1], &xpT[k3], UT_TOL_min0))
                return 5;		      
            }		    
            *xp = xpT[i1];
  	    return 3;
	  }    
        }		
      }	      
    }	    
  }	  

  // case 4
  for (i1=0; i1<6; ++i1) {
    if (UT2D_comp2pt (&xpT[i1], &UT2D_PT_INFTY, UT_TOL_pt)) {
      *xp = UT2D_PT_INFTY;
      j1 = 2 - i1 / 2;
      oxoT[0] = oxiT[j1];       // is a circle!
      oxoT[1] = oxiT[(j1+1)%3]; // is a line
      oxoT[2] = oxiT[(j1+2)%3]; // is a line
      return 4;
    }
  }
  *xp = xpT[4]; // not incident with oxoT[0]
  return 4;
}


//======== EOF =========
