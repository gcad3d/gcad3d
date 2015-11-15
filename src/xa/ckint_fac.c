// intersect faces,edges UT2D_ckint_              RF
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
\file  ../xa/ckint_fac.c
\brief intersect faces,edges UT2D_ckint_
\code
=====================================================
List_functions_start:

UT2D_ckint_FacFac          check if 2 Faces intersect
UT2D_ckint_EdgFac_         check if Edge intersects Face
UT2D_ckint_EdgFac1         -
UT2D_ckint_EdgFac2         -

List_functions_end:
=====================================================

\endcode *//*----------------------------------------

*/





#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#include "../ut/ut_geo.h"              // Point ...


  static int s1,s2,s3;


//================================================================
  int UT2D_ckint_EdgFac_ (Point2 *f1, Point2 *f2, Point2 *f3,
                          Point2 *e1, Point2 *e2) {
//================================================================
/// \code
/// UT2D_ck_intEdgFac_          check if Edge intersects Face
/// Face must be oriented CCW !
/// if Edge touches Face: no intersection !
/// if Edge lies on FaceEdge: is intersection !
/// Identical Points werden nicht getestet ! (vorher machen)
/// 
/// RetCod:
///   0: nein, Edge e1-e2 schneidet Face f1-f2-f3 NICHT
///   1: ja, Edge e1-e2 beruehrt eines der FaceEdges
///   2: ja, Edge e1-e2 liegt genau auf einem FaceEdge
///   3: ja, Edge e1-e2 liegt inside Face f1-f2-f3
/// \endcode


  int      i1,i2, i11,i12, i21,i22, i31,i32, irc;
  Vector2  v1, v2, v3;
  

  printf("UT2D_ckint_EdgFac_\n");
  // GR_Disp_ln2 (f1, f2, 5);  GR_Disp_txi2 (f1, 1, 0);
  // GR_Disp_ln2 (f2, f3, 5);  GR_Disp_txi2 (f2, 2, 0);
  // GR_Disp_ln2 (f3, f1, 5);  GR_Disp_txi2 (f3, 3, 0);
  // GR_Disp_ln2 (e1, e2, 8);

  
  UT2D_vc_2pt (&v1, f1, f2);
  i11 = UT2D_sid_ptvc (e1, f1, &v1);
  i12 = UT2D_sid_ptvc (e2, f1, &v1);

  UT2D_vc_2pt (&v2, f2, f3);
  i21 = UT2D_sid_ptvc (e1, f2, &v2);
  i22 = UT2D_sid_ptvc (e2, f2, &v2);

  UT2D_vc_2pt (&v3, f3, f1);
  i31 = UT2D_sid_ptvc (e1, f3, &v3);
  i32 = UT2D_sid_ptvc (e2, f3, &v3);

  // allg. Tests
  irc = UT2D_ckint_EdgFac1 (&i11,&i12, &i21,&i22, &i31,&i32);
  // irc=3: intersect is possible; test exact.
  if(irc == 3) irc = UT2D_ckint_EdgFac2 (f1, f2, f3, e1, e2);

  return irc;

}

//================================================================
  int UT2D_ckint_EdgFac2 (Point2 *f1, Point2 *f2, Point2 *f3,
                          Point2 *e1, Point2 *e2) {
//================================================================
// eine Intersection ist moeglich; mit allen FaceEdges, die Edge e1-e2 queren.
// Queren: die Punkte liegen auf unterschiedlichen Seiten (s=0)

  int      i1, i2;
  Vector2  v1;


  UT2D_vc_2pt (&v1, e1, e2);

  if(s1 == 0) {                    // test if e1-e2  X f1-f2 intersects
    i1 = UT2D_sid_ptvc (f1, e1, &v1);
    i2 = UT2D_sid_ptvc (f2, e1, &v1);

  } else if(s2 == 0) {            // test if e1-e2  X f2-f3 intersects
    i1 = UT2D_sid_ptvc (f2, e1, &v1);
    i2 = UT2D_sid_ptvc (f3, e1, &v1);

  } else {                        // test if e1-e2  X f3-f1 intersects
    i1 = UT2D_sid_ptvc (f3, e1, &v1);
    i2 = UT2D_sid_ptvc (f1, e1, &v1);
  }
    printf(" _ckint_ i1=%d i2=%d\n",i1,i2);


  // wenn einer davon 0 ist es ein touch ..
  if((i1 == 0)||(i2 == 0)) return 1;

  // gleiche Seite: outside ..
  if(i1 == i2) return 0;

  // verschiedene Seiten: Intersection!
  return 3;

}


