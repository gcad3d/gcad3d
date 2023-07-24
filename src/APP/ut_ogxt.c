// ../ut/ut_ogxt.c                                 RF
/*
 *
 * Copyright (C) 2020 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
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
2020-08-15  new. RF.

-----------------------------------------------------
*/
#ifdef globTag
void OXMT(){}
#endif
/*!
\file  ../ut/ut_ogxt.c
       ../ut/ut_ogxt.h
\brief OgxTab = list of (complex-object (ObjGX) + variable-length-record)
see INF_OgxTab

=====================================================
List_functions_start:

OXMT_init_mm         malloc ox-space, malloc variable-length-space
OXMT_init_mx         malloc ox-space, existing variable-length-space
OXMT_free

OXMT_add_oxt         add obj-tree (<oNr> complex-objects) into MemTab OgxTab.spc
OXMT_add_ume         add all data into Memspc OgxTab.spc
OXMT_reserve_osp     reserve space in OgxTab.spc (occupy, do not write)

OXMT_MEMTAB_IND      nr of root-objects in OgxTab.ogx
OXMT_MEMTAB_DAT      get the MemTab-dataBlock out of ospo

-------------- TESTFUNCTIONS --------------
OXMT_dump__
OXMT_dump_1
OXMT_test_1
OXMT_test_2
OXMT_test_3


List_functions_end:
=====================================================

requires:
#include "../ut/ut_ox_base.h"          // OGX_SET_OBJ






//================================================================ */
void INF_OgxTab (){        /*! \code

otb    OXMT_        OgxTab                  ObjGX + var-len-record   INF_OgxTab

OgxTab is a container of a MemTab .ogx plus a Memspc .spc -
  MemTab is a list of Fixed-Length-Records                           see INF_MemTab
  Memspc is a list of Variable-Length-Records                        see INF_Memspc

See also INF_ObjTab

Examples:
  OgxTab otb1 = _OGXTAB_NUL;
  OXMT_test_1 ()


Files:
../APP/ut_ogxt.c
../APP/test_OGXT.c

../ut/ut_ogxt.c
../ut/ut_ogxt.h


================================================================== \endcode */}



#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>

#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_ogxt.h"             // OgxTab ..
#include "../ut/ut_ox_base.h"          // OGX_SET_OBJ

#include "../db/ut_DB.h"               // DB_GetObjGX

#include "../xa/xa_msg.h"              // MSG_*

// ../ut/ut_memTab.h MemTab
// ../ut/ut_umem.h


typedef_MemTab(ObjGX);




//================================================================
  int OXMT_init_mm (OgxTab *oxtb1, int rNr, int datSiz) {
//================================================================
// OXMT_init_mm         malloc ox-space, malloc variable-length-space
//   malloc space for <rNr> ObjGX-records and a memspce of size <datSiz>-bytes
//  spcTyp    MEMTYP_ALLOC__ .. MEMTYP_STACK__
// Input:
//   rNr      max nr of root-objects
//   datSiz   size (bytes) for data-space

  long  l1;


  printf("OXMT_init_mm rNr=%d datSiz=%d\n",rNr,datSiz);

  // get space for <rNr> ObjGX-records (malloc)
  MemTab_ini__ (&oxtb1->ogx, sizeof(ObjGX), Typ_ObjGX, rNr);
  MemTab_add (&oxtb1->ogx, &l1, NULL, rNr, 2);


  // get space for <datSiz> chars for oSpc (malloc)
  UME_malloc (&oxtb1->spc, datSiz, datSiz / 2);

  return 0;

}


//================================================================
  int OXMT_init_mx (OgxTab *oxtb1, int rNr, Memspc *msp) {
//================================================================
// OXMT_init_mx         malloc ox-space, existing variable-length-space
//  spcTyp    MEMTYP_ALLOC__ .. MEMTYP_STACK__
// Input:
//   rNr      max nr of root-objects
//   datSiz   size (bytes) for data-space

  long  l1;


  printf("OXMT_init_mm rNr=%d\n",rNr);
  UME_dump (msp, "mspc");

  // get space for <rNr> ObjGX-records (malloc)
  MemTab_ini__ (&oxtb1->ogx, sizeof(ObjGX), Typ_ObjGX, rNr);
  MemTab_add (&oxtb1->ogx, &l1, NULL, rNr, 2);


  // get space of <msp>
  oxtb1->spc = *msp;
  // set msp invalid
  msp->spcTyp = MEMTYP_NONE;

    printf(" ex-OXMT_init_mx\n");

  return 0;

}


