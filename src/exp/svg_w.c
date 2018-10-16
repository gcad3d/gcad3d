// SVG export.                                    RF 2010-09-22
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
- does not work with eog; ok with firefox.
- add <svg ... width="854px" height="525px"
- add <title>..Err3</title>
  ..

-----------------------------------------------------
Modifications:
2011-07-21 stroke-width font-size PointsPoistion fixed. RF.
2010-09-22 created. RF.

-----------------------------------------------------
*/
/*!
\file  ../exp/svg_w.c
\brief write SVG file 
\code
=====================================================
List_functions_start:

SVG_w__            mainEntry
SVG_w_obj
SVG_w_pt           export point
SVG_w_ln3
SVG_w_ln2
SVG_w_cir_f
SVG_w_cir_2r
SVG_w_cir_r
SVG_w_crv
SVG_w_bez
SVG_w_ell_360
SVG_w_ell_r
SVG_w_ell_wr
SVG_w_plg
SVG_w_txt_copy
SVG_w_gtxt__
SVG_w_gtxt_c
SVG_w_sym
SVG_w_dim__
SVG_w_trPt
SVG_w_tra
SVG_w_box
SVG_w_fmt1
SVG_w_fmt2
SVG_w_fmti2
SVG_w_setLtyp

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



=====================================================
SVG-FormatDescription:
=====================================================

<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<!-- Testprogramm, manuell.
  fill="#00ff00" stroke="#000000"
  PUBLIC "-//W3C//DTD SVG 1.0//EN"
  "http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd" [
-->

<!-- Styles. -->
<!DOCTYPE svg [
  <!ENTITY st1 "stroke:blue;fill:none">
  <!ENTITY st_p_1 "stroke:black;stroke-width:1">
  <!ENTITY st_l_d "stroke:black;stroke-dasharray:5,2;fill:none">
  <!ENTITY st_t_1 "font-size:12px;font-family:Bitstream Vera Sans">
  <!ENTITY st_t_2 "font-size:6px;font-family:Bitstream Vera Sans">
]>

<svg
  xmlns="http://www.w3.org/2000/svg"
  xmlns:xlink="http://www.w3.org/1999/xlink"
  version="1.1"
  width="210mm" height="297mm"
  viewBox="0 0 210 297"
>

<defs>
  <!-- DefaultStyles. -->
  <style type="text/css"><![CDATA[
    line { stroke:black; stroke-width:0.2; }
  ]]></style>
  <style type="text/css"><![CDATA[
    circle { fill:none; stroke:black; stroke-width:0.2; }
  ]]></style>
  <style type="text/css"><![CDATA[
    ellipse { fill:none; stroke:black; stroke-width:0.2; }
  ]]></style>
  <style type="text/css"><![CDATA[
    path { fill:none; stroke:black; stroke-width:0.2; }
  ]]></style>
  <!-- SymbolPrototypes (group). -->
  <g id="sym1">
    <rect x="0" y="0" width="18" height="18" fill="red" stroke="green"/>
  </g>
</defs>

<line x1="0" y1="200" x2="100" y2="200"/>
<line x1="0" y1="220" x2="100" y2="220" style="&st1;"/>

<circle cx="100" cy="100" r="20"/>
<circle cx="160" cy="100" r="20" style="&st1;"/>

<ellipse cx="180" cy="40" rx="20" ry="10" style="&st_l_d;" />
<ellipse transform="translate(320 181) rotate(-30)" rx="36" ry="17"/>

<rect x="90" y="90" width="15" height="6"
  fill="none" stroke="blue" stroke-width=".2"
/>

<!-- Polygon. Closed! -->
<polygon points="85 0 125 25 185 50"
  transform="rotate(135 125 25)"
/>

<!-- Polygon. Open or closed. -->
<path d="M65 10 L55 27 L75 27"/>

<!-- Bezier Curve. -->
<path d="M100 200 C150 200 150 250 200 250"/>

<!-- relimited circs, relimited ellipses:
a = arc (elliptical sector; |"M"sx sy "A"rx ry angX-Axis large iDir ex ey|
  large    0=small;1=large;
  iDir:    0=CW, 1=CCW;
  ex,ey    endpoint;
-->
<path id="pt1" d="M10 150 A15 15 0 1 1 25 165"/>

<!-- Points: -->
<path d="M50 120 h1 M70 120 h1" style="&st_p_1;"/>

<!-- GrafText: -->
<text x="120" y="200" style="&st_t_1;">
Test2
</text>

<!-- Text along curve with offset (dimension-text): -->
<text style="&st_t_2;">
<textPath xlink:href="#pt1" startOffset="25%">
TextPath
</textPath>
</text>

<!-- Bitmap-Image: -->
<image x="20" y="180" width="50px" height="50px"
  xlink:href="sample_Ziegel1.bmp"/>

<!-- use internal group -->
<use xlink:href="#sym1" transform="translate(20,2.5) rotate(10)" />
<use xlink:href="#sym1" transform="translate(50,2.5) rotate(10)" />

</svg>

-----------------------------------------------------
path: 
  m = moveto (set current position, without line drawing)
  l = lineto (draws the stroke)
  c = curveto (draws the cubic Bezier curve)
  q = quadratic Bézier
  a = arc / elliptical sector
  z = closepath (closes the current vector area)

style="fill:blue;
       fill-opacity:.5;
       stroke-linecap:round;
       stroke-dashoffset:50;
       stroke:red;
       stroke-opacity:.5;
       stroke-linejoin: bevel;
       stroke-dasharray:300,100" />

  fill-rule:evenodd; ??

In einer Group gelten Attribute fuer alle Objekte !

-----------------------------------------------------
Offen:
scale
matrix
onclick ?

Fonts: Arial od Verdana od serif od "Bitstream Vera Sans"

text-anchor="start|middle|end"       // text alignment hor; Ok.      

baseline-shift="20%"   geht in Firefox ned; daher via programm anheben !!!!!!!
<text .. textlength="150"                                    GEHT NED !
<textArea x y width heigth>text<tbreak/>2.Zeile</textArea>   GEHT NED !



-----------------------------------------------------
Test svg-Files:
firefox <fn>.svg


-----------------------------------------------------
*/



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef _MSC_VER
#define _CRT_SECURE_NO_DEPRECATE
__declspec(dllexport) int SVG_w__ (char*);
#define extern __declspec(dllimport)
#endif


// #include "../ut/ut_umem.h"            // UME_reserve
#include "../ut/ut_geo.h"
#include "../ut/ut_txt.h"
#include "../ut/ut_TX.h"
#include "../ut/ut_txTab.h"               // UtxTab
#include "../ut/ut_os.h"                  // OS_
#include "../ut/ut_elli.h"                // UT3D_ango_el

