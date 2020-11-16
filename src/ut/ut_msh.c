//  ut_msh.c                                  2008-01-24    Franz Reiter.
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

MSH_ibnd_ptn            find boundary of 3D-pTab
MSH_ibnd_pt2n           find boundary of 2D-pTab

MSH_pt_prjptmsh_        prj point -> mesh (load mesh)
MSH_pt_prjptmsh1        prj point -> active mesh
MSH_pta_prjptmsh1       prj points -> active mesh
MSH_npt_prjcvmsh_       project curve > Mesh

MSH_eTab_fix            recover EdgeLine-pointers ia

List_functions_end:
=====================================================
- see also:
../ut/ut_face.c      UFA   Funktionen fuer Faces (Fac3)
../ut/ut_tria.c     Triang-functions    (using struct Point)

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




// #include "../ut/ut_umem.h"             // MEM_alloc_tmp
#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_itmsh.h"            // MSHIG_EDGLN_.. typedef_MemTab.. Fac3
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_face.h"              // UFA
#include "../ut/func_types.h"               // UI_Func... SYM_..
#include "../ut/ut_os.h"               // OS_ ..
#include "../ut/ut_TX.h"               // TX_Print

#include "../xa/xa_msg.h"              // MSG_ERR_typ_*
#include "../xa/xa_mem.h"              // memspc51
#include "../xa/xa.h"                  // AP_modact_nam





