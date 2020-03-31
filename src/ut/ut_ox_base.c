//     ut_objgx.c                                  RF
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
2011-06-11  UTO_obj_ato1 replaced by OGX_ox_ato1.
2011-06-10  rename to OGX; OGX_oxm_copy_ox OGX_deloc__ OGX_reloc__ new.
2002-12-20  new from older sources. RF.

-----------------------------------------------------
*/
#ifdef globTag
void OGX(){}
#endif
/*!
\file  ../ut/ut_ox_base.c
\brief complex-object manipulationes (transform, convert ..)  (OGX_) 
\code
=====================================================
List_functions_start:

OGX_SET_OBJ             complexObject from binObj (typ,form,siz,dat)
OGX_GET_OBJ             get type and data of binary-obj
OGX_SET_OBJ_aux         complexObject from binObj with aux
OGX_SET_INDEX           complexObject from DB-Obj (dbTyp,dbInd) (form=Typ_Index)
OGX_GET_INDEX           get DB-typ and DB-index out of complexObject
OGX_SET_INT             complexObject from typ=int data=int (form=Typ_Int4)
OGX_GET_INT             get index out of complexObject (long from data-pointer)
OGX_SET_Float8          complexObject from (typ, float8)
OGX_SET_COLOR           complexObject from ColRGB
OGX_NUL                 empty complexObject (_OGX_NUL)

OGX_ox_ato1             complexObj from 1 atomicObj (ausTyp/ausTab)
OGX_ox_copy_ox          complexObj-group from a group of objs.

OGX_oxm_copy_ox         copy iNr complexObjects into Memspc (serialize)
OGX_oxm_copy_obj        copy (serialize) obj into a Memspc
OGX_ox_copy__           copy (serialize) a ObjGX-Tree into a single memChunk.
OGX_ox_copy_obj
OGX_ck_reloc            check if struct is relocatable without modifications
OGX_is_dbo              check if struct is a DB-obj                          INLINE

OGX_deloc__             serialize (make all pointers relative)
OGX_reloc__             deserialize (relocate all pointers)
OGX_reloc_ox            relocate objTree.
OGX_reloc_adr           relocate a pointer.

OGX_siz__               return size of complete ObjGX-Tree; 
OGX_siz_obj             return size of complete ObjGX-Tree;

-------------- TESTFUNCTIONS --------------
OGX_tst__               test 
OGX_tst_load_ox         get a test-ObjGX-tree

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



Functions resolving ObjGX-Trees:
OGX_ox_copy_obj
OGX_reloc_ox
DEB_dump_nobj_1 + DEB_dump_obj_1
OGX_siz_obj
UTRA_app_oTab
UPRJ_app_obj UNUSED - see UPRJ_app__




Offline testen:
see tst_UTO.c
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


// #include "../ut/ut_umem.h"   is in ut_heo.h
#include "../ut/ut_cast.h"          // INT_PTR
#include "../ut/ut_geo.h"
#include "../ut/ut_gtypes.h"
#include "../ut/ut_obj.h"
#include "../ut/ut_memTab.h"            // MemTab_..
#include "../ut/ut_txt.h"               // term_buf

#define INCLUDE_FULL
#include "../ut/ut_ox_base.h"
#undef INCLUDE_FULL


/// \brief relocationDataRecord
/// \code
/// oPos        startPos of primaryobjct; subract (delocate) or add (relocate)
/// mSiz        size of memory chunk; objectTree mus be complete inside
/// mode        0=delocate, 1=relocate
/// \endcode
typedef struct {void *oPos; long mSiz; int mode;}                 Relocdat;




// EXTERNAL variables:


// prototypes:
  int OGX_deloc_adr (void **vpt, Relocdat *rd);


typedef_MemTab(ObjRange);




//================================================================
  int OGX_ox_ato1 (ObjGX *oxo, int *ausTyp, double *ausTab) {
//================================================================
/// \code
/// OGX_ox_ato1             complexObj from 1 atomicObj (ausTyp/ausTab)
///
/// Output:
///   RetCod: pointer to the complexObj inside mSp1
///
/// typical complexObj-Records:
///  Typ_Val   Typ_Val    ((double*)*value)  ((double)value)
///  Typ_<typ> Typ_Index  ((int)DB-ind)
///
/// \endcode

  long  dbi;


  if(*ausTyp == Typ_Val) {
    OGX_SET_OBJ (oxo, Typ_Val, Typ_Val, 1, (void*)ausTab);


  } else {
    dbi =  *ausTab;
    // OGX_SET_OBJ ((ObjGX*)oo, ausTyp, Typ_Index, 1, PTR_LONG (dbi));
    OGX_SET_INDEX (oxo, *ausTyp, dbi);
  }


  return 0;

}


//================================================================
  ObjGX* OGX_oxm_copy_obj (Memspc *mSpo,
                      void *obji, int typ, int iNr, int isolate) {
//================================================================
// copy (serialize) obj into a Memspc 
// see OGX_ox_copy_obj
  
  int      irc;
  long     oSiz;
  void     *objo, *objSta;

  
  objo = mSpo->next;
  objSta = objo;
  oSiz = UME_ck_free (mSpo);

  
  // add DB-obj
  irc = OGX_ox_copy_obj (&objo, &oSiz,       
                         obji,               // DB-obj-data
                         typ,                // type of struct obji
                         iNr,                // nr of subObjects
                         isolate);
  if(irc < 0) goto L_err;
  
  // fix mSpo (set .next)
  if(UME_adjust (mSpo, objo) < 0) goto L_err;

  
  return objSta;

  
  L_err: 
  return NULL;

}


//======================================================================
  ObjGX* OGX_oxm_copy_ox (Memspc *mSpo, ObjGX *oTab, int iNr, int isolate) {
//======================================================================
/// \code
/// copy iNr complexObjects into Memspc (serialize, no delocate)
/// Input:
///   oTab     iNr complexObjects
///   iNr      size of oTab
///   isolate  0=do not resolv (copy) DB-objects; 1=copy the data of DB-objects
/// Output:
///   mSpo
///   RetCod:  NULL   Error (not enough mem in mSpo)
///            else   adress of primary struct ObjGX in mSpo
/// \endcode


  int      i1, irc, sTyp, sForm, rNr;
  long     oSiz;
  void     *objo, *sDat;
  ObjGX    *oxo, *op1;


  // printf("8888888888888888888888888888888888888888888888888888 \n");
  // printf("OGX_oxm_copy_ox iNr=%d isol=%d\n",iNr,isolate);
  // for(i1=0; i1<iNr; ++i1) DEB_dump_obj__ (Typ_ObjGX,&oTab[i1],"oTab[%d]",i1);


  // reserve space for primary struct
  oxo = UME_reserve (mSpo, sizeof(ObjGX));
  if(!oxo) goto L_err;


  if(iNr < 2) {
    // no primary struct if only 1 object
    op1 = oxo;

  } else {
    // reserve space for secondary struct (group) in spcObj1,
    op1 = UME_reserve (mSpo, sizeof(ObjGX) * iNr);
    if(!op1) goto L_err;

    // create primary struct
    OGX_SET_OBJ (oxo, Typ_ObjGX, Typ_ObjGX, iNr, op1);

  }


    // printf("---------------------\n oxo=%ld op1=%ld\n",oxo,op1);



  // copy obj oTab[i1] -> mSpo
  for(i1=0; i1<iNr; ++i1) {

    sTyp  = oTab[i1].typ;
    sForm = oTab[i1].form;
      // printf(" oTab[%d] typ=%d form=%d\n",i1,sTyp,sForm);

    // fix parentrecord
    OGX_SET_OBJ (op1, sTyp, sForm, oTab[i1].siz, oTab[i1].data);

    if(sTyp  == Typ_Typ)  goto L_objn_nxt;     // no data
    if(sTyp  == Typ_Int4) goto L_objn_nxt;     // no extra data; eg Typ_Color


    // form=Typ_Index: isolate-No: done; else resolve
    if(sForm == Typ_Index) {                 // DB-Link
      if(isolate == 0) goto L_objn_nxt;
      // get data-struct of DB-obj
      sForm = DB_GetObjDat (&sDat, &rNr, sTyp, LONG_PTR(oTab[i1].data));
        // printf(" resDB: typ=%d\n",sForm);
      if(sForm <= Typ_Error) goto L_err;
      // fix form & data
      op1->form = sForm;
      op1->data = mSpo->next;
      // add DB-obj
      if(!OGX_oxm_copy_obj (mSpo, sDat, sForm, 1, isolate))
        goto L_err;
      // next parentrecord ..
      goto L_objn_nxt;
    }


    // this is a  record with additional data-struct; eg Point
    // fix pos in parentrecord
    op1->data = mSpo->next;

    // copy structs for eg form=Typ_PT ..
    if(!OGX_oxm_copy_obj (mSpo, oTab[i1].data, sForm, oTab[i1].siz, isolate))
      goto L_err;


    // next parentrecord
    L_objn_nxt:
      ++op1;
        // printf(" [%d] op1=%ld\n",i1,op1);
  }

  oSiz = UME_get_used (mSpo);
    // printf(" oSiz=%ld\n",oSiz);


  return oxo;

  L_err:
    return NULL;

}


//=======================================================================
  int OGX_ox_copy__ (void *objo, long *oSiz, void *obji, int isolate) {
//=======================================================================
/// \code
/// copy (serialize) a ObjGX-Tree into a single memChunk.
/// Does not resolve DB-Objects (see isolate).
/// Does not delocate (see OGX_deloc__).
/// Input:
///   obji   complexObj to be copied
///   objo   where to save obji
///   oSiz   free space at objo
///   isolate  0=do not resolv (copy) DB-objects; 1=copy the data of DB-objects
/// Output:
///   objo   copy of obji
///   oSiz   oSiz is reduced (new free size)
/// RetCode:
///   -1     out of memory-Error
///   else   size of data stored in objo
///
/// see also UTO_copy_tab DB_store_ox UTRA_app_obj UTO_write_obj
/// \endcode

// copy 1-n ObjGX-Records and its data-objects;
//   if the data-object is a ObjGX: recurse (only copy the parent)


  int     irc;
  long    sizTot;


  printf("OGX_ox_copy__ oSiz=%ld isol=%d\n",*oSiz,isolate);

  sizTot = *oSiz;

  irc = OGX_ox_copy_obj (&objo, oSiz, obji, Typ_ObjGX, 1, isolate);

  if(irc >= 0) irc = sizTot - *oSiz;

  return irc;

}


//=======================================================================
  int OGX_ox_copy_obj (void **objo, long *oSiz,
                       void *obji, int typ, int iNr, int isolate) {
//=======================================================================
/// \code
/// copy (serialize) a ObjGX-Tree into a single memChunk.
/// Does not delocate (see OGX_deloc__).
/// Input:
///   obji     struct(s) of type <typ> to be copied
///   typ      type of struct of obji
///   objo     copy of obji
///   oSiz     free space at objo
///   isolate  0=do not resolv (copy) DB-objects; 1=copy the data of DB-objects
/// Output:
///   objo     next free position
///   oSiz     oSiz is reduced (new free size)
/// RetCode:
///   -1       out of memory-Error
///
/// see also UTO_copy_tab DB_store_ox UTRA_app_obj UTO_write_obj
/// \endcode

// copy 1-n ObjGX-Records and its data-objects;
//   if the data-object is a ObjGX: recurse (only copy the parent)

  int     irc, iSiz, rSiz, sTyp, sForm, sSiz, i1, rNr;
  char    *pi, *po;
  void    *p1, *sDat;


  // printf("OGX_ox_copy_obj oSiz=%ld typ=%d iNr=%d isol=%d\n",
         // *oSiz,typ,iNr,isolate);
  // if(typ==Typ_Model) DEB_dump_obj__ (typ, obji, "obji");
  // if(*oSiz > 1000000) gdb_halt ();


  iSiz = UTO_siz_stru (typ);
  if(iSiz <= 0) {
    TX_Error("OGX_ox_copy_obj E000 %d %d\n",typ,iNr);
    goto L_err_ex;
  }

  rSiz = iNr * iSiz;                // total size of primary level

  if(rSiz > *oSiz) {
    TX_Error("OGX_ox_copy_obj E001 %d\n",rSiz);
    goto L_err_ex;
  }


  // copy the primary level obji (typ is ObjGX) --> objo
  pi = (char*)obji;
  po = (char*)*objo;
  memcpy(po, pi, rSiz);              // copy parentRecord(s)
  *oSiz -= rSiz;                     // reduce outSpace-size
  *objo = (void*)(po + rSiz);        // fix next free outputPos
    // printf(" oSiz=%ld rSiz=%d\n",*oSiz,rSiz);
    // if(typ==Typ_Model) DEB_dump_obj__ (typ, po, "po");



  // check if parent has pointers; if not: copy done ..
  if(OGX_ck_reloc(typ) == 0) goto L_fertig;



  //----------------------------------------------------------------
  // it is a struct with pointers; 
  // loop tru primary ObjGX-Record(s)
  for(i1=0; i1<iNr; ++i1) {


    //----------------------------------------------------------------
    if(typ == Typ_ObjGX) {

      sTyp  = ((ObjGX*)po)->typ;               // get child-typ
      sForm = ((ObjGX*)po)->form;              // get type of structure of child
        // printf(" i1=%d sTyp=%d sForm=%d\n",i1,sTyp,sForm);

      // ignore all primary ObjGX-Records with no child-data
      if(sTyp  == Typ_Typ)   goto L_GX_nxt;    // no data
      if(sForm == Typ_Int4)  goto L_GX_nxt;    // no extra data; eg Typ_Color


      if(sForm == Typ_Index) {                 // DB-Link
        if(isolate == 0) goto L_GX_nxt;
        // get data-struct of DB-obj
        sForm = DB_GetObjDat (&sDat, &rNr, sTyp, LONG_PTR(((ObjGX*)po)->data));
        if(sForm == Typ_Error) goto L_err_ex;
          // printf(" resDB: typ=%d\n",sForm);
        // fix form & data
        ((ObjGX*)po)->form = sForm;
        ((ObjGX*)po)->data = *objo;
        // add DB-obj
        return OGX_ox_copy_obj (objo, oSiz,
                             sDat,             // DB-obj-data
                             sForm,
                             1,                // nr of subObjects
                             isolate);
      }


      p1 = *objo;                              // save next free pos
      irc = OGX_ox_copy_obj (objo, oSiz,          // RECURSE
                          ((ObjGX*)po)->data,  
                          sForm,
                          ((ObjGX*)po)->siz,   // nr of subObjects
                          isolate);
      if(irc < 0) return irc;
      ((ObjGX*)po)->data = p1;                 // replace pointer


    //----------------------------------------------------------------
    } else if(typ == Typ_CVPOL) {     // DB_StoreCvPlg

      // copy lvTab --> objo
      if(((CurvPoly*)po)->lvTab) {
        if(MEM_copy_oTab (objo, oSiz,
                          &((CurvPoly*)po)->lvTab,
                          ((CurvPoly*)po)->ptNr,
                          sizeof(double)) < 0) goto L_err_ex;
      }
      // copy cpTab --> objo
      if(MEM_copy_oTab (objo, oSiz,
                        &((CurvPoly*)po)->cpTab,
                        ((CurvPoly*)po)->ptNr,
                        sizeof(Point)) < 0) goto L_err_ex;


    //-------------------------------------------------------
    } else if(typ == Typ_CVBSP) {

      // save knotvals
      if(MEM_copy_oTab (objo, oSiz,
                        &((CurvBSpl*)po)->kvTab,
                        ((CurvBSpl*)po)->ptNr + ((CurvBSpl*)po)->deg + 1,
                        sizeof(double)) < 0) goto L_err_ex;

      // save controlpoints
      if(MEM_copy_oTab (objo, oSiz,
                        &((CurvBSpl*)po)->cpTab,
                        ((CurvBSpl*)po)->ptNr,
                        sizeof(Point)) < 0) goto L_err_ex;


    //-------------------------------------------------------
    } else if(typ == Typ_SURBSP) {

      // save cpTab
      if(MEM_copy_oTab (objo, oSiz,
                        &((SurBSpl*)po)->cpTab,
                        ((SurBSpl*)po)->ptUNr * ((SurBSpl*)po)->ptVNr,
                        sizeof(Point)) < 0) goto L_err_ex;

      // save kvTabU
      if(MEM_copy_oTab (objo, oSiz,
                        &((SurBSpl*)po)->kvTabU,
                        ((SurBSpl*)po)->ptUNr + ((SurBSpl*)po)->degU + 1,
                        sizeof(double)) < 0) goto L_err_ex;

      // save kvTabV
      if(MEM_copy_oTab (objo, oSiz,
                        &((SurBSpl*)po)->kvTabV,
                        ((SurBSpl*)po)->ptVNr + ((SurBSpl*)po)->degV + 1,
                        sizeof(double)) < 0) goto L_err_ex;


    //-------------------------------------------------------
    } else if(typ == Typ_GTXT) {
        // printf(" Typ_GTXT |%s|\n",((GText*)po)->txt);

      // copy text (increase to a multiple of 4)
      if(MEM_copy_oTab (objo, oSiz,
                        &((GText*)po)->txt,
                        UTI_div4up(strlen(((GText*)po)->txt) + 1),
                        1) < 0) goto L_err_ex;


    //----------------------------------------------------------------
/*
    } else if(typ == Typ_Model) {
        printf(" Typ_Model |%s| iSiz=%d\n",((ModelRef*)pi)->mnam,iSiz);
      // copy modelname
      ((ModelRef*)po)->mnam = *objo;
      memcpy (*objo, ((ModelRef*)pi)->mnam, 4);
      *oSiz -= 4;
      *objo = (char*)*objo + 4; // *objo += 4;
        DEB_dump_obj__ (Typ_Model, po, "model");
*/

    //----------------------------------------------------------------
    } else {
      TX_Error("OGX_ox_copy_obj E002 - unsupported struct %d\n",typ);
      goto L_err_ex;
    }


    L_GX_nxt: // proceed to next ObjGX-ParentRecord ..
    po += iSiz;
  }

    

  //================================================================
  L_fertig:
  // DEB_dump_ox_0 (objo, "ex OGX_ox_copy_obj");
  return 0;


  L_err_ex:
    *oSiz = -1;
    return -1;

}


