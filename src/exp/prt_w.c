// PS PCL5 HPGL-Export for printing.
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
\file  ../exp/prt_w.c
\brief write PS PCL5 HPGL file 
\code
=====================================================
List_functions_start:

AP_print__       init
AP_print_work2   wr GL_2D-Feedbackbuffer -> tempfile
AP_print_work3   wr GL_3D_COLOR-Feedbackbuffer -> tempfile
AP_print_vertex
AP_print_gl1     wr 0=PS 1=HPGL 2=PCL5
AP_print_psv2    wr 2D-postcript ".eps"
AP_print_psv3    wr 3D-postcript ".eps"   UNUSED (no hidden surface removal)
AP_print_pcl5h   wr PCL5-Header
AP_print_pcl5e   wr PCL5-ExitCode
AP_print_pdf     wr <tempDir>/print.pdf

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


*/


#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/gl.h>


#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_txt.h"              // UTX_CleanCR
#include "../ut/ut_os.h"               // OS_get_bas_dir ..
#include "../ut/ut_TX.h"               // TX_Print

#include "../gr/ut_GL.h"               // GL_Feedback
#include "../gr/ut_DL.h"               // DL_GetAtt
#include "../ut/func_types.h"               // UI_Func...

#include "../db/ut_DB.h"               // DB_GetGTxt

#include "../xa/xa_mem.h"              // mem_cbuf1
#include "../xa/xa.h"                  // WC_modact_nam_nam

#include "../exp/prt_w.h"              // ich


// externals:



// internal prototypes:
int AP_print_work2 (GLint size, GLfloat *buffer);
int AP_print_pcl5h (FILE *fp1, char *ptyp, int irot);
int AP_print_pcl5e (FILE *fp1);
int AP_print_TEST_LN (float x1, float y1, float x2, float y2, int iatt);






//=========================================================
  int AP_print__ () {
//=========================================================
// get Feedbackbuffer;
// write file <tempDir>/print.dat

  int      irc = 0;
  char     *cp1;
  float    *feedBuffer;
  int      size;


  // get Feedback from OpenGL into feedBuffer
  if(GL_FeedGet (&size, &feedBuffer, GL_2D) < 0) return -1;
  if(size < 6) {TX_Error("AP_print__ E001"); return -1;}


  // Feedbackbuffer ->  Hilfsdatei <tempDir>/print.tmp
  AP_print_work2 (size, feedBuffer);
  if(size < 7) {
    TX_Error ("AP_print_work2: no data");
    irc = -1;
    goto L_exit;
  }


  L_exit:
  free (feedBuffer);

  return irc;
}


