//  ut_tin.c.c                                2008-01-24    Franz Reiter.
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
\file  ../ut/ut_tin.c
\brief faces (using struct Fac3 - UFA_)
\code
=====================================================
List_functions_start:

UFA_add_fac        add face to MemTab(Fac3)
UFA_facpt_chg__        set face
UFA_facpt_chg_psn       set 1 pointer in face (seqnr) from esn
UFA_2fac_facpt     create 2 new Faces from 1 face (point on edge)
UFA_3fac_facpt     create 3 new Faces from 1 face
UFA_mod_delPt      change all face-pointers >= ip to ip-1
UFA_fac_dump1      dump 1 face
UFA_fac_dump_f      dump faces into file
UFA_fac_ck_sr__    test sense-of-rotation of all faces
UFA_fac_ck_sr_1    test sense-of-rotation of a face
UFA_fac_srar       get (signed) area of face and sense-of-rotation (3D-points)
UFA_fac_srar2      get (signed) area of face 2D
UFA_fac_chg_sr     change sr of face

UFA_2if_find_2ip   find (1|2) faces from unoriented Edge (2 ipt's)
UFA_ifac_ck_edgo   find face from oriented Edge (2 ipt's)
UFA_if_findSegP    find face with LineSegment ips-ps2 inside
UFA_if_find_ptmsh  find faceNr of point (2D-test point inside face)
UFA_if_find_ip1    test if point ipt is already meshed
UFA_if_find_ipIn   find face covering point ipt
UFA_if_getNf_ife   get neighbourface for face, edge
UFA_ife_getNf_ife  get neighbour and esn for face,esn
UFA_nifac_ck_pt    get all faces going tru point with index ipt

UFA_esn_ips        get EdgeSequenceNumber of face from (start-)pointNr
UFA_esn_ipe        get EdgeSequenceNumber of face from (end-)pointNr
UFA_esn_nxt        get Edge|PointSeqNr of next Edge|Point (CCW)           INLINE
UFA_esn_prv        get Edge|PointSeqNr of previous Edge|Point (CCW)       INLINE
UFA_esn_opp_psn    get EdgeSeqNr of Edge opposite to PointSeqNr           INLINE
UFA_2esn_comm2fac  get common edges of 2 faces with common edge

UFA_ipOpp_fac_esn  get point opposite to edge
UFA_ipOpp_facedg   get point opposite to edge (ip3 & ptNr from ip1)
UFA_ip_fac_psn     get pointIndex from face, edgeSeqNr
UFA_ipe_fac_esn    get pointIndex of end of edge from face, edgeSeqNr
UFA_2ip_ck_ptfac   ck if ip1 is used in face; get ip2, ip3.
UFA_2ip_fac_esn    get pointNrs for Edge from face + EdgeSeqNr
UFA_2ip_fac_ip     get the 2 opposite points from face and point
UFA_2pt2_fac_esn   get 2D-points of edge of face + EdgeSeqNr
UFA_ck_2ip_ipa     test if edge ip1-ip2 is on polygon; oriented, closed.

UFA_psn_1fac_ip    get pointSeqNr of pointNr in face

UFA_opt_diag       check common edge for optimize
UFA_opt__          test 2 common Edges
UFA_opt_saf
UFA_opt_ckCvx      check if 2 faces with common edge are convex
UFA_opt_ckOpt      Check Delaunay triangulation

UFA_int_fac_ln2    intersect face - 2D-line

UFA_2fac_flip      flip 2 faces with common edge
UFA_pFac3_esn      get pointers into Face

UFA_fnb_init__     create neighbourfaces for mesh
UFA_fnb_init_1     update fnb[ifac]
UFA_fnb_init_el    set NeighbourFacNr -2, if Edge is a BreakLine
UFA_fnb_upd        update fnb (faces to be updated are marked -16)
UFA_fnb_BL_reset   reset all reset all fnb-values -2 (BreakLines)
UFA_fnb_set_if_es  set 1 fnb-link from iFac and esn
UFA_fnb_get_2ip    find neighbourface for edge from 2 points
UFA_fnb_set_esn    set face-neighbour from startpoint
UFA_fnb_set_val    set face-neighbour
UFA_fnb_mod_val    modify face-neighbour
UFA_nfb_ck__       check all nfb-structs
UFA_nfb_ck_f       check single nfb-struct
UFA_nfb_ck_1       check single edge of nfb-struct

UFA_fnb_dump_1     dump face and its neighbours
UFA_fnb_dump__     dump faces and neighbours
UFA_fnb_dump_f     dump faces and neighbours into file

List_functions_end:
=====================================================
- see also:
UT2D_ck_pt_in_tria
../ut/ut_tria.c     Triangle-functions    (using struct Point)
../ut/ut_msh.c      MSH   Funktionen fuer Meshes (MemTab)

\endcode *//*----------------------------------------



// PointSeqNr psn   1,2,3;   Face CCW
//
// EdgeSeqNr  esn:  0 unknown 
//                  1 (from p1 to p2),
//                  2 (from p2 to p3),
//                  3 (from p3 to p1)
//
//                 p3     (x = pointNrs)
//               / |
//             e3  e2     (e = edgeNrs)
//           /     |
//        p1--e1---p2
//              




*/
#ifdef globTag
 void UFA(){}
 void TIN(){}
#endif

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>




#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_msh.h"              // Fac3 ..
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_tin.h"              // TYP_EDGLN_BL
#include "../ut/ut_txt.h"              // fnam_del
#include "../gr/ut_UI.h"               // UI_Func... SYM_..
#include "../ut/ut_TX.h"               // TX_Print
#include "../ut/ut_os.h"               // OS_ ..





//----------------------------------------------------------------
// prototypes
  int UFA_fac_dump_f (Fac3 *fa, int fNr, char *fnExt, char *txt, ...);


//================================================================
  int UFA_if_find_ptmsh (int *iFac, Point *pt1,
                         Fac3 *fa, int fNr,
                         Point *pa, int pNr) {
//================================================================
// find faceNr of point pt1 (2D-test point inside face)
// see also UFA_pt_prjptmsh1
// Output:
//   iFac      face nr (index into fa)
// Returncodes:
//   1 = NO  = point outside mesh
//   0 = YES = point inside triangle
//  -1 = ~~  = point is on sideline p1-p2
//  -2 = ~~  = point is on sideline p2-p3
//  -3 = ~~  = point is on sideline p3-p1
//  -4 = ~~  = point is equal to p1
//  -5 = ~~  = point is equal to p2
//  -6 = ~~  = point is equal to p3



  int     irc, i1, ii1, ii2, ii3;
  Point2  *pf1, *pf2, *pf3, *pp;


  // UT3D_stru_dump (Typ_PT, pt1, "UFA_if_find_ptmsh: ");


  // check if point is in Triangle or on its boundary
  pp = (Point2*)pt1;

  // loop tru triangles;
  for(i1=0; i1<fNr; ++i1) {
    ii1 = fa[i1].i1;
    ii2 = fa[i1].i2;
    ii3 = fa[i1].i3;

    pf1 = (Point2*)&pa[ii1];
    pf2 = (Point2*)&pa[ii2];
    pf3 = (Point2*)&pa[ii3];

    // test if point ip1 is inside Face ii1-ii2-ii3
    irc = UT2D_ck_pt_in_tria (pf1, pf2, pf3, pp);
    if(irc > 0) continue;    // outside ..
    goto L_found;
  }

  // point is ouside mesh
    // printf("ex UFA_if_find_ptmsh pt is outside ..\n");
  return 1;


  L_found:  //irc: 0=inside, -1=on i1-i2, -2=on i2-i3, -3=on i3-i1
  *iFac = i1;

    // printf("ex UFA_if_find_ptmsh if=%d irc=%d %d %d %d\n",i1,irc,ii1,ii2,ii3);

  return irc;

}


//================================================================
  int UFA_if_find_ip1 (int *ie, int ipt, Fac3 *fa, int fNr) {
//================================================================
// test if point ipt is already meshed
// Output:
//   ie          point-sequence-nr  1|2|3
//   retCod      faceIndex; -1=none
//
// see also 

  int   i1;


  for(i1=0; i1<fNr; ++i1) {
    if(fa[i1].i1 == ipt) {*ie = 1; return i1;}
    if(fa[i1].i2 == ipt) {*ie = 2; return i1;}
    if(fa[i1].i3 == ipt) {*ie = 3; return i1;}
  }

  return -1;

}


//========================================================================
  int UFA_if_find_ipIn (int *io, int *ie, Point *pt1, Point *pa,
                        Fac3 *fa, int fNr) {
//========================================================================
// find face covering point ipt
//
/// 
/// Output:
///   io      rc=0: faceNr; rc=1: pointIndex
///   pto     rc=0: pti on face (Z fixed); rc=1: NULL
///   retCod  0 OK; inside_face, on_face_edge:  io=faceNr, pto=point
///           1 OK; pti is on_edge; io=faceNr, ie=edgeNr, pto=point
///           2 OK; pti is identical_with_point: io=pointIndex; ie=ptSeqNr
///          -1 point is not inside mesh
/// 
// see also MSH_pt_prjptmsh1
// TODO: use UFA_if_find_ipIn in MSH_pt_prjptmsh1



  int     irc, i1, ii1, ii2, ii3;
  Point2  p2i;




  p2i = UT2D_pt_pt3 (pt1);



  // check if point is in Triangle or on its boundary
  // loop tru triangles;
  for(i1=0; i1<fNr; ++i1) {
    *io = i1;
    ii1 = fa[i1].i1;
    ii2 = fa[i1].i2;
    ii3 = fa[i1].i3;

    // test if point ip1 is inside Face ii1-ii2-ii3
    irc = UT2D_ck_pt_in_tria ((Point2*)&pa[ii1],
                              (Point2*)&pa[ii2],
                              (Point2*)&pa[ii3], &p2i);
    if(irc > 0) continue;    // outside ..

    //----------------------------------------------------------------
    // irc: -4: pi == p1; -5: pi == p2; -6: pi == p3.
    if(irc <= -4) {
      // irc 2 - pti_is_identical_with_point: io=pointIndex, pto=point.
      if(irc == -4) { *io = ii1; *ie = 1; }
      if(irc == -5) { *io = ii2; *ie = 2; }
      if(irc == -6) { *io = ii3; *ie = 3; }
      return 2;
    }


    //----------------------------------------------------------------
    // irc: -1=on i1-i2, -2=on i2-i3, -3=on i3-i1
    if(irc <= -1) {
      // irc 1 - pti is on_edge; io=faceNr, ie=edgeNr, pto=point.
      *io = i1;
      if(irc == -1) *ie = 1;
      if(irc == -2) *ie = 2;
      if(irc == -3) *ie = 3;
      return 1;
    }



    //----------------------------------------------------------------
    // retCod  0 OK; inside_face, on_face_edge:  io=faceNr, pto=point
    *io = i1;
    return 1;

  }


  // point is ouside mesh
    // printf("MSH_pt_prjptmsh1 pt is outside ..\n");
  return -1;

}


//=========================================================================
  int UFA_if_findSegP (int *iFac, int *iEdg,
                       int ips, int ipe, Point *ps2,
                       Fac3 *fa, int fNr, Point *pa) {
//=========================================================================
// find face with LineSegment ips-ps2 inside.
// The startPoint of the LineSegment is a facepoint; the endPoint is or is not.
// LineSegment: startPoint = pa[ips]    endpoint = ps2
// Input:
//   ips, ps2   a ray limited at ips tru ps2 (ps2=pa[ipe])
//   ipe     used only for test if breakline already exists
// Output:
//   iFac    Facenr
//   iEdg    EdgeSeqNr of edge in iFace where ips-ps2 goes tru
// RetCod:
//   -1      Error; no Face with point ips ...
//    0      correct intersection with Face iFac;
//    1-6    see UT2D_ptvc_ck_int2pt
//    10     this breakline already exists !



  int        irc, i2, i4, iif, iie, esn, ip1, ip2;
  int        iTab[50], iNr;
  char       psna[50];
  Vector2    vs;
  Point2     *p21, *p22, *ps1;


  // printf("UFA_if_findSegP %d\n",ips);

  // get all faces using point ips
  iNr = 50;
  irc = UFA_nifac_ck_pt (iTab, psna, &iNr, ips, fa, fNr);
  if(irc < 0) goto L_Err1;
  // all facNr's in iTab; corresponding pointSeqNr's are in psna.


  // make vs = vector-segment
  ps1 = (Point2*)&pa[ips];
  UT2D_vc_2pt (&vs, ps1, (Point2*)ps2);
    // UT3D_stru_dump (Typ_VC2, &vs, "  vs:");


  // loop tru iTab;
  // find face where edges goes tru
  for(i2=0; i2<iNr; ++i2) {
    iif = iTab[i2];       // iif = faceNr to test
    iie = psna[i2];       // iie = pointSeqNr of ips in face iif
    esn =  UFA_esn_opp_psn (iie);
      // printf(" test f[%d]-e%d\n",iif,iie);
    // get points of the edge opposite to the point iie
    // get 2D-points of edge of face + EdgeSeqNr
    // UFA_2pt2_fac_esn (&p21, &p22, esn, &fa[iif], pa);
    UFA_2ip_fac_esn (&ip1, &ip2, esn, &fa[iif]);
      // printf(" esn=%d ip1=%d ip2=%d\n",esn,ip1,ip2);

    // if (ip1|ip2 == endpunkt): this breakline already exists !
    if((ip1 == ipe)||(ip2 == ipe)) {
        // printf (" breakline already exists !\n");
      return 10; // this breakline already exists !
    }

    // seg starts at point ps1; check intersection with edge p21-p22
    p21 = (Point2*)&pa[ip1];
    p22 = (Point2*)&pa[ip2];
    irc = UT2D_ptvc_ck_int2pt (1, ps1, &vs, p21, p22);
    if(irc < 0) continue;
    // intersecting Face found; get intersecting EdgeNr
    *iEdg =  esn;
    goto L_found;
  }

  L_Err1:
  iif = -1;
  irc = -1;

  L_found:
  *iFac = iif;

    // printf("ex UFA_if_findSegP irc=%d iFac=%d iEdg=%d\n",irc,iif,esn);
    // if(irc>=0)
      // printf("   f[%d]=%d-%d-%d\n",iif,fa[iif].i1,fa[iif].i2,fa[iif].i3);

  return irc;

}

 
//================================================================
  int UFA_mod_delPt (MemTab(Fac3) *fTab, int ip) {
//================================================================
/// UFA_mod_delPt         change all face-pointers >= ip to ip-1

  int   i1;
  Fac3  *fa = fTab->data;


  for(i1=0; i1<fTab->rNr;  ++i1) {
    if(fa[i1].i1 >= ip) fa[i1].i1 -= 1;
    if(fa[i1].i2 >= ip) fa[i1].i2 -= 1;
    if(fa[i1].i3 >= ip) fa[i1].i3 -= 1;
  }
    
  return 0;
    
} 


//=============================================================================
  int UFA_ife_getNf_ife (int *nfc, int *nec, int iFac, int iEdg,
                           Fac3 *fac, Fac3 *fnb) {
//=============================================================================
// get nfc = neighbour-face of ifc and nec = edgeNr on face nfc
// Input:
//   iFac,iEdg     an egde; find its neighbouredge
// Output:
//   nfc,nec       the neighbouredge
//   retCod    0   OK
//             1   ?
//           -99   internal Error

  int   ipi;
  Fac3  *fac1;
  

  if     (iEdg == 1) *nfc = fnb[iFac].i1;
  else if(iEdg == 2) *nfc = fnb[iFac].i2;
  else if(iEdg == 3) *nfc = fnb[iFac].i3;
  
  // test for boundary
  if(*nfc < 0) return 1;


  // get endPt of iFac-iEdg (= startPt of nec)     see UFA_ipe_fac_esn
  fac1 = &fac[iFac];
  if(iEdg == 1)      ipi = fac1->i2;
  else if(iEdg == 2) ipi = fac1->i3;
  else if(iEdg == 3) ipi = fac1->i1;


  // get esn of face nfc                       see UFA_esn_ips
  fac1 = &fac[*nfc];
  if     (fac1->i1 == ipi) *nec = 1;
  else if(fac1->i2 == ipi) *nec = 2;
  else if(fac1->i3 == ipi) *nec = 3;
  else {
    printf("**** UFA_ife_getNf_ife E001 f=%d e=%d\n", iFac, iEdg);
    return -99;
  }


    // printf("ex UFA_if_getNf_ife nbFac=%d %d %d\n",nbFac,iFac,iEdg);

  return 0;

}


//================================================================
  int UFA_if_getNf_ife (int iFac, int iEdg, Fac3 *fnb) {
//================================================================
// get neighbourFace from esn  (iFac, iEdg)
// Input:
//   iFac           face-nr
//   iEdg           esn (1|2|3 - see :101 "EdgeSeqNr  esn:")
// RetCod:          ind of UFA_if_getNf_ife


  int    nbFac;


  if(iEdg == 1) {
    nbFac = fnb[iFac].i1;

  } else if(iEdg == 2) {
    nbFac = fnb[iFac].i2;

  } else if(iEdg == 3) {
    nbFac = fnb[iFac].i3;

  }

    // printf("ex UFA_if_getNf_ife nbFac=%d %d %d\n",nbFac,iFac,iEdg);

  return nbFac;

}


//================================================================
  int UFA_fnb_init__ (Fac3 *fnb, Fac3 *fa, int fNr) {
//================================================================
/// \code
/// Zu den Faces fa eine Parallelstructur fnb aufbauen,
/// in denen die NachbarFacNrs gespeichert sind.
/// (Fac3*)fnb: fnb[f].i1 = Index zum Nachbarface der Kante 1 (p0-p1)
///             fnb[f].i2 = Index zum Nachbarface der Kante 2 (p1-p2)
///             fnb[f].i3 = Index zum Nachbarface der Kante 3 (p2-p0)
///    (Kante 1 geht von Punkt 1 zu Punkt 2) usw.
///       -1 = this edge is unresolved
/// breaklines are not marked; see UFA_fnb_init_el
/// \endcode

// see UFA_fnb_init_1
// OFFEN:
// TODO: haette man sap; koennte man testen ob ie1 && ie2 BoundaryPoints sind;
//   wenn ja: kein neighbourface moeglich.


  int    i1, ie1, ie2, ifn, ien;


  // printf("UFA_fnb_init__ %d\n",fNr);


  // init all neighbours with -1 (unresolved)
  for(i1=0; i1<fNr; ++i1) {
     fnb[i1].i1 = -1;
     fnb[i1].i2 = -1;
     fnb[i1].i3 = -1;
  }


  // loop tru faces; find unresolved Faces;
  // gegenueberliegende FaceNr eintragen.
  for(i1=0; i1<fNr; ++i1) {

    // loop tru edges of face i1
    ie1 = fa[i1].i1;
    ie2 = fa[i1].i2;
    // find neighbourface for edge ie1-ie2
    ifn = UFA_fnb_get_2ip (fa, fNr, ie1,ie2);
      // printf(" fnb[%d]-e1(%d-%d) = %d\n",i1,ie1,ie2,ifn);
    fnb[i1].i1 = ifn;


    ie1 = fa[i1].i2;
    ie2 = fa[i1].i3;
    ifn = UFA_fnb_get_2ip (fa, fNr, ie1,ie2);
      // printf(" fnb[%d]-e2(%d-%d) = %d\n",i1,ie1,ie2,ifn);
    fnb[i1].i2 = ifn;


    ie1 = fa[i1].i3;
    ie2 = fa[i1].i1;
    ifn = UFA_fnb_get_2ip (fa, fNr, ie1,ie2);
      // printf(" fnb[%d]-e3(%d-%d) = %d\n",i1,ie1,ie2,ifn);
    fnb[i1].i3 = ifn;


    fnb[i1].st = 0;

  }


  // dump neigbours:
  // UFA_fnb_dump__ (fa, fnb, fNr);

  return 0;

}


//================================================================
  int UFA_fnb_init_t (int ifac, int esn, int ie1, int ie2, int ifn,
                      Fac3 *fnb, Fac3 *fa, int fNr) {
//================================================================
// TEST ONLY

  int i1;

  if((ifac != 4885) &&
     (ifac != 4930) &&
     (ifac != 7733)) return 0;

  printf(">>>>>>>>> f%d-e%d: ie1=%d ie2=%d ifn=%d\n",ifac,esn,ie1,ie2,ifn);

  i1=4855; UFA_fnb_dump_1 (&fa[i1], &fnb[i1], "fa[%d] = ",i1);
  i1=4930; UFA_fnb_dump_1 (&fa[i1], &fnb[i1], "fa[%d] = ",i1);
  i1=7733; UFA_fnb_dump_1 (&fa[i1], &fnb[i1], "fa[%d] = ",i1);


  return 0;

}


//================================================================
  int UFA_fnb_set_if_es (Fac3 *fnb, int iFac, int esn, int ival) {
//================================================================
/// UFA_fnb_set_if_es  set 1 fnb-link from iFac and esn

  // printf("UFA_fnb_set_if_es f=%d e=%d %d\n",iFac,esn,ival);


  if(esn == 1) {
    fnb[iFac].i1 = ival;

  } else if(esn == 2) {
    fnb[iFac].i2 = ival;

  } else if(esn == 3) {
    fnb[iFac].i3 = ival;

  } else {
    printf("**** UFA_fnb_set_if_es E001 f=%d e=%d %d\n",iFac,esn,ival);
  }


  return 0;

}


//================================================================
  int UFA_fnb_upd (Fac3 *fnb, Fac3 *fa, int fNr) {
//================================================================
/// UFA_fnb_upd            update fnb (faces to be updated are marked -16)

  int    i1;

  // update all fnb-records marked -16
  for(i1=0; i1<fNr; ++i1) {
    if(fnb[i1].st == 16) {
      UFA_fnb_init_1 (i1, fnb, fa, fNr);
      fnb[i1].st = 0;
    }
  }

  return 0;

}

 
//================================================================
  int UFA_fnb_init_1 (int ifac, Fac3 *fnb, Fac3 *fa, int fNr) {
//================================================================
// UFA_fnb_init_1           update fnb[ifac]
// see UFA_fnb_init__


  int    ie1, ie2, ifn;


  // printf(".... UFA_fnb_init_1 %d\n",ifac);
  // UFA_fnb_dump_1 (&fa[ifac], &fnb[ifac], "fnb_init_1-1 f[%d] ",ifac);
  // if(ifac==6) UFA_fac_dump_f (fa, fNr, "5", "UFA_fnb_init_1-6");
  // if(ifac==6) AP_debug__ ("UFA_fnb_init_1-6");


  // fix i1 (esn1; 
    ie1 = fa[ifac].i1;
    ie2 = fa[ifac].i2;
    // find neighbourface for edge ie1-ie2
    ifn = UFA_fnb_get_2ip (fa, fNr, ie1,ie2);
    // set link on face ifac
    fnb[ifac].i1 = ifn;
    // set link on neighbourface ifn
    if(fnb[ifn].st != 16)
      UFA_fnb_set_esn (ifn, ie2, ifac, fa, fnb);
      // UFA_fnb_init_t (ifac, 1, ie1, ie2, ifn, fnb, fa, fNr);


  // fix i2 (esn2
    ie1 = fa[ifac].i2;
    ie2 = fa[ifac].i3;
    // find neighbourface for edge ie1-ie2
    ifn = UFA_fnb_get_2ip (fa, fNr, ie1,ie2);
    fnb[ifac].i2 = ifn;
    // set link on neighbourface ifn
    if(fnb[ifn].st != 16)
      UFA_fnb_set_esn (ifn, ie2, ifac, fa, fnb);
      // UFA_fnb_init_t (ifac, 2, ie1, ie2, ifn, fnb, fa, fNr);


  // fix i3 (esn3
    ie1 = fa[ifac].i3;
    ie2 = fa[ifac].i1;
    // find neighbourface for edge ie1-ie2
    ifn = UFA_fnb_get_2ip (fa, fNr, ie1,ie2);
    fnb[ifac].i3 = ifn;
    // set link on neighbourface ifn
    if(fnb[ifn].st != 16)
      UFA_fnb_set_esn (ifn, ie2, ifac, fa, fnb);
      // UFA_fnb_init_t (ifac, 3, ie1, ie2, ifn, fnb, fa, fNr);


  fnb[ifac].st = 0;

    // TESTBLOCK
    // UFA_fnb_dump_1 (&fa[ifac], &fnb[ifac], "fnb_init_1-2 f[%d] ",ifac);
    // TESTBLOCK

  return 0;

}


//====================================================================
  int UFA_opt__ (Fac3 *fa, Fac3 *fnb, int fNr, Point2 *p2a) {
//====================================================================
// Optimize: jeweils 2 Faces mit gemeinsamer Kante testen;

// Input:
//   fa[fNr]  
//   fnb[fNr] opposite face#, -2 if edge is fixed (breakline)
//   saf      check for breaklines
// Output:
//   fa[fNr]  modified; 
//   retCod   nr of modified triangle-paires
//           -99 errorExit (debug)


// wenn Querkante kuerzer als gemeinsame Kante: Kante vertauschen.
// 
//               ie4
//                |    if1
//     ie1--------+----------ie2
//                |    if2
//               ie3


  int    irc, i1,  ie1, ie2, ie3, ie4, if1, if2, esn1, esn2, iNr;


  // printf("UFA_opt__ %d\n",fNr);


  iNr = 0;

  // loop tru faces
  for(if1=0; if1<fNr; ++if1) {
      // printf(" _opt__ -------- nxt if1 %d\n",if1);


    //----------------------------------------------------------------
    // test edge e1 (p1-p2) of face if1
    esn1 = 1;
    if2 = fnb[if1].i1;
    if(if2 < 0) goto L_ck_e2;    // skip boundary
    // if((saf[if1] == 2)&&(saf[if2] == 2)) goto L_ck_e2;  // skip BreakLine
      // printf(" opt1: %d %d\n",if1,if2);
    ie1 = fa[if1].i1;
    ie2 = fa[if1].i2;
    ie4 = fa[if1].i3;
    // get opposite pointNr > ie3
    UFA_ipOpp_facedg (&ie3, &esn2, &fa[if2], ie2);
    if(ie3 < 0) goto L_ck_ee;
      // printf(" p1-p2=%d,%d p3-p4=%d,%d f=%d,%d e1\n",ie1,ie2,ie3,ie4,if1,if2);
    // check diag
    irc = UFA_opt_ckCvx (&p2a[ie1], &p2a[ie2], &p2a[ie3], &p2a[ie4]);
      // printf(" _ckCvx %d\n",irc);
    if(irc != 0) goto L_ck_e2;
    irc = UFA_opt_ckOpt (&p2a[ie1], &p2a[ie2], &p2a[ie3], &p2a[ie4]);
      // printf(" _ckOpt %d\n",irc);
    if(irc != 1) goto L_ck_e2;
    // flip
      // printf(" flip1 %d/%d %d/%d\n",if1,esn1,if2,esn2);
    UFA_2fac_flip (if1, esn1, if2, esn2, fa, fnb);
      // UT3D_stru_dump (Typ_Fac3, &fa[if1], "f[%d]",if1);
      // UT3D_stru_dump (Typ_Fac3, &fa[if2], "f[%d]",if2);
    // if((saf[if1] == 2)||(saf[if2] == 2))   // update BreakLine
      // UFA_opt_saf (if1, if2, ie1, ie2, ie3, ie4, eTab, saf);
    ++iNr;


    //----------------------------------------------------------------
    // test edge p2-p3
    L_ck_e2:
    esn1 = 2;
    if2 = fnb[if1].i2;
    if(if2 < 0) goto L_ck_e3;    // skip boundary
    // if((saf[if1] == 2)&&(saf[if2] == 2)) goto L_ck_e3;  // skip BreakLine
      // printf(" opt2: %d %d\n",if1,if2);
    ie1 = fa[if1].i2;
    ie2 = fa[if1].i3;
    ie4 = fa[if1].i1;
    // get opposite pointNr > ie3
    UFA_ipOpp_facedg (&ie3, &esn2, &fa[if2], ie2);
    if(ie3 < 0) goto L_ck_ee;
      // printf(" p1-p2=%d,%d p3-p4=%d,%d f=%d,%d e2\n",ie1,ie2,ie3,ie4,if1,if2);
    // check diag
    irc = UFA_opt_ckCvx (&p2a[ie1], &p2a[ie2], &p2a[ie3], &p2a[ie4]);
    if(irc != 0) goto L_ck_e3;
    irc = UFA_opt_ckOpt (&p2a[ie1], &p2a[ie2], &p2a[ie3], &p2a[ie4]);
    if(irc != 1) goto L_ck_e3;
    // flip
      // printf(" flip2 %d/%d %d/%d\n",if1,esn1,if2,esn2);
    UFA_2fac_flip (if1, esn1, if2, esn2, fa, fnb);
      // UT3D_stru_dump (Typ_Fac3, &fa[if1], "f[%d]",if1);
      // UT3D_stru_dump (Typ_Fac3, &fa[if2], "f[%d]",if2);
    // if((saf[if1] == 2)||(saf[if2] == 2))   // update BreakLine
      // UFA_opt_saf (if1, if2, ie1, ie2, ie3, ie4, eTab, saf);
    ++iNr;


    //----------------------------------------------------------------
    // test edge p3-p1
    L_ck_e3:
    esn1 = 3;
    if2 = fnb[if1].i3;
    if(if2 < 0) goto L_ck_OK;        // skip boundary
    // if((saf[if1] == 2)&&(saf[if2] == 2)) continue;  // skip BreakLine
      // printf(" opt3: %d %d\n",if1,if2);
    ie1 = fa[if1].i3;
    ie2 = fa[if1].i1;
    ie4 = fa[if1].i2;
    // get opposite pointNr > ie3
    UFA_ipOpp_facedg (&ie3, &esn2, &fa[if2], ie2);
    if(ie3 < 0) goto L_ck_ee;
      // printf(" p1-p2=%d,%d p3-p4=%d,%d f=%d,%d e3\n",ie1,ie2,ie3,ie4,if1,if2);
    // check diag
    irc = UFA_opt_ckCvx (&p2a[ie1], &p2a[ie2], &p2a[ie3], &p2a[ie4]);
    if(irc != 0) goto L_ck_OK;
    irc = UFA_opt_ckOpt (&p2a[ie1], &p2a[ie2], &p2a[ie3], &p2a[ie4]);
    if(irc != 1) goto L_ck_OK;
    // flip
      // printf(" flip3 %d/%d %d/%d\n",if1,esn1,if2,esn2);
    UFA_2fac_flip (if1, esn1, if2, esn2, fa, fnb);
      // UT3D_stru_dump (Typ_Fac3, &fa[if1], "f[%d]",if1);
      // UT3D_stru_dump (Typ_Fac3, &fa[if2], "f[%d]",if2);
    // if((saf[if1] == 2)||(saf[if2] == 2))   // update BreakLine
      // UFA_opt_saf (if1, if2, ie1, ie2, ie3, ie4, eTab, saf);
    ++iNr;


    L_ck_OK:
      continue;


    L_ck_ee:
      // there is no point <ie2> in face <if2>
      // did not find ie3 = opp.point of edge ie2 in face if2
      printf("*** UFA_opt E001 if2=%d ie2=%d if1=%d esn1=%d\n",if2,ie2,if1,esn1);
      // TESTBLOCK
      UFA_fnb_dump__ (fa, fnb, fNr, "UFA_opt E001");
      return -99;
      // TESTBLOCK

  }

  // printf("ex UFA_opt %d\n",iNr);

  return iNr;

}


/*
//====================================================================
  int UFA_opt (Fac3 *fa, Fac3 *fnb, int fNr, Point2 *p2a, char *saf) {
//====================================================================
// Optimize: jeweils 2 Faces mit gemeinsamer Kante testen;
// wenn Querkante kuerzer als gemeinsame Kante: Kante vertauschen.

//               ie4
//                |
//     ie1--------+----------ie2
//                |
//               ie3


  int    irc, i1,  ie1, ie2, ie3, ie4, if1, if2, esn1, esn2, iNr;


  // printf("UFA_opt %d\n",fNr);


  iNr = 0;

  // loop tru faces
  for(if1=0; if1<fNr; ++if1) {

    // test edge p1-p2
    esn1 = 1;
    if2 = fnb[if1].i1;
    if(if2 < 0) goto L_ck_e2;    // skip boundary
    if((saf[if1] == 2)&&(saf[if2] == 2)) goto L_ck_e2;  // skip BreakLine
    ie1 = fa[if1].i1;
    ie2 = fa[if1].i2;
    ie4 = fa[if1].i3;
    // get opposite pointNr > ii3
    UFA_ipOpp_facedg (&ie3, &esn2, &fa[if2], ie2);
      // printf(" p1-p2=%d,%d p3-p4=%d,%d f=%d,%d\n",ie1,ie2,ie3,ie4,if1,if2);
    // check diag
    irc = UFA_opt_ckCvx (&p2a[ie1], &p2a[ie2], &p2a[ie3], &p2a[ie4]);
    if(irc != 0) goto L_ck_e2;
    irc = UFA_opt_ckOpt (&p2a[ie1], &p2a[ie2], &p2a[ie3], &p2a[ie4]);
    if(irc != 0) goto L_ck_e2;
    // flip
    UFA_2fac_flip (if1, esn1, if2, esn2, fa, fnb);
    ++iNr;


    // test edge p2-p3
    L_ck_e2:
    esn1 = 2;
    if2 = fnb[if1].i2;
    if(if2 < 0) goto L_ck_e3;    // skip boundary
    if((saf[if1] == 2)&&(saf[if2] == 2)) goto L_ck_e3;  // skip BreakLine
    ie1 = fa[if1].i2;
    ie2 = fa[if1].i3;
    ie4 = fa[if1].i1;
    // get opposite pointNr > ii3
    UFA_ipOpp_facedg (&ie3, &esn2, &fa[if2], ie2);
      // printf(" p1-p2=%d,%d p3-p4=%d,%d f=%d,%d\n",ie1,ie2,ie3,ie4,if1,if2);
    // check diag
    irc = UFA_opt_ckCvx (&p2a[ie1], &p2a[ie2], &p2a[ie3], &p2a[ie4]);
    if(irc != 0) goto L_ck_e3;
    irc = UFA_opt_ckOpt (&p2a[ie1], &p2a[ie2], &p2a[ie3], &p2a[ie4]);
    if(irc != 0) goto L_ck_e3;
    // flip
    UFA_2fac_flip (if1, esn1, if2, esn2, fa, fnb);
    ++iNr;


    // test edge p3-p1
    L_ck_e3:
    esn1 = 3;
    if2 = fnb[if1].i3;
    if(if2 < 0) continue;        // skip boundary
    if((saf[if1] == 2)&&(saf[if2] == 2)) continue;  // skip BreakLine
    ie1 = fa[if1].i3;
    ie2 = fa[if1].i1;
    ie4 = fa[if1].i2;
    // get opposite pointNr > ii3
    UFA_ipOpp_facedg (&ie3, &esn2, &fa[if2], ie2);
      // printf(" p1-p2=%d,%d p3-p4=%d,%d f=%d,%d\n",ie1,ie2,ie3,ie4,if1,if2);
    // check diag
    irc = UFA_opt_ckCvx (&p2a[ie1], &p2a[ie2], &p2a[ie3], &p2a[ie4]);
    if(irc != 0) continue;
    irc = UFA_opt_ckOpt (&p2a[ie1], &p2a[ie2], &p2a[ie3], &p2a[ie4]);
    if(irc != 0) continue;
    // flip
    UFA_2fac_flip (if1, esn1, if2, esn2, fa, fnb);
    ++iNr;


    // L_ck_ee:

  }

  // printf("ex UFA_opt %d\n",iNr);
  return iNr;

}


//=======================================================================
  int UFA_opt_saf (int if1, int if2, int ip1, int ip2, int ip3, int ip4,
                   MemTab(EdgeLine) *eTab, char *saf)  {
//=======================================================================
// Nach dem flip von 2 faces mit common edge muss nun die BreakLine-Info
// in saf fuer faces if1 & if2 korrigiert werden.

// Optimieren: Am Beginn sollte saf so geladen werden, dass man erkennen kann
//   welches Egde eine Breakline ist. Also zB Bit1 fuer Edge1; Bit2 fuer Edge2;
//   Bit3 fuer Edge3; wenn erforderlich Bit4 fuer Boundary ?
//   In UFA_BL_cke kann man dann ganz gezielt vertauschen.

// f1=p4-p3-p2
// f2=p3-p4-p1
//                p4
//               /
//     p1------/-------------p2
//           /
//         p3

  int    i1, i2;


  // printf("UFA_opt_saf %d %d %d %d %d %d\n",if1,if2,ip1,ip2,ip3,ip4);

  // test f1: p3-p2; p2-p4;
  // test f2: p4-p1; p1-p3;

  // check face if1:
  i1 = UFA_BL_cke (ip3, ip2, eTab);
  i2 = UFA_BL_cke (ip2, ip4, eTab);
  if((i1 == 0)||(i2 == 0)) saf[if1] = 2;
  else                     saf[if1] = 0;

  // check face if2:
  i1 = UFA_BL_cke (ip4, ip1, eTab);
  i2 = UFA_BL_cke (ip1, ip3, eTab);
  if((i1 == 0)||(i2 == 0)) saf[if2] = 2;
  else                     saf[if2] = 0;

  return 0;

}
*/

//======================================================================
  int UFA_opt_ckOpt (Point2 *p1, Point2 *p2, Point2 *p3, Point2 *p4) {
//======================================================================
//
//  Check Delaunay triangulation                   Karl Sauer 2008-05-09
//
//  Gegeben sind die diagonal gegenueberliegenden Eckpunkte (P1-P2 bzw. P3-P4)
//  eines 4-Ecks (P1-P3-P2-P4, s.u.), zwischen denen 2 Dreiecke mit einer
//  gemeinsamen Kante (= Diagonale!) aufgespannt werden sollen:
//         (1)  P1 - P2 - P3 und P1 - P2 - P4 (das ist der Iststand)
//    oder (2)  P1 - P3 - P4 und P2 - P4 - P3
//
//                P4                                                   P4
//               /                                                     /
//     P1------/-------------P2   oder   auch P1----------------P2    /
//           /                                                       /
//         P3                                                       P3
//
//
//   Es wird geprueft ob die Variante (1) das Delaunay-Kriterium erfuellt.
//   Es ist erfuellt wenn gilt:
//       P4 liegt NICHT INNERHALB des Umkreises des Dreiecks P1 - P2 - P3.
//
//   Ist dies nicht erfuellt, so ist Variante (2) die korrekte Loesung.
//
//   VERBOTEN: P3 und P4 beide auf der selben Seite von P1 - P2 !!!!
//                             P4
//                             /
//    P1----------------P2    /
//                           /
//                          P3
//
//  INPUT:
//  ------
//    Point2 p1[2]   ... X/Y Koordinaten Punkt 1
//    Point2 p2[2]   ... X/Y Koordinaten Punkt 2
//    Point2 p3[2]   ... X/Y Koordinaten Punkt 3
//    Point2 p4[2]   ... X/Y Koordinaten Punkt 4
//
//  OUTPUT:
//  -------
//    long   rc      ... Funktionswert:  0 = Kriterium OK fuer (1)
//                                       1 = Kriterium OK fuer (2)
//                                       2 = P4 liegt auf Umkreis von P1-P2-P3
//                                      -1 = Dreieck P1-P2-P3 ist degeneriert


  int    rc1;
  Point2 pum1;
  double d, d1, d4;


  // pum1: Umkreismittelpunkt p1 - p2 - p3; 1=OK
  rc1 = UTRI_ptCC_3pt (&pum1, p1, p2, p3);

  if (rc1 == 1) {

    rc1 = 0;

    //d1: pum1 <-> p1
    d = p1->x - pum1.x;
    d1 = d*d;
    d = p1->y - pum1.y;
    d1 += d*d;
    //    rum1 = d1;

    //d4: pum1 <-> p4
    d = p4->x - pum1.x;
    d4 = d*d;
    d = p4->y - pum1.y;
    d4 += d*d;

    // Check position of p4
    if (d4 < d1-UT_TOL_min2) {
    // if (d4 < d1) {
      rc1 = 1;
    } else {
      if (d4 <= d1+UT_TOL_min2) rc1 = 2;
      // if (d4 <= d1) rc1 = 2;
    }

  } else {
    rc1 = -1;
  }


/*
//GEGENPROBE:
  rc2 = UTRI_ptCC_3pt (&pum2, p1, p2, p4);

  if (rc2 == 0) {

    //d1: pum1 <-> p1
    d = p1->x - pum2->x;
    d1 = d*d;
    d = p1->y - pum2->y;
    d1 += d*d;
    rum2 = d1;

    //d3: pum1 <-> p3
    d = p3->x - pum2->x;
    d3 = d*d;
    d = p3->y - pum2->y;
    d3 += d*d;

// Check position of p3
    if (d3 < d1-UT_TOL_min2) {
      rc2 = 1;
    } else{
      if (d3 <= d1+UT_TOL_min2) rc2 = 2;
    }

  } else {
    rc2 = -1;
    printf("*** rc2 = -1 ***\n");

  }

//==========================

  if (rc1 >= 0 && rc2 >= 0 && rc1 != rc2) {
    printf("UFA_opt_ckOpt: different rc bei p1-p2-p3 und p1-p2-p4\n");
    printf("P1 = %lf %lf, ",p1->x, p1->y);
    printf("P2 = %lf %lf, ",p2->x, p2->y);
    printf("P3 = %lf %lf, ",p3->x, p3->y);
    printf("P4 = %lf %lf\n",p4->x, p4->y);
    printf("rc1 = %d, rc2 = %d\n",rc1, rc2);
    printf("rum1 = %le, d4 = %le\n",rum1, d4);
    printf("rum2 = %le, d3 = %le\n\n",rum2, d3);

  }
*/

  return rc1;

}


//================================================================
  int UFA_opt_ckCvx (Point2 *p1, Point2 *p2, Point2 *p3, Point2 *p4) {
//================================================================
// check if 2 faces with common edge are convex or not.
// p1-p2 ist momentan die commonLine.
// test if p3-p4 is crossing p1-p2 or not.

//
// test if p3-p4 intersect p1-p2;
//   no:  retCod = 0;
//   yes: retCod = 1;
// RetCodes:
//   -1   p3-p4 is outside p1-p2
//    0   p3-p4 is crossing p1-p2
//    1   p3-p4 goes tru p1
//    2   p3-p4 goes tru p2

//                p4
//               /
//     p1------/-------------p2
//           /
//         p3


  int       sp1, sp2;
  Vector2   v34;


  //----------------------------------------------------------------
  // test if p3-p4 intersect p1-p2;

  // v34 = p3 -> p4
  UT2D_vc_2pt (&v34, p3, p4);

  // test if p1 is right-or-left of p3-p4; 0=on, 1=left, -1=right
  sp1 = UT2D_sid_ptvc (p1, p3, &v34);
  if(sp1 == 0) return 1;   // p3-p4 goes tru p1

  sp2 = UT2D_sid_ptvc (p2, p3, &v34);
  if(sp2 == 0) return 2;   // p3-p4 goes tru p2

  if(sp1 == sp2) return -1;

  return 0;

}


//================================================================
  int UFA_2fac_flip (int if1, int esn1, int if2, int esn2,
                     Fac3 *fa, Fac3 *fnb) {
//================================================================
/// change diagonalLine;  common Edge is p1-p2; change to p3-p4.
/// Output:
///   fa        modied
///   fnb       modied

// IN:             f2n3
//       f2p1--------------f2p3
//      f1p2 \              |
//        |     \       2   | f2n2
//    f1n2|    f1n1 f2n1    |
//        |   1       \     |
//        |              \ f2p2
//      f1p3-------------f1p1
//             f1n3

// OUT:          f1n2
//      f1p3--------------f1p2
//        |              / f2p1
//        |   1   f1n1/     |
//    f1n3|        /        |f2n3
//        |     / f2n1   2  |
//      f1p1 /              |
//        f2p2-------------f2p3
//               f2n2



// fa- und fnb-Records kopieren, modifizieren, ersetzen.
// pointers in die kopierten Records entsprechend esn setzen.
// Beim Schreiben (in die original-records esn nicht mehr beachten



  int    *f1p1, *f1p2, *f1p3, *f2p1, *f2p2, *f2p3;
  int    f1n2, f2n2;
  Fac3   f1, f2;



  // printf("UFA_2fac_flip f1=%d %d f2=%d %d\n",if1,esn1, if2,esn2);
  // if((if1==15)||(if2==15)) {
  // UFA_fnb_dump_1 (&fa[if1], &fnb[if1], "_flip-inp-f%d ",if1);
  // UFA_fnb_dump_1 (&fa[if2], &fnb[if2], "_flip-inp-f%d ",if2);
  // }


  // copy records
  f1 = fa[if1];
  f2 = fa[if2];

  // get pointers into Fac3
  UFA_pFac3_esn (&f1p1, &f1p2, &f1p3, &f1, esn1);
  UFA_pFac3_esn (&f2p1, &f2p2, &f2p3, &f2, esn2);

  fa[if1].i1 = *f1p3;
  fa[if1].i2 = *f2p3;
  fa[if1].i3 = *f1p2;

  fa[if2].i1 = *f2p3;
  fa[if2].i2 = *f1p3;
  fa[if2].i3 = *f2p2;

  // copy neighbour-records
  f1 = fnb[if1];
  f2 = fnb[if2];

  // get pointers into Fac3
  UFA_pFac3_esn (&f1p1, &f1p2, &f1p3, &f1, esn1);
  UFA_pFac3_esn (&f2p1, &f2p2, &f2p3, &f2, esn2);

  fnb[if1].i1 = if2;
  fnb[if1].i2 = *f2p3;
  fnb[if1].i3 = *f1p2;

  fnb[if2].i1 = if1;
  fnb[if2].i2 = *f1p3;
  fnb[if2].i3 = *f2p2;

    // TESTBLOCK
    // dump if1 & if2
    // printf("ex UFA_2fac_flip:\n");
    // if((if1==15)||(if2==15)) {
    // UFA_fnb_dump_1 (&fa[if1], &fnb[if1], "out-f1:");
    // UFA_fnb_dump_1 (&fa[if2], &fnb[if2], "out-f2:");
    // }
    // TESTBLOCK


  // change neighours of f1n3 (set f2; was f1)
  //   and f2n3 (set f1; was f2)
    // printf(" f1n2=%d  f1p2=%d f=%d\n",fnb[if1].i2,fa[if1].i2,if1);
    // printf(" f2n2=%d  f2p2=%d f=%d\n",fnb[if2].i2,fa[if2].i2,if2);
  f1n2 = fnb[if1].i2; // neighbourface of new face 1 edge 2
  f2n2 = fnb[if2].i2; // neighbourface of new face 2 edge 2

  if(f1n2 >= 0)
  UFA_fnb_set_esn (f1n2, fa[if1].i3, if1, fa, fnb); // f1n3 - 1 > 2
  if(f2n2 >= 0)
  UFA_fnb_set_esn (f2n2, fa[if2].i3, if2, fa, fnb); // f2n3 - 2 > 1

    // TESTBLOCK
    // if((if1==15)||(if2==15)) {
    // UFA_fnb_dump_1 (&fa[if1], &fnb[if1], "_flip-out-f%d ",if1);
    // UFA_fnb_dump_1 (&fa[if2], &fnb[if2], "_flip-out-f%d ",if2);
    // }
    // TESTBLOCK

  return 0;

}


//===================================================================
  int UFA_fnb_set_esn (int ifc, int ips, int ifnb, Fac3 *fa, Fac3 *fnb) {
//===================================================================
/// set the neighbour of edge starting with point ips of Face ifc to ifnb
/// Input:
///   ifc   index of fnb to modify
///   ips   index point where edge starts
///   ifnb  index neigbourface; new value
///
/// see UFA_fnb_set_if_es


  int   esn;


  // if(ifc == 9)
  // printf("UFA_fnb_set_esn f=%d e=%d nb=%d\n",ifc,ips,ifnb);


  if(ifc < 0) return -1;


  // esn = edgeSequenceNumber
  // find esn of edge starting with point ips in face ifc
  esn = UFA_esn_ips (&fa[ifc], ips);

  UFA_fnb_set_val (&fnb[ifc], esn, ifnb);

    // if(ifc == 9)
    // UFA_fnb_dump_1 (&fa[ifc], &fnb[ifc], " _setinb-fa[%d] = ",ifc);

  return 0;

}


//================================================================
  int UFA_fnb_mod_val (Fac3 *fnb1, int valNew, int valOld) {
//================================================================
// UFA_fnb_mod_val    modify face-neighbour

  // printf("UFA_fnb_mod_val %d %d\n",valNew,valOld);

  if     (fnb1->i1 == valOld)  fnb1->i1 = valNew;
  else if(fnb1->i2 == valOld)  fnb1->i2 = valNew;
  else if(fnb1->i3 == valOld)  fnb1->i3 = valNew;
  else return -1;

  return 0;

}



//================================================================
  int UFA_fnb_set_val (Fac3 *fnb1, int esn, int ifnb) {
//================================================================
// UFA_fnb_set_val               set face-neighbour
 
  if(esn == 1)       fnb1->i1 = ifnb;
  else if(esn == 2)  fnb1->i2 = ifnb;
  else if(esn == 3)  fnb1->i3 = ifnb;

    // if(ifc == 9)
    // UFA_fnb_dump_1 (&fa[ifc], &fnb[ifc], " _setinb-fa[%d] = ",ifc);

  return 0;

}


//================================================================
  int UFA_esn_ipe (Fac3 *fac1, int ipe) {
//================================================================
/// UFA_esn_ipe        get EdgeSequenceNumber of face from (end-)pointNr
/// get esn of single face from pointIndex

// see also UFA_esn_ips UFA_ipOpp_facedg

  int   esn;

  if     (fac1->i1 == ipe) esn = 3;
  else if(fac1->i2 == ipe) esn = 1;
  else if(fac1->i3 == ipe) esn = 2;

  // printf("ex UFA_esn_ips esn=%d ip(%d,%d,%d) ie=%d\n",
          // esn, fac1->i1, fac1->i2, fac1->i3, ie1);

  return esn;

}


//================================================================
  int UFA_esn_ips (Fac3 *fac1, int ipi) {
//================================================================
/// get EdgeSequenceNumber of the Edge starting with point ipi
/// get esn of single face from pointIndex

// see also UFA_ipOpp_facedg

  int   esn;

  if     (fac1->i1 == ipi) esn = 1;
  else if(fac1->i2 == ipi) esn = 2;
  else if(fac1->i3 == ipi) esn = 3;

  // printf("ex UFA_esn_ips esn=%d ip(%d,%d,%d) ie=%d\n",
          // esn, fac1->i1, fac1->i2, fac1->i3, ie1);

  return esn;

}


//=======================================================================
  int UFA_pFac3_esn (int **ip1, int **ip2, int **ip3, Fac3 *f, int esn) {
//=======================================================================
// get pointers into Fac3
// get 3 pointers to points for single face; starting from esn
// if esn == 1: pointers to i1,i2,i3;
// if esn == 2: pointers to i2,i3,i1;
// if esn == 3: pointers to i3,i1,i2;

  if(esn == 1) {
    *ip1 = &f->i1;
    *ip2 = &f->i2;
    *ip3 = &f->i3;

  } else if(esn == 2) {
    *ip1 = &f->i2;
    *ip2 = &f->i3;
    *ip3 = &f->i1;

  } else if(esn == 3) {
    *ip1 = &f->i3;
    *ip2 = &f->i1;
    *ip3 = &f->i2;
  }

  return 0;

}


//===========================================================================
  int UFA_fnb_dump_f (Fac3 *fac, Fac3 *fnb, int fNr, char *fnExt, char *inf) {
//===========================================================================
// UFA_fnb_dump_f          dump faces and neighbours into file

  int    i1;
  FILE    *fpo;
  char    s1[128];
  Fac3    *f, *n;


  sprintf(s1, "%sApp_%s.tmp", OS_get_tmp_dir(),fnExt);
    
  printf("UFA_fnb_dump_f %s\n",s1);

  if((fpo=fopen(s1,"w")) == NULL) {
    printf("****** err open %s\n",s1);
    return -1;
  }
    



  fprintf(fpo, "---------------- UFA_fnb_dump__ %d %s\n",fNr,inf);
  for(i1=0; i1<fNr; ++i1) {
    f = &fac[i1];
    n = &fnb[i1];

    fprintf(fpo, " %d (%d %d %d  s%2d) fnb = (%d %d %d  s%2d)\n",i1,
             f->i1,f->i2,f->i3, f->st,
             n->i1,n->i2,n->i3, n->st);
  }

  fclose (fpo);

  return 0;

}



//================================================================
  int UFA_fnb_dump__ (Fac3 *fa, Fac3 *fnb, int fNr, char *inf) {
//================================================================
// dump faces and neighbours

  int    i1;


  printf("---------------- UFA_fnb_dump__ %d %s\n",fNr,inf);
  for(i1=0; i1<fNr; ++i1)
    UFA_fnb_dump_1 (&fa[i1], &fnb[i1], "fa[%d] = ",i1);

  return 0;

}

 
//================================================================
  int UFA_fnb_dump_1 (Fac3 *fac, Fac3 *fnb, char *txt, ...) {
//================================================================
// see MSH_dump_fTab MSH_fns_dump__ MSH_fns_dump_1

  va_list va;
  char    cbuf[80];


  // printf("UFA_fnb_dump_1\n");

  va_start(va, txt);
  vprintf(txt, va);
  va_end(va);

  printf("(%d %d %d  s%2d) fnb = (%d %d %d  s%2d)\n",
          fac->i1,fac->i2,fac->i3, fac->st,
          fnb->i1,fnb->i2,fnb->i3, fnb->st);

  // printf("ex UFA_fnb_dump_1\n");



  return 0;

}
 

/*
//================================================================
  int UFA_opt_diag (Point2 *p1, Point2 *p2, Point2 *p3, Point2 *p4) {
//================================================================
UNUSED; ersetzt durch UFA_opt_ckOpt
// check diagonal-line
// p1-p2 ist momentan die commonLine.
// 1) test if p3-p4 is crossing p1-p2; if yes:
// 2) test if p3-p4 is shorter than p1-p2
//   yes:  retCod = 0; flip faces; change f1: p1-p2-p4 -> p1-p3-p4
//                                 change f2: p2-p1-p3 -> p4-p3-p2

//
// test if p3-p4 intersect p1-p2;
//   no:  retCod = 0;
//   yes: retCod = 1;
// RetCodes:
//   -2   p3-p4 is longer than p1-p2
//   -1   p3-p4 is outside p1-p2
//    0   p3-p4 is inside p1-p2
//    1   p3-p4 goes tru p1
//    2   p3-p4 goes tru p2

//                p4
//               /
//     p1------/-------------p2
//           /
//         p3


  int       irc;
  double    d1, d2;
  double    d3, d4;
  Vector2   v34;




  // // TEST OPTIMIZE: length
  // UT2D_lenq_2pt (&d1, p1, p2);  // active
  // UT2D_lenq_2pt (&d2, p3, p4);  // planned
  // if(d1 <= d2) return -2;
    // // printf("UFA_opt_diag d1=%f d2=%f\n",d1,d2);



  // // TEST OPTIMIZE: min. diff of areas
// GEHT BEI gis_test2 ned !!!!!  (viele spitze Faces; aber gleich grosz!)
  // // test if areas of faces p1-p3-p4/p4-p3-p2 are less different
  // // than areas of faces p1-p2-p4/p2-p1-p3
  // d1 = fabs(UT2D_ar_3pt (p1, p2, p4));
  // d2 = fabs(UT2D_ar_3pt (p2, p1, p3));
  // d3 = fabs(UT2D_ar_3pt (p1, p3, p4));
  // d4 = fabs(UT2D_ar_3pt (p4, p3, p2));
    // // printf(" ar1-4= %f %f %f %f\n",d1,d2,d3,d4);
  // if(d2 > d1) d1 = d2/d1;
  // else        d1 = d1/d2;
  // if(d4 > d3) d2 = d4/d3;
  // else        d2 = d3/d4;
    // // printf(" d1-d2= %f %f\n",d1,d2);
  // // tol unbedingt notwendig; sonst loopts.
  // if(d1 < (d2 * 1.5)) return -2; // d1=ist, d2=soll
  // // if(d1 <= d2) return -2; // d1=ist, d2=soll



  //----------------------------------------------------------------
  // test if p3-p4 intersect p1-p2;

  // test if p1 is right-or-left of p3-p4
  UT2D_vc_2pt (&v34, p3, p4);
  irc = UT2D_sid_ptvc (p1, p3, &v34);
  if(irc < 0) return -1;   // p1 is below p3-p4
  if(irc == 0) return 1;   // p3-p4 goes tru p1

  // test if p2 is right-or-left of p3-p4
  irc = UT2D_sid_ptvc (p2, p3, &v34);
  if(irc > 0) return -1;   // p2 is above p3-p4
  if(irc == 0) return 2;   // p3-p4 goes tru p2

  // printf("ex UFA_opt_diag 0\n");


  // TEST OPTIMIZE: min. inkreisradius ..
  // test if areas of faces p1-p3-p4/p4-p3-p2 are less different
  // than areas of faces p1-p2-p4/p2-p1-p3
  UTRI_rado_3pt (&d1, p1, p2, p4);
  UTRI_rado_3pt (&d2, p2, p1, p3);
  UTRI_rado_3pt (&d3, p1, p3, p4);
  UTRI_rado_3pt (&d4, p4, p3, p2);
    // printf(" ar1-4= %f %f %f %f\n",d1,d2,d3,d4);
  if(d2 < d1) d1 = d2; // d1=ist
  if(d4 < d3) d3 = d4; // d3=soll
    // printf(" d1-d2= %f %f\n",d1,d2);
  // if(d3 < (d1 * 1.1)) return -2;  // Inkreis
  // if(d3 > (d1 * 0.9)) return -2;  // Umkreis
  // if(d3 >= d1) return -2;  // Umkreis
# define gis_TOL_PT  0.1
  if(d3 > (d1 - gis_TOL_PT)) return -2;  // Umkreis

  return 0;

}
*/

//================================================================
  int UFA_ipOpp_facpt (int *ipOpp, Fac3 *fc, int ip) {
//================================================================
// get 3.point of fc into ipOpp. (In: 1.pt; out: 3.pt)

  if(fc->i1 == ip) {
    *ipOpp = fc->i3;

  } else if(fc->i2 == ip) {
    *ipOpp = fc->i1;

  } else if(fc->i3 == ip) {
    *ipOpp = fc->i2;
  }

  // printf("ex UFA_ipOpp_facpt i3=%d i1=%d f=(%d,%d,%d)\n",
          // *ipOpp, ip, fc->i1, fc->i2, fc->i3);

  return 0;

}


//==================================================================
  int UFA_ipOpp_facedg (int *ipOpp, int *esn, Fac3 *fac1, int ips) {
//==================================================================
// UFA_ipOpp_facedg   get point opposite to edge (ip3 & ptNr from ip1)
// get nr of point opposite to edge with startingPoint ips
// and get EdgeSequenceNr of ips.
// Input:
//   ips      index startpoint of edge in face fac1
// Output:
//   esn      edgeNr of edge starting with point ips
//   ipOpp    index point opposite to edge starting with point ips
//            -1 if ips is not found in face fac1

// ips is the starting point of a edge of fac1;
// the opposite point is the point befor ips.


  if(fac1->i1 == ips) {
    *esn = 1;
    *ipOpp = fac1->i3;

  } else if(fac1->i2 == ips) {
    *esn = 2;
    *ipOpp = fac1->i1;

  } else if(fac1->i3 == ips) {
    *esn = 3;
    *ipOpp = fac1->i2;

  } else {
    // there is no point <ips> in face <fac1> 
    *ipOpp = -1;
      printf("*** UFA_ipOpp_facedg E001 p %d f %d %d %d\n",ips,
             fac1->i1,fac1->i2,fac1->i3);
  }

  // printf("ex UFA_ipOpp_facedg opp=%d e=%d sn=%d (%d,%d,%d)\n",
          // *ipOpp, ips, *esn,
          // fac1->i1, fac1->i2, fac1->i3);

  return 0;

}


//================================================================
  int UFA_2ip_ck_ptfac (int *ip2, int *ip3, Fac3 *fc, int ip1) {
//================================================================
// UFA_2ip_ck_ptfac   ck if ip1 is used in face fc; get ip2, ip3.
// check if face fc uses point ip1; if yes get the other points > ip2,ip3.
// Input:
//   fc     face to test
//   ip1    pointIndex to test
// Output:
//   ip2    -1 = ip1 not in face, else next ip of face
//   ip3    next ip of face


  // printf("UFA_2ip_ck_ptfac fac %d %d %d ip1 %d\n",fc->i1,fc->i2,fc->i3,ip1);

  *ip2 = -1;


  if(fc->i1 == ip1) {
    *ip2 = fc->i2;
    *ip3 = fc->i3;
    goto L_fnd;
  }

  if(fc->i2 == ip1) {
    *ip2 = fc->i3;
    *ip3 = fc->i1;
    goto L_fnd;
  }

  if(fc->i3 == ip1) {
    *ip2 = fc->i1;
    *ip3 = fc->i2;
    // goto L_fnd;
  }


  L_fnd:

  // printf("ex UFA_2ip_ck_ptfac: ip2=%d ip3=%d ip1=%d\n",*ip2,*ip3,ip1);

  return 0;

}


//=======================================================================
  int UFA_fnb_get_2ip (Fac3 *fa, int fNr, int ie1, int ie2) {
//=======================================================================
// find neighbourface for edge ie1-ie2.
// oriented - get face on right side.
// direction of neighbouredge is opposite !
// Output:
//   retCod  0-fNr   nr of face with edge ie1-ie2
//           -1      Face not found

// see also UFA_2if_find_2ip

  int    i1;


  // printf("UFA_fnb_get_2ip %d %d\n",ie1,ie2);


  for(i1=0; i1<fNr; ++i1) {
    // test i1-i2
    if(fa[i1].i1 == ie2) {
      if(fa[i1].i2 == ie1) goto L_exit;

    // test i2-i3
    } else if(fa[i1].i2 == ie2) {
      if(fa[i1].i3 == ie1) goto L_exit;

    // test i3-i1
    } else if(fa[i1].i3 == ie2) {
      if(fa[i1].i1 == ie1) goto L_exit;
    }
  }

  i1 = -1;


  L_exit:
    // printf("ex UFA_fnb_get_2ip f=%d p1=%d p2=%d\n",i1,ie1,ie2);
  return i1;

}


//==============================================================================
  int UFA_2fac_facpt (MemTab(Fac3) *fTab, Fac3 *fnb, int ipt, int ifc, int ie) {
//==============================================================================
// create 2 new Faces tru point ipt instead of face ifc
// ipt lies on Edge ie of face ifc
// all faces CCW.
// Input:
//   ifc      index of inputface
//   ie       esn of edge with ipt 
//   ipt      point-index of new point on edge ie on face fTab->data[ifc]
//
//              /| i3
//            / /|
//          /  / |               ie = 1
//       /    /  |
//    /     /    |
//   -------------
// i1       ipt   i2
//  i1-ipt-i3   <<< statt i1-i2-i3    <<<  ie = 1
//  ipt-i2-i3
//
//
//              /| i3
//            /  |
//          / __/|ipt              ie = 2
//       /_ _/   |
//    / _/       |
//   -------------
// i1             i2
//  i1-i2-ipt   <<< statt i1-i2-i3    <<<  ie = 2
//  i1-ipt-i3
//
//
//              /| i3
//         ipt/  |
//          / \  |               ie = 3
//       /     \ |
//    /         \|
//   -------------
// i1             i2
//  i1-i2-ipt   <<< statt i1-i2-i3    <<<  ie = 3
//  i2-i3-ipt
//


  int   i1, i2, i3, fNr;
  Fac3  *f1;


  // TESTBLOCK
  // if(ifc == 132) {
    // printf("UFA_2fac_facpt ipt=%d ifc=%d ie=%d\n",ipt,ifc,ie);
    // UFA_fnb_dump_1 (&fTab->data[ifc], &fnb[ifc], "2fac_facpt %d ",ifc);
  // }
  // TESTBLOCK


  f1 = &fTab->data[ifc];
  i1 = f1->i1;
  i2 = f1->i2;
  i3 = f1->i3;
  fNr = fTab->rNr;

  if(ie == 1) {
    // make i1, ipt, i3
      // printf(" 2fac_facpt-1 f%d %d %d %d\n",ifc, ipt, i2, i3);
    // change f[ifc]
    UFA_facpt_chg__ (fTab->data, ifc, i1, ipt, i3);

    // create face  ipt,i2,i3
      // printf(" 2fac_facpt-1 f%d %d %d %d\n",fTab->rNr, ipt, i2, i3);
    UFA_add_fac (fTab, ipt, i2, i3);

    // update fnb[fNr]; keep i1,i2.
    fnb[fNr].st = 16;  // mark invalid
    // fnb[fNr].i3 = ifc;

    // update fnb[ifc]; keep i1,i2.
    fnb[ifc].st = 16;  // mark invalid
    // fnb[ifc].i2 = fNr;


  } else if(ie == 2) {
    // make i1, i2, ipt
      // printf(" 2fac_facpt-2 f%d %d %d %d\n",ifc, i1, i2, ipt);
    UFA_facpt_chg__ (fTab->data, ifc, i1, i2, ipt);
    fnb[ifc].st = 16;  // mark invalid
    // make i1, ipt, i3
      // printf(" 2fac_facpt-2 f%d %d %d %d\n",fTab->rNr, i1, ipt, i3);
    UFA_add_fac (fTab, i1, ipt, i3);
    fnb[fTab->rNr - 1].st = 16;  // mark invalid


  } else if(ie == 3) {
    // make i1, i2, ipt
      // printf(" 2fac_facpt-3 f%d %d %d %d\n",ifc, i1, ipt, i3);
    UFA_facpt_chg__ (fTab->data, ifc, i1, i2, ipt);
    fnb[ifc].st = 16;  // mark invalid
    // make i2, i3, ipt
      // printf(" 2fac_facpt-3 f%d %d %d %d\n",fTab->rNr, i2, i3, ipt);
    UFA_add_fac (fTab, i2, i3, ipt);
    fnb[fTab->rNr - 1].st = 16;  // mark invalid
  }

    // TESTBLOCK
    // if(ifc == 80) {
    // UFA_fnb_dump_1 (&fTab->data[ifc], &fnb[ifc], "ex c_facpt %d ",ifc);
    // }
    // TESTBLOCK

  return 0;

}


//======================================================================
  int UFA_3fac_facpt (MemTab(Fac3) *fTab, Fac3 *fnb, int ipt, int ifc) {
//======================================================================
// 3 faces from 1 face and point inside face
// create 3 new Faces tru point ipt instead of face ifc
// all faces CW
//
//            i3
//          /   \
//         /   |  \
//        /   ipt   \
//     fNr+1 /   \ fNr\
//      / /   ifc    \  \
//    i1 --------------- i2
//
//  ifc:    i1-i2-ipt   <<< statt i1-i2-i3
//  fNr:    i2-i3-ipt
//  fNr+1:  i3-i1-ipt


  int   i1, i2, i3, fNr;
  Fac3  *f1;


  // printf("UFA_3fac_facpt %d %d\n",ifc,ipt);


  f1 = &fTab->data[ifc];
  fNr = fTab->rNr;
  i1 = f1->i1;
  i2 = f1->i2;
  i3 = f1->i3;
    // printf(" f%d = %d %d %d; new f%d and f%d\n",ifc,i1,i2,i3,
           // fTab->rNr,fTab->rNr + 1);


  UFA_facpt_chg__ (fTab->data, ifc, i1, i2, ipt);

  UFA_add_fac (fTab, i2, i3, ipt);

  UFA_add_fac (fTab, i3, i1, ipt);

  // update fnb
  fnb[ifc].st = 16;  // mark invalid
  fnb[fTab->rNr - 2].st = 16;  // mark invalid
  fnb[fTab->rNr - 1].st = 16;  // mark invalid


    // TESTBLOCK
    // printf("========= UFA_3fac_facpt Pt %d MOD %d CREATE %d %d ======\n",
           // ipt,ifc,fNr,fNr+1);
    // UFA_fnb_dump_1 (&fTab->data[ifc], &fnb[ifc], " %d ",ifc);
    // UFA_fnb_dump_1 (&fTab->data[fNr], &fnb[fNr], " %d ",fNr);
    // UFA_fnb_dump_1 (&fTab->data[fNr+1], &fnb[fNr+1], " %d ",fNr+1);
    // TESTBLOCK


  return 0;

}


//================================================================
  int UFA_facpt_chg_psn (Fac3 *fac1, int ival, int esn) {
//================================================================
// UFA_facpt_chg_psn       set 1 pointer in face (seqnr)


  if(esn == 1) {
    fac1->i1 = ival;

  } else if(esn == 2) {
    fac1->i2 = ival;

  } else if(esn == 3) {
    fac1->i3 = ival;

  }


  return 0;

}


//=============================================================
  int UFA_facpt_chg__ (Fac3 *fa, int fNr, int i1, int i2, int i3) {
//=============================================================

  fa[fNr].i1 = i1;
  fa[fNr].i2 = i2;
  fa[fNr].i3 = i3;

  fa[fNr].st = 0;

  return 0;

}


//================================================================
  int UFA_fac_dump_ (Fac3 *fa, int fNr, char *txt) {
//================================================================

  int   i1;

  printf("UFA_fac_dump_ %s\n",txt);

  for(i1=0; i1<fNr; ++i1) 
    printf("fa[%d] i1=%d i2=%d i3=%d\n",i1,fa[i1].i1,fa[i1].i2,fa[i1].i3);

  return 0;

}


//================================================================
  int UFA_fac_dump1 (Fac3 *fa, char *txt, ...) {
//================================================================

  va_list va;

  printf("f[");

  va_start(va, txt);
  vprintf(txt, va);  // print -> stdout
  va_end(va);

  printf("] i1=%d i2=%d i3=%d\n",fa->i1,fa->i2,fa->i3);

  return 0;

}
 

//====================================================================
  int UFA_fac_dump_f (Fac3 *fa, int fNr, char *fnExt, char *txt, ...) {
//====================================================================
/// UFA_fac_dump_f          dump faces into file

  va_list va;
  int     i1;
  FILE    *fpo;
  char    s1[128];
    
    
  sprintf(s1, "%sApp_%s.tmp", OS_get_tmp_dir(),fnExt);

  printf("UFA_fac_dump_f %s\n",s1);

  // get fList from file srcFiles
  if((fpo=fopen(s1,"w")) == NULL) {
    printf("****** err open %s\n",s1);
    return -1;
  }


  fprintf(fpo, "FFFFFFFFFFFFFFFF MSH_dumpf_fTab %d ",fNr);

  va_start(va, txt);
  vfprintf(fpo, txt, va);  // print -> stdout
  va_end(va);

  fprintf(fpo, "\n");


  for(i1=0; i1<fNr; ++i1) 
    fprintf(fpo, "fa[%d] i1=%d i2=%d i3=%d\n",i1,fa[i1].i1,fa[i1].i2,fa[i1].i3);
  fprintf(fpo, "--------------------------------------------------- \n");


  fclose (fpo);

  return 0;

}


//==================================================================
  int UFA_fac_ck_sr_1 (int ifc, Fac3 *fac, Point *pa) {
//==================================================================
// UFA_fac_ck_sr_1    test sense-of-rotation of a face
/// Output:
///   RetCod = 1 = CCW
///           -1 = CW


  int     sr;
  double  d1;


  // sr = UFA_fac_srar (&d1, &fa[ifc], pa);   // 3D
  // sr = UT2D_srar_inpt3 (&d1, 3, (int*)fac, pa);
  sr = UT2D_srar_3pt (&d1, (Point2*)&pa[fac->i1],
                           (Point2*)&pa[fac->i2],
                           (Point2*)&pa[fac->i3]);

  if(sr < 0)
    printf("*** CW-face[%d] %d %d %d\n",ifc,fac->i1,fac->i2,fac->i3);

  return sr;

}


//==================================================================
  int UFA_fac_ck_sr__ (Fac3 *fa, Point *pa, int fNr, char *txt, ...) {
//==================================================================
/// UFA_fac_ck_sr__      test sense-of-rotation of all faces 3D
/// print all faces not CCW


  int     i1, sr;
  double  d1;
  va_list va;

  printf("---- UFA_fac_ck_sr__ %d ",fNr);

  va_start(va, txt);
  vprintf(txt, va);  // print -> stdout
  va_end(va);

  printf("\n");


  for(i1=0; i1<fNr; ++i1) UFA_fac_ck_sr_1 (i1, &fa[i1], pa);

  return 0;

}


//==============================================================
  int UFA_add_fac (MemTab(Fac3) *fTab, int i1, int i2, int i3) {
//==============================================================
// UFA_add_fac        add face to MemTab(Fac3)

  int       irc;
  long      l1;
  Fac3      f1;


  // if(fTab->rNr == 134)
  // printf("UFA_add_fac f%d = %d %d %d\n",fTab->rNr,i1,i2,i3);


  f1.i1 = i1;
  f1.i2 = i2;
  f1.i3 = i3;

  f1.st = 0;

  return MemTab_sav ((MemTab*)fTab, &l1, &f1, 1);

}


//================================================================
  int UFA_2ip_fac_esn (int *i1, int *i2, int esn, Fac3 *fc1) {
//================================================================
// get the pointNr's from Face and its EdgeSeqNr (1|2|3)
// Input:
//   fc1     a single face
//   esn    1=EdgeNr 1 (i1-i2) or 2 (i2-i3) or 3 (i3-i1) in face <fc1>
// Output:
//   i1,i2  index to points os edge <esn> in face <fc1>


  if(esn < 0) esn = -esn;

  // printf("UFA_2ip_fac_esn %d\n",esn);


  // irc=1: Edge i1-i2;
  if(esn == 1) {
    *i1 = fc1->i1;
    *i2 = fc1->i2;

  // irc=2: Edge i2-i3;
  } else if(esn == 2) {
    *i1 = fc1->i2;
    *i2 = fc1->i3;

  // irc=3: Edge i3-i1;
  } else if(esn == 3) {
    *i1 = fc1->i3;
    *i2 = fc1->i1;
  }

  // printf("ex UFA_2ip_fac_esn %d %d\n",*i1,*i2);

  return 0;

}


//================================================================
  int UFA_2ip_fac_ip (int *ip1, int *ip2, Fac3 *fc1, int ipt) {
//================================================================
// get the 2 opposite points from face and point
// Input:
//   fc1      single face to test
//   ipt      one of the points of fc1
// Output:
//   retCod   -1   no ipt in fc1
//             1|2|3    esn of ip1


  if        (fc1->i1 == ipt) {
    *ip1 = fc1->i2;
    *ip2 = fc1->i3;
    return 2;

  } else if (fc1->i2 == ipt) {
    *ip1 = fc1->i3;
    *ip2 = fc1->i1;
    return 3;

  } else if (fc1->i3 == ipt) {
    *ip1 = fc1->i1;
    *ip2 = fc1->i2;
    return 1;
  }


  return -1;

}


//================================================================
  int UFA_2if_find_2ip (int *if1, int *ie1, int *if2, int *ie2,
                        int ip1, int ip2, Fac3 *fa, int fNr) {
//================================================================
// find the faces (1 or 2) with the unoriented Edge ip1-ip2
// Input:
//   ip1, ip2    points to find
// Output:
//   if1, if2    FaceNr's (-1 = no Face)
//   ie1, ie2    EdgeNr on Faces if1, if2; 1|2|3

// see also UFA_ifac_ck_edgo UFA_fnb_get_2ip

  int   i1, ie;


  // printf("UFA_2if_find_2ip %d %d %d\n",ip1,ip2,fNr);

  *if1 = -1;
  *if2 = -1;

  for(i1=0; i1<fNr; ++i1) {

    if(fa[i1].i1 == ip1) {
      if(fa[i1].i2 == ip2) {ie = 1; goto L_fnd;}
      if(fa[i1].i3 == ip2) {ie = 3; goto L_fnd;}
      continue;
    }

    if(fa[i1].i2 == ip1) {
      if(fa[i1].i1 == ip2) {ie = 1; goto L_fnd;}
      if(fa[i1].i3 == ip2) {ie = 2; goto L_fnd;}
      continue;
    }

    if(fa[i1].i3 == ip1) {
      if(fa[i1].i1 == ip2) {ie = 3; goto L_fnd;}
      if(fa[i1].i2 == ip2) {ie = 2; goto L_fnd;}
      continue;
    }

    continue;


    L_fnd:
    if(*if1 < 0) {
      // printf(" f1=%d e=%d (%d,%d,%d)\n",i1,ie,fa[i1].i1,fa[i1].i2,fa[i1].i3);
      *if1 = i1;
      *ie1 = ie;
      continue;
    }

    // printf(" f2=%d e=%d (%d,%d,%d)\n",i1,ie,fa[i1].i1,fa[i1].i2,fa[i1].i3);
    *if2 = i1;
    *ie2 = ie;
    break;
  }

  // printf("ex UFA_2if_find_2ip f: %d %d e: %d %d\n",*if1,*if2,*ie1,*ie2);

  return 0;

}


//================================================================
  int UFA_ifac_ck_edgo (int *if1, int *ie1,
                        int ip1, int ip2, Fac3 *fa, int fNr) {
//================================================================
// find the face with the oriented Edge ip1-ip2
// Input:
//   ip1,ip2     find face (if1) and edge (ie1) for this points
// Output:
//   if1         FaceNr (-1 = no Face)
//   ie1         EdgeNr on Face if1; 1|2|3

  int   i1, ie;


  // printf("UFA_ifac_ck_edgo %d %d %d\n",ip1,ip2,fNr);

  *if1 = -1;

  for(i1=0; i1<fNr; ++i1) {

    if(fa[i1].i1 == ip1) {
      if(fa[i1].i2 == ip2) {ie = 1; goto L_fnd;}
      continue;
    }

    if(fa[i1].i2 == ip1) {
      if(fa[i1].i3 == ip2) {ie = 2; goto L_fnd;}
      continue;
    }

    if(fa[i1].i3 == ip1) {
      if(fa[i1].i1 == ip2) {ie = 3; goto L_fnd;}
      continue;
    }

    continue;


    L_fnd:
      // printf(" f1=%d e=%d (%d,%d,%d)\n",i1,ie,fa[i1].i1,fa[i1].i2,fa[i1].i3);
    *if1 = i1;
    *ie1 = ie;
    break;
  }

    // printf("ex UFA_ifac_ck_edgo: f=%d %d e=%d %d\n",*if1,*ie1,ip1,ip2);

  return 0;

}


//=======================================================================
  int UFA_nifac_ck_pt (int *iTab, char *psna, int *iNr, int ipt,
                       Fac3 *fa, int fNr) {
//=======================================================================
// get all faces going tru (using) point with index ipt
// Input:
//   ipt    pointIndex; search all faces this point is using
//   iNr    size for iTab,psna
// Output:
//   iTab   table of faces using point ipt
//   psna   esn 1-if ipt=p1; 2-if ipt=p2; 3-if ipt=p3;
//   iNr    nr of faces in iTab,psna

  int   i1, iSiz, ii;

  iSiz = *iNr;
  ii = 0;

  for(i1=0; i1<fNr; ++i1) {
    if(fa[i1].i1 == ipt) {psna[ii] = 1; goto L_add;}
    if(fa[i1].i2 == ipt) {psna[ii] = 2; goto L_add;}
    if(fa[i1].i3 == ipt) {psna[ii] = 3; goto L_add;}
    continue;

    L_add:
      iTab[ii] = i1;
      if(ii < iSiz) {
        ++ii;
      } else {
        TX_Print("UFA_nifac_ck_pt E001");
        printf("***** ERROR UFA_nifac_ck_pt iSiz=%d ii=%d\n",iSiz,ii);
        return -1;
      }
  }

  *iNr = ii;


    // printf("ex UFA_nifac_ck_pt %d %d\n",ipt, ii);
    // for(i1=0;i1<ii; ++i1)
    // printf("nifac_ck_pt[%d]=%d %d\n",i1,iTab[i1],psna[i1]);

  return 0;

}


//================================================================
  int UFA_fac_chg_sr (Fac3 *fc) {
//================================================================
// UFA_fac_chg_sr     change sr of face
// change 1 2 3 -> 1 3 2 (swap i2 - i3)

  MEM_swap_int (&fc->i2, &fc->i3);

  return 0;

}


//================================================================
  int UFA_fac_srar2 (double *ar, Fac3 *fc, Point2 *p2a) {
//================================================================
// UFA_fac_srar2           get (signed) area of face 2D
/// Output:
///   RetCod = 1 = CCW
///           -1 = CW


  // printf("UFA_fac_srar %d %d %d\n",fc->i1, fc->i2,fc->i3);

  // return UT2D_srar_inpt (ar, 3, (int*)fc, p2a);
  return UT2D_srar_3pt (ar, &p2a[fc->i1], &p2a[fc->i2], &p2a[fc->i3]);

}



//================================================================
  int UFA_fac_srar (double *ar, Fac3 *fc, Point *pa) {
//================================================================
// UFA_fac_srar            get (signed) area of face 3D
/// Output:
///   RetCod = 1 = CCW
///           -1 = CW


  // printf("UFA_fac_srar %d %d %d\n",fc->i1, fc->i2,fc->i3);

  return UT2D_srar_inpt3 (ar, 3, (int*)fc, pa);

}


//================================================================
  int UFA_psn_1fac_ip (Fac3 *fac, int ip) {
//================================================================
// get pointSeqNr of pointNr in face
// ips is one of the points in fac; return its pointSeqNr (1|2|3).
// RetCod=-1    Error; ip is not used by fac.
  
  int   psn = -1; 
    
  if     (fac->i1 == ip)      {  psn = 1; }
  else if(fac->i2 == ip)      {  psn = 2; }
  else if(fac->i3 == ip)      {  psn = 3; }
    
  return psn;
  
}
  

//================================================================
  int UFA_ip_fac_psn (Fac3 *fac, int psn) {
//================================================================
// UFA_ip_fac_psn          get pointNr from faceNr pointSeqNr
// Input:
//   psn    PointSeqNr:     1,2,3;
    
  int   ii;
    
    
  if(psn == 1) {
    ii = fac->i1;

  } else if(psn == 2) {
    ii = fac->i2;
    
  } else if(psn == 3) {
    ii = fac->i3;
  } 
    

  return ii;

}


//================================================================
  int UFA_ipe_fac_esn (Fac3 *fac, int psn) {
//================================================================
// UFA_ipe_fac_esn    get pointIndex of end of edge from face, edgeSeqNr

  int   ii;


  if(psn == 1)      ii = fac->i2;
  else if(psn == 2) ii = fac->i3;
  else if(psn == 3) ii = fac->i1;

  return ii;

}


//================================================================
  int UFA_ipOpp_fac_esn (int *ipOpp, int *psn, Fac3 *fac1, int esn) {
//================================================================
// UFA_ipOpp_fac_esn   get point opposite to edge and its pointseqNr
// esn is the EdgeSeqN


  if(esn == 1) {
    *psn = 3;
    *ipOpp = fac1->i3;

  } else if(esn == 2) {
    *psn = 1;
    *ipOpp = fac1->i1;

  } else if(esn == 3) {
    *psn = 2;
    *ipOpp = fac1->i2;
  }

  // printf("ex UFA_ipOpp_fac_esn opp=%d psn=%d esn=%d (%d,%d,%d)\n",
          // *ipOpp, *psn, esn,
          // fac1->i1, fac1->i2, fac1->i3);

  return 0;

}


//================================================================
  int UFA_int_fac_ln2 (int *ieo, int iei,
                          Point2 *ptl, Vector2 *vcl,
                          Fac3 *fac, Point *pa) {
//================================================================
// UFA_int_fac_ln2    intersect face - 2D-line; FACE MUST BE CCW
// Line comes tru Edge iei into Face fac; check intersection.
// Input:
//   iei       edgeNr where line ptl,vcl comes into fac
//   ptl       startpoint Line
//   vcl       vector Line
//   fac       1 face
//   pa        ptArr
// Output:
//   if RetCod=0         normal intersection Line-Edge
//     ieo =   1|2|3     the EdgeSeqNr of the Edge intersected by Line
//   if RetCod=1         Line goes tru point opposit to edge iei;
//     ieo =   ipOpp     PointNr of the Point opposit to Edge iei


  int      irc, ipo, eso, s1;
  double   d1;
  Point2   ptOpp;
  Vector2  vp;


  printf("UFA_int_fac_ln2 pl %f %f vl %f %f\n",ptl->x,ptl->y,vcl->dx,vcl->dy);
  printf("  ei=%d f=%d-%d-%d \n",iei,fac->i1,fac->i2,fac->i3);


  // get ipo = the point opposit to Edge iei into fac
  // and eso = edge#starting with ipo
  UFA_ipOpp_fac_esn (&ipo, &eso, fac, iei);

  ptOpp = UT2D_pt_pt3 (&pa[ipo]);


  // test if point pa[ipo] is left or right of Line
  // 0=on, 1=left, -1=right
  s1 = UT2D_sid_ptvc (&ptOpp, ptl, vcl);
    printf("  _sid_ptvc ptOpp %f %f s=%d ipo=%d\n",ptOpp.x,ptOpp.y,s1,ipo);


  if(s1 == 0) {
    irc = 1;
    *ieo = ipo;  // the PointSeqNr of ptOpp

  } else {
    irc = 0;
    if(s1 > 0) {      // ptOpp is left of line
      *ieo = UFA_esn_prv (iei); 

    } else {          // ptOpp is right of line
      *ieo = UFA_esn_nxt (iei);
    }
  }





/*
  UT2D_vc_2pt (&vp, ptl, &pa[ipo]);
  d1 = UT2D_sar_2vc (vcl, &vp);
  s1 = DSIGTOL (d1, UT_TOL_min0);      // pos: ptOpp is left of line
    // printf(" s1=%d d1=%f\n",s1,d1);

  if(s1 == 0) {
    irc = 1;
    *ieo = eso;  // the PointSeqNr of ptOpp

  } else {
    irc = 0;
    if(s1 > 0) {      // ptOpp is left of line
      *ieo = UFA_esn_nxt (iei);    // next edge

    } else {          // ptOpp is right of line
      *ieo = UFA_esn_prv (iei);    // previous edge
    }
  }
*/
    // printf("ex UFA_int_fac_ln2 irc=%d ieo=%d\n",irc,*ieo);

  return irc;

}


//================================================================
  int UFA_2pt2_fac_esn (Point2 *pe1, Point2 *pe2, int esn,
                        Fac3   *fac,  Point *pa) {
//================================================================
// UFA_2pt2_fac_esn   get 2D-points of edge of face + EdgeSeqNr
// Input:
//   fac    Face
//   esn    EdgeSeqNr (1=p1 to p2 ..)
// Output:
//   2 2D-points  ( - Edge)


  if(esn == 1) {
    *pe1 = UT2D_pt_pt3 (&pa[fac->i1]);
    *pe2 = UT2D_pt_pt3 (&pa[fac->i2]);

  } else if(esn == 2) {
    *pe1 = UT2D_pt_pt3 (&pa[fac->i2]);
    *pe2 = UT2D_pt_pt3 (&pa[fac->i3]);

  } else if(esn == 3) {
    *pe1 = UT2D_pt_pt3 (&pa[fac->i3]);
    *pe2 = UT2D_pt_pt3 (&pa[fac->i1]);
  }


  return 0;

}


//================================================================
  int UFA_ck_2ip_ipa (int ip1, int ip2, int *ipa, int ipNr) {
//================================================================
// test if edge ip1-ip2 is on polygon ipa[ipNr]; oriented, closed.
// RetCod:
//    0         OK, yes
//   -1         No, edge not in ipa

// see also UFA_BL_cke MSH_EL_ck_ln (test all EL's)



  int    i1, ie, irc;

  irc = 0;

  ie = ipNr - 1;

  for(i1=0; i1<ipNr; ++i1) {

    if(ipa[i1] != ip1) continue;
    if(ipa[i1 + 1] == ip2) goto L_yes;
  }

  if((ipa[ipNr - 1] == ip1) && (ipa[0] == ip2)) goto L_yes;


  // no, ip1-ip2 ist not in pointarray ipa.
  irc = -1;


  // yes, ip1-ip2 ist in pointarray ipa.
  L_yes:

  // printf("ex UFA_ck_2ip_ipa irc=%d ip %d %d\n",irc,ip1,ip2);

  return irc;

}


//======================================================================
  int UFA_2esn_comm2fac (int *esn1, int *esn2, Fac3 *fac1, Fac3 *fac2) {
//======================================================================
// get common edges of 2 faces with common edge

// je 2 der 3 Punkte der beiden faces sind gleich !
// mögliche kombinationen:
// 1-2 2-1   1-2 3-2   1-2 1-3
// 2-3 2-1   2-3 3-2   2-3 1-3
// 3-1 2-1   3-1 3-2   3-1 1-3


  // printf("UFA_2esn_comm2fac \n");
  // printf(" fac1 =%d %d %d\n",fac1->i1,fac1->i2,fac1->i3);
  // printf(" fac2 =%d %d %d\n",fac2->i1,fac2->i2,fac2->i3);


  if     (fac1->i1 == fac2->i2) {
    if   (fac1->i2 == fac2->i1) {
      *esn1 = 1; *esn2 = 1; }    // 1-2 2-1
    else {
      *esn1 = 3; *esn2 = 2; }    //
  }
  else if(fac1->i1 == fac2->i3) {
    if   (fac1->i2 == fac2->i2) {
      *esn1 = 1; *esn2 = 2; }    // 1-2 2-3
    else {
      *esn1 = 3; *esn2 = 3; }    // 3-1 1-3
  }
  else if(fac1->i1 == fac2->i1) {
    if   (fac1->i2 == fac2->i3) {
      *esn1 = 1; *esn2 = 3; }    // 1-2 1-3
    else {
      *esn1 = 3; *esn2 = 1; }    // 3-1 1-3
  }

  else if(fac1->i2 == fac2->i2) {
      *esn1 = 2; *esn2 = 1; }    // 2-3 2-1

  else if(fac1->i2 == fac2->i3) {
       *esn1 = 2; *esn2 = 2; }    // 2-3 3-2

  else if(fac1->i2 == fac2->i1) {
       *esn1 = 2; *esn2 = 3; }    // 2-3 1-3


  // printf("ex UFA_2esn_comm2fac %d %d\n",*esn1,*esn2);


  return 0;

}


/*
//================================================================
  int UFA_esn_opp_psn (int ipt) {
//================================================================
// UFA_esn_opp_psn            get EdgeSeqNr of Edge opposite to PointSeqNr
// EdgeSequenceNrs: 1-3; PointSequenceNrs:1-3.
// Input:
//   ip      pointNr; 1|2|3
// Output:
//   irc     edgeNr (1|2|3) of Edge opposite to point with # ipt
//           returns 2 from ipt=1;            1 -> 2; 2 -> 3; 3 -> 1.

//                 x3     (x = pointNrs)
//               / |
//             e3  e2     (e = edgeNrs)
//           /     |
//        1x---e---x2
//             1

  if(ipt < 3) return ipt + 1;
  else        return 1;

}
// #define UFA_esn_opp_psn(esn) ((esn) < 3 ? ++esn : 1)

//================================================================
  int UFA_esn_nxt (int esn) {
//================================================================
// UFA_esn_nxt            get EdgeSeqNr of next Edge (CCW)
// EdgeSequenceNrs: 1-3;
// Input:
//   esn     edgeNr  (1|2|3)
// Output:
//   RetCod  edgeNr of Edge following Edge esn
//           1 -> 2; 2 -> 3; 3 -> 1.
// Usage:
//   esn1 = UFA_esn_nxt (esn1);

// see also UFA_esn_opp_psn

  if(esn < 3) return ++esn;
  else        return 1;

}
// #define UFA_esn_nxt(esn) ((esn) < 3 ? ++esn : 1)

//================================================================
  int UFA_esn_prv (int esn) {
//================================================================
// UFA_esn_prv            get EdgeSeqNr of previous Edge (CCW)
// EdgeSequenceNrs: 1-3;
// Input:
//   esn     edgeNr  (1|2|3)
// Output:
//   RetCod  edgeNr of Edge before Edge esn
//           1 -> 3; 2 -> 1; 3 -> 2.


  if(esn < 2) return 3;
  else        return --esn;

}
// #define UFA_esn_prv(esn) ((esn) < 2 ? 3 : --esn)
*/


//============================================================================
  int UFA_fnb_init_el (MemTab(EdgeLine) *eTab, Fac3 *fnb, Fac3 *fa, int fNr) {
//============================================================================
// UFA_fnb_init_el         set all NeighbourFacNrs -2, if Edge is a BreakLine
// Bei den NachbarFacNrs FaceNr -2, wenn Edge eine BreakLine ist.

//       -2 = this edge is a breakline (-TYP_EDGLN_BL)

// TODO: set 
//       -1 = this edge is unresolved (already done
//       -3 = this edge is a inner breakline (-TYP_EDGLN_IB)
//       -4 = this edge is a outer breakline (-TYP_EDGLN_OB)
// check with sap ?


  int      i1, i2, *ia, iNr, ip1, ip2, iFac, esn1, inbFac, esn2;


  // printf("UFA_fnb_init_el \n");
  if(fNr < 2) {
    printf(" UFA_fnb_init_el E001\n");
    return -1;
  }


  for(i1=0; i1<eTab->rNr; ++i1) {
    if(eTab->data[i1].typ != TYP_EDGLN_BL) continue;

    ia  = eTab->data[i1].ia;
    iNr = eTab->data[i1].iNr - 1;  // letztes - 1 !

    // loop tru segments of active BL
    for(i2=0; i2<iNr; ++i2) {
      ip1 = ia[i2];
      ip2 = ia[i2 + 1];
        // printf(" EL[%d] %d %d\n",i1,ip1,ip2);
      // find ifac und esn of oriented edge ip1-ip2
      UFA_ifac_ck_edgo (&iFac, &esn1, ip1, ip2, fa, fNr);
      if(iFac < 0) {
        // no neighbour exists: must be eg inner boundary or outside OB
        // occurs if BL goes outside OB
        // printf("**** UFA_fnb_init_el I001 BL %d %d\n",ip1,ip2);
        // TX_Print("UFA_fnb_init_el E001 BL %d %d",ip1,ip2);
        continue;
      }
      // get neigbourfaceNr
      inbFac = UFA_if_getNf_ife (iFac, esn1, fnb);
      if(inbFac < 0) continue;  // // no neighbour ..
      // get esn of edge starting with ip2 in face fa[inbFac]
      // esn2 = UFA_esn_2ip (ip2, fa, inbFac);
      esn2 = UFA_esn_ips (&fa[inbFac], ip2);
        // printf(" iFac=%d esn=%d inbFac=%d esn=%d\n",iFac,esn1,inbFac,esn2);

      // mark edges -2
      UFA_facpt_chg_psn (&fnb[iFac], -2, esn1);
      UFA_facpt_chg_psn (&fnb[inbFac], -2, esn2);
    }
  }

  // UFA_fnb_dump__ (fa, fnb, fNr);

  return 0;

}


//============================================================================
  int UFA_nfb_ck_1 (int ifc, int ec, Fac3 *fa, Fac3 *fnb) {
//============================================================================
// check neigbours of face ifc, edge ie
// RetCod: 0 = OK else error

//              /| i3
//            /  |
//          /    |      
//       /   ifc |     ifn
//    /          |
//   -------------
// i1             i2
    

  int     irc, ifn, en, i1, i2, i3, i4;
    

  // get neighb.face and esn on this face for ifc-ec
  irc = UFA_ife_getNf_ife (&ifn, &en, ifc, ec, fa, fnb);
  if(irc < 0) return irc;
  if(ifn < 0) return 0;       // neighbour is outer boundary


  // get points of ifc,ec
  UFA_2ip_fac_esn (&i1, &i2, ec, &fa[ifc]);

  // get points of ifn,en
  UFA_2ip_fac_esn (&i3, &i4, en, &fa[ifn]);

  irc = 0;

  if(i1 != i4) {
    printf("**** UFA_nfb_ck_1 E014 f=%d e=%d fn=%d en=%d\n",ifc,ec,ifn,en);
    ++irc;
  }

  if(i2 != i3) {
    printf("**** UFA_nfb_ck_1 E023 f=%d e=%d fn=%d en=%d\n",ifc,ec,ifn,en);
    ++irc;
  }


  if(irc) {
    UFA_fnb_dump_1 (&fa[ifc], &fnb[ifc], " %d ",ifc);
    UFA_fnb_dump_1 (&fa[ifn], &fnb[ifn], " %d ",ifn);
  }

  return irc;

}


//================================================================
  int UFA_nfb_ck_f (int ifc, Fac3 *fa, Fac3 *fnb) {
//================================================================
// check single nfb-struct

  int  irc;
  

  irc  = UFA_nfb_ck_1 (ifc, 1, fa, fnb);
  irc += UFA_nfb_ck_1 (ifc, 2, fa, fnb);
  irc += UFA_nfb_ck_1 (ifc, 3, fa, fnb);

  return irc;

} 


//============================================================================
  int UFA_nfb_ck__ (Fac3 *fa, Fac3 *fnb, int fNr, char *txt, ...) {
//============================================================================
// check all nfb-structs

  int          i1, irc;
  va_list      va;
  
  printf("----------- UFA_nfb_ck__ %d ", fNr);

  va_start(va, txt);
  vprintf(txt, va);  // print -> stdout
  va_end(va);

  printf("\n");

  irc = 0;

  for(i1=0; i1<fNr; ++i1) irc += UFA_nfb_ck_f (i1, fa, fnb);

  if(irc) {
    UFA_fnb_dump__ (fa, fnb, fNr, "nfb_ck"); 
    return -99;
  }
  
  printf("ex UFA_nfb_ck__ OK\n");
  return irc;

}


//================  EOF  ==================================