//================================================================
  int OGX_ck_reloc (int typ) {
//================================================================
// check if struct is relocatable without modifications
// RetCod: 0=OK; relocatable without modifications (no pointers in struct).

  switch (typ) {

    case Typ_VC:
    case Typ_PT:
    case Typ_LN:
    case Typ_CI:
    case Typ_CVELL:  // CurvElli
    case Typ_CVCLOT: // CurvClot:
    case Typ_CVTRM:  // CurvCCV:
    case Typ_PLN:
    case Typ_SURRV:  // SurRev
    case Typ_SUR:    // SurStd
    case Typ_Color:  // ColRGB:
    case Typ_TEXR:   // TexRef:
    case Typ_Ditto:  // Ditto:
    case Typ_SPH:    // Sphere:
    case Typ_CON:    // Conus:
    case Typ_TOR:    // Torus:
    case Typ_SymRef: // SymRef:
    case Typ_Model:  // ModelRef
      return 0;
    
    default:
      // struct has pointers !
      break;
  }

  return -1;

}


//================================================================
  int OGX_deloc__ (ObjGX *oxi, long mSiz) {
//================================================================
// serialize (make persistent)
// subtract objStartPos from all pointers;
// see also UTO_reloc_obj wrl_reloc__ tess_reloc_f_

  Relocdat  rd;

  // printf("OGX_deloc__ oxi=%ld siz=%ld\n",oxi,mSiz);

  rd.oPos = oxi;
  rd.mSiz = mSiz;
  rd.mode = 0;

  return OGX_reloc_ox (oxi, Typ_ObjGX, 1, &rd);

}


