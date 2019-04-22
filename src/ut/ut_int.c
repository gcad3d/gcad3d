//      Intersect-Functions.       RF   2005-08-19
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
\file  ../ut/ut_int.c
\brief intersect
\code
=====================================================
List_functions_start:

INT2_pta_lna                  connect LineSegments  --> Polygon

INT_intplsur                  Intersect Surface - Plane

INT_intpltor                  intersect Plane Torus
INT_intplcon                  intersect Plane Cone

INT_intsursur                 Intersect Surface - Surface
INT_int_tria_tria             intersect Surface - Surface --> n LineSegments
INT_int_tria_pln              intersect Plane - nTriangles --> nLineSegments
INT_ln2pta__                  connect LineSegments  --> Polygon
INT_pta2crv__                 change Polygon to Line Circ  Elli or BSpl-Curve

List_functions_end:
=====================================================
- see also:
UTO_obj_int
UTO_obj_trim

\endcode *//*----------------------------------------


*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // for ...


#include "../ut/ut_geo.h"              // Point ...

#include "../ut/func_types.h"          // SYM_..
#include "../gr/tess_su.h"             // TypTsuSur
#include "../xa/xa_mem.h"              // memspc51, mem_cbuf1





//================================================================
  int INT2_pta_lna (void *oTab, int *oNr, Memspc *wrkSpc) {
//================================================================
/// \code
/// aus den 2D-Linien ein 2D-Polygon machen
/// das Point2-Polygon wird in oTab zurueckgeliefert, Anzahl oNr wird korrigiert
/// \endcode

// TestFunc test_INT_ln2plg__
// see also INT_ln2pta__ NCT_pta_lna
 
  int         irc, i1, i2, ii, lNr, pNr, *iTab, ips, ipe, tabSiz;
  double      d1, d2;
  char        *cTab;
  Point2      *pTab, *ps, *pe, *pto;
  Line2       *lTab;


  lTab = (Line2*)oTab;
  lNr = *oNr;


  // printf("INT2_pta_lna %d %f %f %f\n",*oNr,UT_TOL_pt,UT_TOL_cv,UT_DISP_cv);
  // for(i1=0; i1<*oNr; ++i1) {
    // // GR_Disp_ln2 (&lTab[i1].p1, &lTab[i1].p2, 9);
    // DEB_dump_obj__ (Typ_LN2, &lTab[i1], "L[%d]=",i1);
  // }


  if(*oNr < 1) return -1;

  // get Space for Flags done / notDone
  // need lNr bytes for cTab
  cTab = wrkSpc->next;
  irc = UME_add (wrkSpc, lNr);
  if(irc < 0) goto L_eom;


  // init output-PointTable
  tabSiz = lNr + 20;
  pTab = wrkSpc->next;
  irc = UME_add (wrkSpc, tabSiz * sizeof(Point));
  if(irc < 0) goto L_eom;


  // init iTab; Pointer to pTab
  iTab = wrkSpc->next;
  irc = UME_add (wrkSpc, tabSiz * sizeof(int));
  if(irc < 0) goto L_eom;


  // init iTab1; 0=unused, 1=already done.
  for(i1=0; i1<lNr; ++i1) cTab[i1] = 0;

  // die erste Linie ganz einfach einspeichern ...
  pTab[0] = lTab[0].p1;
  pTab[1] = lTab[0].p2;
  pNr = 2;
  cTab[0] = 1;
  iTab[0] = 0;
  iTab[1] = 1;
  ips = 0;
  ipe = 1;
  ps = &pTab[0];
  pe = &pTab[1];

   // printf(" init-pt[0]=%f,%f\n",pTab[0].x,pTab[0].y);
   // printf(" init-pt[1]=%f,%f\n",pTab[1].x,pTab[1].y);


  // find next startObj (first ln not yet done)
  L_nxt_obj:
  if(pNr >= tabSiz) goto L_eom;
    // printf("L_nxt_obj:\n");
  for(i1=1; i1<lNr; ++i1) {
    if(cTab[i1] > 0) continue;
      // printf(" nxt i1=%d ips=%d ipe=%d pNr=%d\n",i1,ips,ipe,pNr);

    // das segment i1 mit den aktuellen Anschlusspunkten vergleichen ..
    for(i2=1; i2<lNr; ++i2) {
      if(cTab[i2] > 0) continue;
/*
      // skip Lines kuerzer als UT_DISP_cv
      if(UT2D_comp2pt(&lTab[i2].p1,&lTab[i2].p2,UT_DISP_cv) == 1) {
        cTab[i2] = 1;
        printf(" skip short line %d\n",i2);
        continue;
      }
*/
      // UT_DISP_cv od UT_TOL_cv
      irc = UT2D_comp4pt (ps,pe, &lTab[i2].p1,&lTab[i2].p2, UT_TOL_cv);
        // printf(" irc=%d i1=%d i2=%d\n",irc,i1,i2);
      // 0=nix,1=1e-2a,2=1a-2a,3=1e-2e,4=1a-2e
      // rc=0: keine Verbindung
      // rc=1: bei El. liegen richtig.
      // rc=2: Obj 1 verkehrt
      // rc=3: Obj 2 verkehrt
      // rc=4: Obj 1 und Obj 2 verkehrt
      if(irc < 1) {     // 0=nix
        // kein Anschlussobj gefunden, weiter ..
        continue;
      } else if(irc == 1) {    // 1=pe-ls
        // add le after pe;  ps ... pe le
        pTab[pNr] = lTab[i2].p2;     // save LineEndpunkt
        pe = &pTab[pNr];             // = der neue Endpunkt
        ++ipe;                       // der neue EndpunktIndex
        iTab[pNr] = ipe;
        ++pNr;

      } else if(irc == 2) {    // 2=ps-ls
        // add le before ps;  le - ps ... pe
        pTab[pNr] = lTab[i2].p2;     // save EndptLine
        ps = &pTab[pNr];             // = der neue Startpunkt
        --ips;                       // der neue StartpunktIndex
        iTab[pNr] = ips;
        ++pNr;

      } else if(irc == 3) {    // 3=pe-le
        // add ls after pe;  ps ... pe ls
        pTab[pNr] = lTab[i2].p1;     // save LineStartpt
        pe = &pTab[pNr];             // = der neue Endpunkt
        ++ipe;                       // der neue EndpunktIndex
        iTab[pNr] = ipe;
        ++pNr;

      } else if(irc == 4) {    // 4=ps-le
        // add ls before ps;  ls - ps ... pe
        pTab[pNr] = lTab[i2].p1;     // save LineStartpt
        ps = &pTab[pNr];             // = der neue Startpunkt
        --ips;                       // der neue StartpunktIndex
        iTab[pNr] = ips;
        ++pNr;
        
      }
      // Anschlussobj gespeichert;
      cTab[i2] = 1;
        // printf(" found %d ips=%d ipe=%d\n",i2,ips,ipe);
        // printf(" add pt[%d]=%f,%f\n",pNr-1,pTab[pNr-1].x,pTab[pNr-1].y);
      goto L_nxt_obj;  // ganz raus; naechstes Anschlussel.
    }
    break;
  }


  // test ob alle Punkte gefunden
  for(i1=1; i1<lNr; ++i1) {
    if(cTab[i1] < 1) goto L_fix_gap;
  }
  goto L_done;



  //----------------------------------------------------------------
  L_fix_gap:
  // den kleinsten Spalt suchen und eine Hilfslinie erzeugen
  d2 = UT_VAL_MAX;
  ii = -1;
  for(i1=1; i1<lNr; ++i1) {
    if(cTab[i1] > 0) continue;
    i2 = UT2D_minLenB_4pt (&d1, ps, pe, &lTab[i1].p1,&lTab[i1].p2);
      // printf(" minLen_4pt irc=%d d=%f\n",i2,d1);
    if(d1 < d2) {
      ii = i1;
      irc = i2;
      d2 = d1;
    }
  }
  // create Vebindungslinie
    // printf(" fix ii=%d irc=%d\n",ii,irc);
  if(irc < 1) {     // 0=nix

  } else if(irc == 1) {    // 1=pe-lp1
    // add ls after pe;  ps ... pe le
    pTab[pNr] = lTab[ii].p1;     // save LineEndpunkt
    pe = &pTab[pNr];             // = der neue Endpunkt
    ++ipe;                       // der neue EndpunktIndex
    iTab[pNr] = ipe;
    ++pNr;


  } else if(irc == 2) {    // 2=ps-lp1
    // add ls before ps;  le - ps ... pe
    pTab[pNr] = lTab[ii].p1;     // save EndptLine
    ps = &pTab[pNr];             // = der neue Startpunkt
    --ips;                       // der neue StartpunktIndex
    iTab[pNr] = ips;
    ++pNr;


  } else if(irc == 3) {    // 3=pe-lp2
    // add le after pe;  ps ... pe ls
    pTab[pNr] = lTab[ii].p2;     // save LineStartpt
    pe = &pTab[pNr];             // = der neue Endpunkt
    ++ipe;                       // der neue EndpunktIndex
    iTab[pNr] = ipe;
    ++pNr;


  } else if(irc == 4) {    // 4=ps-lp2
    // add le before ps;  ls - ps ... pe
    pTab[pNr] = lTab[ii].p2;     // save EndptLine
    ps = &pTab[pNr];             // = der neue Startpunkt
    --ips;                       // der neue StartpunktIndex
    iTab[pNr] = ips;
    ++pNr;

  }
    // printf(" ins pt[%d]=%f,%f\n",pNr-1,pTab[pNr-1].x,pTab[pNr-1].y);
  goto L_nxt_obj;  // no amoi ..




  //----------------------------------------------------------------
  L_done:

      // TEST-ONLY-BLOCK:
      // printf("-------- after sort pNr=%d ips=%d ipe=%d\n",pNr,ips,ipe);
      // for(i1=0; i1<pNr; ++i1) printf(" iTab[%d]=%d\n",i1,iTab[i1]);
      // ii = ips;
      // for(i1=0; i1<pNr; ++i1) {
        // for(i2=0; i2<pNr; ++i2) {
          // if(iTab[i2] == ii) {
            // DEB_dump_obj__ (Typ_PT2, &pTab[i2], "%d P[%d]=",i1,i2);
            // ++ii;
            // break;
          // }
        // }
      // }
  


    // printf(" kopieren ...\n");

    // Punkte nach oTab kopieren ...
    // printf(" _pta_lna in %d lines, out %d points\n",*oNr,pNr);
    pto = oTab;

    ii = ips;  // das ist der Punkt mit dem niedrigsten Index; ii ist neg !
    for(i1=0; i1<pNr; ++i1) {
        // printf(" iTab[%d]=%d\n",i1,iTab[i1]);
      // ii in iTab suchen; iTab-Index = ptab-Index; diesen Punkt raus, ..
      for(i2=0; i2<pNr; ++i2) {
         if(iTab[i2] == ii) {
           // DEB_dump_obj__ (Typ_PT2, &pTab[i2], "%d P[%d]=",i1,i2);
           pto[i1] =  pTab[i2];
           ++ii;
           break;
         }
      }
    }

/*
    // der letzte Punkt liegt nun in oTab[pNr]; nach oTab[pNr+1] kopieren
    // den Mittelpunkt nach oTab[pNr].
    pto[pNr+1] = pto[pNr];
    UT2D_pt_mid2pt (&pto[pNr], &pto[pNr-1], &pto[pNr+1]);

    // der erste Punkt liegt nun in oTab[1]; nach oTab)[0] kopieren
    // den Mittelpunkt nach oTab[1].
    pto[0] = pto[1];
    UT2D_pt_mid2pt (&pto[1], &pto[0], &pto[2]);
      
    pNr += 2;
*/

    *oNr = pNr;

    // printf("ex INT2_pta_lna:%d \n",pNr);
    // pTab = (Point2*)oTab;
    // for(i1=0; i1<pNr; ++i1)
      // DEB_dump_obj__ (Typ_PT2, &pTab[i1], "P[%d]=",i1);
    // GR_Disp_cv2 (pTab, pNr, 9);

  return 0;


  //----------------------------------------------------------------
  L_eom:
    TX_Error("INT2_pta_lna EOM");
    return -1;

}


