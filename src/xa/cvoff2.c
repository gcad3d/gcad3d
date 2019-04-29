//**************************************************************************
/*
 *
 * Copyright (C) 2019 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
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
VERRUNDUNG mit Inseln:
- nachträglich mit boolean operation behandeln (myReadme.curves)



-----------------------------------------------------
Modifications:
2019-04-13 Neu: UT3D_vc_perp1vc UT3D_pl_plcoe UT3D_plcoe_pl

-----------------------------------------------------
*/
#ifdef globTag
void CVOFF2(){}
#endif
/*!
\file  ../ut/ut_geo3d.c
\brief create offset-contour for planar curves     CVOFF2_*
\code
=====================================================
List_functions_start:

CVOFF2__                   create offsetcontour for planar curve
CVOFF2_set__               create all parallel-contours; store in file;
CVOFF2_get__               get stored loop <crNr> out of file
CVOFF2_otb__               get oTab1 =parallel-contour of DB-curve + plane

CVOFF2_cr__                create parallel contour oTabO to input oTabI
CVOFF2_open_s              contour is open: add 90-deg-circ
CVOFF2_open_e              contour is open: add 90-deg-circ
CVOFF2_close               connect startPtOut-endPtOut with circle
CVOFF2_cr_off              add parallel-contourObjs
CVOFF2_cr_conn             create connecting circle for parallel contour LN-LN
CVOFF2_add                 add obj and bounding-box
CVOFF2_ln__                create startseg, add offset, create closing-seg -> file
CVOFF2_ci__                360-deg-circ: add offset, write into file
CVOFF2_int_trim            get 4 trimmed segments from 2 segments + inters.point
CVOFF2_otb_obj             add bin-3d-obj to ObjTab as 2D-lines,2D-circs
CVOFF2_save__              write all contour-loops into file
CVOFF2_int_ci1             remove circs with antiparallel connection on both ends
CVOFF2_int__               remove intersections from parallel-contour
CVOFF2_int_2o              test intersection of 2 objects
CVOFF2_int_ckConn          ignore connection-points
CVOFF2_int_upd_bwd         update segments ii1 and ii2 of bwd-loop
CVOFF2_int_bl_wr           add fwd-loop to file; <tmpDir>/oibo_parl.dat
CVOFF2_int_bl_rd           read next loop from file into ObjTab
CVOFF2_int_ll              fill 2 ObjRanges with indexes of a loop
CVOFF2_int_save_sr         save loop as output if sense-of-rotation OK
CVOFF2_int_srl1            get sense-of-rotation of contour-loop
CVOFF2_int_save_l1         add closed-loop to output-file
CVOFF2_int_del1            remove the objects between the intersectionpoints
CVOFF2_pt_set              set start- or endPt of bin-obj

CVOFF2_int_load_l          load output (closed-loops)

CVOFF2_init1               fill static struct CO2DAT
CVOFF2_init2               set CO2DAT.iClo
CVOFF2_init_sr             set CO2DAT.sr = sense-of-rotation of contour
CVOFF2_init_lun            open files
CVOFF2__close              close files, free memspc oSpc1

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


Method:
- L_int: find next Intersection;
 - Intersection found: write fwd-loop into tmpfile; remove fwd-loop; goto L_int.
 - No more intersections:
  - test sr; if sr ok write loop into outfile;
  - read next loop out of tmpfile; goto L_int.



*/



#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                         // for ...

// #include <GL/gl.h>                     // GL_TRIANGLES


// #include <errno.h>
// #include <dlfcn.h>           // Unix: dlopen


#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_geo_const.h"        // UT3D_CCV_NUL
#include "../ut/ut_os.h"               // OS_get_bas_dir ..
#include "../ut/ut_ox_base.h"          // OGX_SET_INDEX
#include "../xa/xa_msg.h"              // MSG_* ERR_*
#include "../xa/xa_mem.h"              // memspc51, mem_cbuf1



// prototypes:
  int UT2D_box_dump (Point2 *p1, Point2 *p2, char *txt, ...);
  void _UT3D_pt_mid_ci (Point *po, Circ *ci1);



//===========================================================================
// EXTERNALS:
// from ../xa/xa.c
// extern Plane     WC_sur_act; 


// static struct CO2DAT
//
// typ    DB-type of curve 
// cvi    DB-index of curve
// pln    plane for inputcurve
// dist = offset of contours
// dq     dist * dist
// tol1   UT_TOL_pt
// tolq1  UT_TOL_pt * UT_TOL_pt
// tol2   UT_TOL_cv
// tolq2  UT_TOL_cv * UT_TOL_cv
// bpi    backplane; eg BCKPLN_XY
// bpd    backplane-offset
// crNr   loop-nr to get; 0=all, 1=first, 2=second ..
// iClo   closed; 0=yes, 1=not_closed; -1=undefined
// sr     sense-rotation; 0=CCW; 1=CW
// sr_del sense-of-rotation of loop to delete
// isOut  side of offset-contour; 0=inside; 1=outside; -1=undefined;
// cNr    nr of closed-loops in outfile
// blNr   counter output-loops-read only for testing
// fp_set_w   fileunit for write (add) bwd-loop
// fp_set_r   fileunit for read next bwd-loop
// fp_get     fileunit for in/output of closed-loops
// 
static struct {Refsys rSys; Plane  pln;
               double dist, dq, tol1, tolq1, tol2, tolq2;
               FILE   *fp_set_r, *fp_set_w, *fp_get;
               UINT_32 cks;
               int    crNr, cNr, blwNr, blrNr;
               char   iClo, sr, sr_del, isOut;} CO2DAT = {
               _RSYS_2D,    // rSys
               _PLANE_NUL,  // pln
               0.,          // dist
               0.,          // dist * dist
               0.,          // UT_TOL_pt
               0.,          // UT_TOL_pt * UT_TOL_pt
               0.,          // UT_TOL_cv
               0.,          // tolq2 = UT_TOL_cv * UT_TOL_cv
               NULL,        // fp_set_r
               NULL,        // fp_set_w
               NULL,        // fp_get
               0,           // cks 
               0,           // crNr
               0,           // cNr
               0,           // blwNr
               0,           // blrNr
               (char)0,     // iClo
               (char)0,     // sr
               (char)0,     // sr_del
               (char)-1     // isOut;
          };


// startPt, endPt of input-contour
static struct {Point2 pts, pte;} CO2SEI;


// trimmed intersection-segments
// see CVOFF2_int_trim ()
static struct {int ifwd, ibwd,
                   stat1b, stat1f, stat2f, stat2b,
                   typ1b, typ1f, typ2f, typ2b; 
               char o1b[OBJ_SIZ_MAX], o1f[OBJ_SIZ_MAX],
                    o2f[OBJ_SIZ_MAX], o2b[OBJ_SIZ_MAX];} CO2TIS;



//================================================================
  int CVOFF2__ (ObjGX *oxo, Memspc *oSpc1,
                ObjGX *oCv, ObjGX *oPln, int crNr, double dist) {
//================================================================
// CVOFF2__             create offsetcontour for planar curve
// Input:
//   oCv         dataStruct of (planar) curve to offset
//   oPln        dataStruct of Plane
//   crNr        offsetContour-nr to get; 0=all, 1=first ..
//   dist        offset from curve; pos=left, neg=right of curve
// Output:
//   oxo         offsetContour created
//   oSpc1       space for subObjs for contour created
//   retCod      nr of created contour-loops
//               -1  no contour possible (eg offset > space inside closed curve)
 

  int        irc, i1, cNr, cvTyp;
  void       *cvDat;
  long       dbInd;


  // printf("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO \n");
  // printf("CVOFF2__ crNr=%d dist=%f\n",crNr,dist);
  // DEB_dump_obj__ (Typ_Memspc, oSpc1, "oSpc1");
  // DEB_dump_ox_0 (oCv, "  oCv");
  // DEB_dump_ox_0 (oPln, "  oPln");



  // test for special-cases (eg circle-360-deg)
  irc = CVOFF2_spc__ (oxo, oSpc1, oCv, dist);
  if(irc < 2) return irc;


  // create all parallel-contours; store in file;
  // get cNr = nr of created contour-loops
  cNr = CVOFF2_set__ (oCv, oPln, dist);
  if((cNr < 1)||(crNr > cNr)) {
    printf("**** CVOFF2_set__ Error cNr=%d crNr=%d\n",cNr,crNr);
    return -1;
  }

  // get stored contour crNr -> (oxo,oSpc1)
  irc = CVOFF2_get__ (oxo, oSpc1, crNr);
  if(irc < -1) { printf("**** CVOFF2_get__ irc=%d\n",irc); return -1;}


    // if(crNr <= cNr) i1 = crNr;
    // else {printf("**** CVOFF2_set__ cNr=%d crNr=%d\n",cNr,crNr); return -1;}

/*
    // crNr=0: all; else only <crNr>
    if(!crNr) { ics = 1;    ice = cNr + 1; }       // crNr=0: all; 1=first ..
    else      { ics = crNr; ice = crNr + 1; }
    for(i1=ics; i1<ice; ++i1) {
      irc = CVOFF2_get__ (oxo, oSpc1, i1);
      if(irc < -1) { printf("**** CVOFF2_get__ %d\n",irc); break; }

      // // test output ..
      // test_disp (&oxo, &ccvNr);

    }
*/


  L_exit:
  return cNr;

}


//========================================================================
  int CVOFF2_spc__ (ObjGX *oxo, Memspc *oSpc1, ObjGX *oxi, double dist)  {
//========================================================================
// test for special-cases (eg circle-360-deg)
// retCod:   0  special-case, but no output possible
//           1  special-case; oxo is output; OK.
//           2  no special-case; continue normal

  int    irc, i1, cvTyp;
  long   oSiz;
  double d1;
  void   *cvDat, *oo1;
  // char  oo1[OBJ_SIZ_MAX];


  cvTyp = oxi->typ;

  if(cvTyp != Typ_CI) goto L_exit;


  //----------------------------------------------------------------
  // test for 360-deg-circ
  
  // get data
  cvTyp = UTO_objDat_ox (&cvDat,  &i1, oxi);

  // test 360 deg
  if(UT3D_ck_ci360 (cvDat)) goto L_exit;

  // get spc in oSpc1
  oSiz = sizeof(Circ);
  oo1 = UME_reserve (oSpc1, oSiz);
  if(!oo1) MSG_ERR__ (ERR_EOM, "oSpc1");


  // test radius negativ ..
  d1 = fabs(((Circ*)cvDat)->rad) + dist;


  // change radius += aus_tab[2]
  irc = UT3D_ci_cirad (oo1, cvDat, d1);

  OGX_SET_OBJ (oxo, Typ_CI, Typ_CI, 1, oo1);

  return 1;


  //----------------------------------------------------------------
  L_exit:
  return 2;

 }

 
/*
//================================================================
  int CVOFF2__close (Memspc *oSpc1) {
//================================================================
// CVOFF2__close              close files, free memspc oSpc1
 
  printf("CVOFF2__close \n");

  // free data
  UME_free (oSpc1);

  // close files
  if(CO2DAT.fp_set_r) {fclose (CO2DAT.fp_set_r); CO2DAT.fp_set_r = NULL;}
  if(CO2DAT.fp_set_w) {fclose (CO2DAT.fp_set_w); CO2DAT.fp_set_w = NULL;}
  if(CO2DAT.fp_get) {fclose (CO2DAT.fp_get); CO2DAT.fp_get = NULL;}

  return 0;

}
*/

//================================================================
  int CVOFF2_set__ (ObjGX *oxCv, ObjGX *oxPln, double dist) {
//================================================================
// create all parallel-contours; store in file;
// Input:
//   oCv       curve to offset; must be DB-obj
//   oPln      plane of oCv; must be bin-obj
//   dist      offset of parallelcurve, pos or neg


  int        irc, typCv, typPl, oNr;
  UINT_32    cks;
  long       l1;
  Plane      *objPl;
  void       *objCv;


  // get curve
  OGX_GET_OBJ (&typCv, &objCv, &oNr, oxCv);

  // get objPl = Plane*
  OGX_GET_OBJ (&typPl, &objPl, &oNr, oxPln);
  if(typPl != Typ_PLN) {TX_Error("CVOFF2_set__ E1-%d",typPl); return -1;}


  // printf("================================================== \n");
  // printf("CVOFF2_set__ dist=%f\n",dist);


  // get cks = checksum of curve-data
  l1 = UTO_siz_stru (typCv);
  cks = UTI_checksum__ (objCv, l1);



  //================================================================
  // test if stored offset = dist
  if(CO2DAT.dist != dist) goto L_work;

  // test if stored cv = oCv 
  if(CO2DAT.cks != cks) goto L_work;

  // compare plane (binary data)
  if(MEM_cmp__(objPl, &CO2DAT.pln, sizeof(Plane))) goto L_work;
  

  // all loops of cv <dbInd> are already stored in file ..
  goto L_read__;



  //================================================================
  // get all loops into file
  L_work:

  // fill CO2DAT
  irc = CVOFF2_init1 (cks, dist, UT_TOL_pt, UT_TOL_cv);
  if(irc < 0) return -1;

  // open file for set
  CVOFF2_set_lun ();

  // get all parallel-contour(s) into file
  irc = CVOFF2_otb__ (oxCv, oxPln);
  if(irc < 0) return -1;

  // close files fp1; keep fp_get open.
  if(CO2DAT.fp_set_r) {fclose (CO2DAT.fp_set_r); CO2DAT.fp_set_r = NULL;}
  if(CO2DAT.fp_set_w) {fclose (CO2DAT.fp_set_w); CO2DAT.fp_set_w = NULL;}
  if(CO2DAT.fp_get)   {fclose (CO2DAT.fp_get);   CO2DAT.fp_get = NULL;}

    // TESTBLOCK
    // ERR_raise (__func__);
    // END TESTBLOCK


  //================================================================
  // get already stored loop <crNr> out of file
  L_read__:
    // printf("ex-CVOFF2__dbo-L_read__ %d\n",CO2DAT.cNr);


  return CO2DAT.cNr;

}


//================================================================
  int CVOFF2_get__ (ObjGX *oxo, Memspc *oSpc, int crNr) {
//================================================================
// CVOFF2_get__    get stored loop <crNr> out of file
// Input:
//   crNr   contour-nr; 0=all, 1=first ..
// Output:
//   oxo    CCV
//   oSpc   CCV-data; existing ob's are removed !
//
// TODO: provide output also in other formats (2D, polygon ..)

 
  int        irc;
  ObjTab     oTab1 = _OBJTAB_NUL;
  ObjTab     oTab2 = _OBJTAB_NUL;


  // printf("CVOFF2_get__ %d\n",crNr);


  // Test if index of requested loop > nr of found loops
  if(crNr > CO2DAT.cNr) {
    printf("***** - only %d loops found ..\n",CO2DAT.cNr);
    return -1;
  }


  // get memspc for oTab1 for 2D-segments only (no BBox2)
  OTB_init (&oTab1, 0, Typ_GEOB_2D,
               memspc102, sizeof(memspc102),
               memspc201, sizeof(memspc201));


  // open lun for fp_get
  irc = CVOFF2_get_lun ();
  if(irc < 0) return -1;


  // load 2D-contur crNr from file into oTab1
  CO2DAT.crNr = crNr;
  irc = CVOFF2_int_load_l (&oTab1);
  if(irc < 0) return -1;

    // TESTBLOCK
    // printf(" nr-of-contours = %d\n",CO2DAT.cNr);
    // OTB_dump (&oTab1, 11, Typ_Att_Symb, "ex-CVOFF2_int_load_l");
    // ERR_raise (__func__);
    // END TESTBLOCK


  //================================================================
  // transfer 2D-objects (oTab1) back -> 3D (oTab2)
  // OTB_reset__ (&oTab1, 1);       // reset space, need no BBox2

  // get memspc oTab2 for 3D-objs, no boxes
  OTB_init (&oTab2, 0, Typ_GEOB_3D,
               memspc101, sizeof(memspc101),
               memspc501, sizeof(memspc501));


  // transfer 2D -> 3D
  irc = OTB_otb_tra_otb2_rsys (&oTab2, &oTab1, &CO2DAT.rSys);
  if(irc < 0) {TX_Error("CVOFF2_otb__ E10-%d",irc); return -1;}


    // TESTBLOCK
    // OTB_dump (&oTab2, 1, Typ_Att_Symb, "ex-OTB__tra_otb2_bp");
    // ERR_raise (__func__);
    // END TESTBLOCK


//================================================================
  // change 3D-contour oTab2 -> CCV 

  // reset space in oSpc (deletes contour stored before !)
  UME_reset (oSpc);
  // UME_init (oSpc, memspc201, sizeof(memspc201));

  // in:  oTab2 (memspc101,memspc501)
  // out: oSpc  (memspc201)
  irc = CVTRM_ocv_otb__ (oxo, oSpc, &oTab2);
  if(irc < 0) return -1;

      // TESTBLOCK
      // store oOut = CCV into DB
      // get next free curve-index
      // { long       dbi1;
        // dbi1 = DB_dbo_get_free (Typ_CV);   printf("  dbi = %ld\n",dbi1);
        // if(!dbi1) return -1;
        // irc = DB_StoreCvCCV (dbi1, oxo->data, oxo->siz);
        // if(irc < 0) return -1;
        // APT_Draw__ (0, Typ_CV, dbi1); // display CCV
      // }
      // END TESTBLOCK


//================================================================
  L_exit:

    // close lun
    if(CO2DAT.fp_get) {fclose (CO2DAT.fp_get); CO2DAT.fp_get = NULL;}

  // return CO2DAT.cNr;
  return 0;

}


