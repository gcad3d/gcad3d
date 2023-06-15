// ut_cvtrm.c                   RF                     2003-08-11
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
#ifdef globTag
void CVTRM(){}
#endif
/*!
\file  ../ut/ut_cvtrm.c
\brief CVTRM = trimmed-curve functions 
\code
=====================================================
List_functions_start:

CVTRM_ck_cyc             check if trimmed-curve passes tru end-startpoint

CVTRM__rev               reverse trimmed-curve (same curve, different direction)

CVTRM__dbo               create trimmedCurve from DB-lFig
CVTRM__obj               create trimmed-curve from obj (form,struct,dbi)
CVTRM__plg_2par          create trimmed-curve from polygon and parameters
CVTRM__plg_iseg          get trimmed-curve from segment of polygon
CVTRM_ocv_otb__          create trimmed-curve from object-table

UTO_obj__ccv_segnr       get obj (typ,data) from segment of CCV.

CVTRM_basCv_trmCv        get basic-curve of trimmed-curve (typ,dbi,obj)
CVTRM__basCv__           get updated trimmedCurve (direct ref to basic-curve)
CVTRM_basCv_trmCv_con    get trimmedCurve of parent-trimmedCurve
CVTRM_par1_con           translate parameters from child-curve to parent-curve

CVTRM_parent_ccv  DO NOT USE    get parent of a trimmedCurve    

CVTRM_set                set pointer to trimmed-curve from index              INLINE

// UT3D_seg_objSel            Segmentnummer am Polygon liefern
// UT3D_seg_dboSel            Segmentnummer am Polygon liefern
// UT3D_segpar_dboSel         den selektieren Punkt am obj feststellen
// UT3D_segpar_ccvSel         give segmentnr & parameter for point on ccv
// UT3D_ccv_contour           create Obj CCV from contours
// UT3D_ln_ccv_pt             make line from trimmed-curve and previous curve
// UT3D_pt_segparccv          Punkt <-- segNr und Paramter auf CCV
// UT3D_obj_segccv            get ObjGX from segNr in CCV
// UT3D_crv_segccv            get ObjGX + data from segment in CCV
// UT3D_stru_segccv           copy struct(LN/AC)  <-- segNr in CCV

List_functions_end:
=====================================================
- see also:
UT3D_pt_ccv_segnr_par1     get point from segment-nr and parameter on CCV
UT3D_vcn_cvtrm             Normalvector for trimmed-curve
UTO_cv_cvtrm               change trimmedCurve into standardCurve
\endcode *//*----------------------------------------


- unused functions:
cn_PnPoly                  crossing number test for a point in a polygon
wn_PnPoly                  winding number test for a point in a polygon
UT3D_pt_endptccv           proj PT -> PolygonCurve
UT3D_2pt_ccv1_lim          get start- or endpoint of trimmed-curve



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

==============================================================
ccv = trimmed-curve = struct CurvCCV  type = Typ_CVTRM

newCC = UT3D_CCV_NUL;      // create empty CurvCCV

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




#include "../ut/ut_cast.h"                // INT__PTR
#include "../ut/ut_geo.h"
#include "../ut/ut_geo_const.h"           // UT3D_CCV_NUL
#include "../ut/func_types.h"             // UI_Func... SYM_..
#include "../ut/ut_plg.h"                 // UT3D_par1_parplg
#include "../ut/ut_memTab.h"           // MemTab

#include "../db/ut_DB.h"               // DB_GetCurv

#include "../xa/xa_mem.h"              // memspc55



//================================================================
  int CVTRM_ocv_otb__ (ObjGX *ocv, Memspc *oSpc, ObjTab *otb1) {
//================================================================
// CVTRM_ocv_otb__            create trimmed-curve from object-table
// Input:
//   otb1      objs in
//   oSpc      memspc for trimmed-curve

  int        i1;
  long       lOff, ipp;
  Line       *ln1;
  Circ       *ci1;
  CurvCCV    *cvTab, cv1 = _CCV_NUL;


  // printf("CVTRM_ocv_otb__ oNr=%d\n",otb1->oNr); fflush (stdout);

  // get memSpace for (otb1->oNr) ccv's
  // cvTab = UME_reserve (oSpc, sizeof(CurvCCV) * (otb1->oNr));
  cvTab = UME__getSpc (oSpc, &lOff, sizeof(CurvCCV) * (otb1->oNr));
  if(!cvTab) {TX_Error("CVTRM_ocv_otb__ E1"); return -1;}


  //----------------------------------------------------------------
  for(i1=0; i1 < otb1->oNr; ++i1) {
      // printf(";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; \n");
      // printf(" otb1[%d] typ=%d\n",i1,otb1->oTyp[i1]);
      // DEB_dump_obj__ (otb1->oTyp[i1], otb1->oDat[i1], "otb1_%d",i1);

    cv1 = UT3D_CCV_NUL;   // empty
    cv1.v0 = 0.;
    cv1.v1 = 1.;
    
    //----------------------------------------------------------------
    if(otb1->oTyp[i1] == Typ_LN) {
      cv1.typ = Typ_LN;
      ln1 = otb1->oDat[i1];
      if(!i1) cv1.ip0 = DB_StorePoint (-1L, &ln1->p1);
      else    cv1.ip0 = ipp;
      ipp = DB_StorePoint (-1L, &ln1->p2);

    //----------------------------------------------------------------
    } else if(otb1->oTyp[i1] == Typ_CI) {
      cv1.typ = Typ_CI;
      ci1 = otb1->oDat[i1];
      if(!i1) cv1.ip0 = DB_StorePoint (-1L, &ci1->p1);
      else    cv1.ip0 = ipp;
      ipp = DB_StorePoint (-1L, &ci1->p2);
      // store circ in DB
      cv1.dbi = DB_StoreCirc (-1L, ci1);
      // create PRCV
      PRCV_set_obj_dbi (Typ_CI, ci1, 1, Typ_CI, cv1.dbi);

    //----------------------------------------------------------------
    } else {TX_Error("CVTRM_ocv_otb__ E2"); return -1;}

    cv1.ip1 = ipp;  // set endPt = startPt of following curve

      // TESTBLOCK
      // DEB_dump_obj__ (Typ_CVTRM, &cv1, " cv1_%d",i1);
      // GR_tDyn_obj (Typ_CVTRM, &cv1, Typ_Att_Symb, 0);
      // END TESTBLOCK

    // add cv1 -> cvTab
    cvTab[i1] = cv1;
  }


  //----------------------------------------------------------------
  // CCV from oiboTab2
  ocv->typ  = Typ_CVTRM;
  ocv->form = Typ_CVTRM; // Typ_CCV;
  ocv->siz  = otb1->oNr;
  ocv->data = cvTab;

    // TESTBLOCK
    // DEB_dump_ox_0 (ocv, "ex-CVOFF2_ccv__");
    // fflush (stdout);
    // END TESTBLOCK

  return 0;

}


//================================================================
  int CVTRM__rev (CurvCCV *cvtrm) {
//================================================================
// CVTRM__rev     reverse trimmed-curve (same curve, different direction)
// see UTO_stru_inv


  MEM_swap_2db(&cvtrm->v0, &cvtrm->v1);
  MEM_swap_2lg(&cvtrm->ip0, &cvtrm->ip1);
  cvtrm->dir = ICHG01 (cvtrm->dir);

  return 0;

}


//================================================================
  int CVTRM__obj (CurvCCV *cvtrm, int form, void *data, long dbi) {
//================================================================
// create trimmed-curve from obj (form,struct,dbi)
// Input:
//   form    type of curve
//   data    curve as stored in DB
//   dbi     DB-index of data; DB-type from form
// see CVTRM__dbo


  int     dir, clo;


  *cvtrm = UT3D_CCV_NUL;

  cvtrm->typ = form;
  cvtrm->dbi = dbi;

  // get parameters as 0-1
  UT3D_ptvcpar_std_obj (NULL, NULL, &cvtrm->v0, 1, Ptyp_start, form, data);
  UT3D_ptvcpar_std_obj (NULL, NULL, &cvtrm->v1, 1, Ptyp_end, form, data);

  // get closed-flag and direction.
  UTO_cv_ck_dir_clo (&dir, &clo, form, data);
  cvtrm->dir = dir;
  cvtrm->clo = clo;

    DEB_dump_obj__ (Typ_CVTRM, cvtrm, " ex-CVTRM__obj");

  return 0;

}


//=========================================================================
  int CVTRM__plg_2par (CurvCCV *cvo, 
                       double *kv0, double *kv1, long dbi, CurvPoly *plg) {
//=========================================================================
/// \code
/// CVTRM__plg_2par         get trimmed-curve from polygon and parameters
/// Input:
///   kv0,kv1  knotvalues v0,v1
///   dbi      DB-index of plgi
///   plgi     polygon | NULL
/// Output:
///   cvo      single trimmed-curve 
/// see CVTRM__plg_iseg
/// \endcode
  
  
  // DEB_dump_obj__ (Typ_CVPOL,  plg, " CVTRM__plg_2par");
  
  *cvo = UT3D_CCV_NUL;      // copy empty CurvCCV

  cvo->typ = Typ_CVPOL;
  cvo->dbi = dbi;


  // parameters -> par-0-1
  // cvo->v0 = UT3D_par1_parplg (kv0, plg);
  // cvo->v1 = UT3D_par1_parplg (kv1, plg);
  // parameters in trimmed-curve have vTyp=0
  cvo->v0 = *kv0;
  cvo->v1 = *kv1;

  if(cvo->v0 > cvo->v1) cvo->dir = 1;    // 1=backward

    // DEB_dump_obj__ (Typ_CVTRM, cvo, " ex CVTRM__plg_iseg");

  return 0;

}


//================================================================
  int CVTRM_basCv_trmCv (int *pTyp, long *pDbi, void **basCv,
                         CurvCCV *tcv) {
//================================================================
// CVTRM_basCv_trmCv    get basic-curve of trimmed-curve (typ,dbi,obj)
// Output:
//   pDbi     can be 0 for Lines (trimmed lines are not stored in DB;
//            IF pDbi of Line == 0 then basCv is static in CVTRM_basCv_trmCv !
//   retCod    0   OK
//            -1   no DB-object exists (eg connection-line in contour)

  int      oNr, cTyp;    // p=parent, c=child
  void     *obj;

static Line ln1;


  // DEB_dump_obj__ (Typ_CVTRM, tcv, " CVTRM_basCv_trmCv");


  L_nxt:
  *pTyp = tcv->typ;
  *pDbi = tcv->dbi;


  // trimmed Line does not have db-Line / PRCV
  if(*pTyp == Typ_LN) {
    if(!*pDbi) {
      ln1.p1 = DB_GetPoint (tcv->ip0);
      ln1.p2 = DB_GetPoint (tcv->ip1);
      ln1.typ = 0;
      *basCv = &ln1;
      goto L_exit;
    }
  }


  if(!*pDbi) return -1;


  // get typ/dbi of parent of tcv
  *pTyp = UTO__dbo (&obj, &oNr, *pTyp, *pDbi);
    // printf(" pTyp=%d\n",*pTyp);

  if(*pTyp == Typ_CVTRM) {
    tcv = obj;
    goto L_nxt;
  }

  *basCv = obj;

  L_exit:
    // printf(" ex _basCv_trmCv pTyp=%d pDbi=%ld\n",*pTyp,*pDbi);
    // DEB_dump_obj__ (*pTyp, *basCv, " ex _basCv_trmCv");

  return 0;

}


//================================================================
  int CVTRM_ck_cyc (CurvCCV *cvt) {
//================================================================
/// \code
/// CVTRM_ck_cyc            check if trimmed-curve passes tru end-startpoint
///
/// retCod:   0 = yes,cyclic; curve is passing tru endpoint/startpoint
///           1 = no, curve does not pass tru endpoint/startpoint 
///                 (but can start at startpoint or end at endpoint)
/// see UTO_cv_ck_dir_clo INF_struct_closed
/// \endcode

  int cyc = 1;


  // DEB_dump_obj__ (Typ_CVTRM, cvt, " CVTRM_ck_cyc");


  switch (cvt->typ) {

    //----------------------------------------------------------------
    case Typ_CI:       // Circ
    case Typ_CVELL:    // CurvElli
      // parameters are increasing for CCW-circs and also for CW-circs.
      // circ                 if(v0 >= v1) cyc=yes; (for CW AND CCW)
      // dir = difference of dirBas - dirtcv;
      if(!cvt->dir) {
        // fwd=CCW
        if(cvt->v0 >= cvt->v1) cyc = 0;    // yes,cyclic
      } else {
        // bwd=CW
        if(cvt->v1 >= cvt->v0) cyc = 0;    // yes,cyclic
      }
      break;


    //----------------------------------------------------------------
    case Typ_CVPOL:    // CurvPoly
    case Typ_CVBSP:    // CurvBSpl
    case Typ_CVRBSP:    // CurvRBSpl
      // bspl: parameters are increasing OR descending (if (dir==1, bwd))
      if(!cvt->dir) {
        if(cvt->v0 >= cvt->v1) cyc = 0;    // fwd; yes,cyclic
      } else { 
        if(cvt->v1 >= cvt->v0) cyc = 0;    // bwd; yes,cyclic
      }
      break;


    //----------------------------------------------------------------
    default:
      printf("***** UTO_cv_ck_cyc I001 %d\n",cvt->typ);

  }

    // printf("ex UTO_cv_ck_cyc %d\n",cyc);


  return cyc;

}


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
  ocv->typ   = Typ_CVTRM;
  ocv->form  = Typ_CVTRM;
  ocv->siz   = 1;
  ocv->data  = ccv;


  return 0;

}

 
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
  // DEB_dump_obj__ (Typ_ObjGX, cv1, " _pt_segparccv: ");
  // DEB_dump_ox_0 (cv1, " _crv_segccv: ");


  irc = UT3D_obj_segccv (oxo, is, cv1);
  if(irc < 0) return -1;


  // In der CCV-struct steht manchmal eine Line manchmals als Typ_LN aber auch
  // als Typ_PT !!
  if(oxo->typ == Typ_PT) {

    // den Endpunkt laden ..
    UTO_objDat_ox (&pp, oxo);
    ln->p2 = *pp;
      // DEB_dump_obj__ (Typ_PT, &ln->p2, " endPt: ");

    // es ist ein (End)Punkt; den Startpunkt dazu beschaffen.
    irc = UT3D_obj_segccv (oxo, is-1, cv1);
    if(irc < 0) return -1;
    if(oxo->typ == Typ_PT) {
      UTO_objDat_ox (&pp, oxo);
      ln->p1 = *pp;

    } else {
      // data des objekts beschaffen ..
      oTyp = UTO_objDat_ox (&oDat, oxo);
      // den Endpunkt des objekts beschaffen ..
      irc = UTO_2pt_limstru (NULL, &ln->p1, NULL, NULL, oTyp, oDat);
      if(irc < 0) return -1;
    }
      // DEB_dump_obj__ (Typ_PT, &ln->p1, " startPt: ");
    oxo->typ  = Typ_LN;
    oxo->form = Typ_LN;
    oxo->siz  = 1;
    oxo->data = ln;
  }

  // DEB_dump_ox_0 (oxo, "ex _crv_segccv:");

  *typ = oxo->typ;

  return 0;
}


//================================================================
  int UT3D_obj_segccv (ObjGX *oxo, int is, ObjGX *cv1) {
REPLACED by UTO_obj__ccv_segnr
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
  // DEB_dump_ox_0 (cv1, "cv1:\n");

  // das erste Seg ist immer ein Punkt; in diesem Fall sofort
  // das folgende liefern ..
  // if(is == 0) is = 1;

  // segment iseg aus cv1 extrahieren
  // oTab = cv1->data;
  // oTab = ((CurvCCV*)cv1->data)->cvtab;
  oTab = cv1->data;
  oSeg = &oTab[is];
    // printf(" typ=%d form=%d\n",oSeg->typ,oSeg->form);
    // DEB_dump_ox_0 (oSeg, "seg:\n");


  // if(oSeg->typ == Typ_PT) {
    // // make a Line from the previous point to this point
    // // line als dynam. Line zwischenspeichern ?
      // printf("  line from point ..\n");
    // // get endpoint of previous segment
    // // UTO_objDat_ox ();
    // // UTO_2pt_limstru ();
    // // get endpoint
    // // UTO_objDat_ox
    // // line -> oxo           UTO_stru_2_obj
    // oxo->typ = Typ_LN;
    // oxo->form = Typ_LN;
    // oxo->siz  = 1;
    // oxo->data  = 1;

  // } else if(oSeg->form == Typ_Index) {

  if(oSeg->form == Typ_Index) {
    ind = LONG__PTR (oSeg->data);
    // printf(" typ=%d ind=%d\n",oSeg->typ, ind);
    *oxo = DB_GetObjGX (oSeg->typ, ind);

  } else {
    // oTyp = UTO_objDat_ox (&od, oSeg);
    // if(oTyp < 0) return -1;
    TX_Error("UT3D_obj_segccv E001");
    return -1;
  }

  // DEB_dump_obj__ (Typ_ObjGX, oxo, "ex UT3D_obj_segccv");

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

  // DEB_dump_obj__ (Typ_ObjGX, &ox1, "  _ci_segccv ox1:\n");


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


  DEB_dump_obj__ (Typ_ObjGX, cv1, "UT3D_seg_objSel\n");


  pTab = (void*)memspc55;
  ptNr = sizeof(memspc55) / sizeof(Point);


  if(cv1->form != Typ_CVPOL) {
    TX_Print("UT3D_seg_objSel E001 %d %d",cv1->typ,cv1->form);
    return -1;
  }



    // create polygon for obj
    irc = UT3D_npt_ox__ (&ptNr, pTab, cv1, UT_DISP_cv*2.);
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


  DEB_dump_obj__ (Typ_LN, ln1, "UT3D_segpar_sel_LN");


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
  // actScrPos = GL_curPos_SC = ?
  // actUsrPos = GL_curPos_WC = ?
  GL_get_curPos_last (&actUsrPos, &actScrPos);
    DEB_dump_obj__ (Typ_PT, &actUsrPos, " actUsrPos");
    DEB_dump_obj__ (Typ_PT, &actScrPos, " actScrPos");

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

  // btyp = AP_typDB_typ (typ);
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
  } else if((typ == Typ_CV)&&(oxi.typ == Typ_CVTRM)) {
    // das Polygon der selektierten CCV holen UND segmnetIndexTable iTab !
    iNr = iTabSiz;
    UT3D_pta_ox_lim (&ptNr, pTab, &oxi, iNr, iTab, UT_DISP_cv*2.);
      // GR_tDyn_npt__ (ptNr, pTab, SYM_STAR_S, 3);
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
    irc = UT3D_npt_ox__ (&ptNr, pTab, &oxi, UT_DISP_cv*2.);
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
    DEB_dump_obj__ (Typ_PT, &spt, "  spt:\n");



  //----------------------------------------------------------------
  if(typ != Typ_CVTRM) goto L_not_CCV;
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
    // DEB_dump_obj__ (Typ_PT, &spt, "  spt = ");


  *isNr = 2;          // size of is
  is[1] = iSeg;       // subSegNr / pointNr
    // printf(" CCV-l0: typ=%d sTyp=%d is=%d,%d\n",typ,sTyp,is[0],is[1]);

  // // get ps=parameter of point spt on curve
  // irc = UTO_par__pt_obj (ps, &spt, ox1.form, ox1.data);
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

    irc = UTO_par__pt_obj (ps, &spt, sTyp, ox1.data);
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
    // DEB_dump_obj__ (Typ_ObjGX, &oxi, "CCV-Seg:\n");
  // typ = AP_typDB_typ (oxi.typ);
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
  irc = UTO_par__pt_obj (ps, &spt, oxi.form, oxi.data);
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


  // DEB_dump_obj__ (Typ_PT, p1, "UT3D_segpar_ccvSel ");


  pTab = (void*)memspc55;
  ptNr = sizeof(memspc55) / sizeof(Point);


  // das Polygon der selektierten CCV holen UND segmnetIndexTable iTab !
  iNr = iTabSiz;
  UT3D_pta_ox_lim (&ptNr, pTab, cv1, iNr, iTab, UT_DISP_cv);
    // GR_tDyn_npt__ (ptNr, pTab, SYM_STAR_S, 3);
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


  // DEB_dump_obj__ (Typ_PT, p1, "UT3D_segpar_ptccv ");


  pTab = (void*)memspc55;
  ptNr = sizeof(memspc55) / sizeof(Point);


  iNr = iTabSiz;
  UT3D_pta_ox_lim (&ptNr, pTab, cv1, iNr, iTab, UT_DISP_cv);
    // GR_tDyn_npt__ (ptNr, pTab, SYM_STAR_S, 3);
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
    // DEB_dump_ox_0 (oSeg, "seg:\n");

  oTyp = UTO_objDat_ox (&od, oSeg);
  if(oTyp < 0) return -1;

  // get Parameter from object
  if(oTyp == Typ_PT) d1 = 0.;
  else UTO_par__pt_obj (&d1, p1, oTyp, od);

  if(fabs(d1-1.) < UT_TOL_min0) d1 = 1.;



  //----------------------------------------------------------------
  L_done:
  // printf("ex UT3D_segpar_ptccv seg=%d par=%f\n",iSeg,d1);

  *is = iSeg;
  *ps = d1;

  return 0;

}
*/

      
/* UNUSED ?
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


  DEB_dump_obj__ (Typ_CVTRM, ccv, "UT3D_2pt_ccv1_lim: ");


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

    
    DEB_dump_obj__ (Typ_LN, lno, "ex UT3D_ln_ccv_pt");

  return 0;

}
*/