//================================================================
  int OXMT_MEMTAB_IND (OgxTab *oxtb) {
//================================================================
// OXMT_MEMTAB_IND      nr of root-objects in OgxTab.ogx

  return oxtb->ogx.rNr;

}


//================================================================
  ObjGX* OXMT_MEMTAB_DAT (OgxTab *oxtb) {
//================================================================
// OXMT_MEMTAB_DAT      get the MemTab-dataBlock out of ospo

  // ObjGX   *ox1;

  printf("OXMT_MEMTAB_DAT\n");

  return oxtb->ogx.data;

  // return ox1;

}


//================================================================
  void* OXMT_reserve_osp (OgxTab *oxtb, int osiz) {
//================================================================
// OXMT_reserve_osp     reserve space in OgxTab.spc (occupy, do not write)

  printf("OXMT_reserve_osp %d\n",osiz);
  MSG_ERR__ (ERR_TEST, "TDO: OXMT_reserve_osp");
  return NULL;

}


//==================================================================
  void* OXMT_add_ume (OgxTab *oxtb, int form, int oNr, void *data) {
//==================================================================
// OXMT_add_ume         add all data into Memspc OgxTab.spc
//   DOES ADD ObjGX-RECORDS ALSO INTO OgxTab.spc !
//
// Input:
//   ox1   ObjGX; copy all of its children into oxtb.spc
// Output:
//   retCod     NULL=error, else position of stored data in OgxTab.spc
// see OGX_ox_copy_obj

  int    irc, i1, ils;
  long   l1;
  void   *os, *oo;


  printf(" OXMT_add_ume form=%d oNr=%d\n",form,oNr);
  DEB_dump_nobj__ (form, oNr, data, "OXMT_add_ume-in");


  // get record-size of a sibling
  ils = UTO_siz_stru (form);
  if(ils < 1) return NULL;

  // // get startPos of siblings in oxtb.spc
  // os = UME_get_next (&oxtb->spc);


  //----------------------------------------------------------------
  // copy all records into oxtb->spc
  os = UME__copy (&oxtb->spc, &l1, data, ils * oNr);
  if(os == NULL) return NULL;
  // if(irc < 0) return NULL;
  // update oxtb->spc ??  if(l1) 

// TODO: use UME_add_obj

  // test if record have extern-data to store,
  // eg Typ_LN has data of struct Line
  if(!UTO_ck_ptr(form)) goto L_exit;

  oo = data;

  // form=ObjGX: test if only links ..
  if(form == Typ_ObjGX) {
    for(i1=0; i1<oNr; ++i1) {
      if(((ObjGX*)oo)->form != Typ_Index) {
        TX_Error("OXMT_add_ume ObjGX %d",((ObjGX*)oo)->form);
        // TODO: integrate ObjGX into UME_cp_obj (recursion)
        return NULL;
      }
      // oo += ils;
      oo = (char*)oo + ils;
    }
    goto L_exit;
  }

  oo = os;


  //----------------------------------------------------------------
  // copy data for struct of type <form> -> oxtb->spc
  // loop tru records, store extern-data
  for(i1=0; i1<oNr; ++i1) {
    UME_cp_obj (&oxtb->spc, form, oo);
    // oo += ils;
    oo = (char*)oo + ils;
  }

  //----------------------------------------------------------------
  L_exit:

    OXMT_dump__ (oxtb, "ex-OXMT_add_ume");

  return os;

}


