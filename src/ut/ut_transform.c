//   ut_transform.c               CADCAM-Services Franz Reiter  2005-02-25
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
#ifdef globTag
void UTRA(){}
#endif
/*!
\file  ../ut/ut_transform.c
\brief transformations UTRA_ 
\code
=====================================================
List_functions_start:

// UTRA_obj_coordSys_pln   change objCoords 3D->2D or 2D->3D
UTRA_obj_abs2rel__      obj from 3D-absolut onto 3D-constructionPlane
UTRA_obj_rel2abs__      obj from 3D-constructionPlane to 3D-absolut

UTRA_obj2_obj3__        curves from 3D-constructionPlane to 2D (removes Z-coord)

UTRA_inirot_pt_vc_angr  define rotation um eine freie Achse
UTRA_inirotZ_pt_angr    define rotation around z-axis; (center, angle)
UTRA_inirot_2vc         define rotation-transformation between 2 vectors
UTRA_def__              define transformation (translation/rotation)

UTRA_nobj_tra           apply transf. to ObjGX-Table
UTRA_app_dbo            transform DB-object (typ,DB-ind) and store in DB
UTRA_app_out            apply transformation, copy into outputspace
UTRA_app__              apply transformation, 
UTRA_app_obj            apply Transformation to struct; NOT object.
UTRA_app_ox             apply Transformation to complex-obj
UTRA_app_pt             apply all defined Transformations to a point
UTRA_app_pt2            apply all defined Transformations to a 2D-point
UTRA_app_vc             apply all defined Transformations to a vector
UTRA_app_CCV            transform CCV
UTRA_app_dbo            transform DB-object and store as dynam. obj.
UTRA_tra_pt             apply a single Transformation to a point
UTRA_tra_pt2            apply a single Transformation to a 2D-point
UTRA_tra_vc             apply a single Transformation to a vector
UTRA_tra_ci
UTRA_tra_ell
UTRA_tra_cbsp
UTRA_tra_plg
UTRA_tra_pln            transform refSys

UTRA_UCS_WCS_VC         transfer vector from WCS into UCS
UTRA_UCS_WCS_PT         transfer point from WCS into UCS
UTRA_pt_wcs2ucs         transform point absolut to constructionPlane

UTRA_WCS_UCS_VC         transfer vector from UCS into WCS
UTRA_WCS_UCS_PT         transfer point from UCS into WCS
UTRA_pt_ucs2wcs         transform point constructionPlane to absolut

UT3D_m3_multm3          der Nutzen ist unbekannt ....

UTRA_dump__             dump translation-data

UTRA_app_obj     DO NOT USE  apply transf. to object (ObjGX-tree)
UTRA_app_oTab    DO NOT USE  apply transf. to ObjGX-table

List_functions_end:
=====================================================
- see also:
../xa/test_3D_to_2D.c         Test worldCoords (3D) -> constructionPlane
UT3D_m3_load
UT3D_m3_load_o
UT3D_m3_get
UT3D_m3_multm3

\endcode *//*----------------------------------------

geom. Transformations UTRA_

Translation    UTRA_def__
Rotation       UTRA_def__
Mirror         UTO_obj_appmir
Scale          TODO
Projection     UPRJ_*


UNUSED:
// UTRA_obj_m3__          transform obj (typ,struct) with matrix
// UTRA_vc_abs2rel__      transform absolut to constructionPlane
// UTRA_vc_rel2abs__      transform constructionPlane to absolut




Mat_4x3:
        VX             VY            VZ        Nullpunkt

  [0][0]=vx.dx   [0][1]=vy.dx  [0][2]=vz.dx   [0][3]=o.x
  [1][0]=vx.dy   [1][1]=vy.dy  [1][2]=vz.dy   [1][3]=o.y
  [2][0]=vx.dz   [2][1]=vy.dz  [2][2]=vz.dz   [2][3]=o.z
--------------------------------------------------------


- usage examples:

  // TRANSLATION:            (Obj verschieben)
  UT3D_vc_3db (&vc1, 15.,0.,5.);    // define a transl.vector
  UTRA_def__ (1, Typ_VC, &vc1);  // init translation-function
  UTRA_app__ ..


  // Z-ROTATION:
  // ptc ist Drehmittelpunkt (oder NULL)
  UTRA_inirotZ_pt_angr (&tr1, &ptc, UT_RADIANS(45.));
  UTRA_def__ (1, Typ_TraRot, &tr1);
  UTRA_app__ ..


  // freie ROTATION:           (Obj drehen)
  // Drehachse pt0, vc0
  TraRot tr1;
  UTRA_inirot_pt_vc_angr (&tr1, &pt0, &vc0, UT_RADIANS(30.));
  UTRA_def__ (1, Typ_TraRot, &tr1);
  UTRA_app__ ..


  // TRANSFORMATION:  (obj von einer Plane in Nullage transportieren   oder
  //                  aus dem HauptRefSys in ein Refsys PlaneX transportieren)
  UT3D_m3_loadpl (m1, pl1);      // trMat m1 zu Plane pl1 generieren
  UT3D_m3_invm3 (mi1, m1);       // inv. trMat zu m1 generieren
  // mi1: obj von einer Plane in Nullage transformieren
  // m1: obj aus Nullage in Refsys m1 (pl1) transportieren
  UTRA_def__ (1, Typ_M4x3, mi1);
  UTRA_app__ (datSpc1, .. Typ_LN, ln2, NULL);



// Example Usage UTRA_app__  (eine struct transformieren)
  char  auxObj[OBJ_SIZ_MAX];
    oSiz = OBJ_SIZ_MAX;
    UME_init (&wrk1Spc, memspc101, sizeof(memspc101));  // translated points
    irc = UTRA_app__ (auxObj, &oSiz, Typ_LN, 1, &ln1, &wrk1Spc);


// Example Usage UTRA_app__  (ein ObjGX-Obj transformieren)
//    zu transformieren ist ObjGX *ox1;
    if(translate == ON) {
      UME_init (&wrk1Spc, memspc101, sizeof(memspc101));  // translated points
      ox2 = (ObjGX*)memspc102;   // ObjGX(Tabelle) out
      oSiz = sizeof(memspc102);
      irc = UTRA_app_obj (ox2, &oSiz, ox1, wrk1Spc);
      if(irc < 0) continue;      // Error
      ox1 = (ObjGX*)memspc102;   // replace pointer 
    }




Typ_VC
 -) nur Verschiebung

Typ_TraRot:
 1) Vom Origin in den Nullpunkt verschieben (ptIn -= Origin)
 2) drehen (ptOut = Rotation(ptIn))
 3) zurueckverschieben  (ptOut += Origin)

Typ_M4x3: 
 -) transformieren (ptOut = Transformation(ptIn));


-----------------------------------------------
*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// #include "../ut/ut_umem.h"

#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_ox_base.h"          // OGX_SET_INDEX
#include "../ut/ut_TX.h"               // TX_Error

#include "../db/ut_DB.h"               // DB_GetObjGX

#include "../ut/func_types.h"               // UI_Func... SYM_..

#include "../xa/xa.h"                  // CONSTRPLN_IS_OFF



//===========================================================================
// EXTERNALS:

// ../xa/xa.c:
extern int       WC_sur_ind;            // Index auf die ActiveConstrPlane
extern Mat_4x3   WC_sur_mat;            // TrMat of ActiveConstrPlane
extern Mat_4x3   WC_sur_imat;           // inverse TrMat of ActiveConstrPlane


// ../ci/NC_Main.c:
extern int     APT_obj_stat;       // 0=permanent, 1=temporary (workmode)


//===========================================================================
// LOCALS:

// static int (*UTRA_pt_tr)(Point*, Point*); // UTRA_pt_tratra od UTRA_pt_trarot


static int TRA_NR = 0;

#define TRA_TAB_SIZ  10
// Typ aktuell definierter Transformationen
static int    TRA_TYP[TRA_TAB_SIZ];

static TraRot TRA_TAB[TRA_TAB_SIZ];



//===========================================================================
// Prototypen:
int UTRA_app_pt (Point*, Point*);
int UTRA_app_vc (Vector*, Vector*);


//================================================================
  int UTRA_dump__ (char *inf) {
//================================================================
/// UTRA_dump__             dump translation-data

  int   i1;

  printf("=========== UTRA_dump__  %s %d ===========\n",inf,TRA_NR);

  for(i1=0; i1<TRA_NR; ++i1) {
    printf(" TRA_TYP[%d] = %d\n",i1,TRA_TYP[i1]);
    DEB_dump_obj__ (Typ_TraRot, &TRA_TAB[i1], "");
  }

  printf("=========== End UTRA_dump__ ===========\n");

  return 0;

}


//================================================================
  int UTRA_app_ox (ObjGX *ox2, ObjGX *ox1, Memspc *wrkSpc) {
//================================================================
/// UTRA_app_ox             apply Transformation to complex-obj


  int      irc, fTyp;
  long     l1;
  void     *poo;


  // printf("UTRA_app_ox \n");
  UTRA_dump__ ("UTRA_app_ox");  // disp translObj
  // DEB_dump_ox_0 (ox1, "ox1");


  // is output a single data-record or a ox
  fTyp = UTO_ck_dbsTyp (ox1->form);  // 0=struct(D,P,L,C); 1=oGX(S,N,A,B);
     printf(" fTyp=%d\n",fTyp);

  if(!fTyp) {
    // std-struct; VPDLCRMI not SABNT

    // get obj-spc in wrkSpc
    l1 = OBJ_SIZ_MAX;
    poo = UME_reserve (wrkSpc, l1);

    // transform
    irc = UTRA_app__ (poo, ox1->typ, ox1->form, ox1->siz, ox1->data, wrkSpc);
      // DEB_dump_obj__ (ox1->form, poo, "poo irc=%d",irc);

    // std-struct; make ObjGX from obj
    OGX_SET_OBJ (ox2, ox1->typ, ox1->form, ox1->siz, poo);

  } else {
    // struct; SABNT not VPDLCRMI
    // 1=already ObjGX
    *ox2 = *ox1;  // copy ObjGX
    // transform all childs
    irc = UTRA_app__ (ox2->data, ox1->typ, ox1->form, ox1->siz, ox1->data, wrkSpc);
  }


  return irc;

}


/* UNUSED
//================================================================
  Vector UTRA_vc_abs2rel__ (Vector *vcAbs) {
//================================================================
// transform from world to userCoordsystem (absolut to constructionPlane)
// TODO !
/
    if(CONSTRPLN_IS_ON) {
      UT3D_pt_tra_pt_m3 (&ln_out->p1, WC_sur_mat, &pta);
      UT3D_vc_tra_vc_m3 (&vc1, WC_sur_mat, &vc1);
if(CONSTRPLN_IS_ON) UT3D_vc_tra_vc_m3 (&vc1, WC_sur_imat, &vc1);
// der Punkt ist absolutKoordinaten; umrechnen in relative Koordinaten
if(CONSTRPLN_IS_ON) UT3D_pt_tra_pt_m3 (&pt1, WC_sur_imat, &pt1);
WC_sur_ind WC_sur_act
WC_sur_mat WC_sur_imat
/
  Vector vcRel;


  vcRel = *vcAbs;

  return (vcRel);

}
*/
/* UNUSED
//================================================================
  Vector UTRA_vc_rel2abs__ (Vector *vcRel) {
//================================================================
// transform from userCoordsystem to world  (constructionPlane to absolut)
// TODO !
/
    if(CONSTRPLN_IS_ON) {
      UT3D_pt_tra_pt_m3 (&ln_out->p1, WC_sur_mat, &pta);
      UT3D_vc_tra_vc_m3 (&vc1, WC_sur_mat, &vc1);
if(CONSTRPLN_IS_ON) UT3D_vc_tra_vc_m3 (&vc1, WC_sur_imat, &vc1);
// der Punkt ist absolutKoordinaten; umrechnen in relative Koordinaten
if(CONSTRPLN_IS_ON) UT3D_pt_tra_pt_m3 (&pt1, WC_sur_imat, &pt1);
WC_sur_ind WC_sur_act
WC_sur_mat WC_sur_imat
/
  Vector vcAbs;


  vcAbs = *vcRel;

  return (vcAbs);

}
*/

