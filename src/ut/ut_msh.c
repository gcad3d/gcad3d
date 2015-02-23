//  ut_msh.c                                  2008-01-24    Franz Reiter.
/*
 *
 * Copyright (C) 2015 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
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
\file  ../ut/ut_msh.c
\brief Mesh-Functions MSH_.. (using struct MemTab) 
\code
=====================================================
List_functions_start:

MSH_bsav_pTab           write pointTable binary > tmp/M#A#.ptab
MSH_bsav_pTabf          write pointTable binary > tmp/M#A#.ptab
MSH_bsav_fTab           write faceTable binary > tmp/M#A#.msh
MSH_bsav_fTabf          write faceTable binary > tmp/M#A#.msh
MSH_bload_pTab          load pointTable from binary file tmp/M#A#.ptab
MSH_bload_pTabf         load pointTable from binary file tmp/M#A#.ptab
MSH_bdel_pTab

MSH_bload_fTab          load faceTable from binary file tmp/M#A#.msh
MSH_bload_fTabf         load faceTable from binary file tmp/M#A#.msh
MSH_bdel_fTab           delete binary faceTableFile
MSH_asav_pTabf          save pointTable ascii into open file
MSH_asav_fTabf          save faceTable ascii into open file
MSH_aload_pTab          load pointTable ascii from open file
MSH_aload_fTab          load faceTable ascii from open file

MSH_msh1                create Mesh from Points & boundary
MSH_msh_OB              create outer boundary
MSH_ibnd_pt2n           find boundary of 2D-pTab
MSH_itess_pt2n          tesselate a 2D-boundary
MSH_nif_findSegP        find all faces covered by Line ip1-ip2

MSH_ibnd_ptn            find boundary of 3D-pTab
MSH_2bnd_fTab_2pt       find boundary around edge (line)
MSH_Ftab_add            add Face to a FaceTable
MSH_ptn2_ptn3           transport all 3D-points > 2D
MSH_if_findSegP         find face with LineSegment ip1-ps2 inside.
MSH_if_find_ptmsh       find faceNr of point pt1 (2D-test point inside face)
MSH_FA_mod_delPt        change all face-pointers >= ip to ip-1
MSH_saf_init            realloc saf-space & init saf
MSH_msh_sap16           remesh all points marked 16 in sap
MSH_ipTab_addu          add point ip to pTab if uniq;

MSH_delFac__            remove deleted records
MSH_delFac_IB__         delete faces inside InnerBoundaries
MSH_delFac_IB_f         test if edge ip1-ip2 is on boundary IB.
MSH_delFac_saf          delete faces inside InnerBoundaries  USE MSH_delFac__

MSH_fTab_BL__           Create Edgeline from Breakline ie1-ie2.
MSH_eTab_fix            recover EdgeLine-pointers ia
MSH_EL_get_tab          get EdgeLineData from EdgeLineTyp.
MSH_EL_del_pt           delete point from EdgeLines
MSH_EL_chg_pt           change point iAct into iNew all EdgeLines
MSH_EL_ck__             test new breakline (intersect EL or go tru point)
MSH_EL_ck_EL__          test if edge intersects other edges
MSH_EL_ck_ln            check if line ip1-ip2 is a Breakline
MSH_EL_ck_EL_4ip        check intersection (4 pointNrs)
MSH_EL_ck_EL_p3ip       check intersection (point, 3 pointNrs)
MSH_EL_ck_pt            test if point is used by existing edgeline
MSH_EL_ck_pTab          test if a point of pTab lies on a (new) edgeLine
MSH_eTab_findRecTyp     get recordNr of EdgeLineTable from edgeLineTyp
MSH_eTab_delRec         delete record in EdgeLineTable
MSH_3ip_EL_iip          get 1 or 2 segments around pt i2 on egde i1

MSH_npt_prjcvmsh_       project curve > Mesh
MSH_msh_pt              add point to mesh; create 2 or 3 new faces.

MSH_pt_ck_pTab          test for "point already exists"; 2D-test w tol

MSH_dump_fTab           dump Faces
MSH_dump_eTab           dump EdgeLines
MSH_dump_saf
MSH_dump_sap

MSH_test_disp1-5

List_functions_end:
=====================================================
- see also:
../ut/ut_tin.c      UFA   Funktionen fuer Faces (Fac3)
../ut/ut_tria.c     Triangle-functions    (using struct Point)

\endcode *//*----------------------------------------


*/
#ifdef globTag
 void MSH(){}
#endif



#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>




// #include "../ut/ut_umem.h"             // UME_alloc_tmp
#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_msh.h"              // Fac3 ..
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_tin.h"              // TYP_EDGLN_..
#include "../gr/ut_UI.h"               // UI_Func... SYM_..
#include "../ut/ut_os.h"               // OS_ ..
#include "../ut/ut_TX.h"               // TX_Print

#include "../xa/xa_mem.h"              // memspc51
#include "../xa/xa.h"                  // WC_modact



typedef_MemTab(int);
typedef_MemTab(char);
typedef_MemTab(Point);
typedef_MemTab(Fac3);
typedef_MemTab(EdgeLine);



// aus xa.c:




//===============================================================================
  int MSH_EL_ck_pTab (Point2 *p1, Point2 *p2, Point *pTab, int pNr, double tol) {
//===============================================================================
// MSH_EL_ck_pTab          test if a point of pTab lies on a (new) edgeLine
// RetCod: -1=no point on edge; else pointNr.


// see also gisPt_ck_EL__

  int    irc, i1;


  // printf("MSH_EL_ck_pTab %d\n",pNr);
  // UT3D_stru_dump (Typ_PT2, p1, "  P1");
  // UT3D_stru_dump (Typ_PT2, p2, "  P1");


  for(i1=0; i1<pNr; ++i1) {
      irc = UT2D_pt_ck_onLine (NULL, p1, p2, (Point2*)&pTab[i1], tol);
        // printf(" onL %d %d\n",irc,i1);

      // skip identical points 0, 2
      if(irc == 1) return i1;
  }

  return -1;

}

//================================================================
  int MSH_EL_ck_EL__ (int *ip3, int *ip4, int *ip1, int *ip2,
                      MemTab(EdgeLine) *eTab, Point *pTab) {
//================================================================
// test if edge intersects other edges
// Output:
//    ip3, ip4
// RetCod: -1=noIntersect, >=0=Intersection.

// see MSH_EL_ck__


  int      irc, i1, i2, *ia, iNr;
  EdgeLine *el1;


  // printf("MSH_EL_ck_EL__ %d %d\n",*ip1, *ip2);


  // loop tru existing edges
  for(i1=0; i1<eTab->rNr; ++i1) {
    el1 = &eTab->data[i1];
    ia  = el1->ia;
    iNr = el1->iNr;
    for(i2=1; i2<iNr; ++i2) {
      // breaklineSeg [i2-1]-[i2]
        // printf(" test %d %d\n",ia[i2-1],ia[i2]);
      *ip3 = ia[i2-1];
      *ip4 = ia[i2];
      irc = MSH_EL_ck_EL_4ip (ip1, ip2, ip3, ip4, pTab);
      if(irc >= 0) return irc;

    }
    // types TYP_EDGLN_IB and TYP_EDGLN_OB and TYP_EDGLN_AB have closing edges
    if(el1->typ != TYP_EDGLN_BL) {
        // printf(" test %d %d\n",ia[iNr-1],ia[0]);
      *ip3 = ia[iNr-1];
      *ip4 = ia[0];
      irc = MSH_EL_ck_EL_4ip (ip1, ip2, ip3, ip4, pTab);
      if(irc >= 0) return irc;
    }
  }


  return -1;

}


//============================================================================
  int MSH_EL_ck_EL_4ip (int *ip1, int *ip2, int *ip3, int *ip4, Point *pTab) {
//============================================================================
// MSH_EL_ck_EL_4ip         check intersection
// RetCod: -1=noIntersect; 0=Intersection; -2=identicalPoints

  int      irc;
  double   d1, d2;
  Point2   ps;


  // check contact
  if((*ip1 == *ip3) ||
     (*ip1 == *ip4) ||
     (*ip2 == *ip3) ||
     (*ip2 == *ip4))   {
      // printf(" MSH_EL_ck_EL_4ip -2\n");
    return -2;
  }

  // intersect
  irc = UT2D_pt_int4pt (&ps, &d1, &d2,
                        (Point2*)&pTab[*ip1], (Point2*)&pTab[*ip2],
                        (Point2*)&pTab[*ip3], (Point2*)&pTab[*ip4]);
  // -1=noIntersection; 1=yes; 0/2/3=goTruPoint; 4=overlap

    // printf("MSH_EL_ck_EL_4ip irc=%d\n",irc);

  return irc;

}


//============================================================================
  int MSH_EL_ck_EL_p3ip (Point *p1,
                          int *ip1, int *ip2, int *ip3, Point *pTab) {
//============================================================================
// MSH_EL_ck_EL_4ip         check intersection
// RetCod: -1=noIntersect; 0=Intersection; -2=identicalPoints

  int      irc;
  double   d1, d2;
  Point2   ps;


  // printf("MSH_EL_ck_EL_p3ip \n");
  // UT3D_stru_dump (Typ_PT, p1, "  P1");
  // UT3D_stru_dump (Typ_PT, &pTab[*ip1], "  P2");
  // UT3D_stru_dump (Typ_PT, &pTab[*ip2], "  P3");
  // UT3D_stru_dump (Typ_PT, &pTab[*ip3], "  P4");


  // intersect
  irc = UT2D_pt_int4pt (&ps, &d1, &d2,
                        (Point2*)p1,
                        (Point2*)&pTab[*ip1],
                        (Point2*)&pTab[*ip2],
                        (Point2*)&pTab[*ip3]);
  // -1=noIntersection; 1=yes; 0/2/3=goTruPoint; 4=overlap
    // printf(" ex UT2D_pt_int4pt irc=%d\n",irc);

  if(irc == -1) return -1;
  if((irc == 0) || (irc == 2)) return -2;

  return 0;

}


//================================================================
  int MSH_FA_mod_delPt (MemTab(Fac3) *fTab, int ip) {
//================================================================
/// MSH_FA_mod_delPt         change all face-pointers >= ip to ip-1

  int   i1;
  Fac3  *fa = fTab->data;

  for(i1=0; i1<fTab->rNr;  ++i1) {
    if(fa[i1].i1 >= ip) fa[i1].i1 -= 1;
    if(fa[i1].i2 >= ip) fa[i1].i2 -= 1;
    if(fa[i1].i3 >= ip) fa[i1].i3 -= 1;
  }

  return 0;

}


//================================================================
   int MSH_aload_pTab (char *lBuf, int bufSiz, FILE *fpi) {
//================================================================
/// \code
/// MSH_aload_pTab             load pointTable ascii from open file
/// Input:
///   all following ascii-lines from open file fpi; "SECTIONEND" = end.
/// Output:
///   write into binary file
/// \endcode


  int    irc, i1, pNr;
  long   ld;
  char   *cp1, *cp2, cbuf[256];
  // MemTab(Point) pTab = MemTab_Init(sizeof(Point), Typ_PT, 10000);
  MemTab(Point) pTab = MemTab_empty;

  MemTab_ini (&pTab, sizeof(Point), Typ_PT, 10000);


  // printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX \n");
  printf("MSH_aload_pTab |%s|\n",lBuf);


  // 1. line: nr of points ("ptnr=%d")
  fgets (cbuf, bufSiz, fpi);
  pNr = atoi(&cbuf[5]);
    // printf(" pNr=%d\n",pNr);

  // check space for pNr records
  irc = MemTab_check ((MemTab*)&pTab, &ld, pNr);
  if(irc < 0) goto L_EOM;


  i1 = 0;
  while (!feof (fpi)) {
    if (fgets (cbuf, bufSiz, fpi) == NULL) break;
    UTX_CleanCR (cbuf);
    if(!strcmp(cbuf, "SECTIONEND")) break;
    pTab.data[i1].x = strtod(cbuf, &cp1);
    pTab.data[i1].y = strtod(cp1, &cp2);
    pTab.data[i1].z = strtod(cp2, &cp1);
      // printf("pt[%d] %f %f %f\n",i1,pTab[i1].x,pTab[i1].y,pTab[i1].z);
    ++i1;
  }
  pTab.rNr = i1;
    // printf(" i1=%d\n",i1);

  // "SECTION PTAB fNam"
  //  01234567890123
  sprintf(cbuf, "%s%s.ptab",OS_get_tmp_dir(),&lBuf[13]);
  MSH_bsav_pTabf (&pTab, cbuf);

  MemTab_free ((MemTab*)&pTab);

  return 0;


  L_EOM:
    printf("MSH_aload_pTab EOM\n");
    return -1;

}


//================================================================
   int MSH_aload_fTab (char *lBuf, int bufSiz, FILE *fpi) {
//================================================================
/// \code
/// MSH_aload_fTab             load faceTable ascii from open file
/// cbuf = buffer with line  "SECTION MESH fNam"
/// \endcode


  int      irc, i1, i2, i3, fNr, eNr, pNr;
  long     ld;
  char     *cp1, *cp2, *cbuf;
  EdgeLine el1;

  MemTab(Fac3) fTab = MemTab_empty;
  MemTab(EdgeLine) eTab = MemTab_empty;
  MemTab(int) eDat = MemTab_empty;




  MemTab_ini (&fTab, sizeof(Fac3), Typ_Fac3, 10000);
  MemTab_ini (&eTab, sizeof(EdgeLine), Typ_EdgeLine, 10);
  MemTab_ini (&eDat, sizeof(int), Typ_Int4, 50);



  // use memspc501 for long edgelines
  cbuf = memspc501;



  // printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
  // printf("MSH_aload_fTab |%s|\n",lBuf);
  // printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");


  // 1. line: nr of faces ("faces=%d edgelines=%d")
  // old version was: "facnr=370"
  fgets (cbuf, bufSiz, fpi);
  fNr = atoi(&cbuf[6]);

  cp1 = strstr(cbuf, "lines=");
  if(cp1) {
    cp1 += 6;
    eNr = atoi(cp1);
  } else eNr = 0;
    // printf(" fNr=%d eNr=%d\n",fNr,eNr);


  // check space for fNr records
  irc = MemTab_check (&fTab, &ld, fNr);
  if(irc < 0) goto L_EOM;
  fTab.rNr = fNr;

  // get space for eNr records
  // if(eNr > 0) {
    irc = MemTab_check (&eTab, &ld, eNr);
    if(irc < 0) goto L_EOM;
  // }


  i1 = 0;
  while (!feof (fpi)) {
    if (fgets (cbuf, bufSiz, fpi) == NULL) break;
    UTX_CleanCR (cbuf);
    if(!strcmp(cbuf, "SECTIONEND")) goto L_wr;

    fTab.data[i1].i1 = strtol(cbuf, &cp1, 10);
    fTab.data[i1].i2 = strtol(cp1, &cp2, 10);
    fTab.data[i1].i3 = strtol(cp2, &cp1, 10);
      // printf("fac[%d] %d %d %d\n",i1,
        // fTab[i1].i1,fTab[i1].i2,fTab[i1].i3);
    ++i1;
    if(i1 >= fNr) break;
  }
    // printf(" i1=%d\n",i1);

  // load egdeLines

  i1 = 0;
  while (!feof (fpi)) {
    if (fgets (cbuf, bufSiz, fpi) == NULL) break;
    UTX_CleanCR (cbuf);
    if(!strcmp(cbuf, "SECTIONEND")) goto L_wr;

    // edgeline=0 points=5 edgetyp=4
      // printf(" elHdr=|%s|\n",cbuf);
    // find pNr = nrOfPoints & typ
    cp1 = strstr(cbuf, "nts=");
    cp1 += 4;
    pNr = atoi(cp1);
    cp2 = strstr(cp1, "typ=");
    cp2 += 4;
    i2 = atoi(cp2);
      // printf(" el-pNr=%d typ=%d\n",pNr,i2);

    // save edgeline
    el1.iNr = pNr;
    el1.typ = i2;
    MemTab_sav (&eTab, &ld, &el1, 1);

    // check space for points
    irc = MemTab_check (&eDat, &ld, pNr);
    if(irc < 0) goto L_EOM;

    // read & save points
    if (fgets (cbuf, bufSiz, fpi) == NULL) break;
    UTX_CleanCR (cbuf);
    if(!strcmp(cbuf, "SECTIONEND")) goto L_wr;
    cp1 = cbuf;
    for(i2=0; i2<pNr; ++i2) {
      i3 = strtol(cp1, &cp2, 10);
        // printf(" el=%d ip[%d] = %d\n",i1,i2,i3);
      MemTab_sav (&eDat, &ld, &i3, 1);
      cp1 = cp2 + 1;
    }
    ++i1;
    if(i1 >= eNr) break;
  }


  MSH_eTab_fix (&eTab, &eDat);


  // get "SECTIONEND"
  while (!feof (fpi)) {
    if (fgets (cbuf, bufSiz, fpi) == NULL) break;
    UTX_CleanCR (cbuf);
    if(!strcmp(cbuf, "SECTIONEND")) break;
  }


  //  01234567890123        - decode fNam from line
  L_wr:
  // "SECTION MESH fNam"
  sprintf(cbuf, "%s%s.msh",OS_get_tmp_dir(),&lBuf[13]);
  // MSH_bsav_fTabf (fTab, fNr, cbuf);
  MSH_bsav_fTabf (&fTab, &eTab, &eDat, cbuf);


  // free(fTab);
  MemTab_free (&eDat);
  MemTab_free (&eTab);
  MemTab_free (&fTab);

  return 0;

  L_EOM:
    printf("MSH_aload_fTab EOM\n");
    return -1;

}


//================================================================
  int MSH_asav_pTabf (Point *pTab, int pNr, char *fNam, FILE *fpo) {
//================================================================
/// MSH_asav_pTabf             save pointTable ascii into open file

  int    i1;
  char   cbuf[80];

  // printf("MSH_asav_pTabf |%s|\n",fNam);

  fprintf(fpo, "SECTION PTAB %s\n",fNam);

  fprintf(fpo,"ptnr=%d\n",pNr);

  for(i1=0; i1<pNr; ++i1) {
    cbuf[0] = '\0';
    UTX_add_fl_10  (cbuf, pTab[i1].x);
    strcat(cbuf, " ");
    UTX_add_fl_10  (cbuf, pTab[i1].y);
    strcat(cbuf, " ");
    UTX_add_fl_10  (cbuf, pTab[i1].z);
    fprintf(fpo,"%s\n",cbuf);
  }

  fprintf(fpo,"SECTIONEND\n");

  return 0;

}