//================================================================
  int CVOFF2_get_lun () {
//================================================================
// Input:
//
// file <tmpdir>CVOFF2_get.dat
//   contours finished;
//   read:fp_get;


  char       fnam[256];


  // printf("CVOFF2_get_lun \n");


  sprintf(fnam, "%sCVOFF2_get.dat",OS_get_tmp_dir());

  if((CO2DAT.fp_get = fopen(fnam,"rb")) == NULL) {
    TX_Print("CVOFF2_get_lun E1-%s\n",fnam);
    return -1;
  }

  return 0;

}


//================================================================
  int CVOFF2_set_lun () {
//================================================================
// Input:
//   mode     1=open fp_set_r,fp_set_w        (_set_)
//            2=open fp_get              (_get_)
//
// file <tmpdir>CVOFF2_get.dat
//   contours finished;
//   write&read:fp_get;
//
// file <tmpdir>CVOFF2_get.dat
//   loops not yet finished;
//   write:fp_set_w; read:fp_set_r;

  char       fnam[256];


  // printf("CVOFF2_set_lun \n");


  sprintf(fnam, "%sCVOFF2_set.dat",OS_get_tmp_dir());

  if((CO2DAT.fp_set_w = fopen(fnam,"wb")) == NULL) {
    TX_Print("CVOFF2_set_lun E1-%s\n",fnam);
    return -1;
  }

  if((CO2DAT.fp_set_r = fopen(fnam,"rb")) == NULL) {
    TX_Print("CVOFF2_set_lun E2-%s\n",fnam);
    return -1;
  }


  sprintf(fnam, "%sCVOFF2_get.dat",OS_get_tmp_dir());

  if((CO2DAT.fp_get = fopen(fnam,"wb")) == NULL) {
    TX_Print("CVOFF2_set_lun E3-%s\n",fnam);
    return -1;
  } 


  return 0;

}


//=============================================================================
  int CVOFF2_init1 (UINT_32 cks, double dist, double tol1, double tol2) {
//=============================================================================

  // printf("CVOFF2_init1 \n");


  CO2DAT.cks  = cks;   // checksum curve
  CO2DAT.dist = dist;
  CO2DAT.tol1 = tol1;  // UT_TOL_pt
  CO2DAT.tolq1 = tol1 * tol1;  // UT_TOL_pt * UT_TOL_pt
  CO2DAT.tol2 = tol2;  // UT_TOL_cv
  CO2DAT.tolq2 = tol2 * tol2;  // UT_TOL_cv * UT_TOL_cv
  CO2DAT.iClo = -1;    // closed; 0=yes, 1=not_closed; -1=undefined
  CO2DAT.blwNr = 0;
  CO2DAT.blrNr = 0;

  CO2DAT.dq   = pow ((fabs(dist) - tol1), 2.);

  CO2DAT.cNr = 0;      // nr of loops stored

  return 0;

}


//====================================================================
  int CVOFF2_init2 (int iClo) {
//====================================================================

  // printf("CVOFF2_init2 iClo=%d\n",iClo);

  CO2DAT.iClo = iClo;    // closed; 0=yes, 1=not_closed; -1=undefined

  return 0;

}


//====================================================================
  int CVOFF2_init_sr (ObjTab *oTab1) {
//====================================================================
// get sense-of-rotation of oTab1

  int     irc, isr, ptNr, ptMax;
  double  d1;
  Point2  *pa;


  // printf(" IIIIIIIIIIIIIIIIIIIIIIIIIIIII CVOFF2_init_sr %d\n",oTab1->oNr);
  // OTB_dump (oTab1, 1, 0, "-init_sr-3");   // 1=dump 3=disp & dump


  if(CO2DAT.iClo == 1) {
    // contour is not closed !
    CO2DAT.sr     = 0;   // 0=CCW
    CO2DAT.isOut  = 0;   // 0=inside; 1=outside
    CO2DAT.sr_del = 0;
    goto L_exit;
  }


  //----------------------------------------------------------------
  // get space for points. get space for oNr * 2 + 100 points
  ptMax = oTab1->oNr + 100;     
    // printf(" CVOFF2_init_sr ptMax=%d\n",ptMax);
  pa = (Point2*)MEM_alloc_tmp (ptMax * sizeof(Point2));
  if(!pa) {TX_Error("CVOFF2_init_sr EOM");return -1;}


  // get polygon from 2D-contour
  // irc = UT2D_npt_sym_nobj (&ptNr, pa, oTab1->oNr, oTab1->oTyp, oTab1->oDat);
  ptNr  = 0;
  irc = UT2D_npt_otb (&ptNr, pa, ptMax, oTab1, &CO2DAT.tol1, 1);
  if(irc < 0) return -1;

    // TESTBLOCK
    // DEB_dump_nobj__ (Typ_PT2, ptNr, pa, "init_sr-otb");
    // GR_Disp_cv2 (pa, ptNr, Typ_Att_hili);
    // END TESTBLOCK


  //----------------------------------------------------------------
  // open contours:
  // if(CO2DAT.iClo > 0) {
    // if(CO2DAT.dist > 0.) CO2DAT.sr_del = 1;
    // else                 CO2DAT.sr_del = 0;
    // goto L_exit;
  // }


  //----------------------------------------------------------------
  // closed contours:
  // get sense-of-rotation of polygon; 1=CCW, -1=CW
  isr = UT2D_srar_polc (&d1, ptNr, pa); 
    // printf("  area-main:%f\n",d1);

  
  // sense-of-rotation -> 0=CCW, 1=CW.
  CO2DAT.sr = (isr > 0) ? 0 : 1;

  //----------------------------------------------------------------
  // set sr_del = sense-of-rotation of loop to delete
  // detect inside or outside:
  // - CCW +dist = inside; -dist = outside
  // - CW: revert
  // set CO2DAT.isOut; 0=inside; 1=outside
  if(!CO2DAT.sr) {
    // CCW
    CO2DAT.isOut = (CO2DAT.dist > 0.) ? 0 : 1;
  } else {
    // CW
    CO2DAT.isOut = (CO2DAT.dist > 0.) ? 1 : 0;
  }

  

  if(CO2DAT.isOut) {
    // outside
    // if outside: delete loop equal input-sr, but NOT first loop
    CO2DAT.sr_del = CO2DAT.sr;
  } else {
    // inside
    // if inside: delete loop not-equal input-sr
    CO2DAT.sr_del = ICHG01(CO2DAT.sr);
  }
    // printf(" sr_del=%d isOut=%d\n",CO2DAT.sr_del,CO2DAT.isOut);

  // if inside: delete loop not-equal input-sr
  // if outside: delete loop equal input-sr, but NOT first loop


  //----------------------------------------------------------------
  L_exit:
    // printf("ex-_init_sr sr_del=%d sr=%d isr=%d\n",CO2DAT.sr_del,CO2DAT.sr,isr);
  return 0;

}


//===========================================================================
  int CVOFF2_int_trim (int typ1, void *obj1, int typ2, void *obj2,
                       Point2 *ptx, int irc, int isRev) {
//===========================================================================
// CVOFF2_int_trim    get 4 trimmed segments from 2 segments + inters.point
// auxFun to separate 2 loops; fwd-loop and bwd-loop
// obj1 is separated into o1b and o1f (both of type typ1)
// obj2 is separated into o2f and o2b (both of type typ2)
//
// Input:
// Output:
//   CO2TIS
//
//
//     <-------<           >---------> 
//    |          \       /            |
//    |           \o2b  /o1f          |
//    |    ii2/obj2\   /obj1          |
//    |             \ /      fwd-loop |
//    | bwd-loop     x ptx            |
//    |             / \               |
//    |    ii1/obj1/   \obj2          |
//    |           /o1b  \o2f          |
//    |          /       \            |
//    x--------->         <----------< 
// startPt



  // aTs - codes: 0 = unmodif; 1=set; 2=NULL.
  // s sequence: o1b o1f o2f o2b
  char    aTs[] = {1, 1, 1, 1,       //  0 irc=0, isRev=0
                   2, 0, 1, 1,       //  4 irc=1, isRev=0
                   1, 1, 2, 0,       //  8 irc=4, isRev=0
                   0, 2, 1, 1,       // 12 irc=2, isRev=0
                   1, 1, 0, 2,       // 16 irc=8, isRev=0
                   2, 0, 2, 0,       // 20 irc=5, isRev=0
                   0, 2, 2, 0,       // 24 irc=6, isRev=0
                   2, 0, 0, 2,       // 28 irc=9, isRev=0
                   0, 2, 0, 2,       // 32 irc=10, isRev=0
                   0, 2, 0, 2,       // 36 irc=1, isRev=1
                   9, 9, 9, 9};      // UU
  char    *pTs;



  // printf("CVOFF2_int_trim irc=%d isRev=%d\n",irc,isRev);
  // DEB_dump_obj__ (Typ_PT2, ptx, " ptx");
  // GR_Disp_pt2 (ptx, SYM_TRI_S, ATT_COL_RED);
  // GR_Disp_obj (typ1, obj1, Typ_Att_Symb, 0);
  // GR_Disp_obj (typ2, obj2, Typ_Att_Symb, 0);


  // copy all types
  CO2TIS.typ1b = typ1;
  CO2TIS.typ1f = typ1;
  CO2TIS.typ2f = typ2;
  CO2TIS.typ2b = typ2;


  // copy all objects
  memcpy (&CO2TIS.o1b, obj1, OBJ_SIZ_MAX);
  memcpy (&CO2TIS.o1f, obj1, OBJ_SIZ_MAX);

  memcpy (&CO2TIS.o2f, obj2, OBJ_SIZ_MAX);
  memcpy (&CO2TIS.o2b, obj2, OBJ_SIZ_MAX);


  //----------------------------------------------------------------
  if(irc == 0) {
  // Func   isRev RC
  // int_ln_ci 0  0  o1=ln, o2=ci;  on ln1 == on ci2
  // int_ci_ci 0  0  o1=c1, o2=c2;  on ci1 == on ci2
  // int_2lnl  0  0  o1=ln, o2=ln;  on ln1 == on ln2
  // 
  //          <-----
  //         |  b   2                   RC = 0     on o1 == on o1
  //         |       2                 
  //         >11111111x111111111
  //      start-->     2        |
  //                    2     f |
  //                     <------
  //   o1b=set; o1f=set;
  //   o2f=set; o2b=set;

  pTs = &aTs[0];
  goto L_trim;


  //----------------------------------------------------------------
  } else if(irc == 1) {
  // Func   isRev RC
  // int_ln_ci 0  1  o1=ln, o2=ci;  startPt ln1 == on ci2
  // int_ci_ci 0  1  o1=c1, o2=c2;  startPt ci1 == on ci2
  // int_2lnl  0  1  o1=ln, o2=ln;  startPt ln1 == on ln2
  // 
  //          <-----
  //         |  b   2                   RC = 1     startPt ln1 == on ci1
  //         |       2                  RC = 1     startPt ci1 == on ci2
  //         >--------x111111111 
  //      start->      2        |
  //                    2     f |
  //                     <------
  //   o1f=NULL; o1b=unMod.
  //   o2b=set; o2f=set;
  
  if((isRev)&&(typ2 == Typ_LN2)) goto L_4;

  pTs = &aTs[4];
  goto L_trim;


  //----------------------------------------------------------------
  } else if(irc == 4) {
  L_4:
  // Func   isRev RC
  // int_ln_ci 1  4  o1=ci, o2=ln;  startPt ci2 == on ln1
  // int_ci_ci 1  4  o1=c2, o2=c1;  startPt ci2 == on ci1
  // int_2lnl  0  4  o1=ln, o2=ln;  startPt ln2 == on ln1
  // int_ln_ci 1  1  o1=ci, o2=ln;  startPt ln2 == on ci1
  // 
  //          <-----
  //         |  b   2                   RC = 4     startPt ci2 == on ln1
  //         |       2                  RC = 4     startPt ci2 == on ci1
  //         >11111111x111111111 
  //      start->      \        |
  //                    \     f |
  //                     <------
  //   o1f=set; o1b=set.
  //   o2b=NULL; o2f=unMod;
  
  pTs = &aTs[8];
  goto L_trim;


  //----------------------------------------------------------------
  } else if(irc == 2) {
  L_2:
  // Func  isRev RC
  // int_ln_ci 0  2  o1=ln, o2=ci;  endPt ln1 == on ci2
  // int_ln_ci 1  8  o1=ci, o2=ln;  endPt ci1 == on ln2
  // int_ci_ci 0  2  o1=c1, o2=c2;  endPt ci1 == on ci2
  // int_2lnl  0  2  o1=ln, o2=ln;  endPt ln1 == on ln2
  // 
  //          <-----
  //         |  b   2                   RC = 2     endPt ln1 == on ci2
  //         |       2                  RC = 8     endPt ci1 == on ln2
  //         >11111111x---------
  //      start->      2        |
  //                    2    f  |
  //                     <------
  //   o1f=unMod; o1b=NULL.
  //   o2b=set; o2f=set;

  pTs = &aTs[12];
  goto L_trim;



  //----------------------------------------------------------------
  } else if(irc == 8) {
  if((isRev)&&(typ2 == Typ_LN)) goto L_2;
  // Func  isRev RC
  // int_ci_ci 1  8  o1=c2, o2=c1;  endPt ci2 == on ci1
  // int_2lnl  0  8  o1=ln, o2=ln;  endtPt ln2 == on ln1
  // 
  //          <-----
  //         |  b   \                   RC = 8     endPt ci2 == on ci1
  //         |       \
  //         >11111111x111111111
  //      start->      2        |
  //                    2    f  |
  //                     <------
  //   o1f=set; o1b=set.
  //   o2b=unMod; o2f=NULL;

  pTs = &aTs[16];
  goto L_trim;


  //----------------------------------------------------------------
  } else if(irc == 5) {
  // Func  isRev RC
  // int_ln_ci 0  5  o1=ln, o2=ci;  startPt ln1 == startPt ci1
  // int_ci_ci 0  5  o1=c1, o2=c2;  startPt ci1 == startPt ci2
  // int_2lnl  0  5  o1=ln, o2=ln;  startPt ln1 == startPt ln2
  // 
  //          <-----
  //         |  b   2                   RC = 5    startPt ln1 == startPt ci1
  //         |       2
  //         >--------x111111111
  //      start->      \        |
  //                    \    f  |
  //                     <------
  //   o1f=NULL; o1b=unMod;
  //   o2b=NULL; o2f=unMod;

  pTs = &aTs[20];
  goto L_trim;


  //----------------------------------------------------------------
  } else if(irc == 6) {
  L_6:
  // Func  isRev RC
  // int_ln_ci 0  6  o1=ln, o2=ci;  endPt ln1 == startPt ci2
  // int_ln_ci 1  9  o1=ci, o2=ln;  endPt ci1 == startPt ln2
  // int_ci_ci 0  6  o1=c1, o2=c2;  endPt ci1 == startPt ci2
  // int_2lnl  0  6  o1=ln, o2=ln;  endtPt ln1 == startPt ln2
  // 
  //          <-----
  //         |  b   2                   RC = 6    endPt ln1 == startPt ci1
  //         |       2                  RC = 9    endPt ci1 == startPt ln1
  //         >11111111x---------        RC = 6    endPt ci1 == startPt ci2
  //      start->      \        |
  //                    \     f |
  //                     <------
  //   o1f=unMod; o1b=NULL;
  //   o2b=NULL; o2f=unMod;

  pTs = &aTs[24];
  goto L_trim;


  //----------------------------------------------------------------
  } else if(irc == 9) {
  if((isRev)&&(typ2 == Typ_LN2)) goto L_6;
  // Func  isRev RC
  // int_ci_ci 1  9  o1=c2, o2=c1;  endPt ci2 == startPt ci1
  // int_2lnl  0  9  o1=ln, o2=ln;  startPt ln1 == endtPt ln2
  // 
  //          <-----
  //         |  b   \                   RC = 9    endPt ci2 == startPt ci1
  //         |       \
  //         >--------x111111111
  //      start->      2        |
  //                    2     f |
  //                     <------
  //   o1f=NULL; o1b=unMod;
  //   o2b=unMod; o2f=NULL;

  pTs = &aTs[28];
  goto L_trim;


  //----------------------------------------------------------------
  } else if(irc == 10) {
  // Func  isRev RC
  // int_ln_ci 0 10  o1=ln, o2=ci;  endPt ln1 == endPt ci2
  // int_ln_ci 1 10  o1=ci, o2=ln;  endPt ci1 == endPt ln2
  // int_ci_ci 0 10  o1=c1, o2=c2;  endPt ci1 == endPt ci2
  // int_ci_ci 1 10  o1=c2, o2=c1;  endPt ci2 == endPt ci1
  // int_2lnl  0 10  o1=ln, o2=ln;  endtPt ln1 == endtPt ln2
  // 
  //          <-----
  //         |  b   \                   RC = 10
  //         |       \
  //         >11111111x---------
  //      start->      2        |
  //                    2     f |
  //                     <------
  //   o1f=unMod; o1b=NULL;
  //   o2b=unMod; o2f=NULL;

  pTs = &aTs[32];
  goto L_trim;


  //----------------------------------------------------------------
  } else { TX_Error("CVOFF2_int_trim E1 %d",irc); return -1; }




  //----------------------------------------------------------------
  L_trim:
  
    // printf(" int_trim = %d %d %d %d\n",pTs[0],pTs[1],pTs[2],pTs[3]);

  // set stat*
  CO2TIS.stat1b = pTs[0];
  CO2TIS.stat1f = pTs[1];
  CO2TIS.stat2f = pTs[2];
  CO2TIS.stat2b = pTs[3];



  // trim objects. 2=endPt; 1=startPt;
  if(CO2TIS.stat1b == 1) CVOFF2_pt_set (CO2TIS.typ1b, &CO2TIS.o1b, ptx, 2);

  if(CO2TIS.stat1f == 1) CVOFF2_pt_set (CO2TIS.typ1f, &CO2TIS.o1f, ptx, 1);

  if(CO2TIS.stat2f == 1) CVOFF2_pt_set (CO2TIS.typ2f, &CO2TIS.o2f, ptx, 2);

  if(CO2TIS.stat2b == 1) CVOFF2_pt_set (CO2TIS.typ2b, &CO2TIS.o2b, ptx, 1);



    // TESTBLOCK
    // GR_Disp_obj (CO2TIS.typ1f, &CO2TIS.o1f, Typ_Att_Symb, 0);
    // GR_Disp_obj (CO2TIS.typ1b, &CO2TIS.o1b, Typ_Att_hili, 0);
    // GR_Disp_obj (CO2TIS.typ2f, &CO2TIS.o2f, Typ_Att_Symb, 0);
    // GR_Disp_obj (CO2TIS.typ2b, &CO2TIS.o2b, Typ_Att_hili, 0);
    // ERR_raise (__func__);
    // END TESTBLOCK


  return 0;

}


