// ut_ccv.c                   RF                     2003-08-11
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
2003-08-11 Neu.  RF.

-----------------------------------------------------
*/
/*!
\file  ../ut/ut_ccv.c
\brief CCV = Contour functions 
\code
=====================================================
List_functions_start:

// UT3D_ccv_contour           create Obj CCV from contours
UT3D_vcn_ccv               Normalvector an (planare) Curve

UT3D_obj_ccv_segnr         get obj (typ,data) from segment of CCV.
UT3D_pt_ccv_segnr_par1     get point from segment-nr and parameter on CCV
UT3D_2pt_ccv1_lim          get start- or endpoint of trimmed-curve
// UT3D_ln_ccv_pt             make line from trimmed-curve and previous curve

UT3D_segpar_ccvSel         give segmentnr & parameter for point on ccv

UT3D_seg_objSel            Segmentnummer am Polygon liefern
UT3D_seg_dboSel            Segmentnummer am Polygon liefern
UT3D_segpar_dboSel         den selektieren Punkt am obj feststellen
UT3D_segpar_iSeg           get segNr iSeg from iTab

// UT3D_pt_segparccv          Punkt <-- segNr und Paramter auf CCV
UT3D_obj_segccv            get ObjGX from segNr in CCV
// UT3D_crv_segccv            get ObjGX + data from segment in CCV
// UT3D_stru_segccv           copy struct(LN/AC)  <-- segNr in CCV

UT3D_pt_endptccv           proj PT -> PolygonCurve

UT3D_segpar_iTab           intern

UT3D_pt_segparlna          point <-- segNr & parameter on lines[] (Typ_CVLNA)

List_functions_end:
=====================================================
- see also:
../xa/xa_cvcomp.c  APT_decode_cvco_add

\endcode *//*----------------------------------------


- unused functions:
cn_PnPoly                  crossing number test for a point in a polygon
wn_PnPoly                  winding number test for a point in a polygon


- missing functions:
UT3D_pt_projptccv
UT3D_pt_intlnccv
UT3D_pt_intciccv
UT3D_pt_intelccv
UT3D_pt_intbspccv
UT3D_pt_int2ccv

UT3D_pt_evalccv
UT3D_vc_evalccv

UT3D_ccv_oTab
UT3D_ccv_projccvpln

?
UT3D_sr_polc
SUStess_ck_srot_


?
 v0, v1: jedes Element liegt in Zahlenbereich 0-1 und hat ganzzahligen
  Offset. 0-1 ist Objekt 1, 1-2 ist Objekt 2 usw. 2.5 ist also in der Mitte des
  Objekts Nr. 3.
  Ganzahl weg: d = floor(d)


==============================================================
*/



#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




#include "../ut/ut_geo.h"
#include "../ut/ut_cast.h"                // INT_PTR

#include "../gr/ut_UI.h"               // UI_Func... SYM_..

#include "../db/ut_DB.h"               // DB_GetCurv

#include "../xa/xa_mem.h"              // memspc55



/*
//=======================================================================
  int UT3D_ccv_contour (ObjGX *ocv, CurvCCV *ccv, ObjGX *oTab, int oNr) {
//=======================================================================
// UT3D_ccv_contour           create Obj CCV from contours

  // printf("UT3D_ccv_contour %d\n",oNr);

 
  ccv->segNr = oNr;
  ccv->cvtab = oTab;
  ccv->v0    = 0.;
  ccv->is0   = 0;
  ccv->v1    = 1.;
  ccv->is1   = oNr - 1;


  //------------------------------------------------
  ocv->typ   = Typ_CVCCV;
  ocv->form  = Typ_CVCCV;
  ocv->siz   = 1;
  ocv->data  = ccv;


  return 0;

}
*/


//================================================================
  int UT3D_obj_ccv_segnr (int *typ, void *obj, int is, ObjGX *cv1) {
//================================================================
/// \code
/// UT3D_obj_ccv_segnr             get obj (typ,data) from segment of CCV.
/// Returns only curves; points are returned as lines.
/// Input:
///   cv1    ObjGX with typ=Typ_CVCCV
///   is     subCurve-nr; 0=first.
/// Output:
///   typ
///   obj    struct is typ; size must be OBJ_SIZ_MAX
/// \endcode

  int      irc;
  CurvCCV  *cca, *cc1;


  // printf("UT3D_obj_ccv_segnr is=%d\n",is);
  // UT3D_stru_dump (Typ_ObjGX, cv1, " _pt_segparccv: ");


  if(cv1->form != Typ_CVCCV) {
    TX_Error("UT3D_obj_ccv_segnr EI_%d_%d",cv1->typ,cv1->form);
    return -1;
  }

  if(is >= cv1->siz) {
    TX_Error("UT3D_obj_ccv_segnr MOD_%d_%d",is,cv1->siz);
    return -1;
  }


  // get segment nr <is> as curve-data (change point -> line)
  cca = (CurvCCV*)cv1->data;
  cc1 = &cca[is];
    // UT3D_stru_dump (Typ_CVCCV, cc1, " cc1: ");

/*
  //----------------------------------------------------------------
  if(cc1->typ == Typ_PT) {
    // make line from cc1 and previous curve
    if(is < 0) {TX_Error("UT3D_obj_ccv_segnr E001"); return -1;}
    irc = UT3D_ln_ccv_pt ((Line*)obj, &cca[is - 1], &cca[is]);
    if(irc < 0) return -1;
    *typ = Typ_LN;
    goto L_done;
  }
*/

  //----------------------------------------------------------------
  // get binary obj from trimmedCurve
  *typ = cc1->typ;
  irc = UTO_cv_cvtrm (typ, obj, NULL, cc1);
  if(irc < 0) {
    TX_Error("UT3D_obj_ccv_segnr E002_%d",cc1->typ);
    return -1;
  }



  //----------------------------------------------------------------
  L_done:
    // UT3D_stru_dump (*typ, obj, "ex UT3D_obj_ccv_segnr: ");
  return 0;

}
 
