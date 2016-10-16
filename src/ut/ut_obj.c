//     ut_obj.c                                  RF
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
  alle _npt_ -> ../ut/ut_npt.c
  structs (ObjgX) in Polygon umwandeln; see UT3D_pta_sus UT3D_pta_ccv

-----------------------------------------------------
Modifications:
2011-06-03 UTO_stru_2_obj replaced by OGX_SET_OBJ
2011-06-03 UTO_obj_load replaced by OGX_SET_OBJ
2002-12-20 new from older sources. RF.

-----------------------------------------------------
*/
#ifdef globTag
void UTO(){}
#endif
/*!
\file  ../ut/ut_obj.c
\brief  complex-object-manipulationes  (transform, convert ..)  (UTO_)
\code
=====================================================
List_functions_start:

--------- functions for curves: ---------------------
UTO_ck_curvLimTyp       check if CurveLimits are Points or parameters
UTO_ck_curvForm         check if objTyp is ObjGX or other typ
UTO_cv_ck_clo           check if curve is closed
UTO_stru_inv            invert object (Typ, struct)
UTO_cvtrm_cv            trimmedCurve from DB-lFig (copy curve -> trimmedCurve)
UTO_cv_cvtrm            standardCurve from trimmedCurve (CCV)

--------- functions for surfaces: -------------------
UTO_ck_surfTyp          returns surfSubTyp
UTO_npt_Tes             get table of pointers from tessSurf

--------- functions for all types of objects: -------
UTO_dump__              dump object
UTO_dump_s_             dump obj - structured display

UTO_ck_dbsTyp             check object-typ (struct or object)
UTO_ck_typTyp           check if 1=curv(LN|AC|CV ) | 2=surf(PLN|A|B)
UTO_ck_nearptnobj       find object nearest to point
UTO_siz_stru            give size of struct

UTO_dbs_ox              DB-struct (data for VDPLC, ox for SAB) from ox
UTO_sav_ost             create DB-object & save it in Model.

UTO_obj_getp            get dataStruct for complexObj; resolv links.
UTO_get_DB              get dataStruct from DB-Obj; keep Refs.
UTO_obj_get    DO NOT USE   get a copy of a complexObj
UTO_get_PT     DO NOT USE   make ObjX aus ObjGX (Pointer (zB Line*) holen).
UTO_get_LN     DO NOT USE
UTO_objx_get   DO NOT USE   use UTO_obj_getp

UTO_parpt_pt_dbo        get parameter from point (obj from typ/ind)
UTO_parpt_pt_obj        get parameter from point on curve
UTO_par_tot             get endParameter - unlimited

UTO_set_ptlim           Anfangs- und/oder Endpunkt einer struct setzen
UTO_pt_objind           get point from object and ptNr

UTO_ptnr_std_obj        get nr of characteristic points for obj

UTO_pt_eval_par1_dbo    get point from 0-1-parameter (curve from typ/ind)
UTO_pt_midnobj          get midpoint of n objects
UTO2_pt_mid_obj         get centerpoint of 2D-obj
UT3D_vcn_cvo            Normalvector an (planare) Curve
UT3D_vc_tng_crv_pt       vector tangent to curve at point

UTO_INT_susu            intersect 2 surfs; result = curve
UTO_INT_crv_sur         intersect CRV X SUR; result = points
UTO_npt_int_2ox         intersect 2 limited objects where result = points
UTO_stru_int            intersect 2 unlim.  objects where result = points
//UTO_obj_int             intersect obj with 1 cutting-object

UTO_CUT__               trim obj with 1 or 2 cutting-objects
UTO_trim_2par1          trim (modify) obj from dataStruct by 2 parameters

UTO_TNG_cv_cv           get tangent curve - curve (P,C,S; not D,A,B)
UTO_TNG_vc_cv           get tangent vector - curve (P,C,S; not D,A,B)
UTO2_TNG_vc_obj         get point on 2D-curve (tangent from vector)
UTO2_TNG_pt_obj         get point on 2D-curve (tangent from point)
UTO_TNG_maxSol          return max nr of tangent-solutions for objtype

// UTO_obj_appprj          project obj > Plane         OBSOLETE: USE UPRJ_app__

UTO_obj_appmir          mirror obj about line | plane
UTO_pt_mir
UTO_vc_mir
UTO_pln_mir

UTO_invert              DO NOT USE       Obj umdrehen   (use UTO_stru_inv)

UTO_obj_tra_m3  DO NOT USE    transform obj with matrix
UTO_obj_save    DO NOT USE    Obj (nur struct) speichern (copy -> Memspc)
UTO_copy_obj    DO NOT USE    copy ObjGX-Table, recursive
UTO_copy_0      DO NOT USE    duplicate obj (oGX-tree; keep data-pointers)
UTO_copy_tab    DO NOT USE    ObjGX-Table (rekursiv) kopieren
UTO_copy_stru   DO NOT USE    copy struct (ohne resolve - nicht recursiv)
UTO_isol__      DO NOT USE    duplicate/isolate object (resolve Links)
UTO_reloc_obj   DO NOT USE    relocate obj (oGX-tree)
UTO_rec_set     DO NOT USE    einen Record schreiben, usagenotes

UTO_write_endRec        write end-record for tesselated data

UTO_addRange            add range to ObjRange
UTO_queryRange          query if typ/DB-ind is in ObjRange
UTO_dumpRange

UTO_MOD_resolv_open
UTO_MOD_resolv_closed
UTO_MOD_resolv_two_open
UTO_MOD_resolv_two_closed

List_functions_end:
=====================================================
- see also:
OGX                     complex-object-functions
UT3D_npt_ox            objekt  -> Polygon umwandeln (../ut/ut_npt.c)
UT2D_void_obj2  DO NOT USE        ObjG2-Objekt -> memory kopieren

\endcode *//*----------------------------------------

replaced/unused FUNCTIONS:
UTO_2pt_lim_ox          get limits (startpoint, endpoint, parameters) of ObjGX
UTO_2pt_lim_dbo         get limits (startpoint, endpoint, parameters) of DB-obj
UTO_2pt_limstru         get limits (startpoint, endpoint, parameters) of struct
UTO_pt_ox               get typical point for complex-obj



Function-output checks or creates or modifies a object (typ+struct) or
  complex-object.



Offline testen:
cc -Wall ut_obj.c -DOFFLINE&&a.out
==============================================================

*/


#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>


// #include "../ut/ut_umem.h"
#include "../ut/ut_cast.h"                // INT_PTR
#include "../ut/ut_geo.h"
#include "../ut/ut_ox_base.h"             // OGX_SET_INDEX
#include "../ut/ut_obj.h"
#include "../ut/ut_memTab.h"              // MemTab_..
#include "../ut/ut_bspl.h"                // UT3D_parbsp_par1
#include "../ut/ut_rbspl.h"               // UT3D_par_par1_rbsp
#include "../ut/ut_plg.h"                 // UT3D_par1_parplg
#include "../ut/ut_txt.h"                 // term_buf
#include "../ut/ut_gtypes.h"              // AP_src_typ__

#include "../ut/func_types.h"                  // SYM_TRI_S

#include "../db/ut_DB.h"

#include "../xa/xa_msg.h"                 // MSG_*
#include "../xa/xa_mem.h"                 // memspc54
// #include "../xa/xa_uid.h"               // UI_MODE_MAN




// EXTERNAL variables:
// aus ../xa/xa.c:
extern Plane     WC_sur_act;            // die aktive Plane
// extern double APT_ln_len;



// ex ../ci/NC_Main.c:
extern double APT_ModSiz;



// local variables:
static char TR_obj[OBJ_SIZ_MAX];  // speichert TransVektor od TraRot f. UTO_pt_tr



// prototypes:
  typedef_MemTab(ObjRange);





//========================================================
//========================================================
#ifdef OFFLINE
  int main () {

  void *pa, *pe;

  Point pt1;
  ObjGX ox1;
 
  char  membuf[1000];




  //----------------------------------
  pa = membuf;


  pt1.x=123.; pt1.y=456.; pt1.z=789.;

  ox1.typ  = Typ_PT;
  ox1.form = Typ_PT;
  ox1.siz  = 1;
  ox1.data = &pt1;


  // UTO_obj_2_mem (&pe, pa, &ox1);

  // printf("|%s|%s|%s|\n",cp1,cp2,cp3);

  return 0;

}
  // void TX_Error(char *text) {
  // printf(" TX_Error|%s|\n",text);
// }
#endif
//========================================================
//========================================================


//================================================================
  int UTO_dbs_ox (void *dbs, int *form, ObjGX *ox) {
//================================================================
/// \code
/// UTO_dbs_ox              DB-struct (data for VDPLC, ox for SAB) from ox
///   
/// Output:
///   dbs       size should be OBJ_SIZ_MAX; typ=ox->form
///   form      struct-type of output; ox.for for VDPLC, Typ_ObjGX for SAB
/// \endcode


  int    fTyp, siz;


  // is output a single data-record or a ox
  fTyp = UTO_ck_dbsTyp (ox->typ);  // 0=struct(D,P,L,C); 1=oGX(S,A,B);
     // printf(" fTyp=%d\n",fTyp);



  if(!fTyp) {
    // 0=struct(D,P,L,C)
    siz = UTO_siz_stru(ox->form);
    // copy data-record from poo -> objo
    memcpy (dbs, ox->data, OBJ_SIZ_MAX);
    *form = ox->form;

  } else {
    // 1=oGX(S,A,B)
    memcpy (dbs, ox, sizeof(ObjGX));
    *form = Typ_ObjGX;
  }

    // UT3D_stru_dump (*form, dbs, "ex UTO_dbs_ox");

  return 0;

}


//====================================================================
   int UTO_vc_perp_sur (Vector *vco,
                       ObjGX *su1, Point *pti, int iDir,
                       Memspc *tmpMen) {
//====================================================================
/// \code
/// UTO_vc_perp_sur           vector right-angled to surface
/// Input:
///   iDir      0=perp; 1=parl, 2=across;
/// \endcode

  int     irc;
  Point   pt2;

  // printf("UTO_vc_perp_sur typ=%d form=%d\n",su1->typ,su1->form);
  // UT3D_stru_dump (Typ_PT, pti, "  pti:");
  

  // get U-V-parameters
  irc = SUR_pt2_prjptsur (&pt2, pti, su1);
  if(irc < 0) goto L_n_i;
    // printf(" du,dv=%lf %lf\n",pt2.x,pt2.y);

 
  //----------------------------------------------------------------
  if(su1->form == Typ_SURBSP) {
      // UT3D_stru_dump (Typ_SURBSP, su1->data, "  suBsp:");

    UT3D_ptvc_evparsbsp (NULL, vco, 2, iDir, su1->data, pt2.x, pt2.y, tmpMen);


  //----------------------------------------------------------------
  } else goto L_n_i;


  return 0;


  L_n_i:
    TX_Error("UTO_vc_perp_sur - form %d not implemented",su1->form);
    return -1;

}

/* REPLACED with UT3D_ptvc_tng_crv_par
//=========================================================================
  int UTO_vc_tng_obj_par (Vector *vco,
                           int typ, void *data, int pTyp, double par) {
//=========================================================================
/// \code
/// UTO_vc_tng_obj_par      vector tangent to curve at parameter 0-1
/// Input:
///   par1       parameter between 0 and 1
///   typ        type of data
///   pTyp       type of parameter;
///                 0=normalized parameter (0-1)
///                 1=native parameter; any value ..
/// \endcode


  Vector vp;
  Point  pt1;


  printf("UTO_vc_tng_obj_par typ=%d par1=%lf\n",typ,par);
  UT3D_stru_dump (typ, data, "  obj =");


  //----------------------------------------------------------------
  if(typ == Typ_LN) {
    UT3D_vc_ln (vco, data);
    return 0;

  //-------------------------------
  } else if(typ == Typ_CVPOL) {
// TODO: until now all plg-parameters are native parameters (the real length)
    return UT3D_vc_evalplg (vco, data, par);

  //----------------------------------------------------------------
  } else if(typ == Typ_CI) {
    if(pTyp == 0) {
      // 0=par0-1
      par = UT3D_angr_ci_par1 (data, par);
    }
    UT3D_ptvc_eval_ci_angr (NULL, vco, data, par);



  //----------------------------------------------------------------
  } else if(typ == Typ_CVELL) {
    // 1=native: angle
    if(pTyp) {
      // pt from angle 
      UT3D_pt_elangd (&pt1,
                      &((CurvElli*)data)->pc,
                      &((CurvElli*)data)->va, 
                      &((CurvElli*)data)->vb,
                      par);
    } else {
      // pt from par1
      UT3D_pt_eval_ell_par1 (&pt1, data, par);
    }
    return UT3D_vc_tangel (vco, &pt1, data);

  //----------------------------------------------------------------
  } else if(typ == Typ_CVBSP) {
    // native: knotValue
    if(pTyp)
      par = UT3D_parbsp_par1 (par, data); // par (knotVal) -> par0-1
    return UT3D_vc_evalparCv (vco, data, par);

  //----------------------------------------------------------------
  } else if(typ == Typ_CVCLOT) {
    return UT3D_ptvc_evparclot (&pt1, vco, 1, data, par);

  //----------------------------------------------------------------
  } else goto L_NYI;



  return 0;


  //----------------------------------------------------------------
  L_NYI:
    TX_Error("not yet implemented - UTO_vc_tng_obj_par1 E001 %d",typ);
    return -1;

}
*/


//================================================================
  int UT3D_vcn_cvo (Vector *vco, Point *pto, ObjGX *oi, Memspc *wrkSpc) {
//================================================================
/// \code
/// Normalvector an (planare) Curve
/// Liefert auch den ersten Punkt der Kontur !
/// see also UT3D_vc_perppta
/// see also UT3D_ptvc_sus (Normalvec) braucht SurStd* !!
/// see also UT3D_pta_sus  braucht SurStd* !
/// see also UT3D_npt_ox
/// \endcode


  int    irc;
  ObjGX  *o1;


  printf("UT3D_vcn_cvo typ=%d form=%d siz=%d\n",oi->typ,oi->form,oi->siz);


  if(oi->typ != Typ_CV) goto L_err;


  o1 = DB_GetCurv ((long)oi->data);
  // UTO_dump__(o1, "-----:");


  if(o1->typ == Typ_CVCCV) {

    // get Normalvec u. 1. Punkt von CCV
    irc = UT3D_vcn_ccv (vco, pto, o1, wrkSpc);
    goto L_fertig;


  } else goto L_err;



  L_fertig:
    // GR_Disp_vc (vco, pto, 2, 0);
    return irc;


  L_err:
    TX_Error("UT3D_vcn_cvo E001_%d_%d",oi->typ,oi->form);
    return -1;

}


//=======================================================================
  void* UTO_obj_save (Memspc *memSpc, ObjGX *oxi) {
//=======================================================================
/// \code
/// UTO_obj_save   DO NOT USE         copy complexObj -> Memspc
/// TODO: make a new OGX_ox_copyMem
/// returns Startadress
/// see also DB_store_stru
/// \endcode

// used only by IGE_r_P__


  int            form, siz;
  // void           *stPos;
  int            osiz;



  // printf("UTO_obj_save\n");
  // printf("     typ=%d form=%d siz=%d\n",oxi->typ,oxi->form,oxi->siz);
  // UTO_dump__ (oxi, oxi->siz);



  form  = oxi->form;
  siz   = oxi->siz;
  // stPos = memSpc->next;


  if(form == Typ_Index) {
    // nur speichern wenn siz > 1; else Adresse unveraendert retour.
    if(siz < 2) return oxi->data;
    osiz = sizeof(long) * siz;


  } else if(form == Typ_Txt) {
    osiz = siz; //siz in byte korrekt; nicht multpliz!

  } else {
    osiz = UTO_siz_stru (form);
    if(osiz < 1) return NULL;

    osiz *= siz;
  }


  // store obj
  return UME_save (memSpc, oxi->data, osiz);




  //===========================
  L_EOM:   // out of memory
  return NULL;

}


//=======================================================================
  int UTO_invert (ObjGX *oxi) {
//=======================================================================
/// \code
/// UTO_invert              Obj umdrehen
/// implem.: Typ_LN Typ_CI
/// \endcode

  if(oxi->typ == Typ_LN) {
    UT3D_ln_inv ((Line*)oxi->data);

  } else if(oxi->typ == Typ_CI) {
    UT3D_ci_inv1 (oxi->data);

  } else {
    TX_Error("UTO_invert E001_%d",oxi->typ);
    return -1;
  }

  return 0;

}

/* replaced by UT3D_ptvcpar_std_obj
//================================================================
  int UTO_2pt_limstru (Point *p1, Point *p2, double *v1, double *v2,
                       int typ, void *o1) {
//================================================================
/// \code
/// UTO_2pt_limstru         get limits (startpoint, endpoint, parameters)
/// 
/// Input:
///   p1        startpoint (none if p1 == NULL)
///   p2        endpoint (none if p2 == NULL)
///   v1        parameter (0-1) for startpoint (v1=NULL: none)
///   v2        parameter (0-1) for endpoint (v2=NULL: none)
/// 
/// Output:
///   *v1 u *v2 only for CVBSP, CVPOL ..
/// 
/// see also UTO_2pt_lim_ox UTO_2pt_lim_dbo UTO_pt_ox
/// \endcode

  int    i1;
  double d1;
  Point  pa[2];


  // printf("UTO_2pt_limstru typ=%d\n",typ);
  // UT3D_stru_dump (typ, o1, "UTO_2pt_limstru\n");


  switch (typ) {

    case Typ_PT:
      if(p1) *p1 = *((Point*)o1);
      if(p2) *p2 = *((Point*)o1);
      goto L_v_def;

    case Typ_LN:
      if(p1) *p1 = ((Line*)o1)->p1;
      if(p2) *p2 = ((Line*)o1)->p2;
      goto L_v_def;

    case Typ_CI:
      if(p1) *p1 = ((Circ*)o1)->p1;
      if(p2) *p2 = ((Circ*)o1)->p2;
      goto L_v_def;

    case Typ_CVELL:
      if(p1) *p1 = ((CurvElli*)o1)->p1;
      if(p2) *p2 = ((CurvElli*)o1)->p2;
      goto L_v_def;

    case Typ_CVCLOT:
      if(p1) *p1 = ((CurvClot*)o1)->stp;
      if(p2) UT3D_ptvc_evparclot (p2, NULL, 0, (CurvClot*)o1, 1.);
      break;

    case Typ_CVBSP:
      if(p1) UT3D_pt_evalparCv(p1, (CurvBSpl*)o1, ((CurvBSpl*)o1)->v0);
      if(p2) UT3D_pt_evalparCv(p2, (CurvBSpl*)o1, ((CurvBSpl*)o1)->v1);
      if(v1) *v1 = ((CurvBSpl*)o1)->v0;
      if(v2) *v2 = ((CurvBSpl*)o1)->v1;
      break;


    case Typ_CVRBSP:
      if(p1) UT3D_pt_evparCrvRBSpl(p1,&d1,(CurvRBSpl*)o1,((CurvRBSpl*)o1)->v0);
      if(p2) UT3D_pt_evparCrvRBSpl(p2,&d1,(CurvRBSpl*)o1,((CurvRBSpl*)o1)->v1);
      if(v1) *v1 = ((CurvRBSpl*)o1)->v0;
      if(v2) *v2 = ((CurvRBSpl*)o1)->v1;
      break;


    case Typ_CVPOL:
      i1  = ((CurvPoly*)o1)->ptNr - 1;
        // if(p1) *p1 = ((CurvPoly*)o1)->cpTab[0];
        // if(p2) *p2 = ((CurvPoly*)o1)->cpTab[i1];
        if(p1) UT3D_pt_evalplg (p1, o1, ((CurvPoly*)o1)->v0);
        if(p2) UT3D_pt_evalplg (p2, o1, ((CurvPoly*)o1)->v1);
      // if(v1) *v1 = ((CurvPoly*)o1)->lvTab[0];
      // if(v2) *v2 = ((CurvPoly*)o1)->lvTab[i1];
      if(v1) *v1 = ((CurvPoly*)o1)->v0;
      if(v2) *v2 = ((CurvPoly*)o1)->v1;
      break;


    // ccv & surfcaes & solids have Typ_ObjGX!
    case Typ_ObjGX:           // Typ_CVCCV
        printf(" typ=%d\n",((ObjGX*)o1)->typ);
      if(((ObjGX*)o1)->typ != Typ_CVCCV) {
        TX_Error("UTO_2pt_limstru E002 %d",typ);
        return -1;
      }
      UT3D_pt_endptccv (pa, o1);
      if(p1) *p1 = pa[0];
      if(p2) *p2 = pa[1];
      break;



    default:
      TX_Error("UTO_2pt_limstru E001 %d",typ);
      return -1;
  }

    // GR_Disp_pt (p1, SYM_TRI_S, 2);
    // GR_Disp_pt (p2, SYM_TRI_S, 2);
    // if(p1) UT3D_stru_dump (Typ_PT, p1, "  _limstru p1\n");
    // if(p2) UT3D_stru_dump (Typ_PT, p2, "  _limstru p2\n");

  return 0;


  L_v_def:
    // structs Line, Circ and CurvElli are independant from parent-objects;
    // the startparameter is always 0 und endparameter always is 1. 
    if(v1) *v1 = 0.;
    if(v2) *v2 = 1.;
    return 0;

    

}


// replaced by UT3D_ptvcpar_std_obj
//================================================================
  int UTO_pt_ox (Point **pto, ObjGX *oxi) {
//================================================================
/// \code
/// get typical point for complex-obj (standardpoints, characteristic points)
/// Typ_PT,Typ_GTXT,Typ_ATXT: position.
/// Curves: startpoint
/// see also UTO_ptnr_std_obj UTO_2pt_limstru
/// \endcode

// see APT_disp_nam UTO_pt_objind APT_DrawLine ..

  int    i1, typ;
  void   *obj;
  Point  pt1;
  static Point  ptx;


  typ = UTO_obj_getp (&obj, &i1, oxi);

  // printf("UTO_pt_ox: _obj_getp typ=%d\n",typ);



  //----------------------------------------------------------------
  if(typ == Typ_PT) {
    *pto = (Point*)obj;


  //----------------------------------------------------------------
  // Typ_CI
  } else if(typ == Typ_CI) {
    *pto = &(((Circ*)obj)->p1);


  //----------------------------------------------------------------
  // Typ_Note
  } else if(typ == Typ_GTXT) {
      // UT3D_stru_dump (Typ_GTXT, obj, "GText:");
    *pto = &(((GText*)obj)->pt);


  //----------------------------------------------------------------
  } else if(typ == Typ_ATXT) {
      // UT3D_stru_dump (Typ_GTXT, obj, "GText:");
    *pto = &(((AText*)obj)->p1);


  //----------------------------------------------------------------
  // Typ_Model
  } else if(typ == Typ_Model) {
      // UT3D_stru_dump (Typ_GTXT, obj, "GText:");
    *pto = &(((ModelRef*)obj)->po);


  //----------------------------------------------------------------
  // Curves:
  } else if((typ == Typ_CVPOL)  ||
            (typ == Typ_CVBSP)  ||
            (typ == Typ_CVELL)  ||
            (typ == Typ_CVCLOT) ||
            (typ == Typ_CVCCV))   {
    UTO_2pt_limstru (&ptx, NULL, NULL, NULL, typ, obj);
    *pto = &ptx;



  //----------------------------------------------------------------
  } else goto L_err1;


  return 0;


  L_err1:
    // printf ("UTO_pt_ox E001 %d %d\n",oxi->typ,oxi->form);
    TX_Print ("UTO_pt_ox E001 %d %d %d",typ,oxi->typ,oxi->form);
    return -1;
}

 
// UNUSED - use UT3D_ptvc_std_obj
//=======================================================================
  int UTO_pt_objind (Point *pto, int typ, long ind, int ptNr) {
//=======================================================================
/// get point from object and ptNr

  int   i1;
  void  *odb;

  
  if(typ == Typ_PT) {
    *pto = DB_GetPoint (ind);


  } else if(typ == Typ_LN) {
    odb = (Line*)DB_get_LN (ind);
    if(ptNr == 0) {
      *pto = ((Line*)odb)->p1;
    } else {
      *pto = ((Line*)odb)->p2;
    }



  } else if(typ == Typ_CI) {
    odb = (Circ*)DB_get_CI (ind);
    if(ptNr == 0) {
      *pto = ((Circ*)odb)->p1;
    } else {
      *pto = ((Circ*)odb)->p2;
    }


  } else if(typ == Typ_CV) {
    // ox1 = DB_GetCurv (ind);
    odb = DB_get_CV (&i1, ind);
    // UT3D_stru_dump(i1,odb,"obj-CV:");
    if(i1 == Typ_CVPOL) {
      *pto = ((CurvPoly*)odb)->cpTab[ptNr];
    } else {
      TX_Error("UTO_pt_objind E002 %d %d",typ,i1);
      return -1;
    }



  } else {
    TX_Error("UTO_pt_objind E001 %d",typ);
    return -1;
  }




  return 0;

}
*/

//=======================================================================
  int UTO_ptnr_std_obj (int typ, long ind) {
//=======================================================================
/// \code
/// UTO_ptnr_std_obj        get nr of characteristic points for obj
/// get points with UT3D_ptvcpar_std_obj
/// see UT3D_pt_std_ci IE_cad_Inp1_nxtpNr
/// \endcode

  int   i1, iNr;
  void  *odb;

  // printf("UTO_ptnr_std_obj typ=%d ind=%d\n",typ,ind);

  
  if(typ < Typ_CV) {

    if      (typ == Typ_LN) iNr = 3;
    else if(typ == Typ_CI) iNr = 3;
    else goto L_E001;
    goto L_exit;
  }



  if(typ < Typ_PLN) {
    // curves
    odb = DB_get_CV (&i1, ind);
      // UT3D_stru_dump(i1,odb,"obj-CV:");
    if     (i1 == Typ_CVELL)  iNr = 5;
    else if(i1 == Typ_CVCLOT) iNr = 2;
    else if(i1 == Typ_CVPOL)  iNr = ((CurvPoly*)odb)->ptNr;
    else if(i1 == Typ_CVBSP)  iNr = ((CurvBSpl*)odb)->ptNr;
    else goto L_E002;
    goto L_exit;

  } else goto L_E001;



  L_exit:
    // printf("ex UTO_ptnr_std_obj %d %d %ld\n",iNr,typ,ind);
  return iNr;


  //----------------------------------------------------------------
  L_E001:
    TX_Print("UTO_ptnr_std_obj E001 %d",typ);
    return -1;

  L_E002:
    TX_Error("UTO_ptnr_std_obj E002 %d %d",typ,i1);
    return -1;
}


/* replaced by UT3D_ptvcpar_std_dbo
//=======================================================================
  int UTO_2pt_lim_dbo (Point *ps, Point *pe, double *v1, double *v2,
                      int typ, long ind) {
//=======================================================================
/// \code
/// UTO_2pt_lim_dbo      get limits (startpoint, endpoint, parameters) of DB-obj
/// Input:
///   ps        startpoint (none if p1 == NULL)
///   pe        endpoint (none if p2 == NULL)
///   v1        parameter (0-1) for startpoint (v1=NULL: none)
///   v2        parameter (0-1) for endpoint (v2=NULL: none)
/// 
///
/// see also UTO_2pt_lim_ox UTO_2pt_limstru
/// \endcode



  ObjGX   oxi;


  printf("UTO_2pt_lim_dbo typ=%d ind=%ld\n",typ,ind);

  OGX_SET_INDEX (&oxi, typ, ind);

  return UTO_2pt_lim_ox (ps, pe, v1, v2, &oxi);

}


// replaced by UT3D_ptvcpar_std_obj
//===============================================================================
  int UTO_2pt_lim_ox (Point *ps, Point *pe, double *v1, double *v2, ObjGX *oxi) {
//===============================================================================
/// \code
/// UTO_2pt_lim_ox           get start- or endpoint of line/curve..
/// Output:
///   ps        startpoint (none if p1 == NULL)
///   pe        endpoint (none if p2 == NULL)
///   v1        parameter (0-1) for startpoint (v1=NULL: none)
///   v2        parameter (0-1) for endpoint (v2=NULL: none)
///
/// see also UTO_2pt_limstru
/// \endcode

  int      irc, ityp, oNr;
  long     dbi;
  Point    p1, p2;
  ObjGX    o1, *oTab, *ox;


  UT3D_stru_dump (Typ_ObjGX, oxi, "UTO_2pt_lim_ox:");
  // UTO_dump__ (oxi, " _ptlim-oxi");

  // get data for DB-objs
  o1 = *oxi;

  if(o1.typ == Typ_CVCCV) {
      // get 1. record of CCV
      // TODO: test for a trimRecord
      oTab = oxi->data;
      ox = &oTab[0];
        UT3D_stru_dump(Typ_ObjGX, ox," _ptlim-oxi[0]");
      if(ox->form != Typ_Index) {TX_Error("UTO_2pt_lim_ox E001"); return -1;}
      o1.form = DB_GetObjDat (&o1.data, &oNr, ox->typ, LONG_PTR(ox->data));


  } else if(o1.form == Typ_Index) {
    OGX_GET_INDEX (&ityp, &dbi, oxi);
    o1.form = DB_GetObjDat (&o1.data, &oNr, ityp, dbi);
  }


  irc = UTO_2pt_limstru (ps, pe, v1, v2, o1.form, o1.data);
  if(irc < 0) return irc;

    // printf("ex UTO_2pt_lim_ox %d %f,%f,%f\n",mode,pte->x,pte->y,pte->z);

  return 0;

  L_err2:
    TX_Error("UTO_2pt_lim_ox E002 %d %d",oxi->typ,oxi->form);
    return -1;

  L_err3:
    TX_Error("UTO_2pt_lim_ox E003 %d %d",oxi->typ,oxi->form);
    return -1;

}


// UNUSED
//===================================================================
  int UTO2_pt_par1_obj (Point2 *pto, double du, int typ, void *obj) {
//===================================================================
// UTO2_pt_par1_obj           get point from 0-1-parameter, obj
// paramter must be between 0. and 1.  (except polygons)

// was UTO_pt_evpar1ost
// see also UTO_pt_eval_par1_dbo


  printf("UTO2_pt_par1_obj %lf %d\n",du,typ);

  switch (typ) {

    case Typ_LN2:
      UT2D_pt_traPtPtPar (pto, &((Line2*)obj)->p1, &((Line2*)obj)->p2, du);
      return 0;

    case Typ_CI2:
      return UT2D_pt_par_ci  (pto, du, obj);

  }

  TX_Error("UTO2_pt_par1_obj E001 %d",typ);
  return -1;

}
*/

/* REPLACED with UT3D_ptvc_tng_crv_par
//===================================================================
  int UTO_pt_par1_obj (Point *pto, double du, int typ, void *obj) {
//===================================================================
// UTO_pt_par1_obj           get point from 0-1-parameter, obj
// paramter must be between 0. and 1.  (except polygons)

// was UTO_pt_evpar1ost
// see also UTO_pt_eval_par1_dbo


  printf("UTO_pt_par1_obj %lf %d\n",du,typ);

  switch (typ) {

    case Typ_LN:
      // UT2D_pt_parvc_pt_vc UT2D_pt_traPtPtPar UT3D_pt_evpar2pt UT3D_pt_evparptcv
      // UT3D_pt_evparln 
      return UT3D_pt_evpar2pt (pto, du, &((Line*)obj)->p1, &((Line*)obj)->p2);

    case Typ_CI:
      return UT3D_pt_evparci  (pto, du, obj);

      // UT2D_pt_elangd

    case Typ_CVELL:    // CurvElli
      // UT3D_pt_elangd UT3D_pt_eval_ell_par1 UT3D_pt_evparcrv
      return UT3D_pt_eval_ell_par1 (pto, obj, du);

    case Typ_CVPOL:    // CurvPoly
      // UT3D_pt_evalplg UT3D_pt_evparcrv
      // change par 0-1  -->  dist
      du = UT3D_par_par1plg (du, obj);
        // printf(" lpar=%f\n",lpar);
      return UT3D_pt_evalplg (pto, obj, du);

    case Typ_CVCLOT:
      return UT3D_ptvc_evparclot (pto, NULL, 0, obj, du);

    case Typ_CVBSP:
      // fuer curves den parameter in einem Bereich 0-1 bringen ..
      du = UT3D_parbsp_par1 (du, obj);
      return UT3D_pt_evparCrvBSpl (pto, obj, du);
      // UT2D_pt_evpar_cbsp UT3D_pt_evparCrvBSpl UT3D_pt_evalparCv
      // bspl_pt_cbspDeg1

  }

  TX_Error("UTO_pt_par1_obj E001 %d",typ);
  return -1;

}
*/

//=======================================================================
  int UTO_pt_eval_par1_dbo (Point *pto, double lpar, int typ, long ind) {
//=======================================================================
/// \code
/// UTO_pt_eval_par1_dbo          get point from 0-1-parameter (curve from typ/ind)
/// ACHTUNG: lpar muss im Bereich 0-1 sein; wird fuer Curves umgerechnet !!
/// 
/// Input:
///   typ    DB-typ
///   ind    DB-index
/// 
/// see also UT3D_pt_evparcrv UTO_obj_int UTO_stru_int UT3D_ptvc_tng_crv_par
/// \endcode


  double UT3D_parbsp_par1();

  // void  *oStru;
  ObjGX ox1;

  // printf("UTO_pt_eval_par1_dbo %f %d %d\n",lpar,typ,ind);


  ox1 = DB_GetObjGX (typ, ind);
  // printf(" typ=%d form=%d\n",ox1.typ,ox1.form);
  // UT3D_stru_dump (ox1.form, ox1.data, "");


  // return UT3D_ptvc_tng_crv_par (pto, lpar, ox1.form, ox1.data);
  return UT3D_ptvc_tng_crv_par (pto, NULL, ox1.form, ox1.data, 0, lpar);

/*
  // fuer curves den parameter in einem Bereich 0-1 bringen ..
  if(typ == Typ_CV) {
    if(ox1.form == Typ_CVBSP) {
      lpar = UT3D_parbsp_par1 (lpar, ox1.data);
      // printf(" kv=%f <-- par\n",lpar);
    // } else if(ox1.form == Typ_CVRBSP) {
    } else TX_Print("UTO_pt_eval_par1_dbo I001");
  }


  return UT3D_pt_evparcrv (pto, lpar, ox1.form, ox1.data);
*/
}


//================================================================
  int UTO_pt_midnobj (Point *pto, ObjGX *oa, int oNr) {
//================================================================
/// \code
/// get midpoint of n objects
/// den Mittelpunkt der EingabeElemente suchen;
/// 
/// see UT3D_pt_gcp_tria
/// \endcode


  int     i1, i2, oTyp;
  Point   pt1, pt2;
  Point2  pt21;


  pt1 = UT3D_PT_NUL;
  i2 = 0;

  for(i1=0; i1<oNr; ++i1) {

    oTyp = oa[i1].form;

    if(oTyp == Typ_PT2) {
      pt2 = UT3D_pt_pt2 (((Point2*)oa[i1].data));


    } else if(oTyp == Typ_PT) {
      pt2 = *((Point*)oa[i1].data);


    } else if(oTyp == Typ_LN2) {
      UT2D_pt_mid2pt (&pt21, &((Line2*)oa[i1].data)->p1,
                             &((Line2*)oa[i1].data)->p2);
      pt2 = UT3D_pt_pt2 (&pt21);


    } else if(oTyp == Typ_LN) {
      UT3D_pt_mid2pt (&pt2, &((Line*)oa[i1].data)->p1,
                            &((Line*)oa[i1].data)->p2);


    } else if(oTyp == Typ_CI2) {
      pt2 = UT3D_pt_pt2 (&((Circ2*)oa[i1].data)->pc);


    } else if(oTyp == Typ_CI) {
      pt2 = ((Circ*)oa[i1].data)->pc;


    } else {
      TX_Print("ERROR UTO_pt_midnobj E001 - %d",oTyp);
      continue;
    }

    UT3D_pt_addpt (&pt1, &pt2);
    ++i2;

  }


  pto->x = pt1.x / i2;
  pto->y = pt1.y / i2;
  pto->z = pt1.z / i2;


  // UT3D_stru_dump(Typ_PT2, pto, "ex UTO_pt_midnobj");
  // GR_Disp_pt (pto, SYM_STAR_S, 2);


  return 0;

}


//=================================================================
  int UTO_parpt_pt_dbo (double *lpar, Point *pti, int typ, long ind) {
//=================================================================
/// \code
/// UTO_parpt_pt_dbo           get parameter from point (obj from typ/ind)
/// ACHTUNG: lpar muss im Bereich 0-1 sein; wird fuer Curves umgerechnet !!
/// see UT3D_pt_evparcrv
/// \endcode

  double UT3D_parbsp_par1();

  ObjGX ox1;


  // UT3D_stru_dump (Typ_PT, pti, "UTO_parpt_pt_dbo %d %d:",typ,ind);


  ox1 = DB_GetObjGX (typ, ind);
  // printf(" typ=%d form=%d\n",ox1.typ,ox1.form);
  // UT3D_stru_dump (ox1.form, ox1.data, "");


  // get paramter from curve-obj (immer Werte 0-1) !
  UTO_parpt_pt_obj (lpar, pti, ox1.form, ox1.data);


  // printf("ex UTO_parpt_pt_dbo %f\n",*lpar);
  return 0;

}