//======================================================================
  int OXMT_add_oxt (OgxTab *oxtb, ObjGX **pStart, ObjGX *oxa, int oNr) {
//======================================================================
// OXMT_add_oxt       add <oNr> complex-objects with data
//   does resolv subObjs of ObjGX
// Input:
//   oxa       table of <oNr> complex-objects (Typ_ObjGX) to copy
//   oNr       nr of ObjGX-records in oxa
// Output:
//   pStart    pointer to startpos of oxa in OgxTab
//   reCod     0=OK, -1=ERR


  int      irc, i1, iTb;
  long     l1;
  void     *pDat;
  ObjGX    *ox1, *oxTb;


  printf("\n::::::::::::::::::::::::::::::::::::::::::::::: \n");
  DEB_dump_obj__ (Typ_ObjGX, oxa, "OXMT_add_oxt-in\n");
  OXMT_dump__ (oxtb, "OXMT_add_oxt-in");
  // for(i1=0; i1<oNr; ++i1) DEB_dump_ox_s_ (&oxa[i1], " ox[%d]",i1);
  DEB_dump_nobj__ (Typ_ObjGX, oNr, oxa, " ox");

  // get startIndex in OgxTab
  iTb = MEMTAB_IND (&oxtb->ogx);
    printf(" _nAdd__-iTb=%d\n",iTb);

  *pStart = MEMTAB__ ((MemTab(ObjGX)*)&oxtb->ogx, iTb);

  oxTb = OXMT_MEMTAB_DAT (oxtb);

  // first copy all ObjGX-records
  irc = MemTab_add (&oxtb->ogx, &l1, oxa, oNr, 0);
  if(irc < 0) return -1;


  // loop tru records, add data into oxtb->spc
  for(i1=0; i1<oNr; ++i1) {
    ox1 = &oxTb[iTb + i1];
      DEB_dump_obj__ (Typ_ObjGX, ox1, " _nAdd__-ox1[%d]",i1);

    // test if ObjGX-record has extern-data to store,
    if(!ox1->data) continue;

    // test form - eg Typ_LN has data of struct Line
    if(!UTO_ck_data(ox1->form)) continue;

    // add data into Memspc of OgxTab
    irc = UME_add_obj (&oxtb->spc, &pDat, ox1->form, ox1->siz, ox1->data);
    if(irc < 0) return -1;

    // update ObjGX-pointer
    ox1->data = pDat;
  }



  return 0;

}


//================================================================
  int OXMT_free (OgxTab *oxtb1) {
//================================================================

  printf("OXMT_free \n");


  MemTab_free (&oxtb1->ogx);

  UME_free (&oxtb1->spc);

  return 0;

}


//================================================================
  int OXMT_dump__ (OgxTab *oxtb1, char *txt) {
//================================================================
// OXMT_dump

  printf("============ OXMT_dump__ %s\n",txt);

  MemTab_dump (&oxtb1->ogx, ".ogx");

  UME_dump (&oxtb1->spc, ".spc");
  printf("============ exit OXMT_dump__ %s\n",txt);

  return 0;

}

//================================================================
  int OXMT_dump_1 (OgxTab *oxtb1, int ii, char *txt, ...) {
//================================================================
// OXMT_dump

  char       s1[256];
  va_list    va;


  va_start(va,txt);
  vsnprintf(s1,sizeof(s1),txt,va);
  va_end(va);


  printf("============ OXMT_dump_1 %d %s\n",ii,s1);


  MemTab_dump (&oxtb1->ogx, ".ogx");

  // UME_dump (&oxtb1->spc, ".spc");

  return 0;

}



//================================================================
//================================================================
// TESTFUNKTIONS:
//================================================================
//================================================================


// Test OXMT_add_oxt see BMSH_test_cre1


#ifdef DEB

//=========================================================
  int OXMT_test_1 () {
//=========================================================
// create bMsh (tesselated surface) manually
// Surf = 1-n PlanarPatches
//            Planar patch = 1-n contours (and vector)
//                               Contour = ipoints (closed, tesselated)

  int   irc;

  OgxTab otb1 = _OGXTAB_NUL;
  ObjGX  *bMsh;

  printf("============================================== OXMT_test1 \n");

  //----------------------------------------------------------------
  // create surf from 2 Circles
  irc = OXMT_test_2 (&bMsh, &otb1);

  //----------------------------------------------------------------
  // get bMsh
  // irc = OXMT_test_3 (&bMsh, &otb1);

  //----------------------------------------------------------------
  // disp bMsh 
  // BMSH_test_disp (bMsh, 2);

  //----------------------------------------------------------------
  L_exit:
  OXMT_free (&otb1);

  return 0;

}