//================================================================
   int MSH_aload_pTab (char *lBuf, int bufSiz, FILE *fpi) {
//================================================================
/// \code
/// MSH_aload_pTab             load pointTable ascii from open file
/// Input:
///   all following ascii-lines from open file fpi; "SECTIONEND" = end.
/// Output:
///   write into binary file    <tmp>_A<dbi>.ptab
/// \endcode


  int    irc, i1, pNr;
  long   ld;
  char   *cp1, *cp2, cbuf[256];
  // MemTab(Point) pTab = MemTab_Init(sizeof(Point), Typ_PT, 10000);
  MemTab(Point) pTab = _MEMTAB_NUL;

  MemTab_ini__ (&pTab, sizeof(Point), Typ_PT, 10000);


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

  MemTab(Fac3) fTab = _MEMTAB_NUL;
  MemTab(EdgeLine) eTab = _MEMTAB_NUL;
  MemTab(int) eDat = _MEMTAB_NUL;




  MemTab_ini__ (&fTab, sizeof(Fac3), Typ_Fac3, 10000);
  MemTab_ini__ (&eTab, sizeof(EdgeLine), Typ_EdgeLine, 10);
  MemTab_ini__ (&eDat, sizeof(int), Typ_Int4, 50);



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
  irc = MemTab_check ((MemTab*)&fTab, &ld, fNr);
  if(irc < 0) goto L_EOM;
  fTab.rNr = fNr;

  // get space for eNr records
  // if(eNr > 0) {
    irc = MemTab_check ((MemTab*)&eTab, &ld, eNr);
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
    irc = MemTab_check ((MemTab*)&eDat, &ld, pNr);
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

  // save faces
  iNr  = fTab->rNr;
  iSiz = sizeof(Fac3);  // fTab->rSiz;
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
    // printf("  nr_fTab=%d\n",iNr);

  // clear fTab
  fTab->rNr = 0;

  if(iNr) {
    // check space for iNr records
    irc = MemTab_check ((MemTab*)fTab, &ld, iNr);
    if(irc < 0) goto L_EOM;
  
    // load faces
    iSiz = sizeof(Fac3);  // fTab->rSiz;
    fread(fTab->data, iSiz, iNr, fp1);
    fTab->rNr = iNr;
  }


  //----------------------------------------------------------------
  // load EdgeLines
  fread(&iNr, sizeof(int), 1, fp1);
    // printf(" nr_eTab=%d\n",iNr);

  if(eTab) {
    // clear eTab
    eTab->rNr = 0;
    if(iNr) {
      // check space for iNr records
      irc = MemTab_check ((MemTab*)eTab, &ld, iNr);
      if(irc < 0) goto L_EOM;
      // load EdgeLines
      iSiz = eTab->rSiz;
      fread(eTab->data, iSiz, iNr, fp1);
      eTab->rNr = iNr;
    }

  } else {
    lSiz = iNr * sizeof(int);
    fseek (fp1, lSiz, 1);
  }



  //----------------------------------------------------------------
  // load EdgeData
  fread(&iNr, sizeof(int), 1, fp1);
    // printf(" nr_eDat=%d\n",iNr);

  if(eDat) {
    // clear eDat
    eDat->rNr = 0;
    if(iNr) {
      // get space for iNr records
      irc = MemTab_check ((MemTab*)eDat, &ld, iNr);
      if(irc < 0) goto L_EOM;
      // load EdgeData
      iSiz = eDat->rSiz;
      fread(eDat->data, iSiz, iNr, fp1);
      eDat->rNr = iNr;
    }

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


  int     i1; 
  // int     *ipa;
  Point2  *p2a;


  // printf("MSH_ibnd_ptn %d ibSiz = %d\n",pNr,*ibNr);
  // for(i1=0;i1<pNr;++i1) DEB_dump_obj__ (Typ_PT,&pa[i1],"P[%d]",i1);

  i1 = sizeof(memspc501) / sizeof(Point2);  // 16 byte
  if(i1 < pNr) {
    p2a = (Point2*) MEM_alloc_tmp ((int)(pNr * sizeof(Point2)));
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
  UT2D_npt_npt3 (p2a, pa, pNr);


  // get boundary into iba
  // i1 = MSH_ibnd_pt2n (iba, ibNr, p2a, p2Nr, ipa);
  i1 = MSH_ibnd_pt2n (iba, ibNr, p2a, pNr);
  if(i1 < 0) return -1;


  // // copy the pointers into pa to iba (replace 2D-pointers with 3D-pointers).
  // for(i1=0; i1 < *ibNr; ++i1) {
    // iba[i1] = ipa[iba[i1]];
  // }


  // TEST: display polygon
  // Point  cv[iNr+1];
  // for(i1=0; i1<bNr; ++i1) { cv[i1] = pa[ipa[iba[i1]]]; }
  // cv[iNr] = cv[0];
  // GR_tDyn_pcv (cv, iNr+1, 5);


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
  // for(i1=0;i1<p2Nr;++i1) DEB_dump_obj__ (Typ_PT2,&p2a[i1],"P[%d]",i1);

  ibSiz = *ibNr;


  // get spc for statArray sa
  sa = MEM_alloc_tmp (p2Nr + 4);
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
    irc = UT2D_ck_pt_in_tria__ (&p2a[ii1], &p2a[ii2], &p2a[ii3], &p2a[i1]);
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
      irc = UT2D_ck_pt_in_tria__ (&p2a[ii1], &p2a[ii2], &p2a[ii3], &p2a[i1]);
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
    irc = UT2D_sid_ptvc__ (&p2a[i1], &p2a[ii1], &ve);
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
    irc = UT2D_ck_pt_in_tria__ (&p2a[ii1], &p2a[iin], &p2a[ii2], &p2a[i1]);
    if(irc > 0) continue;
    sa[i1] = -1; // inside.
  }


  // Punkt iin einfuegen oder ersetzen.
  sa[iin] = -1;



  // Testen ob der Punkt ii2 im 3Eck ii1-iin-ii3 enthalten ist
  irc = UT2D_ck_pt_in_tria__ (&p2a[ii1], &p2a[iin], &p2a[ii3], &p2a[ii2]);
  if(irc < 1) {  // inside or on contour: replace
    // Punkt iin befindet sich vor dem Punkt ie2
    // iin replaces ie2
    iba[ie2] = iin;
    goto L_nxt_prep;
  }


  // Testen ob der Punkt ii1 im 3Eck ii0-iin-ii2 enthalten ist
  irc = UT2D_ck_pt_in_tria__ (&p2a[ii0], &p2a[iin], &p2a[ii2], &p2a[ii1]);
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


//================================================================
  int MSH_pt_prjptmsh_ (Point *pto, Point *pti,
                        int surMsh,  int surPtab, double tol) {
                        // Vector *vp)
//================================================================
/// \code
/// project point > Mesh  (load mesh)
/// 
///   retCod  0 OK; inside_face, on_face_edge:  io=faceNr, pto=point
///           1 OK; pti is on_edge; io=faceNr, ie=edgeNr, pto=point
///           2 OK; pti is identical_with_point: io=pointIndex, ie=edgeNr, pto=pnt
///          -1 point is not inside mesh
///          -2 Error in inputData
/// Output:
/// \endcode


  int       i1, io, ie;
  // int       pNr, fNr;
  // Point     *pTab = NULL;
  // Fac3      *fTab = NULL;

  // MemTab(Point) pTab = MemTab_Init(sizeof(Point), Typ_PT, 10000);
  // MemTab(Fac3) fTab = MemTab_Init(sizeof(Fac3), Typ_Fac3, 10000);
  // MemTab(EdgeLine) eTab = MemTab_Init(sizeof(EdgeLine), Typ_EdgeLine, 10);
  // MemTab(int) eDat = MemTab_Init(sizeof(int), Typ_Int4, 50);
  MemTab(Point) pTab = _MEMTAB_NUL;
  MemTab(Fac3) fTab = _MEMTAB_NUL;
  MemTab(EdgeLine) eTab = _MEMTAB_NUL;
  MemTab(int) eDat = _MEMTAB_NUL;

  MemTab_ini__ (&pTab, sizeof(Point), Typ_PT, 10000);
  MemTab_ini__ (&fTab, sizeof(Fac3), Typ_Fac3, 10000);
  MemTab_ini__ (&eTab, sizeof(EdgeLine), Typ_EdgeLine, 10);
  MemTab_ini__ (&eDat, sizeof(int), Typ_Int4, 50);



  // load PointFile  (write: lxml_read) pTab=malloc !
  i1 = MSH_bload_pTab (&pTab, AP_modact_nam, surPtab);
  if(i1 < 0) {TX_Error("TSU_DrawSurPMsh E001"); return -2;}
    // printf(" pNr=%d\n",pTab.rNr);


  // load MeshFile    fTab=malloc !
  i1 = MSH_bload_fTab (&fTab, &eTab, &eDat, AP_modact_nam, surMsh);
  if(i1 < 0) {TX_Error("TSU_DrawSurPMsh E002"); return -2;}
    // printf(" fNr=%d\n",fTab.rNr);


  // project point > Mesh  (using memspc501)
// TODO: p2a fehlt !
  i1 = MSH_pt_prjptmsh1 (&io, &ie, pto, pti, fTab.data, fTab.rNr,
                         pTab.data, pTab.rNr, tol);
    // GR_tDyn_symB__ (&p1, SYM_STAR_S, 2);

  // if(pTab) free(pTab);
  // if(fTab) free(fTab);
  MemTab_free (&eDat);
  MemTab_free (&eTab);
  MemTab_free (&fTab);
  MemTab_free (&pTab);


    // DEB_dump_obj__ (Typ_PT, pto, "ex MSH_pt_prjptmsh_ %d",i1);


  return i1;

}


//================================================================
  int MSH_pta_prjptmsh1 (int *ptNr, Point *pa1, int *ia1,
                         Fac3 *fa, int fNr,
                         Point *pa, Point2 *p2a, int pNr, double tol) {
//================================================================
/// \code
/// MSH_pta_prjptmsh1       prj points (Z=0) -> active mesh
/// if point.z == 0.  then set Z to Z-value on mesh; else keep Z-value.
/// Deletes points outside mesh !
/// Input 
///   ptNr     nr of points
///   pta      new points (compute Z if Z=0; else keep Z-value)
///   ia1      index of point if identical with existing point in pa
///   pa[pNr]  existing points
/// Output
///   pNr      modified 
///   pta      all points outside mesh are removed !
///   retCod   nr of errors
/// \endcode


  int       i1, i2, ii, ie, eNr, irc;
  Point     px;


  for(i1 = 0; i1 < *ptNr; ++i1) ia1[i1] = -1;  // point not yet known


    // if p1a[iAct].z = 0 then we have to project it onto the mesh.
    i1 = 0;
    i2 = 0;
    eNr = 0;


    //----------------------------------------------------------------
    // loop tru points in pa1
    // next point i1
    L_nxt_ck__:
      // ignore point if (Z != 0)
      if(!UTP_comp_0(pa1[i1].z)) goto L_nxt_ck_nxt;
      // project p1a[iAct] onto the mesh (get the correct Z-value)
      px = pa1[i1];
      // is using memspc501 !
      irc = MSH_pt_prjptmsh1 (&ia1[i1], &ie, &pa1[i1], &px, fa, fNr,
                              pa, pNr, tol);

        // TESTBLOCK
          // printf(" pt_prjptmsh1 p%d irc=%d ia=%d\n",i1,irc,ia1[i1]);
        // TESTBLOCK

      if(irc < 0) {
        // point ouside mesh
// TODO: gis-offset ?  // gis_log_pt3 (&pa1[i1]);  // report errPos
        LOG_A_pt3 (&pa1[i1]);  // report errPos
        LOG_A__ (MSG_ERR_typ_ERR, "point[%d] outside mesh - deleted", i2);
        // TX_Error("point[%d] - %f %f outside mesh - deleted", i1,
                 // pa1[i1].x,pa1[i1].y);
        MEM_del_nrec (ptNr, pa1, i1, 1, sizeof(Point));
        --i1;
        ++eNr;

      }
      

      L_nxt_ck_nxt:
        // printf(" pta_prjptmsh1 p%d i%d %f %f %f\n",i1,ia1[i1],
                // pa1[i1].x,pa1[i1].y,pa1[i1].z);

      ++i1;
      ++i2;
      if(i1 < *ptNr) goto L_nxt_ck__;



  return eNr;

}


//================================================================
  int MSH_pt_prjptmsh1 (int *io, int *ie, Point *pto, Point *pti,
                        Fac3 *fa, int fNr,
                        Point *pa, int pNr, double tol) {
//================================================================
/// \code
/// project point > Mesh
/// using memspc501
/// 
/// Output:
///   io      rc=0: faceNr; rc=1: pointIndex
///   pto     rc=0: pti on face (Z fixed); rc=1: NULL
///   retCod  0 OK; inside_face, on_face_edge:  io=faceNr, pto=point
///           1 OK; pti is on_edge; io=faceNr, ie=edgeNr, pto=point
///           2 OK; pti is identical_with_point: io=pointIndex;
///          -1 point is not inside mesh
/// 
/// - transform points > 2D;
/// - check if point is in Triang or on its boundary
/// - get Z-coord of point on 2D-Triang
/// \endcode


  int     irc, i1, ii1, ii2, ii3, ix;
  // int     p2Nr;
  double  d1, d2, d3;
  Point2  p2i;
  Point   *p1, *p2, *p3;
  Vector  vcn, *vp;
  Plane   pl1;


  // DEB_dump_obj__ (Typ_PT, pti, "MSH_pt_prjptmsh1 fNr=%d pNr=%d",fNr,pNr);
  // printf("MSH_pt_prjptmsh1 tol=%f\n",tol);

  p2i = UT2D_pt_pt3 (pti);
    // DEB_dump_obj__ (Typ_PT2, &p2i, "p2i=");


  vp = (Vector*)&UT3D_VECTOR_Z;  // projectionVector

  // check if point is in Triang or on its boundary
  // loop tru triangles;
  for(i1=0; i1<fNr; ++i1) {
    *io = i1;
    ii1 = fa[i1].i1;
    ii2 = fa[i1].i2;
    ii3 = fa[i1].i3;

    // test if point ip1 is inside Face ii1-ii2-ii3
    irc = UT2D_ck_pt_in_tria__ ((Point2*)&pa[ii1],
                              (Point2*)&pa[ii2],
                              (Point2*)&pa[ii3], &p2i);
    if(irc > 0) continue;    // outside ..
    goto L_found;
  }

  // point is ouside mesh
    // printf("MSH_pt_prjptmsh1 pt is outside ..\n");
  return -1;


  //----------------------------------------------------------------
  L_found:
    // TESTBLOCK
    // printf(" pt_in_tria irc=%d if=%d i1=%d i2=%d i3=%d\n",irc,i1,ii1,ii2,ii3);
    // TESTBLOCK


  // irc: -4: pi == p1; -5: pi == p2; -6: pi == p3.
  if(irc <= -4) {
    // irc 2 - pti_is_identical_with_point: io=pointIndex, pto=point.
    if(irc == -4) { *pto = pa[ii1]; *io = ii1; *ie = 1; }
    if(irc == -5) { *pto = pa[ii2]; *io = ii2; *ie = 2; }
    if(irc == -6) { *pto = pa[ii3]; *io = ii3; *ie = 3; }
    return 2;  // was 1
  }






  //----------------------------------------------------------------
  // get Z-coord of point on 2D-Triang.
  // irc: 0=inside, -1=on i1-i2, -2=on i2-i3, -3=on i3-i1
    // printf(" f=%d irc=%d %d %d %d\n",i1,irc,ii1,ii2,ii3);
  p1 = &pa[ii1];
  p2 = &pa[ii2];
  p3 = &pa[ii3];
    // DEB_dump_obj__ (Typ_PT, p1, "p1=");
    // DEB_dump_obj__ (Typ_PT, p2, "p2=");
    // DEB_dump_obj__ (Typ_PT, p3, "p3=");
    // DEB_dump_obj__ (Typ_VC, &pl1.vz, "vz=");


  // compute intersectionpoint pto (Z fixed)
  // see UT3D_pl1_tria MSH_pt_prjptfac
  // get pl1.vz = normalVector of triangle
  UT3D_vc_perp3pt (&pl1.vz, p1, p2, p3);
  UT3D_vc_setLength (&pl1.vz, &pl1.vz, 1.);
    // DEB_dump_obj__ (Typ_VC, &pl1.vz, "pl1.vz:");

  // set plane-origin, create pl1.p
  UT3D_pl_ptpl (&pl1, p1);
    // DEB_dump_obj__ (Typ_PLN, &pl1, "pl1:");
  // intersect plane with pti-vp (vp=Z-vec)
  ix = UT3D_ptDi_intptvcpln (pto, &d1, &pl1, pti, vp);

  if(ix == 0) {   // plane parallel; Z=zVal of gravityCenterpoint)
    *pto = *pti;
    pto->z = (p1->z + p2->z + p3->z) / 3.;
  }


  //----------------------------------------------------------------
  // irc: -1=on i1-i2, -2=on i2-i3, -3=on i3-i1
  if(irc <= -1) {
    // irc 1 - pti is on_edge; io=faceNr, ie=edgeNr, pto=point.
// TODO: if on-egde then the intersection-point could be computed easier
    if(irc == -1) { *io = ii1; *ie = 1; }
    if(irc == -2) { *io = ii2; *ie = 2; }
    if(irc == -3) { *io = ii3; *ie = 3; }
    return 1;
  }



  //----------------------------------------------------------------
  // irc: 0=inside;
  // check distance from nearest edge; if (dist < tol)  then set (irc = 1)
  // test dist pto p1-p2
  d1 = UT3D_nlen_3pt (p1, pto, p2);
  if(d1 < tol) {*ie = 1; irc = 1;}
  // test dist pto p2-p3
  d1 = UT3D_nlen_3pt (p2, pto, p3);
  if(d1 < tol) {*ie = 2; irc = 1;}
  // test dist pto p3-p1
  d1 = UT3D_nlen_3pt (p3, pto, p1);
  if(d1 < tol) {*ie = 3; irc = 1;}
  // get the smallest distance

    // printf("ex prjptmsh1 irc=%d oi=%d ie=%d\n",irc,*io,*ie);

  return irc;

}


//================================================================
  int MSH_npt_prjcvmsh_ (Point **paOut, int *paNr,
                         Point *pPlg, int plgNr,
                         MemTab(Fac3) *fTab,
                         MemTab(Point) *pTab,
                         MemTab(EdgeLine) *eTab,
                         MemTab(int) *eDat,
                         double *tol) {
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
  // for(i1=0;i1<plgNr;++i1) DEB_dump_obj__ (Typ_PT,&pPlg[i1],"p[%d]=",i1);
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
  i1 = UFA_if_find_ptmsh (&iFac, pe1, fa, fNr, pa, pNr);


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
    UFA_if_findSegP (&iFac, &i2, ii1, -1, pe2, fa, fNr, pa);
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
    if((eTab->data[i2].typ != MSH_EDGLN_IB)  &&
       (eTab->data[i2].typ != MSH_EDGLN_OB)) continue;
    ela = &eTab->data[i2];
    i4 = ela->iNr - 1;
    for(i3=0; i3<ela->iNr; ++i3) {  // loop tru boundary ela
      ii1 = ela->ia[i4];
      ii2 = ela->ia[i3];
      p2s1 = (Point2*)&pa[ii1];
      p2s2 = (Point2*)&pa[ii2];
        // printf("  bnd-seg[%d]=%d,%d\n",i3,ii1,ii2);
      i5 = UT2D_pt_int4pt (&p21, &d1, &d2, tol, p2e1, p2e2, p2s1, p2s2);
      // clear intersection: set intersectionPoint as new startpoint & goto
      // next segment; overlap: skip segment.
      if(i5 != 1) goto L_out_9;
      // set iFac=index of face with segment ii1-ii2
      if(eTab->data[i2].typ == MSH_EDGLN_IB) MEM_swap_int (&ii1, &ii2);
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
    UFA_if_findSegP (&iFac, &i5, ip1, -1, pe2, fa, fNr, pa);
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
    // GR_tDyn_symB__ (&pt1, SYM_STAR_S, 2);


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
    UFA_if_findSegP (&iFac, &i5, ip1, -1, pe2, fa, fNr, pa);
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
      DEB_dump_obj__ (Typ_PT, &pOut[i1], "  pOut[%d]:",i1);
      // GR_tDyn_symB__ (&pOut[i1], SYM_STAR_S, 2);
    }
*/

  *paOut = pOut;
  *paNr = outNr;

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


//================  EOF  ==================================