//================================================================
  int UT2D_ckint_EdgFac1 (int *i11, int *i12,
                          int *i21, int *i22,
                          int *i31, int *i32) {
//================================================================
// do the checks ..
// see UT2D_ck_intEdgFac_

  int      s1,s2,s3, irc;


  printf("UT2D_ckint_EdgFac1\n");
  printf(" i11=%2d i12=%2d\n",*i11,*i12);
  printf(" i21=%2d i22=%2d\n",*i21,*i22);
  printf(" i31=%2d i32=%2d\n",*i31,*i32);

  // i11 = e1 links oder rechts von f1-f2; i12 = e2 links oder rechts von f1-f2
  // i21 = e1 links oder rechts von f2-f3; i22 = e2 links oder rechts von f2-f3


  // Test e1 und e2 ganz ausserhalb einer FaceEdge: irc=0
  //   rechts rechts (-1 -1); summe muss also -2 sein.
  if((*i11 + *i12) == -2) return 0;
  if((*i21 + *i22) == -2) return 0;
  if((*i31 + *i32) == -2) return 0;


  // Testen, ob ein Punkt innen ist; ja: rc=3
  //   innen = links von allen FaceEdges ( 3 X 1 )
  if((*i11 == 1)&&(*i21 == 1)&&(*i31 == 1)) return 3;
  if((*i12 == 1)&&(*i22 == 1)&&(*i32 == 1)) return 3;


  if((*i11 >= 0)&&(*i12 >= 0)) s1 = 1;
  else if((*i11 <= 0)&&(*i12 <= 0)) s1 = -1;
  else s1 = 0;

  if((*i21 >= 0)&&(*i22 >= 0)) s2 = 1;
  else if((*i21 <= 0)&&(*i22 <= 0)) s2 = -1;
  else s2 = 0;

  if((*i31 >= 0)&&(*i32 >= 0)) s3 = 1;
  else if((*i31 <= 0)&&(*i32 <= 0)) s3 = -1;
  else s3 = 0;

  // s1 = 1:  e1 und e2 auf oder innerhalb f1-f2
  // s1 = -1: e1 und e2 auf oder ausserhalb f1-f2
  // s1 = 0:  e1 und e2 auf verschiedenen Seiten von f1-f2
    // printf(" s1=%2d s2=%2d s3=%2d\n",s1,s2,s3);


  // Test touch:
  // - der Beruehrpunkt(e1 od e2) muss auf der Strecke liegen;
  //   also drauf, links und links (0 1  1); summe muss also 2 sein.
  // - e1 u e2 muessen auf FaceEdge od aussen sein (s muss neg sein)

  irc = 0;

  if((*i11 == 0)||(*i12 == 0)) {     // ein Edgepoint liegt genau auf f1-f2
    if((*i11 == 0)&&(*i12 == 0)) {   // edge genau auf f1-f2
      if((s2 + s3) == 0) return 0;    // beide innen + beide aussen
      return 2;
    }
    if(s1 < 0) {        // e1 & e2 aussen
      if(*i11 == 0) {                   // nur e1 liegt auf f1-f2
        if((*i21 + *i31) == 2) irc = 1;     // test if outside faceEdge
      } else {                         // nur e2 liegt auf f1-f2
        if((*i22 + *i32) == 2) irc = 1;     // test if outside faceEdge
      }
    }
  }


  if((*i21 == 0)||(*i22 == 0)) {     // ein Edgepoint liegt genau auf f2-f3
    if((*i21 == 0)&&(*i22 == 0)) {   // edge genau auf f2-f3
      if((s1 + s3) == 0) return 0;
      return 2;
    }
    if(s2 < 0) {        // e1 & e2 aussen
      if(*i21 == 0) {                   // nur e1 liegt auf f2-f3
        if((*i11 + *i31) == 2) irc = 1;     // test if outside faceEdge
      } else {                         // nur e2 liegt auf f2-f3
        if((*i12 + *i32) == 2) irc = 1;     // test if outside faceEdge
      }
    }
  }

  if((*i31 == 0)||(*i32 == 0)) {     // ein Edgepoint liegt genau auf f3-f1
    if((*i31 == 0)&&(*i32 == 0)) {   // edge genau auf f3-f1
      if((s1 + s2) == 0) return 0;
      return 2;
    }
    if(s3 < 0) {        // e1 & e2 aussen
      if(*i31 == 0) {                   // nur e1 liegt auf f3-f1
        if((*i11 + *i21) == 2) irc = 1;     // test if outside faceEdge
      } else {                         // nur e2 liegt auf f3-f1
        if((*i12 + *i22) == 2) irc = 1;     // test if outside faceEdge
      }
    }
  }

  
  if((s1 != 0)&&(s1 + s2 == 0)) return irc;
  if((s2 != 0)&&(s2 + s3 == 0)) return irc;
  if((s3 != 0)&&(s3 + s1 == 0)) return irc;


  // eine Intersection ist moeglich.
  return 3;

}