#include "../gr/ut_DL.h"                  // DL_GetAtt
#include "../gr/ut_GL.h"                  // GL_GetCen
#include "../gr/ut_gr.h"                  // GTX_chh_
#include "../ut/func_types.h"               // SYM_SQUARE ..

#include "../db/ut_DB.h"                  // DB_GetObjGX

#include "../xa/xa_mem.h"                 // memspc501
// memspc501  pointBuffer for curve -> pTab
// mem_cbuf1  outputbuffer (line)  cBuf



static  FILE     *SVG_fp1;
static  double   SVG_siz_x, SVG_siz_y;
static  double   SVG_lu_x,  SVG_lu_y;
static  double   SVG_rl_x,  SVG_rl_y;


static  char     *SVG_sBuf;             // outputbuffer (line)
static  int      SVG_sSiz;              // size of outputbuffer SVG_sBuf

static  char     *SVG_tBuf;             // textbuffer (for text, dimensiontext))
static  int      SVG_tSiz;              // size of outputbuffer SVG_tBuf
static  int      SVG_tlNr;              // nr of lines in SVG_tBuf
static double    SVG_lTypScl;           // scalefactor for lineThickness

static double    SVG_txsiz;             // textsize
static double    SVG_txdimsiz;          // DimensionsTextSize
static double    SVG_ardx, SVG_ardy;    // arrowsize

static double    SVG_txScaleFakt = 1.75;
static double    SVG_tActSiz;           // size of active text
static unsigned long SVG_iAtt;          // iatt from DL_Att
static char      SVG_sAtt[256];



//----------------------------------------------------------------
// extern:
// ex ca.c:
extern double    AP_txsiz;              // Notes-Defaultsize
extern double    AP_txdimsiz;           // Dimensions-Text-Defaultsize

// from ../ci/NC_Main.c
extern int     APT_dispPT;



//----------------------------------------------------------------
// protos:
  char* SVG_w_fmt1 (char *outBuf, char *tx1, double d1);
  char* SVG_w_fmt2 (char *outBuf, char *tx1, double d1, double d2);
  char* SVG_w_fmti2 (char *outBuf, char *tx1, int i1, int i2);





