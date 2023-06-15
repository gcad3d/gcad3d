// ut_dump.c   
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
Difference DEB_dump_ox_0 - DEB_dump_obj__

-----------------------------------------------------
Modifications:
2009-11-20 aus ../ut/ut_geo3d.c extrahiert. RF.

-----------------------------------------------------
*/
#ifdef globTag
void DEB(){}
#endif

/*!
\file  ../ut/ut_dump.c  ../ut/ut_deb.h
\brief dump structs, objects 
\code
=====================================================
List_functions_start:

DEB_dump_pt             simple-dump Point
DEB_dump_pt2            simple-dump Point2
DEB_dump_pTab           dump MemTab(Point) + MemTab(char) (point + status)
DEB_dump_nchar          structured (10-groups) of chars (bin-vals)
DEB_dump_nint           structured (10-groups) of int's

DEB_dump_obj__          dump object
DEB_dump_obj_1          dump bin-obj and its pointers, do not resolve.
DEB_dump_nobj__         dump n object's
DEB_dump_nobj_1         dump bin-obj
DEB_dump_add_pt         dump pointCoords formatted into string

DEB_dump_ox_s_          dump obj - structured display
DEB_dump_ox__           dump complex-obj (ObjGX)
// DEB_dump_ox_0           dump complex-obj (ObjGX)
// DEB_dump_ox_1
DEB_dump_ox_s1
DEB_dump_ox_sWri

DEB_dump_dbo            dump DB-object
DEB_dump_IgaTab

DEB_dump_ntxt           dump n object's
DEB_dump_txt            dump formatted text (fprintf)
DEB_dump__              dump object

DEB_dump_file           dump file

DEB_stop                (conditional)stop in gdb                             INLINE
DEB_exit                exit program - display exit-functionName             INLINE
DEB_goto_L_exit         goto Label L_exit - display exit-functionName        INLINE

DEB_halt                internal -used by DEB_stop

List_functions_end:
=====================================================
see also:
DEB_std_file            redirect stdout -> file or back
DEB_prt_init         start/open | close debugfile;
DEB_prt_print        printd = print-formatted in debug-mode                INTERNAL

INF_debug

DBO_dump__
\endcode *//*----------------------------------------



  DEB_dump_obj__ (Typ_PT, &pt1, "indicatedPos=");

  DEB_dump_obj__ (TYP_FuncAdd, fnam, "");    // dump file

  sprintf(cbuf, "\n put out this text ..\n");
  DEB_dump_obj__(Typ_Txt, cbuf, "");

  DEB_dump_txt("Date:          %s",OS_date1());


*/

#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>                    // va_list

#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_gtypes.h"           // AP_src_typ__
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_itmsh.h"            // MSHIG_EDGLN_.. typedef_MemTab.. Fac3
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_txTab.h"            // TxtTab
#include "../ut/ut_cast.h"             // INT__PTR
#include "../ut/ut_err.h"              // ERR_SET1

#include "../ut/func_types.h"          // Typ_Att_hili
#include "../db/ut_DB.h"               // DB_

#include "../xa/xa_ico.h"              // ICO_PT,

#define extern          // invalidates extern (sets "extern" = "")
#include "../ut/ut_deb.h"              // DEB_*
#undef extern           // reset extern ..



// prototypes:
  int DEB_dump_obj__ (int typ, void *data, char *txt, ...);
  int DEB_dump_ox_0 (ObjGX *oxi, char *txt, ...);
  int DEB_dump_ox_1 (ObjGX *oxi, char *txt, ...);


// LOCAL:

// #define tmpSiz 50000       // size in chars for strings
                           // for BSP-Sur's 50000 ! 2011-08-23 


//================================================================
  int DEB_halt () {
//================================================================
// used by INLINE DEB_stop
// needs in gdb-initfile "break DEB_halt"
 
  return 0;

}


/*
//================================================================
  int DEB_mcheck_out (char *fn) {
//================================================================

  char   s1[128];
  printf("DEB_mcheck_out %s\n",fn);

  sprintf(s1, "*** MCHECK-ERROR func %s",fn);
  if(mcheck(0)) AP_debug__ (s1);
  else          printf(" %s - mcheck Ok\n",fn);

  return 0;

}
*/


//====================================================================
  int DEB_dump_nobj__ (int form, int oNr, void *obj, char *txt) {
//====================================================================
/// DEB_dump_ntxt             dump n object's

  int      i1;
  long     il1;
  char     oNam[80];
  void     *vp1;


  printf("============ DEB_dump_nobj__ typ=%d oNr=%d %s\n",form,oNr,txt);

  il1 = UTO_siz_stru (form);

  for(i1=0; i1<oNr; ++i1) {
    sprintf(oNam, " %s[%d]", txt, i1);
    DEB_dump_obj__ (form, obj, oNam);
    // obj += il1;
    obj = (char*)obj + il1;
  }

  return 0;

}


//================================================================
  int DEB_dump_txt (char* txt, ...) {
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

  DEB_dump_obj__ (Typ_Txt, cbuf1, "");

  return 0;

}


//================================================================
  int DEB_dump_dbo (int typ, long dbi, char *txx) {
//================================================================
/// DEB_dump_dbo            dump DB-object

  int     i1, iNr, oSiz;
  char    oid[40],  *data;


  printf("DEB_dump_dbo %d %ld |%s|\n",typ,dbi,txx);

  APED_oid_dbo__  (oid, typ, dbi);

  // get data for dbo
  typ = UTO__dbo ((void**)&data, &iNr, typ, dbi);
  if(typ < Typ_VAR) return-1;

  oSiz = UTO_siz_stru (typ);

  // dump data
  for(i1=0; i1<iNr; ++i1) {
    DEB_dump_obj__ (typ, (void*)data, " %s %s",oid,txx);
    data += oSiz;
  }

  return 0;


}


