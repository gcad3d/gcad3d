//                 xa_cont.c
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
\file  ../xa/xa_cont.c
\brief create-countour-functions AP_cont_ 
\code
=====================================================
List_functions_start:

AP_cont_nxt       main
AP_cont_PTmod1    get index
AP_cont_PTmod     get index
AP_cont_nearPt    get index of nearest Point
AP_cont_skip      skip CW/CCW und MOD()
AP_cont_mod       get CW/CCW und MOD(#) 
AP_cont_Conn__    connect
AP_cont_ConnLn    give ConnectionLine
AP_cont_inv       invert lin.curves in structs according to iMod
AP_cont_intPt     select intersectionPoint
// AP_cont_nxPT      PT -> ObjGX

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


*/

#ifdef _MSC_VER
#include "MS_Def0.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>




// #include "../ut/ut_umem.h"
#include "../ut/ut_geo.h"         // OFF, ON ..
#include "../ut/ut_txt.h"
#include "../ut/ut_TX.h"          // TX_Error



#include "../db/ut_DB.h"
#include "../ut/func_types.h"               // UI_Func... SYM_..
#include "../ci/NC_apt.h"              // T_CW




//===========================================================================
// Externals aus NC_Main.c:
extern int     APT_obj_stat;          // 0=OK, 1=workmode (in statu nascendi)



//===========================================================================
// Local:
static char  o1Dat[OBJ_SIZ_MAX];
static char  o2Dat[OBJ_SIZ_MAX];
static int   o1Ind, o2Ind, o1Typ, o2Typ;
static Point pto1a, pto1e;
static Point pto2a, pto2e;
static int   modFlg;    // 0: keine MOD()-Angabe fuer dieses Objekt; 1=Ja.
  //  -1=undefined; 0-n = user-selection
static int   o1_mod;    // 0-n = index of MOD or default
static int   o1_sr;     // rot-sense for new-obj;   1=CCW;-1=CW 0=undefined
static int   contNr;    // 1 = first obj of contour ..
static int   o1_clo;     // if new inputObj is closed; 0=yes, else=no





//==============================================================================
  int AP_cont_PTmod1 (int imod) {
//==============================================================================
// den Index holen; Punkt muss != p0 sein
// wenn imod (MOD(#) gegeben: diesen Punkt waehlen;
// else 0 (den ersten Punkt der ptTab)
//        (ptTab[0] wurde auf den dem Endpunkt naechsten Punkt gesetzt)

// see AP_cont_PTmod


  // printf("AP_cont_PTmod %d %d Flag=%d\n",ptNr,imod,modFlg);


  // test ob MOD() gegeben
  if(modFlg >= 0) return imod;

  return 0;

}


//==============================================================================
  int AP_cont_PTmod (Point *p0, Point *ptTab, int ptNr, int imod) {
//==============================================================================
// den Index holen; Punkt muss != p0 sein
// wenn imod (MOD(#) gegeben: diesen Punkt waehlen;
// else den dem p0 naechsten Punkt aus ptTab waehlen.


  // printf("AP_cont_PTmod %d %d Flag=%d\n",ptNr,imod,modFlg);


  // test ob MOD() gegeben
  if(modFlg >= 0) return imod;

  return AP_cont_nearPt (p0, ptTab, ptNr);

}


//==============================================================================
  int AP_cont_nearPt (Point *p0, Point *ptTab, int ptNr) {
//==============================================================================
// return index of nearest Point from n points;
// jedoch nicht jenen, der mit p0 ident ist !!

// vorher testen ob Endpunkte ident (siehe AC > AC)

// usage zB:
// pto = pta[AP_cont_nearPt(&startPt, pta, i1)];



  int     i1, iind;
  double  d1, lmax;


  // UT3D_stru_dump (Typ_PT, p0, "AP_cont_nearPt");

  if(ptNr < 2) return 0;

  lmax = UT_VAL_MAX;


  for(i1=0; i1<ptNr; ++i1) {
    // printf(" nearPt[%d] %f %f %f\n",i1,ptTab[i1].x,ptTab[i1].y,ptTab[i1].z);
    d1 = UT3D_len_2pt (p0, &ptTab[i1]);

    if(fabs(d1) < UT_TOL_cv) continue;  // UT_TOL_pt geht bei curves nicht ..

    if(d1 < lmax) {
      iind = i1;
      lmax = d1;
    }
  }

  // UT3D_stru_dump (Typ_PT, &ptTab[iind], "ex AP_cont_nearPt %d ",iind);

  return iind;

}


//==============================================================================
  int AP_cont_skip (int *iIn,
                    int aus_anz,int aus_typ[],double aus_tab[]) {
//==============================================================================
// skip CW/CCW und MOD()
// RetCod = 0: CCW od CW od MOD found; skip this Record.
// RetCod = 1: normal weiter ...

// es kann CW und MOD vorkommen; dann wird iIn entsprechen weitergesetzt.


  int  irc, ival;

  irc = 1;


  // check for CW CCW
  L_noAmoi:
  if(aus_typ[*iIn] != Typ_cmdNCsub) goto L_mod;
  ival = aus_tab[*iIn];
  if((ival == T_CW)||(ival == T_CCW)) {
    *iIn += 1;
    irc = 0;
  }


  // test for MOD
  L_mod:
  if(aus_typ[*iIn] != Typ_modif) goto L_fertig;
  *iIn += 1;
  irc = 0;
  goto L_noAmoi;



  L_fertig:
  return irc;   // kein CW oder MOD ..

}


//==============================================================================
  int AP_cont_mod (int *dreh, int *imod, int iIn,
                   int aus_anz,int aus_typ[],double aus_tab[]) {
//==============================================================================
/// \code
/// returns rotSense (CW|CCW) and modifier (MOD)
/// Output:
///   dreh      1=CCW; -1=CW; 0=undefined
///   imod      0=undefined, else MOD#
///   modFlg (extern) -1=imod not valid; else MOD#
/// \endcode

// CW/CCW und MOD(#) aus daten holen (ein Element im voraus)



  // printf("AP_cont_mod %d typ=%d tab=%f\n",iIn,aus_typ[iIn+1],aus_tab[iIn+1]);


  *dreh = 0;
  *imod = 0;
  // modFlg = -1;



  L_noAmoi:
  // check for CW|CCW
  if((iIn+1) >= aus_anz) goto L_fertig;
  if(aus_typ[iIn+1] != Typ_cmdNCsub) goto L_mod;

  if(aus_tab[iIn+1] == T_CW) {
    *dreh = -1;
  } else if(aus_tab[iIn+1] == T_CCW) {
    *dreh = 1;
  } else {
    goto L_fertig;
  }
  ++iIn;



  L_mod:
  // check for MOD
  if((iIn+1) >= aus_anz) goto L_fertig;
  if(aus_typ[iIn+1] != Typ_modif) goto L_fertig;

  *imod = aus_tab[iIn+1];
  // modFlg = *imod;
  ++iIn;
    // printf(" found MOD %d\n",*imod);
  goto L_noAmoi;




  L_fertig:
  // printf("ex AP_cont_mod [%d] dreh=%d imod=%d\n",iIn,*dreh,*imod);
  return 0;

}

//================================================================
  int AP_cont_Conn__ () {
//================================================================
// connect;  Punkte pto1a,pto1e, pto2a,pto2e mussen schon gesetzt sein.
// RetCod 0 = Verbindung vorhanden; obj ? ausgeben;
// RetCod 1 = keine Verbindung gefunden; intersect obj's

  int      ConnTyp;
  double   d1;


  // printf("AP_cont_Conn__ %d %d\n",o2Typ,o1Typ);
  // UT3D_stru_dump (Typ_PT, &pto2a, "   o2a:");
  // UT3D_stru_dump (Typ_PT, &pto2e, "   o2e:");
  // UT3D_stru_dump (Typ_PT, &pto1a, "   o1a:");
  // UT3D_stru_dump (Typ_PT, &pto1e, "   o1e:");


  // geringsten Abstand von 4 Punkten suchen
  // ACHTUNG returncodes: 2-1 verkehrt !
  ConnTyp = UT3D_minLen_4pt (&d1, &pto2a, &pto2e, &pto1a, &pto1e);
  if(d1 > UT_TOL_cv) return 1;           // goto intersect

  // invert lin.curves in structs according to ConnTyp
  AP_cont_inv (ConnTyp);
  return 0;

}


//================================================================
 int AP_cont_ConnLn (Line *lno, int iMod,
                     Point *p1a,Point *p1e,Point *p2a,Point *p2e) {
//================================================================
// give ConnectionLine according to ConnTyp


  // printf("AP_cont_ConnLn %d\n",iMod);

  if((iMod == 1)||(iMod == 3)) lno->p1 = *p1e;
  if((iMod == 2)||(iMod == 4)) lno->p1 = *p1a;

  if((iMod == 1)||(iMod == 2)) lno->p2 = *p2a;
  if((iMod == 3)||(iMod == 4)) lno->p2 = *p2e;

  // UT3D_stru_dump (Typ_LN, lno, "ex AP_cont_ConnLn:\n");

  return 0;

}


//======================================================================
  int AP_cont_inv (int iMod) {
//======================================================================
// invert lin.curves in structs according to iMod;
// speziell als Nachfolgeprog f UT3D_comp4pt u UT3D_minLen_4pt
// Input imod
// iMod=1: nix tun; OK
// iMod=2: oldObj 2 umdrehen ..
// iMod=3: newObj 1 umdrehen ..
// iMod=4: old and newObj umdrehen ..

  int    irc;

  // printf("AP_cont_inv %d %d %d\n",o2Typ,o1Typ,iMod);

  //----------------------------------------------------------------
  // invert 1 obj
  if((iMod != 2)&&(iMod != 4)) goto L_inv2;

  // printf(" inv o2\n");
  irc = UTO_stru_inv (o2Typ, o2Dat);
  if(irc < 0) {TX_Error("AP_cont_inv E001 %d",o2Typ); return -1; }

  MEM_swap__ (&pto2a, &pto2e, sizeof(Point));


  //----------------------------------------------------------------
  L_inv2:
  if((iMod != 3)&&(iMod != 4)) goto L_done;

 
  // printf(" inv o1\n");
  irc = UTO_stru_inv (o1Typ, o1Dat);
  if(irc < 0) {TX_Error("AP_cont_inv E002 %d",o1Typ); return -1; }

  MEM_swap__ (&pto1a, &pto1e, sizeof(Point));


  //----------------------------------------------------------------
  L_done:
  return 0;

}
 

//================================================================
  int AP_cont_intPt (int iNr, Point *pta) {
//================================================================
// select intersectionpoint.
// Input:
//   iNr      nr of Intersectionpoints.
//   *pta     Intersectionpoints.
//   modFlg G user-selected index of Intersectionpoint; -1=undefined.
//   o1Dat  G new_obj o1Typ
//   o2Dat  G old_obj o2Typ
// Output:
//    ?

// oldobj umdrehen wenn erforderlich (nur wenn es das erste Obj ist)
// newobj umdrehen wenn erforderlich

// see also AP_cont_PTmod

  int    irc, i1, i2, ii;
  double d1, d2;


  printf("AP_cont_intPt iNr=%d modFlg=%d contNr=%d\n",iNr,modFlg,contNr);
  // UT3D_stru_dump (o2Typ, o2Dat, "  old obj-%d in:\n",o1Typ);
  // UT3D_stru_dump (o1Typ, o1Dat, "  new obj-%d in:\n",o1Typ);
  // for(i1=0;i1<iNr;++i1) UT3D_stru_dump (Typ_PT, &pta[i1], " i[%d]",i1);
 


  // intPt auswaehlen
  if(iNr < 1) {TX_Error("no intersectionpoint .."); return -1;} // keine Loesung


  if(modFlg >= 0) {
    // user has Intersectionpoint defined;
    if(modFlg > iNr) TX_Print("Error Modifier");
    ii = modFlg;
    // fehlt hier: pto2e = pta[ii];   ???
    pto2e = pta[ii]; // 2014-05-07
    goto L_ck_old;
  }


/*
  // Wenn o2 das erste Elemet der Kontur ist:
  // ist es das erste Element der Kontur ?
  if(contNr == 2) {   // old = 1, new = 2 !

    if(iNr < 2) {
      // 1. Object, 1 intersectionPoint: 
      ii = 0;
      // if intersectionPoint ist nearer to Endpoint invert old obj.
      d1 = UT3D_lenq_PtPt (&pto2a, &pta[0]);
      d2 = UT3D_lenq_PtPt (&pto2e, &pta[0]);
      if(d1 < d2) {
        // printf(" inv old obj\n");
        irc = UTO_stru_inv (o2Typ, o2Dat);     // old obj umdrehen ..
        if(irc < 0) return -1;
        MEM_swap__ (&pto2a, &pto2e, sizeof(Point));
      }

    } else {
      // 1. Object, mehrere intersectionPoints: 
      // check which intersectionPoint ist nearest to startPoint or endPt
      i1 = UT3D_ipt_cknear_npt (&pto2a, pta, iNr);
      d1 = UT3D_lenq_PtPt (&pto2a, &pta[i1]);
      i2 = UT3D_ipt_cknear_npt (&pto2e, pta, iNr);
      d2 = UT3D_lenq_PtPt (&pto2e, &pta[i2]);
      // printf(" d1=%f d2=%f\n",d1,d2);
      if(d1 < d2) {
        // printf(" inv old obj\n");
        ii = i1;
        irc = UTO_stru_inv (o2Typ, o2Dat);     // old obj umdrehen ..
        if(irc < 0) return -1;
        MEM_swap__ (&pto2a, &pto2e, sizeof(Point));
      } else {
        ii = i2;
      }
    }


  } else {            // do not turn oldObj; 
*/
    L_ii_ck:
    if(iNr < 2) { ii = 0; goto L_ii_set;}

    // wenn ein intersectionPoint ident ist mit dem oldStartPt, dann diesen
    // Punkt ignorieren (aus der Tabelle loeschen).
    for(i1=0;i1<iNr;++i1) {
      if(UT3D_comp2pt(&pta[i1], &pto2a, UT_TOL_pt) == 1) {
        MEM_del_nrec (&iNr, pta, i1, 1, sizeof(Point));
        goto L_ii_ck;
      }
    }

    // check which intersectionPoint ist nearest to endPt
    ii = UT3D_ipt_cknear_npt (&pto2e, pta, iNr);
  // }


  L_ii_set:
    printf(" L_ii_set: ii=%d\n",ii);

  // pta[ii] soll nun der neue Endpunkt von o2 und der Startpunkt von o1 werden 
  pto2e = pta[ii];  // set endPt of old Obj = intPt
    // GR_Disp_pt(&pto2e, SYM_TRI_B, 2);


  // Wenn es das erste Element ist:
  // Und wenn es ein Vollkreis ist: auch den KreisStartpunkt setzen !
  if(contNr == 2) {
    if(o2Typ == Typ_CI) {
      if(UT3D_ck_ci360 ((Circ*)o2Dat) == YES) {
        pto2a = pta[ii];
      }
    }
  }





  //----------------------------------------------------------------
  L_ck_old:   // selected point is pto2e

  // change old Obj:   set endPt = intPt pto2e
  UTO_set_ptlim (o2Typ, o2Dat, &pto2a, NULL, &pta[ii], NULL);


  // check new obj:
  
  // closed curve: change startpoint and endpoint to intPt
  if(!o1_clo) {
      printf(" obj closed !\n");
    UTO_set_ptlim (o1Typ, o1Dat, &pta[ii], NULL, &pta[ii], NULL);
    goto L_exit;
  }


  // wenn intPt naeher an endPt als an startPt umdrehen
  i1 = UT3D_ipt_cknear_3pt (&pta[ii], &pto1a, &pto1e);
  if(i1 == 1) {
    // printf(" inv new obj\n");
    irc = UTO_stru_inv (o1Typ, o1Dat);     // new obj umdrehen ..
    if(irc < 0) return -1;
    MEM_swap__ (&pto1a, &pto1e, sizeof(Point));
  }


  // ist der neue Startpoint identisch mit dem Endpoint:
  // objekt umdrehen und fertig.
  if(UT3D_comp2pt(&pta[ii], &pto1e, UT_TOL_pt) == 1) {
    // jedoch nicht fuer Vollkreise
    if(UT3D_comp2pt(&pto1a, &pto1e, UT_TOL_pt) == 0) {
      UTO_stru_inv (o2Typ, o2Dat);
    }

  // new Obj:   set startPt = intPt
  // UT3D_ci_cip2 ((Circ*)o2Dat, &pta[ii]); // Endpunkt am alten Cir
  } else {
    pto1a = pta[ii];

    // wenn newObj == Vollkreis, dann auch den Endpunkt gleich setzen ..
    if(o1Typ == Typ_CI) {
      if(UT3D_ck_ci360 ((Circ*)o1Dat) == YES) {
        pto1e = pta[ii];
      }
    }

    UTO_set_ptlim (o1Typ, o1Dat, &pto1a, NULL, &pto1e, NULL);
  }


  L_exit:
  // UT3D_stru_dump (o2Typ, o2Dat, "  old obj-%d out:\n",o1Typ);
  // UT3D_stru_dump (o1Typ, o1Dat, "  new obj-%d out:\n",o1Typ);


  return 0;

}
 

/*
//==============================================================================
  int AP_cont_nxt (ObjGX *oo1, int *oo1i, ObjGX *oo2, int *oo2i, 
                   Memspc *ooDat, ObjGX *o1, int oInd, int dreh, int imod) {
//==============================================================================
// output next obj of contour
// Input:
//   o1    next (new) input-object
//   oInd  dbi - UNUSED ?
//   dreh  1=CCW;-1=CW 0=undefined
//   imod  MOD-1; valid only if modFlg >= 0
// Output:
//   oo1   next output-object

// calling-sequence:
//   - first call: init with o1=NULL
//   ..
//   - last call: o1->Typ=TYP_FuncEnd(265)

// global data:
//   o1_sr     rotSense of o1
//   o1_mod    pointIndex for more than 1 points (eg intersection,projection)
//   o1_clo    if input-obj is closed
//   o1Typ,o1Dat     type and struct of input-obj o1. Can modify o1Dat (not o1).
//   o2Typ,o2Dat     type and struct of old obj
//   pto1a, pto1e    start- end endpoint of input-obj o1Typ,o1Dat
//   vo1a, vo1e      parameters of pto1a, pto1e
//   pto2a, pto2e    start- end endpoint of o2Typ,o2Dat
//   o1Ind, o2Ind    dbi of o1Dat,o2Dat - UNUSED ?

// TODO: o2,o3 auslagern -> o2Dat, o3Dat


// mode of operation:
//   copy input-obj o1 -> o1Typ,o1Dat. 


// das neue Objekt ist o1, das vorherige o2, das zuvor o3.
// Alle neuen Obj. werden in ooDat gespeichert
// Index geht in oo1i bzw oo2i out.
// call aus APT_decode_ccv: ooDat = memspc101.
// RetCod = 0 = OK;




static ObjGX o2, o3;

// o2Typ: der genaue Kurventyp von o2; o2.typ=TypCV, o2Typ= TypCVPOL.


  int      irc, i1, i2, new_typ, old_typ, o1Siz, ConnLn, ConnTyp;
  long     DBind;
  double   d1, d2, da[10], da1[10], vo1a, vo1e;
  Point    pto, pt1, pt2, *ppt, pta[16];
  Point    *p1a, *p1e, *p2a, *p2e;
  Vector   vc1, vc2;
  Line     ln1, ln2, *pln, *pl2;
  Circ     ci1, *pci, *pc2;
  CurvElli  *pell, *pel1;
  CurvPoly  *pplg;
  CurvBSpl *pbsp;
  ObjGX    ox1, *pox;
  // ObjX     op1;
  void     *op1;





  //==========================================================
  // init Kontursucher
  if(o1 == NULL) {
      // printf("AP_cont_nxt init\n");
    o3.typ = Typ_Error;            // Startsituation.
    o2.typ = Typ_Error;            // Startsituation.
    o2Ind  = 0;
    contNr = 0;
    return 0;
  }




  //==========================================================
  *oo1i = 0;
  *oo2i = 0;


  new_typ = o1->typ;
  o1Typ   = o1->typ;
  old_typ = o2.typ;

  oo1->typ = Typ_Error;
  oo2->typ = Typ_Error;




  ConnLn = 0;  // def = keine ConnectionLine
  ++contNr;       // 1 = first obj of contur ..



  modFlg = imod;
  o1_mod = modFlg;
  if(o1_mod < 0) o1_mod = 0;

  o1_sr = dreh; //   dreh  1=CCW;-1=CW 0=undefined




  printf("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\n");
  printf("AP_cont_nxt new_typ=%d old_typ=%d dreh=%d imod=%d\n",
           new_typ, old_typ, dreh, modFlg, imod);
  // printf("  oInd=%d\n",oInd);
  printf("  o1_mod=%d o1_sr=%d\n",o1_mod,o1_sr);




  //=====================================================================
  //================ CloseContourFunktion ===============================
  //=====================================================================
  if(new_typ == TYP_FuncEnd) {
      printf(" new_typ = TYP_FuncInit | TYP_FuncEnd\n");

    o1Siz = 0;



    //----------------------------------------------------------------
    if(old_typ == Typ_PT) {
      // printf(" -- PT > 0\n");

      // nix tun -
      goto  L_exit;




    //----------------------------------------------------------------
    } else if(old_typ == Typ_LN) {
      // printf(" -- LN > 0\n");
      // UT3D_stru_dump (Typ_LN,    o2Dat,   "  old LN:\n");

      // Ausg. Line (Line*)o2Dat
      ln1.p1 = ((Line*)o2Dat)->p1;
      ln1.p2 = ((Line*)o2Dat)->p2;
      *oo1i = o2Ind;
      goto L_ex_1L;




    //----------------------------------------------------------------
    } else if(old_typ == Typ_CI) {
      // printf(" -- AC > 0   %d %d %d\n",new_typ,old_typ,o3.typ);
      // printf(" temp-Flag=%d\n",APT_obj_stat);

      // Ausg. Circ (Circ*)o2Dat
      pci   = (void*)o2Dat;
      ci1 = *pci;              // copy circ
        // UT3D_stru_dump (Typ_CI, &ci1, "last AC=\n");


      // // wenn Vollkreis einziges Obj der Kontur ist: nicht aendern.
      // if(APT_obj_stat == 1) {                // nur waehrend create CCV
        // if(UT3D_ck_ci360(&ci1) == YES) {
          // if(o3.typ != Typ_Error) {
            // // else halbieren, damit man Richtung erkennt.
            // UT3D_ci_cipt180 (&ci1, &ci1.p1);
          // }
        // }
      // }


      // den Kreis ci1 raus, fertig.
      goto L_ex_1C;




    //----------------------------------------------------------------
    } else if(old_typ == Typ_CV) {
        printf(" -- CV > 0  o2typ=%d new_typ=%d\n",o2Typ,new_typ);
        printf(" APT_obj_stat=%d\n",APT_obj_stat);

      //.......................................................
      if(o2Typ == Typ_CVTRM) {
        // printf(" -- Typ_CVTRM > 0\n");
        goto L_ex_CCV;


      //.......................................................
      } else if(o2Typ == Typ_CVELL) {
        pell = (void*)o2Dat;

        // // if(APT_obj_stat == 1)                 // nur waehrend create CCV
        // if(new_typ == TYP_FuncInit) {     // 360-Grad Elli halbieren
          // if(UT3D_ck_el360(pell) == 0) {        // 0=yes
            // UT3D_el_elpt180 (pell, &pell->p1);  // make 180-deg-curve
            // // UT3D_pt_opp2pt (&pell->p2,&pell->pc,&pell->p1);
            // // printf(" new p2=%f,%f,%f\n",pell->p2.x,pell->p2.y,pell->p2.z);
          // }
        // }

        goto L_ex_ELL;      // Ellipse o2Dat speichern und ausgeben



      //.......................................................
      } else if(o2Typ == Typ_CVCLOT) {
        // printf(" -- CVCLOT > 0\n");
        goto L_ex_CLOT;



      //.......................................................
      } else if(o2Typ == Typ_CVBSP) {
        // printf(" -- CVBSP > 0\n");
        // printf("v0=%f v1=%f\n",((CurvBSpl*)o2Dat)->v0,((CurvBSpl*)o2Dat)->v1);

        // wenn CV am Endpunkt selektiert wurde, ist v0=v1 - nix dargestellt.
        if(UTP_comp2db(((CurvBSpl*)o2Dat)->v0,
                       ((CurvBSpl*)o2Dat)->v1, UT_TOL_pt)) {
          // ja, v0 und v1 sind gleich;
          // wenn (v0 != kvTab[0]):  v0 = kvTab[0];
          // else v0 = kvTab[ptNr+deg+1];
          if(UTP_comp2db(((CurvBSpl*)o2Dat)->v0,
                         ((CurvBSpl*)o2Dat)->kvTab[0], UT_TOL_pt))
            ((CurvBSpl*)o2Dat)->v0 = ((CurvBSpl*)o2Dat)->kvTab[0];
          else {
            i1 = ((CurvBSpl*)o2Dat)->ptNr + ((CurvBSpl*)o2Dat)->deg + 1;
            ((CurvBSpl*)o2Dat)->v0 = ((CurvBSpl*)o2Dat)->kvTab[i1];
          }
        }
        // printf("v0=%f v1=%f\n",((CurvBSpl*)o2Dat)->v0,((CurvBSpl*)o2Dat)->v1);
        goto L_ex_CV;




      //.......................................................
      } else if(o2Typ == Typ_CVRBSP) {
        // printf(" -- CVRBSP > 0\n");
        // printf("v0=%f v1=%f\n",((CurvRBSpl*)o2Dat)->v0,((CurvRBSpl*)o2Dat)->v1);

        // wenn CV am Endpunkt selektiert wurde, ist v0=v1 - nix dargestellt.
        if(UTP_comp2db(((CurvRBSpl*)o2Dat)->v0,
                       ((CurvRBSpl*)o2Dat)->v1, UT_TOL_pt)) {
          // ja, v0 und v1 sind gleich;
          // wenn (v0 != kvTab[0]):  v0 = kvTab[0];
          // else v0 = kvTab[ptNr+deg+1];
          if(UTP_comp2db(((CurvRBSpl*)o2Dat)->v0,
                         ((CurvRBSpl*)o2Dat)->kvTab[0], UT_TOL_pt))
            ((CurvRBSpl*)o2Dat)->v0 = ((CurvRBSpl*)o2Dat)->kvTab[0];
          else {
            i1 = ((CurvRBSpl*)o2Dat)->ptNr + ((CurvRBSpl*)o2Dat)->deg + 1;
            ((CurvRBSpl*)o2Dat)->v0 = ((CurvRBSpl*)o2Dat)->kvTab[i1];
          }
        }
        // printf("v0=%f v1=%f\n",((CurvBSpl*)o2Dat)->v0,((CurvBSpl*)o2Dat)->v1);
        goto L_ex_RBSP;




      //.......................................................
      } else if(o2Typ == Typ_CVPOL) {
        printf(" -- CVPOL > 0\n");


        // // wenn CV am Endpunkt selektiert wurde, ist v0=v1 - nix dargestellt.
        // if(UTP_comp2db(((CurvPoly*)o2Dat)->v0,
                       // ((CurvPoly*)o2Dat)->v1, UT_TOL_pt)) {
          // // ja, v0 und v1 sind gleich;
          // // wenn (v0 != kvTab[0]):  v0 = kvTab[0];
          // // else v0 = kvTab[ptNr+deg+1];
          // if(UTP_comp2db(((CurvPoly*)o2Dat)->v0,
                         // ((CurvPoly*)o2Dat)->lvTab[0], UT_TOL_pt))
            // ((CurvPoly*)o2Dat)->v0 = ((CurvPoly*)o2Dat)->lvTab[0];
          // else {
            // i1 = ((CurvPoly*)o2Dat)->ptNr - 1;
            // ((CurvPoly*)o2Dat)->v0 = ((CurvPoly*)o2Dat)->lvTab[i1];
          // }
        // }
// 
// 
        // // wenn CV hinter dem Mittelpkt selektiert wurde, ist v0=v1 - umdrehen
        // i1 = ((CurvPoly*)o2Dat)->ptNr - 1;
        // d1 = ((CurvPoly*)o2Dat)->lvTab[i1] / 2.;
        // // printf(" v0=%f mitte=%f\n",((CurvPoly*)o2Dat)->v0,d1);
        // if(((CurvPoly*)o2Dat)->v0 > d1) {
          // ((CurvPoly*)o2Dat)->v1 = 0.;
        // }

        goto L_ex_POL;  // save polygon o2Dat

      }




    //----------------------------------------------------------------
    } else {
      TX_Error("AP_cont_nxt E001CloseContour");
      return -1;
    }

   




  //=====================================================================
  //========================== Point ====================================
  //=====================================================================
  // normal; old_typ new_typ.

  } else if (new_typ == Typ_PT) {

    // get pt -> o1Typ,o1Dat
    // (Point*)ppt = (void*)o1Dat;

    o1Siz = sizeof(Point);
    UTO_obj_getp (&op1, &i1, o1);
    memcpy(o1Dat, op1, o1Siz);
    o1Typ = Typ_PT;
    ppt = (void*)o1Dat;
    o1_clo = 1;
      // printf(" new PT ppt = %f %f %f\n",ppt->x,ppt->y,ppt->z);


    //----------------------------------------------------------------
    if(old_typ == Typ_Error) {
      // printf(" -- 0 > PT Start %f %f %f\n",ppt->x,ppt->y,ppt->z);
      // Punkt merken u raus.
      pto = *ppt;   // copy point
      *oo1i = oInd;
      goto L_ex_1P; // pto > oo1



    //----------------------------------------------------------------
    } else if(old_typ == Typ_PT) {
      // printf(" -- PT > PT\n");

      // skip Punkt, wenn schon da.
      if(UT3D_comp2pt(ppt, (Point*)o2Dat, UT_TOL_pt)) goto  L_exit;

      // Verbindungsline raus ((Point*)o2Dat -> ppt)
      ln1.p1 = *((Point*)o2Dat);
      ln1.p2 = *ppt;
      o2Ind = oInd;  // geht -> *oo1i
      goto L_ex_1L;  // ln1 > oo1




    //----------------------------------------------------------------
    } else if(old_typ == Typ_LN) {
      // printf(" -- LN > PT\n");

      // actPos auf ln projizieren
      UT3D_pt_projptln (&pt1, &d1, ppt, (Line*)o2Dat);
      // printf("pt1=%f %f %f\n",pt1.x,pt1.y,pt1.z);

      // sowieso schon am Endpunkt ?
      if(d1 < UT_TOL_pt) {
        // (Line*)o2Dat->p1 -> pt1 raus
        ln1.p1 = ((Line*)o2Dat)->p1;
        ln1.p2 = pt1;
        *oo1i = o2Ind;
        goto L_ex_1L;
      }

      // oo1 = Line ((Line*)o2Dat->p1 -> pt1)
      // oo2 = Line (pt1 -> ppt)
      ln1.p1 = ((Line*)o2Dat)->p1;
      ln1.p2 = pt1;
      ln2.p1 = pt1;
      ln2.p2 = *ppt;
      goto L_ex_2LL; // l1=ln1; l2=ln2.




    //----------------------------------------------------------------
    } else if(old_typ == Typ_CI) {
      // printf(" -- AC > PT\n");

      // proj PT -> CIR
      pci = (Circ*)o2Dat;
      UT3D_pt_traptptlen(&pto, &pci->pc, ppt, fabs(pci->rad));

      ci1 = *pci;
      UT3D_ci_cip2 (&ci1, &pto);
      // ci1.p2 = pto;

      // liegt Punkt schon am CIR ?
      // Ja: den Kreis ci1 raus, fertig.
      d1 = UT3D_len_2pt (&pto, ppt);
      if(d1 < UT_TOL_pt) goto L_ex_1C;

      // den Kreis ci1 raus, dann Line (pto -> ppt) raus, fertig.
      ln1.p1 = pto;
      ln1.p2 = *ppt;
      goto L_ex_2CL;





    //----------------------------------------------------------------
    } else if(old_typ == Typ_CV) {
      // printf(" -- CV > PT  %d\n",o2Typ);


      //.......................................................
      if(o2Typ == Typ_CVELL) {
        pell = (CurvElli*)o2Dat;   // pell > ppt
        UT3D_pt_projptel (&i1, pta, pell, ppt);
        APT_set_modMax (i1);
        pto = pta[0];

        pell->p2  = pto; // endpt elli

        // wenn pto != ppt: Inputobj speichern; raus mit PT > 0 !
        if(!UT3D_comp2pt(&pto, ppt, UT_TOL_pt)) {
          // printf(" save ppt\n");
          // *oo2 = *o1;
          // *oo2i = oInd;
          ln2.p1 = pto;
          ln2.p2 = *ppt;
          ConnLn = 1;
        }

        goto L_ex_ELL; // Ellipse o2Dat speichern und ausgeben



      //.......................................................
      } else if(o2Typ == Typ_CVBSP) {
          // printf(" -- CVBSP > PT  %d\n",o2Typ);
          // UT3D_stru_dump (Typ_PT, ppt, "ppt");  // this is the new point !
        i1 = 10; // size of pa, da
        irc = UT3D_pt_projptbspl (&i1, pta, da, (CurvBSpl*)o2Dat, ppt);
          // for(i2=0;i2<i1;++i2)
          // UT3D_stru_dump (Typ_PT, &pta[i2], "pta[%d]-v=%lf",i2,da[i2]);
        if(irc < 0) return irc;
        APT_set_modMax (i1);
        // i2 = UT3D_ipt_cknear_npt (ppt, pta, i1);
        // i2 = AP_cont_PTmod (ppt, pta, i1, imod);
        i2 = AP_cont_PTmod1 (imod);
        pto = pta[i2];
          // printf(" rc=%d par=%f pto=%f %f %f\n",irc,da[0],pto.x,pto.y,pto.z);
        ((CurvBSpl*)o2Dat)->v1 = da[i2];

        // Inputobj speichern
        if(!UT3D_comp2pt(&pto, ppt, UT_TOL_cv)) {
            // printf(" connLn -> \n");
          ln2.p1 = pto;
          ln2.p2 = *ppt;
          ConnLn = 1;
        }
        goto L_ex_CV;



      //.......................................................
      } else if(o2Typ == Typ_CVPOL) {
          printf(" -- Plg > PT\n");
        i1 = 10; // size of pa, da
        irc = UT3D_pt_projptplg (&i1, pta, da, o2Dat, ppt);
        if(irc < 0) return -1;
        APT_set_modMax (i1);
          // for(i2=0;i2<i1;++i2)UT3D_stru_dump(Typ_PT,&pta[i2]," %f",da[i2]);

        // i2 = UT3D_ipt_cknear_npt (ppt, pta, i1);
        // i2 = AP_cont_PTmod (ppt, pta, i1, imod);
        i2 = AP_cont_PTmod1 (imod);
        pto = pta[i2];
        // printf(" rc=%d par=%f pto=%f %f %f\n",irc,da[0],pto.x,pto.y,pto.z);
        ((CurvPoly*)o2Dat)->v1 = da[i2];

        // test if newPoint ppt = endPoint pto;
        //   yes: ?
        //   no:  create connectline

        // Inputobj speichern
        if(!UT3D_comp2pt(&pto, ppt, UT_TOL_pt)) {
          // no; create Line ..
          // *oo2 = *o1;
          // *oo2i = oInd;
          ln2.p1 = pto;
          ln2.p2 = *ppt;
          ConnLn = 1;         // add line after curve
        }
        // AP_cont_nxPT (oo2, ppt, ooDat);
        goto L_ex_POL;

      }




    //----------------------------------------------------------------
    } else {
      TX_Error("AP_cont_nxt E001P %d",o2Typ);
      return -1;
    }

   






  //=====================================================================
  //========================== Line ====================================
  //=====================================================================
  } else if (new_typ == Typ_LN) {

    // get line -> o1Typ,o1Dat
    // get start/endpoints of curve -> pto1a,pto1e;
    // (Line*)pln = (void*)o1Dat;
 
    o1Siz = sizeof(Line);
    UTO_obj_getp (&op1, &i1, o1);
    memcpy(o1Dat, op1, o1Siz);
    o1Typ = Typ_LN;
    pln = (void*)o1Dat;
    o1_clo = 1;


    // if(o1_sr < 0) MEM_swap__(&((Line*)o1Dat)->p1,
                             // &((Line*)o1Dat)->p2, sizeof(Point));


      // UT3D_stru_dump (Typ_LN, pln, "new LN:\n");
    pto1a = ((Line*)o1Dat)->p1;
    pto1e = ((Line*)o1Dat)->p2;

      // UT3D_stru_dump (Typ_LN, o1Dat, "new LN:\n");


    //----------------------------------------------------------------
    if(old_typ == Typ_Error) {
      // printf(" -- 0 > LN\n");
      goto L_ex_copy;  // nix tun
      // // Punkt merken u raus.
      // pto = pln->p1;
      // *oo1i = oInd;
      // goto L_ex_1P;



    //----------------------------------------------------------------
    } else if(old_typ == Typ_PT) {
      // printf(" -- PT > LN\n");

      // UT3D_stru_dump (Typ_PT, o2Dat, " old PT = ");

      // oldObj = actPos ist (Point*)o2Dat;
      // newObj = (Line*)o1Dat (pto1a-pto1e)

      // nur wenn PT erstes Konturelement war:
      // if(contNr == 2) {   // old = 1, new = 2 !
        // wenn actPos = Endpunkt: Line umdrehen
        if(UT3D_len_2pt((Point*)o2Dat, &((Line*)o1Dat)->p2) < UT_TOL_pt)
           UT3D_ln_inv ((Line*)o1Dat);
      // }

      // actPos auf ln projizieren
      UT3D_pt_projptln (&pto, &d1, (Point*)o2Dat, (Line*)o1Dat);

      // Startpkt Line merken
      ((Line*)o1Dat)->p1 = pto;
      pto1a = pto;

      // sowieso schon am Startpunkt ?
      if(d1 < UT_TOL_pt) goto  L_ex_copy;

      // ausgeben Line (Point*)o2Dat -> pln->p1
      ln1.p1 = *((Point*)o2Dat);
      ln1.p2 = ((Line*)o1Dat)->p1;
      *oo1i = o2Ind;
      goto L_ex_1L;   // nur Connectline ln1




    //----------------------------------------------------------------
    } else if(old_typ == Typ_LN) {
      // printf(" -- LN > LN\n");

      pl2 = (void*)o2Dat;
      ln1 = *pl2;               // eine Kopie der Line !

      // UT3D_stru_dump (Typ_LN, pl2, "old LN=\n");
      // UT3D_stru_dump (Typ_LN, pln, "new LN=\n");



      // stimmen die Endpunkte ueberein ?
      i1 = UT3D_comp4pt(&pl2->p1,&pl2->p2,&pln->p1,&pln->p2,UT_TOL_pt);
      // printf("nach UT3D_comp4pt %d\n",i1);
      // rc=1: beide El. liegen richtig.
      if(i1 == 1) goto L_ex_1L;

      // rc=2: Obj 1 verkehrt
      // darf nur passieren, wenn AC das allererste Obj ist (o4.typ == 0)
      if(i1 == 2) {
        UT3D_ln_inv (&ln1);
        goto L_ex_1L;
      }
      // rc=3: Obj 2 verkehrt
      if(i1 == 3) {
        UT3D_ln_inv (pln);
        goto L_ex_1L;
      }
      // rc=4: Obj 1 und Obj 2 verkehrt
      // darf nur passieren, wenn AC das allererste Obj ist (o4.typ == 0)
      if(i1 == 4) {
        UT3D_ln_inv (&ln1);
        UT3D_ln_inv (pln);
        goto L_ex_1L;
      }




      // Schnittpunkt
      UT3D_pt_int2ln (&pto, &pt1, NULL, (Line*)o2Dat, pln);


      // // alte LN umdrehen nur wenn es das erste Element waere !!
      // if(UT3D_len_2pt(&pto,&((Line*)o2Dat)->p1) < UT_TOL_pt) {
         // printf(" oldLN umdrehen\n");
         // UT3D_ln_inv ((Line*)o2Dat);
      // }


      // wenn pto = Endpunkt: Line umdrehen
      if(UT3D_len_2pt (&pto, &pln->p2) < UT_TOL_pt) pln->p2 = pln->p1;

      // Startpkt merken
      pln->p1 = pto;

      // sowieso schon auf der Line ?
      d1 = UT3D_len_2pt (&((Line*)o2Dat)->p1, &pto);
      if(d1 < UT_TOL_pt) goto  L_ex_copy;

      // ausgeben Line (Point*)o2Dat -> pln->p1
      ln1.p1 = ((Line*)o2Dat)->p1;
      ln1.p2 = pto;
      *oo1i = o2Ind;
      goto L_ex_1L;




    //----------------------------------------------------------------
    } else if(old_typ == Typ_CI) {
      // printf("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC \n");
      // printf(" -- AC > LN mod=%d\n",modFlg);

      i1 = AP_cont_Conn__();
      // Verbindg OK; save oldObj o2Dat.
      if(i1 == 0) goto L_ex_AC12;
      // keine Verbindg; intersect.

      // intersect
      UT3D_pt_intlnci__ (&i1, pta, (Line*)o1Dat, (Circ*)o2Dat);
      APT_set_modMax (i1);
        // printf(" int i1=%d modFlg=%d imod=%d\n",i1,modFlg,imod);

      // select intersectionpoint
      i1 = AP_cont_intPt (i1, pta);
      if(i1 < 0) return -1;              // unrecov. error
      goto L_ex_AC12;



      // pci = (Circ*)o2Dat;
      // ci1 = *pci;
      // // UT3D_stru_dump (Typ_CI, pci, "old AC =\n");
// 
// 
      // // stimmen die Endpunkte ueberein ?
      // // wenn pto = Endpunkt: LN umdrehen
      // // if(UT3D_len_2pt(&pci->p2, &pln->p2) < UT_TOL_pt) UT3D_ln_inv (pln);
      // i1 = UT3D_comp4pt(&pci->p1,&pci->p2,&pln->p1,&pln->p2,UT_TOL_pt);
      // // printf("nach UT3D_comp4pt %d\n",i1);
      // // rc=1: bei El. liegen richtig.
      // if(i1 == 1) goto L_ex_1C;
      // // rc=2: Obj 1 verkehrt
      // // darf nur passieren, wenn AC das allererste Obj ist (o4.typ == 0)
      // if(i1 == 2) {
        // UT3D_ci_inv1 (&ci1);
        // goto L_ex_1C;
      // }
      // // rc=3: Obj 2 verkehrt
      // if(i1 == 3) {
        // UT3D_ln_inv (pln);
        // goto L_ex_1C;
      // }
      // // rc=4: Obj 1 und Obj 2 verkehrt
      // // darf nur passieren, wenn AC das allererste Obj ist (o4.typ == 0)
      // if(i1 == 4) {
        // UT3D_ln_inv (pln);
        // UT3D_ci_inv1 (&ci1);
        // goto L_ex_1C;
      // }
// 
// 
// 
      // // intersect
      // UT3D_pt_intlnci__ (&i1, pta, pln, &ci1);
      // printf(" int i1=%d modFlg=%d imod=%d\n",i1,modFlg,imod);
      // if(i1 < 1) {                 // keine Loesung
        // return -1;
// 
      // } else if(i1 < 2) {          // touch; 1 point
        // i2 = 0;
// 
// 
      // } else {                     // 2 points
// 
        // if(modFlg == 1) {
          // i2 = imod;
// 
        // } else {
          // // printf("actPos=%f,%f,%f\n",pci->p1.x,pci->p1.y,pci->p1.z);
          // // wenn ein Schnittpunkt ident mit dem alten Endpt: OK
          // i2 = UT3D_pt_ck_npt (&ci1.p2,pta,i1,UT_TOL_pt);
          // if(i2 >= 0) goto L_AC_LN_9;
// 
          // // den dem alten Startpunkt naeheren Schnittpt waehlen
          // // jedoch NICHT wenn ident mit dem alten Startpunkt
          // i2 = AP_cont_nearPt (&ci1.p1, pta, i1);
// 
        // }
      // }
// 
      // L_AC_LN_9:
      // // UT3D_stru_dump (Typ_PT, &pto, " ci1.p2=");
      // UT3D_ci_cip2 (&ci1, &pta[i2]); // Endpunkt am alten Cir
// 
      // // wenn (pto == ln.p2) Line vertauschen
      // if(UT3D_comp2pt(&ci1.p2, &pln->p2, UT_TOL_pt)) {
        // // printf("AC > LN-vertauschen\n");
        // UT3D_ln_inv (pln);
      // }
      // else pln->p1 = ci1.p2; // Startpunkt der LN merken
      // // UT3D_stru_dump (Typ_CI, &ci1, "out AC =");
      // // UT3D_stru_dump (Typ_LN,  pln, "new LN =");
      // goto L_ex_1C;     // den Circ ci1 speichern, fertig.






    //----------------------------------------------------------------
    } else if(old_typ == Typ_CV) {
      // printf(" -- CV > LN    typ=%d\n",o2Typ);


      //.......................................................
      if(o2Typ == Typ_CVELL) {
        // printf(" -- CVELL > LN mod=%d\n",modFlg);
        pell = (void*)o2Dat;


        i1 = UT3D_comp4pt (&pell->p1, &pell->p2, &pln->p1, &pln->p2, UT_TOL_cv);
        // printf("nach UT3D_comp4pt %d\n",i1);

        // rc=1: beide El. liegen richtig.
        if(i1 == 1) goto L_ex_ELL;

        // rc=2: Obj 1 verkehrt
        if(i1 == 2) {
          UT3D_el_inv1 (pell);
          goto L_ex_ELL;
        }
        // rc=3: Obj 2 verkehrt; O1 umdrehen.
        if(i1 == 3) {
          UT3D_ln_inv (pln);
          goto L_ex_ELL;
        }
        // rc=4: Obj 1 und Obj 2 verkehrt
        if(i1 == 4) {
          UT3D_el_inv1 (pell);
          UT3D_ln_inv (pln);
          goto L_ex_ELL;
        }


        UT3D_pt_intlnel__ (&i1, pta, pln, pell);
        APT_set_modMax (i1);

        // wenn pell.p1 ist ident mit pta: den anderen Punkt nehmen
        // pto = pta[AP_cont_nearPt(&pell->p1, pta, i1)];
        pto = pta[AP_cont_PTmod(&pell->p1, pta, i1, imod)];

        pell->p2 = pto;  // endpt elli
        pln->p1  = pto;  // startpt ln
        goto L_ex_ELL;   // Ellipse pell speichern und ausgeben



      //.......................................................
      } else if(o2Typ == Typ_CVCLOT) {
        printf(" -- CVCLOT > LN mod=%d\n",modFlg);

        // Startpunkt/Endpkt des oldObj
        UTO_2pt_limstru (&pto2a, &pto2e, NULL, NULL, o2Typ, o2Dat);

        // Startpunkt/Endpkt des newObj schon fertig ..

        // geringsten Abstand von 4 Punkten suchen
        ConnTyp = UT3D_minLen_4pt (&d1, &pto2a, &pto2e, &pto1a, &pto1e);
        if(d1 > UT_TOL_cv) goto L_noInt;
          // printf(" d1=%f ConnTyp=%d\n",d1,ConnTyp);

        // invert lin.curves in structs according to ConnTyp
        AP_cont_inv (ConnTyp);
        goto L_ex_CLOT;           // LN o2Dat u ln2 raus



      //.......................................................
      } else if(o2Typ == Typ_CVBSP) {

        // printf(" -- CV-BSP > LN mod=%d\n",modFlg);
        // UT3D_stru_dump (Typ_CVBSP, o2Dat, "  old CV:\n");
        // UT3D_stru_dump (Typ_LN,    pln,   "  new LN:\n");


        // Startpunkt/Endpkt der CV
        UT3D_pt_evalparCv (&pt1, o2Dat, ((CurvBSpl*)o2Dat)->v0);
        UT3D_pt_evalparCv (&pt2, o2Dat, ((CurvBSpl*)o2Dat)->v1);
        // UT3D_stru_dump (Typ_PT, &pt1, "  startPt old BSP: ");
        // UT3D_stru_dump (Typ_PT, &pt2, "  endPt   old BSP: ");


        // stimmen die Endpunkte ueberein ?
        i1 = UT3D_comp4pt(&pt1, &pt2, &pln->p1, &pln->p2, UT_TOL_cv);
        // printf("nach UT3D_comp4pt %d\n",i1);

        // rc=1: beide El. liegen richtig.
        if(i1 == 1) {
          goto L_ex_CV;
        }

        // rc=2: Obj 1 verkehrt
        // darf nur passieren, wenn AC das allererste Obj ist (o4.typ == 0)
        if(i1 == 2) {
          MEM_swap__(&((CurvBSpl*)o2Dat)->v0,
                       &((CurvBSpl*)o2Dat)->v1, sizeof(double));
          goto L_ex_CV;
        }
        // rc=3: Obj 2 verkehrt
        if(i1 == 3) {
          UT3D_ln_inv ((Line*)o1Dat);
          goto L_ex_CV;
        }
        // rc=4: Obj 1 und Obj 2 verkehrt
        // darf nur passieren, wenn AC das allererste Obj ist (o4.typ == 0)
        if(i1 == 4) {
          UT3D_ln_inv ((Line*)o1Dat);
          MEM_swap__(&((CurvBSpl*)o2Dat)->v0,
                       &((CurvBSpl*)o2Dat)->v1, sizeof(double));
          goto L_ex_CV;
        }


        // intersect ..
        i1 = 10; // size of pa, da
        UT3D_pt_intlnbspl (&i1, pta, ooDat, pln, o2Dat);
        // printf(" got %d pts\n",i1);

        // kein Schnittpunkt (Toleranzproblem): Zwischenelement einfuegen
        if(i1 < 1) {
          // pto = prj Endpt CV -> Line pln
          UT3D_pt_projptln (&pto, &d1, &pt2, pln);
          ln2.p1 = pt2;   // Endpt Curve
          ln2.p2 = pto;
          ConnLn = 1;
          goto L_ex_CV; // Bspl o2Dat u ln2 raus
        }
        APT_set_modMax (i1);




        // printf("Fehler AP_cont_nxt CV>LN001\n");
        if(i1 < 2) {          // touch; 1 point
          i2 = 0;


        } else {                     // 2 points

          if(modFlg >= 0) {
            i2 = imod;

          } else {
            // printf("actPos=%f,%f,%f\n",pci->p1.x,pci->p1.y,pci->p1.z);
            // wenn ein Schnittpunkt ident mit dem alten Endpt: OK
            i2 = UT3D_pt_ck_npt (&pt2, pta, i1, UT_TOL_pt);
            if(i2 < 0) 
            // den dem alten Startpunkt naeheren Schnittpt waehlen
            // jedoch NICHT wenn ident mit dem alten Startpunkt
            i2 = AP_cont_nearPt (&pt1, pta, i1);
          }
        }

        pto = pta[i2];

        // den Parameter zum Punkt pto holen
        UT3D_parCv_bsplpt (da, &d1, o2Dat, &pto);
        // printf(" par=%f pto=%f %f %f\n",da[0],pto.x,pto.y,pto.z);
        ((CurvBSpl*)o2Dat)->v1 = da[0];  // Endpt bspl

        pln->p1 = pto;  // Startpt new LN

        goto L_ex_CV;




      //.......................................................
      } else if(o2Typ == Typ_CVRBSP) {           // RBSP > LN
        // printf(" -- RBSP > LN \n");

        i1 = AP_cont_Conn__();
        if(i1 == 0) goto L_ex_RBSP;
        if(i1 == 1) { TX_Error("AP_cont_inv E_RBSP_LN_int"); return -1;}



      //.......................................................
      } else if(o2Typ == Typ_CVPOL) {           // POL > LN
          printf(" POL > LN\n");
        i1 = 10; // size of pa, da
        UT3D_pt_intlnplg (&i1, pta, da, pln, o2Dat);
        APT_set_modMax (i1);
        // i2 = UT3D_ipt_cknear_npt (ppt, pta, i1);
        // pto2a = Startpunkt der CV
        UT3D_pt_evalplg (&pto2a, o2Dat, ((CurvPoly*)o2Dat)->v0);
        // den Index holen; Punkt muss != pto2a(Startpkt plg) sein
        i2 = AP_cont_PTmod (&pto2a, pta, i1, imod);
        pto = pta[i2];
        // printf(" rc=%d par=%f pto=%f %f %f\n",irc,da[0],pto.x,pto.y,pto.z);
        ((CurvPoly*)o2Dat)->v1 = da[i2];  // Endpt plg
        pln->p1 = pto;  // Startpt LN

        goto L_ex_POL;



      }






    //----------------------------------------------------------------
    } else {
      TX_Error("AP_cont_nxt E001P");
      return -1;
    }









  //=====================================================================
  //========================== Circle ====================================
  //=====================================================================
  } else if (new_typ == Typ_CI) {

    // get curve -> o1Typ,o1Dat
    // get start/endpoints of curve -> pto1a,pto1e;
    // (Circ*)pci = (void*)o1Dat;

    // copy o1 -> o1Dat = pci
    o1Siz = sizeof(Circ);
    UTO_obj_getp (&op1, &i1, o1);
    memcpy(o1Dat, op1, o1Siz);
    o1Typ = Typ_CI;
    pci = (void*)o1Dat;

    // fix o1Dat with o1_sr
    if(o1_sr < 0) pci->rad = -(fabs(pci->rad));
    if(o1_sr > 0) pci->rad = (fabs(pci->rad));

    // set o1_clo
    o1_clo = UT3D_ck_ci360 (pci);

      // UT3D_stru_dump (Typ_CI, o1Dat, "new AC:\n");
    pto1a = ((Circ*)o1Dat)->p1;
    pto1e = ((Circ*)o1Dat)->p2;



    // // raus 2004-07-30 weil findet Schnittpunkt LN > AC nicht findet !!
    // // wenn Vollkreis: halbieren 
    // if(UT3D_comp2pt(&pci->p1,&pci->p2,UT_TOL_pt)) {
      // // p2 = p1 visavis pc
      // UT3D_pt_opp2pt (&pci->p2,&pci->pc,&pci->p1);
    // }

    // UT3D_stru_dump (Typ_CI, pci, " .. new AC =\n");



    //----------------------------------------------------------------
    if(old_typ == Typ_Error) {
      // printf(" -- 0 > AC\n");
      goto L_ex_copy;  // copy o1 -> o2



    //----------------------------------------------------------------
    } else if(old_typ == Typ_PT) {
      // printf(" -- PT > AC\n");
        // oldObj = actPos ist (Point*)o2Dat;
        // newObj = (Circ*)o1Dat (pto1a-pto1e)

      // proj PT -> CIR
      UT3D_pt_traptptlen(&pto, &pci->pc, (Point*)o2Dat, fabs(pci->rad));

      // Vollkreis: p1 und p2 aendern
      if(UT3D_ck_ci360 ((Circ*)o1Dat) == YES) {
        // UT3D_ci_cipt180 ((Circ*)o1Dat, &pto); // halbieren
        UT3D_ci_cipt360 ((Circ*)o1Dat, &pto);

      } else {
        // trimmed circle;
        // if pto==endpoint revert circle, else change stratpoint
        if(UT3D_len_2pt((Point*)o2Dat, &((Circ*)o1Dat)->p2) < UT_TOL_pt) {
          UT3D_ci_inv1 ((Circ*)o1Dat);
        } else {
          UT3D_ci_cip1 ((Circ*)o1Dat, &pto);  // pto als Startpunkt am Cir merken
        }
      }

      pto1a = ((Circ*)o1Dat)->p1;
      pto1e = ((Circ*)o1Dat)->p2;
        // UT3D_stru_dump (Typ_PT, &pto1a, " pto1a AC\n");
        // UT3D_stru_dump (Typ_PT, &pto1e, " pto1e AC\n");

      // sowieso schon am Kreis ?
      d1 = UT3D_len_2pt (&((Circ*)o1Dat)->p1, (Point*)o2Dat);
      if(d1 < UT_TOL_pt) goto L_ex_copy;

      // Line (Point*)o2Dat > pto raus
      ln1.p1 = *((Point*)o2Dat);
      ln1.p2 = pto;
      *oo1i = o2Ind;
      goto L_ex_1L;




    //----------------------------------------------------------------
    } else if(old_typ == Typ_LN) {
      // printf(" -- LN > AC mod=%d\n",modFlg);

      i1 = AP_cont_Conn__();
      // Verbindg OK; save oldObj o2Dat.
      if(i1 == 0) goto L_ex_LN12;
      // keine Verbindg; intersect.

      // intersect
      UT3D_pt_intlnci__ (&i1, pta, (Line*)o2Dat, (Circ*)o1Dat);
      APT_set_modMax (i1);
        // printf(" int i1=%d modFlg=%d imod=%d\n",i1,modFlg,imod);

      // select intersectionpoint
      i1 = AP_cont_intPt (i1, pta);
      if(i1 < 0) return -1;              // unrecov. error
      goto L_ex_LN12;                    // (Line*)o2Dat ausgeben



      // pln   = (void*)o2Dat;
      // ln1 = *pln;               // eine Kopie der Line !
// 
      // // UT3D_stru_dump (Typ_LN, pln, "old LN=\n");
      // // UT3D_stru_dump (Typ_CI, pci, "new AC =\n");
// 
      // // wenn pto = Endpunkt: Circ umdrehen
      // // if(UT3D_len_2pt(&pln->p2, &pci->p2) < UT_TOL_pt) UT3D_ci_inv1 (pci);
// 
      // // stimmen die Endpunkte ueberein ?
      // // wenn pto = Endpunkt: LN umdrehen
      // // if(UT3D_len_2pt(&pci->p2, &pln->p2) < UT_TOL_pt) UT3D_ln_inv (pln);
      // i1 = UT3D_comp4pt(&ln1.p1,&ln1.p2,&pci->p1,&pci->p2,UT_TOL_pt);
      // // printf("nach UT3D_comp4pt %d\n",i1);
      // // rc=1: beide El. liegen richtig.
      // if(i1 == 1) goto L_ex_1L;
      // // rc=2: Obj 1 verkehrt
      // if(i1 == 2) {
        // UT3D_ln_inv (&ln1);
        // goto L_ex_1L;
      // }
      // // rc=3: Obj 2 verkehrt
      // if(i1 == 3) {
        // UT3D_ci_inv1 (pci);
        // goto L_ex_1L;
      // }
      // // rc=4: Obj 1 und Obj 2 verkehrt
      // if(i1 == 4) {
        // UT3D_ln_inv (&ln1);
        // UT3D_ci_inv1 (pci);
        // goto L_ex_1L;
      // }
// 
// 
// 
// 
      // // intersect
      // UT3D_pt_intlnci__ (&i1, pta, pln, pci);
      // // printf(" int i1=%d modFlg=%d\n",i1,modFlg);
      // if(i1 < 1) {                 // keine Loesung
        // return -1;
// 
      // } else if(i1 < 2) {          // touch
        // pto = pta[0];
// 
      // } else {
        // // den naeheren Punkt auswaehlen
        // if(modFlg >= 0) i2 = imod;
        // else i2 = AP_cont_nearPt (&pln->p1, pta, i1);
        // // else i2 = UT3D_ipt_cknear_npt (&pln->p1, pta, i1);
        // pto = pta[i2];
      // }
// 
      // // wenn pto = Endpunkt: Circ umdrehen
      // // if(UT3D_len_2pt(&pto, &pci->p2) < UT_TOL_pt) UT3D_ci_inv1 (pci);
// 
      // // if(dreh < 0) {
        // // pci->p2 = pci->p1;
      // // }
// 
      // L_LN_AC_9:
// 
      // // Vollkreis: p1 und p2 aendern
      // if(UT3D_ck_ci360 (pci) == YES) {
        // UT3D_ci_cipt180 (pci, &pto); // halbieren
      // } else {
        // UT3D_ci_cip1 (pci, &pto);  // pto als Startpunkt am Cir merken
      // }
// 
// 
      // // Ausg. Line pln.p1 -> pto;
      // ln1.p1 = pln->p1;
      // ln1.p2 = pto;
      // *oo1i = o2Ind;
      // goto L_ex_1L;





    //----------------------------------------------------------------
    } else if(old_typ == Typ_CI) {
      // printf(" -- AC > AC mod=%d\n",modFlg);

      i1 = AP_cont_Conn__();
      // Verbindg OK; save oldObj o2Dat.
      if(i1 == 0) goto L_ex_AC12;
      // keine Verbindg; intersect.

      // intersect
      i1 = UT3D_pt_intcici (pta, (Circ*)o1Dat, 1, (Circ*)o2Dat, 1);
      if(i1 < 1) return -1;              // unrecov. error
      // printf(" int i1=%d modFlg=%d imod=%d\n",i1,modFlg,imod);
      APT_set_modMax (i1);

      // select intersectionpoint
      i1 = AP_cont_intPt (i1, pta);
      if(i1 < 0) return -1;              // unrecov. error
      goto L_ex_AC12;


      // // old AC -> ci1
      // pc2   = (void*)o2Dat;
      // ci1 = *pc2;
      // // nun ist pci der neue, pc2/ci1 der alte CIR.
// 
      // // UT3D_stru_dump (Typ_CI, o2Dat, "old AC =\n"); // = pc2 = ci1
      // // UT3D_stru_dump (Typ_CI, o1Dat, "new AC =\n"); // = pci
// 
// 
      // // Test ob oldAC und newAC Verbindung haben
      // i1 = UT3D_comp4pt(&pc2->p1,&pc2->p2,&pci->p1,&pci->p2,UT_TOL_cv);
      // // printf("nach UT3D_comp4pt %d\n",i1);
      // // rc=1: beide El. liegen richtig.
      // if(i1 == 1) goto L_ex_1C;
// 
      // // rc=2: Obj 1 verkehrt
      // // darf nur passieren, wenn AC das allererste Obj ist (o4.typ == 0)
      // if(i1 == 2) {
        // UT3D_ci_inv1 (&ci1);
        // goto L_ex_1C;
      // }
// 
      // // rc=3: Obj 2 verkehrt
      // if(i1 == 3) {
        // UT3D_ci_inv1 (pci);
        // goto L_ex_1C;
      // }
// 
      // // rc=4: Obj 1 und Obj 2 verkehrt
      // // darf nur passieren, wenn AC das allererste Obj ist (o4.typ == 0)
      // if(i1 == 4) {
        // UT3D_ci_inv1 (&ci1);
        // UT3D_ci_inv1 (pci);
        // goto L_ex_1C;
      // }
// 
// 
      // // Schnitt CIR - CIR gives 0/1/2 points
      UT3D_pt_intcici (&i1, pta, pci, &ci1);
        // UT3D_stru_dump (Typ_PT, &pta[0], " i1=%d intpt[0]=",i1);
        // UT3D_stru_dump (Typ_PT, &pta[1], " intpt[1]=");
        // GR_Disp_pTab (i1, pta, SYM_STAR_S, 2);


      if(i1 < 1) {                 // keine Loesung. Verbindungslinie ?
        // printf("AP_cont_nxt E_AC_AC\n");
        // endPt old Circ
        ln1.p1 = ci1.p2;
        // project p1 -> new Circ
        // UT3D_pt_projptci (&ln1.p2, &ln1.p1, pci);
        // goto L_ex_2CL; // ci1 u ln1 raus
      // }
// 
// 
      // if(i1 < 2) {                 // 1 PT = touch
        // i2 = 0;
// 
      // } else {   // i1 = 2
// 
        // if(modFlg >= 0) {
          // i2 = imod;
// 
        // } else {
          // // muessen auswaehlen ..
          // // wenn eine Schnittpunkt ident mit dem alten Endpt: OK
          // if(UT3D_pt_ck_npt (&ci1.p2,pta,i1,UT_TOL_pt) >= 0) goto L_AC_AC_9;
// 
          // // den dem alten Startpunkt naeheren Schnittpt waehlen
          // // jedoch NICHT wenn ident mit dem alten Startpunkt
          // i2 = AP_cont_nearPt (&ci1.p1, pta, i1);
        // }
      // }
    //  
      // // endpunkt alter Kreis
      // UT3D_ci_cip2 (&ci1, &pta[i2]);
        // // GR_Disp_pt (&pta[i2], SYM_TRI_S, 2);
// 
// 
      // L_AC_AC_9:
      // // Neuer kreis: Vollkreis: Startpunkt/Endpunkt aendern;
      // if(UT3D_ck_ci360 (pci) == YES) UT3D_ci_cipt180 (pci, &ci1.p2);
      // else UT3D_ci_cip1 (pci, &ci1.p2);  // Startpunkt am neuen Cir merken
      // // UT3D_stru_dump (Typ_CI, pci, " new Circ korr. =\n");
// 
      // // den Kreis ci1 raus.
      // goto L_ex_1C;





    //----------------------------------------------------------------
    } else if(old_typ == Typ_CV) {
      // printf(" -- CV > AC %d\n",o2Typ);


      //.......................................................
      if(o2Typ == Typ_CVBSP) {
      // printf(" -- BSP > AC %d\n",o2Typ);

        // UT3D_stru_dump (Typ_CVBSP, o2Dat, "  old obj:\n");
        // UT3D_stru_dump (Typ_CI,    o1Dat, "  new obj:\n");

        // Startpunkt/Endpkt des oldObj
        UTO_2pt_limstru (&pto2a, &pto2e, NULL, NULL, o2Typ, o2Dat);

        // geringsten Abstand von 4 Punkten suchen
        ConnTyp = UT3D_minLen_4pt (&d1, &pto2a, &pto2e, &pci->p1,&pci->p2);
        if(d1 > UT_TOL_cv) goto L_BSP_AC_int;  // goto intersect

        L_BSP_AC_1:
        // invert lin.curves in structs according to ConnTyp
        AP_cont_inv (ConnTyp);
        goto L_ex_CV; // LN o2Dat u ln2 raus


        L_BSP_AC_int:
        i1 = 10;
        UT3D_pt_intcibspl (&i1, pta, pci, o2Dat, ooDat);
        APT_set_modMax (i1);


        // kein Schnittpunkt: add connecting-line ln2
        if(i1 < 1) {
          ConnLn = 1;
          // give ConnectionLine according to ConnTyp
          AP_cont_ConnLn (&ln2, ConnTyp, &pto1a, &pto1e, &pto2a, &pto2e);
          goto L_BSP_AC_1;
        }

// 
        // UT3D_pt_evalparCv (&pt1, o2Dat, ((CurvBSpl*)o2Dat)->v0);
        // UT3D_pt_evalparCv (&pt2, o2Dat, ((CurvBSpl*)o1Dat)->v1);
// 
        // // stimmen die Endpunkte ueberein ?
        // i1 = UT3D_comp4pt(&pt1,&pt2,&pci->p1,&pci->p2,UT_TOL_cv);
        // // printf("nach UT3D_comp4pt %d\n",i1);
// 
        // // rc=1: beide El. liegen richtig.
        // if(i1 == 1) goto L_C_A_1;
// 
        // // rc=2: Obj 1 verkehrt
        // if(i1 == 2) {
          // MEM_swap__(&((CurvBSpl*)o2Dat)->v0,
                     // &((CurvBSpl*)o2Dat)->v1, sizeof(double));
          // goto L_C_A_1;
        // }
// 
        // // rc=3: Obj 2 verkehrt: CV umdrehen
        // if(i1 == 3) {
          // UT3D_ci_inv1 (pci);
          // goto L_C_A_1;
        // }
// 
        // // rc=4: Obj 1 und Obj 2 verkehrt
        // if(i1 == 4) {
          // MEM_swap__(&((CurvBSpl*)o2Dat)->v0,
                     // &((CurvBSpl*)o2Dat)->v1, sizeof(double));
          // UT3D_ci_inv1 (pci);
          // goto L_C_A_1;
        // }
// 
// 
        // // Schnitt bspl - ci
        // i1 = 10;
        // UT3D_pt_intcibspl (&i1, pta, pci, o2Dat, ooDat);
        // // printf(" n.Inters. %d\n",i1);
// 
        // // kein Schnittpunkt: zusaetzl. Verbindungslinie erzeugen
        // if(i1 < 1) {
          // // CiStart oder CiEnd ?
          // ln2.p1 = pt1;
          // if(UT3D_len_2pt(&pci->p1, &pt1) < UT3D_len_2pt(&pci->p2, &pt2)) {
            // ln2.p2 = pci->p1;
          // } else {
            // ln2.p2 = pci->p2;
          // }
          // ConnLn = 1;
          // goto L_ex_CV; // ln1 u ln2 raus
        // }



        // pto2a = Startpunkt der CV
        UT3D_pt_evalparCv (&pto2a, o2Dat, ((CurvBSpl*)o2Dat)->v0);

        // den Index holen; Punkt muss != pto2a(Startpkt plg) sein
        i2 = AP_cont_PTmod (&pto2a, pta, i1, imod);
        pto = pta[i2];

        UT3D_parCv_bsplpt (&da[i2], &d1, o2Dat, &pto);
        ((CurvBSpl*)o2Dat)->v1 = da[i2];   // Endpt bspl

        // Startpunkt am neuen Cir merken
        if(UT3D_ck_ci360 (pci) == YES) UT3D_ci_cipt180 (pci, &pto);
        else UT3D_ci_cip1 (pci, &pto);


        // Curve raus, fertig.
        L_C_A_1:
        goto L_ex_CV;



      //.......................................................
      } else if(o2Typ == Typ_CVRBSP) {
        // printf(" -- RBSP > AC \n");

        i1 = AP_cont_Conn__();
        if(i1 == 0) goto L_ex_RBSP;
        if(i1 == 1) { TX_Error("AP_cont_inv E_RBSP_AC_int"); return -1;}




      //.......................................................
      } else if(o2Typ == Typ_CVPOL) {
          printf(" -- CVPOL > AC \n");
        // Startpunkt/Endpkt des oldObj
        UTO_2pt_limstru (&pto2a, &pto2e, NULL, NULL, o2Typ, o2Dat);

        // geringsten Abstand von 4 Punkten suchen
        ConnTyp = UT3D_minLen_4pt (&d1, &pto2a, &pto2e, &pci->p1,&pci->p2);
        if(d1 > UT_TOL_cv) goto L_PLG_AC_int;  // goto intersect

        L_PLG_AC_1:
        // invert lin.curves in structs according to ConnTyp
        AP_cont_inv (ConnTyp);
        goto L_ex_POL; // LN o2Dat u ln2 raus


        L_PLG_AC_int:
        i1 = 10; // size of pa, da
        irc = UT3D_pt_intciplg (&i1, pta, da, pci, o2Dat);
        APT_set_modMax (i1);

        // // pto2a = Startpunkt der CV
        // UT3D_pt_evalplg (&pto2a, o2Dat, ((CurvPoly*)o2Dat)->v0);

        // den Index holen; Punkt muss != pto2a(Startpkt plg) sein
        i2 = AP_cont_PTmod (&pto2a, pta, i1, imod);

        // pto2a = Startpunkt der CV
        UT3D_pt_evalparCv (&pto2a, o2Dat, ((CurvBSpl*)o2Dat)->v0);
        pto = pta[i2];

        // printf(" rc=%d par=%f pto=%f %f %f\n",irc,da[0],pto.x,pto.y,pto.z);
        ((CurvPoly*)o2Dat)->v1 = da[i2];   // Endpt plg

        // Startpunkt am neuen Cir merken
        if(UT3D_ck_ci360 (pci) == YES) UT3D_ci_cipt180 (pci, &pto);
        else UT3D_ci_cip1 (pci, &pto);

        goto L_ex_POL;



      //.......................................................
      } else if(o2Typ == Typ_CVELL) {
        // printf(" -- CVELL > AC \n");

        i1 = AP_cont_Conn__();
        // Verbindg OK; save oldObj o2Dat.
        if(i1 == 0) goto L_ex_ELL;
        // keine Verbindg; intersect.
        UT3D_pt_intciel (&i1, pta, (Circ*)o1Dat, (CurvElli*)o2Dat, ooDat);
        APT_set_modMax (i1);
  
        // select intersectionpoint
        i1 = AP_cont_intPt (i1, pta);
        if(i1 < 0) return -1;              // unrecov. error
        goto L_ex_ELL;



      //.......................................................
      } else if(o2Typ == Typ_CVCLOT) {
        printf(" -- CVCLOT > AC mod=%d\n",modFlg);

        // Startpunkt/Endpkt des oldObj
        UTO_2pt_limstru (&pto2a, &pto2e, NULL, NULL, o2Typ, o2Dat);

        // Startpunkt/Endpkt des newObj schon fertig ..

        // geringsten Abstand von 4 Punkten suchen
        ConnTyp = UT3D_minLen_4pt (&d1, &pto2a, &pto2e, &pto1a, &pto1e);
        if(d1 > UT_TOL_cv) goto L_noInt;
          // printf(" d1=%f ConnTyp=%d\n",d1,ConnTyp);

        // invert lin.curves in structs according to ConnTyp
        AP_cont_inv (ConnTyp);
        goto L_ex_CLOT;           // LN o2Dat u ln2 raus


      //.......................................................
      }





    //----------------------------------------------------------------
    } else {
      TX_Error("AP_cont_nxt E001P");
      return -1;
    }









  //=====================================================================
  //========================== Curve ====================================
  //=====================================================================
  } else if (new_typ == Typ_CV) {

    // get curve -> o1Typ,o1Dat
    // get start/endpoints of curve -> pto1a,pto1e;
    // get parameters for start/endpoints -> vo1a,vo1e;

    pox = DB_GetCurv ((long)o1->data);
      printf(" new curv typ=%d old_typ=%d\n",pox->typ,old_typ);
      UTO_dump__ (pox, "newCrv:");

    new_typ = pox->typ;
    o1Typ   = pox->typ;


    //---------------------------------
    if(pox->typ == Typ_CVELL) {
      o1Siz = sizeof(CurvElli);
      memcpy (o1Dat, pox->data, o1Siz);
      pell  = (void*)o1Dat;
      if(o1_sr < 0) pell->dir = -1;
      if(o1_sr > 0) pell->dir = 1;
      pto1a = ((CurvElli*)o1Dat)->p1;
      pto1e = ((CurvElli*)o1Dat)->p2;
      // set o1_clo
      o1_clo = UT3D_ck_el360 (pell);


    //---------------------------------
    } else if(pox->typ == Typ_CVCLOT) {
      printf(" AP_cont_nxt Typ_CVCLOT\n");
      o1Siz = sizeof(CurvClot);
      memcpy (o1Dat, pox->data, o1Siz);
      pto1a = ((CurvClot*)o1Dat)->stp; // see UTO_2pt_limstru
      UT3D_ptvc_evparclot (&pto1e, NULL, 0, o1Dat, 1.);
      o1_clo = 1;


    //---------------------------------
    } else if(pox->typ == Typ_CVBSP) {
      o1Siz = sizeof(CurvBSpl);
      memcpy (o1Dat, pox->data, o1Siz);
// only if closed ??
      if(o1_sr < 0) ((CurvRBSpl*)o1Dat)->dir = 1;
      if(o1_sr > 0) ((CurvRBSpl*)o1Dat)->dir = 0;
// 
      // if(dreh < 0) {
        // // curve umdrehen
        // // printf(" invert CurvBSpl\n");
        // MEM_swap__(&((CurvBSpl*)o1Dat)->v0,
                   // &((CurvBSpl*)o1Dat)->v1, sizeof(double));
      // }

      // printf("  v0=%f v1=%f\n",((CurvBSpl*)o1Dat)->v0,((CurvBSpl*)o1Dat)->v1);
      // TESTAUG nur:
      // UT3D_stru_dump (new_typ, o1Dat, "  new CV-%d:\n",new_typ);
      // UT3D_pt_evalparCv (&pt1, o1Dat, ((CurvBSpl*)o1Dat)->v0);
      // UT3D_stru_dump (Typ_PT, &pt1, "  startPt new BSP: ");
      // UT3D_pt_evalparCv (&pt2, o1Dat, ((CurvBSpl*)o1Dat)->v1);
      // UT3D_stru_dump (Typ_PT, &pt2, "  endPt   new BSP: ");
      // printf("  v0=%f v1=%f\n",((CurvBSpl*)o1Dat)->v0,((CurvBSpl*)o1Dat)->v1);
      UT3D_pt_evalparCv (&pto1a, o1Dat, ((CurvBSpl*)o1Dat)->v0);
      UT3D_pt_evalparCv (&pto1e, o1Dat, ((CurvBSpl*)o1Dat)->v1);
      // set o1_clo
      o1_clo = UT3D_bsp_ck_closed_tr ((CurvBSpl*)o1Dat);


    //---------------------------------
    } else if(pox->typ == Typ_CVRBSP) {
      o1Siz = sizeof(CurvRBSpl);
      memcpy (o1Dat, pox->data, o1Siz);
// only if closed ??
      if(o1_sr < 0) ((CurvRBSpl*)o1Dat)->dir = 1;
      if(o1_sr > 0) ((CurvRBSpl*)o1Dat)->dir = 0;
// 
      // if(dreh < 0) {
        // // curve umdrehen
        // // printf(" invert CurvBSpl\n");
        // MEM_swap__(&((CurvRBSpl*)o1Dat)->v0,
                   // &((CurvRBSpl*)o1Dat)->v1, sizeof(double));
      // }

      // printf("v0=%f v1=%f\n",((CurvRBSpl*)o1Dat)->v0,((CurvRBSpl*)o1Dat)->v1);

      // see UTO_2pt_limstru
      UT3D_pt_evparCrvRBSpl(&pto1a,&d1,(CurvRBSpl*)o1Dat,((CurvRBSpl*)o1Dat)->v0);
      UT3D_pt_evparCrvRBSpl(&pto1e,&d1,(CurvRBSpl*)o1Dat,((CurvRBSpl*)o1Dat)->v1);
// TODO: UT3D_rbsp_ck_closed
      o1_clo = 1;


    //---------------------------------
    } else if(pox->typ == Typ_CVPOL) {
      o1Siz = sizeof(CurvPoly);
      memcpy (o1Dat, pox->data, o1Siz); // copy struct
      // set pto1a, pto1e, vo1a, vo1e
      UTO_2pt_limstru (&pto1a, &pto1e, &vo1a, &vo1e, Typ_CVPOL, (CurvPoly*)o1Dat);
// only if closed ??
      if(o1_sr < 0) ((CurvPoly*)o1Dat)->dir = 1;
      if(o1_sr > 0) ((CurvPoly*)o1Dat)->dir = 0;
        // MEM_swap__(&((CurvPoly*)o1Dat)->v0,
                   // &((CurvPoly*)o1Dat)->v1, sizeof(double));
        // printf(" v0=%f v1=%f\n",((CurvPoly*)o1Dat)->v0,((CurvPoly*)o1Dat)->v1);
      // set o1_clo
      o1_clo = UT3D_ck_plgclo ((CurvPoly*)o1Dat);


    //---------------------------------
    } else if(pox->typ == Typ_CVTRM) {
        // printf("  copy new cv = ccv ..\n");
        // UTO_dump__ (pox, "ccv-in");
      o1Siz = sizeof(ObjGX);
      memcpy (o1Dat, pox, o1Siz);
// TODO: UT3D_ccv_ck_closed
      o1_clo = 1;


    //---------------------------------
    } else {
      TX_Error("AP_cont_nxt E001P");
      return -1;
    }




    //----------------------------------------------------------------
    if(old_typ == Typ_Error) {
      // printf(" -- 0 > CV\n");

      goto L_ex_copy;  // nix tun




    //----------------------------------------------------------------
    } else if(old_typ == Typ_PT) {
        printf(" -- PT > CV\n");

      // pto = *(Point*)o2Dat;
      ppt = (Point*)o2Dat;
        // UT3D_stru_dump (Typ_PT,    o2Dat, "  old obj:");


      //.......................................................
      if(new_typ == Typ_CVELL) {
        // printf(" -- PT > CVELL\n");

        UT3D_pt_projptel (&i1, pta, pell, ppt);
        pto = pta[0];
        pell->p1 = pto;
        pell->p2 = pto;     // 2014-04-27
        APT_set_modMax (i1);

        // wenn schon auf elli: skip Point
        if(UT3D_len_2pt(&pto, ppt) < UT_TOL_pt) goto L_ex_copy;

        // *oo1i = o2Ind;
        // goto L_ex_1P; // den Punkt pto speichern, fertig.

        // connectLine ppt -> pto
        ln1.p1 = *ppt;
        ln1.p2 = pto;
        goto L_ex_1L;



      //.......................................................
      } else if(new_typ == Typ_CVCLOT) {
          // printf(" -- PT > CLOT\n");

        // wenn schon auf pto1a: skip Point
        if(UT3D_len_2pt(ppt, &pto1a) < UT_TOL_pt) goto L_ex_copy;
        goto L_noInt;



      //.......................................................
      } else if(new_typ == Typ_CVBSP) {
        // printf(" -- PT > CVBSP\n");
        // UT3D_stru_dump (Typ_PT, ppt, "ppt");  // old point !
        // UT3D_stru_dump (Typ_PT, (Point*)&o2Dat, "o2Dat");  // old point !

        // wenn PT == CV-Endpunkt: CV umdrehen und raus.
        if(UT3D_len_2pt (ppt, &pto1e) < UT_TOL_cv) {
          MEM_swap__(&((CurvBSpl*)o1Dat)->v0,
                       &((CurvBSpl*)o1Dat)->v1, sizeof(double));
          MEM_swap__(&pto1a, &pto1e, sizeof(double));
          pto = *ppt;
          *oo1i = o2Ind;
          goto L_ex_1P;  // pto out
        }

        // wenn PT == CV-Startpunkt: raus.
        // UT3D_pt_evalparCv (&pt1, o1Dat, ((CurvBSpl*)o1Dat)->v0);
        if(UT3D_len_2pt (ppt, &pto1a) < UT_TOL_cv) {
          pto = *ppt;
          *oo1i = o2Ind;
          goto L_ex_1P;  // pto out
        }


        i1 = 10; // size of pa, da
        irc = UT3D_pt_projptbspl (&i1, pta, da,
                            (CurvBSpl*)o1Dat, (Point*)o2Dat); // Schnitt
        if(irc < 0) return irc;
        APT_set_modMax (i1);
          // for(i2=0;i2<i1;++i2)
          // UT3D_stru_dump (Typ_PT, &pta[i2], "pta[%d]-v=%lf",i2,da[i2]);

        // find nearest point to ppt in pta 2013-12-28
        i2 = UT3D_minLen_npt ((Point*)&o2Dat, pta, i1);
        // i2 = AP_cont_PTmod ((Point*)&o2Dat, pta, i1, imod);
        pto = pta[i2];
          // printf(" i2=%d par=%f pto=%f %f %f\n",i2,da[i2],pto.x,pto.y,pto.z);
          // printf(" par-old=%f\n",((CurvBSpl*)o1Dat)->v0);
        ((CurvBSpl*)o1Dat)->v0 = da[i2];

        // wenn schon auf bsp: skip Point
        // d1 = UT3D_len_2pt(&pto, (Point*)o1Dat);
        d1 = UT3D_len_2pt(&pto, ppt);
          // printf(" d1=%lf\n",d1);
        if(d1 < UT_TOL_cv) goto L_ex_copy;

        // *oo1i = o2Ind;
        // goto L_ex_1P; // den Punkt pto speichern, fertig.

        // connectLine ppt -> pto
          printf(" connLn!\n");
        ln1.p1 = *ppt;
        ln1.p2 = pto;
        goto L_ex_1L;




      //.......................................................
      } else if(new_typ == Typ_CVPOL) {
          // printf(" -- PT > CVPOL conn=%d\n",ConnLn);

        // wenn PT == CV-Startpunkt: raus.
        if(UT3D_comp2pt(&pto1a, ppt, UT_TOL_pt)) {
          goto L_ex_copy;  // nix tun
        } 
        // wenn PT == CV-Endpunkt: CV umdrehen und raus.
        if(UT3D_comp2pt(&pto1e, ppt, UT_TOL_pt)) {
          ((CurvPoly*)o2Dat)->v1 = vo1e;
          goto L_ex_copy;  // nix tun
        }
        // compute normal from pt ppt to plg o1Dat
        i1 = 10; // size of pa, da
        UT3D_pt_projptplg (&i1, pta, da, o1Dat, ppt);
        // printf("i1=%d\n",i1);
        APT_set_modMax (i1);


        // nearest point is in pta[0]
        i2 = 0;
        // i2 = AP_cont_PTmod ((Point*)&o2Dat, pta, i1, imod);
        pto = pta[i2];

        // printf(" par=%f pto=%f %f %f\n",da[0],pto.x,pto.y,pto.z);
        // printf(" par-old=%f\n",((CurvBSpl*)o1Dat)->v0);
        ((CurvPoly*)o1Dat)->v0 = da[i2];

        // wenn schon auf plg: skip Point
        if(UT3D_comp2pt(&pto, ppt, UT_TOL_pt)) goto L_ex_copy;
        // if(UT3D_len_2pt(&pto, (Point*)o1Dat) < UT_TOL_pt) goto L_ex_copy;

        // output connectLine ppt -> pto
        ln1.p1 = *ppt;
        ln1.p2 = pto;
        goto L_ex_1L;
      }




    //----------------------------------------------------------------
    } else if(old_typ == Typ_LN) {
      // printf(" -- LN > CV %d\n",new_typ);

      pln = (Line*)o2Dat;
        // UT3D_stru_dump (Typ_LN, pln, "  old LN: ");

      // Startpunkt/Endpkt des oldObj
      UTO_2pt_limstru (&pto2a, &pto2e, NULL, NULL, old_typ, o2Dat);


      //.......................................................
      if(new_typ == Typ_CVELL) {
        // printf(" -- LN > CVELL %d\n",new_typ);

        // Startpunkt/Endpkt des newObj
        UTO_2pt_limstru (&pto1a, &pto1e, NULL, NULL, new_typ, o1Dat);

        // geringsten Abstand von 4 Punkten suchen
        ConnTyp = UT3D_minLen_4pt (&d1, &pto2a, &pto2e, &pto1a, &pto1e);
        if(d1 > UT_TOL_cv) goto L_LN_ELL_int;  // goto intersect

        L_LN_ELL_1:
        // invert lin.curves in structs according to ConnTyp
        AP_cont_inv (ConnTyp);
        goto L_ex_LN12; // LN o2Dat u ln2 raus


        L_LN_ELL_int:
        UT3D_pt_intlnel__ (&i1, pta, pln, pell);
        APT_set_modMax (i1);

        // kein Schnittpunkt: add connecting-line ln2
        if(i1 < 1) {
          ConnLn = 1;
          // give ConnectionLine according to ConnTyp
          AP_cont_ConnLn (&ln2, ConnTyp, &pto1a, &pto1e, &pto2a, &pto2e);
          goto L_LN_ELL_1;
        }


        if(i1 < 1) {                 // keine Loesung
          return -1;

        } else if(i1 < 2) {          // touch
          pto = pta[0];

        } else {
          // den naeheren Punkt auswaehlen
          if(modFlg >= 0) i2 = imod;
          else {
            // wenn ein Schnittpunkt ident mit dem alten Endpt: OK
            i2 = UT3D_pt_ck_npt (&pln->p2,pta,i1,UT_TOL_pt);
            if(i2 < 0) {
              // den dem alten Startpunkt naeheren Schnittpt waehlen
              // jedoch NICHT wenn ident mit dem alten Startpunkt
              i2 = AP_cont_nearPt (&pln->p1, pta, i1);
            }
          }
          pto = pta[i2];
        }

        pln->p2  = pto;  // endpt old ln

        if(UT3D_ck_el360 (pell) == YES) {
          UT3D_el_elpt180 (pell, &pto); // new VollElli halbieren
        } else {
          pell->p1 = pto;  // startpt new elli
        }

        *oo1i = o2Ind;
        goto L_ex_1P; // den Punkt pto speichern, fertig.



      //.......................................................
      } else if(new_typ == Typ_CVCLOT) {
        printf(" -- LN > CVCLOT %d\n",new_typ);

        // Startpunkt/Endpkt des newObj schon gesetzt
        // Startpunkt/Endpkt des oldObj schon gesetzt

        // geringsten Abstand von 4 Punkten suchen
        ConnTyp = UT3D_minLen_4pt (&d1, &pto2a, &pto2e, &pto1a, &pto1e);
        if(d1 > UT_TOL_cv) goto L_noInt;
          printf(" d1=%f ConnTyp=%d\n",d1,ConnTyp);

        // invert lin.curves in structs according to ConnTyp
        AP_cont_inv (ConnTyp);
        goto L_ex_LN12; // LN o2Dat u ln2 raus



      //.......................................................
      } else if(new_typ == Typ_CVBSP) {
        // printf(" -- LN > CVBSP %d\n",new_typ);

        *oo1i = o2Ind;


        // Startpunkt/Endpkt der CV
        UTO_2pt_limstru (&pt1, &pt2, NULL, NULL, new_typ, o1Dat);
          // printf(" newCV.v0=%f v1=%f\n",((CurvBSpl*)o1Dat)->v0,
                                        // ((CurvBSpl*)o1Dat)->v1);
          // UT3D_stru_dump (Typ_CVBSP, o1Dat, "  new BSP:\0");
          // UT3D_stru_dump (Typ_PT, &pt1, "  startPt BSP: ");
          // UT3D_stru_dump (Typ_PT, &pt2, "  endPt   BSP: ");


        // stimmen die Endpunkte ueberein ?
        // i1 = UT3D_comp4pt(&pln->p1,&pln->p2,&pt1,&pt2,UT_TOL_cv);
        // printf("nach UT3D_comp4pt %d\n",i1);
        ConnTyp = UT3D_minLen_4pt (&d1, &pln->p1,&pln->p2,&pt1,&pt2);

        if(d1 > UT_TOL_cv) goto L_LN_BSP_int;  // goto intersect


        L_LN_BSP_1:
        // invert lin.curves in structs according to ConnTyp
        AP_cont_inv (ConnTyp);
        goto L_ex_LN12; // LN o1Dat u ln2 raus



        // Schnittpunkt errechnen
        L_LN_BSP_int:
        i1 = 10; // size of pa, da
        UT3D_pt_intlnbspl (&i1, pta, ooDat, pln, o1Dat); // inters.
        APT_set_modMax (i1);
          // printf(" n.Inters. %d\n",i1);
          // UT3D_stru_dump (Typ_PT, pta, "  Int.pta[0]");



        // kein Schnittpunkt: add connecting-line
        if(i1 < 1) {
          ConnLn = 1;
          // give ConnectionLine according to ConnTyp
          AP_cont_ConnLn (&ln2, ConnTyp, &pln->p1,&pln->p2,&pt1,&pt2);
          goto L_LN_BSP_1;
        }


        if(i1 < 2) {          // touch
          pto = pta[0];

        } else {
          // den naeheren Punkt auswaehlen
          if(modFlg >= 0) i2 = imod;
          else {
            // wenn ein Schnittpunkt ident mit dem alten Endpt: OK
            i2 = UT3D_pt_ck_npt (&pln->p2,pta,i1,UT_TOL_pt);
            if(i2 < 0) {
              // den dem alten Startpunkt naeheren Schnittpt waehlen
              // jedoch NICHT wenn ident mit dem alten Startpunkt
              i2 = AP_cont_nearPt (&pln->p1, pta, i1);
            }
          }
          pto = pta[i2];
        }


        // den Parameter zum Punkt pto holen
        UT3D_parCv_bsplpt (da, &d1, o1Dat, &pto);
          // printf(" par=%f pto=%f %f %f\n",da[0],pto.x,pto.y,pto.z);
        // wenn ptInt naeher am Endpunkt als am Startpunkt - umdrehen
        if(UTP_db_cknear_2db(da, &((CurvBSpl*)o1Dat)->v0,
                                 &((CurvBSpl*)o1Dat)->v1) == 1)
          MEM_swap_2db(&((CurvBSpl*)o1Dat)->v0,&((CurvBSpl*)o1Dat)->v1);
        ((CurvBSpl*)o1Dat)->v0 = da[0];
        *oo1i = o2Ind;

        ln1.p1 = pln->p1;
        ln1.p2 = pto;  // endpt old ln
          // UT3D_stru_dump (Typ_LN, &ln1, "  out old LN: ");
          // printf(" newCV.v0=%f v1=%f\n",((CurvBSpl*)o1Dat)->v0,
                                        // ((CurvBSpl*)o1Dat)->v1);
        goto L_ex_1L;
        // goto L_ex_1P; // den Punkt pto speichern, fertig.




      //.......................................................
      } else if(new_typ == Typ_CVRBSP) {           // LN > RBSP
        // printf(" -- LN > RBSP %d\n",new_typ);

        i1 = AP_cont_Conn__();
        if(i1 == 0) goto L_ex_LN12;
        if(i1 == 1) { TX_Error("AP_cont_inv E_LN_RBSP_int"); return -1;}




      //.......................................................
      } else if(new_typ == Typ_CVPOL) {           // LN > PLG
        // printf(" -- LN > CVPOL %d\n",new_typ);

        i1 = 10; // size of pa, da
        UT3D_pt_intlnplg (&i1, pta, da, pln, o1Dat); // inters.
        APT_set_modMax (i1);

        i2 = AP_cont_PTmod (&pln->p1, pta, i1, imod);
        pto = pta[i2];

        // printf(" rc=%d par=%f pto=%f %f %f\n",irc,da[0],pto.x,pto.y,pto.z);
        ((CurvPoly*)o1Dat)->v0 = da[i2];  // Startpkt plg
        ln1.p1 = pln->p1;
        ln1.p2 = pta[i2];
        goto L_ex_1L;

      }





    //----------------------------------------------------------------
    } else if(old_typ == Typ_CI) {
      // printf(" -- AC > CV mod=%d\n",modFlg);

      pci = (void*)o2Dat;
      // Start/Endpunkt des oldObj
      UTO_2pt_limstru (&pto2a, &pto2e, NULL, NULL, old_typ, o2Dat);
      // UT3D_stru_dump (Typ_CI, pci, " old Circ =\n");


      //.......................................................
      if(new_typ == Typ_CVBSP) {
        // printf(" -- AC > CVBSP\n");


        // Startpunkt/Endpkt der CV
        pt1 = pto1a;
        pt2 = pto1e;
        // UT3D_stru_dump (Typ_PT, &pt1, "  BSP-v1");
        // UT3D_stru_dump (Typ_PT, &pt2, "  BSP-v0");

        // stimmen die Endpunkte ueberein ?
        i1 = UT3D_comp4pt(&pci->p1,&pci->p2,&pt1,&pt2,UT_TOL_cv);
        // printf("nach UT3D_comp4pt %d\n",i1);

        // rc=1: beide El. liegen richtig.
        if(i1 == 1) goto L_A_C_1;

        // rc=2: Obj 1 verkehrt: Circ umdrehen
        // darf nur passieren, wenn AC das allererste Obj ist (o4.typ == 0)
        if(i1 == 2) {
          UT3D_ci_inv1 (pci);
          goto L_A_C_1;
        }

        // rc=3: Obj 2 verkehrt: CV umdrehen
        if(i1 == 3) {
          MEM_swap__(&((CurvBSpl*)o1Dat)->v0,
                       &((CurvBSpl*)o1Dat)->v1, sizeof(double));
          goto L_A_C_1;
        }

        // rc=4: Obj 1 und Obj 2 verkehrt; Circ und CV umdrehen
        // darf nur passieren, wenn AC das allererste Obj ist (o4.typ == 0)
        if(i1 == 4) {
          UT3D_ci_inv1 (pci);
          MEM_swap__(&((CurvBSpl*)o1Dat)->v0,
                       &((CurvBSpl*)o1Dat)->v1, sizeof(double));
          goto L_A_C_1;
        }


        // Schnitt bspl - ci
        i1 = 10;
        UT3D_pt_intcibspl (&i1, pta, pci, o1Dat, ooDat);
        // printf(" nach UT3D_pt_intcibspl i1=%d\n",i1);
        if(i1 < 1) {
          // es gibt keinen Schnittpunkt; eine Verbindungslinie raus ?
          ln1.p1 = pci->p2;
          ln1.p2 = pt1;
          ci1 = *pci;
          goto L_ex_2CL;
        }
        APT_set_modMax (i1);
        // GR_Disp_pTab (i1, pta, SYM_STAR_S, 2);


        if(modFlg >= 0) i2 = imod;
        else {
          // wenn ein Schnittpunkt ident mit dem alten Endpt: OK
          i2 = UT3D_pt_ck_npt (&ci1.p2,pta,i1,UT_TOL_pt);
          if(i2 < 0) {
            // den dem alten Startpunkt naeheren Schnittpt waehlen
            // jedoch NICHT wenn ident mit dem alten Startpunkt
            i2 = AP_cont_nearPt (&ci1.p1, pta, i1);
          }
          // i2 = UT3D_ipt_ckfar_npt (&pci->p1, pta, i1);
        }
           
        // // i2 = UT3D_ipt_cknear_npt (&pci->p1, pta, i1);
        // i2 = AP_cont_PTmod (&pci->p1, pta, i1, imod);

        pto = pta[i2];

        // pci->p2 = pto; // Endpt old CI
        UT3D_ci_cip2 (pci, &pto);  // Endpt old CI


        // den Parameter zum Punkt pto holen
        UT3D_parCv_bsplpt (&da[i2], &d1, o1Dat, &pto);
        ((CurvBSpl*)o1Dat)->v0 = da[i2];   // Startpt bspl


        // den Kreis ci1 raus, fertig.
        L_A_C_1:
        ci1 = *pci;
        goto L_ex_1C;




      //.......................................................
      } else if(new_typ == Typ_CVRBSP) {           // AC > RBSP
        // printf(" -- AC > RBSP %d\n",new_typ);

        i1 = AP_cont_Conn__();
        if(i1 == 0) goto L_ex_AC12;  // (Circ*)o2Dat & ConnLn out
        if(i1 == 1) { TX_Error("AP_cont_inv E_AC_RBSP_int"); return -1;}




      //.......................................................
      } else if(new_typ == Typ_CVPOL) {   // AC > PLG
        // printf(" -- AC > CVPOL\n");

        i1 = 10; // size of pa, da
        irc = UT3D_pt_intciplg (&i1, pta, da, pci, o1Dat);
        if(i1 < 0) return -1;

        APT_set_modMax (i1);

        i1 = AP_cont_intPt (i1, pta);
        if(i1 < 0) return -1;
        goto L_ex_AC12;

// 
        // // den dem Wert v0 naechsten da-Wert suchen
        // // i2 = UT3D_ipt_cknear_npt (&pci->p1, pta, i1);
        // i2 = AP_cont_PTmod (&pci->p1, pta, i1, imod);
        // pto = pta[i2];
        // // printf(" rc=%d par=%f pto=%f %f %f\n",irc,da[0],pto.x,pto.y,pto.z);
        // ((CurvPoly*)o1Dat)->v0 = da[i2];   // Startpt plg
        // ci1 = *pci;
        // // ci1.p2 = pto; // Endpt CI
        // UT3D_ci_cip2 (&ci1, &pto);  // Endpt CI
        // goto L_ex_1C;



      //.......................................................
      } else if(new_typ == Typ_CVELL) {          // AC > ELL
        // printf(" AC > CVELL \n");
        pell = (void*)o1Dat;    // circ > elli
        ci1  = *pci;

        // UT3D_stru_dump (Typ_CVELL, o1Dat, "  new obj:\n");

        // Startpunkt/Endpkt des oldObj schon gesetzt

        // Startpunkt/Endpkt des newObj
        UTO_2pt_limstru (&pto1a, &pto1e, NULL, NULL, new_typ, o1Dat);

        // geringsten Abstand von 4 Punkten suchen
        ConnTyp = UT3D_minLen_4pt (&d1, &pto2a, &pto2e, &pto1a, &pto1e);
        if(d1 > UT_TOL_cv) goto L_AC_ELL_int;  // goto intersect


        L_AC_ELL_1:
        // invert lin.curves in structs according to ConnTyp
        AP_cont_inv (ConnTyp);
        goto L_ex_AC12; // Circ o2Dat + Line ln2 ausgeben


        L_AC_ELL_int:
        // intersect
        UT3D_pt_intciel (&i1, pta, &ci1, pell, ooDat);


        // kein Schnittpunkt: add connecting-line
        if(i1 < 1) {
          ConnLn = 1;
          // give ConnectionLine according to ConnTyp
          AP_cont_ConnLn (&ln2, ConnTyp, &pto1a, &pto1e, &pto2a, &pto2e);
          goto L_AC_ELL_1;
        }

        APT_set_modMax (i1);

        i1 = AP_cont_intPt (i1, pta);
        if(i1 < 0) return -1;
        goto L_ex_AC12;
// 
        // if(modFlg >= 0) i2 = imod;
        // else {
          // // wenn ein Schnittpunkt ident mit dem alten Endpt: OK
          // i2 = UT3D_pt_ck_npt (&ci1.p2,pta,i1,UT_TOL_pt);
          // if(i2 < 0) {
            // // den dem alten Startpunkt naeheren Schnittpt waehlen
            // // jedoch NICHT wenn ident mit dem alten Startpunkt
            // i2 = AP_cont_nearPt (&ci1.p1, pta, i1);
          // }
        // }
// 
        // // wenn pell.p1 ist ident mit pta: den anderen Punkt nehmen
        // // pto = pta[AP_cont_nearPt(&ci1.p1, pta, i1)];
        // // den Index holen; Punkt muss != pto2a(Startpkt plg) sein
        // // i2 = AP_cont_PTmod (&ci1.p1, pta, i1, imod);
        // pto = pta[i2];
// 
        // // ci1.p2   = pto;  // endpt circ
        // UT3D_ci_cip2 (&ci1, &pto);  // Endpt CI
        // pell->p1 = pto;  // startpt elli
        // goto L_ex_1C;    // circ ci1 speichern und ausgeben




      //.......................................................
      } else if(new_typ == Typ_CVCLOT) {          // AC > CLOT
        printf(" AC > CLOT \n");

        // Startpunkt/Endpkt des newObj schon gesetzt
        // Startpunkt/Endpkt des oldObj schon gesetzt

        // geringsten Abstand von 4 Punkten suchen
        ConnTyp = UT3D_minLen_4pt (&d1, &pto2a, &pto2e, &pto1a, &pto1e);
        if(d1 > UT_TOL_cv) goto L_noInt;
          printf(" d1=%f ConnTyp=%d\n",d1,ConnTyp);

        // invert lin.curves in structs according to ConnTyp
        AP_cont_inv (ConnTyp);
        ci1 = *pci;
        goto L_ex_1C;    // circ ci1 speichern und ausgeben




      //.......................................................
      }



    //----------------------------------------------------------------
    } else if(old_typ == Typ_CV) {
      // printf(" -- CV > CV\n");
      // ACHTUNG: old_typ ist Typ_CV,
      //          o2Typ   ist Typ_CVBSP od ..


      //---------------------------------------------------------
      if(new_typ == Typ_CVRBSP) {
        // pto1a/e schon gesetzt

        //.......................................................
        if(o2Typ == Typ_CVRBSP) {
          // printf(" -- RBSP > RBSP\n");

          i1 = AP_cont_Conn__();
          if(i1 == 0) goto L_ex_RBSP;
          if(i1 == 1) { TX_Error("AP_cont_inv E_RBSP_RBSP_int"); return -1;}


        //.......................................................
        } else if(o2Typ == Typ_CVBSP) {
          // printf(" -- BSP > RBSP %d\n",new_typ);

          i1 = AP_cont_Conn__();
          if(i1 == 0) goto L_ex_CV;  // (Circ*)o2Dat & ConnLn out
          if(i1 == 1) { TX_Error("AP_cont_inv E_BSP_RBSP_int"); return -1;}


        //.......................................................
        } else {
          TX_Error("AP_cont_nxt E001RBSP");
          return -1;
        }



      //---------------------------------------------------------
      } else if(new_typ == Typ_CVBSP) {

        //.......................................................
        if(o2Typ == Typ_CVRBSP) {
          // printf(" -- RBSP > BSP\n");

          i1 = AP_cont_Conn__();
          if(i1 == 0) goto L_ex_RBSP;
          if(i1 == 1) { TX_Error("AP_cont_inv E_RBSP_BSP_int"); return -1;}

// 
  // geht nicht, da einige im folgenden noch kommen;
        // //.......................................................
        // } else {
          // TX_Error("AP_cont_nxt E001RBSP");
          // return -1;
//
        }

      }




      //.......................................................
      if((new_typ == Typ_CVBSP)&&(o2Typ == Typ_CVBSP)) {
        // printf(" CVBSP > CVBSP \n");

        // UT3D_stru_dump (Typ_CVBSP, o2Dat, "  old obj:\n");
        // UT3D_stru_dump (Typ_CVBSP, o1Dat, "  new obj:\n");

        // Startpunkt/Endpkt des oldObj
        UTO_2pt_limstru (&pto2a, &pto2e, NULL, NULL, o2Typ, o2Dat);

        // Startpunkt/Endpkt des newObj
        UTO_2pt_limstru (&pto1a, &pto1e, NULL, NULL, new_typ, o1Dat);

        // geringsten Abstand von 4 Punkten suchen
        ConnTyp = UT3D_minLen_4pt (&d1, &pto2a, &pto2e, &pto1a, &pto1e);
        if(d1 > UT_TOL_cv) goto L_BSP_BSP_int;  // goto intersect

        L_BSP_BSP_1:
        // invert lin.curves in structs according to ConnTyp
        AP_cont_inv (ConnTyp);
        goto L_ex_CV; // LN o2Dat u ln2 raus


        L_BSP_BSP_int:
        // Schnitt bsp x bsp
        i1 = 10; // size of pa, da
        irc = UT3D_pt_int2bspl (&i1, pta, ooDat, o2Dat, o1Dat); // intersect
        if(irc < 0) return irc;
        APT_set_modMax (i1);
        // printf(" int %d\n",i1);

        // kein Schnittpunkt: add connecting-line ln2
        if(i1 < 1) {
          ConnLn = 1;
          // give ConnectionLine according to ConnTyp
          AP_cont_ConnLn (&ln2, ConnTyp, &pto1a, &pto1e, &pto2a, &pto2e);
          goto L_BSP_BSP_1;
        }


        // pto2a = Startpunkt der old-CV
        // den Index holen; Punkt muss != pto2a(Startpkt plg) sein
        i2 = AP_cont_PTmod (&pto2a, pta, i1, imod);
        pto = pta[i2];

        // den Endparameter fuer alte CV setzen
        UT3D_parCv_bsplpt (da, &d1, o2Dat, &pto);
        ((CurvBSpl*)o2Dat)->v1 = da[0];

        // den Startparameter fuer neue CV setzen
        UT3D_parCv_bsplpt (da, &d1, o1Dat, &pto);
        ((CurvBSpl*)o1Dat)->v0 = da[0];
        goto L_ex_CV;



      //.......................................................
      } else if((o2Typ == Typ_CVBSP)&&(new_typ == Typ_CVPOL)) {
        // printf(" CVBSP > CVPOL \n");
        // Schnitt bsp x pol
        i1 = 10; // size of pa, da
        UT3D_pt_intbspplg (&i1, pta, da, ooDat, o2Dat, o1Dat);
        APT_set_modMax (i1);
        // compute Startpunkt am bspl -> pt1
        UT3D_pt_evalparCv (&pt1, o2Dat, ((CurvBSpl*)o2Dat)->v0);
        // den naechsten Punkt aber nicht denselben
        // i2 = AP_cont_nearPt (&pt1, pta, i1);
        i2 = AP_cont_PTmod (&pt1, pta, i1, imod);
        pto = pta[i2];

        // Endpt bspl setzen
        UT3D_parCv_bsplpt (&((CurvBSpl*)o2Dat)->v1, &d1, o2Dat, &pto);

        // Startpt plg setzen
        ((CurvPoly*)o1Dat)->v0 = da[i2];

        goto L_ex_CV;



      //.......................................................
      } else if((o2Typ == Typ_CVPOL)&&(new_typ == Typ_CVBSP)) {
        // printf(" CVPOL > CVBSP \n");
        // Schnitt bsp x bsp
        i1 = 10; // size of pa, da
        UT3D_pt_intbspplg (&i1, pta, da, ooDat, o1Dat, o2Dat);
        APT_set_modMax (i1);
        // compute Startpunkt am plg -> pt1
        UT3D_pt_evalplg (&pt1, o2Dat, ((CurvPoly*)o2Dat)->v0);
        // den naechsten Punkt aber nicht denselben
        // i2 = AP_cont_nearPt (&pt1, pta, i1);
        i2 = AP_cont_PTmod (&pt1, pta, i1, imod);
        pto = pta[i2];

        // Endpt plg setzen
        ((CurvPoly*)o2Dat)->v1 = da[i2];

        // Startpt bspl setzen
        UT3D_parCv_bsplpt (&((CurvBSpl*)o1Dat)->v0, &d1, o1Dat, &pto);

        goto L_ex_POL;



      //.......................................................
      } else if((o2Typ == Typ_CVPOL)&&(new_typ == Typ_CVPOL)) {
        // printf(" CVPOL > CVPOL \n");

        // UT3D_stru_dump (Typ_CVPOL, o2Dat, "  old obj:\n");
        // UT3D_stru_dump (Typ_CVPOL, o1Dat, "  new obj:\n");

        // Startpunkt/Endpkt des oldObj
        UTO_2pt_limstru (&pto2a, &pto2e, NULL, NULL, o2Typ, o2Dat);

        // Startpunkt/Endpkt des newObj
        UTO_2pt_limstru (&pto1a, &pto1e, NULL, NULL, new_typ, o1Dat);

        // geringsten Abstand von 4 Punkten suchen
        ConnTyp = UT3D_minLen_4pt (&d1, &pto2a, &pto2e, &pto1a, &pto1e);
        if(d1 > UT_TOL_cv) goto L_POL_POL_int;  // goto intersect

        L_POL_POL_1:
        // invert lin.curves in structs according to ConnTyp
        AP_cont_inv (ConnTyp);
        goto L_ex_POL; // LN o2Dat u ln2 raus


        L_POL_POL_int:
        // intersect
        i1 = 10; // size of pa, da
        UT3D_pt_int2plg (&i1, pta, da, da1, o2Dat, o1Dat);
        APT_set_modMax (i1);


        // kein Schnittpunkt: add connecting-line ln2
        if(i1 < 1) {
          ConnLn = 1;
          // give ConnectionLine according to ConnTyp
          AP_cont_ConnLn (&ln2, ConnTyp, &pto1a, &pto1e, &pto2a, &pto2e);
          goto L_POL_POL_1;
        }


        // compute Startpunkt am old-plg -> pt1
        UT3D_pt_evalplg (&pt1, o2Dat, ((CurvPoly*)o2Dat)->v0);
        // den naechsten Punkt aber nicht denselben
        // i2 = AP_cont_nearPt (&pt1, pta, i1);
        i2 = AP_cont_PTmod (&pt1, pta, i1, imod);
        pto = pta[i2];

        // Endpt old-plg setzen
        ((CurvPoly*)o2Dat)->v1 = da[i2];

        // Startpt new-plg setzen
        ((CurvPoly*)o1Dat)->v0 = da1[i2];

        goto L_ex_POL;



      //.......................................................
      } else if((o2Typ == Typ_CVELL)&&(new_typ == Typ_CVPOL)) {
        // printf(" CVELL > CVPOL \n");
        pell = (CurvElli*)o2Dat;
        pplg = (CurvPoly*)o1Dat;
        i1 = 10; // size of pa, da
        UT3D_pt_intelplg (&i1, pta, NULL, da, pell, pplg);
        APT_set_modMax (i1);
        // i2 = AP_cont_nearPt(&pell->p1, pta, i1);
        i2 = AP_cont_PTmod (&pell->p1, pta, i1, imod);
        pell->p2 = pta[i2];  // endpt elli
        pplg->v0 = da[i2];   // Startpt plg

        goto L_ex_ELL;       // Ellipse pell speichern und ausgeben



      //.......................................................
      } else if((o2Typ == Typ_CVPOL)&&(new_typ == Typ_CVELL)) {
        // printf(" CVPOL > CVELL \n");
        pplg = (CurvPoly*)o2Dat;
        pell = (CurvElli*)o1Dat;
        i1 = 10; // size of pa, da
        UT3D_pt_intelplg (&i1, pta, NULL, da, pell, pplg);
        APT_set_modMax (i1);

        // compute Startpunkt am old-plg -> pt1
        UT3D_pt_evalplg (&pt1, pplg, pplg->v0);
        // den naechsten Punkt aber nicht denselben
        // i2 = AP_cont_nearPt (&pt1, pta, i1);
        i2 = AP_cont_PTmod (&pt1, pta, i1, imod);
        pplg->v1 = da[i2];   // endpt plg
        pell->p1 = pta[i2];  // Startpt elli

        goto L_ex_POL;       // Polygon o2Dat speichern und ausgeben




      //.......................................................
      } else if((o2Typ == Typ_CVBSP)&&(new_typ == Typ_CVELL)) {
        // printf(" CVBSP > CVELL \n");
        pbsp = (CurvBSpl*)o2Dat;
        pell = (CurvElli*)o1Dat;

        UT3D_pt_evalparCv (&pt1, o2Dat, ((CurvBSpl*)o2Dat)->v0);
        UT3D_pt_evalparCv (&pt2, o2Dat, ((CurvBSpl*)o2Dat)->v1);

        i1 = UT3D_comp4pt (&pt1, &pt2, &pell->p1, &pell->p2, UT_TOL_cv);
        // printf("nach UT3D_comp4pt %d\n",i1);

        // rc=1: beide El. liegen richtig.
        if(i1 == 1) goto L_ex_CV;

        // rc=2: Obj 1 verkehrt
        // darf nur passieren, wenn CV das allererste Obj ist (o4.typ == 0)
        if(i1 == 2) {
          MEM_swap__(&((CurvBSpl*)o2Dat)->v0,
                       &((CurvBSpl*)o2Dat)->v1, sizeof(double));
          goto L_ex_CV;
        }
        // rc=3: Obj 2 verkehrt; O1 umdrehen.
        if(i1 == 3) {
          UT3D_el_inv1 (pell);
          goto L_ex_CV;
        }
        // rc=4: Obj 1 und Obj 2 verkehrt
        // darf nur passieren, wenn CV das allererste Obj ist (o4.typ == 0)
        if(i1 == 4) {
          UT3D_el_inv1 (pell);
          MEM_swap__(&((CurvBSpl*)o2Dat)->v0,
                       &((CurvBSpl*)o2Dat)->v1, sizeof(double));
          goto L_ex_CV;
        }



        i1 = 10; // size of pa, da
        UT3D_pt_intelbspl (&i1, pta, pell, pbsp, ooDat);
        APT_set_modMax (i1);
        // compute Startpunkt am old-bsp -> pt1
        UT3D_pt_evalparCv (&pt1, pbsp, pbsp->v0);
        // i2 = AP_cont_nearPt(&pt1, pta, i1);
        i2 = AP_cont_PTmod (&pt1, pta, i1, imod);
        UT3D_parCv_bsplpt (&pbsp->v1, &d1, pbsp, &pta[i2]); // endpt bsp
        pell->p1 = pta[i2];  // Startpt elli
        goto L_ex_CV;        // B-Spline o2Dat speichern und ausgeben



      //.......................................................
      } else if((o2Typ == Typ_CVELL)&&(new_typ == Typ_CVBSP)) {
        // printf(" CVELL > CVBSP \n");
        pell = (CurvElli*)o2Dat;
        pbsp = (CurvBSpl*)o1Dat;

        // Startpt BSP
        UT3D_pt_evalparCv (&pto2a, o1Dat, ((CurvBSpl*)o1Dat)->v0);
        UT3D_pt_evalparCv (&pto2e, o1Dat, ((CurvBSpl*)o1Dat)->v1);

        i1 = UT3D_comp4pt (&pell->p1, &pell->p2, &pto2a, &pto2e, UT_TOL_cv);
        // printf("nach UT3D_comp4pt %d\n",i1);

        // rc=1: beide El. liegen richtig.
        if(i1 == 1) goto L_ex_ELL;

        // rc=2: Obj 1 verkehrt
        // darf nur passieren, wenn CV das allererste Obj ist (o4.typ == 0)
        if(i1 == 2) {
          UT3D_el_inv1 (pell);
          goto L_ex_ELL;
        }
        // rc=3: Obj 2 verkehrt; O1 (New Spl)umdrehen.
        if(i1 == 3) {
          MEM_swap__(&((CurvBSpl*)o1Dat)->v0,
                       &((CurvBSpl*)o1Dat)->v1, sizeof(double));
          goto L_ex_ELL;
        }
        // rc=4: Obj 1 und Obj 2 verkehrt
        // darf nur passieren, wenn CV das allererste Obj ist (o4.typ == 0)
        if(i1 == 4) {
          UT3D_el_inv1 (pell);
          MEM_swap__(&((CurvBSpl*)o1Dat)->v0,
                       &((CurvBSpl*)o1Dat)->v1, sizeof(double));
          goto L_ex_ELL;
        }


        i1 = 10; // size of pa, da
        UT3D_pt_intelbspl (&i1, pta, pell, pbsp, ooDat);
        APT_set_modMax (i1);

        // i2 = AP_cont_nearPt(&pell->p1, pta, i1);
        i2 = AP_cont_PTmod (&pell->p1, pta, i1, imod);
        pell->p2 = pta[i2];  // endpt elli
        UT3D_parCv_bsplpt (&pbsp->v0, &d1, pbsp, &pta[i2]); // startpt bsp

        goto L_ex_ELL;       // Ellipse pell speichern und ausgeben



      //.......................................................
      } else if((o2Typ == Typ_CVELL)&&(new_typ == Typ_CVELL)) {
        // printf("  CVELL > CVELL\n");

        // Startpunkt/Endpkt des oldObj
        UTO_2pt_limstru (&pto2a, &pto2e, NULL, NULL, o2Typ, o2Dat);

        // Startpunkt/Endpkt des newObj
        UTO_2pt_limstru (&pto1a, &pto1e, NULL, NULL, new_typ, o1Dat);

        // geringsten Abstand von 4 Punkten suchen
        ConnTyp = UT3D_minLen_4pt (&d1, &pto2a, &pto2e, &pto1a, &pto1e);
        if(d1 > UT_TOL_cv) goto L_ELL_ELL_int;  // goto intersect

        L_ELL_ELL_1:
        // invert lin.curves in structs according to ConnTyp
        AP_cont_inv (ConnTyp);
        goto L_ex_ELL; // ELL o2Dat u ln2 raus


        L_ELL_ELL_int:
        // intersect elli - elli
        pell = (CurvElli*)o2Dat;  // old elli
        pel1 = (CurvElli*)o1Dat;  // new elli
        UT3D_pt_intelel (&i1, pta, pell, pel1, ooDat);
        APT_set_modMax (i1);


        // kein Schnittpunkt: add connecting-line ln2
        if(i1 < 1) {
          ConnLn = 1;
          // give ConnectionLine according to ConnTyp
          AP_cont_ConnLn (&ln2, ConnTyp, &pto1a, &pto1e, &pto2a, &pto2e);
          goto L_LN_BSP_1;
        }

        // i2 = AP_cont_nearPt(&pell->p1, pta, i1);
        i2 = AP_cont_PTmod (&pell->p1, pta, i1, imod);
        pell->p2 = pta[i2];  // endpt old-elli
        pel1->p1 = pta[i2];  // startpt new-elli

        goto L_ex_ELL;       // Ellipse pell speichern und ausgeben


      //.......................................................
      } else if((o2Typ == Typ_CVCLOT)&&(new_typ == Typ_CVCLOT)) {
        printf("  CVCLOT > CVCLOT\n");

        // Startpunkt/Endpkt des oldObj
        UTO_2pt_limstru (&pto2a, &pto2e, NULL, NULL, o2Typ, o2Dat);

        // Startpunkt/Endpkt des newObj schon fertig ..

        // geringsten Abstand von 4 Punkten suchen
        ConnTyp = UT3D_minLen_4pt (&d1, &pto2a, &pto2e, &pto1a, &pto1e);
        if(d1 > UT_TOL_cv) goto L_noInt;
          printf(" d1=%f ConnTyp=%d\n",d1,ConnTyp);

        // invert lin.curves in structs according to ConnTyp
        AP_cont_inv (ConnTyp);
        goto L_ex_CLOT;           // LN o2Dat u ln2 raus




      //.......................................................
      }





    //----------------------------------------------------------------
    } else {
      TX_Error("AP_cont_nxt E002P");
      return -1;
    }




  //=====================================================================
  //========================== - ====================================
  } else {

    TX_Error("AP_cont_nxt E001X_%d",new_typ);
    return -1;
  }


  TX_Error("AP_cont_nxt E000 %d %d",old_typ,new_typ);
  return -1;





  //==========================================================
  // den Punkt pto speichern und raus ..
  // *oo1i muss schon gesetzt sein !
  L_ex_1P:
  // DBind = DB_StorePoint (-1L, &pto);
  // oo1->data = (void*)DBind;
  oo1->typ  = Typ_PT;
  oo1->form = Typ_PT;
  oo1->siz  = 1;
  oo1->data = UME_save (ooDat, &pto, sizeof(Point));

  oo2->typ  = Typ_Error;
  goto L_ex_copy;




  //==========================================================
  // line ln1 raus; *oo1i muss schon gesetzt sein !
  L_ex_1L:
    // UT3D_stru_dump (Typ_LN, &ln1, "L_ex_1L: ");
  oo1->typ  = Typ_LN;
  oo1->form = Typ_LN;
  oo1->siz  = 1;
  oo1->dir  = 0;
  oo1->data = UME_save (ooDat, &ln1, sizeof(Line));
  *oo1i = o2Ind;

  oo2->typ  = Typ_Error;
  goto L_ex_copy;



  //==========================================================
  // line o2Dat raus; *oo1i muss schon gesetzt sein !
  L_ex_LN12:
  oo1->typ  = Typ_LN;
  oo1->form = Typ_LN;
  oo1->siz  = 1;
  oo1->dir  = 0;
  oo1->data = UME_save (ooDat, o2Dat, sizeof(Line));
  *oo1i = o2Ind;

  if(ConnLn != 0) goto L_ex_LN2;
  else oo2->typ  = Typ_Error;
  goto L_ex_copy;




  //==========================================================
  // Circ o2Dat + Line ln2 ausgeben
  L_ex_AC12:
  oo1->typ  = Typ_CI;
  oo1->form = Typ_CI;
  oo1->siz  = 1;
  oo1->dir  = 0;
  oo1->data = UME_save (ooDat, o2Dat, sizeof(Circ));
  *oo1i = o2Ind;

  if(ConnLn != 0) goto L_ex_LN2;
  else oo2->typ  = Typ_Error;
  goto L_ex_copy;







  //==========================================================
  // den Kreis ci1 raus, fertig.
  L_ex_1C:
  // DBind = DB_StoreCirc (-1L, &ci1);
  // oo1->data = (void*)DBind;
  oo1->typ  = Typ_CI;
  oo1->form = Typ_CI;
  oo1->siz  = 1;
  oo1->dir  = 0;
  oo1->data = UME_save (ooDat, &ci1, sizeof(Circ));
  *oo1i = o2Ind;

  oo2->typ  = Typ_Error;
  goto L_ex_copy;





// 
  // //==========================================================
  // // den Punkt pto und pt1 speichern und raus ..
  // L_ex_2PP:
  // DBind = DB_StorePoint (-1L, &pto);
  // oo1->data = (void*)DBind;
  // oo1->typ  = Typ_PT;
// 
  // DBind = DB_StorePoint (-1L, &pt1);
  // oo2->data = (void*)DBind;
  // oo2->typ  = Typ_PT;
// 
  // goto L_ex_copy;





  //==========================================================
  // oo1 = Line ((Line*)o2Dat->p1 -> pt1)
  // oo2 = Line (pt1 -> ppt)
  L_ex_2LL:
  oo1->typ  = Typ_LN;
  oo1->form = Typ_LN;
  oo1->siz  = 1;
  oo1->dir  = 0;
  oo1->data = UME_save (ooDat, &ln1, sizeof(Line));
  *oo1i = o2Ind;

  oo2->typ  = Typ_LN;
  oo2->form = Typ_LN;
  oo2->siz  = 1;
  oo1->dir  = 0;
  oo2->data = UME_save (ooDat, &ln2, sizeof(Line));
  *oo2i = oInd;

  goto L_ex_copy;





  //==========================================================
  // oo1 = Circ ci1
  // oo2 = Line ln1
  L_ex_2CL:
  oo1->typ  = Typ_CI;
  oo1->form = Typ_CI;
  oo1->siz  = 1;
  oo1->dir  = 0;
  oo1->data = UME_save (ooDat, &ci1, sizeof(Circ));
  *oo1i = o2Ind;

  oo2->typ  = Typ_LN;
  oo2->form = Typ_LN;
  oo2->siz  = 1;
  oo2->dir  = 0;
  oo2->data = UME_save (ooDat, &ln1, sizeof(Line));
  *oo2i = oInd;

  goto L_ex_copy;






  //==========================================================
  L_ex_ELL:      // Ellipse o2Dat + Line ln2 ausgeben
// 
  // ox1.typ   = Typ_CVELL;
  // ox1.form  = Typ_CVELL;
  // ox1.data  = o2Dat;
  // ox1.siz   = 1;
  // oo1->dir  = 0;
  // DBind = DB_StoreCurv (-1L, &ox1, 0);
  // oo1->typ  = Typ_CVELL;
  // oo1->form = Typ_Index;
  // oo1->siz  = 1;
  // oo1->dir  = 0;
  // oo1->data = (void*)DBind;


  oo1->typ  = Typ_CVELL;
  oo1->form = Typ_CVELL;
  oo1->siz  = 1;
  oo1->dir  = 0;
  oo1->data = UME_save (ooDat, o2Dat, sizeof(CurvElli));

  *oo1i = o2Ind;


  if(ConnLn != 0) goto L_ex_LN2;
  goto L_ex_copy;



  //==========================================================
  L_ex_CV:       // B-Spline o2Dat speichern und ausgeben
        ox1.typ   = Typ_CVBSP;
        ox1.form  = Typ_CVBSP;
        ox1.data  = o2Dat;
        ox1.siz   = 1;
        oo1->dir  = 0;
        DBind = DB_StoreCurv (-1L, &ox1, 0);

        oo1->typ  = Typ_CVBSP;
        oo1->form = Typ_Index;
        oo1->siz  = 1;
        oo1->dir  = 0;
        oo1->data = (void*)DBind;
    *oo1i = o2Ind;

    if(ConnLn != 0) goto L_ex_LN2;
    goto L_ex_copy;


  //==========================================================
  L_ex_CCV:       // o2Dat speichern und ausgeben
        // ox1.typ   = Typ_CVTRM;
        // ox1.form  = Typ_ObjGX;
        // ox1.data  = o2Dat;
        // ox1.siz   = 1;
        // oo1->dir  = 0;
        // DBind = DB_StoreCurv (-1L, &ox1, 0);
// 
        // oo1->typ  = Typ_CVTRM;
        // oo1->form = Typ_Index;
        // oo1->siz  = 1;
        // oo1->dir  = 0;
        // oo1->data = (void*)DBind;
        memcpy (oo1, o1Dat, sizeof(ObjGX));
    *oo1i = o2Ind;

    if(ConnLn != 0) goto L_ex_LN2;
    goto L_ex_copy;



  //==========================================================
  L_ex_CLOT:       // Cloth. o2Dat speichern und ausgeben
        ox1.typ   = Typ_CVCLOT;
        ox1.form  = Typ_CVCLOT;
        ox1.data  = o2Dat;
        ox1.siz   = 1;
        oo1->dir  = 0;
        DBind = DB_StoreCurv (-1L, &ox1, 0);

        oo1->typ  = Typ_CVCLOT;
        oo1->form = Typ_Index;
        oo1->siz  = 1;
        oo1->dir  = 0;
        oo1->data = (void*)DBind;
  *oo1i = o2Ind;

    if(ConnLn != 0) goto L_ex_LN2;
    goto L_ex_copy;



  //==========================================================
  L_ex_RBSP:       // Rat.B-Spline o2Dat speichern und ausgeben
        // UT3D_stru_dump (Typ_CVRBSP, o2Dat, " save rbsp\n");

        ox1.typ   = Typ_CVRBSP;
        ox1.form  = Typ_CVRBSP;
        ox1.data  = o2Dat;
        ox1.siz   = 1;
        ox1.dir   = 0;
        DBind = DB_StoreCurv (-1L, &ox1, 0);
        // printf(" DBind=%d\n",DBind);

        oo1->typ  = Typ_CVRBSP;
        oo1->form = Typ_Index;
        oo1->siz  = 1;
        oo1->dir  = 0;
        oo1->data = (void*)DBind;
        *oo1i = o2Ind;

    if(ConnLn != 0) goto L_ex_LN2;
    goto L_ex_copy;



  //==========================================================
  L_ex_POL:            // Polygon o2Dat speichern und ausgeben
          // UT3D_stru_dump (Typ_CVPOL, (CurvPoly*)o2Dat, "out-plg-o2Dat:");
        ox1.typ   = Typ_CVPOL;
        ox1.form  = Typ_CVPOL;
        ox1.data  = o2Dat;
        ox1.siz   = 1;
        DBind = DB_StoreCurv (-1L, &ox1, 0);

        oo1->typ  = Typ_CVPOL;
        oo1->form = Typ_Index;
        oo1->siz  = 1;
        oo1->dir  = 0;
        oo1->data = (void*)DBind;
    *oo1i = o2Ind;

    if(ConnLn != 0) goto L_ex_LN2;
    goto L_ex_copy;



  
  //==========================================================
  // add zusaetzliche ConnectionLine ln2 als 2. Obj
  L_ex_LN2:
    // UT3D_stru_dump (Typ_LN, &ln2, " L_ex_LN2:\n");
  oo2->typ  = Typ_LN;
  oo2->form = Typ_LN;
  oo2->siz  = 1;
  oo2->dir  = 0;
  oo2->data = UME_save (ooDat, &ln2, sizeof(Line));
  *oo2i = oInd;

  goto L_ex_copy;


// 
  // //==========================================================
  // // den Kreis ci1 raus, dann Eingabelement raus, fertig.
  // L_ex_2CI:
  // DBind = DB_StoreCirc (-1L, &ci1);
  // oo1->data = (void*)DBind;
  // oo1->typ  = Typ_CI;
  // *oo1i = o2Ind;
// 
  // *oo2 = *o1;
  // *oo2i = oInd;
// 
  // goto L_ex_copy;





  //==========================================================
  // umspeichern - merken
  L_ex_copy:
  // o4 = o3;
  o3 = o2;

  o2    = *o1;

  o2Typ = new_typ;
  memcpy(o2Dat, o1Dat, o1Siz);
  pto2a = pto1a;
  pto2e = pto1e;
   

  o2Ind = oInd;



  L_exit:
// 
  // // wenn o4.typ=Typ_Error und o3.typ=LN dann wurde kein Startpunkt
  // // ausgegeben; nachholen!
  // printf("o2.typ=%d\n",o2.typ);
  // printf("o3.typ=%d\n",o3.typ);
  // printf("o4.typ=%d\n",o4.typ);
  // if(o4.typ == Typ_Error) {
    // if(o4.typ == Typ_LN) {
      // // ein Ausg.Ele sollte in oo1 liegen; nach oo2 kopieren
      // // nun den Startpunkt -> oo1
    // }
  // }


  // Testausgaben:
  printf("ex AP_cont_nxt irc=0 oo1i=%d oo2i=%d\n",*oo1i,*oo2i);
  if(oo1->typ != Typ_Error){printf("oo1/%d: ",oo1->typ);UTO_dump__(oo1,"");}
  if(oo2->typ != Typ_Error){printf("oo2/%d: ",oo2->typ);UTO_dump__(oo2,"");}




  return 0;



  L_noFunc:
    TX_Error("AP_cont_nxt: Function not yet avail.");
    return -1;


  L_noInt:
    TX_Error("AP_cont_nxt: Intersection with obj Typ %d not yet impl.",new_typ);
    return -1;

}


//==============================================================================
  int AP_cont_nxPT (ObjGX *ox, Point *pt1, Memspc *ooDat) {
//==============================================================================

  ox->typ  = Typ_PT;
  ox->form = Typ_PT;
  ox->siz  = 1;
  ox->data = UME_save (ooDat, pt1, sizeof(Point));

  return 0;

}
*/

//====================== EOF ===========================