//================================================================
  int UT2D_ckint_FacFac (Point2 *a1, Point2 *a2, Point2 *a3,
                         Point2 *b1, Point2 *b2, Point2 *b3) {
//================================================================
/// \code
/// UT2D_ck_intEdgFac           check if Edge intersects Face
/// Face must be oriented CCW !
/// if Edge touches Face: no intersection !
/// if Edge lies on FaceEdge: is intersection !
/// Identical Points werden nicht getestet ! (vorher machen)
/// 
/// RetCod:
///   0: nein, Face b schneidet Face a NICHT
///   1: ja, Face b beruehrt eines der FaceEdges
///   2: ja, Face b liegt genau auf einem FaceEdge
///   3: ja, Face b liegt inside Face a
/// \endcode


  int      i11,i12,i13, i21,i22,i23, i31,i32,i33, r1,r2,r3, irc;
  Vector2  v1, v2, v3;

  printf("UT2D_ckint_EdgFac \n");
  GR_Disp_ln2 (a1, a2, 1);  GR_Disp_txi2 (a1, 1, 0);
  GR_Disp_ln2 (a2, a3, 1);  GR_Disp_txi2 (a2, 2, 0);
  GR_Disp_ln2 (a3, a1, 1);  GR_Disp_txi2 (a3, 3, 0);
  GR_Disp_ln2 (b1, b2, 2);  GR_Disp_txi2 (b1, 1, 0);
  GR_Disp_ln2 (b2, b3, 2);  GR_Disp_txi2 (b2, 2, 0);
  GR_Disp_ln2 (b3, b1, 2);  GR_Disp_txi2 (b3, 3, 0);



  UT2D_vc_2pt (&v1, a1, a2);
  i11 = UT2D_sid_ptvc (b1, a1, &v1);
  i12 = UT2D_sid_ptvc (b2, a1, &v1);
  i13 = UT2D_sid_ptvc (b3, a1, &v1);

  UT2D_vc_2pt (&v2, a2, a3);
  i21 = UT2D_sid_ptvc (b1, a2, &v2);
  i22 = UT2D_sid_ptvc (b2, a2, &v2);
  i23 = UT2D_sid_ptvc (b3, a2, &v2);

  UT2D_vc_2pt (&v3, a3, a1);
  i31 = UT2D_sid_ptvc (b1, a3, &v3);
  i32 = UT2D_sid_ptvc (b2, a3, &v3);
  i33 = UT2D_sid_ptvc (b3, a3, &v3);
    printf(" i11=%2d i12=%2d i13=%2d\n",i11,i12,i13);
    printf(" i21=%2d i22=%2d i23=%2d\n",i21,i22,i23);
    printf(" i31=%2d i32=%2d i33=%2d\n",i31,i32,i33);


  // wenn alle b-punkte ausserhalb eines a-Edge liegen: irc=0
  //  alle rechts = 3 X -1
  if((i11 + i12 + i13) == -3) return 0;
  if((i21 + i22 + i23) == -3) return 0;
  if((i31 + i32 + i33) == -3) return 0;


  // test Face a with edge b1-b2
  r1 = UT2D_ckint_EdgFac1 (&i11,&i12, &i21,&i22, &i31,&i32);
  if(r1 == 3) r1 = UT2D_ckint_EdgFac2 (a1, a2, a3, b1, b2);
  if(r1 > 0) return r1;


  // test Face a with edge b2-b3
  r2 = UT2D_ckint_EdgFac1 (&i12,&i13, &i22,&i23, &i32,&i33);
  if(r2 == 3) r1 = UT2D_ckint_EdgFac2 (a1, a2, a3, b2, b3);
  if(r2 > 0) return r1;


  // test Face a with edge b3-b1
  r3 = UT2D_ckint_EdgFac1 (&i13,&i11, &i23,&i21, &i33,&i31);
  if(r3 == 3) r1 = UT2D_ckint_EdgFac2 (a1, a2, a3, b3, b1);
  if(r3 > 0) return r1;


    printf(" r1=%2d r2=%2d r3=%2d\n",r1,r2,r3);



  return 3;

}