//===============================================================
  int DEB_dump_obj__ (int typ, void *data, char *txt, ...) {
//===============================================================
/// \code
/// DEB_dump_obj__            testoutput of obj, to stdout or file.
///   TYP_Func* are control-codees; no output !
///
/// typ = TYP_FuncInit:  ab nun Ausgabe -> Datei "txt"  (Open File)
/// typ = TYP_FuncEnd:   ab nun wiederAusgabe -> term.  (Close File)
/// typ = TYP_FuncInit1: ab nun Ausgabe -> FILE "txt"  (File must be open)
/// typ = TYP_FuncExit:  ab nun wiederAusgabe -> term.  (does not close File)
///
/// to dump text-only: use DEB_dump_txt
/// resolv ObjGX: see DEB_dump_ox_0
/// \endcode


static int      DestFlag = 0;   // 0=stdout, 1=file
static FILE     *uo = NULL;

  int        irc, i1, i2, ii;
  char       cbuf[256], *p1;
  // UtxTab_NEW (txTab1);
  TxtTab     txTab1 = _UTXTAB_NUL;
  va_list    va;


  // printf("DEB_dump_obj__ typ=%d txt=|%s|\n",typ,txt);



  //----------------------------------------------------------------
  // Init = Umleitung -> File f alle nachfolgenden Ausgaben
  if(typ == TYP_FuncInit) {
    DestFlag = 1;
    // if(uo) fclose (uo);  GEHT nicht mit stdout; kein reOpen moeglich
    printf("*** DEB_dump_obj__ - open file\n");
    uo = NULL;
    if((uo=fopen(txt,"w")) == NULL) {
        TX_Print("DEB_dump_obj__ open Error |%s|",txt);
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


  //----------------------------------------------------------------
  } else if(typ == Typ_Error) {
    printf(" - ERROR\n");
    return 0;



  }


  //----------------------------------------------------------------
  if(DestFlag == 0) uo = stdout;
  // if(!uo) uo = stdout;
      // printf(" dump_obj__ DestFlag=%d\n",DestFlag);


  va_start(va,txt);
  vsprintf(cbuf,txt,va);
  va_end(va);

    // printf(" _stru_dump cbuf |%s|\n",cbuf);


  // get space for outData
  // p1 = (char*) MEM_alloc_tmp (tmpSiz);   // 50k
  irc = -1;

  UtxTab_init__ (&txTab1);

  // create strings of infos about struct
  i1 = DEB_dump__ (&txTab1, typ, data, cbuf, -1, 0);
  if(i1 < 0) goto L_exit;
    // printf(" DEB_dump_obj__ - nach DEB_dump__\n");
    // UtxTab_dump (&txTab1, "f-DEB_dump__");

  // write txTab1
  for(i1=0; i1<txTab1.iNr; ++i1) {
    p1 = UtxTab__(i1, &txTab1);
    fprintf(uo, "%s\n",&p1[6]);
  }

  fflush(uo);

  irc = 0;

  L_exit:
  UtxTab_free (&txTab1);

    // printf("ex DEB_dump_obj__\n");

  return irc;

}


//=======================================================================
  int DEB_dump_ntxt (TxtTab *sTab, int form, void *obj, int oNr, char *txt,
                  int ipar, int mode) {
//=======================================================================
/// \code
/// create info-textstrings describing binary object form/obj (structured)
/// Input:
///   form                   Typ_ObjGX,Typ_Index,Typ_CV,Typ_SUR,Typ_SOL ..
///   obj                  struct of type <form>
///   oNr                   nr of obj's in <obj> of type <form>
///   txt                   obj.description (should be objName)
///   ipar                  parentRecordNr
///   mode                  0=normal object; create info-textstrings
///                         1=internal object; do not create info-textstrings
/// Output:
///   characters 0-3 = level (ParentRecordNr)
///   characters 4-5 = IconNr
/// \endcode


  int      irc, i1, i2;
  long     il1;
  char     oNam[80];
  void     *vp1;


  // printf("DEB_dump_ntxt form=%d txt=|%s| oNr=%d ipar=%d\n",form,txt,oNr,ipar);


  if(oNr < 2) return DEB_dump__ (sTab, form, obj, txt, ipar, mode);

  sprintf (oNam, "123456 %s %s - %d records", AP_src_typ__(form), txt, oNr);
  UT3D_dump_add (sTab, oNam, ipar, ICO_CV);

  il1 = UTO_siz_stru (form);


  for(i1=0; i1<oNr; ++i1) {

    sprintf(oNam, " %s[%d]", txt, i1);

    irc = DEB_dump__ (sTab, form, obj, oNam, ipar, mode);
    if(irc < 0) return irc;

    // obj += il1;
    obj = (char*)obj + il1;

  }

  return 0;

}


//===============================================================
  int DEB_dump__ (TxtTab *sTab, int typ, void *data, char *iTxt,
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

// DEB_dump__ < DEB_dump_ox_0: typ is ox->form !

// icons see ../gtk/ut_tree_it.h
// ICO_data for binary data,
// ICO_link for links into DB

//see DEB_dump_obj__


  void     *v1;
  int      irc, i1, i2, i3, ptNr, sTyp, *ia;
  long     dbi, il1;
  char     oNam[32], s1[64], s2[64], cbuf[512], txt[320], *cps, *cp1;
  double   d1, *dp;
  Point    *p1, *ptAr;
  Point2   *p2, *pt2Ar;
  // Ray      *lnr;
  CVLn3    *cvLn;
  Line     *ln1;
  Line2    *ln2;
  Circ     *c1;
  Circ2    *c2;
  Circ2C   *c2c;
  Vector   *vc1;
  Vector2  *vc2;
  Vec3f    *vcf;
  CurvElli   *e1;
  CurvEll2   *e2;
  CurvEll2C  *el2c;
  CurvBSpl   *cvbs;
  CurvBSpl2  *cv2bs;
  CurvRBSpl  *cvrbs;
  CurvPsp3   *cvpsp;
  CurvCCV    *cvccv;
  SurBSpl    *sbs;
  SurRBSpl   *srbs;
  SurStripe  *sus;
  Verts      *verts1;
  Faces      *faces1;
  Edges      *edges1;
  CurvPoly   *plg3;
  CurvPol2   *plg2;
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
  IgaTab   *ig1;
  ObjDB    *odb1;
  ObjGX    *ox, *o2, *o3, oo1;
  ModelBas* DB_get_ModBas    ();
  BndSur   *sba; // Typ_SURBND
  wPoint   *wpt;
  CurvPrcv *prcv;
  GridBox  *gb1;
  Mat_4x3  *m43;
  // EdgSur   *esa;



  // printf("DEB_dump__ typ=%d txt=|%s| ipar=%d\n",typ,txt,ipar);
  // printf(" data=%p\n",data);


  if(!data) {
    if((typ != Typ_Int1)  &&
       (typ != Typ_Int4)  &&
       (typ != Typ_Int8)  &&
       (typ != Typ_Index)) {
      // first 6 charaters become replaced in UT3D_dump_add !
      sprintf(cbuf, "123456 %s data = NULL ",AP_src_typ__(typ));
      UT3D_dump_add (sTab, cbuf, ipar, 0);
      return -1;
    }
  }


/*  TODO: do -not use in dll (else no fallback..)
  i1 = ERR_SET1 ();
  if(i1) {
    TX_Print("***** ERROREXIT DEB_dump__ %d *****\n",typ);
    irc = -1;
    goto L_done;
  }
*/


  cps = &cbuf[6];    // size max 512

  // get text with max.length of 240
  i1 = strlen(iTxt);
  if(i1) {
    if(i1 < 240) strcpy(txt, iTxt);
    else {
      strncpy(txt, iTxt, 240);
      txt[240] = '\0';
    }
  } else {
    txt[0] = '\0';
  }

  //----------------------------------------------------------------
  if(typ == Typ_PT2) {
    // p1 = data;
    sprintf(cps,"Point2 %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    //i3=sprintf(cps,"(Point2) %9.3f,%9.3f",((Point2*)data)->x,((Point2*)data)->y);
    i3 = snprintf(cps,50,"(Point2) %9.3f,%9.3f",
                  ((Point2*)data)->x, ((Point2*)data)->y);
    if(i3 >= 50) strcpy (cps, "(Point2) not set ..");
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);



  //----------------------------------------------------------------
  } else if(typ == Typ_PT) {
    p1 = data;
    sprintf(cps,"Point %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    // i3 = sprintf(cps,"(Point) %9.3f,%9.3f,%9.3f",p1->x,p1->y,p1->z);
    i3 = snprintf(cps,60,"(Point) %9.3f,%9.3f,%9.3f",p1->x,p1->y,p1->z);
    if(i3 >= 60) strcpy (cps, "(Point) not set ..");
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);


  //----------------------------------------------------------------
  } else if(typ == Typ_TmpPT) {
    p1 = data;
    sprintf(cps,"tPoint %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    // i3 = sprintf(cps,"(Point) %9.3f,%9.3f,%9.3f",p1->x,p1->y,p1->z);
    i3 = snprintf(cps,60,"(Point) %9.3f,%9.3f,%9.3f",p1->x,p1->y,p1->z);
    if(i3 >= 60) strcpy (cps, "(Point) not set ..");
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);



  //----------------------------------------------------------------
  } else if(typ == Typ_WPT) {
    wpt = data;
    sprintf(cps,"wPoint %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    i3 = snprintf(cps,80,"(wPoint) %9.3f,%9.3f,%9.3f w=%9.3f",
                  wpt->x,wpt->y,wpt->z,wpt->w);
    if(i3 >= 80) strcpy (cps, "(wPoint) not set ..");
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


  //----------------------------------------------------------------
  } else if(typ == Typ_VC3F) {
    vcf = data;
    sprintf(cps,"Vectorf %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_VC);
    sprintf(cps,"(Vectorf) %9.3f,%9.3f,%9.3f",vcf->dx,vcf->dy,vcf->dz);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_VC);


  //----------------------------------------------------------------
  } else if(typ == Typ_CVLN3) {
    cvLn = data;
    sprintf(cps,"cvLn %s lnl = %9.3f",txt,cvLn->lnl);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_CI);
    sprintf(cps,"(cvLn).pt0=%9.3f,%9.3f,%9.3f",cvLn->pt0.x,cvLn->pt0.y,cvLn->pt0.z);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    sprintf(cps,"(cvLn).vcl=%9.3f,%9.3f,%9.3f",cvLn->vcl.dx,cvLn->vcl.dy,cvLn->vcl.dz);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);


  //----------------------------------------------------------------
  } else if(typ == Typ_LN2) {
    ln2 = data;
    sprintf(cps,"2D-Line %s typ = %d",txt,ln2->typ);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_CI);
    sprintf(cps,"(Line2).p1=%9.3f,%9.3f",ln2->p1.x,ln2->p1.y);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    sprintf(cps,"(Line2).p2=%9.3f,%9.3f",ln2->p2.x,ln2->p2.y);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);


  //----------------------------------------------------------------
  } else if(typ == Typ_LN) {
    ln1 = data;
    sprintf(cps,"Line %s ltyp = %d",txt,ln1->typ);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_LN);
    sprintf(cps,"(Line).p1=%9.3f,%9.3f,%9.3f",ln1->p1.x,ln1->p1.y,ln1->p1.z);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    sprintf(cps,"(Line).p2=%9.3f,%9.3f,%9.3f",ln1->p2.x,ln1->p2.y,ln1->p2.z);
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
    sprintf(cps,"2D-CenterEllipse %s sr=%d clo=%d trm=%d",txt,
            el2c->srot,el2c->clo,el2c->trm);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_CV);
    sprintf(cps,"(CurvEll2C).p1=%9.3f,%9.3f",el2c->p1.x,el2c->p1.y);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    sprintf(cps,"(CurvEll2C).p2=%9.3f,%9.3f",el2c->p2.x,el2c->p2.y);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    sprintf(cps,"(CurvEll2C).a=%9.3f  b=%9.3f  angs=%9.3f  ango=%9.3f",
            el2c->a, el2c->b, el2c->angs, el2c->ango);
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
    sprintf(cps,"(CurvEll2).ango=%9.3f sr=%d clo=%d trm=%d",
            e2->ango, e2->srot, e2->clo, e2->trm);
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
    sprintf(cps,"(CurvElli).ango=%9.3f sr=%d clo=%d trm=%d",
            e1->ango, e1->srot, e1->clo, e1->trm);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  } else if(typ == Typ_CVBSP) {
    cvbs = data;
    sprintf(cps,"B-Spline %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_CV);
    sprintf(cps,"  ptNr=%d deg=%d v0=%.6f,v1=%.6f dir=%d clo=%d trm=%d",
            cvbs->ptNr,cvbs->deg,cvbs->v0,cvbs->v1,cvbs->dir,cvbs->clo,cvbs->trm);
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
    sprintf(cps,"  ptNr=%d deg=%d v0=%.6f,v1=%.6f dir=%d clo=%d trm=%d",
            cv2bs->ptNr,cv2bs->deg,cv2bs->v0,cv2bs->v1,
            cv2bs->dir,cv2bs->clo,cv2bs->trm);
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
    sprintf(cps,"    ptNr=%d deg=%d v0=%.6f,v1=%.6f dir=%d clo=%d trm=%d",
            cvrbs->ptNr, cvrbs->deg, cvrbs->v0, cvrbs->v1,
            cvrbs->dir, cvrbs->clo, cvrbs->trm);
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
    plg3 = data;
    sprintf(cps,"Polygon %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_CV);
    sprintf(cps,"(CurvPoly).ptNr=%d v0=%.3f v1=%.3f dir=%d clo=%d trm=%d",
            plg3->ptNr, plg3->v0, plg3->v1, plg3->dir, plg3->clo, plg3->trm);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);

    strcpy (s1, "---");
    for(i1=0; i1<plg3->ptNr; ++i1) {
      // if(i1>50) break;
      if(plg3->lvTab) sprintf(s1, "%.3f", plg3->lvTab[i1]);
      else dp = NULL;
      sprintf(cps," p[%d] = %.3f %.3f %.3f lv=%s",i1,
              plg3->cpTab[i1].x,
              plg3->cpTab[i1].y,
              plg3->cpTab[i1].z,
              s1);                  // lvTab
        // printf(" p[%d] |%s|\n",i1,cps);
      i2 = UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
      if(i2 < 0) break;
    }

  //----------------------------------------------------------------
  } else if(typ == Typ_CVPOL2) {
    plg2 = data;
    sprintf(cps,"2D-Polygon %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_CV);
    sprintf(cps,"(CurvPol2).ptNr=%d", plg2->ptNr);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);

    strcpy (s1, "---");
    for(i1=0; i1<plg2->ptNr; ++i1) {
      // if(plg2->lvTab) sprintf(s1, "%.3f", plg2->lvTab[i1]);
      // else dp = NULL;
      sprintf(cps," p[%d] = %.3f %.3f",i1,
              plg2->pTab[i1].x,
              plg2->pTab[i1].y);
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
  } else if(typ == Typ_CVPSP3) {

    cvpsp = data;

    sprintf(cps,"Polynom.Spline %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_CV);
    sprintf(cps," plyNr=%d v0=%.3f,v1=%.3f",cvpsp->plyNr,cvpsp->v0,cvpsp->v1);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);

    for(i1=0; i1<cvpsp->plyNr; ++i1) {
      pold3 = (polynom_d3*)&cvpsp->plyTab[i1];
      sprintf(cps,"  Polynom_d3 %d u =%9.3f",i1,pold3->u);
      sprintf(s1, " [%d] ",i1);
      DEB_dump__ (sTab, Typ_polynom_d3, pold3, s1, 0, 0);  // recurs
    }


  //----------------------------------------------------------------
  } else if(typ == Typ_polynom_d3) {

    pold3 = data;
    // pold3 = (polynom_d3*)data;

    sprintf(cps,"  Polynom_d3 %s u =%9.3f",txt,pold3->u);
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
  } else if(typ == Typ_Refsys) {
    sprintf(cps,"Refsys %s       bpi = %d bpd = %9.3f",txt,
            ((Refsys*)data)->bpi, ((Refsys*)data)->bpd);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PLN);
    sprintf(s1, " pln");
    DEB_dump__ (sTab, Typ_PLN, ((Refsys*)data)->pln, s1, 0, 0);
    if(((Refsys*)data)->bpi == -2) {
      sprintf(cps," -- undefined --");
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    } else {
    // if(((Refsys*)data)->bpi == -1) {
      sprintf(s1, " mat1");
      DEB_dump__ (sTab, Typ_M4x3, ((Refsys*)data)->mat1, s1, 0, 0);
      sprintf(s1, " mat2");
      DEB_dump__ (sTab, Typ_M4x3, ((Refsys*)data)->mat2, s1, 0, 0);
    }



  //----------------------------------------------------------------
  } else if(typ == Typ_PLN) {
    pln = data;
    sprintf(cps,"Plane %s       p = %9.3f",txt, pln->p);
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
    sprintf(cps,"  BSp-Sur ptUNr=%d ptVNr=%d degU=%d degV=%d",
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
    sprintf(cps,"  ptUNr=%d ptVNr=%d degU=%d degV=%d",
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
    UTO_obj_dbo (&v1, &i1, &i2, srv->indCen);
    DEB_dump_obj__ (i1, v1, "Centerline:\n");
    i1 = srv->typCov;
    UTO_obj_dbo (&v1, &i1, &i2, srv->indCov);
    DEB_dump_obj__ (i1, v1, "ContourCurve:\n");
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
  } else if(typ == Typ_SURPMSH) {
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
        sprintf(s1, " row %d pt %d ",i1,i2);
        DEB_dump__ (sTab, ox->form, ox->data, s1, i2, 0);
        ++ox;
      }
    }


  //----------------------------------------------------------------
  } else if(typ == Typ_Tria) {
    sprintf(cps,"Triang %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_SUR);
    p1 = ((Triang*)data)->pa[0];
    sprintf(cps,"    p0=%9.3f,%9.3f,%9.3f",p1->x,p1->y,p1->z);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    p1 = ((Triang*)data)->pa[1];
    sprintf(cps,"    p1=%9.3f,%9.3f,%9.3f",p1->x,p1->y,p1->z);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    p1 = ((Triang*)data)->pa[2];
    sprintf(cps,"    p2=%9.3f,%9.3f,%9.3f",p1->x,p1->y,p1->z);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  } else if(typ == Typ_Fac3) {
    sprintf(cps,"Face3 %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_SUR);
    sprintf(cps," Fac3 %6d %6d %6d st=%d",
            ((Fac3*)data)->i1,
            ((Fac3*)data)->i2,
            ((Fac3*)data)->i3,
            ((Fac3*)data)->fst0);
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
    sprintf(cps,"(Mat_4x3)  %s", txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps," VX=%9.3f,%9.3f,%9.3f",ma[0],ma[4],ma[8]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps," VY=%9.3f,%9.3f,%9.3f",ma[1],ma[5],ma[9]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps," VZ=%9.3f,%9.3f,%9.3f",ma[2],ma[6],ma[10]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps," PO=%9.3f,%9.3f,%9.3f",ma[3],ma[7],ma[11]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);




  //----------------------------------------------------------------
  // } else if((typ == Typ_M4x4)||(typ == Typ_Tra)) {
  } else if(typ == Typ_M4x4) {
    ma = data;
    // Mat_4x4[4][4]; RecLen = 4
    sprintf(cps," M44VX=%9.3f,%9.3f,%9.3f,%9.3f",ma[0],ma[4],ma[8],ma[12]);
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
  } else if(typ == Typ_IgaTab) {
    ig1 = data;
    sprintf(cps,"IgaTab %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps," (IgaTab) ibeg=%d inr=%d iRef=%d ind=%d typ=%d stat=%d",
            ig1->ibeg,ig1->iNr,ig1->iRef,ig1->ind,ig1->typ,ig1->stat);
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
    dbi = LONG__PTR(data);                          //2010-06-04
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
    irc = UTO_obj_dbo (&v1, &i3, &sTyp, dbi);
    if(irc < 0) goto L_err1;

    // objViewer
    DEB_dump__ (sTab, sTyp, v1, oNam, i2, 0);
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

    // prepare s1 = objDescription  and icon
    if(ox->form == Typ_Index) {
      i1 = ICO_link;
      // dbi = (long)ox->data;
      dbi = LONG__PTR(ox->data);
      APED_oid_dbo__ (oNam, ox->typ, dbi);
      sprintf(s1, "Link %s", oNam);

    } else {
      if(typ == Typ_SOL)      i1 = ICO_SOL;
      else if(typ == Typ_SUR) i1 = ICO_SUR;
      else if(typ == Typ_CV)  i1 = ICO_CV;
      else                    i1 = ICO_data;
      strcpy(s1, AP_src_typ__ (ox->typ));
    }

    sprintf(cps,"Obj %s %s (ObjGX typ=%d form=%d siz=%d dir=%d)",txt,s1,
                   ox->typ, ox->form, ox->siz, ox->dir);
      // printf(" DEB_dump__-ogx-cps |%s|\n",cps);


    UT3D_dump_add (sTab, cbuf, ipar, i1);

    i2 = sTab->iNr - 1;  // parentRecordNr


// TODO: use DEB_dump_ntxt ...............
    il1 = UTO_siz_stru (ox->form);

    v1 = ox->data;                       // 2020-10-17
    if(!v1) {
      strcat (cps, " data = NULL");
      goto L_OK;
    }

//     // if(data == NULL) then datablock is also *ObjGX
//     if(ox->data) v1 = ox->data;
//     else         v1 = &ox[1];       

    // loop tru records
    for(i1=0; i1<ox->siz; ++i1) {
        // printf(" nxt subStru[%d] %d %d\n",i1,ox->typ,ox->form);
      sprintf(oNam, "[%d]", i1);

      // Typ_Index --------------------
      if(ox->form == Typ_Index) {
          // OGX_SET_INDEX (ox->typ, (long)v1, &oo1); // create a ObjGX
          // DEB_dump__ (sTab, Typ_Index, &oo1, oNam, i2, -1);
          DEB_dump__ (sTab, Typ_Index, v1, oNam, i2, 0);


      // Typ_ObjGX --------------------
      } else if(ox->form == Typ_ObjGX) {
          // printf(" subOGX[%d] typ=%d form=%d\n",i1,v1->typ,v1->form);
        DEB_dump__ (sTab, ox->form, v1, oNam, i2, 0);


      // data-struct --------------------
      } else {
        irc = DEB_dump__ (sTab, ox->form, v1, "", i2, 0);
        if(irc < 0) return irc;
      }


      v1 = (void*)((char*)v1 + il1);
    }


  //----------------------------------------------------------------
//   } else if(typ == Typ_ObjTab)   {     // ObjTab see OTB_dump
//     sprintf(cps,"ObjTable nr=%d siz=%d",
//             ((ObjTab*)data)->oNr, ((ObjTab*)data)->oSiz);
//       UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
//       // OTB_dump (data, 1, "");


  //----------------------------------------------------------------
  } else if(typ == Typ_Group)   {     // ObjDB
    sprintf(cps,"GroupObj typ=%d dbi=%ld dli=%ld stat=%d",
            ((ObjDB*)data)->typ, ((ObjDB*)data)->dbInd, ((ObjDB*)data)->dlInd,
            ((ObjDB*)data)->stat);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  } else if(typ == Typ_CVTRM)   {
    cvccv = data;  // CurvCCV
    // sprintf(cps,"ConcatCurve %s",txt);
    // UT3D_dump_add (sTab, cbuf, ipar, ICO_CV);
    sprintf(cps,
      "CurvCCV %s  typ=%d dbi=%ld ip0=%ld ip1=%ld",
      txt, cvccv->typ, cvccv->dbi, cvccv->ip0, cvccv->ip1);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    if(cvccv->v0 == UT_VAL_MAX) strcpy(s1, " -undef- ");
    else sprintf (s1, "%9.3f", cvccv->v0);
    if(cvccv->v1 == UT_VAL_MAX) strcpy(s2, " -undef- ");
    else sprintf (s2, "%9.3f", cvccv->v1);
    sprintf(cps, "   v0 = %s on seg %3d    v1 = %s on seg %3d",
            s1, cvccv->is0, s2, cvccv->is1);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps, "   dir%3d clo%3d trm%3d stat%3d",
            cvccv->dir, cvccv->clo, cvccv->trm, cvccv->stat);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    if(cvccv->ip0) {
      p1 = DB_get_PT (cvccv->ip0);
      sprintf(cps, "     ip0 = Point %9.3f,%9.3f,%9.3f",p1->x,p1->y,p1->z);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    }
    if(cvccv->ip1) {
      p1 = DB_get_PT (cvccv->ip1);
      sprintf(cps, "     ip1 = Point %9.3f,%9.3f,%9.3f",p1->x,p1->y,p1->z);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    }


/*
  //----------------------------------------------------------------
  } else if(typ == Typ_CVTRM)   {
    cvccv = data;
    // sprintf(cps,"ConcatCurve %s",txt);
    // UT3D_dump_add (sTab, cbuf, ipar, ICO_CV);
    sprintf(cps," (CurvCCV) v0=%9.3f on seg %d    v1=%9.3f on seg %d dir=%d",
            cvccv->v0, cvccv->is0,
            cvccv->v1, cvccv->is1, cvccv->dir);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
*/

  //----------------------------------------------------------------
  } else if(typ == Typ_GridBox)   {
    gb1 = data;
    sprintf(cps,"GridBox %s  pMin %f %f %f",txt,
                 gb1->pMin.x, gb1->pMin.y, gb1->pMin.z);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"           ix=%d iy=%d iz=%d",
                 gb1->ix, gb1->iy, gb1->iz);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"           dx=%f dy=%f dz=%f", gb1->dx, gb1->dy, gb1->dz);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);


  //----------------------------------------------------------------
  } else if(typ == Typ_BBox)   {
    sprintf(cps,"BBox %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    p1 = &((BBox*)data)->pb1;
    i3 = sprintf(cps,"(Point-LL) %9.3f,%9.3f,%9.3f",p1->x,p1->y,p1->z);
    if(i3 > 60) strcpy (cps, "(Point-LL) not set ..");
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    p1 = &((BBox*)data)->pb2;
    i3 = sprintf(cps,"(Point-UR) %9.3f,%9.3f,%9.3f",p1->x,p1->y,p1->z);
    if(i3 > 60) strcpy (cps, "(Point-UR) not set ..");
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);


  //----------------------------------------------------------------
  } else if(typ == Typ_BBox2)   {
    sprintf(cps,"BBox2 %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    p2 = &((BBox2*)data)->pb1;
    i3 = sprintf(cps,"(Point-LL) %9.3f,%9.3f",p2->x,p2->y);
    if(i3 > 60) strcpy (cps, "(Point-LL) not set ..");
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    p2 = &((BBox2*)data)->pb2;
    i3 = sprintf(cps,"(Point-UR) %9.3f,%9.3f",p2->x,p2->y);
    if(i3 > 60) strcpy (cps, "(Point-UR) not set ..");
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);



  //----------------------------------------------------------------
  } else if((typ == Typ_Model) ||                  // ModelRef
            (typ == Typ_Mock))     {
    mdr = data;
    sprintf(cps,"ModelReference %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_refM);
    sprintf(cps," (ModelRef).mod=%d scl=%f",
            mdr->modNr,mdr->scl);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps," (ModelRef).po=%12.3f,%12.3f,%12.3f",
            mdr->po.x,mdr->po.y,mdr->po.z);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    sprintf(cps," (ModelRef).vx=%9.3f,%9.3f,%9.3f",
            mdr->vx.dx,mdr->vx.dy,mdr->vx.dz);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_VC);
    sprintf(cps," (ModelRef).vz=%9.3f,%9.3f,%9.3f",
            mdr->vz.dx,mdr->vz.dy,mdr->vz.dz);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_VC);

    // // zusaetzlich noch das BasicModel ausdrucken ..
    // mdb = DB_get_ModBas (mdr->modNr);
    // DEB_dump_obj__ (Typ_SubModel, mdb, "BasicModel %d:\n",mdr->modNr);



  //----------------------------------------------------------------
  } else if(typ == Typ_SubModel) {                     // ModelBas
    mdb = data;
    sprintf(cps,"BasicModel %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_natML);
    sprintf(cps," (ModelBas).mnam=%s", mdb->mnam);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps," (ModelBas).typ=%d DLi=%4ld DLsiz=%d",
            mdb->typ, mdb->DLind, mdb->DLsiz);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    if(UT3D_pt_isFree(&mdb->pb1)) strcpy (cps, " (ModelBas).pb1 not set ..");
    else sprintf(cps," (ModelBas).pb1=%12.3f,%12.3f,%12.3f",
            mdb->pb1.x, mdb->pb1.y, mdb->pb1.z);
    // if(i3 > 60) strcpy (cps, " (ModelBas).pb1 not set ..");
    UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    if(UT3D_pt_isFree(&mdb->pb2)) strcpy (cps, " (ModelBas).pb2 not set ..");
    else sprintf(cps," (ModelBas).pb2=%12.3f,%12.3f,%12.3f",
            mdb->pb2.x, mdb->pb2.y, mdb->pb2.z);
    // if(i3 > 60) strcpy (cps, " (ModelBas).pb2 not set ..");
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
    sprintf(cps," ATX |%s| typ=%d col=%d ltyp=%d scl=%f xSiz=%d ySiz=%d",cp1,
      atx->aTyp,atx->col,atx->ltyp,atx->scl,atx->xSiz,atx->ySiz);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    strcpy (cps,"    p1=");
    DEB_dump_add_pt (cps, &atx->p1, 9, 3);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    strcpy (cps,"    p2=");
    DEB_dump_add_pt (cps, &atx->p2, 9, 3);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  } else if(typ == Typ_GTXT) {
    gtx = data;
    sprintf(cps,"Graph.Text %s",txt);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps," GTX |%s| dir=%.2f size=%.2f xSiz=%d ySiz=%d",
                 gtx->txt, gtx->dir, gtx->size, gtx->xSiz, gtx->ySiz);
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
    sprintf(cps,"%s TEXR uar=%f udx/y=%f,%f uscx/y=%f,%f ibas=%d",txt,rtex->uar,
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


/*
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
*/

  
  //----------------------------------------------------------------
  } else if(typ == Typ_XVal) {
    sprintf(cps,"X-Value %s = %f",txt,*((double*)data));
    UT3D_dump_add (sTab, cbuf, ipar, ICO_Var);

  //----------------------------------------------------------------
  } else if(typ == Typ_YVal) {
    sprintf(cps,"Y-Value %s = %f",txt,*((double*)data));
    UT3D_dump_add (sTab, cbuf, ipar, ICO_Var);

  //----------------------------------------------------------------
  } else if(typ == Typ_ZVal) {
    sprintf(cps,"Z-Value %s = %f",txt,*((double*)data));
    UT3D_dump_add (sTab, cbuf, ipar, ICO_Var);



  //----------------------------------------------------------------
  // polygonal representation curve
  } else if(typ == Typ_PRCV) {
    prcv = data;
    // data (pointer) = int-value itself. 2013-12-20
    sprintf(cps,"%s Prcv typ=%d dbi=%ld mdli=%d siz=%d ptNr=%d spcTyp=%d",txt,
            prcv->typ,prcv->dbi,prcv->mdli,prcv->siz,prcv->ptNr,prcv->spcTyp);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  } else if(typ == Typ_Int1) {
    // data (pointer) = int-value itself. 2013-12-20
    sprintf(cps,"Int1 %s = %s (%d)",txt,AP_src_typ__(Typ_Int1),(int)INT__PTR(data));
    // ia = data;  // 2017-04-28; for MemTab_dump (iTab, "Typ_Int4") - not ObjGX
    // sprintf(cps,"Int4 %s = %s (%d)",txt,AP_src_typ__(Typ_Int4),ia[0]);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  // ex OX-Record; typ=Typ_Size; form=Typ_Int4; data=(long)
  } else if(typ == Typ_Int4) {
    // data (pointer) = int-value itself.
    sprintf(cps,"Int4 %s = %s (%d)",txt,AP_src_typ__(Typ_Int4),(int)INT__PTR(data));
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);

  //----------------------------------------------------------------
  // ex OX-Record; typ=Typ_Size; form=Typ_Int8; data=(long)
  } else if(typ == Typ_Int8) {
    // data (pointer) = long-value itself.
    sprintf(cps,"Int8 %s = %s (%ld)",txt,AP_src_typ__(Typ_Int8),LONG__PTR(data));
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  // typ=Typ_Address; form=Typ_Ptr; data=(pointer)      2013-11-08
  } else if(typ == Typ_Ptr) {
    sprintf(cps,"Ptr %p",data);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);



  //----------------------------------------------------------------
  } else if((typ == Typ_Txt) ||
            (typ == TYP_FilNam))  {
    sprintf(cps," %s",(char*)data);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);


  //----------------------------------------------------------------
  } else if(typ == Typ_Memspc) {
    sprintf(cps,"Memspc %s",txt);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,"  free = %d  tot = %d spcTyp=%d",
            UME_ck_free((Memspc*)data),
            UME_ck_tot((Memspc*)data),
            ((Memspc*)data)->spcTyp);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);


  //----------------------------------------------------------------
  } else if(typ == Typ_MemTab) {
    sprintf(cps,
      " MemTab %s rNr=%d rMax=%d", txt,
      ((MemTab*)data)->rNr,
      ((MemTab*)data)->rMax);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);
    sprintf(cps,
      "   typ=%d rSiz=%d tSiz=%d incSiz=%d spcTyp=%d use=%d",
      ((MemTab*)data)->typ,
      ((MemTab*)data)->rSiz,
      ((MemTab*)data)->tSiz,
      UTI_round_b2i(((MemTab*)data)->incSiz),
      ((MemTab*)data)->spcTyp,
      ((MemTab*)data)->use);
    UT3D_dump_add (sTab, cbuf, ipar, ICO_data);


  //----------------------------------------------------------------
  // unresolvable objects:
  } else if(typ == Typ_Error)  {
    sprintf(cps,"  %s",(char*)data);
      UT3D_dump_add (sTab, cbuf, ipar, ICO_data);


  //----------------------------------------------------------------
  // ignore:
  } else if((typ == Typ_modUndef) ||
            (typ == Typ_unknown)) {

  //----------------------------------------------------------------
  } else {
    TX_Error("DEB_dump__ E001_%d",typ);
    irc = -1;
    goto L_done;
  }


  L_OK:
  irc = 0;


  L_done:
    // printf("ex DEB_dump__\n");

  // ERR_RESET ();

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


  if(sTab->tab == NULL) return -1;


  // characters 0-3 = level (ParentRecordNr)
  // characters 4-5 = IconNr
  sprintf(s1, "%04d%02d",ipar,ico);

  memcpy(cbuf, s1, 6);   // REPLACE first 6 characters

    // printf("UT3D_dump_add  %d |%s|\n",sTab->iNr,cbuf);

  irc = UtxTab_add (sTab, cbuf);
  if(irc < 0) {
    TX_Print("***** UT3D_dump_add UtxTab_add E001-%d",irc);
    return -1;
  }

  return 0;

}