/*
//=========================================================================
  int UT3D_crv_segccv (int *typ, ObjGX *oxo, int is, ObjGX *cv1, Line *ln) {
//=========================================================================
// UT3D_crv_segccv           get ObjGX + data from segment in CCV
// YOU MUST provide the space for a line; leider hat CCV manchmal einen Punkt;
// ACHTUNG: do not use ln; use always oxo->data !!!
// Es wird eine Line konstruiert und via data uebergeben.

// see also UT3D_obj_segccv

  int      irc, oTyp;
  void     *oDat;
  Point    *pp;


  // printf("UT3D_crv_segccv %d\n",is);
  // UT3D_stru_dump (Typ_ObjGX, cv1, " _pt_segparccv: ");
  // UTO_dump__ (cv1, " _crv_segccv: ");


  irc = UT3D_obj_segccv (oxo, is, cv1);
  if(irc < 0) return -1;


  // In der CCV-struct steht manchmal eine Line manchmals als Typ_LN aber auch
  // als Typ_PT !!
  if(oxo->typ == Typ_PT) {

    // den Endpunkt laden ..
    UTO_obj_getp (&pp, oxo);
    ln->p2 = *pp;
      // UT3D_stru_dump (Typ_PT, &ln->p2, " endPt: ");

    // es ist ein (End)Punkt; den Startpunkt dazu beschaffen.
    irc = UT3D_obj_segccv (oxo, is-1, cv1);
    if(irc < 0) return -1;
    if(oxo->typ == Typ_PT) {
      UTO_obj_getp (&pp, oxo);
      ln->p1 = *pp;

    } else {
      // data des objekts beschaffen ..
      oTyp = UTO_obj_getp (&oDat, oxo);
      // den Endpunkt des objekts beschaffen ..
      irc = UTO_2pt_limstru (NULL, &ln->p1, NULL, NULL, oTyp, oDat);
      if(irc < 0) return -1;
    }
      // UT3D_stru_dump (Typ_PT, &ln->p1, " startPt: ");
    oxo->typ  = Typ_LN;
    oxo->form = Typ_LN;
    oxo->siz  = 1;
    oxo->data = ln;
  }

  // UTO_dump__ (oxo, "ex _crv_segccv:");

  *typ = oxo->typ;

  return 0;
}


//================================================================
  int UT3D_obj_segccv (ObjGX *oxo, int is, ObjGX *cv1) {
REPLACED by UT3D_obj_ccv_segnr
//================================================================
// UT3D_obj_segccv             ObjGX  <-- segNr in CCV
// copy segment nr <is> of CCV <cv1> -> oxo
// Input:
//   is         nr of segment in cv1;   0=first segment !

// kopiert ein ObjGX --> oxo !
// see also UT3D_crv_segccv

  int      oTyp;
  long     ind;
  ObjGX    *oTab, *oSeg, ox1;

  // printf("UT3D_obj_segccv %d\n",is);
  // UTO_dump__ (cv1, "cv1:\n");

  // das erste Seg ist immer ein Punkt; in diesem Fall sofort
  // das folgende liefern ..
  // if(is == 0) is = 1;

  // segment iseg aus cv1 extrahieren
  // oTab = cv1->data;
  // oTab = ((CurvCCV*)cv1->data)->cvtab;
  oTab = cv1->data;
  oSeg = &oTab[is];
    // printf(" typ=%d form=%d\n",oSeg->typ,oSeg->form);
    // UTO_dump__ (oSeg, "seg:\n");


  // if(oSeg->typ == Typ_PT) {
    // // make a Line from the previous point to this point
    // // line als dynam. Line zwischenspeichern ?
      // printf("  line from point ..\n");
    // // get endpoint of previous segment
    // // UTO_obj_getp ();
    // // UTO_2pt_limstru ();
    // // get endpoint
    // // UTO_obj_getp
    // // line -> oxo           UTO_stru_2_obj
    // oxo->typ = Typ_LN;
    // oxo->form = Typ_LN;
    // oxo->siz  = 1;
    // oxo->data  = 1;

  // } else if(oSeg->form == Typ_Index) {

  if(oSeg->form == Typ_Index) {
    ind = LONG_PTR (oSeg->data);
    // printf(" typ=%d ind=%d\n",oSeg->typ, ind);
    *oxo = DB_GetObjGX (oSeg->typ, ind);

  } else {
    // oTyp = UTO_obj_getp (&od, oSeg);
    // if(oTyp < 0) return -1;
    TX_Error("UT3D_obj_segccv E001");
    return -1;
  }

  // UT3D_stru_dump (Typ_ObjGX, oxo, "ex UT3D_obj_segccv");

  return 0;

}


//================================================================
  int UT3D_stru_segccv (void *data, int typ, int is, ObjGX *cv1) {
//================================================================
// UT3D_stru_segccv           copy struct(LN/AC)  <-- segNr in CCV

// see UTO_obj_get

  int      irc;
  ObjGX    ox1;


  // printf("UT3D_stru_segccv %d %d\n",typ,is);


  irc = UT3D_obj_segccv (&ox1, is, cv1);
  if(irc < 0) return -1;

  // UT3D_stru_dump (Typ_ObjGX, &ox1, "  _ci_segccv ox1:\n");


  if(ox1.form != typ) {
    TX_Error("UT3D_stru_segccv E001 %d %d",ox1.typ,ox1.form);
    return -1;
  }


  // hier koennte man ev UTO_obj_get benutzen .....
  if(typ == Typ_LN) {
    *(Line*)data = *((Line*)ox1.data);  // copy the circ

  } else if(typ == Typ_CI) {
    *(Circ*)data = *((Circ*)ox1.data);  // copy the circ

  } else {
    TX_Error("UT3D_stru_segccv E002 %d %d %d",ox1.typ,ox1.form,typ);
    return -1;
  }


  return 0;

}


//================================================================
  int UT3D_seg_objSel (int *is, double *ps, ObjGX *cv1) {
//================================================================
// Segmentnummer am Polygon liefern.

// see UT3D_segpar_dboSel

// ACHTUNG:   memspc55 used !

// iTab must have size of CCV (number of objects in CCV)
#define iTabSiz   500


  int      irc, ptNr, i1, iNr, iTab[iTabSiz];
  Point    *pTab;


  UT3D_stru_dump (Typ_ObjGX, cv1, "UT3D_seg_objSel\n");


  pTab = (void*)memspc55;
  ptNr = sizeof(memspc55) / sizeof(Point);


  if(cv1->form != Typ_CVPOL) {
    TX_Print("UT3D_seg_objSel E001 %d %d",cv1->typ,cv1->form);
    return -1;
  }



    // create polygon for obj
    irc = UT3D_npt_ox (&ptNr, pTab, cv1, UT_DISP_cv*2.);
    if(irc < 0) return -1;
    // iTab besetzen.
    iTab[0] = 0;
    iTab[0] = ptNr - 1;
    iNr = 2;
    if(cv1->form == Typ_CVPOL) {
      if(ptNr < 100) {        // keine zu langen Polygone ... (ptNr==oxi.siz)
        for(i1=0; i1<ptNr; ++i1) iTab[i1] = i1;
        iNr = ptNr;
      }
    }


  // VertexNr und offset am segment suchen ...
  // den selektieren Punkt am Polygon feststellen (segmentNr und Abstand)
  GL_selPt (is, ps, ptNr, pTab, iNr, iTab);

  //----------------------------------------------------------------
  L_done:
  printf("ex UT3D_seg_objSel %d %f\n",*is,*ps);

  return 0;

}
*/ 