//===========================================================================
  int INT_intplsur (int *oTyp, void **oDat,
                    Plane *pl1, long *sTab, int *sTyp, int sNr,
                    int imod, int outTyp) {
//===========================================================================
/// \code
/// Intersect Surface - Plane
/// xSpc  is char[OBJ_SIZ_MAX]; Typ_LN: struct Line;  else struct ObjGX
/// sTab *long-Liste von APT-Surfs, Anzahl sNr
/// Input:
///   sTyp    list of Typ_SUR|Typ_SOL
///   sTab    list of DBi's
///   imod    solutionNr; 0=nur_erstes, 1=nur_zweites usw
///   outTyp  solltyp out: Typ_CVBSP (10) | Typ_CVPOL (8)
/// Output:
///   RetCod
///     >0      max nr of solutions 
///     -1      Err
/// \endcode

// ACHTUNG das Tess. benutzt fast alle Speicher; loescht aus_typ usw !


  int       irc, i1, i2, triSiz, surSiz, triNr, surNr, lnSiz, lnNr, iMaxNr;
  ObjGX     *spc1=NULL, *oTab, *oxp, oo1;
  Point     *pTab;
  Line      *lnTab;
  Triangle  *triTab;
  TypTsuSur *surTab;
  Memspc    tSpc, oSpc, pSpc;

 
  // printf("INT_intplsur %d sNr=%d imod=%d\n",*oTyp,sNr,imod);
  // for(i1=0; i1<sNr; ++i1) printf("  A[%d] = %d\n",i1,sTab[i1]);




  //----------------------------------------------------------------
  // tesselate Surf # i1 --> spc1
  // ACHTUNG: benutzt fast alle Speicher !
  irc = TSU_tess_sTab (&spc1, sTyp, sTab, sNr);
  if(irc < 0) return -1;



  //----------------------------------------------------------------
  // intersect all Triangles in spc1 mit Plane pl1

  // space for triangles --> triTab  (12bytes/Tria)
  triTab = (Triangle*)memspc501;
  triSiz = sizeof(memspc501) / sizeof(Triangle);
    // printf(" triSiz=%d\n",triSiz);


  surTab = (TypTsuSur*)memspc51;
  surSiz = sizeof(memspc51) / sizeof(TypTsuSur);
    // printf(" surSiz=%d\n",surSiz);



  // get triangles from spc1 --> triTab
  // (triangles are pointers into tesselated data !)
  surNr = surSiz;
  triNr = triSiz;
  irc = TSU_ntria_bMsh__ (surTab, &surNr, surSiz, triTab, &triNr, triSiz, spc1);
/*
    //----TEST:----------
    printf("ex TSU_ntria_bMsh__ irc=%d surNr=%d\n",irc,surNr);
    for(i1=0; i1<surNr; ++i1) {
      printf(" sur[%d].ip1=%d ip2=%d typ=%d\n",i1,
             surTab[i1].ip1,surTab[i1].ip2,surTab[i1].typ);
      for(i2=surTab[i1].ip1; i2<=surTab[i1].ip2; ++i2) {
        printf(" tri %d:\n",i2);
        DEB_dump_obj__ (Typ_PT, triTab[i2].pa[0], "  p1=");
        DEB_dump_obj__ (Typ_PT, triTab[i2].pa[1], "  p2=");
        DEB_dump_obj__ (Typ_PT, triTab[i2].pa[2], "  p3=");
      }
    }
    return 0;
*/



  // space for resulting linesegments --> lnTab
  lnTab = (Line*)memspc101;
  lnSiz = sizeof(memspc101) / sizeof(Line);
  lnNr  = 0;


  // intersect Plane - Triangles --> n LineSegments
  INT_int_tria_pln (lnTab, &lnNr, lnSiz, triTab, triNr, triSiz, pl1);
    // for(i1=0; i1<lnNr; ++i1) GR_Disp_ln (&lnTab[i1], 8);
    // return -1;


  // free tesselated data
  if(spc1) free (spc1);
  // if(spc2) free (spc2);


  if(lnNr < 1) return -1;
  // return 0;  // TEST


  // space for Objects (Polygons)
  UME_init (&oSpc, memspc52, sizeof(memspc52));

  // space for Points
  UME_init (&pSpc, memspc201, sizeof(memspc201));

  // get tempSpace
  UME_init (&tSpc, memspc50, sizeof(memspc50));


  // connect LineSegments  --> Group of Polygon
  // Output: typ=Typ_ObjGX, form=Typ_ObjGX; dahinter n pointTables (PT-PT)
  INT_ln2pta__ (&oSpc, &pSpc, &tSpc, lnTab, lnNr);
/*
    //----TEST:----------
    oxp = (ObjGX*)oSpc.start;
    oTab = oxp->data;
    for(i1=0; i1<oxp->siz; ++i1) {
      pTab = oTab[i1].data;
      // GR_Disp_pTab (oTab[i1].siz, pTab, SYM_STAR_S, 2);
      GR_Disp_cv (pTab, oTab[i1].siz, 9);
      // for(i2=0; i2<oTab[i1].siz; ++i2) {
        // DEB_dump_obj__(Typ_PT, &pTab[i2],"seg%d P%d ",i1,i2);
      // }
    }
    return -1;
*/
    //--------------------------------------
    // DEB_dump_ox_s_ (oSpc.start, "Group of Polygons:");
    //--------------------------------------
    // return 0;





  // loop tru polygons; remove unnecessary(straight) points
  oxp = (ObjGX*)oSpc.start;
  oTab = oxp->data;
  for(i1=0; i1<oxp->siz; ++i1) {
    if(i1 != imod) continue;        // skip unused
    // printf("_intplsur 1.oTab[%d]; siz=%d\n",i1,oTab[i1].siz);
    if(oTab[i1].siz > 2) {
      i2 = oTab[i1].siz;
      UT3D_cv3_linear (&i2, oTab[i1].data, UT_TOL_pt);
      oTab[i1].siz = i2;
    }
    // printf("_intplsur 2.oTab[%d]; siz=%d\n",i1,oTab[i1].siz);
  }
    //----TEST:----------
    // oxp = (ObjGX*)oSpc.start;
    // oTab = oxp->data;
    // for(i1=0; i1<oxp->siz; ++i1) {
      // pTab = oTab[i1].data;
      // for(i2=0; i2<oTab[i1].siz; ++i2) {
        // DEB_dump_obj__(Typ_PT, &pTab[i2],"seg%d P%d ",i1,i2);
      // }
    // }
    // return 0;






  // change Polygon to Line or Circ or Ellipse or Polygon or BSpl-Curve
  // Output is typ=Group in (ObjGX*)oSpc->start !)
  iMaxNr = INT_pta2crv__ (&oo1, &oSpc, &pSpc, &tSpc, imod, outTyp);
  if(iMaxNr < 0) return irc;
  *oTyp = oo1.form;
  *oDat = oo1.data;

    // DEB_dump_obj__ (*oTyp, *oDat, "ex INT_intplsur\n");

    //----TEST:----------
    // printf(" oTyp=%d\n",oTyp);


/*
  //-------TESTAUSGABE----------
  oxp = (ObjGX*)oSpc.start;
  i2 = oxp->siz;
  for(i1=0; i1<i2; ++i1) {
    (char*)oxp += sizeof(ObjGX); // obj 1 from group
    printf(" OOOOOOOOOOOOOOOOOOOOOOO %d\n",i1);
    // DEB_dump_ox_s_ (oxp, "oxp");
    DEB_dump_ox_0 (oxp, "oxp %d",i1);
  }
  //-------TESTAUSGABE----------




  // Objekt raus -> oTyp, xSpc
  oxp = (ObjGX*)oSpc.start;   // ParentObj
  oTab = oxp->data;           // objTable
  oxp = &oTab[0];             // 1 obj
  *oTyp = oxp->typ;
  // den Datenrecord selbst kopieren
  // *((Line*)xSpc) = *((Line*)oxp->data);
  memcpy (xSpc, oxp->data, OBJ_SIZ_MAX);

  DEB_dump_ox_0 (xSpc, "xSpc %d",i1);
*/

/*
  //----------------------------------------------------------------

  // Wenn Modifier > 0 dann das entspechende Obj ausgeben
  // else eine Group generieren (noch nicht implementiert).
  imod = 1;  // HIER NUR TEST !!
  if(imod > 0 ) {
    // ..
  // } else {
    printf("**** Int1-GroupObject not jet implemented ***** \n");
  }



*/



  return iMaxNr;

}