//================================================================
  int UTRA_def__ (int trNr, int typ, void *data) {
//================================================================
/// \code
/// UTRA_def__             define transformation
/// 
/// Input:
///   trNr    immer mit 1 beginnen; max TRA_TAB_SIZ.
///   typ     Typ_VC      data must be: Vector        (Translation)
///           Typ_TraRot                TraRot        (Rotation)
///           Typ_M4x3                Mat_4x3       (Transformation)
///
/// Example:
///   UTRA_def__ (1, Typ_VC, &vc1);
///   UTRA_app__ (objo, oTyp, .. oDat, memSeg);
/// \endcode

  TraRot  *tr1;


  // printf("UTRA_def__ %d %d\n", trNr, typ);


  if(trNr < 1) {TX_Error("UTRA_def__ E002_%d",trNr); return -1;}
  if(trNr > TRA_TAB_SIZ) {
    TX_Error("UTRA_def__ E003_%d max=%d",trNr,TRA_TAB_SIZ);
    return -1;
  }



  tr1 = &TRA_TAB[trNr-1];


  //----------------------------------------------------------------
  if(typ == Typ_VC) {                 // Translation
    // UTRA_pt_tr = UTRA_app_pt;
    tr1->vz = *(Vector*)data;         // copy Translation-Vector
   

  //----------------------------------------------------------------
  } else if(typ == Typ_TraRot) {           // Rotation
    // UTRA_pt_tr = UTRA_app_pt;
    *tr1 = *(TraRot*)data;


  //----------------------------------------------------------------
  } else if(typ == Typ_M4x3) {           // Transformation
    // UTRA_pt_tr = UTRA_app_pt;
    memcpy (tr1->ma, data, sizeof(Mat_4x3));
      // DEB_dump_obj__ (Typ_M4x3, ma, "UTRA_def__ TraMat\n");


  //----------------------------------------------------------------
  } else {TX_Error("UTRA_def__ E001_%d",typ); return -1;}


  TRA_TYP[trNr-1] = typ;
  TRA_NR = trNr;

  return 0;

}


//================================================================
  int UTRA_nobj_tra (ObjGX *oao, int oNr, ObjGX *oai,
                    Memspc *objSeg, Memspc *tmpSeg) {
//================================================================
/// \code
/// UTRA_nobj_tra          apply transf. to ObjGX-Table
/// nur die oNr primaeren ObjGX nach oo kopieren, alle daten nach SpcObj.
/// Die Transformation muss mit UTRA_def__ geladen werden.
/// see UTO_ox_tra UTRA_app_obj
/// \endcode


  int    i1, iAct, iForm, iNr, oSiz;
  ObjGX  *oo,  *oi;

  // DEB_dump_ox_0 (oai, "UTRA_nobj_tra:");

  for(iAct=0; iAct<oNr; ++iAct) {

    // copy primary ObjGX
    memcpy(&oao[iAct], &oai[iAct], sizeof(ObjGX));

    oi = &oai[iAct];
    oo = &oao[iAct];

    iForm = oi->form;
    iNr = oi->siz;

    switch (iForm) {

      case Typ_PT:
        oSiz = sizeof(Point);
        // get space for the dataStruct
        oo->data = UME_reserve (objSeg, iNr * oSiz);
        if(oo->data == NULL) return -1;
        // copy the dataStruct
        for(i1=0; i1<iNr; ++i1)
          UTRA_app_pt (&((Point*)oo->data)[i1], &((Point*)oi->data)[i1]);
        break;


      case Typ_LN:
        oSiz = sizeof(Line);
        // get space for the dataStruct
        oo->data = UME_reserve (objSeg, iNr * oSiz);
        if(oo->data == NULL) return -1;
        // copy the dataStruct
        for(i1=0; i1<iNr; ++i1) {
          UTRA_app_pt (&((Line*)oo->data)[i1].p1, &((Line*)oi->data)[i1].p1);
          UTRA_app_pt (&((Line*)oo->data)[i1].p2, &((Line*)oi->data)[i1].p2);
        }
        break;


      case Typ_CI:
        oSiz = sizeof(Circ);
        // get space for the dataStruct
        oo->data = UME_reserve (objSeg, iNr * oSiz);
        if(oo->data == NULL) return -1;
        // copy the dataStruct
        for(i1=0; i1<iNr; ++i1) {
          ((Circ*)oo->data)[i1].rad = ((Circ*)oi->data)[i1].rad;
          ((Circ*)oo->data)[i1].ango = ((Circ*)oi->data)[i1].ango;
          UTRA_app_pt (&((Circ*)oo->data)[i1].pc, &((Circ*)oi->data)[i1].pc);
          UTRA_app_pt (&((Circ*)oo->data)[i1].p1, &((Circ*)oi->data)[i1].p1);
          UTRA_app_pt (&((Circ*)oo->data)[i1].p2, &((Circ*)oi->data)[i1].p2);
          UTRA_app_vc (&((Circ*)oo->data)[i1].vz, &((Circ*)oi->data)[i1].vz);
        }
        break;

      default:
        TX_Error("UTRA_app__ E001 %d",iForm);
        return -1;
    }
  }

  // DEB_dump_ox_0 (oao, "ex UTRA_nobj_tra:");


  return 0;

}


/* UNUSED
//============================================================================
  int UTRA_app_obj (void *objo, long *oSiz, ObjGX *obji, Memspc *wrkSpc) {
//============================================================================
/// \code
/// apply predefined Transformation to complex-obj
/// init Transformation mit UTRA_def__ erforderlich !!!
/// Storage:
///   Normale structs werden nach objo ausgegeben;
///   ObjGX-Records: ObjGX-Records werden nach objo ausgegeben; die
///     zugehoerigen Daten (structs) werden in wrkSpc gespeichert.
///
/// Input:
///   oSiz   size of objo; wird recursiv verkleinert !
///   obji   (ObjGX*)  (NUR (ObjGX*) moeglich !)
/// Output:
///   objo   ObjGX, 
///   oSiz   freier Restplatz in objo
///   wrkSpc fuer Daten (structs) von oGX-Objekten
/// 
/// see UTO_obj_tra_obj_m3 oder UTO_ox_tra
/// \endcode

  int   irc, iForm;
  long  oldSiz;
  char  *pi;


  printf("UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU \n");
  printf("UTRA_app_obj oSiz=%ld\n",*oSiz);
  // printf("         obji-typ=%d form=%d siz=%d\n",((ObjGX*)obji)->typ,
            // ((ObjGX*)obji)->form,((ObjGX*)obji)->siz);
  // DEB_dump_ox_s_ (obji, "UTRA_app_obj in");
  DEB_dump_ox_0 (obji, "UTRA_app_obj in");



  iForm = ((ObjGX*)obji)->form;


  // duplicate/isolate object; dataStructAddresses bleiben !
  oldSiz = *oSiz;
  // isolate (change dbi-links into structs)
  // irc = UTO_isol__ (objo, oSiz, obji);
  // make a copy of all data in objo; 
  irc = OGX_ox_copy__ (objo, oSiz, obji, 1);
  if(irc < 0) return -1;


  pi = objo;
  // (char*)objo +=  oldSiz - *oSiz;
  objo =  (char*)objo + (oldSiz - *oSiz);

    // DEB_dump_ox_s_ (pi, "Obj isolated");
    // DEB_dump_ox_0 (pi, "Obj isolated");
    // UTO_obj_Disp__ (pi, &wrkSpc, 2, SYM_STAR_S);


  // oberste struct ist ein ObjGX;
  irc = UTRA_app_oTab (objo, oSiz, pi, wrkSpc);


    DEB_dump_ox_0 (pi, "UTRA_app_obj out");
    // DEB_dump_ox_s_ (pi, "UTRA_app_obj out");

  return irc;
}


//============================================================================
  int UTRA_app_oTab (void *objo, long *oSiz, void *obji, Memspc *wrkSpc) {
//============================================================================
/// \code
/// DO NOT USE - use UTRA_app__
/// apply predefined Transformation to complex-obj
/// Transformation must have been defined with UTRA_def__
/// TODO: provide type of output-obj or return always ObjGX ?
/// Input:
///   oSiz   size of objo; wird recursiv verkleinert !
///   obji    complex-object (ObjGX-Record)
///   memSpc   memspc for Typ_CVPOL and Typ_CVBSP; else can be NULL
/// Output:
///   objo   ObjGX, 
///   oSiz   freier Restplatz in objo
///   wrkSpc fuer Daten (structs) von oGX-Objekten
///
/// see  DB_store_ox
/// \endcode


  int    irc, i1, iTyp, iForm, iSiz, siz;
  long   oldSiz;
  char   *pi;


  iSiz  = ((ObjGX*)obji)->siz;


  // printf("UTRA_app_oTab %ld\n",*oSiz);
  // printf("         obji-typ=%d form=%d siz=%d\n",((ObjGX*)obji)->typ,
            // ((ObjGX*)obji)->form,iSiz);


  iTyp  = ((ObjGX*)obji)->typ;
  iForm = ((ObjGX*)obji)->form;
  pi    = ((ObjGX*)obji)->data;


  if(iForm != Typ_ObjGX) {
    oldSiz = *oSiz;
    irc = UTRA_app__ (objo, iTyp, iForm, iSiz, pi, wrkSpc);
    if(irc < 0) return -1;
    ((ObjGX*)obji)->data = objo;
    return 0;
  }


  // ist ein ObjGX;
  // loop tru obj and translate ..
  for(i1=0; i1<iSiz; ++i1) {
    iTyp  = ((ObjGX*)pi)->typ;
    iForm = ((ObjGX*)pi)->form;
      // printf(" tra2[%d] %d %d\n",i1,iTyp,iForm);

    if(iTyp == Typ_Typ) goto L_GX_nxt; // hat keine data !!

    if(iForm == Typ_ObjGX) { // recurse
      oldSiz = *oSiz;
      irc = UTRA_app_oTab (objo, oSiz, pi, wrkSpc);
      if(irc < 0) return -1;
      // (char*)objo +=  oldSiz - *oSiz;
      objo =  (char*)objo + (oldSiz - *oSiz);


    // normale struct: transl.
    } else {
      oldSiz = *oSiz;
      siz = ((ObjGX*)pi)->siz;
      irc = UTRA_app__ (objo, iTyp, iForm, siz, ((ObjGX*)pi)->data, wrkSpc);
      if(irc < 0) return -1;
      ((ObjGX*)pi)->data = objo;
      // (char*)objo +=  oldSiz - *oSiz;
      objo =  (char*)objo + (oldSiz - *oSiz);
    }


    L_GX_nxt:
    pi += sizeof(ObjGX);
  }


  return 0;

}
*/