/* DZT UNUSED
//================================================================
  int UT3D_seg_dboSel (int *is, int typ, long ind) {
//================================================================
// Segmentnummer am Polygon liefern.
// RetCod gives basicObjectTyp (if typ == CCV).

// see UT3D_segpar_dboSel

// ACHTUNG:   memspc55 used !

// iTab must have size of CCV (number of objects in CCV)
#define iTabSiz   500


  int      irc, btyp, ptNr, i1, iSeg, iNr, iTab[iTabSiz];
  void     *vp;
  double   ps;
  Point    *pTab, spt;
  Line     ln1;
  ObjGX    oxi, ox1;

  // printf("UT3D_seg_dboSel %d %d\n",typ,ind);


  // vom DB-Obj typ/ind ein Polygon generieren;
  // vom DB-Obj ist ein ObjGX-Obj erforderlich
  oxi = DB_GetObjGX (typ, ind);
  if(oxi.typ == Typ_Error) {
    TX_Print("UT3D_seg_dboSel E001 %d %d",typ,ind);
    return -1;
  }
    // printf(" typ=%d form=%d\n",oxi.typ,oxi.form);


  if(oxi.form != Typ_CVPOL) {
    TX_Print("UT3D_seg_dboSel E002 %d %d",typ,ind);
    return -1;
  }

  return UT3D_seg_objSel (is, &ps, &oxi);

}


//====================================================================
  int UT3D_segpar_sel_LN (int *is, int *pSel, double *ps, Line *ln1) {
//====================================================================
// Output:
//   is       1=ln.p1 selected; 2=ln.p2 selected;
//   ps       parameter0-1 of active mousePos on line;
//   pSel     0=p1|p2 selected;



  int     i1;
  double  ls, ll;
  Point   actScrPos, actUsrPos;


  UT3D_stru_dump (Typ_LN, ln1, "UT3D_segpar_sel_LN");


  // test if p1 selected
  i1 = GL_ck_sel_PT (&ln1->p1);
  if(!i1) {
    *pSel = 0;
    *is = 1;
    goto L_done;
  }

  // test if p2 selected
  i1 = GL_ck_sel_PT (&ln1->p2);
  if(!i1) {
    *pSel = 0;
    *is = 2;
    goto L_done;
  }
    

  // get mouse-position in screen and user-coordinates
  // actScrPos = GL_actScrPos = ?
  // actUsrPos = GL_actUsrPos = ?
  GL_GetActSelPos (&actUsrPos, &actScrPos);
    UT3D_stru_dump (Typ_PT, &actUsrPos, " actUsrPos");
    UT3D_stru_dump (Typ_PT, &actScrPos, " actScrPos");

  // length of line in 2D
  ll = UT3D_len_2pt (&ln1->p1, &ln1->p2);

  // get length ln.p1 - ptSel
  // ls = UT3D_len_2pt (&ln1->p1, &actUsrPos);
  ls = UT3D_slen_3pt (&ln1->p1, &ln1->p2, &actUsrPos);
    printf(" lenLn=%lf lenSel=%lf\n",ll,ls);

  *ps = ls / ll;

  L_done:
    printf("ex UT3D_segpar_sel_LN is=%d pSel=%d ps=%lf\n",*is,*pSel,*ps);

  return 0;

}


//============================================================================
  int UT3D_segpar_dboSel (int *pSel, int *is, int *isNr, double *ps,
                          int typ, long ind) {
//============================================================================
// den selektieren Punkt an einem Obj feststellen (segmentNr und Abstand)
// ACHTUNG: Punkt & Sichtstrahl werden via GL_GetEyeLn() geholt !
//
// Line|Circ|Bspl: wenn ein LimitPoint selektiert wurde, wird auch die PointNr
//   ausgegeben (Line|Circ: 1=StartPt; 2=Endpt; Bspl: Endpt=last CtrlPt!).
//
// Polygon: wenn kein LimitPoint selektiert wurde: Segmentnummer liefern.
//   else PointNr liefern.  (SegNr=1 ist des erste Segment).
//
// Line|Circ|Bspl in CCV: Segmentnummer und subSegmentnummer oder
//   Segmentnummer und PointNr liefern.
//
// Wenn ein Punkt selektiert wird, wird die Punktnummer als pSel ausgegeben.
//   pSel=index into pTab, 0=first; is[1|2]=pointNr: 1=first.
//
// Input:
//   typ,ind     DB-object;
// Output:
//   pSel    -1  no limitPoint (Startpt|Endpt|plgPoint) selected
//          >=0  Startpt|Endpt|plgPoint selected; pSel=pointIndex.
//   is[2]    LN|AC|Bspl|Elli:
//              pSel=-1: isNr=1; is[0]=1;
//              pSel>=0: isNr=1; is[0]=pointNr;
//            Plg:
//              pSel=-1: isNr=1; is[0]=segNr;
//              pSel>=0: isNr=1; is[0]=pointNr;
//            LN|AC|Bspl|Elli in CCV:
//              pSel=-1: isNr=2;is[0]=segNr;is[1]=1;
//              pSel>=0: isNr=2;is[0]=segNr;is[1]=pointNr;
//            Plg in CCV:
//              pSel=-1: isNr=2; is[0]=segNr; is[1]=subSegNr;
//              pSel>=0: isNr=2; is[0]=segNr; is[1]=pointNr;
//   isNr     nr of segNr's in is; 1|2
//   ps       parameter on curve
//   RetCod   gives basicObjectTyp (if typ == CCV).


// ACHTUNG:   memspc102 & memspc55 used !

// OFFEN: - damit CCV beliebeig tief verschachtelt sein kann, sollte is
//           ein array sein !
//        - lPar erst nachtraeglich aus spt errechnen;
//           Output: oxi & spt statt ps.

// iTab must have size of CCV (number of objects in CCV)


  int      irc, ptNr, sTyp, i1, i2, i3, iSeg, iPt, is1, is2, iNr;
  int      iTabSiz, *iTab;
  void     *vp;
  Point    *pTab, spt, pt1;
  Vector   vc1;
  Line     ln1;
  ObjGX    oxi, ox1;


  printf("UT3D_segpar_dboSel typ=%d ind=%ld\n",typ,ind);

  // btyp = AP_typ_2_bastyp (typ);
  sTyp = typ;

  *ps   = 0.;

  iTab = (void*)memspc55;
  iTabSiz = sizeof(memspc55) / sizeof(int);

  pTab = (void*)memspc102;
  ptNr = sizeof(memspc102) / sizeof(Point);

    // printf(" ptNr=%d iTabSiz=%d\n",ptNr,iTabSiz);


  // vom DB-Obj typ/ind ein Polygon generieren;
  // vom DB-Obj ist ein ObjGX-Obj erforderlich
  oxi = DB_GetObjGX (typ, ind);
  if(oxi.typ == Typ_Error) {
    TX_Print("UT3D_segpar_dboSel E001 %d %d",typ,ind);
    return -1;
  }
    // printf(" oxi-typ=%d form=%d\n",oxi.typ,oxi.form);
  

  if(typ == Typ_LN) {
    // rays with (length==1) need special handling
    irc = UT3D_segpar_sel_LN (is, pSel, ps, oxi.data);
    goto L_done;


  // das Polygon der curve erzeugen.
  } else if((typ == Typ_CV)&&(oxi.typ == Typ_CVCCV)) {
    // das Polygon der selektierten CCV holen UND segmnetIndexTable iTab !
    iNr = iTabSiz;
    UT3D_pta_ccv (&ptNr, pTab, &oxi, iNr, iTab, UT_DISP_cv*2.);
      // GR_Disp_pTab (ptNr, pTab, SYM_STAR_S, 3);
      // i1=0;while(iTab[i1] >= 0) {printf(" XsY[%d] %d\n",i1,iTab[i1]);++i1;}

  

  } else if((typ == Typ_CV)&&(oxi.typ == Typ_CVCLOT)) {
    // get Sichtlinie
    GL_GetEyeLn (&pt1, &vc1); // Sichtstrahl (Point & Vector) retournieren
    ptNr = 1;
    UT3D_pt_intclotptvc (&ptNr, &spt, ps, &pt1, &vc1, oxi.data, UT_DISP_cv);
    *pSel = -1; // 1=segment selected.
    *is = 0;
    goto L_done;



  } else if((typ == Typ_CV)&&(oxi.typ == Typ_CVLNA)) {
    *pSel = -1;    // no limitPoint
    *isNr = 1;     // nr of segNr's in is
    // get Sichtlinie
    GL_GetEyeLn (&pt1, &vc1); // Sichtstrahl (Point & Vector) retournieren
    // get segmentNr and pointParameter of point on lineArray
    ULN3_segNr_par_prj_pt_nln (&is[0], ps, &pt1, oxi.siz, oxi.data);
    sTyp = Typ_LN;
    goto L_done;



  } else {
    // create polygon for obj
    irc = UT3D_npt_ox (&ptNr, pTab, &oxi, UT_DISP_cv*2.);
    if(irc < 0) return -1;
    sTyp = Typ_LN;

    // iTab besetzen.
    UT3D_segpar_iTab (&iNr, oxi.typ, iTab, iTabSiz, ptNr);
  }



  //----------------------------------------------------------------
  // VertexNr und offset am segment suchen ...
  //----------------------------------------------------------------
  // den selektieren Punkt am Polygon feststellen (segmentNr und Abstand)
  irc = GL_selPt (is, pSel, &spt, ptNr, pTab, iNr, iTab);
  if(irc < 0) return -1;
    printf(" is=%d pSel=%d typ=%d\n",*is,*pSel,typ);


  typ = oxi.typ;
  *isNr = 1;
    printf(" sel-l0: typ=%d is=%d\n",typ,is[0]);
    UT3D_stru_dump (Typ_PT, &spt, "  spt:\n");



  //----------------------------------------------------------------
  if(typ != Typ_CVCCV) goto L_not_CCV;
  //----------------------------------------------------------------
  // handle CCV.

  // decode segNr
  is1 = is[0];
    printf(" CCV; is1=%d\n",is1);


  // get typ of selectd subObj (copy segment of CCV oxi -> ox1)
  irc = UT3D_obj_segccv (&ox1, is1-1, &oxi);
  if(irc < 0) return -1;
  sTyp = ox1.form;
    printf(" CCV: typ=%d form=%d\n",typ,ox1.typ,ox1.form);


  // test if point of subSeg was selected ...
  // Out:is2;  In: subSeg; spt=point;
  if(is1 > 1) i1 = iTab[is1-2];       // 1.seg. = 1 = 0 - iTab[0]
  else        i1 = 0;
  i2 = iTab[is1-1];
  i3 = i2 - i1 + 1;   // nr of points; test EndPoint also ..
    // printf(" i1=%d i2=%d\n",i1,i2);
 
  // fix iTab for seg ox1;
  UT3D_segpar_iTab (&iNr, ox1.typ, iTab, iTabSiz, i3);

  // is2 = GL_selSubPt (iNr, &pTab[i1], &spt);
    // printf(" is2=%d\n",is2);
  irc = GL_selPt (&iSeg, &iPt, &spt, i3, &pTab[i1], iNr, iTab);
    // printf(" -selPt irc=%d iSeg=%d iPt=%d\n",irc,iSeg,iPt);
    // UT3D_stru_dump (Typ_PT, &spt, "  spt = ");


  *isNr = 2;          // size of is
  is[1] = iSeg;       // subSegNr / pointNr
    // printf(" CCV-l0: typ=%d sTyp=%d is=%d,%d\n",typ,sTyp,is[0],is[1]);

  // // get ps=parameter of point spt on curve
  // irc = UTO_parpt_pt_obj (ps, &spt, ox1.form, ox1.data);
    // printf(" -parpt_objpt irc=%d ps=%f\n",irc,*ps);
  // if(irc < 0) return -1;
  // goto L_done;


  //----------------------------------------------------------------
  // Line in CCV selected
  if(sTyp == Typ_LN)      {
    oxi = ox1;         // for compute lPar
    if(iPt < 0) goto L_done;
      // printf(" -LN: iPt=%d\n",iPt);
    if(iPt == 1) {   // endPt sel
      *pSel = iPt;
      is[1] = iPt + 1;   // MOD = index + 1
    }

    goto L_noCCV2;
  }


  //----------------------------------------------------------------
  // Polygon in CCV selected
  if(sTyp == Typ_CVPOL) {

    irc = UTO_parpt_pt_obj (ps, &spt, sTyp, ox1.data);
      // printf(" -parpt_objpt irc=%d ps=%f\n",irc,*ps);
    if(irc < 0) return -1;

    if(iPt < 0) goto L_done;


    // printf(" ps=%lf v0=%lf v1=%lf\n",*ps,
           // ((CurvPoly*)ox1.data)->v0,
           // ((CurvPoly*)ox1.data)->v1);

    // test if point is outside polygon
    irc = UT3D_ck_plgpar1lim(ox1.data, *ps);
    if(irc) {
        // printf(" _plgpar1lim %d\n",irc);
      // if(irc == -1) {
         // is[1] = iPt + 1;   // next subSeg
      *pSel = -1;
      goto L_done;
    }


    *pSel = iPt;
    // statt segNr pointNr ausgeben
    is[1] = iPt + 1;   // MOD = index + 1
    goto L_done;

  }


  //----------------------------------------------------------------
  // L|C|Bspl.. in CCV selected
  oxi = ox1;         // for compute lPar

  // handle point-selection
  if(iPt >= 0) {    // SegEndpt einer CCV selektiert;
      // printf(" _CCV-iSeg=%d iPt=%d\n",iSeg,iPt);

    // do not accept pointNr in Circs
    if((sTyp == Typ_CI)     ||
       (sTyp == Typ_CVELL)  ||
       (sTyp == Typ_CVBSP))    {
      *pSel = -1;
      goto L_noCCV2;  // get lpar
    }

    *pSel = iPt;
    // statt segNr pointNr ausgeben
    is[1] = iPt + 1;   // MOD = index + 1

*
    // if(iPt > 2) {
      // // Bspl must have ControlpointNr; get & replace
      // if(ox1.typ == Typ_CVBSP) {
        // is[1] = ((CurvBSpl*)ox1.data)->ptNr;
      // }
    // }
*
    is[1] = 1;
    goto L_done;

  }

  // no Point of L|C|Bspl.. in CCV selected
  goto L_noCCV2;



/
  //----------------------------------------------------------------
  // handle CCV.
  // selection between endpoints

    // printf(" CCV: is=%d ps=%f\n",*is,*ps);

  // das subelement aus der CCV holen
  // leider hat CCV manchmal einen Punkt; zur Offsetberechnung ist aber eine
  // Line erforderlich.
  // Es wird eine Line konstruiert.
  // irc = UT3D_crv_segccv (&typ, &ox1, is1, &oxi, &ln1);
  // if(irc < 0) return -1;

  // oxi = ox1;
    // UT3D_stru_dump (Typ_ObjGX, &oxi, "CCV-Seg:\n");
  // typ = AP_typ_2_bastyp (oxi.typ);
  // typ = oxi.typ;   // wird zum erkennen von CVPOL gebraucht ..

  // goto L_noCCV2;
/



  //----------------------------------------------------------------
  L_not_CCV:
  //----------------------------------------------------------------
  // handle Polygon
  if(oxi.typ != Typ_CVPOL) goto L_not_POL;


  if(*pSel >= 0) {       // Point of Plg selected
    // statt segNr pointNr ausgeben
    is[0] = *pSel + 1;   // MOD = index + 1
    goto L_done;
  }


  // no Point of Plg selected
  goto L_noCCV2;






  //----------------------------------------------------------------
  L_not_POL:
  //----------------------------------------------------------------
  // handle L|C|Bspl..
  if(*pSel >= 0) {           // Point of L|Curve selected
      // printf(" _not_POL-pSel=%d %d %f\n",*pSel,*is,*ps);

    // statt segNr pointNr ausgeben
    is[0] = *pSel + 1;   // MOD = index + 1

    if(is[0]  > 1) {
      is[0] = 2;        // eg Endpoint of circ.
      // Bspl must have ControlpointNr; get & replace
      if(oxi.typ == Typ_CVBSP) {
       is[0] = ((CurvBSpl*)oxi.data)->ptNr;
      }
    }

    goto L_done;
  }

  // no Point of L|C|Bspl.. in CCV selected
  is[0] = 1;          // subSegNr
  // goto L_noCCV2;





  //----------------------------------------------------------------
  L_noCCV2:
  // get ps=parameter of point spt on curve
  irc = UTO_parpt_pt_obj (ps, &spt, oxi.form, oxi.data);
    // printf(" -parpt_objpt irc=%d ps=%f\n",irc,*ps);


  if(irc < 0) return -1;



  //----------------------------------------------------------------
  L_done:

    // printf("ex UT3D_segpar_dboSel typ=%d pSel=%d is[0]=%d ps=%f\n",
           // sTyp,*pSel,*is,*ps);
    // if(*isNr > 1) printf(" is[1]=%d\n",is[1]);
    // printf("____________________________________________ \n");
    // printf("____________________________________________ \n");
    // exit(0);

  return sTyp;

}
*/