//================================================================
  int INT_intplcon (int *oTyp, void *oSpc, Plane *pli, Conus *coi) {
//================================================================
/// \code
/// intersect Plane Cone; Output:
/// oTyp = Typ_LN (Plane parallel zu Konusachse am Mantel)
/// oTyp = Typ_CI (Plane normal auf Konusachse) ...
/// oTyp = Typ_CVELL (Plane nicht normal auf Konusachse) ...
/// 
/// irc  0   OK  (Circ od Elli out)
/// irc -1   Plane touches cone (Line out)
/// irc -2   Plane outside cone
/// \endcode


static  CurvElli          el1;

  int      i1;
  double   d1, d2, k;
  Vector   vc1, vc2;
  Point    pt1, pt2, pt3;


  // DEB_dump_obj__ (Typ_PLN, pli, "INT_intplcon:\n");
  // DEB_dump_obj__ (Typ_CON, coi, "");


  // Winkel ConeAchse - PlaneNormalachse
  d1 = UT3D_nlen_projvcvc (&pli->vz, &coi->pl.vz);
    // printf(" d1=%f\n",d1);


  //================================================================
  if(d1 < UT_TOL_pt) {
    // Typ_CI (Plane normal auf Konusachse)

    // den Abst. entlang der Z-Achse von pli --> coi-->pl
    d1 = UT3D_slen_ptpl (&pli->po, &coi->pl);
      // printf(" d1=%f\n",d1);

    // ausserhalb 90-h ?
    if(d1 < -UT_TOL_pt)  return -2;
    if(d1 >  (coi->h + UT_TOL_pt))  return -2;

    // CirCen liegt auf ConeAchse in Hoehe d1
    UT3D_vc_multvc (&vc1, &coi->pl.vz, d1);   // vz hat Laenge 1 !
    UT3D_pt_traptvc (&pt1, &coi->pl.po, &vc1);
      // GR_Disp_pt (&pt1, SYM_STAR_S, 2);
    
    // Konusradius auf Hoehe d1 bestimmen ...
    k = (coi->r2 - coi->r1) / coi->h;  // Steigung
    d2 = k * d1 + coi->r1;             // y = k * x + d
      // printf(" d2=%f\n",d2);

    // make 360-Grad Circ from pc, vz, vx and radius
    UT3D_ci_pt2vcr ((Circ*)oSpc, &pt1, &coi->pl.vz, &coi->pl.vx, d2);


    *oTyp = Typ_CI;



  //================================================================
  // Ellipse; derzeit nur fuer cylinder (wenn r1 == r2).
  } else if(fabs(coi->r2-coi->r1) < UT_TOL_pt) {

    // pt1 = intersect. Conusachse mit Plane pli = Center Ellipse.
    i1 = UT3D_pt_intptvcpln (&pt1, &coi->pl.po,&coi->pl.vz, &pli->po,&pli->vz);
      // GR_Disp_pt (&pt1, SYM_STAR_S, 2);

    // pt2 = den Origin der pli auf die ConeAchse projiz.
    // Strecke pli.po / pt2 entspricht ConeRadius (Elli-p1 ..)
    UT3D_pt_projptptvc (&pt2,&d1, NULL, &pli->po, &coi->pl.po,&coi->pl.vz);
      // GR_Disp_pt (&pt2, SYM_STAR_S, 3);

    d2 = coi->r1 / UT3D_len_2pt (&pli->po, &pt2);  // Faktor
      // printf(" d2=%f\n",d2);


    UT3D_vc_2pt (&vc1, &pt1, &pli->po);
    UT3D_vc_multvc (&vc1, &vc1, d2);
    UT3D_pt_traptvc (&pt3, &pt1, &vc1);
      // GR_Disp_pt (&pt3, SYM_STAR_S, 4);

    // minor axis:
    UT3D_vc_perp2vc (&vc2, &pli->vz, &vc1);
    UT3D_vc_setLength (&vc2, &vc2, coi->r1);

    // Elli from 
    UT3D_el_pt2vc2a ((CurvElli*)oSpc, &pt1, &vc1, &vc2, 0., RAD_360, 0);
      // GR_Disp_ell ((CurvElli*)oSpc, 9);

    *oTyp = Typ_CVELL;


  //================================================================
  } else {
    TX_Error("INT_intplcon not yet implemented");
    return -3;
  }


  return 0;

}


