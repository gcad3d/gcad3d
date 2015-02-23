//  read LandXML-Functions                  RF   2007-09-17
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
\file  ../exp/lxml.c
\brief read LandXML file 
\code
=====================================================
List_functions_start:

lxml_read        Load LandXML-File
lxml_r_pt        load & save point
lxml_r_savPt     save point pt1 > LM_pTab
lxml_r_fac       load & save a face
lxml_r_savFace

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


1) Alle Points (<Pnts>) einlesen und (binaer) in eine Datei ausgeben.
   OutFilename: M#A#.ptab

2) Alle Faces (<Faces>) einlesen und (binaer) in eine Datei ausgeben.
   OutFilename: M#A#.msh
           

==================================================================
Format LandXML:

<?xml version="1.0"?>
<LandXML xmlns ....>
..
<Surfaces>
  <Surface name="DGM1" desc="">
  <Definition surfType="TIN"
    area2DSurf="133628894.82049094" area3DSurf="142664100.79739568"
    elevMax="2533.840903081" elevMin="558.850769790297">
  <Pnts>
  <P id="1">-6351.233348619727 -1364.528376596871 1494.969407343888</P>
  <P id="2">-6288.107595301287 -1258.113074824959 1502.359063393669</P>
  </Pnts>
  <Faces>
  <F n="90 18 84">9971 10073 10072</F>
  <F n="8601 8593 8595">2526 2527 2628</F>
  </Faces>
  </Definition>
  </Surface>
  </Surfaces>
</LandXML>


==================================================================

\endcode *//*----------------------------------------


*/



#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



// #include "../ut/ut_umem.h"        // UME_reserve
#include "../ut/ut_geo.h"         // OFF, ON ..
#include "../ut/ut_msh.h"         // Fac3 ..
#include "../ut/ut_txt.h"         // fnam_del
#include "../ut/ut_memTab.h"          // MemTab_..
#include "../gr/ut_UI.h"          // SYM_..
#include "../xa/xa_mem.h"         // memspc..




typedef_MemTab(int);
typedef_MemTab(Point);
typedef_MemTab(Fac3);
typedef_MemTab(EdgeLine);



// PunkteTabelle
  // Point  *pTab;	
  // int    siz_pTab, nr_pTab;
static  MemTab(Point) LM_pTab = MemTab_empty;

// FaceTabelle
  // Fac3   *fTab = NULL;	
  // int    siz_fTab, nr_fTab;
static  MemTab(Fac3) LM_fTab = MemTab_empty;

static  MemTab(EdgeLine) LM_eTab = MemTab_empty;
static  MemTab(int) LM_eDat = MemTab_empty; 



#define inc_pTab  25000
#define inc_fTab  25000