//================================================================
  int OGX_reloc__ (ObjGX *oxi, long mSiz) {
//================================================================
// deserialize; (must use OGX_deloc before !)
// add objStartPos to all pointers;
// mSiz UNUSED !
 
  Relocdat  rd;

  // printf("OGX_reloc__ newPos=%ld siz=%ld\n",oxi,mSiz);

  rd.oPos = oxi;
  rd.mSiz = mSiz;
  rd.mode = 1;

  return OGX_reloc_ox (oxi, Typ_ObjGX, 1, &rd);

}


//================================================================
  int OGX_reloc_ox (void *obj1, int typ, int iNr, Relocdat *rd) {
//================================================================
/// \code
/// delocate objTree.
/// make all pointers relativ to rd->oPos (the startPos of this objTree).
/// \endcode

// not relocatable structs:
//   Triangle CurvPoly CurvBSpl
//   SurStripe SurBSpl SurRBSpl
//   AText TexBas GText 
//   ModelMock ModelBas ModelRef Dimen Dim3 Activity

  int    irc, i1, iSiz, sTyp, sForm;
  ObjGX  *pDat, *op1;
  char   *po;
  void   *ps, *pe;


  // printf("OGX_reloc_ox typ=%d iNr=%d %ld %ld\n",typ,iNr,rd->oPos,rd->mSiz);


  iSiz = UTO_siz_stru (typ);
  if(iSiz <= 0) {
    TX_Error("OGX_reloc_ox E000 %d %d\n",typ,iNr);
    goto L_err_ex;
  }


  // save pointerPos
  po = (char*)obj1;


  // check if parent has pointers; if not: copy done ..
  if(OGX_ck_reloc(typ) == 0) goto L_fertig;


  //----------------------------------------------------------------
  // it is a struct with pointers; 
  // loop tru primary ObjGX-Record(s)
  for(i1=0; i1<iNr; ++i1) {

    //----------------------------------------------------------------
    if(typ == Typ_ObjGX) {

      sTyp  = ((ObjGX*)po)->typ;               // get child-typ
      sForm = ((ObjGX*)po)->form;              // get type of structure of child
        // printf(" i1=%d sTyp=%d sForm=%d\n",i1,sTyp,sForm);

      // ignore all primary ObjGX-Records with no child-data
      if(sTyp  == Typ_Typ)   goto L_GX_nxt;    // hat keine data
      if(sForm == Typ_Index) goto L_GX_nxt;    // hat keine data
      if(sForm == Typ_Int4)  goto L_GX_nxt;    // hat keine data


      // RELOCATE
      if(rd->mode == 1) {
        // change pointer
        OGX_reloc_adr ((void**)&((ObjGX*)po)->data, rd);

        // reset "delocated"
        ((ObjGX*)po)->dir = 0;
      }


      // RECURSE
      irc = OGX_reloc_ox (((ObjGX*)po)->data,
                          sForm,
                          ((ObjGX*)po)->siz,   // nr of subObjects
                          rd);
      if(irc < 0) return irc;


      // DELOCATE
      if(rd->mode == 0) {
        // change pointer, test if inside mSiz
        if(OGX_reloc_adr ((void**)&((ObjGX*)po)->data, rd)) goto L_err_adr;

        // set "delocated"
        ((ObjGX*)po)->dir = 1;
      }


    //----------------------------------------------------------------
    } else if(typ == Typ_CVPOL) {     // DB_StoreCvPlg
      if(OGX_reloc_adr ((void**)&((CurvPoly*)po)->lvTab, rd)) goto L_err_adr;
      if(OGX_reloc_adr ((void**)&((CurvPoly*)po)->cpTab, rd)) goto L_err_adr;


    //-------------------------------------------------------
    } else if(typ == Typ_CVBSP) {
      if(OGX_reloc_adr ((void**)&((CurvBSpl*)po)->kvTab, rd)) goto L_err_adr;
      if(OGX_reloc_adr ((void**)&((CurvBSpl*)po)->cpTab, rd)) goto L_err_adr;


    //-------------------------------------------------------
    } else if(typ == Typ_SURBSP) {
      if(OGX_reloc_adr ((void**)&((SurBSpl*)po)->cpTab, rd)) goto L_err_adr;
      if(OGX_reloc_adr ((void**)&((SurBSpl*)po)->kvTabU, rd)) goto L_err_adr;
      if(OGX_reloc_adr ((void**)&((SurBSpl*)po)->kvTabV, rd)) goto L_err_adr;


    //-------------------------------------------------------
    } else if(typ == Typ_GTXT) {
      if(OGX_reloc_adr ((void**)&((GText*)po)->txt, rd)) goto L_err_adr;



    //----------------------------------------------------------------
    } else {
      TX_Error("OGX_reloc_ox E002 - unsupported struct %d\n",typ);
      goto L_err_ex;
    }


    L_GX_nxt: // proceed to next ObjGX-ParentRecord ..
    po += iSiz;
  }

  //================================================================
  L_fertig:
  // DEB_dump_ox_0 (objo, "ex OGX_reloc_ox");
  return 0;

  L_err_adr:
    TX_Error("OGX_reloc_ox E001 - outside memoryblock %d\n",typ);

  L_err_ex:
    return -1;

}