//============================================================================
  int UT3D_segpar_iTab (int *iNr, int typ, int *iTab, int tabSiz, int ptNr) {
//============================================================================

  int  i1;

  // printf("UT3D_segpar_iTab %d\n",ptNr);

  // iTab besetzen.
  if(typ == Typ_CVPOL) {
/*
    if(tabSiz <= ptNr) {
      TX_Print("UT3D_segpar_iTab EOM");
      goto L_simple;
    }
    for(i1=0; i1<ptNr-1; ++i1) iTab[i1] = i1+1;
    iTab[ptNr] = -1;
    *iNr = ptNr;
*/
    *iNr = -1;
    return 0;
  }


  L_simple:
/*
    iTab[0] = ptNr - 1;
    iTab[1] = -1;
    *iNr = 1;
*/
    *iNr = -2;


  return 0;

}


//================================================================
  int UT3D_segpar_iSeg (int itn, int *iTab) {
//================================================================
// decode segmentNr from iTab and iTab-segmentNr.

  int   iSeg;

  iSeg=0;

  while(iTab[iSeg] >= 0) {
    if(iTab[iSeg] >= itn) goto L_exit;
      // printf(" seg %d  pNr %d\n",iSeg,iTab[iSeg]);
    ++iSeg;
  }
  TX_Error("UT3D_segpar_iSeg E001"); return -1;

  L_exit:
    // printf("ex UT3D_segpar_iSeg %d %d\n",iSeg,itn);

  return iSeg;

}
 