//===========================================================================
  int UTO_set_ptlim (int typ, void *oo,
                       Point *p1, double *v1,
                       Point *p2, double *v2) {
//===========================================================================
/// \code
/// UTO_set_ptlim         set start- and/or endpoint of a struct of type <typ>
/// Input:
///   typ, oo       type and struct of obj
///   p1            startpoint; NULL=undefined
///   v1            parameter startpoint; NULL=undefined
///   p2            endpoint; NULL=undefined
///   v2            parameter endpoint; NULL=undefined
/// 
/// LN:       p1, p2 used;   v1, v2;
/// AC:       p1, p2 used;   v1, v2;
/// CVELL:    p1, p2 used;   v1, v2;
/// CVPOL:    v1, v2 used;   else p1, p2 used;
/// CVBSP:    v1, v2 used;   else p1, p2 used;
/// CVRBSP:   v1, v2 used;   p1, p2;
/// 
/// see 
/// \endcode

// LimitTyp: see UTO_ck_curvLimTyp

  double     d1;



  // UT3D_stru_dump (typ, oo, "UTO_set_ptlim typ=%d\n",typ);
  // if(p1) UT3D_stru_dump (Typ_PT, p1, "p1 ");
  // if(v1) printf(" v1=%lf\n",*v1);
  // if(p2) UT3D_stru_dump (Typ_PT, p2, "p2 ");
  // if(v2) printf(" v2=%lf\n",*v2);


  switch (typ) {

    case Typ_LN:                 // limited by points
      if(p1) ((Line*)oo)->p1 = *p1;
      if(p2) ((Line*)oo)->p2 = *p2;
      break;


    case Typ_CI:
      // UT3D_ci_cip3 ((Circ*)oo, p1, p2);
      if(p1) ((Circ*)oo)->p1 = *p1;
      if(p2) ((Circ*)oo)->p2 = *p2;
      ((Circ*)oo)->ango = UT3D_angr_ci_p1_pt ((Circ*)oo, &((Circ*)oo)->p2);
      // makes ango=0 for 360-deg circ !
      if(fabs(((Circ*)oo)->ango) < UT_TOL_min0) {
        if(((Circ*)oo)->rad > 0.) ((Circ*)oo)->ango = RAD_360;
        else                      ((Circ*)oo)->ango = -RAD_360;
      }
      break;


    case Typ_CVELL:
      if(p1) ((CurvElli*)oo)->p1 = *p1;
      if(p2) ((CurvElli*)oo)->p2 = *p2;
      // UT3D_el_el2pt ((CurvElli*)oo, p1, p2);
      break;


    case Typ_CVBSP:     // pt or par
      if(v1) {
        ((CurvBSpl*)oo)->v0 = *v1;
      } else if(p1) {
        // get par from pt
        UT3D_parCv_bsplpt (&((CurvBSpl*)oo)->v0, &d1, (CurvBSpl*)oo, p1);
      }
      if(v2) {
        ((CurvBSpl*)oo)->v1 = *v2;
      } else if(p2) {
        // get par from pt
        UT3D_parCv_bsplpt (&((CurvBSpl*)oo)->v1, &d1, (CurvBSpl*)oo, p2);
      }
      break;


    case Typ_CVRBSP:
      if(v1)
        ((CurvBSpl*)oo)->v0 = *v1;
      if(v2) 
        ((CurvBSpl*)oo)->v1 = *v2;
      break;


    case Typ_CVPOL:
      if(v1) ((CurvPoly*)oo)->v0 = *v1;
      else if(p1) UT3D_parplg_plgpt (&((CurvPoly*)oo)->v0, p1, (CurvPoly*)oo);
      if(v2) ((CurvPoly*)oo)->v1 = *v2;
      else if(p2) UT3D_parplg_plgpt (&((CurvPoly*)oo)->v1, p2, (CurvPoly*)oo);
        // UT3D_stru_dump (Typ_CVPOL, oo, "  _set_ptlim:\n");
      break;


    default:
      TX_Error("UTO_set_ptlim E001 %d",typ);
      return -1;
  }

  return 0;

}


/*
//=======================================================================
  int UTO_set_ptlim (ObjGX *oxo, Point *pti, int mode) {
//=======================================================================
/// \code
/// UTO_set_ptlim           Anfangs oder Endpunkt setzen (LN, AC).
/// mode = 1 = set Startpoint
/// mode = 2 = set Endpoint
/// 
/// geht nur bei direkten Daten - nicht mit Indexen !!
/// 
/// see UTO_set_ptlim
/// \endcode



  if       (oxo->typ == Typ_LN) {
     if(mode == 1) {
       ((Line*)oxo->data)->p1 = *pti;
     } else {
       ((Line*)oxo->data)->p2 = *pti;
     }


  } else if(oxo->typ == Typ_CI) {
     if(mode == 1) {
       ((Circ*)oxo->data)->p1 = *pti;
     } else {
       ((Circ*)oxo->data)->p2 = *pti;
     }


  } else {
    TX_Error("UTO_set_ptlim E001 %d",oxo->typ);
  }

  // printf("ex UTO_2pt_lim_ox %d %f,%f,%f\n",mode,pte->x,pte->y,pte->z);
  return 0;

}
*/

//================================================================
  int UTO_npt_Tes (int *pNr, Point **ppa, ObjGX *os) {
//================================================================
/// \code
/// get table of pointers from tesselated surface os
/// Input:
///   pNr   size of ppa
/// Output:
///   pNr   nr of points in ppa
/// 
/// see also: UT3D_npt_ox GL_Disp_sur
/// \endcode


  int     i1, i2, i3, ii;
  ObjGX   *actPP, *actCont;

  // printf("UTO_npt_Tes\n");


  // X-Siz/Y-Siz = MaxDist entlang X-Vec und entlang Y-Vec ermitteln
  ii = 0;
  for(i1=0; i1<os->siz; ++i1) {              // resolv: see GL_Disp_sur
    actPP = &((ObjGX*)os->data)[i1];
    if(actPP->form == Typ_PT) {
      if(ii + actPP->siz >= *pNr) goto L_eof;
      for(i2=0; i2<actPP->siz; ++i2) {
        ppa[ii] = &((Point*)actPP->data)[i2];
        ++ii;
      }
      continue;
    }
    for(i2=0; i2<actPP->siz; ++i2) {     // loop tru contours
      actCont = &((ObjGX*)actPP->data)[i2];
      // printf("i1=%d i2=%d typ=%d form=%d\n",i1,i2,actCont->typ,actCont->form);
      if(actCont->form == Typ_PT) {
        if(ii + actCont->siz >= *pNr) goto L_eof;
        for(i3=0; i3<actCont->siz; ++i3) {
          ppa[ii] = &((Point*)actCont->data)[i3];
          ++ii;
        }
      }
    }
  }

  *pNr = ii;

  // for(i1=0; i1<ii; ++i1) {
    // UT3D_stru_dump (Typ_PT, ppa[i1], "ppa[%d]=",i1);
  // }

  return 0;

  L_eof:
  printf("***** UTO_npt_Tes E001 - EOM *****\n");
  return -1;

}


/*
//================================================================
  int UTO_dump_obj (int typ, void *vp, int iNr) {
//================================================================
/// \code
/// typ   form of vp
/// vp    ist eine dem Typ entrrechende struct
/// iNr   nr of structs of typ typ in vp
/// see also UTO_dump__ UI_dump_obj
/// \endcode


static int iLev;

  int    irc, i0, i1, i2, osiz, *iTab;
  long   l1;
  void   *vp1;
  char   cbuf1[256], cbuf2[64];
  ObjGX  *oxi, *ox1, *ox2, o1;


  if(typ == TYP_FuncInit) {iLev = 0; return 0;}
  ++iLev;


  // printf("UTO_dump_obj typ=%d iNr=%d iLev=%d\n",typ,iNr,iLev);


  // check if its a ObjGX or a struct ...
  if((typ == Typ_ObjGX)  ||                       // oTab
     (typ == Typ_SURRU)  ||                       // 2 x Typ_ObjGX
     (typ == Typ_SURCIR) ||
     (typ == Typ_SURSTRIP))      goto L_ox;       // typ=SURCIR, form=ObjGX


     // (typ == Typ_CVCCV)  ||
     // (typ == Typ_SURRV)  ||                       // 2 x Typ_ObjGX
     // (typ == Typ_GTXT)   ||
     // (typ == Typ_SURBSP) ||// 158 158 hat direkt data !



  // unresolvable objects:
  if((typ == Typ_Error))  {
    UT3D_stru_dump(typ, vp, "");
    goto L_done;
  }


  osiz = UTO_siz_stru (typ);
  if(osiz < 1) return -1;
    // printf(" typ=%d osiz=%d\n",typ,osiz);

  
  // display struct(s)
  for(i1=0; i1<iNr; ++i1) {
    if(iNr > 1) sprintf(cbuf2," o[%d]",i1);
    else  cbuf2[0] = '\0';
    UT3D_stru_dump (typ, vp, cbuf2);
    // vp = &((char*)vp)[osiz];
    // (char*)vp += osiz;
    vp = (char*)vp + osiz;
  }
  goto L_done;


  //================================================================
  // typ = ObjGX
  // Typ_ObjGX, Typ_CVCCV, Typ_SURCIR sind ObjGX
  L_ox:
  UT3D_stru_dump(Typ_ObjGX, vp, "");

  for(i0=0; i0<iNr; ++i0) {
    // printf(" next i0=%d %d -------------------------\n",i0,iNr);

    if(iNr > 1) {
      sprintf(cbuf2, "------- subObj %d / %d -------",iLev,i0+1);
      UT3D_stru_dump(Typ_Txt, cbuf2, "");
    }

    // osiz = UTO_siz_stru (oxi->form);
    // if(osiz < 1) return -1;

    oxi = &((ObjGX*)vp)[i0];
      // printf(" oxi-typ=%d form=%d siz=%d\n",oxi->typ,oxi->form,oxi->siz);


    if(oxi->form == Typ_ObjGX) {
      // auch data ist ObjGX


      //----------- typ & form = ObjGX -------------------------
      for(i1=0; i1<oxi->siz; ++i1) {
        if(oxi->siz > 1) {
          sprintf(cbuf2, "======= subObj %d / %d =======",iLev,i1+1);
          UT3D_stru_dump(Typ_Txt, cbuf2, "");
        }
        ox1 = &((ObjGX*)oxi->data)[i1];
        UT3D_stru_dump(Typ_ObjGX, ox1, "");

        if(ox1->typ == Typ_Typ) {  // form=surfTyp; zB Typ_SURPLN
          UT3D_stru_dump(ox1->form, ox1, "");
          continue;
        }

        if(ox1->form == Typ_Txt) {  // siz ist bei Typ_Txt die strlen !!
          continue;                 // Beispiel ?
        }

        if(ox1->form == Typ_Index) {
          if(ox1->siz < 2) {
              l1 = (long)ox1->data;
              // printf(" resInd-1 %d typ=%d\n",l1,oxi->typ);
              o1 = DB_GetObjGX (ox1->typ, l1);
              // printf(" o1-typ=%d form=%d siz=%d\n",o1.typ,o1.form,o1.siz);
              irc = UTO_dump_obj (o1.typ, o1.data, o1.siz);  // recurse
              if(irc < 0) return irc;
    
          } else {
            iTab = (int*)ox1->data;
            for(i1=0; i1<ox1->siz; ++i1) {
              l1 = iTab[i1];
              // printf(" resInd %d %d typ=%d\n",i1,l1,oxi->typ);
              o1 = DB_GetObjGX (ox1->typ, l1);
              // printf(" o1-typ=%d form=%d siz=%d\n",o1.typ,o1.form,o1.siz);
              irc = UTO_dump_obj (o1.typ, o1.data, o1.siz);  // recurse
              if(irc < 0) return irc;
            }
          }
          continue;
        }

        irc = UTO_dump_obj (Typ_ObjGX, ox1, 1); // recurse
        // irc = UTO_dump_obj (Typ_ObjGX, ox1, ox1->siz); // recurse
        // das folgende geht ned, weil Typ_Index aufgeloest werden muss ...
        // irc = UTO_dump_obj (ox1->form, ox1->data, ox1->siz); // recurse
        if(irc < 0) return irc;
      }


    } else if(oxi->form == Typ_Index) {
      //----------- typ ObjGX, form = form_of_struct ----------------
      // ACHTUNG: wenn siz=1, dann ist data selbst die zahl; else ist
      // Date eine Int-Tabelle !
      if(oxi->siz < 2) {
          l1 = (long)oxi->data;
          // printf(" resInd-1 %d typ=%d\n",l1,oxi->typ);
          o1 = DB_GetObjGX (oxi->typ, l1);
          irc = UTO_dump_obj (o1.typ, o1.data, o1.siz);  // recurse
          if(irc < 0) return irc;

      } else {
        iTab = (int*)oxi->data;
        for(i1=0; i1<oxi->siz; ++i1) {
          l1 = iTab[i1];
          // printf(" resInd %d %d typ=%d\n",i1,l1,oxi->typ);
          o1 = DB_GetObjGX (oxi->typ, l1);
          irc = UTO_dump_obj (o1.typ, o1.data, o1.siz);  // recurse
          if(irc < 0) return irc;
        }
      }


    } else {
      //----------- typ ObjGX, form = form_of_struct ----------------
      irc = UTO_dump_obj (oxi->form, oxi->data, oxi->siz);
      if(irc < 0) return irc;
    }
  }



  //================================================================
  L_done:
  --iLev;

  // printf("ex UTO_dump_obj %d\n",iLev);
  return 0;

}
*/


//=======================================================================
  int UTO_dump__ (ObjGX *oxi, char *txt, ...) {
//=======================================================================
/// dump complex-object (Typ_ObjGX)

  int irc;
  void     *v1;
  va_list  va;

  va_start(va,txt);
  v1  = va_arg(va,void*);
  va_end(va);


  printf("UTO_dump__ start -------------------------------\n");
    irc = UTO_dump_1 (oxi, txt, v1); 
  printf("UTO_dump__ exit  -------------------------------\n");

  return irc;

}


//=======================================================================
  int UTO_dump_1 (ObjGX *oxi, char *txt, ...) {
//=======================================================================


  int      irc, i1, i2, i3, ii, form, typ;
  long     l1, *la;
  char     cbuf[256], *p1, s1[60];
  void     *op1, *vp, *vo;
  va_list  va;
  ObjGX    ox1, *oTab, *o2;


  va_start(va,txt);
  vp  = va_arg(va,void*);
  va_end(va);

  sprintf(cbuf,txt,vp);


  printf("UTO_dump_1 |%s| typ=%d form=%d siz=%d dir=%d aux=%d\n",cbuf,
             oxi->typ,oxi->form,oxi->siz,oxi->dir,oxi->aux);


  //----------------------------------------------------------------
  if(oxi->form != Typ_ObjGX) {
    if(oxi->siz < 2) {
      // p1 = AP_src_typ__(oxi->typ);
      UT3D_stru_dump (oxi->form, oxi->data, "");
    } else {
      p1 = oxi->data;
      l1 = UTO_siz_stru (oxi->form);
      for(i3=0; i3 < oxi->siz; ++i3) {
        sprintf (s1, "[%d]", i3);
        UT3D_stru_dump (oxi->form, p1, s1);
        p1 += l1;
      }
    }
    return 0;
  }
/*
    p1 = AP_src_typ__(oxi->typ);
    if(oxi->siz < 2) {
      if(oxi->siz < 1) {    // size 0 ???
        return UT3D_stru_dump (Typ_ObjGX, oxi, p1);
      }
      return UT3D_stru_dump (oxi->form, oxi->data, p1);
    } else {
      return UT3D_nstru_dump (oxi->form, oxi->siz, oxi->data, p1);
    }
*/


  //----------------------------------------------------------------
  // resolv ObjGX
  oTab = oxi->data;
  form = oxi->form;

  for(i1=0; i1 < oxi->siz; ++i1) {
    o2 = &oTab[i1];
    printf(" data[%d] typ=%d form=%d siz=%d\n",i1,o2->typ,o2->form,o2->siz);
    // bei Typ==Typ_Size ist data die (long)size !

    if(o2->form == Typ_ObjGX) {
      i2 = UTO_dump_1 (o2,"sub");      // recurse !!
      if(i2 < 0) {
        printf("******** UTO_dump_1 E002 %d %d \n",oxi->typ,oxi->form);
        return -1;
      }

    } else if(o2->form == Typ_Index) {
      // if (nr of records == 1): index == data; else its a pointer to long*
      if(o2->siz > 1) {  // data = pointer to long*
        la = (long*)o2->data;   // 2013-12-29 for SURCIR
      } else { // data = index
        // la = &(o2->data);  // OK, but warning ..
        l1 = LONG_PTR (o2->data);
        la = &l1;
      }

      ii = 0;
      // printf("    ................ start\n");
      L_ind_n:
          printf(" [%d] = Link to DB-object %c%ld\n",ii,AP_typChar_typ(o2->typ),la[ii]);
        ox1 = DB_GetObjGX (o2->typ, la[ii]);
        i2 = UTO_dump_1 (&ox1,"subInd");
        if(i2 < 0) {
          printf("******** UTO_dump_1 E001 %d %d \n",oxi->typ,oxi->form);
          return -1;
        }
        ++ii;
        if(ii < o2->siz) goto L_ind_n;
      // printf("    ................ exit\n");


    } else if(o2->form == Typ_Txt) {
      break;  // siz ist bei Typ_Txt die strlen !!


    } else {
    if(o2->siz < 2) {
      // p1 = AP_src_typ__(oxi->typ);
      UT3D_stru_dump (o2->form, o2->data, "");
    } else { 
      p1 = o2->data;
      l1 = UTO_siz_stru (oxi->form);
      for(i3=0; i3 < oxi->siz; ++i3) { 
        sprintf (s1, "[%d]", i3);
        UT3D_stru_dump (o2->form, p1, s1);
        p1 += l1;
      }
    }
    return 0;

        // printf(" dump_1 typ=%d form=%d siz=%d\n",o2->typ,o2->form,o2->siz);
      // sprintf(cbuf, "O[%d]:",i1);
/*
      if(o2->siz < 2) {
        if(o2->siz < 1) {    // size 0 ???
          UT3D_stru_dump (Typ_ObjGX, oxi, "");
        }
        UT3D_stru_dump (o2->form, o2->data, "");
      } else {
        UT3D_nstru_dump (o2->form, o2->siz, o2->data, "");
      }
*/
    }


  }


  return 0;

}


//=======================================================================
  int UTO_dump_s_ (ObjGX *oxi, char *txt) {
//=======================================================================
/// \code
/// dump complex-obj - structured display
/// see UTO_dump__ - dump also all subObjects
/// \endcode

  int irc;

  // UTO_dump__ (oxi, "UTO_dump_s_");

  printf("=================== UTO_dump_s_ %s ============ \n",txt);
  UTO_dump_s1 (NULL, txt);         // init

  // display ParentObj
  UTO_dump_sWri (oxi, "");

  if(oxi->form == Typ_ObjGX) {
    irc = UTO_dump_s1 (oxi, txt);  // work
  }

  printf("=================== ex UTO_dump_s_ %s ============ \n",txt);

  return irc;

}


//=======================================================================
  int UTO_dump_sWri (ObjGX *oxi, char *cOff) {
//=======================================================================
/// \code
/// structured display; write Line
/// see UTO_dump_1
/// \endcode

  int   i1;
  long  l1, *iTab;
  char  auxBuf1[64], auxBuf2[64];


  // printf(" ...... UTO_dump_sWri  %d %d %d\n",oxi->typ,oxi->form,oxi->siz);


  printf("%s typ=%s(%d) form=%s(%d) siz=%d dir=%d",cOff,
                AP_src_typ__(oxi->typ),oxi->typ,
                AP_src_typ__(oxi->form), oxi->form,
                oxi->siz, oxi->dir);

  if(oxi->aux) printf(" aux=%d",oxi->aux);


  //----------------------------------------------------------------
  if(oxi->form == Typ_Index) {

    if(oxi->siz == 1) {
      l1 = (long)oxi->data;
      APED_oid_dbo__ (auxBuf1, oxi->typ, l1);
      printf(" (%s)",auxBuf1);

    } else {
      iTab = (long*)oxi->data;
      printf(" (");
      for(i1=0; i1<oxi->siz; ++i1) {
        APED_oid_dbo__ (auxBuf1, oxi->typ, iTab[i1]);
        if(i1 > 0) printf(" ");
        printf("%s",auxBuf1);
      }
      printf(")");
    }


  //----------------------------------------------------------------
  } else if(oxi->form == Typ_Int4) {

      l1 = (long)oxi->data;
      printf(" value=%ld",l1);
  }

  printf("\n");

  return 0;

}


//=======================================================================
  int UTO_dump_s1 (ObjGX *oxi, char *txt) {
//=======================================================================
/// \code
/// structured display
/// see UTO_dump_1
/// \endcode


static char cOff[64];

  int      i1, i2, i3, typ;
  void     *v1;
  char     cbuf[256];
  ObjGX    ox1, *op1, *op2;


  // init cOff
  if(oxi == NULL) {strcpy(cOff,"  "); return 0;}



  // printf("UTO_dump_s1 %s typ=%d form=%d siz=%d dir=%d\n",txt,
                        // oxi->typ,oxi->form,oxi->siz,oxi->dir);



  // display ParentObj
  if(oxi->form != Typ_ObjGX) {
    UTO_dump_sWri (oxi, cOff);     // print obj
    // muss bleiben fuer zB Subrecords of CCV
    return 0;
  }



  // loop tru obj's
  for(i1=0; i1<oxi->siz; ++i1) {
    op1 = &((ObjGX*)oxi->data)[i1];
    // printf(" ..[%d] %d %d %d\n",i1,op1->typ,op1->form,op1->siz);

    UTO_dump_sWri (op1, cOff);     // print obj

    if(op1->form == Typ_ObjGX) {
      strcat(cOff, "  ");
      for(i2=0; i2<op1->siz; ++i2) {
        op2 = &((ObjGX*)op1->data)[i2];
        i3 = UTO_dump_s1 (op2,"");    // recurse
        if(i3 < 0) {
          printf("******** UTO_dump_1 E002 %d %d \n",oxi->typ,oxi->form);
          return -1;
        }
      }
      i2 = strlen(cOff) - 2; if(i2 > 0) cOff[i2] = '\0';


    } else if(op1->form == Typ_Index) {
      // aufloesen ..
      if(op1->siz < 2) // Index else (long*)-Tabelle
        ox1 = DB_GetObjGX (op1->typ, (long)op1->data);
      else
        ox1 = *op1;

      strcat(cOff, "  ");
      UTO_dump_sWri (&ox1, cOff);     // print obj

      if(ox1.form == Typ_ObjGX) {
        // printf("    ................ start\n");
        strcat(cOff, "  ");
        i2 = UTO_dump_s1 (&ox1,"");     // recurse
        if(i2 < 0) {
          printf("******** UTO_dump_1 E001 %d %d \n",oxi->typ,oxi->form);
          return -1;
        }
        // printf("    ................ exit\n");
        i2 = strlen(cOff) - 2; if(i2 > 0) cOff[i2] = '\0';
      }
      i2 = strlen(cOff) - 2; if(i2 > 0) cOff[i2] = '\0';
    }

    // if(op1->form == Typ_Txt) break;  // siz ist bei Typ_Txt die strlen !!

  }


  return 0;

}


//================================================================
  int UTO_ck_nearptnobj (ObjGX *oa, int oNr, Point *pt1) {
//================================================================
/// \code
/// find object nearest to point
/// den Kreis, dessen Mittelpunkt pt21 am naechsten ist, auswaehlen
///  give back its index
/// \endcode

  int     i1, oTyp, ii;
  double  d1, d2;
  Point   pth;
  Point2  pt2h;


  // printf("UTO_ck_nearptnobj %d\n",oNr);

  d1 = UT_VAL_MAX;

  for(i1=0; i1<oNr; ++i1) {
    // UT3D_stru_dump (Typ_ObjGX, &oa[i1], "oa[%d]=",i1);

    oTyp = oa[i1].form;


    if(oTyp == Typ_LN2) {
      // den min. Abstand pt21-Mittelpunkt suchen
      UT2D_pt_mid2pt (&pt2h, &((Line2*)oa[i1].data)->p1,
                             &((Line2*)oa[i1].data)->p2);
      pth = UT3D_pt_pt2 (&pt2h);
      d2 = UT3D_lenq_PtPt (pt1, &pth);
      if(d2 < d1) {d1 = d2; ii = i1;}


    } else if(oTyp == Typ_LN) {
      // den min. Abstand pt21-Mittelpunkt suchen
      UT3D_pt_mid2pt (&pth, &((Line*)oa[i1].data)->p1,
                            &((Line*)oa[i1].data)->p2);
      d2 = UT3D_lenq_PtPt (pt1, &pth);
      if(d2 < d1) {d1 = d2; ii = i1;}


    } else if(oTyp == Typ_CI2) {
      // den min. Abstand pt21-Mittelpunkt suchen
      pth = UT3D_pt_pt2 (&((Circ2*)oa[i1].data)->pc);
      d2 = UT3D_lenq_PtPt (pt1, &pth);
      if(d2 < d1) {d1 = d2; ii = i1;}


    } else if(oTyp == Typ_CI) {
      // den min. Abstand pt21-Mittelpunkt suchen
      pth = ((Circ*)oa[i1].data)->pc;
      d2 = UT3D_lenq_PtPt (pt1, &pth);
      if(d2 < d1) {d1 = d2; ii = i1;}


    } else {
      TX_Print("ERROR UTO_ck_nearptnobj E001 - %d",oTyp);
      continue;
    }

  }

  return ii;

}

/*
//================================================================
  int UTO_ck_dbsTyp (int typ) {
//================================================================
/// \code
/// UTO_ck_dbsTyp           check object-typ (struct or object)
///   typ              must be a basictype; see AP_typ_2_bastyp
/// 0 = data-struct    (Point, Line, Circ, Vector)
/// 1 = complex object (ObjGX, Curve or Surf or GTXT)
/// \endcode


  switch(typ) {
    // im folgenden alle Typen, die in der DB als ObjGX gespeichert sind
    // (ex UTO_get_DB ein ObjGX liefern)
    case Typ_CV:
    case Typ_SUR:
    case Typ_SOL:
    case Typ_GTXT:
        // printf("ex UTO_ck_dbsTyp 1 from %d\n",typ);
      return 1;
  }

  // alle folgenden liefern datastruct;i
  // diese zB in UT3D_stru_dump registrieren !!!
    // printf("ex UTO_ck_dbsTyp 0 from %d\n",typ);
  return 0;

}

//================================================================
  int UTO_ck_typTyp (int typ) {
//================================================================
/// \code
/// UTO_ck_typTyp     check if typ=curve (Typ_lFig) or surface (Typ_SUR)
/// Retcod = Typ_lFig    from LN|AC|CV     (L/C/S)
///          Typ_SUR     from PLN|SUR|SOL  (R/A/B)
/// \endcode

  switch(typ) {
    case Typ_LN:
    case Typ_CI:
    case Typ_CV:
      return Typ_lFig;


    case Typ_PLN:
    case Typ_SUR:
    case Typ_SOL:
      return Typ_SUR;

  }

  // alle folgenden liefern Points
  return 0;

}

 
//================================================================
  int UTO_ck_curvLimTyp (int typ) {
//================================================================
/// \code
/// UTO_ck_curvLimTyp              check if CurveLimits are Points or parameters
/// RetCode:
///   0   die CurveLimits basieren auf Points  (LN, AC, CVELL usw)
///   1   die CurveLimits basieren auf Parameters (CVPOL, CVBSP)
/// \endcode


  switch(typ) {
    case Typ_CVBSP:
    case Typ_CVPOL:
      return 1;
  }

  // alle folgenden liefern Points
  return 0;

}


//================================================================
  int UTO_ck_curvForm (int typ) {
//================================================================
/// \code
/// UTO_ck_curvForm           check if objTyp is ObjGX or other typ
/// Output:
///   RetCode:   Typ_ObjGX (for Typ_CVPOL,Typ_CVBSP,Typ_CVELL ..)
///              else typ
/// \endcode


  switch(typ) {
    case Typ_CVPOL:
    case Typ_CVBSP:
    case Typ_CVRBSP:
    case Typ_CVELL:
    case Typ_CVCLOT:
    case Typ_CVCCV:

      return Typ_ObjGX;


    default:
      // obj without ObjGX:
      // Typ_PT
      // Typ_LN
      // Typ_CI
      // Typ_CI
      return typ;

  }

}


//================================================================
  int UTO_ck_surfTyp (ObjGX *oxi) {
//================================================================
/// \code
/// UTO_ck_surfTyp          returns surfSubTyp
/// Input:
///  oxi      su_tab-record
/// Output:
///  retCod   Typ_SURTPS|Typ_SURBSP|Typ_SURRU|Typ_SURRV ..
/// 
/// see also SUR_ck_typ
/// see GR_DrawSur TSU_DrawSurT_
/// \endcode


  int   ityp;
  ObjGX *ox1;

  // printf("UTO_ck_surfTyp %d %d\n",oxi->typ,oxi->form);
  // UT3D_stru_dump (Typ_ObjGX, oxi, "");
  // UTO_dump__ (oxi, "");

  // if(oxi->typ == Typ_SURPTAB) return Typ_SURPTAB;


  if(oxi->typ == Typ_SUR) {    // eg Typ_PT  from   A=PTAB  
    ityp = oxi->form;
    if(ityp == Typ_ObjGX) {     // planar od gelocht ..
      // form of 1.subObj: Typ_SURPLN or Index of supportSurface
      ox1 = (ObjGX*)oxi->data;
      ityp = ox1->form;
      if(ityp != Typ_SURPLN) ityp = Typ_SURTPS;
    }
    goto L_exit;

  } else {
    ityp = oxi->typ;
    goto L_exit;
  }



  return -1;


  L_exit:
    // printf("ex UTO_ck_surfTyp %d\n",ityp);
  return ityp;

}
*/


//================================================================
  int UTO_siz_stru (int form) {
//================================================================
/// \code
/// UTO_siz_stru            give size of struct
/// form does not have size: 0 returned.
/// \endcode

  int  osiz;


  switch(form) {

    case Typ_PT:         osiz = sizeof(Point);      break;
    case Typ_VC:         osiz = sizeof(Vector);     break;
    case Typ_LN:         osiz = sizeof(Line);       break;
    case Typ_CI:         osiz = sizeof(Circ);       break;
    case Typ_CVPOL:      osiz = sizeof(CurvPoly);   break;
    case Typ_CVELL:      osiz = sizeof(CurvElli);   break;
    case Typ_CVCLOT:     osiz = sizeof(CurvClot);   break;
    case Typ_CVBSP:      osiz = sizeof(CurvBSpl);   break;
    case Typ_CVRBSP:     osiz = sizeof(CurvRBSpl);  break;
    case Typ_CVPSP3:     osiz = sizeof(polynom_d3); break;
    // case Typ_CVComp:     osiz = sizeof(CurvCCV);   break;
    case Typ_CVCCV:      osiz = sizeof(CurvCCV);    break;

    case Typ_GTXT:       osiz = sizeof(GText);      break;
    case Typ_Tag:
    case Typ_ATXT:       osiz = sizeof(AText);      break;
    case Typ_Dimen:      osiz = sizeof(Dimen);      break;
    case Typ_Dim3:       osiz = sizeof(Dim3);       break;

    case Typ_Polynom_:   osiz = sizeof(Polynom_);   break;
    case Typ_Polynom1:   osiz = sizeof(Polynom3);   break;
    case Typ_Polynom3:   osiz = sizeof(Polynom3);   break;
    case Typ_polynom_d3: osiz = sizeof(polynom_d3); break;
    case Typ_TraRot:     osiz = sizeof(TraRot);     break;
    case Typ_PLN:        osiz = sizeof(Plane);      break;
    case Typ_SURRV:      osiz = sizeof(SurRev);     break;
    case Typ_SURSWP:     osiz = sizeof(SurSwp);     break;
    case Typ_SURBSP:     osiz = sizeof(SurBSpl);    break;
    case Typ_SURRBSP:    osiz = sizeof(SurRBSpl);   break;
    case Typ_SURHAT:     osiz = sizeof(SurHat);     break;
    case Typ_SURSTRIP:   osiz = sizeof(SurStripe);  break;
    case Typ_CON:        osiz = sizeof(Conus);      break;
    case Typ_TOR:        osiz = sizeof(Torus);      break;
    case Typ_SPH:        osiz = sizeof(Sphere);     break;
    case Typ_Mock:
    case Typ_Model:      osiz = sizeof(ModelRef);   break;
    case Typ_ObjGX:      osiz = sizeof(ObjGX);      break;
    case Typ_ObjSRC:     osiz = sizeof(ObjSRC);     break;
    case Typ_Val:        osiz = sizeof(double);     break;
    case Typ_VAR:        osiz = sizeof(double);     break;
    case Typ_Int4:
    case Typ_Index:      osiz = sizeof(long);       break;
    case Typ_Txt:        osiz = 1;                  break;
    case Typ_PT2:        osiz = sizeof(Point2);     break;
    case Typ_LN2:        osiz = sizeof(Line2);      break;
    case Typ_CI2:        osiz = sizeof(Circ2);      break;
    case Typ_Activ:      osiz = sizeof(Activity);   break;

    default:
    osiz = 0;
    TX_Print("UTO_siz_stru E001_%d",form);
  }


  // printf(" ex UTO_siz_stru osiz=%d form=%d\n",osiz,form);

  return osiz;

}


//=======================================================================
   int UTO_get_DB (void **oxo, int *oNr, int *otyp, long dbInd) { 
//=======================================================================
/// \code
///
/// UTO_get_DB              get DB-data-struct from typ, DB-index
///   Returns a pointer to the data-struct.
///   Does NOT resolve objRefs (Typ_Index).
/// Input: 
///   otyp            dbtyp
///   dbInd           DB-index
/// Output: 
///   otyp            wird bei Typ_CV auf die genaue ObjType korrigiert;
///                   bei Typ_SUR --> Typ_ObjGX
///   oxo             Pointer auf die korrekte struct - NICHT auf ObjGX !
///   oNr             number of structs in oxo
///   RetCod          0=OK, data-struct;
///                   1=OK, complexObj
///                   -1=Error (dbo not defined)
///
///
///  otyp-in     otyp-out    struct   Grouptype
///  Typ_PT      Typ_PT      Point       0
///  Typ_VC      Typ_VC      Vector      0
///  Typ_LN      Typ_LN      Line        0
///  Typ_CI      Typ_CI      Circ        0
///  Typ_PLN     Typ_PLN     Plane       0
///  Typ_CVPOL   Typ_CVPOL   CurvPoly    0
///  Typ_CVELL   Typ_CVELL   CurvElli    0
///  Typ_CVBSP   Typ_CVBSP   CurvBSpl    0
///  Typ_CVRBSP  Typ_CVRBSP  CurvRBSpl   0
///  Typ_CVCLOT  Typ_CVCLOT  CurvClot    0
///  Typ_CVCCV   Typ_CVCCV   CurvCCV     1     // was ObjGX until 2014-06-04
///  Typ_SUR     Typ_SUR     ObjGX       1
///  Typ_SOL     Typ_SOL     ObjGX       1
///  Typ_Tra     Typ_Tra     ObjGX       1
///  Typ_Note   <Typ_GTXT    GText       0>
///  Typ_Note   <Typ_ATXT    AText       0>
///  Typ_Note   <Typ_Tag     AText       0>
///  Typ_Model   Typ_Model   ModelRef    0
///  Typ_VAR     Typ_VAR     double      0
///  Typ_Activ   Typ_Activ   Activity    0
///
///
/// see also:
/// DB_GetObjGX    get complexObj from DB-Obj Keep Refs.   Give ObjGX.
/// UTO_get_DB     get dataStruct from DB-Obj Keep Refs.   Give pointer.
/// DB_GetObjDat   get dataStruct from DB-Obj Resolv Refs. Give pointer.
///
/// UTO_obj_getp   get dataStruct from ObjGX  Resolv Refs. Give pointer.
/// UTO_obj_get    get dataStruct from ObjGX  Resolv Refs. Give a copy.
/// \endcode

// TODO:
// Typ_Note returns the ObjGX; Typ_GTXT also returns its ObjGX.
//   Better: Typ_GTXT returns its struct; Typ_Note returns the ObjGX.
//   for that AP_typ_2_bastyp should not be called first.




  int     irc, typ;
  ObjGX   *ox1;


  // printf("UTO_get_DB %d %ld\n",*otyp,dbInd);

  typ = AP_typ_2_bastyp (*otyp);
    // printf(" bastyp=%d %d\n",typ,*otyp);

  *oNr = 1;
  irc = 0;


  if(DB_QueryDef (typ, dbInd) < 0) return -1;

  switch (typ) {

    case Typ_PT:
      *oxo = (void*)DB_get_PT(dbInd);
      break;

    case Typ_VC:
      *oxo = (void*)DB_get_VC(dbInd);
      break;

    case Typ_LN:
      *oxo = (void*)DB_get_LN (dbInd);
      break;

    case Typ_CI:
      *oxo = (void*)DB_get_CI (dbInd);
      break;

    case Typ_PLN:
      *oxo = (void*)DB_get_PLN (dbInd);
      break;

    case Typ_CV:
      ox1 = DB_GetCurv (dbInd);
      *oNr = ox1->siz;
      *otyp  = ox1->form;  // 2014-06-04
      *oxo   = ox1->data;
/*
      if(ox1->typ == Typ_CVCCV) {   // CCV: die objTab
        // *otyp  = ox1->form;
        // *otyp  = typ;               // 2013-03-09 removed: is Typ_CV !
        *otyp  = ox1->typ;
        *oxo   = ox1;
        // irc = 1; // 2013-03-09 removed
      } else {                      // curve: data
        *otyp  = ox1->form;
        *oxo   = ox1->data;
      }
*/
        // printf("ex UTO_get_DB otyp=%d dbInd=%ld oNr=%d\n",*otyp,dbInd,*oNr);
      break;

    case Typ_SUR:
      // *otyp  = Typ_ObjGX;
      *otyp  = typ;
      *oxo   = (void*)DB_GetSur (dbInd, 0);
      irc = 1;
      break;

    case Typ_SOL:
      // *otyp  = Typ_ObjGX;
      *otyp  = typ;
      *oxo   = (void*)DB_GetSol (dbInd);
      irc = 1;
      break;

    case Typ_Note:
    // case Typ_GTXT:
    // case Typ_ATXT:
/*
      *otyp  = typ;
      *oxo = (void*)DB_GetGTxt (dbInd);
      irc = 1;
*/
      ox1 = (void*)DB_GetGTxt (dbInd);
      *otyp  = ox1->form;
      *oxo   = ox1->data;
      break;

    case Typ_Model:
    case Typ_Mock:
      *oxo = (void*)DB_get_ModRef (dbInd);
      break;

    case Typ_VAR:
      *oxo = (void*)DB_get_Var (dbInd);
      break;

    case Typ_Tra:
      *oxo = (void*)DB_GetTra (dbInd);
      irc = 1;
      break;

    case Typ_Activ:
      *oxo = (void*)DB_get_Activ (dbInd);
      break;


    default:
      TX_Error("UTO_get_DB: E001 %d",*otyp);
      return -1; // Par_err;
  }


  if(irc == 1) *otyp = AP_typ_2_bastyp (*otyp);

  return irc;

}