//===========================================================================
  int SVG_w__ (char* fnam)  {
//===========================================================================
/// export as SVG

// loop tru DL; resolv subModels later (save subModelNr,
//   loop tru all subModels (see AP_ExportIges_Model)


// see AP_ExportDxf    - loop tru DL
// see AP_ExportIges__ - loop tru models, loop tru db (AP_ExportIges_Model)
// ED_work_PP

  int      i1, irc, anz_obj=0, apt_typ;
  long     gr_ind, l1, apt_ind;
  double   d1, d2;
  char     *p1;
  ObjGX    ox1, *op1;
  DL_Att   dla;



  printf("SVG_w__ |%s|\n",fnam);

  if ((SVG_fp1 = fopen (fnam, "w")) == NULL) {
    // TX_Error ("beim Öffen der Ausgabedatei ****");
    TX_Error ("Open file %s",fnam);
    MSG_err_1 ("FileOp",fnam);
    return -1;
  }


  //----------------------------------------------------------------
  // init
  SVG_sBuf = mem_cbuf1;
  SVG_sSiz = mem_cbuf1_SIZ;

  SVG_tBuf = memspc50;
  SVG_tSiz = sizeof(memspc50);

  SVG_txsiz = GTX_chh_ (AP_txsiz);     // size of Text
    printf(" txsiz = %f\n",SVG_txsiz);

  // size of DimensionText; SVG_txScaleFakt: Anpassung an den SVG-Font!
  SVG_txdimsiz = GTX_chh_ (AP_txdimsiz) * SVG_txScaleFakt;
    printf(" txdimsiz = %f\n",SVG_txdimsiz);


  // get arrowsizes
  GTX_ardxy (&SVG_ardx, &SVG_ardy);
    // printf(" ard %f %f\n",SVG_ardx, SVG_ardy);


  //----------------------------------------------------------------
  // compute scalefactor for lineThickness
  // d1 = GL_get_Scale ();
  AP_Get_scale (&d1);
  i1 = UTP_db_rnd5 (d1);
  d2 = i1;
  SVG_lTypScl = d2 / 100;
    printf(" scl %f %f %f\n",SVG_lTypScl,d1,d2);



  //----------------------------------------------------------------
  fprintf(SVG_fp1,
  "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");
  fprintf(SVG_fp1,
  "<!-- gCAD3D SVG-Export V1.0 -->\n");

  // write styles
  fprintf(SVG_fp1, "<!DOCTYPE svg [\n");

  // defStyle points
  sprintf(SVG_sBuf,
  "<!ENTITY st_p_1 \"stroke:black;stroke-width:%.1f\">\n",
          SVG_lTypScl*0.6);
  fprintf(SVG_fp1, "%s", SVG_sBuf);

  // defStyle lines-dashed
  sprintf(SVG_sBuf,
  "<!ENTITY st_ltyp2 \"stroke:black;stroke-dasharray:%.f,%.f;stroke-width:%.1f;fill:none\">\n",
          SVG_lTypScl*4, SVG_lTypScl*2, SVG_lTypScl*0.2);
  fprintf(SVG_fp1, "%s", SVG_sBuf);

  // defStyle lines-dash-dot
  fprintf(SVG_fp1,
  "<!ENTITY st_ltyp3 \"stroke:black;stroke-dasharray:%.f,%.f,%.f,%.f;stroke-width:%.1f;fill:none\">\n",
          SVG_lTypScl*6, SVG_lTypScl*2, SVG_lTypScl*0.5, SVG_lTypScl*2,
          SVG_lTypScl*0.1);

  // defStyle Text-Graf
  // text for notes
  fprintf(SVG_fp1,
  "<!ENTITY st_t_1 \"font-size:10px;font-family:Bitstream Vera Sans\">\n");
  // text for dimensions
  fprintf(SVG_fp1,
  "<!ENTITY st_t_2 \"font-size:5px;font-family:Bitstream Vera Sans\">\n");
  // curves for dimensions
  fprintf(SVG_fp1,
  "<!ENTITY st_c_dim \"stroke:black;stroke-width:%.1f\">\n",
          SVG_lTypScl*0.1);


  // defStyle Text-Dimensions
  fprintf(SVG_fp1,
  "<!ENTITY st_t_dim \"font-size:%.1fpx;font-family:Verdana\">\n",
          SVG_txdimsiz);

  fprintf(SVG_fp1,"]>\n");


  // Header
  fprintf(SVG_fp1,"<svg\n");
  fprintf(SVG_fp1,"  xmlns=\"http://www.w3.org/2000/svg\"\n");
  fprintf(SVG_fp1,"  xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n");
  fprintf(SVG_fp1,"  version=\"1.1\" \n");
  // fprintf(SVG_fp1,"  width=\"210mm\" height=\"297mm\"\n");
  SVG_w_box ();   // viewBox=
  fprintf(SVG_fp1,">\n");


  //----------------------------------------------------------------
  fprintf(SVG_fp1,"<defs>\n");

  // DefaultStyle Circles.
  fprintf(SVG_fp1,"<style type=\"text/css\"><![CDATA[\n");
  fprintf(SVG_fp1,"  circle { fill:none; stroke:black; stroke-width:%.1f}\n",
          SVG_lTypScl*0.2);
  fprintf(SVG_fp1,"]]></style>\n");

  // DefaultStyle Lines.
  fprintf(SVG_fp1,"<style type=\"text/css\"><![CDATA[\n");
  fprintf(SVG_fp1,"  line { stroke:black; stroke-width:%.1f}\n",
          SVG_lTypScl*0.2);
  fprintf(SVG_fp1,"]]></style>\n");

  // DefaultStyle 360-deg ellipse
  fprintf(SVG_fp1,"<style type=\"text/css\"><![CDATA[\n");
  fprintf(SVG_fp1,"  ellipse { fill:none; stroke:black; stroke-width:%.1f}\n",
          SVG_lTypScl*0.2);
  fprintf(SVG_fp1,"]]></style>\n");

  // DefaultStyle Polygon, relimited circs, bezier-curves.
  fprintf(SVG_fp1,"<style type=\"text/css\"><![CDATA[\n");
  fprintf(SVG_fp1,"  path { fill:none; stroke:black; stroke-width:%.1f}\n",
          SVG_lTypScl*0.2);
  fprintf(SVG_fp1,"]]></style>\n");

  // DefaultStyle text
  fprintf(SVG_fp1,"<style type=\"text/css\"><![CDATA[\n");
  fprintf(SVG_fp1,"  text { font-size:4px;font-family:Bitstream Vera Sans}\n");
  fprintf(SVG_fp1,"]]></style>\n");


  // symbol-protoypes
  // <g id=\"Arr1\"> <path d=\"M3 1 0 0 3 -1\"/> </g>
  SVG_sBuf[0] = '\0';
  p1 = SVG_w_fmt2 (SVG_sBuf, "<g id=\"Arr1\"> <path d=\"M",SVG_ardx,SVG_ardy);
  p1 = SVG_w_fmt2  (SVG_sBuf, " 0 0 ",SVG_ardx,-SVG_ardy);
  fprintf(SVG_fp1, "%s\" style=\"stroke-width:%.1f\" />  </g>\n",
          SVG_sBuf, SVG_lTypScl*0.1);

  // <g id=\"Arr2\"> <path d=\"M-3 1 0 0 -3 -1\"/> </g>
  SVG_sBuf[0] = '\0';
  p1 = SVG_w_fmt2 (SVG_sBuf, "<g id=\"Arr2\"> <path d=\"M",-SVG_ardx,SVG_ardy);
  p1 = SVG_w_fmt2  (SVG_sBuf, " 0 0 ",-SVG_ardx,-SVG_ardy);
  fprintf(SVG_fp1, "%s\" style=\"stroke-width:%.1f\" />  </g>\n",
          SVG_sBuf, SVG_lTypScl*0.1);




  fprintf(SVG_fp1,"</defs>\n");


  //----------------------------------------------------------------
  // loop tru DL
  gr_ind = GL_Get_DLind();
  for(l1=0; l1<gr_ind; ++l1) {

    // dla = DL_GetAtt(l1);                      // get DL-Rec
    irc = DL_get_dla (&dla, l1);  
    if(!irc) continue;

    // if(dla.disp   == OFF) continue;           // skip hidden obj's
    if(DL_OBJ_IS_HIDDEN(dla)) continue;        // skip hidden obj's

    SVG_iAtt = dla.iatt;
    SVG_sAtt[0] = '\0';

    apt_typ = dla.typ;
    apt_ind = dla.ind;

    ox1 = DB_GetObjGX (apt_typ, apt_ind);    // geb DB-obj
    if(ox1.typ == Typ_Error) continue;       // skip unknown types


    irc = SVG_w_obj (&ox1);                  // work obj
    if(irc >= 0) ++anz_obj;

  }



  //----------------------------------------------------------------
  // Close
  fprintf(SVG_fp1,"</svg>\n");
  fclose(SVG_fp1);


  // Statistik ausgeben
  TX_Print("SVG-Export nach Datei %s",fnam);
  TX_Print("   Objekte exportiert:  %d",anz_obj);

  return 0;


}


//================================================================
  int SVG_w_obj (ObjGX *ox1) {
//================================================================
// write obj ox1 -> svg-file
// see also dxfw_rec


  int      typ, i1, i2, i3, i4, i5, i6, i7, i8;
  double   d1, d2;


  // printf("--------------------------------------- \n");
  // printf("SVG_w_obj %d %d\n",ox1->form,ox1->typ);


  if(ox1->typ == Typ_ObjGX) {
    typ = ox1->form;
  } else if(ox1->typ == Typ_Note) {
    typ = ox1->form;
  } else {
    // bei den Kurven ...
    typ = ox1->typ;
  }



  switch (typ) {


    case Typ_PT:
      if(APT_dispPT != OFF) {
        SVG_w_pt ((Point*)ox1->data);
      }
      break;


    case Typ_LN:
      // <line x1="0" y1="200" x2="100" y2="200"/>
      SVG_w_setLtyp ();
      SVG_w_ln3 ((Line*)ox1->data);
     break;


    case Typ_CI:
      SVG_w_setLtyp ();
        // UT3D_stru_dump (Typ_CI,  ox1->data, " Circ:");
      // test for full circle
      if(UT3D_ck_ci360 ((Circ*)ox1->data) == 0) goto L_CI__;


      L_AC__:    // relimited circ.
      SVG_w_cir_r ((Circ*)ox1->data);
      break;


      L_CI__:   // 360-deg-circ
      SVG_w_cir_f ((Circ*)ox1->data);
      break;

      L_CI_schief:
      L_AC_schief:



    //=========================================================
    case Typ_CVPOL:                                 // 8
    case Typ_CVPOL2:
      SVG_w_setLtyp ();
      // <path d="M65 10 L55 27 L75 27"/>
      SVG_w_plg (((CurvPoly*)ox1->data)->ptNr, ((CurvPoly*)ox1->data)->cpTab);
      break;


    case Typ_CVELL:                                // 13
      SVG_w_setLtyp ();
      // test for 360-deg ellipse
      if(UT3D_ck_el360 ((CurvElli*)ox1->data) != 0) goto L_ELL__;
      SVG_w_ell_360 ((CurvElli*)ox1->data);
      break;


      L_ELL__:    // relimited circ.
      SVG_w_ell_r ((CurvElli*)ox1->data);
      break;


    case Typ_CVBSP:
    case Typ_CVRBSP:
    case Typ_CVCLOT:
    case Typ_CVTRM:
      SVG_w_setLtyp ();
      SVG_w_crv (ox1);
      break;


    //=========================================================
    case Typ_GTXT:            // 41   typ=Typ_Note
      SVG_w_gtxt__ ((GText*)ox1->data);
      break;


    case Typ_Dimen:           // 42   typ=Typ_Note
      SVG_w_dim__ ((Dimen*)ox1->data);
      break;

/*



    // case Typ_SUR:
    // case Typ_SURBSP:
    // case Typ_SURRBSP:
    // case Typ_SURRV:
    // case Typ_SURRU:
      // ox1 = (ObjGX*)oxi->data;
      // ox2 = &ox1[1];              // Aussenkontur
      // SVG_w_ell (ox2);
      // break;



Typ_Model   // ModelReference



*/


    //=========================================================
    default:
        printf("  svg_w skip %d\n",typ);
      return -1;

  }



  return 0;

}