//============================================================================
  int UTRA_app_dbo (long *dbi, int *typ, int ipr, Memspc *wrkSpc) {
//============================================================================
// UTRA_app_dbo    transform DB-object (typ,DB-ind) and store in DB
// Returns new (dynamic) index in dbi.
// Transform objects from inside a struct (eg SurRev axis or contour)
// Input:
//   dbi   DB-index of obj to transform and to store
//   typ   dbtyp
//   ipr      1=create PRCV, 0=not;
//   wrkSpc   memspc for Typ_CVPOL and Typ_CVBSP; else can be NULL
// Output:
//   dbi   DB-index of new stored obj (dyn.obj!)
//   typ   exact type of curve or surf

  int     irc, i1, oNr, oTyp, fTyp, sTyp, form;
  long    oSiz;
  void    *opo, *opi, *mSpc, *vp1;
  // char    tmpSpc[OBJ_SIZ_MAX];
  ObjGX   ox2;


  // printf("DDDDDDDDDDDDDDDDDDDDDDDDDDDD UTRA_app_dbo typ=%d dbi=%ld\n",*typ,*dbi);
  // DBO_dump__ (*typ,*dbi);


  mSpc = UME_get_next (wrkSpc);  // get free pos in wrkSpc; not yet reserve.


  // get a ObjGX of the obj to transform
  ox2 = DB_GetObjGX (*typ, *dbi);
  if(ox2.typ == Typ_Error) return -1;

    // TESTBLOCK
    // DEB_dump_obj__ (Typ_ObjGX, &ox2, "pp_dbo-f-app__-ox2-IN");
    // END TESTBLOCK


  // get the space for the transformed objects 
  oSiz = UTO_siz_stru(ox2.form);
  opo = (void *) MEM_alloc_tmp (oSiz * ox2.siz);


  // get type of struct 0=VPDLCRMI, 1=SABNT (UTO_ck_dbsTyp)
  // fTyp = UTO_ck_dbsTyp (ox2.form);
    // printf(" app_dbo-oSiz=%ld fTyp=%d\n",oSiz,fTyp);


  //----------------------------------------------------------------
  irc = UTRA_app__ (opo, ox2.typ, ox2.form, ox2.siz, ox2.data, wrkSpc);
  if(irc < 0) return irc;

    // TESTBLOCK
    // vp1 = opo;
    // for(i1=0;i1<ox2.siz;++i1) {
    // DEB_dump_obj__ (ox2.form, opo, "pp_dbo-f-UTRA_app__-tmpSpc-cpx[%d]",i1);
    // vp1 = (char*)vp1 + oSiz; }
    // END TESTBLOCK


  //----------------------------------------------------------------
  *dbi = -1L;
  irc = DB_store_obj (&vp1, ox2.typ, ox2.form, opo, ox2.siz, dbi);
  if(irc < 0) return irc;

  if(ipr) {
    // create PRCV
    irc = PRCV_set_obj_dbi (ox2.form, opo, 1, ox2.typ, *dbi);
    if(irc < 0) return irc;
  }

  // restore wrkSpc
  UME_set_next (mSpc, wrkSpc);

    // printf("ddddddddddd ex-UTRA_app_dbo irc=%d typ=%d dbi=%ld\n",irc,*typ,*dbi);

  return 0;

}


//==========================================================================
  int UTRA_app_CCV (CurvCCV *objo, CurvCCV *obji, int ipr, Memspc *wrkSpc) {
//==========================================================================
// UTRA_app_CCV          transform a single CCV
// Input:
//   ipr      1=create PRCV, 0=not;
//   wrkSpc   for additional data, eg pTab for Polygon
// Output:
//   objo     outputobj (a single CurvCCV; data can be in wrkSpc)


  int    irc, typ;
  long   dbi;
  Point  pt1;


  // printf("UTRA_app_CCV ipr=%d\n",ipr);
  // DEB_dump_obj__ (Typ_CVTRM, obji, "UTRA_app_CCV-obji:");


  memcpy(objo, obji, sizeof(CurvCCV));


  // transform basicCurve typ.dbi
  // printf("UTRA_app_CCV typ=%d\n",obji->typ);
  // DEB_dump_obj__ (Typ_CVTRM, obji, "in");


  if(obji->dbi) {
    // lines do not have dbi (basic-curve)
    typ = obji->typ;
    // transform DB-obj & store in DB
    irc = UTRA_app_dbo (&objo->dbi, &typ, ipr, wrkSpc);
      // printf(" _app_CCV-UTRA_app_dbo irc=%d dbi=%ld typ=%d\n",irc,objo->dbi,typ);
      // DBO_dump__ (typ, objo->dbi);
    if(irc < 0) return -1;
  }


  // transform startpoint if one exists
  if(obji->ip0) {
    // transform point, change point-index
    UTRA_app_pt (&pt1, DB_get_PT(obji->ip0));
    objo->ip0 = DB_StorePoint (-1L, &pt1);
      // printf(" ip0-1=%ld ip0-2=%ld\n",obji->ip0,objo->ip0);
  }

  // transform endpoint if one exists
  if(obji->ip1) {
      // printf(" ip1-1=%ld\n",objo->ip1);
    UTRA_app_pt (&pt1, DB_get_PT(obji->ip1));
    objo->ip1 = DB_StorePoint (-1L, &pt1);
      // printf(" ip1-1=%ld ip0-2=%ld\n",obji->ip1,objo->ip1);
  }

  // reset stat (no PRCV yet)
  objo->stat = 0;

    // printf("cccccccccccccc ex-UTRA_app_CCV\n"); fflush (stdout);

  return 0;

}


//===========================================================================
  int UTRA_app__ (void *objo,
                  int otyp, int oform, int iNr, void *obji, Memspc *wrkSpc) {
//===========================================================================
/// UTRA_app_out            apply transformation, copy into outputspace
/// Input:
///   objo       space for transformed objs - size must be (size-of-oform * iNr)
///   otyp       is the type of struct of obji
///   oform      type of obji
///   obji       struct-type of obji
///   iNr        nr of structs in obji
/// Output:
///   objo       struct of type otyp/oform, data in wrkSpc
///   RetCod     0=OK, -1=error


  int     irc, i1, ls;
  void    *pooo, *poi;


  // printf("AAAAAAAAAAAAAAAAAAA UTRA_app__ otyp=%d oform=%d iNr=%d\n",otyp,oform,iNr);

  ls = UTO_siz_stru(oform);

  pooo = objo;

  for(i1=0; i1<iNr; ++i1) {
      // DEB_dump_obj__ (oform, obji, "UTRA_app__-obji[%d] ::::::::::::\n",i1);
    // transform (UTRA_app_obj); returns a pointer into wrkSpc
    irc = UTRA_app_obj (&poi, oform, 1, obji, wrkSpc);
    if(irc < 0) return irc;

      // TESTBLOCK
      // DEB_dump_obj__ (oform, poi, "after-UTRA_app_obj i1=%d",i1);
      // END TESTBLOCK

    // copy  from wrkSpc -> objo
    memcpy (pooo, poi, ls);

    pooo = (char*)pooo + ls;
    obji = (char*)obji + ls;
  }


    // TESTBLOCK
    // pooo = objo;
    // for(i1=0; i1<iNr; ++i1) {
      // DEB_dump_obj__ (oform, pooo, "after-UTRA_app_obj [%d]",i1);
      // pooo = (char*)pooo + ls;}
    // printf("aaaaaaaaaaa ex-UTRA_app__ otyp=%d oform=%d iNr=%d\n",otyp,oform,iNr);
    // END TESTBLOCK


  return 0;

}


