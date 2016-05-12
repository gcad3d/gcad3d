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

// #include <errno.h>
// #include <dlfcn.h>           // Unix: dlopen


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
#include "../gr/tess_su.h"             // TypTsuSur
#include "../gui/gui__.h"              // GUI_

#include "../xa/xa_msg.h"              // MSG_*
#include "../xa/xa_mem.h"              // memspc51, mem_cbuf1


// Externals aus ../ci/NC_Main.c:
extern double    APT_ModSiz;

// Externals aus ../xa/xa.c:
extern char      WC_modnam[128];


// protos:
  int tst_sel_CB (int src, long dl_ind);
  int tst_key_CB (int key);



//=========================================================
  int gCad_main () {
//=========================================================
// init userfunction

  int   i1;


  TX_Print("-->> gCad_main aus tst; 2016-04-18.");
  printf("-->> gCad_main aus tst;\n");

  // attach KeyIn - connect KeyIn -> func dia_KeyIn
  AP_UserKeyIn_get (tst_key_CB);


  //================================================================
  i1 = tst_tst__ (0);   if(i1) return 0; // general test ..
  //================================================================
  // TEST EXPORT_DLL'S:
  // tst_print_pdf ();
  // tst_exp_stp ();
  // tst_exp_dxf ();
  // tst_exp_vrml2 ();
  // tst_exp_stl ();
  // tst_exp_obj ();
  // tst_exp_svg ();
  // tst_print__ ();  // OS_dll_do ("xa_print__", "PRI__"
  //================================================================
  // TEST IMPORT_DLL'S:
  // UI_menCB (NULL, "new");
  // tst_imp_dxf ();
  // tst_imp_igs ();
  // tst_imp_vrml1(); // test import VRML-1
  // tst_imp_vrml2(); // test import VRML-2
  // tst_imp_stp (); // tst_imp_exp.c  Test Import/Export-functions
  // tst_imp_stl ();
  // tst_imp_obj ();
  // tst_imp_tess ();
  // LandXml-Import: in core (AP_ImportXML -> lxml_read); 
  //================================================================
  // AP_print__ (); // test feedbackbuffer - out -> <tempDir>/print.tmp

  L_end:
  DL_Redraw ();

  L_fini:
  gCad_fini ();

  return 0;

}





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
      dxfw_gxt (0, memspc011, tx1->txt);
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


//================================================================
  int UT3D_vc_ck_aparl_vc (Vector *v1, Vector *v2, double tol) {
//================================================================
/// \code
/// UT3D_vc_ck_aparl_vc       check for antiparallel (normalized only)
///
/// tol: RAD_1 RAD_01 ..
///
/// Output:
///   retCod     0   vectors are not antiparallel
///              1   vectors are parallel with different direction
/// see also UT3D_comp2vc__
/// \endcode

  double  dd;


  dd = -v1->dx;
  if(!UTP_comp2db (dd, v2->dx, tol)) return 0;

  dd = -v1->dy;
  if(!UTP_comp2db (dd, v2->dy, tol)) return 0;

  dd = -v1->dz;
  if(!UTP_comp2db (dd, v2->dz, tol)) return 0;

  return 1;

}

//========================================================================
  double UT3D_angr_3vcn_CCW (Vector *vz, Vector *v1, Vector *v2) {
//=============================================================================
/// \code
/// UT3D_angr_3vcn    angle between 2 vectors CCW around vz; all normalized
///   Rotation CCW around vz
///   RetCod: angle CCW between v1 and v2;
/// Input:
///   v1, v2    compute angle between these vectors
///   vz        up-vector (necessary if angle > PI)
/// Output:
///   retCod    angle 0 < 2*PI
/// see UT3D_angr_2vc__ UT3D_angr_3vc__
/// see UT3D_angr_ci_p1_pt
/// \endcode


  int       svz;
  double    ao;
  Vector    vcn;


  UT3D_stru_dump (Typ_VC, v1, "v1:");
  UT3D_stru_dump (Typ_VC, v2, "v2:");
  UT3D_stru_dump (Typ_VC, vz, "vz:");

  //  ao = Oeffnungswinkel
  ao = ACOS(UT3D_acos_2vc (v1, v2));
    // printf("ao=%f\n",ao);


  // get the normalvector
  UT3D_vc_perp2vc (&vcn, v1, v2);
    UT3D_stru_dump (Typ_VC, &vcn, "vcn:");

  // side; 1=parl, -1=antiparl
  svz = UT3D_sid_2vc (&vcn, vz);
  if(svz < 0) ao = RAD_360 - ao;
    printf(" svz=%d\n",svz);

    printf("ex UT3D_angr_3vcn_CCW %f %f\n",ao,UT_DEGREES(ao));

  return ao;

}