//================================================================
  int lxml_read (char *fNam, char *mdlNam, int surNr) {
//================================================================
// Load LandXML-File

  int     lNr, ii, i1, i2, lSiz;
  double  d1;
  char    *lBuf, *cp1;
  FILE    *fp1;
  Vector2 vc1;
  Point2  p1, p2, p3;


  printf("lxml_read |%s|\n",fNam);

  MemTab_ini (&LM_pTab, sizeof(Point), Typ_PT, inc_pTab);
  MemTab_ini (&LM_fTab, sizeof(Fac3), Typ_Fac3, inc_fTab);
  MemTab_ini (&LM_eTab, sizeof(EdgeLine), Typ_EdgeLine, 5);
  MemTab_ini (&LM_eDat, sizeof(int), Typ_Int4, 5);



  // open xml-file
  if ((fp1 = fopen (fNam, "r")) == NULL) {
    printf ("Fehler open Datei %s\n", fNam);
    return -1; //exit(-1);
  }

  lBuf = IE_buf;
  lSiz = mem_cbuf1_SIZ;


  // test 1. Line
  if(fgets (lBuf, lSiz, fp1) == NULL) goto L_err1;
  if(strncmp(&lBuf[2], "xml", 3)) goto L_err1;


  // test 2. Line
  if(fgets (lBuf, lSiz, fp1) == NULL) goto L_err1;
  if(strncmp(&lBuf[1], "LandXML", 7)) goto L_err1;


  // attach objSeg
  // UME_init (&objSeg, memspc501, sizeof(memspc501));
  // UME_init (&tmpSeg, memspc201, sizeof(memspc201));


  // // memSpace for pTab
  // lxml_r_savPt (-1, NULL);



  //----------------------------------------------------------------
  // Alle Points (<Pnts>) einlesen und (binaer) in eine Datei ausgeben.


  // proceed until "<Pnts>"
  L_p1:
  if(fgets (lBuf, lSiz, fp1) == NULL) goto L_err2;
  cp1 = UTX_pos_skipDeli1 (lBuf);
    // printf("|%s|\n",cp1);
  if(strncmp(cp1, "<Pnts>", 6)) goto L_p1;


  // load all points
  L_p2:
  if(fgets (lBuf, lSiz, fp1) == NULL) goto L_err2;
  cp1 = UTX_pos_skipDeli1 (lBuf);
    // printf("|%s|\n",cp1);
  if(strncmp(cp1, "</Pnts>", 7)) {
    lxml_r_pt (lBuf);                  // read & save point
    goto L_p2;
  }

  // close pTab
  // ii = lxml_r_savPt (2, NULL);
    printf(" got %d points\n",LM_pTab.rNr);


  // TEST:
  // for(i1=70; i1<80; ++i1)
    // UT3D_stru_dump (Typ_PT, &LM_pTab.data[i1], " _r_savPt1 %d",i1);





  //----------------------------------------------------------------
  // Alle Faces (<Faces>) einlesen und (binaer) in eine Datei ausgeben.

  // proceed until "<Faces>"
  L_f1:
  if(fgets (lBuf, lSiz, fp1) == NULL) goto L_err3;
  cp1 = UTX_pos_skipDeli1 (lBuf);
    // printf("|%s|\n",cp1);
  if(strncmp(cp1, "<Faces>", 7)) goto L_f1;


  // // init fTab (give memSpace)
  // lxml_r_savFace (0, NULL);


  // load all Faces
  L_f2:
  if(fgets (lBuf, lSiz, fp1) == NULL) goto L_err3;
  cp1 = UTX_pos_skipDeli1 (lBuf);
    // printf("|%s|\n",cp1);
  if(strncmp(cp1, "</Faces>", 8)) {
    lxml_r_fac (lBuf);                   // read & save face
    goto L_f2;
  }


  // // close fTab (fix memSpace)
  // lxml_r_savFace (2, NULL);
    printf(" got %d faces\n",LM_fTab.rNr);


  // close xml
  fclose(fp1);


  // // TEST:
  // for(i1=70; i1<80; ++i1)
    // UT3D_stru_dump (Typ_PT, &LM_pTab.data[i1], " _r_savPt2 %d",i1);
    // UT3D_stru_dump (Typ_PT, &LM_pTab.data[77], " P77");



  //----------------------------------------------------------------
  // delete all undefined points
  i2 = 0;
  for(i1=LM_pTab.rNr-1;  i1>=0; --i1) {
    if(LM_pTab.data[i1].x != UT_VAL_MAX) continue;
      // printf(" lxml-del.pt %d\n",i1);
    ++i2;

    // delete point
    MemTab_del (&LM_pTab, i1, 1);

    // change all face-pointers >= i1 to i1-1
    MSH_FA_mod_delPt (&LM_fTab, i1);
  }
  printf("  %d undefined points deleted\n",i2);


  //----------------------------------------------------------------
  // write OutFile M#A#.ptab
  // MSH_bsav_pTab (&LM_pTab, mdlNr, surNr);
  MSH_bsav_pTab (&LM_pTab, mdlNam, surNr);



  //----------------------------------------------------------------
  // write OutFile M#A#.msh
  ++surNr;
  // MSH_bsav_fTab (&LM_fTab, &LM_eTab, &LM_eDat, mdlNr, surNr);
  MSH_bsav_fTab (&LM_fTab, &LM_eTab, &LM_eDat, mdlNam, surNr);



  //----------------------------------------------------------------
  // fix modelsize from points
  UT2D_rect_pta3 (&p1, &p2, LM_pTab.data, LM_pTab.rNr);
  UT2D_vc_2pt (&vc1, &p1, &p2);
  d1 = UT2D_len_vc (&vc1);

  UT2D_pt_mid2pt (&p3, &p1, &p2);
  if(p3.x > d1) d1 = p3.x;
  if(p3.y > d1) d1 = p3.y;

  d1 = UTP_db_rnd5 (d1);
  if(d1 > 50000.) {
    d1 = 50000.;
    TX_Print ("Modeloffset high; offset should be reduced ..");
  }

  NC_setModSiz (d1);                 // was 10000.;



  //----------------------------------------------------------------
  // fertig.
  TX_Print ("%d points, %d faces imported ..",LM_pTab.rNr,LM_fTab.rNr);


  // free(pTab);
  MemTab_free (&LM_fTab);
  MemTab_free (&LM_pTab);
  MemTab_free (&LM_eTab);
  MemTab_free (&LM_eDat);





  return 0;


  L_err1:
  TX_Print("%s is not LandXML ..",fNam);
  goto L_err99;

  L_err2:
  TX_Print("no points in LandXML ..");
  goto L_err99;

  L_err3:
  TX_Print("no faces in LandXML ..");

  L_err99:
  fclose(fp1);
  return -1;

}