//================================================================
  int DEB_dump_ox__ (void *obj1, char *inf) {
//================================================================
// DEB_dump_ox__          dump single ObjGX

  int irc;

  // printf("DEB_dump_ox__ pos=%ld ---------------- %s\n",(long)obj1,inf);
  printf("DEB_dump_ox__ ---------------- %s\n",inf);

  irc = DEB_dump_nobj_1 (obj1, Typ_ObjGX, 1, 0);

  printf("DEB_dump_ox__ end ------------------------\n");

  return irc;

}


//================================================================
  int DEB_dump_nobj_1 (void *obj1, int typ, int iNr, int deloc) {
//================================================================
// deloc   0=absolut address, 1=delocated (relative adress)

  int    i1, iSiz, sTyp, sForm;
  char   *po, s1[40];
  void   *sPtr;

static void*  relPos;


  // TESTBLOCK
  // printf("DEB_dump_nobj_1 pos=%ld typ=%d iNr=%d deloc=%d\n",(long)obj1,
          // typ,iNr,deloc);
  // END TESTBLOCK

  if(deloc == 0) {
    relPos = obj1;
      // printf(" set relPos %ld\n",(long)relPos);
  }


  iSiz = UTO_siz_stru (typ);
  if(iSiz <= 0) {
    TX_Error("DEB_dump_ox__ E000 %d %d\n",typ,iNr);
    goto L_err_ex;
  }
    // printf(" dump_nobj_1-iNr=%d iSiz=%d\n",iNr,iSiz);



  // save pointerPos
  po = (char*)obj1;


  //----------------------------------------------------------------
  // it is a struct with pointers; 
  // loop tru primary ObjGX-Record(s)
  for(i1=0; i1<iNr; ++i1) {

    //----------------------------------------------------------------
    if(typ == Typ_ObjGX) {

      sTyp  = ((ObjGX*)po)->typ;               // get child-typ
      sForm = ((ObjGX*)po)->form;              // get type of structure of child

      printf(" ObjGX[%d]: typ=%d form=%d siz=%d dir=%d\n",i1,
              ((ObjGX*)po)->typ,
              ((ObjGX*)po)->form,
              ((ObjGX*)po)->siz,
              ((ObjGX*)po)->dir);

        // printf(" i1=%d sTyp=%d sForm=%d\n",i1,sTyp,sForm);

      // ignore all primary ObjGX-Records with no child-data
      if(sTyp  == Typ_Typ)   goto L_GX_nxt;    // hat keine data
      if(sForm == Typ_Index) goto L_GX_nxt;    // hat keine data
      if(sForm == Typ_Int4)  goto L_GX_nxt;    // hat keine data

      sPtr  = ((ObjGX*)po)->data;

//       // set offset
//       if(((ObjGX*)po)->dir == 1)
//         sPtr = MEM_ptr_mov (sPtr, LONG__PTR(relPos));

      // RECURSE
      DEB_dump_nobj_1 (sPtr,
                        ((ObjGX*)po)->form,
                        ((ObjGX*)po)->siz,
                        ((ObjGX*)po)->dir);

    } else {
      sprintf(s1, "[%d]",i1);
      DEB_dump_obj_1 (po, typ, s1);

    }

    L_GX_nxt: // proceed to next ObjGX-ParentRecord ..
    po += iSiz;

  }

  L_fertig:
  return 0;


  L_err_ex:
    return -1;

}


