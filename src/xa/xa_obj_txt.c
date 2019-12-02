//    xa_obj_txt.c
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
\file  ../xa/xa_obj_txt.c
\brief convert objects --> APT-Text 
\code
=====================================================
List_functions_start:

AP_stru_2_txt       convert object-struct -> source-text
AP_obj_2_txt        change obj to text and save it with UTF_add1_line

AP_obj_add_dbo      add typ,Ind; zB Typ_PT,10 --> "P10"
AP_obj_add_val      add double (10 Nachkommastellen)
AP_obj_add_nval     add n doubles (10 Nachkommastellen)
AP_obj_add_int
AP_obj_add_pt       add Point* as "P(xyz)"
AP_obj_add_pt_sp    add point mit reduced precision
AP_obj_add_pt2      add 2D-point
AP_obj_add_pt3      old version; do not use.
AP_obj_add_vc       add Vector zB "DZ" or "D(0 0 1)"
AP_obj_add_vc0      add Vector as D(0 0 1)"
AP_obj_add_ln       add Line as "L(Pa Pe)"
AP_obj_add_ci_      add Circ as "C(P(ptStart) P(ptEnd) P(ptCen) D(vz))"
AP_obj_add_ci1      add Circ as "C(P(100 100 0) 32 DZ)"
AP_obj_add_ci2      old version; do not use.
AP_obj_add_cvBsp_b  add "BSP,...."  with binary data block as ascii
AP_obj_add_cvBsp_p  add "BSP,P(..) P(.."  with points
AP_obj_add_func1    add [ANG|DIST|X|R|Y](<val>)
AP_obj_add_curPos   add cursor-position as "P(x y z)" to string
AP_obj_blank        add blank but not after '=' or '|' or blank

AP_obj_2_txt_query  get typ and DB-index of last created Textline
AP_obj_set_last     aux for AP_obj_add_last (set act_typ/act_ind)
AP_obj_add_last     add last created object to buffer

AP_obj_hdr_sur      create textHeader for A

AP_view_2_txt       give Textline "VIEW .."

List_functions_end:
=====================================================

see also:
APED_dbo_oid      Text -> (Typ, Ind)
APED_oid_dbo__      (Typ, Ind) -> Text
UTX_add_fl_u3       uva

\endcode *//*----------------------------------------


Compile:
cc -c xa_obj_txt.c



=========================================================
*/


#ifdef _MSC_VER
#include "MS_Def0.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



#include "../ut/ut_geo.h"         // OFF, ON ..
#include "../ut/ut_cast.h"             // INT_PTR

#include "../db/ut_DB.h"           // APT_LN_SIZ
#include "../xa/xa_obj_txt.h"      // AP_obj_blank



static int  act_typ;
static long act_ind;


static long su_ind;


/*
//==========================================================================
  int AP_pt_segpar (char *txOut, int typ, long ind, int iSeg, double lpar) {
//==========================================================================
// give Text for Point on geometr. Obj near Cursor
// zB gives "P(L21 MOD(1))"   if iSeg > 0
//       od "P(L21 lpar)"     if iSeg == 0

  char    cbuf[32];
  Point   pt1;


  printf("AP_tx_segpar %d %d %d %f\n",typ,ind,iSeg,lpar);

  APED_oid_dbo__ (cbuf, typ, ind);
  

    if(iSeg > 0) {
      if(lpar == 0.) {
        sprintf(txOut, "P(%s MOD(%d))",cbuf,iSeg);
      } else {
        sprintf(txOut, "P(%s MOD(%d) ",cbuf,iSeg);
        UTX_add_fl_u (txOut, lpar);
        strcat(txOut, ")");
      }

    } else if(iSeg == 0) {
      sprintf(txOut, "P(%s ",cbuf);
      UTX_add_fl_u (txOut, lpar);
      strcat(txOut, ")");

    } else {
      TX_Print("AP_tx_segpar E001");
      return -1;
    }

  printf("ex AP_tx_segpar |%s| %d %d\n",txOut,typ,ind);

  return 0;

}
*/

//=================================================================
  int AP_view_2_txt (char *txOut) {
//=================================================================
/// AP_view_2_txt       give Textline "VIEW .."


  int i1;
  double view[10];



  GL_View_get(view);

  strcpy(txOut, "VIEW");

  for(i1=0; i1<10; ++i1) {
    // AP_obj_add_val (txOut, view[i1]);
    strcat (txOut, " ");
    UTX_add_fl_u  (txOut, view[i1]);
  }


  // printf("ex AP_view_2_txt |%s|\n",txOut);

  return 0;

}


/*
//=================================================================
  int AP_typ_ind_2_txt (char *txOut, int typ, long ind) {
//=================================================================



  if(typ == Typ_PT) {
    sprintf(txOut, "P%d ",ind);

  } else if(typ == Typ_LN) {
    sprintf(txOut, "L%d ",ind);

  } else if(typ == Typ_CI) {
    sprintf(txOut, "C%d ",ind);

  } else {
    TX_Error("AP_typ_ind_2_txt E001_%d",typ);
    return -1;
  }





  printf(" ex AP_typ_ind_2_txt |%s| typ=%d ind=%ld\n",txOut,typ,ind);

  return 0;

}
*/

//=================================================================
  int AP_obj_set_last (char *ED_buf1) {
//=================================================================
/// set  act_typ/act_ind

// damit nachfolgender AP_obj_add_last funktioniert

  int  typ;
  long ind;

  APED_dbo_oid (&typ, &ind, ED_buf1);
  act_typ = typ;
  act_ind = ind;

  // printf(" ex AP_obj_set_last %d %d\n",act_typ,act_ind);

  return 0;

}


//=================================================================
  int AP_obj_add_last (char *ED_buf1, long bufLen) {
//=================================================================
/// add last created object to buffer 

  int  i1;
  long l1;
  char auxBuf[32];

  printf("AP_obj_add_last |%s|\n",ED_buf1);

  AP_obj_2_txt_query (&i1, &l1);  // get typ, ind

  APED_oid_dbo__ (auxBuf, i1,l1); // nam <-- typ, ind

  if((strlen(ED_buf1)+34) > bufLen) {
    TX_Error("AP_obj_add_last EOM");
    return -1;
  }

  if(ED_buf1[strlen(ED_buf1)-1] != '=') strcat(ED_buf1, " ");

  strcat(ED_buf1, auxBuf);          // add obj to parent

  return 0;
}


//=================================================================
  int AP_obj_add_val (char *ED_buf1, double val) {
//=================================================================
/// add double mit voller Genauigkeit (10 Nachkommastellen)

  // printf("AP_obj_add_pt %f %f %f\n",pt1->x,pt1->y,pt1->z);


  // if(ED_buf1[strlen(ED_buf1)-1] != '=') strcat (ED_buf1, " ");
  AP_obj_blank (ED_buf1);

  UTX_add_fl_10  (ED_buf1, val);
  // UTX_add_fl_f  (ED_buf1, val, 10);
  // UTX_del_foll0 (ED_buf1);

  return 0;

}


//=================================================================
  int AP_obj_add_int (char *ED_buf1, int ival) {
//=================================================================
/// add double mit voller Genauigkeit (10 Nachkommastellen)

  // printf("AP_obj_add_pt %f %f %f\n",pt1->x,pt1->y,pt1->z);


  // if(ED_buf1[strlen(ED_buf1)-1] != '=') strcat (ED_buf1, " ");
  AP_obj_blank (ED_buf1);

  // see UTX_add_i_u
  sprintf (&ED_buf1[strlen(ED_buf1)],"%d", ival);

  return 0;

}


//=================================================================
  int AP_obj_add_nval (char *ED_buf1, int iNr, double *va) {
//=================================================================
/// \code
/// AP_obj_add_nval     add iNr doubles separated with blanks to ED_buf1
///   10 digits after comma
/// \endcode

  int    i1, ie;

  // printf("AP_obj_add_pt %f %f %f\n",pt1->x,pt1->y,pt1->z);

  AP_obj_blank (ED_buf1);

  ie = iNr - 1;

  for(i1=0; i1<iNr; ++i1) {
    UTX_add_fl_10  (ED_buf1, va[i1]);
    // UTX_add_fl_f  (ED_buf1, va[i1], 10);
    // UTX_del_foll0 (ED_buf1);
    if(i1 < ie) strcat (ED_buf1, " ");
  }

  return 0;

}


//=================================================================
  int AP_obj_add_pt_sp (char *ED_buf1, Point *pt1) {
//=================================================================
/// add point mit reduced precision
// see also SRC_src_pt3_10 AP_obj_add_pt

  // printf("AP_obj_add_pt_sp \n");

  AP_obj_blank (ED_buf1);

  // sprintf(ED_buf1, "P(%f %f %f)", pt1->x,pt1->y,pt1->z);

  strcat(ED_buf1, "P(");                  // 2011-09-29
  UTX_add_fl_u (ED_buf1, pt1->x);

  strcat (ED_buf1, " ");
  UTX_add_fl_u (ED_buf1, pt1->y);

  strcat (ED_buf1, " ");

  UTX_add_fl_u (ED_buf1, pt1->z);
  strcat (ED_buf1, ")");



  return 0;

}


//=================================================================
  int AP_obj_add_pt (char *ED_buf1, Point *pt1) {
//=================================================================
/// \code
/// add struct Point* to string  " P(<x> <y> [<z>])"
/// leading blank, precision = 10 digits
/// see  AP_obj_add_pt_rp (reduced precision)
// see also SRC_src_pt3_10 AP_obj_add_pt_sp
/// \endcode

  // printf("AP_obj_add_pt %f %f %f\n",pt1->x,pt1->y,pt1->z);


  // if(ED_buf1[strlen(ED_buf1)-1] != '=') strcat (ED_buf1, " ");
  AP_obj_blank (ED_buf1);


      strcat (ED_buf1, "P(");
      UTX_add_fl_10  (ED_buf1, pt1->x);
      strcat (ED_buf1, " ");
      UTX_add_fl_10  (ED_buf1, pt1->y);


      if(!UTP_compdb0(pt1->z,UT_TOL_pt)) {
        strcat (ED_buf1, " ");
        UTX_add_fl_10  (ED_buf1, pt1->z);
      }
      strcat (ED_buf1, ")");


  return 0;

}