//================================================================
  int OGX_reloc_adr (void **vpt, Relocdat *rd) {
//================================================================
// serialize a pointer. vpt = vpt - (rd->oPos).
// test if pointers are inside memchunk of size (rd->mSiz).
// (rd->oPos) is the startposition of the object to serialize;
//   this position is subtracted, so all positions now refer to startPos 0.
// Makes all pointers relative to 0 = rd->oPos.
// relocate a pointer. vpt = vpt + (rd->oPos).
// Makes all pointers relative to *vpt.
// Input: delocated pointers !



  //----------------------------------------------------------------
  if(rd->mode == 0) {
    // delocate
    // move pointer
      // printf("OGX_reloc_adr - %ld -> %ld\n",*vpt, *vpt - LONG_PTR(rd->oPos));
    *vpt = MEM_ptr_mov (*vpt, - LONG_PTR(rd->oPos));

    // test if inside mSiz
    if(ILIMCK1(LONG_PTR(*vpt), rd->mSiz)) return 1;


  //----------------------------------------------------------------
  } else {
    // relocate
    // move pointer
      // printf("OGX_reloc_adr + %ld -> %ld\n",*vpt, *vpt + LONG_PTR(rd->oPos));
    *vpt = MEM_ptr_mov (*vpt, LONG_PTR(rd->oPos));

  }

  return 0;

}