//====================================================================
  int UT3D_pt_ccv_segnr_par1 (Point *po, int is, double ps, ObjGX *cv1) {
//====================================================================
/// \code
/// UT3D_pt_ccv_segnr_par1      get point from segment-nr and parameter on CCV
/// is    segmentNr; 0=first
/// \endcode

  int      irc, typ;
  // CurvCCV  *cca, *cc1;
  char      cvo[OBJ_SIZ_MAX];


  // printf("UT3D_pt_ccv_segnr_par1 is=%d par1=%lf\n",is,ps);
  // DEB_dump_obj__ (Typ_ObjGX, cv1, " _pt_segparccv: ");


  // get typ,data from seg.<is> 
  irc = UTO_obj__ccv_segnr (&typ, cvo, is, cv1);
  if(irc < 0) {
    TX_Error("UTO_obj__ccv_segnr EI_%d_%d_%d",irc,cv1->typ,cv1->form);
    return -1;
  }


  //----------------------------------------------------------------
  // get point from parameter <ps>
  L_par:
  // irc = UT3D_pt_evparcrv (po, ps, typ, cvo);
  irc = UT3D_pt_vc__par_cv (po, NULL, typ, cvo, 1, ps);

    // DEB_dump_obj__ (Typ_PT, po, "ex UT3D_pt_ccv_segnr_par1");

  // exit(0);
  return irc;

}