//========================================================================
 int CVOFF2_otb__ (ObjGX *oCv, ObjGX *sur) {
//========================================================================
// CVOFF2_otb__         get oTab1 =parallel-contour of DB-curve + plane
//
// Input:
//   oCv        contour to offset
//   sur        plane of input-contour
//   CO2DAT     offsetdistance, tolerance ..
// Output:
//   retcod     nr of outputContours created
//   -          all outputContours -> file CVDAT.fp_get
//
// USED MEMSPACES: memspc101,memspc102,memspc201,memspc501


  int        irc, i1, cv3Typ, su3Typ, iNr, oNr, ptNr, ptMax, oSiz;
  long       dbInd;
  void       *cv3Dat, *su3Dat;
  int        *oiboTyp, oiboSizT, oiboNr;
  void       *oiboDat;
  double     bpd;
  Point      *pa3, p21, p22;
  Plane      oPln;
  Memspc     o2iSpc;
  ObjTab     oTab1;


  // printf("CVOFF2_otb__ %f\n",CO2DAT.dist);
  // DEB_dump_obj__ (Typ_ObjGX, oCv, "CVOFF2_otb__-oCv ");
  // DEB_dump_obj__ (Typ_ObjGX, sur, "sur ");



  //----------------------------------------------------------------
  //  and bpi = backplane and bpd = offset of backplane
  OGX_GET_OBJ (&su3Typ, &su3Dat, &iNr, sur);
  if(su3Typ == Typ_PLN) {
    // store plane in CO2DAT
    // CO2DAT.pln = *(Plane*)sur->data;
    // get Refsys rSys (backplane| transf.matrix)
    UT3D_rsys_pl (&CO2DAT.rSys, su3Dat);
      // DEB_dump_obj__ (Typ_Refsys, &CO2DAT.rSys, "CO2DAT.rSys");

  } else {
    TX_Error("CVOFF2_otb__ not-planar");
    return -1;
  }



  //----------------------------------------------------------------
  // resolv oCv
  // get input-curve (cv3Typ,cv3Dat); must be data-struct
  OGX_GET_OBJ (&cv3Typ, &cv3Dat, &iNr, oCv);


  //================================================================
  // make oTab1 = 2D-obj's from bin-3D-objs (cv3Typ,cv3Dat,iNr)

  // get memspc for 2D-input-objects see ../xa/xa_mem.h
  UME_init (&o2iSpc, memspc55, sizeof(memspc55));

  // get memspc for oTab1 for 2D-segments only
  OTB_init (&oTab1, 0, Typ_GEOB_2D,
               memspc102, sizeof(memspc102),
               memspc201, sizeof(memspc201));

  // make oTab1 = Offsettable-Idependent-Binary-2D-Objects of inputObjects;
  //   offsettable-objects = LN,CI; (Elli, Bspl ?)
  i1 = 0;
  L_nxto:
    // add bin-3D-objs (cv3Typ,cv3Dat) to oTab1 as 2D-obj
    irc = CVOFF2_otb_obj (&oTab1, cv3Typ, cv3Dat, &CO2DAT.rSys);
    if(irc < 0) return -1;
    if(iNr > 1) {
      if(!i1) oSiz = UTO_siz_stru (cv3Typ);
      cv3Dat = (char*)cv3Dat + oSiz;
      ++i1;
      if(i1 < iNr) goto L_nxto;
    }

    // TESTBLOCK
    /// have now oTab1 = 2D-input-contour as 
    // OTB_dump (&oTab1, 1, Typ_Att_Symb, "-oTab1-3");  // dump only
    // OTB_dump (&oTab1, I_BIN(01011), Typ_Att_Symb, "-Tab1-3");
    // ERR_raise (__func__);
    // END TESTBLOCK



  //================================================================
  // set CO2DAT.iClo = close-flag (if contour is closed or not)
    // get startPoint of contour oTab1
    UT2D_ptvcpar1_std_obj (&CO2SEI.pts, NULL, NULL,
                           Ptyp_start, oTab1.oTyp[0], oTab1.oDat[0]);
    // get endPoint of contour oTab1
    iNr = oTab1.oNr - 1;
    UT2D_ptvcpar1_std_obj (&CO2SEI.pte, NULL, NULL,
                           Ptyp_end, oTab1.oTyp[iNr], oTab1.oDat[iNr]);
    // compare points
    // set iClo closed; 0=yes, 1=not_closed; -1=undefined; -2=degen
    CO2DAT.iClo = ICHG01(UT2D_comp2pt (&CO2SEI.pts, &CO2SEI.pte, UT_DISP_cv));
      // printf(" CO2DAT.iClo -> %d\n",CO2DAT.iClo);



  //================================================================
  // get CO2DAT.sr = sense-of-rotation of input-contour oTab1
  irc = CVOFF2_init_sr (&oTab1);
  if(irc < 0) {TX_Error("CVOFF2_otb__ E6-%d",irc); return -1;}



  //================================================================
  // create all contour-loops and write into file
  irc = CVOFF2_save__ (&oTab1);
  if(irc < 0) return irc;



  //----------------------------------------------------------------
  L_exit:
  return CO2DAT.cNr;


  L_Err_Sur1:
    // not planar .. (Vektorband .. ??)
    TX_Error("CVOFF2_otb__ TODO-2");
    return -1;


}


//================================================================
  int CVOFF2_ln__ (ObjTab *otbo, ObjTab *otbi) {
//================================================================
// create startseg, add offset, create closing-seg, write into file, done.

  int    irc = 0;
  Line2  lno, *lni;


  lni = otbi->oDat[0];

    // DEB_dump_obj__ (Typ_LN2, lni, "CVOFF2_ln__ ");
    // printf(" iClo=%d dist=%f\n",CO2DAT.iClo,CO2DAT.dist);


  //----------------------------------------------------------------
  if(!CO2DAT.iClo) goto L_ln;
  // not closed; add startSeg
  irc = CVOFF2_open_s (otbo, otbi, 1);
  if(irc < 0) {TX_Error("CVOFF2_ln__ E-1"); return -1;}


  //----------------------------------------------------------------
  L_ln:

  UT2D_ln_parl_ln (&lno, lni, &CO2DAT.dist);

  // add new circ
  irc = CVOFF2_add (otbo, Typ_LN2, &lno);
  if(irc < 0) {TX_Error("CVOFF2_ln__ E-2"); return -1;}
    // OTB_dump (otbo, 11, Typ_Att_Symb, "CVOFF2_ci__-360");


  //----------------------------------------------------------------
  L_close:
  if(!CO2DAT.iClo) goto L_exit;
  // not closed; add exitSeg
  irc = CVOFF2_open_e (otbo, otbi, 1);
  if(irc < 0) {TX_Error("CVOFF2_ln__ E-3"); return -1;}


  //----------------------------------------------------------------
  L_exit:


    // TESTBLOCK
    // printf("ex-CVOFF2_ln__ %d\n",irc);
    // OTB_dump (otbo, I_BIN(00011), Typ_Att_Symb, "ex-CVOFF2_cr_off");
    // ERR_raise (__func__);
    // END TESTBLOCK

  return irc;

}


//================================================================
  int CVOFF2_ci__ (ObjTab *otbo, ObjTab *otbi) {
//================================================================
// if 360-deg-circ: add offset, write into file, done.
// else: create startseg, add offset, create closing-seg, write into file, done.
// retCod: nr-of-contours;

  int     irc = 0, i1, i2, srot;
  double  d1, d2, d3;
  Vector2 vc1;
  Circ2   cio, *cii;


  cii = otbi->oDat[0];
  srot = UT2D_sr_ci (cii);

    // DEB_dump_obj__ (Typ_CI2, cii, "CVOFF2_ci__ ");
    // printf("CVOFF2_ci__ iClo=%d dist=%f srot=%d\n",CO2DAT.iClo,CO2DAT.dist,srot);


  // // get memspc for otbo for max 3 segments
  // i1 = (sizeof(int) + sizeof(void*)) * 5;
  // i2 = OBJ_SIZ_MAX * 5;
  // OTB_init (&otbo, 0, 
            // MEM_alloc_tmp(i1), i1,
            // MEM_alloc_tmp(i2), i2);




  //----------------------------------------------------------------
  if(!CO2DAT.iClo) goto L_ci;
  // not closed; add startSeg
  irc = CVOFF2_open_s (otbo, otbi, 1);
  if(irc < 0) {TX_Error("CVOFF2_ci__ E-1"); return -1;}


  //----------------------------------------------------------------
  L_ci:
  // circle - add offset
  d1 = fabs(cii->rad);
  if(srot) d2 = d1 + CO2DAT.dist;      // 1=CW
  else     d2 = d1 - CO2DAT.dist;      // 0=CCW 
  // if(d2 < 0.) { irc = 0; goto L_exit; }   // no contour possible
  if(fabs(d2) < CO2DAT.tol1) goto L_close;
 
  // set cio = new circ
  UT2D_ci_ci_rad (&cio, cii, d2);

    // DEB_dump_obj__ (Typ_CI2, &cio, " cio ");
    // GR_Disp_ci2 (&cio, Typ_Att_dash__);

  // add new circ
  irc = CVOFF2_add (otbo, Typ_CI2, &cio);
  if(irc < 0) {TX_Error("CVOFF2_ci__ E-2"); return -1;}
    // OTB_dump (otbo, 11, Typ_Att_Symb, "CVOFF2_ci__-360");


  //----------------------------------------------------------------
  L_close:
  if(!CO2DAT.iClo) goto L_exit;
  // not closed; add exitSeg
  irc = CVOFF2_open_e (otbo, otbi, 1);
  if(irc < 0) {TX_Error("CVOFF2_ci__ E-3"); return -1;}


  //----------------------------------------------------------------
  L_exit:

    // TESTBLOCK
    // printf("ex-CVOFF2_ci__ %d\n",irc);
    // OTB_dump (otbo, 1, Typ_Att_Symb, "ex-CVOFF2_ci__");
    // OTB_dump (otbo, I_BIN(00011), Typ_Att_Symb, "ex-CVOFF2_cr_off");
    // ERR_raise (__func__);
    // END TESTBLOCK

  return irc;

}