/*
//==================================================================
  int UT3D_segpar_ccvSel (int *is, double *ps, ObjGX *cv1) {
//==================================================================
// UT3D_segpar_ccvSel   give segmentnr & parameter for point on ccv

// Funktionsweise: Es wird (nochmal) das Polygon der CCV errechnet;
//   dabei werden aber auch die Indices der Segmentgrenzen mitgeliefert.
//   dann wird der dem p1 naechstliegende Polygonpunkt gesucht ..

// ACHTUNG:   memspc55 used !

// iTab must have size of CCV (number of objects in CCV)
#define iTabSiz   500

  int      ii, iSeg, ptNr, iNr, iTab[iTabSiz];
  Point    *pTab, spt;


  // UT3D_stru_dump (Typ_PT, p1, "UT3D_segpar_ccvSel ");


  pTab = (void*)memspc55;
  ptNr = sizeof(memspc55) / sizeof(Point);


  // das Polygon der selektierten CCV holen UND segmnetIndexTable iTab !
  iNr = iTabSiz;
  UT3D_pta_ccv (&ptNr, pTab, cv1, iNr, iTab, UT_DISP_cv);
    // GR_Disp_pTab (ptNr, pTab, SYM_STAR_S, 3);
    // i1=0;while(iTab[i1] >= 0) {printf(" [%d] %d\n",i1,iTab[i1]);++i1;}


  // den selektieren Punkt am Polygon feststellen (segmentNr und Abstand)
  GL_selPt (&ii, ps, ptNr, pTab);




  //----------------------------------------------------------------
  // das segment (iSeg) feststellen
  iSeg=0;
  while(iTab[iSeg] >= 0) {
    if(iTab[iSeg] >= ii) goto L_seg;
      // printf(" seg %d  pNr %d\n",iSeg,iTab[iSeg]);
    ++iSeg;
  }
  TX_Error("UT3D_segpar_ccvSel E001"); return -1;


  L_seg:
    printf(" seg=%d\n",iSeg);



  //----------------------------------------------------------------
  L_done:
  printf("ex UT3D_segpar_ccvSel seg=%d par=%f\n",iSeg,*ps);

  *is = iSeg;

  return 0;

}
*/