//================================================================
  double UT3D_angr_2vc_n (Vector *v1, Vector *v2) {
//================================================================
/// \code
/// UT3D_angr_2vc_n           angle between two normalized vec's
///   angle = always 0 <= PI;    direction is CCW OR CW.
/// for CCW-direction / angles (0 < 2PI)  use UT3D_angr_3vcn
/// \endcode

// 0.866,    0.500,    30 deg
// 0.707,    0.707,    45 deg
// 0.500,    0.866,    60 deg


  // UT3D_stru_dump (Typ_VC, v1, "v1");
  // UT3D_stru_dump (Typ_VC, v2, "v2");

  return ACOS(UT3D_skp_2vc (v1, v2));

}


//================================================================
  int OS_ck_SW_is_installed (char *ssw) {
//================================================================
// Test if software <ssw> is installed
// returns 0 = yes, is installed,   elso no, not installed.
// Test with "which <ssw>"

  char    s1[256];

  sprintf(s1, "which %s  1>/dev/null 2>/dev/null", ssw);
    printf("OS_ck_SW_is_installed |%s|\n",s1);

  return system (s1);

}
*/

/*
void*  GL_Print1   (int *iw, int *ih);

#include "../ut/byteorder.h"

typedef struct {
  unsigned short it; unsigned long is; unsigned short iu1, iu2;
  unsigned long io;
} bitMapRec1;


typedef struct {
  unsigned long ish,iw,ih; unsigned long ipl,inr;
  unsigned long ic,isi,ix,iy,icu,ici;
} bitMapRec2;


typedef struct {              
  unsigned char  rgbBlue;      
  unsigned char  rgbGreen;     
  unsigned char  rgbRed;       
  unsigned char  rgbReserved;   
} colRec1;                         // Windows: RGBQUAD oder so


  unsigned short bmp_MB='MB';

struct BMPHeader
{
    char bfType[2];      
    int bfSize;         
    int bfReserved;    
    int bfOffBits;    
    int biSize;      
    int biWidth;    
    int biHeight;  
    short biPlanes;       
    short biBitCount;    
    int biCompression;  
    int biSizeImage;   
    int biXPelsPerMeter; 
    int biYPelsPerMeter;
    int biClrUsed;     
                      
    int biClrImportant;
                      
};


//================================================================
  int bmp_save__ (char *fNam) {
//================================================================

  int    iw, ih, lSiz1, lSiz2, fSiz, i1, ii;
  void   *pm;
  char   *vp1;


  FILE *file;
  struct BMPHeader bmph;

    // int i, j, ipos;
    // int bytesPerLine;
    // unsigned char *line;
    // unsigned char rgb[3] = {16, 32, 64};


  printf("bmp_save__ |%s|\n",fNam);

  // iw = 100;  ih = 100;
  pm = GL_Print1 (&iw, &ih);
    printf(" iw=%d ih=%d\n",iw,ih);

  // lSiz1 = iw;   // size of line - input grayscale; 1 byte per pixel
  lSiz1 = iw * 3;  // size of line - input BGR


  lSiz2 = (lSiz1 + 3) & ~3;   // size of line 4-bytes aligned
  fSiz  = lSiz2 * ih;
    printf(" lSiz1=%d lSiz2=%d fSiz=%d\n", lSiz1, lSiz2, fSiz);fflush(stdout);


  // fill bitmap-header
  strcpy(bmph.bfType, "BM");
  bmph.bfOffBits = 54;
  bmph.bfSize = bmph.bfOffBits + fSiz;
  bmph.bfReserved = 0;
  bmph.biSize = 40;           // Size of BITMAPINFOHEADER, in bytes
  bmph.biWidth = iw;
  bmph.biHeight = ih;
  bmph.biPlanes = 1;          // Number of planes in target device
  // bmph.biBitCount = 8;        // Bits per pixel - grayscale
  bmph.biBitCount = 24;       // Bits per pixel RGB
  bmph.biCompression = 0;
  bmph.biSizeImage = 0;       // Image size (0 = no compression)
  bmph.biXPelsPerMeter = 0;   // Resolution in pixels/meter of display device
  bmph.biYPelsPerMeter = 0;
  bmph.biClrUsed = 0;         // Number of colors in the color table
  bmph.biClrImportant = 0;    // Number of important colors


  file = fopen (fNam, "wb");
  if (file == NULL) return(0);

  // write bitmap-header
  fwrite(&bmph.bfType, 2, 1, file);
  fwrite(&bmph.bfSize, 4, 1, file);
  fwrite(&bmph.bfReserved, 4, 1, file);
  fwrite(&bmph.bfOffBits, 4, 1, file);
  fwrite(&bmph.biSize, 4, 1, file);
  fwrite(&bmph.biWidth, 4, 1, file);
  fwrite(&bmph.biHeight, 4, 1, file);
  fwrite(&bmph.biPlanes, 2, 1, file);
  fwrite(&bmph.biBitCount, 2, 1, file);
  fwrite(&bmph.biCompression, 4, 1, file);
  fwrite(&bmph.biSizeImage, 4, 1, file);
  fwrite(&bmph.biXPelsPerMeter, 4, 1, file);
  fwrite(&bmph.biYPelsPerMeter, 4, 1, file);
  fwrite(&bmph.biClrUsed, 4, 1, file);
  fwrite(&bmph.biClrImportant, 4, 1, file);
  

  // write line by line
  ii = 0;
  vp1 = pm;
  for (i1=0; i1 < ih; ++i1) {
    fwrite (vp1, lSiz2, 1, file);
    vp1 += lSiz1;
  }

  fclose(file);

  if(pm) free (pm);

  // change bmp -> jpg



}
 

//================================================================
  char* OS_get_imgConv2  () {
//================================================================
/// returns bmp2jpg-converter-program; eg /usr/bin/cjpeg
// popen ?

  static int  iStat = 0;          // 0=notYetTested; 1=OK; -1=NotOk.
  static char fn1[] = "cjpeg";


  if(iStat == 0) {    // init
    iStat = system("which djpeg 1>/dev/null 2>/dev/null");
    if(iStat == 0) iStat =  1;   // OK
    else           iStat = -1;   // not OK
  }

  if(iStat > 0) return &fn1[0];

  MSG_pri_1 ("NOEX_fil", "bmp2jpg-converter");

  return "";

}


//================================================================
  int OS_jpg_bmp (char *fn_jpg, char *fn_bmp) {
//================================================================
// convert BMP -> JPG

  char  s1[400];

  sprintf(s1, "%s \"%s\" > \"%s\"",OS_get_imgConv2(),fn_bmp,fn_jpg);
    printf(" |%s|\n",s1);

  return OS_system(s1);


}
*/