//================================================================
  int DEB_dump_obj_1 (void *obj, int typ, char *txt) {
//================================================================
// deloc   0=absolut address, 1=delocated (relative adress)

  int    i1, iSiz, sTyp, sForm;
  char   s1[256];

  //----------------------------------------------------------------
  if(typ == Typ_PT) {
    // printf("  %ld (Point) %9.3f,%9.3f,%9.3f\n", (long)obj,
    printf("  (Point) %9.3f,%9.3f,%9.3f\n", // (long)obj,
           ((Point*)obj)->x,
           ((Point*)obj)->y,
           ((Point*)obj)->z);


  //----------------------------------------------------------------
  } else if(typ == Typ_CVPOL) {
    // printf("  %ld (CurvPoly) ptNr=%d v0=%.3f v1=%.3f lvTab=%ld cpTab=%ld\n",
    printf("  (CurvPoly) ptNr=%d v0=%.3f v1=%.3f\n",
           // (long)obj,
           ((CurvPoly*)obj)->ptNr,
           ((CurvPoly*)obj)->v0,
           ((CurvPoly*)obj)->v1);
           // (long)((CurvPoly*)obj)->lvTab,
           // (long)((CurvPoly*)obj)->cpTab);


  //----------------------------------------------------------------
  } else if(typ == Typ_GTXT) {
    // printf("  %ld (Graph.Text) %9.3f,%9.3f,%9.3f size=%.2f dir=%.2f\n",
    printf("  (Graph.Text) %9.3f,%9.3f,%9.3f size=%.2f dir=%.2f\n",
           // (long)obj,
           ((GText*)obj)->pt.x,
           ((GText*)obj)->pt.y,
           ((GText*)obj)->pt.z,
           ((GText*)obj)->size,
           ((GText*)obj)->dir);
    printf("   txt=%ld\n",LONG__PTR((void*)((GText*)obj)->txt));
//            (long)((GText*)obj)->txt);


  //----------------------------------------------------------------
  } else {
    sprintf(s1, "%s typ=%d", txt, typ);
    DEB_dump_obj__ (typ, obj, s1, typ);
  }

  return 0;

}