//=========================================================
  int UTO_sav_ost (int *dbTyp, long *dbInd,
                   int typ, int form, int siz, void *obj) {
//=========================================================
/// \code
/// UTO_sav_ost         create DB-object & save it in Model.
///   via AP_obj_2_txt ED_add_Line
/// 
/// Output:
///   dbTyp
///   dbInd 
/// RetCod:       0=OK, -1=Err
/// 
/// Example:
///  int          iTyp;
///  long         dbi;
///  Point        pa[]={{0., 0., 0.},{5., 0., 0.},{5., 2., 0.},{7., 0., 0.}};
///  UTO_sav_ost (&iTyp, &dbi, Typ_CVPOL, Typ_PT, 4, (void*)pa);
/// 
/// AP_stru_2_txt (NULL, 0, NULL, 0);    // init the objNr's before UTO_sav_ost
/// ACHTUNG: verwendet mem_cbuf1
/// \endcode

  int     irc, cbSiz;
  ObjGX   ox1;
  char    *cbuf;


  // printf("UTO_sav_ost typ=%d form=%d siz=%d\n",typ,form,siz);


  cbuf = mem_cbuf1;
  cbSiz = mem_cbuf1_SIZ;


  // create complex-obj
  OGX_SET_OBJ (&ox1, typ, form, siz, obj);


  // DB_dbo_get_free (Typ_CV, 0);


  // change Object -> Text
  irc = AP_obj_2_txt (cbuf, cbSiz, &ox1, -1L);
  if(irc < 0) return irc;
    // printf(" _sav_ost |%s|\n",cbuf);



  // query typ and index of created object
  AP_obj_2_txt_query (dbTyp, dbInd);

  // add & execute line
  // ED_add_Line (cbuf);


  ED_srcLn_add (cbuf, 0);              // 2010-11-29


  return 0;

}

/* UNUSED
//=======================================================================
  int UTO_get_PT (ObjX *oxo, ObjGX *oxi) {
//=======================================================================
/// DO NOT USE; use UTO_obj_getp

// get Pointer of PT
// receive oxo auch als **Point od **void


  // printf("UTO_get_PT %p typ=%d form=%d\n",oxi,oxi->typ,oxi->form);

    if(oxi->form == Typ_Index) {
      // printf(" PT ind=%ld\n",(long)oxi->data);
      oxo->pt = (Point*)DB_get_PT ((long)oxi->data);


    } else if(oxi->form == Typ_PT) {
      oxo->pt = (Point*)oxi->data;
      // printf("   _get_PT %p %f,%f,%f\n",oxi->data,
              // oxo->pt->x,oxo->pt->y,oxo->pt->z);

    } else {
      TX_Error("UTO_get_PT E001");
    }


  // printf("ex UTO_get_PT %f,%f,%f\n",oxo->pt->x,oxo->pt->y,oxo->pt->z);

  return 0;

}
*/
/* UNUSED
//=======================================================================
  int UTO_get_LN (ObjX *oxo, ObjGX *oxi) {
//=======================================================================
/// DO NOT USE; use UTO_obj_getp

// get Pointer of LN


  // printf("UTO_get_LN %d %d\n",oxi->typ,oxi->form);

    if(oxi->form == Typ_Index) {
      // printf(" get LN ind=%ld\n",(long)oxi->data);
      oxo->ln = (Line*)DB_get_LN ((long)oxi->data);


    } else if(oxi->form == Typ_LN) {
      oxo->ln = (Line*)oxi->data;
      // printf("UTO_get_PT %f,%f,%f\n",oxo->pt->x,oxo->pt->y,oxo->pt->z);

    } else {
      TX_Error("UTO_get_LN E001");
    }


  // printf("ex UTO_get_LN %f,%f,%f\n",oxo->ln->p1.x,oxo->ln->p1.y,oxo->ln->p1.z);
  // printf("              %f,%f,%f\n",oxo->ln->p2.x,oxo->ln->p2.y,oxo->ln->p2.z);

  return 0;

}
*/
/* UNUSED
//=======================================================================
  int UTO_get_CI (ObjX *oxo, ObjGX *oxi) {
//=======================================================================
/// DO NOT USE; use UTO_obj_getp

// get Pointer of CI
// receive oxo auch als **Circ od **void


  // printf("UTO_get_CI %d %d\n",oxi->typ,oxi->form);

    if(oxi->form == Typ_Index) {
      // printf(" PT ind=%ld\n",(long)oxi->data);
      oxo->ci = (Circ*)DB_get_CI ((long)oxi->data);


    } else if(oxi->form == Typ_CI) {
      oxo->ci = (Circ*)oxi->data;
      // printf("UTO_get_PT %f,%f,%f\n",oxo->pt->x,oxo->pt->y,oxo->pt->z);

    } else {
      TX_Error("UTO_get_CI E001");
    }


  // printf("ex UTO_get_CI %f,%f,%f\n",oxo->pt->x,oxo->pt->y,oxo->pt->z);

  return 0;

}
*/
/* UNUSED
//=======================================================================
  int UTO_get_CV (ObjX *oxo, ObjGX *oxi) {
//=======================================================================
/// DO NOT USE; use UTO_obj_getp

// get Pointer of CV; es muss ein B-Spline sein ...
// Problem: woher kennt man den CurveTyp ?
// RetCod: Curvetyp.


  int    i1;

  // printf("UTO_get_CV %d %d\n",oxi->typ,oxi->form);

    if(oxi->form == Typ_Index) {
      // printf(" Ind=%ld\n",(long)oxi->data);
      oxo->bsp = (CurvBSpl*)DB_get_CV (&i1, (long)oxi->data);


    } else if(oxi->form == Typ_CVBSP) {
      i1 = Typ_CVBSP;
      oxo->bsp = (CurvBSpl*)oxi->data;

    } else {
      TX_Error("UTO_get_CV E001");
    }


  // UT3D_stru_dump(Typ_CVBSP, oxo->bsp, "ex UTO_get_CV");

  return i1;

}
*/


//================================================================
  double UTO_par_tot (int oTyp, void *oDat) {
//================================================================
/// UTO_par_tot             get endParameter - unlimited

  int    i1;
  double d1 = 1.;



  if(oTyp == Typ_CVPOL) {
    // i1 = ((CurvPoly*)oDat)->ptNr - 1;
    // d1 = ((CurvPoly*)oDat)->lvTab[i1];
    d1 = plg_pare_unl ((CurvPoly*)oDat);


  } else if(oTyp == Typ_CVBSP) {
    i1 = ((CurvBSpl*)oDat)->ptNr + ((CurvBSpl*)oDat)->deg;
    d1 = ((CurvBSpl*)oDat)->kvTab[i1];

  }
    printf("ex UTO_par_tot %lf i1=%d\n",d1,i1);

  return d1;

}


//=====================================================================
  int UTO_parpt_pt_obj (double *po, Point *pti, int oTyp, void *oDat) {
//=====================================================================
/// \code
/// UTO_parpt_pt_obj        get parameter from point on curve
/// ACHTUNG: liefert (auch fuer B-Splines) immer Werte zwischen 0-1 !
/// \endcode

// was UT3D_parpt_objpt
// see also UT2D_parvc_2vc UT2D_par_3pt UT3D_parpt_objpt
// UT3D_angr_par1_ell UT3D_angr_elpt
// UT3D_parplg_plgpt UT3D_par_par1plg UT3D_par1_parplg
// UT3D_parCv_bsplpt UT3D_par1_parcbsp UT3D_par1_parBSp UT3D_parbsp_par1


  int    irc, ibp;
  double d1;
  // char   tmpSpc[50000];
  // Memspc wrkSpc;


  // printf("UTO_parpt_pt_obj oTyp=%d\n",oTyp);
  // UT3D_stru_dump (Typ_PT, pti, "  pti = ");
  // UT3D_stru_dump (oTyp, oDat, "  oDat:\n");


  if(oTyp == Typ_Val) {
    *po = *((double*)oDat);


  } else if(oTyp == Typ_LN) {
    ibp = UT3D_bp_2pt (&((Line*)oDat)->p1, &((Line*)oDat)->p2); // get backplane
    *po = UT3D_parpt_lnbp (pti, (Line*)oDat, ibp);


  } else if(oTyp == Typ_CI) {
    *po = UT3D_parpt_cipt (pti, (Circ*)oDat);


  } else if(oTyp == Typ_CVELL) {
    // *po = UT3D_angr_elpt (pti, &((CurvElli*)oDat)->pc,
                          // &((CurvElli*)oDat)->va, &((CurvElli*)oDat)->vb);
    // *po = UT3D_par1_angr_ell (po, oDat);
    UT3D_par1_el_pt (po, (CurvElli*)oDat, pti);


  } else if(oTyp == Typ_CVCLOT) {
    irc = UT3D_par_clotpt (po, pti, (CurvClot*)oDat, UT_DISP_cv);


  } else if(oTyp == Typ_CVBSP) {
    // UME_init (&wrkSpc, tmpSpc, 50000);
    irc = UT3D_parCv_bsplpt (po, &d1, (CurvBSpl*)oDat, pti);
    if(irc != 0) return -1;
    // Knotenwert in einen Paramterwert von 0 - 1 aendern
    *po = UT3D_par1_parbsp (po, oDat);


  } else if(oTyp == Typ_CVPOL) {
    irc = UT3D_parplg_plgpt (po, pti, (CurvPoly*)oDat);
    if(irc < 0) return -1;
    // Knotenwert in einen Parameterwert von 0-1 aendern
    *po = UT3D_par1_parplg (po, oDat);


  } else {
    TX_Error("UT3D_parpt_objpt E001 %d",oTyp);
    return -1;
  }


    // printf("ex UT3D_parpt_objpt %d %f\n",oTyp,*po);

  return 0;

}


//=======================================================================
  int UTO_obj_getp (void **objOut, int *oNr, ObjGX *ox1) {
//=======================================================================
/// \code
/// UTO_obj_getp                   get dataStruct from ObjGX
///   Returns a pointer to the data-struct from ObjGX
///   Referenced objects (Typ_Index) are resolved.
///   
/// Input:
///   ox1      obj as ObjGX
/// Output:
///   objOut   datastruct, typ = RetCode
///   RetCod = typ of datastruct (form). CCV: returns Typ_ObjGX.
///       -1 = Error
/// 
/// CCV:     rc=Typ_ObjGX
/// Polygon: rc=Typ_CVPOL;  objOut=CurvPoly
/// see UTO_get_DB    get dataStruct from DB-Obj Keep Refs.   Give pointer.
/// see DB_GetObjGX DB UTO_obj_get UTO_objx_get
/// \endcode

// es sollten UTO_objx_get und UTO_obj_get durch UTO_obj_getp ersetzt werden !!!

// liefert immer nur typ(rc) und Pointer auf die Datenstruct.
// Bei CCV ist die siz weg !! (liefert das erste Obj; ist also ungeeignet
//   zum decode Typ_Index bei CCV !  (use DB_GetObjGX)
// Gleiches Problem mit RCIR: form=index, data = 3 pointers (3longs)

static ObjGX  *odb;
  int    irc, typ;
  void   *vp1;



  // printf("UTO_obj_getp typ=%d form=%d\n",ox1->typ,ox1->form);
  // if(ox1->typ == 0) AP_debug__ ("UTO_obj_getp E-0");
  // UT3D_stru_dump(Typ_ObjGX, ox1, "-----:");


  typ  = ox1->typ;
  *oNr = ox1->siz;


  //------------------------------------------
  if(typ == Typ_PT) {

    if(ox1->form == Typ_Index) {
      *objOut = (void*)DB_get_PT ((long)ox1->data);

    } else if(ox1->form == Typ_PT) {
      *objOut = ox1->data;

    } else {
      TX_Error("UTO_obj_getp E00PT1");
      return -1;

    }



  //------------------------------------------
  // get Float8-values ..
  } else if((typ == Typ_VAR)   ||
            (typ == Typ_Val)   ||
            (typ == Typ_Par1))       {

    if(ox1->form == Typ_Index) {
      *objOut = (void*)DB_get_Var ((long)ox1->data);

    } else if((ox1->form == Typ_Float8)   ||     // so sollten alle sein ...
              (ox1->form == Typ_Val)      ||
              (ox1->form == Typ_Par1)     ||
              (ox1->form == Typ_VAR))         {
      *objOut = ox1->data;

    } else {
      TX_Error("UTO_obj_getp E00VC1");
      return -1;

    }



  //------------------------------------------
  } else if(typ == Typ_VC) {

    if(ox1->form == Typ_Index) {
      *objOut = (void*)DB_get_VC ((long)ox1->data);

    } else if(ox1->form == Typ_VC) {
      *objOut = ox1->data;

    } else {
      TX_Error("UTO_obj_getp E00VC1");
      return -1;

    }


  //------------------------------------------
  } else if(typ == Typ_LN) {

    if(ox1->form == Typ_Index) {
      *objOut = (void*)DB_get_LN ((long)ox1->data);

    } else if(ox1->form == Typ_LN) {
      *objOut = ox1->data;

    } else {
      TX_Error("UTO_obj_getp E00LN1");
      return -1;

    }


  //------------------------------------------
  } else if(typ == Typ_CI) {

    if(ox1->form == Typ_Index) {
      *objOut = (void*)DB_get_CI ((long)ox1->data);

    } else if(ox1->form == Typ_CI) {
      *objOut = ox1->data;

    } else {
      TX_Error("UTO_obj_getp E00AC1");
      return -1;

    }



  //------------------------------------------
  } else if((typ == Typ_CV)        ||
            (typ == Typ_CVLNA)     ||
            (typ == Typ_CVPOL)     ||
            (typ == Typ_CVELL)     ||
            (typ == Typ_CVCCV)     ||
            (typ == Typ_CVCLOT)    ||
            (typ == Typ_CVBSP)     ||
            (typ == Typ_CVRBSP))      {
      // printf(" curve - typ=%d form=%d\n",typ,ox1->form);

    if(ox1->form == Typ_Index) {
      odb = DB_GetCurv ((long)ox1->data);

      // if(odb->typ == Typ_CVCCV)  {  // raus 2014-07-16
        // typ     = Typ_ObjGX;
        // *objOut = odb;
        // goto L_done;
      // }
         
      typ     = odb->form;
      *objOut = odb->data;
        // *objOut = (void*) DB_get_CV (&typ, (long)ox1->data);
        // printf("  _get_CV %d %ld\n",typ,(long)ox1->data);
        // UT3D_stru_dump (typ, *objOut, "  CV-Index:");




    } else if((ox1->form == Typ_CVLNA)    ||
              (ox1->form == Typ_CVCCV)    ||    // 2014-07-16
              (ox1->form == Typ_CVPOL)    ||
              (ox1->form == Typ_CVELL)    ||
              (ox1->form == Typ_CVCLOT)   ||
              (ox1->form == Typ_CVBSP)    ||
              (ox1->form == Typ_CVRBSP))     {
      *objOut = ox1->data;
      typ = ox1->form;

    } else if(typ == Typ_CVCCV)        {   // 2012-01-11 was ox1->form
      *objOut = ox1;
      typ = Typ_ObjGX;

    } else {
      TX_Error("UTO_obj_getp E00CV1");
      UT3D_stru_dump(Typ_ObjGX, ox1, "-----:");
      return -1;
    }



  //------------------------------------------
  } else if(typ == Typ_PLN) {

    if(ox1->form == Typ_Index) {
      DB_GetRef ((Plane*)&objOut, (long)ox1->data);

    } else if(ox1->form == Typ_PLN) {
      *objOut = ox1->data;

    } else {
      TX_Error("UTO_obj_getp E00PLN1");
      return -1;
    }



  //------------------------------------------
  } else if((typ == Typ_SUR)      ||
            (typ == Typ_SURSWP)   ||
            (typ == Typ_SURRV))       {

    if(ox1->form == Typ_Index) {
      *objOut = DB_GetSur ((long)ox1->data, 1);
      typ = Typ_ObjGX;  // die surf ..
      // typ = ((ObjGX*)*objOut)->form; - das ist der typ der objOut.data !

    } else if(ox1->form == Typ_SURSWP) {   // 2014-02-16
      *objOut = ox1->data;

    } else if(ox1->form == Typ_ObjGX) {
      *objOut = ox1;
      typ = Typ_ObjGX;  // die surf ..

    } else {
      TX_Error("UTO_obj_getp E002_%d_%d",ox1->typ,ox1->form);
    }



  //------------------------------------------
  } else if(ox1->typ == Typ_SOL) {                   // 2014-02-08

    if(ox1->form == Typ_Index) {
      *objOut = DB_GetSol ((long)ox1->data);
      typ = Typ_ObjGX;  // die surf ..

    } else if((ox1->form == Typ_SPH) ||
              (ox1->form == Typ_CON) ||
              (ox1->form == Typ_TOR))    {
      *objOut = ox1;


    } else {
      TX_Error("UTO_obj_getp E003_%d_%d",ox1->typ,ox1->form);
    }



  //------------------------------------------
  } else if(typ == Typ_Model) {

    if(ox1->form == Typ_Index) {
      *objOut = DB_get_ModRef ((long)ox1->data);
      if(DB_isFree_ModRef((ModelRef*)*objOut)) return -1;
  
    } else if(ox1->form == Typ_Model) {
      *objOut = ox1->data;

    } else {
      TX_Error("UTO_obj_getp E001 Typ_Model %d",ox1->form);
      return -1;

    }



  //------------------------------------------
  } else if(typ == Typ_Note) {

    if(ox1->form == Typ_Index) {
      odb = DB_GetGTxt ((long)ox1->data);
      typ = odb->form;
      *objOut = odb->data;

    } else if((ox1->form == Typ_ATXT)  ||
              (ox1->form == Typ_GTXT)  ||
              (ox1->form == Typ_Dimen) ||
              (ox1->form == Typ_Dim3)  ||
              (ox1->form == Typ_Tag))     {
      typ = ox1->form;
      *objOut = ox1->data;

    } else goto L_err_not_implem;


  //------------------------------------------
  } else goto L_err_not_implem;


  L_done:
    // printf(" ex UTO_obj_getp typ=%d\n",typ);

  return typ;


  L_err_not_implem:
    return MSG_STD_ERR (func_not_impl, "/ typ %d form %d",ox1->typ,ox1->form);

}

/* UNUSED
//=======================================================================
  int UTO_objx_get (ObjX *oxo, ObjGX *oxi) {
//=======================================================================
/// DO NOT USE; use UTO_obj_getp

// SOLL DURCH UTO_obj_getp ERSETZT WERDEN ...
// RetCod = form !
// 
// see UTO_get_DB UTO_obj_getp UTO_obj_get


  int  irc;
  void *vp1;


  // printf("UTO_objx_get typ=%d form=%d\n",oxi->typ,oxi->form);

  irc = oxi->typ;



  //------------------------------------------
  if(oxi->typ == Typ_PT) {

    if(oxi->form == Typ_Index) {
      oxo->pt = DB_get_PT ((long)oxi->data);

    } else if(oxi->form == Typ_PT) {
      oxo->pt = (Point*)oxi->data;

    } else {
      TX_Error("UTO_objx_get E00PT1");
      return -1;
    }


  //------------------------------------------
  } else if(oxi->typ == Typ_LN) {

    if(oxi->form == Typ_Index) {
      oxo->ln = DB_get_LN ((long)oxi->data);

    } else if(oxi->form == Typ_LN) {
      oxo->ln = (Line*)oxi->data;

    } else {
      TX_Error("UTO_objx_get E00LN1");
      return -1;
    }


  //------------------------------------------
  } else if(oxi->typ == Typ_VC) {

    if(oxi->form == Typ_Index) {
      oxo->vc = DB_get_VC ((long)oxi->data);

    } else if(oxi->form == Typ_VC) {
      oxo->vc = (Vector*)oxi->data;

    } else {
      TX_Error("UTO_objx_get E00VC1");
      return -1;
    }



  //------------------------------------------
  } else if(oxi->typ == Typ_CI) {

    if(oxi->form == Typ_Index) {
      oxo->ci = DB_get_CI ((long)oxi->data);

    } else if(oxi->form == Typ_CI) {
      oxo->ci = (Circ*)oxi->data;

    } else {
      TX_Error("UTO_objx_get E00AC1");
      return -1;
    }



  //------------------------------------------
  } else if(oxi->typ == Typ_CV) {

    if(oxi->form == Typ_Index) {
      oxo->vp = (void*) DB_get_CV (&irc, (long)oxi->data);

    } else if(oxi->form == Typ_CV) {
      oxo->vp = oxi->data;
      irc = oxi->form;

    } else {
      TX_Error("UTO_objx_get E00CV1");
      return -1;
    }
    // UT3D_stru_dump(irc,oxo->vp,"-----:");




  //------------------------------------------
  } else if(oxi->typ == Typ_CVBSP) {

    if(oxi->form == Typ_Index) {
      oxo->vp = (void*) DB_get_CV (&irc, (long)oxi->data);

    } else if(oxi->form == Typ_CVBSP) {
      oxo->vp = oxi->data;
      irc = oxi->form;

    } else {
      TX_Error("UTO_objx_get E00CVBSP1");
      return -1;
    }
    // UT3D_stru_dump(Typ_CVBSP,oxo->vp,"CVBSP:");
    


  //------------------------------------------
  } else if(oxi->typ == Typ_PLN) {

    if(oxi->form == Typ_Index) {
      oxo->vp = (void*) DB_get_PLN ((long)oxi->data);

    } else if(oxi->form == Typ_PLN) {
      oxo->vp = oxi->data;
      irc = oxi->form;

    } else {
      TX_Error("UTO_objx_get E00PLN1");
      return -1;
    }
    // UT3D_stru_dump(Typ_PLN, oxo->vp, "UTO_objx_get RefSys:");



  //------------------------------------------
  } else if(oxi->typ == Typ_SURRV) {

    if(oxi->form == Typ_Index) {
      vp1 = (void*) DB_GetSur ((long)oxi->data, 0);
      oxo->vp = ((ObjGX*)vp1)->data;

    } else if(oxi->form == Typ_SURRV) {
      oxo->vp = oxi->data;
      irc = oxi->form;

    } else {
      TX_Error("UTO_objx_get E00CVBSRV");
      return -1;
    }
    // UT3D_stru_dump(Typ_CVBSP,oxo->vp,"CVBSP:");





  //------------------------------------------
  } else {
    TX_Error("UTO_objx_get E001 %d",oxi->typ);
    return -1;
  }



  return irc;

}
*/


//================================================================
  int UTO_ind_get (int *typ, long *ind, ObjGX *oxp) {
//================================================================
 
  // printf("UTO_ind_get %d %d\n",oxp->typ,oxp->form);

  if(oxp->form == Typ_Index) {
    *typ = oxp->typ;
    // *ind = (long)oxp->data;
    *ind = INT_PTR(oxp->data);

  } else {
    TX_Error("UTO_ind_get %d %d",oxp->typ,oxp->form);
    return -1;
  }

  return 0;

}


/*
//=======================================================================
  int UTO_obj_get (void *objOut, ObjGX *ox1) {
//=======================================================================
/// \code
/// UTO_obj_get                    get dataStruct from ObjGX
///   Returns a copy of the dataStruct, not a pointer.
///   Referenced objects (Typ_Index) are resolved.
/// cplxObj -> binObj (copy)
/// \endcode

// EV NEU MACHEN: use UTO_obj_getp, dann copy resulting object !!!!!

// entspr ox1.typ (Typ_PT/Typ_LN/Typ_CI) struct liefern
// liefert Daten - nicht nur Pointer !!!
// sollte man maxSiz/outSiz als Parameter zufuegen ?
// od size vorher abfragen ?
// typ=Typ_SUR (150) liefert einen ObjGX-Pointer, nicht daten !!!
// ty0=Typ_CV  liefert Pointer !

// see UTO_get_DB UTO_obj_getp

  int     bTyp;
  ObjGX   *oxp1;


  // printf("UTO_obj_get typ=%d form=%d siz=%d\n",ox1->typ,ox1->form,ox1->siz);
  // UT3D_stru_dump (Typ_ObjGX, ox1, "UTO_obj_get:\n");


  bTyp = AP_typ_2_bastyp (ox1->typ);
    // printf(" bTyp=%d\n",bTyp);


  //------------------------------------------
  if(ox1->typ == Typ_PT) {

    if(ox1->form == Typ_Index) {
      // objOut->pt = DB_GetPoint ((long)ox1->data);
      *(Point*)objOut = DB_GetPoint ((long)ox1->data);

    } else if(ox1->form == Typ_PT) {
      // objOut->pt = *((Point*)ox1->data);
      *(Point*)objOut = *((Point*)ox1->data);
    }

  //------------------------------------------
  } else if(ox1->typ == Typ_VC) {

    if(ox1->form == Typ_Index) {
      // objOut->vc = DB_GetVector ((long)ox1->data);
      *(Vector*)objOut = DB_GetVector ((long)ox1->data);

    } else if(ox1->form == Typ_VC) {
      // objOut->vc = *((Vector*)ox1->data);
      *(Vector*)objOut = *((Vector*)ox1->data);
    }


  //------------------------------------------
  } else if(ox1->typ == Typ_LN) {

    if(ox1->form == Typ_Index) {
      // objOut->ln = DB_GetLine ((long)ox1->data);
      *(Line*)objOut = DB_GetLine ((long)ox1->data);

    } else if(ox1->form == Typ_LN) {
      // objOut->ln = *((Line*)ox1->data);
      *(Line*)objOut = *((Line*)ox1->data);
    }


  //------------------------------------------
  } else if(ox1->typ == Typ_CI) {

    if(ox1->form == Typ_Index) {
      // objOut->ci = DB_GetCirc ((long)ox1->data);
      *(Circ*)objOut = DB_GetCirc ((long)ox1->data);

    } else if(ox1->form == Typ_CI) {
      // objOut->ci = *((Circ*)ox1->data);
      *(Circ*)objOut = *((Circ*)ox1->data);
    }


  //------------------------------------------
  } else if(bTyp == Typ_CV) {
    // liefert hier nur den Pointer !

    if(ox1->form == Typ_Index) {
      // printf("  UTO_obj_get CV %d\n",(long)ox1->data);
      *(ObjGX**)objOut = DB_GetCurv ((long)ox1->data);
        // UTO_dump__ (objOut, "CV %d ",(long)ox1->data);  GEHT SO NED !

    } else if(AP_typ_2_bastyp(ox1->form) == Typ_CV) {
      *(ObjGX**)objOut = ox1;

    } else {
      TX_Error("UTO_obj_get E004_%d_%d",ox1->typ,ox1->form);
    }


  //------------------------------------------
  } else if(bTyp == Typ_SUR) {
    // liefert hier nur den Pointer !

    if(ox1->form == Typ_Index) {
      *(ObjGX**)objOut = DB_GetSur ((long)ox1->data, 1);

    // } else if(ox1->form == Typ_ObjGX) {
      // *(ObjGX**)objOut = ox1;

    } else {
      *(ObjGX**)objOut = ox1;
      // TX_Error("UTO_obj_get E002_%d_%d",ox1->typ,ox1->form);
    }



  //------------------------------------------
  } else if(ox1->typ == Typ_SOL) {

    if(ox1->form == Typ_Index) {
      *(ObjGX**)objOut = DB_GetSol ((long)ox1->data);

    } else if((ox1->form == Typ_SPH) ||
              (ox1->form == Typ_CON) ||
              (ox1->form == Typ_TOR))    {
      *(ObjGX**)objOut = ox1;

    } else {
      TX_Error("UTO_obj_get E003_%d_%d",ox1->typ,ox1->form);
    }




  //------------------------------------------
  } else {
    TX_Error("UTO_obj_get E001 %d",ox1->typ);
  }



  return 0;

}
*/

//================================================================
  int UTO_stru_inv (int oTyp, void *oDat) {
//================================================================
/// \code
/// UTO_stru_inv            invert object (Typ, struct)
/// 
/// ACHTUNG: es wird nur die Drehrichtung umgedreht; die Geometrie bleibt
///   optisch ganz gleich !!
/// 
/// see also UT3D_vc_invert UT3D_pl_invert UT3D_cv_inv UT3D_m3_invm3 
/// \endcode


  // printf("UTO_stru_inv %d\n",oTyp);
  // UT3D_stru_dump (oTyp, oDat, "UTO_stru_inv in:\n");



  if(oTyp == Typ_LN) {
    UT3D_ln_inv ((Line*)oDat);


  } else if(oTyp == Typ_CI) {
    UT3D_ci_inv1 ((Circ*)oDat);


  } else if(oTyp == Typ_CVELL) {
    UT3D_el_inv1 ((CurvElli*)oDat);


  } else if(oTyp == Typ_CVPOL) {        // CurvPoly
    MEM_swap_2db(&((CurvPoly*)oDat)->v0, &((CurvPoly*)oDat)->v1);


  } else if(oTyp == Typ_CVBSP) {
    MEM_swap_2db(&((CurvBSpl*)oDat)->v0, &((CurvBSpl*)oDat)->v1);


  } else if(oTyp == Typ_CVRBSP) {
    MEM_swap_2db(&((CurvRBSpl*)oDat)->v0, &((CurvRBSpl*)oDat)->v1);


  } else if(oTyp == Typ_SURRV) {
    // printf(" _stru_inv %f %f\n",((SurRev*)oDat)->v0,((SurRev*)oDat)->v1);
    MEM_swap_2db(&((SurRev*)oDat)->v0, &((SurRev*)oDat)->v1);
    ((SurRev*)oDat)->v0 = 1. - ((SurRev*)oDat)->v0;
    ((SurRev*)oDat)->v1 = 1. - ((SurRev*)oDat)->v1;
    // printf("ex _stru_inv %f %f\n",((SurRev*)oDat)->v0,((SurRev*)oDat)->v1);


  } else {
    TX_Error("UTO_stru_inv E001 %d",oTyp);
    return -1;
  }


  // UT3D_stru_dump (oTyp, oDat, "ex UTO_stru_inv:\n");

  return 0;

}


/*
//================================================================
  int UTO_obj_int (Point *pa, int *ptNr, double *va,
                   ObjGX *oi, ObjGX *oc1, int i1Nr, Memspc *wrkSpc) {
//================================================================
/// \code
/// intersect obj with 1 cutting-object;                    UNUSED
/// cutting-objects Parameter: Typ=Typ_Val nur von 0-1 (Achtung BSP's!)
/// 
/// Input:
///   ptNr              size of pa
///   oi                obj to cut
///   oc1               cuttingObj
///   i1Nr              ob eine bestimmte IntersectionNr definiert ist;
///                     -1=alle; 0-n=index of intersectPoint
/// \endcode


  int    irc, i1, o0Typ, o1Typ, pSiz;
  void   *obj0, *obj1, *wrkPos;
  double *dTab;
  Point  *pTab;


  // printf("UTO_obj_int %d\n",i1Nr);


  // save wrkSpc-pointer
  wrkPos = wrkSpc->next;


  // reserve space for 20 points & doubles in wrkSpc
  pSiz = 20;
  pTab = wrkSpc->next;
  if(UME_add (wrkSpc, pSiz * sizeof(Point)) < 0) return -1;
  dTab = wrkSpc->next;
  if(UME_add (wrkSpc, pSiz * sizeof(double)) < 0) return -1;


  // get obj to cut
  o0Typ = UTO_obj_getp (&obj0, oi);
  if(o0Typ < 0) return -1;


  // get cutting obj
  o1Typ = UTO_obj_getp (&obj1, oc1);
  if(o1Typ < 0) return -1;


  // intersect
  irc = UTO_stru_int (&pSiz, pTab, va, o0Typ, obj0, o1Typ, obj1, wrkSpc);
  if(irc < 0) return -1;


  // copy requested points from pTab -> pa
  if(i1Nr < 0) {
    if(*ptNr < pSiz) {TX_Error("UTO_obj_int: pSiz too small");return -1;}
    *ptNr = pSiz;
    for(i1=0;i1<pSiz;++i1) pa[i1] = pTab[i1];
    for(i1=0;i1<pSiz;++i1) va[i1] = dTab[i1];

  } else {                  // copy only the requested point ...
    *ptNr = 1;
    pa[0] = pTab[i1Nr];
    va[0] = dTab[i1Nr];
  }


  // reset wrkSpc-pointer
  wrkSpc->next = wrkPos;


  // TestDisplay:
  for(i1=0;i1<*ptNr;++i1) GR_Disp_pt(&pa[i1], SYM_STAR_S, 2);


  return 0;

}
*/