//================================================================
  int lxml_r_pt (char *lBuf) {
//================================================================
// load & save a point > pt1; call lxml_r_savPt
// <P id="10201">8890.855277389255 4003.707113415382 1304.31791295996</P>

  int     id;
  char    *cp1, *cp2, *cp3;
  Point   pt1;

  cp1 = lBuf;

  // decode ID
  cp1 = strchr(lBuf, '"');
  ++cp1;
  id = atoi(cp1);
    // printf("%d \n",id);


  // proceed to '>'    (skipping "<P id="10201">")
  while(*cp1 != '>') ++cp1;
  ++cp1;   // skip also '>'

  // decode pointCoords ..
  pt1.x = strtod(cp1, &cp2);
  pt1.y = strtod(cp2, &cp3);
  pt1.z = strtod(cp3, &cp1);
    // printf(" pt1=%f/%f/%f\n",pt1.x,pt1.y,pt1.z);

  return lxml_r_savPt (id, &pt1);

}


//=======================================================================
  int lxml_r_savPt (int id, Point *pt1) {
//=======================================================================
// save point pt1 > LM_pTab
// id = 0   Init; load pTab ...
// id = >0   add a point to pTab

  int     irc, i1, i2, iOld;
  long    ld;


  // UT3D_stru_dump (Typ_PT, pt1, "lxml_r_savPt %d",id);


/*
  // Init-function
  if(id < 0) {
    siz_pTab = inc_pTab;
    pTab = (Point*) malloc(siz_pTab * sizeof(Point));
    if(pTab == NULL) goto L_EOM;
    nr_pTab = 0;
    return 0;
  }

  L_ckSiz:
  if(id >= siz_pTab) {
    siz_pTab += inc_pTab;
    pTab = (Point*) realloc(pTab, siz_pTab * sizeof(Point));
    if(pTab == NULL) goto L_EOM;
    goto L_ckSiz;
  }

  // copy point
  pTab[id-1] = *pt1;
  nr_pTab = id;
*/

  // we need space for <id> points
  i1 = id - LM_pTab.rNr;

    // TESTONLY:
    // if(id == 79) printf(" L1: i1=%d rNr=%d\n",i1,LM_pTab.rNr);

  if(i1 > 0) {
    iOld = LM_pTab.rNr;
    // irc = MemTab_sav (&LM_pTab, &ld, NULL, i1);
    irc = MemTab_add (&LM_pTab, &ld, NULL, i1, 1);  // 2014-06-23
    if(irc < 0) goto L_EOM;
    LM_pTab.rNr = id;
    // preset points with empty point
    if(i1 > 1) {
      for(i2=iOld; i2<id; ++i2) {
        LM_pTab.data[i2].x = UT_VAL_MAX;
      }
    }
  }

    // TESTONLY:
    // if(id == 79) printf(" L2: i1=%d rNr=%d\n",i1,LM_pTab.rNr);

  LM_pTab.data[id-1] = *pt1;


  return 0;

  L_EOM:
    printf("lxml_r_savPt EOM\n");
    return -1;


}