//============================================================================
  int UTRA_app_obj (void **poo, int otyp, int iNr, void *obji, Memspc *wrkSpc) {
//============================================================================
/// \code
/// apply Transformation to struct; NOT object. 
/// init Transformation before with UTRA_def__
/// returns pointer to the transformed object,
///   the memspace for the output-object poo is reserved in wrkSpc
///  
/// Input:
///   otyp     form of obji;
///   obji     struct(s) to be transformed. Typ_Index=Typ_ObjGX
///   iNr      nr of structs to transform (obji has <iNr> structs of typ <otyp>)
///   wrkSpc   memspc for data
/// Output:
///   poo      pointer to obj with new coordinates; obj-typ=otyp; spc in wrkSpc 
///   RetCod   0=OK; -1=EOM
///
/// see UTO_obj_tra_obj_m3 oder UTO_ox_tra
/// TODO: add parameter outTyp (now: outTyp == inTyp).
/// \endcode

// see also UTRA_obj_abs2rel__


  int       irc, i1, i2, i3, i4, typ, dbTyp, iForm, iSiz, rSiz;
  long      l1, dbi, oldSiz;
  char      *pi, *po, *po1, *pw;
  Point     *pa1, *pa2, p1;
  void      *o1, *opi, *opo, *objo;
  ObjGX     *ox1, *ox2, *oxi, *oxo;


  // printf("UUUUUUUUUUUUUUUUUUUUUUUUUUU UTRA_app_obj otyp=%d iNr=%d\n",otyp,iNr);
  // UME_dump (wrkSpc, "wrkSpc:");
  // DEB_dump_obj__ (otyp, obji, "obji:");
    // geht ned ... DEB_dump_ox_0 (obji, "UTRA_app__");


  irc = 0;
  if(wrkSpc) {
    objo = UME_get_next (wrkSpc); // get position of free space; no reserve yet
    *poo = objo;
  } else {
    objo = *poo;
  }


  switch (otyp) {

    //================================================================
    case Typ_VAR:                     // copy only
      rSiz = sizeof(double);
      goto L_copy;


    //================================================================
    case Typ_PT:
      if(wrkSpc) {
        if(UME_add (wrkSpc, sizeof(Point) * iNr) < 0) goto L_EOM;
      }

      L_PT_nxt:
      UTRA_app_pt (objo, obji);
      if(iNr > 1) {
        --iNr;
        obji = (char*)obji + sizeof(Point);
        objo = (char*)objo + sizeof(Point);
        goto L_PT_nxt;
      }
      break;


    //================================================================
    case Typ_PT2:
      if(wrkSpc) {
        if(UME_add (wrkSpc, sizeof(Point2) * iNr) < 0) goto L_EOM;
      }

      L_PT2_nxt:
      UTRA_app_pt2 (objo, obji);
      if(iNr > 1) {  
        --iNr;
        obji = (char*)obji + sizeof(Point2);
        objo = (char*)objo + sizeof(Point2);
        goto L_PT_nxt;
      }
      break;


    //================================================================
    case Typ_VC:                    // copy only
      rSiz = sizeof(Vector);
      goto L_copy;


    //================================================================
    case Typ_LN:
      if(iNr > 1) goto L_E_INR;
      if(wrkSpc) {
        if(UME_add (wrkSpc, sizeof(Line)) < 0) goto L_EOM;
      }

      ((Line*)objo)->typ = ((Line*)obji)->typ;
      UTRA_app_pt (&((Line*)objo)->p1, &((Line*)obji)->p1);
      UTRA_app_pt (&((Line*)objo)->p2, &((Line*)obji)->p2);
      break;


    //================================================================
    case Typ_LN2:
      if(iNr > 1) goto L_E_INR;
      if(wrkSpc) {
        if(UME_add (wrkSpc, sizeof(Line2)) < 0) goto L_EOM;
      }

      UTRA_app_pt2 (&((Line2*)objo)->p1, &((Line2*)obji)->p1);
      UTRA_app_pt2 (&((Line2*)objo)->p2, &((Line2*)obji)->p2);
      break;



    //================================================================
    case Typ_CI2:
      if(iNr > 1) goto L_E_INR;
      if(wrkSpc) {
        if(UME_add (wrkSpc, sizeof(Circ2)) < 0) goto L_EOM;
      }

        // DEB_dump_obj__ (Typ_CI, obji, "trACi:");
      // copy vz & rad & ango
      *(Circ2*)objo = *(Circ2*)obji;
      // transl pc
      UTRA_app_pt2 (&((Circ2*)objo)->pc, &((Circ2*)obji)->pc);
      // transl p1
      UTRA_app_pt2 (&((Circ2*)objo)->p1, &((Circ2*)obji)->p1);

      if(UT2D_ck_ci360((Circ2*)obji) == 0)
        ((Circ2*)objo)->p2 = ((Circ2*)objo)->p1;  // 360 deg: copy p1
      else      // transl p2
        UTRA_app_pt2 (&((Circ2*)objo)->p2, &((Circ2*)obji)->p2);

        // DEB_dump_obj__ (Typ_CI, objo, "trACo:");
      break;


    //================================================================
    case Typ_CI:
      if(iNr > 1) goto L_E_INR;
      if(wrkSpc) {
        if(UME_add (wrkSpc, sizeof(Circ)) < 0) goto L_EOM;
      }

      UTRA_tra_ci (objo, obji);
      break;


    //================================================================
    case Typ_GTXT:
      if(iNr > 1) goto L_E_INR;
      if(wrkSpc) {
        if(UME_add (wrkSpc, sizeof(GText)) < 0) goto L_EOM;
      }

      memcpy(objo, obji, sizeof(GText));

      // UT3D_pt_tra_pt_m3 ((Point*)pe, trmat, &tx1->pt);
      UTRA_app_pt (&((GText*)objo)->pt, &((GText*)obji)->pt);
      break;



    //================================================================
    case Typ_Dimen:
      if(iNr > 1) goto L_E_INR;
      if(wrkSpc) {
        if(UME_add (wrkSpc, sizeof(Dimen)) < 0) goto L_EOM;
      }

      memcpy(objo, obji, sizeof(Dimen));

      // UT3D_pt_tra_pt_m3 ((Point*)pe, trmat, &tx1->pt);
      UTRA_app_pt2 (&((Dimen*)objo)->p1, &((Dimen*)obji)->p1);
      UTRA_app_pt2 (&((Dimen*)objo)->p2, &((Dimen*)obji)->p2);
      UTRA_app_pt2 (&((Dimen*)objo)->p3, &((Dimen*)obji)->p3);
      break;



    //================================================================
    case Typ_PLN:
      if(iNr > 1) goto L_E_INR;
      if(wrkSpc) {
        if(UME_add (wrkSpc, sizeof(Plane)) < 0) goto L_EOM;
      }

      UTRA_tra_pln (objo, obji);
      break;


    //================================================================
    case Typ_CVELL:
      if(iNr > 1) goto L_E_INR;
      if(wrkSpc) {
        if(UME_add (wrkSpc, sizeof(CurvElli)) < 0) goto L_EOM;
      }

      UTRA_tra_ell (objo, obji);
      break;


    //================================================================
    case Typ_CVPOL:
      if(iNr > 1) goto L_E_INR;
      if(!UME_reserve (wrkSpc, sizeof(CurvPoly))) goto L_EOM;
      irc = UTRA_tra_plg (objo, obji, wrkSpc);
      break;


    //================================================================
    case Typ_CVBSP:
      if(iNr > 1) goto L_E_INR;
      if(UME_add (wrkSpc, sizeof(CurvBSpl)) < 0) goto L_EOM;
      // if(wrkSpc == NULL) goto L_E002;
      // *oSiz -= sizeof(CurvBSpl);
      // if(*oSiz < 0) goto L_EOM;
      irc = UTRA_tra_cbsp (objo, obji, wrkSpc);
      break;


    //================================================================
    // case Typ_CVPSP3:
      // if(iNr > 1) goto L_E_INR;
      // *oSiz -= sizeof(polynom_d3) * iNr;
      // if(*oSiz < 0) goto L_EOM;
      // UTRA_tra_? (objo, obji);
      // break;

/*
    else if(typ == Typ_CVPSP3) {
    pola = cvi->data;

    UT3D_pt_tra_pt_m3 (&pt0, trmat, &UT3D_PT_NUL);

    for(i1=0; i1<cvi->siz; ++i1) {
      pol1 = pola[i1];

      pt1.x = pol1.x.a;
      pt1.y = pol1.y.a;
      pt1.z = pol1.z.a;
      UT3D_pt_tra_pt_m3 (&pt1, trmat, &pt1);
      pol1.x.a = pt1.x;
      pol1.y.a = pt1.y;
      pol1.z.a = pt1.z;

      pt1.x = pol1.x.b;
      pt1.y = pol1.y.b;
      pt1.z = pol1.z.b;
      UT3D_pt_tra_pt_m3 (&pt1, trmat, &pt1);
      pol1.x.b = pt1.x - pt0.x;
      pol1.y.b = pt1.y - pt0.y;
      pol1.z.b = pt1.z - pt0.z;

      pt1.x = pol1.x.c;
      pt1.y = pol1.y.c;
      pt1.z = pol1.z.c;
      UT3D_pt_tra_pt_m3 (&pt1, trmat, &pt1);
      pol1.x.c = pt1.x - pt0.x;
      pol1.y.c = pt1.y - pt0.y;
      pol1.z.c = pt1.z - pt0.z;

      pt1.x = pol1.x.d;
      pt1.y = pol1.y.d;
      pt1.z = pol1.z.d;
      UT3D_pt_tra_pt_m3 (&pt1, trmat, &pt1);
      pol1.x.d = pt1.x - pt0.x;
      pol1.y.d = pt1.y - pt0.y;
      pol1.z.d = pt1.z - pt0.z;

      memcpy(pe, &pol1, sizeof(polynom_d3));
      pe += sizeof (polynom_d3);
    }
*/


    //================================================================
    case Typ_CVTRM:
      // must provide space for poo in wrkSpc
      if(!UME_reserve(wrkSpc,sizeof(CurvCCV) * iNr) < 0) goto L_EOM;
        // printf(" copy CVCCV\n");

      L_CCV_nxt:
      // create PRCV if permanent
      UTRA_app_CCV (objo, obji, ICHG01(APT_obj_stat), wrkSpc);
      if(iNr > 1) {
        --iNr;
        objo = (char*)objo + sizeof(CurvCCV);
        obji = (char*)obji + sizeof(CurvCCV);
        goto L_CCV_nxt;
      }
        // printf("  app_dbo-f-app-CCV\n");
      break;


    //================================================================
    // SURFACES
    //================================================================
    case Typ_SURSTRIP:
      if(iNr > 1) goto L_E_INR;
      if(!UME_reserve (wrkSpc, sizeof(SurStripe))) goto L_EOM;
      // translate points
      i1 = ((SurStripe*)obji)->ptUNr * ((SurStripe*)obji)->ptVNr;
      oxi = ((SurStripe*)obji)->pTab;
      UTRA_app_obj (&opo, Typ_ObjGX, i1, oxi, wrkSpc);
      // copy Sur
      *(SurStripe*)objo = *(SurStripe*)obji;
      // change address
      ((SurStripe*)objo)->pTab = opo;
      break;


    //================================================================
    case Typ_SURRV:    // SurRev
      if(iNr > 1) goto L_E_INR;
      if(UME_add (wrkSpc, sizeof(SurRev)) < 0) goto L_EOM;
      // *oSiz -= sizeof(SurRev);
      // if(*oSiz < 0) goto L_EOM;

      // copy SurRev
      *(SurRev*)objo = *(SurRev*)obji;

      // translate Axis typCen/indCen
      typ = ((SurRev*)obji)->typCen;
      dbi = ((SurRev*)obji)->indCen;
      irc = UTRA_app_dbo (&dbi, &typ, 1, wrkSpc);
      if(irc < 0) goto L_EOM;
      ((SurRev*)objo)->indCen = dbi;

      // translate Contour typCov/indCov
      typ = ((SurRev*)obji)->typCov;
      dbi = ((SurRev*)obji)->indCov;
      irc = UTRA_app_dbo (&dbi, &typ, 1, wrkSpc);
      if(irc < 0) goto L_EOM;
      ((SurRev*)objo)->indCov = dbi;

      // DEB_dump_obj__ (Typ_SURRV, objo, "SurRV-out\n");
      break;



    //================================================================
    case Typ_SURBSP:
      if(iNr > 1) goto L_E_INR;
      if(UME_add (wrkSpc, sizeof(SurBSpl)) < 0) goto L_EOM;
      // if(wrkSpc == NULL) goto L_E002; // need spc for points
      // *oSiz -= sizeof(SurBSpl);
      // if(*oSiz < 0) goto L_EOM;

      // copy SurBSpl
      *(SurBSpl*)objo = *(SurBSpl*)obji;
      // address kvTabU u kvTabV bleiben unveraendert !
      // reserve space for the points
      i2 = ((SurBSpl*)obji)->ptUNr;
      i3 = ((SurBSpl*)obji)->ptVNr;
      i4 = i2 * i3;
      pa1 = ((SurBSpl*)obji)->cpTab;      // from-adress
      pa2 = wrkSpc->next;                  // to-address
      if(UME_add (wrkSpc, i4 * sizeof(Point)) < 0) return -1;
      // transform points
      for(i1=0; i1<i4; ++i1) {
        UTRA_app_pt (&pa2[i1], &pa1[i1]);
          // GR_tDyn_symB__ (&pa2[i1], SYM_TRI_S, 2);
      }
      ((SurBSpl*)objo)->cpTab = pa2;     // change address
      break;



    //================================================================
    // BODIES
    //================================================================
    case Typ_CON:
      if(iNr > 1) goto L_E_INR;
      if(UME_add (wrkSpc, sizeof(Conus)) < 0) goto L_EOM;
      // if(wrkSpc == NULL) goto L_E002; // need spc for points
        // DEB_dump_obj__ (Typ_CON, obji, "trCONi:");
      // *oSiz -= sizeof(Conus);
      // if(*oSiz < 0) goto L_EOM;
      // copy the whole struct (keep r r h )
      memcpy(objo, obji, sizeof(Conus));
      // transform plane
      opi = &((Conus*)obji)->pl;
      opo = &((Conus*)objo)->pl;
      UTRA_app_pt (&((Plane*)opo)->po, &((Plane*)opi)->po);
      UTRA_app_vc (&((Plane*)opo)->vx, &((Plane*)opi)->vx);
      UTRA_app_vc (&((Plane*)opo)->vy, &((Plane*)opi)->vy);
      UTRA_app_vc (&((Plane*)opo)->vz, &((Plane*)opi)->vz);
        // DEB_dump_obj__ (Typ_CON, objo, "trCONo:");
      break;



    //================================================================
    case Typ_SPH:
      if(iNr > 1) goto L_E_INR;
      if(UME_add (wrkSpc, sizeof(Sphere)) < 0) goto L_EOM;
      // if(wrkSpc == NULL) goto L_E002; // need spc for points
        // DEB_dump_obj__ (Typ_SPH, obji, "trSPHi:");
      // *oSiz -= sizeof(Sphere);
      // if(*oSiz < 0) goto L_EOM;
      // copy the whole struct (keep r r h )
      memcpy(objo, obji, sizeof(Sphere));
      // transform center
      UTRA_app_pt (&((Sphere*)objo)->pc, &((Sphere*)obji)->pc);
        // DEB_dump_obj__ (Typ_SPH, objo, "trSPHo:");
      break;



    //================================================================
    case Typ_TOR:
      if(iNr > 1) goto L_E_INR;
      if(UME_add (wrkSpc, sizeof(Torus)) < 0) goto L_EOM;
      // if(wrkSpc == NULL) goto L_E002; // need spc for points
        // DEB_dump_obj__ (Typ_TOR, obji, "trTORi:");
      // *oSiz -= sizeof(Torus);
      // if(*oSiz < 0) goto L_EOM;
      // copy the whole struct (keep r r h )
      memcpy(objo, obji, sizeof(Torus));
      // transform plane
      opi = &((Torus*)obji)->pl;
      opo = &((Torus*)objo)->pl;
      UTRA_app_pt (&((Plane*)opo)->po, &((Plane*)opi)->po);
      UTRA_app_vc (&((Plane*)opo)->vx, &((Plane*)opi)->vx);
      UTRA_app_vc (&((Plane*)opo)->vy, &((Plane*)opi)->vy);
      UTRA_app_vc (&((Plane*)opo)->vz, &((Plane*)opi)->vz);
        // DEB_dump_obj__ (Typ_TOR, objo, "trTORo:");
      break;



    //================================================================
    case Typ_Model:      // struct = ModelRef
      if(iNr > 1) goto L_E_INR;
      if(UME_add (wrkSpc, sizeof(ModelRef)) < 0) goto L_EOM;
      // *oSiz -= sizeof(ModelRef);
      // if(*oSiz < 0) goto L_EOM;

      memcpy(objo, obji, sizeof(ModelRef));

      // UT3D_pt_tra_pt_m3 ((Point*)pe, trmat, &tx1->pt);
      UTRA_app_pt (&((ModelRef*)objo)->po, &((ModelRef*)obji)->po);
      UTRA_app_vc (&((ModelRef*)objo)->vx, &((ModelRef*)obji)->vx);
      UTRA_app_vc (&((ModelRef*)objo)->vz, &((ModelRef*)obji)->vz);
      break;


    //================================================================
    case Typ_ObjGX:
    case Typ_CV:
    case Typ_SURRU:
        // printf(" app_obj-ObjGX(CV/SURRU\n");
        // fflush (stdout);

      ox1 = (ObjGX*)obji;
      oxo = (ObjGX*)objo;


      // get space
      if(UME_add (wrkSpc, sizeof(ObjGX) * iNr) < 0) goto L_EOM;
      // copy all records
      memcpy (objo, obji, sizeof(ObjGX) * iNr);

         // TESTBLOCK
         // printf(" otyp=%d iNr=%d\n",otyp,iNr);
         // DEB_dump_ox_0 (objo, "obj to tra");
         // for(i1=0; i1 < iNr; ++i1)
         // DEB_dump_obj__ (Typ_ObjGX, &ox1[i1], "obj-to-tra[%d]",i1);
         // END TESTBLOCK


      for(i1=0; i1 < iNr; ++i1) {  // Recursion !
        // UTRA_app__ cannot get typ&form&data; resolv links now !
        oxi = &((ObjGX*)obji)[i1];
          // DEB_dump_obj__ (Typ_ObjGX, &ox1[i1], "ox-tra[%d]",i1);
        if(oxi->typ == Typ_Typ) continue;    // already copied

        if(oxi->form == Typ_Index) {
          OGX_GET_INDEX (&dbTyp, &dbi, oxi);
          // skip V,D
          if((dbTyp == Typ_VC)    ||
             (dbTyp == Typ_VAR))        continue;
          // transform DB-object (typ,DB-ind) and store in DB
          irc = UTRA_app_dbo (&dbi, &dbTyp, 0, wrkSpc);
          if(irc < 0) goto L_EOM;
          // store new dbi in objo
          OGX_SET_INDEX (&oxo[i1], dbTyp, dbi);
          continue;
        }
          // printf(" _APP_X ObjGX typ=%d ??????????????????\n",oxi->form);
        // point (eg for RCIR):  recurse ..
        UTRA_app_obj ((void**)&ox2, oxi->form, oxi->siz, oxi->data, wrkSpc);
        // oxo = &((ObjGX*)objo)[i1];
        // oxo->data = ox2;
        oxo[i1].data = ox2;
          // DEB_dump_obj__ (Typ_ObjGX, oxo, "obj-tra[%d]",i1);
      }
        // printf("ex-app-ox\n");
      break;


    //================================================================
    default:
      TX_Error("UTRA_app_obj E001 %d",otyp);
      return -1;
  }


    // printf("uuuuuuuuuuuuuuuu ex-UTRA_app_obj %d \n",irc); fflush (stdout);
    // if(!irc)DEB_dump_obj__ (otyp, objo, "ex UTRA_app__");
    // // if(!irc)GR_tDyn_obj (otyp, objo, 8, 4);
    // printf("ex UTRA_app_obj irc=%d aaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n",irc);


  return irc;


  //----------------------------------------------------------------
  L_copy:
    // copy iNr records of size rSiz
    if(iNr < 2) {
      memcpy (objo, obji, rSiz);
    } else {
      for(i1=0; i1 < iNr; ++i1) {
        memcpy (objo, obji, rSiz);
        objo = ((char*)objo + rSiz);
        obji = ((char*)obji + rSiz);
      }
    }


  L_E002:
    TX_Error("UTRA_app_obj E002 - no workspace for %d",otyp);
    return -1;

  L_EOM:
    TX_Error("UTRA_app_obj EOM");
    return -1;

  L_E_INR:
    TX_Error("UTRA_app_obj E_INR %d %d",otyp,iNr);
    return -1;


  err_not_yet:
   TX_Error("UTRA_app_obj function not yet implemented");
      return -1;
}