//================================================================
  int INT_intpltor (int *oTyp, void *oSpc, Plane *pln, Torus *tor) {
//================================================================
/// \code
/// intersect Plane Torus; Output:
/// oTyp = Typ_CI (Plane normal od parallel zu Torusachse) ...
/// 
/// irc  0   OK  (Circ od Elli out)
/// irc -1   Plane touches Torus
/// irc -2   Plane outside Torus
/// \endcode


  double   d1, dx;
  Point    pt1;
  Vector   vc1;


  // DEB_dump_obj__ (Typ_PLN, pln, "INT_intpltor:\n");
  // DEB_dump_obj__ (Typ_TOR, tor, "");



  //================================================================
  // Ist Torusachse normal zur Plane-VZ ?
  // d1 = UT3D_acos_2vc (&pln->vz, &tor->pl.vz);
  d1 = UT3D_slen_projvcvc (&pln->vz, &tor->pl.vz);
    // printf(" d1=%f\n",d1);
  if(fabs(d1) < UT_TOL_pt) {

    // die Plane muss noch durch die Torusachse gehen;
    // den Abst. des Toruscenter von der Plane messen
    d1 = UT3D_slen_ptpl (&tor->pl.po, pln);
      // printf(" d1=%f\n",d1);
    if((fabs(d1)-UT_TOL_pt) > tor->r1) return -2;
    if(fabs(d1) > UT_TOL_pt) goto L_int_std;


    // CirCen: vom TorCen Len=tor.r1-tor.r2 Richtg=
    UT3D_vc_perp2vc (&vc1, &tor->pl.vz, &pln->vz);
      // GR_Disp_vc (&vc1, &tor->pl.po, 9, 0);
    // die 2. Loesung ist mit vc1-Invers
    d1 = tor->r1 - tor->r2;  // r1 = outermost !
    UT3D_pt_traptvclen (&pt1, &tor->pl.po, &vc1, d1);
      // GR_Disp_pt (&pt1, SYM_STAR_S, 2);

    // Circ from center, axis, x-vec and radius
    UT3D_ci_pt2vcr ((Circ*)oSpc, &pt1, &pln->vz, &pln->vx, tor->r2);

    *oTyp = Typ_CI;
    return 0;
  }


  //================================================================
  // Ist Torusebene parallel zur Plane (Torusachse parallel zur Plane-VZ) ?
  d1 = UT3D_nlen_projvcvc (&pln->vz, &tor->pl.vz);
    // printf(" d1=%f\n",d1);
  if(d1 < UT_TOL_pt) {

    // CirCen: intersect Plane - TorAxis. (prj. Pl.po --> Tor.vz)
    UT3D_pt_projptptvc (&pt1, &d1, NULL, &pln->po, &tor->pl.po,&tor->pl.vz);
      // GR_Disp_pt (&pt1, SYM_STAR_S, 2);


    // den Abst. CirCen - TorCen
    d1 = UT3D_len_2pt (&pt1, &tor->pl.po);
      // printf(" d1=%f\n",d1);
    if((d1-UT_TOL_pt) > tor->r2) return -2;

    // mittl.Radius = (Tor.r1 - Tor.r2)
    // R = mittl.Radius + oder - dx
    UT2D_solvtriri_a (&dx, d1, tor->r2);
      // printf(" dx=%f\n",dx);
    d1 = tor->r1 - tor->r2 + dx;
      // printf(" d1=%f\n",d1);

    // Circ from center, axis, x-vec and radius
    UT3D_ci_pt2vcr ((Circ*)oSpc, &pt1, &tor->pl.vz, &tor->pl.vx, d1);
      // GR_Disp_ac ((Circ*)oSpc, 0);

    d1 = tor->r1 - tor->r2 - dx;
      // printf(" d1=%f\n",d1);

    // Circ from center, axis, x-vec and radius
    UT3D_ci_pt2vcr ((Circ*)oSpc, &pt1, &tor->pl.vz, &tor->pl.vx, d1);
      // GR_Disp_ac ((Circ*)oSpc, 9);



    *oTyp = Typ_CI;
    return 0;
  }




  //================================================================
  // keine anaytische Loesung; Dreiecke bilden und schneiden.
  L_int_std:
  // TX_Error("INT_intpltor not yet implemented");
  return -3;

  return 0;

}


//============================================================================
  int INT_int_tria_pln (Line *lnTab, int *lnNr, int lnSiz,
                        Triangle *triTab, int triNr, int triSiz, Plane *pln) {
//============================================================================
/// intersect Plane - nTriangles --> nLineSegments

  int   i1, irc;

  // printf("INT_int_tria_pln %d\n",triNr);


  for(i1=0; i1<triNr; ++i1) {
     // i1 = 2; // TEST statt for ..

    irc = UT3D_ln_intTriaPln (&lnTab[*lnNr], &triTab[i1], pln);
    if(irc < 0) continue;

    // add line
    if(*lnNr >= lnSiz) {
      TX_Error("INT_int_tria_pln EOM");
      return -1;
    }

    ++(*lnNr);
  }

  // printf("ex INT_int_tria_pln %d\n",*lnNr);

  return 0;

}