//================================================================
  int SVG_w_pt (Point* pt1) {
//================================================================
// export point

  double   d1, d2, x1, y1;
  char     *p1;


  d1 = pt1->x;
  d2 = pt1->y;
  SVG_w_trPt (&x1, &y1, &d1, &d2);


  d1 = SVG_lTypScl*0.6;
  x1 -= (d1 / 2.);        // half thickness of point to the left ..


  SVG_sBuf[0] = '\0';
  p1 = SVG_w_fmt2 (SVG_sBuf, "<path d=\"M", x1, y1);
  p1 = SVG_w_fmt1 (SVG_sBuf, " h", d1); // SVG_lTypScl*0.6);

  fprintf(SVG_fp1, "%s\" style=\"&st_p_1;\"/>\n",SVG_sBuf);

  return 0;

}



//================================================================
  int SVG_w_ln3 (Line*  ln1) {
//================================================================


  return SVG_w_ln2 ((Point2*)&ln1->p1, (Point2*)&ln1->p2);

}


//================================================================
  int SVG_w_ln2 (Point2 *pt1, Point2 *pt2) {
//================================================================

  double   d1, d2, x1, y1;
  char     *p1;


  d1 = pt1->x;
  d2 = pt1->y;
  SVG_w_trPt (&x1, &y1, &d1, &d2);

  SVG_sBuf[0] = '\0';
  p1 = SVG_w_fmt1 (SVG_sBuf, "<line x1=\"", x1);
  p1 = SVG_w_fmt1 (p1, "\" y1=\"", y1);

  d1 = pt2->x;
  d2 = pt2->y;
  SVG_w_trPt (&x1, &y1, &d1, &d2);

  p1 = SVG_w_fmt1 (p1, "\" x2=\"", x1);
  p1 = SVG_w_fmt1 (p1, "\" y2=\"", y1);

  strcat(p1, "\"");

  if(SVG_sAtt[0]) {
    strcat(p1, SVG_sAtt);
  }


  fprintf(SVG_fp1, "%s/>\n",SVG_sBuf);

  return 0;

}


//================================================================
  int SVG_w_cir_f (Circ*  ci1) {
//================================================================


  double   d1, d2, x1, y1, r1;
  char     *p1;


  d1 = ci1->pc.x;
  d2 = ci1->pc.y;
  SVG_w_trPt (&x1, &y1, &d1, &d2);

  r1 = fabs(ci1->rad);

  // fprintf(SVG_fp1,"<circle cx=\"%d\" cy=\"%d\" r=\"%d\"/>\n",i1,i2,i3);

  SVG_sBuf[0] = '\0';
  p1 = SVG_w_fmt1 (SVG_sBuf, "<circle cx=\"", x1);
  p1 = SVG_w_fmt1 (p1, "\" cy=\"", y1);
  p1 = SVG_w_fmt1 (p1, "\" r=\"", r1);

  strcat(p1, "\"");

  if(SVG_sAtt[0]) {
    strcat(p1, SVG_sAtt);
  }

  fprintf(SVG_fp1, "%s />\n",SVG_sBuf);


  return 0;

}


//================================================================
  int SVG_w_cir_2r (Circ2*  ci21) {
//================================================================


  int      i1, i2;
  double   d1, d2, x1, y1, x2, y2, rx, ry, ax;


  // UT3D_stru_dump (Typ_CI2, ci21, "SVG_w_cir_2r:");


  d1 = ci21->p1.x;
  d2 = ci21->p1.y;
  SVG_w_trPt (&x1, &y1, &d1, &d2);


  ax = 0.;        // angX-Axis

  d1 = ci21->rad;
  if(d1 > 0.) { i2 = 0; } else { i2 = 1; }     // iDir

  // // Z-vec must go up; else turn iDir
  // if(ci21->vz.dz < 0.) i2 = ICHG01 (i2);

  rx = fabs(d1);  // radius
  ry = rx;

  d1 = ci21->ango;
  if(d1 > RAD_180) { i1 = 1; } else { i1 = 0; }     // large

  d1 = ci21->p2.x;
  d2 = ci21->p2.y;
  SVG_w_trPt (&x2, &y2, &d1, &d2);


  return SVG_w_ell_wr (&x1, &y1, &rx, &ry, &ax, &i1, &i2, &x2, &y2);

}


//================================================================
  int SVG_w_cir_r (Circ*  ci1) {
//================================================================

  Circ2   ci21;

  UT2D_ci_ci3 (&ci21, ci1);

  return SVG_w_cir_2r (&ci21);

}


//================================================================
  int SVG_w_crv (ObjGX* ox1) {
//================================================================
 
  int           pNr;
  Point         *pa;


  // printf("SVG_w_crv \n");
    // UT3D_stru_dump (Typ_CVBSP, cv1, "");
    //SVG_w_plg (cv1->ptNr, cv1->cpTab);


  pa = (Point*)memspc501;
  pNr = sizeof(memspc501) / sizeof(Point);
    // elli -> polygon -> mem?


  // curve -> polygon
  UT3D_npt_ox__ (&pNr, pa, ox1, UT_DISP_cv);


  // draw polygon
  SVG_w_plg (pNr, pa);

  return 0;

}