//=========================================================
  int AP_print_work2 (int size, float *buffer) {
//=========================================================
// write file <tempDir>/print.tmp from GL_2D-Feedbackbuffer
// In:  Feedbackbuffer;
// Out: Objekte ->  Hilfsdatei <tempDir>/print.tmp
// Normaler Record (Point, Line, Polygon) sieht so aus:
// X-Coord Y-Coord Z-Coord R G B A

static char* txBuf=NULL;

  int     count;
  // GR_Att    att1;
  // Att_ln    *att1;
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

  printf("AP_print_work2 size=%d |%s|\n",size,cbuf);



  NextRec:
  token = buffer[count];
    // printf("........... next: %x [%d]\n",token,count),
  ++count;

      //===================================================================
      if (token == GL_PASS_THROUGH_TOKEN) {    // 0x0700
         // get uservalue (provided from func glPassThrough)
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
/* Crash bei Dimen
             ox1 = DB_GetGTxt (l1);
             gtx1 = ox1->data;
             // fprintf(fpo, "TN %s\n",gtx1->txt);
             printf("TN %s\n",gtx1->txt);
             txBuf = gtx1->txt;
           // }
*/
         }
         ++count;


      //===================================================================
      } else if (token == GL_POINT_TOKEN) {
         // printf ("%d GL_POINT_TOKEN\n",count);
         // AP_print_vertex (fpo, &count, &buffer[count]);
         fprintf(fpo, "PT %f %f\n",buffer[count],buffer[count+1]);
         // count += 7;
         // count += 3;
         count += 2;    // 2D


      //===================================================================
      } else if (token == GL_LINE_TOKEN) {    // 0x0702
         // printf ("%d GL_LINE_TOKEN\n",count);
           // AP_print_TEST_LN (buffer[count],buffer[count+1],
                             // buffer[count+2],buffer[count+3], 8);
         // AP_print_vertex (fpo, &count, &buffer[count]);
         fprintf(fpo, "LN %f %f %f %f\n", buffer[count], buffer[count+1],
                                          buffer[count+2],buffer[count+3]);
                                         // buffer[count+3],buffer[count+4]);
         // count += 6;
         count += 4;    // 2D


      //===================================================================
      } else if (token == GL_POLYGON_TOKEN) {
         nvertices = buffer[count];
         // printf ("%d GL_POLYGON_TOKEN: (%d)\n",count,nvertices);
         ++count;
         fprintf(fpo, "PO %f %f",buffer[count],buffer[count+1]);
         // count += 7;
         // count += 3;
         count += 2;  // 2D
         for (i1=1; i1<nvertices; ++i1) {
           // AP_print_vertex(fpo, &count, &buffer[count]);
           fprintf(fpo, " %f %f",buffer[count],buffer[count+1]);
           // count += 7;
           // count += 3;
           count += 2;    // 2D
         }
         fprintf(fpo, "\n");


      //===================================================================
      } else if (token == GL_BITMAP_TOKEN) {
         // printf ("%d GL_BITMAP_TOKEN\n",count);
         if(txBuf) {   // erster char einer Textnote
           fprintf(fpo,"TN %f %f %s\n",buffer[count],buffer[count+1],txBuf);
           printf("TN %f %f %s\n",buffer[count],buffer[count+1],txBuf);
           txBuf = NULL;
         }
         // count += 7;
         // count += 3;
         count += 2;    // 2D


      //===================================================================
      } else if (token == GL_DRAW_PIXEL_TOKEN) {
         // printf ("%d GL_DRAW_PIXEL_TOKEN\n",count);
         // count += 7; // liefert auch Vertex
         // count += 3;
         count += 2;    // 2D


      //===================================================================
      } else if (token == GL_COPY_PIXEL_TOKEN) {
         // printf ("%d GL_COPY_PIXEL_TOKEN\n",count);
         // count += 7; // liefert auch Vertex
         // count += 3;
         count += 2;    // 2D



      //===================================================================
      } else if (token == GL_LINE_RESET_TOKEN) {  // 0x0707
         // line with stipple reset.
         // printf ("%d GL_LINE_RESET_TOKEN\n",count);
           // AP_print_TEST_LN (buffer[count],buffer[count+1],
                             // buffer[count+2],buffer[count+3], 8);
         // AP_print_vertex (fpo, &count, &buffer[count]);
         fprintf(fpo, "LN %f %f %f %f\n",buffer[count],buffer[count+1],
                                         buffer[count+2],buffer[count+3]);
                                         // buffer[count+3],buffer[count+4]);
         // count += 6;
         count += 4;    // 2D


      //===================================================================
      } else printf ("%d **** unknown GL-TOKEN %x ****\n",size-count,token);
  

  if(count < size) goto NextRec;

  fclose(fpo);

  return 0;

}


//=========================================================
  int AP_print_work3 () {
//=========================================================
// write file <tempDir>/print.tmp from GL_3D_COLOR-Feedbackbuffer 
// UNUSED - NO occlusion culling / hidden surface removal / hidden line removal
// In:  Feedbackbuffer;
// Out: Objekte ->  Hilfsdatei <tempDir>/print.tmp
// Normaler Record (Point, Line, Polygon) sieht so aus:
// X-Coord Y-Coord Z-Coord R G B A
  
static char* txBuf=NULL;

  int      size;
  float    *buffer;

  int     count;
  // Att_ln    *att1;
  char      cbuf[256];
  FILE      *fpo;
  int       i1, i2, i3, token, nvertices, newAtt;
  int       actCol=-1, actLtyp=-1, actLthick=-1;
  long      l1;
  ObjGX     *ox1;
  GText     *gtx1;


  count = 0;


  if(GL_FeedGet (&size, &buffer, GL_3D_COLOR) < 0) return -1;
  if(size < 6) {TX_Error("AP_print_work3 E001"); return -1;}


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
/* Crash bei Dimen
             ox1 = DB_GetGTxt (l1);
             gtx1 = ox1->data;
             // fprintf(fpo, "TN %s\n",gtx1->txt);
             printf("TN %s\n",gtx1->txt);
             txBuf = gtx1->txt;
           // }
*/
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
         fprintf(fpo, "LN %f %f %f %f %f %f\n",
                 buffer[count],buffer[count+1],buffer[count+2],
                 buffer[count+7],buffer[count+8],buffer[count+9]);
         // count += 4;    // 2D
         // count += 6;   // 3D
         count += 14; // 3D-col


      //===================================================================
      } else printf ("%d **** unknown GL-TOKEN 0x%x ****\n",size-count,token);


  if(count < size) goto NextRec;

  fclose(fpo);

  free (buffer);

  return 0;

}