/*
//================================================================
  int UTO_dump_obj (int typ, void *vp, int iNr) {
//================================================================
/// \code
/// typ   form of vp
/// vp    ist eine dem Typ entrrechende struct
/// iNr   nr of structs of typ typ in vp
/// see also DEB_dump_ox_0 UI_dump_obj
/// \endcode


static int iLev;

  int    irc, i0, i1, i2, osiz, *iTab;
  long   l1;
  void   *vp1;
  char   cbuf1[256], cbuf2[64];
  ObjGX  *oxi, *ox1, *ox2, o1;


  if(typ == TYP_FuncInit) {iLev = 0; return 0;}
  ++iLev;


  // printf("UTO_dump_obj typ=%d iNr=%d iLev=%d\n",typ,iNr,iLev);


  // check if its a ObjGX or a struct ...
  if((typ == Typ_ObjGX)  ||                       // oTab
     (typ == Typ_SURRU)  ||                       // 2 x Typ_ObjGX
     (typ == Typ_SURCIR) ||
     (typ == Typ_SURSTRIP))      goto L_ox;       // typ=SURCIR, form=ObjGX


     // (typ == Typ_CVTRM)  ||
     // (typ == Typ_SURRV)  ||                       // 2 x Typ_ObjGX
     // (typ == Typ_GTXT)   ||
     // (typ == Typ_SURBSP) ||// 158 158 hat direkt data !



  // unresolvable objects:
  if((typ == Typ_Error))  {
    DEB_dump_obj__(typ, vp, "");
    goto L_done;
  }


  osiz = UTO_siz_stru (typ);
  if(osiz < 1) return -1;
    // printf(" typ=%d osiz=%d\n",typ,osiz);

  
  // display struct(s)
  for(i1=0; i1<iNr; ++i1) {
    if(iNr > 1) sprintf(cbuf2," o[%d]",i1);
    else  cbuf2[0] = '\0';
    DEB_dump_obj__ (typ, vp, cbuf2);
    // vp = &((char*)vp)[osiz];
    // (char*)vp += osiz;
    vp = (char*)vp + osiz;
  }
  goto L_done;


  //================================================================
  // typ = ObjGX
  // Typ_ObjGX, Typ_CVTRM, Typ_SURCIR sind ObjGX
  L_ox:
  DEB_dump_obj__(Typ_ObjGX, vp, "");

  for(i0=0; i0<iNr; ++i0) {
    // printf(" next i0=%d %d -------------------------\n",i0,iNr);

    if(iNr > 1) {
      sprintf(cbuf2, "------- subObj %d / %d -------",iLev,i0+1);
      DEB_dump_obj__(Typ_Txt, cbuf2, "");
    }

    // osiz = UTO_siz_stru (oxi->form);
    // if(osiz < 1) return -1;

    oxi = &((ObjGX*)vp)[i0];
      // printf(" oxi-typ=%d form=%d siz=%d\n",oxi->typ,oxi->form,oxi->siz);


    if(oxi->form == Typ_ObjGX) {
      // auch data ist ObjGX


      //----------- typ & form = ObjGX -------------------------
      for(i1=0; i1<oxi->siz; ++i1) {
        if(oxi->siz > 1) {
          sprintf(cbuf2, "======= subObj %d / %d =======",iLev,i1+1);
          DEB_dump_obj__(Typ_Txt, cbuf2, "");
        }
        ox1 = &((ObjGX*)oxi->data)[i1];
        DEB_dump_obj__(Typ_ObjGX, ox1, "");

        if(ox1->typ == Typ_Typ) {  // form=surfTyp; zB Typ_SURPLN
          DEB_dump_obj__(ox1->form, ox1, "");
          continue;
        }

        if(ox1->form == Typ_Txt) {  // siz ist bei Typ_Txt die strlen !!
          continue;                 // Beispiel ?
        }

        if(ox1->form == Typ_Index) {
          if(ox1->siz < 2) {
              l1 = (long)ox1->data;
              // printf(" resInd-1 %d typ=%d\n",l1,oxi->typ);
              o1 = DB_GetObjGX (ox1->typ, l1);
              // printf(" o1-typ=%d form=%d siz=%d\n",o1.typ,o1.form,o1.siz);
              irc = UTO_dump_obj (o1.typ, o1.data, o1.siz);  // recurse
              if(irc < 0) return irc;
    
          } else {
            iTab = (int*)ox1->data;
            for(i1=0; i1<ox1->siz; ++i1) {
              l1 = iTab[i1];
              // printf(" resInd %d %d typ=%d\n",i1,l1,oxi->typ);
              o1 = DB_GetObjGX (ox1->typ, l1);
              // printf(" o1-typ=%d form=%d siz=%d\n",o1.typ,o1.form,o1.siz);
              irc = UTO_dump_obj (o1.typ, o1.data, o1.siz);  // recurse
              if(irc < 0) return irc;
            }
          }
          continue;
        }

        irc = UTO_dump_obj (Typ_ObjGX, ox1, 1); // recurse
        // irc = UTO_dump_obj (Typ_ObjGX, ox1, ox1->siz); // recurse
        // das folgende geht ned, weil Typ_Index aufgeloest werden muss ...
        // irc = UTO_dump_obj (ox1->form, ox1->data, ox1->siz); // recurse
        if(irc < 0) return irc;
      }


    } else if(oxi->form == Typ_Index) {
      //----------- typ ObjGX, form = form_of_struct ----------------
      // ACHTUNG: wenn siz=1, dann ist data selbst die zahl; else ist
      // Date eine Int-Tabelle !
      if(oxi->siz < 2) {
          l1 = (long)oxi->data;
          // printf(" resInd-1 %d typ=%d\n",l1,oxi->typ);
          o1 = DB_GetObjGX (oxi->typ, l1);
          irc = UTO_dump_obj (o1.typ, o1.data, o1.siz);  // recurse
          if(irc < 0) return irc;

      } else {
        iTab = (int*)oxi->data;
        for(i1=0; i1<oxi->siz; ++i1) {
          l1 = iTab[i1];
          // printf(" resInd %d %d typ=%d\n",i1,l1,oxi->typ);
          o1 = DB_GetObjGX (oxi->typ, l1);
          irc = UTO_dump_obj (o1.typ, o1.data, o1.siz);  // recurse
          if(irc < 0) return irc;
        }
      }


    } else {
      //----------- typ ObjGX, form = form_of_struct ----------------
      irc = UTO_dump_obj (oxi->form, oxi->data, oxi->siz);
      if(irc < 0) return irc;
    }
  }



  //================================================================
  L_done:
  --iLev;

  // printf("ex UTO_dump_obj %d\n",iLev);
  return 0;

}
*/