//================================================================
  int SVG_w_bez (int pNr, Point* pTab) {
//================================================================
// write bezier-curve; q=quadratic, c=cubic
// GEHT NUR IN AUSNAHMENFAELLEN; DO NOT USE.

  int      i1, i2, ii;
  double   d1, d2;

  // printf("SVG_w_bez %d\n",pNr);


  d1 = pTab[0].x;
  d2 = pTab[0].y;
  SVG_w_tra (&i1, &i2, &d1, &d2);
  fprintf(SVG_fp1,"<path d=\"M%d %d",i1,i2);

  for(ii=0; ii<pNr; ++ii) {
    d1 = pTab[ii].x;
    d2 = pTab[ii].y;
    SVG_w_tra (&i1, &i2, &d1, &d2);
    fprintf(SVG_fp1," Q%d %d",i1,i2);
  }

  fprintf(SVG_fp1,"\"/>\n");

  return 0;

}


//================================================================
  int SVG_w_ell_360 (CurvElli* el1) {
//================================================================
// write 360-deg ellipse
// <ellipse transform="translate(320 181) rotate(-30)" rx="36" ry="17"/>
// {Point p1, p2, pc; Vector vz, va, vb; int dir;}

  double   d1, d2, x1, y1, a1, r1, r2;
  char     *p1;


  // printf("SVG_w_ell_360 \n");

      d1 = el1->pc.x;
      d2 = el1->pc.y;
      SVG_w_trPt (&x1, &y1, &d1, &d2);

      // angX-Axis
      d1 = UT2D_angr_vc ((Vector2*)&el1->va);
      d2 = UT_DEGREES (d1);
      a1 = -d2;

      r1 = UT3D_len_vc (&el1->va);   // rad-x
      r2 = UT3D_len_vc (&el1->vb);   // rad-y


  SVG_sBuf[0] = '\0';
  p1 = SVG_w_fmt2 (SVG_sBuf, "<ellipse transform=\"translate(", x1, y1);
  p1 = SVG_w_fmt1 (p1, ") rotate(", a1);
  p1 = SVG_w_fmt1 (p1, ")\" rx=\"", r1);
  p1 = SVG_w_fmt1 (p1, "\" ry=\"", r2);

  fprintf(SVG_fp1, "%s\"/>\n",SVG_sBuf);




  return 0;

}


//================================================================
  int SVG_w_ell_r (CurvElli* el1) {
//================================================================
// write ellipse, relimited.
// |"M"sx sy "A"rx ry angX-Axis large iDir ex ey|
// <path id="pt1" d="M10 150 A15 15 0 1 1 25 165"/>
// {Point p1, p2, pc; Vector vz, va, vb; int dir;}
  
  int      i1, i2;
  double   d1, d2, as, ao, x1, y1, x2, y2, rx, ry, ax;
  CurvEll2C   el2c;

  // printf("SVG_w_ell_r \n");
    // UT3D_stru_dump (Typ_CVELL, el1, "");

      UT2D_elc_el3 (&el2c, el1);

      d1 = el1->p1.x;
      d2 = el1->p1.y;
      SVG_w_trPt (&x1, &y1, &d1, &d2);

      rx = el2c.a;   // rad-x
      ry = el2c.b;   // rad-y

      // angX-Axis
      d1 = UT2D_angr_vc ((Vector2*)&el1->va);
      d2 = UT_DEGREES (d1);
      ax = -d2;

      // large; 0=small;1=large;
      // d1 = UT3D_ango_el (el1);
      UT2D_2angr_el (&as, &ao, &el2c);
        // printf(" ang0 1,2 = %f %f\n",d3,d1,d2);
      if(fabs(ao) > RAD_180) { i1 = 1; } else { i1 = 0; }


      // iDir; 0=CW, 1=CCW;
      // if(el1->dir > 0) { i2 = 0; } else { i2 = 1; }
      i2 = el1->srot;  // 0=CCW, 1=CW
      // Z-vec must go up; else turn iDir
      if(el1->vz.dz < 0.) i2 = ICHG01 (i2);
        // printf(" large %d iDir %d\n",i1,i2);

      d1 = el1->p2.x;
      d2 = el1->p2.y;
      SVG_w_trPt (&x2, &y2, &d1, &d2);


  return SVG_w_ell_wr (&x1, &y1, &rx, &ry, &ax, &i1, &i2, &x2, &y2);

}


//================================================================
  int SVG_w_ell_wr (double *x1, double *y1, double *rx, double *ry,
                   double *ax, int *i1, int *i2,
                   double *x2, double *y2) {
//================================================================
 
  char   *p1;

  SVG_sBuf[0] = '\0';
  p1 = SVG_w_fmt2  (SVG_sBuf, "<path d=\"M", *x1, *y1);     // sx sy
  p1 = SVG_w_fmt2  (p1, " A", *rx, *ry);                    // "A"rx ry
  p1 = SVG_w_fmt1  (p1, " ", *ax);                          // angX-Axis
  p1 = SVG_w_fmti2 (p1, " ", *i1, *i2);                     // large iDir
  p1 = SVG_w_fmt2  (p1, " ", *x2, *y2);                     // ex ey

  strcat(p1, "\"");

  if(SVG_sAtt[0]) {
    strcat(p1, SVG_sAtt);
  }

  fprintf(SVG_fp1, "%s/>\n",SVG_sBuf);


  return 0;

}


//================================================================
  int SVG_w_plg (int ptNr, Point* pTab) {
//================================================================
// write polygon, open or closed
// <path d="M65 10 L55 27 L75 27"/>

  
  int      i1, i2, ii;
  double   d1, d2, x1, y1;
  char     *p1, *p2;


  // printf("SVG_w_plg %d\n",ptNr);


  d1 = pTab[0].x;
  d2 = pTab[0].y;
  // SVG_w_tra (&i1, &i2, &d1, &d2);
  SVG_w_trPt (&x1, &y1, &d1, &d2);
  // fprintf(SVG_fp1,"<path d=\"M%f %f",x1,y1);

  SVG_sBuf[0] = '\0';
  p1 = SVG_w_fmt2 (SVG_sBuf, "<path d=\"M", x1, y1);
    // printf(" sBuf |%s|\n",SVG_sBuf);


  for(ii=1; ii<ptNr; ++ii) {
    d1 = pTab[ii].x;
    d2 = pTab[ii].y;
    SVG_w_trPt (&x1, &y1, &d1, &d2);
    p1 = SVG_w_fmt2 (p1, " ", x1, y1);
  }

  // fprintf(SVG_fp1,"\"/>\n");
  fprintf(SVG_fp1, "%s\"/>\n",SVG_sBuf);

  return 0;

}


