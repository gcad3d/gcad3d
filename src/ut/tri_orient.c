// Triangs orientieren     2007-03-10   RF
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
\file  ../ut/tri_orient.c
\brief orient triangles 
\code
=====================================================
List_functions_start:

UT3D_tria_orient            alle 3Ecke orientieren (Normalvektoren ausrichten)
UT3D_tria_ori_comm          check for Kanten direkt verbinden
UT3D_tria_ori_parl          naechste Kante mit Spalt suchen
UT3D_tria_ori_par_ck        check ob 2 Linien einen gemeinsamen Bereich haben

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



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



2007-03-09 ReImplementiert aus UT3D_tria_orient
*/




#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
// nur wenn via DLL used  (nicht wenn im Kernel; auskommentieren !)
#ifdef _MSC_VER
// nachfolgende externals werden aus dem Main-Exe imported:
#define extern __declspec(dllimport)
#endif
*/


#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_itmsh.h"            // MSHIG_EDGLN_.. typedef_MemTab.. Fac3
#include "../ut/func_types.h"               // SYM_STAR_S
#include "../xa/xa_mem.h"              // memspc51

#include "../gr/ut_gr.h"               // GR_tDyn_pcv







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


  // TRI_Disp_tStat (tStat, triNr);
  // TRI_Disp_Bound (ta, tStat, triNr);

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
  // TRI_Disp_Bound (ta, tStat, triNr);





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
 

//================ EOF ===========================================