//=========================================================================
  int AP_print_TEST_LN (float x1, float y1, float x2, float y2, int iatt) {
//=========================================================================

  Point2  p21, p22;


  printf(" TEST_LN %d %f %f %f %f\n",iatt,x1,y1,x2,y2);


  p21.x = x1;
  p21.y = y1;

  p22.x = x2;
  p22.y = y2;

  GR_Disp_ln2 (&p21, &p22, iatt);

  return 0;

}

 
//=====================================================================
  int AP_print_vertex (FILE *fpo, int *count, float *buffer) {
//=====================================================================
/* hier eventuell eine veraenderte farbe ausgeben ..
X-Coord Y-Coord Z-Coord R G B A
*/

  int i1;

  printf ("    %d  ",*count);

  for (i1 = 0; i1 < 7; i1++) {
     printf (" %7.2f ", buffer[i1]);
  }

  printf ("\n");

  return 0;

}


//============================================================================
  int AP_print_pdf (int irot, char *pgTyp, char* off, char* scl, char* gray) {
//============================================================================
// create <tempDir>/print.pdf

  int    i1;
  char   s1[400];

  printf("AP_print_pdf \n");


  i1 = OS_ck_SW_is_installed ("ps2pdf");
  if(i1) {
    TX_Print("**** ERROR: ps2pdf is not installed. ****");
    // not installed; ERRMSG.
    return -1;
  }


  // create <tempDir>/print.eps
  AP_print_psv2 (irot, off, scl, gray);


  // ps2pdf -sPAPERSIZE=a4 print.ps print.pdf
  sprintf(s1, "ps2pdf -sPAPERSIZE=a%c \"%sprint.eps\" \"%sprint.pdf\"",
          pgTyp[1], OS_get_tmp_dir(), OS_get_tmp_dir());
    printf("%s\n", s1);
  system (s1);

  return 0;

}