#include <GL/gl.h>

//=====================================================================
  int AP_print_color (FILE *fpo, int *count, float *buffer) {
//=====================================================================
/* hier eventuell eine veraenderte farbe ausgeben ..
X-Coord Y-Coord Z-Coord R G B A
*/

  int i1;

  printf ("    %d  ",*count);

  for (i1 = 0; i1 < 4; i1++) {
     printf (" %7.2f ", buffer[i1]);
  }

  printf ("\n");

  return 0;

}


/*
//=========================================================
  int AP_print_work3 (int size, float *buffer) {
//=========================================================
// write file <tempDir>/print.tmp from GL_3D_COLOR-Feedbackbuffer 
// UNUSED - NO occlusion culling / hidden surface removal / hidden line removal
// In:  Feedbackbuffer;
// Out: Objekte ->  Hilfsdatei <tempDir>/print.tmp
// Normaler Record (Point, Line, Polygon) sieht so aus:
// X-Coord Y-Coord Z-Coord R G B A

static char* txBuf=NULL;

  int     count;
  // GR_Att    att1;
  Att_ln    *att1;
  char      cbuf[256];
  FILE      *fpo;
  int       i1, i2, i3, token, nvertices, newAtt;
  int       actCol=-1, actLtyp=-1, actLthick=-1;
  long      l1;
  ObjGX     *ox1;
  GText     *gtx1;


  count = 0;


  // open outputfile
  sprintf(cbuf,"%sprint.tmp",OS_get_tmp_dir ());
  if ((fpo = fopen (cbuf, "w+")) == NULL) {
    TX_Error ("AP_print_work2 E001");
    return -1;
  }

  printf("AP_print_work3 size=%d |%s|\n",size,cbuf);



  NextRec:
  token = buffer[count];
    printf("........... next: x0%x [%d]\n",token,count),
  ++count;

      //===================================================================
      if (token == GL_PASS_THROUGH_TOKEN) {    // 0x0700
         // get 1 uservalue (provided from func glPassThrough)
         // printf ("%d GL_PASS_THROUGH_TOKEN %f\n",count,buffer[count]);
         // Wert >= 0 ist eine AttributNr;
         if(buffer[count] >= 0.) {
           newAtt = buffer[count];
           // DL_Get_GrAtt (&att1, newAtt);  // graf.Attrib holen; 
           AttLn_Get_ind (&i1, &i2, &i3, newAtt);  // get line-attribute
           // printf(" att %d %d %d\n",att1.col,att1.ltyp,att1.lthick);
           // set Linetyp
           if((i1 != actCol)    ||
              (i2 != actLtyp)   ||
              (i3 != actLthick))     {
             actLtyp = i2;
             fprintf(fpo, "AT %d %d %d\n",i1,i2,i3);
           }

         // Wert < 0 ist ein TextnoteIndex
         } else {
           l1 = -buffer[count];
           // if(l1 < APT_TX_SIZ) {
// Crash bei Dimen
             // ox1 = DB_GetGTxt (l1);
             // gtx1 = ox1->data;
             // // fprintf(fpo, "TN %s\n",gtx1->txt);
             // printf("TN %s\n",gtx1->txt);
             // txBuf = gtx1->txt;
           // // }
//
         }
         ++count;


      //===================================================================
      } else if (token == GL_POINT_TOKEN) {    // 0x0701
         // printf ("%d GL_POINT_TOKEN\n",count);
         // AP_print_vertex (fpo, &count, &buffer[count]);
         fprintf(fpo, "PT %f %f %f\n",
                 buffer[count],buffer[count+1],buffer[count+2]);
         // count += 2;    // 2D
         // count += 3; // 3D
         count += 7; // 3D-col


      //===================================================================
      } else if (token == GL_LINE_TOKEN) {    // 0x0702
         // printf ("%d GL_LINE_TOKEN\n",count);
           // AP_print_TEST_LN (buffer[count],buffer[count+1],
                             // buffer[count+2],buffer[count+3], 8);
         // AP_print_vertex (fpo, &count, &buffer[count]);
         fprintf(fpo, "LN %f %f %f %f %f %f\n",
                 buffer[count],buffer[count+1],buffer[count+2],
                 buffer[count+7],buffer[count+8],buffer[count+9]);
         // count += 4;    // 2D
         // count += 6;   // 3D
         count += 14; // 3D-col


      //===================================================================
      } else if (token == GL_POLYGON_TOKEN) {    // 0x0703
         nvertices = buffer[count];
         // printf ("%d GL_POLYGON_TOKEN: (%d)\n",count,nvertices);
         ++count;
         // write color
         fprintf(fpo, "CO %f %f %f %f\n",
                 buffer[count+3],buffer[count+4],buffer[count+5],buffer[count+6]);
         fprintf(fpo, "PO %f %f %f",
                 buffer[count],buffer[count+1],buffer[count+2]);
         // count += 2;  // 2D
         // count += 3;   // 3D
         // AP_print_color (fpo, &count, &buffer[count]);
         count += 7; // 3D+col
         for (i1=1; i1<nvertices; ++i1) {
           // AP_print_vertex(fpo, &count, &buffer[count]);
           fprintf(fpo, " %f %f %f",
                   buffer[count],buffer[count+1],buffer[count+2]);
           // count += 2;    // 2D
           // count += 3;
           count += 7;  // 3D-col
         }
         fprintf(fpo, "\n");


      //===================================================================
      } else if (token == GL_BITMAP_TOKEN) {
         // printf ("%d GL_BITMAP_TOKEN\n",count);
         if(txBuf) {   // erster char einer Textnote
           fprintf(fpo,"TN %f %f %f %s\n",
                   buffer[count],buffer[count+1],buffer[count+2],txBuf);
           // printf("TN %f %f %s\n",buffer[count],buffer[count+1],txBuf);
           txBuf = NULL;
         }
         // count += 7;
         // count += 3;
         count += 2;    // 2D


      //===================================================================
      } else if (token == GL_DRAW_PIXEL_TOKEN) {
         // printf ("%d GL_DRAW_PIXEL_TOKEN\n",count);
         // count += 2;    // 2D
         // count += 3;    // 3D
         count += 7; // 3D + col


      //===================================================================
      } else if (token == GL_COPY_PIXEL_TOKEN) {
         // printf ("%d GL_COPY_PIXEL_TOKEN\n",count);
         // count += 2;    // 2D
         // count += 3;    // 3D
         count += 7; // 3D + col



      //===================================================================
      } else if (token == GL_LINE_RESET_TOKEN) {  // 0x0707
         // line with stipple reset.
         // printf ("%d GL_LINE_RESET_TOKEN\n",count);
           // AP_print_TEST_LN (buffer[count],buffer[count+1],
                             // buffer[count+2],buffer[count+3], 8);
         // AP_print_vertex (fpo, &count, &buffer[count]);
         fprintf(fpo, "LN %f %f %f %f\n",
                 buffer[count],buffer[count+1],buffer[count+2],
                 buffer[count+7],buffer[count+8],buffer[count+9]);
         // count += 4;    // 2D
         // count += 6;   // 3D
         count += 14; // 3D-col


      //===================================================================
      } else printf ("%d **** unknown GL-TOKEN 0x%x ****\n",size-count,token);


  if(count < size) goto NextRec;

  fclose(fpo);

  return 0;

}


//=====================================================================
  int AP_print_psv3 (int irot,char* off,char* scl,char* gray) {
//=====================================================================
// create <tempDir>/print.eps from 3D-file print.tmp
// UNUSED - hidden surface removal missing
// TODO: occlusion culling / hidden surface removal / hidden line removal
//  for the incoming triangles (filled polygons)
// Input:
//   irot     0, 1=90 deg rotated
//   off      "0,0"    offset
//   scl      "1"      text-scale
//   gray     "2"      unused
//   file <tempDir>/print.tmp
//   file <cfgDir>/psv.setup
// Output:
//   file <tempDir>/print.eps

  GLint   GL_Viewp[4];         // x-left, y-low, width, heigth
  double  d1, fscl;
  int     i1, i2, iw, ih, xOff=0, yOff=0, igray,
          newCol, newLtyp, newLthick, actCol=-1, actLtyp=-1, actLthick=-1;
  long    l1, nUnsupp;
  float   col_r, col_g, col_b, col_m;
  char    cbuf[256], *p1, *p2, *p3;
  FILE    *fp1, *fp2;



  printf ("AP_print_psv3 %d |%s|%s|%s|\n",irot,off,scl,gray);




  glGetIntegerv (GL_VIEWPORT, GL_Viewp);  // get Viewport-Matrix
  printf(" viewp %d %d %d %d\n",GL_Viewp[0], GL_Viewp[1],
                                GL_Viewp[2], GL_Viewp[3]);
  iw = GL_Viewp[2];
  ih = GL_Viewp[3];
  nUnsupp = 0;


  d1 = strtod (off, &p2);
  xOff = d1;
  ++p2;
  d1 = strtod (p2, &p1);
  yOff = d1;

  d1 = strtod (gray, &p2);
  igray = d1;

  fscl = strtod (scl, &p2);
  printf(" off=%d,%d, scl=%f gray=%d\n",xOff,yOff,fscl,igray);




  sprintf(cbuf,"%sprint.eps",OS_get_tmp_dir ());
  if ((fp1 = fopen (cbuf, "w+")) == NULL) {
    TX_Error ("AP_print_psv3 E001");
    return -1;
  }


  fprintf(fp1, "%%!PS-Adobe-2.0 EPSF-2.0\n");
  fprintf(fp1, "%%%%Title: %s\n",WC_modnam);
  fprintf(fp1, "%%%%CreationDate: %s\n", OS_date1());


  if(irot == 0) {
    i1=xOff+iw;
    i2=yOff+ih;
  } else {
    i2=xOff+iw;
    i1=yOff+ih;
  }
  fprintf(fp1, "%%%%BoundingBox: %d %d %d %d\n\n",xOff,yOff,i1,i2);

  // nun die Defaultvariablen:
  fprintf(fp1, "/_rPt %f def\n",2.);






  // add setup-File psv.setup via mem_cbuf1
  sprintf(cbuf,"%spsv.setup",OS_get_cfg_dir ());
  l1 = OS_FilSiz (cbuf);
  if ((fp2 = fopen (cbuf, "r")) == NULL) {
    TX_Error ("AP_print_psv3 file: Error open %s",cbuf);
    return -1;
  }
  if(l1 > sizeof(mem_cbuf1)) return -1;
  fread (mem_cbuf1, l1, 1, fp2);
  // mem_cbuf1[strlen(mem_cbuf1)] = '\0';
  mem_cbuf1[l1] = '\0';
  fclose(fp2);
  // printf("|%s|\n",mem_cbuf1);
  fprintf(fp1, "%s\n",mem_cbuf1);





  // page setup
  if(irot == 0) {
    fprintf(fp1, "%d %d translate\n",xOff,yOff);
  } else {
    fprintf(fp1, "%d %d translate\n",xOff+ih,yOff);
    fprintf(fp1, "90 rotate\n");
  }

  // i1 = iw*fscl;
  // i2 = ih*fscl;
  // fprintf(fp1, "%d %d scale\n",i1,i2);

  fprintf(fp1, "%f %f scale\n",fscl,fscl);

  //======================================================================
  // data
  sprintf(cbuf,"%sprint.tmp",OS_get_tmp_dir ());
  if ((fp2 = fopen (cbuf, "r")) == NULL) {
    TX_Error ("AP_print_pvs file: Error open print.tmp");
    return -1;
  }

  while (!feof (fp2)) {
    if (fgets (cbuf, 250, fp2) == NULL) break;
    UTX_CleanCR (cbuf);                            // remove foll CR,LF ..
    // printf(" in:|%s|\n",cbuf);


    //----------------------------------------------------------------
    if(!strncmp(cbuf, "AT ", 3))  {
      // Graf Att. Col Ltyp Lthick
      sscanf(&cbuf[3], "%d %d %d",&newCol,&newLtyp,&newLthick);
      // printf(" att %d %d %d\n",newCol,newLtyp,newLthick);
      if(newLtyp != actLtyp) {
        actLtyp = newLtyp;
        if(newLtyp < 4) {
          fprintf(fp1, "_ltyp%d\n",newLtyp);
        } else printf("**** AP_print_pvs unsupp. lintyp %d\n",newLtyp);
      }
      if(newLthick != actLthick) {
        actLthick = newLthick;
        if(newLthick < 4) {
          fprintf(fp1, "_lthick%d\n",newLthick);
        } else printf("**** AP_print_pvs unsupp. linthick %d\n",newLthick);
      }


    //----------------------------------------------------------------
    } else if(!strncmp(cbuf, "TN ", 3))  {
      // <x> <y> <text>
      // find 2. ' '
      p1 = strchr (&cbuf[3], ' ');
      if(!p1) goto L_err1;
      ++p1;
      p2 = strchr (p1, ' ');
      if(!p2) goto L_err1;
      *p2 = '\0';         // abtrennen
      ++p2;
      fprintf(fp1, "%s m (%s) show\n",&cbuf[3],p2);



    //----------------------------------------------------------------
    } else if(!strncmp(cbuf, "PT ", 3))  {
      // <x> <y> _pt
      fprintf(fp1, "%s _pt\n", &cbuf[3]);


    //----------------------------------------------------------------
    } else if(!strncmp(cbuf, "CO ", 3))  {
      // Color; r g b transparency (eg for following polygon (triangle))
      sscanf (&cbuf[3], "%f %f %f", &col_r, &col_g, &col_b);
        // printf(" col %f %f %f\n", col_r, col_g, col_b);


    //----------------------------------------------------------------
    } else if(!strncmp(cbuf, "LN ", 3)) {
      // <x> <y> m <x> <y> l
      // find 2. ' ' 
// TODO: in: LN x y z x y z out: x y m x y l
      p1 = strchr (&cbuf[3], ' ');
      if(!p1) goto L_err1;
      ++p1;
      p2 = strchr (p1, ' ');
      if(!p2) goto L_err1;
      *p2 = '\0';         // abtrennen
      ++p2;
      fprintf(fp1, "%s m %s l\n",&cbuf[3],p2);


    //----------------------------------------------------------------
    } else if(!strncmp(cbuf, "PO ", 3)) {
      // <grayVal> g <pt1> m <pt2> p <pt3> tri
      // get first 2 coords
      p1 = &cbuf[3];
      p2 = strchr (p1, ' '); if(!p2) goto L_err1;
      ++p2;
      p2 = strchr (p2, ' '); if(!p2) goto L_err1;
      *p2 = '\0';  printf(" p1 |%s|\n",p1);
      ++p2;
      p3 = strchr (p2, ' '); if(!p3) goto L_err1;
      ++p3;
      p3 = strchr (p3, ' '); if(!p3) goto L_err1;
      *p3 = '\0';  printf(" p2 |%s|\n",p2);
      ++p3;
      col_m = (col_r + col_g + col_b) / 3.;  // monochrom from red/green/blue
      fprintf(fp1, "%f g %s m %s p %s tri\n",col_m,p1,p2,p3);
      // get next 2 coords




    //----------------------------------------------------------------
    } else {
      // printf(" - unsupported: %s\n",cbuf);
      ++nUnsupp;
    }

  }
  fclose(fp2);





  //======================================================================
  fprintf(fp1, "showpage\n");
  fprintf(fp1, "%%%%EOF\n");
  fclose(fp1);

  if(nUnsupp > 0) printf("***** %ld unsupported objects ..\n",nUnsupp);


  return 0;

  L_err1:
    TX_Error ("AP_print_pvs3 format error");
    return -1;
}
*/