//================================================================
  int UTRA_app_pt (Point *p2, Point *p1) {
//================================================================
/// apply all defined Transformations to a point.

  int   i1;

  *p2 = *p1;

  for(i1=0; i1<TRA_NR; ++i1) {
    UTRA_tra_pt (p2, p2, TRA_TYP[i1], &TRA_TAB[i1]);
  }

    // GR_tDyn_symB__ (p2, SYM_STAR_S, 2);

  return 0;

}

//================================================================
  int UTRA_app_pt2 (Point2 *p2, Point2 *p1) {
//================================================================
/// apply all defined Transformations to a 2D-point.

  int   i1;

  *p2 = *p1;

  for(i1=0; i1<TRA_NR; ++i1) {
    UTRA_tra_pt2 (p2, p2, TRA_TYP[i1], &TRA_TAB[i1]);
  }

    // GR_tDyn_symB__ (p2, SYM_STAR_S, 2);

  return 0;

}


//================================================================
  int UTRA_tra_pt2 (Point2 *p2, Point2* p1, int typ, TraRot *tra) {
//================================================================
/// \code
/// apply a single Transformation to a 2D-point.
/// see also UT3D_pt_tra_pt_m3 UTRA_pt_traptm3
/// \endcode

  double   x, y;
  Vector   *vc1;
  Mat_4x3  *ma;


  // DEB_dump_obj__ (Typ_PT, p1, "UTRA_tra_pt %d",typ);


  //----------------------------------------------------------------
  if(typ == Typ_VC) {                 // Translation

    vc1 = &tra->vz;

    p2->x = p1->x + vc1->dx;
    p2->y = p1->y + vc1->dy;



  //----------------------------------------------------------------
  } else if(typ == Typ_TraRot) {           // Rotation

    ma = &tra->ma;

    x = p1->x - (*ma)[0][3];
    y = p1->y - (*ma)[1][3];

    p2->x = (*ma)[0][0]*x + (*ma)[0][1]*y + (*ma)[0][3];
    p2->y = (*ma)[1][0]*x + (*ma)[1][1]*y + (*ma)[1][3];



  //----------------------------------------------------------------
  } else if(typ == Typ_M4x3) {           // Transformation

    ma = &tra->ma;

    x = p1->x;
    y = p1->y;

    p2->x = (*ma)[0][0]*x + (*ma)[0][1]*y + (*ma)[0][3];
    p2->y = (*ma)[1][0]*x + (*ma)[1][1]*y + (*ma)[1][3];



  //----------------------------------------------------------------
  } else {TX_Error("UTRA_tra_pt E001_%d",typ); return -1;}



  // printf("ex UTRA_tra_pt %f,%f,%f",p2->x,p2->y,p2->z);

  return 0;

}


//================================================================
  int UTRA_tra_pt (Point *p2, Point* p1, int typ, TraRot *tra) {
//================================================================
/// \code
/// apply a single Transformation to a point.
/// see also UT3D_pt_tra_pt_m3 UTRA_pt_traptm3
/// \endcode

  double   x, y, z;
  Vector   *vc1;
  Mat_4x3  *ma;


  // DEB_dump_obj__ (Typ_PT, p1, "UTRA_tra_pt %d",typ);

  
  //----------------------------------------------------------------
  if(typ == Typ_VC) {                 // Translation

    vc1 = &tra->vz;

    p2->x = p1->x + vc1->dx;
    p2->y = p1->y + vc1->dy;
    p2->z = p1->z + vc1->dz;



  //----------------------------------------------------------------
  } else if(typ == Typ_TraRot) {           // Rotation

    ma = &tra->ma;

    x = p1->x - (*ma)[0][3];
    y = p1->y - (*ma)[1][3];
    z = p1->z - (*ma)[2][3];

    p2->x = (*ma)[0][0]*x + (*ma)[0][1]*y + (*ma)[0][2]*z + (*ma)[0][3];
    p2->y = (*ma)[1][0]*x + (*ma)[1][1]*y + (*ma)[1][2]*z + (*ma)[1][3];
    p2->z = (*ma)[2][0]*x + (*ma)[2][1]*y + (*ma)[2][2]*z + (*ma)[2][3];



  //----------------------------------------------------------------
  } else if(typ == Typ_M4x3) {           // Transformation

    ma = &tra->ma;

    x = p1->x;
    y = p1->y;
    z = p1->z;

    p2->x = (*ma)[0][0]*x + (*ma)[0][1]*y + (*ma)[0][2]*z + (*ma)[0][3];
    p2->y = (*ma)[1][0]*x + (*ma)[1][1]*y + (*ma)[1][2]*z + (*ma)[1][3];
    p2->z = (*ma)[2][0]*x + (*ma)[2][1]*y + (*ma)[2][2]*z + (*ma)[2][3];



  //----------------------------------------------------------------
  } else {TX_Error("UTRA_tra_pt E001_%d",typ); return -1;}



  // printf("ex UTRA_tra_pt %f,%f,%f",p2->x,p2->y,p2->z);

  return 0;

}


//================================================================
  int UTRA_app_vc (Vector *v2, Vector* v1) {
//================================================================
/// apply all defined Transformations to a Vector.

  int   i1;
 

  // DEB_dump_obj__ (Typ_VC, v1, "UTRA_app_vc");

  *v2 = *v1;

  for(i1=0; i1<TRA_NR; ++i1) {
    // printf(" _app_vc %d %d\n",i1,TRA_TYP[i1]);
    UTRA_tra_vc (v2, v2, TRA_TYP[i1], &TRA_TAB[i1]);
  }

    // GR_tDyn_vc__ (v2, NULL, 0, 0);
    // DEB_dump_obj__ (Typ_VC, v2, "ex-UTRA_app_vc");

  return 0;

}


