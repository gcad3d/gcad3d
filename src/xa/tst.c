// make -f tst.mak
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
\file  ../APP/tst.c
\brief plugin tests
\code
=====================================================
List_functions_start:
..
List_functions_end:
=====================================================

\endcode *//*----------------------------------------

make -f tst.mak
nmake -f tst.nmak


gCad_main
gCad_fini
tst_sel_CB


*/



#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                         // for ...


#ifdef _MSC_VER
// die folgenden 2 Funktionen exportieren (werden vom Main gerufen):
__declspec(dllexport) int gCad_main ();
__declspec(dllexport) int gCad_fini ();
// nachfolgende externals werden aus dem Main-Exe imported:
#define extern __declspec(dllimport)
#endif



#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_os.h"               // OS_get_bas_dir ..
#include "../ut/ut_obj.h"              // UTO_stru_2_obj
#include "../ut/ut_TX.h"               // TX_Print
#include "../db/ut_DB.h"               // DB_get_..
#include "../gr/ut_UI.h"               // SYM_..
#include "../gui/gui__.h"              // GUI_

#include "../xa/xa_msg.h"              // MSG_*



// Externals aus ../ci/NC_Main.c:
extern double    APT_ModSiz;



// protos:
  int tst_sel_CB (int src, long dl_ind);
  int tst_key_CB (int key);