//================================================================
  int UTO_npt_int_2ox (int *pNr, Point *pa, double *va1, int aSiz,
                       ObjGX *oxi1, ObjGX *oxi2, Memspc *wrkSpc) {
//================================================================
/// \code
/// intersect 2 limited objects (ox1 X ox2) where result = points
///   handles CURV X SURF and CURV X CURV, but not SURF X SURF.
/// Input:
///   aSiz     size of pa, va
/// Output:
///   pNr   number of points in pa
///   pa    intersectionPoints (if pa != NULL)
///         Typ_CVPOL X Typ_Val:  returns only va1, not pa.
///   va1   corresponding parameters on o1 (if va1 != NULL)
/// RetCod:
///    0    OK, 0-<pNr> intersectionpoints in pa.
///   -1    Error
///
/// VAR PT LN CI VC CV CVPOL CVPSP3 CVBSP CVRBSP CVCCV CVELL CVBEZ CVRBEZ 
///   CVCLOT CVLNA 
///   PLN SUR ... SOL ...
///   Angle Dist Par1 UVal VVal 
///
/// TODO: if va1 or pt not given: get tempSpace !
///
/// \endcode

// see UTO_stru_int



  int        irc, i1, i2, ip, io, oNr, o1Typ, o2Typ, o1Form, o2Form, nip,
             swapped=0;
  long       dbi;
  double     d1, *vp1, us, ue, *va2 = NULL, *va1o;
  char       obj1[OBJ_SIZ_MAX];
  ObjGX      *ox1, *ox2, *oxTab, oxDbo1, oxDbo2, oxCCV;
  Point      pt1, ps, pe;
  CurvCCV    *ccvTab;
  void       *oo1, *o1, *o2;


  ox1 = oxi1;
  ox2 = oxi2;

  *pNr = 0;



  // printf("iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii\n");
  // printf("UTO_npt_int_2ox pNr=%d aSiz=%d\n",*pNr,aSiz);
    // printf(" ox1 %d %d\n",ox1->typ,ox1->form);
    // printf(" ox2 %d %d\n",ox2->typ,ox2->form);
  // UTO_dump__(ox1, " ox1");
  // UTO_dump__(ox2, " ox2");




  //----------------------------------------------------------------
  // resolv dbo = ObjGX with index
  if(ox1->form == Typ_Index) {
    OGX_GET_INDEX (&o1Form, &dbi, ox1);
    oxDbo1.form = DB_GetObjDat (&oxDbo1.data, &oNr, o1Form, dbi);
    oxDbo1.typ = oxDbo1.form;
    oxDbo1.siz = oNr;
    ox1 = &oxDbo1;
  }


  if(ox2->form == Typ_Index) {
    OGX_GET_INDEX (&o2Form, &dbi, ox2);
    oxDbo2.form = DB_GetObjDat (&oxDbo2.data, &oNr, o2Form, dbi);
    oxDbo2.typ = oxDbo2.form;
    oxDbo2.siz = oNr;
    ox2 = &oxDbo2;
  }



  //----------------------------------------------------------------
  // resolv CCV's
  if(ox1->typ == Typ_CVCCV) {
    // ox1 is a CCV; CurvCCV
    ccvTab = ox1->data;
    oxCCV.siz = 1;
    oNr = ox1->siz;
    for(i1=0; i1<oNr; ++i1) {
      // change trimmedCurve into standardCurve
      UTO_cv_cvtrm (&oxCCV.form, obj1, NULL, &ccvTab[i1]);
      oxCCV.typ = oxCCV.form;
      oxCCV.data = obj1;
        // printf(" ----- nxt CCV-o1-seg %d\n",i1);
        // UT3D_stru_dump (oxCCV.form, obj1, " ccv-o1");
        // UTO_dump__ (&oxCCV, " oxCCV-o1");
      // work (RECURSION!)
      irc = UTO_npt_int_2ox (&i2, &pa[*pNr], &va1[*pNr], aSiz - *pNr,
                             &oxCCV, ox2, wrkSpc);
        // printf(" ex int_2ox-o1 irc=%d i2=%d\n",irc,i2);
      // ignore irc - check all segs
      if(irc >= 0) *pNr += i2;
    }
    return 0;
  }

  if(ox2->typ == Typ_CVCCV) {
    // ox2 is a CCV; CurvCCV
    ccvTab = ox2->data;
    oxCCV.siz = 1;
    oNr = ox2->siz;
    for(i1=0; i1<oNr; ++i1) {
      // change trimmedCurve into standardCurve
      UTO_cv_cvtrm (&oxCCV.form, obj1, NULL, &ccvTab[i1]);
      oxCCV.typ = oxCCV.form;
      oxCCV.data = obj1;
        // printf(" ----- nxt CCV-o2-seg %d\n",i1);
        // UT3D_stru_dump (oxCCV.form, obj1, " ccv-o2");
        // UTO_dump__ (&oxCCV, " oxCCV-02");
      // work (RECURSION!)
      irc = UTO_npt_int_2ox (&i2, &pa[*pNr], &va1[*pNr], aSiz - *pNr,
                             &oxCCV, ox1, wrkSpc);
        // printf(" ex int_2ox-o2 irc=%d i2=%d\n",irc,i2);
      // ignore irc - check all segs
      if(irc >= 0) *pNr += i2;
    }
    return 0;
  }




  //----------------------------------------------------------------
  // make typ2 > typ1. if necessary: swap obj's;
  // Problem: va1 is for parameters on obj1; swapped: need parameters of obj2 ..
  // L_swap2:
  if(va1) va1[0] = UT_VAL_MAX; // check later if already computed ..


  if(ox1->form > ox2->form) {
    MEM_swap_2vp (&ox1, &ox2);
    // va1 = parameters on obj ox1; va2 = parameters on obj ox2
    // get tempspace for va2
    va2 = MEM_alloc_tmp (sizeof(double) * aSiz);
    va2[0] = UT_VAL_MAX; // check later if already computed ..
    // swap also output array for parameters
    va1o = va1;
    MEM_swap_2vp (&va1, &va2);
    swapped = 1;
      // printf(" swapped...\n");
  }

  // now is o1Form < o2Form
  o1Typ  = ox1->typ;
  o2Typ  = ox2->typ;
  o1Form = ox1->form;
  o2Form = ox2->form;
  o1     = ox1->data;
  o2     = ox2->data; 

  nip   = 0; // nr of resulting points



  //----------------------------------------------------------------
  L_Start:
    // printf(" _int_2ox o1Typ=%d o2Typ=%d\n",o1Typ,o2Typ);
    // UT3D_stru_dump (o1Form, o1, " o1");
    // UT3D_stru_dump (o2Form, o2, " o2");




  switch (o1Form) {       // o1 = obj to cut

    // o1Form always less or equal o2Form !


    //================================================================
    case Typ_PT:   // o1=PT 2

      switch (o2Form) {

        case Typ_LN:  // PT -> LN
          UT3D_pt_projptln (pa, &d1, (Point*)o1, (Line*)o2);
          nip = 1;
          UT3D_pt_ln_lim_del (&nip, pa, va2, &((Line*)o1)->p1, &((Line*)o1)->p2);
          break;

        case Typ_CI:  // PT -> CI
          irc = UT3D_pt_projptci (&pa[0], &pa[1], (Point*)o1, (Circ*)o2);
          if(irc < 0) { nip = 0; goto L_exit; }
          nip = 1;  // CUT P C cannot use oppos. point.
          UT3D_pt_ci_lim_del (&nip, pa, va2, (Circ*)o2);
          break;

        case Typ_CVPOL:  // PT -> CVPOL
          nip = aSiz;
          UT3D_pt_projptplg (&nip, pa, va2, (CurvPoly*)o2, (Point*)o1);
          if(nip > 0) {
            // get limited range from (us) - to (ue)
            us = ((CurvPoly*)o2)->v0;
            ue = ((CurvPoly*)o2)->v1;
              // printf(" us=%lf ue=%lf\n",us,ue);
            // get limiting points startPoint (ps) and endPoint (pe)
            UT3D_pt_plg_lim (&ps, &pe, NULL, NULL, (CurvPoly*)o2);
              // UT3D_stru_dump (Typ_PT, &ps, " ps");
              // UT3D_stru_dump (Typ_PT, &pe, " pe");
            UT3D_par_ck_inObj_del (&nip, pa, va2, &ps, &pe, us, ue);
          }
          break;

        case Typ_CVBSP:  // PT -> CVBSP
          nip = aSiz;
          irc = UT3D_pt_projptbspl (&nip,pa,va2,(CurvBSpl*)o2,(Point*)o1);
          if(irc < 0) goto ErrMem;
          if(nip > 0) {
            // skip points if not inside trimming-range
            // get limited range from (us) - to (ue)
            us = ((CurvBSpl*)o2)->v0;
            ue = ((CurvBSpl*)o2)->v1;
              // printf(" us=%lf ue=%lf\n",us,ue);
            // get ps=startpoint, pe=endpoint of curve
            UT3D_pt_cbsp_lim (&ps, &pe, NULL, NULL, (CurvBSpl*)o2);
            // delete all points & parameters not on obj
            UT3D_par_ck_inObj_del (&nip, pa, va2, &ps, &pe, us, ue);
          }
          break;

        case Typ_CVRBSP:   // PT -> CVRBSP
          irc = UT3D_pt_projptrbspl (&nip,pa,va2,wrkSpc, UT_TOL_cv,  // UT_TOL_pt,
                                     (CurvRBSpl*)o2,(Point*)o1);
          if(irc > 0) nip = 1; /// ???
          if(irc < 0){TX_Error("UTO_stru_int E004"); return irc;}
          break;

        case Typ_CVELL:  // PT -> CVELL
          UT3D_pt_projptel (&nip, pa, (CurvElli*)o2, (Point*)o1);
          if(nip < 1) goto L_no_sol;
          // test if points are on Elli o2
          UT3D_pt_ell_lim_del (&nip, pa, va2, (CurvElli*)o2);
          break;

        default:
          goto ParErr;
      }
      break;




    //================================================================
    case Typ_LN:   // o1=LN 3

      switch (o2Form) {

        case Typ_LN: // LN x LN
            // printf(" LN x LN pNr=%d\n",*pNr);
          // UT3D_pt_int2ln (&pa[0],&pa[1], NULL, (Line*)o1, (Line*)o2);
          // intersect limited lines
          // nip = UT3D_pt_intlnln (pa, &pt1, &d1, (Line*)o1, 0, (Line*)o2, 0);
          irc = UT3D_pt_int2pt2pt_lim (pa, va1, NULL,
                                 &((Line*)o1)->p1, &((Line*)o1)->p2,
                                 &((Line*)o2)->p1, &((Line*)o2)->p2, UT_TOL_cv);
          if(irc < 0) goto NoInt;
          nip = 1;
          break;

        case Typ_CI:  // LN x CI
            // printf(" npt_int_2ox LN x CI\n");
          // UT3D_pt_intlnci__ (&nip, pa, (Line*)o1, (Circ*)o2);
          nip = UT3D_pt_intlnci_l (pa, (Line*)o1, 0, (Circ*)o2, 0);
            // printf(" nip=%d\n",nip);
// TODO: get parameters on o1=LN
          break;

        case Typ_CVELL:  // LN x CVELL
          UT3D_pt_intlnel__ (&nip, pa, (Line*)o1, (CurvElli*)o2);
          if(nip < 1) goto L_no_sol;
          // test if points are on Elli o2
          UT3D_pt_ell_lim_del (&nip, pa, va1, (CurvElli*)o2);
          // test if points are on Line o1
          UT3D_pt_ln_lim_del (&nip, pa, va1, &((Line*)o1)->p1, &((Line*)o1)->p2);
          break;

        case Typ_CVPOL:  // LN x CVPOL
          nip = aSiz;
          UT3D_pt_intlnplg (&nip, pa, va1, (Line*)o1, (CurvPoly*)o2, UT_TOL_cv);
          break;

        case Typ_CVBSP:  // LN x CVBSP
          nip = aSiz;
          UT3D_pt_intlnbspl (&nip, pa, wrkSpc, (Line*)o1, (CurvBSpl*)o2);
          break;

        case Typ_CVRBSP:  // LN x CVRBSP
          nip = aSiz;
          irc = UT3D_pt_int_ln_rbsp (&nip, pa, wrkSpc, (Line*)o1, (CurvRBSpl*)o2);
          break;

        case Typ_CVCLOT:  // LN x Clot
          // make pt, vc from line ...
          UT3D_vc_2pt ((Vector*)obj1, &((Line*)o1)->p1, &((Line*)o1)->p2);
          UT3D_pt_intclotptvc (&nip, pa, va1,
                               &((Line*)o1)->p1, (Vector*)obj1,
                               (CurvClot*)o2, UT_DISP_cv);
          break;

        case Typ_PLN:  // LN x R
          irc = UT3D_pt_intlnpl (pa, (Plane*)o2, (Line*)o1);
          if(irc < 0) return -1;
          nip = 1;
          break;

        case Typ_Val:     // LN X Parameter 130
        case Typ_Par1:    // 140
        case Typ_Float8:  // 175
// TODO: o2Form=Typ_Float8; o2Typ=Typ_Dist
          if(o2Typ == Typ_Dist) {
            UT3D_pt_traptptlen (pa, &((Line*)o1)->p1, &((Line*)o1)->p2,
                                *((double*)o2));
          } else{
            UT3D_pt_evparln (pa, *((double*)o2), (Line*)o1);
            if(va1) va1[0] = *((double*)o2);
          }
          nip = 1;
          break;

        default:
          goto ParErr;
      }
      break;


    //================================================================
    case Typ_CI:    // o1=CI 4

      switch (o2Form) {

        // case Typ_LN: // CI x LN
          // UT3D_pt_intlnci__ (&nip, pa, (Line*)o2, (Circ*)o1);
            // printf(" nip=%d\n",nip);
          // goto L_v_Circ;   // get parameters on o1=CI

        case Typ_CI:  // CI x CI
          nip = UT3D_pt_intcici (pa, (Circ*)o1, 0, (Circ*)o2, 0);
            // printf("ex intcici %d\n",nip);
          if(nip < 1) goto L_no_sol;
          break;

        case Typ_CVELL:  // CI x CVELL
// TODO: UT3D_rbez_el must give trimmed ellipse, until now untrimmed;
          UT3D_pt_intciel (&nip, pa, (Circ*)o1, (CurvElli*)o2, wrkSpc);
          if(nip < 1) goto L_no_sol;
          // test if points are on Elli o2
          UT3D_pt_ell_lim_del (&nip, pa, va2, (CurvElli*)o2);
          break;

        case Typ_CVPOL:  // CI x CVPOL
          nip = aSiz;
          UT3D_pt_intciplg (&nip, pa, va2, (Circ*)o1, (CurvPoly*)o2);
          break;

        case Typ_CVBSP:  // CI x CVBSP
          nip = aSiz;
          UT3D_pt_intcibspl (&nip, pa, (Circ*)o1, (CurvBSpl*)o2, wrkSpc);
          if(nip < 1) goto L_no_sol;
          break;

        case Typ_PLN:  // CI x R
          // irc = UT3D_pt_intplci (&nip, pa, (Plane*)o2, (Circ*)o1);
          nip = UT3D_pt_intplnci (pa, (Plane*)o2, (Circ*)o1, 1);
          if(nip < 1) goto L_no_sol;
          // test if points are on Circ o1
          UT3D_pt_ci_lim_del (&nip, pa, va1, (Circ*)o1);
          break;

        case Typ_Val:
        case Typ_Par1:
        case Typ_Float8:
          nip = 1;
          UT3D_pt_evparci (pa, *((double*)o2), (Circ*)o1);
          if(va1) va1[0] = *((double*)o2);
          break;

        default:
          goto ParErr;
      }
      break;

    //================================================================
    case Typ_CVPOL:  // o1=CVPOL 8

      switch (o2Form) {

        case Typ_Val:  // Parameter -> Plg
        case Typ_Par1:
        case Typ_Float8:
          nip = 1;
          // get knotVal of par1
          d1 = UT3D_par_par1plg (*((double*)o2), (CurvPoly*)o1);
          UT3D_pt_evalplg (pa, (CurvPoly*)o1, d1);
          nip = 1;
            // printf(" plg va1=%f ex par=%f\n",va1[0],*(double*)o2);
          if(va1) va1[0] = *((double*)o2);
          break;

        case Typ_CVPOL:  // CVPOL x CVPOL
          nip = aSiz;
          UT3D_pt_int2plg (&nip, pa, va1, va2, (CurvPoly*)o1,(CurvPoly*)o2);
          break;

        case Typ_CVELL:  // CVPOL x CVELL
          nip = aSiz;
          UT3D_pt_intelplg (&nip, pa, va2, va1, (CurvElli*)o2, (CurvPoly*)o1);
          break;

        case Typ_CVBSP:  // CVPOL x CVBSP
          nip = aSiz;
          UT3D_pt_intbspplg (&nip, pa, va1, wrkSpc, (CurvBSpl*)o2, (CurvPoly*)o1);
          break;

        // case Typ_CVRBSP:  // CVPOL x CVRBSP

        case Typ_PLN:  // CVPOL x R
          irc = UT3D_pt_intplplg (&nip, pa, (Plane*)o2, (CurvPoly*)o1);
          if(irc != 0) return -1;
          break;

        default:
          goto ParErr;
      }
      break;

    //================================================================
    case Typ_CVELL:  // o1 = CVELL 13

      switch (o2Form) {

        case Typ_Val:  // value -> CVELL        // 2013-03-15
        case Typ_Float8:
          nip = 1;
          UT3D_pt_eval_ell_par1 (pa, (CurvElli*)o1, *((double*)o2));
          if(va1) va1[0] = *((double*)o2);
          break;

        case Typ_CVELL:  // CVELL x CVELL
// TODO: UT3D_rbez_el must give trimmed ellipse, until now untrimmed; 
//   then UT3D_pt_ell_lim_del can be removed.
          UT3D_pt_intelel (&nip, pa, (CurvElli*)o2, (CurvElli*)o1, wrkSpc);
          if(nip < 1) goto L_no_sol;
          // test if points are on Elli o1
          UT3D_pt_ell_lim_del (&nip, pa, va1, (CurvElli*)o2);
          // test if points are on Elli o1
          UT3D_pt_ell_lim_del (&nip, pa, va1, (CurvElli*)o1);
          break;

        case Typ_CVBSP:  // CVELL x CVBSP
          nip = aSiz;
// TODO: UT3D_rbez_el must give trimmed ellipse, until now untrimmed; 
//   then UT3D_pt_ell_lim_del can be removed.
          UT3D_pt_intelbspl (&nip, pa, (CurvElli*)o1, (CurvBSpl*)o2, wrkSpc);
          if(nip < 1) goto L_no_sol;
          UT3D_pt_ell_lim_del (&nip, pa, va1, (CurvElli*)o1);
          break;

        case Typ_PLN:  // CVELL x R
          irc = UT3D_pt_intplell (&nip, pa, (Plane*)o2, (CurvElli*)o1);
          if(irc != 0) return -1;
          UT3D_pt_ell_lim_del (&nip, pa, va1, (CurvElli*)o1);
          break;

        default:
          goto ParErr;
      }
      break;


    //================================================================
    case Typ_CVBSP:            // o1 = CVBSP 10

      switch (o2Form) {

        case Typ_Par1:
        case Typ_Float8:
          nip = 1;
          d1 = UT3D_parbsp_par1 (*((double*)o2), (CurvBSpl*)o1);
            // printf(" par %f < %f\n",va1[0],*((double*)o2));
          irc = UT3D_pt_evparCrvBSpl (pa, (CurvBSpl*)o1, d1);
          if(irc < 0){TX_Error("UTO_stru_int E002"); return irc;}
          if(va1) va1[0] = *((double*)o2);
          break;

        case Typ_Val:  // Parameter -> CVBSP
          // dzt kein umrechenen von 0-1 Parameter in Knotenwert;
          // dazu ev eigenen Typ fuer Parameter bzw Knotenwert verwenden ?
          // see UT3D_parbsp_par1
          nip = 1;
          irc = UT3D_pt_evparCrvBSpl (pa, (CurvBSpl*)o1, *(double*)o2);
          if(irc < 0){TX_Error("UTO_stru_int E002"); return irc;}
          if(va1) va1[0] = *((double*)o2);
          break;

        // case Typ_PT:  // PT -> CVBSP
          // irc = UT3D_pt_projptbspl (&nip,pa,va1,(CurvBSpl*)o1,(Point*)o2);
          // if(irc > 0) nip = 1;
          // if(irc < 0) { TX_Error("UTO_stru_int E003"); return irc; }
          // break;

        case Typ_LN: // CVBSP x LN
          nip = aSiz;
          UT3D_pt_intlnbspl (&nip, pa, wrkSpc, (Line*)o2, (CurvBSpl*)o1);
          break;

        case Typ_CI:  // CVBSP x CI
          nip = aSiz;
          UT3D_pt_intcibspl (&nip, pa, (Circ*)o2, (CurvBSpl*)o1, wrkSpc);
          break;

        case Typ_CVELL:  // CVBSP x CVELL
          nip = aSiz;
          UT3D_pt_intelbspl (&nip, pa, (CurvElli*)o2, (CurvBSpl*)o1, wrkSpc);
          break;

        case Typ_CVPOL:  // CVBSP x CVPOL
          nip = aSiz;
          UT3D_pt_intbspplg (&nip, pa, va2, wrkSpc, (CurvBSpl*)o1, (CurvPoly*)o2);
          break;

        case Typ_CVBSP:  // CVBSP x CVBSP
          nip = aSiz;
          UT3D_pt_int2bspl (&nip, pa, wrkSpc, (CurvBSpl*)o2, (CurvBSpl*)o1);
          break;

        case Typ_PLN:  // CVBSP x R
          nip = aSiz;
          irc = UT3D_pt_intplbspl (&nip, pa, (Plane*)o2, (CurvBSpl*)o1,
                                   wrkSpc, UT_TOL_cv);
          if(irc != 0) return -1;
          break;

        case Typ_CVRBSP:  // CVBSP x Typ_CVRBSP
          nip = aSiz;
          irc = UT3D_pt_int_bsp_rbsp (&nip, pa, wrkSpc,
                                      (CurvBSpl*)o1, (CurvRBSpl*)o2);
          break;

        default:
          goto ParErr;
      }
      break;



    //================================================================
    case Typ_CVRBSP:                  // o1=Rat-B-Spline 11

      switch (o2Form) {

        // case Typ_PT:   // project point > Curve; return also Parameter (va1)
          // irc = UT3D_pt_projptrbspl (&nip,pa,va1,wrkSpc, UT_TOL_cv,  // UT_TOL_pt,
                                     // (CurvRBSpl*)o1,(Point*)o2);
          // if(irc > 0) nip = 1;
          // if(irc < 0){TX_Error("UTO_stru_int E004"); return irc;}
          // break;

        case Typ_CVBSP:  // CVRBSP x Typ_CVBSP
          nip = aSiz;
          irc = UT3D_pt_int_bsp_rbsp (&nip, pa, wrkSpc, (CurvBSpl*)o2, (CurvRBSpl*)o1);
          break;

        default:
          goto ParErr;
      }
      break;



    //================================================================
    case Typ_CVCLOT:                  // o1=Clot 16

      switch (o2Form) {

        case Typ_Val:  // Clot X Parameter -> Plg
        case Typ_Par1:
        case Typ_Float8:
          UT3D_ptvc_evparclot (pa, obj1, 0, (CurvClot*)o1, *((double*)o2));
          nip = 1;
          if(va1) va1[0] = *((double*)o2);
          break;

        default:
          goto ParErr;

      }
      break;


    //================================================================
    case Typ_PLN:   // R 40

      switch (o2Form) {

        case Typ_LN:      // R x L
        case Typ_CI:      // R x C
        case Typ_CVPOL:   // R x CVPOL
        case Typ_CVELL:   // R x CVELL
        case Typ_CVBSP:   // R x CVBSP
          // goto L_swap;

        default:
          goto ParErr;

      }
      break;


    //================================================================
    case Typ_Val:     // LN X Parameter 130
    case Typ_Par1:    // 140
    case Typ_Float8:  // 175




    //================================================================
    default:
      goto ParErr;
  }


  // test if va of (real) obj1 is filled
  if(swapped) {
    MEM_swap_2vp (&ox1, &ox2);
    // MEM_swap_2vp (&va1, &va2);
    // copy back data va2 -> va1
    if(nip > 0) memcpy (va1o, va2, sizeof(double) * nip);
    va1 = va1o;  // restore pointer -> va1
      // printf(" swapped back ...\n");

  }
  o1Form = ox1->form;
  o1     = ox1->data;
    // printf(" o1Form=%d nip=%d va=%lf\n",o1Form,nip,va1[0]);


  // compute parameters
  if((va1) && (nip > 0)) {
    if(va1[0] == UT_VAL_MAX) {
      // compute paramters of ox1
      switch (o1Form) {
        case Typ_LN:
          for(i1=0; i1<nip; ++i1)
            UT3D_par_pt_2pt (&va1[i1], &pa[i1],
                             &((Line*)o1)->p1, &((Line*)o1)->p2);
          break;
        case Typ_CI:
          for(i1=0; i1<nip; ++i1)
            va1[i1] = UT3D_par1_ci_pt ((Circ*)o1, &pa[i1]);
          break;
        case Typ_CVPOL:
          for(i1=0; i1<nip; ++i1) {
            UT3D_parplg_plgpt (&va1[i1], &pa[i1], (CurvPoly*)o1);
            va1[i1] = UT3D_par1_parplg (&va1[i1], (CurvPoly*)o1);
          }
          break;
        case Typ_CVELL:
          for(i1=0; i1<nip; ++i1)
            UT3D_par1_el_pt (&va1[i1], (CurvElli*)o1, &pa[i1]);
          break;
        case Typ_CVBSP:
          for(i1=0; i1<nip; ++i1)
            UT3D_parCv_bsplpt (&va1[i1], &d1, (CurvBSpl*)o1, &pa[i1]);
          break;
        case Typ_CVRBSP:
          for(i1=0; i1<nip; ++i1) {
            UT3D_par_rbsp_pt (&va1[i1], &d1, (CurvRBSpl*)o1, &pa[i1]);
            va1[i1] = UT3D_par1_par_rbsp (&va1[i1], (CurvRBSpl*)o1);
          }
          break;
        default:
          goto ParErr;
      }
    }
  }



  L_exit:
    // nip intersection-points found.
    // if(nip > 0) *pNr += nip;
    if(nip > 0) *pNr = nip;



//     // TEST ONLY
    // printf("ex UTO_npt_int_2ox nip=%d pNr=%d aSiz=%d\n",nip,*pNr,aSiz);
    // for(i1=0; i1<nip; ++i1) {
      // UT3D_stru_dump (Typ_PT, &pa[i1], " ex UTO_npt_int_2ox [%d]:",i1);
      // if(va1) printf(" va1[%d]=%f\n",i1,va1[i1]);
    // }
    // printf("iiiiiiiiiiiiiiiiiiiiiiiiiiiiii ex UTO_npt_int_2ox\n\n");
//     // END TEST ONLY



  return 0;

/*
  //----------------------------------------------------------------
  L_v_Circ: // get parameters on o1=CI
    if(va1) {
      for(i1=0; i1<nip; ++i1) {
        va1[i1] = UT3D_par1_ci_pt ((Circ*)o1, &pa[i1]);
          // printf(" va1[%d]=%lf\n",i1,va1[i1]);
      }
    }
    goto L_exit;

  //----------------------------------------------------------------
  L_v_Elli:   // get parameters on oo1=Elli
    if(nip > 0) {
      // get start/end-parameter for ellipse
      ps = ((CurvElli*)oo1)->p1;
      pe = ((CurvElli*)oo1)->p2;
      // UT3D_par1_el_pt (&us, (CurvElli*)oo1, &ps);
      // UT3D_par1_el_pt (&ue, (CurvElli*)oo1, &pe);
      us = 0.;
      ue = 1.;
        printf(" us=%lf ue=%lf\n",us,ue);
      // get all parameters for points
      for(i1=0; i1<nip; ++i1) {
          UT3D_stru_dump (Typ_PT, &pa[i1], " p[%d]",i1);
        UT3D_par1_el_pt (&va1[i1], (CurvElli*)oo1, &pa[i1]);
          printf(" va[%d] = %lf\n",i1,va1[i1]);
      }
      // delete all points & parameters not on obj
      UT3D_par_ck_inObj_del (&nip, pa, va1, &ps, &pe, us, ue);
    }
    goto L_exit;

  //----------------------------------------------------------------
  L_v_cBsp:   // get parameters on o1=CurvBSpl
    if(va1) {
      char   tmpSpc[50000];
      Memspc wrkSpc;
      for(i1=0; i1<nip; ++i1) {
        // UME_init (&wrkSpc, tmpSpc, 50000);
        UT3D_parCv_bsplpt (&va1[i1], &d1, (CurvBSpl*)o1, &pa[i1]);
          // UT3D_par1_parbsp
          // printf(" va1[%d]=%lf\n",i1,va1[i1]);
      }
    }
    goto L_exit;


  //----------------------------------------------------------------
  L_v_cRBsp:   // get parameters on o1=CurvRBSpl
    TX_Print("ERROR UTO_stru_int E005");
    if(va1) {
      char   tmpSpc[50000];
      Memspc wrkSpc;
      for(i1=0; i1<nip; ++i1) {
        // UME_init (&wrkSpc, tmpSpc, 50000);
        UT3D_parCv_bsplpt (&va1[i1], &d1, (CurvBSpl*)o1, &pa[i1]);
          // UT3D_par1_parbsp
          printf(" va1[%d]=%lf\n",i1,va1[i1]);
      }
    }
    goto L_exit;
*/





  //----------------------------------------------------------------
  L_no_sol:
    *pNr = 0;
    return 0;

  NoInt:
  printf("UTO_npt_int_2ox E0 - no intersection .. %d %d\n",o1Form,o2Form);
  return -1;

  ParErr:
  printf("UTO_npt_int_2ox E2 - parameter error .. %d %d\n",o1Form,o2Form);
  return -1;


  //----------------------------------------------------------------
  ErrMem:
  TX_Error("UTO_npt_int_2ox E1 - out of memspace .. %d %d\n",o1Form,o2Form);
  return -1;

  DBErr:
  TX_Error("UTO_npt_int_2ox E3 DB - error .. %d %d\n",o1Form,o2Form);
  return -1;

}


//=====================================================================
  int UTO_stru_int (int *pNr, Point *pa, double *va1,
                    int o1Typ, void *o1, int o2Typ, void *o2,
                    Memspc *wrkSpc) {
//=====================================================================
/// \code
/// intersect 2 (unlimited) objects (Typ, struct) where result = points
///   handles CURV X SURF and CURV X CURV, but not SURF X SURF.
/// Input:
///   pNr      size of pa
///   o1       obj to cut (must be struct, not ObjGX)
///   o1Typ    type of o1
///   o2       cutting obj;
///   o2Typ    type of o2 (must be struct, not ObjGX)
///            o2Typ=Typ_Val: o2=(double*)parameter;
///   pa  == NULL:   no points
///   va1 == NULL:   no parameters
/// Output:
///   pNr   number of points in pa
///   pa    intersectionPoints (if pa != NULL)
///         Typ_CVPOL X Typ_Val:  returns only va1, not pa.
///   va1   corresponding parameters on o1 (if va1 != NULL)
/// RetCod:
///    0    OK, 0-<pNr> intersectionpoints in pa.
///   -1    Error
///
///  intersect limited objects: see UTO_npt_int_2ox
/// TODO:
///   requested output (points|parameters|both) should be
///     controlled by a inputParameter.
/// \endcode




  int    irc, i1, i2, ip, io, rNr;
  double d1, *vp1;
  char   tmpSpc[OBJ_SIZ_MAX];
  // Vector vc1;
  ObjGX  *oTab;
  void   *oo1;


  // printf("==================================================\n");
  // printf("UTO_stru_int o1Typ=%d o2Typ=%d %d\n",o1Typ,o2Typ,*pNr);
  // UT3D_stru_dump (o1Typ, o1, "obj to cut:");
  // UT3D_stru_dump (o2Typ, o2, "cutting obj:");
  // printf(" o2=%f\n", *((double*)o2));



  goto L_Start;


  //----------------------------------------------------------------
  L_swap: // swap tables ..
  MEM_swap_int (&o1Typ, &o2Typ);
  MEM_swap_2vp (&o1, &o2);



  //----------------------------------------------------------------
  L_Start:
  switch (o1Typ) {       // o1 = obj to cut

    //================================================================
    case Typ_LN:   // o1=LN

      switch (o2Typ) {

        case Typ_Val:  // Parameter -> LN
        case Typ_Par1:
          *pNr = 1;
          UT3D_pt_evparln (pa, *((double*)o2), (Line*)o1);
          if(va1) va1[0] = *((double*)o2);
          break;

        case Typ_PT:  // PT -> LN
          UT3D_pt_projptln (pa, &d1, (Point*)o2, (Line*)o1);
          *pNr = 1;
            // UT3D_stru_dump (Typ_PT, pa, "LN-PT:");
            // printf(" d1=%f\n",d1);
          break;

        case Typ_LN: // LN x LN
          UT3D_pt_int2ln (&pa[0],&pa[1], NULL, (Line*)o1, (Line*)o2);
          *pNr = 1;
          break;

        case Typ_CI:  // LN x AC
          UT3D_pt_intlnci__ (pNr, pa, (Line*)o1, (Circ*)o2);
          // get parameters on o1=LN
          break;

        case Typ_CVELL:  // LN x CVELL
          UT3D_pt_intlnel__ (pNr, pa, (Line*)o1, (CurvElli*)o2);
          break;

        case Typ_CVPOL:  // LN x CVPOL
          UT3D_pt_intlnplg (pNr, pa, va1, (Line*)o1, (CurvPoly*)o2, UT_TOL_cv);
          break;

        case Typ_CVBSP:  // LN x CVBSP
          UT3D_pt_intlnbspl (pNr, pa, wrkSpc, (Line*)o1, (CurvBSpl*)o2);
          break;

        case Typ_CVCLOT:  // LN x Clot
          goto L_swap;

        case Typ_PLN:  // L x R
          irc = UT3D_pt_intlnpl (pa, (Plane*)o2, (Line*)o1);
          if(irc < 0) return -1;
          *pNr = 1;
          break;

        default:
          goto ParErr;
      }
      break;


    //================================================================
    case Typ_CI:    // o1=AC

      switch (o2Typ) {

        case Typ_Val:  // Parameter -> LN
        case Typ_Par1:
          *pNr = 1;
          UT3D_pt_evparci (pa, *((double*)o2), (Circ*)o1);
          if(va1) va1[0] = *((double*)o2);
          break;

        case Typ_PT:  // PT -> AC
          irc = UT3D_pt_projptci (&pa[0], &pa[1], (Point*)o2, (Circ*)o1);
          if(irc < 0) { *pNr = 0; goto L_exit; }
          *pNr = 2;
          break;

        case Typ_LN: // AC x LN
          UT3D_pt_intlnci__ (pNr, pa, (Line*)o2, (Circ*)o1);
            // printf(" pNr=%d\n",*pNr);
          goto L_v_Circ;   // get parameters on o1=CI

        case Typ_CI:  // AC x AC
          *pNr = UT3D_pt_intcici (pa, (Circ*)o1, 1, (Circ*)o2, 1);
          goto L_v_Circ;   // get parameters on o1=CI

        case Typ_CVELL:  // AC x CVELL
          UT3D_pt_intciel (pNr, pa, (Circ*)o1, (CurvElli*)o2, wrkSpc);
          goto L_v_Circ;   // get parameters on o1=CI

        case Typ_CVPOL:  // AC x CVPOL
          UT3D_pt_intciplg (pNr, pa, va1, (Circ*)o1, (CurvPoly*)o2);
          goto L_v_Circ;   // get parameters on o1=CI

        case Typ_CVBSP:  // AC x CVBSP
          UT3D_pt_intcibspl (pNr, pa, (Circ*)o1, (CurvBSpl*)o2, wrkSpc);
          goto L_v_Circ;   // get parameters on o1=CI

        case Typ_PLN:  // C x R
          // irc = UT3D_pt_intplci (pNr, pa, (Plane*)o2, (Circ*)o1);
          *pNr = UT3D_pt_intplnci (pa, (Plane*)o2, (Circ*)o1, 1);
          if(*pNr < 0) return -1;
          goto L_v_Circ;   // get parameters on o1=CI

        default:
          goto ParErr;
      }
      break;

    //================================================================
    case Typ_CVPOL:  // o1=CVPOL

      switch (o2Typ) {

        case Typ_Val:  // Parameter -> Plg
        case Typ_Par1:
          *pNr = 1;
          // get knotVal of par1
          d1 = UT3D_par_par1plg (*((double*)o2), (CurvPoly*)o1);
          UT3D_pt_evalplg (pa, (CurvPoly*)o1, d1);
          *pNr = 1;
            // printf(" plg va1=%f ex par=%f\n",va1[0],*(double*)o2);
          if(va1) va1[0] = *((double*)o2);
          break;

        case Typ_PT:  // PT -> CVPOL
          UT3D_pt_projptplg (pNr, pa, va1, (CurvPoly*)o1, (Point*)o2);
          break;

        case Typ_LN: // CVPOL x LN
          UT3D_pt_intlnplg (pNr, pa, va1, (Line*)o2, (CurvPoly*)o1, UT_TOL_cv);
          break;

        case Typ_CI:  // CVPOL x AC
          UT3D_pt_intciplg (pNr, pa, va1, (Circ*)o2, (CurvPoly*)o1);
          break;

        case Typ_CVELL:  // CVPOL x CVELL
          UT3D_pt_intelplg (pNr, pa, NULL, va1, (CurvElli*)o2, (CurvPoly*)o1);
          break;

        case Typ_CVPOL:  // CVPOL x CVPOL
          UT3D_pt_int2plg (pNr, pa, va1, &va1[*pNr], (CurvPoly*)o1,(CurvPoly*)o2);
          break;

        case Typ_CVBSP:  // CVPOL x CVBSP
          UT3D_pt_intbspplg (pNr, pa, va1, wrkSpc, (CurvBSpl*)o2, (CurvPoly*)o1);
          break;

        case Typ_PLN:  // CVPOL x R
          irc = UT3D_pt_intplplg (pNr, pa, (Plane*)o2, (CurvPoly*)o1);
          if(irc != 0) return -1;
          if(va1) {
            for(i1=0; i1<*pNr; ++i1) {
              UT3D_parplg_plgpt (&va1[i1], &pa[i1], (CurvPoly*)o1);
              va1[i1] = UT3D_par1_parplg (&va1[i1], (CurvPoly*)o1);
            }
          }
          break;

        default:
          goto ParErr;
      }
      break;

    //================================================================
    case Typ_CVELL:  // o1 = CVELL

      switch (o2Typ) {

        case Typ_Val:  // value -> CVELL        // 2013-03-15
          *pNr = 1;
          UT3D_pt_eval_ell_par1 (pa, (CurvElli*)o1, *((double*)o2));
          if(va1) va1[0] = *((double*)o2);
          break;

        case Typ_PT:  // PT -> CVELL
          UT3D_pt_projptel (pNr, pa, (CurvElli*)o1, (Point*)o2);
          goto L_v_Elli;   // get parameters on o1=Elli

        case Typ_LN: // CVELL x LN
          UT3D_pt_intlnel__ (pNr, pa, (Line*)o2, (CurvElli*)o1);
          goto L_v_Elli;   // get parameters on o1=Elli

        case Typ_CI:  // CVELL x AC
          UT3D_pt_intciel (pNr, pa, (Circ*)o2, (CurvElli*)o1, wrkSpc);
          goto L_v_Elli;   // get parameters on o1=Elli

        case Typ_CVELL:  // CVELL x CVELL
          UT3D_pt_intelel (pNr, pa, (CurvElli*)o2, (CurvElli*)o1, wrkSpc);
          goto L_v_Elli;   // get parameters on o1=Elli

        case Typ_CVPOL:  // CVELL x CVPOL
          UT3D_pt_intelplg (pNr, pa, va1, NULL, (CurvElli*)o1, (CurvPoly*)o2);
          goto L_v_Elli;   // get parameters on o1=Elli

        case Typ_CVBSP:  // CVELL x CVBSP
          UT3D_pt_intelbspl (pNr, pa, (CurvElli*)o1, (CurvBSpl*)o2, wrkSpc);
          goto L_v_Elli;   // get parameters on o1=Elli

        case Typ_PLN:  // CVELL x R
          irc = UT3D_pt_intplell (pNr, pa, (Plane*)o2, (CurvElli*)o1);
          if(irc != 0) return -1;
          goto L_v_Elli;   // get parameters on o1=Elli

        default:
          goto ParErr;
      }
      break;


    //================================================================
    case Typ_CVBSP:            // o1 = CVBSP

      switch (o2Typ) {

        case Typ_Par1:
          *pNr = 1;
          d1 = UT3D_parbsp_par1 (*((double*)o2), (CurvBSpl*)o1);
            // printf(" par %f < %f\n",va1[0],*((double*)o2));
          irc = UT3D_pt_evparCrvBSpl (pa, (CurvBSpl*)o1, d1);
          if(irc < 0){TX_Error("UTO_stru_int E002"); return irc;}
          if(va1) va1[0] = *((double*)o2);
          break;

        case Typ_Val:  // Parameter -> CVBSP
          // dzt kein umrechenen von 0-1 Parameter in Knotenwert;
          // dazu ev eigenen Typ fuer Parameter bzw Knotenwert verwenden ?
          // see UT3D_parbsp_par1
          *pNr = 1;
          irc = UT3D_pt_evparCrvBSpl (pa, (CurvBSpl*)o1, *(double*)o2);
          if(irc < 0){TX_Error("UTO_stru_int E002"); return irc;}
          if(va1) va1[0] = *((double*)o2);
          break;

        case Typ_PT:  // PT -> CVBSP
          irc = UT3D_pt_projptbspl (pNr,pa,va1,(CurvBSpl*)o1,(Point*)o2);
          if(irc < 0) { TX_Error("UTO_stru_int E003"); return irc; }
          break;

        case Typ_LN: // CVBSP x LN
          UT3D_pt_intlnbspl (pNr, pa, wrkSpc, (Line*)o2, (CurvBSpl*)o1);
          goto L_v_cBsp;   // get parameters on o1

        case Typ_CI:  // CVBSP x AC
          UT3D_pt_intcibspl (pNr, pa, (Circ*)o2, (CurvBSpl*)o1, wrkSpc);
          goto L_v_cBsp;   // get parameters on o1

        case Typ_CVELL:  // CVBSP x CVELL
          UT3D_pt_intelbspl (pNr, pa, (CurvElli*)o2, (CurvBSpl*)o1, wrkSpc);
          goto L_v_cBsp;   // get parameters on o1

        case Typ_CVPOL:  // CVBSP x CVPOL
          UT3D_pt_intbspplg (pNr, pa, NULL, wrkSpc, (CurvBSpl*)o1, (CurvPoly*)o2);
          goto L_v_cBsp;   // get parameters on o1

        case Typ_CVBSP:  // CVBSP x CVBSP
          UT3D_pt_int2bspl (pNr, pa, wrkSpc, (CurvBSpl*)o2, (CurvBSpl*)o1);
          goto L_v_cBsp;   // get parameters on o1

        case Typ_PLN:  // CVBSP x R
          irc = UT3D_pt_intplbspl (pNr, pa, (Plane*)o2, (CurvBSpl*)o1,
                                   wrkSpc, UT_TOL_cv);
          if(irc != 0) return -1;
          goto L_v_cBsp;   // get parameters on o1

        case Typ_CVRBSP:  // CVBSP x Typ_CVRBSP
          irc = UT3D_pt_int_bsp_rbsp (pNr, pa, wrkSpc, (CurvBSpl*)o1, (CurvRBSpl*)o2);
          goto L_v_cBsp;   // get parameters on o1

        default:
          goto ParErr;
      }
      break;



    //================================================================
    case Typ_CVRBSP:                  // o1=Rat-B-Spline

      switch (o2Typ) {

        case Typ_PT:   // project point > Curve; return also Parameter (va1)
          irc = UT3D_pt_projptrbspl (pNr,pa,va1,wrkSpc, UT_TOL_cv,  // UT_TOL_pt,
                                     (CurvRBSpl*)o1,(Point*)o2);
          if(irc > 0) *pNr = 1;
          if(irc < 0){TX_Error("UTO_stru_int E004"); return irc;}
          break;

        case Typ_CVBSP:  // CVRBSP x Typ_CVBSP
          irc = UT3D_pt_int_bsp_rbsp (pNr, pa, wrkSpc, (CurvBSpl*)o2, (CurvRBSpl*)o1);
          break;

        default:
          goto ParErr;
      }
      break;



    //================================================================
    case Typ_CVCLOT:                  // o1=Clot

      switch (o2Typ) {

        case Typ_Val:  // Clot X Parameter -> Plg
        case Typ_Par1:
          UT3D_ptvc_evparclot (&pa[0], tmpSpc, 0, (CurvClot*)o1, *((double*)o2));
          *pNr = 1;
          if(va1) va1[0] = *((double*)o2);
          break;

        case Typ_LN: // Clot x LN
          // make pt, vc from line ...
          UT3D_vc_2pt ((Vector*)tmpSpc, &((Line*)o2)->p1, &((Line*)o2)->p2);
          UT3D_pt_intclotptvc (pNr, pa, va1,
                               &((Line*)o2)->p1, (Vector*)tmpSpc,
                               (CurvClot*)o1, UT_DISP_cv);
          break;

        default:
          goto ParErr;

      }
      break;



    //================================================================
    case Typ_CVCCV:                     // o1=CCV
      // loop tru segments of ccv; intersect each segment. RECURSE !
      // add intersectionpoints into pa reducing its free size.
      // i2 = ((CurvCCV*)o1)->segNr;
      // oTab = ((CurvCCV*)o1)->cvtab;
      i2 = ((ObjGX*)o1)->siz;
      oTab = ((ObjGX*)o1)->data;
        printf(" isegNr=%d\n",i2);
      io = 0;
      vp1 = NULL;
      for(i1=0; i1<i2; ++i1) {
        // get struct
        o1Typ = UTO_obj_getp (&oo1, &rNr, &oTab[i1]);
        if(o1Typ < 1) goto DBErr;
        if(o1Typ == Typ_PT) continue;  // skip points
        ip = *pNr - io;  // remaining size of pa, va1
        if(va1) vp1 = &va1[io];
        irc = UTO_stru_int (&ip, &pa[io], vp1, o1Typ,oo1, o2Typ,o2, wrkSpc);
        if(irc < 0) goto DBErr;
        io += ip;  // offset for pa, va1
      }
      *pNr = io;
      break;



    //================================================================
    case Typ_PLN:

      switch (o2Typ) {

        case Typ_LN:      // R x L
        case Typ_CI:      // R x C
        case Typ_CVPOL:   // R x CVPOL
        case Typ_CVELL:   // R x CVELL
        case Typ_CVBSP:   // R x CVBSP
          goto L_swap;

        default:
          goto ParErr;

      }
      break;



    //================================================================
    default:
      goto ParErr;
  }


  L_exit:

/*
//     // TEST ONLY
    for(i1=0; i1<*pNr; ++i1) {
      UT3D_stru_dump (Typ_PT, &pa[i1], " ex UTO_stru_int [%d]:",i1);
      if(va1) printf(" va1[%d]=%f\n",i1,va1[i1]);
    }
    printf("---------- ex UTO_stru_int pNr=%d\n",*pNr);
   
//     // END TEST ONLY
*/

  return 0;


  //----------------------------------------------------------------
  L_v_Circ: // get parameters on o1=CI
    if(va1) {
      for(i1=0; i1<*pNr; ++i1) {
        va1[i1] = UT3D_par1_ci_pt ((Circ*)o1, &pa[i1]);
          // printf(" va1[%d]=%lf\n",i1,va1[i1]);
      }
    }
    goto L_exit;


  //----------------------------------------------------------------
  L_v_Elli:   // get parameters on o1=Elli
    if(va1) {
      for(i1=0; i1<*pNr; ++i1) {
        UT3D_par1_el_pt (&va1[i1], (CurvElli*)o1, &pa[i1]);
          // printf(" va1[%d]=%lf\n",i1,va1[i1]);
      }
    }
    goto L_exit;


  //----------------------------------------------------------------
  L_v_cBsp:   // get parameters on o1=CurvBSpl
    if(va1) {
      char   tmpSpc[50000];
      Memspc wrkSpc;
      for(i1=0; i1<*pNr; ++i1) {
        // UME_init (&wrkSpc, tmpSpc, 50000);
        UT3D_parCv_bsplpt (&va1[i1], &d1, (CurvBSpl*)o1, &pa[i1]);
          // UT3D_par1_parbsp
          // printf(" va1[%d]=%lf\n",i1,va1[i1]);
      }
    }
    goto L_exit;


  //----------------------------------------------------------------
  L_v_cRBsp:   // get parameters on o1=CurvRBSpl
    TX_Print("ERROR UTO_stru_int E005");
/*
    if(va1) {
      char   tmpSpc[50000];
      Memspc wrkSpc;
      for(i1=0; i1<*pNr; ++i1) {
        // UME_init (&wrkSpc, tmpSpc, 50000);
        UT3D_parCv_bsplpt (&va1[i1], &d1, (CurvBSpl*)o1, &pa[i1]);
          // UT3D_par1_parbsp
          printf(" va1[%d]=%lf\n",i1,va1[i1]);
      }
    }
*/
    goto L_exit;





  //----------------------------------------------------------------
  ParErr:
  TX_Error("UTO_stru_int E001 %d %d",o1Typ,o2Typ);
  return -1;

  DBErr:
  TX_Error("UTO_stru_int E002 %d %d",o1Typ,o2Typ);
  return -1;
}