//================================================================
  int MSH_asav_fTabf (MemTab(Fac3) *fTab,
                      MemTab(EdgeLine) *eTab,
                      MemTab(int) *eDat,
                      char *fNam, FILE *fpo) {
//================================================================
/// MSH_asav_fTabf             save faceTable ascii into open file

  int       i1, i2;
  EdgeLine  *el1;

  // printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
  // printf("MSH_asav_fTabf |%s|\n",fNam);
  // printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");

  fprintf(fpo, "SECTION MESH %s\n",fNam);


  fprintf(fpo,"faces=%d edgelines=%d\n",fTab->rNr,eTab->rNr);


  for(i1=0; i1<fTab->rNr; ++i1)
    fprintf(fpo,"%d %d %d\n",fTab->data[i1].i1,
                             fTab->data[i1].i2,
                             fTab->data[i1].i3);


  for(i1=0; i1<eTab->rNr; ++i1) {
    el1 = &eTab->data[i1];
    fprintf(fpo,"edgeline=%d points=%d edgetyp=%d\n",i1,el1->iNr,el1->typ);
    for(i2=0; i2<el1->iNr; ++i2) fprintf(fpo,"%d ",el1->ia[i2]);
    fprintf(fpo,"\n");
  }


  fprintf(fpo,"SECTIONEND\n");

  return 0;

}


//================================================================
  int MSH_bsav_pTab (MemTab *pTab, char *mdlNam, int surNr) {
//================================================================
/// \code
/// MSH_bsav_pTab              write pointTable binary > tmp/<mdlnam>_A#.ptab
/// write OutFile
/// OutFilename: M#A#.ptab
/// \endcode

  char lBuf[256];


  // printf("MSH_bsav_pTab mdlNam=%s surNr=%d nr=%d\n",mdlNam,surNr,pTab->rNr);

  // /temp/_A1.ptab
  // /temp/smNam_A1.ptab
  sprintf(lBuf, "%s%s_A%d.ptab",OS_get_tmp_dir(),mdlNam,surNr);
    // printf(" fNam pt |%s|\n",lBuf);

  return MSH_bsav_pTabf (pTab, lBuf);

}


//================================================================
  int MSH_bsav_pTabf (MemTab *pTab, char *fNam) {
//================================================================

  int  pNr;
  FILE *fp2;


  // printf("MSH_bsav_pTabf |%s|\n",fNam);


  if((fp2=fopen(fNam,"wb")) == NULL) {
    TX_Print("MSH_bsav_pTabf E001\n",fNam);
    return -1;
  }

  pNr = pTab->rNr;
  fwrite(&pNr, sizeof(int), 1, fp2);       // nr of points
  fwrite(pTab->data, sizeof(Point), pNr, fp2);  // points
  fclose(fp2);

  return 0;

}


//================================================================
  int MSH_bsav_fTab (MemTab *fTab, MemTab *eTab, MemTab *eDat,
                      char *mdlNam, int surNr) {
//================================================================
/// \code
/// MSH_bsav_fTab              write faceTable binary > tmp/M#A#.msh
/// write OutFile
/// OutFilename: M#A#.msh
/// \endcode

  char lBuf[256];


  // printf("MSH_bsav_fTab %d %d\n",mdlNr,surNr);


  // /temp/_A1.msh
  // /temp/smNam_A1.msh
  sprintf(lBuf, "%s%s_A%d.msh",OS_get_tmp_dir(),mdlNam,surNr);
    // printf("MSH_bsav_fTab |%s|\n",lBuf);

  return MSH_bsav_fTabf (fTab, eTab, eDat, lBuf);
  // return MSH_bsav_fTabf (fTab, eTab, eNr, lBuf);

}


//================================================================
  int MSH_bsav_fTabf (MemTab *fTab, MemTab *eTab, MemTab *eDat,
                       char *fNam) {
                       // EdgeLine *eTab, int eNr,
//================================================================

  int  iNr, iSiz;
  FILE *fp2;

  // printf("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS \n");
  // printf("MSH_bsav_fTabf |%s| fNr=%d eNr=%d eDat=%d\n",fNam,
          // fTab->rNr,eTab->rNr,eDat->rNr);
  // MSH_dump_eTab (eTab, eDat);


  if((fp2=fopen(fNam,"wb")) == NULL) {
    TX_Print("MSH_bsav_fTabf E001\n",fNam);
    return -1;
  }

  iNr  = fTab->rNr;
  iSiz = fTab->rSiz;
  fwrite(&iNr, sizeof(int), 1, fp2);
  fwrite(fTab->data, iSiz, iNr, fp2);

  // save EdgeLines
  iNr  = eTab->rNr;
  iSiz = eTab->rSiz;
  fwrite(&iNr, sizeof(int), 1, fp2);
  fwrite(eTab->data, iSiz, iNr, fp2);

  // save EdgeLine - data
  iNr  = eDat->rNr;
  iSiz = eDat->rSiz;
  fwrite(&iNr, sizeof(int), 1, fp2);
  fwrite(eDat->data, iSiz, iNr, fp2);


  fclose(fp2);

  return 0;

}



//================================================================
  int MSH_bload_fTab (MemTab(Fac3) *fTab,
                      MemTab(EdgeLine) *eTab,
                      MemTab(int) *eDat,
                      char *mdlNam, int surNr) {
//================================================================
/// \code
/// MSH_bload_fTab             load faceTable from binary file tmp/M#A#.msh
///   fTab is malloced; you must free it !
/// \endcode

  char fNam[256];


  // printf("MSH_bload_fTab \n");


  // load MeshFile
  // /temp/_A1.msh
  // /temp/smNam_A1.msh
  sprintf(fNam, "%s%s_A%d.msh",OS_get_tmp_dir(),mdlNam,surNr);
    // printf(" fNam fc |%s|\n",fNam);

  return MSH_bload_fTabf (fTab, eTab, eDat, fNam);

}


//================================================================
  int MSH_bload_fTabf (MemTab *fTab, MemTab *eTab, MemTab *eDat,
                        char *fNam) {
//================================================================
/// \code
/// MSH_bload_fTab             load faceTable from binary file tmp/M#A#.msh
///   fTab is malloced; you must free it !
/// \endcode

  int    irc, iNr, iSiz;
  long   lSiz, ld;
  MemTab mt1;
  FILE   *fp1;

  // printf("MSH_bload_fTabf |%s|\n",fNam);

  if((fp1=fopen(fNam,"rb")) == NULL) {
    TX_Print("MSH_bload_fTabf E001 %s\n",fNam);
    return -1;
  }

  // // read faces-MemTab
  // fread(&mt1, sizeof(MemTab), 1, fp1);

  // // check recSiz
  // if(fTab->rSiz != mt1.rSiz) TX_Print("MSH_bload_fTabf I001");



  //----------------------------------------------------------------
  // read nr of faces
  fread(&iNr, sizeof(int), 1, fp1);
    // printf(" nr_fTab=%d\n",iNr);

  // clear fTab
  fTab->rNr = 0;

  // check space for iNr records
  irc = MemTab_check (fTab, &ld, iNr);
  if(irc < 0) goto L_EOM;

  // load faces
  iSiz = fTab->rSiz;
  fread(fTab->data, iSiz, iNr, fp1);
  fTab->rNr = iNr;


  //----------------------------------------------------------------
  // load EdgeLines
  fread(&iNr, sizeof(int), 1, fp1);
    // printf(" nr_eTab=%d\n",iNr);

  if(eTab) {
    // clear eTab
    eTab->rNr = 0;
    // check space for iNr records
    irc = MemTab_check (eTab, &ld, iNr);
    if(irc < 0) goto L_EOM;
    // load EdgeLines
    iSiz = eTab->rSiz;
    fread(eTab->data, iSiz, iNr, fp1);
    eTab->rNr = iNr;

  } else {
    lSiz = iNr * sizeof(int);
    fseek (fp1, lSiz, 1);
  }



  //----------------------------------------------------------------
  // load EdgeData
  fread(&iNr, sizeof(int), 1, fp1);
    // printf(" nr_eTab=%d\n",iNr);

  if(eDat) {
    // clear eDat
    eDat->rNr = 0;
    // get space for iNr records
    irc = MemTab_check (eDat, &ld, iNr);
    if(irc < 0) goto L_EOM;
    // load EdgeData
    iSiz = eDat->rSiz;
    fread(eDat->data, iSiz, iNr, fp1);
    eDat->rNr = iNr;

  } else {
    lSiz = iNr * sizeof(int);
    fseek (fp1, lSiz, 1);
  }


  //----------------------------------------------------------------
  fclose(fp1);


  // gis_DatEtab_fix ();
  if(eTab && eDat) MSH_eTab_fix (eTab, eDat);


    // printf("ex MSH_bload_fTabf %d %d %d\n",fTab->rNr,eTab->rNr,eDat->rNr);
    // MSH_dump_eTab (eTab, eDat);

  return 0;


  L_EOM:
    printf("MSH_bload_fTabf EOM\n");
    return -1;

}


//===================================================================
  int MSH_bload_pTab (MemTab(Point) *pTab, char *mdlNam, int surNr) {
//===================================================================
/// \code
/// MSH_bload_pTab             load pointTable from binary file tmp/M#A#.ptab
///   pTab is malloced; you must free it !
/// \endcode

  char fNam[256];

  // printf("PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP \n");
  // printf("MSH_bload_pTab |%s| %d\n",mdlNam,surNr);

  // load PointFile  (write: lxml_read)
  // sprintf(fNam, "%sM%dA%d.ptab",OS_get_tmp_dir(),mdlNr,surNr);
  // /temp/_A1.ptab
  // /temp/smNam_A1.ptab
  sprintf(fNam, "%s%s_A%d.ptab",OS_get_tmp_dir(),mdlNam,surNr);
    // printf(" fNam pt |%s|\n",fNam);

  return MSH_bload_pTabf (pTab, fNam);

}


//================================================================
  int MSH_bload_pTabf (MemTab *pTab, char *fNam) {
//================================================================
/// \code
/// MSH_bload_pTabf            load pointTable from binary file tmp/M#A#.ptab
///   pTab is malloced; you must free it !
/// \endcode

  int   pNr;
  long  lSiz, l1;
  FILE  *fp1;

  // printf("MSH_bload_pTabf |%s| rMax=%d rNr=%d\n",fNam,pTab->rMax,pTab->rNr);

  if((fp1=fopen(fNam,"rb")) == NULL) {
    // TX_Print("MSH_bload_pTabf E001\n",fNam);
    return -1;
  }

  // read nr of points in file
  fread(&pNr, sizeof(int), 1, fp1);
    // printf(" nr_pTab=%d\n",pNr);
  if(pTab->rMax > pNr) goto L_read;

  lSiz = (pNr) * sizeof(Point);
  pTab->data = (Point*) realloc(pTab->data, lSiz);
    // printf(" >>>>>>>>>>>>> pTab = %p\n",*pTab);
  if(pTab->data == NULL) goto L_EOM;
  pTab->rMax = pNr;


  // read points
  L_read:
    pTab->rNr = pNr;
    fread(pTab->data, sizeof(Point), pNr, fp1);
    fclose(fp1);

    // printf("ex MSH_bload_pTabf rMax=%d rNr=%d\n",pTab->rMax,pTab->rNr);

    return 0;


  L_EOM:
    printf("MSH_bload_pTabf EOM\n");
    return -1;

}


//============================================================================
  int MSH_Ftab_add (Fac3 *fa, int *fNr, int fSiz, int ip1, int ip2, int ip3) {
//============================================================================
/// add a Face to a FaceTable

  // printf("MSH_Ftab_add %d %d %d %d %d\n",ip1,ip2,ip3,*fNr,fSiz);


  if(*fNr >= fSiz) {TX_Error("MSH_Ftab_add E001"); return -1;}

  fa[*fNr].i1 = ip1;
  fa[*fNr].i2 = ip2;
  fa[*fNr].i3 = ip3;

  ++(*fNr);

  return 0;

}


//================================================================
  int MSH_ibnd_ptn (int *iba, int *ibNr, Point *pa, int pNr) {
//================================================================
/// \code
/// MSH_ibnd_ptn          get boundary (iTab) for n points (pTab)
/// ACHTUNG: memspc501 memspc201 memspc55 used !!
/// Input:
///   ibNr     size of iba
/// Output:
///   iba      memspc for the indices of the boundarypoints into pa
///   ibNr     nr of indices
/// \endcode


  int     i1, p2Nr;
  // int     *ipa;
  Point2  *p2a;


  // printf("MSH_ibnd_ptn %d ibSiz = %d\n",pNr,*ibNr);
  // for(i1=0;i1<pNr;++i1) UT3D_stru_dump (Typ_PT,&pa[i1],"P[%d]",i1);

  i1 = sizeof(memspc501) / sizeof(Point2);  // 16 byte
  if(i1 < pNr) {
    p2a = (Point2*) UME_alloc_tmp (pNr * sizeof(Point2));
    if(p2a == NULL) {
      TX_Error("MSH_ibnd_ptn E001 %d %d",pNr,i1);
      return -1;
    }

  } else {
    p2a = (Point2*)memspc501;
  }


  // ipa = (int*)memspc201;
  // i1 = sizeof(memspc201) / sizeof(int);    // 4 byte; mind. p2a-Siz/4
  // if(i1 < pNr) {TX_Error("MSH_ibnd_ptn E002"); return -1;}


  // transport all points > 2D (pa -> p2a)
  // and set indexArray ipa
  // MSH_ptn2_ptn3 (p2a, &p2Nr, ipa, pa, pNr);
  MSH_ptn2_ptn3 (p2a, &p2Nr, pa, pNr);


  // get boundary into iba
  // i1 = MSH_ibnd_pt2n (iba, ibNr, p2a, p2Nr, ipa);
  i1 = MSH_ibnd_pt2n (iba, ibNr, p2a, p2Nr);
  if(i1 < 0) return -1;


  // // copy the pointers into pa to iba (replace 2D-pointers with 3D-pointers).
  // for(i1=0; i1 < *ibNr; ++i1) {
    // iba[i1] = ipa[iba[i1]];
  // }


  // TEST: display polygon
  // Point  cv[iNr+1];
  // for(i1=0; i1<bNr; ++i1) { cv[i1] = pa[ipa[iba[i1]]]; }
  // cv[iNr] = cv[0];
  // GR_Disp_cv (cv, iNr+1, 5);


  return 0;

}