/*
////////////////////////////////////////////////////////////////////////


#include "../xa/xa_mem.h"      // memspc..



//===========================================================================
// EXTERNALS:
// from ../xa/xa.c:

extern double     AP_txsiz;       // Notes-Defaultsize
extern double     AP_txdimsiz;    // Dimensions-Text-size



//===========================================================================
// LOCALS:


static int       dxf_version;     // 0=R12; 1=2000





//================================================================
  int dxfw_ELLIPSE (CurvElli *cv1, FILE *fp_in) {
//================================================================
// see ../formate/dxf/doc/DXF2000_ellipse.txt

  double     d1;

  printf("dxfw_ELLIPSE \n");
  UT3D_stru_dump (Typ_CVELL, cv1, "cv1:");


  fprintf(fp_in,"0\nELLIPSE\n");

  // 10,20,30 = centerpoint
  dxfw_point (0, &cv1->pc, fp_in);

  // 11,21,31 = Endpoint of major axis, relative to the center.
  dxfw_fl_out (11, cv1->va.dx, fp_in);
  dxfw_fl_out (21, cv1->va.dy, fp_in);
  dxfw_fl_out (31, cv1->va.dz, fp_in);

  // 210,220,230 = Extrusion direction
  dxfw_vector (&cv1->vz, fp_in);

  // 40 = Ratio of minor axis to major axis
  d1 = UT3D_len_vc(&cv1->vb) / UT3D_len_vc(&cv1->va);
  dxfw_fl_out (40, d1, fp_in);

  // 41 = Start parameter (this value is 0.0 for a full ellipse)
  // 42 = End parameter (this value is 2pi for a full ellipse)
  if(UT3D_comp2pt(&cv1->p1, &cv1->p2, UT_TOL_pt) != 0) {
    dxfw_fl_out (41, 0., fp_in);
    dxfw_fl_out (42, RAD_360, fp_in);

  } else {
    // parametric-Angle <- pt
    d1 = UT3D_angr_elpt (&cv1->p1, &cv1->pc, &cv1->va, &cv1->vb);
    dxfw_fl_out (41, d1, fp_in);
    d1 = UT3D_angr_elpt (&cv1->p2, &cv1->pc, &cv1->va, &cv1->vb);
    dxfw_fl_out (42, d1, fp_in);

  }

  return 0;

}


//================================================================
  int dxfw_SPLINE (CurvBSpl *cv1, FILE *fp_in) {
//================================================================
// see ../formate/dxf/doc/DXF2000_spline.txt

  int      i1, i2;

  // 5 8 62 370


  printf("dxfw_SPLINE \n");
  // UT3D_stru_dump (Typ_CVBSP, cv1, "cv1:");

  fprintf(fp_in,"0\nSPLINE\n");


  // 210,220,230 Normal vector (omitted if the spline is nonplanar)



  // 70 = Spline flag (bit coded):
  //   1 = Closed spline
  //   2 = Periodic spline
  //   4 = Rational spline
  //   8 = Planar
  //   16 = Linear (planar bit is also set)


  // 71 = deg
  fprintf(fp_in,"71\n%d\n",cv1->deg);

  // 72 = Number of knots
  i2 = cv1->ptNr + cv1->deg + 1;
  fprintf(fp_in,"72\n%d\n",i2);

  // 73 = Number of control points
  fprintf(fp_in,"73\n%d\n",cv1->ptNr);

  // 40 = Knot value
  for(i1=0; i1<i2; ++i1) dxfw_fl_out (40, cv1->kvTab[i1], fp_in);

  // export points  10,20,30
  i2 = cv1->ptNr;
  for(i1=0; i1<i2; ++i1) dxfw_point (0, &cv1->cpTab[i1], fp_in);

  return 0;

}


//================================================================
  int dxfw_VERTEX (int typ, Point *pti, FILE *fp_in) {
//================================================================
// typ: 8=Spline, 32=Polyline


  fprintf(fp_in,"0\nVERTEX\n");
  fprintf(fp_in,"8\n0\n");            // Layer
  dxfw_point (0, pti, fp_in);
  fprintf(fp_in,"70\n%d\n",typ);         // TypVertex;

  return 0;

}


//================================================================
  int dxfw_VERTEX2 (int typ, Point2 *pti, FILE *fp_in) {
//================================================================
// typ: 8=Spline, 32=Polyline


  fprintf(fp_in,"0\nVERTEX\n");
  fprintf(fp_in,"8\n0\n");               // Layer
  dxfw_point2 (0, pti, fp_in);
  fprintf(fp_in,"70\n%d\n",typ);         // TypVertex;

  return 0;

}


//================================================================
  int dxfw_hd_POLYLINE (FILE *fp_in) {
//================================================================

        fprintf(fp_in,"0\nPOLYLINE\n");
        fprintf(fp_in,"8\n0\n");    // def.Layer
        fprintf(fp_in,"66\n1\n");   // indicates that VERT's will follow
        dxfw_point (0, (Point*)&UT3D_PT_NUL, fp_in);  // Nullpunkt
        fprintf(fp_in,"70\n8\n");   // bit(8)   3D-Polygon

  return 0;

}


//================================================================
  int dxfw_POLYLINE (int pNr, int typ, Point *pTab, FILE *fp_in) {
//================================================================
// typ: 8=Spline, 32=Polyline

  int   i1;


  dxfw_hd_POLYLINE (fp_in); // wr header of POLYLINE


  for(i1=0;i1<pNr; ++i1) {
    // printf(" dxfw-p[%d]=%f %f %f\n",i1,pTab[i1].x,pTab[i1].y,pTab[i1].z);
    dxfw_VERTEX (typ, &pTab[i1], fp_in);
  }

  fprintf(fp_in,"0\nSEQEND\n");  // end of VERT's after 66/1

  return 0;

}


//=====================================================================
  int DXFW_CI (Circ *ci1, FILE *fp_in) {
//=====================================================================


  int      zparl;
  double   d1, d2, d3;
  Point2   pt2c, pt21;
  Point    ptc, pt1, pt2;
  Mat_4x3  m1, im1;


      // Feststellen, ob es ein Vollkreis ist
      if(!UT3D_comp2pt(&ci1->p1, &ci1->p2, UT_TOL_pt)) goto L_AC;



      //========== es ist ein Vollkreis =======================
      fprintf(fp_in,"0\nCIRCLE\n");
      fprintf(fp_in,"8\n0\n"); // def.Layer
      //TX_Print("    pc=%f,%f,%f",ci1->pc.x,ci1->pc.y,ci1->pc.z);
      //TX_Print("    vz=%f,%f,%f",ci1->vz.dx,ci1->vz.dy,ci1->vz.dz);


      // Feststellen ob die Achse in +Z oder in -Z geht
      // ist vz parallel zum Z-Vektor -
      //if(!((UTP_comp2db(ci1->vz.dx,0.,UT_TOL_pt))   &&
      //     (UTP_comp2db(ci1->vz.dy,0.,UT_TOL_pt))))   {
      if((ci1->vz.dz > 1.-UT_TOL_min1)||(ci1->vz.dz < -1.+UT_TOL_min1)) {
        zparl = ON;

        // Ausgeben eines X-Y-parallelen Vollkreises.
        pt1=ci1->pc;
        goto L_CI_out;
      }


      // schiefer Vollkreis
      zparl = OFF;
      UT3D_vc_normalize (&ci1->vz, &ci1->vz);
      dxfw_load_mat (m1, &ci1->vz);  // eine TrMat nach DXF-Konvention generieren
      UT3D_m3_invm3 (im1, m1);      // RücktransformationsMat. generieren
      UT3D_pt_traptm3 (&pt1, im1, &ci1->pc);
      //TX_Print(" pt1=%f,%f,%f",pt1.x,pt1.y,pt1.z);


      //Ausgabe Vollkreis
      L_CI_out:
      dxfw_point (0, &pt1, fp_in);                  // Mittelpunkt als 10,20,30
      fprintf(fp_in,"40\n%f\n",fabs(ci1->rad));      // Radius 
      if(zparl==OFF) dxfw_vector (&ci1->vz, fp_in);
      return 0;





      //========== es ist ein Kreisbogen =======================
      L_AC:

      fprintf(fp_in,"0\nARC\n");
      fprintf(fp_in,"8\n0\n"); // def.Layer


      // Arc umdrehen, wenn er genau in -Z zeigt.
      // wenn vz = 0,0,-1 dann vz und p1/p2 umdrehen.
      //if((UTP_comp2db(ci1->vz.dx, 0.0, UT_TOL_pt))  &&
      //   (UTP_comp2db(ci1->vz.dy, 0.0, UT_TOL_pt))  &&
      //   (ci1->vz.dz < 0.0))                 {
      if(ci1->vz.dz < -1.+UT_TOL_min1) {
        //TX_Print("umdrehen");
        //pt1=ci1->p1; ci1->p1=ci1->p2; ci1->p2=pt1;
        ci1->rad = -ci1->rad;
        UT3D_vc_invert(&ci1->vz, &ci1->vz);

      }


      // Feststellen ob die Achse in +Z geht
      // ist vz parallel zum Z-Vektor -
      //if(!((UTP_comp2db(ci1->vz.dx,0.,UT_TOL_pt))   &&
      //     (UTP_comp2db(ci1->vz.dy,0.,UT_TOL_pt))))   goto L_AC_schief;
      if(ci1->vz.dz > 1.-UT_TOL_min1) {
        zparl = ON;
        ptc=ci1->pc;
        pt1=ci1->p1;
        pt2=ci1->p2;
        goto L_AC_out;
      }




      L_AC_schief:
      zparl = OFF;
      UT3D_vc_normalize (&ci1->vz, &ci1->vz);
      dxfw_load_mat (m1, &ci1->vz);  // eine TrMat nach DXF-Konvention generieren
      UT3D_m3_invm3 (im1, m1);      // RücktransformationsMat. generieren
      UT3D_pt_traptm3 (&ptc, im1, &ci1->pc);
      UT3D_pt_traptm3 (&pt1, im1, &ci1->p1);
      UT3D_pt_traptm3 (&pt2, im1, &ci1->p2);
      //TX_Print(" pt1=%f,%f,%f",pt1.x,pt1.y,pt1.z);



      L_AC_out:
      dxfw_point (0, &ptc, fp_in);  // Mittelpunkt

      // Radius
      fprintf(fp_in,"40\n%f\n",fabs(ci1->rad));


      // AnfWin
      // AnfWin
      pt2c = UT2D_pt_pt3 (&ptc);
      pt21 = UT2D_pt_pt3 (&pt1);
      d1 = UT_DEGREES(UT2D_angr_ptpt(&pt2c, &pt21));

      // EndWin
      pt21 = UT2D_pt_pt3 (&pt2);
      d2 = UT_DEGREES(UT2D_angr_ptpt(&pt2c, &pt21));

      // CW -> CCW
      if(ci1->rad < 0.) {
        d3=d1; d1=d2; d2=d3;
      }

      fprintf(fp_in,"50\n%f\n",d1);
      fprintf(fp_in,"51\n%f\n",d2);


      if(zparl==OFF) dxfw_vector (&ci1->vz, fp_in);

  return 0;

}


//=====================================================================
  int DXFW_fl_out (int recID, double fVal, FILE *fp_in) {
//=====================================================================
// einen Record mit double ausgeben

  char   cbuf[32];


  cbuf[0] = '\0';
  UTX_add_fl_u(cbuf, fVal);

  fprintf(fp_in,"%d\n%s\n",recID, cbuf);

  return 0;

}


//=====================================================================
  int dxfw_point2 (int pnum, Point2 *point, FILE *fp_in) {
//=====================================================================

  dxfw_fl_out (10+pnum, point->x, fp_in);
  dxfw_fl_out (20+pnum, point->y, fp_in);


  return 1;
}


//=====================================================================
  int DXFW_point (int pnum, Point *point, FILE *fp_in) {
//=====================================================================

  DXFW_fl_out (10+pnum, point->x, fp_in);
  DXFW_fl_out (20+pnum, point->y, fp_in);
  DXFW_fl_out (30+pnum, point->z, fp_in);

  return 1;
}


//=====================================================================
  int DXFW_vector (Vector* vc1, FILE *fp_in)
//=====================================================================

{
  fprintf(fp_in,"%d\n%f\n%d\n%f\n%d\n%f\n",
    210, vc1->dx,
    220, vc1->dy,
    230, vc1->dz);

  return 1;
}


//===========================================================================
  int dxfw_load_mat (Mat_4x3 m1, Vector* vz) {
//===========================================================================
 die Mat für ein  DXF-ECS bestimmen. Input nur der Vektor (eines Kreises).

 Der Nullpunkt (Origin) ist immer ident mit dem Hauptnullpunkt !

1) Der Vektor sei die neue ECS-Z-Achse.
2) Die neue X-Achse ist das Kreuzprodukt der World-Z-Achse mit der neuen Z-Achse
3) Die neue Y-Achse ist das Kreuzprodukt der neuen Z-Achse mit der neuen X-Achse




  Vector    vx, vy;
  Plane     pl1;


  //TX_Print("dxfw_load_mat vz=%f,%f,%f",vz->dx,vz->dy,vz->dz);


  // Die neue X-Achse ist das Kreuzprodukt der World-Z-Achse mit der neuen Z-Achse
  //UT3D_vc_perp2vc (&vx, vz, &UT3D_VECTOR_Z);
  UT3D_vc_perp2vc (&vx, (Vector*)&UT3D_VECTOR_Z, vz);
  //TX_Print("vx=%f,%f,%f",vx.dx,vx.dy,vx.dz);

  // Die neue Y-Achse ist das Kreuzprodukt der neuen Z-Achse mit der neuen X-Achse
  UT3D_vc_perp2vc (&vy, vz, &vx);
  //TX_Print("vy=%f,%f,%f",vy.dx,vy.dy,vy.dz);


  Load_Matrix:
  pl1.po = UT3D_PT_NUL;
  pl1.vx = vx;
  pl1.vy = vy;
  pl1.vz = *vz;

  UT3D_m3_loadpl (m1, &pl1);

  TX_Print(" matVX=%f,%f,%f",m1[0][0],m1[1][0],m1[2][0]);
  TX_Print(" matVY=%f,%f,%f",m1[0][1],m1[1][1],m1[2][1]);
  TX_Print(" matVZ=%f,%f,%f",m1[0][2],m1[1][2],m1[2][2]);
  TX_Print(" matPO=%f,%f,%f",m1[0][3],m1[1][3],m1[2][3]);


}


//================================================================
  int dxfw_ccv (ObjGX *ox1, FILE *fp_in) {
//================================================================
// export CCV as polygon
// TODO: export CCV as analytic obj's with group ?

  int       irc, pNr;
  Point     *pTab;

  pTab = (Point*)memspc201;
  pNr = sizeof(memspc201) / sizeof(Point);


  irc = UT3D_npt_ox (&pNr, pTab, ox1, UT_DISP_cv);
  if(irc < 0) return irc;

  return dxfw_POLYLINE (pNr, 32, pTab, fp_in);

}


//=====================================================================
  int DXFW_ox (ObjGX *ox1, long TrInd, int typ, long dbi,
               FILE *fp_o1, FILE *fp_o2) {
//=====================================================================
// dxfw_rec

  int           i1, zparl, ptNr, pNr;
  double        a1, a2, d1, d2, d3;
  Point2        pt2c, pt21, pt22, *p2a;
  Point         ptc, pt1, pt2, *pa, *pTab;
  Vector2       vc21, vc22;
  Line          *ln1;
  Circ          *ci1;
  // Point2Tab     *p2Tab;
  // PointTab      *pTab;
  GText         *tx1;
  Dimen         *dim1;
  Mat_4x3       m1, im1;
  ObjG          *el;


  printf("DXFW_ox typ=%d tr=%ld typ=%d dbi=%ld\n",ox1->typ,TrInd,typ,dbi);


  // IG_TrInd = TrInd;
  // el       = *objIn;


// 
  // if(ox1->typ == Typ_ObjGX) {
    // typ = ox1->form;
  // } else {
    // // bei den Kurven ...
    // typ = ox1->typ;
  // }
//


  switch (ox1->typ) {


    //=========================================================
    case Typ_PT:
      fprintf(fp_o1,"0\nPOINT\n");
      fprintf(fp_o1,"8\n0\n"); // def.Layer
      pa = ox1->data;
      DXFW_point (0, &pa[0], fp_o1);
      break;


    //=========================================================
    case Typ_LN:
      fprintf(fp_o1,"0\nLINE\n");
      fprintf(fp_o1,"8\n0\n"); // def.Layer
      ln1 = ox1->data;
      DXFW_point (0, &ln1->p1, fp_o1);
      DXFW_point (1, &ln1->p2, fp_o1);
      break;


    //=========================================================
    case Typ_CI:
      DXFW_CI (ox1->data, fp_o1);
      break;


    //=========================================================
    case Typ_CVPOL:
      dxfw_POLYLINE (((CurvPoly*)ox1->data)->ptNr, 32,
                     ((CurvPoly*)ox1->data)->cpTab, fp_o1);
      break;


    //=========================================================
    case Typ_CVBSP:
      if(dxf_version == 0) {    // 0=R12
        dxfw_obj2poly (ox1, fp_o1);
      } else {  // 1=2000
        dxfw_SPLINE (ox1->data, fp_o1);
      }
      break;


    //=========================================================
    case Typ_CVELL:
      if(dxf_version == 0) {    // 0=R12
        dxfw_obj2poly (ox1, fp_o1);
      } else {  // 1=2000
        dxfw_ELLIPSE (ox1->data, fp_o1);
      }
      break;


    //=========================================================
    case Typ_CVCLOT:
      dxfw_obj2poly (ox1, fp_o1);
      // pTab = (Point*)memspc201; 
      // pNr = sizeof(memspc201) / sizeof(Point);
      // // elli -> polygon -> mem?
      // UT3D_npt_ox (&pNr, pTab, ox1, UT_DISP_cv);
      // dxfw_POLYLINE (pNr, 32, pTab, fp_o1);
      break;


    //=========================================================
    case Typ_CVPOL2:
      dxfw_hd_POLYLINE (fp_o1); // wr header of POLYLINE


      // p2Tab = (Point2Tab*)ox1->data;
      // p2a   = (Point2*)p2Tab->ptTab;
      p2a   = (Point2*)ox1->data;
      for(i1=0;i1<ox1->siz; ++i1) {
        dxfw_VERTEX2 (32, &p2a[i1], fp_o1);
      }

      fprintf(fp_o1,"0\nSEQEND\n");
      break;


    //=========================================================
    case Typ_GTXT:
      fprintf(fp_o1,"0\nTEXT\n");
      fprintf(fp_o1,"8\n0\n"); // def.Layer

      tx1 = (GText*)ox1->data;

      // posi
      dxfw_point (0, &tx1->pt, fp_o1);

      // 7=Textstyle "normal#0.300000#0.900000" od cursive#0.500000#3.000000

      // 40-Textsize (characterhoehe)
      // Notes haben size -1!
      d1 = tx1->size;
      if(d1 < 0) d1 = 1.;;
      // d1 *= DXF_fakt_txtSiz;
      dxfw_fl_out (40, d1, fp_o1);

      // 50-Textdirection
      dxfw_fl_out (50, tx1->dir, fp_o1);

      // 1-Text
      GR_gxt_dxfout (0, memspc011, tx1->txt);
      fprintf(fp_o1,"1\n%s\n",memspc011);

      // 39 = Thickness; 1 oder 2
      i1 = 1;
      if(tx1->size > 1.) i1 = 2;
      fprintf(fp_o1,"39\n%d\n",i1);

      // 62 = colornum = 0
      fprintf(fp_o1,"62\n0\n");

      break;



    //=========================================================
    case Typ_Dimen:
      return DXFW_DIM (ox1, fp_o1);


    //=========================================================
    case Typ_CVCCV:
      return dxfw_ccv (ox1, fp_o1);


    //=========================================================
    default:
      printf("  dxfw skip %d\n",typ);
      return -1;

  }







/
  // //  Bestandteile von CCV's (und getrimmten Flaechen ...) ausgeben
  // irc = IGE_w_subObjs (&el, fp_o1, fp_o2);
  // if (irc < 0) return irc;
// 
  // IG_mode  = 0;             // 0=normales Obj; 1=SubObj (von CCV ..)
// 
// 
  // // ein Obj ausgeben (TrMat, D-Zeilen, P-Zeile)
  // irc = IGE_w_obj (&el, apt_typ, apt_ind, fp_o1, fp_o2);
  // if (irc < 0) return irc;
/

  return 0;

}


//=============================================================================
  int DXFW_Model (int modNr, FILE *fp1, FILE *fp2) {
//=============================================================================
// AP_ExportIges_Model

  long     gr_ind;
  long     l1, apt_ind;
  int      i1, irc, tra_ind, tra_act, apt_typ,  anz_obj=0,
           anz_pt=0, anz_ln=0, anz_ac=0, anz_cv=0;
  Point    pt1;
  Line     ln1;
  Circ     ci1;
  ObjG     o1;
  ObjGX    ox1, *op1;
  Plane    pl1;
  Mat_4x3  m1;
  DL_Att   dla;



  printf("DXFW_Model %d\n",modNr);


  //----------------------------------------------------------------
  apt_typ = Typ_PT;

  for(apt_ind=1; apt_ind<APT_PT_SIZ; ++apt_ind) {
    if(DB_QueryDef(apt_typ, apt_ind) < 0) continue;
    ox1 = DB_GetObjGX (apt_typ, apt_ind);
    if(ox1.typ == Typ_Error) continue;

    DXFW_ox (&ox1, tra_ind, apt_typ, apt_ind, fp1, fp2);
    ++anz_obj;
  }


  //-----------------------------------
  apt_typ = Typ_LN;

  for(apt_ind=1; apt_ind<APT_LN_SIZ; ++apt_ind) {
    if(DB_QueryDef(apt_typ, apt_ind) < 0) continue;
    ox1 = DB_GetObjGX (apt_typ, apt_ind);
    if(ox1.typ == Typ_Error) continue;

    DXFW_ox (&ox1, tra_ind, apt_typ, apt_ind, fp1, fp2);
    ++anz_obj;
  }


  //-----------------------------------
  apt_typ = Typ_CI;

  for(apt_ind=1; apt_ind<APT_CI_SIZ; ++apt_ind) {
    if(DB_QueryDef(apt_typ, apt_ind) < 0) continue;
    ox1 = DB_GetObjGX (apt_typ, apt_ind);
    if(ox1.typ == Typ_Error) continue;

    DXFW_ox (&ox1, tra_ind, apt_typ, apt_ind, fp1, fp2);
    ++anz_obj;
  }


  //-----------------------------------
  apt_typ = Typ_CV;

  for(apt_ind=0; apt_ind<APT_CV_SIZ; ++apt_ind) {
    if(DB_QueryDef(apt_typ, apt_ind) < 0) continue;
    ox1 = DB_GetObjGX (apt_typ, apt_ind);
    if(ox1.typ == Typ_Error) continue;

    DXFW_ox (&ox1, tra_ind, apt_typ, apt_ind, fp1, fp2);
    ++anz_obj;
  }


  //-----------------------------------
  apt_typ = Typ_GTXT;

  for(apt_ind=0; apt_ind<APT_TX_SIZ; ++apt_ind) {
    if(DB_QueryDef(apt_typ, apt_ind) < 0) continue;
    ox1 = DB_GetObjGX (apt_typ, apt_ind);
    if(ox1.typ == Typ_Error) continue;

    DXFW_ox (&ox1, tra_ind, apt_typ, apt_ind, fp1, fp2);
    ++anz_obj;
  }


  //-----------------------------------
  apt_typ = Typ_SUR;

  for(apt_ind=0; apt_ind<APT_SU_SIZ; ++apt_ind) {
    if(DB_QueryDef(apt_typ, apt_ind) < 0) continue;
    ox1 = DB_GetObjGX (apt_typ, apt_ind);
    if(ox1.typ == Typ_Error) continue;

    DXFW_ox (&ox1, tra_ind, apt_typ, apt_ind, fp1, fp2);
    ++anz_obj;
  }


  //-----------------------------------
  // SubfigInst; 
  apt_typ = Typ_Model;

  for(apt_ind=0; apt_ind<APT_MR_SIZ; ++apt_ind) {
    if(DB_QueryDef(apt_typ, apt_ind) < 0) continue;
    ox1 = DB_GetObjGX (apt_typ, apt_ind);
    if(ox1.typ == Typ_Error) continue;

    DXFW_ox (&ox1, tra_ind, apt_typ, apt_ind, fp1, fp2);
    ++anz_obj;
  }








  return anz_obj;

}


//================================================================
  int DXFW__ (char * fn, int dxfVersion) {
//================================================================
// Input:
//   dxfVersion    0=DxfR12  1=Dxf2000



  int      i1, anz_obj=0, ipos, modNr;
  FILE     *fp1, *fp2, *fpi;
  char     s1[256];


  printf("DXFW_init__ %d |%s|\n",dxfVersion,fn);


  if ((fp1 = fopen (fn, "w+")) == NULL) {
    TX_Error ("open file %s",fn);
    return;
  }


  //==================================================================
  // in BasicModels gibts Reihenfolgenummer seqNr.
  // Diese in korrekter Reihenfolge -> Datei <tmpdir>/Mod.lst ausgeben.
  DB_list_ModBas ();


  // try to open inListe
  sprintf(s1,"%sMod.lst",OS_get_tmp_dir());
  if((fpi=fopen(s1,"r")) == NULL) {
    TX_Print("AP_ExportIges__ E002 %s",s1);
    return;
  }

  sprintf(s1,"%sModel_",OS_get_tmp_dir());
  ipos = strlen(s1);

  // loop tru subModels
  while (!feof (fpi)) {
    if (fgets (&s1[ipos], 256, fpi) == NULL) break;
    UTX_CleanCR (s1);
      printf("nxt model |%s|\n",s1);

/
    // DB_Init (1);
    // // RUN (abarbeiten)
    // ED_Init ();
    // if(ED_work_file (cbuf) < 0) {TX_Error("AP_ExportIges__: E003"); goto L_err;}
    // ED_lnr_reset ();
    // WC_Work__ (0, NULL);   // Init Levelcounter in WC_Work__
    // ED_Run ();
// 
    // // work submodel
    // modNr = DB_get_ModNr(&cbuf[ipos]);  // get ModelNr from Modelname
    // if(modNr < 0) {TX_Error("AP_ExportIges__: E004"); goto L_err;}
    // i1 = AP_ExportIges_Model (modNr, fp1, fp2);  NEW: DXFW_Model !
    // anz_obj += i1;
/

  }
  fclose(fpi);


  //----------------------------------------------------------------
  // resolv main
  // DB_Init (1);
  // ED_Reset ();  // ED_lnr_act = 0; 2004-02
  // ED_work_END (0);

  //------------------------------------
  fprintf(fp1,"0\nSECTION\n");
  fprintf(fp1,"2\nENTITIES\n");


  i1 = DXFW_Model (-1, fp1, fp2);
  anz_obj += i1;


  fprintf(fp1,"0\nENDSEC\n");



  //----------------------------------------------------------------
  // join files

  fclose (fp1);





  //----------------------------------------------------------------
  // print statistics
  TX_Print("   Objekte exportiert:  %d",anz_obj);


  return 0;

}
*/