//====================================================================
  int INT_ln2pta__ (Memspc *oSpc, Memspc *pSpc, Memspc *tSpc,
                    Line *lnTab, int lnNr) {
//====================================================================
/// \code
/// connect LineSegments  --> Polygon
/// Bei l1e beginnen; Indextabelle iTab1 aufbauen; gefundene als done markieren.
/// Dann verkehrt; bei l1a beginnen; eine zweite Indextabelle aufbauen.
/// Dann beide Tabellen vereinigen und speichern;
/// (zuerst iTab2 verkehrt und dann iTab1 anhaengen).
/// dann das ganze (mit den noch nicht bearbeiteten Lines) von vorn.
/// 
/// OUT:
/// oSpc  die Objekdaten; an der Startadresse liegt ein Obj Typ_Group; seine
///       Members sind 1-n Polygone (ebenfalls als Objekte).
/// pSpc  die Punktdaten
/// tSpc  tempSpace
/// \endcode

// Typ_Group Typ_ObjGX Nr_of_ObjGX-Records(Curves) Adress_of_first_OX-Record
//   Typ_CVPOL Typ_PT Nr_of_Points pTab-Adress(1.Curve)..

// TestFunc test_INT_ln2plg__

  int     i1, ii, irc, ia, ie, ipa, ptNr, ipNr, pTabSiz, iFound, reDo,
          *iTab1, *iTab2, iTab1Nr, iTab2Nr;
  char    *cTab;
  Point   *pTab, *p1, *p2, *p3, *p4;
  ObjGX   *ox0, *ox1;


  // printf("INT_ln2pta__ %d\n",lnNr);

  if(lnNr < 1) return -1;

  // get Space for Flags done / notDone
  // need lnNr bytes for cTab
  cTab = tSpc->next;
  irc = UME_add (tSpc, lnNr);
  if(irc < 0) goto L_eot;


  // get Space for IndexTables
  // need lnNr ints for iTab1
  iTab1 = tSpc->next;
  irc = UME_add (tSpc, sizeof(int)*lnNr);
  if(irc < 0) goto L_eot;
  iTab2 = tSpc->next;
  irc = UME_add (tSpc, sizeof(int)*lnNr);
  if(irc < 0) goto L_eot;


  // init outObj (Group);
  ox0 = oSpc->next;
  irc = UME_add (oSpc, sizeof(ObjGX));
  if(irc < 0) goto L_eoo;
  ox0->typ  = Typ_ObjGX; // Typ_Group;
  ox0->form = Typ_ObjGX;
  ox0->siz  = 0;
  ox0->data = oSpc->next;


  // init output-PointTable
  pTab = pSpc->next;
  pTabSiz =  UME_ck_free(pSpc) / sizeof(Point);
  ptNr = 0;



  // init iTab1; 0=unsed, 1=already done.
  for(i1=0; i1<lnNr; ++i1) cTab[i1] = 0;




  // find next startObj (first ln not yet done)
  L_start:
  for(i1=0; i1<lnNr; ++i1) {
    if(cTab[i1] == 0) {
      ia = i1;
      ie = i1;
      break;
    } 
  } 
  p1 = &lnTab[ia].p1;
  p2 = &lnTab[ie].p2;

  // we start with the first obj.
  cTab[ia]  = 1;
  iTab1[0]  = ia;  // first "found"
  iTab1Nr   = 1;
  iTab2Nr   = 0;
  ipa = ptNr;


  //----------------------------------------------------------------
  L_nxt:
    iFound = 0;
    reDo = 0;
      // DEB_dump_obj__ (Typ_PT, p1, "nxt ia ");
      // DEB_dump_obj__ (Typ_PT, p2, "nxt ie ");

    for(i1=0; i1<lnNr; ++i1) {
      if(cTab[i1] > 0) continue; // skip already  done lines
      // 0=nix,1=1e-2a,2=1a-2a,3=1e-2e,4=1a-2e
      irc = UT3D_comp4pt (p1,p2, &lnTab[i1].p1,&lnTab[i1].p2, UT_TOL_pt);
      if       (irc < 1) {     // 0=nix
        ++reDo;
        continue;

      } else {
        // printf(" L[%d] _comp4pt %d\n",i1,irc);
        ++iFound;
        if(irc == 1) {    // 1=1e-2a
          cTab[i1]  = 1;  // 1=normal; 2=verkehrt.
          iTab1[iTab1Nr] = i1;
          ++iTab1Nr;
          ie = i1; // connect to end of ln[i1]
          p2 = &lnTab[ie].p2;
          goto L_nxt;

        } else if(irc == 2) {    // 2=1a-2a
          cTab[i1]  = 2;
          iTab2[iTab2Nr] = -i1;
          ++iTab2Nr;
          ia = i1; // connect to start of ln[i1]
          p1 = &lnTab[ia].p2;
          goto L_nxt;

        } else if(irc == 3) {    // 3=1e-2e
          cTab[i1]  = 2;
          iTab1[iTab1Nr] = -i1;
          ++iTab1Nr;
          ie = i1; // connect to start of ln[i1]
          p2 = &lnTab[ie].p1;
          goto L_nxt;

        } else if(irc == 4) {    // 4=1a-2e
          cTab[i1]  = 1;  // 1=normal; 2=verkehrt.
          iTab2[iTab2Nr] = i1;
          ++iTab2Nr;
          ia = i1; // connect to start of ln[i1]
          p1 = &lnTab[ia].p1;
          goto L_nxt;
        }
      }
    }




  //----------------------------------------------------------------
  // nun ist eine komplette Kette in iTab2/iTab1.
  // Add Kette als pTab --> cvSpc
  // combine iTab2 und iTab1; zuerst iTab2 verkehrt und dann daran
  // anschlieszend iTab1.

  // printf("-------------- iFound=%d reDo=%d iTab2/1Nr=%d %d\n",
           // iFound,reDo,iTab2Nr,iTab1Nr);


  if(iTab2Nr < 1) {    // keine iTab2 - add Startpunkt des ersten iTab1-objekts.
    ii = iTab1[0];
    // printf(" 0 add1 L[%d] P[%d] ",ii,ptNr);
    if(ptNr >= pTabSiz) goto L_eop;
    pTab[ptNr] = lnTab[ii].p1;
    // printf(" ii0 %f %f %f start\n",pTab[ptNr].x,pTab[ptNr].y,pTab[ptNr].z);
    ++ptNr;
    goto L_add_i1;
  }

  // add Startpunkt des letzten iTab2-objekts
    ii = iTab2[iTab2Nr-1];
    // printf(" 0 add2 L[%d] P[%d] ",ii,ptNr);
    if(ptNr >= pTabSiz) goto L_eop;
    if(ii >= 0) pTab[ptNr] = lnTab[ii].p1;
    else        pTab[ptNr] = lnTab[-ii].p2;
    // printf(" ii1 %f %f %f start\n",pTab[ptNr].x,pTab[ptNr].y,pTab[ptNr].z);
    ++ptNr;


  // add iTab2-Punkte
  for(i1=iTab2Nr-1; i1>=0; --i1) {
    ii = iTab2[i1];
    // printf(" %d add2 L[%d] P[%d] ",i1,ii,ptNr);
    if(ptNr >= pTabSiz) goto L_eop;
    if(ii >= 0) pTab[ptNr] = lnTab[ii].p2;
    else        pTab[ptNr] = lnTab[-ii].p1;
    // printf(" L_add_i-:%f %f %f\n",pTab[ptNr].x,pTab[ptNr].y,pTab[ptNr].z);
    ++ptNr;
  }


  // add iTab1-Punkte
  L_add_i1:
  if(iTab1Nr < 1) goto L_add_done;
  for(i1=0; i1<iTab1Nr; ++i1) {
    ii = iTab1[i1];
    // printf(" %d add1 L[%d] P[%d] ",i1,ii,ptNr);
    if(ptNr >= pTabSiz) goto L_eop;
    if(ii >= 0) pTab[ptNr] = lnTab[ii].p2;
    else        pTab[ptNr] = lnTab[-ii].p1;
    // printf(" L_add_i1: %f %f %f\n",pTab[ptNr].x,pTab[ptNr].y,pTab[ptNr].z);
      // DEB_dump_obj__ (Typ_LN, &lnTab[abs(ii)], "");
    ++ptNr;
  }



  L_add_done:
  // add obj to oSpc
    // GR_Disp_pTab (ptNr, pTab, SYM_TRI_S, 3);
    ipNr = ptNr - ipa;

      //-----TESTDISP--------
      // GR_Disp_cv (&pTab[ipa], ipNr, 9);
      // for(i1=0; i1<ptNr; ++i1) GR_Disp_txi (&pTab[i1], i1, 1);
      //-----TESTDISP--------



  //----------------------------------------------------------------
  // add object (polygon)
  ox1 = oSpc->next;
  irc = UME_add (oSpc, sizeof(ObjGX));
  if(irc < 0) goto L_eoo;
  ox1->typ  = Typ_PT;
  ox1->form = Typ_PT;
  ox1->siz  = ipNr;
  ox1->data = &pTab[ipa];

  // incr Anzahl SubObj's
  ox0->siz += 1;


  // work next curve
  if(reDo > 0) goto L_start;



  //----------------------------------------------------------------
  // update memSpace pSpc (for further use ..)
  irc = UME_add (pSpc, sizeof(Point)*ptNr);
  if(irc < 0) goto L_eop;  // gibts nicht ..



  // DEB_dump_ox_s_ (ox0, "ex INT_ln2pta__:");

  return 0;


  L_eoo:
    TX_Error("INT_ln2pta__ EOM_O");
    return -1;

  L_eot:
    TX_Error("INT_ln2pta__ EOM_T");
    return -1;

  L_eop:
    TX_Error("INT_ln2pta__ EOM_P");
    return -1;


}