//=======================================================================
  int CVOFF2_otb_obj (ObjTab *otbo, int oTyp, void *oDat, Refsys *rSys) {
//=======================================================================
// add bin-3D-obj to ObjTab as 2D-lines,2D-circs.
// Do not change circle into polygon, change all other obj's into polygon.
// Input:
//   oTyp         type of oDat
//   oDat         bin-3d-obj
//   rsys         plane, backplane, matrix
// Output:
//   otbo                               (memspc102,memspc201)

// USED MEMSPACES:
// memspc101,memspc102,memspc201,memspc501  see ../xa/xa_mem.h

  int        irc, i1, ptNr, ptMax, typ3D, typ2D, typ1;
  Point      *pa3;
  void       *o3D, *o2D;
  Memspc     o2Spc;
  char       cv3[OBJ_SIZ_MAX];
  char       cv2[OBJ_SIZ_MAX];


  // printf(":::::::::::: CVOFF2_otb_obj typ=%d bpi=%d\n",oTyp,rSys->bpi);
  // DEB_dump_obj__ (oTyp, oDat, "oDat");
  

  typ3D = oTyp;
  o3D   = oDat;
  o2D   = cv2;


  //----------------------------------------------------------------
  if(oTyp == Typ_LN) {    // Line
    // nothing to do ..

  //----------------------------------------------------------------
  } else if(oTyp == Typ_CI) {
    // nothing to do ..

  //----------------------------------------------------------------
  } else if(oTyp == Typ_CVPOL) {    // CurvPoly
    // nothing to do ..

  //----------------------------------------------------------------
  } else if(oTyp == Typ_CVBSP) {   // CurvBSpl

    // get space pa3 for 3D-points of bspl iDat
    pa3 = (Point*) memspc501;
    ptMax = sizeof(memspc501) / sizeof(Point);

    // make polygon pa3 from bspl
    irc = UT3D_pta_bsp (&ptNr, pa3, oDat, ptMax, UT_DISP_cv);
    if(irc < 0) return -1;
      // GR_Disp_cv (pa3, ptNr, Typ_Att_dash_long);
      // DEB_dump_nobj__ (Typ_PT, ptNr, pa3, "pta_bsp-otb_obj");

    // make cv3 = CurvPoly with points = pa3[ptNr]
    UT3D_plg_npt ((CurvPoly*)cv3, pa3, ptNr);

    typ3D = Typ_CVPOL;
    o3D   = cv3;


  //----------------------------------------------------------------
  } else if(oTyp == Typ_CVELL) {   // CurvElli

    // get space pa3 for 3D-points of bspl iDat
    pa3 = (Point*) memspc501;
    ptMax = sizeof(memspc501) / sizeof(Point);

    // make polygon from bspl
    ptNr = UT3D_ptNr_ell (oDat, UT_DISP_cv);
    if(ptNr > ptMax) {TX_Error("CVOFF2_otb_obj E3"); return -1;}
    irc = UT3D_npt_ell (&ptNr, pa3, oDat);
    if(irc < 0) return -1;
      // GR_Disp_cv (pa3, ptNr, Typ_Att_dash_long);

    // make cv3 = CurvPoly with points = pa3[ptNr]
    UT3D_plg_npt ((CurvPoly*)cv3, pa3, ptNr);

    typ3D = Typ_CVPOL;
    o3D   = cv3;


  //----------------------------------------------------------------
  } else if(oTyp == Typ_CVTRM) {   // CurvCCV

    // get standard-curve from trimmed-curve
    irc = UTO_cv_cvtrm (&typ1, cv3, NULL, oDat);
    if(irc < 0) return -1;
      // DEB_dump_obj__ (typ1, cv3, "_otb_obj- %d",irc);

   // recurse
   return CVOFF2_otb_obj (otbo, typ1, cv3, rSys);





  //----------------------------------------------------------------
  } else {TX_Error("CVOFF2_otb_obj TODO typ %d",oTyp); return -1;}


    // TESTBLOCK
    // have now binary object of typ typ3D in o3D = 3D-input-contour
    // DEB_dump_obj__ (typ3D, o3D, "_otb_obj-o2D");
    // ERR_raise (__func__);
    // END TESTBLOCK



  //================================================================
  // planar: bin-3D-obj -> bin-2D-obj   (projected onto backplane)
  // project <o3DNr> 3D-objs o3D onto backplane bpi -
  // - giving <o3DNr> 2-objs o2D
  //  cv2Dat = project inputCurve (Typ_PT,iDat,ptNr) onto backplane bpi
  //    store 2D-inputCurves in o2iSpc

  // get memspc for 2D-objects (points for polygon, bspl, ellipse ..)
  UME_init (&o2Spc, memspc101, sizeof(memspc101));
  // UME_init (&o2Spc, memspc55, sizeof(memspc55));

  // transf. 3D -> 2D
  irc = UTO2_obj_tra_obj3_rsys (&typ2D, o2D, typ3D, o3D, rSys, &o2Spc);
  if(irc < 0) return -1;

    // TESTBLOCK
    // have now binary object of typ typ2D in o2D = 2D-input-contour
    // DEB_dump_obj__ (typ2D, o2D, "_otb_obj-o2D");
    // ERR_raise (__func__);
    // END TESTBLOCK


  //================================================================
  // add 2D-polygon or 2D-circ to 2D-ObjTab
  irc = OTB__obj_add (otbo, typ2D, o2D);
  if(irc < 0) {TX_Error("CVOFF2_otb__ E2-%d",irc); return -1;}

  // free o2Spc / memspc101


    // TESTBLOCK
    // have now otbo = 2D-input-contour-otb
    // OTB_dump (otbo, 1, Typ_Att_Symb, "ex-CVOFF2_otb_obj");
    // OTB_dump (otbo, I_BIN(01010), Typ_Att_Symb, "-Tab1-3");
    // ERR_raise (__func__);
    // END TESTBLOCK


  return 0;

}


//================================================================
  int CVOFF2_save__ (ObjTab *oTab1) {
//================================================================
// write all contour-loops into file
// Input:
//   oTab1     input-contour 2D
//   retCod    0=OK; -1??
// USED MEMSPACES: memspc101,memspc501


  int     irc, i1, ia[3]= {0,1,2};
  ObjTab  oTab2;
  Point2  pts, pte;


  // printf("CVOFF2_save__ \n");


  // get memspc for oTab2 for 2D-segments + 2D-boxes
  OTB_init (&oTab2, Typ_BBox2, Typ_GEOB_2D,
               memspc101, sizeof(memspc101),
               memspc501, sizeof(memspc501));


  //----------------------------------------------------------------
  // work single obj (line|circ; write offset-contour into file ..)
  if(oTab1->oNr > 1) goto L_cplx;
  i1 = oTab1->oTyp[0];
  if(i1 == Typ_LN2) {
    irc = CVOFF2_ln__ (&oTab2, oTab1);
    if(irc < 0) {TX_Error("CVOFF2_save__ E1-%d",irc); return -1;}
    // no intersections possible;
    i1 = oTab2.oNr;
    irc = CVOFF2_int_save_l1 (&oTab2, 0, NULL, i1, ia, 0, NULL);
    if(irc < 0) {TX_Error("CVOFF2_save__ E2-%d",irc); return -1;}
    goto L_exit;

  } else if(i1 == Typ_CI2) {
    irc = CVOFF2_ci__ (&oTab2, oTab1);
    if(irc < 0) {TX_Error("CVOFF2_save__ E3-%d",irc); return -1;}
    goto L_clean;
  }


    // TESTBLOCK
    /// have now oTab1 = 2D-input-contour as
    // OTB_dump (&oTab1, I_BIN(01010), Typ_Att_Symb, "-Tab1-3");
    // ERR_raise (__func__);
    // END TESTBLOCK



  //----------------------------------------------------------------
  // make oTab2 = parallel-objects of oTab1
  L_cplx:
  irc = CVOFF2_cr__ (&oTab2, oTab1);
  if(irc < 0) {TX_Error("CVOFF2_save__ E4-%d",irc); return -1;}

    // TESTBLOCK
    // have now oTab2 = 2D-parallel-contour
    // OTB_dump (&oTab2, 1, Typ_Att_dash__, "---CVOFF2_int_ci1-1");// dump
    // OTB_dump (&oTab2, 3, Typ_Att_dash__, "---CVOFF2_int_ci1-3");// dump+disp
    // fflush(stdout);
    // ERR_raise (__func__);
    // END TESTBLOCK


  //----------------------------------------------------------------
  // remove circs with antiparallel connection on both ends
  L_clean:
  irc = CVOFF2_int_ci1 (&oTab2, oTab1);
  if(irc < 0) return MSG_ERR__ (irc, "E7");
  // if(irc < 0) {TX_Error("CVOFF2_otb__ E7 %d",irc); return -1;}


  // test if oTab2 closed
  if(!CO2DAT.iClo) {
      // printf(" CO2DAT.cNr=%d\n",CO2DAT.cNr);
    // inputContour is closed; no output if oTab2 not closed
    i1 = oTab2.oNr - 1;  // last index
    // get startPt index-0
    UT2D_ptvcpar1_std_obj (&pts,NULL,NULL, Ptyp_start,oTab2.oTyp[0],oTab2.oDat[0]);
    // get endPt index-i1
    UT2D_ptvcpar1_std_obj (&pte,NULL,NULL, Ptyp_end,oTab2.oTyp[i1],oTab2.oDat[i1]);
    i1 = UT2D_comp2pt (&pts, &pte, UT_DISP_cv);
    if(!i1) return 0;
  }


    // TESTBLOCK
    // DEB_dump_obj__ (Typ_ObjTab, oTab1, "---CVOFF2_int_ci1");
    // OTB_dump (&oTab2, 15, Typ_Att_dash__, "---CVOFF2_int_ci1");
    // OTB_dump (&oTab2, 1, 0, "---CVOFF2_int_ci1");
    // return ERR_TEST;
    // ERR_raise (__func__);
    // END TESTBLOCK


  //----------------------------------------------------------------
  // remove intersections, write all loops into file
  irc = CVOFF2_int__ (&oTab2, oTab1);
  if(irc < 0) {TX_Error("CVOFF2_save__ E9-%d",irc); return -1;}



  L_exit:
    // TESTBLOCK
    // DEB_dump_obj__ (Typ_ObjTab, &oTab2, "---CVOFF2_int__");
    // OTB_dump (&oTab2, 10, Typ_Att_dash__, "---CVOFF2_int__");
    // ERR_raise (__func__);
    // END TESTBLOCK

  return 0;

}


//================================================================
  int CVOFF2_int_ci1 (ObjTab *oTabO, ObjTab *oTabI) {
//================================================================
// remove circs with antiparallel connection on both ends
// retCode    o=ok, -1=error

  int      irc, ii0, ii1, ii2, iim, oNr, sr, typ0, typ2, iConn, nInt, nLoop, isRev;
  void     *obj0, *obj2;
  Circ2    *ci1;
  Vector2  vc1s, vc1e, vc0e, vc2s;
  Point2   pt1;


  // printf("CCCCCCCCCCCCCCCCCCCCCCCCCCCCC CVOFF2_int_ci1 oNr=%d\n",oTabO->oNr);
  // OTB_dump (oTabO, 1, 0, "CVOFF2_int_ci1");
  // return -1; // TEST ONLY


  nLoop = 0;

  L_start:
    oNr = oTabO->oNr;
    if(oNr < 4) goto L_exit;
    nInt = 0;
    if(CO2DAT.iClo) ii1 = 1;
    else ii1 = 0;

  L_nxt1__:
    // find ii1 = next circ
    if(oTabO->oTyp[ii1] != Typ_CI2) goto L_nxt9__;

    // ii0=seg-before-circ; ii1=circ; ii2=seg-following-circ
    // circ to test is ii1;
    // test segments around circ ii1 if antiparallel;
    // get ii0=previous and ii2=next segment
    ii0 = ii1 - 1;   if(ii0 < 0) ii0 = oNr - 1;
    ii2 = ii1 + 1;
    if(ii2 >= oNr) ii2 = 0;
      // printf(" -------_ci1-L_nxt1__ ii0=%d ii1=%d ii2=%d oNr=%d nLoop=%d nInt=%d\n",
             // ii0, ii1, ii2, oNr, nLoop, nInt);


    // test if boxes intersect
    irc = UT2D_ckBoxinBox1 (&((BBox2*)oTabO->xDat)[ii1].pb1,
                            &((BBox2*)oTabO->xDat)[ii1].pb2,
                            &((BBox2*)oTabO->xDat)[ii2].pb1,
                            &((BBox2*)oTabO->xDat)[ii2].pb2);
    if(irc < 0) goto L_nxt9__;
      // printf(" parl_int__-boxes-inters: %d %d\n",ii1,ii2);


    // get startvector of ii1
    UT2D_ptvcpar1_std_obj (NULL, &vc1s, NULL, Ptyp_start,
                           oTabO->oTyp[ii1], oTabO->oDat[ii1]);
      // DEB_dump_obj__ (Typ_VC2, &vc1s, " vc1s");

    // get endvector of ii0
    UT2D_ptvcpar1_std_obj (NULL, &vc0e, NULL, Ptyp_end,
                           oTabO->oTyp[ii0], oTabO->oDat[ii0]);
      // DEB_dump_obj__ (Typ_VC2, &vc0e, " vc0e");

    // compare antiparallel; continue if not
    if(UT2D_ck_parla_2vc (&vc0e, &vc1s, CO2DAT.tol1) != 2) goto L_nxt9__;


    // get endvector of ii1
    UT2D_ptvcpar1_std_obj (NULL, &vc1e, NULL, Ptyp_end,
                           oTabO->oTyp[ii1], oTabO->oDat[ii1]);
      // DEB_dump_obj__ (Typ_VC2, &vc1e, " vc1e");

    // get startvector of ii2
    UT2D_ptvcpar1_std_obj (NULL, &vc2s, NULL, Ptyp_start,
                           oTabO->oTyp[ii2], oTabO->oDat[ii2]);

    // compare antiparallel; continue if not
    if(UT2D_ck_parla_2vc (&vc1e, &vc2s, CO2DAT.tol1) != 2) goto L_nxt9__;

    // compute midvalue iim
    iim = ii1 + (oNr / 2);
    if(iim >= oNr) iim -= oNr;        // printf(" iim=%d\n",iim);


    L_nxt2__:
        // printf(" _ci1-L_nxt2__ ii0=%d ii1=%d ii2=%d oNr=%d\n",ii0,ii1,ii2,oNr);
      // test intersection ii0 X ii2; get pt1 =  intersection-point
      typ0 = oTabO->oTyp[ii0];
      obj0 = oTabO->oDat[ii0];
      typ2 = oTabO->oTyp[ii2];
      obj2 = oTabO->oDat[ii2];
      // test connected; 0=no, 1=yes
      iConn = UTI_is_seq_2i_ring (ii0, ii2, oNr);
      // intersect
      irc = CVOFF2_int_2o (&pt1, &isRev, iConn, typ0, obj0, typ2, obj2);
      if(irc <= ERR_severity_2)  return MSG_ERR__ (irc, "");
      if(irc < 0) goto L_nxt7__;

        // TESTBLOCK
        // if((irc==5)&&(iConn==1)) {
        // DEB_dump_obj__(Typ_PT2, &pt1, "_ci1-FFFFFF-intersPt %d %d",ii0,ii2);
        // if((ii0==1)&&(ii2==0)) {
        // if((nLoop == 0)&&(nInt == 2)) {
        // OTB_dump (oTabO, 11, 0, "ex-CVOFF2_int_ci1-E4");
        // ERR_raise (__func__);
        // }
        // END TESTBLOCK


      // get 4 trimmed segments from 2 segments + inters.point into CO2TIS
      irc = CVOFF2_int_trim (typ0, obj0, typ2, obj2, &pt1, irc, isRev);
      if(irc < 0) {TX_Error("CVOFF2_int_ci1 E3 %d",irc); return -1;}


      // update segments ii0 and ii2 of bwd-loop
      irc = CVOFF2_int_upd_bwd (oTabO, &ii0, &ii2);
      if(irc <= ERR_severity_2)  return MSG_ERR__ (irc, "");


      // set endPt of ii0, set startPt of ii2,
      // remove the fwd-loop = objects between the intersectionpoints on ii0-ii2
      irc = CVOFF2_int_del1 (oTabO, &ii0, &ii2);
      // irc = CVOFF2_int_del_fwd (oTabO, &ii0, &ii2, &pt1);
      if(irc <= ERR_severity_2)  return MSG_ERR__ (irc, "");
  

        // TESTBLOCK
        // if((ii0==1)&&(ii2==0)) {
        // if(nInt == 2) {
        // OTB_dump (oTabO, 11, Typ_Att_dash__, "ex-CVOFF2_int_ci1-E5");
        // ERR_raise (__func__);
        // }
        // END TESTBLOCK

      // update oNr after deletion
      oNr = oTabO->oNr;
      ++nInt;
      goto L_nxt9__;


  L_nxt7__:
    // increment ii2; last value = iim., decrement ii0
    if(ii2 != iim) {
      ++ii2;
      if(ii2 >= oNr) ii2 = 0;
      goto L_nxt2__;
    }

    // decrement ii0; last value = iim not allowed
    ii2 = ii1 + 1;
    --ii0;
    // if closed: connect ..
    if(CO2DAT.iClo) {
      // 1=not_closed
      if(ii2 >= oNr) goto L_nxt9__;
      if(ii0 < 0) goto L_nxt9__;
    } else {
      // 0=closed
      if(ii2 >= oNr) ii2 = 0;
      if(ii0 < 0) ii0 = oNr - 1;
    }
    if(ii0 != iim) {
      goto L_nxt2__;
    }


  L_nxt9__:
    ++ii1;
    if(ii1 < oNr) goto L_nxt1__;
  
     
    // TESTBLOCK
    // OTB_dump (oTabO, 3, 0, "ex-CVOFF2_int_ci1-E9");
    // ERR_raise (__func__);    // stop dll
    // END TESTBLOCK

  L_exit:
    if(nInt) {
      ++nLoop;
      nInt = 0;
      goto L_start;
    }

    return 0;

}