//==========================================================================
  int MSH_ibnd_pt2n (int *iba, int *ibNr, Point2 *p2a, int p2Nr) {
//==========================================================================
/// \code
/// MSH_ibnd_pt2n          get boundary (iTab) for n 2D-points (p2a)
/// Does not provide the closing-point (last point is not equal to first point !)
/// Input:
///   ibNr     size of iba
/// Output:
///   iba      memspc for the indices of the boundarypoints into p2a
///   ibNr     nr of indices
/// \endcode


  int     irc, i1, i2, iNxt, iSiz, ii0, ii1, ii2, ii3, ii4, iin;
  int     ibSiz, bNr, ie0, ie1, ie2, ie3;
  char    *sa;
  double  d1, d2, d3, d4;
  Vector2 ve;


  // printf("MSH_ibnd_pt2n %d ibSiz = %d\n",p2Nr,*ibNr);
  // for(i1=0;i1<p2Nr;++i1) UT3D_stru_dump (Typ_PT2,&p2a[i1],"P[%d]",i1);

  ibSiz = *ibNr;


  // get spc for statArray sa
  sa = UME_alloc_tmp (p2Nr + 4);
  // init sa
  for(i1=0; i1<p2Nr; ++i1) sa[i1] = 0;



  // find extreme-points; (highest, lowest, leftmost, rightmost)
  i1 = UT2D_i4pt_npt (&ii1, &ii2, &ii3, &ii4, p2Nr, p2a);
                  //   le    ri     lo    hi
                  //   xMin  xMax  yMin  yMax
  if(i1 < 0) return -1;


  // check if more than 2 different points
  if((ii1 == ii3) && (ii2 == ii4)) {
    // only 2 points; 0 triangles
    bNr = 2;
    iba[0] = ii1;   sa[ii1] = -1;
    iba[1] = ii2;   sa[ii2] = -1;
    ie1 = 0;
    goto L_nxt_edg;
  }

  // make 1 or 2 CCW-triangles from extreme-points
  bNr = 3;
  if(ii1 == ii4) {  // xMin == yMax
    iba[0] = ii1;
    iba[1] = ii3;
    iba[2] = ii2;
    goto L_1;
  }

  if(ii2 == ii4) {  // xMax == yMax
    iba[0] = ii2;
    iba[1] = ii1;
    iba[2] = ii3;
    goto L_1;
  }

  if(ii1 == ii3) {  // xMin == yMin
    iba[0] = ii1;
    iba[1] = ii2;
    iba[2] = ii4;
    goto L_1;
  }

  if(ii2 == ii3) {  // xMax == yMin
    iba[0] = ii2;
    iba[1] = ii4;
    iba[2] = ii1;
    goto L_1;
  }

  // 2 triangles
  iba[0] = ii2;
  iba[1] = ii4;
  iba[2] = ii1;
  iba[3] = ii3;
  bNr = 4;


  // loop tru all points; check if inside tria; if yes: stat = -2.
  L_1:
  ii1 = iba[0];  sa[ii1] = -1;
  ii2 = iba[1];  sa[ii2] = -1;
  ii3 = iba[2];  sa[ii3] = -1;

    // printf(" iba %d %d %d\n",iba[0],iba[1],iba[2],iba[3]);
    // printf(" bNr=%d ii4=\n",bNr,iba[3]);
    // GR_Disp_trip2 (&p2a[ii1], &p2a[ii2], &p2a[ii3], 8);
  // tria must be ccw; test ..
  // i1 = UT2D_orient_3pt (&p2a[ii1], &p2a[ii2], &p2a[ii3]);
    // printf(" ori.t1 = %d\n",i1);
  // if(i1 > 0) {
    // MEM_swap_int (&ii1, &ii2);
    // MEM_swap_int (&iba[0], &iba[1]);
  // }


  for(i1=0; i1<p2Nr; ++i1) {
    if(sa[i1] < 0) continue;
    irc = UT2D_ck_pt_in_tria (&p2a[ii1], &p2a[ii2], &p2a[ii3], &p2a[i1]);
    if(irc > 0) {
      sa[i1] = 0;
    } else {
      sa[i1] = -1;  // inside or on edge
        // GR_Disp_pt2 (&p2a[i1], SYM_STAR_S, 2);
    }
  }


  // test 2. triangle
  if(bNr > 3) {
    ii1 = iba[0];
    ii2 = iba[2];  sa[ii2] = -1;
    ii3 = iba[3];  sa[ii3] = -1;
      // printf(" tri2-ii1-3 %d %d %d\n",ii1,ii2,ii3);
      // GR_Disp_trip2 (&p2a[ii1], &p2a[ii2], &p2a[ii3], 9);
    // tria must be ccw; test ..
    // i1 = UT2D_orient_3pt (&p2a[ii1], &p2a[ii2], &p2a[ii3]);
      // printf(" ori.t2 = %d\n",i1);
    // if(i1 > 0) {
      // MEM_swap_int (&ii2, &ii3);
      // MEM_swap_int (&iba[2], &iba[3]);
    // }

    for(i1=0; i1<p2Nr; ++i1) {
      if(sa[i1] < 0) continue;
      irc = UT2D_ck_pt_in_tria (&p2a[ii1], &p2a[ii2], &p2a[ii3], &p2a[i1]);
      if(irc > 0) {
        sa[i1] = 0;
      } else {
        sa[i1] = -1;  // inside or on edge
          // GR_Disp_pt2 (&p2a[i1], SYM_STAR_S, 2);
      }
    }
  }



  // variables:
  // pNr      nr of points in pa
  // p2a      the 2D-table
  // p2Nr      nr of points in p2a
  // sa       status-table (chars) for p2a
  // iba      boundary; indices into p2a
  // bNr      nr of points in iba
  // ie1, ie2 pointers to the active edge into b2a


   // for(i1=0; i1<p2Nr; ++i1) printf(" sa[%d] = %d\n",i1,sa[i1]);


  //----------------------------------------------------------------
  ie1 = 0;   // TEST; def=0 !!!

  L_nxt_edg:
  // loop tru edges; find all pt's right of edge; activate with stat=1;
  // eliminate all points on edge (stat = -1).
  // no activate points: continue with next edge.

  if(ie1 >= bNr) goto L_done;

  // disp contour ..
    // for(i1=0; i1<bNr; ++i1) printf("%d; ",iba[i1]);
    // for(i1=0; i1<bNr; ++i1) printf("%d; ",ipa[iba[i1]]);
    // printf("\n");


  // fix ie1, ie2 to point to the next edge.
  // fix i3 = nect point after ie3
  ie2 = ie1 + 1;
  if(ie2 >= bNr) ie2 = 0;

  ie3 = ie2 + 1;
  if(ie3 >= bNr) ie3 = 0;

  ie0 = ie1 - 1;
  if(ie0 < 0) ie0 = bNr - 1;

  ii0 = iba[ie0];
  ii1 = iba[ie1];
  ii2 = iba[ie2];
  ii3 = iba[ie3];
    // printf(" -------------L_nxt_edg: %d (%d) %d (%d)\n",ie1,ii1,ie2,ii2);


  // active-edge -> vector
  UT2D_vc_2pt (&ve, &p2a[ii1], &p2a[ii2]);


  // mark all points on the right of edge
  iin = -1;
  for(i1=0; i1<p2Nr; ++i1) {
    if(sa[i1] < 0) continue;
    irc = UT2D_sid_ptvc (&p2a[i1], &p2a[ii1], &ve);
    if(irc < 0) {
      sa[i1] = 1;  // is on the right; activate this point ..
      ++iin;
        // printf(" found right: %d\n",i1);
        // GR_Disp_pt2 (&p2a[i1], SYM_STAR_S, 2);
    }
  }
  if(iin < 0) {  // no more points; goto next edge ..
    ++ie1;
    goto L_nxt_edg;
  }


  // loop tru all active points; check distance from active edge.
  // select point with max dist as newpoint
  d3 = 0.;
  // d4 = 0.;
  iin = -1;
  for(i1=0; i1<p2Nr; ++i1) {
    if(sa[i1] < 1) continue;
    // d1 = UT2D_slen_nor3pt (&p2a[ii2], &p2a[i1], &p2a[ii1]);
    d1 = fabs(UT2D_slen_nor_2pt_vcNo (&ve, &p2a[ii1], &p2a[i1]));
        // printf(" found right: %d %f\n",ipa[i1],d1);
    if(d1 > d3) { d3 = d1; iin = i1;}
/*
    UT2D_lenq_2pt (&d1, &p2a[i1], &p2a[ii1]);
    UT2D_lenq_2pt (&d2, &p2a[i1], &p2a[ii2]);
        printf(" found right: %d %f %f\n",ipa[i1],d1,d2);
    d1 *= d2;
    if(d1 > d3) { d3 = d1; iin = i1;}
*/
  }
  if(iin < 0) {  // no more points; goto next edge ..
    ++ie1;
    goto L_nxt_edg;
  }
    // printf(" iin=%d\n",iin);
    // GR_Disp_pt2 (&p2a[iin], SYM_STAR_S, 2);
    


  // check all active points, if inside triangle activeEdge - newpoint;
  // if yes: eliminate point (stat = -1).
  for(i1=0; i1<p2Nr; ++i1) {
    if(sa[i1] < 1) continue;
    irc = UT2D_ck_pt_in_tria (&p2a[ii1], &p2a[iin], &p2a[ii2], &p2a[i1]);
    if(irc > 0) continue;
    sa[i1] = -1; // inside.
  }


  // Punkt iin einfuegen oder ersetzen.
  sa[iin] = -1;



  // Testen ob der Punkt ii2 im 3Eck ii1-iin-ii3 enthalten ist
  irc = UT2D_ck_pt_in_tria (&p2a[ii1], &p2a[iin], &p2a[ii3], &p2a[ii2]);
  if(irc < 1) {  // inside or on contour: replace
    // Punkt iin befindet sich vor dem Punkt ie2
    // iin replaces ie2
    iba[ie2] = iin;
    goto L_nxt_prep;
  }


  // Testen ob der Punkt ii1 im 3Eck ii0-iin-ii2 enthalten ist
  irc = UT2D_ck_pt_in_tria (&p2a[ii0], &p2a[iin], &p2a[ii2], &p2a[ii1]);
  if(irc < 1) {  // inside or on contour: replace
    // Punkt iin befindet sich hinter dem Punkt ie1
    // iin replaces ie1
    iba[ie1] = iin;
    goto L_nxt_prep;
  }


  // Punkt iin befindet sich seitlich von der Kante:
  // insert newpoint into boundary between activeEdgepoints.
  // newpoint=iin; boundary=iba; position= after record ie1
  if(bNr >= ibSiz) {TX_Error("MSH_ibnd_pt2n E004"); return -1;}
    // printf(" insert point %d\n",iin);
  MEM_ins_IndRec (iba, &bNr, ie1+1, iin);



  // deactivate all active points
  L_nxt_prep:
  for(i1=0; i1<p2Nr; ++i1) if(sa[i1] > 0) sa[i1] = 0;


  // test the new created edge.
  // if(bNr < 8)          // TEST ONLY
  goto L_nxt_edg;



  //----------------------------------------------------------------
  L_done:
  // contour finished;
  *ibNr = bNr;


  // TEST: display polygon
  // MSH_test_disp2 (iba, bNr, pa);
    // printf("ex MSH_ibnd_pt2n %d\n",*ibNr);

  return 0;

}


//==========================================================================
  // int MSH_ptn2_ptn3 (Point2 *p2a, int *p2Nr, int *ipa, Point *pa, int pNr) {
  int MSH_ptn2_ptn3 (Point2 *p2a, int *p2Nr, Point *pa, int pNr) {
//==========================================================================
/// \code
/// transport all points > 2D (pa -> p2a);
/// set indexArray ipa (points into pa)
/// skip empty (deleted) points;
/// skip identical points
/// ACHTUNG: p2a & ipa must have size pNr !
/// \endcode

// es sollte Normalvektor benutzt werden; dzt copy X- & Y-Coord only.


  int    i1, i2, iNr;


  iNr = 0;

  for(i1=0; i1<pNr; ++i1) {
    if(pa[i1].x == UT_VAL_MAX) continue;     // skip empty points
    p2a[iNr] = UT2D_pt_pt3 (&pa[i1]);

/*
    // gleiche Punkt muessen (im 2D) eliminiert werden
    for(i2=0; i2<iNr; ++i2) {
      if(UT2D_comp2pt (&p2a[i2], &p2a[iNr], 0.1) == 1) {      // UT_DISP_cv
        // printf(" skip pt %d %d\n",i1,i2);
        goto L_skip_2D_pt;
      }
    }
*/
    // ipa[iNr] = i1;
    ++iNr;

    L_skip_2D_pt:;
  }

  *p2Nr = iNr;

  return 0;

}

 
//================================================================
  int MSH_bdel_pTab (char *mdlNam, int surNr) {
//================================================================

  char fNam[256];


  // printf("MSH_bdel_pTab %d %d\n",mdlNr,surNr);


  // load PointFile  (write: lxml_read)
  // sprintf(fNam, "%sM%dA%d.ptab",OS_get_tmp_dir(),mdlNr,surNr);
  // /temp/_A1.ptab
  // /temp/smNam_A1.ptab
  sprintf(fNam, "%s%s_A%d.ptab",OS_get_tmp_dir(),mdlNam,surNr);
    // printf(" fNam pt |%s|\n",fNam);

  OS_file_delete (fNam);

  return 0;

}


//================================================================
  int MSH_test_disp_p1 (Point* pTab, int pNr) {
//================================================================
/// display pointNr's of Gis-Points in 2D

  int   ii;

  for(ii=0; ii<pNr;  ++ii) {
    GR_Disp_txi2 ((Point2*)&pTab[ii], ii, 4);
  }


  return 0;

}

 
//========================================================================
  int MSH_test_disp1 (int *iba, int iNr, Point *pa, int *ipa) {
//========================================================================
/// TEST: display polygon

  int    i1, ii;
  //Point  cv[iNr+1];
  Point  *cv;


  printf("MSH_test_disp1 %d\n",iNr);

  cv = (Point*)UME_alloc_tmp(sizeof(Point)*(iNr + 1));
  // cv = (Point*)_alloca(sizeof(Point)*(iNr + 1));

  for(i1=0; i1<iNr; ++i1) {
    // printf(" P[%d] ib=%d\n",i1,ib[i1]);
    // printf("       ia=%d\n",ia[ib[i1]]);
    ii = ipa[iba[i1]];
    cv[i1] = pa[ii];
  }
  cv[iNr] = cv[0];

  GR_Disp_cv (cv, iNr+1, 5);

  return 0;

}


//========================================================================
  int MSH_test_disp2 (int *ib, int iNr, Point *pa, int att, int mode) {
//========================================================================
/// \code
/// TEST: display polygon
/// mode = 0    open polygon
/// mode = 1    closed polygon
/// \endcode

  int    i1, iSiz;
  //Point  cv[iNr+1];
  Point  *cv;


  // printf("MSH_test_disp2 %d\n",iNr);


  if(iNr < 2) return 0;

  iSiz = iNr + mode;

  cv = (Point*)UME_alloc_tmp(sizeof(Point)*iSiz);
  // cv = (Point*)_alloca(sizeof(Point)*(iNr + 1));


  for(i1=0; i1<iNr; ++i1) {
    // printf(" P[%d] ib=%d\n",i1,ib[i1]);
    // printf("       ia=%d\n",ia[ib[i1]]);
    cv[i1] = pa[ib[i1]];
  }

  if(mode > 0) cv[iNr] = cv[0];

  GR_Disp_cv (cv, iSiz, att);

  return 0;

}



//========================================================================
  int MSH_test_disp3 (int *iba, int iNr, Point2 *pa) {
//========================================================================
/// TEST: display polygon

  int    i1, ii;
  Point2 *cv;


  printf("MSH_test_disp3 %d\n",iNr);

  cv = (Point2*)UME_alloc_tmp(sizeof(Point2)*(iNr + 1));
  // cv = (Point2*)_alloca(sizeof(Point2)*(iNr + 1));


  for(i1=0; i1<iNr; ++i1) {
    // printf(" P[%d] ib=%d\n",i1,ib[i1]);
    ii = iba[i1];
    cv[i1] = pa[ii];

    // disp pointNr as text
    GR_Disp_txi2 (&cv[i1], i1, 1);
  }

  cv[iNr] = cv[0];

  GR_Disp_cv2 (cv, iNr+1, 9);

  return 0;

}


//================================================================
  int MSH_test_disp4 (Fac3 *fa, int fNr, Point2 *pa) {
//================================================================
/// TESTDISPLAY Faces

  int      i1;
  Point2   *p1, *p2, *p3, pgc;

  printf("MSH_test_disp4 %d\n",fNr);

  for(i1=0; i1<fNr; ++i1) {
    p1 = &pa[fa[i1].i1];
    p2 = &pa[fa[i1].i2];
    p3 = &pa[fa[i1].i3];
    GR_Disp_su3p2 (p1, p2, p3, 8);

    UT2D_pt_gcp_3pt (&pgc, p1, p2, p3);
    GR_Disp_txi2 (&pgc, i1, 5);

  }

  DL_Redraw ();

  return 0;

}


//================================================================
  int MSH_test_disp5 (Fac3 *fa, int fNr, Point *pa) {
//================================================================
/// TESTDISPLAY Faces

  int      i1;
  Point    *p1, *p2, *p3, pgc;
  

  printf("MSH_test_disp5 %d\n",fNr);

  for(i1=0; i1<fNr; ++i1) {
    p1 = &pa[fa[i1].i1];
    p2 = &pa[fa[i1].i2];
    p3 = &pa[fa[i1].i3];
    GR_Disp_su3pt (p1, p2, p3, 8);

    UT3D_pt_gcp_3pt (&pgc, p1, p2, p3);
    GR_Disp_txi (&pgc, i1, 5);
  }



  return 0;

}


//================================================================
  int MSH_bdel_fTab (char *mdlNam, int surNr) {
//================================================================
/// MSH_bdel_fTab              delete binary faceTableFile



  char lBuf[256];


  // sprintf(lBuf, "%sM%dA%d.msh",OS_get_tmp_dir(),mdlNr,surNr);
  // /temp/_A1.msh
  // /temp/smNam_A1.msh
  sprintf(lBuf, "%s%s_A%d.msh",OS_get_tmp_dir(),mdlNam,surNr);
    // printf("MSH_bdel_fTab |%s|\n",lBuf);

  OS_file_delete (lBuf);

  return 0;

}


//================================================================
  int MSH_pt_prjptmsh_ (Point *pto, Point *pti,
                        int surMsh,  int surPtab) {
                        // Vector *vp)
//================================================================
/// \code
/// project point > Mesh
/// 
/// RetCod: 0 OK;
///         1 point is not inside mesh
///        -1 Error in inputData
/// Output:
/// \endcode


  int       i1;
  // int       pNr, fNr;
  // Point     *pTab = NULL;
  // Fac3      *fTab = NULL;

  // MemTab(Point) pTab = MemTab_Init(sizeof(Point), Typ_PT, 10000);
  // MemTab(Fac3) fTab = MemTab_Init(sizeof(Fac3), Typ_Fac3, 10000);
  // MemTab(EdgeLine) eTab = MemTab_Init(sizeof(EdgeLine), Typ_EdgeLine, 10);
  // MemTab(int) eDat = MemTab_Init(sizeof(int), Typ_Int4, 50);
  MemTab(Point) pTab = MemTab_empty;
  MemTab(Fac3) fTab = MemTab_empty;
  MemTab(EdgeLine) eTab = MemTab_empty;
  MemTab(int) eDat = MemTab_empty;

  MemTab_ini (&pTab, sizeof(Point), Typ_PT, 10000);
  MemTab_ini (&fTab, sizeof(Fac3), Typ_Fac3, 10000);
  MemTab_ini (&eTab, sizeof(EdgeLine), Typ_EdgeLine, 10);
  MemTab_ini (&eDat, sizeof(int), Typ_Int4, 50);



  // load PointFile  (write: lxml_read) pTab=malloc !
  i1 = MSH_bload_pTab (&pTab, WC_modact, surPtab);
  if(i1 < 0) {TX_Error("TSU_DrawSurMsh E001"); return -1;}
    printf(" pNr=%d\n",pTab.rNr);


  // load MeshFile    fTab=malloc !
  i1 = MSH_bload_fTab (&fTab, &eTab, &eDat, WC_modact, surMsh);
  if(i1 < 0) {TX_Error("TSU_DrawSurMsh E002"); return -1;}
    printf(" fNr=%d\n",fTab.rNr);


  // project point > Mesh
  i1 = MSH_pt_prjptmsh1 (pto, pti, fTab.data, fTab.rNr, pTab.data, pTab.rNr);
    // GR_Disp_pt (&p1, SYM_STAR_S, 2);

  // if(pTab) free(pTab);
  // if(fTab) free(fTab);
  MemTab_free (&eDat);
  MemTab_free (&eTab);
  MemTab_free (&fTab);
  MemTab_free (&pTab);


    // UT3D_stru_dump (Typ_PT, pto, "ex MSH_pt_prjptmsh_ %d",i1);


  return i1;

}


//================================================================
  int MSH_pt_prjptmsh1 (Point *pto, Point *pti,
                        Fac3 *fa, int fNr,
                        Point *pa, int pNr) {
                        // Vector *vp)
//================================================================
/// \code
/// project point > Mesh
/// 
/// RetCod: 0 OK;
///         1 point is not inside mesh
///        -1 Error in inputData
/// Output:
///   pto     NULL=point outside mesh
/// 
/// - transform points > 2D;
/// - check if point is in Triangle or on its boundary
/// - get Z-coord of point on 2D-Triangle
/// \endcode


  int     irc, i1, ii1, ii2, ii3;
  int     p2Nr;
  double  d1;
  Point2  *p2a, p2i;
  Point   *p1, *p2, *p3;
  Vector  vcn, *vp;
  Plane   pl1;


  // printf("MSH_pt_prjptmsh1 %d %d\n",fNr,pNr);


  vp = (Vector*)&UT3D_VECTOR_Z;  // projectionVector


  // prepare space for 2D-points
  p2a = (Point2*)memspc501;
  i1 = sizeof(memspc501) / sizeof(Point2);  // 16 byte
  if(i1 < pNr) {TX_Error("MSH_pt_prjptmsh1 E001"); return -1;}


  // transport all points > 2D (pa -> p2a)
  // for XY-Plane copy points ..
  MSH_ptn2_ptn3 (p2a, &p2Nr, pa, pNr);

  p2i = UT2D_pt_pt3 (pti);
    // UT3D_stru_dump (Typ_PT2, &p2i, "p2i=");


  // check if point is in Triangle or on its boundary
  // loop tru triangles;
  for(i1=0; i1<fNr; ++i1) {
    ii1 = fa[i1].i1;
    ii2 = fa[i1].i2;
    ii3 = fa[i1].i3;

    // test if point ip1 is inside Face ii1-ii2-ii3
    irc = UT2D_ck_pt_in_tria (&p2a[ii1], &p2a[ii2], &p2a[ii3], &p2i);
    if(irc > 0) continue;    // outside ..
    goto L_found;
  }

  // point is ouside mesh
    printf("MSH_pt_prjptmsh1 pt is outside ..\n");
  return 1;


  L_found:  //irc: 0=inside, -1=on i1-i2, -2=on i2-i3, -3=on i3-i1
    // printf(" f=%d irc=%d %d %d %d\n",i1,irc,ii1,ii2,ii3);
  p1 = &pa[ii1];
  p2 = &pa[ii2];
  p3 = &pa[ii3];
    // UT3D_stru_dump (Typ_PT, p1, "p1=");
    // UT3D_stru_dump (Typ_PT, p2, "p2=");
    // UT3D_stru_dump (Typ_PT, p3, "p3=");
    // UT3D_stru_dump (Typ_VC, &pl1.vz, "vz=");


// OPE: USE MSH_pt_prjptfac
  // get Z-coord of point on 2D-Triangle.
  // see UT3D_pl1_tria
  UT3D_vc_perp3pt (&pl1.vz, p1, p2, p3); // normalVector of triangle
  UT3D_vc_setLength (&pl1.vz, &pl1.vz, 1.);
    // UT3D_stru_dump (Typ_VC, &pl1.vz, "pl1.vz:");
  UT3D_pl_ptpl (&pl1, p1);  // create pl1.p
    // UT3D_stru_dump (Typ_PLN, &pl1, "pl1:");
  irc = UT3D_ptDi_intptvcpln (pto, &d1, &pl1, pti, vp);

  if(irc == 0) {   // plane parallel; Z=zVal of gravityCenterpoint)
    *pto = *pti;
    pto->z = (p1->z + p2->z + p3->z) / 3.;
  }

  return 0;

}