//======================================================================
  int MSG_msg_init (int *lngNr, char lngCode[][4], char lngName[][80]) {
//======================================================================
// provide list of supported languages an language-names
 
  int    ii, iNr, lNr;
  char   *p1, cbuf1[256];



  printf("MSG_msg_init %d\n",*lngNr);


  //----------------------------------------------------------------
  // - make list of all <docdir>/msg/msg_*.txt
  lNr = 0;
  iNr = 0;
  sprintf(cbuf1,"%smsg/",OS_get_doc_dir());
  ii = strlen (cbuf1);
    printf(" _scan_ %d |%s|\n",ii,cbuf1);

  OS_dir_scan_ (cbuf1, &iNr);   // Init

  for(;;)  {
    OS_dir_scan_ (cbuf1, &iNr);
    if(iNr < 0) break;
    if(strncmp(&cbuf1[ii], "msg_", 4)) continue;
    if(!strncmp(&cbuf1[ii + 4], "const", 4)) continue;


    // extract & copy language-code
    strncpy (lngCode[lNr], &cbuf1[ii + 4], 2);
    lngCode[lNr][2] = '\0';
      printf(" n.scan |%s| %d |%s|\n",lngCode[lNr], lNr, cbuf1);


    // - get value of LANG__ of all files
    MSG_Init (lngCode[lNr]);
    // p1 = MSG_get_str ("LANG__");
    strcpy (lngName[lNr], MSG_get_str ("LANG__"));
      printf(" lang = |%s|\n",lngName[lNr]);


    ++lNr;
    if(lNr >= *lngNr) {
      TX_Error("MSG_msg_init E001");
      return -1;
    }
  }



  //----------------------------------------------------------------

  return 0;

}