//================================================================
  int CVOFF2_int__ (ObjTab *oTabO, ObjTab *oTabI) {
//================================================================
// remove intersections from parallel-contour.
// Input:
//   oTabO      parallel-contour with intersections
//   oTabI      input-contour
// Output:
//   oTabO      parallel-contour without intersections
    
    
  int    irc, ii1, ii2, oNr, dNr, typ1, typ2, iix, sr, iConn, isRev;
  int    nInt, nLoop;   // for testing only
  void   *obj1, *obj2;
  Point2 *ptx, pt1;
  Line2  *ln1;
  Circ2  *ci1;
  ObjTab oTabB=UME_NEW;
    

  // printf("IIIIIIIIIIIIIIIIIIIIIII CVOFF2_int__ oNr=%d\n",oTabO->oNr);
  // OTB_dump (oTabO, 1, 0, "CVOFF2_int__-oTabO");
  // ERR_raise (__func__);
  // return -1; // TEST ONLY
  

  // find intersections - test boxes.
  nInt = 0;      // nr of intersection this loop
  nLoop = 0;     // nr of loops done


  //----------------------------------------------------------------
  // start with new contour in oTabO
  L_nxt0__:

      // TESTBLOCK
      // printf(" NN0 oNr=%d nLoop=%d nInt=%d\n",oTabO->oNr,nLoop,nInt);
      // if((nLoop == 0)&&(nInt == 1)) {
        // OTB_dump (oTabO, 10, Typ_Att_Symb, "CVOFF2_int__-L_nxt0__:");
        // ERR_raise (__func__); }
      // END TESTBLOCK


    // 2 segments: no intersections possible ..
    oNr = oTabO->oNr;
    if(oNr < 3) {
      if(oTabO->oNr < 2) {TX_Error("CVOFF2_int__ E1");return -1;}
      goto L_bwdloop;
    }
    ii1 = 0;
    ii2 = oNr - 1;

  //----------------------------------------------------------------
  L_nxt1__:
    typ1 = oTabO->oTyp[ii1];
    obj1 = oTabO->oDat[ii1];


    //----------------------------------------------------------------
    L_nxt2__:
        // printf(" NN1 L_nxt2__: ii1=%d ii2=%d oNr=%d nInt=%d\n",ii1,ii2,oNr,nInt);

      // test if segments ii1-ii2 connected; 0=No; 1=Yes
      iConn = UTI_is_seq_2i_ring (ii1, ii2, oNr);

      // test if boxes intersect (only not-connected-objs)
      if(!iConn) {
        irc = UT2D_ckBoxinBox1 (&((BBox2*)oTabO->xDat)[ii1].pb1,
                                &((BBox2*)oTabO->xDat)[ii1].pb2,
                                &((BBox2*)oTabO->xDat)[ii2].pb1,
                                &((BBox2*)oTabO->xDat)[ii2].pb2);
        if(irc < 0) goto L_nxt_cont;
      }
          // printf(" parl_int__-boxes-inters: %d %d\n",ii1,ii2);

      // boxes overlap; test intersection; get pt1 =  intersection-point
      typ2 = oTabO->oTyp[ii2];
      obj2 = oTabO->oDat[ii2];

      // intersect
      irc = CVOFF2_int_2o (&pt1, &isRev, iConn, typ1, obj1, typ2, obj2);
      if(irc < 0) goto L_nxt_cont;


        // TESTBLOCK
        if((irc==5)&&(iConn==1)) {
        // printf("     ii1=%d ii2=%d oNr=%d nInt=%d\n",ii1,ii2,oNr,nInt);
        // DEB_dump_obj__(Typ_PT2, &pt1, "FFFFFFF__ intersPt");
        // if(nInt>64) {
          // OTB_dump (oTabO, 10, Typ_Att_Symb, "CVOFF2_int__-L_nxt2__:");
          // ERR_raise (__func__);
        // }
        // if(irc == ERR_TODO_E) {
        // if((nLoop == 0)&&(nInt == 0)) {
          OTB_dump (oTabO, 11, 0, "CVOFF2_int__T1");
          // ERR_raise (__func__);
          return ERR_TODO_E;
        }
        // END TESTBLOCK



      //----------------------------------------------------------------
      // have intersection = 2 loops. fwd-loop (ii1-ii2) and bwd-loop (ii2-ii1).
      // - both loops can have further intersections;
      // - save fwd-loop into file (CVOFF2_int_bl_wr),
      // - delete fwd-loop (CVOFF2_int_del_fwd) and continue working on bwd-loop
      // Work fwd-loops after fwd-loop is finished.

      // get 4 trimmed segments from 2 segments + inters.point into CO2TIS.o*
      // if the type of this objects (CO2TIS.typ*) is 0 then obj is deleted
      irc = CVOFF2_int_trim (typ1, obj1, typ2, obj2, &pt1, irc, isRev);
      if(irc < 0) {TX_Error("CVOFF2_int__ E2 %d",irc); return -1;}


      // store fwd-loop into file
      irc = CVOFF2_int_bl_wr (oTabO, ii1, ii2, &pt1);
      if(irc < 0) return -1;
  

      // update segments ii1 and ii2 of bwd-loop
      irc = CVOFF2_int_upd_bwd (oTabO, &ii1, &ii2);
      if(irc <= ERR_severity_2)  return MSG_ERR__ (irc, "");


      // keep bwd-loop; delete fwd-loop
      // delete fwd-loop and its boxpoints; trim segments ii1,ii2
      irc = CVOFF2_int_del1 (oTabO, &ii1, &ii2);
      // irc = CVOFF2_int_del_fwd (oTabO, &ii1, &ii2, &pt1);
      if(irc < 0) {TX_Error("CVOFF2_int__ E3 %d",irc); return -1;}


        // TESTBLOCK
        // OTB_dump (oTabO, 1, 0, "-oTabO-T3");
        // if((ii1==0)&&(ii2==3)){OTB_dump(oTabO,2,0,"-oTabO-T3");return -1;}
        // if(nInt == 1) return 0;
        // END TESTBLOCK


      // loop was detected. Continue with same indexes ii1,ii2 -
      // but reduced with deleted segmentNrs.
      // update oNr after deletion
      if(irc < 0) return -1;
      oNr = oTabO->oNr;
      ++nInt;
      goto L_nxt0__;





    //----------------------------------------------------------------
    // no loop found. test next
    L_nxt_cont:
        // printf(" L_nxt_cont: ii1=%d ii2=%d oNr=%d\n",ii1,ii2,oNr);
      --ii2;
      if(ii2 > ii1) goto L_nxt2__;

    L_nxt1_cont:
      ++ii1;
      if(ii1 >= oNr) goto L_bwdloop;
      ii2 = oNr - 1;
      if(ii1 == ii2) goto L_bwdloop;
      goto L_nxt1__;


  //----------------------------------------------------------------
  L_bwdloop:
    // this fwd-loop does not have a intersection -
    // test sense-of-rotation of the active otb (last closed-loop)
    // if sr.loop = sr.inputContour then output loop; else ignore.
    iix = CVOFF2_int_save_sr (oTabO, 0, oTabO->oNr - 1, NULL);
    if(iix  < 0) return iix;


    // if need all loops or bwd-loop then load bwd-contour and restart
    irc = CVOFF2_int_bl_rd (oTabO);
    // irc 0=eof; 1=OK; -1=err.
    if(irc < 0) return -1;
    ++nLoop;


      // TESTBLOCK 
      // goto L_exit;
      // END TESTBLOCK

    // if(!flagBwd) { flagBwd = 1; goto L_nxt0__; }
    if(irc > 0) goto L_nxt0__;


  //----------------------------------------------------------------
  L_exit:
    // close files
    if(CO2DAT.fp_set_r) {fclose (CO2DAT.fp_set_r); CO2DAT.fp_set_r = NULL;}
    if(CO2DAT.fp_set_w) {fclose (CO2DAT.fp_set_w); CO2DAT.fp_set_w = NULL;}



      // TESTBLOCK
      // printf("ex-CVOFF2_int__ cNr=%d\n",CO2DAT.cNr);
      // OTB_dump (oTabO, 3, Typ_Att_dash__, "ex-CVOFF2_int__");
      // END TESTBLOCK


  return CO2DAT.cNr;

}


//================================================================
  int CVOFF2_int_upd_bwd (ObjTab *oTabO, int *ii1, int *ii2) {
//================================================================
// CVOFF2_int_upd_bwd             update segments ii1 and ii2 of bwd-loop
// - update seg ii1 and ii2 of bwd-loop
// - update boxes of segments ii1 and ii2 of bwd-loop


  int    irc;


  // printf("CVOFF2_int_upd_bwd ii1=%d ii2=%d\n",*ii1,*ii2);


  // copy ii1-seg and ii2-seg
  // stat: 0 = unmodif; 1=set; 2=NULL.
  if(CO2TIS.stat1b == 1) {
    memcpy (oTabO->oDat[*ii1], CO2TIS.o1b, OBJ_SIZ_MAX);


    // update boxes for bwd-loop 
    // update box for seg ii1   (see also OTB_add_xDat)
    UT2D_box_obj (&((BBox2*)oTabO->xDat)[*ii1].pb1,
                  &((BBox2*)oTabO->xDat)[*ii1].pb2,
                  CO2TIS.typ1b, CO2TIS.o1b, &CO2DAT.tol1);
      // DEB_dump_obj__ (Typ_BBox2, &((BBox2*)oTabO->xDat)[*ii1], "  _BBox2-bwd-ii1");
  }

    
  if(CO2TIS.stat2b == 1) {
    memcpy (oTabO->oDat[*ii2], CO2TIS.o2b, OBJ_SIZ_MAX);

    // update box for seg ii2
    UT2D_box_obj (&((BBox2*)oTabO->xDat)[*ii2].pb1,
                  &((BBox2*)oTabO->xDat)[*ii2].pb2,
                  CO2TIS.typ2b, &CO2TIS.o2b, &CO2DAT.tol1);
      // DEB_dump_obj__ (Typ_BBox2, &((BBox2*)oTabO->xDat)[*ii2], "  _BBox2-bwd-ii2");
  }




  //----------------------------------------------------------------
  // remove unused segment ii1 | ii2

  if(CO2TIS.stat1b != 2) goto L_exit;

    // delete seg ii1 (later, in CVOFF2_int_del1)
    // get segInd preceding ii1
    *ii1 = OTB_iPrv (oTabO, *ii1);


  L_2b:
  if(CO2TIS.stat2b != 2) goto L_exit;

    // delete seg ii2 (later, in CVOFF2_int_del1)
    // get segInd following ii2
    *ii1 = OTB_iNxt (oTabO, *ii1);


printf(" ?????????????\n");
// ERR_raise (__func__);




  //----------------------------------------------------------------
  L_exit:

    // printf("ex-_int_upd_bwd ii1=%d ii2=%d\n",*ii1,*ii2);

  return 0;

}

 
//================================================================
  int CVOFF2_int_bl_rd (ObjTab *otb1) {
//================================================================
// read next loop from file into ObjTab
// Output:
//   retCod    1  next loop loaded
//             0  no more loop found; EOF
//            -1   ERR
// TODO: OTB_load_rec

  int        irc, ii1, rNr, rTyp;
  char       obj1[OBJ_SIZ_MAX], fnam[256];
  BBox2      box2;


  // printf("CVOFF2_int_bl_rd blrNr=%d\n",CO2DAT.blrNr);


  if(!CO2DAT.fp_set_r) {TX_Error("CVOFF2_int_bl_rd E0"); return -1;}
      

  // reset otb1
  OTB_reset__ (otb1, 0);


  //----------------------------------------------------------------
  fread(&rNr, sizeof(int), 1, CO2DAT.fp_set_r);

  if(feof(CO2DAT.fp_set_r)) { irc = 0; goto L_exit; }

    // printf(" rNr=%d\n",rNr);

  if(rNr > otb1->oSiz) {TX_Error("CVOFF2_int_bl_rd E2"); return -1;}


  // load
  for(ii1=0; ii1 < rNr; ++ii1) {
    fread(&rTyp, sizeof(int), 1, CO2DAT.fp_set_r);
    fread(obj1, OBJ_SIZ_MAX, 1, CO2DAT.fp_set_r);
    fread(&box2, sizeof(BBox2), 1, CO2DAT.fp_set_r);
      // DEB_dump_obj__ (rTyp, obj1, " _load_Bl %d", ii1);
      // DEB_dump_obj__ (Typ_BBox2, &box2, "  _BBox2-ii1 %d", ii1);

    // copy obj1 -> otb1->oSpc, update otb1->oDat
    OTB_add__ (otb1, rTyp, obj1);
    // add bounding-box
    OTB_add_xDat (otb1, -1, 1, &box2);

      // TESTBLOCK
      // GR_Disp_obj (rTyp, obj1, Typ_Att_Symb, 0);
      // GR_Disp_box2 (&((BBox2*)otb1->xDat)[ii1].pb1,
                    // &((BBox2*)otb1->xDat)[ii1].pb2, Typ_Att_dash__);
      // END TESTBLOCK
  }

    CO2DAT.blrNr += 1; // counter only for testing
    irc = CO2DAT.blrNr;

  L_exit:

    // TESTBLOCK
    // OTB_dump (otb1, 1, Typ_Att_Symb, "ex-CVOFF2_int_bl_rd");
    // if(CO2DAT.blrNr == 1) {
      // OTB_dump (otb1, 10, Typ_Att_Symb, "ex-CVOFF2_int_bl_rd");
      // ERR_raise (__func__);    // stop dll
    // }
    // return MSG_ERR__ (ERR_TEST, "CVOFF2_int_bl_rd");
    // END TESTBLOCK


  return irc;

}


//============================================================================
  int CVOFF2_int_bl_wr (ObjTab *otb1, int ii1, int ii2, Point2 *ptx) {
//============================================================================
// add fwd-loop to file; <tmpDir>/oibo_parl.dat
// TODO: OTB_save_rec + OTB_save_aux (oder aux == NULL or data)

  int        irc, i1, oNr, lfNr, *ia, iix, typ1;
  ObjRange   lfa[2];
  // Memspc     oSpc=UME_NEW;
  BBox2      box2;
  char       fnam[256];
  // char       obj1[OBJ_SIZ_MAX];
  void       *obj1;


  // printf("CVOFF2_int_bl_wr ii1=%d ii2=%d oNr=%d blwNr=%d\n",ii1,ii2,
         // otb1->oNr,CO2DAT.blwNr);

  if(!CO2DAT.fp_set_w) {TX_Error("CVOFF2_int_bl_wr E0"); return -1;}


  //----------------------------------------------------------------
  // get list of all objs for loop
  CVOFF2_int_ll (lfa, ii1, ii2, otb1->oNr);

  // get nr of ints of ObjRanges
  lfNr = UTI_ni_ObjRange (NULL, lfa, 2);

  ia = (int*)MEM_alloc_tmp (sizeof(int) * lfNr);

  // get table of indexes of ObjRanges
  UTI_ni_ObjRange (ia, lfa, 2);


  //----------------------------------------------------------------

  // write nr of records
  i1 = lfNr;
  if(CO2TIS.stat1f != 2) ++i1;
  if(CO2TIS.stat2f != 2) ++i1;
  fwrite(&i1, sizeof(int), 1, CO2DAT.fp_set_w);



  //----------------------------------------------------------------
  // add startSegment of fwd-loop (fix intersectionpoint)
  if(CO2TIS.stat1f != 2) {
    obj1 = (void*)CO2TIS.o1f;

    // get box for startSeg
    UT2D_box_obj (&box2.pb1, &box2.pb2, CO2TIS.typ1f, obj1, &CO2DAT.tol1);

    // add seg to file
    fwrite(&otb1->oTyp[ii1], sizeof(int), 1, CO2DAT.fp_set_w);
    fwrite(obj1, OBJ_SIZ_MAX, 1, CO2DAT.fp_set_w);
    fwrite(&box2, sizeof(BBox2), 1, CO2DAT.fp_set_w);
      // GR_Disp_obj (otb1->oTyp[ii1], obj1, Typ_Att_dash_long, 0);
      // GR_Disp_box2 (&box2.pb1, &box2.pb2, Typ_Att_dash__);
      // DEB_dump_obj__ (Typ_BBox2, &box2, "  _BBox2-ii1 %d", ii1);
  }



  //----------------------------------------------------------------
  // add loopsegs
  for(i1=0; i1<lfNr; ++i1) {
    iix = ia[i1];
      // GR_Disp_obj (otb1->oTyp[iix], otb1->oDat[iix], Typ_Att_dash_long, 0);
    // add seg to file
    fwrite(&otb1->oTyp[iix], sizeof(int), 1, CO2DAT.fp_set_w);
    fwrite(otb1->oDat[iix], OBJ_SIZ_MAX, 1, CO2DAT.fp_set_w);
    fwrite(&((BBox2*)otb1->xDat)[iix], sizeof(BBox2), 1, CO2DAT.fp_set_w);
      // GR_Disp_obj (otb1->oTyp[iix], otb1->oDat[iix], Typ_Att_dash_long, 0);
      // DEB_dump_obj__ (Typ_BBox2, &((BBox2*)otb1->xDat)[iix],"_BBox2-ii1 %d",iix);
      // GR_Disp_box2 (&box2.pb1, &box2.pb2, Typ_Att_dash__);
      // DEB_dump_obj__ (Typ_BBox2, &box2, "  _BBox2-ii1 %d", ii1);
  }


  //----------------------------------------------------------------
  // add endSegment of fwd-loop (fix intersectionpoint) 
  if(CO2TIS.stat2f != 2) {
    obj1 = (void*)CO2TIS.o2f;

    // get box for endSeg
    UT2D_box_obj (&box2.pb1, &box2.pb2, CO2TIS.typ2f, obj1, &CO2DAT.tol1);

    // add seg to file
    fwrite(&otb1->oTyp[ii2], sizeof(int), 1, CO2DAT.fp_set_w);
    fwrite(obj1, OBJ_SIZ_MAX, 1, CO2DAT.fp_set_w);
    fwrite(&box2, sizeof(BBox2), 1, CO2DAT.fp_set_w);
      // GR_Disp_obj (otb1->oTyp[ii2], obj1, Typ_Att_dash_long, 0);
      // GR_Disp_box2 (&box2.pb1, &box2.pb2, Typ_Att_dash__);
      // DEB_dump_obj__ (Typ_BBox2, &box2, "  _BBox2-ii1 %d", ii1);
  }


  //----------------------------------------------------------------
  // CO2DAT.iBwd = 1;   // bwd-loop now exists

  // fwrite("EOL", 3, 1, CO2DAT.fp_set_w);
  fflush (CO2DAT.fp_set_w);

  CO2DAT.blwNr += 1; // counter only for testing

    // TESTBLOCK
    // irc = CVOFF2_int_bl_rd (otb1);  printf(" bl_rd-irc=%d\n",irc);
    // irc = CVOFF2_int_bl_rd (otb1);  printf(" bl_rd-irc=%d\n",irc);
    // if(CO2DAT.blwNr == 1) ERR_raise (__func__);    // stop dll
    // return MSG_ERR__ (ERR_TEST, "CVOFF2_int_bl_wr");
    // END TESTBLOCK
  

  return 0;

}