//================================================================
  int MSH_eTab_fix (MemTab(EdgeLine) *eTab, MemTab(int) *eDat) {
//================================================================
/// \code
/// MSH_eTab_fix      recover EdgeLine-pointers ia
/// (nach relocation) die pointers neu aufbauen
/// \endcode

  int    i1, ii;

  // get ii = actual used size of DatEtab
  ii = 0;
  for(i1=0; i1<eTab->rNr; ++i1) {
    eTab->data[i1].ia = &eDat->data[ii];
      // printf(" _fix eTab[%d] = %d\n",i1,ii);
    ii += eTab->data[i1].iNr;
  }

  return 0;

}


//================================================================
  int MSH_EL_get_tab (int **iTab, int *iNr,
                      MemTab(EdgeLine) *eTab, int typ) {
//================================================================
/// \code
/// get a pointer to the integer-list of EdgeLine with typ=<typ>
/// MSH_EL_get_tab   get EdgeLineData from EdgeLineTyp.
/// \endcode

  int   i1;

  // printf("MSH_EL_get_tab %d\n",eTab->rNr);



  for(i1=0; i1<eTab->rNr; ++i1) {
    if(eTab->data[i1].typ == typ) {
      *iTab = eTab->data[i1].ia;
      *iNr  = eTab->data[i1].iNr;
      goto L_9;
    }
  }
  i1 = -1;
  *iTab = NULL;


  L_9:
  // printf("ex MSH_EL_get_tab irc=%d iNr=%d typ=%d\n",i1,*iNr,typ);

  return i1;

}


//================================================================
  int MSH_npt_prjcvmsh_ (Point **paOut, int *paNr,
                         Point *pPlg, int plgNr,
                         MemTab(Fac3) *fTab,
                         MemTab(Point) *pTab,
                         MemTab(EdgeLine) *eTab,
                         MemTab(int) *eDat) {
                         // Vector *vp)
//================================================================
// project curve > Mesh

// Input:
//   typ     Typ of curve
//   ind     DB-index of curve
// Output:
//   paOut   polygonpoints                  memspc501 !
//   pNr     nr of points in pa
// RetCod: 0 OK;
//         1 point is not inside mesh
//        -1 Error in inputData

// Internal:
// iStat = 0;   // Endpoint was inside active Face iFac.
// iStat = 1;   // Endpoint was outside Edge ip1-ip2;
// iStat = 2;   // Endpoint was on Edge ip1-ip2;
// iStat = 3;   // Endpoint was on GisPoint ip1;


  int        i1, i2, i3, i4, i5, i6, plgSiz, outSiz, outNr,
             iSeg, iFac, fNr, pNr,
             ii1, ii2, ii3,
             ip1, ip2, iStat;
  double     d1, d2;
  Point      *pOut, *pa, pt1, pt2;
  Point      *pf1, *pf2, *pf3, *pe1, *pe2, pss;
  Point2     p21, p22, *p2s1, *p2s2, *p2e1, *p2e2;
  Fac3       *fa;
  EdgeLine   *ela;


  // printf("MSH_npt_prjcvmsh_  %d\n",plgNr);
  // for(i1=0;i1<plgNr;++i1) UT3D_stru_dump (Typ_PT,&pPlg[i1],"p[%d]=",i1);
  // MSH_dump_fTab (fTab->data, fTab->rNr);



  fa  = fTab->data;
  fNr = fTab->rNr;

  pa  = pTab->data;
  pNr = fTab->rNr;


  // get memSpace for the output-points
  pOut = (Point*)memspc501;
  outSiz = sizeof(memspc501) / sizeof(Point);
  outNr = 0;


  iSeg  = -1;
  iStat = -1;   // Endpoint unknown (first start);

  // loop tru segments;  seg=pPlg[iSeg]-pPlg[iSeg+1]




  //----------------------------------------------------------------
  // activat next polygonSegment pe1-pe2;
  //----------------------------------------------------------------
  L_nxt_seg:
  ++iSeg;
  if(iSeg >= (plgNr - 1)) goto L_done;
    // printf(" iSeg=%d iStat=%d\n",iSeg,iStat);

  pe1 = &pPlg[iSeg];
  pe2 = &pPlg[iSeg+1];




  //----------------------------------------------------------------
  // find face to connect.
  if(iStat != -1) goto L_sta_2;

  // find face of segmentStartpoint pe1
  i1 = MSH_if_find_ptmsh (&iFac, pe1, fa, fNr, pa, pNr);


  // 1. Segment: startpoint pe1 is inside face iFac; save point.
  if(i1 == 0) {
    if(outNr == 0) {
      // project startpoint > face
      UTRI_pt_prjZpt3pt (&pOut[0], &pPlg[iSeg],
                       &pa[fa[iFac].i1], &pa[fa[iFac].i2], &pa[fa[iFac].i3]);
      ++outNr;
    }
    goto L_int_f;
  }


  // startpoint is equal to facepoint
  if(i1 < -3) {
    if(i1 == -4) {              // pe1 = i1
      ii1 = fa[iFac].i1;

    } else if(i1 == -5) {       // pe1 = i2
      ii1 = fa[iFac].i2;

    } else if(i1 == -6) {       // pe1 = i3
      ii1 = fa[iFac].i3;

    }
    // add point ii1
    if(outNr == 0) {
      pOut[0] = pa[ii1];
      outNr = 1;
    }
    // find face with LineSegment ii1-pe2 inside.
    // (segment starts at facpoint ii1)
    MSH_if_findSegP (&iFac, &i2, ii1, -1, pe2, fa, fNr, pa);
    if(iFac >= 0) goto L_int_f;
    TX_Print("MSH_npt_prjcvmsh_ E055");
    return -1;
  }




  // startpoint is on edge; ..
  if(i1 < 0) {
    if(i1 == -1) {              // pe1 on edge i1-i2
      ii1 = fa[iFac].i1;
      ii2 = fa[iFac].i2;

    } else if(i1 == -2) {       // pe1 on edge i2-i3
      ii1 = fa[iFac].i2;
      ii2 = fa[iFac].i3;

    } else if(i1 == -3) {       // pe1 on edge i3-i1
      ii1 = fa[iFac].i3;
      ii2 = fa[iFac].i1;
    }
    p21 = UT2D_pt_pt3 (pe1);
    goto L_sta_5;
  }



  // segmentStartpoint is not inside face: test if segment crosses bounds ..
  // loop tru all boundarySegments; intersect with active polygonSegment
  p2e1 = (Point2*)pe1;
  p2e2 = (Point2*)pe2;
  for(i2=0; i2<eTab->rNr; ++i2) {
    if((eTab->data[i2].typ != TYP_EDGLN_IB)  &&
       (eTab->data[i2].typ != TYP_EDGLN_OB)) continue;
    ela = &eTab->data[i2];
    i4 = ela->iNr - 1;
    for(i3=0; i3<ela->iNr; ++i3) {  // loop tru boundary ela
      ii1 = ela->ia[i4];
      ii2 = ela->ia[i3];
      p2s1 = (Point2*)&pa[ii1];
      p2s2 = (Point2*)&pa[ii2];
        // printf("  bnd-seg[%d]=%d,%d\n",i3,ii1,ii2);
      i5 = UT2D_pt_int4pt (&p21, &d1, &d2, p2e1, p2e2, p2s1, p2s2);
      // clear intersection: set intersectionPoint as new startpoint & goto
      // next segment; overlap: skip segment.
      if(i5 != 1) goto L_out_9;
      // set iFac=index of face with segment ii1-ii2
      if(eTab->data[i2].typ == TYP_EDGLN_IB) MEM_swap_int (&ii1, &ii2);
      goto L_sta_5;

      L_out_9:
      i4 = i3;
    }
  }
  TX_Print ("******* MSH_npt_prjcvmsh_ E005");
  return -1;



  //----------------------------------------------------------------
  // Last Endpoint was on Edge from points ip1-ip2
  L_sta_2:
  if(iStat != 2) goto L_sta_3;

    // find face with edge ip2-ip1
    UFA_ifac_ck_edgo (&iFac, &i2, ip2, ip1, fa, fNr);
    if(iFac  < 0) {
      // geht ev durch Loch oder Bucht; man sollte Boundaries testen ...
      iStat = -1;
      goto L_nxt_seg;
    }
    goto L_int_f;



  //----------------------------------------------------------------
  // Last Endpoint was on GisPoint ip1; find face with Line pe1-pe2
  L_sta_3:
  if(iStat != 3) goto L_sta_4;

    // find face with LineSegment ip1-pe2 inside.
    MSH_if_findSegP (&iFac, &i5, ip1, -1, pe2, fa, fNr, pa);
    if(iFac >= 0) goto L_int_f;
    TX_Print("MSH_npt_prjcvmsh_ E046");
    return -1;




  //----------------------------------------------------------------
  L_sta_4:
    TX_Print ("******* MSH_npt_prjcvmsh_ E066");
    return -1;



  //----------------------------------------------------------------
  L_sta_5:
    // find face for oriented edge ii1,ii2 and point p21 on it.
    UFA_ifac_ck_edgo (&iFac, &i5, ii1, ii2, fa, fNr);
    if(iFac < 0) {TX_Print ("******* MSH_npt_prjcvmsh_ E004"); return -1;}
    // Punkt pss interpolieren zwischen pa[ii1]-pa[ii2]
    UT3D_pt_ipl_2ptpt2 (&pss, &pa[ii1], &pa[ii2], &p21);
    // pss abspeichern
    if(outNr == 0) {
      pOut[0] = pss;
      outNr = 1;
    }
    pe1 = &pss;



  //----------------------------------------------------------------
  // get intersectionPoints Segment-Face
  L_int_f:
    // printf(" L_int_f: %d\n",iFac);
  ii1 = fa[iFac].i1;
  ii2 = fa[iFac].i2;
  ii3 = fa[iFac].i3;
    // printf(" ii1=%d ii2=%d ii3=%d\n",ii1,ii2,ii3);

  pf1 = &pa[ii1];
  pf2 = &pa[ii2];
  pf3 = &pa[ii3];

  // intersect Line pe1-pe2 with Face iFac;
  i1 = UTRI_ln_int_LnFac (&pt1, pf1, pf2, pf3, pe1, pe2);
    // GR_Disp_pt (&pt1, SYM_STAR_S, 2);


  // add point
  pOut[outNr] = pt1;
  if(outNr < outSiz) ++outNr;
  else {TX_Error("MSH_npt_prjcvmsh_ EOM1"); return -1;}


  if(i1 == 0) {    // pe2 is inside Face; add point, goto L_nxt_seg.
    goto L_nxt_seg;
  }


  if(i1 < 4) {
    // segment intersects Edge: add point; search complementary-face (iFac);
    // then continue with this segment in new face iFac.
    if(i1 == 1) {
      // UFA_ifac_ck_edgo (&iFac, &i2, ii1, ii2, fa, fNr);
      UFA_ifac_ck_edgo (&iFac, &i2, ii2, ii1, fa, fNr);

    } else if(i1 == 2) {
      // UFA_ifac_ck_edgo (&iFac, &i2, ii2, ii3, fa, fNr);
      UFA_ifac_ck_edgo (&iFac, &i2, ii3, ii2, fa, fNr);

    } else {  // i1 == 3
      // UFA_ifac_ck_edgo (&iFac, &i2, ii3, ii1, fa, fNr);
      UFA_ifac_ck_edgo (&iFac, &i2, ii1, ii3, fa, fNr);
    }
    // if endpoint is outside face: skip the rest of the segment.
    if(iFac  < 0) {iStat = -1; goto L_nxt_seg;}
    // connecting face found ..
    goto L_int_f;
  }


  if(i1 < 7) {
    // endpoint of segment lies on Edge: add point; goto next segment.
    if(i1 == 4) {           // Line ends on edge 1
      ip1 = ii1;
      ip1 = ii2;
    } else if(i1 == 5) {    // Line ends on edge 2
      ip1 = ii2;
      ip2 = ii3;
    } else if(i1 == 6) {    // Line ends on edge 3
      ip1 = ii3;
      ip2 = ii1;
    }
    iStat = 2;   // Endpoint was on Edge ip1-ip2;
    goto L_nxt_seg;
  }


  
  // Endpoint of segment goes tru Gis-Point: add point;
  // search next face with active segment;(test all faces with that point).
  // intersect next face, same segment.
  if(i1 < 10) {            // seg goes tru Gis-Point;
    if(i1 == 7) {          // seg goes tru p1
      ip1 = ii1;
    } else if(i1 == 8) {   // seg goes tru p2
      ip1 = ii2;
    } else if(i1 == 9) {   // seg goes tru p3
      ip1 = ii3;
    }
    // find face with LineSegment ip1-ps2 inside.
    MSH_if_findSegP (&iFac, &i5, ip1, -1, pe2, fa, fNr, pa);
    if(iFac >= 0) goto L_int_f;
    TX_Print("MSH_npt_prjcvmsh_ E045");
    return -1;
  }



  // Endpoint lies on GisPoint:
  if(i1 == 10) {          // pe1-pe2 ends on pf1
    ip1 = ii1;
  } else if(i1 == 11) {   // pe1-pe2 ends on pf2
    ip1 = ii2;
  } else if(i1 == 12) {   // pe1-pe2 ends on pf3
    ip1 = ii3;
  }
  iStat = 3;   // Endpoint was on GisPoint ip1;
  goto L_nxt_seg;




  //----------------------------------------------------------------
  L_done:
 
/*
    printf("ex MSH_npt_prjcvmsh_\n");
    for(i1=0;i1<outNr;++i1) {
      UT3D_stru_dump (Typ_PT, &pOut[i1], "  pOut[%d]:",i1);
      // GR_Disp_pt (&pOut[i1], SYM_STAR_S, 2);
    }
*/

  *paOut = pOut;
  *paNr = outNr;

  return 0;

}
 

//================================================================
  int MSH_msh_pt (int ip1, MemTab(Fac3) *fTab, Point2 *p2a,
                  MemTab(EdgeLine) *eTab, MemTab(int) *eDat,
                  MemTab(char) *saf) {
//================================================================
// add point to mesh; create 2 or 3 new faces.
// Function:
//   suche Face, in dem der Punkt liegt;
//   create 2 od 3 new faces.
// Input:
//   ip1     point to add to mesh (index into p2a)
//   fTab    existing faces
//   p2a     points
// Output = Anzahl new erzeugter Faces;
//             index of face in dem der Punkt liegt od pt outside face.
// RetCode:  0=normal end; -1=point is not inside faces (eg inside hole)

// Das Face suchen, in dem der Punkt liegt;
// Wenn der Punkt in einem Face liegt:
// -) das Face durch 3 neue Faces (die durch den Punkt gehen) ersetzen
// Wenn der Punkt auf einer Kante liegt:
//   das zweite Face suchen, das dieselbe Kante hat;
//   beide Faces beim Punkt in je 2 Faces zerbrechen.


  int     irc, i1, i2, ii1, ii2, ii3, if1, if2, ie1, ie2;
  Fac3    *f31;


  // printf("MSH_msh_pt %d ----------------\n",ip1);


  //----------------------------------------------------------------
  // Suche in welchem 3Eck sich Punkt ip1 befindet.
  // In keinem = auf einer Konturlinie: diesen Punkt skippen.

  for(i1=0; i1<fTab->rNr; ++i1) {
    f31 = &fTab->data[i1];
    ii1 = f31->i1;
    ii2 = f31->i2;
    ii3 = f31->i3;
      // printf("  f=%d ii1=%d ii2=%d ii3=%d\n",i1,ii1,ii2,ii3);


    // hier sollte noch eine Ausscheidung via Box gemacht werden ..

    // test if point ip1 is inside Face ii1-ii2-ii3
    irc = UT2D_ck_pt_in_tria (&p2a[ii1], &p2a[ii2], &p2a[ii3], &p2a[ip1]);
      // printf(" _pt_in_tria f=%d irc=%d\n",i1,irc);
    if(irc > 0) continue;    // outside ..

    // point alread exists: ignore this point.
    if(irc < -3) goto L_done;


    if1 = i1;

    // point ip1 lies in or on edge of face if1;
    // ignore it, if face if1 is inside InnerBound
    if(saf->rNr > if1) {
      if(saf->data[if1] == 1) {
        // printf(" skip pt %d in IB f=%d\n",ip1,if1);
        goto L_done;   // ignore point inside InnerBound
      }
    } 


    // point ip1 is on Edge
    if(irc < 0) {
      // get pointnumbers for edge <irc> on face <if1>
      UFA_2ip_fac_esn (&ii1, &ii2, irc, &fTab->data[if1]);
        // printf(" _2ip: ii1=%d ii2=%d\n",ii1,ii2);

      // check if point lies on a Breakline
      i2 = MSH_EL_ck_ln (ii1, ii2, eTab, eDat);
      // >0 = yes, point lies on a Breakline: ignore point.
      if(i2 > 0) goto L_done;

      goto L_fmod1;
    }

    // point ip1 is inside Edge i1
    goto L_fmod2;
  }

  // der Punkt liegt in keinem Face und auf keiner Kante
  // (liegt innerhalb einer inner boundary !)
    // printf ("MSH_msh_pt I001 %d %f %f\n",ip1,p2a[ip1].x,p2a[ip1].y);
    // printf(" next ip1 = %d %d\n",ipa[ip1],ip1);
    // GR_Disp_pt2 (&p2a[ip1], SYM_STAR_S, 2);
  return -1;
  // goto L_done;


  //----------------------------------------------------------------
  // point p2a[ip1] lies on boundary of face if1. Create 2 new faces.
  L_fmod1:
    // printf("L_fmod1 pt=%d fac=%d\n",ip1,if1);
    // Find all Faces mit gleicher Kante (1 oder 2)

    // find the faces (1 or 2 ) with the Edge ii1-ii2
    UFA_ifac_ck_edgu (&if1, &ie1, &if2, &ie2, ii1, ii2, fTab->data, fTab->rNr);
      // printf(" _ifac: if1=%d if2=%d\n",if1,if2);

    // Face durch 2 neue Faces (die durch den Punkt gehen) ersetzen
    if(if1 >= 0) UFA_2fac_facpt (fTab, ip1, if1, ie1);
    if(if2 >= 0) UFA_2fac_facpt (fTab, ip1, if2, ie2);

  goto L_done;



  //----------------------------------------------------------------
  // Faces; point p2a[ip1] lies inside face if1. Create 3 new faces.
  L_fmod2:
    // printf("L_fmod2 pt=%d fac=%d\n",ip1,if1);
    // Face durch 3 neue Faces (die durch den Punkt gehen) ersetzen
    UFA_3fac_facpt (fTab, ip1, if1);
  goto L_done;


  //----------------------------------------------------------------
  L_done:
    // printf("ex MSH_msh_pt %d\n",ip1);
    // MSH_dump_fTab (fTab->data, fTab->rNr);

  return 0;

}