/*
//====================================================================
  int UT3D_pt_segparccv (Point *p1, int is, double ps, ObjGX *cv1) {
//====================================================================
// REPLACED BY UT3D_pt_ccv_segnr_par1
// UT3D_pt_segparccv      Punkt <-- segNr und Paramter auf CCV

// see UTO_pt_par1_dbo
// see UT3D_obj_segccv

  int      irc, typ;
  void     *data;
  Point    *pa, *pe, pta;
  Line     ln1;
  ObjGX    ox1;


  // printf("UT3D_pt_segparccv %d %f\n",is,ps);
  // DEB_dump_obj__ (Typ_ObjGX, cv1, " _pt_segparccv: ");
  // DEB_dump_ox_0 (cv1, " _pt_segparccv: ");



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
  int UT3D_vcn_cvtrm (Vector *vco, Point *pto, ObjGX *oi, Memspc *wrkSpc) {
//=======================================================================
// UT3D_vcn_cvtrm         Normalvector an (planare) CCV
// Liefert auch den ersten Punkt der Kontur !
// Wenn CCV geschlossen: vco zeigt nach innen !!
// see also UT3D_ptvc_sus (Normalvec) braucht SurStd* !!
// see also APT_DrawCurv
// see also UT3D_pta_sus  braucht SurStd* !
// see also UT3D_npt_ox__


  int    irc, ptNr, ibp, isr;
  Point  *ptTab;
  Plane  pl1;


  // DEB_dump_ox_0(oi, "UT3D_vcn_cvtrm:");


  if(oi->typ != Typ_CVTRM) return -1;

  // CCV -> 3D-Polygon umwandeln
  ptTab = (Point*)wrkSpc->start;
  ptNr  = UME_ck_free(wrkSpc) / sizeof(Point);
  irc = UT3D_npt_ox__ (&ptNr, ptTab, oi, UT_DISP_cv);
  // printf(" irc=%d ptNr=%d\n",irc,ptNr);
  if(irc < 0) return irc;

  // Plane from CCV: see GR_DrawSur
  // 3D-Polygon -> Plane
  UT3D_pl_pta (&pl1, ptNr, ptTab);
  *pto = pl1.po;
  *vco = pl1.vz;
  wrkSpc->start = ptTab;   // reset memspc

  // Normalvec entsprechend Drehsinn:
  ibp = UT3D_bp_perp_vc (NULL, &pl1.vz);
  isr = UT3D_sr_polc (ptNr-1, ptTab, &pl1.vz, ibp);
  // printf(" isr=%d\n",isr);
  if(isr < 0)  UT3D_vc_invert (vco, vco);

    // GR_tDyn_vc__ (vco, pto, 2, 0);
  return irc;

}


//==============================================================================
  int CVTRM_basCv_trmCv_con (CurvCCV *cv2, CurvCCV *cv1, int form, void *cvBas) {
//==============================================================================
/// \code
/// make cv2 = combined curve of cv2 on cv1 on basicCurve 
/// Input:
///   form,cvBas
///   cv1              v0,v1 of cv1 refers to cvBas
///   cv2              v0,v1 of cv2 refers to cv1
/// Output:
///   cv2              v0,v1 of cv2 refers to cvBas
/// \endcode

  int      dirb, dir1, cyc;
  double   ang1, v0, v1, dd;


  // DEB_dump_obj__ (form, cvBas, "CVTRM_basCv_trmCv_con");
  // DEB_dump_obj__ (Typ_CVTRM, cv1, " _trmCv_con-1");
  // DEB_dump_obj__ (Typ_CVTRM, cv2, " _trmCv_con-2");


  //----------------------------------------------------------------
  // Line
  if(form != Typ_LN) goto L_circ;
  // translate parameters from child-curve cv2 to parent-curve cv1
  CVTRM_par1_con (&cv2->v0, &cv2->v1, &cv1->v0, &cv1->v1, 0);
  goto L_exit;



  //----------------------------------------------------------------
  // Circ
  L_circ:
  if(form != Typ_CI) goto L_bspl;

  // set dir
  cv2->dir = I_XOR_2I (cv1->dir,cv2->dir);

  cyc = CVTRM_ck_cyc (cv1);

  // translate parameters from child-curve cv2 to parent-curve cv1
  CVTRM_par1_con (&cv2->v0, &cv2->v1, &cv1->v0, &cv1->v1, cv1->dir, cyc);

  goto L_exit;


  //----------------------------------------------------------------
  L_bspl:
  if(form != Typ_CVBSP) goto L_elli;
  // B-spline: keep v0,v1; combine dir = XOR dir,dir2.
  cv2->dir = I_XOR_2I (cv1->dir,cv2->dir);
  goto L_exit;



  //----------------------------------------------------------------
  L_elli:

  // set dir
  cv2->dir = I_XOR_2I (cv1->dir,cv2->dir);

  cyc = CVTRM_ck_cyc (cv1);

  // translate parameters from child-curve cv2 to parent-curve cv1
  CVTRM_par1_con (&cv2->v0, &cv2->v1, &cv1->v0, &cv1->v1, cv1->dir, cyc);


  //----------------------------------------------------------------
  L_exit:

    // DEB_dump_obj__ (Typ_CVTRM, cv2, " ex  _trmCv_con");

  return 0;

}

 
//========================================================================
  int CVTRM__basCv__ (CurvCCV *cv2, void **data, CurvCCV *cv1) {
//========================================================================
// get basicCurve of trimmedCurve  (fix parameters if necessary)
// if basic-curve of ccv1 is also trimmedCurve: modify in cv1:
// - get type and dbi of basicCurve (cv1.typ and cv1.dbi)
// - get v0 and v1 of trimmedCurve on basicCurve (cv1.v0 and cv1.v1)
//
// Input:
//   cv1       trimmedCurve
// Output:
//   cv2       trimmedCurve; typ, dbi, v0, v1 updated (to fix to basicCurve)
//   data      basicCurve of type cv2.typ
//
// TODO: CCV in CCV sollte recursiv aufgelöst werden !

  int      form, oNr, iNr, i1, typ;
  long     dbi;
  double   ang1, ang2;
  Circ     *ci1;
  CurvCCV  cca[10];


  // TESTBLOCK
  // DEB_dump_obj__ (Typ_CVTRM, cv1, "CVTRM__basCv__-in");
  // if((cv1->v0 < 0.)||(cv1->v1 > 1.)) TX_Error("CVTRM__basCv__EIN1");
  // END TESTBLOCK


  // store all CCVS in cca, if basCurve is found: stop and keep in data.
  cca[0] = *cv1;
  iNr = 1;


  // get parent of cv1
  L_res_nxt:
  typ = cca[iNr - 1].typ;
  dbi = cca[iNr - 1].dbi;
  form = UTO__dbo (data, &oNr, typ, dbi);
    // printf(" form1=%d oNr=%d\n",form,oNr);
    // DEB_dump_obj__ (form, *data, " _basCv-1");


  // resolv trimmed-curve
  if(form == Typ_CVTRM) {
    // store -> cca
    if(iNr >= 10) {TX_Error("CVTRM__basCv__ E001"); return -1;}
    cca[iNr] = *(CurvCCV*)*data;   // copy trimmed-curve
    ++iNr;
    goto L_res_nxt;
  }


  //----------------------------------------------------------------
  // data = basCurve; cca has <iNr> trimmed-curves
  if(iNr < 2) goto L_exit;

  // fix parameters.
  // for CurvBSpl & CurvPoly parameters do not change; 
  // for Circ & Ellipse parameters must be fixed

  i1 = iNr - 1;
  for (;;) {
    // work (1 < 2), then (0 < 1)
    CVTRM_basCv_trmCv_con (&cca[i1 - 1], &cca[i1], form, *data);
    --i1;
    if(i1 <= 0) break;
  }



  //----------------------------------------------------------------
  L_exit:
  *cv2 = cca[0];     // copy updated trimmed-curve with ref=basic-curve

  // keep points !
  cv2->ip0 = cv1->ip0;
  cv2->ip1 = cv1->ip1;
  // set typ,dbi
  cv2->typ = form; // cca[iNr - 1].typ;
  cv2->dbi = dbi;  // cca[iNr - 1].dbi;


  // get parameter if parameter and point undefined
  // if(cv2->v0 == 
  // if(cv2->ip0 == 0) 
  // if(cv2->ip1 == 0) 


    // TESTBLOCK
    // printf("ex-CVTRM__basCv__ iNr=%d\n",iNr);
    // DEB_dump_obj__ (Typ_CVTRM, cv2, " _trmCv__-cv2");
    // DEB_dump_obj__ (form, *data, " _trmCv__-data");
    // printf(" \n\n");
    // TESTBLOCK

  return 0;

}


//=========================================================================
  int CVTRM_par1_con (double *ov0, double *ov1,
                      double *iv0, double *iv1, int idir, int icyc) {
//=========================================================================
/// \code
/// CVTRM_par1_con    translate parameters from child-curve to parent-curve
/// Input:
///   ov0, ov1   parameters of child-curve; curve is part of parent-curve iv0-iv1 
///   iv0, iv1   parameters of parent-curve 
///   idir       0=qqsame dir as parent-curve; 1=not,revers
///   icyc       0=child-curve_is_cyclic (passes tru start-endPoint); 1=not
/// Output:
///   ov0, ov1 = parameters of child-curve on parent-curve iv0-iv1
/// \endcode

  double  dd;


  // printf("CVTRM_par1_con dir=%d cyc=%d\n",idir,icyc);

  dd = *iv1 - *iv0;        

  if(!icyc) {
    // yes, cyclic, passes tru start-endPoint
    if(!idir) {
      // fwd
      // dd -= 1.;    // Circ-?
      dd += 1.;     // elli-CASE-1

    } else {
      // bwd
      dd -= 1.;   // elli-CASE-2
    }
  }



  *ov0 = *iv0 + (*ov0 * dd);
  *ov1 = *iv0 + (*ov1 * dd);

  // if(*ov1 < 0.) *ov1 += 1.;
  if(*ov0 < 0.) *ov0 += 1.;        // elli-CASE-2
  if(*ov1 > 1.) *ov1 -= 1.;        // elli-CASE-1

    // printf("ex CVTRM_par1_con v0=%lf v1=%lf dd=%lf\n",*ov0,*ov1,dd);

  return 0;

}
 

//================================================================
  int CVTRM__dbo (CurvCCV *cvtrm, int typ, long dbi) {
//================================================================
/// create trimmedCurve from DB-lFig (copy curve -> trimmedCurve)
// see UTO_cv_cvtrm               change trimmedCurve into standardCurve
// see CVTRM__obj


  int      form, oNr, dir, clo;
  void     *obj;


  // printf("             %d %ld\n",typ,dbi);


  // get baseCurve
  form = UTO__dbo (&obj, &oNr, typ, dbi);

  if(form == Typ_CVTRM) {
    *cvtrm = *(CurvCCV*)obj;
    goto L_exit;
  }


  *cvtrm = UT3D_CCV_NUL;

  cvtrm->typ = typ;
  cvtrm->dbi = dbi;


  // get parameters as 0-1
  UT3D_ptvcpar_std_obj (NULL, NULL, &cvtrm->v0, 1, Ptyp_start, form, obj);
  UT3D_ptvcpar_std_obj (NULL, NULL, &cvtrm->v1, 1, Ptyp_end, form, obj);

  // get closed-flag and direction.
  UTO_cv_ck_dir_clo (&dir, &clo, form, obj);
  cvtrm->dir = dir;
  cvtrm->clo = clo;


  L_exit:

    // DEB_dump_obj__ (Typ_CVTRM, cvtrm, " ex-CVTRM__dbo");

  return 0;

}


//========================================================================
  int CVTRM__plg_iseg (CurvCCV *cvo, int iseg, long dbi, CurvPoly *plgi) {
//========================================================================
/// \code
/// CVTRM__plg_iseg            get segment of polygon
/// Input:
///   dbi    DB-index of plgi
///   plgi   polygon | NULL
///   iseg   index of segment to extract; 0=first.
/// Output:
///   cvo    single trimmed-curve 
/// see UT3D_2pt_plg_iseg UTO_obj__ccv_segnr
/// \endcode


  int       irc, typ, oNr;
  double    kv0, kv1;
  char      obj[OBJ_SIZ_MAX];


  // printf("CVTRM__plg_iseg iseg=%d dbi=%ld\n",iseg,dbi);


  if(iseg >= plgi->ptNr) return -1;

  if(!plgi) {
    irc = UTO__dbo  ((void**)&plgi, &oNr, Typ_CV, dbi);
    if(irc < 0) return -2;
  }
    // DEB_dump_obj__ (Typ_CVPOL, plgi, " _plg_iseg-plgi");


  // get parameters of segment iseg
  UPLG_2par_iseg (&kv0, &kv1, iseg, plgi);

  // get trimmed-curve from polygon and parameters
  return CVTRM__plg_2par (cvo, &kv0, &kv1, dbi, plgi);

}

/*   replaced by OPAR_get_src
//====================================================================
  int CVTRM_parent_ccv (int *typ, long *dbi, int chdTyp, long chdDbi) {
//====================================================================
// DO NOT USE; (use OPAR_get_src); 
// CVTRM_parent_ccv       get parent of a trimmedCurve
// 

  int     irc, i1, form, oNr;
  CurvCCV *ccv1;

  form = UTO__dbo ((void**)&ccv1, &oNr, chdTyp, chdDbi);

  if(form != Typ_CVTRM) return -1;

  *typ = ccv1->typ;
  *dbi = ccv1->dbi;

  return 0;

}
*/