//=======================================================================
  int UTO_obj_tra_m3 (void *stro, int typ, void *stri, Mat_4x3 trmat) {
//=======================================================================
/// \code
/// DO NOT USE - use UTRA_app__
/// stro must have enough size for complete object
/// \endcode


  // double  d1;
  Line    *lni, *lno;
  Circ    *cii, *cio;
  CurvElli *eli, *elo;


  // printf("UTO_obj_tra_m3 %d\n",typ);




  if(typ == Typ_PT) {
    UT3D_pt_traptm3 (stro, trmat, stri);


  } else if(typ == Typ_LN) {
    lni = stri; lno = stro;
    UT3D_pt_traptm3 (&lno->p1, trmat, &lni->p1);
    UT3D_pt_traptm3 (&lno->p2, trmat, &lni->p2);




  } else if(typ == Typ_CI) {
    cii = stri; cio = stro;
    UT3D_pt_traptm3 (&cio->p1, trmat, &cii->p1);
    UT3D_pt_traptm3 (&cio->p2, trmat, &cii->p2);
    UT3D_pt_traptm3 (&cio->pc, trmat, &cii->pc);
    UT3D_vc_travcm3 (&cio->vz, trmat, &cii->vz);
    UT3D_vc_normalize (&cio->vz, &cio->vz);
    cio->rad = cii->rad;

    // printf("UTO_obj_tra_m3 vz=%f,%f,%f\n",cio->vz.dx,cio->vz.dy,cio->vz.dz);




  } else if(typ == Typ_CVELL) {
    eli = stri; elo = stro;
    UT3D_pt_traptm3 (&elo->p1, trmat, &eli->p1);
    UT3D_pt_traptm3 (&elo->p2, trmat, &eli->p2);
    UT3D_pt_traptm3 (&elo->pc, trmat, &eli->pc);
    UT3D_vc_travcm3 (&elo->va, trmat, &eli->va);
    UT3D_vc_travcm3 (&elo->vb, trmat, &eli->vb);
    UT3D_vc_travcm3 (&elo->vz, trmat, &eli->vz);
    UT3D_vc_normalize (&elo->vz, &elo->vz);
    // printf(" in  va=%f %f %f\n",eli->va.dx,eli->va.dy,eli->va.dz);
    // printf(" out va=%f %f %f\n",elo->va.dx,elo->va.dy,elo->va.dz);




  } else {
    TX_Error("UTO_obj_tra_m3 E001 / %d",typ);
    return -1;
  }

  return 0;

}


/* UNUSED; use UPRJ_app__
//=======================================================================
  int UTO_ox_tra (ObjGX **cvo, ObjGX *cvi, Mat_4x3 trmat) {
//=======================================================================
/// \code
/// DO NOT USE
/// REPLACE WITH UTRA_app_obj !!
/// \endcode

// dzt nur f read-iges-112 (Parametric Spline Curve) verwendet..
// Achtung: cvo muss Startadresse des Bereichs liefern !!
// macht ein Duplikat vom ObjGX cv1
// Die DatenStructs werden unmittelbar hinter der ObjGX-struct gespeichert !


  int        typ, i1;
  long       l1;
  char       *pe;
  Point2     *pt2a;
  Point      *pta, pt1, pt0;
  polynom_d3 *pola, pol1;
  CurvBSpl   *cvbi, *cvbo;
  GText      *tx1;
  Dimen      *dim1;
  ObjGX      *ox1;


  printf("UTO_ox_tra typ=%d form=%d siz=%d\n",cvi->typ,cvi->form,cvi->siz);


  // ox1 = memspc54;
  // *cvo = ox1;
  ox1 = *cvo;

  // copy primary ObjGX
  memcpy(ox1, cvi, sizeof(ObjGX));
  // set startAdress of Data to space behind primary ObjGX
  pe = (char*)ox1 + sizeof(ObjGX);
  ox1->data = pe;  // Beginn datenblock korrigieren




  typ = cvi->typ;
  // printf(",,,typ=%d\n",typ);



  //=========================================================
  if(typ == Typ_PT) {

    pta = cvi->data;

    UT3D_pt_traptm3 ((Point*)pe, trmat, pta);



  //=========================================================
  } else if(typ == Typ_LN) {

    UTO_obj_tra_m3 (pe, Typ_LN, cvi->data, trmat);




  //=========================================================
  } else if(typ == Typ_CI) {

    UTO_obj_tra_m3 (pe, Typ_CI, cvi->data, trmat);






  //=========================================================
  } else if(typ == Typ_GTXT) {

    // UTO_obj_tra_m3 (pe, Typ_GTXT, cvi->data, trmat);

    tx1 = cvi->data;

    memcpy(pe, tx1, sizeof(GText));

    tx1 = (GText*)pe;
    UT3D_pt_traptm3 ((Point*)pe, trmat, &tx1->pt);




  //=========================================================
  } else if(typ == Typ_Dimen) {

    dim1 = cvi->data;

    memcpy(pe, dim1, sizeof(Dimen));

    UT2D_pt_traptm3 (&((Dimen*)pe)->p1, trmat, &dim1->p1);
    UT2D_pt_traptm3 (&((Dimen*)pe)->p2, trmat, &dim1->p2);
    UT2D_pt_traptm3 (&((Dimen*)pe)->p3, trmat, &dim1->p3);




  //=========================================================
  } else if(typ == Typ_CVPSP3) {     //see psp_psp3_tra_m3
    pola = cvi->data;

    UT3D_pt_traptm3 (&pt0, trmat, (Point*)&UT3D_PT_NUL);

    for(i1=0; i1<cvi->siz; ++i1) {
      pol1 = pola[i1];

      pt1.x = pol1.x.a;
      pt1.y = pol1.y.a;
      pt1.z = pol1.z.a;
      UT3D_pt_traptm3 (&pt1, trmat, &pt1);
      pol1.x.a = pt1.x;
      pol1.y.a = pt1.y;
      pol1.z.a = pt1.z;

      pt1.x = pol1.x.b;
      pt1.y = pol1.y.b;
      pt1.z = pol1.z.b;
      UT3D_pt_traptm3 (&pt1, trmat, &pt1);
      pol1.x.b = pt1.x - pt0.x;
      pol1.y.b = pt1.y - pt0.y;
      pol1.z.b = pt1.z - pt0.z;

      pt1.x = pol1.x.c;
      pt1.y = pol1.y.c;
      pt1.z = pol1.z.c;
      UT3D_pt_traptm3 (&pt1, trmat, &pt1);
      pol1.x.c = pt1.x - pt0.x;
      pol1.y.c = pt1.y - pt0.y;
      pol1.z.c = pt1.z - pt0.z;

      pt1.x = pol1.x.d;
      pt1.y = pol1.y.d;
      pt1.z = pol1.z.d;
      UT3D_pt_traptm3 (&pt1, trmat, &pt1);
      pol1.x.d = pt1.x - pt0.x;
      pol1.y.d = pt1.y - pt0.y;
      pol1.z.d = pt1.z - pt0.z;

      memcpy(pe, &pol1, sizeof(polynom_d3));
      pe += sizeof (polynom_d3);
    }




  //=========================================================
  } else if(typ == Typ_CVBSP) {
    cvbi = cvi->data;
    cvbo = (CurvBSpl*)pe;

    memcpy(pe, cvbi, sizeof(CurvBSpl)); // save ptNr, deg ..
    pe += sizeof (CurvBSpl);

    cvbo->kvTab = (double*)pe;  // repair kvTab-adress

    l1 = sizeof(double) * (cvbi->ptNr + cvbi->deg + 1);
    memcpy(pe, cvbi->kvTab, l1);          // save knotvalues
    pe += l1;

    //cvbo->cpTab = pe;  // repair cpTab-adress
    cvbo->cpTab = (Point*)pe;  // repair cpTab-adress

    for(i1=0; i1<cvbi->ptNr; ++i1) {               // transform & save points
      UT3D_pt_traptm3 ((Point*)pe, trmat, &cvbi->cpTab[i1]);
      pe += sizeof (Point);
    }





  //=========================================================
  } else if(typ == Typ_CVPOL) {
    if(cvi->form != Typ_PT) goto L_err_form;
    pta = cvi->data;
    for(i1=0; i1<cvi->siz; ++i1) {
      UT3D_pt_traptm3 ((Point*)pe, trmat, &pta[i1]);
      pe += sizeof (Point);
    }



  //=========================================================
  } else if(typ == Typ_CVPOL2) {
    ox1->typ  = Typ_CVPOL;     // Type aendern (2D -> 3D) !!
    ox1->form = Typ_PT;
    pt2a = cvi->data;
    for(i1=0; i1<cvi->siz; ++i1) {
      pt1 = UT3D_pt_pt2 (&pt2a[i1]);
      UT3D_pt_traptm3 ((Point*)pe, trmat, &pt1);
      pe += sizeof (Point);
    }



  //=========================================================
  } else if(typ == Typ_CVCCV2) {
    *cvo = cvi;



  //=========================================================
  } else {
    TX_Error("UTO_ox_tra E001 typ=%d",typ);
    *cvo = cvi;
    return -1;
  }



  return 0;



  L_err_form:
    TX_Error("UTO_ox_tra E002 typ=%d form=%d",typ,cvi->form);
    *cvo = cvi;
    return -1;

}
*/
/* UNUSED; use UPRJ_app__
//=============================================================================
  int UTO_obj_appprj (int typo,void *objo,int typi,void *obji,Memspc *wrkSpc) {
//=============================================================================
/// INPUT:
///   typi = Typ_FuncInit:  init projection;
///          obji = vector for projectionAlongVector   OR
///          obji = NULL: normal projection.
///          typo = Typ of surface where to project;
///          objo = surface where to project;
///   typi = typ of obj to project
///   obji = obj to project
/// OUTPUT:
///   typo = typ of projected obj
///   objo = resulting obj, typ = typo


///        Typ_FuncInit1: init projection along vector; obji=Plane, objo=vector.


static int    prjMode;   // 0=normalProjection; 1=projectionAlongVector
static int    surfTyp;
static Plane  plm;
static Vector vcm;

  int    i1, i2, pNr, iClo;
  double d1, *da;
  Point  pt1, *pa1, *pa2;


  printf("UTO_obj_appprj %d\n",typi);



  //=== INIT ===========
  if(typi == TYP_FuncInit) { 
    surfTyp = typo;
    plm = *(Plane*)objo;

    prjMode = 0;
    if(obji) {
      vcm = *(Vector*)obji;
      prjMode = 1;
    }

    return 0;
  }




  //=== WORK ===========
  switch (typi) {


    //================================================================
    case Typ_PT:
 
      switch (surfTyp) {
        case Typ_PLN:
          if(prjMode == 0) {  // 0=normalProjection;
            UT3D_pt_intptvcpl_ (objo, &plm, obji, &plm.vz);
          } else {            // 1=projectionAlongVector
            UT3D_pt_intptvcpl_ (objo, &plm, obji, &vcm);
          }
          return 0;
        default:
          goto Err_typSur;
      }


    //================================================================
    case Typ_LN:
 
      switch (surfTyp) {
        case Typ_PLN:
          if(prjMode == 0) {  // 0=normalProjection;
            UT3D_pt_intptvcpl_ (&((Line*)objo)->p1, &plm, &((Line*)obji)->p1,
                                &plm.vz);
            UT3D_pt_intptvcpl_ (&((Line*)objo)->p2, &plm, &((Line*)obji)->p2,
                                &plm.vz);
          } else {            // 1=projectionAlongVector
            UT3D_pt_intptvcpl_ (&((Line*)objo)->p1, &plm, &((Line*)obji)->p1,
                                &vcm);
            UT3D_pt_intptvcpl_ (&((Line*)objo)->p2, &plm, &((Line*)obji)->p2,
                                &vcm);
          }
          return 0;
        default:
          goto Err_typSur;
      }


    //================================================================
    case Typ_CI:  // typo muss sein ellipse !!
 
      switch (surfTyp) {
        case Typ_PLN:
          if(prjMode == 0) {  // 0=normalProjection;
            // Ellipse = project circ -> plane
            UT3D_el_projcipl (objo, &plm, obji);
          } else {            // 1=projectionAlongVector
          }
          return 0;
        default:
          goto Err_typSur;
      }


    //================================================================
    case Typ_CVELL:
 
      switch (surfTyp) {
        case Typ_PLN:
          if(prjMode == 0) {  // 0=normalProjection;
            // Ellipse = project circ -> plane
            UT3D_el_projelpl (objo, &plm, obji);
          } else {            // 1=projectionAlongVector
          }
          return 0;
        default:
          goto Err_typSur;
      }


    //================================================================
    case Typ_CVPOL:
 
      switch (surfTyp) {
        case Typ_PLN:
          if(prjMode == 0) {  // 0=normalProjection;
              UT3D_stru_dump(Typ_CVPOL, obji, "plg-prj-in:");
            // *(CurvPoly*)objo = *(CurvPoly*)obji;   // copy ptNr, v0, v1, *lvTab
            pNr = ((CurvPoly*)obji)->ptNr;
            ((CurvPoly*)objo)->ptNr = pNr;

            // Punktetabelle cpTab neu erstellen
            // reserve Space for pNr points in wrkSpc
            pa1 = ((CurvPoly*)obji)->cpTab;
              // GR_Disp_pTab (pNr, pa1, SYM_STAR_S, 2);
            pa2 = wrkSpc->next;
            if(UME_add (wrkSpc, pNr * sizeof(Point)) < 0) return -1;
            for(i1=0; i1<pNr; ++i1) {
              UT3D_pt_projptpl (&pa2[i1], &plm, &pa1[i1]);
                // GR_Disp_pt (&pa2[i1], SYM_TRI_S, 2);
            }
            ((CurvPoly*)objo)->cpTab = pa2;

            // die LaengenTabelle lvTab muss auch neu erstellt werden
            da = wrkSpc->next;
            if(UME_add (wrkSpc, pNr * sizeof(double)) < 0) return -1;
            d1 = UT3D_plg_lvTab (da, pa2, pNr);
            ((CurvPoly*)objo)->lvTab = da;
            ((CurvPoly*)objo)->v0 = 0.;
            ((CurvPoly*)objo)->v1 = ((CurvPoly*)objo)->lvTab[pNr-1];
              // UT3D_stru_dump(Typ_CVPOL, objo, "plg-prj-tmp:");

            // check if closed;
            // iClo = UT3D_ck_plgclo((CurvPoly*)obji);   // 0=yes,closed

            // Start- und Endpunkt muessen neu errechnet werden !
            // dazu sind die Punkte zu transformieren und dann v0/v1 neu berechenen;
            // ausser v0/v1 sind Start/Endpunkt !
            i1 = UT3D_ck_plgparlim (obji, ((CurvPoly*)obji)->v0);
            if(i1 == 1) d1 = ((CurvPoly*)objo)->lvTab[0];
            else if(i1 == 2) d1 = ((CurvPoly*)objo)->lvTab[pNr-1];
            else {
              UT3D_pt_evalplg (&pt1, (CurvPoly*)obji, ((CurvPoly*)obji)->v0);
              UT3D_pt_projptpl (&pt1, &plm, &pt1);
              UT3D_parplg_plgpt (&d1, &pt1, (CurvPoly*)objo);
            }
            ((CurvPoly*)objo)->v0 = d1;

            // check if closed ACHTUNG: UT3D_pt_evalplg wuerde den Start=EndPt liefern
            // if(iClo < 0) {   // 0=yes,closed
            i1 = UT3D_ck_plgparlim (obji, ((CurvPoly*)obji)->v1);
            if(i1 == 1) d1 = ((CurvPoly*)objo)->lvTab[0];
            else if(i1 == 2) d1 = ((CurvPoly*)objo)->lvTab[pNr-1];
            else {
              UT3D_pt_evalplg (&pt1, (CurvPoly*)obji, ((CurvPoly*)obji)->v1);
              UT3D_pt_projptpl (&pt1, &plm, &pt1);
              UT3D_parplg_plgpt (&d1, &pt1, (CurvPoly*)objo);
            }
            ((CurvPoly*)objo)->v1 = d1;

              UT3D_stru_dump(Typ_CVPOL, objo, "plg-prj-out:");

          } else {            // 1=projectionAlongVector
          }
          return 0;
        default:
          goto Err_typSur;
      }


    //================================================================
    case Typ_CVBSP:
 
      switch (surfTyp) {
        case Typ_PLN:
          if(prjMode == 0) {  // 0=normalProjection;
            // copy BSP (ptnr, deg, v0, v1, *kvTab)
            *(CurvBSpl*)objo = *(CurvBSpl*)obji;
            i2 = ((CurvBSpl*)obji)->ptNr;
            // Controlpoints-from
            pa1 = ((CurvBSpl*)obji)->cpTab;
              // GR_Disp_pTab (i2, pa1, SYM_STAR_S, 2);
            // Controlpoints-to
            pa2 = wrkSpc->next;
            // reserve Space for i2 points in wrkSpc
            if(UME_add (wrkSpc, i2 * sizeof(Point)) < 0) return -1;
            for(i1=0; i1<i2; ++i1) {
              UT3D_pt_projptpl (&pa2[i1], &plm, &pa1[i1]);
                // GR_Disp_pt (&pa2[i1], SYM_TRI_S, 2);
            }
            ((CurvBSpl*)objo)->cpTab = pa2;
          } else {            // 1=projectionAlongVector
          }
          return 0;
        default:
          goto Err_typSur;
      }


    //================================================================
    default:
      goto Err_typo;
  }


  return 0;


Err_typo:
  TX_Error("UTO_obj_appprj E001 - %d",typo);
  return -1;


Err_typSur:
  TX_Error("UTO_obj_appprj E002 %d",typo);
  return -1;


}
*/


//================================================================
  int UTO_pln_mir (Plane *plno, Plane *plni, ObjBin *objMir) {
//================================================================

   if(objMir->typ == Typ_PLN) {
    return UT3D_pl_mirpl (plno, plni, (Plane*)objMir->obj);
  } 
    
  return UT3D_pl_mirln (plno, plni, (Line*)objMir->obj);

}   
    

//================================================================
  int UTO_pt_mir (Point *pto, Point *pti, ObjBin *objMir) {
//================================================================

  
  if(objMir->typ == Typ_PLN) {
    return UT3D_pt_mirptpl (pto, pti, (Plane*)objMir->obj);
  }

  return UT3D_pt_mirptln (pto, pti, (Line*)objMir->obj);
 
}


//================================================================
  int UTO_vc_mir (Vector *vco, Vector *vci, ObjBin *objMir) {
//================================================================


  if(objMir->typ == Typ_PLN) {
    return UT3D_vc_mirvcpl (vco, vci, (Plane*)objMir->obj);
  } 


  return UT3D_vc_mirvcln (vco, vci, (Line*)objMir->obj);


}



//=======================================================================
  int UTO_obj_appmir (void *objo, int otyp, void *obji, Memspc *wrkSpc) {
//=======================================================================
/// mirror

// static Plane plm;
static ObjBin  objMir;


  int    i1, i2;
  Point  *pa1, *pa2;


  printf("UTO_obj_appmir %d\n",otyp);
  UT3D_stru_dump (otyp, obji, "  obji:");




  //=== INIT ===========
  if(objo == NULL) {
    // plm = *(Plane*)obji;
    objMir.typ = otyp;
    memcpy ((void*)objMir.obj, obji, UTO_siz_stru(otyp));

    return 0;

  }




  //=== WORK ===========
  switch (otyp) {


    //================================================================
    case Typ_PT:
      UTO_pt_mir (objo, obji, &objMir);
      break;


    //================================================================
    case Typ_LN:
      UTO_pt_mir (&((Line*)objo)->p1, &((Line*)obji)->p1, &objMir);
      UTO_pt_mir (&((Line*)objo)->p2, &((Line*)obji)->p2, &objMir);
      break;


    //================================================================
    case Typ_CI:
      if(objMir.typ == Typ_PLN) {
        ((Circ*)objo)->rad  = -((Circ*)obji)->rad;
        ((Circ*)objo)->ango = -((Circ*)obji)->ango;
      } else {
        ((Circ*)objo)->rad  = ((Circ*)obji)->rad;
        ((Circ*)objo)->ango = ((Circ*)obji)->ango;
      }
      UTO_pt_mir (&((Circ*)objo)->pc, &((Circ*)obji)->pc, &objMir);
      UTO_pt_mir (&((Circ*)objo)->p1, &((Circ*)obji)->p1, &objMir);
      if(UT3D_ck_ci360((Circ*)obji) == 0)
        ((Circ*)objo)->p2 = ((Circ*)objo)->p1;
      else
        UTO_pt_mir (&((Circ*)objo)->p2, &((Circ*)obji)->p2, &objMir);
      // vz spiegeln
      UTO_vc_mir (&((Circ*)objo)->vz, &((Circ*)obji)->vz, &objMir);
      break;


    //================================================================
    case Typ_CVELL:   // mirr elli  p1, p2, pc; vz, va, vb; dir;
      ((CurvElli*)objo)->dir = ((CurvElli*)obji)->dir;
      UTO_pt_mir (&((CurvElli*)objo)->pc, &((CurvElli*)obji)->pc, &objMir);
      UTO_pt_mir (&((CurvElli*)objo)->p1, &((CurvElli*)obji)->p1, &objMir);
      if(UT3D_ck_el360((CurvElli*)obji) == 0)
        ((CurvElli*)objo)->p2 = ((CurvElli*)objo)->p1;
      else
        UTO_pt_mir (&((CurvElli*)objo)->p2, &((CurvElli*)obji)->p2, &objMir);
      // vz spiegeln
      UTO_vc_mir (&((CurvElli*)objo)->vz, &((CurvElli*)obji)->vz, &objMir);
      UTO_vc_mir (&((CurvElli*)objo)->va, &((CurvElli*)obji)->va, &objMir);
      UTO_vc_mir (&((CurvElli*)objo)->vb, &((CurvElli*)obji)->vb, &objMir);
      break;


    //================================================================
    case Typ_CVPOL:
      *(CurvPoly*)objo = *(CurvPoly*)obji;   // copy ptNr, v0, v1, *lvTab
      i2 = ((CurvPoly*)obji)->ptNr;
      // reserve Space for i2 points in wrkSpc
      pa1 = ((CurvPoly*)obji)->cpTab;
        // GR_Disp_pTab (i2, pa1, SYM_STAR_S, 2);
      pa2 = wrkSpc->next;
      if(UME_add (wrkSpc, i2 * sizeof(Point)) < 0) return -1;
      for(i1=0; i1<i2; ++i1) {
        UTO_pt_mir (&pa2[i1], &pa1[i1], &objMir);
          // GR_Disp_pt (&pa2[i1], SYM_TRI_S, 2);
      }
      ((CurvPoly*)objo)->cpTab = pa2;
      break;


    //================================================================
    case Typ_CVBSP:
      // copy BSP (ptnr, deg, v0, v1, *kvTab)
      *(CurvBSpl*)objo = *(CurvBSpl*)obji;
      i2 = ((CurvBSpl*)obji)->ptNr;
      // Controlpoints-from
      pa1 = ((CurvBSpl*)obji)->cpTab;
        // GR_Disp_pTab (i2, pa1, SYM_STAR_S, 2);
      // Controlpoints-to
      pa2 = wrkSpc->next;
      // reserve Space for i2 points in wrkSpc
      if(UME_add (wrkSpc, i2 * sizeof(Point)) < 0) return -1;
      for(i1=0; i1<i2; ++i1) {
        UTO_pt_mir (&pa2[i1], &pa1[i1], &objMir);
          // GR_Disp_pt (&pa2[i1], SYM_TRI_S, 2);
      }
      ((CurvBSpl*)objo)->cpTab = pa2;
      break;


    //================================================================
    case Typ_PLN:
      UTO_pln_mir (objo, obji, &objMir);
      break;


    //================================================================
    case Typ_Model:
      // copy model
      *(ModelRef*)objo = *(ModelRef*)obji;
      // mirror origin
      UTO_pt_mir (&((ModelRef*)objo)->po, &((ModelRef*)obji)->po, &objMir);
      // mirror vectors
      UTO_vc_mir (&((ModelRef*)objo)->vx, &((ModelRef*)obji)->vx, &objMir);
      UTO_vc_mir (&((ModelRef*)objo)->vz, &((ModelRef*)obji)->vz, &objMir);

      break;


    //================================================================
    default:
      TX_Error("UTO_obj_appmir E001 %d",otyp);
      return -1;
  }


    UT3D_stru_dump (otyp, objo, "ex UTO_obj_appmir:");

  return 0;

}


/*
//================================================================
  int UTO_obj_trim (ObjGX *oo, ObjGX *oi, ObjGX *oc1, int i1Nr,
                    ObjGX *oc2, int i2Nr, Memspc *wrkSpc) {
//================================================================
/// \code
/// trim obj with 1 or 2 cutting-objects;
/// cutting-objects Parameter fuer typ=Typ_Par1; form=Typ_Float8:
///   Typ=Typ_Val definiert Knotenwert fuer BSP's;
///   Typ=Typ_Par1 nur von 0-1 (bei BSP's Umrechnung m. UT3D_parbsp_par1)
/// RetCod:
///   1-n  = nr of possible solutions ..
/// 
/// Nur mit einem Obj begrenzen: oc2.typ=Typ_Error.
/// 
/// Input:
///   oi                 - obj to cut!
///   oc1                - 1. cuttingObj!
///   oc2                - 2. cuttingObj
/// ?
/// modFlag1/2:       ob eine bestimmte IntersectionNr definiert ist (1)
///                   oder nicht (0).
/// imod1/2           die definierte IntersectionNr (i1Nr/i2Nr); modFlagx=1
/// \endcode



  int    irc, i1, ii, inxt, o0Typ, o1Typ, o2Typ, pNr, oSiz,
         modFlag1, imod1, modFlag2, imod2, io2, cvTyp;
  // long   o1i, o2i;
  double va[20], v1,  v2, vi1, vi2;
  char   cbuf[32];
  void   *obj0, *obj1, *obj2, *oxo;
  Point  pa[20], pt1, pt2, pti1, pti2;



  // printf("UTO_obj_trim %d %d\n",oi->typ,oi->form);
  // printf("   %d %d %d\n",oc1->typ,oc1->form, i1Nr);
  // printf("   %d %d %d\n",oc2->typ,oc2->form, i2Nr);



  // get obj to cut
  o0Typ = UTO_obj_getp (&obj0, oi);
  if(o0Typ < 0) return -1;

  // get cutting obj 1
  o1Typ = UTO_obj_getp (&obj1, oc1);
  if(o1Typ < 0) return -1;

  // get cutting obj 2
  o2Typ = oc2->typ;
  if(o2Typ != Typ_Error) {
    o2Typ = UTO_obj_getp (&obj2, oc2);
    if(o2Typ < 0) return -1;
  }



  //----------------------------------------------------------------
  // copy obj to cut out
  oo->typ   = o0Typ;
  oo->form  = o0Typ;
  oo->data  = wrkSpc->next;
  oo->siz   = 1;
  oxo = oo->data;
  oSiz = UME_ck_free (wrkSpc);
  irc = UTO_copy_stru (oxo, &oSiz, o0Typ, obj0, 1);
  if(irc < 0) return irc;
  UME_add (wrkSpc, irc);          // irc=size (UTO_siz_stru(o0Typ))


  // check if CurveLimits of outObj shall be Points or parameters
  cvTyp = UTO_ck_curvLimTyp (o0Typ);  // 0=points, 1=Parameters

  // get limits of obj to cut -> pt1/pt2 or v1/v2
  irc = UTO_2pt_limstru (&pt1, &pt2, &v1, &v2, o0Typ, obj0);
  if(irc < 0) return irc;
    // GR_Disp_pt (&pe1, SYM_TRI_S, 2);
    // GR_Disp_pt (&pe2, SYM_TRI_S, 2);



  //----------------------------------------------------------------
  // intersect obj0 with 1. cutting obj obj1
  pNr = 20; // maxNr (size of pa & va)
  irc = UTO_stru_int (&pNr, pa, va, o0Typ, obj0, o1Typ, obj1, wrkSpc);
  if(irc < 0) goto GeomErr;
    // GR_Disp_pTab (pNr, pa, SYM_STAR_S, 2);


  // imod korrigieren; index > pa/va bestimmen
  if(i1Nr > 0) APT_trim_u0 (&ii, &i1Nr, pNr);
  else   { ii = 0;}

  pti1 = pa[ii];
  vi1  = va[ii];


  // nur 1 cut-obj: weiter ..
  if(o2Typ == Typ_Error) goto L_cut1;


  // Sind die beiden cuttingObjects ident ? (zb CUT C20 L20 L20)
  // if(o1Typ == Typ_Val) goto L_int_o2_1;
  if(obj1 != obj2) goto L_int_o2_1;        // Adressvergleich !
  // if(o1Typ != o2Typ) goto L_int_o2_1;
  // // if(o1i != o2i) goto L_int_o2_1;

  // hier muesste man auch Typ_VAL checken !
  if(i2Nr == 0) { // use den 2. IntersectionPunkt
    ii = 1;
    goto L_int_o2_3;
  }
  // select entspr. modFlag2
  goto L_int_o2_2;




  // intersect obj0 with 2. cutting obj obj2
  L_int_o2_1:
  pNr = 20; // maxNr
  irc = UTO_stru_int (&pNr, pa, va, o0Typ, obj0, o2Typ, obj2, wrkSpc);
  if(irc < 0) goto GeomErr;
    // GR_Disp_pTab (pNr, pa, SYM_STAR_S, 2);


  // imod korrigieren; index > pa/va bestimmen
  L_int_o2_2:
  if(i2Nr > 0) APT_trim_u0 (&ii, &imod2, pNr);
  else   { ii = 0;}


  L_int_o2_3:
  pti2 = pa[ii];
  vi2  = va[ii];

  goto L_cut2;




  //----------------------------------------------------------------
  // nur 1 CuttingObj:
  // pa1[ii] ist der eine Punkt; der andere entsprechend imod1.
  L_cut1:


  // Circ u Ellipse: mit Startpunkt halbieren, wenn 360-Grad.
  if(o0Typ == Typ_CI) {
    if(UT3D_ck_ci360((Circ*)oxo) == YES) {
      // UT3D_ci_cipt180 ((Circ*)oxo, &pti1);
      UT3D_pt_opp2pt (&pt1, &((Circ*)oxo)->pc, &pti1);
      pt2 = pt1;
    }

  } else if(o0Typ == Typ_CVELL) {
    if(UT3D_ck_el360((CurvElli*)oxo) == YES) {
      // UT3D_ci_cipt180 ((Circ*)oxo, &pti1);
      UT3D_pt_opp2pt (&pt1, &((CurvElli*)oxo)->pc, &pti1);
      pt2 = pt1;
    }
  }


  // trim----------
  if(i1Nr == 0)  {                // Default; replace pt1, keep pt2
    // pti1-pt2 duerfen nicht gleich sein; else ..
    pt1 = pti1;
    v1 = vi1;

  } else {          // andere Seite: keep pt1, replace pt2
    // pti1-pt1 duerfen nicht gleich sein; else ..
    pt2 = pti1;
    v2 = vi1;
  }

  goto L_out;




  //----------------------------------------------------------------
  // 2 CuttingObjs:
  L_cut2:

  // Circ u Ellipse: entspr imod umdrehen
  if(o0Typ == Typ_CI) {
    if(i1Nr > 0) UT3D_ci_inv2 ((Circ*)oxo);

  } else if(o0Typ == Typ_CVELL) {
    if(i1Nr > 0) UT3D_el_inv2 ((CurvElli*)oxo);
  }



    // replace pt1 & pt2
    pt1 = pti1;
    pt2 = pti2;

    v1 = vi1;
    v2 = vi2;






  //----------------------------------------------------------------
  // create outputObj and replace points
  L_out:

  // oOut = set endpoints or parameters;
  // LN,AC,CVELL: p1, p2 setzen;
  // CVBSP,CVPOL: v1, v2 setzen.
  UTO_set_ptlim (o0Typ, oxo, &pt1, &v1, &pt2, &v2);



  // Testausgaben:
    // UT3D_stru_dump (o0Typ, oxo, "ex UTO_obj_trim\n");
    // UTO_Disp_ox (oo, wrkSpc, 9);




  return pNr;



  GeomErr:
  TX_Error("UTO_obj_trim: Trim: - Geometry Error");
  return -1;

  ParErr:
  TX_Error("UTO_obj_trim: Trim: - Parameter Error");
  return -1;

  // ObjErr:
  // APED_oid_dbo__ (cbuf, i1, o1i);
  // TX_Error("UTO_obj_trim: Obj %s not defined Error",cbuf);
  // return -1;

  NotImp_err:
  TX_Error("UTO_obj_trim: Trim: - not yet implemented Error");
  return -1;


}
*/