//=============================================================================
   int MSH_EL_ck__ (Point *p1, int  *ip1, int ii,
                    MemTab(EdgeLine) *eTab, Point *pTab, int pNr, double tol) {
//=============================================================================
// MSH_EL_ck__              test new breakline
// test if seg p1-ip1 intersects other edgeline or goes tru point
// see also gisPt_ck_EL__
// Input:
//   ii        index of point p1; do not test segments with this point
// RetCod:
//   -1=noIntersect; 0=Intersection; -2=identicalPoints

// see MSH_EL_ck_EL__


  int       irc, i1, i2, *ia, iNr, ip2, ip3, ip4;
  EdgeLine  *el1;


  // printf("MSH_EL_ck__ %d ii=%d\n",*ip1,ii);
  // UT3D_stru_dump (Typ_PT, p1, "  P1");


  // test if seg p1-p2 intersects other edgeline
  // loop tru existing breaklines
  for(i1=0; i1<eTab->rNr; ++i1) {
    el1 = &eTab->data[i1];
    ia  = el1->ia;
    iNr = el1->iNr;
    for(i2=1; i2<iNr; ++i2) {
      // breaklineSeg [i2-1]-[i2]
        // printf(" test %d %d\n",ia[i2-1],ia[i2]);
      ip2 = ia[i2-1];
      if(ip2 == *ip1) continue;
      if(ip2 == ii) continue;
      ip3 = ia[i2];
      if(ip3 == *ip1) continue;
      if(ip3 == ii) continue;
      irc = MSH_EL_ck_EL_p3ip  (p1, ip1, &ip2, &ip3, pTab);
      if(irc >= 0) goto L_ERR_L;
    }

    // types TYP_EDGLN_IB and TYP_EDGLN_OB and TYP_EDGLN_AB have closing edges
    if(el1->typ != TYP_EDGLN_BL) {
        // printf(" test %d %d\n",ia[iNr-1],ia[0]);
      ip2 = ia[iNr-1];
      if(ip2 == *ip1) continue;
      if(ip2 == ii) continue;
      ip3 = ia[0];
      if(ip3 == *ip1) continue;
      if(ip3 == ii) continue;
      irc = MSH_EL_ck_EL_p3ip  (p1, ip1, &ip2, &ip3, pTab);
      if(irc >= 0) goto L_ERR_L;
    }
  }


  // test if seg p1-ip1 goes tru existing point
  // see gisPt_ck_EL__
  irc = MSH_EL_ck_pTab ((Point2*)p1, (Point2*)&pTab[*ip1], pTab, pNr, tol);
  if(irc >= 0) goto L_ERR_P;



  return -1;


  L_ERR_P:
    TX_Print("***** goes through point %d",irc);
    return 0;


  L_ERR_L:
    TX_Print("***** crossing breakline %d - %d",ip2,ip3);
    return 0;

}


//================================================================
  int MSH_EL_ck_ln (int ip1, int ip2,
                    MemTab(EdgeLine) *eTab, MemTab(int) *eDat) {
//================================================================
// check if line ip1-ip2 is a Breakline, not oriented.
// Retcode:
//   -1     line ip1-ip2 is not breakline
//    2     (TYP_EDGLN_BL) line ip1-ip2 is on BreakLine
//    3     (TYP_EDGLN_IB) line ip1-ip2 is on InnerBound
//    4     (TYP_EDGLN_OB) line ip1-ip2 is on OuterBound

  int        irc, i1, i2, i3;
  EdgeLine   *ela;


  // printf("MSH_EL_ck_ln %d %d\n",ip1,ip2);


  // loop tru Edgelines
  for(i1=0; i1<eTab->rNr; ++i1) {
    ela = &eTab->data[i1];
      // printf("eTab[%d].iNr=%d typ=%d\n",i1,ela->iNr,ela->typ);

    for(i2=0; i2<ela->iNr; ++i2) {
      // printf("    i[%d]=%d\n",i2,ela->ia[i2]);
      if(ela->ia[i2] != ip1) continue;
      // compare previous & following with ip2
      // fix i3 = previos index
      i3 = i2 + 1;
      if(i3 >= ela->iNr) i3 = 0;
      if(ela->ia[i3] == ip2) goto L_found;
      i3 = i2 - 1;
      if(i3 < 0) i3 = ela->iNr - 1;
      if(ela->ia[i3] == ip2) goto L_found;
    }
  }

  irc = -1;
  goto L_exit;


  L_found:
    irc = ela->typ;

  L_exit:
    // printf("ex MSH_EL_ck_ln %d\n",irc);

  return irc;

}
 

//================================================================
  int MSH_msh_OB (int *ibp, int *ibNr,
                  MemTab(EdgeLine) *eTab, Point2 *p2a, int p2Nr) {
//================================================================
// MSH_msh_OB           create outer boundary

  int   *iba, i1;


  // get user-defined outer boundary
  MSH_EL_get_tab (&iba, ibNr, eTab, TYP_EDGLN_OB);
  if(iba) {
    // yes, a user-defined outer boundary exists !
    // make ibp = a copy of iba (MSH_itess_pt2n deletes ibp)
    MEM_copy_tab (ibp, iba, ibNr, sizeof(int));
    goto L_exit;
  }


  // create outside-boundary into ibp
  *ibNr = p2Nr;  // size of ibp
  MSH_ibnd_pt2n (ibp, ibNr, p2a, p2Nr);



  //----------------------------------------------------------------
  L_exit:

    // printf(" ibNr=%d\n",*ibNr);
    // for(i1=0;i1 < *ibNr;++i1) printf("ibp[%d]=%d\n",i1,ibp[i1]);

  return 0;

}
 

//=============================================================================
  int MSH_msh1 (MemTab(Fac3) *fTab, MemTab(EdgeLine) *eTab, MemTab(int) *eDat,
               Point *pa, int pNr,
               Point2 *p2a, int p2Nr,
               int *ibp, int ibNr,
               int iOut) {
//=============================================================================
// MSH_msh1                create Mesh from Points & boundary
// Input:
//   pa       the 3D-table
//   pNr      nr of points in pa
//   iOut     0=do not remove outer triangles; 1=yes, remove.

// Method:
// Die Aussenkontur erzeugen = Startkontur.
// Die gesamte AussenKontur vermaschen
// Fuer alle Punkte (innen):
// -) das Face suchen, in dem der Punkt liegt;
// -) das Face durch 3 neue Faces (die durch den Punkt gehen) ersetzen
// Optimize: alle Faces mit 2 benachbarten Kanten testen;
// wenn Querkante kuerzer als gemeinsame Kante: Kante vertauschen.

#define msgNr  2000             // message if more > <msgNr> points

  int     irc, i1, i2, i3, i4;    //, fNr, fSiz;
  // int     *ipa;
  int     ip1;
  int     *ia;
  int     iNr, ie1, ie2;
  long    ld;
  double  dsl;
  char    *sap;
  Fac3    *fnb, *f31;
  MemTab(char) saf = MemTab_empty;

  // static Verts verts1;
  // static Faces faces1;
  // static Edges edges1;


  // printf("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
  // printf("MSH_msh1 %d\n",pNr);
  // MSH_dump_eTab (eTab, eDat);
  // for(i1=0; i1 < ibNr; ++i1) printf("%d ",ibp[i1]);
  // printf("ibp %d\n",ibNr);
/*
  // TEST display of boundary
  // MSH_test_disp1 (iba, ibNr, p2a, p2a);
  // return 0;
*/


  //================================================================
  // create a statusArray sap for the points and saf for the faces ..
  sap = UME_alloc_tmp(pNr);
  if(sap == NULL) {TX_Print("MSH_msh1 EOMsap"); return -1;}

  // fill statusArray sap for the points ..
  // -1 = outside; 0 = unknown; 2 = on BL; 3 = on IB; 4 = on OB
  for(i1=0; i1<p2Nr; ++i1) sap[i1] = 0;

  // fill sap with 3=IB, 4=OB
  for(i1=0; i1<ibNr; ++i1) {  // mark OB
    i2 = ibp[i1];
    if((i2 < 0)||(i2 >= p2Nr)) TX_Print("MSH_msh1 Error sap001 %d %d",i1,i2);
    else sap[i2] = TYP_EDGLN_OB;
  }

  i4 = 0;  // nr of IB's
  for(i1=0; i1<eTab->rNr; ++i1) {  // mark IB's
    if(eTab->data[i1].typ != TYP_EDGLN_IB) continue;
    ++i4;  // nr of IB's
    ia = eTab->data[i1].ia;
    for(i2=0; i2<eTab->data[i1].iNr; ++i2) {
      i3 = ia[i2];
      if((i3 < 0)||(i3 >= p2Nr)) TX_Print("MSH_msh1 Error sap002");
      else sap[i3] = TYP_EDGLN_IB;
    }
  }


  
  //================================================================
  // Create faces from OuterBound.
  L_edgl_10:

  // planare AussenKontur tesselieren (Vermaschen). Zerstoert ibp !
  fTab->rNr = 0;       // remove all faces ..
  irc = MSH_itess_pt2n (fTab, ibp, ibNr, p2a, p2Nr);
  if(irc < 0) return -1;
  if(p2Nr < 4) return 0;


    // TEST disp contour ..
    // printf(" preTess: ");
    // for(i1=0; i1 < ibNr; ++i1) printf("%d; ",ipa[ibp[i1]]);
    // printf("\n");

    // TEST: display mesh ohne Punkte
    // MSH_test_disp4 (fTab->data, fTab->rNr, p2a);
    // MSH_dump_fTab (fTab->data, fTab->rNr);
    // goto L_done;  // TEST-ONLY - SKIP the rest



  //================================================================
  // printf("XXXXXXXXXXXXXXXXX INNER-BOUNDS XXXXXXXXXXXXXX \n");
  if(pNr > msgNr) TX_Print("... add bounds ...");
  // Es gibt ein Mesh nur von der Aussenkontur.
  // Create additional faces from other points (inner points).
  // Fuer alle Punkte:
  // -) das Face suchen, in dem der Punkt liegt;
  // -) das Face durch 3 neue Faces (die durch den Punkt gehen) ersetzen
  // Optimize: alle Faces mit 2 benachbarten Kanten testen;
  // wenn Querkante kuerzer als gemeinsame Kante: Kante vertauschen.


/*
  // create a statusArray sap for the points and saf for the faces ..
  sap = UME_alloc_tmp(pNr);
  if(sap == NULL) {TX_Print("MSH_msh1 EOMsap"); return -1;}


  // fill statusArray sap for the points ..
  // -1 = outside; 0 = unknown; 2 = on BL; 3 = on IB; 4 = on OB
  for(i1=0; i1<p2Nr; ++i1) sap[i1] = 0;

  // fill sap with 3=IB, 4=OB
  for(i1=0; i1<ibNr; ++i1) {  // mark OB
    i2 = ibp[i1];
    if((i2 < 0)||(i2 >= p2Nr)) TX_Print("MSH_msh1 Error sap001 %d %d",i1,i2);
    else sap[i2] = TYP_EDGLN_OB;
  }

  i4 = 0;  // nr of IB's
  for(i1=0; i1<eTab->rNr; ++i1) {  // mark IB's
    if(eTab->data[i1].typ != TYP_EDGLN_IB) continue;
    ++i4;  // nr of IB's
    ia = eTab->data[i1].ia;
    for(i2=0; i2<eTab->data[i1].iNr; ++i2) {
      i3 = ia[i2];
      if((i3 < 0)||(i3 >= p2Nr)) TX_Print("MSH_msh1 Error sap002");
      else sap[i3] = TYP_EDGLN_IB;
    }
  }

    // testdisplay sap, iba
    MSH_dump_sap (sap, pNr);
    for(i1=0;i1<ibNr;++i1) printf(" ibp[%d]=%d;",i1,ibp[i1]);
    printf("\n");
*/


  // create a statusArray saf for the faces ..
  //  1=face is inside IB; 
  MemTab_ini (&saf, sizeof(char), Typ_Int1, pNr);




  //----------------------------------------------------------------
  // test nr of innerBounds
    // printf(" nr of innerBounds = %d\n",i4);
  if(i4 < 1) goto L_points;


  // alle Punkte der InnerBoundaries vermeshen
  for(ip1=0; ip1<p2Nr; ++ip1) {
    // skip all points except innerBoundary
    if(sap[ip1] != TYP_EDGLN_IB) continue;
    // add point to mesh; create 2 or 3 new faces.
    irc = MSH_msh_pt (ip1, fTab, p2a, eTab, eDat, &saf);
  }
  

    // TESTOUTPUTS:
    // printf("--------------------------------mesh-IB-L1\n");
    // MSH_test_disp4 (fTab->data, fTab->rNr, p2a);  // display Faces
    // MSH_test_disp_p1 (pa, pNr);   // display pointNrs
    // MSH_dump_fTab (fTab->data, fTab->rNr);
    // MSH_saf_init (&saf, fTab->rNr); goto L_fin__;        // test otimize
    // goto L_done;  // TEST-ONLY - SKIP the rest



  //----------------------------------------------------------------
  // Alle Punkte der innerBoundaries sind vermesht.
  // Von den InnerBoundaries Edges erzeugen.
  // Alle faces inside InnerBoundaries suchen und markieren.

  // get space for fTab->rNr chars in saf and init saf
  MSH_saf_init (&saf, fTab->rNr);

  
  // loop tru IB's (one IB = one hole)
  for(i1=0; i1<eTab->rNr; ++i1) {
    // skip all but IB's
    if(eTab->data[i1].typ != TYP_EDGLN_IB) continue;
    ia  = eTab->data[i1].ia;
    iNr = eTab->data[i1].iNr;
    i3 = iNr - 1;
    // loop tru segments of a IB (one segment = a single line)
    for(i2=0; i2<iNr; ++i2) {
      // get next edge -> ie1-ie2
      ie1 = ia[i2];
      ie2 = ia[i2 + 1];
      if(i2 < i3) ie2 = ia[i2 + 1];
      else        ie2 = ia[0];
      // work breakline ie1-ie2 (create faces)
      irc = MSH_fTab_BL__ (ie1, ie2, pa, p2a, p2Nr, fTab, &saf, sap);
      if(irc < 0) return -1;
      // if(irc > 0) MSH_msh_actPt ();//remesh points marked 16 (MSH_msh_sap16 ?)
    

        // if(i2 > 1) break;  // TESTABBRUCH
      
    }
  }


    // TESTOUTPUTS:
    // goto L_done;  // TEST-ONLY - SKIP the rest



  // delete faces inside InnerBoundaries
  MSH_delFac_IB__ (fTab, eTab, &saf);


/*
    // // TESTOUTPUTS:
    printf("--------- nach MSH_fTab_BL__ \n");
    for(i1=0;i1<saf.rNr;++i1) printf(" saf[%d]=%d;",i1,saf.data[i1]);
    printf("\n");
    MSH_test_disp4 (fTab->data, fTab->rNr, p2a);    // display Faces
    MSH_test_disp_p1 (pa, pNr);   // display pointNrs
*/
    // MSH_dump_fTab (fTab->data, fTab->rNr);
    // MSH_dump_saf (&saf);
    // goto L_done;  // TEST-ONLY - SKIP the rest



  //================================================================
  // alle uebrigen Punkte vermeshen
  L_points:
    // printf("GGGGGGGGGGGGGGGGG GIS-POINTS GGGGGGGGGGGGGGGGG\n");
  if(pNr > msgNr) {
    UI_winTX_rmLast ();
    TX_Print("meshing - add points - please wait ..");
   }

  for(ip1=0; ip1<p2Nr; ++ip1) {
    // skip points of boundary
    if(sap[ip1] != 0) continue;
    // add point to mesh; create 2 or 3 new faces.
    irc = MSH_msh_pt (ip1, fTab, p2a, eTab, eDat, &saf);
      // if(ip1 == 3) break;   // TESTABBRUCH
    if(irc < 0) sap[ip1] = -1;
  }
  
    // TESTOUTPUTS:
    // MSH_dump_fTab (fTab->data, fTab->rNr);
    // MSH_test_disp4 (fTab->data, fTab->rNr, p2a);    // display Faces
    // MSH_test_disp_p1 (pa, pNr);   // display pointNrs
    // goto L_done;  // TEST-ONLY - SKIP the rest



  //================================================================
  // BreakLines abarbeiten
  // Von den BreakLines in eTab einzelne Edges erzeugen.
  // saf vergroessern.
    // goto L_fin__;  // TEST-SKIP Breaklines ..
  // printf("XXXXXXXXXXXXXXXXX BREAKLINES XXXXXXXXXXXXXX \n");
  if(pNr > msgNr) {
    UI_winTX_rmLast ();
    TX_Print("meshing - add breaklines - please wait ..");
  }

  // get (more) space for fTab->rNr chars in saf and init saf to 0
  MSH_saf_init (&saf, fTab->rNr);
    // printf(" sizeof(saf) = fTab->rNr\n");



  // loop tru BL's
  for(i1=0; i1<eTab->rNr; ++i1) {
    // if(i1 != 1) continue; // TEST ONLY
    // skip all but IB's
    if(eTab->data[i1].typ != TYP_EDGLN_BL) continue;
    ia  = eTab->data[i1].ia;
    iNr = eTab->data[i1].iNr - 1;
    // loop tru points of a BL
    for(i2=0; i2<iNr; ++i2) {
      // get next edge -> ie1-ie2
      ie1 = ia[i2];
      ie2 = ia[i2 + 1];
      // work breakline (create faces); set saf[iFac]=2 if face has breakline.
      irc = MSH_fTab_BL__ (ie1, ie2, pa, p2a, p2Nr, fTab, &saf, sap);
      if(irc < 0) return -1;
      if(irc > 0) {
        // remesh all points marked 16
        MSH_msh_sap16 (fTab, p2a, p2Nr, eTab, eDat, &saf, sap);
      }
    }
  }


    // TESTAUSGABEN
    // MSH_dump_sap (sap, pNr);
    // MSH_dump_fTab (fTab->data, fTab->rNr);
    // goto L_done;  // TEST-ONLY - SKIP the rest




  //================================================================
  // Das Mesh ist fertig.
  // Optimize
  L_fin__:
    // goto L_done;  // TEST-ONLY - SKIP OPTIMIZE
  // printf("OOOOOOOOOOOOOOOOO OPTIMIZE OOOOOOOOOOOOOO \n");
  if(pNr > msgNr) {
    UI_winTX_rmLast ();
    TX_Print("meshing - optimize - please wait ..");
  }
    // printf("fNr=%d safNr=%d \n",fTab->rNr,saf.rNr);


    // TEST
    // saf.data[9] = 2;
    // saf.data[11] = 2;
    // TEST display 2D-Mesh, noch nicht optimiert
    // MSH_test_disp4 (fa, fNr, p2a);

  // prepare space for Neigbourfaces - fnb
  // 3+Int[fNr]; same size as Faces (fa).
  // fnb = (Fac3*)memspc101;
  // i1 = sizeof(memspc101) / sizeof(Fac3);
  // if(i1 < fNr) {TX_Error("MSH_msh1 E080"); return -1;}
  fnb = (Fac3*)UME_alloc_tmp(fTab->rNr * sizeof(Fac3));
  if(fnb == NULL) {TX_Error("MSH_msh1 E080"); return -1;}


  // Zu den Faces eine Parallelstructur fnb aufbauen,
  // in denen die NachbarFacNrs gespeichert sind.
  // (Fac3*)fnb: i1 ist der Index zum Nachbarface der Kante 1
  //    (Kante 1 geht von Punkt 1 zu Punkt 2) usw.
  //    Wert -1: kein Nachbarface existent (edge = boundary).
  UFA_fnb_init__ (fnb, fTab->data, fTab->rNr);
    // goto L_done;  // TEST-ONLY - SKIP the rest


  // Bei den NachbarFacNrs FaceNr -2, wenn common Edge eine BreakLine is
  UFA_fnb_init_el (eTab, fnb, fTab->data, fTab->rNr);


    // TEST: disp faces & neighbours
    // MSH_test_disp4 (fa, fNr, p2a);
    // for(i1=0; i1<fTab->rNr; ++i1)
      // UFA_dump1 (&fTab->data[i1], &fnb[i1], "f%d:",i1);
    // goto L_done;  // TEST-ONLY - SKIP OPTIMIZE


  //----------------------------------------------------------------
  // Optimize: jeweils 2 Faces mit gemeinsamer Kante testen;
  // wenn Querkante kuerzer als gemeinsame Kante: Kante vertauschen.
  i2 = UT_INT_MAX;
  i3 = 0;
  L_fin_o:
  i1 = UFA_opt__(fTab->data, fnb, fTab->rNr, p2a, eTab, saf.data);
  if(i1 < 3) goto L_msh_len__;
  if(i1 < i2) {
    i2 = i1;
    i3 = 0;
    goto L_fin_o;
  }
  ++i3;
  if(i3 < 15) goto L_fin_o;    // 5 zu klein f Westtrasse ..




  //----------------------------------------------------------------
  L_msh_len__:
  //  remove all faces exceeding the max triangleSideLength
  // UFA_dump_fnb (fTab->data, fnb, fTab->rNr);


  // skip this if user-defined outerBound exists ..
  i1 = MSH_eTab_findRecTyp (eTab, TYP_EDGLN_OB);
  if(i1 >= 0) goto L_done;


  // skip this if maxSideLength is off -1=off; 0=auto; else userValue
  if(iOut != 1) goto L_done;
  

  // compute size of whole area (from outer boundary)
  MSH_msh_siz (&dsl, eTab, pa);
    // printf(" rNr=%d dsl=%f\n",fTab->rNr,dsl);


  // compute dsl = max triangleSideLength; default depends on nr of faces
  if       (fTab->rNr < 12) {    goto L_done;
  } else if(fTab->rNr < 50) {    dsl /= 2.;
  } else if(fTab->rNr < 100) {   dsl /= 4.;
  } else if(fTab->rNr < 1000) {  dsl /= 8.;
  } else if(fTab->rNr < 10000) { dsl /= 10.;
  } else {                       dsl /= 16.;
  }


  MSH_saf_init (&saf, fTab->rNr);


  // test faces; set sap[]=1 for faces to remove
  L_msh_len_redo:
  dsl *= 1.5;
  MSH_msh_len_ck1 (dsl, fTab, eTab, p2a, &saf, fnb);

/*
  // test for isolated points

  // isolated points exist; redo !
  if(..) {
    ++i3; 
    if(i3 < 15) goto L_msh_len_redo;
  }
*/


  // remove faces
  MSH_delFac__ (fTab, &saf);




  //----------------------------------------------------------------
  // fertig.
  L_done:
    // printf(" L_done: fNr = %d %d\n",fTab->rNr,saf.rNr);



  MemTab_free (&saf);

/*
  // build Mesh
  verts1.pTab = pa;
  verts1.pNr  = pNr;
  ms1->p      = &verts1;

  faces1.fTab = fTab->data;
  faces1.fNr  = fTab->rNr;
  faces1.fTyp = 5;
  ms1->f      = &faces1;

  ms1->e      = NULL;
  ms1->mTyp   = 0;  // open mesh
*/

    // TESTDISPLAY
    // UT3D_stru_dump (Typ_SURMSH, ms1, "");
    // for(i1=0;i1<fNr;++i1) UT3D_stru_dump (Typ_Fac3, &fa[i1], "fa[%d]",i1);
    // MSH_test_disp5 (fa, fNr, pa);

  if(pNr > msgNr) {
    UI_winTX_rmLast ();
    TX_Print("... mesh done ...");
  }

  return 0;

}