//================================================================
//================================================================
// Liste_TESTFUNKTIONEN:
//================================================================
//================================================================


#ifdef DEB


//================================================================
  int UT2D_ckint_Test1 () {
//================================================================
/// Testdaten zu UT2D_ckint_EdgFac:


  Point2  p1={0,0}, p2={10,0}, p3={0,10};

  // Point2  e1={6,6}; Point2  e2={15,15}; //0
  // Point2  e1={6,6}; Point2  e2={25,-15}; //0
  // Point2  e1={6,6}; Point2  e2={-2,12}; //0
  // Point2  e1={6,6}; Point2  e2={0,11}; //0
  // Point2  e1={6,6}; Point2  e2={25.1,-15}; //0
  // Point2  e1={11,-1}; Point2  e2={13,-3}; //0
  // Point2  e1={10,-5}; Point2  e2={20,10}; //0
  // Point2  e1={20,10}; Point2  e2={10,-5}; //0
  Point2  e1={1,-2}; Point2  e2={-2,1}; //0

  // Point2  e1={5,5}; Point2  e2={15,15}; //1
  // Point2  e1={5,5}; Point2  e2={111,-5}; //1
  // Point2  e1={10,0}; Point2  e2={11,-1}; //1    ist leider 0

  // Point2  e1={5,5}; Point2  e2={10,0}; //2
  // Point2  e1={5,5}; Point2  e2={9,1}; //2
  // Point2  e1={5,5}; Point2  e2={11,-1};  //2

  // Point2  e1={9.9,0}; Point2  e2={-1,25}; //3
  // Point2  e1={5,5}; Point2  e2={5,0}; //3
  // Point2  e1={0,5}; Point2  e2={111,-5}; //3
  // Point2  e1={6,6}; Point2  e2={15,-15}; //3
  // Point2  e1={3,4}; Point2  e2={15,-15}; //3
  // Point2  e1={3,4}; Point2  e2={5,5}; //3
  // Point2  e1={-5,20}; Point2  e2={15,-15}; //3
  // Point2  e1={11,-1}; Point2  e2={13,-1}; //3
  // Point2  e1={1,1}; Point2  e2={-2,3}; //3      test 0
  // Point2  e1={3,-2}; Point2  e2={-2,3}; //3      test 0


  printf("UT2D_ckint_EdgFac=%d\n",UT2D_ckint_EdgFac_(&p1, &p2, &p3, &e1, &e2));

  return 0;

}

//================================================================
  int UT2D_ckint_Test2 () {
//================================================================
/// Testdaten zu UT2D_ckint_FacFac:


  Point2  p1={0,0}, p2={10,0}, p3={0,10};

  // Point2  p4={20,0}, p5={30,0}, p6={20,10}; //0
  // Point2  p4={10,-5}, p5={30,0}, p6={20,10}; //0

  Point2  p4={0,-5}, p5={20,0}, p6={10,10}; //0

  printf("_ckint_FacFac=%d\n",UT2D_ckint_FacFac (&p1,&p2,&p3, &p4,&p5,&p6));

  return 0;

}

#endif

// EOF