/*
#include "/home/fwork/devel/tst/gl2ps-1.3.9-source/gl2ps.h"

// TODO:   move gl2ps_print3 -> ../gl2ps/gl2ps.c

//================================================================
  int gl2ps_print3 (char *fNam, char *title, char *producer,
                    void gl_redraw()) {
//================================================================
// write file <fNam> from open-GL-window
// 2016-04-18   ReiterFranz
// Input:
//   fNam       outfilename; file-extension must be: "pdf"|"svg"|"eps"
//   title
//   producer
//   gl_redraw  function to redraw all openGl-objects
// Output:
//   file <fnam>
//   retCod     0=OK, else Error
//
// Example:
//   gl2ps_print3 ("test.eps", "test", "its me", display);


  FILE *fp;
  int state = GL2PS_OVERFLOW, buffsize = 0, sort, format, opts, icol;
  char ext[32], *cp1;
  GLint viewport[4];


  // extract ext from fNam
  cp1 = strrchr (fNam, '.');
  if(!cp1) {printf("**** gl2ps_print3 E001\n"); return -1;}
  ++cp1;
  strcpy(ext, cp1);


  // set format (GL2PS_PDF|GL2PS_SVG|GL2PS_TEX|GL2PS_PS)
  if(!strcmp ("pdf", ext)) {
    format = GL2PS_PDF;

  } else if(!strcmp ("eps", ext)) {
    format = GL2PS_EPS;

  } else if(!strcmp ("svg", ext)) {
    format = GL2PS_SVG;

  } else {printf("**** gl2ps_print3 E002\n"); return -2;}


  // sort = GL2PS_SIMPLE_SORT;
  sort = GL2PS_BSP_SORT;

  icol = 0;   // gray
  // opts = GL2PS_DRAW_BACKGROUND; // gray, no bitmaps

  // icol = 16;
  opts = GL2PS_DRAW_BACKGROUND | GL2PS_BEST_ROOT;
  // opts = GL2PS_DRAW_BACKGROUND | GL2PS_OCCLUSION_CULL | GL2PS_BEST_ROOT;



  glGetIntegerv (GL_VIEWPORT, viewport);


  fp = fopen(fNam, "wb");
  printf("Writing %s\n",fNam);

  while (state == GL2PS_OVERFLOW) {
    buffsize += 1024*1024;
    gl2psBeginPage (title, producer,
                    viewport,
                    format,
                    sort,
                    opts,
                    GL_RGBA, 0, NULL,
                    icol, icol, icol,
                    buffsize, fp, fNam);
    (*gl_redraw) (); // redraw all openGl-objects

    state = gl2psEndPage();
  }

  fclose(fp);

  return 0;

}
*/