//================================================================
  int SVG_w_txt_copy (char *txt) {
//================================================================
// copy text, modify specialCharacters
// Output:
//   SVG_tBuf   textouputbuffer
//   SVG_tlNr   nr of lines in SVG_tBuf

// specialCharacters: see GL_txt__
//   [+=plusMinus [g=Degree  [d=Diameter [n=newline
//   [/=higher [\\=lower
//   [0 bis [9 sind die Anzahl Nachkommastellen


  int     ii, io, slen;
  char    c1;


  // printf("SVG_w_txt_copy |%s|\n",txt);



  SVG_tlNr = 1;
  ii = 0;
  io = 0;
  slen = strlen (txt);


  NextChar:
  if(ii >= slen) goto L_exit;
  c1 = txt[ii];
    // printf(" next char %d %d\n",ii,i2);
  if(c1 == '\0') goto L_exit;

  if(c1 == '[') {                 // SONDERZEICHEN
    ++ii;
    c1 = txt[ii];
    if(c1 == 'd') {                        // d=Diameter  is char 126
      c1 = 'D';

    } else if(c1 == 'g') {                 // g=Degree    is char 127
      c1 = 'd';

    } else if(c1 == '+') {                 // +=plusMinus -> '+-'
      SVG_tBuf[io] = '+';
      ++io;
      c1 = '-';


    } else if(c1 == 'n') {                 // n=newline
      c1 = '\0';
      ++SVG_tlNr;


    } else if((c1 > '0')&&(c1 < '9')) {    // 0-9=Anzahl Nachkommastellen
      // bMod = c1 - '0';
      goto L_cont;
    }
  }


  SVG_tBuf[io] = c1;

  L_cont:
  ++ii;
  ++io;
  goto NextChar;


  L_exit:
  SVG_tBuf[io] = '\0';
    // printf("ex SVG_w_txt_copy %d\n",SVG_tlNr);


  return 0;

}


//================================================================
  int SVG_w_gtxt__ (GText* tx1) {
//================================================================
// GrafText (Note).

// see GL_DrawTxtG



  double d1, d2, x1, y1, a1;
  char   *p1, sStyl[64];


  // printf("SVG_w_gtxt__ |%s|\n",tx1->txt);
  // printf(" tx1->size=%f AP_txsiz=%f\n",tx1->size,AP_txsiz);
  // UT3D_stru_dump (Typ_GTXT, tx1, "SVG_w_gtxt__");



  d1 = tx1->pt.x;
  d2 = tx1->pt.y;
  SVG_w_trPt (&x1, &y1, &d1, &d2);


  // characterhoehe; Notes haben size -1
  if(tx1->size <= 0) SVG_tActSiz = AP_txsiz;
  else SVG_tActSiz = tx1->size;
  SVG_tActSiz *= SVG_txScaleFakt;
    // printf("SVG_tActSiz = %f\n",SVG_tActSiz);


  a1 = tx1->dir * -1.;      // Textdirection

  // copy text, modify specialCharacters > SVG_tBuf
  SVG_w_txt_copy (tx1->txt);


  sprintf (SVG_sAtt, " style=\"font-size:%.1fpx\"",SVG_tActSiz);

  SVG_w_gtxt_c (&tx1->pt.x, &tx1->pt.y, &a1);


  return 0;

}


//================================================================
  int SVG_w_gtxt_c (double *x1, double *y1,
                    double *a1) {
//================================================================
// draw text; hor|rotated, leftAligned|centered
// <text x="x1" y="y1">
// Test2
// </text>
// <text transform="translate(x1 y1) rotate(a1)" ta>
//
// Mehrzeilig: immer mit transform; jede Textzeile:
//   <tspan x="0" y="dx">3D-Text.</tspan>
//
// Input:
//   a1         rotAng
//   SVG_sAtt   text-attributes; NULL or:
//                " style=\"font-size:3.5px\"
//                " text-anchor=\"middle\" style=\"&st_t_dim;\"
//                " style="&st_t_1;"
//   SVG_tBuf   text to be displayed
// </text>

  int    i1;
  double x2, y2, dy;
  char   *p1;


  // printf("SVG_w_gtxt_c |%s| a1=%f\n",SVG_tBuf,*a1);

  SVG_w_trPt (&x2, &y2, x1, y1);

  SVG_sBuf[0] = '\0';

  if((fabs(*a1) > 0.1)||(SVG_tlNr > 1)) {
    p1 = SVG_w_fmt2 (SVG_sBuf, "<text transform=\"translate(",x2,y2);
    if(fabs(*a1) > 0.1) {
      p1 = SVG_w_fmt1 (p1, ") rotate(", *a1);
    }
    strcat (p1, ")\"");

  } else {
    p1 = SVG_w_fmt1 (SVG_sBuf, "<text x=\"", x2);
    p1 = SVG_w_fmt1 (p1, "\" y=\"", y2);
    strcat (p1, "\"");
  }


  if(SVG_sAtt[0]) {
    strcat(p1, SVG_sAtt);
  }

  fprintf(SVG_fp1, "%s>\n",SVG_sBuf);



  if(SVG_tlNr < 2) {                         // einzeilig
    fprintf(SVG_fp1, "%s\n",SVG_tBuf);

  } else {                                   // mehrzeilig
    dy = 0.;
    p1 = SVG_tBuf;
    for(i1=0; i1<SVG_tlNr; ++i1) {
      fprintf(SVG_fp1, "<tspan x=\"0\" y=\"%.1f\">%s</tspan>\n",
              dy,p1);
      p1 = &p1[strlen(p1)];
      ++p1;
      dy += SVG_tActSiz;
    }

  }

  fprintf(SVG_fp1, "</text>\n");

  return 0;

}


//================================================================
  int SVG_w_sym (char *symNam, int symNr, Point2 *pt1, double *a1) {
//================================================================
// draw symbol
// <use xlink:href="#ArrLeft" transform="translate(74.4 67.3) rotate(-330.9)" />
 
  double x2, y2;
  char   *p1;

  // printf("SVG_w_sym |%s|\n",symNam);


  SVG_w_trPt (&x2, &y2, &pt1->x, &pt1->y);

  sprintf(SVG_sBuf, "<use xlink:href=\"#%s%d\"",symNam,symNr);

  p1 = SVG_w_fmt2 (SVG_sBuf, " transform=\"translate(",x2,y2);
  p1 = SVG_w_fmt1 (p1, ") rotate(", *a1);

  fprintf(SVG_fp1, "%s)\" />\n",SVG_sBuf);

  return 0;

}


