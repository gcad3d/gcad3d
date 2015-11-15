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
UFA_chg_fac        set face
UFA_chg1_fac       set 1 pointer in face (seqnr) from esn
UFA_2fac_facpt     create 2 new Faces from 1 face (point on edge)
UFA_3fac_facpt     create 3 new Faces from 1 face
UFA_mod_delPt      change all face-pointers >= ip to ip-1

UFA_ifac_ck_edgu   find (1|2) faces from unoriented Edge (2 ipt's)
UFA_ifac_ck_edgo   find face from oriented Edge (2 ipt's)
UFA_nifac_ck_pt    get all faces going tru point with index ipt
UFA_if_findSegP    find face with LineSegment ips-ps2 inside
UFA_if_find_ptmsh  find faceNr of point (2D-test point inside face)

UFA_esn_2ip        get esn from startpoint of edge
UFA_esn_nxt        get Edge|PointSeqNr of next Edge|Point (CCW)           INLINE
UFA_esn_prv        get Edge|PointSeqNr of previous Edge|Point (CCW)       INLINE
UFA_esn_opp_psn    get EdgeSeqNr of Edge opposite to PointSeqNr           INLINE
UFA_2esn_comm2fac  get common edges of 2 faces with common edge

UFA_ipOpp_fac_esn  get point opposite to edge
UFA_ipOpp_facedg   get point opposite to edge (ip3 & ptNr from ip1)
UFA_ip_fac_psn     get pointNr from faceNr pointSeqNr
UFA_2ip_ck_ptfac   ck if ip1 is used in face; get ip2, ip3.
UFA_2ip_fac_esn    get pointNrs for Edge from face + EdgeSeqNr
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
UFA_esn_edg        get EdgeSequenceNumber of face from pointNr
UFA_pFac3_esn      get pointers into Face

UFA_fnb_init__     create neighbourfaces for mesh
UFA_fnb_init_el    set NeighbourFacNr -2, if Edge is a BreakLine
UFA_inbFac_esn     get neighbourFace from esn
UFA_edg_find_facn  find neighbourface for edge
UFA_setinb         set neighbour

UFA_ar_fac         get (signed) area of face

UFA_BL_cke         test if ip1-ip2 is a BreakLine.

UFA_dump1          dump face and its neighbours
UFA_dump_fnb       dump faces and neighbours

List_functions_end:
=====================================================
- see also:
UT2D_ck_pt_in_tria
../ut/ut_tria.c     Triangle-functions    (using struct Point)
../ut/ut_msh.c      MSH   Funktionen fuer Meshes (MemTab)

\endcode *//*----------------------------------------



// PointSeqNr psn   1,2,3;
// EdgeSeqNr  esn:  1 (from p1 to p2), 2 (from p2 to p3), 3 (from p3 to p1)
//
//                 p3     (x = pointNrs)
//               / |
//             e3  e2     (e = edgeNrs)
//           /     |
//        1p---e---p2
//             1




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
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_tin.h"              // TYP_EDGLN_BL
#include "../gr/ut_UI.h"               // UI_Func... SYM_..
#include "../ut/ut_TX.h"               // TX_Print



typedef_MemTab(Fac3);
typedef_MemTab(EdgeLine);




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


//================================================================
  int UFA_BL_cke (int ie1, int ie2, MemTab(EdgeLine) *eTab) {
//================================================================
/// \code
/// UFA_BL_cke         test if ip1-ip2 is a BreakLine, not oriented.
/// RetCod: 0   yes; ie1-ie2 is a BreakLine;
///        -1   no;  ie1-ie2 is no BreakLine.
/// \endcode

// see also MSH_EL_ck_ln


  int   irc, i1, i2, iNr, *ia;



  // loop tru eTab; test BL's
  for(i1=0; i1<eTab->rNr; ++i1) {
    if(eTab->data[i1].typ != TYP_EDGLN_BL) continue;
    ia  = eTab->data[i1].ia;
    iNr = eTab->data[i1].iNr - 1;  // letztes - 1 !
      // printf(" _BL_cke i1=%d iNr=%d\n",i1,iNr);
    // loop tru points of a IB
    for(i2=0; i2<iNr; ++i2) {
        // printf(" _BL_cke i1=%d i2=%d e1=%d e2=%d\n",i1,i2,ia[i2],ia[i2+1]);
      // test next edge; EL sind nicht orientiert !
      if(ia[i2] == ie1)
        if(ia[i2 + 1] == ie2) goto L_yes;
      if(ia[i2 + 1] == ie1)
        if(ia[i2] == ie2) goto L_yes;
    }
  }

  // no; is not BreakLine.
    // printf("ex UFA_BL_cke -1 %d %d\n",ie1,ie2);
  return -1;


  L_yes:
    // printf("ex UFA_BL_cke 0 %d %d\n",ie1,ie2);
  return 0;

}


//================================================================
  int UFA_inbFac_esn (int iFac, int iEdg, Fac3 *fnb) {
//================================================================
// get neighbourFace from esn  (iFac, iEdg)
// Input:
//   iFac           face-nr
//   iEdg           esn
// RetCod:          ind of UFA_inbFac_esn


  int    nbFac;


  if(iEdg == 1) {
    nbFac = fnb[iFac].i1;

  } else if(iEdg == 2) {
    nbFac = fnb[iFac].i2;

  } else if(iEdg == 3) {
    nbFac = fnb[iFac].i3;

  }


  // printf("ex UFA_inbFac_esn nbFac=%d %d %d\n",nbFac,iFac,iEdg);

  return nbFac;

}


//================================================================
  int UFA_fnb_init__ (Fac3 *fnb, Fac3 *fa, int fNr) {
//================================================================
/// \code
/// Zu den Faces fa eine Parallelstructur fnb aufbauen,
/// in denen die NachbarFacNrs gespeichert sind.
/// (Fac3*)fnb: i1 ist der Index zum Nachbarface der Kante 1
///    (Kante 1 geht von Punkt 1 zu Punkt 2) usw.
///    Index -1: kein NachbarFace gefunden (outer boundary).
/// EdgeSeqNrs der NachbarFaces werden nicht geliefert.
/// \endcode

// OFFEN:
// haette man sap; koennte man testen ob ie1 && ie2 BoundaryPoints sind;
//   wenn ja: kein neighbourface moeglich.


  int    i1, ie1, ie2, ifn, ien;


  // printf("UFA_fnb_init__ %d\n",fNr);


  // init all neighbours with -2 (unresolved)
  for(i1=0; i1<fNr; ++i1) {
     fnb[i1].i1 = -2;
     fnb[i1].i2 = -2;
     fnb[i1].i3 = -2;
  }


  // loop tru faces; find unresolved Faces;
  // gegenueberliegende FaceNr eintragen.
  for(i1=0; i1<fNr; ++i1) {

    // loop tru edges of face i1
    ie1 = fa[i1].i1;
    ie2 = fa[i1].i2;
    ifn = UFA_edg_find_facn (fa, fNr, ie1,ie2);
      // printf(" fnb[%d]-e1(%d-%d) = %d\n",i1,ie1,ie2,ifn);
    fnb[i1].i1 = ifn;


    ie1 = fa[i1].i2;
    ie2 = fa[i1].i3;
    ifn = UFA_edg_find_facn (fa, fNr, ie1,ie2);
      // printf(" fnb[%d]-e2(%d-%d) = %d\n",i1,ie1,ie2,ifn);
    fnb[i1].i2 = ifn;


    ie1 = fa[i1].i3;
    ie2 = fa[i1].i1;
    ifn = UFA_edg_find_facn (fa, fNr, ie1,ie2);
      // printf(" fnb[%d]-e3(%d-%d) = %d\n",i1,ie1,ie2,ifn);
    fnb[i1].i3 = ifn;

  }


  // dump neigbours:
  // UFA_dump_fnb (fa, fnb, fNr);

  return 0;

}


//====================================================================
  int UFA_opt__ (Fac3 *fa, Fac3 *fnb, int fNr, Point2 *p2a,
                MemTab(EdgeLine) *eTab, char *saf) {
//====================================================================
// Optimize: jeweils 2 Faces mit gemeinsamer Kante testen;

// Input:
//   saf    check for breaklines


// wenn Querkante kuerzer als gemeinsame Kante: Kante vertauschen.
// 
//               ie4
//                |
//     ie1--------+----------ie2
//                |
//               ie3


  int    irc, i1,  ie1, ie2, ie3, ie4, if1, if2, esn1, esn2, iNr;


  // printf("UFA_opt__ %d\n",fNr);
  // for(i1=0;i1<fNr;++i1) printf(" saf[%d]=%d;",i1,saf[i1]);
  // printf("\n");


  iNr = 0;

  // loop tru faces
  for(if1=0; if1<fNr; ++if1) {

    // test edge p1-p2
    esn1 = 1;
    if2 = fnb[if1].i1;
    if(if2 < 0) goto L_ck_e2;    // skip boundary
    if((saf[if1] == 2)&&(saf[if2] == 2)) goto L_ck_e2;  // skip BreakLine
      // printf(" opt1: %d %d\n",if1,if2);
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
    if(irc != 1) goto L_ck_e2;
    // flip
      // printf(" flip1 %d/%d %d/%d\n",if1,esn1,if2,esn2);
    UFA_2fac_flip (if1, esn1, if2, esn2, fa, fnb);
      // UT3D_stru_dump (Typ_Fac3, &fa[if1], "f[%d]",if1);
      // UT3D_stru_dump (Typ_Fac3, &fa[if2], "f[%d]",if2);
    if((saf[if1] == 2)||(saf[if2] == 2))   // update BreakLine
      UFA_opt_saf (if1, if2, ie1, ie2, ie3, ie4, eTab, saf);
    ++iNr;


    // test edge p2-p3
    L_ck_e2:
    esn1 = 2;
    if2 = fnb[if1].i2;
    if(if2 < 0) goto L_ck_e3;    // skip boundary
    if((saf[if1] == 2)&&(saf[if2] == 2)) goto L_ck_e3;  // skip BreakLine
      // printf(" opt2: %d %d\n",if1,if2);
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
    if(irc != 1) goto L_ck_e3;
    // flip
      // printf(" flip2 %d/%d %d/%d\n",if1,esn1,if2,esn2);
    UFA_2fac_flip (if1, esn1, if2, esn2, fa, fnb);
      // UT3D_stru_dump (Typ_Fac3, &fa[if1], "f[%d]",if1);
      // UT3D_stru_dump (Typ_Fac3, &fa[if2], "f[%d]",if2);
    if((saf[if1] == 2)||(saf[if2] == 2))   // update BreakLine
      UFA_opt_saf (if1, if2, ie1, ie2, ie3, ie4, eTab, saf);
    ++iNr;


    // test edge p3-p1
    L_ck_e3:
    esn1 = 3;
    if2 = fnb[if1].i3;
    if(if2 < 0) continue;        // skip boundary
    if((saf[if1] == 2)&&(saf[if2] == 2)) continue;  // skip BreakLine
      // printf(" opt3: %d %d\n",if1,if2);
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
    if(irc != 1) continue;
    // flip
      // printf(" flip3 %d/%d %d/%d\n",if1,esn1,if2,esn2);
    UFA_2fac_flip (if1, esn1, if2, esn2, fa, fnb);
      // UT3D_stru_dump (Typ_Fac3, &fa[if1], "f[%d]",if1);
      // UT3D_stru_dump (Typ_Fac3, &fa[if2], "f[%d]",if2);
    if((saf[if1] == 2)||(saf[if2] == 2))   // update BreakLine
      UFA_opt_saf (if1, if2, ie1, ie2, ie3, ie4, eTab, saf);
    ++iNr;


    // L_ck_ee:

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
*/

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
// 1) test if p3-p4 is crossing p1-p2; if yes:
// 2) test if p3-p4 is shorter than p1-p2

//
// test if p3-p4 intersect p1-p2;
//   no:  retCod = 0;
//   yes: retCod = 1;
// RetCodes:
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
  Vector2   v34;


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


  return 0;

}


//================================================================
  int UFA_2fac_flip (int if1, int esn1, int if2, int esn2,
                     Fac3 *fa, Fac3 *fnb) {
//================================================================
/// change diagonalLine;  common Edge is p1-p2; change to p3-p4.

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
  // UFA_dump1 (&fa[if1], &fnb[if1], "inp-f1:");
  // UFA_dump1 (&fa[if2], &fnb[if2], "inp-f2:");

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

  // dump if1 & if2
  // printf("ex UFA_2fac_flip:\n");
  // UFA_dump1 (&fa[if1], &fnb[if1], "out-f1:");
  // UFA_dump1 (&fa[if2], &fnb[if2], "out-f2:");


  // change neighours of f1n3 (set f2; was f1)
  //   and f2n3 (set f1; was f2)
    // printf(" f1n2=%d  f1p2=%d f=%d\n",fnb[if1].i2,fa[if1].i2,if1);
    // printf(" f2n2=%d  f2p2=%d f=%d\n",fnb[if2].i2,fa[if2].i2,if2);
  f1n2 = fnb[if1].i2; // neighbourface of new face 1 edge 2
  f2n2 = fnb[if2].i2; // neighbourface of new face 2 edge 2

  if(f1n2 >= 0)
  UFA_setinb (f1n2, fa[if1].i3, if1, fa, fnb); // f1n3 - 1 > 2
  if(f2n2 >= 0)
  UFA_setinb (f2n2, fa[if2].i3, if2, fa, fnb); // f2n3 - 2 > 1


  return 0;

}


//===================================================================
  int UFA_setinb (int ifc, int ie1, int ifnb, Fac3 *fa, Fac3 *fnb) {
//===================================================================
/// set the neighbour of edge starting with point ie1 of Face ifc to ifnb

  int   esn;


  // printf("UFA_setinb f=%d e=%d nb=%d\n",ifc,ie1,ifnb);

  // find edgeSequenceNumber of edge starting with point ie1
  esn = UFA_esn_edg (&fa[ifc], ie1);

  if(esn == 1)       fnb[ifc].i1 = ifnb;
  else if(esn == 2)  fnb[ifc].i2 = ifnb;
  else if(esn == 3)  fnb[ifc].i3 = ifnb;

  // UFA_dump1 (&fa[ifc], &fnb[ifc], "out-f%d:",ifc);

  return 0;

}


//================================================================
  int UFA_esn_edg (Fac3 *fac1, int ie1) {
//================================================================
/// get EdgeSequenceNumber of the Edge starting with point ie1

// see also UFA_ipOpp_facedg

  int   esn;

  if(fac1->i1 == ie1) {
    esn = 1;

  } else if(fac1->i2 == ie1) {
    esn = 2;

  } else if(fac1->i3 == ie1) {
    esn = 3;
  }

  // printf("ex UFA_esn_edg esn=%d ip(%d,%d,%d) ie=%d\n",
          // esn, fac1->i1, fac1->i2, fac1->i3, ie1);

  return esn;

}


//=======================================================================
  int UFA_pFac3_esn (int **ip1, int **ip2, int **ip3, Fac3 *f, int esn) {
//=======================================================================
// get pointers into Fac3
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


//================================================================
  int UFA_dump_fnb (Fac3 *fa, Fac3 *fnb, int fNr) {
//================================================================
// dump faces and neighbours

  int    i1;


  for(i1=0; i1<fNr; ++i1)
    printf(" fa[%d] = (%d %d %d) fnb = (%d %d %d)\n",i1,
           fa[i1].i1,fa[i1].i2,fa[i1].i3,
           fnb[i1].i1,fnb[i1].i2,fnb[i1].i3);

  return 0;

}

 
//================================================================
  int UFA_dump1 (Fac3 *fac, Fac3 *fnb, char *txt, ...) {
//================================================================
// see MSH_dump_fTab

  va_list va;
  char    cbuf[80];


  // printf("UFA_dump1\n");

  va_start(va, txt);
  vprintf(txt, va);
  va_end(va);

  printf(" fa=%d,%d,%d    fnb=%d,%d,%d\n",
          fac->i1,fac->i2,fac->i3,
          fnb->i1,fnb->i2,fnb->i3);

  // printf("ex UFA_dump1\n");



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


//================================================================
  int UFA_ipOpp_facedg (int *ipOpp, int *esn, Fac3 *fac1, int ie) {
//================================================================
// UFA_ipOpp_facedg   get point opposite to edge (ip3 & ptNr from ip1)
// get nr of point opposite to edge with startingPoint ie
// and get EdgeSequenceNr of ie.

// ie is the starting point of a edge of fac1;
// the opposite point is the point befor ie.


  if(fac1->i1 == ie) {
    *esn = 1;
    *ipOpp = fac1->i3;

  } else if(fac1->i2 == ie) {
    *esn = 2;
    *ipOpp = fac1->i1;

  } else if(fac1->i3 == ie) {
    *esn = 3;
    *ipOpp = fac1->i2;
  }

  // printf("ex UFA_ipOpp_facedg opp=%d e=%d sn=%d (%d,%d,%d)\n",
          // *ipOpp, ie, *esn,
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
  int UFA_edg_find_facn (Fac3 *fa, int fNr, int ie1, int ie2) {
//=======================================================================
// find neighbourface for edge ie1-ie2.
// direction of neighbouredge is opposite !
// Output:
//   retCod  0-fNr   nr of face with edge ie1-ie2
//           -1      Face not found

// see also UFA_ifac_ck_edgu

  int    i1;

  // printf("UFA_edg_find_facn %d %d\n",ie1,ie2);

  for(i1=0; i1<fNr; ++i1) {
    // test i1-i2
    if(fa[i1].i1 == ie2) {
      if(fa[i1].i2 == ie1) return i1;

    // test i2-i3
    } else if(fa[i1].i2 == ie2) {
      if(fa[i1].i3 == ie1) return i1;

    // test i3-i1
    } else if(fa[i1].i3 == ie2) {
      if(fa[i1].i1 == ie1) return i1;
    }
  }

  return -1;

}


//=============================================================================
  int UFA_2fac_facpt (MemTab(Fac3) *fTab, int ipt, int ifc, int ie) {
//=============================================================================
// create 2 new Faces tru point ipt instead of face ifc
// ipt lies on Edge ie
//
//    i1------ipt-----i3         displayed for case ie=3 !!!
//      \      |     /
//         \   |  /
//            i2
//
//  i1-ipt-i3   <<< statt i1-i2-i3    <<<  ie = 1
//  ipt-i2-i3
//
//  i1-i2-ipt   <<< statt i1-i2-i3    <<<  ie = 2
//  i1-ipt-i3
//
//  i1-i2-ipt   <<< statt i1-i2-i3    <<<  ie = 3
//  i2-i3-ipt
//


  int   i1, i2, i3;
  Fac3  *f1;

  // printf("UFA_2fac_facpt %d %d %d\n",ipt,ifc,ie);


  f1 = &fTab->data[ifc];
  i1 = f1->i1;
  i2 = f1->i2;
  i3 = f1->i3;

  if(ie == 1) {
    UFA_chg_fac (fTab->data, ifc, i1, ipt, i3);
    UFA_add_fac (fTab, ipt, i2, i3);

  } else if(ie == 2) {
    UFA_chg_fac (fTab->data, ifc, i1, i2, ipt);
    UFA_add_fac (fTab, i1, ipt, i3);

  } else if(ie == 3) {
    UFA_chg_fac (fTab->data, ifc, i1, i2, ipt);
    UFA_add_fac (fTab, i2, i3, ipt);
  }

  return 0;

}


//================================================================
  int UFA_3fac_facpt (MemTab(Fac3) *fTab, int ipt, int ifc) {
//================================================================
// create 3 new Faces tru point ipt instead of face ifc
//
//            i3
//          /   \
//         /   |  \
//        /   ipt   \
//       /   /   \    \
//      / /          \  \
//    i1 --------------- i2
//
//  i1-i2-ipt   <<< statt i1-i2-i3
//  i2-i3-ipt
//  i3-i1-ipt


  Fac3  *f1;
  int   i1, i2, i3;

  f1 = &fTab->data[ifc];
  i1 = f1->i1;
  i2 = f1->i2;
  i3 = f1->i3;

  UFA_chg_fac (fTab->data, ifc, i1, i2, ipt);

  UFA_add_fac (fTab, i2, i3, ipt);

  UFA_add_fac (fTab, i3, i1, ipt);


  return 0;

}


//================================================================
  int UFA_chg1_fac (Fac3 *fac1, int ival, int psn) {
//================================================================
// UFA_chg1_fac       set 1 pointer in face (seqnr)


  if(psn == 1) {
    fac1->i1 = ival;

  } else if(psn == 2) {
    fac1->i2 = ival;

  } else if(psn == 3) {
    fac1->i3 = ival;

  }


  return 0;

}


//=============================================================
  int UFA_chg_fac (Fac3 *fa, int fNr, int i1, int i2, int i3) {
//=============================================================

  fa[fNr].i1 = i1;
  fa[fNr].i2 = i2;
  fa[fNr].i3 = i3;

  return 0;

}



//==============================================================
  int UFA_add_fac (MemTab(Fac3) *fTab, int i1, int i2, int i3) {
//==============================================================
// UFA_add_fac        add face to MemTab(Fac3)

  int       irc;
  long      l1;
  Fac3      f1;

  f1.i1 = i1;
  f1.i2 = i2;
  f1.i3 = i3;

  return MemTab_sav ((MemTab*)fTab, &l1, &f1, 1);

}


//================================================================
  int UFA_2ip_fac_esn (int *i1, int *i2, int esn, Fac3 *fc) {
//================================================================
// get the pointNr's from Face and its EdgeSeqNr (1|2|3)
// Input:
//   esn    EdgeNr 1 (i1-i2) or 2 (i2-i3) or 3 (i3-i1)


  if(esn < 0) esn = -esn;

  // printf("UFA_2ip_fac_esn %d\n",esn);


  // irc=1: Edge i1-i2;
  if(esn == 1) {
    *i1 = fc->i1;
    *i2 = fc->i2;

  // irc=2: Edge i2-i3;
  } else if(esn == 2) {
    *i1 = fc->i2;
    *i2 = fc->i3;

  // irc=3: Edge i3-i1;
  } else if(esn == 3) {
    *i1 = fc->i3;
    *i2 = fc->i1;
  }

  // printf("ex UFA_2ip_fac_esn %d %d\n",*i1,*i2);

  return 0;

}


//================================================================
  int UFA_ifac_ck_edgu (int *if1, int *ie1, int *if2, int *ie2,
                        int ip1, int ip2, Fac3 *fa, int fNr) {
//================================================================
// find the faces (1 or 2) with the unoriented Edge ip1-ip2
// Output:
//   if1, if2    FaceNr's (-1 = no Face)
//   ie1         EdgeNr on Face if1; 1|2|3

// see also UFA_ifac_ck_edgo UFA_edg_find_facn

  int   i1, ie;


  // printf("UFA_ifac_ck_edgu %d %d %d\n",ip1,ip2,fNr);

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

  // printf("ex UFA_ifac_ck_edgu f: %d %d e: %d %d\n",*if1,*if2,*ie1,*ie2);

  return 0;

}


//================================================================
  int UFA_ifac_ck_edgo (int *if1, int *ie1,
                        int ip1, int ip2, Fac3 *fa, int fNr) {
//================================================================
// find the face with the oriented Edge ip1-ip2
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
//   iNr    size for iTab,psna
// Output:
//   iTab   table of faces using point ipt
//   psna   1-if ipt=p1; 2-if ipt=p2; 3-if ipt=p3;
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
  int UFA_ar_fac (double *ar, Fac3 *fc, Point2 *pa) {
//================================================================
// UFA_ar_fac            get (signed) area of face

  // printf("UFA_ar_fac %d %d %d\n",fc->i1, fc->i2,fc->i3);

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
// UFA_int_fac_ln2    intersect face - 2D-line
// Line goes tru Edge iei into Face fac; check intersection.
// Input:
//   iei       edgeNr where line ptl,vcl comes into fac
//   ptl,vcl   Line
//   fac       1 face
//   pa        ptArr
// Output:
//   if RetCod=0         normal intersection Line-Edge
//     ieo =   1|2|3     the EdgeSeqNr of the Edge intersected by Line
//   if RetCod=1         Line goes tru point opposit to edge iei;
//     ieo =   1|2|3     the PointSeqNr of the Point opposit to Edge iei


  int      irc, i1, i2, s1;
  double   d1;
  Point2   ptOpp;
  Vector2  vp;


  // printf("UFA_int_fac_ln2 ei=%d f=%d-%d-%d \n",iei,fac->i1,fac->i2,fac->i3);


  // get the point opposit to Edge iei into ptOpp
  UFA_ipOpp_fac_esn (&i1, &i2, fac, iei);

  // test if point pa[i1] is left or right of Line
  UT2D_vc_2pt (&vp, ptl, &pa[i1]);
  d1 = UT2D_sar_2vc (vcl, &vp);
  s1 = DSIGTOL (d1, UT_TOL_min0);      // pos: ptOpp is left of line
    // printf(" s1=%d d1=%f\n",s1,d1);

  if(s1 == 0) {
    irc = 1;
    *ieo = i2;  // the PointSeqNr of ptOpp

  } else {
    irc = 0;
    if(s1 > 0) {      // ptOpp is left of line
      *ieo = UFA_esn_nxt (iei);    // next edge

    } else {          // ptOpp is right of line
      *ieo = UFA_esn_prv (iei);    // previous edge
    }
  }

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


//================================================================
  int UFA_esn_2ip (int ip1,  Fac3 *fa, int iFac) {
//================================================================
// get esn from startpoint of edge

  int      esn1=0;

  // printf(" fa[%d] %d %d %d\n",iFac,fa[iFac].i1,fa[iFac].i2,fa[iFac].i3);


  if(ip1 == fa[iFac].i1)       { esn1 = 1; }
  else if(ip1 == fa[iFac].i2)  { esn1 = 2; }
  else if(ip1 == fa[iFac].i3)  { esn1 = 3; }

  // printf("ex UFA_esn_2ip %d ip1=%d iFac=%d\n",esn1,ip1,iFac);
  return esn1;

}

//============================================================================
  int UFA_fnb_init_el (MemTab(EdgeLine) *eTab, Fac3 *fnb, Fac3 *fa, int fNr) {
//============================================================================
// UFA_fnb_init_el         set NeighbourFacNr -2, if Edge is a BreakLine
// Bei den NachbarFacNrs FaceNr -2, wenn Edge eine BreakLine ist.

  int      i1, i2, *ia, iNr, ip1, ip2, iFac, esn1, inbFac, esn2;


  // printf("UFA_fnb_init_el \n");


  for(i1=0; i1<eTab->rNr; ++i1) {
    if(eTab->data[i1].typ != TYP_EDGLN_BL) continue;

    ia  = eTab->data[i1].ia;
    iNr = eTab->data[i1].iNr - 1;  // letztes - 1 !

    // loop tru edges of a IB
    for(i2=0; i2<iNr; ++i2) {
      ip1 = ia[i2];
      ip2 = ia[i2 + 1];
        // printf(" EL[%d] %d %d\n",i1,ip1,ip2);
      // find ifac und esn of oriented edge ip1-ip2
      UFA_ifac_ck_edgo (&iFac, &esn1, ip1, ip2, fa, fNr);
      if(iFac < 0) {
        // no neighbour exists: must be eg inner boundary
        // TX_Print("UFA_fnb_init_el E001 BL %d %d",ip1,ip2);
        continue;
      }
      // get neigbourfaceNr
      inbFac = UFA_inbFac_esn (iFac, esn1, fnb);
      if(inbFac < 0) continue;  // // no neighbour ..
      // get esn of edge starting with ip2 in inbFac
      esn2 = UFA_esn_2ip (ip2, fa, inbFac);
        // printf(" iFac=%d esn=%d inbFac=%d esn=%d\n",iFac,esn1,inbFac,esn2);

      // mark edges -2
      UFA_chg1_fac (&fnb[iFac], -2, esn1);
      UFA_chg1_fac (&fnb[inbFac], -2, esn2);
    }
  }

  // UFA_dump_fnb (fa, fnb, fNr);

  return 0;

}


//================  EOF  ==================================