//================================================================
  int MSH_saf_init (MemTab(char) *saf, int iSiz) {
//================================================================
// get (more) space for fTab->rNr chars in saf and init saf to 0

  int     irc, i1;
  long    ld;


  // get (more) space for fTab->rNr chars in saf
  irc = MemTab_check (saf, &ld, iSiz);
  if(irc < 0) return irc;
  saf->rNr = iSiz;                   // occupy
                     
  // init saf
  for(i1=0; i1<iSiz; ++i1) saf->data[i1] = 0;


  return 0;

}


//================================================================
  int MSH_msh_sap16 (MemTab(Fac3) *fTab, Point2 *p2a, int p2Nr,
                     MemTab(EdgeLine) *eTab, MemTab(int) *eDat,
                     MemTab(char) *saf, char *sap) {
//================================================================
// remesh all points marked 16 in sap
// Reset sap to 0 !

  int      ip1, irc;

  for(ip1=0; ip1<p2Nr; ++ip1) {
    // skip points != 16
    if(sap[ip1] != 16) continue;
    // add point to mesh; create 2 or 3 new faces.
    irc = MSH_msh_pt (ip1, fTab, p2a, eTab, eDat, saf);
      // if(ip1 == 3) break;   // TESTABBRUCH
    if(irc < 0) sap[ip1] = -1;
    else        sap[ip1] = 0;
  }



  return 0;

}


//================================================================
  int MSH_delFac_IB__ (MemTab(Fac3) *fTab,
                       MemTab(EdgeLine) *eTab,
                       MemTab(char) *saf) {
//================================================================
// delete faces inside InnerBoundaries

// remove all faces inside innerBoundaries.
// loop tru IB's, test each segment. Get the face (oriented = inside).
//   Test both edges of each face of the segments of the innerBoundaries.
//      Get the neighbourface; test its both opposite edges. recurse.
//   mark faces for delete (saf[] = 1)

// die an den 2 opposite edges anliegenden Faces holen, testen:
// wenn edge auf der IB-Kontur liegt: nix tun;
// else das anliegende face suchen und testen usw (goto L_nxt_fac;).

// Innerhalb IB darf kein Punkt ein face erzeugen;
// das geht auch nicht, wenn man vorher die innerFaces entfernt.



  int        i1, i2, i3, *ia, iNr, ip1, ip2;
  int        if1, esn1, fNr;
  char       *sTab;
  Fac3       *fa, *fnb;


  // printf("---------------------- MSH_delFac_IB__ ------------ \n");
  // printf(" eTab->rNr=%d\n",eTab->rNr);
    // MSH_dump_fTab (fTab->data, fTab->rNr);
    // MSH_dump_saf (saf);



  fa  = fTab->data;
  fNr = fTab->rNr;

  sTab = saf->data;


  fnb = (Fac3*)UME_alloc_tmp(fTab->rNr * sizeof(Fac3));
  if(fnb == NULL) {TX_Error("MSH_delFac_IB__ EOF-1"); return -1;}


  // get partnerfaces for all 
  UFA_fnb_init__ (fnb, fa, fNr);




  // loop tru IB's (one IB = one hole)
  for(i1=0; i1<eTab->rNr; ++i1) {
    // skip all but IB's
    if(eTab->data[i1].typ != TYP_EDGLN_IB) continue;
    ia  = eTab->data[i1].ia;
    iNr = eTab->data[i1].iNr;
    i3 = iNr - 1;
      // printf(" nxt IB %d iNr=%d\n",i1,iNr);



    // loop tru segments of a IB (one segment = a single line)
    for(i2=0; i2<iNr; ++i2) {
      // get next edge -> ie1-ie2
      ip1 = ia[i2];
      ip2 = ia[i2 + 1];
      if(i2 < i3) ip2 = ia[i2 + 1];
      else        ip2 = ia[0];
        // printf(" OB[%d] nxt edge %d %d\n",i2,ip1,ip2);

      // find face with oriented edge ie1-ie2 (bound CCW)
      UFA_ifac_ck_edgo (&if1, &esn1, ip1, ip2, fa, fNr);
        // printf(" if1=%d esn1=%d\n",if1,esn1);

      // if this face already deleted: done ..
      if(sTab[if1] == 1) continue;

      // add face if1 to delete-list
      // saf->data[if1] = 1;
      sTab[if1] = 1;

      // rekursiv testen ob die beiden nächsten Edges an faces innerhalb
      // der Boundary grenzen; wenn ja: Faces löschen und weitersuchen ..
      // get next edge on face;
      esn1 = UFA_esn_nxt (esn1);    // get next edge
      MSH_delFac_IB_f (if1, esn1, fa, fnb, sTab, fNr, ia, iNr);

      // get next edge on face;
      esn1 = UFA_esn_nxt (esn1);    // get next edge
      MSH_delFac_IB_f (if1, esn1, fa, fnb, sTab, fNr, ia, iNr);

    }

  }





  //----------------------------------------------------------------
  // delete faces
    // MSH_dump_saf (saf);
  MSH_delFac__ (fTab, saf);


  return 0;

}


//================================================================
  int MSH_delFac_IB_f (int iFac, int iEdg,
                       Fac3 *fa, Fac3 *fnb, char *sTab, int fNr,
                       int *iba, int ibNr) {
//================================================================
// test if edge ip1-ip2 is on boundary IB.
// Input:
//   iFac     face to test
//   iEdg     esn to test
//   iba      inner boundary
//   ibNr     nr of point of iba


  int    irc, ip1, ip2, nbFac, esn1;


  // get pointNrs of edge iEdg on face iFac
  UFA_2ip_fac_esn (&ip1, &ip2, iEdg, &fa[iFac]);


  // printf("MSH_delFac_IB_f %d %d ip %d %d\n",iFac,iEdg,ip1,ip2);


  // test if edge ip1-ip2 is on IB; 0=yes, -1=no.
  irc = UFA_ck_2ip_ipa (ip1, ip2, iba, ibNr);
  if(irc == 0) return -1;


  // this edge is inside boundary and not on boundary.
  // get neighbourFace of iFac, iEdg.
  nbFac = UFA_inbFac_esn (iFac, iEdg, fnb);
  if(nbFac < 0) {  // grober Fehler - eine Aussenkante !
    printf("***** MSH_delFac_IB_f I001 %d %d\n",ip1,ip2);
    return -1;
  }

  if(sTab[nbFac] == 1) return 0;   // already deleted ..


  // delete this face.
  sTab[nbFac] = 1;
    // printf(" markdelete %d\n",nbFac);


  // which esn in nbFac ?
  esn1 = UFA_esn_2ip (ip2, fa, nbFac);


  // get next edge & test (Rekursion!)
  esn1 = UFA_esn_nxt (esn1);
  MSH_delFac_IB_f (nbFac, esn1, fa, fnb, sTab, fNr, iba, ibNr);


  // get next edge & test (Rekursion!)
  esn1 = UFA_esn_nxt (esn1);
  MSH_delFac_IB_f (nbFac, esn1, fa, fnb, sTab, fNr, iba, ibNr);


  return 0;

}


//================================================================
  int MSH_itess_pt2n (MemTab(Fac3) *fTab,
                       int *iba, int ibNr,
                       Point2 *p2a, int p2Nr) {
//================================================================
// MSH_itess_pt2n                  planare Kontur tesselieren
//   (nur die Aussenkontur komplett tesselieren, keine innenliegenden Punkte
//    berücksichtgen)
// ACHTUNG: iba WIRD ZERSTOERT !
// Funktionsweise:
// Fuer je 2 Kanten mit SpitzWinkel die Laenge fuer Abkuerzung merken.
//  - fuer gesamte Kontur.
//  Die beiden Kanten mit der kuerzesten Strecke suchen:
//    ein Face bilden,
//    die beiden Kanten durch eine Kante ersetzen (den Konturpunkt loeschen)

// Input:
//   fNr    size of fa
//   p2a    2D-PunkteTabelle
//   iba    2D-Kontur (indices in die 2D-PunkteTabelle); not closed !
//   ipa    pointers into 3D-pointTable; used only for Information
// Output:
//   fa     Faces (indices in die 2D-PunkteTabelle als *Fac3)
//   fNr    nr of faces
//   rc     0=OK;  -1=Err.

  int      i1, ii1, ii2, ii3, ie1, ie2, ie3, iMin;
  double   lMin, d1;


  // printf("MSH_itess_pt2n %d %d %d\n",fTab->rNr,ibNr,p2Nr);
  // for(i1=0; i1<ibNr; ++i1) printf(" iba[%d]=%d\n",i1,iba[i1]);
  // MSH_test_disp3 (iba, ibNr, p2a);
// return -1;

  // fTab->rNr = 0;


  //----------------------------------------------------------------
  // OutsideContour 3 points only: nothing to do ..
  if(ibNr < 4) {
    if(ibNr < 3) {TX_Print("MSH_itess_pt2n E001"); return -1;}
    UFA_add_fac (fTab, iba[0], iba[1], iba[2]);
    // MSH_Ftab_add (fa, fNr, fSiz, iba[0], iba[1], iba[2]);
    goto L_exit;
  }



  //----------------------------------------------------------------
  // ACHTUNG: der Algorithmus funktioniert erst ab 5 Kanten !
  if(ibNr < 5) goto L_finish;



  //----------------------------------------------------------------
  L_nxt__:
  ie1 = 0;
  lMin = UT_VAL_MAX;
  iMin = -1;


  // loop tru boundary; get length of
  // ie1, ie2, ie3 are the next 3 points = 2 edges.
  // lse = length short edge; lse[0] = length p2a[0]-p2a[2];
  //     lse[1] = length p2a[1]-p2a[3];
  L_nxt_e2:
  ie2 = ie1 + 1;
  if(ie2 >= ibNr) ie2 = 0;

  ie3 = ie2 + 1;
  if(ie3 >= ibNr) ie3 = 0;

  ii1 = iba[ie1];
  ii2 = iba[ie2];
  ii3 = iba[ie3];

    // printf(" ii1/2/3 = %d %d %d ie1/2/3 = %d %d %d\n",ii1,ii2,ii3,ie1,ie2,ie3);


  // check if edges ie1-ie2-ie3 go left or right
  i1 = UT2D_sid_3pt (&p2a[ii1], &p2a[ii2], &p2a[ii3]);
    // printf(" ii1=%d ii2=%d ii3=%d side=%d\n",ii1,ii2,ii3,i1);
  // contour is CCW; 1=Ok to remove; -1=dont touch; 0=linear !
  if(i1 <= 0) goto L_cont;  // L_nxt_e2;


  // test length ii1-ii3
  UT2D_lenq_2pt (&d1, &p2a[ii1], &p2a[ii3]);
  if(d1 < lMin) {
    lMin = d1;
    iMin = ie1;
  }

  L_cont:
  ++ie1;
  if(ie1 < ibNr) goto L_nxt_e2;


  //----------------------------------------------------------------
  L_del_nxt:
  ie1 = iMin;

  ie2 = ie1 + 1;
  if(ie2 >= ibNr) ie2 = 0;

  ie3 = ie2 + 1;
  if(ie3 >= ibNr) ie3 = 0;
    // printf(" CreFac ie1/2/3 %d %d %d / %d %d %d\n",ie1,ie2,ie3,
              // iba[ie1], iba[ie2], iba[ie3]);
    // printf(" minEdge=%d %d %d\n",ie1,ie2,ie3);
    // GR_Disp_ln2 (&p2a[iba[ie1]], &p2a[iba[ie3]], 8);

  // add face ie1-ie2-ie3.
    // GR_Disp_su3p2 (&p2a[iba[ie1]], &p2a[iba[ie2]], &p2a[iba[ie3]], 10);
  UFA_add_fac (fTab, iba[ie1], iba[ie2], iba[ie3]);
  // MSH_Ftab_add (fa, fNr, fSiz, iba[ie1], iba[ie2], iba[ie3]);


  // remove point ie2 from contour
    // printf(" del pt %d\n",ie2);
  i1 = MEM_del_IndRec (&ibNr, iba, ie2);
  if(i1 < 0) return -1;



  if(ibNr > 4) goto L_nxt__;



 //----------------------------------------------------------------
  // ACHTUNG: der Algorithmus funktioniert erst ab 5 Kanten !
  L_finish:
  // Test winkel P0/P1/P2; if kleiner 180:
  //     fac1 = p0 p1 p2;   fac2 = p0 p2 p3
  //   else
  //     fac1 = p0 p1 p3;   fac2 = p1 p2 p3

  ii1 = iba[0];
  ii2 = iba[1];
  ii3 = iba[2];
  i1 = UT2D_sid_3pt (&p2a[ii3], &p2a[ii1], &p2a[ii2]);
    // printf(" 4 points: i1=%d\n",i1);
  if(i1 > 0) {
    UFA_add_fac (fTab, iba[0], iba[1], iba[2]);
    UFA_add_fac (fTab, iba[0], iba[2], iba[3]);
  } else {
    UFA_add_fac (fTab, iba[0], iba[1], iba[3]);
    UFA_add_fac (fTab, iba[1], iba[2], iba[3]);
  }



  L_exit:
    // TESTDISPLAY Faces
    // MSH_test_disp4 (fa, *fNr, p2a);
    // printf("ex MSH_itess_pt2n %d\n",fTab->rNr);
    // MSH_dump_fTab (fTab->data, fTab->rNr);

  return 0;

}