//=================================================================
  int AP_obj_add_pt2 (char *ED_buf1, Point2 *pt1) {
//=================================================================
/// \code
/// add struct Point2* to string  " P(<x> <y>)"
/// leading blank, precision = 10 digits
/// \endcode

  // printf("AP_obj_add_pt %f %f %f\n",pt1->x,pt1->y,pt1->z);


  // if(ED_buf1[strlen(ED_buf1)-1] != '=') strcat (ED_buf1, " ");
  AP_obj_blank (ED_buf1);


      strcat (ED_buf1, "P(");
      UTX_add_fl_10  (ED_buf1, pt1->x);
      strcat (ED_buf1, " ");
      UTX_add_fl_10  (ED_buf1, pt1->y);
      strcat (ED_buf1, ")");

  return 0;

}


//=================================================================
  int AP_obj_add_pt3 (char *ED_buf1, Point *pt1) {
//=================================================================
/// \code
/// add struct Point* to string  "<x> <y> [Z(<z>)]"
/// NO leading blank, precision = 10 digits
/// \endcode

  // printf("AP_obj_add_pt %f %f %f\n",pt1->x,pt1->y,pt1->z);


  UTX_add_fl_10  (ED_buf1, pt1->x);
  strcat (ED_buf1, " ");
  UTX_add_fl_10  (ED_buf1, pt1->y);

  if(!UTP_compdb0(pt1->z,UT_TOL_pt)) {
    strcat (ED_buf1, " Z(");
    UTX_add_fl_10  (ED_buf1, pt1->z);
    strcat (ED_buf1, ")");
  }


  return 0;

}



//=================================================================
  int AP_obj_add_ln (char *ED_buf1, int mode, Point *pt1, Point *pt2) {
//=================================================================
/// \code
/// add line to string;
/// mode = 0:  L(Pa Pe)
/// mode = 1:  L(Pe)
/// \endcode

  AP_obj_blank (ED_buf1);

  strcat(ED_buf1, "L(");

  if(mode == 0) {
    // AP_obj_add_pt3 (ED_buf1, pt1);
    AP_obj_add_pt (ED_buf1, pt1);
    strcat(ED_buf1, " ");
  }


  // AP_obj_add_pt3 (ED_buf1, pt2);
  AP_obj_add_pt (ED_buf1, pt2);


  strcat(ED_buf1, ")");


  return 0;

}


//=================================================================
  int AP_obj_add_ci_ (char *ED_buf1, Circ *ci1) {
//=================================================================
/// add Circle as  C(P(ptStart) P(ptEnd) P(ptCen) D(vz))


  AP_obj_blank (ED_buf1);

  strcat(ED_buf1, "C(ARC");

  AP_obj_add_pt (ED_buf1, &ci1->p1);
  AP_obj_add_pt (ED_buf1, &ci1->p2);
  AP_obj_add_pt (ED_buf1, &ci1->pc);
  AP_obj_add_vc (ED_buf1, &ci1->vz);

  if(ci1->rad < 0.) {
    strcat(ED_buf1, " CW");
  }

  strcat(ED_buf1, ")");

  // printf("ex AP_obj_add_ci_ |%s|\n",ED_buf1);

  return 0;

}


//=================================================================
  int AP_obj_add_ci2 (char *ED_buf1, int mode, Circ *ci1) {
//=================================================================
/// \code
/// DO NOT USE - OLD VERSION !
/// use AP_obj_add_ci_
/// mode = 0:  C(Pa Pe Pc Dreh)
/// mode = 1:  C(Pe Pc Dreh)
/// \endcode

  AP_obj_blank (ED_buf1);

  strcat(ED_buf1, "C(");

  if(mode == 0) {
    AP_obj_add_pt3 (ED_buf1, &ci1->p1);
    strcat(ED_buf1, " ");
  }

  AP_obj_add_pt3 (ED_buf1, &ci1->p2);

  strcat(ED_buf1, " ");

  AP_obj_add_pt3 (ED_buf1, &ci1->pc);


  // ist vz ungleich 0 0 1 ?
  if(UT3D_comp2vc_p(&ci1->vz, (Vector*)&UT3D_VECTOR_Z, UT_TOL_min1) == 0) {
    strcat(ED_buf1, " D(");
    UTX_add_fl_10  (ED_buf1, ci1->vz.dx);
    // UTX_add_fl_f  (ED_buf1, ci1->vz.dx, 10);
    // UTX_del_foll0 (ED_buf1);
    strcat (ED_buf1, " ");
    UTX_add_fl_10  (ED_buf1, ci1->vz.dy);
    // UTX_add_fl_f  (ED_buf1, ci1->vz.dy, 10);
    // UTX_del_foll0 (ED_buf1);
    strcat (ED_buf1, " ");
    UTX_add_fl_10  (ED_buf1, ci1->vz.dz);
    // UTX_add_fl_f  (ED_buf1, ci1->vz.dz, 10);
    // UTX_del_foll0 (ED_buf1);
    strcat(ED_buf1, ")");
  }




  if(ci1->rad < 0.) {
    strcat(ED_buf1, " CW");
  }

  strcat(ED_buf1, ")");

  // printf("ex AP_obj_add_ci |%s|\n",ED_buf1);

  // exit(0);

  return 0;

}


//========================================================================
  int AP_obj_add_ci1 (char *ED_buf1, Point *pc, double *rdc, Vector *vz) {
//========================================================================
/// \code
/// add 3D-Circ to string as "C(P(100 100 0) 32 DZ)"
/// Input: center, radius, Z-Vec
/// \endcode

  // printf("AP_obj_add_ci1 %f %f %f\n",pt1->x,pt1->y,pt1->z);


  AP_obj_blank (ED_buf1);

  strcat (ED_buf1, "C(");

  AP_obj_add_pt (ED_buf1, pc);

  AP_obj_add_val (ED_buf1, *rdc);

  AP_obj_add_vc (ED_buf1, vz);

  strcat (ED_buf1, ")");

  return 0;

}


//================================================================
  void AP_obj_blank (char *ED_buf1) {
//================================================================
/// do NOT add blank after = ( blank.

  char c1;

  if(ED_buf1[0] == '\0') return ;

  // get last char
  c1 = ED_buf1[strlen(ED_buf1)-1];

  if((c1 == '=')||(c1 == '(')||(c1 == ' ')) return;

  // printf(" AP_obj_blank |%s|\n",ED_buf1);

  strcat (ED_buf1, " ");

  return;

}
 

//=================================================================
  int AP_obj_add_vc (char *ED_buf1, Vector *vc1) {
//=================================================================
/// \code
/// add struct Vector* to string  " D(x y z)"
/// see also AP_obj_add_vc0
/// \endcode

  char cbuf[16];


  // printf("AP_obj_add_vc %f %f %f\n",vc1->dx,vc1->dy,vc1->dz);


  // if(ED_buf1[strlen(ED_buf1)-1] != '=') strcat (ED_buf1, " ");
  AP_obj_blank (ED_buf1);


  // check if Vector is a Defaultvektor (DX or DIX or ...)
  if(APED_oid_vc(cbuf, vc1) != 0) {
    strcat(ED_buf1, cbuf);
    return 0;
  }


  return AP_obj_add_vc0 (ED_buf1,vc1);

}


//=================================================================
  int AP_obj_add_vc0 (char *ED_buf1, Vector *vc1) {
//=================================================================
/// vec -> txt ("D(dx dy dz)"); no Standardvectors.


  // printf("AP_obj_add_vc0 %f %f %f\n",vc1->dx,vc1->dy,vc1->dz);


  // if(ED_buf1[strlen(ED_buf1)-1] != '=') strcat (ED_buf1, " ");
  AP_obj_blank (ED_buf1);


  strcat (ED_buf1, "D(");
  // UTX_add_fl_u2 (ED_buf1, vc1->dx, vc1->dy);
  UTX_add_fl_10  (ED_buf1, vc1->dx);
  // UTX_add_fl_f  (ED_buf1, vc1->dx, 10);
  // UTX_del_foll0 (ED_buf1);
  strcat (ED_buf1, " ");
  UTX_add_fl_10  (ED_buf1, vc1->dy);
  // UTX_add_fl_f  (ED_buf1, vc1->dy, 10);
  // UTX_del_foll0 (ED_buf1);


  if(!UTP_compdb0(vc1->dz,UT_TOL_min1)) {
    strcat (ED_buf1, " ");
    UTX_add_fl_10  (ED_buf1, vc1->dz);
    // UTX_add_fl_f  (ED_buf1, vc1->dz, 10);
    // UTX_del_foll0 (ED_buf1);
  }
  strcat (ED_buf1, ")");


  return 0;

}


//===========================================================================
  int AP_obj_add_cvBsp_p (char *so, int soSiz,
                          Point *pa, int ptNr, int deg, int ctrl) {
//===========================================================================
/// \code
/// AP_obj_add_cvBsp_p  add "BSP,P(..) P(.."  with points
/// Input:
///   soSiz    size of outputbuffer so
///   pa       points
///   ptNr     nr of points
///   deg      degree;   default = 2
///   ctrl     0=curve through points; 1=points are controlpoints
/// \endcode

  int    i1;
  char   *ss, *se;


  // printf("AP_obj_add_cvBsp_p %d %d %d\n", ptNr, deg, ctrl);

  ss = so;
  se = ss + soSiz - 48;

  // strcat(so,"BSP");
// 
  for(i1=0; i1<ptNr; ++i1) {
    AP_obj_add_pt (so, &pa[i1]);
    so += strlen(so);
    *so = ' ';
    ++so;
    *so = '\0';
    if(so > se) goto L_EOM;
  }


  if(deg != 2) {
    sprintf (so, "%d ", deg);
    so += strlen(so);
  }

  if(ctrl) strcat(so, "CTRL");

  // printf("ex AP_obj_add_cvBsp_p |%s|\n",ss);

  return 0;


  L_EOM:
    TX_Error ("AP_obj_add_cvBsp_p EOM");
    return -1;

}