//================================================================
  int INT_pta2crv__ (ObjGX *oxo, 
                     Memspc *oSpc, Memspc *pSpc, Memspc *tSpc,
                     int imod, int outTyp) {
//================================================================
/// \code
/// change Polygon to Line or Circ or Ellipse or BSpl-Curve
/// oSpc space for Input-Objects (Polygons)
/// pSpc space for Points
/// tSpc tempSpace
/// Input:
///    oSpc    Memspc with group of polygonCurves
///    imod    nr of polygonCurve to change
///    outTyp  Typ_CVBSP (10) | Typ_CVPOL (8)
/// Output:
///    oxo     LN|Plg|Bsp
///    RetCod  max nr of solutions
/// \endcode

// see  APT_store_obj > APT_int_obj > INT_intplsur


  int      irc, i1, ptNr, iMaxNr;
  Point    *pTab;
  Line     *ln1;
  CurvBSpl *cvBsp;
  CurvPoly *cvPlg;
  ObjGX    *ox0, *ox1, *oTab;


  // printf("INT_pta2crv__ siz=%d mod=%d\n",((ObjGX*)oSpc->start)->siz,imod);
  // printf("   oTyp=%d\n",*oTyp);
    // DEB_dump_ox_s_ ((ObjGX*)oSpc->start, "INT_pta2crv__");
    // DEB_dump_ox_0 ((ObjGX*)oSpc->start, "INT_pta2crv__");



  ox0 = oSpc->start;

  if(ox0->form != Typ_ObjGX) {TX_Error("INT_pta2crv__ E001"); return -1;}

  oTab = ox0->data;

  iMaxNr = ox0->siz;

  // loop tru Group
  for(i1=0; i1<iMaxNr; ++i1) {

    // skip this record if MODifier exists ...
    if(i1 != imod) continue;

    // next polygon
    ox1 = &oTab[i1];
    ptNr = ox1->siz;
    pTab = (Point*)ox1->data;

    // data must be a polygon
    // printf(" rec=%d form=%d siz=%d\n",i1,ox1->form,ox1->siz);   
    if(ox1->form != Typ_PT) {TX_Error("INT_pta2crv__ E002"); return -1;}

    // less than 5 points
    if(ptNr < 3) goto L_ln;
    if(ptNr < 5) goto L_plg;  //not enough points for Spline

    // check if polygon is linear = line
    // irc = UT3D_pt_ck_linear (ptNr, pTab, UT_TOL_pt); // 1=lin, 0=not.
    irc = UT3D_pt_ck_linear (ptNr, pTab, UT_DISP_cv); // 1=lin, 0=not.
    if(irc == 1) goto L_ln;
    if(outTyp == Typ_CVPOL) goto L_plg;
    goto L_bsp;



    //----------------------------------------------------------------
    // upgrade polygon --> Line
    L_ln:
    // printf(" plg --> LN !\n");

    // ((Line*)oxo)->p1 = pTab[0];
    // ((Line*)oxo)->p2 = pTab[1];

    // add Line into pSpc
    ln1 = pSpc->next;
    irc = UME_add (pSpc, sizeof(Line));
    if(irc < 0) goto L_eop;

    ln1->p1 = pTab[0];
    ln1->p2 = pTab[1];

    // change obj to Line
    // *oTyp = Typ_LN;
    oxo->typ  = Typ_LN;
    oxo->form = Typ_LN;
    oxo->siz  = 1;
    oxo->data = ln1;

    continue;



    //----------------------------------------------------------------
    // upgrade polygon --> BSpl-Curve
    L_bsp:
    // printf(" plg --> BSpl !\n");

    // add BSP into pSpc
    cvBsp = pSpc->next;
    irc = UME_add (pSpc, sizeof(CurvBSpl));
    if(irc < 0) goto L_eop;  // gibts nicht ..

    // approx
    irc = UT3D_bsp_pta__ (cvBsp, ptNr, pTab, UT_TOL_cv, 1., pSpc, tSpc);
    if(irc < 0) return irc;

    // change obj to BSpl-Curve
    // *oTyp = Typ_CV;
    oxo->typ  = Typ_CVBSP;
    oxo->form = Typ_CVBSP;
    oxo->siz  = 1;
    oxo->data = cvBsp;

    continue;



    //----------------------------------------------------------------
    // upgrade polygon --> Polygon
    L_plg:
    // printf(" plg --> Plg !\n");

    // add cvPlg into pSpc
    cvPlg = pSpc->next;
    irc = UME_add (pSpc, sizeof(CurvPoly));
    if(irc < 0) goto L_eop;  // gibts nicht ..

    // approx
    irc = UT3D_plg_pta (cvPlg, pTab, ptNr, pSpc);

    // change obj to BSpl-Curve
    // *oTyp = Typ_CV;
    oxo->typ  = Typ_CVPOL;
    oxo->form = Typ_CVPOL;
    oxo->siz  = 1;
    oxo->data = cvPlg;

    continue;


  }




  //----------------------------------------------------------------
    // DEB_dump_ox_s_ ((ObjGX*)oSpc->start, "ex INT_pta2crv__");
    // DEB_dump_ox_0 ((ObjGX*)oSpc->start, "INT_pta2crv__");
  return iMaxNr;


  L_eop:
    TX_Error("INT_pta2crv__ EOM_P");
    return -1;
}
 