/* replaced by UT3D_ptvcpar_std_obj
//=========================================================================
  int UT3D_pt_endptccv (Point *pTab, ObjGX *ccv1) {
//=========================================================================
// UT3D_pt_endptccv           proj PT -> PolygonCurve
// liefert 2 Punkte; 0=Startpunkt, 1=Endpunkt.

  int    i1;
  ObjGX  *oxTab;


  printf("UT3D_pt_endptccv \n");
  DEB_dump_obj__ (Typ_CVTRM, ccv1, "ccv1:\n");



  // oxTab = ((CurvCCV*)ccv1->data)->cvtab;
  // i1 = ((CurvCCV*)ccv1->data)->segNr;
  oxTab = ccv1->data;
  // i1 = ccv1->siz - 1;

///
//   // get last curve  // wouldnt be last point better ?
//   i1 = ccv1->siz - 2;         // last record is the trim-record !!
//   // if there is only 1 record:
//   if(i1 < 0) i1 = 0;
// 
//   // UTO_2pt_lim_ox (&pTab[0], &oxTab[0],  1);    // 1=Anf.
//   // UTO_2pt_lim_ox (&pTab[1], &oxTab[i1], 2);    // 2=End.
///

  UTO_2pt_lim_ox (&pTab[0], &pTab[1], NULL, NULL, oxTab);

  return 0;

}
*/


//=========================================================================
/* unused Functions:
//=========================================================================


//===================================================================
inline int isLeft( Point2 P0, Point2 P1, Point2 P2 ) {
//===================================================================

  return ( (P1.x - P0.x) * (P2.y - P0.y) - (P2.x - P0.x) * (P1.y - P0.y) );
}

//===================================================================
  int wn_PnPoly (Point2 P, Point2* V, int n) {
//===================================================================
// wn_PnPoly(): winding number test for a point in a closed polygon
// winding number: 1 if point is inside a CCW-circ
//   2 if inside a second circle CCW 
//   0 if inside a CW-circle inside CCW-circle
//
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
// polygon must not be self-intersecting
//      Input:   P = a point,
//               pa[] = vertex points of a polygon pa[n+1] with pa[n]=V[0]
//      Return:  0 = outside, 1 = inside
// This code is patterned after [Franklin, 2000]
// intersect point P with ray through pa; even nr of intersections = outside.


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