//====================================================================
  int AP_obj_add_cvBsp_b (char *ED_buf1, long bufLen, CurvBSpl *cv1) {
//====================================================================
/// AP_obj_add_cvBsp_b  add "BSP,...."  with binary data block as ascii


  int      i1, i2;
  Point    *pTab;


    strcat(ED_buf1,"BSP,");


      UTX_add_i_u (ED_buf1, cv1->ptNr);
      strcat(ED_buf1,",");
      UTX_add_i_u (ED_buf1, cv1->deg);
      // printf("Typ_CVBSP: %s\n",ED_buf1);


      // Kontrollpunkte raus
      pTab = cv1->cpTab;
      for(i1=0; i1<cv1->ptNr; ++i1) {

        if((strlen(ED_buf1)+100) > bufLen) {
          TX_Error("CVBSP:textbuffer-overflow-error ");
          return -1;
        }

        strcat(ED_buf1,",");
        UTX_add_fl_u3 (ED_buf1, pTab[i1].x,pTab[i1].y,pTab[i1].z, ',');
      }

      // Knotvec raus
      i2 = cv1->ptNr + cv1->deg + 1;
      for(i1=0; i1<i2; ++i1) {
        if((strlen(ED_buf1)+20) > bufLen) {
          TX_Error("CVBSP:textbuffer-overflow-error ");
          return -1;
        }
        strcat(ED_buf1,",");
        UTX_add_fl_u (ED_buf1, cv1->kvTab[i1]);
      }

      // Start/Endparameter raus (v0,v1)
      if((strlen(ED_buf1)+30) > bufLen) {
        TX_Error("CVBSP:textbuffer-overflow-error ");
        return -1;
      }
      strcat(ED_buf1,",");
      UTX_add_fl_u2 (ED_buf1, cv1->v0, cv1->v1);


  return 1;

}


//=================================================================
  int AP_obj_add_func1 (char *ED_buf1, int mode, double ang1) {
//=================================================================
/// mode: A D X R Y

  // printf("AP_obj_add_vc %f %f %f\n",vc1->dx,vc1->dy,vc1->dz);


  // if(ED_buf1[strlen(ED_buf1)-1] != '=') strcat (ED_buf1, " ");
  AP_obj_blank (ED_buf1);


  if(mode == 'A') {
    strcat (ED_buf1, "ANG(");

  } else if(mode == 'D') {
    strcat (ED_buf1, "DIST(");

  } else if(mode == 'X') {
    strcat (ED_buf1, "X(");

  } else if(mode == 'R') {
    strcat (ED_buf1, "RAD(");

  } else if(mode == 'Y') {
    strcat (ED_buf1, "Y(");

  } else {
    TX_Error("AP_obj_add_func1 E001_%d",mode);
    return -1;
  }


  UTX_add_fl_10  (ED_buf1, ang1);
  // UTX_add_fl_f  (ED_buf1, ang1, 10);
  // UTX_del_foll0 (ED_buf1);
  strcat (ED_buf1, ")");


  return 0;

}


//================================================================
  int AP_obj_add_curPos (char *ED_buf1) {
//================================================================
/// add cursor-position as "P(x y z)" to string

  char *ps;

  if(strlen(ED_buf1) > 0) strcat (ED_buf1, " ");

  UI_GR_get_actPos_ (&ps);

  strcat(ED_buf1, ps);

    printf("ex AP_obj_add_curPos |%s|\n",ED_buf1);


  return 0;

}

 
//=================================================================
  int AP_obj_add_dbo (char *ED_buf1, int typ, long ind) {
//=================================================================
/// \code
/// add DB-obj to string; " L22"
/// Input obj-typ und DB-index
/// \endcode


  char     cBuf[64];

  // printf("AP_obj_add_dbo %d %d |%s|\n",typ,ind,ED_buf1);


  // if(ED_buf1[strlen(ED_buf1)-1] != '=')
  if(strlen(ED_buf1) > 0) strcat (ED_buf1, " ");

  APED_oid_dbo__ (cBuf, typ, ind);
  strcat (ED_buf1, cBuf);

    // printf("  ex AP_obj_add_dbo %d %d |%s|\n",typ,ind,ED_buf1);

  return 0;

}


//=================================================================
  int AP_obj_2_txt_query (int *typ, long *ind) {
//=================================================================
/// AP_obj_2_txt_query  get typ and DB-index of last created Textline

  *typ = act_typ;
  *ind = act_ind;

  // printf("ex AP_obj_2_txt_query %d %d\n",act_typ,act_ind);

  return 0;
}



//=================================================================
  int AP_obj_hdr_sur (char *ED_buf1, long bufLen, ObjGX *o1, long ind) {
//=================================================================
/// returns next free index for surface and "A<index>" in ED_buf1
/// o1 unused

  int  irc;


  // hole naechsten freien Index
  if(ind < 0) {
    ++su_ind;
    ind = su_ind;
  }
  irc = ind;
  // printf(" create A%d \n",ind);

  sprintf(ED_buf1,"A%ld=",ind);

  printf(" ex AP_obj_hdr_sur |%s|\n",ED_buf1);

  return irc;

}


//====================================================================
  int AP_obj_2_txt (char *ED_buf1, long bufLen, ObjGX *o1, long ind) {
//====================================================================
/// \code
/// change obj to text and save it with UTF_add1_line
/// Input:
///   ind          DB-index; for -1L a new (free) index is used.
///   o1
/// Output:
///   ED_buf1
/// retCod    0    OK
///          -1    out of mem
/// Example see UTF_insert1
/// get get db-typ, db-ind with AP_obj_2_txt_query () (after AP_obj_2_txt)
/// see UTRA_app_oTab AP_stru_2_txt
/// \endcode

  int    irc, i1, iSiz, iTyp, iForm, iLen;
  long   l1, cLen1, cLen2;
  char   *pi, *cPos1, *cPos2, auxBuf[32];


  // Init:
  if(ED_buf1 == NULL) {AP_stru_2_txt (NULL, 0L, NULL, 0L); return 0;}



  // DEB_dump_ox_0 (o1, "\n\n\nAP_obj_2_txt");
  // printf("AP_obj_2_txt-typ=%d form=%d siz=%d ind=%ld\n",((ObjGX*)o1)->typ,
            // ((ObjGX*)o1)->form,((ObjGX*)o1)->siz,ind);



  iTyp  = o1->typ;
  iForm = o1->form;
    // printf(" iTyp=%d iForm=%d\n",iTyp,iForm);


  // wenn obj eine einfache struct ist: direkt ausgeben.
  if((iForm != Typ_ObjGX) ||
     (iTyp == Typ_SURCIR)) {    // IndexTable geht noch nicht !
    // oldSiz = *oSiz;
    irc = AP_stru_2_txt (ED_buf1, bufLen, o1, ind);
      // printf(" _2_txt %d AP_stru_2_txt-typ=%d form=%d siz=%d ind=%ld |%s|\n",
               // irc,o1->typ,o1->form,o1->siz,ind,ED_buf1);
    if(irc < 0) return -1;
    irc = UTF_add1_line (ED_buf1); // add Lines -> Aux.Buffer
    return irc;
  }


  //________ form ist ObjGX_________________________
  // ist ein echtes object;
  iSiz  = o1->siz;
  pi    = o1->data;
   // printf(" iTyp=%d iForm=%d iSiz=%d\n",iForm,iTyp,iSiz);

  cPos1 = ED_buf1;
  cLen1 = bufLen;


  // SURRU ex Iges geht so nicht immer (hat Indices bei den Subelementen)
  if(iTyp == Typ_SURRU) {
    irc = AP_stru_2_txt(ED_buf1, bufLen, o1, ind);
      // printf(" ex AP_obj_2_txt %d |%s|\n",irc,ED_buf1);
    if(irc < 0) return -1;
    AP_obj_set_last (ED_buf1);
    return UTF_add1_line (ED_buf1); // add Lines -> Aux.Buffer
  }


  i1 = AP_typDB_typ (iTyp);
  if(iTyp == Typ_CVTRM) {  // CCV: den Header "S#=CCV" holen
    irc = AP_stru_2_txt(cPos1, bufLen, o1, ind);  // "S%d=CCV"
    if(irc < 0) return -1;
      // printf("    ex stru_2_txt-CCV:\n%s\n",cPos1);

  } else if(i1 == Typ_SUR) {  // SUR  den Header "A#=" holen
    AP_obj_hdr_sur (cPos1, bufLen, o1, ind);


  } else goto L_E001;


  cPos2 = cPos1 + strlen(cPos1) + 1;
  cLen2 = cLen1 - strlen(cPos1) - 1;


  // loop tru obj ...
  for(i1=0; i1<iSiz; ++i1) {
    iTyp  = ((ObjGX*)pi)->typ;
    iForm = ((ObjGX*)pi)->form;
      // printf(" objI[%d] iTyp=%d iForm=%d\n",i1,iTyp,iForm);
    

    if(iTyp == Typ_Typ) {     // skip


    } else if(iForm == Typ_ObjGX) { // recurse
      // CCV
      AP_obj_2_txt (cPos2, cLen2, (ObjGX*)pi, ind);  // RECURSE !!!

      // add obj to parent ...
      AP_obj_add_last (cPos1, cLen1);
        // printf("    parent:\n%s\n",cPos1);
      cPos2 = cPos1 + strlen(cPos1) + 1;
      cLen2 = cLen1 - strlen(cPos1) - 1;


    } else {       // normale struct: convert --> text
      irc = AP_stru_2_txt (cPos2, cLen2, pi, ind);
      if(irc < 0) return -1;
        // printf("    ex stru_2_txt:\n%s\n",cPos2);
      irc = UTF_add1_line (cPos2); // add Lines -> Aux.Buffer
      if(irc < 0) return irc;
      *cPos2 = '\0';

      // add obj to parent ...
      AP_obj_add_last (cPos1, cLen1);
        // printf("    parent:\n%s\n",cPos1);
      cPos2 = cPos1 + strlen(cPos1) + 1;
      cLen2 = cLen1 - strlen(cPos1) - 1;
    }


    L_nxt:
    pi += sizeof(ObjGX);

  }

  // vor generieren des obj in cPos1; damit nachfolgender AP_obj_add_last
  // funktioniert: act_typ/act_ind setzen
  AP_obj_set_last (cPos1);


  // printf("out in AP_obj_2_txt: iTyp=%d iForm=%d iSiz=%d\n",iForm,iTyp,iSiz);
  // printf(" ,cPos1:|%s|\n",cPos1);

  return UTF_add1_line (cPos1); // add Lines -> Aux.Buffer


  L_E001:
    TX_Error("AP_obj_2_txt E001 %d",iTyp);
    return -1;
}