/*
//==================================================================
  int UT3D_segpar_ptccv (int *is, double *ps, ObjGX *cv1, Point *p1) {
//==================================================================
// UT3D_segpar_ptccv   give segmentnr & parameter for point on ccv

// Funktionsweise: Es wird (nochmal) das Polygon der CCV errechnet;
//   dabei werden aber auch die Indices der Segmentgrenzen mitgeliefert.
//   dann wird der dem p1 naechstliegende Polygonpunkt gesucht ..

// ACHTUNG:   memspc55 used !

// iTab must have size of CCV (number of objects in CCV)
#define iTabSiz   500

  int      i1, ii, iSeg, ptNr, iNr, iTab[iTabSiz], oTyp;
  double   d1;
  void     *od;
  Point    *pTab;
  ObjGX    *oTab, *oSeg;


  // UT3D_stru_dump (Typ_PT, p1, "UT3D_segpar_ptccv ");


  pTab = (void*)memspc55;
  ptNr = sizeof(memspc55) / sizeof(Point);


  iNr = iTabSiz;
  UT3D_pta_ccv (&ptNr, pTab, cv1, iNr, iTab, UT_DISP_cv);
    // GR_Disp_pTab (ptNr, pTab, SYM_STAR_S, 3);
    // i1=0;while(iTab[i1] >= 0) {printf(" [%d] %d\n",i1,iTab[i1]);++i1;}


  ii = UT3D_ipt_cknear_npt (p1, pTab, ptNr);
    printf(" ii=%d\n",ii);


  if(ii == 0) {
    iSeg = 0;       // Line after Startpoint
    d1 = 0.;
    goto L_done;
  }



  //----------------------------------------------------------------
  // das segment (iSeg) feststellen
  iSeg=0;
  while(iTab[iSeg] >= 0) {
    if(iTab[iSeg] >= ii) goto L_seg;
      // printf(" seg %d  pNr %d\n",iSeg,iTab[iSeg]);
    ++iSeg;
  }
  TX_Error("UT3D_segpar_ptccv E001"); return -1;


  L_seg:
    printf(" seg=%d\n",iSeg);




  //----------------------------------------------------------------
  // den Paramter aus seg iSeg feststellen

  // segment iseg aus cv1 extrahieren
  oTab = cv1->data;
  oSeg = &oTab[iSeg];
    // printf(" typ=%d form=%d\n",oSeg->typ,oSeg->form);
    // UTO_dump__ (oSeg, "seg:\n");

  oTyp = UTO_obj_getp (&od, oSeg);
  if(oTyp < 0) return -1;

  // get Parameter from object
  if(oTyp == Typ_PT) d1 = 0.;
  else UTO_parpt_pt_obj (&d1, p1, oTyp, od);

  if(fabs(d1-1.) < UT_TOL_min0) d1 = 1.;



  //----------------------------------------------------------------
  L_done:
  // printf("ex UT3D_segpar_ptccv seg=%d par=%f\n",iSeg,d1);

  *is = iSeg;
  *ps = d1;

  return 0;

}
*/

      
      