//================================================================
  int tst_tst__ () {
//================================================================
// general test ..
 

  int    ii, iNr;
  char   *p1, cbuf1[256];

  // TX_Print(" plugin tst start ..");

//----------------------------------------------------------------
#define LNG_MAX_NR 16
  typedef  char lngCode[4];
  typedef  char lngName[80];
  lngCode    *TabLngCode;
  lngName    *TabLngName;
  TabLngCode = (void*) UME_alloc_tmp (LNG_MAX_NR * 4);
  TabLngName = (void*) UME_alloc_tmp (LNG_MAX_NR * 80);

  ii = LNG_MAX_NR;
  MSG_msg_init (&ii, TabLngCode, TabLngName);


/*
      // filter filetyp
      i1 = strlen(cbuf1);

#ifdef _MSC_VER
      // for(i2=i1-3; i2<i1; ++i2) cbuf1[i2] = tolower (cbuf1[i2]);
      if(strncmp(&cbuf1[i1-4], ".dll", 4)) continue;
#else
      if(strncmp(&cbuf1[i1-3], ".so", 3)) continue;
#endif

*/

  return 0;

}


/*
//================================================================
  int tst_tst__ () {
//================================================================
// general test ..
 

  int     imod=0, sn, lTyp=3;
  Point   *pt1;
  Vector  vc1;
  Line    *ln1, lno;
  Circ    *ci1, *ci2;

  // line tangent circ-vector  (C21,L21)
  pt1 = DB_get_PT (22L);
  ci1 = DB_get_CI (21L);
  ci2 = DB_get_CI (22L);
  ln1 = DB_get_LN (21L);
    // UT3D_stru_dump(Typ_PT, pt1, "pt1");
    UT3D_stru_dump(Typ_CI, ci1, "ci1");
    UT3D_stru_dump(Typ_CI, ci2, "ci2");
    // UT3D_stru_dump(Typ_LN, ln1, "ln1");


  // sn = UT3D_ln_tng_ci_ci (&lno, ci1, ci2, imod);
    // printf(" sn=%d\n",sn);
    // GR_Disp_ln (&lno, 9);


  UT3D_ln_tng_ci_pt (&lno, ci1, pt1, imod);

  UT3D_ln_unlim (&lno, lTyp);
    GR_Disp_ln (&lno, 9);


  // UT3D_vc_ln (&vc1, ln1);  // vc from ln
  // UT3D_ln_tng_ci_vc (&lno, ci1, &vc1, imod);

  return 0;

}


//================================================================
  int UT3D_ln_unlim (Line *lno, int lTyp) {
//================================================================
// UT3D_ln_unlim           set  length for construction-line
 
  double  d1, d2;
  Point   *pta, *pte;
  Vector  vc1;


  printf("UT3D_ln_unlim %d\n",lTyp);


  lno->typ = lTyp;

  if(!lTyp) return 0;

  pta = &lno->p1;
  pte = &lno->p2;

  UT3D_vc_2pt (&vc1, pta, pte); // ln -> vc

  d1 = UT3D_len_vc (&vc1);
  d2 = UT_DISP_ln / d1;   // UT_DISP_ln=length of construction-line
  UT3D_vc_multvc (&vc1, &vc1, d2);



  if(lTyp == 1) {  // p1 is startpoint, p2 unlimited) UNL1
    UT3D_pt_traptvc (pte, pte, &vc1);

  } else if(lTyp == 2) {  // p2 is startpoint, p1 unlimited) UNL2
    UT3D_pt_traptivc (pta, pta, &vc1);

  } else if(lTyp == 3) {  // both sides unlimited   UNL
    UT3D_pt_traptivc (pta, pta, &vc1);
    UT3D_pt_traptvc (pte, pte, &vc1);
  }


  return 0;

}

//=====================================================================
  int UT3D_ln_tng_ci_pt (Line *lno, Circ *ci1, Point *pt1, int imod) {
//=====================================================================
/// \code
/// UT3D_ln_tng_ci_pt          tangent to circ tru point
/// imod      0 = point at y-axis (vc1=x-axis; ci1.vz=z-axis)
///           1 = point at negative y-axis
/// RetCod    Nr of solutions; -1,1,2.
/// \endcode


  int     irc;
  Point   po1, po2, *pt2;


  irc = UT3D_pt_tangptci (&po1, &po2, pt1, ci1);
    GR_Disp_pt (&po1, SYM_TRI_S, ATT_COL_RED);
    GR_Disp_pt (&po2, SYM_TRI_S, ATT_COL_RED);

  // if(irc == 1) {
  // }


  if(imod) pt2 = &po2;
  else     pt2 = &po1;

  UT3D_ln_ptpt (lno, pt2, pt1);
    GR_Disp_ln (lno, 9);

  return irc;

}

//=====================================================================
  int UT3D_ln_tng_ci_vc (Line *lno, Circ *ci1, Vector *vc1, int imod) {
//=====================================================================
/// \code
/// UT3D_ln_tng_ci_vc          tangent with fixed vector to circ
/// imod      0 = point at y-axis (vc1=x-axis; ci1.vz=z-axis)
///           1 = point at negative y-axis
/// \endcode


  Point   pt1;

  UT3D_pt_tng_ci_vc (&pt1, ci1, vc1, imod); // tangent-point on circ
    // UT3D_stru_dump(Typ_PT, &pt1, "pt1");

  UT3D_ln_ptvc (lno, &pt1, vc1);
    // GR_Disp_ln (lno, 9);


  return 0;

}
*/