//=====================================================================
  int AP_stru_2_txt (char *ED_buf1, long bufLen, ObjGX *o1, long ind) {
//=====================================================================
/// \code
/// convert object-struct -> source-text (definition-line)
/// ED_buf1=NULL,ind=0: Init DB-indices
/// ED_buf1=NULL,ind=1: query DB-indices (o1=int[7])
/// Die Objektindexe muessen initialisiert werden; init mit o1->typ=TYP_FuncInit
/// In und Out: pt_ind wird um 1 incrementiert !
/// Input:
///   o1
///   ind      DB-index; for -1L a new (free) index is used.
///   bufLen   max siz of ED_buf1
/// Output:
///   ED_buf1  source-obj; init function if NULL.
///   RC = -1: Fehler am Objekt, weiter.
///   RC = -2: Ueberlauf; sofort abbrechen.
///
/// see also AP_obj_2_txt APED_search_dbLimits
/// \endcode

  // static int    AP_ind_p=20, AP_ind_c=20, AP_ind_l=20, AP_ind_s=20,
  //               AP_ind_d=20, AP_ind_n=20, AP_ind_r=20, AP_ind_m=20;

  static long   vc_ind, pt_ind, ln_ind, ci_ind, cv_ind, bd_ind, tx_ind,
                pl_ind, mr_ind;


  int      typ, form, irc, ciMode, i1, i2, iNr, *ia;
  long     *lTab, l1;
  double   d1, *da;
  char     cbuf[240], *cp1;


  Point2    *pt21, *p2Tab, p20;
  Point     *pt31, *pTab, pt1;
  Line2     *ln21;
  Line      *ln1;
  Circ2     *ci21;
  Circ      *ci1;
  CurvElli  *ell1;
  Plane     *pl1;
  GText     *tx1;
  Dimen     *dim1;
  polynom_d3 *polTab;
  CurvBSpl  *cpbsp;
  CurvRBSpl *prbsp;
  SurBSpl   *subsp;
  SurRBSpl  *srbsp;
  SurRev    *srv;
  Conus     *bco;
  ModelRef  *mdr;
  ModelBas  *mdb;
  ObjGX     *oTab, *ox1;



  // if(o1) {
  // printf("AP_stru_2_txt len=%ld ind=%ld typ=%d form=%d\n",bufLen,ind,
         // o1->typ,o1->form);
  // if(o1->typ == Typ_Done) {
    // printf(" typ==done\n");
  // }
  // }
  // DEB_dump_obj__ (Typ_ObjGX, o1, "AP_stru_2_txt");



  //=====================================================================
  if(ED_buf1 == NULL) {

    if(ind == 0) {                         // init DB-indices
      vc_ind = DB_dbo_get_free (Typ_VC);    // D
      pt_ind = DB_dbo_get_free (Typ_PT);    // P
      ln_ind = DB_dbo_get_free (Typ_LN);    // L
      ci_ind = DB_dbo_get_free (Typ_CI);    // C
      cv_ind = DB_dbo_get_free (Typ_CV);    // S
      pl_ind = DB_dbo_get_free (Typ_PLN);   // R
      su_ind = DB_dbo_get_free (Typ_SUR);   // A
      bd_ind = DB_dbo_get_free (Typ_SOL);   // B
      tx_ind = DB_dbo_get_free (Typ_GTXT);  // N
      mr_ind = DB_dbo_get_free (Typ_Model); // M

        // printf(" cv_ind=%d\n",cv_ind);
        // printf(" su_ind=%d\n",su_ind);
        // printf(" bd_ind=%d\n",bd_ind);
        // printf(" tx_ind=%d\n",tx_ind);
        // printf(" ci_ind=%d\n",ci_ind);
        // printf(" ln_ind=%d\n",ln_ind);
        // printf(" pt_ind=%d\n",pt_ind);
        // printf(" mr_ind=%d\n",mr_ind);


    } else if(ind == 1) {                // query DB-indices
      lTab = (void*)o1;
      lTab[0] = cv_ind;
      lTab[1] = su_ind;
      lTab[2] = bd_ind;
      lTab[3] = tx_ind;
      lTab[4] = ci_ind;
      lTab[5] = ln_ind;
      lTab[6] = pt_ind;
      lTab[7] = mr_ind;

    }

    return 0;
  }
    


  //=====================================================================
  typ  = o1->typ;
  form = o1->form;
  irc  = 0;



  // printf("AP_stru_2_txt typ=%d form=%d siz= %d ind=%d bufLen=%d\n",typ,form,
                // o1->siz,ind,bufLen);
  // DEB_dump_ox_0 (o1, "AP_stru_2_txt");



  //=====================================================================
  if(typ == Typ_VC) {

    // hole naechsten freien PT-Index
    if(ind < 0) {
      ++vc_ind;
      ind = vc_ind;
    }

    irc = ind;

    sprintf(ED_buf1,"D%ld=",ind);

    if(form == Typ_Txt) {
      goto L_add_text;


    } else if(form == Typ_VC) {   // add " D(x y z)"
      AP_obj_add_vc (ED_buf1, o1->data);
      goto L_fertig;


    } else goto L_unknown_form;




  //=====================================================================
  } else if(typ == Typ_PT) {


    // hole naechsten freien PT-Index
    if(ind < 0) {
      ++pt_ind;
      ind = pt_ind;
    }


    // if(ind >= APT_PT_SIZ) {
      // TX_Error("max. Anzahl Punkte erreicht");
      // goto L_err_2;
    // }
    irc = ind;

    sprintf(ED_buf1,"P%ld=",ind);

    if(form == Typ_Txt) goto L_add_text;



    //-----------------------------------------
    if(form == Typ_PT2) {
      pt21 = o1->data;
      AP_obj_add_pt2 (ED_buf1, pt21);



    //-----------------------------------------
    } else if(form == Typ_PT) {

      pt31 = o1->data;
      AP_obj_add_pt (ED_buf1, pt31);


    //-----------------------------------------
    } else {
        printf(" _stru_2_txt unk PT %d %d\n",typ,form);
      goto L_unknown_form;
    }





  //=====================================================================
  } else if (typ == Typ_LN) {


    // hole naechsten freien LN-Index
    if(ind < 0) {
      ++ln_ind;
      ind = ln_ind;
    }
    // if(ind >= APT_LN_SIZ) {
      // TX_Error("max. Anzahl Linien erreicht");
      // goto L_err_2;
    // }
    irc = ind;

    sprintf(ED_buf1,"L%ld=",ind);

    //-----------------------------------------
    if(form == Typ_Txt) {
      goto L_add_text;


    //-----------------------------------------
    } else if (form == Typ_LN2) {

      // zu kurze Lines (koennen durch Linien nur in Z-Richtung
      // entstehen) eliminieren
      ln21 = (Line2*)o1->data;
        // printf(" p1=%f %f\n",ln21->p1.x,ln21->p1.y);
        // printf(" p2=%f %f\n",ln21->p2.x,ln21->p2.y);


      d1 = UT2D_len_2pt (&ln21->p1, &ln21->p2);
      // if(d1 < UT_TOL_ln) {
      if(d1 < UT_TOL_pt) {
        AP_errText_set ("line too short");
        // TX_Print("*** Linie zu kurz %f,%f",ln21->p1.x,ln21->p1.y);
        // TX_Print("              bis %f,%f",ln21->p2.x,ln21->p2.y);
        // printf("  dist=%f UT_TOL_ln=%f\n",d1,UT_TOL_ln);
        goto L_err_1;
      }

      AP_obj_add_pt2 (ED_buf1, &ln21->p1);
      AP_obj_add_pt2 (ED_buf1, &ln21->p2);

        // printf(" ln2 irc=%d\n",irc);



    //-----------------------------------------
    } else if (form == Typ_LN) {

      // zu kurze Lines (koennen durch Linien nur in Z-Richtung
      // entstehen) eliminieren
      ln1 = o1->data;

      d1 = UT3D_len_2pt (&ln1->p1, &ln1->p2);
      // if(d1 < UT_TOL_ln) {
      if(d1 < UT_TOL_pt) {
        AP_errText_set ("line too short");
        // TX_Print("*** Linie zu kurz %f,%f,%f",ln1->p1.x,ln1->p1.y,ln1->p1.z);
        // TX_Print("              bis %f,%f,%f",ln1->p2.x,ln1->p2.y,ln1->p2.z);
        // printf("  dist=%f UT_TOL_ln=%f\n",d1,UT_TOL_ln);
        goto L_err_1;
      }

      AP_obj_add_pt (ED_buf1, &ln1->p1);
      AP_obj_add_pt (ED_buf1, &ln1->p2);


    //-----------------------------------------
    } else if (form == Typ_Index) {
/*
      ia = o1->data;
        // printf(" ip1=%d ip2=%d\n",ia[0],ia[1]);
      AP_obj_add_dbo (ED_buf1, Typ_PT, ia[0]);
      AP_obj_add_dbo (ED_buf1, Typ_PT, ia[1]);
*/
     AP_obj_add_dbo (ED_buf1, Typ_LN, LONG_PTR(o1->data)); // 2017-04-28



    //-----------------------------------------
    } else {
        printf(" _stru_2_txt unk LN %d %d\n",typ,form);
      goto L_unknown_form;
    }




  //=====================================================================
  } else if(typ == Typ_CI) {

    // hole naechsten freien Index
    if(ind < 0) {
      ++ci_ind;
      ind = ci_ind;
    }

    // if(ind >= APT_CI_SIZ) {
      // TX_Error("max. Anzahl Kreise erreicht");
      // goto L_err_2;
    // }
    irc = ind;

    sprintf(ED_buf1,"C%ld=",ind);

    if(form == Typ_Txt) goto L_add_text;




    //-----------------------------------------
    if (form == Typ_CI2) {


      ci21 = o1->data;


      //-------- Startpoint ---------
      if(!UT2D_comp2pt(&ci21->p1, &ci21->p2, UT_TOL_pt)) {

        ciMode = 'A';
        strcat (ED_buf1, "ARC");
        AP_obj_add_pt2 (ED_buf1, &ci21->p1);

        //-------- Endpoint ---------
        AP_obj_add_pt2 (ED_buf1, &ci21->p2);
      } else {
        ciMode = 'C';
      }

      //-------- Centerpoint ---------
      AP_obj_add_pt2 (ED_buf1, &ci21->pc);


      // Radius
      if(ciMode == 'C') {
        AP_obj_add_val (ED_buf1, fabs(ci21->rad));
      }


      if(ci21->rad < 0.) {
        strcat (ED_buf1, " CW");
      }







    //-----------------------------------------
    } else if (form == Typ_CI) {

      ci1 = o1->data;


      //-------- Startpoint ---------
      // Vollkreis oder Arc
      if(!UT3D_comp2pt(&ci1->p1, &ci1->p2, UT_TOL_min1)) {
        ciMode = 'A';
        strcat (ED_buf1, "ARC");
        AP_obj_add_pt (ED_buf1, &ci1->p1);
  
        //-------- Endpoint ---------
        AP_obj_add_pt (ED_buf1, &ci1->p2);
      } else {
        ciMode = 'C';
      }



      //-------- Centerpoint ---------
      AP_obj_add_pt (ED_buf1, &ci1->pc);

      // Radius
      if(ciMode == 'C') {
        AP_obj_add_val (ED_buf1, fabs(ci1->rad));
      }




      //-------- Z-Vector ---------
      // if(!((UTP_comp2db(ci1->vz.dx,0.,UT_TOL_pt))   &&
           // (UTP_comp2db(ci1->vz.dy,0.,UT_TOL_pt))   &&
           // (UTP_comp2db(ci1->vz.dz,1.,UT_TOL_pt))))       {
        AP_obj_add_vc (ED_buf1, &ci1->vz);
      // }


/*
      if(ci1->rad < 0.) {
        strcat (ED_buf1, " CW");
      }
*/


    //-----------------------------------------
    } else {
        printf(" _stru_2_txt unk AC %d %d\n",typ,form);
      goto L_unknown_form;
    }




  //=====================================================================
  } else if (typ == Typ_CV) {


    // hole naechsten freien Index
    if(ind < 0) {
      ++cv_ind;
      ind = cv_ind;
    }

    irc = ind;

    sprintf(ED_buf1,"S%ld=",ind);
      // printf(" out Typ_CVELL: |%s|\n",ED_buf1);

    if(form == Typ_Txt) {
      goto L_add_text;


    } else goto L_unknown_form;




  //=====================================================================
  } else if (typ == Typ_CVELL) {


    // hole naechsten freien Index
    if(ind < 0) {
      ++cv_ind;
      ind = cv_ind;
    }
/*
    if(ind >= APT_CV_SIZ) {
      TX_Error("max. Anzahl Curves erreicht");
      goto L_err_2;
    }
*/
    irc = ind;

    sprintf(ED_buf1,"S%ld=",ind);
      // printf(" out Typ_CVELL: |%s|\n",ED_buf1);

    if(form == Typ_Txt) goto L_add_text;

    strcat (ED_buf1, "ELL");

    ell1 = o1->data;


    AP_obj_add_pt (ED_buf1, &ell1->pc);
    AP_obj_add_vc0 (ED_buf1, &ell1->va); // NICHT normieren !
    AP_obj_add_vc0 (ED_buf1, &ell1->vb); // NICHT normieren !
    AP_obj_add_pt (ED_buf1, &ell1->p1);
    AP_obj_add_pt (ED_buf1, &ell1->p2);









  //=====================================================================
  } else if (typ == Typ_GTXT) {           // Note ..

    // hole naechsten freien Index
    if(ind < 0) {
      ++tx_ind;
      ind = tx_ind;
    }
/*
    if(ind >= APT_TX_SIZ) {
      TX_Error("max. Anzahl Textnotes erreicht");
      goto L_err_2;
    }
*/
    irc = ind;

    sprintf(ED_buf1,"N%ld=",ind);

    if(form == Typ_Txt) goto L_add_text;


      tx1 = (GText*)o1->data;
      pt31 = &tx1->pt;
      // printf(" Text %d |%s|\n",ind,(char*)tx1->txt);

      AP_obj_add_pt (ED_buf1, pt31);


      // Texthoehe
      strcat(ED_buf1,",");
      UTX_add_fl_u  (ED_buf1, tx1->size);

      // Richtung
      if(fabs(tx1->dir) > 0.1) {
        strcat(ED_buf1,",ANG(");
        UTX_add_fl_u  (ED_buf1, tx1->dir);
        strcat(ED_buf1,")");
      }

      // Text
      strcat(ED_buf1,",\"");
      if(tx1->txt) strcat(ED_buf1,tx1->txt);
      strcat(ED_buf1,"\"");




  //=====================================================================
  } else if (typ == Typ_Dimen) {

    // hole naechsten freien Index
    if(ind < 0) { ++tx_ind; ind = tx_ind; }

    irc = ind;
    sprintf(ED_buf1,"N%ld=",ind);
      // printf(" out Typ_Dimen: |%s|\n",ED_buf1);

    if(form == Typ_Txt) goto L_add_text;

    dim1 = (Dimen*)o1->data;

    if(dim1->dtyp == 0) {
      strcat(ED_buf1, "DIM");
      AP_obj_add_pt2 (ED_buf1, &dim1->p1);
      AP_obj_add_pt2 (ED_buf1, &dim1->p2);
      AP_obj_add_pt2 (ED_buf1, &dim1->p3);
      AP_obj_add_func1 (ED_buf1, 'A', dim1->a1);
      if((dim1->hd != 12)||(dim1->ld != 11)) {
        strcat(ED_buf1," ");
        UTX_add_i_u (ED_buf1, dim1->hd);
        strcat(ED_buf1," ");
        UTX_add_i_u (ED_buf1, dim1->ld);
      }


    } else if(dim1->dtyp == 3) {
      strcat(ED_buf1, "DIMA");
      AP_obj_add_pt2 (ED_buf1, &dim1->p1);
      AP_obj_add_func1 (ED_buf1, 'A', dim1->a1);
      AP_obj_add_pt2 (ED_buf1, &dim1->p2);
      AP_obj_add_func1 (ED_buf1, 'A', dim1->a2);
      AP_obj_add_pt2 (ED_buf1, &dim1->p3);
      if((dim1->hd != 12)||(dim1->ld != 11)) {
        strcat(ED_buf1," ");
        UTX_add_i_u (ED_buf1, dim1->hd);
        strcat(ED_buf1," ");
        UTX_add_i_u (ED_buf1, dim1->ld);
      }

    } else if(dim1->dtyp == 1) {
      strcat(ED_buf1, "DIMD C(");
      UT2D_pt_mid2pt (&p20, &dim1->p1, &dim1->p2);
      AP_obj_add_pt2 (ED_buf1, &p20);
      AP_obj_add_val (ED_buf1, dim1->a2);  // rad
      strcat(ED_buf1,")");
      AP_obj_add_pt2 (ED_buf1, &dim1->p3);
      if(dim1->hd != 12) {
        strcat(ED_buf1," ");
        UTX_add_i_u (ED_buf1, dim1->hd);
      }


    } else if(dim1->dtyp == 2) {
      strcat(ED_buf1, "DIMR C(");
      AP_obj_add_pt2 (ED_buf1, &dim1->p1);
      AP_obj_add_val (ED_buf1, dim1->a2);  // rad
      strcat(ED_buf1,")");
      AP_obj_add_pt2 (ED_buf1, &dim1->p3);
      strcat(ED_buf1," ");
      if(dim1->hd != 1) {
        strcat(ED_buf1," ");
        UTX_add_i_u (ED_buf1, dim1->hd);
      }


    } else if(dim1->dtyp == 21) {
      strcat(ED_buf1, "LDR");
      AP_obj_add_pt2 (ED_buf1, &dim1->p1);
      AP_obj_add_pt2 (ED_buf1, &dim1->p2);
      if(dim1->p3.x != UT_DB_LEER) AP_obj_add_pt2 (ED_buf1, &dim1->p3);
      if(dim1->hd != 1) {
        strcat(ED_buf1," ");
        UTX_add_i_u (ED_buf1, dim1->hd);
      }



    } else {
        TX_Print("*** Dimen: unknown typ %d",dim1->dtyp);
        goto L_err_1;
    }

    if(dim1->txt) {
      if(strlen(dim1->txt) > 0) {
        strcat(ED_buf1, " \"");
        strcat(ED_buf1, dim1->txt);
        strcat(ED_buf1, "\"");
      }
    }




  //=====================================================================
  } else if (typ == Typ_CVPOL2) {

    // hole naechsten freien Index
    if(ind < 0) {
      ++cv_ind;
      ind = cv_ind;
    }
/*
    if(ind >= APT_CV_SIZ) {
      TX_Error("max. Anzahl Curves erreicht");
      goto L_err_2;
    }
*/
    irc = ind;

    sprintf(ED_buf1,"S%ld=POL2",ind);
      // printf(" out Typ_CVPOL2: |%s|\n",ED_buf1);

    if(form == Typ_Txt) goto L_add_text;

      p2Tab = o1->data;
      iNr  = o1->siz;
      // printf("  CVPOL: ptNr=%d\n",iNr);



      for(i1=0; i1<iNr; ++i1) {

        if((strlen(ED_buf1)+100) > bufLen) {
          TX_Error("POL2:textbuffer-overflow-error ");
          goto L_err_2;
        }

        pt21 = &p2Tab[i1];

        AP_obj_add_pt2 (ED_buf1, pt21);

      }








  //=====================================================================
  } else if (typ == Typ_CVPOL) {

    // hole naechsten freien Index
    if(ind < 0) {
      ++cv_ind;
      ind = cv_ind;
    }
/*
    if(ind >= APT_CV_SIZ) {
      TX_Error("max. Anzahl Curves erreicht");
      goto L_err_2;
    }
*/
    irc = ind;

    sprintf(ED_buf1,"S%ld=POL ",ind);
      // printf(" out Typ_CVPOL: |%s|\n",ED_buf1);

    if(form == Typ_Txt) goto L_add_text;


    // form kann sein Typ_PT; Anzahl dann in Obj.siz
    if(form == Typ_PT) {
      pTab = o1->data;
      iNr  = o1->siz;

    // oder ein echtes Polygon (Typ_CVPOL)
    } else if(form == Typ_CVPOL) {
      pTab = ((CurvPoly*)o1->data)->cpTab;
      iNr  = ((CurvPoly*)o1->data)->ptNr;


    } else goto L_unknown_form;
      // printf("  CVPOL: ptNr=%d\n",iNr);


      for(i1=0; i1<iNr; ++i1) {

        if((strlen(ED_buf1)+100) > bufLen) {
          TX_Error("POL:textbuffer-overflow-error ");
            printf(" error; done %d of %d\n",i1,iNr);
          goto L_err_2;
        }

        pt31 = &pTab[i1];

        AP_obj_add_pt (ED_buf1, pt31);

      }









  //=====================================================================
  } else if (typ == Typ_CVPSP3) {

    // hole naechsten freien Index
    if(ind < 0) {
      ++cv_ind;
      ind = cv_ind;
    }
/*
    if(ind >= APT_CV_SIZ) {
      TX_Error("max. Anzahl Curves erreicht");
      goto L_err_2;
    }
*/
    irc = ind;

    sprintf(ED_buf1,"S%ld=PSP3",ind);
      // printf(" out Typ_CVPSP3: |%s|\n",ED_buf1);

    if(form == Typ_Txt) goto L_add_text;

      polTab = o1->data;  // data = memspc55 !!


      for(i1=0; i1<o1->siz; ++i1) {
        // printf(" seg %d u=%f %f  %f  %f\n",i1,polTab[i1].u,
               // polTab[i1].x.a,polTab[i1].y.a,polTab[i1].z.a);


        if((strlen(ED_buf1)+100) > bufLen) {
          TX_Error("CVPSP3:textbuffer-overflow-error ");
          goto L_err_2;
        }

        strcat(ED_buf1,",");
        UTX_add_fl_u  (ED_buf1, polTab[i1].u);
        strcat(ED_buf1,",");

        UTX_add_fl_u3 (ED_buf1,
          polTab[i1].x.a,polTab[i1].y.a,polTab[i1].z.a, ',');
        strcat(ED_buf1,",");

        UTX_add_fl_u3 (ED_buf1,
          polTab[i1].x.b,polTab[i1].y.b,polTab[i1].z.b, ',');
        strcat(ED_buf1,",");

        UTX_add_fl_u3 (ED_buf1,
          polTab[i1].x.c,polTab[i1].y.c,polTab[i1].z.c, ',');
        strcat(ED_buf1,",");

        UTX_add_fl_u3 (ED_buf1,
          polTab[i1].x.d,polTab[i1].y.d,polTab[i1].z.d, ',');
      }






  //=====================================================================
  } else if (typ == Typ_CVBSP) {

    // hole naechsten freien Index
    if(ind < 0) {
      ++cv_ind;
      ind = cv_ind;
    }
/*
    if(ind >= APT_CV_SIZ) {
      TX_Error("max. Anzahl Curves erreicht");
      goto L_err_2;
    }
*/
    irc = ind;

    sprintf(ED_buf1,"S%ld=BSP ",ind);
      // printf(" out Typ_CVBSP: |%s|\n",ED_buf1);


    if(form == Typ_Txt) {
      goto L_add_text;


    } else if(form == Typ_PT) {
      iNr  = o1->siz;
      pTab = o1->data;
        // printf(" BSP from PT nr=%d\n",iNr);
      for(i1=0; i1<iNr; ++i1) {    // see also Typ_CVPOL
        if((strlen(ED_buf1)+100) > bufLen) {
          TX_Error("BSP:textbuffer-overflow-error ");
            printf(" error; done %d of %d\n",i1,iNr);
          goto L_err_2;
        }
        pt31 = &pTab[i1];
        AP_obj_add_pt (ED_buf1, pt31);
      }
    

    } else if(form == Typ_CVBSP) {

      cpbsp = o1->data;  // data = memspc55 !!


      UTX_add_i_u (ED_buf1, cpbsp->ptNr);
      strcat(ED_buf1,",");
      UTX_add_i_u (ED_buf1, cpbsp->deg);
      // printf("Typ_CVBSP: %s\n",ED_buf1);


      // Kontrollpunkte raus
      pTab = cpbsp->cpTab;
      if((strlen(ED_buf1)+(25*cpbsp->ptNr)) > bufLen) {
        TX_Error("CVBSP:textbuffer-overflow-error ");
        goto L_err_2;
      }
      for(i1=0; i1<cpbsp->ptNr; ++i1) {
        strcat(ED_buf1,",");
        // UTX_add_fl_u3 (ED_buf1, pTab[i1].x,pTab[i1].y,pTab[i1].z, ',');
        UTX_add_fl_10 (ED_buf1, pTab[i1].x);
        strcat(ED_buf1,",");
        UTX_add_fl_10 (ED_buf1, pTab[i1].y);
        strcat(ED_buf1,",");
        UTX_add_fl_10 (ED_buf1, pTab[i1].z);
      }

      // Knotvec raus
      i2 = cpbsp->ptNr + cpbsp->deg + 1;
      if((strlen(ED_buf1)+(10*i2)) > bufLen) {
        TX_Error("CVBSP:textbuffer-overflow-error ");
        goto L_err_2;
      }
      for(i1=0; i1<i2; ++i1) {
        strcat(ED_buf1,",");
        // UTX_add_fl_u (ED_buf1, cpbsp->kvTab[i1]);
        UTX_add_fl_10 (ED_buf1, cpbsp->kvTab[i1]);
      }

      // Start/Endparameter raus (v0,v1)
      if((strlen(ED_buf1)+30) > bufLen) {
        TX_Error("CVBSP:textbuffer-overflow-error ");
        goto L_err_2;
      }
      strcat(ED_buf1,",");
      // UTX_add_fl_u2 (ED_buf1, cpbsp->v0, cpbsp->v1);
      UTX_add_fl_10 (ED_buf1, cpbsp->v0);
      strcat(ED_buf1,",");
      UTX_add_fl_10 (ED_buf1, cpbsp->v1);


    } else goto L_unknown_form;



  //=====================================================================
  } else if (typ == Typ_CVRBSP) {

    // hole naechsten freien Index
    if(ind < 0) {
      ++cv_ind;
      ind = cv_ind;
    }



    irc = ind;

    sprintf(ED_buf1,"S%ld=RBSP,",ind);
      // printf(" out Typ_CVRBSP: |%s|\n",ED_buf1);


    if(form == Typ_Txt) goto L_add_text;
    if(form != Typ_CVRBSP) goto L_unknown_form;

      prbsp = o1->data;  // data = memspc55 !!


      UTX_add_i_u (ED_buf1, prbsp->ptNr);
      strcat(ED_buf1,",");
      UTX_add_i_u (ED_buf1, prbsp->deg);
      // printf("Typ_CVRBSP: %s\n",ED_buf1);


      // Kontrollpunkte raus
      pTab = prbsp->cpTab;
      if((strlen(ED_buf1)+(25*prbsp->ptNr)) > bufLen) {
        TX_Error("CVRBSP:textbuffer-overflow-error ");
        goto L_err_2;
      }
      for(i1=0; i1<prbsp->ptNr; ++i1) {
        strcat(ED_buf1,",");
        // UTX_add_fl_u3 (ED_buf1, pTab[i1].x,pTab[i1].y,pTab[i1].z, ',');
        UTX_add_fl_10 (ED_buf1, pTab[i1].x);
        strcat(ED_buf1,",");
        UTX_add_fl_10 (ED_buf1, pTab[i1].y);
        strcat(ED_buf1,",");
        UTX_add_fl_10 (ED_buf1, pTab[i1].z);
      }


      // Knotvec raus
      i2 = prbsp->ptNr + prbsp->deg + 1;
      if((strlen(ED_buf1)+(10*i2)) > bufLen) {
        TX_Error("CVRBSP:textbuffer-overflow-error ");
        goto L_err_2;
      }
      for(i1=0; i1<i2; ++i1) {
        strcat(ED_buf1,",");
        UTX_add_fl_10 (ED_buf1, prbsp->kvTab[i1]);
      }

      // weights raus
      if((strlen(ED_buf1)+(10*prbsp->ptNr)) > bufLen) {
        TX_Error("CVRBSP:textbuffer-overflow-error ");
        goto L_err_2;
      }
      for(i1=0; i1<prbsp->ptNr; ++i1) {
        strcat(ED_buf1,",");
        UTX_add_fl_10 (ED_buf1, prbsp->wTab[i1]);
      }


      // Start/Endparameter raus (v0,v1)
      if((strlen(ED_buf1)+30) > bufLen) {
        TX_Error("CVBSP:textbuffer-overflow-error ");
        goto L_err_2;
      }
      strcat(ED_buf1,",");
      UTX_add_fl_10 (ED_buf1, prbsp->v0);
      strcat(ED_buf1,",");
      UTX_add_fl_10 (ED_buf1, prbsp->v1);






  //=====================================================================
  } else if (typ == Typ_CVTRM) {
     // nur Header generieren f AP_iges_ccv2txt
    if(ind < 0) {
      ++cv_ind;
      ind = cv_ind;
    }
/*
    if(ind >= APT_CV_SIZ) {
      TX_Error("max. Anzahl Curves erreicht");
      goto L_err_2;
    }
*/
    irc = ind;

    sprintf(ED_buf1,"S%ld=CCV ",ind);
      // printf(" out Typ_CVTRM: |%s|\n",ED_buf1);

    if(form == Typ_Txt) goto L_add_text;





  //=====================================================================
  } else if (typ == Typ_SUR) {

    // hole naechsten freien Index
    if(ind < 0) {
      ++su_ind;
      ind = su_ind;
    }
    irc = ind;
    // printf(" create A%d \n",ind);

    sprintf(ED_buf1,"A%ld=",ind);
      // printf(" out Typ_SUR: |%s|\n",ED_buf1);

    if(form == Typ_Txt) goto L_add_text;

      // goto L_unknown_form;






  //=====================================================================
  } else if (typ == Typ_SURRU) {

    // hole naechsten freien Index
    if(ind < 0) {
      ++su_ind;
      ind = su_ind;
    }
    irc = ind;

    sprintf(ED_buf1,"A%ld=SRU ",ind);
      // printf(" out Typ_SURRU: |%s|\n",ED_buf1);


    if(form == Typ_Txt) goto L_add_text;


    //-----------------------------------------
    if(form == Typ_ObjGX) {

      oTab = o1->data;

      ox1 = &oTab[0];
      if(ox1->form == Typ_Index) {
        // printf(" o1 typ=%d ind=%d\n",ox1->typ,(long)ox1->data);
        AP_obj_add_dbo (ED_buf1, ox1->typ, LONG_PTR(ox1->data));

      } else if(ox1->form == Typ_LN) {
        ln1 = (Line*)ox1->data;
        AP_obj_add_ln (ED_buf1, 0, &ln1->p1, &ln1->p2);

      } else if(ox1->form == Typ_CI) {
        AP_obj_add_ci_ (ED_buf1, (Circ*)ox1->data);

      } else if(ox1->form == Typ_CVBSP) {
        strcat (ED_buf1, " S(");
        i1 = AP_obj_add_cvBsp_b (ED_buf1, bufLen, (CurvBSpl*)ox1->data);
        if(i1 < 0) goto L_err_2;
        strcat (ED_buf1, ")");

      } else goto L_unknown_form;



      ox1 = &oTab[1];
      if(ox1->form == Typ_Index) {
        // printf(" o2 typ=%d ind=%d\n",ox1->typ,(long)ox1->data);
        AP_obj_add_dbo (ED_buf1, ox1->typ, LONG_PTR(ox1->data));

      } else if(ox1->form == Typ_LN) {
        ln1 = (Line*)ox1->data;
        AP_obj_add_ln (ED_buf1, 0, &ln1->p1, &ln1->p2);

      } else if(ox1->form == Typ_CI) {
        AP_obj_add_ci_ (ED_buf1, (Circ*)ox1->data);

      } else if(ox1->form == Typ_CVBSP) {
        strcat (ED_buf1, " S(");
        i1 = AP_obj_add_cvBsp_b (ED_buf1, bufLen, (CurvBSpl*)ox1->data);
        if(i1 < 0) goto L_err_2;
        strcat (ED_buf1, ")");

      } else goto L_unknown_form;

      if(ox1->dir != 0) 
        strcat (ED_buf1, " MOD(CW)");


    //-----------------------------------------
    } else {   // RCIR-Surf via Indextabelle
      goto L_unknown_form;
    }





  //=====================================================================
  } else if (typ == Typ_SURRV) {

    // hole naechsten freien Index
    if(ind < 0) {
      ++su_ind;
      ind = su_ind;
    }
    irc = ind;

    sprintf(ED_buf1,"A%ld=SRV",ind);
      // printf(" out Typ_SURRV: |%s|\n",ED_buf1);

    if(form == Typ_Txt) goto L_add_text;

    srv = o1->data;  // data = memspc55 !!
      // printf(" SRV %d %d %d %d %f %f %f %f\n",
        // srv->typCen,srv->indCen, srv->typCov,srv->indCen,
        // srv->ang1, srv->ang2, srv->v0, srv->v1);

    AP_obj_add_dbo (ED_buf1, srv->typCen,srv->indCen);
    AP_obj_add_dbo (ED_buf1, srv->typCov,srv->indCov);
    AP_obj_add_val (ED_buf1, UT_DEGREES(srv->ang1));
    AP_obj_add_val (ED_buf1, UT_DEGREES(srv->ang2));
    AP_obj_add_val (ED_buf1, srv->v0);
    AP_obj_add_val (ED_buf1, srv->v1);

    goto L_fertig;




  //=====================================================================
  } else if (typ == Typ_SURCIR) {

    // hole naechsten freien Index
    if(ind < 0) {
      ++su_ind;
      ind = su_ind;
    }
/*
    if(ind >= APT_SU_SIZ) {
      TX_Error("max. Anzahl Surf. erreicht");
      goto L_err_2;
    }
*/
    irc = ind;

    sprintf(ED_buf1,"A%ld=RCIR",ind);
      // printf(" out Typ_SURCIR: |%s|\n",ED_buf1);


    if(form == Typ_Txt) goto L_add_text;


    //-----------------------------------------
    ox1 = o1->data;  // erste Substruct

    if(ox1->form == Typ_PT) {           // RCIR-Surf via Koordinaten

      pTab = ox1->data;
      for(i1=0; i1<ox1->siz; ++i1) {
        AP_obj_add_pt (ED_buf1, &pTab[i1]);
      }


    //-----------------------------------------
    } else {   // RCIR-Surf via Indextabelle
      goto L_unknown_form;
    }










  //=====================================================================
  } else if (typ == Typ_SURBSP)          {
             // (typ == Typ_SURRBSP))    {

    // hole naechsten freien Index
    if(ind < 0) {
      ++su_ind;
      ind = su_ind;
    }

    irc = ind;

    // if(typ == Typ_SURBSP)
      sprintf(ED_buf1,"A%ld=BSP,",ind);
    // else
      // sprintf(ED_buf1,"A%ld=RBSP,",ind);
      // printf(" out Typ_SURBSP: |%s|\n",ED_buf1);

    if(form == Typ_Txt) goto L_add_text;

      subsp = o1->data;  // data = memspc55 !!
      // DEB_dump_obj__ (Typ_SURBSP, subsp, "SURBSP:");

      UTX_add_i_u (ED_buf1, subsp->ptUNr);
      strcat(ED_buf1,",");
      UTX_add_i_u (ED_buf1, subsp->ptVNr);
      strcat(ED_buf1,",");
      UTX_add_i_u (ED_buf1, subsp->degU);
      strcat(ED_buf1,",");
      UTX_add_i_u (ED_buf1, subsp->degV);
      // printf("Typ_SURBSP: %s\n",ED_buf1);


      // Kontrollpunkte raus
      pTab = subsp->cpTab;
      i2 = subsp->ptUNr * subsp->ptVNr;
      for(i1=0; i1<i2; ++i1) {

        if((strlen(ED_buf1)+100) > bufLen) {
          TX_Error("SURBSP:textbuffer-overflow-error ");
          goto L_err_2;
        }

        strcat(ED_buf1,",");
        UTX_add_fl_u3 (ED_buf1, pTab[i1].x,pTab[i1].y,pTab[i1].z, ',');
      }



      // Knotvec Richtg. U raus
      i2 = subsp->ptUNr + subsp->degU + 1;
      for(i1=0; i1<i2; ++i1) {
        if((strlen(ED_buf1)+20) > bufLen) {
          TX_Error("CVBSP:textbuffer-overflow-error ");
          goto L_err_2;
        }
        strcat(ED_buf1,",");
        UTX_add_fl_u (ED_buf1, subsp->kvTabU[i1]);
      }


      // Knotvec Richtg. V raus
      i2 = subsp->ptVNr + subsp->degV + 1;
      for(i1=0; i1<i2; ++i1) {
        if((strlen(ED_buf1)+20) > bufLen) {
          TX_Error("CVBSP:textbuffer-overflow-error ");
          goto L_err_2;
        }
        strcat(ED_buf1,",");
        UTX_add_fl_u (ED_buf1, subsp->kvTabV[i1]);
      }

      goto L_fertig;



  //=====================================================================
  } else if (typ == Typ_SURRBSP)    {

    // hole naechsten freien Index
    if(ind < 0) {
      ++su_ind;
      ind = su_ind;
    }

    irc = ind;

    // if(typ == Typ_SURBSP)
      // sprintf(ED_buf1,"A%ld=BSP,",ind);
    // else
      sprintf(ED_buf1,"A%ld=RBSP,",ind);
      // printf(" out Typ_SURBSP: |%s|\n",ED_buf1);


    if(form == Typ_Txt) goto L_add_text;


      srbsp = o1->data;  // data = memspc55 !!
      // DEB_dump_obj__ (Typ_SURBSP, srbsp, "SURBSP:");

      UTX_add_i_u (ED_buf1, srbsp->ptUNr);
      strcat(ED_buf1,",");
      UTX_add_i_u (ED_buf1, srbsp->ptVNr);
      strcat(ED_buf1,",");
      UTX_add_i_u (ED_buf1, srbsp->degU);
      strcat(ED_buf1,",");
      UTX_add_i_u (ED_buf1, srbsp->degV);
      // printf("Typ_SURBSP: %s\n",ED_buf1);


      // Kontrollpunkte raus
      pTab = srbsp->cpTab;
      i2 = srbsp->ptUNr * srbsp->ptVNr;
      for(i1=0; i1<i2; ++i1) {

        if((strlen(ED_buf1)+100) > bufLen) {
          TX_Error("SURBSP:textbuffer-overflow-error ");
          goto L_err_2;
        }

        strcat(ED_buf1,",");
        UTX_add_fl_u3 (ED_buf1, pTab[i1].x,pTab[i1].y,pTab[i1].z, ',');
      }


      // Knotvec Richtg. U raus
      i2 = srbsp->ptUNr + srbsp->degU + 1;
      for(i1=0; i1<i2; ++i1) {
        if((strlen(ED_buf1)+20) > bufLen) {
          TX_Error("CVBSP:textbuffer-overflow-error ");
          goto L_err_2;
        }
        strcat(ED_buf1,",");
        UTX_add_fl_u (ED_buf1, srbsp->kvTabU[i1]);
      }


      // Knotvec Richtg. V raus
      i2 = srbsp->ptVNr + srbsp->degV + 1;
      for(i1=0; i1<i2; ++i1) {
        if((strlen(ED_buf1)+20) > bufLen) {
          TX_Error("CVBSP:textbuffer-overflow-error ");
          goto L_err_2;
        }
        strcat(ED_buf1,",");
        UTX_add_fl_u (ED_buf1, srbsp->kvTabV[i1]);
      }


      // weights ausgeben
      i2 = srbsp->ptUNr * srbsp->ptVNr;
      da = ((SurRBSpl*)srbsp)->wTab;
      AP_obj_add_nval (ED_buf1, i2, da);
      // for(i1=0; i1<i2; ++i1) {
        // strcat(ED_buf1,",");
        // UTX_add_fl_u (ED_buf1, srbsp->kvTabU[i1]);
      // }






  //=====================================================================
  } else if (typ == Typ_PLN) {

    // hole naechsten freien PT-Index
    if(ind < 0) {
      ++pl_ind;
      ind = pl_ind;
    }

/*
    if(ind >= APT_PL_SIZ) {
      TX_Error("max. Anzahl Planes erreicht");
      goto L_err_2;
    }
*/
    irc = ind;

    sprintf(ED_buf1,"R%ld=",ind);
      // printf(" out Typ_PLN: |%s|\n",ED_buf1);

    if(form == Typ_Txt) goto L_add_text;

    pl1 = o1->data;

    // origin
    AP_obj_add_pt (ED_buf1, &pl1->po);
    // X-axis                                  // 2013-05-30
    AP_obj_add_vc (ED_buf1, &pl1->vx);
    // Y-axis
    AP_obj_add_vc (ED_buf1, &pl1->vy);

/*
    // Z-Achse
    AP_obj_add_vc (ED_buf1, &pl1->vz);
    // Punkt auf der X-Achse bilden
    pt1.x = pl1->po.x + pl1->vx.dx;
    pt1.y = pl1->po.y + pl1->vx.dy;
    pt1.z = pl1->po.z + pl1->vx.dz;
    AP_obj_add_pt (ED_buf1, &pt1);
*/




  //=====================================================================
  } else if (typ == Typ_CON) {

    // hole naechsten freien PT-Index
    if(ind < 0) {
      ++bd_ind;
      ind = bd_ind;
    }

/*
    if(ind >= APT_PL_SIZ) {
      TX_Error("max. Anzahl Bodies erreicht");
      goto L_err_2;
    }
*/
    irc = ind;

    sprintf(ED_buf1,"B%ld=CON",ind);
      // printf(" out Typ_CON: |%s|\n",ED_buf1);

    // B=CON center1 center2 radius1 radius2

    bco = o1->data;

    AP_obj_add_pt (ED_buf1, &bco->pl.po);

    UT3D_pt_traptvclen (&pt1, &bco->pl.po, &bco->pl.vz, bco->h);
    AP_obj_add_pt (ED_buf1, &pt1);

    // radius
    AP_obj_add_val (ED_buf1, bco->r1);
    AP_obj_add_val (ED_buf1, bco->r2);








  //=====================================================================
  } else if (typ == Typ_TOR) {

    // hole naechsten freien PT-Index
    if(ind < 0) {
      ++bd_ind;
      ind = bd_ind;
    }

/*
    if(ind >= APT_PL_SIZ) {
      TX_Error("max. Anzahl Bodies erreicht");
      goto L_err_2;
    }
*/
    irc = ind;

    sprintf(ED_buf1,"B%ld=TOR",ind);
      // printf(" out Typ_TOR: |%s|\n",ED_buf1);

    // B=TOR center1 vz radius1 radius2

    bco = o1->data;

    AP_obj_add_pt (ED_buf1, &bco->pl.po);

    AP_obj_add_vc (ED_buf1, &bco->pl.vz);


    // radius
    AP_obj_add_val (ED_buf1, bco->r1);
    AP_obj_add_val (ED_buf1, bco->r2);






  //=====================================================================
  } else if (typ == Typ_SOL) {

    // hole naechsten freien Index
    if(ind < 0) {
      ++bd_ind;
      ind = bd_ind;
    }

    irc = ind;

    sprintf(ED_buf1,"B%ld=",ind);

    if(form == Typ_Txt) goto L_add_text;

    goto L_unknown_form;



  //=====================================================================
  } else if (typ == Typ_Model) {
    // create subModel-call; eg "M1="<modelname>" origin scl vz vx
    // in: ModelRef mdr

    // hole naechsten freien Index
    if(ind < 0) {
      ++mr_ind;
      ind = mr_ind;
    }

    irc = ind;

    if(form == Typ_Txt) {
      sprintf(ED_buf1,"M%ld=",ind);
      goto L_add_text;
    }

    mdr = o1->data;
      // DEB_dump_obj__ (Typ_Model, mdr, "ModRef - ");

    // catalog-part | ditto ?
    DB_mdlTyp_iBas (&i1, mdr->modNr);
    if(i1 == -3) return -1;

    // get modelname of basic-model-nr
    cp1 = DB_mdlNam_iBas (mdr->modNr);
    if(!cp1) return -1;

    if(i1 == MBTYP_CATALOG) {
      CTLG_PartID_mnam (cbuf, cp1);
      sprintf(ED_buf1,"M%ld=CTLG \"%s\"",ind,cbuf);
    } else {
      sprintf(ED_buf1,"M%ld=\"%s\"",ind,cp1);
    }
      // printf(" out Typ_Model: |%s|\n",ED_buf1);

    // add origin
    AP_obj_add_pt (ED_buf1, &mdr->po);
      // printf(" M+po: |%s|\n",ED_buf1);


    // scale; default = 1
    if(fabs(mdr->scl - 1.) > 0.1) {
      AP_obj_add_val (ED_buf1, mdr->scl);
    }


    // wenn vx != UT3D_VECTOR_X muss auch vz raus !
    // if((&mdr->vz == &UT3D_VECTOR_Z)&&(&mdr->vx == &UT3D_VECTOR_X)) goto L_fertig;
    AP_obj_add_vc (ED_buf1, &mdr->vz);
      // printf(" M+vz: |%s|\n",ED_buf1);

    // if(&mdr->vx == &UT3D_VECTOR_X) goto L_fertig;
    AP_obj_add_vc (ED_buf1, &mdr->vx);
      // printf(" ModRef = |%s|\n",ED_buf1);





  //=====================================================================
  } else {
    TX_Print("AP_stru_2_txt: Error 1 / typ=%d form=%d\n",typ,form);
    goto  L_err_1;
  }





  //----------------------------------------------------------------
  L_fertig:
    // printf("ex AP_stru_2_txt|%s|\n",ED_buf1);
    // exit(0);

  act_typ = typ;
  act_ind = ind;

  return irc;


  L_err_1:
  return -1;

  L_err_2:
  return -2;



  //----------------------------------------------------------------
  L_unknown_form:
  TX_Print("AP_stru_2_txt: unknown form %d; typ %d\n",form,typ);
  DEB_dump_ox_0 (o1, "");
  // TX_Error("AP_stru_2_txt: unknown form %d; typ %d\n",form,typ);
  // exit(0);
  goto  L_err_1;



  //----------------------------------------------------------------
  L_buffer_too_small:
  TX_Print("AP_stru_2_txt: Buffer too small");
  goto L_err_2;



  //----------------------------------------------------------------
  L_add_text:
  // printf(" L_add_text: |%s|\n",(char*)o1->data); // exit(0);

  if((strlen(ED_buf1)+strlen(o1->data)) > bufLen) goto L_buffer_too_small;

  strcat(ED_buf1,(char*)o1->data);

  goto L_fertig;
}




/* ====================== EOF =========================== */