//=====================================================================
  int INT_int_tria_tria (Line *lnTab, int *lnNr, int lnSiz,
                         Triangle *tri1Tab, int tri1Nr, int tri1Siz,
                         Triangle *tri2Tab, int tri2Nr, int tri2Siz) {
//=====================================================================
/// intersect Surface - Surface --> n LineSegments (see also INT_int_tria_pln

//- tri1Tab: GesamtBox (um alle Tria's) und eine Box um jedes einzelne 3Eck
//    erstellen;
//- Loop ueber alle 3Ecke der tri2Tab:
//   eine Box bilden,
//     Testen mit GesamtBox; wenn innen:
//     Loop ueber alle 3Ecke der Gruppe 1.
//       Alle Intersections speichern, Punkte ignorieren.



  int     i1, i2, irc;
  Point   pb1, pb2;
  Line    *b2Tab;           // tabel of boxes for tri2Tab


  // printf("INT_int_tria_tria %d %d\n",tri1Nr,tri2Nr);

    // TEST ONLY:
    // for(i1=0; i1<tri1Nr; ++i1) GR_Disp_tria (&tri1Tab[i1], 9);
    // for(i2=0; i2<tri2Nr; ++i2) GR_Disp_tria (&tri2Tab[i2], 9);
    // return 0;
    // for(i1=0; i1<tri1Nr; ++i1) GR_Disp_triv (&tri1Tab[i1], 8, i1, -1);
    // for(i2=0; i2<tri2Nr; ++i2) GR_Disp_triv (&tri2Tab[i2], 9, i2, -1);
    // i1=2;
    // // GR_Disp_triv(&tri1Tab[i1],8,i1);GR_Disp_tria(&tri1Tab[i1],8);
    // i2=0;
    // // GR_Disp_triv(&tri2Tab[i2],9,i2);GR_Disp_tria(&tri2Tab[i2],9);
    // _UT3D_ln_intTriaTria (lnTab, &tri1Tab[i1], &tri2Tab[i2]);
    // *lnNr = 1;
    // return 0;





  // tri2Tab: eine Box um jedes einzelne 3Eck erstellen;
  // tri2Tab: eine GesamtBox um alle 3Ecke erstellen;
  // Loop ueber alle 3Ecke der tri1Tab:
  //   eine Box bilden,
  //     Testen mit GesamtBox; wenn innen:
  //     Loop ueber alle 3Ecke der tri2Tab
  //       Alle Intersections speichern, Punkte ignorieren.



  *lnNr = 0;


  // create boxes for all trias of tri2Tab
  b2Tab = (Line*)malloc(tri2Nr * sizeof(Line));
  if(b2Tab == NULL) {TX_Error("INT_int_tria_tria EOM b2Tab"); return -1;}

  for(i1=0; i1<tri2Nr; ++i1) {
    UT3D_box_tria (&b2Tab[i1].p1, &b2Tab[i1].p2, &tri2Tab[i1], 0.);
      // GR_Disp_box (&b2Tab[i1].p1, &b2Tab[i1].p2, 9);
  }



    // TEST ONLY:
    // UT3D_box_tria (&pb1, &pb2, &tri1Tab[5], 0.);
    // i2 = 5;
    // irc = UT3D_ckBoxinBox1 (&pb1, &pb2, &b2Tab[i2].p1, &b2Tab[i2].p2);
    // printf(" irc = %d\n",irc);
    // return -1;




  // Loop ueber alle 3Ecke der tri2Tab
  for(i1=0; i1<tri1Nr; ++i1) {
    
    // create box from tri1Tab[i1]
    UT3D_box_tria (&pb1, &pb2, &tri1Tab[i1], 0.);
      // GR_Disp_box (&pb1, &pb2, 8);

    // Loop ueber alle 3Ecke der tri2Tab
    for(i2=0; i2<tri2Nr; ++i2) {

      // test if box for tri2Tab[i2] touches box tri1Tab[i1]
      irc = UT3D_ckBoxinBox1 (&pb1, &pb2, &b2Tab[i2].p1, &b2Tab[i2].p2);
        // if(irc < 0) printf(" ***** skip %d - %d\n",i1,i2);
      if(irc < 0) continue;

      // test intersection
      irc = UT3D_ln_intTriaTria (&lnTab[*lnNr], &tri1Tab[i1], &tri2Tab[i2]);
      if(irc < 0) continue;

      // add line
      if(*lnNr >= lnSiz) {
        TX_Error("INT_int_tria_tria EOM lnTab");
        free (b2Tab);
        return -1;
      }
      ++(*lnNr);
    }
  }

  free (b2Tab);

    // printf("ex INT_int_tria_tria %d\n",*lnNr);

  return 0;

}


