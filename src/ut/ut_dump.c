// ut_dump.c   
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
2009-11-20 aus ../ut/ut_geo3d.c extrahiert. RF.

-----------------------------------------------------
*/
/*!
\file  ../ut/ut_dump.c
\brief dump structs, objects 
\code
=====================================================
List_functions_start:

UT3D_stru_dump
UT3D_dump__
OVR_dump_add

List_functions_end:
=====================================================
see also
UT3D_dump_txt
\endcode *//*----------------------------------------



  UT3D_stru_dump (Typ_PT, &pt1, "indicatedPos=");

  UT3D_stru_dump (TYP_FuncAdd, fnam, "");    // dump file

  sprintf(cbuf, "\n put out this text ..\n");
  UT3D_stru_dump(Typ_Txt, cbuf, "");

  UT3D_dump_txt("Date:          %s",OS_date1());


*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>                    // va_list

#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_gtypes.h"           // AP_src_typ__
#include "../ut/ut_msh.h"              // Fac3 ..
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_txTab.h"            // TxtTab
#include "../ut/ut_memTab.h"           // MemTab
#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_err.h"              // ERR_SET1

#include "../gr/ut_UI.h"               // Typ_Att_hili
#include "../db/ut_DB.h"               // DB_

#include "../xa/xa_ico.h"              // ICO_PT,




// LOCAL:

// #define tmpSiz 50000       // size in chars for strings
                           // for BSP-Sur's 50000 ! 2011-08-23 




//================================================================
  int UT3D_dump_txt (char* txt, ...) {
//================================================================
/// formatiertes Rausschreiben; wie bei printf.
/// Beispiel:
/// TX_Print("Value d1 = %f string =|%s|",d1,cBuf);


  va_list va;
  char    cbuf1[1024];



  va_start (va, txt);
  vsprintf (cbuf1, txt, va);
  va_end (va);

  // if(strlen(cbuf1) > 255) {
    // cbuf1[256] ='\0';
  // }

  UT3D_stru_dump (Typ_Txt, cbuf1, "");

  return 0;

}




//===============================================================
  int UT3D_stru_dump (int typ, void *data, char *txt, ...) {
//===============================================================
/// \code
/// UT3D_stru_dump            Testausgabe Geom.Element.
/// typ = TYP_FuncInit:  ab nun Ausgabe -> Datei "txt"  (Open File)
/// typ = TYP_FuncEnd:   ab nun wiederAusgabe -> term.  (Close File)
/// typ = TYP_FuncInit1: ab nun Ausgabe -> FILE "txt"  (File must be open)
/// typ = TYP_FuncExit:  ab nun wiederAusgabe -> term.  (does not close File)
///
/// resolv ObjGX: see UTO_dump__
/// \endcode



static int      DestFlag = 0;
static FILE     *uo = NULL;

  int        irc, i1, i2, ii;
  char       cbuf[256], *p1, *tmpSpc;
  UtxTab_NEW (txTab1);
  va_list    va;


  printf("UT3D_stru_dump typ=%d txt=|%s|\n",typ,txt);



  //----------------------------------------------------------------
  // Init = Umleitung -> File f alle nachfolgenden Ausgaben
  if(typ == TYP_FuncInit) {
    DestFlag = 1;
    // if(uo) fclose (uo);  GEHT nicht mit stdout; kein reOpen moeglich
    uo = NULL;
    if((uo=fopen(txt,"w")) == NULL) {
        TX_Print("UT3D_stru_dump open Error |%s|",txt);
        return -1;
    }
    if(data) {
      if(!strcmp(data,"htm"))
        fprintf(uo, "<HTML>\n<HEAD>\n</HEAD>\n<BODY>\n<PRE>\n");
    }
    return 0;


  //----------------------------------------------------------------
  } else if(typ == TYP_FuncInit1) {
    // if(uo) fclose (uo);
    uo =  (FILE*)data;
    DestFlag = 1;
    return 0;


  //----------------------------------------------------------------
  } else if(typ == TYP_FuncAdd) {
    // add file <data>
    // write file -> alread open lun uo ..
    UTX_cat_file (uo, data);
    return 0;



  //----------------------------------------------------------------
  } else if(typ == TYP_FuncEnd) {
    if(data) {
      if(!strcmp(data,"htm"))
        fprintf(uo, "</PRE>\n</HTML>\n");
    }
    if(uo) fclose (uo);
    uo = stdout;
    DestFlag = 0;
    return 0;


  //----------------------------------------------------------------
  } else if(typ == TYP_FuncExit) {
    uo = stdout;
    DestFlag = 0;
    return 0;

  }


  //----------------------------------------------------------------
  if(DestFlag == 0) uo = stdout;

  va_start(va,txt);
  vsprintf(cbuf,txt,va);
  va_end(va);

    // printf(" _stru_dump cbuf |%s|\n",cbuf);

  // sprintf(cbuf,txt,v1,v2,v3,v4,v5,v6);
  // UTX_CleanCR (TX_buf1);
  // printf("%s",cbuf);
  // fprintf(uo, "%s",cbuf);                    //2010-01-01


  // p1 = (char*) UME_alloc_tmp (tmpSiz);   // 50k

  irc = -1;
  i1 = 100000;  // space for outputdata
  tmpSpc = (char*) malloc (i1); 
  if(!tmpSpc) {TX_Print("***** UT3D_stru_dump EOM *****"); return -1;}
  UtxTab_init_spc (&txTab1, tmpSpc, i1);

  // create strings of infos about struct
  i1 = UT3D_dump__ (&txTab1, typ, data, cbuf, -1, 0);
  if(i1 < 0) goto L_exit;
    // printf(" UT3D_stru_dump - nach UT3D_dump__\n");


  // add infos about struct to node parNd
  for(i1=0; i1<txTab1.iNr; ++i1) {
    p1 = UtxTab__(i1, &txTab1);
    // sscanf(p1, "%4d", &ii);          // get ParentRecordNr from first 4 chars
      // printf(" ii=%d p1=|%s|\n",ii,p1);
    // ii *= 2;
    // i2 = ILIM2 (ii, 0, 100);
      // printf(" i2=%d\n",i2);
    // memset(cbuf,' ', i2);
    // cbuf[i2] = '\0';
    // sscanf(p1, "%2d", &ii);            // get iconNr from first 2 chars
      // printf(" [%d]=|%s|\n",i1,ii,&p1[2]);
    // fprintf(uo, "%s%s\n", cbuf,&p1[6]);
    fprintf(uo, "%s\n",&p1[6]);
  }

  irc = 0;

  L_exit:
  free (tmpSpc);
  // es ist kein UtxTab_free erforderlich - tmpSiz liegt in tempspace !!!!!!

    // printf("ex UT3D_stru_dump\n");

  return irc;

}


//===============================================================
  int UT3D_dump__ (TxtTab *sTab, int typ, void *data, char *txt,
                  int ipar, int mode) {
//===============================================================
// create info-textstrings describing binary object typ/data (structured)
// Input:
//   typ                   Typ_ObjGX,Typ_Index,Typ_CV,Typ_SUR,Typ_SOL ..
//   data                  struct of type <typ>
//   txt                   obj.description
//   ipar                  parentRecordNr
//   mode                  0=normal object; create info-textstrings
//                         1=internal object; do not create info-textstrings
// Output:
//   characters 0-3 = level (ParentRecordNr)
//   characters 4-5 = IconNr

// UT3D_dump__ < UTO_dump__: typ is ox->form !

// icons see ../gtk/ut_tree_it.h
// ICO_data for binary data,
// ICO_link for links into DB

//see UT3D_stru_dump


  void     *v1;
  int      irc, i1, i2, i3, i4, ptNr, *iTab, sTyp;
  long     dbi, il1;
  char     oNam[32], sAux[64], cbuf[256], *cps, *cp1;
  double   d1, *dp;
  Point    *p1, *ptAr;
  Point2   *pt2Ar;
  // Ray      *lnr;
  Line     *l1;
  Line2    *l2;
  Circ     *c1;
  Circ2    *c2;
  Circ2C   *c2c;
  Vector   *vc1;
  Vector2  *vc2;
  CurvElli   *e1;
  CurvEll2   *e2;
  CurvEll2C  *el2c;
  CurvBSpl   *cvbs;
  CurvBSpl2  *cv2bs;
  CurvRBSpl  *cvrbs;
  // CurvComp   *cvcvc;
  CurvCCV    *cvccv;
  SurBSpl    *sbs;
  SurRBSpl   *srbs;
  SurStripe  *sus;
  Verts      *verts1;
  Faces      *faces1;
  Edges      *edges1;
  CurvPoly   *plg;
  Polynom1   *pol1;
  Polynom3   *pol3;
  polynom_d3 *pold3;
  Polynom_ *pol_;
  Plane    *pln;
  Sphere   *sph;
  Conus    *con;
  Torus    *tor;
  SurRev   *srv;
  SurSwp   *swp;
  SurHat   *shat;
  double   *ma;
  ModelRef *mdr;
  ModelBas *mdb;
  Dimen    *dim;
  Dim3     *dim3;
  AText    *atx;
  GText    *gtx;
  TexBas   *btex;
  TexRef   *rtex;
  Ind_Att_ln *lTyp;
  ColRGB   *col;
  ObjSRC   *os1;
  IndTab   *it1;
  ObjDB    *odb1;
  ObjGX    *ox, *o2, *o3, oo1;
  ModelBas* DB_get_ModBas    ();
  BndSur   *sba; // Typ_SURBND
  wPoint   *wpt;
  // EdgSur   *esa;



  // printf("UT3D_dump__ typ=%d txt=|%s| ipar=%d\n",typ,txt,ipar);


  if(!data) {
    // first 6 charaters become replaced in UT3D_dump_add !
    sprintf(cbuf, "123456 %s data = NULL ",AP_src_typ__(typ));
    UT3D_dump_add (sTab, cbuf, ipar, 0);
    return -1;
  }


  i1 = ERR_SET1 ();
  if(i1) {
    TX_Print("***** ERROREXIT UT3D_dump__ %d *****\n",typ);
    irc = -1;
    goto L_done;
  }



  cps = &cbuf[6];

  //----------------------------------------------------------------
  if(typ == Typ_PT2) {
    p1 = data;
    sprintf(cps,"Point2 %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    sprintf(cps,"(Point2) %9.3f,%9.3f", p1->x, p1->y);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);



  //----------------------------------------------------------------
  } else if(typ == Typ_PT) {
    p1 = data;
    if(strlen(txt)) {
      sprintf(cps,"Point %s",txt);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    }
    sprintf(cps,"(Point) %9.3f,%9.3f,%9.3f",p1->x,p1->y,p1->z);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);


  //----------------------------------------------------------------
  } else if(typ == Typ_WPT) {
    wpt = data;
    if(strlen(txt)) {
      sprintf(cps,"wPoint %s",txt);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    }
    sprintf(cps,"(wPoint) %9.3f,%9.3f,%9.3f w=%9.3f",wpt->x,wpt->y,wpt->z,wpt->w);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);




  //----------------------------------------------------------------
  } else if(typ == Typ_VC2) {
    vc2 = data;
    sprintf(cps,"Vector2 %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_VC);
    sprintf(cps,"(Vector2) %9.3f,%9.3f",vc2->dx,vc2->dy);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_VC);


  //----------------------------------------------------------------
  } else if(typ == Typ_VC) {
    vc1 = data;
    sprintf(cps,"Vector %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_VC);
    sprintf(cps,"(Vector) %9.3f,%9.3f,%9.3f",vc1->dx,vc1->dy,vc1->dz);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_VC);

/*
  //----------------------------------------------------------------
  } else if(typ == Typ_Ray) {
    lnr = data;
    sprintf(cps,"Ray %s typ = %d",txt,lnr->typ);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_CI);
    sprintf(cps,"(Ray).p=%9.3f,%9.3f,%9.3f",lnr->p.x,lnr->p.y,lnr->p.z);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    sprintf(cps,"(Ray).v=%9.3f,%9.3f,%9.3f",lnr->v.dx,lnr->v.dy,lnr->v.dz);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
*/

  //----------------------------------------------------------------
  } else if(typ == Typ_LN2) {
    l2 = data;
    sprintf(cps,"2D-Line %s typ = %d",txt,l2->typ);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_CI);
    sprintf(cps,"(Line2).p1=%9.3f,%9.3f",l2->p1.x,l2->p1.y);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    sprintf(cps,"(Line2).p2=%9.3f,%9.3f",l2->p2.x,l2->p2.y);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);


  //----------------------------------------------------------------
  } else if(typ == Typ_LN) {
    l1 = data;
    sprintf(cps,"Line %s typ = %d",txt,l1->typ);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_LN);
    sprintf(cps,"(Line).p1=%9.3f,%9.3f,%9.3f",l1->p1.x,l1->p1.y,l1->p1.z);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    sprintf(cps,"(Line).p2=%9.3f,%9.3f,%9.3f",l1->p2.x,l1->p2.y,l1->p2.z);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);


  //----------------------------------------------------------------
  } else if(typ == Typ_CI2C) {
    c2c = data;
    sprintf(cps,"2D-CenterCircle %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_CI);
    sprintf(cps,"(Circ2C).r=%9.3f as=%9.3f ao=%9.3f",c2c->rad,c2c->angs,c2c->ango);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    
    
  //----------------------------------------------------------------
  } else if(typ == Typ_CI2) {
    c2 = data;
    sprintf(cps,"2D-Circle %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_CI);
    sprintf(cps,"(Circ2).p1=%9.3f,%9.3f",c2->p1.x,c2->p1.y);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    sprintf(cps,"(Circ2).p2=%9.3f,%9.3f",c2->p2.x,c2->p2.y);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    sprintf(cps,"(Circ2).pc=%9.3f,%9.3f",c2->pc.x,c2->pc.y);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    sprintf(cps,"(Circ2).r=%9.3f a=%9.3f", c2->rad,c2->ango);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  } else if(typ == Typ_CI) {
    c1 = data;
    sprintf(cps,"Circle %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_CI);
    sprintf(cps,"(Circ).p1=%9.3f,%9.3f,%9.3f",c1->p1.x,c1->p1.y,c1->p1.z);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    sprintf(cps,"(Circ).p2=%9.3f,%9.3f,%9.3f",c1->p2.x,c1->p2.y,c1->p2.z);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    sprintf(cps,"(Circ).pc=%9.3f,%9.3f,%9.3f",c1->pc.x,c1->pc.y,c1->pc.z);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    sprintf(cps,"(Circ).vz=%9.3f,%9.3f,%9.3f",c1->vz.dx,c1->vz.dy,c1->vz.dz);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_VC);
    sprintf(cps,"(Circ).r =%9.3f    .ango=%9.3f",c1->rad,c1->ango);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);


  //----------------------------------------------------------------
  } else if(typ == Typ_CVELL2C) {
    el2c = data;
    sprintf(cps,"2D-CenterEllipse %s sr=%d",txt,el2c->sr);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_CV);
    sprintf(cps,"(CurvEll2C).p1=%9.3f,%9.3f",el2c->p1.x,el2c->p1.y);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    sprintf(cps,"(CurvEll2C).p2=%9.3f,%9.3f",el2c->p2.x,el2c->p2.y);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    sprintf(cps,"(CurvEll2).a=%9.3f",el2c->a);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_VC);
    sprintf(cps,"(CurvEll2).b=%9.3f",el2c->b);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_VC);
  

  //----------------------------------------------------------------
  } else if(typ == Typ_CVELL2) {
    e2 = data;
    sprintf(cps,"2D-Ellipse %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_CV);
    sprintf(cps,"(CurvEll2).p1=%9.3f,%9.3f",e2->p1.x,e2->p1.y);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    sprintf(cps,"(CurvEll2).p2=%9.3f,%9.3f",e2->p2.x,e2->p2.y);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    sprintf(cps,"(CurvEll2).pc=%9.3f,%9.3f",e2->pc.x,e2->pc.y);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    sprintf(cps,"(CurvEll2).va=%9.3f,%9.3f",e2->va.dx,e2->va.dy);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_VC);
    sprintf(cps,"(CurvEll2).vb=%9.3f,%9.3f",e2->vb.dx,e2->vb.dy);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_VC);
    sprintf(cps,"(CurvEll2).dir=%d",e2->dir);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);


  //----------------------------------------------------------------
  } else if(typ == Typ_CVELL) {
    e1 = data;
    sprintf(cps,"Ellipse %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_CV);
    sprintf(cps,"(CurvElli).p1=%9.3f,%9.3f,%9.3f",e1->p1.x,e1->p1.y,e1->p1.z);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    sprintf(cps,"(CurvElli).p2=%9.3f,%9.3f,%9.3f",e1->p2.x,e1->p2.y,e1->p2.z);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    sprintf(cps,"(CurvElli).pc=%9.3f,%9.3f,%9.3f",e1->pc.x,e1->pc.y,e1->pc.z);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    sprintf(cps,"(CurvElli).va=%9.3f,%9.3f,%9.3f",e1->va.dx,e1->va.dy,e1->va.dz);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_VC);
    sprintf(cps,"(CurvElli).vb=%9.3f,%9.3f,%9.3f",e1->vb.dx,e1->vb.dy,e1->vb.dz);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_VC);
    sprintf(cps,"(CurvElli).vz=%9.3f,%9.3f,%9.3f",e1->vz.dx,e1->vz.dy,e1->vz.dz);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_VC);
    sprintf(cps,"(CurvElli).dir=%d",e1->dir);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  } else if(typ == Typ_CVBSP) {
    cvbs = data;
    sprintf(cps,"B-Spline %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_CV);
    sprintf(cps,"  ptNr=%d deg=%d v0=%.6f,v1=%.6f dir=%d",
            cvbs->ptNr, cvbs->deg, cvbs->v0, cvbs->v1, cvbs->dir);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    for(i1=0; i1<cvbs->ptNr; ++i1) {
      sprintf(cps," p[%d] = %12.6f,%12.6f,%12.6f",i1,
              cvbs->cpTab[i1].x,cvbs->cpTab[i1].y,cvbs->cpTab[i1].z);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    }
    i2 = cvbs->ptNr + cvbs->deg + 1;
    for(i1=0; i1<i2; ++i1) {
      sprintf(cps," k[%d] = %9.6f",i1, (cvbs->kvTab)[i1]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    }



  //----------------------------------------------------------------
  } else if(typ == Typ_CVBSP2) {
    cv2bs = data;
    sprintf(cps,"2D-B-Spline %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_CV);
    sprintf(cps,"  ptNr=%d deg=%d v0=%.6f,v1=%.6f dir=%d",
            cv2bs->ptNr, cv2bs->deg, cv2bs->v0, cv2bs->v1, cv2bs->dir);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    for(i1=0; i1<cv2bs->ptNr; ++i1) {
      sprintf(cps," p[%d] = %12.6f,%12.6f",i1,
              cv2bs->cpTab[i1].x,cv2bs->cpTab[i1].y);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    }
    i2 = cv2bs->ptNr + cv2bs->deg + 1;
    for(i1=0; i1<i2; ++i1) {
      sprintf(cps," k[%d] = %9.6f",i1, (cv2bs->kvTab)[i1]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    }




  //----------------------------------------------------------------
  } else if(typ == Typ_CVRBSP) {
    cvrbs = data;
    sprintf(cps,"Rat-B-Spline %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_CV);
    sprintf(cps,"    ptNr=%d deg=%d v0=%.6f,v1=%.6f dir=%d",
            cvrbs->ptNr, cvrbs->deg, cvrbs->v0, cvrbs->v1, cvrbs->dir);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    for(i1=0; i1<cvrbs->ptNr; ++i1) {
      sprintf(cps," p[%d] = %12.6f,%12.6f,%12.6f  w = %9.6f",i1,
              cvrbs->cpTab[i1].x,cvrbs->cpTab[i1].y,cvrbs->cpTab[i1].z,
              cvrbs->wTab[i1]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    }
    i2 = cvrbs->ptNr + cvrbs->deg + 1;
    for(i1=0; i1<i2; ++i1) {
      sprintf(cps," k[%d] = %9.6f",i1, (cvrbs->kvTab)[i1]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    }


  //----------------------------------------------------------------
  } else if(typ == Typ_CVPOL) {
    plg = data;
    sprintf(cps,"Polygon %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_CV);
    sprintf(cps,"(CurvPoly).ptNr=%d v0=%.3f v1=%.3f dir=%d",
            plg->ptNr, plg->v0, plg->v1, plg->dir);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);

    strcpy (sAux, "---");
    for(i1=0; i1<plg->ptNr; ++i1) {
      // if(i1>50) break;
      if(plg->lvTab) sprintf(sAux, "%.3f", plg->lvTab[i1]);
      else dp = NULL;
      sprintf(cps," p[%d] = %.3f %.3f %.3f lv=%s",i1,
              plg->cpTab[i1].x,
              plg->cpTab[i1].y,
              plg->cpTab[i1].z,
              sAux);                  // lvTab
        // printf(" p[%d] |%s|\n",i1,cps);
      i2 = UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
      if(i2 < 0) break;
    }


  //----------------------------------------------------------------
  } else if(typ == Typ_CVCLOT) {
    // clot = data;
    sprintf(cps,"Clothoid %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_CV);
    sprintf(cps," pc = %9.3f   lr = %d",   ((CurvClot*)data)->pc,
                                            ((CurvClot*)data)->lr);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps," cs = %9.3f   ce = %9.3f",((CurvClot*)data)->cs,
                                            ((CurvClot*)data)->ce);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    p1 = &((CurvClot*)data)->stp;
    sprintf(cps," stp   %9.3f,%9.3f,%9.3f",p1->x,p1->y,p1->z);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    vc1 = &((CurvClot*)data)->stv;
    sprintf(cps," stv   %9.3f,%9.3f,%9.3f",vc1->dx,vc1->dy,vc1->dz);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    vc1 = &((CurvClot*)data)->plv;
    sprintf(cps," plv   %9.3f,%9.3f,%9.3f",vc1->dx,vc1->dy,vc1->dz);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  } else if(typ == Typ_CVBEZ) {
    sprintf(cps,"BezierCurve %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_CV);
    ptNr = ((CurvBez*)data)->ptNr;
    ptAr = ((CurvBez*)data)->cptab;
    sprintf(cps," Bez ptNr=%d va=%.3f,vb=%.3f",ptNr,
           ((CurvBez*)data)->va, ((CurvBez*)data)->vb);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    for(i1=0; i1<ptNr; ++i1) {
      sprintf(cps," P[%d] = %9.3f %9.3f %9.3f",i1,
                           ptAr[i1].x,ptAr[i1].y,ptAr[i1].z);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    }


  //----------------------------------------------------------------
  } else if(typ == Typ_CVRBEZ) {
    sprintf(cps,"Rat.Bez.Curve %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_CV);
    ptNr = ((CurvRBez*)data)->ptNr;
    ptAr = ((CurvRBez*)data)->cptab;
    sprintf(cps," R.Bez ptNr=%d va=%.3f,vb=%.3f",ptNr,
           ((CurvRBez*)data)->va, ((CurvRBez*)data)->vb);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    for(i1=0; i1<ptNr; ++i1) {
      sprintf(cps," P[%d] = %9.3f %9.3f %9.3f w = %9.3f",i1,
                           ptAr[i1].x,ptAr[i1].y,ptAr[i1].z,
                           ((CurvRBez*)data)->wtab[i1]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    }


  //----------------------------------------------------------------
  } else if(typ == Typ_Polynom1) {

    pol1 = (Polynom1*)data;

    sprintf(cps,"Polynom1Curve %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_CV);

    ptNr = pol1->polNr;

    for(i1=0; i1<ptNr; ++i1) {
       sprintf(cps," [%d] %9.3f",i1, pol1->pol[i1]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    }



  //----------------------------------------------------------------
  } else if((typ == Typ_CVPSP3)      ||
            (typ == Typ_polynom_d3))    {

    pold3 = (polynom_d3*)data;

    sprintf(cps,"Polynom3 %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps," u=%9.3f",pold3->u);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    x    a=%9.3f b=%9.3f c=%9.3f d=%9.3f",
         pold3->x.a,pold3->x.b,pold3->x.c,pold3->x.d);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    y    a=%9.3f b=%9.3f c=%9.3f d=%9.3f",
         pold3->y.a,pold3->y.b,pold3->y.c,pold3->y.d);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    z    a=%9.3f b=%9.3f c=%9.3f d=%9.3f",
         pold3->z.a,pold3->z.b,pold3->z.c,pold3->z.d);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  } else if(typ == Typ_Polynom3) {

    pol3 = (Polynom3*)data;
    ptNr = pol3->polNr;

    sprintf(cps,"Polynom3Curve %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_CV);
    for(i1=0; i1<ptNr; ++i1) {
       sprintf(cps," [%d] %9.3f %9.3f %9.3f",i1,
         pol3->pol[0][i1],pol3->pol[1][i1],pol3->pol[2][i1]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    }



  //----------------------------------------------------------------
  } else if(typ == Typ_Polynom_) {

    pol_ = (Polynom_*)data;
    i3   = pol_->dim2;

    sprintf(cps,"Polynom %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    for(i1=0; i1<pol_->dim1; ++i1) {
      for(i2=0; i2<pol_->dim2; ++i2) {
        sprintf(cps," [%d][%d] %9.3f",i1,i2,pol_->pol[i1*i3+i2]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
      }
    }



  //----------------------------------------------------------------
  } else if(typ == Typ_PLN) {
    pln = data;
    sprintf(cps,"Plane %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PLN);
    sprintf(cps," .po=%9.3f,%9.3f,%9.3f",pln->po.x,pln->po.y,pln->po.z);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    sprintf(cps," .vx=%9.3f,%9.3f,%9.3f",pln->vx.dx,pln->vx.dy,pln->vx.dz);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_VC);
    sprintf(cps," .vy=%9.3f,%9.3f,%9.3f",pln->vy.dx,pln->vy.dy,pln->vy.dz);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_VC);
    sprintf(cps," .vz=%9.3f,%9.3f,%9.3f",pln->vz.dx,pln->vz.dy,pln->vz.dz);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_VC);



  //----------------------------------------------------------------
  } else if(typ == Typ_SURBND) {
    sba = data;
    sprintf(cps,"BoundedSurface %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_SUR);
    sprintf(cps,"  Bnd-Sur suID=%d contNr=%d typb=%d typt=%d stat=%d",
                      sba->suID,sba->contNr,sba->typb,sba->typt,sba->stat);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    // sprintf(cps,"  edgSt=%d edgMax=%d", sba->edgSt, sba->edgMax);
      // UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    
    // for(i1=0; i1<sba->seNr; ++i1) {
      // // p1 = &sbs->cpTab[(i1*sbs->ptUNr) + i2];
      // sprintf(cps,"    sba[%d] = ",i2);
      // UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    // }



  //----------------------------------------------------------------
  } else if(typ == Typ_SURBSP) {
    sbs = data;
    sprintf(cps,"B-SplineSurface %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_SUR);
    sprintf(cps,"  BSp-Sur ptUNr=%ld ptVNr=%ld degU=%d degV=%d",
                sbs->ptUNr,sbs->ptVNr, sbs->degU,sbs->degV);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);

    // Punkte
    for(i1=0; i1<sbs->ptVNr; ++i1) {
      for(i2=0; i2<sbs->ptUNr; ++i2) {
        p1 = &sbs->cpTab[(i1*sbs->ptUNr) + i2];
        sprintf(cps,"     pt[u%d][v%d] = %12.6f,%12.6f,%12.6f",i2,i1,
                                      p1->x,p1->y,p1->z);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
      }
    }
    // Anzahl Knots in U:
    i2 = sbs->ptUNr + sbs->degU + 1;           // Anzahl U-Knots
    for(i1=0; i1<i2; ++i1) {
      sprintf(cps," kU[%d] = %f",i1,sbs->kvTabU[i1]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    }
    // Bereich Knotvektor U:
    sprintf(cps," kU von %f bis %f",sbs->kvTabU[0],sbs->kvTabU[i2-1]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);

    // Anzahl Knots in V:
    i2 = sbs->ptVNr + sbs->degV + 1;           // Anzahl V-Knots
    for(i1=0; i1<i2; ++i1) {
      sprintf(cps," kV[%d] = %f",i1,sbs->kvTabV[i1]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    }
    // Bereich Knotvektor V:
    sprintf(cps," kV von %f bis %f",sbs->kvTabV[0],sbs->kvTabV[i2-1]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);




  //----------------------------------------------------------------
  } else if(typ == Typ_SURRBSP) {
    srbs = data;
    sprintf(cps,"Rat.B-SplineSurface %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_SUR);
    sprintf(cps,"  ptUNr=%ld ptVNr=%ld degU=%d degV=%d",
                srbs->ptUNr,srbs->ptVNr, srbs->degU,srbs->degV);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);

    // Punkte
    for(i1=0; i1<srbs->ptVNr; ++i1) {
      i3 = i1 * srbs->ptUNr;
      for(i2=0; i2<srbs->ptUNr; ++i2) {
        p1 = &srbs->cpTab[i3 + i2];
        sprintf(cps,"     pt[u%d][v%d] = %12.6f,%12.6f,%12.6f  w = %9.6f",
                i2,i1, p1->x, p1->y, p1->z, srbs->wTab[i3 + i2]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
      }
    }
    // Anzahl Knots in U:
    i2 = srbs->ptUNr + srbs->degU + 1;           // Anzahl U-Knots
    for(i1=0; i1<i2; ++i1) {
      sprintf(cps," kU[%d] = %f",i1,srbs->kvTabU[i1]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    }
    // Bereich Knotvektor U:
    sprintf(cps," kU von %f bis %f",srbs->kvTabU[0],srbs->kvTabU[i2-1]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);

    // Anzahl Knots in V:
    i2 = srbs->ptVNr + srbs->degV + 1;           // Anzahl V-Knots
    for(i1=0; i1<i2; ++i1) {
      sprintf(cps," kV[%d] = %f",i1,srbs->kvTabV[i1]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    }
    // Bereich Knotvektor V:
    sprintf(cps," kV von %f bis %f",srbs->kvTabV[0],srbs->kvTabV[i2-1]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  } else if(typ == Typ_SURRU) {
    srv = data;
    sprintf(cps,"RuledSurface %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_SUR);



  //----------------------------------------------------------------
  } else if(typ == Typ_SURRV) {
    srv = data;
    sprintf(cps,"RevolvedSurface %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_SUR);
    sprintf(cps,"SRV Cen-typ=%d ind=%ld",srv->typCen,srv->indCen);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    Cov-typ=%d ind=%ld",srv->typCov,srv->indCov);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    a1=%9.3f a2=%9.3f",srv->ang1,srv->ang2);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    v0=%9.3f v1=%9.3f dir=%d",srv->v0,srv->v1,srv->dir);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
/*
    // display also Centerline and ContourCurve
    i1 = srv->typCen;
    UTO_get_DB (&v1, &i1, &i2, srv->indCen);
    UT3D_stru_dump (i1, v1, "Centerline:\n");
    i1 = srv->typCov;
    UTO_get_DB (&v1, &i1, &i2, srv->indCov);
    UT3D_stru_dump (i1, v1, "ContourCurve:\n");
*/




  //----------------------------------------------------------------
  } else if(typ == Typ_SURSWP) {
    swp = data;
    sprintf(cps,"SweepSurface %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_SUR);
    sprintf(cps,"SWP Path-typ=%d ind=%ld",swp->typPath,swp->indPath);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    Cont-typ=%d ind=%ld",swp->typCov,swp->indCov);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    u0=%9.3lf u1=%9.3lf",swp->u0,swp->u1);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    v0=%9.3lf v1=%9.3lf dir=%d",swp->v0,swp->v1,swp->dir);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  } else if(typ == Typ_SURPLN) {
    // sprintf(cps,"PlanarSurface %s",txt);
    // UT3D_dump_add (sTab, cbuf, ipar, ICO_SUR);
    sprintf(cps," TYP = planar Surface");
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    // typ=Typ_Typ, form=Typ_SURPLN, data ist leer !




  //----------------------------------------------------------------
  } else if(typ == Typ_SURMSH) {
    sprintf(cps,"MeshSurface %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_SUR);
    sprintf(cps," Mesh mTyp=%d",((Mesh*)data)->mTyp);
  // Verts      *verts1;
  // Faces      *faces1;
  // Edges      *edges1;
    if(((Mesh*)data)->p) {  // Verts
      verts1 = ((Mesh*)data)->p;
      sprintf(cps,"   Verts pNr=%d",verts1->pNr);
      for(i1=0; i1<verts1->pNr; ++i1) {
        sprintf(cps,"     pTab[%d] = %9.3f,%9.3f,%9.3f", i1,
                    verts1->pTab[i1].x,
                    verts1->pTab[i1].y,
                    verts1->pTab[i1].z);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
      }
    }
    if(((Mesh*)data)->f) {  // Faces
      faces1 = ((Mesh*)data)->f;
      sprintf(cps,"   Faces fNr=%d fTyp=%d",faces1->fNr,faces1->fTyp);
      for(i1=0; i1<faces1->fNr; ++i1) {
        if(faces1->fTyp == 5) {
          sprintf(cps,"     Fac3[%d] = %6d %6d %6d", i1,
                  ((Fac3*)faces1->fTab)[i1].i1,
                  ((Fac3*)faces1->fTab)[i1].i2,
                  ((Fac3*)faces1->fTab)[i1].i3);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
        }
      }
    }
    if(((Mesh*)data)->e) {  // Edges
      edges1 = ((Mesh*)data)->e;
      sprintf(cps,"   Edges eNr=%d",edges1->eNr);
      for(i1=0; i1<edges1->eNr; ++i1) {
        // if(faces1->fTyp == 5) {
          sprintf(cps," Edg3[%d] = %6d %6d", i1,
                  ((Edg3*)edges1->eTab)[i1].i1,
                  ((Edg3*)edges1->eTab)[i1].i2);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
        // }
      }
    }


  //----------------------------------------------------------------
  } else if(typ == Typ_SURSTRIP) {
    sus = data;
    sprintf(cps,"Stripe %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_SUR);
    sprintf(cps,"  ptUNr=%d ptVNr=%d", sus->ptUNr,sus->ptVNr);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    // Punkte
    ox = sus->pTab;
    for(i1=0; i1<sus->ptVNr; ++i1) {
      i3 = i1 * sus->ptUNr;
      for(i2=0; i2<sus->ptUNr; ++i2) {
        sprintf(sAux, " row %d pt %d ",i1,i2);
        UT3D_dump__ (sTab, ox->form, ox->data, sAux, i2, 0);
        ++ox;
      }
    }


  //----------------------------------------------------------------
  } else if(typ == Typ_Tria) {
    sprintf(cps,"Triangle %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_SUR);
    p1 = ((Triangle*)data)->pa[0];
    sprintf(cps,"    p0=%9.3f,%9.3f,%9.3f",p1->x,p1->y,p1->z);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    p1 = ((Triangle*)data)->pa[1];
    sprintf(cps,"    p1=%9.3f,%9.3f,%9.3f",p1->x,p1->y,p1->z);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    p1 = ((Triangle*)data)->pa[2];
    sprintf(cps,"    p2=%9.3f,%9.3f,%9.3f",p1->x,p1->y,p1->z);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  } else if(typ == Typ_Fac3) {
    sprintf(cps,"Face3 %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_SUR);
    sprintf(cps," Fac3 %6d %6d %6d",
            ((Fac3*)data)->i1,
            ((Fac3*)data)->i2,
            ((Fac3*)data)->i3);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);




  //----------------------------------------------------------------
  } else if(typ == Typ_SPH) {
    sph = data;
    sprintf(cps,"Sphere %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_SOL);
    sprintf(cps," (Sphere).r=%9.3f",sph->rad);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps," (Sphere).pc=%9.3f,%9.3f,%9.3f",sph->pc.x,sph->pc.y,sph->pc.z);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  } else if(typ == Typ_CON) {
    con = data;
    pln = &con->pl;
    sprintf(cps,"Conus %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_SOL);
    sprintf(cps," (Conus).po=%9.3f,%9.3f,%9.3f",pln->po.x,pln->po.y,pln->po.z);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps," (Conus).vz=%9.3f,%9.3f,%9.3f",pln->vz.dx,pln->vz.dy,pln->vz.dz);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps," (Conus).r1=%9.3f r2=%9.3f h=%9.3f",con->r1,con->r2,con->h);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);




  //----------------------------------------------------------------
  } else if(typ == Typ_TOR) {
    tor = data;
    pln = &tor->pl;
    sprintf(cps,"Torus %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_SOL);
    sprintf(cps," (Torus).po=%9.3f,%9.3f,%9.3f",pln->po.x,pln->po.y,pln->po.z);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps," (Torus).vz=%9.3f,%9.3f,%9.3f",pln->vz.dx,pln->vz.dy,pln->vz.dz);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps," (Torus).vx=%9.3f,%9.3f,%9.3f",pln->vx.dx,pln->vx.dy,pln->vx.dz);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps," (Torus).r1=%9.3f r2=%9.3f",tor->r1,tor->r2);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);




  //----------------------------------------------------------------
  } else if(typ == Typ_M3x2) {
    ma = data;
    // Mat_3x2[2][3]; RecLen = 3
    sprintf(cps," M32VX=%9.3f,%9.3f",ma[0],ma[3]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    VY=%9.3f,%9.3f",ma[1],ma[4]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    PT=%9.3f,%9.3f",ma[2],ma[5]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  } else if(typ == Typ_M3x3) {
    ma = data;
    // Mat_3x3[3][3]; RecLen = 3
    sprintf(cps," M33VX=%9.3f,%9.3f,%9.3f",ma[0],ma[3],ma[6]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    VY=%9.3f,%9.3f,%9.3f",ma[1],ma[4],ma[7]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    VZ=%9.3f,%9.3f,%9.3f",ma[2],ma[5],ma[8]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);




  //----------------------------------------------------------------
  } else if(typ == Typ_M4x3) {
    ma = data;
    // Mat_4x3[3][4]; RecLen = 4
    sprintf(cps," M43VX=%9.3f,%9.3f,%9.3f",ma[0],ma[4],ma[8]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    VY=%9.3f,%9.3f,%9.3f",ma[1],ma[5],ma[9]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    VZ=%9.3f,%9.3f,%9.3f",ma[2],ma[6],ma[10]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    PO=%9.3f,%9.3f,%9.3f",ma[3],ma[7],ma[11]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);




  //----------------------------------------------------------------
  // } else if((typ == Typ_M4x4)||(typ == Typ_Tra)) {
  } else if(typ == Typ_M4x4) {
    ma = data;
    // Mat_4x4[4][4]; RecLen = 4
    sprintf(cps," M43VX=%9.3f,%9.3f,%9.3f,%9.3f",ma[0],ma[4],ma[8],ma[12]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    VY=%9.3f,%9.3f,%9.3f,%9.3f",ma[1],ma[5],ma[9],ma[13]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    VZ=%9.3f,%9.3f,%9.3f,%9.3f",ma[2],ma[6],ma[10],ma[14]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    PO=%9.3f,%9.3f,%9.3f,%9.3f",ma[3],ma[7],ma[11],ma[15]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    // sprintf(cps,"    ??=%9.3f,%9.3f,%9.3f,%9.3f",ma[12],ma[13],ma[14],ma[15]);



  //----------------------------------------------------------------
  } else if(typ == Typ_TraRot) {
    ma  = (double*)((TraRot*)data)->ma;
    vc1 = &((TraRot*)data)->vz;
    d1  = ((TraRot*)data)->angr;
    // sprintf(cps," TraRo=%9.3f,%9.3f,%9.3f",ma[3],ma[7],ma[11]);
    sprintf(cps," TraRo=%9.3f", d1);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    vz=%9.3f,%9.3f,%9.3f",vc1->dx,vc1->dy,vc1->dz);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps," ma-VX=%9.3f,%9.3f,%9.3f",ma[0],ma[4],ma[8]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    VY=%9.3f,%9.3f,%9.3f",ma[1],ma[5],ma[9]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    VZ=%9.3f,%9.3f,%9.3f",ma[2],ma[6],ma[10]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    PO=%9.3f,%9.3f,%9.3f",ma[3],ma[7],ma[11]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  } else if(typ == Typ_IndTab) {
    it1 = data;
    sprintf(cps,"IndTab %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps," (IndTab) ibeg=%d inr=%d typi=%d typd=%d aux=%d stat=%d",
            it1->ibeg,it1->iNr,it1->typi,it1->typd,it1->aux,it1->stat);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  } else if(typ == Typ_ObjSRC) {
    os1 = data;
    sprintf(cps,"ObjSRC %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    APED_oid_dbo__ (oNam, os1->typ,os1->dbi);
    sprintf(cps," (ObjSRC) %s  dli=%ld lnr=%ld lev=%d",
            oNam,os1->dli,os1->lnr,os1->iPar);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);


  //----------------------------------------------------------------
  } else if(typ == Typ_ObjDB) {
    odb1 = data;
    sprintf(cps,"ObjDB %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    APED_oid_dbo__ (oNam, odb1->typ,odb1->dbInd);
    sprintf(cps," (ObjDB) %s  dbi =%ld dli=%ld typ=%d stat=%d",
            oNam,odb1->dbInd,odb1->dlInd,odb1->typ,odb1->stat);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);




  //----------------------------------------------------------------
  // ex OX-Record; typ=Typ_Size; form=Typ_Int4; data=(long)
  } else if(typ == Typ_Index) {         // data=ObjGX !
    dbi = LONG_PTR(data);                          //2010-06-04
    sprintf(cps," %s Index = %ld", txt, dbi);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_link);

    // sprintf(cps," Index = %d",(int)ox->data);
    // UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
/*
    ox = data;
    dbi = (long)ox->data;
    APED_oid_dbo__ (oNam, ox->typ, dbi);

    if(mode >= 0) {  // do not output for internal call
      sprintf(cps,"Obj Link %s (ObjGX typ=%d; form=%d siz=%d)",
              oNam, ox->typ,ox->form,ox->siz);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_link);
    }

    i2 = sTab->iNr - 1;  // parentRecordNr

    // get objStruct from dbo
    // irc = UTO_get_dbo (&sTyp, &v1, &i3, ox->typ, dbi);
    sTyp = ox->typ;
    irc = UTO_get_DB (&v1, &i3, &sTyp, dbi);
    if(irc < 0) goto L_err1;

    // objViewer
    UT3D_dump__ (sTab, sTyp, v1, oNam, i2, 0);
*/


  //----------------------------------------------------------------
  } else if((typ == Typ_ObjGX)    ||
            (typ == Typ_CV)       ||
            (typ == Typ_Note)     ||
            (typ == Typ_Tra)      ||
            (typ == Typ_SUR)      ||
            (typ == Typ_SOL))           {
    ox = data;
      // printf(" OGX: typIn=%d ox->typ=%d form=%d siz=%d\n",typ,
             // ox->typ, ox->form, ox->siz);

    // prepare sAux = objDescription  and icon
    if(ox->form == Typ_Index) {
      i1 = ICO_link;
      // dbi = (long)ox->data;
      dbi = LONG_PTR(ox->data);
      APED_oid_dbo__ (oNam, ox->typ, dbi);
      sprintf(sAux, "Link %s", oNam);

    } else {
      if(typ == Typ_SOL)      i1 = ICO_SOL;
      else if(typ == Typ_SUR) i1 = ICO_SUR;
      else if(typ == Typ_CV)  i1 = ICO_CV;
      else                    i1 = ICO_data;
      strcpy(sAux, AP_src_typ__ (ox->typ));
    }

    sprintf(cps,"Obj %s %s (ObjGX typ=%d form=%d siz=%d dir=%d)",txt,sAux,
                   ox->typ, ox->form, ox->siz, ox->dir);
      // printf(" %s\n",cps);


    UT3D_dump_add (sTab, cbuf, ipar, i1);

    i2 = sTab->iNr - 1;  // parentRecordNr


    // loop tru records
    il1 = UTO_siz_stru (ox->form);
    v1 = ox->data;

    for(i1=0; i1<ox->siz; ++i1) {
        // printf(" nxt subStru[%d] %d %d\n",i1,ox->typ,ox->form);
      sprintf(oNam, "[%d]", i1);

      // Typ_Index --------------------
      if(ox->form == Typ_Index) {
          // OGX_SET_INDEX (ox->typ, (long)v1, &oo1); // create a ObjGX
          // UT3D_dump__ (sTab, Typ_Index, &oo1, oNam, i2, -1);
          UT3D_dump__ (sTab, Typ_Index, v1, oNam, i2, 0);


      // Typ_ObjGX --------------------
      } else if(ox->form == Typ_ObjGX) {
          // printf(" subOGX[%d] typ=%d form=%d\n",i1,v1->typ,v1->form);
        UT3D_dump__ (sTab, ox->form, v1, oNam, i2, 0);


      // data-struct --------------------
      } else {
        irc = UT3D_dump__ (sTab, ox->form, v1, "", i2, 0);
        if(irc < 0) return irc;
      }


      v1 = (void*)((char*)v1 + il1);
    }




  //----------------------------------------------------------------
  } else if(typ == Typ_Group)   {     // ObjDB
    sprintf(cps,"GroupObj typ=%d dbi=%ld dli=%ld stat=%d",
            ((ObjDB*)data)->typ, ((ObjDB*)data)->dbInd, ((ObjDB*)data)->dlInd,
            ((ObjDB*)data)->stat);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  } else if(typ == Typ_CVCCV)   {
    cvccv = data;
    // sprintf(cps,"ConcatCurve %s",txt);
    // UT3D_dump_add (sTab, cbuf, ipar, ICO_CV);
    sprintf(cps," (CurvComp%s) typ=%d dbi=%ld ip0=%ld ip1=%ld",txt,
            cvccv->typ, cvccv->dbi, cvccv->ip0, cvccv->ip1);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"   v0=%9.3f on seg %d    v1=%9.3f on seg %d rev=%d",
            cvccv->v0, cvccv->is0,
            cvccv->v1, cvccv->is1, cvccv->rev);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);


/*
  //----------------------------------------------------------------
  } else if(typ == Typ_CVCCV)   {
    cvccv = data;
    // sprintf(cps,"ConcatCurve %s",txt);
    // UT3D_dump_add (sTab, cbuf, ipar, ICO_CV);
    sprintf(cps," (CurvCCV) v0=%9.3f on seg %d    v1=%9.3f on seg %d dir=%d",
            cvccv->v0, cvccv->is0,
            cvccv->v1, cvccv->is1, cvccv->dir);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
*/

  //----------------------------------------------------------------
  } else if((typ == Typ_Model) ||
            (typ == Typ_Mock))     {
    mdr = data;
    sprintf(cps,"ModelReference %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_refM);
    sprintf(cps," (ModelRef).mod=%d scl=%f mnam=%s",
            mdr->modNr,mdr->scl,mdr->mnam);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps," (ModelRef).po=%9.3f,%9.3f,%9.3f",mdr->po.x,mdr->po.y,mdr->po.z);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    sprintf(cps," (ModelRef).vx=%9.3f,%9.3f,%9.3f",mdr->vx.dx,mdr->vx.dy,mdr->vx.dz);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_VC);
    sprintf(cps," (ModelRef).vz=%9.3f,%9.3f,%9.3f",mdr->vz.dx,mdr->vz.dy,mdr->vz.dz);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_VC);

    // // zusaetzlich noch das BasicModel ausdrucken ..
    // mdb = DB_get_ModBas (mdr->modNr);
    // UT3D_stru_dump (Typ_SubModel, mdb, "BasicModel %d:\n",mdr->modNr);



  //----------------------------------------------------------------
  } else if(typ == Typ_SubModel) {
    mdb = data;
    sprintf(cps,"BasicModel %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_natM);
    sprintf(cps," (ModelBas).mnam=%s", mdb->mnam);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps," (ModelBas).typ=%d seq=%d DLind=%4ld DLsiz=%4ld",
            mdb->typ,mdb->seqNr,mdb->DLind,mdb->DLsiz);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps," (ModelBas).po=%9.3f,%9.3f,%9.3f",mdb->po.x,mdb->po.y,mdb->po.z);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);



  //----------------------------------------------------------------
  } else if(typ == Typ_Dimen) {
    dim = data;
    sprintf(cps,"Dimension %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps," DIM a1=%.2f a2=%.2f dtyp=%d hd=%d ld=%d |%s|",
      dim->a1,dim->a2,dim->dtyp,dim->hd,dim->ld,dim->txt);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    p1=%9.3f,%9.3f",dim->p1.x,dim->p1.y);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    p2=%9.3f,%9.3f",dim->p2.x,dim->p2.y);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    p3=%9.3f,%9.3f",dim->p3.x,dim->p3.y);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  } else if(typ == Typ_Dim3) {
    dim3 = data;
    sprintf(cps,"3D-Dimension %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps," DIM3 dtyp=%c bp=%d |%s|",
      dim3->dtyp,dim3->bp,dim3->txt);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    ip1=%ld ip2=%ld ipt=%ld",dim3->ip1,dim3->ip2,dim3->ipt);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    // sprintf(cps,"    p1=%9.3f,%9.3f",dim->p1.x,dim->p1.y);
    // sprintf(cps,"    p2=%9.3f,%9.3f",dim->p2.x,dim->p2.y);
    // sprintf(cps,"    p3=%9.3f,%9.3f",dim->p3.x,dim->p3.y);



  //----------------------------------------------------------------
  } else if(typ == Typ_ATXT) {    // Typ_Tag
    atx = data;
    cp1 = atx->txt;
    sprintf(cps,"Text %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    if(cp1 == NULL) cp1 = (char*)&UT_CHR_NULL;
    sprintf(cps," ATX |%s| typ=%d col=%d ltyp=%d scl=%f",cp1,
      atx->typ,atx->col,atx->ltyp,atx->scl);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    p1=%9.3f,%9.3f,%9.3f",atx->p1.x,atx->p1.y,atx->p1.z);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    p2=%9.3f,%9.3f,%9.3f",atx->p2.x,atx->p2.y,atx->p2.z);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  } else if(typ == Typ_GTXT) {
    gtx = data;
    sprintf(cps,"Graph.Text %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps," GTX dir=%.2f size=%.2f |%s|",
      gtx->dir,gtx->size,gtx->txt);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    pt=%9.3f,%9.3f,%9.3f",gtx->pt.x,gtx->pt.y,gtx->pt.z);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  } else if(typ == Typ_TEXB) {
    btex = data;
    // sprintf(cps,"Texture %s",txt);
    // UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"%s TEXB %s siz=%d/%d, tNr=%d",txt,btex->fnam,
            btex->xSiz,btex->ySiz,btex->texNr);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);


  //----------------------------------------------------------------
  } else if(typ == Typ_TEXR) {
    rtex = data;
    sprintf(cps,"%s TEXR uar=%f udx/y=%f,%f uscx/y=%f,%f ibas=%ld",txt,rtex->uar,
            rtex->udx,rtex->udy,rtex->uscx,rtex->uscy,rtex->ibas);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    px/py=%f,%f ssx/ssy=%f,%f",
            rtex->px,rtex->py,rtex->ssx,rtex->ssy);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    vx=%9.3f,%9.3f,%9.3f",rtex->vx.dx,rtex->vx.dy,rtex->vx.dz);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"    vy=%9.3f,%9.3f,%9.3f",rtex->vy.dx,rtex->vy.dy,rtex->vy.dz);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  } else if(typ == Typ_Activ) {
    i1  = ((Activity*)data)->typ;
    il1 = ((Activity*)data)->ind;
    APED_oid_dbo__ (oNam, i1, il1);
    sprintf(cps," Activity on  %s (%d %ld) %s",oNam, i1, il1,txt);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps," |%s|",((Activity*)data)->data);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);


  //----------------------------------------------------------------
  } else if(typ == Typ_Ltyp) {
    lTyp = (Ind_Att_ln*)data;
    sprintf(cps,"%s Linetype - ind=%d lim=%d",txt,
            lTyp->indAtt, lTyp->lim);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  } else if(typ == Typ_Color) {
    col = (ColRGB*)data;
    sprintf(cps,"%s Color %d %d %d",txt,(unsigned char)col->cr,
                                   (unsigned char)col->cg,
                                   (unsigned char)col->cb);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"       tra=%d sym=%d col=%d tex=%d",
            col->vtra,col->vsym,col->color,col->vtex);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  } else if((typ == Typ_Float8)  ||
            (typ == Typ_Val)     ||
            (typ == Typ_VAR)     ||
            (typ == Typ_Angle)   ||
            (typ == Typ_Par1))         {
    sprintf(cps,"Value %s = %f",txt,*((double*)data));
    UT3D_dump_add (sTab, cbuf, ipar, ICO_Var);



  //----------------------------------------------------------------
  } else if(typ == Typ_ValX) {
    sprintf(cps,"X-Value %s = %f",txt,*((double*)data));
    UT3D_dump_add (sTab, cbuf, ipar, ICO_Var);
  
  //----------------------------------------------------------------
  } else if(typ == Typ_ValY) {
    sprintf(cps,"Y-Value %s = %f",txt,*((double*)data));
    UT3D_dump_add (sTab, cbuf, ipar, ICO_Var);
  
  //----------------------------------------------------------------
  } else if(typ == Typ_ValZ) {
    sprintf(cps,"Z-Value %s = %f",txt,*((double*)data));
    UT3D_dump_add (sTab, cbuf, ipar, ICO_Var);


  
  //----------------------------------------------------------------
  } else if(typ == Typ_XVal) {
    sprintf(cps,"X-Dist. %s = %f",txt,*((double*)data));
    UT3D_dump_add (sTab, cbuf, ipar, ICO_Var);

  //----------------------------------------------------------------
  } else if(typ == Typ_YVal) {
    sprintf(cps,"Y-Dist. %s = %f",txt,*((double*)data));
    UT3D_dump_add (sTab, cbuf, ipar, ICO_Var);

  //----------------------------------------------------------------
  } else if(typ == Typ_ZVal) {
    sprintf(cps,"Z-Dist. %s = %f",txt,*((double*)data));
    UT3D_dump_add (sTab, cbuf, ipar, ICO_Var);




  //----------------------------------------------------------------
  // ex OX-Record; typ=Typ_Size; form=Typ_Int4; data=(long)
  } else if(typ == Typ_Int4) {
    // data (pointer) = int-value itself. 2013-12-20
    sprintf(cps,"Int4 %s = %s (%d)",txt,AP_src_typ__(Typ_Int4),INT_PTR(data));
    // sprintf(cps,"Int4 %s = %s (%d)",txt,AP_src_typ__(Typ_Int4),*(int*)data);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  // typ=Typ_Address; form=Typ_Data; data=(pointer)      2013-11-08
  } else if(typ == Typ_Data) {
    sprintf(cps,"Ptr %p",data);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  } else if((typ == Typ_Txt) ||
            (typ == TYP_FilNam))  {
    sprintf(cps," %s",(char*)data);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);


  //----------------------------------------------------------------
  } else if(typ == Typ_Memspc) {
    sprintf(cps,"  free = %d",UME_ck_free((Memspc*)data));
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"  tot = %d",UME_ck_tot((Memspc*)data));
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);


  //----------------------------------------------------------------
  } else if(typ == Typ_MemTab) {
    sprintf(cps,"  rMax=%d rNr=%d rSiz=%d typ=%d incSiz=%d use=%d",
      ((MemTab*)data)->rMax, ((MemTab*)data)->rNr, ((MemTab*)data)->rSiz,
      ((MemTab*)data)->typ, UTI_round_b2i(((MemTab*)data)->incSiz),
      ((MemTab*)data)->use);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);


  //----------------------------------------------------------------
  // unresolvable objects:
  } else if(typ == Typ_Error)  {
    sprintf(cps,"  %s",(char*)data);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);


  //----------------------------------------------------------------
  } else {
    TX_Error("UT3D_dump__ E001_%d",typ);
    irc = -1;
    goto L_done;
  }

  irc = 0;


  L_done:
    // printf("ex UT3D_dump__\n");

  ERR_RESET ();

  return irc;


  L_err1:
    strcpy(cbuf, "123456 ***** ERROR *****");
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    return -1;

}


//================================================================
  int UT3D_dump_add (TxtTab *sTab, char *cbuf, int ipar, int ico) {
//================================================================
// characters 0-3 = level (ParentRecordNr)
// save ico(Nr) as 2 characters (4-5) in string

  int      irc;
  char     s1[32];


  // characters 0-3 = level (ParentRecordNr)
  // characters 4-5 = IconNr
  sprintf(s1, "%04d%02d",ipar,ico);

  memcpy(cbuf, s1, 6);   // REPLACE first 6 characters

    // printf("UT3D_dump_add  %d |%s|\n",sTab->iNr,cbuf);

  irc = UtxTab_add (sTab, cbuf);
  if(irc < 0) TX_Print("***** UT3D_dump_add UtxTab_add E001-%d",irc);

  return 0;

}


// EOF