//============================================================================
  int CVOFF2_int_ll (ObjRange *ora, int i1, int i2, int oNr) {
//============================================================================
// Input:
//   ora     2 structs ObjRange (to be filled)
//   i1-i2   startindex, endindex
//   oNr     nr of objs
// Output:
//   ora     2 structs ObjRange; typ,ind = startObj, oNr nr of following objs

  int   is, dNr, ii;


  // printf("CVOFF2_int_ll %d %d %d\n",i1,i2,oNr);


  ora[0].typ = 0;
  ora[0].oNr = 0;
  ora[1].typ = 0;
  ora[1].oNr = 0;


  // from i1 to i2;
  if(i1 < i2) {
    is = i1 + 1;    // is = 1. obj to delete
    dNr = i2 - is;  // dNr = nr of objs to delete
    ora[0].typ = Typ_CV;
    ora[0].ind = is;
    ora[0].oNr = dNr;
      // printf(" _ll-1: is=%d dNr=%d\n",is,dNr);
    goto L_exit;
  }


  // i1 > i2; eg 6,1
  // from i1 to oNr
  if(i1 < oNr-1) {
    is = i1 + 1;    // is = 1. obj to delete
    dNr = oNr - is;  // dNr = nr of objs to delete
    ora[0].typ = Typ_CV;
    ora[0].ind = is;
    ora[0].oNr = dNr;
      // printf(" _ll-2: is=%d dNr=%d\n",is,dNr);
  }


  // from 0 to i1
  if(i2 > 0) {
    is = 0;          // is = 1. obj to delete
    dNr = i2;        // dNr = nr of objs to delete
    if(ora[0].typ) ii = 1; else ii = 0;
    ora[ii].typ = Typ_CV;
    ora[ii].ind = is;
    ora[ii].oNr = dNr;
      // printf(" _ll-3: is=%d dNr=%d\n",is,dNr);
  }

  //----------------------------------------------------------------
  L_exit:

   // TESTBLOCK
   // if(ora[0].typ) printf(" ex _int_ll-0 %ld %d\n",ora[0].ind,ora[0].oNr);
   // if(ora[1].typ) printf(" ex _int_ll-1 %ld %d\n",ora[1].ind,ora[1].oNr);
   // END TESTBLOCK


  return 0;

}


//================================================================
  int CVOFF2_int_load_l (ObjTab *otb1) {
//================================================================
// CVOFF2_int_load_l        load output (closed-loops)
// TODO: transpone -> 3D; then -> CCV; then -> DB ..

  int        irc, ii1, ii2, rNr, lNr, typ1;
  char       obj1[OBJ_SIZ_MAX], fnam[256];


  // printf("CVOFF2_int_load_l cNr=%d crNr=%d\n",CO2DAT.cNr,CO2DAT.crNr);


  // test if loopFile has any loops
  if(CO2DAT.cNr < 1) return 0;

  // loopFile must already be open
  if(!CO2DAT.fp_get) {TX_Error("CVOFF2_int_load_l E0"); return -1;}


  lNr = 0;
  fflush (CO2DAT.fp_get);
  rewind (CO2DAT.fp_get);


    // TESTBLOCK
    // CO2DAT.crNr = 2;
    // END TESTBLOCK


  //----------------------------------------------------------------
  // read until start of loop # CO2DAT.crNr
  if(CO2DAT.crNr == 0) goto L_nxt_0;



  // skip (CO2DAT.crNr - 1) contours
  ii1 = CO2DAT.crNr - 1;


  L_skip_nxt:
    if(lNr >= ii1) goto L_nxt_0;
      // printf(" ------ int_load_l-skip loop %d\n",lNr);

    // read rNr = nr of segments of this loop
    fread(&rNr, sizeof(int), 1, CO2DAT.fp_get);
      // printf(" rNr=%d\n",rNr);

    for(ii2=0; ii2 < rNr; ++ii2) {
       fread(&typ1, sizeof(int), 1, CO2DAT.fp_get);
       fread(obj1, OBJ_SIZ_MAX, 1, CO2DAT.fp_get);
    }

    ++lNr;
    goto L_skip_nxt;



  //----------------------------------------------------------------
  // start read next loop
  L_nxt_0:
      // printf(" ------ int_load_l-load loop %d\n",lNr);

    // read rNr = nr of segments of this loop
    fread(&rNr, sizeof(int), 1, CO2DAT.fp_get);
    if(feof(CO2DAT.fp_get)) goto L_exit;
      // printf(" rNr=%d\n",rNr);


    for(ii2=0; ii2 < rNr; ++ii2) {
      // read objType
      fread(&typ1, sizeof(int), 1, CO2DAT.fp_get);
      // read objData
      fread(obj1, OBJ_SIZ_MAX, 1, CO2DAT.fp_get);

        // TESTBLOCK
        // GR_Disp_obj (typ1, obj1, Typ_Att_Symb, 0);
        // END TESTBLOCK

      // add segment into otb1
      irc = OTB_add__ (otb1, typ1, obj1);
      if(irc < 0) return -1;
    }


    if(CO2DAT.crNr == 0) {
      ++lNr;
      if(CO2DAT.cNr >= lNr) goto L_nxt_0;
    }


  L_exit:
  return 0;

}


//============================================================================
  int CVOFF2_int_save_sr (ObjTab *otb1, int ii1, int ii2, Point2 *ptx) {
//============================================================================
// CVOFF2_int_save_sr          save loop if sense-of-rotation OK

  int        irc, i1, iix, lfNr, *ia, typ1, typ2, sr;
  ObjRange   lfa[2];
  char       obj1[OBJ_SIZ_MAX], obj2[OBJ_SIZ_MAX];


  // printf("CVOFF2_int_save_sr ii1=%d ii2=%d cNr=%d\n",ii1,ii2,CO2DAT.cNr);


  //----------------------------------------------------------------
  // get list of all objs for fwd-loop
  CVOFF2_int_ll (lfa, ii1, ii2, otb1->oNr);

  // get nr of ints of ObjRanges
  lfNr = UTI_ni_ObjRange (NULL, lfa, 2);

  // get tempSpc for list of all objNrs
  ia = (int*)MEM_alloc_tmp (sizeof(int) * lfNr);

  // get ia = table of indexes of ObjRanges -> ia
  UTI_ni_ObjRange (ia, lfa, 2);


  //----------------------------------------------------------------
  // fix first and last seg -> obj1, obj2
  // copy startSeg
  typ1 = otb1->oTyp[ii1];
  memcpy (obj1, otb1->oDat[ii1], OBJ_SIZ_MAX);
  // set startPt
  if(ptx) CVOFF2_pt_set (typ1, obj1, ptx, 1);
    // DEB_dump_obj__ (otb1->oTyp[ii1], obj1, "_save_l %d",ii1);
    // GR_Disp_obj (typ1, obj1, Typ_Att_dash_long, 0);

  // copy endSeg
  typ2 = otb1->oTyp[ii2];
  memcpy (obj2, otb1->oDat[ii2], OBJ_SIZ_MAX);
  // set endPt
  if(ptx) CVOFF2_pt_set (typ2, obj2, ptx, 2);
    // DEB_dump_obj__ (otb1->oTyp[ii2], obj2, "_save_l %d",ii2);
    // GR_Disp_obj (typ2, obj2, Typ_Att_dash_long, 0);



  //----------------------------------------------------------------
  // test sense-of-rotation of the loop
  // first-seg = (typ1,obj1), midsegs = ia[lfNr], last-seg = (typ2,obj2)
  sr = CVOFF2_int_srl1 (otb1, typ1, obj1, lfNr, ia, typ2, obj2);
  if(sr < 0) return -1;
    
    // printf(" _save_sr-sr=%d cNr=%d isOut=%d sr_del=%d\n",sr,
           // CO2DAT.cNr,CO2DAT.isOut,CO2DAT.sr_del);

  if(CO2DAT.cNr == 0) {
    // first contour found; open-contour: always out
    if(CO2DAT.iClo) goto L_write;
    // first contour found; closed-contour; outside: always out
    if(CO2DAT.isOut) goto L_write;
  }


  // if((CO2DAT.cNr == 0)&&(CO2DAT.isOut))   goto L_write;

  if(sr != CO2DAT.sr_del) goto L_write;
  
  goto L_exit;




  //----------------------------------------------------------------
  L_write:
    irc = CVOFF2_int_save_l1 (otb1, typ1, obj1, lfNr, ia, typ2, obj2);
    if(irc < 0) return -1;



  //----------------------------------------------------------------
  L_exit:

  return 0;

}


//=======================================================================
  int CVOFF2_int_srl1 (ObjTab *otb1, int typ1, void *obj1, 
                             int lfNr, int *ia, int typ2, void *obj2) {
//=======================================================================
// CVOFF2_int_srl1          get sense-of-rotation of contour-loop
// loop start with typ1,obj1; then <lfNr> objects in ia, then typ2,obj2.


  int        irc, i1, oNr, iix, ptNr, ptMax, isr;
  double     d1, *tol;
  // char       obj1[OBJ_SIZ_MAX];
  // ObjRange   lfa[2];
  Point2     *pta;



  // printf("CVOFF2_int_srl1 %d %d %d\n",typ1,lfNr,typ2);


  // get space for points
  // ptMax = otb1->oNr + 100;
  ptMax = (lfNr + 10) * 2;
    // printf(" CVOFF2_int_srl1 ptMax=%d\n",ptMax);
  pta = (Point2*)MEM_alloc_tmp (ptMax * sizeof(Point2));
  if(!pta) {TX_Error("CCVOFF2_int_srl EOM");return -1;}

  ptNr  = 0;

  tol = &CO2DAT.tol1;


  //----------------------------------------------------------------
  // add startSegment (fix intersectionpoint)
  // add obj as polygon to pta
  irc = UT2D_npt_obj (&ptNr, pta, ptMax, typ1, obj1, 1, tol, 2);
  if(irc < 0) return -1;



  //----------------------------------------------------------------
  // add loopsegs
  for(i1=0; i1<lfNr; ++i1) {
    iix = ia[i1];
      // printf(" ::::::::::::: nxt i1=%d iix=%d ptNr=%d\n",i1,iix,ptNr);
      // GR_Disp_obj (otb1->oTyp[iix], otb1->oDat[iix], Typ_Att_dash__, 0);

    // connected-objects: remove last endPt (== next startPt)
    if(i1) ptNr -= 1;

    // add obj as polygon to pta
    irc = UT2D_npt_obj (&ptNr, pta, ptMax,
                        otb1->oTyp[iix], otb1->oDat[iix], 1, tol, 2);
    if(irc < 0) return -1;
  }


  //----------------------------------------------------------------
  // add endSegment (fix intersectionpoint)
    // GR_Disp_obj (otb1->oTyp[ii2], obj1, Typ_Att_dash__, 0);

  // connected-objects: remove last endPt (== next startPt)
  if(i1) ptNr -= 1;

  // add obj as polygon to pta
  irc = UT2D_npt_obj (&ptNr, pta, ptMax, typ2, obj2, 1, tol, 2);
  if(irc < 0) return -1;


    // TESTBLOCK
    // GR_Disp_cv2 (pta, ptNr, Typ_Att_hili);
    // END TESTBLOCK

  //----------------------------------------------------------------
  // get sense-of-rotation of polygon; 1=CCW, -1=CW
  isr = UT2D_srar_polc (&d1, ptNr, pta);

  // sense-of-rotation -> 0=CCW, 1=CW.
  i1 = (isr > 0) ? 0 : 1;

    // TESTBLOCK
    // printf("ex-parl_int_srl1 sr=%d area=%f sr=%d\n",i1,d1,isr);
    // ERR_raise (__func__);    // stop dll
    // END TESTBLOCK



  return i1;

}


//=======================================================================
  int CVOFF2_int_save_l1 (ObjTab *otb1, int typ1, void *obj1, 
                                int lfNr, int *ia, int typ2, void *obj2) {
//=======================================================================
// CVOFF2_int_save_l1          add closed-loop to output-file
// Input:
//   typ1,obj1    startSeg
//   typ2,obj1    endSeg

  int        irc, i1, rNr, iix;


  // printf("CVOFF2_int_save_l1 typ1=%d nr=%d typ2=%d\n",typ1,lfNr,typ2);



  //----------------------------------------------------------------
  // write first record = nr of objects of closed-loop
  rNr = lfNr;
  if(typ1) ++rNr;
  if(typ2) ++rNr;
    // printf(" rNr=%d\n",rNr);

  fwrite(&rNr, sizeof(int), 1, CO2DAT.fp_get);


  //----------------------------------------------------------------
  // add startSegment (fix intersectionpoint)
    // DEB_dump_obj__ (typ1, obj1, "_save_l end");
    // GR_Disp_obj (typ1, obj1, Typ_Att_dash_long, 0);

  if(typ1) {
    fwrite(&typ1, sizeof(int), 1, CO2DAT.fp_get);
    fwrite(obj1, OBJ_SIZ_MAX, 1, CO2DAT.fp_get);
  }


  //----------------------------------------------------------------
  // add loopsegs
  for(i1=0; i1<lfNr; ++i1) {
    iix = ia[i1];
      // DEB_dump_obj__ (otb1->oTyp[iix], otb1->oDat[iix], "_save_l %d",iix);
      // GR_Disp_obj (otb1->oTyp[iix], otb1->oDat[iix], Typ_Att_dash_long, 0);

    // add seg to file
    fwrite(&otb1->oTyp[iix], sizeof(int), 1, CO2DAT.fp_get);
    fwrite(otb1->oDat[iix], OBJ_SIZ_MAX, 1, CO2DAT.fp_get);
  }


  //----------------------------------------------------------------
  // add endSegment (fix intersectionpoint)
    // DEB_dump_obj__ (typ2, obj2, "_save_l end");
    // GR_Disp_obj (typ2, obj2, Typ_Att_dash_long, 0);

  if(typ1) {
    fwrite(&typ2, sizeof(int), 1, CO2DAT.fp_get);
    fwrite(obj2, OBJ_SIZ_MAX, 1, CO2DAT.fp_get);
  }


  //----------------------------------------------------------------
  // incr nr of loops
  CO2DAT.cNr += 1;

    // TESTBLOCK
    // printf("ex-CVOFF2_int_save_l1 cNr=%d\n",CO2DAT.cNr);
    // CVOFF2_int_load_l (otb1); // close lun, reload, display
    // if(CO2DAT.cNr  == 3)
    // ERR_raise (__func__);    // stop dll
    // END TESTBLOCK



  return 0;

}