//================================================================
  int OXMT_test_2 (ObjGX **bMsh, OgxTab *otb1) {
//================================================================
// OXMT_test_2            create surf from 2 Circles


  int       irc, i1, i2;
  long      dbiOB, dbiIB, dbiSu;
  Point     ptc={130.,130.,100.};
  Circ      ciOB, ciIB;
  ObjGX     ox1, *pox1, *pox2, *poxSu;


  printf("OXMT_test_2 \n");

  // clear all
  UI_men__ ("clear");


  //----------------------------------------------------------------
  // create binary obj circle ciOB from centerPt ptc and radius 40
  UT3D_ci_ptvcr (&ciOB, &ptc, (Vector*)&UT3D_VECTOR_Z, 40.);

  // create binary obj circle ciIB from centerPt ptc and radius 20
  UT3D_ci_ptvcr (&ciIB, &ptc, (Vector*)&UT3D_VECTOR_Z, 20.);

  // store circs in DB as dynamic objs
  dbiOB = DB_StoreCirc (-1L, &ciOB);
  dbiIB = DB_StoreCirc (-1L, &ciIB);
    printf(" dbiOB=%ld dbiIB=%ld\n",dbiOB,dbiIB);

//   // create PRCV for new bnd
//   irc = PRCV_set_dbo__ (Typ_CV, iOB1);
//   if(irc < 0) return -1;


  //----------------------------------------------------------------
  // get space for n output-surfaces in otps (automatic reallocating)
  i1 = 16;     // i1 = nr of ObjGX-records
  i2 = 512;    // nr of bytes for ObjGX-data
  // get space of wrkSpc = memspc for surface-components (invalidates wrkSpc)
  OXMT_init_mm (otb1, i1, i2);

    // TESTBLOCK
    OXMT_dump__ (otb1, "SMSH_ope__-OXMT_init_mx");
    // return MSG_ERR__ (ERR_TEST, "");
    // END TESTBLOCK


  //----------------------------------------------------------------
  // copy the supportSurface of sm1 into otb1.Memspc
  OGX_SET_INDEX (&ox1, Typ_PLN, (long)DB_VCZ_IND);
  pox1 = OXMT_add_ume (otb1, Typ_ObjGX, 1, &ox1);
  if(!pox1) return MSG_ERR__ (ERR_internal, "");

  // add ciOB as outer boundary -> otb1.Memspc
  OGX_SET_INDEX (&ox1, Typ_CI, dbiOB);
  pox2 = OXMT_add_ume (otb1, Typ_ObjGX, 1, &ox1);
  if(!pox2) return MSG_ERR__ (ERR_internal, "");

  // add ciIB as inner boundary -> otb1.Memspc
  OGX_SET_INDEX (&ox1, Typ_CI, dbiIB);
  pox2 = OXMT_add_ume (otb1, Typ_ObjGX, 1, &ox1);
  if(!pox2) return MSG_ERR__ (ERR_internal, "");

  // get ox1 = surf (root-obj) 1.Rec=support; 2.Rec=OB, 3.Rec=IB;
  OGX_SET_OBJ (&ox1, Typ_SUTP, Typ_ObjGX, 3, pox1);

  // add surf as ObjGX - copy ox1 -> otps.MemTab;
  irc = OXMT_add_oxt (otb1, (ObjGX**)&poxSu, &ox1, 1);   // 
  if(irc < 0) return MSG_ERR__ (ERR_internal, "");

  // store surf in DB
  dbiSu = -1L;
  irc = DB_StoreSur (&dbiSu, poxSu);
  if(irc < 0) return MSG_ERR__ (ERR_internal, "");

    // TESTBLOCK
    DEB_dump_obj__ (Typ_ObjGX, &ox1, "ox1");
    OXMT_dump__ (otb1, "SMSH_ope__-OXMT_init_mx");
    printf(" ogx.rNr=%d\n",otb1->ogx.rNr);
    printf(" spc-used=%ld\n",UME_get_used(&otb1->spc));
    printf(" dbiSu=%ld\n",dbiSu);
    // return MSG_ERR__ (ERR_TEST, "");
    // END TESTBLOCK


  //----------------------------------------------------------------
  // disp surf
  APT_Draw__ (0, Typ_SUR, dbiSu);
  DL_Redraw ();


  return 0;

}