//================================================================
  int lxml_r_fac (char *lBuf) {
//================================================================
// load & save a face
// <F n="90 18 84">9971 10073 10072</F>
// <F i="1" n="6852 6840 6846">399 7913 391</F>
// skip all lines with i="#" 
// die Facepointer sind 399 7913 391; also die 3 Zahlen nach ">"


  char    *cp1, *cp2, *cp3;
  Fac3    fc1;

  // cp1 = lBuf;

  // printf("lxml_r_fac |%s|\n",lBuf);

  cp1 = strchr(lBuf, 'F');
  if(cp1 == NULL) return -1;
  if(cp1[2] == 'i') {
    // printf(" skip 1\n");
    return 0;
  }


  // proceed to '>'    (skipping "<F n="90 18 84">")
  // while(*cp1 != '>') ++cp1;
  cp1 = strchr(lBuf, '>');
  if(cp1 == NULL) return -1;
  ++cp1;   // skip also '>'
  // decode FaceIndexNrs
  fc1.i1 = strtol(cp1, &cp2, 10) - 1;
  fc1.i2 = strtol(cp2, &cp3, 10) - 1;
  fc1.i3 = strtol(cp3, &cp1, 10) - 1;
    // printf(" fac1=%d/%d/%d\n",fc1.i1,fc1.i2,fc1.i3);


  return lxml_r_savFace (1, &fc1);

}


//=======================================================================
  int lxml_r_savFace (int mode, Fac3 *fc1) {
//=======================================================================
// mode = 0   Init; load pTab ...
// mode = 1   add a Face to fTab
// mode = 2   close; free the rest of the memspace (for faces ..)


  int        irc;
  long       ld;
  Point      *pa;

/*
  // Init-function
  if(mode == 0) {
    // memspc pTab uebernehmen ..
    nr_fTab = 0;
    fTab = (void*)pTab;     // face-table
    siz_fTab = siz_pTab * sizeof(Point) / sizeof(Fac3);
      printf(" siz_fTab=%d\n",siz_fTab);
    return 0;
  }

  // copy face
  if(nr_fTab >= siz_fTab) {
    siz_fTab += inc_fTab;
    fTab = (Fac3*) realloc(fTab, siz_fTab * sizeof(Fac3));
      printf("lxml_r_savFace realloc %d\n",siz_fTab);
    if(fTab == NULL) goto L_EOM;
  }

  fTab[nr_fTab] = *fc1;
    // printf(" fac[%d]= %d %d %d\n",nr_fTab,fTab[nr_fTab].i2,
            // fTab[nr_fTab].i2,fTab[nr_fTab].i3);
  ++nr_fTab;
*/


  // test if all points of face exist  (pt.x == UT_VAL_MAX)
  pa = LM_pTab.data;


    // TESTONLY:
    // if((fc1->i1 == 77)||(fc1->i2 == 77)||(fc1->i3 == 77)) {
      // printf(" face-%d-%d-%d\n",fc1->i1,fc1->i2,fc1->i3);
      // UT3D_stru_dump (Typ_PT, &pa[78], " P78");
    // }



  if(pa[fc1->i1].x == 0.) {irc = fc1->i1; goto L_Err1;}
  if(pa[fc1->i2].x == 0.) {irc = fc1->i2; goto L_Err1;}
  if(pa[fc1->i3].x == 0.) {irc = fc1->i3; goto L_Err1;}


  irc = MemTab_sav (&LM_fTab, &ld, fc1, 1);
  if(irc < 0) goto L_EOM;


  return 0;


  L_EOM:
    printf("lxml_r_savFace EOM\n");
    return -1;

  L_Err1:
    TX_Print("remove Face with non-existing point %d\n",irc);
    printf(" face-%d-%d-%d\n",fc1->i1,fc1->i2,fc1->i3);
    return 0;


}


//============== EOF ===============