//================================================================
  long OGX_siz__ (ObjGX *oxi, int isolate) {
//================================================================
// OGX_siz__               return size of complete ObjGX-Tree; 
// isolate
//      0 = reports space for ObjGX-Tree without resolving DB-objects
//      1 = reports complete space ..
 
  int     irc;
  long    oSiz;

  oSiz = 0;

  irc = OGX_siz_obj (&oSiz, oxi, Typ_ObjGX, 1, isolate);
  if(irc < 0) return irc;

  return oSiz;

}


//=========================================================================
  int OGX_siz_obj (long *oSiz, void *obj1, int typ, int iNr, int isolate) {
//=========================================================================
// OGX_siz_obj              return size of complete ObjGX-Tree;
// isolate
//      0 = reports space for ObjGX-Tree without resolving DB-objects
//      1 = reports complete space ..

  int    irc, i1, iSiz, sTyp, sForm, rNr;
  char   *po;
  void   *sDat;


  // printf("OGX_siz_obj typ=%d iNr=%d, isol=%d\n",typ,iNr,isolate);


  iSiz = UTO_siz_stru (typ);

  if(iSiz <= 0) {
    TX_Error("OGX_siz_obj E000 %d %d\n",typ,iNr);
    goto L_err_ex;
  }


  // save pointerPos
  po = (char*)obj1;


  // add parent-size
  *oSiz += iSiz * iNr;


  // check if parent has pointers; if not: copy done ..
  if(OGX_ck_reloc(typ) == 0) goto L_fertig;



  //----------------------------------------------------------------
  // it is a struct with pointers; 
  // loop tru primary ObjGX-Record(s)
  for(i1=0; i1<iNr; ++i1) {

    //----------------------------------------------------------------
    if(typ == Typ_ObjGX) {

      sTyp  = ((ObjGX*)po)->typ;               // get child-typ
      sForm = ((ObjGX*)po)->form;              // get type of structure of child
        // printf(" i1=%d sTyp=%d sForm=%d\n",i1,sTyp,sForm);

      // ignore all primary ObjGX-Records with no child-data
      if(sTyp  == Typ_Typ)   goto L_GX_nxt;    // no data
      if(sForm == Typ_Int4)  goto L_GX_nxt;    // no extra data; eg Typ_Color



      if(sForm == Typ_Index) {                 // DB-Link
        if(isolate == 0) goto L_GX_nxt; 
        // get data-struct of DB-obj
        sForm = DB_GetObjDat (&sDat, &rNr, sTyp, LONG_PTR(((ObjGX*)po)->data));
        if(sForm == Typ_Error) goto L_err_ex;
          // printf(" resDB: typ=%d\n",sForm);
        // add size of DB-obj
        return OGX_siz_obj (oSiz,
                            sDat,               // DB-obj-data
                            sForm,
                            1,                   // nr of subObjects
                            isolate);
      }

      // RECURSE a ObjGX-record
      irc = OGX_siz_obj (oSiz,
                         ((ObjGX*)po)->data,
                         sForm,
                         ((ObjGX*)po)->siz,   // nr of subObjects
                         isolate);
      if(irc < 0) return irc;


    //----------------------------------------------------------------
    } else if(typ == Typ_CVPOL) {     // DB_StoreCvPlg
      *oSiz += sizeof(double) * ((CurvPoly*)po)->ptNr;  // lvTab
      *oSiz += sizeof(Point) * ((CurvPoly*)po)->ptNr;   // cpTab


    //-------------------------------------------------------
    } else if(typ == Typ_CVBSP) {     // DB_StoreCvBsp DB_StoreCvRBsp
      *oSiz += sizeof(double) * (((CurvBSpl*)po)->ptNr +
                                 ((CurvBSpl*)po)->deg + 1);  // kvTab
      *oSiz += sizeof(Point) * ((CurvBSpl*)po)->ptNr;        // cpTab


    //-------------------------------------------------------
    } else if(typ == Typ_SURBSP) {     // DB_StoreSurBsp
      *oSiz += sizeof(double) * (((SurBSpl*)po)->ptUNr +
                                 ((SurBSpl*)po)->degU + 1);  // kvTab
      *oSiz += sizeof(double) * (((SurBSpl*)po)->ptVNr +
                                 ((SurBSpl*)po)->degV + 1);  // kvTab
      *oSiz += sizeof(Point) * (((SurBSpl*)po)->ptUNr *
                                ((SurBSpl*)po)->ptVNr);      // cpTab


    //-------------------------------------------------------
    } else if(typ == Typ_GTXT) {       // DB_StoreGTxt
      *oSiz += UTI_div4up (strlen(((GText*)po)->txt) + 1);   // txt



    //----------------------------------------------------------------
    } else {
      TX_Error("OGX_siz_obj E002 - unsupported struct %d\n",typ);
      goto L_err_ex;
    }


    L_GX_nxt: // proceed to next ObjGX-ParentRecord ..
    po += iSiz;
  }

  //================================================================
  L_fertig:
  // DEB_dump_ox_0 (objo, "ex OGX_reloc_ox");
  return 0;

  // L_err_adr:
    // TX_Error("OGX_siz_obj E001 - outside memoryblock %d\n",typ);

  L_err_ex:
    return -1;

}