//================================================================
  int DEB_dump_IgaTab (MemTab(IgaTab) *oTab) {
//================================================================
/// \code
/// RetCod:
///   0      yes typ/ind is inside a range-obj;
///  -1      no.
/// \endcode

  int    i1;

  printf("======== DEB_dump_IgaTab %d =========== \n",oTab->rNr);

  for(i1=0; i1< oTab->rNr; ++i1) {
    printf("or[%d].typ=%d ibeg=%d iNr=%d\n",i1,
      oTab->data[i1].typ,
      oTab->data[i1].ibeg,
      oTab->data[i1].iNr);

  }

  return 0;

}


//=======================================================================
  int DEB_dump_ox_0 (ObjGX *oxi, char *txt, ...) {
//=======================================================================
/// dump complex-object (Typ_ObjGX)

  int irc;
  void     *v1;
  va_list  va;

  va_start(va,txt);
  v1  = va_arg(va,void*);
  va_end(va);


  printf("DEB_dump_ox_0 start -------------------------- %s\n",txt);
    irc = DEB_dump_ox_1 (oxi, txt, v1); 
  printf("DEB_dump_ox_0 exit  -------------------------------\n");

  return irc;

}


//=======================================================================
  int DEB_dump_ox_1 (ObjGX *oxi, char *txt, ...) {
//=======================================================================


  int      irc, i1, i2, i3, ii, form, typ;
  long     l1, *la;
  char     cbuf[256], *p1, s1[60];
  void     *op1, *vp, *vo;
  va_list  va;
  ObjGX    ox1, *oTab, *o2;



  va_start(va,txt);
  vp  = va_arg(va,void*);
  va_end(va);

  sprintf(cbuf,txt,vp);


  // printf("DEB_dump_ox_1 |%s| typ=%d form=%d siz=%d dir=%d aux=%d\n",cbuf,
             // oxi->typ,oxi->form,oxi->siz,oxi->dir,oxi->aux);


  //----------------------------------------------------------------
  if(oxi->form != Typ_ObjGX) {
    if(oxi->siz < 2) {
      // p1 = AP_src_typ__(oxi->typ);
      DEB_dump_obj__ (oxi->form, oxi->data, "");
    } else {
      p1 = oxi->data;
      l1 = UTO_siz_stru (oxi->form);
      for(i3=0; i3 < oxi->siz; ++i3) {
        sprintf (s1, "[%d]", i3);
        DEB_dump_obj__ (oxi->form, p1, s1);
        p1 += l1;
      }
    }
    return 0;
  }
/*
    p1 = AP_src_typ__(oxi->typ);
    if(oxi->siz < 2) {
      if(oxi->siz < 1) {    // size 0 ???
        return DEB_dump_obj__ (Typ_ObjGX, oxi, p1);
      }
      return DEB_dump_obj__ (oxi->form, oxi->data, p1);
    } else {
      return DEB_dump_nobj__ (oxi->form, oxi->siz, oxi->data, p1);
    }
*/


  //----------------------------------------------------------------
  // resolv ObjGX
  oTab = oxi->data;
  form = oxi->form;

  for(i1=0; i1 < oxi->siz; ++i1) {
    o2 = &oTab[i1];
    printf(" data[%d] typ=%d form=%d siz=%d dir=%d\n",
                  i1, o2->typ,o2->form,o2->siz,o2->dir);
    // bei Typ==Typ_Size ist data die (long)size !

    if(o2->form == Typ_ObjGX) {
      i2 = DEB_dump_ox_1 (o2,"sub");      // recurse !!
      if(i2 < 0) {
        printf("******** DEB_dump_ox_1 E002 %d %d \n",oxi->typ,oxi->form);
        return -1;
      }

    } else if(o2->form == Typ_Index) {
      // if (nr of records == 1): index == data; else its a pointer to long*
      if(o2->siz > 1) {  // data = pointer to long*
// TODO: use Typ_Int8
        la = (long*) (o2->data);   // 2013-12-29 for SURCIR
      } else { // data = index
        // la = &(o2->data);  // OK, but warning ..
        l1 = LONG__PTR (o2->data);
        if(!l1) {
          // dbi=0 - error - undefined
          sprintf(cbuf,"  %s ",AP_src_typ__(o2->typ));
          printf("%s dbi = 0 (undefined)\n",cbuf);
          continue;
        }
        la = &l1;
      }

      ii = 0;
      // printf("    ................ start\n");
      L_ind_n:
          printf(" [%d] = Link to DB-object %c%ld\n",ii,AP_typChar_typ(o2->typ),la[ii]);
        ox1 = DB_GetObjGX (o2->typ, la[ii]);
        i2 = DEB_dump_ox_1 (&ox1,"subInd");
        if(i2 < 0) {
          printf("******** DEB_dump_ox_1 E001 %d %d \n",oxi->typ,oxi->form);
          return -1;
        }
        ++ii;
        if(ii < o2->siz) goto L_ind_n;
      // printf("    ................ exit\n");


    } else if(o2->form == Typ_Txt) {
      break;  // siz ist bei Typ_Txt die strlen !!


    } else {
    if(o2->siz < 2) {
      // p1 = AP_src_typ__(oxi->typ);
      DEB_dump_obj__ (o2->form, o2->data, "");
    } else { 
      p1 = o2->data;
      l1 = UTO_siz_stru (oxi->form);
      for(i3=0; i3 < oxi->siz; ++i3) { 
        sprintf (s1, "[%d]", i3);
        DEB_dump_obj__ (o2->form, p1, s1);
        p1 += l1;
      }
    }
    return 0;

        // printf(" dump_1 typ=%d form=%d siz=%d\n",o2->typ,o2->form,o2->siz);
      // sprintf(cbuf, "O[%d]:",i1);
/*
      if(o2->siz < 2) {
        if(o2->siz < 1) {    // size 0 ???
          DEB_dump_obj__ (Typ_ObjGX, oxi, "");
        }
        DEB_dump_obj__ (o2->form, o2->data, "");
      } else {
        DEB_dump_nobj__ (o2->form, o2->siz, o2->data, "");
      }
*/
    }


  }


  return 0;

}