//================================================================
  int UTRA_tra_vc (Vector *v2, Vector* v1, int typ, TraRot *tra) {
//================================================================
/// apply a single Transformation to a Vector.


  double   x, y, z;
  Vector   *vc1;
  Mat_4x3  *ma;


  // DEB_dump_obj__ (Typ_VC, v1, "UTRA_tra_vc %d",typ);


  //----------------------------------------------------------------
  if(typ == Typ_VC) {                 // Translation

    // Translation: nur kopieren;
    memcpy(v2, v1, sizeof(Vector));


  //----------------------------------------------------------------
  } else if((typ == Typ_TraRot) ||           // Rotation
            (typ == Typ_M4x3)) {           // Transformation

    ma = &tra->ma;

    x = v1->dx;
    y = v1->dy;
    z = v1->dz;

    v2->dx = (*ma)[0][0]*x + (*ma)[0][1]*y + (*ma)[0][2]*z;
    v2->dy = (*ma)[1][0]*x + (*ma)[1][1]*y + (*ma)[1][2]*z;
    v2->dz = (*ma)[2][0]*x + (*ma)[2][1]*y + (*ma)[2][2]*z;


  //----------------------------------------------------------------
  } else {TX_Error("UTRA_tra_vc E001_%d",typ); return -1;}

  // printf("ex UTRA_tra_vc %f,%f,%f",p2->x,p2->y,p2->z);

  return 0;

}


/*
//================================================================
  int UTRA_pt_traptm3 (Point *p2, Point* p1) {
//================================================================
// UTRA_pt_traptm3           apply transformation to point (from 4x3-matrix)

// p2 und p1 duerfen ident sein



  double x, y, z;

  // printf("UTRA_pt_traptm3\n");
  // DEB_dump_obj__ (Typ_PT, p1, "UTRA_pt_traptm3");


  // Rotate: normalize (--> origin)
  if(traAct == Typ_TraRot) {
    x = p1->x - ma[0][3];
    y = p1->y - ma[1][3];
    z = p1->z - ma[2][3];

  // Transform: nicht in den origin verschieben ..
  } else {
    x = p1->x;
    y = p1->y;
    z = p1->z;
  }


  // printf("UTRA_pt_traptm3    %f,%f,%f",x,y,z);

  p2->x = ma[0][0]*x + ma[0][1]*y + ma[0][2]*z + ma[0][3];
  p2->y = ma[1][0]*x + ma[1][1]*y + ma[1][2]*z + ma[1][3];
  p2->z = ma[2][0]*x + ma[2][1]*y + ma[2][2]*z + ma[2][3];


  // printf("ex UTRA_pt_traptm3 %f,%f,%f",p2->x,p2->y,p2->z);
  return 0;

}


//================================================================
  int UTRA_pt_travcm3 (Vector *p2, Vector *p1) {
//================================================================

  double x, y, z;

    x = p1->dx;
    y = p1->dy;
    z = p1->dz;

    p2->dx = ma[0][0]*x + ma[0][1]*y + ma[0][2]*z;
    p2->dy = ma[1][0]*x + ma[1][1]*y + ma[1][2]*z;
    p2->dz = ma[2][0]*x + ma[2][1]*y + ma[2][2]*z;

  return 0;

}


//================================================================
  int UTRA_tra_vc (Vector *p2, Vector *p1) {
//================================================================
// was  UT3D_vc_rotvcm3

  double x, y, z;

  // DEB_dump_obj__ (Typ_VC, p1, "UTRA_tra_vc-vci:");


  //----------------------------------------------------------------
  if(traAct == Typ_VC)  {
    // Translation: kopieren;
    memcpy(p2, p1, sizeof(Vector));



  //----------------------------------------------------------------
  } else if((traAct == Typ_TraRot)  ||
            (traAct == Typ_M4x3))    {

    UTRA_pt_travcm3 (p2, p1);



  //----------------------------------------------------------------
  } else {
    TX_Error("UTRA_tra_vc E001_%d",traAct);
    return -1;
  }


    // DEB_dump_obj__ (Typ_VC, p2, "UTRA_tra_vc-vco:");

  return 0;
}
*/


//================================================================
  int UTRA_inirotZ_pt_angr (TraRot *tr1, Point *ptOri, double ar) {
//================================================================
/// \code
/// UTRA_inirotZ_pt_angr       define rotation around z-axis (center, angle)
///   ptOri      centerpoint of rotation; NULL=centerposition
///   ar         angle in radians
/// \endcode


  // DEB_dump_obj__ (Typ_PT, ptOri, "UTRA_inirotZ_pt_angr ");

  UT3D_m3_iniZrot_angr (tr1->ma, ptOri, ar);

  tr1->angr = ar;
  tr1->vz   = UT3D_VECTOR_Z;

    // DEB_dump_obj__ (Typ_M4x3, *ma1, "UTRA_inirotZ_pt_angr: ma\n");

  return 0;

}


//=====================================================================
  int UTRA_inirot_pt_vc_angr (TraRot *tr1, Point *pa, Vector *va, double ar) {
//=====================================================================
/// transformation from rotationaxis (pa, va) and angle (ar)
// 
// Der RotationOrigin steckt in der ma in ma[0][3], ma[1][3], ma[2][3].
// Extrahieren Via Funktion UT3D_m3_get (&ptOri, 3, trr.ma);


  UT3D_m3_inirot_angr (tr1->ma, pa, va, ar);

  tr1->angr = ar;
  tr1->vz   = *va;  // copy vec

  // DEB_dump_obj__ (Typ_M4x3, *ma1, "UTRA_inirot_pt_vc_angr: ma\n");


  return 0;

}


//=====================================================================
  void UTRA_m3_multm3 (Mat_4x3 m3, Mat_4x3 ma, Mat_4x3 mb) {
//=====================================================================
/// UT3D_m3_multm3             4x3-Matrix mit 4x3 Matrix multiplizieren
///  (nur den 3x3-Anteil)

//  der Nutzen ist unbekannt ....
// 
//  Test:
//   // erste ist mi1 (von einer Plane in Nullage transportieren)
//   UTRA_inirotZ_pt_angr (&tr1, NULL, UT_RADIANS(5.)); // zweite; eine Rotation
//   UTRA_m3_multm3 (mb, mi1, tr1.ma);
//   UTRA_def__ (1, Typ_M4x3, mb);
//   UTRA_app__ ..

// see also UT3D_m3_multm3 UT3D_m4_addrot

//  Eine Matrix mal ihrer inversen Matrix ergibt Einheitsmatrix



    int           i, zeile, spalte, i1;

  // DEB_dump_obj__ (Typ_M4x3, ma, "mult: ma\n");
  // DEB_dump_obj__ (Typ_M4x3, mb, "mult: mb\n");

/*
  for( i=0; i<3; i++ ) {
      m3[i][0] = ma[i][0] * mb[0][0] +
                 ma[i][1] * mb[1][0] +
                 ma[i][2] * mb[2][0];
      m3[i][1] = ma[i][0] * mb[0][1] +
                 ma[i][1] * mb[1][1] +
                 ma[i][2] * mb[2][1];
      m3[i][2] = ma[i][0] * mb[0][2] +
                 ma[i][1] * mb[1][2] +
                 ma[i][2] * mb[2][2];
  }
*/

/*
    for (zeile = 0; zeile < 3; zeile++)   {
        for (spalte = 0; spalte < 3; spalte++)  {
            m3[zeile][spalte] = 0.0;
            for (i1 = 0; i1 < 3; i1++) {
                m3[zeile][spalte] += (ma[zeile][i1] * mb[i1][spalte]);
            }
        }
    }
*/

  // i  j      i  -       -  j
  m3[0][0]= ma[0][0] * mb[0][0] +      // i=0 j=0
            ma[0][1] * mb[1][0] +
            ma[0][2] * mb[2][0];

  m3[1][0]= ma[1][0] * mb[0][0] +      // i=1 j=0
            ma[1][1] * mb[1][0] +
            ma[1][2] * mb[2][0];

  m3[2][0]= ma[2][0] * mb[0][0] +      // i=2 j=0
            ma[2][1] * mb[1][0] +
            ma[2][2] * mb[2][0];


  m3[0][1]= ma[0][0] * mb[0][1] +      // i=0 j=1
            ma[0][1] * mb[1][1] +
            ma[0][2] * mb[2][1];

  m3[1][1]= ma[1][0] * mb[0][1] +      // i=1 j=1
            ma[1][1] * mb[1][1] +
            ma[1][2] * mb[2][1];

  m3[2][1]= ma[2][0] * mb[0][1] +      // i=2 j=1
            ma[2][1] * mb[1][1] +
            ma[2][2] * mb[2][1];


  m3[0][2]= ma[0][0] * mb[0][2] +      // i=0 j=2
            ma[0][1] * mb[1][2] +
            ma[0][2] * mb[2][2];

  m3[1][2]= ma[1][0] * mb[0][2] +      // i=1 j=2
            ma[1][1] * mb[1][2] +
            ma[1][2] * mb[2][2];

  m3[2][2]= ma[2][0] * mb[0][2] +      // i=2 j=2
            ma[2][1] * mb[1][2] +
            ma[2][2] * mb[2][2];


/*
  // i  j      i  -       -  j
  m3[0][0]= ma[0][0] * mb[0][0] +      // i=0 j=0
            ma[0][1] * mb[1][0] +
            ma[0][2] * mb[2][0];

  m3[0][1]= ma[1][0] * mb[0][0] +      // i=1 j=0
            ma[1][1] * mb[1][0] +
            ma[1][2] * mb[2][0];

  m3[0][2]= ma[2][0] * mb[0][0] +      // i=2 j=0
            ma[2][1] * mb[1][0] +
            ma[2][2] * mb[2][0];


  m3[1][0]= ma[0][0] * mb[0][1] +      // i=0 j=1
            ma[0][1] * mb[1][1] +
            ma[0][2] * mb[2][1];

  m3[1][1]= ma[1][0] * mb[0][1] +      // i=1 j=1
            ma[1][1] * mb[1][1] +
            ma[1][2] * mb[2][1];

  m3[1][2]= ma[2][0] * mb[0][1] +      // i=2 j=1
            ma[2][1] * mb[1][1] +
            ma[2][2] * mb[2][1];


  m3[2][0]= ma[0][0] * mb[0][2] +      // i=0 j=2
            ma[0][1] * mb[1][2] +
            ma[0][2] * mb[2][2];

  m3[2][1]= ma[1][0] * mb[0][2] +      // i=1 j=2
            ma[1][1] * mb[1][2] +
            ma[1][2] * mb[2][2];

  m3[2][2]= ma[2][0] * mb[0][2] +      // i=2 j=2
            ma[2][1] * mb[1][2] +
            ma[2][2] * mb[2][2];
*/


  m3[0][3]= ma[0][3] + mb[0][3];
  m3[1][3]= ma[1][3] + mb[1][3];
  m3[2][3]= ma[2][3] + mb[2][3];



  // DEB_dump_obj__ (Typ_M4x3, m3, "mult: m3\n");

}