//================================================================
  int UTO_trim_2par1 (void *objo, void *obji, int typ,
                      double v0, double v1, Memspc *memSeg) {
//================================================================
/// \code
/// trim (modify) obj from dataStruct by 2 parameters
/// Input:
///   obji       dataStruct in
///   typ        type of dataStruct
///   v0, v1     parameters 0-1
/// Output:
///   objo       modified dataStruct out.
///   oTyp       type of outputObj; ed Typ_LN|..
///   objo       dataStruct of outputObj; maxSiz OBJ_SIZ_MAX.
///   RetCod:
/// 
/// see UT3D_pt_evparcrv
/// see UTO_stru_int
/// see UTO_CUT__
/// \endcode


  printf("UTO_trim_2par1 %d %f %f\n",typ,v0,v1);
  // UT3D_stru_dump (typ, obji, "");
  // GR_Disp_ost (obji, typ, NULL, 9);



  
  switch (typ) {

    //----------------------------------------------------------------
    case Typ_LN:
      UT3D_pt_evparln (&((Line*)objo)->p1, v0, obji);
      UT3D_pt_evparln (&((Line*)objo)->p2, v1, obji);

      break;


    //----------------------------------------------------------------
    case Typ_CI:
      // copy the circ
      memcpy (objo, obji, sizeof(Circ));
      UT3D_pt_evparci (&((Circ*)objo)->p1, v0, obji);
      UT3D_pt_evparci (&((Circ*)objo)->p2, v1, obji);
      UT3D_ci_setangr (objo);

      break;


    //----------------------------------------------------------------
    case Typ_CVBSP:

      memcpy (objo, obji, sizeof(CurvBSpl));
      ((CurvBSpl*)objo)->v0 = v0;
      ((CurvBSpl*)objo)->v1 = v1;
      // // change par 0-1  -->  knotVal
      // lpar = UT3D_parbsp_par1 (lpar, data);
      // UT3D_pt_evparCrvBSpl (pto, data, lpar);      // see UT3D_pt_evalparCv
      break;




    //----------------------------------------------------------------
    default:
      TX_Error("UTO_trim_2par1 E001 %d",typ);
      return -1;
  }



  //----------------------------------------------------------------
    // UT3D_stru_dump (typ, obji, "obji:\n");
    // UT3D_stru_dump (typ, objo, "objo:\n");
    // GR_Disp_ost (objo, typ, memSeg, 9);


  return 0;

}

/*
//=======================================================================
  int UTO_obj_apptra (void *objo, int otyp, void *obji, Memspc *wrkSpc) {
//=======================================================================
/// \code
/// init Translation: UTO_obj_apptra (NULL, Typ_VC, &translVector);
/// init Rotation:    UTO_obj_apptra (NULL, Typ_TraRot, (TraRot*)&TrRorStru);
///                     (TrRorStru: Matrix muss mit UT3D_m3_inirot geladen sein)
/// see UTO_obj_tra_m3 oder UTO_ox_tra
/// 
/// Input:  obji immer eine struct (auch bei Typ_CV !)
/// Output: objo immer eine struct (auch bei Typ_CV !)
/// wrkSpc wird nur fuer Curves benutzt (kann bei PT/LN/AC NULL sein)
/// \endcode


static int traAct;

  int    irc, i1, i2;
  Point  *pa1, *pa2;
  void   *o1;


  //=== INIT ===========
  if(objo == NULL) {

    if(otyp == Typ_VC) {
      traAct = Typ_VC;
      UTO_pt_tr = UTO_pt_tratra;
      *(Vector*)TR_obj = *(Vector*)obji;

    } else if(otyp == Typ_TraRot) {
      traAct = Typ_TraRot;
      UTO_pt_tr = UTO_pt_trarot;
      *(TraRot*)TR_obj = *(TraRot*)obji;

    } else {
      TX_Error("UTO_obj_apptra E001 %d",otyp);
      return -1;
    }

    return 0;

  }




  //=== WORK ===========
  switch (otyp) {


    //================================================================
    case Typ_PT:
      irc = (*UTO_pt_tr)(objo, obji);
      break;


    //================================================================
    case Typ_LN:
      irc = (*UTO_pt_tr)(&((Line*)objo)->p1, &((Line*)obji)->p1);
      irc = (*UTO_pt_tr)(&((Line*)objo)->p2, &((Line*)obji)->p2);
      break;


    //================================================================
    case Typ_CI:
      *(Circ*)objo = *(Circ*)obji;   // copy vz & rad & ango
      irc = (*UTO_pt_tr)(&((Circ*)objo)->pc, &((Circ*)obji)->pc);
      irc = (*UTO_pt_tr)(&((Circ*)objo)->p1, &((Circ*)obji)->p1);
      if(UT3D_ck_ci360((Circ*)obji) == 0)
        ((Circ*)objo)->p2 = ((Circ*)objo)->p1;
      else  (*UTO_pt_tr)(&((Circ*)objo)->p2, &((Circ*)obji)->p2);

      if(traAct == Typ_TraRot)   // nur Rotate:
        UT3D_vc_travcm3 (&((Circ*)objo)->vz,
                         ((TraRot*)TR_obj)->ma,
                         &((Circ*)obji)->vz);
      break;


    //================================================================
    case Typ_CVELL:
      // copy Elli
      *(CurvElli*)objo = *(CurvElli*)obji;

      // transf elli
      *(CurvElli*)objo = *(CurvElli*)obji;   // copy vz & rad & ango
      irc = (*UTO_pt_tr)(&((CurvElli*)objo)->pc, &((CurvElli*)obji)->pc);
      irc = (*UTO_pt_tr)(&((CurvElli*)objo)->p1, &((CurvElli*)obji)->p1);
      if(UT3D_ck_el360((CurvElli*)obji) == 0)
        ((CurvElli*)objo)->p2 = ((CurvElli*)objo)->p1;
      else  (*UTO_pt_tr)(&((CurvElli*)objo)->p2, &((CurvElli*)obji)->p2);

      if(traAct == Typ_TraRot) { // nur Rotate:
        UT3D_vc_travcm3 (&((CurvElli*)objo)->va,
                         ((TraRot*)TR_obj)->ma,
                         &((CurvElli*)obji)->va);
        UT3D_vc_travcm3 (&((CurvElli*)objo)->vb,
                         ((TraRot*)TR_obj)->ma,
                         &((CurvElli*)obji)->vb);
        UT3D_vc_travcm3 (&((CurvElli*)objo)->vz,
                         ((TraRot*)TR_obj)->ma,
                         &((CurvElli*)obji)->vz);
      }
      break;


    //================================================================
    case Typ_CVPOL:
      *(CurvPoly*)objo = *(CurvPoly*)obji;   // copy ptNr, v0, v1, *lvTab
      i2 = ((CurvPoly*)obji)->ptNr;
      // reserve Space for i2 points in wrkSpc
      pa1 = ((CurvPoly*)obji)->cpTab;
        // GR_Disp_pTab (i2, pa1, SYM_STAR_S, 2);
      pa2 = wrkSpc->next;
      if(UME_add (wrkSpc, i2 * sizeof(Point)) < 0) return -1;
      for(i1=0; i1<i2; ++i1) {
        (*UTO_pt_tr)(&pa2[i1], &pa1[i1]);
          // GR_Disp_pt (&pa2[i1], SYM_TRI_S, 2);
      }
      ((CurvPoly*)objo)->cpTab = pa2;
      break;



    //================================================================
    case Typ_CVBSP:
      // copy BSP (ptnr, deg, v0, v1, *kvTab)
      *(CurvBSpl*)objo = *(CurvBSpl*)obji;
      i2 = ((CurvBSpl*)obji)->ptNr;
      // Controlpoints-from
      pa1 = ((CurvBSpl*)obji)->cpTab;
        // GR_Disp_pTab (i2, pa1, SYM_STAR_S, 2);
      // Controlpoints-to
      pa2 = wrkSpc->next;
      // reserve Space for i2 points in wrkSpc
      if(UME_add (wrkSpc, i2 * sizeof(Point)) < 0) return -1;
      for(i1=0; i1<i2; ++i1) {
        (*UTO_pt_tr)(&pa2[i1], &pa1[i1]);
          // GR_Disp_pt (&pa2[i1], SYM_TRI_S, 2);
      }
      ((CurvBSpl*)objo)->cpTab = pa2;
      break;


    //================================================================

  } else if(typ == Typ_CVPSP3) {
    pola = cvi->data;

    UT3D_pt_traptm3 (&pt0, trmat, &UT3D_PT_NUL);

    for(i1=0; i1<cvi->siz; ++i1) {
      pol1 = pola[i1];

      pt1.x = pol1.x.a;
      pt1.y = pol1.y.a;
      pt1.z = pol1.z.a;
      UT3D_pt_traptm3 (&pt1, trmat, &pt1);
      pol1.x.a = pt1.x;
      pol1.y.a = pt1.y;
      pol1.z.a = pt1.z;

      pt1.x = pol1.x.b;
      pt1.y = pol1.y.b;
      pt1.z = pol1.z.b;
      UT3D_pt_traptm3 (&pt1, trmat, &pt1);
      pol1.x.b = pt1.x - pt0.x;
      pol1.y.b = pt1.y - pt0.y;
      pol1.z.b = pt1.z - pt0.z;

      pt1.x = pol1.x.c;
      pt1.y = pol1.y.c;
      pt1.z = pol1.z.c;
      UT3D_pt_traptm3 (&pt1, trmat, &pt1);
      pol1.x.c = pt1.x - pt0.x;
      pol1.y.c = pt1.y - pt0.y;
      pol1.z.c = pt1.z - pt0.z;

      pt1.x = pol1.x.d;
      pt1.y = pol1.y.d;
      pt1.z = pol1.z.d;
      UT3D_pt_traptm3 (&pt1, trmat, &pt1);
      pol1.x.d = pt1.x - pt0.x;
      pol1.y.d = pt1.y - pt0.y;
      pol1.z.d = pt1.z - pt0.z;

      memcpy(pe, &pol1, sizeof(polynom_d3));
      pe += sizeof (polynom_d3);
    }
//


    //================================================================
    default:
      TX_Error("UTO_obj_apptra E002 %d",otyp);
      return -1;
  }



  return 0;

}


//================================================================
  int UTO_pt_tratra (Point *po, Point *pi) {
//================================================================
/// Translate Point via UTO_obj_apptra

  UT3D_pt_traptvc (po, pi, (Vector*)TR_obj);
  return 0;

}


//================================================================
  int UTO_pt_trarot (Point *po, Point *pi) {
//================================================================
/// rotate Point via UTO_obj_apptra

  return UT3D_pt_rotptm3 (po, pi, ((TraRot*)TR_obj)->ma);

}


//=======================================================================
  ObjGX UTO_obj_load (int typ, int form, int size, void* data) {
//=======================================================================
/// \code
/// UTO_obj_load       make ObjGX from typ,form,siz,struct
///
/// see also OGX_SET_OBJ OGX_SET_INDEX OGX_GET_INT UTO_stru_2_obj
/// \endcode

  ObjGX  ox1;


  ox1.typ  = typ;
  ox1.form = form;
  ox1.siz  = size;
  ox1.data = data;


  return ox1;

}

//=======================================================================
  ObjGX UTO_stru_2_obj (int typ, int form, void* data) {
//=======================================================================
/// UTO_stru_2_obj       make ObjGX from typ+struct
///
/// see also OGX_SET_OBJ UTO_obj_load

  ObjGX  ox1;


  ox1.typ  = typ;
  ox1.form = form;
  ox1.siz  = 1;
  ox1.data = data;


  return ox1;

}
*/


/*
//=======================================================================
  int UTO_Memspc_init (Memspc *memSpc, void* objDat, int osiz) {
//=======================================================================

  memSpc->start = objDat;
  memSpc->next  = objDat;
  memSpc->end   = objDat + osiz;

  return 0;

}



//=======================================================================
  void* UTO_save__ (Memspc *memSpc, void* objDat, int osiz) {
//=======================================================================

  void *actPos;

  // printf("UTO_save__ %d %d\n",osiz,memSpc->next-memSpc->start);

  actPos = memSpc->next;

  if((memSpc->next + osiz) > memSpc->end) return NULL;

  memcpy(memSpc->next, objDat, osiz);

  memSpc->next += osiz;

  return actPos;

}
*/


//=======================================================================
  int UTO_copy_stru (void *oo, int *oSiz, int typ, void *ii, int iNr) {
//=======================================================================
/// \code
/// UTO_copy_stru           copy struct (no resolve - not recursive)
/// see OGX_ox_copy_obj DB_store_stru
/// Input:
///   oSiz  size of outSpace
///   ii    obj to copy
///   typ   type of struct ii
///   iNr   nr of objects of same type in ii
/// Output:
///   oSiz  remaining size of outSpace
///   oo    here is a copy of ii
/// 
/// RetCod: copied size
/// \endcode


  long siz1;


  siz1 = UTO_siz_stru (typ) * iNr;

  // printf("UTO_copy_stru typ=%d oSiz=%d siz1=%d\n",typ,*oSiz,siz1);


  // if(siz1 > maxSiz) ...

  if(siz1 > *oSiz) {
    TX_Error("UTO_copy_stru E001 %d\n",siz1);
    *oSiz = -1;
    return -1;
  }


  memcpy (oo, ii, siz1);

  *oSiz -= siz1;

  // printf("ex UTO_copy_stru typ=%d siz=%d\n",typ,siz1);
  return siz1;

}

/*
//================================================================
  int UTO_Disp_ox (ObjGX *ox1, int att) {
//================================================================
/// \code
/// Input:
///   ox1 
///   wrkSpc   necessary for TypCV, else NULL
///   att
/// 
/// see UTO_Draw_ox GR_Disp_ost GR_Disp_dbo GR_Draw_dbo
/// \endcode


  long dli;


  printf("UTO_Disp_ox typ=%d form=%d att=%d\n",ox1->typ,ox1->form,att);


  dli = DL_StoreObj (ox1->typ, -1L, att);
  
  return UTO_Draw_ox (&dli, att, ox1);

}


//==========================================================================
  int UTO_Draw_ox (long *ind, int att, ObjGX *ox1) {
//==========================================================================
/// \code
/// Input:
///   ind      DispListIndex; get it from DL_StoreObj
///   att      Points: 0=normal, black;  1=red (hilited);
///   ox1 
/// 
/// see UTO_Disp_ox GR_Disp_ost GR_Disp_dbo GR_Draw_dbo
///     APT_DrawCurv GR_DrawCurv
/// \endcode

// use *ind = -2 for temp.Obj (fixed temp index)



  int       oTyp, i1;
  long      dbi;
  void      *p1;
  ObjGX     *ox2, o2;
  Memspc    tSpc1;


  printf("UTO_Draw_ox ind=%ld att=%d typ=%d form=%d\n",*ind,att,
          ox1->typ,ox1->form);
  // UTO_dump__ (ox1, "UTO_Draw_ox");


  oTyp = ox1->form;


  switch (oTyp) {

    //================================================================
    case Typ_PT:
      GR_DrawPoint (ind, att, (Point*)ox1->data);
      break;


    //================================================================
    case Typ_LN:
      GR_DrawLine (ind, att, (Line*)ox1->data);
      break;


    //================================================================
    case Typ_CI:
      GR_DrawCirc (ind, att, (Circ*)ox1->data);
      break;


    //================================================================
    case Typ_CVELL:
      GR_DrawCvEll (ind, att, ox1->data);  // see GR_Disp_ell GR_DrawCvEll
      break;


    //================================================================
    case Typ_CVPOL:     // CurvPoly
      GR_DrawCvPol (ind, att, ox1->data); // GR_Disp_pol GR_DrawCvPol
      break;


    //================================================================
    case Typ_CVBSP:
      GR_DrawCvBSp (ind, att, ox1->data); // CurvBSpl
      break;


    //================================================================
    case Typ_CVCCV:
      UME_init (&tSpc1, memspc201, sizeof(memspc201));
      // GR_DrawCvCCV (ind, att, ox1->data, &tSpc1);
      GR_DrawCvCCV (ind, att, ox1, &tSpc1);
      break;



    //================================================================
    case Typ_SURRV:
      // TSU_DrawSurTRV sollte noch in Disp & Draw geteilt werden !
      TSU_DrawSurTRV (ox1, att, -1L);
      break;


    //================================================================
    case Typ_SURBSP:
      // nur Kontrollpunkte:
      // sbsp = (SurBSpl*)ox1->data;
      // GR_Disp_sbsp (sbsp->ptUNr, sbsp->ptVNr, sbsp->cpTab, att); // SurBSpl
      // TSU_DrawSurTBSP sollte noch in Disp & Draw geteilt werden !
      TSU_DrawSurTBSP ((SurBSpl*)ox1->data, att, -1L);
      break;


    //================================================================
    case Typ_Index:
      dbi = OGX_GET_INT (ox1);
      o2 = DB_GetObjGX (ox1->typ, dbi);
      UTO_Draw_ox (ind, att, &o2);
      break;


    //================================================================
    case Typ_ObjGX:
      i1 = AP_typ_2_bastyp (ox1->typ);

      //----------------------------------------------------------------
      if(i1 == Typ_SUR) {
        // SUR_Disp_Sur (ox1, wrkSpc, att, p1);

      //----------------------------------------------------------------
      } else {
        ox2 = ox1->data;
        for(i1=0; i1 < ox1->siz; ++i1) // Recursion !
          UTO_Draw_ox (ind, att, &ox2[i1]);
      }
      break;


    //================================================================
    default:
      goto L_E001;
  }

  // // save wrkSpc
  // if(wrkSpc) memstart = wrkSpc->next;
  // // release work space
  // if(wrkSpc) wrkSpc->next = memstart;

  return 0;


  L_E001:
      TX_Error("UTO_Draw_ox not yet impl: %d %d",ox1->typ,ox1->form);
      return -1;

  L_E002:
      TX_Error("UTO_Draw_ox E002"); // par fehlt ..
      return -1;

  // L_E003:
      // TX_Error("UTO_Draw_ox E003"); // wrkSpc fehlt ..
      // return -1;

}


//================================================================
  int UTO_Disp_ox (ObjGX *ox1, Memspc *wrkSpc, int att, ...) {
//================================================================
/// \code
/// wrkSpc:  nur fuer TypCV, sonst NULL
/// Zusatzparameter:
/// f Typ_PT: (int)PointTyp eg SYM_STAR_S
/// 
/// see GR_Disp_dbo GR_Disp_ost
/// \endcode


  int     oTyp, i1;
  va_list va;
  void    *p1, *memstart;
  ObjGX   *ox2;
  SurBSpl *sbsp;


  // UTO_dump__ (ox1, "UTO_Disp_ox");


  // die Adressen der Parameter holen ..
  va_start(va,att);
  p1 = va_arg(va,void*);
  va_end(va);


  // save wrkSpc
  if(wrkSpc) memstart = wrkSpc->next;
 

  oTyp = ox1->form;


  switch (oTyp) {

    // unsupported:
    // GR_Disp_tx
    // GR_Disp_vc

    // GR_Disp_pln



    //================================================================
    case Typ_PT:
      if(((int)p1 < 0)||((int)p1 > 256)) goto L_E002;
      printf(" p1=%d\n",p1);
      GR_Disp_pt (ox1->data, p1, att);
      break;


    //================================================================
    case Typ_LN:
      GR_Disp_cv (ox1->data, 2, att);
      break;


    //================================================================
    case Typ_CI:
      GR_Disp_ac (ox1->data, att);
      break;


    //================================================================
    case Typ_CVELL:
      GR_Disp_ell (ox1->data, att);
      break;


    //================================================================
    case Typ_CVPOL:
      GR_Disp_pol (ox1->data, att); // CurvPoly
      break;


    //================================================================
    case Typ_CVBSP:
      if(!wrkSpc) goto L_E003;
      GR_Disp_bsp (ox1->data, att); // CurvBSpl
      break;



    // //================================================================
    // case Typ_CVPSP3:



    //================================================================
    case Typ_SURRV:
      TSU_DrawSurTRV (ox1, att, -1L);
      break;



    //================================================================
    case Typ_SURBSP:
      // nur Kontrollpunkte:
      // sbsp = (SurBSpl*)ox1->data;
      // GR_Disp_sbsp (sbsp->ptUNr, sbsp->ptVNr, sbsp->cpTab, att); // SurBSpl
      TSU_DrawSurTBSP ((SurBSpl*)ox1->data, att, -1L);
      break;


    //================================================================
    case Typ_ObjGX:
      i1 = AP_typ_2_bastyp (ox1->typ);
      if(i1 == Typ_SUR) {
        SUR_Disp_Sur (ox1, wrkSpc, att, p1);

      } else {
        ox2 = ox1->data;
        for(i1=0; i1 < ox1->siz; ++i1) {
          // Recursion !
          UTO_Disp_ox (&ox2[i1], wrkSpc, att, p1);
        }
      }
      break;


    //================================================================
    default:
      TX_Error("UTO_Disp_ox E001 %d",oTyp);
      return -1;
  }

  // release work space
  if(wrkSpc) wrkSpc->next = memstart;


  return 0;

  L_E002:
      TX_Error("UTO_Disp_ox E002"); // par fehlt ..
      return -1;

  L_E003:
      TX_Error("UTO_Disp_ox E003"); // wrkSpc fehlt ..
      return -1;

}
*/


//================================================================
  int UTO_copy_obj (void *objo, int *oSiz, void *obji) {
//================================================================
/// \code
/// copy ObjGX-Table, recursive
/// Input:
///   obji   complexObj to be copied
///   objo   where to save obji
///   oSiz   free space at objo
/// Output:
///   objo   copy of obji
///   oSiz   oSiz is reduced (new free size)
/// RetCode:
///   -1     out of memory-Error
///
/// see also UTO_copy_tab DB_store_obj UTRA_app_obj UTO_write_obj
/// \endcode

  int  irc;
  void *p1;

  // printf("UTO_copy_obj oSiz=%d typ=%d form=%d siz=%d\n",*oSiz,
          // ((ObjGX*)obji)->typ,((ObjGX*)obji)->form,((ObjGX*)obji)->siz);

  // copy ParentObj
  *(ObjGX*)objo = *(ObjGX*)obji;
   
  // update oSiz
  *oSiz -= sizeof(ObjGX);
  if(*oSiz < 0) return -1;

  // update objo
  p1 = objo;
  objo = (void*)((char*)objo + sizeof(ObjGX));

  // update data-pointer in objo
  ((ObjGX*)p1)->data = objo;

  // copy data recursive from obji -> objo
  // if(iForm != Typ_ObjGX) {
  irc = UTO_copy_tab (objo, oSiz, obji);
  if(irc < 0) return irc;


  return 0;

}


//================================================================
  int UTO_copy_tab (void *objo, int *oSiz, void *obji) {
//================================================================
/// \code
/// copy ObjGX-object-data (not parentRecord); recursive.
/// Input:
///   objo   where to save data-records of obji
///   oSiz   free space at objo
///   obji   ObjGX-parentRecord (only its data will be copied)
/// Output:
///   objo   all data-records of obji
///   oSiz   oSiz is reduced (new free size)
///   obji   data-pointer correctd; points into objo.
/// RetCode:
///   -1     out of memory-Error
///
/// see also DB_store_obj
/// \endcode

// seit 2005-03-25 werden auch die Datenstructs kopiert; (bisher nicht);
//   ev braucht man zusaetzl ein Flag structs_kopieren-Ja/Nein
// Inputrecord ist vom typ ObjGX; form ist ebenfalls ObjGX
// - es gibt einen oGX-Subrecord. den gesamten Subrecord nach objo kopieren,
// dann durchsuchen und alle alle oGX-Subrecords ebenfalls kopieren.

  int     irc, i1, iForm, iSiz, sTyp, sForm, sSiz, rSiz, oldSiz;
  char    *pi, *po;


  // printf("UTO_copy_tab oSiz=%d typ=%d form=%d siz=%d\n",*oSiz,
          // ((ObjGX*)obji)->typ,((ObjGX*)obji)->form,((ObjGX*)obji)->siz);
  // UTO_dump__ (obji, "UTO_copy_tab");



  if(((ObjGX*)obji)->form != Typ_ObjGX) return 0;


  //================================================================
  // den referenzierten Record kopieren.
  // den gesamten ObjGX-Block nach objo kopieren
  iSiz  = ((ObjGX*)obji)->siz;
  rSiz = iSiz * sizeof(ObjGX);
  *oSiz -= rSiz;
  if(*oSiz < 0) goto L_EOM;
  pi = ((ObjGX*)obji)->data;   // Zeiger auf die InputTable
  po = objo;
  // (char*)objo += rSiz;
  objo = (void*)((char*)objo + rSiz);
  memcpy(po, pi, rSiz);               // ALLE oGX kopieren

  // die Adresse des Subrecord im ParentRecord korrigieren
  ((ObjGX*)obji)->data = po;

  // Loop tru oGX-Table; rekursiv kopieren.
  for(i1=0; i1<iSiz; ++i1) {

    sTyp  = ((ObjGX*)po)->typ;
    if(sTyp  == Typ_Typ)   goto L_GX_nxt;   // hat keine data

    sForm = ((ObjGX*)po)->form;
    sSiz = ((ObjGX*)po)->siz;

    if(sForm == Typ_ObjGX) {
      // Subrecord |Typ_xx|Typ_ObjGX|2|(Point*)| - kopieren !
      oldSiz = *oSiz;
      irc = UTO_copy_tab (objo, oSiz, po);     // Recursion
      // (char*)objo +=  oldSiz - *oSiz;
      objo =  (void*)((char*)objo + oldSiz - *oSiz);
      if(irc < 0) return irc;

    } else {
      if((sForm == Typ_Index) ||
         (sForm == Typ_Int4))    {
        if(sSiz < 2) goto L_GX_nxt;   // Index ist data
        TX_Print("UTO_copy_tab I001 %d",sForm); goto L_GX_nxt;
      }
      // copy struct(s)
      irc = UTO_copy_stru (objo,oSiz,sForm,((ObjGX*)po)->data,sSiz);
      if(irc < 0) return irc;
      ((ObjGX*)po)->data = objo; // Adresse korrigieren
      // (char*)objo +=  irc;
      objo =  (void*)((char*)objo + irc);
    }

    L_GX_nxt:
    // proceed to next ..
    po += sizeof(ObjGX);
  }

  return 0;


  L_EOM:
    TX_Error("UTO_copy_tab EOM");
    return -1;
}


//================================================================
  int UTO_copy_0 (void *objo, int *oSiz, void *obji) {
//================================================================
/// \code
/// DO NOT USE; use OGX_ox_copy__
/// copy ObjGX-Record for isolate.
/// do not copy/modify .data (or later isolate)  <<< NOT YET; using UTO_copy_tab
/// obji must be ObjGX !
///
/// Input:
///   oSiz   size of objo; wird recursiv verkleinert !
///   obji   ist der parentRecord
/// Output:
///   oSiz   freier Restplatz in objo
/// RetCode:   
///   -1     out of memory-Error
///
/// see also UTO_copy_obj UTO_obj_save
/// \endcode

// scheinbar gleich mit UTO_reloc_obj;
// sollte aber alle ObjGX-Records kopieren und nicht ihre data-structs;
// Purpose: nachfolgend alle DB-IndexRecords ersetzen mit den dataStructs
//   (ein isolate data)

  int     irc;
  char    *pi, *po;

  // printf("UTO_copy_0 %d\n",*oSiz);

  // der Inputrecord ist vom Typ ObjGX. Inhalt ist unbekannt.
  // ein Duplikat des obersten obj --> objo
  pi = (char*)obji;
  po = (char*)objo;
  memcpy(po, pi, sizeof(ObjGX));     // parentRecord kopieren
  *oSiz -= sizeof(ObjGX);
  po += sizeof(ObjGX);

  // Record |Typ_PT|Typ_PT   |-|(Point*)|  ist nun fertig kopiert !
  // Record |Typ_xx|Typ_ObjGX|2|(Point*)|  gesamte Subrecord kopieren

  if(((ObjGX*)obji)->form != Typ_ObjGX) goto L_fertig;

  // ein ObjGX-Table rekursiv kopieren
  irc = UTO_copy_tab (po, oSiz, objo);
  if(irc < 0) return -1;



  //================================================================
  L_fertig:
  // UTO_dump__ (objo, "ex UTO_copy_0");

  return 0;


  L_EOM:
   TX_Error("UTO_copy_0 EOM");
   return -1;

}


//=======================================================================
  int UTO_reloc_obj (ObjGX *oxi, long pDist) {
//=======================================================================
/// \code
/// fix all ObjGX-Obj addresses (eg for realloc)
/// pDist = (char*)oldAdress - (char*)newAdress
/// 
/// see also tess_reloc_f_ wrl_reloc__ TSU_store (reallokiert auch !)
/// \endcode


  int   i1;
  ObjGX *oxTab, *oxs;


  // printf("UTO_reloc_obj %ld %d %d %d\n",pDist,oxi->typ,oxi->form,oxi->siz);


  // innerhalb struct wird nix relociert !! Nur fuer Typ_PT OK !
  if(oxi->form != Typ_ObjGX) return 0;

  // (char*)(oxi->data) += pDist;
  oxi->data = (void*)((char*)(oxi->data) + pDist);
  // printf(" change adress in %d %d\n",oxi->typ,oxi->form);

  oxTab = oxi->data;

  for(i1=0; i1<oxi->siz; ++i1) {      // resolv ObjGX(tab)
    oxs = &oxTab[i1];
    // printf(" -objs %d %d %d %d\n",i1,oxs->typ,oxs->form,oxs->siz);

    if(oxs->form == Typ_Int4) {           // Size, Done ..
      continue;

    } else if(oxs->form == Typ_ObjGX) {   // Typ_ObjGX: recurse ..
      UTO_reloc_obj (oxs, pDist);

    } else {
      // skip Typ_Index mit size=1
      if((oxs->form == Typ_Index)&&(oxs->form == 1)) continue;

      // (char*)(oxs->data) += pDist;  // else change adress
      oxs->data = (void*)((char*)(oxs->data) + pDist);  // else change adress
      // printf(" change adress in %d %d\n",oxi->typ,oxi->form);
    }
  }

  return 0;

}


//================================================================
  int UTO_isol_llt (void *oo, int *oSiz, void *oi) {
//================================================================
/// \code
/// resolv LinkTable in oi.
/// replace data in oi (oi is already copied)
/// add new records (Point*) into oo.
/// \endcode

  int    irc, i1, iTyp, iSiz;
  long   *iTab;
  void   *po;


  iTyp = ((ObjGX*)oi)->typ;
  iSiz = ((ObjGX*)oi)->siz;
  iTab = ((ObjGX*)oi)->data;  // die (long*)-IndexTabelle


  if(iTyp == Typ_PT) {
    po = oo;
    for(i1=0; i1<iSiz; ++i1) {
      *oSiz -= iSiz* sizeof(Point);
      if(*oSiz < 0) goto L_EOM;
      *((Point*)oo) = DB_GetPoint (iTab[i1]);
      // (char*)oo += sizeof(Point);
      oo = (void*)((char*)oo + sizeof(Point));
    }
    ((ObjGX*)oi)->form = Typ_PT;
    ((ObjGX*)oi)->data = po;


  } else {
    TX_Error("UTO_isol_llt ETYP %d",iTyp);
  }


  L_fertig:
    return 0;


  L_EOM:
    TX_Error("UTO_isol_llt EOM %d",iTyp);
    return -1;
}


//================================================================
  int UTO_isol_ll1 (void *oo, int *oSiz, void *oi) {
//================================================================
/// \code
/// DO NOT USE; use OGX_ox_copy__
/// resolv Link in oi.
/// replace form und data in oi (oi is already copied)
/// add new records into oo.
/// Input:
///   oi     ein ObjGX-Record mit einem Link
/// \endcode


  int     irc, iTyp, iDir, ssNr, oldSiz;
  char    *pTab;
  ObjGX   ox1;


  // UT3D_stru_dump(Typ_ObjGX, oi, "UTO_isol_ll1");


  iTyp = ((ObjGX*)oi)->typ;
  iDir = ((ObjGX*)oi)->dir;


  // Objekt zum Link aus DB holen.
  ox1 = DB_GetObjGX (iTyp, (long)((ObjGX*)oi)->data);
  if(ox1.typ == Typ_Error){TX_Error("UTO_isol_ll1 E001");return -1;}


  // Den aktuellen Record (den Link) ueberschreiben mit
  //   |typ|form=oGX|siz=subRecSiz|
  memcpy(oi, &ox1, sizeof(ObjGX));    // replace parentRecord
  ((ObjGX*)oi)->dir = iDir;           // copy dir !!!

  // Record |Typ_PT|Typ_PT   |-|(Point*)|  ist nun fertig;
  if(ox1.form != Typ_ObjGX) goto L_fertig;

  // Record |Typ_xx|Typ_ObjGX|2|(Point*)|  gesamte Subrecord kopieren

  // den zugehoerigen Subrecord kopieren ...
  pTab = oo;
  ssNr = ox1.siz;
  oldSiz = *oSiz;
  irc = UTO_copy_tab (oo, oSiz, oi);  // copy subRecords
  if(irc < 0) return irc;
  // (char*)oo +=  oldSiz - *oSiz;
  oo =  (void*)((char*)oo + oldSiz - *oSiz);
  // den neu generierten Subrecord aufloesen ...
  oldSiz = *oSiz;
  irc = UTO_isol_l0 (oo, oSiz, pTab, ssNr); // recurse
  if(irc < 0) return irc;
  // (char*)oo +=  oldSiz - *oSiz;


  L_fertig:
  return 0;

}


//================================================================
  int UTO_isol_l0 (void *objo, int *oSiz, void *obji, int oNr) {
//================================================================
/// \code
/// DO NOT USE; use OGX_ox_copy__
/// die Links (Typ_Index) in obji aufloesen.
/// obji ist eine (schon duplizierte) table of oNr ObjGX-records.
/// objo nur fuer ganz neue objekte
/// \endcode


  int     irc, i1, iTyp, iForm, iSiz, iDir, sNr, ssNr, oldSiz;
  char    *ps, *pTab;
  ObjGX   ox1;


  // printf("UTO_isol_l0 %d %d\n",*oSiz,oNr);


  // loop durch das gesamte obj
  ps = obji;
  for(i1=0; i1<oNr; ++i1) {
    iForm = ((ObjGX*)ps)->form;
    iTyp  = ((ObjGX*)ps)->typ;
    iSiz  = ((ObjGX*)ps)->siz;
    iDir  = ((ObjGX*)ps)->dir;
    // printf(" subGX[%d] typ=%d form=%d siz=%d dir=%d\n",i1,iTyp,iForm,
             // ((ObjGX*)ps)->siz, ((ObjGX*)ps)->dir);

    // subTable: recurse
    if(iForm == Typ_ObjGX) {
      sNr = ((ObjGX*)ps)->siz;
      oldSiz = *oSiz;
      irc = UTO_isol_l0 (objo, oSiz, ((ObjGX*)ps)->data, sNr); // recurse
      // (char*)objo +=  oldSiz - *oSiz;
      objo =  (void*)((char*)objo + oldSiz - *oSiz);
      if(irc < 0) return irc;
    }

    // Link (Index): resolve
    if(iForm == Typ_Index) {
      oldSiz = *oSiz;
      if(iSiz < 2)
        irc = UTO_isol_ll1 (objo, oSiz, ps);
      else 
        irc = UTO_isol_llt (objo, oSiz, ps);
      if(irc < 0) return irc;
      // (char*)objo +=  oldSiz - *oSiz;
      objo =  (void*)((char*)objo + oldSiz - *oSiz);
    }

    L_next:
    ps += sizeof(ObjGX);
  }

  return 0;

}