/*
// REPLACES OS_dll__
//================================================================
  int _OS_dll__ (void **dll, int mode, void *data) {
//================================================================
//
/// Input:
///   mode       0 = load dll
///              1 = start function
///              2 = unload dll
///              4 = recompile dll
/// Output:
///   dll        loaded dll

// see also OS_dll_run

  int   (*up1)(void**);
  char  s1[256];
  void  *dBlock[2];


  printf("OS_dll__ %d |%s|\n",mode,(char*)data);


  //----------------------------------------------------------------
  // 0 = load DLL
  if(mode != 0) goto L_1;

  sprintf(s1, "%s%s.so",OS_get_bin_dir(),(char*)data);
    printf(" dll=|%s|\n",s1); fflush(stdout);


  if(*dll) {
    dlclose (*dll);           // unload DLL
    *dll = NULL;
  }
  *dll = dlopen (s1, RTLD_LAZY);    // dlfcn.h
  if(*dll == NULL) {
    TX_Error("DLL_run3: cannot open dyn. Lib. |%s|",(char*)data);
    return -1;
  }

  // damit Debugger stoppt, nachdem DLL geladen wurde
  OS_debug_dll_ ((char*)data);
  return 0;




  //----------------------------------------------------------------
  // 1 = start function
  L_1:
  if(mode != 1) goto L_2;

  // Adresse von Func.fncNam holen
  up1 = dlsym (*dll, (char*)data);
  if(up1 == NULL) {
    TX_Error("OS_dll__: cannot open Func. |%s|",(char*)data);
    dlclose (*dll);           // unload DLL
    *dll = NULL;
    return -1;
  }

  // start userprog
  dBlock[0] = data;
  dBlock[1] = *dll;
  (*up1)(dBlock);
  return 0;



  //----------------------------------------------------------------
  // 2 = unload dll
  L_2:
  if(mode != 2) goto L_4;

  if(*dll) {
    dlclose (*dll);           // unload DLL
    *dll = NULL;
  }
  return 0;


  //----------------------------------------------------------------
  // 4 = recompile dll
  L_4:
  // if(mode != 2) goto L_4;

  if(*dll) {
    dlclose (*dll);           // unload DLL
    *dll = NULL;
  }
  sprintf(s1, "%s.so",(char*)data);
    printf(" dll=|%s|\n",s1); fflush(stdout);
  
  if(DLL_build__ (s1) != 0) {
     TX_Print("***** Error compile/link %s",s1);
     return -1;
  }



  return 0;

}


//================================================================
  int AP_PRI__ (int mode) {
//================================================================
//
/// Input:
///   mode       0 = load & start dll
///              2 = unload dll


  static void  *dll1 = NULL; // pointer to loaded dll "xa_print__.so"


  printf("AP_PRI__ %d\n",mode);


  //----------------------------------------------------------------
  if(mode == 0) {   // load & start
    // TESTBLOCK
    if(&dll1) OS_dll__ (&dll1,  4, "xa_print__");       // rebuild
    // END TESTBLOCK

    // load dll
    if(&dll1) OS_dll__ (&dll1,  0, "xa_print__");

    // connect func
    OS_dll__ (&dll1,  1, "PRI__");

    // start func
    OS_dll__ (&dll1,  2, NULL);


  //----------------------------------------------------------------
  } else if(mode == 2) {   // unload
    if(&dll1) _OS_dll__ (&dll1,  2, NULL); // unload

  }


  return 0;

}
*/