//================================================================
  int SVG_w_dim__ (Dimen *dim1) {
//================================================================
// see dxfw_rec GL_DrawDimen

  int     i1;
  double  d1, dVal, atx;
  Line2   ll1, ll2, lnd;
  Circ2   acd;
  Point2  ptx;
  Vector2 vct;
  SymRef2 hd1, hd2;
  char    dimText[256];


  // printf("SVG_w_dim__ \n");
  // UT3D_stru_dump (Typ_Dimen, dim1, "");

  strcpy (SVG_sAtt, " style=\"&st_c_dim;\"");


  //================================================================
  if(dim1->dtyp != 0) goto L_dia;
  // get leaderlines, dimensionline, symbolpositions, symboldirection,value,text
  GR_dim_lin__ (&ll1, &ll2, &lnd, &hd1, &hd2, &vct, &dVal, SVG_tBuf,
                dim1);

  // die 2 Chars "[+" umwandeln in "+-"
  UTX_chg_str2 (SVG_tSiz, SVG_tBuf, "[+", "+-");

  // den Textpunkt um (Texthoehe / 12) ueber die dimensionLine heben ..
  d1 = SVG_txdimsiz / 6.;
  UT2D_pt_tranorptvclen (&ptx, (Point2*)&dim1->p3, &vct, d1);


  // create svg-lines
  i1 = dim1->ld / 10;
  if(i1 == 1) SVG_w_ln2 (&ll1.p1, &ll1.p2);
  i1 = dim1->ld % 10;
  if(i1 == 1) SVG_w_ln2 (&ll2.p1, &ll2.p2);

  // draw dimension-line
  SVG_w_ln2 (&lnd.p1, &lnd.p2);


  // draw text; txt=SVG_tBuf; direction=vct;
  // textposition is around dim1->p3
  strcpy(SVG_sAtt," text-anchor=\"middle\" style=\"&st_t_dim;\"");
  d1 = - dim1->a1;
  SVG_w_gtxt_c (&ptx.x, &ptx.y, &d1);


  // draw arrows
  SVG_w_sym ("Arr", hd1.typ, &hd1.pt, &d1);
  SVG_w_sym ("Arr", hd2.typ, &hd2.pt, &d1);

  goto L_exit;


  //================================================================
  L_dia:    
  if(dim1->dtyp != 1) goto L_rad;

  // get dimensionline, symbolpositions, symboldirection, value, text
  GR_dim_dia__ (&lnd, &hd1, &hd2, &vct, &atx, &dVal, SVG_tBuf,
                dim1);

  // die ersten 2 Chars "[d" umwandeln in "D"
  UTX_chg_str2 (SVG_tSiz, SVG_tBuf, "[d", "D");

  // die 2 Chars "[+" umwandeln in "+-"
  UTX_chg_str2 (SVG_tSiz, SVG_tBuf, "[+", "+-");


  // den Textpunkt um (Texthoehe / 12) ueber die dimensionLine heben ..
  d1 = SVG_txdimsiz / 6.;
  UT2D_pt_tranorptvclen (&ptx, (Point2*)&dim1->p3, &vct, d1);

  // draw dimension-line
  SVG_w_ln2 (&lnd.p1, &lnd.p2);

  // draw arrows
  atx *= -1;
  SVG_w_sym ("Arr", hd1.typ, &hd1.pt, &atx);
  SVG_w_sym ("Arr", hd2.typ, &hd2.pt, &atx);

  // draw text; txt=SVG_tBuf; direction=vct;
  // textposition is around dim1->p3
  strcpy(SVG_sAtt," text-anchor=\"middle\" style=\"&st_t_dim;\"");
  SVG_w_gtxt_c (&ptx.x, &ptx.y, &atx);


  goto L_exit;


  //================================================================
  L_rad:    
  if(dim1->dtyp != 2) goto L_ang;

  // get dimensionline, symbolpositions, symboldirection, value, text
  GR_dim_rad__ (&lnd, &hd1, &vct, &atx, &dVal, SVG_tBuf,
                dim1);

  // die 2 Chars "[+" umwandeln in "+-"
  UTX_chg_str2 (SVG_tSiz, SVG_tBuf, "[+", "+-");

  // den Textpunkt um (Texthoehe / 12) ueber die dimensionLine heben ..
  d1 = SVG_txdimsiz / 6.;
  UT2D_pt_tranorptvclen (&ptx, (Point2*)&dim1->p3, &vct, d1);

  // draw dimension-line
  SVG_w_ln2 (&lnd.p1, &lnd.p2);

  // draw arrow
  atx *= -1;
  SVG_w_sym ("Arr", hd1.typ, &hd1.pt, &atx);

  // draw text; txt=SVG_tBuf; direction=vct;
  // textposition is around dim1->p3
  strcpy(SVG_sAtt," text-anchor=\"middle\" style=\"&st_t_dim;\"");
  SVG_w_gtxt_c (&ptx.x, &ptx.y, &atx);


  goto L_exit;



  //================================================================
  L_ang:    
  if(dim1->dtyp != 3) goto L_ldr;


 // get leaderlines, dimensionline, symbolpositions, symboldirection,value,text
  GR_dim_ang__ (&ll1, &ll2, &acd, &hd1, &hd2,
                &vct, &atx,
                &dVal, SVG_tBuf,
                dim1);

  // die 2 Chars "[g" umwandeln in "°"
  UTX_chg_str2 (SVG_tSiz, SVG_tBuf, "[g", "°");
  // die 2 Chars "[+" umwandeln in "+-"
  UTX_chg_str2 (SVG_tSiz, SVG_tBuf, "[+", "+-");



  // den Textpunkt um (Texthoehe / 12) ueber die dimensionLine heben ..
  d1 = SVG_txdimsiz / 6.;
    // printf(" off=%f\n",d1);
  UT2D_pt_tranorptvclen (&ptx, (Point2*)&dim1->p3, &vct, d1);
    // GR_Disp_pt2 (&ptx, SYM_STAR_S, 0);


  // create svg-lines
  i1 = dim1->ld / 10;
  if(i1 == 1) SVG_w_ln2 (&ll1.p1, &ll1.p2);
  i1 = dim1->ld % 10;
  if(i1 == 1) SVG_w_ln2 (&ll2.p1, &ll2.p2);

  // draw dimension-Arc acd
  SVG_w_cir_2r (&acd);


  // draw text; txt=SVG_tBuf; direction=vct;
  // textposition is around dim1->p3
  strcpy(SVG_sAtt," text-anchor=\"middle\" style=\"&st_t_dim;\"");
  d1 = -atx;
  SVG_w_gtxt_c (&ptx.x, &ptx.y, &d1);


  // draw arrows
  d1 = - UT_DEGREES (hd1.ang);
  SVG_w_sym ("Arr", hd1.typ, &hd1.pt, &d1);
  d1 = - UT_DEGREES (hd2.ang);
  SVG_w_sym ("Arr", hd2.typ, &hd2.pt, &d1);


  goto L_exit;


  //================================================================
  L_ldr:    // Leader
  if(dim1->dtyp != 21) goto L_unSupp;

 // get leaderlines, dimensionline, symbolpositions, symboldirection,value,text
  GR_dim_ldr__ (&ll1, &ll2, &hd1, &ptx,
                &vct, &atx,
                SVG_tBuf,
                dim1);

  // den Textpunkt um (Texthoehe / 12) ueber die dimensionLine heben ..
  d1 = SVG_txdimsiz / 6.;
    // printf(" off=%f\n",d1);
  UT2D_pt_tranorptvclen (&ptx, &ptx, &vct, d1);
    // GR_Disp_pt2 (&ptx, SYM_STAR_S, 0);

  // create svg-lines
  // i1 = dim1->ld / 10;
  // if(i1 == 1)
  SVG_w_ln2 (&ll1.p1, &ll1.p2);
  // i1 = dim1->ld % 10;
  // if(i1 == 1)
  if(dim1->p3.x != UT_DB_LEER)
    SVG_w_ln2 (&ll2.p1, &ll2.p2);

  // draw text; txt=SVG_tBuf; direction=vct;
  // textposition is around dim1->p3
  strcpy(SVG_sAtt," style=\"&st_t_dim;\"");
  d1 = -atx;
  SVG_w_gtxt_c (&ptx.x, &ptx.y, &d1);

  // draw arrows
  d1 = - UT_DEGREES (hd1.ang);
  SVG_w_sym ("Arr", hd1.typ, &hd1.pt, &d1);

  goto L_exit;



  //================================================================
  L_exit:
    return 0;

  //================================================================
  L_unSupp:
    printf("***** SVG_w_dim__ ERROR E001 %d\n",dim1->dtyp);
    return -1;
}
 