//=====================================================================
  int AP_print_psv2 (int irot,char* off,char* scl,char* gray) {
//=====================================================================
// create <tempDir>/print.eps from 2D-file print.tmp
// Input:
//   irot     0, 1=90 deg rotated
//   off      "0,0"    offset
//   scl      "1"      text-scale
//   gray     "2"      unused
//   file <tempDir>/print.tmp
//   file <cfgDir>/psv.setup
// Output:
//   file <tempDir>/print.eps

/* siehe auch AP_Print0
  Die Farben:
     0 COL_Default
     1 COL_BLACK
     2 COL_ROT
     3 COL_GRUEN
     4 COL_BLAU
     5 COL_GELB
     6 COL_MAGENTA
     7 COL_CYAN
     8 COL_WEISS
     9 hili
    10 COL_DIMMED
    11 blaugrau f. faces
    12 kupferrot f. faces
    13 grau f. faces
    14 dim

  Die Linientypen:
    0 voll
    1 str punkt
    2 strichl
    3 strichl. lang

  Die Liniendicken:
    1-6;    1=dünn, 6 ist am dicksten.
*/

  GLint   GL_Viewp[4];         // x-left, y-low, width, heigth
  double  d1, fscl;
  int     i1, i2, iw, ih, xOff=0, yOff=0, igray,
          newCol, newLtyp, newLthick, actCol=-1, actLtyp=-1, actLthick=-1;
  long    l1, nUnsupp;
  float   col_r, col_g, col_b, col_m;
  char    cbuf[256], *p1, *p2, *p3;
  FILE    *fp1, *fp2;



  printf ("AP_print_psv2 %d |%s|%s|%s|\n",irot,off,scl,gray);




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
    TX_Error ("AP_print_psv2 E001");
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
    TX_Error ("AP_print_psv2 file: Error open %s",cbuf);
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
/*
  i1 = iw*fscl;
  i2 = ih*fscl;
  fprintf(fp1, "%d %d scale\n",i1,i2);
*/
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
    TX_Error ("AP_print_pvs format error");
    return -1;
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
/*
  i1 = iw*fscl;
  i2 = ih*fscl;
  fprintf(fp1, "%d %d scale\n",i1,i2);
*/
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


//=====================================================================
  int AP_print_gl1 (int mode,char *ptyp,int irot,char* off,char* scl) {
//=====================================================================
// create <tempDir>/print.hpgl|pcl      (HPGL|PCL5)
// input: <tempDir>/print.tmp
// In: mode = 0=PS 1=HPGL 2=PCL5
// In: ptyp = "A3" oder "A4"
// rot=0=normal,1=90Grad_drehen.


  GLint   GL_Viewp[4];         // x-left, y-low, width, heigth
  double  d1, fscl, wx, wy, xOff=0, yOff=0;
  int     newCol, newLtyp, newLthick, actCol=-1, actLtyp=-1, actLthick=-1;
  long    l1, nUnsupp;
  char    cbuf[256], *p1, *p2;
  FILE    *fp1, *fp2;


  printf ("AP_print_gl1\n");


  glGetIntegerv (GL_VIEWPORT, GL_Viewp);  // get Viewport-Matrix
  printf(" viewp %d %d %d %d\n",GL_Viewp[0], GL_Viewp[1],
                                GL_Viewp[2], GL_Viewp[3]);
  wx = GL_Viewp[2];
  wy = GL_Viewp[3];

  nUnsupp = 0;


  // das Querformat wird durch PCL-umschalten erreicht.


  // Hochformat = Portrait; irot=0
  // es wird ca ausgegeb SC 0 210 0 297
  if(irot != 0) goto L_quer;
  d1 = 1.41414141;      // d1 = 297/210;
  // ymax muss xmax * d1 sein.
  wy = wx * d1;


  goto L__1;



  // Querformat = Landscape; irot=1
  // es wird ca ausgegeb SC 0 297 0 210
  L_quer:
  d1 = 0.70707070;     // d1 = 210/297:
  // ymax muss xmax * d1 sein.
  wy = wx * d1;





  //========================================================
  L__1:
  xOff = strtod (off, &p2);
  ++p2;
  yOff = strtod (p2, &p1);


  fscl = strtod (scl, &p2);
    printf(" off=%f,%f, scl=%f\n",xOff,yOff,fscl);


  wx *= fscl;
  wy *= fscl;

  wx += xOff;
  wy += yOff;


  if(mode == 2) sprintf(cbuf,"%sprint.pcl",OS_get_tmp_dir ());
  else          sprintf(cbuf,"%sprint.hpgl",OS_get_tmp_dir ());
  if ((fp1 = fopen (cbuf, "w+")) == NULL) {
    TX_Error ("AP_print_gl1 E001");
    return -1;
  }


  if(mode == 2) AP_print_pcl5h (fp1, ptyp, irot);  // PCL5: add Header


  fprintf(fp1, "IN;SC%f,%f,%f,%f;\n",xOff,wx,yOff,wy);


  if(mode == 1) {                 // HPGL only
    if(irot != 0) {               // Querformat
      fprintf(fp1, "RO90;\n");
    }
  }



  //======================================================================
  // data
  sprintf(cbuf,"%sprint.tmp",OS_get_tmp_dir ());
  if ((fp2 = fopen (cbuf, "r")) == NULL) {
    TX_Error ("AP_print_gl1  Error open print.tmp");
    return -1;
  }



  //======================================================================
  while (!feof (fp2)) {
    if (fgets (cbuf, 250, fp2) == NULL) break;
    UTX_CleanCR (cbuf);                            // remove foll CR,LF ..
    // printf(" in:|%s|\n",cbuf);


    //========= Graf. Attribute ===========================================
    if(!strncmp(cbuf, "AT ", 3))  {
      // Graf Att. Col Ltyp Lthick
      sscanf(&cbuf[3], "%d %d %d",&newCol,&newLtyp,&newLthick);
      printf(" att: col=%d lt=%d thi=%d\n",newCol,newLtyp,newLthick);
      // LineTyp (Dash ..)
      if(newLtyp != actLtyp) {
        actLtyp = newLtyp;
        if(newLtyp == 0) {           // solid
          fprintf(fp1, "LT;\n");
        } else if(newLtyp == 1) {    // dash-dot
          fprintf(fp1, "LT4;\n");
        } else if(newLtyp == 2) {    // shortDash
          fprintf(fp1, "LT2;\n");
        } else if(newLtyp == 3) {    // longDash
          fprintf(fp1, "LT3;\n");
        } else printf("**** AP_print_pvs unsupp. lintyp %d\n",newLtyp);
      }
      // LineThickness
      if(newLthick != actLthick) {
        actLthick = newLthick;
        fprintf(fp1, "SP%d;\n",newLthick+1);
      }


    //========= TEXTNOTE ==================================================
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
      fprintf(fp1, "PU%s;LB%s%c\n",&cbuf[3],p2,3);



    //========= POINT =====================================================
    } else if(!strncmp(cbuf, "PT ", 3))  {
      // <x> <y> _pt
      fprintf(fp1, "PU%s;CI1;\n", &cbuf[3]);



    //========= LINE ======================================================
    } else if(!strncmp(cbuf, "LN ", 3)) {
      // <x> <y> m <x> <y> l
      // find 2. ' '
      p1 = strchr (&cbuf[3], ' ');
      if(!p1) goto L_err1;
      ++p1;
      p2 = strchr (p1, ' ');
      if(!p2) goto L_err1;
      *p2 = '\0';         // abtrennen
      ++p2;
      fprintf(fp1, "PU%s;PD%s;\n",&cbuf[3],p2);


    } else {
      // printf(" - unsupported: %s\n",cbuf);
      ++nUnsupp;
    }

  }



  //======================================================================

  fprintf(fp1, "SP;PG;\n");

  if(mode == 2) AP_print_pcl5e (fp1);

  fclose(fp2);
  fclose(fp1);

  if(nUnsupp > 0) printf("***** %ld unsupported objects ..\n",nUnsupp);

  return 0;


  L_err1:
    TX_Error ("AP_print_gl1 format error");
    return -1;



}

//=====================================================================
  int AP_print_pcl5h (FILE *fp1, char *ptyp, int irot) {
//=====================================================================
/* den PCL5-Header schreiben
In: ptyp = "A3" oder "A4"
In: irot = 0=normal,1=90Grad_drehen.

^[E                        // Reset
^[&l26A                    // Paper A4 (A3=l27A)
^[&l1O                     // Orientation; 0O=Portrait, 1O=Landsc.
^[%1B                      // Enter HPGL
IN;...                     // HPGL-Data
^[%1A                      // Enter PCL5   (^[51A ??)
^[E                        // Reset / eject page
*/



  fprintf(fp1, "%cE\n", 27);
  if(ptyp[1] == '4') {
    fprintf(fp1, "%c&l26A\n", 27);   // A4
  } else if(ptyp[1] == '3') {
    fprintf(fp1, "%c&l27A\n", 27);   // A3
  }
  if(irot == 0) {
    fprintf(fp1, "%c&l0O\n", 27);
  } else {
    fprintf(fp1, "%c&l1O\n", 27);
  }
  fprintf(fp1, "%c%%1B\n", 27);

  return 0;

}

//=====================================================================
  int AP_print_pcl5e (FILE *fp1) {
//=====================================================================
/* PCL5-ExitCode schreiben

^[%1A                      // Enter PCL5   (^[51A ??)
^[E                        // Reset / eject page

*/



  fprintf(fp1, "%c&1A\n",27);
  fprintf(fp1, "%cE\n",27);

  return 0;

}


/*
PS - Bitmap; unused.

//=====================================================================
  int AP_Print0 (int mode,char* cmd,int irot,char* off,char* scl) {
//=====================================================================

// 1=view
// 2=File
// 3=print

  int     i1, i2, iw, ih, ilen, xOff=0, yOff=0, igray;
  char    *p1, *p2;
  double  d1, fscl;
  void    *dat;
  FILE    *fp1;



   printf("AP_Print0 %d |%s|%s|%s|\n",mode,cmd,off,scl);


  d1 = strtod (off, &p2);
  xOff = d1;
  ++p2;
  d1 = strtod (p2, &p1);
  yOff = d1;

  // d1 = strtod (gray, &p2);
  igray = 2;

  fscl = strtod (scl, &p2);
  printf(" off=%d,%d, scl=%f gray=%d\n",xOff,yOff,fscl,igray);


  // load ilen Pixels -> dat
  dat = GL_Print1 (&iw, &ih);
  if(dat == NULL) {
    // fclose(fp1);
    return -1;
  }

  ilen = iw * ih;

  



  //============== create fileheader ===============================
  strcpy(txbuf, OS_get_bas_dir ());
  strcat(txbuf, "/tmp/print.eps");

  if ((fp1 = fopen (txbuf, "w+")) == NULL) {
    TX_Error ("*** eror open printfile ****");
    return -1;
  }

  fprintf(fp1, "%%!PS-Adobe-2.0 EPSF-2.0\n");
  if(irot == 0) {
    i1=xOff+iw;
    i2=yOff+ih;
  } else {
    i2=xOff+iw;
    i1=yOff+ih;
  }
  fprintf(fp1, "%%%%BoundingBox: %d %d %d %d\n",xOff,yOff,i1,i2);
  fprintf(fp1, "%%%%Pages: 1\n");
  fprintf(fp1, "%%%%EndComments\n");
  fprintf(fp1, "%%%%Page: 1 1\n");
  if(irot == 0) {
    fprintf(fp1, "%d %d translate\n",xOff,yOff);
  } else {
    fprintf(fp1, "%d %d translate\n",xOff+ih,yOff);
    fprintf(fp1, "90 rotate\n");
  }
  i1 = iw*fscl;
  i2 = ih*fscl;
  fprintf(fp1, "%d %d scale\n",i1,i2);
  fprintf(fp1, "/scanline %d 1 mul string def\n",iw);
  fprintf(fp1, "%d %d 8 [ %d 0 0 %d 0 0 ]\n",iw,ih,iw,ih);
  fprintf(fp1, "{ currentfile scanline readhexstring pop } image\n");
  // fprintf(fp1, "%s  // Open-baseDir\n",AP_dir_open);

  // in Datei raus
  AP_PrintWrHex (fp1, (unsigned char*)dat, ilen, igray);

  free(dat);

  fprintf(fp1, "showpage\n");
  fprintf(fp1, "%%%%EOF\n");
  fclose(fp1);





  //============== execute show / print ===================================
  if(mode != 2)  goto L_mode1_3;

  // out into file
  strcpy(txbuf, "cp ");
  strcat(txbuf, OS_get_bas_dir ());
  strcat(txbuf, "/tmp/print.eps ");
  strcat(txbuf, cmd);
  goto L_execute;



  L_mode1_3:
  strcpy(txbuf, cmd);
  strcat(txbuf, " ");

  // add filename
  strcat(txbuf, OS_get_bas_dir ());
  strcat(txbuf, "/tmp/print.eps");




  // execute
  L_execute:
  printf("system %s\n",txbuf);
  system(txbuf);



  return 0;

}



//=========================================================
  int AP_PrintWrHex (FILE *fpo, unsigned char *dat, int ilen, int gray) {
//=========================================================
// data als hexchar raus (ohne open, close)


  int  linLen = 30;


  int  i1, i2, icol;
  unsigned char *p1, *end;
  unsigned char lbuf[80];



  p1=dat;
  end = p1 + ilen;


  NextLine:
  lbuf[linLen*2] = '\0'; 
  for(i1=0; i1<linLen; ++i1) {
    i2 = i1 * 2;
    icol=*p1;
    if(icol < 255) {
      icol=icol/gray;                 // gray = verdunkelungsfaktor;1-n
      if(icol < 0) icol = 0;
    }
    sprintf(&lbuf[i2], "%02x",icol);
    ++p1;
    if(p1 > end) {
      if(i1 == 0) goto Fertig2;
      lbuf[i2] = '\0';
      goto Fertig1;
    }
  }
  fprintf(fpo, "%s\n",lbuf);
  goto NextLine;


  //--------------------------------
  Fertig1:
  fprintf(fpo, "%s\n",lbuf);

  Fertig2:

  return 1;

}
*/

/* ======================== EOF ====================== */