//================================================================
//================================================================
// Liste_TESTFUNKTIONEN:
//================================================================
//================================================================


#ifdef DEB

 
//================================================================
  int OGX_tst__ () {
//================================================================

  int      irc, isolate;
  long     oSiz, mSiz2;
  ObjGX    *pox1, *pox2;
  Memspc   spcObj1=UME_NEW,
           spcObj2=UME_NEW;
  void     *vp1, *spc2;


  printf("OGX_tst__\n");


  //----------------------------------------------------------------
  // load & copy
  isolate = 1;    // 0=no, 1=yes
  OGX_tst_load_ox (&spcObj1, isolate);
// goto L_exit;



  //----------------------------------------------------------------
  // delocate spcObj1
  printf("delocate ===============================================\n");

  // get size of objTree (BEFORE OGX_deloc__ !)
  oSiz = UME_get_used (&spcObj1);
    printf(" oSiz=%ld\n",oSiz);
  // oSiz = UME_ck_tot (&spcObj1); // get size of spcObjs


  // delocate objTree; check all pointers if inside spcObj1
  pox1 = UME_get_start (&spcObj1);
  irc = OGX_deloc__ (pox1, oSiz);
  if(irc < 0) return -1;

  DEB_dump_ox__ (pox1, "del1");
// exit(0);


  //----------------------------------------------------------------
  // relocate spcObj1 in spcObj2
  printf("relocate ===============================================\n");

  // UME_init (&spcObj2, memspc52, sizeof(memspc52));
  UME_malloc (&spcObj2, oSiz, oSiz);

  // get the primary-object-address (a ObjGX*)
  // pox2 = UME_get_start (&spcObj2);

  // copy whole tree from pox1 -> pox2   (or use DBF)
  // (nur damit Adressen falsch sind !)
  // memcpy (pox2, pox1, oSiz);
  pox2 = UME_save (&spcObj2, pox1, oSiz);


  // relocate objTree
  mSiz2 = UME_ck_tot (&spcObj2);
  irc = OGX_reloc__ (pox2, mSiz2);
  if(irc < 0) exit(0);



  //----------------------------------------------------------------
  // dump data
  DEB_dump_ox__ (pox2, "del2");
  DEB_dump_obj__ (Typ_ObjGX, pox2, "pox2");
  // OGX_dump__ (pox1, "tst_UTO");



  //----------------------------------------------------------------
  L_exit:
  // clear primary space
  UME_free (&spcObj1);
  UME_free (&spcObj2);


  return 0;

}