//======================================================================
  int UTO_rec_set (ObjGX *oxo, int typ, int form, int siz, void *dat) {
//======================================================================
/// DO NOT USE - replaced with OGX_SET_OBJ

// UTO_rec_set             einen Record schreiben
// 
// Usagenotes:
// form = Typ_Int4:   UTO_add_obj1 (&ox, Typ_Int4, Typ_Int4, 1, (void*)1);
//                    setzen manuell:  ox.data = (void*)((long)i1);
//                    extract:  i1 = (int)ox.data;
// form = Typ_Txt:    siz = 1; bei \0 innerhalb der Daten use Typ_Int4
// 
//  UTO_dump__ (oTab, "oTab");
//  UTO_dump_s_ (oTab, "oTab");
// 
// see OGX_SET_OBJ

  // printf("UTO_rec_set typ=%d form=%d siz=%d\n",typ,form,siz);


  oxo->typ  = typ;     // die Bedeutung der Daten im Feld <data>
  oxo->form = form;    // die Struktur der Daten im Feld <data>
  oxo->siz  = siz;     // Anzahl der Daten; wenn > 1: ein Array im Feld <data>
  oxo->data = dat;     //


  // printf("cBuf=|%s|\n",cBuf);

  // // add line
  // UTF_add1_line (cBuf);


  return 0;

}


//================================================================
  int UTO_isol__ (void *objo, int *oSiz, void *obji) {
//================================================================
/// \code
/// DO NOT USE - use OGX_ox_copy__
/// duplicate/isolate object
/// isolate = kill links; change Typ_Index --> Typ_<struct> + Address
/// die DatenAdressen (.data of structs) werden nicht veraendert.
/// Input MUSS ein oGX sein !!!
///
/// Input:
///   obji   (ObjGX*)
///   oSiz   size of objo; wird recursiv verkleinert !
/// Output:
///   objo   (ObjGX*)
///   oSiz   freier Restplatz in objo
/// \endcode

  int  irc, oldSiz;
  char *po;

  // printf("UTO_isol__ %d\n",*oSiz);
  // UTO_dump__ (obji, "UTO_isol__");


  po = objo;

  // den kompletten Record obji nach objo kopieren.
  // die DatenAdressen (.data of structs) werden nicht veraendert.
  oldSiz = *oSiz;
  irc = UTO_copy_0 (objo, oSiz, obji);
  if(irc < 0) return irc;
  // (char*)objo +=  oldSiz - *oSiz; 
  objo =  (void*)((char*)objo + oldSiz - *oSiz);


  // die Links (Typ_Index) in objo aufloesen.
  irc = UTO_isol_l0 (objo, oSiz, po, 1);
  if(irc < 0) return irc;


  // printf("ex UTO_isol__ %d\n",*oSiz);
  // UTO_dump__ ((ObjGX*)po, "ex UTO_isol__");

  return 0;
}


//================================================================
  int UTO_write_endRec (void *outSpc) {
//================================================================
/// UTO_write_endRec        write end-record for tesselated data

  ObjGX *ox1;

  ox1 = outSpc;

  ox1->typ  = Typ_Done;
  ox1->form = Typ_Int4;
  ox1->siz  = 1;
  ox1->data = (void*)0L;

  return 0;

}



//================================================================
  int UTO_addRange (MemTab *oTab, int iTyp, long ind, long oNr) {
//================================================================
// add range to ObjRange

  long        l1;
  ObjRange    or1;


  // printf("UTO_addRange typ=%d ind=%d oNr=%d\n",iTyp,ind,oNr);

  or1.typ = iTyp;
  or1.ind = ind;
  or1.oNr = oNr - 1;     // Anzahl - 1 !!!!!!!

  MemTab_sav (oTab, &l1, &or1, 1);

  return 0;

}


//================================================================
  int UTO_queryRange (MemTab(ObjRange) *rTab, int typ, long ind) {
//================================================================
/// \code
/// UTO_queryRange          query if typ/DB-ind is in ObjRange
/// RetCod:
///   0      yes typ/ind is inside a range-obj;
///  -1      no.
/// \endcode


  int    irc, i1;


  // printf("UTO_queryRange %d %d\n",typ,ind);

  irc = -1;

  for(i1=0; i1<rTab->rNr; ++i1) {
    if(rTab->data[i1].typ != typ) continue;
    if(rTab->data[i1].ind > ind) continue;
    if(rTab->data[i1].ind + rTab->data[i1].oNr < ind) continue;
    irc = 0;
    break;
  }


  // printf("ex UTO_queryRange irc=%d typ=%d ind=%d\n",irc,typ,ind);

  return irc;

}


//================================================================
  int UTO_dumpRange (MemTab(ObjRange) *oTab) {
//================================================================
/// \code
/// RetCod:
///   0      yes typ/ind is inside a range-obj;
///  -1      no.
/// \endcode

  int    i1;

  printf("======== UTO_dumpRange %d =========== \n",oTab->rNr);

  for(i1=0; i1< oTab->rNr; ++i1) {
    printf("or[%d].typ=%d ind=%ld oNr=%d\n",i1,
      oTab->data[i1].typ,
      oTab->data[i1].ind,
      oTab->data[i1].oNr);

  }

  return 0;

}


//================================================================
  int UTO_INT_crv_sur (int *pNr, Point *pa, 
                       int typCv, void *o1,
                       ObjGX  *su1,
                       Memspc *wrkSpc) {
//================================================================
// intersect CRV X SUR; result = points

/// Input:
/// Output:

  int      irc, typSu, cNr, i1;
  double   d1, d2, dist;
  Point    pt1;
  Plane    pl1;
  Vector   vc1;
  ObjGX    *ox2;


  // printf("UTO_INT_crv_sur %d\n",typCv);


  // get typ of surface
  // typSu = UTO_ck_surfTyp (su1);
  i1 = SUR_ck_typ (&typSu, &ox2, &cNr, su1);
    // printf(" typSu=%d\n",typSu);



  //================================================================
  if(typCv == Typ_LN) {

      if(typSu == Typ_SURBSP) {
        // irc = UT3D_pt_intlnsbs (pNr, pa, *pNr, (Line*)o1, su1->data, wrkSpc);
        // ox2 must be the struct of the supporting-surf
        irc = UT3D_pt_intlnsbs (pNr, pa, *pNr, (Line*)o1, ox2->data, wrkSpc);
        if(irc < 0) goto L_fertig;
        goto L_fertig;



      } else if(typSu == Typ_SURPLN) {
        // get plane for SURPLN
        UT3D_ptvc_sur (&pt1, &pl1.vz, NULL, su1);
          // UT3D_stru_dump (Typ_PT, &pt1, "  pt1:");
          // UT3D_stru_dump (Typ_VC, &pl1.vz, "  pl1.vz:");
          // UT3D_stru_dump (Typ_VC, &vc1, "  vc1:");
        UT3D_pl_ptpl (&pl1, &pt1);
        UT3D_vc_ln (&vc1, (Line*)o1);
        UT3D_ptDi_intptvcpln (pa, &dist, &pl1, &((Line*)o1)->p1, &vc1);
          // UT3D_stru_dump (Typ_PT, p2, "  p2:");
        *pNr = 1;
        goto L_fertig;



      } else if(typSu == Typ_SURRU) {
        // intersect line X ruledSurf
        TSU_tr_init_ (su1);
        SRU_init_basPts();
        irc = SRU_parsru_intObj (&d1, &d2, pa, Typ_LN, o1);
        *pNr = 1;
        goto L_fertig;


      } else goto L_noFunc;



  //================================================================
  } else goto L_noFunc;









  L_fertig:
    // printf("ex UTO_INT_crv_sur %d %d\n",irc,*pNr);

  return irc;


  L_noFunc:
  TX_Error(" Intersect not yet implemented %d %d",typCv);
  irc = -1;
  goto L_fertig;


}


//================================================================
  int UTO_INT_susu (int *oTyp, void **oDat,
                    int *typ1Tab, long *dbi1Tab,  int o1Nr,
                    int *typ2Tab, long *dbi2Tab,  int o2Nr,
                    int imod, int outTyp,
                    Memspc *wrkSpc) {
//================================================================
// intersect 2 surfs; result = curve
/// ATTENTION: needs all memspc !
/// Input:
///   o1Typ   list of types; 
///   o1      list of structs; 
///   imod    solutionNr; 0=nur_erstes, 1=nur_zweites usw
///   outTyp  solltyp out: Typ_CVBSP (10) | Typ_CVPOL (8)
/// Output:
///   oTyp    TypLN|Typ_CI|Typ_CVBSP|Typ_CVPOL
///   oDat    struct of type oTyp
///   RetCod  nr of possible solutions
// APT_int_obj :18126

  int   iMaxNr, i1, o1Typ, o2Typ, *ip;
  long  *lp;
  void  *o1, *o2;
  ObjGX *ox1;


  // printf("UTO_INT_susu\n");

  *oTyp  = 0;
  iMaxNr = 1; // nr of solutions

  goto L_Start;


  //----------------------------------------------------------------
  L_swap: // swap tables ..
  MEM_swap_2vp (&typ1Tab, &typ2Tab);
  MEM_swap_2vp (&dbi1Tab, &dbi2Tab);
  MEM_swap_int (&o1Nr, &o2Nr);



  //----------------------------------------------------------------
  L_Start:
  o1Typ = typ1Tab[0];
  o2Typ = typ2Tab[0];


  // printf("UTO_INT_susu %d %d\n",o1Typ,o2Typ);




  //================================================================
  // L = INT R R
  if((o1Typ == Typ_PLN)     &&
     (o2Typ == Typ_PLN))         {

      printf(" INT R%ld R%ld\n",dbi1Tab[0],dbi2Tab[0]);

    o1 = (void*)DB_get_PLN (dbi1Tab[0]);
    o2 = (void*)DB_get_PLN (dbi2Tab[0]);

   // *oDat = UME_reserve (wrkSpc, sizeof(Line));

    UT3D_ln_int2pl ((Line*)*oDat, o1, o2);
    // MAKES ERROR when modelsize changes !  2014-01-05
    // UT3D_ln_setLen ((Line*)*oDat,&((Line*)*oDat)->p1,APT_ln_len,(Line*)*oDat);
      // GR_Disp_ln ((Line*)oDat, 2);

     *oTyp = Typ_LN;
     goto Done;


  //================================================================
  // L = INT A|B R                     // Surf|Solid X Plane
  } else if(o2Typ == Typ_PLN)  {
    goto L_swap;  // swap input; retry ..



  //================================================================
  // L = INT R A                     // Plane X Surf
  } else if((o1Typ == Typ_PLN)     &&
            (o2Typ == Typ_SUR))         {

    o1 = (void*)DB_get_PLN (dbi1Tab[0]);

    goto L_RxS;  // intersect Plane X Surf/Sol.



  //================================================================
  // L = INT R B        // Plane X Body   (70 x 71)
  } else if((o1Typ == Typ_PLN)     &&
            (o2Typ == Typ_SOL))         {

    o1 = (void*)DB_get_PLN (dbi1Tab[0]);    // Plane*
    ox1 = DB_GetSol (dbi2Tab[0]);    // ObjGX*
      // UTO_dump__ (ox1, "o2");


   if(ox1->form == Typ_ObjGX) i1 = ox1->typ;   // zb Typ_SOL ..
   else i1 = ox1->form;

    if(i1 == Typ_SPH) {       // Sphere --> immer Circ
      UT3D_ci_intplsph ((Circ*)*oDat,  o1, ox1->data);
      *oTyp = Typ_CI;

    } else if(i1 == Typ_CON) {
      // Cone: Circ oder Ellipse; Ellipse not yet implemented;
      // daher  nur Circ; else goto Plane X Surf/Sol
      // testen ob KonusAchse parallel zu Plane-Z-Achse
      i1 = INT_intplcon (oTyp, *oDat,  o1, ox1->data);
      if(i1 == -3) goto L_RxS;
      if(i1 < 0) return i1;


    } else if(i1 == Typ_TOR) {
      // Torus: testen ob TorusHauptachse in Plane liegt
      i1 = INT_intpltor (oTyp, *oDat,  o1, ox1->data);
      if(i1 == -3) goto L_RxS;
      if(i1 < 0) return i1;


    } else if(i1 == Typ_SOL) {
      goto L_RxS;  // intersect Plane X Surf/Sol.


    } else {
      // printf("- APT_int_obj R|Sol typ=%d form=%d\n",ox1->typ,ox1->form);
      goto NotImp_err;
    }




  //================================================================
  // L = INT A|B A|B                     // Surf X Surf
  } else if(
    ((o1Typ == Typ_SUR)||(o1Typ == Typ_SOL)) &&
    ((o2Typ == Typ_SUR)||(o2Typ == Typ_SOL)))         {


    // TEST: in xa_test.c
    iMaxNr = INT_intsursur (oTyp, oDat,
                        typ1Tab, dbi1Tab, o1Nr,
                        typ2Tab, dbi2Tab, o2Nr,
                        imod, outTyp);
    goto Done;



  //================================================================
  } else goto NotImp_err;
  goto Done;


  //----------------------------------------------------------------
  // int Plane X Surf/Sol
  L_RxS:

   // *oDat = UME_reserve (wrkSpc, OBJ_SIZ_MAX);

    // ACHTUNG das Tess. benutzt fast alle Speicher; loescht aus_typ usw !
    // sTyp/sTab is a list DB-objects (sTab is the list DB-ind's)
    iMaxNr = INT_intplsur (oTyp, oDat, o1, dbi2Tab, typ2Tab, o2Nr, imod, outTyp);
    goto Done;




  //----------------------------------------------------------------
  Done:
    // printf("ex UTO_INT_susu %d %d\n",*oTyp,iMaxNr);

  return iMaxNr;


  NotImp_err:
  TX_Error("Intersect: - not yet implemented Error");
  return -1;

  // ParErr:
  // TX_Error("Intersect: - Error Parameter %d",i1);
  // return -1;


}

/* replaced by OGX_ox_ato1
//================================================================
  int UTO_obj_ato1 (ObjGX *ocx, int *ii, int *ausTyp, double *ausTab) {
//================================================================
/// \code
/// convert atomicObj (ausTyp/ausTab) into complexObj
///  Typ_Val   Typ_Val    ((double)value)
///  Typ_<typ> Typ_Index  ((int)DB-ind)
/// \endcode

  long  dbi;

  ocx->typ  = ausTyp[*ii];

  if(ocx->typ == Typ_Val) {
    ocx->form = Typ_Val;
    ocx->data = (void*)&ausTab[*ii];  // external address !

  } else {
    ocx->form = Typ_Index;
    dbi =  ausTab[*ii];
    ocx->data = (void*)dbi;
  }

  *ii += 1;


  return 0;

}
*/

//================================================================
  int UTO_CUT__ (ObjGX *oo, ObjGX *oi, ObjGX *oc1, ObjGX *oc2,
                 int imod, Memspc *wrkSpc) {
//================================================================
/// \code
/// trim obj with 1 or 2 cutting-objects;
/// cutting-Parameters:
///    typ=Typ_Par1; form=Typ_Float8; value 0-1;
///    typ=Typ_Val;  form=Typ_Float8; used for knotvalues for BSP's;
///
/// Input:
///   oi              obj to cut!
///   oc1             1. cuttingObj or parameter
///   oc2             2. cuttingObj or parameter; none: oc2.typ=Typ_Error.
///   imod            solutionNr; 0=undefined, 1=first sol...
///
/// Output:
///   oo
///   RetCod          1-n   nr of possible solutions ..
///                  -3     object not yet complete
/// 
/// \endcode

// was UTO_obj_trim
// see also UTO_trim_2par1

#define TABSIZ 20

  int    irc, i1, i2, iMaxSol, inxt, o0Typ, o1Typ, o2Typ, pNr, p1Nr, p2Nr, oSiz,
         io2, iDir, iSol1, iSol2, ii1, ii2, ip1, ip2, ipa, iClo;
  // long   o1i, o2i;
  double va[TABSIZ], v1,  v2, vi1, vi2, d1, vTot;
  char   cbuf[32];
  void   *obj0, *obj1, *obj2, *oxo;
  Point  pa[TABSIZ], *pts, *pte, pt1, pt2, pti1, pti2;



  // printf("================================================= \n");
  // printf("UTO_CUT__ typ=%d form=%d imod=%d\n",oi->typ,oi->form,imod);
  // printf(" _CUT__-0  %d %d\n",oi->typ,oi->form);
  // printf(" _CUT__-1  %d %d\n",oc1->typ,oc1->form);
  // printf(" _CUT__-2  %d %d\n",oc2->typ,oc2->form);
  // if(oc1->form == Typ_Float8) printf(" val1=%f\n",*((double*)oc1->data));
  // if(oc2->form == Typ_Float8) printf(" val2=%f\n",*((double*)oc2->data));
    // UTO_dump_s_ (oi, "oi");



  //----------------------------------------------------------------
  // get obj0 = obj to cut = 'curve'
  o0Typ = UTO_obj_getp (&obj0, &i1, oi);
  if(o0Typ < 0) return -1;
    // UT3D_stru_dump (o0Typ, obj0, "obj0");

  // get obj1 = cutting obj 1  = 'cutter1'
  o1Typ = UTO_obj_getp (&obj1, &i1, oc1);
  if(o1Typ < 0) return -1;
    // UT3D_stru_dump (o1Typ, obj1, "obj1");

  // get obj2 = cutting obj 2 = 'cutter2'
  o2Typ = oc2->typ;
  if(o2Typ != Typ_Error) {
    o2Typ = UTO_obj_getp (&obj2, &i1, oc2);
    if(o2Typ < 0) return -1;
  }
    // printf(" o0Typ=%d o1Typ=%d o2Typ=%d\n",o0Typ,o1Typ,o2Typ);


  //----------------------------------------------------------------
  // create oo = copy of o0
  // copy obj to cut out; get dataSpace from wrkSpc
  oo->typ   = AP_typ_2_bastyp (o0Typ);
  oo->form  = o0Typ;
  oo->siz   = 1;
  oo->data  = wrkSpc->next;
  oxo = oo->data;
  oSiz = UME_ck_free (wrkSpc);
  irc = UTO_copy_stru (oxo, &oSiz, o0Typ, obj0, 1);
  if(irc < 0) return irc;
  UME_add (wrkSpc, irc);          // irc=size (UTO_siz_stru(o0Typ))
    // UTO_dump__ (oo, "oo");


  //----------------------------------------------------------------
  // iClo = test if element is closed (360-deg-circ/elli); 0=YES,1=NO
  if(o0Typ == Typ_LN) {
    iClo = 1; // not closed
  } else {
    iClo = UTO_cv_ck_clo (o0Typ, oxo);  // 1=NO; for L
  }
    // printf(" o0Typ=%d o1Typ=%d o2Typ=%d iClo=%d\n",o0Typ,o1Typ,o2Typ,iClo);


  //----------------------------------------------------------------
  pNr = 0;
  p1Nr = 0;
  p2Nr = 0;


  // if not closed and only one cutter:
  // add startpoint as first point into pa/va.
  if((iClo == 1) && (o2Typ == Typ_Error)) {
      // printf(" _CUT_ 1\n");
    if((TABSIZ - pNr) < 1) goto TabSizErr; 
    // get start and endpoint
    // irc = UTO_2pt_limstru (&pt1, &pt2, &v1, &v2, o0Typ, obj0);
    // get startpoint
    irc = UT3D_ptvcpar_std_obj (&pt1, NULL, &v1, Ptyp_0, o0Typ, obj0);
    if(irc < 0) return irc;
      // UT3D_stru_dump (Typ_PT, &pt1, " _limstru-pt1 v1=%lf",v1);
      // UT3D_stru_dump (Typ_PT, &pt2, " pt2 v2=%lf",v2);
      // GR_Disp_pTab (1, &pt1, SYM_TRI_S, 2);
      // GR_Disp_pTab (1, &pt2, SYM_TRI_S, 2);
      // printf(" __limstru v1=%f v2=%f\n",v1,v2);
    // add startpoint as 1.pt
    pa[pNr] = pt1;
    va[pNr] = v1;
    ++pNr;
    ++p1Nr;
      // printf(" startPt added ..\n");
  }

    // TEST
    // printf(" n.limstru:\n");
    // for(i1=0; i1<pNr; ++i1)printf("pa[%d] %lf %lf %lf va %lf\n",
      // i1, pa[i1].x, pa[i1].y, pa[i1].z, va[i1]);
    // END TEST



  // add intersection-points with cutter1 into pa/va
  // i1 = TABSIZ - pNr; // maxNr (size of pa & va)
  // irc = UTO_stru_int (&i1, &pa[pNr],&va[pNr], o0Typ,obj0, o1Typ,obj1, wrkSpc);
  irc = UTO_npt_int_2ox (&i1, &pa[pNr], &va[pNr], TABSIZ - pNr,
                         oi, oc1, wrkSpc);
  if(irc < 0) goto GeomErr;
  pNr += i1;
  p1Nr += i1;
    // printf(" _stru_int irc=%d i1=%d\n",irc,i1);
    // if((o1Typ == Typ_PT) && (p1Nr > 1)) p1Nr = 1;// why ? closedObj start/end ?
    // printf(" pNr=%d va=%f %f\n",pNr,va[pNr],va[pNr+1]);

    // TESTBLOCK
    // printf(" n.int_2ox: pNr=%d p1Nr=%d p2Nr=%d\n",pNr,p1Nr,p2Nr);
    // for(i1=0; i1<pNr; ++i1) printf("pa[%d] %lf %lf %lf va %lf\n",
        // i1, pa[i1].x, pa[i1].y, pa[i1].z, va[i1]);
    // GR_Disp_pTab (pNr, pa, SYM_STAR_S, 2);
    // END TESTBLOCK



  // if not closed and only one cutter:
  // add endpoint as last point into pa/va.
  if((iClo == 1) && (o2Typ == Typ_Error)) {

    // get endpoint
    irc = UT3D_ptvcpar_std_obj (&pt2, NULL, &v2, Ptyp_1, o0Typ, obj0);
    if(irc < 0) return irc;
    // add endpoint as last point into pa/va.
    if((TABSIZ - pNr) < 1) goto TabSizErr; 
    pa[pNr] = pt2;
    va[pNr] = v2;
    ++pNr;
    ++p1Nr;
      // printf(" end added ..\n");

    // if a closed curve goes tru startpoint (endPt < startPt):
    //   (only plg,bsp)
    if(v2 < v1) {
      // test all intersection-points; if(iPt < startPt) add totalCurve.
      vTot = UTO_par_tot (o0Typ, obj0);
      for(i1=1; i1<pNr; ++i1) {
        if(va[i1] < v1) va[i1] += vTot;
      }
    }
  }


  // sort ascending parameters, points (else closed curve goes over gaps !)
  UTP_sort_npar_npt (p1Nr, va, pa);


  // restore values > vTot after sort  (if > vTot: -= vTot).
  if((iClo == 1) && (o2Typ == Typ_Error)) {
      for(i1=1; i1<pNr; ++i1) {
        if(va[i1] > vTot) va[i1] -= vTot;
      }
  }


  // add intersection-points with cutter2 into pa/va
  if(o2Typ != Typ_Error) {
    // i1 = TABSIZ - pNr; // maxNr (size of pa & va)
    // irc = UTO_stru_int (&i1,&pa[pNr],&va[pNr],o0Typ,obj0,o2Typ,obj2,wrkSpc);
    irc = UTO_npt_int_2ox (&i1, &pa[pNr], &va[pNr], TABSIZ - pNr,
                           oi, oc2, wrkSpc);
    if(irc < 0) goto GeomErr;
    pNr += i1;
    p2Nr += i1;
      // printf(" _stru_int irc=%d i1=%d\n",irc,i1);
      // if((o2Typ == Typ_PT) && (p2Nr > 1)) p2Nr = 1;  ???
      // printf(" _stru_int p2Nr=%d\n",p2Nr);
      // printf(" ipa=%d va=%f %f\n",ipa,va[pNr],va[pNr+1]);
      // GR_Disp_pTab (p2Nr,  &pa[p1Nr], SYM_STAR_S, 3);
      // printf(" _stru_int pNr=%d p1Nr=%d p2Nr=%d\n",pNr,p1Nr,p2Nr);

      // TESTBLOCK
      // printf(" n.int_2ox: pNr=%d p1Nr=%d p2Nr=%d\n",pNr,p1Nr,p2Nr);
      // for(i1=0; i1<pNr; ++i1) printf("pa[%d] %lf %lf %lf va %lf\n",
          // i1, pa[i1].x, pa[i1].y, pa[i1].z, va[i1]);
      // GR_Disp_pTab (pNr, pa, SYM_STAR_S, 2);
      // END TESTBLOCK
  }



  //----------------------------------------------------------------
  // remove equal points
    // printf(" lp1: %d %d %d\n",pNr,p1Nr,p2Nr);
    // for(i1=0; i1<pNr; ++i1)printf("pa[%d] %lf %lf %lf va %lf\n",
      // i1, pa[i1].x, pa[i1].y, pa[i1].z, va[i1]);

  for(i1=0; i1<pNr; ++i1) {
    for(i2=0; i2<pNr; ++i2) {
      if(i1 == i2) continue;
      if(UT3D_comp2pt(&pa[i1], &pa[i2], UT_TOL_pt) != 0) {
          // printf(" identPts i1=%d i2=%d of %d\n",i1,i2,pNr);
        // del rec i2; reduce pNr
        MEM_del_nrec (&pNr, &pa[i2], i2, 1, sizeof(Point));
        ++pNr; // reduced !
        MEM_del_nrec (&pNr, &va[i2], i2, 1, sizeof(double));
        // reduce p1Nr or p2Nr
        if(i2 < p1Nr) --p1Nr;
        else          --p2Nr;
      }
    }
  }


    // TEST
    // printf(" lp2: pNr=%d %d %d\n",pNr,p1Nr,p2Nr);
    // for(i1=0; i1<pNr; ++i1)printf("pa[%d] %lf %lf %lf va %lf\n",
      // i1, pa[i1].x, pa[i1].y, pa[i1].z, va[i1]);
    // END TEST


  if(pNr < 1) { iMaxSol = 0;  goto L_exit; }



  //----------------------------------------------------------------
  // compute iMaxSol = nr of max. solutions and
  --imod;
  // select solution <imod>  - gives ii1, ii2.
  if(o2Typ == Typ_Error) {
    // only cutter1


    if(iClo == 0) {
      // closed
      // need at least 2 points 
      if(pNr < 2) return -3;
      iMaxSol = UTO_MOD_resolv_closed (&ii1, &ii2, imod, pNr);
    } else {
      // not closed
      iMaxSol = UTO_MOD_resolv_open (&ii1, &ii2, imod, pNr);
    }

  } else {
    // do not provide 2 points from same cutter
    if(iClo == 0) {
      // closed
      iMaxSol = UTO_MOD_resolv_two_closed (&iDir, &ii1, &ii2, imod, p1Nr, p2Nr);
      if(iDir) {
        ii2 += p1Nr;
      } else {
        ii1 += p1Nr;
      }
    } else {
      // not closed
      iMaxSol = UTO_MOD_resolv_two_open (&ii1, &ii2, imod, p1Nr, p2Nr);
      ii2 += p1Nr;
    }
  }
    // printf(" iMaxSol=%d ii1=%d ii2=%d\n",iMaxSol,ii1,ii2);

  if(iMaxSol < 1) goto GeomErr;


  //----------------------------------------------------------------
  // LN,AC,CVELL: p1, p2 setzen;
  // CVBSP,CVPOL: v1, v2 setzen.
    // printf(" set: v1=%f v2=%f\n",v1,v2);
    // printf(" ii1=%d ii2=%d\n",ii1,ii2);
  if(ii1 < 0) {
    irc = UTO_set_ptlim (o0Typ, oxo, NULL,     NULL,
                                     &pa[ii2], &va[ii2]);
  } else if(ii2 < 0) {
    irc = UTO_set_ptlim (o0Typ, oxo, &pa[ii1], &va[ii1],
                                     NULL,     NULL);
  } else {
    irc = UTO_set_ptlim (o0Typ, oxo, &pa[ii1], &va[ii1],
                                     &pa[ii2], &va[ii2]);
  }
  if(irc < 0) return irc;


  L_exit:

    // TEST ONLY:
    // printf("---------- ex _CUT__ %d %d\n",irc,iMaxSol);
    // UT3D_stru_dump (Typ_ObjGX, oo, "oo");
    // UTO_dump__ (oo, "oo");
    // UTO_dump_s_ (oo, "oo");
    // printf("---------- ex UTO_CUT__ %d\n",iMaxSol);


  return iMaxSol;


/*
//================================================================
// old Version:

  //----------------------------------------------------------------
  // o0=obj to cut; o1=1.cutter; o2=2.cutter; oo=resultingObj.
  // No o2: o2Typ=Typ_Error.

  // if only one cuttingObject and not a full circle: get limits
  ipa = 0;
  if(o2Typ != Typ_Error) goto L_int11;

  // only 1 cutter.
  // open trimObj: add 1. limitPoint into pa/va
  if(iClo == NO) {
    // 1 cutter; curve not closed.

    // get both limitpoints and parameters of obj to cut -> pt1/pt2 and v1/v2
    irc = UTO_2pt_limstru (&pt1, &pt2, &v1, &v2, o0Typ, obj0);
    if(irc < 0) return irc;
      // printf(" __limstru v1=%f v2=%f\n",v1,v2);

    pa[ipa] = pt1;
    va[ipa] = v1;
    ++ipa;
  }


  //================================================================
  // intersect obj0 with 1.cutter o1;
  L_int11:
  // get points and/or parameters
  p1Nr = TABSIZ - ipa; // maxNr (size of pa & va)
  irc = UTO_stru_int (&p1Nr, &pa[ipa],&va[ipa], o0Typ,obj0, o1Typ,obj1, wrkSpc);
  if(irc < 0) goto GeomErr;
  if((o1Typ == Typ_PT) && (p1Nr > 1)) p1Nr = 1;  // why ? closedObj start/end ?
    printf(" _stru_int p1Nr=%d\n",p1Nr);
    // printf(" ipa=%d va=%f %f\n",ipa,va[ipa],va[ipa+1]);
    // GR_Disp_pt (pa, SYM_STAR_S, 2);
    // GR_Disp_pTab (p1Nr, pa, SYM_STAR_S, 2);

  if(iClo == NO) {
    iMaxSol = UTO_MOD_resolv_open (&ii1, &ii2, imod, p1Nr + 2);
  } else {
    iMaxSol = UTO_MOD_resolv_closed (&ii1, &ii2, imod, p1Nr);
  }
    printf("  iMaxSol=%d\n",iMaxSol);

  // 1 or 2 cuts ?
  if(o2Typ != Typ_Error) goto L_int2;
  // only 1 CuttingObj

  // only 1 cuttingObj..
  //  open Obj to trim: add endpoint
  if(iClo != YES) {    // not for full circle
    i1 = p1Nr + 1;
    pa[i1] = pt2;
    va[i1] = v2;
  }

  // for 360-deg-circ && if ptNr == 1: trim circ to 180 deg
  if((iClo == YES) && (p1Nr == 1)) {
    ii2 = ii1 + 1;
    if(o0Typ == Typ_CI) {
      UT3D_pt_opp2pt (&pa[ii2], &((Circ*)oxo)->pc, &pa[ii1]);
    } else if(o0Typ == Typ_CVELL) {
      UT3D_pt_opp2pt (&pa[ii2], &((CurvElli*)oxo)->pc, &pa[ii1]);
      va[ii2] = 1.;
    }
    if(imod > 1) MEM_swap_int (&ii1, &ii2);
    iMaxSol = 2;
  }


  L_int19:

  pt1 = pa[ii1];
  pt2 = pa[ii2];
  v1 = va[ii1]; 
  v2 = va[ii2]; 
  goto L_99;




  //----------------------------------------------------------------
  // intersect obj0 with 2. cutting obj obj2
  L_int2:
    printf(" L_int2:\n");
// TODO: if o1==o2 skip second intersect ?

  p2Nr = TABSIZ - p1Nr - ipa; // maxNr
  ip2 = ipa + p1Nr;
  irc = UTO_stru_int (&p2Nr, &pa[ip2],&va[ip2], o0Typ,obj0, o2Typ,obj2, wrkSpc);
  if(irc < 0) goto GeomErr;
  if((o2Typ == Typ_PT) && (p2Nr > 1)) p2Nr = 1;
    printf(" _stru_int p2Nr=%d\n",p2Nr);
    // printf(" ipa=%d va=%f %f\n",ipa,va[ip2],va[ip2+1]);
    // GR_Disp_pTab (p2Nr,  &pa[ip2], SYM_STAR_S, 2);


  // remove equal points
  for(i1=0; i1<p1Nr; ++i1) {
    ip1 = ipa + i1;
      // printf(" ip1=%d ip2=%d\n",ip1,ip2);
    for(i2=0; i2<p2Nr; ++i2) {
      if(UT3D_comp2pt(&pa[ip1], &pa[ip2 + i2], UT_TOL_pt) != 0) {
          // printf(" remove pt %d of %d\n",i2,p2Nr);
        MEM_del_nrec (&p2Nr, &pa[ip2], i2, 1, sizeof(Point));
        ++p2Nr;
        MEM_del_nrec (&p2Nr, &va[ip2], i2, 1, sizeof(double));
      }
    }
  }

  if(p2Nr < 1) goto L_int19;


  // // get direction & solutionNr for 2.Cut
  if(iClo != YES) {
    iMaxSol = APT_MOD_resolv_twoOpen (&ii1, &ii2, imod, p1Nr, p2Nr);
    iDir = 0;
  } else {
    iMaxSol = APT_MOD_resolv_twoClosed (&iDir, &ii1, &ii2, imod, p1Nr, p2Nr);
  }
    // printf("n APT_res.. iDir=%d ii1=%d ii2=%d\n",iDir,ii1,ii2);

  pt1 = pa[ii1];
  pt2 = pa[ii2 + p1Nr];
  v1 = va[ii1]; 
  v2 = va[ii2 + p1Nr]; 

  if(iDir < 1) {
    pt1 = pa[ii1];
    pt2 = pa[ii2 + p1Nr];
    v1 = va[ii1]; 
    v2 = va[ii2 + p1Nr]; 
  } else {
    pt2 = pa[ii1];
    pt1 = pa[ii2 + p1Nr];
    v2 = va[ii1]; 
    v1 = va[ii2 + p1Nr]; 
  }
  // goto L_99;



  //----------------------------------------------------------------
  // oOut = set endpoints or parameters;
  L_99:

  // LN,AC,CVELL: p1, p2 setzen;
  // CVBSP,CVPOL: v1, v2 setzen.
    // printf(" set: v1=%f v2=%f\n",v1,v2);
  UTO_set_ptlim (o0Typ, oxo, &pt1, &v1, &pt2, &v2);

  // if(o0Typ == Typ_CVBSP) {
    // // trim also controlpoints and knots; until now trimming by v0,v1
    // CurvBSpl   cvo;
    // Memspc     memTmp;
    // UME_init (&memTmp, memspc102, sizeof(memspc102));
    // UT3D_cbsp_trim (&cvo, oxo, wrkSpc, &memTmp);
    // *(CurvBSpl*)oxo = cvo;   // copy out ..
  // }


//   // Testausgaben:
//     UT3D_stru_dump (o0Typ, oxo, "ex UTO_obj_trim\n");
//     // UTO_Disp_ox (oo, wrkSpc, 9);




  return iMaxSol;  // return nr of possible solutions
  // return p1Nr;
*/

  GeomErr:
  TX_Error("UTO_CUT__: Trim: - Geometry Error");
  return -1;

  ParErr:
  TX_Error("UTO_CUT__: Trim: - Parameter Error");
  return -1;

  TabSizErr:
  TX_Error("UTO_obj_trim: Trim: - TabSiz Error");
  return -1;

  // ObjErr:
  // APED_oid_dbo__ (cbuf, i1, o1i);
  // TX_Error("UTO_obj_trim: Obj %s not defined Error",cbuf);
  // return -1;

  NotImp_err:
  TX_Error("UTO_obj_trim: Trim: - not yet implemented Error");
  return -1;


}


//=====================================================================
  int UTO2_TNG_vc_obj (Point2 *ptg, Vector2 *vtg, int typ, void *obj,
                       int imod) {
//=====================================================================
/// \code
/// get point on 2D-curve (tangent from vector)
/// Typ_CI2 Typ_CVELL2 Typ_CVBSP2
///   imod   0-n   solution-number
/// RetCod: nr of solutions
/// \endcode

  int   ii;

  // printf("UTO2_TNG_vc_obj %d\n",typ);

  switch (typ) {

    case Typ_PT2:
      if(imod) return -1;
      *ptg = *(Point2*)obj;
      ii = 1;
      break;

    case Typ_CI2:
      if(imod > 1) return -1;
      UT2D_pt_tng_ci_vc (ptg, obj, vtg, imod);
      ii = 2;
      break;

    case Typ_CVELL2:
      if(imod > 1) return -1;
      UT2D_pt_tng_ell_vc (ptg, vtg, (CurvEll2*)obj, imod);
      ii = 2;
      break;

    case Typ_CVBSP2:
      ii = UT2D_pt_tng_cbsp_vc (ptg, vtg, obj, imod);
        printf(" \n");

      break;

    default:
      TX_Error("UTO2_TNG_vc_obj: Typ %d unsupported",typ);
      return -1;
  }

    // printf("ex _TNG_vc_obj ii=%d\n",ii);
    // UT3D_stru_dump(Typ_PT2, ptg, "  ptg:");

  return ii;

}

 
//=====================================================================
  int UTO2_TNG_pt_obj (Point2 *ptg, int *isolMax,
                       Point2 *pt1, int typ, void *obj, int isol) {
//=====================================================================
/// \code
/// get point on 2D-curve (tangent from point)
///   isol     0-n; 0=solution-1; 1=solution-2; ..
///    -1      Error isol (no solution possible; decrement isol)
/// \endcode


  int   irc;


  // printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
  // printf("UTO2_TNG_pt_obj %d\n",typ);
  // UT3D_stru_dump (Typ_PT2, pt1, "  pt1:");
  // UT3D_stru_dump (typ, obj, "  obj:");

  switch (typ) {

    case Typ_PT2:
      *isolMax = 0;
      // *ptg = *pt1;
      break;

    case Typ_VC2:
      *isolMax = 0;
      break;

    case Typ_CI2:      // 24
      irc = UT2D_pt_tng_ci_pt (ptg, obj, pt1, isol);
      *isolMax = 1;
      break;

    case Typ_CVELL2:
      irc = UT2D_pt_tng_ell_pt (ptg, obj, pt1, isol);
      *isolMax = 1;
      break;

    case Typ_CVBSP2:   // 30
      irc = UT2D_pt_tng_cbsp_pt (ptg, isolMax, obj, pt1, isol);
      break;

    default:
      TX_Error("UTO2_TNG_pt_obj: Typ %d unsupported",typ);
      return -1;
  }

    // UT3D_stru_dump(Typ_PT2, ptg, "  ptg:");
    // printf("ex UTO2_TNG_pt_obj %d\n",irc);

  return irc;

}


