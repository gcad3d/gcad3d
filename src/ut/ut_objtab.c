//**************************************************************************
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
  OTB_add__: change UME_save -> UME__copy; if reallocate: update oTab->oDat

-----------------------------------------------------
Modifications:
2018-11-03 New. RF.

-----------------------------------------------------
*/
#ifdef globTag
void OTB(){}
#endif
/*!
\file  ../ut/ut_objtab.c
\brief functions for list of binary-objects (type,data) with boundingBox
\code
=====================================================
List_functions_start:

OTB_malloc              init ObjTab - malloc
OTB_init                give spc1 to oTyp oTab bbox, give spc2 to oSpc
OTB_reset__             reset oTyp,oDat,oSpc [xdat]
OTB_free

OTB_reserve       UU    occupy space in oTab
OTB_add__               add single bin.obj to ObjTab
OTB__obj_add            add binary-2D-objs to otbo
OTB_add_xDat            add auxiliary obeject(s) to ObjTab
OTB_del                 delete inr records starting with iis
OTB_otb_tra_otb2_rsys   transfer 2D-objects onto 3D-plane

OTB_rSiz                return recordsize for oTyp + oTab + pba1 + pba2
OTB_iPrv                get segInd preceding i1 (if connected ring)
OTB_iNxt                get segInd following i1 (if connected ring)
OTB_dump                dump/display ..

List_functions_end:
=====================================================

ObjTab =  list of [oTyp, pointer to obj, Memspc for obj, aux.obj]


\endcode *//*----------------------------------------


ObjTab is a parallel table of types and data-structs and can have a 
  parallel table of auxilary-objects (eg bounding-boxes).


// Examples:

--------------------------
  // malloc space; does realloc, must free

  int     irc, rNr, siz1, siz2;
  ObjTab  otb;
  rNr = 100;                                  // malloc space for rNr of records 
  siz1 = rNr * (sizeof(int) + sizeof(void));  // compute size of oTyp,oDat
  siz2 = rNr * sizeof(Circ2);                 // estimate size of oSpc
  irc = OTB_init (&otb, NULL, siz1, MEMSPCTYP_MALLOC__,
                           NULL, siz2, MEMSPCTYP_MALLOC__);
  OTB_add__ (&otb, oTyp, oDat);
  ..
  OTB_free (&otb);


--------------------------
  // get tempspace (stack, only valid in acive func, no realloc):



--- testMode:
cp ../ut/ut_objtab.c ../xa/tst_ut_objtab.c
gcad_src.mak



*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>



#include "../ut/ut_geo.h"              // Point ...
#include "../xa/xa_msg.h"              // MSG_*



//========================================================================
  int OTB_otb_tra_otb2_rsys (ObjTab *oTab3, ObjTab *oTab2, Refsys *rSys) {
//========================================================================
// OTB_otb_tra_otb2_rsys     transfer 2D-objects onto 3D-plane
// Input:
//   oTab2        2D-objects
//   rSys         3D-plane, matrix, bpi ..
// Output:
//   oTab3        3D-objects


  int   irc, i1, *oTyp2;
  char  so1[OBJ_SIZ_MAX];
  void  **opDat2;


  // printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
  // printf("OTB_otb_tra_otb2_rsys O-Siz=%d I-Nr=%d\n",
                    // oTab3->oSiz, oTab2->oNr);
  // OTB_dump (oTab2, 0, 0, "in-_otb2_bp");


  // loop tru in-obj's
  oTyp2  = oTab2->oTyp;
  opDat2 = oTab2->oDat;

  for(i1=0; i1<oTab2->oNr; ++i1) {

    // get 3D-obj for binary-2D-obj

    if(oTyp2[i1] == Typ_LN2) {
      UT3D_ln_tra_ln2_rsys (so1, opDat2[i1], rSys);
      // store obj -> oTab3->oSpc
      irc = OTB_add__ (oTab3, Typ_LN, so1);
      if(irc < 0) return -1;


    } else if(oTyp2[i1] == Typ_CI2) {
      UT3D_ci_tra_ci2_rsys (so1, opDat2[i1], rSys);
      // store obj -> oTab3->oSpc
      irc = OTB_add__ (oTab3, Typ_CI, so1);
      if(irc < 0) return -1;


    } else {
      TX_Error("OTB_otb_tra_otb2_rsys E1-%d",oTyp2[i1]);
      return -1;
    }

  }

    // TESTBLOCK
    // OTB_dump (oTab3, 1, 0, "ex-OTB_otb_tra_otb2_rsys");
    // END TESTBLOCK


  return 0;

}


//==========================================================================
  int OTB__obj_add (ObjTab *otbo, int iTyp, void *iDat) {
//==========================================================================
// add binary-2D-objs to otbo

  int     i0, i1, iNr;
  Line2   *lna, ln2;
  Point2  *pta;


  // printf("################################### \n");
  // printf("OTB__obj_add siz=%d iTyp=%d\n",otbo->oSiz,iTyp);
  // DEB_dump_obj__ (iTyp, iDat, "obj_add-iDat");
  // OTB_dump (otbo, I_BIN(1), 0, "in-OTB__obj_add");


  switch (iTyp) {

    //================================================================
    case Typ_LN2:
      OTB_add__ (otbo, Typ_LN2, iDat);
      break;


    //================================================================
    case Typ_CVPOL2:
      // create lines from polygon
      // pta = (Point2*)iDat;
      pta = ((CurvPol2*)iDat)->pTab;
      iNr = ((CurvPol2*)iDat)->ptNr;
      i1 = 0;
      ln2.p1 = pta[i1];
      ++i1;
      L_pt2_nxt:
        ln2.p2 = pta[i1];
        ++i1;
        OTB_add__ (otbo, Typ_LN2, &ln2);
        if(i1 < iNr) {
          ln2.p1 = ln2.p2;
          goto L_pt2_nxt;
        }
      break;


    //================================================================
    case Typ_CI2:
      OTB_add__ (otbo, Typ_CI2, iDat);
      break;


    //================================================================
    default:
      TX_Error("OTB__obj_add E002 %d",iTyp);
      return -1;
  }


    // TESTBLOCK
    // printf("ex-OTB__obj_add %d\n",otbo->oNr);
    // OTB_dump (otbo, I_BIN(1), 0, "ex-OTB__obj_add");
    // ERR_raise (__func__);
    // END TESTBLOCK

  return 0;


   L_Err_EOM:
     TX_Error("OTB__obj_add EOM");
     return -1;

}


//================================================================
  int OTB_rSiz (int xTyp) {
//================================================================
// return recordsize for oTyp + oTab + pba1 + pba2
// Input:
//   boxTyp type of boxpoints (pba1,pba2): Typ_PT|Typ_PT2|Typ_Error=0=no-boxPoints

  int rSiz;

  rSiz = sizeof(int) + sizeof(void*);
  if(xTyp) {
    if(xTyp == Typ_PT) rSiz += sizeof(BBox);
    else if(xTyp == Typ_PT2) rSiz += sizeof(BBox2);
  }

  return rSiz;

}
 

//========================================================================
 int OTB_malloc (ObjTab *oTab, int rNr, int xTyp, int siz2) {
//========================================================================
// OTB_malloc                       init ObjTab - malloc
//   malloc rNr records for oTyp oTab  pba1 pba2; get siz2 bytes for oSpc.
// Input:
//   rNr       nr of records for oTyp oTab pba1 pba2
//   xTyp      type of auxiliary object; 0=none, BBox2|BBox ?
//   siz2      size of oSpc (bytes)

  int   rSiz, xSiz;
  long  l1, l2;
  void  *vpx;

  // printf(" OTB_malloc rNr=%d siz2=%d\n", rNr, siz2);

  if(oTab->spcTyp) return MSG_ERR__ (ERR_internal, "memspc not free");

  // get space
  vpx = malloc (rNr * sizeof(int));
  oTab->oTyp = vpx;


  vpx = malloc (rNr * sizeof(void*));
  oTab->oDat = vpx;

  if(xTyp) {
    xSiz = UTO_siz_stru (xTyp);
    if(xTyp == Typ_PT)       vpx = malloc (rNr * xSiz);
    oTab->xDat = vpx;
    oTab->xSiz = xSiz;
  }

  oTab->oNr  = 0;
  oTab->oSiz = rNr;
  oTab->xTyp = xTyp;
  oTab->spcTyp = MEMSPCTYP_MALLOC__;


  //----------------------------------------------------------------
  l1 = siz2;
  l2 = l1 / 3;
  UME_malloc (&oTab->oSpc, l1, l2);   // set MEMSPCTYP_MALLOC__



    // UME_dump (&oTab->oSpc, "ex-OTB_malloc");

  return 0;

}


//================================================================
  int OTB_init (ObjTab *oTab, int xTyp, int fmtb,
                   void *spc1, int siz1, void *spc2, int siz2) {
//================================================================
// init ObjTab; give spc1 to oTyp oTab bbox, give spc2 to oSpc.
//   spc1 and spc2 cannot be reallocated, must not be freed
//     (eg stackspace or fixed memblocks)
// Input:
//   xTyp      type of auxiliary object; 0=none, Typ_BBox|Typ_BBox2
//   fmtb      format of binary obj's: Typ_GEOB_2D|Typ_GEOB_3D
//   spc1      memSpc for oTyp + oDat + bbox
//   siz1      size of spc1 (bytes)
//   spc2      memSpc for oSpc
//   siz2      size of spc2 (bytes) for oSpc
//

  int   i1, rNr, rSiz, xSiz;
  long  l1, l2;
  char  *pc1, *pc2, *pc3;
  void  *vpx;




  //----------------------------------------------------------------
  // fixed space:  give spc1 to oSiz, oTyp, bbox
  // find max nr of records
  // get rSiz = recordsize
  // rSiz = OTB_rSiz (xTyp);
  rSiz = sizeof(int) + sizeof(void*);
  if(xTyp) {
    xSiz = UTO_siz_stru (xTyp);
    rSiz += xSiz;
    oTab->xSiz   = xSiz;
  }


  rNr = siz1 / rSiz;
    // printf(" OTB_init rNr=%d xTyp=%d fmtb=%d\n",rNr,xTyp,fmtb);


  oTab->oTyp = spc1;
  pc1 = (char*) spc1; 
  pc2 = &pc1[rNr * sizeof(int)];
  oTab->oDat = (void*) pc2;

  if(xTyp) {
    pc3 = &pc2[rNr * sizeof(void*)];
    oTab->xDat = (void*) pc3;
  }


  // connect oSpc = spc2
  UME_init (&oTab->oSpc, spc2, siz2);


  oTab->oNr    = 0;
  oTab->oSiz   = rNr;
  oTab->spcTyp = MEMSPCTYP_TMP;
  oTab->xTyp   = xTyp;
  oTab->fmtb   = fmtb;


    // UME_dump (&oTab->oSpc, "ex-OTB_init");

  return 0;

}


//================================================================
  int OTB_free (ObjTab *oTab) {
//================================================================

  if(MEM_CANNOT_FREE(oTab->spcTyp)) return -1;

  free (oTab->oTyp);
  oTab->oTyp = NULL;

  free (oTab->oDat);
  oTab->oDat = NULL;

  if(MEM_MUST_FREE(oTab->xTyp)) {
    if(oTab->xDat) free (oTab->xDat);
  }
  oTab->xDat = NULL;

  UME_free (&oTab->oSpc);

  oTab->spcTyp = MEMSPCTYP_NONE;

  return 0;

}
 

//================================================================
  int OTB_add_xDat (ObjTab *oTab, int ind, int rNr, void *xDat) {
//================================================================
// OTB_add_xDat                     add auxiliary obeject(s) to ObjTab
// Input:
//   ind     first recordNr to write; 0 = first;
//           neg. = from end; -1 = last record
//   rNr     nr of records to write; size of records is oTab->xSiz
//   xDat    data to write


  void    *vpd;
  int     ii;

  // printf("OTB_add_xDat %d %d\n", ind, rNr);

  if(!oTab->xSiz) {TX_Error("OTB_add_xDat E1"); return -1;}

  if(ind < 0) ii = oTab->oNr - 1;
  else        ii = ind;

  if((ii + rNr) > oTab->oNr) {TX_Error("OTB_add_xDat E2"); return -1;}

  vpd = &((char*)oTab->xDat)[ii * oTab->xSiz];
  memcpy (vpd, xDat, oTab->xSiz * rNr);


  return 0;

}

 
//================================================================
  int OTB_add__ (ObjTab *oTab, int iTyp, void *oDat) {
//================================================================
// OTB_add__             add single bin.obj to ObjTab

  int     ii;
  long    spcOff, osiz;
  void    *vpd, *vpTol;
  va_list va;


  // DEB_dump_obj__ (iTyp, oDat, " OTB_add__");
  // printf("OTB_add__ typ=%d oNr=%d oSiz=%d\n",iTyp,oTab->oNr,oTab->oSiz);

  ii = oTab->oNr;

  oTab->oTyp[ii] = iTyp;

  osiz = UTO_siz_stru (iTyp);

  vpd = UME__getSpc (&oTab->oSpc, &spcOff, osiz);
  if(!vpd) return -1;


  // realloc all ObjTab-spaces
  if(spcOff) {
    TX_Error("OTB_add__ EOM-TODO"); return -1;
  }
   
  // copy data
  memcpy (vpd, oDat, osiz);
  oTab->oDat[ii] = vpd;


  oTab->oNr += 1;
  if(oTab->oNr >= oTab->oSiz) {TX_Error("OTB_add__ EOM-1"); return -1;}

  return 0;

}


//================================================================
  int OTB_del (int iis, int inr, ObjTab *oTab) {
//================================================================
// OTB_del                     delete inr records starting with iis
// deletes type and the pointer to data, updates oNr, 
// does not delete data itself.
// Input:
//   iis   index of first record to delete
//   inr   nr of records to delete

  int    i1, rSiz;


  // printf("OTB_del %d %d\n",iis,inr);
  // OTB_dump (oTab, 1, 0, "OTB_del");


  if(oTab->oNr < (iis + inr)) return MSG_ERR__ (ERR_internal, "");

  // delete type oTyp
  i1 = oTab->oNr;
  MEM_del_nrec (&i1, oTab->oTyp, iis, inr, sizeof(int));

  // delete data-pointer oDat
  i1 = oTab->oNr;
  MEM_del_nrec (&i1, oTab->oDat, iis, inr, sizeof(void*));

  // del xDat
  if(oTab->xTyp) {
    i1 = oTab->oNr;
    MEM_del_nrec (&i1, oTab->xDat, iis, inr, oTab->xSiz);
  }


  oTab->oNr -= inr;
  


    // TESTBLOCK
    // OTB_dump (oTab, 1, 0, "ex-OTB_del");
    // END TESTBLOCK

  return 0;

}


//================================================================
  int OTB_dump (ObjTab *oTab, int mode, int att, char *sTxt) {
//================================================================
// dump object-table
// Input:
//   mode  bitval  1 dump
//         bitval  2 display
//         bitval  4 display bounding-box
//         bitval  8 display index of grafic-objects
//         bitval 16 dump bounding-box
//   att   see INF_COL_CV
//
// example: OTB_dump (&otb, I_BIN(01110), Typ_Att_dash__, "fncX");
//                                |8421  
//     dump bounding-box----------/||||
//     disp. index of data-objects-/|||
//     display bounding-box---------/||
//     display data-objects----------/|
//     dump data-objects--------------/


  int    irc, i1, iDump, iDisp, iBbox, iIndx, iDumx;
  char   s1[32];
  Point  pt3;
  Point2 pt2;

  iDump = mode & 1;
  iDisp = mode & 2;
  iBbox = mode & 4;
  iIndx = mode & 8;
  iDumx = mode & 16;


  printf("================ OTB_dump %s siz=%d nr=%d iDump=%d iDisp=%d\n",
         sTxt,oTab->oSiz,oTab->oNr,iDump,iDisp);


  for(i1=0; i1<oTab->oNr; ++i1) {
    // printf(" typ[%d] = %d\n",i1,oTab->oTyp[i1]);
    if(iDump) DEB_dump_obj__ (oTab->oTyp[i1], oTab->oDat[i1], "%d",i1);
    // if(iDisp) GR_Disp_obj (oTab->oTyp[i1], oTab->oDat[i1], att, 0);
    if(iDisp) GR_Disp_obj (oTab->oTyp[i1], oTab->oDat[i1], Typ_Att_dash_long, 0);
    if(iBbox) {
      if(oTab->xTyp == Typ_BBox2) 
        GR_Disp_box2 (&((BBox2*)oTab->xDat)[i1].pb1,
                      &((BBox2*)oTab->xDat)[i1].pb2, Typ_Att_dash__);
                      // &((BBox2*)oTab->xDat)[i1].pb2, Typ_Att_dash_long);
      else printf(" ***** OTB_dump 4 %d not supported ...\n",oTab->xTyp);
    }
    if(iIndx) {
      sprintf(s1, "%d", i1);
      // 2D or 3D ?
      if(oTab->fmtb == Typ_GEOB_2D) {
        irc = UT2D_ptvcpar1_std_obj (&pt2, NULL, NULL, Ptyp_mid,
                                     oTab->oTyp[i1], oTab->oDat[i1]);
        GR_Disp_tx2 (&pt2, s1, Typ_Att_def);

      } else if(oTab->fmtb == Typ_GEOB_3D) {
        irc = UT3D_ptvcpar1_std_obj (&pt3, NULL, NULL, Ptyp_mid,
                                     oTab->oTyp[i1], oTab->oDat[i1]);
        GR_Disp_tx (&pt3, s1, Typ_Att_def);
      } else TX_Error("OTB_dump-fmtb not set %d",oTab->fmtb); 
    }
    if(iDumx) {
      if(oTab->xTyp == Typ_BBox2)
        DEB_dump_obj__ (Typ_BBox2, &((BBox2*)oTab->xDat)[i1], "bbx2[%d]",i1);
      else printf(" ***** OTB_dump 16 %d not supported ...\n",oTab->xTyp);
    }
  }

  printf("================ OTB_dump end \n");


  return 0;

}


//================================================================
  int OTB_reset__ (ObjTab *oTab, int mode) {
//================================================================
// reset oTyp,oDat,oSpc [xdat]
// Input:
//   mode   0    do not change xDat
//          1    clear xDat


  oTab->oNr = 0;
  UME_reset (&oTab->oSpc);

  if(mode) {
    if(MEM_MUST_FREE(oTab->spcTyp)) {
      if(oTab->xDat) free (oTab->xDat);
    }
    oTab->xDat = NULL;
    oTab->fmtb = 0;
  }

  return 0;

}


//================================================================
  int OTB_reserve (int **ia, void **oa, ObjTab *oTab, int oNr) {
//================================================================
// OTB_reserve      occupy space in oTab; return type-array, pointer-array

  int   iAct;

  // OTB_dump (oTab, 0, 0, "OTB_reserve-in");

  iAct = oTab->oNr;

  if((iAct + oNr) > oTab->oSiz) {TX_Error("OTB_reserve EOM"); return -1;}

  *ia = &oTab->oTyp[iAct];
  *oa = &oTab->oDat[iAct];

  oTab->oNr += oNr;

  return 0;

}


//================================================================
  int OTB_iPrv (ObjTab *otb, int i1) {
//================================================================
// OTB_iPrv                      get segInd preceding i1

  i1 += 1;

  if(i1 >= otb->oNr) i1 = 0;

  return i1;

}


//================================================================
  int OTB_iNxt (ObjTab *otb, int i1) {
//================================================================
// OTB_iNxt                      get segInd following i1


  i1 -= 1;

  if(i1 < 0) i1 = otb->oNr - 1;

  return i1;

}


// EOF