//================================================================
  int OGX_tst_load_ox (Memspc *spcObj1, int isolate) {
//================================================================
// OGX_tst_load_ox         get a test-ObjGX-tree
// isolate    1=prepare space in spcObj1 for isolate, 0=not.
// RetCod:  -1  EOM

  //----------------------------------------------------------------
  // create a compound-object (ObjGX) of data from different locations
  // Location realStructure typ       form       nr  data  realStructure
  //----------------------------------------------------------------
  // 0        ObjGX         Typ_ObjGX Typ_ObjGX   4  A1    ObjGX
  // A1-1     ObjGX         Typ_PT    Typ_PT      1  A2    ObjGX
  // A1-2     ObjGX         Typ_CVPOL Typ_CVPOL   1  A3    ObjGX
  // A1-3     ObjGX         Typ_GTXT  Typ_GTXT    1  A4    ObjGX
  // A1-4     ObjGX         Typ_LN    Typ_Index   1  A4    ObjGX
  // A2       Point
  // A3       CurvPoly
  // A4       GText

  int      irc, iNr;
  long     oSiz, mSiz2;
  Point    pt1={12., 34., 56.}, pt2={21., 43., 65.};
  Point    pa1[]={{0., 0., 0.},{10., 0., 0.},{10., 10., 0.},{10., 10., 10.}};
  double   da1[]={0., 10., 10., 10.};
  CurvPoly cv1;
  GText    tx1;
  ObjGX    oTab[4];
  ObjGX    ox1, *pox1, *pox2;
  ObjGX    *op0, *op1, *op2;
  Memspc   tmpSpc=UME_NEW;
  void     *vp1;
  char     s1[32];



  printf("OGX_tst_load_ox tst isol=%d\n",isolate);


  //----------------------------------------------------------------
  // create GText
  tx1.pt = pt2; tx1.size = 2.5; tx1.dir = 45.; tx1.txt = s1;
  strcpy(s1, "ABCD");
  DEB_dump_obj__ (Typ_GTXT, &tx1, "tx1");


  //----------------------------------------------------------------
  // create polgyon-curve; with a pointer inside.
  cv1.ptNr=4; cv1.v0=0.; cv1.v1=30.; cv1.lvTab=da1; cv1.cpTab=pa1;
  DEB_dump_obj__ (Typ_CVPOL, &cv1, "cv1");


  //----------------------------------------------------------------
  // add all objects into oTab
  OGX_SET_OBJ (&oTab[0], Typ_PT,    Typ_PT,    1, &pt1);
  OGX_SET_OBJ (&oTab[1], Typ_CVPOL, Typ_CVPOL, 1, &cv1);
  OGX_SET_OBJ (&oTab[2], Typ_GTXT,  Typ_GTXT,  1, &tx1);
  OGX_SET_OBJ (&oTab[3], Typ_LN,    Typ_Index, 1, PTR_INT(25));


  //----------------------------------------------------------------
  // UME_init (&tmpSpc, memspc51, sizeof(memspc51));
  // tmpSpc1 = MEM_alloc_tmp  (1000);
  // UME_malloc (&tmpSpc, 1000, 1000);
  // UME_alloc_tmp (&tmpSpc, 1024);
  vp1 = MEM_alloc_tmp (1024);
  UME_init (&tmpSpc, vp1, 1024);


  // copy iNr objs -> tmpSpc (serialize, no delocate, w|wo isolate)
  iNr = 1;
  pox1 = OGX_oxm_copy_ox (&tmpSpc, &oTab[3], iNr, isolate);

  printf("9999999999999999999999999999999999999999999999999 \n");
  // dump tree
  // pox1 = UME_get_start (&tmpSpc);
  DEB_dump_obj__ (Typ_ObjGX, pox1, "--------------- pox1 -------------\n");
  DEB_dump_ox__ (pox1, "del3");



  //----------------------------------------------------------------
  // copy (serialize)
  printf("copy ===============================================\n");

  // UTO_siz_obj (&oSiz, pox1);       // GEHT NED !
  oSiz = OGX_siz__ (pox1, isolate);
    printf(" oSiz=%ld\n",oSiz);
  if(oSiz < 0) return -1;
// exit(0);


  // spc2 = MEM_alloc_tmp  (oSiz);
  // UME_init (&spcObj2, spc2, oSiz);
  // UME_init (&spcObj1, memspc52, sizeof(memspc52));
  UME_malloc (spcObj1, oSiz, 1000);


  // get the primary-object-address (a ObjGX*)
  pox2 = UME_get_start (spcObj1);

  mSiz2 = UME_ck_tot (spcObj1);
    printf(" mSiz2=%ld\n",mSiz2);

  irc = OGX_ox_copy__ (pox2, &mSiz2, pox1, isolate);
    printf(" irc=%d mSiz2=%ld\n",irc,mSiz2);
  if(irc < 0) return -1;

  // reserve used memory in spcObj2
  UME_set_free (mSiz2, spcObj1);


  DEB_dump_obj__ (Typ_ObjGX, pox2, "--------------- pox2 -------------\n");
  DEB_dump_ox__ (pox2, "del4");


  // UME_free (&tmpSpc);

  return 0;

}


#endif


//========================== EOF ========================================