//===========================================================================
  int INT_intsursur (int *oTyp, void **oDat,
                     int *s1Typ, long *s1Dbi, int s1Nr,
                     int *s2Typ, long *s2Dbi, int s2Nr,
                     int imod, int outTyp) {
//===========================================================================
/// \code
/// Intersect Surface - Surface
/// oDat  is char[OBJ_SIZ_MAX]; Typ_LN: struct Line;  else struct ObjGX
/// sTab *long-Liste von APT-Surfs, Anzahl sNr
/// imod Index auszugebender Objekte; 0=nur_erstes, 1=nur_zweites usw
/// Input:
///   imod    solutionNr
///   outTyp  solltyp out: Typ_CVBSP (10) | Typ_CVPOL (8)
/// RetCod:
///   >0      max nr of solutions
///   -1      Err
/// \endcode

// ACHTUNG das Tess. benutzt fast alle Speicher; loescht aus_typ usw !
// see INT_intplsur

  int       irc, i1, i2, lnNr, lnSiz, iMaxNr,
            tri1Siz, sur1Siz, tri1Nr, sur1Nr,
            tri2Siz, sur2Siz, tri2Nr, sur2Nr;
  ObjGX     *spc1=NULL, *spc2=NULL, *oTab, *oxp1, *oxp2, oo1;
  Point     *pTab;
  Line      *lnTab;
  Triangle  *tri1Tab, *tri2Tab;
  TypTsuSur *sur1Tab, *sur2Tab;
  Memspc    tSpc, oSpc, pSpc;



  // printf("INT_intsursur imod=%d outTyp=%d\n",imod,outTyp);
  // printf("  A0 = %d   %d\n",s1Dbi[0],s1Typ[0]);
  // printf("  A1 = %d   %d\n",s2Dbi[0],s2Typ[0]);


  if(s1Typ[0] == s2Typ[0]) {
    if(s1Dbi[0] == s2Dbi[0]) {
      TX_Error("***** ERROR: selfIntersect ..");
      return -1;
    }
  }


  //----------------------------------------------------------------
  // tesselate Surf # i1 --> spc1
  // ACHTUNG: benutzt fast alle Speicher !
  irc = TSU_tess_sTab (&spc1, s1Typ, s1Dbi, 1);
  if(irc < 0) return -1;


  // ACHTUNG: benutzt fast alle Speicher !
  irc = TSU_tess_sTab (&spc2, s2Typ, s2Dbi, 1);
  if(irc < 0) return -1;




  //----------------------------------------------------------------
  // get space for triangles --> triTab
  // tess_triaNr_bMsh (&sur1Siz, &tri1Siz, spc1);
  // tess_triaNr_bMsh (&sur2Siz, &tri2Siz, spc2);

  sur1Tab = (TypTsuSur*)memspc51;
  sur1Siz = sizeof(memspc101) / sizeof(TypTsuSur);


  tri1Tab = (Triangle*)memspc501;
  tri1Siz = sizeof(memspc501) / sizeof(Triangle);


  // get triangles from spc1 --> triTab
  // (triangles are pointers into tesselated data !)
  sur1Nr = sur1Siz;
  tri1Nr = tri1Siz;
  irc = TSU_ntria_bMsh__ (sur1Tab, &sur1Nr, sur1Siz,
                        tri1Tab, &tri1Nr, tri1Siz, spc1);
    // printf(" tri1Nr=%d\n",tri1Nr);

/*
    //----TEST:----------
    printf("ex TSU_ntria_bMsh__ irc=%d sur1Nr=%d\n",irc,sur1Nr);
    for(i1=0; i1<sur1Nr; ++i1) {
      printf(" sur[%d].ip1=%d ip2=%d typ=%d\n",i1,
             sur1Tab[i1].ip1,sur1Tab[i1].ip2,sur1Tab[i1].typ);
      for(i2=sur1Tab[i1].ip1; i2<=sur1Tab[i1].ip2; ++i2) {
        printf(" tri %d:\n",i2);
        DEB_dump_obj__ (Typ_PT, tri1Tab[i2].pa[0], "  p1=");
        DEB_dump_obj__ (Typ_PT, tri1Tab[i2].pa[1], "  p2=");
        DEB_dump_obj__ (Typ_PT, tri1Tab[i2].pa[2], "  p3=");
        // GR_Disp_tria (&tri1Tab[i2], 9);
      }
    }
    // return 0;
*/



  //----------------------------------------------------------------
  // space for triangles --> triTab

  sur2Tab = (TypTsuSur*)memspc52;
  sur2Siz = sizeof(memspc52) / sizeof(TypTsuSur);


  tri2Tab = (Triangle*)memspc201;
  tri2Siz = sizeof(memspc201) / sizeof(Triangle);


  // get triangles from spc1 --> triTab
  // (triangles are pointers into tesselated data !)
  sur2Nr = sur2Siz;
  tri2Nr = tri2Siz;
  irc = TSU_ntria_bMsh__ (sur2Tab, &sur2Nr, sur2Siz,
                        tri2Tab, &tri2Nr, tri2Siz, spc2);
    // printf(" tri2Nr=%d\n",tri2Nr);

/*
    //----TEST:----------
    printf("ex TSU_ntria_bMsh__ irc=%d sur2Nr=%d\n",irc,sur2Nr);
    for(i1=0; i1<sur2Nr; ++i1) {
      printf(" sur[%d].ip1=%d ip2=%d typ=%d\n",i1,
             sur2Tab[i1].ip1,sur2Tab[i1].ip2,sur2Tab[i1].typ);
      for(i2=sur2Tab[i1].ip1; i2<=sur2Tab[i1].ip2; ++i2) {
        printf(" tri %d:\n",i2);
        DEB_dump_obj__ (Typ_PT, tri2Tab[i2].pa[0], "  p1=");
        DEB_dump_obj__ (Typ_PT, tri2Tab[i2].pa[1], "  p2=");
        DEB_dump_obj__ (Typ_PT, tri2Tab[i2].pa[2], "  p3=");
        // GR_Disp_tria (&tri2Tab[i2], 9);
      }
    }
    // return 0;
*/




  //----------------------------------------------------------------
  // space for resulting linesegments --> lnTab
  lnTab = (Line*)memspc101;
  lnSiz = sizeof(memspc101) / sizeof(Line);
  lnNr  = 0;


  // intersect Surface - Surface --> n LineSegments (see also INT_int_tria_pln
  INT_int_tria_tria (lnTab, &lnNr, lnSiz, tri1Tab, tri1Nr, tri1Siz,
                                          tri2Tab, tri2Nr, tri2Siz);

  L_susu9:
  // free tesselated data
  if(spc1) free (spc1);
  // if(spc2) free (spc2);

  // free tesselated data
  if(spc2) free (spc2);
  // if(spc2) free (spc2);

  // free tri1Tab, tri2Tab.

  if(lnNr < 1) return -1;


    // TEST ONLY:
    // for(i1=0; i1<lnNr; ++i1) GR_Disp_ln (&lnTab[i1], 9);
    // return -1;



  //----------------------------------------------------------------
  // now we have <lnNr> lineSegments in lnTab
  // joint lineSegments to 1-n concatenated polygons

  // space for Objects (Polygons)
  UME_init (&oSpc, memspc52, sizeof(memspc52));

  // space for Points
  UME_init (&pSpc, memspc201, sizeof(memspc201));

  // get tempSpace
  UME_init (&tSpc, memspc50, sizeof(memspc50));


  // connect LineSegments  --> Group of Polygon
  // Output: typ=Typ_ObjGX, form=Typ_ObjGX; dahinter n pointTables (PT-PT)
  INT_ln2pta__ (&oSpc, &pSpc, &tSpc, lnTab, lnNr);

/*
    // TESTS:
    DEB_dump_ox_s_ (oSpc.start, "Group of Polygons:");
    oxp1 = (ObjGX*)oSpc.start;
    printf(" typ=%d form=%d\n",oxp1->typ,oxp1->form);
    printf(" Nr of concatenated curves = siz = %d\n",oxp1->siz);
    oTab = oxp1->data;
    for(i1=0; i1<oxp1->siz; ++i1) {
      oxp2 = &oTab[i1]; //.data;
      printf(" cv[%d]: typ=%d form=%d siz=%d\n",i1,
              oxp2->typ,oxp2->form,oxp2->siz);
      pTab = oxp2->data;
      // GR_Disp_pTab (oTab[i1].siz, pTab, SYM_STAR_S, 2);
      GR_Disp_cv (pTab, oxp2->siz, 9);
      // for(i2=0; i2<oTab[i1].siz; ++i2) {
        // DEB_dump_obj__(Typ_PT, &pTab[i2],"seg%d P%d ",i1,i2);
      // }
    }
    return -1;
*/




  //----------------------------------------------------------------
  // 2008-10-24: entfernen; macht Ergebnis fuer BSP-Curves sehr ungenau.
  // loop tru polygons; remove unnecessary(straight) points
  if(outTyp == Typ_CVBSP) goto L_iss_9;
  oxp1 = (ObjGX*)oSpc.start;
  oTab = oxp1->data;
  for(i1=0; i1<oxp1->siz; ++i1) {
    if(i1 != imod) continue;        // skip unused
    // printf("_intplsur 1.oTab[%d]; siz=%d\n",i1,oTab[i1].siz);
    if(oTab[i1].siz > 2) {
      i2 = oTab[i1].siz;
      UT3D_cv3_linear (&i2, oTab[i1].data, UT_TOL_cv);
      oTab[i1].siz = i2;
    }
    // printf("_intplsur 2.oTab[%d]; siz=%d\n",i1,oTab[i1].siz);
  }
/*
    //----TEST:----------
    oxp1 = (ObjGX*)oSpc.start;
    printf(" Nr of concatenated curves = siz = %d\n",oxp1->siz);
    oTab = oxp1->data;
    for(i1=0; i1<oxp1->siz; ++i1) {
      if(i1 != imod) continue;        // skip unused
      pTab = oTab[i1].data;
      GR_Disp_cv (pTab, oTab[i1].siz, 9);
      // for(i2=0; i2<oTab[i1].siz; ++i2) {
        // DEB_dump_obj__(Typ_PT, &pTab[i2],"seg%d P%d ",i1,i2);
      // }
    }
    return 0;
*/




  //----------------------------------------------------------------
  // change Polygon to Line or Circ or Ellipse or Polygon or BSpl-Curve
  L_iss_9:
  // Output is typ=Group in (ObjGX*)oSpc->start !)
  iMaxNr = INT_pta2crv__ (&oo1, &oSpc, &pSpc, &tSpc, imod, outTyp);
  if(iMaxNr < 0) return irc;


  *oTyp = oo1.form;
  *oDat = oo1.data;

    // DEB_dump_obj__ (*oTyp, *oDat, "ex INT_intsursur\n");

/*
  //-------TESTAUSGABE----------
  printf(" oTyp=%d\n",*oTyp);
  oxp1 = oDat;
  DEB_dump_ox_s_ (oxp1, "xSpc");
  if(oxp1->typ == Typ_CVBSP) {
    GR_Disp_CvBSp (oxp1->data, 9);
    DEB_dump_obj__(Typ_CVBSP, oxp1->data, "");
  }
  oxp1 = (ObjGX*)oSpc.start;
  i2 = oxp1->siz;
  for(i1=0; i1<i2; ++i1) {
    (char*)oxp1 += sizeof(ObjGX); // obj 1 from group
    printf(" OOOOOOOOOOOOOOOOOOOOOOO %d\n",i1);
    // DEB_dump_ox_s_ (oxp1, "oxp1");
    DEB_dump_ox_0 (oxp1, "oxp1 %d",i1);
  }
  //-------TESTAUSGABE----------
*/



/*
  // Objekt raus -> oTyp, xSpc
  oxp1 = (ObjGX*)oSpc.start;   // ParentObj
  oTab = oxp1->data;           // objTable
  oxp1 = &oTab[0];             // 1 obj
  *oTyp = oxp1->typ;
  // den Datenrecord selbst kopieren
  // *((Line*)xSpc) = *((Line*)oxp1->data);
  memcpy (xSpc, oxp1->data, OBJ_SIZ_MAX);

  DEB_dump_ox_0 (xSpc, "xSpc %d",i1);
*/


/*
  //----------------------------------------------------------------

  // Wenn Modifier > 0 dann das entspechende Obj ausgeben
  // else eine Group generieren (noch nicht implementiert).
  imod = 1;  // HIER NUR TEST !!
  if(imod > 0 ) {
    // ..
  // } else {
    printf("**** Int1-GroupObject not jet implemented ***** \n");
  }
*/



  return iMaxNr;

}

//================ EOF ===========================================