//================================================================
  int OXMT_test_3 (ObjGX **bMsh, OgxTab *otb1) {
//================================================================
// OXMT_test_3       create bMsh (tesselated surface) manually
// create a surface and add into otb1; return the pointer to bMsh
// Result is:
// typ=SurGL_(70) form=ObjGX(205) siz=2 dir=0        bMsh
//   typ=SurGLpp(71) form=ObjGX(205) siz=2 dir=0       planar-patch;
//     typ=Vec(2) form=Vec(2) siz=1 dir=0                normal-vec
//     typ=PT(3) form=PT(3) siz=5 dir=0 aux=6            5 points (GL_TRIANGLE_FAN)  OB
//   typ=SurGLpp(71) form=ObjGX(205) siz=2 dir=0       planar-patch;
//     typ=Vec(2) form=Vec(2) siz=1 dir=0                normal-vec
//     typ=PT(3) form=PT(3) siz=5 dir=0 aux=6            5 points (GL_TRIANGLE_FAN)  IB
// for writing into file must be serialized

#define GL_TRIANGLE_FAN  6             // dont do this ..


  int   irc;

  Point pp1[]={600., 0., 0., 650., 0., 0.,
               650.,20., 0., 600.,20., 0., 600., 0., 0.,};
  Point pp2[]={600.,20., 0., 650.,20., 0.,
               650.,40., 0., 600.,40., 0., 600.,20., 0.,};
  Point pp3[]={600.,40., 0., 650.,40., 0.,
               650.,40.,40., 600.,40.,40., 600.,40., 0.,};

  ObjGX  *pSur, *pPat, *dPat, oxa[2];

  printf("============================================== OXMT_test_3 \n");

  // get space for 10 ObjGX and 16k data
  OXMT_init_mm (otb1, 10, 16000);        //, MEMTYP_STACK__);
    // OXMT_dump__ (otb1, "\nOXMT_tst_2__-L1");


  // add primary obj pSur with 2 childs (patches)
  OGX_SET_OBJ (&oxa[0], Typ_GL_Sur, Typ_ObjGX, 2, NULL);
  OXMT_add_oxt (otb1, &pSur, &oxa[0], 1);

  // add 2 patches, will have 2 childs (a vector and a contour)
  OGX_SET_OBJ (&oxa[0], Typ_GL_PP, Typ_ObjGX, 2, NULL);
  OGX_SET_OBJ (&oxa[1], Typ_GL_PP, Typ_ObjGX, 2, NULL);
  OXMT_add_oxt (otb1, &pPat, &oxa[0], 2);

  // connect surf with patch
  ((ObjGX*)pSur)->data = pPat;

  // add data of 1. patch dPat[0]
  OGX_SET_OBJ_aux (&oxa[0], Typ_VC, Typ_VC, 1, (Vector*)&UT3D_VECTOR_Z, 0);
  OGX_SET_OBJ_aux (&oxa[1], Typ_PT, Typ_PT, 5, pp1, GL_TRIANGLE_FAN);
  // add 2 structs
  OXMT_add_oxt (otb1, &dPat, oxa, 2);

  // connect 1. patch with patchData
  ((ObjGX*)&pPat[0])->data = dPat;

  // add data of 2. patch dPat[1]
  OGX_SET_OBJ_aux (&oxa[0], Typ_VC, Typ_VC, 1, (Vector*)&UT3D_VECTOR_IY, 0);
  OGX_SET_OBJ_aux (&oxa[1], Typ_PT, Typ_PT, 5, pp3, GL_TRIANGLE_FAN);
  // add 2 structs
  OXMT_add_oxt (otb1, &dPat, oxa, 2);

  // connect 2. patch with patchData
  ((ObjGX*)&pPat[1])->data = dPat;


    // OXMT_dump__ (otb1, "\nOXMT_tst1__-L2");
    // DEB_dump_ox_s1 (OXMT_MEMTAB_DAT(otb1), "sur-tess:");
    DEB_dump_ox_s_ (pSur, "ex-OXMT_test_3");


  *bMsh = pSur;

  return 0;

}


#endif



// EOF