//====================================================================
  int CVOFF2_int_del1 (ObjTab *oTab1, int *io1, int *io2) {
//====================================================================
// remove the objects between the intersectionpoints on io1-io2
//   (but keep seg io1 and seg io2)
//   and update indexes io1 and io2
//    if(ii1 > ii2) then
//      remove iis - end  AND  remove 0 - ii2
//    else
//      remove ii1 - ii2

  int   irc, iis, dNr, ii1, ii2;


  ii1 = *io1;
  ii2 = *io2;


  // printf("CVOFF2_int_del1 ii1=%d ii2=%d oNr=%d\n",ii1,ii2,oTab1->oNr);
    // if((ii1==1)&&(ii2==0)) {
    // OTB_dump (oTab1, 3, 0, "err-_int_del1");
    // ERR_raise (__func__);    // stop dll
    // return MSG_ERR__ (ERR_internal, "");
    // }



  if(ii1 < ii2) {
    // from ii1 to ii2; eg 2-6
    iis = ii1 + 1;    // iis = 1. obj to delete
    dNr = ii2 - iis;  // dNr = nr of objs to delete
    if(dNr) {
      // delete types and obj-pointers and BBoxes
      irc = OTB_del (iis, dNr, oTab1);
      if(irc < 0) return -1;
      *io2 -= dNr;
        // printf(" int_del1-io2-0=%d dNr=%d\n",*io2,dNr);
    }
    goto L_exit;
  }



  // ii1 > ii2; eg 6,1
  if(ii1 < oTab1->oNr - 1) {
    // from ii1 to oNr
    iis = ii1 + 1;    // iis = 1. obj to delete
    dNr = oTab1->oNr - iis;  // dNr = nr of objs to delete
    if(dNr) {
      // delete types and obj-pointers and BBoxes
      irc = OTB_del (iis, dNr, oTab1);
      if(irc < 0) return -1;
      // *io2 -= dNr;
        // printf(" int_del1-io2-1=%d dNr=%d\n",*io2,dNr);
    }
  }
 
  if(ii2 > 0) {
    // from 0 to ii1
    iis = 0;          // iis = 1. obj to delete
    dNr = ii2;        // dNr = nr of objs to delete
    if(dNr) {
      // delete types and obj-pointers and BBoxes
      irc = OTB_del (iis, dNr, oTab1);
      if(irc < 0) return -1;
      *io1 -= dNr;
    }
    // *io2 -= dNr;
    *io2 = 0;
      // printf(" int_del1-io2-2=%d dNr=%d\n",*io2,dNr);
  }




  //----------------------------------------------------------------
  L_exit:
    // // now closed-contour ! ??
    // CO2DAT.iClo = 0;


    // TESTBLOCK
    // printf(" ex-int_del1 io1=%d io2=%d oNr=%d\n",*io1,*io2,oTab1->oNr);
    // MSG_ERR__ (ERR_internal, "");
    // TESTBLOCK


  return 0;

}


//=======================================================================
  int CVOFF2_pt_set (int typ1, void *obj1, Point2 *pt1, int mode) {
//=======================================================================
// Input:
//   mode     1 = modify startPt
//            2 = modify endPt

  // printf("CVOFF2_pt_set %d %d\n",typ1,mode);


  //----------------------------------------------------------------
  if(typ1 == Typ_LN2) {

    if(mode == 1) {
      // modify startPt
      ((Line2*)obj1)->p1 = *pt1;

    } else {
      // modify endPt
      ((Line2*)obj1)->p2 = *pt1;
    }


  //----------------------------------------------------------------
  } else if(typ1 == Typ_CI2) {

    if(mode == 1) {
      // modify startPt
      UT2D_ci_cip1 ((Circ2*)obj1, pt1);

    } else {
      // modify endPt
      UT2D_ci_cip2 ((Circ2*)obj1, pt1);
    }


  //----------------------------------------------------------------
  } else { TX_Error("CVOFF2_pt_set E1 %d",typ1); return -1; }

  return 0;

}


//======================================================================
  int CVOFF2_int_2o (Point2 *ptx, int *isRev, int iConn,
                           int typ1, void *obj1, int typ2, void *obj2) {
//======================================================================
// CVOFF2_int_2o          test intersection of 2 objects;
// Input:
//   iConn    0   objs are not connected
//            1   objs are connected
// Output:
//   ptx          intersectionPoint
//   retCod  -1   no intersection
//           -9   internal error; stop
//            0   normal intersection in segments
//            1   startPt o1 == on o2 
//            2   endtPt o1 == on o2
//            4   startPt o2 == on o1
//            5   startPt o1 == startPt o2
//            6   endtPt o1 == startPt o2
//            8   endtPt o2 == on o1
//            9   startPt o1 == endtPt o2
//           10   endtPt o1 == endtPt o2
//   isRev    1 = obj1/obj2 swapped; 0 = not;


  int       irc, ipa[2]={0,0};
  Point2    pta[2];


  // printf("CVOFF2_int_2o %d X %d iConn=%d\n",typ1,typ2,iConn);

  *isRev = 0;


  if(typ1 == Typ_LN2) {

    //----------------------------------------------------------------
    if(typ2 == Typ_LN2) {
      // inters LN X LN
      irc = UT2D_pt_int_2lnl (ipa, pta, obj1, obj2, &CO2DAT.tol1);
      // skip connection-points of connected objs
      if(iConn) {if((ipa[0]==6)||(ipa[0]==9)) irc = -1;}
      if(irc < 0) goto L_exit;
      // skip 9 = startPt ln1 == endtPt ln2
			if(ipa[0] == 9) goto L_err_conn;
      if(irc) goto L_E_TODO;
      if(ipa[0] < 0) goto L_exit;
      // if(ipa[0]) goto L_E_TODO;
      *ptx = pta[0];
      irc = ipa[0];
      goto L_exit;
    

    //----------------------------------------------------------------
    } else if(typ2 == Typ_CI2) {
      // inters LN X CI
      irc = UT2D_2pt_int_ln_ci (ipa, pta, obj1, obj2, &CO2DAT.tol1);
      // skip connection-points of connected objs
      if(iConn) irc = CVOFF2_int_ckConn (ipa, irc);
      if(irc < 0) goto L_exit;
      if(ipa[0] < 0) goto L_lc_2;
      // rc1 = 1 = touch ..
      // if(ipa[0]) goto L_E_TODO;
      *ptx = pta[0];
      irc = ipa[0];
      goto L_exit;

      L_lc_2:
      if(ipa[1] < 0) goto L_exit;  // eg 9,-1
      // if(ipa[1]) goto L_E_TODO;
      *ptx = pta[1];
      irc = ipa[1];
      goto L_exit;
      


    //----------------------------------------------------------------
    } else goto L_E2;

  } else if(typ1 == Typ_CI2) {

    //----------------------------------------------------------------
    if(typ2 == Typ_LN2) {
      // inters CI X LN
      *isRev = 1;   // obj1/obj2 swapped
      irc = UT2D_2pt_int_ln_ci (ipa, pta, obj2, obj1, &CO2DAT.tol1);
      // skip connection-points of connected objs
      if(iConn) irc = CVOFF2_int_ckConn (ipa, irc);
      if(irc < 0) goto L_exit;
      if(ipa[0] < 0) goto L_cl_2;
      // rc1 = 1 = touch ..
      // if(ipa[0]) goto L_E_TODO;
      *ptx = pta[0];
      irc = ipa[0];
      goto L_exit;

      L_cl_2:
      if(ipa[1] < 0) goto L_exit;
      // if(ipa[1]) goto L_E_TODO;
      *ptx = pta[1];
      irc = ipa[1];
      goto L_exit;

      

    //----------------------------------------------------------------
    } else if(typ2 == Typ_CI2) {           // CI X CI
      // inters CI X CI
      irc = UT2D_2pt_int_ci_ci (ipa, pta, obj1, obj2, &CO2DAT.tol1);
      // skip connection-points of connected objs
      if(iConn) irc = CVOFF2_int_ckConn (ipa, irc);
      if(irc < 0) goto L_exit;
      // rc1 = 1,2 = touch ..
      if(ipa[0] < 0) goto L_cc_2;
      // if(ipa[0]) goto L_E_TODO;
      *ptx = pta[0];
      irc = ipa[0];
      goto L_exit;
      
      L_cc_2:
      if(ipa[1] < 0) goto L_exit;
      // if(ipa[1]) goto L_E_TODO;
      *ptx = pta[1];
      irc = ipa[1];
      goto L_exit;


    //----------------------------------------------------------------
    } else goto L_E2;
  } else goto L_E1;



  //----------------------------------------------------------------
  L_exit:
    // TESTBLOCK
    // printf("ex-int_2o-irc irc=%d ipa0=%d ipa1=%d isRev=%d\n",
            // irc,ipa[0],ipa[1],*isRev);
    // if(irc >= 0) GR_Disp_pt2 (ptx, SYM_TRI_S, ATT_COL_RED);
    // if(irc == 5) AP_debug__ ("CVOFF2_int_2o D1");
    // END TESTBLOCK

    return irc;


  //----------------------------------------------------------------
  L_err_conn:
    // no error, connected obj with no intersection
    return -1;


  L_E1:
    TX_Error("CVOFF2_int_2o E1 %d",typ1);
    return -1;

  L_E2:
    TX_Error("CVOFF2_int_2o E2 %d",typ2);
    return -1;

  L_E_TODO:
    TX_Error("CVOFF2_int_2o TODO %d %d",typ1,typ2);
    return MSG_ERR__ (ERR_TODO_E, "%d %d",typ1,typ2);

}


//================================================================
  int CVOFF2_int_ckConn (int *ipa, int irc) {
//================================================================
// ignore connection-points
// skip 6=endtPt ln == startPt ci; if also touches: ignore.
// skip 9=startPt ln == endtPt ci; if also touches: ignore.


  if((ipa[0]==6)||(ipa[0]==9)) {
    ipa[0] = -1;
    if(ipa[1] < 0) { irc = -1; goto L_exit; }
  }


  if((ipa[1]==6)||(ipa[1]==9)) {
    ipa[1] = -1;
    if(ipa[0] < 0) irc = -1;
  }


  L_exit:
    // printf("ex-CVOFF2_int_ckConn %d %d %d\n",irc,ipa[0],ipa[1]);


  return irc;

}


//============================================================================
  int CVOFF2_cr__ (ObjTab *oTabO, ObjTab *oTabI) {
//============================================================================
// CVOFF2_cr__     create parallel contour oTabO to input oTabI
// make parallel-objects oiboTab2 of oiboTab1
// was UT2D_npt_parl_pln 

  int   irc, ii, iiAct, ioAct, ioPrv, *typI, *typO;
  void  **objI, **objO;


  // printf("CVOFF2_cr__ SizO=%d NrI=%d\n",oTabO->oSiz,oTabI->oNr);


  // reserve complete space in oTabO for binary objects
  // ii = oTabO->oSiz;
  // OTB_reserve (&typO, &objO, oTabO, ii);



  //----------------------------------------------------------------
  if(CO2DAT.iClo) {
    // contour is open
    irc = CVOFF2_open_s (oTabO, oTabI, 1);
    if(irc < 0) return MSG_ERR__ (irc, "CVOFF2_cr__-E1");
  }


  //----------------------------------------------------------------
  typI = oTabI->oTyp;
  objI = oTabI->oDat;
  iiAct = -1;


  //----------------------------------------------------------------
  L_nxt:

  // iiPrv = iiAct;
  ++iiAct;
  if(iiAct >= oTabI->oNr) goto L_close;
  // nxt inp-obj = typI[iiAct], objI[iiAct]
  // nxt out-obj = typO[ioAct], objO[ioAct]
    // printf("XXXXXXXXXXXXXXXXXXXXXXXXXXX iiAct=%d\n",iiAct);



  // make parallel-obj to input-obj (typI[iiAct],objI[iiAct])
  irc = CVOFF2_cr_off (oTabO, typI, objI, iiAct);
  if(irc < 0) {TX_Error("CVOFF2_cr__ E2-%d",irc); return -1;}

    // TESTBLOCK
    // DEB_dump_obj__ (typO, objO, " _crParObj-objO");
    // GR_Disp_obj (typO, objO, Typ_Att_Symb,0);
    // END TESTBLOCK

  goto L_nxt;


  //----------------------------------------------------------------
  L_close:
  if(CO2DAT.iClo) {
    // contour is open: add 90-deg-circ
    irc = CVOFF2_open_e (oTabO, oTabI, 1);

  } else {
    // contour is closed: add closing arc. 0=yes, 1=not_closed; -1=undefined
    irc = CVOFF2_close (oTabO, oTabI);
  }
  if(irc < 0) {TX_Error("CVOFF2_cr__ E3-%d",irc); return -1;}


  //----------------------------------------------------------------
  L_exit:

    // TESTBLOCK
    // OTB_dump (oTabO, 3, 0, "ex-_oibo_parl__");
    // ERR_raise (__func__);
    // END TESTBLOCK


  return 0;

}


//================================================================
  int CVOFF2_open_s (ObjTab *oTabO,  ObjTab *oTabI, int mode) {
//================================================================
// contour is open: add 90-deg-circ
// mode  0 do not add xdat; else add BBox2 as xdat

  int      irc, i1;
  Vector2  vc1;
  Circ2    cic;


  // printf("CVOFF2_open_s %d\n",mode);
  // DEB_dump_obj__ (oTabI->oTyp[0], oTabI->oDat[0], " CVOFF2_open_s");


  // get cic.pc = startpoint of input
  // cic.pc = CO2SEI.pts;
  UT2D_ptvcpar1_std_obj (&cic.pc, &vc1, NULL,
                         Ptyp_start, oTabI->oTyp[0], oTabI->oDat[0]);
    // GR_Disp_pt2 (&cic.pc, SYM_TRI_S, ATT_COL_RED);
    // DEB_dump_obj__ (Typ_VC2, &vc1, "vc1");


  // set vc1 length = fabs(dist) * -1.;
  UT2D_vc_setLength (&vc1, &vc1, -fabs(CO2DAT.dist));

  // get cic.p1 = startpoint of contour
  cic.p1 = cic.pc;
  UT2D_pt_add_vc__ (&cic.p1, &vc1);
    // GR_Disp_pt2 (&cic.p1, SYM_TRI_S, ATT_COL_BLUE);
    // DEB_dump_obj__ (Typ_PT2, &cic.p1, "cic.p1");




  if(CO2DAT.dist > 0.) {
    // CCW
    // get cic.p2 = rot p1 90 deg CW around pc
    UT2D_pt_rot_90_cw_2pt (&cic.p2, &cic.pc, &cic.p1);
    cic.rad  = -CO2DAT.dist;
    cic.ango = -RAD_90;
  } else {
    // CW
    // get cic.p2 = rot p1 90 deg CCW around pc
    UT2D_pt_rot_90_ccw_2pt (&cic.p2, &cic.pc, &cic.p1);
    cic.rad  = -CO2DAT.dist;
    cic.ango = RAD_90;
  }

    // TESTBLOCK
    // GR_Disp_pt2 (&cic.p2, SYM_TRI_S, ATT_COL_YELLOW);
    // DEB_dump_obj__ (Typ_CI2, &cic, "cic");
    // END TESTBLOCK


  if(mode) irc = CVOFF2_add (oTabO, Typ_CI2, &cic);
  else     irc = OTB_add__ (oTabO, Typ_CI2, &cic);
  if(irc < 0) {TX_Error("CVOFF2_open_s E-2"); return -1;}


  return 0;

}