//================================================================
  int MSH_dump_saf (MemTab(char) *saf) {
//================================================================
 
  int    i1;


  printf("sssssssssssssssss MSH_dump_saf %d sssssssssssssssss\n",saf->rNr);

  for(i1=0;i1<saf->rNr;++i1) printf(" saf[%d]=%d;",i1,saf->data[i1]);
  printf("\n");

  return 0;

}


//================================================================
  int MSH_dump_sap (char *sap, int pNr) {
//================================================================

  int    i1;
  
  
  printf("ppppppppppppppppp MSH_dump_sap %d ppppppppppppppppp\n",pNr);
  
  for(i1=0;i1<pNr;++i1) printf(" [%d]=%d;",i1,sap[i1]);
  printf("\n");
  
  return 0;            
                       
}


//================================================================
  int MSH_dump_fTab (Fac3 *fa, int fNr) {
//================================================================
// MSH_dump_fTab (fTab->data, fTab->rNr);

  int   i1;

  printf("FFFFFFFFFFFFFFFF MSH_dump_fTab %d FFFFFFFFFFFFFFFFFF\n",fNr);
  for(i1=0; i1<fNr; ++i1)
    UT3D_stru_dump (Typ_Fac3, &fa[i1], "if[%d]",i1);
  printf("--------------------------------------------------- \n");

  return 0;

}


//=============================================================================
  int MSH_fTab_BL__ (int ie1, int ie2, Point *pa,
                    Point2 *p2a, int p2Nr,
                    MemTab(Fac3) *MT_fac,
                    MemTab(char) *MT_saf, char *sap) {
//=============================================================================
// work breakline ie1 - ie2 (create Edgeline)
// Wenn die Edgeline durch existierende Faces geht, werden diese Faces
//   entlang der Edgeline aufgebrochen.
// Input:
//   ie1,ie2   pointNrs of breakline (indices into pa)
// Output:
//   fa        is modified.
// RetCod      nr of included points (must remesh !)

// Methode:
// - alle covered faces rund um breakline suchen
// - konturen aller faces links und rechts von der breakline suchen
// - TYP_EDGLN_BL: alle faces rund um breakline löschen
// - kontur links von der breakline neu vermeshen und die faces zufügen
// - nicht TYP_EDGLN_IB: kontur rechts von der breakline neu vermeshen und ..

// Wenn Breakline teilweise genau auf existing edgeline:
//   change point of breakline (remove doubly defined edgepart)


  int   irc, i1, ie, fn1, en1, ip1, ip2, ipe,
        *bTab1, bNr1, *bTab2, bNr2, incNr=0;
  Fac3  *fTab;
  int   fNr;

  int   *cfTab, cfNr;
#define  SIZ_cfTab 1000


  // printf("ffffffffffffffffffffffffffffffffffff \n");
  // printf("MSH_fTab_BL__ %d %d %d %d\n",ie1,ie2,MT_fac->rNr,MT_saf->rNr);


  fTab = MT_fac->data;
  fNr  = MT_fac->rNr;

  // get space for coveredFacesList
  cfTab = (int*)UME_alloc_tmp (SIZ_cfTab * sizeof(int));


  //----------------------------------------------------------------
  // Covered Faces suchen; Startface, die Austrittskante, diese ist die
  //   Eintrittkante in das FolgeFace usw
  // find first face & its exitEdge 
  cfNr = SIZ_cfTab; // maxSiz
  ipe = MSH_nif_findSegP (cfTab, &cfNr, ie1, ie2, fTab, fNr, pa);
  // breakline already exists
  if(ipe < 0) goto L_exit;   
  // no face is covered: exit ok.
  if(cfNr < 1) goto L_exit;   


  // if(ipe != ie2) then create new breakline ipe -> ie2
  if(ipe != ie2) printf("***** _MSH_fTab_BL__ I0001 \n");
  // ..


  //----------------------------------------------------------------
  // for delete all faces inside the polygon (cfTab):
  // update MT_saf; faces werden später in MSH_delFac_saf gelöscht ..
  for(i1=0; i1<cfNr; ++i1) MT_saf->data[cfTab[i1]] = 1;


  //----------------------------------------------------------------
  // Polygon1 = Umgrenzende Polygone suchen;
  // get space for coveredFacesList
  bTab1 = (int*)UME_alloc_tmp ((cfNr+4) * sizeof(int));
  bTab2 = (int*)UME_alloc_tmp ((cfNr+4) * sizeof(int));

  // beide umgrenzenden polygone suchen;
  // eingeschlossene Punkte sap[]=16 markieren
  incNr = MSH_2bnd_fTab_2pt (bTab1, &bNr1, bTab2, &bNr2,
                            cfTab, cfNr, ie1, ipe, fTab, fNr, sap);

  // polygon umdrehen
  MEM_inv_itab (bNr1, bTab1);


  // Tesselate bTab1; create new faces
  fNr  = MT_fac->rNr;
  MSH_itess_pt2n (MT_fac, bTab1, bNr1, p2a, p2Nr);
 
  // Tesselate bTab2; create new faces (only for breaklines, not for holes)
  MSH_itess_pt2n (MT_fac, bTab2, bNr2, p2a, p2Nr);


  // mark new created faces 2 = "breakLineFace"
  MT_saf->rNr = MT_fac->rNr;
  for(i1 = fNr; i1 < MT_fac->rNr; ++i1) {
    // test if one edge of face i1 is the breakline ie1-ie2
    UFA_2ip_ck_ptfac (&ip1, &ip2, &MT_fac->data[i1], ie1);
    if(ip1 < 0) goto L_saf_normal;
      // printf(" fac[%d] ip1=%d ip2=%d\n",i1,ip1,ip2);
    if((ip1 != ie2)&&(ip2 != ie2)) goto L_saf_normal;
    // this face has a breakline. Keep this face (do not optimize).
    MT_saf->data[i1] = 2;
    continue;

    L_saf_normal:
    MT_saf->data[i1] = 0;
  }



  //----------------------------------------------------------------
  // remove deleted records
  MSH_delFac__ (MT_fac, MT_saf);




  //----------------------------------------------------------------
  L_exit:
    // printf("ex MSH_fTab_BL__ %d\n",incNr);

  return incNr;

}


//================================================================
  int MSH_delFac__ (MemTab(Fac3) *fTab, MemTab(char) *saf) {
//================================================================
// MSH_delFac__                 remove deleted records
// records mit saf[i] == 1 werden gelöscht
//   (durch den letzten Record überschrieben).

  int     i1, ie;



  // printf("MSH_delFac__  %d\n",fTab->rNr);


  i1 = -1;
  goto L_del_nxt;

  
  L_del__:
    // 1=deleted
    if(saf->data[i1] != 1) goto L_del_nxt;
      // printf("L_del__: %d\n",i1);

  
    if(i1 < ie) {
      // copy the last fac-record ie -> i1
      fTab->data[i1] = fTab->data[ie];
      
      // copy the last saf-record ie -> i1
      saf->data[i1] = saf->data[ie];
    }

    fTab->rNr -= 1;
    --i1;                 // den soeben kopierten Record nochmal testen

  L_del_nxt:
    ++i1;
    ie = fTab->rNr - 1;
    if(i1 <= ie) goto L_del__;

    saf->rNr = fTab->rNr;

    // printf("ex MSH_delFac__  %d\n",fTab->rNr);
    // MSH_dump_fTab (fTab->data, fTab->rNr);
    // MSH_dump_saf (saf);

  return 0;

}


//================================================================
  int MSH_nif_findSegP (int *cfTab, int *cfNr,
                        int ips, int ipe,
                        Fac3 *fa, int fNr, Point *pa) {
//================================================================
// Covered Faces suchen; find all faces covered by Line ip1-ip2
// Input:
//   ips, ipe        index of start/endpoint of in pa
//   fa[fNr]         faces
//   pa              points
// Output:
//   cfTab[cfNr]     table of faceNrs
//   RetCod:
//     Exitpoint     must be ipe; else create new breakline -> ipe
//     -1            this breakline already exists; ignore ..

// Method:
//  find all Faces with startpoint;
//  check intersection with opposite edge;
//  add FaceNr.

  int     irc, iFac, siz_cf, ip1, ip2, ifn, iei, ieo;
  Point2  p2s;
  Vector2 vcl;


  // printf("MSH_nif_findSegP Line=%d,%d fNr=%d\n",ips,ipe,fNr);

  iei = 0;
  ieo = 0;

  siz_cf = *cfNr;
  *cfNr = 0;

  p2s = UT2D_pt_pt3 (&pa[ips]);
  UT2D_vc_2pt3 (&vcl, &pa[ips], &pa[ipe]);


  // find startpoint-face and exitEdge of point ip1
  irc = MSH_if_findSegP (&iFac, &ieo, ips, ipe, &pa[ipe], fa, fNr, pa);
    // printf(" ex _if_findSegP %d\n",irc);
  if(irc < 0) {TX_Print("MSH_nif_findSegP E001"); return -1;}
  if(irc == 0) goto L_1;    // correct intersection


  // test if endpoint reached (breakline = existing edge)
  if(irc == 10) return -1;


  //----------------------------------------------------------------
  // irc = 1-6: Line goes parallel to Edge ..

  TX_Print("MSH_nif_findSegP I001");
  return -1;



  //----------------------------------------------------------------
  // we have a face iFac and its outgoingEdge ieo.
  L_1:

  // get the pointNrs of edge ieo in face iFac
  UFA_2ip_fac_esn (&ip1, &ip2, ieo, &fa[iFac]);
    // printf(" iFac=%d ip1=%d ip2=%d\n",iFac,ip1,ip2);

  // add face iFac to output-facetabe ..
  cfTab[*cfNr] = iFac;
  if(*cfNr < siz_cf) *cfNr += 1;
  else { TX_Print("MSH_nif_findSegP EOF cfTab"); return -1; }

  // find next face with edge ie2-ie1 -> ifn=nextFaceNr, iei=incomingEdgeNr
  UFA_ifac_ck_edgo (&ifn, &iei, ip2, ip1, fa, fNr);
  if(ifn < 0) {
    printf("*** ERROR MSH_nif_findSegP_2 %d %d \n",ip1,ip2);
    return 0;
  }
    // printf(" _edgo: ifn=%d iei=%d\n",ifn,iei);

  // Line goes tru Edge ien into Face ifn; find exit-edge or exit-point
  irc = UFA_int_fac_ln2 (&ieo, iei, &p2s, &vcl, &fa[ifn], pa);
    // printf(" _fac_ln: irc=%d ifn=%d iei=%d ieo=%d\n",irc,ifn,iei,ieo);
  if(irc == 1) goto L_tru_pt;

  // exit tru edge ieo
    iFac = ifn;
    goto L_1;


  // exit tru PointSeqNr ieo
  L_tru_pt:
    // get pointNr ip1 from faceNr ifn & pointSeqNr ieo
    ip1 = UFA_ip_fac_psn (&fa[ifn], ieo);
      // printf(" _fac_psn ip=%d\n",ip1);


    // test if this is the endpoint
    if(ip1 == ipe) goto L_end_pt;

    // go tru point ip1, but this is not the endpoint;
    L_mid_pt:
      // must close contour here (ips-ip1);
      // create new breakline with the rest (ip1-ipe)


    L_end_pt:
    // endpoint reached. Close contours.
      // printf(" exitPt %d\n",ip1);

    // add exitFace ifn
    cfTab[*cfNr] = ifn;
    if(*cfNr < siz_cf) *cfNr += 1;
    else { TX_Print("MSH_nif_findSegP EOF cfTab"); return -1; }
    


  //----------------------------------------------------------------
    // printf("ex MSH_nif_findSegP %d\n",*cfNr);
    // dump faces:
    // for(irc=0; irc < *cfNr; ++irc) printf("cfTab[%d]=%d\n",irc,cfTab[irc]);


  return ip1;

}


//======================================================================
  int MSH_delFac_saf (int mode, MemTab(Fac3) *fTab, MemTab(char) *saf) {
//======================================================================
// delete faces from list
// die hintersten zuerst !!
// Input:
//   mode   0=delete in fTab
//          1=delete in fTab and saf

  int     i1, i2;

  // printf("MSH_delFac_saf %d\n",saf->rNr);

  i2 = saf->rNr - 1;

  for(i1=i2; i1>=0; --i1) {
      // printf(" saf[%d]=%d\n",i1,saf->data[i1]);
    if(saf->data[i1] != 1) continue;
      // printf(">>>>>>>> delete face %d\n",i1);
    MemTab_del (fTab, i1, 1);
    if(mode > 0)
      MemTab_del (saf, i1, 1);
    // MEM_del_nrec (&fTab->rNr, fTab->data, i1, 1, fTab->rSiz);
  }

  return 0;

}


//================================================================
  int MSH_pt_ck_pTab (Point *pTab, int pNr, Point *px, double tol) {
//================================================================
// test for "point already exists"; 2D-test with tolerance
// RetCod:
//   -1       ok, point does not yet exist
//  >=0       point already exists with that index

  int       i1;


  for(i1=0; i1<pNr; ++i1) {
    if(UT2D_comp2pt ((Point2*)(&pTab[i1]), (Point2*)px, tol) == 1) {
      // identical..
      // printf("ex MSH_pt_ck_pTab %d\n",i1);
      return i1;
    }
  }

    // printf("ex MSH_pt_ck_pTab -1\n");

  return -1;

}


//================================================================
  int MSH_eTab_findRecTyp (MemTab(EdgeLine) *eTab, int elTyp) {
//================================================================
// get recordNr of EdgeLineTable from edgeLineTyp
// RetCod:
//    RecordNr; -1 = notExists.

  int    i1, ii;

  ii = -1;

  for(i1=0; i1<eTab->rNr; ++i1) {

    if(eTab->data[i1].typ == elTyp) {
      ii = i1;
      break;
    }
  }

    // printf("ex MSH_eTab_findRecTyp %d %d\n",ii, elTyp);

  return ii;

}


//================================================================
  int MSH_3ip_EL_iip (int *ip1, int *ip2, int *ip3,
                      MemTab(EdgeLine) *eTab, MemTab(int) *eDat,
                      int iEL, int ipt) {
//================================================================
// MSH_3ip_EL_iip          get 1 or 2 segments around pt i2 on egde i1
// ipt   is the PointIndex in eTab[iEL].ia
// ip2   is point with pointNr ipt
// ip1   is the point before ip2
// ip3   is the point after ip2

  int      iNr, typ, *ia;
  EdgeLine *el1;


  // printf("MSH_3ip_EL_iip %d %d\n",iEL,ipt);


  el1 = &eTab->data[iEL];
  ia  = el1->ia;
  iNr = el1->iNr;
  typ = el1->typ;

  *ip2 = ia[ipt];


  if(ipt == 0) {
    if(typ == TYP_EDGLN_BL) {
      *ip1 = -1;
      *ip3 = ia[1];
    } else {
      *ip1 = ia[iNr - 1];
      *ip3 = ia[1];
    }


  } else if (ipt == (iNr - 1)) {
    if(typ == TYP_EDGLN_BL) {
      *ip1 = ia[iNr - 2];
      *ip3 = -1;
    } else {
      *ip1 = ia[iNr - 2];
      *ip3 = ia[0];
    }
    

  } else {
      *ip1 = ia[ipt - 1];
      *ip3 = ia[ipt + 1];
  }
    

  // printf("ex MSH_3ip_EL_iip %d %d %d\n",*ip1,*ip2,*ip3);


  return 0;

}

 
//================================================================
  int MSH_eTab_delRec (MemTab(EdgeLine) *eTab, MemTab(int) *eDat,
                       int elInd) {
//================================================================
// MSH_eTab_delRec         delete record in EdgeLineTable

// see MSH_EL_del_pt MSH_EL_get_tab MSH_eTab_fix

  int     i1, iSta, iNr;


  // MSH_dump_eTab (eTab, eDat);
  // printf("MSH_eTab_delRec %d\n",elInd);


  // iSta = startPos of datavalues in eDat
  // if((elInd < 0)||(elInd > ActEtab.rNr)) iSta = ActEtab.rNr;
  iSta = 0;
  for(i1=0; i1<elInd; ++i1) iSta += eTab->data[i1].iNr;


  iNr  = eTab->data[elInd].iNr;
    // printf(" iSta=%d iNr=%d\n",iSta,iNr);


  // delete ActEtab-record Nr elInd
  MEM_del_nrec (&eTab->rNr, eTab->data, elInd, 1, eTab->rSiz);


  // delete data out of DatEtab
  if(iNr > 0)
  MEM_del_nrec  (&eDat->rNr, eDat->data, iSta, iNr, eDat->rSiz);


  // die pointers ActEtab[].ia neu aufbauen
  MSH_eTab_fix (eTab, eDat);

    // MSH_dump_eTab (eTab, eDat);

  return 0;

}
 

//================================================================
  int MSH_EL_ck_pt (int *iEL, int *ipp,
                    MemTab(EdgeLine) *eTab, MemTab(int) *eDat,
                    int ip1) {
//================================================================
// MSH_EL_ck_pt          test if point ip1 is used by existing edgeline
// Input:
//   iEL       next breakline to test; -1 for first run ..
// Output:
//   iEL       breaklineNr
//   ipp       pointNr (in breakline iEL)
// RetCod:
//    0        point[ipp] in edgeLine[iEL] == ip1
//   -1        ip1 not used in following edgeLines; end of scan.
//   -2        ip1 not used in any edgeLine.


  int   i1, i2, iNr, *iTab, irc;


  // printf("MSH_EL_ck_pt iEL=%d ip1=%d\n",*iEL,ip1);
  // MSH_dump_eTab (eTab, eDat);


  if(eTab->rNr < 1) return -2;

  if(*iEL < 0) irc = -2;
  else         irc = -1;


  *iEL += 1;

  for(i1 = *iEL; i1<eTab->rNr; ++i1) {
    iNr  = eTab->data[i1].iNr;
    iTab = eTab->data[i1].ia;
    for(i2=0; i2<iNr; ++i2) {
        // printf(" i1=%d i2=%d ii=%d\n",i1,i2,iTab[i2]);
      if(iTab[i2] != ip1) continue;
      *iEL = i1;
      *ipp = i2;
      irc = 0;
      goto L_exit;
    }
  }


  L_exit:

    // printf("ex MSH_EL_ck_pt irc=%d iEL=%d ipp=%d\n",irc,*iEL,*ipp);

  return irc;

}


//================================================================
  int MSH_EL_chg_pt (MemTab(EdgeLine) *eTab, MemTab(int) *eDat,
                     int iNew, int iAct) {
//================================================================
// MSH_EL_chg_pt          change point iAct into iNew all EdgeLines


  int   i1, i2, iNr, iOff, *iTab;


  // printf("MSH_EL_chg_pt %d %d\n",iNew,iAct);

  if(eTab->rNr < 1) return 0;


  // change point iAct into iNew all EdgeLines
  iOff = 0;
  for(i1=0; i1<eTab->rNr; ++i1) {
    iNr  = eTab->data[i1].iNr;
    iTab = &eDat->data[iOff];
    for(i2=0; i2<iNr; ++i2) {
      if(iTab[i2] == iAct) iTab[i2] = iNew;
    }
    iOff += iNr;
  }

  return 0;

}