//=======================================================================
  int DEB_dump_ox_s_ (ObjGX *oxi, char *txt) {
//=======================================================================
/// \code
/// dump complex-obj - structured display
/// see DEB_dump_ox_0 - dump also all subObjects
/// \endcode

  int irc = 0;



  printf("=================== DEB_dump_ox_s_ %s ============ \n",txt);
  DEB_dump_ox_s1 (NULL, txt);         // init

  // display ParentObj
  DEB_dump_ox_sWri (oxi, "");

  if(oxi->form == Typ_ObjGX) {
    irc = DEB_dump_ox_s1 (oxi, txt);  // work
  }

  printf("=================== ex DEB_dump_ox_s_ %s ============ \n",txt);

  return irc;

}


//=======================================================================
  int DEB_dump_ox_sWri (ObjGX *oxi, char *cOff) {
//=======================================================================
/// \code
/// structured display; write Line
/// see DEB_dump_ox_1
/// \endcode

  int   i1;
  long  l1, *iTab;
  char  auxBuf1[64], auxBuf2[64];


  // printf(" .._dump_sWri |%s| typ=%d form=%d siz=%d\n",cOff,
          // oxi->typ,oxi->form,oxi->siz);


  printf("%s typ=%s(%d) form=%s(%d) siz=%d dir=%d",cOff,
                AP_src_typ__(oxi->typ),oxi->typ,
                AP_src_typ__(oxi->form), oxi->form,
                oxi->siz, oxi->dir);

  if(oxi->aux) printf(" aux=%d",oxi->aux);


  //----------------------------------------------------------------
  if(oxi->form == Typ_Index) {

    if(oxi->siz == 1) {
      l1 = LONG__PTR(oxi->data);
      APED_oid_dbo__ (auxBuf1, oxi->typ, l1);
      printf(" (%s)",auxBuf1);


    } else {
// TODO: use Typ_Int8
      iTab = (long*)(oxi->data);
      printf(" (");
      for(i1=0; i1<oxi->siz; ++i1) {
        APED_oid_dbo__ (auxBuf1, oxi->typ, iTab[i1]);
        if(i1 > 0) printf(" ");
        printf("%s",auxBuf1);
      }
      printf(")");
    }


  //----------------------------------------------------------------
  } else if(oxi->form == Typ_Int4) {

      // l1 = LONG__PTR(oxi->data);
      // printf(" value=%ld",l1);
      i1 = INT__PTR(oxi->data);
      printf(" value=%d",i1);
  }

  printf("\n");

  return 0;

}