//================================================================
  int UTRA_obj2_obj3__ (void **o2, int *typ2, void *o3, int typ3,
                      Memspc *ma) {
//================================================================
/// \code
/// UTRA_obj2_obj3__              get 2D-obj from 3D-obj-in-plane
/// removes Z-coordinate; must have z == dpd
///   eg get curves from 3D-constructionPlane to 2D
/// Input:
///   ma          required only for CurvBSpl;
///               else set o2 to spc of size>=OBJ_SIZ_MAX and ma=NULL
/// \endcode

  int      i1, i2, irc;
  Point2   *p2a, *p2;
  void     *bo;


  // printf("UTRA_obj2_obj3__ %d\n",typ3);
  // DEB_dump_obj__(typ3, o3, "  o3:");

  switch (typ3) {

    case Typ_PT:
      *typ2 = Typ_PT2;
      if(ma) *o2 = UME_reserve (ma, sizeof(Point2));
      *(Point2*)*o2 = UT2D_pt_pt3 ((Point*)o3);
      break;

      case Typ_LN:
      *typ2 = Typ_LN2;
      if(ma) *o2 = UME_reserve (ma, sizeof(Line2));
      ((Line2*)*o2)->typ = ((Line*)o3)->typ;
// UT2D_ln_ln3
      ((Line2*)*o2)->p1 = UT2D_pt_pt3 (&((Line*)o3)->p1);
      ((Line2*)*o2)->p2 = UT2D_pt_pt3 (&((Line*)o3)->p2);
      break;

    case Typ_VC:
      *typ2 = Typ_VC2;
      if(ma) *o2 = UME_reserve (ma, sizeof(Vector2));
      UT2D_vc_vc3 ((Vector2*)*o2, (Vector*)o3);
      break;
  
    case Typ_CI:
      *typ2 = Typ_CI2;
      if(ma) *o2 = UME_reserve (ma, sizeof(Circ2));
      UT2D_ci_ci3 (*o2, o3);
      break;

    case Typ_CVELL:
      *typ2 = Typ_CVELL2;
      if(ma) *o2 = UME_reserve (ma, sizeof(CurvEll2));
      ((CurvEll2*)*o2)->p1 = UT2D_pt_pt3 (&((CurvElli*)o3)->p1);
      ((CurvEll2*)*o2)->p2 = UT2D_pt_pt3 (&((CurvElli*)o3)->p2);
      ((CurvEll2*)*o2)->pc = UT2D_pt_pt3 (&((CurvElli*)o3)->pc);
      UT2D_vc_vc3 (&((CurvEll2*)*o2)->va, &((CurvElli*)o3)->va);
      UT2D_vc_vc3 (&((CurvEll2*)*o2)->vb, &((CurvElli*)o3)->vb);
      ((CurvEll2*)*o2)->ango = ((CurvElli*)o3)->ango;
      ((CurvEll2*)*o2)->srot = ((CurvElli*)o3)->srot;
      ((CurvEll2*)*o2)->clo = ((CurvElli*)o3)->clo;
      ((CurvEll2*)*o2)->trm = ((CurvElli*)o3)->trm;
      break;

    case Typ_CVBSP:
      *typ2 = Typ_CVBSP2;
// UT2D_bsp_tra_bsp3_m3
      *o2 = UME_reserve (ma, sizeof(CurvBSpl2));
      memcpy (*o2, o3, sizeof(CurvBSpl2));
      i2 = ((CurvBSpl*)o3)->ptNr;
      p2a = UME_reserve (ma, i2 * sizeof(Point2));
      for(i1=0; i1 < i2; ++i1)
        p2a[i1] = UT2D_pt_pt3 (&((CurvBSpl*)o3)->cpTab[i1]);
      ((CurvBSpl2*)*o2)->cpTab = p2a;
      break;

    case Typ_CVTRM:
      // get std-obj from trimmed-curve
      bo = UME_reserve (ma, OBJ_SIZ_MAX);
      irc = UTO_cv_cvtrm (&typ3, bo, NULL, o3);
      if(irc < 0) return -1;
        // DEB_dump_obj__(typ3, bo, "  bo:");
      // recurse
      irc = UTRA_obj2_obj3__ (o2, typ2, bo, typ3, ma);
      if(irc < 0) return -1;
      break;




    default:
      TX_Error("UTRA_obj2_obj3__: Typ %d unsupported",typ3);
        DEB_dump_obj__(typ3, o3, "  o3:");
      return -1;
  }


    // DEB_dump_obj__(*typ2, *o2, "ex UTRA_obj2_obj3__:");

  return 0;

}


//================================================================
  int UTRA_obj_abs2rel__ (void *robj,
                          int typ, void *aobj, Memspc *memDat1) {
//================================================================
// get obj from 3D-absolut onto 3D-constructionPlane
// if objects are on a refsys, it must be active.
/// robj     must have appropriate size (OBJ_UNKNOWN or UTO_siz_stru(typ))
/// Input:
///   oSiz     (free) size of objo;
///   memspc   memspc for Typ_CVPOL and Typ_CVBSP; else can be NULL
/// Output:
///   rbjo     obj with new coordinates; obj-typ=typ;
///              size: same as aobj or OBJ_SIZ_MAX bytes
///   oSiz     remaining free size of objo;


// Refsys: see .irs in dispList.

// see also UTRA_app__
// see UTRA_pt_abs2rel__


  if(CONSTRPLN_IS_OFF) {
    memcpy (robj, aobj, UTO_siz_stru(typ));
    return 0;
  }

  // return UTRA_obj_m3__ (robj, typ, aobj, WC_sur_imat, memDat1);
  UTRA_def__ (1, Typ_M4x3, WC_sur_imat);
  return UTRA_app__ (robj, typ, typ, 1, aobj, memDat1);

}

/* UNUSED
//=========================================================================
  int UTRA_obj_m3__ (void *robj,
                       int typ, void *aobj, Mat_4x3 mat, Memspc *memDat1) {
//=========================================================================
// get obj from 3D-absolut onto 3D-constructionPlane
// if objects are on a refsys, it must be active.
// memDat1  can be NULL except for Typ_CVPOL and Typ_CVBSP

// Refsys: see .irs in dispList.

// see also UTRA_app__
// see UTRA_pt_abs2rel__


  int     i1,  i2;
  Point   *pa1, *pa2;


  printf("UTRA_obj_abs2rel__ typ=%d iref=%d\n",typ,WC_sur_ind);
  DEB_dump_obj__(typ, aobj, "  aobj:");
exit(0);


  switch (typ) {

    case Typ_PT:
      UT3D_pt_tra_pt_m3 (robj, mat, aobj);
      break;

    case Typ_LN:
      UT3D_pt_tra_pt_m3 (&((Line*)robj)->p1, mat, &((Line*)aobj)->p1);
      UT3D_pt_tra_pt_m3 (&((Line*)robj)->p2, mat, &((Line*)aobj)->p2);
      break;

    case Typ_CI:
      *(Circ*)robj = *(Circ*)aobj;  // copy vz & rad & ango
      UT3D_pt_tra_pt_m3 (&((Circ*)robj)->pc, mat, &((Circ*)aobj)->pc);
      UT3D_pt_tra_pt_m3 (&((Circ*)robj)->p1, mat, &((Circ*)aobj)->p1);
      if(UT3D_ck_ci360((Circ*)aobj) == 0)
        ((Circ*)robj)->p2 = ((Circ*)robj)->p1;  // 360 deg: copy p1
      else
        UT3D_pt_tra_pt_m3 (&((Circ*)robj)->p2, mat, &((Circ*)aobj)->p2);
      UT3D_vc_tra_vc_m3 (&((Circ*)robj)->vz, mat, &((Circ*)aobj)->vz);
      break;

    case Typ_CVELL:
      *(CurvElli*)robj = *(CurvElli*)aobj;  // copy vz & rad & ango
      UT3D_pt_tra_pt_m3 (&((CurvElli*)robj)->pc, mat, &((CurvElli*)aobj)->pc);
      UT3D_pt_tra_pt_m3 (&((CurvElli*)robj)->p1, mat, &((CurvElli*)aobj)->p1);
      if(UT3D_ck_el360((CurvElli*)aobj) == 0)
        ((CurvElli*)robj)->p2 = ((CurvElli*)robj)->p1;  // 360 deg: copy p1
      else
        UT3D_pt_tra_pt_m3 (&((CurvElli*)robj)->p2, mat, &((CurvElli*)aobj)->p2);
      UT3D_vc_tra_vc_m3 (&((CurvElli*)robj)->va, mat, &((CurvElli*)aobj)->va);
      UT3D_vc_tra_vc_m3 (&((CurvElli*)robj)->vb, mat, &((CurvElli*)aobj)->vb);
      UT3D_vc_tra_vc_m3 (&((CurvElli*)robj)->vz, mat, &((CurvElli*)aobj)->vz);
      break;

    case Typ_CVPOL:
      if(memDat1 == NULL) goto L_E002;
      *(CurvPoly*)robj = *(CurvPoly*)aobj;   // copy ptNr, v0, v1, *lvTab
      pa1 = ((CurvPoly*)aobj)->cpTab;
      // reserve Space for points in wrkSpc
      i2 = ((CurvPoly*)aobj)->ptNr;
      pa2 = UME_reserve (memDat1, sizeof(Point) * i2);
      if(!pa2) goto L_EOM;
      for(i1=0; i1<i2; ++i1)
        UT3D_pt_tra_pt_m3 (&pa2[i1], mat, &pa1[i1]);
      ((CurvPoly*)robj)->cpTab = pa2;
      // was ist mit der lvTab ? dzt nix !
      break;

    case Typ_CVBSP:
      if(memDat1 == NULL) goto L_E002;
      // copy BSP (ptnr, deg, v0, v1, *kvTab) --> objo
      *(CurvBSpl*)robj = *(CurvBSpl*)aobj;
      i2 = ((CurvBSpl*)aobj)->ptNr;
      // Controlpoints-from
      pa1 = ((CurvBSpl*)aobj)->cpTab;
      // reserve Space for i2 points in wrkSpc
      pa2 = UME_reserve (memDat1, sizeof(Point) * i2);
      if(!pa2) goto L_EOM;
      for(i1=0; i1<i2; ++i1)
        UT3D_pt_tra_pt_m3 (&pa2[i1], mat, &pa1[i1]);
      ((CurvBSpl*)robj)->cpTab = pa2;
      break;

    default:
      TX_Error("UTRA_obj_abs2rel__ unsupp. objTyp %d",typ);
      return -1;

  }


  L_exit:
    DEB_dump_obj__(typ, robj, "ex _abs2rel__:");
  return 0;


  L_E002:
    TX_Error("UTRA_obj_abs2rel__ E002 - no workspc");
    return -1;


  L_EOM:
    TX_Error("UTRA_obj_abs2rel__ EOM");
    return -1;

}
*/

//================================================================
  int UTRA_tra_pln (Plane *pl2, Plane *pl1) {
//================================================================
/// transform refSys pl1 with transformation (axis,angle)


    UTRA_app_vc (&pl2->vx, &pl1->vx);
    UTRA_app_vc (&pl2->vy, &pl1->vy);
    UTRA_app_vc (&pl2->vz, &pl1->vz);

    // move origin:
    UTRA_app_pt (&pl2->po, &pl1->po);

    // update p:
    UT3D_pl_p (pl2);

    // GR_tDyn_obj (Typ_PLN, pl2, 8, 4);
    // DEB_dump_obj__ (Typ_PLN, pl2, "pl2");

  return 0;

}


//===========================================================================
  int UTRA_inirot_2vc (TraRot *tra1, Point *pto, Vector *vc1, Vector *vc2) {
//===========================================================================
/// \code
/// UTRA_inirot_2vc         define rotation-transformation between 2 vectors
/// RetCod:  0     OK, transformation in ma1 defined;
///          1     vc1-vc2 are parallel (same direction); ma1 not defined.
///         -1     vc1-vc2 are antiparallel; ma1 not defined.
/// \endcode

// see also UT3D_m3_inirot_2vc


  double ao;
  Vector rx;

  // DEB_dump_obj__ (Typ_VC, vc1, "UTRA_inirot_2vc vc1");
  // DEB_dump_obj__ (Typ_VC, vc2, " vc2");

  // rotation-axis = crossprod vc1-vc2
  UT3D_vc_perp2vc (&rx, vc1, vc2);

  // check for parallel
  if(UT3D_compvcNull (&rx)) {
      printf(" vecs parall ...\n");
    return UT3D_sid_2vc (vc1, vc2);
  }

  // ao = opening-angle vc1 to vc2
  ao = UT3D_angr_2vc__ (vc1, vc2) * -1.;
    // printf(" ao=%lf\n",ao);

  UT3D_vc_setLength (&rx, &rx, 1.);
  UTRA_inirot_pt_vc_angr (tra1, pto, &rx, ao);

  return 0;

}