//================================================================
  int SVG_w_trPt (double *x1, double *y1, double *dx, double *dy) {
//================================================================
// translate point

  // printf("SVG_w_tra %f %f\n",*dx,*dy);


  *x1 = *dx - SVG_lu_x;
  *y1 = SVG_siz_y - (*dy - SVG_rl_y);

  // printf("   _w_tra %f %f\n",*x1,*y1);

  return 0;

}


//================================================================
  int SVG_w_tra (int *ix, int *iy, double *dx, double *dy) {
//================================================================
// translate point

  // printf("SVG_w_tra %f %f\n",*dx,*dy);


  *ix = *dx - SVG_lu_x;
  *iy = SVG_siz_y - (*dy - SVG_rl_y);

  // printf("   _w_tra %d %d\n",*ix,*iy);

  return 0;

}

 
//================================================================
  int SVG_w_box () {
//================================================================
// viewBox
// der Koordinatenraum muss links oben mit 0,0 beginnen !
// rechts unten ist SVG_siz_x,SVG_siz_y

  int      ix, iy;
  Point    ptc;
  double   xrl, yrl, d1, dx, dy;


  // get center of grWin in userCoords
  ptc = GL_GetCen ();
    // UT3D_stru_dump (Typ_PT,  &ptc, " GL_cen:");


  // get size of graficWindow in userCoords
  GL_GetViewSizU (&dx, &dy);
    // printf(" scrSiz %f %f\n",dx,dy);


  SVG_lu_x = ptc.x - (dx / 2.);         // x-val left vert. border
  SVG_rl_y = ptc.y - (dy / 2.);         // y-val of lower hor. border
    // printf(" lux = %f rly = %f\n",SVG_lu_x,SVG_rl_y);

  SVG_siz_x = dx;
  SVG_siz_y = dy;
    // printf(" siz = %f %f\n",SVG_siz_x,SVG_siz_y);

  fprintf(SVG_fp1,"  viewBox=\"-5 -5 %.1f %.1f\"\n",
    SVG_siz_x + 10, SVG_siz_y + 10);

  return 0;

}

//================================================================
  char* SVG_w_fmt1 (char *outBuf, char *tx1, double d1) {
//================================================================
// add "text double" to outBuf

  char     *p1;
    
    
  // printf("SVG_w_fmt1 |%s| %f\n",tx1,d1);


  if(tx1) {
    strcat(outBuf, tx1);
  }
  
  
  // add "double blank double" mit n Nachkommastellen
  p1 = UTX_add_fl_1uf (outBuf, d1);

    // printf(" sBuf |%s|\n",SVG_sBuf);

  return &p1[strlen(p1)];

}


//================================================================
  char* SVG_w_fmt2 (char *outBuf, char *tx1, double d1, double d2) {
//================================================================
// add "text double blank double" to outBuf

  char     *p1, *p2;
    
    
  // printf("SVG_w_fmt2 |%s| %f %f\n",tx1,d1,d2);


  if(tx1) {
    strcat(outBuf, tx1);
  }


  // add "double blank double" mit n Nachkommastellen
  p1 = UTX_add_fl_1uf (outBuf, d1);

  strcat(p1, " ");

  p2 = UTX_add_fl_1uf (p1, d2);
    // printf(" sBuf |%s|\n",SVG_sBuf);

  return &p2[strlen(p2)];

}

 
//================================================================
  char* SVG_w_fmti2 (char *outBuf, char *tx1, int i1, int i2) {
//================================================================
// add "text int blank int" to outBuf

  char     *p1, *p2;

    
  // printf("SVG_w_fmti2 |%s|%s| %d %d\n",SVG_sBuf,tx1,i1,i2);
  
  
  if(tx1) {
    strcat(outBuf, tx1);
  }
  
  p1 = &outBuf[strlen(outBuf)];

  sprintf (p1, "%d %d", i1,i2);

    // printf(" sBuf |%s|\n",SVG_sBuf);

  return &p1[strlen(p1)];

} 


//================================================================
  int SVG_w_setLtyp () {
//================================================================

  if((SVG_iAtt > 0)&&(SVG_iAtt < 4)) {
    sprintf(SVG_sAtt, " style=\"&st_ltyp%ld;\"",SVG_iAtt);
  }

  return 0;

}

 
/*
*/

// EOF