//=======================================================================
  int CVOFF2_open_e (ObjTab *oTabO,  ObjTab *oTabI, int mode) {
//=======================================================================
// contour is open: add 90-deg-circ
// Input:
//   srot     0=CCW, 1=CW
//   mode     0 do not add xdat; else add BBox2 as xda


  int      irc, i1;
  Circ2    cic;


  // endpt of input-contour;
  // DEB_dump_obj__ (Typ_PT2, &CO2SEI.pte, "CVOFF2_open_e-pc");


  // get cic.p1 = endpoint of contour
  i1 = oTabO->oNr - 1; 
  UT2D_ptvcpar1_std_obj (&cic.p1, NULL, NULL,
                         Ptyp_end, oTabO->oTyp[i1], oTabO->oDat[i1]);
    // DEB_dump_obj__ (oTabO->oTyp[i1], oTabO->oDat[i1], "CVOFF2_open_oTabO[%d]",i1);
    // DEB_dump_obj__ (Typ_PT2, &cic.p1, "CVOFF2_open_e-p1");
    // GR_Disp_pt2 (&cic.p1, SYM_TRI_S, ATT_COL_BLUE);


  // get cic.pc = endpoint of input
  cic.pc = CO2SEI.pte;
  // i1 = oTabI->oNr - 1; 
  // UT2D_ptvcpar1_std_obj (&cic.pc, NULL, NULL,
                         // Ptyp_end, oTabI->oTyp[i1], oTabI->oDat[i1]);
    // GR_Disp_pt2 (&cic.pc, SYM_TRI_S, ATT_COL_RED);


  if(CO2DAT.dist > 0.) {
    // CCW
    // get cic.p2 = rot p1 90 deg CW around pc
    UT2D_pt_rot_90_cw_2pt (&cic.p2, &cic.pc, &cic.p1);
    cic.rad  = -CO2DAT.dist;
    cic.ango = -RAD_90;
  } else {
    // CW
    // get cic.p2 = rot p1 90 deg CCW around pc
    UT2D_pt_rot_90_ccw_2pt (&cic.p2, &cic.pc, &cic.p1);
    cic.rad  = -CO2DAT.dist;
    cic.ango = RAD_90;
  }
    // GR_Disp_pt2 (&cic.p2, SYM_TRI_S, ATT_COL_YELLOW);
    // DEB_dump_obj__ (Typ_CI2, &cic, "CVOFF2_open_e-cic");


  if(mode) irc = CVOFF2_add (oTabO, Typ_CI2, &cic);
  else     irc = OTB_add__ (oTabO, Typ_CI2, &cic);
  if(irc < 0) {TX_Error("CVOFF2_open_e E-2"); return -1;}


  return 0;

}
 

//================================================================
  int CVOFF2_close (ObjTab *oTabO,  ObjTab *oTabI) {
//================================================================
// connect startPtOut-endPtOut with circle; center = startPtIn pis.
// SEE CVOFF2_LN_LN

  int    irc, typ2, typ1, typ3, ioe, iie;
  char   oo2[OBJ_SIZ_MAX];
  void   *oo1, *oo3, *oi1, *oi3;


  // get ioe = index last offset-contour-obj
  ioe = oTabO->oNr - 1;

  // get iie = index last input-contour-obj
  iie = oTabI->oNr - 1;

  // get typ1 = typ of last offset-contour-obj
  typ1 = oTabO->oTyp[ioe];

  // get typ3 = typ of first offset-contour-obj
  typ3 = oTabO->oTyp[0];

  // get oo1 = last offset-contour-obj
  oo1 = oTabO->oDat[ioe];
  
  // get oo1 = first offset-contour-obj
  oo3 = oTabO->oDat[0];
  
  // get oi1 = last input-contour-obj
  oi1 = oTabI->oDat[iie];

  // get oi3 = first input-contour-obj
  oi3 = oTabI->oDat[0];


  // get (typo2,oo2) = circle connecting oo1-oo3
  irc = CVOFF2_cr_conn (&typ2, oo2,               // connect-obj out
                        typ1, typ3,                // types of offset-objects
                        oo1, oo3,                  // offset-objects
                        oi1, oi3);                 // input-objects
  if(irc < 0) {TX_Error("CVOFF2_close E-1"); return -1;}


    // TESTBLOCK
    // if(typ2) DEB_dump_obj__ (typ2, oo2,  " o_parl_close cic");
    // if(typ2) GR_Disp_obj (typ2, oo2, Typ_Att_Symb, 0);
    // ERR_raise (__func__);
    // END TESTBLOCK


  if(typ2) {
    irc = CVOFF2_add (oTabO, typ2, oo2);
    if(irc < 0) {TX_Error("CVOFF2_close E-2"); return -1;}
  }

 
  return 0;

}
 

//=============================================================================
  int CVOFF2_cr_off (ObjTab *oTabO,
                       int *typI, void **objI, int iiAct) {
//=============================================================================
// CVOFF2_cr_off       add parallel-contourObjs for inputContourObj iiAct
// Input:
//   iiAct    index into typI, objI
//   typI     array of input-obj-types
//   objI     array of  binary-objs
//   CO2DAT   dist, tol1, tol2
//
// make parallel-obj to input-obj objI[iiAct];
// oiAct = active input-contourObj (index = iiAct);
// oiPrv = previous input-contourObj (index = iiPrv);
// oo1,oo2,oo3 = output-contour;
// oo1 = parallel oiPrv; oo2 = connecting-obj; oo3 = parallel oiAct;
//   connecting-obj is eg circ for LN-LN;


  int   irc, typiAct, typo2, typo3, ioPrv, typiPrv, iiPrv;
  char  oo2[OBJ_SIZ_MAX], oo3[OBJ_SIZ_MAX];
  void  *oiPrv, *oiAct, *oo1;


  // printf("^^^^^^^^^^^^^^^^^^^^^^ CVOFF2_cr_off %d\n",iiAct);
  // DEB_dump_obj__ (typI[iiAct], objI[iiAct], " _parl_crParObj[%d]",iiAct);



  typiAct = typI[iiAct];
  oiAct   = objI[iiAct];
  typo2   = 0;             // connecting-obj,0=none, Typ_CI2=circ
  typo3   = 0;             // offset-obj parallel to oiAct
  

  //----------------------------------------------------------------
  // create oo3 = obj parallel to input-obj oiAct
  if(typiAct == Typ_LN2) {
    // create oo3 = line parallel to objI[iiAct]
    UT2D_ln_parl_ln ((Line2*)oo3, oiAct, &CO2DAT.dist);
    typo3 = Typ_LN2;


  } else if(typiAct == Typ_CI2) {
    // create oo3 = circ parallel to objI[iiAct]
    irc = UT2D_ci_parl_ci ((Circ2*)oo3, oiAct, &CO2DAT.dist);
    if(irc) goto L_exit;
    typo3 = Typ_CI2;

  } else {
   TX_Error("CVOFF2_cr_off E1-%d",typiAct);
   return -1;
  }


  //----------------------------------------------------------------
  // create oo2 = connecting-circle between previous-obj and oo3
  // work LN-LN | LN-CI | CI-LN | CI-CI
  L_connect:
      // printf(" _cr_off-L_connect: typo3=%d\n",typo3);


    // first input-obj: do not create connecting-circle; only store parallel-obj
    if(!iiAct) goto L_add;

    // set prev. inputObj und typ
    iiPrv = iiAct - 1;
    typiPrv = typI[iiPrv];
    oiPrv  = objI[iiPrv];

    // get oo1 = previous-outObj (last obj in oTabO->oDat
    ioPrv = oTabO->oNr - 1;
    oo1 = oTabO->oDat[ioPrv];


    // connect previous and active obj with obj oo2
    irc = CVOFF2_cr_conn  (&typo2, oo2,               // connect-obj
                           typiPrv, typiAct,
                           oo1, oo3,                  // offset-objects
                           oiPrv, oiAct);             // input-objects
    if(irc < 0) {TX_Error("CVOFF2_cr_off E-2"); return -1;}



  //----------------------------------------------------------------
  // add oo2, oo3 to oTabO
  L_add:

// TODO: if typo2 exist:
// test if p1==p2/tolPT; yes: use midpoint for oo1,oo3
// test if p1==p2/tolCV; yes: make oo2 = line ?

    // add Typ_LN2-oo2 to oTabO
    if(typo2) {
      // add segment
        // GR_Disp_obj (typo2, oo2, Typ_Att_Symb, 0);
      irc = CVOFF2_add (oTabO, typo2, oo2);
      if(irc < 0) {TX_Error("CVOFF2_cr_off E-3"); return -1;}
    }


    // add Typ_LN2-oo3 to oTabO
    if(typo3) {
        // GR_Disp_obj (typo3, oo3, Typ_Att_Symb, 0);
      irc = CVOFF2_add (oTabO, typo3, oo3);
      if(irc < 0) {TX_Error("CVOFF2_cr_off E-4"); return -1;}
    }



  //----------------------------------------------------------------
  L_exit:
    // TESTBLOCK "ex-CVOFF2_cr_off"
    // printf(" ex-CVOFF2_cr_off\n");
    // if(iiAct == 2) {
      // OTB_dump (&oTabO, I_BIN(00001), Typ_Att_Symb, "ex-CVOFF2_cr_off");
      // ERR_raise (__func__);
    // }
    // END TESTBLOCK


  return 0;

}


//================================================================
  int CVOFF2_add (ObjTab *otbo, int typ, void *obj) {
//================================================================
// add obj and bounding-box to OTB
 
  int        irc;
  BBox2      box2;


  // DEB_dump_obj__ (typ, obj, " CVOFF2_add");


  // add segment
  irc = OTB_add__ (otbo, typ, obj);
  if(irc < 0) {TX_Error("CVOFF2_add E-2"); return -1;}

  // get bounding-box
  UT2D_box_obj (&box2.pb1, &box2.pb2, typ, obj, &CO2DAT.tol1);

  // add bounding-box
  OTB_add_xDat (otbo, -1, 1, &box2);

  return 0;

}


//=========================================================================
  int CVOFF2_cr_conn (int *typoc, void *ooc, 
                      int typPrv, int typAct,
                      void *ooPrv, void *ooAct,
                      void *oiPrv, void *oiAct) {
//=========================================================================
// create connecting circle for parallel contour LN-LN
// Input:
//   typPrv   type of previous input- and offset-obj;
//   typAct   type of active input- and offset-obj;
//   ooPrv    previous offset-obj; can be modified 
//   ooAct    active offset-obj; can be modified 
//   oiPrv    previous input-obj
//   oiAct    active input-obj
// Output:
//   typoc    type of ooc; Typ_LN2 | Typ_CI2 | none
//   ooc      bin.obj connecting ooPrv - ooAct
//
// was UT2D_npt_parl_pln

  int       is1;
  Point2    ppe, pas, ptx;
  double    ao, dn, rdc;
  Vector2   vcope, vcoas, vc1, vc2;


  // printf("         .............. CVOFF2_cr_conn\n");
  // DEB_dump_obj__ (typPrv, ooPrv, "  ooPrv");
  // DEB_dump_obj__ (typAct, ooAct, "  ooAct");
  // DEB_dump_obj__ (typPrv, oiPrv, "  oiPrv");
  // DEB_dump_obj__ (typAct, oiAct, "  oiAct");
  // GR_Disp_obj (typPrv, ooPrv, Typ_Att_hili,0);
  // GR_Disp_obj (typAct, ooAct, Typ_Att_hili,0);
  // GR_Disp_obj (typPrv, oiPrv, Typ_Att_hili,0);
  // GR_Disp_obj (typAct, oiAct, Typ_Att_hili,0);



  //----------------------------------------------------------------
  // ppe = endpoint prev.offset-obj
  UT2D_ptvcpar1_std_obj (&ppe, &vcope, NULL, Ptyp_end, typPrv, ooPrv);

  // pas = startpoint act.offset-obj
  UT2D_ptvcpar1_std_obj (&pas, &vcoas, NULL, Ptyp_start, typAct, ooAct);

  // get quadr. distance ppe - pas
  UT2D_lenq_2pt (&dn, &ppe, &pas);
  if(dn < CO2DAT.tolq2) goto L_pt_mid;




  //----------------------------------------------------------------
  // only LN-LN:
  if((typPrv == Typ_LN2)&&(typAct == Typ_LN2)) {
    // test dn = deviation; if less than tolPT then connect with line
    // get dn = deviation = dist ln2->p2 normal onto ln1.
    UT2D_vc_2pt (&vc1, &((Line2*)ooPrv)->p1, &((Line2*)ooPrv)->p2);
    UT2D_vc_2pt (&vc2, &((Line2*)ooAct)->p1, &((Line2*)ooAct)->p2);
  
    UT2D_slenq_nor_2vc (&dn, &vc1, &vc2);
    if(fabs(dn) < CO2DAT.tol2) goto L_pt_mid;
  }


  //----------------------------------------------------------------
  // create circ from ppe to pas; center is the endPt of oiPrv

  // ptx = endpoint prv.input-obj
  UT2D_ptvcpar1_std_obj (&ptx, NULL, NULL, Ptyp_end, typPrv, oiPrv);

  // add circ and line; find CW or CCW

  *typoc = Typ_CI2;
  ((Circ2*)ooc)->p1 = ppe;
  ((Circ2*)ooc)->p2 = pas;
  ((Circ2*)ooc)->pc = ptx;


  //----------------------------------------------------------------
  // get sense-of-rotation for connecting-circle
  // get dn = dist left(pos.val) or right (neg.val)
  UT2D_slenq_nor_2vc (&dn, &vcope, &vcoas);

  // get sense-of-rotation is1 for connecting-circle
  if(CO2DAT.dist > 0.) {
      // left
      if(dn > 0.) is1 = 0; // left left   - CCW
      else is1 = 1;        // left right  - CW
  } else {
      // right
      if(dn > 0.) is1 = 0; // right left  - CCW
      else is1 = 1;        // right right - CW
  }


/* fuer closed-contours ?
  if(CO2DAT.sr) {
    // CW
    if(CO2DAT.dist > 0.) {
      // CW inside
      if(dn > 0.) is1 = 1; // CW inside left   - CW
      else is1 = 0;        // CW inside right  - >CCW
    } else {
      // CW outside
      if(dn > 0.) is1 = 1; // CW outside left  - CW
      else is1 = 0;        // CW outside right - CCW
    }
  } else {
    // CCW
    if(CO2DAT.dist > 0.) {
      // CCW inside
      if(dn > 0.) is1 = 0; // CCW inside left  - CCW
      else is1 = 1;        // CCW inside right - CW
    } else {
      // CCW outside
      if(dn > 0.) is1 = 0; // CCW outside left  - CCW
      else is1 = 1;        // CCW outside right - CW
    }
  }
*/
    // printf(" cr_conn is1=%d dn=%f\n",is1,dn);


  //----------------------------------------------------------------
  // create connecting-circle
  rdc = fabs(CO2DAT.dist);
  if(is1) rdc *= -1;

  // create connecting circle from ppe to pas
  ao = UT2D_angr_ci (&ppe, &pas, &ptx, rdc);
  // ao = UT2D_angr_ci (&ppe, &pas, &ptx, rdc);
  // ao = UT2D_angr_2vc (&vcope, &vcoas);
  // UT2D_angr_2vc_sr (&ao, &vcope, &vcoas, is1);
  // UT2D_angr_3pt_sr (&ao, &vcope, &vcoas, is1);
     // printf("  cr_conn is1 = %d ao = %f\n",is1,UT_DEGREES(ao));

  ((Circ2*)ooc)->rad  = rdc;
  ((Circ2*)ooc)->ango = ao;
  goto L_exit;



  //----------------------------------------------------------------
  L_pt_mid:
    // get ptx = midpoint 
    UT2D_pt_mid2pt (&ptx, &ppe, &pas);

    // set ptx = endPt of prev.offset-obj
    UTO2__pt_set_std_pt (typPrv, ooPrv, Ptyp_end, &ptx);

    // set ptx = startPt of prev.offset-obj
    UTO2__pt_set_std_pt (typAct, ooAct, Ptyp_start, &ptx);
    *typoc = 0;

    // goto L_exit;


  //----------------------------------------------------------------
  L_exit:

    // TESTBLOCK
    // printf("ex-CVOFF2_cr_conn %d\n",*typoc);
    // if(*typoc) DEB_dump_obj__ (*typoc, ooc, "ex-CVOFF2_cr_conn");
    // if(*typoc) GR_Disp_obj (*typoc, ooc, Typ_Att_Symb, 0);
    // ERR_raise (__func__);
    // END TESTBLOCK


  return 0;

}


// EOF