//================================================================
  int UTRA_obj_rel2abs__ (void *aobj,
                          int typ, void *robj, Memspc *memDat1) {
//================================================================
/// \code
/// put obj from 3D-constructionPlane to 3D-absolut
/// aobj must have appropriate size (OBJ_UNKNOWN or UTO_siz_stru(typ))
/// Input:
///   oSiz     (free) size of objo;
///   memspc   memspc for Typ_CVPOL and Typ_CVBSP; else can be NULL
/// Output:
///   abjo     obj with new coordinates; obj-typ=typ;
///              size: same as robj or OBJ_SIZ_MAX bytes
///   oSiz     remaining free size of objo;
/// \endcode
  
// Refsys: see .irs in dispList.

// see UTRA_pt_rel2abs__
  
  // printf("UTRA_obj_rel2abs__ %d\n",typ);
  // DEB_dump_obj__(typ, robj, "  robj:");

  
  if(CONSTRPLN_IS_OFF) {
    memcpy (aobj, robj, UTO_siz_stru(typ));
    return 0;
  }

  // return UTRA_obj_m3__ (aobj, typ, robj, WC_sur_mat, memDat1);
  UTRA_def__ (1, Typ_M4x3, WC_sur_mat);
  return UTRA_app__ (aobj, typ, typ, 1, robj, memDat1);

}


//================================================================
  int UTRA_pt_ucs2wcs (Point *pt1) {
//================================================================
/// translate point from constructionplane (relativ) to absolut (UCS -> WCS)
// see also UTRA_obj_rel2abs__

  if(WC_sur_ind)
  UT3D_pt_tra_pt_m3 (pt1, WC_sur_mat, pt1);

  return 0;

}


//================================================================
  int UTRA_pt_wcs2ucs (Point *pt1) {
//================================================================
/// translate point from absolut to relativ (constructionplane) (WCS -> UCS)
// see also UTRA_obj_abs2rel__

  if(WC_sur_ind)
  UT3D_pt_tra_pt_m3 (pt1, WC_sur_imat, pt1);

  return 0;

}


/*
//===========================================================================
  int UTRA_obj_coordSys_pln (void *objo, long *oSiz,
                             int typi, void *obji, int oNr,
                             Plane *pl1, int mode,  Memspc *memSpc) {
//===========================================================================
/// \code
/// UTRA_obj_coordSys_pln          change objCoords 3D->2D or 2D->3D
/// change coordinateSytem onto refSys from plane or back
/// get local coordinates of 3D-obj on refSys from 3D-object with world-coords
/// get world-coordinates of 3D-obj on local refSys from plane
/// Input:
///   oSiz     (free) size of objo;
///   Plane    new coordinate-system; NULL: active construction-plane
///   mode     1   get user-coordinates, local on pl1 (obji has world-coords)
///            2   get world-coordinates (obji is on pl1, has user-coordinates)
///   memSpc   memspc for Typ_CVPOL and Typ_CVBSP; else can be NULL
/// Output:
///   oSiz     remaining free size of objo;
///   objo     obj with new coordinates; obj-typ=typi; 
///   RetCod:  0 = OK;
///           -1 = out of memory;
/// \endcode

// see also UTRA_obj_m3__ UTRA_obj2_obj3__ UPRJ_app__

  // int        irc;
  Mat_4x3    ma, ima;


  DEB_dump_obj__ (typi, obji, "UTRA_obj_coordSys_pln");


  //----------------------------------------------------------------
  // define transformation
  if(!pl1) {
    // no plane given: use active construction-plane
    if(mode == 1) {
      UTRA_def__ (1, Typ_M4x3, WC_sur_imat);
    } else {
      UTRA_def__ (1, Typ_M4x3, WC_sur_mat);
    }

  } else {
    // get trMat from given plane
    UT3D_m3_loadpl (ma, pl1);
    if(mode == 1) {
      UT3D_m3_invm3 (ima, ma);           // invert trMat
      UTRA_def__ (1, Typ_M4x3, ima);
    } else {
      UTRA_def__ (1, Typ_M4x3, ma);
    }
  }


  return UTRA_app__ (objo, typi, typi, oNr, obji, memSpc);

}
*/


//================================================================
  int UTRA_tra_plg (CurvPoly *cvo, CurvPoly *cvi, Memspc *memSpc) {
//================================================================
// get memspc for points & knotvalues in memSpc

  int        i1;
  Point      *pai, *pao;


  // DEB_dump_obj__ (Typ_CVPOL, cvi, "UTRA_tra_plg");


  // copy curve
  *cvo = *cvi;      // copy ptNr
    // DEB_dump_obj__ (Typ_CVPOL, cvo, "    _app_plg");

  // reserve Space for pNr points in objSpc
  cvo->cpTab = UME_get_next (memSpc);
  if(!UME_reserve(memSpc, cvi->ptNr * sizeof(Point))) goto L_EOM;


  pai = cvi->cpTab;
  pao = cvo->cpTab;
  for(i1=0; i1<cvi->ptNr; ++i1) UTRA_app_pt (&pao[i1], &pai[i1]);

  // keep old lenghtTable
  // keep old v0, v1
  // see UPRJ_app_plg

  return 0;


  L_EOM:
    TX_Error("UTRA_tra_plg EOM");
    return -1;
}


//================================================================
  int UTRA_tra_cbsp (CurvBSpl *cvo, CurvBSpl *cvi, Memspc *memSpc) {
//================================================================
// get memspc for points & knotvalues in memSpc

  int        i1;
  Point      *pai, *pao;


  // DEB_dump_obj__ (Typ_CVBSP, cvi, "UTRA_tra_cbsp");

  // copy curve
  *cvo = *cvi;      // copy ptNr deg ..
    // DEB_dump_obj__ (Typ_CVBSP, cvo, "    _app_plg");

  // reserve Space for pNr points in objSpc
  cvo->cpTab = memSpc->next;
  if(UME_add (memSpc, cvi->ptNr * sizeof(Point)) < 0) goto L_EOM;

  pai = cvi->cpTab;
  pao = cvo->cpTab;
  for(i1=0; i1<cvi->ptNr; ++i1) UTRA_app_pt (&pao[i1], &pai[i1]);

  // keep kvTab, v0, v1 unmodified ..

  return 0;


  L_EOM:
    TX_Error("UTRA_tra_cbsp EOM");
    return -1;
}


//================================================================
  int UTRA_tra_ell (CurvElli *cvo, CurvElli *cvi) {
//================================================================


  // DEB_dump_obj__ (Typ_CVELL, cvi, "UTRA_tra_ell-in");

  // copy curve
  *cvo = *cvi;      // copy ptNr
    // DEB_dump_obj__ (Typ_CVELL, cvo, "    _app_ell");

  UTRA_app_pt (&cvo->pc, &cvi->pc);
  UTRA_app_pt (&cvo->p1, &cvi->p1);

  if(UT3D_ck_el360(cvi) == 0)
    cvo->p2 = cvo->p1;
  else  UTRA_app_pt (&cvo->p2, &cvi->p2);

  UTRA_app_vc (&cvo->va, &cvi->va);
  UTRA_app_vc (&cvo->vb, &cvi->vb);
  UTRA_app_vc (&cvo->vz, &cvi->vz);

    // DEB_dump_obj__ (Typ_CVELL, cvo, "ex UTRA_tra_ell");

  return 0;

}


//================================================================
  int UTRA_tra_ci (Circ *cvo, Circ *cvi) {
//================================================================

  // DEB_dump_obj__ (Typ_CI, cvi, "UTRA_tra_ci");


  // copy curve
  *cvo = *cvi;      // copy vz & rad & ango

  UTRA_app_pt (&cvo->pc, &cvi->pc);
  UTRA_app_pt (&cvo->p1, &cvi->p1);

  if(UT3D_ck_ci360(cvi) == 0)
    cvo->p2 = cvo->p1;
  else  UTRA_app_pt (&cvo->p2, &cvi->p2);

  UTRA_app_vc (&cvo->vz, &cvi->vz);
  // UT3D_vc_setLength (&cvo->vz, &cvo->vz, 1.);


  return 0;


  L_EOM:
    TX_Error("UTRA_tra_ci EOM");
    return -1;
}



// //----------------------------------------------------------------
// // UTRA_UCS_WCS_VC                      transfer vector from WCS into UCS
// // input is absolute; if constrPlane is active, transfer input into UCS
// void UTRA_UCS_WCS_VC (Vector* vco, Vector* vci) {
// // #define UTRA_UCS_WCS_VC(vco, vci) {
// 
//   DEB_dump_obj__ (Typ_VC, vci, "UTRA_UCS_WCS_VC ind=%d",WC_sur_ind);
// 
//   if(WC_sur_ind) UT3D_vc_tra_vc_m3 (vco, WC_sur_imat, vci);
//   else if(vco != vci) *vco = *vci;
// 
//   DEB_dump_obj__ (Typ_VC, vco, "ex-UTRA_UCS_WCS_VC");
// 
// }
// 
// // UTRA_UCS_WCS_PT                      transfer point from WCS into UCS
// // input is absolute; if constrPlane is active, transfer input into UCS
// void UTRA_UCS_WCS_PT (Point* pto, Point* pti) {
// // #define UTRA_UCS_WCS_PT(pto, pti) {
// 
//   DEB_dump_obj__ (Typ_PT, pti, "UTRA_UCS_WCS_PT ind=%d",WC_sur_ind);
// 
//   if(WC_sur_ind) UT3D_pt_tra_pt_m3 (pto, WC_sur_imat, pti);
//   else if(pto != pti) *pto = *pti;
// 
//   DEB_dump_obj__ (Typ_PT, pto, "ex-UTRA_UCS_WCS_PT");
// 
// }
// 
// // UTRA_WCS_UCS_VC                      transfer vector from UCS into WCS
// // input is absolute; if constrPlane is active, transfer input into WCS
// void UTRA_WCS_UCS_VC (Vector* vco, Vector* vci) {
// // #define UTRA_WCS_UCS_VC(vco, vci) {
// 
//   DEB_dump_obj__ (Typ_VC, vci, "UTRA_WCS_UCS_VC ind=%d",WC_sur_ind);
// 
//   if(WC_sur_ind) UT3D_vc_tra_vc_m3 (vco, WC_sur_mat, vci);
//   else if(vco != vci) *vco = *vci;
// 
//   DEB_dump_obj__ (Typ_VC, vco, "ex-UTRA_WCS_UCS_VC");
// 
// }
// 
// // UTRA_WCS_UCS_PT                      transfer point from UCS into WCS
// // input is absolute; if constrPlane is active, transfer input into WCS
// void UTRA_WCS_UCS_PT (Point* pto, Point* pti) {
// // #define UTRA_WCS_UCS_PT(pto, pti) {
// 
//   DEB_dump_obj__ (Typ_PT, pti, "UTRA_WCS_UCS_PT ind=%d",WC_sur_ind);
// 
//   if(WC_sur_ind) UT3D_pt_tra_pt_m3 (pto, WC_sur_mat, pti);
//   else if(pto != pti) *pto = *pti;
// 
//   DEB_dump_obj__ (Typ_PT, pto, "ex-UTRA_WCS_UCS_PT");
// 
// }


//====================== EOF =============================