//================================================================
  int MSH_EL_del_pt (MemTab(EdgeLine) *eTab, MemTab(int) *eDat,
                     int ii) {
//================================================================
// MSH_EL_del_pt          delete point from EdgeLines
// check if point ii is used by EdgeLines;
// if yes: remove this point from the EdgeLines
// ATTENTION: after this EdgeLine-pointers ia are to be corrected ! (MSH_eTab_fix)


  int   i1, i2, iNr, iOff, *iTab;


  // printf("MSH_EL_del_pt %d\n",ii);
  // for(i1=0;i1<eDat->rNr;++i1)printf(" [%d]%d",i1,eDat->data[i1]);printf("\n");


  if(eTab->rNr < 1) return 0;


  // remove point ii from all EdgeLines
  iOff = 0;
  for(i1=0; i1<eTab->rNr; ++i1) {
    iNr  = eTab->data[i1].iNr;
    iTab = &eDat->data[iOff];
    for(i2=0; i2<iNr; ++i2) {
      // L_noAmoi:
      if(iTab[i2] == ii) {
        // delete point from EdgeLine
          // printf(" del eDat-rec %d\n",iOff + i2);
        MemTab_del (eDat, iOff + i2, 1);
        iNr -= 1;
        eTab->data[i1].iNr = iNr;
        eTab->data[i1].stat = 1;
        // --i2;
      }
    }
    iOff += iNr;
  }

  // for(i1=0;i1<eDat->rNr;++i1)printf(" [%d]%d",i1,eDat->data[i1]);printf("\n");

  return 0;

}


//=========================================================================
  int MSH_if_findSegP (int *iFac, int *iEdg,
                       int ips, int ipe, Point *ps2,
                       Fac3 *fa, int fNr, Point *pa) {
//=========================================================================
// find face with LineSegment ips-ps2 inside.
// The startPoint of the LineSegment is a facepoint; the endPoint is or is not.
// LineSegment: startPoint = pa[ips]    endpoint = ps2
// Input:
//   ips, ps2   a ray limited at ips tru ps2 (ps2=pa[ipe])
//   ipe     used only for test if breakline already exists
// Output:
//   iFac    Facenr
//   iEdg    EdgeSeqNr of edge in iFace where ips-ps2 goes tru
// RetCod:
//   -1      Error; no Face with point ips ...
//    0      correct intersection with Face iFac;
//    1-6    see UT2D_ptvc_ck_int2pt
//    10     this breakline already exists !



  int        irc, i2, i4, iif, iie, esn, ip1, ip2;
  int        iTab[50], iNr;
  char       psna[50];
  Vector2    vs;
  Point2     *p21, *p22, *ps1;


  // printf("MSH_if_findSegP %d\n",ips);


  // get all faces using point ips
  iNr = 50;
  irc = UFA_nifac_ck_pt (iTab, psna, &iNr, ips, fa, fNr);
  if(irc < 0) goto L_Err1;
  // all facNr's in iTab; corresponding pointSeqNr's are in psna.


  // make vs = vector-segment
  ps1 = (Point2*)&pa[ips];
  UT2D_vc_2pt (&vs, ps1, (Point2*)ps2);
    // UT3D_stru_dump (Typ_VC2, &vs, "  vs:");


  // loop tru iTab;
  // find face where edges goes tru
  for(i2=0; i2<iNr; ++i2) {
    iif = iTab[i2];       // iif = faceNr to test
    iie = psna[i2];       // iie = pointSeqNr of ips in face iif
    esn =  UFA_esn_opp_psn (iie);
      // printf(" test f[%d]-e%d\n",iif,iie);
    // get points of the edge opposite to the point iie
    // get 2D-points of edge of face + EdgeSeqNr
    // UFA_2pt2_fac_esn (&p21, &p22, esn, &fa[iif], pa);
    UFA_2ip_fac_esn (&ip1, &ip2, esn, &fa[iif]);
      // printf(" esn=%d ip1=%d ip2=%d\n",esn,ip1,ip2);

    // if (ip1|ip2 == endpunkt): this breakline already exists !
    if((ip1 == ipe)||(ip2 == ipe)) {
        // printf (" breakline already exists !\n");
      return 10; // this breakline already exists !
    }

    // seg starts at point ps1; check intersection with edge p21-p22
    p21 = (Point2*)&pa[ip1];
    p22 = (Point2*)&pa[ip2];
    irc = UT2D_ptvc_ck_int2pt (1, ps1, &vs, p21, p22);
    if(irc < 0) continue;
    // intersecting Face found; get intersecting EdgeNr
    *iEdg =  esn;
    goto L_found;
  }

  L_Err1:
  iif = -1;
  irc = -1;

  L_found:
  *iFac = iif;

    // printf("ex MSH_if_findSegP irc=%d iFac=%d iEdg=%d\n",irc,iif,esn);
    // if(irc>=0)
      // printf("   f[%d]=%d-%d-%d\n",iif,fa[iif].i1,fa[iif].i2,fa[iif].i3);

  return irc;

}
 

//================================================================
  int MSH_if_find_ptmsh (int *iFac, Point *pt1,
                         Fac3 *fa, int fNr,
                         Point *pa, int pNr) {
//================================================================
// find faceNr of point pt1 (2D-test point inside face)
// see also MSH_pt_prjptmsh1
// Output:
//   iFac      face nr (index into fa)
// Returncodes:
//   1 = NO  = point outside mesh
//   0 = YES = point inside triangle
//  -1 = ~~  = point is on sideline p1-p2
//  -2 = ~~  = point is on sideline p2-p3
//  -3 = ~~  = point is on sideline p3-p1
//  -4 = ~~  = point is equal to p1
//  -5 = ~~  = point is equal to p2
//  -6 = ~~  = point is equal to p3



  int     irc, i1, ii1, ii2, ii3;
  Point2  *pf1, *pf2, *pf3, *pp;


  // UT3D_stru_dump (Typ_PT, pt1, "MSH_if_find_ptmsh: ");


  // check if point is in Triangle or on its boundary
  pp = (Point2*)pt1;

  // loop tru triangles;
  for(i1=0; i1<fNr; ++i1) {
    ii1 = fa[i1].i1;
    ii2 = fa[i1].i2;
    ii3 = fa[i1].i3;

    pf1 = (Point2*)&pa[ii1];
    pf2 = (Point2*)&pa[ii2];
    pf3 = (Point2*)&pa[ii3];

    // test if point ip1 is inside Face ii1-ii2-ii3
    irc = UT2D_ck_pt_in_tria (pf1, pf2, pf3, pp);
    if(irc > 0) continue;    // outside ..
    goto L_found;
  }

  // point is ouside mesh
    // printf("ex MSH_if_find_ptmsh pt is outside ..\n");
  return 1;


  L_found:  //irc: 0=inside, -1=on i1-i2, -2=on i2-i3, -3=on i3-i1
  *iFac = i1;

    // printf("ex MSH_if_find_ptmsh if=%d irc=%d %d %d %d\n",i1,irc,ii1,ii2,ii3);

  return irc;

}


//================================================================
  int MSH_2bnd_fTab_2pt (int *bTab1, int *bNr1,
                         int *bTab2, int *bNr2,
                         int *ifTab, int ifNr,
                         int ips, int ipe,
                         Fac3 *fa, int fNr, char *sap) {
//================================================================
// MSH_2bnd_fTab_2pt       find boundaries around edge (line)
// eingeschlossene Punkte sap[]=16 markieren
// get boundary of Line tru faces.
// ifTab: faces, strip, connected side by side.
// Line ips,ipe is crossing all faces in ifTab;
// ie1 is on ifTab[0], ie2 is ifTab[ifNr-1].
// Input:
//   mode        1=get CCW-ouput-polygon; -1=get CW-ouput-polygon
//   ifTab[ifNr] connected faceNrs
//   ips,ipe     line (pointNrs)
// Output:
//   bTab[bNr]  pointNrs; a closed polygon with ips, ipe
// RetCod       nr of included points (must remesh !)

  int   irc, i1, ii, psn1, psn2, esn1, esn2, ip1, ip2, incNr,
        oldip1 = -1, oldip2 = -1;
  Fac3  *fac1, *fac2;


  // printf("MSH_2bnd_fTab_2pt ips=%d ipe=%d\n",ips,ipe);


  // copy startPoint
  bTab1[0] = ips;   *bNr1 = 1;
  bTab2[0] = ips;   *bNr2 = 1;

  ii = 0;
  incNr = 0;

  //----------------------------------------------------------------
  L_nxt:
  fac1 = &fa[ifTab[ii]];
  ++ii;
  fac2 = &fa[ifTab[ii]];


  // test if last face
  if(ii+1 >= ifNr) goto L_done;


  // get common edges of 2 faces with common edge
  irc = UFA_2esn_comm2fac (&esn1, &esn2, fac1, fac2);

  // use esn2; add ip1 to bTab1, ip2 to bTab2.

  // get ip's of esn2
  UFA_2ip_fac_esn (&ip1, &ip2, esn2, fac2);
    // printf(" add ip's %d %d\n",ip1,ip2);

  irc = MSH_ipTab_addu (bNr1, bTab1, ip1, sap);  // add point 
  incNr += irc;

  irc = MSH_ipTab_addu (bNr2, bTab2, ip2, sap);  // add point 
  incNr += irc;


  goto L_nxt;


  //----------------------------------------------------------------
  L_done:  // fac2 = last face;

  // get & add points of last face
  // get psn of point ipe in fac2 -> psn1
  psn1 = UFA_psn_1fac_ip (fac2, ipe);

  // get edgeSeqNr of edge opposit to psn1 -> esn1
  esn1 = UFA_esn_opp_psn (psn1);

  // get ip's of esn2
  UFA_2ip_fac_esn (&ip1, &ip2, esn1, fac2);
    // printf(" add ip's %d %d\n",ip1,ip2);

  irc = MSH_ipTab_addu (bNr1, bTab1, ip1, sap);  // add point 
  incNr += irc;

  irc = MSH_ipTab_addu (bNr2, bTab2, ip2, sap);  // add point 
  incNr += irc;


  //  add closing point
  bTab1[*bNr1] = ipe;   *bNr1 += 1;
  bTab2[*bNr2] = ipe;   *bNr2 += 1;


  // change direction of bTab2
  // ..



  //----------------------------------------------------------------
  // printf("ex MSH_bnd1_fTab_2pt %d %d incNr=%d\n",*bNr1,*bNr2,incNr);
  // for(i1=0;i1 < *bNr1;++i1) printf("bTab1[%d]=%d\n",i1,bTab1[i1]);
  // for(i1=0;i1 < *bNr2;++i1) printf("bTab2[%d]=%d\n",i1,bTab2[i1]);

  

  return incNr;

}


//================================================================
  int MSH_ipTab_addu (int *pNr, int *pTab, int ip1, char *sap) {
//================================================================
// add point ip to pTab if uniq; if not: remove all following points
// eingeschlossene Punkte sap[]=16 markieren
// RetCod  nr of points included
//   
 
  int      i1, i2, incNr;

  // printf("MSH_ipTab_addu pNr=%d ip1=%d\n",*pNr,ip1);


  incNr = 0;  // nr of points inside

  for(i1=0; i1<*pNr; ++i1) {
    if(pTab[i1] == ip1) {
      // eingeschlossene Punkte sap[]=16 markieren
      for(i2=i1 + 1; i2<*pNr; ++i2) {
          // printf(" must remesh %d\n",pTab[i2]);
        if(sap[pTab[i2]] = 16) {
          sap[pTab[i2]] = 16;
          ++incNr;
        }
      }
      *pNr = i1 + 1;
      goto L_exit;
    }
  }

  // add point ..
  pTab[*pNr] = ip1;
  *pNr += 1;



  L_exit:
  // printf("ex MSH_ipTab_addu pNr=%d ip1=%d\n",*pNr,ip1);
  return incNr;

}


//================================================================
  int MSH_dump_eTab (MemTab(EdgeLine) *eTab, MemTab(int) *eDat) {
//================================================================
/// MSH_dump_eTab         dump EdgeLines

  int        i1, i2, ii, iNr;
  EdgeLine   *ela;


  // printf("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE MSH_dump_eTab\n");
  UT3D_stru_dump (Typ_MemTab, eTab, "MSH_dump_eTab:");


  // ActEtab.data is a table of EdgeLines
  for(i1=0; i1<eTab->rNr; ++i1) {
    ela = &eTab->data[i1];
    printf("eTab[%d].iNr=%d typ=%d\n",i1,ela->iNr,ela->typ);
    for(i2=0; i2<ela->iNr; ++i2) {
      printf("    i[%d]=%d\n",i2,ela->ia[i2]);
    }
  }

  UT3D_stru_dump (Typ_MemTab, eDat, "eDat:");
  printf("------------------------------------- \n");

  return 0;

}


//=============================================================================
  int MSH_msh_siz (double *d1, MemTab(EdgeLine) *eTab, Point *pa) {
//=============================================================================
// MSH_msh_len__           create Mesh from Points & boundary
// Input:
//   pa       the 3D-table
//   pNr      nr of points in pa

// Method:

  int      i1, i2, ii, *ia, iNr;
  Point2   p1, p2;
  Vector2  vc1;


  iNr = -1;

  for(i1=0; i1<eTab->rNr; ++i1) {
    if(eTab->data[i1].typ != TYP_EDGLN_AB) continue;

    ia  = eTab->data[i1].ia;
    iNr = eTab->data[i1].iNr - 1;  // letztes - 1 !

    // loop tru edges of a IB
    p1 = UT2D_pt_pt3 (&pa[ia[0]]);
    p2 = UT2D_pt_pt3 (&pa[ia[1]]);
    for(i2=2; i2<iNr; ++i2) {
      UT2D_box_extend (&p1, &p2, (Point2*)&pa[ia[i2]]);
    }
  }

  if(iNr < 0) return -1;

  UT2D_vc_2pt (&vc1, &p1, &p2);
  *d1 = vc1.dx + vc1.dy;

  // printf("ex MSH_msh_siz %f\n",*d1);



  return 0;

}


//=============================================================================
  int MSH_msh_len_ck1 (double dsl, MemTab(Fac3) *fTab, MemTab(EdgeLine) *eTab,
                                   Point2 *p2a, MemTab(char) *saf, Fac3 *fnb) {
//=============================================================================
// see MSH_delFac_IB__



  // get index of autoBound

  // go along edges of the autoBound; test the 2 inner edges recursive;
  // if (edgeLen < dsl) end; else mark face foe deletion.


  int        i1, i2, i3, *ia, iNr, ip1, ip2;
  int        if1, esn1, fNr;
  char       *sTab;
  Fac3       *fa;


  // printf("MSH_msh_len_ck1 %f\n",dsl);


  fa  = fTab->data;
  fNr = fTab->rNr;

  sTab = saf->data;

  iNr = -1;

  for(i1=0; i1<eTab->rNr; ++i1) {
    if(eTab->data[i1].typ != TYP_EDGLN_AB) continue;

    ia  = eTab->data[i1].ia;
    iNr = eTab->data[i1].iNr;
    i3 = iNr - 1;

    // loop tru edges of a IB
    for(i2=0; i2<iNr; ++i2) {
      // get next edge -> ie1-ie2
      ip1 = ia[i2];
      ip2 = ia[i2 + 1];
      if(i2 < i3) ip2 = ia[i2 + 1];
      else        ip2 = ia[0];
        // printf(" AB[%d] nxt edge %d %d\n",i2,ip1,ip2);

      // find face with oriented edge ie1-ie2 (bound CCW)
      UFA_ifac_ck_edgo (&if1, &esn1, ip1, ip2, fa, fNr);
        // printf(" if1=%d esn1=%d\n",if1,esn1);
      if(if1 < 0) continue;  // both sides have no face !

      // // if this face already deleted: done ..
      // if(sTab[if1] == 1) continue;

      // rekursiv testen ob die beiden nächsten Edges an faces innerhalb
      // der Boundary grenzen; wenn ja: Faces löschen und weitersuchen ..
      // get next edge on face;
      esn1 = UFA_esn_nxt (esn1);    // get next edge
      // MSH_msh_len_ck2 (if1, esn1, fa, fnb, sTab, fNr, ia, iNr);
      MSH_msh_len_ck2 (dsl, if1, esn1, fa, fnb, p2a, sTab, fNr);

      // get next edge on face;
      esn1 = UFA_esn_nxt (esn1);    // get next edge
      // MSH_msh_len_ck2 (if1, esn1, fa, fnb, sTab, fNr, ia, iNr);
      MSH_msh_len_ck2 (dsl, if1, esn1, fa, fnb, p2a, sTab, fNr);
    }
  }

  if(iNr < 0) return -1;


  // TEST 
  // for(i1=0; i1<fNr; ++i1) if(sTab[i1] == 1) printf(" del fac %d\n",i1);



  return 0;

}


//=============================================================================
  int MSH_msh_len_ck2 (double dsl, int iFac, int iEdg,
                       Fac3 *fa, Fac3 *fnb, Point2 *p2a, char *sTab, int fNr) {
//=============================================================================

// see also MSH_delFac_IB_f

  int     irc, ip1, ip2, nbFac, esn1;  
  double  dd;
  Vector2 vc1;


  // printf("MSH_msh_len_ck2 ifac=%d iEdg=%d\n",iFac,iEdg);

  // get pointNrs of edge iEdg on face iFac
  UFA_2ip_fac_esn (&ip1, &ip2, iEdg, &fa[iFac]);
    // printf(" ip1=%d ip2=%d\n",ip1,ip2);


  // length ip1-ip2 ?
  UT2D_vc_2pt (&vc1, &p2a[ip1], &p2a[ip2]);
  dd = fabs(vc1.dx) + fabs(vc1.dy);
    // printf(" dd %f\n",dd);
  if(dd < dsl) return 0;


  // add face if1 to delete-list
  sTab[iFac] = 1;
    // printf(" markdelete %d\n",iFac);


  // get neighbour face and mark for deletion
  nbFac = UFA_inbFac_esn (iFac, iEdg, fnb);
  if(nbFac < 0) return 0; // no neighbour ..


  // done if face already deleted ..
  if(sTab[nbFac] == 1) return 0;

  // delete this face.
  sTab[nbFac] = 1;
    // printf(" markdelete %d\n",nbFac);

  // which esn in nbFac ?
  esn1 = UFA_esn_2ip (ip2, fa, nbFac);

  // test both oppositeEdges of this face
  // get next edge & test (Rekursion!)
  esn1 = UFA_esn_nxt (esn1);
  MSH_msh_len_ck2 (dsl, nbFac, esn1, fa, fnb, p2a, sTab, fNr);

  esn1 = UFA_esn_nxt (esn1);
  MSH_msh_len_ck2 (dsl, nbFac, esn1, fa, fnb, p2a, sTab, fNr);


  return 0;

}


//================  EOF  ==================================