//====================================================================
  int UT3D_pt_segparlna (Point *p1, int is, double ps, ObjGX *cv1) {
//====================================================================
// UT3D_pt_segparlna      point <-- segNr & parameter on lines[] (Typ_CVLNA)

// see UTO_pt_par1_dbo
// see UT3D_obj_segccv
        
  int      irc, typ;
  void     *data;
  Point    *pa, *pe, pta;
  Line     *ln1;
  ObjGX    ox1;

    
  // printf("UT3D_pt_segparlna %d %f\n",is,ps);
  // UT3D_stru_dump (Typ_ObjGX, cv1, " _pt_segparlna: ");
  // UTO_dump__ (cv1, " _pt_segparlna: "); 


  // get line <is> out of curve
  if(is < 0) return -1;
  if(is >= cv1->siz) return -1;
  ln1 = &((Line*)cv1->data)[is];
    // UT3D_stru_dump (Typ_LN, ln1, " _pt_segparlna ln1:");


  // create paraemetric point on ln1
  return UT3D_pt_evpar2pt (p1, ps, &ln1->p1, &ln1->p2);

}


//================================================================
  int UT3D_2pt_ccv1_lim (Point *ps, Point *pe, CurvCCV *ccv) {
//================================================================
/// \code
// UT3D_2pt_ccv1_lim       get start- or endpoint of trimmed-curve
/// Input:
///   ps        startpoint (none if p1 == NULL)
///   pe        endpoint (none if p2 == NULL)
///
/// see also UTO_2pt_limstru
/// \endcode

  int       irc, typ;
  void      *vp1;
  char      cvo[OBJ_SIZ_MAX];


  UT3D_stru_dump (Typ_CVCCV, ccv, "UT3D_2pt_ccv1_lim: ");


  //----------------------------------------------------------------
  if(ccv->typ == Typ_PT) {
    vp1 = DB_get_PT (ccv->dbi);
    if(ps) *ps = *(Point*)vp1;
    if(pe) *pe = *(Point*)vp1;



  //----------------------------------------------------------------
  } else {
    typ = ccv->typ;
    irc = UTO_cv_cvtrm (&typ, cvo, NULL, ccv);
    if(irc < 0) {
      TX_Error("UT3D_2pt_ccv1_lim E001_%d",ccv->typ);
      return -1;
    }


    irc = UTO_2pt_limstru (ps, pe, NULL, NULL, typ, cvo);
    if(irc < 0) {
      TX_Error("UT3D_2pt_ccv1_lim E002_%d",ccv->typ);
      return -1;
    }
  }


  return 0;

}


/*
//================================================================
  int UT3D_ln_ccv_pt (Line *lno, CurvCCV *cc1, CurvCCV *cc2) {
//================================================================
// make line from cc1 and previous curve

  int     irc;


  printf("UT3D_ln_ccv_pt %d %d\n",cc1->typ,cc2->typ);



  // get endpoint of cc1
  irc = UT3D_2pt_ccv1_lim (NULL, &lno->p1, cc1);
  if(irc < 0) return -1;


  // get endpoint of cc2
  irc = UT3D_2pt_ccv1_lim (NULL, &lno->p2, cc2);
  if(irc < 0) return -1;

    
    UT3D_stru_dump (Typ_LN, lno, "ex UT3D_ln_ccv_pt");

  return 0;

}
*/

//====================================================================
  int UT3D_pt_ccv_segnr_par1 (Point *po, int is, double ps, ObjGX *cv1) {
//====================================================================
/// UT3D_pt_ccv_segnr_par1      get point from segment-nr and parameter on CCV

  int      irc, typ;
  // CurvCCV  *cca, *cc1;
  char      cvo[OBJ_SIZ_MAX];


  // printf("UT3D_pt_ccv_segnr_par1 is=%d par1=%lf\n",is,ps);
  // UT3D_stru_dump (Typ_ObjGX, cv1, " _pt_segparccv: ");


  // get typ,data from seg.<is> 
  irc = UT3D_obj_ccv_segnr (&typ, cvo, is, cv1);
  if(irc < 0) {
    TX_Error("UT3D_obj_ccv_segnr EI_%d_%d_%d",irc,cv1->typ,cv1->form);
    return -1;
  }


  //----------------------------------------------------------------
  // get point from parameter <ps>
  L_par:
  irc = UT3D_pt_evparcrv (po, ps, typ, cvo);

    // UT3D_stru_dump (Typ_PT, po, "ex UT3D_pt_ccv_segnr_par1");

  // exit(0);
  return irc;

}


/*
//====================================================================
  int UT3D_pt_segparccv (Point *p1, int is, double ps, ObjGX *cv1) {
//====================================================================
// REPLACED BY UT3D_pt_segparccv
// UT3D_pt_segparccv      Punkt <-- segNr und Paramter auf CCV

// see UTO_pt_par1_dbo
// see UT3D_obj_segccv

  int      irc, typ;
  void     *data;
  Point    *pa, *pe, pta;
  Line     ln1;
  ObjGX    ox1;


  // printf("UT3D_pt_segparccv %d %f\n",is,ps);
  // UT3D_stru_dump (Typ_ObjGX, cv1, " _pt_segparccv: ");
  // UTO_dump__ (cv1, " _pt_segparccv: ");



  // das subelement aus der CCV holen
  // leider hat CCV manchmal einen Punkt; zur Offsetberechnung ist aber eine
  // Line erforderlich.
  // Es wird eine Line konstruiert.
  irc = UT3D_crv_segccv (&typ, &ox1, is, cv1, &ln1);
  if(irc < 0) return -1;


  return UT3D_pt_evparcrv (p1, ps, ox1.form, ox1.data);

}
*/