//=======================================================================
  int DEB_dump_ox_s1 (ObjGX *oxi, char *txt) {
//=======================================================================
/// \code
/// structured display
/// see DEB_dump_ox_1
/// \endcode


static char cOff[64];

  int      i1, i2, i3, typ;
  long     dbi;
  void     *v1;
  char     cbuf[256];
  ObjGX    ox1, *op1, *op2;


  // init cOff
  if(oxi == NULL) {strcpy(cOff,"  "); return 0;}



  // printf("DEB_dump_ox_s1 %s typ=%d form=%d siz=%d dir=%d\n",txt,
                        // oxi->typ,oxi->form,oxi->siz,oxi->dir);


  // display ParentObj
  if(oxi->form != Typ_ObjGX) {
    DEB_dump_ox_sWri (oxi, cOff);     // print obj
    // muss bleiben fuer zB Subrecords of CCV
    return 0;
  }


  // loop tru obj's
  for(i1=0; i1<oxi->siz; ++i1) {
    op1 = &((ObjGX*)oxi->data)[i1];
    if(!op1) continue; // skip
      // printf(" _s1-[%d] %d %d %d\n",i1,op1->typ,op1->form,op1->siz);
    if(!op1) continue;  // skip
    DEB_dump_ox_sWri (op1, cOff);     // print obj

    if(op1->form == Typ_ObjGX) {
      if(strlen(cOff) < 60) strcat(cOff, "  ");
      for(i2=0; i2<op1->siz; ++i2) {
        op2 = &((ObjGX*)op1->data)[i2];
        i3 = DEB_dump_ox_s1 (op2,"");    // recurse
        if(i3 < 0) {
          printf("******** DEB_dump_ox_1 E002 %d %d \n",oxi->typ,oxi->form);
          return -1;
        }
      }
      i2 = strlen(cOff) - 2; if(i2 > 0) cOff[i2] = '\0';


    } else if(op1->form == Typ_Index) {
      // aufloesen ..
      if(op1->siz < 2) {  // Index else (long*)-Tabelle
        dbi = LONG__PTR(op1->data);
          // printf(" res-index-typ=%d dbi=%ld\n",op1->typ,dbi);
        if(!dbi) {
          // dbi=0 - error - undefined
          sprintf(cbuf,"  %s ",AP_src_typ__(oxi->typ));
          printf("%s dbi = 0 (undefined)\n",cbuf);
          continue;
        }
        ox1 = DB_GetObjGX (op1->typ, dbi);
      } else {
// TODO: use Typ_Int8
        ox1 = *op1;
      }

      strcat(cOff, "  ");
      DEB_dump_ox_sWri (&ox1, cOff);     // print obj

      if(ox1.form == Typ_ObjGX) {
        // printf("    ................ start\n");
        if(strlen(cOff) < 60) strcat(cOff, "  ");
        i2 = DEB_dump_ox_s1 (&ox1,"");     // recurse
        if(i2 < 0) {
          printf("******** DEB_dump_ox_1 E001 %d %d \n",oxi->typ,oxi->form);
          return -1;
        }
        // printf("    ................ exit\n");
        i2 = strlen(cOff) - 2; if(i2 > 0) cOff[i2] = '\0';
      }
      i2 = strlen(cOff) - 2; if(i2 > 0) cOff[i2] = '\0';
    }

    // if(op1->form == Typ_Txt) break;  // siz ist bei Typ_Txt die strlen !!

  }


  return 0;

}


//===============================================================================
  int DEB_dump_add_pt (char *so, Point *pt1, int iFmt1, int iFmt2) {
//===============================================================================
// DEB_dump_add_pt      dump pointCoords formatted into string
//   replaces format-overflow by " -undef-"
// Input:
//   iFmt1     nr of digits left of '.'
//   iFmt2     nr of digits right of '.'
//

  int    i1, i4, ii, lMax;
  char   fmt[16], sUndef[80];


  // lMax = iFmt1 + iFmt2 + 1;
  lMax = iFmt1 + 1;
     // printf(" lMax=%d\n",lMax);

  sprintf(fmt, "%%%ds",lMax);
     // printf(" fmt = |%s|\n",fmt);

  sprintf(sUndef, fmt, "-undef-");
     // printf(" sUndef = |%s|\n",sUndef);

  sprintf(fmt, "%%%d.%df",iFmt1,iFmt2);
     // printf(" fmt = |%s|\n",fmt);

  //----------------------------------------------------------------
  i1 = strlen(so);
  ii = sprintf(&so[i1], fmt, pt1->x);
    // printf(" ii=%d\n",ii);
  if(ii > lMax) ii = sprintf(&so[i1], "%s",sUndef);

  strcat(&so[ii], ",");
  ++ii;
  i1 += ii;

  ii = sprintf(&so[i1], fmt, pt1->y);
    // printf(" ii=%d\n",ii);
  if(ii > lMax) ii = sprintf(&so[i1], "%s",sUndef);

  strcat(&so[ii], ",");
  ++ii;
  i1 += ii;

  ii = sprintf(&so[i1], fmt, pt1->z);
    // printf(" ii=%d\n",ii);
  if(ii > lMax) ii = sprintf(&so[i1], "%s",sUndef);


    // printf("ex-DEB_dump_add_pt |%s|\n",so);

  return 0;

}


//================================================================
  int DEB_dump_pt (Point *pt, char *txt, ...) {
//================================================================
// see DEB_dump_obj__

  va_list    va;
  char       cbuf[256];

  
  va_start(va,txt);
  vsprintf(cbuf,txt,va);
  va_end(va);
  
  printf("%s %9.3f,%9.3f,%9.3f\n",cbuf,pt->x,pt->y,pt->z);
  
  return 0;

} 
  
  
//================================================================
  int DEB_dump_pt2 (Point2 *pt2, char *txt, ...) {
//================================================================
// see DEB_dump_obj__

  va_list    va;
  char       cbuf[256];

  
  va_start(va,txt);
  vsprintf(cbuf,txt,va);
  va_end(va);
  
  printf("%s %9.3f,%9.3f\n",cbuf,pt2->x,pt2->y);
  
  return 0;

} 
  
  
//======================================================================
  int DEB_dump_pTab (MemTab(Point) *mpt, MemTab(char) *mps, char *txt) {
//======================================================================
// DEB_dump_pTab           dump MemTab(Point) + MemTab(char) (point + status)
// statusArray mps can be NULL

  int   i1;
  char  *sap;
  Point *pta;


  if(!DEB_STAT) return 0;
  
  printf("========== DEB_dump_pTab %d ========= %s\n",mpt->rNr,txt);
  

  pta = mpt->data;

  if(mps) {
    sap = mps->data;
    for(i1=0; i1<mpt->rNr;  ++i1) {
      printf(" P[%4d] %9.3f %9.3f %9.3f   %3d\n",i1,
             pta[i1].x, pta[i1].y, pta[i1].z, sap[i1]);
    }


  } else {
    for(i1=0; i1<mpt->rNr;  ++i1) {
      printf(" P[%4d] %9.3f %9.3f %9.3f\n",i1,
             pta[i1].x, pta[i1].y, pta[i1].z);
    }
  }


  return 0;
  
} 


//================================================================
  int DEB_dump_nchar (char *ca, int cNr) {
//================================================================
// DEB_dump_nchar         structured (10-groups) of chars (bin-vals)

#define ssnchar \
  "------------0;----1;----2;----3;----4;----5;----6;----7;----8;----9;\n"

  int    i1, i2, i3, ie;

  printf(ssnchar);

  i3 = 0;
  for(i1=0; i1<cNr; i1 += 10) {
    ie = cNr - i1;
    if(ie > 10) ie = 10;
    printf(" [%5d]",i1);
    for(i2=0;i2<ie;++i2) {
      printf(" % 4d;",ca[i1+i2]);
    }
    printf("\n");
    ++i3;
    if(i3 >= 20) {
      printf(ssnchar);
      i3 = 0;
    }
  }
  printf("\n");

  return 0;

}


//================================================================
  int DEB_dump_nint (int *ia, int iNr) {
//================================================================
// DEB_dump_nint          structured (10-groups) of int's
// maxVal is 999999

#define ssnint \
"------|------0;------1;------2;------3;------4;------5;------6;------7;------8;------9;\n"

  int    i1, i2, i3, ie;


  printf(ssnint);

  i3 = 0;
  for(i1=0; i1<iNr; i1 += 10) {
    ie = iNr - i1;
    if(ie > 10) ie = 10;
    printf("%6d|",i1);
    for(i2=0;i2<ie;++i2) {
      printf("% 7d;",ia[i1+i2]);
    }
    printf("\n");
    ++i3;
    if(i3 >= 20) {
      printf(ssnint);
      i3 = 0;
    }
  }
  printf("\n");

  return 0;

}


//================================================================
  int DEB_dump_file (char *fn, char *inf) {
//================================================================
// DEB_dump_file           dump file


  // char     s1[400];
  long        fSiz, cPos1;
  char        *fBuf;


  // printf("DEB_dump_file |%s|%s|\n",fn,inf);


  printf("------------- %s             DEB_dump_file\n",inf);


//   sprintf(s1,"/bin/cat %s > /dev/tty",fn);
//   OS_system (s1);


  // get file -> fBuf
  MEM_alloc_file (&fBuf, &fSiz, fn);          // alloc temp
  UTX_str_file (fBuf, &fSiz, fn);
  if(fSiz < 1) {
    TX_Print("DEB_dump_file E001 |%s|",fn);
    return -1;
  }

  printf("%s\n",fBuf);


  printf("------------- \n");



  return 0;

}

 
// EOF