//================================================================
  int tst_tst__ (int ii) {
//================================================================
// general test ..
// retcod   0 exit and unload
//          1 do not unload
 

  int    irc, i1, iNr;
  double d1, d2, d3;
  char   *p1, s1[256], s2[256];
  int       triSiz, triNr, surSiz, surNr;
  int       sTyp[10];
  long      sTab[10];
  ObjGX     *oTab=NULL;
  Triangle  *triTab;
  TypTsuSur *surTab;
  Point2    p20={-4.03350,-65.62150}; // 1691
  Point2    p21={-4.03750,-65.63950}; // 2138
  Point2    p22={-2.46750,-65.68750}; // 1694
  Point2    p23={9.55,50.6};
  Point     p30={0., 0., 0.}, p31={0., 100., 0.};
  Point     p32={0., -50., 0.}, p33={0., 50., 0.};
  Point     p34;
  Vector2   v21={10., 0.};
  Vector2   v22={-15., 5.};
  Vector    v31={1., 0., 0.};
  // Vector    v32={1., 1., .0};
  // Vector    v32={0.866,    0.500, 0.};    // 30 deg
  // Vector    v32={0.707,    0.707, 0.};    // 45 deg
  Vector    v32={+0.500,   -0.866, 0.};    // 60 deg
  // Vector    v32={1.0,    1.72, 0.};    // 60 deg
  // Vector    v32={0.,    1., 0.};    // 90 deg
  // Vector    v32={-0.500,    0.866, 0.};    // 120 deg
  // Vector    v32={-0.707,    0.707, 0.};    // 135 deg
  // Vector    v32={-0.866,    0.500, 0.};    // 150 deg
  // Vector    v32={0., -1., 0.};  // 180 deg
  Vector    v33={0., 0., -1.};  // 180 deg

  // FILE *fp;
  // int state = GL2PS_OVERFLOW, buffsize = 0, format, sort, opts, icol;
  // char ext[32], fNam[256];
  // GLint viewport[4];
  char fNam[256];

  static void  *dll1 = NULL;


  printf("\n\n");
  TX_Print ("XXXXXXXXXXXXX q=Quit, l=reLoad; u=unLoad;  tst_tst__ %d\n",ii);
  printf("CXXXXXXXXXXXXXXXXXXXXXXXX  tst_tst__ %d\n",ii);
    printf(" dll1=%p\n",dll1);

  if(ii == 1) {
    UI_PRI__ (FUNC_UNLOAD);
    // i1 = _OS_dll__ (&dll1,  2, NULL); // unload
    return 0;
  }

  UI_PRI__ (FUNC_EXEC); // 0=start, 2=unload


  return 1;
  // do not yet unload (gui is active)



//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// xa_print__.mak ../xa/print__.c
// xa_print_pdf.mak
// ../exp/print_pdf.c  int PRI_PDF__ (char* fnam)
  // i1 = OS_dll_do ("xa_print__", "PRI__", "abc");

  // i1 = OS_ck_SW_is_installed ("ps2pdf");
  // printf(" ps2pdf = %d\n",i1);
  // GUI_printer__ ("C:\Users\freiter\AppData\Roaming\gCAD3D\tmp\print.eps");
  // GUI_printer__ ("/mnt/serv1/Devel/GITHUB/gcad3d/gCAD3D/tmp/print.eps");


/*
//XXXXXX GL2PS XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  // see GL_Feedback
  // pdf svg tex eps
  sprintf(fNam, "%s.eps", "test");
  gl2ps_print3 (fNam, WC_modnam, "gCAD3D", GL_Redraw);
    // GL_Redraw (); // display();   // redraw
*/



//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
/*
  sprintf(s1, "%swin.bmp",OS_get_tmp_dir());
  sprintf(s2, "%swin.jpg",OS_get_tmp_dir());

  // create BMP-file of active OpenGL-window
  bmp_save__ (s1);
  // create JPG-file from BMP-file
  OS_jpg_bmp (s2, s1);
*/


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
/*
  // AP_print__ (); // test feedbackbuffer - out -> <tempDir>/print.tmp

  AP_print_work3 ();
  AP_print_psv3 (1, "0,0", "1", "2");
  // system("v /mnt/serv1/Devel/GITHUB/gcad3d/gCAD3D/tmp/print.eps");
*/


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  // irc = UT3D_pt_int2pt2pt_lim (&p34, NULL, NULL, &p30, &p31, &p32, &p33, 0.05);
  // printf(" irc=%d\n",irc);
  // UT3D_stru_dump (Typ_PT, &p34, "p34");

  // tst_tst_1 (&v31, &v32);

  // UT3D_vc_perp2vc (&v33, &v31, &v32);
  // UT3D_stru_dump (Typ_VC, &v33, "v33");

  // d1 = UT3D_angr_3vcn_CCW (&v33, &v31, &v32);
  // printf(" d1=%f %f\n",d1, UT_DEGREES(d1));

return 0;
  // TX_Print(" plugin tst start ..");

  // sTyp[0] = Typ_SUR;
  sTyp[0] = Typ_SOL;
  sTab[0] = 20L;




  TSU_tess_sTab (&oTab, sTyp, sTab, 1);

  // space for triangles --> triTab  (12bytes/Tria)
  triTab = (Triangle*)memspc501;
  triSiz = sizeof(memspc501) / sizeof(Triangle);
    printf(" triSiz=%d\n",triSiz);

  surTab = (TypTsuSur*)memspc51;
  surSiz = sizeof(memspc51) / sizeof(TypTsuSur);
    printf(" surSiz=%d\n",surSiz);

  // get triangles from spc1 --> triTab
  // (triangles are pointers into tesselated data !)
  surNr = surSiz;
  triNr = triSiz;
  irc = TSU_tsu2tria__ (surTab, &surNr, surSiz, triTab, &triNr, triSiz, oTab);
    printf(" _tsu2tria__ irc=%d surNr=%d triNr=%d\n",irc,surNr,triNr);


    // dump_Triangle
    // for(ii=0;ii<triNr;++ii) UT3D_stru_dump (Typ_Tria,&triTab[ii],"tria");



  // free tesselated data
  if(oTab) free (oTab);



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

    case 'l':
      tst_tst__ (0);
      break;

    case 'u':
      tst_tst__ (1);
      break;

    case 't':
      printf(" GUI_Win_exist=%d\n",GUI_Win_exist ("Export/Print"));

      break;

  }

  return 0;

}



//======== EOF =========