//=======================================================================
  int UT3D_vcn_ccv (Vector *vco, Point *pto, ObjGX *oi, Memspc *wrkSpc) {
//=======================================================================
// UT3D_vcn_ccv         Normalvector an (planare) CCV
// Liefert auch den ersten Punkt der Kontur !
// Wenn CCV geschlossen: vco zeigt nach innen !!
// see also UT3D_ptvc_sus (Normalvec) braucht SurStd* !!
// see also APT_DrawCurv
// see also UT3D_pta_sus  braucht SurStd* !
// see also UT3D_npt_ox


  int    irc, ptNr, ibp, isr;
  Point  *ptTab;
  Plane  pl1;


  // UTO_dump__(oi, "UT3D_vcn_ccv:");


  if(oi->typ != Typ_CVCCV) return -1;

  // CCV -> 3D-Polygon umwandeln
  ptTab = (Point*)wrkSpc->start;
  ptNr  = UME_ck_free(wrkSpc) / sizeof(Point);
  irc = UT3D_npt_ox (&ptNr, ptTab, oi, UT_DISP_cv);
  // printf(" irc=%d ptNr=%d\n",irc,ptNr);
  if(irc < 0) return irc;

  // Plane from CCV: see GR_DrawSur
  // 3D-Polygon -> Plane
  UT3D_pl_pta (&pl1, ptNr, ptTab);
  *pto = pl1.po;
  *vco = pl1.vz;
  wrkSpc->start = ptTab;   // reset memspc

  // Normalvec entsprechend Drehsinn:
  ibp = UT3D_bp_vcz (&pl1.vz);
  isr = UT3D_sr_polc (ptNr-1, ptTab, &pl1.vz, ibp);
  // printf(" isr=%d\n",isr);
  if(isr < 0)  UT3D_vc_invert (vco, vco);

    // GR_Disp_vc (vco, pto, 2, 0);
  return irc;

}


//=========================================================================
  int UT3D_pt_endptccv (Point *pTab, ObjGX *ccv1) {
//=========================================================================
// UT3D_pt_endptccv           proj PT -> PolygonCurve
// liefert 2 Punkte; 0=Startpunkt, 1=Endpunkt.

  int    i1;
  ObjGX  *oxTab;


  printf("UT3D_pt_endptccv \n");
  UT3D_stru_dump (Typ_CVCCV, ccv1, "ccv1:\n");



  // oxTab = ((CurvCCV*)ccv1->data)->cvtab;
  // i1 = ((CurvCCV*)ccv1->data)->segNr;
  oxTab = ccv1->data;
  // i1 = ccv1->siz - 1;

/*
  // get last curve  // wouldnt be last point better ?
  i1 = ccv1->siz - 2;         // last record is the trim-record !!
  // if there is only 1 record:
  if(i1 < 0) i1 = 0;

  // UTO_2pt_lim_ox (&pTab[0], &oxTab[0],  1);    // 1=Anf.
  // UTO_2pt_lim_ox (&pTab[1], &oxTab[i1], 2);    // 2=End.
*/

  UTO_2pt_lim_ox (&pTab[0], &pTab[1], NULL, NULL, oxTab);

  return 0;

}



//=========================================================================
/* unused Functions:
//=========================================================================


//===================================================================
inline int isLeft( Point P0, Point P1, Point P2 ) {
//===================================================================

  return ( (P1.x - P0.x) * (P2.y - P0.y) - (P2.x - P0.x) * (P1.y - P0.y) );
}

//===================================================================
  int wn_PnPoly (Point P, Point* V, int n) {
//===================================================================
// wn_PnPoly(): winding number test for a point in a polygon
//      Input:   P = a point,
//               V[] = vertex points of a polygon V[n+1] with V[n]=V[0]
//      Return:  wn = the winding number (=0 only if P is outside V[])

    int    i, wn = 0;    // the winding number counter

    // loop through all edges of the polygon
    for (i=0; i<n; i++) {            // edge from V[i] to V[i+1]
        if (V[i].y <= P.y) {         // start y <= P.y
            if (V[i+1].y > P.y)      // an upward crossing
                if (isLeft( V[i], V[i+1], P) > 0)  // P left of edge
                    ++wn;            // have a valid up intersect
        }
        else {                       // start y > P.y (no test needed)
            if (V[i+1].y <= P.y)     // a downward crossing
                if (isLeft( V[i], V[i+1], P) < 0)  // P right of edge
                    --wn;            // have a valid down intersect
        }
    }
    return wn;
}


//=================================================================
  int cn_PnPoly (int ptNr, Point2 *pa, Point2 P)  {
//=================================================================
// cn_PnPoly(): crossing number test for a point in a polygon
//      Input:   P = a point,
//               pa[] = vertex points of a polygon pa[n+1] with pa[n]=V[0]
//      Return:  0 = outside, 1 = inside
// This code is patterned after [Franklin, 2000]


  int    i1, cn;
  double vt;

  cn = 0;      // the crossing number counter

  // loop through all edges of the polygon
  for (i1=0; i1<ptNr; i1++) {    // edge from pa[i1] to pa[i1+1]

    if (((pa[i1].y <= P.y) && (pa[i1+1].y > P.y))    // an upward crossing
      || ((pa[i1].y > P.y) && (pa[i1+1].y <= P.y))) { // a downward crossing

      // compute the actual edge-ray intersect x-coordinate
      vt = (P.y - pa[i1].y) / (pa[i1+1].y - pa[i1].y);
      if(P.x < pa[i1].x + vt * (pa[i1+1].x - pa[i1].x)) {  // P.x < intersect
         ++cn;   // a valid crossing of y=P.y right of P.x
         printf(" %d %f,%f\n",cn,pa[i1].x,pa[i1].y);
      }
    }
  }
  printf("ex cn_PnPoly %d %d\n",cn,cn&1);
  return (cn&1);    // 0 if even (out), and 1 if odd (in)

}

*/


//===================== EOF ===========================