//================================================================
  int UTO2_pt_mid_obj (Point2 *ptc, int typ, void *obj) {
//================================================================
// get centerpoint of 2D-obj

  // printf("UTO2_pt_mid_obj %d\n",typ);

  switch (typ) {

    case Typ_PT2:
      *ptc = *(Point2*)obj;
      break;

    case Typ_CI2:
      *ptc = ((Circ2*)obj)->pc;
      break;

    case Typ_CVELL2:
      *ptc = ((CurvEll2*)obj)->pc;
      break;

    case Typ_CVBSP2:
      UT2D_pt_mid_npt (ptc, ((CurvBSpl2*)obj)->cpTab, ((CurvBSpl2*)obj)->ptNr);
      break;

    default:
      TX_Error("UTO2_pt_mid_obj: Typ %d unsupported",typ);
      return -1;
  }

  // UT3D_stru_dump(Typ_PT2, ptc, "  ptc:");

  return 0;

}


//================================================================
  int UTO_TNG_maxSol (int typ) {
//================================================================
// return max nr of tangent-solutions for objtype
  
  int   irc;

  // printf("UTO_TNG_maxSol %d\n",typ);
  
  switch (typ) {
  
    case Typ_PT2:
      irc = 1;
      break;
  
    case Typ_VC2:
      irc = 1;
      break;
  
    case Typ_CI2:
    case Typ_CI2C:
    case Typ_CVELL2:
    case Typ_CVELL2C:
      irc = 2;
      break;
  
    case Typ_CVBSP2:
      irc = 999;
      break;
  
    default:
      TX_Error("UTO_TNG_maxSol: Typ %d unsupported",typ);
      irc = -1;
  }
  
  return irc;
  
} 
    

//================================================================
  int UTO_TNG_vc_cv (Line *lno, ObjGX *ox1, ObjGX *ox2, int isol,
                    Memspc *memDat1) {
//================================================================
/// \code
/// get tangent vector - curve (P,C,S; not D,A,B)
/// Input:
///   ox1      must be line or vector.
///   ox2      P,C,S
///   isol     0-n;  solution-number
/// Output:
///   RetCod:
///     >0     max nr of solutions
///     -1     no solution for isol; decrement isol.
///     -2     cannot find solution (internal error)
/// \endcode

// see also UTO_TNG_cv_cv

  int     ii, i1, i2;
  long    l1, l2;
  int     o1typ, o2typ;     // types of 3D-curves
  void    *o1, *o2;         // dataStructs of curves 3D-abs
  void    *or1, *or2;       // dataStructs of curves 3D-rel
  int     o21typ, o22typ;   // types of 2D-curves
  void    *o21, *o22;       // dataStructs of 2D-curves
  Vector2 vtg;
  Point2  ptg1, ptg2;
  Line    ln2;


  printf("UTO_TNG_vc_cv %d %d\n",ox1->typ,ox2->typ);

  // get types & dataStrucst of objects
  o1typ = UTO_obj_getp (&o1, &i1, ox1);
  o2typ = UTO_obj_getp (&o2, &i1, ox2);
    UT3D_stru_dump(o1typ, o1, "  o1:");
    UT3D_stru_dump(o2typ, o2, "  o2:");
    printf(" o1typ=%d o2typ=%d\n",o1typ,o2typ);


  if((o1typ == Typ_PT) ||
     (o1typ == Typ_VC) ||
     (o1typ == Typ_LN))    {
    if(o2typ == Typ_PT) return -1;
    if(o2typ == Typ_VC) return -1;
    if(o2typ == Typ_LN) return -1;
  }


  // get objects from 3D-absolut onto 3D-construc2tionPlane (relative, XY-plane)
  l1 = UTO_siz_stru(o1typ);
  l2 = UTO_siz_stru(o2typ);
  or1 = UME_reserve (memDat1, l1);
  or2 = UME_reserve (memDat1, l2);
  UTRA_obj_abs2rel__ (or1, o1typ, o1, memDat1);
  UTRA_obj_abs2rel__ (or2, o2typ, o2, memDat1);
    UT3D_stru_dump(o1typ, or1, "  or1:");
    UT3D_stru_dump(o2typ, or2, "  or2:");


  // get 2D-curves from 3D-objects on constructionPlane
  UTRA_obj2_obj3__ (&o21, &o21typ, or1, o1typ, memDat1);
  UTRA_obj2_obj3__ (&o22, &o22typ, or2, o2typ, memDat1);
    UT3D_stru_dump(o21typ, o21, "  o21:");
    UT3D_stru_dump(o22typ, o22, "  o22:");


  // get tangent vtg;
  if(o21typ == Typ_VC2) {
    vtg = *(Vector2*)o21;
  } else if (o21typ == Typ_LN2) {
    UT2D_vc_ln (&vtg, (Line2*)o21);
  }
    // UT3D_stru_dump(Typ_VC2, &vtg, "  vtg:");


  // get tangent-point ptg
  ii = UTO2_TNG_vc_obj (&ptg1, &vtg, o22typ, o22, isol);
  if(ii < 0) goto Err_no_sol;
    // GR_Disp_pt2 (&ptg1, SYM_STAR_S, 0);

  // set ptg2; dir=vtg; len=APT_ln_len
  // UT2D_pt_traptvclen (&ptg2, &ptg1, &vtg, APT_ln_len);
  UT2D_pt_traptvclen (&ptg2, &ptg1, &vtg, 1.0);  // create ray; 2014-01-07
    // GR_Disp_ln2 (&ptg1, &ptg2, 9);

  // get line from 2D to 3D-constructionPlane
  ln2.p1 = UT3D_pt_pt2 (&ptg1);
  ln2.p2 = UT3D_pt_pt2 (&ptg2);

  // get line back from 3D-constructionPlane to 3D-absolut
  // l1 = sizeof(Line);
  UTRA_obj_rel2abs__  (lno, Typ_LN, &ln2, memDat1);

  return ii;


//----------------------------------------------------------------
  Err_no_sol:
    TX_Error ("UTO_TNG_vc_cv internal error E001");
    return -2;
}


//================================================================
  int UTO_TNG_cv_cv (Line *lno, ObjGX *ox1, ObjGX *ox2, int isol,
                    Memspc *memDat1) {
//================================================================
/// \code
/// get tangent curve - curve (P,C,S; not D,A,B)
/// Tangent curve-point:  point must be first object !
///   isol     0-n; 0=solution-1; 1=solution-2; ..
///   RetCod:
///     >0     max. nr of solutions
///     -1     no solution for isol; decrement isol.
///     -2     cannot find solution (internal error)
/// \endcode

  // Point2 *plg1, *plg2;     // testpolygon
  // int    pt1Nr, pt2Nr;     // nr of points of testpolygon

  int     ii, i1, i2, iRun1, iRun2;
  int     isol1, isol2;
  int     isolMax1, isolMax2;
  int     o1typ, o2typ;     // types of 3D-curves
  void    *o1, *o2;         // dataStructs of curves 3D-abs
  void    *or1, *or2;       // dataStructs of curves 3D-rel
  int     o21typ, o22typ;   // types of 2D-curves
  void    *o21, *o22;       // dataStructs of 2D-curves
  double  d1;
  long    l1, l2;
  Vector  vc1;
  Point2  o1Cen, o2Cen;     // centerPoints of 2D-curves
  Vector2 vt1, vt2;
  Point2  ptg1, ptg2;       // tangentPoints on obj1,obj2
  Line    ln2;


  // printf("UTO_TNG_cv_cv %d\n",isol);


  // get types & dataStrucst of curves
  o1typ = UTO_obj_getp (&o1, &i1, ox1);
  o2typ = UTO_obj_getp (&o2, &i1, ox2);
    // UT3D_stru_dump(o1typ, o1, "  o1:");
    // UT3D_stru_dump(o2typ, o2, "  o2:");


  if((o1typ == Typ_PT)&&(o2typ == Typ_PT)) return -1;


  // point must be 1. object          2013-12-28
  if(o2typ == Typ_PT) {
    MEM_swap_int (&o1typ, &o2typ)
    MEM_swap_2vp (&o1, &o2);
  }


  if((o1typ == Typ_CI)&&(o2typ == Typ_CI)) {
    // problem if circs intersect ..  // 2013-12-28
    ii = UT3D_ln_tng_ci_ci (lno, o1, o2, isol);
    if(ii < 0) return -2;
    // ii = nr of solutions
    return ii;
  }


  // bspl-curves: planar or not ?
  if(o2typ == Typ_CVBSP) {
    if(UT3D_bsp_ck_planar (o2, &WC_sur_act.vz)) {
        // printf(" bspl=3D\n");  // cbsp NOT planar
      if(o1typ == Typ_PT) {
        // 3D-spline; point must be on spline
        // { char   tmpSpc[50000]; Memspc wrkSpc;
        // UME_init (&wrkSpc, tmpSpc, 50000);
        // project pt onto spline; get tangent;
        UT3D_vc_tng_crv_pt (&vc1, o1, Typ_CVBSP, o2);
          // UT3D_stru_dump (Typ_VC, &vc1, " vc1=");
        // }
        // create line from vector
        // UT3D_ln_pt_vc (lno,
        d1 = APT_ModSiz / 2.;
        if(isol) d1 *= -1.;
        UT3D_vc_setLength (&vc1, &vc1, d1);
        UT3D_ln_ptvc (lno, (Point*)o1, &vc1);
        return 2;
      }
      TX_Error("cannot yet get tangent to 3D-spline (point not on spline)..");
      return -1;
    }
  }


  // get curves from 3D-absolut onto 3D-constructionPlane (relative, XY-plane)
  l1 = UTO_siz_stru(o1typ);
  l2 = UTO_siz_stru(o2typ);
  or1 = UME_reserve (memDat1, l1);
  or2 = UME_reserve (memDat1, l2);
  UTRA_obj_abs2rel__ (or1, o1typ, o1, memDat1);
  UTRA_obj_abs2rel__ (or2, o2typ, o2, memDat1);
    // UT3D_stru_dump(o1typ, or1, "  or1:");
    // UT3D_stru_dump(o2typ, or2, "  or2:");

  
  // get 2D-curves from 3D-curves on constructionPlane
  UTRA_obj2_obj3__ (&o21, &o21typ, or1, o1typ, memDat1);
  UTRA_obj2_obj3__ (&o22, &o22typ, or2, o2typ, memDat1);
    // UT3D_stru_dump(o21typ, o21, "  o21:");
    // UT3D_stru_dump(o22typ, o22, "  o22:");


  //----------------------------------------------------------------
  // get max nr of tangent-solutions for objs
  isolMax1 = UTO_TNG_maxSol (o21typ) - 1;
  isolMax2 = UTO_TNG_maxSol (o22typ) - 1;

  iRun2 = 0;

  L_init_0:
    // printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX iRun2=%d\n",iRun2);
    // printf("L_init_0: isolMax1=%d isolMax2=%d\n",isolMax1,isolMax2);
  ++iRun2;
  if(iRun2 > 10) goto Err_no_sol;
  ii = isol;
  isol1 = 0;
  isol2 = 0;

  L_init_1:
    // printf("L_init_1: ii=%d isol1=%d isol2=%d\n",ii,isol1,isol2);
  // increment isol2 until error
  // error isol2: increment isol1; reset isol2; goto L_init_0;
  if(ii <= 0) goto L_start1;
  if(isol2 < isolMax2) { 
    --ii;
    ++isol2;
    goto L_init_1;
  } 
  if(isol1 < isolMax1) {
    --ii;
    ++isol1;
    isol2 = 0;
    if(o22typ == Typ_CVBSP2) isolMax2 = 999;
    goto L_init_1;
  }
  goto Err_no_sol;



  //----------------------------------------------------------------
  L_start1:
// isol1=0;isol2=0;   // 0
// isol1=1;isol2=0;   // 1
// isol1=1;isol2=1;   // 2
    // printf("isol=%d isol1=%d isol2=%d\n",isol,isol1,isol2);


  // get vt1 = vector along centerpoints of both objs
  // Point2 o1Cen, o2Cen;     // centerPoints of objects
  UTO2_pt_mid_obj (&o1Cen, o21typ, o21);
  UTO2_pt_mid_obj (&o2Cen, o22typ, o22);

  // make initial vector of centerpoints;
  UT2D_vc_2pt (&vt1, &o1Cen, &o2Cen);

  // get ptg1 = startpoint on obj1
  // get initial point on obj1 (tangent from vector)
  ii = UTO2_TNG_vc_obj (&ptg1, &vt1, o21typ, o21, isol1);
  if(ii < 0) goto Err_int;

  iRun1 = 0;


  // find point on obj2 (tangent from point)
  L_tng_o2:
  ++iRun1;
  ii = UTO2_TNG_pt_obj (&ptg2, &isolMax2, &ptg1, o22typ, o22, isol2);
  if(ii < 0) goto L_init_0;
  UT2D_vc_2pt (&vt1, &ptg1, &ptg2);
  UT2D_vc_setLength (&vt1, &vt1, 1.);
    // UT3D_stru_dump(Typ_VC2, &vt1, "  vt1:");

  // if o1 or o2 = point: done.
  if((o21typ == Typ_PT2)||(o22typ == Typ_PT2)) goto L_done;

  // test deviation;
  if(iRun1 > 1) {
    ii = UT2D_ckvc_parl (&vt1, &vt2, UT_TOL_Ang1);
    if(ii == 0) goto L_done;
  }
  // refine point on obj1 (tangent from point)
  ii = UTO2_TNG_pt_obj (&ptg1, &isolMax1, &ptg2, o21typ, o21, isol1);
  if(ii < 0) goto L_init_0;
  UT2D_vc_2pt (&vt2, &ptg1, &ptg2);
  UT2D_vc_setLength (&vt2, &vt2, 1.);
    // UT3D_stru_dump(Typ_VC2, &vt2, "  vt2:");


  // test deviation;
  ii = UT2D_ckvc_parl (&vt1, &vt2, UT_TOL_Ang1);
  if(ii == 0) goto L_done;
  if(iRun1 > 100) goto Err_int;
  goto L_tng_o2;


  //----------------------------------------------------------------
  L_done:
    // printf(" iRun1=%d iRun2=%d\n",iRun1,iRun2);
    // GR_Disp_ln2 (&ptg1, &ptg2, 5);

  // get line from 2D to 3D-constructionPlane
  ln2.p1 = UT3D_pt_pt2 (&ptg1);
  ln2.p2 = UT3D_pt_pt2 (&ptg2);

  // get line back from 3D-constructionPlane to 3D-absolut
  // l1 = sizeof(Line);
  UTRA_obj_rel2abs__  (lno, Typ_LN, &ln2, memDat1);
    // GR_Disp_ln (lno, 9);


    // printf("L_init_0: isolMax1=%d isolMax2=%d\n",isolMax1,isolMax2);
  ii = (isolMax1 + 1) * (isolMax2 + 1);
  return ii;

//----------------------------------------------------------------
  Err_no_sol:
    TX_Error ("UTO_TNG_cv_cv no-solution-error E001\n");
  return -1;

  Err_int:
    TX_Error ("UTO_TNG_cv_cv internal error E002");
    return -2;
}


//================================================================
  int UTO_cvtrm_cv (CurvCCV *cvtrm, int typ, long dbi) {
//================================================================
/// create trimmedCurve from DB-lFig (copy curve -> trimmedCurve)


  cvtrm->typ = typ;
  cvtrm->dbi = dbi;


  cvtrm->v0  = 0.;   // unmod.
  cvtrm->v1  = 1.;   // unmod.

  cvtrm->ip0 = 0;    // undef
  cvtrm->ip1 = 0;    // undef

  cvtrm->rev = 0;    // unmod.

  cvtrm->is0 = 0;    // TODO (only CurvCCV)
  cvtrm->is1 = 0;    // TODO (only CurvCCV)

    // UT3D_stru_dump (Typ_CVCCV, cvtrm, " UTO_cvtrm_cv");

  return 0;

}


//====================================================================
  int UTO_cv_cvtrm (int *otyp, void *cvo, void *cvi, CurvCCV *cvtrm) {
//====================================================================
/// \code
/// change trimmedCurve into standardCurve
/// Input:
///   otyp     type of struct cvi
///   cvi      untrimmed curve (basic curve of cvtrm); NULL if unknown
///   cvtrm    trim-record
/// Output:
///   otyp     exact type of struct cvo
///   cvo      trimmed curve of type otyp (if size unknown: OBJ_SIZ_MAX);
///
/// see also UT3D_obj_ccv_segnr
/// \endcode

// char    obj1[OBJ_SIZ_MAX];
// UTO_cv_cvtrm (&typ1, obj1, NULL, ccv);


  int       irc, i1, oNr; //pTyp=Typ_PT;
  double    d1, u0, u1;
  Point     p1, p2, *pp1;


  // printf("____________________________________ \n");
  // printf("UTO_cv_cvtrm typ=%d dbi=%ld cvi=%p\n",cvtrm->typ,cvtrm->dbi,cvi);
  // printf("  otyp=%d v0=%lf v1=%lf\n",*otyp,cvtrm->v0,cvtrm->v1);
  // UT3D_stru_dump (Typ_CVCCV, cvtrm, "UTO_cv_cvtrm");



  if(!cvi) {   // 2016-06-26
    // base-curve not given; get it ..
    // line can have dbi=0;
    *otyp = cvtrm->typ;    // 2014-07-20
    if(cvtrm->dbi) {
      irc = UTO_get_DB (&cvi, &oNr, otyp, cvtrm->dbi);
      if(irc < 0) return -1;
        // UT3D_stru_dump (*otyp, cvi, " cvi");
    }
      // printf("UTO_cv_cvtrm otyp=%d\n",*otyp);
  }

/*
    if(cvtrm->dbi) {
      *otyp = cvtrm->typ;    // 2014-07-25
      irc = UTO_get_DB (&cvi, &oNr, otyp, cvtrm->dbi);
      if(irc < 0) return -1;
        // UT3D_stru_dump (*otyp, cvi, " cvi");
    } else {
        *otyp = cvtrm->typ;    // 2014-07-20
    }
*/


  switch (*otyp) {


    //----------------------------------------------------------------
    case Typ_PT:        // Point
      memcpy (cvo, cvi, sizeof(Point));
      break;


    //----------------------------------------------------------------
    case Typ_LN:        // Line
      if(cvi) memcpy (cvo, cvi, sizeof(Line));
      // if(cvtrm->rev) UT3D_ln_inv ((Line*)cvo);  // revert line
      if(cvtrm->ip0) {
        // irc = UTO_get_DB ((void**)&pp1, &oNr, &pTyp, cvtrm->ip0);
        // ((Line*)cvo)->p1 = *pp1;
        ((Line*)cvo)->p1 = DB_GetPoint (cvtrm->ip0);
      } else {
        UT3D_pt_evpar2pt (&((Line*)cvo)->p1, cvtrm->v0,
                          &((Line*)cvi)->p1, &((Line*)cvi)->p2);
      }
      if(cvtrm->ip1) {
        // irc = UTO_get_DB ((void**)&pp1, &oNr, &pTyp, cvtrm->ip1);
        // ((Line*)cvo)->p2 = *pp1;
        ((Line*)cvo)->p2 = DB_GetPoint (cvtrm->ip1);
      } else {
        UT3D_pt_evpar2pt (&((Line*)cvo)->p2, cvtrm->v1,
                          &((Line*)cvi)->p1, &((Line*)cvi)->p2);
      }
      break;


    //----------------------------------------------------------------
    case Typ_CI:        // Circ
      memcpy (cvo, cvi, sizeof(Circ));
      if(cvtrm->rev) ((Circ*)cvo)->rad *= -1.;
      if(cvtrm->ip0) {
        // irc = UTO_get_DB ((void**)&pp1, &oNr, &pTyp, cvtrm->ip0);
        // p1 = *pp1;
        p1 = DB_GetPoint (cvtrm->ip0);
      } else {
        UT3D_pt_evparci (&p1, cvtrm->v0, cvi);
      }
      if(cvtrm->ip1) {
        // irc = UTO_get_DB ((void**)&pp1, &oNr, &pTyp, cvtrm->ip1);
        // p2 = *pp1;
        p2 = DB_GetPoint (cvtrm->ip1);
      } else {
        UT3D_pt_evparci (&p2, cvtrm->v1, cvi);
      }
        // UT3D_stru_dump (Typ_PT, &p1, "  p1");
        // UT3D_stru_dump (Typ_PT, &p2, "  p2");
      // test for 360-deg
      if(UT3D_comp2pt(&p1, &p2, UT_TOL_min0)) {
        UT3D_ci_cipt360 (cvo, &p1);
      } else {
        UT3D_ci_cip3 (cvo, &p1, &p2);
      }
      // if(cvtrm->rev) UT3D_ci_inv2 (cvo);  // revert circ (makes complement !)
      break;


    //----------------------------------------------------------------
    case Typ_CVPOL:        // CurvPoly
      memcpy (cvo, cvi, sizeof(CurvPoly));
      if(cvtrm->ip0) {
        // get parameter from point on polygon
        // irc = UTO_get_DB ((void**)&pp1, &oNr, &pTyp, cvtrm->ip0);
        pp1 = DB_get_PT (cvtrm->ip0);
        UT3D_parplg_plgpt (&((CurvPoly*)cvo)->v0, pp1, cvi);
      } else {
        ((CurvPoly*)cvo)->v0 = UT3D_par_par1plg (cvtrm->v0, cvi);
      }
      if(cvtrm->ip1) {
        // get parameter from point on polygon
        // irc = UTO_get_DB ((void**)&pp1, &oNr, &pTyp, cvtrm->ip1);
        pp1 = DB_get_PT (cvtrm->ip1);
        UT3D_parplg_plgpt (&((CurvPoly*)cvo)->v1, pp1, cvi);
      } else {
        ((CurvPoly*)cvo)->v1 = UT3D_par_par1plg (cvtrm->v1, cvi);
      }
        // printf(" plg-v0=%lf v1=%lf\n",((CurvPoly*)cvo)->v0,((CurvPoly*)cvo)->v1);
      if(((CurvPoly*)cvo)->v1 < ((CurvPoly*)cvo)->v0) cvtrm->rev = 1;
      // set bit: 0=CCW, 1=CW
      ((CurvPoly*)cvo)->dir = UT3D_sr_rev_obj (((CurvPoly*)cvi)->dir, cvtrm->rev);
        // printf(" CVPOL-dir=%d dir=%d rev=%d\n",((CurvPoly*)cvo)->dir,
          // ((CurvPoly*)cvi)->dir,cvtrm->rev);
      break;


    //----------------------------------------------------------------
    case Typ_CVELL:        // CurvElli
      memcpy (cvo, cvi, sizeof(CurvElli));
      if(cvtrm->ip0) {
        // irc = UTO_get_DB ((void**)&pp1, &oNr, &pTyp, cvtrm->ip0);
        // ((CurvElli*)cvo)->p1 = *pp1;
        ((CurvElli*)cvo)->p1 = DB_GetPoint (cvtrm->ip0);
      } else {
        // compute point from 0-1 parameter
        UT3D_pt_eval_ell_par1 (&((CurvElli*)cvo)->p1, (CurvElli*)cvi, cvtrm->v0);
      }
      if(cvtrm->ip1) {
        // irc = UTO_get_DB ((void**)&pp1, &oNr, &pTyp, cvtrm->ip1);
        // ((CurvElli*)cvo)->p2 = *pp1;
        ((CurvElli*)cvo)->p2 = DB_GetPoint (cvtrm->ip1);
      } else {
        // compute point from 0-1 parameter
        UT3D_pt_eval_ell_par1 (&((CurvElli*)cvo)->p2, (CurvElli*)cvi, cvtrm->v1);
      }
      ((CurvElli*)cvo)->dir = UT3D_sr_rev_obj (((CurvElli*)cvi)->dir, cvtrm->rev);
      break;


    //----------------------------------------------------------------
    case Typ_CVBSP:        // CurvBSpl
      memcpy (cvo, cvi, sizeof(CurvBSpl));
      if(cvtrm->ip0) {
        // get parameter from point on polygon
        // irc = UTO_get_DB ((void**)&pp1, &oNr, &pTyp, cvtrm->ip0);
        pp1 = DB_get_PT (cvtrm->ip0);
          // UT3D_stru_dump(Typ_PT, pp1, "  bsp-ip0:");
        // UT3D_parCv_bsplpt (&u0, &d1, cvi, pp1);
        UT3D_parCv_bsplpt (&((CurvBSpl*)cvo)->v0, &d1, cvi, pp1);
      } else {
        // ((CurvBSpl*)cvo)->v0 = cvtrm->v0;  ERR: par1 -> knotVal !
        // u0 == UT3D_parbsp_par1 (cvtrm->v0, (CurvBSpl*)cvi);
        ((CurvBSpl*)cvo)->v0 = UT3D_parbsp_par1 (cvtrm->v0, (CurvBSpl*)cvi);
      }
      if(cvtrm->ip1) {
        // get parameter from point on polygon
        // irc = UTO_get_DB ((void**)&pp1, &oNr, &pTyp, cvtrm->ip1);
        pp1 = DB_get_PT (cvtrm->ip1);
          // UT3D_stru_dump(Typ_PT, pp1, "  bsp-ip1:");
        UT3D_parCv_bsplpt (&u1, &d1, cvi, pp1);
        UT3D_parCv_bsplpt (&((CurvBSpl*)cvo)->v1, &d1, cvi, pp1);
      } else {
        // ((CurvBSpl*)cvo)->v1 = cvtrm->v1;  ERR: par1 -> knotVal !
        // u1 = UT3D_parbsp_par1 (cvtrm->v1, (CurvBSpl*)cvi);
        ((CurvBSpl*)cvo)->v1 = UT3D_parbsp_par1 (cvtrm->v1, (CurvBSpl*)cvi);
      }

        // printf(" bsp-v0=%lf v1=%lf\n",((CurvBSpl*)cvo)->v0,((CurvBSpl*)cvo)->v1);
      // ((CurvBSpl*)cvo)->v0 = u0;
      // ((CurvBSpl*)cvo)->v1 = u1;

      // set bit: 0=CCW, 1=CW
      ((CurvBSpl*)cvo)->dir = UT3D_sr_rev_obj (((CurvBSpl*)cvi)->dir, cvtrm->rev);
      break;


    //----------------------------------------------------------------
    case Typ_CVRBSP:       // CurvRBSpl
      memcpy (cvo, cvi, sizeof(CurvRBSpl));
      // ((CurvRBSpl*)cvo)->v0 = cvtrm->v0;
      // ((CurvRBSpl*)cvo)->v1 = cvtrm->v1;
      u0 = UT3D_par_par1_rbsp (cvtrm->v0, (CurvRBSpl*)cvo);
      u1 = UT3D_par_par1_rbsp (cvtrm->v1, (CurvRBSpl*)cvo);
      ((CurvRBSpl*)cvo)->v0 = u0;
      ((CurvRBSpl*)cvo)->v1 = u1;
      // ((CurvRBSpl*)cvo)->dir = UT3D_sr_rev_obj (((CurvRBSpl*)cvi)->dir, cvtrm->rev);
      break;

    //----------------------------------------------------------------
    case Typ_CVCLOT:       // CurvClot
      //UT3D_ptvc_evparclot(&((CurvClot*)cvo)->p1,NULL,0,(CurvClot*)cvi,cvtrm->v0);

    //----------------------------------------------------------------
    // case Typ_CVCCV:       // CurvCCV


    default:
      TX_Error("UTO_cv_cvtrm Etyp_%d",*otyp);
      return -1;
  }


    // UT3D_stru_dump (*otyp, cvo, "ex UTO_cv_cvtrm");
    // printf("____________________________________ \n");

  return 0;

}


//==================================================================
  int UTO_MOD_resolv_open (int *ii1, int *ii2, int isol, int ptNr) {
//==================================================================
// get indexes according to resolutionNr isol.
// Input:
//   isol     MODificationNr; (0=not defined; 1=first sol, 2=second sol ..)
//   ptNr     nr of points;  3 gives 3 solutions; 4 gives 5 solutions..
// Output:
//   ii1      index to 1. point;
//   ii2      index to 2. point;
//   RetCod   max nr of solutions
//
// Example:  ptNr=3; retCod=2;
//      ii1  ii2  isol
//       0    1     0
//   //  0    2       skipped
//       1    2     1
//
// Example:  ptNr=4; retCod=5;
//       0    1     0
//       0    2     1
//   //  0    3        skipped
//       1    2     2
//       1    3     3
//       2    3     4
//
// Example:  ptNr=5; retCod=9;
//       0    1     0
//       0    2     1
//       0    3     2
//   //  0    4       skipped
//       1    2     3
//       1    3     4
//       1    4     5
//       2    3     6
//       2    4     7
//       3    4     8


// see also APT_trim_u0 APT_trim_uu

  int  iMax, ie;

  // printf("UTO_MOD_resolv_open isol=%d ptNr=%d\n",isol,ptNr);

  if(isol < 0) isol = 0;

  if(ptNr <= 2) {
    iMax = 1;
    *ii1 = 0;
    *ii2 = 1;
    goto L_exit;
  }

  // get nr of max solutions
  ie = ptNr - 1;
  iMax = UTI_sum_row (ie) - 1; // + ptNr;
  isol = isol % iMax;
    // printf("  isol=%d ptNr=%d iMax=%d\n",isol,ptNr,iMax);

  // 1. loop: nr = ptNr; ii1=0; ii2=0-ptNr
  *ii1 = 0;
  *ii2 = 1;

  while(isol) {
    *ii2 += 1;
    if(*ii2 >= ptNr) {
      *ii1 += 1;
      *ii2 = *ii1 + 1;
    }
    if((*ii1 == 0) && (*ii2 == ie)) continue; // skip comp first+last
    --isol;
  }


  L_exit:
    // printf("  ex _MOD_resolv_open ii1=%d ii2=%d iMax=%d\n",*ii1,*ii2,iMax);

  return iMax;

}
 

//====================================================================
  int UTO_MOD_resolv_closed (int *ii1, int *ii2, int isol, int ptNr) {
//====================================================================
// get indexes according to resolutionNr isol.
// circle; 
// Input:
//   isol     MODificationNr; (0=not defined; 1=first sol, 2=second sol ..)
//   ptNr     nr of points;  3 gives 6 solutions; 4 gives 12 solutions..
// Output:
//   ii1      index to 1. point;
//   ii2      index to 2. point;
//   RetCod   max nr of solutions (ptNr * (ptNr - 1))
//
// Example:  ptNr=2; retCod=2;
//      ii1  ii2  isol
//       0    1     0
//       1    0     1
//
// Example:  ptNr=3; retCod=6;
//      ii1  ii2  isol
//       0    1     0
//       0    2     1
//       1    0     2
//       1    2     3
//       2    0     4
//       2    1     5
//
// Example:  ptNr=4; retCod=12;
//      ii1  ii2  isol
//       0    1     0
//       0    2     1
//       0    3     2
//       1    0     3
//       1    2     4
//       1    3     5
//       2    0     6
//       2    1     7
//       2    3     8
//       3    0     9
//       3    1    10
//       3    2    11

// see also APT_trim_u0 APT_trim_uu

  int  iMax;


  // printf("UTO_MOD_resolv_closed isol=%d ptNr=%d\n",isol,ptNr);

  if(isol < 0) isol = 0;

  // get nr of max solutions
  iMax = ptNr * (ptNr - 1);

  if(ptNr < 2) goto L_exit;

  isol = isol % iMax;
    // printf("  isol=%d ptNr=%d iMax=%d\n",isol,ptNr,iMax);

  

  // 1. loop: nr = ptNr; ii1=0; ii2=0-ptNr
  *ii1 = 0;
  *ii2 = 1;

  while(isol) {
    *ii2 += 1;
    if(*ii2 >= ptNr) {
      *ii1 += 1;
      *ii2 = 0;
    }
    if(*ii1 == *ii2) continue;
    --isol;
  }


  L_exit:
    // printf("  ex UTO_MOD_resolv_closed ii1=%d ii2=%d iMax=%d\n",
           // *ii1,*ii2,iMax);

  return iMax;

}
 

//===========================================================================
  int UTO_MOD_resolv_two_open (int *ii1, int *ii2, int imod, int p1Nr, int p2Nr) {
//===========================================================================
// get indexes according to resolutionNr imod.
// Input:
//   imod     MODificationNr; (0=not defined; 1=first sol, 2=second sol ..)
//   p1Nr     nr of points on 1. cuttingObj;
//   p2Nr     nr of points on 2. cuttingObj;
// Output:
//   ii1      index to 1. point;
//   ii2      index to 2. point;
//   RetCod   max nr of solutions; is always (p1Nr * p2Nr)
//
// Example:  p1Nr=2; p2Nr=2; retCod=4;
//      ii1  ii2  imod
//       0    0     0
//       0    1     1
//       1    0     2
//       1    1     3

// see also APT_trim_u0 APT_trim_uu UTO_MOD_resolv_closed UTO_MOD_resolv_open

  int  iMax;


  // printf("APT_MOD_resolv_twoOpen imod=%d p1Nr=%d p2Nr=%d\n",imod,p1Nr,p2Nr);


  if(p2Nr < 1) {
    return UTO_MOD_resolv_open (ii1, ii2, imod, p1Nr);
  }

  if(imod < 0) imod = 0;

  iMax = p1Nr * p2Nr;
  if(iMax < 1) {      // % 0 makes crash
    iMax = p1Nr + p2Nr;
    *ii1 = 0;
    *ii2 = 0;
    goto L_exit;
  }

  imod = imod % iMax;

  *ii1 = imod / p2Nr;
  *ii2 = imod % p2Nr;

 L_exit:
    // printf("ex _MOD_resolv_two_open ii1=%d ii2=%d iMax=%d\n",*ii1,*ii2,iMax);

  return iMax;

}


//===========================================================================
  int UTO_MOD_resolv_two_closed (int *iRev, int *ii1, int *ii2,
                                int imod, int p1Nr, int p2Nr) {
//===========================================================================
// get indexes according to resolutionNr imod.
// Input:
//   imod     MODificationNr; (0=first sol, 1=second sol ..)
//   p1Nr     nr of points on 1. cuttingObj;
//   p2Nr     nr of points on 2. cuttingObj;
// Output:
//   iRev     0: ii1 is index in table 1; ii2 in table 2.
//            1: ii1 is index in table 2; ii2 in table 1.
//   ii1      index to 1. point;
//   ii2      index to 2. point;
//   RetCod   max nr of solutions; is always (2 * (p1Nr + p2Nr))
//
// p2-points follow p1-points; first point of p2 has index [p1Nr]
//
// Example:  p1Nr=1; p2Nr=1; retCod=2;
//      iRev ii1  ii2  imod
//       0    0    0     0
//       1    0    0     1
//
// Example:  p1Nr=2; p2Nr=1; retCod=4;
//      iRev ii1  ii2  imod
//       0    0    0     0
//       0    1    0     1
//       1    2    0     2
//       1    2    0     4
//
// Example:  p1Nr=2; p2Nr=2; retCod=8;
//      iRev ii1  ii2  imod
//       0    0    0     0
//       0    0    1     1
//       0    1    0     2
//       0    1    1     3
//       1    0    0     4
//       1    0    1     5
//       1    1    0     6
//       1    1    1     7

// see also APT_trim_u0 APT_trim_uu UTO_MOD_resolv_closed UTO_MOD_resolv_open
  
  int  iMax, ii;

  // printf("APT_MOD_resolv_twoClosed imod=%d p1Nr=%d p2Nr=%d\n",imod,p1Nr,p2Nr);


  if(imod < 0) imod = 0;

  if(p1Nr < 1) {
    *iRev = 1;
    *ii1  = -1;
    *ii2  = imod;
   iMax   = p2Nr;
   goto L_exit;
  }

  if(p2Nr < 1) {
    *iRev = 0;
    *ii1  = imod;
    *ii2  = -1;
   iMax   = p1Nr;
   goto L_exit;
  }

  ii = p1Nr * p2Nr;
  iMax = ii * 2;

  if(iMax) imod = imod % iMax;
  else     imod = 0;

  *iRev = imod / ii;   // 0 or 1
  if(*iRev) imod -= ii;

  *ii1 = imod / p2Nr;
  *ii2 = imod % p2Nr;

 L_exit:
    // printf("  ex APT_MOD_resolv_twoClosed iRev=%d ii1=%d ii2=%d iMax=%d\n",
            // *iRev,*ii1,*ii2,iMax);

  return iMax;

}


//========================== EOF ========================================