//=========================================================
  int gCad_main () {
//=========================================================
// init userfunction


  TX_Print("-->> gCad_main aus tst; 2014-03-20.");
  printf("-->> gCad_main aus tst;\n");


  //================================================================
  tst_tst__ ();   goto L_fini;     // general test ..
  //================================================================
  // TEST EXPORT_DLL'S:
  // tst_exp_stp ();
  tst_exp_dxf ();
  // tst_exp_vrml2 ();
  // tst_exp_stl ();
  // tst_exp_obj ();
  // tst_exp_svg ();
  //================================================================
  // TEST IMPORT_DLL'S:
  // UI_menCB (NULL, "new");
  // tst_imp_dxf();
  // tst_imp_igs ();
  // tst_imp_vrml1(); // test import VRML-1
  // tst_imp_vrml2(); // test import VRML-2
  // tst_imp_stp (); // tst_imp_exp.c  Test Import/Export-functions
  // tst_imp_stl ();
  // tst_imp_obj ();
  // tst_imp_tess ();
  // LandXml-Import: in core (AP_ImportXML -> lxml_read); 
  //================================================================

  L_end:
  DL_Redraw ();

  L_fini:
  gCad_fini ();

  return 0;

}



//=========================================================
  int gCad_fini () {
//=========================================================
// dll being unloaded - reset Input, kill all open windows !!!

  printf("gCad_fini tst \n");

  TX_Print("---<<<<<< exit tst__ <<<<<<<---\n");

  // tst_ogl_reset ();
  // VcSel_exit ();

  AP_User_reset ();

  return 0;

}



//=========================================================
  int tst_sel_CB (int src, long dl_ind) {
//=========================================================
// userselection callback

  int   irc, typ;
  long  dbi;
  // char  so[512];
  char  *so;
  // ObjGX o1;


  printf("tst_sel_CB src=%d ind=%ld\n",src,dl_ind);

  return 0;

}


//=========================================================
  int tst_key_CB (int key) {
//=========================================================
// userKeyin callback


  printf("tst_key_CB %d\n",key);

  switch (key) {

    case GUI_KeyEsc:
    case 'q':
      // DL_hili_off (-1L);         //  -1 = unhilite all
      // DL_Redraw ();               // Redraw DispList
      gCad_fini ();
      // PED_CB1 (NULL, "Exit");
      break;

  }

  return 0;

}



//======== EOF =========
